// SkbnIn.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"

#include "../DBDinp/ocxdef.h"
#include "resource.h"
#include "DBInpSubFunc.h"

#include "SyzInp.h"
#include "SkbnIn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkbnIn ダイアログ

//#define PN_MAX	SY_TAIKA_PN+1
//#define PN_MAX		SY_TOKUS_PN+1
#define PN_MAX			SY_MENWARI_PN + 1	// インボイス対応 + 1	// インボイス対応

#define TAIKA_VER

static struct _InpCtl INP_CTL2[PN_MAX] = {
	{IDC_ICSDBEDTCTRL1,0,0,0,-1},	// 消費税仕訳区分
	{IDC_ICSDBEDTCTRL2,0,0,0,-1},	// 税率
	{IDC_ICSDBEDTCTRL3,0,0,0,-1},	// 税区分
	{IDC_ICSDBEDTCTRL4,0,0,0,-1},	// 売上／仕入 区分
	{IDC_ICSDBEDTCTRL5,0,0,0,-1},	// 取得・譲渡
	{IDC_ICSDBEDTCTRL6,0,0,0,-1},	// 対価金額
	{IDC_ICSDBEDT_TOKU,0,0,0,-1},	// 特定収入
	{IDC_ICSDBEDT_INV,0,0,0,-1},	// インボイス対応 登録番号
	{IDC_CHECK_MENZEI,0,0,0,-1},	// インボイス対応 免税事業者からの課税仕入れ
	{IDC_ICSDBEDT_MEN,0,0,0,-1},	// インボイス対応 登録番号
};


static int STATIC_CTL[PN_MAX] = {
	IDC_STATIC1,	IDC_STATIC2,	IDC_STATIC3,
	IDC_STATIC4,	IDC_STATIC5,	IDC_STATIC6,
	IDC_STATIC8,	IDC_STATIC18,	IDC_STATIC20, -1,
};

// Now ポジション
static int NOW_PN = 0;
//直前のポジション
static int PREV_PN = 0;

// 項目移動順
static int PN_ORDER[] = {
	SY_SYZSWKBN_PN, SY_ZRITU_PN, SY_SZKBN_PN,
	SY_TOKUS_PN, SY_URISIRE_PN, SY_TAISEL_PN,
	SY_TAIKA_PN,  SY_INV_PN, SY_MENZEI_PN, SY_MENWARI_PN, -1
};

CSkbnIn::CSkbnIn( CWnd* pParent /*=NULL*/, BOOL bToku /*=FALSE*/, BOOL bResize/*=TRUE*/, bool bInvMaster/*=false*/, bool bInvUse/*=falase*/, 
								bool bInvDisp/*=false*/, int inpMode/*=INPMODE_NORMAL*/, int koujo8050/*=SYZ_MEN_NOT*/)
	: ICSDialog(CSkbnIn::IDD, pParent)
{
	DBGFUNCTION;
	//{{AFX_DATA_INIT(CSkbnIn)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
	m_THIS_DIALOG_OK = FALSE;
	m_pParent	= pParent;
	MyTrace("〇〇〇 CSkbnIn m_pParent = %08x(%08x)\n〇〇〇 ", m_pParent, m_pParent->m_hWnd );

	m_bToku		= bToku;
	m_bResize	= bResize;
	m_Dispflg = FALSE;
	m_DlgRect.SetRectEmpty();

	m_defSwkKbn = 0;

	// インボイス対応
	m_bInvMaster = bInvMaster;
	m_bInvUse = bInvUse;
	m_bInvDisp = bInvDisp;
	m_InpMode = inpMode;
	m_Koujo80_50 = koujo8050;
	m_bMsgDsp = FALSE;
	m_bIgnoreFocus = FALSE;

	m_brBk.CreateSolidBrush(RGB_YELLOW);	// 背景(黄)
	m_brDefBk.CreateSolidBrush(::GetSysColor(COLOR_3DFACE));

	Create( CSkbnIn::IDD, pParent );

	//アクセラレータテーブルロード
	m_hAccelTable = ::LoadAccelerators( AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));

//	MyTrace("CSkbnIn ctor m_bInvMaster(%d) m_bInvUse (%d) m_bInvDisp =(%d)\n", m_bInvMaster, m_bInvUse, m_bInvDisp);
}


void CSkbnIn::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSkbnIn)
	// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	DDX_Control(pDX, IDC_ICSDBEDTCTRL1, m_DBedt1);
	DDX_Control(pDX, IDC_ICSDBEDTCTRL2, m_DBedt2);
	DDX_Control(pDX, IDC_ICSDBEDTCTRL3, m_DBedt3);
	DDX_Control(pDX, IDC_ICSDBEDTCTRL4, m_DBedt4);
	DDX_Control(pDX, IDC_ICSDBEDTCTRL5, m_DBedt5);
	DDX_Control(pDX, IDC_ICSDBEDTCTRL6, m_DBedt6);
	DDX_Control(pDX, IDC_ICSDBEDT_TOKU, m_DBedtToku);
	DDX_Control(pDX, IDC_ICSDBEDT_INV, m_DBedtInv);
	DDX_Control(pDX, IDC_CHECK_MENZEI, m_MenzeiCheck);
	DDX_Control(pDX, IDC_ICSDBEDT_MEN, m_MenWari);
	DDX_Control(pDX, IDC_ICSDBEDT_INVDISP, m_DBedtInvDisp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSkbnIn, ICSDialog)
	//{{AFX_MSG_MAP(CSkbnIn)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_RETROFOCUS, RetroFocus)
	ON_MESSAGE(WM_MYMESSAGE, OnMyMessage)
	ON_BN_CLICKED(IDC_CHECK_MENZEI, &CSkbnIn::OnBnClickedMenzei)
	ON_BN_SETFOCUS(IDC_CHECK_MENZEI, &CSkbnIn::OnSetfocusCheckMenzei)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// IDC get
struct _InpCtl *CSkbnIn::getINP_CTL( int pn )
{
//	DBGFUNCTION;
//	TRACE("%s:pn = %d\n", __FUNCTION__, pn );

	if( pn >= 0 && pn < PN_MAX )
		return( &INP_CTL2[pn] );
	else
		return( &INP_CTL2[0] );	//ERROR
}

void CSkbnIn::set_fastfocus( BOOL modefy_sg, int pn )
{
	DBGFUNCTION;

	m_MODEFY_OK = modefy_sg;
	m_THIS_DIALOG_OK = TRUE;

	PostMessage( WM_RETROFOCUS, pn );

	for (int n = 0; n < PN_MAX; n++) {
		if( GetDlgItem(STATIC_CTL[n]) ) {
			GetDlgItem(STATIC_CTL[n])->Invalidate();
		}
	}
}

void CSkbnIn::set_nowfocus()
{
	set_focus( get_nowpn() );
	RedrawWindow();
}

// カーソルセット
void CSkbnIn::set_focus( int pn )
{
	if( pn >= 0 )
	{
		PostMessage(WM_RETROFOCUS, pn);
	}
}


LRESULT CSkbnIn::RetroFocus( WPARAM wParam, LPARAM lParam )
{
	switch( wParam ) {
	case SY_SYZSWKBN_PN:
	case SY_ZRITU_PN:
	case SY_SZKBN_PN:
	case SY_URISIRE_PN:
	case SY_TAISEL_PN:
	case SY_TAIKA_PN:
	case SY_TOKUS_PN:
	case SY_INV_PN:
	case SY_MENZEI_PN:
	case SY_MENWARI_PN:
		c_set( getINP_CTL( wParam )->IDC_X );
		break;
	}

	return 1;
}


/*================================================================
	仕訳欄の項目移動による消費税ダイアログ表示ポジション取得
	
	vect 1 = Return キーでの順送り
		 2 = Return キー以外の順送り
		-1 = 戻り
  ================================================================*/
int CSkbnIn::CheckSyzPn( int vect )
{
	DBGFUNCTION;

	int pn = -1;

	if( ! m_pSy->IsSyohizeiMaster() )
		return -1;

	m_pSy->SyMsg( &m_smpk );
	struct _SY_MSG_TBL* psmt;
	struct _SY_MSG_REC* pRec;

	int i, cnt;
	int ritu_sw, zeikbn_sw, uri_sw, zei_sw, swk_sw, taika_sw, toku_sw;
	ritu_sw = zeikbn_sw = uri_sw = zei_sw = swk_sw = taika_sw = toku_sw = 0;

	swk_sw = m_smpk.SY_MSG_SWKBN->sel_no;
	ritu_sw = m_smpk.SY_MSG_RITU->sel_no;
	zeikbn_sw = m_smpk.SY_MSG_ZEI->sel_no;
	uri_sw = m_smpk.SY_MSG_URISIRE->sel_no;
#ifdef TAIKA_VER
	taika_sw = m_smpk.SY_MSG_TAIKAKBN->sel_no;
#endif
	// 特定収入
	toku_sw = m_smpk.SY_MSG_TOKTEI->sel_no;

	if( !swk_sw && !ritu_sw && !zeikbn_sw && !uri_sw && !taika_sw && !toku_sw)
		return -1;

	if( vect < 0 || vect == 2 ) {	//戻り or RETURNでない送り
		//簡易課税・個別対応は、売上／仕入区分に
		if( m_pSy->IsKaniMaster() && uri_sw ) {
			pn = SY_URISIRE_PN;
		}
		if( m_pSy->IsKobetsuMaster() && uri_sw ) {
			pn = SY_URISIRE_PN;
		}

		if( pn == -1 ) {
			if( zeikbn_sw )	pn = SY_SZKBN_PN;
			else if( swk_sw )		pn = SY_SYZSWKBN_PN;
			else if( ritu_sw )		pn = SY_ZRITU_PN;
		//	else if( zeikbn_sw )	pn = SY_SZKBN_PN;
			else if( uri_sw )		pn = SY_URISIRE_PN;
			else if( taika_sw ) {
				psmt = m_smpk.SY_MSG_TAIKAKBN;
				pRec = psmt->msg_tbl[taika_sw-1];
				if( pRec->code == CD_TAIKA )
					pn = SY_TAIKA_PN;
				else
					pn = SY_TAISEL_PN;
			}
			else if( toku_sw )		pn = SY_TOKUS_PN;
		}
	}
	else {
		if( vect == 1 ) {	// VK_RETURN での送り
			if( taika_sw ) {
				psmt = m_smpk.SY_MSG_TAIKAKBN;
				pRec = psmt->msg_tbl[taika_sw-1];
				if( pRec->code == CD_TAIKA )
					pn = SY_TAIKA_PN;
				else
					pn = SY_TAISEL_PN;
			}
			else if( toku_sw ) {
				pn = SY_TOKUS_PN;
			}
			else if( m_pSy->IsKaniMaster() && uri_sw ) {
				psmt = m_smpk.SY_MSG_URISIRE;
				pRec = psmt->msg_tbl[uri_sw-1];

				cnt = 0;
				for( i = 0; i < psmt->trn; i++ ) {
					if( psmt->msg_tbl[i]->code != 0 )
						cnt++;
				}

				if( cnt > 1 ) {
					if( ! dc_kmdefcheck(0) ) {	//科目にデフォルト設定がない
						if( (m_pZm->zvol->s_sgn3 & 0xf8) ) {
							pn = SY_URISIRE_PN;
						}
					}
				}
			}
			else if( m_pSy->IsKobetsuMaster() && uri_sw ) {
				psmt = m_smpk.SY_MSG_URISIRE;
				pRec = psmt->msg_tbl[uri_sw-1];

				cnt = 0;
				for( i = 0; i < psmt->trn; i++ ) {
					if( psmt->msg_tbl[i]->code != 0 )
						cnt++;
				}

				if( cnt > 1 ) {
					if( ! dc_kmdefcheck(1) ) {	//科目にデフォルト設定がない
						pn = SY_URISIRE_PN;
					}
				}
			}
		}
	}

	return pn;
}

//借方・貸方の科目に売上区分か仕入区分のデフォルト設定があるか？
//	sgn == 0	簡易課税
//		　 1	個別対応
//
BOOL CSkbnIn::dc_kmdefcheck( int sgn )
{
	DBGFUNCTION;

	struct _DBKNREC* pKn;
	BOOL bRet = FALSE;
	BYTE mask;
	int	idx;

	pKn = DB_PjisToKnrec( m_pZm->dbdata->dbt, m_pZm, TRUE );
	
	// 03.31 /08
	if( sgn == 0 ) {
		mask	= 0x07;
		idx		= 3;
	}
	else {
		mask	= 0x03;
		idx		= 4;
	}

	if(pKn != NULL) {
		if ((pKn->knsgn[idx] & mask))
			bRet = TRUE;
	}

	if( ! bRet ) {
		pKn = DB_PjisToKnrec( m_pZm->dbdata->cre, m_pZm, TRUE );
		if (pKn != NULL) {
			if( (pKn->knsgn[idx] & mask) )
				bRet = TRUE;
		}
	}

	return bRet;
}



BOOL CSkbnIn::skbn_dataset( int *set_pn )
{
	DBGFUNCTION;
	BOOL ret;
	
//	m_pDInput->DB_SyIniz( m_pData );

	ret = sydata_set( set_pn );

	return ret;
}


//
//	消費税データセット
//		int* set_pn	.... 入力可能な項目をビットで知らせる
//
//
BOOL CSkbnIn::sydata_set( int *set_pn )
{
	DBGFUNCTION;

	struct _InpCtl* p_inp;
	VARIANT var;
	char	tmp[64];
	int idc, i, typ;
	int ritu_sw, zeikbn_sw, uri_sw, zei_sw, swk_sw, taika_sw, toku_sw, menzei_sw;

	ritu_sw = zeikbn_sw = uri_sw = zei_sw = swk_sw = taika_sw = toku_sw = menzei_sw = 0;

	m_pSy->SyMsg( &m_smpk );
	struct _SY_MSG_TBL* psmt;

	//エディット初期化
	var.pbVal = NULL;

	for( i = SY_SYZSWKBN_PN; i < PN_MAX; i++ ) {
		p_inp = getINP_CTL( i );
		idc = p_inp->IDC_X;
		var.pbVal = NULL;
		typ = (i == SY_TAIKA_PN) ? ICSDBEDT_TYPE_ARITH : ICSDBEDT_TYPE_STRING;


		// インボイス対応
		if( i == SY_MENZEI_PN ) {
			continue;
		}

		DBdata_set( this, idc, &var, typ, 0 );

		//if( i != SY_TAIKA_PN )
		if(( i != SY_TAIKA_PN ) /*&& ( i != SY_INV_PN )*/ && ( i != SY_MENZEI_PN )) {
			DBSetDispString( this, idc, NULL );
		}
		//表示のみの登録番号
		if( i == SY_INV_PN ) {
			idc = IDC_ICSDBEDT_INVDISP;
			DBdata_set(this, idc, &var, typ, 0);
			DBSetDispString(this, idc, NULL);
		}
	}

#ifdef OLD
	for( i = 0; i < m_smpk.SY_MSG_RITU->trn; i++ ) {
		if( m_smpk.SY_MSG_RITU->msg_tbl[i]->use )
			ritu_sw++;
	}
	for( i = 0; i < m_smpk.SY_MSG_ZEI->trn; i++ ) {
		if( m_smpk.SY_MSG_ZEI->msg_tbl[i]->use )
			zeikbn_sw++;
	}
	for( i = 0; i < m_smpk.SY_MSG_URISIRE->trn; i++ ) {
		if( m_smpk.SY_MSG_URISIRE->msg_tbl[i]->use )
			uri_sw++;
	}
	for( i = 0; i < m_smpk.SY_MSG_SWKBN->trn; i++ ) {
		if( m_smpk.SY_MSG_SWKBN->msg_tbl[i]->use )
			swk_sw++;
	}
//	for( i = 0; i < m_smpk.SY_MSG_SWKBN->trn; i++ ) {
//		if( ! m_smpk.SY_MSG_RITU->msg_tbl[i].use )
//			zei_sw++;
//	}
#endif

	swk_sw = m_smpk.SY_MSG_SWKBN->sel_no;
	ritu_sw = m_smpk.SY_MSG_RITU->sel_no;
	zeikbn_sw = m_smpk.SY_MSG_ZEI->sel_no;
	uri_sw = m_smpk.SY_MSG_URISIRE->sel_no;
#ifdef TAIKA_VER
	taika_sw = m_smpk.SY_MSG_TAIKAKBN->sel_no;
#endif
	toku_sw = m_smpk.SY_MSG_TOKTEI->sel_no;		// 特定収入
	menzei_sw = m_smpk.SY_MSG_EXEMPT->sel_no;

	int kanseifuri = 0;

	//if( ! ritu_sw && ! zeikbn_sw && ! uri_sw && ! swk_sw && ! taika_sw && !toku_sw ) {
	if( ! ritu_sw && ! zeikbn_sw && ! uri_sw && ! swk_sw && ! taika_sw && !toku_sw && !menzei_sw ) {
		if( set_pn != NULL )	*set_pn = 0;
		return FALSE;
	}
	else {
		//消費税仕訳区分
		p_inp = getINP_CTL( SY_SYZSWKBN_PN );
		idc = p_inp->IDC_X;
		GetDlgItem( idc )->EnableWindow( TRUE );
		psmt = m_smpk.SY_MSG_SWKBN;
		if( swk_sw != 0  )
		{
			GetDlgItem( IDC_STATIC1 )->EnableWindow( TRUE );
			sprintf_s( tmp, sizeof tmp, "%d", psmt->sel_no );
			var.pbVal = (BYTE*)tmp;
			DBdata_set( this, idc, &var, ICSDBEDT_TYPE_STRING, 0 );
			DBSetDispString( this, idc, psmt->msg_tbl[psmt->sel_no-1]->msg );
			p_inp->INP_ena = TRUE;

			// 最初の 消費税仕訳区分 サイン
			if( set_pn != NULL ) {
				m_defSwkKbn = psmt->msg_tbl[psmt->sel_no-1]->code;
			}
			//完成時振替は、登録番号だけ入力不可
			if( CD_KANSEI_FURI == psmt->msg_tbl[psmt->sel_no - 1]->code ) {
				kanseifuri = 1;
			}
		}
		else
		{
			GetDlgItem( IDC_STATIC1 )->EnableWindow( FALSE );
			var.pbVal = NULL;
			DBdata_set( this, idc, &var, ICSDBEDT_TYPE_STRING, 0 );
			GetDlgItem( idc )->EnableWindow( FALSE );
			p_inp->INP_ena = FALSE;
		}
		// 税率
		GetDlgItem( IDC_STATIC2 )->SetWindowText( "税 率" );
		p_inp = getINP_CTL( SY_ZRITU_PN );
		idc = p_inp->IDC_X;
		GetDlgItem( idc )->EnableWindow( TRUE );
		psmt = m_smpk.SY_MSG_RITU;
		if( ritu_sw != 0  )
		{
			GetDlgItem( IDC_STATIC2 )->EnableWindow( TRUE );
			sprintf_s( tmp, sizeof tmp, "%d", psmt->sel_no );
			var.pbVal = (BYTE*)tmp;
			DBdata_set( this, idc, &var, ICSDBEDT_TYPE_STRING, 0 );
			DBSetDispString( this, idc, psmt->msg_tbl[psmt->sel_no-1]->msg );
			p_inp->INP_ena = TRUE;
		}
		else
		{
			GetDlgItem( IDC_STATIC2 )->EnableWindow( FALSE );

			var.pbVal = NULL;
			DBdata_set( this, idc, &var, ICSDBEDT_TYPE_STRING, 0 );
			GetDlgItem( idc )->EnableWindow( FALSE );
			p_inp->INP_ena = FALSE;
		}
		// 税区分
		p_inp = getINP_CTL( SY_SZKBN_PN );
		idc = p_inp->IDC_X;
		GetDlgItem( idc )->EnableWindow( TRUE );
		psmt = m_smpk.SY_MSG_ZEI;
		if( zeikbn_sw != 0  )
		{
			GetDlgItem( IDC_STATIC3 )->EnableWindow( TRUE );
			sprintf_s( tmp, sizeof tmp, "%d", psmt->sel_no );
			var.pbVal = (BYTE*)tmp;
			DBdata_set( this, idc, &var, ICSDBEDT_TYPE_STRING, 0 );
			DBSetDispString( this, idc, psmt->msg_tbl[psmt->sel_no-1]->msg );
			p_inp->INP_ena = TRUE;
		}
		else
		{
			GetDlgItem( IDC_STATIC3 )->EnableWindow( FALSE );

			var.pbVal = NULL;
			DBdata_set( this, idc, &var, ICSDBEDT_TYPE_STRING, 0 );
			GetDlgItem( idc )->EnableWindow( FALSE );
			p_inp->INP_ena = FALSE;
		}
		//売上／仕入区分
		p_inp = getINP_CTL( SY_URISIRE_PN );
		idc = p_inp->IDC_X;
		GetDlgItem( idc )->EnableWindow( TRUE );
		psmt = m_smpk.SY_MSG_URISIRE;

		if( uri_sw )
		{

			if( m_pSy->IsKobetsuMaster() ) {
				GetDlgItem( IDC_STATIC4 )->SetWindowText( "仕入区分" );
			}
			else {
				GetDlgItem( IDC_STATIC4 )->SetWindowText( "売上区分" );
			}

			GetDlgItem( IDC_STATIC4 )->EnableWindow( TRUE );
			sprintf_s( tmp, sizeof tmp, "%d", psmt->sel_no );
			var.pbVal = (BYTE*)tmp;
			DBdata_set( this, idc, &var, ICSDBEDT_TYPE_STRING, 0 );
			DBSetDispString( this, idc, psmt->msg_tbl[psmt->sel_no-1]->msg );
			p_inp->INP_ena = TRUE;
		}
		else
		{
			GetDlgItem( IDC_STATIC4 )->EnableWindow( FALSE );
			var.pbVal = NULL;
			DBdata_set( this, idc, &var, ICSDBEDT_TYPE_STRING, 0 );
			GetDlgItem( idc )->EnableWindow( FALSE );
			p_inp->INP_ena = FALSE;
		}

#ifdef TAIKA_VER
		// 取得・譲渡, 対価
		p_inp = getINP_CTL( SY_TAIKA_PN );
		idc = p_inp->IDC_X;
		psmt = m_smpk.SY_MSG_TAIKAKBN;

		if( taika_sw ) {
			GetDlgItem( IDC_STATIC5 )->EnableWindow( TRUE );
			m_DBedt5.EnableWindow( TRUE );
			getINP_CTL( SY_TAISEL_PN )->INP_ena = TRUE;

			sprintf_s( tmp, sizeof tmp, "%d", psmt->sel_no );
			var.pbVal = (BYTE*)tmp;
			DBdata_set( this, IDC_ICSDBEDTCTRL5, &var, ICSDBEDT_TYPE_STRING, 0 );
			DBSetDispString( this, IDC_ICSDBEDTCTRL5, psmt->msg_tbl[psmt->sel_no-1]->msg );

			if( psmt->msg_tbl[psmt->sel_no-1]->code == CD_TAIKA ) {
				if( m_pSy->IsTaikaData( m_pZm->dbdata ) == 2 )
					GetDlgItem( IDC_STATIC2 )->SetWindowText( "非課税分" );

				GetDlgItem( IDC_STATIC6 )->EnableWindow( TRUE );
				m_DBedt6.EnableWindow( TRUE );
				::ZeroMemory( tmp, sizeof tmp );
				l_input(tmp, (void*)(LPCTSTR)m_pZm->dbdata->taika );
				var.pbVal = (BYTE*)tmp;

				DBdata_set( this, idc, &var, ICSDBEDT_TYPE_ARITH, 0 );
				getINP_CTL( SY_TAIKA_PN )->INP_ena = TRUE;
			}
			else {
				GetDlgItem( IDC_STATIC6 )->EnableWindow( FALSE );
				m_DBedt6.EnableWindow( FALSE );
				getINP_CTL( SY_TAIKA_PN )->INP_ena = FALSE;
			}
		}
		else {
			GetDlgItem( IDC_STATIC5 )->EnableWindow( FALSE );
			m_DBedt5.EnableWindow( FALSE );
			GetDlgItem( IDC_STATIC6 )->EnableWindow( FALSE );
			m_DBedt6.EnableWindow( FALSE );

			var.pbVal = NULL;
			DBdata_set( this, IDC_ICSDBEDTCTRL5, &var, ICSDBEDT_TYPE_STRING, 0 );
			var.pbVal = NULL;
			DBdata_set( this, idc, &var, ICSDBEDT_TYPE_ARITH, 0 );
	
			getINP_CTL( SY_TAISEL_PN )->INP_ena = FALSE;
			getINP_CTL( SY_TAIKA_PN )->INP_ena = FALSE;
		}
#else
		GetDlgItem( IDC_STATIC5 )->EnableWindow( FALSE );
		m_DBedt5.EnableWindow( FALSE );
		GetDlgItem( IDC_STATIC6 )->EnableWindow( FALSE );
		m_DBedt6.EnableWindow( FALSE );

		var.pbVal = NULL;
		DBdata_set( this, idc, &var, ICSDBEDT_TYPE_STRING, 0 );

		getINP_CTL( SY_TAISEL_PN )->INP_ena = FALSE;
		getINP_CTL( SY_TAIKA_PN )->INP_ena = FALSE;
#endif

		// 特定収入[06.03 /08]
		p_inp = getINP_CTL( SY_TOKUS_PN );
		idc = p_inp->IDC_X;
		GetDlgItem( idc )->EnableWindow( TRUE );
		psmt = m_smpk.SY_MSG_TOKTEI;
		if( toku_sw != 0  )
		{
			GetDlgItem( IDC_STATIC8 )->EnableWindow( TRUE );
			sprintf_s( tmp, sizeof tmp, "%d", psmt->sel_no );
			var.pbVal = (BYTE*)tmp;
			DBdata_set( this, idc, &var, ICSDBEDT_TYPE_STRING, 0 );
			DBSetDispString( this, idc, psmt->msg_tbl[psmt->sel_no-1]->msg );
			p_inp->INP_ena = TRUE;
		}
		else
		{
			GetDlgItem( IDC_STATIC8 )->EnableWindow( FALSE );

			var.pbVal = NULL;
			DBdata_set( this, idc, &var, ICSDBEDT_TYPE_STRING, 0 );
			GetDlgItem( idc )->EnableWindow( FALSE );
			p_inp->INP_ena = FALSE;
		}

		if( set_pn != NULL ) {
			*set_pn = 0;
			if( swk_sw )	*set_pn |= 0x01 << SY_SYZSWKBN_PN;
			if( ritu_sw )	*set_pn |= 0x01 << SY_ZRITU_PN;
			if( zeikbn_sw ) *set_pn |= 0x01 << SY_SZKBN_PN;
			if( uri_sw )	*set_pn |= 0x01 << SY_URISIRE_PN;
			if( taika_sw ) {
				if( getINP_CTL( SY_TAIKA_PN )->INP_ena )
					*set_pn |= 0x01 << SY_TAIKA_PN;
				else
					*set_pn |= 0x01 << SY_TAISEL_PN;
			}
			if( toku_sw ) {
				*set_pn |= 0x01 << SY_TOKUS_PN;
			}
		}
MyTrace("sydata_set dbt = %s, cre = %s, menzei_sw = %d\n", m_pZm->dbdata->dbt, m_pZm->dbdata->cre, menzei_sw );

//MyTrace("sydata_set SY_MSG_EXEMPT max(%d), trn(%d), sel_no(%d)\n",m_smpk.SY_MSG_EXEMPT->max, m_smpk.SY_MSG_EXEMPT->trn, m_smpk.SY_MSG_EXEMPT->sel_no);
//for( int n = 0; n < m_smpk.SY_MSG_EXEMPT->trn; n++ ) {
//	MyTrace("sydata_set SY_MSG_EXEMPT msg = %s\n", m_smpk.SY_MSG_EXEMPT->msg_tbl[n]->msg );
//}
		// インボイス対応
		if( m_bInvMaster && m_bInvUse && ( m_InpMode == INPMODE_NORMMAL )) {
			p_inp = getINP_CTL( SY_INV_PN );

			if( m_bInvDisp ) {
				// 登録番号
				idc = p_inp->IDC_X;
				int idc2 = IDC_ICSDBEDT_INVDISP;

				if( menzei_sw && !kanseifuri ) {
					GetDlgItem( idc )->EnableWindow( TRUE );
					GetDlgItem(idc2)->EnableWindow(TRUE);
					GetDlgItem( IDC_STATIC18 )->EnableWindow( TRUE );
	
					CString tmpInvno;
					tmpInvno = m_pZm->dbdata->invno;
					if( !tmpInvno.IsEmpty() ) {
						tmpInvno.Replace( _T( "T" ), _T( "" ));
						sprintf_s( tmp, sizeof( tmp ), _T( "%s" ), (char*)(LPCTSTR)tmpInvno);
						var.pbVal = ( BYTE* )tmp;
						DBdata_set( this, getINP_CTL( SY_INV_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
	
						DBSetDispString( this, IDC_ICSDBEDT_INVDISP/*idc*/, m_pZm->dbdata->invno );

						MyTrace("sydata_set DBSetDispString tmp = %s, dbdata->invno = %s\n", tmp, m_pZm->dbdata->invno);
					}
					p_inp->INP_ena = TRUE;
				}
				else {
					GetDlgItem( idc )->EnableWindow( FALSE );
					GetDlgItem(idc2)->EnableWindow(FALSE);
					GetDlgItem( IDC_STATIC18 )->EnableWindow( FALSE );
					memset( tmp, '\0', sizeof( tmp ));
					var.pbVal = ( BYTE* )tmp;
					DBdata_set( this, IDC_ICSDBEDT_INVDISP/*getINP_CTL( SY_INV_PN )->IDC_X*/, &var, ICSDBEDT_TYPE_STRING, 0 );
					DBSetDispString( this, idc, tmp );
					p_inp->INP_ena = FALSE;
				}
			}
			else {
				p_inp = getINP_CTL( SY_INV_PN );
				p_inp->INP_ena = FALSE;
			}

			// 免税事業者からの課税仕入れ
			p_inp = getINP_CTL( SY_MENZEI_PN );
			idc = p_inp->IDC_X;
			if( menzei_sw ) {
				GetDlgItem( idc )->EnableWindow( TRUE );
				int menCheck = BST_UNCHECKED;
				if( menzei_sw == 2 || menzei_sw == 3 ) {
					menCheck = BST_CHECKED;
				}
				m_MenzeiCheck.SetCheck( menCheck );
				p_inp->INP_ena = TRUE;

				MenWariDBEdit(menCheck == BST_CHECKED ? TRUE : FALSE);
			}
			else {
				GetDlgItem( idc )->EnableWindow( FALSE );
				m_MenzeiCheck.SetCheck( BST_UNCHECKED );
				p_inp->INP_ena = FALSE;

				MenWariDBEdit(FALSE);
			}
		}
		else {
			int ctrlId = 0;
			for( int i = 0; i < PN_MAX; i++ ) {
				p_inp = getINP_CTL(i);
				ctrlId = p_inp->IDC_X;
				if( ctrlId == IDC_ICSDBEDT_INV || ctrlId == IDC_CHECK_MENZEI || ctrlId == IDC_ICSDBEDT_MEN ) {
					p_inp->INP_ena = FALSE;
				}
			}
		}

		return TRUE;
	}
	return TRUE;
}


//
//区分選択作成
void CSkbnIn::kbn_select_make( struct _SY_MSG_TBL *sel, int pn ) 
{
	DBGFUNCTION;
int i;
static CString str[MaxRec];

	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( 1 /*skbn_sw == SYFNCRET_SEL_SET */)
	{
		switch( pn )
		{
		case SY_SYZSWKBN_PN:
		case SY_ZRITU_PN:
		case SY_SZKBN_PN:
		case SY_URISIRE_PN:
		case SY_TAISEL_PN:
		case SY_TAIKA_PN:
		case SY_TOKUS_PN:
		case SY_MENWARI_PN:
			if( sel ) {
				if( pn == SY_MENWARI_PN ) {
					int idx = 0;
					for( i = 0; i < MaxRec; ++i ) {
						if( i < sel->trn ) {
							if( sel->msg_tbl[i]->code == CD_SP_NO_EXEMPT )
								continue;

							if( m_Koujo80_50 == SYZ_MEN_80 || m_Koujo80_50 == SYZ_MEN_8050 || m_Koujo80_50 == SYZ_MEN_50 ) {
								if( sel->msg_tbl[i]->code == CD_SP_EXEMPT ) {
									str[idx++] = "80% 控除";
								}
							}
							if( m_Koujo80_50 == SYZ_MEN_8050 || m_Koujo80_50 == SYZ_MEN_50 ) {
								if( sel->msg_tbl[i]->code == CD_SP_EXEMPT_50 ) {
									str[idx++] = "50% 控除";
								}
							}
						}
						else {
							break;
						}
					}
					for( i = idx; i < MaxRec; i++ ) {
						str[i].Empty();
					}
				}
				else {
					for( i = 0; i < sel->trn && i < MaxRec; ++i ) {
						str[i] = sel->msg_tbl[i]->msg;
					}
				}
			}
			else {
				for( i = 0 ; i < MaxRec ; ++i )
					str[i].Empty();
			}

			m_pParent->SendMessage( WM_SKBN_ICHIRN, (WPARAM)i, (LPARAM)str );
			m_selbox_user = pn;
			invmng.m_invPG = 0;

			break;

		case SY_INV_PN:
			{
				int invCount = 0;
				int sidx;

				invCount = (int)invmng.m_pInvAry->GetCount();
				sidx = (invmng.m_invPG * invmng.m_invPGCnt);

				for( i = 0; i < invmng.m_invPGCnt; ++i ) {
					if( sidx < invCount ) {
						str[i] = invmng.m_pInvAry->GetAt(sidx).inv_tkNam;
						sidx++;
					}
					else {
						str[i].Empty();
					}
				}
				WPARAM wp = MAKEWPARAM(i, 0x01);
				m_pParent->SendMessage(WM_SKBN_ICHIRN, (WPARAM)wp, (LPARAM)str);
				m_selbox_user = pn;
			}
			break;
		case SY_MENZEI_PN:
			for( i = 0; i < MaxRec; ++i ) {
				str[i].Empty();
			}
			m_pParent->SendMessage(WM_SKBN_ICHIRN, (WPARAM)0, (LPARAM)str);
			m_selbox_user = pn;
			invmng.m_invPG = 0;
			break;
		}
	}
	else
	{
		m_pParent->SendMessage( WM_SKBN_ICHIRN, (WPARAM)0, (LPARAM)str );
		m_selbox_user = pn;
	}
}

// 番号より 消費税区分をセット
int CSkbnIn::SetSkbnSelNo( int no )
{
	DBGFUNCTION;
//	MyTrace("SetSkbnSelNo pn = %d, no = %d\n", get_nowpn(), no);
	int ret = 0;
	DWORD code;
	int invCount = 0;
	int noMax;

//	no = (isEntrySys() ? no - 8 : no);

	struct _SY_MSG_TBL* psmt;
	int pn = get_nowpn();

	if( m_MODEFY_OK )
	{

		switch( pn )
		{
			//消費税仕訳区分
		case SY_SYZSWKBN_PN:
			psmt = m_smpk.SY_MSG_SWKBN;
			if( psmt )
				if( no > 0 && no <= psmt->trn )
				{
					code = psmt->msg_tbl[no-1]->code;

					if( m_pSy->SyFnc( code ) != 0 )
						ret = -1;
					else
						GetParent()->SendMessage( WM_SKBN_LINEDISP, no );
				}
			break;
			//税率
		case SY_ZRITU_PN:
			psmt = m_smpk.SY_MSG_RITU;
			if( psmt )
				if( no > 0 && no <= psmt->trn )
				{
					code = psmt->msg_tbl[no-1]->code;
					if( m_pSy->SyFnc( code ) != 0 )
						ret = -1;
					else
						GetParent()->SendMessage( WM_SKBN_LINEDISP, no );
				}
			break;

			//税区分
		case SY_SZKBN_PN:
			psmt = m_smpk.SY_MSG_ZEI;
			if( psmt )
				if( no > 0 && no <= psmt->trn )
				{
					code = psmt->msg_tbl[no-1]->code;
					if( m_pSy->SyFnc( code ) != 0 )
						ret = -1;
					else
						GetParent()->SendMessage( WM_SKBN_LINEDISP, no );
				}
			break;
			//売上/仕入区分
		case SY_URISIRE_PN:
			psmt = m_smpk.SY_MSG_URISIRE;
			if( psmt )
				if( no > 0 && no <= psmt->trn )
				{
					code = psmt->msg_tbl[no-1]->code;
					if( m_pSy->SyFnc( code ) != 0 )
						ret = -1;
					else
						GetParent()->SendMessage( WM_SKBN_LINEDISP, no );
				}

		DSIGN_TRACE(m_pZm->dbdata->dsign);

			break;

#ifdef TAIKA_VER
			//取得・譲渡
		case SY_TAISEL_PN:
			psmt = m_smpk.SY_MSG_TAIKAKBN;
			if( psmt )

				if( no > 0 && no <= psmt->trn )
				{
					code = psmt->msg_tbl[no-1]->code;

					BYTE dsign[4];
					memcpy(dsign, &m_pZm->dbdata->dsign[0], 4);

					if( m_pSy->SyFnc(code) != 0 ) {
						ret = -1;
					}
					else {
						// 対価入力を可能に
						if( code == CD_TAIKA ) {
							GetDlgItem( IDC_STATIC6 )->EnableWindow( TRUE );
							m_DBedt6.EnableWindow( TRUE );
							char tmp[36];
							VARIANT var;
							::ZeroMemory( tmp, sizeof tmp );
							l_input(tmp, (void*)(LPCTSTR)m_pZm->dbdata->taika );
							var.pbVal = (BYTE*)tmp;
							DBdata_set( this, IDC_ICSDBEDTCTRL6, &var, ICSDBEDT_TYPE_STRING, 0 );
							getINP_CTL( SY_TAIKA_PN )->INP_ena = TRUE;
						}
						else {
							GetDlgItem( IDC_STATIC6 )->EnableWindow( FALSE );
							m_DBedt6.EnableWindow( FALSE );
							getINP_CTL( SY_TAIKA_PN )->INP_ena = FALSE;
							//取得で課税→不課税に変わった場合は、課税にする
							if( code == CD_SYUTOK ) {

								if( (dsign[2] & 0x0f) == 1 && (m_pZm->dbdata->dsign[2] & 0x0f) == 2 ) {
									DWORD fncCode = CD_KAZEI;
									//外税・内税も直前を反映
									if( (dsign[1] & 0xf0) == 0x00 )		fncCode |= CD_SOTOZ;
									else if((dsign[1] & 0xf0) == 0x10 )	fncCode |= CD_UTIZ; 
									else {
										fncCode |= CD_BEKKI;
									}

									m_pSy->SyFnc(fncCode);
								}
							}
						}
						GetParent()->SendMessage( WM_SKBN_LINEDISP, no );
					}
				}
			break;
#endif

		//特定収入
		case SY_TOKUS_PN:
			psmt = m_smpk.SY_MSG_TOKTEI;
			if( psmt )
				if( no > 0 && no <= psmt->trn )
				{
					code = psmt->msg_tbl[no-1]->code;
					if( m_pSy->SyFnc( code ) != 0 )
						ret = -1;
					else
						GetParent()->SendMessage( WM_SKBN_LINEDISP, no );
				}
			break;

		// インボイス対応
		// 登録番号
		case SY_INV_PN:
			invCount = ( int )invmng.m_pInvAry->GetCount();
			if(( no > 0 ) && ( no <= invmng.m_invPGCnt)) {
				// 科目設定で登録番号のチェックは行われているはずだが、一応仕訳入力でも行っておく
				CString checkInvno;
				int sidx = (invmng.m_invPG * invmng.m_invPGCnt);
				int aryIdx = sidx + (no - 1);

				if( aryIdx < invCount ) {
					checkInvno = invmng.m_pInvAry->GetAt(aryIdx).inv_invNo;
					if( syzInvoice.CheckInvoiceNumber(checkInvno) == -1 ) {
						m_bIgnoreFocus = TRUE;
					//	ICSMessageBox(syzInvoice.GetErrorMessage(), MB_ICONSTOP);
					//	m_bIgnoreFocus = FALSE;
						m_ErrMsg = syzInvoice.GetErrorMessage();
						PostMessage(WM_MYMESSAGE, INV_ERRMSG);
						ret = -1;
					}
					else {
						m_pZm->dbdata->invno = checkInvno;
					}
				}
			}
			GetParent()->SendMessage( WM_SKBN_LINEDISP, no < 0 ? 0 : 1 );
			break;

		case SY_MENZEI_PN:
		case SY_MENWARI_PN:
			set_nowpn( m_selbox_user );
			if( pn == SY_MENZEI_PN && no == 0) {
				GetParent()->SendMessage(WM_SKBN_LINEDISP, no);
			}
		//	break;
		//	set_nowpn(m_selbox_user);
			code = 0;
			noMax = 0;

			if( m_Koujo80_50 == SYZ_MEN_80) {
				noMax = 1;

				if( m_pZm->dbdata->ddate > SYZMEN4_YMD ) {
					noMax = 0;
				}
				else if( m_pZm->dbdata->ddate < SYZMEN1_YMD ) {
					noMax = 0;
				}
			}
			else if( m_Koujo80_50 == SYZ_MEN_8050 || m_Koujo80_50 == SYZ_MEN_50 ) {
				noMax = 2;
			}
			if( noMax ) {
				if( no > 0 && no <= noMax )
				{
					//1番目除く(課税事業者からの課税仕入れ)
					//code = CD_SP_EXEMPT;		80%
					//code = CD_SP_EXEMPT_50;	50%
					//code = psmt->msg_tbl[no - 0]->code;
				//	if( m_Koujo80_50 == SYZ_MEN_50 ) {
				//		code = CD_SP_EXEMPT_50;	//50 %
				//	}
				//	else {
						if( no == 1 ) {
							code = CD_SP_EXEMPT;	//80 %
						}
						else {
							code = CD_SP_EXEMPT_50;	//50 %
						}
				//	}
				}
			}
			if( code != 0 ) {
				if( m_pSy->SyFnc(code) != 0 )
					ret = -1;
				else
					GetParent()->SendMessage(WM_SKBN_LINEDISP, no);
			}
			
			break;
		default:
			set_nowpn( m_selbox_user );
			break;
		}
		sydata_set(NULL);
	}
#ifdef LATER_CLOSE
	if( ret != -1 )
		if( INP_mode == 0 /*_APPEND*/ )
			TKattr2 = TKattr = 0;
#endif		

	return ret;

}


// 現在ポジションセット
int CSkbnIn::set_nowpn( int pn )
{
	DBGFUNCTION;
	PREV_PN = NOW_PN;
	NOW_PN = pn;

	return( PREV_PN );	//直前のポジション
}

// 現在ポジションゲット
int CSkbnIn::get_nowpn( void )
{
	return( NOW_PN );
}

// 直前のポジションゲット
int CSkbnIn::get_prevpn()
{
	return( PREV_PN );
}


//次のポジション検索
int CSkbnIn::get_nextpn( int now, int vk/*= 0*/ )
{
	DBGFUNCTION;
	int pn;

#ifdef CLOSE
	if( m_MODEFY_OK )
		for( pn = now + 1 ; pn < PN_MAX ; ++pn )
		{
			if( getINP_CTL( pn )->INP_ena )
			{
				return pn;
			}
		}
#endif
	if( m_MODEFY_OK ) {
		int i, sgn = 0;
		for( i = 0; PN_ORDER[i] != -1 ; i++ ) {
			if( now == PN_ORDER[i] ) {
				sgn = 1;
				continue;
			}

			if( sgn ) {
				pn = PN_ORDER[i];
				if( getINP_CTL( pn )->INP_ena )
				{
					return pn;
				}
			}
		}
	}

	return -1;
}

//前のポジション検索
int CSkbnIn::get_prevpn( int now )
{
	DBGFUNCTION;
	int pn;

#ifdef CLOSE
	for( pn = now - 1 ; pn >= SY_SYZSWKBN_PN ; --pn )
	{
		if( getINP_CTL( pn )->INP_ena )
			return pn;
	}
#endif

	int i, max;
	max	= sizeof PN_ORDER / sizeof PN_ORDER[0] ;
	int sgn = 0;
	
	for( i = max-1; i >= 0; i-- ) {
		if( now == PN_ORDER[i] ) {
			sgn = 1;
			continue;
		}

		if( sgn ) {
			pn = PN_ORDER[i];
			if( getINP_CTL( pn )->INP_ena )
			{
				return pn;
			}
		}
	}

	return -1;
}

//親の現在のフォーカスポジションへ
void CSkbnIn::to_parent( CWnd *parent, long nChar )
{
	DBGFUNCTION;
	BYTE dsign[4];
	memcpy(dsign, &m_pZm->dbdata->dsign[0], 4);

	GetParent()->SendMessage( WM_SKBN_INEND, nChar );
}



/////////////////////////////////////////////////////////////////////////////
// CSkbnIn メッセージ ハンドラ


BOOL CSkbnIn::OnInitDialog() 
{
	DBGFUNCTION;
	MoveCtrl();

	// インボイス対応
	MoveInvCtrl();
	ShowInvCtrl();

	ICSDialog::OnInitDialog();
	
//	ControlInit(147,53,-1);

	// TODO: この位置に初期化の補足処理を追加してください
	m_DBedt1.ImeEndMode( FALSE );
	m_DBedt2.ImeEndMode( FALSE );
	m_DBedt3.ImeEndMode( FALSE );
	m_DBedt4.ImeEndMode( FALSE );
	m_DBedt5.ImeEndMode( FALSE );
	m_DBedt6.ImeEndMode( FALSE );
	m_DBedtInv.ImeEndMode( TRUE );

	m_DBedt1.EnableDelete(0);
	m_DBedt2.EnableDelete(0);
	m_DBedt3.EnableDelete(0);
	m_DBedt4.EnableDelete(0);
	m_DBedt5.EnableDelete(0);
	m_DBedt6.EnableDelete(0);
	m_DBedtInv.EnableDelete(0);
	m_DBedtInv.EnableHome(1);
	m_DBedtInv.SetKanaTerm(TRUE, INVKANA_MAX, ICSDBEDT_KTYPE_KANA);

	if( m_bToku ) {
		m_DBedtToku.ImeEndMode( FALSE );
		m_DBedtToku.EnableDelete(0);
	}
	else {
		GetDlgItem( IDC_STATIC8 )->ShowWindow( SW_HIDE );
		m_DBedtToku.SetEnabled( FALSE );
	}

	m_MenWari.EnableDelete(0);

	// リサイズ前を取得
	GetWindowRect( &m_orgRect );

	ModifyStyle(0, WS_CLIPSIBLINGS);

	invmng.m_hWnd = m_DBedtInv.m_hWnd;
	invmng.m_nID = IDC_ICSDBEDT_INV;
	invmng.m_pDlg = this;

	if( m_bResize ) {
		ICSDialog::OnInitDialogEX();
	}
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}


//
//	特定収入でないときは、コントロールを移動する。
//
void CSkbnIn::MoveCtrl()
{
	if( m_bToku )	return;

	CRect rc1, rc2, rect;
	int	mv = 0;

	// 移動する基準となるコントロール
	GetDlgItem( IDC_STATIC8 )->GetWindowRect( rc1 );
	ScreenToClient( rc1 );
	GetDlgItem( IDC_ICSDBEDT_TOKU )->GetWindowRect( rc2 );
	ScreenToClient( rc2 );

	// インボイス対応
	//int stID[] = {IDC_STATIC4, IDC_STATIC5, IDC_STATIC6, 0};
	//int	dbID[] = {IDC_ICSDBEDTCTRL4,IDC_ICSDBEDTCTRL5,IDC_ICSDBEDTCTRL6, 0};

	int stID[] = { IDC_STATIC4, IDC_STATIC5, IDC_STATIC6, IDC_STATIC18, 0 };
	int	dbID[] = { IDC_ICSDBEDTCTRL4, IDC_ICSDBEDTCTRL5, IDC_ICSDBEDTCTRL6, IDC_ICSDBEDT_INV, 0 };

	for( int i = 0; stID[i] != 0; i++ ) {

		GetDlgItem( stID[i] )->GetWindowRect( rect );
		ScreenToClient( rect );
		//移動する高さ
		if( i == 0 ) {
			mv = (rect.top - rc1.top);
		}
		
		GetDlgItem( stID[i] )->MoveWindow( rect.left, rect.top - mv, rect.Width(), rect.Height() );

		GetDlgItem( dbID[i] )->GetWindowRect( rect );
		ScreenToClient( rect );
		GetDlgItem( dbID[i] )->MoveWindow( rect.left, rect.top - mv, rect.Width(), rect.Height() );

		if( dbID[i] == IDC_ICSDBEDT_INV ) {
			GetDlgItem(IDC_ICSDBEDT_INVDISP)->SetWindowPos(&CWnd::wndTop, rect.left, rect.top - mv, rect.Width(), rect.Height(), SWP_SHOWWINDOW);
		//	GetDlgItem(IDC_ICSDBEDT_INVDISP)->MoveWindow(rect.left, rect.top - mv, rect.Width(), rect.Height());
		}
	}

	// チェックボックスも移動
	GetDlgItem( IDC_CHECK_MENZEI )->GetWindowRect( rect );
	ScreenToClient( rect );
	GetDlgItem( IDC_CHECK_MENZEI )->MoveWindow( rect.left, rect.top - mv, rect.Width(), rect.Height() );

	// 控除割合 コンボボックスも移動
	GetDlgItem(IDC_ICSDBEDT_MEN)->GetWindowRect(rect);
	ScreenToClient(rect);
	GetDlgItem(IDC_ICSDBEDT_MEN)->MoveWindow(rect.left, rect.top - mv, rect.Width(), rect.Height());
	GetDlgItem(IDC_STATIC20)->GetWindowRect(rect);
	ScreenToClient(rect);
	GetDlgItem(IDC_STATIC20)->MoveWindow(rect.left, rect.top - mv, rect.Width(), rect.Height());


	// ダイアログ自体も小さくする
	GetWindowRect( rect );

	MoveWindow( rect.left, rect.top, rect.Width(), rect.Height()-mv );

//	MyTrace("CSkbnIn::MoveCtrl  rect(%d,%d,%d,%d)\n", rect.left, rect.top, rect.right, rect.bottom);

}

void CSkbnIn::PostNcDestroy() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	
	ICSDialog::PostNcDestroy();

	delete this;
}

/* IDC_ICSDBEDTCTRL1 から IDC_ICSDBEDT_TOKU までは連番 を割り当てる */

BEGIN_EVENTSINK_MAP(CSkbnIn, ICSDialog)
    //{{AFX_EVENTSINK_MAP(CSkbnIn)
	//}}AFX_EVENTSINK_MAP
	ON_EVENT_RANGE(CSkbnIn, IDC_ICSDBEDTCTRL1, IDC_ICSDBEDT_INVDISP, 3 /* SetFocus */, OnSetFocusIcsdbedtctrl, VTS_I4)
	ON_EVENT_RANGE(CSkbnIn, IDC_ICSDBEDTCTRL1, IDC_ICSDBEDT_INVDISP, 2 /* KillFocus */, OnKillFocusIcsdbedtctrl, VTS_I4 VTS_I4)
	ON_EVENT_RANGE(CSkbnIn, IDC_ICSDBEDTCTRL1,IDC_ICSDBEDT_MEN, 1 /* Termination */, OnTerminationIcsdbedtctrl, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT_RANGE(CSkbnIn, IDC_ICSDBEDTCTRL1,IDC_ICSDBEDT_MEN, 5, ImeEndCompositionIcsdbedtctrl, VTS_I4 VTS_I2 VTS_BSTR VTS_BSTR)
END_EVENTSINK_MAP()


void CSkbnIn::OnSetFocusIcsdbedtctrl(UINT ID) 
{
	DBGFUNCTION;

	if( m_THIS_DIALOG_OK == FALSE )
		return;

	struct _SY_MSG_TBL *psmt;
	int pn = -1;

	switch( ID ) {
	case IDC_ICSDBEDTCTRL1: // 消費税仕訳区分
		pn = SY_SYZSWKBN_PN;	psmt = m_smpk.SY_MSG_SWKBN;
		break;
	case IDC_ICSDBEDTCTRL2:	// 税率
		pn = SY_ZRITU_PN;		psmt = m_smpk.SY_MSG_RITU;
		break;
	case IDC_ICSDBEDTCTRL3:	// 税区分
		pn = SY_SZKBN_PN;		psmt = m_smpk.SY_MSG_ZEI;
		break;
	case IDC_ICSDBEDTCTRL4:	// 売上仕入区分
		pn = SY_URISIRE_PN;		psmt = m_smpk.SY_MSG_URISIRE;
		break;
#ifdef TAIKA_VER
	case IDC_ICSDBEDTCTRL5:	// 譲渡・取得
		pn = SY_TAISEL_PN;			psmt = m_smpk.SY_MSG_TAIKAKBN;
		break;
	case IDC_ICSDBEDTCTRL6:	// 対価
		pn = SY_TAIKA_PN;			psmt = NULL;
		break;
#endif
	case IDC_ICSDBEDT_TOKU:	// 特定収入
		pn = SY_TOKUS_PN;			psmt = m_smpk.SY_MSG_TOKTEI;
		break;
	case IDC_ICSDBEDT_INV:	// 登録番号
		m_DBedtInvDisp.ShowWindow(SW_HIDE);
		pn = SY_INV_PN;
		psmt = NULL;
		break;
	case IDC_ICSDBEDT_MEN:
		pn = SY_MENWARI_PN;
		psmt = m_smpk.SY_MSG_EXEMPT;
		break;

	case IDC_ICSDBEDT_INVDISP:
		m_DBedtInvDisp.ShowWindow(SW_HIDE);
		set_focus(SY_INV_PN);
		return;
		break;

	default:
		ASSERT( FALSE );
		break;
	}
	if( pn != SY_INV_PN ) {
		invmng.InvNoSearch(NULL);
	}
	if( pn == -1 )
		return;

	if( getINP_CTL( pn )->INP_ena == FALSE )
	{
		set_focus( get_nowpn() );
		return;
	}

	set_nowpn( pn );
	kbn_select_make( psmt, get_nowpn() ); 

}

void CSkbnIn::OnKillFocusIcsdbedtctrl(UINT ID, long inplen) 
{
	DBGFUNCTION;
	if( m_THIS_DIALOG_OK == FALSE )
		return;
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( m_bIgnoreFocus ) {
		ID = 0;
	}
	
	invmng.CancelIME();

	switch( ID ) {
	case IDC_ICSDBEDTCTRL1:
		OnTerminationDBEdt1( 0, inplen, 0 );
		break;
	case IDC_ICSDBEDTCTRL2:
		OnTerminationDBEdt2( 0, inplen, 0 );
		break;
	case IDC_ICSDBEDTCTRL3:
		OnTerminationDBEdt3( 0, inplen, 0 );
		break;
	case IDC_ICSDBEDTCTRL4:
		OnTerminationDBEdt4( 0, inplen, 0 );
		break;
#ifdef TAIKA_VER
	case IDC_ICSDBEDTCTRL5:	// 譲渡・取得
		OnTerminationDBEdt5( 0, inplen, 0 );
		break;
	case IDC_ICSDBEDTCTRL6:	// 対価
		OnTerminationDBEdt6( 0, inplen, 0 );
		break;
#endif
	case IDC_ICSDBEDT_TOKU:	// 特定収入
		OnTerminationDBEdtToku( 0, inplen, 0 );
		break;
	case IDC_ICSDBEDT_INV:	// 登録番号
		OnTerminationDBEdtInv( 0, inplen, 0 );

		WINDOWPLACEMENT place;
		m_DBedtInvDisp.GetWindowPlacement(&place);
		m_DBedtInvDisp.SetWindowPos(&CWnd::wndTop, place.rcNormalPosition.left, place.rcNormalPosition.top, place.rcNormalPosition.right - place.rcNormalPosition.left, place.rcNormalPosition.bottom - place.rcNormalPosition.top, SWP_SHOWWINDOW);
		break;
	case IDC_ICSDBEDT_MEN:	// 免税事業者控除割合
		OnTerminationDBEdtMen(0, inplen, 0);
		break;
	default:
	//	ASSERT(FALSE);
		return;
	}	
}

void CSkbnIn::OnTerminationIcsdbedtctrl(UINT ID, long nChar, long inplen, long kst) 
{
	DBGFUNCTION;
	if( m_THIS_DIALOG_OK == FALSE )
		return;
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
//TRACE( "Skbn OnTerminationDBEdt1 ID %d nChar %d inplen %d kst %d\n", ID,nChar,inplen,kst );

	switch( ID ) {
	case IDC_ICSDBEDTCTRL1:
		OnTerminationDBEdt1( nChar, inplen, kst );
		break;
	case IDC_ICSDBEDTCTRL2:
		OnTerminationDBEdt2( nChar, inplen, kst );
		break;
	case IDC_ICSDBEDTCTRL3:
		OnTerminationDBEdt3( nChar, inplen, kst );
		break;
	case IDC_ICSDBEDTCTRL4:
		OnTerminationDBEdt4( nChar, inplen, kst );
		break;
#ifdef TAIKA_VER
	case IDC_ICSDBEDTCTRL5:	// 譲渡・取得
		OnTerminationDBEdt5( nChar, inplen, kst );
		break;
	case IDC_ICSDBEDTCTRL6:	// 対価
		OnTerminationDBEdt6( nChar, inplen, kst );
		break;
#endif
	case IDC_ICSDBEDT_TOKU:	// 特定収入
		OnTerminationDBEdtToku( nChar, inplen, kst );
		break;
	case IDC_ICSDBEDT_INV:	// 登録番号
		OnTerminationDBEdtInv( nChar, inplen, kst );
		break;
	case IDC_ICSDBEDT_MEN:	// 免税事業者　控除割合
		OnTerminationDBEdtMen(nChar, inplen, kst);
		break;
	default:
		ASSERT(FALSE);
		return;
	}
}


// 消費税仕訳区分
int CSkbnIn::OnTerminationDBEdt1(long nChar, long inplen, long kst) 
{
	DBGFUNCTION;
	int i,pn;
	char pbuf[50];
	VARIANT var;

	if( m_THIS_DIALOG_OK == FALSE )
		return 1;

	switch( nChar )
	{
	case NULL:
	case VK_RIGHT:
	case VK_RETURN:
	case VK_TAB:
	case VK_DOWN:
//	case VK_F3:
	case VK_F2:
	case VK_UP:
	case VK_LEFT:
		if( inplen ) {
			if( m_MODEFY_OK ) {
				pbuf[0] = '\0';
				DBdata_get( this, getINP_CTL( SY_SYZSWKBN_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
				strcpy_s( pbuf, sizeof pbuf, (char*)var.pbVal );

				for( i = 0 ; pbuf[i] ; ++i )
				{
					if( isdigit( pbuf[i] ) )
						continue;
					else
					{
						pbuf[0] = '\0';
						break;
					}
				}
				SetSkbnSelNo( atoi( pbuf ) );
			}
			else {
				sydata_set(NULL);
			}
		}
	}

	if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) ) {
		if( (pn = get_prevpn( get_nowpn())) != -1 )	{
			set_focus( pn );
			return 0;
		}
		else {
			to_parent( m_pParent, VK_LEFT );
			return 0;
		}
	}
	switch( nChar )
	{
	case VK_LEFT:
	case VK_RIGHT:
		to_parent( m_pParent, nChar );
		return 0;
	case VK_TAB:
	case VK_RETURN:
	case VK_DOWN:
//	case VK_F3:
		pn = get_nextpn( get_nowpn(), nChar);
		if( pn != -1 )
		{
			set_focus( pn );
			return 0;
		}
		else
		{
			to_parent( m_pParent, nChar );
			return 0;
		}
		break;

	case VK_F2:
	case VK_UP:
		if( (pn = get_prevpn( get_nowpn())) != -1 )
		{
			set_focus( pn );
			return 0;
		}
		else
		{
			to_parent( m_pParent, nChar );
			return 0;
		}
		break;

	default:
		return 1;
		break;
	}
}


// 税率
int CSkbnIn::OnTerminationDBEdt2(long nChar, long inplen, long kst)  
{
	DBGFUNCTION;
	int i,pn;
	char pbuf[50];
	VARIANT var;

	if( m_THIS_DIALOG_OK == FALSE )
		return 1;

	switch( nChar )
	{
	case NULL:
	case VK_RIGHT:
	case VK_RETURN:
	case VK_TAB:
	case VK_DOWN:
//	case VK_F3:
	case VK_F2:
	case VK_UP:
	case VK_LEFT:
		if( inplen ) {
			if( m_MODEFY_OK  ) {
				pbuf[0] = '\0';
				DBdata_get( this, getINP_CTL( SY_ZRITU_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
				strcpy_s( pbuf, sizeof pbuf, (char*)var.pbVal );

				for( i = 0 ; pbuf[i] ; ++i )
				{
					if( isdigit( pbuf[i] ) )
						continue;
					else
					{
						pbuf[0] = '\0';
						break;
					}
				}
				SetSkbnSelNo( atoi( pbuf ) );
			}
			else {
				sydata_set(NULL);
			}
		}
	}

	if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) ) {
		if( (pn = get_prevpn( get_nowpn())) != -1 )	{
			set_focus( pn );
			return 0;
		}
		else {
			to_parent( m_pParent, VK_LEFT );
			return 0;
		}
	}

	switch( nChar )
	{
	case VK_LEFT:
		if( ( pn = get_prevpn( get_nowpn()) ) == SY_SYZSWKBN_PN )
		{
			set_focus( pn );
			return 0;
		}
		to_parent( m_pParent, nChar );
		return 0;
	case VK_RIGHT:
		if( ( pn = get_nextpn( get_nowpn()) ) != -1  )
		{
			set_focus( pn );
			return 0;
		}
		to_parent( m_pParent, nChar );
		return 0;
	case VK_DOWN:	// 下 矢印で 売上／仕入区分 へ
		if( ( pn = get_nextpn( SY_SZKBN_PN ) ) != -1 )
		{
			set_focus( pn );
			return 0;
		}
		to_parent( m_pParent, nChar );
		return 0;
	case VK_RETURN:
		if( (pn = get_prevpn( get_nowpn() )) == SY_SZKBN_PN )
		{
			set_focus( pn );
			return 0;
		}
	case VK_TAB:
//	case VK_F3:
		if( (pn = get_nextpn( get_nowpn(), nChar)) != -1 )
		{
			set_focus( pn );
			return 0;
		}
		else
		{
			to_parent( m_pParent, nChar );
			return 0;
		}
		break;

	case VK_UP:
	case VK_F2:
		if( (pn = get_prevpn( get_nowpn())) != -1 )
		{
			set_focus( pn );
			return 0;
		}
		else
		{
			to_parent( m_pParent, nChar );
			return 0;
		}
		break;

	default:
		return 1;
		break;
	}
}

// 税区分
int CSkbnIn::OnTerminationDBEdt3(long nChar, long inplen, long kst) 
{
	DBGFUNCTION;
	int i,pn;
	char pbuf[50];
	VARIANT var;

	if( m_THIS_DIALOG_OK == FALSE )
		return 1;

	switch( nChar )
	{
	case NULL:
	case VK_RIGHT:
	case VK_RETURN:
	case VK_TAB:
	case VK_DOWN:
//	case VK_F3:
	case VK_F2:
	case VK_UP:
	case VK_LEFT:
		if( inplen ) {
			if( m_MODEFY_OK ) {
				pbuf[0] = '\0';
				DBdata_get( this, getINP_CTL( SY_SZKBN_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
				strcpy_s( pbuf, sizeof pbuf, (char*)var.pbVal );

				for( i = 0 ; pbuf[i] ; ++i )
				{
					if( isdigit( pbuf[i] ) )
						continue;
					else
					{
						pbuf[0] = '\0';
						break;
					}
				}
				SetSkbnSelNo( atoi( pbuf ) );
			}
			else {
				sydata_set(NULL);
			}
		}
	}

	if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) ) {
		if( (pn = get_prevpn( get_nowpn())) != -1 )	{
			set_focus( pn );
			return 0;
		}
		else {
			to_parent( m_pParent, VK_LEFT );
			return 0;
		}
	}

	switch( nChar )
	{
	case VK_RIGHT:
		if( ( pn = get_nextpn( get_nowpn())) != -1 )
		{
			set_focus( pn );
			return 0;
		}
		else {
			to_parent( m_pParent, nChar );
		}
		return 0;
	case VK_DOWN:
		//to_parent( m_pParent, nChar );
		if(( pn = get_nextpn( get_nowpn())) != -1 )	{
			set_focus( pn );
			return 0;
		}
		else {
			to_parent( m_pParent, nChar );
		}
		return 0;

	case VK_RETURN:
		if( (pn = get_prevpn( get_nowpn() )) == SY_SZKBN_PN )
		{
			set_focus( pn );
			return 0;
		}
	case VK_TAB:
//	case VK_F3:
		if( (pn = get_nextpn( get_nowpn(), nChar)) != -1 )
		{
			set_focus( pn );
			return 0;
		}
		else
		{
			to_parent( m_pParent, nChar );
			return 0;
		}
		break;
	case VK_UP:
		if( (pn = get_prevpn( SY_ZRITU_PN )) != -1 )
		{
			set_focus( pn );
			return 0;
		}
		else
		{
			to_parent( m_pParent, nChar );
			return 0;
		}
		break;
	case VK_LEFT:
	case VK_F2:
		if( (pn = get_prevpn( get_nowpn())) != -1 )
		{
			set_focus( pn );
			return 0;
		}
		else
		{
			to_parent( m_pParent, nChar );
			return 0;
		}
		break;

	default:
		return 1;
		break;
	}
}


// 売上仕入 区分
int CSkbnIn::OnTerminationDBEdt4(long nChar, long inplen, long kst) 
{
	DBGFUNCTION;
	int i,pn;
	char pbuf[50];
	VARIANT var;

	if( m_THIS_DIALOG_OK == FALSE )
		return 1;

	switch( nChar )
	{
	case NULL:
	case VK_RIGHT:
	case VK_RETURN:
	case VK_TAB:
	case VK_DOWN:
//	case VK_F3:
	case VK_F2:
	case VK_UP:
	case VK_LEFT:
		if( inplen ) {
			if( m_MODEFY_OK ) {
				pbuf[0] = '\0';
				DBdata_get( this, getINP_CTL( SY_URISIRE_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
				strcpy_s( pbuf, sizeof pbuf, (char*)var.pbVal );

				for( i = 0 ; pbuf[i] ; ++i )
				{
					if( isdigit( pbuf[i] ) )
						continue;
					else
					{
						pbuf[0] = '\0';
						break;
					}
				}
				SetSkbnSelNo( atoi( pbuf ) );
			}
			else {
				sydata_set(NULL);
			}
		}
	}

	if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) ) {
		if( (pn = get_prevpn( get_nowpn())) != -1 )	{
			set_focus( pn );
			return 0;
		}
		else {
			to_parent( m_pParent, VK_LEFT );
			return 0;
		}
	}

	switch( nChar )
	{
	case VK_LEFT:
		if( ( pn = get_prevpn( get_nowpn()) ) == SY_SZKBN_PN )
		{
			set_focus( pn );
			return 0;
		}
	case VK_RIGHT:
		//to_parent( m_pParent, nChar );
		if(( pn = get_nextpn( get_nowpn(), nChar )) != -1 ) {
			set_focus( pn );
			return 0;
		}
		else {
		to_parent( m_pParent, nChar );
		return 0;
		}
		return 0;

	case VK_DOWN:
		//to_parent( m_pParent, nChar );
		if(( pn = get_nextpn( get_nowpn(), nChar )) != -1 ) {
			set_focus( pn );
			return 0;
		}
		else {
		to_parent( m_pParent, nChar );
		return 0;
		}
		return 0;

	case VK_RETURN:
	//	if( (pn = get_prevpn( get_nowpn() )) == SY_SZKBN_PN )
	//	{
	//		set_focus( pn );
	//		return 0;
	//	}
	case VK_TAB:
//	case VK_F3:
		if( (pn = get_nextpn( get_nowpn(), nChar)) != -1 )
		{
			set_focus( pn );
			return 0;
		}
		else
		{
			to_parent( m_pParent, nChar );
			return 0;
		}
		break;

	case VK_UP:
	case VK_F2:
		if( (pn = get_prevpn( get_nowpn())) != -1 )
		{
			set_focus( pn );
			return 0;
		}
		else
		{
			to_parent( m_pParent, nChar );
			return 0;
		}
		break;

	default:
		return 1;
		break;
	}
}



//消費税選択後の移動
void CSkbnIn::set_nextpos()
{
	DBGFUNCTION;

	switch( get_nowpn() ) {
		//追加消費税区分
		case SY_SYZSWKBN_PN:
			OnTerminationDBEdt1( VK_RETURN, 0, 0 );
			break;
		//税率
		case SY_ZRITU_PN:
			OnTerminationDBEdt2( VK_RETURN, 0, 0 );
			break;
		//消費税区分
		case SY_SZKBN_PN:
			OnTerminationDBEdt3( VK_RETURN, 0, 0 );
			break;
		//売上/仕入区分
		case SY_URISIRE_PN:
			OnTerminationDBEdt4( VK_RETURN, 0, 0 );
			break;
		//取得・譲渡
		case SY_TAISEL_PN:
			OnTerminationDBEdt5( VK_RETURN, 0, 0 );
			break;
		//対価
		case SY_TAIKA_PN:
			OnTerminationDBEdt6( VK_RETURN, 0, 0 );
			break;
		// 特定収入
		case SY_TOKUS_PN:
			OnTerminationDBEdtToku( VK_RETURN, 0, 0 );
			break;
			// 登録番号
		case SY_INV_PN:
			OnTerminationDBEdtInv( VK_RETURN, 0, 0 );
			break;
			// 免税事業者からの課税仕入れ
		case SY_MENZEI_PN:
			OnTerminationCheckMenzei( VK_RETURN );
			break;
	}
}


// 取得・譲渡の選択
int CSkbnIn::OnTerminationDBEdt5(long nChar, long inplen, long kst) 
{
	DBGFUNCTION;
	int i,pn;
	char pbuf[50];
	VARIANT var;

	if( m_THIS_DIALOG_OK == FALSE )
		return 1;

	switch( nChar )
	{
	case NULL:
	case VK_RIGHT:
	case VK_RETURN:
	case VK_TAB:
	case VK_DOWN:
//	case VK_F3:
	case VK_F2:
	case VK_UP:
	case VK_LEFT:
		if( inplen ) {
			if( m_MODEFY_OK ) {
				pbuf[0] = '\0';
				DBdata_get( this, getINP_CTL( SY_TAISEL_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
				strcpy_s( pbuf, sizeof pbuf, (char*)var.pbVal );

				for( i = 0 ; pbuf[i] ; ++i )
				{
					if( isdigit( pbuf[i] ) )
						continue;
					else
					{
						pbuf[0] = '\0';
						break;
					}
				}
				SetSkbnSelNo( atoi( pbuf ) );
			}
			else {
				sydata_set(NULL);
			}
		}
	}

	if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) ) {
		if( (pn = get_prevpn( get_nowpn())) != -1 )	{
			set_focus( pn );
			return 0;
		}
		else {
			to_parent( m_pParent, VK_LEFT );
			return 0;
		}
	}

	switch( nChar )
	{
	case VK_LEFT:
		if( ( pn = get_prevpn( get_nowpn()) ) == SY_SZKBN_PN )
		{
			set_focus( pn );
			return 0;
		}
	case VK_RIGHT:
		to_parent( m_pParent, nChar );
		return 0;

	case VK_DOWN:
	case VK_RETURN:
	case VK_TAB:
//	case VK_F3:
		if( (pn = get_nextpn( get_nowpn(), nChar)) != -1 )
		{
			set_focus( pn );
			return 0;
		}
		else
		{
			to_parent( m_pParent, nChar );
			return 0;
		}
		break;

	case VK_UP:
	case VK_F2:
		if( (pn = get_prevpn( get_nowpn())) != -1 )
		{
			set_focus( pn );
			return 0;
		}
		else
		{
			to_parent( m_pParent, nChar );
			return 0;
		}
		break;

	default:
		return 1;
		break;
	}
}


// 対価入力
int CSkbnIn::OnTerminationDBEdt6(long nChar, long inplen, long kst) 
{
	DBGFUNCTION;
	int		pn;
	char pbuf[50];
	VARIANT var;

	if( m_THIS_DIALOG_OK == FALSE )
		return 1;

	switch( nChar )
	{
	case NULL:
	case VK_RIGHT:
	case VK_RETURN:
	case VK_TAB:
	case VK_DOWN:
//	case VK_F3:
	case VK_F2:
	case VK_UP:
	case VK_LEFT:
		if (inplen == 0) { //電卓からの場合、inplen == 0 で来るので金額をチェックする。
			CString tai1, tai2;
			tai1 = m_pZm->dbdata->taika;
			tai1.TrimLeft();

			DBdata_get(this, getINP_CTL(SY_TAIKA_PN)->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0);

			LPSTR ptai = tai2.GetBuffer(64);
			::ZeroMemory(ptai, sizeof 64);
			l_print(ptai, var.pbVal, "sssssssssss9");
			tai2.ReleaseBuffer();
			tai2.TrimLeft();
			if (tai1.CompareNoCase(tai2)) {
				inplen = 1;
			}
		}
		if( inplen ) {
			if( m_MODEFY_OK ) {
				pbuf[0] = '\0';
				DBdata_get( this, getINP_CTL( SY_TAIKA_PN )->IDC_X, &var, ICSDBEDT_TYPE_ARITH, 0 );
				::ZeroMemory( pbuf, sizeof pbuf );
				l_print( pbuf, var.pbVal, "sssssssssss9" );

#ifdef TAIKA_VER
				m_pSy->SyMsg( &m_smpk );
				struct _SY_MSG_TBL* psmt;
				psmt = m_smpk.SY_MSG_TAIKAKBN;
				if( psmt->sel_no > 0 ) {
					int taisgn = 0;

					if( psmt->msg_tbl[psmt->sel_no-1]->code == CD_TAIKA ) {
						taisgn = 2;
					}
					else if( psmt->msg_tbl[psmt->sel_no-1]->code == CD_SYUTOK ) {
						strcpy_s( pbuf, sizeof pbuf, "0" );
						taisgn = 1;
					}

					if( taisgn > 0 ) {
						m_pSy->SyTaikaVal( pbuf );
						GetParent()->SendMessage( WM_SKBN_LINEDISP, 0 );
					}
				}
#endif
			}
			else {
				sydata_set(NULL);
			}
		}
	}

	if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) ) {
		if( (pn = get_prevpn( get_nowpn())) != -1 )	{
			set_focus( pn );
			return 0;
		}
		else {
			to_parent( m_pParent, VK_LEFT );
			return 0;
		}
	}

	switch( nChar )
	{
	case VK_LEFT:
		if( ( pn = get_prevpn( get_nowpn()) ) == SY_TAISEL_PN )
		{
			set_focus( pn );
			return 0;
		}
	case VK_RIGHT:
		to_parent( m_pParent, nChar );
		return 0;

	case VK_DOWN:
	case VK_RETURN:
	case VK_TAB:
//	case VK_F3:
		if( (pn = get_nextpn(get_nowpn())) != -1 )
		{
			set_focus(pn);
			return 0;
		}
		else
		{
			to_parent(m_pParent, nChar);
		}
		return 0;

	case VK_UP:
	case VK_F2:
		if( (pn = get_prevpn( get_nowpn())) != -1 )
		{
			set_focus( pn );
			return 0;
		}
		else
		{
			to_parent( m_pParent, nChar );
			return 0;
		}
		break;

	default:
		return 1;
		break;
	}
}


// 特定収入[06.03 /08]
int CSkbnIn::OnTerminationDBEdtToku(long nChar, long inplen, long kst) 
{
	DBGFUNCTION;
	int i,pn;
	char pbuf[50];
	VARIANT var;

	if( m_THIS_DIALOG_OK == FALSE )
		return 1;

	switch( nChar )
	{
	case NULL:
	case VK_RIGHT:
	case VK_RETURN:
	case VK_TAB:
	case VK_DOWN:
//	case VK_F3:
	case VK_F2:
	case VK_UP:
	case VK_LEFT:
		if( inplen ) {
			if( m_MODEFY_OK ) {
				pbuf[0] = '\0';
				DBdata_get( this, getINP_CTL( SY_TOKUS_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
				strcpy_s( pbuf, sizeof pbuf, (char*)var.pbVal );

				for( i = 0 ; pbuf[i] ; ++i )
				{
					if( isdigit( pbuf[i] ) )
						continue;
					else
					{
						pbuf[0] = '\0';
						break;
					}
				}
				SetSkbnSelNo( atoi( pbuf ) );
			}
			else {
				sydata_set(NULL);
			}
		}
	}

	if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) ) {
		if( (pn = get_prevpn( get_nowpn())) != -1 )	{
			set_focus( pn );
			return 0;
		}
		else {
			to_parent( m_pParent, VK_LEFT );
			return 0;
		}
	}

	switch( nChar )
	{
	case VK_LEFT:
		if( ( pn = get_prevpn( get_nowpn()) ) == SY_SZKBN_PN )
		{
			set_focus( pn );
			return 0;
		}
	case VK_RIGHT:
		to_parent( m_pParent, nChar );
		return 0;

	case VK_DOWN:
		to_parent( m_pParent, nChar );
		return 0;

	case VK_RETURN:
	case VK_TAB:
//	case VK_F3:
		if( (pn = get_nextpn( get_nowpn(), nChar)) != -1 )
		{
			set_focus( pn );
			return 0;
		}
		else
		{
			to_parent( m_pParent, nChar );
			return 0;
		}
		break;

	case VK_UP:
	case VK_F2:
		if( (pn = get_prevpn( get_nowpn())) != -1 )
		{
			set_focus( pn );
			return 0;
		}
		else
		{
			to_parent( m_pParent, nChar );
			return 0;
		}
		break;

	default:
		return 1;
		break;
	}
}


BOOL CSkbnIn::PreTranslateMessage(MSG* pMsg) 
{
//	DBGFUNCTION;
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	if( (m_hAccelTable != NULL) && ::TranslateAccelerator( m_pParent->m_hWnd, m_hAccelTable, pMsg))
	{
		return TRUE;
	}

	if( pMsg->message == WM_KEYDOWN ) {
		CWnd *pWnd = GetFocus();
		if( pWnd ) {
			int id = pWnd->GetDlgCtrlID();
			if( id == IDC_CHECK_MENZEI ) {
				set_nowpn( SY_MENZEI_PN );
				OnTerminationCheckMenzei( pMsg->wParam );
				return TRUE;
			}
			if( id == IDC_ICSDBEDT_INV ){
				invmng.KanaCheckVK_BACK(pMsg);
				//if( invmng.KanaCheckVK_BACK(pMsg) )
				//	return 1;
			}
		}
	}
	else if( pMsg->message == WM_KEYUP ) {
		CWnd *pWnd = GetFocus();
		if( pWnd ) {
			int id = pWnd->GetDlgCtrlID();
			if( id == IDC_ICSDBEDT_INV ) {
				if( invmng.FukuKanaSearch(pMsg) )
					return 1;
			}
		}
	}
	else if( pMsg->message == WM_LBUTTONDOWN ) {
		CWnd *pWnd = GetFocus();
		if( pWnd ) {
			int id = pWnd->GetDlgCtrlID();
			if( id == IDC_ICSDBEDT_INV ) {
				invmng.CancelIME();
			}
		}
	}



	return ICSDialog::PreTranslateMessage(pMsg);
}


// 消費税ダイアログ右側・下側に余白があるかをチェック
//	bVert ：(TRUE) 下側に余白あり
//	bHorz ：(TRUE) 右側に余白あり
//
void CSkbnIn::DlgRectCheck( BOOL& bVert, BOOL& bHorz, CRect* zkbnRect, int* width)
{
	DBGFUNCTION;
	int typ = GetMultiWindowType();
//MyTrace( "@CSkbnIn::DlgRectCheck TOP multi (%d)\n", typ );

	bVert = FALSE;
	bHorz = FALSE;

	CRect rc1, rc2, rcWnd;
	GetDlgItem( IDC_ICSDBEDTCTRL3 )->GetWindowRect( rc1 );
//MyTrace("@CSkbnIn::DlgRectCheck zeiKbn rect (%d, %d, %d, %d)\n", rc1.left, rc1.top, rc1.right, rc1.bottom );

	m_pParent->ScreenToClient( rc1 );
	*zkbnRect = rc1;

	//コントロールから幅を取得
	GetDlgItem(IDC_STATIC2)->GetWindowRect(rc2);
	m_pParent->ScreenToClient(rc2);
	*width = rc1.right - rc2.left;

	//マルチウィンドウでない
	if( typ == MULTIWINDOW_NOP || typ == MULTIWINDOW_NONE ) {
		return;
	}
//MyTrace("@CSkbnIn::DlgRectCheck zeiKbn rect (%d, %d, %d, %d)\n", rc1.left, rc1.top, rc1.right, rc1.bottom);

	// インボイス対応
	// 底を対価から免税事業者からの課税仕入れに変更
	//GetDlgItem( IDC_ICSDBEDTCTRL6 )->GetWindowRect( rc2 );
	//GetDlgItem( IDC_CHECK_MENZEI )->GetWindowRect( rc2 );
	GetDlgItem(IDC_ICSDBEDT_MEN)->GetWindowRect(rc2);
	m_pParent->ScreenToClient( rc2 );

	GetWindowRect( rcWnd );
	m_pParent->ScreenToClient( rcWnd );

#ifdef TEST_CLOSE
CRect rcTmp;
rcTmp = rc1;
MyTrace( "@CSyzInpSub::DlgRectCheck[ Ctrl(1) ] %d, %d, %d, %d, [w = %d, h = %d]\n", rcTmp.left, rcTmp.top, rcTmp.right, rcTmp.bottom,
		rcTmp.Width(), rcTmp.Height() );
rcTmp = rc2;
MyTrace( "@CSyzInpSub::DlgRectCheck[ Ctrl(2) ] %d, %d, %d, %d, [w = %d, h = %d]\n", rcTmp.left, rcTmp.top, rcTmp.right, rcTmp.bottom,
		rcTmp.Width(), rcTmp.Height() );
rcTmp = rcWnd;
MyTrace( "@CSyzInpSub::DlgRectCheck[ DlgWnd ] %d, %d, %d, %d, [w = %d, h = %d]\n", rcTmp.left, rcTmp.top, rcTmp.right, rcTmp.bottom,
		rcTmp.Width(), rcTmp.Height() );
#endif

	int chkwd = rc1.Width();
	int chkht = rc2.Height();

	if( (rcWnd.right - rc1.right) > chkwd ) {
		bHorz = TRUE;
	}

	if( (rcWnd.bottom - rc2.bottom) > chkht ) {
		bVert = TRUE;
	}

//MyTrace( "@CSyzInpSub::DlgRectCheck END [v = %d, h = %d]\n", bHorz, bVert );
}


int CSkbnIn::GetDlgWidth()
{
	DBGFUNCTION;
	CRect rc1, rc2, rcWnd;
	GetDlgItem(IDC_ICSDBEDTCTRL3)->GetWindowRect(rc1);

	GetDlgItem(IDC_STATIC1)->GetWindowRect(rc2);

	GetWindowRect(rcWnd);

	//余白分
	int sps = rc2.left - rcWnd.left;

//CRect rcTmp;
//rcTmp = rc1;
//MyTrace("@CSyzInpSub::GetDlgWidth[ 税区分DBEDIT ] %d, %d, %d, %d, [w = %d, h = %d]\n", rcTmp.left, rcTmp.top, rcTmp.right, rcTmp.bottom,
//	rcTmp.Width(), rcTmp.Height());
//rcTmp = rcWnd;
//MyTrace("@CSyzInpSub::GetDlgWidth[ DlgWnd ] %d, %d, %d, %d, [w = %d, h = %d]\n", rcTmp.left, rcTmp.top, rcTmp.right, rcTmp.bottom,
//	rcTmp.Width(), rcTmp.Height());

	return 	(rc1.right - rcWnd.left) + sps;
}


int CSkbnIn::GetDlgHeight()
{
	DBGFUNCTION;
	CRect rc1, rc2, rcWnd;
	// インボイス対応
	// 底を対価から免税事業者からの課税仕入れに変更
	//GetDlgItem(IDC_ICSDBEDTCTRL6)->GetWindowRect(rc1);
	//GetDlgItem(IDC_CHECK_MENZEI)->GetWindowRect(rc1);
	GetDlgItem(IDC_ICSDBEDT_MEN)->GetWindowRect(rc1);

	GetDlgItem(IDC_STATIC1)->GetWindowRect(rc2);

	GetWindowRect(rcWnd);

	//余白分
	int sps = rc2.top - rcWnd.top;

	return 	(rc1.bottom - rcWnd.top) + sps;
}


//=============================================================================
// インボイスバージョンアップマスターでなければ、ダイアログを縮小する
//=============================================================================
void CSkbnIn::MoveInvCtrl()
{
	DBGFUNCTION;
	bool bAll = false;

	if( m_InpMode == INPMODE_DENP ) {
		// 旧のコンストラクタが呼び出された場合、もしくは伝票入力の場合
		// 登録番号と免税事業者からの課税仕入れを移動
		bAll = true;
	}
	else {
		// 通常入力の場合
		if( m_bInvMaster && m_bInvUse && m_bInvDisp ) {
			// バージョンアップ済みかつ登録番号を表示する場合は何もしない
			return;
		}

		if( !m_bInvMaster || !m_bInvUse ) {
			// バージョンアップマスターでない場合、登録番号と免税事業者からの課税仕入れを移動
			bAll = true;
		}
		else {
			if( !m_bInvDisp ) {
				// 登録番号を表示しないマスターの場合、登録番号を移動
				bAll = false;
			}
		}
	}

	MoveInvCtrl( bAll );
}

//=============================================================================
// インボイスバージョンアップマスターでなければ、ダイアログを縮小する
//=============================================================================
void CSkbnIn::MoveInvCtrl( bool bAll )
{
	DBGFUNCTION;
	CRect rc1, rc2, rect;
	int	mv = 0;

	// 移動する基準となるコントロール
	GetDlgItem( IDC_STATIC18 )->GetWindowRect( rc1 );
	ScreenToClient( rc1 );

	int mrgn = 4;

	if( bAll ) {
		// 登録番号、免税事業者からの課税仕入れを表示しない

		// ダイアログの底を登録番号のトップの位置に
		GetWindowRect( rc2 );

		int typ = GetMultiWindowType();
		if( typ == MULTIWINDOW_NOP || typ == MULTIWINDOW_NONE ) {
		}
		else {
			CRect rcDB3;
			GetDlgItem(IDC_ICSDBEDTCTRL3)->GetWindowRect(rcDB3);

			GetWindowRect(rc2);

			if( rc2.right < rcDB3.right )
				rc2.right = rcDB3.right;
		}

		MoveWindow( rc2.left, rc2.top, rc2.Width(), rc1.top + mrgn );
	}
	else {
			// 登録番号のみ表示しない

			// チェックボックスを登録番号の位置に
			GetDlgItem( IDC_CHECK_MENZEI )->GetWindowRect( rc2 );
			ScreenToClient( rc2 );
			
			mv = ( rc2.top - rc1.top );
			GetDlgItem( IDC_CHECK_MENZEI )->MoveWindow( rc2.left, rc2.top - mv, rc2.Width(), rc2.Height() );

			// 免税事業者　課税割合
			GetDlgItem(IDC_ICSDBEDT_MEN)->GetWindowRect(rc2);
			ScreenToClient(rc2);
			GetDlgItem(IDC_ICSDBEDT_MEN)->MoveWindow(rc2.left, rc2.top - mv, rc2.Width(), rc2.Height());

			GetDlgItem(IDC_STATIC20)->GetWindowRect(rc2);
			ScreenToClient(rc2);
			GetDlgItem(IDC_STATIC20)->MoveWindow(rc2.left, rc2.top - mv, rc2.Width(), rc2.Height());

			// ダイアログの底をチェックボックスの底の位置に
			GetWindowRect( rc2 );
			MoveWindow( rc2.left, rc2.top, rc2.Width(), rc2.Height() - mv + mrgn);
		}
	}

//=============================================================================
// インボイスコントロール表示非表示切り替え
//=============================================================================
void CSkbnIn::ShowInvCtrl()
{
	DBGFUNCTION;
	bool bAll = false;

	if( m_InpMode == INPMODE_DENP ) {
		// 旧のコンストラクタが呼び出された場合、もしくは伝票入力の場合
		// 登録番号と免税事業者からの課税仕入れを非表示
		bAll = true;
	}
	else {
		// 通常入力の場合
		if( m_bInvMaster && m_bInvUse && m_bInvDisp ) {
			// バージョンアップ済みかつ登録番号を表示する場合は何もしない
			return;
		}

	// インボイス対応
	if( !m_bInvMaster || !m_bInvUse ) {
		// バージョンアップマスターでない場合、登録番号と免税事業者からの課税仕入れを非表示
			bAll = true;
		}
		else {
			if( !m_bInvDisp ) {
				// 登録番号を表示しないマスターの場合、登録番号を非表示
				bAll = false;
			}
		}
	}

	ShowInvCtrl( bAll );
}

//=============================================================================
// インボイスコントロール表示非表示切り替え
//=============================================================================
void CSkbnIn::ShowInvCtrl( bool bAll )
{
	DBGFUNCTION;
	// インボイス対応
	if( bAll ) {
		// インボイスコントロール全非表示
		int ctrlId[] = { IDC_STATIC18, IDC_ICSDBEDT_INV, IDC_ICSDBEDT_INVDISP, IDC_CHECK_MENZEI, IDC_STATIC20, IDC_ICSDBEDT_MEN, 0 };
		for( int i= 0; ctrlId[i] != 0; i++ ) {
			GetDlgItem( ctrlId[i] )->ShowWindow( SW_HIDE );
		}
	}
	else {
		// 登録番号のみ非表示
			int ctrlId[] = { IDC_STATIC18, IDC_ICSDBEDT_INV, IDC_ICSDBEDT_INVDISP,0 };
			for( int i= 0; ctrlId[i] != 0; i++ ) {
				GetDlgItem( ctrlId[i] )->ShowWindow( SW_HIDE );
			}
		}
	}


//=============================================================================
// 登録番号ターミネーション
//=============================================================================
int CSkbnIn::OnTerminationDBEdtInv( long nChar, long inplen, long kst )
{
	DBGFUNCTION;
//	MyTrace("@%s: ----nChar(%d), inplen(%d)\n", __FUNCTION__, nChar, inplen );

	int pn = 0, ncnt = 0;
	char pbuf[50] = { 0 };
	VARIANT var;

	if( m_THIS_DIALOG_OK == FALSE ) {
		return 1;
	}

	if( nChar == 0 && m_bMsgDsp ) {
		return 1;
	}

	switch( nChar ) {
	case NULL:
	case VK_RIGHT:
	case VK_RETURN:
	case VK_TAB:
	case VK_DOWN:
	case VK_F2:
	case VK_UP:
	case VK_LEFT:
	case VK_INSERT:
	case 0x16:	//貼り付け
		if( m_MODEFY_OK ) {
			if( nChar == 0x16 && inplen == 0 ) { // 貼り付けの場合はinplenが0になる
				CString dbInvno, edtInvno;
				dbInvno = m_pZm->dbdata->invno;
				if( !dbInvno.IsEmpty() ) {
					dbInvno.Replace(_T("T"), _T(""));
				}

				DBdata_get( this, getINP_CTL( SY_INV_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
				edtInvno = var.pbVal;
				if( dbInvno.CompareNoCase( edtInvno )) {
					inplen = 1;
				}
			}
			else if( inplen == 0 ) {
				CString dbInvno, edtInvno;
				DBdata_get(this, getINP_CTL(SY_INV_PN)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0);
				edtInvno = var.pbVal;

			//	MyTrace("@%s ----edtInvno(%s)\n", __FUNCTION__, edtInvno);

				if( edtInvno.GetLength() > 0 ) {
					inplen = 1;
				}
			}

			if( inplen > 0 || inplen < 0 ) {
				pbuf[0] = '\0';
				DBdata_get( this, getINP_CTL( SY_INV_PN )->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
				sprintf_s( pbuf, sizeof( pbuf ), _T( "%s" ), ( char* )var.pbVal );

				if( inplen < 0 ) {
					//全部消した場合
					if( pbuf[0] == 0 ) {
						// Delete, Back Spaceで削除すると-1となる
						m_bMsgDsp = TRUE;
						//メッセージ出さない
						m_pZm->dbdata->invno.Empty();
						var.pbVal = NULL;
						DBdata_set(this, getINP_CTL(SY_INV_PN)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0);
						sydata_set(NULL);
						GetParent()->SendMessage(WM_SKBN_LINEDISP, 0);
						m_bMsgDsp = FALSE;
						goto LABEL_KEY;
					}
				}

				for( int i = 0; pbuf[i] != '\0'; i++ ) {
					if( isdigit( pbuf[i] )) {
						ncnt++;
						continue;
					}
					else {
						pbuf[i] = '\0';
						break;
					}
				}

				if(( ncnt >= 1 ) && ( ncnt <= 2 )) {
					// 摘要情報から登録番号を取得
					SetSkbnSelNo( atoi( pbuf ));
				}
				else {
					CString checkInvno;
					checkInvno.Format( _T( "T%s" ), pbuf );
					if( syzInvoice.CheckInvoiceNumber( checkInvno ) == -1 ) {
						//m_pZm->dbdata->invno.Empty();
						m_bIgnoreFocus = TRUE;
					//	var.pbVal = (BYTE*)0;
					//	var.pbVal = (BYTE*)pbuf;
					//	ICSMessageBox( syzInvoice.GetErrorMessage(), MB_ICONSTOP );
						m_ErrMsg = syzInvoice.GetErrorMessage();
						PostMessage(WM_MYMESSAGE, INV_ERRMSG);

						var.pbVal = (BYTE*)pbuf;
						DBdata_set(this, getINP_CTL(SY_INV_PN)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0);
					//	DBSetDispString(this, getINP_CTL(SY_INV_PN)->IDC_X, "");
						DBSetDispString(this, IDC_ICSDBEDT_INVDISP, "");
					//	set_focus( SY_INV_PN );
					//	m_bIgnoreFocus = FALSE;
						return 0;
					}
					else {
						m_pZm->dbdata->invno = checkInvno;
						DBSetDispString( this, IDC_ICSDBEDT_INVDISP/*getINP_CTL( SY_INV_PN )->IDC_X*/, checkInvno );
						SetSkbnSelNo( -1 );
						GetParent()->SendMessage(WM_SKBN_LINEDISP, 0);
					}
				}
			}
			else {
#ifdef CLOSE	//SetDisplayStringを別DBEditにしたため
				if( inplen < 0 ) {
					// Delete, Back Spaceで削除すると-1となる
				//	CString dmyInvno;
				//	syzInvoice.CheckInvoiceNumber(dmyInvno);
					m_bMsgDsp = TRUE;
					//メッセージ出さない
				//	ICSMessageBox(syzInvoice.GetErrorMessage(), MB_ICONSTOP); //必ずエラーになる
					m_pZm->dbdata->invno.Empty();
					var.pbVal = NULL;
					DBdata_set(this, getINP_CTL(SY_INV_PN)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0);
					sydata_set(NULL);
					GetParent()->SendMessage(WM_SKBN_LINEDISP, 0);
					set_focus(SY_INV_PN);
					nChar = 0;
					m_bMsgDsp = FALSE;
				}
#endif
			}
		}
		else {
			sydata_set( NULL );
		}
		break;
	case VK_DELETE:
		if( m_MODEFY_OK ) {
			m_pZm->dbdata->invno.Empty();
			var.pbVal = NULL;
			DBdata_set(this, getINP_CTL(SY_INV_PN)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0);
			GetParent()->SendMessage(WM_SKBN_LINEDISP, 0);
			sydata_set(NULL);
		}
		break;
	case VK_HOME:
		invmng.InvNoSearch(NULL);
		break;
	default:
		break;
	}

LABEL_KEY:
	if(( nChar == VK_TAB ) && ( kst & ICSDBEDT_KST_SHIFT )) {
		if(( pn = get_prevpn( get_nowpn())) != -1 )	{
			set_focus( pn );
			return 0;
		}
		else {
			to_parent( m_pParent, VK_LEFT );
			return 0;
		}
	}

	switch( nChar ) {
#ifdef CLOSE
	case VK_RIGHT:
		to_parent( m_pParent, nChar );
		return 0;

	case VK_DOWN:
		to_parent( m_pParent, nChar );
		return 0;
#endif
	case VK_RIGHT:
	case VK_DOWN:
	case VK_RETURN:
	case VK_TAB:
		if(( pn = get_nextpn( get_nowpn(), nChar )) != -1 ) {
			set_focus( pn );
			return 0;
		}
		else {
			to_parent( m_pParent, nChar );
			return 0;
		}
		break;

	case VK_LEFT:
	case VK_UP:
	case VK_F2:
		if(( pn = get_prevpn( get_nowpn() )) != -1 ) {
			set_focus( pn );
			return 0;
		}
		else {
			to_parent( m_pParent, nChar );
			return 0;
		}
		break;
	case VK_HOME:
		invmng.InvDispPage(0);
		break;
	case VK_PRIOR:
		invmng.InvDispPage(-1);
		break;
	case VK_NEXT:
		invmng.InvDispPage(1);
		break;
	default:
		return 1;
		break;
	}

	return 0;
}

//=============================================================================
// 免税事業者からの課税仕入れターミネーション
//=============================================================================
int CSkbnIn::OnTerminationCheckMenzei( DWORD nChar )
{
	DBGFUNCTION;
	int pn = 0;
	int st;

	if( m_THIS_DIALOG_OK == FALSE )
		return 1;

	if( ::GetKeyState(VK_SHIFT) < 0 ) {
		if( nChar == VK_TAB ) {
			nChar = VK_LEFT;
		}
	}

	switch( nChar )	{
	case VK_RIGHT:
		to_parent( m_pParent, nChar );
		return 0;

	case VK_RETURN:
	case VK_TAB:
	case VK_DOWN:
		if(( pn = get_nextpn( get_nowpn(), nChar )) != -1 ) {
			set_focus( pn );
			return 0;
		}
		else {
			to_parent( m_pParent, nChar );
			return 0;
		}
		return 0;
	case VK_LEFT:
	case VK_UP:
	case VK_F2:
		if(( pn = get_prevpn( get_nowpn() )) != -1 ) {
			set_focus( pn );
			return 0;
		}
		else {
			to_parent( m_pParent, nChar );
			return 0;
		}
		break;
	case VK_SPACE:
		SetMenzeiCheck();
		MenCheckButtonJob(0);
		break;
	case '1': case VK_NUMPAD1:
	case '2': case VK_NUMPAD2:
		st = m_MenzeiCheck.GetCheck();
		if( st == BST_UNCHECKED ) {
			SetMenzeiCheck();
		}
		MenCheckButtonJob(nChar);
		break;

	default:
		return 1;
		break;
	}

	return 0;
}

//=============================================================================
// 免税事業者からの課税仕入れのチェック切り替え
//=============================================================================
void CSkbnIn::SetMenzeiCheck()
{
	DBGFUNCTION;
	int check = 0, st = 0;
	st = m_MenzeiCheck.GetCheck();

	if( st == BST_CHECKED ) {
		check = BST_UNCHECKED;
	}
	else {
		check = BST_CHECKED;
	}

	m_MenzeiCheck.SetCheck( check );
}

void CSkbnIn::OnBnClickedMenzei()
{
	DBGFUNCTION;
	MenCheckButtonJob(0);
}

void CSkbnIn::MenCheckButtonJob(long nChar)
{
	if( m_MODEFY_OK ) {
		int st = m_MenzeiCheck.GetCheck();

		if( st == BST_CHECKED ) {
			// 免税事業者からの仕入れ
			DWORD fnc = 0;
			if( nChar == 0 ) {
				if( m_pZm->dbdata->ddate > SYZMEN4_YMD ) {
					fnc = CD_SP_EXEMPT_00;
				}
				else if( m_pZm->dbdata->ddate >= SYZMEN3_YMD ) {
					fnc = CD_SP_EXEMPT_50;
				}
				else {
					fnc = CD_SP_EXEMPT;
				}
			}
			else {
				if( m_Koujo80_50 == SYZ_MEN_80 ) {
					if( nChar == '1' || nChar == VK_NUMPAD1 ) {
						fnc = CD_SP_EXEMPT;
					}
				}
				else if( m_Koujo80_50 == SYZ_MEN_8050 ) {
					if( nChar == '2' || nChar == VK_NUMPAD2 ) {
						fnc = CD_SP_EXEMPT_50;
					}
					if( nChar == '1' || nChar == VK_NUMPAD1 ) {
						fnc = CD_SP_EXEMPT;
					}
				}
				else if( m_Koujo80_50 == SYZ_MEN_50 ) {
					if( nChar == '2' || nChar == VK_NUMPAD2 ) {
						fnc = CD_SP_EXEMPT_50;
					}
					if( nChar == '1' || nChar == VK_NUMPAD1 ) {
						fnc = CD_SP_EXEMPT;
					}
				}

				if( m_pZm->dbdata->ddate > SYZMEN4_YMD ) {
					fnc = CD_SP_EXEMPT_00;
				}
				else if( m_pZm->dbdata->ddate < SYZMEN1_YMD ) {
					fnc = CD_SP_NO_EXEMPT;
				}
			}
//MyTrace("MenCheckButtonJob = %08x\n", fnc);

			if( fnc != 0 ) {
				m_pSy->SyFnc(fnc);
				MenWariDBEdit(fnc == CD_SP_NO_EXEMPT ? FALSE : TRUE);
			}
		}
		else {
			// 課税事業者からの仕入
			m_pSy->SyFnc(CD_SP_NO_EXEMPT);
			MenWariDBEdit(FALSE);
		}
		set_nowpn(SY_MENZEI_PN);
		SetSkbnSelNo(NULL);
	}
	else {
		sydata_set(NULL);
	}
}



//免税事業者から仕入 控除割合
void CSkbnIn::MenWariDBEdit(BOOL bEnable)
{
	DBGFUNCTION;
	GetDlgItem(IDC_STATIC20)->EnableWindow(bEnable);
	m_MenWari.EnableWindow(bEnable);

	struct _InpCtl* p_inp;
	p_inp = getINP_CTL(SY_MENWARI_PN);
	p_inp->INP_ena = bEnable;

	if( bEnable ) {
		MenWariSetDBedtData();
	}
	else {
		//エディット初期化
		VARIANT var;
		var.pbVal = NULL;
		DBdata_set(this, IDC_ICSDBEDT_MEN, &var, ICSDBEDT_TYPE_STRING, 0);
		DBSetDispString(this, IDC_ICSDBEDT_MEN, NULL);
	}
}

//コンボボックス 割合セット
void CSkbnIn::MenWariSetDBedtData()
{
	int sel = -1;

	int tmp = (m_pZm->dbdata->dsign[6] & 0xc0);

	if( tmp == 0 ) {
		//80%
		sel = 0;
	}
	tmp >>= 6;

	if( tmp == 1 ) {
		sel = 1;	//50%
	}
	else if( tmp == 2 ) {
		//0%
		sel = -1;
	}
	//struct _SY_MSG_TBL* psmt;
	//psmt = m_smpk.SY_MSG_EXEMPT;

	VARIANT var;
	char tmpbf[32];
	sprintf_s(tmpbf, sizeof tmpbf, "%d", sel);
	var.pbVal = (BYTE*)tmpbf;
	DBdata_set(this, IDC_ICSDBEDT_MEN, &var, ICSDBEDT_TYPE_STRING, 0);
	DBSetDispString(this, IDC_ICSDBEDT_MEN, GetMenzeiWariaiString(sel));
}


//=============================================================================
// 控除割合ターミネーション
//=============================================================================
int CSkbnIn::OnTerminationDBEdtMen(long nChar, long inplen, long kst)
{
	DBGFUNCTION;
	int i, pn = 0;
	char pbuf[50];
	VARIANT var;

	if( m_THIS_DIALOG_OK == FALSE )
		return 1;

	if( ::GetKeyState(VK_SHIFT) < 0 ) {
		if( nChar == VK_TAB ) {
			nChar = VK_LEFT;
		}
	}
	switch( nChar )
	{
	case NULL:
	case VK_RIGHT:
	case VK_RETURN:
	case VK_TAB:
	case VK_DOWN:
		//	case VK_F3:
	case VK_F2:
	case VK_UP:
	case VK_LEFT:
		if( inplen ) {
			if( m_MODEFY_OK ) {
				pbuf[0] = '\0';
				DBdata_get(this, getINP_CTL(SY_MENWARI_PN)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0);
				strcpy_s(pbuf, sizeof pbuf, (char*)var.pbVal);

				for( i = 0; pbuf[i]; ++i )
				{
					if( isdigit(pbuf[i]) )
						continue;
					else
					{
						pbuf[0] = '\0';
						break;
					}
				}
				SetSkbnSelNo(atoi(pbuf));
			}
			else {
				sydata_set(NULL);
			}
		}
	}

	switch( nChar ) {
	case VK_RIGHT:
	case VK_DOWN:
		to_parent(m_pParent, nChar);
		return 0;
		break;

	case VK_RETURN:
	case VK_TAB:
		if( (pn = get_nextpn(get_nowpn(), nChar)) != -1 ) {
			set_focus(pn);
			return 0;
		}
		else {
			to_parent(m_pParent, nChar);
			return 0;
		}
		return 0;
	case VK_UP:
	case VK_LEFT:
	case VK_F2:
		if( (pn = get_prevpn(get_nowpn())) != -1 ) {
			set_focus(pn);
			return 0;
		}
		else {
			to_parent(m_pParent, nChar);
			return 0;
		}
		break;
	default:
		return 1;
		break;
	}

	return 0;
}


//登録番号入力チェック
BOOL CSkbnIn::CheckInvnoInput()
{
	if( get_nowpn() != SY_INV_PN )
		return TRUE;

	VARIANT var;
	char pbuf[128];
	DBdata_get(this, getINP_CTL(SY_INV_PN)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0);
	sprintf_s(pbuf, sizeof(pbuf), _T("%s"), (char*)var.pbVal);

	CString checkInvno;
	checkInvno.Format(_T("T%s"), pbuf);

	if( syzInvoice.CheckInvoiceNumber(checkInvno) == -1 ) {
		m_bMsgDsp = TRUE;

	//	m_pZm->dbdata->invno.Empty();
	//	var.pbVal = (BYTE*)0;
	//	DBdata_set(this, getINP_CTL(SY_INV_PN)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0);
		m_bIgnoreFocus = TRUE;
		ICSMessageBox(syzInvoice.GetErrorMessage(), MB_ICONSTOP);

		m_bMsgDsp = FALSE;
		m_bIgnoreFocus = FALSE;

		set_focus(SY_INV_PN);
		return FALSE;
	}
	return TRUE;
}


void CSkbnIn::OnSetfocusCheckMenzei()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	set_nowpn(SY_MENZEI_PN);
	struct _SY_MSG_TBL* psmt;
	psmt = m_smpk.SY_MSG_EXEMPT;
	int pn = SY_MENWARI_PN;

	invmng.InvNoSearch(NULL);

	kbn_select_make(psmt, pn);
}




HBRUSH CSkbnIn::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = ICSDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: ここで DC の属性を変更してください。
#ifdef CLOSE
	if( pWnd->GetSafeHwnd() == m_MenzeiCheck.GetSafeHwnd() ) {
		HWND hwnd = ::GetFocus();
		if( hwnd == m_MenzeiCheck.m_hWnd ) {
			return (HBRUSH)m_brBk;
		}
		else {
			return (HBRUSH)m_brDefBk;
		}
	}
#endif
	// TODO: 既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}



void CSkbnIn::ImeEndCompositionIcsdbedtctrl(UINT ID, short nChar, LPCTSTR string, LPCTSTR ystring)
{
	if( ID == IDC_ICSDBEDT_INV ) {
		char	tmp[128] = {0};

//MyTrace("@%s ----nChar(%d), string(%s)\n", __FUNCTION__, nChar, string );
		if( !lastBrnInpStr(string, tmp, sizeof tmp) ) {

			invmng.CancelIME();
			return;
		}
//MyTrace("@%s ----(2)\n", __FUNCTION__);

		if( CheckDataDBeditInv(nChar, tmp) <= 0 )
			return;

	//	invmng.CancelIME();

//MyTrace("@%s ----(3)\n", __FUNCTION__);

		OnTerminationDBEdtInv(nChar, 1, 0);
	}
}


long CSkbnIn::CheckDataDBeditInv(long nChar, LPCTSTR string)
{
	if( m_MODEFY_OK )
	{
		VARIANT var;
		int no = atoi(string);
		int idc = IDC_ICSDBEDT_INV;
		//選択欄より
		if( (no > 0) && (no <= invmng.m_invPGCnt) ) {
			func_invketachk(idc, this, string);
			//var.pbVal = (BYTE*)string;
			//DBdata_set(this, idc, &var, ICSDBEDT_TYPE_STRING, 0);
		}
		else {
			//登録番号チェック は Terminationで
			CICSDBEDT* pDedt;
			pDedt = (CICSDBEDT*)(GetDlgItem(idc));
			int pos = pDedt->GetCaretPosition();
			var.pbVal = (BYTE*)string;
			// 選択部分
			long sel;
			sel = pDedt->GetSelect();
			TRACE("pos(%d), sel = %08x\n", pos, sel);

			if( strlen(string) == 13 || pos == 13 ) {
				DBdata_set(this, idc, &var, ICSDBEDT_TYPE_STRING, 0);
			}
			else {
				pDedt->InsertData(&var, ICSDBEDT_TYPE_STRING, 0, pos);
			}
		}
	}
	else {
		return 0;
	}

	return nChar;
}


//インボイス番号より
LRESULT CSkbnIn::OnMyMessage(WPARAM wp, LPARAM lp)
{
	if( wp == INV_KBNMAKE ) {
		kbn_select_make(NULL, SY_INV_PN);
	}
	else if( wp == INV_ONTERM ) {
		WORD vkcode = LOWORD(lp);
		OnTerminationDBEdtInv(vkcode, 0, 0);
	}
	else if( wp == INV_ERRMSG ) {
		if( IsWindowVisible() ) {
			ICSMessageBox(m_ErrMsg, MB_ICONSTOP, 0,0,this);
			m_DBedtInvDisp.ShowWindow(SW_HIDE);
			set_focus(SY_INV_PN);
		}
		m_bIgnoreFocus = FALSE;
		m_ErrMsg.Empty();
	}

	return 1;
}


/////////////////////////////////////////////////////////////////////////////////////////////
// インボイス登録番号管理クラス

CInvnoMng::CInvnoMng()
{
	m_pDefInv = NULL;
	m_pInvAry = NULL;
	//	CArray<INVOICE_INFO> m_AryInv;

	m_invPG = 0;		//現在ページ数
	m_invPGCnt = 0;	//ページ当たりデータ数

	m_nID = 0;
	m_hWnd = NULL;
	m_pDlg = NULL;
}

CInvnoMng::~CInvnoMng()
{
}


void CInvnoMng::SetInvArray(CArray<INVOICE_INFO> *ary)
{
	m_pDefInv = ary;
	m_pInvAry = ary;
}

int  CInvnoMng::FukuKanaSearch(MSG* pMsg)
{
	HIMC	hImc;
	char	buf[128] = { 0 };
	char	yomi[128] = { 0 };
	char	yomi_org[128] = { 0 };

	if( pMsg->wParam == VK_RETURN || pMsg->wParam == VK_TAB ) {
		return 0;
	}

	if( m_hWnd == NULL )
		return 0;

	if( pMsg->wParam == VK_NEXT || pMsg->wParam == VK_PRIOR ) {
		return 0;
	}

	hImc = ImmGetContext(m_hWnd);

	if( pMsg->wParam == VK_END ) {
		ImmNotifyIME(hImc, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
		ImmReleaseContext(m_hWnd, hImc);
		return 0;
	}

	ImmGetCompositionString(hImc, GCS_COMPSTR, buf, sizeof buf);
	ImmGetCompositionString(hImc, GCS_COMPREADSTR, yomi_org, sizeof yomi_org);

	//読みにアルファベットがあるか？
	CString stmp(yomi);
	kana_check(yomi, yomi_org);

	//MyTrace("●CDBbaseView::FukuKanaSearch buf, = %s, yomi_org = %s, yomi = %s, pMsg->wParam = %d●\n", buf, yomi_org, yomi, pMsg->wParam );

	int orglen = strlen(yomi_org);
	int searchsgn = 1;
	if( orglen > 0 ) {
		int len = strlen(yomi);
		if( len < orglen ) {
			searchsgn = 0;
		}
	}

	int rval = 0;

	if( buf[0] && yomi[0] && searchsgn ) {
		InvNoSearch(yomi);
		m_pDlg->SendMessage(WM_MYMESSAGE, INV_KBNMAKE);
	//	kbn_select_make(NULL, SY_INV_PN);
	}
	else if( (buf[0] == '\0') && (yomi[0] == '\0') ) {
		if( pMsg->wParam == VK_BACK || pMsg->wParam == VK_DELETE ) {
			//初期表示
			InvNoSearch(yomi);
			m_pDlg->SendMessage(WM_MYMESSAGE, INV_KBNMAKE);
		//	kbn_select_make(NULL, SY_INV_PN);
		}
		if( pMsg->wParam == VK_BACK ) {
			rval = 1;
		}
	}
	ImmReleaseContext(m_hWnd, hImc);

	return rval;
}

int  CInvnoMng::KanaCheckVK_BACK(MSG* pMsg)
{
	HIMC	hImc;
	char	buf[128] = { 0 };
	char	yomi[128] = { 0 };
	char	yomi_org[128] = { 0 };

	if( m_hWnd == NULL )
		return 0;

	//
	UINT vkcode = 0;
	// IMEONの時は、どのキーでも VK_PROCESSKEY になる
	if( pMsg->wParam == VK_PROCESSKEY ) {
		//		vkcode = MapVirtualKey( HIWORD(pMsg->lParam), MAPVK_VSC_TO_VK);
		//		vkcode = MapVirtualKey(pMsg->wParam, MAPVK_VSC_TO_VK);
		vkcode = ImmGetVirtualKey(m_hWnd);
	}
	else {
		vkcode = pMsg->wParam;
	}
	TRACE("KanaCheckVK_BACK pMsg->wParam = %08x, lParam = %08x, vk_code = %04x\n", pMsg->wParam, pMsg->lParam, vkcode);

	//IMEONの時は、
	if( pMsg->wParam != VK_BACK && vkcode != VK_F2 /*&& vkcode != VK_HOME*/ ) {
		return 0;
	}

	hImc = ImmGetContext(m_hWnd);

	if( vkcode == VK_F2 /*|| vkcode == VK_HOME*/ ) {
		ImmNotifyIME(hImc, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
		ImmReleaseContext(m_hWnd, hImc);
#ifdef CLOSE
		//VK_HOME 時は検索クリア
		if( vkcode == VK_HOME ) {
			LPARAM lp = MAKELPARAM(vkcode, 0);
			m_pDlg->SendMessage(WM_MYMESSAGE, INV_ONTERM,  lp );
		//	OnTerminationDBEdtInv(vkcode, 0, 0);
		}
#endif
		return 0;
	}
	ImmGetCompositionString(hImc, GCS_COMPSTR, buf, sizeof buf);
	ImmGetCompositionString(hImc, GCS_COMPREADSTR, yomi_org, sizeof yomi_org);

	int rval = 0;

	if( (buf[0] == '\0') && (yomi[0] == '\0') ) {
		if( pMsg->wParam == VK_BACK ) {
			rval = 1;
		}
	}
	ImmReleaseContext(m_hWnd, hImc);

	return rval;
}


int  CInvnoMng::InvNoSearch(LPSTR kana)
{
	char invkana[16] = { 0 };
	if( kana != NULL ) {
		sprintf_s(invkana, sizeof invkana, "%.8s", kana);
	}

	if( invkana[0] == '\0' ) {
		m_pInvAry = m_pDefInv;
		m_invPG = 0;
		m_AryInv.RemoveAll();

		if( m_pInvAry != NULL ) {
			return m_pInvAry->GetCount();
		}
		else {
			return 0;
		}
	}
	else {
		CArray<INVOICE_INFO> tmpAry;
		tmpAry.Copy(m_AryInv);
		m_AryInv.RemoveAll();

		for( int n = 0; n < m_pDefInv->GetCount(); n++ ) {
			INVOICE_INFO info = m_pDefInv->GetAt(n);

			CString reckana;
			reckana.Format("%.8s", info.inv_tkKana);
			reckana.TrimRight();

			if( reckana.Find(invkana) == 0 ) {
				m_AryInv.Add(info);
			}
		}
		//検索結果がゼロ
		if( m_AryInv.GetCount() == 0 ) {
			if( tmpAry.GetCount() == 0 ) {
				m_AryInv.Copy(*m_pDefInv);
			}
			else {
				m_AryInv.Copy(tmpAry);
			}
		}
		m_pInvAry = &m_AryInv;
		m_invPG = 0;

		return  m_pInvAry->GetCount();
	}
	return 0;
}


// 登録番号ページ表示
//	sgn = 0		先頭から表示
//  sgn = -1	前頁
//  sgn = 1		次頁
//
void CInvnoMng::InvDispPage(int sgn)
{
	if( sgn == 0 ) {
		m_invPG = 0;
		m_pDlg->SendMessage(WM_MYMESSAGE, INV_KBNMAKE);
	}
	else if( sgn < 0 ) {
		if( m_invPG > 0 ) {
			m_invPG--;
			m_pDlg->SendMessage(WM_MYMESSAGE, INV_KBNMAKE);
		}
	}
	else {
		int invCount = (int)m_pInvAry->GetCount();
		int dispPGmax = (m_invPG * m_invPGCnt) + m_invPGCnt;
		if( invCount > dispPGmax ) {
			m_invPG++;
			m_pDlg->SendMessage(WM_MYMESSAGE, INV_KBNMAKE);
		}
	}
}



int  CInvnoMng::CancelIME()
{
	HIMC	hImc;
MyTrace("@%s TOP====\n", __FUNCTION__);

	if( m_hWnd == NULL )
		return 0;


	//IMEONの時は、CANCEL
	hImc = ImmGetContext(m_hWnd);
	ImmNotifyIME(hImc, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
	ImmReleaseContext(m_hWnd, hImc);

	MyTrace("@%s ----END \n", __FUNCTION__);

	return 0;
}
