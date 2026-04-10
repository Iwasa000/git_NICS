// DnpBaseDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "resource.h"

#include "DenpRec.h"
#include "DnpBaseDlg.h"

#include "DInputSub.h"
#include "DnpBaseClass.h"

#include "icsdbedt.h"
#include "imagestatic.h"
#include "husenstatic.h"

#include "DBInpSubFunc.h"


// 借方・貸方合計 buff.
//extern struct _DenpData			DEBT_TOTAL, CRED_TOTAL;
//extern struct _DNPLINE_CND		DEBT_TOTAL_CND, CRED_TOTAL_CND;

extern BOOL IsDebtPosition( int pn );
extern BOOL IsCredPosition( int pn );


extern
int get_strcnt(char* str, int byte);


// 消費税文字列 改行を入れる
void check_syzstring( CString& str )
{
	int l = str.GetLength();

	if( l > 8 ) {
		CString tmp, wk1, wk2;
		tmp = str;

		int n = get_strcnt( (char*)(LPCTSTR)tmp, 8 );
		wk1 = tmp.Left( n );
		wk2 = tmp.Right( l-n );

		str = wk1 +"\n"+ wk2;
	}
}


/*************************************************************
	伝票ダイアログ CIcsdiag8ctrl の配置仕様
	
	------------------------------------------------
	タイトル・伝票番号・日付等  = IDC_ICSDIAG8CTRL1
	------------------------------------------------

	伝票実データ部 データ×７行 = IDC_ICSDIAG8CTRL2

	合計行
	------------------------------------------------

***************************************************************/


// CDnpBaseDlg ダイアログ

IMPLEMENT_DYNAMIC(CDnpBaseDlg, ICSDialog)

int CDnpBaseDlg::m_BmonLNG = 0;
int CDnpBaseDlg::m_KojiLNG = 0;
int CDnpBaseDlg::m_EdaLNG = 0;
int CDnpBaseDlg::m_SyoNumLNG = 0;

CString CDnpBaseDlg::m_ScanTitle;


// 部門・工事番号の入力レングスセット( 対象マスターでない場合は 0 )
void CDnpBaseDlg::SetMasterPar( int bmlng, int kjlng, int edaln, int syonum)
{
	m_BmonLNG = bmlng;
	m_KojiLNG = kjlng;
	m_EdaLNG = edaln;
	m_SyoNumLNG = syonum;
}

// 修正タイトル変更
void CDnpBaseDlg::SetScanTitle( BOOL bConfirmMst )
{
	if( bConfirmMst ) {
		m_ScanTitle = _T( "確定 伝票" );
	}
	else {
		m_ScanTitle = _T( "伝票 修正" );
	}	
}


CDnpBaseDlg::CDnpBaseDlg(UINT dlgID, CWnd* pParent /*=NULL*/)
	: ICSDialog(dlgID, pParent)
{
	m_dlgID = dlgID;
	m_pDiag = NULL;

	m_dgno = -1;
	m_index = -1;

	m_DclmCount = 0;

	m_pHDpos = NULL;
	m_pDTpos = NULL;
	m_pKeiPos = NULL;

	m_TkyID		= IDC_ICSDBEDT8CTRL1;
	m_TkyLine	= -1;

	m_bHjiyuEna = TRUE;
	m_pScBar = NULL;

	m_pBaseCls = NULL;
	
//	m_hAccelTable = ::LoadAccelerators( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
}

CDnpBaseDlg::~CDnpBaseDlg()
{
}

void CDnpBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDnpBaseDlg, ICSDialog)
	ON_WM_SIZE()
	ON_MESSAGE( WM_RETROFOCUS, OnRetroFocus )
	ON_MESSAGE( WM_RETROFOCUS2, OnRetroFocus2 )

	ON_MESSAGE( MESSAGE_IMGST_KEY, OnImageStaticKey)
	ON_MESSAGE( MESSAGE_IMGST_FOCUS, OnImageStaticFocus)

	ON_MESSAGE(WM_DBDINPMSG, OnDBDinpMsg)
END_MESSAGE_MAP()


// CDnpBaseDlg メッセージ ハンドラ

BOOL CDnpBaseDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	((CICSDBEDT*)GetDlgItem(m_TkyID))->ImeEndMode(TRUE);
	((CICSDBEDT*)GetDlgItem(m_TkyID))->EnableDelete(0);
	((CICSDBEDT*)GetDlgItem(m_TkyID))->EnableHome(1);
	((CICSDBEDT*)GetDlgItem(m_TkyID))->SetBackColor( RGB(0xE0, 0xFF, 0xFF) );

	((CICSDBEDT*)GetDlgItem(IDC_ICSDBEDT_KASITKY))->ImeEndMode(TRUE);
	((CICSDBEDT*)GetDlgItem(IDC_ICSDBEDT_KASITKY))->EnableDelete(0);
	((CICSDBEDT*)GetDlgItem(IDC_ICSDBEDT_KASITKY))->EnableHome(1);
	((CICSDBEDT*)GetDlgItem(IDC_ICSDBEDT_KASITKY))->SetBackColor( RGB(0xE0, 0xFF, 0xFF) );

	GetDlgItem(IDC_ICSDBEDT_KASITKY)->ModifyStyle( 0, WS_CLIPSIBLINGS);
	GetDlgItem(IDC_SCROLLBAR1)->ModifyStyle( 0, WS_CLIPSIBLINGS);

	((CICSDBEDT*)GetDlgItem(IDC_ICSDBEDT_KOJI))->ImeEndMode(TRUE);
	((CICSDBEDT*)GetDlgItem(IDC_ICSDBEDT_KOJI))->EnableDelete(0);

	((CICSDBEDT*)GetDlgItem(IDC_ICSDBEDT_TGNUM))->ImeEndMode(TRUE);
	((CICSDBEDT*)GetDlgItem(IDC_ICSDBEDT_TGNUM))->EnableDelete(0);

	((CICSDBEDT*)GetDlgItem(IDC_ICSDBEDT_BMON))->ImeEndMode(TRUE);
	((CICSDBEDT*)GetDlgItem(IDC_ICSDBEDT_BMON))->EnableDelete(0);

	((CICSDBEDT*)GetDlgItem(IDC_ICSDBEDT_SYONUM))->ImeEndMode(FALSE);
	((CICSDBEDT*)GetDlgItem(IDC_ICSDBEDT_SYONUM))->EnableDelete(0);

	((CICSDBEDT*)GetDlgItem(IDC_ICSDBEDT_KMINP))->ImeEndMode(TRUE);
	((CICSDBEDT*)GetDlgItem(IDC_ICSDBEDT_KMINP))->EnableDelete(0);
	((CICSDBEDT*)GetDlgItem(IDC_ICSDBEDT_KMINP))->EnableHome(1);
	((CICSDBEDT*)GetDlgItem(IDC_ICSDBEDT_KMINP))->SetKanaTerm(TRUE, m_pBaseCls->KanaSearchKeta(), ICSDBEDT_KTYPE_KANA);

	//派生クラス先で行う
//	ICSDialog::OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


//--------------------------------------------
//	ダイアログ初期化
//	DNP_POS* dpos	伝票項目詳細データ構造体
//
//--------------------------------------------
void CDnpBaseDlg::InitDnpBase( DNP_POS* head, DNP_POS* data, DNP_POS* kei )
{

	m_pHDpos	= head;
	m_pDTpos	= data;
	m_pKeiPos	= kei;

	// 一仕訳あたりのDiag項目数
	int cnt = 0;
	for( int i = 0; (m_pDTpos + i)->dpno != -1; i++ ) {
		if( (m_pDTpos + i)->dpno != -1 )
			cnt++;
	}
	m_DclmCount = cnt;
}


//---------------------------------------------------
//	ダイアグラム変数を初期化
//	CICSDiag* pDiag	ダイアグラムコントロール変数
//
//---------------------------------------------------
void CDnpBaseDlg::InitBaseDiag( CIcsdiag8ctrl* pDiag, int diagcnt )
{
	m_pDiag = pDiag;

	for( int i = 0; i < diagcnt; i++ ) {
		(m_pDiag+i)->CursorEndMode(1);
		(m_pDiag+i)->EnableDelete(0);
	}

	DiagInit();
}

//----------------------------------------------------
//	コントロールポジションセット
//
//----------------------------------------------------
void CDnpBaseDlg::ControlPos()
{
	CRect rect;

	for( int i = 0; i < m_LineCount; i++ ) {
		// イメージ表示
		GetRect( i+1, FD_TKY_PN, rect );
		ScreenToClient( rect );
		rect.DeflateRect(1,1,1,0);
		GetDlgItem( IDC_IMAGE1+i )->MoveWindow( rect );
		// 付箋表示
		GetRect( i+1, FD_HUSEN_PN, rect );
		ScreenToClient( rect );
		rect.DeflateRect(1,1,2,2);
		GetDlgItem( IDC_HUSEN1+i )->MoveWindow( rect );
	}
}


//----------------------------------------------------
//	イメージ等 コントロールイニシャライズ
//
//----------------------------------------------------
void CDnpBaseDlg::control_iniz()
{
	ImageStatic*	pImg;
	CHusenStatic*	pHusen;
	int ln;

	GetDlgItem( m_TkyID )->SetWindowPos( NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER );

	for( ln = 1; ln <= m_LineCount; ln++ ) {
		ImagePaintSw( ln, 0 );
		ImageBackColor( ln, FALSE );
		ImageBackTrans( ln, FALSE, RGB_WHITE );
#ifdef OLD_FUNC
		DeleteLine( ln, FALSE );
#endif
	}

	for( ln = 1; ln <= m_LineCount; ln++ ) {
		pImg = (ImageStatic*)GetDlgItem( IDC_IMAGE1 + (ln-1) );
		pImg->ClearImage();
		OffImage( ln, FALSE );
	}

	OffImage( -1, TRUE );	// 貸方イメージＯＦＦ

	flgKasiTeki		=	FALSE;
	flgKasiImage	=	FALSE;

	KasiTekiyoDisplay( FALSE );

	// 付箋スタティック
	for( ln = 1; ln <= m_LineCount; ln++ ) {
		pHusen = (CHusenStatic*)GetDlgItem( IDC_HUSEN1 + (ln-1) );
		pHusen->ModifyStyle( 0, WS_CLIPSIBLINGS);

		pHusen->UpdateCaption("", NULL );
		pHusen->DefaultColor( FALSE );
		pHusen->SetBackColor( RGB_WHITE );

	}
}


//----------------------------------------------------
//	ダイアグラムの現在位置より伝票行・項目取得
//	int& ln		行( 1 から始まる)
//	int& pn		ポジションサイン
//
//----------------------------------------------------
void CDnpBaseDlg::GetDnpNowPos( int& ln, int& pn )
{
	DNP_POS* pos;
	pos = m_pHDpos;

	int chk = 0;
	//ヘッド部
	for( ; ; pos++ ) {
		if( pos->dpno < 0 ) {
			ln = -1;
			pn = -1;
			break;
		}

		if( pos->dpno == m_dgno && pos->dpindex == m_index ) {
			ln = -1;
			pn = pos->dp_pn;
			chk++;
			break;
		}
	}

	if( chk )	return;
	//借方・貸方 合計
	pos = m_pKeiPos;

	for( ; ; pos++ ) {
		if( pos->dpno < 0 ) {
			ln = -1;
			pn = -1;
			break;
		}
		if( pos->dpno == m_dgno && pos->dpindex == m_index ) {
			ln = -1;
			pn = pos->dp_pn;
			chk++;
			break;
		}
	}

	if( chk )	return;
	//仕訳データ
	pos = m_pDTpos;

	int dataidx, line;
	dataidx = (m_index % m_DclmCount);
	line = (m_index / m_DclmCount) + 1;

	for( ; ; pos++ ) {
		if( pos->dpno < 0 ) {
			ln = -1;
			pn = -1;
			break;
		}
		if( pos->dpno == m_dgno && pos->dpindex == dataidx ) {
			ln = line;
			pn = pos->dp_pn;
			chk++;
			break;
		}
	}
#ifdef _DEBUG
	if( m_dgno != -1 && m_index != -1 ) {
ASSERT( chk );
	}
#endif

	if( chk )	return;
}


//----------------------------------------------------
//	指定ポジションより DNP_POSデータを取得
//	int ln		行
//	int pn		ポジションサイン
//
//----------------------------------------------------
DNP_POS* CDnpBaseDlg::GetDNP_POS( int ln, int pn )
{
	DNP_POS* pos, *pRpos;
	int i, sgn = 0;;

	pRpos = NULL;

	if( ln == -1 || (pn >= DP_DENP_PN && pn <= DP_KOJI_PN) ) {
		// ヘッド部
		pos = m_pHDpos;

		for( i = 0; (pos+i)->dpno != -1 ; i++ ) {
			if( pn == (pos+i)->dp_pn ) {
				m_PosBuf = *(pos+i);
				pRpos = &m_PosBuf;
				break;
			}
		}
	}
	else {
		// 仕訳データ部
		pos = m_pDTpos;

		for( i = 0; (pos+i)->dpno != -1 ; i++ ) {
			if( pn == (pos+i)->dp_pn ) {
				m_PosBuf = *(pos+i);
				int idx;

				idx = m_PosBuf.dpindex + (m_DclmCount * (ln-1));
				m_PosBuf.dpindex = idx;

				if( m_PosBuf.dpindex > (m_DclmCount * m_LineCount) ) { 
					int scrn_ln = (ln - m_pBaseCls->GetDnpTopIndex());
					if( scrn_ln < 0 )
						return NULL;

					idx = (pos+i)->dpindex + (m_DclmCount * (scrn_ln-1));
					m_PosBuf.dpindex = idx;
				}

				pRpos = &m_PosBuf;
				break;
			}
		}
	}

	return pRpos;
}

//----------------------------------------------------
//	決修、四半期などの表示
//
//----------------------------------------------------
void CDnpBaseDlg::kesyu_dsp( char* str )
{
	DNP_POS* pos;
	pos = m_pHDpos;

	int chk = 0;
	DIAGRAM_DATA dd;

	//ヘッド部
	for( ; pos->dpno != -1; pos++ ) {
		if( pos->dp_pn == DP_KESYU_PN ) {
			if( str[0] ) {
				dd.data_disp = str;
				dd.data_imgdata = NULL;
				(m_pDiag + pos->dpno)->SetData( pos->dpindex, (LPUNKNOWN)&dd );
			}
			else {
				(m_pDiag + pos->dpno)->DataClear( pos->dpindex, TRUE );
			}
			break;
		}
	}
}

//----------------------------------------------------
//	伝票へデータのセット
//		基本的にはデータはクリアされている
//
//----------------------------------------------------
void CDnpBaseDlg::inpdata_dsp()
{
	int i, ln, n, idx;
	DNP_POS* p;
	struct _DenpData* pdata;
	struct _DNPLINE_CND* pcnd;
	SETMEISYO	stm;
	struct _SET_KMKETC sk;

	// ヘッド部
	p = m_pHDpos;
	for( i = 0; (p+i)->dpno != -1; i++ ) {
		// 入力可能項目のみ
		if( ! (p+i)->dp_enable )
			continue;

		pdata = m_pBaseCls->getDATA( (p+i)->dp_pn );
		pcnd = m_pBaseCls->getCND( (p+i)->dp_pn );

		switch( (p+i)->dp_pn ) {
			case DP_DENP_PN:
				if( pcnd->INP_sg )
					_SetDnpyoNo( (p+i)->dpno, (p+i)->dpindex, pdata->DP_DENP );
				else
					DiagClear( (p+i)->dpno, (p+i)->dpindex );
				break;
			case DP_MM_PN:
				if( pcnd->INP_sg )
					_SetMM( (p+i)->dpno, (p+i)->dpindex, pdata->DP_BCDDAY[0] );
				else
					DiagClear( (p+i)->dpno, (p+i)->dpindex );
				break;
			case DP_DD_PN:
				if( pcnd->INP_sg )
					_SetDD( (p+i)->dpno, (p+i)->dpindex, pdata->DP_BCDDAY[0] );
				else
					DiagClear( (p+i)->dpno, (p+i)->dpindex );
				break;
			case DP_SYONUM_PN:
				if( pcnd->INP_sg ) {
					_SetTgNumber((p + i)->dpno, (p + i)->dpindex, (char*)pdata->DP_SNUM);
				}
				else {
					DiagClear((p + i)->dpno, (p + i)->dpindex);
				}
				break;

			case DP_BMN_PN:
				if( pcnd->INP_sg ) {
					sk.etc		= pcnd->ETC_msg;
					sk.etc_col	= pcnd->ETC_col;

					_SetBmon( (p+i)->dpno, (p+i)->dpindex, pdata->DP_BMN, &sk );
				}
				else
					DiagClear( (p+i)->dpno, (p+i)->dpindex );
				break;
			case DP_KOJI_PN:
				if( pcnd->INP_sg ) {
					sk.etc		= pcnd->ETC_msg;
					sk.etc_col	= pcnd->ETC_col;

					_SetKoji( (p+i)->dpno, (p+i)->dpindex, CString( pdata->DP_KOJI ), &sk );
				}
				else
					DiagClear( (p+i)->dpno, (p+i)->dpindex );
				break;
			default:
				ASSERT( FALSE );
				break;
		}
	}

	// 仕訳データ部
	p = m_pDTpos;

	for( ln = 1; ln <= m_LineCount; ln++ ) {
		n = ((ln-1) * m_DclmCount);

		for( i = 0; (p+i)->dpno != -1; i++ ) {
			// 行ごとの インデックス
			idx = ((p+i)->dpindex + n);
			// 入力可能項目のみ
			if( ! (p+i)->dp_enable ) {
				DiagClear( (p+i)->dpno, idx );
				continue;
			}

			pdata = m_pBaseCls->_getDATA( ln, (p+i)->dp_pn );
			pcnd = m_pBaseCls->_getCND( ln, (p+i)->dp_pn );

			switch( (p+i)->dp_pn ) {
			case FD_DBVAL_PN:
			case FD_CRVAL_PN:
				if( pcnd->INP_sg ) {
					_SetVal( (p+i)->dpno, idx, (char*)pdata->DP_VAL );
				}
				else	DiagClear( (p+i)->dpno, idx );
				break;

			case FD_DBZEI_PN:
			case FD_CRZEI_PN:
				if( pcnd->INP_sg ) {
					_SetVal( (p+i)->dpno, idx, (char*)pdata->DP_VAL );
				}
				else	DiagClear( (p+i)->dpno, idx );
				break;

			case FD_DBMN_PN:
			case FD_CBMN_PN:
				if( pcnd->INP_sg ) {
					sk.etc		= pcnd->ETC_msg;
					sk.etc_col	= pcnd->ETC_col;

					_SetBmon( (p+i)->dpno, idx, pdata->DP_BMN, &sk );
				}
				else	DiagClear( (p+i)->dpno, idx );
				break;

			case FD_DKOJI_PN:
			case FD_CKOJI_PN:
				if( pcnd->INP_sg ) {
					sk.etc		= pcnd->ETC_msg;
					sk.etc_col	= pcnd->ETC_col;

					_SetKoji( (p+i)->dpno, idx, CString( pdata->DP_KOJI ), &sk );
				}
				else	DiagClear( (p+i)->dpno, idx );
				break;

			case FD_DEBT_PN:
			case FD_CRED_PN:
				if( pcnd->INP_sg ) {
					strcpy_s( stm.sm_name, sizeof stm.sm_name, pdata->DP_stKMK.kd_name );
					_SetKnam( (p+i)->dpno, idx, &stm );
				}
				else	DiagClear( (p+i)->dpno, idx );
				break;

			case FD_DSYOG_PN:
			case FD_CSYOG_PN:
				if( pcnd->INP_sg ) {
					_SetSyogBrn( (p+i)->dpno, idx, pdata->DP_cBRN );
				}
				else	DiagClear( (p+i)->dpno, idx );
				break;

			case FD_TKY_PN:
				if( pcnd->INP_sg ) {
					_SetTekiyo( (p+i)->dpno, idx, (char*)pdata->DP_TKY );
				}
				else	DiagClear( (p+i)->dpno, idx );

				break;
			case FD_DSKBN_PN:
			case FD_CSKBN_PN:
				if( pcnd->INP_sg ) {
					DIAGRAM_DATA dd;
					dd.data_imgdata = NULL;
					dd.data_edit = pdata->SYDATA.SKBNtxt;
					check_syzstring( dd.data_edit );
					(m_pDiag+ (p+i)->dpno )->SetData( idx, (LPUNKNOWN)&dd );

				}
				else	DiagClear( (p+i)->dpno, idx );
				break;

			case FD_TGNUM_PN:
				if( pcnd->INP_sg ) {
					_SetTgNumber( (p+i)->dpno, idx, (char*)pdata->DP_SNUM );
				}
				else {
					DiagClear( (p+i)->dpno, idx );
				}
				break;

			case FD_TGDATE_PN:
				if( pcnd->INP_sg ) {
					_SetTgDate( (p+i)->dpno, idx, pdata->DP_TGBCD );
				}
				else {
					DiagClear( (p+i)->dpno, idx );
				}
				break;

			case FD_HJIYU_PN:
				if( pcnd->INP_sg ) {
					CString txt;
				//	m_pBaseCls->get_hjiyustr( pdata->DP_BMN, &txt );
					m_pBaseCls->_get_hjiyustr( pdata->DP_BMN, pdata->NAIBCD, txt );
					_SetHjiyu( (p+i)->dpno, idx, &txt );
				}
				else {
					DiagClear( (p+i)->dpno, idx );
				}
				break;

			case FD_HUSEN_PN:
				SetHusen( ln, FD_HUSEN_PN, pdata->DP_HUSEN, (char*)pdata->DP_HCMNT );
				break;

			default:
				break;
			}
		}
	}

}

//-------------------------------------------------------------------------
//	伝票へ仕訳データ部のセット
//		int dataln ...	画面の伝票に表示するの 実際の伝票データの開始行
//						(1～)
//		int	setln		-1 でない時は、その指定行のみセットする
//
//-------------------------------------------------------------------------
void CDnpBaseDlg::dataline_disp( int dataln, int setln/*=-1*/ )
{
	DNP_POS* p;
	struct _DenpData* pdata;
	struct _DNPLINE_CND* pcnd;
	SETMEISYO	stm;
	struct _SET_KMKETC sk;
	ImageStatic*	pImg;
	char	sy_chg[4] = {0};

	// 仕訳データ部
	p = m_pDTpos;

	int n, i, ln, dt_ln, idx;

	OffImage( -1, TRUE );	// 貸方イメージＯＦＦ

	for( ln = 1; ln <= m_LineCount; ln++ ) {
		if( setln != -1 ) {
			if( ln != setln )	continue;
		}

		//イメージクリア
		pImg = (ImageStatic*)GetDlgItem( IDC_IMAGE1 + (ln-1) );
		pImg->ClearImage();
		OffImage( ln, FALSE );

		n = ((ln-1) * m_DclmCount);
		//実際のデータ行 位置
		dt_ln = dataln + (ln-1);

		::ZeroMemory(sy_chg, sizeof sy_chg);

		for( i = 0; (p+i)->dpno != -1; i++ ) {
			// 入力可能項目のみ
			if( ! (p+i)->dp_enable )
				continue;

			pdata = m_pBaseCls->_getDATA( dt_ln, (p+i)->dp_pn );
			pcnd = m_pBaseCls->_getCND( dt_ln, (p+i)->dp_pn );
			// 行ごとの インデックス
			idx = ((p+i)->dpindex + n);

			switch( (p+i)->dp_pn ) {
			case FD_DBVAL_PN:
			case FD_CRVAL_PN:
				if( pcnd->INP_sg ) {
					_SetVal( (p+i)->dpno, idx, (char*)pdata->DP_VAL );
				}
				else	DiagClear( (p+i)->dpno, idx );
				break;

			case FD_DBZEI_PN:
			case FD_CRZEI_PN:

				if( pcnd->INP_sg ) {
					COLORREF back = m_pBaseCls->GetConfirmBackColor(pcnd->EDT_sg);

					_SetVal( (p+i)->dpno, idx, (char*)pdata->DP_VAL, pcnd->INP_type, back);

					if ((p + i)->dp_pn == FD_DBZEI_PN)
						sy_chg[2] = pcnd->INP_type;
					else
						sy_chg[3] = pcnd->INP_type;
				}
				else	DiagClear( (p+i)->dpno, idx );
				break;

			case FD_DBMN_PN:
			case FD_CBMN_PN:
				if( pcnd->INP_sg ) {
					sk.etc		= pcnd->ETC_msg;
					sk.etc_col	= pcnd->ETC_col;

					_SetBmon( (p+i)->dpno, idx, pdata->DP_BMN, &sk );
				}
				else	DiagClear( (p+i)->dpno, idx );
				break;

			case FD_DKOJI_PN:
			case FD_CKOJI_PN:
				if( pcnd->INP_sg ) {
					sk.etc		= pcnd->ETC_msg;
					sk.etc_col	= pcnd->ETC_col;

					_SetKoji( (p+i)->dpno, idx, CString( pdata->DP_KOJI ), &sk );
				}
				else	DiagClear( (p+i)->dpno, idx );
				break;

			case FD_DEBT_PN:
			case FD_CRED_PN:
				if( pcnd->INP_sg ) {
					::ZeroMemory( &stm, sizeof stm );

					int code	= pdata->DP_BRN;
					char	edstr[32];
				//	m_pBaseCls->EdabanToStr( edstr, sizeof edstr, code );
					set_codestr( edstr, sizeof edstr, code, m_pBaseCls->m_pZm->zvol->edcol );

					CString tmp;
					DBKNREC* pKn;
					tmp	= pdata->DP_CdNum;
					if( (pKn = DB_PjisToKnrec( tmp, m_pBaseCls->m_pZm )) != NULL ) {
						CString nam;
						m_pBaseCls->KamokuString( pKn, nam );
						sprintf_s( stm.sm_name, sizeof stm.sm_name, "%.14s%s", (LPCTSTR)nam, edstr );
					}
					else {
						sprintf_s( stm.sm_name, sizeof stm.sm_name, "%.14s%s", pdata->DP_stKMK.kd_name, edstr );
					}
					sprintf_s( stm.sm_etc, sizeof stm.sm_etc, "%.40s", pcnd->ETC_msg );
					stm.sm_etccol = pcnd->ETC_col;
					_SetKnam( (p+i)->dpno, idx, &stm );
				}
				else	DiagClear( (p+i)->dpno, idx );
				break;

			case FD_DSYOG_PN:
			case FD_CSYOG_PN:
				if( pcnd->INP_sg ) {
					_SetSyogBrn( (p+i)->dpno, idx, pdata->DP_cBRN );
				}
				else	DiagClear( (p+i)->dpno, idx );
				break;

			case FD_TKY_PN:
				m_pBaseCls->ImgTekiyoSet( this, dt_ln, ln );

				if( pcnd->INP_sg ) {
					_SetTekiyo( (p+i)->dpno, idx, (char*)pdata->DP_TKY );
				}
				else	DiagClear( (p+i)->dpno, idx );

				break;
			case FD_DSKBN_PN:
			case FD_CSKBN_PN:
				if( pcnd->INP_sg ) {
					DIAGRAM_DATA dd;
					dd.data_imgdata = NULL;
					dd.data_edit = pdata->SYDATA.SKBNtxt;
					check_syzstring( dd.data_edit );
					(m_pDiag+ (p+i)->dpno )->SetData( idx, (LPUNKNOWN)&dd );

					// 色の変更
					DIAGRAM_ATTRIBUTE da;
					(m_pDiag + (p+i)->dpno )->GetAttribute( idx, (LPUNKNOWN)&da );

					if( (p+i)->dp_pn == FD_DSKBN_PN )
						sy_chg[0] = pdata->_udta._SYDATA.sy_chg;
					else
						sy_chg[1] = pdata->_udta._SYDATA.sy_chg;

					da.attr_bcolor	= pdata->_udta._SYDATA.sy_chg ? RGB_SYZ_MDFY : RGB_WHITE;

					(m_pDiag + (p+i)->dpno )->SetAttribute( idx, (LPUNKNOWN)&da, TRUE );
				}
				else	DiagClear( (p+i)->dpno, idx );
				break;

			case FD_TGNUM_PN:
				if( pcnd->INP_sg ) {
					_SetTgNumber( (p+i)->dpno, idx, (char*)pdata->DP_SNUM );
				}
				else {
					DiagClear( (p+i)->dpno, idx );
				}
				break;

			case FD_TGDATE_PN:
				if( pcnd->INP_sg ) {
					_SetTgDate( (p+i)->dpno, idx, pdata->DP_TGBCD );
				}
				else {
					DiagClear( (p+i)->dpno, idx );
				}
				break;

			case FD_HJIYU_PN:
				if( pcnd->INP_sg ) {
					CString txt;
				//	m_pBaseCls->get_hjiyustr( pdata->DP_BMN, &txt );
					m_pBaseCls->_get_hjiyustr( pdata->DP_BMN, pdata->NAIBCD, txt );
					_SetHjiyu( (p+i)->dpno, idx, &txt );
				}
				else {
					DiagClear( (p+i)->dpno, idx );
				}
				break;

			case FD_HUSEN_PN:
				SetHusen( ln, FD_HUSEN_PN, pdata->DP_HUSEN, (char*)pdata->DP_HCMNT);
				break;

			default:
				break;
			}
		}
		TransLine( dt_ln, ln, sy_chg );
		FuriSyogDataColor( dt_ln, ln );
	}
}


//----------------------------------------------------
//	ダイアグ Clear
//	int dgno ...... ダイアグラム番号(0から）
//	int index ..... インデックス
//----------------------------------------------------
void CDnpBaseDlg::DiagClear( int dgno, int index )
{
	(m_pDiag+dgno)->DataClear(index, TRUE);
}

//----------------------------------------------------
//	ダイアグ EditON
//	int diag_id ... ダイアグラムID
//	int index ..... インデックス
//
//----------------------------------------------------
void CDnpBaseDlg::DiagEditON( int diag_id, int index )
{
	if( ! m_pBaseCls->m_PRO_OK )		return;
	if( ! IsDenpView() )	return;

	int prev_no, prev_idx;
	prev_no		= m_dgno;
	prev_idx	= m_index;

	m_dgno	= diag_id - IDC_ICSDIAG8CTRL1;
	m_index	= index;

	int ln, pn;
	GetDnpNowPos( ln, pn );

	// 取消データライン
	if( m_pBaseCls->GetDelSgn( ln ) ) {
		int dpn = m_pBaseCls->get_delpn();
		if( pn != dpn ) {
			PostMessage( WM_RETROFOCUS, ln, dpn );
		}
		m_pBaseCls->ToViewDelFocusJob( ln, dpn );
		return;
	}
/*
	if( pn == FD_TGNUM_PN || pn == FD_TGDATE_PN ) {
		if( ! m_pBaseCls->IsBillInput( ln ) ) {
			WPARAM wp;
			wp = MAKEWPARAM( prev_no, 1 );
			PostMessage( WM_RETROFOCUS, wp, prev_idx );
			m_pBaseCls->ToViewDelFocusJob( ln, -1 );
			return;
		}
	}
	if( pn == FD_HJIYU_PN ) {
		if( ! m_pBaseCls->IsHjiyuInput( ln ) ) {
			WPARAM wp;
			wp = MAKEWPARAM( prev_no, 1 );
			PostMessage( WM_RETROFOCUS, wp, prev_idx );
			m_pBaseCls->ToViewDelFocusJob( ln, -1 );
			return;	
		}
	}
*/
	CRect rect;

	(m_pDiag+m_dgno)->GetRect( m_index, (LPUNKNOWN)&rect );
	
	m_pBaseCls->ToViewFocusJob( ln, pn, rect );
}


void CDnpBaseDlg::DiagEditOFF()
{
	if( ! m_pBaseCls->m_PRO_OK || ! IsDenpView() )	return;

	DIAGRAM_DATA		dd;
	UINT ID;
	ID = (IDC_ICSDIAG8CTRL1+m_dgno);
	diag_getdata( ID, m_index, &dd, CIcsdiag8ctrl );

	CDnpBaseDlg::DiagTermination( ID, m_index, 0, dd.data_check, (LPUNKNOWN)&dd);
}

// ダイアグラムターミネーション
void CDnpBaseDlg::DiagTermination(int diag_id, short index, short nChar, short length, LPUNKNOWN data)
{
//MyTrace( "DiagTermination [Top] %d,%d nChar[%d], %d\n", diag_id, index, nChar, length );

	if( ! m_pBaseCls->m_PRO_OK || ! IsDenpView() )	return;

	int ln, pn, dno, chk, bmn, st, n;
	long	inplen = 0;
	long	CharCode;
	CString string;
	GetDnpNowPos( ln, pn );
	DIAGRAM_DATA* dd;
	BYTE	mm = 0, day;
	dd = (DIAGRAM_DATA*)data;
	struct _KamokuData km, *pkd;
	pkd = NULL;

	CharCode = nChar;

	if( ln == -1 && pn == -1 )
		return;

	// shift tab チェック
	if( CharCode == VK_TAB && is_SHIFT() )
		CharCode = VK_F2;

	// 取消仕訳は フォーカスOFFを無視
	if( m_pBaseCls->GetDelSgn( ln ) ) {
		if( CharCode == 0 )
			return;
	}

#ifdef _DEBUG
	ASSERT( index == m_index );
	ASSERT( m_dgno == diag_id - IDC_ICSDIAG8CTRL1 );
#endif

	if( length )	inplen = 1;
	else {
		// 科目名称・工事は、IMEの確定をしないといけないので、
		// フォーカスOFFでデータの入力はない
		if( pn != FD_DEBT_PN && pn != FD_CRED_PN && pn != FD_DKOJI_PN && pn != FD_CKOJI_PN && pn != DP_KOJI_PN ) {
			if((CharCode == 0 && data != NULL && dd->data_check) )
				inplen = 1;
		}
	}
	if( ln > 0 ) {	//仕訳データ部
		switch( pn ) {
		case FD_DEBT_PN:
		case FD_CRED_PN:
#ifdef OLD_CLOSE	//DBEditで 入力 [09.16 /22]
			if( inplen > 0 ) {
				if( m_pBaseCls->CheckKamoku( CharCode, dd->data_edit, &km ) )
					pkd = &km;
				else {
					m_pBaseCls->KnamRedisp( ln, pn );
					return;
				}
			}

			CharCode = m_pBaseCls->ItemInp( ln, pn, CharCode, pkd );
#endif
			break;

		case FD_DSYOG_PN:
		case FD_CSYOG_PN:
			n = -1;
			if( inplen > 0 ) {
				if( m_pBaseCls->CheckSyog( CharCode, dd->data_edit ) )
					n = atoi( dd->data_edit );
				else {
					SetSyogBrn( ln, pn, -1 );
					return;
				}
			}

			CharCode = m_pBaseCls->SyogInp( ln, pn, CharCode, &n );
			break;

		case FD_DBVAL_PN:		//借方金額 or 出金伝票
			if( data != NULL )
				m_pBaseCls->ValInp( ln, pn, CharCode, inplen, dd->data_val );
			break;
		case FD_CRVAL_PN:		//貸方 or 入金伝票
			if( data != NULL )
				m_pBaseCls->ValInp( ln, pn, CharCode, inplen, dd->data_val );
			break;
		case FD_DBZEI_PN:
		case FD_CRZEI_PN:
			if( data != NULL )
				m_pBaseCls->ZeiInp( ln, pn, CharCode, inplen, dd->data_val );
			break;

		case FD_DBMN_PN:		//部門番号
		case FD_CBMN_PN:
#ifdef OLD_CLOSE	//DBEditで 入力 [01.30 /13]
			if( inplen > 0 ) {
				edit = &dd->data_edit;
			}
			else	edit = NULL;	//現在のデータをデータバッファより取得

			st = m_pBaseCls->Bmn_Check( pn, CharCode, &bmn, edit );
			m_pBaseCls->Bmn_Inp( pn, CharCode, &bmn );
#endif
			break;

		case FD_DKOJI_PN:		//工事番号
		case FD_CKOJI_PN:		//DBEdit で処理に変更
//			if( inplen > 0 ) {
//				edit = &dd->data_edit;
//			}
//			else	edit = NULL;	//現在のデータをデータバッファより取得

//			m_pBaseCls->Koji_Check( pn, CharCode, &string, edit );
//			m_pBaseCls->Koji_Inp( pn, CharCode, &string );
			break;

		case FD_TGNUM_PN:		//DBEdit で処理に変更
//			if( data != NULL )
//				m_pBaseCls->TgNumInp( ln, pn, CharCode, inplen, dd->data_edit );
			break;

		case FD_TGDATE_PN:
			if( data != NULL )
				m_pBaseCls->TgDateInp( ln, pn, CharCode, inplen, dd->data_day );
			break;

		case FD_HJIYU_PN:
			break;
		}
	}
	else {	//ヘッド部
		switch( pn ) {
		case DP_DENP_PN:		//伝票番号
			if( m_pBaseCls->IsTransDenpyo() ) {
				dno = -1;	//変更不可
			}
			else {
				if( inplen > 0 ) {
					if( dd->data_edit.IsEmpty() )
							dno = -1;
					else	dno = atoi( dd->data_edit );
				}
				else		dno = -1;
			}
			m_pBaseCls->DenpInp( pn, CharCode, dno );
			break;
		case DP_MM_PN:			//月
			if( m_pBaseCls->IsTransDenpyo() ) {
				chk = 99;	//変更不可
			}
			else {
				if( inplen > 0 ) {
					// 月チェック
					chk =  atoi( dd->data_edit );
				}
				else	chk = 99;	//現在のデータをデータバッファより取得
			}

			m_pBaseCls->MM_Check( pn, chk, &mm );
			m_pBaseCls->MM_Inp( pn, CharCode, mm );
			break;
		case DP_DD_PN:			//日
			if( m_pBaseCls->IsTransDenpyo() ) {
				day = 0xff;	//変更不可
			}
			else {
				if( inplen > 0 ) {
					day = dd->data_day[0];
				}
				else	day = 0xff;	//現在のデータをデータバッファより取得
			}

			m_pBaseCls->DD_Check( pn, day, &mm );
			m_pBaseCls->DD_Inp( pn, CharCode, mm );
			break;

		case DP_BMN_PN:			//部門(借貸で同じコード)
#ifdef OLD_CLOSE	//DBEditで 入力 [01.30 /13]
			if( inplen > 0 ) {
				edit = &dd->data_edit;
			}
			else	edit = NULL;	//現在のデータをデータバッファより取得

			st = m_pBaseCls->Bmn_Check( pn, CharCode, &bmn, edit );
			m_pBaseCls->Bmn_Inp( pn, CharCode, &bmn );
#endif
			break;

		case DP_KOJI_PN:		//工事番号(借貸で同じコード)
//			if( inplen > 0 ) {
//				edit = &dd->data_edit;
//			}
//			else	edit = NULL;	//現在のデータをデータバッファより取得

//			m_pBaseCls->Koji_Check( pn, CharCode, &string, edit );
//			m_pBaseCls->Koji_Inp( pn, CharCode, &string );
			break;

		}
	}

	// 合計部門を入力した場合、カーソル再セット
	if( pn == DP_BMN_PN || pn == FD_DBMN_PN || pn == FD_CBMN_PN ) {
#ifdef OLD_CLOSE	//DBEditで 入力 [01.30 /13]
		if( st == -1 ) {
			set_focus( ln, pn );
			return;
		}
#endif
	}

	if( ln == -1 ) {
	//	if( pn == DP_BMN_PN /*|| pn == DP_KOJI_PN*/ ) {
	//		CharCode = m_pBaseCls->ToViewSelectTerm( pn, CharCode );
	//	}

		m_pBaseCls->ToViewUpsideTermJob( pn, CharCode );
#ifdef OLD_CLOSE
		switch( CharCode ) {
		case VK_RETURN:
		case VK_TAB:
		case VK_RIGHT:
			set_nextfocus( ln, pn );
			break;
		case VK_LEFT:
			set_prevfocus( ln, pn );
			break;
		case VK_DOWN:
			// データポジションの先頭へ
			set_datafirstfocus();
			break;
		}
#endif
	}
	else {
		if( CharCode != 0 ) {
			m_pBaseCls->ToViewTermJob( ln, pn, CharCode );
		}
	}

//MyTrace( "DiagTermination END nChar[%d]\n", nChar );

}

// ダイアグラムカナターミネーション
void CDnpBaseDlg::DiagKanaTermination(int ID, short index, LPCTSTR kana)
{
	m_pBaseCls->ToViewKanaTerm( kana );
}


// 上段の 工事番号 入力のためのターミネーション
void CDnpBaseDlg::UpsideTermJob( int pn, long nChar )
{
	switch( nChar ) {
	case VK_RETURN:
	case VK_TAB:
	case VK_RIGHT:
		set_nextfocus( -1, pn );
		break;
	case VK_F2:
	case VK_LEFT:
		set_prevfocus( -1, pn );
		break;
	case VK_DOWN:
		// データポジションの先頭へ
		set_datafirstfocus();
		break;
	}
}


//---------------------------------------------
//	ダイアグラムInit
//
//---------------------------------------------
void CDnpBaseDlg::DiagInit()
{
	int		index, flg, no;
	// システム情報ファイル
//	ICSSysInfo	Csysinfo;
//	Csysinfo.GetInformation();
//	flg = Csysinfo.TekiyoEnter();
	flg = 0;

	DIAGRAM_ATTRIBUTE	datr;

	for( int ln = 0; ln < m_LineCount; ln++ ) {
	
		for( int i = 0; (m_pDTpos+i)->dpno != -1; i++ ) {
	
			index = (m_pDTpos+i)->dpindex + (ln * m_DclmCount);
			no = (m_pDTpos+i)->dpno;
			(m_pDiag + no)->GetAttribute( index, (LPUNKNOWN)&datr );

			switch( (m_pDTpos+i)->dp_pn ) {
			case FD_DEBT_PN:
			case FD_CRED_PN:
				datr.attr_editattr	|=	0x0300;
//				datr.attr_editattr	|=	0x0100;
				break;
			case FD_TKY_PN:
				datr.attr_editattr	|=	0x0300;
				if( flg ) datr.attr_editattr	|= 0x0400/*DIAG_EATTR_TEKIKEY*/;
				break;
			default:
				datr.attr_editattr	|=	0x0100;
				break;
			}

			(m_pDiag + no)->SetAttribute( index, (LPUNKNOWN)&datr, FALSE );
		}
	}

}

//------------------------------------------------------
//	入力許可サインをセット(部門など)
//
//------------------------------------------------------
void CDnpBaseDlg::ctl_init( int pn, BOOL enable )
{
	DNP_POS*	pDNpos;
	int i, no, idx, sgn;

	no = idx = -1;
	sgn = 0;

	//ヘッド部
	pDNpos = m_pHDpos;
	for( i = 0; (pDNpos+i)->dpno != -1; i++ ) {
		if( pn == (pDNpos+i)->dp_pn ) {
			sgn = 1;

			(pDNpos+i)->dp_enable = enable;
			break;
		}
	}

	if( sgn )	return;

	// データ部
	pDNpos = m_pDTpos;

	for( i = 0; (pDNpos+i)->dpno != -1; i++ ) {
		if( pn == (pDNpos+i)->dp_pn ) {
			sgn = 1;

			(pDNpos+i)->dp_enable = enable;
			break;
		}
	}
}

//------------------------------------------------------
//	1仕訳最初の入力位置をセット(仕訳データ部)
//
//------------------------------------------------------
DNP_POS* CDnpBaseDlg::GetFirstDnpPos()
{
	DNP_POS *pDNpos, *pRet;
	pDNpos = m_pDTpos;
	pRet = NULL;
	int i, chkpn;

	for( i = 0; (pDNpos+i)->dpno != -1; i++ ) {
		chkpn = (pDNpos+i)->dp_pn;

		if( ! (pDNpos+i)->dp_enable ) {
			continue;
		}
		else {
			pRet = (pDNpos+i);
			break;
		}
	}

	return pRet;
}


//------------------------------------------------------
//	1仕訳の最後の入力位置をセット(仕訳データ部)
//
//------------------------------------------------------
DNP_POS* CDnpBaseDlg::GetLastDnpPos()
{
	DNP_POS *pDNpos, *pRet;
	pDNpos = m_pDTpos;
	pRet = NULL;
	int i, chkpn;

	for( i = 0; (pDNpos+i)->dpno != -1; i++ ) {
		chkpn = (pDNpos+i)->dp_pn;

		if( ! (pDNpos+i)->dp_enable ) {
			continue;
		}
		else {
			pRet = (pDNpos+i);
		}
	}

	return pRet;
}


// フォーカスセット
//	PostMessage用
//
LRESULT CDnpBaseDlg::OnRetroFocus( WPARAM w, LPARAM l )
{
//	MAKEWORD

	int sgn = (short)HIWORD( w );
	int wp = (short)LOWORD( w );

	if( sgn == 0 ) {
		set_focus( wp, l );
	}
	else {
		(m_pDiag+wp)->SetPosition( l );
	}

	return 1;
}


LRESULT CDnpBaseDlg::OnRetroFocus2( WPARAM w, LPARAM l )
{
	CWnd* pwnd = GetDlgItem( w );
	if( pwnd != NULL ) {
		GotoDlgCtrl( pwnd );
	}

	return 1;
}


//------------------------------------------------------
//	フォーカスセット
//		int ln ... 行数(-1 のときはヘッド部)
//		inp pn ... ポジションサイン
//
//		return	 0 = OK, 
//				-1 = フォーカスセットできない
//------------------------------------------------------
int CDnpBaseDlg::set_focus( int ln, int pn )
{
	int i, ret, n;
	int no, idx;

	DNP_POS*	pDNpos;

	no = idx = -1;
	ret = -1;

	//ヘッド部
	if( ln == -1 ) {
		pDNpos = m_pHDpos;
		for( i = 0; (pDNpos+i)->dpno != -1; i++ ) {
			if( pn == (pDNpos+i)->dp_pn ) {

				if( (pDNpos+i)->dp_enable ) {
					no = (pDNpos+i)->dpno;
					idx = (pDNpos+i)->dpindex;
				}
				break;
			}
		}
	}
	else {
		// データ部
		pDNpos = m_pDTpos;

		for( i = 0; (pDNpos+i)->dpno != -1; i++ ) {

			if( pn == (pDNpos+i)->dp_pn ) {

				if( (pDNpos+i)->dp_enable ) {
					no = (pDNpos+i)->dpno;
					idx = (pDNpos+i)->dpindex;
					n = ((ln-1) * m_DclmCount);
					idx += n;
				}
				break;
			}
		}
	}

	if( no != -1 && idx != -1 ) {
		(m_pDiag + no)->SetPosition( idx );
		ret = 0;
	}

	return ret;
}

//------------------------------------------------------
//	次の位置へフォーカスセット
//		int ln ... 行数(-1 のときはヘッド部)
//		inp pn ... ポジションサイン
//------------------------------------------------------
int CDnpBaseDlg::set_nextfocus( int ln, int pn )
{
	int i, line, vect = 0;
	int chkpn, sgn, n;
	DNP_POS*	pDNpos;
	DNP_POS		newpos = {0};
	DNP_POS		fstpos = {0};
	newpos.dp_pn	= -1;
	newpos.dpindex	= -1;
	newpos.dpno		= -1;
	fstpos.dp_pn	= -1;
	fstpos.dpindex	= -1;
	fstpos.dpno		= -1;

	sgn = 0;
	chkpn = line = -1;

	//現在位置がヘッド部
	if( ln == -1 ) {
		pDNpos = m_pHDpos;
		for( i = 0; (pDNpos+i)->dpno != -1; i++ ) {
			if( pn == (pDNpos+i)->dp_pn ) {
				sgn = 1;
				continue;
			}

			if( sgn ) {
				if( ! (pDNpos+i)->dp_enable ) {
					continue;
				}
				else {
					newpos = *(pDNpos+i);
					break;
				}
			}
		}
	}
	//データ部検索
	if( newpos.dpno == -1 ) {
		pDNpos = m_pDTpos;
		sgn = 0;
		if( ln == -1 ) {
			pn = pDNpos->dp_pn;
		}

		for( i = 0; (pDNpos+i)->dpno != -1; i++ ) {
			chkpn = (pDNpos+i)->dp_pn;

			if( pn == (pDNpos+i)->dp_pn ) {
				if( ln == -1 ) {	// ヘッド部からの送り
					if( (pDNpos+i)->dp_enable ) {
						line = (ln == -1) ? 1 : ln;
						newpos = *(pDNpos+i);
						break;
					}
				}
				sgn = 1;
				continue;
			}

			if( sgn ) {
				if( ! (pDNpos+i)->dp_enable ) {
					continue;
				}
				else {
					line = (ln == -1) ? 1 : ln;
					newpos = *(pDNpos+i);
					break;
				}
			}
		}
		//最初の位置ヘセット
		if( newpos.dpno == -1 ) {
			DNP_POS* ps;
			ps = GetFirstDnpPos();
			if( ps ) {
				line = (ln == -1) ? 1 : ln;
				line++;
				memcpy( &newpos, ps, sizeof DNP_POS );
			}
		}
		//転送済み仕訳のチェックのため
		if( newpos.dpno != -1 ) {
			m_pBaseCls->set_position( line, newpos.dp_pn );
		}
		return 0;
	}

	if( newpos.dpno != -1 ) {
		if( line > m_LineCount )	line = m_LineCount;
		n = 0;
		if( line > 0 )	n = ((line-1) * m_DclmCount);

		(m_pDiag + newpos.dpno)->SetPosition( (newpos.dpindex + n) );
	}

	return 0;
}


//------------------------------------------------------
//	前の位置へフォーカスセット
//		int ln ... 行数(-1 のときはヘッド部)
//		inp pn ... ポジションサイン
//------------------------------------------------------
int CDnpBaseDlg::set_prevfocus( int ln, int pn )
{
	int i, line, vect = 0;
	int chkpn, spos;
	DNP_POS*	pDNpos;
	DNP_POS		newpos = {0};
	DNP_POS		fstpos = {0};
	newpos.dp_pn	= -1;
	newpos.dpindex	= -1;
	newpos.dpno		= -1;
	fstpos.dp_pn	= -1;
	fstpos.dpindex	= -1;
	fstpos.dpno		= -1;

	spos = 0;
	chkpn = line = -1;

	//現在位置がヘッド部
	if( ln == -1 ) {
		pDNpos = m_pHDpos;
		for( i = 0; (pDNpos+i)->dpno != -1; i++ ) {
			if( pn == (pDNpos+i)->dp_pn ) {
				spos = i;
				break;
			}
		}

		if( spos > 0 ) {
			for( i = (spos-1); i >= 0; i-- ) {

				if( ! (pDNpos+i)->dp_enable ) {
					continue;
				}
				else {
					newpos = *(pDNpos+i);
					break;
				}
			}
		}
	}
	else {
		//データ部検索
		pDNpos = m_pDTpos;
		spos = 0;

		for( i = 0; (pDNpos+i)->dpno != -1; i++ ) {
			chkpn = (pDNpos+i)->dp_pn;

			if( pn == (pDNpos+i)->dp_pn ) {
				spos = i;
				break;;
			}
		}

		if( spos > 0 ) {
			// 一つ前の位置よりサーチ
			for( i = (spos-1); i >= 0; i-- ) {

				if( ! (pDNpos+i)->dp_enable ) {
					continue;
				}
				else {
					line = ln;
					newpos = *(pDNpos+i);
					break;
				}
			}
		}
		else {
			// ヘッド部の最終位置へ
			pDNpos = m_pHDpos;
			int idx = -1;
			for( i = 0; (pDNpos+i)->dpno != -1; i++ ) {
				if( (pDNpos+i)->dp_enable ) {
					idx = i;
				}
			}

			if( idx >= 0 ) {
				newpos = *(pDNpos+idx);
				line = -1;
			}
		}

		//最初の位置ヘセット
		if( newpos.dpno == -1 ) {
			DNP_POS* ps;
			ps = GetFirstDnpPos();
			if( ps ) {
				line = (ln == -1) ? 1 : ln;
				line++;
				memcpy( &newpos, ps, sizeof DNP_POS );
			}
		}
	}

	if( newpos.dpno != -1 ) {
		if( line > m_LineCount )	line = m_LineCount;
		int n = 0;
		if( line > 0 )	n = ((line-1) * m_DclmCount);

		(m_pDiag + newpos.dpno)->SetPosition( (newpos.dpindex + n) );
	}

	return 0;
}


//------------------------------------------------------
//	データ行の先頭へフォーカスセット
//------------------------------------------------------
int CDnpBaseDlg::set_datafirstfocus()
{
	int i, line, vect = 0;
	int chkpn, spos;
	DNP_POS*	pDNpos;
	DNP_POS		newpos = {0};
	newpos.dp_pn	= -1;
	newpos.dpindex	= -1;
	newpos.dpno		= -1;

	spos = 0;
	chkpn = line = -1;

	//データ部検索
	pDNpos = m_pDTpos;
	spos = 0;

	for( i = 0; (pDNpos+i)->dpno != -1; i++ ) {

		if( ! (pDNpos+i)->dp_enable ) {
			continue;
		}
		else {
			line = 1;
			newpos = *(pDNpos+i);
			break;
		}
	}

	if( newpos.dpno != -1 ) {
		if( line > m_LineCount )	line = m_LineCount;
		int n = 0;
		if( line > 0 )	n = ((line-1) * m_DclmCount);

		(m_pDiag + newpos.dpno)->SetPosition( (newpos.dpindex + n) );
	}

	return 0;
}

//------------------------------------------------------
//		伝票をダイアグラムにセット
//
//------------------------------------------------------
void CDnpBaseDlg::_SetDnpyoNo(int dgno, int index, int dnpno )
{
	int dno, idx;

	if( dgno == -1 )	dno = m_dgno;
	else				dno = dgno;

	if( index == -1 )	idx = m_index;
	else				idx = index;

	DIAGRAM_DATA dd;
	dd.data_imgdata = NULL;

	if( dnpno != -1 ) {
		dd.data_edit.Format( "%d", dnpno );
		(m_pDiag + dno)->SetData( idx, (LPUNKNOWN)&dd );
	}
	else {
		dd.data_edit = "";
		(m_pDiag + dno)->DataClear( idx, TRUE );
	}

//	(m_pDiag + dno)->OffControl( TRUE );
//	(m_pDiag + dno)->SetPosition( idx );
//	(m_pDiag + dno)->OffControl( FALSE );

}


//------------------------------------------------------
//		月をダイアグラムにセット
//
//------------------------------------------------------
void CDnpBaseDlg::_SetMM(int dgno, int index, BYTE mm )
{
	int dno, idx;

	if( dgno == -1 )	dno = m_dgno;
	else				dno = dgno;

	if( index == -1 )	idx = m_index;
	else				idx = index;

	DIAGRAM_DATA dd;
	dd.data_imgdata = NULL;

	if( mm != 0 ) {
		dd.data_edit.Format( "%02x", mm & 0x1f );
		(m_pDiag + dno)->SetData( idx, (LPUNKNOWN)&dd );
	}
	else {
		dd.data_edit = "";
		(m_pDiag + dno)->DataClear( idx, TRUE );
	}
}


//------------------------------------------------------
//		日をダイアグラムにセット
//
//------------------------------------------------------
void CDnpBaseDlg::_SetDD(int dgno, int index, BYTE day )
{
	int dno, idx;

	if( dgno == -1 )	dno = m_dgno;
	else				dno = dgno;

	if( index == -1 )	idx = m_index;
	else				idx = index;

	DIAGRAM_DATA dd;
	dd.data_imgdata = NULL;

	if( day != 0 ) {
	//	dd.data_edit.Format( "%02x", day & 0x31 );
		dd.data_day[0] = day;
		(m_pDiag + dno)->SetData( idx, (LPUNKNOWN)&dd );
	}
	else {
		dd.data_edit = "";
		(m_pDiag + dno)->DataClear( idx, TRUE );
	}
}


//------------------------------------------------------
//		部門をダイアグラムにセット
//
//------------------------------------------------------
void CDnpBaseDlg::_SetBmon( int dgno, int index, int bmn, struct _SET_KMKETC* psk, BOOL bReset/*=FALSE*/ )
{
	int dno, idx;

	if( dgno == -1 )	dno = m_dgno;
	else				dno = dgno;

	if( index == -1 )	idx = m_index;
	else				idx = index;

	DIAGRAM_DATA dd;
	dd.data_imgdata = NULL;

	if( bmn != -1 ) {
		DIAGRAM_ATTRIBUTE	da;
		(m_pDiag + dno)->GetAttribute( idx, (LPUNKNOWN)&da );

		if( psk == NULL || psk->etc.IsEmpty() ) {
			set_codestr( dd.data_edit.GetBuffer(128), 128, bmn, m_pBaseCls->m_pZm->zvol->bmcol );
		//	m_pBaseCls->BumonCodeToStr( dd.data_edit.GetBuffer(128), 128, bmn );
			dd.data_edit.ReleaseBuffer();
			da.attr_fcolor = RGB_BLACK;
			(m_pDiag + dno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );
		}
		else {
			// 部門名称をセット
			dd.data_edit = psk->etc;
			LPSTR p = dd.data_edit.GetBuffer(64);
			int n = get_strcnt( p, 8 );
			p[n] = '\0';
			dd.data_edit.ReleaseBuffer(-1);

			da.attr_fcolor = psk->etc_col;
			(m_pDiag + dno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );
		}

		dd.data_imgdata = NULL;

		if( bReset )	(m_pDiag + dno)->OffControl( TRUE );

		(m_pDiag + dno)->SetData( idx, (LPUNKNOWN)&dd );

		if( bReset ) {
			(m_pDiag + dno)->SetPosition( idx );
			(m_pDiag + dno)->OffControl( FALSE );
		}
	}
	else {
		dd.data_edit = "";
		(m_pDiag + dno)->DataClear( idx, TRUE );
	}
}

//------------------------------------------------------
//		工事番号をダイアグラムにセット
//
//------------------------------------------------------
void CDnpBaseDlg::_SetKoji( int dgno, int index, CString koji, struct _SET_KMKETC* psk, BOOL bReset/*=FALSE*/ )
{
	int dno, idx;

	if( dgno == -1 )	dno = m_dgno;
	else				dno = dgno;

	if( index == -1 )	idx = m_index;
	else				idx = index;

	DIAGRAM_DATA dd;
	dd.data_imgdata = NULL;
	char bf[128];

	if( ! koji.IsEmpty() ) {

		DIAGRAM_ATTRIBUTE	da;
		(m_pDiag + dno)->GetAttribute( idx, (LPUNKNOWN)&da );

		if( psk == NULL || psk->etc.IsEmpty() ) {
		//	m_pBaseCls->KojiCodeToStr( bf, sizeof bf, koji );
			set_codestring( bf, sizeof bf, (char*)(LPCTSTR)koji, m_pBaseCls->m_pZm->zvol->kjcol );
			dd.data_edit.Format( "%s", bf );
			da.attr_fcolor = RGB_BLACK;
			(m_pDiag + dno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );
		}
		else {
			// 工事名称をセット
			dd.data_edit = psk->etc;
			LPSTR p = dd.data_edit.GetBuffer(64);
			int n = get_strcnt( p, 12 );
			p[n] = '\0';
			dd.data_edit.ReleaseBuffer(-1);

			da.attr_fcolor = psk->etc_col;
			(m_pDiag + dno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );
		}

		dd.data_imgdata = NULL;

		if( bReset )	(m_pDiag + dno)->OffControl( TRUE );

		(m_pDiag + dno)->SetData( idx, (LPUNKNOWN)&dd );

		if( bReset ) {
			(m_pDiag + dno)->SetPosition( idx );
			(m_pDiag + dno)->OffControl( FALSE );
		}
	}
	else {
		dd.data_edit = "";
		(m_pDiag + dno)->DataClear( idx, TRUE );
	}
}


//------------------------------------------------------
//		科目名称をダイアグラムにセット
//
//------------------------------------------------------
void CDnpBaseDlg::_SetKnam(int dgno, int index, SETMEISYO* knam, BOOL bReset/*=FALSE*/ )
{
	int dno, idx;

	if( dgno == -1 )	dno = m_dgno;
	else				dno = dgno;

	if( index == -1 )	idx = m_index;
	else				idx = index;
	DIAGRAM_DATA dd;
	DIAGRAM_ATTRIBUTE	da;
	(m_pDiag + dno)->GetAttribute( idx, (LPUNKNOWN)&da );

	if( knam == NULL ) {
		da.attr_fcolor = RGB_BLACK;
		(m_pDiag + dno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );

		(m_pDiag + dno)->DataClear( idx, TRUE );
	}
	else {
		if( knam->sm_etc[0] ) {
			dd.data_edit = knam->sm_etc;
			da.attr_fcolor = knam->sm_etccol;
			(m_pDiag + dno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );
		}
		else {
			dd.data_edit = knam->sm_name;
			da.attr_fcolor = RGB_BLACK;
			(m_pDiag + dno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );
		}
		dd.data_imgdata = NULL;

		if( bReset ) {
			(m_pDiag + dno)->OffControl( TRUE );
		}

		(m_pDiag + dno)->SetData( idx, (LPUNKNOWN)&dd );

		if( bReset ) {
			(m_pDiag + dno)->SetPosition( idx );
			(m_pDiag + dno)->OffControl( FALSE );
		}
	}
}


//------------------------------------------------------
//		諸口枝番をダイアグラムにセット
//
//------------------------------------------------------
void CDnpBaseDlg::_SetSyogBrn(int dgno, int index, int syogbrn )
{
	int dno, idx;

	if( dgno == -1 )	dno = m_dgno;
	else				dno = dgno;

	if( index == -1 )	idx = m_index;
	else				idx = index;

	DIAGRAM_DATA dd;
	DIAGRAM_ATTRIBUTE	da;
	(m_pDiag + dno)->GetAttribute( idx, (LPUNKNOWN)&da );

	if( syogbrn <= 0 ) {
		(m_pDiag + dno)->DataClear( idx, TRUE );
	}
	else {
		da.attr_fcolor = RGB_DARK_BLUE;
		(m_pDiag + dno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );

		dd.data_edit.Format( "%03d", syogbrn );
		(m_pDiag + dno)->SetData( idx, (LPUNKNOWN)&dd );
	}
}


//------------------------------------------------------
//	金額をダイアグラムにセット
//	sgn = 0 金額表示
//	sgn = 1 有価証券非課税(*)
//  sgn = 2 金額を表示しない
//  sgn = 3 税額がデフォルトと異なる(背景色 緑)
//	sgn = 4 税額がデフォルトと異なる(背景色 赤 --> 確定仕訳で修正可)
//  sgn = 5 輸出売上で、税額修正不可
//------------------------------------------------------
void CDnpBaseDlg::_SetVal(int dgno, short index, char* val, int sgn/*=0*/, COLORREF back/*=(DWORD)-1*/ )
{
	int dno, idx;

	if( dgno == -1 )	dno = m_dgno;
	else				dno = dgno;

	if( index == -1 )	idx = m_index;
	else				idx = index;

	DIAGRAM_ATTRIBUTE	da;
	DIAGRAM_DATA		dd;

	(m_pDiag + dno)->GetAttribute( idx, (LPUNKNOWN)&da );
	if( val != NULL && l_test( val ) < 0 )
			da.attr_fcolor = RGB_RED;
	else	da.attr_fcolor = RGB_BLACK;

	if (sgn == 3) {
		da.attr_bcolor = RGB_ZEI_MDFY;
	}
	else if (sgn == 4) {
		da.attr_bcolor = RGB_SYZ_MDFY;
	}
	else {
		if (back == (DWORD)-1)
			da.attr_bcolor = RGB_WHITE;
		else
			da.attr_bcolor = back;
	}

	DNP_POS* pos;
	pos = m_pKeiPos;
	BOOL bKei = FALSE;

	for(int i = 0; (pos+i)->dpno != -1; i++ ) {
		if( (pos+i)->dpindex == index ) {
			bKei = TRUE;
		}
	}

	if( sgn == 0 || sgn > 1 ) {
		da.attr_editformat = bKei ? "S,SSS,SSS,SSS,SSS" : "SSS,SSS,SSS,SSS";
	}
	else {
		da.attr_editformat = bKei ? "*SSSS,SSS,SSS,SSS" : "*SSS,SSS,SSS,SSS";
	}

	(m_pDiag + dno)->SetAttribute( idx, (LPUNKNOWN)&da, TRUE );

	if( val != NULL && sgn != 2 ) {
		memcpy( dd.data_val, val, sizeof dd.data_val );;
		dd.data_imgdata = NULL;

	//	(m_pDiag + dno)->OffControl( TRUE );
		(m_pDiag + dno)->SetData( idx, (LPUNKNOWN)&dd );
	//	(m_pDiag + dno)->SetPosition( idx );
	}
	else {
		(m_pDiag + dno)->DataClear( idx, TRUE );
	}
}


//------------------------------------------------------
//		摘要をダイアグラムにセット
//
//------------------------------------------------------
void CDnpBaseDlg::_SetTekiyo(int dgno, short index, char* tky)
{
	int dno, idx;

	if( dgno == -1 )	dno = m_dgno;
	else				dno = dgno;

	if( index == -1 )	idx = m_index;
	else				idx = index;

	DIAGRAM_DATA		dd;

#ifdef KEEP
	DIAGRAM_ATTRIBUTE	da;
	(m_pDiag + dno)->GetAttribute( idx, (LPUNKNOWN)&da );
	(m_pDiag + dno)->SetAttribute( idx, (LPUNKNOWN)&da, TRUE );
#endif

	if( tky != NULL ) {
		dd.data_edit = tky;
		dd.data_imgdata = NULL;

	//	(m_pDiag + dno)->OffControl( TRUE );
		(m_pDiag + dno)->SetData( idx, (LPUNKNOWN)&dd );
	//	(m_pDiag + dno)->SetPosition( idx );
	}
	else {
TRACE( "_SetTekiyo Clear dno = %d, idx = %d\n", dno, idx );

		(m_pDiag + dno)->DataClear( idx, TRUE );
	}
}

//------------------------------------------------------
//		手形番号をダイアグラムにセット
//
//------------------------------------------------------
void CDnpBaseDlg::_SetTgNumber(int dgno, short index, char* tgnum)
{
	int dno, idx;

	if( dgno == -1 )	dno = m_dgno;
	else				dno = dgno;

	if( index == -1 )	idx = m_index;
	else				idx = index;

	DIAGRAM_DATA		dd;

#ifdef KEEP
	DIAGRAM_ATTRIBUTE	da;
	(m_pDiag + dno)->GetAttribute( idx, (LPUNKNOWN)&da );
	(m_pDiag + dno)->SetAttribute( idx, (LPUNKNOWN)&da, TRUE );
#endif

	if( tgnum != NULL ) {
		dd.data_edit = tgnum;
		dd.data_imgdata = NULL;

	//	(m_pDiag + dno)->OffControl( TRUE );
		(m_pDiag + dno)->SetData( idx, (LPUNKNOWN)&dd );
	//	(m_pDiag + dno)->SetPosition( idx );
	}
	else {
		(m_pDiag + dno)->DataClear( idx, TRUE );
	}
}

//------------------------------------------------------
//		手形期日をダイアグラムにセット
//
//------------------------------------------------------
void CDnpBaseDlg::_SetTgDate(int dgno, short index, BYTE* tgdate)
{
	int dno, idx;

	if( dgno == -1 )	dno = m_dgno;
	else				dno = dgno;

	if( index == -1 )	idx = m_index;
	else				idx = index;

	DIAGRAM_DATA		dd;

#ifdef KEEP
	DIAGRAM_ATTRIBUTE	da;
	(m_pDiag + dno)->GetAttribute( idx, (LPUNKNOWN)&da );
	(m_pDiag + dno)->SetAttribute( idx, (LPUNKNOWN)&da, TRUE );
#endif

	if( tgdate != NULL ) {
		memcpy( dd.data_day, tgdate, 3 );
		dd.data_imgdata = NULL;

	//	(m_pDiag + dno)->OffControl( TRUE );
		(m_pDiag + dno)->SetData( idx, (LPUNKNOWN)&dd );
	//	(m_pDiag + dno)->SetPosition( idx );
	}
	else {
		(m_pDiag + dno)->DataClear( idx, TRUE );
	}
}

//------------------------------------------------------
//		変動事由をダイアグラムにセット
//
//------------------------------------------------------
void CDnpBaseDlg::_SetHjiyu(int dgno, short index, CString* hjiyu)
{
	int dno, idx;

	if( dgno == -1 )	dno = m_dgno;
	else				dno = dgno;

	if( index == -1 )	idx = m_index;
	else				idx = index;

	DIAGRAM_DATA		dd;

	if( hjiyu != NULL && ! hjiyu->IsEmpty() ) {
		dd.data_edit = *hjiyu;
		dd.data_imgdata = NULL;

	//	(m_pDiag + dno)->OffControl( TRUE );
		(m_pDiag + dno)->SetData( idx, (LPUNKNOWN)&dd );
	//	(m_pDiag + dno)->SetPosition( idx );
	}
	else {
		(m_pDiag + dno)->DataClear( idx, TRUE );
	}
}



// 他から呼ばれるpublic なデータセット関数
//
void CDnpBaseDlg::SetDnpyoNo( int pn, int dnpno )
{
	DNP_POS* pos;
	pos = GetDNP_POS( -1, pn );
	if( pos == NULL ) {
		ASSERT( FALSE );
		return;
	}
	
	_SetDnpyoNo( pos->dpno, pos->dpindex, dnpno );
}

void CDnpBaseDlg::SetMM( int pn, BYTE mm )
{
	DNP_POS* pos;
	pos = GetDNP_POS( -1, pn );
	if( pos == NULL ) {
		ASSERT( FALSE );
		return;
	}
	_SetMM( pos->dpno, pos->dpindex, mm );
}

void CDnpBaseDlg::SetDD( int pn, BYTE day )
{
	DNP_POS* pos;
	pos = GetDNP_POS( -1, pn );
	if( pos == NULL ) {
		ASSERT( FALSE );
		return;
	}
	_SetDD( pos->dpno, pos->dpindex, day );
}

void CDnpBaseDlg::SetBmon( int ln, int pn, int bmn, struct _SET_KMKETC* psk, BOOL bReset/*=FALSE*/ )
{
	DNP_POS* pos;
	pos = GetDNP_POS( ln, pn );
	if( pos == NULL ) {
		ASSERT( FALSE );
		return;
	}
	_SetBmon( pos->dpno, pos->dpindex, bmn, psk, bReset);
}

void CDnpBaseDlg::SetKoji( int ln, int pn, CString koji, struct _SET_KMKETC* psk, BOOL bReset/*=FALSE*/ )
{
	DNP_POS* pos;
	pos = GetDNP_POS( ln, pn );
	if( pos == NULL ) {
		ASSERT( FALSE );
		return;
	}
	_SetKoji( pos->dpno, pos->dpindex, koji, psk, bReset );
}

void CDnpBaseDlg::SetKnam( int ln, int pn,  SETMEISYO* knam, BOOL bReset/*=FALSE*/ )
{
	DNP_POS* pos;
	pos = GetDNP_POS( ln, pn );
	if( pos == NULL ) {
		ASSERT( FALSE );
		return;
	}
	_SetKnam( pos->dpno, pos->dpindex, knam, bReset );
}


void CDnpBaseDlg::SetSyogBrn( int ln, int pn, int syogbrn )
{
	DNP_POS* pos;
	pos = GetDNP_POS( ln, pn );
	if( pos == NULL ) {
		ASSERT( FALSE );
		return;
	}

	_SetSyogBrn( pos->dpno, pos->dpindex, syogbrn );
}


void CDnpBaseDlg::SetVal( int ln, int pn, char* val, int sgn)
{
	DNP_POS* pos;
	pos = GetDNP_POS( ln, pn );
	if( pos == NULL ) {
		ASSERT( FALSE );
		return;
	}
	int dt_ln = m_pBaseCls->get_dataline(ln);
	// 背景色取得
	COLORREF back = m_pBaseCls->GetConfirmBackColor(m_pBaseCls->_getCND(dt_ln, FD_TKY_PN)->EDT_sg);

	_SetVal( pos->dpno, pos->dpindex, val, sgn, back );
}


void CDnpBaseDlg::SetTekiyo( int ln, int pn, char* tky)
{
	DNP_POS* pos;
	pos = GetDNP_POS( ln, pn );
	if( pos == NULL ) {
		ASSERT( FALSE );
		return;
	}
	_SetTekiyo( pos->dpno, pos->dpindex, tky );
}

// 手形番号
void CDnpBaseDlg::SetTgNumber( int ln, int pn, char* tgnum)
{
	DNP_POS* pos;
	pos = GetDNP_POS( ln, pn );
	if( pos == NULL ) {
		ASSERT( FALSE );
		return;
	}
	_SetTgNumber( pos->dpno, pos->dpindex, tgnum );
}

// 手形期日
void CDnpBaseDlg::SetTgDate( int ln, int pn, BYTE* tgdate)
{
	DNP_POS* pos;
	pos = GetDNP_POS( ln, pn );
	if( pos == NULL ) {
		ASSERT( FALSE );
		return;
	}
	_SetTgDate( pos->dpno, pos->dpindex, tgdate );
}

// 変動事由
void CDnpBaseDlg::SetHjiyu( int ln, int pn, CString* hjiyu )
{
	DNP_POS* pos;
	pos = GetDNP_POS( ln, pn );
	if( pos == NULL ) {
		ASSERT( FALSE );
		return;
	}
	_SetHjiyu( pos->dpno, pos->dpindex, hjiyu );
}

// 付箋
void CDnpBaseDlg::SetHusen( int ln, int pn, int husen, char* hscmnt)
{
	DNP_POS* pos;
	pos = GetDNP_POS( ln, pn );
	if( pos == NULL ) {
		ASSERT( FALSE );
		return;
	}

	CHusenStatic* pHusen;
	pHusen = (CHusenStatic*)GetDlgItem( IDC_HUSEN1 + (ln-1) );

	pHusen->SetIconIndex( husen );
	if (husen == 0) {
		char tmp[128] = { 0 };
		strcpy_s(tmp, sizeof tmp, hscmnt);
		if (tmp[0] != '\0') {
			int l = get_strcnt((char*)(LPCTSTR)tmp, 4);
			tmp[l] = '\0';
		}
		pHusen->UpdateCaption(tmp, NULL);
		pHusen->SetBackColor(RGB_WHITE);
	}
	pHusen->Invalidate();
	pHusen->UpdateWindow();
}

//-------------------------------------------------------
//	消費税文字列をセット
//
//-------------------------------------------------------
void CDnpBaseDlg::PutsSyz( int ln, int pn, char* syztxt, char sy_chg, COLORREF backcol )
{
	DNP_POS* pos;

	pos = GetDNP_POS( ln, pn );
	if( pos == NULL ) {
		ASSERT( FALSE );
		return;
	}

	DIAGRAM_DATA dd;
	dd.data_edit = syztxt;
	dd.data_imgdata = NULL;

//	(m_pDiag + dno)->OffControl( TRUE );
	check_syzstring( dd.data_edit );

	(m_pDiag + pos->dpno)->SetData( pos->dpindex, (LPUNKNOWN)&dd );

	// 色の変更
	DIAGRAM_ATTRIBUTE da;
	(m_pDiag + pos->dpno)->GetAttribute( pos->dpindex, (LPUNKNOWN)&da );

	if( sy_chg == -1 ) {
		da.attr_bcolor	= RGB_GRAY;
	}
	else {
		da.attr_bcolor	= sy_chg ? RGB_SYZ_MDFY : backcol;
	}
//	da.attr_fcolor	= txt;

	(m_pDiag + pos->dpno)->SetAttribute( pos->dpindex, (LPUNKNOWN)&da, TRUE );

//	(m_pDiag + dno)->SetPosition( idx );
}


//-----------------------------------------------------------
//	入力項目の矩形取得
//-----------------------------------------------------------
void CDnpBaseDlg::GetRect( int ln, int pn, CRect& rect )
{
	DNP_POS* pos;

	pos = GetDNP_POS( ln, pn );
	if( pos == NULL ) {
		ASSERT( FALSE );
		return;
	}

	(m_pDiag+pos->dpno)->GetRect( pos->dpindex, (LPUNKNOWN)&rect );
}


//------------------------------------------
//	諸口貸借を表示
//	char* syog		諸口金額
//------------------------------------------
void CDnpBaseDlg::SyogDisp( char* syog )
{
	DNP_POS* pos;

	pos = GetDNP_POS( -1, DP_SYOGT_PN );
	if( pos == NULL ) {
		ASSERT( FALSE );
		return;
	}
	char buf[36];

	l_print( buf, syog, "sss,sss,sss,ss9" );

	DIAGRAM_ATTRIBUTE	da;
	DIAGRAM_DATA		dd;

	(m_pDiag + pos->dpno)->GetAttribute( pos->dpindex, (LPUNKNOWN)&da );
	if( l_test( syog ) < 0 )
			da.attr_fcolor = RGB_RED;
	else	da.attr_fcolor = RGB_BLACK;
	(m_pDiag + pos->dpno)->SetAttribute( pos->dpindex, (LPUNKNOWN)&da, TRUE );

	dd.data_disp = buf;
	dd.data_imgdata = NULL;

	(m_pDiag + pos->dpno)->SetData( pos->dpindex, (LPUNKNOWN)&dd );
}

//----------------------------------------
//	借方合計表示
//	貸方合計表示
//----------------------------------------
void CDnpBaseDlg::total_dsp()
{
	DNP_POS* pos;
	pos = m_pKeiPos;

	// 一仕訳あたりのDiag項目数
	int cnt = 0;
	for( int i = 0; (pos + i)->dpno != -1; i++ ) {
		if( (pos+i)->dp_pn == FD_DBVAL_PN ) {
			if( m_pBaseCls->DEBT_TOTAL_CND.INP_sg ) {
				_SetVal( (pos+i)->dpno, (pos+i)->dpindex, (char*)m_pBaseCls->DEBT_TOTAL.DP_VAL );
			}
			else {
				_SetVal( (pos+i)->dpno, (pos+i)->dpindex, NULL );
			}
		}
		else if( (pos+i)->dp_pn == FD_CRVAL_PN ) {
			if( m_pBaseCls->CRED_TOTAL_CND.INP_sg ) {
				_SetVal( (pos+i)->dpno, (pos+i)->dpindex, (char*)m_pBaseCls->CRED_TOTAL.DP_VAL );
			}
			else {
				_SetVal( (pos+i)->dpno, (pos+i)->dpindex, NULL );
			}
		}
	}
}


void CDnpBaseDlg::PostNcDestroy()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	delete this;

//	ICSDialog::PostNcDestroy();
}

BOOL CDnpBaseDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( (m_hAccelTable != NULL) && ::TranslateAccelerator( m_hWnd, m_hAccelTable, pMsg))
	{
		return TRUE;
	}
	if( m_pBaseCls->m_DI.IsConFirmMaster() ) {
		if( pMsg->message == WM_RBUTTONDOWN ) {
			return TRUE;
		}
	}
	m_pBaseCls->LineCopyMsgJob( pMsg );

	if( pMsg->message == WM_KEYDOWN ) {
		if( pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_END || pMsg->wParam == VK_PRIOR || pMsg->wParam == VK_NEXT ||
			pMsg->wParam == VK_INSERT || pMsg->wParam == VK_DELETE ||
			pMsg->wParam == VK_F3 || pMsg->wParam == VK_F4 || pMsg->wParam == VK_F12 || pMsg->wParam == VK_F7 ) {

			if( m_pBaseCls->ToViewAccelerator( 0, pMsg->wParam ) )
				return TRUE;
		}
		if( pMsg->wParam == VK_PROCESSKEY ) {
			m_pBaseCls->BmnIMEkeydownJob( pMsg->wParam );
		//	if( m_pBaseCls->BmnIMEkeydownJob( pMsg->wParam ) )
		//		return TRUE;
		}
		if( m_pBaseCls->IsKamokuPosition() ) {
			if( m_pBaseCls->KanaCheckVK_BACK(pMsg) )
				return 1;
		}
	}
	else if( pMsg->message == WM_KEYUP ) {
		if( m_pBaseCls->IsKamokuPosition() ) {
			m_pBaseCls->FukuKanaSearch(pMsg);
		}
	}
	else if( pMsg->message == WM_CHAR ) {
		// 条件に一致しないので、文字入力を許可しない
		if( pMsg->wParam != VK_RETURN ) {
			if( ! m_pBaseCls->IsInputCharEnable() )
				return TRUE;
		}
	}
	else if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F10) {
		if (m_pBaseCls->ChangeZeiritsu()) {
			return TRUE;
		}
		else {
			if( m_pBaseCls->ChangeSotoMenZei() ) {
				return TRUE;
			}
		}
	}
	else if( pMsg->message == WM_LBUTTONDOWN ) {
		m_pBaseCls->CancelInvNoIME();
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}


//--------------------------------------------------
//	ダイアグの設定
//	int bmon	部門ありか？
//	int koji	工事ありか？
//--------------------------------------------------
void CDnpBaseDlg::diag_inz()
{
	DNP_POS* p;
	int ln, n, i, idx;
	DIAGRAM_ATTRIBUTE da;

	// 仕訳データ部
	p = m_pHDpos;

	for( i = 0; (p+i)->dpno != -1; i++ ) {
		idx = (p+i)->dpindex;

		// 入力レングス設定
		if( (p+i)->dp_pn == DP_BMN_PN ) {
			(m_pDiag + (p+i)->dpno)->GetAttribute( idx, (LPUNKNOWN)&da );
			da.attr_inpmax = m_BmonLNG;
			(m_pDiag + (p+i)->dpno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );
		}
		else if( (p+i)->dp_pn == DP_KOJI_PN ) {
			(m_pDiag + (p+i)->dpno)->GetAttribute( idx, (LPUNKNOWN)&da );
			da.attr_inpmax = m_KojiLNG;
			(m_pDiag + (p+i)->dpno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );
		}
		else if( (p + i)->dp_pn == DP_SYONUM_PN ) {
			(m_pDiag + (p + i)->dpno)->GetAttribute(idx, (LPUNKNOWN)&da);
			da.attr_inpmax = m_SyoNumLNG;
			(m_pDiag + (p + i)->dpno)->SetAttribute(idx, (LPUNKNOWN)&da, FALSE);
		}
	}

	CICSDBEDT *pDBedt;
	pDBedt = (CICSDBEDT *)GetDlgItem( IDC_ICSDBEDT_KOJI );

	pDBedt->SetMaxLen( m_KojiLNG );
	pDBedt->SetMinval( "0" );
	pDBedt->SetMaxval( "0" );
	pDBedt->SetFrame( 1, RGB_BLACK );

	pDBedt = (CICSDBEDT *)GetDlgItem( IDC_ICSDBEDT_BMON );

	pDBedt->SetMaxLen( m_BmonLNG );
	pDBedt->SetMinval( "0" );
	pDBedt->SetMaxval( "0" );
	pDBedt->SetFrame( 1, RGB_BLACK );

	pDBedt = (CICSDBEDT *)GetDlgItem( IDC_ICSDBEDT_TGNUM );
	pDBedt->SetFrame( 1, RGB_BLACK );
	pDBedt->ImeEndMode( TRUE );
	pDBedt->SetInverse( TRUE );
	pDBedt->SetKana( TRUE );
	pDBedt->SetCmove( TRUE );

	pDBedt = (CICSDBEDT *)GetDlgItem(IDC_ICSDBEDT_SYONUM);
	pDBedt->SetFrame(1, RGB_BLACK);
	pDBedt->SetMaxLen(m_SyoNumLNG);
	pDBedt->ImeEndMode(FALSE);
	pDBedt->SetInverse(TRUE);
	pDBedt->SetCmove(TRUE);

	pDBedt = (CICSDBEDT *)GetDlgItem(IDC_ICSDBEDT_KMINP);
	int kmklen;
	kmklen = KMKCD_LN + 1 + m_pBaseCls->m_pZm->zvol->edcol;
	pDBedt->SetMaxLen(kmklen);
	pDBedt->SetFrame(1, RGB_BLACK);

	// 仕訳データ部
	p = m_pDTpos;

	for( ln = 1; ln <= m_LineCount; ln++ ) {
		n = ((ln-1) * m_DclmCount);

		for( i = 0; (p+i)->dpno != -1; i++ ) {
			idx = (p+i)->dpindex + n;
	
			if( ! (p+i)->dp_enable ) {
				(m_pDiag + (p+i)->dpno)->ModifyItem( idx, DIAG_MDFY_READONLY );
			}
			else {
				(m_pDiag + (p+i)->dpno)->ModifyItem( idx, DIAG_MDFY_EDIT );
			}
			// 入力レングス設定
			if( (p+i)->dp_pn == FD_DBMN_PN || (p+i)->dp_pn == FD_CBMN_PN ) {
				(m_pDiag + (p+i)->dpno)->GetAttribute( idx, (LPUNKNOWN)&da );
				da.attr_inpmax = m_BmonLNG;
				(m_pDiag + (p+i)->dpno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );
			}
			else if( (p+i)->dp_pn == FD_DKOJI_PN || (p+i)->dp_pn == FD_CKOJI_PN ) {
				(m_pDiag + (p+i)->dpno)->GetAttribute( idx, (LPUNKNOWN)&da );
				da.attr_inpmax = m_KojiLNG;
				(m_pDiag + (p+i)->dpno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );
			}
		}
	}
}


//--------------------------------------------------
//	Diag の 仕訳データ部の部門・工事の設定
//	BOOL bTanBmn	単一部門入力か？
//--------------------------------------------------
void CDnpBaseDlg::diag_tanbmninz( BOOL bTanBmn )
{
	DNP_POS* p;
	int ln, n, i, idx;
	DIAGRAM_ATTRIBUTE da;

	// 仕訳データ部
	p = m_pDTpos;

	for( ln = 1; ln <= m_LineCount; ln++ ) {
		n = ((ln-1) * m_DclmCount);

		for( i = 0; (p+i)->dpno != -1; i++ ) {
			idx = (p+i)->dpindex + n;
	
			// 入力レングス設定
			if( (p+i)->dp_pn == FD_DBMN_PN || (p+i)->dp_pn == FD_CBMN_PN ) {
				// 入力可 or 不可

				if( (p+i)->dp_enable ) {
					(m_pDiag + (p+i)->dpno)->ModifyItem( idx, bTanBmn ? DIAG_MDFY_READONLY : DIAG_MDFY_EDIT );
				}
				else {
					(m_pDiag + (p+i)->dpno)->ModifyItem( idx, DIAG_MDFY_READONLY );
				}

				(m_pDiag + (p+i)->dpno)->GetAttribute( idx, (LPUNKNOWN)&da );
				da.attr_inpmax = bTanBmn ? 0 : m_BmonLNG;
				(m_pDiag + (p+i)->dpno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );
			}
			else if( (p+i)->dp_pn == FD_DKOJI_PN || (p+i)->dp_pn == FD_CKOJI_PN ) {
				// 入力可 or 不可
				if( (p+i)->dp_enable ) {
					(m_pDiag + (p+i)->dpno)->ModifyItem( idx, bTanBmn ? DIAG_MDFY_READONLY : DIAG_MDFY_EDIT );
				}
				else {
					(m_pDiag + (p+i)->dpno)->ModifyItem( idx, DIAG_MDFY_READONLY );
				}

				(m_pDiag + (p+i)->dpno)->GetAttribute( idx, (LPUNKNOWN)&da );
				da.attr_inpmax = bTanBmn ? 0 : m_KojiLNG;
				(m_pDiag + (p+i)->dpno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );
			}
		}
	}
}


//
// 入力不可 イニシャライズ
//
void CDnpBaseDlg::diag_disableinz()
{
	DNP_POS* p;
	int ln, n, i, idx;
	DIAGRAM_ATTRIBUTE da;

	// 仕訳データ部
	p = m_pDTpos;

	for( ln = 1; ln <= m_LineCount; ln++ ) {
		n = ((ln-1) * m_DclmCount);

		for( i = 0; (p+i)->dpno != -1; i++ ) {
			idx = (p+i)->dpindex + n;

			if( (p+i)->dp_pn == FD_HJIYU_PN ) {
			//	if( IsOKGyosyu( Voln1->apno, SW_SOCIAL|SW_SYUKYO)/* == 0x52*/ ) {	// 社会福祉用
				if( m_pBaseCls->m_DI.IsHjiyuItemGyosyu() == 0 ) {
					// 入力不可
					if( m_bHjiyuEna ) {
						(m_pDiag + (p+i)->dpno)->ModifyItem( idx, DIAG_MDFY_READONLY );
						m_bHjiyuEna = FALSE;
						(m_pDiag + (p+i)->dpno)->GetAttribute( idx, (LPUNKNOWN)&da );
						da.attr_inpmax = 0;
						(m_pDiag + (p+i)->dpno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );
					}
				}
				else {
					if( ! m_bHjiyuEna ) {
						(m_pDiag + (p+i)->dpno)->ModifyItem( idx, DIAG_MDFY_EDIT );
						m_bHjiyuEna = TRUE;
					}
				}
			}
			else if( (p+i)->dp_pn == FD_DSYOG_PN || (p+i)->dp_pn == FD_CSYOG_PN ) {
				if (!m_pBaseCls->IsFuriSyogBrn()) {
					// 入力不可
					(m_pDiag + (p+i)->dpno)->ModifyItem( idx, DIAG_MDFY_READONLY );
					(m_pDiag + (p+i)->dpno)->GetAttribute( idx, (LPUNKNOWN)&da );
					da.attr_inpmax = 0;
					(m_pDiag + (p+i)->dpno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );
				}
			}
			else if( (p+i)->dp_pn == FD_DBZEI_PN || (p+i)->dp_pn == FD_CRZEI_PN ) {
				if( m_pBaseCls->IsZeigkMdfy() || m_pBaseCls->m_DI.IsMasterType(MST_SYZ_INV) ) { //インボイスは外税で免税事業者等からの課税仕入 は入力可能) {
					// 入力可
					(m_pDiag + (p+i)->dpno)->ModifyItem( idx, DIAG_MDFY_EDIT );
				}
				else {
					// 入力不可
					(m_pDiag + (p+i)->dpno)->ModifyItem( idx, DIAG_MDFY_READONLY );
				}
			}
		}
	}
}


//
// 諸口枝番 設定
//
void CDnpBaseDlg::diag_sknbrn()
{
	DNP_POS* p;
	int ln, n, i, idx;
	DIAGRAM_ATTRIBUTE da;
	int dgno = -1;

	// 仕訳データ部
	p = m_pDTpos;

	for( ln = 1; ln <= m_LineCount; ln++ ) {
		n = ((ln-1) * m_DclmCount);

		for( i = 0; (p+i)->dpno != -1; i++ ) {
			idx = (p+i)->dpindex + n;
			
			if( (p+i)->dp_pn == FD_DSYOG_PN || (p+i)->dp_pn == FD_CSYOG_PN ) {
				if( m_pBaseCls->IsFuriSyogBrn() ) {
					// 入力可
					(m_pDiag + (p+i)->dpno)->ModifyItem( idx, DIAG_MDFY_EDIT );
					(m_pDiag + (p+i)->dpno)->GetAttribute( idx, (LPUNKNOWN)&da );
					da.attr_inpmax = 2;
					(m_pDiag + (p+i)->dpno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );
				}
				else {
					// 入力不可
					if( dgno != -1 && (p+i)->dpno != -1 ) {
						dgno = (p+i)->dpno;
					}
					(m_pDiag + (p+i)->dpno)->DataClear( idx, TRUE );
					(m_pDiag + (p+i)->dpno)->ModifyItem( idx, DIAG_MDFY_READONLY );
					(m_pDiag + (p+i)->dpno)->GetAttribute( idx, (LPUNKNOWN)&da );
					da.attr_inpmax = 0;
					(m_pDiag + (p+i)->dpno)->SetAttribute( idx, (LPUNKNOWN)&da, FALSE );
				}
			}
		}
	}

	if( dgno != -1 ) {
		(m_pDiag + dgno)->Refresh();
	}
}


// 摘要入力用 DBEDT
// 工事
// 手形番号
// 部門 [01.30 /13]
void CDnpBaseDlg::SetFocusDBEDT( UINT id )
{
	DBGFUNCTION;
	if (!m_pBaseCls->m_PRO_OK)	return;

	if( id == IDC_ICSDBEDT8CTRL1 )
		FocusTkyColor( m_pBaseCls->get_nowln(), TRUE );
}

void CDnpBaseDlg::KillFocusDBEDT( UINT id, long inplen )
{
	DBGFUNCTION;
	if (!m_pBaseCls->m_PRO_OK)	return;

//	TRACE( ">>>CDnpBaseDlg::KillFocusDBEDT\n" );
	switch( id ) {
	case IDC_ICSDBEDT8CTRL1:
	case IDC_ICSDBEDT_KASITKY:
		m_pBaseCls->TerminationTekiyo( id, 0, inplen, 0 );
		break;
	case IDC_ICSDBEDT_KOJI:
		m_pBaseCls->TerminationKojiDBedit( id, 0, inplen, 0 );
		break;
	case IDC_ICSDBEDT_TGNUM:
		m_pBaseCls->TerminationTgNumDBedit( id, 0, inplen, 0 );
		break;

	case IDC_ICSDBEDT_BMON:
		m_pBaseCls->TerminationBmonDBedit( id, 0, inplen, 0 );
		break;
	case IDC_ICSDBEDT_SYOBRN:
		m_pBaseCls->TerminationSyoBrnDBedit(id, 0, inplen, 0);
		break;
	case IDC_ICSDBEDT_SYONUM:
		m_pBaseCls->TerminationSyoNumDBedit(id, 0, inplen, 0);
		break;
	case IDC_ICSDBEDT_KMINP:
		m_pBaseCls->TerminationKmkinp(id, 0, inplen, 0);
		break;
	}
}

void CDnpBaseDlg::TerminationDBEDT( UINT id, long nChar, long inplen, long kst )
{
	DBGFUNCTION;
	if (!m_pBaseCls->m_PRO_OK)	return;

	switch( id ) {
	case IDC_ICSDBEDT8CTRL1:
	case IDC_ICSDBEDT_KASITKY:
		m_pBaseCls->TerminationTekiyo( id, nChar, inplen, kst );
		break;
	case IDC_ICSDBEDT_KOJI:
		m_pBaseCls->TerminationKojiDBedit( id, nChar, inplen, kst );
		break;
	case IDC_ICSDBEDT_TGNUM:
		m_pBaseCls->TerminationTgNumDBedit( id, nChar, inplen, kst );
		break;

	case IDC_ICSDBEDT_BMON:
		m_pBaseCls->TerminationBmonDBedit( id, nChar, inplen, kst );
		break;
	case IDC_ICSDBEDT_SYOBRN:
		m_pBaseCls->TerminationSyoBrnDBedit(id, nChar, inplen, kst);
		break;
	case IDC_ICSDBEDT_SYONUM:
		m_pBaseCls->TerminationSyoNumDBedit(id, nChar, inplen, kst);
		break;
	case IDC_ICSDBEDT_KMINP:
		m_pBaseCls->TerminationKmkinp(id, nChar, inplen, kst);
		break;
	}
}

void CDnpBaseDlg::ImeEndDBEDT( UINT id, long nChar, LPCTSTR string, LPCTSTR ystring )
{
	DBGFUNCTION;
	if (!m_pBaseCls->m_PRO_OK)	return;

	switch( id ) {
	case IDC_ICSDBEDT8CTRL1:
	case IDC_ICSDBEDT_KASITKY:
	case IDC_ICSDBEDT_KOJI:
	case IDC_ICSDBEDT_BMON:
	case IDC_ICSDBEDT_TGNUM:
	case IDC_ICSDBEDT_SYONUM:
	case IDC_ICSDBEDT_KMINP:
		m_pBaseCls->ImeEndJob( id, nChar, string, ystring );
		break;
	}
}

void CDnpBaseDlg::KanaTermDBEDT(UINT id, LPCTSTR Kana)
{
	switch( id ) {
	case IDC_ICSDBEDT_KMINP:
		m_pBaseCls->ToViewKanaTerm(Kana);
		break;
	}
}
////////////////////////////////////////////////////////////////////////////////////
// イメージ関係
void CDnpBaseDlg::SetImage( int mode, int ln, IMREC* imp )
{
	UINT	id;
	CWnd*	pWnd;

	if( !mode ) {
		id	=	IDC_IMAGE1 + (ln-1);
		pWnd	=	GetDlgItem( id );
		pWnd->SetWindowPos( NULL, 0,0,0,0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER );
		if( imp ) {
			((ImageStatic*)pWnd)->SetImage( imp->dotxs, imp->dotys, imp->img_l, imp->imdata );
		}
		else {
			((ImageStatic*)pWnd)->SetImage( 0, 0, 0, NULL );
		}
	}
	else {	// 貸方イメージ
		id	=	IDC_IMAGE8;
		pWnd	=	GetDlgItem( id );
		if( imp ) {
			((ImageStatic*)pWnd)->SetImage( imp->dotxs, imp->dotys, imp->img_l, imp->imdata );
		}
		else {
			((ImageStatic*)pWnd)->SetImage( 0, 0, 0, NULL );
		}
//		((ImageStatic*)pWnd)->SetBackColor( TRUE );
	}
}


void CDnpBaseDlg::OffImage( int ln, int sgn )
{
	CWnd*	pWnd;
	if( !sgn )	pWnd	=	GetDlgItem( IDC_IMAGE1 + (ln-1) );
	else		pWnd	=	GetDlgItem( IDC_IMAGE8 );

	pWnd->SetWindowPos( NULL, 0,0,0,0, SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER );
}


void CDnpBaseDlg::SetFocusToImage( int ln, int sgn )
{
	CWnd*	pWnd;
	if( !sgn )	pWnd	=	GetDlgItem( IDC_IMAGE1 + (ln-1) );
	else		pWnd	=	GetDlgItem( IDC_IMAGE8 );

//	GotoDlgCtrl( pWnd );
	pWnd->SetFocus();
}

//--------------------------------------------------------------
//	イメージを描画するかのサインをセット
//	int	ln		イメージの行数
//	int	mode	0 = イメージ再描画, 
//				1 = ImageStatic::OnPaint() の処理をさせない
//--------------------------------------------------------------
void CDnpBaseDlg::ImagePaintSw( int ln, int mode )
{
	ImageStatic*	pWnd;
	UINT	id	=	IDC_IMAGE1 + (ln-1);
	pWnd	=	(ImageStatic*)GetDlgItem( id );

	pWnd->ImagePaintSw( mode );
}

void CDnpBaseDlg::ImageBackColor( int ln, BOOL sgn )
{
	CWnd*	pOrg	=	GetDlgItem( IDC_IMAGE1 + (ln-1) );
	((ImageStatic*)pOrg)->SetBackColor( sgn );
}

void CDnpBaseDlg::ImageBackTrans( int ln, BOOL sgn, COLORREF transback )
{
	CWnd*	pOrg	=	GetDlgItem( IDC_IMAGE1 + (ln-1) );
	((ImageStatic*)pOrg)->SetBackTrans( sgn, transback );
}

void CDnpBaseDlg::KasiImageDisplay( int ln, BOOL sgn )
{
	CWnd*	pImg	=	GetDlgItem( IDC_IMAGE8 );
	CWnd*	pOrg	=	GetDlgItem( IDC_IMAGE1 + (ln-1) );
	CRect	rect, rectOrg, rectTky;

	pOrg->GetWindowRect( rectOrg );
	GetRect( ln, FD_TKY_PN, rectTky );

	rect	=	rectOrg;
	rect.top	=	rectTky.bottom;
	rect.bottom	=	rect.top + rectOrg.Height();
	ScreenToClient( &rect );

	if( sgn ) {
		pImg->SetWindowPos( &wndTopMost, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW | SWP_NOZORDER );
		((ImageStatic*)pImg)->SetBackColor( TRUE );
		((ImageStatic*)pOrg)->SetBackColor( TRUE );
	}
	else {
		((ImageStatic*)pOrg)->SetBackColor( FALSE );
		((ImageStatic*)pImg)->ClearImage();
		pImg->SetWindowPos( &wndTopMost, 0,0,0,0, SWP_HIDEWINDOW | SWP_NOZORDER );
	}
}


void CDnpBaseDlg::KasiTekiyoDisplay( BOOL bShow )
{
	UINT nFlags;
	nFlags = SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER;

	nFlags |= bShow ? SWP_SHOWWINDOW : SWP_HIDEWINDOW ;

	GetDlgItem( IDC_ICSDBEDT_KASITKY )->SetWindowPos( NULL, 0,0,0,0, nFlags );
}


// 仕訳データ色の 変更
//		char* syz_chg	[0] 借方消費税変更サイン, [1] 貸方消費税変更サイン
//
void CDnpBaseDlg::SetDataLineColor( int ln, COLORREF back, COLORREF txt, char* syz_chg )
{
	DIAGRAM_ATTRIBUTE da;

	//仕訳データ
	DNP_POS* pos;
	pos = m_pDTpos;
	int dataidx;
	dataidx = pos->dpindex + ((ln-1)*m_DclmCount);

	for( ; ; pos++ ) {
		if( pos->dpno < 0 ) {
			break;
		}
		//消費税変更した仕訳は先に色を変更
		if( syz_chg != NULL ) {
			if( pos->dp_pn == FD_DSKBN_PN && syz_chg[0] ) {
				continue;
			}
			if( pos->dp_pn == FD_CSKBN_PN && syz_chg[1] ) {
				continue;
			}
			//消費税額変更(緑)
			if (pos->dp_pn == FD_DBZEI_PN && (syz_chg[2] == 3 || syz_chg[2] == 4)) {
				continue;
			}
			if (pos->dp_pn == FD_CRZEI_PN && (syz_chg[3] == 3 || syz_chg[3] == 4)) {
				continue;
			}
		}

		// 色の変更
		dataidx = pos->dpindex + ((ln-1)*m_DclmCount);

		(m_pDiag + pos->dpno)->GetAttribute( dataidx, (LPUNKNOWN)&da );

		da.attr_bcolor	= back;
		da.attr_frcolor	= back;
//		da.attr_fcolor	= txt;

		(m_pDiag + pos->dpno)->SetAttribute( dataidx, (LPUNKNOWN)&da, FALSE );
	}
	// 再描画
	(m_pDiag + 1)->Refresh();

	// イメージスタティック
	CHusenStatic*	pHusen;

	if( m_pBaseCls->m_DI.CheckConfirmBkColor( back ) )
		ImageBackTrans( ln, TRUE, back );
	else
		ImageBackTrans( ln, FALSE, RGB_WHITE );

	// 付箋スタティック
	pHusen = (CHusenStatic*)GetDlgItem( IDC_HUSEN1 + (ln-1) );
	pHusen->SetBackColor( back );

}


// 確定仕訳・転送済み仕訳かどうか？
#ifndef OLD_CLOSE
void CDnpBaseDlg::TransLine( int dt_ln, int ln, char* syz_chg )
#else
void CDnpBaseDlg::TransLine( CDBipDataRec* pdt, int ln )
#endif
{
	COLORREF	backcol;
	DIAGRAM_ATTRIBUTE da;

#ifdef CLOSE // DnpInView でセットするように変更
	int			sgn, st;
	if( (st = m_DI.tline_chk( pdt )) < 0 ) {

		if( Confirm_Mode() == 0 ) {			//確定マスター修正不可
			if( st == -1 ) {	// 確定仕訳
				sgn = 2;
				backcol = RGB_KAKUT_DATA;
			}
			else {				// 転送仕訳
				sgn = 3;
				backcol = RGB_TENSO_DATA;
			}
		}
		else {
			// 確定マスターでなく、転送済み仕訳の修正可
			sgn = 1;
			backcol = RGB_TENSOMDFY_DATA;
		}

	}
	else {
		backcol = RGB_WHITE;
		sgn = 0;
	}
	int pn;

	for( pn = FD_DBVAL_PN; pn < FD_MAX_PN; pn++ ) {
		m_pBaseCls->_getCND( ln, pn )->EDT_sg = sgn;
	}
#endif

//	int dt_ln = m_pBaseCls->get_dataline( ln );

	switch( m_pBaseCls->_getCND( dt_ln, FD_DBVAL_PN )->EDT_sg ) {
	case 1:	backcol = RGB_TENSOMDFY_DATA;	// 確定マスターでなく、転送済み仕訳の修正可
			break;
	case 2:
	case 10:	//参照モード
			backcol = RGB_KAKUT_DATA;	// 確定仕訳
			break;
	case 3:	backcol = RGB_TENSO_DATA;	// 転送仕訳
			break;
	default:
		backcol = RGB_WHITE;
		break;
	}

	SetDataLineColor( ln, backcol, RGB_BLACK, syz_chg );
}


// 現在行 色変更
void CDnpBaseDlg::FocusLine( int ln, BOOL bFocus )
{
#ifdef CLOSE
	DIAGRAM_ATTRIBUTE da;

	//仕訳データ
	DNP_POS* pos;
	pos = m_pDTpos;
	int dataidx;
	dataidx = pos->dpindex + ((ln-1)*m_DclmCount);

//	矩形取得
	CRect rect;
	GetLineRect( ln, rect ); 
	ScreenToClient( rect );
	rect.DeflateRect( 1,1 );

	GetDlgItem( IDC_ST_FOCUS )->MoveWindow( rect );
#endif
/*
	for( ; ; pos++ ) {
		if( pos->dpno < 0 ) {
			break;
		}
		// 色の変更
		dataidx = pos->dpindex + ((ln-1)*m_DclmCount);

		(m_pDiag + pos->dpno)->GetAttribute( dataidx, (LPUNKNOWN)&da );

		da.attr_bcolor = backcol;

		(m_pDiag + pos->dpno)->SetAttribute( dataidx, (LPUNKNOWN)&da, FALSE );
	}
	// 再描画
	(m_pDiag + 1)->Refresh();
*/
}


//
void CDnpBaseDlg::FocusTkyColor( int ln, BOOL bFocus )
{
	//仕訳データ
	DNP_POS* pos;
	pos = m_pDTpos;
	int dataidx, sgn;
	DIAGRAM_ATTRIBUTE da;
	COLORREF back;

	// 基本位置取得
	sgn = 0;
	for( ; ; pos++ ) {
		if( pos->dpno < 0 ) {
			break;
		}

		if( pos->dp_pn == FD_TKY_PN ) {
			sgn = 1;
			break;
		}
	}

//MyTrace( "FocusTkyColor ln %d, m_TkyLine = %d, Focus %d, \n", ln, m_TkyLine, bFocus );

	if( ! sgn || ! m_pBaseCls->m_PRO_OK )	return;

	// 色を元に戻す
	if( m_TkyLine != -1 ) {
		int dt_ln = m_pBaseCls->get_dataline(m_TkyLine);
		// 確定仕訳か？
		if( m_pBaseCls->_getCND(dt_ln, FD_TKY_PN )->EDT_sg > 0 )
			back = m_pBaseCls->GetConfirmBackColor( m_pBaseCls->_getCND(dt_ln, FD_TKY_PN )->EDT_sg );
		else
			back = RGB_WHITE;

		dataidx = pos->dpindex + ((m_TkyLine-1)*m_DclmCount);

		(m_pDiag + pos->dpno)->GetAttribute( dataidx, (LPUNKNOWN)&da );

		da.attr_bcolor	= back;
		da.attr_frcolor	= back;

		(m_pDiag + pos->dpno)->SetAttribute( dataidx, (LPUNKNOWN)&da, FALSE );

		m_TkyLine = -1;
	}

	if( ln <= 0 ) {
		(m_pDiag + pos->dpno)->Refresh();
		return;
	}

	if( ! bFocus ) {
		int dt_ln = m_pBaseCls->get_dataline(ln);
		// 確定仕訳か？
		if( m_pBaseCls->_getCND(dt_ln, FD_TKY_PN )->EDT_sg > 0 )
			back = m_pBaseCls->GetConfirmBackColor( m_pBaseCls->_getCND(dt_ln, FD_TKY_PN )->EDT_sg );
		else
			back = RGB_WHITE;

		dataidx = pos->dpindex + ((ln-1)*m_DclmCount);

		(m_pDiag + pos->dpno)->GetAttribute( dataidx, (LPUNKNOWN)&da );

		da.attr_bcolor	= back;
		da.attr_frcolor	= back;

		(m_pDiag + pos->dpno)->SetAttribute( dataidx, (LPUNKNOWN)&da, TRUE );
	}
	else {
		if( ln != -1 ) {
			m_TkyLine = ln;

			dataidx = pos->dpindex + ((ln-1)*m_DclmCount);

			(m_pDiag + pos->dpno)->GetAttribute( dataidx, (LPUNKNOWN)&da );

			da.attr_bcolor	= SELECT_BACK_COL;
			da.attr_frcolor	= SELECT_BACK_COL;

			(m_pDiag + pos->dpno)->SetAttribute( dataidx, (LPUNKNOWN)&da, TRUE );
		}
	}
}


#ifdef OLD_FUNC	////////////////////////////////////////////////

// 仕訳行の削除
void CDnpBaseDlg::DeleteLine( int ln, BOOL sw )
{
	CWnd*	pWnd;

	ImageStatic*	pImg;
	pImg	=	(ImageStatic*)GetDlgItem( IDC_IMAGE1 + (ln-1) );

	if( sw == FALSE ) {
		m_pBaseCls->SetDelSgn( ln, 0 );
		pImg->DeleteLine( 0 );

		_delete_line( ln, sw );
	}
	else {
		m_pBaseCls->SetDelSgn( ln, 1 );
		pImg->DeleteLine( 1 );

		_delete_line( ln, sw );
	}
}

//	BOOL sw		FALSE	取消線消去
//				TRUE	　〃　表示
//
void CDnpBaseDlg::_delete_line( int ln, BOOL sw ) 
{
	int idx;
	long	flg;

	idx = del_idx( ln );

	flg = sw ? DIAG_MDFY_OPAQUE : DIAG_MDFY_TRANSPARENT;

	(m_pDiag+1)->ModifyItem( idx, flg );
	(m_pDiag+1)->ModifyItem( (idx+1), flg );
}

#endif /////////////////////////////////////////////////


void CDnpBaseDlg::OnSize(UINT nType, int cx, int cy)
{
	ICSDialog::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( m_pDiag == NULL )	return;

	ControlPos();
}



LRESULT CDnpBaseDlg::OnImageStaticFocus( WPARAM wParam, LPARAM lParam )
{
//	if( flgNOP )	return	0;
//	if( flgFork )	return	0;
	if( IsReadOnly( wParam ) )	return	0;
	if( IsDelete( wParam ) )	return	0;
	
	//KILL FOCUS
	if( lParam == -1 ) {
		if( wParam == IDC_IMAGE8 ) {
			flgKasiImage	=	FALSE;
		}
		else {
		}
	}
	//SET FOCUS
	else {
		if( wParam == IDC_IMAGE8 ) {
			flgKasiImage	=	TRUE;
			int ln = m_pBaseCls->get_nowln();

			((ImageStatic*)GetDlgItem( IDC_IMAGE1 + (ln-1) ))->SetBackColor( TRUE );
			((ImageStatic*)GetDlgItem( wParam ))->SetBackColor( TRUE );
			GetDlgItem( wParam )->SetWindowPos( NULL, 0,0,0,0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER );
		}
		else {
			flgKasiImage	=	FALSE;

//			((ImageStatic*)GetDlgItem( wParam ))->SetBackColor( TRUE );

			m_pBaseCls->ToViewImageStaticFocus( wParam );
		}
	}

	return	1;
}

LRESULT CDnpBaseDlg::OnImageStaticKey( WPARAM wParam, LPARAM lParam )
{
//	if( flgNOP )	return	0;
//	if( flgFork )	return	0;
	if( IsReadOnly( wParam ) )	return	0;
	if( IsDelete( wParam ) )	return	0;
	
	UINT	id	=	wParam;
	UINT	nChar	=	lParam;

	switch( nChar ) {
		case	VK_F8:
		case	VK_F7:
		case	VK_F6:
		case	VK_F5:
//		case	VK_F3:
		case	VK_F2:
		case	VK_F1:
		case	VK_NEXT:
		case	VK_PRIOR:
			return	m_pBaseCls->ToViewAccelerator( id, nChar );
			break;

		case	VK_F4:
			return	m_pBaseCls->ToViewAccelerator( id, nChar );
			break;
		case	VK_END:
#ifdef OLD_KEEP_CLOSE
			if( IsJob() == JOB_SCAN ) {
				//貸方イメージでここへきた時に表示が残るため
				GotoDlgCtrl( GetDlgItem( IDC_ICSINP_DENP ) );

				return	m_pBaseCls->ToViewAccelerator( id, 20 );
			}
			else {
				return	m_pBaseCls->ToViewAccelerator( id, nChar );
			}
#endif
			return	m_pBaseCls->ToViewAccelerator( id, nChar );

			break;
	}

	m_pBaseCls->ToViewImageStaticKey( wParam, lParam );

	return	1;
}


BOOL CDnpBaseDlg::IsReadOnly( UINT id )
{
	return m_pBaseCls->IsReadOnly( id );
}


BOOL CDnpBaseDlg::IsDelete( UINT id )
{
	return m_pBaseCls->IsDelete( id );
}


// ｳｨﾝﾄﾞｳ 再描画
void CDnpBaseDlg::UpdateSubWindow( int ln )
{
	int idx = ln-1;

	if( idx >= 0 ) {
		GetDlgItem( IDC_HUSEN1+idx )->Invalidate();
		GetDlgItem( IDC_HUSEN1+idx )->UpdateWindow();
	}
}

// 現在表示ビューが 伝票の画面か？
BOOL CDnpBaseDlg::IsDenpView()
{
//	int vwsgn;
//	vwsgn = ((CMainFrame*)GetDBDinpMainFrame())->NowView();
//	return (vwsgn == DNP_VIEW);

	return TRUE;
}


// 修正 文言
void CDnpBaseDlg::ChangeJob( int job )
{
}

// 文字列をICSDisp にセット
void CDnpBaseDlg::DispScanTitle( CICSDisp* disp )
{
	disp->ClearDisp();
	disp->ColmDisp( 0, m_ScanTitle );
}

//
//	他のウィンドウからの コントロールへのカーソル移動
//
void CDnpBaseDlg::cursor_set( UINT id )
{
	GotoDlgCtrl( GetDlgItem( id ) );
}


// スクロールバーセット
void CDnpBaseDlg::set_scrollbar( CScrollBar* pscroll, int npos, int dmax, int pmax )
{
	SCROLLINFO si;
	CScrollBar* pBar;

	if( m_pScBar == NULL ) {
		m_pScBar = pscroll;
	}

	if( pscroll == NULL ) {
		pBar = m_pScBar;
	}
	else {
		pBar = pscroll;
	}

	ASSERT( pBar != NULL );
	ASSERT( ::IsWindow( pBar->m_hWnd ) != FALSE );

	if( npos == -1 ) {
		si.cbSize = sizeof( SCROLLINFO );
		si.fMask = SIF_ALL;
		si.nMin = 0;
		si.nMax = dmax- 1;
		si.nPage = pmax;
		si.nPos = 0;
		si.nTrackPos = 0;
		pBar->SetScrollInfo(&si, TRUE);	
	}
	else {
		pBar->GetScrollInfo(&si);
		si.nPos = npos;
		pBar->SetScrollInfo(&si, TRUE);	
	}
}


// データ部1行の使用アイテムの入力状態
BOOL CDnpBaseDlg::dataline_input( int ln )
{
	DNP_POS* p;
	struct _DNPLINE_CND* pcnd;

	// 仕訳データ部
	p = m_pDTpos;

	BOOL bRet = FALSE;

	for( int i = 0; (p+i)->dpno != -1; i++ ) {
		// 入力可能項目のみ
		if( ! (p+i)->dp_enable )
			continue;

	//	pdata = m_pBaseCls->_getDATA( ln, (p+i)->dp_pn );
		pcnd = m_pBaseCls->_getCND( ln, (p+i)->dp_pn );

		if( pcnd->INP_sg ) {
			bRet = TRUE;
			break;
		}
	}

	return bRet;
}


// データ部1行の選択ポジションか？
int CDnpBaseDlg::IsLine1Selpos( POINT pt )
{
	DNP_POS* p;

	// 仕訳データ部
	p = m_pDTpos;

	int rval = 0;
	int	pn = -1;

	for( int i = 0; (p+i)->dpno != -1; i++ ) {
		// 入力可能項目のみ
		if( ! (p+i)->dp_enable )
			continue;

		pn = (p+i)->dp_pn;
		break;
	}

	if( pn < 0 )	return FALSE;

	CRect rect;

	for( int i = 0; i < m_LineCount; i++ ) {
		GetRect( i+1, pn, rect );

		if( rect.PtInRect( pt ) ) {
			rval = (i+1);
			break;
		}
	}

	return rval;
}

//--------------------------------------------
//	ダイアグラム再描画
//--------------------------------------------
int CDnpBaseDlg::DiagRefresh()
{
	DNP_POS* p;

	// 仕訳データ部
	p = m_pDTpos;

	this->Invalidate();
	this->UpdateWindow();


	for( int i = 0; (p+i)->dpno != -1; i++ ) {
		// 入力可能項目のみ
		(m_pDiag + (p+i)->dpno)->Refresh();
		break;
	}

	return 0;
}




void CDnpBaseDlg::DiagDeleteInput()
{
	int dno, idx;

	dno = m_dgno;
	idx = m_index;

//	(m_pDiag + dno)->DeleteInput();

	if( dno != -1 ) {
		(m_pDiag + dno)->Refresh();
	}
}


void CDnpBaseDlg::FuriSyogDataColor( int dt_ln, int ln )
{
	COLORREF	backcol = 0;

	// (1)確定マスターでなく、転送済み仕訳の修正可
	// (2)確定仕訳
	// (3)転送仕訳
	if( m_pBaseCls->_getCND( dt_ln, FD_DBVAL_PN )->EDT_sg <= 1 ) {
		backcol = RGB_GRAY;
	}

	DIAGRAM_ATTRIBUTE da;

	int dc_sw = -1;

	if( m_pBaseCls->FuriSyogPosition( dt_ln, FD_DEBT_PN ) )
		dc_sw = 0;
	else if( m_pBaseCls->FuriSyogPosition( dt_ln, FD_CRED_PN ) )
		dc_sw = 1;

	//諸口詰めないモードか 詰めない仕訳でない場合。
	if( dc_sw == -1 )
		return;

	//仕訳データ
	DNP_POS* pos;
	pos = m_pDTpos;
	int dataidx;
	dataidx = pos->dpindex + ((ln-1)*m_DclmCount);

	for( ; ; pos++ ) {
		if( pos->dpno < 0 ) {
			break;
		}

		if( dc_sw == 0 ) {
			if( ! IsDebtPosition( pos->dp_pn ) )
				continue;
		}
		else  {
			if( ! IsCredPosition( pos->dp_pn ) )
				continue;
		}

		// 色の変更
		dataidx = pos->dpindex + ((ln-1)*m_DclmCount);

		(m_pDiag + pos->dpno)->GetAttribute( dataidx, (LPUNKNOWN)&da );

		da.attr_bcolor	= backcol;
		da.attr_frcolor	= backcol;
//		da.attr_fcolor	= txt;

		(m_pDiag + pos->dpno)->SetAttribute( dataidx, (LPUNKNOWN)&da, FALSE );
	}
	// 再描画
	(m_pDiag + 1)->Refresh();

}


LRESULT CDnpBaseDlg::OnDBDinpMsg(WPARAM wp, LPARAM lp)
{
	if( wp == DWP_TKYSWKDLG ) { //KanaTerm時の自動仕訳
		m_pBaseCls->MakeAutoSiwake(&m_pBaseCls->m_autoTKrec);
	}

	return 1;
}

//科目カナ検索 設定更新
void CDnpBaseDlg::KanaSearchUpdate()
{
	CICSDBEDT *pDBedt;
	//科目カナ検索
	pDBedt = (CICSDBEDT *)GetDlgItem(IDC_ICSDBEDT_KMINP);
	pDBedt->SetKanaTerm(TRUE, m_pBaseCls->KanaSearchKeta(), ICSDBEDT_KTYPE_KANA);
}

