//

#include "stdafx.h"
#include "DBDinp.h"

#include "DBDinpRec.h"
#include "DataListCtrl.h"

#include "MainFrm.h"
#include "DBDinpDoc.h"

#include "DBDinpRec.h"
#include "DBDinpView.h"

#include "DinpSub.h"
#include "Input1.h"
#include "dataline.h"

#include "ScanView.h"
#include "ScanKeyView.h"
#include "DBAbookView.h"
#include "DnpInView.h"

#include "MstRw.h"
#include "external.h"
#include "RpkeyDlg.h"
#include "MsgDlg.h"

//#include "perform_chk.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDBDinpView で定義
extern int PROGRAM_OK;

extern struct _AUTOSEL *pAUTOSEL;

extern SCAN_ITEMEX	SC_Item;

extern int DnpInSetMouseHook();
extern int DnpInMouseUnhook();

//#undef FILETrace
//#define	FILETrace	MyTrace


//上手WLユーザで給与ライセンスがないか？
BOOL IsWLuserNotKyuyo()
{
	BOOL bRet = FALSE;
	int wl_kyu_lic = 0;

//AfxMessageBox("IsWLuserNotKyuyo test RETURN TRUE!!");
//return TRUE;

	// WL 顧問先ユーザー判別
	// int GET_JZUSER(
	//　0　以外　顧問先ユーザー
	if (GET_JZUSER()) {
		int	pkcode = atoi(GET_PKGTYPE());
		// 2014/08/20 減償ｄｂ対応
		//	if( (pkcode >= 200 && pkcode < 400 ) || pkcode == 610 ) {
		if ((pkcode >= 200 && pkcode < 400) || (pkcode >= 610 && pkcode < 620)) {
			// 給与上手くんｄｂWL
			wl_kyu_lic++;
		}
		// 2014/08/20 減償ｄｂ対応
		//	if( (pkcode >= 400 && pkcode < 600 ) || pkcode == 620 ) {
		if ((pkcode >= 400 && pkcode < 600) || (pkcode >= 620 && pkcode < 630)) {
			// 給与上手くんｄｂWLPROⅡ
			wl_kyu_lic++;
		}
		if (wl_kyu_lic == 0) {
			bRet = TRUE;
		}
	}
	return bRet;
}


//上手WLユーザで減価償却ライセンスがないか？
BOOL IsWLuserNotGenka()
{
	BOOL bRet = FALSE;
	int wl_genka_lic = 0;

	//AfxMessageBox("IsWLuserNotGenka test RETURN TRUE!!");
	//return TRUE;

	// WL 顧問先ユーザー判別
	// int GET_JZUSER(
	//　0　以外　顧問先ユーザー
	if (GET_JZUSER()) {
		int	pkcode = atoi(GET_PKGTYPE());

		switch (pkcode) {
		case 15: case 25: case 35:
		case 85: case 95:
		case 215: case 225: case 235:
		case 285: case 295:
		case 415: case 425:
		case 435: case 485: case 495:
		case 615: case 625: case 650:
			wl_genka_lic++;
			break;
		default:
			break;
		}
		if (wl_genka_lic == 0) {
			bRet = TRUE;
		}
	}
	return bRet;
}


// CMainFrame
IMPLEMENT_DYNAMIC(CMainFrame, ICSMainFrame)

BEGIN_MESSAGE_MAP(CMainFrame, ICSMainFrame)
	ON_MESSAGE(WM_CLOSE, OnMessageClose)
	ON_WM_CREATE()
	ON_MESSAGE(ICS_MAINFRAME_MESSAGE_CALL,OnCall)
	ON_MESSAGE(ICS_MAINFRAME_MESSAGE_JOBSEL,OnFromForkChain)
	ON_MESSAGE(ICS_MAINFRAME_MESSAGE_COSEL,OnFromParentCosel)

	ON_MESSAGE(WM_CHGTOOLBAR, OnChgToolBar)
	ON_MESSAGE(WM_BUNKI, OnBunki)

#ifdef _SLIP_ACCOUNTS_
	ON_MESSAGE(ICS_MAINFRAME_MESSAGE_MULTIPOS, OnMessageMultipos)
#endif

	ON_UPDATE_COMMAND_UI(ID_BUTTON_F5, &CMainFrame::OnUpdateButtonF5)
	ON_WM_ACTIVATE()
	ON_COMMAND(IDM_INDKMK, &CMainFrame::OnIndkmk)
	ON_UPDATE_COMMAND_UI(IDM_INDKMK, &CMainFrame::OnUpdateIndkmk)
	ON_UPDATE_COMMAND_UI(IDM_JGYKMK, &CMainFrame::OnUpdateJgykmk)
	ON_COMMAND(IDM_JGYKMK, &CMainFrame::OnJgykmk)

	ON_COMMAND(ID_BUTTON_SHINCYOKU, &CMainFrame::OnButtonShicyoku)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_SHINCYOKU, &CMainFrame::OnUpdateShicyoku)
	ON_COMMAND(IDM_HELP_DISP, &CMainFrame::OnHelpDisp)
	ON_MESSAGE(WM_ADDINV_MSG, &CMainFrame::OnWmAddInvMsg)

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ステータス ライン インジケータ
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame コンストラクション/デストラクション

CMainFrame::CMainFrame()
{
	SetCommandBarsID( IDR_DBDINPTYPE );
	// TODO: メンバ初期化コードをここに追加してください。

	m_NowBarType = -1;

#ifdef _DEBUG
	VERIFY( mon = new ICSMon( GetCurrentProcessId() ) );
    PCOM_SetInt( _T("CompanySin" ),  0 );
	_debug_SetDsn();	// DSNに登録
#endif

	m_inpview = m_skeyview = m_scanview = m_acbkview = m_dnpview = NULL;
	m_INPview_id = m_SKEYview_id = m_SCANview_id = m_ACBKview_id = m_DNPview_id = -1;
	m_nowvw_flg = NON_VIEW;
	m_prevw_flg = NON_VIEW;

	m_SKEYprev_flg = INP_VIEW;

	bTabClickChange = FALSE;
	bSysClose = FALSE;

	IsTkyOnly() = FALSE;
	IsSCANMDFY() = FALSE;
	m_ScmdfySeq	= 0;
	bReCOSEL = FALSE;
	m_bChgView = FALSE;

	m_SkeyKmkType	= 0;
	m_AutoSwkType	= 0;
	m_AcbkOption	= 0;
	m_AcbkKojiWd	= 0;
	m_bKyuyoDB	= FALSE;
	m_bGensyoDB	= FALSE;

	m_bBeginApp = FALSE;
	m_bMsgBox	= FALSE;
	m_bMsgBoxOnSize = FALSE;
	m_bIgnoreOnSize = FALSE;

	m_IsShincyoku = 0;
	m_FuriMode = FURI_DATABMON;
	bCloudEnd = FALSE;
	m_bReload = FALSE;
	m_bForkChain = FALSE;
	m_ActivateFlag = -1;

	m_bOnCloud = FALSE;
	m_bGenpyoScan = FALSE;

	m_MouseRouting = 2;
	m_AcbkKikanSwk = 0;
	m_DupliSwk = 0;
	m_bWIZMaster = FALSE;
	m_prog = NULL;
}

CMainFrame::~CMainFrame()
{
#ifdef _DEBUG
	delete mon;
	_debug_ResetDsn();	// DSNを削除
	SET_MonitorVersion( 1 );
#endif

//	_CrtDumpMemoryLeaks();
}

int CMainFrame::TabLButtonDown()
{
	ICSMainFrame::TabLButtonDown();

	int index;
	CWnd* pwnd;

	pwnd = GetLClickedChild( &index );

	if( m_nowvw_flg != index ) {
		if( m_nowvw_flg != m_SCANview_id ) {
			// スキャン画面は、CDBbaseView 派生でない
			if( ((CDBbaseView*)GetCView( m_nowvw_flg ))->TabLbuttonDownJob() != 0 )
				return 1;
		}
#ifdef _SLIP_ACCOUNTS_
		else if( m_nowvw_flg == m_SCANview_id ){
			if( ((CScanView*)GetCView( m_nowvw_flg ))->TabLbuttonDownJob() != 0 )
				return 1;
		}
#endif
	}

	TabClickChange() = TRUE;

	return	0;
}

int CMainFrame::TabRButtonDown()
{
	ICSMainFrame::TabRButtonDown();
	return	0;
}

//void CMainFrame::ChildFrameActive( CWnd* pChild )
//{
//	ICSMainFrame::ChildFrameActive( pChild );
//}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (ICSMainFrame::OnCreate(lpCreateStruct) == -1)
		return -1;

	CFont*	pFntText	=	new CFont;

	LOGFONT lf = {0};
	strcpy_s( lf.lfFaceName, sizeof lf.lfFaceName, "ＭＳ ゴシック" );
	lf.lfCharSet	= SHIFTJIS_CHARSET;
	lf.lfHeight	= 110;
	lf.lfWeight	= FW_BOLD;
	pFntText->CreatePointFontIndirect( &lf );
//	pFntText->CreatePointFont( 110, "ＭＳ ゴシック" );

	CXTPStatusBarPane*	pPane	=	 m_wndStatusBar.AddIndicator( ID_KENGYOMODE, 1 );
	pPane->SetBackgroundColor( RGB(255,255,255) );
	pPane->SetTextColor( RGB(0,0,255) );
	pPane->SetTextFont( pFntText );
	pPane->SetTextAlignment( DT_CENTER );
	pPane->SetTooltip( "科目選択欄 業種区分" );
	
	CString	NewText	=	"全業種";
	m_wndStatusBar.SetPaneText( 1, NewText );
	m_wndStatusBar.SetPaneWidth( 1, 80 );

	if( pFntText )	delete pFntText;	// '09.03.07

	return 0;
}

void	CMainFrame::RedrawStatusBar( CString& str )
{
	CXTPStatusBarPane*	pPane	=	 m_wndStatusBar.GetPane( 1 );
	
	COLORREF	color	=	RGB(255,255,255);
	if( str.IsEmpty() )	color = ::GetSysColor( COLOR_3DFACE );
	pPane->SetBackgroundColor( color );

	pPane->SetText( str );
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !ICSMainFrame::PreCreateWindow(cs) )
		return FALSE;

	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return TRUE;
}


// CMainFrame 診断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	ICSMainFrame::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	ICSMainFrame::Dump(dc);
}

#endif //_DEBUG


// LPCTSTR seq_path ... 03.15 /03
//
void CMainFrame::OnInitialize( LPCTSTR seq_path )
{
	//もう一つのビューをイニシャライズ
//	((CScanView*)m_scanview)->OnInitialize( seq_path );
}

//-------------------------------------------------------------------
//	スキャンビューにチェンジ
//	int sw			0: 初期検索データ表示, 1: 現在行ページを表示
//	int ofs			表示オフセット
//	int scan_sgn	0: 通常検索		1: 取消仕訳検索
//-------------------------------------------------------------------
void CMainFrame::ChgToScanView( int sw, int ofs, int scan_sgn )	// ofs == rno
{

	if( m_nowvw_flg != SCAN_VIEW )
	{
		m_prevw_flg = m_nowvw_flg;

		m_nowvw_flg = SCAN_VIEW;
		SetActiveChild( m_SCANview_id );
		DnpInMouseUnhook();

		((CScanView*)m_scanview)->UpdateDataLineDisp();

		if( !sw )
		{
			//初期データの検索 & 表示
			((CScanView*)m_scanview)->scan_init_rd( ofs, scan_sgn );
		}
		else if( sw == 1 )
		{
			((CScanView*)m_scanview)->scan_now_dsp( ofs );	//現在行ページの表示
		}
		else {
			//#* 03.15 /03
			// シーケンス番号テーブル フォーク
		}

		ChangeToolBar(3, scan_sgn);
	}

}

//検索一覧にフォーク
void CMainFrame::ChgToScanViewEx()
{

	//常に全仕訳
	scanitem_initEX( &SC_Item );
	pDBzm->IsDelScan() = FALSE;
	pDBzm->IsDupliScan() = FALSE;

	if( m_nowvw_flg != SCAN_VIEW )
	{
		if( m_nowvw_flg == INP_VIEW ) {
			//入力または行戻りの修正はモードをセーブしておく
			if( INP_mode == _APPEND || INP_mode == _MODEFY )
			{
				((CDBDinpView*)m_inpview)->_save_inpcondition();
				((CDBDinpView*)m_inpview)->SaveDinpPara( INP_mode );
			}
		}
		((CDBDinpView*)m_inpview)->HideDispImgDlg(TRUE);

		m_prevw_flg = m_nowvw_flg;

		m_nowvw_flg = SCAN_VIEW;
		SetActiveChild( m_SCANview_id );
		DnpInMouseUnhook();

		((CScanView*)m_scanview)->UpdateDataLineDisp();

		((CScanView*)m_scanview)->scan_init_seqrd( m_ScmdfySeq );

		ChangeToolBar(3, 0);
	}
	else {
		((CScanView*)m_scanview)->scan_init_seqrd( m_ScmdfySeq );
	}

}


void CMainFrame::DirectScanView()	// ofs == rno
{
	if( m_SCANview_id == -1 )
		return;

	if( m_nowvw_flg != SCAN_VIEW )
	{
		int prev = m_nowvw_flg;
		m_prevw_flg = m_nowvw_flg;
		m_nowvw_flg = SCAN_VIEW;
		PROGRAM_OK = FALSE;	// OnSize() の処理させない
		
	//	if( INP_mode != _KEYSCAN ) {
			if( prev == INP_VIEW ) {
				((CDBDinpView*)m_inpview)->TabClickChangeWork();
				((CDBDinpView*)m_inpview)->insert_clear();
			}
			else if( prev == SKEY_VIEW ) {
				((CScanKeyView*)m_skeyview)->get_skeyitem(0);
			}

			((CScanView*)m_scanview)->UpdateDataLineDisp();

			inpmode_set( _KEYSCAN );
			((CScanView*)m_scanview)->scan_init_rd( 0, 0 );	//初期データの検索 & 表示
	//	}
	//	else {
	//		((CScanView*)m_scanview)->scan_now_dsp( SCROLL_NL() - get_nowln() );	//現在行ページの表示
	//	}

		ChangeToolBar(3, 0);

		PROGRAM_OK = TRUE;	// OnSize() の処理させない
	}
}


//入力ビューにチェンジ
void CMainFrame::ChgToInpView( int sw, int seq, int now_seq )
{
//FILETrace( "ChgToInpView %d,%d,%d\n",  sw, seq, now_seq );

	//ﾄﾞｷｭﾒﾝﾄﾌﾟﾚｰﾄに登録されているﾋﾞｭｰにセット
	if( m_nowvw_flg != INP_VIEW )
	{
		PROGRAM_OK = FALSE;
		m_prevw_flg = m_nowvw_flg;
		m_nowvw_flg = INP_VIEW;

		((CDBDinpView*)m_inpview)->SetSelectWindow();

		if( sw == 1 ) {
			((CDBDinpView *)m_inpview)->ScanModeSign() = TRUE;
		}

		SetActiveChild( m_INPview_id );

		DnpInMouseUnhook();
		ChangeToolBar(0);

		switch( sw )
		{
		case 0:
			((CDBDinpView *)m_inpview)->dbedit_clear();	//06.21 /13  DBEdit Clear

			((CDBDinpView *)m_inpview)->to_inpmode();	//入力モードにチェンジ

			break;
		case 1:
			((CDBDinpView *)m_inpview)->dbedit_clear();	//05.29 /14
			((CDBDinpView *)m_inpview)->to_scanmode();	//検索修正モードに
			((CDBDinpView *)m_inpview)->ScanModeSign() = FALSE;
			break;
		case 2:
//			((CDBDinpView *)m_inpview)->to_scankey();	//検索項目入力に
			ASSERT( FALSE );
			break;
		case 3:
			((CDBDinpView *)m_inpview)->to_scanmode2( seq, now_seq );	//検索修正モード2に
			break;
		case 4:		// 入力モードに変更
			((CDBDinpView *)m_inpview)->to_appendmode();
			break;
		}
		PROGRAM_OK = TRUE;
	
	}

}


void CMainFrame::DirectInpView()
{
	if( m_INPview_id == -1 )
		return;

	if( m_nowvw_flg != INP_VIEW )
	{
		int prev = m_nowvw_flg;
		m_prevw_flg = m_nowvw_flg;
		m_nowvw_flg = INP_VIEW;

ASSERT( prev != -1 );

PROGRAM_OK = FALSE;

		((CDBDinpView*)m_inpview)->SetSelectWindow();
		((CScanKeyView*)m_skeyview)->ScanKeyClear();
#ifdef BLK_CLOSE
		// 06.13 /12
		((CDBDinpView*)m_inpview)->OnSizeJob();
#else
		((CDBDinpView*)m_inpview)->BlkMoveOnSizeJob(0,10);
#endif

		((CDBDinpView *)m_inpview)->insert_clear();
		((CDBDinpView *)m_inpview)->dbedit_clear();

		BOOL bAppendMode = FALSE;

		if( ((CDBDinpView *)m_inpview)->m_reCoDispType == DNP_VIEW || 
			((CDBDinpView *)m_inpview)->m_reCoDispType == ACBK_VIEW ) {
			if( prev == m_SKEYview_id ) {
				bAppendMode = TRUE;
			}
		}

		if( bAppendMode ) {
			((CDBDinpView*)m_inpview)->to_appendmode();
			((CDBDinpView*)m_inpview)->CtrlDispJob();
		}
		else {
			if( prev == m_SCANview_id ) {
				((CDBDinpView*)m_inpview)->to_inpmode();	//現在行ページの表示
			}
			else if( prev == m_SKEYview_id ) {
				((CDBDinpView*)m_inpview)->to_inpmode();	//現在行ページの表示
			}
			else {
				((CDBDinpView*)m_inpview)->to_appendmode();
				((CDBDinpView*)m_inpview)->CtrlDispJob();
			}
		}
		ChangeToolBar(0);

PROGRAM_OK = TRUE;

	}
}

// 検索項目画面 
void CMainFrame::ChgToSkeyView()
{
	//ﾄﾞｷｭﾒﾝﾄﾌﾟﾚｰﾄに登録されているﾋﾞｭｰにセット
	if( m_nowvw_flg != SKEY_VIEW )
	{
		PROGRAM_OK = FALSE;
		m_prevw_flg = m_nowvw_flg;
		m_SKEYprev_flg = m_prevw_flg;
		m_nowvw_flg = SKEY_VIEW;

		SetActiveChild( m_SKEYview_id );
		DnpInMouseUnhook();

		//一括修正項目 初期化
		CRpkeyDlg::keyline_inz();

		((CDBbaseView*)m_skeyview)->SetSelectWindow();
		((CDBbaseView*)m_skeyview)->UpdateSelectPar();
		((CScanKeyView*)m_skeyview)->to_scankey();

		ChangeToolBar(2);

		PROGRAM_OK = TRUE;

	}
}

void CMainFrame::DirectSkeyView()
{
	if( m_nowvw_flg != SKEY_VIEW )
	{
		PROGRAM_OK = FALSE;
		m_prevw_flg = m_nowvw_flg;
		m_SKEYprev_flg = m_prevw_flg;
		m_nowvw_flg = SKEY_VIEW;

		if( m_prevw_flg == INP_VIEW ) {
	//		((CDBDinpView*)m_inpview)->directscankey();
			((CDBDinpView*)m_inpview)->insert_clear();
		}

		((CDBbaseView*)m_skeyview)->SetSelectWindow();
		((CDBbaseView*)m_skeyview)->UpdateSelectPar();
		((CScanKeyView*)m_skeyview)->to_scankey();
		ChangeToolBar(2);

		PROGRAM_OK = TRUE;
	
	}
}

// 出納帳 へ ビュー変更
void CMainFrame::ChgToACBKView()
{
	if( m_nowvw_flg != ACBK_VIEW )
	{
		m_prevw_flg = m_nowvw_flg;
		m_nowvw_flg = ACBK_VIEW;

		SetActiveChild( m_ACBKview_id );
		DnpInMouseUnhook();

		((CDBbaseView*)m_acbkview)->SetSelectWindow();
		((CDBbaseView*)m_acbkview)->UpdateSelectPar();
		((CDBAbookView*)m_acbkview)->to_acbkmode();
		ChangeToolBar(5);
	}
}

// 出納帳 へ タブクリックによる直接変更
void CMainFrame::DirectACBKView()
{
	if( m_ACBKview_id == -1 )
		return;

	if( m_nowvw_flg != ACBK_VIEW )
	{
		PROGRAM_OK = FALSE;

		int prev = m_nowvw_flg;
		m_prevw_flg = m_nowvw_flg;
		m_nowvw_flg = ACBK_VIEW;
		
ASSERT( prev != -1 );
		// 入力画面の前回情報を覚える
	//	if( m_prevw_flg == INP_VIEW ) {
	//		((CDBDinpView*)m_inpview)->directacbook();
	//	}
		if( prev == INP_VIEW ) {
			((CDBDinpView*)m_inpview)->insert_clear();
		}
		((CScanKeyView*)m_skeyview)->ScanKeyClear();

		((CDBbaseView*)m_acbkview)->SetSelectWindow();
		((CDBbaseView*)m_acbkview)->UpdateSelectPar();
		((CDBAbookView*)m_acbkview)->to_acbkmode();
		ChangeToolBar(5);

		PROGRAM_OK = TRUE;

	}
}


//伝票ビューにチェンジ
//	BOOL bScan	伝票検索による呼び出し
//
void CMainFrame::ChgToDenpView( BOOL bScan/*=TRUE*/ )
{
	if( m_nowvw_flg != DNP_VIEW )
	{
		m_prevw_flg = m_nowvw_flg;
		m_nowvw_flg = DNP_VIEW;

		SetActiveChild( m_DNPview_id );
		DnpInSetMouseHook();

		((CDBbaseView*)m_dnpview)->SetSelectWindow();
		((CDBbaseView*)m_dnpview)->UpdateSelectPar();
		
		// 10.16 /12
		((CDnpInView*)m_dnpview)->diagctrl_job();

		if( bScan ) {
			//初期データの検索 & 表示
			((CDnpInView*)m_dnpview)->denp_init_rd();
		}
		else {
			((CDnpInView*)m_dnpview)->ChangeDnpView();
		}
	}
}


void CMainFrame::DirectDenpView()
{
	if( m_DNPview_id == -1 )
		return;

	if( m_nowvw_flg != DNP_VIEW )
	{
		PROGRAM_OK = FALSE;

		int prev = m_nowvw_flg;
		m_prevw_flg = m_nowvw_flg;
		m_nowvw_flg = DNP_VIEW;

		if( prev == INP_VIEW ) {
			((CDBDinpView*)m_inpview)->insert_clear();
		}
ASSERT( prev != -1 );
		((CScanKeyView*)m_skeyview)->ScanKeyClear();

		((CDBbaseView*)m_dnpview)->SetSelectWindow();
		((CDBbaseView*)m_dnpview)->UpdateSelectPar();
		((CDnpInView*)m_dnpview)->ChangeDnpView();

		PROGRAM_OK = TRUE;

	}
}



// 各ビューの取出し
CView* CMainFrame::GetCView( int viewno )
{
	CView *view = NULL;

	switch( viewno )
	{
	case INP_VIEW:
		view = m_inpview;
		break;

	case SCAN_VIEW:
		view = m_scanview;
		break;

	case SKEY_VIEW:
		view = m_skeyview;
		break;

	case DNP_VIEW:
		view = m_dnpview;
		break;
	
	case ACBK_VIEW:
		view = m_acbkview;
		break;

	}
	return view;
}


//-----------------------------------
//	ツールバー項目消去か？
//	int type	0 = 入力時, 1 = 摘要欄
//	int index
//-----------------------------------
static int isHidden( int type, int index ) 
{
	int i;
	short idx[3][20] = {
		{ 2, 4, 6, 13, -1 },
		{ 1, 3, 5, 13, -1 },
	};

	if( type >= 2 )	return 0;

	for( i = 0; idx[type][i] != -1; i++ ) {
		if( index == idx[type][i] )
			return 1;
	}

	return 0;
}

//印刷；印刷を行います；F5；印刷 <--- ツールバー文字列書式
#ifndef _SLIP_ACCOUNTS_
// 通常のツールバー表示
static char* ToolTitle1[] = {
	"F2 前項目","F3 表示設定","F4 入力設定","F5 固 定","F6 月指定","F7 登録業務","F8 仕訳検索","F9 合計","F11 仕訳切替","F12 ",
	"Ins 定型仕訳", "Del ", "Home ", "End 処理終了", " ﾊﾞｰｼﾞｮﾝ"
};
#else
static char* ToolTitle1[] = {
	"F2 前項目","F3 表示設定","F4 入力設定","F5 固 定","F6 月指定","F7 定型登録","F8 仕訳検索","F9 合計","F11 仕訳切替","F12 ",
	"Ins 定型仕訳", "Del ", "Home ", "End 処理終了", "進捗記録", " ﾊﾞｰｼﾞｮﾝ"
};
#endif

// 摘要欄のツールバー表示
static char* ToolTitle2[] = {
	"F2 先頭文字","F3 借枝番消","F4 貸枝番消","F5 ","F6 月指定","F7 摘要登録","F8 仕訳検索","F9 合計","F11 摘要複写","F12 摘要辞書",
	"Ins 定型仕訳", "Del ", "Home ", "End 処理終了", "進捗記録"," ﾊﾞｰｼﾞｮﾝ"
};

// 検索項目指定 のツールバー表示
static char* ToolTitle3[] = {
	"F2 前項目","F3 ","F4 ","F5 損益科目","F6 複数摘要","F7 摘要登録","F8 仕訳検索","F9 取消検索","F11 全項目消去","F12 摘要辞書",
	"Ins 摘要のみ", "Del ", "Home ", "End 入力へ", "進捗記録", " ﾊﾞｰｼﾞｮﾝ"
};

// 検索結果仕訳画面 のツールバー表示
static char* ToolTitle4[] = {
	"F2 前項目","F3 Excel出力","F4 一括修正","F5 印　　刷","F6 ","F7 定型登録","F8 仕訳検索","F9 合計","F11 仕訳移動","F12 ",
	"Ins ", "Del 仕訳取消", "Home 先頭仕訳", "End 入力へ", "進捗記録", " ﾊﾞｰｼﾞｮﾝ"
//	"Ins ", "Del 仕訳取消", " ﾊﾞｰｼﾞｮﾝ", "End 入力へ", "進捗記録"
};

// 伝票入力画面 のツールバー表示
static char* ToolTitle5[] = {
	"F2 前項目","F3 伝票切替","F4 ","F5 ","F6 ","F7 登録業務","F8 仕訳検索","F9 複合仕訳","F11 伝票登録","F12 自動仕訳",
	"Ins 行挿入", "Del 行削除", "Home ", "End 処理終了", "進捗記録", " ﾊﾞｰｼﾞｮﾝ"
};

// 出納帳入力画面 のツールバー表示
static char* ToolTitle6[] = {
	"F2 前項目","F3 表示設定","F4 入力設定","F5 固 定","F6 月指定","F7 登録業務","F8 先頭仕訳","F9 貸借変更","F11 科目指定","F12 自動仕訳",
	"Ins 仕訳挿入", "Del 仕訳取消", "Home ", "End 処理終了", "進捗記録", " ﾊﾞｰｼﾞｮﾝ"
};

#ifdef _SLIP_ACCOUNTS_
// 原票表示画面 のツールバー表示（摘要欄以外）
static char* ToolTitle7[] = {
	"F2 前項目","F3 ","F4 入力設定","F5 固 定","F6 月指定","F7 前の原票","F8 次の原票","F9 関連付け","F11 転記","F12 ",
	"Ins 定型仕訳", "Del ", "Home ", "End 処理終了", "進捗記録", " ﾊﾞｰｼﾞｮﾝ"
};

// 原票表示画面 のツールバー表示（摘要欄）
static char* ToolTitle8[] = {
	"F2 先頭文字","F3 借枝番消","F4 貸枝番消","F5 ","F6 月指定","F7 前の原票","F8 次の原票","F9 関連付け","F11 転記","F12 摘要辞書",
	"Ins 定型仕訳", "Del ", "Home ", "End 処理終了", "進捗記録", " ﾊﾞｰｼﾞｮﾝ"
};
#endif

//伝票修正ダイアログ時の空白用
static char* ToolTitle9[] = {
//	"F2 ","F3 ","F4 ","F5 ","F6 ","F7 ","F8 ","F9 ","F11 ","F12 ", "Ins ", "Del ", "Home ", "End ", " 進捗記録"
	"　 　　　　"," 　　　　"," 　　　　"," 　　　　"," 　　　　"," 　　　　"," 　　　　"," 　　　　"," 　　　　", " 　　　　",  " 　　　　", " 　　　　", " 　　　　", "　　　　", " 進捗記録"
};

//摘要欄F2用
static char* strF2_1 = "F2 前項目";
static char* strF2_2 = "F2 先頭文字";


//--------------------------------------------------------
//	伝票形式：摘要欄のツールバー文字列変更
//	lParam	0 = 先頭文字, 1 = 前項目
//--------------------------------------------------------
void DNPteki_toolsub(LPARAM lParam)
{
	ToolTitle5[0] = (lParam == 0) ? strF2_2 : strF2_1;
}

//--------------------------------------------------------
//	伝票形式のツールバー文字列変更
//	lParam	0, 1	新規伝票時
//			10,11	伝票修正時
//--------------------------------------------------------
void denp_toolsub( LPARAM lParam )
{
static char* predp = "F5 前の伝票";
static char* nxtdp = "F6 次の伝票";

static char* nonF5 = "F5 ";
static char* nonF6 = "F6 ";

static char* tanswk = "F9 単一仕訳";
static char* fukswk = "F9 複合仕訳";

static char* swkdel = "F4 仕訳取消";
static char* dendel	= "F4 伝票消去";

static char* denreg = "F11 伝票登録";
static char* nonF11	= "F11 ";

static char* inpend = "End 処理終了";
static char* mdfend	= "End 入力へ";

static char* dnpChg = "F3 伝票切替";
static char* dnpDel	= "F3 伝票削除";

static char* dnpF7_RDNP = "F7 定型伝票";
static char* dnpF7_TKY  = "F7 摘要登録";

	switch (lParam) {
	case 0: 	case 1:
		ToolTitle5[1] = dnpChg;
		ToolTitle5[2] = dendel;
		ToolTitle5[3] = predp;
		ToolTitle5[4] = nxtdp;

		ToolTitle5[7] = (lParam == 0) ? fukswk : tanswk;
		ToolTitle5[8] = denreg;
		ToolTitle5[13] = inpend;
		break;
	case 10:
	case 11:
		ToolTitle5[1] = dnpDel;
		ToolTitle5[2] = swkdel;
		ToolTitle5[3] = predp;
		ToolTitle5[4] = nxtdp;

		ToolTitle5[7] = (lParam == 10) ? fukswk : tanswk;
		ToolTitle5[8] = nonF11;

		if (bCONFIRM_MASTER)
			ToolTitle5[13] = inpend;
		else
			ToolTitle5[13] = mdfend;

	case 20:	//摘要欄以外
		ToolTitle5[5] = dnpF7_RDNP;
		break;
	case 21:	//摘要欄
	case 22:
		ToolTitle5[5] = dnpF7_TKY;
		break;
	}
	//摘要欄のツールバー(カーソル位置で判定)
	if (lParam == 22 || lParam == 31)	//先頭文字
		DNPteki_toolsub(0);
	else if( lParam == 20 || lParam == 21 || lParam == 30 )	//前項目
		DNPteki_toolsub(1);
}


//--------------------------------------------------------
//	スキャン項目設定のツールバー文字列変更
//	lParam	0 = 複数摘要, 1 = 科目範囲
//--------------------------------------------------------
void skey_toolsub( LPARAM lParam )
{
static char* strf6_1 = "F6 複数摘要";
static char* strf6_2 = "F6 科目範囲";

	ToolTitle3[4] = lParam ? strf6_2 : strf6_1;
}


//--------------------------------------------------------
//	スキャン結果画面のツールバー文字列変更
//	lParam	0 = 一括修正, 1 = 取消復帰
//--------------------------------------------------------
void scan_toolsub( LPARAM lParam )
{
static char* strf4_1 = "F4 一括修正";
static char* strf4_2 = "F4 取消復帰";

static char* strEND_1 = "Del 仕訳取消";
static char* strEND_2 = "Del 仕訳復帰";

	ToolTitle4[2] = lParam ? strf4_2 : strf4_1;

	ToolTitle4[11] = lParam ? strEND_2 : strEND_1;
}

//--------------------------------------------------------
//	通常形式：摘要欄のツールバー文字列変更
//	lParam	0 = 先頭文字, 1 = 前項目
//--------------------------------------------------------
void tekiyo_toolsub( LPARAM lParam )
{
	ToolTitle2[0] = (lParam==0) ? strF2_2 : strF2_1;
}

//--------------------------------------------------------
//	金額欄での固定解除のため
//	lParam	0 = 固定, 1 = 固定解除
//	type	0 = 通常, 1 = 原票関係
//--------------------------------------------------------
void fix_toolsub( LPARAM lParam, int type = 0 )
{
static char* strf5_1 = "F5 固 定";
static char* strf5_2 = "F5 固定解除";

static char* strIns_2 = "Ins 仕訳挿入";
static char* strIns_1 = "Ins 定型仕訳";

	// 原票
	if( type == 1 ) {
		if( lParam == 0 ) {
			ToolTitle7[10] = strIns_1;
		}
		else if( lParam == 2 ) {
			ToolTitle7[10] = strIns_2;
		}
		return;
	}

	if( lParam == 0 ) {
		ToolTitle1[10] = strIns_1;
		ToolTitle1[3]  = strf5_1;
	}
	else if( lParam == 2 ) {
		ToolTitle1[10] = strIns_2;
	}
	else {
		ToolTitle1[3] = strf5_2;
	}
}


//--------------------------------------------------------
//	END キーの表示変更
//	type	0 = 通常, 1 = 原票関係
//--------------------------------------------------------
void CMainFrame::end_toolsub( int tkyo, int type/* = 0*/ )
{
static char* strfEnd_1 = "End 処理終了";
static char* strfEnd_2 = "End 検索一覧へ";
const int e_idx = 13;

	BOOL bKeyScan = ((CDBDinpView*)m_inpview)->IsKeyScanMode();

	// 原票
	if( type == 1 ) {
		if( bKeyScan == 0 ) {
			ToolTitle7[e_idx] = strfEnd_1;
		}
		else {
			ToolTitle7[e_idx] = strfEnd_2;
		}
		return;
	}
	else {
		if( tkyo ) {
			if( bKeyScan == 0 ) {
				ToolTitle2[e_idx] = strfEnd_1;
			}
			else {
				ToolTitle2[e_idx] = strfEnd_2;
			}
		}
		else {
			if( bKeyScan == 0 ) {
				ToolTitle1[e_idx] = strfEnd_1;
			}
			else {
				ToolTitle1[e_idx] = strfEnd_2;
			}
		}
	}
}


//----------------------------------------------------------------
//	出納帳のツールバー文字列変更
//
//lParam
//	■□□□■■■■
//	D7 ON 摘要欄(F7 摘要登録)　OFF それ以外(F7 定型登録)
//	D0-D3 0 = 修正行, 1 = 新規入力行, 2 = 摘要欄(先頭文字)
//
//----------------------------------------------------------------
void acbook_toolsub( LPARAM lParam )
{
static char* strF7_1 = "F7 定型登録";
static char* strF7_2 = "F7 摘要登録";

static char* strINS_1 = "Ins 仕訳挿入";
static char* strINS_2 = "Ins 定型仕訳";
	
	if (lParam & 0x80) {
		ToolTitle6[5] = strF7_2;
		if ((lParam & 0x0f) == 2) {
			ToolTitle6[0] = strF2_2;
		}
		else {
			ToolTitle6[0] = strF2_1;
		}
	}
	else {
		ToolTitle6[5] = strF7_1;
		ToolTitle6[0] = strF2_1;
	}

	if( (lParam & 0x0f) == 1 ) {
		ToolTitle6[10]	= strINS_2;
	}
	else {
		ToolTitle6[10] = strINS_1;
	}
}


// ツールバーのボタン切り替え
//	ﾂｰﾙﾊﾞｰ切替
//  chg_sg 0 = 通常, 1, 摘要欄の時, 2 = スキャン項目ﾋﾞｭｰ, 3  = ｽｷｬﾝﾋﾞｭｰ, 
//		   4 = 伝票形式入力, 5 = 出納帳入力, 6 = 原票会計 (_SLIP_ACCOUNTS_)
//
void CMainFrame::ChangeToolBar( int type, int sub/* = 0*/ )
{
FILETrace( "@CMainFrame::ChangeToolBar %d,%d\n", type, sub );

	PostMessage( WM_CHGTOOLBAR, type, sub );
}


//-------------------------------------------------------------------
//	WPARAM wParam	ツールバー切り替えコード
//	LPARAM lParam	サブ切り替えコード
//-------------------------------------------------------------------
LRESULT CMainFrame::OnChgToolBar( WPARAM wParam, LPARAM lParam )
{
	int i, index, type, max;
	type = wParam;

//	if( m_NowBarType == type )	return 0;
	char** title;

	switch( type ) {
	case 0:
	case 1:
		index = m_INPview_id;
		if( type == 0 ) {
			fix_toolsub( lParam );
			title = ToolTitle1;
			max = sizeof ToolTitle1 / sizeof ToolTitle1[0];
		}
		else {
			tekiyo_toolsub( lParam );
			title = ToolTitle2;
			max = sizeof ToolTitle2 / sizeof ToolTitle2[0];
		}
		end_toolsub( type, 0 );
		break;
	case 2:	//スキャン項目設定
		skey_toolsub( lParam );

		index = m_SKEYview_id;
		title = ToolTitle3;
		// 取消仕訳処理をしない
		if( pAUTOSEL ) {
			if( pAUTOSEL->DELSWK_OPT == OPT_DELSWK ) {
				ToolTitle3[7] = "F9 ";
			}
			else {
				ToolTitle3[7] = "F9 取消検索";
			}
		}

		max = sizeof ToolTitle3 / sizeof ToolTitle3[0];
		break;
	case 3:		//スキャン結果
		scan_toolsub( lParam );
		index = m_SCANview_id;
		title = ToolTitle4;
		max = sizeof ToolTitle4 / sizeof ToolTitle4[0];
		break;
	case 4:		//伝票形式
		denp_toolsub( lParam ); 
		title = ToolTitle5;
		max = sizeof ToolTitle5 / sizeof ToolTitle5[0];
		break;
	case 5:		//出納帳
		title = ToolTitle6;
		max = sizeof ToolTitle6 / sizeof ToolTitle6[0];
		acbook_toolsub( lParam );
		break;
#ifdef _SLIP_ACCOUNTS_
	case 6:		// 原票会計
		fix_toolsub( lParam, 1 );
		title = ToolTitle7;
		end_toolsub( 0, 1 );
		max = sizeof ToolTitle7 / sizeof ToolTitle7[0];
		break;
	case 7:		// 原票会計 (摘要欄)
		title = ToolTitle8;
		end_toolsub( 1, 1 );
		max = sizeof ToolTitle8 / sizeof ToolTitle8[0];
		break;
#endif
	case 999:
		ToolBarClearFunc();
		return 1;
		break;
	}

	ICSToolBar* pBar = GetToolBar( 1 );

	char* p;
	CString	descript;

	for( i = 0; i < max; i++ ) {
		p = title[i];
		pBar->SetButtonTooltip(i, p );
		pBar->SetButtonText( i, p );

		div_toolstr( p, descript );
		pBar->SetButtonDescription( i, descript );
	}

	m_NowBarType = type;

	return 1;
}


// ツールバーの説明文
typedef struct _DESCRIPT_MSG
{
	char* tool;
	char* desript;
} DESCRIPT_MSG;

DESCRIPT_MSG	DES_MSG[] = 
{
	{ "前項目", 	"前の項目へ移動します" },
	{ "表示設定",	"仕訳行の表示設定を行います" },
	{ "入力設定",	"仕訳入力の設定を行います" },
	{ "固 定",		"入力項目を固定します" },
	{ "固定解除",	"入力項目の固定を解除します" },
	{ "合計",		"貸借合計を表示します" },
	{ "仕訳切替", "新規仕訳の入力形式を切替えます" },
	{ "定型仕訳", "定型仕訳画面を表示します" },
	{ "処理終了", "処理を終了します" },
	{ "検索一覧へ", "検索一覧に戻ります" },
// 摘要欄のツールバー表示
	{ "先頭文字",	"摘要の先頭にカーソルを移動します" },
	{ "借枝番消",	"借方の枝番を取り消します" },
	{ "貸枝番消",	"貸方の枝番を取り消します" },
	{ "摘要複写",	"上段の摘要を複写します" },
	{ "摘要辞書", "摘要辞書選択を行います" },

// 検索項目指定 のツールバー表示
	{ "損益科目",	"損益科目を指定します" },
	{ "複数摘要",	"複数の摘要から検索します" },
	{ "科目範囲",	"科目を範囲指定して検索します" },
	{ "取消検索","取消仕訳の検索を行います" },
	{ "全削除",	"検索項目の全削除を行います" },

	{ "摘要のみ", "摘要のみ修正で検索します" },
	{ "入力へ",	"入力処理に戻ります" },

//検索一覧
	{ "Excel出力", "印刷結果をExcelファイルへ出力します" },
	{ "一括修正", "仕訳の一括修正を行います" },
	{ "取消復帰", "取消仕訳の復帰を行います" },
	{ "印　　刷", "印刷を開始します" },
	{ "定型登録", "仕訳を定型仕訳に登録します" },
	{ "仕訳取消", "仕訳を取消します" },

// 伝票入力画面 のツールバー表示
	{ "伝票切替", "振替／入金／出金伝票を切替えます" },
	{ "前の伝票", "前の伝票へ移動します" },
	{ "次の伝票", "次の伝票へ移動します" },
	{ "複合仕訳", "貸借複合／貸借単一仕訳作成を切替えます" },
	{ "単一仕訳", "貸借複合／貸借単一仕訳作成を切替えます" },
/*"仕訳取消"*/
	{ "伝票消去", "伝票を消去します" },
	{ "伝票登録", "伝票を登録します" },
	{ "自動仕訳", "自動仕訳モード／通常入力モード切替" },
// 出納帳
	{ "先頭仕訳", "先頭の仕訳の位置に移動します" },
	{ "貸借変更", "仕訳の貸借を入れ替えます" },
	{ "科目指定", "相手科目の指定を行います" },
	{ "仕訳挿入", "仕訳を挿入します" }, 
	{ NULL, NULL } 
};


// ツールバー文字列を分割し、ステータスバーに表示する文字を作成する。
void CMainFrame::div_toolstr( char* str, CString& descript )
{
	CString tmp, str1, str2;
	tmp = str;

	int idx = tmp.Find( " " );

	if( idx != -1 ) {
		str1 = tmp.Left(idx);

		int len = tmp.GetLength();
		int cnt = (len - (idx+1));

		if( cnt > 0 ) {
			str2 = tmp.Right( cnt );

			descript.Empty();

			for( int i = 0; DES_MSG[i].tool != NULL; i++ ) {
				if( str2.Compare( DES_MSG[i].tool ) == 0 ) {
					descript = DES_MSG[i].desript;
					break;
				}
			}

			if( descript.IsEmpty() ) {
				descript = str2 + "を行います。";
			}
		}
		else {
			descript.LoadString( AFX_IDS_IDLEMESSAGE );
		}
	}
}


BOOL CMainFrame::GetToolBarEnabled( UINT nID )
{
//	int index;
	UINT style = 0;
/*
	index = m_wndToolBar.CommandToIndex( nID );
	style = m_wndToolBar.GetButtonStyle( index );
*/
	return !(style & TBBS_DISABLED);
}


void CMainFrame::ChangeToolBarText( int index, char* text )
{
	ICSToolBar* pBar = GetToolBar( 1 );
	pBar->SetButtonText( index, text );
}



void CMainFrame::GetFrameRect( CRect& rect )
{
	CRect rc;

//	if( m_wndStatusBar.IsWindowVisible() )
//		m_wndStatusBar.GetWindowRect( rect );

	this->GetWindowRect( rc );

TRACE( "status %d,%d,%d,%d", rect.left, rect.right, rect.top, rect.bottom );
TRACE( "main %d,%d,%d,%d", rc.left, rc.right, rc.top, rc.bottom );

}

//合計問い合わせ
void CMainFrame::totaldisp()
{
	((CDBDinpView*)m_inpview)->totaldisp( MODE_NORMAL );
}


int CMainFrame::BeginApp()
{
//FILETrace( "@CMainFrame::BeginApp TOP\n" );

	// 会社確認 ＆ マスターオープン
	if( StartBlock() != 0 )
		return 1;

	// オンクラウド環境チェック
	if( IsCloudEnv() ) {
		m_bOnCloud = TRUE;
	}

	GetMouseWheelRouting();

	// 1枚目のビューはこの中で作られる
	int st = ICSMainFrame::BeginApp();

//	theApp.GetDocumentTemplate()->Open(m_INPview_id);
	m_inpview	=	(CView*)theApp.GetDocumentTemplate()->GetViewWnd( m_INPview_id );

	theApp.GetDocumentTemplate()->Open(m_SKEYview_id);
	theApp.GetDocumentTemplate()->Open(m_SCANview_id);
	theApp.GetDocumentTemplate()->Open(m_DNPview_id);
	theApp.GetDocumentTemplate()->Open(m_ACBKview_id);

	m_skeyview	=	(CView*)theApp.GetDocumentTemplate()->GetViewWnd( m_SKEYview_id );
	m_scanview	=	(CView*)theApp.GetDocumentTemplate()->GetViewWnd( m_SCANview_id );
	m_dnpview	=	(CView*)theApp.GetDocumentTemplate()->GetViewWnd( m_DNPview_id );
	m_acbkview	=	(CView*)theApp.GetDocumentTemplate()->GetViewWnd( m_ACBKview_id );

	((CDBDinpView *)m_inpview)->save_scanview( m_scanview );	//データスキャンビュークラスアドレスセーブ

FILETrace( "@CMainFrame::BeginApp (A)\n" );

	m_nowvw_flg = INP_VIEW;

	CString string	=	"通常入力";
	SetTabLabel( m_INPview_id, string );

	string	=	"検索項目";
	SetTabLabel( m_SKEYview_id, string );

	string	=	"検索";
	SetTabLabel( m_SCANview_id, string );

	string	=	"伝票入力";
	SetTabLabel( m_DNPview_id, string );

	string	=	"出納帳入力";
	SetTabLabel( m_ACBKview_id, string );

	/*---VER2----*/
	int type;

	if( DispInfoWork( type, TRUE ) != 0 ) {
		ErrExit( 0, "画面の設定を取得できません！" );
		return 1;
	}
	//スキャン一覧より
	if( IsSCANMDFY() ) {
		SetActiveChild( m_SCANview_id );
	}
	else {
		// 前回終了時の画面より起動する
		if( type == 0 ) {
			SetActiveChild( m_INPview_id );
		}
		else if( type == 1 ) {
			SetActiveChild( m_DNPview_id );
			DnpInSetMouseHook();
		}
		else {
			SetActiveChild( m_ACBKview_id );
		}
	}
	bSCAN_ONLY_MODE = FALSE;

FILETrace( "@CMainFrame::BeginApp (B)\n" );
	// 伝票入力タイプ
	CString section;
	int rval = theApp.GetOptionInt( OPT_SECTION, OPT_KEYDENPTYPE, 0 );
	DenpTypeMode() = rval;

	if( ((CDBDinpView*)m_inpview)->BeginApp() != 0 )	
		return 1;

	((CScanKeyView*)m_skeyview)->BeginApp();

	((CDnpInView*)m_dnpview)->BeginApp();

	((CDBAbookView*)m_acbkview)->BeginApp();

	((CScanView*)m_scanview)->OnInitialize( "" );

	// 前回終了時の画面より起動する
/*	if( type == 0 ) {
		((CDBDinpView*)m_inpview)->SetSelectWindow();
	}
	else if( type == 1 ) {
		((CDnpInView*)m_dnpview)->SetSelectWindow();
	}
	else {
		((CDBAbookView*)m_acbkview)->SetSelectWindow();
	}
*/
	// 摘要 入力モードGet
	int tkysgn = theApp.GetTekiyoKeyMode();
	TekiyoKeyMode() = tkysgn;

	//	金額 Enter 複写 フラグを取得 [09.08 /08]
//	section.Format( "Software\\日本ICS株式会社\\%s\\DBAPP\\ValCopy", GET_LOGINNAME() );
	section = OPT_KEYVALCPY;
	ICSRegistryManager*	reg;
	reg = theApp.GetRegistryManager();
	DWORD	val_mode = 0;
	BOOL b = reg->GetProfileDword( section, "mode", &val_mode );
	ValEnterCopyMode() = val_mode;

	section = OPT_KEYTKYREG;
	val_mode = 0;
	b = reg->GetProfileDword( section, "type", &val_mode );
	TkyRegMode() = val_mode;

	// ICSMCTL の usertbl.optionchar に移行-------------------------
	// 金額 Enter 複写
	rval = theApp.GetOptionInt( OPT_SECTION, OPT_KEYVALCPY, -1 );
	if( rval == -1 ) {
		// キーが存在しないため移行する。
		theApp.SetOptionInt( OPT_SECTION, OPT_KEYVALCPY, ValEnterCopyMode() );
	}
	else {
		ValEnterCopyMode() = rval;
	}
	// 摘要登録
	rval = theApp.GetOptionInt( OPT_SECTION, OPT_KEYTKYREG, -1 );
	if( rval == -1 ) {
		// キーが存在しないため移行する。
		rval = theApp.SetOptionInt( OPT_SECTION, OPT_KEYTKYREG, TkyRegMode() );
	}
	else {
		TkyRegMode() = rval;
	}
	// 残高問合せ移行
	rval = theApp.GetOptionInt( OPT_SECTION, OPT_KEYZMOVE, 0 );
	ZmoveMode() = rval;

	// タブキーの動作
	rval = theApp.GetOptionInt( OPT_SECTION, OPT_KEYTKTAB, 0 );
	TKtabMode() = rval;

	// 上手クラウド
	rval = theApp.GetOptionInt( JZCLOUD_SEC, JZCLOUD_KEY, 0 );
	JZcloudEndKakutei() = rval;

	bCloudEnd = FALSE;

	rval = theApp.GetOptionInt( _T("DateTab"), _T("Mode"), 1 );
	DateTabKeyMode() = rval;
//MyTrace( "★DateTabKeyMode = %d\n", rval );

	rval = theApp.GetOptionInt( OPT_SECTION, OPT_RDZRITSU, 1 );
	RDsiwakeZeirituMode() = rval;
//MyTrace( "★RDsiwakeZeirituMode = %d\n", rval );

	rval = theApp.GetOptionInt(OPT_SECTION, OPT_KANASEARCH, 0);
	KanaSearchType() = rval;
	//消費税欄毎回停止
	rval = theApp.GetOptionInt(OPT_SECTION, OPT_SYZSTOP, 0);
	SyzStopModeValue() = rval;

	// 給与DBより取込DLLチェック
	char pdev[32] = {0};
	PCOM_GetString("ProgramDevice", pdev );

	m_ObjsPath.Format( "%s:\\ICSWin\\OBJS", pdev );

#if _MSC_VER < 1900
#ifdef _DEBUG
	section.Format( "E:\\ICSDEV\\DLL_DEBUG8\\DBKyZmconnectMod8D.dll", pdev );
#else
	section.Format( "%s:\\ICSWin\\OBJS\\DBKyZmconnectMod8.dll", pdev );
#endif
#else
	#ifdef _DEBUG
		section.Format( "E:\\ICSDEV\\DLL_DEBUG15\\KYU15_KyZmconnectModD.dll", pdev );
	#else
		section.Format( "%s:\\ICSWin\\OBJS\\KYU15_KyZmconnectMod.dll", pdev );
	#endif
#endif

	CFileFind	fnd;
	BOOL bEntry = GET_EntrySign();

//	if( fnd.FindFile( section ) && ! bEntry ) {
	if( fnd.FindFile( section ) ) {	// 上手君もOK --- 10.25 /11
		m_bKyuyoDB = TRUE;
		fnd.Close();
	}
	// WL で給与dbない場合
	if (IsWLuserNotKyuyo()) {
		m_bKyuyoDB = FALSE;
	}
	int jz = IsJz2();
	int cloud = IsCloudEnv();
//	MyTrace("IsJz2() = %d, IsCloudEnv = %d\n", jz, cloud);
	if( jz && cloud ) {
		m_bKyuyoDB = FALSE;
	}

	//減価償却DBより取込
	char buf[64], buf2[64];
	PCOM_GetString( "ExecGrJobs", buf, buf2 );

#if _MSC_VER < 1900
#ifdef _DEBUG
	section.Format( "E:\\ICSDEV\\DLL_DEBUG8\\FinancialJournalInput8D.dll", pdev );
#else
	section.Format( "%s:\\ICSWin\\CMDS\\GR%s\\JOBS%s\\FinancialJournalInput8.dll", pdev, buf, buf2 );
#endif
#else
#ifdef _DEBUG
	section.Format( "E:\\ICSDEV\\DLL_DEBUG8\\GEN15_FinancialJournalInput.dll", pdev );
#else
	section.Format( "%s:\\ICSWin\\CMDS\\GR%s\\JOBS%s\\GEN15_FinancialJournalInput.dll", pdev, buf, buf2 );
#endif
#endif

#ifdef _CUT_CLOSE // dllチェックをやめる
	//	減価償却
	if( fnd.FindFile( section ) ) {	// 上手君もOK
		m_bGensyoDB = TRUE;
		fnd.Close();
	}
#endif
	m_bGensyoDB = TRUE;
	if (IsWLuserNotGenka()) {
		m_bGensyoDB = FALSE;
	}

	//業務の減価償却DBがあるか？
	if( m_bGensyoDB ) {
		section.Format( "%s:\\ICSWin\\CMDS\\GR9F", pdev );
		if( fnd.FindFile( section ) ) {	// 上手君もOK
			fnd.FindNextFile();
			if( ! fnd.IsDirectory() ) {
				m_bGensyoDB = FALSE;
			}
			fnd.Close();
		}
		else {
			m_bGensyoDB = FALSE;
		}
	}

	if( ! (bSCAN_ONLY_MODE || IsSCANMDFY() ) ) {
		//WIZ 連携マスタ チェック
		if( WizRenkeiMasterCheck() != 0 )
			return 1;
	}

	if( ! m_bKyuyoDB || ! m_bGensyoDB ) {
		ICSToolBar* pBar = GetToolBar( 0 );
		CMenu	menu;
		menu.LoadMenu( IDR_DBDINPTYPE );

		if( ! m_bKyuyoDB )	menu.DeleteMenu( ID_MENU_KYUCSV, MF_BYCOMMAND );
		if( ! m_bGensyoDB )	menu.DeleteMenu( ID_MENU_GENCSV, MF_BYCOMMAND );

		pBar->LoadMenu( &menu );
	}

	PROGRAM_OK = TRUE;

	// 仕訳最大まで登録
	if( bSCAN_ONLY_MODE ) {
		PostMessage( WM_BUNKI, bSCAN_ONLY_MODE, 0 );
	}
	else {
		/*---VER2----*/
		if( IsSCANMDFY() ) {
			PostMessage( WM_BUNKI, 2, 0 );
		}
		else {
			// 請求書データダウンロード
			if( WizDataDownLoadJob() > 0 ) {
				//仕訳クエリ
				PostMessage(WM_BUNKI, 0, 3);
			}
			else {
				if( type == 0 ) {
					// 通常入力にフォーカスセット
					((CDBDinpView*)m_inpview)->set_firstfocus();
				}
				else if( type == 1 ) {
					//入力モードセーブ
					((CDBDinpView*)m_inpview)->_save_inpcondition();
					((CDBDinpView*)m_inpview)->SaveDinpPara(INP_mode);
					// 伝票形式
					PostMessage(WM_BUNKI, 0, 1);
					//		ChgToDenpView( FALSE );
				}
				else if( type == 2 ) {
					//入力モードセーブ
					((CDBDinpView*)m_inpview)->_save_inpcondition();
					((CDBDinpView*)m_inpview)->SaveDinpPara(INP_mode);
					// 出納帳形式
					PostMessage(WM_BUNKI, 0, 2);
					//		ChgToACBKView();
				}
			}
		}
	}

	//進捗管理
	GetShincyokuFlag();

	ICSColorInfo ci;
	((ICSWinApp*)AfxGetApp())->GetStanderdColor( ci );
	m_brBack.CreateSolidBrush( ci.ViewColor() );

	ICSToolBar* pBar = GetToolBar( 3 );

	int btncnt = pBar->GetButtonCount();
	int chklst_sgn = 0;
	for( int i = 0; i < btncnt; i++ ) {
		CString str = pBar->GetButtonText(i);
		if( str.Find( "ﾁｪｯｸﾘｽﾄ" ) != -1 ) {
			chklst_sgn = 1;
			break;
		}
	}
	if( ! chklst_sgn ) {
		((CScanView*)m_scanview)->IsCallByGenpyo() = TRUE;
	}

	//	PopupCloseState(0);
	BeginAppFlag() = TRUE;

FILETrace( "@CMainFrame::BeginApp END st = %d\n", st );

	return st;
}


//----------------------------------------------------------------------------
//		画面の分岐
//		LPARAM lParam	0: 検索項目画面, 1: 伝票画面, 2: 出納帳画面
//						3: WIZからのダウンロード時
//
//----------------------------------------------------------------------------
LRESULT CMainFrame::OnBunki( WPARAM wParam, LPARAM lParam )
{
FILETrace( "OnBunki w = %d, l = %d", wParam, lParam );

	if( wParam == 2 ) {
		// 検索一覧にフォーク
		inpmdisp( _KEYSCAN );
		ChgToScanViewEx();
	}
	else if( wParam == 0 ) {
		if( lParam == 0 ) {
			ChgToSkeyView();
		}
		else if( lParam == 1 ) {
			ChgToDenpView( FALSE );
		}
		else if( lParam == 2 ) {
			ChgToACBKView();
		}
		else if( lParam == 3 ) {
			//WIZのダウンロードデータ表示
			ChgToScanViewWizData();
		}
	}
	else if( wParam == 1 ) {	//bSCAN_ONLY_MODE
		ChgToSkeyView();
	}

	return 1;
}

void CMainFrame::ChildFrameActive( CWnd* pChild )
{
	if( ! bSysClose && PROGRAM_OK ) {
		int	cnt	=	theApp.GetDocumentTemplate()->GetDocTemplateCount();
		int vid = -1;

		for( int i = 0; i < cnt; i++ ) {
			CWnd*	pWnd	=	theApp.GetDocumentTemplate()->GetFrameWnd( i );
//TRACE( "CMainFrame::ChildFrameActive  pWnd = %08x\n", pWnd );

			if( pWnd->GetSafeHwnd() == pChild->GetSafeHwnd() ) {
	//			pWnd = theApp.GetDocumentTemplate()->GetViewWnd( i );
				vid = i;
	//			TRACE( "ChildFrameActive %d\n", i );
				break;
			}
		}
		if( PROGRAM_OK && vid != -1 ) {
			if( m_nowvw_flg == m_INPview_id )	// 入力画面情報を保存
				((CDBDinpView*)m_inpview)->tabchg_saveinfo( (vid != m_nowvw_flg) );

			if( vid != m_nowvw_flg ) {

				if( m_nowvw_flg != m_SCANview_id ) {
					// スキャン画面は、CDBbaseView 派生でない
					((CDBbaseView*)GetCView( m_nowvw_flg ))->TabClickChangedPrevView();
				}

				//一括修正項目 初期化
				CRpkeyDlg::keyline_inz();

				if( vid == m_SCANview_id ) {
					DirectScanView();
				}
				else if( vid == m_INPview_id ) {
					DirectInpView();
				}
				else if( vid == m_SKEYview_id ) {
					DirectSkeyView();
				}
				else if( vid == m_DNPview_id ) {
					DirectDenpView();
				}
				else if( vid == m_ACBKview_id ) {
					DirectACBKView();
				}

				if( vid == m_DNPview_id ) {
					 DnpInSetMouseHook();
				}
				else {
					DnpInMouseUnhook();
				}
			}
		}
	}
TRACE( ">>>--ChildFrameActive bSysClose %d, POK %d\n", bSysClose, PROGRAM_OK );

	ICSMainFrame::ChildFrameActive( pChild );

}


void CMainFrame::SetViewID( int* id )
{
	m_INPview_id	= id[0];
	m_SKEYview_id	= id[1];
	m_SCANview_id	= id[2];
	m_DNPview_id	= id[3];
	m_ACBKview_id	= id[4];
}


int CMainFrame::StartBlock()
{
	//会社確認

	try {
		int st = cocod_ok( &cosel, this, TRUE );

		switch( st )
		{
		case IDOK:
			break;
		case IDCANCEL:
		//	ermset( 0, "処理を中止します。");
			ermset( -2, NULL );
			return -1;
			break;
		case -1:
			ermset( ERROR_STOP, (char *)"会社確認エラーが発生しました。" );
			break;
		default:
			ermset( ERROR_STOP, "把握できないエラーにより処理を中止します。" );
			break;
		}

		/* ｶﾓｸ ﾒｲｼｮｳ & ﾚﾝｿｳ ﾃｷﾖｳ */
		st = init_open();
		draw_kengyobar();

		if( st == -2 ) {
			vol_close();
			ermset( 0, "残高合併マスターは処理できません。" );
		}
		else if( st == -3 ) {
			vol_close();
			CString str, msg;
			GetConfirmString(str);
			msg.Format("%sで、仕訳がありません！", str);
			ermset( ERROR_STOP, (char*)(LPCTSTR)msg );
		}

//		insseqpos_make(this);

		mst_open();

		// 07.08 /09
		SetKessanki( pDBzm->zvol->ki );
	}
	catch(CErrBlk E)
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return -1;
	}

	return 0;
}


//----------------------------------------------------------
//	スキャン項目画面より、Endキーを押された場合の
//	画面の戻り( 直前に開いていた画面に戻る )
//----------------------------------------------------------
void CMainFrame::SkeyOnButtonEnd()
{
	switch( m_SKEYprev_flg ) {
	case DNP_VIEW:
		if( bCONFIRM_MASTER && ! ((CDnpInView*)m_dnpview)->IsInputConfirmMaster() ) {
			ChgToInpView( 0 );
		}
		else {
			ChgToDenpView(FALSE);
		}
		break;
	case ACBK_VIEW:
		ChgToACBKView();
		break;
	default:
		ChgToInpView( 0 );
		break;
	}
}

//----------------------------------------------------------
//	検索項目クリア
//----------------------------------------------------------
void CMainFrame::ScanKeyClear()
{
	if( m_skeyview == NULL )	return;

	((CScanKeyView*)m_skeyview)->ScanKeyClear();
}


//----------------------------------------------------------
//	終了処理
//
//
//----------------------------------------------------------
LRESULT CMainFrame::OnMessageClose( WPARAM wParam, LPARAM lParam )
{
	if( m_inpview != NULL ) {
		if( CDBbaseView::m_ImgPrintFlg != PRINT_NONE )
			return 1;
	}

	int mode = 0;
	if( pDBzm != NULL ) {
		mode = pDBzm->IsModify() ? 1 : 0;
	}

	if( ! OnCloseSub( wParam, FALSE ) )
		return 1;

	ICSMainFrame::OnCloseProcess( mode );

	bSysClose = TRUE;

	ICSMainFrame::OnMessageClose( wParam, lParam );

	return 0;
}


//	各ビューの終了処理へ
//	BOOL bCosel		ツールバーでの会社選択時の呼び出し
//
BOOL CMainFrame::OnCloseSub( WPARAM w, BOOL bCosel/*=FALSE*/ )
{
//MyTrace("OnCloseSub TOP = %d, pDBzm(%08x)\n", w, pDBzm);

	if( ! pDBzm )	return TRUE;

	DWORD	flg = 0;
	int		disp_type = 0;	// 終了画面タイプ

#define DINP_BIT	0x01
#define SKEY_BIT	0x02
#define SCAN_BIT	0x04
#define DENP_BIT	0x08
#define ABOOK_BIT	0x10

	if( w != 100 ) {	// 03.11 /08 一括終了
						// 検索項目・一覧の画面でも終了できるように

		// 進捗管理用に、定型仕訳のチェックを行う
		if( m_nowvw_flg == m_INPview_id ) {
			if( ((CDBDinpView*)m_inpview)->EndAppChkRDdata( w ) == FALSE )
				return FALSE;
		}

		if( m_nowvw_flg == m_DNPview_id ) {
			((CDnpInView*)m_dnpview)->EndContorlJob();
		}

		if( w == 0 ) {
			// 進捗記録
			ShicyokuKiroku( 0 );
		}

		if( m_nowvw_flg == m_SCANview_id ) {
			if( ((CScanView*)m_scanview)->EndApp( w, bCosel ) == FALSE )
				return FALSE;

			flg |= SCAN_BIT;
		}
		else if( m_nowvw_flg == m_INPview_id ) {
			if( ((CDBDinpView*)m_inpview)->EndApp( w, bCosel ) == FALSE )
				return FALSE;

			flg |= DINP_BIT;
		}
		else if( m_nowvw_flg == m_SKEYview_id ) {
			if( ((CScanKeyView*)m_skeyview)->EndApp( w, bCosel ) == FALSE )
				return FALSE;

			flg |= SKEY_BIT;
		}
		else if( m_nowvw_flg == m_ACBKview_id ) {
			if( ((CDBAbookView *)m_acbkview)->EndApp( w, bCosel ) == FALSE )
				return FALSE;

			flg |= ABOOK_BIT;
			disp_type = 2;		// 出納帳形式
		}
		else if( m_nowvw_flg == m_DNPview_id ) {
			if( ((CDnpInView*)m_dnpview)->EndApp( w, bCosel ) == FALSE )
				return FALSE;

			flg |= DENP_BIT;
			disp_type = 1;		// 伝票形式
		}
	}

	// 他の画面での、未入力チェック
	if( w == 0 && ! bCONFIRM_MASTER && ! bCloudEnd ) {
		if( !(flg & DINP_BIT) ) {
			if( ((CDBDinpView*)m_inpview)->EndChk() == FALSE ) {
				PROGRAM_OK = TRUE;
				ChgToInpView( 4 );
				return FALSE;
			}
		}

		if( !(flg & DENP_BIT) ) {
			if( ((CDnpInView*)m_dnpview)->EndChk() == FALSE ) {
				PROGRAM_OK = TRUE;
				ChgToDenpView( FALSE );
				(( CDnpInView* )m_dnpview)->DenpDlgUpdateWindow();
				return FALSE;
			}
		}

		if( !(flg & ABOOK_BIT) ) {
			if( ((CDBAbookView *)m_acbkview)->EndChk() == FALSE ) {
				PROGRAM_OK = TRUE;
				ChgToACBKView();
				return FALSE;
			}
		}
	}

	/*---VER2----*/
	if( pDBzm ) {
		if( pDBzm->dbdata != NULL )
			pDBzm->dbdata->Requery(-2,0,"seq=-1",0,0);	// for speed up from scan

	//	DispInfoWork( disp_type, FALSE ); --> JobEndOperation()へ
	}

	// マスタークローズ等
	JobEndOperation( bCosel );

	if( bCloudEnd ) {
		ChainProcess();
	}

	return TRUE;
}


//他のプログラムで会社切替
BOOL CMainFrame::ReCoselCheckOtherPro()
{
	if( m_nowvw_flg == m_INPview_id ) {
		// 定型仕訳テーブルクリア
		if( m_nowvw_flg == m_INPview_id ) {
			((CDBDinpView*)m_inpview)->ReCoselCheckOtherPro();
		}
	}
	return TRUE;
}


//	各ビューの終了処理へ
//	BOOL bCosel		ツールバーでの会社選択時の呼び出し
//
BOOL CMainFrame::ReCoselEndCheck()
{
	if( ! pDBzm )	return TRUE;

	if( m_nowvw_flg == m_INPview_id ) {

		// 定型仕訳のチェック
		if( m_nowvw_flg == m_INPview_id ) {
			if( ((CDBDinpView*)m_inpview)->EndAppChkRDdata(0) == FALSE )
				return FALSE;
		}
		if( ((CDBDinpView*)m_inpview)->EndApp( 0, TRUE ) == FALSE )
			return FALSE;
	}
	else if( m_nowvw_flg == m_ACBKview_id ) {
		if( ((CDBAbookView *)m_acbkview)->EndApp( 0, TRUE ) == FALSE )
			return FALSE;
	}
	else if( m_nowvw_flg == m_DNPview_id ) {
		if( ((CDnpInView*)m_dnpview)->EndApp( 0, TRUE ) == FALSE )
			return FALSE;
	}
#ifdef _SLIP_ACCOUNTS_
	else if( m_nowvw_flg == m_SCANview_id ) {
		((CScanView*)m_scanview)->TabLbuttonDownJob();
	}
#endif

	return TRUE;
}


//-----------------------------------------------------------------------
//	マスター終了処理
//	通常入力のマスター終了処理を呼ぶ
//	BOOL bCosel		会社確認ツールバーでの会社選択時は TRUE
//					MainFrameを破棄しないように
//
//-----------------------------------------------------------------------
void CMainFrame::JobEndOperation( BOOL bCosel )
{
	// 06.24 /10 ここに移動
	if( pDBzm != NULL ) {
		if( m_inpview != NULL ) {
			//検索項目　科目タイプ
			SkeyKmkType() = ((CScanKeyView*)m_skeyview)->GetSkeyKmkType();
			//自動仕訳
			AutoSwkType() = 0;
			if( ((CDBAbookView*)m_acbkview)->GetAutoSwkType() )
				AutoSwkType() |= 0x01;
			if( ((CDnpInView*)m_dnpview)->GetAutoSwkType() )
				AutoSwkType() |= 0x02;
			// 10.18 /10
			((CDBAbookView*)m_acbkview)->GetAcbkKojiWidth();
			// 08.19 /15
			FuriMode() = ((CDnpInView*)m_dnpview)->GetFuriMode();
		}

		int		disp_type = 0;	// 終了画面タイプ

		if( m_nowvw_flg == m_ACBKview_id ) {
			disp_type = 2;		// 出納帳形式
		}
		else if( m_nowvw_flg == m_DNPview_id ) {
			disp_type = 1;		// 伝票形式
		}
		DispInfoWork( disp_type, FALSE );
	}

	if( ! bCosel ) {
		// 財務クラス解放前に 分析表のレコード解放
		if( m_inpview != NULL ) {
			((CDBbaseView*)m_inpview)->CloseWork();
			((CDBbaseView*)m_skeyview)->CloseWork();
			((CDBbaseView*)m_acbkview)->CloseWork();
			((CDBbaseView*)m_dnpview)->CloseWork();
			((CScanView*)m_scanview)->ScanCloseWork();

			((CDBDinpView*)m_inpview)->EndBlock(0);

		}
		else {
			//メモリー開放
			if( pTkjour != NULL ) {
				pTkjour->End();
				delete pTkjour;	pTkjour = NULL;
			}
			if( pDBzm != NULL ) {
				delete pDBzm;	pDBzm = NULL;
			}
			if( pDBsy != NULL ) {
				delete pDBsy;	pDBsy = NULL;
			}
			if (pZeiCl != NULL) {
				delete pZeiCl;	pZeiCl = NULL;
			}
			if( pAddInv != NULL ) {
				delete pAddInv;	pAddInv = NULL;
			}
#ifdef NAIBU_VER2
			if( pDBnc != NULL ) {
				delete pDBnc;	pDBnc = NULL;
			}
#endif
		}
	}
}



// CMainFrame メッセージ ハンドラ

LRESULT CMainFrame::OnCall( WPARAM wParam, LPARAM lParam )
{
FILETrace( "@OnCall  wParam = %d\n", wParam );

	if( wParam == ICS_MAINFRAME_PARAM_CALLSTART ) {
		char*	pStr	=	(char*)lParam;
		//呼び出しを許可しない場合０以外を返す
FILETrace( "@OnCall  ICS_MAINFRAME_PARAM_CALLSTART %s\n", pStr );

		//フォーク不可能の場合
		CView *pvw;
		pvw = GetCView(NowView());
		if( pvw != m_scanview ) {
			if( !((CDBbaseView*)pvw)->EnableCallPro() ) {
				return 1;
			}
		}

		//現在のビュー UCOM セット
#ifdef CLOSE	// ここでUCOMをセットしなくても、ICS_MAINFRAME_PARAM_FORKCHANGING を通るので、そこでセットする
		CView *pvw;
		pvw = GetCView( NowView() );

		CheckListUcom( CString(pStr ) );

		if( pvw ) {
			pvw->SetFocus();

			if( pvw == m_scanview ) {
				((CScanView*)pvw)->ForkUcomSet();	// CDBbaseViewより派生していないため
			}
			else {
				((CDBbaseView*)pvw)->ForkUcomSet();
			}
		}
#endif
               //return 1L;
	}
		//プロセス呼び出しに成功
	else if( wParam == ICS_MAINFRAME_PARAM_CALLOK ) {
		//CHAINの場合で自分自身を終了させる場合は０以外を返す
               //return 1L;
FILETrace( "@OnCall  ICS_MAINFRAME_PARAM_CALLOK\n" );
	}
		//プロセス呼び出しに失敗
	else if( wParam == ICS_MAINFRAME_PARAM_CALLERROR ) {
		//CHAINの場合で自分自身を終了させる場合は０以外を返す
               //return 1L;
FILETrace( "@OnCall  ICS_MAINFRAME_PARAM_CALLERROR\n" );
	}
	return	0L;

}

LRESULT CMainFrame::OnFromForkChain( WPARAM wParam, LPARAM lParam )
{
	LRESULT	st	=	ICSMainFrame::OnFromForkChain( wParam, lParam );

	int sgn = 0;	// PROGRAM_OK 変更サイン
	FILETrace("@OnFromForkChain TOP wParam = %08x, lParam = %08x, st = %d, bReCOSEL = %d, PROGRAM_OK(%d)\n", wParam, lParam, st, bReCOSEL, PROGRAM_OK);

	if( PROGRAM_OK ) {
		sgn = 1;
		PROGRAM_OK = FALSE;	// フォーカス無効
	}

	if( wParam == ICS_MAINFRAME_PARAM_FORKCHANGING && lParam == -1 ) {
		m_bForkChain = TRUE;
	}

	// 他のプログラムで会社選択中にこの関数にくる場合があった。　03.29 /10
	if( bReCOSEL ) {
		return st;
	}
	// WIZの処理中にここにきてICS_MAINFRAME_PARAM_FORKCHANGING のForkUCOMSetで PROGRAM_OK がもとに戻る
	if( IgnoreOnSizeFlag() ) {
		return st;
	}

	//プロセスが終了
	if( wParam == ICS_MAINFRAME_PARAM_FORKEND ) {
FILETrace( "@OnFromForkChain  ICS_MAINFRAME_PARAM_FORKEND\n" );
	}
	//自身がFORK済みでアクティブになったとき
	else if( wParam == ICS_MAINFRAME_PARAM_FORKCHANGEACTIVE ) {
FILETrace( "@OnFromForkChain  ICS_MAINFRAME_PARAM_FORKCHANGEACTIVE st %d, m_bReload = %d\n", st, m_bReload );
//CString msg;
//msg.Format( "ICS_MAINFRAME_PARAM_FORKCHANGEACTIVE st = %d\n", st );
//AfxMessageBox(msg);

		////原票読取中
		if( m_bGenpyoScan ) {
			return st;
		}

		if( !m_bReload ) {	// 修正行の原票表示中は再読み込み処理が終了しない間に、もう一度来る。
			m_bReload = TRUE;
			// 10.12 /12 移動
			fnc_scanmdfy();
			CView *pvw;
			pvw = GetCView(NowView());

			if( st ) {
				ezquery_flag() = TRUE;

				if( pvw == m_scanview ) {
					((CScanView*)pvw)->ReloadView();
				}
				else {
					//現在のビュー再読み込み
					((CDBbaseView*)pvw)->ReloadView();
				}

				if( pDBzm && IsKouekiMaster(pDBzm) ) {
					draw_kengyobar();
				}

				st = 0;
			}
			else {
			}
#ifdef CLOSE // 10.12 /12
			fnc_scanmdfy();
#endif

			if( pvw	== m_acbkview ) {
				((CDBAbookView*)pvw)->ForkActiveJob();
			}

			if( IsSCANMDFY() ) {
				MyTrace("IsSCANMDFY ->ChgToScanViewEx");
				ChgToScanViewEx();
			}
			m_bReload = FALSE;
		}
	}
	//自身がアクティブでボタン押下により、他のプロセスへ切り替えられようとしているとき
	else if( wParam == ICS_MAINFRAME_PARAM_FORKCHANGING ) {
		CView *pvw;
		pvw = GetCView( NowView() );
		//現在のビュー UCOM セット

		//選択画面削除(ダイアログ枠のウィンドウのため、FORK戻りでフォーカスが他の場所にセットされていた)
		if( m_inpview != NULL ) {
			((CDBDinpView *)m_inpview)->SelWndDelete();
		}
		//出納帳の定型残 の表示ダイアログ
		if( m_acbkview != NULL ) {
			((CDBAbookView *)m_acbkview)->RemainDlgDelete();
		}

		if( lParam != (LPARAM)-1 ) {
			CheckListUcom( CString( (LPSTR)lParam ) );
		}

		if( pvw ) {
		//	pvw->SetFocus();

			if( pvw == m_scanview ) {
				((CScanView*)pvw)->ForkUcomSet();	// CDBbaseViewより派生していないため
			}
			else {
				((CDBbaseView*)pvw)->LastDataGet();
				((CDBbaseView*)pvw)->ForkUcomSet();

				// 04.12 /11
				if( pvw == m_inpview ) {
				//	((CDBDinpView *)m_inpview)->SaveDinpPara( _APPEND );
					((CDBDinpView *)m_inpview)->SaveDinpParaFunc();		// 05.13 /11
				}
			}
		}
		// 今回修正or追加したか？
		if( pDBzm != NULL ) {
			if( pDBzm->IsModify() ) {
				pDBzm->IsModify() = FALSE;
				st = 1;
			}
			else	st = 0;
		}
FILETrace( "@OnFromForkChain  ICS_MAINFRAME_PARAM_FORKCHANGING st = %d, pDBzm = %08x\n", st, pDBzm );

	}	
	//自身が初めてプロセス起動されたとき
	else if( wParam == ICS_MAINFRAME_PARAM_FORKCALLEDSTART ) {
FILETrace( "@OnFromForkChain  ICS_MAINFRAME_PARAM_FORKCALLEDSTART\n" );
		
		fnc_scanmdfy();
		//原票読取から起動されたか？
		fnc_fromReadOCR();
	}
	//起動されている自身以外のプロセスがアクティブに切り替えられた時
	else if( wParam == ICS_MAINFRAME_PARAM_FORKCHANGE ) {
FILETrace( "@OnFromForkChain  ICS_MAINFRAME_PARAM_FORKCHANGE\n" );
	}

	// フォーカス有効
	if( sgn ) { 
		PROGRAM_OK = TRUE;
	}

	m_bForkChain = FALSE;

FILETrace( "@OnFromForkChain ◆◆◆　END　◆◆◆ st = %d, PROGRAM_OK(%d)\n", st, PROGRAM_OK );

	return	st;

}


void CMainFrame::fnc_scanmdfy()
{
	char	buf[32] = {0}, buf1[32] = {0};
	UCOM_GetString( "修正仕訳ＳＥＱ", buf, buf1 );

	if( (buf[0] && atoi( buf ) == 0) ) {
		IsSCANMDFY() = FALSE;
		m_ScmdfySeq = -1;
	}
	else if( ! buf[0] ) {
		IsSCANMDFY() = FALSE;
		m_ScmdfySeq = -1;
	}
	else {
		IsSCANMDFY() = TRUE;
		m_ScmdfySeq	 = atoi( buf );
		UCOM_SetString_BC( "修正仕訳ＳＥＱ", "0", "0" );

		if( m_inpview != NULL ) {
			((CDBDinpView *)m_inpview)->insert_clear();
		}
	}
//MyTrace( "fnc_scanmdfy1 %02x,%02x, = %d\n", buf[0], buf1[0], m_ScmdfySeq );
//MyTrace( "fnc_scanmdfy2 %s,%s, = %d\n", buf, buf1, m_ScmdfySeq );
}


// 会社選択バーより　会社選択された
LRESULT CMainFrame::OnFromParentCosel( WPARAM wParam, LPARAM lParam )
{
FILETrace( "@OnFromParentCosel  w = %d, l = %d\n", wParam, lParam );

	int sw = 0;
	int ret, st, chk = 0;
	BOOL endflg = FALSE;

	// 会社切替で、フォーカス処理等を通さない。
	PROGRAM_OK = FALSE;

	bReCOSEL = TRUE;

	try {
		while( 1 ) {
#ifdef CLOSE
			// 進捗記録
			ShicyokuKiroku( 0 );
#endif

			if( wParam == 1 ) {
				// 他のプロセスで会社変更
				ReCoselCheckOtherPro();
				st = cocod_ok( &cosel, this, TRUE );
			}
			else {
				// 終了確認
				if( ! chk ) {
					if( ReCoselEndCheck() == FALSE ) {
						bReCOSEL = FALSE;
						return 1;
					}
				}
				// ReCoselEndCheck で ＯＮになるので
				PROGRAM_OK = FALSE;
				// 進捗記録
				if( pDBzm != NULL ) {
					ShicyokuKiroku( 0 );
				}

				// 自身の会社バーで会社変更
				st = cocod_ok( &cosel, this, FALSE );
			}
			// 会社変更なし
			if( st == IDCANCEL ) {
				// 他のプロセスで会社変更時のキャンセルは、販売制限なので 起動させない
				if( wParam == 1 ) {
					PostMessage( WM_CLOSE, 1, 0 );
					bReCOSEL = FALSE;
					return 0;
				}
				// 合併マスターなど、処理できない場合
				if( endflg ) {
					PostMessage( WM_CLOSE );
					bReCOSEL = FALSE;
					return 0;
				}
				PROGRAM_OK = TRUE;

				//フォーカスセット
				ReCoselSetFocus(FALSE);
				bReCOSEL = FALSE;
				return 1;
			}
			else if( st == -1 ) {
				PostMessage( WM_CLOSE, 1, 0 );
				bReCOSEL = FALSE;
				return 0;
			}

			// マスタークローズ
			JobEndOperation( FALSE );

			//ボリュームオープン＆チェック
			ret = init_open();
			if( ret == -2 || ret == -3 ) {
				vol_close();
				CString msg;
				if( ret == -2 ) {
					msg = _T("残高合併マスターは処理できません。");
				}
				else {
					CString str;
					GetConfirmString(str);
					msg.Format("%sで、仕訳がありません！", str);
				}

				if( wParam == 0 ) {
					myICSMessageBox( msg );
					endflg = TRUE;
					continue;
				}
				else {
					myICSMessageBox( msg );
					ICSMainFrame::OnMessageClose(0,0);
					break;
				}
			}

			insseqpos_make(this);

			draw_kengyobar();
			//マスターオープン
			mst_open();
			// 07.08 /09
			SetKessanki( pDBzm->zvol->ki );

			//進捗管理
			GetShincyokuFlag();

			//ビューを通常入力に戻す
			if( m_nowvw_flg == SCAN_VIEW || m_nowvw_flg == SKEY_VIEW ) {
				m_prevw_flg		= m_nowvw_flg;
				m_nowvw_flg		= INP_VIEW;
				m_SKEYprev_flg	= INP_VIEW;
				SetActiveChild( m_INPview_id );
				DnpInMouseUnhook();
			}

			// 画面の更新等
			((CDBDinpView*)m_inpview)->Recosel_Job();

			// 出納帳の情報等を取得
			int type;
			DispInfoWork( type, TRUE );

			if( ((CDBDinpView*)m_inpview)->BeginApp(1) != 0 ) {
				ermset( 0, "会社再選択時の初期化処理でエラーが発生しました！" );
			}

			// SREC の情報をセットする。
			((CDBDinpView*)m_inpview)->ReCosel_DinpData();

			// 選択モジュールへの ウィンドウセット
			switch( m_nowvw_flg ) {
			case INP_VIEW:		((CDBDinpView*)m_inpview)->SetSelectWindow();	break;
			case SKEY_VIEW:		((CDBbaseView*)m_skeyview)->SetSelectWindow();	break;
			case DNP_VIEW:		((CDBbaseView*)m_dnpview)->SetSelectWindow();	break;
			case ACBK_VIEW:		((CDBbaseView*)m_acbkview)->SetSelectWindow();	break;
			}

			((CScanKeyView*)m_skeyview)->BeginApp();
			((CDnpInView*)m_dnpview)->BeginApp();

			((CDBAbookView*)m_acbkview)->BeginApp();
			((CDBAbookView*)m_acbkview)->ReCosel_Job();

			((CScanView*)m_scanview)->OnInitialize( "" );

FILETrace( "@OnFromParentCosel2 now %d\n", get_nowln() );

			ResetWizRenkei();
			BOOL oldFlg = m_bWIZMaster;
			if( WizRenkeiMasterCheck() != 0 )
				break;
			if( WizDataDownLoadJob() > 0 ) {
				//仕訳クエリ
				PostMessage(WM_BUNKI, 0, 3);
				break;
			}
			//フォーカスセット
			ReCoselSetFocus(TRUE);
#ifdef CLOSE
			// 通常入力に
			((CDBDinpView*)m_inpview)->set_firstfocus();
#endif
			break;
		}
	}
	catch(CErrBlk E)
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return -1;
	}

	bReCOSEL = FALSE;

	return	0;
}


void CMainFrame::ReCoselSetFocus( BOOL bNewCo )
{
	// 通常入力に
	CDBbaseView* pv = NULL;
	int tlbar, sub;
	sub = 0;

	switch( m_nowvw_flg ) {
	case INP_VIEW:	pv = (CDBbaseView*)m_inpview;	
					tlbar = 0;
					break;
	case DNP_VIEW:	pv = (CDBbaseView*)m_dnpview;	
					tlbar = 4;
					break;
	case ACBK_VIEW:	pv = (CDBbaseView*)m_acbkview;	
					tlbar = 5;
					break;
	case SKEY_VIEW:	pv = (CDBbaseView*)m_skeyview;
					sub = ((CScanKeyView*)m_skeyview)->GetToolSub();
					tlbar = 2;
					break;
	case SCAN_VIEW:	pv = NULL;
					tlbar = 3;
					break;
	}

	ChangeToolBar( tlbar, sub );

	// 仕訳が登録されていないので、検索画面へ
	if( bSCAN_ONLY_MODE ) {
		PostMessage( WM_BUNKI, bSCAN_ONLY_MODE, 0 );
		PROGRAM_OK = TRUE;
	}
	else {
		if( pv ) {
			pv->ReCoselSetFocus( bNewCo );
			PROGRAM_OK = TRUE;
		}
	}
}



void CMainFrame::OnUpdateButtonF5(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->Enable( FALSE );
}


// 個別のプログラムを呼び出す
void CMainFrame::CallOtherPro( int prosgn )
{
	CString job;
	char buf[64], buf2[64];
	PCOM_GetString( "ExecGrJobs", buf, buf2 );

	switch( prosgn ) {
	case 0:
		((CScanView*)m_scanview)->ChecklistUcomSet( 0 );
		job.Format( "GR%s\\JOBS%s\\Chklst8.exe", buf, buf2 );
		CallJob( job );
		break;
	case 1:
		((CScanView*)m_scanview)->ChecklistUcomSet( 1 );
		job.Format( "GR%s\\JOBS%s\\Chklst8.exe", buf, buf2 );
		CallJob( job );
		break;
	}

}


//
// チェックリストのUCOMをセット[チェインの種類]する
//
void CMainFrame::CheckListUcom( CString& pro )
{
	CString fnam;

	int n = pro.ReverseFind('\\');
	LPSTR p = pro.GetBuffer(256);
	fnam.Format( _T("%s"), &p[n+1] );
	pro.ReleaseBuffer();
/*
FILETrace( "@CheckListUcom BOOL %d\n", ((CScanView*)m_scanview)->IsCheckListPrint() );
char buf[64], buf1[64], buf2[64];
int st;
if(	(st = UCOM_GetString( CHECK_UCSTR1, buf, buf1, buf2 )) > 0 ) {
FILETrace( "@CheckListUcom sign %s\n", buf2 );
}
*/
	if( fnam.CompareNoCase( _T("Chklst8.exe") ) == 0 ) {
		if( ! ((CScanView*)m_scanview)->IsCheckListPrint() ) {
			SetNormalCheckUCOM();
		}
		else {
			((CScanView*)m_scanview)->IsCheckListPrint() = FALSE;
		}
	}

//CString msg;
//msg.Format( "n = %d, pro = %s, fnam %s", n, pro, fnam );
//msg.Format( "len %d, n = %d, --%d, tmp = %s, fnam %s", len, n, (len-n)+1, tmp, fnam );
//AfxMessageBox(msg);
}


//
// チェックリストのUCOMをセット[チェインの種類]する
//		プログラムバーからの通常呼び出し
//
void CMainFrame::SetNormalCheckUCOM()
{
	char buf[64], buf1[64], buf2[64];

	int st;
	if(	(st = UCOM_GetString( CHECK_UCSTR1, buf, buf1, buf2 )) > 0 ) {
		UCOM_SetString_BC( CHECK_UCSTR1, buf, buf1, _T("0") );
	}
}


// チェックリストのUCOMをリセットする
// 未使用[チェックリスト側でやってもらう]
//
void CMainFrame::ResetCheckUCOM()
{
	char buf[64], buf1[64];

	sprintf_s( buf, sizeof buf, "%d", -1);
	sprintf_s( buf1, sizeof buf1, "%d", -1);

	UCOM_SetString_BC( CHECK_UCSTR1, buf, buf1 );

	::ZeroMemory( buf, sizeof buf );
	UCOM_SetStringAll_BC( CHECK_UCSTR2, buf );
}


//設定保存のイニシャライズ owntb itm_name DBDINP_SETUP2
//
//		owntb->itm_name "DBDINP_SETUP2"
//			vl[0] =【伝票形式】：単一仕訳(0) 複合仕訳(1); --- 複合仕訳をデフォルト
//			vl[1] =【全体】 前回表示画面 = type;
//			vl[2] =【伝票形式】：資金繰り枝番 行う(0);
//			vl[3] = 検索項目科目タイプ;
//			vl[4] = 自動仕訳タイプ
//			vl[5] = 出納帳　：オプション
//			vl[6] = 出納帳　：工事番号幅
//			vl[7] =【伝票形式】：部門チェックボックス
//			vl[8] =【伝票形式】：修正時、諸口仕訳表示モード
//			vl[9] =【伝票形式】：伝票番号重複チェック
//			vl[10] =【伝票形式】：部門／工事番号クリア
//			vl[11] = 出納帳　：月指定の期間で仕訳を表示する
//			vl[12] = 重複仕訳のチェックボックス
//
int InitDBDinpSetup2()
{
	ICSMCTL	mctrl;
	mctl_usertbl	utbl;

	if( mctrl.SetDatabase() == 0 ) {
		mctrl.get_usertbl(utbl);
	}
	else {
		return -1;
	}
	CString filter;
	filter.Format("apl_name = '%s' AND itm_name = '%s' AND u_id = %d", PRO_NAME, SETUP_NAME, utbl.user_id);

	pDBzm->owntb->Requery(filter);

	if( pDBzm->owntb->st == -1 ) {
		pDBzm->owntb->AddNew();
		pDBzm->owntb->apl_name = PRO_NAME;
		pDBzm->owntb->itm_name = SETUP_NAME;
		pDBzm->owntb->itm_seq = 1;
		pDBzm->owntb->u_id = utbl.user_id;

		//【伝票】複合仕訳をデフォルトで
		pDBzm->owntb->vl[0] = 1;
		//【伝票】資金繰り枝番 vl[1] は 前回表示画面
		pDBzm->owntb->vl[2] = 0;
		//【伝票】振替伝票 部門 １データごと部門
		pDBzm->owntb->vl[7] = FURI_DATABMON;
		//【伝票】諸口詰めるか？
		pDBzm->owntb->vl[8] = 0;
		//【伝票】伝票番号重複チェック
		pDBzm->owntb->vl[9] = 0;
		//【伝票】部門／工事番号クリア
		if( BMON_MST || M_KOJI ) {
			pDBzm->owntb->vl[10] = 1;
		}
		//【全体】通常入力をデフォルトで
		pDBzm->owntb->vl[1] = 0;
		//【検索】科目単一検索 をデフォルト
		pDBzm->owntb->vl[3] = 0;
		//【出納帳】表示項目設定
		pDBzm->owntb->vl[5] = (ACQ_BUMON | ACQ_KOJI | ACQ_EDA);
		//【出納帳】工事幅
		pDBzm->owntb->vl[6] = 0;
		
		//【出納帳】月指定期間で仕訳表示
		pDBzm->owntb->vl[11] = 0;
		//【検索】重複仕訳のチェックボックス
		pDBzm->owntb->vl[12] = 0;

		pDBzm->owntb->Update();
		pDBzm->owntb->Requery(filter);
	}
	else {
		//部門／工事番号クリア
		if( BMON_MST || M_KOJI ) {
			if( pDBzm->owntb->vl[10] < 0 )
				pDBzm->owntb->vl[10] = 1;
		}

		//間違えて vl[8] で 【出納帳】月指定期間で仕訳表示 を保存していたため、こちらに移動する(03.29 /24)
		if( pDBzm->owntb->vl[11] == -1 ) {
			pDBzm->owntb->vl[11] = pDBzm->owntb->vl[8];
		}
	}
	return 0;
}


//--------------------------------------------------------------
//	画面の情報を取得
//		int&	type	0:通常入力, 1:伝票形式, 2:出納帳
//		BOOL	bRead	TRUE: 読み込み、FALSE: 書き込み
//
//--------------------------------------------------------------
int CMainFrame::DispInfoWork( int& type, BOOL bRead )
{
	CString filter;
	int own_sgn = 0;

	if( pDBzm->owntb == NULL ) {
		if( pDBzm->OwnTblOpen() != 0 )
			return -1;
		own_sgn = 1;
	}

	// 設定情報初期化およびread
	if( InitDBDinpSetup2() == -1 )
		return -1;

	if( bRead ) {
		type = pDBzm->owntb->vl[1];
		if( type < 0 )	type = 0;

		if( pDBzm->owntb->vl[3] <= 0 )
			SkeyKmkType() = 0;
		else
			SkeyKmkType() = 1;

		// 06.22 /10
		if( pDBzm->owntb->vl[4] <= 0 )
			AutoSwkType() = 0;
		else
			AutoSwkType() = pDBzm->owntb->vl[4];
		// 07.29 /10
		if( pDBzm->owntb->vl[5] < 0 ) {
			pDBzm->owntb->vl[5] = (ACQ_BUMON|ACQ_KOJI|ACQ_EDA);
		}
		AcbkOption() = pDBzm->owntb->vl[5];
		// 10.18 /10
		if( pDBzm->owntb->vl[6] < 0 )	pDBzm->owntb->vl[6] = 0;
		AcbkKojiWd() = pDBzm->owntb->vl[6];
		// 08.19 /15
		if( pDBzm->owntb->vl[7] < 0 )	pDBzm->owntb->vl[7] = FURI_DATABMON;
		FuriMode() = pDBzm->owntb->vl[7];

		if( pDBzm->owntb->vl[11] < 0 )
			pDBzm->owntb->vl[11] = 0;
		AcbkKikanSwk() = pDBzm->owntb->vl[11];

		if( pDBzm->owntb->vl[12] < 0 )
			pDBzm->owntb->vl[12] = 0;
		DupliSwk() = pDBzm->owntb->vl[12];
	}
	else {
		pDBzm->owntb->Edit();
		pDBzm->owntb->vl[1] = type;
		pDBzm->owntb->vl[3] = SkeyKmkType();
		// 06.22 /10
		pDBzm->owntb->vl[4] = AutoSwkType();
		// 07.29 /10
		pDBzm->owntb->vl[5] = AcbkOption();
		// 10.18 /10
		pDBzm->owntb->vl[6] = AcbkKojiWd();
		// 08.19 /15
		pDBzm->owntb->vl[7] = FuriMode();
		// 02.01 /24
		pDBzm->owntb->vl[11] = AcbkKikanSwk();
		// 03.29 /24
		pDBzm->owntb->vl[12] = DupliSwk();

		pDBzm->owntb->Update();
	}

	if( own_sgn ) {
		pDBzm->OwnTblClose();
	}

	return 0;
}


void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
//MyTrace("●●●@CMainFrame::OnActivate nState = %d\n", nState);
//
//if( pWndOther ) {
//	MyTrace("●●●@CMainFrame::OnActivate pWndOther = %08x\n", pWndOther->m_hWnd);
//	CWnd* pwnd = CWnd::FromHandle(pWndOther->m_hWnd);
//	CRect rect;
//	pwnd->GetWindowRect(&rect);
//	MyTrace("●●●@CMainFrame::OnActivate pWndOther rect(%d,%d,%d,%d)\n", rect.left, rect.top, rect.right, rect.bottom);
//
//	CWnd* parent;
//	parent = pWndOther->GetParent();
//	if( parent ) {
//		MyTrace("●●●@CMainFrame::OnActivate pWndOther's Parent = %08x\n", pWndOther->m_hWnd);
//	}
//}

//
//if( pWndOther ) {
//	MyTrace("●●●@CMainFrame::OnActivate pWndOther = %08x\n", pWndOther->m_hWnd);
//	CWnd* pwnd = CWnd::FromHandle(pWndOther->m_hWnd);
//	CRect rect;
//	pwnd->GetWindowRect(&rect);
//	MyTrace("●●●@CMainFrame::OnActivate pWndOther rect(%d,%d,%d,%d)\n", rect.left, rect.top, rect.right, rect.bottom);
//
//	CWnd* parent;
//	parent = pWndOther->GetParent();
//	if( parent ) {
//		MyTrace("●●●@CMainFrame::OnActivate pWndOther's Parent = %08x\n", pWndOther->m_hWnd);
//	}
//}

	m_ActivateFlag = nState;

	if( nState == WA_INACTIVE ) {
		if( !bSysClose ) {
			if( m_nowvw_flg == ACBK_VIEW ) {
				if( CDBbaseView::m_ImgPrintFlg != 0 )
					return;
				//出納帳項目幅保存
				((CDBbaseView*)GetCView(m_nowvw_flg))->OnActivateJob(nState, pWndOther, bMinimized);
			}
		}
	}
	else if( nState == WA_ACTIVE ) {
		if( m_nowvw_flg == INP_VIEW ) {
			((CDBbaseView*)GetCView(m_nowvw_flg))->OnActivateJob(nState, pWndOther, bMinimized);
		}
	}

	//ビューの::OnSize() は この中から呼ばれる
	ICSMainFrame::OnActivate(nState, pWndOther, bMinimized);
//MyTrace( "◆◆◆@CMainFrame::OnActivate nState = %d\n", nState );

	if( nState == WA_INACTIVE ) {
		if( ! bSysClose ) {
			if( m_nowvw_flg != NON_VIEW ) {
				if( CDBbaseView::m_ImgPrintFlg != 0 )
					return;

				if( m_nowvw_flg != m_SCANview_id ) {
					// スキャン画面は、CDBbaseView 派生でない
					((CDBbaseView*)GetCView( m_nowvw_flg ))->OnActivateJob(nState, pWndOther, bMinimized);
				}
				else {
					((CScanView*)GetCView( m_nowvw_flg ))->OnActivateJob(nState, pWndOther, bMinimized);
				}
			}
		}
	}

	m_ActivateFlag = -1;
}


/*-- 一括終了のための ICSMessageBox --*/
int myICSMessageBox( LPCTSTR text, UINT type/* = MB_OK*/, UINT help/* = 0*/, short point/* = 0*/, CWnd* pParent/* = NULL*/ )
{
	int st;

	((CMainFrame*)GetDBDinpMainFrame())->MsgBoxOnSizeFlag() = TRUE;

	((CMainFrame*)GetDBDinpMainFrame())->PopupCloseState(2);

	st = ICSMessageBox( text, type, help, point, pParent );

	((CMainFrame*)GetDBDinpMainFrame())->PopupCloseState(0);

	((CMainFrame*)GetDBDinpMainFrame())->MsgBoxOnSizeFlag() = FALSE;

	return st;
}


/*-- 一括終了のための ICSExtBox --*/
int myICSExtBox( LPCTSTR title, LPCTSTR text, LPCTSTR btn1text, LPCTSTR btn2text, LPCTSTR btn3text, UINT type, UINT help/* = 0*/, short point/* = 0*/, CWnd* pParent/* = NULL*/ )
{
	int st;

	((CMainFrame*)GetDBDinpMainFrame())->PopupCloseState(2);

	st = ICSExtBox( title, text, btn1text, btn2text, btn3text, type, help, point, pParent );

	((CMainFrame*)GetDBDinpMainFrame())->PopupCloseState(0);

	return st;

}

#ifdef _20081201_NP_EXTEND_
// 個人科目 表示制限
void CMainFrame::OnIndkmk()
{
	if( pDBzm != NULL ) {
		int type =  IsKojinGyosyuMaster( pDBzm );
		if( type ) {
			if( IsKojinKengyoMaster( type ) ) {
				int st = indkmk_dialog( this, pDBzm->m_dwIndSgn, pDBzm );

				if( st == IDOK ) {
					pAUTOSEL->INDSGN_OPT = (short)pDBzm->m_dwIndSgn;
					//現在表示中の科目表示更新
					CDBbaseView* pv = NULL;

					switch( m_nowvw_flg ) {
					case INP_VIEW:	pv = (CDBbaseView*)m_inpview;
									break;
					case DNP_VIEW:	pv = (CDBbaseView*)m_dnpview;
									break;
					case ACBK_VIEW:	pv = (CDBbaseView*)m_acbkview;
									break;
					case SKEY_VIEW:	pv = (CDBbaseView*)m_skeyview;
									break;
					}

					if( pv ) {
						pv->ResetSelect();

						if( m_nowvw_flg == ACBK_VIEW ) {
							((CDBAbookView*)m_acbkview)->BaseKmkIndCheck();
						}
						else if( m_nowvw_flg == DNP_VIEW ) {
							((CDnpInView*)m_dnpview)->DispBaseKmk();
						}
					}

					draw_kengyobar();
				}
				else if( st == -1 ) {
					ErrExit( 0, "個人科目表示設定でエラーが発生しました！" );
				}
			}
		}
	}
}
#endif

// 個人科目 表示制限
void CMainFrame::OnIndkmk()
{
	if( pDBzm != NULL ) {
		int type =  IsKojinGyosyuMaster( pDBzm );
		if( type ) {
			if( IsKojinKengyoMaster( type ) ) {
				int st = indkmk_dialog( this, pDBzm->m_dwIndSgn, pDBzm );

				if( st == IDOK ) {
					pAUTOSEL->INDSGN_OPT = (short)pDBzm->m_dwIndSgn;
					//現在表示中の科目表示更新
					CDBbaseView* pv = NULL;

					switch( m_nowvw_flg ) {
					case INP_VIEW:	pv = (CDBbaseView*)m_inpview;
									break;
					case DNP_VIEW:	pv = (CDBbaseView*)m_dnpview;
									break;
					case ACBK_VIEW:	pv = (CDBbaseView*)m_acbkview;
									break;
					case SKEY_VIEW:	pv = (CDBbaseView*)m_skeyview;
									break;
					}

					if( pv ) {
						pv->ResetSelect();

						if( m_nowvw_flg == ACBK_VIEW ) {
							((CDBAbookView*)m_acbkview)->BaseKmkIndCheck();
						}
						else if( m_nowvw_flg == DNP_VIEW ) {
							((CDnpInView*)m_dnpview)->DispBaseKmk();
						}
					}

					draw_kengyobar();
				}
				else if( st == -1 ) {
					ErrExit( 0, "個人科目表示設定でエラーが発生しました！" );
				}
			}
		}
		else if( IsKouekiMaster(pDBzm) ){
			int rt = IsExtendJgyOrKnrMaster( pDBzm );
			if( rt == -1 ){
				ErrExit( 0, "公益科目表示設定でエラーが発生しました！" );
			}
			else if( rt == 1 ){
			
				int st = kpubkmk_dialog( this, &pDBzm->m_Jgykcd, &pDBzm->m_Knrkcd, pDBzm );

				if( st == IDOK ){
					pAUTOSEL->NPJGY_OPT = pDBzm->m_Jgykcd;
					pAUTOSEL->NPKNR_OPT = pDBzm->m_Knrkcd;
					//現在表示中の科目表示更新
					CDBbaseView* pv = NULL;

					switch( m_nowvw_flg ) {
					case INP_VIEW:	pv = (CDBbaseView*)m_inpview;
									break;
					case DNP_VIEW:	pv = (CDBbaseView*)m_dnpview;
									break;
					case ACBK_VIEW:	pv = (CDBbaseView*)m_acbkview;
									break;
					case SKEY_VIEW:	pv = (CDBbaseView*)m_skeyview;
									break;
					}

					if( pv ) {
						pv->ResetSelect();

						if( m_nowvw_flg == ACBK_VIEW ) {
							((CDBAbookView*)m_acbkview)->BaseKmkIndCheck();
						}
						else if( m_nowvw_flg == DNP_VIEW ) {
							((CDnpInView*)m_dnpview)->DispBaseKmk();
						}
					}

					draw_kengyobar();
				}
			}
		}
	}
}

// 個人科目
void CMainFrame::OnUpdateIndkmk(CCmdUI *pCmdUI)
{
	if( pDBzm != NULL && m_nowvw_flg != SCAN_VIEW ) {
		int type =  IsKojinGyosyuMaster( pDBzm );
		if( type ) {
			if( IsKojinKengyoMaster( type ) ) {
				pCmdUI->Enable( TRUE );
				return;
			}
		}
	}

	pCmdUI->Enable( FALSE );
}

#ifdef _20081201_NP_EXTEND_
 void CMainFrame::draw_kengyobar()
 {
	CString	str;

	int type =  IsKojinGyosyuMaster( pDBzm );
	if( type ) {
		if( IsKojinKengyoMaster( type ) ) {
			if( pDBzm->m_dwIndSgn & SL_IND_ALL )
				str = "全業種";
			else if( pDBzm->m_dwIndSgn & SL_IND_IPPAN )		str = "一般";
			else if( pDBzm->m_dwIndSgn & SL_IND_HUDOSAN )	str = "不動産";
			else if( pDBzm->m_dwIndSgn & SL_IND_NOGYO )		str = "農業";
		}
		else {
			switch( type&0x0f ) {
			case 0x01:	str = "一般";		break;
			case 0x02:	str = "不動産";		break;
			case 0x04:	str = "農業";		break;
			}
		}
	}

	str = _T("公益");
	RedrawStatusBar( str );
 }
#endif

void CMainFrame::draw_kengyobar()
{
	CString	str;

	int type =  IsKojinGyosyuMaster( pDBzm );
	if( type ) {
		if( IsKojinKengyoMaster( type ) ) {
			if( pDBzm->m_dwIndSgn & SL_IND_ALL )
				str = "全業種";
			else if( pDBzm->m_dwIndSgn & SL_IND_IPPAN )		str = "一般";
			else if( pDBzm->m_dwIndSgn & SL_IND_HUDOSAN )	str = "不動産";
			else if( pDBzm->m_dwIndSgn & SL_IND_NOGYO )		str = "農業";
		}
		else {
			switch( type&0x0f ) {
			case 0x01:	str = "一般";		break;
			case 0x02:	str = "不動産";		break;
			case 0x04:	str = "農業";		break;
			}
		}
	}
/*- '09.02.04 -*/
//	else if( IsKouekiMaster(pDBzm) ){
/*-------------*/
	else if( IsKouekiMaster(pDBzm) && IsExtendJgyOrKnrMaster(pDBzm) ){
/*-------------*/
		CXTPStatusBarPane*	pPane = m_wndStatusBar.GetPane( 1 );
		if( pPane ){
			CString	str;
			str = pPane->GetTooltip();
			if( str != "科目選択欄 事業費・管理費" ){
				pPane->SetTooltip( "科目選択欄 事業費・管理費" );
				CFont*	pFntText	=	new CFont;

				LOGFONT lf = {0};
				strcpy_s( lf.lfFaceName, sizeof lf.lfFaceName, "ＭＳ ゴシック" );
				lf.lfCharSet = SHIFTJIS_CHARSET;
				lf.lfHeight	= 90;
				lf.lfWeight	= FW_BOLD;
				pFntText->CreatePointFontIndirect( &lf );
				pPane->SetTextFont( pFntText );

				m_wndStatusBar.SetPaneWidth( 1, 360 );

				if( pFntText )	delete pFntText;	// '09.03.07
			}
		}
		// 名称取得
		GetJgyKnrDspstr( pDBzm, pDBzm->m_Jgykcd, pDBzm->m_Knrkcd, &str );
	}

	RedrawStatusBar( str );
}

 BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
 {
	 // TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	 if( pMsg->message == WM_LBUTTONDOWN ) {
		CXTPStatusBarPane*	pPane;

		int xPos, yPos;
		CPoint point;

		xPos = GET_X_LPARAM(pMsg->lParam); 
		yPos = GET_Y_LPARAM(pMsg->lParam); 

		point.SetPoint( xPos, yPos );
		pPane = m_wndStatusBar.HitTest( point );
//		pPane = m_wndStatusBar.FindPane( ID_KENGYOMODE );

		::ClientToScreen( pMsg->hwnd, &point );
		CRect	Rect;

		m_wndStatusBar.GetWindowRect(&Rect);
		if( Rect.PtInRect( point )) {
			if( pPane != NULL ) {
				if( pPane->GetID() == ID_KENGYOMODE ) {
					if( m_nowvw_flg != SCAN_VIEW ) {
						OnIndkmk();
						return TRUE;
					}
				}
			}
		}
	 }

	 return ICSMainFrame::PreTranslateMessage(pMsg);
 }

 void CMainFrame::OnUpdateJgykmk(CCmdUI *pCmdUI)
 {
	 // TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
 	if( pDBzm != NULL && m_nowvw_flg != SCAN_VIEW ) {
		if( IsKouekiMaster(pDBzm) && IsExtendJgyOrKnrMaster(pDBzm) ){
			pCmdUI->Enable( TRUE );
			return;
		}
	}

	pCmdUI->Enable( FALSE );
}

 void CMainFrame::OnJgykmk()
 {
	 // TODO: ここにコマンド ハンドラ コードを追加します。
	if( pDBzm != NULL ) {
		if( IsKouekiMaster(pDBzm) ){
			int rt = IsExtendJgyOrKnrMaster( pDBzm );
			if( rt == -1 ){
				ErrExit( 0, "公益科目表示設定でエラーが発生しました！" );
			}
			else if( rt == 1 ){
			
				int st = kpubkmk_dialog( this, &pDBzm->m_Jgykcd, &pDBzm->m_Knrkcd, pDBzm );

				if( st == IDOK ){
					pAUTOSEL->NPJGY_OPT = pDBzm->m_Jgykcd;
					pAUTOSEL->NPKNR_OPT = pDBzm->m_Knrkcd;
					//現在表示中の科目表示更新
					CDBbaseView* pv = NULL;

					switch( m_nowvw_flg ) {
					case INP_VIEW:	pv = (CDBbaseView*)m_inpview;
									break;
					case DNP_VIEW:	pv = (CDBbaseView*)m_dnpview;
									break;
					case ACBK_VIEW:	pv = (CDBbaseView*)m_acbkview;
									break;
					case SKEY_VIEW:	pv = (CDBbaseView*)m_skeyview;
									break;
					}

					if( pv ) {
						pv->ResetSelect();

						if( m_nowvw_flg == ACBK_VIEW ) {
							((CDBAbookView*)m_acbkview)->BaseKmkIndCheck();
						}
						else if( m_nowvw_flg == DNP_VIEW ) {
							((CDnpInView*)m_dnpview)->DispBaseKmk();
						}
					}

					draw_kengyobar();
				}
			}
		}
	}
}


// ステータスバーにガイド表示
void CMainFrame::StatusGuideDisp( int strid )
{
	CString str;
	str.LoadString( strid );
	CXTPStatusBarPane*	pPane = m_wndStatusBar.GetPane( 0 );
	pPane->SetText( str );
}

#ifdef _SLIP_ACCOUNTS_
LRESULT CMainFrame::OnMessageMultipos( WPARAM w, LPARAM l )
{
	if( m_nowvw_flg == INP_VIEW ){
		int	sw = 0;
		if( w == 0 ){	// 左に位置する
		//	sw = 1;
			sw = 2;		// 05.22 /13
		}
		else{			// それ以外の位置
			sw = 0;
		}
		((CDBDinpView*)m_inpview)->MoveSubWindow( sw );
	}

	return ICSMainFrame::OnMessageMultipos( w, l );
}

//-----------------------------------------------------------------------------
// 設定変更を各ビューに伝える ('12.09.26)
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CMainFrame::SendChangeSlipOption()
{
	if( m_nowvw_flg != INP_VIEW ){
		((CDBDinpView*)m_inpview)->ImageOptionChanged();
	}
	if( m_nowvw_flg != m_SCANview_id ){
		((CScanView*)m_scanview)->ImageOptionChanged();
	}

	return 0;
}
#endif


void CMainFrame::SwkListSortSign( int id, int type )
{
	if( m_inpview != NULL ) {
		if( id == -1 ) {
			((CDBDinpView*)m_inpview)->SwkListSortClear();
		}
		else {
			((CDBDinpView*)m_inpview)->SwkListSortSign( id, type );
		}
	}
}

void CMainFrame::GetSwkListSortSign( int& id, int& type )
{
	if( m_inpview != NULL ) {
		((CDBDinpView*)m_inpview)->GetSwkListSortSign( id, type );
	}
}

/////////////////////////////////////////////////////////////////////////////
// 進捗記録

// kind ... 0:処理終了時　1:ツールバーから
void CMainFrame::ShicyokuKiroku( int kind )
{
	if( ! m_IsShincyoku )	return;

	char		cnam[64] = {0};
	ProgData	data;

	CWnd*	pWnd; /* = theApp.GetDocumentTemplate()->GetFrameWnd( 0 );*/

	if( m_nowvw_flg == m_SCANview_id ) {
		pWnd = (CWnd*)m_scanview;
	}
	else if( m_nowvw_flg == m_INPview_id ) {
		pWnd = (CWnd*)m_inpview;
	}
	else if( m_nowvw_flg == m_SKEYview_id ) {
		pWnd = (CWnd*)m_skeyview;
	}
	else if( m_nowvw_flg == m_ACBKview_id ) {
		pWnd = (CWnd*)m_acbkview;
	}
	else if( m_nowvw_flg == m_DNPview_id ) {
		pWnd = (CWnd*)m_dnpview;
	}

	// 初期化済みパケット取得
	data = GetInitProgData();

	strcpy_s( cnam, sizeof(cnam), pDBzm->zvol->c_nam );

#ifdef CLOSE
	LstKjsCut( cnam, data.Komon_Nam, pDBzm->zvol->c_nam.GetLength() );
#else
	// 後ろスペースをカット
	int n = kjlen( cnam, sizeof(cnam) );
	*(cnam + n) = '\0';
	memcpy( data.Komon_Nam, cnam, sizeof data.Komon_Nam );
#endif

	strcpy_s( data.pd_msid, sizeof(data.pd_msid), _T("MS001070") );		//	明細区分ID：

	data.pd_jigyo_symd = pDBzm->zvol->ss_ymd;	//事業年度　自yyyymmdd
	data.pd_jigyo_eymd = pDBzm->zvol->ee_ymd;	//事業年度　至yyyymmdd

	if( kind ) {	// ツールバーから
		ProgTuutiGen( pWnd, &data );
	}
	else {			// 処理終了時
		PROGRAM_OK = FALSE;
		int st = ProgEndTuuti( pWnd, &data );
		PROGRAM_OK = TRUE;
	}
}

void CMainFrame::OnButtonShicyoku()
{
	ShicyokuKiroku( 1 );
}

void CMainFrame::OnUpdateShicyoku(CCmdUI *pCmdUI)
{
	if( !m_IsShincyoku ) {	// 進捗管理を「行わない」
		pCmdUI->Enable( FALSE );
	}
}


void CMainFrame::GetShincyokuFlag()
{
FILETrace( "GetShincyokuFlag TOP\n" );

//===========================================================================
// 進捗管理対応
	// 進捗管理を使用しない設定かをチェック
	m_IsShincyoku = GetShincyokuAdminSign( 1 );	// 引　数：1＝進捗管理を「行う⇔行わない」情報を取得
												// 返送値：1＝進捗管理を「行う」
												// 　　　　0＝進捗管理を「行わない」
//===========================================================================

FILETrace( "GetShincyokuFlag = %d\n", m_IsShincyoku );

//	if( !(mkt & KIND_DAYREPRT) ) {
	if( m_IsShincyoku ) {
		CProgress_Mod	pmod;
		pmod.CProgress_Iniz();

FILETrace( "GetShincyokuFlag ---> CProgress_Iniz OK\n" );
		int mkt = pmod.GetMakeOption();
FILETrace( "GetShincyokuFlag ---> GetMakeOption OK\n" );
		pmod.CProgress_end();
FILETrace( "GetShincyokuFlag ---> CProgress_end OK\n" );

		if( mkt != KIND_PROGRES );
		else {
			m_IsShincyoku = 0;
		}
FILETrace( "GetShincyokuFlag = %d, mkt = %d\n", m_IsShincyoku, mkt );
#ifdef CLOSE
FILETrace( "GetShincyokuFlag CProgress_Mod 使用していない。\n" );
#endif
	}

#ifdef CLOSE // 06.24 /13
	if( pDBzm->zvol->apno >= 0x50 ) {
		m_IsShincyoku = 0;
	}
#endif
}

void CMainFrame::OnHelpDisp()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	OnHelpDisplay();
}



BOOL CMainFrame::GetGensyoDBsign() 
{
	BOOL bRet = FALSE;

	if( pDBzm == NULL )
		return FALSE;

	if( pDBzm->zvol == NULL )
		return FALSE;

	if( bCONFIRM_MASTER )
		return FALSE;

	if( m_bGensyoDB ) {
		if( pDBzm->zvol->apno >= 0x50 ) {
			return FALSE;
		}
		if( pDBzm->zvol->sub_sw & 0x10000 ) {
			bRet = TRUE;
		}
	}

	return bRet;
}


//　下記の４つの条件を満たす場合確定仕訳の誘導メッセージボックスを出す
// 1.クラウドライセンス有り
// 2.事務所側
// 3.上手くんαクラウド共有マスター
// 4.オプションにより　仕訳確定のダイアログを出さないにしていない
// 5.会社選択バーからの会社選択中でない場合
//	return 0: 表示しない。1:表示する

int CMainFrame::IsCloudEndMsg()
{
	int st = 1;
//AfxMessageBox( "IsCloudEndMsg test return 1" );
//return st;

	//顧問先でなく、共有マスターの場合
	if( ! IsCloudSetupEnv() )	return 0;

	if( JZcloudEndKakutei() )	return 0;

	if( pDBzm->m_dacnt == 0 )
		return 0;

	if( bCONFIRM_MASTER )	return 0;

	//会社選択バーからの会社選択中の場合、表示しない
	if( bReCOSEL )	return 0;

	return st;
}


void CMainFrame::ChainProcess()
{
	CString str_prog, str_command;
	int		wMode;

	str_prog.Format( "%C:\\ICSWin\\Cmds\\GR%02x\\JOBS10\\DecChk8.exe", ::GET_PROGRAMDEVICE(), ::GET_GRNO() );
	str_command = "-cloud";
	
	if( IsZoomed())
		wMode = SW_SHOWMAXIMIZED;
	else
		wMode = SW_SHOWNORMAL;

	if( ::ICSChain( m_hWnd, str_prog, str_command, wMode ) == 0 )	{
	}
	else{
		ICSMessageBox("仕訳確定起動に失敗しました。\n財務dbの導入・更新より仕訳確定業務を行ってください。");
	}	
}

//
// return TRUE クラウド共有マスターで仕訳確定業務を行う。
//
BOOL CMainFrame::CloudEndChainJob()
{
	if(IsCloudEndMsg()){
		CString str;
		str.Format("%s", "仕訳確定処理を行いますか？");

		int chg_sgn = 0;
		if( PROGRAM_OK ) {
			PROGRAM_OK = FALSE;
			chg_sgn = 1;
		}

		int st = ICSExtBox( "", str, _T( "仕訳確定(&S)" ), _T( "いいえ(&N)" ),  _T( "" ), MB_ICONEXCLAMATION, 0, 0, this );

		if( chg_sgn ) {
			PROGRAM_OK = TRUE;
		}

		if( st == IDYES ) { //　仕訳確定へ→仕訳入力は終了
		//	ChainProcess();	//　仕訳確定の呼び出し
			bCloudEnd = TRUE;
			return	TRUE;
		}
	}
	return FALSE;
}

void CMainFrame::ToolBarClear()
{
	PostMessage( WM_CHGTOOLBAR, 999 );
}

void CMainFrame::ToolBarClearFunc()
{
	ICSToolBar* pBar = GetToolBar( 1 );
	int max = pBar->GetButtonCount();
//	int max = sizeof ToolTitle9 / sizeof ToolTitle9[0];
	char* p = "   　　　　";

	for( int i = 0; i < max-1; i++ ) {
		pBar->SetButtonTooltip(i, "" );
		pBar->SetButtonText( i, p );
		pBar->SetButtonDescription( i, "" );
	}
}


void CMainFrame::CurrentDirCheck()
{
	CString dir;
	const int bufsz = 1024;

	LPSTR	ptr = dir.GetBuffer( bufsz );
	GetCurrentDirectory( bufsz, ptr );

	dir.ReleaseBuffer();

	if( dir.CompareNoCase( GetObjsPath() ) != 0 ) {
		SetCurrentDirectory( GetObjsPath() );
	}
}


void CMainFrame::GetMouseWheelRouting()
{
	const char* MouseKey = "Control Panel\\Desktop";

	CRegKey	reg;
	int st = reg.Open(HKEY_CURRENT_USER, MouseKey, KEY_READ);

	DWORD dwVal = 0;

	LSTATUS ret = reg.QueryDWORDValue("MouseWheelRouting", dwVal);

	reg.Close();

//	MyTrace("GetMouseWheelRouting ret(%d), dwVal = %d", ret, dwVal);

	m_MouseRouting = dwVal;
}


//伝票形式検索時のメッセージ
BOOL CMainFrame::ScanDenpCheck()
{
	if( ! ((CDnpInView*)m_dnpview)->EndChk(1) )
		return FALSE;

	return TRUE;
}

// ReadOCR からのFork起動か
void CMainFrame::fnc_fromReadOCR()
{
	char buf1[128], buf2[128], buf3[256];

	// Fork起動か？
	PCOM_GetString("ProgramType", buf1);

	if( strcmp(buf1, "Fork") == 0 ) {

		PCOM_GetString("GrTitle", buf1);
		CString stmp;
		stmp = buf1;
		int chk = 0;

		if( stmp.Find("ＯＣＲ処理ｄｂ") != -1 ) {
			chk++;
		}
		PCOM_GetString("ExeTitle", buf2);
		stmp = buf2;
		if( stmp.Find("原票読取") != -1 ) {
			chk++;
		}
		else if( stmp.Find("カード明細/通帳読取") != -1 ) {
			chk++;
		}
		else if( stmp.Find("証憑/カード明細/通帳") != -1 ) {
			chk++;
		}
		else if( stmp.Find("汎用") != -1 ) {
			chk++;
		}

		UCOM_GetStringAll("例外業務1", buf3, sizeof buf3);
		stmp = buf3;
		if( stmp.Find("DBDinp.exe") != -1 ) {
			chk++;
		}
		if( chk >=3 ) {
			FromReadOCR() = TRUE;
		}
	}
//	MyTrace("fnc_fromReadOCR m_formReadOCR = %d\n", m_formReadOCR);
}


void CMainFrame::set_DocShtypCombo()
{
	if( m_skeyview == NULL )
		return;

	((CScanKeyView*)m_skeyview)->set_DocShtypCombo();
}


//#define WIZ_CLOSE	1


int CMainFrame::WizRenkeiMasterCheck()
{
	m_bWIZMaster = FALSE;
	//CString emsg;
	//emsg.Format("%s, (%d)\n", __FUNCTION__, __LINE__ );
	//AfxMessageBox(emsg);

	//上手くん参照モード は下でメッセージを表示する
	//if( IsJzSansyo() )
	//	return 0;

	if( !IsBuyWizOption() )
		return 0;

	CString msg;

	if( InvalidWizOptionFlg() > 1 ) {
		msg.Format("%sとの連携情報が不正な状態になっているため、\r\n連携情報をリセットします。\r\n請求書管理オプションとマスターの連携設定は\r\n新規会社登録・修正・削除で行ってください。", WizTitle);
		myICSMessageBox(msg, 0, 0, 0, this);

		try {
			ResetWizOptionFlg();
		}
		catch( CErrBlk E )
		{
			ErrExit(E.ErrCode, E.ErrMsg);
			return -1;
		}
		return 0;
	}
	if( IsMasterWizRenkei(Voln1->apno, Voln1->v_cod) ) {
		m_bWIZMaster = TRUE;
	}
	else {
#ifdef PH1_5_CLOSE	//請求書との連携は会社情報登録で行う
		if( IsWizRenkeiExist(Voln1->apno, Voln1->v_cod) )
			return 0;

		//原票なしのマスターは連携不可
		if( !(Voln1->sub_sw & 0x40) ) {
			return 0;
		}

		msg.Format("選択された会社を%sと連携しますか？", WizTitle );
		int st = myICSMessageBox(msg, MB_YESNO | MB_DEFBUTTON2, 0, 0, this);

		if( st == IDYES ) {
			try {
				if( RenkeiWizMaster(Voln1->apno, Voln1->v_cod) == 0 ) {
					m_bWIZMaster = TRUE;
				}
				else {
					//エラー
					return -1;
				}
			}
			catch( CErrBlk E )
			{
				ErrExit(E.ErrCode, E.ErrMsg);
				return -1;
			}
		}
#endif
	}
	return 0;
}


//請求書管理オプションのデータ処理
// 返送値	0:  連携しない
//			1:　連携データダウンロードした
//			-1: エラー
int CMainFrame::WizDataDownLoadJob()
{
	MyTrace("■WizDataDownLoadJob top = %d\n", m_bWIZMaster);
	if( !m_bWIZMaster )	return 0;

	CString msg;
	// 原票会計 未購入で 原票イメージ保存 行う のマスターはここでドキュメントを開ける
	if( !bSCANSAVE_Master ) {
		if( Voln1->sub_sw & 0x40 ) {
			if( OpenDocDBMaster(CBlkIns::pcom_sqlsvr, CBlkIns::pcom_apno, CBlkIns::pcom_cocode, CBlkIns::pcom_kikan) == -1 )
				return -1;
		}
	}
	if( pAddInv == NULL ) {
#ifndef WIZ_CLOSE
		try {
			if( (pAddInv = new CAddInvData) == NULL ) {
				ermset(ERROR_MEMORY, "請求書データ取得クラスオブジェクトを作成できません！");
				return -1;
			}
			if( pAddInv->Init(pDBzm, pDCsb) != 0 ) {
				//このエラーはトークン取得エラー(設定されていない）なので、何も表示せずに戻る
			//	ermset(-1, (char*)(LPCTSTR)pAddInv->GetErrorMessage());
				return 0;
			}
		}
		catch( CErrBlk E )
		{
			ErrExit(E.ErrCode, E.ErrMsg);
			return -1;
		}
#else
		AfxMessageBox("仕訳連携モジュールは作成していない！");
#endif
	}

	int st = pAddInv->AddInvPing();
	if( st == -1 || st == -2 ) {
		if( st == -1 ) {
			msg.Format("受領請求書の疎通確認に失敗しました。");
		}
		else{
			msg = pAddInv->GetErrorMessage();
		}
		myICSMessageBox(msg, MB_OK, 0, 0, this);
		//エラー
		return -1;
	}
	//プログレス用のウィンドウセット
	pAddInv->SetProgressMessageWnd(this);

	int datacnt = pAddInv->GetInvoiceDataCount();
	int Trandatacnt = pAddInv->GetTranDataCount();

	MyTrace("■WIZ datacnt = %d, Trandatacnt = %d\n", datacnt, Trandatacnt);

	if( datacnt == -1 || Trandatacnt == -1 ) {
		msg.Format("受領請求書の取込対象仕訳データの確認に失敗しました。\r\n%s", pAddInv->GetErrorMessage());
		myICSMessageBox(msg,MB_OK, 0,0,this);
		//エラー
		return -1;
	}
	datacnt += Trandatacnt;

	MyTrace("■WIZ 合計データ数 = %d\n", datacnt);

	if( datacnt == 0 )
		return 0;

	//他にデータベースのセッションがあるプロセスを探す
	st = DBsessionCheck();
	MyTrace("■WIZ 他DBセッション = %d\n", st );

	if( st == -1 )	return -1;	//エラー
	if( st == 1 )	return 0;	//他のDBセッションあり

	//ロックする
	char	sqlsvr[256], apno[20], cocode[20], kikan[20];
	int hdl, lmode;
	lmode = 2;	//排他
	if( PCOM_GetString("SELECTCODSN", sqlsvr, apno, cocode, kikan) == 4 ) {
		co_unlockx();
		hdl = co_lock_ms( atoi(apno), atoi(cocode), atoi(kikan), lmode);
	}
	else {
		hdl = -2;
	}
	if( hdl <= -1 ) {
		return -1;
	}
	//決算確定、上手君参照モード
	if( bCONFIRM_MASTER ) {
		CString str;
		if( bCONFIRM_MASTER & 0x100 ) {
			GetConfirmString(str);
			msg.Format("選択されたマスターは%sのため、\r\n取込対象仕訳データを取得できません。", str);
		}
		else {
			str = "決算確定";
			msg.Format("選択されたマスターは%sしているため、\r\n取込対象仕訳データを取得できません。", str);
		}
		myICSMessageBox(msg);
		co_unlock(hdl);
		return 0;
	}
	msg = "取込対象仕訳データが存在します。\r\n取込を行いますか？";
	st = myICSMessageBox(msg, MB_YESNO | MB_DEFBUTTON2, 0, 0, this);

	if( st != IDYES ) {
		co_unlock(hdl);
		return 0;
	}

	m_prog = NULL;
	if( (m_prog = new ICSProgressWnd()) == NULL ) {
		return -1;
	}
	m_prog->SetParent(this);
	m_prog->SetTitle("仕訳入力【受領請求書】");
	m_prog->SetText("受領請求書の取込対象仕訳データを取得しています");
	m_prog->SetProgressRange(0, datacnt);
	m_prog->BeginProgress();

	PROGRAM_OK = FALSE;
	IgnoreOnSizeFlag() = TRUE;;

	pDBzm->ExitMode(1);
	//仕訳データ作成
#ifndef WIZ_CLOSE
	st = pAddInv->InvoiceDataAppend();
#else
	Sleep(5000);
	st = -1;
#endif
	pDBzm->ExitMode(0);

	m_prog->EndProgress();

	MyTrace("■WIZ InvoiceDataAppend 返送値 = %d\n", st);

	if( st == -1 ) {
		CString msg;
		msg.Format("受領請求書の取込対象仕訳データの取得に失敗しました。\r\n%s", pAddInv->GetErrorMessage());
		myICSMessageBox(msg, MB_OK, 0, 0, this);
		//マスタークローズ
		if( m_inpview != NULL ) {
			((CDBbaseView*)m_inpview)->CloseWork();
			((CDBbaseView*)m_skeyview)->CloseWork();
			((CDBbaseView*)m_acbkview)->CloseWork();
			((CDBbaseView*)m_dnpview)->CloseWork();
			((CScanView*)m_scanview)->ScanCloseWork();
		}
		mst_close(1);
		m_prog->SetTitle("仕訳入力【受領請求書】");
		m_prog->SetText("仕訳データを復帰しています・・・");
		m_prog->SetProgressRange(0, 0);
		m_prog->BeginProgress();
		//エラーの場合、マスターを元に戻す
		if( pAddInv->RestoreMaster() == -1 ) {
			m_prog->EndProgress();
			delete m_prog;
			m_prog = NULL;
			co_unlock(hdl);
			ErrExit(0, "マスターリストア処理でエラーが発生しました。");
			return -1;
		}

		m_prog->SetText("仕訳データを読み込んでいます・・・");
		try {
			init_open();
			mst_open();
			// 選択クラスへのデータをセットする
			CDBbaseView::m_bSelDataInit = FALSE;
			if( ((CDBDinpView*)m_inpview)->BeginApp(1) != 0 ) {
				ermset(0, "会社再選択時の初期化処理でエラーが発生しました！");
			}
			// SREC の情報をセットする。
			((CDBDinpView*)m_inpview)->ReCosel_DinpData();

			// 選択モジュールへの ウィンドウセット
			switch( m_nowvw_flg ) {
			case INP_VIEW:		((CDBDinpView*)m_inpview)->SetSelectWindow();	break;
			case SKEY_VIEW:		((CDBbaseView*)m_skeyview)->SetSelectWindow();	break;
			case DNP_VIEW:		((CDBbaseView*)m_dnpview)->SetSelectWindow();	break;
			case ACBK_VIEW:		((CDBbaseView*)m_acbkview)->SetSelectWindow();	break;
			}

			((CScanKeyView*)m_skeyview)->BeginApp();
			((CDnpInView*)m_dnpview)->BeginApp();
			((CDBAbookView*)m_acbkview)->BeginApp();
			((CDBAbookView*)m_acbkview)->ReCosel_Job();
			((CScanView*)m_scanview)->OnInitialize("");
		}
		catch( CErrBlk E )
		{
			m_prog->EndProgress();
			delete m_prog;
			m_prog = NULL;
			co_unlock(hdl);
			ErrExit(E.ErrCode, E.ErrMsg);
			IgnoreOnSizeFlag() = FALSE;
			return -1;
		}
		m_prog->EndProgress();
		delete m_prog;
		m_prog = NULL;
		co_unlock(hdl);
		PROGRAM_OK = TRUE;
		IgnoreOnSizeFlag() = FALSE;

		return -1;
	}
	else {
		delete m_prog;
		m_prog = NULL;

		pDBzm->insseq_make() = FALSE;
		DBDinpAppendParamReload(pDBzm->m_dacnt, pDBzm->m_dacnt+ datacnt);
		CString filter;
		filter = _T("seq = 1");
		pDBzm->dcntl->Requery(filter);
		if( pDBzm->dcntl->st != -1 ) {
		pDBzm->m_dacnt = pDBzm->dcntl->cnt;
		}
		//入力行 追加ＳＥＱ分 更新
		((CDBDinpView*)m_inpview)->WizRenkeiEndJob();
	}
	co_unlock(hdl);

	PROGRAM_OK = TRUE;
	IgnoreOnSizeFlag() = FALSE;

	return 1;
}


//WIZ ダウンロードデータで 検索一覧にフォーク
void CMainFrame::ChgToScanViewWizData()
{
	//常に全仕訳
	scanitem_initEX(&SC_Item);
	pDBzm->IsDelScan() = FALSE;
	pDBzm->IsDupliScan() = FALSE;
	pDBzm->IsWizScan() = TRUE;

	CString filter;

	if( m_nowvw_flg != SCAN_VIEW )
	{
		if( m_nowvw_flg == INP_VIEW ) {
			//入力または行戻りの修正はモードをセーブしておく
			if( INP_mode == _APPEND || INP_mode == _MODEFY )
			{
				((CDBDinpView*)m_inpview)->_save_inpcondition();
				((CDBDinpView*)m_inpview)->SaveDinpPara(INP_mode);
			}
		}
		((CDBDinpView*)m_inpview)->HideDispImgDlg(TRUE);

		m_prevw_flg = m_nowvw_flg;
		m_nowvw_flg = SCAN_VIEW;
		SetActiveChild(m_SCANview_id);
		DnpInMouseUnhook();

		((CScanView*)m_scanview)->UpdateDataLineDisp();
		((CScanView*)m_scanview)->scan_init_wizread(filter);

		ChangeToolBar(3, 0);
	}
	else {
		((CScanView*)m_scanview)->scan_init_wizread(filter);
	}
}


// Wiz連携マスターOFF
void CMainFrame::SetWizMasterOFF()
{
	if( ResetMasterWizOptionFlg() == 1 ) {
		m_bWIZMaster = FALSE;
	}
}


LRESULT CMainFrame::OnWmAddInvMsg(WPARAM wParam, LPARAM lParam)
{
	if( m_prog == NULL )
		return 1;

	if( lParam > 0 ) {
		m_prog->SetProgressPos(lParam);
	}
	m_prog->SetText((LPCSTR)wParam );
	m_prog->Refresh();

	return 1;
}


