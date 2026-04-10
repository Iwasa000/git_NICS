// インボイス登録番号追加対応_23/09/11 add -->
//--------------------------------------------------
//	frmUc071KoteisisanI.cpp
//--------------------------------------------------

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "frmUc071Koteisisan.h"

extern BOOL	bG_Kanso;

IMPLEMENT_DYNCREATE( CfrmUc071KoteisisanI, CfrmUc000Common )


BEGIN_MESSAGE_MAP( CfrmUc071KoteisisanI, CfrmUc000Common )
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
	ON_COMMAND( ID_PRINTSETUP_MENU, &CfrmUc071KoteisisanI::OnMenuPrintSetup )
	ON_COMMAND( ID_PRINTALL_MENU, &CfrmUc071KoteisisanI::OnMenuPrintAll )
	ON_COMMAND( ID_SPECIALROW_MENU, &CfrmUc071KoteisisanI::OnMenuSpecialRow )
	ON_COMMAND( ID_SORT_MENU, &CfrmUc071KoteisisanI::OnMenuSort )
	ON_COMMAND( ID_ZEROMONEY_MENU, &CfrmUc071KoteisisanI::OnMenuZeroMoney )
	ON_COMMAND( ID_ZEROMONEYMOVE_MENU, &CfrmUc071KoteisisanI::OnMenuZeroMoneyMove )
	ON_COMMAND( ID_AMOUTDIALOG_MENU, &CfrmUc071KoteisisanI::OnMenuOutKeiSetup )
	ON_COMMAND( ID_CLEAR_MENU, &CfrmUc071KoteisisanI::OnMenuClear )
	ON_COMMAND( ID_TITLEMENU, &CfrmUc071KoteisisanI::OnMenuTitleRestore )
	ON_COMMAND( ID_USER_SETTING_MENU, &CfrmUc071KoteisisanI::OnMenuOptionSetting )
	ON_COMMAND( ID_COPY_SETTING_INFO_MENU, &CfrmUc071KoteisisanI::OnMenuCopySettingInfo )
	ON_COMMAND( ID_DELETE_DATA_MENU, &CfrmUc071KoteisisanI::OnMenuDeleteDataEachYoushiki )
	ON_BN_CLICKED( IDC_PAGEBACK_BUTTON1, &CfrmUc071KoteisisanI::OnPageBack )
	ON_BN_CLICKED( IDC_PAGENEXT_BUTTON1, &CfrmUc071KoteisisanI::OnPageNext )
	ON_BN_CLICKED( IDC_PAGENEW_BUTTON1, &CfrmUc071KoteisisanI::OnPageNew )
	ON_BN_CLICKED( IDC_PAGEINSERT_BUTTON1, &CfrmUc071KoteisisanI::OnPageInsert )
	ON_BN_CLICKED( IDC_ROWCOPY_BUTTON1, &CfrmUc071KoteisisanI::OnRowCopy )
	ON_BN_CLICKED( IDC_ROWPASTE_BUTTON1, &CfrmUc071KoteisisanI::OnRowPaste )
	ON_BN_CLICKED( IDC_ROWINSERT_BUTTON1, &CfrmUc071KoteisisanI::OnRowInsert )
	ON_BN_CLICKED( IDC_YOUSHIKINEXT_BUTTON1, &CfrmUc071KoteisisanI::OnMoveYoushikiNext )
	ON_BN_CLICKED( IDC_YOUSHIKIBACK_BUTTON1, &CfrmUc071KoteisisanI::OnMoveYoushikiBack )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_F6, &CfrmUc071KoteisisanI::OnUpdateButtonF6 )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_F7, &CfrmUc071KoteisisanI::OnUpdateButtonF7 )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_F8, &CfrmUc071KoteisisanI::OnUpdateButtonF8 )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_F9, &CfrmUc071KoteisisanI::OnUpdateButtonF9 )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_F11, &CfrmUc071KoteisisanI::OnUpdateButtonF11 )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_F12, &CfrmUc071KoteisisanI::OnUpdateButtonF12 )
	ON_UPDATE_COMMAND_UI( ID_BUTTON_HOME, &CfrmUc071KoteisisanI::OnUpdateButtonHome )
	ON_UPDATE_COMMAND_UI( ID_SORT_MENU, &CfrmUc071KoteisisanI::OnUpdateMenuSort )
	ON_UPDATE_COMMAND_UI( ID_CLEAR_MENU, &CfrmUc071KoteisisanI::OnUpdateMenuClear )
	ON_UPDATE_COMMAND_UI( ID_ZEROMONEY_MENU, &CfrmUc071KoteisisanI::OnUpdateMenuZeroMoney )
	ON_UPDATE_COMMAND_UI( ID_ZEROMONEYMOVE_MENU, &CfrmUc071KoteisisanI::OnUpdateMenuZeroMoneyMove)
	ON_UPDATE_COMMAND_UI(ID_SPECIALROW_MENU, &CfrmUc071KoteisisanI::OnUpdateMenuSpecialRow)
	ON_UPDATE_COMMAND_UI( ID_TITLEMENU, &CfrmUc071KoteisisanI::OnUpdateMenuTitleRestore )
	ON_UPDATE_COMMAND_UI( ID_DELETE_DATA_MENU, &CfrmUc071KoteisisanI::OnUpdateMenuDeleteDataEachYoushiki )
	ON_MESSAGE(WM_USER_REDRAWVIEW, OnUserReDrawView)
END_MESSAGE_MAP()


CfrmUc071KoteisisanI::CfrmUc071KoteisisanI()
	: CfrmUc000Common( CfrmUc071KoteisisanI::IDD )
{
	m_pDB = ( (CUCHIWAKEApp*)AfxGetApp() )->m_pDB;
	m_pTitleDiag = &m_TitleDiag;
	m_pPageDiag	= &m_PageDiag;
	m_pHeadDiag = &m_HeadDiag;		// 修正No.168481,168502,168517 add
	m_pDiagKei2 = &m_DiagKei2;		// 修正No.168481,168502,168517 add
}

CfrmUc071KoteisisanI::~CfrmUc071KoteisisanI()
{
	// 2009.09.08 TS自動ログオフ対応(virEndProcへ移動)
	TRACE(_T("***** CfrmUc071KoteisisanI::~CfrmUc071KoteisisanI\n"));
}

void CfrmUc071KoteisisanI::DoDataExchange( CDataExchange* pDX )
{
	CfrmUc000Common::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_YOUSHIKIDIAG1, m_Diag1 );
	DDX_Control( pDX, IDC_YOUSHIKIDIAG2, m_Diag2 );
	DDX_Control( pDX, IDC_YOUSHIKIDIAG3, m_Diag3 );
	DDX_Control( pDX, IDC_YOUSHIKIDIAG_KEI1, m_DiagKei1 );
	DDX_Control( pDX, IDC_YOUSHIKIDIAG_KEI2, m_DiagKei2 );
	DDX_Control( pDX, IDC_TITLEDIAG1, m_TitleDiag );
	DDX_Control( pDX, IDC_PAGEDIAG1, m_PageDiag );
	DDX_Control( pDX, IDC_NUMBER_DIAG1, m_NumberDiag );
	DDX_Control( pDX, IDC_TOPICDIAG1, m_HeadDiag);
	DDX_Control(pDX, IDC_SAVEDATA_DIAG1, m_SaveDataDiag);
	DDX_Control(pDX, IDC_ICSDISPGUIDE1, m_Guide1);
}

void CfrmUc071KoteisisanI::OnInitialUpdate()
{
	CfrmUc000Common::OnInitialUpdate();
	ResizeParentToFit();

	// 帳表テーブルのオープンOK?
	if(m_InitialFlag == TRUE)	{
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
void CfrmUc071KoteisisanI::AssertValid() const
{
	CfrmUc000Common::AssertValid();
}

#ifndef _WIN32_WCE
void CfrmUc071KoteisisanI::Dump( CDumpContext& dc ) const
{
	CfrmUc000Common::Dump( dc );
}
#endif
#endif //_DEBUG

BEGIN_EVENTSINK_MAP( CfrmUc071KoteisisanI, CfrmUc000Common )
	ON_EVENT( CfrmUc071KoteisisanI, IDC_YOUSHIKIDIAG1, 3, CfrmUc071KoteisisanI::TerminationYoushikidiag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN )
	ON_EVENT( CfrmUc071KoteisisanI, IDC_YOUSHIKIDIAG1, 2, CfrmUc071KoteisisanI::EditOFFYoushikidiag1, VTS_I2 )
	ON_EVENT( CfrmUc071KoteisisanI, IDC_YOUSHIKIDIAG1, 1, CfrmUc071KoteisisanI::EditONYoushikidiag1, VTS_I2 )
	ON_EVENT( CfrmUc071KoteisisanI, IDC_YOUSHIKIDIAG1, 12, CfrmUc071KoteisisanI::VScrollYoushikidiag1, VTS_I2)

	ON_EVENT( CfrmUc071KoteisisanI, IDC_YOUSHIKIDIAG2, 3, CfrmUc071KoteisisanI::TerminationYoushikidiag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN )
	ON_EVENT( CfrmUc071KoteisisanI, IDC_YOUSHIKIDIAG2, 2, CfrmUc071KoteisisanI::EditOFFYoushikidiag1, VTS_I2 )
	ON_EVENT( CfrmUc071KoteisisanI, IDC_YOUSHIKIDIAG2, 1, CfrmUc071KoteisisanI::EditONYoushikidiag1, VTS_I2 )
	ON_EVENT( CfrmUc071KoteisisanI, IDC_YOUSHIKIDIAG2, 12, CfrmUc071KoteisisanI::VScrollYoushikidiag1, VTS_I2)

	ON_EVENT( CfrmUc071KoteisisanI, IDC_YOUSHIKIDIAG3, 3, CfrmUc071KoteisisanI::TerminationYoushikidiag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN )
	ON_EVENT( CfrmUc071KoteisisanI, IDC_YOUSHIKIDIAG3, 2, CfrmUc071KoteisisanI::EditOFFYoushikidiag1, VTS_I2 )
	ON_EVENT( CfrmUc071KoteisisanI, IDC_YOUSHIKIDIAG3, 1, CfrmUc071KoteisisanI::EditONYoushikidiag1, VTS_I2 )
	ON_EVENT( CfrmUc071KoteisisanI, IDC_YOUSHIKIDIAG3, 12, CfrmUc071KoteisisanI::VScrollYoushikidiag1, VTS_I2)

	ON_EVENT( CfrmUc071KoteisisanI, IDC_TITLEDIAG1, 3, CfrmUc071KoteisisanI::TerminationTitlediag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN )
	ON_EVENT( CfrmUc071KoteisisanI, IDC_TITLEDIAG1, 2, CfrmUc071KoteisisanI::EditOFFTitlediag1, VTS_I2 )
	ON_EVENT( CfrmUc071KoteisisanI, IDC_TITLEDIAG1, 1, CfrmUc071KoteisisanI::EditONTitlediag1, VTS_I2 )
	ON_EVENT( CfrmUc071KoteisisanI, IDC_TITLEDIAG1, 6, CfrmUc071KoteisisanI::FocusTitlediag1, VTS_NONE )

	ON_EVENT( CfrmUc071KoteisisanI, IDC_PAGEDIAG1, 3, CfrmUc071KoteisisanI::TerminationPagediag1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN )
	ON_EVENT( CfrmUc071KoteisisanI, IDC_PAGEDIAG1, 2, CfrmUc071KoteisisanI::EditOFFPagediag1, VTS_I2 )
	ON_EVENT( CfrmUc071KoteisisanI, IDC_PAGEDIAG1, 1, CfrmUc071KoteisisanI::EditONPagediag1, VTS_I2 )
	ON_EVENT( CfrmUc071KoteisisanI, IDC_PAGEDIAG1, 6, CfrmUc071KoteisisanI::FocusPagediag1, VTS_NONE )
END_EVENTSINK_MAP()

//********************************************************************************
//	初期処理
//********************************************************************************
BOOL	CfrmUc071KoteisisanI::virStartJob()
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
void	CfrmUc071KoteisisanI::virSetInitialUchiwakeInfo()
{
	// 「固定資産」の固有データ
	m_uInfo.intOutKeiDialog = AM_TYPE_ALL;					// 計設定ダイアログのボタン指定
	m_uInfo.intRowMax = ID_ROWKOBETU_071;					// １頁内の合計行を含む全体の行数
	m_uInfo.intRowMaxData = ID_ROWKOBETU_071 - 1;			// １頁内のデータ行数（合計行は含まず）
	m_uInfo.intColMax = ID_COLMAX_071I;						// １行内のコントロール個数
	m_uInfo.intColMaxData = ID_COLMAXDATA_071I;				// １行内の列数
	m_uInfo.intCtlStartIndex = ID_CTLSTARTINDEX_071I;		// １行目の入力項目の先頭インデックス番号
	m_uInfo.intT1StartIndex = ID_T1STARTNORMAL_071I;		// 合計行①の先頭インデックス番号
	m_uInfo.intT2StartIndex = ID_T2STARTNORMAL_071I;		// 合計行②の入力項目の先頭インデックス番号
	m_uInfo.intCursolDefPos = ID_COL_071I_SYURUI;			// カーソル移動時の初期位置（タイトルから↓，合計から↑で使用）
	m_uInfo.intCursolKeiPos = ID_COL_071I_SYURUI;			// 合計行のカーソル移動位置（カーソル移動で使用）

	m_uInfo.intFormSeq = ID_FORMNO_071;						// 様式シーケンス番号
	m_uInfo.intFormNo = 7;									// 様式番号（uc_inf_sub）
	m_uInfo.intFormNo2 = 1;									// 様式番号枝番（uc_inf_sub）

	m_uInfo.intRowNormal = ID_ROWNORMAL_071;				// 通常時：１頁内の合計行を含む全体の行数
	m_uInfo.intRowKobetu = ID_ROWKOBETU_071;				// 個別時：１頁内の合計行を含む全体の行数
	m_uInfo.intT1StartNormal = ID_T1STARTNORMAL_071I;		// 通常時：合計行①の先頭インデックス
	m_uInfo.intT2StartNormal = ID_T2STARTNORMAL_071I;		// 通常時：合計行②の先頭インデックス
	m_uInfo.intT1StartKobetu = ID_T1STARTKOBETU_071I;		// 個別時：合計行①の先頭インデックス
	m_uInfo.intT2StartKobetu = ID_T2STARTKOBETU_071I;		// 個別時：合計行②の先頭インデックス
	m_uInfo.intRowKei = ID_ROWKEI_071;						// 帳表固定の合計行数（1:合計あり，0:なし）
	m_uInfo.intKei1StartIndex = ID_COL_071I_KEI1_MEISYO;	// 計ダイアグラム1行目の先頭インデックス
	m_uInfo.intKei2StartIndex = ID_COL_071I_KEI2_MEISYO;	// 計ダイアグラム2行目の先頭インデックス

	// 集計処理の設定
	m_uInfo.uCalcInfo.intMaxCount = ID_CALC_071_MAX;		// 集計フィールド件数
	m_uInfo.uCalcInfo.strField[0] = ID_CALC_071_FIELD1;		// 集計フィールド名称1
	m_uInfo.uCalcInfo.strField[1] = ID_CALC_071_FIELD2;		// 集計フィールド名称2
	m_uInfo.uCalcInfo.strField[2] = ID_CALC_071_FIELD3;		// 集計フィールド名称3
}

//**************************************************
// ICSDiagコントロールを通常／個別で切り替える
// 【引数】    なし
// 【戻値】    なし
//**************************************************
void	CfrmUc071KoteisisanI::virSetInitialDiag()
{

	// オブジェクト未生成時は処理を抜ける
	if( (m_Diag1.m_hWnd == NULL) || (m_Diag2.m_hWnd == NULL) || (m_Diag3.m_hWnd == NULL))	{
		return;
	}

	//表示するダイアログの設定
	m_Diag		= CmnGetDiagType( &m_Diag1, &m_Diag2, &m_Diag3 );
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
void	CfrmUc071KoteisisanI::virSetInitialValue()
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
BOOL	CfrmUc071KoteisisanI::InitialControlDataArea()
{
	int		intMax;
	int		i;
	int		len=0;
	DIAGRAM_ATTRIBUTE diaatt;

	// 全データクリア
	m_pDiag->AllClear();
	m_pDiagKei->AllClear();

	// 各Diagに背景色を設定
	SetDiagBackColor();

	// 背景色クリア
	CmnDiagSetAttrColorAllClear( m_pDiag );

	// 列の属性により更にクリアと初期設定が必要
	intMax = m_uInfo.intT1StartIndex;

	// コントロールプロパティ設定
	for( i = m_uInfo.intCtlStartIndex; i < intMax; i += m_uInfo.intColMax ) {
		// ComboBox：クリア
		CmnDiagSetString(m_pDiag, i + ID_COL_071I_INVONOT - 1, "", 0);	// 登録番号(T選択)

		// 異動年月日
		m_pDiag->GetAttribute((i + ID_COL_071I_SPDATE - 1),(LPUNKNOWN)&diaatt);
		diaatt.attr_dayattr = 15;
		m_pDiag->SetAttribute((i + ID_COL_071I_SPDATE - 1),(LPUNKNOWN)&diaatt,TRUE);

		// 漢字入力欄の初期化
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_071I_SYURUI  - 1, MAX_KETA_071_SYURUI );	// 種類・構造
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_071I_YOUTO   - 1, MAX_KETA_071_YOUTO );	// 用途
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_071I_AD2ADD1 - 1, MAX_KETA_071_AD2ADD1 );	// 物件の所在地1
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_071I_AD2ADD2 - 1, MAX_KETA_071_AD2ADD2 );	// 物件の所在地2
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_071I_SPDATE  - 1, MAX_KETA_071_SPDATE );	// 異動年月日
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_071I_RIYUU   - 1, MAX_KETA_071_RIYUU );	// 異動事由
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_071I_ADNAME1 - 1, MAX_KETA_071_ADNAME1 );	// 売却先の名称1
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_071I_ADADD1  - 1, MAX_KETA_071_ADADD1 );	// 売却先の所在地1
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_071I_ADADD2  - 1, MAX_KETA_071_ADADD2 );	// 売却先の所在地2
		CmnDiagSetAttrInpmax( m_pDiag, i + ID_COL_071I_SPDATE2 - 1, MAX_KETA_071_SPDATE2 );
	
		// 金額欄：桁数初期化
		len = ID_VAL_071_K_DATA;
		CmnDiagSetAttrDigit( m_pDiag, i + ID_COL_071I_VAL    - 1, len );					// 期末現在高

		len = ID_MSVAL_071_K_DATA;
		CmnDiagSetAttrDigit( m_pDiag, i + ID_COL_071I_MSVAL  - 1, len );					// 所得(処分)価額

		len = ID_MSVAL2_071_K_DATA;
		CmnDiagSetAttrDigit( m_pDiag, i + ID_COL_071I_MSVAL2 - 1, len );					// 異動直前の帳簿価額

		// 小数点付き数値の設定
		len = 8;
		CmnDiagSetAttrDigit( m_pDiag, i + ID_COL_071I_AREA - 1, len, TRUE, TRUE, 2 );	// 面積

		//	入力不可モードの解除
		SetInputMode( CmnGetControlRow( i ), DIAG_MDFY_INPUT );
	}

	// ComboBoxの初期設定
	if(InitialControlComboBox() != TRUE) {
		return FALSE;
	}

	// 半角/全角設定
	CmnDiagSetAttrColZenHan( m_pDiag , ID_COL_071I_SYURUI );	// 種類・構造
	CmnDiagSetAttrColZenHan( m_pDiag , ID_COL_071I_YOUTO );		// 用途
	CmnDiagSetAttrColZenHan( m_pDiag , ID_COL_071I_AD2ADD1 );	// 物件の所在地1
	CmnDiagSetAttrColZenHan( m_pDiag , ID_COL_071I_AD2ADD2 );	// 物件の所在地2
	CmnDiagSetAttrColZenHan( m_pDiag , ID_COL_071I_RIYUU );		// 異動事由
	CmnDiagSetAttrColZenHan( m_pDiag , ID_COL_071I_ADNAME1 );	// 売却(購入)先の名称1(氏名)
	CmnDiagSetAttrColZenHan( m_pDiag , ID_COL_071I_ADADD1 );	// 売却(購入)先の所在地1(住所)
	CmnDiagSetAttrColZenHan( m_pDiag , ID_COL_071I_ADADD2 );	// 売却(購入)先の所在地2(住所)

	return TRUE;
}

//********************************************************************************
//	帳表コントロールの初期化（ComboBox）
//********************************************************************************
BOOL	CfrmUc071KoteisisanI::InitialControlComboBox()
{
	DIAGRAM_DATA	diadata;		// ICSDIAG構造体
	int				intTotalType;
	int				intMax;
	int				intIndex;
	int				i;
	BOOL			bFlg = FALSE;				// フォントサイズ変更する？

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
	m_clsFunc.DiagInit(&diadata);

	// ComboBoxの選択項目設定
	diadata.text_combo = _T("T\r\n \r\n");

	// 全行設定
	for(i = 1; i <= intMax; i++)	{
		// 対象コントロールのインデックス番号を取得
		intIndex = CmnGetControlIndex(i, ID_COL_071I_INVONOT);

		// 初期設定（リスト情報をセット）
		m_pDiag->SetData(intIndex, (LPUNKNOWN)&diadata);

		// フォントサイズ設定
		if(m_EndView == FALSE && m_ChangeFont != FALSE && m_ImportMode == FALSE) {
			// T選択
			m_pDiag->ChangeFontSize(intIndex, m_TselFSize);
		}
	}
	m_ChangeFont = FALSE;

	return TRUE;
}

//********************************************************************************
//	帳表コントロールの初期化（頁計/累計領域）
//		文言はデータに含まれているので、その内容を使用する
//********************************************************************************
void	CfrmUc071KoteisisanI::InitialControlTotalArea()
{
	int		nKeiType;			//	計の種別
	int		nDataRow[2];		//	データ行属性
	int		nDataRowPos[2];		//	データ行位置
	int		nKeiRow[2];			//	計行属性
	int		nCnt;				//	カウント
	int		nStart;				//	開始位置
	int		ii=0;
	int		intIndex=0;
	int		nSw=0;

	intIndex = CmnGetControlIndex(m_uInfo.intCurRow,m_uInfo.intCurCol);

	// カレント頁の合計行の種別を取得
	nKeiType = CmnGetControlTotalType();
	
	nDataRowPos[0] = m_uInfo.intRowMax - 2;
	nDataRowPos[1] = m_uInfo.intRowMax - 1;

	//	計の種別で分岐
	switch ( nKeiType ){
		case ID_OUTKEI_OFF:				// なし
			//	データ行の設定
			if(m_DataKakutei == FALSE)	{
				for(ii=0; ii<2; ii++)	{
					if(IsSpecialRow(m_ReadData[nDataRowPos[ii]+1].m_FgFunc) != 0)	{
						nDataRow[ii] = ( DIAG_MDFY_NOINPUT	| DIAG_MDFY_OPAQUE );
					}
					else	{
						nDataRow[ii] = ( DIAG_MDFY_EDIT	| DIAG_MDFY_OPAQUE );
					}
				}
			}
			else	{
				nDataRow[0] = ( DIAG_MDFY_READONLY	| DIAG_MDFY_OPAQUE );
				nDataRow[1] = ( DIAG_MDFY_READONLY	| DIAG_MDFY_OPAQUE );
			}
			//	計行の設定
			nKeiRow[0] = ( DIAG_MDFY_READONLY | DIAG_MDFY_TRANSPARENT );
			nKeiRow[1] = ( DIAG_MDFY_READONLY | DIAG_MDFY_TRANSPARENT );

			// 28行目の合計行①
			nStart = m_uInfo.intT1StartIndex;
			for ( nCnt = 0; nCnt < m_uInfo.intColMax; nCnt++ ){
				m_pDiag->ModifyItem( nStart + nCnt, nKeiRow[0] );
			}
			// 29行目の合計行②
			nStart = m_uInfo.intT2StartIndex;
			for(nCnt = 0; nCnt < m_uInfo.intColMax; nCnt++) {
				m_pDiag->ModifyItem( nStart + nCnt, nKeiRow[1] );
			}
			break;

		case ID_OUTKEI_PAGEKEI:			// 頁計
		case ID_OUTKEI_RUIKEI:			// 累計
			//	データ行の設定
			if(m_DataKakutei == FALSE)	{
				if(IsSpecialRow(m_ReadData[nDataRowPos[0]+1].m_FgFunc) != 0)	{
					nDataRow[0] = ( DIAG_MDFY_NOINPUT	| DIAG_MDFY_OPAQUE );
				}
				else	{
					nDataRow[0] = ( DIAG_MDFY_EDIT	| DIAG_MDFY_OPAQUE );
				}
			}
			else	{
				nDataRow[0] = ( DIAG_MDFY_READONLY	| DIAG_MDFY_OPAQUE );
			}
			nDataRow[1] = ( DIAG_MDFY_READONLY | DIAG_MDFY_TRANSPARENT );
			//	計行の設定
			nKeiRow[0] = ( DIAG_MDFY_READONLY | DIAG_MDFY_TRANSPARENT );
			nKeiRow[1] = ( DIAG_MDFY_READONLY | DIAG_MDFY_OPAQUE );

			// 28行目の合計行①
			nStart = m_uInfo.intT1StartIndex;
			for ( nCnt = 0; nCnt < m_uInfo.intColMax; nCnt++ ){
				m_pDiag->ModifyItem( nStart + nCnt, nKeiRow[0] );
			}
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

	int	col=0,per=0;

	// 28行目のデータ行
	nStart = nDataRowPos[0] * m_uInfo.intColMax;
	for ( nCnt = 0; nCnt < m_uInfo.intColMax; nCnt++){
		if((nDataRow[0] & DIAG_MDFY_READONLY) == 0)	{
			// 28行目が入力可能な場合
			per = (nCnt % m_uInfo.intColMax);
			col = per - m_uInfo.intCtlStartIndex + 1;
			if(col == ID_COL_071I_SPDATE || col == ID_COL_071I_SPDATE2)	{
				// 日付ならMODIFY_GENGODEFをONにする
				nDataRow[0] = nDataRow[0] | MODIFY_GENGODEF;
			}
		}
		m_pDiag->ModifyItem( nStart + nCnt, nDataRow[0] );

		if((intIndex == (nStart + nCnt)) && ((nDataRow[0] & DIAG_MDFY_READONLY) != 0))	nSw=1;
	}
	// 29行目のデータ行
	nStart = nDataRowPos[1] * m_uInfo.intColMax;
	for ( nCnt = 0; nCnt < m_uInfo.intColMax; nCnt++){
		if((nDataRow[1] & DIAG_MDFY_READONLY) == 0)	{
			// 29行目が入力可能な場合
			per = (nCnt % m_uInfo.intColMax);
			col = per - m_uInfo.intCtlStartIndex + 1;
			if(col == ID_COL_071I_SPDATE || col == ID_COL_071I_SPDATE2)	{
				// 日付ならMODIFY_GENGODEFをONにする
				nDataRow[1] = nDataRow[1] | MODIFY_GENGODEF;
			}
		}
		m_pDiag->ModifyItem( nStart + nCnt, nDataRow[1] );
		if((intIndex == (nStart + nCnt)) && ((nDataRow[1] & DIAG_MDFY_READONLY) != 0))	nSw=1;
	}
	if(nSw != 0)	{
		// 現在のカーソルポジションが入力不可になった場合は先頭に戻す
		m_uInfo.intCurRow = 1;
		m_uInfo.intCurCol = 1;
	}

	// 計専用ダイアグラムの金額欄を入力不可にする
	int intKeiRow = (DIAG_MDFY_READONLY | DIAG_MDFY_OPAQUE);
	m_pDiagKei->ModifyItem(ID_COL_071I_KEI1_VAL, intKeiRow);
	m_pDiagKei->ModifyItem(ID_COL_071I_KEI2_VAL, intKeiRow);
	m_pDiagKei->ModifyItem(ID_COL_071I_KEI1_MSVAL, intKeiRow);
	m_pDiagKei->ModifyItem(ID_COL_071I_KEI2_MSVAL, intKeiRow);
	m_pDiagKei->ModifyItem(ID_COL_071I_KEI1_MSVAL2, intKeiRow);
	m_pDiagKei->ModifyItem(ID_COL_071I_KEI2_MSVAL2, intKeiRow);
}

//********************************************************************************
//	帳表コントロールの特殊行初期化
//	IN	int		行番号
//		int		特殊行フラグ（FgFunc）
//	RET	なし
//********************************************************************************
void	CfrmUc071KoteisisanI::InitialControlSpecialRow( int intRow, int intFgFunc )
{
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
			// 桁数を設定
			CmnDiagSetAttrDigit( m_pDiag, CmnGetControlIndex( intRow , ID_COL_071I_VAL ), ID_VAL_071_TOTAL );
			CmnDiagSetAttrDigit( m_pDiag, CmnGetControlIndex( intRow , ID_COL_071I_MSVAL ), ID_MSVAL_071_TOTAL );
			CmnDiagSetAttrDigit( m_pDiag, CmnGetControlIndex( intRow , ID_COL_071I_MSVAL2 ), ID_MSVAL2_071_TOTAL );
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
				// 「登録番号(T選択)」はComboBoxのため、DropDown不可とする
				m_pDiag->ModifyItem(CmnGetControlIndex(intRow, ID_COL_071I_INVONOT), DIAG_MDFY_NODROPDOWN);

				// ComboBoxの選択項目クリア
				m_clsFunc.DiagInit(&diadata);
				diadata.text_combo = _T("\r\n");
				m_pDiag->SetData(CmnGetControlIndex(intRow, ID_COL_071I_INVONOT), (LPUNKNOWN)&diadata);
			}

			//	入力不可モードの設定
			if(m_DataKakutei == FALSE)	SetInputMode(intRow,DIAG_MDFY_NOINPUT);
			else						SetInputMode(intRow,DIAG_MDFY_READONLY);
			break;

		//	特殊行以外
		default:
			break;
	}
}

//********************************************************************************
//	帳表タイトル情報の表示
//********************************************************************************
void	CfrmUc071KoteisisanI::virUpdateControlTitle()
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
void	CfrmUc071KoteisisanI::UpdateControlPage()
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
BOOL	CfrmUc071KoteisisanI::virUpdateControlTblData()
{
	// ダイアログ切替
	if( m_pDiag != NULL ){
		virSetInitialDiag();
	}

	CdbUc071Koteishisan	dbRec( m_pDB );

	m_pDiag->DeleteInput();		//	入力ボックス削除 

	// 頁情報の表示
	UpdateControlPage();

	// コントロールのクリア＋初期設定
	if ( InitialControlDataArea() != TRUE ){	// データ行
		return FALSE;
	}

	// 頁計、累計行の桁数をセットする
	CmnDiagSetAttrDigit( m_pDiagKei, ID_COL_071I_KEI1_VAL, ID_VAL_071_TOTAL );
	CmnDiagSetAttrDigit( m_pDiagKei, ID_COL_071I_KEI2_VAL, ID_VAL_071_TOTAL );
	CmnDiagSetAttrDigit( m_pDiagKei, ID_COL_071I_KEI1_MSVAL, ID_MSVAL_071_TOTAL );
	CmnDiagSetAttrDigit( m_pDiagKei, ID_COL_071I_KEI2_MSVAL, ID_MSVAL_071_TOTAL );
	CmnDiagSetAttrDigit( m_pDiagKei, ID_COL_071I_KEI1_MSVAL2, ID_MSVAL2_071_TOTAL );
	CmnDiagSetAttrDigit( m_pDiagKei, ID_COL_071I_KEI2_MSVAL2, ID_MSVAL2_071_TOTAL );

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
		else {
			// データ行の表示（空行，特殊行を含む）
			UpdateControlRowData( &dbRec );
		}

		// 電子申告の文字数を超えていれば、項目の文字色を変える
		// 種類・構造
		CmnInitialControlFgStringCell( m_pDiag, dbRec.m_NumRow, ID_COL_071I_SYURUI, dbRec.m_FgFunc, UC_KAMOKU_LEN, dbRec.m_Syurui);
		// 用途
		CmnInitialControlFgStringCell( m_pDiag, dbRec.m_NumRow, ID_COL_071I_YOUTO, dbRec.m_FgFunc, UC_071_YOUTO_LEN, dbRec.m_Youto );
		// 異動事由
		CmnInitialControlFgStringCell( m_pDiag, dbRec.m_NumRow, ID_COL_071I_RIYUU, dbRec.m_FgFunc, UC_071_RIYUU_LEN, dbRec.m_Riyuu );

		dbRec.MoveNext();
	}

	// 閉じる
	dbRec.Fin();

	InitialControlTotalArea();				// 頁計/累計行

	// 保管ﾃﾞｰﾀ件数の表示
	virUpdateControlTblData2();

	//if(bG_Kanso == TRUE) {
		TitleColorChg();
	//}

	// 画面更新
	m_pDiag->Refresh();
	m_pDiagKei->Refresh();

	return TRUE;
}

//********************************************************************************
//	保管件数の表示
//********************************************************************************
BOOL CfrmUc071KoteisisanI::virUpdateControlTblData2()
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
//		IN		CdbUc071Koteishisan*	「固定資産」
//		RET		なし
//********************************************************************************
void	CfrmUc071KoteisisanI::UpdateControlRowData( CdbUc071Koteishisan* prmDbRec ) {
	DIAGRAM_DATA	diadata;	// ICSDiag構造体
	int				intCell;	// 保存フィールド
	CString			cs1, cs2, cst;

	// 初期化
	m_clsFunc.DiagInit( &diadata );

	// 読込み対象セル取得
	intCell = CmnGetControlIndex( prmDbRec->m_NumRow, ID_COL_071I_SYURUI );

	// 種類・構造
	diadata.data_edit = prmDbRec->m_Syurui;
	m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	intCell++;

	// 用途
	diadata.data_edit = prmDbRec->m_Youto;
	m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	intCell++;

	// 面積
	if ( prmDbRec->m_Area != "" ) {
		CmnChangeFieldValData( &diadata.data_val[0], prmDbRec->m_Area );
		m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	}
	else {
		m_pDiag->DataClear( intCell, TRUE );
	}
	intCell++;

	// 物件の所在地1
	diadata.data_edit = prmDbRec->m_Ad2Add1;
	m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	intCell++;

	// 物件の所在地2
	//	データ行？
	switch ( prmDbRec->m_FgFunc ) {
		//------------------------------
		//	空行
		case ID_FGFUNC_NULL:
		//	データ行
		case ID_FGFUNC_DATA:
		//	一括金額（手動）
		case ID_FGFUNC_IKKATUMANUAL:
		//------------------------------
			diadata.data_edit = prmDbRec->m_Ad2Add2;
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
		//	一括金額（自動）
		case ID_FGFUNC_IKKATUAUTO:
		//------------------------------
			diadata.data_edit = prmDbRec->m_KeiStr;
			break;
		default:
			break;
	}
	m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	intCell++;

	// 期末現在高
	if ( prmDbRec->m_Val != "" ) {
		CmnChangeFieldValData( &diadata.data_val[0], prmDbRec->m_Val );
		m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	}
	else {
		m_pDiag->DataClear( intCell, TRUE );
	}
	intCell++;

	CdateConvert	dc;
	// 異動年月日
	dc.Convert( prmDbRec->m_SpDate, diadata.data_day, DC_DATE_GWMD );
	if ( dc.m_nDate != 0 ) {
		m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	}
	else {
		m_pDiag->DataClear( intCell , TRUE );
	}
	intCell++;

	// 異動事由
	diadata.data_edit = prmDbRec->m_Riyuu;
	m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	intCell++;

	// 所得(処分)価額
	if ( prmDbRec->m_MsVal != "" ) {
		CmnChangeFieldValData( &diadata.data_val[0], prmDbRec->m_MsVal );
		m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	}
	else {
		m_pDiag->DataClear( intCell, TRUE );
	}
	intCell++;

	// 異動直前の帳簿価額
	if ( prmDbRec->m_MsVal2 != "" ) {
		CmnChangeFieldValData( &diadata.data_val[0], prmDbRec->m_MsVal2 );
		m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	}
	else {
		m_pDiag->DataClear( intCell, TRUE );
	}
	intCell++;

	// 登録番号を分割
	cs1 = cs2 = _T("");
	if(prmDbRec->m_InvNo.IsEmpty() == FALSE) {
		InvoNoSplit(prmDbRec->m_InvNo, &cs1, &cs2);
	}

	// 登録番号（T選択）	ID_COL_071I_INVONOT
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

	// 登録番号			ID_COL_071I_INVONO
	m_clsFunc.DiagInit(&diadata);
	switch(prmDbRec->m_FgFunc)	{
		//------------------------------
		//	空行
		case ID_FGFUNC_NULL:
		//	データ行
		case ID_FGFUNC_DATA:
		//	一括金額（手動）
		case ID_FGFUNC_IKKATUMANUAL:
		////	一括金額（自動）
		//case ID_FGFUNC_IKKATUAUTO:
		//------------------------------
			diadata.data_edit = cs2;
			break;
		//------------------------------
		default:
			break;
	}
	m_pDiag->SetData(intCell, (LPUNKNOWN)&diadata);
	intCell++;

	// 売却(購入)先の名称1(氏名)
	diadata.data_edit = m_clsFunc.StrDocking( prmDbRec->m_AdName1 , prmDbRec->m_AdName2 );
	m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	intCell++;

	// 売却(購入)先の所在地1(住所)
	diadata.data_edit = prmDbRec->m_AdAdd1;
	m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	intCell++;

	// 売却(購入)先の所在地2(住所)
	diadata.data_edit = prmDbRec->m_AdAdd2;
	m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	intCell++;

	// 売却物件の取得年月
	dc.Convert( prmDbRec->m_SpDate2, diadata.data_day, DC_DATE_GWM );
	if ( dc.m_nDate != 0 ) {
		m_pDiag->SetData( intCell, ( LPUNKNOWN )&diadata );
	}
	else {
		m_pDiag->DataClear( intCell , TRUE );
	}
}

//********************************************************************************
//	帳表データの１行表示（累計，頁計行）
//		IN		CdbUc071Koteishisan*	「固定資産」テーブル情報
//		RET		なし
//********************************************************************************
void	CfrmUc071KoteisisanI::UpdateControlRowTotal( CdbUc071Koteishisan* prmDbRec )
{
	int		intTotalType;
	int		intRowName = 0;		// 名称表示行（0:非表示）
	int		intRowVal = 0;		// 期末現在高表示行（0:非表示）
	int		intRowMsVal = 0;		// 所得(処分)価額表示行（0:非表示）
	int		intRowMsVal2 = 0;		// 異動直前の帳簿価額表示行（0:非表示）

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
			intRowMsVal = 2;
			intRowMsVal2 = 2;
		}
		break;
	case ID_OUTKEI_RUIKEI:
		// カレント頁："累計"なら、合計行②に"累計"を表示
		if ( prmDbRec->m_FgFunc == ID_FGFUNC_RUIKEI ) {
			intRowName = 2;
			intRowVal = 2;
			intRowMsVal = 2;
			intRowMsVal2 = 2;
		}
		break;
	case ID_OUTKEI_BOTH:
		// カレント頁："頁計＋累計"なら、合計行①に"頁計"＋合計行②に"累計"を表示
		if ( prmDbRec->m_FgFunc == ID_FGFUNC_PAGEKEI ) {
			intRowName = 1;
			intRowVal = 1;
			intRowMsVal = 1;
			intRowMsVal2 = 1;
		}
		else if ( prmDbRec->m_FgFunc == ID_FGFUNC_RUIKEI ) {
			intRowName = 2;
			intRowVal = 2;
			intRowMsVal = 2;
			intRowMsVal2 = 2;
		}
		break;
	}

	// 頁計/累計の名称表示
	if ( intRowName > 0 ) {
		CmnDiagSetString( m_pDiagKei, CmnGetKeiDiagIndex( intRowName, ID_COL_071I_KEI1_MEISYO ), prmDbRec->m_KeiStr, 1 );
	}
	// 頁計/累計の金額表示
	if ( intRowVal > 0 ) {
		CmnDiagSetKingaku( m_pDiagKei, CmnGetKeiDiagIndex( intRowVal, ID_COL_071I_KEI1_VAL ), prmDbRec->m_Val );
	}
	if ( intRowMsVal > 0 ) {
		CmnDiagSetKingaku( m_pDiagKei, CmnGetKeiDiagIndex( intRowMsVal, ID_COL_071I_KEI1_MSVAL ), prmDbRec->m_MsVal );
	}
	if ( intRowMsVal2 > 0 ) {
		CmnDiagSetKingaku( m_pDiagKei, CmnGetKeiDiagIndex( intRowMsVal2, ID_COL_071I_KEI1_MSVAL2 ), prmDbRec->m_MsVal2 );
	}
}

//********************************************************************************
//	【F6:項目複写】１つ前のデータ表示処理
//		IN		CdbUc000Common*	共通テーブルクラス
//		RET		なし
//********************************************************************************
void	CfrmUc071KoteisisanI::virFncCellCopyProc( CdbUc000Common* rsData )
{
	CdbUc071Koteishisan*	rs;
	int						intCol;	// 現在の列
	int						ind1,ind2;
	CString					cs1,cs2;

	rs = (CdbUc071Koteishisan*)rsData;

	// 列取得
	intCol = CmnGetControlCol();

	// 列によって処理を分岐
	switch( intCol ){
		// 種類・構造
		case ID_COL_071I_SYURUI:
			CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_Syurui, 0 );
			break;
		// 用途
		case ID_COL_071I_YOUTO:
			CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_Youto, 0 );
			break;
		// 面積
		case ID_COL_071I_AREA:
			CmnDiagSetKingaku( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_Area );
			break;
		// 物件の所在地1
		case ID_COL_071I_AD2ADD1:
			CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_Ad2Add1, 0 );
			break;
		// 物件の所在地2
		case ID_COL_071I_AD2ADD2:
			CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_Ad2Add2, 0 );
			break;
		// 期末現在高
		case ID_COL_071I_VAL:
			CmnDiagSetKingaku( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_Val );
			break;
		// 異動年月日
		case ID_COL_071I_SPDATE:
			CmnDiagSetDateWMD(m_pDiag, m_uInfo.intCurCtlIndex, DC_DATE_GWMD, rs->m_SpDate);
			break;
		// 異動事由
		case ID_COL_071I_RIYUU:
			CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_Riyuu, 0 );
			break;
		// 所得(処分)価額
		case ID_COL_071I_MSVAL:
			CmnDiagSetKingaku( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_MsVal );
			break;
		// 異動直前の帳簿価額
		case ID_COL_071I_MSVAL2:
			CmnDiagSetKingaku( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_MsVal2 );
			break;
		// 登録番号（T選択）
		case ID_COL_071I_INVONOT:
		// 登録番号
		case ID_COL_071I_INVONO:
			if(intCol == ID_COL_071I_INVONOT) {
				ind1 = m_uInfo.intCurCtlIndex;
				ind2 = m_uInfo.intCurCtlIndex+1;
			}
			else {
				ind1 = m_uInfo.intCurCtlIndex-1;
				ind2 = m_uInfo.intCurCtlIndex;
			}
			// 登録番号選択
			CmnDiagSetCombo3( m_pDiag, ind1, rs->m_InvNo );
			// 登録番号
			InvoNoSplit(rs->m_InvNo, &cs1, &cs2);
			CmnDiagSetString(m_pDiag, ind2, cs2, 0);
			break;
		// 売却(購入)先の名称1(氏名)
		case ID_COL_071I_ADNAME1:
			CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, m_clsFunc.StrDocking( rs->m_AdName1,rs->m_AdName2 ) , 0 );
			break;
		// 売却(購入)先の所在地1(住所)
		case ID_COL_071I_ADADD1:
			CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_AdAdd1, 0 );
			break;
		// 売却(購入)先の所在地2(住所)
		case ID_COL_071I_ADADD2:
			CmnDiagSetString( m_pDiag, m_uInfo.intCurCtlIndex, rs->m_AdAdd2, 0 );
			break;
		// 売却物件の取得年月
		case ID_COL_071I_SPDATE2:
			CmnDiagSetDate( m_pDiag, m_uInfo.intCurCtlIndex, DC_DATE_GWM, rs->m_SpDate2 );
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
int	CfrmUc071KoteisisanI::virFncReferenceGetType(int intCol, int* intGrSeq, long* lngItemSeq, int* intAdd1, int* intAdd2, int* intAdd1Col, int* intAdd2Col, REFERENCE_INFO* pudReference)
{
	int		intRefType = 0;

	switch ( intCol ) {
		// 物件の所在地1/2
		case ID_COL_071I_AD2ADD1:
		case ID_COL_071I_AD2ADD2:
			intRefType = ID_DLGTYPE_ICSADDSEARCH;
			*intAdd1 = 20 * 2;			// Byte指定
			*intAdd2 = 20 * 2;			// Byte指定
			*intAdd1Col = ID_COL_071I_AD2ADD1;
			*intAdd2Col = ID_COL_071I_AD2ADD2;
			break;

		// 登録番号
		case ID_COL_071I_INVONOT:
		case ID_COL_071I_INVONO:
			intRefType = ID_DLGTYPE_INVOICENO;
			*intGrSeq = ID_ADDRESSGR_NONE;
			*lngItemSeq = 0;
			break;

		// 売却(購入)先の所在地1/2(住所)
		case ID_COL_071I_ADADD1:
		case ID_COL_071I_ADADD2:
			intRefType = ID_DLGTYPE_ICSADDSEARCH;
			*intAdd1 = 15 * 2;			// Byte指定
			*intAdd2 = 15 * 2;			// Byte指定
			*intAdd1Col = ID_COL_071I_ADADD1;
			*intAdd2Col = ID_COL_071I_ADADD2;
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
void	CfrmUc071KoteisisanI::virFncReferenceSetICSData(CdlgAddressSearch* dlgAddd)
{
	int			intIndex;

	switch ( m_uInfo.intCurCol ) {
		// 物件の所在地1/2
		case ID_COL_071I_AD2ADD1:
		case ID_COL_071I_AD2ADD2:
			// 物件の所在地1
			intIndex = CmnGetControlIndex( m_uInfo.intCurRow, ID_COL_071I_AD2ADD1 );
			CmnDiagSetString( m_pDiag, intIndex, dlgAddd->m_AdAdd1, 0 );
			// 物件の所在地2
			intIndex = CmnGetControlIndex( m_uInfo.intCurRow, ID_COL_071I_AD2ADD2 );
			CmnDiagSetString( m_pDiag, intIndex, dlgAddd->m_AdAdd2, 0 );
			break;

		// 売却(購入)先の所在地1/2(住所)
		case ID_COL_071I_ADADD1:
		case ID_COL_071I_ADADD2:
			// 売却(購入)先の所在地1(住所)
			intIndex = CmnGetControlIndex( m_uInfo.intCurRow, ID_COL_071I_ADADD1 );
			CmnDiagSetString( m_pDiag, intIndex, dlgAddd->m_AdAdd1, 0 );
			// 売却(購入)先の所在地2(住所)
			intIndex = CmnGetControlIndex( m_uInfo.intCurRow, ID_COL_071I_ADADD2 );
			CmnDiagSetString( m_pDiag, intIndex, dlgAddd->m_AdAdd2, 0 );
			break;
	}
}

void CfrmUc071KoteisisanI::virActivateFrame(UINT nState, CFrameWnd* pDeactivateFrame)
{
	int		index = 0;
	CWnd*	pWnd = this->GetFocus();

	if(pWnd != NULL)	{
		if(m_DataKakutei == FALSE)	{
			if(m_uInfo.intCurCol == ID_COL_071I_SPDATE || m_uInfo.intCurCol == ID_COL_071I_SPDATE2)	{
				if(m_uInfo.EditSign == 1)	{
					m_uInfo.EditSign = -1;
					m_uInfo.GenListSw = 0;
					if(m_uInfo.intCurCol == ID_COL_071I_SPDATE) {
						m_uInfo.OldDate[0] = 0;
						m_uInfo.OldGengo[0] = 0;
					}
					else	{
						m_uInfo.OldDate[1] = 0;
						m_uInfo.OldGengo[1] = 0;
					}
				}
			}
		}
	}
}

//********************************************************************************
//	その他イベント処理
//********************************************************************************
BOOL	CfrmUc071KoteisisanI::PreTranslateMessage( MSG* pMsg )
{
	BOOL			blnMove = TRUE;		// カーソル移動可能/不可フラグ
	int				intOrgIndex;		// カーソル移動前のインデックス番号
	int				intMoveRow;			// カーソル移動先（行）
	int				intMoveCol;			// カーソル移動先（列）
	int				rv=0;
	int				intRet=0;
	int				nMaxDataCol=0;		// 1頁の最大行数
	int				intTotalType=0;		// 合計タイプ
	int				id=0;
	int				intCol=0;
	int				sw=0;
	CWnd*			pWnd = this->GetFocus();

	if ( pWnd != NULL ){
		id = pWnd->GetDlgCtrlID();
		if(id == IDC_PAGEBACK_BUTTON1 || id == IDC_PAGENEXT_BUTTON1 || id == IDC_PAGENEW_BUTTON1 || 
			id == IDC_PAGEINSERT_BUTTON1 || id == IDC_ROWCOPY_BUTTON1 || id == IDC_ROWPASTE_BUTTON1 || 
			id == IDC_ROWINSERT_BUTTON1 || id == IDC_YOUSHIKIBACK_BUTTON1 || id == IDC_YOUSHIKINEXT_BUTTON1 )	{
			nG_MScroll = 0;
			sw = 1;
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
						// 例外処理：登録番号(T)選択でPageUp，PageDownキーが押下された場合は値を元に戻す
						if(intCol == ID_COL_071I_INVONOT) {
							m_pDiag->ComboDropDown(m_uInfo.intCurCtlIndex, FALSE);
							CmnDiagSetCombo(m_pDiag, m_uInfo.intCurCtlIndex, m_uInfo.OldCombo);
						}

						// Scroll＋フォーカス移動先を取得
						if (pMsg->wParam == VK_PRIOR) {
							// PageUpの場合：Scroll＋フォーカス移動（行－１１）
							m_pDiag->ScrollPos(SB_PAGEUP , 0);
							intMoveRow = intMoveRow - 5;
							if (intMoveRow < 1) {
								blnMove = FALSE;					// 移動しない
							}
						}
						else {
							// PageDownの場合：Scroll＋フォーカス移動（行＋１１）
							m_pDiag->ScrollPos(SB_PAGEDOWN, 0);
							intMoveRow = intMoveRow + 5;
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
						intCol = CmnGetControlCol();
						if(intCol == ID_COL_071I_SPDATE || intCol == ID_COL_071I_SPDATE2 || intCol == ID_COL_071I_INVONOT)	{
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
				else if(m_DataKakutei == FALSE && (sw == 0 && m_Pagefocus == 0 && m_Titlefocus == 0 ))	{
					intCol = CmnGetControlCol();
					if (intCol == ID_COL_071I_SPDATE || intCol == ID_COL_071I_SPDATE2) {
						if(pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN)	{
							if(intCol == ID_COL_071I_SPDATE)	{
								m_uInfo.OldDate[0] = CmnDiagGetDate(m_pDiag, m_uInfo.intCurCtlIndex, DC_DATE_GWMD);
								m_uInfo.OldGengo[0] = CmnDiagGetDate(m_pDiag, m_uInfo.intCurCtlIndex, DC_DATE_GENGOU);
							}
							else	{
								m_uInfo.OldDate[1] = CmnDiagGetDate(m_pDiag, m_uInfo.intCurCtlIndex, DC_DATE_GWM);
								m_uInfo.OldGengo[1] = CmnDiagGetDate(m_pDiag, m_uInfo.intCurCtlIndex, DC_DATE_GENGOU);
							}
							m_uInfo.GenListSw = 0;
						}

						if(IsSpecialRow(m_ReadData[m_uInfo.intCurRow].m_FgFunc) == 1 || pMsg->wParam == VK_LEFT || pMsg->wParam == VK_RIGHT)	{
							rv = CursorControl((short)pMsg->wParam,0);
						}
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
void	CfrmUc071KoteisisanI::FocusPagediag1()
{
}

// 頁コントロールのゲットフォーカス
void	CfrmUc071KoteisisanI::EditONPagediag1( short index )
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
	int intRet = virRecBufSaveData(m_uInfo.intCurPage, m_uInfo.intCurRow);
	if ((intRet == FUNCTION_UPDATE) || ( intRet == FUNCTION_DISPUPDATE)) {
		m_PageDiag.SetPosition(1);
	}
}

// 頁コントロールのロストフォーカス
void	CfrmUc071KoteisisanI::EditOFFPagediag1( short index )
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
void	CfrmUc071KoteisisanI::TerminationPagediag1( short index, short nChar, short length, LPUNKNOWN data )
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
			CmnDiagSetFocus(m_pDiag, nMaxDataCol, ID_COL_071I_SPDATE2);
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
void	CfrmUc071KoteisisanI::FocusTitlediag1()
{
}

// タイトルコントロールのゲットフォーカス
void	CfrmUc071KoteisisanI::EditONTitlediag1( short index )
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
	int intRet = virRecBufSaveData(m_uInfo.intCurPage, m_uInfo.intCurRow);
	if ((intRet == FUNCTION_UPDATE) || ( intRet == FUNCTION_DISPUPDATE)) {
		m_TitleDiag.SetPosition(0);
	}
}

// タイトルコントロールのロストフォーカス
void	CfrmUc071KoteisisanI::EditOFFTitlediag1 (short index )
{
	m_Titlefocus = 0;
	virUpdateTitle();
}

//	タイトル更新
void CfrmUc071KoteisisanI::virUpdateTitle()
{
	// 帳表タイトル取得してuc_inf_subを更新
	m_uInfo.strTitleName = m_clsFunc.DiagGetString( &m_TitleDiag, 0 );
	CmnUcInfSubSetTitleInfo( TRUE );
}

// タイトルコントロールのキーイベント
void	CfrmUc071KoteisisanI::TerminationTitlediag1( short index, short nChar, short length, LPUNKNOWN data )
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
//	「固定資産」データコントロール処理（EditON，EditOFF，Terminationイベント）
//********************************************************************************

// 「固定資産」データコントロールのゲットフォーカス
void	CfrmUc071KoteisisanI::EditONYoushikidiag1( short index )
{
	DIAGRAM_DATA	diadata;
	int				intRet = -1;

	// スクロールフラグをクリアする
	nG_MScroll = 0;

	// フォーカスフラグON
	m_uInfo.intCurFocus = 1;

	// ポジション取得
	m_uInfo.intCurCtlIndex = m_pDiag->GetPosition();

	// 行・列取得
	CmnGetControlPosition( &m_uInfo.intCurRow , &m_uInfo.intCurCol, &m_uInfo.OldCol );

	// 頁、行、列のいずれかが変わったらEditOFF発行カウントをクリア
	if(m_uInfo.OldPage != m_uInfo.intCurPage || m_uInfo.intCurRow != m_uInfo.OldRow || m_uInfo.intCurCol != m_uInfo.OldCol)	{
		m_uInfo.GenListSw = 0;
	}
	if(m_uInfo.intCurCol == ID_COL_071I_SPDATE || m_uInfo.intCurCol == ID_COL_071I_SPDATE2)	m_uInfo.EditSign = 1;
	else																					m_uInfo.EditSign = -1;

	switch (m_uInfo.intCurCol) {
		case ID_COL_071I_SPDATE:	// 異動年月日
			if(m_uInfo.GenListSw == 0)	{
				m_uInfo.OldDate[0] = 0;
				m_uInfo.OldGengo[0] = 0;
			}
			break;

		case ID_COL_071I_SPDATE2:	// 売却物件の取得年月
			if(m_uInfo.GenListSw == 0)	{
				m_uInfo.OldDate[1] = 0;
				m_uInfo.OldGengo[1] = 0;
			}
			break;

		case ID_COL_071I_INVONOT:	// 登録番号(T選択)
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
			// T選択⇔登録番号は保存しない
			BOOL tsel_to_invno = IsSaveMoveReference(ID_COL_071I_INVONOT, ID_COL_071I_INVONO);
			// データ保存
			if((tsel_to_invno != FALSE)) {
				// カレント行登録＋再表示
				intRet = virRecBufSaveData(m_uInfo.intCurPage , m_uInfo.OldRow );
				if((intRet == FUNCTION_UPDATE) || (intRet == FUNCTION_DISPUPDATE)) {
					// カーソル移動処理
					CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
				}
			}
		}
	}

	// ボタン操作
	CmnChangeButtonEnable(1);

	// ---- 登録番号一覧関連 ----->
	int		sw=0;
	// 登録番号(T選択)にカーソルがあるとき、登録番号一覧を表示する
	if(m_uInfo.intCurCol == ID_COL_071I_INVONO && m_EndSyorityu == 0)	{
// 修正No.168377 del -->
		//// 登録番号一覧は、空行、データ行、一括金額(手動)の時に表示
		//if(m_ReadData[m_uInfo.intCurRow].m_FgFunc == ID_FGFUNC_NULL || m_ReadData[m_uInfo.intCurRow].m_FgFunc == ID_FGFUNC_DATA ||
		//   m_ReadData[m_uInfo.intCurRow].m_FgFunc == ID_FGFUNC_IKKATUMANUAL) {
// 修正No.168377 del <--
// 修正No.168377 add -->
		// 登録番号のT選択の値を取得
		int pos = CmnGetControlIndex(m_uInfo.intCurRow, ID_COL_071I_INVONOT);
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
void	CfrmUc071KoteisisanI::EditOFFYoushikidiag1( short index )
{
	if( m_EndView != FALSE )	return;

// 修正No.168443 add -->
	// ---- 登録番号一覧関連 ----->
	if(m_uInfo.intCurCol != ID_COL_071I_INVONO) {
		if(pZmSel && pZmSel->IsDisplay() != FALSE) {
			pZmSel->DialogOFF();
		}
	}
	// ---- 登録番号一覧関連 <-----
// 修正No.168443 add <--

	// 入力確定
	virInputDecisionEditOFF();

	// 異動年月日 取得年月
	if(m_uInfo.intCurCol == ID_COL_071I_SPDATE || m_uInfo.intCurCol == ID_COL_071I_SPDATE2)	{
		// 元号+年月日の入力状況を取得
		Date_ChkAndSet(index, m_uInfo.intCurCol, 1);
		m_uInfo.EditSign = 0;
	}
	else {
		m_uInfo.EditSign = -1;
	}

// 修正No.168443 del -->
	//// ---- 登録番号一覧関連 ----->
	//if(pZmSel && m_uInfo.intCurCol != ID_COL_071I_INVONO) {
	//	if(pZmSel->IsDisplay() != FALSE) {
	//		pZmSel->DialogOFF();
	//	}
	//}
	//// ---- 登録番号一覧関連 <-----

	//if(m_uInfo.intCurCol == ID_COL_071I_INVONO)		{
	//	// 入力された登録番号から参照ダイアログを検索し、登録済みの場合はデータを取得して画面に表示
	//	if(ChkInvoiceNum(m_pDB, 2, m_uInfo.intCurPage, m_uInfo.intCurRow) == -1) {
	//		//// カーソル移動処理
	//		//CmnDiagSetFocus( m_pDiag, m_uInfo.intCurRow, ID_COL_071I_INVONO );
	//	}
	//}
// 修正No.168443 del <--
// 修正No.168443 add -->
	if(m_uInfo.intCurCol == ID_COL_071I_INVONO)		{
		// 入力された登録番号をチェック
		ChkInvoiceNum(m_pDB, 2, m_uInfo.intCurPage, m_uInfo.intCurRow);
	}
// 修正No.168443 add <--
}

//	入力確定
void CfrmUc071KoteisisanI::virInputDecisionEditOFF()
{
	// フォーカスフラグOFF
	m_uInfo.intCurFocus = 0;

	// 入力項目を強制的に確定
	// 依頼No.155271
	// 並べて表示・右側表示
	// ⑦固定資産（土地、土地の上に存する権利及び建物に限る。）の内訳書
	// 最終行のデータ入力で、元号を選択しても元号が表示されない。その状態で年月を入力してenterキー押下すると
	// 「パラメーターが間違っています。」と表示されます。
	// [修正依頼：問合せシート_20190919.xlsx] 管理No.071
	// →モジュールが修正されるまでの対応
	if(CmnGetControlCol() != ID_COL_071I_SPDATE2)	{
		CmnDiagSetEnterReDraw( m_pDiag, m_pDiag->GetPosition() );
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
void	CfrmUc071KoteisisanI::TerminationYoushikidiag1( short index, short nChar, short length, LPUNKNOWN data )
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
					if(intCol != ID_COL_071I_INVONOT) {
						m_pDiag->DataClear(m_uInfo.intCurCtlIndex, TRUE);
					}

					if(intCol == ID_COL_071I_SPDATE)	{
						m_uInfo.OldDate[0] = 0;
						m_uInfo.OldGengo[0] = 0;
					}
					else if(intCol == ID_COL_071I_SPDATE2)	{
						m_uInfo.OldDate[1] = 0;
						m_uInfo.OldGengo[1] = 0;
					}

					m_uInfo.GenListSw = 0;

					if(intCol == ID_COL_071I_SPDATE || intCol == ID_COL_071I_SPDATE2)	{
						SendMessage(WM_USER_FOCUSSET,0,0);
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

void CfrmUc071KoteisisanI::VScrollYoushikidiag1(short mode)
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
int	CfrmUc071KoteisisanI::GetFocusPosition( int nChar )
{
	int		intRow;				// カレント行番号
	int		intCol;				// カレント列番号
	int		intVector[4];		// 矢印キーの移動先（0:↑，1:→，2:↓，3:←）
	int		intNext = 0;		// フォーカス移動先
	int		intRowChange = 0;	// 行をまたぐ場合のインデックス増減数

	// カレント行＋列番号を取得
	CmnGetControlPosition( &intRow, &intCol );

	// 行をまたぐ場合のインデックス増減数（最右セル→最左セル）
	intRowChange = m_uInfo.intCtlStartIndex + 1 + 6;	// ここの値は検討が必要

	// 「売上物件の取得年月」はComboBoxのため、「↑，↓」キーではカーソルを移動しない
	if (intCol == ID_COL_071I_SPDATE2) {
		if ((nChar == VK_UP) || (nChar == VK_DOWN)) {
			// キー操作を無視する
			return -3;
		}
	}

	//	計行じゃない？
	if ( CmnCheckTotalRow() == FALSE ){
		// 「登録番号(T選択)」はComboBoxのため、「↑，↓」キーではカーソルを移動しない
		if(intCol == ID_COL_071I_INVONOT)	{
			if ((nChar == VK_UP) || (nChar == VK_DOWN)) {
				// キー操作を無視する
				return -3;
			}
		}
	}

	//----------------------------------------------------------------
	//	矢印キーによる移動先を取得（合計欄は次のステップで対応）
	//	基本的にReturn，Tab，Shift+Tabは、←，→キーと同じなので省略
	//----------------------------------------------------------------
	// カレント列が「種類・構造」欄の場合
	if ( intCol == ID_COL_071I_SYURUI ){
		intVector[0] = m_uInfo.intCurCtlIndex - m_uInfo.intColMax + 4;	// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + 3;						// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - intRowChange;			// ←
	}
	// カレント列が「用途」欄の場合
	else if ( intCol == ID_COL_071I_YOUTO ){
		intVector[0] = m_uInfo.intCurCtlIndex - m_uInfo.intColMax + 3;	// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + 2;						// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;						// ←
	}
	// カレント列が「面積」欄の場合
	else if ( intCol == ID_COL_071I_AREA ){
		intVector[0] = m_uInfo.intCurCtlIndex - m_uInfo.intColMax + 2;	// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + 1;						// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;						// ←
	}
	// カレント列が「物件の所在地」上段 欄の場合
	else if ( intCol == ID_COL_071I_AD2ADD1 ){
		intVector[0] = m_uInfo.intCurCtlIndex - 3;						// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + 1;						// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;						// ←
	}
	// カレント列が「物件の所在地」下段 欄の場合
	else if ( intCol == ID_COL_071I_AD2ADD2 ){
		intVector[0] = m_uInfo.intCurCtlIndex - 1;						// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + m_uInfo.intColMax - 4;	// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;						// ←
	}
	// カレント列が「異動年月日」、「取得(処分)価額」欄の場合
	else if ( (intCol == ID_COL_071I_SPDATE) || (intCol == ID_COL_071I_MSVAL) ){
		intVector[0] = m_uInfo.intCurCtlIndex - m_uInfo.intColMax + 1;	// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + 1;						// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;						// ←
	}
	// カレント列が「異動事由」、「異動直前の帳簿価額」欄の場合
	else if ( (intCol == ID_COL_071I_RIYUU) || (intCol == ID_COL_071I_MSVAL2) ){
		intVector[0] = m_uInfo.intCurCtlIndex - 1;						// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + m_uInfo.intColMax - 1;	// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;						// ←
	}
	// カレント列が「売却(購入)先の名称(氏名) 上段」欄の場合
	else if ( intCol == ID_COL_071I_ADNAME1 ){
		intVector[0] = m_uInfo.intCurCtlIndex - m_uInfo.intColMax + 2;	// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + 1;						// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;						// ←
	}
	// カレント列が「売却(購入)先の名称(氏名) 下段」、「売却(購入)先の所在地(住所) 上段」欄の場合
	else if (intCol == ID_COL_071I_ADADD1){
		intVector[0] = m_uInfo.intCurCtlIndex - 1;						// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + 1;						// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;						// ←
	}
	// カレント列が「売却(購入)先の所在地(住所) 下段」欄の場合
	else if (  (intCol == ID_COL_071I_ADADD2) ){
		intVector[0] = m_uInfo.intCurCtlIndex - 1;						// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + 1;						// →
		intVector[2] = m_uInfo.intCurCtlIndex + m_uInfo.intColMax - 2;	// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;						// ←
	}
	// カレント列が「売却物件の取得年月」欄の場合
	else if ( intCol == ID_COL_071I_SPDATE2 ){
		intVector[0] = m_uInfo.intCurCtlIndex - m_uInfo.intColMax;		// ↑
		intVector[1] = m_uInfo.intCurCtlIndex + intRowChange;			// →
		intVector[2] = m_uInfo.intCurCtlIndex + m_uInfo.intColMax;		// ↓
		intVector[3] = m_uInfo.intCurCtlIndex - 1;						// ←
	}
	// カレント列が上記以外の欄の場合
	else{
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
				if(intCol == ID_COL_071I_SYURUI)		intNext = intVector[1];			// カレント列が「種類・構造」欄の場合「→」キーと同じ
				else if(intCol == ID_COL_071I_YOUTO)	intNext = intVector[1];			// カレント列が「用途」欄の場合「→」キーと同じ
				else if(intCol == ID_COL_071I_INVONOT)	intNext = intVector[1];			// カレント列が「登録番号(T表示)」欄の場合「→」キーと同じ
				else									intNext = intVector[2];			// Enterキーでカーソルを下に移動するチェック時は「↓」キーと同じ
			}
			else										intNext = intVector[1];			// 「Return」キー押下時：「→」キーと同じ
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
int	CfrmUc071KoteisisanI::GetTotalRowIndex( int nCharOrg, int intIndex, int intNext )
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
		if (( intNext > CmnGetControlIndex( m_uInfo.intRowMax, ID_COL_071I_SPDATE2 ) ) && ( nCharOrg == VK_TAB )){
			return( -2 );
		}
		//	データ行を超えた？かつ計行あり？
		else if (( nNextRow > nBorder[0] ) && ( nBorder[1] > 0 )){
			//	計行に移動
			if (nCharOrg == VK_TAB) {
				return -2;
			}
		}
	}
	
	//	戻値を返す
	return( nRet );
}

//********************************************************************************
//	ソート項目を再設定後に、ソートを実行する
//		IN		CdlgSort*			ソートダイアログ情報
//				CdbUc000Common*		テーブル情報
//		RET		int					実行結果（FUNCTION_OK, FUNCTION_NG）
//********************************************************************************
int	CfrmUc071KoteisisanI::virTblSortSubSortProc( CdlgSort* pCdlgSort, CdbUc000Common* rsData )
{
	CdbUc071Koteishisan*	rs;
	CString				strFilter;
	CString				strSort;

	// ソート項目の値の再設定は無し

	// ソート条件取得
	CmnTblSortSubGetSortParam( pCdlgSort, &strFilter, &strSort) ;

	rs = (CdbUc071Koteishisan*)rsData;

	// 対象フィールドがNULLならNULL文字列にする：文字列連結のソート時は必須
	rs->UpdateDataTableWork();

	// ソート実行
	return rs->RequerySortParameter( strFilter, strSort );
}

/**********************************************************************
	RecBufSetData(CdbUc071Koteishisan* prmDbRec)
		ローカルのレコードバッファに、1行分のデータを格納する

	引数
		CdbUc071Koteishisan*	prmDbRec	レコードセット(データがあること）
	戻値
		int		成功/失敗
				FUNCTION_OK
***********************************************************************/
int	CfrmUc071KoteisisanI::RecBufSetData( CdbUc071Koteishisan* prmDbRec )
{
	// 1ページ分のレコードバッファの該当行にデータを格納する
	int	trgRow = prmDbRec->m_NumRow;
	m_ReadData[trgRow].m_Seq = prmDbRec->m_Seq;				// シーケンス番号
	m_ReadData[trgRow].m_NumPage = prmDbRec->m_NumPage;		// 頁番号
	m_ReadData[trgRow].m_NumRow = prmDbRec->m_NumRow;		// 行番号
	m_ReadData[trgRow].m_FgFunc = prmDbRec->m_FgFunc;		// 特殊行フラグ
	m_ReadData[trgRow].m_FgShow = prmDbRec->m_FgShow;		// 表示フラグ
	m_ReadData[trgRow].m_KeiStr = prmDbRec->m_KeiStr;		// 計
	m_ReadData[trgRow].m_NumGroup = prmDbRec->m_NumGroup;	// グループ番号
	m_ReadData[trgRow].m_Syurui = prmDbRec->m_Syurui;		// 種類・構造
	m_ReadData[trgRow].m_Youto = prmDbRec->m_Youto;			// 用途
	m_ReadData[trgRow].m_Area = prmDbRec->m_Area;			// 面積
	m_ReadData[trgRow].m_Ad2Add1 = prmDbRec->m_Ad2Add1;		// 物件の所在地（上段）
	m_ReadData[trgRow].m_Ad2Add2 = prmDbRec->m_Ad2Add2;		// 物件の所在地（下段）
	m_ReadData[trgRow].m_Val = prmDbRec->m_Val;				// 期末現在高
	m_ReadData[trgRow].m_SpDate = prmDbRec->m_SpDate;		// 異動年月日
	m_ReadData[trgRow].m_Riyuu = prmDbRec->m_Riyuu;			// 異動事由
	m_ReadData[trgRow].m_MsVal = prmDbRec->m_MsVal;			// 取得価額
	m_ReadData[trgRow].m_MsVal2 = prmDbRec->m_MsVal2;		// 異動直前の帳簿価額
	m_ReadData[trgRow].m_AdSeq = prmDbRec->m_AdSeq;			// 売却先：取引先（シーケンス番号）
	m_ReadData[trgRow].m_AdOrder = prmDbRec->m_AdOrder;		// 売却先：取引先（並び順）
	m_ReadData[trgRow].m_AdName1 = prmDbRec->m_AdName1;		// 売却先（名称：上段）
	m_ReadData[trgRow].m_AdName2 = prmDbRec->m_AdName2;		// 売却先（名称：下段）
	m_ReadData[trgRow].m_AdAdd1 = prmDbRec->m_AdAdd1;		// 売却先（所在地：上段）
	m_ReadData[trgRow].m_AdAdd2 = prmDbRec->m_AdAdd2;		// 売却先（所在地：下段）
	m_ReadData[trgRow].m_SpDate2 = prmDbRec->m_SpDate2;		// 売却物件の取得年月
	m_ReadData[trgRow].m_InvNo = prmDbRec->m_InvNo;			// 登録番号（法人番号）

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
int	CfrmUc071KoteisisanI::RecBufClearAllData()
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
		P_REC_UC_071_KOTEISHISAN inRecData レコードへのポインタ
	戻値
		int		成功/失敗
				FUNCTION_OK
***********************************************************************/
int	CfrmUc071KoteisisanI::RecBufClearData( P_REC_UC_071_KOTEISHISAN inRecData )
{
	inRecData->m_Seq = 0;			// シーケンス番号
	inRecData->m_NumPage = 0;		// 頁番号
	inRecData->m_NumRow = 0;		// 行番号
	inRecData->m_FgFunc = 0;		// 特殊行フラグ
	inRecData->m_FgShow = 0;		// 表示フラグ
	inRecData->m_KeiStr = _T("");	// 計
	inRecData->m_NumGroup = 0;		// グループ番号
	inRecData->m_Syurui = _T("");	// 種類・構造
	inRecData->m_Youto = _T("");	// 用途
	inRecData->m_Area = _T("");		// 面積
	inRecData->m_Ad2Add1 = _T("");	// 物件の所在地（上段）
	inRecData->m_Ad2Add2 = _T("");	// 物件の所在地（下段）
	inRecData->m_Val = _T("");		// 期末現在高
	inRecData->m_SpDate = 0;		// 異動年月日
	inRecData->m_Riyuu = _T("");	// 異動事由
	inRecData->m_MsVal = _T("");	// 取得価額
	inRecData->m_MsVal2 = _T("");	// 異動直前の帳簿価額
	inRecData->m_AdSeq = 0;			// 売却先：取引先（シーケンス番号）
	inRecData->m_AdOrder = 0;		// 売却先：取引先（並び順）
	inRecData->m_AdName1 = _T("");	// 売却先（名称：上段）
	inRecData->m_AdName2 = _T("");	// 売却先（名称：下段）
	inRecData->m_AdAdd1 = _T("");	// 売却先（所在地：上段）
	inRecData->m_AdAdd2 = _T("");	// 売却先（所在地：下段）
	inRecData->m_SpDate2 = 0;		// 売却物件の取得年月
	inRecData->m_ShowKeiZero = 0;	// ０円計表示フラグ
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
int	CfrmUc071KoteisisanI::virRecBufSaveData( int inPage, int inRow )
{
	REC_UC_071_KOTEISHISAN	tmpSaveData;	// 画面データ1レコード格納用
	BOOL	blnSpDate = FALSE;				// 「異動年月日」入力更新フラグ

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

	// 初期化
	m_clsFunc.DiagInit( &diadata );

	// 種類・構造
	intIndex = CmnGetControlIndex( inRow, ID_COL_071I_SYURUI );
	tmpSaveData.m_Syurui = m_clsFunc.DiagGetString( m_pDiag, intIndex );

	// 用途
	intIndex = CmnGetControlIndex( inRow, ID_COL_071I_YOUTO );
	tmpSaveData.m_Youto = m_clsFunc.DiagGetString( m_pDiag, intIndex );

	// 面積
	intIndex = CmnGetControlIndex( inRow, ID_COL_071I_AREA );
	tmpSaveData.m_Area = CmnDiagGetKingaku( m_pDiag, intIndex );

	// 物件の所在地1
	intIndex = CmnGetControlIndex( inRow, ID_COL_071I_AD2ADD1 );
	tmpSaveData.m_Ad2Add1 = m_clsFunc.DiagGetString( m_pDiag, intIndex );

	// 物件の所在地2
	intIndex = CmnGetControlIndex( inRow, ID_COL_071I_AD2ADD2 );
	tmpSaveData.m_Ad2Add2 = m_clsFunc.DiagGetString( m_pDiag, intIndex );

	// 期末現在高
	intIndex = CmnGetControlIndex( inRow, ID_COL_071I_VAL );
	tmpSaveData.m_Val = CmnDiagGetKingaku( m_pDiag, intIndex );

	// 異動年月日
	intIndex = CmnGetControlIndex( inRow, ID_COL_071I_SPDATE );
	tmpSaveData.m_SpDate = CmnDiagGetDateGWMD( m_pDiag, intIndex, m_ReadData[inRow].m_SpDate );
	blnSpDate = CmnCheckDate( m_pDiag, intIndex, DC_DATE_GWMD, m_ReadData[inRow].m_SpDate );

	// 異動事由
	intIndex = CmnGetControlIndex( inRow, ID_COL_071I_RIYUU );
	tmpSaveData.m_Riyuu = m_clsFunc.DiagGetString( m_pDiag, intIndex );

	// 所得(処分)価額
	intIndex = CmnGetControlIndex( inRow, ID_COL_071I_MSVAL );
	tmpSaveData.m_MsVal = CmnDiagGetKingaku( m_pDiag, intIndex );

	// 異動直前の帳簿価額
	intIndex = CmnGetControlIndex( inRow, ID_COL_071I_MSVAL2 );
	tmpSaveData.m_MsVal2 = CmnDiagGetKingaku( m_pDiag, intIndex );

	// 登録番号（T選択）
	intIndex = CmnGetControlIndex(inRow, ID_COL_071I_INVONOT);
	m_pDiag->GetData(intIndex, (LPUNKNOWN)&diadata);
	tsel = diadata.data_combo;

	// 登録番号（法人番号）
	intIndex = CmnGetControlIndex(inRow, ID_COL_071I_INVONO);
	ino = m_clsFunc.DiagGetString(m_pDiag, intIndex);
	cst.Empty();
	if(ino.IsEmpty() == FALSE)	{
		if(tsel == 0)	cst.Format(_T("T%s"), ino);
		else			cst.Format(_T(" %s"), ino);
	}
	tmpSaveData.m_InvNo = cst;

	// 売却(購入)先の名称1(氏名)
	CStringArray strArray;
	intIndex = CmnGetControlIndex( inRow, ID_COL_071I_ADNAME1 );
	m_clsFunc.StrDivision( m_clsFunc.DiagGetString( m_pDiag, intIndex ), &strArray, 2 );
	tmpSaveData.m_AdName1 = strArray.GetAt( 0 );
	tmpSaveData.m_AdName2 = strArray.GetAt( 1 );
	// 2行目が空行なら1行目のみを再表示
	if ( tmpSaveData.m_AdName2.IsEmpty() ){
		if(m_uInfo.intCurCol == ID_COL_071I_ADNAME1 && m_uInfo.intCurRow != m_uInfo.OldRow){	// 各カラムごとにvirRecBufSaveData()がコールされるようになり、かつタイミングがEditOffからEditOnに移ったことによりCmnDiagSetString()でカレットが末尾から先頭に移ってしまう為、制御
			CmnDiagSetString( m_pDiag, intIndex, tmpSaveData.m_AdName1, 0 );
		}
	}

	// 売却(購入)先の所在地1(住所)
	intIndex = CmnGetControlIndex( inRow, ID_COL_071I_ADADD1 );
	tmpSaveData.m_AdAdd1 = m_clsFunc.DiagGetString( m_pDiag, intIndex );

	// 売却(購入)先の所在地2(住所)
	intIndex = CmnGetControlIndex( inRow, ID_COL_071I_ADADD2 );
	tmpSaveData.m_AdAdd2 = m_clsFunc.DiagGetString( m_pDiag, intIndex );

	// 売却物件の取得年月
	intIndex = CmnGetControlIndex( inRow, ID_COL_071I_SPDATE2 );
	tmpSaveData.m_SpDate2 = CmnDiagGetDate( m_pDiag, intIndex, DC_DATE_GWM );

	///////////////////////////////////////////////////////////////
	// 登録処理呼び出し
	int	intRet = RecBufSaveDataSub( inPage, inRow, &tmpSaveData );

	// 登録処理が更新あり&エラー以外かつ日付入力値が更新必要な場合、画面更新
	if ((( intRet != FUNCTION_UPDATE ) && ( intRet != FUNCTION_NG )) &&
		 ( blnSpDate == TRUE )){
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
int	CfrmUc071KoteisisanI::virRecBufSaveDataForPasteInsert( int inPage, int inRow )
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
int	CfrmUc071KoteisisanI::virRecBufSaveDataForPaste( int inPage, int inRow )
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
			REC_UC_071_KOTEISHISAN
					inSaveData	登録するデータレコードへのポインタ
	戻値
			int					成功(更新あり)/成功(更新なし)/失敗
								FUNCTION_UPDATE	: 成功(更新あり)
								FUNCTION_OK		: 成功(更新なし)
								FUNCTION_NG		: 失敗
***********************************************************************/
int	CfrmUc071KoteisisanI::RecBufSaveDataSub( 
							int inPage,
							int inRow,
							P_REC_UC_071_KOTEISHISAN inSaveData )
{
	CString	tmp_Syurui;			// 画面上のデータ格納用(種類・構造)
	CString	tmp_Youto;			// 画面上のデータ格納用(用途)
	CString	tmp_Area;			// 画面上のデータ格納用(面積)
	CString	tmp_Ad2Add1;		// 画面上のデータ格納用(物件の所在地) 上段
	CString	tmp_Ad2Add2;		// 画面上のデータ格納用(物件の所在地) 下段
	CString	tmp_Val;			// 画面上のデータ格納用(期末現在高)
	long	tmp_SpDate;			// 画面上のデータ格納用(異動年月日)
	CString	tmp_Riyuu;			// 画面上のデータ格納用(異動事由)
	CString	tmp_MsVal;			// 画面上のデータ格納用(取得(処分)価額)
	CString	tmp_MsVal2;			// 画面上のデータ格納用(異動直前の帳簿価額)
	CString	tmp_InvNo;			// 画面上のデータ格納用(登録番号(法人番号))
	CString	tmp_AdName1;		// 画面上のデータ格納用(売却(購入)先の名称(氏名) 上段)
	CString	tmp_AdName2;		// 画面上のデータ格納用(売却(購入)先の名称(氏名) 下段)
	CString	tmp_AdAdd1;			// 画面上のデータ格納用(売却(購入)先の所在地(住所) 上段)
	CString	tmp_AdAdd2;			// 画面上のデータ格納用(売却(購入)先の所在地(住所) 下段)
	long	tmp_SpDate2;		// 画面上のデータ格納用(売却物件の取得年月)

	BYTE	tmp_FgFunc;
	CString	cst;
	CString	tno1, tno2;			// 登録番号比較用

	// 変更があったかのフラグ（変更あり:TRUE、なし:FALSE)
	BOOL	ufgUpData	= FALSE;		// 変更確認フラグ(全体用データ行/空行)
	BOOL	ufgUpToku	= FALSE;		// 変更確認フラグ(全体用特殊行)
	BOOL	ufgSyurui	= FALSE;		// 変更確認フラグ(種類・構造)
	BOOL	ufgYouto	= FALSE;		// 変更確認フラグ(用途)
	BOOL	ufgArea		= FALSE;		// 変更確認フラグ(面積)
	BOOL	ufgAd2Add1	= FALSE;		// 変更確認フラグ(物件の所在地) 上段
	BOOL	ufgAd2Add2	= FALSE;		// 変更確認フラグ(物件の所在地) 下段
	BOOL	ufgVal		= FALSE;		// 変更確認フラグ(期末現在高)
	BOOL	ufgSpDate	= FALSE;		// 変更確認フラグ(異動年月日)
	BOOL	ufgRiyuu	= FALSE;		// 変更確認フラグ(異動事由)
	BOOL	ufgMsVal	= FALSE;		// 変更確認フラグ(取得(処分)価額)
	BOOL	ufgMsVal2	= FALSE;		// 変更確認フラグ(異動直前の帳簿価額)
	BOOL	ufgInvNo	= FALSE;		// 変更確認フラグ(登録番号(法人番号))
	BOOL	ufgAdName1	= FALSE;		// 変更確認フラグ(売却(購入)先の名称(氏名) 上段)
	BOOL	ufgAdName2	= FALSE;		// 変更確認フラグ(売却(購入)先の名称(氏名) 下段)
	BOOL	ufgAdAdd1	= FALSE;		// 変更確認フラグ(売却(購入)先の所在地(住所) 上段)
	BOOL	ufgAdAdd2	= FALSE;		// 変更確認フラグ(売却(購入)先の所在地(住所) 下段)
	BOOL	ufgSpDate2	= FALSE;		// 変更確認フラグ(売却物件の取得年月)
	BOOL	ufgFunc		= FALSE;		// 変更確認フラグ(特殊行)

	// レコード更新したことを示すフラグ
	BOOL	ufgRsUpdate	= FALSE;

	// テーブルアクセスクラス
	CdbUc071Koteishisan	rsKote( m_pDB );

	///////////////////////////////////////////////////////////////
	// 作業用変数へ 登録対象のフィールド値を代入
	tmp_Syurui	= inSaveData->m_Syurui;		// (種類・構造)
	tmp_Youto	= inSaveData->m_Youto;		// (用途)
	tmp_Area	= inSaveData->m_Area;		// (面積)
	tmp_Ad2Add1	= inSaveData->m_Ad2Add1;	// (物件の所在地) 上段
	tmp_Ad2Add2	= inSaveData->m_Ad2Add2;	// (物件の所在地) 下段
	tmp_Val		= inSaveData->m_Val;		// (期末現在高)
	tmp_SpDate	= inSaveData->m_SpDate;		// (異動年月日)
	tmp_Riyuu	= inSaveData->m_Riyuu;		// (異動事由)
	tmp_MsVal	= inSaveData->m_MsVal;		// (取得(処分)価額)
	tmp_MsVal2	= inSaveData->m_MsVal2;		// (異動直前の帳簿価額)
	tmp_InvNo	= inSaveData->m_InvNo;		// (登録番号(法人番号))
	tmp_AdName1	= inSaveData->m_AdName1;	// (売却(購入)先の名称(氏名) 上段)
	tmp_AdName2	= inSaveData->m_AdName2;	// (売却(購入)先の名称(氏名) 下段)
	tmp_AdAdd1	= inSaveData->m_AdAdd1;		// (売却(購入)先の所在地(住所) 上段)
	tmp_AdAdd2	= inSaveData->m_AdAdd2;		// (売却(購入)先の所在地(住所) 下段)
	tmp_SpDate2	= inSaveData->m_SpDate2;	// (売却物件の取得年月)
	
	///////////////////////////////////////////////////////////////
	// データ行か、空行かのチェック
	tmp_FgFunc = m_ReadData[inRow].m_FgFunc;
	if ( tmp_FgFunc <= ID_FGFUNC_DATA ) {	// データ行 または、空行なら
		// コンポーネントをチェックして、データがあるかを調べる
		// (仮変数に入れた値でチェックしてもいいのかも？)

		if ((tmp_Syurui == "")				// 画面上のデータ格納用(種類・構造)
			&& (tmp_Youto == "")			// 画面上のデータ格納用(用途)
			&& (tmp_Area == "")				// 画面上のデータ格納用(面積)
			&& (tmp_Ad2Add1 == "")			// 画面上のデータ格納用(物件の所在地) 上段
			&& (tmp_Ad2Add2 == "")			// 画面上のデータ格納用(物件の所在地) 下段
			&& (tmp_Val == "")				// 画面上のデータ格納用(期末現在高)
			&& (tmp_SpDate == 0)			// 画面上のデータ格納用(異動年月日)
			&& (tmp_Riyuu == "")			// 画面上のデータ格納用(異動事由)
			&& (tmp_MsVal == "")			// 画面上のデータ格納用(取得(処分)価額)
			&& (tmp_MsVal2 == "")			// 画面上のデータ格納用(異動直前の帳簿価額)
			&& (tmp_InvNo == "")			// 画面上のデータ格納用(登録番号)
			&& (tmp_AdName1 == "")			// 画面上のデータ格納用(売却(購入)先の名称(氏名) 上段)
			&& (tmp_AdName2 == "")			// 画面上のデータ格納用(売却(購入)先の名称(氏名) 下段)
			&& (tmp_AdAdd1 == "")			// 画面上のデータ格納用(売却(購入)先の所在地(住所) 上段)
			&& (tmp_AdAdd2 == "")			// 画面上のデータ格納用(売却(購入)先の所在地(住所) 下段)
			&& (tmp_SpDate2 == 0)) {		// 画面上のデータ格納用(売却物件の取得年月)
			// 空行なら
			tmp_FgFunc = ID_FGFUNC_NULL;
		}
		else {
			// データが入っていれば
			tmp_FgFunc = ID_FGFUNC_DATA;
		}
	}

	///////////////////////////////////////////////////////////////
	// 更新状況のチェック
	//(1) 種類・構造
	if (tmp_Syurui != m_ReadData[inRow].m_Syurui) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgUpToku	= TRUE;		// 全体(特殊行)
		ufgSyurui	= TRUE;		// 種類・構造用
	}

	//(2) 用途
	if (tmp_Youto != m_ReadData[inRow].m_Youto) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgYouto	= TRUE;		// 用途用
	}

	//(3) 面積
	if (tmp_Area != m_ReadData[inRow].m_Area) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgArea		= TRUE;		// 面積用
	}

	//(4) 物件の所在地 上段
	if (tmp_Ad2Add1 != m_ReadData[inRow].m_Ad2Add1) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgAd2Add1	= TRUE;		// 物件の所在地用 上段
	}

	//(5) 物件の所在地 下段
	if (tmp_Ad2Add2 != m_ReadData[inRow].m_Ad2Add2) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgAd2Add2	= TRUE;		// 物件の所在地用 下段
	}

	//(6) 期末現在高
	if (tmp_Val != m_ReadData[inRow].m_Val) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgVal		= TRUE;		// 期末現在高用
	}

	//(7) 異動年月日
	if (tmp_SpDate != m_ReadData[inRow].m_SpDate) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgSpDate	= TRUE;		// 異動年月日用
	}

	//(8) 異動事由
	if (tmp_Riyuu != m_ReadData[inRow].m_Riyuu) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgRiyuu	= TRUE;		// 異動事由用
	}

	//(9) 取得(処分)価額
	if (tmp_MsVal != m_ReadData[inRow].m_MsVal) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgMsVal	= TRUE;		// 取得(処分)価額用
	}

	//(10) 異動直前の帳簿価額
	if (tmp_MsVal2 != m_ReadData[inRow].m_MsVal2) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgMsVal2	= TRUE;		// 異動直前の帳簿価額用
	}

	//(11) 登録番号
	if(tmp_InvNo != m_ReadData[inRow].m_InvNo)	{
		ufgUpData = TRUE;		// 全体(データ行/空行)
		ufgInvNo = TRUE;		// 登録番号用
	}

	//(12) 売却(購入)先の名称(氏名) 上段
	if (tmp_AdName1 != m_ReadData[inRow].m_AdName1) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgAdName1	= TRUE;		// 売却(購入)先の名称(氏名) 上段用
	}

	//(13) 売却(購入)先の名称(氏名) 下段
	if (tmp_AdName2 != m_ReadData[inRow].m_AdName2) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgAdName2	= TRUE;		// 売却(購入)先の名称(氏名) 下段用
	}

	//(14) 売却(購入)先の所在地(住所) 上段
	if (tmp_AdAdd1 != m_ReadData[inRow].m_AdAdd1) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgAdAdd1	= TRUE;		// 売却(購入)先の所在地(住所) 上段用
	}

	//(15) 売却(購入)先の所在地(住所) 下段
	if (tmp_AdAdd2 != m_ReadData[inRow].m_AdAdd2) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgAdAdd2	= TRUE;		// 売却(購入)先の所在地(住所) 下段用
	}

	//(16) 売却物件の取得年月
	if (tmp_SpDate2 != m_ReadData[inRow].m_SpDate2) {
		ufgUpData	= TRUE;		// 全体(データ行/空行)
		ufgSpDate2	= TRUE;		// 売却物件の取得年月用
	}

	//(17) 特殊行フラグ(基本的には、空行/データ行のチェックになるはず) 
	if (m_ReadData[inRow].m_FgFunc <= ID_FGFUNC_DATA) {
		// 元が空行orデータ行の場合、入力内容により空行orデータ行となる
		// 元が特殊行の場合、FgFuncは更新しない
		if (tmp_FgFunc != m_ReadData[inRow].m_FgFunc) {
			ufgUpData	= TRUE;		// 全体(データ行/空行)
			ufgFunc		= TRUE;		// ufgFunc用
		}
	}

	///////////////////////////////////////////////////////////////////
	// 1 レコード更新処理
	if ( tmp_FgFunc == ID_FGFUNC_DATA ) {	// データ行
		///////////////////////////////////////////////////////////////////
		// データ行であった場合の処理

		// 金融機関名称リストへの登録チェック
		// 「固定資産」では必要ない処理。

		// 科目について KnOrder に関連する、名称、シーケンスを取得
		// 「固定資産」では必要ない処理。

		// 更新処理
		if (ufgUpData == TRUE) {		// テーブルへ更新(1レコード更新)
			// ページ、行 で抽出し、必要なデータを更新
			rsKote.RequeryPageRow(inPage, inRow);

			if ( !rsKote.IsEOF() ) {		// レコードが取得できた(取得できないことはないはず)
				rsKote.Edit();

				if (ufgSyurui == TRUE) {	// 変更ありの場合(種類・構造)
					CStringArray strArray;
					m_clsFunc.StrDivision( tmp_Syurui, &strArray, 2, TRUE, TRUE );
					rsKote.m_Syurui		= m_clsFunc.StrDocking( &strArray );
				}

				if (ufgYouto == TRUE) {		// 変更ありの場合(用途)
					CStringArray strArray;
					m_clsFunc.StrDivision( tmp_Youto, &strArray, 2, TRUE, TRUE );
					rsKote.m_Youto		= m_clsFunc.StrDocking( &strArray );
				}

				if (ufgArea == TRUE) {		// 変更ありの場合(面積)
					rsKote.m_Area	= tmp_Area;
				}

				if (ufgAd2Add1 == TRUE) {	// 変更ありの場合(物件の所在地) 上段
					rsKote.m_Ad2Add1	= m_clsFunc.DeleteRightSpace( tmp_Ad2Add1 );
				}

				if (ufgAd2Add2 == TRUE) {	// 変更ありの場合(物件の所在地) 下段
					rsKote.m_Ad2Add2	= m_clsFunc.DeleteRightSpace( tmp_Ad2Add2 );
				}

				if (ufgVal == TRUE) {		// 変更ありの場合(期末現在高)
					rsKote.m_Val	= tmp_Val;
				}

				if (ufgSpDate == TRUE) {	// 変更ありの場合(異動年月日)
					rsKote.m_SpDate	= tmp_SpDate;
				}

				if (ufgRiyuu == TRUE) {		// 変更ありの場合(異動事由)
					CStringArray strArray;
					m_clsFunc.StrDivision( tmp_Riyuu, &strArray, 2, TRUE, TRUE );
					rsKote.m_Riyuu		= m_clsFunc.StrDocking( &strArray );
				}

				if (ufgMsVal == TRUE) {		// 変更ありの場合(取得(処分)価額)
					rsKote.m_MsVal	= tmp_MsVal;
				}

				if (ufgMsVal2 == TRUE) {	// 変更ありの場合(異動直前の帳簿価額)
					rsKote.m_MsVal2	= tmp_MsVal2;
				}

				if(ufgInvNo == TRUE)	{	// 変更ありの場合(登録番号)
					rsKote.m_InvNo = tmp_InvNo;
				}

				if (ufgAdName1 == TRUE || ufgAdName2 == TRUE || ufgInvNo == TRUE) {	// 変更ありの場合(売却(購入)先の名称(氏名))
					// 末尾スペース削除後に空行削除処理
					tmp_AdName1 = m_clsFunc.DeleteRightSpace( tmp_AdName1 );
					tmp_AdName2 = m_clsFunc.DeleteRightSpace( tmp_AdName2 );
					m_clsFunc.DeleteNullRow( &tmp_AdName1, &tmp_AdName2 );
					rsKote.m_AdName1	= tmp_AdName1;
					rsKote.m_AdName2	= tmp_AdName2;
				}

				if (ufgAdAdd1 == TRUE || ufgInvNo == TRUE) {	// 変更ありの場合(売却(購入)先の所在地(住所) 上段)
					rsKote.m_AdAdd1	= m_clsFunc.DeleteRightSpace( tmp_AdAdd1 );
				}

				if (ufgAdAdd2 == TRUE || ufgInvNo == TRUE) {	// 変更ありの場合(売却(購入)先の所在地(住所) 上段)
					rsKote.m_AdAdd2	= m_clsFunc.DeleteRightSpace( tmp_AdAdd2 );
				}

				if (ufgSpDate2 == TRUE) {	// 変更ありの場合(売却物件の取得年月)
					rsKote.m_SpDate2	= tmp_SpDate2;
				}

				// 特殊行フラグは、強制的に書き込む
				rsKote.m_FgFunc		= tmp_FgFunc;

				rsKote.Update();
				ufgRsUpdate = TRUE;		// レコード更新したので ON
			}
		}

	}
	else if ( tmp_FgFunc == ID_FGFUNC_NULL ) {	// 空行
		///////////////////////////////////////////////////////////////////
		// 空行の場合の処理

		// 更新処理
		if (ufgUpData == TRUE) {		// テーブルへ更新(1レコード更新)
			// ページ、行 で抽出し、必要なデータを更新
			rsKote.RequeryPageRow(inPage, inRow);

			if ( !rsKote.IsEOF() ) {		// レコードが取得できた(取得できないことはないはず)
				rsKote.Edit();
				rsKote.m_FgFunc		= tmp_FgFunc;
				rsKote.m_KeiStr		= _T("");
				rsKote.m_NumGroup	= 0;
				rsKote.m_Syurui		= _T("");
				rsKote.m_Youto		= _T("");
				rsKote.m_Area		= _T("");
				rsKote.m_Ad2Add1	= _T("");
				rsKote.m_Ad2Add2	= _T("");
				rsKote.m_Val		= _T("");
				rsKote.m_SpDate		= 0;
				rsKote.m_Riyuu		= _T("");
				rsKote.m_MsVal		= _T("");
				rsKote.m_MsVal2		= _T("");
				rsKote.m_AdName1	= _T("");
				rsKote.m_AdName2	= _T("");
				rsKote.m_AdAdd1		= _T("");
				rsKote.m_AdAdd2		= _T("");
				rsKote.m_SpDate2	= 0;
				rsKote.m_InvNo		= _T("");		// 登録番号
				rsKote.Update();

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
			rsKote.RequeryPageRow(inPage, inRow);

			if ( !rsKote.IsEOF() ) {		// レコードが取得できた(取得できないことはないはず)
				rsKote.Edit();
				rsKote.m_Ad2Add1	= tmp_Ad2Add1;
				rsKote.m_Ad2Add2	= tmp_Ad2Add2;
				rsKote.Update();

				ufgRsUpdate = TRUE;		// レコード更新したので ON
			}
		}
	}

	///////////////////////////////////////////////////////////////////
	// 内部 1ページバッファも更新する
	int retFunc = FUNCTION_OK;				//戻り値：成功（変更なし)
	if (ufgRsUpdate == TRUE) {
		retFunc = FUNCTION_UPDATE;		//戻り値：成功（変更あり)
		if ( !rsKote.IsEOF() ) {		// レコードが取得できた(取得できないことはないはず)
			RecBufSetData( &rsKote );
		}
	}

	rsKote.Fin();

	return retFunc;
}

//********************************************************************************
//	「固定資産」テーブルのオブジェクトを生成し、ポインタを返す
//********************************************************************************
CdbUc000Common*	CfrmUc071KoteisisanI::virGetDataTableObject()
{
	// 「固定資産」テーブルのオブジェクトを生成し、ポインタを返す
	return (CdbUc000Common*)( new CdbUc071Koteishisan( m_pDB ) );
}

//********************************************************************************
//	行コピー処理
//	IN	なし
//	RET	なし
//********************************************************************************
void	CfrmUc071KoteisisanI::virRowButtonCopyProc()
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
void	CfrmUc071KoteisisanI::virGetCurrentInfo( CURRENT_INFO* uCurInfo )
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
void CfrmUc071KoteisisanI::SetInputMode( int nRow, long nMode )
{
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_071I_SYURUI ), nMode );		//	種類・構造
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_071I_YOUTO ), nMode );		//	用途
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_071I_AREA ), nMode );			//	面積
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_071I_AD2ADD1 ), nMode );		//	物件の所在地（上段）
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_071I_AD2ADD2 ), nMode );		//	物件の所在地（下段）
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_071I_VAL ), nMode );			//	期末現在高
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_071I_SPDATE ), nMode );		//	異動年月日
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_071I_RIYUU ), nMode );		//	異動事由
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_071I_MSVAL ), nMode );		//	取得価額
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_071I_MSVAL2 ), nMode );		//	異動直前の帳簿価額
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_071I_INVONOT ), nMode );		//	登録番号（T選択）
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_071I_INVONO ), nMode );		//	登録番号
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_071I_ADNAME1 ), nMode );		//	名称（上段）
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_071I_ADADD1 ), nMode );		//	所在地（上段）
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_071I_ADADD2 ), nMode );		//	所在地（下段）
	m_pDiag->ModifyItem( CmnGetControlIndex( nRow, ID_COL_071I_SPDATE2 ), nMode );		//	売却物件の取得年月
}

//**************************************************
//	各Diagの背景色(BackColor)に色を設定
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CfrmUc071KoteisisanI::SetDiagBackColor()
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


//***************************************************************************
//（2009.09.08 TS自動ログオフ対応）
//	Viewの終了処理
//		・デストラクタで行っていた処理をここに移動
//
//	IN		なし
//	RET		BOOL				TRUE=エラー，FALSE=正常
//***************************************************************************
BOOL CfrmUc071KoteisisanI::virEndProc(void)
{
	TRACE(_T("***** CfrmUc071KoteisisanI::virEndProc\n"));

	// ↓この処理でコントロールが破棄されているが、当関数が実行される時点では、
	// 　そのコントロールを持つView自体は生きている。
	// 　このため、コントロールに対する処理（セル位置の取得）でエラーになってしまう。
	// 　もともとはデストラクタにあったため、コントロールを破棄した後、
	// 　すぐにView自体も殺していたので問題なかった。
	// 　処理自体も特に意味がないので、コメントアウトする。

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

int CfrmUc071KoteisisanI::CursorControl(short nChar, int sw)
{
	int				rv=0;
	int				intCol=0;
	int				pos=0;
	char			key=0x00;

	// 現在の列番号を取得
	intCol = CmnGetControlCol();

	key = (char)GetKeyState(VK_SHIFT);				//	Shiftキー状態取得
	// 例外処理：移動前にComnoBoxで←/→キーが押下された場合は値を元に戻す
	if((nChar == VK_TAB && (key & 0x80) == 0x80) || (nChar == VK_LEFT) || (nChar == VK_RIGHT)) {
		// ComboBoxは選択内容を確定させない
		if(intCol == ID_COL_071I_INVONOT)	{	// 登録番号(T選択)
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

// ------------------------------------------------------------------------------------------------------------------------
// 全頁の電子申告の桁数チェックを行い、タイトルの文字色を変更する
// ------------------------------------------------------------------------------------------------------------------------
void CfrmUc071KoteisisanI::TitleColorChg(void)
{
	int					nCharOver[3]={0};	// 電子申告の文字数を超えたかどうかのサイン		1:電子申告の文字数を超えた
											// 様式⑦		[0]:種類・構造(10文字)				[1]:用途(10文字)
											//				[2]:異動事由(10文字)
	CRecordset			rs(m_pDB);
	CString				strSQL=_T("");
	CString				strData=_T("");

	// 種類・構造
	strSQL.Format(_T("select max(oversgn) from (select CASE when len(%s) > %d then 1 else 0 end as oversgn from uc_071_koteishisan where FgShow=%d) tbl1"),
		MakeReplaceSent(_T("Syurui")),UC_KAMOKU_LEN,ID_FGSHOW_OFF);
	rs.Open( CRecordset::forwardOnly, strSQL);
	rs.GetFieldValue((short)0, strData);
	rs.Close();
	nCharOver[0] = 0;
	if(_tstoi(strData) != 0) {
		nCharOver[0] = 1;
	}

	// 用途
	strSQL.Format(_T("select max(oversgn) from (select CASE when len(%s) > %d then 1 else 0 end as oversgn from uc_071_koteishisan where FgShow=%d) tbl1"),
		MakeReplaceSent(_T("Youto")),UC_071_YOUTO_LEN,ID_FGSHOW_OFF);
	rs.Open( CRecordset::forwardOnly, strSQL);
	rs.GetFieldValue((short)0, strData);
	rs.Close();
	nCharOver[1] = 0;
	if(_tstoi(strData) != 0) {
		nCharOver[1] = 1;
	}

	// 異動事由
	strSQL.Format(_T("select max(oversgn) from (select CASE when len(%s) > %d then 1 else 0 end as oversgn from uc_071_koteishisan where FgShow=%d) tbl1"),
		MakeReplaceSent(_T("Riyuu")),UC_071_RIYUU_LEN,ID_FGSHOW_OFF);
	rs.Open( CRecordset::forwardOnly, strSQL);
	rs.GetFieldValue((short)0, strData);
	rs.Close();
	nCharOver[2] = 0;
	if(_tstoi(strData) != 0) {
		nCharOver[2] = 1;
	}

	// 項目タイトル(ヘッダ部)の再設定
	if(nCharOver[0] == 1) 	CmnHeadDiagSetTitle(&m_HeadDiag,0,_T("種類･構造(10)"),1);
	else					CmnHeadDiagSetTitle(&m_HeadDiag,0,_T("種類･構造"),0);

	if(nCharOver[1] == 1)	CmnHeadDiagSetTitle(&m_HeadDiag,1,_T("用途(10)"),1);
	else					CmnHeadDiagSetTitle(&m_HeadDiag,1,_T("用途"),0);

	if(nCharOver[2] == 1)	CmnHeadDiagSetTitle(&m_HeadDiag,8,_T("異動事由(10)"),1);
	else					CmnHeadDiagSetTitle(&m_HeadDiag,8,_T("異動事由"),0);

	// 欄外ガイドの表示
	if(nCharOver[0] == 1 || nCharOver[1] == 1 || nCharOver[2] == 1)	{
		m_Guide1.ShowWindow(TRUE);
	}
	else {
		m_Guide1.ShowWindow(FALSE);
	}

	// 簡素化の時は、以下の項目は均等割りから中央寄せに変更する
	CmnHeadDiagChangeAlign(&m_HeadDiag, 7, 1);	// 異動年月日
	CmnHeadDiagChangeAlign(&m_HeadDiag, 8, 1);	// 異動事由
	CmnHeadDiagChangeAlign(&m_HeadDiag, 9, 1);	// 所得(処分)価額
	CmnHeadDiagChangeAlign(&m_HeadDiag, 10, 1);	// 異動直前の帳簿価額
	CmnHeadDiagChangeAlign(&m_HeadDiag, 12, 1);	// 売却(購入)先の名称(氏名)
	CmnHeadDiagChangeAlign(&m_HeadDiag, 13, 1);	// 売却(購入)先の所在地(住所)
}

// --------------------------------------------------------------------------------------------------------
//	「異動年月日 取得年月」列なら入力された日付をチェック
// --------------------------------------------------------------------------------------------------------
//	【引数】	short index	インデックス値
//				short sign	呼ばれた処理	0:PreTranslateMessage
//											1:EditOFFYoushikidiag1
//											2:TerminationYoushikidiag1
// --------------------------------------------------------------------------------------------------------
void CfrmUc071KoteisisanI::Date_ChkAndSet(short index, int col, short sign)
{
	short			sv_sel=0;
	int				sv_index=0;
	int				day=0,gen=0;
	DIAGRAM_DATA	diadata;
	CdateConvert	cDC;			// 日付変換クラス

	if(bG_Kanso != TRUE)										return;
	if(m_DataKakutei == TRUE)									return;
	if(col != ID_COL_071I_SPDATE && col != ID_COL_071I_SPDATE2)	return;

	if(m_uInfo.EditSign == -1)									return;

	// 「異動年月日 取得年月」列なら入力された日付を取得

	// ポジション取得
	m_uInfo.intCurCtlIndex = m_pDiag->GetPosition();
	sv_index = m_pDiag->GetPosition();

	// 行・列取得
	CmnGetControlPosition( &m_uInfo.intCurRow , &m_uInfo.intCurCol, &m_uInfo.OldCol );

	// 初期化
	m_clsFunc.DiagInit( &diadata );

	m_pDiag->GetData(m_uInfo.intCurCtlIndex , (LPUNKNOWN)&diadata );

	gen = diadata.data_day[0];
	if(col == ID_COL_071I_SPDATE)	{		// 異動年月日
		day = cDC.Convert(diadata.data_day[0], diadata.data_day[1], diadata.data_day[2], diadata.data_day[3]);
	}
	else if(col == ID_COL_071I_SPDATE2)	{	// 取得年月
		day = cDC.Convert(diadata.data_day[0], diadata.data_day[1], diadata.data_day[2], 0);
	}

	// EditOFFYoushikidiag1
	if(sign == 1)	{
		if(col == ID_COL_071I_SPDATE)	{		// 異動年月日
			if(m_uInfo.OldDate[0] > 0 || m_uInfo.OldGengo[0] > 0)	{
				// 初期化
				m_clsFunc.DiagInit( &diadata );

				if(m_uInfo.GenListSw == 0 && m_uInfo.OldDate[0] > 0 && day == 0)	{
					// 元号リストが表示されたことにより日付がクリアされるので、
					// 保存している日付を再セット
					m_uInfo.GenListSw++;

					cDC.Convert( m_uInfo.OldDate[0], diadata.data_day, DC_DATE_GWMD );
					if(cDC.m_nDate != 0)	m_pDiag->SetData(index, ( LPUNKNOWN )&diadata);
					else					m_pDiag->DataClear(index, TRUE);
				}
				else if(m_uInfo.GenListSw > 0)	{
					if(gen == 0 || m_uInfo.EditSign == 0)	{
						m_uInfo.OldDate[0] = 0;
						m_uInfo.OldGengo[0] = 0;
						m_uInfo.GenListSw = 0;

						m_pDiag->DataClear(index, TRUE);
					}
					else if(m_uInfo.OldDate[0] != day)	{
						m_uInfo.OldDate[0] = 0;
						m_uInfo.OldGengo[0] = 0;
						m_uInfo.GenListSw = 0;

						// 日付、もしくは元号が変更された
						cDC.Convert(day, diadata.data_day, DC_DATE_GWMD);
						if(cDC.m_nDate != 0)	m_pDiag->SetData(index, (LPUNKNOWN)&diadata);
						else					m_pDiag->DataClear(index, TRUE);
					}
				}
			}
		}
		else if(col == ID_COL_071I_SPDATE2)	{	// 取得年月
			if(m_uInfo.OldDate[1] > 0 || m_uInfo.OldGengo[1] > 0)	{
				// 初期化
				m_clsFunc.DiagInit( &diadata );

				if(m_uInfo.GenListSw == 0 && m_uInfo.OldDate[1] > 0 && day == 0)	{
					// 元号リストが表示されたことにより日付がクリアされるので、
					// 保存している日付を再セット
					m_uInfo.GenListSw++;

					cDC.Convert( m_uInfo.OldDate[1], diadata.data_day, DC_DATE_GWM );
					if(cDC.m_nDate != 0)	m_pDiag->SetData(index, ( LPUNKNOWN )&diadata);
					else					m_pDiag->DataClear(index, TRUE);
				}
				else if(m_uInfo.GenListSw > 0)	{
					if(gen == 0 || m_uInfo.EditSign == 0)	{
						m_uInfo.OldDate[1] = 0;
						m_uInfo.OldGengo[1] = 0;
						m_uInfo.GenListSw = 0;

						m_pDiag->DataClear(index, TRUE);
					}
					else if(m_uInfo.OldDate[1] != day)	{
						m_uInfo.OldDate[1] = 0;
						m_uInfo.OldGengo[1] = 0;
						m_uInfo.GenListSw = 0;

						// 日付、もしくは元号が変更された
						cDC.Convert(day, diadata.data_day, DC_DATE_GWM);
						if(cDC.m_nDate != 0)	m_pDiag->SetData(index, (LPUNKNOWN)&diadata);
						else					m_pDiag->DataClear(index, TRUE);
					} 
				}
			}
		}
	}
}

LRESULT CfrmUc071KoteisisanI::OnUserReDrawView(WPARAM wParam, LPARAM lParam)
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
void CfrmUc071KoteisisanI::virDiagGetHjnData(CDatabase* pDB, HJN_REC_NEW* phjnRec)
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
	intIndex = CmnGetControlIndex(nRow, ID_COL_071I_INVONO);
	cst = m_clsFunc.DiagGetString(m_pDiag, intIndex);
	if(cst.IsEmpty() == FALSE)	{
		memset(buf, 0, sizeof(buf));
		sprintf_s(buf, _T("%s"), cst);
		memcpy(phjnRec->wHjnNo1, buf, 13);
	}

	// 名称1(氏名)
	intIndex = CmnGetControlIndex(nRow, ID_COL_071I_ADNAME1);
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
	intIndex = CmnGetControlIndex(nRow, ID_COL_071I_ADADD1);
	cst = m_clsFunc.DiagGetString(m_pDiag, intIndex);
	cst.TrimRight();
	if(cst.IsEmpty() == FALSE)	{
		memset(buf, 0, sizeof(buf));
		sprintf_s(buf, _T("%s"), cst);
		memcpy(phjnRec->wAdrs1, buf, strlen(buf));
	}

	// 所在地2(住所)
	intIndex = CmnGetControlIndex(nRow, ID_COL_071I_ADADD2);
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
void	CfrmUc071KoteisisanI::virDiagSetHjnData(int pSw, CString rtInvNo, CString rtName, CString rtAdrs1, CString rtAdrs2, CString rtKana)
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
	//	//CmnDiagSetFocus( m_pDiag, m_uInfo.intCurRow, ID_COL_071I_INVONO );
	//	return;
	//}
	//else if(ret == 1)	{	// 未登録
// 修正No.168361,168369,168371,168378 del <--
		// 登録番号（T選択）
		intIndex = CmnGetControlIndex(m_uInfo.intCurRow, ID_COL_071I_INVONOT);
		CmnDiagSetCombo3(m_pDiag, intIndex, strInvoNo);

		// 登録番号
		InvoNoSplit(strInvoNo, &cs1, &cs2);
		intIndex = CmnGetControlIndex(m_uInfo.intCurRow, ID_COL_071I_INVONO);
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
int CfrmUc071KoteisisanI::virCallChkInvoiceNum(int pSw, int inPage, int inRow)
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
int CfrmUc071KoteisisanI::ChkInvoiceNum(CDatabase* pDB, int pSw, int inPage, int inRow, 
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
		intIndex = CmnGetControlIndex(inRow, ID_COL_071I_INVONOT);
		m_pDiag->GetData(intIndex, (LPUNKNOWN)&diadata);
		tsel = diadata.data_combo;
		// 登録番号（法人番号）
		intIndex = CmnGetControlIndex(inRow, ID_COL_071I_INVONO);
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
	//		m_pDiag->SetData(CmnGetControlIndex(inRow, ID_COL_071I_INVONOT), (LPUNKNOWN)&ddata);
	//		ddata.data_edit.Empty();
	//		m_pDiag->DataClear(CmnGetControlIndex(inRow, ID_COL_071I_INVONO), TRUE);
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
	//		m_pDiag->SetData(CmnGetControlIndex(inRow, ID_COL_071I_INVONOT), (LPUNKNOWN)&ddata);
	//		ddata.data_edit.Empty();
	//		m_pDiag->DataClear(CmnGetControlIndex(inRow, ID_COL_071I_INVONO), TRUE);
	//		gErrMsg  = _T("登録番号（法人番号）に誤りがあります。\r\n誤った番号は保存されません。\r\n\r\n");
	//		gErrMsg += _T("登録番号（法人番号）を再入力してください。");
	//		ICSMessageBox(gErrMsg, MB_ICONSTOP | MB_OK, 0, 0, this);
	//		return(-1);
	//	}

	//	// ※法人検索の表示や、データの取得等は今回取りやめになったが、今後対応するかもしれないので、コメントは残す（23/11/28）
	//	//if(m_ReadData[inRow].m_InvNo != invono && (pName.IsEmpty() == FALSE || pAdrs1.IsEmpty() == FALSE || pAdrs2.IsEmpty() == FALSE)) {
	//	if(m_ReadData[inRow].m_InvNo != invono) {
	//		return(1);
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
				if(pWnd1 != NULL) {
					CWnd *pWnd2 = pWnd1->GetFocus();
					if((pWnd2 != NULL && pWnd2 != m_pDiag) || (pWnd2 == NULL))	{
						if(pSyzInvoice.CheckInvoiceNumber(invono2) == -1) {
							m_pDiag->DataClear(CmnGetControlIndex(inRow, ID_COL_071I_INVONO), TRUE);
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
					m_pDiag->SetData(CmnGetControlIndex(inRow, ID_COL_071I_INVONOT), (LPUNKNOWN)&ddata);
					// 登録番号
					m_clsFunc.DiagInit(&ddata);
					ddata.data_edit.Format(tmpzd.sel_name);
					m_pDiag->SetData(CmnGetControlIndex(inRow, ID_COL_071I_INVONO), (LPUNKNOWN)&ddata);
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
					//	m_pDiag->DataClear(CmnGetControlIndex(inRow, ID_COL_071I_INVONO), TRUE);
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
						m_pDiag->DataClear(CmnGetControlIndex(inRow, ID_COL_071I_INVONO), TRUE);
						m_InvnoErrFlg = 0;

						return(0);
					}
					else {
						m_InvnoErrFlg = 1;

						m_uInfo.intCurCol = ID_COL_071I_INVONO;

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
				//	m_pDiag->DataClear(CmnGetControlIndex(inRow, ID_COL_071I_INVONO), TRUE);
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
					m_pDiag->DataClear(CmnGetControlIndex(inRow, ID_COL_071I_INVONO), TRUE);
					m_InvnoErrFlg = 0;

					return(0);
				}
				else {
					m_InvnoErrFlg = 1;

					m_uInfo.intCurCol = ID_COL_071I_INVONO;

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

		// ※法人検索の表示や、データの取得等は今回取りやめになったが、今後対応するかもしれないので、コメントは残す（23/11/28）
		if(m_ReadData[inRow].m_InvNo != invono) {
			return(1);
		}
	}
// 修正No.168361,168369,168371,168378 add <--

	m_InvnoErrFlg = 0;		// 修正No.168443 add

	return(0);
}
// インボイス登録番号追加対応_23/09/11 add <--
