// インボイス登録番号追加対応_23/09/11 add -->
//--------------------------------------------------
//	frmUc091KaikakekinI.cpp
//--------------------------------------------------

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "frmUc091Kaikakekin.h"

extern BOOL	bG_Kanso;

// 改良No.21-0086,21-0529 cor -->
//extern	int KamokuRowEnableSgn(CDatabase* pDB, int pSw);
// ------------------------------
extern	int KamokuRowEnableSgn(CDatabase* pDB, int pSw, int nFormSeq);
// 改良No.21-0086,21-0529 cor <--

IMPLEMENT_DYNCREATE( CfrmUc091KaikakekinI, CfrmUc000Common )


BEGIN_MESSAGE_MAP( CfrmUc091KaikakekinI, CfrmUc000Common )
	ON_WM_SIZE()
	ON_COMMAND( ID_BUTTON_F2, OnButtonF2 )
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
	ON_COMMAND( ID_PRINTSETUP_MENU, &CfrmUc091KaikakekinI::OnMenuPrintSetup )
	ON_COMMAND( ID_PRINTALL_MENU, &CfrmUc091KaikakekinI::OnMenuPrintAll )
	ON_COMMAND( ID_SPECIALROW_MENU, &CfrmUc091KaikakekinI::OnMenuSpecialRow )
	ON_COMMAND( ID_SORT_MENU, &CfrmUc091KaikakekinI::OnMenuSort )
	ON_COMMAND(ID_IKKATUADD_MENU, &CfrmUc091KaikakekinI::OnIkkatuAddMenu)
	ON_COMMAND( ID_DISPDIALOG_MENU, &CfrmUc091KaikakekinI::OnMenuIkkatuKingaku )
	ON_COMMAND( ID_ZEROMONEY_MENU, &CfrmUc091KaikakekinI::OnMenuZeroMoney )
	ON_COMMAND( ID_ZEROMONEYMOVE_MENU, &CfrmUc091KaikakekinI::OnMenuZeroMoneyMove )
	ON_COMMAND( ID_AMOUTDIALOG_MENU, &CfrmUc091KaikakekinI::OnMenuOutKeiSetup )
	ON_COMMAND( ID_CLEAR_MENU, &CfrmUc091KaikakekinI::OnMenuClear )
	ON_COMMAND( ID_RENDO_MENU, &CfrmUc091KaikakekinI::OnMenuZaimuRendo )
	ON_COMMAND( ID_TITLEMENU, &CfrmUc091KaikakekinI::OnMenuTitleRestore )
	ON_COMMAND( ID_USER_SETTING_MENU, &CfrmUc091KaikakekinI::OnMenuOptionSetting )
	ON_COMMAND( ID_COPY_SETTING_INFO_MENU, &CfrmUc091KaikakekinI::OnMenuCopySettingInfo )
	ON_COMMAND( ID_DELETE_DATA_MENU, &CfrmUc091KaikakekinI::OnMenuDeleteDataEachYoushiki )
	ON_BN_CLICKED( IDC_PAGEBACK_BUTTON1, &CfrmUc091KaikakekinI::OnPageBack )
	ON_BN_CLICKED( IDC_PAGENEXT_BUTTON1, &CfrmUc091KaikakekinI::OnPageNext )
	ON_BN_CLICKED( IDC_PAGENEW_BUTTON1, &CfrmUc091KaikakekinI::OnPageNew )
	ON_BN_CLICKED( IDC_PAGEINSERT_BUTTON1, &CfrmUc091KaikakekinI::OnPageInsert )
	ON_BN_CLICKED( IDC_ROWCOPY_BUTTON1, &CfrmUc091KaikakekinI::OnRowCopy )
	ON_BN_CLICKED( IDC_ROWPASTE_BUTTON1, &CfrmUc091KaikakekinI::OnRowPaste )
	ON_BN_CLICKED( IDC_ROWINSERT_BUTTON1, &CfrmUc091KaikakekinI::OnRowInsert )
	ON_BN_CLICKED( IDC_MARGINAL_BUTTON1, &CfrmUc091KaikakekinI::OnBnClickedMarginalButton1 )
	ON_BN_CLICKED( IDC_YOUSHIKINEXT_BUTTON1, &CfrmUc091KaikakekinI::OnMoveYoushikiNext )
	ON_BN_CLICKED( IDC_YOUSHIKIBACK_BUTTON1, &CfrmUc091KaikakekinI::OnMoveYoushikiBack )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_F6, &CfrmUc091KaikakekinI::OnUpdateButtonF6 )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_F7, &CfrmUc091KaikakekinI::OnUpdateButtonF7 )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_F8, &CfrmUc091KaikakekinI::OnUpdateButtonF8 )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_F9, &CfrmUc091KaikakekinI::OnUpdateButtonF9 )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_F11, &CfrmUc091KaikakekinI::OnUpdateButtonF11 )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_F12, &CfrmUc091KaikakekinI::OnUpdateButtonF12 )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_HOME, &CfrmUc091KaikakekinI::OnUpdateButtonHome )
	ON_UPDATE_COMMAND_UI( ID_IKKATUADD_MENU, &CfrmUc091KaikakekinI::OnUpdateIkkatuaddMenu)
	ON_UPDATE_COMMAND_UI( ID_DISPDIALOG_MENU, &CfrmUc091KaikakekinI::OnUpdateMenuIkkatuKingaku )
	ON_UPDATE_COMMAND_UI( ID_SORT_MENU, &CfrmUc091KaikakekinI::OnUpdateMenuSort )
	ON_UPDATE_COMMAND_UI( ID_CLEAR_MENU, &CfrmUc091KaikakekinI::OnUpdateMenuClear )
	ON_UPDATE_COMMAND_UI( ID_ZEROMONEY_MENU, &CfrmUc091KaikakekinI::OnUpdateMenuZeroMoney )
	ON_UPDATE_COMMAND_UI( ID_ZEROMONEYMOVE_MENU, &CfrmUc091KaikakekinI::OnUpdateMenuZeroMoneyMove)
	ON_UPDATE_COMMAND_UI( ID_RENDO_MENU, &CfrmUc091KaikakekinI::OnUpdateMenuZaimuRendo )
	ON_UPDATE_COMMAND_UI(ID_SPECIALROW_MENU, &CfrmUc091KaikakekinI::OnUpdateMenuSpecialRow)
	ON_UPDATE_COMMAND_UI( ID_TITLEMENU, &CfrmUc091KaikakekinI::OnUpdateMenuTitleRestore )
	ON_UPDATE_COMMAND_UI( ID_DELETE_DATA_MENU, &CfrmUc091KaikakekinI::OnUpdateMenuDeleteDataEachYoushiki )
	ON_MESSAGE(WM_USER_REDRAWVIEW, OnUserReDrawView)
END_MESSAGE_MAP()


CfrmUc091KaikakekinI::CfrmUc091KaikakekinI()
	: CfrmUc000Common( CfrmUc091KaikakekinI::IDD )
{
	m_pDB = ( (CUCHIWAKEApp*)AfxGetApp() )->m_pDB;
	m_pTitleDiag = &m_TitleDiag;
	m_pPageDiag	= &m_PageDiag;
	m_pHeadDiag = &m_HeadDiag;		// 修正No.168481,168502,168517 add
	m_pDiagKei2 = &m_DiagKei2;		// 修正No.168481,168502,168517 add
}

CfrmUc091KaikakekinI::~CfrmUc091KaikakekinI()
{
	// 2009.09.08 TS自動ログオフ対応(virEndProcへ移動)
	TRACE(_T("***** CfrmUc091KaikakekinI::~CfrmUc091KaikakekinI\n"));

	//m_pDiag = NULL;
	//m_pTitleDiag = NULL;
}

void CfrmUc091KaikakekinI::DoDataExchange( CDataExchange* pDX )
{
	CfrmUc000Common::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_YOUSHIKIDIAG1, m_Diag1 );
	DDX_Control( pDX, IDC_YOUSHIKIDIAG3, m_Diag2 );		// なぜかIDC_YOUSHIKIDIAG2が予約されているのでDIAG3を使う
	DDX_Control( pDX, IDC_YOUSHIKIDIAG_KEI1, m_DiagKei1 );
	DDX_Control( pDX, IDC_YOUSHIKIDIAG_KEI2, m_DiagKei2 );
	DDX_Control( pDX, IDC_TITLEDIAG1, m_TitleDiag );
	DDX_Control( pDX, IDC_PAGEDIAG1, m_PageDiag );
	DDX_Control( pDX, IDC_NUMBER_DIAG1, m_NumberDiag );
	DDX_Control(pDX, IDC_MARGINAL_BUTTON1, m_btnMarjinal);
	DDX_Control( pDX, IDC_TOPICDIAG1, m_HeadDiag);
	DDX_Control(pDX, IDC_SAVEDATA_DIAG1, m_SaveDataDiag);
	DDX_Control(pDX, IDC_ICSDISPGUIDE1, m_Guide1);
}

void CfrmUc091KaikakekinI::OnInitialUpdate()
{
	CfrmUc000Common::OnInitialUpdate();
	ResizeParentToFit();

	// 帳表テーブルのオープンOK?
	if(m_InitialFlag == TRUE)	{
		// コンボボックスアイテムのリサイズ制御
		m_Diag1.ComboxFontMode(TRUE);
		m_Diag2.ComboxFontMode(TRUE);

		// 背景色を取得
		m_ViewColor = CmnGetBackColor();

		// 欄外ガイドの背景色
		m_Guide1.SetBackColor(m_ViewColor);
		m_Guide1.ShowWindow(FALSE);

		// 帳表表示処理OK?
		if(virStartJob() == TRUE)	{
		}
		else	{
			// カーソルフラグ=FALSE(この後フォーカスセット処理が走らないように)
			m_CursorSetFlag = FALSE;
		}
	}
}

#ifdef _DEBUG
void CfrmUc091KaikakekinI::AssertValid() const
{
	CfrmUc000Common::AssertValid();
}

#ifndef _WIN32_WCE
void CfrmUc091KaikakekinI::Dump( CDumpContext& dc ) const
{
	CfrmUc000Common::Dump( dc );
}
#endif
#endif //_DEBUG

BEGIN_EVENTSINK_MAP( CfrmUc091KaikakekinI, CfrmUc000Common )
	ON_EVENT( CfrmUc091KaikakekinI, IDC_YOUSHIKIDIAG1, 3, CfrmUc091KaikakekinI::TerminationYoushikidiag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN )
	ON_EVENT( CfrmUc091KaikakekinI, IDC_YOUSHIKIDIAG1, 2, CfrmUc091KaikakekinI::EditOFFYoushikidiag1, VTS_I2 )
	ON_EVENT( CfrmUc091KaikakekinI, IDC_YOUSHIKIDIAG1, 1, CfrmUc091KaikakekinI::EditONYoushikidiag1, VTS_I2 )
	ON_EVENT(CfrmUc091KaikakekinI, IDC_YOUSHIKIDIAG1, 12, CfrmUc091KaikakekinI::VScrollYoushikidiag1, VTS_I2)
	ON_EVENT(CfrmUc091KaikakekinI, IDC_YOUSHIKIDIAG1, 8, CfrmUc091KaikakekinI::ComboSelYoushikidiag1, VTS_I2 VTS_UNKNOWN)

	ON_EVENT( CfrmUc091KaikakekinI, IDC_YOUSHIKIDIAG3, 3, CfrmUc091KaikakekinI::TerminationYoushikidiag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN )
	ON_EVENT( CfrmUc091KaikakekinI, IDC_YOUSHIKIDIAG3, 2, CfrmUc091KaikakekinI::EditOFFYoushikidiag1, VTS_I2 )
	ON_EVENT( CfrmUc091KaikakekinI, IDC_YOUSHIKIDIAG3, 1, CfrmUc091KaikakekinI::EditONYoushikidiag1, VTS_I2 )
	ON_EVENT(CfrmUc091KaikakekinI, IDC_YOUSHIKIDIAG3, 12, CfrmUc091KaikakekinI::VScrollYoushikidiag1, VTS_I2)
	ON_EVENT(CfrmUc091KaikakekinI, IDC_YOUSHIKIDIAG3, 8, CfrmUc091KaikakekinI::ComboSelYoushikidiag1, VTS_I2 VTS_UNKNOWN)

	ON_EVENT( CfrmUc091KaikakekinI, IDC_TITLEDIAG1, 3, CfrmUc091KaikakekinI::TerminationTitlediag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN )
	ON_EVENT( CfrmUc091KaikakekinI, IDC_TITLEDIAG1, 2, CfrmUc091KaikakekinI::EditOFFTitlediag1, VTS_I2 )
	ON_EVENT( CfrmUc091KaikakekinI, IDC_TITLEDIAG1, 1, CfrmUc091KaikakekinI::EditONTitlediag1, VTS_I2 )
	ON_EVENT( CfrmUc091KaikakekinI, IDC_TITLEDIAG1, 6, CfrmUc091KaikakekinI::FocusTitlediag1, VTS_NONE )

	ON_EVENT( CfrmUc091KaikakekinI, IDC_PAGEDIAG1, 3, CfrmUc091KaikakekinI::TerminationPagediag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN )
	ON_EVENT( CfrmUc091KaikakekinI, IDC_PAGEDIAG1, 2, CfrmUc091KaikakekinI::EditOFFPagediag1, VTS_I2 )
	ON_EVENT( CfrmUc091KaikakekinI, IDC_PAGEDIAG1, 1, CfrmUc091KaikakekinI::EditONPagediag1, VTS_I2 )
	ON_EVENT( CfrmUc091KaikakekinI, IDC_PAGEDIAG1, 6, CfrmUc091KaikakekinI::FocusPagediag1, VTS_NONE )
END_EVENTSINK_MAP()

//********************************************************************************
//	初期処理
//********************************************************************************
BOOL	CfrmUc091KaikakekinI::virStartJob()
{
	// 初期値設定
	if ( CmnSetInitialUchiwakeInfo() != TRUE){	// 共通データ
		return FALSE;
	}

	// ---- 登録番号一覧関連 ----->
	pZmSel = NULL;
	// 初期化
	InitZmSel();
	// ---- 登録番号一覧関連 <-----

	m_InvnoErrFlg = 0;	// 修正No.168443 add

	// 科目、銀行、取引先参照の番号が正しく連番になっているかチェック
	// 連番になっていない場合、修復処理を行う
	((CUCHIWAKEApp*)AfxGetApp())->OrderCheckEx(m_uInfo.intFormSeq);

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

	// 様式送りボタンの制御
	UpdateYoushikiMoveButton( m_YoushikiSkip );

	return TRUE;
}

//********************************************************************************
//	共通データの初期値設定
//********************************************************************************
void	CfrmUc091KaikakekinI::virSetInitialUchiwakeInfo()
{
	// 「買掛金」の固有データ
	m_uInfo.intOutKeiDialog = AM_TYPE_ALL;					// 計設定ダイアログのボタン指定
	m_uInfo.intRowMax = ID_ROWKOBETU_091;					// １頁内の合計行を含む全体の行数
	m_uInfo.intRowMaxData = ID_ROWKOBETU_091 - 1;			// １頁内のデータ行数（合計行は含まず）
	m_uInfo.intColMax = ID_COLMAX_091I;						// １行内のコントロール個数
	m_uInfo.intColMaxData = ID_COLMAXDATA_091I;				// １行内の列数
	m_uInfo.intCtlStartIndex = ID_CTLSTARTINDEX_091I;		// １行目の入力項目の先頭インデックス番号
	m_uInfo.intT1StartIndex = ID_T1STARTNORMAL_091I;		// 合計行①の先頭インデックス番号
	m_uInfo.intT2StartIndex = ID_T2STARTNORMAL_091I;		// 合計行②の入力項目の先頭インデックス番号
	m_uInfo.intCursolDefPos = ID_COL_091I_INVONO;			// カーソル移動時の初期位置（タイトルから↓，合計から↑で使用）
	m_uInfo.intCursolKeiPos = ID_COL_091I_KNORDER;			// 合計行のカーソル移動位置（カーソル移動で使用）

	m_uInfo.intFormSeq = ID_FORMNO_091;						// 様式シーケンス番号
	m_uInfo.intFormNo = 9;									// 様式番号（uc_inf_sub）
	m_uInfo.intFormNo2 = 1;									// 様式番号枝番（uc_inf_sub）

	m_uInfo.intRowNormal = ID_ROWNORMAL_091;				// 通常時：１頁内の合計行を含む全体の行数
	m_uInfo.intRowKobetu = ID_ROWKOBETU_091;				// 個別時：１頁内の合計行を含む全体の行数
	m_uInfo.intT1StartNormal = ID_T1STARTNORMAL_091I;		// 通常時：合計行①の先頭インデックス
	m_uInfo.intT2StartNormal = ID_T2STARTNORMAL_091I;		// 通常時：合計行②の先頭インデックス
	m_uInfo.intT1StartKobetu = ID_T1STARTKOBETU_091I;		// 個別時：合計行①の先頭インデックス
	m_uInfo.intT2StartKobetu = ID_T2STARTKOBETU_091I;		// 個別時：合計行②の先頭インデックス
	m_uInfo.intRowKei = ID_ROWKEI_091;						// 帳表固定の合計行数（1:合計あり，0:なし）
	m_uInfo.intKei1StartIndex = ID_COL_091I_KEI1_MEISYO;	// 計ダイアグラム1行目の先頭インデックス
	m_uInfo.intKei2StartIndex = ID_COL_091I_KEI2_MEISYO;	// 計ダイアグラム2行目の先頭インデックス

	// 集計処理の設定
	m_uInfo.uCalcInfo.intMaxCount = ID_CALC_091_MAX;		// 集計フィールド件数
	m_uInfo.uCalcInfo.strField[0] = ID_CALC_091_FIELD1;		// 集計フィールド名称1
}

//**************************************************
// ICSDiagコントロールを通常／個別で切り替える
// 【引数】    なし
// 【戻値】    なし
//**************************************************
void	CfrmUc091KaikakekinI::virSetInitialDiag()
{
	int		nIndex;

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

	//	コンボボックスの属性取得
	nIndex = CmnGetControlIndex( 1, ID_COL_091I_KNORDER );
	m_pDiag->GetAttribute( nIndex, ( LPUNKNOWN )&m_uInfo.uComboAttr );
}

//********************************************************************************
//	帳表固有変数の初期値設定
//********************************************************************************
void	CfrmUc091KaikakekinI::virSetInitialValue()
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
BOOL	CfrmUc091KaikakekinI::InitialControlDataArea()
{
	int		intMax;
	int		i;
	int		len=0;

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
		CmnDiagSetString(m_pDiag, i + ID_COL_091I_KNORDER - 1, "", 0);	// 科目
		CmnDiagSetString(m_pDiag, i + ID_COL_091I_INVONOT - 1, "", 0);	// 登録番号(T選択)
		
		// 漢字入力欄の初期化
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_091I_ADNAME1 - 1, MAX_KETA_091_ADNAME1 );		//	名称１
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_091I_ADADD1  - 1, MAX_KETA_091_ADADD1 );		//	住所１
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_091I_ADADD2  - 1, MAX_KETA_091_ADADD2 );		//	住所２
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_091I_TEKI    - 1, MAX_KETA_091_TEKI );		//	摘要

		// 金額欄：桁数初期化
		len = ID_VAL_091_K_DATA;
		CmnDiagSetAttrDigit( m_pDiag, i + ID_COL_091I_VAL - 1, len );	// 金額

		//	入力不可モードの解除
		SetInputMode( CmnGetControlRow( i ), DIAG_MDFY_INPUT );
	}

	// ComboBoxの初期設定
	if ( InitialControlComboBox() != TRUE ){
		return FALSE;
	}

	// 半角/全角設定
	CmnDiagSetAttrColZenHan( m_pDiag , ID_COL_091I_ADNAME1 );	// 名称1
	CmnDiagSetAttrColZenHan( m_pDiag , ID_COL_091I_ADADD1 );	// 所在地1
	CmnDiagSetAttrColZenHan( m_pDiag , ID_COL_091I_ADADD2 );	// 所在地2
	CmnDiagSetAttrColZenHan( m_pDiag , ID_COL_091I_TEKI );		// 摘要

	return TRUE;
}

//********************************************************************************
//	帳表コントロールの初期化（ComboBox）
//********************************************************************************
BOOL	CfrmUc091KaikakekinI::InitialControlComboBox()
{
	DIAGRAM_DATA	diadata;			// ICSDIAG構造体（科目）
	DIAGRAM_DATA	diadata2;			// ICSDIAG構造体（T選択）
	int				intTotalType;
	int				intMax;
	int				intIndex;
	int				intIndex2;
	int				i;
	BOOL			bFlg = FALSE;				// フォントサイズ変更する？

	// カレント頁の合計行の種別を取得
	intTotalType = CmnGetControlTotalType();

	// カレント頁で"頁計と累計"を表示する場合、対象行は"-1"となる
	switch(intTotalType) {
		case ID_OUTKEI_BOTH:			// 頁計と累計
			intMax = m_uInfo.intRowMaxData - 1;
			break;
		default:
			intMax = m_uInfo.intRowMaxData;
			break;
	}

	// 初期化
	m_clsFunc.DiagInit(&diadata);
	m_clsFunc.DiagInit(&diadata2);

	// 科目
	// ComboBoxの選択項目取得
	if(CmnUcLstKamokuGetComboList(&diadata.text_combo, bFlg) != TRUE) {
		return FALSE;
	}
	// T選択
	diadata2.text_combo = _T("T\r\n \r\n");

	// 全行設定
	for(i = 1; i <= intMax; i++) {
		// 対象コントロールのインデックス番号を取得
		// 科目
		intIndex = CmnGetControlIndex(i, ID_COL_091I_KNORDER);
		// T選択
		intIndex2 = CmnGetControlIndex(i, ID_COL_091I_INVONOT);

		// 初期設定（ドロップダウンON＋リスト情報をセット）
		// 科目
		m_pDiag->ModifyItem(intIndex, DIAG_MDFY_NODROPDOWN);
		m_pDiag->SetData(intIndex, (LPUNKNOWN)&diadata);
		// T選択
		m_pDiag->ModifyItem(intIndex2, DIAG_MDFY_NODROPDOWN);
		m_pDiag->SetData(intIndex2, (LPUNKNOWN)&diadata2);

		// フォントサイズ設定
		if(m_EndView == FALSE && m_ChangeFont != FALSE && m_ImportMode == FALSE) {
			// 科目
			SetComboFontSize(m_pDiag, intIndex, bFlg);
			// T選択
			m_pDiag->ChangeFontSize(intIndex2, m_TselFSize);
		}
	}
	m_ChangeFont = FALSE;

	return TRUE;
}

//********************************************************************************
//	帳表コントロールの初期化（頁計/累計領域）
//		文言はデータに含まれているので、その内容を使用する
//********************************************************************************
void	CfrmUc091KaikakekinI::InitialControlTotalArea()
{
	int		nKeiType;			//	計の種別
	int		nDataRow;			//	データ行属性
	int		nDataRowPos;		//	データ行位置
	int		nKeiRow[2];			//	計行属性
	int		nCnt;				//	カウント
	int		nStart;				//	開始位置
	int		intIndex=0;
	int		nSw=0;
	int		flg2=0;

	intIndex = CmnGetControlIndex(m_uInfo.intCurRow,m_uInfo.intCurCol);

	// カレント頁の合計行の種別を取得
	nKeiType = CmnGetControlTotalType();
	
	nDataRowPos = m_uInfo.intRowMax - 2;

	//	計の種別で分岐
	switch ( nKeiType ){
		case ID_OUTKEI_OFF:				// なし
		case ID_OUTKEI_PAGEKEI:			// 頁計
		case ID_OUTKEI_RUIKEI:			// 累計
			//	データ行の設定
			if(m_DataKakutei == FALSE)	{
				if(IsSpecialRow(m_ReadData[nDataRowPos+1].m_FgFunc) != 0)	{
					nDataRow = (DIAG_MDFY_NOINPUT | DIAG_MDFY_OPAQUE);
					if(IsSpecialRow2(m_ReadData[nDataRowPos+1].m_FgFunc) != 0)	{
						flg2 = 1;
					}
				}
				else	{
					nDataRow = (DIAG_MDFY_EDIT | DIAG_MDFY_OPAQUE);
				}
			}
			else						nDataRow = (DIAG_MDFY_READONLY | DIAG_MDFY_OPAQUE);
			//	計行の設定
			nKeiRow[0] = ( DIAG_MDFY_READONLY | DIAG_MDFY_TRANSPARENT );
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
			break;
	}

	// 28行目のデータ行
	nStart = nDataRowPos * m_uInfo.intColMax;
	for ( nCnt = 0; nCnt < m_uInfo.intColMax; nCnt++){
		m_pDiag->ModifyItem( nStart + nCnt, nDataRow );
		if((intIndex == (nStart + nCnt)) && ((nDataRow & DIAG_MDFY_READONLY) != 0))	nSw=1;
	}

	if(flg2 == 1)	{
		// 一括自動行の科目を入力可能にする
		m_pDiag->ModifyItem( CmnGetControlIndex( (nDataRowPos+1), ID_COL_091I_KNORDER ), (DIAG_MDFY_EDIT | DIAG_MDFY_OPAQUE) );
	}

	if(nSw != 0)	{
		// 現在のカーソルポジションが入力不可になった場合は先頭に戻す
		m_uInfo.intCurRow = 1;
		m_uInfo.intCurCol = 1;
	}

	// 計専用ダイアグラムの金額欄を入力不可にする
	int intKeiRow = (DIAG_MDFY_READONLY | DIAG_MDFY_OPAQUE);
	m_pDiagKei->ModifyItem(ID_COL_091I_KEI1_VAL, intKeiRow);
	m_pDiagKei->ModifyItem(ID_COL_091I_KEI2_VAL, intKeiRow);
}

//********************************************************************************
//	帳表コントロールの特殊行初期化
//	IN	int		行番号
//		int		特殊行フラグ（FgFunc）
//	RET	なし
//********************************************************************************
void	CfrmUc091KaikakekinI::InitialControlSpecialRow( int intRow, int intFgFunc )
{
	int				len=0;
	int				nIndex;		//	インデックス
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
		case ID_FGFUNC_RUIKEI:			//	頁計
		case ID_FGFUNC_PAGEKEI:			//	累計
			// 桁数を設定
			len = ID_VAL_091_K_TOTAL;
			CmnDiagSetAttrDigit( m_pDiag, CmnGetControlIndex( intRow , ID_COL_091I_VAL ), len );
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
			if(intFgFunc != ID_FGFUNC_IKKATUAUTO) {
				// 「科目」はComboBoxのため、DropDown不可とする
				m_pDiag->ModifyItem(CmnGetControlIndex(intRow, ID_COL_091I_KNORDER), DIAG_MDFY_NODROPDOWN);
				// 「登録番号(T選択)」はComboBoxのため、DropDown不可とする
				m_pDiag->ModifyItem(CmnGetControlIndex(intRow, ID_COL_091I_INVONOT), DIAG_MDFY_NODROPDOWN);

				// ComboBoxの選択項目クリア
				m_clsFunc.DiagInit(&diadata);
				diadata.text_combo = _T("\r\n");
				m_pDiag->SetData(CmnGetControlIndex(intRow, ID_COL_091I_KNORDER), (LPUNKNOWN)&diadata);
				m_pDiag->SetData(CmnGetControlIndex(intRow, ID_COL_091I_INVONOT), (LPUNKNOWN)&diadata);
			}

			//	入力不可モードの設定
			if(m_DataKakutei == FALSE)	SetInputMode(intRow,DIAG_MDFY_NOINPUT);
			else						SetInputMode(intRow,DIAG_MDFY_READONLY);

// 改良No.21-0086,21-0529 cor -->
			//if(KamokuRowEnableSgn(m_pDB,1) == 1)	{
// ------------------------------
			if(KamokuRowEnableSgn(m_pDB, 1, m_uInfo.intFormSeq) == 1)	{
// 改良No.21-0086,21-0529 cor <--
				// 一括集計金額（自動）行の場合、科目のみ入力可能とする
				if (intFgFunc == ID_FGFUNC_IKKATUAUTO ){
					//	科目
					m_pDiag->ModifyItem( CmnGetControlIndex( intRow, ID_COL_091I_KNORDER ), DIAG_MDFY_INPUT );
				}
			}
			break;
	
		case ID_FGFUNC_RUIKEI:		//	頁計
		case ID_FGFUNC_PAGEKEI:		//	累計
			//	何もしない
			break;

		//	特殊行以外
		default:
			break;
	}
}

//********************************************************************************
//	帳表タイトル情報の表示
//********************************************************************************
void	CfrmUc091KaikakekinI::virUpdateControlTitle()
{
	DIAGRAM_ATTRIBUTE diaatt;

	// 帳表ナンバーの背景色設定
	CmnDiagSetAttrColor( &m_NumberDiag, 0, &m_ViewColor );

	// 帳表ナンバー表示
	CmnDiagSetString( &m_NumberDiag, 0, m_uInfo.strTitleNumber, 1 );

	// 帳表タイトル表示
	CmnDiagSetString( &m_TitleDiag, 0, m_uInfo.strTitleName, 0 );

	//	タイトル入力最大文字数設定
	CmnDiagSetAttrInpmax( &m_TitleDiag, 0, ID_TITLE_INPMAX );
	
	long	nMode;		//	入力不可モードフラグ

	// 全ての様式において、編集可能
	nMode = DIAG_MDFY_INPUT;	//	入力可

	//	入力不可モード切り替え
	m_TitleDiag.ModifyItem( 0, nMode );

	// 帳表タイトル_半角/全角設定
	m_TitleDiag.GetAttribute( 0 , (LPUNKNOWN)&diaatt );
	diaatt.attr_editattr = diaatt.attr_editattr | DIAG_EATTR_ZENHAN;
	m_TitleDiag.SetAttribute( 0 , (LPUNKNOWN)&diaatt , TRUE );

	// 保管ﾃﾞｰﾀ件数の背景色設定
	CmnDiagSetAttrColor(&m_SaveDataDiag, 0, &m_ViewColor);
}

//********************************************************************************
//	帳表ページ情報の表示
//********************************************************************************
void	CfrmUc091KaikakekinI::UpdateControlPage()
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
	PostMessage(WM_USER_CONTROLPAGE, bBack, bNext);
}

//********************************************************************************
//	帳表データの表示（１ページ分）
//********************************************************************************
BOOL	CfrmUc091KaikakekinI::virUpdateControlTblData()
{
	int len = 0;

	// ダイアログ切替
	if( m_pDiag != NULL ){
		virSetInitialDiag();
	}

	CdbUc091Kaikakekin	dbRec( m_pDB );

	m_pDiag->DeleteInput();		//	入力ボックス削除 

	// 頁情報の表示
	UpdateControlPage();

	// コントロールのクリア＋初期設定
	if ( InitialControlDataArea() != TRUE ){	// データ行
		return FALSE;
	}

	// 頁計、累計行の桁数をセットする
	len = ID_VAL_091_K_TOTAL;
	CmnDiagSetAttrDigit( m_pDiagKei, ID_COL_091I_KEI1_VAL, len );
	CmnDiagSetAttrDigit( m_pDiagKei, ID_COL_091I_KEI2_VAL, len );

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

		// 電子申告の文字数を超えていれば、項目の文字色を変える
		CmnInitialControlFgStringCell( m_pDiag, dbRec.m_NumRow, ID_COL_091I_KNORDER, dbRec.m_FgFunc, UC_KAMOKU_LEN, dbRec.m_KnName);

		dbRec.MoveNext();
	}

	// 閉じる
	dbRec.Fin();

	InitialControlTotalArea();				// 頁計/累計行

	// 保管ﾃﾞｰﾀ件数の表示
	virUpdateControlTblData2();

	// 項目タイトル(ヘッダ部)の再設定
	TitleColorChg();

	// 画面更新
	m_pDiag->Refresh();
	m_pDiagKei->Refresh();

	return TRUE;
}

//********************************************************************************
//	保管件数の表示
//********************************************************************************
BOOL CfrmUc091KaikakekinI::virUpdateControlTblData2()
{
	int					hcnt=0;
	CString				cs=_T("");

	hcnt=virGetDataTableObject()->GetCountFgShow(ID_FGSHOW_HOKAN);
	cs.Format(_T("保管%3d件"),hcnt);
	CmnDiagSetString(&m_SaveDataDiag,0,cs,1);

	return TRUE;
}

//********************************************************************************
//	帳表データの１行表示（空行，データ行，特殊行）
//		IN		CdbUc091Kaikakekin*	「買掛金」
//		RET		なし
//********************************************************************************
void	CfrmUc091KaikakekinI::UpdateControlRowData( CdbUc091Kaikakekin* prmDbRec ) {

	DIAGRAM_DATA		diadata;		// ICSDiag構造体
	int					intCell;		// 保存フィールド
	CString				cs1, cs2, cst;

	// 初期化
	m_clsFunc.DiagInit( &diadata );
	// 読込み対象セル取得
	intCell = CmnGetControlIndex( prmDbRec->m_NumRow, ID_COL_091I_KNORDER );

	// 科目
	//	データ行？
	switch(prmDbRec->m_FgFunc) {
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
			// KnOrder（科目順序）をキーにコンボボックスのインデックスを取得
			m_SortMap1.Lookup(prmDbRec->m_KnOrder, diadata.data_combo);
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

	// 登録番号を分割
	cs1 = cs2 = _T("");
	if(prmDbRec->m_InvNo.IsEmpty() == FALSE) {
		InvoNoSplit(prmDbRec->m_InvNo, &cs1, &cs2);
	}

	// 登録番号（T選択）	ID_COL_091I_INVONOT
	m_clsFunc.DiagInit(&diadata);
	switch(prmDbRec->m_FgFunc)	{
		//------------------------------
		//	空行
		case ID_FGFUNC_NULL:
		//	データ行
		case ID_FGFUNC_DATA:
		//	一括金額（手動）
		case ID_FGFUNC_IKKATUMANUAL:
		//------------------------------
			if(strcmp(cs1, _T(" ")) == 0)	diadata.data_combo = 1;
			else							diadata.data_combo = 0;
			break;
		//------------------------------
		//	一括金額（自動）
		case ID_FGFUNC_IKKATUAUTO:
		//	小計
		case ID_FGFUNC_SYOKEI:
		//	小計（空行）
		case ID_FGFUNC_SYOKEINULL:
		//	中計
		case ID_FGFUNC_CHUKEI:
		//	中計計（空行）
		case ID_FGFUNC_CHUKEINULL:
		//------------------------------
			diadata.data_combo = 1;
			break;
		//------------------------------
		default:
			break;
	}
	m_pDiag->SetData(intCell, (LPUNKNOWN)&diadata);
	intCell++;

	// 登録番号			ID_COL_091I_INVONO
	m_clsFunc.DiagInit(&diadata);
	switch(prmDbRec->m_FgFunc)	{
		//------------------------------
		//	空行
		case ID_FGFUNC_NULL:
		//	データ行
		case ID_FGFUNC_DATA:
		//	一括金額（手動）
		case ID_FGFUNC_IKKATUMANUAL:
		//------------------------------
			diadata.data_edit = cs2;
			break;
		//------------------------------
		default:
			break;
	}
	m_pDiag->SetData(intCell, (LPUNKNOWN)&diadata);
	intCell++;

	//	名称１
	//	データ行？
	switch(prmDbRec->m_FgFunc) {
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
			diadata.data_edit = m_clsFunc.StrDocking(prmDbRec->m_AdName1, prmDbRec->m_AdName2);
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
	diadata.data_edit = prmDbRec->m_Teki;
	m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
}

//********************************************************************************
//	帳表データの１行表示（累計，頁計行）
//		IN		CdbUc091Kaikakekin*	「買掛金」テーブル情報
//		RET		なし
//********************************************************************************
void	CfrmUc091KaikakekinI::UpdateControlRowTotal( CdbUc091Kaikakekin* prmDbRec ) {

	int		intTotalType;
	int		intRowName = 0;		// 名称表示行（0:非表示）
	int		intRowVal = 0;		// 期末現在高表示行（0:非表示）

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
		CmnDiagSetString( m_pDiagKei, CmnGetKeiDiagIndex( intRowName, ID_COL_091I_KEI1_MEISYO ), prmDbRec->m_KeiStr, 1 );
	}
	// 頁計/累計の金額表示
	if ( intRowVal > 0 ) {
		CmnDiagSetKingaku( m_pDiagKei, CmnGetKeiDiagIndex( intRowVal, ID_COL_091I_KEI1_VAL ), prmDbRec->m_Val );
	}
}

//********************************************************************************
//	【F6:項目複写】１つ前のデータ表示処理
//		IN		CdbUc000Common*	共通テーブルクラス
//		RET		なし
//********************************************************************************
void	CfrmUc091KaikakekinI::virFncCellCopyProc( CdbUc000Common* rsData )
{
	CdbUc091Kaikakekin*	rs;
	int					intCol;	// 現在の列
	int					ind1,ind2;
	CString				cs1,cs2;

	rs = (CdbUc091Kaikakekin*)rsData;

	// 列取得
	intCol = CmnGetControlCol();

	// 列によって処理を分岐
	switch( intCol ){
		// 科目
		case ID_COL_091I_KNORDER:
			CmnDiagSetCombo2( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_KnOrder );
			break;
		// 登録番号（T選択）
		case ID_COL_091I_INVONOT:
		// 登録番号
		case ID_COL_091I_INVONO:
			if(intCol == ID_COL_091I_INVONOT) {
				ind1 = m_uInfo.intCurCtlIndex;
				ind2 = m_uInfo.intCurCtlIndex+1;
			}
			else {
				ind1 = m_uInfo.intCurCtlIndex-1;
				ind2 = m_uInfo.intCurCtlIndex;
			}
			// 登録番号選択
			CmnDiagSetCombo3(m_pDiag, ind1, rs->m_InvNo);
			// 登録番号
			InvoNoSplit(rs->m_InvNo, &cs1, &cs2);
			CmnDiagSetString(m_pDiag, ind2, cs2, 0);
			break;
		// 名称1
		case ID_COL_091I_ADNAME1:
			CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, m_clsFunc.StrDocking( rs->m_AdName1,rs->m_AdName2 ) , 0 );
			break;
		// 所在地1
		case ID_COL_091I_ADADD1:
			CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_AdAdd1, 0 );
			break;
		// 所在地2
		case ID_COL_091I_ADADD2:
			CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_AdAdd2, 0 );
			break;
		// 金額
		case ID_COL_091I_VAL:
			CmnDiagSetKingaku( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_Val );
			break;
		// 摘要
		case ID_COL_091I_TEKI:
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
int	CfrmUc091KaikakekinI::virFncReferenceGetType(int intCol, int* intGrSeq, long* lngItemSeq, int* intAdd1, int* intAdd2, int* intAdd1Col, int* intAdd2Col, REFERENCE_INFO* pudReference)
{
	int				intRefType;

	switch (intCol) {
		// 種類
		case ID_COL_091I_KNORDER:
			intRefType = ID_DLGTYPE_KAMOKU;
			*intGrSeq = ID_ADDRESSGR_NONE;
			*lngItemSeq = m_ReadData[m_uInfo.intCurRow].m_KnSeq;
			break;

		// 登録番号(法人番号)
		case ID_COL_091I_INVONOT:
		case ID_COL_091I_INVONO:
			intRefType = ID_DLGTYPE_INVOICENO;
			*intGrSeq = ID_ADDRESSGR_KAI;
			*lngItemSeq = m_ReadData[m_uInfo.intCurRow].m_AdSeq;
			break;

		//	取引先
		case ID_COL_091I_ADNAME1:
			intRefType = ID_DLGTYPE_ADDRESS;
			*intGrSeq = ID_ADDRESSGR_KAI;
			*lngItemSeq = m_ReadData[m_uInfo.intCurRow].m_AdSeq;
			break;

		// 所在地
		case ID_COL_091I_ADADD1:
		case ID_COL_091I_ADADD2:
			intRefType = ID_DLGTYPE_ICSADDSEARCH;
			*intAdd1 = MAX_KETA_091_ADADD1 * 2;			// Byte指定
			*intAdd2 = MAX_KETA_091_ADADD2 * 2;			// Byte指定
			*intAdd1Col = ID_COL_091I_ADADD1;
			*intAdd2Col = ID_COL_091I_ADADD2;
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
void	CfrmUc091KaikakekinI::virFncReferenceSetData( CdlgReference* dlgRef, int intRefType, int intCol )
{
	int			nCurRow;		//	カレント行
	int			nCurCol;		//	カレント列
	int			nPos;			//	位置
	CString		cst,str1,str2;

	switch (intRefType) {
		// 種類
		case ID_DLGTYPE_KAMOKU:
			// 選択内容を表示
			CmnDiagSetCombo2( m_pDiag, m_uInfo.intCurCtlIndex, dlgRef->m_udKamoku.m_KnOrder );
			break;

		//	取引先
		case ID_DLGTYPE_ADDRESS:
			// カレント行＋列番号を取得
			CmnGetControlPosition(&nCurRow, &nCurCol);
			if(intCol == ID_COL_091I_INVONOT) {
				//	現在行の取引先の名称１の位置を算出
				//nPos = ((nCurRow - 1) * m_uInfo.intColMax) + (m_uInfo.intCtlStartIndex + (ID_COL_091I_INVONO - 1));	// 修正No.168412 del
				nPos = ((nCurRow - 1) * m_uInfo.intColMax) + (m_uInfo.intCtlStartIndex + (ID_COL_091I_INVONOT - 1));	// 修正No.168412 add

				//	選択内容を表示
				CmnDiagSetCombo3(m_pDiag, nPos, dlgRef->m_udAddress.m_InvNo);									//	登録番号（T選択）
				InvoNoSplit(dlgRef->m_udAddress.m_InvNo, &str1, &str2);
				CmnDiagSetString(m_pDiag, nPos + 1, str2, 0);													//	登録番号
				CmnDiagSetString(m_pDiag, nPos + 2, m_clsFunc.StrDocking(dlgRef->m_udAddress.m_AdName1,
																		 dlgRef->m_udAddress.m_AdName2), 0);	//	名称１
				CmnDiagSetString(m_pDiag, nPos + 3, dlgRef->m_udAddress.m_AdAdd1, 0);							//	住所１
				CmnDiagSetString(m_pDiag, nPos + 4, dlgRef->m_udAddress.m_AdAdd2, 0);							//	住所２
			}
			else if(intCol == ID_COL_091I_INVONO) {
				//	現在行の取引先の名称１の位置を算出
				nPos = ((nCurRow - 1) * m_uInfo.intColMax) + (m_uInfo.intCtlStartIndex + (ID_COL_091I_INVONO - 1));

				//	選択内容を表示
				CmnDiagSetCombo3(m_pDiag, nPos - 1, dlgRef->m_udAddress.m_InvNo);								//	登録番号（T選択）
				InvoNoSplit(dlgRef->m_udAddress.m_InvNo, &str1, &str2);
				CmnDiagSetString(m_pDiag, nPos, str2, 0);														//	登録番号
				CmnDiagSetString(m_pDiag, nPos + 1, m_clsFunc.StrDocking(dlgRef->m_udAddress.m_AdName1,
																		 dlgRef->m_udAddress.m_AdName2), 0);	//	名称１
				CmnDiagSetString(m_pDiag, nPos + 2, dlgRef->m_udAddress.m_AdAdd1, 0);							//	住所１
				CmnDiagSetString(m_pDiag, nPos + 3, dlgRef->m_udAddress.m_AdAdd2, 0);							//	住所２
			}
			else if(intCol == ID_COL_091I_ADNAME1) {
				//	現在行の取引先の名称１の位置を算出
				nPos = ((nCurRow - 1) * m_uInfo.intColMax) + (m_uInfo.intCtlStartIndex + (ID_COL_091I_ADNAME1 - 1));

				//	選択内容を表示
				CmnDiagSetCombo3(m_pDiag, nPos - 2, dlgRef->m_udAddress.m_InvNo);								//	登録番号（T選択）
				InvoNoSplit(dlgRef->m_udAddress.m_InvNo, &str1, &str2);
				CmnDiagSetString(m_pDiag, nPos - 1, str2, 0);													//	登録番号
				CmnDiagSetString(m_pDiag, nPos, m_clsFunc.StrDocking(dlgRef->m_udAddress.m_AdName1,
																	 dlgRef->m_udAddress.m_AdName2), 0);		//	名称１
				CmnDiagSetString(m_pDiag, nPos + 1, dlgRef->m_udAddress.m_AdAdd1, 0);							//	住所１
				CmnDiagSetString(m_pDiag, nPos + 2, dlgRef->m_udAddress.m_AdAdd2, 0);							//	住所２
			}
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
void	CfrmUc091KaikakekinI::virFncReferenceSetICSData(CdlgAddressSearch* dlgAddd)
{
	int			intIndex;

	// 売却(買入)先の所在地1(住所)
	intIndex = CmnGetControlIndex( m_uInfo.intCurRow, ID_COL_091I_ADADD1 );
	CmnDiagSetString( m_pDiag, intIndex, dlgAddd->m_AdAdd1, 0 );

	// 売却(買入)先の所在地2(住所)
	intIndex = CmnGetControlIndex( m_uInfo.intCurRow, ID_COL_091I_ADADD2 );
	CmnDiagSetString( m_pDiag, intIndex, dlgAddd->m_AdAdd2, 0 );
}

//********************************************************************************
//	【F9:参照】参照ダイアログの操作によりテーブルを更新
//		IN		int			参照ダイアログの種別
//		RET		なし
//********************************************************************************
void	CfrmUc091KaikakekinI::virFncReferenceUpdateTbl( int intRefType, int nRefMode/*=0*/ )
{
	switch (intRefType) {
		// 科目
		case ID_DLGTYPE_KAMOKU:
			{
				// "uc_091_Kaikakekin"の更新
				CdbUcLstKamoku	rsKmk( m_pDB );
				rsKmk.UpdateDataTable(m_uInfo.intFormSeq, nRefMode);
				rsKmk.Fin();
				break;
			}

		//	取引先
		case ID_DLGTYPE_ADDRESS:
			{
				CdbUcLstAddress		mfcRec( m_pDB );
				mfcRec.UpdateDataTableAll_Inv(m_RegAutoRef);
				mfcRec.Fin(); 
				break;
			}

		default:
			break;
	}	
}

void CfrmUc091KaikakekinI::virActivateFrameK(UINT nState, CFrameWnd* pDeactivateFrame)
{
	int		index = 0;
	CWnd*	pWnd = this->GetFocus();

	if(pWnd != NULL)	{
		if(m_DataKakutei == FALSE)	{
			if(m_uInfo.intCurCol == ID_COL_091I_KNORDER)	{
				virKamokuAddCancel();
			}
		}
	}
}

void CfrmUc091KaikakekinI::virKamokuAddCancel()
{
	if(m_DataKakutei == TRUE)	return;

	// ポジション取得
	m_uInfo.intCurCtlIndex = m_pDiag->GetPosition();
	// 行・列取得
	CmnGetControlPosition(&m_uInfo.intCurRow,&m_uInfo.intCurCol,&m_uInfo.OldCol);
	// 科目列なら
	if(m_uInfo.intCurCol == ID_COL_091I_KNORDER && m_F9OnSw != TRUE)	{
		DIAGRAM_DATA	diadata;
		m_pDiag->GetData(m_uInfo.intCurCtlIndex,(LPUNKNOWN)&diadata);
		// 【新規作成】が選択されていれば
		if(diadata.data_combo > m_CmbCnt)	{
			// 前回の値に戻す
			CmnDiagSetCombo(m_pDiag,CmnGetControlIndex(m_uInfo.intCurRow,ID_COL_091I_KNORDER),m_uInfo.OldCombo);
		}
	}
}

//********************************************************************************
//	その他イベント処理
//********************************************************************************
BOOL	CfrmUc091KaikakekinI::PreTranslateMessage( MSG* pMsg )
{
	BOOL			blnMove = TRUE;		// カーソル移動可能/不可フラグ
	int				intOrgIndex;		// カーソル移動前のインデックス番号
	int				intMoveRow;			// カーソル移動先（行）
	int				intMoveCol;			// カーソル移動先（列）
	int				rv=0;
	int				intRet=0;
	int				id=0;
	int				nMaxDataCol=0;		// 1頁の最大行数
	int				intTotalType=0;		// 合計タイプ
	int				intCol=0;
	DIAGRAM_DATA	diadata;
	CWnd* pWnd = this->GetFocus();

	if ( pWnd != NULL ){
		id = pWnd->GetDlgCtrlID();
		if(id == IDC_PAGEBACK_BUTTON1 || id == IDC_PAGENEXT_BUTTON1 || id == IDC_PAGENEW_BUTTON1 || 
			id == IDC_PAGEINSERT_BUTTON1 || id == IDC_ROWCOPY_BUTTON1 || id == IDC_ROWPASTE_BUTTON1 || 
			id == IDC_ROWINSERT_BUTTON1 || id == IDC_YOUSHIKIBACK_BUTTON1 || id == IDC_YOUSHIKINEXT_BUTTON1 ||
			id == IDC_MARGINAL_BUTTON1)	{
			nG_MScroll = 0;
		}
	}

	//	キーが押された？
	if ( pMsg->message == WM_KEYDOWN ){
		//	パラメータで分岐
		switch( pMsg->wParam ){
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
			// PageUp，PageDownキー
			case VK_PRIOR:
			case VK_NEXT:
				if(m_DataKakutei == FALSE)	{
// 修正No.168362 add -->
					// ---- 登録番号一覧関連 ----->
					if(pZmSel && pZmSel->IsDisplay() != FALSE)	{
						if(pMsg->wParam == VK_NEXT) {
							pZmSel->PageDown();
						}
						else if(pMsg->wParam == VK_PRIOR) {
							pZmSel->PageUp();
						}
						rv = 1;
					}
					// <--- 登録番号一覧関連 ------
					else {
// 修正No.168362 add <--
						// データ入力欄にフォーカスある場合のみ実行を許す
						if (m_uInfo.intCurFocus != 1 && nG_MScroll == 0) {
							// 何もせずに抜ける
							break;
						}

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

						intCol = CmnGetControlCol();
						// 例外処理：移動前に【新規作成】が選ばれている場合は値を元に戻す
						if (intCol == ID_COL_091I_KNORDER) {
							m_pDiag->GetData( m_uInfo.intCurCtlIndex , (LPUNKNOWN)&diadata );
							if(diadata.data_combo > m_CmbCnt)	{
								m_pDiag->ComboDropDown(m_uInfo.intCurCtlIndex, FALSE);
								CmnDiagSetCombo(m_pDiag, m_uInfo.intCurCtlIndex, m_uInfo.OldCombo);
							}
						}
						// 例外処理：登録番号(T)選択でPageUp，PageDownキーが押下された場合は値を元に戻す
						if(intCol == ID_COL_091I_INVONOT) {
							m_pDiag->ComboDropDown(m_uInfo.intCurCtlIndex, FALSE);
							CmnDiagSetCombo(m_pDiag, m_uInfo.intCurCtlIndex, m_uInfo.OldCombo);
						}

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
							// カレント頁の合計行の種別を取得
							intTotalType = CmnGetControlTotalType();
							// カレント頁で"頁計と累計"を表示する場合、対象行は"-1"となる
							// 頁計と累計
							if(intTotalType == ID_OUTKEI_BOTH)	nMaxDataCol = m_uInfo.intRowMaxData - 1;
							else								nMaxDataCol = m_uInfo.intRowMaxData;

							if (intMoveRow > nMaxDataCol) {
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
						//if(intCol == ID_COL_091I_KNORDER)	{
						if(intCol == ID_COL_091I_KNORDER || intCol == ID_COL_091I_INVONOT)	{
							rv=1;
						}
// 修正No.168362 add -->
					} 
// 修正No.168362 add <--
				}
				else	{
					// Scroll
					if (pMsg->wParam == VK_PRIOR)	m_pDiag->ScrollPos(SB_PAGEUP , 0);
					else							m_pDiag->ScrollPos(SB_PAGEDOWN, 0);
				}
				break;

			// カーソル移動キー
			case VK_TAB:
			case VK_UP:
			case VK_DOWN:
			case VK_LEFT:
			case VK_RIGHT:
				if(nG_MScroll == 1)	{
					rv = CursorControl((short)pMsg->wParam,1);
				}
				else if(m_DataKakutei == FALSE && m_Pagefocus == 0 && m_Titlefocus == 0)	{
					intCol = CmnGetControlCol();
					if(intCol == ID_COL_091I_KNORDER && (pMsg->wParam == VK_LEFT || pMsg->wParam == VK_RIGHT))	{
						rv = CursorControl((short)pMsg->wParam, 0);
					}
				}
				break;

// 修正No.168362 add -->
			case VK_HOME:
				// ---- 登録番号一覧関連 ----->
				if(pZmSel && pZmSel->IsDisplay() != FALSE) {
					pZmSel->PageChange();
					rv = 1;
				}
				// <--- 登録番号一覧関連 ------
				break;
// 修正No.168362 add <--

			case VK_ESCAPE:
				// ---- 登録番号一覧関連 ----->
				if(pZmSel && pZmSel->IsDisplay() != FALSE) {
					pZmSel->DialogOFF();
					rv = 1;
				}
				// <--- 登録番号一覧関連 ------
				break;
		}
	}
	// マウス左クリック
	else if(pMsg->message == WM_LBUTTONDOWN)	{
		// 科目コンボボックスで【新規追加】が選択されたままタイトル部やページ番号部にカーソルがセットされた時、
		// コンボボックスの値を元に戻す
		int		id=-1;
		POINT	pt;
		RECT	rt;

		// 現在のカーソルポジションを取得する
		GetCursorPos(&pt);
		ScreenToClient(&pt);

		m_btnMarjinal.GetWindowRect(&rt);
		ScreenToClient(&rt);
		if(pt.x >= rt.left && pt.x <= rt.right && pt.y >= rt.top && pt.y <= rt.bottom) {
			id = IDC_MARGINAL_BUTTON1;		// ⑤棚卸方法登録ボタン/⑨欄外登録ボタン
		}
		if(id != -1)	{
			CmnEventStartProc(id);
		}
	}
// 修正No.168362 add -->
	else if(pMsg->message == WM_MOUSEWHEEL)	{
		// ---- 登録番号一覧関連 ----->
		if(pZmSel && pZmSel->IsDisplay() != FALSE)	{
			rv = 1;
		}
		// <--- 登録番号一覧関連 ------
	}
// 修正No.168362 add <--

	if(rv == 1)	{
		return(TRUE);
	}
	return CfrmUc000Common::PreTranslateMessage(pMsg);
}

//********************************************************************************
//	頁コントロール処理（Focus，EditOFF，Terminationイベント）
//********************************************************************************

// 頁コントロールのフォーカス設定
void	CfrmUc091KaikakekinI::FocusPagediag1()
{
}

// 頁コントロールのゲットフォーカス
void	CfrmUc091KaikakekinI::EditONPagediag1( short index )
{
// 修正No.168443 add -->
	// ---- 登録番号一覧関連 ----->
	if(pZmSel && pZmSel->IsDisplay() != FALSE) {
		pZmSel->DialogOFF();
	}
	// ---- 登録番号一覧関連 <-----
// 修正No.168443 add <--

	// スクロールフラグをクリアする
	nG_MScroll = 0;
	m_Pagefocus = 1;

	// 1行登録
	if (virRecBufSaveData(m_uInfo.intCurPage, m_uInfo.intCurRow) == FUNCTION_UPDATE) {
		m_PageDiag.SetPosition(1);
	}
}

// 頁コントロールのロストフォーカス
void	CfrmUc091KaikakekinI::EditOFFPagediag1( short index )
{
	// 会社切替時にメッセージを表示した場合、EditOffが走りDB書き込み時にエラーが発生していた
	if(m_EndView != FALSE)	return;

	// 頁入力値を取得
	int	intTempPage = CmnDiagGetValue( &m_PageDiag, 1 );

	m_Pagefocus = 0;
	// 頁が変更された場合は画面更新が必要
	if ( m_uInfo.intCurPage != intTempPage ) {
		if ( ( intTempPage >= 1 ) && ( intTempPage <= m_uInfo.intMaxPage ) ) {
			// 範囲内：新ページに更新
			m_uInfo.intCurPage = intTempPage;

			m_ChangeFont = TRUE;

			// データを再描画してフォーカスをデータ入力欄に
			PostMessage(WM_USER_REDRAWVIEW, 0, 0);

			return;
		}
	}

	// ページ情報のみ更新："0001"→"1"で表示したい
	UpdateControlPage();
}

// 頁コントロールのキー操作
void	CfrmUc091KaikakekinI::TerminationPagediag1( short index, short nChar, short length, LPUNKNOWN data )
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
			if(m_DataKakutei == FALSE)	m_TitleDiag.SetPosition(0);
		}
		else {
			// Shift+Tab：データ部の最終行へ移動（頁計/累計行の制御を行う）
			CmnDiagSetFocus(m_pDiag, nMaxDataCol, ID_COL_091I_TEKI);
		}
		break;
	case VK_DOWN:
		// タイトルコントロールへ移動
		if(m_DataKakutei == FALSE)	m_TitleDiag.SetPosition(0);
		break;
	}
}

//********************************************************************************
//	タイトルコントロール処理（Focus，EditOFF，Terminationイベント）
//********************************************************************************

// タイトルコントロールのフォーカス設定
void	CfrmUc091KaikakekinI::FocusTitlediag1()
{
}

// タイトルコントロールのゲットフォーカス
void	CfrmUc091KaikakekinI::EditONTitlediag1( short index )
{
// 修正No.168443 add -->
	// ---- 登録番号一覧関連 ----->
	if(pZmSel && pZmSel->IsDisplay() != FALSE) {
		pZmSel->DialogOFF();
	}
	// ---- 登録番号一覧関連 <-----
// 修正No.168443 add <--

	// スクロールフラグをクリアする
	nG_MScroll = 0;
	m_Titlefocus = 1;
	// 1行登録
	if (virRecBufSaveData(m_uInfo.intCurPage, m_uInfo.intCurRow) == FUNCTION_UPDATE) {
		m_TitleDiag.SetPosition(0);
	}
}

// タイトルコントロールのロストフォーカス
void	CfrmUc091KaikakekinI::EditOFFTitlediag1 (short index )
{
	m_Titlefocus = 0;
	virUpdateTitle();
}

//	タイトル更新
void CfrmUc091KaikakekinI::virUpdateTitle()
{
	// 帳表タイトル取得してuc_inf_subを更新
	m_uInfo.strTitleName = m_clsFunc.DiagGetString( &m_TitleDiag, 0 );
	CmnUcInfSubSetTitleInfo( TRUE );
}

// タイトルコントロールのキーイベント
void	CfrmUc091KaikakekinI::TerminationTitlediag1( short index, short nChar, short length, LPUNKNOWN data )
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
//	「買掛金」データコントロール処理（EditON，EditOFF，Terminationイベント）
//********************************************************************************

// 「買掛金」データコントロールのゲットフォーカス
void	CfrmUc091KaikakekinI::EditONYoushikidiag1( short index )
{
	DIAGRAM_DATA	diadata;

	// スクロールフラグをクリアする
	nG_MScroll = 0;
	m_ComboSgn = 0;

	// フォーカスフラグON
	m_uInfo.intCurFocus = 1;

	// ポジション取得
	m_uInfo.intCurCtlIndex = m_pDiag->GetPosition();

	// 行・列取得
	CmnGetControlPosition( &m_uInfo.intCurRow , &m_uInfo.intCurCol, &m_uInfo.OldCol );

	switch (m_uInfo.intCurCol) {
		case ID_COL_091I_KNORDER:	// 科目
			// コンボボックスのオフセット値取得
			m_pDiag->GetData( m_uInfo.intCurCtlIndex , (LPUNKNOWN)&diadata );
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
			break;

		case ID_COL_091I_INVONOT:	// 登録番号(T選択)
			// コンボボックスのオフセット値取得
			m_pDiag->GetData(m_uInfo.intCurCtlIndex, (LPUNKNOWN)&diadata);
			m_uInfo.OldCombo = diadata.data_combo;
			break;

		default:
			// 現在の入力値を取得
			m_uInfo.OldstrDiag = m_clsFunc.DiagGetString( m_pDiag, m_uInfo.intCurCtlIndex );
			break;
	}


	// 行移動確認
	if ( ( m_uInfo.OldRow > 0 ) ) {
		if ( m_uInfo.OldPage == m_uInfo.intCurPage && m_uInfo.OldPage > 0 ) {
			// 所在地1(住所)→所在地2(住所)　所在地2(住所)→所在地1(住所)移動は保存しない
			BOOL adadd1_to_adadd2 = IsSaveMoveReference(ID_COL_091I_ADADD1, ID_COL_091I_ADADD2);
			// [Home]キー押下の住所検索ダイアログの戻り（フォーカスをセットしている）は保存しない（改良依頼No15-1613）
			BOOL adadd1_to_adadd1 = IsSaveMoveReference(ID_COL_091I_ADADD1, ID_COL_091I_ADADD1);	// 所在地1(住所)→所在地1(住所)
			BOOL adadd2_to_adadd2 = IsSaveMoveReference(ID_COL_091I_ADADD2, ID_COL_091I_ADADD2);	// 所在地2(住所)→所在地2(住所)　所在地2から住所が長すぎる場合、ここがFALSEになる
			// T選択⇔登録番号は保存しない
			BOOL tsel_to_invno = IsSaveMoveReference(ID_COL_091I_INVONOT, ID_COL_091I_INVONO);

			// データ保存
			//if((adadd1_to_adadd2 != FALSE) && (adadd1_to_adadd1 != FALSE) && (adadd2_to_adadd2 != FALSE)){
			if((adadd1_to_adadd2 != FALSE) && (adadd1_to_adadd1 != FALSE) && (adadd2_to_adadd2 != FALSE) && (tsel_to_invno != FALSE)){
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
		}
	}

	// ボタン操作
	CmnChangeButtonEnable(1);

	// ---- 登録番号一覧関連 ----->
	int		sw=0;
	// 登録番号(T選択)にカーソルがあるとき、登録番号一覧を表示する
	if(m_uInfo.intCurCol == ID_COL_091I_INVONO && m_EndSyorityu == 0)	{
// 修正No.168377 del -->
		//// 登録番号一覧は、空行、データ行、一括金額(手動)の時に表示
		//if(m_ReadData[m_uInfo.intCurRow].m_FgFunc == ID_FGFUNC_NULL || m_ReadData[m_uInfo.intCurRow].m_FgFunc == ID_FGFUNC_DATA ||
		//   m_ReadData[m_uInfo.intCurRow].m_FgFunc == ID_FGFUNC_IKKATUMANUAL) {
// 修正No.168377 del <--
// 修正No.168377 add -->
		// 登録番号のT選択の値を取得
		int pos = CmnGetControlIndex(m_uInfo.intCurRow, ID_COL_091I_INVONOT);
		m_pDiag->GetData(pos, (LPUNKNOWN)&diadata);

		// 登録番号一覧は、「T」が選択されている空行、データ行、一括金額(手動)の時に表示
		if((m_ReadData[m_uInfo.intCurRow].m_FgFunc == ID_FGFUNC_NULL || m_ReadData[m_uInfo.intCurRow].m_FgFunc == ID_FGFUNC_DATA ||
		   m_ReadData[m_uInfo.intCurRow].m_FgFunc == ID_FGFUNC_IKKATUMANUAL) && diadata.data_combo == 0) {
// 修正No.168377 add <--
			// 処理終了中は表示しない（m_EndSyorityu = 1）
			// 進捗確認ダイアログと、処理終了確認ダイアログの間にEditONが発生するため
			sw = 1;
		}
	}
	PostMessage(WM_ZMSEL_MESSAGE, (WPARAM)sw, (LPARAM)index);
	// ---- 登録番号一覧関連 <-----
}

// データコントロールのロストフォーカス
void	CfrmUc091KaikakekinI::EditOFFYoushikidiag1( short index )
{
	if( m_EndView != FALSE )	return;

// 修正No.168443 add -->
	// ---- 登録番号一覧関連 ----->
	if(m_uInfo.intCurCol != ID_COL_091I_INVONO) {
		if(pZmSel && pZmSel->IsDisplay() != FALSE) {
			pZmSel->DialogOFF();
		}
	}
	// ---- 登録番号一覧関連 <-----
// 修正No.168443 add <--

	// 入力確定
	virInputDecisionEditOFF();

	if(m_uInfo.intCurCol == ID_COL_091I_KNORDER)	{
		if(m_ComboSgn != 1)	{
			ComboNew(index);
		}
	}
	m_ComboSgn = 0;

// 修正No.168443 del -->
	//if(m_uInfo.intCurCol == ID_COL_091I_INVONO)		{
	//	// 入力された登録番号から参照ダイアログを検索し、登録済みの場合はデータを取得して画面に表示
	//	if(ChkInvoiceNum(m_pDB, 2, m_uInfo.intCurPage, m_uInfo.intCurRow) == -1) {
	//		//// カーソル移動処理
	//		//CmnDiagSetFocus( m_pDiag, m_uInfo.intCurRow, ID_COL_091I_INVONO );
	//	}
	//}

	//// ---- 登録番号一覧関連 ----->
	//if(pZmSel && m_uInfo.intCurCol != ID_COL_091I_INVONO) {
	//	if(pZmSel->IsDisplay() != FALSE) {
	//		pZmSel->DialogOFF();
	//	}
	//}
	//// ---- 登録番号一覧関連 <-----
// 修正No.168443 del <--
// 修正No.168443 add -->
	if(m_uInfo.intCurCol == ID_COL_091I_INVONO)		{
		// 入力された登録番号をチェック
		ChkInvoiceNum(m_pDB, 2, m_uInfo.intCurPage, m_uInfo.intCurRow);
	}
// 修正No.168443 add <--
}

//	入力確定
void CfrmUc091KaikakekinI::virInputDecisionEditOFF()
{
	// フォーカスフラグOFF
	m_uInfo.intCurFocus = 0;

	//	入力項目を強制的に確定
	CmnDiagSetEnterReDraw( m_pDiag, m_pDiag->GetPosition() );

	// 自動一括集計行なら、変更できてしまうComboBoxの値を元に戻す
// 改良No.21-0086,21-0529 cor -->
	//if (KamokuRowEnableSgn(m_pDB,0) == 0 && m_ReadData[m_uInfo.intCurRow].m_FgFunc == ID_FGFUNC_IKKATUAUTO) {
// ------------------------------
	if(KamokuRowEnableSgn(m_pDB, 0, m_uInfo.intFormSeq) == 0 && m_ReadData[m_uInfo.intCurRow].m_FgFunc == ID_FGFUNC_IKKATUAUTO) {
// 改良No.21-0086,21-0529 cor <--
		switch (m_uInfo.intCurCol) {
		case ID_COL_091I_KNORDER:
			CmnDiagSetCombo( m_pDiag, CmnGetControlIndex(m_uInfo.intCurRow, ID_COL_091I_KNORDER ), m_uInfo.OldCombo);
			break;
		}
	}

	// 行取得
	if ( m_uInfo.intCurCtlIndex < m_uInfo.intT1StartIndex ) {
		m_uInfo.OldRow = CmnGetControlRow();
	}
	else{
		m_uInfo.OldRow = -1;
	}

	// ページ番号取得
	m_uInfo.OldPage = m_uInfo.intCurPage;

	// ボタン操作
	CmnChangeButtonEnable(0);
}

// 預貯金データコントロールのキーイベント
void	CfrmUc091KaikakekinI::TerminationYoushikidiag1( short index, short nChar, short length, LPUNKNOWN data )
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
						case ID_COL_091I_KNORDER:	// 科目
						case ID_COL_091I_INVONOT:	// 登録番号(T選択)
							break;
						default:
							m_pDiag->DataClear( m_uInfo.intCurCtlIndex, TRUE );
							break;
					}
					break;
			}
			break;

		// カーソル移動キー
		case VK_TAB:
		case VK_RETURN:
		case VK_UP:
		case VK_DOWN:
		case VK_LEFT:
		case VK_RIGHT:
			CursorControl(nChar,0);
			break;
	}
}

void CfrmUc091KaikakekinI::VScrollYoushikidiag1(short mode)
{
// 修正No.168430 del -->
	//if(mode == 8 && m_DataKakutei == FALSE && m_Pagefocus == 0 && m_Titlefocus == 0)	{
	//	virInputDecisionEditOFF();
	//	nG_MScroll=1;
	//}
// 修正No.168430 del <--
// 修正No.168430 add -->
	if(m_DataKakutei == FALSE && m_Pagefocus == 0 && m_Titlefocus == 0)	{
		if(m_InvnoErrFlg == 0) {	// 修正No.168480 add
			if(mode == 8) {
// 修正No.168532 add -->
				// ----- 登録番号一覧関連 ----->
				if(pZmSel && pZmSel->IsDisplay() != FALSE) {
					pZmSel->DialogOFF();
				}
				// <---- 登録番号一覧関連 ------
// 修正No.168532 add <--

				virInputDecisionEditOFF();
				nG_MScroll = 1;
			}
// 修正No.168532 del -->
			//else if(mode == 5) {
			//	// ----- 登録番号一覧関連 ----->
			//	if(pZmSel && pZmSel->IsDisplay() != FALSE) {
			//		pZmSel->DialogOFF();
			//	}
			//	// <---- 登録番号一覧関連 ------
			//}
// 修正No.168532 del <--
		}							// 修正No.168480 add
	}
// 修正No.168430 add <--
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
int	CfrmUc091KaikakekinI::GetFocusPosition( int nChar )
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
		// 「種類」と「登録番号(T選択)」はComboBoxのため、「↑，↓」キーではカーソルを移動しない
		//if ( intCol == ID_COL_091I_KNORDER ){		
		if(intCol == ID_COL_091I_KNORDER || intCol == ID_COL_091I_INVONOT)	{
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
	if ( intCol == ID_COL_091I_KNORDER ){
		// カレント列が「科目」の場合
		intVector[0] = m_uInfo.intCurCtlIndex;							// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + m_uInfo.intColMax;		// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - intRowChange;			// ←
	}
	else if ( intCol == ID_COL_091I_ADNAME1 ){
		// カレント列が「名称１」の場合
		intVector[0] = m_uInfo.intCurCtlIndex - m_uInfo.intColMax;		// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + m_uInfo.intColMax;		// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;						// ←
	}
	else if ( intCol == ID_COL_091I_ADADD1 ){
		// カレント列が「住所１」の場合
		intVector[0] = m_uInfo.intCurCtlIndex - m_uInfo.intColMax + 1;	// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + 1;						// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;						// ←
	}
	else if ( intCol == ID_COL_091I_ADADD2 ){
		// カレント列が「住所２」の場合
		intVector[0] = m_uInfo.intCurCtlIndex - 1;						// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + m_uInfo.intColMax - 1;	// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;						// ←
	}
	else if ( intCol == ID_COL_091I_TEKI ){
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
			if(m_ReturnMoveDown) {
				if(intCol == ID_COL_091I_INVONOT)	intNext = intVector[1];			//	カレント列が「登録番号(T表示)」欄の場合「→」キーと同じ
				else								intNext = intVector[2];			//	Enterキーでカーソルを下に移動するチェック時は「↓」キーと同じ
			}
			else									intNext = intVector[1];			// 「Return」キー押下時：「→」キーと同じ
			break;
		case VK_TAB:
			if (CmnCheckShiftKey() == FALSE) {
				intNext = intVector[1];		// 「Tab」キー押下時：「→」キーと同じ
			}
			else {
				// 例外処理：先頭項目で"Shift+Tab"は別コントロールへ移動
				// それ以外は「←」キーと同じ
				if ((intRow <= 1) && (intCol == ID_COL_091I_KNORDER)) {
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
int	CfrmUc091KaikakekinI::GetTotalRowIndex( int nCharOrg, int intIndex, int intNext )
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
		//	データ行を超えた？かつ計行あり？
		if ( nNextRow > nBorder[0] ){
			//	計行に移動
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
				nRet = CmnGetControlIndex( nBorder[0], ID_COL_091I_ADNAME1 );
				break;
			case VK_LEFT:		// ←/Shift+Tab：23行目のデータ行の摘要項目へ移動
				nRet = CmnGetControlIndex( nBorder[0], ID_COL_091I_TEKI );
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
					nRet = CmnGetControlIndex( ( nBorder[2] + 1 ), ID_COL_091I_KNORDER );
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
				nRet = CmnGetControlIndex( ( nBorder[1] + 1 ), ID_COL_091I_KNORDER );
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
void	CfrmUc091KaikakekinI::virTblEditIkkatuLine( int nType, CdbUc000Common* rsData,
									CString strName, CALCKEI_INFO uCalcKei, SORTKAMOKU_INFO uKamoku )
{
	CdbUc091Kaikakekin*	mfcRec = (CdbUc091Kaikakekin*)rsData;

	switch (nType) {
		case	0:
			// ソートの一括集計処理 ･･･ 科目の無い帳表はいらない
			mfcRec->m_AdName1 = strName;				// 名称：名称２
			mfcRec->m_Val = uCalcKei.strKei[0];			// 金額："0:Val"固定
			mfcRec->m_KnSeq = uKamoku.intKnSeq;			// 科目情報
			mfcRec->m_KnOrder = uKamoku.intKnOrder;		// 科目情報
			mfcRec->m_KnName = uKamoku.strKnName;		// 科目情報
			mfcRec->m_KnKana = uKamoku.strKnKana;		// 科目情報（カナ)
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
int	CfrmUc091KaikakekinI::virTblSortSubSortProc( CdlgSort* pCdlgSort, CdbUc000Common* rsData )
{
	CdbUc091Kaikakekin*	rs;
	CString				strFilter;
	CString				strSort;

	// ソート項目の再設定（科目）		→ 帳表により実行有無が異なる
	CdbUcLstKamoku	rsKmk( m_pDB );
	rsKmk.UpdateDataTable(m_uInfo.intFormSeq);
	rsKmk.Fin();

	// ソート項目の再設定（取引先）		→ 帳表により実行有無が異なる
	CdbUcLstAddress	rsAdd( m_pDB );
	rsAdd.UpdateDataTable( m_uInfo.intFormSeq, m_RegAutoRef );
	rsAdd.Fin();

	// ソート条件取得
	CmnTblSortSubGetSortParam(pCdlgSort, &strFilter, &strSort);

	rs = (CdbUc091Kaikakekin*)rsData;

	// 対象フィールドがNULLならNULL文字列にする：文字列連結のソート時は必須
	rs->UpdateDataTableWork();

	// ソート実行
	return rs->RequerySortParameter(strFilter, strSort);
}

/**********************************************************************
	RecBufSetData(CdbUc091Kaikakekin* prmDbRec)
		ローカルのレコードバッファに、1行分のデータを格納する

	引数
		CdbUc091Kaikakekin*	prmDbRec	レコードセット(データがあること）
	戻値
		int		成功/失敗
				FUNCTION_OK
***********************************************************************/
int	CfrmUc091KaikakekinI::RecBufSetData( CdbUc091Kaikakekin* prmDbRec )
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
	m_ReadData[trgRow].m_Val = prmDbRec->m_Val;
	m_ReadData[trgRow].m_Teki = prmDbRec->m_Teki;
	m_ReadData[trgRow].m_KnKana = prmDbRec->m_KnKana;	// 科目カナ
	m_ReadData[trgRow].m_AdKana = prmDbRec->m_AdKana;	// 相手先カナ
	m_ReadData[trgRow].m_InvNo = prmDbRec->m_InvNo;		// 登録番号（法人番号）

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
int	CfrmUc091KaikakekinI::RecBufClearAllData()
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
		P_REC_UC_091_KAIKAKEKIN inRecData レコードへのポインタ
	戻値
		int		成功/失敗
				FUNCTION_OK
***********************************************************************/
int	CfrmUc091KaikakekinI::RecBufClearData( P_REC_UC_091_KAIKAKEKIN inRecData )
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
	inRecData->m_Val = _T("");
	inRecData->m_Teki = _T("");
	inRecData->m_ShowKeiZero = 0;	// ０円計表示フラグ
	inRecData->m_KnKana = _T("");	// 科目
	inRecData->m_AdKana = _T("");	// 相手先
	inRecData->m_InvNo = _T("");	// 登録番号（法人番号）

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
int	CfrmUc091KaikakekinI::virRecBufSaveData( int inPage, int inRow )
{
	REC_UC_091_KAIKAKEKIN	tmpSaveData;	// 画面データ1レコード格納用

	RecBufClearData( &tmpSaveData );	// レコードを初期化

	// 頁計/累計行は登録しない
	if ((m_ReadData[inRow].m_FgFunc == ID_FGFUNC_PAGEKEI) || (m_ReadData[inRow].m_FgFunc == ID_FGFUNC_RUIKEI)) {
		return FUNCTION_OK;
	}

	///////////////////////////////////////////////////////////////
	// コンポーネント(画面上の1レコード分の情報)を仮変数へ格納
	DIAGRAM_DATA	diadata;	// ICSDiag構造体
	int				intIndex;	// 対象行の各インデックス
	int				tsel = 0;
	CString			cst,ino;
	CString			cst2;		// 修正No.168553 add

	// 初期化
	m_clsFunc.DiagInit( &diadata );

	// 科目
	intIndex = CmnGetControlIndex( inRow, ID_COL_091I_KNORDER );
	m_pDiag->GetData( intIndex , (LPUNKNOWN)&diadata );
	// コンボボックスの値をキーにKnOrder（科目順序）を取得
	m_SortMap2.Lookup(diadata.data_combo,tmpSaveData.m_KnOrder);

	// 登録番号（T選択）
	intIndex = CmnGetControlIndex(inRow, ID_COL_091I_INVONOT);
	m_pDiag->GetData(intIndex, (LPUNKNOWN)&diadata);
	tsel = diadata.data_combo;

	// 登録番号（法人番号）
	intIndex = CmnGetControlIndex(inRow, ID_COL_091I_INVONO);
	ino = m_clsFunc.DiagGetString(m_pDiag, intIndex);
	cst.Empty();
	if(ino.IsEmpty() == FALSE)	{
		if(tsel == 0)	cst.Format(_T("T%s"), ino);
		else			cst.Format(_T(" %s"), ino);
// 修正No.168553 add -->
		cst2.Format(_T("T%s"), ino);
		// 並べて表示の際に何度かEditONが発生してしまい、ChkInvoiceNum()を介さないでここにきてしまうことがあるため、
		// ここでも入力された登録番号のチェックを行う。
		if(cst2.IsEmpty() == FALSE)	{
			if(cst2.GetLength() < 14)	{
				cst.Empty();
			}
			else if(pSyzInvoice.CheckInvoiceNumber(cst2) == -1)	{
				cst.Empty();
			}
		}
// 修正No.168553 add <--
	}
	tmpSaveData.m_InvNo = cst;

// 修正No.168534,168539 add -->
	// 登録番号をF9参照で確認する際に必要なので、AdSeqもセット
	if(m_ReadData[inRow].m_AdSeq > 0) {
		tmpSaveData.m_AdSeq = m_ReadData[inRow].m_AdSeq;
	}
// 修正No.168534,168539 add <--

	// 名称1
	CStringArray strArray;
	intIndex = CmnGetControlIndex( inRow, ID_COL_091I_ADNAME1 );
	m_clsFunc.StrDivision(m_clsFunc.DiagGetString(m_pDiag,intIndex),&strArray,2,FALSE,TRUE);
	tmpSaveData.m_AdName1 = strArray.GetAt( 0 );
	tmpSaveData.m_AdName2 = strArray.GetAt( 1 );
	// 2行目が空行なら1行目のみを再表示
	if ( tmpSaveData.m_AdName2.IsEmpty() ){
		if(m_uInfo.intCurCol == ID_COL_091I_ADNAME1 && m_uInfo.intCurRow != m_uInfo.OldRow){	// 各カラムごとにvirRecBufSaveData()がコールされるようになり、かつタイミングがEditOffからEditOnに移ったことによりCmnDiagSetString()でカレットが末尾から先頭に移ってしまう為、制御
			CmnDiagSetString( m_pDiag, intIndex, tmpSaveData.m_AdName1, 0 );
		}
	}

	// 所在地1
	intIndex = CmnGetControlIndex( inRow, ID_COL_091I_ADADD1 );
	tmpSaveData.m_AdAdd1 = m_clsFunc.DiagGetString( m_pDiag, intIndex );

	// 所在地2
	intIndex = CmnGetControlIndex( inRow, ID_COL_091I_ADADD2 );
	tmpSaveData.m_AdAdd2 = m_clsFunc.DiagGetString( m_pDiag, intIndex );

	// 金額
	intIndex = CmnGetControlIndex( inRow, ID_COL_091I_VAL );
	tmpSaveData.m_Val = CmnDiagGetKingaku( m_pDiag, intIndex );

	// 摘要
	intIndex = CmnGetControlIndex( inRow, ID_COL_091I_TEKI );
	tmpSaveData.m_Teki = m_clsFunc.DiagGetString( m_pDiag, intIndex );

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
int	CfrmUc091KaikakekinI::virRecBufSaveDataForPasteInsert( int inPage, int inRow )
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
int	CfrmUc091KaikakekinI::virRecBufSaveDataForPaste( int inPage, int inRow )
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
			REC_UC_091_KAIKAKEKIN
					inSaveData	登録するデータレコードへのポインタ
	戻値
			int					成功(更新あり)/成功(更新なし)/失敗
								FUNCTION_UPDATE	: 成功(更新あり)
								FUNCTION_OK		: 成功(更新なし)
								FUNCTION_NG		: 失敗
***********************************************************************/
int	CfrmUc091KaikakekinI::RecBufSaveDataSub( 
							int inPage,
							int inRow,
							P_REC_UC_091_KAIKAKEKIN inSaveData )
{
	REC_UC_091_KAIKAKEKIN	udTemp;		//	画面上のデータ格納用
	CString					cst;
	CString					tno1, tno2;		// 登録番号比較用

	//	画面上のデータ格納用初期化
	RecBufClearData( &udTemp );

	// 変更があったかのフラグ（変更あり:TRUE、なし:FALSE)
	BOOL	ufgUpData	= FALSE;		// 変更確認フラグ(全体用データ行/空行)
	BOOL	ufgUpToku	= FALSE;		// 変更確認フラグ(全体用特殊行)
	BOOL	ufgKn		= FALSE;		// 変更確認フラグ(科目)
	BOOL	ufgInvNo	= FALSE;		// 変更確認フラグ(登録番号)
	BOOL	ufgAd		= FALSE;		// 変更確認フラグ(取引先)
	BOOL	ufgVal		= FALSE;		// 変更確認フラグ(期末現在高)
	BOOL	ufgTeki		= FALSE;		// 変更確認フラグ(取引の内容)
	BOOL	ufgFunc		= FALSE;		// 変更確認フラグ(未使用？)

	// レコード更新したことを示すフラグ
	BOOL	ufgRsUpdate	= FALSE;

	// テーブルアクセスクラス
	CdbUc091Kaikakekin	mfcRec( m_pDB );

	// 作業用変数へ 登録対象のフィールド値を代入
	udTemp = *inSaveData;
	// 末尾のスペースをカット
	udTemp.m_AdName1 = m_clsFunc.DeleteRightSpace( udTemp.m_AdName1 );	// (相手先名称１)
	udTemp.m_AdName2 = m_clsFunc.DeleteRightSpace( udTemp.m_AdName2 );	// (相手先名称２)
	udTemp.m_AdAdd1 = m_clsFunc.DeleteRightSpace( udTemp.m_AdAdd1 );	// (相手先所在地１)
	udTemp.m_AdAdd2 = m_clsFunc.DeleteRightSpace( udTemp.m_AdAdd2 );	// (相手先所在地２)

	// データ行か、空行かのチェック
	udTemp.m_FgFunc = m_ReadData[inRow].m_FgFunc;

	if ( udTemp.m_FgFunc <= ID_FGFUNC_DATA ) {	// データ行 または、空行なら
		// コンポーネントをチェックして、データがあるかを調べる
		// (仮変数に入れた値でチェックしてもいいのかも？)
		
		if (( udTemp.m_KnOrder == 0 ) &&
			( udTemp.m_InvNo == "" ) &&
			( udTemp.m_AdName1 == "" ) &&
			( udTemp.m_AdName2 == "" ) &&
			( udTemp.m_AdAdd1 == "" ) &&
			( udTemp.m_AdAdd2 == "" ) &&
			( udTemp.m_Val == "" ) &&
			( udTemp.m_Teki == "" )){

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

	// (2)登録番号
	if(udTemp.m_InvNo != m_ReadData[inRow].m_InvNo)	{
		ufgUpData = TRUE;		// 全体(データ行/空行)
		ufgInvNo = TRUE;		// 登録番号用
	}

	//	(3) 名称１
	if (udTemp.m_AdName1 != m_ReadData[inRow].m_AdName1) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgAd		= TRUE;		// 取引先
	}
		
	//	(4) 名称２
	if (udTemp.m_AdName2 != m_ReadData[inRow].m_AdName2) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgUpToku	= TRUE;		// 全体(特殊行)
		ufgAd		= TRUE;		// 取引先
	}
	
	//	(5) 住所１
	if (udTemp.m_AdAdd1 != m_ReadData[inRow].m_AdAdd1) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgAd		= TRUE;		// 取引先
	}

	//	(6) 住所２
	if (udTemp.m_AdAdd2 != m_ReadData[inRow].m_AdAdd2) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgAd		= TRUE;		// 取引先
	}

	//	(7) 期末現在高
	if (udTemp.m_Val != m_ReadData[inRow].m_Val) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgVal		= TRUE;		// 金額用
	}

	//	(8) 摘要
	if (udTemp.m_Teki != m_ReadData[inRow].m_Teki) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgTeki	= TRUE;		// 取引の内容
	}

	//	(9) 特殊行フラグ(基本的には、空行/データ行のチェックになるはず) 
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
			udTemp.m_KnKana = rsKamo.m_KnKana;

			rsKamo.Fin();
		}

		// 取引先名称リストへの登録チェック
// 修正No.168534,168539 add -->
		if((udTemp.m_FgFunc == ID_FGFUNC_DATA) && (m_InvnoErrFlg == 0 && ufgInvNo == TRUE)) {	// 変更ありの場合
			CdbUcLstAddress		rsAdd(m_pDB);

			if(m_RegAutoRef)	udTemp.m_AdSeq = rsAdd.CheckAdSeqEntry_Inv(udTemp.m_AdSeq, udTemp.m_AdName1, udTemp.m_AdName2, udTemp.m_AdAdd1, udTemp.m_AdAdd2, udTemp.m_InvNo, ID_ADDRESSGR_KAI, FALSE, TRUE, 0);
			else				udTemp.m_AdSeq = rsAdd.GetSeqFromAdName_Inv(ID_ADDRESSGR_KAI, udTemp.m_AdName1, udTemp.m_AdName2, udTemp.m_AdAdd1, udTemp.m_AdAdd2, udTemp.m_InvNo);

			if(udTemp.m_AdSeq < 0) {
				rsAdd.Fin();
				return(FUNCTION_NG);
			}

			if(udTemp.m_AdSeq == 0) {
				udTemp.m_AdOrder = 0;
			}
			else {
				udTemp.m_AdOrder = rsAdd.m_OrderNum;
				udTemp.m_AdKana = rsAdd.m_AdKana;
			}

			rsAdd.Fin();
		}
// 修正No.168534,168539 add <--

// 修正No.168534,168539 del -->
		////if ((udTemp.m_FgFunc == ID_FGFUNC_DATA) && (ufgAd == TRUE)) {	// 変更ありの場合
		////if ((udTemp.m_FgFunc == ID_FGFUNC_DATA) && (ufgAd == TRUE || ufgInvNo == TRUE)) {	// 変更ありの場合								// 修正No.168443 del
		//if ((udTemp.m_FgFunc == ID_FGFUNC_DATA) && (ufgAd == TRUE || (m_InvnoErrFlg == 0 && ufgInvNo == TRUE)))		{	// 変更ありの場合	// 修正No.168443 add
// 修正No.168534,168539 del <--
// 修正No.168534,168539 add -->
		if ((udTemp.m_FgFunc == ID_FGFUNC_DATA) && (ufgAd == TRUE))	{	// 変更ありの場合
// 修正No.168534,168539 add <--
			CdbUcLstAddress		rsAdd( m_pDB );

			if(m_RegAutoRef)	udTemp.m_AdSeq = rsAdd.CheckEntry_Inv(udTemp.m_AdName1, udTemp.m_AdName2, udTemp.m_AdAdd1, udTemp.m_AdAdd2, udTemp.m_InvNo, ID_ADDRESSGR_KAI, TRUE);
			else				udTemp.m_AdSeq = rsAdd.GetSeqFromAdName_Inv(ID_ADDRESSGR_KAI, udTemp.m_AdName1, udTemp.m_AdName2, udTemp.m_AdAdd1, udTemp.m_AdAdd2, udTemp.m_InvNo);

			if ( udTemp.m_AdSeq < 0 ){
				rsAdd.Fin();
				return( FUNCTION_NG );
			}
			
			if ( udTemp.m_AdSeq == 0 )		{
				udTemp.m_AdOrder = 0;
			}
			else							{
				udTemp.m_AdOrder = rsAdd.m_OrderNum;
				udTemp.m_AdKana = rsAdd.m_AdKana;
			}

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
					mfcRec.m_KnKana		= udTemp.m_KnKana;
				}
				
				// 登録番号
				if(ufgInvNo == TRUE)	{	// 変更ありの場合
					mfcRec.m_InvNo		= udTemp.m_InvNo;
				}

				//	取引先変更あり？
				//if (ufgAd == TRUE) {
				if (ufgAd == TRUE || ufgInvNo == TRUE) {
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
					mfcRec.m_AdKana		= udTemp.m_AdKana;
				}

				//	期末現在高変更あり？
				if (ufgVal == TRUE) {
					mfcRec.m_Val		= udTemp.m_Val;
				}
				
				//	摘要変更あり？
				if (ufgTeki == TRUE) {
					CStringArray strArray;
					m_clsFunc.StrDivision( udTemp.m_Teki, &strArray, 2, TRUE, TRUE );
					mfcRec.m_Teki		= m_clsFunc.StrDocking( &strArray );
				}
				
				// 特殊行フラグは、強制的に書き込む
				mfcRec.m_FgFunc		= udTemp.m_FgFunc;

				mfcRec.Update();
				ufgRsUpdate = TRUE;		// レコード更新したので ON
			}
		}
	}
	else if ( udTemp.m_FgFunc == ID_FGFUNC_IKKATUAUTO && ufgKn == TRUE ) {
// 改良No.21-0086,21-0529 cor -->
		//if(KamokuRowEnableSgn(m_pDB,0) == 1) {
// ------------------------------
		if(KamokuRowEnableSgn(m_pDB, 0, m_uInfo.intFormSeq) == 1) {
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
				mfcRec.m_Val		= _T("");
				mfcRec.m_Teki		= _T("");
				mfcRec.m_AdKana		= _T("");	// 相手先カナ
				mfcRec.m_KnKana		= _T("");	// 科目カナ
				mfcRec.m_InvNo		= _T("");	// 登録番号
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
//	「買掛金」テーブルのオブジェクトを生成し、ポインタを返す
//********************************************************************************
CdbUc000Common*	CfrmUc091KaikakekinI::virGetDataTableObject()
{
	// 「買掛金」テーブルのオブジェクトを生成し、ポインタを返す
	return (CdbUc000Common*)( new CdbUc091Kaikakekin( m_pDB ) );
}

//********************************************************************************
//	行コピー処理
//	IN	なし
//	RET	なし
//********************************************************************************
void	CfrmUc091KaikakekinI::virRowButtonCopyProc()
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
void	CfrmUc091KaikakekinI::virTblGetKamokuInfo( CdbUc000Common* rsData, SORTKAMOKU_INFO* uKamoku )
{
	CdbUc091Kaikakekin*	rs = (CdbUc091Kaikakekin*)rsData;

	// 本当ならGetFieldValue()で取得する予定だったが、時間も無いのであきらめた
	uKamoku->intKnSeq = rs->m_KnSeq;
	uKamoku->intKnOrder = rs->m_KnOrder;
	uKamoku->strKnName = rs->m_KnName;
	uKamoku->strKnKana = rs->m_KnKana;
}

//********************************************************************************
//	共通処理から帳表のカレント情報を取得
//	IN	CURRENT_INFO	取得する情報の格納領域
//	RET	なし
//********************************************************************************
void	CfrmUc091KaikakekinI::virGetCurrentInfo( CURRENT_INFO* uCurInfo )
{
	uCurInfo->intFgFunc = m_ReadData[m_uInfo.intCurRow].m_FgFunc;
	uCurInfo->intNumGroup = m_ReadData[m_uInfo.intCurRow].m_NumGroup;
	uCurInfo->intCopyFlag = m_nCopyFlag;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	ボタンクリック（欄外登録）
//++++++++++++++++++++++++++++++++++++++++++++++++++
void CfrmUc091KaikakekinI::OnBnClickedMarginalButton1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	CdlgKaikakekin	dlgKai( this );		//	欄外登録ダイアログクラス

	// 実行前の準備処理
	CmnEventStartProc(IDC_MARGINAL_BUTTON1);

	// 欄外登録関連のテーブルがオープン可かチェック
	if ( CheckTableMarginal() == TRUE ){
		//	欄外登録ダイアログ表示
		dlgKai.ShowDialog(m_pDB, m_uInfo, m_DataKakutei); 
	}
}

//**************************************************
//	入力不可モードの設定/解除
//	【引数】	nRow	…	行
//				nMode	…	入力不可モードフラグ
//								DIAG_MDFY_NOINPUT	…	エディットアイテムで全てのWM_CHARを無効とする
//								DIAG_MDFY_INPUT		…	DIAG_MDFY_NOINPUTモード解除
//	【戻値】	なし
//**************************************************
void CfrmUc091KaikakekinI::SetInputMode( int nRow, long nMode )
{
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_091I_KNORDER ), nMode );	//	科目
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_091I_INVONOT ), nMode );	//	登録番号（T選択）
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_091I_INVONO ), nMode );	//	登録番号
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_091I_ADNAME1 ), nMode );	//	名称（上段）
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_091I_ADADD1 ), nMode );	//	所在地（上段）
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_091I_ADADD2 ), nMode );	//	所在地（下段）
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_091I_VAL ), nMode );		//	期末現在高
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_091I_TEKI ), nMode );		//	摘要
}

//**************************************************
//	各Diagの背景色(BackColor)に色を設定
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CfrmUc091KaikakekinI::SetDiagBackColor()
{
	// 入力画面の各Diagの背景色を共通設定色に合わせる
	m_PageDiag.SetBackColor( m_ViewColor );
	m_TitleDiag.SetBackColor( m_ViewColor );
	m_NumberDiag.SetBackColor( m_ViewColor );
	m_HeadDiag.SetBackColor( m_ViewColor );
	m_Diag->SetBackColor( m_ViewColor );
	m_DiagKei->SetBackColor( m_ViewColor );
	m_SaveDataDiag.SetBackColor( m_ViewColor );
}

//********************************************************************************
//	財務連動：連動データの作成（新規作成）
//		IN		CdbUc000Common*		内訳書テーブル情報
//				RENTEMP_INFO*		財務連動情報
//		RET		なし					
//********************************************************************************
void CfrmUc091KaikakekinI::virRendoCreateData(CdbUc000Common* rsData, RENTEMP_INFO* uRenInfo)
{
	CString			strTemp;
	CStringArray	strArry;
	int				intSeq;
	int				sw = 0;
	CdbUcLstAddress	rsLstAdd(m_pDB);	// rs_lst_Address

	CdbUc091Kaikakekin*	rs = (CdbUc091Kaikakekin*)rsData;

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
			rs->m_KnKana	= rsLstKmk.m_KnKana;
		}
	}
	rsLstKmk.Fin();

	// 名称(氏名)
	// 取引先情報：取引先登録チェック
	sw = 0;
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
		m_clsFunc.StrDivisionEx( uRenInfo->strAdName , &strArry , ( MAX_KETA_091_ADNAME1 / 2 ) , 2 );
		if(sw == 1)	{
			intSeq = rsLstAdd.CheckEntryRen_Inv(strArry.GetAt(0), strArry.GetAt(1) , _T("") , _T("") , uRenInfo->strAdKana, uRenInfo->strInvNo, ID_ADDRESSGR_KAI, TRUE);
			if (intSeq > 0) {
				rs->m_AdSeq		= rsLstAdd.m_AdSeq;
				rs->m_AdOrder	= rsLstAdd.m_OrderNum;
				rs->m_AdName1	= rsLstAdd.m_AdName1;
				rs->m_AdName2	= rsLstAdd.m_AdName2;
				rs->m_AdKana	= rsLstAdd.m_AdKana;
				rs->m_InvNo		= rsLstAdd.m_InvNo;
			}
			rsLstAdd.Fin();
		}
		else if(sw == 2)	{
			if(strArry.GetAt(0).IsEmpty() == FALSE || strArry.GetAt(1).IsEmpty() == FALSE)	{
				rs->m_AdSeq		= 0;
				rs->m_AdOrder	= 0;
				rs->m_AdName1	= strArry.GetAt(0);
				rs->m_AdName2	= strArry.GetAt(1);
				rs->m_AdKana	= _T("");
				rs->m_InvNo		= uRenInfo->strInvNo;
			}
		}
	}

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
void CfrmUc091KaikakekinI::virRendoUpdateData(CdbUc000Common* rsData, RENTEMP_INFO* uRenInfo)
{
	CdbUc091Kaikakekin*	rs = (CdbUc091Kaikakekin*)rsData;

	rs->Edit();

	rs->m_RenFgTemp	= ID_RENFGTEMP_UPDATE;		// 財務連動テンポラリ情報
	rs->m_Val		= uRenInfo->strVal;			// 金額

	rs->Update();
}

//**************************************************
//	欄外登録(OnBnClickedMarginalButton1)実行前に必要なテーブルがオープン可かチェックする
//	【引数】	なし
//	【戻値】	TRUE 　… オープン可
//				FALSE  … オープン不可
//**************************************************
BOOL CfrmUc091KaikakekinI::CheckTableMarginal(){

	// ソート項目情報リストテーブルのチェック
	CdbUc092Kaikakekin2 mfcKai( m_pDB );
	if ( m_clsFunc.CheckTableOpen( (CdbBase*)&mfcKai ) != TRUE ){
		return FALSE;
	}

	return TRUE;
}


//***************************************************************************
//（2009.09.08 TS自動ログオフ対応）
//	Viewの終了処理
//		・デストラクタで行っていた処理をここに移動
//
//	IN		なし
//	RET		BOOL				TRUE=エラー，FALSE=正常
//***************************************************************************
BOOL CfrmUc091KaikakekinI::virEndProc(void)
{
	TRACE(_T("***** CfrmUc091KaikakekinI::virEndProc\n"));

	// ↓この処理でコントロールが破棄されているが、当関数が実行される時点では、
	// 　そのコントロールを持つView自体は生きている。
	// 　このため、コントロールに対する処理（セル位置の取得）でエラーになってしまう。
	// 　もともとはデストラクタにあったため、コントロールを破棄した後、
	// 　すぐにView自体も殺していたので問題なかった。
	// 　処理自体も特に意味がないので、コメントアウトする。

	//m_pDiag = NULL;
	//m_pTitleDiag = NULL;

	// ---- 登録番号一覧関連 ----->
	if(pZmSel) {
		if(pZmSel->IsDisplay() != FALSE) {
			pZmSel->DialogOFF();
		}
		//delete pZmSel;
		pZmSel = NULL;
	}
	// ---- 登録番号一覧関連 <-----

	// 親Viewの終了処理
	return CfrmUc000Common::virEndProc();
}

void CfrmUc091KaikakekinI::ComboNew(short index)
{
	short			sv_sel=0;
	short			sv_sel2=0;		// 変更前のコンボボックスの値を取得
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
	if ( m_uInfo.intCurCol == ID_COL_091I_KNORDER ){
		m_pDiag->GetData( m_uInfo.intCurCtlIndex , (LPUNKNOWN)&diadata );
		if(diadata.data_combo > m_CmbCnt)	{
			// 実際に登録された科目の件数と選択されたIndexが異なる場合は、「新規作成」が選択されたとする
			m_F9OnSw = TRUE;
			sv_sel = diadata.data_combo;
			sv_cnt = m_CmbCnt;
			sv_sel2 = GetKnOrder(m_uInfo.intFormSeq,m_uInfo.intCurPage,m_uInfo.intCurRow);

			// F9参照
			OnButtonF9();

			m_F9OnSw = FALSE;
			// 選択されたものを再取得
			m_pDiag->GetData( m_uInfo.intCurCtlIndex , (LPUNKNOWN)&diadata );
			// 何も変わってなかったらキャンセル
			if(sv_sel == diadata.data_combo && sv_cnt == m_CmbCnt)	{
				// sv_sel2が新規追加の場合、クリアする
				if(sv_sel2 > m_CmbCnt)	sv_sel2 = 0;

				diadata.data_combo = sv_sel2;
				m_uInfo.OldCombo = sv_sel2;
				m_pDiag->SetData( m_uInfo.intCurCtlIndex , (LPUNKNOWN)&diadata );
			}
			PostMessage(WM_USER_FOCUSSET,0,0);
		}
	}
}

int CfrmUc091KaikakekinI::CursorControl(short nChar, int sw)
{
	int				rv=0;
	int				intCol=0;
	int				pos=0;
	char			key=0x00;
	DIAGRAM_DATA	diadata;

	// 現在の列番号を取得
	intCol = CmnGetControlCol();

	// 例外処理：移動前に【新規作成】を選び、Tabキー、左矢印キー、右矢印キーを押下された場合は値を元に戻す
	if(nChar == VK_TAB || nChar == VK_LEFT || nChar == VK_RIGHT)	{
		if (intCol == ID_COL_091I_KNORDER) {
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

	key = (char)GetKeyState(VK_SHIFT);				//	Shiftキー状態取得
	// 例外処理：移動前にComnoBoxで←/→キーが押下された場合は値を元に戻す
	if((nChar == VK_TAB && (key & 0x80) == 0x80) || (nChar == VK_LEFT) || (nChar == VK_RIGHT)) {
		// ComboBoxは選択内容を確定させない
		if(intCol == ID_COL_091I_INVONOT)	{	// 登録番号(T選択)
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

void CfrmUc091KaikakekinI::ComboSelYoushikidiag1(short index, LPUNKNOWN data)
{
	char	key1 = 0x00;		// Enterキー状態取得
	char	key2 = 0x00;		// マウス左クリック状態取得
	char	key3 = 0x00;		// 右矢印キー状態取得

	key1 = (char)GetKeyState( VK_RETURN );			// Enterキー状態取得
	key2 = (char)GetAsyncKeyState(VK_LBUTTON);		// マウス左クリック状態取得
	key3 = (char)GetKeyState( VK_RIGHT );			// 右矢印キー状態取得

	m_ComboSgn = 0;
	// 右矢印キーで来た時は抜ける
	if((key3 & 0x80) == 0x80)	{
		return;
	}
	// マウスクリックされた時のみ
	// Enter押下の処理はEditOFFでしているのでここでは入れないようにする
	if( ((key1 & 0x80) != 0x80) && ((key2 & 0xff) == 0x01) )	{
		if(m_uInfo.intCurCol == ID_COL_091I_KNORDER) {
			ComboNew(index);
			m_ComboSgn = 1;
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------
// 全頁の電子申告の桁数チェックを行い、タイトルの文字色を変更する
// ------------------------------------------------------------------------------------------------------------------------
void CfrmUc091KaikakekinI::TitleColorChg(void)
{
	int nCharOver=0;		// 電子申告の文字数を超えたかどうかのサイン		1:電子申告の文字数を超えた
							// 様式⑨		[0]:科目(10文字)
	CRecordset			rs(m_pDB);
	CString				strSQL=_T("");
	CString				strData=_T("");

	// 科目
	strSQL.Format(_T("select max(oversgn) from (select CASE when len(%s) > %d then 1 else 0 end as oversgn from uc_091_kaikakekin where FgShow=%d) tbl1"),
		MakeReplaceSent(_T("KnName")),UC_KAMOKU_LEN,ID_FGSHOW_OFF);
	rs.Open( CRecordset::forwardOnly, strSQL);
	rs.GetFieldValue((short)0, strData);
	rs.Close();
	nCharOver = 0;
	if(_tstoi(strData) != 0) {
		nCharOver = 1;
	}

	// 項目タイトル(ヘッダ部)の再設定
	if(nCharOver == 1)	CmnHeadDiagSetTitle(&m_HeadDiag,0,_T("科  目(10)"),1);
	else				CmnHeadDiagSetTitle(&m_HeadDiag,0,_T("科　目"),0);

	// 欄外ガイドの表示
	if(nCharOver == 1)	m_Guide1.ShowWindow(TRUE);
	else				m_Guide1.ShowWindow(FALSE);
}

LRESULT CfrmUc091KaikakekinI::OnUserReDrawView(WPARAM wParam, LPARAM lParam)
{
	virUpdateControlTblData();
	CmnDiagSetFocus(m_pDiag, 1, 1);

	return(0);
}

// --------------------------------------------------------------------------------------------------------
//	【Home:法人検索】法人検索ダイアログを利用してデータを取得する際に、入力済みのデータをとってくる
// --------------------------------------------------------------------------------------------------------
//		IN		HJN_REC_NEW		法人情報
//		RET		なし
// --------------------------------------------------------------------------------------------------------
void CfrmUc091KaikakekinI::virDiagGetHjnData(CDatabase* pDB, HJN_REC_NEW* phjnRec)
{
	int				nPage,nRow;
	int				intIndex;	// 対象行の各インデックス
	int				tsel=0;
	int				ret=0;
	char			buf[256];
	CString			cst,ino;
	CStringArray	strArray;
	DIAGRAM_DATA	diadata;	// ICSDiag構造体

	nPage = m_uInfo.intCurPage;
	nRow = m_uInfo.intCurRow;

	// ---- 今選択中の行に入力されているデータを取得 ----
	// 登録番号（法人番号）
	cst.Empty();
	intIndex = CmnGetControlIndex(nRow, ID_COL_091I_INVONO);
	cst = m_clsFunc.DiagGetString(m_pDiag, intIndex);
	if(cst.IsEmpty() == FALSE)	{
		memset(buf, 0, sizeof(buf));
		sprintf_s(buf, _T("%s"), cst);
		memcpy(phjnRec->wHjnNo1, buf, 13);
	}

	// 名称1(氏名)
	intIndex = CmnGetControlIndex(nRow, ID_COL_091I_ADNAME1);
	m_clsFunc.StrDivision(m_clsFunc.DiagGetString(m_pDiag, intIndex), &strArray, 2, FALSE, TRUE);
	cst.Empty();
	if(strArray.GetAt(0).IsEmpty() == FALSE && strArray.GetAt(1).IsEmpty() == FALSE) {
		cst.Format(_T("%s%s"), strArray.GetAt(0), strArray.GetAt(1));
	}
	else if(strArray.GetAt(0).IsEmpty() == FALSE) {
		cst.Format(_T("%s"), strArray.GetAt(0));
	}
	else if(strArray.GetAt(1).IsEmpty() == FALSE) {
		cst.Format(_T("%s"), strArray.GetAt(1));
	}
	cst.TrimRight();
	if(cst.IsEmpty() == FALSE) {
		memset(buf, 0, sizeof(buf));
		sprintf_s(buf, _T("%s"), cst);
		memcpy(phjnRec->wHjnName, buf, strlen(buf));
	}

	// 所在地1(住所)
	intIndex = CmnGetControlIndex(nRow, ID_COL_091I_ADADD1);
	cst = m_clsFunc.DiagGetString(m_pDiag, intIndex);
	cst.TrimRight();
	if(cst.IsEmpty() == FALSE)	{
		memset(buf, 0, sizeof(buf));
		sprintf_s(buf, _T("%s"), cst);
		memcpy(phjnRec->wAdrs1, buf, strlen(buf));
	}

	// 所在地2(住所)
	intIndex = CmnGetControlIndex(nRow, ID_COL_091I_ADADD2);
	cst = m_clsFunc.DiagGetString(m_pDiag, intIndex);
	cst.TrimRight();
	if(cst.IsEmpty() == FALSE) {
		memset(buf, 0, sizeof(buf));
		sprintf_s(buf, _T("%s"), cst);
		memcpy(phjnRec->wAdrs2, buf, strlen(buf));
	}

	// カナをuc_lst_adressから取得
	if(m_ReadData[nRow].m_AdSeq != 0) {
		CdbUcLstAddress		rsAdd(pDB);
		ret = rsAdd.RequeryAdInfo(ID_ADDRESSGR_URI, m_ReadData[nRow].m_AdSeq);
		if(ret == 0) {
			// カナ
			memset(buf, 0, sizeof(buf));
			sprintf_s(buf, _T("%s"), rsAdd.m_AdKana);
			memcpy(phjnRec->wHjnKana, buf, strlen(buf));
		}
	}
}

// --------------------------------------------------------------------------------------------------------
//	【Home:法人検索】法人検索ダイアログを利用してデータを取得
//	もしくは、登録番号一覧からデータを取得してきた時の処理
//	F9:参照ダイアログ（uc_lst_adress）に登録しているデータかどうかを検索し、未登録なら取得したデータをセット
//	登録済みの場合は、F9:参照の登録データをセット
// --------------------------------------------------------------------------------------------------------
//		IN		int			pSw		0:法人検索、1:登録番号一覧
//				CString*	rtInvNo	法人番号または登録番号
//				CString*	rtName	名称
//				CString*	rtAdrs1	住所（上段）
//				CString*	rtAdrs2	住所（下段）
//				CString*	rtKana	カナ
//		RET		なし
// --------------------------------------------------------------------------------------------------------
void	CfrmUc091KaikakekinI::virDiagSetHjnData(int pSw, CString rtInvNo, CString rtName, CString rtAdrs1, CString rtAdrs2, CString rtKana)
{
	int					ret = 0;
	int					intIndex;
	CString				strInvoNo, strName, strAdd1, strAdd2, strKana;
	CString				cst, cs1, cs2, cs3;
	CUcFunctionCommon	ufc;

	if(pSw == 0)	strInvoNo.Format(_T(" %s"), rtInvNo);
	else			strInvoNo.Format(_T("T%s"), rtInvNo);

// 修正No.168361,168369,168371,168378 del -->
//※法人番号検索、登録番号一覧から取得してきたものはそのままセットする
//F9:参照に登録されているかのチェックも今回は見ない
	//// F9:参照(取引先＝uc_lst_adress)に登録されているかチェック
	//ret = ChkInvoiceNum(m_pDB, pSw, m_uInfo.intCurPage, m_uInfo.intCurRow, strInvoNo, rtName, rtAdrs1, rtAdrs2);
	//if(ret == -1)	{		//  エラー
	//	//// カーソル移動処理
	//	//CmnDiagSetFocus( m_pDiag, m_uInfo.intCurRow, ID_COL_091I_INVONO );
	//	return;
	//}
	//else if(ret == 1)	{	// 未登録
// 修正No.168361,168369,168371,168378 del <--
		// 登録番号（T選択）
		intIndex = CmnGetControlIndex(m_uInfo.intCurRow, ID_COL_091I_INVONOT);
		CmnDiagSetCombo3(m_pDiag, intIndex, strInvoNo);

		// 登録番号
		InvoNoSplit(strInvoNo, &cs1, &cs2);
		intIndex = CmnGetControlIndex(m_uInfo.intCurRow, ID_COL_091I_INVONO);
		CmnDiagSetString(m_pDiag, intIndex, cs2, 0);

		m_pDiag->Refresh();

		// カーソル再セット
		CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);	// 修正No.168530 add
// 修正No.168361,168369,168371,168378 del -->
	//}
// 修正No.168361,168369,168371,168378 del <--

}

// --------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------
int CfrmUc091KaikakekinI::virCallChkInvoiceNum(int pSw, int inPage, int inRow)
{
	//return(ChkInvoiceNum(m_pDB, 2, inPage, inRow));	// 修正No.168369,168371,168443 del
	//return(ChkInvoiceNum(m_pDB, 3, inPage, inRow));	// 修正No.168369,168371,168443 add
	return(ChkInvoiceNum(m_pDB, pSw, inPage, inRow));	// 修正No.168369,168371,168443 add
}

// --------------------------------------------------------------------------------------------------------
//	入力された登録番号から参照ダイアログを検索し、登録済みの場合はデータを取得して画面に表示
// --------------------------------------------------------------------------------------------------------
//	【引数】	CDatabase*	pDB		データベース操作クラス
//				int			pSw		呼び元　0:法人検索、1:登録番号一覧、2:EditOFF、3:virCallChkInvoiceNum
//				int			inPage	頁番号
//				int			inRow	選択行
//				CString		pInvno	登録番号
// --------------------------------------------------------------------------------------------------------
int CfrmUc091KaikakekinI::ChkInvoiceNum(CDatabase* pDB, int pSw, int inPage, int inRow, 
										  CString pInvno/*=""*/, CString pName/*=""*/, CString pAdrs1/*=""*/, CString pAdrs2/*=""*/)
{
	int					intIndex;		// 対象行の各インデックス
	int					tsel=0;
	int					ret=0;
	CString				invono, invono2;
	CString				cst,cs1,cs2;
	DIAGRAM_DATA		diadata;		// ICSDiag構造体
	CdbUcLstAddress		rsAdd(pDB);
	ZmselDBData			tmpzd;			// 修正No.168361,168369,168371,168378 add

	//if(pSw != 2) {					// 修正No.168369,168371 del
	if(pSw == 0 || pSw == 1)	{		// 修正No.168369,168371 add
		InvoNoSplit(pInvno, &cs1, &cs2);
		invono.Empty();
		invono2.Empty();
		if(cs2.IsEmpty() == FALSE) {
			if(strcmp(cs1, _T("T")) == 0) {
				invono = pInvno;
				invono2 = invono;					// チェックディジット用
			}
			else {
				invono = pInvno;
				invono2.Format(_T("T%s"), cs2);		// チェックディジット用
			}
		}
	}
	else {
		// 登録番号（T選択）
		intIndex = CmnGetControlIndex(inRow, ID_COL_091I_INVONOT);
		m_pDiag->GetData(intIndex, (LPUNKNOWN)&diadata);
		tsel = diadata.data_combo;
		// 登録番号（法人番号）
		intIndex = CmnGetControlIndex(inRow, ID_COL_091I_INVONO);
		cst = m_clsFunc.DiagGetString(m_pDiag, intIndex);
		invono.Empty();
		invono2.Empty();
		if(cst.IsEmpty() == FALSE) {
			if(tsel == 0) {
				invono.Format(_T("T%s"), cst);
				invono2 = invono;					// チェックディジット用
			}
			else {
				invono.Format(_T(" %s"), cst);
				invono2.Format(_T("T%s"), cst);		// チェックディジット用
			}
		}
	}

// 修正No.168361,168369,168371,168378 del -->
	//if(invono.IsEmpty() == FALSE)	{
	//	gErrMsg.Empty();

	//	// 桁数チェック
	//	if(invono2.GetLength() < 14) {
	//		DIAGRAM_DATA	ddata;
	//		ddata.data_combo = 0;
	//		m_pDiag->SetData(CmnGetControlIndex(inRow, ID_COL_091I_INVONOT), (LPUNKNOWN)&ddata);
	//		ddata.data_edit.Empty();
	//		m_pDiag->DataClear(CmnGetControlIndex(inRow, ID_COL_091I_INVONO), TRUE);
	//		// エラーメッセージを表示
	//		gErrMsg  = _T("登録番号（法人番号）の文字数に誤りがあります。\r\n");
	//		gErrMsg += _T("登録番号（法人番号）を再入力してください。");
	//		ICSMessageBox(gErrMsg, MB_ICONSTOP | MB_OK, 0, 0, this);
	//		return(-1);
	//	}

	//	// 指定された登録番号が正しいかどうかチェック
	//	if(pSyzInvoice.CheckInvoiceNumber(invono2) == -1) {
	//		DIAGRAM_DATA	ddata;
	//		ddata.data_combo = 0;
	//		m_pDiag->SetData(CmnGetControlIndex(inRow, ID_COL_091I_INVONOT), (LPUNKNOWN)&ddata);
	//		ddata.data_edit.Empty();
	//		m_pDiag->DataClear(CmnGetControlIndex(inRow, ID_COL_091I_INVONO), TRUE);
	//		gErrMsg  = _T("登録番号（法人番号）に誤りがあります。\r\n誤った番号は保存されません。\r\n\r\n");
	//		gErrMsg += _T("登録番号（法人番号）を再入力してください。");
	//		ICSMessageBox(gErrMsg, MB_ICONSTOP | MB_OK, 0, 0, this);
	//		return(-1);
	//	}
	//	else	{
	//		// ※法人検索の表示や、データの取得等は今回取りやめになったが、今後対応するかもしれないので、コメントは残す（23/11/28）
	//		//if(m_RegAutoRef) {
	//		//	// 自動登録をする場合、uc_lst_address内の修復処理を行う
	//		//	((CUCHIWAKEApp*)AfxGetApp())->AddressConvert(pDB);

	//		//	// 登録番号入力時に、参照ダイアログにとうろくされているかチェックし、登録されていれば名称、所在地を返送する
	//		//	ret = rsAdd.GetAddressInf_Inv(ID_ADDRESSGR_KAI, invono);
	//		//	if(ret == 0) {			//	正常終了（データ取得済み）
	//		//		// 登録番号（T選択）
	//		//		intIndex = CmnGetControlIndex(inRow, ID_COL_091I_INVONOT);
	//		//		CmnDiagSetCombo3(m_pDiag, intIndex, rsAdd.m_InvNo);

	//		//		// 登録番号
	//		//		InvoNoSplit(rsAdd.m_InvNo, &cs1, &cs2);
	//		//		intIndex = CmnGetControlIndex(inRow, ID_COL_091I_INVONO);
	//		//		CmnDiagSetString(m_pDiag, intIndex, cs2, 0);

	//		//		// 名称
	//		//		cst.Format(_T("%s\r\n%s"), rsAdd.m_AdName1,rsAdd.m_AdName2);
	//		//		intIndex = CmnGetControlIndex(inRow, ID_COL_091I_ADNAME1);
	//		//		CmnDiagSetString(m_pDiag, intIndex, cst, 0);

	//		//		// 所在地1(住所)
	//		//		intIndex = CmnGetControlIndex(inRow, ID_COL_091I_ADADD1);
	//		//		CmnDiagSetString(m_pDiag, intIndex, rsAdd.m_AdAdd1, 0);

	//		//		// 所在地2(住所)
	//		//		intIndex = CmnGetControlIndex(inRow, ID_COL_091I_ADADD2);
	//		//		CmnDiagSetString(m_pDiag, intIndex, rsAdd.m_AdAdd2, 0);

	//		//		m_pDiag->Refresh();
	//		//	}
	//		//	rsAdd.Fin();

	//		//	return(ret);
	//		//}
	//		//else {
	//		//	if(m_ReadData[inRow].m_InvNo != invono && (pName.IsEmpty() == FALSE || pAdrs1.IsEmpty() == FALSE || pAdrs2.IsEmpty() == FALSE)) {
	//		//		return(1);
	//		//	}
	//		//}
	//		if(m_ReadData[inRow].m_InvNo != invono) {
	//			return(1);
	//		}
	//	}
	//}
// 修正No.168361,168369,168371,168378 del <--
// 修正No.168361,168369,168371,168378 add -->
	if(cst.GetLength() > 0)	{
		gErrMsg.Empty();

		// EditOFF、virCallChkInvoiceNumから来た時は入力コードのチェックを行う
		if(pSw == 2 || pSw == 3 || pSw == 4) {
// 修正No.168443 add -->
			if(pSw == 2) {
				// データ部のDiagからカーソルが離れた場合、入力されたコードをクリアして戻す
				CWnd *pWnd1 = AfxGetMainWnd();
				if(pWnd1 != NULL)	{
					CWnd *pWnd2 = pWnd1->GetFocus();
					if((pWnd2 != NULL && pWnd2 != m_pDiag) || (pWnd2 == NULL))	{
						if(pSyzInvoice.CheckInvoiceNumber(invono2) == -1) {
							m_pDiag->DataClear(CmnGetControlIndex(inRow, ID_COL_091I_INVONO), TRUE);
							m_InvnoErrFlg = 0;
							return(0);
						}
					}
				}
			}
// 修正No.168443 add <--

			if(pZmSel && pZmSel->IsDisplay() != FALSE && cst.GetLength() < 3) {
				// 2桁の場合は科目選択から
				int no = atoi(cst);
				pZmSel->InputData(1, no, -1, ZSEL_INVNO);
				pZmSel->ResultData(&tmpzd);
				if(tmpzd.errflg != -1) {
					DIAGRAM_DATA	ddata;
					// T選択
					m_clsFunc.DiagInit(&ddata);
					ddata.data_combo = 0;
					m_pDiag->SetData(CmnGetControlIndex(inRow, ID_COL_091I_INVONOT), (LPUNKNOWN)&ddata);
					// 登録番号
					m_clsFunc.DiagInit(&ddata);
					ddata.data_edit.Format(tmpzd.sel_name);
					m_pDiag->SetData(CmnGetControlIndex(inRow, ID_COL_091I_INVONO), (LPUNKNOWN)&ddata);
					// 再描画
					m_pDiag->Refresh();

					m_InvnoErrFlg = 0;		// 修正No.168443 add

					return(1);
				}
			}
			else {
				// ３桁以上は手入力なので桁数チェック
				if(invono2.GetLength() < 14) {
// 修正No.168425,168443 del -->
					//if(pSw == 3) {
					//	m_pDiag->DataClear(CmnGetControlIndex(inRow, ID_COL_091I_INVONO), TRUE);
					//}
					//else {
					//	// エラーメッセージを表示
					//	gErrMsg = _T("登録番号（法人番号）の文字数に誤りがあります。\r\n");
					//	gErrMsg += _T("登録番号（法人番号）を再入力してください。");
					//	ICSMessageBox(gErrMsg, MB_ICONSTOP | MB_OK, 0, 0, this);
					//}
// 修正No.168425,168443 del <--
// 修正No.168425,168443 add -->
					if(pSw == 3 || m_EndSyorityu == 1)	{
						m_pDiag->DataClear(CmnGetControlIndex(inRow, ID_COL_091I_INVONO), TRUE);
						m_InvnoErrFlg = 0;

						return(0);
					}
					else {
						m_InvnoErrFlg = 1;

						m_uInfo.intCurCol = ID_COL_091I_INVONO;

						// エラーメッセージを表示
						gErrMsg = _T("登録番号（法人番号）の文字数に誤りがあります。\r\n");
						gErrMsg += _T("登録番号（法人番号）を再入力してください。");
						ICSMessageBox(gErrMsg, MB_ICONSTOP | MB_OK, 0, 0, this);

						return(-1);
					}
// 修正No.168425,168443 add <--
				}
			}

			// 指定された登録番号が正しいかどうかチェック
			if(pSyzInvoice.CheckInvoiceNumber(invono2) == -1) {
// 修正No.168425,168443 del -->
				//if(pSw == 3) {
				//	m_pDiag->DataClear(CmnGetControlIndex(inRow, ID_COL_091I_INVONO), TRUE);
				//}
				//else {
				//	// エラーメッセージを表示
				//	gErrMsg = _T("登録番号（法人番号）に誤りがあります。\r\n");
				//	gErrMsg += _T("登録番号（法人番号）を再入力してください。");
				//	ICSMessageBox(gErrMsg, MB_ICONSTOP | MB_OK, 0, 0, this);
				//}
// 修正No.168425,168443 del <--
// 修正No.168425,168443 add -->
				if(pSw == 3 || m_EndSyorityu == 1)	{
					m_pDiag->DataClear(CmnGetControlIndex(inRow, ID_COL_091I_INVONO), TRUE);
					m_InvnoErrFlg = 0;

					return(0);
				}
				else {
					m_InvnoErrFlg = 1;

					m_uInfo.intCurCol = ID_COL_091I_INVONO;

					// エラーメッセージを表示
					gErrMsg = _T("登録番号（法人番号）に誤りがあります。\r\n");
					gErrMsg += _T("登録番号（法人番号）を再入力してください。");
					ICSMessageBox(gErrMsg, MB_ICONSTOP | MB_OK, 0, 0, this);

					return(-1);
				}
// 修正No.168425,168443 add <--
			}
		}

		m_InvnoErrFlg = 0;		// 修正No.168443 add

		if(m_ReadData[inRow].m_InvNo != invono) {
			return(1);
		}
	}
// 修正No.168361,168369,168371,168378 add <--

	m_InvnoErrFlg = 0;		// 修正No.168443 add

	return(0);
}
// インボイス登録番号追加対応_23/09/11 add <--
