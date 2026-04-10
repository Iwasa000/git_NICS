//--------------------------------------------------
//	frmUc041Karibarai.cpp
//--------------------------------------------------

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "frmUc041Karibarai.h"
#include "frmUc042Kasituke.h"		// 通常/個別切り替えにて"④-2"データ更新が必要
// midori 190301 add -->
#include "frmUc042KasitukeK.h"		// 通常/個別切り替えにて"④-2"データ更新が必要

extern BOOL	bG_Kanso;
// midori 190301 add <--

// 改良No.21-0086,21-0529 cor -->
//// No.200905 add -->
//extern	int KamokuRowEnableSgn(CDatabase* pDB, int pSw);
//// No.200905 add <--
// ------------------------------
extern	int KamokuRowEnableSgn(CDatabase* pDB, int pSw, int nFormSeq);
// 改良No.21-0086,21-0529 cor <--

IMPLEMENT_DYNCREATE( CfrmUc041Karibarai, CfrmUc000Common )


BEGIN_MESSAGE_MAP( CfrmUc041Karibarai, CfrmUc000Common )
	ON_WM_SIZE()
//	ON_COMMAND( ID_BUTTON_F1, OnButtonF1)
	ON_COMMAND( ID_BUTTON_F2, OnButtonF2 )
//	ON_COMMAND( ID_BUTTON_F3, OnButtonF3 )
	ON_COMMAND( ID_BUTTON_F4, OnButtonF4 )
	ON_COMMAND( ID_BUTTON_F5, OnButtonF5 )
	ON_COMMAND( ID_BUTTON_F6, OnButtonF6 )
	ON_COMMAND( ID_BUTTON_F7, OnButtonF7 )
	ON_COMMAND( ID_BUTTON_F8, OnButtonF8 )
	ON_COMMAND( ID_BUTTON_F9, OnButtonF9 )
	ON_COMMAND( ID_BUTTON_F11, OnButtonF11 )
	ON_COMMAND( ID_BUTTON_F12, OnButtonF12 )
	ON_COMMAND( ID_BUTTON_HOME, OnButtonHome )
	ON_COMMAND( ID_BUTTON_INSERT, OnButtonInsert )
	//ON_COMMAND( ID_BUTTON_END, OnButtonEnd )
	ON_COMMAND( ID_PRINTSETUP_MENU, &CfrmUc041Karibarai::OnMenuPrintSetup )
	ON_COMMAND( ID_PRINTALL_MENU, &CfrmUc041Karibarai::OnMenuPrintAll )
	ON_COMMAND( ID_OUTPUTFORM_MENU, &CfrmUc041Karibarai::OnMenuFormTypeSetup )
	ON_COMMAND( ID_SPECIALROW_MENU, &CfrmUc041Karibarai::OnMenuSpecialRow )
	ON_COMMAND( ID_SORT_MENU, &CfrmUc041Karibarai::OnMenuSort )
	ON_COMMAND(ID_IKKATUADD_MENU, &CfrmUc041Karibarai::OnIkkatuAddMenu)
	ON_COMMAND( ID_DISPDIALOG_MENU, &CfrmUc041Karibarai::OnMenuIkkatuKingaku )
	ON_COMMAND( ID_ZEROMONEY_MENU, &CfrmUc041Karibarai::OnMenuZeroMoney )
	ON_COMMAND( ID_ZEROMONEYMOVE_MENU, &CfrmUc041Karibarai::OnMenuZeroMoneyMove )
	ON_COMMAND( ID_AMOUTDIALOG_MENU, &CfrmUc041Karibarai::OnMenuOutKeiSetup )
// midori 160606 add -->
	ON_COMMAND( ID_CLEAR_MENU, &CfrmUc041Karibarai::OnMenuClear )
// midori 160606 add <--
	ON_COMMAND( ID_RENDO_MENU, &CfrmUc041Karibarai::OnMenuZaimuRendo )
	ON_COMMAND( ID_TITLEMENU, &CfrmUc041Karibarai::OnMenuTitleRestore )
	ON_COMMAND( ID_USER_SETTING_MENU, &CfrmUc041Karibarai::OnMenuOptionSetting )
	ON_COMMAND( ID_COPY_SETTING_INFO_MENU, &CfrmUc041Karibarai::OnMenuCopySettingInfo )
	ON_COMMAND( ID_DELETE_DATA_MENU, &CfrmUc041Karibarai::OnMenuDeleteDataEachYoushiki )
	ON_BN_CLICKED( IDC_PAGEBACK_BUTTON1, &CfrmUc041Karibarai::OnPageBack )
	ON_BN_CLICKED( IDC_PAGENEXT_BUTTON1, &CfrmUc041Karibarai::OnPageNext )
	ON_BN_CLICKED( IDC_PAGENEW_BUTTON1, &CfrmUc041Karibarai::OnPageNew )
	ON_BN_CLICKED( IDC_PAGEINSERT_BUTTON1, &CfrmUc041Karibarai::OnPageInsert )
	ON_BN_CLICKED( IDC_ROWCOPY_BUTTON1, &CfrmUc041Karibarai::OnRowCopy )
	ON_BN_CLICKED( IDC_ROWPASTE_BUTTON1, &CfrmUc041Karibarai::OnRowPaste )
	ON_BN_CLICKED( IDC_ROWINSERT_BUTTON1, &CfrmUc041Karibarai::OnRowInsert )
	ON_BN_CLICKED( IDC_YOUSHIKINEXT_BUTTON1, &CfrmUc041Karibarai::OnMoveYoushikiNext )
	ON_BN_CLICKED( IDC_YOUSHIKIBACK_BUTTON1, &CfrmUc041Karibarai::OnMoveYoushikiBack )
//	ON_UPDATE_COMMAND_UI( ID_BUTTON_F1, &CfrmUc041Karibarai::OnUpdateButtonF1 )
//	ON_UPDATE_COMMAND_UI( ID_BUTTON_F3, &CfrmUc041Karibarai::OnUpdateButtonF3 )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_F6, &CfrmUc041Karibarai::OnUpdateButtonF6 )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_F7, &CfrmUc041Karibarai::OnUpdateButtonF7 )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_F8, &CfrmUc041Karibarai::OnUpdateButtonF8 )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_F9, &CfrmUc041Karibarai::OnUpdateButtonF9 )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_F11, &CfrmUc041Karibarai::OnUpdateButtonF11 )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_F12, &CfrmUc041Karibarai::OnUpdateButtonF12 )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_HOME, &CfrmUc041Karibarai::OnUpdateButtonHome )
	ON_UPDATE_COMMAND_UI( ID_IKKATUADD_MENU, &CfrmUc041Karibarai::OnUpdateIkkatuaddMenu)
	ON_UPDATE_COMMAND_UI( ID_DISPDIALOG_MENU, &CfrmUc041Karibarai::OnUpdateMenuIkkatuKingaku )
// midori 160612 add -->
	ON_UPDATE_COMMAND_UI( ID_SORT_MENU, &CfrmUc041Karibarai::OnUpdateMenuSort )
	ON_UPDATE_COMMAND_UI( ID_CLEAR_MENU, &CfrmUc041Karibarai::OnUpdateMenuClear )
// midori 160612 add <--
	ON_UPDATE_COMMAND_UI( ID_ZEROMONEY_MENU, &CfrmUc041Karibarai::OnUpdateMenuZeroMoney )
// midori 160612 add -->
	ON_UPDATE_COMMAND_UI( ID_ZEROMONEYMOVE_MENU, &CfrmUc041Karibarai::OnUpdateMenuZeroMoneyMove)
// midori 160612 add <--
	ON_UPDATE_COMMAND_UI( ID_RENDO_MENU, &CfrmUc041Karibarai::OnUpdateMenuZaimuRendo )
	ON_UPDATE_COMMAND_UI(ID_SPECIALROW_MENU, &CfrmUc041Karibarai::OnUpdateMenuSpecialRow)
// midori 160612 add -->
	ON_UPDATE_COMMAND_UI( ID_TITLEMENU, &CfrmUc041Karibarai::OnUpdateMenuTitleRestore )
	ON_UPDATE_COMMAND_UI( ID_DELETE_DATA_MENU, &CfrmUc041Karibarai::OnUpdateMenuDeleteDataEachYoushiki )
// midori 160612 add <--
	ON_MESSAGE(WM_USER_REDRAWVIEW, OnUserReDrawView)		// 修正No.162446 add
END_MESSAGE_MAP()


CfrmUc041Karibarai::CfrmUc041Karibarai()
	: CfrmUc000Common( CfrmUc041Karibarai::IDD )
{
	m_pDB = ( (CUCHIWAKEApp*)AfxGetApp() )->m_pDB;
	m_pTitleDiag = &m_TitleDiag;
// midori 152718 add -->
	m_pPageDiag	= &m_PageDiag;
// midori 152718 add <--
}

CfrmUc041Karibarai::~CfrmUc041Karibarai()
{
	// 2009.09.08 TS自動ログオフ対応(virEndProcへ移動)
	TRACE(_T("***** CfrmUc041Karibarai::~CfrmUc041Karibarai\n"));

	//m_pDiag = NULL;
	//m_pTitleDiag = NULL;
}

void CfrmUc041Karibarai::DoDataExchange( CDataExchange* pDX )
{
	CfrmUc000Common::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_YOUSHIKIDIAG1, m_DiagNormal );
	DDX_Control( pDX, IDC_YOUSHIKIDIAG2, m_DiagKobetu );
	DDX_Control( pDX, IDC_YOUSHIKIDIAG3, m_DiagKobetu2 );
	DDX_Control( pDX, IDC_YOUSHIKIDIAG4, m_DiagKobetu3 );
	DDX_Control( pDX, IDC_YOUSHIKIDIAG_KEI1, m_DiagKei1 );
	DDX_Control( pDX, IDC_YOUSHIKIDIAG_KEI2, m_DiagKei2 );	
	DDX_Control( pDX, IDC_TITLEDIAG1, m_TitleDiag );
	DDX_Control( pDX, IDC_PAGEDIAG1, m_PageDiag );
	DDX_Control( pDX, IDC_NUMBER_DIAG1, m_NumberDiag );
	DDX_Control( pDX, IDC_TOPICDIAG1, m_HeadDiag);
// midori 161108 add -->
	DDX_Control(pDX, IDC_SAVEDATA_DIAG1, m_SaveDataDiag);
// midori 161108 add <--
// 修正No.157626 add -->
	DDX_Control(pDX, IDC_ICSDISPGUIDE1, m_Guide1);
// 修正No.157626 add <--
}

void CfrmUc041Karibarai::OnInitialUpdate()
{
	CfrmUc000Common::OnInitialUpdate();
	ResizeParentToFit();

	// 帳表テーブルのオープンOK?
	if(m_InitialFlag == TRUE)	{
		// コンボボックスアイテムのリサイズ制御
		m_DiagNormal.ComboxFontMode(TRUE);
		m_DiagKobetu.ComboxFontMode(TRUE);
		m_DiagKobetu2.ComboxFontMode(TRUE);
		m_DiagKobetu3.ComboxFontMode(TRUE);

		// リサイズ
		//SetResize( IDC_YOUSHIKIDIAG1 , ICS_RESIZE_BOTTOM );
		//SetResize( IDC_YOUSHIKIDIAG2 , ICS_RESIZE_BOTTOM );

		// 背景色を取得
		m_ViewColor = CmnGetBackColor();

// 修正No.158252 add -->
		// 欄外ガイドの背景色
		m_Guide1.SetBackColor(m_ViewColor);
		m_Guide1.ShowWindow(FALSE);
// 修正No.158252 add <--

		// 帳表表示処理OK?
		if(virStartJob() == TRUE)	{
			// 最小化／最大化ボックスを無効に設定
//			CmnEnableMinMaxBox( GetParent() );

			//*********************************************************************
			// 問題点の対応：2009/08/19 ICS様よりお電話にて
			//	・通常／個別切り替えで、画面項目のフォントが変わる
			//		（⑩-1.仮受金、所在地項目で発生）
			//
			//【原因】
			//	起動時に表示する方のDiag(通常 or 個別)は、virStartJobでセットした設定値(桁数等)でリサイズされる
			//	だが、表示しない方のDiag(通常 or 個別)は、リソースの設定値(桁数等)でリサイズされるため違いが出ていた
			//	※OnInitialUpdateより後に桁数をセットしてもリサイズには反映されない
			//
			//【対応】
			//　表示していない方のDiagも、リサイズに影響する桁数等だけは起動時(OnInitialUpdate)にセットしておく
			//	※通常/個別の帳表は全部対応しておく
			//
			//*********************************************************************

			// 表示していない方のDiagの初期設定を行う(桁数など)
			InitialControlDataAreaHide( m_pDiagHide );		// データ行
			InitialControlTotalAreaHide( m_pDiagHide );		// 頁計/累計行
			m_pDiagHide->Refresh();							// 更新
		}
		else	{
			// カーソルフラグ=FALSE(この後フォーカスセット処理が走らないように)
			m_CursorSetFlag = FALSE;
		}
	}
}

#ifdef _DEBUG
void CfrmUc041Karibarai::AssertValid() const
{
	CfrmUc000Common::AssertValid();
}

#ifndef _WIN32_WCE
void CfrmUc041Karibarai::Dump( CDumpContext& dc ) const
{
	CfrmUc000Common::Dump( dc );
}
#endif
#endif //_DEBUG

BEGIN_EVENTSINK_MAP( CfrmUc041Karibarai, CfrmUc000Common )
	ON_EVENT( CfrmUc041Karibarai, IDC_YOUSHIKIDIAG1, 3, CfrmUc041Karibarai::TerminationYoushikidiag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN )
	ON_EVENT( CfrmUc041Karibarai, IDC_YOUSHIKIDIAG1, 2, CfrmUc041Karibarai::EditOFFYoushikidiag1, VTS_I2 )
	ON_EVENT( CfrmUc041Karibarai, IDC_YOUSHIKIDIAG1, 1, CfrmUc041Karibarai::EditONYoushikidiag1, VTS_I2 )
// midori 151405 add -->
	ON_EVENT(CfrmUc041Karibarai, IDC_YOUSHIKIDIAG1, 12, CfrmUc041Karibarai::VScrollYoushikidiag1, VTS_I2)
// midori 151405 add <--
// midori 152134 add -->
	ON_EVENT(CfrmUc041Karibarai, IDC_YOUSHIKIDIAG1, 8, CfrmUc041Karibarai::ComboSelYoushikidiag1, VTS_I2 VTS_UNKNOWN)
// midori 152134 add <--

	ON_EVENT( CfrmUc041Karibarai, IDC_YOUSHIKIDIAG2, 3, CfrmUc041Karibarai::TerminationYoushikidiag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN )
	ON_EVENT( CfrmUc041Karibarai, IDC_YOUSHIKIDIAG2, 2, CfrmUc041Karibarai::EditOFFYoushikidiag1, VTS_I2 )
	ON_EVENT( CfrmUc041Karibarai, IDC_YOUSHIKIDIAG2, 1, CfrmUc041Karibarai::EditONYoushikidiag1, VTS_I2 )
// midori 151405 add -->
	ON_EVENT(CfrmUc041Karibarai, IDC_YOUSHIKIDIAG2, 12, CfrmUc041Karibarai::VScrollYoushikidiag1, VTS_I2)
// midori 151405 add <--
// midori 152134 add -->
	ON_EVENT(CfrmUc041Karibarai, IDC_YOUSHIKIDIAG2, 8, CfrmUc041Karibarai::ComboSelYoushikidiag1, VTS_I2 VTS_UNKNOWN)
// midori 152134 add <--

	ON_EVENT( CfrmUc041Karibarai, IDC_YOUSHIKIDIAG3, 3, CfrmUc041Karibarai::TerminationYoushikidiag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN )
	ON_EVENT( CfrmUc041Karibarai, IDC_YOUSHIKIDIAG3, 2, CfrmUc041Karibarai::EditOFFYoushikidiag1, VTS_I2 )
	ON_EVENT( CfrmUc041Karibarai, IDC_YOUSHIKIDIAG3, 1, CfrmUc041Karibarai::EditONYoushikidiag1, VTS_I2 )
// midori 151405 add -->
	ON_EVENT(CfrmUc041Karibarai, IDC_YOUSHIKIDIAG3, 12, CfrmUc041Karibarai::VScrollYoushikidiag1, VTS_I2)
// midori 151405 add <--
// midori 152134 add -->
	ON_EVENT(CfrmUc041Karibarai, IDC_YOUSHIKIDIAG3, 8, CfrmUc041Karibarai::ComboSelYoushikidiag1, VTS_I2 VTS_UNKNOWN)
// midori 152134 add <--

	ON_EVENT( CfrmUc041Karibarai, IDC_YOUSHIKIDIAG4, 3, CfrmUc041Karibarai::TerminationYoushikidiag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN )
	ON_EVENT( CfrmUc041Karibarai, IDC_YOUSHIKIDIAG4, 2, CfrmUc041Karibarai::EditOFFYoushikidiag1, VTS_I2 )
	ON_EVENT( CfrmUc041Karibarai, IDC_YOUSHIKIDIAG4, 1, CfrmUc041Karibarai::EditONYoushikidiag1, VTS_I2 )
// midori 151405 add -->
	ON_EVENT(CfrmUc041Karibarai, IDC_YOUSHIKIDIAG4, 12, CfrmUc041Karibarai::VScrollYoushikidiag1, VTS_I2)
// midori 151405 add <--
// midori 152134 add -->
	ON_EVENT(CfrmUc041Karibarai, IDC_YOUSHIKIDIAG4, 8, CfrmUc041Karibarai::ComboSelYoushikidiag1, VTS_I2 VTS_UNKNOWN)
// midori 152134 add <--

	ON_EVENT( CfrmUc041Karibarai, IDC_TITLEDIAG1, 3, CfrmUc041Karibarai::TerminationTitlediag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN )
	ON_EVENT( CfrmUc041Karibarai, IDC_TITLEDIAG1, 2, CfrmUc041Karibarai::EditOFFTitlediag1, VTS_I2 )
	ON_EVENT( CfrmUc041Karibarai, IDC_TITLEDIAG1, 1, CfrmUc041Karibarai::EditONTitlediag1, VTS_I2 )
	ON_EVENT( CfrmUc041Karibarai, IDC_TITLEDIAG1, 6, CfrmUc041Karibarai::FocusTitlediag1, VTS_NONE )

	ON_EVENT( CfrmUc041Karibarai, IDC_PAGEDIAG1, 3, CfrmUc041Karibarai::TerminationPagediag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN )
	ON_EVENT( CfrmUc041Karibarai, IDC_PAGEDIAG1, 2, CfrmUc041Karibarai::EditOFFPagediag1, VTS_I2 )
	ON_EVENT( CfrmUc041Karibarai, IDC_PAGEDIAG1, 1, CfrmUc041Karibarai::EditONPagediag1, VTS_I2 )
	ON_EVENT( CfrmUc041Karibarai, IDC_PAGEDIAG1, 6, CfrmUc041Karibarai::FocusPagediag1, VTS_NONE )
END_EVENTSINK_MAP()

//********************************************************************************
//	初期処理
//********************************************************************************
BOOL	CfrmUc041Karibarai::virStartJob()
{
	// 初期値設定
	if ( CmnSetInitialUchiwakeInfo() != TRUE){	// 共通データ
		return FALSE;
	}

// midori UC_0038 add -->
	// 科目、銀行、取引先参照の番号が正しく連番になっているかチェック
	// 連番になっていない場合、修復処理を行う
	((CUCHIWAKEApp*)AfxGetApp())->OrderCheckEx(m_uInfo.intFormSeq);
// midori UC_0038 add <--

	virSetInitialValue();			// 帳表固有データ

	// テーブルデータ無しなら１頁分のデータを作成
	CmnTblInitialMakeData();

	// 帳表タイトル表示
	virUpdateControlTitle();

	// 頁の表示
	m_ChangeFont = TRUE;
	if ( virUpdateControlTblData() != TRUE ){
		return FALSE;
	}

	// 開始インデックスへ移動
	CmnDiagSetFocus( m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol );

	// MainFrameからチェック状態を取得
//	CMainFrame*	pFrame = (CMainFrame*)AfxGetMainWnd();
//	m_chkYoushikiSkip.SetCheck( pFrame->GetMainYoushikiSkipMode() );

	// 様式送りボタンの制御
	UpdateYoushikiMoveButton( m_YoushikiSkip );

	return TRUE;
}

//********************************************************************************
//	共通データの初期値設定
//********************************************************************************
void	CfrmUc041Karibarai::virSetInitialUchiwakeInfo()
{
	// 「仮払金」の固有データ
	m_uInfo.intOutKeiDialog = AM_TYPE_ALL;				// 計設定ダイアログのボタン指定
	m_uInfo.intRowMax = ID_ROWKOBETU_041;				// １頁内の合計行を含む全体の行数
	m_uInfo.intRowMaxData = ID_ROWKOBETU_041 - 1;		// １頁内のデータ行数（合計行は含まず）
	m_uInfo.intColMax = ID_COLMAX_041;					// １行内のコントロール個数
	m_uInfo.intColMaxData = ID_COLMAXDATA_041;			// １行内の列数
	m_uInfo.intCtlStartIndex = ID_CTLSTARTINDEX_041;	// １行目の入力項目の先頭インデックス番号
	m_uInfo.intT1StartIndex = ID_T1STARTNORMAL_041;		// 合計行①の先頭インデックス番号
	m_uInfo.intT2StartIndex = ID_T2STARTNORMAL_041;		// 合計行②の入力項目の先頭インデックス番号
	m_uInfo.intCursolDefPos = ID_COL_041_ADNAME1;		// カーソル移動時の初期位置（タイトルから↓，合計から↑で使用）
	m_uInfo.intCursolKeiPos = ID_COL_041_ADSEQ;			// 合計行のカーソル移動位置（カーソル移動で使用）

	m_uInfo.intFormSeq = ID_FORMNO_041;					// 様式シーケンス番号
	m_uInfo.intFormNo = 4;								// 様式番号（uc_inf_sub）
	m_uInfo.intFormNo2 = 1;								// 様式番号枝番（uc_inf_sub）

	m_uInfo.intRowNormal = ID_ROWNORMAL_041;			// 通常時：１頁内の合計行を含む全体の行数
	m_uInfo.intRowKobetu = ID_ROWKOBETU_041;			// 個別時：１頁内の合計行を含む全体の行数
	m_uInfo.intT1StartNormal = ID_T1STARTNORMAL_041;	//	通常時：合計行①の先頭インデックス
	m_uInfo.intT2StartNormal = ID_T2STARTNORMAL_041;	//	通常時：合計行②の先頭インデックス
	m_uInfo.intT1StartKobetu = ID_T1STARTKOBETU_041;	//	個別時：合計行①の先頭インデックス
	m_uInfo.intT2StartKobetu = ID_T2STARTKOBETU_041;	//	個別時：合計行②の先頭インデックス
	m_uInfo.intRowKei = ID_ROWKEI_041;					// 帳表固定の合計行数（1:合計あり，0:なし）
	m_uInfo.intKei1StartIndex = ID_COL_041_KEI1_MEISYO;	// 計ダイアグラム1行目の先頭インデックス
	m_uInfo.intKei2StartIndex = ID_COL_041_KEI2_MEISYO;	// 計ダイアグラム2行目の先頭インデックス

	// 集計処理の設定
	m_uInfo.uCalcInfo.intMaxCount = ID_CALC_041_MAX;		// 集計フィールド件数
	m_uInfo.uCalcInfo.strField[0] = ID_CALC_041_FIELD1;		// 集計フィールド名称1
}

//**************************************************
//	ICSDiagコントロールを通常／個別で切り替える
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CfrmUc041Karibarai::virSetInitialDiag()
{	
	int		nIndex;

	// 別の入力画面から参照した場合、画面表示しないので、
	// 各オブジェクトはメンバ変数に割り当てられていない。
	if ((m_DiagNormal.m_hWnd == NULL) || 
		(m_DiagKobetu.m_hWnd == NULL) || (m_DiagKobetu2.m_hWnd == NULL) || (m_DiagKobetu3.m_hWnd == NULL)) {
		return;
	}

	//	出力形式で分岐
	switch( m_uInfo.intFormType ){
	//	通常
	case ID_FORMTYPE_NORMAL:
		m_DiagNormal.ShowWindow( TRUE );		// CmnGetDiagTypeで表示/非表示設定をしているので不要かもしれない
		m_DiagKobetu.ShowWindow( FALSE );		// 同上
		m_pDiag = &m_DiagNormal;		
		m_pDiagHide = CmnGetDiagType( &m_DiagKobetu, &m_DiagKobetu2, &m_DiagKobetu3 );	//m_pDiagHide = &m_DiagKobetu;
		m_pDiagKei = CmnGetDiagType( &m_DiagKei1, &m_DiagKei2 );
		m_intT1StartIndexHide = m_uInfo.intT1StartKobetu;
		m_intT2StartIndexHide = m_uInfo.intT2StartKobetu;
		m_pDiagKei->ShowWindow( FALSE );	// 通常出力時には計ダイアログを使用しないので非表示にする
		break;
	//	個別
	case ID_FORMTYPE_KOBETU:
		m_DiagNormal.ShowWindow( FALSE );		// CmnGetDiagTypeで表示/非表示設定をしているので不要かもしれない
		m_DiagKobetu.ShowWindow( TRUE );		// 同上
		//m_pDiag = &m_DiagKobetu;
		m_pDiag = CmnGetDiagType( &m_DiagKobetu, &m_DiagKobetu2, &m_DiagKobetu3 );
		m_pDiagKei = CmnGetDiagType( &m_DiagKei1, &m_DiagKei2 );
		m_pDiagHide = &m_DiagNormal;
		m_intT1StartIndexHide = m_uInfo.intT1StartNormal;
		m_intT2StartIndexHide = m_uInfo.intT2StartNormal;
		break;
	}
	// 隠すダイアログを非表示
	m_pDiagHide->ShowWindow( FALSE );	

	// 入力不可行でのDeleteキー無効
	m_pDiag->EnableDelete(0);

	//	コンボボックスの属性取得
	nIndex = CmnGetControlIndex( 1, ID_COL_041_ADSEQ );
	m_pDiag->GetAttribute( nIndex, ( LPUNKNOWN )&m_uInfo.uComboAttr );
}

//********************************************************************************
//	帳表固有変数の初期値設定
//********************************************************************************
void	CfrmUc041Karibarai::virSetInitialValue()
{
	// 1ページ分のローカルバッファをクリア
	RecBufClearAllData();

	// 行コピー用のローカルバッファをクリア
	m_nCopyFlag = 0;
	RecBufClearData( &m_CopyData );
	return;
}

//********************************************************************************
//	帳表コントロールの初期化（データ領域）
//********************************************************************************
BOOL	CfrmUc041Karibarai::InitialControlDataArea()
{
	int		intMax;
	int		i;
// No.200903 add -->
	int		len=0;
// No.200903 add <--

	// 全データクリア
	m_pDiag->AllClear();
	m_pDiagKei->AllClear();

	// 各Diagに背景色を設定
	SetDiagBackColor();

	// 背景色クリア
	CmnDiagSetAttrColorAllClear( m_pDiag );

	// 列の属性により更にクリアと初期設定が必要
	intMax = m_uInfo.intT1StartIndex;
	for ( i = m_uInfo.intCtlStartIndex; i < intMax; i += m_uInfo.intColMax ) {
		// ComboBox：クリア
		CmnDiagSetString( m_pDiag, i + ID_COL_041_ADSEQ - 1, "", 0 );	// 科目

		// 漢字入力欄の初期化
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_041_ADNAME1    - 1, MAX_KETA_041_ADNAME1 );		//	名称１
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_041_ADADD1    - 1, MAX_KETA_041_ADADD1 );			//	住所１
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_041_ADADD2    - 1, MAX_KETA_041_ADADD2 );			//	住所２
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_041_KANKEI    - 1, MAX_KETA_041_KANKEI );			//	法人・代表者との関係
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_041_NAIYOU    - 1, MAX_KETA_041_NAIYOU );			//	摘要

		// 金額欄：桁数初期化
// No.200903 del -->
		//CmnDiagSetAttrDigit( m_pDiag, i + ID_COL_041_VAL - 1, ID_VAL_041_DATA );	// 期末現在高
// No.200903 del <--
// No.200903 add -->
		// 期末現在高
		if(bG_Kanso == TRUE)	len = ID_VAL_041_K_DATA;
		else					len = ID_VAL_041_DATA;
		CmnDiagSetAttrDigit( m_pDiag, i + ID_COL_041_VAL - 1, len );
// No.200903 add <--

		//	入力不可モードの解除
		SetInputMode( CmnGetControlRow( i ), DIAG_MDFY_INPUT );
	}

	// ComboBoxの初期設定
	if ( InitialControlComboBox() != TRUE ){
		return FALSE;
	}

	// 半角/全角設定
	CmnDiagSetAttrColZenHan( m_pDiag , ID_COL_041_ADNAME1 );		// 名称1(氏名)
	CmnDiagSetAttrColZenHan( m_pDiag , ID_COL_041_ADADD1 );		// 所在地1(住所)
	CmnDiagSetAttrColZenHan( m_pDiag , ID_COL_041_ADADD2 );		// 所在地2(住所)
	CmnDiagSetAttrColZenHan( m_pDiag , ID_COL_041_KANKEI );		// 法人・代表者との関係
	CmnDiagSetAttrColZenHan( m_pDiag , ID_COL_041_NAIYOU );		// 取引の内容

	return TRUE;
}

//********************************************************************************
//	帳表コントロールの初期化（データ領域）
//		→表示していない方のDiag(通常 or 個別)の設定に使用する
//********************************************************************************
BOOL	CfrmUc041Karibarai::InitialControlDataAreaHide( CICSDiag* pDiag )
{
	int		intMax;
	int		i;
// No.200903 add -->
	int		len=0;
// No.200903 add <--

	// 列の属性により更にクリアと初期設定が必要
	intMax = m_intT1StartIndexHide;
	for ( i = m_uInfo.intCtlStartIndex; i < intMax; i += m_uInfo.intColMax ) {
		// ComboBox：クリア
		CmnDiagSetString( pDiag, i + ID_COL_041_ADSEQ - 1, "", 0 );	// 科目

		// 漢字入力欄の初期化
		CmnDiagSetAttrInpmax( pDiag, i + ID_COL_041_ADNAME1    - 1, MAX_KETA_041_ADNAME1 );			//	名称１
		CmnDiagSetAttrInpmax( pDiag, i + ID_COL_041_ADADD1    - 1, MAX_KETA_041_ADADD1 );			//	住所１
		CmnDiagSetAttrInpmax( pDiag, i + ID_COL_041_ADADD2    - 1, MAX_KETA_041_ADADD2 );			//	住所２
		CmnDiagSetAttrInpmax( pDiag, i + ID_COL_041_KANKEI    - 1, MAX_KETA_041_KANKEI );			//	法人・代表者との関係
		CmnDiagSetAttrInpmax( pDiag, i + ID_COL_041_NAIYOU    - 1, MAX_KETA_041_NAIYOU );			//	摘要

		// 金額欄：桁数初期化
// No.200903 del -->
		//CmnDiagSetAttrDigit( pDiag, i + ID_COL_041_VAL - 1, ID_VAL_041_DATA );	// 期末現在高
// No.200903 del <--
// No.200903 add -->
		// 期末現在高
		if(bG_Kanso == TRUE)	len = ID_VAL_041_K_DATA;
		else					len = ID_VAL_041_DATA;
		CmnDiagSetAttrDigit( pDiag, i + ID_COL_041_VAL - 1, len );
// No.200903 add <--
	}

	return TRUE;
}

//********************************************************************************
//	帳表コントロールの初期化（ComboBox）
//********************************************************************************
BOOL	CfrmUc041Karibarai::InitialControlComboBox()
{
	DIAGRAM_DATA	diadata;	// ICSDIAG構造体
	int				intTotalType;
	int				intMax;
	int				intIndex;
	int				i;
	int				nFontSize = 100;
	BOOL			bFlg = FALSE;	// フォントサイズ変更する？

	// カレント頁の合計行の種別を取得
	intTotalType = CmnGetControlTotalType();

	// カレント頁で"頁計と累計"を表示する場合、対象行は"-1"となる
	switch ( intTotalType ) {
	case ID_OUTKEI_BOTH:			// 頁計と累計
		intMax = m_uInfo.intRowMaxData - 1;
		break;
	default:
		intMax = m_uInfo.intRowMaxData;
		break;
	}

	// 初期化
	m_clsFunc.DiagInit( &diadata );

	// ComboBoxの選択項目取得
	if ( CmnUcLstKamokuGetComboList( &diadata.text_combo, bFlg ) != TRUE ){
		return FALSE;
	}

	// 全行設定
	for ( i = 1; i <= intMax; i++ ) {
		// 対象コントロールのインデックス番号を取得
		intIndex = CmnGetControlIndex( i, ID_COL_041_ADSEQ );

		// 初期設定（ドロップダウンOFF＋リスト情報をセット）
		m_pDiag->ModifyItem( intIndex, DIAG_MDFY_NODROPDOWN );
		m_pDiag->SetData( intIndex, (LPUNKNOWN)&diadata );

		// フォントサイズ設定
		if( m_EndView == FALSE && m_ChangeFont != FALSE && m_ImportMode == FALSE ){
			SetComboFontSize( m_pDiag, intIndex, bFlg );
		}
	}
	m_ChangeFont = FALSE;

	return TRUE;
}

//********************************************************************************
//	帳表コントロールの初期化（頁計/累計領域）
//		文言はデータに含まれているので、その内容を使用する
//********************************************************************************
void	CfrmUc041Karibarai::InitialControlTotalArea()
{
	int		nKeiType;			//	計の種別
	int		nDataRow[2];		//	データ行属性
	int		nDataRowPos[2];		//	データ行位置
	int		nKeiRow[2];			//	計行属性
	int		nCnt;				//	カウント
	int		nStart;				//	開始位置
// midori 152364 add -->
	int		ii=0;
// midori 152364 add <--
// midori 152166 add -->
	int		intIndex=0;
	int		nSw=0;
// 修正No.157898 add -->
	int		flg2[2] = {0};
// 修正No.157898 add <--

	intIndex = CmnGetControlIndex(m_uInfo.intCurRow,m_uInfo.intCurCol);
// midori 152166 add <--

	// カレント頁の合計行の種別を取得
	nKeiType = CmnGetControlTotalType();
	
	nDataRowPos[0] = m_uInfo.intRowMax - 2;
	nDataRowPos[1] = m_uInfo.intRowMax - 1;

	//	計の種別で分岐
	switch ( nKeiType ){
	case ID_OUTKEI_OFF:				// なし
		//	データ行の設定
// midori 160612 cor -->
		//nDataRow[0] = ( DIAG_MDFY_EDIT	| DIAG_MDFY_OPAQUE );
		//nDataRow[1] = ( DIAG_MDFY_EDIT	| DIAG_MDFY_OPAQUE );
// ---------------------
		if(m_DataKakutei == FALSE)	{
// midori 152364 del -->
			//nDataRow[0] = (DIAG_MDFY_EDIT | DIAG_MDFY_OPAQUE);
			//nDataRow[1] = (DIAG_MDFY_EDIT | DIAG_MDFY_OPAQUE);
// midori 152364 del <--
// midori 152364 add -->
			for(ii=0; ii<2; ii++)	{
				if(IsSpecialRow(m_ReadData[nDataRowPos[ii]+1].m_FgFunc) != 0)	{
					nDataRow[ii] = (DIAG_MDFY_NOINPUT | DIAG_MDFY_OPAQUE);
// 修正No.157898 add -->
					if(IsSpecialRow2(m_ReadData[nDataRowPos[ii]+1].m_FgFunc) != 0)	{
						flg2[ii] = 1;
					}
// 修正No.157898 add <--
				}
				else	{
					nDataRow[ii] = (DIAG_MDFY_EDIT | DIAG_MDFY_OPAQUE);
				}
			}
// midori 152364 add <--
		}
		else	{
			nDataRow[0] = (DIAG_MDFY_READONLY | DIAG_MDFY_OPAQUE);
			nDataRow[1] = (DIAG_MDFY_READONLY | DIAG_MDFY_OPAQUE);
		}
// midori 160612 cor <--
		//	計行の設定
		nKeiRow[0] = ( DIAG_MDFY_READONLY | DIAG_MDFY_TRANSPARENT );
		nKeiRow[1] = ( DIAG_MDFY_READONLY | DIAG_MDFY_TRANSPARENT );
		break;
	case ID_OUTKEI_PAGEKEI:			// 頁計
	case ID_OUTKEI_RUIKEI:			// 累計
		//	データ行の設定
// midori 160612 cor -->
		//nDataRow[0] = ( DIAG_MDFY_EDIT	| DIAG_MDFY_OPAQUE );
// ---------------------
		if(m_DataKakutei == FALSE)	{
// midori 152364 del -->
			//nDataRow[0] = (DIAG_MDFY_EDIT | DIAG_MDFY_OPAQUE);
// midori 152364 del <--
// midori 152364 add -->
			if(IsSpecialRow(m_ReadData[nDataRowPos[0]+1].m_FgFunc) != 0)	{
				nDataRow[0] = (DIAG_MDFY_NOINPUT | DIAG_MDFY_OPAQUE);
// 修正No.157898 add -->
				if(IsSpecialRow2(m_ReadData[nDataRowPos[0]+1].m_FgFunc) != 0)	{
					flg2[0] = 1;
				}
// 修正No.157898 add <--
			}
			else	{
				nDataRow[0] = (DIAG_MDFY_EDIT | DIAG_MDFY_OPAQUE);
			}
// midori 152364 add <--
		}
		else	{
			nDataRow[0] = (DIAG_MDFY_READONLY | DIAG_MDFY_OPAQUE);
		}
// midori 160612 cor <--
		nDataRow[1] = ( DIAG_MDFY_READONLY | DIAG_MDFY_TRANSPARENT );
		//	計行の設定
		nKeiRow[0] = ( DIAG_MDFY_READONLY | DIAG_MDFY_TRANSPARENT );
		nKeiRow[1] = ( DIAG_MDFY_READONLY | DIAG_MDFY_OPAQUE );
		break;
	case ID_OUTKEI_BOTH:			// 頁計と累計
		//	データ行の設定
		nDataRow[0] = ( DIAG_MDFY_READONLY | DIAG_MDFY_TRANSPARENT );
		nDataRow[1] = ( DIAG_MDFY_READONLY | DIAG_MDFY_TRANSPARENT );
		//	計行の設定
		nKeiRow[0] = ( DIAG_MDFY_READONLY | DIAG_MDFY_OPAQUE );
		nKeiRow[1] = ( DIAG_MDFY_READONLY | DIAG_MDFY_OPAQUE );
		break;
	}

	// 28行目のデータ行
	nStart = nDataRowPos[0] * m_uInfo.intColMax;
	for ( nCnt = 0; nCnt < m_uInfo.intColMax; nCnt++){
		m_pDiag->ModifyItem( nStart + nCnt, nDataRow[0] );
// midori 152166 add -->
		if((intIndex == (nStart + nCnt)) && ((nDataRow[0] & DIAG_MDFY_READONLY) != 0))	nSw=1;
// midori 152166 add <--
	}
	// 29行目のデータ行
	nStart = nDataRowPos[1] * m_uInfo.intColMax;
	for ( nCnt = 0; nCnt < m_uInfo.intColMax; nCnt++){
		m_pDiag->ModifyItem( nStart + nCnt, nDataRow[1] );
// midori 152166 add -->
		if((intIndex == (nStart + nCnt)) && ((nDataRow[1] & DIAG_MDFY_READONLY) != 0))	nSw=1;
// midori 152166 add <--
	}
	// 28行目の合計行①
	nStart = m_uInfo.intT1StartIndex;
	for ( nCnt = 0; nCnt < m_uInfo.intColMax; nCnt++ ){
		m_pDiag->ModifyItem( nStart + nCnt, nKeiRow[0] );
// midori 152166 add -->
		if((intIndex == (nStart + nCnt)) && ((nKeiRow[0] & DIAG_MDFY_READONLY) != 0))	nSw=1;
// midori 152166 add <--
	}
	// 29行目の合計行②
	nStart = m_uInfo.intT2StartIndex;
	for ( nCnt = 0; nCnt < m_uInfo.intColMax; nCnt++ ){
		m_pDiag->ModifyItem( nStart + nCnt, nKeiRow[1] );
// midori 152166 add -->
		if((intIndex == (nStart + nCnt)) && ((nKeiRow[1] & DIAG_MDFY_READONLY) != 0))	nSw=1;
// midori 152166 add <--
	}

// 修正No.157898 add -->
	if(flg2[0] == 1)	{
		// 一括自動行の科目を入力可能にする
		m_pDiag->ModifyItem( CmnGetControlIndex( (nDataRowPos[0]+1), ID_COL_041_ADSEQ ), (DIAG_MDFY_EDIT | DIAG_MDFY_OPAQUE) );
	}
	if(flg2[1] == 1)	{
		// 一括自動行の科目を入力可能にする
		m_pDiag->ModifyItem( CmnGetControlIndex( (nDataRowPos[1]+1), ID_COL_041_ADSEQ ), (DIAG_MDFY_EDIT | DIAG_MDFY_OPAQUE) );
	}
// 修正No.157898 add <--

	// 計専用ダイアグラムの金額欄を入力不可にする
	int intKeiRow = (DIAG_MDFY_READONLY | DIAG_MDFY_OPAQUE);
	m_pDiagKei->ModifyItem(ID_COL_041_KEI1_VAL, intKeiRow);
	m_pDiagKei->ModifyItem(ID_COL_041_KEI2_VAL, intKeiRow);
// midori 152166 add -->
	if(nSw != 0)	{
		// 現在のカーソルポジションが入力不可になった場合は先頭に戻す
		m_uInfo.intCurRow = 1;
		m_uInfo.intCurCol = 1;
	}
// midori 152166 add <--

// midori M-16081501 del -->
	//// 合計行の名称欄は、カーソルを飛ばしたいので入力許可する
	//switch ( nKeiType ) {
	//case ID_OUTKEI_OFF:
	//	nKeiRow[0] = ( DIAG_MDFY_READONLY | DIAG_MDFY_TRANSPARENT | DIAG_MDFY_NOINPUT );
	//	nKeiRow[1] = ( DIAG_MDFY_READONLY | DIAG_MDFY_TRANSPARENT | DIAG_MDFY_NOINPUT );
	//	break;
	//case ID_OUTKEI_PAGEKEI:
	//case ID_OUTKEI_RUIKEI:
	//	nKeiRow[0] = ( DIAG_MDFY_READONLY | DIAG_MDFY_TRANSPARENT | DIAG_MDFY_NOINPUT );
	//	nKeiRow[1] = ( DIAG_MDFY_EDIT | DIAG_MDFY_OPAQUE | DIAG_MDFY_NOINPUT );
	//	break;
	//case ID_OUTKEI_BOTH:
	//	nKeiRow[0] = ( DIAG_MDFY_EDIT | DIAG_MDFY_OPAQUE | DIAG_MDFY_NOINPUT );
	//	nKeiRow[1] = ( DIAG_MDFY_EDIT | DIAG_MDFY_OPAQUE | DIAG_MDFY_NOINPUT );
	//	break;
	//}

	//// 入力可能（ただし、画面レイアウトで入力文字数"0"としているため、実際は入力できない）
	//m_pDiag->ModifyItem( CmnGetControlTotalIndex( 1, ID_COL_041_ADSEQ ), nKeiRow[0] );
	//m_pDiag->ModifyItem( CmnGetControlTotalIndex( 2, ID_COL_041_ADSEQ ), nKeiRow[1] );
// midori M-16081501 del <--

// No.200903 del -->
//	// 合計行の金額欄は、金額欄 + 1桁
//// midori M-16081501 del -->
//	//CmnDiagSetAttrDigit( m_pDiag, CmnGetControlTotalIndex( 1, ID_COL_041_VAL ), ID_VAL_041_TOTAL );
//	//CmnDiagSetAttrDigit( m_pDiag, CmnGetControlTotalIndex( 2, ID_COL_041_VAL ), ID_VAL_041_TOTAL );
//// midori M-16081501 del <--
//	CmnDiagSetAttrDigit( m_pDiagKei, ID_COL_041_KEI1_VAL, ID_VAL_041_TOTAL );
//	CmnDiagSetAttrDigit( m_pDiagKei, ID_COL_041_KEI1_VAL, ID_VAL_041_TOTAL );
// No.200903 del <--
}

//********************************************************************************
//	帳表コントロールの初期化（頁計/累計領域）
//		→表示していない方のDiag(通常 or 個別)の設定に使用する
//********************************************************************************
void	CfrmUc041Karibarai::InitialControlTotalAreaHide( CICSDiag* pDiag )
{
	int intIndex = 0;

	//＜注意＞
	// 下記のインデックス番号の算出は「CmnGetControlTotalIndex」でも同じ計算をしている
	// そのためどちらかのシーケンスが変わるとお互いに影響する可能性がある

	// 合計行の金額欄は、金額欄 + 1桁
	intIndex = m_intT1StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_041_VAL - 1;
	CmnDiagSetAttrDigit( pDiag, intIndex, ID_VAL_041_TOTAL );

	intIndex = m_intT2StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_041_VAL - 1;
	CmnDiagSetAttrDigit( pDiag, intIndex, ID_VAL_041_TOTAL );
}

//********************************************************************************
//	帳表コントロールの特殊行初期化
//	IN	int		行番号
//		int		特殊行フラグ（FgFunc）
//	RET	なし
//********************************************************************************
void	CfrmUc041Karibarai::InitialControlSpecialRow( int intRow, int intFgFunc )
{
	DIAGRAM_DATA	diadata;	// ICSDIAG構造体

	// 対象行の色を変更
	CmnDiagSetAttrSpecialRow( m_pDiag, intRow, intFgFunc );

	// 特殊行の金額欄は桁数変更 ･･･ 頁計/累計行は初期設定するのでここでは対象外
	switch ( intFgFunc ) {
	case ID_FGFUNC_SYOKEI:			// 小計
	case ID_FGFUNC_SYOKEINULL:		// 小計（空行）
	case ID_FGFUNC_CHUKEI:			// 中計
	case ID_FGFUNC_CHUKEINULL:		// 中計（空行）
	case ID_FGFUNC_IKKATUMANUAL:	// 一括集計金額行（手動）
	case ID_FGFUNC_IKKATUAUTO:		// 一括集計金額行（自動）
		// 桁数を設定
		CmnDiagSetAttrDigit( m_pDiag, CmnGetControlIndex( intRow , ID_COL_041_VAL ), ID_VAL_041_TOTAL );
		break;
	}

	// 特殊行は入力不可とする
	switch (intFgFunc) {
	case ID_FGFUNC_SYOKEI:			// 小計
	case ID_FGFUNC_SYOKEINULL:		// 小計（空行）
	case ID_FGFUNC_CHUKEI:			// 中計
	case ID_FGFUNC_CHUKEINULL:		// 中計（空行）
	case ID_FGFUNC_IKKATUAUTO:		// 一括集計金額行（自動）
		
		// 一括集計金額行（自動）以外？
		if ( intFgFunc != ID_FGFUNC_IKKATUAUTO ){

			// 「科目」はComboBoxのため、DropDown不可とする
			m_pDiag->ModifyItem( CmnGetControlIndex( intRow, ID_COL_041_ADSEQ ), DIAG_MDFY_NODROPDOWN );

			// ComboBoxの選択項目クリア
			m_clsFunc.DiagInit( &diadata );
			diadata.text_combo = _T( "\r\n" );
			m_pDiag->SetData( CmnGetControlIndex( intRow, ID_COL_041_ADSEQ ), (LPUNKNOWN)&diadata );
		}

		//	入力不可モードの設定
// midori 160612 cor -->
		//SetInputMode( intRow, DIAG_MDFY_NOINPUT );
// ---------------------
		if(m_DataKakutei == FALSE)	SetInputMode(intRow,DIAG_MDFY_NOINPUT);
		else						SetInputMode(intRow,DIAG_MDFY_READONLY);
// midori 160612 cor <--

// No.200905 add -->
// 改良No.21-0086,21-0529 cor -->
		//if(KamokuRowEnableSgn(m_pDB,1) == 1)	{
// ------------------------------
		if(KamokuRowEnableSgn(m_pDB, 1, m_uInfo.intFormSeq) == 1)	{
// 改良No.21-0086,21-0529 cor <--
			// 一括集計金額（自動）行の場合、科目のみ入力可能とする
			if (intFgFunc == ID_FGFUNC_IKKATUAUTO ){
				//	科目
				m_pDiag->ModifyItem( CmnGetControlIndex( intRow, ID_COL_041_ADSEQ ), DIAG_MDFY_INPUT );
			}
		}
// No.200905 add <--

		break;
	
	case ID_FGFUNC_RUIKEI:		//	頁計
	case ID_FGFUNC_PAGEKEI:		//	累計
		//	何もしない
		break;
	}
}

//********************************************************************************
//	帳表タイトル情報の表示
//********************************************************************************
void	CfrmUc041Karibarai::virUpdateControlTitle()
{
	DIAGRAM_ATTRIBUTE diaatt;

	// 帳表ナンバーの背景色設定
	CmnDiagSetAttrColor( &m_NumberDiag, 0, &m_ViewColor );

	// 帳表ナンバー表示
	CmnDiagSetString( &m_NumberDiag, 0, m_uInfo.strTitleNumber, 1 );

	// 帳表タイトル表示
	CmnDiagSetString( &m_TitleDiag, 0, m_uInfo.strTitleName, 0 );

/*
	// 帳表タイトル編集可能/不可
	BOOL	bEnable = CmnCheckEnableTitleEdit();
	GetDlgItem( IDC_TITLEDIAG1 )->EnableWindow( bEnable );
*/

	//	タイトル入力最大文字数設定
	CmnDiagSetAttrInpmax( &m_TitleDiag, 0, ID_TITLE_INPMAX );
	
	long	nMode;		//	入力不可モードフラグ

/*
	// 帳表タイトル編集可能/不可
	if ( CmnCheckEnableTitleEdit() == TRUE ){
		nMode = DIAG_MDFY_INPUT;	//	入力可
	} else {
		nMode = DIAG_MDFY_NOINPUT;	//	入力不可
	}
*/
	// 全ての様式において、編集可能
	nMode = DIAG_MDFY_INPUT;	//	入力可

	//	入力不可モード切り替え
	m_TitleDiag.ModifyItem( 0, nMode );

	// 帳表タイトル_半角/全角設定
	m_TitleDiag.GetAttribute( 0 , (LPUNKNOWN)&diaatt );
	diaatt.attr_editattr = diaatt.attr_editattr | DIAG_EATTR_ZENHAN;
	m_TitleDiag.SetAttribute( 0 , (LPUNKNOWN)&diaatt , TRUE );

// midori 161108 add -->
	// 保管ﾃﾞｰﾀ件数の背景色設定
	CmnDiagSetAttrColor(&m_SaveDataDiag, 0, &m_ViewColor);
// midori 161108 add <--

// midori 190301 add -->
	// ---------------------------------------------------
	// 項目名の変更（新様式：摘要、旧様式：取引の内容）
	// ---------------------------------------------------
	if(bG_Kanso == TRUE)	{
		m_clsFunc.DiagSetString(&m_HeadDiag,6,ID_DIAGDATA_DISP,_T("摘要"));
	}
	else	{
		m_clsFunc.DiagSetString(&m_HeadDiag,6,ID_DIAGDATA_DISP,_T("取引の内容"));
	}
// midori 190301 add <--

// 修正No.158252 del -->
//// 修正No.157626 add -->
//	// 欄外ガイドの背景色
//	m_Guide1.SetBackColor(m_ViewColor);
//	m_Guide1.ShowWindow(FALSE);
//// 修正No.157626 add <--
// 修正No.158252 del <--
}

//********************************************************************************
//	帳表ページ情報の表示
//********************************************************************************
void	CfrmUc041Karibarai::UpdateControlPage()
{
	BOOL	bBack = TRUE;
	BOOL	bNext = TRUE;

	// 最大頁取得
	m_uInfo.intMaxPage = CmnTblGetPageMax();

	// カレント頁番号のチェック
	if ( m_uInfo.intCurPage > m_uInfo.intMaxPage ) {
		m_uInfo.intCurPage = m_uInfo.intMaxPage;
	}

	// カレント頁/最大頁表示
	CmnDiagSetValue( &m_PageDiag, 1, m_uInfo.intCurPage, 0 );
	CmnDiagSetValue( &m_PageDiag, 3, m_uInfo.intMaxPage, 1 );

	// 前頁/次頁ボタンの制御
	CmnCheckEnableMoveButton( &bBack, &bNext );
// midori 155879 del -->
//	GetDlgItem( IDC_PAGEBACK_BUTTON1 )->EnableWindow( bBack );	// 前頁ボタン
//	GetDlgItem( IDC_PAGENEXT_BUTTON1 )->EnableWindow( bNext );	// 次頁ボタン
// midori 155879 del <--
// midori 155879 add -->
	PostMessage(WM_USER_CONTROLPAGE, bBack, bNext);
// midori 155879 add <--
}

//********************************************************************************
//	帳表データの表示（１ページ分）
//********************************************************************************
BOOL	CfrmUc041Karibarai::virUpdateControlTblData()
{
	// ダイアログ切替
	if( m_pDiag != NULL ){
		virSetInitialDiag();
	}

	CdbUc041Karibarai	dbRec( m_pDB );
	
	m_pDiag->DeleteInput();		//	入力ボックス削除 

	// 頁情報の表示
	UpdateControlPage();

	// コントロールのクリア＋初期設定
	if ( InitialControlDataArea() != TRUE ){	// データ行
		return FALSE;
	}
// midori 160612 del -->
	//InitialControlTotalArea();	// 頁計/累計行
	//m_pDiag->Refresh();			// 更新
// midori 160612 del <--

// No.200903 add -->
	// 頁計、累計行の桁数をセットする
	if(m_uInfo.intFormType == ID_FORMTYPE_NORMAL)	{
		CmnDiagSetAttrDigit( m_pDiagKei, CmnGetControlTotalIndex(1, ID_COL_041_VAL), ID_VAL_041_TOTAL );
		CmnDiagSetAttrDigit( m_pDiagKei, CmnGetControlTotalIndex(2, ID_COL_041_VAL), ID_VAL_041_TOTAL );
	}
	else {
		CmnDiagSetAttrDigit( m_pDiagKei, ID_COL_041_KEI1_VAL, ID_VAL_041_TOTAL );
		CmnDiagSetAttrDigit( m_pDiagKei, ID_COL_041_KEI2_VAL, ID_VAL_041_TOTAL );
	}
// No.200903 add <--

	//----------------------- １行データ表示 ------------------------------//

	// 倍長設定
	l_defn( 0x16 );

	// 指定ページ番号のデータを取得
	dbRec.Init( m_uInfo.intCurPage );

	// 1ページ分のローカルバッファをクリア
	RecBufClearAllData();

	// 1ページ分のデータ1レコードづつ取得し、画面に表示
	while ( !dbRec.IsEOF() ) {		// ファイル終端まで
		// 1ページ分のローカルバッファにデータを格納
		RecBufSetData( &dbRec );

		// 対象行金額欄の桁数を変更
		InitialControlSpecialRow( dbRec.m_NumRow, dbRec.m_FgFunc );

		// 累計/頁計の場合、特殊処理
		if ( ( dbRec.m_FgFunc == ID_FGFUNC_RUIKEI ) || ( dbRec.m_FgFunc == ID_FGFUNC_PAGEKEI ) ) {
			// 累計/頁計の表示
			UpdateControlRowTotal( &dbRec );
		}
		// 累計・頁計・空行以外
		else{
			// データ行の表示（空行，特殊行を含む）
			UpdateControlRowData( &dbRec );
		}

// No.200601 add -->
		// 簡素化様式のみ
		if(bG_Kanso == TRUE) {
			// 電子申告の文字数を超えていれば、項目の文字色を変える
			// 科目
			CmnInitialControlFgStringCell( m_pDiag, dbRec.m_NumRow, ID_COL_041_ADSEQ, dbRec.m_FgFunc, UC_KAMOKU_LEN, dbRec.m_KnName);
			// 相手先 法人・代表者との関係
			CmnInitialControlFgStringCell( m_pDiag, dbRec.m_NumRow, ID_COL_041_KANKEI, dbRec.m_FgFunc, UC_041_KANKEI_LEN, dbRec.m_Kankei);
		}
// No.200601 add <--

		dbRec.MoveNext();
	}

	// 閉じる
	dbRec.Fin();

// midori 160612 add -->
	// InitialControlSpecialRow 関数内で、入力確定の判定を行っているので終わってから下記の関数を行う
	// ※先に行うと計行が入力可能になってしまう
	InitialControlTotalArea();				// 頁計/累計行
// midori 160612 add <--

// midori 161108 add -->
	// 保管ﾃﾞｰﾀ件数の表示
	virUpdateControlTblData2();
// midori 161108 add <--

// No.200601 add -->
	// 簡素化様式のみ
	if(bG_Kanso == TRUE) {
		// 項目タイトル(ヘッダ部)の再設定
		TitleColorChg();
	}
// No.200601 add <--

	// 画面更新
	m_pDiag->Refresh();
	m_pDiagKei->Refresh();

	return TRUE;
}

// midori 161108 add -->
//********************************************************************************
//	保管件数の表示
//********************************************************************************
BOOL CfrmUc041Karibarai::virUpdateControlTblData2()
{
	int					hcnt=0;
	CString				cs=_T("");

	hcnt=virGetDataTableObject()->GetCountFgShow(ID_FGSHOW_HOKAN);
	cs.Format(_T("保管%3d件"),hcnt);
	CmnDiagSetString(&m_SaveDataDiag,0,cs,1);

	return TRUE;
}
// midori 161108 add <--

//********************************************************************************
//	帳表データの１行表示（空行，データ行，特殊行）
//		IN		CdbUc041Karibarai*	「仮払金」
//		RET		なし
//********************************************************************************
void	CfrmUc041Karibarai::UpdateControlRowData( CdbUc041Karibarai* prmDbRec ) {

	DIAGRAM_DATA		diadata;		// ICSDiag構造体
	int					intCell;		// 保存フィールド

	// 初期化
	m_clsFunc.DiagInit( &diadata );
	// 読込み対象セル取得
	intCell = CmnGetControlIndex( prmDbRec->m_NumRow, ID_COL_041_ADSEQ );

	// 科目
	//	データ行？
	switch( prmDbRec->m_FgFunc ){
	//------------------------------
	//	空行
	case ID_FGFUNC_NULL:
	//	データ行
	case ID_FGFUNC_DATA:
	//	一括金額（手動）
	case ID_FGFUNC_IKKATUMANUAL:
	//	一括金額（自動）
	case ID_FGFUNC_IKKATUAUTO:
	//------------------------------
		//	コンボの選択インデックス
// midori 152137 del -->
//		diadata.data_combo = (short)prmDbRec->m_KnOrder;
// midori 152137 del <--
// midori 152137 add -->
		// KnOrder（科目順序）をキーにコンボボックスのインデックスを取得
		m_SortMap1.Lookup(prmDbRec->m_KnOrder,diadata.data_combo);
// midori 152137 add <--
		break;
	//------------------------------
	//	小計
	case ID_FGFUNC_SYOKEI:
	//	小計（空行）
	case ID_FGFUNC_SYOKEINULL:
	//	中計
	case ID_FGFUNC_CHUKEI:
	//	中計計（空行）
	case ID_FGFUNC_CHUKEINULL:
	//------------------------------		
		diadata.text_combo = prmDbRec->m_KeiStr;
		break;
	default:
		break;
	}
	m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	intCell++;

	//	名称１
	//	データ行？
	switch( prmDbRec->m_FgFunc ){
	//------------------------------
	//	空行
	case ID_FGFUNC_NULL:
	//	データ行
	case ID_FGFUNC_DATA:
	//	一括金額（手動）
	case ID_FGFUNC_IKKATUMANUAL:
	//	一括金額（自動）
	case ID_FGFUNC_IKKATUAUTO:
	//------------------------------
		diadata.data_edit = m_clsFunc.StrDocking( prmDbRec->m_AdName1 , prmDbRec->m_AdName2 );
		break;
	default:
		diadata.data_edit.Empty();
		break;
	}
	m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	intCell++;
	
	// 住所１
	diadata.data_edit = prmDbRec->m_AdAdd1;
	m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	intCell++;
	
	// 住所２
	diadata.data_edit = prmDbRec->m_AdAdd2;
	m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	intCell++;
	
	// 法人・代表者との関係
	diadata.data_edit = prmDbRec->m_Kankei;
	m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	intCell++;

	// 期末現在高
	if ( prmDbRec->m_Val != "" ){
		CmnChangeFieldValData( &diadata.data_val[0] , prmDbRec->m_Val );
		m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	}
	else{
		m_pDiag->DataClear( intCell, TRUE );
	}
	intCell++;

	// 取引の内容
	diadata.data_edit = prmDbRec->m_Naiyou;
	m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
}

//********************************************************************************
//	帳表データの１行表示（累計，頁計行）
//		IN		CdbUc041Karibarai*	「仮払金」テーブル情報
//		RET		なし
//********************************************************************************
void	CfrmUc041Karibarai::UpdateControlRowTotal( CdbUc041Karibarai* prmDbRec ) {

	int				intTotalType;
	int				intRowName = 0;			// 名称表示行（0:非表示）
	int				intRowKingaku = 0;		// 金額表示行（0:非表示）

	// カレント頁の合計行の種別を取得
	intTotalType = CmnGetControlTotalType();

	switch (intTotalType) {
	case ID_OUTKEI_OFF:
		break;
	case ID_OUTKEI_PAGEKEI:
		// カレント頁："頁計"なら、合計行②に"頁計"を表示
		if (prmDbRec->m_FgFunc == ID_FGFUNC_PAGEKEI) {
			intRowName = 2;
			intRowKingaku = 2;
		}
		break;
	case ID_OUTKEI_RUIKEI:
		// カレント頁："累計"なら、合計行②に"累計"を表示
		if (prmDbRec->m_FgFunc == ID_FGFUNC_RUIKEI) {
			intRowName = 2;
			intRowKingaku = 2;
		}
		break;
	case ID_OUTKEI_BOTH:
		// カレント頁："頁計＋累計"なら、合計行①に"頁計"＋合計行②に"累計"を表示
		if (prmDbRec->m_FgFunc == ID_FGFUNC_PAGEKEI) {
			intRowName = 1;
			intRowKingaku = 1;
		}
		else if (prmDbRec->m_FgFunc == ID_FGFUNC_RUIKEI) {
			intRowName = 2;
			intRowKingaku = 2;
		}
		break;
	}

	// 頁計/累計の名称表示
	if (intRowName > 0) {
		if( m_uInfo.intFormType == ID_FORMTYPE_NORMAL ){
			// 計なしは従来通り
			CmnDiagSetString(m_pDiag, CmnGetControlTotalIndex(intRowName, ID_COL_041_ADSEQ), prmDbRec->m_KeiStr, 0);
		}
		else{
			// 計ありは新たに追加した計ダイアログにセット
			CmnDiagSetString( m_pDiagKei, CmnGetKeiDiagIndex( intRowName, ID_COL_041_KEI1_MEISYO ), prmDbRec->m_KeiStr, 1 );
		}
	}
	// 頁計/累計の金額表示
	if (intRowKingaku > 0) {
		if( m_uInfo.intFormType == ID_FORMTYPE_NORMAL ){
			// 計なしは従来通り
			CmnDiagSetKingaku(m_pDiag, CmnGetControlTotalIndex(intRowKingaku, ID_COL_041_VAL), prmDbRec->m_Val);
		}
		else{
			// 計ありは新たに追加した計ダイアログにセット
			CmnDiagSetKingaku( m_pDiagKei, CmnGetKeiDiagIndex( intRowKingaku, ID_COL_041_KEI1_VAL ), prmDbRec->m_Val );
		}
	}
}

//********************************************************************************
//	【F6:項目複写】１つ前のデータ表示処理
//		IN		CdbUc000Common*	共通テーブルクラス
//		RET		なし
//********************************************************************************
void	CfrmUc041Karibarai::virFncCellCopyProc( CdbUc000Common* rsData )
{
	CdbUc041Karibarai*	rs;
	int					intCol;	// 現在の列

	rs = (CdbUc041Karibarai*)rsData;

	// 列取得
	intCol = CmnGetControlCol();

	// 列によって処理を分岐
	switch( intCol ){

	// 科目
	case ID_COL_041_ADSEQ:
// midori 152137 del -->
//		CmnDiagSetCombo( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_KnOrder );
// midori 152137 del <--
// midori 152137 add -->
		CmnDiagSetCombo2( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_KnOrder );
// midori 152137 add <--
		break;
	// 名称1(氏名)
	case ID_COL_041_ADNAME1:
		CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, m_clsFunc.StrDocking( rs->m_AdName1 , rs->m_AdName2 ) , 0 );
		break;
	// 所在地1(住所)
	case ID_COL_041_ADADD1:
		CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_AdAdd1, 0 );
		break;
	// 所在地2(住所)
	case ID_COL_041_ADADD2:
		CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_AdAdd2, 0 );
		break;
	// 法人・代表者との関係
	case ID_COL_041_KANKEI:
		CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_Kankei, 0 );
		break;
	// 期末現在高
	case ID_COL_041_VAL:
		CmnDiagSetKingaku( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_Val );
		break;
	// 取引の内容
	case ID_COL_041_NAIYOU:
		CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_Naiyou, 0 );
		break;
	}

	// 画面更新
	m_pDiag->Refresh();
}
//********************************************************************************
//	【F9:参照】参照ダイアログで使用する種別を取得
//		IN		int				カレント列番号
//				int*			取引先時に使用するグループ番号（更新項目）･･･ NDS参照ダイアログで使用
//									ID_ADDRESSGR_NONE	：なし
//									ID_ADDRESSGR_URI	：売掛金
//									ID_ADDRESSGR_KAI	：買掛金
//				long*			現在選択項目のシーケンス番号　（更新項目）･･･ NDS参照ダイアログで使用
//				int*				住所１の分割するバイト数 ･･･ ICS様住所検索で使用
//				int*				住所２の分割するバイト数 ･･･ ICS様住所検索で使用
//				int*				住所１の列番号
//				int*				住所２の列番号
//		RET		int				参照型の種別
//									ID_DLGTYPE_BANK	：金融機関
//									ID_DLGTYPE_ADDRESS	：取引先
//									ID_DLGTYPE_KAMOKU	：科目
//									ID_DLGTYPE_NONE	：参照不可
//********************************************************************************
int		CfrmUc041Karibarai::virFncReferenceGetType(int intCol, int* intGrSeq, long* lngItemSeq, int* intAdd1, int* intAdd2, int* intAdd1Col, int* intAdd2Col, REFERENCE_INFO* pudReference)
{
	int				intRefType;

	switch (intCol) {

	// 種類
	case ID_COL_041_ADSEQ:
		intRefType = ID_DLGTYPE_KAMOKU;
		*intGrSeq = ID_ADDRESSGR_NONE;
		*lngItemSeq = m_ReadData[m_uInfo.intCurRow].m_KnSeq;
		break;

	//	取引先
	case ID_COL_041_ADNAME1:
	//case ID_COL_041_ADADD1:
	//case ID_COL_041_ADADD2:
		intRefType = ID_DLGTYPE_ADDRESS;
		*intGrSeq = ID_ADDRESSGR_KAI;
		*lngItemSeq = m_ReadData[m_uInfo.intCurRow].m_AdSeq;
		// 住所欄なら住所１、２の列番号取得
		//if ( intCol != ID_COL_041_ADNAME1 ){
		//	*intAdd1Col = ID_COL_041_ADADD1;
		//	*intAdd2Col = ID_COL_041_ADADD2;
		//}
		break;

	// 所在地
	case ID_COL_041_ADADD1:
	case ID_COL_041_ADADD2:
		intRefType = ID_DLGTYPE_ICSADDSEARCH;
		*intAdd1 = MAX_KETA_041_ADADD1/*20*/ * 2;			// Byte指定
		*intAdd2 = MAX_KETA_041_ADADD2/*20*/ * 2;			// Byte指定
		*intAdd1Col = ID_COL_041_ADADD1;
		*intAdd2Col = ID_COL_041_ADADD2;
		break;

	// その他：参照対象外
	default:
		intRefType = ID_DLGTYPE_NONE;
		*intGrSeq = ID_ADDRESSGR_NONE;
		*lngItemSeq = 0;
		break;
	}
	return intRefType;
}

//********************************************************************************
//	【F9:参照】参照ダイアログで選択された内容を表示
//		IN		CdlgReference*	参照ダイアログ情報
//				int				参照ダイアログの種別
//				int				カレント列番号
//		RET		なし
//********************************************************************************
void	CfrmUc041Karibarai::virFncReferenceSetData( CdlgReference* dlgRef, int intRefType, int intCol )
{
	int		nCurRow;	//	カレント行
	int		nCurCol;	//	カレント列
	int		nPos;		//	位置

	switch (intRefType) {

	// 種類
	case ID_DLGTYPE_KAMOKU:
		// 選択内容を表示
// midori 152137 del -->
//		CmnDiagSetCombo( m_pDiag, m_uInfo.intCurCtlIndex, dlgRef->m_udKamoku.m_KnOrder );
// midori 152137 del <--
// midori 152137 add -->
		CmnDiagSetCombo2( m_pDiag, m_uInfo.intCurCtlIndex, dlgRef->m_udKamoku.m_KnOrder );
// midori 152137 add <--
		break;

	//	取引先
	case ID_DLGTYPE_ADDRESS:

		// カレント行＋列番号を取得
		CmnGetControlPosition( &nCurRow, &nCurCol );
		//	現在行の取引先の名称１の位置を算出
		nPos = (( nCurRow - 1 ) * m_uInfo.intColMax ) + ( m_uInfo.intCtlStartIndex + ( ID_COL_041_ADNAME1 - 1 ));
		
		//	選択内容を表示
		CmnDiagSetString( m_pDiag, nPos, m_clsFunc.StrDocking( dlgRef->m_udAddress.m_AdName1 ,
			dlgRef->m_udAddress.m_AdName2 ) , 0 );		//	名称１
		CmnDiagSetString( m_pDiag, nPos + 1, dlgRef->m_udAddress.m_AdAdd1, 0 );		//	住所１
		CmnDiagSetString( m_pDiag, nPos + 2, dlgRef->m_udAddress.m_AdAdd2, 0 );		//	住所２
		break;

	default:
		return;
		break;
	}
	m_pDiag->Refresh();
}

//********************************************************************************
//	【F9:参照】ICS住所検索ダイアログで選択された内容を表示
//		IN		CdlgAddressSearch*	参照ダイアログ情報
//		RET		なし
//********************************************************************************
void	CfrmUc041Karibarai::virFncReferenceSetICSData(CdlgAddressSearch* dlgAddd)
{
	int			intIndex;

	// 売却(買入)先の所在地1(住所)
	intIndex = CmnGetControlIndex( m_uInfo.intCurRow, ID_COL_041_ADADD1 );
	CmnDiagSetString( m_pDiag, intIndex, dlgAddd->m_AdAdd1, 0 );

	// 売却(買入)先の所在地2(住所)
	intIndex = CmnGetControlIndex( m_uInfo.intCurRow, ID_COL_041_ADADD2 );
	CmnDiagSetString( m_pDiag, intIndex, dlgAddd->m_AdAdd2, 0 );
}


//********************************************************************************
//	【F9:参照】参照ダイアログの操作によりテーブルを更新
//		IN		int			参照ダイアログの種別
//		RET		なし
//********************************************************************************
void	CfrmUc041Karibarai::virFncReferenceUpdateTbl( int intRefType, int nRefMode/*=0*/ )
{
	switch (intRefType) {
	
	// 科目
	case ID_DLGTYPE_KAMOKU:
		{
			// "uc_041_Karibarai"の更新
			CdbUcLstKamoku	rsKmk( m_pDB );
//@			rsKmk.UpdateDataTable(TBL_NAME_041, m_uInfo.intFormSeq);
			rsKmk.UpdateDataTable(m_uInfo.intFormSeq, nRefMode);
			rsKmk.Fin();
			break;
		}

	//	取引先
	case ID_DLGTYPE_ADDRESS:
		{
			CdbUcLstAddress		mfcRec( m_pDB );
//@			mfcRec.UpdateDataTable( TBL_NAME_041 );
			mfcRec.UpdateDataTableAll(m_RegAutoRef);
			mfcRec.Fin(); 
			break;
		}
	default:
		break;
	}	
}

// ウィンドゥ拡大・縮小時のフロート画面サイズ設定
void	CfrmUc041Karibarai::OnSize( UINT nType, int cx, int cy )
{
	CfrmUc000Common::OnSize( nType, cx, cy );

	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

// ウィンドウ消去
BOOL	CfrmUc041Karibarai::DestroyWindow()
{
	return CFormView::DestroyWindow();
}

//********************************************************************************
//	その他イベント処理
//********************************************************************************
BOOL	CfrmUc041Karibarai::PreTranslateMessage( MSG* pMsg )
{
//	int				intID;
	BOOL			blnMove = TRUE;		// カーソル移動可能/不可フラグ
	int				intOrgIndex;		// カーソル移動前のインデックス番号
	int				intMoveRow;			// カーソル移動先（行）
	int				intMoveCol;			// カーソル移動先（列）
// midori 160611 add -->
	int				rv=0;
// midori 160611 add <--
// midori 152364 add -->
	int				intRet=0;
// midori 152364 add <--
// midori 151405 add -->
	int				id=0;
// midori UC_0001 add -->
	int				nMaxDataCol=0;		// 1頁の最大行数
	int				intTotalType=0;		// 合計タイプ
// midori UC_0001 add <--
// midori 152134 17/10/07 add -->
	int				intCol=0;
	DIAGRAM_DATA	diadata;
// midori 152134 17/10/07 add <--
	CWnd* pWnd = this->GetFocus();

	if ( pWnd != NULL ){
		id = pWnd->GetDlgCtrlID();
		if(id == IDC_PAGEBACK_BUTTON1 || id == IDC_PAGENEXT_BUTTON1 || id == IDC_PAGENEW_BUTTON1 || 
			id == IDC_PAGEINSERT_BUTTON1 || id == IDC_ROWCOPY_BUTTON1 || id == IDC_ROWPASTE_BUTTON1 || 
			id == IDC_ROWINSERT_BUTTON1 || id == IDC_YOUSHIKIBACK_BUTTON1 || id == IDC_YOUSHIKINEXT_BUTTON1 )	{
			nG_MScroll = 0;
		}
	}
// midori 151405 add <--

	//	キーが押された？
	if ( pMsg->message == WM_KEYDOWN ){

		//	パラメータで分岐
		switch( pMsg->wParam ){
// midori 151405 del -->
//// midori 152364 add -->
//		// Enterキー
//		case VK_RETURN:
//			if(IsSpecialRow(m_ReadData[m_uInfo.intCurRow].m_FgFunc) == 1)	{
//				intRet = GetFocusPosition(VK_RETURN);
//				m_pDiag->SetPosition(intRet);
//				rv=1;
//			}
//			break;
//// midori 152364 add <--
// midori 151405 del <--
// midori 151405 add -->
		// Enterキー
		case VK_RETURN:
			if(nG_MScroll == 1)	{
				rv = CursorControl((short)pMsg->wParam,1);
			}
			else if(m_DataKakutei == FALSE && m_uInfo.intCurFocus == 1)	{
				if(IsSpecialRow(m_ReadData[m_uInfo.intCurRow].m_FgFunc) == 1)	{
					intRet = GetFocusPosition(VK_RETURN);
					m_pDiag->SetPosition(intRet);
					rv=1;
				}
			}
			break;
// midori 151405 add <--
		// PageUp，PageDownキー
		case VK_PRIOR:
		case VK_NEXT:
// midori 152390 add -->
			if(m_DataKakutei == FALSE)	{
// midori 152390 add <--
				// データ入力欄にフォーカスある場合のみ実行を許す
// midori 151405 del -->
				//if (m_uInfo.intCurFocus != 1) {
				//	// 何もせずに抜ける
				//	return CfrmUc000Common::PreTranslateMessage(pMsg);
				//}
// midori 151405 del <--
// midori 151405 add -->
				if (m_uInfo.intCurFocus != 1 && nG_MScroll == 0) {
					// 何もせずに抜ける
					break;
				}
// midori 151405 add <--

// midori 160611 del -->
//				// ComboBoxではPageUp，PageDownキーを実行しない又は計行である又は特殊行である
//				if ((CmnGetControlCol() != ID_COL_041_ADSEQ) || (CmnCheckTotalRow() == TRUE )){
// midori 160611 del <--
				// カレントインデックスを保存
				intOrgIndex = m_pDiag->GetPosition();

				// カレント行を取得
				if (m_uInfo.intCurCtlIndex < m_uInfo.intT1StartIndex) {
					intMoveRow = CmnGetControlRow();		// データ行の場合
					intMoveCol = m_uInfo.intCurCol;
				}
				else if (m_uInfo.intCurCtlIndex >= m_uInfo.intT2StartIndex) {
					intMoveRow = m_uInfo.intRowMax;			// 合計行②の場合(24)
					intMoveCol = m_uInfo.intCursolDefPos;	// 移動先がデータ行になるため、Combo以外へ移動したい
				}
				else {
					intMoveRow = m_uInfo.intRowMax - 1;		// 合計行①の場合(23)
					intMoveCol = m_uInfo.intCursolDefPos;	// 移動先がデータ行になるため、Combo以外へ移動したい
				}

// midori 152134 17/10/07 add -->
				intCol = CmnGetControlCol();
				// 例外処理：移動前に【新規作成】が選ばれている場合は値を元に戻す
				if (intCol == ID_COL_041_ADSEQ) {
					m_pDiag->GetData( m_uInfo.intCurCtlIndex , (LPUNKNOWN)&diadata );
					if(diadata.data_combo > m_CmbCnt)	{
						m_pDiag->ComboDropDown(m_uInfo.intCurCtlIndex, FALSE);
						CmnDiagSetCombo(m_pDiag, m_uInfo.intCurCtlIndex, m_uInfo.OldCombo);
					}
				}
// midori 152134 17/10/07 add <--

				// Scroll＋フォーカス移動先を取得
				if (pMsg->wParam == VK_PRIOR) {
					// PageUpの場合：Scroll＋フォーカス移動（行－１１）
					m_pDiag->ScrollPos(SB_PAGEUP , 0);
					intMoveRow = intMoveRow - 11;
					if (intMoveRow < 1) {
						blnMove = FALSE;					// 移動しない
					}
				}
				else {
					// PageDownの場合：Scroll＋フォーカス移動（行＋１１）
					m_pDiag->ScrollPos(SB_PAGEDOWN, 0);
					intMoveRow = intMoveRow + 11;
// midori UC_0001 del -->
//					if (intMoveRow > m_uInfo.intRowMax) {
// midori UC_0001 del <--
// midori UC_0001 add -->
					// カレント頁の合計行の種別を取得
					intTotalType = CmnGetControlTotalType();
					// カレント頁で"頁計と累計"を表示する場合、対象行は"-1"となる
					// オフ
					if(intTotalType == ID_OUTKEI_OFF)	{
						nMaxDataCol = m_uInfo.intRowMax;
					}
					// 頁計又は累計
					else if(intTotalType == ID_OUTKEI_PAGEKEI || intTotalType == ID_OUTKEI_RUIKEI) {
						nMaxDataCol = m_uInfo.intRowMax - 1;
					}
					// 頁計と累計
					else								{
						nMaxDataCol = m_uInfo.intRowMax - 2;
					}

					if (intMoveRow > nMaxDataCol) {
// midori UC_0001 add <--
						blnMove = FALSE;					// 移動しない
					}
				}

				// カーソル移動処理
				if (blnMove == TRUE) {
					// 頁計/累計行の制御は、当処理で制御する
					CmnDiagSetFocus(m_pDiag, intMoveRow, intMoveCol);
				}
				else {
					// 移動しない場合でも同じ位置にカーソルを再設定
					m_pDiag->SetPosition(intOrgIndex);
				}
// midori 152134 17/10/07 del -->
//// midori 160611 cor -->
//				//}
//// ---------------------
//				if(CmnGetControlCol() == ID_COL_041_ADSEQ)	{
//					rv=1;
//				}
//// midori 160611 cor <--
// midori 152134 17/10/07 del <--
// midori 152134 17/10/07 add -->
				if(intCol == ID_COL_041_ADSEQ)	{
					rv=1;
				}
// midori 152134 17/10/07 add <--
// midori 152390 add -->
			}
			else	{
				// Scroll
				if (pMsg->wParam == VK_PRIOR)	m_pDiag->ScrollPos(SB_PAGEUP , 0);
				else							m_pDiag->ScrollPos(SB_PAGEDOWN, 0);
			}
// midori 152390 add <--
			break;
/*
		// Tabキー（前頁ボタンで[Shift＋Tab]キー処理。これ以外は別のイベントで行う）
		case VK_TAB:
			// 現在フォーカスを持つコントロールのIDを取得
			intID = CmnGetDlgCtrlID();

			// 前頁ボタンで[Shift＋Tab]キーが押されたらカーソルセット
			if ((intID == IDC_PAGEBACK_BUTTON1) && (CmnCheckShiftKey() == TRUE)) {
				// データ部の最終行へ移動（頁計/累計行の制御を行う）
				CmnDiagSetFocus(m_pDiag, m_uInfo.intRowMax, ID_COL_041_NAIYOU);
				return 1;
			}
			break;
*/
// midori 151405 add -->
		// カーソル移動キー
		case VK_TAB:
		case VK_UP:
		case VK_DOWN:
		case VK_LEFT:
		case VK_RIGHT:
			if(nG_MScroll == 1)	{
				rv = CursorControl((short)pMsg->wParam,1);
			}
// 修正No.158259 add -->
			else if(m_DataKakutei == FALSE && m_Pagefocus == 0 && m_Titlefocus == 0)	{
				intCol = CmnGetControlCol();
				if(intCol == ID_COL_041_ADSEQ && (pMsg->wParam == VK_LEFT || pMsg->wParam == VK_RIGHT))	{
					rv = CursorControl((short)pMsg->wParam, 0);
				}
			}
// 修正No.158259 add <--
			break;
// midori 151405 add <--
		}
	}
// midori 160611 add -->
	if(rv == 1)	{
		return(TRUE);
	}
// midori 160611 add <--

	return CfrmUc000Common::PreTranslateMessage(pMsg);
}

//********************************************************************************
//	頁コントロール処理（Focus，EditOFF，Terminationイベント）
//********************************************************************************

// 頁コントロールのフォーカス設定
void	CfrmUc041Karibarai::FocusPagediag1()
{
//マウスクリックでキャレットが表示できないためコメントとする
//	// ボタンからTabキー移動した場合に必要
//	m_PageDiag.SetPosition( 1 );
}

// 頁コントロールのゲットフォーカス
void	CfrmUc041Karibarai::EditONPagediag1( short index )
{
// midori 151405 add -->
	// スクロールフラグをクリアする
	nG_MScroll = 0;
	m_Pagefocus = 1;
// midori 151405 add <--

	// 1行登録
//	if( CmnGetControlCol() != ID_COL_041_VAL ){
		if (virRecBufSaveData(m_uInfo.intCurPage, m_uInfo.intCurRow) == FUNCTION_UPDATE) {
			m_PageDiag.SetPosition(1);
		}
//	}
}

// 頁コントロールのロストフォーカス
void	CfrmUc041Karibarai::EditOFFPagediag1( short index )
{
// midori 155028 add -->
	// 会社切替時にメッセージを表示した場合、EditOffが走りDB書き込み時にエラーが発生していた
	if(m_EndView != FALSE)	return;
// midori 155028 add <--

	// 頁入力値を取得
	int	intTempPage = CmnDiagGetValue( &m_PageDiag, 1 );

// midori 151405 add -->
	m_Pagefocus = 0;
// midori 151405 add <--
	// 頁が変更された場合は画面更新が必要
	if ( m_uInfo.intCurPage != intTempPage ) {
		if ( ( intTempPage >= 1 ) && ( intTempPage <= m_uInfo.intMaxPage ) ) {
			// 範囲内：新ページに更新
			m_uInfo.intCurPage = intTempPage;

// 修正No.162446 del -->
			//// １頁の更新
			//virUpdateControlTblData();
			//// フォーカスをデータ入力欄に
			////CmnDiagSetFocus( m_pDiag, 1, 1 );	// 修正No.162381,162382 del
			//PostMessage(WM_USER_FOCUSSET, 1, 1);	// 修正No.162381,162382 add
// 修正No.162446 del <--
// 修正No.162446 add -->
			m_ChangeFont = TRUE;

			// データを再描画してフォーカスをデータ入力欄に
			PostMessage(WM_USER_REDRAWVIEW, 0, 0);
// 修正No.162446 add <--

			return;
		}
	}

	// ページ情報のみ更新："0001"→"1"で表示したい
	UpdateControlPage();
}

// 頁コントロールのキー操作
void	CfrmUc041Karibarai::TerminationPagediag1( short index, short nChar, short length, LPUNKNOWN data )
{
	int nMaxDataCol = 0;
	// カレント頁の合計行の種別を取得
	int intTotalType = CmnGetControlTotalType();
	
	// カレント頁で"頁計と累計"を表示する場合、対象行は"-1"となる
	switch (intTotalType) {
	case ID_OUTKEI_BOTH:			// 頁計と累計
		nMaxDataCol = m_uInfo.intRowMaxData - 1;
		break;
	default:
		nMaxDataCol = m_uInfo.intRowMaxData;
		break;
	}

	// フォーカス移動
	switch ( nChar ) {
	case VK_RETURN:
		// Return：頁確定のため
		CmnDiagSetFocus( m_pDiag, 1, 1 );
		break;
	case VK_TAB:
		// Tab/Shift+Tabにてフォーカス移動
		if ( CmnCheckShiftKey() == FALSE ) {
			// Tab：タイトルコントロールへ移動
// midori 160612 cor -->
			//m_TitleDiag.SetPosition(0);
// ---------------------
			if(m_DataKakutei == FALSE)	m_TitleDiag.SetPosition(0);
// midori 160612 cor <--
		}
		else {
			// Shift+Tab：データ部の最終行へ移動（頁計/累計行の制御を行う）
			//CmnDiagSetFocus(m_pDiag, m_uInfo.intRowMax, ID_COL_041_NAIYOU);
			CmnDiagSetFocus(m_pDiag, nMaxDataCol, ID_COL_041_NAIYOU);
		}
		break;
	case VK_DOWN:
		// タイトルコントロールへ移動
// midori 160612 cor -->
		//m_TitleDiag.SetPosition( 0 );
// ---------------------
		if(m_DataKakutei == FALSE)	m_TitleDiag.SetPosition(0);
// midori 160612 cor <--
		break;
	}
}

//********************************************************************************
//	タイトルコントロール処理（Focus，EditOFF，Terminationイベント）
//********************************************************************************

// タイトルコントロールのフォーカス設定
void	CfrmUc041Karibarai::FocusTitlediag1()
{
//マウスクリックでキャレットが表示できないためコメントとする
//	// ボタンからTabキー移動した場合に必要
//	m_TitleDiag.SetPosition( 0 );
}

// タイトルコントロールのゲットフォーカス
void	CfrmUc041Karibarai::EditONTitlediag1( short index )
{
// midori 151405 add -->
	// スクロールフラグをクリアする
	nG_MScroll = 0;
	m_Titlefocus = 1;
// midori 151405 add <--
	// 1行登録
//	if( CmnGetControlCol() != ID_COL_041_VAL ){
		if (virRecBufSaveData(m_uInfo.intCurPage, m_uInfo.intCurRow) == FUNCTION_UPDATE) {
			m_TitleDiag.SetPosition(0);
		}
//	}
}

// タイトルコントロールのロストフォーカス
void	CfrmUc041Karibarai::EditOFFTitlediag1 (short index )
{
// midori 151405 add -->
	m_Titlefocus = 0;
// midori 151405 add <--
	virUpdateTitle();
}

//	タイトル更新
void CfrmUc041Karibarai::virUpdateTitle()
{
	// 帳表タイトル取得してuc_inf_subを更新
	m_uInfo.strTitleName = m_clsFunc.DiagGetString( &m_TitleDiag, 0 );
	CmnUcInfSubSetTitleInfo( TRUE );
}

// タイトルコントロールのキーイベント
void	CfrmUc041Karibarai::TerminationTitlediag1( short index, short nChar, short length, LPUNKNOWN data )
{
	// フォーカス移動
	switch ( nChar ) {
	case VK_RETURN:
		// １行目の左上へ移動
		CmnDiagSetFocus( m_pDiag, 1, 1 );
		break;
	case VK_TAB:
		// Shiftキーの有無は？
		if ( CmnCheckShiftKey() == FALSE ) {
			// １行目の左上へ移動
			CmnDiagSetFocus( m_pDiag, 1, 1 );
		}
		else {
			// 頁コントロールへ移動
			m_PageDiag.SetPosition(1);
		}
		break;
	case VK_UP:
		// 頁コントロールへ移動
		m_PageDiag.SetPosition( 1 );
		break;
	case VK_DOWN:
		// データ部の初期位置へ移動
		CmnDiagSetFocus( m_pDiag, 1, m_uInfo.intCursolDefPos );
		break;
	case VK_DELETE:
		// タイトル削除
		m_TitleDiag.DataClear( 0 , TRUE );
		break;
	}

}

//********************************************************************************
//	「仮払金」データコントロール処理（EditON，EditOFF，Terminationイベント）
//********************************************************************************

// 「仮払金」データコントロールのゲットフォーカス
void	CfrmUc041Karibarai::EditONYoushikidiag1( short index )
{
	DIAGRAM_DATA	diadata;

// midori 151405 add -->
	// スクロールフラグをクリアする
	nG_MScroll = 0;
// midori 151405 add <--
// midori 152134 add -->
	m_ComboSgn = 0;
// midori 152134 add <--

	// フォーカスフラグON
	m_uInfo.intCurFocus = 1;

	// ポジション取得
	m_uInfo.intCurCtlIndex = m_pDiag->GetPosition();

	// 行・列取得
	CmnGetControlPosition( &m_uInfo.intCurRow , &m_uInfo.intCurCol, &m_uInfo.OldCol );

	switch (m_uInfo.intCurCol) {
	case ID_COL_041_ADSEQ:	// 科目
		// コンボボックスのオフセット値取得
		m_pDiag->GetData( m_uInfo.intCurCtlIndex , (LPUNKNOWN)&diadata );
// 要通知案件修正 21/07/09 del -->
		//m_uInfo.OldCombo = diadata.data_combo;
// 要通知案件修正 21/07/09 del <--
// 要通知案件修正 21/07/09 add -->
		if(diadata.data_combo <= m_CmbCnt)	{
			// 選択しているのが【新規作成】であれば保存しない
			// 1.キー操作で【新規作成】までもっていく。
			// 2.欄外の例えば今回追加したガイド等をクリックする（コンボは閉じるが【新規作成】のまま）
			// 3.ウインドウの右下をドラッグして少し小さくする（【新規作成】のまま）
			// 4.[Ins 様式切替] 等、EditOff が発生する動作を行う
			// 5.科目追加ダイアログが開かれていて閉じるともう一度表示される
			// ※3の操作でEditOnが発生して OldCombo に【新規作成】が保存される
			m_uInfo.OldCombo = diadata.data_combo;
		}
// 要通知案件修正 21/07/09 add <--
		break;
	default:
		// 現在の入力値を取得
		m_uInfo.OldstrDiag = m_clsFunc.DiagGetString( m_pDiag, m_uInfo.intCurCtlIndex );
		break;
	}


	// 行移動確認
	if ( /*( m_uInfo.OldRow != CmnGetControlRow() ) &&*/ ( m_uInfo.OldRow > 0 ) ) {
		if ( m_uInfo.OldPage == m_uInfo.intCurPage && m_uInfo.OldPage > 0 ) {
//			if( CmnGetControlCol() != ID_COL_041_VAL ){
			// 所在地1(住所)→所在地2(住所)　所在地2(住所)→所在地1(住所)移動は保存しない
			BOOL adadd1_to_adadd2 = IsSaveMoveReference(ID_COL_041_ADADD1, ID_COL_041_ADADD2);
			// [Home]キー押下の住所検索ダイアログの戻り（フォーカスをセットしている）は保存しない（改良依頼No15-1613）
			BOOL adadd1_to_adadd1 = IsSaveMoveReference(ID_COL_041_ADADD1, ID_COL_041_ADADD1);	// 所在地1(住所)→所在地1(住所)
			BOOL adadd2_to_adadd2 = IsSaveMoveReference(ID_COL_041_ADADD2, ID_COL_041_ADADD2);	// 所在地2(住所)→所在地2(住所)　所在地2から住所が長すぎる場合、ここがFALSEになる

			// データ保存
			if((adadd1_to_adadd2 != FALSE) && (adadd1_to_adadd1 != FALSE) && (adadd2_to_adadd2 != FALSE)){	
				time_t now = time(NULL);
				struct tm pnow;
				::ZeroMemory(&pnow, sizeof(pnow));
				localtime_s(&pnow, &now);
				CString time;
				time.Format("%d:%d:%d\n",pnow.tm_hour,pnow.tm_min,pnow.tm_sec);

				TRACE("virRecBufSaveData start " + time);
				// カレント行登録＋再表示
				if ( virRecBufSaveData(m_uInfo.intCurPage , m_uInfo.OldRow ) == FUNCTION_UPDATE ) {
					// カーソル移動処理
					CmnDiagSetFocus( m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol );
				}
			}
//			}
		}
	}

	// ボタン操作
	CmnChangeButtonEnable(1);
}

// データコントロールのロストフォーカス
void	CfrmUc041Karibarai::EditOFFYoushikidiag1( short index )
{
	if( m_EndView != FALSE )	return;
	virInputDecisionEditOFF();

// midori 152134 del -->
//// midori 161106 add -->
//	ComboNew(index);
//// midori 161106 add <--
// midori 152134 del <--
// midori 152134 add -->
	if(m_uInfo.intCurCol == ID_COL_041_ADSEQ)	{
		if(m_ComboSgn != 1)	{
			ComboNew(index);
		}
	}
	m_ComboSgn = 0;
// midori 152134 add <--
}

//	入力確定
void CfrmUc041Karibarai::virInputDecisionEditOFF()
{
		// フォーカスフラグOFF
	m_uInfo.intCurFocus = 0;

	//	入力項目を強制的に確定
	CmnDiagSetEnterReDraw( m_pDiag, m_pDiag->GetPosition() );

	// 自動一括集計行なら、変更できてしまうComboBoxの値を元に戻す
// No.200905 del -->
	//if (m_ReadData[m_uInfo.intCurRow].m_FgFunc == ID_FGFUNC_IKKATUAUTO) {
// No.200905 del <--
// No.200905 add -->
// 改良No.21-0086,21-0529 cor -->
	//if (KamokuRowEnableSgn(m_pDB,0) == 0 && m_ReadData[m_uInfo.intCurRow].m_FgFunc == ID_FGFUNC_IKKATUAUTO) {
// ------------------------------
	if(KamokuRowEnableSgn(m_pDB, 0, m_uInfo.intFormSeq) == 0 && m_ReadData[m_uInfo.intCurRow].m_FgFunc == ID_FGFUNC_IKKATUAUTO) {
// 改良No.21-0086,21-0529 cor <--
// No.200905 add <--
		if (m_uInfo.intCurCol == ID_COL_041_ADSEQ) {
			//	前回の値に戻す
			CmnDiagSetCombo(m_pDiag, CmnGetControlIndex(m_uInfo.intCurRow, ID_COL_041_ADSEQ), m_uInfo.OldCombo);
		}
	}

	// 行取得
	if ( m_uInfo.intCurCtlIndex < m_uInfo.intT1StartIndex ) {
		m_uInfo.OldRow = CmnGetControlRow();
	}
	else{
		m_uInfo.OldRow = -1;
	}

	// 金額欄の入力が確定した時点で反映
//	if (  m_uInfo.OldRow > 0 ){
//		//if( CmnGetControlCol() == ID_COL_041_VAL ){
//			//if ( m_uInfo.OldPage == m_uInfo.intCurPage ) {
//				// カレント行登録＋再表示
//		// 項目複写かつ相手先の所在地下段のときはセーブしない
//		int current_column = CmnGetControlCol();
//		if(m_isF6 == FALSE && !(current_column == ID_COL_041_ADADD1)){
//				virRecBufSaveData(m_uInfo.intCurPage , m_uInfo.OldRow );
//		}
//			//}
//		//}
//	}

	// ページ番号取得
	m_uInfo.OldPage = m_uInfo.intCurPage;

	// ボタン操作
	CmnChangeButtonEnable(0);
}

// 要通知案件修正 21/07/09 add -->
void CfrmUc041Karibarai::virActivateFrameK(UINT nState, CFrameWnd* pDeactivateFrame)
{
	int		index = 0;
	CWnd*	pWnd = this->GetFocus();

	if(pWnd != NULL)	{
		if(m_DataKakutei == FALSE)	{
			if(m_uInfo.intCurCol == ID_COL_041_ADSEQ)	{
				virKamokuAddCancel();
			}
		}
	}
}

void CfrmUc041Karibarai::virKamokuAddCancel()
{
	if(m_DataKakutei == TRUE)	return;

	// ポジション取得
	m_uInfo.intCurCtlIndex = m_pDiag->GetPosition();
	// 行・列取得
	CmnGetControlPosition(&m_uInfo.intCurRow,&m_uInfo.intCurCol,&m_uInfo.OldCol);
	// 科目列なら
	if(m_uInfo.intCurCol == ID_COL_041_ADSEQ && m_F9OnSw != TRUE)	{
		DIAGRAM_DATA	diadata;
		m_pDiag->GetData(m_uInfo.intCurCtlIndex,(LPUNKNOWN)&diadata);
		// 【新規作成】が選択されていれば
		if(diadata.data_combo > m_CmbCnt)	{
			// 前回の値に戻す
			CmnDiagSetCombo(m_pDiag,CmnGetControlIndex(m_uInfo.intCurRow,ID_COL_041_ADSEQ),m_uInfo.OldCombo);
		}
	}
}
// 要通知案件修正 21/07/09 add <--

// 預貯金データコントロールのキーイベント
void	CfrmUc041Karibarai::TerminationYoushikidiag1( short index, short nChar, short length, LPUNKNOWN data )
{
	//--------------------------------------------------------------------------------
	//	キー操作仕様
	//		Deleteキー		: カレントの項目をクリア（ComboBoxはクリアしない）
	//		Tab/Shift+Tab	: １つ前/次の項目へ移動
	//						先頭の項目（左上）で"Shift+Tab"は、タイトルへ移動
	//						最後の項目（合計行②）で"Tab"は、前頁ボタンor頁項目へ移動
	//		Return：		次の項目へ移動
	//						最後の項目（合計行②）では移動しない
	//		矢印キー：		←/→キーで１つ前/次の項目へ移動
	//						↑/↓キーで１つ上/下の項目へ移動
	//						なお、矢印キーでは先頭/最後のセルでも別のコントロールへ移動しない
	//		例外処理：		ComboBoxで←/→キーが押されると値を戻す
	//
	//	カーソル移動における前提条件
	//		1.合計欄①，②行は、頁計/累計の名称欄のみ移動可能（ただし入力不可）
	//		2.小計などの特殊行は対象項目へ移動可能（ただし入力不可）
	//--------------------------------------------------------------------------------

	// 現在の列番号を取得
	int intCol = CmnGetControlCol();

	switch ( nChar ) {
	// Deleteキー
	case VK_DELETE:
		// 項目をクリア（ただしComboBoxはクリアしない）
		switch (m_ReadData[m_uInfo.intCurRow].m_FgFunc) {
		case ID_FGFUNC_SYOKEI:				// 小計
		case ID_FGFUNC_SYOKEINULL:			// 小計（空行）
		case ID_FGFUNC_CHUKEI:				// 中計
		case ID_FGFUNC_CHUKEINULL:			// 中計（空行）
		case ID_FGFUNC_IKKATUAUTO:			// 一括集計金額行（自動）
		case ID_FGFUNC_PAGEKEI:				//	頁計
		case ID_FGFUNC_RUIKEI:				//	累計
			break;
		default:
			switch ( intCol ) {
			case ID_COL_041_ADSEQ:	// 科目
				break;
			default:
				m_pDiag->DataClear( m_uInfo.intCurCtlIndex, TRUE );
				break;
			}
			break;
		}
		break;

// midori 151405 del -->
	//// カーソル移動キー
	//case VK_TAB:
	//case VK_RETURN:
	//case VK_UP:
	//case VK_DOWN:
	//case VK_LEFT:
	//case VK_RIGHT:
	//	// 例外処理：移動前にComnoBoxで←/→キーが押下された場合は値を元に戻す
	//	if ( ( nChar == VK_LEFT ) || ( nChar == VK_RIGHT ) ) {
	//		// ComboBoxは選択内容を確定させない
	//		switch ( intCol ) {
	//		case ID_COL_041_ADSEQ:	// 科目
	//			m_pDiag->ComboDropDown( m_uInfo.intCurCtlIndex, FALSE );
	//			CmnDiagSetCombo( m_pDiag, m_uInfo.intCurCtlIndex, m_uInfo.OldCombo );
	//			break;
	//		default:
	//			break;
	//		}
	//	}

	//	// 期末現在高で値が入力された場合の金額確定処理
	//	//if( intCol == ID_COL_041_VAL ){		
	//	//	switch(nChar){
	//	//		// 行を跨がないカーソル移動
	//	//		case VK_TAB:
	//	//		case VK_RIGHT:
	//	//		case VK_LEFT:
	//	//		case VK_RETURN:
	//	//			int intRow = CmnGetControlRow();
	//	//			// 登録処理
	//	//			virRecBufSaveData(m_uInfo.intCurPage , intRow );
	//	//			break;
	//	//	}
	//	//}

	//	// フォーカス移動先を取得
	//	int intRet = GetFocusPosition( nChar );
	//	switch ( intRet ) {
	//	case -1:	// 先頭項目でShift+Tabキーのため、タイトルへ移動
	//	case -4:	// 先頭行で↑or先頭項目で←は、タイトルへ移動
	//		m_TitleDiag.SetPosition( 0 );
	//		break;
	//	case -2:	// 最終項目でTabキーのため、頁コントロールへ移動
	//		m_PageDiag.SetPosition(1);
	//		break;
	//	case -3:	// ComboBoxで↑↓キーのため何もしない
	//		break;
	//	default:	// フォーカス移動を行う
	//		m_pDiag->SetPosition( intRet );
	//		break;
	//	}

	//	// ポジション取得
	//	m_uInfo.intCurCtlIndex = m_pDiag->GetPosition();
	//	break;
// midori 151405 del <--
// midori 151405 add -->
	// カーソル移動キー
	case VK_TAB:
	case VK_RETURN:
	case VK_UP:
	case VK_DOWN:
	case VK_LEFT:
	case VK_RIGHT:
		CursorControl(nChar,0);
		break;
// midori 151405 add <--
	}
}

// midori 151405 add -->
void CfrmUc041Karibarai::VScrollYoushikidiag1(short mode)
{
	if(mode == 8 && m_DataKakutei == FALSE && m_Pagefocus == 0 && m_Titlefocus == 0)	{
// midori UC_0002 add -->
		virInputDecisionEditOFF();
// midori UC_0002 add <--
		nG_MScroll=1;
	}
}
// midori 151405 add <--

//********************************************************************************
//	フォーカス移動処理（メイン）
//		IN		int		キーコード（VK_UP，VK_RIGHT，VK_DOWN，VK_LEFT，VK_RETURN，VK_TAB）
//		RET		int		0～:移動先コントロールのインデックス番号
//						-1: 先頭項目で"Shift+Tab"のためタイトルへ移動
//						-2: 最終項目で"Shift"のため別コントロールへ移動
//						-3: ComboBoxで"↑/↓"はキー操作を無視する
//						-4: 先頭行で↑or先頭項目で←はタイトルへ移動
//********************************************************************************
int	CfrmUc041Karibarai::GetFocusPosition( int nChar )
{
	int		intRow;					// カレント行番号
	int		intCol;					// カレント列番号
	int		intVector[4];			// 矢印キーの移動先（0:↑，1:→，2:↓，3:←）
	int		intNext = 0;			// フォーカス移動先
	int		intRowChange = 0;		// 行をまたぐ場合のインデックス増減数

	// カレント行＋列番号を取得
	CmnGetControlPosition(&intRow, &intCol);

	// 行をまたぐ場合のインデックス増減数（摘要→次の行の金融機関名）
	intRowChange = m_uInfo.intCtlStartIndex + 1;
	
	//	計行じゃない？
	if ( CmnCheckTotalRow() == FALSE ){
		
		// 「種類」はComboBoxのため、「↑，↓」キーではカーソルを移動しない
		if ( intCol == ID_COL_041_ADSEQ ){
		
			if ((nChar == VK_UP) || (nChar == VK_DOWN)) {
				// キー操作を無視する
				return -3;
			}
		}
	}

	//---------------------------------------------------------------- 
	//	矢印キーによる移動先を取得（合計欄は次のステップで対応）
	//	基本的にReturn，Tab，Shift+Tabは、←，→キーと同じなので省略
	//	（※特殊行は除く）
	//---------------------------------------------------------------- 
	if ( intCol == ID_COL_041_ADSEQ ){
		// カレント列が「科目」の場合
		intVector[0] = m_uInfo.intCurCtlIndex;							// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + m_uInfo.intColMax;		// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - intRowChange;			// ←
	}
	else if ( intCol == ID_COL_041_ADNAME1 ){
		// カレント列が「名称１」の場合
		intVector[0] = m_uInfo.intCurCtlIndex - m_uInfo.intColMax;		// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + m_uInfo.intColMax;		// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;						// ←
	}
	else if ( intCol == ID_COL_041_ADADD1 ){
		// カレント列が「住所１」の場合
		intVector[0] = m_uInfo.intCurCtlIndex - m_uInfo.intColMax + 1;	// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + 1;						// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;						// ←
	}
	else if ( intCol == ID_COL_041_ADADD2 ){
		// カレント列が「住所２」の場合
		intVector[0] = m_uInfo.intCurCtlIndex - 1;						// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + m_uInfo.intColMax - 1;	// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;						// ←
	}
	else if ( intCol == ID_COL_041_NAIYOU ){
		// カレント列が「取引の内容」の場合
		intVector[0] = m_uInfo.intCurCtlIndex - m_uInfo.intColMax;		// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + intRowChange;			// →
		intVector[2] = m_uInfo.intCurCtlIndex + m_uInfo.intColMax;		// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;						// ←
	}
	else{
		// その他の行の場合
		intVector[0] = m_uInfo.intCurCtlIndex - m_uInfo.intColMax;		// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + m_uInfo.intColMax;		// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;						// ←
	}

	//---------------------------------------------------------------- 
	//	押下キーから移動先を取得（合計欄は次のステップで対応）
	//	基本的にReturn，Tab，Shift+Tabは、←，→キーと同じ
	//---------------------------------------------------------------- 
	switch (nChar) {
	case VK_UP:
		intNext = intVector[0];			// 「↑」キー押下時
		break;
	case VK_RIGHT:
		intNext = intVector[1];			// 「→」キー押下時
		break;
	case VK_DOWN:
		intNext = intVector[2];			// 「↓」キー押下時
		break;
	case VK_LEFT:
		intNext = intVector[3];			// 「←」キー押下時
		break;
	case VK_RETURN:	// Enterキーでカーソルを下に移動するモード対応
		if(m_ReturnMoveDown)	intNext = intVector[2];			//	Enterキーでカーソルを下に移動するチェック時は「↓」キーと同じ
		else					intNext = intVector[1];			// 「Return」キー押下時：「→」キーと同じ
		break;
	case VK_TAB:
		if (CmnCheckShiftKey() == FALSE) {
			intNext = intVector[1];		// 「Tab」キー押下時：「→」キーと同じ
		}
		else {
			// 例外処理：先頭項目で"Shift+Tab"は別コントロールへ移動
			// それ以外は「←」キーと同じ
			if ((intRow <= 1) && (intCol == ID_COL_041_ADSEQ)) {
				return -1;
			}
			intNext = intVector[3];		// 「Shift+Tab」キー押下時：「→」キーと同じ
		}
		break;
	}

	if (intNext >= 0) {
		// 当処理で合計行を考慮したカーソル移動先を確定させる
		intNext = GetTotalRowIndex(nChar, m_uInfo.intCurCtlIndex, intNext);
	}
	else {
		// コントロールのフィールド外（上へ）へはタイトルに移動する
		return -4;
	}

	return intNext;
}

//********************************************************************************
//	フォーカス移動処理（合計欄を考慮）
//		IN		int		キーコード（VK_UP，VK_RIGHT，VK_DOWN，VK_LEFT，VK_RETURN，VK_TAB）
//				int		カレントのコントロールインデックス
//				int		合計欄を考慮していない移動先のインデックス番号
//		RET		int		合計欄を考慮した移動先ののインデックス番号
//********************************************************************************
int	CfrmUc041Karibarai::GetTotalRowIndex( int nCharOrg ,int intIndex ,int intNext )
{
	int		nCharTemp;				// キーコード（VK_RETURN，VK_TABを矢印キーに置き換え）
	int		nNowRow;				//	現在の行
	int		nNextRow;				//	移動先の行
	int		nBorder[3];				//	境界
	int		nRet;					//	戻値

	// キーコード置き換え（VK_RETURN，VK_TABを矢印キーに置き換え）
	switch( nCharOrg ){
	case VK_RETURN:			// Returnキーなら→キーと同じ
		nCharTemp = VK_RIGHT;
		break;
	case VK_TAB:			// Tabキーなら→キーと同じ，Shift+Tabなら←キーと同じ
		nCharTemp = VK_RIGHT;
		if ( CmnCheckShiftKey() == TRUE ) {
			nCharTemp = VK_LEFT;
		}
		break;
	default:
		nCharTemp = nCharOrg;
		break;
	}
	
	nNowRow = CmnGetControlRow( intIndex );	//	カレント行を取得
	nNextRow = CmnGetControlRow( intNext );	//	移動先行を取得（暫定）
	nRet = intNext;							//	次の移動先を戻値として取得
	
	//	現在の頁の計の種別で分岐
	switch( CmnGetControlTotalType() ){
	case ID_OUTKEI_OFF:			//	オフ
		nBorder[0] = m_uInfo.intRowMax;			//	データ行
		nBorder[1] = 0;							//	頁計（なし）
		nBorder[2] = 0;							//	累計（なし）
		break;
	case ID_OUTKEI_PAGEKEI:		//	頁計
	case ID_OUTKEI_RUIKEI:		//	累計

		//	現在行が計行？
		if ( intIndex >= m_uInfo.intT1StartIndex ){
			nNowRow = m_uInfo.intRowMax;
		}
		
		//	移動先行が計行？
		if ( intNext >= m_uInfo.intT1StartIndex ){
			nNextRow = m_uInfo.intRowMax;
		}

		nBorder[0] = m_uInfo.intRowMax - 1;		//	データ行
		nBorder[1] = m_uInfo.intRowMax;			//	頁計
		nBorder[2] = 0;							//	累計（なし）
		break;
	case ID_OUTKEI_BOTH:		//	頁計＆累計

		//	現在行が計２？
		if ( intIndex >= m_uInfo.intT2StartIndex ){
			nNowRow = m_uInfo.intRowMax;
		}
		//	現在行が計１？
		else if ( intIndex >= m_uInfo.intT1StartIndex ){
			nNowRow = m_uInfo.intRowMax - 1;
		}
		
		//	移動先行が計２？
		if ( intNext >= m_uInfo.intT2StartIndex ){
			nNextRow = m_uInfo.intRowMax;
		}
		//	移動先行が計１？
		else if ( intNext >= m_uInfo.intT1StartIndex ){
			nNextRow = m_uInfo.intRowMax - 1;
		}

		nBorder[0] = m_uInfo.intRowMax - 2;		//	データ行
		nBorder[1] = m_uInfo.intRowMax - 1;		//	頁計
		nBorder[2] = m_uInfo.intRowMax;			//	累計
		break;
	}

	//	現在、データ行に居る
	if ( nNowRow <= nBorder[0] ){
		
		//	移動先が最終行の最終列でかつ「Tab」が押されてる？
		if (( intNext > CmnGetControlIndex( m_uInfo.intRowMax, ID_COL_041_NAIYOU ) ) && ( nCharOrg == VK_TAB )){
			return( -2 );
		}
		//	データ行を超えた？かつ計行あり？
		else if (( nNextRow > nBorder[0] ) && ( nBorder[1] > 0 )){
			//	計行に移動
			//nRet = CmnGetControlIndex( ( nBorder[1] + 2 ), ID_COL_041_ADSEQ );
			if (nCharOrg == VK_TAB) {
				return -2;
			}
		}
	}
	//	現在、頁計に居る
	else if ( nNowRow == nBorder[1] ){

		// すでに合計行にいるので、"intNext"は無視する
		switch ( nCharTemp ){
		case VK_UP:			// ↑：23行目のデータ行の支店名項目へ移動
			nRet = CmnGetControlIndex( nBorder[0], ID_COL_041_ADNAME1 );
			break;
		case VK_LEFT:		// ←/Shift+Tab：23行目のデータ行の摘要項目へ移動
			nRet = CmnGetControlIndex( nBorder[0], ID_COL_041_NAIYOU );
			break;
		default:			// →/↓/Return/Tab：25行目の合計行②へ移動

			//	累計あり？
			if ( nBorder[2] == 0 ){
				// 例外処理：最終項目で"Tab"は別コントロールへ移動
				// それ以外は「→」キーと同じ
				if (nCharOrg == VK_TAB) {
					return -2;
				}
			}
			//	累計なし
			else{
				nRet = CmnGetControlIndex( ( nBorder[2] + 2 ), ID_COL_041_ADSEQ );
			}
			break;
		}
	}
	//	現在、累計に居る場合
	else if ( nNowRow == nBorder[2] ){

		// すでに合計行にいるので、"intNext"は無視する
		switch ( nCharTemp ){
		case VK_UP:			// ↑：24行目の合計行① or 24行目のデータ行の支店名項目へ移動
		case VK_LEFT:		// ←/Shift+Tab：24行目の合計行① or 24行目のデータ行の摘要項目へ移動
			nRet = CmnGetControlIndex( ( nBorder[1] + 2 ), ID_COL_041_ADSEQ );
			break;
		default:			// →/↓：移動できない
			// 例外処理：最終項目で"Tab"は別コントロールへ移動
			// それ以外は「→」キーと同じ
			if (nCharOrg == VK_TAB) {
				return -2;
			}
			break;
		}
	}
	
	//	戻値を返す
	return( nRet );
}

//********************************************************************************
//	一括集計金額行のデータ設定
//		IN		int					呼び出し元（0:ソート，1:一括金額参照，2:特殊行挿入）
//				CfrmUc000Common*	テーブル情報
//				CString				名称
//				CALCKEI_INFO		一括集計金額情報
//				SORTKAMOKU_INFO		科目情報
//		RET		なし
//********************************************************************************
void	CfrmUc041Karibarai::virTblEditIkkatuLine( int nType, CdbUc000Common* rsData,
									CString strName, CALCKEI_INFO uCalcKei, SORTKAMOKU_INFO uKamoku )
{
	CdbUc041Karibarai*	mfcRec = (CdbUc041Karibarai*)rsData;

	switch (nType) {
	case	0:
		// ソートの一括集計処理 ･･･ 科目の無い帳表はいらない
		mfcRec->m_AdName1 = strName;				// 名称：名称２
		mfcRec->m_Val = uCalcKei.strKei[0];			// 金額："0:Val"固定

		mfcRec->m_KnSeq = uKamoku.intKnSeq;			// 科目情報
		mfcRec->m_KnOrder = uKamoku.intKnOrder;		// 科目情報
		mfcRec->m_KnName = uKamoku.strKnName;		// 科目情報
// midori 152137 add -->
		mfcRec->m_KnKana = uKamoku.strKnKana;		// 科目情報（カナ)
// midori 152137 add <--
		break;
	case	1:
		// 一括金額参照の戻す処理
		mfcRec->m_AdName1 = strName;				// 名称：名称２
		mfcRec->m_Val = uCalcKei.strKei[0];			// 金額："0:Val"固定
		break;
	case	2:
		// 特殊行挿入の一括金額選択時
		mfcRec->m_AdName1 = strName;				// 名称：名称２
		break;
	}
}

//********************************************************************************
//	ソート項目を再設定後に、ソートを実行する
//		IN		CdlgSort*			ソートダイアログ情報
//				CdbUc000Common*		テーブル情報
//		RET		int					実行結果（FUNCTION_OK, FUNCTION_NG）
//********************************************************************************
int	CfrmUc041Karibarai::virTblSortSubSortProc( CdlgSort* pCdlgSort, CdbUc000Common* rsData )
{
	CdbUc041Karibarai*	rs;
	CString				strFilter;
	CString				strSort;

	// ソート項目の再設定（科目）		→ 帳表により実行有無が異なる
	CdbUcLstKamoku	rsKmk( m_pDB );
//@	rsKmk.UpdateDataTable(TBL_NAME_041, m_uInfo.intFormSeq);
	rsKmk.UpdateDataTable(m_uInfo.intFormSeq);
	rsKmk.Fin();

	// ソート項目の再設定（取引先）		→ 帳表により実行有無が異なる
	CdbUcLstAddress	rsAdd( m_pDB );
	rsAdd.UpdateDataTable( m_uInfo.intFormSeq, m_RegAutoRef );
	rsAdd.Fin();

	// ソート条件取得
	CmnTblSortSubGetSortParam(pCdlgSort, &strFilter, &strSort);

	rs = (CdbUc041Karibarai*)rsData;

	// 対象フィールドがNULLならNULL文字列にする：文字列連結のソート時は必須
	rs->UpdateDataTableWork();

	// ソート実行
	return rs->RequerySortParameter(strFilter, strSort);
}

/**********************************************************************
	RecBufSetData(CdbUc041Karibarai* prmDbRec)
		ローカルのレコードバッファに、1行分のデータを格納する

	引数
		CdbUc041Karibarai*	prmDbRec	レコードセット(データがあること）
	戻値
		int		成功/失敗
				FUNCTION_OK
***********************************************************************/
int	CfrmUc041Karibarai::RecBufSetData( CdbUc041Karibarai* prmDbRec )
{
	// 1ページ分のレコードバッファの該当行にデータを格納する
	int	trgRow = prmDbRec->m_NumRow;
	m_ReadData[trgRow].m_Seq = prmDbRec->m_Seq;
	m_ReadData[trgRow].m_NumPage = prmDbRec->m_NumPage;
	m_ReadData[trgRow].m_NumRow = prmDbRec->m_NumRow;
	m_ReadData[trgRow].m_FgFunc = prmDbRec->m_FgFunc;
	m_ReadData[trgRow].m_FgShow = prmDbRec->m_FgShow;
	m_ReadData[trgRow].m_KeiStr = prmDbRec->m_KeiStr;
	m_ReadData[trgRow].m_NumGroup = prmDbRec->m_NumGroup;
	m_ReadData[trgRow].m_KnSeq = prmDbRec->m_KnSeq;
	m_ReadData[trgRow].m_KnOrder = prmDbRec->m_KnOrder;
	m_ReadData[trgRow].m_KnName = prmDbRec->m_KnName;
	m_ReadData[trgRow].m_AdSeq = prmDbRec->m_AdSeq;
	m_ReadData[trgRow].m_AdOrder = prmDbRec->m_AdOrder;
	m_ReadData[trgRow].m_AdName1 = prmDbRec->m_AdName1;
	m_ReadData[trgRow].m_AdName2 = prmDbRec->m_AdName2;
	m_ReadData[trgRow].m_AdAdd1 = prmDbRec->m_AdAdd1;
	m_ReadData[trgRow].m_AdAdd2 = prmDbRec->m_AdAdd2;
	m_ReadData[trgRow].m_Kankei = prmDbRec->m_Kankei;
	m_ReadData[trgRow].m_Val = prmDbRec->m_Val;
	m_ReadData[trgRow].m_Naiyou = prmDbRec->m_Naiyou;
// midori 152137 add -->
	m_ReadData[trgRow].m_KnKana = prmDbRec->m_KnKana;	// 科目
	m_ReadData[trgRow].m_AdKana = prmDbRec->m_AdKana;	// 相手先(名称)
// midori 152137 add <--

	return FUNCTION_OK;
}

/**********************************************************************
	RecBufClearAllData()
		ローカルの1ページ分のレコードバッファを初期化する

	引数
		なし
	戻値
		int		成功/失敗
				FUNCTION_OK
***********************************************************************/
int	CfrmUc041Karibarai::RecBufClearAllData()
{
	// 1ページ分のレコードバッファを初期化する
	// 1行目から、最終行(計行も含む)までクリア
	// 配列の添え字は、行番号をそのまま使用
	// 使用していないが配列[0]も初期化している
	for ( int trgRow = 0; trgRow <= m_uInfo.intRowMax; trgRow++ ) {
		RecBufClearData( &m_ReadData[trgRow] );
	}

	return FUNCTION_OK;
}

/**********************************************************************
	RecBufClearAllData()
		1レコード分のデータを初期化する

	引数
		P_REC_UC_041_KARIBARAI inRecData レコードへのポインタ
	戻値
		int		成功/失敗
				FUNCTION_OK
***********************************************************************/
int	CfrmUc041Karibarai::RecBufClearData( P_REC_UC_041_KARIBARAI inRecData )
{
	inRecData->m_Seq = 0;
	inRecData->m_NumPage = 0;
	inRecData->m_NumRow = 0;
	inRecData->m_FgFunc = 0;
	inRecData->m_FgShow = 0;
	inRecData->m_KeiStr = _T("");
	inRecData->m_NumGroup = 0;
	inRecData->m_KnSeq = 0;
	inRecData->m_KnOrder = 0;
	inRecData->m_KnName = _T("");
	inRecData->m_AdSeq = 0;
	inRecData->m_AdOrder = 0;
	inRecData->m_AdName1 = _T("");
	inRecData->m_AdName2 = _T("");
	inRecData->m_AdAdd1 = _T("");
	inRecData->m_AdAdd2 = _T("");
	inRecData->m_Kankei = _T("");
	inRecData->m_Val = _T("");
	inRecData->m_Naiyou = _T("");
	inRecData->m_ShowKeiZero = 0;	//０円計表示フラグ
// midori 152137 add -->
	inRecData->m_KnKana = _T("");	// 科目
	inRecData->m_AdKana = _T("");	// 相手先(名称)
// midori 152137 add <--

	return FUNCTION_OK;
}

/**********************************************************************
	virRecBufSaveData()
		指定された行のデータをテーブルに保存

	引数
		int		inPage	登録するレコードのページ番号
		int		inRow	登録するレコードの行番号
	戻値
		int				成功(更新あり)/成功(更新なし)/失敗
						FUNCTION_UPDATE	: 成功(更新あり)
						FUNCTION_OK		: 成功(更新なし)
						FUNCTION_NG		: 失敗
***********************************************************************/
int	CfrmUc041Karibarai::virRecBufSaveData( int inPage, int inRow )
{
	REC_UC_041_KARIBARAI	tmpSaveData;	// 画面データ1レコード格納用

	RecBufClearData( &tmpSaveData );	// レコードを初期化
	
	// 頁計/累計行は登録しない
	if ((m_ReadData[inRow].m_FgFunc == ID_FGFUNC_PAGEKEI) || (m_ReadData[inRow].m_FgFunc == ID_FGFUNC_RUIKEI)) {
		return FUNCTION_OK;
	}

	///////////////////////////////////////////////////////////////
	// コンポーネント(画面上の1レコード分の情報)を仮変数へ格納
	DIAGRAM_DATA	diadata;	// ICSDiag構造体
	int				intIndex;	// 対象行の各インデックス

	// 初期化
	m_clsFunc.DiagInit( &diadata );

	// 科目
	intIndex = CmnGetControlIndex( inRow, ID_COL_041_ADSEQ );
	m_pDiag->GetData( intIndex , (LPUNKNOWN)&diadata );
// midori 152137 del -->
//	tmpSaveData.m_KnOrder = diadata.data_combo;
// midori 152137 del <--
// midori 152137 add -->
	// コンボボックスの値をキーにKnOrder（科目順序）を取得
	m_SortMap2.Lookup(diadata.data_combo,tmpSaveData.m_KnOrder);
// midori 152137 add <--

	// 名称1(氏名)
	CStringArray strArray;
	intIndex = CmnGetControlIndex( inRow, ID_COL_041_ADNAME1 );
// 158138 del -->
	//m_clsFunc.StrDivision( m_clsFunc.DiagGetString( m_pDiag, intIndex ), &strArray, 2 );
// 158138 del <--
// 158138 add -->
	m_clsFunc.StrDivision(m_clsFunc.DiagGetString(m_pDiag,intIndex),&strArray,2,FALSE,TRUE);
// 158138 add <--
	tmpSaveData.m_AdName1 = strArray.GetAt( 0 );
	tmpSaveData.m_AdName2 = strArray.GetAt( 1 );
	// 2行目が空行なら1行目のみを再表示
	if ( tmpSaveData.m_AdName2.IsEmpty() ){
		if(m_uInfo.intCurCol == ID_COL_041_ADNAME1 && m_uInfo.intCurRow != m_uInfo.OldRow){	// 各カラムごとにvirRecBufSaveData()がコールされるようになり、かつタイミングがEditOffからEditOnに移ったことによりCmnDiagSetString()でカレットが末尾から先頭に移ってしまう為、制御（修正依頼No150489）
			CmnDiagSetString( m_pDiag, intIndex, tmpSaveData.m_AdName1, 0 );
		}
	}

	// 所在地1(住所)
	intIndex = CmnGetControlIndex( inRow, ID_COL_041_ADADD1 );
	tmpSaveData.m_AdAdd1 = m_clsFunc.DiagGetString( m_pDiag, intIndex );

	// 所在地2(住所)
	intIndex = CmnGetControlIndex( inRow, ID_COL_041_ADADD2 );
	tmpSaveData.m_AdAdd2 = m_clsFunc.DiagGetString( m_pDiag, intIndex );

	// 法人・代表者との関係
	intIndex = CmnGetControlIndex( inRow, ID_COL_041_KANKEI );
	tmpSaveData.m_Kankei = m_clsFunc.DiagGetString( m_pDiag, intIndex );

	// 期末現在高
	intIndex = CmnGetControlIndex( inRow, ID_COL_041_VAL );
	tmpSaveData.m_Val = CmnDiagGetKingaku( m_pDiag, intIndex );

	// 取引の内容
	intIndex = CmnGetControlIndex( inRow, ID_COL_041_NAIYOU );
	tmpSaveData.m_Naiyou = m_clsFunc.DiagGetString( m_pDiag, intIndex );

	///////////////////////////////////////////////////////////////
	// 登録処理呼び出し
	int	intRet = RecBufSaveDataSub( inPage, inRow, &tmpSaveData );

	// 行登録した場合、必ず計再計算と1頁再表示を行う
	if (intRet == FUNCTION_UPDATE) {
		// 計再計算
		CmnTblCalcKei();

		// 1頁再表示
		virUpdateControlTblData();

	}

	return intRet;
}

/**********************************************************************
	virRecBufSaveDataForPasteInsert()
		指定された行のデータをテーブルに保存（挿入貼付作業用）

	引数
		int		inPage		登録するレコードのページ番号
		int		inRow		登録するレコードの行番号
	戻値
		int			成功(更新あり)/成功(更新なし)/失敗
					FUNCTION_UPDATE	: 成功 (更新あり)
					FUNCTION_OK		: 成功 (更新なし)
					FUNCTION_NG		: 失敗

【コメント】
	ローカルの1ページバッファが、「挿入貼付」の場合
	更新がされていない状態なので、貼付該当行を強制的にクリアする
	(1レコードのデータを初期化する)
	データクリア状態で、FgFunc=データ行 なので、
	必ず RecBufSaveDataSub() の登録が実施されるはず。
	「行貼付」の時も、新しいデータに置き換えるので、この対応でOK
	RecBufSaveDataSub() の登録の際に、m_ReadData[] のシーケンス番号など
	重要な列のデータを必要とする場合は、この処理ではNG。
	(現時点2006/02/01では、問題無い)
	問題が発生するようであれば、該当のレコードをテーブルから読みだし、
	m_ReadData[] の該当行にセットするように変更する。
***********************************************************************/
int	CfrmUc041Karibarai::virRecBufSaveDataForPasteInsert( int inPage, int inRow )
{
	RecBufClearData( &m_ReadData[inRow] );			// レコードを初期化
	m_ReadData[inRow].m_FgFunc = ID_FGFUNC_DATA;	// データ行とする

	///////////////////////////////////////////////////////////////
	// 登録処理呼び出し
	return RecBufSaveDataSub( inPage, inRow, &m_CopyData );
}

/**********************************************************************
	virRecBufSaveDataForPaste()
		指定された行のデータをテーブルに保存(上書き貼付作業用)
***********************************************************************/
int	CfrmUc041Karibarai::virRecBufSaveDataForPaste( int inPage, int inRow )
{
	///////////////////////////////////////////////////////////////
	// 登録処理呼び出し
	return RecBufSaveDataSub( inPage, inRow, &m_CopyData );
}

/**********************************************************************
	RecBufSaveDataSub()
		指定された行のデータをテーブルに保存(補助ルーチン)
		※ShowKeiZeroはUpdateCalcKeiで保存している

	引数
			int		inPage		登録するレコードのページ番号

			int		inRow		登録するレコードの行番号
			REC_UC_041_KARIBARAI
					inSaveData	登録するデータレコードへのポインタ
	戻値
			int				成功(更新あり)/成功(更新なし)/失敗
							FUNCTION_UPDATE	: 成功(更新あり)
							FUNCTION_OK	: 成功(更新なし)
							FUNCTION_NG	: 失敗
***********************************************************************/
int	CfrmUc041Karibarai::RecBufSaveDataSub( 
						int		inPage,
						int		inRow,
						P_REC_UC_041_KARIBARAI	inSaveData )
{
	REC_UC_041_KARIBARAI	udTemp;		//	画面上のデータ格納用
	
	//	画面上のデータ格納用初期化
	RecBufClearData( &udTemp );

	// 変更があったかのフラグ（変更あり:TRUE、なし:FALSE)
	BOOL	ufgUpData	= FALSE;		// 変更確認フラグ(全体用データ行/空行)
	BOOL	ufgUpToku	= FALSE;		// 変更確認フラグ(全体用特殊行)
	BOOL	ufgKn		= FALSE;		// 変更確認フラグ(科目)
	BOOL	ufgAd		= FALSE;		// 変更確認フラグ(取引先)
	BOOL	ufgKankei	= FALSE;		// 変更確認フラグ(法人・代表者との関係)
	BOOL	ufgVal		= FALSE;		// 変更確認フラグ(期末現在高)
	BOOL	ufgNaiyou	= FALSE;		// 変更確認フラグ(取引の内容)
	BOOL	ufgFunc		= FALSE;		// 変更確認フラグ(未使用？)

	// レコード更新したことを示すフラグ
	BOOL	ufgRsUpdate	= FALSE;
	// テーブルアクセスクラス
	CdbUc041Karibarai	mfcRec( m_pDB );

	// 作業用変数へ 登録対象のフィールド値を代入
	udTemp = *inSaveData;
// midori 190504 add -->
	// 末尾のスペースをカット
	udTemp.m_AdName1 = m_clsFunc.DeleteRightSpace( udTemp.m_AdName1 );	// (相手先名称1)
	// 末尾のスペースをカット
	udTemp.m_AdName2 = m_clsFunc.DeleteRightSpace( udTemp.m_AdName2 );	// (相手先名称2)
	// 末尾のスペースをカット
	udTemp.m_AdAdd1 = m_clsFunc.DeleteRightSpace( udTemp.m_AdAdd1 );	// (相手先所在地1)
	// 末尾のスペースをカット
	udTemp.m_AdAdd2 = m_clsFunc.DeleteRightSpace( udTemp.m_AdAdd2 );	// (相手先所在地2)
// midori 190504 add <--

	// データ行か、空行かのチェック
	udTemp.m_FgFunc = m_ReadData[inRow].m_FgFunc;

	if ( udTemp.m_FgFunc <= ID_FGFUNC_DATA ) {	// データ行 または、空行なら
		// コンポーネントをチェックして、データがあるかを調べる
		// (仮変数に入れた値でチェックしてもいいのかも？)
		
		if (( udTemp.m_KnOrder == 0 ) &&
			( udTemp.m_AdName1 == "" ) &&
			( udTemp.m_AdName2 == "" ) &&
			( udTemp.m_AdAdd1 == "" ) &&
			( udTemp.m_AdAdd2 == "" ) &&
			( udTemp.m_Kankei == "" ) &&
			( udTemp.m_Val == "" ) &&
			( udTemp.m_Naiyou == "" )){

			// 空行なら
			udTemp.m_FgFunc = ID_FGFUNC_NULL;
		}
		else {
			// データが入っていれば
			udTemp.m_FgFunc = ID_FGFUNC_DATA;
		}
	}

	///////////////////////////////////////////////////////////////
	// 更新状況のチェック

	//	(1) 科目（Order番号管理になっている)
	if (udTemp.m_KnOrder != m_ReadData[inRow].m_KnOrder) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgKn		= TRUE;		// 科目用
	}

	//	(2) 名称１
	if (udTemp.m_AdName1 != m_ReadData[inRow].m_AdName1) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgAd		= TRUE;		// 取引先
	}
		
	//	(3) 名称２
	if (udTemp.m_AdName2 != m_ReadData[inRow].m_AdName2) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgUpToku	= TRUE;		// 全体(特殊行)
		ufgAd		= TRUE;		// 取引先
	}
	
	//	(4) 住所１
	if (udTemp.m_AdAdd1 != m_ReadData[inRow].m_AdAdd1) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgAd		= TRUE;		// 取引先
	}

	//	(5) 住所２
	if (udTemp.m_AdAdd2 != m_ReadData[inRow].m_AdAdd2) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgAd		= TRUE;		// 取引先
	}

	//	(6) 法人・代表者との関係
	if (udTemp.m_Kankei != m_ReadData[inRow].m_Kankei) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgKankei	= TRUE;		// 法人・代表者との関係
	}

	//	(7) 期末現在高
	if (udTemp.m_Val != m_ReadData[inRow].m_Val) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgVal		= TRUE;		// 金額用
	}

	//	(8) 取引の内容
	if (udTemp.m_Naiyou != m_ReadData[inRow].m_Naiyou) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgNaiyou	= TRUE;		// 取引の内容
	}

	//	(8) 特殊行フラグ(基本的には、空行/データ行のチェックになるはず) 
	if (m_ReadData[inRow].m_FgFunc <= ID_FGFUNC_DATA) {
		// 元が空行orデータ行の場合、入力内容により空行orデータ行となる
		// 元が特殊行の場合、FgFuncは更新しない
		if (udTemp.m_FgFunc != m_ReadData[inRow].m_FgFunc) {
			ufgUpData	= TRUE;		// 全体(データ行/空行)
			ufgFunc		= TRUE;		// ufgFunc用
		}
	}

	///////////////////////////////////////////////////////////////////
	// 1 レコード更新処理
	if (( udTemp.m_FgFunc  == ID_FGFUNC_DATA ) || ( udTemp.m_FgFunc == ID_FGFUNC_IKKATUMANUAL )) {	// データ行or一括集計金額行（手動）
		///////////////////////////////////////////////////////////////////
		// データ行or一括集計金額行（手動）の場合の処理

		// 種類について KnOrder に関連する、名称、シーケンスを取得
		if ((ufgKn == TRUE) && (udTemp.m_KnOrder > 0)) {
			// 科目インデックスがNULLなら登録しない
			CdbUcLstKamoku		rsKamo( m_pDB );

			int intRet = rsKamo.RequeryKnOrder(m_uInfo.intFormSeq, udTemp.m_KnOrder);
			if ( intRet != DB_ERR_OK ) {	// 失敗
				return FUNCTION_NG;
			}
			udTemp.m_KnSeq = rsKamo.m_KnSeq;
			udTemp.m_KnName = rsKamo.m_KnName;
// midori 152137 add -->
			udTemp.m_KnKana = rsKamo.m_KnKana;
// midori 152137 add <--

			rsKamo.Fin();
		}

		// 取引先名称リストへの登録チェック
		if ((udTemp.m_FgFunc == ID_FGFUNC_DATA) && (ufgAd == TRUE)) {	// 変更ありの場合
			CdbUcLstAddress		rsAdd( m_pDB );

			if(m_RegAutoRef)	udTemp.m_AdSeq = rsAdd.CheckEntry( udTemp.m_AdName1, udTemp.m_AdName2, udTemp.m_AdAdd1, udTemp.m_AdAdd2, ID_ADDRESSGR_KAI, TRUE);
			else				udTemp.m_AdSeq = rsAdd.GetSeqFromAdName(ID_ADDRESSGR_KAI, udTemp.m_AdName1, udTemp.m_AdName2, udTemp.m_AdAdd1, udTemp.m_AdAdd2 );
			
			if ( udTemp.m_AdSeq < 0 ){
				rsAdd.Fin();
				return( FUNCTION_NG );
			}
			
// midori 152137 del -->
//			if ( udTemp.m_AdSeq == 0 )		udTemp.m_AdOrder = 0;
//			else							udTemp.m_AdOrder = rsAdd.m_OrderNum;
// midori 152137 del <--
// midori 152137 add -->
			if ( udTemp.m_AdSeq == 0 )		{
				udTemp.m_AdOrder = 0;
			}
			else							{
				udTemp.m_AdOrder = rsAdd.m_OrderNum;
				udTemp.m_AdKana = rsAdd.m_AdKana;
			}
// midori 152137 add <--

			rsAdd.Fin();
		}

		// 更新処理
		if (ufgUpData == TRUE) {		// テーブルへ更新(1レコード更新)
			// ページ、行 で抽出し、必要なデータを更新
			mfcRec.RequeryPageRow(inPage, inRow);

			if ( !mfcRec.IsEOF() ) {		// レコードが取得できた(取得できないことはないはず)
				mfcRec.Edit();
				
				//	科目変更あり？
				if (ufgKn == TRUE) {	
					mfcRec.m_KnSeq		= udTemp.m_KnSeq;
					mfcRec.m_KnName		= udTemp.m_KnName;
					mfcRec.m_KnOrder	= udTemp.m_KnOrder;
// midori 152137 add -->
					mfcRec.m_KnKana		= udTemp.m_KnKana;
// midori 152137 add <--
				}
				
				//	取引先変更あり？
				if (ufgAd == TRUE) {
					mfcRec.m_AdSeq		= udTemp.m_AdSeq;
					mfcRec.m_AdOrder	= udTemp.m_AdOrder;
					// 末尾スペース削除後に空行削除処理
					udTemp.m_AdName1 = m_clsFunc.DeleteRightSpace( udTemp.m_AdName1 );
					udTemp.m_AdName2 = m_clsFunc.DeleteRightSpace( udTemp.m_AdName2 );
					m_clsFunc.DeleteNullRow( &udTemp.m_AdName1, &udTemp.m_AdName2 );
					mfcRec.m_AdName1	= udTemp.m_AdName1;
					mfcRec.m_AdName2	= udTemp.m_AdName2;
					mfcRec.m_AdAdd1		= m_clsFunc.DeleteRightSpace( udTemp.m_AdAdd1 );
					mfcRec.m_AdAdd2		= m_clsFunc.DeleteRightSpace( udTemp.m_AdAdd2 );
// midori 152137 add -->
					mfcRec.m_AdKana		= udTemp.m_AdKana;
// midori 152137 add <--
				}

				//	法人・代表者との関係変更あり？
				if (ufgKankei == TRUE) {
					mfcRec.m_Kankei		= m_clsFunc.DeleteRightSpace( udTemp.m_Kankei );
				}

				//	期末現在高変更あり？
				if (ufgVal == TRUE) {
					mfcRec.m_Val		= udTemp.m_Val;
				}
				
				//	摘要変更あり？
				if (ufgNaiyou == TRUE) {
					CStringArray strArray;
					m_clsFunc.StrDivision( udTemp.m_Naiyou, &strArray, 2, TRUE, TRUE );
					mfcRec.m_Naiyou		= m_clsFunc.StrDocking( &strArray );
				}
				
				// 特殊行フラグは、強制的に書き込む
				mfcRec.m_FgFunc		= udTemp.m_FgFunc;

				mfcRec.Update();
				ufgRsUpdate = TRUE;		// レコード更新したので ON
			}
		}
	}
// No.200905 add -->
	else if ( udTemp.m_FgFunc == ID_FGFUNC_IKKATUAUTO && ufgKn == TRUE ) {
// 改良No.21-0086,21-0529 cor -->
		//if(KamokuRowEnableSgn(m_pDB,0) == 1) {
// ------------------------------
		if(KamokuRowEnableSgn(m_pDB, 0, m_uInfo.intFormSeq) == 1)	{
// 改良No.21-0086,21-0529 cor <--
			// 種類について KnOrder に関連する、名称、シーケンスを取得
			if (udTemp.m_KnOrder > 0) {
				// 科目インデックスがNULLなら登録しない
				CdbUcLstKamoku		rsKamo( m_pDB );
				int intRet = rsKamo.RequeryKnOrder(m_uInfo.intFormSeq, udTemp.m_KnOrder);
				if ( intRet != DB_ERR_OK ) {	// 失敗
					return FUNCTION_NG;
				}
				udTemp.m_KnSeq = rsKamo.m_KnSeq;
				udTemp.m_KnName = rsKamo.m_KnName;
				udTemp.m_KnKana = rsKamo.m_KnKana;

				rsKamo.Fin();
			}
			// ページ、行 で抽出し、必要なデータを更新
			mfcRec.RequeryPageRow( inPage, inRow );

			if ( !mfcRec.IsEOF() ) {		// レコードが取得できた(取得できないことはないはず)
				mfcRec.Edit();

				//	科目
				mfcRec.m_KnSeq		= udTemp.m_KnSeq;
				mfcRec.m_KnName		= udTemp.m_KnName;
				mfcRec.m_KnOrder	= udTemp.m_KnOrder;
				mfcRec.m_KnKana		= udTemp.m_KnKana;
				mfcRec.Update();
				ufgRsUpdate = TRUE;		// レコード更新したので ON
			}
		}
	}
// No.200905 add <--
	else if ( udTemp.m_FgFunc == ID_FGFUNC_NULL ) {	// 空行
		///////////////////////////////////////////////////////////////////
		// 空行の場合の処理

		// 更新処理
		if (ufgUpData == TRUE) {		// テーブルへ更新(1レコード更新)
			// ページ、行 で抽出し、必要なデータを更新
			mfcRec.RequeryPageRow(inPage, inRow);

			if ( !mfcRec.IsEOF() ) {		// レコードが取得できた(取得できないことはないはず)
				mfcRec.Edit();
				mfcRec.m_FgFunc		= udTemp.m_FgFunc;
				mfcRec.m_KeiStr		= _T("");
				mfcRec.m_NumGroup	= 0;
				mfcRec.m_KnSeq		= 0;
				mfcRec.m_KnOrder	= 0;
				mfcRec.m_KnName		= _T("");
				mfcRec.m_AdSeq		= 0;
				mfcRec.m_AdOrder	= 0;
				mfcRec.m_AdName1	= _T("");
				mfcRec.m_AdName2	= _T("");
				mfcRec.m_AdAdd1		= _T("");
				mfcRec.m_AdAdd2		= _T("");
				mfcRec.m_Kankei		= _T("");
				mfcRec.m_Val		= _T("");
				mfcRec.m_Naiyou		= _T("");
// midori 152137 add -->
				mfcRec.m_AdKana		= _T("");	// 振出人
				mfcRec.m_KnKana		= _T("");	// 科目
// midori 152137 add <--
				mfcRec.Update();

				ufgRsUpdate = TRUE;		// レコード更新したので ON
			}
		}
	}
	else  {		// データ行、空行 ではない（特殊行)
		///////////////////////////////////////////////////////////////////
		// 特殊行の場合の処理

		// 更新処理
		if (ufgUpToku == TRUE) {		// テーブルへ更新(1レコード更新)
			// ページ、行 で抽出し、必要なデータを更新
			mfcRec.RequeryPageRow(inPage, inRow);

			if ( !mfcRec.IsEOF() ) {		// レコードが取得できた(取得できないことはないはず)
				mfcRec.Edit();
				mfcRec.m_AdName2 = udTemp.m_AdAdd2; 
				mfcRec.Update();

				ufgRsUpdate = TRUE;		// レコード更新したので ON
			}
		}
	}


	///////////////////////////////////////////////////////////////////
	// 内部 1ページバッファも更新する
	int retFunc = FUNCTION_OK;				//戻り値：成功（変更なし)
	if (ufgRsUpdate == TRUE) {
		retFunc = FUNCTION_UPDATE;		//戻り値：成功（変更あり)
		if ( !mfcRec.IsEOF() ) {		// レコードが取得できた(取得できないことはないはず)
			RecBufSetData( &mfcRec );
		}
	}

	mfcRec.Fin();

	return retFunc;
}

//********************************************************************************
//	「仮払金」テーブルのオブジェクトを生成し、ポインタを返す
//********************************************************************************
CdbUc000Common*	CfrmUc041Karibarai::virGetDataTableObject()
{
	// 「仮払金」テーブルのオブジェクトを生成し、ポインタを返す
	return (CdbUc000Common*)( new CdbUc041Karibarai( m_pDB ) );
}

//********************************************************************************
//	行コピー処理
//	IN	なし
//	RET	なし
//********************************************************************************
void	CfrmUc041Karibarai::virRowButtonCopyProc()
{
	// カレント行のデータを、コーピー用バッファに格納
	m_CopyData = m_ReadData[m_uInfo.intCurRow];

	// コピーフラグON
	m_nCopyFlag = 1;
}

//********************************************************************************
//	テーブルから科目情報を取得
//	IN	Cdb000Common* rsData	テーブル情報
//		SORTKAMOKU_INFO*		科目情報
//	RET	なし
//********************************************************************************
void	CfrmUc041Karibarai::virTblGetKamokuInfo( CdbUc000Common* rsData, SORTKAMOKU_INFO* uKamoku )
{
	CdbUc041Karibarai*	rs = (CdbUc041Karibarai*)rsData;

	// 本当ならGetFieldValue()で取得する予定だったが、時間も無いのであきらめた
	uKamoku->intKnSeq = rs->m_KnSeq;
	uKamoku->intKnOrder = rs->m_KnOrder;
	uKamoku->strKnName = rs->m_KnName;
// midori 152137 add -->
	uKamoku->strKnKana = rs->m_KnKana;
// midori 152137 add <--
}

//********************************************************************************
//	共通処理から帳表のカレント情報を取得
//	IN	CURRENT_INFO	取得する情報の格納領域
//	RET	なし
//********************************************************************************
void	CfrmUc041Karibarai::virGetCurrentInfo( CURRENT_INFO* uCurInfo )
{
	uCurInfo->intFgFunc = m_ReadData[m_uInfo.intCurRow].m_FgFunc;
	uCurInfo->intNumGroup = m_ReadData[m_uInfo.intCurRow].m_NumGroup;
	uCurInfo->intCopyFlag = m_nCopyFlag;
}

//********************************************************************************
//	通常/個別切り替え時、関連帳表データ頁構成の更新処理
//		IN		なし
//		RET		BOOL		TRUE:正常終了，FALSE:エラー
//********************************************************************************
int	CfrmUc041Karibarai::virUpdateRelationTblChengeFormType()
{
// midori 190301 del -->
//	// ④-1で通常/個別を変更した場合、④-2の頁構成の更新が必要となる
//	CfrmUc042Kasituke	pfrm042;
//// midori 152770 del -->
////	int	intRet = pfrm042.UpdateTbl042ChangeFormType();
//// midori 152770 del <--
//// midori 152770 add -->
//	int	intRet = pfrm042.UpdateTbl042ChangeFormType(m_uInfoSv);
//// midori 152770 add <--
// midori 190301 del <--
// midori 190301 add -->
	int		intRet=0;

	// ④-1で通常/個別を変更した場合、④-2の頁構成の更新が必要となる
	if(bG_Kanso == TRUE)	{
		CfrmUc042KasitukeK	pfrm042;

		intRet = pfrm042.UpdateTbl042ChangeFormType(m_uInfoSv);
	}
	else	{
		CfrmUc042Kasituke	pfrm042;

		intRet = pfrm042.UpdateTbl042ChangeFormType(m_uInfoSv);
	}
// midori 190301 add <--

	return intRet;
}

// midori 152770 add -->
//********************************************************************************
//	通常/個別切り替え時、関連帳表データ頁構成の更新処理
//		IN		なし
//		RET		BOOL		TRUE:正常終了，FALSE:エラー
//********************************************************************************
BOOL CfrmUc041Karibarai::virUpdateRelationTblChengeFormTypeSave()
{
	// ④-1で通常/個別を変更した場合、④-2のm_uInfoを保存する
// midori 190301 del -->
	//CfrmUc042Kasituke	pfrm042;
	//BOOL				ret=FALSE;

	//ret = pfrm042.CmnSetInitialUchiwakeInfo(ID_FORMNO_042);

	//m_uInfoSv = pfrm042.m_uInfo;
// midori 190301 del <--
// midori 190301 add -->
	BOOL				ret=FALSE;

	if(bG_Kanso == TRUE)	{
		CfrmUc042KasitukeK	pfrm042;

		ret = pfrm042.CmnSetInitialUchiwakeInfo(ID_FORMNO_042);

		m_uInfoSv = pfrm042.m_uInfo;
	}
	else	{
		CfrmUc042Kasituke	pfrm042;

		ret = pfrm042.CmnSetInitialUchiwakeInfo(ID_FORMNO_042);

		m_uInfoSv = pfrm042.m_uInfo;
	}
// midori 190301 add <--

	return(ret);
}
// midori 152770 add <--

// midori 152770 del -->
////********************************************************************************
////	通常/個別切り替え時の頁構成を更新（④-2から実行される処理）
////		IN		なし
////		RET		なし
////********************************************************************************
//int	CfrmUc041Karibarai::UpdateTbl041ChangeFormType()
//{
//
//	// 初期値設定
//	CmnSetInitialUchiwakeInfo(ID_FORMNO_041);	// 共通データ
//	virSetInitialValue();						// 帳表固有データ
//
//	// データ無しならデータ更新しない
//	if (m_uInfo.intMaxPage <= 0) {
//		return FUNCTION_OK;
//	}
//
//	// 頁と行の再設定
//	CmnTblUpdatePageRowAllResetMain();
//
//	return FUNCTION_OK;
//}
// midori 152770 del <--
// midori 152770 add -->
//********************************************************************************
//	通常/個別切り替え時の頁構成を更新（④-2から実行される処理）
//		IN		UCHIWAKE_INFO		出力形式変更前のUCHIWAKE_INFO構造体情報
//		RET		なし
//********************************************************************************
int	CfrmUc041Karibarai::UpdateTbl041ChangeFormType(UCHIWAKE_INFO pInfoSv)
{
	// データ無しならデータ更新しない
	if (pInfoSv.intMaxPage <= 0) {
		return FUNCTION_OK;
	}

	// Save用の構造体から、m_infoにデータを読み込み、空行を作成
	m_uInfo = pInfoSv;
	CmnTblUpdatePageRowAllResetMain(1);

	// 初期値設定
	CmnSetInitialUchiwakeInfo(ID_FORMNO_041);	// 共通データ
	virSetInitialValue();						// 帳表固有データ

	// 頁と行の再設定（先頭の空行を削除する）
	CmnTblUpdatePageRowAllResetMain(2);

	return FUNCTION_OK;
}
// midori 152770 add <--

//**************************************************
//	入力不可モードの設定/解除
//	【引数】	nRow	…	行
//				nMode	…	入力不可モードフラグ
//								DIAG_MDFY_NOINPUT	…	エディットアイテムで全てのWM_CHARを無効とする
//								DIAG_MDFY_INPUT		…	DIAG_MDFY_NOINPUTモード解除
//	【戻値】	なし
//**************************************************
void CfrmUc041Karibarai::SetInputMode( int nRow, long nMode )
{
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_041_ADSEQ ), nMode );		//	科目（※科目がエディットボックスに変更された際に必要）
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_041_ADNAME1 ), nMode );	//	名称（上段）
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_041_ADADD1 ), nMode );	//	所在地（上段）
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_041_ADADD2 ), nMode );	//	所在地（下段）
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_041_KANKEI ), nMode );	//	法人・代表者との関係
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_041_VAL ), nMode );		//	期末現在高
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_041_NAIYOU ), nMode );	//	取引の内容
}

//**************************************************
//	各Diagの背景色(BackColor)に色を設定
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CfrmUc041Karibarai::SetDiagBackColor()
{
	// 入力画面の各Diagの背景色を共通設定色に合わせる
	m_PageDiag.SetBackColor( m_ViewColor );
	m_TitleDiag.SetBackColor( m_ViewColor );
	m_NumberDiag.SetBackColor( m_ViewColor );
	m_HeadDiag.SetBackColor( m_ViewColor );
	m_DiagNormal.SetBackColor( m_ViewColor );
	m_DiagKobetu.SetBackColor( m_ViewColor );
	m_DiagKobetu2.SetBackColor( m_ViewColor );
	m_DiagKobetu3.SetBackColor( m_ViewColor );
// midori 161108 add -->
	m_SaveDataDiag.SetBackColor( m_ViewColor );
// midori 161108 add <--
// 修正No.157875 add -->
	m_DiagKei1.SetBackColor( m_ViewColor );
	m_DiagKei2.SetBackColor( m_ViewColor );
// 修正No.157875 add <--
}

//********************************************************************************
//	財務連動：連動データの作成（新規作成）
//		IN		CdbUc000Common*		内訳書テーブル情報
//				RENTEMP_INFO*		財務連動情報
//		RET		なし					
//********************************************************************************
void CfrmUc041Karibarai::virRendoCreateData(CdbUc000Common* rsData, RENTEMP_INFO* uRenInfo)
{
	CString			strTemp;
	CStringArray	strArry;
	int				intSeq;
// midori 152153 add -->
	int				sw = 0;
	CdbUcLstAddress	rsLstAdd(m_pDB);	// rs_lst_Address
// midori 152153 add <--

	CdbUc041Karibarai*	rs = (CdbUc041Karibarai*)rsData;

	// 頁番号/行番号の加算
	CmnGetNextPageRow(&uRenInfo->nPage, &uRenInfo->nRow);

	// 新規レコード作成
	rs->CreateNewRecord();					// 新規レコード追加（初期化済み）

	rs->Edit();
	rs->m_NumPage	= uRenInfo->nPage;		// 頁番号
	rs->m_NumRow	= uRenInfo->nRow;		// 行番号
	rs->m_FgFunc	= ID_FGFUNC_DATA;		// 特殊行フラグ
	rs->m_FgShow	= ID_FGSHOW_OFF;		// 表示フラグ

	switch (uRenInfo->intOpRenType) {
	case ID_RENDATA_KZ:
	case ID_RENDATA_EZ:
		rs->m_RenKcd	= uRenInfo->strRenKcd;		// 財務連動元（科目）
		rs->m_RenEcd	= uRenInfo->lngRenEcd;		// 財務連動元（枝番）
		rs->m_RenFgTemp	= ID_RENFGTEMP_UPDATE;		// 財務連動テンポラリ情報
		break;
	}

	// 科目情報：uc_lst_kamokuより詳細を取得
	CdbUcLstKamoku		rsLstKmk(m_pDB);								// rs_lst_kamoku
	if (rsLstKmk.RequeryKnSeq(m_uInfo.intFormSeq, uRenInfo->lngKnSeq) == DB_ERR_OK) {
		if (rsLstKmk.m_KnSeq > 0) {
			rs->m_KnSeq		= rsLstKmk.m_KnSeq;
			rs->m_KnOrder	= rsLstKmk.m_KnOrder;
			rs->m_KnName	= rsLstKmk.m_KnName;
// midori 152137 add -->
			rs->m_KnKana	= rsLstKmk.m_KnKana;
// midori 152137 add <--
		}
	}
	rsLstKmk.Fin();

	// 名称(氏名)
	// 取引先情報：取引先登録チェック
// midori 152153 del -->
	//CdbUcLstAddress		rsLstAdd(m_pDB);								// rs_lst_Address
	//m_clsFunc.StrDivisionEx( uRenInfo->strAdName , &strArry , ( MAX_KETA_041_ADNAME1 / 2 ) , 2 );
	//intSeq = rsLstAdd.CheckEntry( strArry.GetAt(0), strArry.GetAt(1) , _T("") , _T("") , ID_ADDRESSGR_KAI, TRUE);
	//if (intSeq > 0) {
	//	rs->m_AdSeq		= rsLstAdd.m_AdSeq;
	//	rs->m_AdOrder	= rsLstAdd.m_OrderNum;
	//	rs->m_AdName1	= rsLstAdd.m_AdName1;
	//	rs->m_AdName2	= rsLstAdd.m_AdName2;
	//}
	//rsLstAdd.Fin();
// midori 152153 del <--
// midori 152153 add -->
	sw = 0;
	// 仕訳データ
	// 「科目残高かつ科目名称を取引先に転記するにチェックあり」
	// または「枝番残高かつ枝番未登録・未入力のデータかつ科目名称を取引先に転記するにチェックあり」
	if((uRenInfo->intOpRenType == ID_RENDATA_KZ && uRenInfo->byteKmkName == TRUE) ||
		(uRenInfo->intOpRenType == ID_RENDATA_EZ && uRenInfo->byteEdasgn == FALSE && uRenInfo->byteKmkName == TRUE))	{
		// 取引先、金融機関を自動で登録するにチェックあり
		if(m_RegAutoRef != FALSE)	{
			// 取引先、金融機関の[F9 参照]に登録する
			sw = 1;
		}
		else	{
			// 取引先、金融機関の[F9 参照]に登録する
			if(uRenInfo->byteF9Upd == TRUE)		sw = 1;
			// 取引先、金融機関の[F9 参照]に登録しない
			else								sw = 2;
		}
	}

	// 枝番残高かつ枝番取込の場合
	if(uRenInfo->intOpRenType == ID_RENDATA_EZ && uRenInfo->byteEdasgn == TRUE)	{
		// 取引先、金融機関の[F9 参照]に登録する
		sw = 1;
	}

	if(sw != 0)	{
		m_clsFunc.StrDivisionEx( uRenInfo->strAdName , &strArry , ( MAX_KETA_041_ADNAME1 / 2 ) , 2 );
		if(sw == 1)	{
// midori 152137 del -->
//			intSeq = rsLstAdd.CheckEntry( strArry.GetAt(0), strArry.GetAt(1) , _T("") , _T("") , ID_ADDRESSGR_KAI, TRUE);
// midori 152137 del <--
// midori 152137 add -->
			intSeq = rsLstAdd.CheckEntryRen( strArry.GetAt(0), strArry.GetAt(1) , _T("") , _T(""), uRenInfo->strAdKana, ID_ADDRESSGR_KAI, TRUE);
// midori 152137 add <--
			if (intSeq > 0) {
				rs->m_AdSeq		= rsLstAdd.m_AdSeq;
				rs->m_AdOrder	= rsLstAdd.m_OrderNum;
				rs->m_AdName1	= rsLstAdd.m_AdName1;
				rs->m_AdName2	= rsLstAdd.m_AdName2;
// midori 152137 add -->
				rs->m_AdKana	= rsLstAdd.m_AdKana;
// midori 152137 add <--
			}
			rsLstAdd.Fin();
		}
		else if(sw == 2)	{
			if(strArry.GetAt(0).IsEmpty() == FALSE || strArry.GetAt(1).IsEmpty() == FALSE)	{
				rs->m_AdSeq		= 0;
				rs->m_AdOrder	= 0;
				rs->m_AdName1	= strArry.GetAt(0);
				rs->m_AdName2	= strArry.GetAt(1);
// midori 152137 add -->
				rs->m_AdKana	= _T("");
// midori 152137 add <--
			}
		}
	}
// midori 152153 add <--

	// 金額
	rs->m_Val = uRenInfo->strVal;		

	rsData->Update();
}

//********************************************************************************
//	財務連動：連動データの作成（金額のみ更新）
//		IN		CdbUc000Common*		内訳書テーブル情報（更新対象データ）
//				RENTEMP_INFO*		財務連動情報
//		RET		なし					
//********************************************************************************
void CfrmUc041Karibarai::virRendoUpdateData(CdbUc000Common* rsData, RENTEMP_INFO* uRenInfo)
{
	CdbUc041Karibarai*	rs = (CdbUc041Karibarai*)rsData;

	rs->Edit();

	rs->m_RenFgTemp	= ID_RENFGTEMP_UPDATE;		// 財務連動テンポラリ情報
	rs->m_Val		= uRenInfo->strVal;			// 金額

	rs->Update();
}


//***************************************************************************
//（2009.09.08 TS自動ログオフ対応）
//	Viewの終了処理
//		・デストラクタで行っていた処理をここに移動
//
//	IN		なし
//	RET		BOOL				TRUE=エラー，FALSE=正常
//***************************************************************************
BOOL CfrmUc041Karibarai::virEndProc(void)
{
	TRACE(_T("***** CfrmUc041Karibarai::virEndProc\n"));

	// ↓この処理でコントロールが破棄されているが、当関数が実行される時点では、
	// 　そのコントロールを持つView自体は生きている。
	// 　このため、コントロールに対する処理（セル位置の取得）でエラーになってしまう。
	// 　もともとはデストラクタにあったため、コントロールを破棄した後、
	// 　すぐにView自体も殺していたので問題なかった。
	// 　処理自体も特に意味がないので、コメントアウトする。

	//m_pDiag = NULL;
	//m_pTitleDiag = NULL;

	// 親Viewの終了処理
	return CfrmUc000Common::virEndProc();
}

// midori 161106 add -->
void CfrmUc041Karibarai::ComboNew(short index)
{
	short			sv_sel=0;
// midori 152201 add -->
	short			sv_sel2=0;		// 変更前のコンボボックスの値を取得
// midori 152201 add <--
	int				sv_cnt=0;
	int				sv_index=0;
	int				intCol=0;
	DIAGRAM_DATA	diadata;

	m_F9OnSw = FALSE;
	// ポジション取得
	m_uInfo.intCurCtlIndex = m_pDiag->GetPosition();
	sv_index = m_pDiag->GetPosition();
	// 行・列取得
	CmnGetControlPosition( &m_uInfo.intCurRow , &m_uInfo.intCurCol, &m_uInfo.OldCol );
	// 「種類」列ならコンボボックスのオフセット値取得
	if ( m_uInfo.intCurCol == ID_COL_041_ADSEQ ){
		m_pDiag->GetData( m_uInfo.intCurCtlIndex , (LPUNKNOWN)&diadata );
		if(diadata.data_combo > m_CmbCnt)	{
			// 実際に登録された科目の件数と選択されたIndexが異なる場合は、「新規作成」が選択されたとする
			m_F9OnSw = TRUE;
			sv_sel = diadata.data_combo;
			sv_cnt = m_CmbCnt;
// midori 152201 add -->
			sv_sel2 = GetKnOrder(m_uInfo.intFormSeq,m_uInfo.intCurPage,m_uInfo.intCurRow);
// midori 152201 add <--

			// F9参照
			OnButtonF9();

			m_F9OnSw = FALSE;
			// 選択されたものを再取得
			m_pDiag->GetData( m_uInfo.intCurCtlIndex , (LPUNKNOWN)&diadata );
			// 何も変わってなかったらキャンセル
			if(sv_sel == diadata.data_combo && sv_cnt == m_CmbCnt)	{
// midori 152201 del -->
				//diadata.data_combo = 0;
				//m_uInfo.OldCombo = 0;
// midori 152201 del <--
// midori 152201 add -->
				// sv_sel2が新規追加の場合、クリアする
				if(sv_sel2 > m_CmbCnt)	sv_sel2 = 0;

				diadata.data_combo = sv_sel2;
				m_uInfo.OldCombo = sv_sel2;
// midori 152201 add <--
				m_pDiag->SetData( m_uInfo.intCurCtlIndex , (LPUNKNOWN)&diadata );
			}
// midori 152134 del -->
//			m_pDiag->SetPosition( sv_index );
// midori 152134 del <--
// midori 152134 add -->
			PostMessage(WM_USER_FOCUSSET,0,0);
// midori 152134 add <--
		}
	}
}
// midori 161106 add <--

// midori 151405 add -->
int CfrmUc041Karibarai::CursorControl(short nChar, int sw)
{
	int				rv=0;
	int				intCol=0;
	int				pos=0;
// midori 152134 17/10/07 add -->
	DIAGRAM_DATA	diadata;
// midori 152134 17/10/07 add <--

	// 現在の列番号を取得
	intCol = CmnGetControlCol();

// 修正No.158296 del -->
//// midori 152134 17/10/07 add -->
//	// 例外処理：移動前に【新規作成】を選び、Tabキーを押下された場合は値を元に戻す
//	if(nChar == VK_TAB)	{
//		if (intCol == ID_COL_041_ADSEQ) {
//			m_pDiag->GetData( m_uInfo.intCurCtlIndex , (LPUNKNOWN)&diadata );
//			if(diadata.data_combo > m_CmbCnt)	{
//				m_pDiag->ComboDropDown(m_uInfo.intCurCtlIndex, FALSE);
//				CmnDiagSetCombo(m_pDiag, m_uInfo.intCurCtlIndex, m_uInfo.OldCombo);
//			}
//		}
//	}
//// midori 152134 17/10/07 add <--
// 修正No.158296 del <--
// 修正No.158296 add -->
	// 例外処理：移動前に【新規作成】を選び、Tabキー、左矢印キー、右矢印キーを押下された場合は値を元に戻す
	if(nChar == VK_TAB || nChar == VK_LEFT || nChar == VK_RIGHT)	{
		if (intCol == ID_COL_041_ADSEQ) {
			int		sw = 0;
			m_pDiag->GetData( m_uInfo.intCurCtlIndex , (LPUNKNOWN)&diadata );
			if(diadata.data_combo > m_CmbCnt)	{
				sw = 1;
			}
			else if(diadata.data_combo != m_uInfo.OldCombo)	{
				if(nChar == VK_LEFT || nChar == VK_RIGHT)	{
					sw = 1;
				}
			}
			if(sw == 1)	{
				m_pDiag->ComboDropDown(m_uInfo.intCurCtlIndex, FALSE);
				CmnDiagSetCombo(m_pDiag, m_uInfo.intCurCtlIndex, m_uInfo.OldCombo);
			}
		}
	}
// 修正No.158296 add <--

// 修正No.158296 del -->
	//// 例外処理：移動前にComnoBoxで←/→キーが押下された場合は値を元に戻す
	//if ((nChar == VK_LEFT) || (nChar == VK_RIGHT)) {
	//	// ComboBoxは選択内容を確定させない
	//	if (intCol == ID_COL_041_ADSEQ) {
	//		m_pDiag->ComboDropDown(m_uInfo.intCurCtlIndex, FALSE);
	//		CmnDiagSetCombo(m_pDiag, m_uInfo.intCurCtlIndex, m_uInfo.OldCombo);
	//	}
	//}
// 修正No.158296 del <--

	// フォーカス移動先を取得
	int intRet = GetFocusPosition(nChar);
	switch (intRet) {
	case -1:		// 先頭項目でShift+Tabキーのため、タイトルへ移動
	case -4:		// 先頭行で↑or先頭項目で←は、タイトルへ移動
		m_TitleDiag.SetPosition(0);
		rv = 1;
		break;
	case -2:		// 最終項目でTabキーのため、頁コントロールへ移動
		m_PageDiag.SetPosition(1);
		rv = 1;
		break;
	case -3:		// スクロールで無く、ComboBoxで↑↓キーを押された場合何もしない
		if(nG_MScroll == 1)	{
			pos = m_pDiag->GetPosition();
			m_pDiag->SetPosition(pos);
			rv = 1;
		}
		break;
	default:		// フォーカス移動を行う
		m_pDiag->SetPosition(intRet);
		rv = 1;
		break;
	}

	// ポジション取得
	m_uInfo.intCurCtlIndex = m_pDiag->GetPosition();
	if(sw == 1)	nG_MScroll = 0;

	return(rv);
}
// midori 151405 add <--

// midori 152134 add -->
void CfrmUc041Karibarai::ComboSelYoushikidiag1(short index, LPUNKNOWN data)
{
// 要通知案件修正 21/07/09 del -->
	//char	key=0x00;		// →矢印キー状態取得
	//char	key2=0x00;		// Enterキー状態取得

	//key = ( char )GetKeyState( VK_RIGHT );				//	→矢印キー状態取得
	//key2 = ( char )GetKeyState( VK_RETURN );			//	→Enterキー状態取得

	//m_ComboSgn = 0;
	//if((key & 0x80) != 0x80 && (key2 & 0x80) != 0x80)	{
	//	if(m_uInfo.intCurCol == ID_COL_041_ADSEQ)	{
	//		ComboNew(index);
	//		m_ComboSgn = 1;
	//	}
	//}
// 要通知案件修正 21/07/09 del <--
// 要通知案件修正 21/07/09 add -->
	char	key1 = 0x00;		// Enterキー状態取得
	char	key2 = 0x00;		// マウス左クリック状態取得
// 修正No.158262 add -->
	char	key3 = 0x00;		// 右矢印キー状態取得
// 修正No.158262 add <--

	key1 = (char)GetKeyState( VK_RETURN );			// Enterキー状態取得
	key2 = (char)GetAsyncKeyState(VK_LBUTTON);		// マウス左クリック状態取得
// 修正No.158262 add -->
	key3 = (char)GetKeyState( VK_RIGHT );			// 右矢印キー状態取得
// 修正No.158262 add <--

	m_ComboSgn = 0;
// 修正No.158262 add -->
	// 右矢印キーで来た時は抜ける
	if((key3 & 0x80) == 0x80)	{
		return;
	}
// 修正No.158262 add <--
	// マウスクリックされた時のみ
	// Enter押下の処理はEditOFFでしているのでここでは入れないようにする
	if( ((key1 & 0x80) != 0x80) && ((key2 & 0xff) == 0x01) )	{
		if(m_uInfo.intCurCol == ID_COL_041_ADSEQ) {
			ComboNew(index);
			m_ComboSgn = 1;
		}
	}
// 要通知案件修正 21/07/09 add <--
}
// midori 152134 add <--

// No.200601 add -->
// ------------------------------------------------------------------------------------------------------------------------
// 全頁の電子申告の桁数チェックを行い、タイトルの文字色を変更する
// ------------------------------------------------------------------------------------------------------------------------
void CfrmUc041Karibarai::TitleColorChg(void)
{
	int					nCharOver[2]={0};	// 電子申告の文字数を超えたかどうかのサイン		1:電子申告の文字数を超えた
											// 様式④		[0]:科目(10文字)	[1]:相手先 法人・代表者との関係(10)
	int					nFontSize=0;		// 「法人・代表者との関係」のフォントサイズ
	CRecordset			rs(m_pDB);
	CString				strSQL=_T("");
	CString				strData=_T("");

	// 科目
// 修正No.157628 del -->
	//strSQL.Format(_T("select max(oversgn) from (select CASE when len(Replace(isnull(KnName,''),CHAR(13)+CHAR(10),'')) > %d then 1 else 0 end as oversgn from uc_041_karibarai where FgShow=%d) tbl1"),UC_KAMOKU_LEN,ID_FGSHOW_OFF);
// 修正No.157628 del <--
// 修正No.157628 add -->
	strSQL.Format(_T("select max(oversgn) from (select CASE when len(%s) > %d then 1 else 0 end as oversgn from uc_041_karibarai where FgShow=%d) tbl1"),
		MakeReplaceSent(_T("KnName")),UC_KAMOKU_LEN,ID_FGSHOW_OFF);
// 修正No.157628 add <--
	rs.Open( CRecordset::forwardOnly, strSQL);
	rs.GetFieldValue((short)0, strData);
	rs.Close();
	nCharOver[0] = 0;
	if(_tstoi(strData) != 0) {
		nCharOver[0] = 1;
	}
	// 法人・代表者との関係
// 修正No.157628 del -->
//	strSQL.Format(_T("select max(oversgn) from (select CASE when len(Replace(isnull(Kankei,''),CHAR(13)+CHAR(10),'')) > %d then 1 else 0 end as oversgn from uc_041_karibarai where FgShow=%d) tbl1"),UC_041_KANKEI_LEN,ID_FGSHOW_OFF);
// 修正No.157628 del <--
// 修正No.157628 add -->
	strSQL.Format(_T("select max(oversgn) from (select CASE when len(%s) > %d then 1 else 0 end as oversgn from uc_041_karibarai where FgShow=%d) tbl1"),
		MakeReplaceSent(_T("Kankei")),UC_041_KANKEI_LEN,ID_FGSHOW_OFF);
// 修正No.157628 add <--
	rs.Open( CRecordset::forwardOnly, strSQL);
	rs.GetFieldValue((short)0, strData);
	rs.Close();
	nCharOver[1] = 0;
	if(_tstoi(strData) != 0) {
		nCharOver[1] = 1;
	}

	// 項目タイトル(ヘッダ部)の再設定
// 修正No.157626 del -->
	//CmnHeadDiagSetTitle(&m_HeadDiag,0,_T("科  目(10)"),nCharOver[0]);
	////CmnHeadDiagSetTitle(&m_HeadDiag,4,_T("法人・代表者\r\nとの関係(10)"),nCharOver[1]);
	//CmnHeadDiagSetTitle(&m_HeadDiag,4,_T("法人・代表　　\r\n者との関係(10)"),nCharOver[1]);
// 修正No.157626 del <--
// 修正No.157626 add -->
	if(nCharOver[0] == 1)	CmnHeadDiagSetTitle(&m_HeadDiag,0,_T("科  目(10)"),1);
	else					CmnHeadDiagSetTitle(&m_HeadDiag,0,_T("科　目"),0);

	if(nCharOver[1] == 1)	CmnHeadDiagSetTitle(&m_HeadDiag,4,_T("法人・代表　　\r\n者との関係(10)"),1);
	else					CmnHeadDiagSetTitle(&m_HeadDiag,4,_T("法人・代表\r\n者との関係"),0);

	// 欄外ガイドの表示
	if(nCharOver[0] == 1 || nCharOver[1] == 1)	{
		m_Guide1.ShowWindow(TRUE);
	}
	else {
		m_Guide1.ShowWindow(FALSE);
	}
// 修正No.157626 add <--

	// 「法人・代表者との関係」のみフォントサイズを変更
	if(bG_Kanso == TRUE)	nFontSize = 100;
	else					nFontSize = 110;
	try	{
		m_HeadDiag.ChangeFontSize(4, nFontSize);
	}
	catch(CException* e){
		e->Delete();
	}
}
// No.200601 add <--

// 修正No.162446 add -->
LRESULT CfrmUc041Karibarai::OnUserReDrawView(WPARAM wParam, LPARAM lParam)
{
	virUpdateControlTblData();
	CmnDiagSetFocus(m_pDiag, 1, 1);

	return(0);
}
// 修正No.162446 add <--
