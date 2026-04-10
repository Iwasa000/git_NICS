//--------------------------------------------------
//	frmUc081Siharaitegata.cpp
//--------------------------------------------------

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "frmUc081Siharaitegata.h"


IMPLEMENT_DYNCREATE( CfrmUc081Siharaitegata, CfrmUc000Common )


BEGIN_MESSAGE_MAP( CfrmUc081Siharaitegata, CfrmUc000Common )
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
	ON_COMMAND( ID_PRINTSETUP_MENU, &CfrmUc081Siharaitegata::OnMenuPrintSetup )
	ON_COMMAND( ID_PRINTALL_MENU, &CfrmUc081Siharaitegata::OnMenuPrintAll )
	//ON_COMMAND( ID_OUTPUTFORM_MENU, &CfrmUc081Siharaitegata::OnMenuFormTypeSetup )
	ON_COMMAND( ID_SPECIALROW_MENU, &CfrmUc081Siharaitegata::OnMenuSpecialRow )
	ON_COMMAND( ID_SORT_MENU, &CfrmUc081Siharaitegata::OnMenuSort )
	ON_COMMAND(ID_IKKATUADD_MENU, &CfrmUc081Siharaitegata::OnIkkatuAddMenu)
	ON_COMMAND( ID_DISPDIALOG_MENU, &CfrmUc081Siharaitegata::OnMenuIkkatuKingaku )
	ON_COMMAND( ID_ZEROMONEY_MENU, &CfrmUc081Siharaitegata::OnMenuZeroMoney )
	ON_COMMAND( ID_ZEROMONEYMOVE_MENU, &CfrmUc081Siharaitegata::OnMenuZeroMoneyMove )
	ON_COMMAND( ID_AMOUTDIALOG_MENU, &CfrmUc081Siharaitegata::OnMenuOutKeiSetup )
// midori 160606 add -->
	ON_COMMAND( ID_CLEAR_MENU, &CfrmUc081Siharaitegata::OnMenuClear )
// midori 160606 add <--
	ON_COMMAND( ID_RENDO_MENU, &CfrmUc081Siharaitegata::OnMenuZaimuRendo )
	ON_COMMAND( ID_TITLEMENU, &CfrmUc081Siharaitegata::OnMenuTitleRestore )
	ON_COMMAND( ID_USER_SETTING_MENU, &CfrmUc081Siharaitegata::OnMenuOptionSetting )
	ON_COMMAND( ID_COPY_SETTING_INFO_MENU, &CfrmUc081Siharaitegata::OnMenuCopySettingInfo )
	ON_COMMAND( ID_DELETE_DATA_MENU, &CfrmUc081Siharaitegata::OnMenuDeleteDataEachYoushiki )
	ON_BN_CLICKED( IDC_PAGEBACK_BUTTON1, &CfrmUc081Siharaitegata::OnPageBack )
	ON_BN_CLICKED( IDC_PAGENEXT_BUTTON1, &CfrmUc081Siharaitegata::OnPageNext )
	ON_BN_CLICKED( IDC_PAGENEW_BUTTON1, &CfrmUc081Siharaitegata::OnPageNew )
	ON_BN_CLICKED( IDC_PAGEINSERT_BUTTON1, &CfrmUc081Siharaitegata::OnPageInsert )
	ON_BN_CLICKED( IDC_ROWCOPY_BUTTON1, &CfrmUc081Siharaitegata::OnRowCopy )
	ON_BN_CLICKED( IDC_ROWPASTE_BUTTON1, &CfrmUc081Siharaitegata::OnRowPaste )
	ON_BN_CLICKED( IDC_ROWINSERT_BUTTON1, &CfrmUc081Siharaitegata::OnRowInsert )
	ON_BN_CLICKED( IDC_YOUSHIKINEXT_BUTTON1, &CfrmUc081Siharaitegata::OnMoveYoushikiNext )
	ON_BN_CLICKED( IDC_YOUSHIKIBACK_BUTTON1, &CfrmUc081Siharaitegata::OnMoveYoushikiBack )
//	ON_UPDATE_COMMAND_UI( ID_BUTTON_F1, &CfrmUc081Siharaitegata::OnUpdateButtonF1 )
//	ON_UPDATE_COMMAND_UI( ID_BUTTON_F3, &CfrmUc081Siharaitegata::OnUpdateButtonF3 )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_F6, &CfrmUc081Siharaitegata::OnUpdateButtonF6 )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_F7, &CfrmUc081Siharaitegata::OnUpdateButtonF7 )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_F8, &CfrmUc081Siharaitegata::OnUpdateButtonF8 )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_F9, &CfrmUc081Siharaitegata::OnUpdateButtonF9 )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_F11, &CfrmUc081Siharaitegata::OnUpdateButtonF11 )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_F12, &CfrmUc081Siharaitegata::OnUpdateButtonF12 )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_HOME, &CfrmUc081Siharaitegata::OnUpdateButtonHome )
	ON_UPDATE_COMMAND_UI( ID_IKKATUADD_MENU, &CfrmUc081Siharaitegata::OnUpdateIkkatuaddMenu)
	ON_UPDATE_COMMAND_UI( ID_DISPDIALOG_MENU, &CfrmUc081Siharaitegata::OnUpdateMenuIkkatuKingaku )
// midori 160612 add -->
	ON_UPDATE_COMMAND_UI( ID_SORT_MENU, &CfrmUc081Siharaitegata::OnUpdateMenuSort )
	ON_UPDATE_COMMAND_UI( ID_CLEAR_MENU, &CfrmUc081Siharaitegata::OnUpdateMenuClear )
// midori 160612 add <--
	ON_UPDATE_COMMAND_UI( ID_ZEROMONEY_MENU, &CfrmUc081Siharaitegata::OnUpdateMenuZeroMoney )
// midori 160612 add -->
	ON_UPDATE_COMMAND_UI( ID_ZEROMONEYMOVE_MENU, &CfrmUc081Siharaitegata::OnUpdateMenuZeroMoneyMove)
// midori 160612 add <--
	ON_UPDATE_COMMAND_UI( ID_RENDO_MENU, &CfrmUc081Siharaitegata::OnUpdateMenuZaimuRendo )
	ON_UPDATE_COMMAND_UI(ID_SPECIALROW_MENU, &CfrmUc081Siharaitegata::OnUpdateMenuSpecialRow)
// midori 160612 add -->
	ON_UPDATE_COMMAND_UI( ID_TITLEMENU, &CfrmUc081Siharaitegata::OnUpdateMenuTitleRestore )
	ON_UPDATE_COMMAND_UI( ID_DELETE_DATA_MENU, &CfrmUc081Siharaitegata::OnUpdateMenuDeleteDataEachYoushiki )
// midori 160612 add <--
	ON_MESSAGE(WM_USER_REDRAWVIEW, OnUserReDrawView)		// 修正No.162446 add
END_MESSAGE_MAP()


CfrmUc081Siharaitegata::CfrmUc081Siharaitegata()
	: CfrmUc000Common( CfrmUc081Siharaitegata::IDD )
{
	m_pDB = ( (CUCHIWAKEApp*)AfxGetApp() )->m_pDB;
	m_pTitleDiag = &m_TitleDiag;
// midori 152718 add -->
	m_pPageDiag	= &m_PageDiag;
// midori 152718 add <--
}

CfrmUc081Siharaitegata::~CfrmUc081Siharaitegata()
{
	// 2009.09.08 TS自動ログオフ対応(virEndProcへ移動)
	TRACE(_T("***** CfrmUc081Siharaitegata::~CfrmUc081Siharaitegata\n"));

	//m_pDiag = NULL;
	//m_pTitleDiag = NULL;
}

void CfrmUc081Siharaitegata::DoDataExchange( CDataExchange* pDX )
{
	CfrmUc000Common::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_YOUSHIKIDIAG1, m_Diag1 );
	DDX_Control( pDX, IDC_YOUSHIKIDIAG2, m_Diag2 );
	DDX_Control( pDX, IDC_YOUSHIKIDIAG_KEI1, m_DiagKei1 );
	DDX_Control( pDX, IDC_YOUSHIKIDIAG_KEI2, m_DiagKei2 );
	DDX_Control( pDX, IDC_TITLEDIAG1, m_TitleDiag );
	DDX_Control( pDX, IDC_PAGEDIAG1, m_PageDiag );
	DDX_Control( pDX, IDC_NUMBER_DIAG1, m_NumberDiag );
	DDX_Control( pDX, IDC_TOPICDIAG1, m_HeadDiag);
// midori 161108 add -->
	DDX_Control(pDX, IDC_SAVEDATA_DIAG1, m_SaveDataDiag);
// midori 161108 add <--
}

void CfrmUc081Siharaitegata::OnInitialUpdate()
{
	CfrmUc000Common::OnInitialUpdate();
	ResizeParentToFit();

	// 帳表テーブルのオープンOK?
	if ( m_InitialFlag == TRUE ){
		// リサイズ
		//SetResize( IDC_YOUSHIKIDIAG1 , ICS_RESIZE_BOTTOM );

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
void CfrmUc081Siharaitegata::AssertValid() const
{
	CfrmUc000Common::AssertValid();
}

#ifndef _WIN32_WCE
void CfrmUc081Siharaitegata::Dump( CDumpContext& dc ) const
{
	CfrmUc000Common::Dump( dc );
}
#endif
#endif //_DEBUG

BEGIN_EVENTSINK_MAP( CfrmUc081Siharaitegata, CfrmUc000Common )
	ON_EVENT( CfrmUc081Siharaitegata, IDC_YOUSHIKIDIAG1, 3, CfrmUc081Siharaitegata::TerminationYoushikidiag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN )
	ON_EVENT( CfrmUc081Siharaitegata, IDC_YOUSHIKIDIAG1, 2, CfrmUc081Siharaitegata::EditOFFYoushikidiag1, VTS_I2 )
	ON_EVENT( CfrmUc081Siharaitegata, IDC_YOUSHIKIDIAG1, 1, CfrmUc081Siharaitegata::EditONYoushikidiag1, VTS_I2 )
// midori 151405 add -->
	ON_EVENT(CfrmUc081Siharaitegata, IDC_YOUSHIKIDIAG1, 12, CfrmUc081Siharaitegata::VScrollYoushikidiag1, VTS_I2)
// midori 151405 add <--

	ON_EVENT( CfrmUc081Siharaitegata, IDC_YOUSHIKIDIAG2, 3, CfrmUc081Siharaitegata::TerminationYoushikidiag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN )
	ON_EVENT( CfrmUc081Siharaitegata, IDC_YOUSHIKIDIAG2, 2, CfrmUc081Siharaitegata::EditOFFYoushikidiag1, VTS_I2 )
	ON_EVENT( CfrmUc081Siharaitegata, IDC_YOUSHIKIDIAG2, 1, CfrmUc081Siharaitegata::EditONYoushikidiag1, VTS_I2 )
// midori 151405 add -->
	ON_EVENT(CfrmUc081Siharaitegata, IDC_YOUSHIKIDIAG2, 12, CfrmUc081Siharaitegata::VScrollYoushikidiag1, VTS_I2)
// midori 151405 add <--

	ON_EVENT( CfrmUc081Siharaitegata, IDC_TITLEDIAG1, 3, CfrmUc081Siharaitegata::TerminationTitlediag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN )
	ON_EVENT( CfrmUc081Siharaitegata, IDC_TITLEDIAG1, 2, CfrmUc081Siharaitegata::EditOFFTitlediag1, VTS_I2 )
	ON_EVENT( CfrmUc081Siharaitegata, IDC_TITLEDIAG1, 1, CfrmUc081Siharaitegata::EditONTitlediag1, VTS_I2 )
	ON_EVENT( CfrmUc081Siharaitegata, IDC_TITLEDIAG1, 6, CfrmUc081Siharaitegata::FocusTitlediag1, VTS_NONE )

	ON_EVENT( CfrmUc081Siharaitegata, IDC_PAGEDIAG1, 3, CfrmUc081Siharaitegata::TerminationPagediag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN )
	ON_EVENT( CfrmUc081Siharaitegata, IDC_PAGEDIAG1, 2, CfrmUc081Siharaitegata::EditOFFPagediag1, VTS_I2 )
	ON_EVENT( CfrmUc081Siharaitegata, IDC_PAGEDIAG1, 1, CfrmUc081Siharaitegata::EditONPagediag1, VTS_I2 )
	ON_EVENT( CfrmUc081Siharaitegata, IDC_PAGEDIAG1, 6, CfrmUc081Siharaitegata::FocusPagediag1, VTS_NONE )
END_EVENTSINK_MAP()

//********************************************************************************
//	初期処理
//********************************************************************************
BOOL	CfrmUc081Siharaitegata::virStartJob()
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
void	CfrmUc081Siharaitegata::virSetInitialUchiwakeInfo()
{
	// 「支払手形」の固有データ
	m_uInfo.intOutKeiDialog = AM_TYPE_ALL;			// 計設定ダイアログのボタン指定
	m_uInfo.intRowMax = ID_ROWKOBETU_081;			// １頁内の合計行を含む全体の行数
	m_uInfo.intRowMaxData = ID_ROWKOBETU_081 - 1;	// １頁内のデータ行数（合計行は含まず）
	m_uInfo.intColMax = ID_COLMAX_081;				// １行内のコントロール個数
	m_uInfo.intColMaxData = ID_COLMAXDATA_081;		// １行内の列数
	m_uInfo.intCtlStartIndex = ID_CTLSTARTINDEX_081;// １行目の入力項目の先頭インデックス番号
	m_uInfo.intT1StartIndex = ID_T1STARTNORMAL_081;	// 合計行①の先頭インデックス番号
	m_uInfo.intT2StartIndex = ID_T2STARTNORMAL_081;	// 合計行②の入力項目の先頭インデックス番号
	m_uInfo.intCursolDefPos = ID_COL_081_ADNAME1;	// カーソル移動時の初期位置（タイトルから↓，合計から↑で使用）
	m_uInfo.intCursolKeiPos = ID_COL_081_ADNAME1;	// 合計行のカーソル移動位置（カーソル移動で使用）

	m_uInfo.intFormSeq = ID_FORMNO_081;				// 様式シーケンス番号
	m_uInfo.intFormNo = 8;							// 様式番号（uc_inf_sub）
	m_uInfo.intFormNo2 = 1;							// 様式番号枝番（uc_inf_sub）


	m_uInfo.intRowNormal = ID_ROWNORMAL_081;		// 通常時：１頁内の合計行を含む全体の行数
	m_uInfo.intRowKobetu = ID_ROWKOBETU_081;		// 個別時：１頁内の合計行を含む全体の行数
	m_uInfo.intT1StartNormal = ID_T1STARTNORMAL_081;	// 通常時：合計行①の先頭インデックス
	m_uInfo.intT2StartNormal = ID_T2STARTNORMAL_081;	// 通常時：合計行②の先頭インデックス
	m_uInfo.intT1StartKobetu = ID_T1STARTKOBETU_081;	// 個別時：合計行①の先頭インデックス
	m_uInfo.intT2StartKobetu = ID_T2STARTKOBETU_081;	// 個別時：合計行②の先頭インデックス
	m_uInfo.intRowKei = ID_ROWKEI_081;				// 帳表固定の合計行数（1:合計あり，0:なし）
	m_uInfo.intKei1StartIndex = ID_COL_081_KEI1_MEISYO;	// 計ダイアグラム1行目の先頭インデックス
	m_uInfo.intKei2StartIndex = ID_COL_081_KEI2_MEISYO;	// 計ダイアグラム2行目の先頭インデックス

	// 集計処理の設定
	m_uInfo.uCalcInfo.intMaxCount = ID_CALC_081_MAX;		// 集計フィールド件数
	m_uInfo.uCalcInfo.strField[0] = ID_CALC_081_FIELD1;		// 集計フィールド名称1

/************************************************
	// Test：コントロールをComboBox→EditBoxへ変更
	// １行目ComboBoxの属性をバックアップ
	int	intIndex = CmnGetControlIndex( 1, ID_COL_XXX_XXXXXX );
	m_pDiag->GetAttribute( intIndex, (LPUNKNOWN)&m_uInfo.uComboAttr );
************************************************/
}

//**************************************************
// ICSDiagコントロールを通常／個別で切り替える
// 【引数】    なし
// 【戻値】    なし
//**************************************************
void	CfrmUc081Siharaitegata::virSetInitialDiag()
{
	// オブジェクト未生成時は処理を抜ける
	if ( (m_Diag1.m_hWnd == NULL) || (m_Diag2.m_hWnd == NULL)){
		return;
	}

	//表示するダイアログの設定
	m_Diag		= CmnGetDiagType( &m_Diag1, &m_Diag2 );
	m_DiagKei	= CmnGetDiagType( &m_DiagKei1, &m_DiagKei2 );

	// 親クラスのダイアログにコピー
	m_pDiag		= m_Diag;
	m_pDiagKei	= m_DiagKei;

	// 入力不可行でのDeleteキー無効
	m_pDiag->EnableDelete(0);

}

//********************************************************************************
//	帳表固有変数の初期値設定
//********************************************************************************
void	CfrmUc081Siharaitegata::virSetInitialValue()
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
BOOL	CfrmUc081Siharaitegata::InitialControlDataArea()
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

		// 漢字入力欄の初期化
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_081_ADNAME1    - 1, MAX_KETA_081_ADNAME1 );	// 支払先1
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_081_SPDATE    - 1, MAX_KETA_081_SPDATE );		// 振出年月日
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_081_PMDATE    - 1, MAX_KETA_081_PMDATE );		// 支払期日
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_081_BKNAME1    - 1, MAX_KETA_081_BKNAME1 );	// 支払銀行名
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_081_BKNAME2    - 1, MAX_KETA_081_BKNAME2 );	// 支払銀行名(支店名)
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_081_TEKI    - 1, MAX_KETA_081_TEKI );			// 摘要

		// 金額欄：桁数初期化
		CmnDiagSetAttrDigit( m_pDiag, i + ID_COL_081_VAL - 1, ID_VAL_081_DATA );	// 金額

		//	入力不可モードの解除
		SetInputMode( CmnGetControlRow( i ), DIAG_MDFY_INPUT );
	}

	// ComboBoxの初期設定
	if ( InitialControlComboBox() != TRUE ){
		return FALSE;
	}

	// 半角/全角設定
	CmnDiagSetAttrColZenHan( m_pDiag , ID_COL_081_ADNAME1 );		// 支払先1
	CmnDiagSetAttrColZenHan( m_pDiag , ID_COL_081_BKNAME1 );		// 支払銀行名
	CmnDiagSetAttrColZenHan( m_pDiag , ID_COL_081_BKNAME2 );		// 支払支店名
	CmnDiagSetAttrColZenHan( m_pDiag , ID_COL_081_TEKI );		// 摘要

	return TRUE;
}

//********************************************************************************
//	帳表コントロールの初期化（ComboBox）
//********************************************************************************
BOOL	CfrmUc081Siharaitegata::InitialControlComboBox()
{
	// コンボボックスを初期化する関数です。
	return TRUE;
}
//********************************************************************************
//	帳表コントロールの初期化（頁計/累計領域）
//		文言はデータに含まれているので、その内容を使用する
//********************************************************************************
void	CfrmUc081Siharaitegata::InitialControlTotalArea()
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
		//if(m_DataKakutei == FALSE)	nDataRow = (DIAG_MDFY_EDIT | DIAG_MDFY_OPAQUE);
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
		else						nDataRow = (DIAG_MDFY_READONLY | DIAG_MDFY_OPAQUE);
// midori 160612 cor <--
		//	計行の設定
		nKeiRow[0] = ( DIAG_MDFY_READONLY | DIAG_MDFY_TRANSPARENT );
		//nKeiRow[1] = ( DIAG_MDFY_READONLY | DIAG_MDFY_OPAQUE );
		// 28行目の合計行①
		nStart = m_uInfo.intT1StartIndex;
		for ( nCnt = 0; nCnt < m_uInfo.intColMax; nCnt++ ){
			m_pDiag->ModifyItem( nStart + nCnt, nKeiRow[0] );
		}
		break;
	case ID_OUTKEI_BOTH:			// 頁計と累計
		//	データ行の設定
		nDataRow = ( DIAG_MDFY_READONLY | DIAG_MDFY_TRANSPARENT );
		//	計行の設定
		nKeiRow[0] = ( DIAG_MDFY_READONLY | DIAG_MDFY_OPAQUE );
		//nKeiRow[1] = ( DIAG_MDFY_READONLY | DIAG_MDFY_OPAQUE );
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
// midori 152166 add -->
	if(nSw != 0)	{
		// 現在のカーソルポジションが入力不可になった場合は先頭に戻す
		m_uInfo.intCurRow = 1;
		m_uInfo.intCurCol = 1;
	}
// midori 152166 add <--

	// 計専用ダイアグラムの金額欄を入力不可にする
	int intKeiRow = (DIAG_MDFY_READONLY | DIAG_MDFY_OPAQUE);
	m_pDiagKei->ModifyItem(ID_COL_081_KEI1_VAL, intKeiRow);
	m_pDiagKei->ModifyItem(ID_COL_081_KEI2_VAL, intKeiRow);
	
	//// 29行目の合計行②
	//nStart = m_uInfo.intT2StartIndex;
	//for ( nCnt = 0; nCnt < m_uInfo.intColMax; nCnt++ ){
	//	m_pDiag->ModifyItem( nStart + nCnt, nKeiRow[1] );
	//}

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
	
	// 入力可能（ただし、画面レイアウトで入力文字数"0"としているため、実際は入力できない）
	//m_pDiag->ModifyItem( CmnGetControlTotalIndex( 1, ID_COL_081_ADNAME1 ), nKeiRow[0] );
	//m_pDiag->ModifyItem( CmnGetControlTotalIndex( 2, ID_COL_081_ADNAME1 ), nKeiRow[1] );
	
	// 合計行の金額欄は、金額欄 + 1桁
	CmnDiagSetAttrDigit( m_pDiagKei, ID_COL_081_KEI1_VAL, ID_VAL_081_TOTAL );
	CmnDiagSetAttrDigit( m_pDiagKei, ID_COL_081_KEI2_VAL, ID_VAL_081_TOTAL );
	//CmnDiagSetAttrDigit( m_pDiag, CmnGetControlTotalIndex( 1, ID_COL_081_VAL ), ID_VAL_081_TOTAL );
	//CmnDiagSetAttrDigit( m_pDiag, CmnGetControlTotalIndex( 2, ID_COL_081_VAL ), ID_VAL_081_TOTAL );
}

//********************************************************************************
//	帳表コントロールの特殊行初期化
//	IN	int		行番号
//		int		特殊行フラグ（FgFunc）
//	RET	なし
//********************************************************************************
void	CfrmUc081Siharaitegata::InitialControlSpecialRow( int intRow, int intFgFunc )
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
		CmnDiagSetAttrDigit( m_pDiag, CmnGetControlIndex( intRow , ID_COL_081_VAL ), ID_VAL_081_TOTAL );
		break;
	}

	// 特殊行は入力不可とする
	switch (intFgFunc) {
	case ID_FGFUNC_SYOKEI:			// 小計
	case ID_FGFUNC_SYOKEINULL:		// 小計（空行）
	case ID_FGFUNC_CHUKEI:			// 中計
	case ID_FGFUNC_CHUKEINULL:		// 中計（空行）
	case ID_FGFUNC_IKKATUAUTO:		// 一括集計金額行（自動）

		//	入力不可モードの設定
// midori 160612 cor -->
		//SetInputMode( intRow, DIAG_MDFY_NOINPUT );
// ---------------------
		if(m_DataKakutei == FALSE)	SetInputMode(intRow,DIAG_MDFY_NOINPUT);
		else						SetInputMode(intRow,DIAG_MDFY_READONLY);
// midori 160612 cor <--
		break;
	/************************************************
		// Test：コントロールをComboBox→EditBoxへ変更
		CmnDiagSetAttrComboToEdit( m_pDiag, CmnGetControlIndex( intRow, ID_COL_XXX_XXXXXX ), FALSE );
	************************************************/
	}
}

//********************************************************************************
//	帳表タイトル情報の表示
//********************************************************************************
void	CfrmUc081Siharaitegata::virUpdateControlTitle()
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
}

//********************************************************************************
//	帳表ページ情報の表示
//********************************************************************************
void	CfrmUc081Siharaitegata::UpdateControlPage()
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
BOOL	CfrmUc081Siharaitegata::virUpdateControlTblData()
{
	// ダイアログ切替
	if( m_pDiag != NULL ){
		virSetInitialDiag();
	}

	CdbUc081Siharaitegata	dbRec( m_pDB );
	
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
		else if ( dbRec.m_FgFunc != ID_FGFUNC_NULL ){
			// データ行の表示（空行，特殊行を含む）
			UpdateControlRowData( &dbRec );
		}

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

	// 画面更新
	m_pDiag->Refresh();
	m_pDiagKei->Refresh();

	return TRUE;
}

// midori 161108 add -->
//********************************************************************************
//	保管件数の表示
//********************************************************************************
BOOL CfrmUc081Siharaitegata::virUpdateControlTblData2()
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
//		IN		CdbUc081Siharaitegata*	「支払手形」
//		RET		なし
//********************************************************************************
void	CfrmUc081Siharaitegata::UpdateControlRowData( CdbUc081Siharaitegata* prmDbRec ) {
	DIAGRAM_DATA	diadata;	// ICSDiag構造体
	int				intCell;	// 保存フィールド

	// 初期化
	m_clsFunc.DiagInit( &diadata );

	// 読込み対象セル取得
	intCell = CmnGetControlIndex( prmDbRec->m_NumRow, ID_COL_081_ADNAME1 );

	// 支払先1
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
		diadata.data_edit = m_clsFunc.StrDocking( prmDbRec->m_AdName1 , prmDbRec->m_AdName2 );
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

	CdateConvert	dc;
	// 振出年月日
	dc.Convert( prmDbRec->m_SpDate, diadata.data_day, DC_DATE_WMD );
	if ( dc.m_nDate != 0 ) {
		m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	}
	else {
		m_pDiag->DataClear( intCell , TRUE );
	}
	intCell++;

	// 支払期日
	dc.Convert( prmDbRec->m_PmDate, diadata.data_day, DC_DATE_WMD );
	if ( dc.m_nDate != 0 ) {
		m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	}
	else {
		m_pDiag->DataClear( intCell , TRUE );
	}
	intCell++;

	// 支払銀行名
	diadata.data_edit = prmDbRec->m_BkName1;
	m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	intCell++;

	// 支払支店名
	diadata.data_edit = prmDbRec->m_BkName2;
	m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	intCell++;

	// 金額
	if ( prmDbRec->m_Val != "" ) {
		CmnChangeFieldValData( &diadata.data_val[0], prmDbRec->m_Val );
		m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	}
	else {
		m_pDiag->DataClear( intCell, TRUE );
	}
	intCell++;

	// 摘要
	diadata.data_edit = prmDbRec->m_Teki;
	m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
}

//********************************************************************************
//	帳表データの１行表示（累計，頁計行）
//		IN		CdbUc081Siharaitegata*	「支払手形」テーブル情報
//		RET		なし
//********************************************************************************
void	CfrmUc081Siharaitegata::UpdateControlRowTotal( CdbUc081Siharaitegata* prmDbRec )
{
	int		intTotalType;
	int		intRowName = 0;		// 名称表示行（0:非表示）
	int		intRowVal = 0;		// 金額表示行（0:非表示）

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
			intRowVal = 2;
		}
		break;
	case ID_OUTKEI_RUIKEI:
		// カレント頁："累計"なら、合計行②に"累計"を表示
		if ( prmDbRec->m_FgFunc == ID_FGFUNC_RUIKEI ) {
			intRowName = 2;
			intRowVal = 2;
		}
		break;
	case ID_OUTKEI_BOTH:
		// カレント頁："頁計＋累計"なら、合計行①に"頁計"＋合計行②に"累計"を表示
		if ( prmDbRec->m_FgFunc == ID_FGFUNC_PAGEKEI ) {
			intRowName = 1;
			intRowVal = 1;
		}
		else if ( prmDbRec->m_FgFunc == ID_FGFUNC_RUIKEI ) {
			intRowName = 2;
			intRowVal = 2;
		}
		break;
	}

	// 頁計/累計の名称表示
	if ( intRowName > 0 ) {
		//CmnDiagSetString( m_pDiag, CmnGetControlTotalIndex( intRowName, ID_COL_081_ADNAME1 ), prmDbRec->m_KeiStr, 0 );
		CmnDiagSetString( m_pDiagKei, CmnGetKeiDiagIndex( intRowName, ID_COL_081_KEI1_MEISYO ), prmDbRec->m_KeiStr, 1 );
	}
	// 頁計/累計の金額表示
	if ( intRowVal > 0 ) {
		//CmnDiagSetKingaku( m_pDiag, CmnGetControlTotalIndex( intRowName, ID_COL_081_VAL ), prmDbRec->m_Val );
		CmnDiagSetKingaku( m_pDiagKei, CmnGetKeiDiagIndex( intRowVal, ID_COL_081_KEI1_VAL ), prmDbRec->m_Val );
	}
}

//********************************************************************************
//	【F6:項目複写】１つ前のデータ表示処理
//		IN		CdbUc000Common*	共通テーブルクラス
//		RET		なし
//********************************************************************************
void	CfrmUc081Siharaitegata::virFncCellCopyProc( CdbUc000Common* rsData )
{
	CdbUc081Siharaitegata*	rs;
	int					intCol;	// 現在の列

	rs = (CdbUc081Siharaitegata*)rsData;

	// 列取得
	intCol = CmnGetControlCol();

	// 列によって処理を分岐
	switch( intCol ){

	// 支払先1
	case ID_COL_081_ADNAME1:
		CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, m_clsFunc.StrDocking( rs->m_AdName1,rs->m_AdName2 ) , 0 );
		break;
	// 振出年月日
	case ID_COL_081_SPDATE:
		CmnDiagSetDateWMD( m_pDiag, m_uInfo.intCurCtlIndex, DC_DATE_WMD, rs->m_SpDate );
		break;
	// 支払期日
	case ID_COL_081_PMDATE:
		CmnDiagSetDateWMD( m_pDiag, m_uInfo.intCurCtlIndex, DC_DATE_WMD, rs->m_PmDate );
		break;
	// 支払銀行名
	case ID_COL_081_BKNAME1:
		CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_BkName1, 0 );
		break;
	// 支払支店名
	case ID_COL_081_BKNAME2:
		CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_BkName2, 0 );
		break;
	// 金額
	case ID_COL_081_VAL:
		CmnDiagSetKingaku( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_Val );
		break;
	// 摘要
	case ID_COL_081_TEKI:
		CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_Teki, 0 );
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
//									ID_DLGTYPE_NONE		：参照不可
//********************************************************************************
int	CfrmUc081Siharaitegata::virFncReferenceGetType(int intCol, int* intGrSeq, long* lngItemSeq, int* intAdd1, int* intAdd2, int* intAdd1Col, int* intAdd2Col, REFERENCE_INFO* pudReference)
{
	int		intRefType;

	switch ( intCol ) {

	// 支払先1・支払先2
	case ID_COL_081_ADNAME1:
		intRefType = ID_DLGTYPE_ADDRESS;
		*intGrSeq = ID_ADDRESSGR_KAI;
		*lngItemSeq = m_ReadData[m_uInfo.intCurRow].m_AdSeq;
		break;

	// 支払銀行・支店名
	case ID_COL_081_BKNAME1:
	case ID_COL_081_BKNAME2:
		intRefType = ID_DLGTYPE_BANK;
		*intGrSeq = ID_ADDRESSGR_NONE;
		*lngItemSeq = m_ReadData[m_uInfo.intCurRow].m_BkSeq;
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
void	CfrmUc081Siharaitegata::virFncReferenceSetData( CdlgReference* dlgRef, int intRefType, int intCol )
{
	switch ( intRefType ) {

	// 支払先1・支払先2
	case ID_DLGTYPE_ADDRESS:
		if ( intCol == ID_COL_081_ADNAME1 ) {
			// 選択内容を表示
			CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, m_clsFunc.StrDocking( dlgRef->m_udAddress.m_AdName1 ,
				dlgRef->m_udAddress.m_AdName2 ) , 0 );
		}
		break;

	// 支払銀行・支店名
	case ID_DLGTYPE_BANK:
		if ( intCol == ID_COL_081_BKNAME1 ) {
			// 選択内容を表示
			CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, dlgRef->m_udBank.m_BkName1, 0 );
			CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex + 1, dlgRef->m_udBank.m_BkName2, 0 );
		}
		else if ( intCol == ID_COL_081_BKNAME2 ) {
			// 選択内容を表示
			CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, dlgRef->m_udBank.m_BkName2, 0 );
			CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex - 1, dlgRef->m_udBank.m_BkName1, 0 );
		}
		break;

	default:
		return;
		break;
	}

	m_pDiag->Refresh();
}

//********************************************************************************
//	【F9:参照】参照ダイアログの操作によりテーブルを更新
//		IN		int			参照ダイアログの種別
//		RET		なし
//********************************************************************************
void	CfrmUc081Siharaitegata::virFncReferenceUpdateTbl( int intRefType, int nRefMode/*=0*/ )
{
	CdbUcLstBank		rsBank( m_pDB );
	CdbUcLstAddress		rsAddress( m_pDB );

	switch ( intRefType ) {
	// 支払先1・支払先2
	case ID_DLGTYPE_ADDRESS:
		// 取引先を使用する全テーブルの更新
		rsAddress.UpdateDataTableAll(m_RegAutoRef);
		rsAddress.Fin();
		break;

	// 支払銀行・支店名
	case ID_DLGTYPE_BANK:
		// 金融機関を使用する全テーブルの更新
		CdbUcLstBank	rsBank( m_pDB );
		rsBank.UpdateDataTableAll(m_RegAutoRef);
		rsBank.Fin();
		break;
	}
}

//********************************************************************************
//	その他イベント処理
//********************************************************************************
BOOL	CfrmUc081Siharaitegata::PreTranslateMessage( MSG* pMsg )
{
//	int				intID;
	BOOL			blnMove = TRUE;		// カーソル移動可能/不可フラグ
	int				intOrgIndex;		// カーソル移動前のインデックス番号
	int				intMoveRow;			// カーソル移動先（行）
	int				intMoveCol;			// カーソル移動先（列）
// midori 152364 add -->
	int				rv=0;
// midori 152364 add <--
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
					// PageUpの場合：Scroll＋フォーカス移動（行－１１）
					m_pDiag->ScrollPos(SB_PAGEUP , 0);
					intMoveRow = intMoveRow - 9;
					if (intMoveRow < 1) {
						blnMove = FALSE;					// 移動しない
					}
				}
				else {
					// PageDownの場合：Scroll＋フォーカス移動（行＋１１）
					m_pDiag->ScrollPos(SB_PAGEDOWN, 0);
					intMoveRow = intMoveRow + 9;
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
				CmnDiagSetFocus(m_pDiag, m_uInfo.intRowMax, ID_COL_081_ADNAME1);
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
// midori 152364 add -->
	if(rv == 1)	{
		return(TRUE);
	}
// midori 152364 add <--

	return CfrmUc000Common::PreTranslateMessage(pMsg);
}

//********************************************************************************
//	頁コントロール処理（Focus，EditOFF，Terminationイベント）
//********************************************************************************

// 頁コントロールのフォーカス設定
void	CfrmUc081Siharaitegata::FocusPagediag1()
{
//マウスクリックでキャレットが表示できないためコメントとする
//	// ボタンからTabキー移動した場合に必要
//	m_PageDiag.SetPosition( 1 );
}

// 頁コントロールのゲットフォーカス
void	CfrmUc081Siharaitegata::EditONPagediag1( short index )
{
// midori 151405 add -->
	// スクロールフラグをクリアする
	nG_MScroll = 0;
	m_Pagefocus = 1;
// midori 151405 add <--

	// 1行登録
//	if( CmnGetControlCol() != ID_COL_081_VAL ){
		int intRet = virRecBufSaveData(m_uInfo.intCurPage, m_uInfo.intCurRow);
		if ((intRet == FUNCTION_UPDATE) || ( intRet == FUNCTION_DISPUPDATE)) {
			m_PageDiag.SetPosition(1);
		}
//	}
}

// 頁コントロールのロストフォーカス
void	CfrmUc081Siharaitegata::EditOFFPagediag1( short index )
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
void	CfrmUc081Siharaitegata::TerminationPagediag1( short index, short nChar, short length, LPUNKNOWN data )
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
			//CmnDiagSetFocus(m_pDiag, m_uInfo.intRowMax, ID_COL_081_ADNAME1);
			CmnDiagSetFocus(m_pDiag, nMaxDataCol, ID_COL_081_TEKI);
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
void	CfrmUc081Siharaitegata::FocusTitlediag1()
{
//マウスクリックでキャレットが表示できないためコメントとする
//	// ボタンからTabキー移動した場合に必要
//	m_TitleDiag.SetPosition( 0 );
}

// タイトルコントロールのゲットフォーカス
void	CfrmUc081Siharaitegata::EditONTitlediag1( short index )
{
// midori 151405 add -->
	// スクロールフラグをクリアする
	nG_MScroll = 0;
	m_Titlefocus = 1;
// midori 151405 add <--
	// 1行登録
//	if( CmnGetControlCol() != ID_COL_081_VAL ){
		int intRet = virRecBufSaveData(m_uInfo.intCurPage, m_uInfo.intCurRow);
		if ((intRet == FUNCTION_UPDATE) || ( intRet == FUNCTION_DISPUPDATE)) {
			m_TitleDiag.SetPosition(0);
		}
//	}
}

// タイトルコントロールのロストフォーカス
void	CfrmUc081Siharaitegata::EditOFFTitlediag1 (short index )
{
// midori 151405 add -->
	m_Titlefocus = 0;
// midori 151405 add <--
	virUpdateTitle();
}

//	タイトル更新
void CfrmUc081Siharaitegata::virUpdateTitle()
{
	// 帳表タイトル取得してuc_inf_subを更新
	m_uInfo.strTitleName = m_clsFunc.DiagGetString( &m_TitleDiag, 0 );
	CmnUcInfSubSetTitleInfo( TRUE );
}

// タイトルコントロールのキーイベント
void	CfrmUc081Siharaitegata::TerminationTitlediag1( short index, short nChar, short length, LPUNKNOWN data )
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
//	「支払手形」データコントロール処理（EditON，EditOFF，Terminationイベント）
//********************************************************************************

// 「支払手形」データコントロールのゲットフォーカス
void	CfrmUc081Siharaitegata::EditONYoushikidiag1( short index )
{
	DIAGRAM_DATA	diadata;
	int				intRet = -1;

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
	case ID_COL_081_SPDATE:	// 振出年月日
	case ID_COL_081_PMDATE:	// 支払期日
// midori 200101 del -->
		//// 日付の値を取得
		//m_uInfo.OldDateDiag = CmnDiagGetDate( m_pDiag, m_uInfo.intCurCtlIndex, DC_DATE_WMD );
		//m_uInfo.OldDateDiag = CmnDiagGetDate( m_pDiag, m_uInfo.intCurCtlIndex, DC_DATE_WMD );
// midori 200101 del <--
		break;
	default:
		// 現在の入力値を取得
		m_uInfo.OldstrDiag = m_clsFunc.DiagGetString( m_pDiag, m_uInfo.intCurCtlIndex );
		break;
	}


	// 行移動確認
	if ( /*( m_uInfo.OldRow != CmnGetControlRow() ) &&*/ ( m_uInfo.OldRow > 0 ) ) {
		if ( m_uInfo.OldPage == m_uInfo.intCurPage && m_uInfo.OldPage > 0 ) {
//			if( CmnGetControlCol() != ID_COL_081_VAL ){
			if(IsSaveMoveReference(ID_COL_081_BKNAME1, ID_COL_081_BKNAME2)){	// 銀行名→支店名　支店名→銀行名の移動は保存しない
				// カレント行登録＋再表示
				intRet = virRecBufSaveData(m_uInfo.intCurPage , m_uInfo.OldRow );
				if ((intRet == FUNCTION_UPDATE) || (intRet == FUNCTION_DISPUPDATE)) {
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
void	CfrmUc081Siharaitegata::EditOFFYoushikidiag1( short index )
{
	if( m_EndView != FALSE )	return;
	virInputDecisionEditOFF();
}

//	入力確定
void CfrmUc081Siharaitegata::virInputDecisionEditOFF()
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
//	if (  m_uInfo.OldRow > 0 ){
//		//if( CmnGetControlCol() == ID_COL_081_VAL ){
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
void	CfrmUc081Siharaitegata::TerminationYoushikidiag1( short index, short nChar, short length, LPUNKNOWN data )
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

		switch (m_ReadData[m_uInfo.intCurRow].m_FgFunc) {
		case ID_FGFUNC_SYOKEI:				// 小計
		case ID_FGFUNC_SYOKEINULL:			// 小計（空行）
		case ID_FGFUNC_CHUKEI:				// 中計
		case ID_FGFUNC_CHUKEINULL:			// 中計（空行）
		case ID_FGFUNC_RUIKEI:				// 累計行
		case ID_FGFUNC_PAGEKEI:				// 頁計行
		case ID_FGFUNC_IKKATUAUTO:			// 一括集計金額行（自動）
			break;
		default:
			// 項目をクリア（ただしComboBoxはクリアしない）
			m_pDiag->DataClear( m_uInfo.intCurCtlIndex, TRUE );
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
	//	}

	//	// 期末現在高で値が入力された場合の金額確定処理
	//	//if( intCol == ID_COL_081_VAL ){		
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
void CfrmUc081Siharaitegata::VScrollYoushikidiag1(short mode)
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
int	CfrmUc081Siharaitegata::GetFocusPosition( int nChar )
{
	int		intRow;				// カレント行番号
	int		intCol;				// カレント列番号
	int		intVector[4];		// 矢印キーの移動先（0:↑，1:→，2:↓，3:←）
	int		intNext = 0;		// フォーカス移動先
	int		intRowChange = 0;	// 行をまたぐ場合のインデックス増減数

	// カレント行＋列番号を取得
	CmnGetControlPosition( &intRow, &intCol );

	// 行をまたぐ場合のインデックス増減数（摘要→次の行の金融機関名）
	// ★★★★★カスタマイズ "2"は破線コントロールの数です。帳表毎に異なるため、変更して下さい。
	intRowChange = m_uInfo.intCtlStartIndex + 1 + 2;	// ここの値は検討が必要

	//----------------------------------------------------------------
	//	矢印キーによる移動先を取得（合計欄は次のステップで対応）
	//	基本的にReturn，Tab，Shift+Tabは、←，→キーと同じなので省略
	//----------------------------------------------------------------
	if ( intCol == ID_COL_081_ADNAME1 ) {
		// カレント列が「支払先:1行目」の場合
		intVector[0] = m_uInfo.intCurCtlIndex - m_uInfo.intColMax;		// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + m_uInfo.intColMax;		// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - intRowChange;			// ←
	}
	else if (( intCol == ID_COL_081_SPDATE  ) || 
			 ( intCol == ID_COL_081_BKNAME1 )) {
		// カレント列が「振出年月日」,「支払銀行名」,「割引銀行名」の場合
		intVector[0] = m_uInfo.intCurCtlIndex - m_uInfo.intColMax + 1;		// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;							// →
		intVector[2] = m_uInfo.intCurCtlIndex + 1;							// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;							// ←
	}
	else if (( intCol == ID_COL_081_PMDATE  ) ||
			 ( intCol == ID_COL_081_BKNAME2 )) {
		// カレント列が「支払期日」,「支払銀行名(支店名)」,「割引銀行名(支店名)」の場合
		intVector[0] = m_uInfo.intCurCtlIndex - 1;							// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;							// →
		intVector[2] = m_uInfo.intCurCtlIndex + m_uInfo.intColMax - 1;		// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;							// ←
	}
	else if ( intCol == ID_COL_081_TEKI ) {
		// カレント列が「摘要」の場合
		intVector[0] = m_uInfo.intCurCtlIndex - m_uInfo.intColMax;		// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + intRowChange;			// →
		intVector[2] = m_uInfo.intCurCtlIndex + m_uInfo.intColMax;		// ↓
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
int	CfrmUc081Siharaitegata::GetTotalRowIndex( int nCharOrg, int intIndex, int intNext )
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
			//nRet = CmnGetControlIndex( ( nBorder[1] + 1 ), ID_COL_081_ADNAME1 );
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
			nRet = CmnGetControlIndex( nBorder[0], ID_COL_081_ADNAME1 );
			break;
		case VK_LEFT:		// ←/Shift+Tab：23行目のデータ行の摘要項目へ移動
			nRet = CmnGetControlIndex( nBorder[0], ID_COL_081_TEKI );
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
				nRet = CmnGetControlIndex( ( nBorder[2] + 1 ), ID_COL_081_ADNAME1 );
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
			nRet = CmnGetControlIndex( ( nBorder[1] + 1 ), ID_COL_081_ADNAME1 );
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
void	CfrmUc081Siharaitegata::virTblEditIkkatuLine( int nType, CdbUc000Common* rsData,
									CString strName, CALCKEI_INFO uCalcKei, SORTKAMOKU_INFO uKamoku )
{
	CdbUc081Siharaitegata*	rs = (CdbUc081Siharaitegata*)rsData;

	switch (nType) {
	case 0:
		// ソートの一括集計処理
		rs->m_AdName1 = strName;				// 名称：支払先(下)
		rs->m_Val = uCalcKei.strKei[0];			// 金額："0:Val"固定
		break;
	case 1:
		// 一括金額参照の戻す処理
		rs->m_AdName1 = strName;				// 名称：支払先(下)
		rs->m_Val = uCalcKei.strKei[0];			// 金額："0:Val"固定
		break;
	case 2:
		// 特殊行挿入の一括金額選択時
		rs->m_AdName1 = strName;				// 名称：金融機関名
		break;
	}
}

//********************************************************************************
//	ソート項目を再設定後に、ソートを実行する
//		IN		CdlgSort*			ソートダイアログ情報
//				CdbUc000Common*		テーブル情報
//		RET		int					実行結果（FUNCTION_OK, FUNCTION_NG）
//********************************************************************************
int	CfrmUc081Siharaitegata::virTblSortSubSortProc( CdlgSort* pCdlgSort, CdbUc000Common* rsData )
{
	CdbUc081Siharaitegata*	rs;
	CString				strFilter;
	CString				strSort;

	// ソート項目の再設定（取引先）
	CdbUcLstAddress		rsAddress( m_pDB );
	rsAddress.UpdateDataTable( m_uInfo.intFormSeq, m_RegAutoRef );
	rsAddress.Fin();

	// ソート項目の再設定（金融機関）
	CdbUcLstBank	rsBank( m_pDB );
	rsBank.UpdateDataTable( m_uInfo.intFormSeq, m_RegAutoRef );
	rsBank.Fin();


	// ソート条件取得
	CmnTblSortSubGetSortParam( pCdlgSort, &strFilter, &strSort) ;

	rs = (CdbUc081Siharaitegata*)rsData;

	// 対象フィールドがNULLならNULL文字列にする：文字列連結のソート時は必須
	rs->UpdateDataTableWork();

	// ソート実行
	return rs->RequerySortParameter( strFilter, strSort );
}

/**********************************************************************
	RecBufSetData(CdbUc081Siharaitegata* prmDbRec)
		ローカルのレコードバッファに、1行分のデータを格納する

	引数
		CdbUc081Siharaitegata*	prmDbRec	レコードセット(データがあること）
	戻値
		int		成功/失敗
				FUNCTION_OK
***********************************************************************/
int	CfrmUc081Siharaitegata::RecBufSetData( CdbUc081Siharaitegata* prmDbRec )
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
	m_ReadData[trgRow].m_AdSeq = prmDbRec->m_AdSeq;	// 支払先：取引先（シーケンス番号）
	m_ReadData[trgRow].m_AdOrder = prmDbRec->m_AdOrder;	// 支払先：取引先（並び順）
	m_ReadData[trgRow].m_AdName1 = prmDbRec->m_AdName1;	// 支払先（名称：上段）
	m_ReadData[trgRow].m_AdName2 = prmDbRec->m_AdName2;	// 支払先（名称：下段）
	m_ReadData[trgRow].m_SpDate = prmDbRec->m_SpDate;	// 振出年月日
	m_ReadData[trgRow].m_PmDate = prmDbRec->m_PmDate;	// 支払期日
	m_ReadData[trgRow].m_BkSeq = prmDbRec->m_BkSeq;	// 金融機関（シーケンス番号）
	m_ReadData[trgRow].m_BkOrder = prmDbRec->m_BkOrder;	// 金融機関（並び順）
	m_ReadData[trgRow].m_BkName1 = prmDbRec->m_BkName1;	// 支払銀行名（銀行名）
	m_ReadData[trgRow].m_BkName2 = prmDbRec->m_BkName2;	// 支払銀行名（支店名）
	m_ReadData[trgRow].m_Val = prmDbRec->m_Val;	// 金額
	m_ReadData[trgRow].m_Teki = prmDbRec->m_Teki;	// 摘要
// midori 152137 add -->
	m_ReadData[trgRow].m_AdKana = prmDbRec->m_AdKana;	// 支払先
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
int	CfrmUc081Siharaitegata::RecBufClearAllData()
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
		P_REC_UC_081_SIHARAITEGATA inRecData レコードへのポインタ
	戻値
		int		成功/失敗
				FUNCTION_OK
***********************************************************************/
int	CfrmUc081Siharaitegata::RecBufClearData( P_REC_UC_081_SIHARAITEGATA inRecData )
{
	inRecData->m_Seq = 0;	// シーケンス番号
	inRecData->m_NumPage = 0;	// 頁番号
	inRecData->m_NumRow = 0;	// 行番号
	inRecData->m_FgFunc = 0;	// 特殊行フラグ
	inRecData->m_FgShow = 0;	// 表示フラグ
	inRecData->m_KeiStr = _T("");	// 計
	inRecData->m_NumGroup = 0;	// グループ番号
	inRecData->m_AdSeq = 0;	// 支払先：取引先（シーケンス番号）
	inRecData->m_AdOrder = 0;	// 支払先：取引先（並び順）
	inRecData->m_AdName1 = _T("");	// 支払先（名称：上段）
	inRecData->m_AdName2 = _T("");	// 支払先（名称：下段）
	inRecData->m_SpDate = 0;	// 振出年月日
	inRecData->m_PmDate = 0;	// 支払期日
	inRecData->m_BkSeq = 0;	// 金融機関（シーケンス番号）
	inRecData->m_BkOrder = 0;	// 金融機関（並び順）
	inRecData->m_BkName1 = _T("");	// 支払銀行名（銀行名）
	inRecData->m_BkName2 = _T("");	// 支払銀行名（支店名）
	inRecData->m_Val = _T("");	// 金額
	inRecData->m_Teki = _T("");	// 摘要
	inRecData->m_ShowKeiZero = 0;	//０円計表示フラグ
// midori 152137 add -->
	inRecData->m_AdKana = _T("");	// 支払先
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
int	CfrmUc081Siharaitegata::virRecBufSaveData( int inPage, int inRow )
{
	REC_UC_081_SIHARAITEGATA	tmpSaveData;	// 画面データ1レコード格納用
	BOOL	blnSpDate = FALSE;					// 「振出年月日」入力更新フラグ
	BOOL	blnPmDate = FALSE;					// 「支払期日」入力更新フラグ

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

	// 支払先1
	CStringArray strArray;
	intIndex = CmnGetControlIndex( inRow, ID_COL_081_ADNAME1 );
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
		if(m_uInfo.intCurCol == ID_COL_081_ADNAME1 && m_uInfo.intCurRow != m_uInfo.OldRow){	// 各カラムごとにvirRecBufSaveData()がコールされるようになり、かつタイミングがEditOffからEditOnに移ったことによりCmnDiagSetString()でカレットが末尾から先頭に移ってしまう為、制御（修正依頼No150489）
			CmnDiagSetString( m_pDiag, intIndex, tmpSaveData.m_AdName1, 0 );
		}
	}

	// 振出年月日
	intIndex = CmnGetControlIndex( inRow, ID_COL_081_SPDATE );
	tmpSaveData.m_SpDate = CmnDiagGetDateWMD( m_pDiag, intIndex, DC_DATE_WMD, m_ReadData[inRow].m_SpDate );
	blnSpDate = CmnCheckDate( m_pDiag, intIndex, DC_DATE_WMD, m_ReadData[inRow].m_SpDate );

	// 支払期日
	intIndex = CmnGetControlIndex( inRow, ID_COL_081_PMDATE );
	tmpSaveData.m_PmDate = CmnDiagGetDateWMD( m_pDiag, intIndex, DC_DATE_WMD, m_ReadData[inRow].m_PmDate );
	blnPmDate = CmnCheckDate( m_pDiag, intIndex, DC_DATE_WMD, m_ReadData[inRow].m_PmDate );

	// 支払銀行名
	intIndex = CmnGetControlIndex( inRow, ID_COL_081_BKNAME1 );
	tmpSaveData.m_BkName1 = m_clsFunc.DiagGetString( m_pDiag, intIndex );

	// 支払支店名
	intIndex = CmnGetControlIndex( inRow, ID_COL_081_BKNAME2 );
	tmpSaveData.m_BkName2 = m_clsFunc.DiagGetString( m_pDiag, intIndex );

	// 金額
	intIndex = CmnGetControlIndex( inRow, ID_COL_081_VAL );
	tmpSaveData.m_Val = CmnDiagGetKingaku( m_pDiag, intIndex );

	// 摘要
	intIndex = CmnGetControlIndex( inRow, ID_COL_081_TEKI );
	tmpSaveData.m_Teki = m_clsFunc.DiagGetString( m_pDiag, intIndex );

	///////////////////////////////////////////////////////////////
	// 登録処理呼び出し
	int	intRet = RecBufSaveDataSub( inPage, inRow, &tmpSaveData );

	// 登録処理が更新あり&エラー以外かつ日付入力値が更新必要な場合、画面更新
	if ((( intRet != FUNCTION_UPDATE ) && ( intRet != FUNCTION_NG )) &&
		(( blnSpDate == TRUE ) || ( blnPmDate == TRUE ))){
		intRet = FUNCTION_DISPUPDATE;
	}

	// 行登録した場合、計再計算を行う
	if (intRet == FUNCTION_UPDATE){
		// 計再計算
		CmnTblCalcKei();
	}
	// 行登録 or 入力値の更新があった場合、1頁再表示を行う
	if ((intRet == FUNCTION_UPDATE) || (intRet == FUNCTION_DISPUPDATE)){
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
int	CfrmUc081Siharaitegata::virRecBufSaveDataForPasteInsert( int inPage, int inRow )
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
int	CfrmUc081Siharaitegata::virRecBufSaveDataForPaste( int inPage, int inRow )
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
			REC_UC_081_SIHARAITEGATA
					inSaveData	登録するデータレコードへのポインタ
	戻値
			int					成功(更新あり)/成功(更新なし)/失敗
								FUNCTION_UPDATE	: 成功(更新あり)
								FUNCTION_OK		: 成功(更新なし)
								FUNCTION_NG		: 失敗
***********************************************************************/
int	CfrmUc081Siharaitegata::RecBufSaveDataSub( 
							int inPage,
							int inRow,
							P_REC_UC_081_SIHARAITEGATA inSaveData )
{
    REC_UC_081_SIHARAITEGATA udtTemp;
/*
	CString	tmp_AdName1;	// 画面上のデータ格納用(支払先1)
	CString	tmp_AdName2;	// 画面上のデータ格納用(支払先2)
	long	tmp_SpDate;	// 画面上のデータ格納用(振出年月日)
	long	tmp_PmDate;	// 画面上のデータ格納用(支払期日)
	CString	tmp_BkName1;	// 画面上のデータ格納用(支払銀行名)
	CString	tmp_BkName2;	// 画面上のデータ格納用(支払支店名)
	CString	tmp_Val;	// 画面上のデータ格納用(金額)
	CString	tmp_Teki;	// 画面上のデータ格納用(摘要)
*/

	udtTemp.m_BkOrder = 0;
	udtTemp.m_BkSeq = 0;
	udtTemp.m_FgFunc = 0;
// midori 152137 add -->
	udtTemp.m_AdKana = _T("");	// 支払先
// midori 152137 add <--

//	// 変更があったかのフラグ（変更あり:TRUE、なし:FALSE)
	BOOL	ufgUpData = FALSE;	// 変更確認フラグ(全体用データ行/空行)
	BOOL	ufgUpToku = FALSE;	// 変更確認フラグ(全体用特殊行)
	BOOL	ufgAdName = FALSE;	// 変更確認フラグ(支払先)
	BOOL	ufgSpDate = FALSE;	// 変更確認フラグ(振出年月日)
	BOOL	ufgPmDate = FALSE;	// 変更確認フラグ(支払期日)
	BOOL	ufgBk = FALSE;		// 変更確認フラグ(支払銀行名・支店名)
	BOOL	ufgVal = FALSE;		// 変更確認フラグ(金額)
	BOOL	ufgTeki = FALSE;	// 変更確認フラグ()
	BOOL	ufgFunc = FALSE;	// 変更確認フラグ(摘要)
	// レコード更新したことを示すフラグ
	BOOL	ufgRsUpdate = FALSE;

	// テーブルアクセスクラス
	CdbUc081Siharaitegata	rs( m_pDB );

	///////////////////////////////////////////////////////////////
	// 作業用変数へ 登録対象のフィールド値を代入
// midori 190504 del -->
	//udtTemp.m_AdName1 = inSaveData->m_AdName1;	// (支払先1)
	//udtTemp.m_AdName2 = inSaveData->m_AdName2;	// (支払先2)
	//udtTemp.m_SpDate = inSaveData->m_SpDate;	// (振出年月日)
	//udtTemp.m_PmDate = inSaveData->m_PmDate;	// (支払期日)
	//udtTemp.m_BkName1 = inSaveData->m_BkName1;	// (支払銀行名)
	//udtTemp.m_BkName2 = inSaveData->m_BkName2;	// (支払銀行名（支店名）)
	//udtTemp.m_Val = inSaveData->m_Val;			// (金額)
	//udtTemp.m_Teki = inSaveData->m_Teki;		// (摘要)
// midori 190504 del <--
// midori 190504 add -->
	udtTemp.m_AdName1 = m_clsFunc.DeleteRightSpace( inSaveData->m_AdName1 );	// (支払先1)
	udtTemp.m_AdName2 = m_clsFunc.DeleteRightSpace( inSaveData->m_AdName2 );	// (支払先2)
	udtTemp.m_SpDate = inSaveData->m_SpDate;									// (振出年月日)
	udtTemp.m_PmDate = inSaveData->m_PmDate;									// (支払期日)
	udtTemp.m_BkName1 = m_clsFunc.DeleteRightSpace( inSaveData->m_BkName1 );	// (支払銀行名)
	udtTemp.m_BkName2 = m_clsFunc.DeleteRightSpace( inSaveData->m_BkName2 );	// (支払銀行名（支店名）)
	udtTemp.m_Val = inSaveData->m_Val;											// (金額)
	udtTemp.m_Teki = inSaveData->m_Teki;										// (摘要)
// midori 190504 add <--

	///////////////////////////////////////////////////////////////
	// データ行か、空行かのチェック
	udtTemp.m_FgFunc = m_ReadData[inRow].m_FgFunc;
	if ( udtTemp.m_FgFunc <= ID_FGFUNC_DATA ) {	// データ行 または、空行なら
		// コンポーネントをチェックして、データがあるかを調べる
		// (仮変数に入れた値でチェックしてもいいのかも？)

		if ( ( udtTemp.m_AdName1 == "" )		// 画面上のデータ格納用(支払先1)
		  && ( udtTemp.m_AdName2 == "" )		// 画面上のデータ格納用(支払先2)
		  && ( udtTemp.m_SpDate == 0 )		// 画面上のデータ格納用(振出年月日)
		  && ( udtTemp.m_PmDate == 0 )		// 画面上のデータ格納用(支払期日)
		  && ( udtTemp.m_BkName1 == "" )		// 画面上のデータ格納用(支払銀行名)
		  && ( udtTemp.m_BkName2 == "" )		// 画面上のデータ格納用(支払支店名)
		  && ( udtTemp.m_Val == "" )		// 画面上のデータ格納用(金額)
		  && ( udtTemp.m_Teki == "" ) ) {		// 画面上のデータ格納用(摘要)
			// 空行なら
			udtTemp.m_FgFunc = ID_FGFUNC_NULL;
		}
		else {
			// データが入っていれば
			udtTemp.m_FgFunc = ID_FGFUNC_DATA;
		}
	}

	///////////////////////////////////////////////////////////////
	// 更新状況のチェック
	//(1) 支払先1
	if ( udtTemp.m_AdName1 != m_ReadData[inRow].m_AdName1 ) {
		ufgUpData = TRUE;		// 全体(データ行/空行)
		ufgUpToku = TRUE;		// 全体(特殊行)
		ufgAdName = TRUE;		// 「振出人1」用
	}

	//(2) 支払先2
	if ( udtTemp.m_AdName2 != m_ReadData[inRow].m_AdName2 ) {
		ufgUpData = TRUE;		// 全体(データ行/空行)
		ufgAdName = TRUE;		// 「振出人2」用
	}

	//(3) 振出年月日
	if ( udtTemp.m_SpDate != m_ReadData[inRow].m_SpDate ) {
		ufgUpData = TRUE;		// 全体(データ行/空行)
		ufgSpDate = TRUE;		// 「振出年月日」用
	}

	//(4) 支払期日
	if ( udtTemp.m_PmDate != m_ReadData[inRow].m_PmDate ) {
		ufgUpData = TRUE;		// 全体(データ行/空行)
		ufgPmDate = TRUE;		// 「支払期日」用
	}

	//(5) 支払銀行名
	if ( udtTemp.m_BkName1 != m_ReadData[inRow].m_BkName1 ) {
		ufgUpData = TRUE;		// 全体(データ行/空行)
		ufgBk = TRUE;			// 「支払銀行名」用
	}

	//(6) 支払銀行名（支店名）
	if ( udtTemp.m_BkName2 != m_ReadData[inRow].m_BkName2 ) {
		ufgUpData = TRUE;		// 全体(データ行/空行)
		ufgBk = TRUE;			// 「支払銀行名(支店名)」用
	}

	//(7) 金額
	if ( udtTemp.m_Val != m_ReadData[inRow].m_Val ) {
		ufgUpData = TRUE;		// 全体(データ行/空行)
		ufgVal = TRUE;			// 「金額」用
	}

	//(8) 摘要
	if ( udtTemp.m_Teki != m_ReadData[inRow].m_Teki ) {
		ufgUpData = TRUE;		// 全体(データ行/空行)
		ufgTeki = TRUE;			// 「摘要」用
	}

	//(9) 特殊行フラグ(基本的には、空行/データ行のチェックになるはず)
	if ( m_ReadData[inRow].m_FgFunc <= ID_FGFUNC_DATA ) {
		// 元が空行orデータ行の場合、入力内容により空行orデータ行となる
		// 元が特殊行の場合、FgFuncは更新しない
		if ( udtTemp.m_FgFunc != m_ReadData[inRow].m_FgFunc ) {
			ufgUpData = TRUE;		// 全体(データ行/空行)
			ufgFunc = TRUE;     // ufgFunc用
		}
	}

	///////////////////////////////////////////////////////////////////
	// 1 レコード更新処理
	if ( ( udtTemp.m_FgFunc == ID_FGFUNC_DATA ) || ( udtTemp.m_FgFunc == ID_FGFUNC_IKKATUMANUAL ) ) {	// データ行or一括集計金額行（手動）
		///////////////////////////////////////////////////////////////////
		// データ行or一括集計金額行（手動）の場合の処理

		// 支払先：取引先名称リストへの登録チェック
		if ((udtTemp.m_FgFunc == ID_FGFUNC_DATA) && (ufgAdName == TRUE)) {	// 変更ありの場合
			CdbUcLstAddress		rsAdd( m_pDB );

			if(m_RegAutoRef)	udtTemp.m_AdSeq = rsAdd.CheckEntryAddNone( udtTemp.m_AdName1, udtTemp.m_AdName2, "", "", ID_ADDRESSGR_KAI , FALSE , TRUE);
			else				udtTemp.m_AdSeq = rsAdd.GetSeqFromAdName(ID_ADDRESSGR_KAI, udtTemp.m_AdName1, udtTemp.m_AdName2 );

			//	エラー？
			if ( udtTemp.m_AdSeq < 0 ){
				rsAdd.Fin();
				return( FUNCTION_NG );
			}
			
// midori 152137 del -->
//			if ( udtTemp.m_AdSeq == 0 )		udtTemp.m_AdOrder = 0;
//			else							udtTemp.m_AdOrder = rsAdd.m_OrderNum;
// midori 152137 del <--
// midori 152137 add -->
			if ( udtTemp.m_AdSeq == 0 )		{
				udtTemp.m_AdOrder = 0;
			}
			else							{
				udtTemp.m_AdOrder = rsAdd.m_OrderNum;
				udtTemp.m_AdKana = rsAdd.m_AdKana;
			}
// midori 152137 add <--

			rsAdd.Fin();
		}

		// 支払銀行名：金融機関名称リストへの登録チェック
		if ( ( udtTemp.m_FgFunc == ID_FGFUNC_DATA ) && ( ufgBk == TRUE ) ) {  // 変更ありの場合
			CdbUcLstBank	rsBank( m_pDB );

			if(m_RegAutoRef)	udtTemp.m_BkSeq = rsBank.CheckEntry( udtTemp.m_BkName1, udtTemp.m_BkName2);
			else				udtTemp.m_BkSeq = rsBank.GetSeqFromBkName( udtTemp.m_BkName1, udtTemp.m_BkName2 );

			if ( udtTemp.m_BkSeq < 0 ){
				rsBank.Fin();
				return( FUNCTION_NG );
			}
			
			if ( udtTemp.m_BkSeq == 0 )		udtTemp.m_BkOrder = 0;				
			else							udtTemp.m_BkOrder = rsBank.m_OrderNum;

			rsBank.Fin();
		}

		// 更新処理
		if ( ufgUpData == TRUE ) {		// テーブルへ更新(1レコード更新)
			// ページ、行 で抽出し、必要なデータを更新
			rs.RequeryPageRow( inPage, inRow );

			if ( !rs.IsEOF() ) {		// レコードが取得できた(取得できないことはないはず)
				rs.Edit();

				// 支払先
				if ( ufgAdName == TRUE ) {  // 変更ありの場合
					rs.m_AdSeq = udtTemp.m_AdSeq;
					rs.m_AdOrder = udtTemp.m_AdOrder;
					// 末尾スペース削除後に空行削除処理
					udtTemp.m_AdName1 = m_clsFunc.DeleteRightSpace( udtTemp.m_AdName1 );
					udtTemp.m_AdName2 = m_clsFunc.DeleteRightSpace( udtTemp.m_AdName2 );
					m_clsFunc.DeleteNullRow( &udtTemp.m_AdName1, &udtTemp.m_AdName2 );
					rs.m_AdName1 = udtTemp.m_AdName1;
					rs.m_AdName2 = udtTemp.m_AdName2;
// midori 152137 add -->
					rs.m_AdKana = udtTemp.m_AdKana;
// midori 152137 add <--
				}

				// 振出年月日
				if ( ufgSpDate = TRUE ) {  // 変更ありの場合
					rs.m_SpDate = udtTemp.m_SpDate;
				}

				// 支払期日
				if ( ufgPmDate = TRUE ) {  // 変更ありの場合
					rs.m_PmDate = udtTemp.m_PmDate;
				}

				// 支払銀行名
				if ( ufgBk == TRUE ) {	// 変更ありの場合
					rs.m_BkSeq = udtTemp.m_BkSeq;
					rs.m_BkName1 = m_clsFunc.DeleteRightSpace( udtTemp.m_BkName1 );
					rs.m_BkName2 = m_clsFunc.DeleteRightSpace( udtTemp.m_BkName2 );
					rs.m_BkOrder = udtTemp.m_BkOrder;
				}

				// 金額
				if ( ufgVal == TRUE ) {	// 変更ありの場合
					rs.m_Val = udtTemp.m_Val;
				}
			
				// 摘要
				if ( ufgTeki == TRUE ) {	// 変更ありの場合
					CStringArray strArray;
					m_clsFunc.StrDivision( udtTemp.m_Teki, &strArray, 2, TRUE, TRUE );
					rs.m_Teki		= m_clsFunc.StrDocking( &strArray );
				}

				// 特殊行フラグは、強制的に書き込む
				rs.m_FgFunc = udtTemp.m_FgFunc;

				rs.Update();
				ufgRsUpdate = TRUE;		// レコード更新したので ON
			}
		}

	}
	else if ( udtTemp.m_FgFunc == ID_FGFUNC_NULL ) {	// 空行
		///////////////////////////////////////////////////////////////////
		// 空行の場合の処理

		// 更新処理
		if ( ufgUpData == TRUE ) {		// テーブルへ更新(1レコード更新)
			// ページ、行 で抽出し、必要なデータを更新
			rs.RequeryPageRow( inPage, inRow );

			if ( !rs.IsEOF() ) {		// レコードが取得できた(取得できないことはないはず)
				rs.Edit();
				rs.m_FgFunc = udtTemp.m_FgFunc;
				rs.m_KeiStr = _T("");	// 計
				rs.m_NumGroup = 0;	// グループ番号
				rs.m_AdSeq = 0;	// 支払先：取引先（シーケンス番号）
				rs.m_AdOrder = 0;	// 支払先：取引先（並び順）
				rs.m_AdName1 = _T("");	// 支払先（名称：上段）
				rs.m_AdName2 = _T("");	// 支払先（名称：下段）
				rs.m_SpDate = 0;	// 振出年月日
				rs.m_PmDate = 0;	// 支払期日
				rs.m_BkSeq = 0;	// 金融機関（シーケンス番号）
				rs.m_BkOrder = 0;	// 金融機関（並び順）
				rs.m_BkName1 = _T("");	// 支払銀行名（銀行名）
				rs.m_BkName2 = _T("");	// 支払銀行名（支店名）
				rs.m_Val = _T("");	// 金額
				rs.m_Teki = _T("");	// 摘要
// midori 152137 add -->
				rs.m_AdKana = _T("");	// 振出人
// midori 152137 add <--

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
//	「支払手形」テーブルのオブジェクトを生成し、ポインタを返す
//********************************************************************************
CdbUc000Common*	CfrmUc081Siharaitegata::virGetDataTableObject()
{
	// 「支払手形」テーブルのオブジェクトを生成し、ポインタを返す
	return (CdbUc000Common*)( new CdbUc081Siharaitegata( m_pDB ) );
}

//********************************************************************************
//	行コピー処理
//	IN	なし
//	RET	なし
//********************************************************************************
void	CfrmUc081Siharaitegata::virRowButtonCopyProc()
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
void	CfrmUc081Siharaitegata::virGetCurrentInfo( CURRENT_INFO* uCurInfo )
{
	uCurInfo->intFgFunc = m_ReadData[m_uInfo.intCurRow].m_FgFunc;
	uCurInfo->intNumGroup = m_ReadData[m_uInfo.intCurRow].m_NumGroup;
	uCurInfo->intCopyFlag = m_nCopyFlag;
}

//**************************************************
//	入力不可モードの設定/解除
//	【引数】	nRow	…	行
//				nMode	…	入力不可モードフラグ
//								DIAG_MDFY_NOINPUT	…	エディットアイテムで全てのWM_CHARを無効とする
//								DIAG_MDFY_INPUT		…	DIAG_MDFY_NOINPUTモード解除
//	【戻値】	なし
//**************************************************
void CfrmUc081Siharaitegata::SetInputMode( int nRow, long nMode )
{
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_081_ADNAME1 ), nMode );	//	支払先（上段）
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_081_SPDATE ), nMode );	//	振出年月日
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_081_PMDATE ), nMode );	//	支払期日
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_081_BKNAME1 ), nMode );	//	銀行名
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_081_BKNAME2 ), nMode );	//	支店名
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_081_VAL ), nMode );		//	金額
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_081_TEKI ), nMode );		//	摘要
}

//**************************************************
//	各Diagの背景色(BackColor)に色を設定
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CfrmUc081Siharaitegata::SetDiagBackColor()
{
	// 入力画面の各Diagの背景色を共通設定色に合わせる
	m_PageDiag.SetBackColor( m_ViewColor );
	m_TitleDiag.SetBackColor( m_ViewColor );
	m_NumberDiag.SetBackColor( m_ViewColor );
	m_HeadDiag.SetBackColor( m_ViewColor );
	m_Diag->SetBackColor( m_ViewColor );
	m_DiagKei->SetBackColor( m_ViewColor );
// midori 161108 add -->
	m_SaveDataDiag.SetBackColor( m_ViewColor );
// midori 161108 add <--
}

//********************************************************************************
//	財務連動：連動データの作成（新規作成）
//		IN		CdbUc000Common*		内訳書テーブル情報
//				RENTEMP_INFO*		財務連動情報
//		RET		なし					
//********************************************************************************
void CfrmUc081Siharaitegata::virRendoCreateData(CdbUc000Common* rsData, RENTEMP_INFO* uRenInfo)
{
	CString			strTemp;
	CStringArray	strArry;
	int				intSeq;
// midori 152153 add -->
	int				sw = 0;
	CdbUcLstAddress	rsLstAdd(m_pDB);	// rs_lst_Address
// midori 152153 add <--

	CdbUc081Siharaitegata*	rs = (CdbUc081Siharaitegata*)rsData;

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

	// 取引先情報：取引先登録チェック(買)
// midori 152153 del -->
	//CdbUcLstAddress		rsLstAdd(m_pDB);								// rs_lst_Address
	//m_clsFunc.StrDivisionEx( uRenInfo->strAdName , &strArry , ( MAX_KETA_081_ADNAME1 / 2 ) , 2 );
	//intSeq = rsLstAdd.CheckEntryAddNone( strArry.GetAt(0), strArry.GetAt(1) , _T("") , _T("") , ID_ADDRESSGR_KAI , FALSE, TRUE);
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
	if(uRenInfo->intOpRenType == ID_RENDATA_DA)	{
		sw = 1;
	}
	// 科目残高、枝番残高
	else	{
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
	}

	if(sw != 0)	{
		m_clsFunc.StrDivisionEx( uRenInfo->strAdName , &strArry , ( MAX_KETA_081_ADNAME1 / 2 ) , 2 );
		if(sw == 1)	{
// midori 152137 del -->
//			intSeq = rsLstAdd.CheckEntryAddNone( strArry.GetAt(0), strArry.GetAt(1) , _T("") , _T("") , ID_ADDRESSGR_KAI , FALSE, TRUE);
// midori 152137 del <--
// midori 152137 add -->
			intSeq = rsLstAdd.CheckEntryAddNoneRen( strArry.GetAt(0), strArry.GetAt(1) , _T("") , _T("") , uRenInfo->strAdKana, ID_ADDRESSGR_KAI , FALSE, TRUE);
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
void CfrmUc081Siharaitegata::virRendoUpdateData(CdbUc000Common* rsData, RENTEMP_INFO* uRenInfo)
{
	CdbUc081Siharaitegata*	rs = (CdbUc081Siharaitegata*)rsData;

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
BOOL CfrmUc081Siharaitegata::virEndProc(void)
{
	TRACE(_T("***** CfrmUc081Siharaitegata::virEndProc\n"));

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
int CfrmUc081Siharaitegata::CursorControl(short nChar, int sw)
{
	int				rv=0;
	int				intCol=0;
	int				pos=0;

	// 現在の列番号を取得
	intCol = CmnGetControlCol();

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

// 20-0873 add -->
//********************************************************************************
//	取引先コードの取得
//		IN		CfrmUc000Common*	テーブル情報
//		RET		int					取引先コード
//********************************************************************************
int CfrmUc081Siharaitegata::virTblSortGetTorihikisakiCode( CdbUc000Common* rsData )
{
	int						ret=0;
	CdbUc081Siharaitegata*	rs = (CdbUc081Siharaitegata*)rsData;
	ret = rs->m_AdOrder;
	return(ret);
}

//********************************************************************************
//	取引先名称の取得
//		IN		CfrmUc000Common*	テーブル情報
//		RET		CString				取引先名称
//********************************************************************************
CString CfrmUc081Siharaitegata::virTblSortGetTorihikisakiName( CdbUc000Common* rsData )
{
	CString					ret=_T("");
	CString					cs1=_T(""),cs2=_T("");
	CdbUc021Uketoritegata*	rs = (CdbUc021Uketoritegata*)rsData;
	cs1 = rs->m_AdName1;
	cs2 = rs->m_AdName2;
	cs1.TrimRight();
	cs2.TrimRight();
	return(cs1 + cs2);
}
// 20-0873 add <--

// 修正No.162446 add -->
LRESULT CfrmUc081Siharaitegata::OnUserReDrawView(WPARAM wParam, LPARAM lParam)
{
	virUpdateControlTblData();
	CmnDiagSetFocus(m_pDiag, 1, 1);

	return(0);
}
// 修正No.162446 add <--
