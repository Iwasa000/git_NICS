//--------------------------------------------------
//	frmUc142Jinkenhi.cpp
//--------------------------------------------------

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "frmUc142Jinkenhi.h"
#include "frmUc141Yakuin.h"			// 通常/個別切り替えにて"⑭-1"データ更新が必要

// midori 190301 add -->
extern BOOL	bG_Kanso;
// midori 190301 add <--

IMPLEMENT_DYNCREATE( CfrmUc142Jinkenhi, CfrmUc000Common )


BEGIN_MESSAGE_MAP( CfrmUc142Jinkenhi, CfrmUc000Common )
	ON_WM_SIZE()
//	ON_COMMAND( ID_BUTTON_F1, OnButtonF1 )
	ON_COMMAND( ID_BUTTON_F2, OnButtonF2 )
//	ON_COMMAND( ID_BUTTON_F3, OnButtonF3 )
	ON_COMMAND( ID_BUTTON_F4, OnButtonF4 )
	ON_COMMAND( ID_BUTTON_F5, OnButtonF5 )
//	ON_COMMAND( ID_BUTTON_F6, OnButtonF6 )
//	ON_COMMAND( ID_BUTTON_F7, OnButtonF7 )
//	ON_COMMAND( ID_BUTTON_F8, OnButtonF8 )
//	ON_COMMAND( ID_BUTTON_F9, OnButtonF9 )
//	ON_COMMAND( ID_BUTTON_F11, OnButtonF11 )
//	ON_COMMAND( ID_BUTTON_F12, OnButtonF12 )
	ON_COMMAND( ID_BUTTON_HOME, OnButtonHome )
	ON_COMMAND( ID_BUTTON_INSERT, OnButtonInsert )
	//ON_COMMAND( ID_BUTTON_END, OnButtonEnd )
	ON_COMMAND( ID_PRINTSETUP_MENU, &CfrmUc142Jinkenhi::OnMenuPrintSetup )
	ON_COMMAND( ID_PRINTALL_MENU, &CfrmUc142Jinkenhi::OnMenuPrintAll )
	ON_COMMAND( ID_OUTPUTFORM_MENU, &CfrmUc142Jinkenhi::OnMenuFormTypeSetup )
//	ON_COMMAND( ID_DISPDIALOG_MENU, &CfrmUc142Jinkenhi::OnMenuIkkatuKingaku )
//	ON_COMMAND( ID_ZEROMONEY_MENU, &CfrmUc142Jinkenhi::OnMenuZeroMoney )
	ON_COMMAND( ID_ZEROMONEYMOVE_MENU, &CfrmUc142Jinkenhi::OnMenuZeroMoneyMove )
	ON_COMMAND( ID_AMOUTDIALOG_MENU, &CfrmUc142Jinkenhi::OnMenuOutKeiSetup )
// midori 160606 add -->
	ON_COMMAND( ID_CLEAR_MENU, &CfrmUc142Jinkenhi::OnMenuClear )
// midori 160606 add <--
	ON_COMMAND( ID_RENDO_MENU, &CfrmUc142Jinkenhi::OnMenuZaimuRendo )
	ON_COMMAND( ID_TITLEMENU, &CfrmUc142Jinkenhi::OnMenuTitleRestore )
	ON_COMMAND( ID_USER_SETTING_MENU, &CfrmUc142Jinkenhi::OnMenuOptionSetting )
	ON_COMMAND( ID_COPY_SETTING_INFO_MENU, &CfrmUc142Jinkenhi::OnMenuCopySettingInfo )
	ON_COMMAND( ID_DELETE_DATA_MENU, &CfrmUc142Jinkenhi::OnMenuDeleteDataEachYoushiki )
//	ON_BN_CLICKED( IDC_PAGEBACK_BUTTON1, &CfrmUc142Jinkenhi::OnPageBack )
//	ON_BN_CLICKED( IDC_PAGENEXT_BUTTON1, &CfrmUc142Jinkenhi::OnPageNext )
//	ON_BN_CLICKED( IDC_PAGENEW_BUTTON1, &CfrmUc142Jinkenhi::OnPageNew )
//	ON_BN_CLICKED( IDC_PAGEINSERT_BUTTON1, &CfrmUc142Jinkenhi::OnPageInsert )
//	ON_BN_CLICKED( IDC_ROWCOPY_BUTTON1, &CfrmUc142Jinkenhi::OnRowCopy )
//	ON_BN_CLICKED( IDC_ROWPASTE_BUTTON1, &CfrmUc142Jinkenhi::OnRowPaste )
//	ON_BN_CLICKED( IDC_ROWINSERT_BUTTON1, &CfrmUc142Jinkenhi::OnRowInsert )
	ON_BN_CLICKED( IDC_YOUSHIKINEXT_BUTTON1, &CfrmUc142Jinkenhi::OnMoveYoushikiNext )
	ON_BN_CLICKED( IDC_YOUSHIKIBACK_BUTTON1, &CfrmUc142Jinkenhi::OnMoveYoushikiBack )
//	ON_UPDATE_COMMAND_UI( ID_BUTTON_F1, &CfrmUc142Jinkenhi::OnUpdateButtonF1 )
//	ON_UPDATE_COMMAND_UI( ID_BUTTON_F3, &CfrmUc142Jinkenhi::OnUpdateButtonF3 )
//	ON_UPDATE_COMMAND_UI( ID_BUTTON_F6, &CfrmUc142Jinkenhi::OnUpdateButtonF6 )
//	ON_UPDATE_COMMAND_UI( ID_BUTTON_F7, &CfrmUc142Jinkenhi::OnUpdateButtonF7 )
//	ON_UPDATE_COMMAND_UI( ID_BUTTON_F8, &CfrmUc142Jinkenhi::OnUpdateButtonF8 )
//	ON_UPDATE_COMMAND_UI( ID_BUTTON_F9, &CfrmUc142Jinkenhi::OnUpdateButtonF9 )
//	ON_UPDATE_COMMAND_UI( ID_BUTTON_F11, &CfrmUc142Jinkenhi::OnUpdateButtonF11 )
//	ON_UPDATE_COMMAND_UI( ID_DISPDIALOG_MENU, &CfrmUc142Jinkenhi::OnUpdateMenuIkkatuKingaku )
//	ON_UPDATE_COMMAND_UI( ID_ZEROMONEY_MENU, &CfrmUc142Jinkenhi::OnUpdateMenuZeroMoney )
	ON_UPDATE_COMMAND_UI( ID_RENDO_MENU, &CfrmUc142Jinkenhi::OnUpdateMenuZaimuRendo )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_HOME, &CfrmUc142Jinkenhi::OnUpdateButtonHome )
// midori 160612 add -->
	ON_UPDATE_COMMAND_UI( ID_CLEAR_MENU, &CfrmUc142Jinkenhi::OnUpdateMenuClear )
	ON_UPDATE_COMMAND_UI( ID_ZEROMONEYMOVE_MENU, &CfrmUc142Jinkenhi::OnUpdateMenuZeroMoneyMove)
	ON_UPDATE_COMMAND_UI( ID_TITLEMENU, &CfrmUc142Jinkenhi::OnUpdateMenuTitleRestore )
	ON_UPDATE_COMMAND_UI( ID_DELETE_DATA_MENU, &CfrmUc142Jinkenhi::OnUpdateMenuDeleteDataEachYoushiki )
// midori 160612 add <--
END_MESSAGE_MAP()


CfrmUc142Jinkenhi::CfrmUc142Jinkenhi()
	: CfrmUc000Common( CfrmUc142Jinkenhi::IDD )
{
	m_pDB = ( (CUCHIWAKEApp*)AfxGetApp() )->m_pDB;
	m_pTitleDiag = &m_TitleDiag;
// midori 152718 add -->
	m_pPageDiag	= &m_PageDiag;
// midori 152718 add <--
}

CfrmUc142Jinkenhi::~CfrmUc142Jinkenhi()
{
	// 2009.09.08 TS自動ログオフ対応(virEndProcへ移動)
	TRACE(_T("***** CfrmUc142Jinkenhi::~CfrmUc142Jinkenhi\n"));

	//m_pDiag = NULL;
	//m_pTitleDiag = NULL;
}

void CfrmUc142Jinkenhi::DoDataExchange( CDataExchange* pDX )
{
	CfrmUc000Common::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_YOUSHIKIDIAG1, m_Diag );
	DDX_Control( pDX, IDC_TITLEDIAG1, m_TitleDiag );
	DDX_Control( pDX, IDC_PAGEDIAG1, m_PageDiag );
	DDX_Control( pDX, IDC_NUMBER_DIAG1, m_NumberDiag );
	DDX_Control( pDX, IDC_TOPICDIAG1, m_HeadDiag);
}

void CfrmUc142Jinkenhi::OnInitialUpdate()
{
	CfrmUc000Common::OnInitialUpdate();
	ResizeParentToFit();

	// 帳表テーブルのオープンOK?
	if ( m_InitialFlag == TRUE ){
		// リサイズ
		SetResize( IDC_YOUSHIKIDIAG1 , ICS_RESIZE_BOTTOM );

		// 背景色を取得
		m_ViewColor = CmnGetBackColor();

		// 帳表表示処理OK?
		if ( virStartJob() == TRUE ){ 
			// 最小化／最大化ボックスを無効に設定
//			CmnEnableMinMaxBox( GetParent() );
		}
		else{
			// カーソルフラグ=FALSE(この後フォーカスセット処理が走らないように)
			m_CursorSetFlag = FALSE;
		}
	}
}

#ifdef _DEBUG
void CfrmUc142Jinkenhi::AssertValid() const
{
	CfrmUc000Common::AssertValid();
}

#ifndef _WIN32_WCE
void CfrmUc142Jinkenhi::Dump( CDumpContext& dc ) const
{
	CfrmUc000Common::Dump( dc );
}
#endif
#endif //_DEBUG

BEGIN_EVENTSINK_MAP( CfrmUc142Jinkenhi, CfrmUc000Common )
	ON_EVENT( CfrmUc142Jinkenhi, IDC_YOUSHIKIDIAG1, 3, CfrmUc142Jinkenhi::TerminationYoushikidiag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN )
	ON_EVENT( CfrmUc142Jinkenhi, IDC_YOUSHIKIDIAG1, 2, CfrmUc142Jinkenhi::EditOFFYoushikidiag1, VTS_I2 )
	ON_EVENT( CfrmUc142Jinkenhi, IDC_YOUSHIKIDIAG1, 1, CfrmUc142Jinkenhi::EditONYoushikidiag1, VTS_I2 )

	ON_EVENT( CfrmUc142Jinkenhi, IDC_TITLEDIAG1, 3, CfrmUc142Jinkenhi::TerminationTitlediag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN )
	ON_EVENT( CfrmUc142Jinkenhi, IDC_TITLEDIAG1, 2, CfrmUc142Jinkenhi::EditOFFTitlediag1, VTS_I2 )
	ON_EVENT( CfrmUc142Jinkenhi, IDC_TITLEDIAG1, 1, CfrmUc142Jinkenhi::EditONTitlediag1, VTS_I2 )
	ON_EVENT( CfrmUc142Jinkenhi, IDC_TITLEDIAG1, 6, CfrmUc142Jinkenhi::FocusTitlediag1, VTS_NONE )

	ON_EVENT( CfrmUc142Jinkenhi, IDC_PAGEDIAG1, 3, CfrmUc142Jinkenhi::TerminationPagediag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN )
	ON_EVENT( CfrmUc142Jinkenhi, IDC_PAGEDIAG1, 2, CfrmUc142Jinkenhi::EditOFFPagediag1, VTS_I2 )
	ON_EVENT( CfrmUc142Jinkenhi, IDC_PAGEDIAG1, 1, CfrmUc142Jinkenhi::EditONPagediag1, VTS_I2 )
	ON_EVENT( CfrmUc142Jinkenhi, IDC_PAGEDIAG1, 6, CfrmUc142Jinkenhi::FocusPagediag1, VTS_NONE )
END_EVENTSINK_MAP()

//********************************************************************************
//	初期処理
//********************************************************************************
BOOL	CfrmUc142Jinkenhi::virStartJob()
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
void	CfrmUc142Jinkenhi::virSetInitialUchiwakeInfo()
{
	// 「人件費」の固有データ
	m_uInfo.intOutKeiDialog = AM_TYPE_OFF | AM_TYPE_RUIKEI;		// 計設定ダイアログのボタン指定
	m_uInfo.intRowMax = ID_ROWKOBETU_142;						// １頁内の合計行を含む全体の行数
	m_uInfo.intRowMaxData = ID_ROWKOBETU_142 - 1;				// １頁内のデータ行数（合計行は含まず）
	m_uInfo.intColMax = ID_COLMAX_142;							// １行内のコントロール個数
	m_uInfo.intColMaxData = ID_COLMAXDATA_142;					// １行内の列数
	m_uInfo.intCtlStartIndex = ID_CTLSTARTINDEX_142;			// １行目の入力項目の先頭インデックス番号
	m_uInfo.intT1StartIndex = ID_T1STARTNORMAL_142;				// 合計行①の先頭インデックス番号
	m_uInfo.intT2StartIndex = ID_T2STARTNORMAL_142;				// 合計行②の入力項目の先頭インデックス番号
	m_uInfo.intCursolDefPos = ID_COL_142_VAL;					// カーソル移動時の初期位置（タイトルから↓，合計から↑で使用）
	m_uInfo.intCursolKeiPos = ID_COL_142_VAL;					// 合計行のカーソル移動位置（カーソル移動で使用）

	m_uInfo.intFormSeq = ID_FORMNO_142;							// 様式シーケンス番号
	m_uInfo.intFormNo = 14;										// 様式番号（uc_inf_sub）
	m_uInfo.intFormNo2 = 2;										// 様式番号枝番（uc_inf_sub）

	m_uInfo.intRowNormal = ID_ROWNORMAL_142;					// 通常時：１頁内の合計行を含む全体の行数
	m_uInfo.intRowKobetu = ID_ROWKOBETU_142;					// 個別時：１頁内の合計行を含む全体の行数
	m_uInfo.intT1StartNormal = ID_T1STARTNORMAL_142;			// 通常時：合計行①の先頭インデックス
	m_uInfo.intT2StartNormal = ID_T2STARTNORMAL_142;			// 通常時：合計行②の先頭インデックス
	m_uInfo.intT1StartKobetu = ID_T1STARTKOBETU_142;			// 個別時：合計行①の先頭インデックス
	m_uInfo.intT2StartKobetu = ID_T2STARTKOBETU_142;			// 個別時：合計行②の先頭インデックス
	m_uInfo.intRowKei = ID_ROWKEI_142;							// 帳表固定の合計行数（1:合計あり，0:なし）

	// 集計処理の設定
	m_uInfo.uCalcInfo.intMaxCount = ID_CALC_142_MAX;			// 集計フィールド件数
	m_uInfo.uCalcInfo.strField[0] = ID_CALC_142_FIELD1;			// 集計フィールド名称1
	m_uInfo.uCalcInfo.strField[1] = ID_CALC_142_FIELD2;			// 集計フィールド名称2
}

//**************************************************
// ICSDiagコントロールを通常／個別で切り替える
// 【引数】    なし
// 【戻値】    なし
//**************************************************
void	CfrmUc142Jinkenhi::virSetInitialDiag()
{
	// オブジェクト未生成時は処理を抜ける
	if ( m_Diag.m_hWnd == NULL ){
		return;
	}

	m_pDiag = &m_Diag;

	// 入力不可行でのDeleteキー無効
	m_pDiag->EnableDelete(0);

}

//********************************************************************************
//	帳表固有変数の初期値設定
//********************************************************************************
void	CfrmUc142Jinkenhi::virSetInitialValue()
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
BOOL	CfrmUc142Jinkenhi::InitialControlDataArea()
{
	int		intMax;
	int		i;
// No.200903 add -->
	int		len=0;
// No.200903 add <--

	// 各Diagに背景色を設定
	SetDiagBackColor();

	// 列の属性により更にクリアと初期設定が必要
	intMax = m_uInfo.intT1StartIndex;
	for ( i = m_uInfo.intCtlStartIndex; i < intMax; i += m_uInfo.intColMax ) {
		// 金額欄：桁数初期化
// No.200903 del -->
		//CmnDiagSetAttrDigit( m_pDiag, i + ID_COL_142_VAL - 1, ID_VAL_142_DATA );	// 総額
		//CmnDiagSetAttrDigit( m_pDiag, i + ID_COL_142_VAL2 - 1, ID_VAL2_142_DATA );	// 総額のうち代表者及びその家族分
// No.200903 del <--
// No.200903 add -->
		if(bG_Kanso == TRUE)	len = ID_VAL_142_K_DATA;
		else					len = ID_VAL_142_DATA;
		CmnDiagSetAttrDigit( m_pDiag, i + ID_COL_142_VAL - 1, len );	// 総額

		if(bG_Kanso == TRUE)	len = ID_VAL2_142_K_DATA;
		else					len = ID_VAL2_142_DATA;
		CmnDiagSetAttrDigit( m_pDiag, i + ID_COL_142_VAL2 - 1, len );	// 総額のうち代表者及びその家族分
// No.200903 add <--

		//	入力不可モードの解除
		SetInputMode( CmnGetControlRow( i ), DIAG_MDFY_INPUT );
	}

	return TRUE;
}

//********************************************************************************
//	帳表コントロールの初期化（頁計/累計領域）
//		文言はデータに含まれているので、その内容を使用する
//********************************************************************************
void	CfrmUc142Jinkenhi::InitialControlTotalArea()
{
	int		nKeiRow;			//	計行属性
	int		nCnt;				//	カウント
	int		nStart;				//	開始位置

	//	計行の設定
	nKeiRow = ( DIAG_MDFY_READONLY | DIAG_MDFY_OPAQUE );
	
	// 28行目の合計行①
	nStart = m_uInfo.intT1StartIndex;
	for ( nCnt = 0; nCnt < m_uInfo.intColMax; nCnt++ ){
		m_pDiag->ModifyItem( nStart + nCnt, nKeiRow );
	}

// No.200903 del -->
	//// 合計行の金額欄は、金額欄 + 1桁
	//CmnDiagSetAttrDigit( m_pDiag, CmnGetControlTotalIndex( 1, ID_COL_142_VAL ), ID_VAL_142_TOTAL );
	//CmnDiagSetAttrDigit( m_pDiag, CmnGetControlTotalIndex( 1, ID_COL_142_VAL2 ), ID_VAL2_142_TOTAL );
// No.200903 del <--
}

//********************************************************************************
//	帳表タイトル情報の表示
//********************************************************************************
void	CfrmUc142Jinkenhi::virUpdateControlTitle()
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

// midori 190301 add -->
	// ----------------------------------------------------------------------------
	// 項目名の変更（新様式：役員給与・給与手当、旧様式：役員報酬手当・給料手当）
	// ----------------------------------------------------------------------------
	if(bG_Kanso == TRUE)	{
		m_clsFunc.DiagSetString(&m_Diag,1,ID_DIAGDATA_DISP,_T("役　　員　　給　　与"));
		m_clsFunc.DiagSetString(&m_Diag,7,ID_DIAGDATA_DISP,_T("給　与　手　当"));
	}
	else	{
		m_clsFunc.DiagSetString(&m_Diag,1,ID_DIAGDATA_DISP,_T("役　員　報　酬　手　当"));
		m_clsFunc.DiagSetString(&m_Diag,7,ID_DIAGDATA_DISP,_T("給　料　手　当"));
	}
// midori 190301 add <--
}

//********************************************************************************
//	帳表ページ情報の表示
//********************************************************************************
void	CfrmUc142Jinkenhi::UpdateControlPage()
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

	//	全て無効に設定
	GetDlgItem( IDC_PAGEBACK_BUTTON1 )->EnableWindow( FALSE );				// 前頁ボタン
	GetDlgItem( IDC_PAGENEXT_BUTTON1 )->EnableWindow( FALSE );				// 次頁ボタン
	GetDlgItem( IDC_PAGENEW_BUTTON1 )->EnableWindow( FALSE );				// 改頁ボタン
	GetDlgItem( IDC_PAGEINSERT_BUTTON1 )->EnableWindow( FALSE );			// 頁挿入ボタン
	GetDlgItem( IDC_ROWCOPY_BUTTON1 )->EnableWindow( FALSE );				// 行コピーボタン
	GetDlgItem( IDC_ROWPASTE_BUTTON1 )->EnableWindow( FALSE );				// 行貼付ボタン
	GetDlgItem( IDC_ROWINSERT_BUTTON1 )->EnableWindow( FALSE );				// 行挿入貼付ボタン
//	GetDlgItem( IDC_YOUSHIKIBACK_BUTTON1 )->EnableWindow( FALSE );			// 行削除ボタン
//	GetDlgItem( IDC_YOUSHIKINEXT_BUTTON1 )->EnableWindow( FALSE );			// 行挿入ボタン
	//m_PageDiag.ModifyItem( 1, ( DIAG_MDFY_READONLY | DIAG_MDFY_OPAQUE ) );	//	頁Diag
}

//********************************************************************************
//	帳表データの表示（１ページ分）
//********************************************************************************
BOOL	CfrmUc142Jinkenhi::virUpdateControlTblData()
{
// No.200903 add -->
	int len = 0;
// No.200903 add <--

	CdbUc142Jinkenhi	dbRec( m_pDB );

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
	if(bG_Kanso == TRUE)	len = ID_VAL_142_K_TOTAL;
	else					len = ID_VAL_142_TOTAL;
	CmnDiagSetAttrDigit( m_pDiag, CmnGetControlTotalIndex( 1, ID_COL_142_VAL ), len );
	if(bG_Kanso == TRUE)	len = ID_VAL2_142_K_TOTAL;
	else					len = ID_VAL2_142_TOTAL;
	CmnDiagSetAttrDigit( m_pDiag, CmnGetControlTotalIndex( 1, ID_COL_142_VAL2 ), len );
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

		dbRec.MoveNext();
	}

	// 閉じる
	dbRec.Fin();
// midori 160612 add -->
	fm142DiagSetAttrSpecialRow();
	InitialControlTotalArea();				// 頁計/累計行
// midori 160612 add <--

	// 画面更新
	m_pDiag->Refresh();

	return TRUE;
}

//********************************************************************************
//	帳表データの１行表示（空行，データ行，特殊行）
//		IN		CdbUc142Jinkenhi*	「人件費」
//		RET		なし
//********************************************************************************
void	CfrmUc142Jinkenhi::UpdateControlRowData( CdbUc142Jinkenhi* prmDbRec ) {
	DIAGRAM_DATA	diadata;	// ICSDiag構造体
	int				intCell;	// 保存フィールド

	// 初期化
	m_clsFunc.DiagInit( &diadata );

	// 読込み対象セル取得
	intCell = CmnGetControlIndex( prmDbRec->m_NumRow, ID_COL_142_VAL );

	// 総額
	if ( prmDbRec->m_Val != "" ) {
		CmnChangeFieldValData( &diadata.data_val[0], prmDbRec->m_Val );
		m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	}
	else {
		m_pDiag->DataClear( intCell, TRUE );
	}
	intCell++;

	// 総額のうち代表者及びその家族分
	if ( prmDbRec->m_Val2 != "" ) {
		CmnChangeFieldValData( &diadata.data_val[0], prmDbRec->m_Val2 );
		m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	}
	else {
		m_pDiag->DataClear( intCell, TRUE );
	}
}

//********************************************************************************
//	帳表データの１行表示（累計，頁計行）
//		IN		CdbUc142Jinkenhi*	「人件費」テーブル情報
//		RET		なし
//********************************************************************************
void	CfrmUc142Jinkenhi::UpdateControlRowTotal( CdbUc142Jinkenhi* prmDbRec ) {
	
	//	頁計/累計の名称表示
	CmnDiagSetString( m_pDiag, 19, prmDbRec->m_KeiStr, 1 );
	//	頁計/累計の金額表示
	CmnDiagSetKingaku( m_pDiag, CmnGetControlTotalIndex( 1, ID_COL_142_VAL ), prmDbRec->m_Val );
	CmnDiagSetKingaku( m_pDiag, CmnGetControlTotalIndex( 1, ID_COL_142_VAL2 ), prmDbRec->m_Val2 );
}

//********************************************************************************
//	その他イベント処理
//********************************************************************************
BOOL	CfrmUc142Jinkenhi::PreTranslateMessage( MSG* pMsg )
{
/*
	int		intID;

	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	if ( pMsg->message == WM_KEYDOWN ) {
		switch ( pMsg->wParam ) {

		// Tabキー（前頁ボタンで[Shift＋Tab]キー処理。これ以外は別のイベントで行う）
		case VK_TAB:
			// 現在フォーカスを持つコントロールのIDを取得
			intID = CmnGetDlgCtrlID();

			// 前頁ボタンで[Shift＋Tab]キーが押されたらカーソルセット
			if ( ( intID == IDC_PAGEBACK_BUTTON1 ) && ( CmnCheckShiftKey() == TRUE ) ) {
				// データ部の最終行へ移動（頁計/累計行の制御を行う）
				CmnDiagSetFocus(m_pDiag, m_uInfo.intRowMax, ID_COL_142_VAL);
				return 1;
			}
			break;
		}
	}
*/
	return CfrmUc000Common::PreTranslateMessage( pMsg );
}
//********************************************************************************
//	頁コントロール処理（Focus，EditOFF，Terminationイベント）
//********************************************************************************

// 頁コントロールのフォーカス設定
void	CfrmUc142Jinkenhi::FocusPagediag1()
{
//マウスクリックでキャレットが表示できないためコメントとする
//	// ボタンからTabキー移動した場合に必要
//	m_PageDiag.SetPosition( 1 );
}

// 頁コントロールのゲットフォーカス
void	CfrmUc142Jinkenhi::EditONPagediag1( short index )
{
	// 1行登録
//	int nCol = CmnGetControlCol();
//	if(( nCol != ID_COL_142_VAL ) || ( nCol != ID_COL_142_VAL2 )){
		if (virRecBufSaveData(m_uInfo.intCurPage, m_uInfo.intCurRow) == FUNCTION_UPDATE) {
			m_PageDiag.SetPosition(1);
		}
//	}
}

// 頁コントロールのロストフォーカス
void	CfrmUc142Jinkenhi::EditOFFPagediag1( short index )
{
// midori 155028 add -->
	// 会社切替時にメッセージを表示した場合、EditOffが走りDB書き込み時にエラーが発生していた
	if(m_EndView != FALSE)	return;
// midori 155028 add <--

	// 頁入力値を取得
	int	intTempPage = CmnDiagGetValue( &m_PageDiag, 1 );

	// 頁が変更された場合は画面更新が必要
	if ( m_uInfo.intCurPage != intTempPage ) {
		if ( ( intTempPage >= 1 ) && ( intTempPage <= m_uInfo.intMaxPage ) ) {
			// 範囲内：新ページに更新
			m_uInfo.intCurPage = intTempPage;

			// １頁の更新
			virUpdateControlTblData();

			// フォーカスをデータ入力欄に
			CmnDiagSetFocus( m_pDiag, 1, 1 );
			return;
		}
	}

	// ページ情報のみ更新："0001"→"1"で表示したい
	UpdateControlPage();
}

// 頁コントロールのキー操作
void	CfrmUc142Jinkenhi::TerminationPagediag1( short index, short nChar, short length, LPUNKNOWN data )
{
	// フォーカス移動
	switch ( nChar ) {
	case VK_RETURN:
		// Return：頁確定のため
		CmnDiagSetFocus( m_pDiag, 1, 1 );
		break;
	case VK_TAB:
		// Tab/Shift+Tabにてフォーカス移動
		if ( CmnCheckShiftKey() == FALSE ) {
			// Tab：タイトル欄へ移動
// midori 160612 cor -->
			//m_TitleDiag.SetPosition( 0 );
// ---------------------
			if(m_DataKakutei == FALSE)	m_TitleDiag.SetPosition(0);
// midori 160612 cor <--
		}
		else {
			// 最終行へ移動
			CmnDiagSetFocus( m_pDiag , ID_COL_142_VAL2 + 1 ,  m_uInfo.intRowMax - 2 );
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
void	CfrmUc142Jinkenhi::FocusTitlediag1()
{
//マウスクリックでキャレットが表示できないためコメントとする
//	// ボタンからTabキー移動した場合に必要
//	m_TitleDiag.SetPosition( 0 );
}

// タイトルコントロールのゲットフォーカス
void	CfrmUc142Jinkenhi::EditONTitlediag1( short index )
{
	// 1行登録
//	int nCol = CmnGetControlCol();
//	if(( nCol != ID_COL_142_VAL ) || ( nCol != ID_COL_142_VAL2 )){
		if (virRecBufSaveData(m_uInfo.intCurPage, m_uInfo.intCurRow) == FUNCTION_UPDATE) {
			m_TitleDiag.SetPosition(0);
		}
//	}
}

// タイトルコントロールのロストフォーカス
void	CfrmUc142Jinkenhi::EditOFFTitlediag1 (short index )
{
	virUpdateTitle();
}

//	タイトル更新
void CfrmUc142Jinkenhi::virUpdateTitle()
{
	// 帳表タイトル取得してuc_inf_subを更新
	m_uInfo.strTitleName = m_clsFunc.DiagGetString( &m_TitleDiag, 0 );
	CmnUcInfSubSetTitleInfo( TRUE );
}

// タイトルコントロールのキーイベント
void	CfrmUc142Jinkenhi::TerminationTitlediag1( short index, short nChar, short length, LPUNKNOWN data )
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
			// 最終行へ移動
			m_PageDiag.SetPosition( 1 );
//			GetDlgItem( IDC_ROWINSERT_BUTTON1 )->SetFocus();
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
//	「人件費」データコントロール処理（EditON，EditOFF，Terminationイベント）
//********************************************************************************

// 「人件費」データコントロールのゲットフォーカス
void	CfrmUc142Jinkenhi::EditONYoushikidiag1( short index )
{
	DIAGRAM_DATA	diadata;

	// フォーカスフラグON
	m_uInfo.intCurFocus = 1;

	// ポジション取得
	m_uInfo.intCurCtlIndex = m_pDiag->GetPosition();

	// 行・列取得
	CmnGetControlPosition( &m_uInfo.intCurRow , &m_uInfo.intCurCol, &m_uInfo.OldCol );

	// 現在の入力値を取得
	m_uInfo.OldstrDiag = m_clsFunc.DiagGetString( m_pDiag, m_uInfo.intCurCtlIndex );

	// 行移動確認
	//int nCol = CmnGetControlCol();
	//if ( ( m_uInfo.OldRow != CmnGetControlRow() ) && ( m_uInfo.OldRow > 0 ) ) {
	//	if ( m_uInfo.OldPage == m_uInfo.intCurPage && m_uInfo.OldPage > 0 ) {
	//		if(( nCol != ID_COL_142_VAL ) || ( nCol != ID_COL_142_VAL2 )){
	//			// カレント行登録＋再表示
	//			if ( virRecBufSaveData(m_uInfo.intCurPage , m_uInfo.OldRow ) == FUNCTION_UPDATE ) {
	//				// カーソル移動処理
	//				CmnDiagSetFocus( m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol );
	//			}
	//		}
	//	}
	//}

	// ボタン操作
	CmnChangeButtonEnable(1);
}

// データコントロールのロストフォーカス
void	CfrmUc142Jinkenhi::EditOFFYoushikidiag1( short index )
{
	if( m_EndView != FALSE )	return;
	virInputDecisionEditOFF();
}

//	入力確定
void CfrmUc142Jinkenhi::virInputDecisionEditOFF()
{
	// フォーカスフラグOFF
	m_uInfo.intCurFocus = 0;

	// 入力項目を強制的に確定
	CmnDiagSetEnterReDraw( m_pDiag, m_pDiag->GetPosition() );

	// 行取得
	if ( m_uInfo.intCurCtlIndex < m_uInfo.intT1StartIndex ) {
		m_uInfo.OldRow = CmnGetControlRow();
	}
	else{
		m_uInfo.OldRow = -1;
	}

	// 金額欄の入力が確定した時点で反映
//	int nCol = CmnGetControlCol();
	if (  m_uInfo.OldRow > 0 ){
		//if(( nCol == ID_COL_142_VAL ) || ( nCol == ID_COL_142_VAL2 )){
			//if ( m_uInfo.OldPage == m_uInfo.intCurPage ) {
				// カレント行登録＋再表示
				virRecBufSaveData(m_uInfo.intCurPage , m_uInfo.OldRow );
			//}
		//}
	}

	// ページ番号取得
	m_uInfo.OldPage = m_uInfo.intCurPage;

	// ボタン操作
	CmnChangeButtonEnable(0);
}

// 預貯金データコントロールのキーイベント
void	CfrmUc142Jinkenhi::TerminationYoushikidiag1( short index, short nChar, short length, LPUNKNOWN data )
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
			m_pDiag->DataClear( m_uInfo.intCurCtlIndex, TRUE );
		break;

	// カーソル移動キー
	case VK_TAB:
	case VK_RETURN:
	case VK_UP:
	case VK_DOWN:
	case VK_LEFT:
	case VK_RIGHT:
	
		//	賃金手当の家族分欄でEnterが押された？
		if (( index == 17 ) && ( nChar == VK_RETURN )){
			// カレント行登録＋再表示
			virRecBufSaveData(m_uInfo.intCurPage , m_uInfo.intCurRow );
		}

		// 例外処理：移動前にComnoBoxで←/→キーが押下された場合は値を元に戻す
		if ( ( nChar == VK_LEFT ) || ( nChar == VK_RIGHT ) ) {
			// ComboBoxは選択内容を確定させない
		}

		// フォーカス移動先を取得
		int intRet = GetFocusPosition( nChar );
		switch ( intRet ) {
		case -1:	// 先頭項目でShift+Tabキーのため、タイトルへ移動
		case -4:	// 先頭行で↑or先頭項目で←は、タイトルへ移動
			m_TitleDiag.SetPosition( 0 );
			break;
		case -2:	// 最終項目でTabキーのため、タイトル項目へ移動
			m_PageDiag.SetPosition( 1 );		// 頁項目
			break;
		case -3:	// ComboBoxで↑↓キーのため何もしない
			break;
		default:	// フォーカス移動を行う
			m_pDiag->SetPosition( intRet );
			break;
		}

		// ポジション取得
		m_uInfo.intCurCtlIndex = m_pDiag->GetPosition();
		break;
	}
}

//********************************************************************************
//	フォーカス移動処理（メイン）
//		IN		int		キーコード（VK_UP，VK_RIGHT，VK_DOWN，VK_LEFT，VK_RETURN，VK_TAB）
//		RET		int		0～:移動先コントロールのインデックス番号
//						-1: 先頭項目で"Shift+Tab"のためタイトルへ移動
//						-2: 最終項目で"Shift"のため別コントロールへ移動
//						-3: ComboBoxで"↑/↓"はキー操作を無視する
//						-4: 先頭行で↑or先頭項目で←はタイトルへ移動
//********************************************************************************
int	CfrmUc142Jinkenhi::GetFocusPosition( int nChar )
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
	
	//---------------------------------------------------------------- 
	//	矢印キーによる移動先を取得（合計欄は次のステップで対応）
	//	基本的にReturn，Tab，Shift+Tabは、←，→キーと同じなので省略
	//	（※特殊行は除く）
	//---------------------------------------------------------------- 
	if ( intCol == ID_COL_142_VAL ){
		
		intVector[0] = m_uInfo.intCurCtlIndex - m_uInfo.intColMax;		// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + m_uInfo.intColMax;		// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - intRowChange;			// ←
	}
	else if ( intCol == ID_COL_142_VAL2 ){
		intVector[0] = m_uInfo.intCurCtlIndex - m_uInfo.intColMax;		// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + intRowChange;			// →
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
		if(m_ReturnMoveDown)	intNext = intVector[2];			//	Enterキーでカーソルを下に移動するチェック時は「↓」キーと同じ
		else					intNext = intVector[1];			// 「Return」キー押下時：「→」キーと同じ
		break;
	case VK_TAB:
		if ( CmnCheckShiftKey() == FALSE ) {
			intNext = intVector[1];		// 「Tab」キー押下時：「→」キーと同じ
		}
		else {
			// 例外処理：先頭項目で"Shift+Tab"は別コントロールへ移動
			// それ以外は「←」キーと同じ
			intNext = intVector[3];		// 「Shift+Tab」キー押下時：「→」キーと同じ
		}
		break;
	}

	if (intNext >= 0) {
		// 最終データ行を越える？
		if (( intNext >= m_uInfo.intT1StartIndex ) && ( nChar == VK_TAB )){
			return -2;
		}
	}
	else {
		// コントロールのフィールド外（上へ）へはタイトルに移動する
		return -4;
	}

	return intNext;
}

/**********************************************************************
	RecBufSetData(CdbUc142Jinkenhi* prmDbRec)
		ローカルのレコードバッファに、1行分のデータを格納する

	引数
		CdbUc142Jinkenhi*	prmDbRec	レコードセット(データがあること）
	戻値
		int		成功/失敗
				FUNCTION_OK
***********************************************************************/
int	CfrmUc142Jinkenhi::RecBufSetData( CdbUc142Jinkenhi* prmDbRec )
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
	m_ReadData[trgRow].m_Val = prmDbRec->m_Val;	// 総額
	m_ReadData[trgRow].m_Val2 = prmDbRec->m_Val2;	// 総額のうち代表者及びその家族分

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
int	CfrmUc142Jinkenhi::RecBufClearAllData()
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
		P_REC_UC_142_JINKENHI inRecData レコードへのポインタ
	戻値
		int		成功/失敗
				FUNCTION_OK
***********************************************************************/
int	CfrmUc142Jinkenhi::RecBufClearData( P_REC_UC_142_JINKENHI inRecData )
{
	inRecData->m_Seq = 0;	// シーケンス番号
	inRecData->m_NumPage = 0;	// 頁番号
	inRecData->m_NumRow = 0;	// 行番号
	inRecData->m_FgFunc = 0;	// 特殊行フラグ
	inRecData->m_FgShow = 0;	// 表示フラグ
	inRecData->m_KeiStr = _T("");	// 計
	inRecData->m_NumGroup = 0;	// グループ番号
	inRecData->m_Val = _T("");	// 総額
	inRecData->m_Val2 = _T("");	// 総額のうち代表者及びその家族分
	inRecData->m_ShowKeiZero = 0;	//０円計表示フラグ

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
int	CfrmUc142Jinkenhi::virRecBufSaveData( int inPage, int inRow )
{
	REC_UC_142_JINKENHI	tmpSaveData;	// 画面データ1レコード格納用

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

	// 総額
	intIndex = CmnGetControlIndex( inRow, ID_COL_142_VAL );
	tmpSaveData.m_Val = CmnDiagGetKingaku( m_pDiag, intIndex );

	// 総額のうち代表者及びその家族分
	intIndex = CmnGetControlIndex( inRow, ID_COL_142_VAL2 );
	tmpSaveData.m_Val2 = CmnDiagGetKingaku( m_pDiag, intIndex );

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
	RecBufSaveDataSub()
		指定された行のデータをテーブルに保存(補助ルーチン)
		※ShowKeiZeroはUpdateCalcKeiで保存している

	引数
			int		inPage		登録するレコードのページ番号
			int		inRow		登録するレコードの行番号
			REC_UC_142_JINKENHI
					inSaveData	登録するデータレコードへのポインタ
	戻値
			int					成功(更新あり)/成功(更新なし)/失敗
								FUNCTION_UPDATE	: 成功(更新あり)
								FUNCTION_OK		: 成功(更新なし)
								FUNCTION_NG		: 失敗
***********************************************************************/
int	CfrmUc142Jinkenhi::RecBufSaveDataSub( 
							int inPage,
							int inRow,
							P_REC_UC_142_JINKENHI inSaveData )
{
	REC_UC_142_JINKENHI		udTemp;

	RecBufClearData( &udTemp );

//	// 変更があったかのフラグ（変更あり:TRUE、なし:FALSE)
	BOOL	ufgUpData = FALSE;	// 変更確認フラグ(全体用データ行/空行)
	BOOL	ufgVal = FALSE;
	BOOL	ufgVal2 = FALSE;
	BOOL	ufgFunc = FALSE;	// 変更確認フラグ(摘要)

	// レコード更新したことを示すフラグ
	BOOL	ufgRsUpdate = FALSE;

	// テーブルアクセスクラス
	CdbUc142Jinkenhi	rs( m_pDB );

	///////////////////////////////////////////////////////////////
	udTemp = *inSaveData;

	///////////////////////////////////////////////////////////////
	// データ行か、空行かのチェック
	udTemp.m_FgFunc = m_ReadData[inRow].m_FgFunc;
	if ( udTemp.m_FgFunc <= ID_FGFUNC_DATA ) {	// データ行 または、空行なら
		// コンポーネントをチェックして、データがあるかを調べる
		// (仮変数に入れた値でチェックしてもいいのかも？)

		if ( ( udTemp.m_Val == "" )		// 画面上のデータ格納用(総額)
		  && ( udTemp.m_Val2 == "" ) ) {		// 画面上のデータ格納用(総額のうち代表者及びその家族分)
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
	//(1) 総額
	if ( udTemp.m_Val != m_ReadData[inRow].m_Val ) {
		ufgUpData = TRUE;		// 全体(データ行/空行)
		ufgVal = TRUE;
	}

	//(2) 総額のうち代表者及びその家族分
	if ( udTemp.m_Val2 != m_ReadData[inRow].m_Val2 ) {
		ufgUpData = TRUE;		// 全体(データ行/空行)
		ufgVal2 = TRUE;
	}

	//(3) 特殊行フラグ(基本的には、空行/データ行のチェックになるはず)
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
		
		// 更新処理
		if ( ufgUpData == TRUE ) {		// テーブルへ更新(1レコード更新)
			// ページ、行 で抽出し、必要なデータを更新
			rs.RequeryPageRow( inPage, inRow );

			if ( !rs.IsEOF() ) {		// レコードが取得できた(取得できないことはないはず)
				rs.Edit();
					
				if ( ufgVal == TRUE ){
					rs.m_Val = udTemp.m_Val;
				}

				if ( ufgVal2 == TRUE ){
					rs.m_Val2 = udTemp.m_Val2;
				}

				// 特殊行フラグは、強制的に書き込む
				rs.m_FgFunc = udTemp.m_FgFunc;

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
				rs.m_KeiStr = _T("");	// 計
				rs.m_NumGroup = 0;	// グループ番号
				rs.m_Val = _T("");	// 総額
				rs.m_Val2 = _T("");	// 総額のうち代表者及びその家族分

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
//	「人件費」テーブルのオブジェクトを生成し、ポインタを返す
//********************************************************************************
CdbUc000Common*	CfrmUc142Jinkenhi::virGetDataTableObject()
{
	// 「人件費」テーブルのオブジェクトを生成し、ポインタを返す
	return (CdbUc000Common*)( new CdbUc142Jinkenhi( m_pDB ) );
}

//********************************************************************************
//	共通処理から帳表のカレント情報を取得
//	IN	CURRENT_INFO	取得する情報の格納領域
//	RET	なし
//********************************************************************************
void	CfrmUc142Jinkenhi::virGetCurrentInfo( CURRENT_INFO* uCurInfo )
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
int	CfrmUc142Jinkenhi::virUpdateRelationTblChengeFormType()
{
	// ⑭-2で通常/個別を変更した場合、⑭-1の頁構成の更新が必要となる
	CfrmUc141Yakuin		pfrm141;
	int intRet = pfrm141.UpdateTbl141ChangeFormType();

	return intRet;
}

//********************************************************************************
//	通常/個別切り替え時の頁構成を更新（⑭-1から実行される処理）
//		IN		なし
//		RET		なし
//********************************************************************************
int	CfrmUc142Jinkenhi::UpdateTbl142ChangeFormType()
{
	// 初期値設定
	CmnSetInitialUchiwakeInfo(ID_FORMNO_142);	// 共通データ
	virSetInitialValue();						// 帳表固有データ

	// データ無しならデータ更新しない
	if (m_uInfo.intMaxPage <= 0) {
		return FUNCTION_OK;
	}

	// 頁と行の再設定
	CmnTblUpdatePageRowAllResetMain();

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
void CfrmUc142Jinkenhi::SetInputMode( int nRow, long nMode )
{
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_142_VAL ), nMode );		//	総額
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_142_VAL2 ), nMode );		//	総額のうち代表者及びその家族分
}

//**************************************************
//	各Diagの背景色(BackColor)に色を設定
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CfrmUc142Jinkenhi::SetDiagBackColor()
{
	// 入力画面の各Diagの背景色を共通設定色に合わせる
	m_PageDiag.SetBackColor( m_ViewColor );
	m_TitleDiag.SetBackColor( m_ViewColor );
	m_NumberDiag.SetBackColor( m_ViewColor );
	m_HeadDiag.SetBackColor( m_ViewColor );
	m_Diag.SetBackColor( m_ViewColor );
}

//********************************************************************************
//	財務連動：連動データの作成（科目残高集計）
//		IN		CdbUc000Common*		内訳書テーブル情報
//				RENTEMP_INFO*		財務連動情報
//		RET		なし					
//********************************************************************************
void CfrmUc142Jinkenhi::virRendoSumData(CdbUc000Common* rsData, RENTEMP_INFO* uRenInfo)
{
	// 科目残高集計からのデータ作成は、他の科目残高、枝番残高、仕訳データからの
	// データ作成処理とは、処理内容が異なるため、別関数名とする
	CdbUcLstKamoku	rsLstKmk(m_pDB);
	int				intRow = 0;
	CString			strTemp;

	CdbUc142Jinkenhi*	rs = (CdbUc142Jinkenhi*)rsData;

	// 「⑭-2人件費」は３行固定で、適切な位置に加算する
	// uc_lst_kamokuのKnOrderに集計行がセットされている（アプリで変更不可）

	// 集計位置（行）を取得
	rsLstKmk.RequeryKnSeq(m_uInfo.intFormSeq, uRenInfo->lngKnSeq);
	if (!rsLstKmk.IsEOF()) {
		intRow = rsLstKmk.m_KnOrder;
	}
	rsLstKmk.Fin();

	// 対象行の範囲チェック
	if ((intRow > 0) && (intRow <= m_uInfo.intRowMaxData)) {
		// 集計行のデータ取得
		rsData->RequeryPageRow(1, intRow);
		if (!rsData->IsEOF()) {
			// 金額を加算
			rsData->Edit();

			strTemp = uRenInfo->strVal;
			CmnKingakuAddFunc(&rs->m_Val, strTemp, 1);		// 金額を加算

			rsData->Update();
		}
	}
}


//***************************************************************************
//（2009.09.08 TS自動ログオフ対応）
//	Viewの終了処理
//		・デストラクタで行っていた処理をここに移動
//
//	IN		なし
//	RET		BOOL				TRUE=エラー，FALSE=正常
//***************************************************************************
BOOL CfrmUc142Jinkenhi::virEndProc(void)
{
	TRACE(_T("***** CfrmUc142Jinkenhi::virEndProc\n"));

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

// midori 160612 add -->
//********************************************************************************
//	ICS-Controlの対象行の有効/無効を切り替える
//********************************************************************************
void CfrmUc142Jinkenhi::fm142DiagSetAttrSpecialRow( void )
{
	DIAGRAM_ATTRIBUTE	diaatt;				// ICSDiag属性構造体
	COLORREF			color;				// RGB構造体
	int					ii=0;
// midori 160612 add -->
	BOOL				bedit=FALSE;
// midori 160612 add <--

	// 背景色取得＆指定
	if(m_DataKakutei)	{
		color = BC_DISABLE;
	}
	else	{
		color = (COLORREF)RGB(0xff,0xff,0xff);	//	白色
	}

// midori 160612 cor -->
	if(m_DataKakutei == FALSE)	bedit=TRUE;
	else						bedit=FALSE;
// midori 160612 cor <--

	for(ii=2;ii<=17;ii++)	{
		if(ii == 6 || ii == 7 || ii == 12 || ii == 13)	continue;
		// 属性を取得
		m_Diag.GetAttribute(ii, (LPUNKNOWN)&diaatt);
		// 背景色を設定
		memcpy(&diaatt.attr_bcolor, &color, sizeof(COLORREF));
		// 属性を変更
		m_Diag.SetAttribute(ii, (LPUNKNOWN)&diaatt, TRUE);	
// midori 160612 cor -->
		// 入力可／不可の設定
		DlgSet_Attr(&m_Diag,ii,bedit);
// midori 160612 cor <--
	}
}
// midori 160612 add <--
