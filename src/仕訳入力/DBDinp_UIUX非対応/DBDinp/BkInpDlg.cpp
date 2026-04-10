// BkInpDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dbdinp.h"

#include "input1.h"
#include "external.h"

#include "BkInpDlg.h"

#include "MstRw.h"
#include "SubFnc.h"

#include "DataListCtrl.h"
#include "DinpSub.h"

#include "DBDinpDoc.h"
#include "DBDinpView.h"

// 入力設定項目選択
#include "AutoInp.h"
extern struct _AUTOSEL *pAUTOSEL;

#include "BmnChk.h"
extern CBmnHaniCheck	BmChk;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// フォーク中サイン
extern BOOL	PROGRAM_OK;

/////////////////////////////////////////////////////////////////////////////
// CBkInpDlg ダイアログ

#define BK_DEBT_PN			0
#define BK_CRED_PN			1
#define BK_PN_MAX	BK_CRED_PN+1

static struct _InpCtl BKINP_CTL[BK_PN_MAX] = {
	{IDC_ICSDBEDTCTRL1,0,0,0,-1},	// 借方
	{IDC_ICSDBEDTCTRL2,0,0,0,-1}	// 貸方
};


CBkInpDlg::CBkInpDlg(CWnd* pParent /*=NULL*/)
	: ICSDialog(CBkInpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBkInpDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
	m_nID = IDD;
	m_pParent = pParent;

	m_THIS_DIALOG_OK = FALSE;

	m_NOW_PN = m_PREV_PN = 0;

	//アクセラレータテーブルロード
	m_hAccelTable = ::LoadAccelerators( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
}


//オリジナルダイアログサイズ
void CBkInpDlg::get_originalsize( double& w, double& h )
{
	w = (double)(m_original_rect.right - m_original_rect.left);
	h = (double)(m_original_rect.bottom - m_original_rect.top);
}


// IDC get
struct _InpCtl *CBkInpDlg::getINP_CTL( int pn )
{
	if( pn >= 0 && pn < BK_PN_MAX )
		return( &BKINP_CTL[pn] );
	else
		return( &BKINP_CTL[0] );	//ERROR
}

void CBkInpDlg::set_fastfocus( BOOL modefy_sg, int pn )
{
	m_MODEFY_OK = modefy_sg;
	m_THIS_DIALOG_OK = TRUE;

	PostMessage( WM_RETROFOCUS, pn );
}

void CBkInpDlg::set_nowfocus()
{
	set_focus( get_nowpn() );
}

// カーソルセット
void CBkInpDlg::set_focus( int pn )
{
	if( pn >= 0 )
	{
		PostMessage(WM_RETROFOCUS, pn);
	}
}

LRESULT CBkInpDlg::RetroFocus( WPARAM wParam, LPARAM lParam )
{
	switch( wParam ) {
	case BK_DEBT_PN:
	case BK_CRED_PN:
		c_set( getINP_CTL( wParam )->IDC_X );
		break;
	}

	return 1;
}


// 現在ポジションセット
int CBkInpDlg::set_nowpn( int pn )
{
	m_PREV_PN = m_NOW_PN;
	m_NOW_PN = pn;

#ifdef LATER_CLOSE
	select_color( PREV_PN, OFF );
	select_color( NOW_PN, ON );
#endif

	return( m_PREV_PN );	//直前のポジション
}

// 現在ポジションゲット
int CBkInpDlg::get_nowpn( void )
{
	return( m_NOW_PN );
}

// 直前のポジションゲット
int CBkInpDlg::get_prevpn()
{
	return( m_PREV_PN );
}


void CBkInpDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBkInpDlg)
	DDX_Control(pDX, IDC_ICSDBEDTCTRL1, m_DBedt1);
	DDX_Control(pDX, IDC_ICSDBEDTCTRL2, m_DBedt2);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_ST_DBMN, m_StDbk);
	DDX_Control(pDX, IDC_ST_CBMN, m_StCbk);
}


BOOL CBkInpDlg::Create()
{
	return ICSDialog::Create(m_nID, m_pParent);
}


void CBkInpDlg::PostNcDestroy() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	delete this;
	
//	ICSDialog::PostNcDestroy();
}


void CBkInpDlg::Bkinp_Init( int mode, CDBINPDataRec* data, struct _DATA_LINE* dtline )
{
	m_pData = data;
	m_Dtline = dtline;
	m_Mode = mode;
	VARIANT var;
	char	buf[64];

	DBeditCond( m_Mode );

	_clr_kmketc(&m_DebtSK);
	_clr_kmketc(&m_CredSK);

	if( ! m_Mode ) {
		GetDlgItem(IDC_STATIC1)->SetWindowText("借方部門：");
		GetDlgItem(IDC_STATIC2)->SetWindowText("貸方部門：");
/*
		sprintf_s( tmp, sizeof tmp, "%d", pDBzm->GetBumonMaxDecimal() );
		m_DBedt1.SetMaxval( tmp );
		m_DBedt2.SetMaxval( tmp );
*/
		// 部門名称取得
		m_DebtSK.bmn = m_pData->m_dbmn;
		_set_bmnetc( &m_DebtSK );

		if( m_pData->m_dbmn != -1 ) {
			pDBzm->BumonCodeToStr( buf, sizeof buf, m_pData->m_dbmn );
			var.pbVal = (BYTE*)buf;
		}
		else	var.pbVal = NULL;

		DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

		// 部門名称取得
		m_CredSK.bmn = m_pData->m_cbmn;
		_set_bmnetc( &m_CredSK );

		if( m_pData->m_cbmn != -1 ) {
			pDBzm->BumonCodeToStr( buf, sizeof buf, m_pData->m_cbmn );
			var.pbVal = (BYTE*)buf;
		}
		else	var.pbVal = NULL;

		DBdata_set( this, IDC_ICSDBEDTCTRL2, &var, ICSDBEDT_TYPE_STRING, 0 );
	}
	else {
		GetDlgItem(IDC_STATIC1)->SetWindowText("借方工事：");
		GetDlgItem(IDC_STATIC2)->SetWindowText("貸方工事：");
/*
		sprintf_s( tmp, sizeof tmp, "%d", pDBzm->GetKojiMaxDecimal() );
		m_DBedt1.SetMaxval( tmp );
		m_DBedt2.SetMaxval( tmp );
*/
		// 部門名称取得
		m_DebtSK.kno = m_pData->m_dkno;
		_set_kojietc( &m_DebtSK );

		if( ! m_pData->m_dkno.IsEmpty() ) {
			pDBzm->KojiCodeToStr( buf, sizeof buf, m_pData->m_dkno );
		//	strcpy_s( buf, sizeof buf, m_pData->m_dkno );
			var.pbVal = (BYTE*)buf;
		}
		else	var.pbVal = NULL;

		DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

		// 部門名称取得
		m_CredSK.kno = m_pData->m_ckno;
		_set_kojietc( &m_CredSK );

		if( ! m_pData->m_ckno.IsEmpty() ) {
			pDBzm->KojiCodeToStr( buf, sizeof buf, m_pData->m_ckno );
		//	strcpy_s( buf, sizeof buf, m_pData->m_ckno );
			var.pbVal = (BYTE*)buf;
		}
		else	var.pbVal = NULL;

		DBdata_set( this, IDC_ICSDBEDTCTRL2, &var, ICSDBEDT_TYPE_STRING, 0 );
	}

	SetBmnEtcDisp( BK_DEBT_PN );
	SetBmnEtcDisp( BK_CRED_PN );
}

void CBkInpDlg::DBeditCond( int mode )
{
	CICSDBEDT* pDBed;
	char tmp[128];

	for( int i = 0; i < 2; i++ ) {
		pDBed = (i==0) ? &m_DBedt1 : &m_DBedt2;

		if( ! mode ) {
			pDBed->SetNumeric( TRUE );
			pDBed->SetKana( TRUE );
			pDBed->SetZenkaku( FALSE );
			pDBed->SetAscii( FALSE );
			pDBed->SetAlphabet( FALSE );
			pDBed->SetFormat( FALSE ); // カンマ編集
			pDBed->SetMinval( "0" );
			sprintf_s( tmp, sizeof tmp, "%d", pDBzm->GetBumonMaxDecimal() );
			pDBed->SetMinval( "0" );
			pDBed->SetMaxval( "0" );
			pDBed->SetMaxLen( pDBzm->zvol->bmcol );
			pDBed->SetInverse( TRUE );
		}
		else {
			pDBed->SetNumeric( TRUE );
			pDBed->SetKana( TRUE );
			pDBed->SetZenkaku( FALSE );
			pDBed->SetAscii( TRUE );
			pDBed->SetAlphabet( TRUE );
			pDBed->SetFormat( FALSE ); // カンマ編集
	//		pDBed->SetMinval( "0" );
	//		sprintf_s( tmp, sizeof tmp, "%d", pDBzm->GetKojiMaxDecimal() );
	//		pDBed->SetMaxval( tmp );
			pDBed->SetMinval( "0" );
			pDBed->SetMaxval( "0" );
			pDBed->SetMaxLen( pDBzm->zvol->kjcol );
			pDBed->SetInverse( TRUE );
		}
	}
}


BEGIN_MESSAGE_MAP(CBkInpDlg, ICSDialog)
	//{{AFX_MSG_MAP(CBkInpDlg)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_RETROFOCUS, RetroFocus )
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBkInpDlg メッセージ ハンドラ

BOOL CBkInpDlg::OnInitDialog() 
{
//	ControlInit( 198, 16, -1 );

	ICSDialog::OnInitDialog();
	
	GetWindowRect( &m_original_rect );	//オリジナルのサイズゲット

	m_DBedt1.ImeEndMode( TRUE );
	m_DBedt2.ImeEndMode(TRUE );

	m_DBedt1.EnableDelete(0);
	m_DBedt2.EnableDelete(0);

	m_DBedt1.EnableHome(1);
	m_DBedt2.EnableHome(1);

	m_StDbk.DefaultColor(FALSE);
	m_StCbk.DefaultColor(FALSE);

	ICSDialog::OnInitDialogEX();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}


BEGIN_EVENTSINK_MAP(CBkInpDlg, ICSDialog)
    //{{AFX_EVENTSINK_MAP(CBkInpDlg)
	//}}AFX_EVENTSINK_MAP
	ON_EVENT_RANGE(CBkInpDlg, IDC_ICSDBEDTCTRL1,IDC_ICSDBEDTCTRL2, 3 /* SetFocus */, OnSetFocusIcsdbedtctrl, VTS_I4)
	ON_EVENT_RANGE(CBkInpDlg, IDC_ICSDBEDTCTRL1,IDC_ICSDBEDTCTRL2, 2 /* KillFocus */, OnKillFocusIcsdbedtctrl, VTS_I4 VTS_I4)
	ON_EVENT_RANGE(CBkInpDlg, IDC_ICSDBEDTCTRL1,IDC_ICSDBEDTCTRL2, 1 /* Termination */, OnTerminationIcsdbedtctrl, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT_RANGE(CBkInpDlg, IDC_ICSDBEDTCTRL1,IDC_ICSDBEDTCTRL2, 5, CBkInpDlg::ImeEndCompositionIcsdbedtctrl, VTS_I4 VTS_I2 VTS_BSTR VTS_BSTR)
END_EVENTSINK_MAP()


void CBkInpDlg::OnSetFocusIcsdbedtctrl(UINT ID) 
{
	if( m_THIS_DIALOG_OK == FALSE )
		return;

	int pn;
	pn = (ID == IDC_ICSDBEDTCTRL1) ? BK_DEBT_PN : BK_CRED_PN;

	set_nowpn( pn );

	// ダイアログの高さがないため、表示しない
	SetBmnEtcDisp( pn, TRUE );
}

void CBkInpDlg::OnKillFocusIcsdbedtctrl(UINT ID, long inplen) 
{
	if( m_THIS_DIALOG_OK == FALSE )
		return;

	if( 0 == m_Mode ) {
		TerminationBumon( ID, 0, inplen, 0 );
	}	
	else {
		TerminationKoji( ID, 0, inplen, 0 );
	}
}

void CBkInpDlg::OnTerminationIcsdbedtctrl(UINT ID, long nChar, long inplen, long kst) 
{
	if( m_THIS_DIALOG_OK == FALSE )
		return;

	if( 0 == m_Mode ) {
		TerminationBumon( ID, nChar, inplen, kst );
	}
	else {
		TerminationKoji( ID, nChar, inplen, kst );
	}
}


void CBkInpDlg::ImeEndCompositionIcsdbedtctrl(UINT ID, short nChar, LPCTSTR string, LPCTSTR ystring)
{
	if( 1 == m_Mode ) {
		CheckImeEndKoji( ID, nChar, string );
	}
	else {
		if( CheckImeEndBmon( ID, nChar, string ) == 0 ) {
			TerminationBumon( ID, nChar, 1, 0 );
		}
	}
}


// IME END 処理
int CBkInpDlg::CheckImeEndKoji( UINT ID, long nChar, LPCTSTR string ) 
{
	VARIANT var;

	CString buf = string;
	int		ret = 0;

	if( nChar == VK_TAB || nChar == VK_F12 ) {
		// カナ検索
		if( ! isdigitstr( (char*)string ) ) {
			SelPar par;
			::ZeroMemory( par.kmk_kana, sizeof par.kmk_kana );
			par.sel_pn = SL_KOJIKANA_PN;
			memcpy( par.kmk_kana, string, sizeof par.kmk_kana );

			((CDBbaseView*)m_pParent)->BaseSelectJob( &par );
			ret = 1;
		}
		else {
			var.pbVal = (BYTE*)buf.GetBuffer(64);
			buf.ReleaseBuffer();

			DBdata_set( this, ID, &var, ICSDBEDT_TYPE_STRING, 0 );
		}
	}
	else {
		kana_chk( &buf, buf.GetLength() );

		var.pbVal = (BYTE*)buf.GetBuffer(64);
		buf.ReleaseBuffer();

		DBdata_set( this, ID, &var, ICSDBEDT_TYPE_STRING, 0 );
	}

	return ret;
}


// IME END 処理
int CBkInpDlg::CheckImeEndBmon( UINT ID, long nChar, LPCTSTR string ) 
{
	VARIANT var;

	CString buf = string;
	int		ret = 0;

	if( nChar == VK_TAB || nChar == VK_F12 ) {
		// カナ検索
		if( ! isdigitstr( (char*)string ) ) {
			SelPar par;
			::ZeroMemory( par.kmk_kana, sizeof par.kmk_kana );
			par.sel_pn = SL_BMNKANA_PN;
			memcpy( par.kmk_kana, string, sizeof par.kmk_kana );

			((CDBbaseView*)m_pParent)->BaseSelectJob( &par );
			ret = 1;
		}
		else {
			var.pbVal = (BYTE*)buf.GetBuffer(64);
			buf.ReleaseBuffer();

			DBdata_set( this, ID, &var, ICSDBEDT_TYPE_STRING, 0 );
		}
	}
	else {
		kana_chk( &buf, buf.GetLength() );

		if( !isdigitstr((char*)string) ) {
			ret = 1;
		}
		else {
			var.pbVal = (BYTE*)buf.GetBuffer(64);
			buf.ReleaseBuffer();
	
			DBdata_set( this, ID, &var, ICSDBEDT_TYPE_STRING, 0 );
		}
	}

	return ret;
}


// 部門のターミネーション
void CBkInpDlg::TerminationBumon( UINT ID, long nChar, long inplen, long kst ) 
{
//struct _ItemData check = {0};
//struct _ItemData prev = {0};
int check = -1, prev = -1;
int number, sel_sw, dc_sw;
short len, n;
int bmon, pn, bmnno_input;
long nKey;
VARIANT var;
static int	do_bmncheck = 0;
int set_bmntky = 0;

dc_sw = ( ID == IDC_ICSDBEDTCTRL1) ? 0 : 1;
pn = dc_sw ? BK_CRED_PN : BK_DEBT_PN;

char	tmp[30] = {0}, bf[30] = {0};

	try
	{
//		if( !nChar )	//フォーカスＯＦＦ
//		{
			nKey = (pAUTOSEL->BMNSEL_OPT == OPT_BMNSEL) ? VK_RETURN : VK_TAB;
			bmnno_input = 0;

			sel_sw = 1;
			// 直前のデータを保存
			prev = dc_sw ? m_pData->m_cbmn : m_pData->m_dbmn;

			DBdata_get( this, ID, &var, ICSDBEDT_TYPE_STRING, 0 );
			strcpy_s( bf, sizeof bf, (const char*)var.pbVal );
			len = 0;

			len = strlen( (const char*)bf );
			bmon = -1;

			if( len > 0 ) {
				bmon = atoi( (LPCTSTR)bf );
			}
			check = bmon;

#ifdef _2KT_BSEL	// 10.01 /02 .......................................
			// 画面番号優先のときのみ 2桁 で 画面番号選択
			if( inplen > 0 && (len  > 0 && len <= 2) && nKey == VK_RETURN ) {
				// 選択画面より番号選択
				if( bk_modefy_ok() ) {
					number = atoi( bf );
					CString	bmnname;

					if( ((CDBDinpView*)m_pParent)->bmncode_todata2( m_pData, number, dc_sw, &bmnname ) != FALSE ) {
						bk_select_dsp( dc_sw, FALSE );
						check = (dc_sw) ? m_pData->m_cbmn : m_pData->m_dbmn;
						((CDBDinpView*)m_pParent)->bmnkoji_to_tekiyo( m_pData, &bmnname );
						((CDBDinpView*)m_pParent)->bmneda_update(m_pData, dc_sw);
					}
					else {
						check = dc_sw ? m_pData->m_cbmn : m_pData->m_dbmn;
						Buzzer();
						sel_sw = 0;	// 不正な選択番号を入力
					}
				}
			}
			else if( inplen > 0 ) {
#else
			{
#endif	// ...........................................................
				// 部門を手入力
				n = BMON_LNG-len;
				if( n >= 0 ) {
					bmon = atoi( bf );

					if( pDBzm->IsGokeiBumon( bmon ) ) {
						// 合計部門は入力不可
						PROGRAM_OK = FALSE;
						myICSMessageBox("合計部門は入力できません！",0,0,0,this);
						PROGRAM_OK = TRUE;

						nChar	= -1;	// フォーカス移動なし
						bmon	= prev;
					}
					else {
						if( nChar == VK_RETURN && nKey == VK_RETURN ) {
							bmnno_input = 1;	// 部門No 手入力サインON
							do_bmncheck = 1;
						}
						set_bmntky = 1;
					}
				}
				else {
					bmon = -1;
				}
				check = bmon;
			}
			else {
				// 入力されていない場合、TABでの検索はしない
				if( ! inplen )
					bmnno_input = 1;
			}

			if( ! bk_modefy_ok() )
			{
			//	if( memcmp( check.IP_BMN, getLINE_DATA()->BMON_DATA.IP_BMN, BMON_LNG/2 ) )
				if( check != prev )
				{
					Buzzer();
				}

				pDBzm->BumonCodeToStr( (char*)tmp, sizeof tmp, prev );
				var.pbVal = (BYTE*)tmp;
				DBdata_set( this, ID, &var, ICSDBEDT_TYPE_STRING, 0 );
			}
			else
			{
				if( check == -1 )
				{
					if( dc_sw )	m_pData->m_cbmn = -1;
					else		m_pData->m_dbmn = -1;
				}
				else
				{
					//部門範囲チェック
#ifdef CLOSE
					if( INP_mode != _APPEND ) {
						if( dc_sw )	{
							if( ! BmChk.BmnHaniCheck( check, m_pData->m_cre ) ) {
								check = -1;
							}
						}
						else {
							if( ! BmChk.BmnHaniCheck( check, m_pData->m_dbt ) ) {
								check = -1;
							}
						}
					}
#endif

					if( dc_sw )	m_pData->m_cbmn = check;
					else		m_pData->m_dbmn = check;
				}
				pDBzm->BumonCodeToStr( tmp, sizeof tmp, check );
				if( dc_sw ) m_Dtline->_XXDTA.dd_cbmn = check;
				else		m_Dtline->_XXDTA.dd_dbmn = check;

				var.pbVal = (BYTE*)tmp;
				DBdata_set( this, ID, &var, ICSDBEDT_TYPE_STRING, 0 );

				set_etcdsp( pn );
				if( set_bmntky ) {
					struct _SET_KMKETC sk;
					// 部門
					sk.bmn = check;
					_set_bmnetc( &sk );
					((CDBDinpView*)m_pParent)->bmnkoji_to_tekiyo( m_pData, &sk.etc );
					((CDBDinpView*)m_pParent)->bmneda_update(m_pData, dc_sw);
				}
			}
//			return;
//		}

		if( !nChar ) {
			// 08.30 /04 ... 画面番号優先機能をつけたため、チェックする
			if( do_bmncheck ) {
				do_bmncheck = 0;
				if( ((CDBDinpView*)m_pParent)->BmnCodeChk( this, check, sel_sw, pn ) )
					return;
			}
			return;
		}
		// 合計部門入力は、カーソル再セット
		if( nChar == -1 ) {
			set_focus( pn );
			return;
		}

		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			nChar = VK_F2;

		if( nChar )
		{
			//#* 部門選択 ... 02.27 /02
			nChar = ((CDBbaseView*)m_pParent)->SelBmonDsp( nChar );

			//#* 部門チェック ... 08.28 /02
			if( INP_mode != _APPEND && (nChar == VK_UP || nChar == VK_DOWN) ) {
				if( ((CDBDinpView*)m_pParent)->BmnCodeChk( this, check, sel_sw, pn ) )
					return;
			}
			if( ! nChar )
				return;
		}

		if( bk_modefy_ok() && nChar == VK_DELETE )
		{
			check = -1;
			::ZeroMemory( tmp, sizeof tmp );
			var.pbVal = (BYTE*)tmp;
			DBdata_set( this, ID, &var, ICSDBEDT_TYPE_STRING, 0 );

			if( dc_sw ) {
				m_pData->m_cbmn = -1;
			}
			else {
				m_pData->m_dbmn = -1;
			}
			if( dc_sw ) m_Dtline->_XXDTA.dd_cbmn = -1;
			else		m_Dtline->_XXDTA.dd_dbmn = -1;

			((CDBDinpView*)m_pParent)->bmneda_update(m_pData, dc_sw);
		}

		if( nChar == VK_RETURN || nChar == VK_TAB || nChar == VK_RIGHT || nChar == VK_F3 )
		{
			//#* 部門チェック ... 08.28 /02
			// 画面番号優先のときはチェックしない [08.30 /04]
			if( nChar == VK_RETURN && nKey == VK_TAB ) {
				do_bmncheck = 0;
				if( ((CDBDinpView*)m_pParent)->BmnCodeChk( this, check, sel_sw, pn ) )
					return;
			}

			if( bk_modefy_ok() ) {
				if( nKey == VK_TAB && nChar == nKey && ! bmnno_input ) {
					if( check != -1 ) {
						number = check;
						CString bmnname;

						// 番号が不正なときはブザー
						if( !((CDBDinpView*)m_pParent)->bmncode_todata2( m_pData, number, dc_sw, &bmnname ) ) {
							if( dc_sw ) {
								m_pData->m_cbmn = prev;
							}
							else {
								m_pData->m_dbmn = prev;
							}
							if( inplen > 0 )	Buzzer();
						}
						// 部門再表示
						pDBzm->BumonCodeToStr( (char*)tmp, sizeof tmp, dc_sw ? m_pData->m_cbmn : m_pData->m_dbmn );
						var.pbVal = (BYTE*)tmp;
						DBdata_set( this, ID, &var, ICSDBEDT_TYPE_STRING, 0 );
					//	bmon_select_dsp( dc_sw, FALSE );
					}
				}
			}

			TermWork( dc_sw, nChar );
		}
		else if( nChar == VK_LEFT || nChar == VK_F2 || nChar == VK_UP || nChar == VK_DOWN )
		{
			TermWork( dc_sw, nChar );
		}
		else if( nChar == VK_DELETE )
			TermWork( dc_sw, nChar );
		else
			Buzzer();
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}


// 工事番号のターミネーション
void CBkInpDlg::TerminationKoji( UINT ID, long nChar, long inplen, long kst ) 
{
CString check, prev, kno, zerokno, kjnam;
int number, sel_sw, dc_sw, kno_input;
short len, n;
int pn, digit_sgn;
int set_kjtky = 0;
VARIANT var;
long nKey;
static int	do_knocheck = 0;

dc_sw = ( ID == IDC_ICSDBEDTCTRL1) ? 0 : 1;
pn = dc_sw ? BK_CRED_PN : BK_DEBT_PN;

char	tmp[30] = {0}, bf[30] = {0};

	try
	{
//		if( !nChar )	//フォーカスＯＦＦ
//		{

			// 工事選択 画面番号優先( VK_RETURN )
			nKey = (pAUTOSEL->BMNSEL_OPT == OPT_BMNSEL) ? VK_RETURN : VK_TAB;
			kno_input = 0;

			sel_sw = 1;
			// 直前のデータを保存
			prev = dc_sw ? m_pData->m_ckno : m_pData->m_dkno;

			DBdata_get( this, ID, &var, ICSDBEDT_TYPE_STRING, 0 );
			strcpy_s( bf, sizeof bf, (const char*)var.pbVal );
			len = 0;
			len = strlen( (const char*)bf );

			trim_zero( bf, sizeof bf, strlen( bf ));
			kno.Empty();

			if( len > 0 ) {
				kno = bf;
			}
			check = kno;

#ifdef _2KT_BSEL	// 10.01 /02 .......................................
			digit_sgn = isdigitstr( bf );

			if( inplen > 0 && (len  > 0 && len <= 2) && digit_sgn && nKey == VK_RETURN ) {
				// 選択画面より番号選択
				if( bk_modefy_ok() ) {
					number = atoi( bf );
					if( ((CDBbaseView*)m_pParent)->kojicode_todata( m_pData, &kjnam, number, dc_sw ) != FALSE ) {
						bk_select_dsp( dc_sw, FALSE );
						check = (dc_sw) ? m_pData->m_ckno : m_pData->m_dkno;

						((CDBDinpView*)m_pParent)->bmnkoji_to_tekiyo( &CREC, &kjnam, 1 );
					}
					else {
						check = dc_sw ? m_pData->m_ckno : m_pData->m_dkno;
						Buzzer();
						sel_sw = 0;	// 不正な選択番号を入力
					}
				}
			}
			else if( inplen > 0 && len > 2 ) {
#else
			{
#endif	// ...........................................................

				// 工事を手入力
				n = Voln1->kjcol-len;

				if( n >= 0 ) {
					kno = bf;

					// 選択画面番号を優先時
					if( nChar == VK_RETURN && nKey == VK_RETURN ) {
						kno_input = 1;	// 工事No 手入力サインON
						do_knocheck = 1;
					}
					set_kjtky = 1;
				}
				else {
					kno.Empty();
				}
				check = kno;
			}
			else {
				// 入力されていない場合、TABでの検索はしない
				if( ! inplen )
					kno_input = 1;
				else
					set_kjtky = 1;
			}

			if( ! bk_modefy_ok() )
			{
			//	if( memcmp( check.IP_BMN, getLINE_DATA()->BMON_DATA.IP_BMN, BMON_LNG/2 ) )
				if( check != prev )
				{
					Buzzer();
				}

				pDBzm->KojiCodeToStr( (char*)tmp, sizeof tmp, prev );
				var.pbVal = (BYTE*)tmp;
				DBdata_set( this, ID, &var, ICSDBEDT_TYPE_STRING, 0 );
			}
			else
			{
				if( check.IsEmpty() )
				{
					if( dc_sw )	m_pData->m_ckno.Empty();
					else		m_pData->m_dkno.Empty();
				}
				else
				{
					if( dc_sw )	m_pData->m_ckno = check;
					else		m_pData->m_dkno = check;
				}
				pDBzm->KojiCodeToStr( tmp, sizeof tmp, check );
				if( dc_sw ) m_Dtline->_XXDTA.dd_ckno = check;
				else		m_Dtline->_XXDTA.dd_dkno = check;

				var.pbVal = (BYTE*)tmp;
				DBdata_set( this, ID, &var, ICSDBEDT_TYPE_STRING, 0 );
				// 工事名称
				set_etcdsp( pn );

				if( set_kjtky ) {
					((CDBDinpView*)m_pParent)->m_Seldata.GetKojiName( check, kjnam, FALSE );
					((CDBDinpView*)m_pParent)->bmnkoji_to_tekiyo( m_pData, &kjnam, 1 );
				}
			}
//			return;
//		}

		if( !nChar ) {
			// 08.30 /04 ... 画面番号優先機能をつけたため、チェックする
			if( do_knocheck ) {
				do_knocheck = 0;
				if( ((CDBDinpView*)m_pParent)->KojiCodeChk( this, check, sel_sw, pn ) )
					return;
			}
			return;
		}

		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			nChar = VK_F2;

		if( nChar )
		{
			// 工事選択 
			nChar = ((CDBDinpView*)m_pParent)->SelKojiDsp( nChar );

			// 工事チェック
			if( INP_mode != _APPEND && (nChar == VK_UP || nChar == VK_DOWN) ) {
				if( ((CDBDinpView*)m_pParent)->KojiCodeChk( this, check, sel_sw, pn ) )
					return;
			}
			if( ! nChar )
				return;
		}

		if( bk_modefy_ok() && nChar == VK_DELETE )
		{
			check.Empty();
			::ZeroMemory( tmp, sizeof tmp );
			var.pbVal = (BYTE*)tmp;
			DBdata_set( this, ID, &var, ICSDBEDT_TYPE_STRING, 0 );

			if( dc_sw ) {
				m_pData->m_ckno.Empty();
			}
			else {
				m_pData->m_dkno.Empty();
			}
			if( dc_sw ) m_Dtline->_XXDTA.dd_ckno.Empty();
			else		m_Dtline->_XXDTA.dd_dkno.Empty();
		}

		if( nChar == VK_RETURN || nChar == VK_TAB || nChar == VK_RIGHT || nChar == VK_F3 )
		{
			// 画面番号優先のときはチェックしない [08.30 /04]
			if( nChar == VK_RETURN && nKey == VK_TAB ) {
				do_knocheck = 0;
				if( ((CDBDinpView*)m_pParent)->KojiCodeChk( this, check, sel_sw, pn ) )
					return;
			}

			// 工事番号優先時は、ここで 画面番号より選択( TABキー押された場合 )
			if( nKey == VK_TAB && nChar == nKey && ! kno_input ) {
				if( ! check.IsEmpty() ) {
					digit_sgn = isdigitstr( (char*)(LPCTSTR)check );
					if( digit_sgn ) {
						number = atoi( check );

						// 番号が不正なときはブザー
						if( ! ((CDBbaseView*)m_pParent)->kojicode_todata( &CREC, &kjnam, number, dc_sw ) ) {
							if( dc_sw ) {
								m_pData->m_ckno = prev;
							}
							else {
								m_pData->m_dkno = prev;
							}
							Buzzer();
						}
						else {
							((CDBDinpView*)m_pParent)->bmnkoji_to_tekiyo( &CREC, &kjnam, 1 );
						}
						// 工事名称再表示
						pDBzm->KojiCodeToStr( tmp, sizeof tmp, prev );
						var.pbVal = (BYTE*)tmp;
						DBdata_set( this, ID, &var, ICSDBEDT_TYPE_STRING, 0 );
					}
				}
			}

			TermWork( dc_sw, nChar );
		}
		else if( nChar == VK_LEFT || nChar == VK_F2 || nChar == VK_UP || nChar == VK_DOWN )
		{
			TermWork( dc_sw, nChar );
		}
		else if( nChar == VK_DELETE )
			TermWork( dc_sw, nChar );
		else
			Buzzer();
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return;
	}
}



void CBkInpDlg::set_etcdsp( int pn )
{
struct _DATA_LINE *data;
	data = m_Dtline;

	int setpn;

	if( !m_Mode )
		setpn = (pn == BK_DEBT_PN) ? BMON_PN : CBMN_PN;
	else
		setpn = (pn == BK_DEBT_PN) ? KOJI_PN : CKJI_PN;

	_set_bmndsp( data, setpn );

	if( ! m_Mode ) {
		if( pn == BK_DEBT_PN ) {
			m_DebtSK.etc = data->_XXDTA.dd_dbmn_etc;
			m_DebtSK.etc_col = data->LINE_CND[setpn].ETC_TXT_ForeCOL;
			m_DebtSK.bmn = data->_XXDTA.dd_dbmn;
		}
		else {
			m_CredSK.etc = data->_XXDTA.dd_cbmn_etc;
			m_CredSK.etc_col = data->LINE_CND[setpn].ETC_TXT_ForeCOL;
			m_CredSK.bmn = data->_XXDTA.dd_dbmn;
		}
	}
	else {
		if( pn == BK_DEBT_PN ) {
			m_DebtSK.etc = data->_XXDTA.dd_dkno_etc;
			m_DebtSK.etc_col = data->LINE_CND[setpn].ETC_TXT_ForeCOL;
			m_DebtSK.kno = data->_XXDTA.dd_dkno;
		}
		else {
			m_CredSK.etc = data->_XXDTA.dd_ckno_etc;
			m_CredSK.etc_col = data->LINE_CND[setpn].ETC_TXT_ForeCOL;
			m_CredSK.kno = data->_XXDTA.dd_ckno;
		}
	}

	SetBmnEtcDisp( pn );
}


// 部門・工事 を表示
void CBkInpDlg::bk_select_dsp( int dc_sw, BOOL bTerm )
{
	int pn;
	pn = (dc_sw) ? BK_CRED_PN : BK_DEBT_PN;

	char	tmp[40] = {0};
	VARIANT var;
	if( ! GetMode() ) {
		pDBzm->BumonCodeToStr( tmp, sizeof tmp, dc_sw ? m_pData->m_cbmn : m_pData->m_dbmn );
	}
	else {
		pDBzm->KojiCodeToStr( tmp, sizeof tmp, dc_sw ? m_pData->m_ckno : m_pData->m_dkno );
	}

	var.pbVal = (BYTE*)tmp;
	DBdata_set( this, getINP_CTL( pn )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

	if( bTerm ) {

#ifdef	_ICSPEN_
		if( flgPen != FALSE )
		{
			//選択後次へ移動
			GetDlgItem( getINP_CTL( get_nowpn() )->IDC_X )->PostMessage( WM_KEYDOWN, VK_TAB );
			return;
		}
		else
		{
			//データセットのためのダミーフォーカスＯＦＦ
			if( ! GetMode() ) {
				TerminationBumon( getINP_CTL( pn )->IDC_X, 0, 0, 0 );
			}
			else {
				TerminationKoji( getINP_CTL( pn )->IDC_X, 0, 0, 0 );
			}
		}
#else
		//データセットのためのダミーフォーカスＯＦＦ
		TerminationBumon( dc_sw, 0, 0, 0 );
#endif
	}
}


// ターミネーション処理
void CBkInpDlg::TermWork( int dc_sw, long nChar )
{
	if( nChar == VK_UP || nChar == VK_DOWN ) {
		to_parent( nChar );
	}
	else {
		switch( nChar ) {
		case VK_RETURN : case VK_TAB : case VK_RIGHT :
		case VK_DELETE :
			if( ! dc_sw ) {
				set_focus( BK_CRED_PN );
			}
			else {
				this->SetFocus();
				to_parent( nChar );
			}
			break;
		case VK_F2:
		case VK_LEFT:
			if( ! dc_sw ) {
				this->SetFocus();
				to_parent( nChar );
			}
			else {
				set_focus( BK_DEBT_PN );
			}
			break;
		}
	}
}


void CBkInpDlg::to_parent( long nChar )
{
	m_pParent->SendMessage( WM_BKINP_END, nChar );
}


void CBkInpDlg::SelectedIcsselctrl( short position, long Number, int place )
{
	if( place ) {
		int pn = get_nowpn();
		int dcsw  = ((pn == BK_DEBT_PN) ? 0 : 1 );
		if( ! m_Mode ) {
			CString bmnname;
			if( ((CDBDinpView*)m_pParent)->bmncode_todata2( &CREC, Number, dcsw, &bmnname ) != FALSE ) {
				((CDBDinpView*)m_pParent)->bmnkoji_to_tekiyo( &CREC, &bmnname );
				((CDBDinpView*)m_pParent)->bmneda_update(&CREC, dcsw);
				bk_select_dsp( dcsw, TRUE );
			}
		}
		else {
			CString kjnam;

			if( ((CDBbaseView*)m_pParent)->kojicode_todata( &CREC, &kjnam, Number, dcsw ) != FALSE ) {
				((CDBDinpView*)m_pParent)->bmnkoji_to_tekiyo( &CREC, &kjnam, 1 );
				bk_select_dsp( dcsw, TRUE );
			}
		}
	}
}

BOOL CBkInpDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( (m_hAccelTable != NULL) && ::TranslateAccelerator( m_hWnd, m_hAccelTable, pMsg))
	{
		return TRUE;
	}
	
	return ICSDialog::PreTranslateMessage(pMsg);
}


//--------------------------------------------
//	部門名称の表示
//		BOOL bHide: TRUE で 表示しない
//--------------------------------------------
void CBkInpDlg::SetBmnEtcDisp( int pn, BOOL bHide/*=FALSE*/ )
{
	BOOL bDisp;
	bDisp = ((pAUTOSEL->BMNDSP_OPT == OPT_BMNDSP) ? TRUE : FALSE);

	if( bHide )	bDisp = FALSE;

	if( pn == BK_DEBT_PN ) {
		if( bDisp ) 
			_dsp_etcwnd( &m_DebtSK, this, &m_StDbk, IDC_ICSDBEDTCTRL1, TRUE );
		else
			m_StDbk.ShowWindow(SW_HIDE);
	}
	else {
		if( bDisp ) 
			_dsp_etcwnd( &m_CredSK, this, &m_StCbk, IDC_ICSDBEDTCTRL2, TRUE );
		else
			m_StCbk.ShowWindow(SW_HIDE);
	}
}


//-------------------------------------------
//	部門工事のデータ 修正OKか？
//	return TRUE: 修正OK
//
//-------------------------------------------
BOOL CBkInpDlg::bk_modefy_ok()
{
	return ((CDBDinpView*)m_pParent)->dinp_modefy_ok( m_Dtline );

}


//--------------------------------------
//	F2 JOB
//
//--------------------------------------
BOOL CBkInpDlg::bk_F2job()
{
	if( get_nowpn() == BK_CRED_PN ) {
		set_focus( BK_DEBT_PN );
		return TRUE;
	}
	return FALSE;
}


