
// メイン郡
#include "stdafx.h"
#include "UCHIWAKE.h"
#include "frmUc011Yotyokin.h"

IMPLEMENT_DYNCREATE(CfrmUc011YotyokinK, CfrmUc000Common)


BEGIN_MESSAGE_MAP(CfrmUc011YotyokinK, CfrmUc000Common)
	ON_WM_SIZE()
//	ON_COMMAND(ID_BUTTON_F1, OnButtonF1)
	ON_COMMAND(ID_BUTTON_F2, OnButtonF2)
//	ON_COMMAND(ID_BUTTON_F3, OnButtonF3)
	ON_COMMAND(ID_BUTTON_F4, OnButtonF4)
	ON_COMMAND(ID_BUTTON_F5, OnButtonF5)
	ON_COMMAND(ID_BUTTON_F6, OnButtonF6)
	ON_COMMAND(ID_BUTTON_F7, OnButtonF7)
	ON_COMMAND(ID_BUTTON_F8, OnButtonF8)
	ON_COMMAND(ID_BUTTON_F9, OnButtonF9)
	ON_COMMAND(ID_BUTTON_F11, OnButtonF11)
	ON_COMMAND(ID_BUTTON_F12, OnButtonF12)
	ON_COMMAND(ID_BUTTON_HOME, OnButtonHome)
	ON_COMMAND( ID_BUTTON_INSERT, OnButtonInsert )
	//ON_COMMAND(ID_BUTTON_END, OnButtonEnd)
	ON_COMMAND(ID_PRINTSETUP_MENU, &CfrmUc011YotyokinK::OnMenuPrintSetup)
	ON_COMMAND(ID_PRINTALL_MENU, &CfrmUc011YotyokinK::OnMenuPrintAll)
//	ON_COMMAND(ID_OUTPUTFORM_MENU, &CfrmUc011YotyokinK::OnMenuFormTypeSetup)		// ①預貯金は実行不可
//	ON_COMMAND(ID_DISPDIALOG_MENU, &CfrmUc011YotyokinK::OnMenuIkkatuKingaku)		// ①預貯金は実行不可
	ON_COMMAND( ID_SPECIALROW_MENU, &CfrmUc011YotyokinK::OnMenuSpecialRow )
	ON_COMMAND( ID_SORT_MENU, &CfrmUc011YotyokinK::OnMenuSort )
	ON_COMMAND(ID_ZEROMONEY_MENU, &CfrmUc011YotyokinK::OnMenuZeroMoney)
	ON_COMMAND(ID_ZEROMONEYMOVE_MENU, &CfrmUc011YotyokinK::OnMenuZeroMoneyMove)
	ON_COMMAND(ID_AMOUTDIALOG_MENU, &CfrmUc011YotyokinK::OnMenuOutKeiSetup)
// midori 160606 add -->
	ON_COMMAND(ID_CLEAR_MENU, &CfrmUc011YotyokinK::OnMenuClear)
// midori 160606 add <--
	ON_COMMAND( ID_RENDO_MENU, &CfrmUc011YotyokinK::OnMenuZaimuRendo )
	ON_COMMAND( ID_TITLEMENU, &CfrmUc011YotyokinK::OnMenuTitleRestore )
	ON_COMMAND( ID_USER_SETTING_MENU, &CfrmUc011YotyokinK::OnMenuOptionSetting )
	ON_COMMAND( ID_COPY_SETTING_INFO_MENU, &CfrmUc011YotyokinK::OnMenuCopySettingInfo )
	ON_COMMAND( ID_DELETE_DATA_MENU, &CfrmUc011YotyokinK::OnMenuDeleteDataEachYoushiki )
	ON_BN_CLICKED(IDC_PAGEBACK_BUTTON1, &CfrmUc011YotyokinK::OnPageBack)
	ON_BN_CLICKED(IDC_PAGENEXT_BUTTON1, &CfrmUc011YotyokinK::OnPageNext)
	ON_BN_CLICKED(IDC_PAGENEW_BUTTON1, &CfrmUc011YotyokinK::OnPageNew)
	ON_BN_CLICKED(IDC_PAGEINSERT_BUTTON1, &CfrmUc011YotyokinK::OnPageInsert)
	ON_BN_CLICKED(IDC_ROWCOPY_BUTTON1, &CfrmUc011YotyokinK::OnRowCopy)
	ON_BN_CLICKED(IDC_ROWPASTE_BUTTON1, &CfrmUc011YotyokinK::OnRowPaste)
	ON_BN_CLICKED(IDC_ROWINSERT_BUTTON1, &CfrmUc011YotyokinK::OnRowInsert)
	ON_BN_CLICKED( IDC_YOUSHIKINEXT_BUTTON1, &CfrmUc011YotyokinK::OnMoveYoushikiNext )
	ON_BN_CLICKED( IDC_YOUSHIKIBACK_BUTTON1, &CfrmUc011YotyokinK::OnMoveYoushikiBack )	
//	ON_UPDATE_COMMAND_UI(ID_BUTTON_F1, &CfrmUc011YotyokinK::OnUpdateButtonF1)
//	ON_UPDATE_COMMAND_UI(ID_BUTTON_F3, &CfrmUc011YotyokinK::OnUpdateButtonF3)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F6, &CfrmUc011YotyokinK::OnUpdateButtonF6)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F7, &CfrmUc011YotyokinK::OnUpdateButtonF7)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F8, &CfrmUc011YotyokinK::OnUpdateButtonF8)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F9, &CfrmUc011YotyokinK::OnUpdateButtonF9)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F11, &CfrmUc011YotyokinK::OnUpdateButtonF11)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F12, &CfrmUc011YotyokinK::OnUpdateButtonF12)
	ON_UPDATE_COMMAND_UI( ID_BUTTON_HOME, &CfrmUc011YotyokinK::OnUpdateButtonHome )
//	ON_UPDATE_COMMAND_UI(IDC_ROWDELETE_BUTTON1, &CfrmUc011YotyokinK::OnUpdateButtOnMoveYoushikiBack)
//	ON_UPDATE_COMMAND_UI(IDC_ROWNULLINSERT_BUTTON1, &CfrmUc011YotyokinK::OnUpdateButtonNullInsert)
//	ON_UPDATE_COMMAND_UI(ID_DISPDIALOG_MENU, &CfrmUc011YotyokinK::OnUpdateMenuIkkatuKingaku)
// midori 160612 add -->
	ON_UPDATE_COMMAND_UI( ID_SORT_MENU, &CfrmUc011YotyokinK::OnUpdateMenuSort )
	ON_UPDATE_COMMAND_UI( ID_CLEAR_MENU, &CfrmUc011YotyokinK::OnUpdateMenuClear )
// midori 160612 add <--
	ON_UPDATE_COMMAND_UI(ID_ZEROMONEY_MENU, &CfrmUc011YotyokinK::OnUpdateMenuZeroMoney)
// midori 160612 add -->
	ON_UPDATE_COMMAND_UI( ID_ZEROMONEYMOVE_MENU, &CfrmUc011YotyokinK::OnUpdateMenuZeroMoneyMove)
// midori 160612 add <--
	ON_UPDATE_COMMAND_UI( ID_RENDO_MENU, &CfrmUc011YotyokinK::OnUpdateMenuZaimuRendo )
	ON_UPDATE_COMMAND_UI(ID_SPECIALROW_MENU, &CfrmUc011YotyokinK::OnUpdateMenuSpecialRow)
// midori 160612 add -->
	ON_UPDATE_COMMAND_UI( ID_TITLEMENU, &CfrmUc011YotyokinK::OnUpdateMenuTitleRestore )
	ON_UPDATE_COMMAND_UI( ID_DELETE_DATA_MENU, &CfrmUc011YotyokinK::OnUpdateMenuDeleteDataEachYoushiki )
// midori 160612 add <--
	ON_MESSAGE(WM_USER_REDRAWVIEW, OnUserReDrawView)		// 修正No.162446 add

END_MESSAGE_MAP()


CfrmUc011YotyokinK::CfrmUc011YotyokinK()
	: CfrmUc000Common(CfrmUc011YotyokinK::IDD)
{
	m_pDB = ((CUCHIWAKEApp*)AfxGetApp())->m_pDB;
	m_pTitleDiag = &m_TitleDiag;
// midori 152718 add -->
	m_pPageDiag	= &m_PageDiag;
// midori 152718 add <--
}

CfrmUc011YotyokinK::~CfrmUc011YotyokinK()
{
	// 2009.09.08 TS自動ログオフ対応(virEndProcへ移動)
	TRACE(_T("***** CfrmUc011YotyokinK::~CfrmUc011YotyokinK\n"));

	//m_pDiag = NULL;
	//m_pTitleDiag = NULL;
}

void CfrmUc011YotyokinK::DoDataExchange(CDataExchange* pDX)
{
	CfrmUc000Common::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_YOUSHIKIDIAG1, m_Diag1);
	DDX_Control(pDX, IDC_YOUSHIKIDIAG2, m_Diag2);
	DDX_Control(pDX, IDC_YOUSHIKIDIAG_KEI1, m_DiagKei1);
	DDX_Control(pDX, IDC_YOUSHIKIDIAG_KEI2, m_DiagKei2);
	DDX_Control(pDX, IDC_TITLEDIAG1, m_TitleDiag);
	DDX_Control(pDX, IDC_PAGEDIAG1, m_PageDiag);
	DDX_Control(pDX, IDC_NUMBER_DIAG1, m_NumberDiag);
	DDX_Control(pDX, IDC_TOPICDIAG1, m_HeadDiag);	
// midori 161108 add -->
	DDX_Control(pDX, IDC_SAVEDATA_DIAG1, m_SaveDataDiag);
// midori 161108 add <--
// 修正No.157626 add -->
	DDX_Control(pDX, IDC_ICSDISPGUIDE1, m_Guide1);
// 修正No.157626 add <--
}

void CfrmUc011YotyokinK::OnInitialUpdate()
{
	CfrmUc000Common::OnInitialUpdate();
	ResizeParentToFit();

	// 帳表テーブルのオープンOK?
	if(m_InitialFlag == TRUE)	{
		// コンボボックスアイテムのリサイズ制御
		m_Diag1.ComboxFontMode(TRUE);
		m_Diag2.ComboxFontMode(TRUE);

		// リサイズ
		//SetResize( IDC_YOUSHIKIDIAG1 , ICS_RESIZE_BOTTOM );
		//SetResize( IDC_YOUSHIKIDIAG2 , ICS_RESIZE_BOTTOM );
		//SetResize( IDC_YOUSHIKIDIAG_KEI1 , ICS_RESIZE_BOTTOM );
		//SetResize( IDC_YOUSHIKIDIAG_KEI2 , ICS_RESIZE_BOTTOM );

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
		}
		else	{
			// カーソルフラグ=FALSE(この後フォーカスセット処理が走らないように)
			m_CursorSetFlag = FALSE;
		}
	}
}

#ifdef _DEBUG
void CfrmUc011YotyokinK::AssertValid() const
{
	CfrmUc000Common::AssertValid();
}

#ifndef _WIN32_WCE
void CfrmUc011YotyokinK::Dump(CDumpContext& dc) const
{
	CfrmUc000Common::Dump(dc);
}
#endif
#endif //_DEBUG

BEGIN_EVENTSINK_MAP(CfrmUc011YotyokinK, CfrmUc000Common)
	ON_EVENT(CfrmUc011YotyokinK, IDC_YOUSHIKIDIAG1, 3, CfrmUc011YotyokinK::TerminationYoushikidiag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CfrmUc011YotyokinK, IDC_YOUSHIKIDIAG1, 2, CfrmUc011YotyokinK::EditOFFYoushikidiag1, VTS_I2)
	ON_EVENT(CfrmUc011YotyokinK, IDC_YOUSHIKIDIAG1, 1, CfrmUc011YotyokinK::EditONYoushikidiag1, VTS_I2)
// midori 151405 add -->
	ON_EVENT(CfrmUc011YotyokinK, IDC_YOUSHIKIDIAG1, 12, CfrmUc011YotyokinK::VScrollYoushikidiag1, VTS_I2)
// midori 151405 add <--
// midori 152134 add -->
	ON_EVENT(CfrmUc011YotyokinK, IDC_YOUSHIKIDIAG1, 8, CfrmUc011YotyokinK::ComboSelYoushikidiag1, VTS_I2 VTS_UNKNOWN)
// midori 152134 add <--

	ON_EVENT(CfrmUc011YotyokinK, IDC_YOUSHIKIDIAG2, 3, CfrmUc011YotyokinK::TerminationYoushikidiag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CfrmUc011YotyokinK, IDC_YOUSHIKIDIAG2, 2, CfrmUc011YotyokinK::EditOFFYoushikidiag1, VTS_I2)
	ON_EVENT(CfrmUc011YotyokinK, IDC_YOUSHIKIDIAG2, 1, CfrmUc011YotyokinK::EditONYoushikidiag1, VTS_I2)
// midori 151405 add -->
	ON_EVENT(CfrmUc011YotyokinK, IDC_YOUSHIKIDIAG2, 12, CfrmUc011YotyokinK::VScrollYoushikidiag1, VTS_I2)
// midori 151405 add <--
// midori 152134 add -->
	ON_EVENT(CfrmUc011YotyokinK, IDC_YOUSHIKIDIAG2, 8, CfrmUc011YotyokinK::ComboSelYoushikidiag1, VTS_I2 VTS_UNKNOWN)
// midori 152134 add <--

	ON_EVENT(CfrmUc011YotyokinK, IDC_TITLEDIAG1, 3, CfrmUc011YotyokinK::TerminationTitlediag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CfrmUc011YotyokinK, IDC_TITLEDIAG1, 2, CfrmUc011YotyokinK::EditOFFTitlediag1, VTS_I2)
	ON_EVENT(CfrmUc011YotyokinK, IDC_TITLEDIAG1, 1, CfrmUc011YotyokinK::EditONTitlediag1, VTS_I2)
	ON_EVENT(CfrmUc011YotyokinK, IDC_TITLEDIAG1, 6, CfrmUc011YotyokinK::FocusTitlediag1, VTS_NONE)

	ON_EVENT(CfrmUc011YotyokinK, IDC_PAGEDIAG1, 3, CfrmUc011YotyokinK::TerminationPagediag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CfrmUc011YotyokinK, IDC_PAGEDIAG1, 2, CfrmUc011YotyokinK::EditOFFPagediag1, VTS_I2)
	ON_EVENT(CfrmUc011YotyokinK, IDC_PAGEDIAG1, 1, CfrmUc011YotyokinK::EditONPagediag1, VTS_I2)
	ON_EVENT(CfrmUc011YotyokinK, IDC_PAGEDIAG1, 6, CfrmUc011YotyokinK::FocusPagediag1, VTS_NONE)
END_EVENTSINK_MAP()

//********************************************************************************
//	預貯金等の内訳書：初期処理
//********************************************************************************
BOOL CfrmUc011YotyokinK::virStartJob()
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

	// 様式送りボタンの制御
	UpdateYoushikiMoveButton( m_YoushikiSkip );

	return TRUE;
}

//********************************************************************************
//	共通データの初期値設定
//********************************************************************************
void CfrmUc011YotyokinK::virSetInitialUchiwakeInfo()
{
	// 預貯金の固有データ
	m_uInfo.intOutKeiDialog = AM_TYPE_ALL;			// 計設定ダイアログのボタン指定
	m_uInfo.intRowMax = ID_ROWKOBETU_011;			// １頁内の合計行を含む全体の行数
	m_uInfo.intRowMaxData = ID_ROWKOBETU_011 - 1;	// １頁内のデータ行数（合計行は含まず）
	m_uInfo.intColMax = ID_COLMAX_011K;				// １行内のコントロール個数	（預貯金=13）
	m_uInfo.intColMaxData = ID_COLMAXDATA_011;		// １行内の列数				（預貯金=6）
	m_uInfo.intCtlStartIndex = ID_CTLSTARTINDEX_011;// １行目の入力項目の先頭インデックス番号
	m_uInfo.intT1StartIndex = ID_T1STARTNORMAL_011K;	// 合計行①の先頭インデックス番号
	m_uInfo.intT2StartIndex = ID_T2STARTNORMAL_011K;	// 合計行②の入力項目の先頭インデックス番号
	m_uInfo.intCursolDefPos = ID_COL_011_KINYU;		// カーソル移動時の初期位置（タイトルから↓，合計から↑で使用）
	m_uInfo.intCursolKeiPos = ID_COL_011_KINYU;		// 合計行のカーソル移動位置（カーソル移動で使用）

	m_uInfo.intFormSeq = ID_FORMNO_011;				// 様式シーケンス番号
	m_uInfo.intFormNo = 1;							// 様式番号　　（uc_inf_sub）
	m_uInfo.intFormNo2 = 1;							// 様式番号枝番（uc_inf_sub）

	m_uInfo.intRowNormal = ID_ROWNORMAL_011;		// 通常時：１頁内の合計行を含む全体の行数
	m_uInfo.intRowKobetu = ID_ROWKOBETU_011;		// 個別時：１頁内の合計行を含む全体の行数
	m_uInfo.intT1StartNormal = ID_T1STARTNORMAL_011K;// 通常時：合計行①の先頭インデックス
	m_uInfo.intT2StartNormal = ID_T2STARTNORMAL_011K;// 通常時：合計行②の先頭インデックス
	m_uInfo.intT1StartKobetu = ID_T1STARTKOBETU_011K;// 個別時：合計行①の先頭インデックス
	m_uInfo.intT2StartKobetu = ID_T2STARTKOBETU_011K;// 個別時：合計行②の先頭インデックス
	m_uInfo.intRowKei = ID_ROWKEI_011;				// 帳表固定の合計行数（1:合計あり，0:なし）
	m_uInfo.intKei1StartIndex = ID_COL_011_KEI1_MEISYO;	// 計ダイアグラム1行目の先頭インデックス
	m_uInfo.intKei2StartIndex = ID_COL_011K_KEI2_MEISYO;	// 計ダイアグラム2行目の先頭インデックス

	// 集計処理の設定
	m_uInfo.uCalcInfo.intMaxCount = ID_CALC_011_MAX;		// 集計フィールド件数
	m_uInfo.uCalcInfo.strField[0] = ID_CALC_011_FIELD1;		// 集計フィールド名称１

/************************************************
	// Test：コントロールをComboBox→EditBoxへ変更
	// １行目ComboBoxの属性をバックアップ
	int	intIndex = CmnGetControlIndex(1, ID_COL_011_SYURUI);
	m_pDiag->GetAttribute(intIndex, (LPUNKNOWN)&m_uInfo.uComboAttr);
************************************************/
}

//**************************************************
// ICSDiagコントロールを通常／個別で切り替える
// 【引数】    なし
// 【戻値】    なし
//**************************************************
void	CfrmUc011YotyokinK::virSetInitialDiag()
{
	int     nIndex;

	// オブジェクト未生成時は処理を抜ける
	if ( (m_Diag1.m_hWnd == NULL) || (m_Diag2.m_hWnd == NULL) ){
		return;
	}

	//表示するダイアログの設定
	m_Diag		= CmnGetDiagType( &m_Diag1, &m_Diag2 );
	m_DiagKei	= CmnGetDiagType( &m_DiagKei1, &m_DiagKei2 );
	if( (m_Diag == NULL) || (m_DiagKei == NULL) ){
		return;
	}

	// 計ダイアログの表示位置の設定（明細ダイアログにリサイズがかかっているため）
	//SetKeiDialogPosition( m_Diag, m_DiagKei );

	// 親クラスのダイアログにコピー
	m_pDiag		= m_Diag;
	m_pDiagKei	= m_DiagKei;

	// 入力不可行でのDeleteキー無効
	m_pDiag->EnableDelete(0);

	// コンボボックスの属性取得
	nIndex = CmnGetControlIndex( 1, ID_COL_011_SYURUI );
	m_pDiag->GetAttribute( nIndex, ( LPUNKNOWN )&m_uInfo.uComboAttr );
}

//********************************************************************************
//	帳表固有変数の初期値設定
//********************************************************************************
void CfrmUc011YotyokinK::virSetInitialValue()
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
BOOL CfrmUc011YotyokinK::InitialControlDataArea()
{
	int			intMax;
	int			i;

	// 全データクリア
	m_pDiag->AllClear();
	m_pDiagKei->AllClear();

	// 各Diagに背景色を設定
	SetDiagBackColor();

	// 背景色クリア
	CmnDiagSetAttrColorAllClear(m_pDiag);

	// 列の属性により更にクリアと初期設定が必要
	intMax = m_uInfo.intT1StartIndex;
	for ( i = m_uInfo.intCtlStartIndex; i < intMax; i += m_uInfo.intColMax ) {
		// ComboBox：クリア
/************************************************
		// Test：コントロールをComboBox→EditBoxへ変更
		CmnDiagSetAttrComboToEdit( m_pDiag, i + ID_COL_011_SYURUI - 1, TRUE);
************************************************/
		CmnDiagSetString( m_pDiag, i + ID_COL_011_SYURUI - 1, "", 0 );
		
		// 漢字入力欄の初期化
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_011_KINYU    - 1, MAX_KETA_011_KINYU );		// 金融機関名
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_011_KINYUSUB - 1, MAX_KETA_011_KINYUSUB );	// 金融機関支店名
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_011_KOUZA    - 1, MAX_KETA_011_KOUZA );		// 口座番号
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_011_TEKIYOU  - 1, MAX_KETA_011_TEKIYOU );		// 摘要

// No.200903 del -->
		//// 金額欄：桁数初期化（11桁）
		//CmnDiagSetAttrDigit( m_pDiag, i + ID_COL_011_KIMATU - 1, ID_VAL_011_DATA );
// No.200903 del <--
// No.200903 add -->
		// 金額欄：桁数初期化（13桁）
		CmnDiagSetAttrDigit( m_pDiag, i + ID_COL_011_KIMATU - 1, ID_VAL_011_K_DATA );
// No.200903 add <--

		//	入力不可モードの解除
		SetInputMode( CmnGetControlRow( i ), DIAG_MDFY_INPUT );
	}

	// ComboBoxの初期設定
	if ( InitialControlComboBox() != TRUE ){
		return FALSE;
	}

	// 半角/全角設定
	CmnDiagSetAttrColZenHan( m_pDiag , ID_COL_011_KINYU );			// 金融機関名
	CmnDiagSetAttrColZenHan( m_pDiag , ID_COL_011_KINYUSUB );		// 支店名
	CmnDiagSetAttrColZenHan( m_pDiag , ID_COL_011_TEKIYOU );		// 摘要

	return TRUE;
}

//********************************************************************************
//	帳表コントロールの初期化（ComboBox）
//********************************************************************************
BOOL CfrmUc011YotyokinK::InitialControlComboBox()
{
	DIAGRAM_DATA	diadata;			// ICSDIAG構造体
	int				intTotalType;
	int				intMax;
	int				intIndex;
	int				i;
	int				nFontSize = 100;
	BOOL			bFlg = FALSE;	// フォントサイズ変更する？

	// カレント頁の合計行の種別を取得
	intTotalType = CmnGetControlTotalType();
	
	// カレント頁で"頁計と累計"を表示する場合、対象行は"-1"となる
	switch (intTotalType) {
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
	for (i = 1; i <= intMax; i++) {
		// 対象コントロールのインデックス番号を取得
		intIndex = CmnGetControlIndex(i, ID_COL_011_SYURUI);

		// 初期設定（ドロップダウンON＋リスト情報をセット）
		m_pDiag->ModifyItem(intIndex, DIAG_MDFY_NODROPDOWN);
		m_pDiag->SetData(intIndex, (LPUNKNOWN)&diadata);

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
void CfrmUc011YotyokinK::InitialControlTotalArea()
{
	int				int23DRow;			// 23行目データ行の入力欄属性
	int				int23TRow;			// 23行目合計行①の入力欄属性
	//int				int24TRow;			// 24行目合計行②の入力欄属性
	int				intKeiRow;			// 計専用ダイアグラムの入力欄属性
	int				intTotalType;		// カレント頁の合計行の種別
	int				intStart;
	int				i;
// midori 152166 add -->
	int				intIndex=0;
	int				nSw=0;

	intIndex = CmnGetControlIndex(m_uInfo.intCurRow,m_uInfo.intCurCol);
// midori 152166 add <--

	// カレント頁の合計行の種別を取得
	intTotalType = CmnGetControlTotalType();

	// 23＋24行目の行全体に対して、"編集可能/不可"＋"表示/非表示"の設定を行う
	switch (intTotalType) {
	case ID_OUTKEI_OFF:				// なし
	case ID_OUTKEI_PAGEKEI:			// 頁計
	case ID_OUTKEI_RUIKEI:			// 累計
// midori 160612 cor -->
//		int23DRow = (DIAG_MDFY_EDIT		| DIAG_MDFY_OPAQUE);
// ---------------------
// midori 152364 del -->
		//if(m_DataKakutei == FALSE)	int23DRow = (DIAG_MDFY_EDIT | DIAG_MDFY_OPAQUE);
// midori 152364 del <--
// midori 152364 add -->
		if(m_DataKakutei == FALSE)	{
			if(IsSpecialRow(m_ReadData[23].m_FgFunc) != 0)	{
				int23DRow = (DIAG_MDFY_NOINPUT | DIAG_MDFY_OPAQUE);
			}
			else	{
				int23DRow = (DIAG_MDFY_EDIT | DIAG_MDFY_OPAQUE);
			}
		}
// midori 152364 add <--
		else						int23DRow = (DIAG_MDFY_READONLY | DIAG_MDFY_OPAQUE);
// midori 160612 cor <--
		int23TRow = (DIAG_MDFY_READONLY | DIAG_MDFY_TRANSPARENT);

		// 23行目の合計行①：Index=299～311
		intStart = m_uInfo.intT1StartIndex;
		for (i = 0;  i < m_uInfo.intColMax; i++) {
			m_pDiag->ModifyItem(intStart + i, int23TRow);
		}

		break;
	case ID_OUTKEI_BOTH:			// 頁計と累計
		int23DRow = (DIAG_MDFY_READONLY | DIAG_MDFY_TRANSPARENT);
		int23TRow = (DIAG_MDFY_READONLY | DIAG_MDFY_OPAQUE);

		break;
	}

	// 23行目のデータ行：Index=286～298
	intStart = (23 - 1) * m_uInfo.intColMax;
	for (i = 0;  i < m_uInfo.intColMax; i++) {
		m_pDiag->ModifyItem(intStart + i, int23DRow);
// midori 152166 add -->
		if((intIndex == (intStart + i)) && ((int23DRow & DIAG_MDFY_READONLY) != 0))	nSw=1;
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
	intKeiRow = (DIAG_MDFY_READONLY | DIAG_MDFY_OPAQUE);
	m_pDiagKei->ModifyItem(ID_COL_011K_KEI1_KIMATU, intKeiRow);
	m_pDiagKei->ModifyItem(ID_COL_011K_KEI2_KIMATU, intKeiRow);


	// 合計行の名称欄は、カーソルを飛ばしたいので入力許可する
	switch (intTotalType) {
	case ID_OUTKEI_OFF:
	case ID_OUTKEI_PAGEKEI:
	case ID_OUTKEI_RUIKEI:
		int23TRow = (DIAG_MDFY_READONLY | DIAG_MDFY_TRANSPARENT	| DIAG_MDFY_NOINPUT );
		break;
	case ID_OUTKEI_BOTH:
		int23TRow = (DIAG_MDFY_EDIT		| DIAG_MDFY_OPAQUE		| DIAG_MDFY_NOINPUT);
		break;
	}
	// 入力可能。ただし"入力文字数=0"にて操作不可とする
	m_pDiag->ModifyItem(CmnGetControlTotalIndex(1, ID_COL_011_KINYU), int23TRow);

// No.200903 del -->
	//// 合計行の金額欄は、12桁固定とする
	//CmnDiagSetAttrDigit(m_pDiagKei, ID_COL_011K_KEI1_KIMATU, ID_VAL_011_TOTAL);
	//CmnDiagSetAttrDigit(m_pDiagKei, ID_COL_011K_KEI2_KIMATU, ID_VAL_011_TOTAL);
// No.200903 del <--
// No.200903 add -->
	// 合計行の金額欄は、13桁固定とする
	CmnDiagSetAttrDigit(m_pDiagKei, ID_COL_011K_KEI1_KIMATU, ID_VAL_011_K_TOTAL);
	CmnDiagSetAttrDigit(m_pDiagKei, ID_COL_011K_KEI2_KIMATU, ID_VAL_011_K_TOTAL);
// No.200903 add <--
}

//********************************************************************************
//	帳表コントロールの特殊行初期化
//		IN		int				行番号
//				int				特殊行フラグ（FgFunc）
//		RET		なし
//********************************************************************************
void CfrmUc011YotyokinK::InitialControlSpecialRow(int intRow, int intFgFunc)
{
	DIAGRAM_DATA	diadata;			// ICSDIAG構造体

	// 対象行の色を変更
	CmnDiagSetAttrSpecialRow(m_pDiag, intRow, intFgFunc);

	// 特殊行の金額欄は桁数変更 ･･･ 頁計/累計行は初期設定するのでここでは対象外
	switch (intFgFunc) {
	case ID_FGFUNC_SYOKEI:				// 小計
	case ID_FGFUNC_SYOKEINULL:			// 小計（空行）
	case ID_FGFUNC_CHUKEI:				// 中計
	case ID_FGFUNC_CHUKEINULL:			// 中計（空行）
	case ID_FGFUNC_IKKATUMANUAL:		// 一括集計金額行（手動）
	case ID_FGFUNC_IKKATUAUTO:			// 一括集計金額行（自動）
// No.200903 del -->
		//// 12桁を設定
		//CmnDiagSetAttrDigit(m_pDiag, CmnGetControlIndex(intRow , ID_COL_011_KIMATU), ID_VAL_011_TOTAL);
// No.200903 del <--
// No.200903 add -->
		// 13桁を設定
		CmnDiagSetAttrDigit(m_pDiag, CmnGetControlIndex(intRow , ID_COL_011_KIMATU), ID_VAL_011_K_TOTAL);
// No.200903 add <--
		break;
	}

	// 特殊行は入力不可とする
	switch (intFgFunc) {
	case ID_FGFUNC_SYOKEI:				// 小計
	case ID_FGFUNC_SYOKEINULL:			// 小計（空行）
	case ID_FGFUNC_CHUKEI:				// 中計
	case ID_FGFUNC_CHUKEINULL:			// 中計（空行）
	case ID_FGFUNC_IKKATUAUTO:			// 一括集計金額行（自動）
		
		if (intFgFunc != ID_FGFUNC_IKKATUAUTO) {
			// 種類はComboBoxのため、DropDown不可とする
			m_pDiag->ModifyItem(CmnGetControlIndex(intRow, ID_COL_011_SYURUI), DIAG_MDFY_NODROPDOWN);
			
			// ComboBoxの選択項目クリア
			m_clsFunc.DiagInit(&diadata);
			diadata.text_combo = _T("\r\n");
			m_pDiag->SetData(CmnGetControlIndex(intRow, ID_COL_011_SYURUI), (LPUNKNOWN)&diadata);
		}
		
		//	入力不可モードの設定
// midori 160612 cor -->
//		SetInputMode( intRow, DIAG_MDFY_NOINPUT );
// ---------------------
		if(m_DataKakutei == FALSE)	SetInputMode(intRow,DIAG_MDFY_NOINPUT);
		else						SetInputMode(intRow,DIAG_MDFY_READONLY);
// midori 160612 cor <--

	/************************************************
		// Test：コントロールをComboBox→EditBoxへ変更
		CmnDiagSetAttrComboToEdit(m_pDiag, CmnGetControlIndex(intRow, ID_COL_011_SYURUI), FALSE);
	************************************************/
		break;
	}

//	if (intFgFunc == ID_FGFUNC_IKKATUAUTO) {
//		int intTemp = (DIAG_MDFY_READONLY);
//		m_pDiag->ModifyItem(CmnGetControlIndex(intRow, ID_COL_011_SYURUI), intTemp);
//	}
}


//********************************************************************************
//	帳表タイトル情報の表示
//********************************************************************************
void CfrmUc011YotyokinK::virUpdateControlTitle()
{
//	int				intMax;
	DIAGRAM_ATTRIBUTE diaatt;

	// 帳表ナンバーの背景色設定
	CmnDiagSetAttrColor(&m_NumberDiag, 0, &m_ViewColor );

	// 帳表ナンバー表示
	CmnDiagSetString(&m_NumberDiag, 0, m_uInfo.strTitleNumber, 1);

	// 帳表タイトル表示
	CmnDiagSetString(&m_TitleDiag, 0, m_uInfo.strTitleName, 0);

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

// midori 161108 add -->
	// 保管ﾃﾞｰﾀ件数の背景色設定
	CmnDiagSetAttrColor(&m_SaveDataDiag, 0, &m_ViewColor);
// midori 161108 add <--

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
void CfrmUc011YotyokinK::UpdateControlPage()
{
	BOOL			bBack = TRUE;
	BOOL			bNext = TRUE;

	// 最大頁取得
	m_uInfo.intMaxPage = CmnTblGetPageMax();

	// カレント頁番号のチェック
	if (m_uInfo.intCurPage > m_uInfo.intMaxPage) {
		m_uInfo.intCurPage = m_uInfo.intMaxPage;
	}

	// カレント頁/最大頁表示
	CmnDiagSetValue(&m_PageDiag, 1, m_uInfo.intCurPage, 0);
	CmnDiagSetValue(&m_PageDiag, 3, m_uInfo.intMaxPage, 1);

	// 前頁/次頁ボタンの制御
	CmnCheckEnableMoveButton(&bBack, &bNext);
// midori 155879 del -->
//	GetDlgItem(IDC_PAGEBACK_BUTTON1)->EnableWindow(bBack);		// 前頁ボタン
//	GetDlgItem(IDC_PAGENEXT_BUTTON1)->EnableWindow(bNext);		// 次頁ボタン
// midori 155879 del <--
// midori 155879 add -->
	PostMessage(WM_USER_CONTROLPAGE, bBack, bNext);
// midori 155879 add <--
}

//********************************************************************************
//	帳表データの表示（１ページ分）
//********************************************************************************
BOOL CfrmUc011YotyokinK::virUpdateControlTblData()
{
	// ダイアログ切替
	if( m_pDiag != NULL ){
		virSetInitialDiag();
	}

	CdbUc011Yotyokin	Yotyokin( m_pDB );
	
// midori 154237,154238 del -->
//// midori 154190 add -->
//	if(((CUCHIWAKEApp*)AfxGetApp())->m_YsChg != 0)	{;
//		if(m_pDiag->GetPosition() != -1)	{
//			CmnDiagSetFocus(m_pDiag,1,1);
//		}
//	}
//// midori 154190 add <--
// midori 154237,154238 del <--

	m_pDiag->DeleteInput();					//	入力ボックス削除 

	// 頁情報の表示
	UpdateControlPage();

	// コントロールのクリア＋初期設定
	if ( InitialControlDataArea() != TRUE ){	// データ行
		return FALSE;
	}
// midori 160612 del -->
//	InitialControlTotalArea();				// 頁計/累計行
//	m_pDiag->Refresh();						// 更新
// midori 160612 del <--

	//----------------------- １行データ表示 ------------------------------//

	// 倍長設定
	l_defn(0x16);

	// 指定ページ番号のデータを取得
	Yotyokin.Init( m_uInfo.intCurPage );

	// 1ページ分のローカルバッファをクリア
	RecBufClearAllData();

	// 1ページ分のデータ1レコードづつ取得し、画面に表示
	while( !Yotyokin.IsEOF()){		// ファイル終端まで
		// 1ページ分のローカルバッファにデータを格納
		RecBufSetData( &Yotyokin );

		// 対象行金額欄の桁数を変更
		InitialControlSpecialRow( Yotyokin.m_NumRow, Yotyokin.m_FgFunc );

		// 累計/頁計の場合、特殊処理
		if ( ( Yotyokin.m_FgFunc == ID_FGFUNC_RUIKEI ) || ( Yotyokin.m_FgFunc == ID_FGFUNC_PAGEKEI ) ){
			// 累計/頁計の表示
			UpdateControlRowTotal( &Yotyokin );
		}
		// 累計・頁計・空行以外
		else if ( Yotyokin.m_FgFunc != ID_FGFUNC_NULL ){
			// データ行の表示（空行，特殊行を含む）
			UpdateControlRowData( &Yotyokin );
		}

// No.200601 add -->
		// 電子申告の文字数を超えていれば、項目の文字色を変える
		// 金融機関名
		CmnInitialControlFgStringCell( m_pDiag, Yotyokin.m_NumRow, ID_COL_011_KINYU, Yotyokin.m_FgFunc, UC_BANK_LEN, Yotyokin.m_BkName1 );
		// 支店名
		CmnInitialControlFgStringCell( m_pDiag, Yotyokin.m_NumRow, ID_COL_011_KINYUSUB, Yotyokin.m_FgFunc, UC_BANK_LEN, Yotyokin.m_BkName2 );
		// 種類
		CmnInitialControlFgStringCell( m_pDiag, Yotyokin.m_NumRow, ID_COL_011_SYURUI, Yotyokin.m_FgFunc, UC_KAMOKU_LEN, Yotyokin.m_KnName );
		// 摘要
		CmnInitialControlFgStringCell( m_pDiag, Yotyokin.m_NumRow, ID_COL_011_TEKIYOU, Yotyokin.m_FgFunc, UC_011_TEKI_LEN,Yotyokin.m_Teki );
// No.200601 add <--

		Yotyokin.MoveNext();
	}

	// 閉じる
	Yotyokin.Fin();

// midori 160612 add -->
	// InitialControlSpecialRow 関数内で、入力確定の判定を行っているので終わってから下記の関数を行う
	// ※先に行うと23行目に斜線が表示される
	InitialControlTotalArea();				// 頁計/累計行
// midori 160612 add <--

// midori 161108 add -->
	// 保管ﾃﾞｰﾀ件数の表示
	virUpdateControlTblData2();
// midori 161108 add <--

// midori 190301 add -->
	// プロパティで設定した行毎の入力桁数が SetAttribuute 関数によってクリアされているので
	// すべての設定が終わったここで、設定し直す
	CmnDiagSetAttrEditFormat(m_pDiag,ID_COL_011_KINYUSUB,MAX_KETA_011_KINYUSUB,2,_T("5,5"));	// 支店名
	CmnDiagSetAttrEditFormat(m_pDiag,ID_COL_011_TEKIYOU,MAX_KETA_011_TEKIYOU,2,_T("20,20"));	// 摘要
// midori 190301 add <--

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
BOOL CfrmUc011YotyokinK::virUpdateControlTblData2()
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
//		IN		CdbUc011Yotyokin*	預貯金テーブル情報
//		RET		なし
//********************************************************************************
void CfrmUc011YotyokinK::UpdateControlRowData( CdbUc011Yotyokin* Yotyokin ){

	DIAGRAM_DATA		diadata;		// ICSDiag構造体
	int					intCell;		// 保存フィールド
	
	// 初期化
	m_clsFunc.DiagInit( &diadata );
	// 読込み対象セル取得
	intCell = CmnGetControlIndex( Yotyokin->m_NumRow, ID_COL_011_KINYU );
	
// midori 190301 del -->
	//// 金融機関名
	//diadata.data_edit = Yotyokin->m_BkName1;		// 金融機関名
	////diag_setdata( IDC_YOUSHIKIDIAG1,intCell ,&diadata, CICSDiag);
	//m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	//// 支店名
	//if ((Yotyokin->m_FgFunc == ID_FGFUNC_SYOKEI) || (Yotyokin->m_FgFunc == ID_FGFUNC_CHUKEI)) {
	//	diadata.data_edit = Yotyokin->m_KeiStr;			// 特殊行の名称
	//}
	//else {
	//	diadata.data_edit = Yotyokin->m_BkName2;
	//}
	////diag_setdata( IDC_YOUSHIKIDIAG1,intCell + 1 ,&diadata, CICSDiag);
	//m_pDiag->SetData( intCell + 1, ( LPUNKNOWN )&diadata );
// midori 190301 del <--
// midori 190301 add -->
	// 金融機関名
	if((Yotyokin->m_FgFunc == ID_FGFUNC_SYOKEI) || (Yotyokin->m_FgFunc == ID_FGFUNC_CHUKEI))	{
		diadata.data_edit = Yotyokin->m_KeiStr;			// 特殊行の名称
	}
	else	{
		diadata.data_edit = Yotyokin->m_BkName1;		// 金融機関名
	}
	m_pDiag->SetData(intCell,(LPUNKNOWN)&diadata);
	// 支店名
	diadata.data_edit = Yotyokin->m_BkName2;
	m_pDiag->SetData(intCell + 1,(LPUNKNOWN)&diadata);
// midori 190301 add <--

	// 種類名
// midori 152137 del -->
//	diadata.data_combo = (short)Yotyokin->m_KnOrder;
// midori 152137 del <--
// midori 152137 add -->
	// KnOrder（科目順序）をキーにコンボボックスのインデックスを取得
	m_SortMap1.Lookup(Yotyokin->m_KnOrder,diadata.data_combo);
// midori 152137 add <--
/************************************************
	// Test：コントロールをComboBox→EditBoxへ変更
	if (Yotyokin->m_FgFunc <= ID_FGFUNC_DATA) {
		diadata.data_combo = (short)Yotyokin->m_KnOrder;
	} else {
		diadata.data_edit = Yotyokin->m_KeiStr;			// 特殊行の名称
	}
************************************************/
	//diag_setdata( IDC_YOUSHIKIDIAG1,intCell + 2 ,&diadata, CICSDiag);
	m_pDiag->SetData( intCell + 2, ( LPUNKNOWN )&diadata );
	// 口座番号
	diadata.data_edit = Yotyokin->m_AcNum;
	//diag_setdata( IDC_YOUSHIKIDIAG1,intCell + 3 ,&diadata, CICSDiag);
	m_pDiag->SetData( intCell + 3, ( LPUNKNOWN )&diadata );
	// 金額
	if ( Yotyokin->m_Val != "" ){
		CmnChangeFieldValData( &diadata.data_val[0] , Yotyokin->m_Val );
		//diag_setdata( IDC_YOUSHIKIDIAG1,intCell + 4 ,&diadata, CICSDiag);
		m_pDiag->SetData( intCell + 4, ( LPUNKNOWN )&diadata );
	}
	else{
		m_pDiag->DataClear( intCell + 4 , TRUE );
	}
	// 摘要
	diadata.data_edit = Yotyokin->m_Teki;
	//diag_setdata( IDC_YOUSHIKIDIAG1,intCell + 5 ,&diadata, CICSDiag);
	m_pDiag->SetData( intCell + 5, ( LPUNKNOWN )&diadata );
}

//********************************************************************************
//	帳表データの１行表示（累計，頁計行）
//		IN		CdbUc011Yotyokin*	預貯金テーブル情報
//		RET		なし
//********************************************************************************
void CfrmUc011YotyokinK::UpdateControlRowTotal( CdbUc011Yotyokin* Yotyokin ){

	int				intTotalType;
	int				intRowName = 0;			// 名称表示行（0:非表示）
	int				intRowKingaku = 0;		// 金額表示行（0:非表示）

	// カレント頁の合計行の種別を取得
	intTotalType = CmnGetControlTotalType();

	switch (intTotalType) {
	case ID_OUTKEI_OFF:
		// カレント頁："なし"なら、合計行②に名称のみ表示
		if ((Yotyokin->m_FgFunc == ID_FGFUNC_PAGEKEI) || (Yotyokin->m_FgFunc == ID_FGFUNC_RUIKEI)) {
			intRowName = 2;
		}
		break;
	case ID_OUTKEI_PAGEKEI:
		// カレント頁："頁計"なら、合計行②に"頁計"を表示
		if (Yotyokin->m_FgFunc == ID_FGFUNC_PAGEKEI) {
			intRowName = 2;
			intRowKingaku = 2;
		}
		break;
	case ID_OUTKEI_RUIKEI:
		// カレント頁："累計"なら、合計行②に"累計"を表示
		if (Yotyokin->m_FgFunc == ID_FGFUNC_RUIKEI) {
			intRowName = 2;
			intRowKingaku = 2;
		}
		break;
	case ID_OUTKEI_BOTH:
		// カレント頁："頁計＋累計"なら、合計行①に"頁計"＋合計行②に"累計"を表示
		if (Yotyokin->m_FgFunc == ID_FGFUNC_PAGEKEI) {
			intRowName = 1;
			intRowKingaku = 1;
		}
		else if (Yotyokin->m_FgFunc == ID_FGFUNC_RUIKEI) {
			intRowName = 2;
			intRowKingaku = 2;
		}
		break;
	}

	// 頁計/累計の名称表示
	if (intRowName > 0) {
		CmnDiagSetString( m_pDiagKei, CmnGetKeiDiagIndex( intRowName, ID_COL_011_KEI1_MEISYO ), Yotyokin->m_KeiStr, 1 );
	}
	// 頁計/累計の金額表示
	if (intRowKingaku > 0) {
		CmnDiagSetKingaku( m_pDiagKei, CmnGetKeiDiagIndex( intRowKingaku, ID_COL_011K_KEI1_KIMATU ), Yotyokin->m_Val );
	}
}


//********************************************************************************
//	【F6:項目複写】１つ前のデータ表示処理
//		IN		CdbUc000Common*	共通テーブルクラス
//		RET		なし
//********************************************************************************
void CfrmUc011YotyokinK::virFncCellCopyProc( CdbUc000Common* rsData )
{
	CdbUc011Yotyokin*	rs;
	int					intCol;		// 現在の列

	rs = (CdbUc011Yotyokin*)rsData;

	// 列取得
	intCol = CmnGetControlCol();

	// 列によって処理を分岐
	switch( intCol ){

	// 金融機関
	case ID_COL_011_KINYU:
		CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_BkName1, 0 );
		break;
	// 金融機関支店名
	case ID_COL_011_KINYUSUB:
		CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_BkName2, 0 );
		break;
	// 種類
	case ID_COL_011_SYURUI:
// midori 152137 del -->
//		CmnDiagSetCombo( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_KnOrder );
// midori 152137 del <--
// midori 152137 add -->
		CmnDiagSetCombo2( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_KnOrder );
// midori 152137 add <--
		break;
	// 口座番号
	case ID_COL_011_KOUZA:
		CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_AcNum, 0 );
		break;
	// 金額
	case ID_COL_011_KIMATU:
		CmnDiagSetKingaku( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_Val );
		break;
	// 摘要
	case ID_COL_011_TEKIYOU:
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
//									ID_ADDRESSGR_NONE：	なし
//									ID_ADDRESSGR_URI：	売掛金
//									ID_ADDRESSGR_KAI：	買掛金
//				long*			現在選択項目のシーケンス番号　（更新項目）･･･ NDS参照ダイアログで使用
//				int*			住所１の分割するバイト数 ･･･ ICS様住所検索で使用
//				int*			住所２の分割するバイト数 ･･･ ICS様住所検索で使用
//				int*			住所１の列番号
//				int*			住所２の列番号
//		RET		int				参照型の種別
//									ID_DLGTYPE_BANK：	金融機関
//									ID_DLGTYPE_ADDRESS：取引先
//									ID_DLGTYPE_KAMOKU：	科目
//									ID_DLGTYPE_NONE：	参照不可
//********************************************************************************
int CfrmUc011YotyokinK::virFncReferenceGetType(int intCol, int* intGrSeq, long* lngItemSeq, int* intAdd1, int* intAdd2, int* intAdd1Col, int* intAdd2Col, REFERENCE_INFO* pudReference)
{
	int				intRefType;

	switch (intCol) {

	// 金融機関・支店名
	case ID_COL_011_KINYU:
	case ID_COL_011_KINYUSUB:
		intRefType = ID_DLGTYPE_BANK;
		*intGrSeq = ID_ADDRESSGR_NONE;
		*lngItemSeq = m_ReadData[m_uInfo.intCurRow].m_BkSeq;
		break;

	// 種類
	case ID_COL_011_SYURUI:
		intRefType = ID_DLGTYPE_KAMOKU;
		*intGrSeq = ID_ADDRESSGR_NONE;
		*lngItemSeq = m_ReadData[m_uInfo.intCurRow].m_KnSeq;
		break;

/****** //---- Debug Start ----
	case ID_COL_011_TEKIYOU:
		intRefType = ID_DLGTYPE_ICSADDSEARCH;
		*intAdd1 = 20;
		*intAdd2 = 20;
		break;
******/ //---- Debug End ----

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
void CfrmUc011YotyokinK::virFncReferenceSetData(CdlgReference* dlgRef, int intRefType, int intCol)
{
// midori 190301 add -->
	CString				cs=_T("");
	CString				cs1=_T("");
	CString				cs2=_T("");
	CString				cs3=_T("");
	CUcFunctionCommon	ufc;
// midori 190301 add <--

	switch (intRefType) {

	// 金融機関・支店名
	case ID_DLGTYPE_BANK:
		if (intCol == ID_COL_011_KINYU) {
			// 選択内容を表示
			CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, dlgRef->m_udBank.m_BkName1, 0 );
// midori 190301 del -->
			//CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex + 1, dlgRef->m_udBank.m_BkName2, 0 );
// midori 190301 del <--
// midori 190301 add -->
			// 全角５文字を超えていれば改行マークを挿入する
			cs = dlgRef->m_udBank.m_BkName2;
			if(cs.GetLength() > 10)	{
				cs2 = ufc.GetSpritString(cs,&cs1,10);
				cs = cs2;
				cs3 = ufc.GetSpritString(cs,&cs2,10);
				cs = cs1 + PRT_FORMAT_CHCODE + cs2;
			}
			CmnDiagSetString(m_pDiag,m_uInfo.intCurCtlIndex + 1,cs,0);
// midori 190301 add <--
		}
		else if (intCol == ID_COL_011_KINYUSUB) {
			// 選択内容を表示
// midori 190301 del -->
			//CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, dlgRef->m_udBank.m_BkName2, 0 );
// midori 190301 del <--
// midori 190301 add -->
			// 全角５文字を超えていれば改行マークを挿入する
			cs = dlgRef->m_udBank.m_BkName2;
			if(cs.GetLength() > 10)	{
				cs2 = ufc.GetSpritString(cs,&cs1,10);
				cs = cs2;
				cs3 = ufc.GetSpritString(cs,&cs2,10);
				cs = cs1 + PRT_FORMAT_CHCODE + cs2;
			}
			CmnDiagSetString(m_pDiag,m_uInfo.intCurCtlIndex,cs,0);
// midori 190301 add <--
			CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex - 1, dlgRef->m_udBank.m_BkName1, 0 );
		}
		break;

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

	default:
		return;
		break;
	}
	m_pDiag->Refresh();
}

/****** //---- Debug Start ----
void CfrmUc011YotyokinK::virFncReferenceSetICSData(CdlgAddressSearch* dlgAddd)
{
	CString		strTemp = dlgAddd->m_AdAdd1 + _T("\r\n") + dlgAddd->m_AdAdd2;
	CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, strTemp, 0 );
}
******/ //---- Debug End ----

//********************************************************************************
//	【F9:参照】参照ダイアログの操作によりテーブルを更新
//		IN		int				参照ダイアログの種別
//		RET		なし
//********************************************************************************
void CfrmUc011YotyokinK::virFncReferenceUpdateTbl(int intRefType, int nRefMode/*=0*/)
{
	switch (intRefType) {
	// 金融機関名
	case ID_DLGTYPE_BANK:
		{
			// "uc_011_Yotyokin"の更新 ･･･ 本当は全帳表分の更新が必要
			CdbUcLstBank	rsBank( m_pDB );
//			rsBank.UpdateDataTable(TBL_NAME_011);
			rsBank.UpdateDataTableAll(m_RegAutoRef);
			rsBank.Fin();
// midori 190301 add -->
			//// CdbUcLstBank から挿入した支店名に、改行マークが入っていないので
			//// ここで挿入する
			//DataConver();
// midori 190301 add <--
			break;
		}
	// 科目
	case ID_DLGTYPE_KAMOKU:
		{
			// "uc_011_Yotyokin"の更新
			CdbUcLstKamoku	rsKmk( m_pDB );
//@			rsKmk.UpdateDataTable(TBL_NAME_011, m_uInfo.intFormSeq);
			rsKmk.UpdateDataTable(m_uInfo.intFormSeq, nRefMode);
			rsKmk.Fin();
			break;
		}
	}	
}


// ウィンドゥ拡大・縮小時のフロート画面サイズ設定
void CfrmUc011YotyokinK::OnSize(UINT nType, int cx, int cy)
{
	CfrmUc000Common::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
}


// ウィンドウ消去
BOOL CfrmUc011YotyokinK::DestroyWindow() 
{
	return CFormView::DestroyWindow();

}


/*BOOL CfrmUc011YotyokinK::OnInitDialog() 
{
//	CFormView::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください


	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}*/

//********************************************************************************
//	その他イベント処理
//********************************************************************************
BOOL CfrmUc011YotyokinK::PreTranslateMessage(MSG* pMsg)
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

	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	if ( pMsg->message == WM_KEYDOWN ) {
		switch ( pMsg->wParam ) {
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
				// 特殊行でのEnterキーは、次項目へ移動
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
//				// ComboBoxではPageUp，PageDownキーを実行しない
//				if (CmnGetControlCol() != ID_COL_011_SYURUI) {
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
					intMoveRow = m_uInfo.intRowMaxData;		// 合計行①の場合(23)
					intMoveCol = m_uInfo.intCursolDefPos;	// 移動先がデータ行になるため、Combo以外へ移動したい
				}

// midori 152134 17/10/07 add -->
				intCol = CmnGetControlCol();
				// 例外処理：移動前に【新規作成】が選ばれている場合は値を元に戻す
				if (intCol == ID_COL_011_SYURUI) {
					m_pDiag->GetData( m_uInfo.intCurCtlIndex , (LPUNKNOWN)&diadata );
					if(diadata.data_combo > m_CmbCnt)	{
						m_pDiag->ComboDropDown(m_uInfo.intCurCtlIndex, FALSE);
						CmnDiagSetCombo(m_pDiag, m_uInfo.intCurCtlIndex, m_uInfo.OldCombo);
					}
				}
// midori 152134 17/10/07 add <--

				// Scroll＋フォーカス移動先を取得
				if (pMsg->wParam == VK_PRIOR) {
					// PageUpの場合：Scroll＋フォーカス移動（行－11）
					m_pDiag->ScrollPos(SB_PAGEUP , 0);
					intMoveRow = intMoveRow - 11;
					if (intMoveRow < 1) {
						blnMove = FALSE;					// 移動しない
					}
				}
				else {
					// PageDownの場合：Scroll＋フォーカス移動（行＋11）
					m_pDiag->ScrollPos(SB_PAGEDOWN, 0);
					intMoveRow = intMoveRow + 11;
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
// midori 152134 17/10/07 del -->
//// midori 160611 cor -->
//				//}
//// ---------------------
//				if(CmnGetControlCol() == ID_COL_011_SYURUI)	{
//					rv=1;
//				}
//// midori 160611 cor <--
// midori 152134 17/10/07 del <--
// midori 152134 17/10/07 add -->
				if(intCol == ID_COL_011_SYURUI)	{
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
				CmnDiagSetFocus(m_pDiag, m_uInfo.intRowMax, ID_COL_011_KINYU);
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
				rv = CursorControl((short)pMsg->wParam, 1);
			}
// 修正No.158259 add -->
			else if(m_DataKakutei == FALSE && m_Pagefocus == 0 && m_Titlefocus == 0)	{
				intCol = CmnGetControlCol();
				if(intCol == ID_COL_011_SYURUI && (pMsg->wParam == VK_LEFT || pMsg->wParam == VK_RIGHT))	{
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
void CfrmUc011YotyokinK::FocusPagediag1() 
{
//マウスクリックでキャレットが表示できないためコメントとする
//	// ボタンからTabキー移動した場合に必要
//	m_PageDiag.SetPosition(1);
}

// 頁コントロールのゲットフォーカス
void CfrmUc011YotyokinK::EditONPagediag1(short index)
{
// midori 151405 add -->
	// スクロールフラグをクリアする
	nG_MScroll = 0;
	m_Pagefocus = 1;
// midori 151405 add <--

	// 1行登録
//	if( CmnGetControlCol() != ID_COL_011_KIMATU ){
		if (virRecBufSaveData(m_uInfo.intCurPage, m_uInfo.intCurRow) == FUNCTION_UPDATE) {
			m_PageDiag.SetPosition(1);
		}
//	}
}

// 頁コントロールのロストフォーカス
void CfrmUc011YotyokinK::EditOFFPagediag1(short index)
{
// midori 155028 add -->
	// 会社切替時にメッセージを表示した場合、EditOffが走りDB書き込み時にエラーが発生していた
	if(m_EndView != FALSE)	return;
// midori 155028 add <--

	// 頁入力値を取得
	int	intTempPage = CmnDiagGetValue(&m_PageDiag, 1);
	
// midori 151405 add -->
	m_Pagefocus = 0;
// midori 151405 add <--
	// 頁が変更された場合は画面更新が必要
	if (m_uInfo.intCurPage != intTempPage)	{
		if ((intTempPage >= 1) && (intTempPage <= m_uInfo.intMaxPage))	{
			// 範囲内：新ページに更新
			m_uInfo.intCurPage = intTempPage;

// 修正No.162446 del -->
			//// １頁の更新
			//virUpdateControlTblData();
			//// フォーカスをデータ入力欄に
			////CmnDiagSetFocus( m_pDiag, 1, 1 );	// 修正No.162381,162382 del
			//PostMessage(WM_USER_FOCUSSET, 0, 0);	// 修正No.162381,162382 add
// 修正No.162446 del <--
// 修正No.162446 add -->
			// １頁の更新
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
void CfrmUc011YotyokinK::TerminationPagediag1(short index, short nChar, short length, LPUNKNOWN data)
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
//		m_ChangeFont = TRUE;
//		virUpdateControlTblData();
//	}
//// 改良No.22-0503 add <--
// 修正No.162446 del <--

	// フォーカス移動
	switch(nChar)	{
	case VK_RETURN:
		// Return：頁確定のため
		CmnDiagSetFocus(m_pDiag, 1, 1);
		break;
	case VK_TAB:
		// Tab/Shift+Tabにてフォーカス移動
		if(CmnCheckShiftKey() == FALSE)	{
			// Tab：タイトルコントロールへ移動
// midori 160612 cor -->
			//m_TitleDiag.SetPosition(0);
// ---------------------
			if(m_DataKakutei == FALSE)	m_TitleDiag.SetPosition(0);
// midori 160612 cor <--
		}
		else {
			// Shift+Tab：データ部の最終行へ移動（頁計/累計行の制御を行う）
			CmnDiagSetFocus(m_pDiag, nMaxDataCol, ID_COL_011_TEKIYOU);
		}
		break;
	case VK_DOWN:
		// タイトルコントロールへ移動
// midori 160612 cor -->
		//m_TitleDiag.SetPosition(0);
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
void CfrmUc011YotyokinK::FocusTitlediag1() 
{
//マウスクリックでキャレットが表示できないためコメントとする
//	// ボタンからTabキー移動した場合に必要
//	m_TitleDiag.SetPosition(0);
}

// タイトルコントロールのゲットフォーカス
void CfrmUc011YotyokinK::EditONTitlediag1( short index )
{
// midori 151405 add -->
	// スクロールフラグをクリアする
	nG_MScroll = 0;
	m_Titlefocus = 1;
// midori 151405 add <--
	// 1行登録
//	if( CmnGetControlCol() != ID_COL_011_KIMATU ){
		if ( virRecBufSaveData(m_uInfo.intCurPage, m_uInfo.intCurRow) == FUNCTION_UPDATE) {
			m_TitleDiag.SetPosition(0);
		}
//	}
}

// タイトルコントロールのロストフォーカス
void CfrmUc011YotyokinK::EditOFFTitlediag1(short index)
{
// midori 151405 add -->
	m_Titlefocus = 0;
// midori 151405 add <--
	virUpdateTitle();
}

//	タイトルの更新
void CfrmUc011YotyokinK::virUpdateTitle()
{
	// 帳表タイトル取得してuc_inf_subを更新
	m_uInfo.strTitleName = m_clsFunc.DiagGetString(&m_TitleDiag, 0);
	CmnUcInfSubSetTitleInfo(TRUE);	
}

// タイトルコントロールのキーイベント
void CfrmUc011YotyokinK::TerminationTitlediag1(short index, short nChar, short length, LPUNKNOWN data) 
{
	// フォーカス移動
	switch (nChar) {
	case VK_RETURN:
		// １行目の左上へ移動
		CmnDiagSetFocus(m_pDiag, 1, 1);
		break;
	case VK_TAB:
		// Shiftキーの有無は？
		if (CmnCheckShiftKey() == FALSE) {
			// １行目の左上へ移動
			CmnDiagSetFocus(m_pDiag, 1, 1);
		}
		else {
			// 頁コントロールへ移動
			m_PageDiag.SetPosition(1);
		}
		break;
	case VK_UP:
		// 頁コントロールへ移動
		m_PageDiag.SetPosition(1);
		break;
	case VK_DOWN:
		// データ部の初期位置へ移動
		CmnDiagSetFocus(m_pDiag, 1, m_uInfo.intCursolDefPos);
		break;
	case VK_DELETE:
		// タイトル削除
		m_TitleDiag.DataClear( 0 , TRUE );
		break;
	}
}

//********************************************************************************
//	預貯金データコントロール処理（EditON，EditOFF，Terminationイベント）
//********************************************************************************

// 預貯金データコントロールのゲットフォーカス
void CfrmUc011YotyokinK::EditONYoushikidiag1(short index) 
{
	DIAGRAM_DATA diadata;

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
	// 「種類」列ならコンボボックスのオフセット値取得
	if ( m_uInfo.intCurCol == ID_COL_011_SYURUI ){
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
	}
	else{
		// 現在の入力値を取得
		m_uInfo.OldstrDiag = m_clsFunc.DiagGetString( m_pDiag, m_uInfo.intCurCtlIndex );
	}	

	// 行移動確認
	// 値の保存（virRecBufSaveData）は長谷川が引き継いでから改良依頼No13-1449対応の一度EditOff時になったが、再度EditOn時にする
	// 理由：銀行名→支店名のようにカラムは分かれているがセットで保存したい箇所が存在する為
	// 改良依頼No13-1449に関してはF9押下時も値を保存することで対応できた
	if ( /*( m_uInfo.OldRow != CmnGetControlRow() ) &&*/ ( m_uInfo.OldRow > 0 ) ){
		if ( m_uInfo.OldPage == m_uInfo.intCurPage && m_uInfo.OldPage > 0 ) {
			//if( CmnGetControlCol() != ID_COL_011_KIMATU ){
			if(IsSaveMoveReference(ID_COL_011_KINYU, ID_COL_011_KINYUSUB)){	// 銀行名→支店名　支店名→銀行名の移動は保存しない
				// カレント行登録＋再表示
				if ( virRecBufSaveData(m_uInfo.intCurPage , m_uInfo.OldRow) == FUNCTION_UPDATE ) {
					// カーソル移動処理
					CmnDiagSetFocus( m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol );
				}
			}
// No.200601 add -->
			else {
				BankCellChg(m_uInfo.OldRow,m_uInfo.OldCol);
			}
// No.200601 add <--
			//}
		}
	}

	// ボタン操作
	CmnChangeButtonEnable(1);
}

// データコントロールのロストフォーカス
void CfrmUc011YotyokinK::EditOFFYoushikidiag1(short index) 
{
	// 平成26年度消費税一次対応で会社切替時にメッセージが出力された場合、
	// EditOffが走りDB書き込み時にエラーが発生していた為、切替先会社が
	// 消費税コンバージョンが必要な(ret == -2)場合はEditOffを即終了
	if( m_EndView != FALSE )	return;

	virInputDecisionEditOFF();

// midori 152134 del -->
//// midori 161106 add -->
//	ComboNew(index);
//// midori 161106 add <--
// midori 152134 del <--
// midori 152134 add -->
	if(m_uInfo.intCurCol == ID_COL_011_SYURUI)	{
		if(m_ComboSgn != 1)	{
			ComboNew(index);
		}
	}
	m_ComboSgn = 0;
// midori 152134 add <--
}

//	入力確定
void CfrmUc011YotyokinK::virInputDecisionEditOFF()
{
	// フォーカスフラグOFF
	m_uInfo.intCurFocus = 0;

	//	入力項目を強制的に確定
	CmnDiagSetEnterReDraw( m_pDiag, m_pDiag->GetPosition() );

// 当帳表では、一括集計できないため、処理が不要
//	// 自動一括集計行なら、変更できてしまうComboBoxの値を元に戻す
//	if (m_ReadData[m_uInfo.intCurRow].m_FgFunc == ID_FGFUNC_IKKATUAUTO) {
//		if (m_uInfo.intCurCol == ID_COL_011_SYURUI) {
//			CmnDiagSetCombo(m_pDiag, CmnGetControlIndex(m_uInfo.intCurRow, ID_COL_011_SYURUI), m_uInfo.OldCombo);
//		}
//	}

	// 行取得
	if (m_uInfo.intCurCtlIndex < m_uInfo.intT1StartIndex) {
		m_uInfo.OldRow = CmnGetControlRow();
	}
	else{
		m_uInfo.OldRow = -1;
	}

	// 金額欄の入力が確定した時点で反映
	// 廃止：従来通り、EditOn時に反映する
	// 理由：銀行名→支店名をセットで保存したい為
//	if (  m_uInfo.OldRow > 0 ){
//		//if( CmnGetControlCol() == ID_COL_011_KIMATU ){
//		//if ( m_uInfo.OldPage == m_uInfo.intCurPage ) {
//
//		// カレント行登録＋再表示
//		virRecBufSaveData(m_uInfo.intCurPage , m_uInfo.OldRow );
//		//}
//		//}
//	}

	// ページ番号取得
	m_uInfo.OldPage = m_uInfo.intCurPage;

	// ボタン操作
	CmnChangeButtonEnable(0);
}

// 要通知案件修正 21/07/09 add -->
void CfrmUc011YotyokinK::virActivateFrameK(UINT nState, CFrameWnd* pDeactivateFrame)
{
	int		index = 0;
	CWnd*	pWnd = this->GetFocus();

	if(pWnd != NULL)	{
		if(m_DataKakutei == FALSE)	{
			if(m_uInfo.intCurCol == ID_COL_011_SYURUI)	{
				virKamokuAddCancel();
			}
		}
	}
}

void CfrmUc011YotyokinK::virKamokuAddCancel()
{
	if(m_DataKakutei == TRUE)	return;

	// ポジション取得
	m_uInfo.intCurCtlIndex = m_pDiag->GetPosition();
	// 行・列取得
	CmnGetControlPosition(&m_uInfo.intCurRow,&m_uInfo.intCurCol,&m_uInfo.OldCol);
	// 科目列なら
	if(m_uInfo.intCurCol == ID_COL_011_SYURUI && m_F9OnSw != TRUE)	{
		DIAGRAM_DATA	diadata;
		m_pDiag->GetData(m_uInfo.intCurCtlIndex,(LPUNKNOWN)&diadata);
		// 【新規作成】が選択されていれば
		if(diadata.data_combo > m_CmbCnt)	{
			// 前回の値に戻す
			CmnDiagSetCombo(m_pDiag,CmnGetControlIndex(m_uInfo.intCurRow,ID_COL_011_SYURUI),m_uInfo.OldCombo);
		}
	}
}
// 要通知案件修正 21/07/09 add <--

// 預貯金データコントロールのキーイベント
void CfrmUc011YotyokinK::TerminationYoushikidiag1(short index, short nChar, short length, LPUNKNOWN data) 
{
	//--------------------------------------------------------------------------------
	//	キー操作仕様
	//		Deleteキー：	カレントの項目をクリア（ComboBoxはクリアしない）
	//		Tab/Shift+Tab：	１つ前/次の項目へ移動
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
	int	intCol = CmnGetControlCol();

	switch (nChar) {
	// Deleteキー
	case VK_DELETE:
		// 項目をクリア（ただし特殊行はクリアしない＋ComboBoxはクリアしない）
//		if (intCol != ID_COL_011_SYURUI) {
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
			// 入力可能な行の場合
			if (intCol != ID_COL_011_SYURUI) {
				m_pDiag->DataClear(m_uInfo.intCurCtlIndex, TRUE);
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
	//	if ((nChar == VK_LEFT) || (nChar == VK_RIGHT)) {
	//		// ComboBoxは選択内容を確定させない
	//		if (intCol == ID_COL_011_SYURUI) {
	//			m_pDiag->ComboDropDown(m_uInfo.intCurCtlIndex, FALSE);
	//			CmnDiagSetCombo(m_pDiag, m_uInfo.intCurCtlIndex, m_uInfo.OldCombo);
	//		}
	//	}

	//	// フォーカス移動先を取得
	//	int intRet = GetFocusPosition(nChar);
	//	switch (intRet) {
	//	case -1:		// 先頭項目でShift+Tabキーのため、タイトルへ移動
	//	case -4:		// 先頭行で↑or先頭項目で←は、タイトルへ移動
	//		m_TitleDiag.SetPosition(0);
	//		break;
	//	case -2:		// 最終項目でTabキーのため、頁コントロールへ移動
	//		m_PageDiag.SetPosition(1);
	//		break;
	//	case -3:		// ComboBoxで↑↓キーのため何もしない
	//		break;
	//	default:		// フォーカス移動を行う
	//		m_pDiag->SetPosition(intRet);
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
void CfrmUc011YotyokinK::VScrollYoushikidiag1(short mode)
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
//						-1:	先頭項目で"Shift+Tab"のためタイトルへ移動
//						-2:	最終項目で"Shift"のため別コントロールへ移動
//						-3:	ComboBoxで"↑/↓"はキー操作を無視する
//						-4:	先頭行で↑or先頭項目で←はタイトルへ移動
//********************************************************************************
int	CfrmUc011YotyokinK::GetFocusPosition(int nChar)
{
	int		intRow;					// カレント行番号
	int		intCol;					// カレント列番号
	int		intVector[4];			// 矢印キーの移動先（0:↑，1:→，2:↓，3:←）
	int		intNext = 0;			// フォーカス移動先
	int		intRowChange = 0;		// 行をまたぐ場合のインデックス増減数

	// カレント行＋列番号を取得
	CmnGetControlPosition(&intRow, &intCol);

	// 行をまたぐ場合のインデックス増減数（摘要→次の行の金融機関名）
// midori 190301 del -->
	//intRowChange = m_uInfo.intCtlStartIndex + 2;				// ここの値は検討が必要
// midori 190301 del <--
// midori 190301 add -->
	intRowChange = m_uInfo.intCtlStartIndex + 1;				// ここの値は検討が必要
// midori 190301 add <--

	// 「種類」はComboBoxのため、「↑，↓」キーではカーソルを移動しない
	if (intCol == ID_COL_011_SYURUI) {
		if ((nChar == VK_UP) || (nChar == VK_DOWN)) {
			// キー操作を無視する
			return -3;
		}
	}

	//---------------------------------------------------------------- 
	//	矢印キーによる移動先を取得（合計欄は次のステップで対応）
	//	基本的にReturn，Tab，Shift+Tabは、←，→キーと同じなので省略
	//---------------------------------------------------------------- 
// midori 190301 del -->
	//if ( intCol == ID_COL_011_KINYU ){
	//	// カレント列が「金融機関名」の場合
	//	intVector[0] = m_uInfo.intCurCtlIndex - m_uInfo.intColMax + 1;	// ↑
	//	intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
	//	intVector[2] = m_uInfo.intCurCtlIndex + 1;						// ↓
	//	intVector[3] = m_uInfo.intCurCtlIndex - intRowChange;			// ←
	//}
	//else if ( intCol == ID_COL_011_KINYUSUB ){
	//	// カレント列が「支店名」の場合
	//	intVector[0] = m_uInfo.intCurCtlIndex - 1;						// ↑
	//	intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
	//	intVector[2] = m_uInfo.intCurCtlIndex + m_uInfo.intColMax - 1;	// ↓
	//	intVector[3] = m_uInfo.intCurCtlIndex - 1;						// ←
	//}
// midori 190301 del <--
// midori 190301 add -->
	if ( intCol == ID_COL_011_KINYU ){
		// カレント列が「金融機関名」の場合
		intVector[0] = m_uInfo.intCurCtlIndex - m_uInfo.intColMax;		// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + m_uInfo.intColMax;		// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - intRowChange;			// ←
	}
// midori 190301 add <--
	else if ( intCol == ID_COL_011_TEKIYOU ){
		// カレント列が「摘要」の場合
		intVector[0] = m_uInfo.intCurCtlIndex - m_uInfo.intColMax;		// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + intRowChange;			// →
		intVector[2] = m_uInfo.intCurCtlIndex + m_uInfo.intColMax;		// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;						// ←
	}
	else{
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
			if ((intRow <= 1) && (intCol == ID_COL_011_KINYU)) {
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
int CfrmUc011YotyokinK::GetTotalRowIndex(int nCharOrg ,int intIndex ,int intNext)
{
	BOOL	blnFlg;					// カレント頁の計設定
	int		nCharTemp;				// キーコード（VK_RETURN，VK_TABを矢印キーに置き換え）
	int		intNowRow;
	int		intNextRow;

	// カレント頁の計設定を取得
	blnFlg = CmnGetTotalRowType();

	// キーコード置き換え（VK_RETURN，VK_TABを矢印キーに置き換え）
	switch (nCharOrg) {
	case VK_RETURN:			// Returnキーなら→キーと同じ
		nCharTemp = VK_RIGHT;
		break;
	case VK_TAB:			// Tabキーなら→キーと同じ，Shift+Tabなら←キーと同じ
		nCharTemp = VK_RIGHT;
		if (CmnCheckShiftKey() == TRUE) {
			nCharTemp = VK_LEFT;
		}
		break;
	default:
		nCharTemp = nCharOrg;
		break;
	}

	//------------------------------------------------
	//	預貯金画面のコントロール構成
	//		 1～23行目：データ行（1行=13個）
	//		　　23行目：合計行①（1行=13個）
	//		　　24行目：合計行②（1行=13個）
	//------------------------------------------------

	intNowRow = CmnGetControlRow(intIndex);		// カレントの行を取得
	intNextRow = CmnGetControlRow(intNext);		// 移動予定の行を取得

	// 現在22行目のデータ行にいる場合
	if (intNowRow == 22) {
		// 計が2行あり && 次の行へ移る
		if( (blnFlg != FALSE ) && (intNextRow >= 23) ){
			// 最終行・最終カラム→Tabキーでページダイアグラムへ移動
			if( nCharOrg == VK_TAB ){
				return -2;
			}
		}
	}

	// 現在23行目のデータ行にいる場合
	else if ((intNowRow == 23) && (intIndex < m_uInfo.intT1StartIndex)) {
		if( (blnFlg == FALSE ) && (intNext >= m_uInfo.intT1StartIndex) ){
			if( nCharOrg == VK_TAB ){
				return -2;
			}
		}
	}

	return intNext;
}

//********************************************************************************
//	ソート項目を再設定後に、ソートを実行する
//		IN		CdlgSort*			ソートダイアログ情報
//				CdbUc000Common*		テーブル情報
//		RET		int					実行結果（FUNCTION_OK, FUNCTION_NG）
//********************************************************************************
int CfrmUc011YotyokinK::virTblSortSubSortProc(CdlgSort* pCdlgSort, CdbUc000Common* rsData)
{
	CdbUc011Yotyokin*	rs;
	CString				strFilter;
	CString				strSort;

	// ソート項目の再設定（金融機関）	→ 帳表により実行有無が異なる
	CdbUcLstBank	rsBank( m_pDB );
//	rsBank.UpdateDataTable(TBL_NAME_011);
	rsBank.UpdateDataTable(m_uInfo.intFormSeq, m_RegAutoRef);
	rsBank.Fin();

	// ソート項目の再設定（科目）		→ 帳表により実行有無が異なる
	CdbUcLstKamoku	rsKmk( m_pDB );
//@	rsKmk.UpdateDataTable(TBL_NAME_011, m_uInfo.intFormSeq);
	rsKmk.UpdateDataTable(m_uInfo.intFormSeq);
	rsKmk.Fin();

	// ソート条件取得
	CmnTblSortSubGetSortParam(pCdlgSort, &strFilter, &strSort);

	rs = (CdbUc011Yotyokin*)rsData;

	// 対象フィールドがNULLならNULL文字列にする：文字列連結のソート時は必須
	rs->UpdateDataTableWork();

	// ソート実行
		return rs->RequerySortParameter(strFilter, strSort);
}


/**********************************************************************
	RecBufSetData(CdbUc011Yotyokin* Yotyokin)
		ローカルのレコードバッファに、1行分のデータを格納する

	引数
			CdbUc011Yotyokin*	Yotyokin	レコードセット(データがあること）
	戻値
			int					成功/失敗
								FUNCTION_OK
***********************************************************************/
int CfrmUc011YotyokinK::RecBufSetData(CdbUc011Yotyokin* Yotyokin)
{
	// 1ページ分のレコードバッファの該当行にデータを格納する
	int	trgRow = Yotyokin->m_NumRow;
	m_ReadData[trgRow].m_Seq		= Yotyokin->m_Seq;
	m_ReadData[trgRow].m_NumPage	= Yotyokin->m_NumPage;
	m_ReadData[trgRow].m_NumRow		= Yotyokin->m_NumRow;
	m_ReadData[trgRow].m_FgFunc		= Yotyokin->m_FgFunc;
	m_ReadData[trgRow].m_FgShow		= Yotyokin->m_FgShow;
	m_ReadData[trgRow].m_KeiStr		= Yotyokin->m_KeiStr;
	m_ReadData[trgRow].m_NumGroup	= Yotyokin->m_NumGroup;

	m_ReadData[trgRow].m_AcNum		= Yotyokin->m_AcNum;
	m_ReadData[trgRow].m_BkName1	= Yotyokin->m_BkName1;
	m_ReadData[trgRow].m_BkName2	= Yotyokin->m_BkName2;
	m_ReadData[trgRow].m_BkOrder	= Yotyokin->m_BkOrder;
	m_ReadData[trgRow].m_BkSeq		= Yotyokin->m_BkSeq;
	m_ReadData[trgRow].m_KnName		= Yotyokin->m_KnName;
	m_ReadData[trgRow].m_KnOrder	= Yotyokin->m_KnOrder;
	m_ReadData[trgRow].m_KnSeq		= Yotyokin->m_KnSeq;
	m_ReadData[trgRow].m_Teki		= Yotyokin->m_Teki;
	m_ReadData[trgRow].m_Val		= Yotyokin->m_Val;
// midori 152137 add -->
	m_ReadData[trgRow].m_BkKana1	= Yotyokin->m_BkKana1;
	m_ReadData[trgRow].m_BkKana2	= Yotyokin->m_BkKana2;
	m_ReadData[trgRow].m_KnKana		= Yotyokin->m_KnKana;
// midori 152137 add <--

	return FUNCTION_OK;
}

/**********************************************************************
	RecBufClearAllData()
		ローカルの1ページ分のレコードバッファを初期化する

	引数
			なし
	戻値
			int					成功/失敗
								FUNCTION_OK
***********************************************************************/
int CfrmUc011YotyokinK::RecBufClearAllData()
{
	// 1ページ分のレコードバッファを初期化する
	// 1行目から、最終行(計行も含む)までクリア
	// 配列の添え字は、行番号をそのまま使用
	// 使用していないが配列[0]も初期化している
	for (int trgRow = 0; trgRow <= m_uInfo.intRowMax; trgRow++) {
		
		RecBufClearData( &m_ReadData[trgRow] );
	}
	return FUNCTION_OK;
}

/**********************************************************************
	RecBufClearAllData()
		1レコード分のデータを初期化する

	引数
			P_REC_UC_011_YOTYOKIN inRecData	レコードへのポインタ
	戻値
			int					成功/失敗
								FUNCTION_OK
***********************************************************************/
int CfrmUc011YotyokinK::RecBufClearData( P_REC_UC_011_YOTYOKIN inRecData )
{
	inRecData->m_Seq		= 0;
	inRecData->m_NumPage	= 0;
	inRecData->m_NumRow		= 0;
	inRecData->m_FgFunc		= 0;
	inRecData->m_FgShow		= 0;
	inRecData->m_KeiStr		= _T("");
	inRecData->m_NumGroup	= 0;

	inRecData->m_AcNum		= _T("");
	inRecData->m_BkName1	= _T("");
	inRecData->m_BkName2	= _T("");
	inRecData->m_BkOrder	= 0;
	inRecData->m_BkSeq		= 0;
	inRecData->m_KnName		= _T("");
	inRecData->m_KnOrder	= 0;
	inRecData->m_KnSeq		= 0;
	inRecData->m_Teki		= _T("");
	inRecData->m_Val		= _T("");
	inRecData->m_ShowKeiZero = 0;	//０円計表示フラグ
// midori 152137 add -->
	inRecData->m_BkKana1	= _T("");
	inRecData->m_BkKana2	= _T("");
	inRecData->m_KnKana		= _T("");
// midori 152137 add <--

	return FUNCTION_OK;
}

/**********************************************************************
	virRecBufSaveData()
		指定された行のデータをテーブルに保存

	引数
			int		inPage		登録するレコードのページ番号
			int		inRow		登録するレコードの行番号
	戻値
			int					成功(更新あり)/成功(更新なし)/失敗
								FUNCTION_UPDATE	: 成功(更新あり)
								FUNCTION_OK		: 成功(更新なし)
								FUNCTION_NG		: 失敗
***********************************************************************/
int CfrmUc011YotyokinK::virRecBufSaveData(int inPage, int inRow)
{
	REC_UC_011_YOTYOKIN tmpSaveData;	// 画面データ1レコード格納用

	// 頁計/累計行は登録しない
	if ((m_ReadData[inRow].m_FgFunc == ID_FGFUNC_PAGEKEI) || (m_ReadData[inRow].m_FgFunc == ID_FGFUNC_RUIKEI)) {
		return FUNCTION_OK;
	}

	RecBufClearData( &tmpSaveData );	// レコードを初期化

	///////////////////////////////////////////////////////////////
	// コンポーネント(画面上の1レコード分の情報)を仮変数へ格納
	DIAGRAM_DATA	diadata;			// ICSDiag構造体
	int				intIndex;			// 対象行の各インデックス
	
	// 初期化
	m_clsFunc.DiagInit( &diadata );

	// 金融機関名格納
	intIndex = CmnGetControlIndex( inRow , ID_COL_011_KINYU );
	tmpSaveData.m_BkName1 = m_clsFunc.DiagGetString( m_pDiag, intIndex );
	
	// 支店名格納
	intIndex = CmnGetControlIndex( inRow , ID_COL_011_KINYUSUB );
	tmpSaveData.m_BkName2 = m_clsFunc.DiagGetString( m_pDiag, intIndex );
	
	// 種類インデックス格納
	intIndex = CmnGetControlIndex( inRow , ID_COL_011_SYURUI );
	m_pDiag->GetData( intIndex , (LPUNKNOWN)&diadata );
// midori 152137 del -->
//	tmpSaveData.m_KnOrder = diadata.data_combo;
// midori 152137 del <--
// midori 152137 add -->
	// コンボボックスの値をキーにKnOrder（科目順序）を取得
	m_SortMap2.Lookup(diadata.data_combo,tmpSaveData.m_KnOrder);
// midori 152137 add <--
	
	// 口座番号格納
	intIndex = CmnGetControlIndex( inRow , ID_COL_011_KOUZA );
	tmpSaveData.m_AcNum = m_clsFunc.DiagGetString( m_pDiag, intIndex );
	
	// 期末現在高格納
	intIndex = CmnGetControlIndex( inRow , ID_COL_011_KIMATU );
	tmpSaveData.m_Val = CmnDiagGetKingaku( m_pDiag, intIndex );

	// 摘要格納
	intIndex = CmnGetControlIndex( inRow , ID_COL_011_TEKIYOU );
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
		指定された行のデータをテーブルに保存(挿入貼付作業用)

	引数
			int		inPage		登録するレコードのページ番号
			int		inRow		登録するレコードの行番号
	戻値
			int					成功(更新あり)/成功(更新なし)/失敗
								FUNCTION_UPDATE	: 成功(更新あり)
								FUNCTION_OK		: 成功(更新なし)
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
int CfrmUc011YotyokinK::virRecBufSaveDataForPasteInsert(int inPage, int inRow)
{
	RecBufClearData( &m_ReadData[inRow] );			// レコードを初期化
	m_ReadData[inRow].m_FgFunc	= ID_FGFUNC_DATA;	//データ行とする

	///////////////////////////////////////////////////////////////
	// 登録処理呼び出し
	return	RecBufSaveDataSub( inPage, inRow, &m_CopyData );
}

/**********************************************************************
	virRecBufSaveDataForPaste()
		指定された行のデータをテーブルに保存(上書き貼付作業用)
***********************************************************************/
int CfrmUc011YotyokinK::virRecBufSaveDataForPaste(int inPage, int inRow)
{
	///////////////////////////////////////////////////////////////
	// 登録処理呼び出し
	return	RecBufSaveDataSub( inPage, inRow, &m_CopyData );
}

/**********************************************************************
	RecBufSaveDataSub()
		指定された行のデータをテーブルに保存(補助ルーチン)
		※ShowKeiZeroはUpdateCalcKeiで保存している

	引数
			int		inPage		登録するレコードのページ番号
			int		inRow		登録するレコードの行番号
			REC_UC_011_YOTYOKIN
					inSaveData	登録するデータレコードへのポインタ
	戻値
			int					成功(更新あり)/成功(更新なし)/失敗
								FUNCTION_UPDATE	: 成功(更新あり)
								FUNCTION_OK		: 成功(更新なし)
								FUNCTION_NG		: 失敗
***********************************************************************/
int CfrmUc011YotyokinK::RecBufSaveDataSub(
							int inPage, 
							int inRow,  
							P_REC_UC_011_YOTYOKIN inSaveData)
{
	CString				tmp_BkName1;		// 画面上のデータ格納用(銀行名)
	CString				tmp_BkName2;		// 画面上のデータ格納用(支店名)
	long				tmp_KnOrder;		// 画面上のデータ格納用(種類のorder番号)
	CString				tmp_AcNum;			// 画面上のデータ格納用(口座番号)
	CString				tmp_Val;			// 画面上のデータ格納用(金額)
	CString				tmp_Teki;			// 画面上のデータ格納用(摘要)
	long				tmp_BkOrder = 0;
	long				tmp_BkSeq = 0;
	CString				tmp_KnName = "";
	long				tmp_KnSeq = 0;
	BYTE				tmp_FgFunc = 0;
// midori 152137 add -->
	CString				tmp_BkKana1 = _T("");
	CString				tmp_BkKana2 = _T("");
	CString				tmp_KnKana = _T("");
// midori 152137 add <--
// midori 190301 add -->
	CString				cs = _T("");
// midori 190301 add <--
// midori 190504 add -->
	CStringArray		strArrayTmp;
// midori 190504 add <--

	// 変更があったかのフラグ（変更あり:TRUE、なし:FALSE)
	BOOL				ufgUpData	= FALSE;		// 変更確認フラグ(全体用データ行/空行)
	BOOL				ufgUpToku	= FALSE;		// 変更確認フラグ(全体用特殊行)
	BOOL				ufgBk		= FALSE;		// 変更確認フラグ(銀行名、支店名)
	BOOL				ufgKn		= FALSE;		// 変更確認フラグ(種類)
	BOOL				ufgAcNum	= FALSE;		// 変更確認フラグ(口座番号)
	BOOL				ufgVal		= FALSE;		// 変更確認フラグ(金額)
	BOOL				ufgTeki		= FALSE;		// 変更確認フラグ(摘要)
	BOOL				ufgFunc		= FALSE;		// 変更確認フラグ(摘要)

	// レコード更新したことを示すフラグ
	BOOL				ufgRsUpdate	= FALSE;

	// テーブルアクセスクラス
	CdbUc011Yotyokin	rsYoty( m_pDB );

	///////////////////////////////////////////////////////////////
	// 作業用変数へ 登録対象のフィールド値を代入
// midori 190504 del -->
	//tmp_BkName1	= inSaveData->m_BkName1;		// (銀行名)
	//tmp_BkName2	= inSaveData->m_BkName2;		// (支店名)
// midori 190504 del <--
// midori 190504 add -->
	// (銀行名)
	// 末尾のスペースをカット
	tmp_BkName1	= m_clsFunc.DeleteRightSpace( inSaveData->m_BkName1 );		
	// (支店名)
	// 各行末尾のスペースをカット
	//m_clsFunc.StrDivision(inSaveData->m_BkName2, &strArrayTmp, 2, TRUE, TRUE);
	//tmp_BkName2		= m_clsFunc.StrDocking( &strArrayTmp );
	tmp_BkName2	= m_clsFunc.DeleteRightSpace(inSaveData->m_BkName2);
// midori 190504 add <--
	tmp_KnOrder	= inSaveData->m_KnOrder;		// (種類のorder番号)
	tmp_AcNum	= inSaveData->m_AcNum;			// (口座番号)
	tmp_Val		= inSaveData->m_Val;			// (金額)
	tmp_Teki	= inSaveData->m_Teki;			// (摘要)
	
	///////////////////////////////////////////////////////////////
	// データ行か、空行かのチェック
	tmp_FgFunc = m_ReadData[inRow].m_FgFunc;

	if(tmp_FgFunc <= ID_FGFUNC_DATA)	{	// データ行 または、空行なら
		// コンポーネントをチェックして、データがあるかを調べる
		// (仮変数に入れた値でチェックしてもいいのかも？)

		if( (tmp_BkName1 == "")				// 画面上のデータ格納用(銀行名)
		 && (tmp_BkName2 == "")				// 画面上のデータ格納用(支店名)
		 && (tmp_KnOrder == 0)				// 画面上のデータ格納用(種類のorder番号)
		 && (tmp_AcNum == "")				// 画面上のデータ格納用(口座番号)
		 && (tmp_Val == "")					// 画面上のデータ格納用(金額)
		 && (tmp_Teki == "") )	{			// 画面上のデータ格納用(摘要)
			// 空行なら
			tmp_FgFunc = ID_FGFUNC_NULL;
		}
		else	{
			// データが入っていれば
			tmp_FgFunc = ID_FGFUNC_DATA;
		}
	}

	///////////////////////////////////////////////////////////////
	// 更新状況のチェック
	//(1) 銀行名、支店名
	if(tmp_BkName1 != m_ReadData[inRow].m_BkName1)	{
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgUpToku	= TRUE;		// 全体(特殊行)
		ufgBk		= TRUE;		// 銀行名用
	}
	if(tmp_BkName2 != m_ReadData[inRow].m_BkName2)	{
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgBk		= TRUE;		// 支店名用
	}

	//(2) 種類（Order番号管理になっている)
	if(tmp_KnOrder != m_ReadData[inRow].m_KnOrder)	{
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgKn		= TRUE;		// 種類用
	}

	//(3) 口座番号
	if(tmp_AcNum != m_ReadData[inRow].m_AcNum)	{
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgAcNum	= TRUE;		// 口座用
	}

	//(4) 金額
	if(tmp_Val != m_ReadData[inRow].m_Val)	{
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgVal		= TRUE;		// 金額用
	}

	//(5) 摘要
	if(tmp_Teki != m_ReadData[inRow].m_Teki)	{
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgTeki		= TRUE;		// 摘要用
	}

	//(6) 特殊行フラグ(基本的には、空行/データ行のチェックになるはず) 
	if(m_ReadData[inRow].m_FgFunc <= ID_FGFUNC_DATA)	{
		// 元が空行orデータ行の場合、入力内容により空行orデータ行となる
		// 元が特殊行の場合、FgFuncは更新しない
		if(tmp_FgFunc != m_ReadData[inRow].m_FgFunc)	{
			ufgUpData	= TRUE;		// 全体(データ行/空行)
			ufgFunc		= TRUE;		// ufgFunc用
		}
	}

	///////////////////////////////////////////////////////////////////
	// 1 レコード更新処理
	if((tmp_FgFunc == ID_FGFUNC_DATA) || (tmp_FgFunc == ID_FGFUNC_IKKATUMANUAL))	{	// データ行or一括集計金額行（手動）
		///////////////////////////////////////////////////////////////////
		// データ行or一括集計金額行（手動）の場合の処理

		// 金融機関名称リストへの登録チェック
		if((tmp_FgFunc == ID_FGFUNC_DATA) && (ufgBk == TRUE))	{	// 変更ありの場合
			CdbUcLstBank		rsBank( m_pDB );

// midori 190301 del -->
			//if(m_RegAutoRef)	tmp_BkSeq = rsBank.CheckEntry(tmp_BkName1, tmp_BkName2);
			//else				tmp_BkSeq = rsBank.GetSeqFromBkName(tmp_BkName1, tmp_BkName2);
// midori 190301 del <--
// midori 190301 add -->
			cs = tmp_BkName2;
			cs.Replace(PRT_FORMAT_CHCODE, _T(""));	// 改行マークを変換

			if(m_RegAutoRef)	tmp_BkSeq = rsBank.CheckEntry(tmp_BkName1,cs);
			else				tmp_BkSeq = rsBank.GetSeqFromBkName(tmp_BkName1,cs);
// midori 190301 add <--

			// エラー？
			if(tmp_BkSeq < 0)	{
				// 抜ける
				return(FUNCTION_NG);
			}
			// 全てヌルだった？
			if(tmp_BkSeq == 0)	{
				tmp_BkSeq = 0;
				tmp_BkOrder = 0;
// midori 152137 add -->
				tmp_BkKana1 = _T("");
				tmp_BkKana2 = _T("");
// midori 152137 add <--
			}
			//	登録確認成功
			else	{
				tmp_BkOrder = rsBank.m_OrderNum;
// midori 152137 add -->
				tmp_BkKana1 = rsBank.m_BkKana1;
				tmp_BkKana2 = rsBank.m_BkKana2;
// midori 152137 add <--
			}

			rsBank.Fin();
		}

		// 種類について KnOrder に関連する、名称、シーケンスを取得
		if((ufgKn == TRUE) && (tmp_KnOrder > 0))	{
			// 科目インデックスがNULLなら登録しない
			CdbUcLstKamoku		rsKamo( m_pDB );
			int		intRet = rsKamo.RequeryKnOrder(m_uInfo.intFormSeq, tmp_KnOrder);
			if(intRet != DB_ERR_OK)	{	// 失敗
				return FUNCTION_NG;
			}
			tmp_KnSeq	= rsKamo.m_KnSeq;
			tmp_KnName	= rsKamo.m_KnName;
// midori 152137 add -->
			tmp_KnKana	= rsKamo.m_KnKana;
// midori 152137 add <--

			rsKamo.Fin();
		}

		// 更新処理
		if(ufgUpData == TRUE)	{		// テーブルへ更新(1レコード更新)
			// ページ、行 で抽出し、必要なデータを更新
			rsYoty.RequeryPageRow(inPage, inRow);

			if( !rsYoty.IsEOF() )	{		// レコードが取得できた(取得できないことはないはず)
				rsYoty.Edit();

				if(ufgBk == TRUE)	{	// 変更ありの場合
					rsYoty.m_BkSeq		= tmp_BkSeq;
					rsYoty.m_BkName1	= m_clsFunc.DeleteRightSpace( tmp_BkName1 );
					rsYoty.m_BkName2	= m_clsFunc.DeleteRightSpace( tmp_BkName2 );
					rsYoty.m_BkOrder	= tmp_BkOrder;
// midori 152137 add -->
					rsYoty.m_BkKana1	= tmp_BkKana1;
					rsYoty.m_BkKana2	= tmp_BkKana2;
// midori 152137 add <--
				}

				if(ufgKn == TRUE)	{	// 変更ありの場合
					rsYoty.m_KnSeq		= tmp_KnSeq;
					rsYoty.m_KnName		= tmp_KnName;
					rsYoty.m_KnOrder	= tmp_KnOrder;
// midori 152137 add -->
					rsYoty.m_KnKana		= tmp_KnKana;
// midori 152137 add <--
				}

				if(ufgAcNum == TRUE)	{	// 変更ありの場合
					rsYoty.m_AcNum		= tmp_AcNum;
				}

				if(ufgVal == TRUE)	{		// 変更ありの場合
					rsYoty.m_Val		= tmp_Val;
				}

				if(ufgTeki == TRUE)		{	// 変更ありの場合
					CStringArray strArray;
// midori 190301 del -->
					//m_clsFunc.StrDivision( tmp_Teki, &strArray, 2, TRUE, TRUE );
// midori 190301 del <--
// midori 190301 add -->
					//m_clsFunc.StrDivision(tmp_Teki,&strArray,3,TRUE,TRUE);
					m_clsFunc.StrDivision(tmp_Teki, &strArray, 2, TRUE, TRUE);
// midori 190301 add <--
					rsYoty.m_Teki		= m_clsFunc.StrDocking( &strArray );
				}
				
				// 特殊行フラグは、強制的に書き込む
				rsYoty.m_FgFunc		= tmp_FgFunc;

				rsYoty.Update();
				ufgRsUpdate = TRUE;		// レコード更新したので ON
			}
		}

	}
	else if(tmp_FgFunc == ID_FGFUNC_NULL)		{	// 空行
		///////////////////////////////////////////////////////////////////
		// 空行の場合の処理

		// 更新処理
		if(ufgUpData == TRUE)	{		// テーブルへ更新(1レコード更新)
			// ページ、行 で抽出し、必要なデータを更新
			rsYoty.RequeryPageRow(inPage, inRow);

			if( !rsYoty.IsEOF() )	{		// レコードが取得できた(取得できないことはないはず)
				rsYoty.Edit();
				rsYoty.m_FgFunc		= tmp_FgFunc;
				rsYoty.m_KeiStr		= _T("");
				rsYoty.m_NumGroup	= 0;
				rsYoty.m_BkName1	= _T("");
				rsYoty.m_BkName2	= _T("");
				rsYoty.m_BkOrder	= 0;
				rsYoty.m_BkSeq		= 0;
				rsYoty.m_KnName		= _T("");
				rsYoty.m_KnOrder	= 0;
				rsYoty.m_KnSeq		= 0;
				rsYoty.m_AcNum		= _T("");
				rsYoty.m_Val		= _T("");
				rsYoty.m_Teki		= _T("");
// midori 152137 add -->
				rsYoty.m_BkKana1	= _T("");
				rsYoty.m_BkKana2	= _T("");
				rsYoty.m_KnKana		= _T("");
// midori 152137 add <--

				rsYoty.Update();

				ufgRsUpdate = TRUE;		// レコード更新したので ON
			}
		}
	}
// 2006.02.26：特殊行は登録しない
/***
	else  {		// データ行、空行 ではない（特殊行)
		///////////////////////////////////////////////////////////////////
		// 特殊行の場合の処理

		// 更新処理
		if (ufgUpToku == TRUE) {		// テーブルへ更新(1レコード更新)
			// ページ、行 で抽出し、必要なデータを更新
			rsYoty.RequeryPageRow(inPage, inRow);

			if ( !rsYoty.IsEOF() ) {		// レコードが取得できた(取得できないことはないはず)
				rsYoty.Edit();
				rsYoty.m_BkName1	= tmp_BkName1;
				rsYoty.Update();

				ufgRsUpdate = TRUE;		// レコード更新したので ON
			}
		}
	}
***/

	///////////////////////////////////////////////////////////////////
	// 内部 1ページバッファも更新する
	int		retFunc = FUNCTION_OK;		// 戻り値：成功（変更なし)
	if(ufgRsUpdate == TRUE)		{
		retFunc = FUNCTION_UPDATE;		// 戻り値：成功（変更あり)
		if( !rsYoty.IsEOF() )	{		// レコードが取得できた(取得できないことはないはず)
			RecBufSetData( &rsYoty );
		}
	}

	rsYoty.Fin();
	
	return(retFunc);
}

//
CdbUc000Common* CfrmUc011YotyokinK::virGetDataTableObject()
{
	// 預貯金テーブルのオブジェクトを生成し、ポインタを返す	
	return (CdbUc000Common*)(new CdbUc011Yotyokin(m_pDB));
}


//********************************************************************************
//	行コピー処理
//		IN		なし
//		RET		なし
//********************************************************************************
void CfrmUc011YotyokinK::virRowButtonCopyProc()
{
	// カレント行のデータを、コーピー用バッファに格納
	m_CopyData	= m_ReadData[m_uInfo.intCurRow];

	// コピーフラグON
	m_nCopyFlag	= 1;
}

//********************************************************************************
//	テーブルから科目情報を取得
//		IN		Cdb000Common* rsData	テーブル情報
//				SORTKAMOKU_INFO*		科目情報
//		RET		なし
//********************************************************************************
void CfrmUc011YotyokinK::virTblGetKamokuInfo(CdbUc000Common* rsData, SORTKAMOKU_INFO* uKamoku)
{
	CdbUc011Yotyokin*	rs011 = (CdbUc011Yotyokin*)rsData;

	// 本当ならGetFieldValue()で取得する予定だったが、時間も無いのであきらめた
	uKamoku->intKnSeq = rs011->m_KnSeq;
	uKamoku->intKnOrder = rs011->m_KnOrder;
	uKamoku->strKnName = rs011->m_KnName;
// midori 152137 add -->
	uKamoku->strKnKana = rs011->m_KnKana;
// midori 152137 add <--
}

//********************************************************************************
//	共通処理から帳表のカレント情報を取得
//		IN		CURRENT_INFO			取得する情報の格納領域
//		RET		なし
//********************************************************************************
void CfrmUc011YotyokinK::virGetCurrentInfo(CURRENT_INFO* uCurInfo)
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
void CfrmUc011YotyokinK::SetInputMode( int nRow, long nMode )
{
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_011_KINYU ), nMode );			//	銀行名
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_011_KINYUSUB ), nMode );		//	支店名
// midori M-16082901 add -->
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_011_SYURUI ), nMode );		//	種類
// midori M-16082901 add <--
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_011_KOUZA ), nMode );			//	口座番号
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_011_KIMATU ), nMode );		//	期末現在高
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_011_TEKIYOU ), nMode );		//	摘要
}

//**************************************************
//	各Diagの背景色(BackColor)に色を設定
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CfrmUc011YotyokinK::SetDiagBackColor()
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
void CfrmUc011YotyokinK::virRendoCreateData(CdbUc000Common* rsData, RENTEMP_INFO* uRenInfo)
{
	CString			strTemp;
// midori 190301 add -->
	CString				cs1=_T("");
	CString				cs2=_T("");
	CString				cs3=_T("");
	CUcFunctionCommon	ufc;
// midori 190301 add <--
	CStringArray	strArry;
	int				intSeq;
// midori 152153 add -->
	int				sw = 0;
	CdbUcLstBank	rsLstBnk(m_pDB);	// rs_lst_bank
// midori 152153 add <--

	CdbUc011Yotyokin*	rs = (CdbUc011Yotyokin*)rsData;

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

	// 銀行情報：取引先登録チェック
// midori 152153 del -->
	//CdbUcLstBank		rsLstBnk(m_pDB);								// rs_lst_bank
	//m_clsFunc.StrDivisionEx( uRenInfo->strAdName , &strArry , MAX_KETA_011_KINYU , 2 );
	//intSeq = rsLstBnk.CheckEntry(strArry.GetAt(0), strArry.GetAt(1));
	//if (intSeq > 0) {
	//	rs->m_BkSeq		= rsLstBnk.m_Seq;
	//	rs->m_BkOrder	= rsLstBnk.m_OrderNum;
	//	rs->m_BkName1	= rsLstBnk.m_BkName1;
	//	rs->m_BkName2	= rsLstBnk.m_BkName2;
	//}
	//rsLstBnk.Fin();
// midori 152153 del <--
// midori 152153 add -->
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
		m_clsFunc.StrDivisionEx( uRenInfo->strAdName , &strArry , MAX_KETA_011_KINYU , 2 );
		if(sw == 1)	{
// midori 152137 del -->
//			intSeq = rsLstBnk.CheckEntry(strArry.GetAt(0), strArry.GetAt(1));
// midori 152137 del <--
// midori 152137 add -->
			intSeq = rsLstBnk.CheckEntryRen(strArry.GetAt(0), strArry.GetAt(1),uRenInfo->strAdKana);
// midori 152137 add <--

			if (intSeq > 0) {
				rs->m_BkSeq		= rsLstBnk.m_Seq;
				rs->m_BkOrder	= rsLstBnk.m_OrderNum;
				rs->m_BkName1	= rsLstBnk.m_BkName1;
// midori 190301 del -->
				//rs->m_BkName2	= rsLstBnk.m_BkName2;
// midori 190301 del <--
// midori 190301 add -->
				strTemp = rsLstBnk.m_BkName2;
				if(strTemp.GetLength() > 10)	{
					cs2 = ufc.GetSpritString(strTemp,&cs1,10);
					strTemp = cs2;
					cs3 = ufc.GetSpritString(strTemp,&cs2,10);
					strTemp = cs1 + PRT_FORMAT_CHCODE + cs2;
				}
				rs->m_BkName2 = strTemp;
// midori 190301 add <--
// midori 152137 add -->
				rs->m_BkKana1	= rsLstBnk.m_BkKana1;
				rs->m_BkKana2	= rsLstBnk.m_BkKana2;
// midori 152137 add <--
			}
		}
		else if(sw == 2)	{
			if(strArry.GetAt(0).IsEmpty() == FALSE || strArry.GetAt(1).IsEmpty() == FALSE)	{
				rs->m_BkSeq		= 0;
				rs->m_BkOrder	= 0;
				rs->m_BkName1	= strArry.GetAt(0);
// midori 190301 del -->
				//rs->m_BkName2	= strArry.GetAt(1);
// midori 190301 del <--
// midori 190301 add -->
				strTemp = strArry.GetAt(1);
				if(strTemp.GetLength() > 10)	{
					cs2 = ufc.GetSpritString(strTemp,&cs1,10);
					strTemp = cs2;
					cs3 = ufc.GetSpritString(strTemp,&cs2,10);
					strTemp = cs1 + PRT_FORMAT_CHCODE + cs2;
				}
				rs->m_BkName2 = strTemp;
// midori 190301 add <--
// midori 152137 add -->
				rs->m_BkKana1	= _T("");
				rs->m_BkKana2	= _T("");
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
void CfrmUc011YotyokinK::virRendoUpdateData(CdbUc000Common* rsData, RENTEMP_INFO* uRenInfo)
{
	CdbUc011Yotyokin*	rs = (CdbUc011Yotyokin*)rsData;

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
BOOL CfrmUc011YotyokinK::virEndProc(void)
{
	TRACE(_T("***** CfrmUc011YotyokinK::virEndProc\n"));

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
void CfrmUc011YotyokinK::ComboNew(short index)
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
	if ( m_uInfo.intCurCol == ID_COL_011_SYURUI ){
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
int CfrmUc011YotyokinK::CursorControl(short nChar, int sw)
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
//		if (intCol == ID_COL_011_SYURUI) {
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
		if (intCol == ID_COL_011_SYURUI)	{
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
	//	if (intCol == ID_COL_011_SYURUI) {
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
void CfrmUc011YotyokinK::ComboSelYoushikidiag1(short index, LPUNKNOWN data)
{
// 要通知案件修正 21/07/09 del -->
	//char	key=0x00;		// →矢印キー状態取得
	//char	key2=0x00;		// Enterキー状態取得

	//key = ( char )GetKeyState( VK_RIGHT );				//	→矢印キー状態取得
	//key2 = ( char )GetKeyState( VK_RETURN );			//	→Enterキー状態取得

	//m_ComboSgn = 0;
	//if((key & 0x80) != 0x80 && (key2 & 0x80) != 0x80)	{
	//	if(m_uInfo.intCurCol == ID_COL_011_SYURUI)	{
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
		if(m_uInfo.intCurCol == ID_COL_011_SYURUI) {
			ComboNew(index);
			m_ComboSgn = 1;
		}
	}
// 要通知案件修正 21/07/09 add <--
}
// midori 152134 add <--

// midori 190301 add -->
// CdbUcLstBank からセットした支店名に、改行マークを挿入する
//void CfrmUc011YotyokinK::DataConver( void )
//{
//	CdbUc011Yotyokin	db011(((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub->m_database);
//	int					sw=0;
//	CString				cs=_T("");
//	CString				cs1=_T("");
//	CString				cs2=_T("");
//	CString				cs3=_T("");
//	CUcFunctionCommon	ufc;
//
//	db011.Open();
//	while(!db011.IsEOF()){		// ファイル終端まで
//		sw=0;
//		cs = db011.m_BkName2;
//		if(cs.IsEmpty() == FALSE)	{
//			// 全角５文字を超えていれば改行マークを挿入して取り込む
//			if(cs.GetLength() > 10)	{
//				cs2 = ufc.GetSpritString(cs,&cs1,10);
//				cs = cs2;
//				cs3 = ufc.GetSpritString(cs,&cs2,10);
//				cs = cs1 + PRT_FORMAT_CHCODE + cs2;
//				sw = 1;
//			}
//		}
//		if(sw != 0)	{
//			db011.Edit();
//			db011.m_BkName2 = cs;
//			db011.Update();
//		}
//		db011.MoveNext();
//	}
//	db011.Close();
//}
// midori 190301 add <--

// No.200601 add -->
// ------------------------------------------------------------------------------------------------------------------------
// 金融機関名 → 支店名にカーソルが移動する場合、
// データの保存、再読み込みが発生しないため、
// 電子申告の桁数チェックを行い、文字色を変更する
// ------------------------------------------------------------------------------------------------------------------------
void CfrmUc011YotyokinK::BankCellChg(int nRow,int nCol)
{
	int				sign=0;
	int				index=0;
	int				nCharOver=0;
// 修正No.157626 add -->
	int				nCharOver2[5]={0};
// 修正No.157626 add <--
	CString			cst=_T("");
	CString			strData=_T("");
	CString			strSQL=_T("");
	CString			strSQL2=_T("");
	CString			strSQL3=_T("");
	CRecordset		rs(m_pDB);

	// 金融機関、支店でない場合処理を行わない
	if(nCol != ID_COL_011_KINYU && nCol != ID_COL_011_KINYUSUB)	{
		return;
	}
	// 画面から名称を取得
	index = CmnGetControlIndex(nRow , nCol);
	cst = m_clsFunc.DiagGetString(m_pDiag, index);
	// 入力した金融機関名または支店名の文字色を変更する
	sign = CmnInitialControlFgStringCell(m_pDiag, nRow, nCol, m_ReadData[nRow].m_FgFunc, UC_BANK_LEN, cst);
	nCharOver = sign;
	if(nCharOver == 0)	{
		// 銀行名
		if(nCol == ID_COL_011_KINYU)	{
// 修正No.157628 del -->
			//strSQL.Format(_T("select max(oversgn) from (select CASE when len(Replace(isnull(BkName1,''),CHAR(13)+CHAR(10),'')) > %d then 1 else 0 end as oversgn "),UC_BANK_LEN);
// 修正No.157628 del <--
// 修正No.157628 add -->
			strSQL.Format(_T("select max(oversgn) from (select CASE when len(%s) > %d then 1 else 0 end as oversgn "),MakeReplaceSent(_T("BkName1")),UC_BANK_LEN);
// 修正No.157628 add <--
			//strSQL2.Format(_T("from uc_011_yotyokin where FgShow=%d AND NumPage <> %d AND NumRow <> %d) tbl1 "),ID_FGSHOW_OFF,m_ReadData[nRow].m_NumPage,nRow);
			strSQL2.Format(_T("from uc_011_yotyokin where FgShow=%d AND NOT(NumPage = %d AND NumRow = %d)) tbl1 "),ID_FGSHOW_OFF,m_ReadData[nRow].m_NumPage,nRow);
			strSQL3 = strSQL + strSQL2;
			rs.Open(CRecordset::forwardOnly, strSQL3);
			rs.GetFieldValue((short)0, strData);
			rs.Close();
			if(_tstoi(strData) != 0)	{
				nCharOver = 1;
			}
		}
		// 支店名
		else	{
// 修正No.157628 del -->
			//strSQL.Format(_T("select max(oversgn) from (select CASE when len(Replace(isnull(BkName2,''),CHAR(13)+CHAR(10),'')) > %d then 1 else 0 end as oversgn "),UC_BANK_LEN);
// 修正No.157628 del <--
// 修正No.157628 add -->
			strSQL.Format(_T("select max(oversgn) from (select CASE when len(%s) > %d then 1 else 0 end as oversgn "),MakeReplaceSent(_T("BkName2")),UC_BANK_LEN);
// 修正No.157628 add <--
			//strSQL2.Format(_T("from uc_011_yotyokin where FgShow=%d AND NumPage <> %d AND NumRow <> %d) tbl1 "),ID_FGSHOW_OFF,m_ReadData[nRow].m_NumPage,nRow);
			strSQL2.Format(_T("from uc_011_yotyokin where FgShow=%d AND NOT(NumPage = %d AND NumRow = %d)) tbl1 "),ID_FGSHOW_OFF,m_ReadData[nRow].m_NumPage,nRow);
			strSQL3 = strSQL + strSQL2;
			rs.Open(CRecordset::forwardOnly, strSQL3);
			rs.GetFieldValue((short)0, strData);
			rs.Close();
			if(_tstoi(strData) != 0)	{
				nCharOver = 1;
			}
		}
	}

// 修正No.157626 del -->
	//if(nCol == ID_COL_011_KINYU)	{
	//	CmnHeadDiagSetTitle(&m_HeadDiag, 0, _T("金 融 機 関 名(11)"), nCharOver);
	//}
	//else	{
	//	CmnHeadDiagSetTitle(&m_HeadDiag, 1, _T("支店名(11)"), nCharOver);
	//}
// 修正No.157626 del <--
// 修正No.157626 add -->
	if(nCol == ID_COL_011_KINYU)	{
		if(nCharOver == 1)	CmnHeadDiagSetTitle(&m_HeadDiag, 0, _T("金 融 機 関 名(11)"), 1);
		else				CmnHeadDiagSetTitle(&m_HeadDiag, 0, _T("金 融 機 関 名"), 0);
	}
	else	{
		if(nCharOver == 1)	CmnHeadDiagSetTitle(&m_HeadDiag, 1, _T("支店名(11)"), 1);
		else				CmnHeadDiagSetTitle(&m_HeadDiag, 1, _T("支店名"), 0);
	}

	// 欄外ガイドの表示のために他の項目が電子申告の文字数をオーバーしていないかどうか確認する
	// 金融機関名
	if(nCol == ID_COL_011_KINYUSUB)	{
		strSQL.Format(_T("select max(oversgn) from (select CASE when len(%s) > %d then 1 else 0 end as oversgn from uc_011_yotyokin where FgShow=%d) tbl1"),
			MakeReplaceSent(_T("BkName1")), UC_BANK_LEN, ID_FGSHOW_OFF);
		rs.Open(CRecordset::forwardOnly, strSQL);
		rs.GetFieldValue((short)0, strData);
		rs.Close();
		nCharOver2[0] = 0;
		if(_tstoi(strData) != 0) {
			nCharOver2[0] = 1;
		}
	}

	// 支店名
	if(nCol == ID_COL_011_KINYU) {
		strSQL.Format(_T("select max(oversgn) from (select CASE when len(%s) > %d then 1 else 0 end as oversgn from uc_011_yotyokin where FgShow=%d) tbl1"),
			MakeReplaceSent(_T("BkName2")), UC_BANK_LEN, ID_FGSHOW_OFF);
		rs.Open(CRecordset::forwardOnly, strSQL);
		rs.GetFieldValue((short)0, strData);
		rs.Close();
		nCharOver2[0] = 0;
		if(_tstoi(strData) != 0) {
			nCharOver2[0] = 1;
		}
	}

	// 種類
	strSQL.Format(_T("select max(oversgn) from (select CASE when len(%s) > %d then 1 else 0 end as oversgn from uc_011_yotyokin where FgShow=%d) tbl1"),
		MakeReplaceSent(_T("KnName")),UC_KAMOKU_LEN,ID_FGSHOW_OFF);
	rs.Open( CRecordset::forwardOnly, strSQL);
	rs.GetFieldValue((short)0, strData);
	rs.Close();
	nCharOver2[1] = 0;
	if(_tstoi(strData) != 0) {
		nCharOver2[1] = 1;
	}

	// 摘要
	strSQL.Format(_T("select max(oversgn) from (select CASE when len(%s) > %d then 1 else 0 end as oversgn from uc_011_yotyokin where FgShow=%d) tbl1"),
		MakeReplaceSent(_T("Teki")),UC_011_TEKI_LEN,ID_FGSHOW_OFF);
	rs.Open( CRecordset::forwardOnly, strSQL);
	rs.GetFieldValue((short)0, strData);
	rs.Close();
	nCharOver2[2] = 0;
	if(_tstoi(strData) != 0) {
		nCharOver2[2] = 1;
	}

	// 欄外ガイドの表示
	if(nCharOver == 1 || nCharOver2[0] == 1 || nCharOver2[1] == 1 || nCharOver2[2] == 1)	{
		m_Guide1.ShowWindow(TRUE);
	}
	else {
		m_Guide1.ShowWindow(FALSE);
	}
// 修正No.157626 add <--
}

// ------------------------------------------------------------------------------------------------------------------------
// 全頁の電子申告の桁数チェックを行い、タイトルの文字色を変更する
// ------------------------------------------------------------------------------------------------------------------------
void CfrmUc011YotyokinK::TitleColorChg(void)
{
	int					nCharOver[4]={0};	// 電子申告の文字数を超えたかどうかのサイン		1:電子申告の文字数を超えた
											// 様式①	[0]:金融機関名(11文字)	[1]:支店名(11文字)
											//			[2]:種類(10文字)		[3]:摘要(30文字)
	CRecordset			rs(m_pDB);
	CString				strSQL=_T("");
	CString				strData=_T("");

	// 金融機関名
// 修正No.157628 del -->
	//strSQL.Format(_T("select max(oversgn) from (select CASE when len(Replace(isnull(BkName1,''),CHAR(13)+CHAR(10),'')) > %d then 1 else 0 end as oversgn from uc_011_yotyokin where FgShow=%d) tbl1"),UC_BANK_LEN,ID_FGSHOW_OFF);
// 修正No.157628 del <--
// 修正No.157628 add -->
	strSQL.Format(_T("select max(oversgn) from (select CASE when len(%s) > %d then 1 else 0 end as oversgn from uc_011_yotyokin where FgShow=%d) tbl1"),
		MakeReplaceSent(_T("BkName1")),UC_BANK_LEN,ID_FGSHOW_OFF);
// 修正No.157628 add <--
	rs.Open( CRecordset::forwardOnly, strSQL);
	rs.GetFieldValue((short)0, strData);
	rs.Close();
	nCharOver[0] = 0;
	if(_tstoi(strData) != 0) {
		nCharOver[0] = 1;
	}

	// 支店名
// 修正No.157628 del -->
	//strSQL.Format(_T("select max(oversgn) from (select CASE when len(Replace(isnull(BkName2,''),CHAR(13)+CHAR(10),'')) > %d then 1 else 0 end as oversgn from uc_011_yotyokin where FgShow=%d) tbl1"),UC_BANK_LEN,ID_FGSHOW_OFF);
// 修正No.157628 del <--
// 修正No.157628 add -->
	strSQL.Format(_T("select max(oversgn) from (select CASE when len(%s) > %d then 1 else 0 end as oversgn from uc_011_yotyokin where FgShow=%d) tbl1"),
		MakeReplaceSent(_T("BkName2")),UC_BANK_LEN,ID_FGSHOW_OFF);
// 修正No.157628 add <--
	rs.Open( CRecordset::forwardOnly, strSQL);
	rs.GetFieldValue((short)0, strData);
	rs.Close();
	nCharOver[1] = 0;
	if(_tstoi(strData) != 0) {
		nCharOver[1] = 1;
	}

	// 種類
// 修正No.157628 del -->
	//strSQL.Format(_T("select max(oversgn) from (select CASE when len(Replace(isnull(KnName,''),CHAR(13)+CHAR(10),'')) > %d then 1 else 0 end as oversgn from uc_011_yotyokin where FgShow=%d) tbl1"),UC_KAMOKU_LEN,ID_FGSHOW_OFF);
// 修正No.157628 del <--
// 修正No.157628 add -->
	strSQL.Format(_T("select max(oversgn) from (select CASE when len(%s) > %d then 1 else 0 end as oversgn from uc_011_yotyokin where FgShow=%d) tbl1"),
		MakeReplaceSent(_T("KnName")),UC_KAMOKU_LEN,ID_FGSHOW_OFF);
// 修正No.157628 add <--
	rs.Open( CRecordset::forwardOnly, strSQL);
	rs.GetFieldValue((short)0, strData);
	rs.Close();
	nCharOver[2] = 0;
	if(_tstoi(strData) != 0) {
		nCharOver[2] = 1;
	}

	// 摘要
// 修正No.157628 del -->
	//strSQL.Format(_T("select max(oversgn) from (select CASE when len(Replace(isnull(Teki,''),CHAR(13)+CHAR(10),'')) > %d then 1 else 0 end as oversgn from uc_011_yotyokin where FgShow=%d) tbl1"),UC_011_TEKI_LEN,ID_FGSHOW_OFF);
// 修正No.157628 del <--
// 修正No.157628 add -->
	strSQL.Format(_T("select max(oversgn) from (select CASE when len(%s) > %d then 1 else 0 end as oversgn from uc_011_yotyokin where FgShow=%d) tbl1"),
		MakeReplaceSent(_T("Teki")),UC_011_TEKI_LEN,ID_FGSHOW_OFF);
// 修正No.157628 add <--
	rs.Open( CRecordset::forwardOnly, strSQL);
	rs.GetFieldValue((short)0, strData);
	rs.Close();
	nCharOver[3] = 0;
	if(_tstoi(strData) != 0) {
		nCharOver[3] = 1;
	}

	// 項目タイトル(ヘッダ部)の再設定
// 修正No.157626 del -->
	//CmnHeadDiagSetTitle(&m_HeadDiag,0,_T("金 融 機 関 名(11)"),nCharOver[0]);
	//CmnHeadDiagSetTitle(&m_HeadDiag,1,_T("支店名(11)"),nCharOver[1]);
	//CmnHeadDiagSetTitle(&m_HeadDiag,2,_T("種  類(10)"),nCharOver[2]);
	//CmnHeadDiagSetTitle(&m_HeadDiag,5,_T("摘     要(30)"),nCharOver[3]);
// 修正No.157626 del <--
// 修正No.157626 add -->
	if(nCharOver[0] == 1)	CmnHeadDiagSetTitle(&m_HeadDiag,0,_T("金 融 機 関 名(11)"),1);
	else					CmnHeadDiagSetTitle(&m_HeadDiag,0,_T("金 融 機 関 名"),0);

	if(nCharOver[1] == 1)	CmnHeadDiagSetTitle(&m_HeadDiag,1,_T("支店名(11)"),1);
	else					CmnHeadDiagSetTitle(&m_HeadDiag,1,_T("支店名"),0);
	
	if(nCharOver[2] == 1)	CmnHeadDiagSetTitle(&m_HeadDiag,2,_T("種  類(10)"),1);
	else					CmnHeadDiagSetTitle(&m_HeadDiag,2,_T("種  類"),0);

	if(nCharOver[3] == 1)	CmnHeadDiagSetTitle(&m_HeadDiag,5,_T("摘       要(30)"),1);
	else					CmnHeadDiagSetTitle(&m_HeadDiag,5,_T("摘          要"),0);

	// 欄外ガイドの表示
	if(nCharOver[0] == 1 || nCharOver[1] == 1 || nCharOver[2] == 1 || nCharOver[3] == 1)	{
		m_Guide1.ShowWindow(TRUE);
	}
	else {
		m_Guide1.ShowWindow(FALSE);
	}
// 修正No.157626 add <--
}
// No.200601 add <--

// 修正No.162446 add -->
LRESULT CfrmUc011YotyokinK::OnUserReDrawView(WPARAM wParam, LPARAM lParam)
{
	virUpdateControlTblData();
	CmnDiagSetFocus(m_pDiag, 1, 1);

	return(0);
}
// 修正No.162446 add <--
