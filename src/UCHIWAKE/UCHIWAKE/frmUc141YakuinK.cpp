//--------------------------------------------------
//	frmUc141Yakuin.cpp
//--------------------------------------------------

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "frmUc141Yakuin.h"
#include "frmUc142Jinkenhi.h"		// 通常/個別切り替えにて"⑭-2"データ更新が必要

// midori 190301 add -->
extern BOOL	bG_Kanso;
// midori 190301 add <--

IMPLEMENT_DYNCREATE( CfrmUc141YakuinK, CfrmUc000Common )


BEGIN_MESSAGE_MAP( CfrmUc141YakuinK, CfrmUc000Common )
	ON_WM_SIZE()
//	ON_COMMAND( ID_BUTTON_F1, OnButtonF1 )
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
	ON_COMMAND( ID_PRINTSETUP_MENU, &CfrmUc141YakuinK::OnMenuPrintSetup )
	ON_COMMAND( ID_PRINTALL_MENU, &CfrmUc141YakuinK::OnMenuPrintAll )
	ON_COMMAND( ID_OUTPUTFORM_MENU, &CfrmUc141YakuinK::OnMenuFormTypeSetup )
	ON_COMMAND( ID_SPECIALROW_MENU, &CfrmUc141YakuinK::OnMenuSpecialRow )
//	ON_COMMAND( ID_DISPDIALOG_MENU, &CfrmUc141YakuinK::OnMenuIkkatuKingaku )
	ON_COMMAND( ID_ZEROMONEY_MENU, &CfrmUc141YakuinK::OnMenuZeroMoney )
	ON_COMMAND( ID_ZEROMONEYMOVE_MENU, &CfrmUc141YakuinK::OnMenuZeroMoneyMove )
	ON_COMMAND( ID_AMOUTDIALOG_MENU, &CfrmUc141YakuinK::OnMenuOutKeiSetup )
// midori 160606 add -->
	ON_COMMAND( ID_CLEAR_MENU, &CfrmUc141YakuinK::OnMenuClear )
// midori 160606 add <--
	ON_COMMAND( ID_TITLEMENU, &CfrmUc141YakuinK::OnMenuTitleRestore )
	ON_COMMAND( ID_USER_SETTING_MENU, &CfrmUc141YakuinK::OnMenuOptionSetting )
	ON_COMMAND( ID_COPY_SETTING_INFO_MENU, &CfrmUc141YakuinK::OnMenuCopySettingInfo )
	ON_COMMAND( ID_DELETE_DATA_MENU, &CfrmUc141YakuinK::OnMenuDeleteDataEachYoushiki )
	ON_BN_CLICKED( IDC_PAGEBACK_BUTTON1, &CfrmUc141YakuinK::OnPageBack )
	ON_BN_CLICKED( IDC_PAGENEXT_BUTTON1, &CfrmUc141YakuinK::OnPageNext )
	ON_BN_CLICKED( IDC_PAGENEW_BUTTON1, &CfrmUc141YakuinK::OnPageNew )
	ON_BN_CLICKED( IDC_PAGEINSERT_BUTTON1, &CfrmUc141YakuinK::OnPageInsert )
	ON_BN_CLICKED( IDC_ROWCOPY_BUTTON1, &CfrmUc141YakuinK::OnRowCopy )
	ON_BN_CLICKED( IDC_ROWPASTE_BUTTON1, &CfrmUc141YakuinK::OnRowPaste )
	ON_BN_CLICKED( IDC_ROWINSERT_BUTTON1, &CfrmUc141YakuinK::OnRowInsert )
	ON_BN_CLICKED( IDC_YOUSHIKINEXT_BUTTON1, &CfrmUc141YakuinK::OnMoveYoushikiNext )
	ON_BN_CLICKED( IDC_YOUSHIKIBACK_BUTTON1, &CfrmUc141YakuinK::OnMoveYoushikiBack )
//	ON_UPDATE_COMMAND_UI( ID_BUTTON_F1, &CfrmUc141YakuinK::OnUpdateButtonF1 )
//	ON_UPDATE_COMMAND_UI( ID_BUTTON_F3, &CfrmUc141YakuinK::OnUpdateButtonF3 )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_F6, &CfrmUc141YakuinK::OnUpdateButtonF6 )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_F7, &CfrmUc141YakuinK::OnUpdateButtonF7 )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_F8, &CfrmUc141YakuinK::OnUpdateButtonF8 )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_F9, &CfrmUc141YakuinK::OnUpdateButtonF9 )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_F11, &CfrmUc141YakuinK::OnUpdateButtonF11 )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_F12, &CfrmUc141YakuinK::OnUpdateButtonF12 )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_HOME, &CfrmUc141YakuinK::OnUpdateButtonHome )
//	ON_UPDATE_COMMAND_UI( ID_DISPDIALOG_MENU, &CfrmUc141YakuinK::OnUpdateMenuIkkatuKingaku )
// midori 160612 add -->
	ON_UPDATE_COMMAND_UI( ID_CLEAR_MENU, &CfrmUc141YakuinK::OnUpdateMenuClear )
// midori 160612 add <--
	ON_UPDATE_COMMAND_UI( ID_ZEROMONEY_MENU, &CfrmUc141YakuinK::OnUpdateMenuZeroMoney )
// midori 160612 add -->
	ON_UPDATE_COMMAND_UI( ID_ZEROMONEYMOVE_MENU, &CfrmUc141YakuinK::OnUpdateMenuZeroMoneyMove)
// midori 160612 add <--
	ON_UPDATE_COMMAND_UI(ID_SPECIALROW_MENU, &CfrmUc141YakuinK::OnUpdateMenuSpecialRow)
// midori 160612 add -->
	ON_UPDATE_COMMAND_UI( ID_TITLEMENU, &CfrmUc141YakuinK::OnUpdateMenuTitleRestore )
	ON_UPDATE_COMMAND_UI( ID_DELETE_DATA_MENU, &CfrmUc141YakuinK::OnUpdateMenuDeleteDataEachYoushiki )
// midori 160612 add <--
	ON_MESSAGE(WM_USER_REDRAWVIEW, OnUserReDrawView)		// 修正No.162446 add
END_MESSAGE_MAP()


CfrmUc141YakuinK::CfrmUc141YakuinK()
	: CfrmUc000Common( CfrmUc141YakuinK::IDD )
{
	m_pDB = ( (CUCHIWAKEApp*)AfxGetApp() )->m_pDB;
	m_pTitleDiag = &m_TitleDiag;
// midori 152718 add -->
	m_pPageDiag	= &m_PageDiag;
// midori 152718 add <--
}

CfrmUc141YakuinK::~CfrmUc141YakuinK()
{
	// 2009.09.08 TS自動ログオフ対応(virEndProcへ移動)
	TRACE(_T("***** CfrmUc141YakuinK::~CfrmUc141YakuinK\n"));

	//m_pDiag = NULL;
	//m_pTitleDiag = NULL;
}

void CfrmUc141YakuinK::DoDataExchange( CDataExchange* pDX )
{
	CfrmUc000Common::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_YOUSHIKIDIAG1, m_DiagNormal );
	DDX_Control( pDX, IDC_YOUSHIKIDIAG3, m_DiagNormal2 );
	DDX_Control( pDX, IDC_YOUSHIKIDIAG2, m_DiagKobetu );
	DDX_Control( pDX, IDC_YOUSHIKIDIAG4, m_DiagKobetu2 );
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

void CfrmUc141YakuinK::OnInitialUpdate()
{
	CfrmUc000Common::OnInitialUpdate();
	ResizeParentToFit();

	// 帳表テーブルのオープンOK?
	if(m_InitialFlag == TRUE)	{
		// コンボボックスアイテムのリサイズ制御
		m_DiagNormal.ComboxFontMode(TRUE);
		m_DiagNormal2.ComboxFontMode(TRUE);
		m_DiagKobetu.ComboxFontMode(TRUE);
		m_DiagKobetu2.ComboxFontMode(TRUE);

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
			// ※詳細は④-1(frmUc041Karibarai.cpp)の同箇所のコメントを参照
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
void CfrmUc141YakuinK::AssertValid() const
{
	CfrmUc000Common::AssertValid();
}

#ifndef _WIN32_WCE
void CfrmUc141YakuinK::Dump( CDumpContext& dc ) const
{
	CfrmUc000Common::Dump( dc );
}
#endif
#endif //_DEBUG

BEGIN_EVENTSINK_MAP( CfrmUc141YakuinK, CfrmUc000Common )
	ON_EVENT( CfrmUc141YakuinK, IDC_YOUSHIKIDIAG1, 3, CfrmUc141YakuinK::TerminationYoushikidiag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN )
	ON_EVENT( CfrmUc141YakuinK, IDC_YOUSHIKIDIAG1, 2, CfrmUc141YakuinK::EditOFFYoushikidiag1, VTS_I2 )
	ON_EVENT( CfrmUc141YakuinK, IDC_YOUSHIKIDIAG1, 1, CfrmUc141YakuinK::EditONYoushikidiag1, VTS_I2 )
// midori 151405 add -->
	ON_EVENT(CfrmUc141YakuinK, IDC_YOUSHIKIDIAG1, 12, CfrmUc141YakuinK::VScrollYoushikidiag1, VTS_I2)
// midori 151405 add <--

	ON_EVENT( CfrmUc141YakuinK, IDC_YOUSHIKIDIAG2, 3, CfrmUc141YakuinK::TerminationYoushikidiag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN )
	ON_EVENT( CfrmUc141YakuinK, IDC_YOUSHIKIDIAG2, 2, CfrmUc141YakuinK::EditOFFYoushikidiag1, VTS_I2 )
	ON_EVENT( CfrmUc141YakuinK, IDC_YOUSHIKIDIAG2, 1, CfrmUc141YakuinK::EditONYoushikidiag1, VTS_I2 )
// midori 151405 add -->
	ON_EVENT(CfrmUc141YakuinK, IDC_YOUSHIKIDIAG2, 12, CfrmUc141YakuinK::VScrollYoushikidiag1, VTS_I2)
// midori 151405 add <--

	ON_EVENT( CfrmUc141YakuinK, IDC_YOUSHIKIDIAG3, 3, CfrmUc141YakuinK::TerminationYoushikidiag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN )
	ON_EVENT( CfrmUc141YakuinK, IDC_YOUSHIKIDIAG3, 2, CfrmUc141YakuinK::EditOFFYoushikidiag1, VTS_I2 )
	ON_EVENT( CfrmUc141YakuinK, IDC_YOUSHIKIDIAG3, 1, CfrmUc141YakuinK::EditONYoushikidiag1, VTS_I2 )
// midori 151405 add -->
	ON_EVENT(CfrmUc141YakuinK, IDC_YOUSHIKIDIAG3, 12, CfrmUc141YakuinK::VScrollYoushikidiag1, VTS_I2)
// midori 151405 add <--

	ON_EVENT( CfrmUc141YakuinK, IDC_YOUSHIKIDIAG4, 3, CfrmUc141YakuinK::TerminationYoushikidiag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN )
	ON_EVENT( CfrmUc141YakuinK, IDC_YOUSHIKIDIAG4, 2, CfrmUc141YakuinK::EditOFFYoushikidiag1, VTS_I2 )
	ON_EVENT( CfrmUc141YakuinK, IDC_YOUSHIKIDIAG4, 1, CfrmUc141YakuinK::EditONYoushikidiag1, VTS_I2 )
// midori 151405 add -->
	ON_EVENT(CfrmUc141YakuinK, IDC_YOUSHIKIDIAG4, 12, CfrmUc141YakuinK::VScrollYoushikidiag1, VTS_I2)
// midori 151405 add <--

	ON_EVENT( CfrmUc141YakuinK, IDC_TITLEDIAG1, 3, CfrmUc141YakuinK::TerminationTitlediag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN )
	ON_EVENT( CfrmUc141YakuinK, IDC_TITLEDIAG1, 2, CfrmUc141YakuinK::EditOFFTitlediag1, VTS_I2 )
	ON_EVENT( CfrmUc141YakuinK, IDC_TITLEDIAG1, 1, CfrmUc141YakuinK::EditONTitlediag1, VTS_I2 )
	ON_EVENT( CfrmUc141YakuinK, IDC_TITLEDIAG1, 6, CfrmUc141YakuinK::FocusTitlediag1, VTS_NONE )

	ON_EVENT( CfrmUc141YakuinK, IDC_PAGEDIAG1, 3, CfrmUc141YakuinK::TerminationPagediag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN )
	ON_EVENT( CfrmUc141YakuinK, IDC_PAGEDIAG1, 2, CfrmUc141YakuinK::EditOFFPagediag1, VTS_I2 )
	ON_EVENT( CfrmUc141YakuinK, IDC_PAGEDIAG1, 1, CfrmUc141YakuinK::EditONPagediag1, VTS_I2 )
	ON_EVENT( CfrmUc141YakuinK, IDC_PAGEDIAG1, 6, CfrmUc141YakuinK::FocusPagediag1, VTS_NONE )
END_EVENTSINK_MAP()

//********************************************************************************
//	初期処理
//********************************************************************************
BOOL	CfrmUc141YakuinK::virStartJob()
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

// midori 190301 add -->
	//// プロパティを変更していないので(9桁→10桁)、ここで設定する
	//// (データセット→桁数設定のながれだと、新様式起動時にオーバーフローで表示されるため)
	//if(bG_Kanso == TRUE)	{
	//	CmnDiagSetAttrDigit(m_pDiagKei,ID_COL_141_KEI1_VAL,ID_VAL_141_TOTAL_K);
	//	CmnDiagSetAttrDigit(m_pDiagKei,ID_COL_141_KEI2_VAL,ID_VAL_141_TOTAL_K);
	//}
	//else	{
	//	CmnDiagSetAttrDigit(m_pDiagKei,ID_COL_141_KEI1_VAL,ID_VAL_141_TOTAL);
	//	CmnDiagSetAttrDigit(m_pDiagKei,ID_COL_141_KEI2_VAL,ID_VAL_141_TOTAL);
	//}
// midori 190301 add <--

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
void	CfrmUc141YakuinK::virSetInitialUchiwakeInfo()
{
	// 「役員報酬」の固有データ
	m_uInfo.intOutKeiDialog = AM_TYPE_ALL;				// 計設定ダイアログのボタン指定
	m_uInfo.intRowMax = ID_ROWKOBETU_141;				// １頁内の合計行を含む全体の行数
	m_uInfo.intRowMaxData = ID_ROWKOBETU_141 - 1;		// １頁内のデータ行数（合計行は含まず）
	m_uInfo.intColMax = ID_COLMAX_141;					// １行内のコントロール個数
	m_uInfo.intColMaxData = ID_COLMAXDATA_141;			// １行内の列数
	m_uInfo.intCtlStartIndex = ID_CTLSTARTINDEX_141;	// １行目の入力項目の先頭インデックス番号
	m_uInfo.intT1StartIndex = ID_T1STARTNORMAL_141;		// 合計行①の先頭インデックス番号
	m_uInfo.intT2StartIndex = ID_T2STARTNORMAL_141;		// 合計行②の入力項目の先頭インデックス番号
	m_uInfo.intCursolDefPos = ID_COL_141_NAMEYAKU;		// カーソル移動時の初期位置（タイトルから↓，合計から↑で使用）
	m_uInfo.intCursolKeiPos = ID_COL_141_NAMEYAKU;		// 合計行のカーソル移動位置（カーソル移動で使用）

	m_uInfo.intFormSeq = ID_FORMNO_141;					// 様式シーケンス番号
	m_uInfo.intFormNo = 14;								// 様式番号（uc_inf_sub）
	m_uInfo.intFormNo2 = 1;								// 様式番号枝番（uc_inf_sub）

	m_uInfo.intRowNormal = ID_ROWNORMAL_141;			// 通常時：１頁内の合計行を含む全体の行数
	m_uInfo.intRowKobetu = ID_ROWKOBETU_141;			// 個別時：１頁内の合計行を含む全体の行数
	m_uInfo.intT1StartNormal = ID_T1STARTNORMAL_141;	// 通常時：合計行①の先頭インデックス
	m_uInfo.intT2StartNormal = ID_T2STARTNORMAL_141;	// 通常時：合計行②の先頭インデックス
	m_uInfo.intT1StartKobetu = ID_T1STARTKOBETU_141;	// 個別時：合計行①の先頭インデックス
	m_uInfo.intT2StartKobetu = ID_T2STARTKOBETU_141;	// 個別時：合計行②の先頭インデックス
	m_uInfo.intRowKei = ID_ROWKEI_141;					// 帳表固定の合計行数（1:合計あり，0:なし）
	m_uInfo.intKei1StartIndex = ID_COL_141_KEI1_MEISYO;	// 計ダイアグラム1行目の先頭インデックス
	m_uInfo.intKei2StartIndex = ID_COL_141_KEI2_MEISYO;	// 計ダイアグラム2行目の先頭インデックス

	// 集計処理の設定
	m_uInfo.uCalcInfo.intMaxCount = ID_CALC_141_MAX;		// 集計フィールド件数
	m_uInfo.uCalcInfo.strField[0] = ID_CALC_141_FIELD1;		// 集計フィールド名称1
	m_uInfo.uCalcInfo.strField[1] = ID_CALC_141_FIELD2;		// 集計フィールド名称2
	m_uInfo.uCalcInfo.strField[2] = ID_CALC_141_FIELD3;		// 集計フィールド名称3
	m_uInfo.uCalcInfo.strField[3] = ID_CALC_141_FIELD4;		// 集計フィールド名称4
	m_uInfo.uCalcInfo.strField[4] = ID_CALC_141_FIELD5;		// 集計フィールド名称5
	m_uInfo.uCalcInfo.strField[5] = ID_CALC_141_FIELD6;		// 集計フィールド名称6
	m_uInfo.uCalcInfo.strField[6] = ID_CALC_141_FIELD7;		// 集計フィールド名称6
}

//**************************************************
// ICSDiagコントロールを通常／個別で切り替える
// 【引数】    なし
// 【戻値】    なし
//**************************************************
void	CfrmUc141YakuinK::virSetInitialDiag()
{
	// 別の入力画面から参照した場合、画面表示しないので、
	// 各オブジェクトはメンバ変数に割り当てられていない。
	if ((m_DiagNormal.m_hWnd == NULL) || (m_DiagNormal2.m_hWnd == NULL) ||
		(m_DiagKobetu.m_hWnd == NULL) || (m_DiagKobetu2.m_hWnd == NULL)) {
		return;
	}

	// 出力形式で分岐
	switch ( m_uInfo.intFormType ) {
	// 通常
	case ID_FORMTYPE_NORMAL:
		m_DiagNormal.ShowWindow( TRUE );
		m_DiagNormal2.ShowWindow( TRUE );
		m_DiagKobetu.ShowWindow( FALSE );
		m_DiagKobetu2.ShowWindow( FALSE );
		m_pDiag = CmnGetDiagType( &m_DiagNormal, &m_DiagNormal2);
		m_pDiagKei = CmnGetDiagType( &m_DiagKei1, &m_DiagKei2 );
		m_pDiagHide = CmnGetDiagType( &m_DiagKobetu, &m_DiagKobetu2);
		m_intT1StartIndexHide = m_uInfo.intT1StartKobetu;
		m_intT2StartIndexHide = m_uInfo.intT2StartKobetu;
		break;
	// 個別
	case ID_FORMTYPE_KOBETU:
		m_DiagNormal.ShowWindow( FALSE );
		m_DiagNormal2.ShowWindow( FALSE );
		m_DiagKobetu.ShowWindow( TRUE );
		m_DiagKobetu2.ShowWindow( TRUE );
		m_pDiag = CmnGetDiagType( &m_DiagKobetu, &m_DiagKobetu2);
		m_pDiagKei = CmnGetDiagType( &m_DiagKei1, &m_DiagKei2 );
		m_pDiagHide = CmnGetDiagType( &m_DiagNormal, &m_DiagNormal2 );
		m_intT1StartIndexHide = m_uInfo.intT1StartNormal;
		m_intT2StartIndexHide = m_uInfo.intT2StartNormal;
		break;
	}
	// 隠すダイアログを非表示
	m_pDiagHide->ShowWindow( FALSE );

	// 入力不可行でのDeleteキー無効
	m_pDiag->EnableDelete(0);

}

//********************************************************************************
//	帳表固有変数の初期値設定
//********************************************************************************
void	CfrmUc141YakuinK::virSetInitialValue()
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
BOOL	CfrmUc141YakuinK::InitialControlDataArea()
{
	int		intMax;
	int		i;

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
		CmnDiagSetString( m_pDiag, i + ID_COL_141_FPTIME - 1, "", 0 );	// 常勤・非常勤の区別
			
		// 漢字入力欄の初期化
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_141_NAMEYAKU - 1, MAX_KETA_141_NAMEYAKU );	//	役職名
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_141_NAMETANT - 1, MAX_KETA_141_NAMETANT );	//	担当業務
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_141_ADNAME1  - 1, MAX_KETA_141_ADNAME1 );		//	名称１
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_141_KANKEI   - 1, MAX_KETA_141_KANKEI );		//	代表者との関係
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_141_ADADD1   - 1, MAX_KETA_141_ADADD1 );		//	住所１
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_141_ADADD2   - 1, MAX_KETA_141_ADADD2 );		//	住所２

		// 金額欄：桁数初期化
// 改良No.23-0961 del -->
//		CmnDiagSetAttrDigit( m_pDiag, i + ID_COL_141_EMVAL - 1, ID_EMVAL_141_DATA );	// 使用人職務分
//		CmnDiagSetAttrDigit( m_pDiag, i + ID_COL_141_PRVAL - 1, ID_PRVAL_141_DATA );	// 定期同額給与
//		CmnDiagSetAttrDigit( m_pDiag, i + ID_COL_141_BFVAL - 1, ID_BFVAL_141_DATA );	// 事前確定届出給与
//		CmnDiagSetAttrDigit( m_pDiag, i + ID_COL_141_PFVAL - 1, ID_PFVAL_141_DATA );	// 利益連動給与
//		CmnDiagSetAttrDigit( m_pDiag, i + ID_COL_141_ETVAL - 1, ID_ETVAL_141_DATA );	// その他
//		CmnDiagSetAttrDigit( m_pDiag, i + ID_COL_141_RTVAL - 1, ID_RTVAL_141_DATA );	// 退職給与
//// midori 190301 del -->
//		//CmnDiagSetAttrDigit( m_pDiag, i + ID_COL_141_VAL - 1, ID_VAL_141_DATA );		// 役員給与計
//// midori 190301 del <--
//// midori 190301 add -->
//		if(bG_Kanso == TRUE)	{
//			CmnDiagSetAttrDigit(m_pDiag,i+ID_COL_141_VAL-1,ID_VAL_141_DATA_K);	// 役員給与計
//		}
//		else	{
//			CmnDiagSetAttrDigit(m_pDiag,i+ID_COL_141_VAL-1,ID_VAL_141_DATA);	// 役員給与計
//		}
// midori 190301 add <--
// 改良No.23-0961 del <--
// 改良No.23-0961 add -->
		if(bG_Kanso == TRUE) {
			CmnDiagSetAttrDigit(m_pDiag, i + ID_COL_141_EMVAL - 1, ID_EMVAL_141_DATA_K);	// 使用人職務分
			CmnDiagSetAttrDigit(m_pDiag, i + ID_COL_141_PRVAL - 1, ID_PRVAL_141_DATA_K);	// 定期同額給与
			CmnDiagSetAttrDigit(m_pDiag, i + ID_COL_141_BFVAL - 1, ID_BFVAL_141_DATA_K);	// 事前確定届出給与
			CmnDiagSetAttrDigit(m_pDiag, i + ID_COL_141_PFVAL - 1, ID_PFVAL_141_DATA_K);	// 利益連動給与
			CmnDiagSetAttrDigit(m_pDiag, i + ID_COL_141_ETVAL - 1, ID_ETVAL_141_DATA_K);	// その他
			CmnDiagSetAttrDigit(m_pDiag, i + ID_COL_141_RTVAL - 1, ID_RTVAL_141_DATA_K);	// 退職給与
			CmnDiagSetAttrDigit(m_pDiag, i + ID_COL_141_VAL   - 1, ID_VAL_141_DATA_K);		// 役員給与計
		}
		else	{
			CmnDiagSetAttrDigit(m_pDiag, i + ID_COL_141_EMVAL - 1, ID_EMVAL_141_DATA);		// 使用人職務分
			CmnDiagSetAttrDigit(m_pDiag, i + ID_COL_141_PRVAL - 1, ID_PRVAL_141_DATA);		// 定期同額給与
			CmnDiagSetAttrDigit(m_pDiag, i + ID_COL_141_BFVAL - 1, ID_BFVAL_141_DATA);		// 事前確定届出給与
			CmnDiagSetAttrDigit(m_pDiag, i + ID_COL_141_PFVAL - 1, ID_PFVAL_141_DATA);		// 利益連動給与
			CmnDiagSetAttrDigit(m_pDiag, i + ID_COL_141_ETVAL - 1, ID_ETVAL_141_DATA);		// その他
			CmnDiagSetAttrDigit(m_pDiag, i + ID_COL_141_RTVAL - 1, ID_RTVAL_141_DATA);		// 退職給与
			CmnDiagSetAttrDigit(m_pDiag, i + ID_COL_141_VAL   - 1, ID_VAL_141_DATA);		// 役員給与計
		}
// 改良No.23-0961 add <--

		//	入力不可モードの解除
		SetInputMode( CmnGetControlRow( i ), DIAG_MDFY_INPUT );
	}

	// ComboBoxの初期設定
	if ( InitialControlComboBox() != TRUE ){
		return FALSE;
	}

	// 半角/全角設定
	CmnDiagSetAttrColZenHan( m_pDiag , ID_COL_141_NAMEYAKU );	// 役職名
	CmnDiagSetAttrColZenHan( m_pDiag , ID_COL_141_NAMETANT );	// 担当業務
	CmnDiagSetAttrColZenHan( m_pDiag , ID_COL_141_ADNAME1 );	// 氏名1
	CmnDiagSetAttrColZenHan( m_pDiag , ID_COL_141_KANKEI );		// 代表者との関係
	CmnDiagSetAttrColZenHan( m_pDiag , ID_COL_141_ADADD1 );		// 住所1
	CmnDiagSetAttrColZenHan( m_pDiag , ID_COL_141_ADADD2 );		// 住所2

	return TRUE;
}

//********************************************************************************
//	帳表コントロールの初期化（データ領域）
//		→表示していない方のDiag(通常 or 個別)の設定に使用する
//********************************************************************************
BOOL	CfrmUc141YakuinK::InitialControlDataAreaHide( CICSDiag* pDiag )
{
	int		intMax;
	int		i;

	// 列の属性により更にクリアと初期設定が必要
	intMax = m_intT1StartIndexHide;
	for ( i = m_uInfo.intCtlStartIndex; i < intMax; i += m_uInfo.intColMax ) {
		// ComboBox：クリア
		CmnDiagSetString( pDiag, i + ID_COL_141_FPTIME - 1, "", 0 );	// 常勤・非常勤の区別
			
		// 漢字入力欄の初期化
		CmnDiagSetAttrInpmax( pDiag, i + ID_COL_141_NAMEYAKU    - 1, MAX_KETA_141_NAMEYAKU );		//	役職名
		CmnDiagSetAttrInpmax( pDiag, i + ID_COL_141_NAMETANT    - 1, MAX_KETA_141_NAMETANT );		//	担当業務
		CmnDiagSetAttrInpmax( pDiag, i + ID_COL_141_ADNAME1    - 1, MAX_KETA_141_ADNAME1 );		//	名称１
		CmnDiagSetAttrInpmax( pDiag, i + ID_COL_141_KANKEI    - 1, MAX_KETA_141_KANKEI );			//	代表者との関係
		CmnDiagSetAttrInpmax( pDiag, i + ID_COL_141_ADADD1    - 1, MAX_KETA_141_ADADD1 );			//	住所１
		CmnDiagSetAttrInpmax( pDiag, i + ID_COL_141_ADADD2    - 1, MAX_KETA_141_ADADD2 );			//	住所２

		// 金額欄：桁数初期化
// 改良No.23-0961 del -->
//		CmnDiagSetAttrDigit( pDiag, i + ID_COL_141_EMVAL - 1, ID_EMVAL_141_DATA );	// 使用人職務分
//		CmnDiagSetAttrDigit( pDiag, i + ID_COL_141_PRVAL - 1, ID_PRVAL_141_DATA );	// 定期同額給与
//		CmnDiagSetAttrDigit( pDiag, i + ID_COL_141_BFVAL - 1, ID_BFVAL_141_DATA );	// 事前確定届出給与
//		CmnDiagSetAttrDigit( pDiag, i + ID_COL_141_PFVAL - 1, ID_PFVAL_141_DATA );	// 利益連動給与
//		CmnDiagSetAttrDigit( pDiag, i + ID_COL_141_ETVAL - 1, ID_ETVAL_141_DATA );	// その他
//		CmnDiagSetAttrDigit( pDiag, i + ID_COL_141_RTVAL - 1, ID_RTVAL_141_DATA );	// 退職給与
//// midori 190301 del -->
//		//CmnDiagSetAttrDigit( pDiag, i + ID_COL_141_VAL - 1, ID_VAL_141_DATA );		// 役員給与計
//// midori 190301 del <--
//// midori 190301 add -->
//		if(bG_Kanso == TRUE)	{
//			CmnDiagSetAttrDigit(pDiag,i+ID_COL_141_VAL-1,ID_VAL_141_DATA_K);	// 役員給与計
//		}
//		else	{
//			CmnDiagSetAttrDigit(pDiag,i+ID_COL_141_VAL-1,ID_VAL_141_DATA);		// 役員給与計
//		}
//// midori 190301 add <--
// 改良No.23-0961 del <--
// 改良No.23-0961 add -->
		if(bG_Kanso == TRUE)	{
			CmnDiagSetAttrDigit(pDiag, i + ID_COL_141_EMVAL - 1, ID_EMVAL_141_DATA_K);	// 使用人職務分
			CmnDiagSetAttrDigit(pDiag, i + ID_COL_141_PRVAL - 1, ID_PRVAL_141_DATA_K);	// 定期同額給与
			CmnDiagSetAttrDigit(pDiag, i + ID_COL_141_BFVAL - 1, ID_BFVAL_141_DATA_K);	// 事前確定届出給与
			CmnDiagSetAttrDigit(pDiag, i + ID_COL_141_PFVAL - 1, ID_PFVAL_141_DATA_K);	// 利益連動給与
			CmnDiagSetAttrDigit(pDiag, i + ID_COL_141_ETVAL - 1, ID_ETVAL_141_DATA_K);	// その他
			CmnDiagSetAttrDigit(pDiag, i + ID_COL_141_RTVAL - 1, ID_RTVAL_141_DATA_K);	// 退職給与
			CmnDiagSetAttrDigit(pDiag, i + ID_COL_141_VAL   - 1, ID_VAL_141_DATA_K);	// 役員給与計
		}
		else	{
			CmnDiagSetAttrDigit(pDiag, i + ID_COL_141_EMVAL - 1, ID_EMVAL_141_DATA);	// 使用人職務分
			CmnDiagSetAttrDigit(pDiag, i + ID_COL_141_PRVAL - 1, ID_PRVAL_141_DATA);	// 定期同額給与
			CmnDiagSetAttrDigit(pDiag, i + ID_COL_141_BFVAL - 1, ID_BFVAL_141_DATA);	// 事前確定届出給与
			CmnDiagSetAttrDigit(pDiag, i + ID_COL_141_PFVAL - 1, ID_PFVAL_141_DATA);	// 利益連動給与
			CmnDiagSetAttrDigit(pDiag, i + ID_COL_141_ETVAL - 1, ID_ETVAL_141_DATA);	// その他
			CmnDiagSetAttrDigit(pDiag, i + ID_COL_141_RTVAL - 1, ID_RTVAL_141_DATA);	// 退職給与
			CmnDiagSetAttrDigit(pDiag, i + ID_COL_141_VAL   - 1, ID_VAL_141_DATA);		// 役員給与計
		}
// 改良No.23-0961 add <--
	}

	return TRUE;
}

//********************************************************************************
//	帳表コントロールの初期化（ComboBox）
//********************************************************************************
BOOL	CfrmUc141YakuinK::InitialControlComboBox()
{
	DIAGRAM_DATA	diadata;	// ICSDIAG構造体
	int				intTotalType;
	int				intMax;
	int				intIndex;
	int				i;

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
	diadata.text_combo.Format( "%s\r\n%s\r\n%s\r\n", "　" ,"常" ,"非" ); 

	// 全行設定
	for ( i = 1; i <= intMax; i++ ) {
		// 対象コントロールのインデックス番号を取得
		intIndex = CmnGetControlIndex( i, ID_COL_141_FPTIME );

		// 初期設定（ドロップダウンON＋リスト情報をセット）
		m_pDiag->ModifyItem( intIndex, DIAG_MDFY_NODROPDOWN );
		m_pDiag->SetData( intIndex, (LPUNKNOWN)&diadata );
	}

	return TRUE;
}

//********************************************************************************
//	帳表コントロールの初期化（頁計/累計領域）
//		文言はデータに含まれているので、その内容を使用する
//********************************************************************************
void	CfrmUc141YakuinK::InitialControlTotalArea()
{
int		nKeiType;			//	計の種別
	int		nDataRow;			//	データ行属性
	int		nDataRowPos;		//	データ行位置
	int		nKeiRow[2];			//	計行属性
	int		nCnt;				//	カウント
	int		nStart;				//	開始位置
// midori 152166 add -->
	int		intIndex=0;
	int		nSw=0;

	intIndex = CmnGetControlIndex(m_uInfo.intCurRow,m_uInfo.intCurCol);
// midori 152166 add <--

	// カレント頁の合計行の種別を取得
	nKeiType = CmnGetControlTotalType();
	
	nDataRowPos = m_uInfo.intRowMax - 2;

	//	計の種別で分岐
	switch ( nKeiType ){
	case ID_OUTKEI_OFF:				// なし
	case ID_OUTKEI_PAGEKEI:			// 頁計
	case ID_OUTKEI_RUIKEI:			// 累計
		//	データ行の設定
// midori 160612 cor -->
		//nDataRow = ( DIAG_MDFY_EDIT	| DIAG_MDFY_OPAQUE );
// ---------------------
// midori 152364 del -->
		//if(m_DataKakutei == FALSE)	nDataRow = ( DIAG_MDFY_EDIT	| DIAG_MDFY_OPAQUE );
// midori 152364 del <--
// midori 152364 add -->
		if(m_DataKakutei == FALSE)	{
			if(IsSpecialRow(m_ReadData[nDataRowPos+1].m_FgFunc) != 0)	{
				nDataRow = (DIAG_MDFY_NOINPUT | DIAG_MDFY_OPAQUE);
			}
			else	{
				nDataRow = (DIAG_MDFY_EDIT | DIAG_MDFY_OPAQUE);
			}
		}
// midori 152364 add <--
		else						nDataRow = ( DIAG_MDFY_READONLY	| DIAG_MDFY_OPAQUE );
// midori 160612 cor <--
		//	計行の設定
		nKeiRow[0] = ( DIAG_MDFY_READONLY | DIAG_MDFY_TRANSPARENT );
		nKeiRow[1] = ( DIAG_MDFY_READONLY | DIAG_MDFY_OPAQUE );
		break;
	case ID_OUTKEI_BOTH:			// 頁計と累計
		//	データ行の設定
		nDataRow = ( DIAG_MDFY_READONLY | DIAG_MDFY_TRANSPARENT );
		//	計行の設定
		nKeiRow[0] = ( DIAG_MDFY_READONLY | DIAG_MDFY_OPAQUE );
		nKeiRow[1] = ( DIAG_MDFY_READONLY | DIAG_MDFY_OPAQUE );
		break;
	}

	// 28行目のデータ行
	nStart = nDataRowPos * m_uInfo.intColMax;
	for ( nCnt = 0; nCnt < m_uInfo.intColMax; nCnt++){
		m_pDiag->ModifyItem( nStart + nCnt, nDataRow );
// midori 152166 add -->
		if((intIndex == (nStart + nCnt)) && ((nDataRow & DIAG_MDFY_READONLY) != 0))	nSw=1;
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
// midori 152166 add -->
	if(nSw != 0)	{
		// 現在のカーソルポジションが入力不可になった場合は先頭に戻す
		m_uInfo.intCurRow = 1;
		m_uInfo.intCurCol = 1;
	}
// midori 152166 add <--
	// 計専用ダイアグラムの金額欄を入力不可にする
	int intKeiRow = (DIAG_MDFY_READONLY | DIAG_MDFY_OPAQUE);
	m_pDiagKei->ModifyItem(ID_COL_141_KEI1_EMVAL, intKeiRow);
	m_pDiagKei->ModifyItem(ID_COL_141_KEI2_EMVAL, intKeiRow);
	m_pDiagKei->ModifyItem(ID_COL_141_KEI1_PRVAL, intKeiRow);
	m_pDiagKei->ModifyItem(ID_COL_141_KEI2_PRVAL, intKeiRow);
	m_pDiagKei->ModifyItem(ID_COL_141_KEI1_BFVAL, intKeiRow);
	m_pDiagKei->ModifyItem(ID_COL_141_KEI2_BFVAL, intKeiRow);
	m_pDiagKei->ModifyItem(ID_COL_141_KEI1_PFVAL, intKeiRow);
	m_pDiagKei->ModifyItem(ID_COL_141_KEI2_PFVAL, intKeiRow);
	m_pDiagKei->ModifyItem(ID_COL_141_KEI1_ETVAL, intKeiRow);
	m_pDiagKei->ModifyItem(ID_COL_141_KEI2_ETVAL, intKeiRow);
	m_pDiagKei->ModifyItem(ID_COL_141_KEI1_RTVAL, intKeiRow);
	m_pDiagKei->ModifyItem(ID_COL_141_KEI2_RTVAL, intKeiRow);
	m_pDiagKei->ModifyItem(ID_COL_141_KEI1_VAL, intKeiRow);
	m_pDiagKei->ModifyItem(ID_COL_141_KEI2_VAL, intKeiRow);

	//// 合計行の名称欄は、カーソルを飛ばしたいので入力許可する
	//switch ( nKeiType ) {
	//case ID_OUTKEI_OFF:
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
	//
	//// 入力可能（ただし、画面レイアウトで入力文字数"0"としているため、実際は入力できない）
	//m_pDiag->ModifyItem( CmnGetControlTotalIndex( 1, ID_COL_141_NAMEYAKU ), nKeiRow[0] );
	//m_pDiag->ModifyItem( CmnGetControlTotalIndex( 2, ID_COL_141_NAMEYAKU ), nKeiRow[1] );
	
	// 合計行の金額欄は、金額欄 + 1桁
// 改良No.23-0961 del -->
//	CmnDiagSetAttrDigit( m_pDiagKei, ID_COL_141_KEI1_EMVAL, ID_EMVAL_141_TOTAL );
//	CmnDiagSetAttrDigit( m_pDiagKei, ID_COL_141_KEI1_PRVAL, ID_PRVAL_141_TOTAL );
//	CmnDiagSetAttrDigit( m_pDiagKei, ID_COL_141_KEI1_BFVAL, ID_BFVAL_141_TOTAL );
//	CmnDiagSetAttrDigit( m_pDiagKei, ID_COL_141_KEI1_PFVAL, ID_PFVAL_141_TOTAL );
//	CmnDiagSetAttrDigit( m_pDiagKei, ID_COL_141_KEI1_ETVAL, ID_ETVAL_141_TOTAL );
//	CmnDiagSetAttrDigit( m_pDiagKei, ID_COL_141_KEI1_RTVAL, ID_RTVAL_141_TOTAL );
//// midori 190301 del -->
//	//CmnDiagSetAttrDigit( m_pDiagKei, ID_COL_141_KEI1_VAL, ID_VAL_141_TOTAL );
//// midori 190301 del <--
//// midori 190301 add -->
//	if(bG_Kanso == TRUE)	{
//		CmnDiagSetAttrDigit(m_pDiagKei,ID_COL_141_KEI1_VAL,ID_VAL_141_TOTAL_K);
//	}
//	else	{
//		CmnDiagSetAttrDigit(m_pDiagKei,ID_COL_141_KEI1_VAL,ID_VAL_141_TOTAL);
//	}
//// midori 190301 add <--
//
//	CmnDiagSetAttrDigit( m_pDiagKei, ID_COL_141_KEI2_EMVAL, ID_EMVAL_141_TOTAL );
//	CmnDiagSetAttrDigit( m_pDiagKei, ID_COL_141_KEI2_PRVAL, ID_PRVAL_141_TOTAL );
//	CmnDiagSetAttrDigit( m_pDiagKei, ID_COL_141_KEI2_BFVAL, ID_BFVAL_141_TOTAL );
//	CmnDiagSetAttrDigit( m_pDiagKei, ID_COL_141_KEI2_PFVAL, ID_PFVAL_141_TOTAL );
//	CmnDiagSetAttrDigit( m_pDiagKei, ID_COL_141_KEI2_ETVAL, ID_ETVAL_141_TOTAL );
//	CmnDiagSetAttrDigit( m_pDiagKei, ID_COL_141_KEI2_RTVAL, ID_RTVAL_141_TOTAL );
//// midori 190301 del -->
//	//CmnDiagSetAttrDigit( m_pDiagKei, ID_COL_141_KEI2_VAL, ID_VAL_141_TOTAL );
//// midori 190301 del <--
//// midori 190301 add -->
//	if(bG_Kanso == TRUE)	{
//		CmnDiagSetAttrDigit(m_pDiagKei,ID_COL_141_KEI2_VAL,ID_VAL_141_TOTAL_K);
//	}
//	else	{
//		CmnDiagSetAttrDigit(m_pDiagKei,ID_COL_141_KEI2_VAL,ID_VAL_141_TOTAL);
//	}
//// midori 190301 add <--
//	//CmnDiagSetAttrDigit( m_pDiag, CmnGetControlTotalIndex( 1, ID_COL_141_EMVAL ), ID_EMVAL_141_TOTAL );
//	//CmnDiagSetAttrDigit( m_pDiag, CmnGetControlTotalIndex( 1, ID_COL_141_PRVAL ), ID_PRVAL_141_TOTAL );
//	//CmnDiagSetAttrDigit( m_pDiag, CmnGetControlTotalIndex( 1, ID_COL_141_BFVAL ), ID_BFVAL_141_TOTAL );
//	//CmnDiagSetAttrDigit( m_pDiag, CmnGetControlTotalIndex( 1, ID_COL_141_PFVAL ), ID_PFVAL_141_TOTAL );
//	//CmnDiagSetAttrDigit( m_pDiag, CmnGetControlTotalIndex( 1, ID_COL_141_ETVAL ), ID_ETVAL_141_TOTAL );
//	//CmnDiagSetAttrDigit( m_pDiag, CmnGetControlTotalIndex( 1, ID_COL_141_RTVAL ), ID_RTVAL_141_TOTAL );
//	//CmnDiagSetAttrDigit( m_pDiag, CmnGetControlTotalIndex( 1, ID_COL_141_VAL ), ID_VAL_141_TOTAL );
//
//	//CmnDiagSetAttrDigit( m_pDiag, CmnGetControlTotalIndex( 2, ID_COL_141_EMVAL ), ID_EMVAL_141_TOTAL );
//	//CmnDiagSetAttrDigit( m_pDiag, CmnGetControlTotalIndex( 2, ID_COL_141_PRVAL ), ID_PRVAL_141_TOTAL );
//	//CmnDiagSetAttrDigit( m_pDiag, CmnGetControlTotalIndex( 2, ID_COL_141_BFVAL ), ID_BFVAL_141_TOTAL );
//	//CmnDiagSetAttrDigit( m_pDiag, CmnGetControlTotalIndex( 2, ID_COL_141_PFVAL ), ID_PFVAL_141_TOTAL );
//	//CmnDiagSetAttrDigit( m_pDiag, CmnGetControlTotalIndex( 2, ID_COL_141_ETVAL ), ID_ETVAL_141_TOTAL );
//	//CmnDiagSetAttrDigit( m_pDiag, CmnGetControlTotalIndex( 2, ID_COL_141_RTVAL ), ID_RTVAL_141_TOTAL );
//	//CmnDiagSetAttrDigit( m_pDiag, CmnGetControlTotalIndex( 2, ID_COL_141_VAL ), ID_VAL_141_TOTAL );
// 改良No.23-0961 del <--
// 改良No.23-0961 add -->
	// 計１
	if(bG_Kanso == TRUE)	{
		CmnDiagSetAttrDigit(m_pDiagKei, ID_COL_141_KEI1_EMVAL, ID_EMVAL_141_TOTAL_K);
		CmnDiagSetAttrDigit(m_pDiagKei, ID_COL_141_KEI1_PRVAL, ID_PRVAL_141_TOTAL_K);
		CmnDiagSetAttrDigit(m_pDiagKei, ID_COL_141_KEI1_BFVAL, ID_BFVAL_141_TOTAL_K);
		CmnDiagSetAttrDigit(m_pDiagKei, ID_COL_141_KEI1_PFVAL, ID_PFVAL_141_TOTAL_K);
		CmnDiagSetAttrDigit(m_pDiagKei, ID_COL_141_KEI1_ETVAL, ID_ETVAL_141_TOTAL_K);
		CmnDiagSetAttrDigit(m_pDiagKei, ID_COL_141_KEI1_RTVAL, ID_RTVAL_141_TOTAL_K);
		CmnDiagSetAttrDigit(m_pDiagKei, ID_COL_141_KEI1_VAL,   ID_VAL_141_TOTAL_K);
	}
	else	{
		CmnDiagSetAttrDigit(m_pDiagKei, ID_COL_141_KEI1_EMVAL, ID_EMVAL_141_TOTAL);
		CmnDiagSetAttrDigit(m_pDiagKei, ID_COL_141_KEI1_PRVAL, ID_PRVAL_141_TOTAL);
		CmnDiagSetAttrDigit(m_pDiagKei, ID_COL_141_KEI1_BFVAL, ID_BFVAL_141_TOTAL);
		CmnDiagSetAttrDigit(m_pDiagKei, ID_COL_141_KEI1_PFVAL, ID_PFVAL_141_TOTAL);
		CmnDiagSetAttrDigit(m_pDiagKei, ID_COL_141_KEI1_ETVAL, ID_ETVAL_141_TOTAL);
		CmnDiagSetAttrDigit(m_pDiagKei, ID_COL_141_KEI1_RTVAL, ID_RTVAL_141_TOTAL);
		CmnDiagSetAttrDigit(m_pDiagKei, ID_COL_141_KEI1_VAL,   ID_VAL_141_TOTAL);
	}
	// 計２
	if(bG_Kanso == TRUE)	{
		CmnDiagSetAttrDigit(m_pDiagKei, ID_COL_141_KEI2_EMVAL, ID_EMVAL_141_TOTAL_K);
		CmnDiagSetAttrDigit(m_pDiagKei, ID_COL_141_KEI2_PRVAL, ID_PRVAL_141_TOTAL_K);
		CmnDiagSetAttrDigit(m_pDiagKei, ID_COL_141_KEI2_BFVAL, ID_BFVAL_141_TOTAL_K);
		CmnDiagSetAttrDigit(m_pDiagKei, ID_COL_141_KEI2_PFVAL, ID_PFVAL_141_TOTAL_K);
		CmnDiagSetAttrDigit(m_pDiagKei, ID_COL_141_KEI2_ETVAL, ID_ETVAL_141_TOTAL_K);
		CmnDiagSetAttrDigit(m_pDiagKei, ID_COL_141_KEI2_RTVAL, ID_RTVAL_141_TOTAL_K);
		CmnDiagSetAttrDigit(m_pDiagKei, ID_COL_141_KEI2_VAL,   ID_VAL_141_TOTAL_K);
	}
	else	{
		CmnDiagSetAttrDigit(m_pDiagKei, ID_COL_141_KEI2_EMVAL, ID_EMVAL_141_TOTAL);
		CmnDiagSetAttrDigit(m_pDiagKei, ID_COL_141_KEI2_PRVAL, ID_PRVAL_141_TOTAL);
		CmnDiagSetAttrDigit(m_pDiagKei, ID_COL_141_KEI2_BFVAL, ID_BFVAL_141_TOTAL);
		CmnDiagSetAttrDigit(m_pDiagKei, ID_COL_141_KEI2_PFVAL, ID_PFVAL_141_TOTAL);
		CmnDiagSetAttrDigit(m_pDiagKei, ID_COL_141_KEI2_ETVAL, ID_ETVAL_141_TOTAL);
		CmnDiagSetAttrDigit(m_pDiagKei, ID_COL_141_KEI2_RTVAL, ID_RTVAL_141_TOTAL);
		CmnDiagSetAttrDigit(m_pDiagKei, ID_COL_141_KEI2_VAL,   ID_VAL_141_TOTAL);
	}
// 改良No.23-0961 add <--
}

//********************************************************************************
//	帳表コントロールの初期化（頁計/累計領域）
//		→表示していない方のDiag(通常 or 個別)の設定に使用する
//********************************************************************************
void	CfrmUc141YakuinK::InitialControlTotalAreaHide( CICSDiag* pDiag )
{
	int intIndex = 0;

	//＜注意＞
	// 下記のインデックス番号の算出は「CmnGetControlTotalIndex」でも同じ計算をしている
	// そのためどちらかのシーケンスが変わるとお互いに影響する可能性がある

	// 合計行の金額欄は、金額欄 + 1桁
// 改良No.23-0961 del -->
//	intIndex = m_intT1StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_EMVAL - 1;
//	CmnDiagSetAttrDigit( pDiag, intIndex, ID_EMVAL_141_TOTAL );
//
//	intIndex = m_intT1StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_PRVAL - 1;
//	CmnDiagSetAttrDigit( pDiag, intIndex, ID_PRVAL_141_TOTAL );
//
//	intIndex = m_intT1StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_BFVAL - 1;
//	CmnDiagSetAttrDigit( pDiag, intIndex, ID_BFVAL_141_TOTAL );
//
//	intIndex = m_intT1StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_PFVAL - 1;
//	CmnDiagSetAttrDigit( pDiag, intIndex, ID_PFVAL_141_TOTAL );
//
//	intIndex = m_intT1StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_ETVAL - 1;
//	CmnDiagSetAttrDigit( pDiag, intIndex, ID_ETVAL_141_TOTAL );
//
//	intIndex = m_intT1StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_RTVAL - 1;
//	CmnDiagSetAttrDigit( pDiag, intIndex, ID_RTVAL_141_TOTAL );
//
//	intIndex = m_intT1StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_VAL - 1;
//// midori 190301 del -->
//	//CmnDiagSetAttrDigit( pDiag, intIndex, ID_VAL_141_TOTAL );
//// midori 190301 del <--
//// midori 190301 add -->
//	if(bG_Kanso == TRUE)	{
//		CmnDiagSetAttrDigit(pDiag,intIndex,ID_VAL_141_TOTAL_K);
//	}
//	else	{
//		CmnDiagSetAttrDigit(pDiag,intIndex,ID_VAL_141_TOTAL);
//	}
//// midori 190301 add <--
//
//	intIndex = m_intT2StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_EMVAL - 1;
//	CmnDiagSetAttrDigit( pDiag, intIndex, ID_EMVAL_141_TOTAL );
//
//	intIndex = m_intT2StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_PRVAL - 1;
//	CmnDiagSetAttrDigit( pDiag, intIndex, ID_PRVAL_141_TOTAL );
//
//	intIndex = m_intT2StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_BFVAL - 1;
//	CmnDiagSetAttrDigit( pDiag, intIndex, ID_BFVAL_141_TOTAL );
//
//	intIndex = m_intT2StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_PFVAL - 1;
//	CmnDiagSetAttrDigit( pDiag, intIndex, ID_PFVAL_141_TOTAL );
//
//	intIndex = m_intT2StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_ETVAL - 1;
//	CmnDiagSetAttrDigit( pDiag, intIndex, ID_ETVAL_141_TOTAL );
//
//	intIndex = m_intT2StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_RTVAL - 1;
//	CmnDiagSetAttrDigit( pDiag, intIndex, ID_RTVAL_141_TOTAL );
//
//	intIndex = m_intT2StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_VAL - 1;
//// midori 190301 del -->
//	//CmnDiagSetAttrDigit( pDiag, intIndex, ID_VAL_141_TOTAL );
//// midori 190301 del <--
//// midori 190301 add -->
//	if(bG_Kanso == TRUE)	{
//		CmnDiagSetAttrDigit(pDiag,intIndex,ID_VAL_141_TOTAL_K);
//	}
//	else	{
//		CmnDiagSetAttrDigit(pDiag,intIndex,ID_VAL_141_TOTAL);
//	}
//// midori 190301 add <--
// 改良No.23-0961 del <--
// 改良No.23-0961 add -->
	// 計１
	if(bG_Kanso == TRUE) {
		intIndex = m_intT1StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_EMVAL - 1;
		CmnDiagSetAttrDigit(pDiag, intIndex, ID_EMVAL_141_TOTAL_K);

		intIndex = m_intT1StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_PRVAL - 1;
		CmnDiagSetAttrDigit(pDiag, intIndex, ID_PRVAL_141_TOTAL_K);

		intIndex = m_intT1StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_BFVAL - 1;
		CmnDiagSetAttrDigit(pDiag, intIndex, ID_BFVAL_141_TOTAL_K);

		intIndex = m_intT1StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_PFVAL - 1;
		CmnDiagSetAttrDigit(pDiag, intIndex, ID_PFVAL_141_TOTAL_K);

		intIndex = m_intT1StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_ETVAL - 1;
		CmnDiagSetAttrDigit(pDiag, intIndex, ID_ETVAL_141_TOTAL_K);

		intIndex = m_intT1StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_RTVAL - 1;
		CmnDiagSetAttrDigit(pDiag, intIndex, ID_RTVAL_141_TOTAL_K);

		intIndex = m_intT1StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_VAL - 1;
		CmnDiagSetAttrDigit(pDiag, intIndex, ID_VAL_141_TOTAL_K);
	}
	else {
		intIndex = m_intT1StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_EMVAL - 1;
		CmnDiagSetAttrDigit(pDiag, intIndex, ID_EMVAL_141_TOTAL);

		intIndex = m_intT1StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_PRVAL - 1;
		CmnDiagSetAttrDigit(pDiag, intIndex, ID_PRVAL_141_TOTAL);

		intIndex = m_intT1StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_BFVAL - 1;
		CmnDiagSetAttrDigit(pDiag, intIndex, ID_BFVAL_141_TOTAL);

		intIndex = m_intT1StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_PFVAL - 1;
		CmnDiagSetAttrDigit(pDiag, intIndex, ID_PFVAL_141_TOTAL);

		intIndex = m_intT1StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_ETVAL - 1;
		CmnDiagSetAttrDigit(pDiag, intIndex, ID_ETVAL_141_TOTAL);

		intIndex = m_intT1StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_RTVAL - 1;
		CmnDiagSetAttrDigit(pDiag, intIndex, ID_RTVAL_141_TOTAL);

		intIndex = m_intT1StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_VAL - 1;
		CmnDiagSetAttrDigit(pDiag, intIndex, ID_VAL_141_TOTAL);
	}
	// 計２
	if(bG_Kanso == TRUE) {
		intIndex = m_intT2StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_EMVAL - 1;
		CmnDiagSetAttrDigit(pDiag, intIndex, ID_EMVAL_141_TOTAL_K);

		intIndex = m_intT2StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_PRVAL - 1;
		CmnDiagSetAttrDigit(pDiag, intIndex, ID_PRVAL_141_TOTAL_K);

		intIndex = m_intT2StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_BFVAL - 1;
		CmnDiagSetAttrDigit(pDiag, intIndex, ID_BFVAL_141_TOTAL_K);

		intIndex = m_intT2StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_PFVAL - 1;
		CmnDiagSetAttrDigit(pDiag, intIndex, ID_PFVAL_141_TOTAL_K);

		intIndex = m_intT2StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_ETVAL - 1;
		CmnDiagSetAttrDigit(pDiag, intIndex, ID_ETVAL_141_TOTAL_K);

		intIndex = m_intT2StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_RTVAL - 1;
		CmnDiagSetAttrDigit(pDiag, intIndex, ID_RTVAL_141_TOTAL_K);

		intIndex = m_intT2StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_VAL - 1;
		CmnDiagSetAttrDigit(pDiag, intIndex, ID_VAL_141_TOTAL_K);
	}
	else {
		intIndex = m_intT2StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_EMVAL - 1;
		CmnDiagSetAttrDigit(pDiag, intIndex, ID_EMVAL_141_TOTAL);

		intIndex = m_intT2StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_PRVAL - 1;
		CmnDiagSetAttrDigit(pDiag, intIndex, ID_PRVAL_141_TOTAL);

		intIndex = m_intT2StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_BFVAL - 1;
		CmnDiagSetAttrDigit(pDiag, intIndex, ID_BFVAL_141_TOTAL);

		intIndex = m_intT2StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_PFVAL - 1;
		CmnDiagSetAttrDigit(pDiag, intIndex, ID_PFVAL_141_TOTAL);

		intIndex = m_intT2StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_ETVAL - 1;
		CmnDiagSetAttrDigit(pDiag, intIndex, ID_ETVAL_141_TOTAL);

		intIndex = m_intT2StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_RTVAL - 1;
		CmnDiagSetAttrDigit(pDiag, intIndex, ID_RTVAL_141_TOTAL);

		intIndex = m_intT2StartIndexHide + m_uInfo.intCtlStartIndex + ID_COL_141_VAL - 1;
		CmnDiagSetAttrDigit(pDiag, intIndex, ID_VAL_141_TOTAL);
	}
// 改良No.23-0961 add <--
}

//********************************************************************************
//	帳表コントロールの特殊行初期化
//	IN	int		行番号
//		int		特殊行フラグ（FgFunc）
//	RET	なし
//********************************************************************************
void	CfrmUc141YakuinK::InitialControlSpecialRow( int intRow, int intFgFunc )
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
// 改良No.23-0961 del -->
//		CmnDiagSetAttrDigit( m_pDiag, CmnGetControlIndex( intRow , ID_COL_141_EMVAL ), ID_EMVAL_141_TOTAL );
//		CmnDiagSetAttrDigit( m_pDiag, CmnGetControlIndex( intRow , ID_COL_141_PRVAL ), ID_PRVAL_141_TOTAL );
//		CmnDiagSetAttrDigit( m_pDiag, CmnGetControlIndex( intRow , ID_COL_141_BFVAL ), ID_BFVAL_141_TOTAL );
//		CmnDiagSetAttrDigit( m_pDiag, CmnGetControlIndex( intRow , ID_COL_141_PFVAL ), ID_PFVAL_141_TOTAL );
//		CmnDiagSetAttrDigit( m_pDiag, CmnGetControlIndex( intRow , ID_COL_141_ETVAL ), ID_ETVAL_141_TOTAL );
//		CmnDiagSetAttrDigit( m_pDiag, CmnGetControlIndex( intRow , ID_COL_141_RTVAL ), ID_RTVAL_141_TOTAL );
//// midori 190301 del -->
//		//CmnDiagSetAttrDigit( m_pDiag, CmnGetControlIndex( intRow , ID_COL_141_VAL ), ID_VAL_141_TOTAL );
//// midori 190301 del <--
//// midori 190301 add -->
//		if(bG_Kanso == TRUE)	{
//			CmnDiagSetAttrDigit(m_pDiag,CmnGetControlIndex(intRow,ID_COL_141_VAL),ID_VAL_141_TOTAL_K);
//		}
//		else	{
//			CmnDiagSetAttrDigit(m_pDiag,CmnGetControlIndex(intRow,ID_COL_141_VAL),ID_VAL_141_TOTAL);
//		}
//// midori 190301 add <--
// 改良No.23-0961 del <--
// 改良No.23-0961 add -->
		if(bG_Kanso == TRUE) {
			CmnDiagSetAttrDigit(m_pDiag, CmnGetControlIndex(intRow, ID_COL_141_EMVAL), ID_EMVAL_141_TOTAL_K);
			CmnDiagSetAttrDigit(m_pDiag, CmnGetControlIndex(intRow, ID_COL_141_PRVAL), ID_PRVAL_141_TOTAL_K);
			CmnDiagSetAttrDigit(m_pDiag, CmnGetControlIndex(intRow, ID_COL_141_BFVAL), ID_BFVAL_141_TOTAL_K);
			CmnDiagSetAttrDigit(m_pDiag, CmnGetControlIndex(intRow, ID_COL_141_PFVAL), ID_PFVAL_141_TOTAL_K);
			CmnDiagSetAttrDigit(m_pDiag, CmnGetControlIndex(intRow, ID_COL_141_ETVAL), ID_ETVAL_141_TOTAL_K);
			CmnDiagSetAttrDigit(m_pDiag, CmnGetControlIndex(intRow, ID_COL_141_RTVAL), ID_RTVAL_141_TOTAL_K);
			CmnDiagSetAttrDigit(m_pDiag, CmnGetControlIndex(intRow, ID_COL_141_VAL),   ID_VAL_141_TOTAL_K);
		}
		else {
			CmnDiagSetAttrDigit(m_pDiag, CmnGetControlIndex(intRow, ID_COL_141_EMVAL), ID_EMVAL_141_TOTAL);
			CmnDiagSetAttrDigit(m_pDiag, CmnGetControlIndex(intRow, ID_COL_141_PRVAL), ID_PRVAL_141_TOTAL);
			CmnDiagSetAttrDigit(m_pDiag, CmnGetControlIndex(intRow, ID_COL_141_BFVAL), ID_BFVAL_141_TOTAL);
			CmnDiagSetAttrDigit(m_pDiag, CmnGetControlIndex(intRow, ID_COL_141_PFVAL), ID_PFVAL_141_TOTAL);
			CmnDiagSetAttrDigit(m_pDiag, CmnGetControlIndex(intRow, ID_COL_141_ETVAL), ID_ETVAL_141_TOTAL);
			CmnDiagSetAttrDigit(m_pDiag, CmnGetControlIndex(intRow, ID_COL_141_RTVAL), ID_RTVAL_141_TOTAL);
			CmnDiagSetAttrDigit(m_pDiag, CmnGetControlIndex(intRow, ID_COL_141_VAL),   ID_VAL_141_TOTAL);
		}
// 改良No.23-0961 add <--
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
		if ( intFgFunc != ID_FGFUNC_IKKATUAUTO ) {

			// 「常勤・非常勤の区別」はComboBoxのため、DropDown不可とする
			m_pDiag->ModifyItem( CmnGetControlIndex( intRow, ID_COL_141_FPTIME ), DIAG_MDFY_NODROPDOWN );

			// ComboBoxの選択項目クリア
			m_clsFunc.DiagInit( &diadata );
			diadata.text_combo = _T( "\r\n" );
			m_pDiag->SetData( CmnGetControlIndex( intRow, ID_COL_141_FPTIME ), (LPUNKNOWN)&diadata );
		}

		//	入力不可モードの設定
// midori 160612 cor -->
//		SetInputMode( intRow, DIAG_MDFY_NOINPUT );
// ---------------------
		if(m_DataKakutei == FALSE)	SetInputMode(intRow,DIAG_MDFY_NOINPUT);
		else						SetInputMode(intRow,DIAG_MDFY_READONLY);
// midori 160612 cor <--
		break;
	}
}

//**************************************************
//	帳表コントロールの自動計算セル初期化
//	【引数】	nRow		…	行番号
//				nFgFunc		…	特殊行フラグ
//				bFgInput	…	自動計算フラグ
//	【戻値】	なし
//**************************************************
void CfrmUc141YakuinK::virInitialControlFgInputCell( int nRow, int nFgFunc, BYTE bFgInput )
{	
	int		nRowFirst;	//	行開始位置
	int		nBackPos;	//	背景位置
	int		nDataPos;	//	データ位置

	//	位置計算
	nRowFirst = ( nRow - 1 ) * m_uInfo.intColMax;
	nBackPos = nRowFirst + 12;
	nDataPos = nRowFirst + ( m_uInfo.intColMax - ( m_uInfo.intColMaxData + 5 ) ) + ( ID_COL_141_VAL );

	//	自動計算対象セルの属性設定を行う
	CmnDiagSetAttrFgInputCell( m_pDiag, nBackPos, nDataPos, nFgFunc, bFgInput );
}

//********************************************************************************
//	帳表タイトル情報の表示
//********************************************************************************
void	CfrmUc141YakuinK::virUpdateControlTitle()
{
//	int		intMax;
	DIAGRAM_ATTRIBUTE diaatt;

	// 帳表ナンバーの背景色設定
	CmnDiagSetAttrColor( &m_NumberDiag, 0, &m_ViewColor );

	// 帳表ナンバー表示
	CmnDiagSetString( &m_NumberDiag, 0, m_uInfo.strTitleNumber, 1 );

	// 帳表タイトル表示
	CmnDiagSetString( &m_TitleDiag, 0, m_uInfo.strTitleName, 0 );

/*
	// 帳表タイトル編集可能/不可
	if (CmnCheckEnableTitleEdit() == TRUE) {
		intMax = ID_TITLE_INPMAX;
	} else {
		intMax = 0;
	}
	CmnDiagSetAttrInpmax( &m_TitleDiag, 0, intMax );
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
	// -------------------------------------------------------------
	// 項目名の変更（新様式：業績連動給与、旧様式：利益連動給与）
	// -------------------------------------------------------------
	if(bG_Kanso == TRUE)	{
		m_clsFunc.DiagSetString(&m_HeadDiag,10,ID_DIAGDATA_DISP,_T("業績連動\n給　　与"));
	}
	else	{
		m_clsFunc.DiagSetString(&m_HeadDiag,10,ID_DIAGDATA_DISP,_T("利益連動\n給　　与"));
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
void	CfrmUc141YakuinK::UpdateControlPage()
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
BOOL	CfrmUc141YakuinK::virUpdateControlTblData()
{
// midori 18-0887 add -->
	int					inpsw=0;
	int					nCtlIndex=0;
	CString				szNow=_T("");
	CString				szTmp1=_T("");
	CString				szTmp2=_T("");
	CString				szTmp3=_T("");
	CString				szTmp4=_T("");
	CString				szTmp5=_T("");
	CString				szTmp6=_T("");
// midori 18-0887 add <--

	// ダイアログ切替
	if( m_pDiag != NULL ){
		virSetInitialDiag();
	}

	CdbUc141Yakuin	dbRec( m_pDB );

// midori 154237,154238 del -->
//// midori 154190 add -->
//	if(((CUCHIWAKEApp*)AfxGetApp())->m_YsChg != 0)	{;
//		if(m_pDiag->GetPosition() != -1)	{
//			CmnDiagSetFocus(m_pDiag,1,1);
//		}
//	}
//// midori 154190 add <--
// midori 154237,154238 del <--

	m_pDiag->DeleteInput();		//	入力ボックス削除 

	// 頁情報の表示
	UpdateControlPage();

// midori 190301 add -->
	// プロパティを変更していないので(9桁→10桁)、ここで設定する
	// (データセット→桁数設定のながれだと、新様式起動時にオーバーフローで表示されるため)
	if(bG_Kanso == TRUE)	{
		CmnDiagSetAttrDigit(m_pDiagKei,ID_COL_141_KEI1_VAL,ID_VAL_141_TOTAL_K);
		CmnDiagSetAttrDigit(m_pDiagKei,ID_COL_141_KEI2_VAL,ID_VAL_141_TOTAL_K);
	}
	else	{
		CmnDiagSetAttrDigit(m_pDiagKei,ID_COL_141_KEI1_VAL,ID_VAL_141_TOTAL);
		CmnDiagSetAttrDigit(m_pDiagKei,ID_COL_141_KEI2_VAL,ID_VAL_141_TOTAL);
	}
// midori 190301 add <--

	// コントロールのクリア＋初期設定
	if ( InitialControlDataArea() != TRUE ){	// データ行
		return FALSE;
	}
// midori 160612 del -->
	//InitialControlTotalArea();	// 頁計/累計行
	//m_pDiag->Refresh();			// 更新
// midori 160612 del <--

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

// midori 18-0887 add -->
		//	自動計算結果と期末現在高が同じ場合でも背景色が緑の場合はここで自動計算フラグをＯＦＦにする
		if((dbRec.m_FgFunc == ID_FGFUNC_DATA) || (dbRec.m_FgFunc == ID_FGFUNC_IKKATUMANUAL))	{
			//	役員給与計の入力値取得
			nCtlIndex = CmnGetControlIndex(dbRec.m_NumRow,ID_COL_141_VAL);
			szNow = CmnDiagGetKingaku(m_pDiag,nCtlIndex);
			//	(1) 使用人職務分の入力値取得
			nCtlIndex = CmnGetControlIndex(dbRec.m_NumRow,ID_COL_141_EMVAL);
			szTmp1 = CmnDiagGetKingaku(m_pDiag,nCtlIndex);
			//	(2) 定期同額給与の入力値取得
			nCtlIndex = CmnGetControlIndex(dbRec.m_NumRow,ID_COL_141_PRVAL);
			szTmp2 = CmnDiagGetKingaku(m_pDiag,nCtlIndex);
			//	(3) 事前確定届出給与の入力値取得
			nCtlIndex = CmnGetControlIndex(dbRec.m_NumRow,ID_COL_141_BFVAL);
			szTmp3 = CmnDiagGetKingaku(m_pDiag,nCtlIndex);
			//	(4) 利益連動給与の入力値取得
			nCtlIndex = CmnGetControlIndex(dbRec.m_NumRow,ID_COL_141_PFVAL);
			szTmp4 = CmnDiagGetKingaku(m_pDiag,nCtlIndex);
			//	(5) その他の入力値取得
			nCtlIndex = CmnGetControlIndex(dbRec.m_NumRow,ID_COL_141_ETVAL);
			szTmp5 = CmnDiagGetKingaku(m_pDiag,nCtlIndex);
			//	(1)から(5)を足し算
// midori UC_0025 del -->
			//szTmp6 = GetAddVal(szTmp1,szTmp2,szTmp3,szTmp4,szTmp5);
// midori UC_0025 del <--
// midori UC_0025 add -->
			if(szTmp1 == "" && szTmp2 == "" && szTmp3 == "" && szTmp4 == "" && szTmp5 == "")	{
				szTmp6 = "";
			}
			else	{
				szTmp6 = GetAddVal(szTmp1,szTmp2,szTmp3,szTmp4,szTmp5);
			}
// midori UC_0025 add <--
			//	自動計算した結果と現在の入力値が違う？
			if(szNow != szTmp6 && szNow.IsEmpty() != TRUE)	{
				inpsw = ID_AUTOCALC_INPUT;
			}
			else	{
				inpsw = ID_AUTOCALC_AUTO;
			}
			if(inpsw != dbRec.m_FgInput)	{
				// 自動計算をフラグを書き換える
				dbRec.Edit();
				dbRec.m_FgInput = inpsw;
				dbRec.Update();
			}
		}
// midori 18-0887 add <--

		//	帳表コントロールの自動計算セル初期化
		virInitialControlFgInputCell( dbRec.m_NumRow, dbRec.m_FgFunc, dbRec.m_FgInput );

// No.200601 add -->
		// 電子申告の文字数を超えていれば、項目の文字色を変える
		CmnInitialControlFgStringCell( m_pDiag, dbRec.m_NumRow, ID_COL_141_NAMETANT, dbRec.m_FgFunc, UC_141_NAMETANT_LEN, dbRec.m_NameTant);
// No.200601 add <--

		dbRec.MoveNext();
	}

	// 閉じる
	dbRec.Fin();

// midori 160612 add -->
	InitialControlTotalArea();				// 頁計/累計行
// midori 160612 add <--

// midori 161108 add -->
	// 保管ﾃﾞｰﾀ件数の表示
	virUpdateControlTblData2();
// midori 161108 add <--

// No.200601 add -->
	// 項目タイトル(ヘッダ部)の再設定
	TitleColorChg();
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
BOOL CfrmUc141YakuinK::virUpdateControlTblData2()
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
//		IN		CdbUc141Yakuin*	「役員報酬」
//		RET		なし
//********************************************************************************
void	CfrmUc141YakuinK::UpdateControlRowData( CdbUc141Yakuin* prmDbRec ) {
	DIAGRAM_DATA	diadata;	// ICSDiag構造体
	int				intCell;	// 保存フィールド

	// 初期化
	m_clsFunc.DiagInit( &diadata );

	// 読込み対象セル取得
	intCell = CmnGetControlIndex( prmDbRec->m_NumRow, ID_COL_141_NAMEYAKU );
	
	// 担当業務
	diadata.data_edit = prmDbRec->m_NameYaku;
	m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	intCell++;

	// 役職名
	//	データ行？
	switch ( prmDbRec->m_FgFunc ) {
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
		diadata.data_edit = prmDbRec->m_NameTant;
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
		diadata.data_edit = prmDbRec->m_KeiStr;
		break;
	default:
		break;
	}
	m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	intCell++;

	// 氏名1
	//	データ行？
	switch( prmDbRec->m_FgFunc ) {
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

	// 代表者との関係
	diadata.data_edit = prmDbRec->m_Kankei;
	m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	intCell++;

	// 住所1
	diadata.data_edit = prmDbRec->m_AdAdd1;
	m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	intCell++;

	// 住所2
	diadata.data_edit = prmDbRec->m_AdAdd2;
	m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	intCell++;

	// 常勤・非常勤の区別
	diadata.data_combo = (short)prmDbRec->m_FpTime;
	m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	intCell++;

	// 使用人職務分
	if ( prmDbRec->m_EmVal != "" ) {
		CmnChangeFieldValData( &diadata.data_val[0], prmDbRec->m_EmVal );
		m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	}
	else {
		m_pDiag->DataClear( intCell, TRUE );
	}
	intCell++;

	// 定期同額給与
	if ( prmDbRec->m_PrVal != "" ) {
		CmnChangeFieldValData( &diadata.data_val[0], prmDbRec->m_PrVal );
		m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	}
	else {
		m_pDiag->DataClear( intCell, TRUE );
	}
	intCell++;

	// 事前確定届出給与
	if ( prmDbRec->m_BfVal != "" ) {
		CmnChangeFieldValData( &diadata.data_val[0], prmDbRec->m_BfVal );
		m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	}
	else {
		m_pDiag->DataClear( intCell, TRUE );
	}
	intCell++;

	// 利益連動給与
	if ( prmDbRec->m_PfVal != "" ) {
		CmnChangeFieldValData( &diadata.data_val[0], prmDbRec->m_PfVal );
		m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	}
	else {
		m_pDiag->DataClear( intCell, TRUE );
	}
	intCell++;

	// その他
	if ( prmDbRec->m_EtVal != "" ) {
		CmnChangeFieldValData( &diadata.data_val[0], prmDbRec->m_EtVal );
		m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	}
	else {
		m_pDiag->DataClear( intCell, TRUE );
	}
	intCell++;

	// 退職給与
	if ( prmDbRec->m_RtVal != "" ) {
		CmnChangeFieldValData( &diadata.data_val[0], prmDbRec->m_RtVal );
		m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	}
	else {
		m_pDiag->DataClear( intCell, TRUE );
	}
	intCell++;

	// 役員給与計
	if ( prmDbRec->m_Val != "" ) {
		CmnChangeFieldValData( &diadata.data_val[0], prmDbRec->m_Val );
		m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	}
	else {
		m_pDiag->DataClear( intCell, TRUE );
	}

}

//********************************************************************************
//	帳表データの１行表示（累計，頁計行）
//		IN		CdbUc141Yakuin*	「役員報酬」テーブル情報
//		RET		なし
//********************************************************************************
void	CfrmUc141YakuinK::UpdateControlRowTotal( CdbUc141Yakuin* prmDbRec ) {

	int		intTotalType;
	int		intRowName = 0;		// 名称表示行（0:非表示）
	int		intRowEmVal = 0;	// 使用人職務分表示行（0:非表示）
	int		intRowPrVal = 0;	// 定期同額給与表示行（0:非表示）
	int		intRowBfVal = 0;	// 事前確定届出給与表示行（0:非表示）
	int		intRowPfVal = 0;	// 利益連動給与表示行（0:非表示）
	int		intRowEtVal = 0;	// その他表示行（0:非表示）
	int		intRowRtVal = 0;	// 退職給与表示行（0:非表示）
	int		intRowVal = 0;		// 役員給与計表示行（0:非表示）

	// カレント頁の合計行の種別を取得
	intTotalType = CmnGetControlTotalType();

	switch ( intTotalType ) {
	case ID_OUTKEI_OFF:
		// カレント頁："なし"なら、合計行②に名称のみ表示
		if ( ( prmDbRec->m_FgFunc == ID_FGFUNC_PAGEKEI ) || ( prmDbRec->m_FgFunc == ID_FGFUNC_RUIKEI ) ) {
			intRowName = 2;
		}
		break;
	case ID_OUTKEI_PAGEKEI:
		// カレント頁："頁計"なら、合計行②に"頁計"を表示
		if ( prmDbRec->m_FgFunc == ID_FGFUNC_PAGEKEI ) {
			intRowName = 2;
			intRowEmVal = 2;
			intRowPrVal = 2;
			intRowBfVal = 2;
			intRowPfVal = 2;
			intRowEtVal = 2;
			intRowRtVal = 2;
			intRowVal= 2;
		}
		break;
	case ID_OUTKEI_RUIKEI:
		// カレント頁："累計"なら、合計行②に"累計"を表示
		if ( prmDbRec->m_FgFunc == ID_FGFUNC_RUIKEI ) {
			intRowName = 2;
			intRowEmVal = 2;
			intRowPrVal = 2;
			intRowBfVal = 2;
			intRowPfVal = 2;
			intRowEtVal = 2;
			intRowRtVal = 2;
			intRowVal= 2;
		}
		break;
	case ID_OUTKEI_BOTH:
		// カレント頁："頁計＋累計"なら、合計行①に"頁計"＋合計行②に"累計"を表示
		if ( prmDbRec->m_FgFunc == ID_FGFUNC_PAGEKEI ) {
			intRowName = 1;
			intRowEmVal = 1;
			intRowPrVal = 1;
			intRowBfVal = 1;
			intRowPfVal = 1;
			intRowEtVal = 1;
			intRowRtVal = 1;
			intRowVal= 1;
		}
		else if ( prmDbRec->m_FgFunc == ID_FGFUNC_RUIKEI ) {
			intRowName = 2;
			intRowEmVal = 2;
			intRowPrVal = 2;
			intRowBfVal = 2;
			intRowPfVal = 2;
			intRowEtVal = 2;
			intRowRtVal = 2;
			intRowVal= 2;
		}
		break;
	}

	// 頁計/累計の名称表示
	if ( intRowName > 0 ) {
		//CmnDiagSetString( m_pDiag, CmnGetControlTotalIndex( intRowName, ID_COL_141_NAMEYAKU ), prmDbRec->m_KeiStr, 0 );
		CmnDiagSetString( m_pDiagKei, CmnGetKeiDiagIndex( intRowName, ID_COL_141_KEI1_MEISYO ), prmDbRec->m_KeiStr, 1 );
	}
	// 頁計/累計の金額表示
	if ( intRowEmVal > 0 ) {
		//CmnDiagSetKingaku( m_pDiag, CmnGetControlTotalIndex( intRowName, ID_COL_141_EMVAL ), prmDbRec->m_EmVal );
		CmnDiagSetKingaku( m_pDiagKei, CmnGetKeiDiagIndex( intRowEmVal, ID_COL_141_KEI1_EMVAL ), prmDbRec->m_EmVal );
	}

	if ( intRowPrVal > 0 ) {
		//CmnDiagSetKingaku( m_pDiag, CmnGetControlTotalIndex( intRowName, ID_COL_141_PRVAL ), prmDbRec->m_PrVal );
		CmnDiagSetKingaku( m_pDiagKei, CmnGetKeiDiagIndex( intRowPrVal, ID_COL_141_KEI1_PRVAL ), prmDbRec->m_PrVal );
	}

	if ( intRowBfVal > 0 ) {
		//CmnDiagSetKingaku( m_pDiag, CmnGetControlTotalIndex( intRowName, ID_COL_141_BFVAL ), prmDbRec->m_BfVal );
		CmnDiagSetKingaku( m_pDiagKei, CmnGetKeiDiagIndex( intRowBfVal, ID_COL_141_KEI1_BFVAL ), prmDbRec->m_BfVal );
	}

	if ( intRowPfVal > 0 ) {
		//CmnDiagSetKingaku( m_pDiag, CmnGetControlTotalIndex( intRowName, ID_COL_141_PFVAL ), prmDbRec->m_PfVal );
		CmnDiagSetKingaku( m_pDiagKei, CmnGetKeiDiagIndex( intRowPfVal, ID_COL_141_KEI1_PFVAL ), prmDbRec->m_PfVal );
	}

	if ( intRowEtVal > 0 ) {
		//CmnDiagSetKingaku( m_pDiag, CmnGetControlTotalIndex( intRowName, ID_COL_141_ETVAL ), prmDbRec->m_EtVal );
		CmnDiagSetKingaku( m_pDiagKei, CmnGetKeiDiagIndex( intRowEtVal, ID_COL_141_KEI1_ETVAL ), prmDbRec->m_EtVal );
	}

	if ( intRowRtVal > 0 ) {
		//CmnDiagSetKingaku( m_pDiag, CmnGetControlTotalIndex( intRowName, ID_COL_141_RTVAL ), prmDbRec->m_RtVal );
		CmnDiagSetKingaku( m_pDiagKei, CmnGetKeiDiagIndex( intRowRtVal, ID_COL_141_KEI1_RTVAL ), prmDbRec->m_RtVal );
	}

	if ( intRowVal > 0 ) {
		//CmnDiagSetKingaku( m_pDiag, CmnGetControlTotalIndex( intRowName, ID_COL_141_VAL ), prmDbRec->m_Val );
		CmnDiagSetKingaku( m_pDiagKei, CmnGetKeiDiagIndex( intRowVal, ID_COL_141_KEI1_VAL ), prmDbRec->m_Val );
	}

}

//********************************************************************************
//	【F6:項目複写】１つ前のデータ表示処理
//		IN		CdbUc000Common*	共通テーブルクラス
//		RET		なし
//********************************************************************************
void	CfrmUc141YakuinK::virFncCellCopyProc( CdbUc000Common* rsData )
{
	CdbUc141Yakuin*	rs;
	int					intCol;	// 現在の列

	rs = (CdbUc141Yakuin*)rsData;

	// 列取得
	intCol = CmnGetControlCol();

	// 列によって処理を分岐
	switch( intCol ){

	// 役職名
	case ID_COL_141_NAMEYAKU:
		CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_NameYaku, 0 );
		break;
	// 担当業務
	case ID_COL_141_NAMETANT:
		CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_NameTant, 0 );
		break;
	// 氏名1
	case ID_COL_141_ADNAME1:
		CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, m_clsFunc.StrDocking( rs->m_AdName1,rs->m_AdName2), 0 );
		break;
	// 代表者との関係
	case ID_COL_141_KANKEI:
		CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_Kankei, 0 );
		break;
	// 住所1
	case ID_COL_141_ADADD1:
		CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_AdAdd1, 0 );
		break;
	// 住所2
	case ID_COL_141_ADADD2:
		CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_AdAdd2, 0 );
		break;
	// 常勤・非常勤の区別
	case ID_COL_141_FPTIME:
		CmnDiagSetCombo( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_FpTime );
		break;
	// 使用人職務分
	case ID_COL_141_EMVAL:
		CmnDiagSetKingaku( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_EmVal );
		break;
	// 定期同額給与
	case ID_COL_141_PRVAL:
		CmnDiagSetKingaku( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_PrVal );
		break;
	// 事前確定届出給与
	case ID_COL_141_BFVAL:
		CmnDiagSetKingaku( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_BfVal );
		break;
	// 利益連動給与
	case ID_COL_141_PFVAL:
		CmnDiagSetKingaku( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_PfVal );
		break;
	// その他
	case ID_COL_141_ETVAL:
		CmnDiagSetKingaku( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_EtVal );
		break;
	// 退職給与
	case ID_COL_141_RTVAL:
		CmnDiagSetKingaku( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_RtVal );
		break;
	// 役員給与計
	case ID_COL_141_VAL:
		CmnDiagSetKingaku( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_Val );
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
//				int*			住所１の分割するバイト数 ･･･ ICS様住所検索で使用
//				int*			住所２の分割するバイト数 ･･･ ICS様住所検索で使用
//				int*			住所１の列番号
//				int*			住所２の列番号
//		RET		int				参照型の種別
//									ID_DLGTYPE_BANK		：金融機関
//									ID_DLGTYPE_ADDRESS	：取引先
//									ID_DLGTYPE_KAMOKU	：科目
//									ID_DLGTYPE_YAKU_ZOKU：続柄or役職
//									ID_DLGTYPE_NONE		：参照不可
//********************************************************************************
int	CfrmUc141YakuinK::virFncReferenceGetType(int intCol, int* intGrSeq, long* lngItemSeq, int* intAdd1, int* intAdd2, int* intAdd1Col, int* intAdd2Col, REFERENCE_INFO* pudReference)
{
	int		intRefType = 0;

	switch ( intCol ) {

	// 住所1/2
	case ID_COL_141_ADADD1:
	case ID_COL_141_ADADD2:
		intRefType = ID_DLGTYPE_ICSADDSEARCH;
		*intAdd1 = 20 * 2;			// Byte指定
		*intAdd2 = 20 * 2;			// Byte指定
		*intAdd1Col = ID_COL_141_ADADD1;
		*intAdd2Col = ID_COL_141_ADADD2;
		break;
	// 役職名 or 代表者との関係
	case ID_COL_141_NAMEYAKU:
	case ID_COL_141_KANKEI:
		intRefType = ID_DLGTYPE_YAKU_ZOKU;
		*intAdd1 = MAX_KETA_141_NAMEYAKU * 2;			// Byte指定
		*intAdd2 = MAX_KETA_141_KANKEI * 2;			// Byte指定
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
//	【F9:参照】ICS住所検索ダイアログで選択された内容を表示
//		IN		CdlgAddressSearch*	参照ダイアログ情報
//		RET		なし
//********************************************************************************
void	CfrmUc141YakuinK::virFncReferenceSetICSData(CdlgAddressSearch* dlgAddd)
{
	int			intIndex;

	// 住所1
	intIndex = CmnGetControlIndex( m_uInfo.intCurRow, ID_COL_141_ADADD1 );
	CmnDiagSetString( m_pDiag, intIndex, dlgAddd->m_AdAdd1, 0 );

	// 住所2
	intIndex = CmnGetControlIndex( m_uInfo.intCurRow, ID_COL_141_ADADD2 );
	CmnDiagSetString( m_pDiag, intIndex, dlgAddd->m_AdAdd2, 0 );
}

//********************************************************************************
//	【F9:参照】ICS住所検索ダイアログで選択された内容を表示
//		IN		CdlgAddressSearch*	参照ダイアログ情報
//		RET		なし
//********************************************************************************
void	CfrmUc141YakuinK::virFncReferenceSetICSData(CdlgYakuZokuSelect* dlgZokuYaku, int type)
{
	int			intIndex;

	if( type == ID_KUBUN_SELECT_YAKUSYOKU ){
		// 役職名
		intIndex = CmnGetControlIndex( m_uInfo.intCurRow, ID_COL_141_NAMEYAKU );
	}
	else{
		// 代表者との関係
		intIndex = CmnGetControlIndex( m_uInfo.intCurRow, ID_COL_141_KANKEI );
	}
	
	CmnDiagSetString( m_pDiag, intIndex, dlgZokuYaku->m_RetStr, 0 );
}

//********************************************************************************
//	その他イベント処理
//********************************************************************************
BOOL	CfrmUc141YakuinK::PreTranslateMessage( MSG* pMsg )
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
//				if ((CmnGetControlCol() != ID_COL_141_FPTIME)){
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

				// Scroll＋フォーカス移動先を取得
				if (pMsg->wParam == VK_PRIOR) {
					// PageUpの場合：Scroll＋フォーカス移動（行－６）
					m_pDiag->ScrollPos(SB_PAGEUP , 0);
					intMoveRow = intMoveRow - 6;
					if (intMoveRow < 1) {
						blnMove = FALSE;					// 移動しない
					}
				}
				else {
					// PageDownの場合：Scroll＋フォーカス移動（行＋６）
					m_pDiag->ScrollPos(SB_PAGEDOWN, 0);
					intMoveRow = intMoveRow + 6;
// midori UC_0001 del -->
//					if (intMoveRow > m_uInfo.intRowMax) {
// midori UC_0001 del <--
// midori UC_0001 add -->
					// カレント頁の合計行の種別を取得
					intTotalType = CmnGetControlTotalType();
					// カレント頁で"頁計と累計"を表示する場合、対象行は"-1"となる
					// 頁計と累計
					if(intTotalType == ID_OUTKEI_BOTH)	nMaxDataCol = m_uInfo.intRowMaxData - 1;
					else								nMaxDataCol = m_uInfo.intRowMaxData;

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
// midori 160611 cor -->
				//}
// ---------------------
				if(CmnGetControlCol() == ID_COL_141_FPTIME)	{
					rv=1;
				}
// midori 160611 cor <--
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
				CmnDiagSetFocus(m_pDiag, m_uInfo.intRowMax, ID_COL_141_NAMEYAKU);
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
void	CfrmUc141YakuinK::FocusPagediag1()
{
//マウスクリックでキャレットが表示できないためコメントとする
//	// ボタンからTabキー移動した場合に必要
//	m_PageDiag.SetPosition( 1 );
}

// 頁コントロールのゲットフォーカス
void	CfrmUc141YakuinK::EditONPagediag1( short index )
{
// midori 151405 add -->
	// スクロールフラグをクリアする
	nG_MScroll = 0;
	m_Pagefocus = 1;
// midori 151405 add <--

	// 1行登録
//	int nCol = CmnGetControlCol();
//	if(( nCol != ID_COL_141_VAL ) || ( nCol != ID_COL_141_EMVAL ) || ( nCol != ID_COL_141_PRVAL ) || ( nCol != ID_COL_141_BFVAL ) ||
//		( nCol != ID_COL_141_PFVAL ) || ( nCol != ID_COL_141_ETVAL ) || ( nCol != ID_COL_141_RTVAL )){
			if (virRecBufSaveData(m_uInfo.intCurPage, m_uInfo.intCurRow) == FUNCTION_UPDATE) {
				m_PageDiag.SetPosition(1);
			}
//	}
}

// 頁コントロールのロストフォーカス
void	CfrmUc141YakuinK::EditOFFPagediag1( short index )
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
void	CfrmUc141YakuinK::TerminationPagediag1( short index, short nChar, short length, LPUNKNOWN data )
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

// 修正No.162446 del -->
//// 改良No.22-0503 add -->
//	int				page;
//	DIAGRAM_DATA	pgdata;
//
//	m_pPageDiag->GetData(index, (LPUNKNOWN)&pgdata);
//	page = _tstoi(pgdata.data_edit);
//
//	// ページ番号が変更されたかどうか
//	if(page > 0 && page <= m_uInfo.intMaxPage && m_uInfo.OldPage != page)	{
//		m_uInfo.intCurPage = page;
//
//		// データの表示
//		//m_ChangeFont = TRUE;
//		virUpdateControlTblData();
//	}
//// 改良No.22-0503 add <--
// 修正No.162446 del <--

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
			//CmnDiagSetFocus(m_pDiag, m_uInfo.intRowMax, ID_COL_141_NAMEYAKU);
			CmnDiagSetFocus(m_pDiag, nMaxDataCol, ID_COL_141_VAL);
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
void	CfrmUc141YakuinK::FocusTitlediag1()
{
//マウスクリックでキャレットが表示できないためコメントとする
//	// ボタンからTabキー移動した場合に必要
//	m_TitleDiag.SetPosition( 0 );
}

// タイトルコントロールのゲットフォーカス
void	CfrmUc141YakuinK::EditONTitlediag1( short index )
{
// midori 151405 add -->
	// スクロールフラグをクリアする
	nG_MScroll = 0;
	m_Titlefocus = 1;
// midori 151405 add <--
	// 1行登録
//	int nCol = CmnGetControlCol();
//	if(( nCol != ID_COL_141_VAL ) || ( nCol != ID_COL_141_EMVAL ) || ( nCol != ID_COL_141_PRVAL ) || ( nCol != ID_COL_141_BFVAL ) ||
//		( nCol != ID_COL_141_PFVAL ) || ( nCol != ID_COL_141_ETVAL ) || ( nCol != ID_COL_141_RTVAL )){
		if (virRecBufSaveData(m_uInfo.intCurPage, m_uInfo.intCurRow) == FUNCTION_UPDATE) {
			m_TitleDiag.SetPosition(0);
		}
//	}
}

// タイトルコントロールのロストフォーカス
void	CfrmUc141YakuinK::EditOFFTitlediag1 (short index )
{
// midori 151405 add -->
	m_Titlefocus = 0;
// midori 151405 add <--
	virUpdateTitle();
}

//	タイトル更新
void CfrmUc141YakuinK::virUpdateTitle()
{
	// 帳表タイトル取得してuc_inf_subを更新
	m_uInfo.strTitleName = m_clsFunc.DiagGetString( &m_TitleDiag, 0 );
	CmnUcInfSubSetTitleInfo( TRUE );
}

// タイトルコントロールのキーイベント
void	CfrmUc141YakuinK::TerminationTitlediag1( short index, short nChar, short length, LPUNKNOWN data )
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
//		if ( CmnCheckEnableTitleEdit() == TRUE ){
			m_TitleDiag.DataClear( 0 , TRUE );
//		}
		break;
	}
}

//********************************************************************************
//	「役員報酬」データコントロール処理（EditON，EditOFF，Terminationイベント）
//********************************************************************************

// 「役員報酬」データコントロールのゲットフォーカス
void	CfrmUc141YakuinK::EditONYoushikidiag1( short index )
{
	DIAGRAM_DATA	diadata;

// midori 151405 add -->
	// スクロールフラグをクリアする
	nG_MScroll = 0;
// midori 151405 add <--

	// フォーカスフラグON
	m_uInfo.intCurFocus = 1;

	// ポジション取得
	m_uInfo.intCurCtlIndex = m_pDiag->GetPosition();

	// 行・列取得
	CmnGetControlPosition( &m_uInfo.intCurRow , &m_uInfo.intCurCol, &m_uInfo.OldCol );

	switch (m_uInfo.intCurCol) {
	case ID_COL_141_FPTIME:	// 常勤・非常勤の区別
		// コンボボックスのオフセット値取得
		m_pDiag->GetData( m_uInfo.intCurCtlIndex , (LPUNKNOWN)&diadata );
		m_uInfo.OldCombo = diadata.data_combo;
		break;
	default:
		// 現在の入力値を取得
		m_uInfo.OldstrDiag = m_clsFunc.DiagGetString( m_pDiag, m_uInfo.intCurCtlIndex );
		break;
	}


	// 行移動確認
	//int nCol = CmnGetControlCol();
	if ( /*( m_uInfo.OldRow != CmnGetControlRow() ) &&*/ ( m_uInfo.OldRow > 0 ) ) {
		if ( m_uInfo.OldPage == m_uInfo.intCurPage && m_uInfo.OldPage > 0 ) {
//			if(( nCol != ID_COL_141_VAL ) || ( nCol != ID_COL_141_EMVAL ) || ( nCol != ID_COL_141_PRVAL ) || ( nCol != ID_COL_141_BFVAL ) ||
//				( nCol != ID_COL_141_PFVAL ) || ( nCol != ID_COL_141_ETVAL ) || ( nCol != ID_COL_141_RTVAL )){
					// カレント行登録＋再表示
					if ( virRecBufSaveData(m_uInfo.intCurPage , m_uInfo.OldRow ) == FUNCTION_UPDATE ) {
						// レコード更新があったとき、再表示する
						// カーソル移動処理
						CmnDiagSetFocus( m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol );
					}
//			}
		}
	}

	// ボタン操作
	CmnChangeButtonEnable(1);
}

// データコントロールのロストフォーカス
void	CfrmUc141YakuinK::EditOFFYoushikidiag1( short index )
{
	if( m_EndView != FALSE )	return;
	virInputDecisionEditOFF();
}

//	入力確定
void CfrmUc141YakuinK::virInputDecisionEditOFF()
{
	// フォーカスフラグOFF
	m_uInfo.intCurFocus = 0;

	//	期末現在高の値が範囲を超えてない？
	//	（※期末現在高の値が範囲を超えて"************"で表示されている場合、入力を確定してしまうと"0"になってしまう）
//	if ( CheckOutsideValue( m_pDiag, m_pDiag->GetPosition(), m_uInfo.intCurCol ) == FALSE ){
		//	入力項目を強制的に確定
		CmnDiagSetEnterReDraw( m_pDiag, m_pDiag->GetPosition() );
//	}
	
// 当帳表では、一括集計できないため、処理が不要
//	// 自動一括集計行なら、変更できてしまうComboBoxの値を元に戻す
//	if (m_ReadData[m_uInfo.intCurRow].m_FgFunc == ID_FGFUNC_IKKATUAUTO) {
//		switch (m_uInfo.intCurCol) {
//		case ID_COL_141_FPTIME:
//			CmnDiagSetCombo( m_pDiag, CmnGetControlIndex(m_uInfo.intCurRow, ID_COL_141_FPTIME ), m_uInfo.OldCombo);
//			break;
//		}
//	}

	//	自動計算フラグ確認
	CheckFgInput();
	//	自動計算結果を設定
	SetAutoCalc();

	// 行取得
	if ( m_uInfo.intCurCtlIndex < m_uInfo.intT1StartIndex ) {
		m_uInfo.OldRow = CmnGetControlRow();
	}
	else{
		m_uInfo.OldRow = -1;
	}

	// 金額欄の入力が確定した時点で反映
//	int nCol = CmnGetControlCol();
//	if (  m_uInfo.OldRow > 0 ){
//		//if(( nCol == ID_COL_141_VAL ) || ( nCol == ID_COL_141_EMVAL ) || ( nCol == ID_COL_141_PRVAL ) || 
//		//	( nCol == ID_COL_141_BFVAL ) || ( nCol == ID_COL_141_PFVAL ) || ( nCol == ID_COL_141_ETVAL ) || ( nCol == ID_COL_141_RTVAL )){
//			//if ( m_uInfo.OldPage == m_uInfo.intCurPage ) {
//				// カレント行登録＋再表示
//				virRecBufSaveData(m_uInfo.intCurPage , m_uInfo.OldRow );
//			//}
//		//}
//	}

	// ページ番号取得
	m_uInfo.OldPage = m_uInfo.intCurPage;

	// ボタン操作
	CmnChangeButtonEnable(0);
}

// 預貯金データコントロールのキーイベント
void	CfrmUc141YakuinK::TerminationYoushikidiag1( short index, short nChar, short length, LPUNKNOWN data )
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
			case ID_COL_141_FPTIME:	// 常勤・非常勤の区別
				break;
			default:
				m_pDiag->DataClear( m_uInfo.intCurCtlIndex, TRUE );

				//	役員給与計の列？
				if ( intCol == ID_COL_141_VAL ){
					//	一個前の文字列もクリア（この後、EditOFFイベント発生のため）
					m_uInfo.OldstrDiag = _T("");
					// 行・列取得
					CmnGetControlPosition( &m_uInfo.intCurRow , &m_uInfo.intCurCol, &m_uInfo.OldCol );
					//	自動計算フラグを自動計算に設定
					m_ReadData[m_uInfo.intCurRow].m_FgInput = ID_AUTOCALC_AUTO;
					//	背景色を変更
					virInitialControlFgInputCell( m_uInfo.intCurRow, m_ReadData[m_uInfo.intCurRow].m_FgFunc, m_ReadData[m_uInfo.intCurRow].m_FgInput );
				}
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
	//		case ID_COL_141_FPTIME:	// 常勤・非常勤の区別
	//			m_pDiag->ComboDropDown( m_uInfo.intCurCtlIndex, FALSE );
	//			CmnDiagSetCombo( m_pDiag, m_uInfo.intCurCtlIndex, m_uInfo.OldCombo );
	//			break;
	//		default:
	//			break;
	//		}
	//	}

	//	// 期末現在高で値が入力された場合の金額確定処理
	//	//if(( intCol == ID_COL_141_EMVAL ) || ( intCol == ID_COL_141_PRVAL ) || ( intCol == ID_COL_141_BFVAL ) || 
	//	//	( intCol == ID_COL_141_PFVAL ) || ( intCol == ID_COL_141_ETVAL ) || ( intCol == ID_COL_141_RTVAL ) || ( intCol == ID_COL_141_VAL )){		
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
void CfrmUc141YakuinK::VScrollYoushikidiag1(short mode)
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
int	CfrmUc141YakuinK::GetFocusPosition( int nChar )
{
	int		intRow;				// カレント行番号
	int		intCol;				// カレント列番号
	int		intVector[4];		// 矢印キーの移動先（0:↑，1:→，2:↓，3:←）
	int		intNext = 0;		// フォーカス移動先
	int		intRowChange = 0;	// 行をまたぐ場合のインデックス増減数

	// カレント行＋列番号を取得
	CmnGetControlPosition( &intRow, &intCol );

	// 行をまたぐ場合のインデックス増減数（摘要→次の行の金融機関名）
	intRowChange = m_uInfo.intCtlStartIndex + 1 + 4;	// ここの値は検討が必要

	// 「種類」はComboBoxのため、「↑，↓」キーではカーソルを移動しない
	if ( intCol == ID_COL_141_FPTIME ) {
		if ( ( nChar == VK_UP ) || ( nChar == VK_DOWN ) ) {
			// キー操作を無視する
			return -3;
		}
	}

	//----------------------------------------------------------------
	//	矢印キーによる移動先を取得（合計欄は次のステップで対応）
	//	基本的にReturn，Tab，Shift+Tabは、←，→キーと同じなので省略
	//----------------------------------------------------------------
	if ( intCol == ID_COL_141_NAMEYAKU ) {
		// カレント列が「役職名」の場合
		intVector[0] = m_uInfo.intCurCtlIndex - m_uInfo.intColMax + 1;	// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + 1;						// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - intRowChange;			// ←
	}
	else if ( intCol == ID_COL_141_NAMETANT ) {
		// カレント列が「担当業務」の場合
		intVector[0] = m_uInfo.intCurCtlIndex - 1;						// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + m_uInfo.intColMax - 1;	// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;						// ←
	}
	else if ( intCol == ID_COL_141_ADNAME1 ) {
		// カレント列が「氏名１」の場合
		intVector[0] = m_uInfo.intCurCtlIndex - m_uInfo.intColMax + 3;	// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + 2;						// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;						// ←
	}
	else if ( intCol == ID_COL_141_ADADD1 ) {
		// カレント列が「住所１」の場合
		intVector[0] = m_uInfo.intCurCtlIndex - 2;						// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + 1;						// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;						// ←
	}
	else if ( intCol == ID_COL_141_ADADD2 ) {
		// カレント列が「住所２」の場合
		intVector[0] = m_uInfo.intCurCtlIndex - 1;						// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + m_uInfo.intColMax - 3;	// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;						// ←
	}
	else if ( intCol == ID_COL_141_KANKEI ) {
		// カレント列が「代表者との関係」の場合
		intVector[0] = m_uInfo.intCurCtlIndex - m_uInfo.intColMax + 2;	// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + 1;						// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;						// ←
	}
	else if ( intCol == ID_COL_141_FPTIME ) {
		// カレント列が「常勤・非常勤の別」の場合
		intVector[0] = m_uInfo.intCurCtlIndex;							// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + m_uInfo.intColMax;		// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;						// ←
	}
	else if ( intCol == ID_COL_141_RTVAL ) {
		// カレント列が「退職給与」の場合
		intVector[0] = m_uInfo.intCurCtlIndex - m_uInfo.intColMax + 1;	// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + 1;						// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;						// ←
	}
	else if ( intCol == ID_COL_141_VAL ) {
		// カレント列が「役員給与計」の場合
		intVector[0] = m_uInfo.intCurCtlIndex - 1;						// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + intRowChange;			// →
		intVector[2] = m_uInfo.intCurCtlIndex + m_uInfo.intColMax - 1;	// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;						// ←
	}
	else {
		// カレント列が上記以外の場合
		intVector[0] = m_uInfo.intCurCtlIndex - m_uInfo.intColMax;		// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + m_uInfo.intColMax;		// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;						// ←
	}

	//----------------------------------------------------------------
	//	押下キーから移動先を取得（合計欄は次のステップで対応）
	//	基本的にReturn，Tab，Shift+Tabは、←，→キーと同じ
	//----------------------------------------------------------------
	switch ( nChar ) {
	case VK_UP:
		intNext = intVector[0];		// 「↑」キー押下時
		break;
	case VK_RIGHT:
		intNext = intVector[1];		// 「→」キー押下時
		break;
	case VK_DOWN:
		intNext = intVector[2];		// 「↓」キー押下時
		break;
	case VK_LEFT:
		intNext = intVector[3];		// 「←」キー押下時
		break;
	case VK_RETURN:	// Enterキーでカーソルを下に移動するモード対応
		if(m_ReturnMoveDown){
			if(intCol == ID_COL_141_ADNAME1)	intNext = intVector[1];			// 「カレント列が「氏名１」の場合「→」キーと同じ
			else								intNext = intVector[2];			//	Enterキーでカーソルを下に移動するチェック時は「↓」キーと同じ
		}
		else									intNext = intVector[1];			// 「Return」キー押下時：「→」キーと同じ
		break;
	case VK_TAB:
		if ( CmnCheckShiftKey() == FALSE ) {
			intNext = intVector[1];		// 「Tab」キー押下時：「→」キーと同じ
		}
		else {
			// 例外処理：先頭項目で"Shift+Tab"は別コントロールへ移動
			// それ以外は「←」キーと同じ
			if ( ( intRow <= 1 ) && ( intCol == ID_COL_141_NAMEYAKU ) ) {
				return -1;
			}
			intNext = intVector[3];		// 「Shift+Tab」キー押下時：「→」キーと同じ
		}
		break;
	}

	if (intNext >= 0) {
		// 当処理で合計行を考慮したカーソル移動先を確定させる
		intNext = GetTotalRowIndex( nChar, m_uInfo.intCurCtlIndex, intNext );
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
int	CfrmUc141YakuinK::GetTotalRowIndex( int nCharOrg, int intIndex, int intNext )
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
		
		//	データ行を超えた？
		if ( nNextRow > nBorder[0] ){
			//	計行に移動
			//nRet = CmnGetControlIndex( ( nBorder[1] + 1 ), ID_COL_141_NAMEYAKU );
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
			nRet = CmnGetControlIndex( nBorder[0], ID_COL_141_NAMETANT );
			break;
		case VK_LEFT:		// ←/Shift+Tab：23行目のデータ行の摘要項目へ移動
			nRet = CmnGetControlIndex( nBorder[0], ID_COL_141_VAL );
			break;
		default:			// →/↓/Return/Tab：25行目の合計行②へ移動
			
			//	累計なし？
			if ( nBorder[2] == 0 ){
				// 例外処理：最終項目で"Tab"は別コントロールへ移動
				// それ以外は「→」キーと同じ
				if (nCharOrg == VK_TAB) {
					return -2;
				}
			}
			//	累計あり
			else{
				nRet = CmnGetControlIndex( ( nBorder[2] + 1 ), ID_COL_141_NAMEYAKU );
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
			nRet = CmnGetControlIndex( ( nBorder[1] + 1 ), ID_COL_141_NAMEYAKU );
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

/**********************************************************************
	RecBufSetData(CdbUc141Yakuin* prmDbRec)
		ローカルのレコードバッファに、1行分のデータを格納する

	引数
		CdbUc141Yakuin*	prmDbRec	レコードセット(データがあること）
	戻値
		int		成功/失敗
				FUNCTION_OK
***********************************************************************/
int	CfrmUc141YakuinK::RecBufSetData( CdbUc141Yakuin* prmDbRec )
{
	// 1ページ分のレコードバッファの該当行にデータを格納する
	int	trgRow = prmDbRec->m_NumRow;
	m_ReadData[trgRow].m_Seq = prmDbRec->m_Seq;	// シーケンス番号
	m_ReadData[trgRow].m_NumPage = prmDbRec->m_NumPage;	// 頁番号
	m_ReadData[trgRow].m_NumRow = prmDbRec->m_NumRow;	// 行番号
	m_ReadData[trgRow].m_FgFunc = prmDbRec->m_FgFunc;	// 特殊行フラグ
	m_ReadData[trgRow].m_FgShow = prmDbRec->m_FgShow;	// 表示フラグ
	m_ReadData[trgRow].m_KeiStr = prmDbRec->m_KeiStr;	// 計
	m_ReadData[trgRow].m_NumGroup = prmDbRec->m_NumGroup;	// グループ番号
	m_ReadData[trgRow].m_NameYaku = prmDbRec->m_NameYaku;	// 役職名
	m_ReadData[trgRow].m_NameTant = prmDbRec->m_NameTant;	// 担当業務
	m_ReadData[trgRow].m_AdSeq = prmDbRec->m_AdSeq;	// 氏名/住所：取引先（シーケンス番号）
	m_ReadData[trgRow].m_AdOrder = prmDbRec->m_AdOrder;	// 氏名/住所：取引先（並び順）
	m_ReadData[trgRow].m_AdName1 = prmDbRec->m_AdName1;	// 氏名（名称：上段）
	m_ReadData[trgRow].m_AdName2 = prmDbRec->m_AdName2;	// 氏名（名称：下段）
	m_ReadData[trgRow].m_AdAdd1 = prmDbRec->m_AdAdd1;	// 住所（所在地：上段）
	m_ReadData[trgRow].m_AdAdd2 = prmDbRec->m_AdAdd2;	// 住所（所在地：下段）
	m_ReadData[trgRow].m_Kankei = prmDbRec->m_Kankei;	// 代表者との関係
	m_ReadData[trgRow].m_FpTime = prmDbRec->m_FpTime;	// 常勤・非常勤の別
	m_ReadData[trgRow].m_EmVal = prmDbRec->m_EmVal;		// 使用人職務分			// 2006.10.23：新会社法対応
	m_ReadData[trgRow].m_PrVal = prmDbRec->m_PrVal;		// 定期同額給与			// 2006.10.23：新会社法対応
	m_ReadData[trgRow].m_BfVal = prmDbRec->m_BfVal;		// 事前確定届出給与		// 2006.10.23：新会社法対応
	m_ReadData[trgRow].m_PfVal = prmDbRec->m_PfVal;		// 利益連動給与			// 2006.10.23：新会社法対応
	m_ReadData[trgRow].m_EtVal = prmDbRec->m_EtVal;		// その他				// 2006.10.23：新会社法対応
	m_ReadData[trgRow].m_RtVal = prmDbRec->m_RtVal;		// 退職給与				// 2006.10.23：新会社法対応
	m_ReadData[trgRow].m_Val = prmDbRec->m_Val;			// 役員給与計			// 2006.10.23：新会社法対応

	m_ReadData[trgRow].m_FgInput = prmDbRec->m_FgInput;	// ｢役員給与計｣ チェックフラグ

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
int	CfrmUc141YakuinK::RecBufClearAllData()
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
		P_REC_UC_141_YAKUIN inRecData レコードへのポインタ
	戻値
		int		成功/失敗
				FUNCTION_OK
***********************************************************************/
int	CfrmUc141YakuinK::RecBufClearData( P_REC_UC_141_YAKUIN inRecData )
{
	inRecData->m_Seq = 0;						// シーケンス番号
	inRecData->m_NumPage = 0;					// 頁番号
	inRecData->m_NumRow = 0;					// 行番号
	inRecData->m_FgFunc = 0;					// 特殊行フラグ
	inRecData->m_FgShow = 0;					// 表示フラグ
	inRecData->m_KeiStr = _T("");				// 計
	inRecData->m_NumGroup = 0;					// グループ番号
	inRecData->m_NameYaku = _T("");				// 役職名
	inRecData->m_NameTant = _T("");				// 担当業務
	inRecData->m_AdSeq = 0;						// 氏名/住所：取引先（シーケンス番号）
	inRecData->m_AdOrder = 0;					// 氏名/住所：取引先（並び順）
	inRecData->m_AdName1 = _T("");				// 氏名（名称：上段）
	inRecData->m_AdName2 = _T("");				// 氏名（名称：下段）
	inRecData->m_AdAdd1 = _T("");				// 住所（所在地：上段）
	inRecData->m_AdAdd2 = _T("");				// 住所（所在地：下段）
	inRecData->m_Kankei = _T("");				// 代表者との関係
	inRecData->m_FpTime = 0;					// 常勤・非常勤の別
	inRecData->m_EmVal = _T("");				// 使用人職務分				// 2006.10.23：新会社法対応
	inRecData->m_PrVal = _T("");				// 定期同額給与				// 2006.10.23：新会社法対応
	inRecData->m_BfVal = _T("");				// 事前確定届出給与			// 2006.10.23：新会社法対応
	inRecData->m_PfVal = _T("");				// 利益連動給与				// 2006.10.23：新会社法対応
	inRecData->m_EtVal = _T("");				// その他					// 2006.10.23：新会社法対応
	inRecData->m_RtVal = _T("");				// 退職給与					// 2006.10.23：新会社法対応
	inRecData->m_Val = _T("");					// 役員給与計				// 2006.10.23：新会社法対応
	inRecData->m_FgInput = ID_AUTOCALC_AUTO;	// ｢役員給与計｣ チェックフラグ
	inRecData->m_ShowKeiZero = 0;				//０円計表示フラグ

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
int	CfrmUc141YakuinK::virRecBufSaveData( int inPage, int inRow )
{
	REC_UC_141_YAKUIN	tmpSaveData;	// 画面データ1レコード格納用

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

	// 役職名
	intIndex = CmnGetControlIndex( inRow, ID_COL_141_NAMEYAKU );
	tmpSaveData.m_NameYaku = m_clsFunc.DiagGetString( m_pDiag, intIndex );

	// 担当業務
	intIndex = CmnGetControlIndex( inRow, ID_COL_141_NAMETANT );
	tmpSaveData.m_NameTant = m_clsFunc.DiagGetString( m_pDiag, intIndex );

	// 氏名1
	CStringArray strArray;
	intIndex = CmnGetControlIndex( inRow, ID_COL_141_ADNAME1 );
	m_clsFunc.StrDivision( m_clsFunc.DiagGetString( m_pDiag, intIndex ), &strArray, 2 );
	tmpSaveData.m_AdName1 = strArray.GetAt( 0 );
	tmpSaveData.m_AdName2 = strArray.GetAt( 1 );
	// 2行目が空行なら1行目のみを再表示
	if ( tmpSaveData.m_AdName2.IsEmpty() ){
		if(m_uInfo.intCurCol == ID_COL_141_ADNAME1 && m_uInfo.intCurRow != m_uInfo.OldRow){	// 各カラムごとにvirRecBufSaveData()がコールされるようになり、かつタイミングがEditOffからEditOnに移ったことによりCmnDiagSetString()でカレットが末尾から先頭に移ってしまう為、制御（修正依頼No150489）
			CmnDiagSetString( m_pDiag, intIndex, tmpSaveData.m_AdName1, 0 );
		}
	}

	// 代表者との関係
	intIndex = CmnGetControlIndex( inRow, ID_COL_141_KANKEI );
	tmpSaveData.m_Kankei = m_clsFunc.DiagGetString( m_pDiag, intIndex );

	// 住所1
	intIndex = CmnGetControlIndex( inRow, ID_COL_141_ADADD1 );
	tmpSaveData.m_AdAdd1 = m_clsFunc.DiagGetString( m_pDiag, intIndex );

	// 住所2
	intIndex = CmnGetControlIndex( inRow, ID_COL_141_ADADD2 );
	tmpSaveData.m_AdAdd2 = m_clsFunc.DiagGetString( m_pDiag, intIndex );

	// 常勤・非常勤の区別
	intIndex = CmnGetControlIndex( inRow, ID_COL_141_FPTIME );
	m_pDiag->GetData( intIndex , (LPUNKNOWN)&diadata );
	tmpSaveData.m_FpTime = (BYTE)diadata.data_combo;

	// 使用人職務分
	intIndex = CmnGetControlIndex( inRow, ID_COL_141_EMVAL );
	tmpSaveData.m_EmVal = CmnDiagGetKingaku( m_pDiag, intIndex );

	// 定期同額給与
	intIndex = CmnGetControlIndex( inRow, ID_COL_141_PRVAL );
	tmpSaveData.m_PrVal = CmnDiagGetKingaku( m_pDiag, intIndex );

	// 事前確定届出給与
	intIndex = CmnGetControlIndex( inRow, ID_COL_141_BFVAL );
	tmpSaveData.m_BfVal = CmnDiagGetKingaku( m_pDiag, intIndex );

	// 利益連動給与
	intIndex = CmnGetControlIndex( inRow, ID_COL_141_PFVAL );
	tmpSaveData.m_PfVal = CmnDiagGetKingaku( m_pDiag, intIndex );

	// その他
	intIndex = CmnGetControlIndex( inRow, ID_COL_141_ETVAL );
	tmpSaveData.m_EtVal = CmnDiagGetKingaku( m_pDiag, intIndex );

	// 退職給与
	intIndex = CmnGetControlIndex( inRow, ID_COL_141_RTVAL );
	tmpSaveData.m_RtVal = CmnDiagGetKingaku( m_pDiag, intIndex );

	// 役員給与計
	tmpSaveData.m_Val = GetAutoCalc( inRow , tmpSaveData.m_EmVal , tmpSaveData.m_PrVal , tmpSaveData.m_BfVal ,
									 tmpSaveData.m_PfVal , tmpSaveData.m_EtVal );

	tmpSaveData.m_FgInput = m_ReadData[inRow].m_FgInput;

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
int	CfrmUc141YakuinK::virRecBufSaveDataForPasteInsert( int inPage, int inRow )
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
int	CfrmUc141YakuinK::virRecBufSaveDataForPaste( int inPage, int inRow )
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
			REC_UC_141_YAKUIN
					inSaveData	登録するデータレコードへのポインタ
	戻値
			int					成功(更新あり)/成功(更新なし)/失敗
								FUNCTION_UPDATE	: 成功(更新あり)
								FUNCTION_OK		: 成功(更新なし)
								FUNCTION_NG		: 失敗
***********************************************************************/
int	CfrmUc141YakuinK::RecBufSaveDataSub( 
							int inPage,
							int inRow,
							P_REC_UC_141_YAKUIN inSaveData )
{
	REC_UC_141_YAKUIN	udTemp;

	//	画面上のデータ格納用初期化
	RecBufClearData( &udTemp );

//	// 変更があったかのフラグ（変更あり:TRUE、なし:FALSE)
	BOOL	ufgUpData	= FALSE;	// 変更確認フラグ(全体用データ行/空行)
	BOOL	ufgUpToku	= FALSE;	// 変更確認フラグ(全体用特殊行)
	
	BOOL	ufgYaku		= FALSE;
	BOOL	ufgTant		= FALSE;
	BOOL	ufgName1	= FALSE;
	BOOL	ufgName2	= FALSE;
	BOOL	ufgKankei	= FALSE;
	BOOL	ufgAdd1		= FALSE;
	BOOL	ufgAdd2		= FALSE;
	BOOL	ufgTime		= FALSE;
	BOOL	ufgEmVal	= FALSE;
	BOOL	ufgPrVal	= FALSE;
	BOOL	ufgBfVal	= FALSE;
	BOOL	ufgPfVal	= FALSE;
	BOOL	ufgEtVal	= FALSE;
	BOOL	ufgRtVal	= FALSE;
	BOOL	ufgVal		= FALSE;

	BOOL	ufgFunc		= FALSE;	// 変更確認フラグ(摘要)

	// レコード更新したことを示すフラグ
	BOOL	ufgRsUpdate = FALSE;
	// テーブルアクセスクラス
	CdbUc141Yakuin	rs( m_pDB );
	// 作業用変数へ 登録対象のフィールド値を代入
	udTemp = *inSaveData;
	// データ行か、空行かのチェック
	udTemp.m_FgFunc = m_ReadData[inRow].m_FgFunc;

	if ( udTemp.m_FgFunc <= ID_FGFUNC_DATA ) {	// データ行 または、空行なら
		// コンポーネントをチェックして、データがあるかを調べる
		// (仮変数に入れた値でチェックしてもいいのかも？)

		if ( ( udTemp.m_NameYaku == "" )	// 画面上のデータ格納用(役職名)
		  && ( udTemp.m_NameTant == "" )	// 画面上のデータ格納用(担当業務)
		  && ( udTemp.m_AdName1 == "" )		// 画面上のデータ格納用(氏名1)
		  && ( udTemp.m_AdName2 == "" )		// 画面上のデータ格納用(氏名2)
		  && ( udTemp.m_Kankei == "" )		// 画面上のデータ格納用(代表者との関係)
		  && ( udTemp.m_AdAdd1 == "" )		// 画面上のデータ格納用(住所1)
		  && ( udTemp.m_AdAdd2 == "" )		// 画面上のデータ格納用(住所2)
		  && ( udTemp.m_FpTime == 0 )		// 画面上のデータ格納用(常勤・非常勤の区別)
		  && ( udTemp.m_EmVal == "" )		// 画面上のデータ格納用(使用人職務分)
		  && ( udTemp.m_PrVal == "" )		// 画面上のデータ格納用(定期同額給与)
		  && ( udTemp.m_BfVal == "" )		// 画面上のデータ格納用(事前確定届出給与)
		  && ( udTemp.m_PfVal == "" )		// 画面上のデータ格納用(利益連動給与)
		  && ( udTemp.m_EtVal == "" )		// 画面上のデータ格納用(その他)
		  && ( udTemp.m_RtVal == "" )		// 画面上のデータ格納用(退職給与)
		  && ( udTemp.m_Val == "" )) {	// 画面上のデータ格納用(役員給与計)
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
	//(1) 役職名
	if ( udTemp.m_NameYaku != m_ReadData[inRow].m_NameYaku ) {
		ufgUpData = TRUE;		// 全体(データ行/空行)
		ufgYaku	= TRUE;
	}

	//(2) 担当業務
	if ( udTemp.m_NameTant != m_ReadData[inRow].m_NameTant ) {
		ufgUpData = TRUE;		// 全体(データ行/空行)
		ufgUpToku = TRUE;		// 全体(特殊行)
		ufgTant = TRUE;
	}

	//(3) 氏名1
	if ( udTemp.m_AdName1 != m_ReadData[inRow].m_AdName1 ) {
		ufgUpData = TRUE;		// 全体(データ行/空行)
		ufgName1 = TRUE;
	}

	//(4) 氏名2
	if ( udTemp.m_AdName2 != m_ReadData[inRow].m_AdName2 ) {
		ufgUpData = TRUE;		// 全体(データ行/空行)
		ufgName2 = TRUE;
	}

	//(5) 代表者との関係
	if ( udTemp.m_Kankei != m_ReadData[inRow].m_Kankei ) {
		ufgUpData = TRUE;		// 全体(データ行/空行)
		ufgKankei = TRUE;
	}

	//(6) 住所1
	if ( udTemp.m_AdAdd1 != m_ReadData[inRow].m_AdAdd1 ) {
		ufgUpData = TRUE;		// 全体(データ行/空行)
		ufgAdd1 = TRUE;
	}

	//(7) 住所2
	if ( udTemp.m_AdAdd2 != m_ReadData[inRow].m_AdAdd2 ) {
		ufgUpData = TRUE;		// 全体(データ行/空行)
		ufgAdd2 = TRUE;
	}

	//(8) 常勤・非常勤の区別
	if ( udTemp.m_FpTime != m_ReadData[inRow].m_FpTime ) {
		ufgUpData = TRUE;		// 全体(データ行/空行)
		ufgTime = TRUE;
	}

	//(9) 使用人職務分
	if ( udTemp.m_EmVal != m_ReadData[inRow].m_EmVal ) {
		ufgUpData = TRUE;		// 全体(データ行/空行)
		ufgEmVal = TRUE;
	}

	//(10) 定期同額給与
	if ( udTemp.m_PrVal != m_ReadData[inRow].m_PrVal ) {
		ufgUpData = TRUE;		// 全体(データ行/空行)
		ufgPrVal = TRUE;
	}

	//(11) 事前確定届出給与
	if ( udTemp.m_BfVal != m_ReadData[inRow].m_BfVal ) {
		ufgUpData = TRUE;		// 全体(データ行/空行)
		ufgBfVal = TRUE;
	}

	//(12) 利益連動給与
	if ( udTemp.m_PfVal != m_ReadData[inRow].m_PfVal ) {
		ufgUpData = TRUE;		// 全体(データ行/空行)
		ufgPfVal = TRUE;
	}

	//(13) その他
	if ( udTemp.m_EtVal != m_ReadData[inRow].m_EtVal ) {
		ufgUpData = TRUE;		// 全体(データ行/空行)
		ufgEtVal = TRUE;
	}

	//(14) 退職給与
	if ( udTemp.m_RtVal != m_ReadData[inRow].m_RtVal ) {
		ufgUpData = TRUE;		// 全体(データ行/空行)
		ufgRtVal = TRUE;
	}

	//(15) 役員給与計
	if ( udTemp.m_Val != m_ReadData[inRow].m_Val ) {
		ufgUpData = TRUE;		// 全体(データ行/空行)
		ufgVal = TRUE;
	}

	//(16) 特殊行フラグ(基本的には、空行/データ行のチェックになるはず)
	if ( m_ReadData[inRow].m_FgFunc <= ID_FGFUNC_DATA ) {
		// 元が空行orデータ行の場合、入力内容により空行orデータ行となる
		// 元が特殊行の場合、FgFuncは更新しない
		if ( udTemp.m_FgFunc != m_ReadData[inRow].m_FgFunc ) {
			ufgUpData = TRUE;		// 全体(データ行/空行)
			ufgFunc = TRUE;     // ufgFunc用
		}
	}

	///////////////////////////////////////////////////////////////////
	// 1 レコード更新処理
	if ( ( udTemp.m_FgFunc == ID_FGFUNC_DATA ) || ( udTemp.m_FgFunc == ID_FGFUNC_IKKATUMANUAL ) ) {	// データ行or一括集計金額行（手動）
		

		if ( ufgUpData == TRUE ) {		// テーブルへ更新(1レコード更新)
			// ページ、行 で抽出し、必要なデータを更新
			rs.RequeryPageRow( inPage, inRow );

			if ( !rs.IsEOF() ) {		// レコードが取得できた(取得できないことはないはず)
				rs.Edit();

				if ( ufgYaku == TRUE ){
					rs.m_NameYaku = m_clsFunc.DeleteRightSpace( udTemp.m_NameYaku );
				}
				
				if ( ufgTant == TRUE ){
					rs.m_NameTant = m_clsFunc.DeleteRightSpace( udTemp.m_NameTant );
				}
				
				if ( ufgName1 == TRUE || ufgName2 == TRUE ){
					// 末尾スペース削除後に空行削除処理
					udTemp.m_AdName1 = m_clsFunc.DeleteRightSpace( udTemp.m_AdName1 );
					udTemp.m_AdName2 = m_clsFunc.DeleteRightSpace( udTemp.m_AdName2 );
					m_clsFunc.DeleteNullRow( &udTemp.m_AdName1, &udTemp.m_AdName2 );
					rs.m_AdName1 = udTemp.m_AdName1;
					rs.m_AdName2 = udTemp.m_AdName2;
				}
				
				if ( ufgKankei == TRUE ){
					rs.m_Kankei = m_clsFunc.DeleteRightSpace( udTemp.m_Kankei );
				}

				if ( ufgAdd1 == TRUE ){
					rs.m_AdAdd1 = m_clsFunc.DeleteRightSpace( udTemp.m_AdAdd1 );
				}
				
				if ( ufgAdd2 == TRUE ){
					rs.m_AdAdd2 = m_clsFunc.DeleteRightSpace( udTemp.m_AdAdd2 );
				}
				
				if ( ufgTime == TRUE ){
					rs.m_FpTime = udTemp.m_FpTime;
				}

				if ( ufgEmVal == TRUE ){
					rs.m_EmVal = udTemp.m_EmVal;
				}
				
				if ( ufgPrVal == TRUE ){
					rs.m_PrVal = udTemp.m_PrVal;
				}

				if ( ufgBfVal == TRUE ){
					rs.m_BfVal = udTemp.m_BfVal;
				}
				
				if ( ufgPfVal == TRUE ){
					rs.m_PfVal = udTemp.m_PfVal;
				}

				if ( ufgEtVal == TRUE ){
					rs.m_EtVal = udTemp.m_EtVal;
				}

				if ( ufgRtVal == TRUE ){
					rs.m_RtVal = udTemp.m_RtVal;
				}

				if ( ufgVal == TRUE ){
					rs.m_Val = udTemp.m_Val;
				}

				// 特殊行フラグは、強制的に書き込む
				rs.m_FgFunc = udTemp.m_FgFunc;
				rs.m_FgInput = udTemp.m_FgInput;

				rs.Update();
				ufgRsUpdate = TRUE;		// レコード更新したので ON
			}
		}

	}
	else if ( udTemp.m_FgFunc == ID_FGFUNC_NULL ) {	// 空行
		///////////////////////////////////////////////////////////////////
		// 空行の場合の処理

		// 更新処理
		if ( ufgUpData == TRUE ) {		// テーブルへ更新(1レコード更新)
			// ページ、行 で抽出し、必要なデータを更新
			rs.RequeryPageRow( inPage, inRow );

			if ( !rs.IsEOF() ) {		// レコードが取得できた(取得できないことはないはず)
				rs.Edit();
				rs.m_FgFunc = udTemp.m_FgFunc;
				rs.m_KeiStr = _T("");				// 計
				rs.m_NumGroup = 0;					// グループ番号
				rs.m_NameYaku = _T("");				// 役職名
				rs.m_NameTant = _T("");				// 担当業務
				rs.m_AdSeq = 0;						// 氏名/住所：取引先（シーケンス番号）
				rs.m_AdOrder = 0;					// 氏名/住所：取引先（並び順）
				rs.m_AdName1 = _T("");				// 氏名（名称：上段）
				rs.m_AdName2 = _T("");				// 氏名（名称：下段）
				rs.m_AdAdd1 = _T("");				// 住所（所在地：上段）
				rs.m_AdAdd2 = _T("");				// 住所（所在地：下段）
				rs.m_Kankei = _T("");				// 代表者との関係
				rs.m_FpTime = 0;					// 常勤・非常勤の別
				rs.m_EmVal = _T("");				// 使用人職務分				// 2006.10.23：新会社法対応
				rs.m_PrVal = _T("");				// 定期同額給与				// 2006.10.23：新会社法対応
				rs.m_BfVal = _T("");				// 事前確定届出給与			// 2006.10.23：新会社法対応
				rs.m_PfVal = _T("");				// 利益連動給与				// 2006.10.23：新会社法対応
				rs.m_EtVal = _T("");				// その他					// 2006.10.23：新会社法対応
				rs.m_RtVal = _T("");				// 退職給与					// 2006.10.23：新会社法対応
				rs.m_Val = _T("");					// 役員給与計				// 2006.10.23：新会社法対応
				rs.m_FgInput = ID_AUTOCALC_AUTO;	// ｢役員給与計｣ チェックフラグ

				rs.Update();

				ufgRsUpdate = TRUE;		// レコード更新したので ON
			}
		}
	}

	///////////////////////////////////////////////////////////////////
	// 内部 1ページバッファも更新する
	int	retFunc = FUNCTION_OK;		//戻り値：成功（変更なし)
	if ( ufgRsUpdate == TRUE ) {
		retFunc = FUNCTION_UPDATE;		//戻り値：成功（変更あり)
		if ( !rs.IsEOF() ) {		// レコードが取得できた(取得できないことはないはず)
			RecBufSetData( &rs );
		}
	}

	rs.Fin();

	return retFunc;
}

//********************************************************************************
//	「役員報酬」テーブルのオブジェクトを生成し、ポインタを返す
//********************************************************************************
CdbUc000Common*	CfrmUc141YakuinK::virGetDataTableObject()
{
	// 「役員報酬」テーブルのオブジェクトを生成し、ポインタを返す
	return (CdbUc000Common*)( new CdbUc141Yakuin( m_pDB ) );
}

//********************************************************************************
//	行コピー処理
//	IN	なし
//	RET	なし
//********************************************************************************
void	CfrmUc141YakuinK::virRowButtonCopyProc()
{
	// カレント行のデータを、コーピー用バッファに格納
	m_CopyData = m_ReadData[m_uInfo.intCurRow];

	// コピーフラグON
	m_nCopyFlag = 1;
}

//********************************************************************************
//	共通処理から帳表のカレント情報を取得
//	IN	CURRENT_INFO	取得する情報の格納領域
//	RET	なし
//********************************************************************************
void	CfrmUc141YakuinK::virGetCurrentInfo( CURRENT_INFO* uCurInfo )
{
	uCurInfo->intFgFunc = m_ReadData[m_uInfo.intCurRow].m_FgFunc;
	uCurInfo->intNumGroup = m_ReadData[m_uInfo.intCurRow].m_NumGroup;
	uCurInfo->intCopyFlag = m_nCopyFlag;
	uCurInfo->intFgInput = m_ReadData[m_uInfo.intCurRow].m_FgInput;
}

//**************************************************
//	自動計算フラグ確認
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CfrmUc141YakuinK::CheckFgInput()
{
	CString		szNow;	//	現在の入力値を保存
// midori 18-0887 add -->
	CString		szTmp1=_T("");
	CString		szTmp2=_T("");
	CString		szTmp3=_T("");
	CString		szTmp4=_T("");
	CString		szTmp5=_T("");
	CString		szTmp6=_T("");
	int			nCtlIndex=0;
// midori 18-0887 add <--

	//	初期化
	szNow.Empty();
	
	//	特殊行フラグで分岐
	switch( m_ReadData[m_uInfo.intCurRow].m_FgFunc ){
	//	空行
	case ID_FGFUNC_NULL:
	//	データ行
	case ID_FGFUNC_DATA:
	//	一括金額（手動）
	case ID_FGFUNC_IKKATUMANUAL:
		// ポジション取得
		m_uInfo.intCurCtlIndex = m_pDiag->GetPosition();
		// 行・列取得
		CmnGetControlPosition( &m_uInfo.intCurRow , &m_uInfo.intCurCol, &m_uInfo.OldCol );

		// 「役員給与計」列？
		if ( m_uInfo.intCurCol == ID_COL_141_VAL ){
			
			// 現在の入力値を取得
			szNow = m_clsFunc.DiagGetString( m_pDiag, m_uInfo.intCurCtlIndex );

// midori 18-0887 del -->
			////	現在の入力値と前回の入力値が違う？
			//if ( szNow != m_uInfo.OldstrDiag ){
			//	//	自動計算フラグを手入力に設定
			//	m_ReadData[m_uInfo.intCurRow].m_FgInput = ID_AUTOCALC_INPUT;
			//	//	背景色を変更
			//	virInitialControlFgInputCell( m_uInfo.intCurRow, m_ReadData[m_uInfo.intCurRow].m_FgFunc, m_ReadData[m_uInfo.intCurRow].m_FgInput );
			//}
// midori 18-0887 del <--
// midori 18-0887 add -->
			//	(1) 使用人職務分の入力値取得
			nCtlIndex = CmnGetControlIndex(m_uInfo.intCurRow,ID_COL_141_EMVAL);
			szTmp1 = CmnDiagGetKingaku(m_pDiag,nCtlIndex);
			//	(2) 定期同額給与の入力値取得
			nCtlIndex = CmnGetControlIndex(m_uInfo.intCurRow,ID_COL_141_PRVAL);
			szTmp2 = CmnDiagGetKingaku(m_pDiag,nCtlIndex);
			//	(3) 事前確定届出給与の入力値取得
			nCtlIndex = CmnGetControlIndex(m_uInfo.intCurRow,ID_COL_141_BFVAL);
			szTmp3 = CmnDiagGetKingaku(m_pDiag,nCtlIndex);
			//	(4) 利益連動給与の入力値取得
			nCtlIndex = CmnGetControlIndex(m_uInfo.intCurRow,ID_COL_141_PFVAL);
			szTmp4 = CmnDiagGetKingaku(m_pDiag,nCtlIndex);
			//	(5) その他の入力値取得
			nCtlIndex = CmnGetControlIndex(m_uInfo.intCurRow,ID_COL_141_ETVAL);
			szTmp5 = CmnDiagGetKingaku(m_pDiag,nCtlIndex);
			//	(1)から(5)を足し算
// midori UC_0025 del -->
			//szTmp6 = GetAddVal(szTmp1,szTmp2,szTmp3,szTmp4,szTmp5);
// midori UC_0025 del <--
// midori UC_0025 add -->
			if(szTmp1 == "" && szTmp2 == "" && szTmp3 == "" && szTmp4 == "" && szTmp5 == "")	{
				szTmp6 = "";
			}
			else	{
				szTmp6 = GetAddVal(szTmp1,szTmp2,szTmp3,szTmp4,szTmp5);
			}
// midori UC_0025 add <--
			//	現在の入力値からカンマとスペースを除く
			szNow.Replace(_T(","),_T(""));
			szNow.TrimLeft();
			//	自動計算した結果と現在の入力値が違う？
			if(szNow != szTmp6 && szNow.IsEmpty() != TRUE)	{
				//	自動計算フラグを手入力に設定
				m_ReadData[m_uInfo.intCurRow].m_FgInput = ID_AUTOCALC_INPUT;
			}
			else	{
				//	自動計算フラグを自動計算に設定
				m_ReadData[m_uInfo.intCurRow].m_FgInput = ID_AUTOCALC_AUTO;
			}
			//	背景色を変更
			virInitialControlFgInputCell(m_uInfo.intCurRow,m_ReadData[m_uInfo.intCurRow].m_FgFunc,m_ReadData[m_uInfo.intCurRow].m_FgInput);
// midori 18-0887 add <--
		}	
		break;
	default:
		break;
	}
}

//**************************************************
//	自動計算結果を設定
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CfrmUc141YakuinK::SetAutoCalc()
{
	int			nCtlIndex;	//	コントロールインデックス
	int			nCnt;
	CString		szBuf[5];
	CString		szVal;		//	計（下段）
	
	//	初期化
	for( nCnt = 0; nCnt < 5; nCnt++ ){
		szBuf[nCnt].Empty(); 
	}
	
	// ポジション取得
	m_uInfo.intCurCtlIndex = m_pDiag->GetPosition();
	// 行・列取得
	CmnGetControlPosition( &m_uInfo.intCurRow , &m_uInfo.intCurCol, &m_uInfo.OldCol );
	
	//	特殊行フラグで分岐
	switch( m_ReadData[m_uInfo.intCurRow].m_FgFunc ){
	//	空行
	case ID_FGFUNC_NULL:
	//	データ行
	case ID_FGFUNC_DATA:
	//	一括金額（手動）
	case ID_FGFUNC_IKKATUMANUAL:

		//	列で分岐
		switch( m_uInfo.intCurCol ){
		//	使用人職務分
		case ID_COL_141_EMVAL:
		//	定期同額給与
		case ID_COL_141_PRVAL:
		//	事前確定届出給与
		case ID_COL_141_BFVAL:
		//	利益連動給与
		case ID_COL_141_PFVAL:
		//	その他
		case ID_COL_141_ETVAL:
		//	役員給与計
		case ID_COL_141_VAL:
			//	自動計算フラグ有効？
			if ( m_ReadData[m_uInfo.intCurRow].m_FgInput != ID_AUTOCALC_INPUT ){
				
				//	使用人職務分のコントロールインデックス取得
				nCtlIndex = CmnGetControlIndex( m_uInfo.intCurRow, ID_COL_141_EMVAL );
				//	使用人職務分の入力値取得
				szBuf[0] = CmnDiagGetKingaku( m_pDiag, nCtlIndex );
				//	定期同額給与のコントロールインデックス取得
				nCtlIndex = CmnGetControlIndex( m_uInfo.intCurRow, ID_COL_141_PRVAL );
				//	定期同額給与の入力値取得
				szBuf[1] = CmnDiagGetKingaku( m_pDiag, nCtlIndex );
				//	事前確定届出給与のコントロールインデックス取得
				nCtlIndex = CmnGetControlIndex( m_uInfo.intCurRow, ID_COL_141_BFVAL );
				//	事前確定届出給与の入力値取得
				szBuf[2] = CmnDiagGetKingaku( m_pDiag, nCtlIndex );
				//	利益連動給与のコントロールインデックス取得
				nCtlIndex = CmnGetControlIndex( m_uInfo.intCurRow, ID_COL_141_PFVAL );
				//	利益連動給与の入力値取得
				szBuf[3] = CmnDiagGetKingaku( m_pDiag, nCtlIndex );
				//	その他のコントロールインデックス取得
				nCtlIndex = CmnGetControlIndex( m_uInfo.intCurRow, ID_COL_141_ETVAL );
				//	その他の入力値取得
				szBuf[4] = CmnDiagGetKingaku( m_pDiag, nCtlIndex );
				
				//	役員給与計のコントロールインデックスを取得
				nCtlIndex = CmnGetControlIndex( m_uInfo.intCurRow, ID_COL_141_VAL );

				//	全て入力無し？
				if (( szBuf[0] == "" ) && ( szBuf[1] == "" ) && ( szBuf[2] == "" ) &&
					( szBuf[3] == "" ) && ( szBuf[4] == "" )){
					//	役員給与計をクリア
					m_pDiag->DataClear( nCtlIndex, TRUE );
				}
				else{
					//	自動計算
					szVal = GetAddVal( szBuf[0], szBuf[1], szBuf[2], szBuf[3], szBuf[4] );
					//	ICS-Controlへ金額を設定
					CmnDiagSetKingaku( m_pDiag, nCtlIndex, szVal );
// midori 152392 add -->
					// 現在の入力値を取得
					m_uInfo.OldstrDiag = m_clsFunc.DiagGetString( m_pDiag, m_uInfo.intCurCtlIndex );
// midori 152392 add <--
				}
				// 更新
				m_pDiag->Refresh();			
			}
			break;

		//	その他
		default:
			break;
		}
		break;

	//	その他
	default:
		break;
	}
}

//**************************************************
//	自動計算結果を取得
//	【引数】	なし
//	【戻値】	自動計算結果文字列
//**************************************************
CString CfrmUc141YakuinK::GetAutoCalc( int nRow , CString strVal1 , CString strVal2 , CString strVal3 , CString strVal4 , CString strVal5 )
{
	CString		szRet;		//	戻値
	int			nIndex;		//	インデックス
	BOOL		fAutoCalc;	//	自動計算したかフラグ（TRUE：自動計算した/FALSE：自動計算してない）

	//	初期化
	szRet.Empty();
	fAutoCalc = FALSE;
	
	//	特殊行フラグで分岐
	switch( m_ReadData[nRow].m_FgFunc ){
	//	空行
	case ID_FGFUNC_NULL:
	//	データ行
	case ID_FGFUNC_DATA:
	//	一括金額（手動）
	case ID_FGFUNC_IKKATUMANUAL:

		//	自動計算フラグ有効？
		if ( m_ReadData[nRow].m_FgInput != ID_AUTOCALC_INPUT ){
		
			//	役員給与計のコントロールインデックスを取得
			nIndex = CmnGetControlIndex( nRow, ID_COL_141_VAL );

			//	自動計算元に1つでも入力あり？
			if (( strVal1 != "" ) || ( strVal2 != "" ) || ( strVal3 != "" ) ||
				( strVal4 != "" ) || ( strVal5 != "" )){
				//	自動計算
				szRet = GetAddVal( strVal1, strVal2, strVal3, strVal4, strVal5 );
				fAutoCalc = TRUE;
			}
		}
		break;
	
	//	その他
	default:
		break;
	}

	//	自動計算してない？
	if ( fAutoCalc == FALSE ){
		nIndex = CmnGetControlIndex( nRow , ID_COL_141_VAL );
		//szRet = CheckOutsideValue( m_pDiag, nIndex );
		szRet = CmnDiagGetKingaku( m_pDiag , nIndex );
	}

	//	戻値を返す
	return( szRet );
}

//**************************************************
//	掛け算した値を取得
//	【引数】	szVal1		…	値1
//				szVal2		…	値2
//				szVal3		…	値3
//				szVal4		…	値4
//				szVal5		…	値5
//	【戻値】	加算結果
//**************************************************
CString CfrmUc141YakuinK::GetAddVal( CString szVal1, CString szVal2, CString szVal3, CString szVal4, CString szVal5 )
{
	CString		szRet;			//	戻値
	__int64		i64Val[6];			//	金額格納バッファ
	char		szRes[32];		//	結果
	
	//	初期化
	szRet = _T("0");
	i64Val[0] = 0;
	i64Val[1] = 0;
	i64Val[2] = 0;
	i64Val[3] = 0;
	i64Val[4] = 0;
	i64Val[5] = 0;
	ZeroMemory( szRes, sizeof( szRes ) );

	// 計算結果を取得
	i64Val[0] = _strtoi64( szVal1 , NULL , 10 );
	i64Val[1] = _strtoi64( szVal2 , NULL , 10 );
	i64Val[2] = _strtoi64( szVal3 , NULL , 10 );
	i64Val[3] = _strtoi64( szVal4 , NULL , 10 );
	i64Val[4] = _strtoi64( szVal5 , NULL , 10 );
	i64Val[5] = i64Val[0] + i64Val[1] + i64Val[2] + i64Val[3] + i64Val[4];

	_i64toa_s(i64Val[5], szRes, 32, 10);

	//	値が範囲を超えた?
	if ( i64Val[5] > DB_DECIMAL_MAX ){
		_i64toa_s(DB_DECIMAL_MAX, szRes, 32, 10);
	}
	else if ( i64Val[5] < DB_DECIMAL_MIN ){
		_i64toa_s(DB_DECIMAL_MIN, szRes, 32, 10);
	}

	//	戻値に設定
	szRet = szRes;

	//	戻値を返す
	return( szRet );
}

//**************************************************
//	「F6：項目複写」ボタンの監視
//	※自動計算対象列は項目複写しない対応
//	【引数】	bFncEnable	…	ボタンの有効／無効状態
//									TRUE	…	有効
//									FALSE	…	無効
//	【戻値】	なし
//**************************************************
void CfrmUc141YakuinK::virOnUpdateButtonF6(BOOL* bFncEnable)
{
/*
	int		nCol;	//	列格納
	
	//	現在の列取得
	nCol = CmnGetControlCol();
	
	//	ボタンが有効でかつ列が期末現在高？
	if (( *bFncEnable == TRUE ) && ( nCol == ID_COL_141_VAL2 )){
		//	項目複写しないから無効に設定
		*bFncEnable = FALSE;
	}
*/
}

//********************************************************************************
//	通常/個別切り替え時、関連帳表データ頁構成の更新処理
//		IN		なし
//		RET		BOOL		TRUE:正常終了，FALSE:エラー
//********************************************************************************
int	CfrmUc141YakuinK::virUpdateRelationTblChengeFormType()
{
	// ⑭-1で通常/個別を変更した場合、⑭-2の頁構成の更新が必要となる
	CfrmUc142Jinkenhi	pfrm142;
	int intRet = pfrm142.UpdateTbl142ChangeFormType();

	return intRet;
}

//********************************************************************************
//	通常/個別切り替え時の頁構成を更新（⑭-2から実行される処理）
//		IN		なし
//		RET		なし
//********************************************************************************
int	CfrmUc141YakuinK::UpdateTbl141ChangeFormType()
{
	// 初期値設定
	CmnSetInitialUchiwakeInfo(ID_FORMNO_141);	// 共通データ
	virSetInitialValue();						// 帳表固有データ

	// データ無しならデータ更新しない
	if (m_uInfo.intMaxPage <= 0) {
		return FUNCTION_OK;
	}

// midori 152770 del -->
//	// 頁と行の再設定
//	CmnTblUpdatePageRowAllResetMain();
// midori 152770 del <--
// midori 152770 add -->
	// 頁と行の再設定（先頭の空行を削除する）
	CmnTblUpdatePageRowAllResetMain(2);
// midori 152770 add <--

	return FUNCTION_OK;
}

//**************************************************
//	入力不可モードの設定/解除
//	【引数】	nRow	…	行
//				nMode	…	入力不可モードフラグ
//								DIAG_MDFY_NOINPUT	…	エディットアイテムで全てのWM_CHARを無効とする
//								DIAG_MDFY_INPUT		…	DIAG_MDFY_NOINPUTモード解除
//	【戻値】	なし
//**************************************************
void CfrmUc141YakuinK::SetInputMode( int nRow, long nMode )
{
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_141_NAMEYAKU ), nMode );	//	役職名
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_141_NAMETANT ), nMode );	//	担当業務
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_141_ADNAME1 ), nMode );	//	氏名（上段）
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_141_KANKEI ), nMode );	//	代表者との関係
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_141_ADADD1 ), nMode );	//	住所（上段）
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_141_ADADD2 ), nMode );	//	住所（下段）
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_141_FPTIME ), nMode );	//	常勤・非常勤の別
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_141_EMVAL ), nMode );		//	使用人職務分
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_141_PRVAL ), nMode );		//	定期同額給与
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_141_BFVAL ), nMode );		//	事前確定届出給与
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_141_PFVAL ), nMode );		//	利益連動給与
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_141_ETVAL ), nMode );		//	その他
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_141_RTVAL ), nMode );		//	退職給与
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_141_VAL ), nMode );		//	役員給与計
}

//**************************************************
//	各Diagの背景色(BackColor)に色を設定
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CfrmUc141YakuinK::SetDiagBackColor()
{
	// 入力画面の各Diagの背景色を共通設定色に合わせる
	m_PageDiag.SetBackColor( m_ViewColor );
	m_TitleDiag.SetBackColor( m_ViewColor );
	m_NumberDiag.SetBackColor( m_ViewColor );
	m_HeadDiag.SetBackColor( m_ViewColor );
	m_DiagNormal.SetBackColor( m_ViewColor );
	m_DiagNormal2.SetBackColor( m_ViewColor );
	m_DiagKobetu.SetBackColor( m_ViewColor );
	m_DiagKobetu2.SetBackColor( m_ViewColor );
// midori 161108 add -->
	m_SaveDataDiag.SetBackColor( m_ViewColor );
// midori 161108 add <--
// 修正No.157875 add -->
	m_DiagKei1.SetBackColor( m_ViewColor );
	m_DiagKei2.SetBackColor( m_ViewColor );
// 修正No.157875 add <--
}

//**************************************************
//	値の範囲外確認
//	【引数】	pDiag		…	DIAGRAM_DATA構造体
//				nIndex		…	インデックス
//	【戻値】	期末現在高
//**************************************************
/*CString CfrmUc141YakuinK::CheckOutsideValue( CICSDiag* pDiag, int nIndex )
{
	CString			szBuf;	//	バッファ
	CString			szRet;	//	戻値

	//	金額を取得
	szRet = CmnDiagGetKingaku( m_pDiag, nIndex );
	
	//	金額が0？
	if ( szRet.Trim() == "0" ){
		
		//	本当は値が0じゃない？
		if ( CheckOutsideValueSub( pDiag, nIndex ) == TRUE ){
			//	文字列に変換
			szBuf.Format( "%f", ( double )DB_DECIMAL_MAX );
			//	範囲外の値を設定
			szRet = szBuf.Left( szBuf.Find(".") );
		}
	}
	
	//	戻値を返す
	return ( szRet );
}*/

//**************************************************
//	値の範囲外確認
//	【引数】	pDiag		…	DIAGRAM_DATA構造体
//				nIndex		…	インデックス
//				nCol		…	列
//	【戻値】	TRUE		…	0じゃない
//				FALSE		…	0
//**************************************************
/*BOOL CfrmUc141YakuinK::CheckOutsideValue( CICSDiag* pDiag, int nIndex, int nCol )
{
	BOOL			fRet = FALSE;	//	
	CString			szRet;			//	戻値
	
	//	役員給与計？
	if ( nCol == ID_COL_141_VAL ){

		//	金額を取得
		szRet = CmnDiagGetKingaku( m_pDiag, nIndex );
		
		//	金額が0？
		if ( szRet.Trim() == "0" ){
			
			//	本当は値が0じゃない？
			fRet = CheckOutsideValueSub( pDiag, nIndex );
		}
	}
	
	//	戻値を返す
	return ( fRet );
}*/

//**************************************************
//	値が0か確認
//	【引数】	pDiag		…	DIAGRAM_DATA構造体
//				nIndex		…	インデックス
//	【戻値】	TRUE		…	0じゃない
//				FALSE		…	0
//**************************************************
/*BOOL CfrmUc141YakuinK::CheckOutsideValueSub( CICSDiag* pDiag, int nIndex )
{
	BOOL			fRet;
	DIAGRAM_DATA	udData;
	
	// 初期化
	fRet = FALSE;
	m_clsFunc.DiagInit( &udData );
	
	// 金額取得
	pDiag->GetData( nIndex , ( LPUNKNOWN )&udData );

	//	'*'あり？
	if ( udData.data_edit.Find( '*' ) != -1 ){
		fRet = TRUE;
	}
	
	//	戻値を返す

	return( fRet );
}*/


//***************************************************************************
//（2009.09.08 TS自動ログオフ対応）
//	Viewの終了処理
//		・デストラクタで行っていた処理をここに移動
//
//	IN		なし
//	RET		BOOL				TRUE=エラー，FALSE=正常
//***************************************************************************
BOOL CfrmUc141YakuinK::virEndProc(void)
{
	TRACE(_T("***** CfrmUc141YakuinK::virEndProc\n"));

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

// midori 151405 add -->
int CfrmUc141YakuinK::CursorControl(short nChar, int sw)
{
	int				rv=0;
	int				intCol=0;
	int				pos=0;

	// 現在の列番号を取得
	intCol = CmnGetControlCol();

	// 例外処理：移動前にComnoBoxで←/→キーが押下された場合は値を元に戻す
	if ((nChar == VK_LEFT) || (nChar == VK_RIGHT)) {
		// ComboBoxは選択内容を確定させない
		if (intCol == ID_COL_141_FPTIME) {
			m_pDiag->ComboDropDown(m_uInfo.intCurCtlIndex, FALSE);
			CmnDiagSetCombo(m_pDiag, m_uInfo.intCurCtlIndex, m_uInfo.OldCombo);
		}
	}

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

// No.200601 add -->
// ------------------------------------------------------------------------------------------------------------------------
// 全頁の電子申告の桁数チェックを行い、タイトルの文字色を変更する
// ------------------------------------------------------------------------------------------------------------------------
void CfrmUc141YakuinK::TitleColorChg(void)
{
	int					nCharOver={0};	// 電子申告の文字数を超えたかどうかのサイン		1:電子申告の文字数を超えた
										// 様式⑭-1		[0]:担当業務(10文字)
	CRecordset			rs(m_pDB);
	CString				strSQL=_T("");
	CString				strData=_T("");

	// 担当業務
// 修正No.157628 del -->
	//strSQL.Format(_T("select max(oversgn) from (select CASE when len(Replace(isnull(NameTant,''),CHAR(13)+CHAR(10),'')) > %d then 1 else 0 end as oversgn from uc_141_yakuin where FgShow=%d) tbl1"),UC_141_NAMETANT_LEN,ID_FGSHOW_OFF);
// 修正No.157628 del <--
// 修正No.157628 add -->
	strSQL.Format(_T("select max(oversgn) from (select CASE when len(%s) > %d then 1 else 0 end as oversgn from uc_141_yakuin where FgShow=%d) tbl1"),
		MakeReplaceSent(_T("NameTant")),UC_141_NAMETANT_LEN,ID_FGSHOW_OFF);
// 修正No.157628 add <--
	rs.Open( CRecordset::forwardOnly, strSQL);
	rs.GetFieldValue((short)0, strData);
	rs.Close();
	nCharOver = 0;
	if(_tstoi(strData) != 0) {
		nCharOver = 1;
	}

	// 項目タイトル(ヘッダ部)の再設定
// 修正No.157626 del -->
	//CmnHeadDiagSetTitle(&m_HeadDiag,1,_T("担当業務(10)"),nCharOver);
// 修正No.157626 del <--
// 修正No.157626 add -->
	if(nCharOver == 1)	CmnHeadDiagSetTitle(&m_HeadDiag,1,_T("担当業務(10)"),1);
	else				CmnHeadDiagSetTitle(&m_HeadDiag,1,_T("担当業務"),0);

	// 欄外ガイドの表示
	if(nCharOver == 1)	m_Guide1.ShowWindow(TRUE);
	else				m_Guide1.ShowWindow(FALSE);
// 修正No.157626 add <--
}
// No.200601 add <--

// 修正No.162446 add -->
LRESULT CfrmUc141YakuinK::OnUserReDrawView(WPARAM wParam, LPARAM lParam)
{
	virUpdateControlTblData();
	CmnDiagSetFocus(m_pDiag, 1, 1);

	return(0);
}
// 修正No.162446 add <--
