
#include "stdafx.h"
#include "UCHIWAKE.h"
#include "frmUc000Common.h"
#include "CopyOwnData.h"
// midori 160610 add -->
#include "dlgReference2.h"
// midori 160610 add <--
// midori 180101 add -->
#include "dlgPrintHyoshi.h"
// midori 180101 add <--
// midori 190505 add -->
#include "dlgIkkatuConfirmation.h"
#include "dlgPrintConfirmation.h"
#include "dlgPrintConfirmation2.h"
// midori 20/10/08u del -->
//#include "dlgPrintConfirmation3.h"
// midori 20/10/08u del <--
// midori 190505 add <--
// midori 156950 add -->
#include "dlgPrintConfirmation4.h"
// midori 156950 add <--

// 内訳書インポートモジュール
//#include "UcFunctionCommon.h"
// midori 180104 del -->
//// midori 150894 add -->
//#include "..\..\UCHIIMP\UCHIIMP\UcCsvCommon.h"
//// midori 150894 add <--
//#include "..\..\UCHIIMP\UCHIIMP\DBUchiImpDEF.h"
//#include "..\..\UCHIIMP\UCHIIMP\DBUchiImp8.h"
// midori 180104 del <--
// midori 180104 add -->
#include "..\UCHIIMP\UcCsvCommon.h"
#include "..\UCHIIMP\DBUchiImpDEF.h"
#include "..\UCHIIMP\DBUchiImp8.h"
// midori 180104 add <--
// 14-1821_15-1030 add -->
#include "dbUc021UketoritegataEx.h"
// 14-1821_15-1030 add <--
#include "dbUc081SiharaitegataEx.h"		// 改良No.21-0086,21-0529 add

// 158063 del -->
//// 20-0873 add -->
//#include "dlgTorihikiAmount1.h"
//#include "dlgTorihikiAmount2.h"
//// 20-0873 add <--
// 158063 del <--

// midori UC_0015 del -->
//// midori 180104 add -->
//extern	int		bTantai;	// TRUE:単体マスター
//// midori 180104 add <--
// midori UC_0015 del <--

// 改良No.21-0086,21-0529 cor -->
//// midori 156189,156190,156191 add -->
//extern	int		KamokuRowEnableSgn(CDatabase* pDB, int pSw);
//// midori 156189,156190,156191 add <--
// ------------------------------
extern	int		KamokuRowEnableSgn(CDatabase* pDB, int pSw, int nFormSeq);
// 改良No.21-0086,21-0529 cor <--
// midori 190505 add -->
extern int ChkIkkatuSort(CDatabase* pDB, CdbUcInfSub* pfcRec);
// midori 190505 add <--

// インボイス登録番号追加対応_23/09/11 add -->
extern BOOL		bG_InvNo;			// インボイス番号使用サイン（決算期末が令和6年3月1日を超えている）
BOOL			m_Invoice;			// 改良No.21-0086,21-0529 add

// 修正No.168547 add -->
int			ZselWid;
int			ZselHei;
// 修正No.168547 add <--

int			ResizeSw;		// 修正No.168548,168552 add
int			ResizeClm;		// 修正No.168548,168552 add
// インボイス登録番号追加対応_23/09/11 add <--

// 24/04/22_ScoreLink add -->
int			g_nLastPage;
// 24/04/22_ScoreLink add <--

IMPLEMENT_DYNCREATE(CfrmUc000Common, ICSFormView)

BEGIN_MESSAGE_MAP(CfrmUc000Common, ICSFormView)
//	ON_MESSAGE(WM_END_VIEW, OnEndView)	// ユーザ定義メッセージ
	ON_COMMAND(ID_DATAIMPORT_MENU, &CfrmUc000Common::OnMenuDataImport)
	ON_COMMAND(ID_DATAEXPORT_MENU, &CfrmUc000Common::OnMenuDataExport)
// midori 160610 add -->
	ON_COMMAND(ID_REFERENCE2_MENU, &CfrmUc000Common::OnMenuReference2)
// midori 160610 add <--
// midori 180101 add -->
	ON_COMMAND(ID_PRINTHYOSHI_MENU, &CfrmUc000Common::OnMenuPrintHyoshi)
// midori 180101 add <--
// midori 180104 add -->
	ON_COMMAND(ID_FROMKOMON_MENU, &CfrmUc000Common::OnFromkomonMenu)
	ON_UPDATE_COMMAND_UI(ID_FROMKOMON_MENU, &CfrmUc000Common::OnUpdateFromkomonMenu)
// midori 180104 add <--
// midori 160612 add -->
	ON_UPDATE_COMMAND_UI(ID_DATAIMPORT_MENU, &CfrmUc000Common::OnUpdateMenuDataImport)
// midori 160612 add <--
	ON_MESSAGE(WM_YOUSHIKI_SELECT, DoButtonInsert)
// midori M-16090701 add -->
	ON_MESSAGE(WM_USER_FOCUSSET, OnUserFocusset)
// midori M-16090701 add <--
// midori 155879 add -->
	ON_MESSAGE(WM_USER_CONTROLPAGE, OnUserControlPage)
// midori 155879 add <--
// インボイス登録番号追加対応_23/11/06 add -->
	ON_MESSAGE(WM_ZMSEL_MESSAGE, OnShowZmselDialog)
	ON_WM_SIZE()
	ON_MESSAGE(WM_USER_FOCUSSET4, OnUserFocusset4)	// 修正No.168393 add
// インボイス登録番号追加対応_23/11/06 add <--
END_MESSAGE_MAP()

CfrmUc000Common::CfrmUc000Common()
	: ICSFormView(CfrmUc000Common::IDD)
{
// midori 161106 add -->
	m_F9OnSw=FALSE;			// TRUE:「新規作成」が選択されてF9を呼び出された
// midori 161106 add <--
// 要通知案件修正 21/07/06 add -->
	m_bInitColor = TRUE;
// 要通知案件修正 21/07/06 add <--
// 要通知案件 修正No.158229 add -->
	m_SaveBtnID = 0;
// 要通知案件 修正No.158229 add <--

// インボイス登録番号追加対応_23/11/21 add -->
	// ---- 登録番号一覧関連 ----->
	pZmSel = NULL;
	m_EndSyorityu = 0;
	// ---- 登録番号一覧関連 <-----
	m_TselFSize = 110;
// インボイス登録番号追加対応_23/11/21 add <--
}

CfrmUc000Common::CfrmUc000Common(UINT prmld)
	: ICSFormView(prmld)
{
// midori 161106 add -->
	m_F9OnSw=FALSE;			// TRUE:「新規作成」が選択されてF9を呼び出された
// midori 161106 add <--
// 要通知案件修正 21/07/06 add -->
	m_bInitColor = TRUE;
// 要通知案件修正 21/07/06 add <--
// 要通知案件 修正No.158229 add -->
	m_SaveBtnID = 0;
// 要通知案件 修正No.158229 add <--

// インボイス登録番号追加対応_23/11/21 add -->
	// ---- 登録番号一覧関連 ----->
	pZmSel = NULL;
	m_EndSyorityu = 0;
	// ---- 登録番号一覧関連 <-----
	m_TselFSize = 110;
// インボイス登録番号追加対応_23/11/21 add <--
}


CfrmUc000Common::~CfrmUc000Common()
{
	// 2009.09.08 TS自動ログオフ対応(virEndProcへ移動)
	TRACE(_T("***** CfrmUc000Common::~CfrmUc000Common\n"));

	// 処理なし
}

void CfrmUc000Common::DoDataExchange(CDataExchange* pDX)
{
	ICSFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PAGEBACK_BUTTON1, m_btnPageBack);
	DDX_Control(pDX, IDC_PAGENEXT_BUTTON1, m_btnPageNext);
	DDX_Control(pDX, IDC_PAGENEW_BUTTON1, m_btnPageNew);
	DDX_Control(pDX, IDC_PAGEINSERT_BUTTON1, m_btnPageInsert);
	DDX_Control(pDX, IDC_ROWCOPY_BUTTON1, m_btnRowCopy);
	DDX_Control(pDX, IDC_ROWPASTE_BUTTON1, m_btnRowPaste);
	DDX_Control(pDX, IDC_ROWINSERT_BUTTON1, m_btnRowInsert);
	DDX_Control(pDX, IDC_YOUSHIKINEXT_BUTTON1, m_btnYousikiNext);
	DDX_Control(pDX, IDC_YOUSHIKIBACK_BUTTON1, m_btnYousikiBack);
}

BOOL CfrmUc000Common::m_EndView;
BOOL CfrmUc000Common::m_ImportMode;
BOOL CfrmUc000Common::m_YoushikiSkip;
BOOL CfrmUc000Common::m_DispIkkatDlg;
BOOL CfrmUc000Common::m_DispYoushikiSelect;
BOOL CfrmUc000Common::m_ReturnMoveDown;
// midori 160612 add -->
BOOL CfrmUc000Common::m_DataKakutei;
// midori 160612 add <--
// midori 190301 add -->
extern	BOOL	bG_Kanso;
// midori 190301 add <--

BOOL CfrmUc000Common::m_RegAutoRef;
// midori 152135,152136 add -->
int CfrmUc000Common::m_BankSort;			// 参照ダイアログ::表示順（金融機関）	0:番号順	1:50音順
// midori 152745 del -->
//int CfrmUc000Common::m_AddressSort;		// 参照ダイアログ::表示順（取引先）		0:番号順	1:50音順
// midori 152745 del <--
// midori 152745 add -->
int CfrmUc000Common::m_AddressSort[2];		// 参照ダイアログ::表示順（取引先）		0:番号順	1:50音順	[0]:仕入先 [1]：得意先
// midori 152745 add <--
// midori 152135,152136 add <--
BOOL CfrmUc000Common::m_isF6;
// midori 190505 add -->
int CfrmUc000Common::m_DispFlg;
// midori 190505 add <--

void CfrmUc000Common::OnInitialUpdate()
{
	ICSFormView::OnInitialUpdate();
	ResizeParentToFit();

	// 各帳表クラスの初期処理実行フラグ初期化
	m_InitialFlag	= FALSE;
	m_ActiveNonFlag	= FALSE;
// 14-1821_15-1030 del -->
	//m_NullSyokeiRow = FALSE;
// 14-1821_15-1030 del <--
	m_ChangeFont	= FALSE;
	m_prtJob		= NULL;		// 印刷クラス初期化
	m_pDiag			= NULL;		// 明細ダイアグラム初期化
	m_EndView		= FALSE;
	m_ImportMode	= FALSE;
	m_isF6			= FALSE;
// midori 190505 add -->
	m_DispFlg		= 0;
// midori 190505 add <--
// midori 151405 add -->
	nG_MScroll		= 0;
	m_Titlefocus	= 0;
	m_Pagefocus		= 0;
	m_Headfocus		= 0;
// midori 151405 add <--
// midori 152134 add -->
	m_ComboSgn		= 0;
// midori 152134 add <--

	// owntblをuc_inf_subとuc_inf_mainにコピー
	// ここだと切替ごとに呼ばれてしまうが、コピーが走る時にm_YoushikiSkipに値が反映されない為
	CopyOwntblData();

	// 対象の帳表がオープン可?
	if ( CmnCheckTableInit() == TRUE ){
//		SetResize(IDC_YOUSHIKIDIAG1, ICS_RESIZE_BOTTOM);

		// 会社情報取得
		m_RenZvol = ((CUCHIWAKEApp*)AfxGetApp())->m_RenZvol;

		// アクティブイベントにてカーソルセットする／しないフラグ（TRUE:する、FALSE:しない）
		m_CursorSetFlag = TRUE;

		// 各帳表クラスの初期処理実行フラグ有効
		m_InitialFlag = TRUE;
	}
// No.200905 add -->
	//m_KmkRowSgn = KamokuRowEnableSgn(m_pDB,0);	// 改良No.21-0086,21-0529 del
// No.200905 add <--
}

#ifdef _DEBUG
void CfrmUc000Common::AssertValid() const
{
	ICSFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CfrmUc000Common::Dump(CDumpContext& dc) const
{
	ICSFormView::Dump(dc);
}
#endif
#endif //_DEBUG

/*// ［F1：ヘルプ］※ヘルプ機能実装時は有効
void CfrmUc000Common::OnButtonF1()
{
}
*/

// ［F2：前項目］
void CfrmUc000Common::OnButtonF2()
{
	//TRACE("PostMessage F2\n");
	PostMessage( WM_KEYDOWN, VK_F2 );
}

// ［F3：EXCEL出力］
void CfrmUc000Common::OnButtonF3()
{
// midori 181101 del -->
	//// 実行前の準備処理
	//CmnEventStartProc(ID_BUTTON_F3);

	//// 各様式の印刷オブジェクト取得
	//m_prtJob = CreatePrintObject(m_uInfo.intFormSeq);

	//// 印刷処理の実行
	//if (m_prtJob->SelectJobOne(((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub, ((CUCHIWAKEApp*)AfxGetApp())->m_strPath, m_uInfo.intFormSeq, m_NameKei) == FUNCTION_OK) {

	//	// EXCEL出力
	//	m_prtJob->JobFilePrintToCsv(m_uInfo);
	//	ICSFormView::DoFilePrintToCsv(&m_prtJob->m_icsPcsv);
	//}
	//else{

	//	// エラーメッセージは、CprtJobFunction内で表示する。
	//}
// midori 181101 del <--
}

// ［F4：プレビュ］
void CfrmUc000Common::OnButtonF4() 
{
	// 実行前の準備処理
	CmnEventStartProc(ID_BUTTON_F4);

// 修正No.158301 add -->
	int		rv = 0;
// 修正No.158301 add <--
// 修正No.158174 add -->
	// 日付にカーソルがある状態でF4:プレビューを実行すると、プレビュー画面内のF7:拡大F8:縮小がキーボードで押せなくなる現象を解消するため、
	// 一旦カーソル位置を退避させる
	int		col = -1;
	int		ret = 0;

	if(g_BatchPrt.IsBatchExec() == FALSE)	{	// 一括印刷でない
		m_PrnDlgCol = -1;
		m_PrnDlgRow = -1;
		//	日付入力項目にカーソルがセットされている時に退避させる
		ret = CmnTblDataToFirstCol(&col);
		if(ret == 1)	{
			m_PrnDlgCol = m_uInfo.intCurCol;
			m_PrnDlgRow = m_uInfo.intCurRow;
			CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, col);
		}
	}
// 修正No.158174 add <--

	// オプション設定→プレビュー、印刷時に様式設定ダイアログを表示するONで一括印刷ダイアログ表示
	if(m_DispIkkatDlg){
// 修正No.158301 del -->
		//OnMenuPrintAllSub(m_uInfo.intFormSeq);
// 修正No.158301 del <--
// 修正No.158301 add -->
		rv = OnMenuPrintAllSub(m_uInfo.intFormSeq);

		if(rv == 0)	{
			if(g_BatchPrt.IsBatchExec() == FALSE) {	// 一括印刷でない
				if(m_PrnDlgCol != (-1) && m_PrnDlgRow != (-1)) {
					CmnDiagSetFocus(m_pDiag, m_PrnDlgRow, m_PrnDlgCol);
				}
				m_PrnDlgCol = -1;
				m_PrnDlgRow = -1;
			}
		}
// 修正No.158301 add <--

		return;
	}	
// midori 190505 add -->
	else {
// midori 156950 del -->
		//if(SortPrintMsg(m_uInfo.intFormSeq,0) != 0)	{
// midori 156950 del <--
// midori 156950 add -->
		if(SortPrintMsg(m_uInfo.intFormSeq) != 0)	{
// midori 156950 add <--
			return;
		}
	}
// midori 190505 add <--

	// 各様式の印刷オブジェクト取得
	if(m_prtJob == NULL){
		m_prtJob = CreatePrintObject(m_uInfo.intFormSeq);
	}

	// 計の再計算
	//CmnTblCalcKei();

	// 印刷処理の実行
	if( m_prtJob->SelectJobOne( ((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub, ((CUCHIWAKEApp*)AfxGetApp())->m_strPath, m_uInfo.intFormSeq, m_NameKei ) == FUNCTION_OK){
		OnFilePrintPreview();
	}
	else{

		// エラーメッセージは、CprtJobFunction内で表示する。
	}

	// プレビュの終了処理は"OnEndPrinting()"にて行う
}

// ［F5：印刷］
void CfrmUc000Common::OnButtonF5()
{
	// 一括印刷中なら「OnFilePrint()」を実行する
	// これは、一括印刷の「印刷ダイアログ→プレビュー」で当イベント発生するため
	// 一括印刷中フラグにて制御を行う → 2007.08.20 ICS様からの指摘対応

	// 一括印刷からの戻りでない場合、各様式の印刷オブジェクト取得
	if( m_prtJob == NULL ){
		m_prtJob = CreatePrintObject(m_uInfo.intFormSeq);
	}
// 24/04/22_ScoreLink add -->
	g_nLastPage = 0;
// 24/04/22_ScoreLink add <--

	if (m_prtJob->m_bPrtAll == TRUE || m_prtJob->m_isF5FromPrev != FALSE){

		m_ActiveNonFlag	= TRUE;

		OnFilePrint();

		m_ActiveNonFlag	= FALSE;
		if(m_prtJob != NULL)	m_prtJob->m_isF5FromPrev = FALSE;	// OnFilePrint()後 m_prtJobがデリート済みの場合がある

		return;
	}

	// 実行前の準備処理
	CmnEventStartProc(ID_BUTTON_F5);

	// オプション設定→プレビュー、印刷時に様式設定ダイアログを表示するONで一括印刷ダイアログ表示
	if(m_DispIkkatDlg){
		OnMenuPrintAllSub(0);
		return;
	}
// midori 190505 add -->
	else {
		// [F5] → 印刷ダイアログからF4 → [閉じる] で再表示されないように
		if(m_DispFlg == 0)	{
// midori 156950 del -->
			//if(SortPrintMsg(m_uInfo.intFormSeq,0) != 0)	{
// midori 156950 del <--
// midori 156950 add -->
			if(SortPrintMsg(m_uInfo.intFormSeq) != 0)	{
// midori 156950 add <--
				return;
			}
		}
	}
// midori 190505 add <--

	// 印刷処理の実行
	if( m_prtJob->SelectJobOne( ((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub, ((CUCHIWAKEApp*)AfxGetApp())->m_strPath, m_uInfo.intFormSeq, m_NameKei ) == FUNCTION_OK){

		m_ActiveNonFlag	= TRUE;

		OnFilePrint();

		m_ActiveNonFlag	= FALSE;
	}
	else{

		// エラーメッセージは、CprtJobFunction内で表示する。
	}

	// 以下の処理はキャンセル時の対応。実際に印刷した場合"OnEndPrinting()"にて行う
	// カーソル移動
	// OnActivateViewイベントでフォーカス再設定するようにした
	//CmnDiagSetFocus( m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol );
}

// ［F6：項目複写］
void CfrmUc000Common::OnButtonF6() 
{
	// 実行前の準備処理
	CmnEventStartProc(ID_BUTTON_F6);

	CdbUc000Common*		rsData;			// テーブルアクセスクラス オブジェクトへのポインタ

	rsData = virGetDataTableObject();	// 引数に pdb 必要??
										// 多分、呼び出し先の GetDataTableObject() 内で
										// 参照可能なメンバー変数で pdb は持っていると思われる

	// １つ前のデータを取得
	rsData->RequeryNextBeforeRecord(m_uInfo.intCurPage, m_uInfo.intCurRow, -1, 1);
	if (rsData->IsEOF() == 0) {
		// 複写項目貼り付け
		virFncCellCopyProc(rsData);
	}

	rsData->Fin();
	delete rsData;

	// カーソル移動
	m_isF6 = TRUE;
	CmnDiagSetFocus( m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol );
	m_isF6 = FALSE;
}


// ［F7：削除］
void CfrmUc000Common::OnButtonF7()
{
	int		nDelLine;

	// 実行前の準備処理
	CmnEventStartProc(ID_BUTTON_F7);

	// 保管テーブル対応前の保管データを保管テーブルにコピー
	//if( IsRegExistHokanData() != FALSE ){
	//	CopyHokanDataToHokanTable(m_uInfo.intFormSeq);
	//}

// 修正No.157633_元号日付 del -->
//// 157382 add -->
//	int		colmsv=-1;
//	if(m_uInfo.intFormSeq == ID_FORMNO_021)	{
//		if(m_uInfo.intCurCol == ID_COL_021_SPDATE || m_uInfo.intCurCol == ID_COL_021_PMDATE)	{
//			colmsv = m_uInfo.intCurCol;
//			CmnDiagSetFocus(m_pDiag,m_uInfo.intCurRow,ID_COL_021_KAMOKU);
//		}
//	}
//// 157382 add <--
// 修正No.157633_元号日付 del <--
// 修正No.157633_元号日付 add -->
	int		colmsv=-1;
	int		colmset=-1;
	int		sign = 0;

// 修正No.158301 del -->
//	if(m_uInfo.intFormSeq == ID_FORMNO_021)	{
//		if(m_uInfo.intCurCol == ID_COL_021_SPDATE || m_uInfo.intCurCol == ID_COL_021_PMDATE)	{
//			colmset = ID_COL_021_KAMOKU;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_061)	{
//		if(bG_Kanso == TRUE && m_uInfo.intCurCol == ID_COL_061K_SPDATE)	{
//			colmset = ID_COL_061K_KUBUN;
//			sign = 1;
//		}
//		else if(bG_Kanso == FALSE && m_uInfo.intCurCol == ID_COL_061_SPDATE)	{
//			colmset = ID_COL_061_KUBUN;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_071)	{
//		if(m_uInfo.intCurCol == ID_COL_071_SPDATE || m_uInfo.intCurCol == ID_COL_071_SPDATE2)	{
//			colmset = ID_COL_071_SYURUI;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_081)	{
//		if(m_uInfo.intCurCol == ID_COL_081_SPDATE || m_uInfo.intCurCol == ID_COL_081_PMDATE)	{
//			colmset = ID_COL_081_ADNAME1;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_102)	{
//// 修正No.157883,157884 del -->
////		if(m_uInfo.intCurCol == ID_COL_102_SPDATE)	{
//// 修正No.157883,157884 del <--
//// 修正No.157883,157884 add -->
//		if(m_uInfo.intCurCol == ID_COL_102_SPDATE || m_uInfo.intCurCol == ID_COL_102_VAL)	{
//// 修正No.157883,157884 add <--
//			colmset = ID_COL_102_KNORDER;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_121)	{
//		if(m_uInfo.intCurCol == ID_COL_121_SPDATE)	{
//			colmset = ID_COL_121_KUBUN;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_151)	{
//		if(m_uInfo.intCurCol == ID_COL_151_SPDATE || m_uInfo.intCurCol == ID_COL_151_SPDATE2)	{
//			colmset = ID_COL_151_KUBUN;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_152)	{
//		if(m_uInfo.intCurCol == ID_COL_152_SPDATE)	{
//			colmset = ID_COL_152_ADNAME1;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_153)	{
//		if( m_uInfo.intCurCol == ID_COL_153_KYDATE || m_uInfo.intCurCol == ID_COL_153_KYDATE2 ||
//			m_uInfo.intCurCol == ID_COL_153_SPDATE || m_uInfo.intCurCol == ID_COL_153_SPDATE2 )	{
//			colmset = ID_COL_153_HDNAME;
//			sign = 1;
//		}
//	}
//	if(sign == 1)	{
//		colmsv = m_uInfo.intCurCol;
//		CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, colmset);
//	}
// 修正No.158301 del <--
// 修正No.158301 add -->
	sign = CmnTblDataToFirstCol(&colmset);
	if(sign == 1)	{
		colmsv = m_uInfo.intCurCol;
		CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, colmset);
	}
// 修正No.158301 add <--
// 修正No.157633_元号日付 add <--

// 修正No.168546 add -->
	// 並べて表示の際に、EditOnが走って削除メッセージに登録番号一覧が重なってしまうための対処
	if(InvNoClmChk() == TRUE)	{
		// ---- 登録番号一覧関連 ----->
		if(pZmSel && pZmSel->IsDisplay() != FALSE) {
			pZmSel->DialogOFF();
		}
		// ---- 登録番号一覧関連 <-----

		colmsv = m_uInfo.intCurCol;
		CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol-1);
	}
// 修正No.168546 add <--

	// 削除メイン処理（削除，一時保管）
	if (CmnTblDeleteMainProc( &nDelLine ) == FALSE) {
		// キャンセル時は、カーソル移動で抜ける
		// OnActivateViewイベントでフォーカス再設定するようにした
		//CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
// 157382 add -->
		if(colmsv != -1)	{
			CmnDiagSetFocus(m_pDiag,m_uInfo.intCurRow,colmsv);
		}
// 157382 add <--
		return;
	}

	// 頁と行の調整処理
	CmnTblDeleteFgFunc(ID_FGFUNC_PAGEKEI);											// 頁計の削除
	CmnTblDeleteFgFunc(ID_FGFUNC_RUIKEI);											// 累計の削除
	CmnTblUpdatePageNumRowNumAdd(m_uInfo.intCurPage, m_uInfo.intCurRow, nDelLine);	// 頁番号/行番号振り直し
	CmnTblUpdateEndPageSetting();													// 最終頁を調整する
	CmnTblInsertPageKeiRuiKei();													// 頁計/累計挿入（最終ページの削除or空行追加）

	// 小計/中計/頁計/累計の再計算
	CmnTblCalcKei();

// midori 156968 add -->
	// 先頭行が空行の場合削除する
	CmnTblUpdatePageRowAllResetMain(6);
// midori 156968 add <--

// midori 200101 del -->
	// 元号のコンボボックスを開いた状態で処理を行うと空白行ができる不具合の修正
	// InitialControlComboBox 内で SetComboFontSize を行っている様式(②、⑥)で起きていた
	// OnButtonF7で科目の文字数が変わることはないため m_ChangeFont をTRUEにする必要はないので
	// カットすることで対応とする
	//// 次の再表示によりダイアグラムが切り替わるときはTRUE
	//m_ChangeFont = TRUE;
// midori 200101 del <--

	// １ページ再表示
	virUpdateControlTblData();

// 157382 add -->
	if(colmsv != -1)	{
		m_uInfo.intCurCol = colmsv;
	}
// 157382 add <--

	// カーソル移動
	CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
}

// ［F8：挿入］
void CfrmUc000Common::OnButtonF8()
{
	// 実行前の準備処理
	CmnEventStartProc(ID_BUTTON_F8);

// midori 156968 add -->
	// 最終行（頁の最大行）が小計の場合、小計の下に空行を追加する
	InsertNull();
// midori 156968 add <--

	// 頁番号/行番号振り直し
	CmnTblUpdatePageNumRowNumAdd(m_uInfo.intCurPage, m_uInfo.intCurRow, 1);

	// 空行挿入
	CmnTblInsertNullLine(m_uInfo.intCurPage, m_uInfo.intCurRow, 1);

	// 頁と行の調整処理（挿入後処理）
	CmnTblDeleteFgFunc(ID_FGFUNC_PAGEKEI);					// 頁計の削除
	CmnTblDeleteFgFunc(ID_FGFUNC_RUIKEI);					// 累計の削除
	CmnTblUpdateEndPageSetting();							// 最終頁を調整する
	CmnTblInsertPageKeiRuiKei();							// 頁計/累計挿入（最終ページの削除or空行追加）
	
	// 小計/中計/頁計/累計の再計算
	CmnTblCalcKei();

// midori 156968 add -->
	// 先頭行が空行の場合削除する
	CmnTblUpdatePageRowAllResetMain(6);
// midori 156968 add <--

	// １ページ再表示
	virUpdateControlTblData();

	// カーソル移動
	CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
}

// ［F9：参照］
void CfrmUc000Common::OnButtonF9() 
{
	int				intRefType;		// 参照ダイアログの種別
	int				intCol;			// カレント列番号
	int				intGrSeq = 0;	// 
	long			lngItemSeq = 0;	// 
	int				intAdd1 = 0;
	int				intAdd2 = 0;
	int				intAdd1Col = 0;
	int				intAdd2Col = 0;
	int				intSetCol = 0;
// midori 152135,152136 add -->
// midori 152745 del -->
//	int				intSort = 0;		// 表示順
//	int				intSortSave = 0;	// ダイアログ表示前の表示順
// midori 152745 del <--
// midori 152745 add -->
	int				intSort[2];			// 表示順
	int				intSortSave[2];		// ダイアログ表示前の表示順
// midori 152745 add <--
// midori 152135,152136 add <--
	BOOL			bRefSel = FALSE;
	REFERENCE_INFO	udReference;
	CURRENT_INFO	uCurInfo;
// midori 152135,152136 add -->
	CdbUcInfMain	rsMain(m_pDB);		// uc_inf_mainテーブル
	CdbUcInfSub		rsSub(m_pDB);		// uc_inf_subテーブル
// midori 152135,152136 add <--

	//	初期化
	udReference.szName1.Empty();
	udReference.szName2.Empty();
	udReference.szAdd1.Empty();
	udReference.szAdd2.Empty();

	// 実行前の準備処理
	CmnEventStartProc(ID_BUTTON_F9);

	// 列位置を取得（m_uInfo.intCurColでも良いと思う）
	intCol = CmnGetControlCol();

	// カレント列から参照ダイアログの種別を取得
	intRefType = virFncReferenceGetType(intCol, &intGrSeq, &lngItemSeq, &intAdd1, &intAdd2,
		&intAdd1Col , &intAdd2Col , &udReference);

// インボイス登録番号追加対応_23/11/17 add -->
	// 登録番号(法人番号)の場合は、取引先と同じ動きをさせる
	if(intRefType == ID_DLGTYPE_INVOICENO)	{
		intRefType = ID_DLGTYPE_ADDRESS;
	}
// インボイス登録番号追加対応_23/11/17 add <--

	// 参照関連のテーブルがオープン可かチェック
	if ( CmnCheckTableReference( intRefType ) == TRUE ){
		// 参照ダイアログ表示
		CdlgReference dlgRef( this );

		// カレント行にインバースをかける
		virGetCurrentInfo(&uCurInfo);
// 修正No.157627 add -->
		CmnRowBeforeInverse(&uCurInfo);
// 修正No.157627 add <--
		CmnRowInverse(TRUE , &uCurInfo);

// midori 155327 add -->
		if (intRefType == ID_DLGTYPE_BANK) {
			// 自動登録をする場合、uc_lst_bank内の修復処理を行う
			if (m_RegAutoRef)	((CUCHIWAKEApp*)AfxGetApp())->BankConvert(m_pDB);
		}
		else if(intRefType == ID_DLGTYPE_ADDRESS) {
			// 自動登録をする場合、uc_lst_address内の修復処理を行う
			if(m_RegAutoRef)	((CUCHIWAKEApp*)AfxGetApp())->AddressConvert(m_pDB);
		}
// midori 155327 add <--

		// 2006.07.10 N-327
		//		if (dlgRef.ShowDialog(m_pDB, (EnumIdDlgType)intRefType, m_uInfo.intFormSeq, intGrSeq, lngItemSeq, &udReference) == ID_DLG_OK) {
		//			// カレント行のインバースを解除
		//			CmnRowInverse( FALSE , &uCurInfo );
		//			// 選択内容を表示
		//			virFncReferenceSetData(&dlgRef, intRefType, intCol);
		//		}
		//		else{
		//			// カレント行のインバースを解除
		//			CmnRowInverse( FALSE , &uCurInfo );
		//		}

		BOOL RegAutoRefOld = m_RegAutoRef;
// midori 152135,152136 del -->
//// midori 161106 del -->
////		if (dlgRef.ShowDialog(m_RegAutoRef, m_pDB, (EnumIdDlgType)intRefType, m_uInfo.intFormSeq, intGrSeq, lngItemSeq, &udReference) == ID_DLG_OK) {
//// midori 161106 del <--
//// midori 161106 add -->
//		if (dlgRef.ShowDialog(m_RegAutoRef, m_pDB, (EnumIdDlgType)intRefType, m_uInfo.intFormSeq, intGrSeq, lngItemSeq, &udReference, m_F9OnSw) == ID_DLG_OK) {
//// midori 161106 add <--
// midori 152135,152136 del <--
// midori 152745 del -->
//// midori 152135,152136 add -->
//		// 金融機関
//		if(intRefType == ID_DLGTYPE_BANK)			intSort = m_BankSort;
//		// 取引先
//		else if(intRefType == ID_DLGTYPE_ADDRESS)	intSort = m_AddressSort;
//		// 科目
//		else if(intRefType == ID_DLGTYPE_KAMOKU)	intSort = m_clsFunc.IsKamokuSort(m_uInfo.intFormSeq,m_pDB);
//		// ダイアログ表示前に表示順を保持する
//		intSortSave = intSort;
//		if (dlgRef.ShowDialog(m_RegAutoRef, intSort, m_pDB, (EnumIdDlgType)intRefType, m_uInfo.intFormSeq, intGrSeq, lngItemSeq, &udReference, m_F9OnSw) == ID_DLG_OK) {
//// midori 152135,152136 add <--
// midori 152745 del <--
// midori 152745 add -->
		intSort[0] = 0;
		intSort[1] = 0;
		// 金融機関
		if(intRefType == ID_DLGTYPE_BANK)			{
			intSort[0] = m_BankSort;
		}
		// 取引先
		else if(intRefType == ID_DLGTYPE_ADDRESS)	{
			intSort[0] = m_AddressSort[0];
			intSort[1] = m_AddressSort[1];
		}
		// 科目
		else if(intRefType == ID_DLGTYPE_KAMOKU)	{
			intSort[0] = m_clsFunc.IsKamokuSort(m_uInfo.intFormSeq,m_pDB);
		}
		// ダイアログ表示前に表示順を保持する
		intSortSave[0] = intSort[0];
		intSortSave[1] = intSort[1];

		if (dlgRef.ShowDialog(m_RegAutoRef, intSort, m_pDB, (EnumIdDlgType)intRefType, m_uInfo.intFormSeq, intGrSeq, lngItemSeq, &udReference, m_F9OnSw) == ID_DLG_OK) {
// midori 152745 add <--
			// 選択を行った場合、科目リストと画面上の値が一致していないため、
			// ただちに画面表示をせずに、画面上のデータを更新した上で、選択内容を表示する
			bRefSel = TRUE;
		}

		// カレント行のインバースを解除
		CmnRowInverse( FALSE , &uCurInfo );
// 修正No.157627 add -->
		// 保存した文字色に戻す
		CmnRowAfterInverse(&uCurInfo);
// 修正No.157627 add <--

		// 参照ダイアログの操作により、各帳表テーブルの更新が必要？
// midori 152135,152136,152137 del -->
		//if (dlgRef.m_fUpdate == 1) {
		//	// カレント行の登録
		//	virRecBufSaveData(m_uInfo.intCurPage, m_uInfo.intCurRow);

		//	// 各帳表テーブルの更新
		//	virFncReferenceUpdateTbl(intRefType);

		//	// 次の再表示によりダイアグラムが切り替わるときはTRUE
		//	m_ChangeFont = TRUE;
		//	// １ページ再表示
		//	virUpdateControlTblData();
		//}
// midori 152135,152136,152137 del <--
// midori 152135,152136,152137 add -->
		if (dlgRef.m_fUpdate == TRUE) {
			// 各帳表テーブルの更新
			virFncReferenceUpdateTbl(intRefType);
		}

// midori 152745 del -->
//		// 金融機関、取引先
//		if(intRefType == ID_DLGTYPE_BANK || intRefType == ID_DLGTYPE_ADDRESS)	{
//			// 表示順又は「取引先、金融機関を自動で登録する」が変更されていたら保存
//			if((intSort != intSortSave) || (RegAutoRefOld != m_RegAutoRef))	{
//				if(rsMain.Init() == DB_ERR_OK){
//					rsMain.Edit();
//					CCtrlMainGeneralVar cm(rsMain);
//					// 表示順
//					if(intSort != intSortSave)	{
//						// 金融期間
//						if(intRefType == ID_DLGTYPE_BANK)	{
//							if(intSort == 0)	cm.bBankSort = FALSE;
//							else				cm.bBankSort = TRUE;
//							m_BankSort = intSort;
//						}
//						// 取引先
//						else	{
//							if(intSort == 0)	cm.bAddressSort = FALSE;
//							else				cm.bAddressSort = TRUE;
//							m_AddressSort = intSort;
//						}
//					}
//					// 「取引先、金融機関を自動で登録する」
//					if(RegAutoRefOld != m_RegAutoRef)	{
//						// 取引先、金融機関を自動で登録する
//						if(m_RegAutoRef)		cm.bRegAutoRef = TRUE;
//						else					cm.bRegAutoRef = FALSE;
//					}
//
//					cm.Set(rsMain);
//					rsMain.Update();
//					rsMain.Fin();
//				}
//			}
//		}
//		// 科目
//		else if(intRefType == ID_DLGTYPE_KAMOKU)	{
//			// 表示順が変更されていたら保存
//			if(intSort != intSortSave)	{
//				CdbUcInfSub mfcSub(m_pDB);
//				mfcSub.RequeryFormSeq(m_uInfo.intFormSeq);
//				CCtrlSubGeneralVar cs(mfcSub);
//
//				// 表示順
//				if(intSort == 0)	cs.bKamokuSort = FALSE;
//				else				cs.bKamokuSort = TRUE;
//
//				mfcSub.Edit();
//				cs.Set(mfcSub);
//				mfcSub.Update();
//				mfcSub.Fin();
//			}
//		}
// midori 152745 del <--
// midori 152745 add -->
		// 金融機関、取引先
		if(intRefType == ID_DLGTYPE_BANK || intRefType == ID_DLGTYPE_ADDRESS)	{
			// 表示順又は「取引先、金融機関を自動で登録する」が変更されていたら保存
			if((intSort[0] != intSortSave[0]) || (intSort[1] != intSortSave[1]) || (RegAutoRefOld != m_RegAutoRef))	{
				if(rsMain.Init() == DB_ERR_OK){
					rsMain.Edit();
					CCtrlMainGeneralVar cm(rsMain);
					// 表示順
					// 金融期間
					if(intRefType == ID_DLGTYPE_BANK)	{
						if(intSort[0] != intSortSave[0])	{
							if(intSort[0] == 0)	cm.bBankSort = FALSE;
							else				cm.bBankSort = TRUE;
							m_BankSort = intSort[0];
						}
					}
					// 取引先
					else	{
						// 仕入先
						if(intSort[0] != intSortSave[0])	{
							if(intSort[0] == 0)	cm.bAddressSort = FALSE;
							else				cm.bAddressSort = TRUE;
							m_AddressSort[0] = intSort[0];
						}
						// 得意先
						if(intSort[1] != intSortSave[1])	{
							if(intSort[1] == 0)	cm.bAddressSort2 = FALSE;
							else				cm.bAddressSort2 = TRUE;
							m_AddressSort[1] = intSort[1];
						}
					}
					// 「取引先、金融機関を自動で登録する」
					if(RegAutoRefOld != m_RegAutoRef)	{
						// 取引先、金融機関を自動で登録する
						if(m_RegAutoRef)		cm.bRegAutoRef = TRUE;
						else					cm.bRegAutoRef = FALSE;
					}

					cm.Set(rsMain);
					rsMain.Update();
					rsMain.Fin();
				}
			}
		}
		// 科目
		else if(intRefType == ID_DLGTYPE_KAMOKU)	{
			// 表示順が変更されていたら保存
			if(intSort[0] != intSortSave[0])	{
				CdbUcInfSub mfcSub(m_pDB);
				mfcSub.RequeryFormSeq(m_uInfo.intFormSeq);
				CCtrlSubGeneralVar cs(mfcSub);

				// 表示順
				if(intSort[0] == 0)	cs.bKamokuSort = FALSE;
				else				cs.bKamokuSort = TRUE;

				mfcSub.Edit();
				cs.Set(mfcSub);
				mfcSub.Update();
				mfcSub.Fin();
			}
		}
// midori 152745 add <--

// midori UC_0006 del -->
		//if(dlgRef.m_fUpdate == TRUE || (intRefType == ID_DLGTYPE_KAMOKU && (intSort != intSortSave)))	{
// midori UC_0006 del <--
// midori UC_0006 add -->
		if(dlgRef.m_fUpdate == TRUE || (intRefType == ID_DLGTYPE_KAMOKU && (intSort[0] != intSortSave[0])))	{
// midori UC_0006 add <--
			// 選択ダイアログ内の項目に変更があった場合または、科目でソート順(番号順 OR 50音順)が変更された場合
			// 次の再表示によりダイアグラムが切り替わるときはTRUE
			m_ChangeFont = TRUE;

			// １ページ再表示
			virUpdateControlTblData();
		}
// midori 152135,152136,152137 add <--

		if (bRefSel == TRUE) {
			// 選択内容を表示：この内容は、行移動/Fncキーのタイミングで登録される
			virFncReferenceSetData(&dlgRef, intRefType, intCol);

			// フォーカスを移す列を判断
			if ( ( intAdd1Col > 0 ) && ( intAdd2Col > 0 ) ){
				if ( dlgRef.m_udAddress.m_AdAdd2 == "" ){
					intSetCol = intAdd1Col;
				}
				else{
					intSetCol = intAdd2Col;
				}
			}
			else{
				intSetCol = m_uInfo.intCurCol;
			}

// 修正No.168535 add -->
			// 登録番号追加版の場合、登録番号選択にカーソルがあれば、登録番号欄に移動させる
			if(bG_InvNo == TRUE)	{
				if(m_uInfo.intFormSeq == ID_FORMNO_021 && m_uInfo.intCurCol == ID_COL_021I_INVONOT)
					intSetCol = ID_COL_021I_INVONO;
				if(m_uInfo.intFormSeq == ID_FORMNO_031 && m_uInfo.intCurCol == ID_COL_031I_INVONOT)
					intSetCol = ID_COL_031I_INVONO;
				if(m_uInfo.intFormSeq == ID_FORMNO_041 && m_uInfo.intCurCol == ID_COL_041I_INVONOT)
					intSetCol = ID_COL_041I_INVONO;
				if(m_uInfo.intFormSeq == ID_FORMNO_042 && m_uInfo.intCurCol == ID_COL_042I_INVONOT)
					intSetCol = ID_COL_042I_INVONO;
				if(m_uInfo.intFormSeq == ID_FORMNO_071 && m_uInfo.intCurCol == ID_COL_071I_INVONOT)
					intSetCol = ID_COL_071I_INVONO;
				if(m_uInfo.intFormSeq == ID_FORMNO_081 && m_uInfo.intCurCol == ID_COL_081I_INVONOT)
					intSetCol = ID_COL_081I_INVONO;
				if(m_uInfo.intFormSeq == ID_FORMNO_091 && m_uInfo.intCurCol == ID_COL_091I_INVONOT)
					intSetCol = ID_COL_091I_INVONO;
				if(m_uInfo.intFormSeq == ID_FORMNO_101 && m_uInfo.intCurCol == ID_COL_101I_INVONOT)
					intSetCol = ID_COL_101I_INVONO;
				if(m_uInfo.intFormSeq == ID_FORMNO_121 && m_uInfo.intCurCol == ID_COL_121I_INVONOT)
					intSetCol = ID_COL_121I_INVONO;
				if(m_uInfo.intFormSeq == ID_FORMNO_151 && m_uInfo.intCurCol == ID_COL_151I_INVONOT)
					intSetCol = ID_COL_151I_INVONO;
				if(m_uInfo.intFormSeq == ID_FORMNO_152 && m_uInfo.intCurCol == ID_COL_152I_INVONOT)
					intSetCol = ID_COL_152I_INVONO;
				if(m_uInfo.intFormSeq == ID_FORMNO_153 && m_uInfo.intCurCol == ID_COL_153I_INVONOT)
					intSetCol = ID_COL_153I_INVONO;
				if(m_uInfo.intFormSeq == ID_FORMNO_161 && m_uInfo.intCurCol == ID_COL_161I_INVONOT)
					intSetCol = ID_COL_161I_INVONO;
				if(m_uInfo.intFormSeq == ID_FORMNO_162 && m_uInfo.intCurCol == ID_COL_162I_INVONOT)
					intSetCol = ID_COL_162I_INVONO;
				if(m_uInfo.intFormSeq >= ID_FORMNO_171 && m_uInfo.intFormSeq <= ID_FORMNO_1720 && m_uInfo.intCurCol == ID_COL_171I_INVONOT)
					intSetCol = ID_COL_171I_INVONO;
			}
// 修正No.168535 add <--

			// フォーカスをセット
			CmnDiagSetFocus( m_pDiag, m_uInfo.intCurRow, intSetCol );

			// カレットを末尾にセット→根本原因となる、virRecBufSaveData()内の相手先名称下段が空の時のデータ再セットを制御したためカレット制御は不要
			//CmnDiagSetCursorPos(m_pDiag, m_uInfo.intCurRow, intSetCol);
		}
// 修正No.158295 del -->
//// 修正No.158260 add -->
//		else	{
//			// フォーカスが消えてしまう不具合修正の為に、元の場所に戻す
//			// フォーカスをセット
//			CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
//		}
//// 修正No.158260 add <--
// 修正No.158295 del <--
// 修正No.158295 add -->
		else if(bRefSel == FALSE && dlgRef.m_fUpdate == TRUE)	{
			// フォーカスが消えてしまう不具合修正の為に、元の場所に戻す
			// フォーカスをセット
			CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
		}
// 修正No.158295 add <--

// midori 152135,152136 del -->
		//// 「取引先、金融機関を自動で登録する」が変更されていたら保存
		//if((!RegAutoRefOld && m_RegAutoRef) || (RegAutoRefOld && !m_RegAutoRef)){
		//	CdbUcInfMain	rsMain(m_pDB);
		//	if(rsMain.Init() == DB_ERR_OK){
		//		rsMain.Edit();
		//		CCtrlMainGeneralVar cm(rsMain);
		//		// 取引先、金融機関を自動で登録する
		//		if(m_RegAutoRef)		cm.bRegAutoRef = TRUE;
		//		else					cm.bRegAutoRef = FALSE;

		//		cm.Set(rsMain);
		//		rsMain.Update();
		//	}
		//	rsMain.Fin();
		//}
// midori 152135,152136 del <--
	}
}

// ［F11：1行上へ］
void CfrmUc000Common::OnButtonF11() 
{
	// 実行前の準備処理
	CmnEventStartProc(ID_BUTTON_F11);

	// 行入れ替え
	CmnTblChangeRecord(&m_uInfo.intCurPage, &m_uInfo.intCurRow, -1);

	// 小計/中計/頁計/累計の再計算
	CmnTblCalcKei();

// midori 200101 del -->
	// 元号のコンボボックスを開いた状態で処理を行うと空白行ができる不具合の修正
	// InitialControlComboBox 内で SetComboFontSize を行っている様式(②、⑥)で起きていた
	// OnButtonF11(F12)で科目の文字数が変わることはないため m_ChangeFont をTRUEにする必要はないので
	// カットすることで対応とする
	//// 次の再表示によりダイアグラムが切り替わるときはTRUE
	//m_ChangeFont = TRUE;
// midori 200101 del <--

	// １ページ再表示
	virUpdateControlTblData();

	// カーソル移動
	CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
}

// ［F12：１行下へ］
void CfrmUc000Common::OnButtonF12() 
{
	// 実行前の準備処理
	CmnEventStartProc(ID_BUTTON_F12);

	// 行入れ替え
	CmnTblChangeRecord(&m_uInfo.intCurPage, &m_uInfo.intCurRow, 1);

	// 小計/中計/頁計/累計の再計算
	CmnTblCalcKei();

// midori 200101 del -->
	// 元号のコンボボックスを開いた状態で処理を行うと空白行ができる不具合の修正
	// InitialControlComboBox 内で SetComboFontSize を行っている様式(②、⑥)で起きていた
	// OnButtonF11(F12)で科目の文字数が変わることはないため m_ChangeFont をTRUEにする必要はないので
	// カットすることで対応とする
	//// 次の再表示によりダイアグラムが切り替わるときはTRUE
	//m_ChangeFont = TRUE;
// midori 200101 del <--

	// １ページ再表示
	virUpdateControlTblData();

	// カーソル移動
	CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
}

// ［Home：住所検索 or 続柄・役職選択 or 法人検索］
void CfrmUc000Common::OnButtonHome() 
{
	int				intRefType;		// 参照ダイアログの種別
	int				intCol;			// カレント列番号
	int				intGrSeq = 0;	// 
	long			lngItemSeq = 0;	// 
	int				intAdd1 = 0;
	int				intAdd2 = 0;
	int				intAdd1Col = 0;
	int				intAdd2Col = 0;
	int				intSetCol = 0;
	REFERENCE_INFO	udReference;
	CURRENT_INFO	uCurInfo;

	//	初期化
	udReference.szName1.Empty();  
	udReference.szName2.Empty(); 
	udReference.szAdd1.Empty();
	udReference.szAdd2.Empty();

	// 実行前の準備処理
	CmnEventStartProc(ID_BUTTON_HOME);

	// 列位置を取得（m_uInfo.intCurColでも良いと思う）
	intCol = CmnGetControlCol();

	// カレント列から参照ダイアログの種別を取得
	intRefType = virFncReferenceGetType(intCol, &intGrSeq, &lngItemSeq, &intAdd1, &intAdd2,
		&intAdd1Col , &intAdd2Col , &udReference);

	// 参照関連のテーブルがオープン可かチェック
	if ( CmnCheckTableReference( intRefType ) == TRUE ){
		//if ( !(intRefType == ID_DLGTYPE_ICSADDSEARCH || intRefType == ID_DLGTYPE_YAKU_ZOKU)) {										// インボイス登録番号追加対応_23/11/17 del
		if( !(intRefType == ID_DLGTYPE_ICSADDSEARCH || intRefType == ID_DLGTYPE_YAKU_ZOKU || intRefType == ID_DLGTYPE_INVOICENO))	{	// インボイス登録番号追加対応_23/11/17 add
			return;
		}
	}

// インボイス登録番号追加対応_23/11/17 del -->
	//if( intRefType == ID_DLGTYPE_ICSADDSEARCH ){
// インボイス登録番号追加対応_23/11/17 del <--
// インボイス登録番号追加対応_23/11/17 add -->
	// ※法人番号検索を行うことは取りやめになったが、今後必要なことを考慮してif文はコメントで残しておく。（23/11/28）
	if(intRefType == ID_DLGTYPE_ICSADDSEARCH || intRefType == ID_DLGTYPE_INVOICENO) {
// インボイス登録番号追加対応_23/11/17 add <--
		// ICS様住所検索ダイアログ表示
		CdlgAddressSearch dlgAdd( this );

		// カレント行にインバースをかける
		virGetCurrentInfo(&uCurInfo);
// 修正No.157627 add -->
		CmnRowBeforeInverse(&uCurInfo);
// 修正No.157627 add <--
		CmnRowInverse( TRUE , &uCurInfo );

		if (dlgAdd.ShowDialog(intAdd1, intAdd2) == ID_DLG_OK) {
			// カレント行のインバースを解除
			CmnRowInverse( FALSE , &uCurInfo );
// 修正No.157627 add -->
			CmnRowAfterInverse(&uCurInfo);
// 修正No.157627 add <--
			// 選択内容を表示
			virFncReferenceSetICSData(&dlgAdd);

			// フォーカスを移す列を判断
			if ( ( intAdd1Col > 0 ) && ( intAdd2Col > 0 ) ){
				if ( dlgAdd.m_AdAdd2 == "" ){
					intSetCol = intAdd1Col;
				}
				else{
					intSetCol = intAdd2Col;
				}
			}
			else{
				intSetCol = m_uInfo.intCurCol;
			}

			// フォーカスをセット
			CmnDiagSetFocus( m_pDiag, m_uInfo.intCurRow, intSetCol );

		}
		else{
			// カレント行のインバースを解除
			CmnRowInverse( FALSE , &uCurInfo );
// 修正No.157627 add -->
			CmnRowAfterInverse(&uCurInfo);
// 修正No.157627 add <--
		}
	}
	else {
		CdlgYakuZokuSelect dlg(this);

		// カレント行にインバースをかける
		virGetCurrentInfo(&uCurInfo);
// 修正No.157627 add -->
		CmnRowBeforeInverse(&uCurInfo);
// 修正No.157627 add <--
		CmnRowInverse( TRUE , &uCurInfo );

		int nType = ID_KUBUN_SELECT_YAKUSYOKU;
// midori 154816 add -->
		int nIndex=0;
		CString strTmp = "";
// midori 154816 add <--

		if( intCol == ID_COL_141_KANKEI ){
			// 代表者との関係
			nType = ID_KUBUN_SELECT_KANKEI;
// midori 154816 add -->
			nIndex = CmnGetControlIndex(m_uInfo.intCurRow, ID_COL_141_KANKEI);
			strTmp = m_clsFunc.DiagGetString(m_pDiag, nIndex);
// midori 154816 add <--
		}
// midori 154827 add -->
		else	{
			// 役職名
			nType = ID_KUBUN_SELECT_YAKUSYOKU;
			nIndex = CmnGetControlIndex(m_uInfo.intCurRow, ID_COL_141_NAMEYAKU);
			strTmp = m_clsFunc.DiagGetString(m_pDiag, nIndex);
		}
// midori 154827 add <--

		// ダイアログ表示
// midori 154816 del -->
//		if( dlg.ShowDialog( nType ) == ID_DLG_OK ){
// midori 154816 del <--
// midori 154816 add -->
		if( dlg.ShowDialog(nType, strTmp) == ID_DLG_OK ){
// midori 154816 add <--
			// カレント行のインバースを解除
			CmnRowInverse( FALSE , &uCurInfo );
// 修正No.157627 add -->
			CmnRowAfterInverse(&uCurInfo);
// 修正No.157627 add <--
			// 選択内容を表示
			virFncReferenceSetICSData(&dlg, nType);

			// フォーカスをセット
			CmnDiagSetFocus( m_pDiag, m_uInfo.intCurRow, intSetCol );
		}
		else{
			// カレント行のインバースを解除
			CmnRowInverse( FALSE , &uCurInfo );
// 修正No.157627 add -->
			CmnRowAfterInverse(&uCurInfo);
// 修正No.157627 add <--
		}
	}
}

// ［Ins：様式選択］
void CfrmUc000Common::OnButtonInsert() 
{
	//DoButtonInsert(FROM_INSERT_KEY);
	SendMessage(WM_YOUSHIKI_SELECT, FROM_INSERT_KEY);
}

// 2007.04.27 終了処理の変更(内訳書特有→他アプリ共通)に伴い削除
// ［End：処理終了］終了イベント通知処理
///*void CfrmUc000Common::OnButtonEnd() 
//{
//	// 実行前の準備処理
//	CmnEventStartProc(ID_BUTTON_END);
//
//	// 終了イベント通知
//	//GetParentFrame()->PostMessage( WM_CLOSE );	// 自分への終了イベント通知
//	AfxGetMainWnd()->PostMessage( WM_CLOSE );		// 親への終了イベント通知
//}*/

/*
// ［F1：ヘルプ］ボタン監視※ヘルプ機能実装時は有効
void CfrmUc000Common::OnUpdateButtonF1(CCmdUI* pCmdUI) 
{
}
*/

// ［F2：前項目］ボタン監視
void CfrmUc000Common::OnUpdateButtonF2(CCmdUI* pCmdUI) 
{
	return;
}

/*// ［F3：CSV］ボタン監視※CSV機能実装時は有効
void CfrmUc000Common::OnUpdateButtonF3(CCmdUI* pCmdUI) 
{
}
*/
// ［F4：プレビュ］ボタン監視
void CfrmUc000Common::OnUpdateButtonF4(CCmdUI* pCmdUI) 
{
	return;
}

// ［F5：印刷］ボタン監視
void CfrmUc000Common::OnUpdateButtonF5(CCmdUI* pCmdUI) 
{
	return;
}

// ［F6：項目複写］ボタン監視
void CfrmUc000Common::OnUpdateButtonF6(CCmdUI* pCmdUI) 
{
// No.200905 add -->
	int				intGrSeq;
	long			lngItemSeq;
	int				intAdd1;
	int				intAdd2;
	int				intAdd1Col = 0;
	int				intAdd2Col = 0;
	int				nRefType = 0;
	REFERENCE_INFO	udReference;
// No.200905 add <--
	BOOL			bFncEnable = FALSE;
	CURRENT_INFO	uCurInfo;

// midori 160612 cor -->
	//// データ入力欄にフォーカスがある事
	//if (m_uInfo.intCurFocus == 1) {
	//	// 合計行①，②は操作不可。更に、空行orデータ行or一括集計金額行（手動）のみ可能
	//	if (m_uInfo.intCurCtlIndex < m_uInfo.intT1StartIndex) {
	//		// 対象行の特殊行フラグを取得
	//		virGetCurrentInfo(&uCurInfo);

	//		switch (uCurInfo.intFgFunc) {
	//		case ID_FGFUNC_NULL:				// 空行
	//		case ID_FGFUNC_DATA:				// データ行
	//		case ID_FGFUNC_IKKATUMANUAL:		// 一括集計金額行（手動）
	//			// １頁目の先頭行では操作不可
	//			if ((m_uInfo.intCurPage == 1) && (m_uInfo.intCurRow == 1)) {
	//			}
	//			else {
	//				// 操作可能
	//				bFncEnable = TRUE;
	//			}
	//			break;
	//		}
	//	}
	//}
	//// 各帳表による個別対応はここで実行する
	//virOnUpdateButtonF6(&bFncEnable);
// ---------------------
	// 入力データを確定するにチェック無し
	if(m_DataKakutei == FALSE)	{
		// データ入力欄にフォーカスがある事
		if (m_uInfo.intCurFocus == 1) {
			// 合計行①，②は操作不可。更に、空行orデータ行or一括集計金額行（手動）のみ可能
			if (m_uInfo.intCurCtlIndex < m_uInfo.intT1StartIndex) {
				// 対象行の特殊行フラグを取得
				virGetCurrentInfo(&uCurInfo);

				switch (uCurInfo.intFgFunc) {
				case ID_FGFUNC_NULL:				// 空行
				case ID_FGFUNC_DATA:				// データ行
				case ID_FGFUNC_IKKATUMANUAL:		// 一括集計金額行（手動）
					// １頁目の先頭行では操作不可
					if ((m_uInfo.intCurPage == 1) && (m_uInfo.intCurRow == 1)) {
					}
					else {
						// 操作可能
						bFncEnable = TRUE;
					}
					break;
// No.200905 add -->
				case ID_FGFUNC_IKKATUAUTO:			// 一括集計金額行（自動）
					// １頁目の先頭行では操作不可
					if ((m_uInfo.intCurPage == 1) && (m_uInfo.intCurRow == 1)) {
					}
					else {
// 改良No.21-0086,21-0529 cor -->
						//if(m_KmkRowSgn == 1) {
// ------------------------------
						if(KamokuRowEnableSgn(m_pDB, 0, m_uInfo.intFormSeq) == 1)	{
// 改良No.21-0086,21-0529 cor <--
							nRefType = virFncReferenceGetType(m_uInfo.intCurCol, &intGrSeq, &lngItemSeq, &intAdd1, &intAdd2,&intAdd1Col, &intAdd2Col, &udReference);
							if(nRefType == ID_DLGTYPE_KAMOKU) {
								// 操作可能
								bFncEnable = TRUE;
							}
						}
					}
					break;
// No.200905 add <--
				}
			}
		}
		// 各帳表による個別対応はここで実行する
		virOnUpdateButtonF6(&bFncEnable);
	}
// midori 160612 cor <--

	pCmdUI->Enable(bFncEnable);
}

// ［F7：前様式］ボタン監視
void CfrmUc000Common::OnUpdateButtonF7(CCmdUI* pCmdUI) 
{
	BOOL			bFncEnable = FALSE;
	CURRENT_INFO	uCurInfo;

// midori 160612 cor -->
	//// データ入力欄にフォーカスがある事
	//if (m_uInfo.intCurFocus == 1) {
	//	// 合計行①，②は操作不可。更に一括集計金額行（自動）も操作不可
	//	if (m_uInfo.intCurCtlIndex < m_uInfo.intT1StartIndex) {
	//		// 対象行の特殊行フラグを取得
	//		virGetCurrentInfo(&uCurInfo);

	//		switch (uCurInfo.intFgFunc) {
	//		case ID_FGFUNC_IKKATUAUTO:			// 一括集計金額行（自動）
	//			// 操作不可
	//			bFncEnable = FALSE;
	//			break;
	//		default:
	//			// 操作可能
	//			bFncEnable = TRUE;
	//			break;
	//		}
	//	}
	//}
// ---------------------
	// 入力データを確定するにチェック無し
	if(m_DataKakutei == FALSE)	{
		// データ入力欄にフォーカスがある事
		if (m_uInfo.intCurFocus == 1) {
			// 合計行①，②は操作不可。更に一括集計金額行（自動）も操作不可
			if (m_uInfo.intCurCtlIndex < m_uInfo.intT1StartIndex) {
				// 対象行の特殊行フラグを取得
				virGetCurrentInfo(&uCurInfo);

				switch (uCurInfo.intFgFunc) {
				case ID_FGFUNC_IKKATUAUTO:			// 一括集計金額行（自動）
					// 操作不可
					bFncEnable = FALSE;
					break;
				default:
					// 操作可能
					bFncEnable = TRUE;
					break;
				}
			}
		}
	}
// midori 160612 cor <--

	pCmdUI->Enable(bFncEnable);
}
//void CfrmUc000Common::OnUpdateButtonF7(CCmdUI* pCmdUI) 
//{
//	BOOL			bBack			= TRUE;
//
//	if( m_YoushikiSkip == FALSE ){
//		if( m_uInfo.intFormSeq == ID_FORMNO_011 ){	// 様式が始端(様式番号：①のときは「前の様式へ」ボタンをグレーアウト
//			bBack = FALSE;
//		}
//	}
//	else{
//		if( CmnIsExistData( TRUE ) == -1 ){		// 現在の様式より前の様式の中にデータ行がある様式が存在する？
//			bBack = FALSE;
//		}
//	}
//
//	pCmdUI->Enable(bBack);
//}

// ［F8：挿入］ボタン監視
void CfrmUc000Common::OnUpdateButtonF8(CCmdUI* pCmdUI) 
{
	BOOL			bFncEnable = FALSE;

// midori 160612 cor -->
	//// データ入力欄にフォーカスがある事
	//if (m_uInfo.intCurFocus == 1) {
	//	// 合計行①，②は操作不可。それ以外は操作可能。
	//	if (m_uInfo.intCurCtlIndex < m_uInfo.intT1StartIndex) {
	//		// 操作可能
	//		bFncEnable = TRUE;
	//	}
	//}
// ---------------------
	// 入力データを確定するにチェック無し
	if(m_DataKakutei == FALSE)	{
		// データ入力欄にフォーカスがある事
		if (m_uInfo.intCurFocus == 1) {
			// 合計行①，②は操作不可。それ以外は操作可能。
			if (m_uInfo.intCurCtlIndex < m_uInfo.intT1StartIndex) {
				// 操作可能
				bFncEnable = TRUE;
			}
		}
	}
// midori 160612 cor <--
	pCmdUI->Enable(bFncEnable);
}

// ［F9：参照］ボタン監視
void CfrmUc000Common::OnUpdateButtonF9(CCmdUI* pCmdUI) 
{
	BOOL			bFncEnable = FALSE;
	CURRENT_INFO	uCurInfo;
	int				intGrSeq;
	long			lngItemSeq;
	int				intAdd1;
	int				intAdd2;
	int				intAdd1Col = 0;
	int				intAdd2Col = 0;
	int				nRefType = 0;
	REFERENCE_INFO	udReference;
	
	//	初期化
	udReference.szName1.Empty();  
	udReference.szName2.Empty(); 
	udReference.szAdd1.Empty();
	udReference.szAdd2.Empty();

// midori 160612 cor -->
	//nRefType = virFncReferenceGetType(m_uInfo.intCurCol, &intGrSeq, &lngItemSeq, &intAdd1, &intAdd2,
	//										&intAdd1Col, &intAdd2Col, &udReference);

	//// 参照ダイアログの種別を取得：カレント列により参照可能の判断に使用する
	//if (nRefType == ID_DLGTYPE_NONE || nRefType == ID_DLGTYPE_ICSADDSEARCH || nRefType == ID_DLGTYPE_YAKU_ZOKU) {
	//	pCmdUI->Enable(FALSE);
	//	return;
	//}

	//// データ入力欄にフォーカスがある事
	//if (m_uInfo.intCurFocus == 1) {
	//	// 合計行①，②は操作不可。それ以外は操作可能。
	//	if (m_uInfo.intCurCtlIndex < m_uInfo.intT1StartIndex) {
	//		// 対象行の特殊行フラグを取得
	//		virGetCurrentInfo(&uCurInfo);

	//		switch (uCurInfo.intFgFunc) {
	//		case ID_FGFUNC_NULL:			// 空行
	//		case ID_FGFUNC_DATA:			// データ行
	//		case ID_FGFUNC_IKKATUMANUAL:	// 一括集計金額行（手動）
	//			// 操作可能
	//			bFncEnable = TRUE;
	//			break;
	//		}
	//	}
	//}
// ---------------------
	// 入力データを確定するにチェック無し
	if(m_DataKakutei == FALSE)	{
		nRefType = virFncReferenceGetType(m_uInfo.intCurCol, &intGrSeq, &lngItemSeq, &intAdd1, &intAdd2,
												&intAdd1Col, &intAdd2Col, &udReference);

		// 参照ダイアログの種別を取得：カレント列により参照可能の判断に使用する
		if (nRefType == ID_DLGTYPE_NONE || nRefType == ID_DLGTYPE_ICSADDSEARCH || nRefType == ID_DLGTYPE_YAKU_ZOKU) {
			pCmdUI->Enable(FALSE);
			return;
		}

// インボイス登録番号追加対応_23/11/17 add -->
		// 登録番号にカーソルがあるが、元々参照ダイアログを使用しない様式の場合はF9をディセーブルにする
		if(nRefType == ID_DLGTYPE_INVOICENO && intGrSeq == ID_ADDRESSGR_NONE) {
			pCmdUI->Enable(FALSE);
			return;
		}
// インボイス登録番号追加対応_23/11/17 add <--

		// データ入力欄にフォーカスがある事
		if (m_uInfo.intCurFocus == 1) {
			// 合計行①，②は操作不可。それ以外は操作可能。
			if (m_uInfo.intCurCtlIndex < m_uInfo.intT1StartIndex) {
				// 対象行の特殊行フラグを取得
				virGetCurrentInfo(&uCurInfo);

				switch (uCurInfo.intFgFunc) {
				case ID_FGFUNC_NULL:			// 空行
				case ID_FGFUNC_DATA:			// データ行
				case ID_FGFUNC_IKKATUMANUAL:	// 一括集計金額行（手動）
					// 操作可能
					bFncEnable = TRUE;
					break;
// No.200905 add -->
				case ID_FGFUNC_IKKATUAUTO:			// 一括集計金額行（自動）
// 改良No.21-0086,21-0529 cor -->
					//if(m_KmkRowSgn == 1) {
// ------------------------------
					if(KamokuRowEnableSgn(m_pDB, 0, m_uInfo.intFormSeq) == 1)	{
// 改良No.21-0086,21-0529 cor <--
						if(nRefType == ID_DLGTYPE_KAMOKU) {
							// 操作可能
							bFncEnable = TRUE;
						}
					}
					break;
// No.200905 add <--
				}
			}
		}
	}
// midori 160612 cor <--
	pCmdUI->Enable(bFncEnable);
}

// ［F11：１行上へ］ボタン監視
void CfrmUc000Common::OnUpdateButtonF11(CCmdUI* pCmdUI) 
{
	BOOL	bFncEnable = FALSE;

// midori 160612 cor -->
	//// データ入力欄にフォーカスがある事
	//if (m_uInfo.intCurFocus == 1) {
	//	// 合計行①，②は操作不可。更に１頁目の先頭行では操作不可。
	//	if (m_uInfo.intCurCtlIndex < m_uInfo.intT1StartIndex) {
	//		if ((m_uInfo.intCurPage == 1) && (m_uInfo.intCurRow == 1)) {
	//		}
	//		else {
	//			// 操作可能
	//			bFncEnable = TRUE;
	//		}
	//	}
	//}
// ---------------------
	// 入力データを確定するにチェック無し
	if(m_DataKakutei == FALSE)	{
		// データ入力欄にフォーカスがある事
		if (m_uInfo.intCurFocus == 1) {
			// 合計行①，②は操作不可。更に１頁目の先頭行では操作不可。
			if (m_uInfo.intCurCtlIndex < m_uInfo.intT1StartIndex) {
				if ((m_uInfo.intCurPage == 1) && (m_uInfo.intCurRow == 1)) {
				}
				else {
					// 操作可能
					bFncEnable = TRUE;
				}
			}
		}
	}
// midori 160612 cor <--
	pCmdUI->Enable(bFncEnable);
}

// ［F12：１行下へ］ボタン監視
void CfrmUc000Common::OnUpdateButtonF12(CCmdUI* pCmdUI) 
{
	BOOL	bFncEnable = FALSE;
	int		intTotalType = 0;
	int		intMaxRowTemp = m_uInfo.intRowMaxData;

// midori 160612 cor -->
	//// データ入力欄にフォーカスがある事
	//if (m_uInfo.intCurFocus == 1) {
	//	// 合計行①，②は操作不可。更に最終頁の最終データ行では操作不可。
	//	if (m_uInfo.intCurCtlIndex < m_uInfo.intT1StartIndex) {

	//		// 計設定とカレントページから、カレントページのデータ行数を取得
	//		intTotalType = CmnGetControlTotalType();
	//		if (m_uInfo.intRowKei == 1) {
	//			// 計あり帳表
	//			switch (intTotalType) {
	//			case ID_OUTKEI_BOTH:
	//				intMaxRowTemp = m_uInfo.intRowMaxData - 1;
	//				break;
	//			}
	//		}
	//		else {
	//			// 計なし帳表
	//			switch (intTotalType) {
	//			case ID_OUTKEI_RUIKEI:
	//			case ID_OUTKEI_BOTH:
	//				intMaxRowTemp = m_uInfo.intRowMaxData - 1;
	//				break;
	//			}
	//		}

	//		// 最終頁の最終データ行？
	//		if ((m_uInfo.intCurPage == m_uInfo.intMaxPage) && (m_uInfo.intCurRow == intMaxRowTemp)) {
	//		}
	//		else {
	//			// 操作可能
	//			bFncEnable = TRUE;
	//		}
	//	}
	//}
// ---------------------
	// 入力データを確定するにチェック無し
	if(m_DataKakutei == FALSE)	{
		// データ入力欄にフォーカスがある事
		if (m_uInfo.intCurFocus == 1) {
			// 合計行①，②は操作不可。更に最終頁の最終データ行では操作不可。
			if (m_uInfo.intCurCtlIndex < m_uInfo.intT1StartIndex) {

				// 計設定とカレントページから、カレントページのデータ行数を取得
				intTotalType = CmnGetControlTotalType();
				if (m_uInfo.intRowKei == 1) {
					// 計あり帳表
					switch (intTotalType) {
					case ID_OUTKEI_BOTH:
						intMaxRowTemp = m_uInfo.intRowMaxData - 1;
						break;
					}
				}
				else {
					// 計なし帳表
					switch (intTotalType) {
					case ID_OUTKEI_RUIKEI:
					case ID_OUTKEI_BOTH:
						intMaxRowTemp = m_uInfo.intRowMaxData - 1;
						break;
					}
				}

				// 最終頁の最終データ行？
				if ((m_uInfo.intCurPage == m_uInfo.intMaxPage) && (m_uInfo.intCurRow == intMaxRowTemp)) {
				}
				else {
					// 操作可能
					bFncEnable = TRUE;
				}
			}
		}
	}
// midori 160612 cor <--
	pCmdUI->Enable(bFncEnable);
}

// ［Home：住所検索］ボタン監視
void CfrmUc000Common::OnUpdateButtonHome(CCmdUI* pCmdUI)
{
	// フォーカスを持つインデックスによってHomeキーキャプションを変更する
	CMainFrame*  pFrame  = (CMainFrame*)AfxGetMainWnd();
// インボイス登録番号追加対応_23/11/17 del -->
	//if( m_uInfo.intFormSeq == ID_FORMNO_141 ){
	//	if( pFrame != NULL ){
	//		// ツールバー取得
	//		ICSToolBar	*pTool = pFrame->GetToolBar( 1 );			// 「１：プログラム」ツールバーを取得
	//		if( pTool != NULL ){
	//			// Homeキー取得
	//			CXTPControl *pCtrl = pTool->GetControl( 11 );	// 「11:Home 住所検索」メニューを取得
	//			if( pCtrl != NULL ){
	//				CString strHomeCaption = pCtrl->GetCaption();
	//				// 文字列変更
	//				if( m_uInfo.intCurCol == ID_COL_141_NAMEYAKU ){
	//					strHomeCaption = _T("Home 役職名設定");
	//				}
	//				else if( m_uInfo.intCurCol == ID_COL_141_KANKEI ){
	//					strHomeCaption = _T("Home 続柄設定");
	//				}
	//				else {
	//					strHomeCaption = _T("Home 住所検索");
	//				}
	//				// 文字列セット
	//				pCtrl->SetCaption( strHomeCaption );
	//				pCtrl->SetDescription( strHomeCaption );
	//				pCtrl->SetTooltip( strHomeCaption );
	//			}
	//		}
	//	}
	//}
// インボイス登録番号追加対応_23/11/17 del <--
// インボイス登録番号追加対応_23/11/17 add -->
	if(pFrame != NULL) {
		// ツールバー取得
		ICSToolBar	*pTool = pFrame->GetToolBar(1);			// 「１：プログラム」ツールバーを取得
		if(pTool != NULL) {
			// Homeキー取得
			CXTPControl *pCtrl = pTool->GetControl(11);	// 「11:Home 住所検索」メニューを取得
			if(pCtrl != NULL) {
				CString strHomeCaption = pCtrl->GetCaption();
				if(m_uInfo.intFormSeq == ID_FORMNO_141) {
					// 文字列変更
					if(m_uInfo.intCurCol == ID_COL_141_NAMEYAKU) {
						strHomeCaption = _T("Home 役職名設定");
					}
					else if(m_uInfo.intCurCol == ID_COL_141_KANKEI) {
						strHomeCaption = _T("Home 続柄設定");
					}
					else {
						strHomeCaption = _T("Home 住所検索");
					}
				}
				else {
					strHomeCaption = _T("Home 住所検索");
				}
				// 文字列セット
				pCtrl->SetCaption(strHomeCaption);
				pCtrl->SetDescription(strHomeCaption);
				pCtrl->SetTooltip(strHomeCaption);
			}
		}
	}
// インボイス登録番号追加対応_23/11/17 add <--

	BOOL			bFncEnable = FALSE;
	CURRENT_INFO	uCurInfo;
	int				intGrSeq;
	long			lngItemSeq;
	int				intAdd1;
	int				intAdd2;
	int				intAdd1Col = 0;
	int				intAdd2Col = 0;
	int				nRefType = 0;
	REFERENCE_INFO	udReference;
	
	//	初期化
	udReference.szName1.Empty();  
	udReference.szName2.Empty(); 
	udReference.szAdd1.Empty();
	udReference.szAdd2.Empty();

// midori 160612 cor -->
	//nRefType = virFncReferenceGetType(m_uInfo.intCurCol, &intGrSeq, &lngItemSeq, &intAdd1, &intAdd2,
	//										&intAdd1Col, &intAdd2Col, &udReference);

	//// 参照ダイアログの種別を取得：カレント列により参照可能の判断に使用する
	//if ( !(nRefType == ID_DLGTYPE_ICSADDSEARCH || nRefType == ID_DLGTYPE_YAKU_ZOKU)) {
	//	pCmdUI->Enable(FALSE);
	//	return;
	//}

	//// データ入力欄にフォーカスがある事
	//if (m_uInfo.intCurFocus == 1) {
	//	// 合計行①，②は操作不可。それ以外は操作可能。
	//	if (m_uInfo.intCurCtlIndex < m_uInfo.intT1StartIndex) {
	//		// 対象行の特殊行フラグを取得
	//		virGetCurrentInfo(&uCurInfo);

	//		switch (uCurInfo.intFgFunc) {
	//		case ID_FGFUNC_NULL:			// 空行
	//		case ID_FGFUNC_DATA:			// データ行
	//		case ID_FGFUNC_IKKATUMANUAL:	// 一括集計金額行（手動）
	//			// 操作可能
	//			bFncEnable = TRUE;
	//			break;
	//		}
	//	}
	//}
// ---------------------
	// 入力データを確定するにチェック無し
	if(m_DataKakutei == FALSE)	{
		nRefType = virFncReferenceGetType(m_uInfo.intCurCol, &intGrSeq, &lngItemSeq, &intAdd1, &intAdd2,
												&intAdd1Col, &intAdd2Col, &udReference);

		// 参照ダイアログの種別を取得：カレント列により参照可能の判断に使用する
// インボイス登録番号追加対応_23/11/17 del -->
		//if ( !(nRefType == ID_DLGTYPE_ICSADDSEARCH || nRefType == ID_DLGTYPE_YAKU_ZOKU)) {
// インボイス登録番号追加対応_23/11/17 del <--
// インボイス登録番号追加対応_23/11/17 add -->
		if ( !(nRefType == ID_DLGTYPE_ICSADDSEARCH || nRefType == ID_DLGTYPE_YAKU_ZOKU)) {
// インボイス登録番号追加対応_23/11/17 add <--
			pCmdUI->Enable(FALSE);
			return;
		}

		// データ入力欄にフォーカスがある事
		if (m_uInfo.intCurFocus == 1) {
			// 合計行①，②は操作不可。それ以外は操作可能。
			if (m_uInfo.intCurCtlIndex < m_uInfo.intT1StartIndex) {
				// 対象行の特殊行フラグを取得
				virGetCurrentInfo(&uCurInfo);

				switch (uCurInfo.intFgFunc) {
				case ID_FGFUNC_NULL:			// 空行
				case ID_FGFUNC_DATA:			// データ行
				case ID_FGFUNC_IKKATUMANUAL:	// 一括集計金額行（手動）
					// 操作可能
					bFncEnable = TRUE;
					break;
				}
			}
		}
	}
// midori 160612 cor <--
	pCmdUI->Enable(bFncEnable);
}

// ［Ins：様式切替］ボタン監視
void CfrmUc000Common::OnUpdateButtonInsert(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

//------------------------	メニュー操作関連　------------------------

//　[データインポート]メニュー
void CfrmUc000Common::OnMenuDataImport()
{
	CDBUchiImp  clsImport;
	int			intRet	= -1;
	m_ImportMode		= TRUE;	// インポートフラグ　インポート後のvirStart()内のChangeFontSizeで落ちているので、ここだけ通さない

	// 実行前の準備処理
	CmnEventStartProc(ID_DATAIMPORT_MENU);

// 修正No.158301 add -->
	int		colmsv = -1, colmset = -1;
	int		rowsv = -1, rowset = -1;
	int		sign = 0;

	sign = CmnTblDataToFirstCol(&colmset);
	if(sign == 1)	{
		colmsv = m_uInfo.intCurCol;
		rowsv = m_uInfo.intCurRow;
		//CmnDiagSetFocus(m_pDiag, rowsv, colmset);
		m_pDiag->DeleteInput();
	}
// 修正No.158301 add <--

	m_ActiveNonFlag	= TRUE;

// 修正No.158301 del -->
//// 修正No.157844 add -->
//	int		colmsv = -1, colmset = -1;
//	int		rowsv = -1, rowset = -1;
//	int		sign = 0;
//
//	if(m_uInfo.intFormSeq == ID_FORMNO_021)	{
//		if(m_uInfo.intCurCol == ID_COL_021_SPDATE || m_uInfo.intCurCol == ID_COL_021_PMDATE)	{
//			rowset = 1;
//			colmset = ID_COL_021_KAMOKU;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_061)	{
//		if(bG_Kanso == TRUE && m_uInfo.intCurCol == ID_COL_061K_SPDATE)	{
//			rowset = 1;
//			colmset = ID_COL_061K_KUBUN;
//			sign = 1;
//		}
//		else if(bG_Kanso == FALSE && m_uInfo.intCurCol == ID_COL_061_SPDATE)	{
//			rowset = 1;
//			colmset = ID_COL_061_KUBUN;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_071)	{
//		if(m_uInfo.intCurCol == ID_COL_071_SPDATE || m_uInfo.intCurCol == ID_COL_071_SPDATE2)	{
//			rowset = 1;
//			colmset = ID_COL_071_SYURUI;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_081)	{
//		if(m_uInfo.intCurCol == ID_COL_081_SPDATE || m_uInfo.intCurCol == ID_COL_081_PMDATE)	{
//			rowset = 1;
//			colmset = ID_COL_081_ADNAME1;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_102)	{
//// 修正No.157883,157884 del -->
//		//if(m_uInfo.intCurCol == ID_COL_102_SPDATE)	{
//// 修正No.157883,157884 del <--
//// 修正No.157883,157884 add -->
//		if(m_uInfo.intCurCol == ID_COL_102_SPDATE || m_uInfo.intCurCol == ID_COL_102_VAL)	{
//// 修正No.157883,157884 add <--
//			rowset = 1;
//			colmset = ID_COL_102_KNORDER;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_121)	{
//		if(m_uInfo.intCurCol == ID_COL_121_SPDATE)	{
//			rowset = 1;
//			colmset = ID_COL_121_KUBUN;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_151)	{
//		if(m_uInfo.intCurCol == ID_COL_151_SPDATE || m_uInfo.intCurCol == ID_COL_151_SPDATE2)	{
//			rowset = 1;
//			colmset = ID_COL_151_KUBUN;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_152)	{
//		if(m_uInfo.intCurCol == ID_COL_152_SPDATE)	{
//			rowset = 1;
//			colmset = ID_COL_152_ADNAME1;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_153)	{
//		if( m_uInfo.intCurCol == ID_COL_153_KYDATE || m_uInfo.intCurCol == ID_COL_153_KYDATE2 ||
//			m_uInfo.intCurCol == ID_COL_153_SPDATE || m_uInfo.intCurCol == ID_COL_153_SPDATE2 )	{
//			rowset = 1;
//			colmset = ID_COL_153_HDNAME;
//			sign = 1;
//		}
//	}
//	if(sign == 1)	{
//		colmsv = m_uInfo.intCurCol;
//		rowsv = m_uInfo.intCurRow;
//		CmnDiagSetFocus(m_pDiag, rowset, colmset);
//	}
//// 修正No.157844 add <--
// 修正No.158301 del <--

	// 2010/08/31 業務統合メニュー マルチウィンドウ対応
	//            → プログレスバーが画面中央に表示される件は、ICSモジュールにて対応（2010/08/31 ICS様よりメール連絡）
	// データインポート実行(DLL)
	intRet = clsImport.Import( ((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub, this);

	// データインポート後の後処理
	switch(intRet){
	// テーブル更新を行わなかったため、後処理しない
	case IMP_DIALOG_ERRCODE_CANCEL:
// 修正No.157844 add -->
		if(colmsv != -1 || rowsv != -1)	{
			m_uInfo.intCurRow = rowsv;
			m_uInfo.intCurCol = colmsv;
		}
// 修正No.157844 add <--
		break;
	// 成功：頁を再構成
	case IMP_DIALOG_ERRCODE_OK:

// midori 170502 del -->
		//// 再構成中とのメッセージを表示
		//CdlgImportMessage* pDlgImpMsg;

		//// 2010/08/30 業務統合メニュー マルチウィンドウ対応
		////            親ウィンドウ中央に表示されない対応（念のため）
		//// → CdlgImportMessage() クラス の OnSize() イベントが無い事が原因。
		////pDlgImpMsg = new CdlgImportMessage;
		//pDlgImpMsg = new CdlgImportMessage( this );

		//pDlgImpMsg->Create(IDD_DIALOG_IMPORT,this);
		//pDlgImpMsg->ShowWindow(SW_SHOW);

		//pDlgImpMsg->UpdateWindow();

		//// 後処理(頁調整)
		//int intProcRet = CmnDataImportPostProcess();

		//// メッセージを破棄
		//delete pDlgImpMsg;

		//// 後処理(頁調整)が成功していたら画面更新
		//if ( intProcRet == FUNCTION_OK ){

		//	// 表示している帳表の全データを更新して再表示
		//	virStartJob();

		//	// インポート後は、先頭頁の左上へ移動
		//	m_uInfo.intCurPage = 1;
		//	m_uInfo.intCurRow = 1;
		//	m_uInfo.intCurCol = 1;
		//}
// midori 170502 del <--
// midori 170502 add -->
		// 表示している帳表の全データを更新して再表示
		virStartJob();

		// インポート後は、先頭頁の左上へ移動
		m_uInfo.intCurPage = 1;
		m_uInfo.intCurRow = 1;
		m_uInfo.intCurCol = 1;
// midori 170502 add <--

		break;
	}
	m_ActiveNonFlag	= FALSE;
	m_ImportMode	= FALSE;	// インポートモード終了

	// カーソル移動
	CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
}

//　[データエクスポート]メニュー
void CfrmUc000Common::OnMenuDataExport()
{
	CDBUchiImp  clsImport;
	int			intRet = -1;

	// 実行前の準備処理
	CmnEventStartProc(ID_DATAEXPORT_MENU);

// 修正No.157844 add -->
	int		colmsv = -1, colmset = -1;
	int		rowsv = -1, rowset = -1;
	int		sign = 0;

// 修正No.158301 del -->
//	if(m_uInfo.intFormSeq == ID_FORMNO_021)	{
//		if(m_uInfo.intCurCol == ID_COL_021_SPDATE || m_uInfo.intCurCol == ID_COL_021_PMDATE)	{
//			rowset = 1;
//			colmset = ID_COL_021_KAMOKU;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_061)	{
//		if(bG_Kanso == TRUE && m_uInfo.intCurCol == ID_COL_061K_SPDATE)	{
//			rowset = 1;
//			colmset = ID_COL_061K_KUBUN;
//			sign = 1;
//		}
//		else if(bG_Kanso == FALSE && m_uInfo.intCurCol == ID_COL_061_SPDATE)	{
//			rowset = 1;
//			colmset = ID_COL_061_KUBUN;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_071)	{
//		if(m_uInfo.intCurCol == ID_COL_071_SPDATE || m_uInfo.intCurCol == ID_COL_071_SPDATE2)	{
//			rowset = 1;
//			colmset = ID_COL_071_SYURUI;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_081)	{
//		if(m_uInfo.intCurCol == ID_COL_081_SPDATE || m_uInfo.intCurCol == ID_COL_081_PMDATE)	{
//			rowset = 1;
//			colmset = ID_COL_081_ADNAME1;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_102)	{
//// 修正No.157883,157884 del -->
//		//if(m_uInfo.intCurCol == ID_COL_102_SPDATE)	{
//// 修正No.157883,157884 del <--
//// 修正No.157883,157884 add -->
//		if(m_uInfo.intCurCol == ID_COL_102_SPDATE || m_uInfo.intCurCol == ID_COL_102_VAL)	{
//// 修正No.157883,157884 add <--
//			rowset = 1;
//			colmset = ID_COL_102_KNORDER;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_121)	{
//		if(m_uInfo.intCurCol == ID_COL_121_SPDATE)	{
//			rowset = 1;
//			colmset = ID_COL_121_KUBUN;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_151)	{
//		if(m_uInfo.intCurCol == ID_COL_151_SPDATE || m_uInfo.intCurCol == ID_COL_151_SPDATE2)	{
//			rowset = 1;
//			colmset = ID_COL_151_KUBUN;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_152)	{
//		if(m_uInfo.intCurCol == ID_COL_152_SPDATE)	{
//			rowset = 1;
//			colmset = ID_COL_152_ADNAME1;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_153)	{
//		if( m_uInfo.intCurCol == ID_COL_153_KYDATE || m_uInfo.intCurCol == ID_COL_153_KYDATE2 ||
//			m_uInfo.intCurCol == ID_COL_153_SPDATE || m_uInfo.intCurCol == ID_COL_153_SPDATE2 )	{
//			rowset = 1;
//			colmset = ID_COL_153_HDNAME;
//			sign = 1;
//		}
//	}
//	if(sign == 1)	{
//		colmsv = m_uInfo.intCurCol;
//		rowsv = m_uInfo.intCurRow;
//		CmnDiagSetFocus(m_pDiag, rowset, colmset);
//	}
// 修正No.158301 del <--
// 修正No.158301 add -->
	sign = CmnTblDataToFirstCol(&colmset);
	if(sign == 1)	{
		colmsv = m_uInfo.intCurCol;
		rowsv = m_uInfo.intCurRow;
		//CmnDiagSetFocus(m_pDiag, rowsv, colmset);
		m_pDiag->DeleteInput();
	}
// 修正No.158301 add <--

// 修正No.157844 add <--

	m_ActiveNonFlag	= TRUE;

	// データインポート実行(DLL)
	intRet = clsImport.Export( ((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub, this );

	m_ActiveNonFlag	= FALSE;

// 修正No.157844 add -->
	if(colmsv != -1 || rowsv != -1)	{
		m_uInfo.intCurRow = rowsv;
		m_uInfo.intCurCol = colmsv;
	}
// 修正No.157844 add <--

	// カーソル移動
	CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
}

// ［印刷設定］メニュー
void CfrmUc000Common::OnMenuPrintSetup() 
{
	int nRet = ID_DLG_CANCEL;
// midori 151404 add -->
	BOOL isYousiki[UC_ID_FORMNO_MAX] = {FALSE};		// ここからのコールでは未使用
// midori 151404 add <--
	// 実行前の準備処理
	CmnEventStartProc(ID_PRINTSETUP_MENU);

	// 印刷設定関連のテーブルがオープン可かチェック
	if ( CmnCheckTablePrintSetup() == TRUE ){
		// ダイアログ表示
		CdlgPrintSetup	dlgSet( this );
		BOOL isChange = FALSE;	// ここからのコールでは未使用
// midori 151404 cor -->
//// midori 160608 cor -->
////		nRet = dlgSet.ShowDialog( isChange, m_pDB , m_uInfo.intFormSeq );
//// ---------------------
//		nRet = dlgSet.ShowDialog( isChange, m_pDB , m_uInfo.intFormSeq, -1 );
//// midori 160608 cor <--
// ---------------------
		nRet = dlgSet.ShowDialog( isChange, isYousiki, m_pDB , m_uInfo.intFormSeq, -1 );
// midori 151404 cor <--

		// カーソルセット
		// OnActivateViewイベントでフォーカス再設定するようにした
		//CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
	}

	if( nRet != ID_DLG_OK )		return;

// midori 190505 del -->
	//// 小計の後ろの科目名を削除
	//if( m_uInfo.intFormSeq == ID_FORMNO_111 ){
	//	CdbUcInfSub		mfcRec( m_pDB );	//	uc_inf_subテーブルクラス
	//	// 様式⑪でリクエリ
	//	if ( mfcRec.RequeryFormSeq( ID_FORMNO_111 ) != DB_ERR_OK ){
	//		return;
	//	}

	//	// 「科目行を出力する」チェックついてる？
	//	CCtrlSubGeneralVar cs( mfcRec );
	//	if( mfcRec.m_GeneralConstVal[15] & BIT_D1 ){
	//		if( cs.bOutKamokuRow ){
	//			if( DeleteKnNameBehindSyokei(&mfcRec) == 1 ){
	//				// １ページ再表示
	//				virUpdateControlTblData();
	//			}
	//		}
	//	}
	//}
// midori 190505 del <--
}

// ［一括印刷］メニュー
// 修正No.158301 del -->
//void CfrmUc000Common::OnMenuPrintAllSub(int call_by)
// 修正No.158301 del <--
// 修正No.158301 add -->
int CfrmUc000Common::OnMenuPrintAllSub(int call_by)
// 修正No.158301 add <--
{
// 修正No.158301 add -->
	int		ret = 0;
// 修正No.158301 add <--

	// 2010/08/02 一括印刷対応
	const CString	FN = _T("CfrmUc000Common::OnMenuPrintAll()");
	DbgWriteLog( FN, _T("START") );
	BOOL fPrintRetFlg		= FALSE;		// 印刷結果フラグ
	BOOL fIsErrMsgShowFlg	= FALSE;		// エラーメッセージ表示済フラグ

	// 実行前の準備処理
	CmnEventStartProc(ID_PRINTALL_MENU);

	// ダイアログ表示
	//CdlgPrintAll	dlgAll( this );
	CdlgPrintAll	dlgAll( &g_BatchPrt, this );

	//----------------------------------------------------------------------------------------
	// 2011/01/11 修正依頼No.N-037 一括印刷ユーザ情報共通化
	// owntbのチェック 結果
	// owntb 一括印刷用 ユーザIDレコードチェック（共通ID、ログインID）
	// → 該当レコード無しの場合、誘導メッセージ表示 ＆ 一括印刷出力/CDR 正常終了
	//	  CdlgPrintAll コンストラクタ内でメッセージ表示 ＆ 終了指示送信 済
	//----------------------------------------------------------------------------------------
// 24/01/16_ScoreLink cor -->
	//if ( g_BatchPrt.IsBatchModePrt() || g_BatchPrt.IsBatchModeCDR() ) {
// --------------------------
	if((g_BatchPrt.IsBatchModePrt() || g_BatchPrt.IsBatchModeCDR()) && g_BatchPrt.IsScoreLinkExec() == FALSE) {
// 24/01/16_ScoreLink cor <--
		// 2011/02/08 修正依頼No.N-040 一括印刷情報マスタ単位昇格（運用中マスタ対応）
		//if ( g_BatchPrt.IsDoSetMode() ) {
		//	DbgWriteLog( FN, _T("END") );
		//	return;
		//}
		int nRetSetMode = g_BatchPrt.IsDoSetMode();
		if ((nRetSetMode == OWNTB_CHKREC_NG_DOSET01) ||
			(nRetSetMode == OWNTB_CHKREC_NG_DOSET02) ||
			(nRetSetMode == OWNTB_CHKREC_NG_DOSET03)) {
			DbgWriteLog( FN, _T("END") );
// 修正No.158301 del -->
			//return;
// 修正No.158301 del <--
// 修正No.158301 add -->
			return(1);
// 修正No.158301 add <--
		}
	}

	if( dlgAll.ShowDialog(call_by, m_uInfo.intFormSeq, m_pDB) == ID_DLG_OK ){
// 修正No.158301 add -->
		ret = 1;
// 修正No.158301 add <--

		// 2010/07/XX 一括印刷対応
		// 　→印刷処理の制御
		switch ( g_BatchPrt.GetBatchMode() ) {
		case ID_BATCHMODE_SET:		// 一括印刷 設定モード
			// 一括印刷 設定モードでは何もしない。
// 修正No.158301 del -->
			//if(call_by == ID_PRT_IKKATSU)	return;
// 修正No.158301 del <--
// 修正No.158301 add -->
			if(call_by == ID_PRT_IKKATSU)	return(ret);
// 修正No.158301 add <--

			// F4,F5の場合はSelectJobAllに流したい
		case ID_BATCHMODE_PRT:		// 一括印刷 出力モード
		case ID_BATCHMODE_CDR:		// 一括印刷 CDR出力モード
		default:					// アプリ通常起動
			// インスタンスがこれでいいか不明
			if( m_prtJob == NULL ){
				m_prtJob = CreatePrintObject( m_uInfo.intFormSeq );
			}

			// 印刷処理の実行
// midori 180101 del -->
//// midori 160608 cor -->
//			//if( m_prtJob->SelectJobAll(	((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub,
//			//							((CUCHIWAKEApp*)AfxGetApp())->m_strPath,
//			//							dlgAll.m_nOutGenzai,
//			//							dlgAll.m_PrtList,
//			//							m_NameKei,
//			//							call_by)
//			//							== FUNCTION_OK ){
//// ---------------------
//			if( m_prtJob->SelectJobAll(	((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub,
//										((CUCHIWAKEApp*)AfxGetApp())->m_strPath,
//										dlgAll.m_nOutGenzai,
//										dlgAll.m_PrtList,
//										dlgAll.m_nOutZero,
//										m_NameKei,
//										call_by)
//										== FUNCTION_OK ){
//// midori 160608 cor <--
// midori 180101 del <--
// midori 180101 add -->
			if( m_prtJob->SelectJobAll(	((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub,
										((CUCHIWAKEApp*)AfxGetApp())->m_strPath,
										dlgAll.m_PrtList,
										dlgAll.m_nOutZero,
										m_NameKei,
										call_by)
										== FUNCTION_OK ){
// midori 180101 add <--
				m_ActiveNonFlag	= TRUE;

				if(call_by == ID_PRT_IKKATSU || call_by == ID_PRT_F5)	OnFilePrint();			// 一括印刷メニューorF5から
				else													OnFilePrintPreview();	// F4から

				m_ActiveNonFlag	= FALSE;

				// 2010/07/XX 一括印刷対応
				// 印刷終了 状態セット = 成功
				fPrintRetFlg = TRUE;
			}
			else{

				// エラーメッセージは、CprtJobFunction内で表示する。

				// 2010/07/XX 一括印刷対応
				// エラーメッセージ表示済フラグ = TRUE
				// 　→エラーメッセージは表示済みなので、
				// 　　以降一括印刷エラー処理でメッセージを表示しない。
				fIsErrMsgShowFlg = TRUE;
			}
			break;
		}
	}
// midori 155906 add -->
	else {
// 修正No.158301 add -->
		ret = 0;
// 修正No.158301 add <--

		ClearTempTbl();
// midori 190505 add -->
		m_DispFlg = 0;
// midori 190505 add <--
	}
// midori 155906 add <--

	// 2010/07/XX 一括印刷対応
	// 出力後の終了指示（設定モードは上でリターンしたから注意！）
	switch ( g_BatchPrt.GetBatchMode() ) {
	case	ID_BATCHMODE_PRT:				// 一括印刷 出力モード
	case	ID_BATCHMODE_CDR:				// 一括印刷 CDR出力モード
		// 印刷エラー
		if (fPrintRetFlg != TRUE && fIsErrMsgShowFlg != TRUE) {
			// 2010/08/30 実装クラス変更 → UCHIIMP ビルド対応
			// 出力データがない場合、メッセージを表示して終了する。
			// 2010/08/12 一括印刷 印刷時メッセージ非表示対応
			g_BatchPrt.ICSMessageBoxEx(_T("指定された条件に該当するデータがありません。"));
		}

		// 2010/08/10 一括印刷 印刷時メッセージ非表示対応
		// ① WM_CLOSE受信後の 終了処理 EndProcBprt() で実施とする。
		// ② 印刷データ作成処理で、印刷失敗とするエラーが発生した場合は、以下の関数で エラー発生をセットする。
		//    
		//		CBatchPrt::SetErrorExit();
		// ③ 前項②で情報をセット後、CBatchPrt::ExitBatchMode() 終了通知により、EndProcBprt() で UCOMにセットする
		//
		//BATCHPRTUCOMINF_RETPRT	udBatchPrtRet;

		//switch ( g_BatchPrt.GetBatchMode() ) { 
		////--------------------------------
		//// 一括印刷 設定モードでの起動時
		////--------------------------------
		//// 一括印刷の 出力時（CDR出力は結果返却を行わない）出力結果を UCOM にセット
		//case ID_BATCHMODE_PRT:
		//	// 印刷結果格納
		//	if ( fPrintRetFlg )		{ udBatchPrtRet.nPrtRet = UCOMITM_BATCHPRT_OK; }	// 印刷成功
		//	else					{ udBatchPrtRet.nPrtRet = UCOMITM_BATCHPRT_NG; }	// 印刷失敗
		//	// 印刷結果 セット（UCOM）
		//	g_BatchPrt.SetUcomInfPrt( &udBatchPrtRet );
		//	break;

		////--------------------------------
		//// 一括印刷 CDR出力モードでの起動時
		////--------------------------------
		//case ID_BATCHMODE_CDR:
		//	//	・CDR出力は結果返却を行わない
		//	break;

		//default:
		//	break;
		//}

// 24/01/16_ScoreLink add -->
		// ScoreLinkから起動時は印刷終了後にCSVエクスポートを行う
// 24/04/22_ScoreLink cor -->
		//if(g_BatchPrt.IsScoreLinkExec() == TRUE)	{
// --------------------------
		if(g_BatchPrt.IsScoreLinkExec() == TRUE && g_nLastPage != 0)	{
// 24/04/22_ScoreLink cor <--
			OnMenuDataExport();
		}
// 24/01/16_ScoreLink add <--

		// 一括印刷終了（WM_CLOSE 送信）
		g_BatchPrt.ExitBatchMode();
		break;
	}

	// カーソルセット
	// OnActivateViewイベントでフォーカス再設定するようにした
	//CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);

	// 2010/08/02 一括印刷対応
	DbgWriteLog( FN, _T("END") );

// 修正No.158301 add -->
	return(ret);
// 修正No.158301 add <--
}

// ［一括印刷］メニュー入り口
void CfrmUc000Common::OnMenuPrintAll() 
{
// 24/01/16_ScoreLink cor -->
	//OnMenuPrintAllSub(-1);
// --------------------------
	int		sw=-1;
	if(g_BatchPrt.IsScoreLinkExec() == TRUE)	sw=0;
	OnMenuPrintAllSub(sw);
// 24/01/16_ScoreLink cor <--
}

// midori 160610 add -->
// ［科目指定］メニュー
void CfrmUc000Common::OnMenuReference2() 
{
// 要通知案件修正 21/07/09 add -->
	// 実行前の準備処理
	CmnEventStartProc(ID_REFERENCE2_MENU);
// 要通知案件修正 21/07/09 add <--

// midori 151404 cor -->
	//BOOL			isChange = FALSE;	// ここからのコールでは未使用
	//CdlgReference2	dlg(this);
	//dlg.ShowDialog(isChange,m_pDB,m_uInfo.intFormSeq);
// ---------------------
	BOOL			isChange = FALSE;						// ここからのコールでは未使用
	BOOL			isYousiki[UC_ID_FORMNO_MAX] = {FALSE};	// ここからのコールでは未使用
	CdlgReference2	dlg(this);

	dlg.ShowDialog(isChange,isYousiki,m_pDB,m_uInfo.intFormSeq);
// midori 151404 cor <--
}
// midori 160610 add <--

// ［出力形式］メニュー
void CfrmUc000Common::OnMenuFormTypeSetup() 
{
	int			intSaveFormType = m_uInfo.intFormType;

	// 実行前の準備処理
	CmnEventStartProc(ID_OUTPUTFORM_MENU);

// midori 152770 add -->
	// 別様式関連テーブル、出力形式変更前の行数を保存する。
	// 各様式のCmnSetInitialUchiwakeInfoを呼び出し、セーブ用の構造体に保存
	virUpdateRelationTblChengeFormTypeSave();
// midori 152770 add <--

	// ダイアログ表示
	CdlgOutputForm	dlgOut( this );
	if (dlgOut.ShowDialog(m_pDB ,m_uInfo.intFormSeq)  == ID_DLG_OK) {
		// 変更内容を共通データにセット
		m_uInfo.intFormType = dlgOut.m_FormType;

		// 変更された場合のみ、uc_inf_subのタイトル情報を更新
		if (intSaveFormType != m_uInfo.intFormType) {
			// ④，⑩，⑭～⑯の再チェック ･･･ 本来は必要ないかも
			if (CmnGetChangeFormType() == TRUE) {
				// タイトルを更新
				CdbUcInfSub		rsSub(m_pDB);

				// 帳表タイトルを編集したタイトルに戻す
				rsSub.UpdateTitleTK(m_uInfo.intFormSeq, m_uInfo.intFormType);

				// 更新されたタイトルを取得
				if (rsSub.RequeryFormSeq(m_uInfo.intFormSeq) == DB_ERR_OK) {
					if (!rsSub.IsEOF()) {
						m_uInfo.strTitleName = rsSub.m_Title;
					}
				}
				rsSub.Fin();

				virUpdateControlTitle();
			}
		}

// midori 152709 add -->
		CmnTblUpdatePageRowAllResetMain(1);
// midori 152709 add <--

		// 出力形式と計設定から１頁の最大行数/データ行数と合計行の先頭インデックスを取得
		CmnSetInitialRowMax();

// midori 152709 del -->
		//// 頁と行の再設定
		//CmnTblUpdatePageRowAllResetMain();
// midori 152709 del <--
// midori 152709 add -->
		CmnTblUpdatePageRowAllResetMain(2);
// midori 152709 add <--

		// 関連テーブルの更新（対象：④，⑩，⑭～⑯）
		//	例）④-1で出力形式を更新した場合、④-2でも選択された出力形式の頁構成へ更新が必要
		virUpdateRelationTblChengeFormType();

		// 出力形式の切り替え後は、先頭頁の左上へ移動
		m_uInfo.intCurPage = 1;
		m_uInfo.intCurRow = 1;
		m_uInfo.intCurCol = 1;

		// 次の再表示によりダイアグラムが切り替わるときはTRUE
		m_ChangeFont = TRUE;
		// １ページ再表示
		virUpdateControlTblData();

		// カーソルセット
		CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
	}
}

// midori 180101 add -->
void CfrmUc000Common::OnMenuPrintHyoshi() 
{
	int					nRet=ID_DLG_CANCEL;
	CdlgPrintHyoshi		dlgHyoshi(this);	// 表紙設定ダイアログ

	// 実行前の準備処理
	CmnEventStartProc(ID_PRINTHYOSHI_MENU);

	// ダイアログ表示
	nRet = dlgHyoshi.ShowDialog(m_pDB);

	if( nRet != ID_DLG_OK )		return;
}
// midori 180101 add <--

//　[特殊行挿入]メニュー
void CfrmUc000Common::OnMenuSpecialRow()
{
	CURRENT_INFO uCurInfo;
// 157154 add -->
	CString strkei1=_T("");
	CString strkei2=_T("");
// 157154 add <--

	// 実行前の準備処理
	CmnEventStartProc(ID_SPECIALROW_MENU);

	// カレント行にインバースをかける
	virGetCurrentInfo(&uCurInfo);
// 修正No.157627 add -->
	CmnRowBeforeInverse(&uCurInfo);
// 修正No.157627 add <--
	CmnRowInverse( TRUE , &uCurInfo );

	// ダイアログ表示
	CdlgSpecialRow dlgSpecialRow( this );
// midori 190505 del -->
//	if( dlgSpecialRow.ShowDialog( m_pDB , m_uInfo.intFormSeq ) == ID_DLG_CANCEL ){
// midori 190505 del <--
// midori 190505 add -->
	if( dlgSpecialRow.ShowDialog( m_pDB, m_uInfo.intFormSeq, uCurInfo.intFgFunc ) == ID_DLG_CANCEL ){
// midori 190505 add <--
		// カーソル移動
		// OnActivateViewイベントでフォーカス再設定するようにした
		//CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
		// カレント行のインバースを解除
		CmnRowInverse( FALSE , &uCurInfo );
// 修正No.157627 add -->
		CmnRowAfterInverse(&uCurInfo);
// 修正No.157627 add <--
		return;
	}	
	// カレント行のインバースを解除
	CmnRowInverse( FALSE , &uCurInfo );
// 修正No.157627 add -->
	CmnRowAfterInverse(&uCurInfo);
// 修正No.157627 add <--

// midori 156968 add -->
	// 最終行（頁の最大行）が小計の場合、小計の下に空行を追加する
	InsertNull();
// midori 156968 add <--

	// ダイアログの設定内容から、小計/中計のカッコ付き名称を取得
	CString		strName;
// midori 190505 add -->
	CString		strName2;
// midori 190505 add <--
	switch (dlgSpecialRow.m_FgFunc) {
// midori 190505 del -->
	//case ID_FGFUNC_SYOKEI:
	//case ID_FGFUNC_CHUKEI:
	//	strName = CmnGetSykeiNameWithKakko(dlgSpecialRow.m_OpName, 
	//									   dlgSpecialRow.m_OpLeft, dlgSpecialRow.m_OpRight);
	//	break;
// midori 190505 del <--
// midori 190505 add -->
	case ID_FGFUNC_SYOKEI:
		strName2 = CmnGetSykeiNameWithKakko(dlgSpecialRow.m_OpName, 
										   dlgSpecialRow.m_OpLeft, dlgSpecialRow.m_OpRight);
		strName = GetSyokeiName(m_uInfo.intFormSeq,m_uInfo.intCurPage, m_uInfo.intCurRow,strName2);
// 157154 add -->
		// 小計名称が編集可能の場合
// 改良No.21-0086,21-0529 cor -->
		//if(KamokuRowEnableSgn(m_pDB,0) == 1) {
// ------------------------------
		if(KamokuRowEnableSgn(m_pDB, 0, m_uInfo.intFormSeq) == 1) {
// 改良No.21-0086,21-0529 cor <--
			m_clsFunc.KeiStrSprit(strName, &strkei1, &strkei2);
			if (strkei2.IsEmpty() == FALSE)		strName = strkei1 + _T("\r\n") + strkei2;
		}
// 157154 add <--
		break;
	case ID_FGFUNC_CHUKEI:
		strName = CmnGetSykeiNameWithKakko(dlgSpecialRow.m_OpName, 
										   dlgSpecialRow.m_OpLeft, dlgSpecialRow.m_OpRight);
		break;
// midori 190505 add <--
	default:
		strName = dlgSpecialRow.m_OpName;
		break;
	}

// midori 190505 add -->
	// 科目行は挿入を行った行の科目名称をセットする
	if(dlgSpecialRow.m_FgFunc == ID_FGFUNC_KAMOKU)	{
		CdbUc000Common*		rsData;
		rsData = virGetDataTableObject();
		// 頁と行からレコード抽出
		rsData->RequeryPageRow(m_uInfo.intCurPage,m_uInfo.intCurRow);
		strName = virTblSortGetKamokuName(rsData);
	}
// midori 190505 add <--

	// 頁番号/行番号振り直し
	CmnTblUpdatePageNumRowNumAdd(m_uInfo.intCurPage, m_uInfo.intCurRow, 1);
	
	// 特殊行挿入
	CmnTblInsertSpecialLine(m_uInfo.intCurPage, m_uInfo.intCurRow, 
							dlgSpecialRow.m_FgFunc, strName);

	// 頁と行の調整処理（挿入後処理）
	CmnTblDeleteFgFunc(ID_FGFUNC_PAGEKEI);					// 頁計の削除
	CmnTblDeleteFgFunc(ID_FGFUNC_RUIKEI);					// 累計の削除
	CmnTblUpdateEndPageSetting();							// 最終頁を調整する
	CmnTblInsertPageKeiRuiKei();							// 頁計/累計挿入（最終ページの削除or空行追加）

	// 小計/中計/頁計/累計の再計算
	CmnTblCalcKei();
	
// midori 190505 add -->
	// 小計の後ろの科目名を削除
	if( dlgSpecialRow.m_FgFunc == ID_FGFUNC_KAMOKU ) {
		SetKnNameSyokei(m_uInfo.intFormSeq,m_uInfo.intCurPage,m_uInfo.intCurRow,1);
	}
// midori 190505 add <--

// midori 156968 add -->
	// 先頭行が空行の場合削除する
	CmnTblUpdatePageRowAllResetMain(6);
// midori 156968 add <--

	// １ページ再表示
	virUpdateControlTblData();

	// カーソル移動
// midori 156934 del -->
//	CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
// midori 156934 del <--
// midori 156934 add -->
	int		nCol=0;
	switch(dlgSpecialRow.m_FgFunc)	{
	case ID_FGFUNC_KAMOKU:
	case ID_FGFUNC_SYOKEI:
// midori 157046 add -->
	case ID_FGFUNC_CHUKEI:
// midori 157046 add <--
// midori 157046 del -->
		//if(bG_Kanso == TRUE &&( m_uInfo.intFormSeq == ID_FORMNO_021 || m_uInfo.intFormSeq == ID_FORMNO_111))	{
// midori 157046 del <--
// 改良No.21-0086,21-0529 cor -->
//// midori 157046 add -->
//		if(KamokuRowEnableSgn(m_pDB,0) == 1 &&( m_uInfo.intFormSeq == ID_FORMNO_021 || m_uInfo.intFormSeq == ID_FORMNO_111))	{
//// midori 157046 add <--
//			// ID_COL_021_ADNAME1 = 2	振出人
//			// ID_COL_111_ADNAME1 = 2	借入先1
//			nCol = 2;
//		}
// ------------------------------
		if(KamokuRowEnableSgn(m_pDB, 0, m_uInfo.intFormSeq) == 1 &&
			(m_uInfo.intFormSeq == ID_FORMNO_021 || (bG_InvNo == TRUE && m_uInfo.intFormSeq == ID_FORMNO_081) || m_uInfo.intFormSeq == ID_FORMNO_111))	{
			// ID_COL_021_ADNAME1,ID_COL_021I_ADNAME1	振出人
			// ID_COL_081I_ADNAME1						支払先
			// ID_COL_111_ADNAME1						借入先1
			if(m_uInfo.intFormSeq == ID_FORMNO_021)	{
				if(bG_InvNo == FALSE)	nCol = ID_COL_021_ADNAME1;
				else					nCol = ID_COL_021I_ADNAME1;
			}
			else if(m_uInfo.intFormSeq == ID_FORMNO_081)	{
				nCol = ID_COL_081I_ADNAME1;
			}
			else	{
				nCol = ID_COL_111_ADNAME1;
			}
		}
// 改良No.21-0086,21-0529 cor <--
		else	{
			nCol = m_uInfo.intCurCol;
		}
		break;
	default:
		nCol = m_uInfo.intCurCol;
		break;
	}
	CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, nCol);
// midori 156934 add <--
}

//　[ソート]メニュー
void CfrmUc000Common::OnMenuSort()
{
	CdlgSort	dlgSort( this );
// midori 152745 add -->
	int			as=0;
// midori 152745 add <--

	// 実行前の準備処理
	CmnEventStartProc(ID_SORT_MENU);

// 21/07/09 add -->
	int		colsv=-1, rowsv=-1;
	int		colset=-1;
	int		sign = 0;

	sign = CmnTblDataToFirstCol(&colset);
	if(sign == 1)	{
		rowsv = m_uInfo.intCurRow;
		colsv = m_uInfo.intCurCol;
		CmnDiagSetFocus(m_pDiag, rowsv, colset);
	}
// 21/07/09 add <--

	// ソート関連のテーブルがオープン可かチェック
	if ( CmnCheckTableSort() == TRUE ){
		// ダイアログ表示
// midori 152745 del -->
//// midori 161109 del -->
////		if (dlgSort.ShowDialog(m_IkkatuOutZero, m_pDB , m_uInfo.intFormSeq) == ID_DLG_CANCEL) {
//// midori 161109 del <--
//// midori 161109 add -->
//		if (dlgSort.ShowDialog(m_IkkatuOutZero, m_pDB , m_uInfo.intFormSeq, m_BankSort, m_AddressSort) == ID_DLG_CANCEL) {
//// midori 161109 add <--
// midori 152745 del <--
// midori 152745 add -->
		switch(m_uInfo.intFormSeq)	{
			// ② 受取手形の内訳書
			case	ID_FORMNO_021:
			// ③ 売掛金（未収入金）の内訳書
			case	ID_FORMNO_031:
			// ④－２ 貸付金及び受取利息の内訳書
			case	ID_FORMNO_042:
			// ⑩－１ 仮受金（前受金・預り金）の内訳書
			case	ID_FORMNO_101:
			// ⑯－２ 雑損失等の内訳書
			case	ID_FORMNO_162:
				// 得意先を参照
				as = m_AddressSort[1];
				break;
			// ④－１ 仮払金（前渡金）の内訳書
			// ⑧ 支払手形の内訳書
			// ⑨ 買掛金（未払金・未払費用）の内訳書
			// ⑯－１ 雑益、雑損失等の内訳書
			default:
				// 仕入先を参照
				as = m_AddressSort[0];
				break;
		}
		if(dlgSort.ShowDialog(m_IkkatuOutZero,m_pDB,m_uInfo.intFormSeq,m_BankSort,as) == ID_DLG_CANCEL)	{
// midori 152745 add <--
			// カーソル移動
			// OnActivateViewイベントでフォーカス再設定するようにした
			//CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
// 21/07/09 add -->
			// カーソルを元に戻す
// 修正No.158301 del -->
			//if(sign == 1)	{
// 修正No.158301 del <--
// 修正No.158301 add -->
			if(rowsv != -1 || colsv != -1)	{
// 修正No.158301 add <--
				CmnDiagSetFocus(m_pDiag, rowsv, colsv);
			}
// 21/07/09 add <--
			return;
		}
		if(m_IkkatuOutZero != dlgSort.m_OpIkkatuOutZero){
			m_IkkatuOutZero = dlgSort.m_OpIkkatuOutZero;
			CdbUcInfSub mfcSub(m_pDB);
			mfcSub.RequeryFormSeq(m_uInfo.intFormSeq);
			if(!mfcSub.IsEOF()){
				CCtrlSubGeneralVar cs(mfcSub);
				cs.bIkkatuOutZero = m_IkkatuOutZero;
				mfcSub.Edit();
				cs.Set(mfcSub);
				mfcSub.Update();
			}
			mfcSub.Fin();
		}

// midori 151397 add -->
		if(dlgSort.m_SortSaveSign == 0)	{
// midori 151397 add <--
			// プログレスバーを止め、カーソルを砂時計にする
			AfxGetApp()->DoWaitCursor(1);

			// ソート処理
			if (CmnTblSortMainProc(&dlgSort) == FUNCTION_NG) {
				// カーソルを元に戻す
// 21/07/09 add -->
// 修正No.158301 del -->
				//if(sign == 1)	{
// 修正No.158301 del <--
// 修正No.158301 add -->
				if(rowsv != -1 || colsv != -1)	{
// 修正No.158301 add <--
					CmnDiagSetFocus(m_pDiag, rowsv, colsv);
				}
// 21/07/09 add <--
				AfxGetApp()->DoWaitCursor(-1);
				return;
			}

			// 頁と行の再設定
			CmnTblUpdatePageRowAllResetMainAfterSort(&dlgSort);

			// ソート実行後は、先頭頁の左上へ移動
			m_uInfo.intCurPage = 1;
			m_uInfo.intCurRow = 1;
			m_uInfo.intCurCol = 1;

// midori 200101 add -->
			// カーソル移動
			// →元号のコンボボックスを開いたままにしていると【小計】が正しく追加されない場合がある
			CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
// midori 200101 add <--

			// カーソルを元に戻す
			AfxGetApp()->DoWaitCursor(-1);

			// 次の再表示によりダイアグラムが切り替わるときはTRUE
			m_ChangeFont = TRUE;
			// １ページ再表示
			virUpdateControlTblData();
// midori 151397 add -->
		}
// midori 151397 add <--
// 21/07/09 add -->
		else	{
// 修正No.158301 del -->
			//if(sign == 1)	{
// 修正No.158301 del <--
// 修正No.158301 add -->
			if(rowsv != -1 || colsv != -1)	{
// 修正No.158301 add <--
				m_uInfo.intCurRow = rowsv;
				m_uInfo.intCurCol = colsv;
			}
		}
// 21/07/09 add <--

		// カーソル移動
		CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
	}
// 21/07/09 add -->
	else	{
// 修正No.158301 del -->
		//if(sign == 1)	{
// 修正No.158301 del <--
// 修正No.158301 add -->
		if(rowsv != -1 || colsv != -1)	{
// 修正No.158301 add <--
			CmnDiagSetFocus(m_pDiag, rowsv, colsv);
		}
	}
// 21/07/09 add <--
}

//　[一括金額追加]メニュー
void CfrmUc000Common::OnIkkatuAddMenu()
{
	CURRENT_INFO uCurInfo;
	CString		 strMsg;

	// TODO: ここにコマンド ハンドラ コードを追加します。
	// 実行前の準備処理
	CmnEventStartProc(ID_IKKATUADD_MENU);

	// カレント行にインバースをかける
	virGetCurrentInfo(&uCurInfo);
// 修正No.157627 add -->
	CmnRowBeforeInverse(&uCurInfo);
// 修正No.157627 add <--
	CmnRowInverse( TRUE , &uCurInfo );

	// 確認メッセージ文作成
	strMsg.Format("該当行を一括金額に加算してよろしいですか？");
	strMsg += "\n";
	strMsg += "\n集計先は該当行以降の最初の一括金額行に加算します。";
	strMsg += "\n一括金額行がない場合は、小計位置に一括金額行を作成します。";

	// 確認メッセージ表示
	if ( ICSMessageBox( strMsg, MB_OKCANCEL, 0, 0, this ) == IDOK ){
		// カレント行のインバースを解除
		CmnRowInverse( FALSE , &uCurInfo );
// 修正No.157627 add -->
		CmnRowAfterInverse(&uCurInfo);
// 修正No.157627 add <--
		// 一括金額追加処理
		CmnTblIkkatuAddProc();
	}
	else{
		// カレント行のインバースを解除
		CmnRowInverse( FALSE , &uCurInfo );
// 修正No.157627 add -->
		CmnRowAfterInverse(&uCurInfo);
// 修正No.157627 add <--
	}

	// カーソル移動
	CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
}

// ［一括金額参照］メニュー
void CfrmUc000Common::OnMenuIkkatuKingaku() 
{
	// 実行前の準備処理
	CmnEventStartProc(ID_DISPDIALOG_MENU);

	// 一括金額参照処理
	CmnTblIkkatuHokanProc( ID_FGSHOW_IKKATU );

	// カーソル移動
	// OnActivateViewイベントでフォーカス再設定するようにした
	//CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
}

// ［一時保管参照］メニュー
//　全ての"一時保管"文言を"保管"に変更
void CfrmUc000Common::OnMenuZeroMoney() 
{
	// 実行前の準備処理
	CmnEventStartProc(ID_ZEROMONEY_MENU);

	// 保管テーブル対応前の保管データを保管テーブルにコピー
	//if( IsRegExistHokanData() != FALSE ){
	//	CopyHokanDataToHokanTable(m_uInfo.intFormSeq);
	//}

	// 一時保管参照処理
	CmnTblIkkatuHokanProc( ID_FGSHOW_HOKAN );

	// カーソル移動
	// OnActivateViewイベントでフォーカス再設定するようにした
	//CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
}

// ［一括で一時保管へ移動］メニュー
void CfrmUc000Common::OnMenuZeroMoneyMove() 
{
	// 実行前の準備処理
	CmnEventStartProc(ID_ZEROMONEYMOVE_MENU);

	// 保管テーブル対応前の保管データを保管テーブルにコピー
	//if( IsRegExistHokanData() != FALSE ){
	//	CopyHokanDataToHokanTable(m_uInfo.intFormSeq);
	//}

// 21/07/09 add -->
	int		colsv=-1, rowsv=-1;
	int		colset=-1;
	int		sign = 0;

	sign = CmnTblDataToFirstCol(&colset);
	if(sign == 1)	{
		rowsv = m_uInfo.intCurRow;
		colsv = m_uInfo.intCurCol;
		CmnDiagSetFocus(m_pDiag, rowsv, colset);
	}
// 21/07/09 add <--

	// 一括で一時保管へ移動ダイアログ表示
	CdlgZeroMoneyMove	dlgMove( this );
	if (dlgMove.ShowDialog( m_uInfo.intFormSeq, m_pDB ) == ID_DLG_OK) {

		// 一時保管へ移動
		if ( CmnTblZeroMoneyMoveMainProc( dlgMove.m_HokanData ) == FUNCTION_OK ){

			// 一時保管へ移動後は、先頭頁の左上へ移動
			m_uInfo.intCurPage = 1;
			m_uInfo.intCurRow = 1;
			m_uInfo.intCurCol = 1;

			// 次の再表示によりダイアグラムが切り替わるときはTRUE
			m_ChangeFont = TRUE;
			// １ページ再表示
			virUpdateControlTblData();

			// カーソル移動
			CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
		}
	}
// 21/07/09 add -->
	else	{
// 修正No.158301 del -->
		//if(sign == 1)	{
// 修正No.158301 del <--
// 修正No.158301 add -->
		if(rowsv != -1 || colsv != -1)	{
// 修正No.158301 add <--
			CmnDiagSetFocus(m_pDiag, rowsv, colsv);
		}
	}
// 21/07/09 add <--
}

// ［計設定］メニュー
void CfrmUc000Common::OnMenuOutKeiSetup()
{
	//int bitNameKei = GetFormSeqBit( m_uInfo.intFormSeq );

	// 実行前の準備処理
	CmnEventStartProc(ID_AMOUTDIALOG_MENU);

// 修正No.157633_元号日付 del -->
//// 157382 add -->
//	int		colmsv=-1;
//	if(m_uInfo.intFormSeq == ID_FORMNO_021)	{
//		if(m_uInfo.intCurCol == ID_COL_021_SPDATE || m_uInfo.intCurCol == ID_COL_021_PMDATE)	{
//			colmsv = m_uInfo.intCurCol;
//			CmnDiagSetFocus(m_pDiag,m_uInfo.intCurRow,ID_COL_021_KAMOKU);
//		}
//	}
//// 157382 add <--
// 修正No.157633_元号日付 del <--
// 修正No.157633_元号日付 add -->
	int		colmsv = -1, colmset = -1;
	int		rowsv = -1, rowset = -1;
	int		sign = 0;

// 修正No.158301 del -->
//	if(m_uInfo.intFormSeq == ID_FORMNO_021)	{
//		if(m_uInfo.intCurCol == ID_COL_021_SPDATE || m_uInfo.intCurCol == ID_COL_021_PMDATE)	{
//			rowset = 1;
//			colmset = ID_COL_021_KAMOKU;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_061)	{
//		if(bG_Kanso == TRUE && m_uInfo.intCurCol == ID_COL_061K_SPDATE)	{
//			rowset = 1;
//			colmset = ID_COL_061K_KUBUN;
//			sign = 1;
//		}
//		else if(bG_Kanso == FALSE && m_uInfo.intCurCol == ID_COL_061_SPDATE)	{
//			rowset = 1;
//			colmset = ID_COL_061_KUBUN;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_071)	{
//		if(m_uInfo.intCurCol == ID_COL_071_SPDATE || m_uInfo.intCurCol == ID_COL_071_SPDATE2)	{
//			rowset = 1;
//			colmset = ID_COL_071_SYURUI;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_081)	{
//		if(m_uInfo.intCurCol == ID_COL_081_SPDATE || m_uInfo.intCurCol == ID_COL_081_PMDATE)	{
//			rowset = 1;
//			colmset = ID_COL_081_ADNAME1;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_102)	{
//// 修正No.157883,157884 del -->
//		//if(m_uInfo.intCurCol == ID_COL_102_SPDATE)	{
//// 修正No.157883,157884 del <--
//// 修正No.157883,157884 add -->
//		if(m_uInfo.intCurCol == ID_COL_102_SPDATE || m_uInfo.intCurCol == ID_COL_102_VAL)	{
//// 修正No.157883,157884 add <--
//			rowset = 1;
//			colmset = ID_COL_102_KNORDER;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_121)	{
//		if(m_uInfo.intCurCol == ID_COL_121_SPDATE)	{
//			rowset = 1;
//			colmset = ID_COL_121_KUBUN;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_151)	{
//		if(m_uInfo.intCurCol == ID_COL_151_SPDATE || m_uInfo.intCurCol == ID_COL_151_SPDATE2)	{
//			rowset = 1;
//			colmset = ID_COL_151_KUBUN;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_152)	{
//		if(m_uInfo.intCurCol == ID_COL_152_SPDATE)	{
//			rowset = 1;
//			colmset = ID_COL_152_ADNAME1;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_153)	{
//		if( m_uInfo.intCurCol == ID_COL_153_KYDATE || m_uInfo.intCurCol == ID_COL_153_KYDATE2 ||
//			m_uInfo.intCurCol == ID_COL_153_SPDATE || m_uInfo.intCurCol == ID_COL_153_SPDATE2 )	{
//			rowset = 1;
//			colmset = ID_COL_153_HDNAME;
//			sign = 1;
//		}
//	}
//	if(sign == 1)	{
//		colmsv = m_uInfo.intCurCol;
//		rowsv = m_uInfo.intCurRow;
//		CmnDiagSetFocus(m_pDiag, rowset, colmset);
//	}
// 修正No.158301 del <--
// 修正No.158301 add -->
	sign = CmnTblDataToFirstCol(&colmset);
	if(sign == 1)	{
		colmsv = m_uInfo.intCurCol;
		rowsv = m_uInfo.intCurRow;
		CmnDiagSetFocus(m_pDiag, rowsv, colmset);
	}
// 修正No.158301 add <--

// 修正No.157633_元号日付 add <--

	// 計設定ダイアログ表示
	CdlgAmount	dlgAmout( this );
	if (dlgAmout.ShowDialog(m_pDB, m_uInfo.intFormSeq, m_uInfo.intOutKeiDialog, m_NameKei) == ID_DLG_OK) {
// midori 156968 add -->
		// 計設定の切り替え後は、先頭頁の左上へ移動
		m_uInfo.intCurPage = 1;
		m_uInfo.intCurRow = 1;
		m_uInfo.intCurCol = 1;

		InsertNull();
// midori 156968 add <--
		// ダイアログで選択された値を、ローカルのメンバー変数へ反映させる
		m_uInfo.intOutKei = dlgAmout.m_OutKei;

		if(m_NameKei != dlgAmout.m_nNameKei){
			m_NameKei = dlgAmout.m_nNameKei;
			CdbUcInfSub mfcSub(m_pDB);
			mfcSub.RequeryFormSeq(m_uInfo.intFormSeq);
			CCtrlSubGeneralVar cs(mfcSub);

			if(m_NameKei)	cs.bNameKei = TRUE;
			else			cs.bNameKei = FALSE;

			mfcSub.Edit();
			cs.Set(mfcSub);
			mfcSub.Update();
			mfcSub.Fin();
		}

		// 出力形式と計設定から１頁の最大行数/データ行数と合計行の先頭インデックスを取得
		CmnSetInitialRowMax();

		// 頁と行の再設定
		CmnTblUpdatePageRowAllResetMain();
		
// midori 156968 del -->
		//// 計設定の切り替え後は、先頭頁の左上へ移動
		//m_uInfo.intCurPage = 1;
		//m_uInfo.intCurRow = 1;
		//m_uInfo.intCurCol = 1;
// midori 156968 del <--
// midori 156968 add -->
		// 先頭行が空行の場合削除する
		CmnTblUpdatePageRowAllResetMain(6);
// midori 156968 add <--

		// 次の再表示によりダイアグラムが切り替わるときはTRUE
		m_ChangeFont = TRUE;
		// １ページ再表示
		virUpdateControlTblData();

		// カーソル移動
		CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
	}
// 157382 add -->
	else	{
// 修正No.157633_元号日付 del -->
		//if(colmsv != -1)	{
		//	CmnDiagSetFocus(m_pDiag,m_uInfo.intCurRow,colmsv);
		//}
// 修正No.157633_元号日付 del <--
// 修正No.157633_元号日付 add -->
		if(colmsv != -1 || rowsv != -1)	{
			CmnDiagSetFocus(m_pDiag, rowsv, colmsv);
		}
// 修正No.157633_元号日付 add <--
	}
// 157382 add <--

}

// midori 160606 add -->
// ［金額等数値0クリア］メニュー
void CfrmUc000Common::OnMenuClear()
{
	// 実行前の準備処理
	CmnEventStartProc(ID_CLEAR_MENU);

	// 金額等数値０クリアダイアログ表示
	CdlgZeroMoneyClear	dlgClear(this);
	if(dlgClear.ShowDialog(m_uInfo.intFormSeq,m_pDB) == ID_DLG_OK)	{
		if(CmnTblZeroMoneyClearMainProc(dlgClear.m_ClearData) == FUNCTION_OK)	{
			// 次の再表示によりダイアグラムが切り替わるときはTRUE
			m_ChangeFont = TRUE;
			// １ページ再表示
			virUpdateControlTblData();
		}
	}
}

//********************************************************************************
//	金額等数値０クリア処理（メイン）
//********************************************************************************
int CfrmUc000Common::CmnTblZeroMoneyClearMainProc( CLEAR_DATA *hData )
{
	CfrmUc000Common*	pView=NULL;			// FormViewオブジェクト
	CdbUc000Common*		pData1=NULL;
	int					ii=0;

	// 帳表分ループ
	for(ii=0; ii<UC_ID_FORMNO_MAX; ii++)	{
		// ０クリア、空欄が最低１つチェックされている時のみ実行
		if((hData[ii].blnZeroFg == TRUE) || (hData[ii].blnNullFg == TRUE))	{
			// 一応、FormSeq番号が取れているかも見る
			if(hData[ii].intFormSeq > 0)	{
				pView = ((CUCHIWAKEApp*)AfxGetApp())->UchiwakeCreateFormViewObject(hData[ii].intFormSeq);		// FormViewオブジェクト
				if(pView->CmnCheckTableInit() != TRUE)	{	// 帳表テーブルがオープン可?
					if(pView != NULL)	{
						delete pView;
					}
					return FUNCTION_NG;
				}
				// 初期設定（共通データ）
				if(pView->CmnSetInitialUchiwakeInfo(hData[ii].intFormSeq) != TRUE)	{
					if(pView != NULL)	{
						delete pView;
					}
					return FUNCTION_NG;
				}
				// 初期設定（帳表固有データ）
				pView->virSetInitialValue();
				// dbオブジェクト取得
				pData1 = pView->virGetDataTableObject();
				// 1.金額等を０クリアする
				if(hData[ii].blnZeroFg == TRUE)	{
					pData1->UpdateYokukiZeroClear(0);
				}
				// 2.金額等を空欄にする
				if(hData[ii].blnNullFg == TRUE)	{
					pData1->UpdateYokukiZeroClear(1);
				}
			}
		}
	}

	return FUNCTION_OK;
}
// midori 160606 add <--

// ［財務連動実行］メニュー
void CfrmUc000Common::OnMenuZaimuRendo()
{
	// 実行前の準備処理
	CmnEventStartProc(ID_RENDO_MENU);

	// 財務連動関連のテーブルがオープン可かチェック
	if ( CmnCheckTableZaimuRendo() == TRUE ){
		// ダイアログ表示
		CdlgRenMain		dlgRenMain(this);
// midori 152153 del -->
//		if (dlgRenMain.ShowDialog(m_pDB, &m_RenZvol, m_uInfo.intFormSeq) == ID_DLG_CANCEL) {
// midori 152153 del <--
// midori 152153 add -->
		if (dlgRenMain.ShowDialog(m_pDB, &m_RenZvol, m_uInfo.intFormSeq, m_RegAutoRef) == ID_DLG_CANCEL) {
// midori 152153 add <--
			// 新規内訳書科目が追加された場合はページ更新(科目コンボリスト更新のため)
			if ( dlgRenMain.m_UcKmkEntry == 1 ){
				// 次の再表示によりダイアグラムが切り替わるときはTRUE
				m_ChangeFont = TRUE;
				// １ページ再表示
				virUpdateControlTblData();
				// カーソル移動
				CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
			}
			return;
		}

		// プログレスバー開始（暫定：カーソル変更）
		AfxGetApp()->DoWaitCursor(1);

		// 帳表毎のテーブルクラスを"New"する
		CdbUc000Common*		rsData;
		rsData = virGetDataTableObject();

		// 財務連動データ取込処理
		CmnTblRendoMainProc(&dlgRenMain, rsData);

		// uc_ren_main：前回実行時の連動元データ選択内容を更新
		CdbUcRenMain	rsRenMain(m_pDB);
		rsRenMain.RequeryFormSeq(m_uInfo.intFormSeq);
		if (!rsRenMain.IsEOF()) {
			rsRenMain.Edit();
			rsRenMain.m_OpOldRenType = dlgRenMain.m_OpRenType;
			rsRenMain.Update();
		}
		rsRenMain.Fin();

		// 頁と行の再設定
		CmnTblUpdatePageRowAllResetMain();

		// プログレスバー終了（暫定：カーソル変更）
		AfxGetApp()->DoWaitCursor(-1);


		// テーブルクラスのメモリ開放
		delete rsData;

		// 財務連動実行後は、先頭頁の左上へ移動
		m_uInfo.intCurPage = 1;
		m_uInfo.intCurRow = 1;
		m_uInfo.intCurCol = 1;

		// 次の再表示によりダイアグラムが切り替わるときはTRUE
		m_ChangeFont = TRUE;
		// １ページ再表示
		virUpdateControlTblData();

		// カーソル移動
		CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
	}
}

// ［タイトルを元に戻す］メニュー
void CfrmUc000Common::OnMenuTitleRestore()
{
	// 実行前の準備処理
	CmnEventStartProc(ID_TITLEMENU);

	// uc_inf_sub更新
	CmnUcInfSubSetTitleInfo(FALSE);

	// 帳表タイトルを更新
	virUpdateControlTitle();

	// タイトル欄にフォーカスを移動する
	m_pTitleDiag->SetPosition(0);
}

// ［オプション設定］メニュー
// 旧ユーザー設定
void CfrmUc000Common::OnMenuOptionSetting()
{
	BOOL bYoushikiSkipOld	= m_YoushikiSkip;
// 24/01/30_23-0845 add -->
	// 参照モードのためOFFだったのをONにしているのでOFFに戻す
	if(((CUCHIWAKEApp*)AfxGetApp())->m_CloudSansyoFlg2 == TRUE)	{
		m_DataKakutei = FALSE;
	}
// 24/01/30_23-0845 add <--
// midori 160612 add -->
	BOOL bDataKakuteiOld	= m_DataKakutei;
// midori 190301 add -->
	BOOL bUseStyleOld		= bG_Kanso;
// midori 190301 add <--
	BOOL bUseStyleOld2		= bG_InvNo;		// インボイス登録番号追加対応_23/11/06 add

	int		intIndex=0;
// midori 160612 add <--
// midori 200101 add -->
	int		svbG_Kanso=0;
// midori 200101 add <--
	BOOL	svbG_InvNo=0;					// インボイス登録番号追加対応_23/11/06 add

// midori 190301 del -->
//// midori 160612 cor -->
////	USER_OPTION_INFO usi(m_YoushikiSkip, m_DispIkkatDlg, m_DispYoushikiSelect, m_ReturnMoveDown);	
//// ---------------------
//	USER_OPTION_INFO usi(m_YoushikiSkip, m_DispIkkatDlg, m_DispYoushikiSelect, m_ReturnMoveDown, m_DataKakutei);	
//// midori 160612 cor <--
// midori 190301 del <--
// midori 190301 add -->
	//USER_OPTION_INFO usi(m_YoushikiSkip,m_DispIkkatDlg,m_DispYoushikiSelect,m_ReturnMoveDown,m_DataKakutei,bG_Kanso);				// インボイス登録番号追加対応_23/11/06 del
// 24/01/30_23-0845 cor -->
	//USER_OPTION_INFO usi(m_YoushikiSkip,m_DispIkkatDlg,m_DispYoushikiSelect,m_ReturnMoveDown,m_DataKakutei,bG_Kanso,bG_InvNo);	// インボイス登録番号追加対応_23/11/06 add
// ------------------------
	USER_OPTION_INFO usi(m_YoushikiSkip,m_DispIkkatDlg,m_DispYoushikiSelect,m_ReturnMoveDown,m_DataKakutei,bG_Kanso,bG_InvNo,((CUCHIWAKEApp*)AfxGetApp())->m_CloudSansyoFlg);
// 24/01/30_23-0845 cor <--
// midori 190301 add <--

	// 実行前の準備処理
	CmnEventStartProc(ID_USER_SETTING_MENU);

	CdlgUserSet	dlgUserSet(this);	// 元々ユーザー設定ダイアログだったのでCdlgUserSetになっている
// 24/01/30_23-0845 cor -->
	//if( dlgUserSet.ShowDialog(usi) != ID_DLG_OK )	return;
// ------------------------
	if(dlgUserSet.ShowDialog(usi) != ID_DLG_OK)	{
		// 参照モードのためONに戻す
		if(((CUCHIWAKEApp*)AfxGetApp())->m_CloudSansyoFlg2 == TRUE)	{
			m_DataKakutei = TRUE;
		}
		return;
	}
// 24/01/30_23-0845 cor <--

	BOOL isUpdate = FALSE;	// 変更あり？
	if((m_YoushikiSkip && !usi.YoushikiSkip) || (!m_YoushikiSkip && usi.YoushikiSkip)){	// usi.YoushikiSkipがTRUE, m_YoushikiSkipがBIT_D3等　真でも値が異なることがあるので、このような判定方法をとる　以下の変数も同じ
		m_YoushikiSkip = usi.YoushikiSkip;
		isUpdate = TRUE;
	}
	if((!m_DispIkkatDlg && usi.DispIkkatDlg) || (m_DispIkkatDlg && !usi.DispIkkatDlg)){
		m_DispIkkatDlg = usi.DispIkkatDlg;
		isUpdate = TRUE;
	}
	if((!m_DispYoushikiSelect && usi.DispYoushikiSelect) || (m_DispYoushikiSelect && !usi.DispYoushikiSelect)){
		m_DispYoushikiSelect = usi.DispYoushikiSelect;
		isUpdate = TRUE;
	}
	if((!m_ReturnMoveDown && usi.EnterMoveDown) || (m_ReturnMoveDown && !usi.EnterMoveDown)){
		m_ReturnMoveDown = usi.EnterMoveDown;
		isUpdate = TRUE;
	}
// midori 160612 add -->
	if((!m_DataKakutei && usi.DataKakutei) || (m_DataKakutei && !usi.DataKakutei)){
		m_DataKakutei = usi.DataKakutei;
		isUpdate = TRUE;
	}
// midori 160612 add <--
// midori 190301 add -->
	if((!bG_Kanso && usi.UseStyle) || (bG_Kanso && !usi.UseStyle)){
		bG_Kanso = usi.UseStyle;
		isUpdate = TRUE;
	}
// midori 190301 add <--
// インボイス登録番号追加対応_23/11/06 add -->
	if((!bG_InvNo && usi.UseStyle2) || (bG_InvNo && !usi.UseStyle2))	{
		bG_InvNo = usi.UseStyle2;
		isUpdate = TRUE;
	}
// インボイス登録番号追加対応_23/11/06 add <--

	if(isUpdate){
		CdbUcInfMain	rsMain(m_pDB);
		if(rsMain.Init() == DB_ERR_OK){
			rsMain.Edit();
			CCtrlMainGeneralVar cm(rsMain);
			// データのある様式へ移動する
			if(m_YoushikiSkip)			cm.bYoushikiSkip = TRUE;
			else						cm.bYoushikiSkip = FALSE;
			// プレビュー、印刷時に一括印刷画面を表示する
			if(m_DispIkkatDlg)			cm.bDispIkkatDlg = TRUE;
			else						cm.bDispIkkatDlg = FALSE;
			// 起動時に様式選択画面を表示する
			if(m_DispYoushikiSelect)	cm.bDispYoushikiSelect = TRUE;
			else						cm.bDispYoushikiSelect = FALSE;
			// Enterキーでカーソルを下に移動する
			if(m_ReturnMoveDown)		cm.bReturnMoveDown = TRUE;
			else						cm.bReturnMoveDown = FALSE;
// midori 160612 add -->
			// 入力データを確定する
			if(m_DataKakutei)			cm.bDataKakutei = TRUE;
			else						cm.bDataKakutei = FALSE;
// midori 160612 add <--
// midori 190301 add -->
			// 使用する様式
			if(bG_Kanso)				cm.bUseStyle = TRUE;
			else						cm.bUseStyle = FALSE;
// midori 190301 add <--
// インボイス登録番号追加対応_23/11/06 add -->
			// 使用する様式（登録番号追加版）
			if(bG_InvNo)				cm.bUseStyle2 = TRUE;
			else						cm.bUseStyle2 = FALSE;
// インボイス登録番号追加対応_23/11/06 add <--

			cm.Set(rsMain);
			rsMain.Update();
		}
		rsMain.Fin();
	}

	// 様式移動モードが変更されていた場合、様式移動ボタンの状態更新
	if( bYoushikiSkipOld != m_YoushikiSkip ){
		UpdateYoushikiMoveButton( m_YoushikiSkip );
	}

// midori 190425_3 del -->
//// midori 190301 add -->
//	// 使用する様式が変更されていた場合、様式を切り替える
//	if(bUseStyleOld != bG_Kanso)	{
//		// フォーカスを先頭に戻すことで、先頭項目の表示が一瞬消えるため
//		// ウインドウをロックする
//		((CMainFrame*)AfxGetMainWnd())->LockWindowUpdate();
//		CmnDiagSetFocus(m_pDiag,1,1);
//		// カレント情報などを保存
//		CmnUcInfSubSetCurrentInfo();
//		// 空行データの削除処理
//		if(CmnTblCheckDeleteData() == TRUE)	{
//			CmnTblEndDeleteData();
//		}
//		// 新様式⇔旧様式の切り替えにより行数が変化するデータの書き換え
//		((CUCHIWAKEApp*)AfxGetApp())->KansoDataConvert();
//		// 表示する様式の切替を行う
//		((CUCHIWAKEApp*)AfxGetApp())->ChangeDocument(m_uInfo.intFormSeq,1);
//		((CMainFrame*)AfxGetMainWnd())->UnlockWindowUpdate();
//	}
//// midori 190301 add -->
// midori 190425_3 del <--

// midori 160612 add -->
	// 入力データを確定するが変更されていた場合、画面の状態更新
	if( bDataKakuteiOld != m_DataKakutei )	{
// midori UC_0053 add -->
		// 勘定科目内訳書（非連動マスターの場合
		if(((CUCHIWAKEApp*)AfxGetApp())->m_swUtwTandoku == 1 ) {
			// zvolumeテーブルの確定サインを更新する
			if (((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub->VolumeOpen() != 0) {
				// エラー
				m_clsFunc.ZmSubErrMsg( ((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub );
// 24/01/30_23-0845 add -->
				// 参照モードのためONに戻す
				if(((CUCHIWAKEApp*)AfxGetApp())->m_CloudSansyoFlg2 == TRUE)	{
					m_DataKakutei = TRUE;
				}
// 24/01/30_23-0845 add <--
				return;
			}
			((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub->zvol->Edit();
			if(m_DataKakutei)	((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub->zvol->tl_cor &= ~0x10;
			else				((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub->zvol->tl_cor |= 0x10;
			((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub->zvol->Update();
			((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub->VolumeClose();
		}
// midori UC_0053 add <--

		// １ページ再表示
		virUpdateControlTblData();

		// ボタン操作
		CmnChangeButtonEnable(2);

		// フォーカスセット
		if(m_DataKakutei == FALSE)	{
			intIndex = CmnGetControlIndex(m_uInfo.intCurRow,m_uInfo.intCurCol);
			m_pDiag->SetPosition(intIndex);
			m_uInfo.intCurFocus = 1;
		}
		else	{
			((CICSDiag *)GetDlgItem(IDC_PAGEDIAG1))->SetPosition(1);
			m_uInfo.intCurFocus = 0;
		}

		// 画面右下に「入力確定」を表示する
// midori 152766 del -->
//		AfxGetMainWnd()->PostMessage( WM_USER_UPDATE_INDICATE, 0 );
// midori 152766 del <--
// midori 152766 add -->
		AfxGetMainWnd()->PostMessage( WM_USER_UPDATE_INDICATE, 1 );
// midori 152766 add <--

// 改良No.22-0811 add -->
		// メニューバー「財務連動」の有効無効切り替え
		CmnMenuEnableSwitch();
// 改良No.22-0811 add <--
	}
// midori 160612 add <--

// midori 190425_3 add -->
	// 使用する様式が変更されていた場合、様式を切り替える
// インボイス登録番号追加対応_23/11/06 del -->
//	if(bUseStyleOld != bG_Kanso)	{
// インボイス登録番号追加対応_23/11/06 del <--
// インボイス登録番号追加対応_23/11/06 add -->
	if(bUseStyleOld2 != bG_InvNo)	{
		// フォーカスを先頭に戻すことで、先頭項目の表示が一瞬消えるため
		// ウインドウをロックする
		((CMainFrame*)AfxGetMainWnd())->LockWindowUpdate();
		// CmnDiagSetFocusで行われる画面の保存処理に影響があるため、
		// bG_InvNoの状態を変更前に一時的に戻す
		svbG_InvNo = bG_InvNo;
		bG_InvNo = bUseStyleOld2;

		CmnDiagSetFocus(m_pDiag,1,1);
		// カレント情報などを保存
		CmnUcInfSubSetCurrentInfo();
		// 空行データの削除処理
		if(CmnTblCheckDeleteData() == TRUE)	{
			CmnTblEndDeleteData();
		}
		bG_InvNo = svbG_InvNo;

// 改良No.21-0086,21-0529 add -->
		// 旧様式（簡素化）に切り替えた際に、様式⑧の科目指定出力のサインがONになっている場合はOFFに変更する
		if(bG_InvNo == FALSE)	{
			CdbUcInfSub		mfcSub(m_pDB);	//	uc_inf_subテーブルクラス

			try	{
				if(mfcSub.RequeryFormSeq(ID_FORMNO_081) == DB_ERR_OK)	{
					// レコードあり？
					if(!mfcSub.IsEOF())	{
						// 先頭に移動
						mfcSub.MoveFirst();
						// ----------------------------------------
						mfcSub.Edit();
						// 科目指定－科目指定を行う
						if(mfcSub.m_GeneralVar[1] & BIT_D3)	{
							mfcSub.m_GeneralVar[1] &= ~BIT_D3;
						}
						// ソート－科目行を挿入する
						CCtrlSubGeneralVar	cs(mfcSub);
						cs.bKamokuTitle = 0;
						cs.Set(mfcSub);

						mfcSub.Update();
						// ----------------------------------------
					}
				}
				mfcSub.Fin();
			}
			catch(...)	{
				// レコード開いてる？
				if(mfcSub.IsOpen())	{
					// レコード閉じる
					mfcSub.Fin();
				}
			}
		}
// 改良No.21-0086,21-0529 add <--

		// 新様式⇔旧様式の切り替えによるデータの書き換え
		// インボイス登録番号を追加したことにより、様式②と様式⑧の支払銀行、支払銀行支店名に改行マークを差し込む
		((CUCHIWAKEApp*)AfxGetApp())->BankNameResetConvert();
		// 表示する様式の切替を行う
		((CUCHIWAKEApp*)AfxGetApp())->ChangeDocument(m_uInfo.intFormSeq,1);
		((CMainFrame*)AfxGetMainWnd())->UnlockWindowUpdate();
	}
	else if(bUseStyleOld != bG_Kanso)	{
// インボイス登録番号追加対応_23/11/06 add <--
		// フォーカスを先頭に戻すことで、先頭項目の表示が一瞬消えるため
		// ウインドウをロックする
		((CMainFrame*)AfxGetMainWnd())->LockWindowUpdate();
// midori 200101 add -->
		// CmnDiagSetFocusで行われる画面の保存処理に影響があるため、
		// bG_Kansoの状態を変更前に一時的に戻す
		svbG_Kanso = bG_Kanso;
		bG_Kanso = bUseStyleOld;
// midori 200101 add <--
		CmnDiagSetFocus(m_pDiag,1,1);
		// カレント情報などを保存
		CmnUcInfSubSetCurrentInfo();
		// 空行データの削除処理
		if(CmnTblCheckDeleteData() == TRUE)	{
			CmnTblEndDeleteData();
		}
// midori 200101 add -->
		bG_Kanso = svbG_Kanso;
// midori 200101 add <--
		// 新様式⇔旧様式の切り替えにより行数が変化するデータの書き換え
		((CUCHIWAKEApp*)AfxGetApp())->KansoDataConvert();
// 20-0450 add -->
		((CUCHIWAKEApp*)AfxGetApp())->SyuruiMeigaraConvert(0,bG_Kanso);
// 20-0450 add <--
		// 表示する様式の切替を行う
		((CUCHIWAKEApp*)AfxGetApp())->ChangeDocument(m_uInfo.intFormSeq,1);
		((CMainFrame*)AfxGetMainWnd())->UnlockWindowUpdate();
	}
// midori 190425_3 add <--
// 24/01/30_23-0845 add -->
	// 参照モードのためONに戻す
	if(((CUCHIWAKEApp*)AfxGetApp())->m_CloudSansyoFlg2 == TRUE)	{
		m_DataKakutei = TRUE;
	}
// 24/01/30_23-0845 add <--
}

// ［他マスターから設定情報のコピー］メニュー
void CfrmUc000Common::OnMenuCopySettingInfo()
{
// midori 181002 add -->
	int		rv=0;
// midori 181002 add <--

	// 実行前の準備処理
	CmnEventStartProc(ID_COPY_SETTING_INFO_MENU);

	CdlgCopySettingInfo	dlg(this);
	// ダイアログ表示
// midori 181002 del -->
//// midori 160612 cor -->
////	if( dlg.ShowDialog( m_pDB ) != ID_DLG_OK ){
//// ---------------------
//	if( dlg.ShowDialog( m_pDB, m_DataKakutei ) != ID_DLG_OK ){
//		return;
//	}
//// midori 160612 cor <--
// midori 181002 del <--
// midori 181002 add -->
	rv = dlg.ShowDialog(m_pDB,m_DataKakutei);
	if(rv == ID_DLG_CANCEL)	{
		return;
	}
	else if(rv == -1)	{
		ICSMessageBox(_T("処理を終了します。"));
		AfxGetMainWnd()->PostMessage(WM_CLOSE,1,0);
		return;
	}
// midori 181002 add <--

	// 次の再表示によりダイアグラムが切り替わるときはTRUE
	m_ChangeFont = TRUE;
	// １ページ再表示
	virUpdateControlTblData();

	// 更新されたタイトルを取得
	CdbUcInfSub		rsSub(m_pDB);
	if (rsSub.RequeryFormSeq(m_uInfo.intFormSeq) == DB_ERR_OK) {
		if (!rsSub.IsEOF()) {
			m_uInfo.strTitleName = rsSub.m_Title;
		}
	}
	rsSub.Fin();
	// タイトル更新
	virUpdateControlTitle();

// midori M-16090701 add -->
	// カーソル移動
	CmnDiagSetFocus( m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol );
// midori M-16090701 add <--
}

// ［様式別全データ削除］メニュー
void CfrmUc000Common::OnMenuDeleteDataEachYoushiki()
{
	// 実行前の準備処理
	CmnEventStartProc(ID_DELETE_DATA_MENU);

	CdlgDeleteDataEachYoushiki	dlg(this);
	CMap<int, int, DELETE_YOUSHIKI, DELETE_YOUSHIKI> map;

	map.InitHashTable( UC_ID_FORMNO_MAX + 10/*最大値の20％を指定すると高速になるようだ*/);

	// ダイアログ表示
	if( dlg.ShowDialog( m_pDB, map ) != ID_DLG_OK )			return;

	// 様式テーブル取得
	CdbUc000Common* rsData;

	// 様式削除	
	POSITION pos = map.GetStartPosition();
	while( pos != NULL ){
		int nKey = 0;
		DELETE_YOUSHIKI stValue;

		// キーと値取得
		map.GetNextAssoc( pos, nKey, stValue );

		// DBオブジェクト取得
		rsData = m_clsFunc.CreateDbObject( nKey, m_pDB );

		// 削除
		if( stValue.tsujo != FALSE && stValue.hokan != FALSE ){
			rsData->DeleteAllRecord();
// midori 155014 add -->
			// 棚卸資産の内訳書、買掛金の内訳書は欄外登録の内容も削除する
			if((nKey == ID_FORMNO_051 && bG_Kanso == FALSE) || nKey == ID_FORMNO_091)	{
				rsData->Fin();
				delete rsData;
				if(nKey == ID_FORMNO_051)	{
					rsData = (CdbUc000Common*)(new CdbUc052Tanaoroshi2(m_pDB));
				}
				else	{
					rsData = (CdbUc000Common*)(new CdbUc092Kaikakekin2(m_pDB));
				}
				rsData->DeleteAllRecord();
			}
// midori 155014 add <--

			// 保管テーブルも削除
			CdbUcLstHokan mfcRec( m_pDB );
			mfcRec.DeleteFormSeq( nKey );

			mfcRec.Fin();
		}
		else if( stValue.tsujo != FALSE ){
			rsData->DeleteFgShow( ID_FGSHOW_OFF );
// midori 156774 add -->
			rsData->DeleteFgShow( ID_FGSHOW_IKKATU );
// midori 156774 add <--
// midori 154982 add -->
			// 棚卸資産の内訳書、買掛金の内訳書は欄外登録の内容も削除する
			if((nKey == ID_FORMNO_051 && bG_Kanso == FALSE) || nKey == ID_FORMNO_091)	{
				rsData->Fin();
				delete rsData;
				if(nKey == ID_FORMNO_051)	{
					rsData = (CdbUc000Common*)(new CdbUc052Tanaoroshi2(m_pDB));
				}
				else	{
					rsData = (CdbUc000Common*)(new CdbUc092Kaikakekin2(m_pDB));
				}
				rsData->DeleteFgShow(ID_FGSHOW_OFF);
			}
// midori 154982 add <--
		}
		else if( stValue.hokan != FALSE ){
			rsData->DeleteFgShow( ID_FGSHOW_HOKAN );

			// 保管テーブルも削除
			CdbUcLstHokan mfcRec( m_pDB );
			mfcRec.DeleteFormSeq( nKey );

			mfcRec.Fin();
		}

		rsData->Fin();
		delete rsData;
	}	
	rsData = NULL;

	// 最大ページ数を更新(1以上だとページ挿入メッセージが表示されるため)
	m_uInfo.intMaxPage = CmnTblGetPageMax();

	// テーブルデータ無しなら１頁分のデータを作成
	CmnTblInitialMakeData();

	// 次の再表示によりダイアグラムが切り替わるときはTRUE
	m_ChangeFont = TRUE;
	// １ページ再表示
	virUpdateControlTblData();

	// マップ削除
	map.RemoveAll();

// midori M-16090701 add -->
	// カーソル移動
	CmnDiagSetFocus( m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol );
// midori M-16090701 add <--
}

// midori 180104 add -->
// ［顧問先情報の取込］メニュー監視
void CfrmUc000Common::OnFromkomonMenu()
{
// 要通知案件修正 21/07/09 add -->
	// 実行前の準備処理
	CmnEventStartProc(ID_FROMKOMON_MENU);
// 要通知案件修正 21/07/09 add <--

	m_clsFunc.SetCpdataByCpinfo(1,this);
}

void CfrmUc000Common::OnUpdateFromkomonMenu(CCmdUI *pCmdUI)
{
// 修正No.159012 del -->
	//pCmdUI->Enable( TRUE );
// 修正No.159012 del <--
// 修正No.159012 add -->
	int			type = 0;
	BOOL		bflg = TRUE;

	bflg = TRUE;
	//GET_USERTYPE()	返送値 1：dbWL 2：上手くんα
	type = GET_USERTYPE();
	if(IsCompanyPackage() != TRUE && (type == 1 || type == 2))	{
		bflg = FALSE;
	}
	pCmdUI->Enable( bflg );
// 修正No.159012 add <--

// midori 153984 add -->
	if(IsCompanyPackage() == TRUE)	{
		pCmdUI->SetText(_T("事業所情報の取込(&C)"));
	}
	else	{
		pCmdUI->SetText(_T("顧問先情報の取込(&C)"));
	}
// midori 153984 add <--
}
// midori 180104 add <--

// midori 160612 add -->
// ［データインポート］メニュー監視
void CfrmUc000Common::OnUpdateMenuDataImport(CCmdUI *pCmdUI)
{
	BOOL			bFncEnable = FALSE;

	// 入力データを確定するにチェック無し
	if(m_DataKakutei == FALSE)	bFncEnable = TRUE;

	pCmdUI->Enable(bFncEnable);
}
// midori 160612 add <--

// ［特殊行挿入］メニュー監視
void CfrmUc000Common::OnUpdateMenuSpecialRow(CCmdUI *pCmdUI)
{
	BOOL			bFncEnable = FALSE;

// midori 160612 cor -->
//	// データ入力欄にフォーカスがある事
//	if (m_uInfo.intCurFocus == 1) {
//		// 合計行①，②は操作不可。それ以外は操作可能。
//		if (m_uInfo.intCurCtlIndex < m_uInfo.intT1StartIndex) {
//			// 操作可能
//			bFncEnable = TRUE;
//		}
//	}
// ---------------------
	// 入力データを確定するにチェック無し
	if(m_DataKakutei == FALSE)	{
		// データ入力欄にフォーカスがある事
		if (m_uInfo.intCurFocus == 1) {
			// 合計行①，②は操作不可。それ以外は操作可能。
			if (m_uInfo.intCurCtlIndex < m_uInfo.intT1StartIndex) {
				// 操作可能
				bFncEnable = TRUE;
			}
		}
	}
// midori 160612 cor <--
	pCmdUI->Enable(bFncEnable);
}

// midori 160612 add -->
// ［金額等数値0クリア］メニュー監視
void CfrmUc000Common::OnUpdateMenuClear(CCmdUI *pCmdUI)
{
	BOOL			bFncEnable = FALSE;

	// 入力データを確定するにチェック無し
	if(m_DataKakutei == FALSE)	bFncEnable = TRUE;

	pCmdUI->Enable(bFncEnable);
}
// midori 160612 add <--

// ［ソート］メニュー監視
void CfrmUc000Common::OnUpdateMenuSort(CCmdUI *pCmdUI)
{
// midori 160612 cor -->
//	return;
// ---------------------
	BOOL			bFncEnable = FALSE;

	// 入力データを確定するにチェック無し
	if(m_DataKakutei == FALSE)	bFncEnable = TRUE;

	pCmdUI->Enable(bFncEnable);
// midori 160612 cor <--
}

// ［一括金額追加］メニュー監視
void CfrmUc000Common::OnUpdateIkkatuaddMenu(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	BOOL			bFncEnable = FALSE;
	CURRENT_INFO	uCurInfo;

// midori 160612 cor -->
	//// 帳表のカレント情報を取得
	//virGetCurrentInfo(&uCurInfo);

	//// データ入力欄にフォーカスがある事
	//if (m_uInfo.intCurFocus == 1) {
	//	// 合計行①，②は操作不可。データ行のみ操作可能。
	//	if (m_uInfo.intCurCtlIndex < m_uInfo.intT1StartIndex) {
	//		if (uCurInfo.intFgFunc == ID_FGFUNC_DATA) {
	//			// 操作可能
	//			bFncEnable = TRUE;
	//		}
	//	}
	//}
// ---------------------
	// 入力データを確定するにチェック無し
	if(m_DataKakutei == FALSE)	{
		// 帳表のカレント情報を取得
		virGetCurrentInfo(&uCurInfo);

		// データ入力欄にフォーカスがある事
		if (m_uInfo.intCurFocus == 1) {
			// 合計行①，②は操作不可。データ行のみ操作可能。
			if (m_uInfo.intCurCtlIndex < m_uInfo.intT1StartIndex) {
				if (uCurInfo.intFgFunc == ID_FGFUNC_DATA) {
					// 操作可能
					bFncEnable = TRUE;
				}
			}
		}
	}
// midori 160612 cor <--
	pCmdUI->Enable(bFncEnable);
}

// ［一括金額参照］メニュー監視
void CfrmUc000Common::OnUpdateMenuIkkatuKingaku(CCmdUI* pCmdUI) 
{
	BOOL			bFncEnable = FALSE;
	CURRENT_INFO	uCurInfo;

// midori 160612 cor -->
	//// 帳表のカレント情報を取得
	//virGetCurrentInfo(&uCurInfo);

	//// データ入力欄にフォーカスがある事
	//if (m_uInfo.intCurFocus == 1) {
	//	// 合計行①，②は操作不可。自動一括集計金額行のみ操作可能。
	//	if (m_uInfo.intCurCtlIndex < m_uInfo.intT1StartIndex) {
	//		if (uCurInfo.intFgFunc == ID_FGFUNC_IKKATUAUTO) {
	//			// 操作可能
	//			bFncEnable = TRUE;
	//		}
	//	}
	//}
// ---------------------
	// 入力データを確定するにチェック無し
	if(m_DataKakutei == FALSE)	{
		// 帳表のカレント情報を取得
		virGetCurrentInfo(&uCurInfo);

		// データ入力欄にフォーカスがある事
		if (m_uInfo.intCurFocus == 1) {
			// 合計行①，②は操作不可。自動一括集計金額行のみ操作可能。
			if (m_uInfo.intCurCtlIndex < m_uInfo.intT1StartIndex) {
				if (uCurInfo.intFgFunc == ID_FGFUNC_IKKATUAUTO) {
					// 操作可能
					bFncEnable = TRUE;
				}
			}
		}
	}
// midori 160612 cor <--
	pCmdUI->Enable(bFncEnable);
}

// ［一時保管データ参照］メニュー監視
void CfrmUc000Common::OnUpdateMenuZeroMoney(CCmdUI* pCmdUI) 
{
// midori 160612 cor -->
	//BOOL			bFncEnable = FALSE;

	//// データ入力欄にフォーカスがある事
	//if (m_uInfo.intCurFocus == 1) {
	//	// 合計行①，②は操作不可。それ以外は操作可能。
	//	if (m_uInfo.intCurCtlIndex < m_uInfo.intT1StartIndex) {
	//		// 操作可能
	//		bFncEnable = TRUE;
	//	}
	//}
	//pCmdUI->Enable(bFncEnable);
// ---------------------
	// 常に有効にする
	pCmdUI->Enable(TRUE);
// midori 160612 cor <--
}

// ［一括で一時保管へ移動］メニュー監視
void CfrmUc000Common::OnUpdateMenuZeroMoneyMove(CCmdUI* pCmdUI) 
{
// midori 160612 cor -->
//	return;
// ---------------------
	BOOL			bFncEnable = FALSE;

	// 入力データを確定するにチェック無し
// midori 152389 del -->
	//if(m_DataKakutei == FALSE)		bFncEnable = TRUE;
// midori 152389 del <--
// midori 152389 add -->
	// 入力データを確定するのチェックＯＦＦでかつ「人件費の内訳書」以外
	if(m_DataKakutei == FALSE && m_uInfo.intFormSeq != ID_FORMNO_142) {
		bFncEnable = TRUE;
	}
// midori 152389 add <--
	pCmdUI->Enable(bFncEnable);
// midori 160612 cor <--
}

// ［財務連動実行］メニュー監視
void CfrmUc000Common::OnUpdateMenuZaimuRendo(CCmdUI* pCmdUI)
{
	CString strGrNum = m_clsFunc.GetGrNo();	// グループNOを取得する
	BOOL	bZaimu = FALSE;

// midori 160612 cor -->
//	if( strGrNum.CompareNoCase( _T("1B") ) == 0 ){
//		bZaimu = TRUE;
//	}
// ---------------------
	// 入力データを確定するにチェック無し
	if(m_DataKakutei == FALSE)	{
		if( strGrNum.CompareNoCase( _T("1B") ) == 0 ){
// midori 180104 del -->
			//bZaimu = TRUE;
// midori 180104 del <--
// midori 180104 add -->
// midori UC_0015 del -->
			//if(bTantai == FALSE)	{
// midori UC_0015 del <--
// midori UC_0015 add -->
// midori UC_0037 del -->
			//if(((CUCHIWAKEApp*)AfxGetApp())->m_swUtwInfoRegister == 0)	{
// midori UC_0037 del <--
// midori UC_0037 add -->
			if(((CUCHIWAKEApp*)AfxGetApp())->m_swUtwTandoku == 0)	{
// midori UC_0037 add <--
// midori UC_0015 add <--
				bZaimu = TRUE;
			}
// midori 180104 add <--
		}
	}
// midori 160612 cor <--

	// 財務連動実行フラグがTRUEかつGRNOがGR1bなら有効
	pCmdUI->Enable(m_uInfo.bRenFlg & bZaimu);
}

// midori 160612 add -->
// ［タイトル初期化］メニュー監視
void CfrmUc000Common::OnUpdateMenuTitleRestore(CCmdUI *pCmdUI)
{
	BOOL			bFncEnable = FALSE;

	// 入力データを確定するにチェック無し
	if(m_DataKakutei == FALSE)	bFncEnable = TRUE;
	pCmdUI->Enable(bFncEnable);
}

// ［様式別全データ削除］メニュー監視
void CfrmUc000Common::OnUpdateMenuDeleteDataEachYoushiki(CCmdUI *pCmdUI)
{
	BOOL			bFncEnable = FALSE;

	// 入力データを確定するにチェック無し
	if(m_DataKakutei == FALSE)	bFncEnable = TRUE;
	pCmdUI->Enable(bFncEnable);
}
// midori 160612 add <--

//------------------------	ページ操作関連　------------------------

// ［前頁］ボタン
void CfrmUc000Common::OnPageBack()
{
	// 実行前の準備処理
	CmnEventStartProc(IDC_PAGEBACK_BUTTON1);

// 修正No.158301 del -->
//// 157382 add -->
//	if(m_uInfo.intFormSeq == ID_FORMNO_021)	{
//		if(m_uInfo.intCurCol == ID_COL_021_SPDATE || m_uInfo.intCurCol == ID_COL_021_PMDATE)	{
//			CmnDiagSetFocus(m_pDiag,m_uInfo.intCurRow,ID_COL_021_KAMOKU);
//		}
//	}
//// 157382 add <--
//// 修正No.157633_元号日付 add -->
//	else if(m_uInfo.intFormSeq == ID_FORMNO_061)	{
//		if(bG_Kanso == TRUE && m_uInfo.intCurCol == ID_COL_061K_SPDATE)	{
//			CmnDiagSetFocus(m_pDiag,m_uInfo.intCurRow,ID_COL_061K_KUBUN);
//		}
//		else if(bG_Kanso == FALSE && m_uInfo.intCurCol == ID_COL_061_SPDATE)	{
//			CmnDiagSetFocus(m_pDiag,m_uInfo.intCurRow,ID_COL_061_KUBUN);
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_071)	{
//		if(m_uInfo.intCurCol == ID_COL_071_SPDATE || m_uInfo.intCurCol == ID_COL_071_SPDATE2)	{
//			CmnDiagSetFocus(m_pDiag,m_uInfo.intCurRow,ID_COL_071_SYURUI);
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_081)	{
//		if(m_uInfo.intCurCol == ID_COL_081_SPDATE || m_uInfo.intCurCol == ID_COL_081_PMDATE)	{
//			CmnDiagSetFocus(m_pDiag,m_uInfo.intCurRow,ID_COL_081_ADNAME1);
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_102)	{
//// 修正No.157883,157884 del -->
//		//if(m_uInfo.intCurCol == ID_COL_102_SPDATE)	{
//// 修正No.157883,157884 del <--
//// 修正No.157883,157884 add -->
//		if(m_uInfo.intCurCol == ID_COL_102_SPDATE || m_uInfo.intCurCol == ID_COL_102_VAL)	{
//// 修正No.157883,157884 add <--
//			CmnDiagSetFocus(m_pDiag,m_uInfo.intCurRow,ID_COL_102_KNORDER);
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_121)	{
//		if(m_uInfo.intCurCol == ID_COL_121_SPDATE)	{
//			CmnDiagSetFocus(m_pDiag,m_uInfo.intCurRow,ID_COL_121_KUBUN);
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_151)	{
//		if(m_uInfo.intCurCol == ID_COL_151_SPDATE || m_uInfo.intCurCol == ID_COL_151_SPDATE2)	{
//			CmnDiagSetFocus(m_pDiag,m_uInfo.intCurRow,ID_COL_151_KUBUN);
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_152)	{
//		if(m_uInfo.intCurCol == ID_COL_152_SPDATE)	{
//			CmnDiagSetFocus(m_pDiag,m_uInfo.intCurRow,ID_COL_152_ADNAME1);
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_153)	{
//		if( m_uInfo.intCurCol == ID_COL_153_KYDATE || m_uInfo.intCurCol == ID_COL_153_KYDATE2 ||
//			m_uInfo.intCurCol == ID_COL_153_SPDATE || m_uInfo.intCurCol == ID_COL_153_SPDATE2 )	{
//			CmnDiagSetFocus(m_pDiag,m_uInfo.intCurRow,ID_COL_153_HDNAME);
//		}
//	}
//// 修正No.157633_元号日付 add <--
// 修正No.158301 del <--
// 修正No.158301 add -->
	int		sign = 0;
	int		colmset = -1;

	sign = CmnTblDataToFirstCol(&colmset);
	if(sign == 1) {
		//CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, colmset);
		m_pDiag->DeleteInput();
	}
// 修正No.158301 add <--

	// 前頁へ
	if (m_uInfo.intCurPage > 1) {
		m_uInfo.intCurPage = m_uInfo.intCurPage - 1;
	}

	// １ページ再表示
	m_ChangeFont = TRUE;
	virUpdateControlTblData();

	// フォーカスをデータ入力欄に
	CmnDiagSetFocus( m_pDiag, 1, 1 );
}

// ［次頁］ボタン
void CfrmUc000Common::OnPageNext()
{
	// 実行前の準備処理
	CmnEventStartProc(IDC_PAGENEXT_BUTTON1);

// 修正No.158301 del -->
//// 157382 add -->
//	if(m_uInfo.intFormSeq == ID_FORMNO_021)	{
//		if(m_uInfo.intCurCol == ID_COL_021_SPDATE || m_uInfo.intCurCol == ID_COL_021_PMDATE)	{
//			CmnDiagSetFocus(m_pDiag,m_uInfo.intCurRow,ID_COL_021_KAMOKU);
//		}
//	}
//// 157382 add <--
//// 修正No.157633_元号日付 add -->
//	else if(m_uInfo.intFormSeq == ID_FORMNO_061)	{
//		if(bG_Kanso == TRUE && m_uInfo.intCurCol == ID_COL_061K_SPDATE)	{
//			CmnDiagSetFocus(m_pDiag,m_uInfo.intCurRow,ID_COL_061K_KUBUN);
//		}
//		else if(bG_Kanso == FALSE && m_uInfo.intCurCol == ID_COL_061_SPDATE)	{
//			CmnDiagSetFocus(m_pDiag,m_uInfo.intCurRow,ID_COL_061_KUBUN);
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_071)	{
//		if(m_uInfo.intCurCol == ID_COL_071_SPDATE || m_uInfo.intCurCol == ID_COL_071_SPDATE2)	{
//			CmnDiagSetFocus(m_pDiag,m_uInfo.intCurRow,ID_COL_071_SYURUI);
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_081)	{
//		if(m_uInfo.intCurCol == ID_COL_081_SPDATE || m_uInfo.intCurCol == ID_COL_081_PMDATE)	{
//			CmnDiagSetFocus(m_pDiag,m_uInfo.intCurRow,ID_COL_081_ADNAME1);
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_102)	{
//// 修正No.157883,157884 del -->
//		//if(m_uInfo.intCurCol == ID_COL_102_SPDATE)	{
//// 修正No.157883,157884 del <--
//// 修正No.157883,157884 add -->
//		if(m_uInfo.intCurCol == ID_COL_102_SPDATE || m_uInfo.intCurCol == ID_COL_102_VAL)	{
//// 修正No.157883,157884 add <--
//			CmnDiagSetFocus(m_pDiag,m_uInfo.intCurRow,ID_COL_102_KNORDER);
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_121)	{
//		if(m_uInfo.intCurCol == ID_COL_121_SPDATE)	{
//			CmnDiagSetFocus(m_pDiag,m_uInfo.intCurRow,ID_COL_121_KUBUN);
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_151)	{
//		if(m_uInfo.intCurCol == ID_COL_151_SPDATE || m_uInfo.intCurCol == ID_COL_151_SPDATE2)	{
//			CmnDiagSetFocus(m_pDiag,m_uInfo.intCurRow,ID_COL_151_KUBUN);
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_152)	{
//		if(m_uInfo.intCurCol == ID_COL_152_SPDATE)	{
//			CmnDiagSetFocus(m_pDiag,m_uInfo.intCurRow,ID_COL_152_ADNAME1);
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_153)	{
//		if( m_uInfo.intCurCol == ID_COL_153_KYDATE || m_uInfo.intCurCol == ID_COL_153_KYDATE2 ||
//			m_uInfo.intCurCol == ID_COL_153_SPDATE || m_uInfo.intCurCol == ID_COL_153_SPDATE2 )	{
//			CmnDiagSetFocus(m_pDiag,m_uInfo.intCurRow,ID_COL_153_HDNAME);
//		}
//	}
//// 修正No.157633_元号日付 add <--
// 修正No.158301 del <--
// 修正No.158301 add -->
	int		colmset = -1;
	int		sign = 0;
	sign = CmnTblDataToFirstCol(&colmset);
	if(sign == 1)	{
		//CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, colmset);
		m_pDiag->DeleteInput();
	}
// 修正No.158301 add <--

	// 次頁へ
	if (m_uInfo.intCurPage < m_uInfo.intMaxPage) {
		m_uInfo.intCurPage = m_uInfo.intCurPage + 1;
	}

	// １ページ再表示
	m_ChangeFont = TRUE;
	virUpdateControlTblData();
	
	// フォーカスをデータ入力欄に
	CmnDiagSetFocus( m_pDiag, 1, 1 );
}

// ［改頁］ボタン
void CfrmUc000Common::OnPageNew()
{
	int			nTargetPage;			// 挿入位置の頁番号
	int			nTargetRow;				// 挿入位置の行番号
	int			nTargetCount;			// 挿入行数
// midori 20/10/14 add -->
	int			nMaxPage;				// 最大頁数
// midori 20/10/14 add <--
	CURRENT_INFO uCurInfo;
// midori 20/10/14 add -->
	CdbUc000Common*	rsData;
// midori 20/10/14 add <--
	// 実行前の準備処理
	CmnEventStartProc(IDC_PAGENEW_BUTTON1);
	
	// カレント行にインバースをかける
	virGetCurrentInfo(&uCurInfo);
// 修正No.157627 add -->
	CmnRowBeforeInverse(&uCurInfo);
// 修正No.157627 add <--
	CmnRowInverse( TRUE , &uCurInfo );

	//	確認メッセージでＯＫが押された？
	if ( ICSMessageBox( _T("現在入力行から以降を次頁へ改頁します。"), MB_OKCANCEL, 0, 0, this ) == IDOK ){
		
		// カレント行のインバースを解除
		CmnRowInverse( FALSE , &uCurInfo );
// 修正No.157627 add -->
		CmnRowAfterInverse(&uCurInfo);
// 修正No.157627 add <--

// 157153 add -->
		InsertNull();
// 157153 add <--

		// カレント頁のカレント行から１頁分のデータ行数を空行で挿入する
		nTargetPage = m_uInfo.intCurPage;
		nTargetRow = m_uInfo.intCurRow;
		nTargetCount = m_uInfo.intRowMaxData - m_uInfo.intCurRow + 1;
		
		// 頁と行の再設定 ･･･ 挿入時は挿入前に頁と行をズラす。
		CmnTblUpdatePageNumRowNumAdd(nTargetPage, nTargetRow, nTargetCount);
		
// midori 154714 add -->
		// 新しくできた頁に空行以外のデータが無ければ、新しくできた頁を削除する
		CmnTblUpdateNewPageCheckAndDelete();
// midori 154714 add <--

		// 改頁分の空行を挿入
		CmnTblInsertNullLine(nTargetPage, nTargetRow, nTargetCount);

		// 頁と行の調整処理（挿入後処理）
		CmnTblDeleteFgFunc(ID_FGFUNC_PAGEKEI);					// 頁計の削除
		CmnTblDeleteFgFunc(ID_FGFUNC_RUIKEI);					// 累計の削除
	//	CmnTblUpdateEndPageSetting();							// 最終頁を調整する ･･･ 改頁では実行しない
		CmnTblInsertPageKeiRuiKei();							// 頁計/累計挿入（最終ページの削除or空行追加）
		
		// 小計/中計/頁計/累計の再計算
		CmnTblCalcKei();

		// カレントを次頁の先頭行に移動
// midori 20/10/14 del -->
		//m_uInfo.intCurPage = m_uInfo.intCurPage + 1;
// midori 20/10/14 del <--
// midori 20/10/14 add -->
		rsData = virGetDataTableObject();
		nMaxPage = rsData->GetNumPage();
		// intCurPageが最大頁より小さい場合、ページ数を＋１する
		if(m_uInfo.intCurPage < nMaxPage) {
			m_uInfo.intCurPage = m_uInfo.intCurPage + 1;
		}
		delete rsData;
// midori 20/10/14 add <--
		m_uInfo.intCurRow = 1;
		m_uInfo.intCurCol = 1;

		// 次の再表示によりダイアグラムが切り替わるときはTRUE
		m_ChangeFont = TRUE;
		// １ページ再表示
		virUpdateControlTblData();
	}
	else{
		// カレント行のインバースを解除
		CmnRowInverse( FALSE , &uCurInfo );
// 修正No.157627 add -->
		CmnRowAfterInverse(&uCurInfo);
// 修正No.157627 add <--
	}

	// カーソル移動
	CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
}

// ［頁挿入］ボタン
void CfrmUc000Common::OnPageInsert()
{
	int			nTargetPage;			// 挿入位置の頁番号
	int			nTargetRow;				// 挿入位置の行番号
	int			nTargetCount;			// 挿入行数
	int			nRet = IDOK;			// 頁挿入確認メッセージの戻値格納

//	// 実行前の準備処理
//	CmnEventStartProc(IDC_PAGEINSERT_BUTTON1);

	// 次頁の先頭行から１頁分のデータ行数を空行で挿入する
	if (m_uInfo.intMaxPage > 0) {
		// 実行前の準備処理：これを初期のデータ作成処理で実行するとタイトルがクリアされてしまう
		CmnEventStartProc(IDC_PAGEINSERT_BUTTON1);
		
		//	確認メッセージ
		nRet = ICSMessageBox( _T("次頁に新規頁を挿入します。"), MB_OKCANCEL, 0, 0, this );
		
		nTargetPage = m_uInfo.intCurPage + 1;
	}
	else {
		nTargetPage = 1;		// 帳表の初期表示でレコード無しなら１頁目挿入する対応
	}
	nTargetRow = 1;
	nTargetCount = m_uInfo.intRowMaxData;
	
	//	確認メッセージでＯＫが押された？
	if ( nRet == IDOK ){

		// 頁と行の再設定 ･･･ 挿入時は挿入前に頁と行をズラす。
		CmnTblUpdatePageNumRowNumAdd(nTargetPage, nTargetRow, nTargetCount);
		
		// １頁分の空行を挿入
		CmnTblInsertNullLine(nTargetPage, nTargetRow, nTargetCount);

		// 頁と行の調整処理（挿入後処理）
		CmnTblDeleteFgFunc(ID_FGFUNC_PAGEKEI);					// 頁計の削除
		CmnTblDeleteFgFunc(ID_FGFUNC_RUIKEI);					// 累計の削除
		CmnTblUpdatePageNumRowNumReset(1, 1);					// 先頭頁からページ再設定（最終頁で頁挿入した場合に頁番号再構築が必要）
																// 「計なし」帳表で「累計のみ」設定の場合に頁番号が崩れるため、必要な処理である。
	//	CmnTblUpdateEndPageSetting();							// 最終頁を調整する ･･･ 改頁では実行しない
		CmnTblInsertPageKeiRuiKei();							// 頁計/累計挿入（最終ページの削除or空行追加）

		// 小計/中計/頁計/累計の再計算
		CmnTblCalcKei();

		// カレントを次頁の先頭行に移動
		m_uInfo.intCurPage = nTargetPage;		
		m_uInfo.intCurRow = 1;
		m_uInfo.intCurCol = 1;
		
		// 次の再表示によりダイアグラムが切り替わるときはTRUE
		m_ChangeFont = TRUE;
		// １ページ再表示
		virUpdateControlTblData();
	}

	// カーソル移動
	CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
}
//------------------------	行操作関連　------------------------

// ［行コピー］ボタン
void CfrmUc000Common::OnRowCopy() 
{
	// 事前チェック
	if (CmnCheckRowButtonProc(0) == TRUE) {
		// 実行前の準備処理
		CmnEventStartProc(IDC_ROWCOPY_BUTTON1);

		// 行コピー処理
		virRowButtonCopyProc();
	}

	// カーソル移動処理（元のセルへ）
	CmnDiagSetFocus( m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol );
}

// ［行貼付］ボタン
void CfrmUc000Common::OnRowPaste() 
{
	// 事前チェック
	if (CmnCheckRowButtonProc(1) == TRUE) {
		// 実行前の準備処理
		CmnEventStartProc(IDC_ROWPASTE_BUTTON1);

		// コピーデータ（カレント行位置）をDBに保存
		virRecBufSaveDataForPaste( m_uInfo.intCurPage, m_uInfo.intCurRow );

		// 計再計算
		CmnTblCalcKei();

		// 1頁再表示
		virUpdateControlTblData();
	}
	// カーソル移動処理（元のセルへ）
	CmnDiagSetFocus( m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol );
}

// ［行挿入貼付］ボタン
void CfrmUc000Common::OnRowInsert() 
{
	// 事前チェック
	if (CmnCheckRowButtonProc(2) == TRUE) {
		// 実行前の準備処理
		CmnEventStartProc(IDC_ROWINSERT_BUTTON1);

// midori 156968 add -->
		// 最終行（頁の最大行）が小計の場合、小計の下に空行を追加する
		InsertNull();
// midori 156968 add <--

		// 頁番号行番号構築
		CmnTblUpdatePageNumRowNumAdd( m_uInfo.intCurPage, m_uInfo.intCurRow, 1 );

		// 空行挿入
		CmnTblInsertNullLine( m_uInfo.intCurPage, m_uInfo.intCurRow, 1 );

		// コピーデータ（カレント行位置）をDBに保存
		virRecBufSaveDataForPasteInsert( m_uInfo.intCurPage, m_uInfo.intCurRow );

		// 頁と行の調整処理（挿入後処理）
		CmnTblDeleteFgFunc(ID_FGFUNC_PAGEKEI);					// 頁計の削除
		CmnTblDeleteFgFunc(ID_FGFUNC_RUIKEI);					// 累計の削除
		CmnTblUpdateEndPageSetting();							// 最終頁を調整する
		CmnTblInsertPageKeiRuiKei();							// 頁計/累計挿入（最終ページの削除or空行追加）

		// 計再計算
		CmnTblCalcKei();

// midori 156968 add -->
		// 先頭行が空行の場合削除する
		CmnTblUpdatePageRowAllResetMain(6);
// midori 156968 add <--

		// 1頁再表示
		virUpdateControlTblData();
	}
	// カーソル移動処理
	CmnDiagSetFocus( m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol );
}

// ［前様式］ボタン
void CfrmUc000Common::OnMoveYoushikiBack()
{
	int				nNowFormSeq		= 0;
	CdbUcInfMain	mfcRec( m_pDB );			//	uc_inf_mainテーブル

	// 実行前の準備処理
	CmnEventStartProc( ID_BUTTON_F7 );

	if ( m_uInfo.intFormSeq <= ID_FORMNO_011 ) {
		// 現在の様式が始端の場合、リターン
		return;
	}

// midori 154237,154238 add -->
	// フォーカスを先頭に戻すことで、先頭項目の表示が一瞬消えるため
	// ウインドウをロックする
	((CMainFrame*)AfxGetMainWnd())->LockWindowUpdate();

// midori 155872 del -->
	//CmnDiagSetFocus(m_pDiag,1,1);
// midori 155872 del <--
// midori 155872 add -->
	if(m_uInfo.intFormSeq == ID_FORMNO_102)	{
		CmnDiagSetFocus(m_pDiag,1,3);
	}
	else	{
		CmnDiagSetFocus(m_pDiag,1,1);
	}
// midori 155872 add <--
// midori 154237,154238 add <--

	// カレント情報などを保存
	CmnUcInfSubSetCurrentInfo();

	// 空行データの削除処理
	if ( CmnTblCheckDeleteData() == TRUE ){
		CmnTblEndDeleteData();
	}

	// 前の様式へ
	if( m_YoushikiSkip == FALSE ){	// 全て空行でも、1つ前の様式へ移動
		// 前の様式のシーケンス番号取得
		nNowFormSeq = CmnGetDestinationYoushiki( TRUE );	// TRUE:前の様式		
	}
	else{	// データ行がある様式へ移動
		// 現在の様式より前にデータ入力のある様式が存在する場合
		nNowFormSeq = m_clsFunc.IsExistData(m_pDB, m_uInfo.intFormSeq, TRUE, 1);
// midori 154237,154238 del -->
		//if( nNowFormSeq == -1 )
		//	return;
// midori 154237,154238 del <--
// midori 154237,154238 add -->
		if(nNowFormSeq == -1)	{
			((CMainFrame*)AfxGetMainWnd())->UnlockWindowUpdate();
			return;
		}
// midori 154237,154238 add <--
	}

	//	様式を移動する場合、inf_mainへ書き込み
	if( m_uInfo.intFormSeq != nNowFormSeq ){
		//	初期化成功？
		if ( mfcRec.Init() == DB_ERR_OK ){
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				mfcRec.MoveFirst();		//	レコードを先頭に移動
			}
		}
		mfcRec.Edit();					//	編集開始
		mfcRec.m_FormSeq = nNowFormSeq;	//	様式シーケンス番号変更
		mfcRec.Update();				//	更新
	}
	//	レコード閉じる
	mfcRec.Fin();

	//	レコード開いてる？
	if ( mfcRec.IsOpen() ){
		//	レコード閉じる
		mfcRec.Fin();
	}

	// 表示する様式の切替を行う
// midori 154237,154238 del -->
	//((CUCHIWAKEApp*)AfxGetApp())->ChangeDocument( nNowFormSeq );
// midori 154237,154238 del <--
// midori 154237,154238 add -->
	((CUCHIWAKEApp*)AfxGetApp())->ChangeDocument(nNowFormSeq,1);

	((CMainFrame*)AfxGetMainWnd())->UnlockWindowUpdate();
// midori 154237,154238 add <--

	return;	//	←このリターン重要
}

// ［次様式］ボタン
void CfrmUc000Common::OnMoveYoushikiNext()
{
	int				nNowFormSeq		= 0;
	CdbUcInfMain	mfcRec( m_pDB );			//	uc_inf_mainテーブル

	// 実行前の準備処理
	CmnEventStartProc(ID_BUTTON_F8);

	if ( m_uInfo.intFormSeq >= ID_FORMNO_1820 ) {
		//　現在の様式が終端の場合、リターン
		return;
	}

// midori 154237,154238 add -->
	// フォーカスを先頭に戻すことで、先頭項目の表示が一瞬消えるため
	// ウインドウをロックする
	((CMainFrame*)AfxGetMainWnd())->LockWindowUpdate();
// midori 155872 del -->
	//CmnDiagSetFocus(m_pDiag,1,1);
// midori 155872 del <--
// midori 155872 add -->
	if(m_uInfo.intFormSeq == ID_FORMNO_102)	{
		CmnDiagSetFocus(m_pDiag,1,3);
	}
	else	{
		CmnDiagSetFocus(m_pDiag,1,1);
	}
// midori 155872 add <--
// midori 154237,154238 add <--

	// カレント情報などを保存
	CmnUcInfSubSetCurrentInfo();

	// 空行データの削除処理
	if ( CmnTblCheckDeleteData() == TRUE ){
		CmnTblEndDeleteData();
	}

	// 次の様式へ
	if( m_YoushikiSkip == FALSE ){	// 全て空行でも、1つ次の様式へ移動
		// 次の様式のシーケンス番号取得
		nNowFormSeq = CmnGetDestinationYoushiki( FALSE );	// FALSE:次の様式
	}
	else{	// データ行がある様式へ移動
		// 現在の様式より後にデータ入力のある様式が存在する場合
		nNowFormSeq = m_clsFunc.IsExistData(m_pDB, m_uInfo.intFormSeq, FALSE, 1);
// midori 154237,154238 del -->
		//if( nNowFormSeq == -1 )
		//	return;
// midori 154237,154238 del <--
// midori 154237,154238 add -->
		if(nNowFormSeq == -1)	{
			((CMainFrame*)AfxGetMainWnd())->UnlockWindowUpdate();
			return;
		}
// midori 154237,154238 add <--
	}

	//	様式を移動する場合、inf_mainへ書き込み
	if( m_uInfo.intFormSeq != nNowFormSeq ){
		//	初期化成功？
		if ( mfcRec.Init() == DB_ERR_OK ){
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				mfcRec.MoveFirst();		//	レコードを先頭に移動
			}
		}
		mfcRec.Edit();					//	編集開始
		mfcRec.m_FormSeq = nNowFormSeq;	//	様式シーケンス番号変更
		mfcRec.Update();				//	更新
	}
	//	レコード閉じる
	mfcRec.Fin();

	//	レコード開いてる？
	if ( mfcRec.IsOpen() ){
		//	レコード閉じる
		mfcRec.Fin();
	}

	// 表示する様式の切替を行う
// midori 154237,154238 del -->
	//((CUCHIWAKEApp*)AfxGetApp())->ChangeDocument( nNowFormSeq );
// midori 154237,154238 del <--
// midori 154237,154238 add -->
	((CUCHIWAKEApp*)AfxGetApp())->ChangeDocument(nNowFormSeq,1);
	((CMainFrame*)AfxGetMainWnd())->UnlockWindowUpdate();
// midori 154237,154238 add <--

	return;	//	←このリターン重要

	//// 実行前の準備処理
	//CmnEventStartProc( IDC_YOUSHIKINEXT_BUTTON1 );

	//// 頁番号/行番号振り直し
	//CmnTblUpdatePageNumRowNumAdd(m_uInfo.intCurPage, m_uInfo.intCurRow, 1);

	//// 空行挿入
	//CmnTblInsertNullLine(m_uInfo.intCurPage, m_uInfo.intCurRow, 1);

	//// 頁と行の調整処理（挿入後処理）
	//CmnTblDeleteFgFunc(ID_FGFUNC_PAGEKEI);					// 頁計の削除
	//CmnTblDeleteFgFunc(ID_FGFUNC_RUIKEI);					// 累計の削除
	//CmnTblUpdateEndPageSetting();							// 最終頁を調整する
	//CmnTblInsertPageKeiRuiKei();							// 頁計/累計挿入（最終ページの削除or空行追加）
	//
	//// 小計/中計/頁計/累計の再計算
	//CmnTblCalcKei();

	//// １ページ再表示
	//virUpdateControlTblData();

	//// カーソル移動
	//CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
}


//------------------------	印刷処理関連　------------------------

void CfrmUc000Common::OnFilePrintPreview() 
{
	m_prtJob->JobFilePrintPreview(this);
}

BOOL CfrmUc000Common::OnPreparePrinting(CPrintInfo* pInfo) 
{
// midori 190505 del -->
	//return m_prtJob->JobPreparePrinting(pInfo, this);
// midori 190505 del <--
// midori 190505 add -->
	BOOL	brv=FALSE;
	brv = m_prtJob->JobPreparePrinting(pInfo,this);
	if(brv == FALSE)	{
		m_DispFlg = 0;
	}
	return(brv);
// midori 190505 add <--
}

void CfrmUc000Common::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	m_prtJob->JobBeginPrinting(pDC);

	// 2009.10.08 Windows7対応
	// メインフレームクラスを取得します
	CMainFrame*  pFrame  = (CMainFrame*)AfxGetMainWnd();
	// メインフレームに印刷プレビューのビュークラスのポインタを設定します
	pFrame->m_pActivePreview = this;
// midori 152766 add -->
	AfxGetMainWnd()->PostMessage( WM_USER_UPDATE_INDICATE, 0 );
// midori 152766 add <--
}

void CfrmUc000Common::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	ICSFormView::OnPrepareDC(pDC, pInfo);

	m_prtJob->JobPrepareDC(pDC);
}

void CfrmUc000Common::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	m_prtJob->JobEndPrinting(pDC, pInfo);

	// 2009.10.08 Windows7対応
	// メインフレームクラスを取得します
	CMainFrame*  pFrame  = (CMainFrame*)AfxGetMainWnd();
	// メインフレームに印刷プレビューのビュークラスのポインタを初期化します
	pFrame->m_pActivePreview = NULL;

// midori 190505 add -->
	// [F5] → 印刷ダイアログからF4 → [閉じる] で再表示されないように
	m_DispFlg = m_prtJob->m_icsVprn.PreviewOnPrintdlg();
// midori 190505 add <--

	//一括印刷からの戻りでない場合、プリントオブジェクト破棄
	if(m_prtJob->m_bPrtAll == FALSE && m_prtJob->m_isF5FromPrev == FALSE){
		delete m_prtJob;
		m_prtJob = NULL;
// midori 155906 add -->
		ClearTempTbl();
// midori 155906 add <--
	}

// 修正No.158174 add -->
	// 日付にカーソルがある状態でF4:プレビューを実行すると、プレビュー画面内のF7:拡大F8:縮小がキーボードで押せなくなる現象を解消するため、
	// 退避させていたカーソル位置に戻す
	if(g_BatchPrt.IsBatchExec() == FALSE)	{	// 一括印刷でない
		if(m_PrnDlgCol != (-1) && m_PrnDlgRow != (-1))	{
			CmnDiagSetFocus(m_pDiag, m_PrnDlgRow, m_PrnDlgCol);
		}
		m_PrnDlgCol = -1;
		m_PrnDlgRow = -1;
	}
// 修正No.158174 add <--

// midori 152766 add -->
	AfxGetMainWnd()->PostMessage( WM_USER_UPDATE_INDICATE, 1 );
// midori 152766 add <--

	// ※OnEndPriting中はまだ印刷中ダイアログが出ているので、フォーカスセットさせない
	//// F4:プレビュ，F5:印刷の終了処理
	//// カーソル移動
	//CmnDiagSetFocus( m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol );
// midori M-16090701 add -->
	PostMessage(WM_USER_FOCUSSET,0,0);
// midori M-16090701 add <--
}

void CfrmUc000Common::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	m_prtJob->JobPrint(pDC, pInfo, this);
}


//------------------------	共通処理　------------------------


// virtual関数：各帳表で実装する処理
CdbUc000Common* CfrmUc000Common::virGetDataTableObject()
{
	return (CdbUc000Common*)NULL;
}

// virtual関数：各帳表で実装する処理
BOOL CfrmUc000Common::virStartJob()
{
	return FALSE;
}

// virtual関数：各帳表で実装する処理
int	 CfrmUc000Common::virRecBufSaveData(int inPage, int inRow)
{
	return	0;
}

// virtual関数：各帳表で実装する処理
void CfrmUc000Common::virUpdateControlTitle()
{
	return;
}

// virtual関数：各帳表で実装する処理
BOOL CfrmUc000Common::virUpdateControlTblData()
{
	return TRUE;
}

// midori 161108 add -->
// virtual関数：各帳表で実装する処理
BOOL CfrmUc000Common::virUpdateControlTblData2()
{
	return TRUE;
}
// midori 161108 add <--

// virtual関数：各帳表で実装する処理
void CfrmUc000Common::virFncCellCopyProc(CdbUc000Common* rs)
{
	return;
}

// virtual関数：各帳表で実装する処理
void CfrmUc000Common::virSetInitialDiag()
{
	return;
}

// virtual関数：各帳表で実装する処理
void CfrmUc000Common::virSetInitialUchiwakeInfo()
{
	return;
}

// virtual関数：各帳表で実装する処理
void CfrmUc000Common::virSetInitialValue()
{
	return;
}

// virtual関数：各帳表で実装する処理
void CfrmUc000Common::virTblEditIkkatuLine(int nType, CdbUc000Common* rsData, CString strName, CALCKEI_INFO strSum, SORTKAMOKU_INFO uKamoku)
{
	return;
}

// midori 190505 add -->
// virtual関数：各帳表で実装する処理（科目行挿入）
void CfrmUc000Common::virTblEditKamokuLine(CdbUc000Common* rsData, int pPage, int pRow, CString strName)
{		
	return;
}

// virtual関数：科目(見出し)行に挿入する科目名を取得
CString CfrmUc000Common::virTblSortGetKamokuName( CdbUc000Common* rsData )
{
	return(_T(""));
}
// midori 190505 add <--

// 20-0873 add -->
// virtual関数：取引先コードを取得
int CfrmUc000Common::virTblSortGetTorihikisakiCode( CdbUc000Common* rsData )
{
	return(0);
}

// virtual関数：取引先名称を取得
CString CfrmUc000Common::virTblSortGetTorihikisakiName( CdbUc000Common* rsData )
{
	return(0);
}

// virtual関数：科目コードを取得
int CfrmUc000Common::virTblSortGetKamokuCode( CdbUc000Common* rsData )
{
	return(0);
}
// 20-0873 add <--

// virtual関数：各帳表で実装する処理
int CfrmUc000Common::virTblSortSubSortProc(CdlgSort* pCdlgSort, CdbUc000Common* rsData)
{
	return 0;
}

// virtual関数：各帳表で実装する処理
int CfrmUc000Common::virFncReferenceGetType(int intCol, int* intGrSeq, long* lngItemSeq, int* intAdd1, int* intAdd2, int* intAdd1Col, int* intAdd2Col, REFERENCE_INFO* pudReference )
{
	return 0;
}

// virtual関数：各帳表で実装する処理
void CfrmUc000Common::virFncReferenceSetData(CdlgReference* dlgRef, int intRefType, int intCol)
{
	return;
}

// virtual関数：各帳表で実装する処理
void CfrmUc000Common::virFncReferenceSetICSData(CdlgAddressSearch* dlgAdd)
{
	return;
}

// virtual関数：各帳表で実装する処理
void CfrmUc000Common::virFncReferenceSetICSData(CdlgYakuZokuSelect* dlg, int type)
{
	return;
}

// virtual関数：各帳表で実装する処理
void CfrmUc000Common::virFncReferenceUpdateTbl(int intRefType, int nRefMode/*=0*/)
{
	return;
}

// virtual関数：各帳表で実装する処理
void CfrmUc000Common::virRowButtonCopyProc()
{
	return;
}

// virtual関数：各帳表で実装する処理
int CfrmUc000Common::virRecBufSaveDataForPasteInsert(int inPage, int inRow)
{
	return 0;
}

// virtual関数：各帳表で実装する処理
int CfrmUc000Common::virRecBufSaveDataForPaste(int inPage, int inRow)
{
	return 0;
}

// virtual関数：各帳表で実装する処理
//		ソート（一括集計）で使用する集計処理
void CfrmUc000Common::virTblGetIkkatuSumValue(CdbUc000Common* rsData, int nFgShow, int nGroup, CALCKEI_INFO* uCalcKei)
{
	// 通常は"Val"項目のみ集計する。
	// ただし④-2，⑪-1は複数フィールド集計するため、"virtual"にて各画面Viewで集計を行う。
	// なお、一括集計の集計フィールドと小計などの集計フィールドは異なるので注意が必要。
	uCalcKei->intMaxCount = 1;
	uCalcKei->strField[0] = "Val";
	uCalcKei->strKei[0] = "";

	rsData->GetSumValSub(nFgShow, nGroup, uCalcKei->strField[0], &uCalcKei->strKei[0]);		// "0:Val"項目の集計
	return;
}

// virtual関数：各帳表で実装する処理
void CfrmUc000Common::virTblGetKamokuInfo(CdbUc000Common* rsData, SORTKAMOKU_INFO* uKamoku)
{
	uKamoku->intKnSeq = 0;
	uKamoku->intKnOrder = 0;
	uKamoku->strKnName = "";
// midori 152137 add -->
	uKamoku->strKnKana = _T("");
// midori 152137 add <--
	return;
}

// virtual関数：各帳表で実装する処理
void CfrmUc000Common::virGetCurrentInfo(CURRENT_INFO* uCurInfo)
{
	uCurInfo->intFgFunc = 0;
	uCurInfo->intNumGroup = 0;
	uCurInfo->intCopyFlag = 0;
	return;
}

// virtual関数：各帳表で実装する処理
int CfrmUc000Common::virUpdateRelationTblChengeFormType()
{
	return FUNCTION_OK;
}

// midori 152770 add -->
// virtual関数：各帳表で実装する処理
BOOL CfrmUc000Common::virUpdateRelationTblChengeFormTypeSave()
{
	return(TRUE);
}
// midori 152770 add <--

// virtual関数：各帳表で実装する処理
void CfrmUc000Common::virOnUpdateButtonF6(BOOL* bFncEnable)
{
	return;
}

// virtual関数：各帳表で実装する処理
void CfrmUc000Common::virUpdateTitle()
{
	return;
}

// virtual関数：各帳表で実装する処理
void CfrmUc000Common::virInputDecisionEditOFF()
{
	return;
}

// virtual関数：各帳表で実装する処理
void CfrmUc000Common::virInitialControlFgInputCell( int nRow, int nFgFunc, BYTE bFgInput )
{
	return;
}

// virtual関数：各帳表で実装する処理
void CfrmUc000Common::virRendoCreateData(CdbUc000Common* rsData, RENTEMP_INFO* uRenInfo)
{
	return;
}

// virtual関数：各帳表で実装する処理
void CfrmUc000Common::virRendoUpdateData(CdbUc000Common* rsData, RENTEMP_INFO* uRenInfo)
{
	return;
}

// virtual関数：各帳表で実装する処理
void CfrmUc000Common::virRendoSumData(CdbUc000Common* rsData, RENTEMP_INFO* uRenInfo)
{
	return;
}

// 修正No.157633_元号日付 add -->
// virtual関数：各帳表で実装する処理
void CfrmUc000Common::virActivateFrame(UINT nState, CFrameWnd* pDeactivateFrame)
{
	return;
}
// 修正No.157633_元号日付 add <--

// 要通知案件修正 21/07/09 add -->
// virtual関数：各帳表で実装する処理
void CfrmUc000Common::virActivateFrameK(UINT nState, CFrameWnd* pDeactivateFrame)
{
	return;
}

// virtual関数：各帳表で実装する処理
void CfrmUc000Common::virKamokuAddCancel()
{
	return;
}
// 要通知案件修正 21/07/09 add <--

// インボイス登録番号追加対応_23/11/06 add -->
// virtual関数：各帳表で実装する処理
void CfrmUc000Common::virDiagGetHjnData(CDatabase* pDB, HJN_REC_NEW* phjnRec)
{
	return;
}

// virtual関数：各帳表で実装する処理
void CfrmUc000Common::virDiagSetHjnData(int pSw, CString rtInvNo, CString rtName, CString rtAdrs1, CString rtAdrs2, CString rtKana)
{
	return;
}

// virtual関数：各帳表で実装する処理
//int CfrmUc000Common::virCallChkInvoiceNum(int inPage, int inRow)
int CfrmUc000Common::virCallChkInvoiceNum(int pSw, int inPage, int inRow)
{
	return(0);
}
// インボイス登録番号追加対応_23/11/06 add <--

//********************************************************************************
//	Function/Menu/Buttonなどの開始前の準備処理
//		IN		int	intEventNo	イベントサイン（ID_BUTTON_F4等）
//				int	intForkSel	ICS_MAINFRAME_PARAM_FORKCHANGINGで来た時に入るサブサイン
//								業務切替時に来る
//		RET		なし
//********************************************************************************
//void CfrmUc000Common::CmnEventStartProc(int intEventNo)							// 修正No.168443 del
void CfrmUc000Common::CmnEventStartProc(int intEventNo, int intForkSel/*=-1*/)	// 修正No.168443 add
{
// 必要ない！！
//	// 入力ボックスを消去する（消去した際にEditOFFが発生）
//	m_pDiag->DeleteInput();

	int		sign;		// 修正No.168443 add

// 要通知案件修正 21/07/09 add -->
	switch(intEventNo)	{
	case	ID_BUTTON_F4:						// F4:プレビュー
	case	ID_BUTTON_F5:						// F5:印刷
	case	ID_BUTTON_F7:						// F7:行削除
	case	ID_BUTTON_F8:						// F8:行挿入
	case	ID_BUTTON_F9:						// F9:参照
	case	ID_BUTTON_F11:						// F11:1行上へ
	case	ID_BUTTON_F12:						// F12:1行下へ
	case	ID_BUTTON_INSERT:					// Ins:様式一覧

	case	ID_DATAIMPORT_MENU:					// ﾒﾆｭｰ[ﾌｧｲﾙ]ﾃﾞｰﾀｲﾝﾎﾟｰﾄ
	case	ID_DATAEXPORT_MENU:					// ﾒﾆｭｰ[ﾌｧｲﾙ]ﾃﾞｰﾀｴｸｽﾎﾟｰﾄ
	case	ID_PRINTALL_MENU:					// ﾒﾆｭｰ[印刷設定]一括印刷
	case	ID_PRINTSETUP_MENU:					// ﾒﾆｭｰ[印刷設定]印刷設定
	case	ID_REFERENCE2_MENU:					// ﾒﾆｭｰ[印刷設定]科目指定
	case	ID_OUTPUTFORM_MENU:					// ﾒﾆｭｰ[印刷設定]出力形式
	case	ID_PRINTHYOSHI_MENU:				// ﾒﾆｭｰ[印刷設定]表紙設定
	case	ID_SORT_MENU:						// ﾒﾆｭｰ[編集]ソート
	case	ID_SPECIALROW_MENU:					// ﾒﾆｭｰ[編集]特殊行挿入
	case	ID_AMOUTDIALOG_MENU:				// ﾒﾆｭｰ[編集]合計設定
	case	ID_CLEAR_MENU:						// ﾒﾆｭｰ[編集]金額等数値0クリア
	case	ID_IKKATUADD_MENU:					// ﾒﾆｭｰ[一括金額]一括金額追加
	case	ID_DISPDIALOG_MENU:					// ﾒﾆｭｰ[一括金額]一括金額参照
	case	ID_ZEROMONEY_MENU:					// ﾒﾆｭｰ[保管]保管参照
	case	ID_ZEROMONEYMOVE_MENU:				// ﾒﾆｭｰ[保管]一括で一時保管へ移動
	case	ID_RENDO_MENU:						// ﾒﾆｭｰ[財務連動]財務連動実行
	case	ID_USER_SETTING_MENU:				// ﾒﾆｭｰ[ｵﾌﾟｼｮﾝ]オプション設定（※確定/確定解除も兼ねてる）
	case	ID_TITLEMENU:						// ﾒﾆｭｰ[ｵﾌﾟｼｮﾝ]タイトル初期化
	case	ID_COPY_SETTING_INFO_MENU:			// ﾒﾆｭｰ[ｵﾌﾟｼｮﾝ]他マスターから設定情報のコピー
	case	ID_DELETE_DATA_MENU:				// ﾒﾆｭｰ[ｵﾌﾟｼｮﾝ]様式別全データ削除
	case	ID_FROMKOMON_MENU:					// ﾒﾆｭｰ[取込処理]顧問先情報の取込

	case	ID_BUTTON_HOME:						// ヘルプ
	case	ID_BUTTON_END:						// 業務終了
	case	ICS_MAINFRAME_MESSAGE_COSEL:		// 会社切替
	case	ICS_MAINFRAME_PARAM_FORKCHANGING:	// 業務ツールでの切り替え

	case	IDC_PAGEBACK_BUTTON1:				// 前頁ボタン
	case	IDC_PAGENEXT_BUTTON1:				// 次頁ボタン
	case	IDC_PAGENEW_BUTTON1:				// 改頁ボタン
	case	IDC_PAGEINSERT_BUTTON1:				// 頁挿入ボタン
	case	IDC_ROWCOPY_BUTTON1:				// 行コピーボタン
	case	IDC_ROWPASTE_BUTTON1:				// 行貼付ボタン
	case	IDC_ROWINSERT_BUTTON1:				// 行挿入貼付
	case	IDC_MARGINAL_BUTTON1:				// ⑤棚卸方法登録/⑨欄外登録
	case	IDC_YOUSHIKIBACK_BUTTON1:			// 前様式ボタン
	case	IDC_YOUSHIKINEXT_BUTTON1:			// 次様式ボタン
		virKamokuAddCancel();
		break;
	default:
		break;
	}
// 要通知案件修正 21/07/09 add <--

// インボイス登録番号追加対応_23/11/21 add -->
	if(bG_InvNo == TRUE) {
		// ---- 登録番号一覧関連 ----->
		// 登録番号一覧が開いていたら閉じる
		if(pZmSel && pZmSel->IsDisplay() != FALSE) {
			pZmSel->DialogOFF();
		}
		// ---- 登録番号一覧関連 <-----
	}
// インボイス登録番号追加対応_23/11/21 add <--

	// カレント行の登録
	switch (intEventNo) {
	case ID_BUTTON_F6:				// 項目複写
//	case ID_BUTTON_F9:				// 参照（依頼13-1449のため、参照ダイアログ呼び出しでもカレント行を登録する）
	case ID_BUTTON_HOME:			// 住所検索 (2012/07/20)
		break;
	default:
		virInputDecisionEditOFF();									//	入力確定（2006.04.28）

// 修正No.168422,168443 add -->
		// 今カーソルがある場所が登録番号の場合、入力された登録番号をチェック
		if(InvNoClmChk() == TRUE) {
// 修正No.168443 del -->
			//virCallChkInvoiceNum(m_uInfo.intCurPage, m_uInfo.intCurRow);
// 修正No.168443 del <--
// 修正No.168443 add -->
			if(intEventNo != ID_BUTTON_END) {	// OnMessageCloseでチェックしているのでここではしない	// 修正No.168537 add
				sign = 3;
				//if(intEventNo == ICS_MAINFRAME_PARAM_FORKCHANGING && intForkSel == -1)	sign = 4;
				virCallChkInvoiceNum(sign, m_uInfo.intCurPage, m_uInfo.intCurRow);
			}
// 修正No.168443 add <--
		}
// 修正No.168422,168443 add <--

		// 改良依頼No.22-0376 の対応
		// 業務ツールでの切り替え時はデータの登録を行わない
		// (ここで行わなくても、切り替え先の業務から戻ってきたときまたは切り替え先の業務で
		// 一括終了したときに EditONYoushikidiag1 が発行され、そこで virRecBufSaveData 関数が呼ばれる)
		// イベントログをみると、IMEのモジュールでエラーが発生しており
		// データ登録時のカナ取得関数(GetFurigana)内でIMEの関数を使用しているので
		// 勘定科目内訳書→他業務に切り替えたときにIMEの設定が変わるためエラーが発生していると考えられる
		if(intEventNo != ICS_MAINFRAME_PARAM_FORKCHANGING)	{
// 修正No.168422,168443 del -->
//// インボイス登録番号追加対応_23/11/21 add -->
//			if(InvNoClmChk() == TRUE) {
//				// 入力された登録番号をチェック
//				virCallChkInvoiceNum(m_uInfo.intCurPage, m_uInfo.intCurRow);
//			}
//// インボイス登録番号追加対応_23/11/21 add <--
// 修正No.168422,168443 del <--
			virRecBufSaveData(m_uInfo.intCurPage, m_uInfo.intCurRow);	//	１行登録
		}
		virUpdateTitle();											//	タイトルの更新（2006.4.22 add）
		break;
	}

// インボイス登録番号追加対応_23/11/21 add -->
	// ---- 登録番号一覧関連 ----->
	// virCallChkInvoiceNum()後 カーソルを再セットされるため 登録番号一覧が開いていたら閉じる
	if(pZmSel && pZmSel->IsDisplay() != FALSE)	{
		pZmSel->DialogOFF();
	}
	// ---- 登録番号一覧関連 <-----

// 修正No.168422 del -->
//	// 登録番号の位置にカーソルがある状態で印刷、様式切替等を行うと、登録一覧が再表示されてしまうのため、
//	// フォーカス位置をコンボに移動させる。
//	if(InvNoClmChk() == TRUE) {
//		if(intEventNo != ID_BUTTON_F6) {
//			m_uInfo.intCurCol = m_uInfo.intCurCol - 1;
//			CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
//		}
//	}
// 修正No.168422 del <--
// 修正No.168422 add -->
	switch(intEventNo)	{
		case	ID_BUTTON_F4:						// F4:プレビュー
		case	ID_BUTTON_F5:						// F5:印刷
		//case	ID_BUTTON_F7:						// F7:行削除
		//case	ID_BUTTON_F8:						// F8:行挿入
		case	ID_BUTTON_F9:						// F9:参照			// 修正No.168536 add
		//case	ID_BUTTON_F11:						// F11:1行上へ
		//case	ID_BUTTON_F12:						// F12:1行下へ
		case	ID_BUTTON_INSERT:					// Ins:様式一覧

		case	ID_DATAIMPORT_MENU:					// ﾒﾆｭｰ[ﾌｧｲﾙ]ﾃﾞｰﾀｲﾝﾎﾟｰﾄ
		case	ID_DATAEXPORT_MENU:					// ﾒﾆｭｰ[ﾌｧｲﾙ]ﾃﾞｰﾀｴｸｽﾎﾟｰﾄ
		case	ID_PRINTALL_MENU:					// ﾒﾆｭｰ[印刷設定]一括印刷
		case	ID_PRINTSETUP_MENU:					// ﾒﾆｭｰ[印刷設定]印刷設定
		case	ID_REFERENCE2_MENU:					// ﾒﾆｭｰ[印刷設定]科目指定
		case	ID_OUTPUTFORM_MENU:					// ﾒﾆｭｰ[印刷設定]出力形式
		case	ID_PRINTHYOSHI_MENU:				// ﾒﾆｭｰ[印刷設定]表紙設定
		case	ID_SORT_MENU:						// ﾒﾆｭｰ[編集]ソート
		case	ID_SPECIALROW_MENU:					// ﾒﾆｭｰ[編集]特殊行挿入
		case	ID_AMOUTDIALOG_MENU:				// ﾒﾆｭｰ[編集]合計設定
		case	ID_CLEAR_MENU:						// ﾒﾆｭｰ[編集]金額等数値0クリア
		case	ID_IKKATUADD_MENU:					// ﾒﾆｭｰ[一括金額]一括金額追加
		case	ID_DISPDIALOG_MENU:					// ﾒﾆｭｰ[一括金額]一括金額参照
		case	ID_ZEROMONEY_MENU:					// ﾒﾆｭｰ[保管]保管参照
		case	ID_ZEROMONEYMOVE_MENU:				// ﾒﾆｭｰ[保管]一括で一時保管へ移動
		case	ID_RENDO_MENU:						// ﾒﾆｭｰ[財務連動]財務連動実行
		case	ID_USER_SETTING_MENU:				// ﾒﾆｭｰ[ｵﾌﾟｼｮﾝ]オプション設定（※確定/確定解除も兼ねてる）
		case	ID_TITLEMENU:						// ﾒﾆｭｰ[ｵﾌﾟｼｮﾝ]タイトル初期化
		case	ID_COPY_SETTING_INFO_MENU:			// ﾒﾆｭｰ[ｵﾌﾟｼｮﾝ]他マスターから設定情報のコピー
		case	ID_DELETE_DATA_MENU:				// ﾒﾆｭｰ[ｵﾌﾟｼｮﾝ]様式別全データ削除
		case	ID_FROMKOMON_MENU:					// ﾒﾆｭｰ[取込処理]顧問先情報の取込

		//case	ID_BUTTON_HOME:						// ヘルプ
		//case	ID_BUTTON_END:						// 業務終了
		//case	ICS_MAINFRAME_MESSAGE_COSEL:		// 会社切替
		//case	ICS_MAINFRAME_PARAM_FORKCHANGING:	// 業務ツールでの切り替え

		case	IDC_PAGEBACK_BUTTON1:				// 前頁ボタン
		case	IDC_PAGENEXT_BUTTON1:				// 次頁ボタン
		case	IDC_PAGENEW_BUTTON1:				// 改頁ボタン
		case	IDC_PAGEINSERT_BUTTON1:				// 頁挿入ボタン
		case	IDC_ROWCOPY_BUTTON1:				// 行コピーボタン
		case	IDC_ROWPASTE_BUTTON1:				// 行貼付ボタン
		case	IDC_ROWINSERT_BUTTON1:				// 行挿入貼付
		case	IDC_MARGINAL_BUTTON1:				// ⑤棚卸方法登録/⑨欄外登録
		case	IDC_YOUSHIKIBACK_BUTTON1:			// 前様式ボタン
		case	IDC_YOUSHIKINEXT_BUTTON1:			// 次様式ボタン
			// 登録番号の位置にカーソルがある状態で印刷や様式切替等を行うと、登録番号一覧が再表示されてしまうため、
			// フォーカス位置をコンボに移動させる。
			//if(m_DataKakutei == FALSE)	{		// 修正No.168544 del
				if(InvNoClmChk() == TRUE)	{
					m_uInfo.intCurCol = m_uInfo.intCurCol - 1;
					CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
				}
			//}										// 修正No.168544 del
			break;
		default:
			break;
	}
// 修正No.168422 add <--
// インボイス登録番号追加対応_23/11/21 add <--

	m_uInfo.intCurFocus = 0;
}

//********************************************************************************
//	共通データの初期設定
//		IN		int				FormSeq番号（0:uc_inf_mainから取得）
//		RET		なし
//********************************************************************************
BOOL CfrmUc000Common::CmnSetInitialUchiwakeInfo(int intFormSeq)
{
	if (intFormSeq == 0) {
// 改良No.21-0086,21-0529 add -->
		// ※CfrmUc000Common::OnInitialUpdate()関数でセットしていたが、会社切替で同じ様式を開けた時に入らないので、
		// m_RenZvol の中身が更新されていなかった
		// 会社情報取得
		if( ((CUCHIWAKEApp*)AfxGetApp())->m_RenZvol.CoType != m_RenZvol.CoType ||
			((CUCHIWAKEApp*)AfxGetApp())->m_RenZvol.EzFlag != m_RenZvol.EzFlag )	{
			m_RenZvol = ((CUCHIWAKEApp*)AfxGetApp())->m_RenZvol;
		}
// 改良No.21-0086,21-0529 add <--

		// uc_inf_mainから初期情報を取得
		CmnUcInfMainGetInitInfo();
	}
	else {
		// Cfrmxxxxxクラスでテーブル更新のみ行う場合、FormSeq番号を指定する
		m_uInfo.intFormSeq = intFormSeq;
	}

	// デフォルト値を含め全てをセットする
	m_uInfo.intMaxPage = CmnTblGetPageMax();		// 最大頁数
	m_uInfo.intCurPage = 1;							// カレント頁番号（uc_inf_sub）
	m_uInfo.intCurRow = 1;							// カレント行番号
	m_uInfo.intCurCol = 1;							// カレント列番号
	m_uInfo.intCurCtlIndex = 0;						// カレントコントロールのインデックス番号
	m_uInfo.intCurFocus = 0;						// カレントフォーカス情報

	m_uInfo.intOutKei = ID_OUTKEI_RUIKEI;			// 計設定内容（uc_inf_sub）
	m_uInfo.intFormType = ID_FORMTYPE_KOBETU;		// 出力形式設定内容（uc_inf_form_type）
	m_uInfo.intOutKeiDialog = AM_TYPE_ALL;			// 計設定ダイアログのボタン指定				･･･ 帳表固有
	m_uInfo.intRowMax = 0;							// １頁内の合計行を含む全体の行数			･･･ 帳表固有
	m_uInfo.intRowMaxData = 0;						// １頁内のデータ行数（合計行は含まず）		･･･ 帳表固有
	m_uInfo.intColMax = 0;							// １行内のコントロール個数					･･･ 帳表固有
	m_uInfo.intColMaxData = 0;						// １行内の列数								･･･ 帳表固有
	m_uInfo.intCtlStartIndex = 0;					// １行目の入力項目の先頭インデックス番号	･･･ 帳表固有
	m_uInfo.intT1StartIndex = 0;					// 合計行①の先頭インデックス番号			･･･ 帳表固有
	m_uInfo.intT2StartIndex = 0;					// 合計行②の入力項目の先頭インデックス番号	･･･ 帳表固有
	m_uInfo.intCursolDefPos = 1;					// カーソル移動時の初期位置					･･･ 帳表固有
	m_uInfo.intCursolKeiPos = 1;					// 合計行のカーソル移動位置					･･･ 帳表固有

	m_uInfo.OldPage = 0;							// 行移動確認の頁番号
	m_uInfo.OldRow = 0;								// 行移動確認の行番号
	m_uInfo.OldstrDiag = _T("");					// データ入力の更新チェック用文字列
	m_uInfo.OldCombo = 0;							// データ入力の更新チェック用ComboBox値
	m_uInfo.OldCol = 0;								// 列移動確認の列番号
// 修正No.157633_元号日付 add -->
	for(int ii = 0; ii < 5; ii++) {
		m_uInfo.OldDate[ii] = 0;					// 日付データ入力更新チェック用の値
		m_uInfo.OldGengo[ii] = 0;					// 日付(元号番号)データ入力更新チェック用の値
	}
	m_uInfo.GenListSw = 0;							// 
	m_uInfo.EditSign = -1;							// EditON⇔EditOFFがペアで通ってるか確認するためのサイン
// 修正No.157633_元号日付 add <--

//	m_uInfo.intFormSeq = ID_FORMNO_011;				// 様式シーケンス番号（uc_inf_main）		･･･ 帳表固有
	m_uInfo.intFormNo = 1;							// 様式番号　　（uc_inf_sub）
	m_uInfo.intFormNo2 = 1;							// 様式番号枝番（uc_inf_sub）
	m_uInfo.bRenFlg = FALSE;						// 財務連動実行可/不可フラグ

	m_uInfo.strTitleName = "";						// 帳表タイトル名称（uc_inf_sub）
	m_uInfo.strTitleNumber = "";					// 帳表タイトル番号（uc_inf_sub）

	m_uInfo.intRowNormal = 0;						// 通常時：１頁内の合計行を含む全体の行数	･･･ 帳表固有
	m_uInfo.intRowKobetu = 0;						// 個別時：１頁内の合計行を含む全体の行数	･･･ 帳表固有
	m_uInfo.intT1StartNormal = 0;					// 通常時：合計行①の先頭インデックス		･･･ 帳表固有
	m_uInfo.intT1StartKobetu = 0;					// 個別時：合計行①の先頭インデックス		･･･ 帳表固有
	m_uInfo.intT2StartNormal = 0;					// 通常時：合計行②の先頭インデックス		･･･ 帳表固有
	m_uInfo.intT2StartKobetu = 0;					// 個別時：合計行②の先頭インデックス		･･･ 帳表固有
	m_uInfo.intRowKei = 1;							// 帳表固定の合計行数（1:合計あり，0:なし）	･･･ 帳表固有

	for (int i = 0; i < CALCKEI_MAXCOUNT; i++) {
		m_uInfo.uCalcInfo.strField[i] = "";			// 集計処理：集計フィールド名称クリア		･･･ 帳表固有
		m_uInfo.uCalcInfo.strKei[i] = "";			// 集計処理：集計値クリア
		m_uInfo.uCalcInfo.bExistNum[i] = FALSE;		// 集計処理：表示フラグクリア
	}

	// 帳表固有データの設定
	virSetInitialUchiwakeInfo();

	// uc_inf_subから初期情報を取得
	if ( CmnUcInfSubGetInitInfo() != TRUE ){
		return FALSE;
	}

	// uc_inf_form_typeから初期情報を取得
	if ( CmnUcInfFormTypeGetFormType() != TRUE ){
		return FALSE;
	}

	// uc_ren_mainから財務連動実行可/不可情報を取得
	if ( CmnUcRenMainGetRendoInfo() != TRUE ){
		return FALSE;
	}

	// 出力形式から１頁の最大行数/データ行数と合計行の先頭インデックスを取得
	CmnSetInitialRowMax();

	return TRUE;
}

//********************************************************************************
//	出力形式から１頁の最大行数/データ行数と合計行の先頭インデックスを取得
//		IN		なし
//		RET		なし
//********************************************************************************
void CfrmUc000Common::CmnSetInitialRowMax()
{
	// 注意：当処理では通常頁のデータ行数を"intRowMaxData"に設定する
	//		 "累計"や"頁計/累計"時の最終頁はここでは考慮せず、例外処理として使用箇所で対応を行う
	if (m_uInfo.intFormType == ID_FORMTYPE_KOBETU) {
		// 個別
		if (m_uInfo.intRowKei == 1) {
			// 計あり帳表
			m_uInfo.intRowMaxData = m_uInfo.intRowKobetu - 1;
		}
		else if ((m_uInfo.intOutKei == ID_OUTKEI_PAGEKEI) || (m_uInfo.intOutKei == ID_OUTKEI_BOTH)) {
			// 頁計 or 頁計/累計
			m_uInfo.intRowMaxData = m_uInfo.intRowKobetu - 1;
		}
		else {
			// 最下段もデータとして使用する
			m_uInfo.intRowMaxData = m_uInfo.intRowKobetu;
		}
		m_uInfo.intRowMax = m_uInfo.intRowKobetu;				// １頁内の合計行を含む全体の行数
		m_uInfo.intT1StartIndex = m_uInfo.intT1StartKobetu;		// 合計行①の先頭インデックス
		m_uInfo.intT2StartIndex = m_uInfo.intT2StartKobetu;		// 合計行②の先頭インデックス
	}
	else {
		// 通常
		if (m_uInfo.intRowKei == 1) {
			// 計あり帳表
			m_uInfo.intRowMaxData = m_uInfo.intRowNormal - 1;
		}
		else if ((m_uInfo.intOutKei == ID_OUTKEI_PAGEKEI) || (m_uInfo.intOutKei == ID_OUTKEI_BOTH)) {
			// 頁計 or 頁計/累計
			m_uInfo.intRowMaxData = m_uInfo.intRowNormal - 1;
		}
		else {
			// 最下段もデータとして使用する
			m_uInfo.intRowMaxData = m_uInfo.intRowNormal;
		}
		m_uInfo.intRowMax = m_uInfo.intRowNormal;				// １頁内の合計行を含む全体の行数
		m_uInfo.intT1StartIndex = m_uInfo.intT1StartNormal;		// 合計行①の先頭インデックス
		m_uInfo.intT2StartIndex = m_uInfo.intT2StartNormal;		// 合計行②の先頭インデックス
	}

	// m_pDiagの切り替え：通常/個別の切り替えがある場合に、各帳表クラスで実行して下さい
	virSetInitialDiag();
}

//********************************************************************************
//	自動一括集計金額行の追加/更新処理
//		IN		int				追加/更新区分（0:追加，1:更新，2:指定頁/行に追加）
//				int				追加対象のグループ番号
//				int*			頁番号				（追加時のみ使用）
//				int*			行番号				（追加時のみ使用）
//				SORTKAMOKU_INFO	ソート時の科目情報	（追加時のみ使用）
//		RET		int				実行結果（FUNCTION_OK, FUNCTION_NG）
//********************************************************************************
int CfrmUc000Common::CmnTblIkkatuAutoUpdateProc(int nType, int nGroup, int* nPage, int* nRow, SORTKAMOKU_INFO uKamoku)
{
	CString			strName;			// 名称
//	CString			strSum;				// 集計金額
	CALCKEI_INFO	uCalcKei;			// 集計金額
	int				nCount;				// 集計件数
	int				nTempPage = 0;
	int				nTempRow = 0;

	// 帳表毎のテーブルクラスを"New"する
	CdbUc000Common*		rsData;
	rsData = virGetDataTableObject();

	// 金額と件数を集計
	virTblGetIkkatuSumValue(rsData, ID_FGSHOW_IKKATU, nGroup, &uCalcKei);
	nCount = rsData->GetCountVal(ID_FGSHOW_IKKATU, nGroup);

	// 一括金額レコードの名称
	strName.Format("%s %d%s", NAME_IKKATU_1, nCount, NAME_IKKATU_2);

	if ((nType == 0) || (nType == 2)) {
		// 自動一括集計金額行の追加：１件以上あれば追加
		if (nCount > 0) {
			// 追加行の頁と行を取得
			if (nType == 0) {
				CmnGetNextPageRow(nPage, nRow);
			}

			// 新規レコード追加
			rsData->CreateNewRecord();					// レコード初期化済み

			// 特殊行データへ更新
			rsData->Edit();
			rsData->m_NumPage = *nPage;					// 頁番号
			rsData->m_NumRow = *nRow;					// 行番号
			rsData->m_FgFunc = ID_FGFUNC_IKKATUAUTO;	// 特殊行フラグ
			rsData->m_FgShow = ID_FGSHOW_OFF;			// 表示フラグ
			rsData->m_NumGroup = nGroup;				// グループ番号

			virTblEditIkkatuLine(0, rsData, strName, uCalcKei, uKamoku);		// 帳表個別データ設定
			rsData->Update();
		}
	}
	else {
		// 自動一括集計金額行の更新：レコードがあれば更新（対象０件なら削除が必要）
		rsData->RequeryFgFuncGroup(ID_FGFUNC_IKKATUAUTO, nGroup);
		if (rsData->IsEOF() == 0) {
			if (nCount <= 0) {
				// 削除前に削除レコードの頁と行をバックアップ
				nTempPage = rsData->m_NumPage;
				nTempRow = rsData->m_NumRow;

				// 対象データが０件なので自動一括集計金額データの削除
				rsData->Delete();

				// データ削除した場合、頁番号/行番号振り直しが必要
				CmnTblUpdatePageNumRowNumAdd(nTempPage, nTempRow, -1);
			}
			else {
				// 自動一括集計金額データの更新
				rsData->Edit();
				virTblEditIkkatuLine(1, rsData, strName, uCalcKei, uKamoku);	// 帳表個別データ設定
				rsData->Update();
			}
		}
	}
	rsData->Fin();
	delete rsData;

	return	FUNCTION_OK;
}

//********************************************************************************
//	一括金額参照/一時保管参照の処理
//		IN		EnumIdFgShow		一括金額 or 一時保管
//		RET		なし
//********************************************************************************
int CfrmUc000Common::CmnTblIkkatuHokanProc(EnumIdFgShow nFgShow)
{
	int					nGroup = 0;			// グループ番号
// 修正No.157633_元号日付 add -->
	int					colmsv = -1, colmset = -1;
	int					rowsv = -1, rowset = -1;
	int					sign = 0;
// 修正No.157633_元号日付 add <--
	SORTKAMOKU_INFO		uKamoku;			// 実は必要なし！
	CURRENT_INFO		uCurInfo;			// 各帳表のカレント情報


// 修正No.157633_元号日付 add -->
	colmsv = rowsv = -1;
	colmset = rowset = -1;
	// カーソルの位置を保存、場合によっては一時退避
// 修正No.158301 del -->
//	if(m_uInfo.intFormSeq == ID_FORMNO_021)	{
//		if(m_uInfo.intCurCol == ID_COL_021_SPDATE || m_uInfo.intCurCol == ID_COL_021_PMDATE)	{
//			colmset = ID_COL_021_KAMOKU;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_061)	{
//		if(bG_Kanso == TRUE && m_uInfo.intCurCol == ID_COL_061K_SPDATE)	{
//			colmset = ID_COL_061K_KUBUN;
//			sign = 1;
//		}
//		else if(bG_Kanso == FALSE && m_uInfo.intCurCol == ID_COL_061_SPDATE)	{
//			colmset = ID_COL_061_KUBUN;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_071)	{
//		if(m_uInfo.intCurCol == ID_COL_071_SPDATE || m_uInfo.intCurCol == ID_COL_071_SPDATE2)	{
//			colmset = ID_COL_071_SYURUI;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_081)	{
//		if(m_uInfo.intCurCol == ID_COL_081_SPDATE || m_uInfo.intCurCol == ID_COL_081_PMDATE)	{
//			colmset = ID_COL_081_ADNAME1;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_102)	{
//// 修正No.157883,157884 del -->
//		//if(m_uInfo.intCurCol == ID_COL_102_SPDATE)	{
//// 修正No.157883,157884 del <--
//// 修正No.157883,157884 add -->
//		if(m_uInfo.intCurCol == ID_COL_102_SPDATE || m_uInfo.intCurCol == ID_COL_102_VAL)	{
//// 修正No.157883,157884 add <--
//			colmset = ID_COL_102_KNORDER;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_121)	{
//		if(m_uInfo.intCurCol == ID_COL_121_SPDATE)	{
//			colmset = ID_COL_121_KUBUN;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_151)	{
//		if(m_uInfo.intCurCol == ID_COL_151_SPDATE || m_uInfo.intCurCol == ID_COL_151_SPDATE2)	{
//			colmset = ID_COL_151_KUBUN;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_152)	{
//		if(m_uInfo.intCurCol == ID_COL_152_SPDATE)	{
//			colmset = ID_COL_152_ADNAME1;
//			sign = 1;
//		}
//	}
//	else if(m_uInfo.intFormSeq == ID_FORMNO_153)	{
//		if( m_uInfo.intCurCol == ID_COL_153_KYDATE || m_uInfo.intCurCol == ID_COL_153_KYDATE2 ||
//			m_uInfo.intCurCol == ID_COL_153_SPDATE || m_uInfo.intCurCol == ID_COL_153_SPDATE2 )	{
//			colmset = ID_COL_153_HDNAME;
//			sign = 1;
//		}
//	}
// 修正No.158301 del <--
// 修正No.158301 add -->
	sign = CmnTblDataToFirstCol(&colmset);
// 修正No.158301 add <--
	if(sign == 1)	{
		colmsv = m_uInfo.intCurCol;
		rowsv = m_uInfo.intCurRow;
		//CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, colmset);
		m_pDiag->DeleteInput();
	}
// 修正No.157633_元号日付 add <--

	// 帳表のカレント情報を取得
	virGetCurrentInfo(&uCurInfo);

	// チェック処理：累計or頁計行は実行不可
	switch (uCurInfo.intFgFunc) {
	case ID_FGFUNC_RUIKEI:
	case ID_FGFUNC_PAGEKEI:
		ICSMessageBox( _T("挿入できません。"), MB_OK, 0, 0, this );
		return FUNCTION_NG;
	}

	// 一括金額はグループ情報が必要（一時保管は"0"固定）
	if (nFgShow == ID_FGSHOW_IKKATU) {
		nGroup = uCurInfo.intNumGroup;
	}

	// ダイアログを表示
// midori 152133 del -->
//	CdlgDisplay dlgDisp( this );
// midori 152133 del <--
// midori 152133 add -->
	CdlgDisplay2 dlgDisp( this );
// midori 152133 add <--
// midori 160612 cor -->
//	// カレント行にインバースをかける
//	CmnRowInverse( TRUE , &uCurInfo );
// ---------------------
	// 入力データを確定するにチェック無し
	if(m_DataKakutei == FALSE)	{
		// カレント行にインバースをかける
// 修正No.157627 add -->
		CmnRowBeforeInverse(&uCurInfo);
// 修正No.157627 add <--
		CmnRowInverse( TRUE , &uCurInfo );
	}
// midori 160612 cor <--

// midori 160612 cor -->
//	int nRet = dlgDisp.ShowDialog( m_pDB, m_uInfo.intFormSeq, nFgShow, nGroup );
// -----------------------
	int nRet = dlgDisp.ShowDialog( ((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub, m_uInfo.intFormSeq, nFgShow, nGroup, m_DataKakutei );
// midori 160612 cor <--

// midori 160612 cor -->
//	// カレント行のインバースを解除
//	CmnRowInverse( FALSE , &uCurInfo );
// ---------------------
	// 入力データを確定するにチェック無し
	if(m_DataKakutei == FALSE)	{
		// カレント行のインバースを解除
		CmnRowInverse( FALSE , &uCurInfo );
// 修正No.157627 add -->
		CmnRowAfterInverse(&uCurInfo);
// 修正No.157627 add <--
	}
// midori 160612 cor <--

	switch( nRet ){
	case ID_DLG_BACK:		// 戻す
		break;
	default:
// midori 161108 add -->
		// 保管件数のみ再表示
		virUpdateControlTblData2();
// midori 161108 add <--
// 修正No.157633_元号日付 add -->
		if(colmsv != -1 || rowsv != -1)	{
			CmnDiagSetFocus(m_pDiag, rowsv, colmsv);
		}
// 修正No.157633_元号日付 add <--
		return FUNCTION_NG;
		break;
	}
	//--------------------------------------------------------------------------------
	//	一括金額/一時保管ダイアログでは、複数レコードの戻すが選択可能。
	//	戻す対象のレコードは、各帳表のレコードにフラグが立っている。
	//		"FgShow" = "ID_FGSHOW_RET_HOKAN" or "ID_FGSHOW_RET_IKKATU"
	//
	//	なお、一時保管参照で削除を行った場合、ダイアログ側でレコードを削除。
	//--------------------------------------------------------------------------------

	// データとして戻す（頁番号、行番号、フラグを再設定）
	switch( nRet ){
	case ID_DLG_BACK:		// 戻す
		CmnTblUpdateFgShowDataReturn(nFgShow, m_uInfo.intCurPage, m_uInfo.intCurRow, nGroup);
		break;
	}

	if( nFgShow == ID_FGSHOW_IKKATU ){
		// 一括金額行の更新
		CmnTblIkkatuAutoUpdateProc(1, nGroup, 0, 0, uKamoku);
// 157153 del -->
//// midori 156968 add -->
//		// 先頭行が空行の場合削除する
//		CmnTblUpdatePageRowAllResetMain(6);
//// midori 156968 add <--
// 157153 del <--
	}

	// 頁と行の再設定
	CmnTblUpdatePageRowAllResetMain();

// 157153 add -->
	// 先頭行が空行の場合削除する
	CmnTblUpdatePageRowAllResetMain(6);
// 157153 add <--

	// 1頁再表示
	virUpdateControlTblData();

	// フォーカスセット
	//	Activeイベントと２回発生することになるが、ここでセットしないと、
	//	カーソル位置がおかしくなってしまうため
// 修正No.157633_元号日付 del -->
	//CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
// 修正No.157633_元号日付 del <--
// 修正No.157633_元号日付 add -->
	if(colmsv != -1 || rowsv != -1)	{
		CmnDiagSetFocus(m_pDiag, rowsv, colmsv);
	}
	else	{
		CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
	}
// 修正No.157633_元号日付 add <--

	return FUNCTION_OK;
}


//********************************************************************************
//	現在のコントロール番号から行番号を取得
//		IN		なし or 行番号
//		RET		行番号
//********************************************************************************
int CfrmUc000Common::CmnGetControlRow()
{
	return CmnGetControlRowSub(m_uInfo.intCurCtlIndex);
}

int CfrmUc000Common::CmnGetControlRow(int intIndex)
{
	return CmnGetControlRowSub(intIndex);
}

int CfrmUc000Common::CmnGetControlRowSub(int intIndex)
{
	int		intLine;		// 戻り値用行番号

	// 現在の行を取得
	if ((intIndex >= m_uInfo.intT1StartIndex) && (intIndex < m_uInfo.intT2StartIndex)) {
		// 合計行①
		intLine = m_uInfo.intRowMax - 1;
	}
	else if (intIndex >= m_uInfo.intT2StartIndex) {
		// 合計行②
		intLine = m_uInfo.intRowMax;
	}
	else {
		// データ行
		intLine = (intIndex / m_uInfo.intColMax) + 1;
	}

	return ( intLine );
}

//********************************************************************************
//	現在のコントロール番号から列番号を取得
//		IN		なし
//		RET		列番号
//********************************************************************************
int CfrmUc000Common::CmnGetControlCol()
{
	int			intRet;		// 取得列格納
	int			intPer;		// 余りを格納

	// 余りを格納
	intPer = (m_uInfo.intCurCtlIndex % m_uInfo.intColMax);

	// 列取得
	intRet = intPer - m_uInfo.intCtlStartIndex + 1;

	return ( intRet );
}

//********************************************************************************
//	現在のコントロール番号から行番号/列番号を取得
//		IN		int*			行番号
//				int*			列番号(移動先)
//		RET		なし
//********************************************************************************
void CfrmUc000Common::CmnGetControlPosition(int* intRow, int* intCol)
{
	// 行取得
	*intRow = CmnGetControlRow();

	// 列取得
	*intCol = CmnGetControlCol();
}

//********************************************************************************
//	現在のコントロール番号から行番号/列番号を取得
//		IN		int*			行番号
//				int*			列番号(移動先)
//				int*			列番号(移動元)
//		RET		なし
//********************************************************************************
void CfrmUc000Common::CmnGetControlPosition(int* intRow, int* intCol, int* intOldCol)
{
	// 列保存
	*intOldCol = *intCol;

	// 行、列取得
	CmnGetControlPosition(intRow, intCol);
}

//********************************************************************************
//	行番号/列番号からコントロールのインデックス番号を取得
//		IN		int				行番号
//				int				列番号
//		RET		インデックス番号
//********************************************************************************
int CfrmUc000Common::CmnGetControlIndex(int intRow, int intCol)
{
	int			intRet;		// 取得インデックス格納
	
	// 取得する行の開始インデックス番号を取得
	intRet = m_uInfo.intColMax * (intRow - 1);

	// インデックス取得
	intRet = intRet + m_uInfo.intCtlStartIndex + intCol - 1;

	return ( intRet );
}

// midori 190301 add -->
//********************************************************************************
//	行番号/列番号からコントロールのインデックス番号を取得(入力項目以外用)
//		IN		int				行番号
//				int				列番号
//		RET		インデックス番号
//********************************************************************************
int CfrmUc000Common::CmnGetControlIndexEx(int intRow, int intCol)
{
	int			intRet;		// 取得インデックス格納
	
	// 取得する行の開始インデックス番号を取得
	intRet = m_uInfo.intColMax * (intRow - 1);

	// インデックス取得
//	intRet = intRet + m_uInfo.intCtlStartIndex + intCol - 1;
	intRet = intRet + intCol;

	return ( intRet );
}
// midori 190301 add <--

//********************************************************************************
//	行番号/列番号から合計行コントロールのインデックス番号を取得
//		IN		int				合計行（1:合計行①，2:合計行②）
//				int				列番号
//		RET		インデックス番号
//********************************************************************************
int CfrmUc000Common::CmnGetControlTotalIndex(int intTotalNo, int intCol)
{
	int			intRet;		// 取得インデックス格納

	//＜注意＞
	// 下記のインデックス番号の算出は通常/個別帳表の「InitialControlTotalAreaHide」でも同じ計算をしている
	// そのためどちらかのシーケンスが変わるとお互いに影響する可能性がある

	// 合計行の開始位置から対象列のインデックス番号を取得
	if (intTotalNo == 1) {
		intRet = m_uInfo.intT1StartIndex;
	}
	else {
		intRet = m_uInfo.intT2StartIndex;
	}
	intRet = intRet + m_uInfo.intCtlStartIndex + intCol - 1;

	return ( intRet );
}

//********************************************************************************
//	計設定と頁情報からカレント頁の合計行コントロールの種別を取得
//		IN		なし
//		RET		int				カレント頁の合計行の種別				
//********************************************************************************
int CfrmUc000Common::CmnGetControlTotalType()
{
	int				intTotalType = m_uInfo.intOutKei;

	// カレント頁の合計行は、通常計設定の設定内容に依存されるが、
	// "頁計と累計"などでは、最終頁とそれ以外の頁で、合計行の表示内容が異なる
	// 当処理では、カレント頁の合計表示の種別を取得する

	switch (intTotalType) {
	case ID_OUTKEI_RUIKEI:
		// "累計"：最終頁のみ"累計"を表示して、それ以外の頁では"なし"
		//		   データ無しの場合（MaxPage=0）１頁として処理したいので、そのまま"累計"で処理したい
		if ((m_uInfo.intMaxPage > 0) && (m_uInfo.intCurPage != m_uInfo.intMaxPage)) {
			intTotalType = ID_OUTKEI_OFF;
		}
		break;

	case ID_OUTKEI_BOTH:
		// "頁計と累計"：データ無しの場合は１頁として"累計"のみ表示
		if (m_uInfo.intMaxPage == 0) {
			intTotalType = ID_OUTKEI_RUIKEI;
		}
		// "頁計と累計"：最終頁以外の頁では"頁計"のみ表示
		else if (m_uInfo.intCurPage != m_uInfo.intMaxPage) {
			intTotalType = ID_OUTKEI_PAGEKEI;
		}
		// "頁計と累計"：１頁しかない場合は"累計"のみ表示
		else if ((m_uInfo.intCurPage == 1) && (m_uInfo.intMaxPage == 1)) {
			intTotalType = ID_OUTKEI_RUIKEI;
		}
		break;
	}
	return intTotalType;
}

//------------------------	元：CUchiwakeFunction	------------------------
/*
CUcFunctionCommon::DiagInit()へ移行
//********************************************************************************
//	ICS-ControlのDIAGRAM_DATA初期化
//		IN		DIAGRAM_DATA*	DIAGRAM_DATAデータ
//		RET		なし
//********************************************************************************
void CfrmUc000Common::CmnDiagInit(DIAGRAM_DATA* diadata)
{
	// 初期化
	diadata->data_edit.Empty();
	diadata->data_disp.Empty();
	diadata->data_combo = 0;
	diadata->data_check = 0;
	memset( diadata->data_val, '\0', sizeof(diadata->data_val));
	memset( diadata->data_day,'\0', sizeof(diadata->data_day));
	diadata->data_imgdata = (char*)NULL;
	diadata->data_imgwidth = 0;
	diadata->data_imgheight = 0;
	diadata->text_combo.Empty();
	diadata->text_check.Empty();
	diadata->movepos = 0;
}
*/
/*
CUcFunctionCommon::DiagGetString()へ移行
//********************************************************************************
//	ICS-Controlから文字列を取得
//		IN		CICSDiag*		対象コントロール
//				int				対象コントロールのインデックス番号
//		RET		対象コントロールの文字列
//********************************************************************************
CString CfrmUc000Common::CmnDiagGetString(CICSDiag* pDiag, int intIndex)
{
	DIAGRAM_DATA	diadata;		// ICSDiag構造体

	// 初期化
	m_clsFunc.DiagInit( &diadata );
	
	// 文字列取得
	pDiag->GetData( intIndex , (LPUNKNOWN)&diadata );
	
	// 取得文字列返送
	return ( diadata.data_edit );
}
*/
//********************************************************************************
//	ICS-Controlから数値を取得
//		IN		CICSDiag*		対象コントロール
//				int				対象コントロールのインデックス番号
//		RET		対象コントロールの数値
//********************************************************************************
int CfrmUc000Common::CmnDiagGetValue(CICSDiag* pDiag, int intIndex)
{
	CString			strTemp;
	int				intTemp;

	// 文字列を取得して数値に変換
	strTemp = m_clsFunc.DiagGetString(pDiag, intIndex);
	intTemp = atoi(strTemp);

	// 数値を返す
	return ( intTemp );
}

//********************************************************************************
//	ICS-Controlから金額を取得
//		IN		CICSDiag*		対象コントロール
//				int				対象コントロールのインデックス番号
//		RET		対象コントロールの金額
//********************************************************************************
CString CfrmUc000Common::CmnDiagGetKingaku(CICSDiag* pDiag, int intIndex)
{
	DIAGRAM_DATA	diadata;		// ICSDiag構造体
	CString			strTemp = "";

	// 初期化
	m_clsFunc.DiagInit( &diadata );

	// 金額取得
	pDiag->GetData( intIndex , (LPUNKNOWN)&diadata );

	if( diadata.data_edit != "" ){
		CmnGetNotCommaValData( &strTemp , &diadata.data_val[0] );
		strTemp.TrimLeft();
	}
	return ( strTemp );
}

//********************************************************************************
//	ICS-Controlから日付を取得
//		IN		CICSDiag*		対象コントロール
//				int				対象コントロールのインデックス番号
//				int				日付変換種別（DC_DATE_WMD, DC_DATE_GWM, ･･･）
//		RET		int				数値変換した日付
//********************************************************************************
long CfrmUc000Common::CmnDiagGetDate(CICSDiag* pDiag, int intIndex, int intFlg)
{
	DIAGRAM_DATA	diadata;		// ICSDiag構造体
	CdateConvert	cDC;			// 日付変換クラス
	long			rtnDate;		// 返信日付データ（数値）

	// 初期化
	m_clsFunc.DiagInit( &diadata );

	// 日付取得
	pDiag->GetData( intIndex , (LPUNKNOWN)&diadata );

	//-----------------------------------
	// 日付データを数値変換して返信
	//-----------------------------------
	switch (intFlg) {
// 修正No.157633_元号日付 add -->
	case DC_DATE_GENGOU:
		rtnDate = diadata.data_day[0];
		break;
// 修正No.157633_元号日付 add <--

	// 和暦、月
	case DC_DATE_WM:
		rtnDate = cDC.Convert(0, diadata.data_day[0], diadata.data_day[1], 0);
		break;
	// 和暦、月、日
	case DC_DATE_WMD:
		rtnDate = cDC.Convert(0, diadata.data_day[0], diadata.data_day[1], diadata.data_day[2]);
		break;
	// 元号、和暦
	case DC_DATE_GW:
		rtnDate = cDC.Convert(diadata.data_day[0], diadata.data_day[1], 0, 0);
		break;
	// 元号、和暦、月
	case DC_DATE_GWM:
		rtnDate = cDC.Convert(diadata.data_day[0], diadata.data_day[1], diadata.data_day[2], 0);
		break;
	// 元号、和暦、月、日
	case DC_DATE_GWMD:
	default:
		rtnDate = cDC.Convert(diadata.data_day[0], diadata.data_day[1], diadata.data_day[2], diadata.data_day[3]);
		break;
	
	}

	return rtnDate;
}

// midori 123 add -->
//********************************************************************************
//	ICS-Controlから日付を取得(ICSDiagの入力形式が「和暦.月」になっているもののみ)　
//	※「和暦.月」の日付変換結果では元号の区別ができないための対応処理
//		IN		CICSDiag*		対象コントロール
//				int				対象コントロールのインデックス番号
//				int				変更前の日付
//		RET		int				数値変換した日付
//********************************************************************************
long CfrmUc000Common::CmnDiagGetDateWM(CICSDiag* pDiag, int intIndex, int intDate )
{
	DIAGRAM_DATA	diadata;		// ICSDiag構造体
	CdateConvert	cDC;			// 日付変換クラス
	long			rtnDate;		// 返信日付データ（数値）
	char			chrDate[4] = "";// 変更前の日付格納
	// 初期化
	m_clsFunc.DiagInit( &diadata );
	memset( chrDate,'\0', sizeof(chrDate));

	// 日付取得
	pDiag->GetData( intIndex , (LPUNKNOWN)&diadata );

	// 変更前の日付を「和暦.月.日」に変換)
	cDC.Convert( intDate, chrDate, DC_DATE_WM );

	// 変更前の「和暦.月」と現在の「和暦.月」を比較
	if ( chrDate[0] == diadata.data_day[0] && 
		 chrDate[1] == diadata.data_day[1] &&
		 chrDate[2] == diadata.data_day[2] && 
		 chrDate[3] == diadata.data_day[3] ){
		rtnDate = intDate;
	}
	// 「和暦.月」が一致しなければ通常の処理(CmnDiagGetDate)を実行
	else{
		rtnDate = CmnDiagGetDate( pDiag, intIndex, DC_DATE_WM );
	}

	return rtnDate;
}
// midori 123 add <--

//********************************************************************************
//	ICS-Controlから日付を取得(ICSDiagの入力形式が「和暦.月.日」になっているもののみ)　
//	※「和暦.月.日」の日付変換結果では元号の区別ができないための対応処理
//		IN		CICSDiag*		対象コントロール
//				int				対象コントロールのインデックス番号
//				int				日付変換種別（DC_DATE_WMD, DC_DATE_GWM, ･･･）
//				int				変更前の日付
//		RET		int				数値変換した日付
//********************************************************************************
long CfrmUc000Common::CmnDiagGetDateWMD(CICSDiag* pDiag, int intIndex, int intFlg, int intDate )
{
	DIAGRAM_DATA	diadata;		// ICSDiag構造体
	CdateConvert	cDC;			// 日付変換クラス
	long			rtnDate;		// 返信日付データ（数値）
	char			chrDate[4] = "";// 変更前の日付格納
	// 初期化
	m_clsFunc.DiagInit( &diadata );
	memset( chrDate,'\0', sizeof(chrDate));

	// 日付取得
	pDiag->GetData( intIndex , (LPUNKNOWN)&diadata );

// midori 181101 add -->
	if(IsValidKaigen() == TRUE) {
		if(diadata.data_day[0] == 0x01 && diadata.data_day[1] == 0x01 && diadata.data_day[2] >= 0x01 && diadata.data_day[2] <= 0x07) {
			memset(diadata.data_day,0x00,sizeof(diadata.data_day));
			pDiag->SetData( intIndex , (LPUNKNOWN)&diadata );
			pDiag->SetPosition(intIndex);
		}
	}
// midori 181101 add <--


	// 変更前の日付を「和暦.月.日」に変換)
	cDC.Convert( intDate, chrDate, intFlg );

	// 変更前の「和暦.月.日」と現在の「和暦.月.日」を比較
	if ( chrDate[0] == diadata.data_day[0] && 
		 chrDate[1] == diadata.data_day[1] &&
		 chrDate[2] == diadata.data_day[2] && 
		 chrDate[3] == diadata.data_day[3] ){
		rtnDate = intDate;
	}
	// 「和暦.月.日」が一致しなければ通常の処理(CmnDiagGetDate)を実行
	else{
		rtnDate = CmnDiagGetDate( pDiag, intIndex, intFlg );
	}

	return rtnDate;
}

// midori 200101 add -->
// ------------------------------------------------------------------------------------------------------
//	ICS-Controlから日付を取得(ICSDiagの入力形式が「元号.和暦.月」になっているもののみ)　
//	※「和暦.月」の日付変換結果では元号の区別ができないための対応処理
//		IN		CICSDiag*		対象コントロール
//				int				対象コントロールのインデックス番号
//				int				変更前の日付
//		RET		int				数値変換した日付
// ------------------------------------------------------------------------------------------------------
long CfrmUc000Common::CmnDiagGetDateGWM(CICSDiag* pDiag, int intIndex, int intDate )
{
	DIAGRAM_DATA	diadata;		// ICSDiag構造体
	CdateConvert	cDC;			// 日付変換クラス
	long			rtnDate;		// 返信日付データ（数値）
	char			chrDate[4] = "";// 変更前の日付格納
	// 初期化
	m_clsFunc.DiagInit( &diadata );
	memset( chrDate,'\0', sizeof(chrDate));

	// 日付取得
	pDiag->GetData( intIndex , (LPUNKNOWN)&diadata );

	// 明治1年9月以前が入力された場合は入力無しにする
	if(diadata.data_day[0] == 0x01 && diadata.data_day[1] == 0x01 && diadata.data_day[2] < 0x09) {
		memset(diadata.data_day,0x00,sizeof(diadata.data_day));
		pDiag->SetData( intIndex , (LPUNKNOWN)&diadata );
		pDiag->SetPosition(intIndex);
	}

	// 変更前の日付を「元号.和暦.月」に変換)
	cDC.Convert( intDate, chrDate, DC_DATE_GWM );

	// 変更前の「元号.和暦.月」と現在の「元号.和暦.月」を比較
	if ( chrDate[0] == diadata.data_day[0] && 
		 chrDate[1] == diadata.data_day[1] &&
		 chrDate[2] == diadata.data_day[2] && 
		 chrDate[3] == diadata.data_day[3] ){
		rtnDate = intDate;
	}
	// 「元号.和暦.月」が一致しなければ通常の処理(CmnDiagGetDate)を実行
	else{
		rtnDate = CmnDiagGetDate( pDiag, intIndex, DC_DATE_GWM );
	}

	return rtnDate;
}

// ------------------------------------------------------------------------------------------------------
//	ICS-Controlから日付を取得(ICSDiagの入力形式が「元号.和暦.月.日」になっているもののみ)　
//	※「和暦.月.日」の日付変換結果では元号の区別ができないための対応処理
//		IN		CICSDiag*		対象コントロール
//				int				対象コントロールのインデックス番号
//				int				日付変換種別（DC_DATE_WMD, DC_DATE_GWM, ･･･）
//				int				変更前の日付
//		RET		int				数値変換した日付
// ------------------------------------------------------------------------------------------------------
long CfrmUc000Common::CmnDiagGetDateGWMD(CICSDiag* pDiag, int intIndex, int intDate )
{
	DIAGRAM_DATA	diadata;		// ICSDiag構造体
	CdateConvert	cDC;			// 日付変換クラス
	long			rtnDate;		// 返信日付データ（数値）
	char			chrDate[4] = "";// 変更前の日付格納
	// 初期化
	m_clsFunc.DiagInit( &diadata );
	memset( chrDate,'\0', sizeof(chrDate));

	// 日付取得
	pDiag->GetData( intIndex , (LPUNKNOWN)&diadata );

	// 明治1年9月8日以前が入力された場合は入力無しにする
	if(diadata.data_day[0] == 0x01 && diadata.data_day[1] == 0x01 && (diadata.data_day[2] < 0x09 || (diadata.data_day[2] == 0x09 && diadata.data_day[3] < 0x08))) {
		memset(diadata.data_day,0x00,sizeof(diadata.data_day));
		pDiag->SetData( intIndex , (LPUNKNOWN)&diadata );
		pDiag->SetPosition(intIndex);
	}

	// 変更前の日付を「元号.和暦.月.日」に変換)
	cDC.Convert( intDate, chrDate, DC_DATE_GWMD );

	// 変更前の「元号.和暦.月.日」と現在の「元号.和暦.月.日」を比較
	if ( chrDate[0] == diadata.data_day[0] && 
		 chrDate[1] == diadata.data_day[1] &&
		 chrDate[2] == diadata.data_day[2] && 
		 chrDate[3] == diadata.data_day[3] ){
		rtnDate = intDate;
	}
	// 「元号.和暦.月.日」が一致しなければ通常の処理(CmnDiagGetDate)を実行
	else{
		rtnDate = CmnDiagGetDate( pDiag, intIndex, DC_DATE_GWMD );
	}

	return rtnDate;
}
// midori 200101 add <--

//********************************************************************************
//	ICS-Controlへ文字列を設定
//		IN		CICSDiag*		対象コントロール
//				int				対象コントロールのインデックス番号
//				CString			セットする文字列
//				int				対象コントロールの種別（0:入力，1:表示）
//		RET		なし
//********************************************************************************
void CfrmUc000Common::CmnDiagSetString(CICSDiag* pDiag, int intIndex, CString strData, int intFlg)
{
	DIAGRAM_DATA	diadata;		// ICSDiag構造体

	if ( strData != "" ) {
		// 初期化
		m_clsFunc.DiagInit( &diadata );

		if ( intFlg == 1 ) {
			diadata.data_disp = strData;		// 1:表示
		}
		else {
			diadata.data_edit = strData;		// 0:入力
		}

		// 文字列セット
		pDiag->SetData( intIndex, (LPUNKNOWN)&diadata );
	}
	else {
		// データクリア
		pDiag->DataClear( intIndex, TRUE );
	}
}

//********************************************************************************
//	ICS-Controlへ値を設定
//		IN		CICSDiag*		対象コントロール
//				int				対象コントロールのインデックス番号
//				int				セットする値
//				int				対象コントロールの種別（0:入力，1:表示）
//		RET		なし
//********************************************************************************
void CfrmUc000Common::CmnDiagSetValue(CICSDiag* pDiag, int intIndex, int intData, int intFlg)
{
	CString			strTemp;

	// 値を文字列に変換して設定
	strTemp.Format("%d", intData);
	CmnDiagSetString(pDiag, intIndex, strTemp, intFlg);
}

//********************************************************************************
//	ICS-ControlのComboBoxへ設定
//		IN		CICSDiag*		対象コントロール
//				int				対象コントロールのインデックス番号
//				long			ConmoBoxの選択番号
//		RET		なし
//********************************************************************************
void CfrmUc000Common::CmnDiagSetCombo(CICSDiag* pDiag, int intIndex, long lngData)
{
	DIAGRAM_DATA	diadata;		// ICSDiag構造体

	// 初期化
	m_clsFunc.DiagInit( &diadata );

	pDiag->GetData( intIndex , (LPUNKNOWN)&diadata );

	diadata.data_combo = (short)lngData;

	pDiag->SetData( intIndex , (LPUNKNOWN)&diadata );
}

// midori 152137 add -->
//********************************************************************************
//	ICS-ControlのComboBoxへ設定（KnOrderからComboBoxのインデックスに変更してセット）
//		IN		CICSDiag*		対象コントロール
//				int				対象コントロールのインデックス番号
//				long			科目順序番号
//		RET		なし
//********************************************************************************
void CfrmUc000Common::CmnDiagSetCombo2(CICSDiag* pDiag, int intIndex, long lngData)
{
	DIAGRAM_DATA	diadata;		// ICSDiag構造体
	short			sel=0;

	m_SortMap1.Lookup(lngData,sel);

	// 初期化
	m_clsFunc.DiagInit( &diadata );

	pDiag->GetData( intIndex , (LPUNKNOWN)&diadata );

	diadata.data_combo = sel;

	pDiag->SetData( intIndex , (LPUNKNOWN)&diadata );
}
// midori 152137 add <--

// インボイス登録番号追加対応_23/11/17 add -->
//********************************************************************************
//	ICS-ControlのComboBoxへ設定（登録番号のデータによってComboBoxのインデックスに変更してセット）
//		IN		CICSDiag*		対象コントロール
//				int				対象コントロールのインデックス番号
//				long			科目順序番号
//		RET		なし
//********************************************************************************
void CfrmUc000Common::CmnDiagSetCombo3(CICSDiag* pDiag, int intIndex, CString strData)
{
	DIAGRAM_DATA	diadata;		// ICSDiag構造体
	short			sel = 0;
	CString			cs1, cs2;

	// 一番左端の文字が「T」か「空白」かそれ以外か
	cs1 = strData;
	cs2 = cs1.Left(1);
	if(strcmp(cs2, _T("T")) == 0)		sel = 0;
	else if(strcmp(cs2, _T(" ")) == 0)	sel = 1;
	else								sel = 0;

	// 初期化
	m_clsFunc.DiagInit(&diadata);

	pDiag->GetData(intIndex, (LPUNKNOWN)&diadata);

	diadata.data_combo = sel;

	pDiag->SetData(intIndex, (LPUNKNOWN)&diadata);
}
// インボイス登録番号追加対応_23/11/17 add <--

//********************************************************************************
//	ICS-Controlへ金額を設定
//		IN		CICSDiag*		対象コントロール
//				int				対象コントロールのインデックス番号
//				CString			セットする金額
//		RET		なし
//********************************************************************************
void CfrmUc000Common::CmnDiagSetKingaku(CICSDiag* pDiag, int intIndex, CString strData)
{
	DIAGRAM_DATA	diadata;		// ICSDiag構造体

	if ( strData != "" ) {
		// 初期化
		m_clsFunc.DiagInit( &diadata );

		// 金額を変換
		CmnChangeFieldValData( &diadata.data_val[0], strData );

		// 文字列セット
		pDiag->SetData( intIndex, (LPUNKNOWN)&diadata );
	}
	else {
		// データクリア
		pDiag->DataClear( intIndex, TRUE );
	}
}

//********************************************************************************
//	ICS-Controlへ日付を設定
//		IN		CICSDiag*		対象コントロール
//				int				対象コントロールのインデックス番号
//				int				日付表示種別（DC_DATE_WMD, DC_DATE_GWM, ･･･）
//				int				セットする日付
//		RET		なし
//********************************************************************************
void CfrmUc000Common::CmnDiagSetDate(CICSDiag* pDiag, int intIndex, int intFlg, long intDate)
{
	DIAGRAM_DATA	diadata;		// ICSDiag構造体
	CdateConvert	cDC;			// 日付変換クラス

	if ( intDate != 0 ) {
		// 初期化
		m_clsFunc.DiagInit( &diadata );

		// int型の日付を表示用に変換する
		cDC.Convert(intDate, diadata.data_day, intFlg);

		// 日付をセット
		pDiag->SetData( intIndex, (LPUNKNOWN)&diadata );
	}
	else {
		// データクリア
		pDiag->DataClear( intIndex, TRUE );
	}
}

//********************************************************************************
//	ICS-Controlへ日付を設定(ICSDiagの入力形式が「和暦.月.日」になっているもののみ)　
//	※「和暦.月.日」では元号の区別ができないための対応処理
//		→日付が昭和のものである場合、和暦値を「平成」でのものに変換(昭和64年→平成1年)
//		IN		CICSDiag*		対象コントロール
//				int				対象コントロールのインデックス番号
//				int				日付表示種別（DC_DATE_WMD, DC_DATE_GWM, ･･･）
//				int				セットする日付
//		RET		なし
//********************************************************************************
void CfrmUc000Common::CmnDiagSetDateWMD(CICSDiag* pDiag, int intIndex, int intFlg, long intDate)
{
	DIAGRAM_DATA	diadata;		// ICSDiag構造体
	CdateConvert	cDC;			// 日付変換クラス
	int				intRet = -1;

	if ( intDate != 0 ) {
		// 初期化
		m_clsFunc.DiagInit( &diadata );

		// 日付の範囲が「昭和(19890101～19890107)」かチェック
		if ( cDC.ConvertWMD(intDate, diadata.data_day, intFlg ) == TRUE ){
			// 和暦を「平成」での値に変換したものをセット
			pDiag->SetData( intIndex, (LPUNKNOWN)&diadata );
		}
		else{
			// 「昭和」ではないので通常の処理(CmnDiagSetDate)を実行
			CmnDiagSetDate( pDiag, intIndex, intFlg, intDate );
		}
	}
	else {
		// データクリア
		pDiag->DataClear( intIndex, TRUE );
	}
}

//*******************************************************************************
//	ICS-Controlの対象コントロールにフォーカス設定
//		IN		CICSDiag*		対象コントロール
//				int				行番号
//				int				列番号
//		RET		なし
//*******************************************************************************
void CfrmUc000Common::CmnDiagSetFocus(CICSDiag* pDiag, int intRow, int intCol)
{
	int		intIndex;		// 移動インデックス
	//int		intTotalType;

	// 行、列からインデックス取得
	intIndex = CmnGetControlIndex(intRow, intCol);

	//【方針】
	//	ここで計算されたインデックス番号はデータ行に対するインデックス番号である
	//	実際は対象行が合計行かもしれない事を考慮する必要がある
	//					↓
	//	削除ボタンで最終頁が削除され、カレント頁の頁計/累計表示内容が変わる可能性がある
	//	合わせてPageUp/PageDownの移動先制御もここで行う

	//**************************************************************
	// 計を別ダイアログ化により、合計行を考慮する必要なし(2012/3/30
	//**************************************************************
	//カレント頁の合計行の状態を取得
	//intTotalType = CmnGetControlTotalType();

	// 合計行なら移動先のインデックスを差し替える
	//if (m_uInfo.intRowKei == 1) {
	//	// 計あり帳表
	//	switch (intTotalType) {		// 合計行の状態で分岐
	//	case ID_OUTKEI_OFF:					// なし
	//	case ID_OUTKEI_PAGEKEI:				// 頁計
	//	case ID_OUTKEI_RUIKEI:				// 累計
	//		// フォーカス移動先は合計行②？
	//		if (intRow > (m_uInfo.intRowMax - 1)) {
	//			intIndex = CmnGetControlTotalIndex(2, m_uInfo.intCursolKeiPos);
	//		}
	//		break;

	//	case ID_OUTKEI_BOTH:				// 頁計と累計
	//		// フォーカス移動先は合計行①？
	//		if (intRow == (m_uInfo.intRowMax - 1)) {
	//			intIndex = CmnGetControlTotalIndex(1, m_uInfo.intCursolKeiPos);
	//		}
	//		// フォーカス移動先は合計行②？
	//		else if (intRow > (m_uInfo.intRowMax - 1)) {
	//			intIndex = CmnGetControlTotalIndex(2, m_uInfo.intCursolKeiPos);
	//		}
	//		break;
	//	}
	//}
	//else {
	//	// 計なし帳表
	//	switch (intTotalType) {		// 合計行の状態で分岐
	//	case ID_OUTKEI_OFF:					// なし
	//		break;

	//	case ID_OUTKEI_PAGEKEI:				// 頁計
	//	case ID_OUTKEI_RUIKEI:				// 累計
	//		// フォーカス移動先は合計行②？
	//		if (intRow > (m_uInfo.intRowMax - 1)) {
	//			intIndex = CmnGetControlTotalIndex(2, m_uInfo.intCursolKeiPos);
	//		}
	//		break;

	//	case ID_OUTKEI_BOTH:				// 頁計と累計
	//		// フォーカス移動先は合計行①？
	//		if (intRow == (m_uInfo.intRowMax - 1)) {
	//			intIndex = CmnGetControlTotalIndex(1, m_uInfo.intCursolKeiPos);
	//		}
	//		// フォーカス移動先は合計行②？
	//		else if (intRow > (m_uInfo.intRowMax - 1)) {
	//			intIndex = CmnGetControlTotalIndex(2, m_uInfo.intCursolKeiPos);
	//		}
	//		break;
	//	}
	//}

// midori 152710 del -->
//// midori 151403 cor -->
////// midori 160612 cor -->
//////	// カーソル移動
//////	pDiag->SetPosition( intIndex );
////// ---------------------
////	if(m_DataKakutei == FALSE)	pDiag->SetPosition( intIndex );
////	else						((CICSDiag *)GetDlgItem(IDC_PAGEDIAG1))->SetPosition(1);
////// midori 160612 cor <--
//// ---------------------
//	if(((CMainFrame*)AfxGetMainWnd())->m_Init == FALSE)	{
//		if(m_DataKakutei == FALSE)	pDiag->SetPosition( intIndex );
//		else						((CICSDiag *)GetDlgItem(IDC_PAGEDIAG1))->SetPosition(1);
//	}
//// midori 151403 cor <--
// midori 152710 del <--
// midori 152710 add -->
	if(m_DataKakutei == FALSE)	pDiag->SetPosition( intIndex );
	else 						((CICSDiag *)GetDlgItem(IDC_PAGEDIAG1))->SetPosition(1);
// midori 152710 add <--
}

//********************************************************************************
//	ICS-Controlの入力文字数を変更
//		IN		CICSDiag*		対象コントロール
//				int				対象コントロールのインデックス番号
//				int				入力文字数（0:カーソル移動するが入力不可）
//		RET		なし
//********************************************************************************
void CfrmUc000Common::CmnDiagSetAttrInpmax(CICSDiag* pDiag, int intIndex, int intInpmax)
{
	DIAGRAM_ATTRIBUTE	diaatt;			// ICSDiag属性構造体

	// 現在の属性を取得
	pDiag->GetAttribute( intIndex , (LPUNKNOWN)&diaatt );

	// 変更桁数を格納
	diaatt.attr_inpmax = intInpmax;

	// 属性変更
	pDiag->SetAttribute( intIndex , (LPUNKNOWN)&diaatt , TRUE );
}

//********************************************************************************
//	ICS-Controlの金額欄の桁数変更
//		IN		CICSDiag*		対象コントロール
//				int				対象コントロールのインデックス番号
//				int				桁数
//				BOOL			カンマ設定（TRUE：カンマ付き／FALSE：カンマ無し）
//				BOOL			小数点設定（TRUE：小数点付き／FALSE：小数点無し）
//				int				小数点以下の桁数
//		RET		なし
//********************************************************************************
void CfrmUc000Common::CmnDiagSetAttrDigit(CICSDiag* pDiag, int intIndex, int intDigit,
										  BOOL fKanma /*=TRUE*/, BOOL fDecimal /*=FALSE*/, int nDecDegit /*=0*/)
{
	DIAGRAM_ATTRIBUTE	diaatt;			// ICSDiag属性構造体

	// 現在の属性を取得
	pDiag->GetAttribute( intIndex , (LPUNKNOWN)&diaatt );

	// 変更桁数を格納
	diaatt.attr_inpmax = intDigit;

	// 金額フォーマット取得
	diaatt.attr_editformat = CmnDiagGetDigitFormat( intDigit, &diaatt, fKanma, fDecimal, nDecDegit );

	// 属性変更
	pDiag->SetAttribute( intIndex , (LPUNKNOWN)&diaatt , TRUE );
}

//********************************************************************************
//	ICS-Controlの金額欄に指定するフォーマット文字列作成
//		IN		int				桁数
//				BOOL			カンマ設定（TRUE：カンマ付き／FALSE：カンマ無し）
//				BOOL			小数点設定（TRUE：小数点付き／FALSE：小数点無し）
//				int				小数点以下の桁数
//		RET		フォーマット文字列（ss,sss,sss,ss9）
//********************************************************************************
CString CfrmUc000Common::CmnDiagGetDigitFormat(int intDigit, DIAGRAM_ATTRIBUTE* pudAttr,
											   BOOL fKanma /*=TRUE*/, BOOL fDecimal /*=FALSE*/, int nDecDegit /*=0*/)
{
	CString				strFormat = _T("9");	// 想定外の桁数なら１桁とする
	int					i;

	// カンマ付き？
	if ( fKanma == TRUE ){
		// 「カンマ編集」ON
		pudAttr->attr_editattr |= 0x02;
	}

	if (intDigit > 0) {
		for (i = 2; i <= intDigit; i++) {
			// 4, 7, 10･･･桁目の場合にカンマ付きでセットする
			if ((i % 3 == 1) && ( fKanma == TRUE )){
				strFormat = "s," + strFormat;		// s,ss9
			}
			else {
				strFormat = "s" + strFormat;		// ss9
			}
		}
	}

	//	小数点付き？
	if ( fDecimal == TRUE ){

		//	小数点以下の桁数あり？
		if ( nDecDegit > 0 ){
			
			//	小数点以下の桁数を設定
			pudAttr->attr_frctmax = nDecDegit;
			//	小数点以下を扱う属性を設定
			pudAttr->attr_editattr |= 0x04;
			
		}
	}

	return strFormat;
}

//**********************************************************************************
//	ICS-Controlの対象列に｢半角/全角｣混在入力を設定
//		IN		CICSDiag*		対象コントロール
//				int				列番号
//		RET		なし
//**********************************************************************************
void CfrmUc000Common::CmnDiagSetAttrColZenHan(CICSDiag* pDiag, int intCol)
{
	DIAGRAM_ATTRIBUTE	diaatt;		// ICSDiag属性構造体
	int					i;

	// 指定された列番号の「ANK(半角文字)」有効
	for ( i = CmnGetControlIndex(1, intCol); i < m_uInfo.intT1StartIndex; i += m_uInfo.intColMax ) {
		// 対象セルの現属性を取得
		pDiag->GetAttribute(i, (LPUNKNOWN)&diaatt);

		// 属性にANK文字の入力有効指定
		diaatt.attr_editattr = DIAG_EATTR_ZENHAN;

		// 対象セルにANK文字入力有効指定
		pDiag->SetAttribute(i, (LPUNKNOWN)&diaatt, TRUE);
	}
}

//********************************************************************************
//	ICS-Controlの対象コントロールに指定の背景色を設定
//		IN		CICSDiag*		対象コントロール
//				int				対象コントロールのインデックス番号
//				COLORREF*		背景色
//		RET		なし
//********************************************************************************
void CfrmUc000Common::CmnDiagSetAttrColor(CICSDiag* pDiag, int intIndex, COLORREF* color)
{
	DIAGRAM_ATTRIBUTE	diaatt;		// ICSDiag属性構造体

	// 対象セルの現属性を取得
	pDiag->GetAttribute( intIndex , (LPUNKNOWN)&diaatt );

	// 属性に背景色を指定
	memcpy( &diaatt.attr_bcolor , color , sizeof( COLORREF ) );

	// 属性変更
	pDiag->SetAttribute( intIndex , (LPUNKNOWN)&diaatt , TRUE );
}

//********************************************************************************
//	ICS-Controlの背景色をクリア
//		IN		CICSDiag*		対象コントロール
//		RET		なし
//********************************************************************************
void CfrmUc000Common::CmnDiagSetAttrColorAllClear(CICSDiag* pDiag)
{
	DIAGRAM_ATTRIBUTE	diaatt;				// ICSDiag属性構造体
	int					intIndex;
	int					i;
	int					j;
	int					sw=0,per=0,col=0;

	// 白で全てクリアする
	COLORREF color = RGB( 255 , 255 , 255 );

	for (i = 0; i < m_uInfo.intRowMaxData; i++) {
		for (j = 0; j < m_uInfo.intColMax; j++) {
			// 対象コントロールのインデックス番号を取得
			intIndex = (m_uInfo.intColMax * i) + j;

			// 属性を取得
			pDiag->GetAttribute(intIndex, (LPUNKNOWN)&diaatt);

			// 背景色を設定
			memcpy(&diaatt.attr_bcolor, &color, sizeof(COLORREF));

			// 入力コントロールのみフレーム色を設定：これをしないと枠線が表示される
			if ((j >= m_uInfo.intCtlStartIndex) && (j < m_uInfo.intCtlStartIndex + m_uInfo.intColMaxData)) {
// インボイス登録番号追加対応_23/11/27 del -->
				//memcpy(&diaatt.attr_frcolor, &color, sizeof(COLORREF));
// インボイス登録番号追加対応_23/11/27 del <--
// インボイス登録番号追加対応_23/11/27 add -->
				sw = 0;
				if(bG_InvNo == TRUE) {
					if(m_uInfo.intFormSeq == ID_FORMNO_021) {
						per = (intIndex % m_uInfo.intColMax);
						col = per - m_uInfo.intCtlStartIndex + 1;
						// 直接入力に変更したため、フレームの色変更を行わない
						if(col == ID_COL_021I_SPDATE || col == ID_COL_021I_PMDATE || col == ID_COL_021I_TEKI) {
							sw = 1;
						}
					}
					else if(m_uInfo.intFormSeq == ID_FORMNO_153) {
						per = (intIndex % m_uInfo.intColMax);
						col = per - m_uInfo.intCtlStartIndex + 1;
						// 直接入力に変更したため、フレームの色変更を行わない
						if(col == ID_COL_153I_HDNAME || col == ID_COL_153I_ADNAME1 ||
						   col == ID_COL_153I_ADADD1 || col == ID_COL_153I_ADADD2 || col == ID_COL_153I_TEKI) {
							sw = 1;
						}
					}
				}
				if(sw == 0)	memcpy(&diaatt.attr_frcolor, &color, sizeof(COLORREF));
// インボイス登録番号追加対応_23/11/27 add <--
			}

// インボイス登録番号追加対応_23/11/27 del -->
			// ⑮-3のみ、住所の区切り線の色も変更する
			if(bG_InvNo == TRUE && m_uInfo.intFormSeq == ID_FORMNO_153) {
				per = (intIndex % m_uInfo.intColMax);
				col = per - m_uInfo.intCtlStartIndex + 1;
				if(col == 14)	{
					memcpy(&diaatt.attr_lfcolor, &color, sizeof(COLORREF));	// 線の色
					memcpy(&diaatt.attr_lbcolor, &color, sizeof(COLORREF));	// 線の隙間の色
				}
			}
// インボイス登録番号追加対応_23/11/27 del <--

			// 属性を変更
			pDiag->SetAttribute(intIndex, (LPUNKNOWN)&diaatt, TRUE);	
		}
	}
}

//********************************************************************************
//	ICS-Controlの対象行に特殊行の属性設定を行う
//		IN		CICSDiag*		対象コントロール
//				int				対象行番号
//				int				特殊行フラグ（FgFunc）
//		RET		なし
//********************************************************************************
void CfrmUc000Common::CmnDiagSetAttrSpecialRow(CICSDiag* pDiag, int intRow, int FgFunc)
{
	DIAGRAM_ATTRIBUTE	diaatt;				// ICSDiag属性構造体
	COLORREF			color;				// RGB構造体
	int					intStartIndex;
	int					intEndIndex;
	int					i;
// midori M-16113001 cor -->
//// midori 160612 add -->
//	int					csw=0;
//	BOOL				bedit=FALSE;
//	ICSColorInfo		icsColInfo;
//// midori 160612 add <--
// -------------------------
	BOOL				bedit=FALSE;
// midori M-16113001 cor <--

	// 特殊行の色を決定
	switch( FgFunc ){
	case ID_FGFUNC_SYOKEI:
	case ID_FGFUNC_SYOKEINULL:
		color = COLOR_SYOKEI;				// 小計行
		break;
	case ID_FGFUNC_CHUKEI:
	case ID_FGFUNC_CHUKEINULL:
		color = COLOR_CHUKEI;				// 中計行
		break;
	case ID_FGFUNC_IKKATUMANUAL:
		color = COLOR_IKKATUMANUAL;			// 一括集計金額行（手動）
		break;
	case ID_FGFUNC_IKKATUAUTO:
		color = COLOR_IKKATUAUTO;			// 一括集計金額行（自動）
		break;
// midori 190505 add -->
	case ID_FGFUNC_KAMOKU:
		color = COLOR_KAMOKU;				// 科目行（見出し）
		break;
// midori 190505 add <--
	default:
// midori M-16113001 cor -->
//// midori 160612 cor -->
////		return;								// 上記以外は設定しない
////		break;
//// ---------------------
//		// 背景色取得＆指定
//		if(m_DataKakutei)	{
//			color = BC_DISABLE;
//		}
//		else	{
//			if ( ((ICSWinApp *)AfxGetApp())->GetStanderdColor( icsColInfo ) == 0 ){
//				if(icsColInfo.m_swOnOff == 1)	{
//					csw = 1;
//				}
//			}
//			if(csw)	{
//				color	= icsColInfo.BackColor();
//			}
//			else	{
//				color	= (COLORREF)RGB(0xff,0xff,0xff);	//	白色
//			}
//		}
//// midori 160612 cor <--
// -------------------------
		// 背景色取得＆指定
		if(m_DataKakutei)	color = BC_DISABLE;
		else				color = RGB(255,255,255);
// midori M-16113001 cor <--
	}

	// 指定行から、開始/終了のコントロール番号を取得
	intStartIndex = (intRow - 1) * m_uInfo.intColMax;
	intEndIndex = intRow * m_uInfo.intColMax - 1;

// midori 160612 add -->
	if(m_DataKakutei == FALSE)	bedit=TRUE;
	else						bedit=FALSE;
// midori 160612 add <--

	int sw = 0;	// インボイス登録番号追加対応_23/11/27 add

	for (i = intStartIndex; i <= intEndIndex; i++) {
		// 属性を取得
		pDiag->GetAttribute(i, (LPUNKNOWN)&diaatt);

		// 背景色を設定
		memcpy(&diaatt.attr_bcolor, &color, sizeof(COLORREF));

		// 入力コントロールのみフレーム色を設定：これをしないと白枠が表示される
		if ((i >= intStartIndex + m_uInfo.intCtlStartIndex) && (i < intStartIndex + m_uInfo.intCtlStartIndex + m_uInfo.intColMaxData)) {
// インボイス登録番号追加対応_23/11/27 del -->
			//memcpy(&diaatt.attr_frcolor, &color, sizeof(COLORREF));
// インボイス登録番号追加対応_23/11/27 del <--
// インボイス登録番号追加対応_23/11/27 add -->
			sw = 0;
			if(bG_InvNo == TRUE) {
				if(m_uInfo.intFormSeq == ID_FORMNO_021) {
					// 直接入力に変更したため、フレームの色変更を行わない
					if(i == CmnGetControlIndex(intRow, ID_COL_021I_SPDATE) ||
					   i == CmnGetControlIndex(intRow, ID_COL_021I_PMDATE) ||
					   i == CmnGetControlIndex(intRow, ID_COL_021I_TEKI)) {
						sw = 1;
					}
				}
				else if(m_uInfo.intFormSeq == ID_FORMNO_153) {
					// 直接入力に変更したため、フレームの色変更を行わない
					if(i == CmnGetControlIndex(intRow, ID_COL_153I_HDNAME) || i == CmnGetControlIndex(intRow, ID_COL_153I_ADNAME1) ||
					   i == CmnGetControlIndex(intRow, ID_COL_153I_ADADD1) || i == CmnGetControlIndex(intRow, ID_COL_153I_ADADD2) ||
					   i == CmnGetControlIndex(intRow, ID_COL_153I_TEKI) ) {
						sw = 1;
					}
				}
			}
			
			if(sw == 0)	memcpy(&diaatt.attr_frcolor, &color, sizeof(COLORREF));
// インボイス登録番号追加対応_23/11/27 add <--
		}

// インボイス登録番号追加対応_23/11/27 del -->
		// ⑮-3のみ、住所の区切り線の色も変更する
		if(bG_InvNo == TRUE && m_uInfo.intFormSeq == ID_FORMNO_153 && i == CmnGetControlIndex(intRow, 14)) {
			memcpy(&diaatt.attr_lfcolor, &color, sizeof(COLORREF));	// 線の色
			memcpy(&diaatt.attr_lbcolor, &color, sizeof(COLORREF));	// 線の隙間の色
		}
// インボイス登録番号追加対応_23/11/27 del <--

		// 属性を変更
		pDiag->SetAttribute(i, (LPUNKNOWN)&diaatt, TRUE);	

// midori 160612 add -->
		// 入力可／不可の設定
		DlgSet_Attr(pDiag,i,bedit);
// midori 160612 add <--
	}
}

//**************************************************
//	自動計算対象セルの属性設定を行う
//	【引数】	pDiag		…	ICSDiagコントロール変数のポインタ
//				nBackPos	…	背景位置
//				nDataPos	…	データ位置
//				nFgFunc		…	特殊行フラグ
//				bFgInput	…	自動計算フラグ
//	【戻値】	なし
//**************************************************
void CfrmUc000Common::CmnDiagSetAttrFgInputCell( CICSDiag* pDiag, int nBackPos, int nDataPos, int nFgFunc, BYTE bFgInput )
{
	DIAGRAM_ATTRIBUTE	udAttr;		//	ICSDiag属性構造体
	COLORREF			udColor;	//	RGB構造体
	int					nPos[2];	//	位置格納
	int					nCnt;		//	カウント用
	int					sw=0, per=0, col=0;

// midori 160612 add -->
	// 入力データを確定するにチェック有り
	if(m_DataKakutei)	return;
// midori 160612 add <--

	//	特殊行フラグで分岐
	switch( nFgFunc ){
	//	空行
	case ID_FGFUNC_NULL:
	//	データ行
	case ID_FGFUNC_DATA:
	//	一括金額（手動）
	case ID_FGFUNC_IKKATUMANUAL:
	
		//	自動計算しない？
		if ( bFgInput == ID_AUTOCALC_INPUT ){
			udColor = COLOR_AUTOCALC_INPUT;			//	行内自動計算
		}
		//	自動計算する
		else{
			
			//	一括金額（手動）？
			if ( nFgFunc == ID_FGFUNC_IKKATUMANUAL ){
				udColor = COLOR_IKKATUMANUAL;
			}
			else{
				udColor = RGB( 255, 255, 255 );			//	白
			}
		}
		
		//	背景位置取得
		nPos[0] = nBackPos;
		//	データ位置取得
		nPos[1] = nDataPos;
		
		//	ループ
		for( nCnt = 0; nCnt < 2; nCnt++ ){
			// 属性を取得
			pDiag->GetAttribute( nPos[nCnt], ( LPUNKNOWN )&udAttr );
			// 背景色を設定
			memcpy (&udAttr.attr_bcolor, &udColor, sizeof( COLORREF ) );
			
			//	入力？
			if ( nCnt == 1 ){
// インボイス登録番号追加対応_23/11/27 del -->
				////	枠色を設定（※これをしないと白枠が表示される）
				//memcpy( &udAttr.attr_frcolor, &udColor, sizeof( COLORREF ) );
// インボイス登録番号追加対応_23/11/27 del <--
// インボイス登録番号追加対応_23/11/27 add -->
				sw = 0;
				if(bG_InvNo == TRUE) {
					if(m_uInfo.intFormSeq == ID_FORMNO_021) {
						// 直接入力に変更したため、フレームの色変更を行わない
						per = (nPos[nCnt] % m_uInfo.intColMax);
						col = per - m_uInfo.intCtlStartIndex + 1;
						if(col == ID_COL_021I_SPDATE || col == ID_COL_021I_PMDATE || col == ID_COL_021I_TEKI) {
							sw = 1;
						}
					}
					else if(m_uInfo.intFormSeq == ID_FORMNO_153) {
						// 直接入力に変更したため、フレームの色変更を行わない
						per = (nPos[nCnt] % m_uInfo.intColMax);
						col = per - m_uInfo.intCtlStartIndex + 1;
						if(col == ID_COL_153I_HDNAME || col == ID_COL_153I_ADNAME1 ||
						   col == ID_COL_153I_ADADD1 || col == ID_COL_153I_ADADD1 || col == ID_COL_153I_TEKI) {
							sw = 1;
						}
					}
				}
			
				// 枠色を設定（※これをしないと白枠が表示される）
				if(sw == 0)	memcpy(&udAttr.attr_frcolor, &udColor, sizeof(COLORREF));
// インボイス登録番号追加対応_23/11/27 add <--
			}

// インボイス登録番号追加対応_23/11/27 del -->
			// ⑮-3のみ、住所の区切り線の色も変更する
			if(bG_InvNo == TRUE && m_uInfo.intFormSeq == ID_FORMNO_153) {
				per = (nPos[nCnt] % m_uInfo.intColMax);
				col = per - m_uInfo.intCtlStartIndex + 1;
				if(col == 14)	{
					memcpy(&udAttr.attr_lfcolor, &udColor, sizeof(COLORREF));	// 線の色
					memcpy(&udAttr.attr_lbcolor, &udColor, sizeof(COLORREF));	// 線の隙間の色
				}
			}
// インボイス登録番号追加対応_23/11/27 del <--

			// 属性を変更
			pDiag->SetAttribute( nPos[nCnt], ( LPUNKNOWN )&udAttr, TRUE );
		}
		break;
	default:
		break;
	}
}

// No.200601 add -->
//**************************************************
//	電子申告の文字数と比較する対象セルの属性設定を行う
//	【引数】	pDiag		…	ICSDiagコントロール変数のポインタ
//				nDataPos	…	データ位置
//				nFgFunc		…	特殊行フラグ
//				nFgInput	…	自動計算フラグ
//	【戻値】	なし
//**************************************************
void CfrmUc000Common::CmnDiagSetAttrFgStringCell( CICSDiag* pDiag, int nDataPos, int nFgFunc, int nFgInput )
{
	DIAGRAM_ATTRIBUTE	udAttr;		//	ICSDiag属性構造体
	COLORREF			udColor;	//	RGB構造体
	int					nPos[2];	//	位置格納

	// 簡素化のみ
	if(bG_Kanso == FALSE)	return;

	// 特殊行フラグで分岐
	switch( nFgFunc ){
		// 空行
		case ID_FGFUNC_NULL:
		// データ行
		case ID_FGFUNC_DATA:
		// 一括金額（手動）
		case ID_FGFUNC_IKKATUMANUAL:
// 修正No.157890 add -->
		// 一括金額（自動）
		case ID_FGFUNC_IKKATUAUTO:
// 修正No.157890 add <--

			// 電子申告の文字数を超えたセル？
			if(nFgInput == ID_AUTOCHAR_OUT)	{
				udColor = COLOR_AUTOCHAR_OUT;		// 電子申告の文字数を超えた
			}
			else	{
				udColor = RGB( 0, 0, 0 );			// 黒
			}
		
			//	データ位置取得
			nPos[0] = nDataPos;
			// 属性を取得
			pDiag->GetAttribute(nPos[0], ( LPUNKNOWN )&udAttr);
			// 文字色を設定
			memcpy(&udAttr.attr_fcolor, &udColor, sizeof(COLORREF));
			// 属性を変更
			pDiag->SetAttribute(nPos[0], ( LPUNKNOWN )&udAttr, TRUE);

			break;

// 修正No.157890 add -->
		case ID_FGFUNC_SYOKEI:			//	10	小計行
		case ID_FGFUNC_SYOKEINULL:		//	11	小計行（空行）
		case ID_FGFUNC_CHUKEI:			//	12	中計行
		case ID_FGFUNC_CHUKEINULL:		//	13	中計行（空行）
		case ID_FGFUNC_RUIKEI:			//	14	累計行
		case ID_FGFUNC_PAGEKEI:			//	15	頁計行
			// 元々データ行で文字色が青になっていた行で特殊行挿入をした場合、
			// 文字色が青のままで挿入されることがあるので、計行は常に黒字にする
			udColor = RGB( 0, 0, 0 );			// 黒
			//	データ位置取得
			nPos[0] = nDataPos;
			// 属性を取得
			pDiag->GetAttribute(nPos[0], ( LPUNKNOWN )&udAttr);
			// 文字色を設定
			memcpy(&udAttr.attr_fcolor, &udColor, sizeof(COLORREF));
			// 属性を変更
			pDiag->SetAttribute(nPos[0], ( LPUNKNOWN )&udAttr, TRUE);

			break;
// 修正No.157890 add <--

		default:
			break;
	}
}
// No.200601 add <--

//********************************************************************************
//	ICS-Controlの入力コントロールをComboBoxからEditBoxへ変更
//		IN		CICSDiag*		対象コントロール
//				int				対象コントロールのインデックス番号
//				BOOL			FLASE:ComboBox→EditBox
//								TRUE: EditBox →ComboBox
//		RET		なし
//	[補足事項]
//		当処理は、特殊行で小計等の名称を表示する場合に、科目などのComboBox欄へ
//		表示する場合に、レイアウトで作成でComnoBoxで作成されたコントロールを
//		EditBoxへ属性を変換する処理です。
//		ComboBox以外→EditBoxの場合、変更パラメータが違うため、別関数が必要
//********************************************************************************
void CfrmUc000Common::CmnDiagSetAttrComboToEdit(CICSDiag* pDiag, int intIndex, BOOL bRestore)
{
	DIAGRAM_ATTRIBUTE	diaatt;		// ICSDiag属性構造体

	// 対象セルの現属性を取得
	pDiag->GetAttribute( intIndex , (LPUNKNOWN)&diaatt );

	if (bRestore == FALSE) {
		// 変換：ComboBox→EditBox
		diaatt.attr_inpkind = 0;			// 0:エディトボックス
		diaatt.attr_editkind = 0;			// 0:ANK1
		diaatt.attr_inpmax = 0;				// 入力文字数=0
	}
	else {
		// 戻す：EditBox→ComboBox
		diaatt.attr_inpkind = m_uInfo.uComboAttr.attr_inpkind;		// 入力種別
		diaatt.attr_editkind = m_uInfo.uComboAttr.attr_editkind;	// エディト種別
		diaatt.attr_inpmax = m_uInfo.uComboAttr.attr_inpmax;		// 入力文字数
	}

	// 属性変更
	pDiag->SetAttribute( intIndex , (LPUNKNOWN)&diaatt , TRUE );
}

//********************************************************************************
//	ICS-Controlのエディットボックスの種類を変更
//　現在⑩－２専用
//		IN		CICSDiag*		対象コントロール
//				int				対象コントロールのインデックス番号
//				BOOL			FALSE:日付→半/全角文字
//								TRUE: 半/全角文字→日付
//		RET		なし
//	[補足事項]
//		当処理は、特殊行で小計等の名称を表示する場合、入力欄を半/全角文字へ変換する処理です。
//********************************************************************************
void CfrmUc000Common::CmnDiagSetAttrChangeEdit(CICSDiag* pDiag, int intIndex, BOOL bRestore)
{
	DIAGRAM_ATTRIBUTE	diaatt;		// ICSDiag属性構造体

	// 対象セルの現属性を取得
	pDiag->GetAttribute( intIndex , (LPUNKNOWN)&diaatt );

	if (bRestore == FALSE) {
		// 変換：元の属性→半/全角文字
		diaatt.attr_editkind = 2;					// 2:全角文字
		diaatt.attr_editattr = DIAG_EATTR_ZENHAN;	// ANK有効(0x80)
		diaatt.attr_inpmax = 0;						// 入力文字数=0
		diaatt.attr_linemax = 1;					// 入力行数=1
	}
	else {
		// 戻す：半/全角文字→元の属性(uEditAttrに必ず保持させておく)
		// uEditAttrが前のページの1行目の属性を持ってきていたため、前ページ1行目に小計があると、日付入力欄が小計属性を持ってしまっていた。
		// 前のページからではなく、正しくデフォルトのデータ行属性を持つようにする。
		//diaatt = m_uInfo.uEditAttr;

		// ⑩－２源泉所得税預り金の内訳書のパラメータ
		// 他の様式で用いるならそのパラメータをセット
		diaatt.attr_editkind = 3;					// 3:日付
		diaatt.attr_editattr = 0;					// 属性なし
		diaatt.attr_inpmax = 4;						// 入力文字数=4
		diaatt.attr_linemax = 10;					// 入力行数=10
	}

	// 属性変更
	pDiag->SetAttribute( intIndex , (LPUNKNOWN)&diaatt , TRUE );
}

// インボイス登録番号追加対応_23/09/11 add -->
//********************************************************************************
//	ICS-Controlのエディットボックスの種類を変更
//　現在様式②専用（登録番号と小計入力の切り替え）
//		IN		CICSDiag*		対象コントロール
//				int				対象コントロールのインデックス番号
//				short			表示アラインメント	0:左寄せ、1:中央寄せ、2右寄せ、3:均等割
//				short			入力文字数
//				short			入力行数
//				short			エディット種別	0:ANK、1:数字、2:漢字、3:日付、4:金額
//				short			エディット属性	0x80:ANK有効 …
//		RET		なし
//	[補足事項]
//		当処理は、特殊行で小計等の名称を表示する場合、入力欄を半/全角文字へ変換する処理です。
//********************************************************************************
void CfrmUc000Common::CmnDiagSetAttrChangeEdit2(CICSDiag* pDiag, int pIndex, short pAlign, short pInpmax, short pLinemax, short pEditkind, short pEditattr)
{
	DIAGRAM_ATTRIBUTE	diaatt;		// ICSDiag属性構造体

	// 対象セルの現属性を取得
	pDiag->GetAttribute(pIndex, (LPUNKNOWN)&diaatt);

	// 変換
	diaatt.attr_align		= pAlign;				// 表示アラインメント
	diaatt.attr_inpmax		= pInpmax;				// 入力文字数
	diaatt.attr_linemax		= pLinemax;				// 入力行数
	diaatt.attr_editkind	= pEditkind;			// エディット種別
	diaatt.attr_editattr	= pEditattr;			// エディット属性

	// 属性変更
	pDiag->SetAttribute(pIndex, (LPUNKNOWN)&diaatt, TRUE);
}
// インボイス登録番号追加対応_23/09/11 add <--

// midori 190301 add -->
//**********************************************************************************
//	ICS-Controlの対象列に｢行毎の入力桁数｣を設定
//		IN		CICSDiag*		対象コントロール
//				int				列番号
//				int				最大入力桁数
//				int				入力行数
//				CString			行毎のMAX
//		RET		なし
//**********************************************************************************
void CfrmUc000Common::CmnDiagSetAttrEditFormat( CICSDiag* pDiag, int intCol, int intInpmax, int intLinemax, CString strEditfmt )
{
	DIAGRAM_ATTRIBUTE	diaatt;		// ICSDiag属性構造体
	int					ii;

	for(ii=CmnGetControlIndex(1,intCol); ii<m_uInfo.intT1StartIndex; ii+=m_uInfo.intColMax)	{
		// 対象セルの現属性を取得
		pDiag->GetAttribute(ii, (LPUNKNOWN)&diaatt);

		// 最大入力桁数
		diaatt.attr_inpmax		= intInpmax;
		// 入力行数
		diaatt.attr_linemax		= intLinemax;
		// 行毎のMAX
		diaatt.attr_editformat	= strEditfmt;

		// 対象セルに設定した属性を指定
		pDiag->SetAttribute(ii, (LPUNKNOWN)&diaatt, TRUE);
	}
}

//**********************************************************************************
//	ICS-Controlの対象列を｢入力不可｣「非表示」に設定
//		IN		CICSDiag*		対象コントロール
//				int				列番号
//				int				1:入力項目　0:その他の項目(線など)　2:計行
//		RET		なし
//**********************************************************************************
void CfrmUc000Common::CmnDiagSetAttrDisableHide( CICSDiag* pDiag, int intCol, int pSw )
{
	DIAGRAM_ATTRIBUTE	diaatt;		// ICSDiag属性構造体
	int					ii;

	if(pSw < 2)	{
		if(pSw == 0)	ii=CmnGetControlIndexEx(1,intCol);
		else			ii=CmnGetControlIndex(1,intCol);

		for(; ii<m_uInfo.intT1StartIndex; ii+=m_uInfo.intColMax)	{
			pDiag->ModifyItem(ii,DIAG_MDFY_READONLY);
			pDiag->ModifyItem(ii,DIAG_MDFY_TRANSPARENT);
		}
	}
	else	{
		pDiag->ModifyItem(intCol,DIAG_MDFY_READONLY);
		pDiag->ModifyItem(intCol,DIAG_MDFY_TRANSPARENT);
	}
}
// midori 190301 add <--

//********************************************************************************
//	前頁/次頁ボタンの有効/無効判断
//		IN		BOOL*			前頁ボタン（TRUE:有効，FALSE:無効）
//				BOOL*			次頁ボタン（TRUE:有効，FALSE:無効）
//		RET		なし
//********************************************************************************
void CfrmUc000Common::CmnCheckEnableMoveButton(BOOL* bBack, BOOL* bNext)
{
	// 1頁しか存在しない場合
	if( m_uInfo.intCurPage <= 1 && m_uInfo.intMaxPage <= 1 ){
		*bBack = FALSE;				// 前頁ボタン無効
		*bNext = FALSE;				// 次頁ボタン無効
	}
	// 現在の頁が最大頁の場合
	else if( m_uInfo.intCurPage == m_uInfo.intMaxPage ){
		*bBack = TRUE;				// 前頁ボタン有効
		*bNext = FALSE;				// 次頁ボタン無効
	}
	// 現在の頁が1頁かつ、最大頁ではない場合の場合
	else if( m_uInfo.intCurPage == 1 && m_uInfo.intCurPage < m_uInfo.intMaxPage ){
		*bBack = FALSE;				// 前頁ボタン無効
		*bNext = TRUE;				// 次頁ボタン有効
	}
	// それ以外
	else{
		*bBack = TRUE;				// 前頁ボタン有効
		*bNext = TRUE;				// 次頁ボタン有効
	}
}

// midori 160612 cor -->
////********************************************************************************
////	ボタンの有効/無効切り替え
////		IN		int			呼び出し元フラグ（1:EditON，0:EditOFF）
////		RET		なし
////
////	【注意事項】
////		当処理は、改頁，頁挿入，行コピー，行貼付，行挿入貼付のボタン制御を行う
////		ICS-Diagの"EditON"と"EditOFF"イベントで呼び出します
////		1215追記：行削除、行挿入機能のボタン移行に伴いこれらの制御も行う
////********************************************************************************
// ---------------------
//********************************************************************************
//	タイトル名称、ボタンの有効/無効切り替え
//		IN		int			呼び出し元フラグ（2:オプション設定又は業務起動時 1:EditON，0:EditOFF）
//		RET		なし
//
//	【注意事項】
//		当処理は、改頁，頁挿入，行コピー，行貼付，行挿入貼付のボタン制御を行う
//		ICS-Diagの"EditON"と"EditOFF"イベントで呼び出します
//		1215追記：行削除、行挿入機能のボタン移行に伴いこれらの制御も行う
//********************************************************************************
// midori 160612 cor <--
void CfrmUc000Common::CmnChangeButtonEnable(int intEventFlag)
{
	BOOL				bPageNew		= FALSE;	// 改頁ボタン
	BOOL				bPageInsert		= FALSE;	// 頁挿入ボタン
	BOOL				bRowCopy		= FALSE;	// 行コピーボタン
	BOOL				bRowPaste		= FALSE;	// 行貼付ボタン
	BOOL				bRowInsert		= FALSE;	// 行挿入貼付ボタン
// midori 160612 add -->
	BOOL				bTitle			= FALSE;	// タイトルダイアログ
	COLORREF			color			= 0;
	DIAGRAM_ATTRIBUTE	diaatt;						// ICSDiag属性構造体
// midori 160612 add <--
	CURRENT_INFO		uCurInfo;

// midori 160612 cor -->
//	// ⑭-2は対象外とする
//	if (m_uInfo.intFormSeq == ID_FORMNO_142) {
//		// 何もしないで抜ける
//		return;
//	}
// ---------------------
	// 「入力データを確定する」にチェックがある場合、無効にする
	if(m_DataKakutei)	{
		bTitle = FALSE;
		color = BC_DISABLE;
	}
	else	{
		bTitle = TRUE;
		color	= (COLORREF)RGB(0xff,0xff,0xff);	//	白色
	}
	// タイトル名称の有効/無効切り替え
	// 属性を取得
	m_pTitleDiag->GetAttribute(0, (LPUNKNOWN)&diaatt);
	// 背景色を設定
	memcpy(&diaatt.attr_bcolor, &color, sizeof(COLORREF));
	// 属性を変更
	m_pTitleDiag->SetAttribute(0, (LPUNKNOWN)&diaatt, TRUE);	
	DlgSet_Attr(m_pTitleDiag,0,bTitle);

	// ⑭-2はタイトルのみ対象とする
	if (m_uInfo.intFormSeq == ID_FORMNO_142) {
		// 何もしないで抜ける
		return;
	}

	// ボタンの有効/無効切り替え
// midori 160612 cor <--

	// "EditOFF"の場合、対象ボタンをクリックして当処理を実行している場合がある
	if (intEventFlag == 0) {
// 要通知案件 修正No.158229 del -->
		//int intID = CmnGetDlgCtrlID();
		//switch (intID) {			
		//	case IDC_PAGENEW_BUTTON1:
		//	case IDC_PAGEINSERT_BUTTON1:
		//	case IDC_ROWCOPY_BUTTON1:
		//	case IDC_ROWPASTE_BUTTON1:
		//	case IDC_ROWINSERT_BUTTON1:
		//		// クリックにて"EditOFF"が実行されたと判断して何もしないで抜ける
		//		return;
		//}
// 要通知案件 修正No.158229 del <--
// 要通知案件 修正No.158229 add -->
		int intID;
		if(m_SaveBtnID != 0)	{
			intID = m_SaveBtnID;
			m_SaveBtnID = 0;
		}
		else	{
			intID = CmnGetDlgCtrlID();
		}
		switch (intID)	{
			case IDC_PAGENEW_BUTTON1:		// 改頁ボタン
			case IDC_PAGEINSERT_BUTTON1:	// 頁挿入ボタン
			case IDC_ROWCOPY_BUTTON1:		// 行コピーボタン
			case IDC_ROWPASTE_BUTTON1:		// 行貼付ボタン
			case IDC_ROWINSERT_BUTTON1:		// 行挿入貼付ボタン
				// クリックにて"EditOFF"が実行されたと判断して何もしないで抜ける
				return;
		}
// 要通知案件 修正No.158229 add <--
	}

	// 帳表のカレント情報を取得
	virGetCurrentInfo(&uCurInfo);

	// 【改頁ボタン，頁挿入ボタン】
	//		データ入力欄にフォーカスがある事＋合計行①，②は操作不可
	if (m_uInfo.intCurFocus == 1) {
		if (m_uInfo.intCurCtlIndex < m_uInfo.intT1StartIndex) {
			// 操作可能
			bPageNew		= TRUE;
			bPageInsert		= TRUE;
		}
	}

	// 【行コピーボタン】
	//		データ入力欄にフォーカスがある事＋合計行①，②は操作不可＋空行/データ行のみ使用可能
	if (m_uInfo.intCurFocus == 1) {
		if (m_uInfo.intCurCtlIndex < m_uInfo.intT1StartIndex) {
			if ((uCurInfo.intFgFunc == ID_FGFUNC_NULL) || (uCurInfo.intFgFunc == ID_FGFUNC_DATA)) {
				// 操作可能
				bRowCopy = TRUE;
			}
		}
	}

	// 【行貼付ボタン】
	//		行コピーボタンが押せる＋コピー済みである
	if ((bRowCopy == TRUE) && (uCurInfo.intCopyFlag == 1)) {
		// 操作可能
		bRowPaste = TRUE;
	}

	// 【行挿入貼付ボタン】
	//		データ入力欄にフォーカスがある事 + コピー済みである + 合計行①, ②は操作不可
	if ((m_uInfo.intCurFocus == 1) && ( uCurInfo.intCopyFlag == 1 )){
		if (m_uInfo.intCurCtlIndex < m_uInfo.intT1StartIndex) {
			bRowInsert = TRUE;
		}
	}

// midori 160612 add -->
	// 「入力データを確定する」にチェックがある場合、無効にする
	if(m_DataKakutei)	{
		bPageNew = FALSE;
		bPageInsert = FALSE;
		bRowCopy = FALSE;
		bRowPaste = FALSE;
		bRowInsert = FALSE;
	}
// midori 160612 add <--
	GetDlgItem(IDC_PAGENEW_BUTTON1)->EnableWindow(bPageNew);				// 改頁ボタン
	GetDlgItem(IDC_PAGEINSERT_BUTTON1)->EnableWindow(bPageInsert);			// 頁挿入ボタン
	GetDlgItem(IDC_ROWCOPY_BUTTON1)->EnableWindow(bRowCopy);				// 行コピーボタン
	GetDlgItem(IDC_ROWPASTE_BUTTON1)->EnableWindow(bRowPaste);				// 行貼付ボタン
	GetDlgItem(IDC_ROWINSERT_BUTTON1)->EnableWindow(bRowInsert);			// 行挿入貼付ボタン
}

//********************************************************************************
// 2012/7/20：タイトル編集は全ての様式において可能とする
//	タイトル編集の操作可能/操作不可を判断
//		IN		なし
//		RET		BOOL			TRUE:操作可能，FALSE:操作不可
//********************************************************************************
BOOL CfrmUc000Common::CmnCheckEnableTitleEdit()
{
	BOOL		bRet = TRUE;		// Default=操作可能

	switch (m_uInfo.intFormSeq) {
	case ID_FORMNO_141:			//	⑭-1
	case ID_FORMNO_151:			//	⑮-1
	case ID_FORMNO_161:			//	⑯-1
		// ⑭～⑯：個別選択時は操作不可
		if (m_uInfo.intFormType == ID_FORMTYPE_KOBETU) {
			bRet = FALSE;
		}
		break;

	// ⑭－２、⑮－２、⑯－２でも操作可能に変更 2012/7
	case ID_FORMNO_142:			//	⑭-2
	case ID_FORMNO_152:			//	⑮-2
	case ID_FORMNO_162:			//	⑯-2
		// ⑭～⑯：個別選択時は操作不可
		//		   通常選択時は"??-2"帳表はどこにも反映できないので操作不可
		bRet = FALSE;
		break;

	//case ID_FORMNO_153:			// ⑮-3は常に操作可能のため対象外
	}
	return bRet;
}

//********************************************************************************
//	行コピー，行貼付，行貼付挿入の事前チェック
//		IN		int				ボタン種別（0:行コピー，1:行貼付，2:行貼付挿入）
//		RET		BOOL			TRUE:正常，FALSE:エラー（実行不可）
//********************************************************************************
BOOL CfrmUc000Common::CmnCheckRowButtonProc(int intButtonType)
{
	CURRENT_INFO		uCurInfo;			// 各帳表のカレント情報

	// 帳表のカレント情報を取得
	virGetCurrentInfo(&uCurInfo);

	//----------------------------------------------------------------
	//	下記以外にもチェックが必要？ → 検討必要
	//	　・CICSDiag にフォーカスがない場合は？
	//	　・空行もコピーを許可する？
	//----------------------------------------------------------------
	switch (intButtonType) {
	case	0:
		// コピー元データをチェック
		switch (uCurInfo.intFgFunc) {
		// 空行
		case ID_FGFUNC_NULL:
		// データ行
		case ID_FGFUNC_DATA:
			break;
		// それ以外の行
		default:
			ICSMessageBox( _T("コピーできません。"), MB_OK, 0, 0, this );
			return FALSE;
		}
		break;

	case	1:
		// コピー済みチェック
		if (uCurInfo.intCopyFlag == 0) {
			ICSMessageBox( _T("貼付できません。"), MB_OK, 0, 0, this );
			return FALSE;
		}
		// コピー先データをチェック
		switch (uCurInfo.intFgFunc) {
		// 空行 or データ行
		case ID_FGFUNC_NULL:
		case ID_FGFUNC_DATA:
			break;
		// それ以外の行
		default:
			ICSMessageBox( _T("貼付できません。"), MB_OK, 0, 0, this );
			return FALSE;
		}
		break;

	case	2:
		// コピー済みチェック
		if (uCurInfo.intCopyFlag == 0) {
			ICSMessageBox( _T("貼付できません。"), MB_OK, 0, 0, this );
			return FALSE;
		}
		// コピー先データをチェック
		switch (uCurInfo.intFgFunc) {
		// 頁計 or 累計行
		case ID_FGFUNC_PAGEKEI:
		case ID_FGFUNC_RUIKEI:
			ICSMessageBox( _T("貼付できません。"), MB_OK, 0, 0, this );
			return FALSE;
		}
		break;
	}
	return TRUE;
}

//********************************************************************************
//	金額値→倍長フィールド変換関数
//		IN		char*			倍長フィールド金額値格納用ポインタ
//				CString			金額値文字列
//		RET		なし
//********************************************************************************
void CfrmUc000Common::CmnChangeFieldValData(char* chrVal, CString strVal)
{
	int		intDefn;				// 倍長フィールド保存用
	char	chrValData[32];

	// 現在の倍長フィールド取得
	intDefn = l_retn();
	// 倍長フィールド指定
	l_defn( 0x16 );

	// メモリクリア
	memset( chrVal, '\0', sizeof(char[6]) );

	// 文字列格納
	//strcpy( &chrValData[0] , strVal );
	strcpy_s( &chrValData[0] , sizeof(chrValData), strVal );
	
	// 金額値を倍長フィールドに変換
	l_input( &chrVal[0] , &chrValData[0]);

	// 倍長フィールドを元に戻す
	l_defn( intDefn );
}

//********************************************************************************
//	カンマ省略金額値取得関数　※カンマ付の金額値ではデータベースに保存できないため
//	【引数】	strVal	…	カンマ省略金額値格納用変数のポインタ
//				chrVal	…	倍長フィールド型金額値	
//	【戻り値】	なし
//********************************************************************************
void CfrmUc000Common::CmnGetNotCommaValData(CString* strVal, char* chrVal)
{
	char	chrData[32];				// 変換金額値格納用
	char*	hfmt = "ssssssssssssss9";	// 金額値書式フォーマット･･･現在テーブルは最大14桁
	int		intDefn;					// 倍長フィールド保存用

	// 現在の倍長フィールド取得
	intDefn = l_retn();
	// 倍長フィールド指定
	l_defn( 0x16 );

	// 倍長型金額値をカンマ抜き金額値に変換
	l_print( &chrData[0] , chrVal ,hfmt);
	
	// CString型変数に金額値を格納
	*strVal = chrData;

	// 倍長フィールドを元に戻す
	l_defn( intDefn );
}

//********************************************************************************
//	金額を加算/減算して合計値を求める
//	【引数】	CString *inTotalVal	…	合計金額値格納用変数のポインタ
//				CString inAddVal	…	加算金額値格納用変数のポインタ
//				int		inAddFlag	…	1:加算（省略可能），-1:減算
//	【戻り値】	なし
//********************************************************************************
void CfrmUc000Common::CmnKingakuAddFunc(CString *inTotalVal, CString inAddVal, int inAddFlag)
{
	char	prmTotalVal[32];		// 引数の合計金額格納用（CString型をchar型で格納）
	char	prmAddVal[32];			// 引数の加算金額格納用（CString型をchar型で格納）
	char	tmpTotalVal[32];		// 計算後の合計金額格納用（char型で格納）
	char	btAddVal[32];			// 計算後の合計金額格納用（倍長フィールド）
	char	buffAnser[32];			// 計算後の合計金額格を金額フォーマットで格納（char型で格納）

	//--------------------------------------------------------------
	// 初期化
	//--------------------------------------------------------------
	ZeroMemory(prmTotalVal, sizeof(prmTotalVal));
	ZeroMemory(prmAddVal, sizeof(prmAddVal));
	ZeroMemory(tmpTotalVal, sizeof(tmpTotalVal));
	ZeroMemory(btAddVal, sizeof(btAddVal));
	ZeroMemory(buffAnser, sizeof(buffAnser));
	
	//--------------------------------------------------------------
	// CString -> char* 変換
	//--------------------------------------------------------------
	lstrcpy(prmTotalVal, *inTotalVal);
	lstrcpy(prmAddVal, inAddVal);

	//--------------------------------------------------------------
	// 加算/減算
	//--------------------------------------------------------------
	__int64	i64;

	// char型を__int64に変換して加算/減算
	if (inAddFlag == 1) {
		i64 = _strtoi64(prmTotalVal, NULL, 10) + _strtoi64(prmAddVal, NULL, 10);
	}
	else {
		i64 = _strtoi64(prmTotalVal, NULL, 10) - _strtoi64(prmAddVal, NULL, 10);
	}

	// 上限、下限チェック
	if (i64 < DB_DECIMAL_MIN) {
		i64 = DB_DECIMAL_MIN;
	} else if (DB_DECIMAL_MAX < i64) {
		i64 = DB_DECIMAL_MAX;
	}

	// 合計値をchar型へ変換
	_i64toa_s(i64, tmpTotalVal, 32, 10);

	// CString型変数にchar型の合計金額値を格納
	*inTotalVal = tmpTotalVal;

	// 以下の処理は必要なし
	/*
	//--------------------------------------------------------------
	//	合計金額値を金額フォーマットへ変換
	//--------------------------------------------------------------
	int		intDefn;				// 倍長フィールド保存用

	// 現在の倍長フィールド取得
	intDefn = l_retn();

	// 倍長フィールド指定
	l_defn( 0x16 );

	// 倍長フィールドへ変換
	l_input(btAddVal, tmpTotalVal);

	// 合計金額値を金額フォーマットへ変換
	l_print(buffAnser, btAddVal, "SSSSSSSSSSSSS9");
	
	// CString型変数にchar型の合計金額値を格納
	*inTotalVal = buffAnser;

	// 倍長フィールドを元に戻す
	l_defn( intDefn );
	*/
}

// 計欄の上限値／下限値チェックをしていなかった時のメソッド
/*
void CfrmUc000Common::CmnKingakuAddFunc(CString *inTotalVal, CString inAddVal)
{
	char	tmpTotalVal[32];		// 合計金額格納用
	char	tmpAddVal[32];			// 加算金額格納用
	char	btTotalVal[32];			// 合計金額格納用
	char	btAddVal[32];			// 加算金額格納用
	char	buffAnser[32];			// フォーマット後の文字列格納用
	int		intDefn;				// 倍長フィールド保存用

	// 初期化
	ZeroMemory(tmpTotalVal, sizeof(tmpTotalVal));
	ZeroMemory(tmpAddVal, sizeof(tmpAddVal));
	ZeroMemory(btTotalVal, sizeof(btTotalVal));
	ZeroMemory(btAddVal, sizeof(btAddVal));
	ZeroMemory(buffAnser, sizeof(buffAnser));
	
	// CString -> char* 変換
	lstrcpy(tmpTotalVal, *inTotalVal);
	lstrcpy(tmpAddVal, inAddVal);

	// 現在の倍長フィールド取得
	intDefn = l_retn();
	// 倍長フィールド指定
	l_defn( 0x16 );

	// 計算用変数に格納
	l_input(btTotalVal, tmpTotalVal);
	l_input(btAddVal, tmpAddVal);

	// 加算
	l_add(btTotalVal, btTotalVal, btAddVal);

	// フォーマット指定
	l_print(buffAnser, btTotalVal, "SSSSSSSSSSSSS9");
	
	// CString型変数に金額値を格納
	*inTotalVal = buffAnser;

	// 倍長フィールドを元に戻す
	l_defn( intDefn );
}
*/

// 158096 del -->
////********************************************************************************
////	金額を比較する
////		IN		CString			比較対象金額１（a）
////				CString			比較対象金額２（b）
////		RET		int				比較結果（1以上：a>b，0：a=b，-1以下：a<b）
////********************************************************************************
//int CfrmUc000Common::CmnKingakuCmpFunc(CString strKingaku1, CString strKingaku2)
// 158096 del <--
// 158096 add -->
//********************************************************************************
//	金額を比較する
//		IN		CString			比較対象金額１（a）
//				CString			比較対象金額２（b）
//				CString			明細金額
//		RET		int				比較結果（1以上：a>b，0：a=b，-1以下：a<b）
//********************************************************************************
int CfrmUc000Common::CmnKingakuCmpFunc( CString strKingaku1, CString strKingaku2, CString strKingaku3 )
// 158096 add <--
{
	char		prmKingaku1[32];
	char		prmKingaku2[32];
	__int64		i64Kingaku1;
	__int64		i64Kingaku2;
	int			intRet;

	// Char型をクリア
	ZeroMemory(prmKingaku1, sizeof(prmKingaku1));
	ZeroMemory(prmKingaku2, sizeof(prmKingaku2));

	// CString型 → Char型
	lstrcpy(prmKingaku1, strKingaku1);
	lstrcpy(prmKingaku2, strKingaku2);

	// Char型 → __int64型
	i64Kingaku1 = _strtoi64(prmKingaku1, NULL, 10);
	i64Kingaku2 = _strtoi64(prmKingaku2, NULL, 10);

	// 一括金額に０円を含めるかチェック
	if(m_IkkatuOutZero){	// 一括金額に０円を含めない
		// 比較対象金額１が０円の場合、一括対象としない
// 158096 del -->
		//if( i64Kingaku1 == 0 ){
// 158096 del <--
// 158096 add -->
		char		prmKingaku3[32]={0};
		__int64		i64Kingaku3;
		lstrcpy(prmKingaku3,strKingaku3);
		i64Kingaku3 = _strtoi64(prmKingaku3,NULL,10);
		if(i64Kingaku3 == 0)	{
// 158096 add <--
			return 0;
		}
	}

	// __int64型で大小を比較
	if (i64Kingaku1 == i64Kingaku2) {
		intRet = 0;
	}
	else if (i64Kingaku1 > i64Kingaku2) {
		intRet = 1;
	}
	else {
		intRet = -1;
	}
	return intRet;

// テーブルの上限/下限対応にて"Arith"は使用しないように修正
// 使用箇所は、自動一括集計の閾値との判断のみであるため、現行仕様でも動作は可能。
// ただし、ここだけ"Arith"を使うのもおかしいので、"int64"の処理へ修正。
//		→ 現仕様：テーブル最大14桁，一括閾値最大12桁。
/*******
	char	tmpVal1[32];		// 金額１格納用
	char	tmpVal2[32];		// 金額２格納用
	char	btVal1[32];			// 
	char	btVal2[32];			// 
	int		intDefn;			// 倍長フィールド保存用
	int		intRet;

	// 初期化
	ZeroMemory(tmpVal1, sizeof(tmpVal1));
	ZeroMemory(tmpVal2, sizeof(tmpVal2));
	ZeroMemory(btVal1, sizeof(btVal1));
	ZeroMemory(btVal2, sizeof(btVal2));

	// CString -> char* 変換
	lstrcpy(tmpVal1, strKingaku1);
	lstrcpy(tmpVal2, strKingaku2);

	// 現在の倍長フィールド取得
	intDefn = l_retn();
	// 倍長フィールド指定
	l_defn( 0x16 );

	// 計算用変数に格納
	l_input(btVal1, tmpVal1);
	l_input(btVal2, tmpVal2);

	// 比較
	intRet = l_cmp(btVal1, btVal2);

	// 倍長フィールドを元に戻す
	l_defn( intDefn );

	return intRet;
*******/
}

//********************************************************************************
//	対象のインデックスが最終行インデックスか判断
//	【引数】	index		…	対象のインデックス
//				shtLast		…	最終インデックス
//	【戻値】	TRUE		…	最終インデックスである
//				FALSE		…	最終インデックスではない
//********************************************************************************
BOOL CfrmUc000Common::CmnGetLastRowIndex(int index, int shtLast)
{
	// 最終インデックスか?
	if ( index == shtLast ){
			return ( TRUE );
	}

	return ( FALSE );
}


//*******************************************************************************
// 「計」行が2段表示か判断（「頁計・累計」&最終頁時）
//【引数】	uInfo	…	テーブル情報構造体
//【戻値】	TRUE	…	「計」が2段表示
//			FALSE	…	「計」が1段表示
//*******************************************************************************
BOOL CfrmUc000Common::CmnGetTotalRowType()
{	
	BOOL	blnRet;		// 戻り値

	// 初期化
	blnRet = FALSE;

	// 頁計・累計?
	if ( m_uInfo.intOutKei == ID_OUTKEI_BOTH ){
		// 表示頁が最大頁?かつ最大頁数が1ではない場合
		if ( ( m_uInfo.intCurPage == m_uInfo.intMaxPage ) && ( m_uInfo.intMaxPage != 1 ) ){
			// 戻り値格納
			blnRet = TRUE;
		}
	}

	return blnRet;
}

//****************************************************************************
//	頁＋行から次の行を取得する
//		IN:		int		頁番号 → 新しい頁番号へ更新
//				int		頁番号 → 新しい行番号へ更新
//		RET:	int		実行結果（FUNCTION_OK, FUNCTION_NG）
//****************************************************************************
int CfrmUc000Common::CmnGetNextPageRow(int* pPage, int* pRow)
{
	// 次の行は改行が必要？
	if (*pRow + 1 > m_uInfo.intRowMaxData) {
		*pPage = *pPage + 1;
		*pRow = 1;
	}
	else {
		*pRow = *pRow + 1;
	}
	
	return	FUNCTION_OK;
}

//****************************************************************************
//	頁＋行から次の行が次頁の先頭行か判断する
//		IN:		int		頁番号
//		RET:	BOOL	TRUE ･･･次の行は先頭頁
//						FALSE･･･次の行は先頭頁以外
//****************************************************************************
BOOL CfrmUc000Common::CmnGetNextFirstRow(int intRow)
{
	// 次の行は先頭行？
	if (intRow + 1 > m_uInfo.intRowMaxData) {
		return	TRUE;
	}
	
	return	FALSE;
}

//****************************************************************************
//	現在フォーカスを持つコントロールのIDを取得
//		IN:		なし
//		RET:	int				フォーカスを持つコントロールのID
//****************************************************************************
int CfrmUc000Common::CmnGetDlgCtrlID()
{
	// フォーカスを持つウィンドウを取得
	CWnd	*pWnd = GetFocus();

	// フォーカスを持つコントロールのIDを取得
	if (pWnd == NULL) {
		return 0;
	}
	return pWnd->GetDlgCtrlID();
}

//****************************************************************************
//	小計/中計の括弧付き名称を取得
//		IN:		CString			小計名称
//				CString			括弧（左）
//				CString			括弧（右）
//				CString			科目名（特例で科目名付きの場合あり）
//		RET:	CString			小計/中計の括弧付き名称
//****************************************************************************
CString	CfrmUc000Common::CmnGetSykeiNameWithKakko(CString strSykeiName, CString strLeft, CString strRight, CString strKamoku)
{
	CString		strTemp = strSykeiName;
	CString		strRet;

	// 特例の科目名付きの場合は、"小計（科目名）"とする
	if (strKamoku != "") {
// midori 191002 del -->
		//strTemp = strTemp + _T("（") + strKamoku + _T("）");
// midori 191002 del <--
// midori 191002 add -->
		strTemp = strTemp + _T("(") + strKamoku + _T(")");
// midori 191002 add <--
	}

	// 小計/中計名称の作成 ･･･ 指定カッコで括る
	if ((strLeft == "") || (strRight == "")) {
		strRet = strTemp;
	}
	else {
		strRet = strLeft + strTemp + strRight;
	}
	return strRet;
}

CString	CfrmUc000Common::CmnGetSykeiNameWithKakko(CString strSykeiName, CString strLeft, CString strRight)
{
	return CmnGetSykeiNameWithKakko(strSykeiName, strLeft, strRight, "");
}

//****************************************************************************
//	アクティブな帳表ウィンドウを閉じる
//		IN:		なし
//		RET:	なし
//****************************************************************************
void CfrmUc000Common::CmnCloseActiveDocument()
{
	CdbUcInfMain			mfcRec( m_pDB );	//	uc_inf_mainテーブル
	CdbUcInfSub				mfcRecSub( m_pDB );	//	uc_inf_subテーブル
	int						nNowFormSeq = 0;
	int						nNowFormOrder = 0;

	try{
		//	初期化成功？
		if ( mfcRec.Init() == DB_ERR_OK ){
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				mfcRec.MoveFirst();				//	レコードを先頭に移動
				nNowFormSeq = mfcRec.m_FormSeq;	//	現在選択されている様式のシーケンス番号取得
			}
			//	レコード閉じる
			mfcRec.Fin();
		}

		// FormOrder番号を取得
		if ( mfcRecSub.RequeryFormSeq( nNowFormSeq ) == DB_ERR_OK ) {
			//	レコードあり？
			if ( !( mfcRecSub.IsEOF() ) ){
				mfcRecSub.MoveFirst();					//	レコードを先頭に移動			
				nNowFormOrder = mfcRecSub.m_FormOrder;	//	現在選択されている様式のオーダー番号取得
			}
			//	レコード閉じる
			mfcRecSub.Fin();
		}

//		// 表示中のウィンドウを閉じる
//		((CUCHIWAKEApp*)AfxGetApp())->CloseDocument( nNowFormOrder );
//		((CUCHIWAKEApp*)AfxGetApp())->CloseDocument();
	}
	catch(...){

		//	レコード開いてる？
		if ( mfcRec.IsOpen() ){
			//	レコード閉じる
			mfcRec.Fin();
		}
	}

}

//****************************************************************************
//	「共通設定」アプリ情報から背景色を取得
//		IN:		なし
//		RET:	COLORREF	使用する色
//****************************************************************************
COLORREF CfrmUc000Common::CmnGetBackColor()
{
	ICSColorInfo icsColInfo;	// 色情報クラス
	COLORREF	 retColor;		// 返送値の色

	// デフォルトは「ボタンの表面」色としておく
	retColor = GetSysColor( COLOR_BTNFACE );

	// 色設定有効?
	if( ((ICSWinApp *)AfxGetApp())->GetStanderdColor( icsColInfo ) == 0 ){
		// 色の使用がONなら設定中の標準色を取得
		if ( icsColInfo.m_swOnOff == ID_ICS_STANDARDCOLOR_ON ){
			retColor = icsColInfo.ViewColor();
		}
	}

	return retColor;

}

//------------------------	テーブル操作　------------------------

//********************************************************************************
//	帳表の初期処理：テーブルのレコード無しなら１頁分レコード作成
//		IN		なし
//		RET		なし
//********************************************************************************
void CfrmUc000Common::CmnTblInitialMakeData()
{
	// 最大頁が０の場合、頁挿入ボタンを実行して１頁分レコードを作成
	if (CmnTblGetPageMax() <= 0) {
		OnPageInsert();
	}
}

//********************************************************************************
//	帳表の空行チェック処理：テーブルが全て空行データかどうか判断
//		IN		なし
//		RET		TRUE	…　空行しかない and 「はい」選択 
//				FALSE	…　空行以外あり or  「いいえ」選択
//********************************************************************************
BOOL CfrmUc000Common::CmnTblCheckDeleteData()
{
// midori 160608 del -->
//	CString		strMsg;
// midori 160608 del <--
	BOOL		blnRet = FALSE;

	// 帳表毎のテーブルクラスを"New"する
	CdbUc000Common*		rsData;
	rsData = virGetDataTableObject();


	// 空行データの判断(一時保管データが存在する場合は削除確認メッセージを表示しない　2012/7/20)
	if (rsData->GetCountVisualDataRecord() == 0) {
		// 確認メッセージ表示
// midori 160608 cor -->
		//strMsg.Format("%s.%sは、全て空行です。", m_uInfo.strTitleNumber, m_uInfo.strTitleName);
		//strMsg += "\n空ページを削除しますか？";
		//strMsg += "\n    [は　い]：ページを削除";
		//strMsg += "\n    [いいえ]：ページを残す";
		//if ( ICSMessageBox( strMsg, MB_YESNO, 0, 0, this ) == IDYES ){
		//	blnRet = TRUE;
		//}
// ---------------------
		blnRet = TRUE;
// midori 160608 cor <--
	}

	rsData->Fin();
	delete rsData;

	return blnRet;
}

//********************************************************************************
//	帳表の終了処理：帳表の全レコード削除
//		IN		なし
//		RET		なし
//********************************************************************************
void CfrmUc000Common::CmnTblEndDeleteData()
{
	CString		strMsg;

	// 帳表毎のテーブルクラスを"New"する
	CdbUc000Common*		rsData;
	rsData = virGetDataTableObject();
	CString strTable = rsData->GetDefaultSQL();

// midori 154391 del -->
	//CString strTanaoroshi = "[dbo].";
	//		strTanaoroshi += STR_TBL_NAME_051;

	//CString strKaikakekin = "[dbo].";
	//		strKaikakekin += STR_TBL_NAME_091;

	////********************************************
	////削除対象様式に欄外登録がある場合、削除する
	////********************************************

	//// 様式⑤：棚卸資産
	//if( strTanaoroshi.Compare( strTable ) == 0 ){
	//	CdbUc052Tanaoroshi2		mfcRec( m_pDB );
	//	mfcRec.DeleteAllRecord();

	//	mfcRec.Fin();
	//}

	//// 様式⑨：買掛金
	//if( strKaikakekin.Compare( strTable ) == 0 ){
	//	CdbUc092Kaikakekin2		mfcRec( m_pDB );
	//	mfcRec.DeleteAllRecord();

	//	mfcRec.Fin();
	//}
// midori 154391 del <--

	// テーブルの画面上のレコードを削除( 2012/07/20 )
	rsData->DeleteVisualRecord();

	rsData->Fin();
	delete rsData;
}


//*******************************************************************************
//	最大頁取得
//		IN		なし
//		RET		最大頁
//*******************************************************************************
int	CfrmUc000Common::CmnTblGetPageMax()
{
	CdbUc000Common*		rsData;
	int					intRet;

	rsData = virGetDataTableObject();

	// 最大頁取得
	intRet = rsData->GetNumPage();

	rsData->Fin();
	delete rsData;

	return intRet;
}

//*******************************************************************************
//	空行挿入･･･複数行可能
//		IN		int		挿入位置（頁）
//				int		挿入位置（行）
//				int		挿入行数
//		RET		int		実行結果（FUNCTION_OK, FUNCTION_NG）
//*******************************************************************************
int CfrmUc000Common::CmnTblInsertNullLine(int nPage, int nRow, int nLine)
{
	int					i = 0;
	int					nCountPage = nPage;
	int					nCountRow = nRow;

	// パラメータのチェック
	if (nPage <= 0 || nRow <= 0 || nLine <= 0) {
		return FUNCTION_NG;
	}

	// 帳表毎のテーブルクラスを"New"する
	CdbUc000Common*		rsData;
	rsData = virGetDataTableObject();

	// ｎ件分のレコード追加
	for (i = 0; i < nLine; i++){
		// 新規レコード追加
		rsData->CreateNewRecord();				// レコード初期化済み

		// 空行データとする
		rsData->Edit();
		rsData->m_NumPage = nCountPage;			// ページ番号
		rsData->m_NumRow = nCountRow;			// 行番号
		rsData->m_FgFunc = ID_FGFUNC_NULL;		// 特殊行フラグ
		rsData->m_FgShow = ID_FGSHOW_OFF;		// 表示フラグ

		// レコード更新
		rsData->Update();

		// 次の頁と行を取得
		CmnGetNextPageRow(&nCountPage, &nCountRow);
	}
	rsData->Fin();

	delete rsData;
	return FUNCTION_OK;
}

//*******************************************************************************
//	特殊行挿入･･･１行のみ可能
//		IN		int		挿入位置（頁）
//				int		挿入位置（行）
//				int		特殊行の種別
//				CString	特殊行の表示名称
//		RET		int		実行結果（FUNCTION_OK, FUNCTION_NG）
//*******************************************************************************
int CfrmUc000Common::CmnTblInsertSpecialLine(int nPage, int nRow, int nType, CString strName)
{
	BOOL		bIkkatuLine = FALSE;
// midori 190505 add -->
	BOOL		bKamokuLine = FALSE;
// midori 190505 add <--
	
	// パラメータのチェック
	if (nPage <= 0 || nRow <= 0) {
		return FUNCTION_NG;
	}
	switch ( nType ) {
	case ID_FGFUNC_SYOKEI:		// 小計行
	case ID_FGFUNC_SYOKEINULL:	// 小計行（空行）
	case ID_FGFUNC_CHUKEI:		// 中計行
	case ID_FGFUNC_CHUKEINULL:	// 中計行（空行）
	case ID_FGFUNC_RUIKEI:		// 累計行
	case ID_FGFUNC_PAGEKEI:		// 頁計行
		bIkkatuLine = FALSE;
// midori 190505 add -->
		bKamokuLine = FALSE;
// midori 190505 add <--
		break;
	case ID_FGFUNC_IKKATUMANUAL:// 一括集計金額行（手動）
	case ID_FGFUNC_IKKATUAUTO:	// 一括集計金額行（自動）
		bIkkatuLine = TRUE;
// midori 190505 add -->
		bKamokuLine = FALSE;
// midori 190505 add <--
		break;
// midori 190505 add -->
	case ID_FGFUNC_KAMOKU:		// 科目行
		bIkkatuLine = FALSE;
		bKamokuLine = TRUE;
		break;
// midori 190505 add <--
	default:
		return FUNCTION_NG;
	}

	// 帳表毎のテーブルクラスを"New"する
	CdbUc000Common*		rsData;
	rsData = virGetDataTableObject();

	// 新規レコード追加
	rsData->CreateNewRecord();				// レコード初期化済み

	// 特殊行データ編集（共通データ）
	rsData->Edit();
	rsData->m_NumPage = nPage;				// ページ番号
	rsData->m_NumRow = nRow;				// 行番号
	rsData->m_FgFunc = nType;				// 特殊行フラグ
	rsData->m_FgShow = ID_FGSHOW_OFF;		// 表示フラグ

	// 特殊行の名称
	if (bIkkatuLine == TRUE) {
		CALCKEI_INFO		uCalcKei;		// 実は未使用
		SORTKAMOKU_INFO		uKamoku;		// 実は未使用
		virTblEditIkkatuLine(2, rsData, strName, uCalcKei, uKamoku);
	}
// midori 190505 add -->
	else if(bKamokuLine == TRUE)	{
		// 科目行挿入
		virTblEditKamokuLine(rsData,nPage,nRow,strName);
	}
// midori 190505 add <--
	else {
		rsData->m_KeiStr = strName;
	}

	// レコード更新
	rsData->Update();
	rsData->Fin();

	delete rsData;
	return FUNCTION_OK;
}

/*------------------------------------------------------------
	<引数>
		なし
	<戻値>
		 0	:	成功
		-1	:	失敗
	<機能>
		頁計、累計行を挿入する。
------------------------------------------------------------*/
int CfrmUc000Common::CmnTblInsertPageKeiRuiKei(void)
{
	CdbUc000Common*		rs;							// 全レコード取得用
	int					nSetNumPage;				// 頁番号保存変数
	int					nSetNumRow;					// 行番号保存変数
	int					retVal = FUNCTION_NG;
	CString				strNameKei;

	retVal = FUNCTION_OK;

	// 計の名称セット
	if(m_NameKei)	strNameKei = NAME_GOUKEI;
	else			strNameKei = NAME_KEI;

	rs = virGetDataTableObject();

	// 頁番号の昇順、行番号の昇順でソートされたレコードセットを取得する。
	rs->RequeryFgShowSortPageRow(ID_FGSHOW_OFF);

	// 1頁目の1行目からチェックする。
	nSetNumPage = 1;
	nSetNumRow = 1;

	// Step1：先頭頁の先頭行から順に参照して、頁計/累計行を挿入する
	switch (m_uInfo.intOutKei) {
	case ID_OUTKEI_OFF:		// なし
	case ID_OUTKEI_PAGEKEI:	// 頁計
	case ID_OUTKEI_RUIKEI:	// 累計
	case ID_OUTKEI_BOTH:	// 累計と累計

		// 計行、頁計行、累計行を挿入していく。
		while (!rs->IsEOF()) {
			// 頁計を挿入する条件式
			if (nSetNumRow > m_uInfo.intRowMaxData) {
				if (m_uInfo.intRowKei == 1) {
					// 計あり帳表
					switch (m_uInfo.intOutKei) {
					case ID_OUTKEI_OFF:		// なし
					case ID_OUTKEI_RUIKEI:	// 累計のみを挿入する
						// 頁計なしの場合、"計"or"合計"文字をセット
						//CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, NAME_KEI);
						CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, strNameKei);
						break;

					case ID_OUTKEI_PAGEKEI:	// 頁計のみを挿入する
					case ID_OUTKEI_BOTH:	// 累計と累計の両方を挿入する
						// 頁計をセット
						CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, NAME_PAGEKEI);
						break;
					}
				}
				else {
					// 計なし帳表
					switch (m_uInfo.intOutKei) {
					case ID_OUTKEI_PAGEKEI:	// 頁計のみを挿入する
					case ID_OUTKEI_BOTH:	// 累計と累計の両方を挿入する
						// 頁計をセット
						CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, NAME_PAGEKEI);
						break;
					}
				}
				nSetNumPage++;		// 頁番号をインクリメント
				nSetNumRow = 1;		// 行番号を先頭に戻す
			}
			else {
				rs->MoveNext();
				nSetNumRow++;		// 行番号をインクリメント
			}
		}
		break;
	}

	// Step2：最終頁の空行挿入＋頁計/累計作成
	switch (m_uInfo.intOutKei) {
	case ID_OUTKEI_OFF:		// 計なし
		// 計あり帳表：最終行は合計行とする
		// 計なし帳表：最終行まで空行で埋める
		while (nSetNumRow <= m_uInfo.intRowMax) {
			if ((nSetNumRow == m_uInfo.intRowMax) && (m_uInfo.intRowKei == 1)) {
				// 最終行に合計行を挿入
				//CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, NAME_KEI);
				CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, strNameKei);
			}
			else {
				// 空行挿入
				CmnTblInsertNullLine(nSetNumPage, nSetNumRow, 1);
			}
			nSetNumRow++;		// 行番号をインクリメント
		}
		break;

	case ID_OUTKEI_PAGEKEI:	// 頁計
		// 計あり帳表，計なし帳表共に、最終行は合計行とする
		while (nSetNumRow <= m_uInfo.intRowMax) {
			if (nSetNumRow == m_uInfo.intRowMax) {
				// 最終行に合計行を挿入
				CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, NAME_PAGEKEI);
			}
			else {
				// 空行挿入
				CmnTblInsertNullLine(nSetNumPage, nSetNumRow, 1);
			}
			nSetNumRow++;		// 行番号をインクリメント
		}
		break;

	case ID_OUTKEI_RUIKEI:	// 累計
		// 計あり帳表：最終行は合計行とする
		// 計なし帳表：最終行は合計行とする。また最終行がデータ行なら次頁を空行で埋め最終行を累計とする
		if (m_uInfo.intRowKei == 1) {
			// 計あり帳表
			while (nSetNumRow <= m_uInfo.intRowMax) {
				if (nSetNumRow == m_uInfo.intRowMax) {
					// 最終行に合計行を挿入
					//CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_RUIKEI, NAME_RUIKEI);
					CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_RUIKEI, strNameKei);
				}
				else {
					// 空行挿入
					CmnTblInsertNullLine(nSetNumPage, nSetNumRow, 1);
				}
				nSetNumRow++;		// 行番号をインクリメント
			}
		}
		else {
			// 計なし帳表：最終データが空行なら削除する（最終行の条件を付けるべきかも）
			if (rs->IsBOF() == 0) {			// 空行削除では、前レコードが存在しない場合がある
				rs->MovePrev();
				if (!rs->IsEOF()) {
					if ((rs->m_FgFunc == 0) && (rs->m_FgShow == 0)) {
						rs->Delete();
						nSetNumRow--;
					}
				}
			}
			// すでに最終行がデータで埋まっている場合は次頁に累計行を入れる
			if (nSetNumRow > m_uInfo.intRowMax) {
				nSetNumPage++;			// 頁番号をインクリメント
				nSetNumRow = 1;			// 行番号をクリア
			}
			// あとは空行で埋め、最終行に合計行を挿入
			if (nSetNumRow <= m_uInfo.intRowMax) {
				while (nSetNumRow <= m_uInfo.intRowMax) {
					if (nSetNumRow == m_uInfo.intRowMax) {
						// 最終行に合計行を挿入
						//CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_RUIKEI, NAME_RUIKEI);
						CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_RUIKEI, strNameKei);
					}
					else {
						// 空行挿入
						CmnTblInsertNullLine(nSetNumPage, nSetNumRow, 1);
					}
					nSetNumRow++;		// 行番号をインクリメント
				}
			}
		}
		break;

	case ID_OUTKEI_BOTH:	// 頁計と累計
		// 計あり帳表，計なし帳表共に、データの最後を頁計と累計とする。
		//	１頁しかない場合は、累計行のみ追加する
		//	合計行を１行しか挿入できない場合、該当頁は頁計として、次頁に頁計と累計行とする
		if (rs->IsBOF() == 0) {			// 空行削除では、前レコードが存在しない場合がある
			rs->MovePrev();
			if (!rs->IsEOF()) {
				if ((rs->m_FgFunc == 0) && (rs->m_FgShow == 0)) {
					rs->Delete();
					nSetNumRow--;
				}
			}
		}
		// 既に最終行－１がデータで埋まっている場合
		if (nSetNumRow >= m_uInfo.intRowMax) {
			if (nSetNumPage == 1) {
				// 累計を挿入して終了
				//CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_RUIKEI, NAME_RUIKEI);
				CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_RUIKEI, strNameKei);
				nSetNumRow++;			// 行番号をインクリメント
			}
			else {
				// 頁計を挿入して次頁に頁計と累計を出す
				CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, NAME_PAGEKEI);
				nSetNumPage++;			// 頁番号をインクリメント
				nSetNumRow = 1;			// 行番号をクリア
			}
		}
		// あとは空行で埋め、最終行－１と最終行に合計行を挿入
		while (nSetNumRow <= m_uInfo.intRowMax) {
			if (nSetNumRow == m_uInfo.intRowMax) {
				// 最終行に累計行を挿入
				//CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_RUIKEI, NAME_RUIKEI);
				CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_RUIKEI, strNameKei);
			}
			else if ((nSetNumRow == m_uInfo.intRowMax - 1) && (nSetNumPage >= 2)) {
				// 最終行－１に頁計行を挿入
				CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, NAME_PAGEKEI);
			}
			else {
				// 空行挿入
				CmnTblInsertNullLine(nSetNumPage, nSetNumRow, 1);
			}
			nSetNumRow++;		// 行番号をインクリメント
		}
	}
	delete rs;

	// カレント頁の調整
	m_uInfo.intMaxPage = nSetNumPage;

	if (m_uInfo.intCurPage > m_uInfo.intMaxPage) {
		m_uInfo.intCurPage = nSetNumPage;
	}

	return retVal;	
}

/*------------------------------------------------------------
	<引数>
		inFgFuncKind	:	削除したい特殊行種別
	<戻値>
		 0	:	成功
		-1	:	失敗
	<機能>
		パラメータで指定された特殊行を削除する
------------------------------------------------------------*/
int CfrmUc000Common::CmnTblDeleteFgFunc(int inFgFuncKind)
{
	CdbUc000Common*		rsData;
	int					retVal = FUNCTION_NG;

	retVal = FUNCTION_OK;

	rsData = virGetDataTableObject();

	// 特殊行削除メソッド
	rsData->DeleteFgFunc(inFgFuncKind);

	rsData->Fin();
	delete rsData;

	return retVal;	
}

//********************************************************************************
//	削除ボタン：削除メイン処理
//		IN		int*			削除行数
//		RET		BOOL			TRUE:削除/一時保管実行，FALSE:キャンセル
//********************************************************************************
BOOL CfrmUc000Common::CmnTblDeleteMainProc( int* pnDelLine )
{
	CURRENT_INFO	uCurInfo;
	CString			strMsgTitle = "";
	int				intMsgRet;
	BOOL			bRet;
	int				intProc = 0;
	int				nRet;

	// 帳表のカレント情報を取得
	virGetCurrentInfo(&uCurInfo);

	// カレント行のチェック：ボタン操作不可
	switch (uCurInfo.intFgFunc) {
		case ID_FGFUNC_RUIKEI:			// 累計行
		case ID_FGFUNC_PAGEKEI:			// 頁計行
		case ID_FGFUNC_IKKATUAUTO:		// 一括集計金額行（自動）
			// 実行不可
			return FALSE;
	}

	// データ行/手動一括集計金額行のみ一時保管を実行可能
	if ((uCurInfo.intFgFunc == ID_FGFUNC_DATA) || (uCurInfo.intFgFunc == ID_FGFUNC_IKKATUMANUAL)) {
		// 確認メッセージのメッセージ編集
		strMsgTitle =	_T("削除してよろしいですか？\r\n");
		strMsgTitle +=	_T("\r\n");
		strMsgTitle +=	_T("[削除]　　　：データを削除します\r\n");
		strMsgTitle +=	_T("[保管]      ：保管場所にデータを移動します\r\n");	// 全ての"一時保管"文言を"保管"に変更
		strMsgTitle +=	_T("[キャンセル]：削除を実行しません");

		// 確認メッセージ
		CdlgMessageBox		dlgMessage( this );

		// カレント行にインバースをかける
// 修正No.157627 add -->
		CmnRowBeforeInverse(&uCurInfo);
// 修正No.157627 add <--
		CmnRowInverse( TRUE , &uCurInfo );

		intMsgRet = dlgMessage.ShowDialog(_T("削除"), strMsgTitle, _T("キャンセル"), _T("保管"), _T("削除"));

		// カレント行のインバースを解除
		CmnRowInverse( FALSE , &uCurInfo );
// 修正No.157627 add -->
		CmnRowAfterInverse(&uCurInfo);
// 修正No.157627 add <--

		switch (intMsgRet) {
		case ID_DLG_BUTTON1:			// キャンセル
			intProc = 0;
			break;
		case ID_DLG_BUTTON2:			// 一時保管
			intProc = 1;
			break;
		case ID_DLG_BUTTON3:			// 削除
			intProc = 2;
			break;
		}
	}
	//	空行のみ空行削除を実行可能
	else if (uCurInfo.intFgFunc == ID_FGFUNC_NULL){
		// 確認メッセージのメッセージ編集
		strMsgTitle =	_T("削除してよろしいですか？\r\n");
		strMsgTitle +=	_T("\r\n");
		strMsgTitle +=	_T("[削除]　　　：該当行を削除します\r\n");
		strMsgTitle +=	_T("[空行削除]　：該当行以降の空行を削除します\r\n");
		strMsgTitle +=	_T("[キャンセル]：削除を実行しません");

		// 確認メッセージ
		CdlgMessageBox		dlgMessage( this );

		// カレント行にインバースをかける
// 修正No.157627 add -->
		CmnRowBeforeInverse(&uCurInfo);
// 修正No.157627 add <--
		CmnRowInverse( TRUE , &uCurInfo );

		intMsgRet = dlgMessage.ShowDialog(_T("削除"), strMsgTitle, _T("キャンセル"), _T("空行削除"), _T("削除"));

		// カレント行のインバースを解除
		CmnRowInverse( FALSE , &uCurInfo );
// 修正No.157627 add -->
		CmnRowAfterInverse(&uCurInfo);
// 修正No.157627 add <--

		switch (intMsgRet) {
		case ID_DLG_BUTTON1:			// キャンセル
			intProc = 0;
			break;
		case ID_DLG_BUTTON2:			// 空行削除
			intProc = 3;
			break;
		case ID_DLG_BUTTON3:			// 削除
			intProc = 2;
			break;
		}
	}
	else {
		// 確認メッセージのメッセージ編集
		strMsgTitle =	_T("削除してよろしいですか？\r\n");
		strMsgTitle +=	_T("\r\n");
		strMsgTitle +=	_T("[削除]　　　：データを削除します\r\n");
		strMsgTitle +=	_T("[キャンセル]：削除を実行しません");

		// 確認メッセージ
		CdlgMessageBox		dlgMessage( this );

		// カレント行にインバースをかける
// 修正No.157627 add -->
		CmnRowBeforeInverse(&uCurInfo);
// 修正No.157627 add <--
		CmnRowInverse( TRUE , &uCurInfo );

		intMsgRet = dlgMessage.ShowDialog(_T("削除"), strMsgTitle, _T("キャンセル"), _T("削除"));

		// カレント行のインバースを解除
		CmnRowInverse( FALSE , &uCurInfo );
// 修正No.157627 add -->
		CmnRowAfterInverse(&uCurInfo);
// 修正No.157627 add <--

		switch (intMsgRet) {
		case ID_DLG_BUTTON1:			// キャンセル
			intProc = 0;
			break;
		case ID_DLG_BUTTON2:			// 削除
			intProc = 2;
			break;
		}
	}

// midori 156968 add -->
	if (intProc > 0) {
		// 最終行（頁の最大行）が小計の場合、小計の下に空行を追加する
		InsertNull();
	}
// midori 156968 add <--

	// 削除/一時保管/空行削除の実行
	switch (intProc) {
	case	0:							// キャンセル
		bRet = FALSE;
		break;
	case	1:							// 一時保管
		// 保管テーブルに追加
		CmnTblAddToHokanOneLine(m_uInfo.intFormSeq, m_uInfo.intCurPage, m_uInfo.intCurRow);
		// カレント行を一時保管へ移行
		CmnTblDeleteToHokan(m_uInfo.intCurPage, m_uInfo.intCurRow);		
		bRet = TRUE;
		*pnDelLine = -1;
		break;
	case	2:							// 削除
// midori 190505 add -->
		// 削除する行が科目行の場合、科目行下の小計行に科目名称を追加する
		if(uCurInfo.intFgFunc == ID_FGFUNC_KAMOKU) {
			SetKnNameSyokei(m_uInfo.intFormSeq,m_uInfo.intCurPage,m_uInfo.intCurRow,0);
		}
// midori 190505 add <--
		// カレント行の削除を実行
		CmnTblDeleteLine(m_uInfo.intCurPage, m_uInfo.intCurRow, 1);
		bRet = TRUE;
		*pnDelLine = -1;
		break;
	case	3:							//	空行削除
		//	カレント行以降の空行を削除
		nRet = CmnDeleteNullLine(m_uInfo.intCurPage, m_uInfo.intCurRow);
		
		//	削除された？
		if ( nRet > 0 ){
			bRet = TRUE;
			*pnDelLine = nRet * -1;
		}
		else{
			bRet = FALSE;
		}

		break;
	}

	return bRet;
}

//********************************************************************************
//	空行削除処理
//		IN		int				開始頁
//		IN		int				開始行
//		RET		int				0より大きい	…	成功（削除行数）
//								0以下		…	失敗（エラー値）
//********************************************************************************
int CfrmUc000Common::CmnDeleteNullLine( int nPage, int nRow )
{
	int					nRet;		//	戻値
	CdbUc000Common*		rsData;		//	テーブルオブジェクト

	//	テーブルオブジェクト取得
	rsData = virGetDataTableObject();
	//	次の空行、頁計行、累計行以外の行を検索
	nRet = rsData->RequeryNextNotNullLine( nPage, nRow );
	
	//	検索成功？
	if ( nRet == DB_ERR_OK ){
		//	指定範囲の行を削除（頁計、累計は除く）
		nRet = rsData->ExecuteDeleteNullLine( nPage, nRow, rsData->m_NumPage, rsData->m_NumRow );
	}

	//	戻値を返す
	return( nRet );
}

/*------------------------------------------------------------
	<引数>
		inPage	:	一時保管の対象頁
		inRow	:	一時保管の対象行
	<戻値>
		 0	:	成功
		-1	:	失敗
	<機能>
		パラメータで指定された頁/行のデータを一時保管データとする
------------------------------------------------------------*/
int CfrmUc000Common::CmnTblDeleteToHokan(int inPage, int inRow)
{
	CdbUc000Common*		rsData;
	rsData = virGetDataTableObject();

	// 頁と行からレコード抽出
	rsData->RequeryPageRow(inPage, inRow);

	// 入れ替え先が取得できない場合は終了
	if (!rsData->IsEOF()) {
		// 一時保管データへ編集
		rsData->Edit();
		rsData->m_NumPage = 0;					// 頁をクリア
		rsData->m_NumRow = 0;					// 行をクリア
		rsData->m_FgShow = ID_FGSHOW_HOKAN;		// 一時保管
		rsData->Update();
	}
	rsData->Fin();
	delete rsData;

	return FUNCTION_OK;
}

/*------------------------------------------------------------
	<引数>
		inPage	:	削除したい頁
		inRow	:	削除したい行
		inNum	:	削除する行数
	<戻値>
		 0	:	成功
		-1	:	失敗
	<機能>
		パラメータで指定された頁/行から指定行数を削除する
------------------------------------------------------------*/
int CfrmUc000Common::CmnTblDeleteLine(int inPage, int inRow, int inNum)
{
	CdbUc000Common*		rsData;
	int			i;
	int			retVal = FUNCTION_NG;

	retVal = FUNCTION_OK;

	rsData = virGetDataTableObject();

	// 頁番号の昇順、行番号の昇順でソートしたレコードセットを取得
	rsData->RequerySortPageRow();

	// カウンタ変数初期化
	i = 1;

	// 削除処理
	while ((!rsData->IsEOF()) && (i != 0)) {

		// 削除対象行のチェック
		if (((rsData->m_NumPage == inPage) && (rsData->m_NumRow >= inRow)) || (rsData->m_NumPage > inPage)) {

			if (i <= inNum) {
				rsData->Delete();
				i++;
			} else {
				i = 0;
			}
		}

		rsData->MoveNext();
	}

	rsData->Fin();
	delete rsData;

	return retVal;
}

/*------------------------------------------------------------
	<引数>
		なし
	<戻値>
		行数
	<機能>
		行数をカウントアップする際の、ベースになる１頁の行数を求める
------------------------------------------------------------*/
int CfrmUc000Common::CmnTblCountMaxRow()
{
	int		nBaseRow;

	// デフォルトに「計あり」の行数をセットしておく
	nBaseRow = m_uInfo.intRowMaxData;

	// ベースになる行数を決める
	// 「計なし」帳表で、計設定が「オフ」または「累計のみ」の場合は、「計なし」の場合の行数をセットする
	if ((m_uInfo.intRowKei == 0) && ((m_uInfo.intOutKei == ID_OUTKEI_OFF) || (m_uInfo.intOutKei == ID_OUTKEI_RUIKEI))) {
		nBaseRow = m_uInfo.intRowMax;
	}

	return nBaseRow;
}

/*------------------------------------------------------------
	<引数>
		inPage	:	頁番号
		inRow	:	行番号
		inAdd	:	加減数
	<戻値>
		 0	:	成功
		-1	:	失敗
	<機能>
		パラメータの頁番号(inPage)、行番号(inRow)以降の
		頁番号と行番号を加減算する。
------------------------------------------------------------*/
int CfrmUc000Common::CmnTblUpdatePageNumRowNumAdd(int inPage, int inRow, int inAdd)
{
	CdbUc000Common*		rsData;
	int					nBaseRow;
	int					retVal = FUNCTION_NG;

	retVal = FUNCTION_OK;

	rsData = virGetDataTableObject();

	// 行番号を振り直すための基準となる行数を取得
	nBaseRow = CmnTblCountMaxRow();

	// 頁番、行番振り直し
	rsData->UpdatePageNumRowNumAdd(inPage, inRow, inAdd, nBaseRow);

	rsData->Fin();
	delete rsData;

	return retVal;
}

// midori 154714 add -->
/*------------------------------------------------------------
	<引数>	なし
	<戻値>	なし
	<機能>	新しくできた頁に空行以外のデータが無ければ、新しくできた頁を削除する
------------------------------------------------------------*/
void CfrmUc000Common::CmnTblUpdateNewPageCheckAndDelete( void )
{
	CdbUc000Common*		rsData;

	rsData = virGetDataTableObject();

	rsData->UpdateNewPageCheckAndDelete();

	rsData->Fin();
	delete rsData;
}
// midori 154714 add <--

/*------------------------------------------------------------
	<引数>
		inPage	:	頁番号
		inRow	:	行番号
	<戻値>
		 0	:	成功
		-1	:	失敗
	<機能>
		パラメータの頁番号(inPage)、行番号(inRow)以降の
		頁番号と行番号を振り直す。
------------------------------------------------------------*/
int CfrmUc000Common::CmnTblUpdatePageNumRowNumReset(int inPage, int inRow)
{
	CdbUc000Common*		rsData;
	int					intNumPage;
	int					intNumRow;
	int					nBaseRow;
	int					retVal = FUNCTION_NG;

	retVal = FUNCTION_OK;

	rsData = virGetDataTableObject();

	// 行番号を振り直すための基準となる行数を取得
	nBaseRow = CmnTblCountMaxRow();

	// 頁番号の昇順、行番号の昇順でソートされたレコードセットを取得する。
	rsData->RequeryFgShowSortPageRow(ID_FGSHOW_OFF);

	intNumPage = inPage;
	intNumRow = inRow;

	// 頁番、行番振り直し
	while (!rsData->IsEOF()) {

		// パラメータの頁番号、行番号以上のレコードから番号を振り直す
		if ((rsData->m_NumPage > inPage) || ((rsData->m_NumPage == inPage) && (rsData->m_NumRow >= inRow))) {
			rsData->Edit();
			rsData->m_NumPage = intNumPage;		// 頁番号をセット
			rsData->m_NumRow = intNumRow;		// 行番号をセット
			rsData->Update();
		}

		// 頁番号と行番号をインクリメント
		if (intNumRow >= nBaseRow) {
			intNumPage++;
			intNumRow = 1;
		}
		else {
			intNumRow++;
		}

		// 次のレコードへ
		rsData->MoveNext();
	}

	rsData->Fin();
	delete rsData;

	return retVal;
}

// midori 152709 add -->
/*------------------------------------------------------------
	<引数>
		なし
	<戻値>
		 0	:	成功
		-1	:	失敗
	<機能>
		小計が最終行で頁の最後の場合、空白行を挿入する
------------------------------------------------------------*/
// midori 156968 del -->
//int CfrmUc000Common::CmnTblUpdatePageNumRowNumReset_Sub(void)
// midori 156968 del <--
// midori 156968 add -->
int CfrmUc000Common::CmnTblUpdatePageNumRowNumReset_Sub(int pSw)
// midori 156968 add <--
{
	CdbUc000Common*					rsData;
	int								intNumPage=0;
	int								intNumRow=0;
	int								nBaseRow=0;
	int								retVal = FUNCTION_NG;
	int								numrow=0;
	int								numpage=0;
	int								fgfunc=0,fgfunc2=0;
	int								ii=0;
	int								sw=0;				//	1:小計行の次に空白を含める
// midori 156968_3 add -->
	int								sw2=0;
// midori 156968_3 add <--
	int								cnt=0;				//	mapのデータ件数
// midori 156968 add -->
	CString							strfilter=_T("");
	CString							strsort=_T("");
// midori 156968 add <--
	CdbUcInfSub						mfcRec( m_pDB );	//	uc_inf_subテーブルクラス

	// 小計の行次に空行を挿入するにチェックが有る場合処理を行う
	sw = 0;
	if(mfcRec.RequeryFormSeq(m_uInfo.intFormSeq) == DB_ERR_OK)	{
		if(mfcRec.m_OpOutNullLine == 1)	{
			sw = 1;
		}
	}
	mfcRec.Fin();
	if(sw == 0)	{
		return(0);
	}

	retVal = FUNCTION_OK;

	rsData = virGetDataTableObject();

	// 行番号を振り直すための基準となる行数を取得
	nBaseRow = CmnTblCountMaxRow();

// midori 156968 del -->
	//// 頁番号の昇順、行番号の昇順でソートされたレコードセットを取得する。
	//rsData->RequeryFgShowSortPageRow(ID_FGSHOW_OFF);
// midori 156968 del <--
// midori 156968 add -->
	// 頁番号の昇順、行番号の昇順でソートされたレコードセットを取得する。
	if(pSw == 0) {
		// 全頁のレコードセットを取得する
		rsData->RequeryFgShowSortPageRow(ID_FGSHOW_OFF);
	}
	else {
		// フォーカスのある位置以降のレコードセットを取得する
		strfilter.Format(_T("FgShow = %d and (NumPage > %d or (NumPage = %d and NumRow >= %d ))"),ID_FGSHOW_OFF,m_uInfo.intCurPage,m_uInfo.intCurPage,m_uInfo.intCurRow);
		strsort = _T("NumPage ASC, NumRow ASC, Seq DESC");
		rsData->RequerySortParameter(strfilter,strsort);
	}
// midori 156968 add <--

	// 最終行-1行目までループを行う（最終行の場合は下に空白行を作成しないため）
	while (!rsData->IsEOF()) {
		// 現在行のデータを取得
		numpage = rsData->m_NumPage;
		numrow = rsData->m_NumRow;
		fgfunc = rsData->m_FgFunc;
		// 次行のデータを取得
		rsData->MoveNext();
		if(rsData->IsEOF())	{
			// 次の行が無ければ終了
			break;
		}
		fgfunc2 = rsData->m_FgFunc;
		// 前の行に戻す
		rsData->MovePrev();
// midori 156968_3 del -->
//// midori 152909_1 del -->
//		//// 現在の行が小計で頁の最終行かつ次の行が空白行以外の場合
//		//// 空白行を挿入する
//		//if(fgfunc == ID_FGFUNC_SYOKEI && fgfunc2 != ID_FGFUNC_NULL && numrow == nBaseRow)	{
//// midori 152909_1 del <--
//// midori 152909_1 add -->
//		// 現在の行が小計で頁の最終行かつ次の行が空白行・中計以外の場合
//		// 空白行を挿入する
//		if( fgfunc == ID_FGFUNC_SYOKEI && numrow == nBaseRow && 
//			fgfunc2 != ID_FGFUNC_NULL && 
//			fgfunc2 != ID_FGFUNC_CHUKEI && 
//			fgfunc2 != ID_FGFUNC_CHUKEINULL )	{
//// midori 152909_1 add <--
//// midori 152765 del -->
//			//rsData->CreateNewRecord();
//			//rsData->Edit();
//			//rsData->m_NumPage = numpage;	// 03.ページ番号
//			//rsData->m_NumRow = numrow+1;	// 04.行番号
//			//rsData->m_FgFunc = 0;			// 05.特殊行フラグ
//			//rsData->m_FgShow = 0;			// 06.表示フラグ
//			//rsData->Update();
//// midori 152765 del <--
//// midori 152765 add -->
//			rsData->CreateNewRecord2(numpage,numrow+1);
//// midori 152765 add <--
//		}
// midori 156968_3 del <--
// midori 156968_3 add -->
		sw2 = 0;
		// 現在の行が小計で頁の最終行かつ次の行が空白行・中計以外の場合
		if (fgfunc == ID_FGFUNC_SYOKEI && numrow == nBaseRow &&
			fgfunc2 != ID_FGFUNC_NULL &&
			fgfunc2 != ID_FGFUNC_CHUKEI &&
			fgfunc2 != ID_FGFUNC_CHUKEINULL) {
			sw2 = 1;
		}
		if(bG_Kanso == TRUE) {
			// 現在の行が中計で次の行が空白行以外の場合
			if (fgfunc == ID_FGFUNC_CHUKEI && numrow == nBaseRow && fgfunc2 != ID_FGFUNC_NULL) {
				sw2 = 1;
			}
		}

		// 空白行を挿入する
		if( sw2 == 1) {
// 157153 del -->
			//rsData->CreateNewRecord2(numpage,numrow+1);
// 157153 del <--
// 157153 add -->
			rsData->CreateNewRecord2(numpage,numrow+1,0);
// 157153 add <--
		}
// midori 156968_3 add <--
		rsData->MoveNext();
	}

	rsData->Fin();
	delete rsData;

	return(retVal);
}

// 157153 add -->
/*------------------------------------------------------------
	<引数>
		なし
	<戻値>
		 0	:	成功
		-1	:	失敗
	<機能>
		小計が最終行で頁の最後の場合、空白行を挿入する
------------------------------------------------------------*/
int CfrmUc000Common::CmnTblUpdatePageNumRowNumReset_Sub_Kanso(int pSw)
{
	CdbUc000Common*					rsData;
	int								intNumPage=0;
	int								intNumRow=0;
	int								nBaseRow=0;
	int								retVal = FUNCTION_NG;
	int								numrow=0;
	int								numpage=0;
	int								fgfunc=0,fgfunc2=0;
	int								ii=0;
	int								sw=0;				//	1:小計行の次に空白を含める
	int								sw2=0;
	int								cnt2=0;				// 第１ソート項目ごとのデータ件数
	int								flg=0;				// 0:空白行を挿入	1:小計NULLを挿入
	int								nOpOutSyokei = 0;
	int								sort=0;
// 20-0450 add -->
	int								sw3=0;
	CUcFunctionCommon				uc;
	CStringArray					sa1;
	CStringArray					sa2;
// 20-0450 add <--
	ITEMSORT_INFO					uItemSort;
	CString							strItem[2]={_T("")};
	CString							strNewItem[2]={_T("")};
	CString							strfilter=_T("");
	CString							strsort=_T("");
	CdbUcInfSub						mfcRec( m_pDB );	//	uc_inf_subテーブルクラス

	// 小計の行次に空行を挿入するにチェックが有る場合処理を行う
	sw = 0;
	if(mfcRec.RequeryFormSeq(m_uInfo.intFormSeq) == DB_ERR_OK)	{
		// 自動小計フラグ[FgSykei]がONの第１ソート項目が選択されている場合のみ処理を行う
		sort = mfcRec.m_Sort1;
		if(sort == 0)	{
			// 一度も編集～ソートを開いていない場合は、第１ソート項目の項目番号が書かれていない
			if(m_uInfo.intFormSeq == 2)	sort=8;
			else						sort=1;
		}
		CmnUcLstItemSortGetData(sort, &uItemSort);
// 211022 del -->
		//nOpOutSyokei = mfcRec.m_OpOutSyokei;
// 211022 del <--
// 211022 add -->
		nOpOutSyokei = 1;
		if(mfcRec.m_OpSykeiAuto != 0 && mfcRec.m_OpOutSyokei == 0)	{
			nOpOutSyokei = 0;
		}
// 211022 add <--
		if(mfcRec.m_OpOutNullLine == 1 && uItemSort.bytFgSykei == 1)	{
			sw = 1;
		}
		if(mfcRec.m_OpOutSyokei == 0 && uItemSort.bytFgSykei == 1)	{
			sw = 1;
		}
	}
	mfcRec.Fin();
	if(sw == 0)	{
		return(0);
	}

	retVal = FUNCTION_OK;

	rsData = virGetDataTableObject();

	// 行番号を振り直すための基準となる行数を取得
	nBaseRow = CmnTblCountMaxRow();

	// 頁番号の昇順、行番号の昇順でソートされたレコードセットを取得する。
	if(pSw == 0) {
		// 全頁のレコードセットを取得する
		rsData->RequeryFgShowSortPageRow(ID_FGSHOW_OFF);
	}
	else {
		// フォーカスのある位置以降のレコードセットを取得する
		strfilter.Format(_T("FgShow = %d and (NumPage > %d or (NumPage = %d and NumRow >= %d ))"),ID_FGSHOW_OFF,m_uInfo.intCurPage,m_uInfo.intCurPage,m_uInfo.intCurRow);
		strsort = _T("NumPage ASC, NumRow ASC, Seq DESC");
		rsData->RequerySortParameter(strfilter,strsort);
	}

	// 最終行-1行目までループを行う（最終行の場合は下に空白行を作成しないため）
	while (!rsData->IsEOF()) {
		// 現在行のデータを取得
		numpage = rsData->m_NumPage;
		numrow = rsData->m_NumRow;
		fgfunc = rsData->m_FgFunc;
		// 一行でも小計を挿入するにチェックが無い場合のみ
		if(nOpOutSyokei == 0) {
			// 第１ソート項目のデータを取得
			strItem[0] = rsData->GetFieldValueString(uItemSort.strFieldSykei);
			strItem[1] = rsData->GetFieldValueString(uItemSort.strFieldSykei2);
		}
		// 次行のデータを取得
		rsData->MoveNext();
		if(rsData->IsEOF())	{
			// 次の行が無ければ終了
			break;
		}
		fgfunc2 = rsData->m_FgFunc;
		// 一行でも小計を挿入するにチェックが無い場合のみ
		if(nOpOutSyokei == 0) {
			// 第１ソート項目のデータを取得
			strNewItem[0] = rsData->GetFieldValueString(uItemSort.strFieldSykei);
			strNewItem[1] = rsData->GetFieldValueString(uItemSort.strFieldSykei2);
		}
		// 前の行に戻す
		rsData->MovePrev();
		sw2 = 0;
		flg = 0;
		// 現在の行が小計で頁の最終行かつ次の行が空白行・中計以外の場合
		if (fgfunc == ID_FGFUNC_SYOKEI && numrow == nBaseRow &&
			fgfunc2 != ID_FGFUNC_NULL &&
			fgfunc2 != ID_FGFUNC_CHUKEI &&
			fgfunc2 != ID_FGFUNC_CHUKEINULL) {
			sw2 = 1;
		}
		// 現在の行が中計で次の行が空白行以外の場合
		if (fgfunc == ID_FGFUNC_CHUKEI && numrow == nBaseRow && fgfunc2 != ID_FGFUNC_NULL) {
			sw2 = 1;
		}

		// 一行でも小計を挿入するにチェックが無い場合のみ
		if(nOpOutSyokei == 0) {
			// 現在の行がデータ行（明細行、一括金額(手動、自動)）かつ
			// 次の行がデータ行（明細行、一括金額(手動、自動)）
			if( (fgfunc == ID_FGFUNC_DATA || fgfunc == ID_FGFUNC_IKKATUMANUAL || fgfunc == ID_FGFUNC_IKKATUAUTO) && 
				fgfunc2 == ID_FGFUNC_DATA || fgfunc2 == ID_FGFUNC_IKKATUMANUAL || fgfunc2 == ID_FGFUNC_IKKATUAUTO) {
// 20-0450 del -->
				//// 現在の行と次の行の第一ソート項目のデータが異なる
				//if((strItem[0] != strNewItem[0]) || (strItem[1] != strNewItem[1])) {
// 20-0450 del <--
// 20-0450 add -->
				sw3 = 0;
				if(bG_Kanso == TRUE && m_uInfo.intFormSeq == ID_FORMNO_061 && 
					uItemSort.strFieldSykei.Compare(_T("Syurui")) == 0)	{
					uc.StrDivision(strItem[0],&sa1,2);
					uc.StrDivision(strNewItem[0],&sa2,2);
					if(uItemSort.intItemSeq == 2)	{
						// 種類
						if(sa1.GetAt(0) != sa2.GetAt(0))	{
							sw3 = 1;
						}
					}
					else	{
						// 銘柄
						if(sa1.GetAt(1) != sa2.GetAt(1))	{
							sw3 = 1;
						}
					}
				}
				else	{
					// 現在の行と次の行の第一ソート項目のデータが異なる
					if((strItem[0] != strNewItem[0]) || (strItem[1] != strNewItem[1])) {
						sw3 = 1;
					}
				}
				if(sw3 != 0)	{
// 20-0450 add <--
					// 現在の行が最終行
					if(numrow == nBaseRow) {
						// 現在の行以外にデータが無ければ
						if(cnt2 == 0) {
							sw2 = 1;
							// 小計NULLを挿入
							flg = 1;
						}
					}
					// データカウントを初期化
					cnt2 = 0;
				}
				// 現在の行と次の行の第一ソート項目のデータが同じ
				else {
					// データカウントを上げる
					cnt2++;
				}
			}
			// 現在の行または次の行がデータ行（明細行、一括金額(手動、自動)）でない
			else {
				// データカウントを初期化
				cnt2 = 0;
			}
		}

		// 空白行または小計NULLを挿入する
		if( sw2 == 1) {
			rsData->CreateNewRecord2(numpage,numrow+1,flg);
		}
		rsData->MoveNext();
	}

	rsData->Fin();
	delete rsData;

	return(retVal);
}
// 157153 add <--

// 157153 del -->
//// midori 156188 add -->
//// -----------------------------------------------------------------------------------------------------------------------------------------------------------------
////	<引数>
////		なし
////	<戻値>
////		 0	:	成功
////		-1	:	失敗
////	<機能>
////		データ行が最終行で頁の最後の場合かつ次の頁の先頭行と第1ソートキーが異なる場合、小計NULLを挿入する
//// -----------------------------------------------------------------------------------------------------------------------------------------------------------------
//int CfrmUc000Common::CmnTblUpdatePageNumRowNumReset_Sub2(void)
//{
//	CdbUc000Common*								rsData;
//	int											intNumPage=0;
//	int											intNumRow=0;
//	int											nBaseRow=0;
//	int											retVal = FUNCTION_NG;
//	int											numrow=0;
//	int											numpage=0;
//	int											fgfunc=0,fgfunc2=0;
//	int											sw=0;
//	int											cnt=0;				//	mapのデータ件数
//	ITEMSORT_INFO								uItemSort;			// 第１ソート項目のソート情報
//	SORTKAMOKU_INFO								uKamoku;			// レコードの科目情報
//	SORTKAMOKU_INFO								uNewKamoku;			// 次レコードの科目情報
//	CdbUcInfSub									mfcRec( m_pDB );	//	uc_inf_subテーブルクラス
//	CRecordset									rs(m_pDB);
//	CString										strdata[2];
//
//	// 「小計」を自動挿入するにチェックがあり、一行でも小計を挿入するにチェックが無い場合処理を行う
//	// ソートダイアログを開いていないまたは第１ソートキーが科目の場合のみ処理を行う
//	sw = 0;
//	if(mfcRec.RequeryFormSeq(m_uInfo.intFormSeq) == DB_ERR_OK)	{
//		// uc_lst_item_sortを参照して必要情報を取得
//		CmnUcLstItemSortGetData2(m_uInfo.intFormSeq,mfcRec.m_Sort1, &uItemSort);
//		if(mfcRec.m_OpSykeiAuto == 1 && mfcRec.m_OpOutSyokei == 0 && (mfcRec.m_Sort1 == 0 || uItemSort.bytFgItem == ID_FGITEM_KAMOKU))	{
//			sw = 1;
//		}
//	}
//	mfcRec.Fin();
//
//	if(sw == 0)	{
//		return(0);
//	}
//
//	retVal = FUNCTION_OK;
//
//	rsData = virGetDataTableObject();
//
//	// 行番号を振り直すための基準となる行数を取得
//	nBaseRow = CmnTblCountMaxRow();
//
//	// 頁番号の昇順、行番号の昇順でソートされたレコードセットを取得する。
//	rsData->RequeryFgShowSortPageRow(ID_FGSHOW_OFF);
//
//	// 最終行-1行目までループを行う（最終行の場合は下に空白行を作成しないため）
//	while (!rsData->IsEOF()) {
//		// 現在行のデータを取得
//		numpage = rsData->m_NumPage;
//		numrow = rsData->m_NumRow;
//		fgfunc = rsData->m_FgFunc;
//		virTblGetKamokuInfo(rsData,&uKamoku);
//		// 次行のデータを取得
//		rsData->MoveNext();
//		if(rsData->IsEOF())	{
//			// 次の行が無ければ終了
//			// 最終行がデータ行の場合、小計NULLを挿入する
//			if(fgfunc == ID_FGFUNC_DATA || fgfunc == ID_FGFUNC_IKKATUAUTO || fgfunc == ID_FGFUNC_IKKATUMANUAL) {
//				rsData->CreateNewRecord3(numpage,numrow+1);
//			}
//			break;
//		}
//		fgfunc2 = rsData->m_FgFunc;
//		virTblGetKamokuInfo(rsData,&uNewKamoku);
//		// 前の行に戻す
//		rsData->MovePrev();
//
//		// 現在の行が最終行かつデータ行（一括手動、一括自動含む）で次の行もデータ行で、
//		// 第1ソートキーが異なる場合、小計NULLを挿入する
//		if(numrow == nBaseRow) {
//			if(fgfunc == ID_FGFUNC_DATA || fgfunc == ID_FGFUNC_IKKATUAUTO || fgfunc == ID_FGFUNC_IKKATUMANUAL) {
//				if(fgfunc2 == ID_FGFUNC_DATA || fgfunc2 == ID_FGFUNC_IKKATUAUTO || fgfunc2 == ID_FGFUNC_IKKATUMANUAL) {
//					if(uKamoku.intKnOrder != uNewKamoku.intKnOrder) {
//						rsData->CreateNewRecord3(numpage,numrow+1);
//					}
//				}
//			}
//		}
//		rsData->MoveNext();
//	}
//
//	rsData->Fin();
//	delete rsData;
//
//	return(retVal);
//}
//// midori 156188 add <--
// 157153 del <--

// midori 156968 del -->
//// midori 156188 del -->
//////********************************************************************************
//////	様式切替時、先頭行の空白を削除する
//////********************************************************************************
////void CfrmUc000Common::CmnTblDeleteFirstNullLine(void)
//// midori 156188 del <--
//// midori 156188 add -->
//// --------------------------------------------------------------------------------------------
//// 様式切替時、先頭行の空白または小計NULLを削除する
////	int		pSw				0:空白行を削除		1:小計NULLを削除
//// --------------------------------------------------------------------------------------------
//void CfrmUc000Common::CmnTblDeleteFirstNullLine(int pSw)
//// midori 156188 add <--
// midori 156968 del <--
// midori 156968 add -->
// --------------------------------------------------------------------------------------------
// 様式切替時、先頭行の空白または小計NULLを削除する
//	int		pSw				0:空白行を削除		1:小計NULLを削除
//	int		pSw2			0:全頁対象			1:表示頁より後の頁のみ対象
// --------------------------------------------------------------------------------------------
void CfrmUc000Common::CmnTblDeleteFirstNullLine(int pSw,int pSw2)
// midori 156968 add <--
{
	int					numpage=0;
	int					numrow=0;
	int					delsw=0;
	int					nBaseRow=0;
	int					retVal = FUNCTION_NG;
// midori 156188 add -->
	int					fgfunc = 0;
// midori 156188 add <--
// midori 156968_4 add -->
	int					svfgfunc = 0;
	int					sw=0;
	CString				strfilter=_T("");
	CString				strsort=_T("");
// midori 156968_4 add <--
	CdbUc000Common*		rsData;
// 157153 del -->
	//CdbUcInfSub			mfcRec( m_pDB );	//	uc_inf_subテーブルクラス
// 157153 del <--

	retVal = FUNCTION_OK;

	rsData = virGetDataTableObject();

	// 行番号を振り直すための基準となる行数を取得
	nBaseRow = CmnTblCountMaxRow();

	// 頁番号の昇順、行番号の昇順でソートされたレコードセットを取得する。
// midori 156968_4 del -->
	//rsData->RequeryFgShowSortPageRow(ID_FGSHOW_OFF);
// midori 156968_4 del <--
// midori 156968_4 add -->
	numpage = 1;
	numrow = 1;
	if(pSw2 == 0) {
		rsData->RequeryFgShowSortPageRow(ID_FGSHOW_OFF);
	}
	else {
		// 頁番号の昇順、行番号の昇順でソートされたレコードセットを取得する。
		strfilter.Format(_T("FgShow = %d and (NumPage = %d and NumRow >= %d) or (NumPage > %d)"),ID_FGSHOW_OFF,m_uInfo.intCurPage,m_uInfo.intCurRow,m_uInfo.intCurPage);
		strsort = _T("NumPage ASC, NumRow ASC, Seq DESC");
		rsData->RequerySortParameter(strfilter,strsort);
		if (!rsData->IsEOF()) {
			numpage = rsData->m_NumPage;
			numrow = rsData->m_NumRow;
		}
	}
// midori 156968_4 add <--

// midori 156968_4 del -->
	//numpage = 1;
	//numrow = 1;
// midori 156968_4 del <--
// midori 156188 add -->
	if(pSw == 0)			fgfunc = ID_FGFUNC_NULL;
	else					fgfunc = ID_FGFUNC_SYOKEINULL;
// midori 156188 add <--
	while(!rsData->IsEOF())	{
		// 頁の先頭で空白行の場合、削除する
		delsw=0;
// midori 156968_4 del -->
//// midori 156188 del -->
//		//if(rsData->m_FgFunc == ID_FGFUNC_NULL && numrow == 1)	{
//// midori 156188 del <--
//// midori 156188 add -->
//		if(rsData->m_FgFunc == fgfunc && numrow == 1)	{
//// midori 156188 add <--
// midori 156968_4 del <--
// midori 156968_4 add -->
		sw = 0;
		if(bG_Kanso == FALSE || fgfunc == ID_FGFUNC_SYOKEINULL) {
			if(rsData->m_FgFunc == fgfunc && numrow == 1)	sw = 1;
		}
		else {
			if(rsData->m_FgFunc == fgfunc && numrow == 1 && (svfgfunc == ID_FGFUNC_SYOKEI || svfgfunc == ID_FGFUNC_CHUKEI)) {
				sw = 1;
			}
		}
		svfgfunc = rsData->m_FgFunc;
		if(sw == 1) {
// midori 156968_4 add <--
			// 空白行の削除
			rsData->Delete();
			delsw=1;
		}
		rsData->MoveNext();
		if(delsw == 0)	{
			numrow++;
			if(numrow > nBaseRow)	{
				numpage++;
				numrow = 1;
			}
		}
	}
}
// midori 152709 add <--

// 157153,157155 add -->
// --------------------------------------------------------------------------------------------
// 様式切替時、先頭行の空白または小計NULLを削除する
//	int		pSw2			0:全頁対象			1:表示頁より後の頁のみ対象
// --------------------------------------------------------------------------------------------
void CfrmUc000Common::CmnTblDeleteFirstNullLine_Kanso(int pSw2)
{
	int					numpage=0;
	int					numrow=0;
	int					delsw=0;
	int					nBaseRow=0;
	int					retVal = FUNCTION_NG;
	int					fgfunc = 0;
// 157381 add -->
	int					zero=0;
// 157381 add <--
	CString				strfilter=_T("");
	CString				strsort=_T("");
	CdbUc000Common*		rsData;

	retVal = FUNCTION_OK;

	rsData = virGetDataTableObject();

	// 行番号を振り直すための基準となる行数を取得
	nBaseRow = CmnTblCountMaxRow();

	// 頁番号の昇順、行番号の昇順でソートされたレコードセットを取得する。
	numpage = 1;
	numrow = 1;
	if(pSw2 == 0) {
		rsData->RequeryFgShowSortPageRow(ID_FGSHOW_OFF);
	}
	else {
		// 頁番号の昇順、行番号の昇順でソートされたレコードセットを取得する。
		strfilter.Format(_T("FgShow = %d and (NumPage = %d and NumRow > %d) or (NumPage > %d)"),ID_FGSHOW_OFF,m_uInfo.intCurPage,m_uInfo.intCurRow,m_uInfo.intCurPage);
		strsort = _T("NumPage ASC, NumRow ASC, Seq DESC");
		rsData->RequerySortParameter(strfilter,strsort);
		if (!rsData->IsEOF()) {
			numpage = rsData->m_NumPage;
			numrow = rsData->m_NumRow;
		}
	}

	while(!rsData->IsEOF())	{
		delsw=0;
		// １行目が空白または小計NULLの場合、削除する
		if(numrow == 1) {
			if(rsData->m_FgFunc == ID_FGFUNC_NULL || rsData->m_FgFunc == ID_FGFUNC_SYOKEINULL )	{
// 157381 add -->
				if(rsData->m_ShowKeiZero & BIT_D7)	{
					zero=1;
				}
// 157381 add <--
				// 削除
				rsData->Delete();
				delsw=1;
			}
		}
		rsData->MoveNext();
// 157381 add -->
		if(zero != 0)	{
			rsData->Edit();
			rsData->m_ShowKeiZero |= BIT_D7;
			rsData->Update();
			zero=0;
		}
// 157381 add <--
		if(delsw == 0)	{
			numrow++;
			if(numrow > nBaseRow)	{
				numpage++;
				numrow = 1;
			}
		}
	}
	rsData->Fin();
}
// 157153,157155 add <--

// midori 152709 del -->
////********************************************************************************
////	テーブル全体に対して頁と行の再設定を行う（メイン）
////		例）ソート，出力形式，計設定など
////
////		IN		なし
////		RET		int			FUNCTION_OK:正常終了，FUNCTION_NG:エラー
////********************************************************************************
//int CfrmUc000Common::CmnTblUpdatePageRowAllResetMain()
//{
//	// 頁と行の調整処理
//	CmnTblDeleteFgFunc(ID_FGFUNC_PAGEKEI);			// 頁計の削除
//	CmnTblDeleteFgFunc(ID_FGFUNC_RUIKEI);			// 累計の削除
//	CmnTblUpdatePageNumRowNumReset(1, 1);			// 先頭頁からページ再設定
//	CmnTblUpdateEndPageSetting();					// 最終頁を調整する
//	CmnTblInsertPageKeiRuiKei();					// 頁計/累計挿入（最終ページの削除or空行追加）
//
//	// 小計/中計/頁計/累計の再計算
//	CmnTblCalcKei();
//
//	return FUNCTION_OK;
//}
// midori 152709 del <--
// midori 152709 add -->
//********************************************************************************
//	テーブル全体に対して頁と行の再設定を行う（メイン）
//		例）ソート，出力形式，計設定など
//
//		IN		なし
//		RET		int			0:正常終了，-1:エラー ,1:空白行挿入有り（出力形式の切り替えで使用）
//********************************************************************************
int CfrmUc000Common::CmnTblUpdatePageRowAllResetMain(int pSw)
{
// midori 156968_3 add -->
	// [F7:削除][F8:挿入][保管/一括金額参照][一括保管][合計設定]で
	// 小計行下に空白行を追加する処理は
	// 簡素化様式のみ処理を行う
	if (pSw == 5 || pSw == 6) {
		if (bG_Kanso == FALSE) {
			return(FUNCTION_OK);
		}
	}
// midori 156968_3 add <--

	// 頁と行の調整処理
	CmnTblDeleteFgFunc(ID_FGFUNC_PAGEKEI);			// 頁計の削除
	CmnTblDeleteFgFunc(ID_FGFUNC_RUIKEI);			// 累計の削除

// 157153 del -->
//	if(pSw == 1)	{
//		CmnTblUpdatePageNumRowNumReset_Sub();
//	}
//	else if(pSw == 2)		{
//		CmnTblDeleteFirstNullLine();
//	}
//// midori 156188 add -->
//	else if(pSw == 3)		{
//		// 小計NULLを挿入する
//		CmnTblUpdatePageNumRowNumReset_Sub2();
//	}
//	else if(pSw == 4)		{
//		// 頁先頭の小計NULLを削除する
//		CmnTblDeleteFirstNullLine(1);
//	}
//// midori 156188 add <--
//// midori 156968 add -->
//	else if(pSw == 5)		{
//		CmnTblUpdatePageNumRowNumReset_Sub(1);
//	}
//	else if(pSw == 6)		{
//		CmnTblDeleteFirstNullLine(0,1);
//	}
//// midori 156968 add <--
// 157153 del <--
// 157153 add -->
	if(pSw == 1)	{
		if(bG_Kanso == TRUE)	CmnTblUpdatePageNumRowNumReset_Sub_Kanso();
		else					CmnTblUpdatePageNumRowNumReset_Sub();
	}
	else if(pSw == 2)		{
		if(bG_Kanso == TRUE)	CmnTblDeleteFirstNullLine_Kanso(0);
		else					CmnTblDeleteFirstNullLine();
	}
	else if(pSw == 5)		{
		CmnTblUpdatePageNumRowNumReset_Sub_Kanso(1);
	}
	else if(pSw == 6)		{
		CmnTblDeleteFirstNullLine_Kanso(1);
	}
// 157153 add <--

	CmnTblUpdatePageNumRowNumReset(1, 1);			// 先頭頁からページ再設定
	CmnTblUpdateEndPageSetting();					// 最終頁を調整する
	CmnTblInsertPageKeiRuiKei();					// 頁計/累計挿入（最終ページの削除or空行追加）

	// 小計/中計/頁計/累計の再計算
	CmnTblCalcKei();

	return(FUNCTION_OK);
}
// midori 152709 add <--

//********************************************************************************
//	テーブル全体に対して頁と行の再設定を行う（メイン）（ソート後）
//　小計を自動挿入する際、のエラーを回避する為にソート後はこちらを呼ぶ
//		例）ソート，出力形式，計設定など
//
//		IN		なし
//		RET		int			FUNCTION_OK:正常終了，FUNCTION_NG:エラー
//********************************************************************************
int CfrmUc000Common::CmnTblUpdatePageRowAllResetMainAfterSort(CdlgSort* pCdlgSort)
{
	// 頁と行の調整処理
	CmnTblDeleteFgFunc(ID_FGFUNC_PAGEKEI);			// 頁計の削除
	CmnTblDeleteFgFunc(ID_FGFUNC_RUIKEI);			// 累計の削除
	CmnTblUpdatePageNumRowNumReset(1, 1);			// 先頭頁からページ再設定
	CmnTblUpdateEndPageSetting();					// 最終頁を調整する
	CmnTblInsertPageKeiRuiKei();					// 頁計/累計挿入（最終ページの削除or空行追加）

	// 小計/中計/頁計/累計の再計算
	CmnTblCalcKei();
// 14-1821_15-1030 add -->
// この対応を行ったことで、１行だけの科目が最終行にある場合の再計算は不要となりました
// そのまま置いておいても影響ないかと思ったのですが、第１ソート項目が未入力の【小計】計算が正しく行われていなかったため
// カットしました。
// 科目指定出力についても同じながれがありましたが、14-1821 については入力のように計算後に修正ができないことと
// 第１ソート項目が未入力の【小計】計算が正しく行われていないことに対しても、未入力の項目を最終に持っていくため
// 第１ソート項目に 999999 をセットしていることで問題が起きていなかったため、変更は行っていません。
	//// 小計の自動挿入のときのみ
	//if( m_NullSyokeiRow != FALSE ){
	//	// 小計のみ上書き
	//	CmnTblCalcKeiAfterSort(pCdlgSort);
	//	m_NullSyokeiRow = FALSE;
	//}
// 14-1821_15-1030 add <--

	return FUNCTION_OK;
}

//****************************************************************************
//	行入れ替え
//		IN:		int		挿入位置（頁）→ カレント頁を更新
//				int		挿入位置（行）→ カレント行を更新
//				int		-1:上へ，1:下へ
//		RET:	int		実行結果（FUNCTION_OK, FUNCTION_NG）
//****************************************************************************
int CfrmUc000Common::CmnTblChangeRecord(int* pPage, int* pRow, int nTarget)
{
	CdbUc000Common*		rs1;
	CdbUc000Common*		rs2;
	int					nTempPage;
	int					nTempRow;
	
	// パラメータのチェック
	if (*pPage <= 0 || *pRow <= 0) {
		return FUNCTION_NG;
	}
	if (nTarget != -1 && nTarget != 1) {
		return FUNCTION_NG;
	}

	rs1 = virGetDataTableObject();
	rs2 = virGetDataTableObject();

	// クエリー実行にて、入れ替え元＋先データ取得
	rs1->RequeryPageRow(*pPage, *pRow);
	rs2->RequeryNextBeforeRecord(*pPage, *pRow, nTarget, 0);
	
	// 入れ替え先が取得できない場合は終了
	if (rs2->IsEOF() != 0) {
		rs1->Fin();
		rs2->Fin();
		delete rs1;
		delete rs2;

		return FUNCTION_NG;
	}
	
	rs1->Edit();
	rs2->Edit();
	
	// ページの入れ替え
	nTempPage = rs1->m_NumPage;
	rs1->m_NumPage = rs2->m_NumPage;
	rs2->m_NumPage = nTempPage;
	*pPage = rs1->m_NumPage;				// カレント頁を更新
	
	// 行の入れ替え
	nTempRow = rs1->m_NumRow;
	rs1->m_NumRow = rs2->m_NumRow;
	rs2->m_NumRow = nTempRow;
	*pRow = rs1->m_NumRow;					// カレント行を更新
	
	// レコード更新
	rs1->Update();
	rs2->Update();
	
	rs1->Fin();
	rs2->Fin();
	
	delete rs1;
	delete rs2;
	
	return	FUNCTION_OK;
}

//********************************************************************************
//	一括金額/一時保管参照のデータを戻す処理
//		IN		EnumIdFgShow		ID_FGSHOW_IKKATU or ID_FGSHOW_HOKAN
//				int					行番号
//				int					列番号
//				int					グループ番号
//		RET		なし
//********************************************************************************
int CfrmUc000Common::CmnTblUpdateFgShowDataReturn(EnumIdFgShow nFgShow, int nPage, int nRow, int nGroup)
{
	int			intFgShowTemp = 0;
	int			nBaseRow;

	// 行番号を振り直すための基準となる行数を取得
	nBaseRow = CmnTblCountMaxRow();

	// パラメータのチェック
	if( nPage <= 0 || nRow <= 0 ){
		return FUNCTION_NG;
	}
	switch( nFgShow ){
	case ID_FGSHOW_IKKATU:	// 一括金額参照
		intFgShowTemp = ID_FGSHOW_RET_IKKATU;
		break;
	case ID_FGSHOW_HOKAN:	// 一時保管参照
		intFgShowTemp = ID_FGSHOW_RET_HOKAN;
		break;
	default:
		return FUNCTION_NG;
		break;
	}

	CdbUc000Common*		rsData;

	rsData = virGetDataTableObject();

	// 対象行数の取得
	int nNumRow = rsData->GetCountVal(intFgShowTemp, nGroup);

	// 対象レコードが、1つもないときは処理終了
	if( nNumRow <= 0 ){
		rsData->Fin();
		delete rsData;
		return FUNCTION_NG;
	}

// midori 156968 add -->
	// 最終行（頁の最大行）が小計の場合、小計の下に空行を追加する
	InsertNull();
// midori 156968 add <--

	// 頁番号行番号構築
	CmnTblUpdatePageNumRowNumAdd( nPage, nRow, nNumRow );

	// 指定レコードを抽出
	if( rsData->RequeryFgShow( intFgShowTemp ) != DB_ERR_OK ){
		rsData->Fin();
		delete rsData;
		return FUNCTION_NG;
	}

	if( nFgShow == ID_FGSHOW_HOKAN ){		// 一時保管

		// 様式テーブルの実体に頁番号、行番号を設定
//		nRow--;	// ROW_NUMBER()が1で始まるため
//		rsData->SetPageRowNumToReturnData( nPage, nRow, nBaseRow );
		SetPageRowNumToReturnDataByOrder( rsData, nPage, nRow, nBaseRow );

		// 様式レコードをトップへ戻す
		rsData->MoveFirst();

		// 保管テーブルの整合性をとる
		DoProcAfterReturnHokanTable( rsData );
	}
	else{	// 一括金額
		// 頁番号、行番号を設定
		rsData->MoveFirst();
		while( !rsData->IsEOF() ){	
			rsData->Edit();
			rsData->m_NumPage = nPage;			// 頁番号
			rsData->m_NumRow = nRow++;			// 行番号
			rsData->m_FgShow = ID_FGSHOW_OFF;	// 表示
			rsData->m_NumGroup = 0;				// グループ番号
			rsData->Update();
			rsData->MoveNext();	// 次のレコードへ

			// 次の行が。最終行＋1であれば、次頁へ
			if( nRow >= nBaseRow + 1 ){
				nPage++;
				nRow = 1;
			}
		}
	}
	
// 157153 del -->
//// midori 156968 add -->
//	// 先頭行が空行の場合削除する
//	CmnTblUpdatePageRowAllResetMain(6);
//// midori 156968 add <--
// 157153 del <--

	rsData->Fin();
	delete rsData;

	return FUNCTION_OK;
}

/*------------------------------------------------------------
	<引数>
		なし
	<戻値>
		 0	:	成功
		-1	:	失敗
	<機能>
		最終頁を調整する。最終頁の全行が空行なら、頁まるごと削除する。
------------------------------------------------------------*/
int CfrmUc000Common::CmnTblUpdateEndPageSetting(void)
{
	CdbUc000Common*		rsData;
	int			nBaseMaxPage, nMaxPage;	// 最大頁数
	int			nRowNum;				// 空行且つ表示可能な行の行数
	int			nNullRowNum;			// 空行の行数
	int			retVal = FUNCTION_NG;

	retVal = FUNCTION_OK;

	rsData = virGetDataTableObject();

	// 最大頁数を取得
	nBaseMaxPage = rsData->GetNumPage();
	nMaxPage = nBaseMaxPage;
	
	// 最終頁が全て空行なら頁まるごと削除する。1頁目は対象外とする。
	while (nMaxPage >= 2) {
		
		// 空行(FgFunc=0)、且つ表示行(FgShow=0)の行数を取得する。
		nRowNum = rsData->GetNumRowFgFuncFgShow(nMaxPage, ID_FGFUNC_NULL, ID_FGSHOW_OFF);

		// 表示行(FgShow=0)の行数を取得する。
		nNullRowNum = rsData->GetNumRowFgShow(nMaxPage, ID_FGSHOW_OFF);
		
		// 最終頁の削除有無をチェック
		if (nRowNum == nNullRowNum) {
			nMaxPage--;
		} else {
			break;	// ループから抜ける
		}

	}

	// 空行頁を削除する。
	if (nMaxPage < nBaseMaxPage) {
		rsData->DeletePage(nMaxPage + 1);
	}

	rsData->Fin();
	delete rsData;

	return retVal;
}

/*------------------------------------------------------------
	<引数>
		なし
	<戻値>
		 0	:	成功
		-1	:	失敗
	<機能>
		小計、中計、頁計、累計の合計を求める
------------------------------------------------------------*/
int CfrmUc000Common::CmnTblCalcKei()
{
	CALCKEI_INFO	uCalcInfo;
	CString		strSyoKei[CALCKEI_MAXCOUNT];		// 小計計算用
	CString		strChuKei[CALCKEI_MAXCOUNT];		// 中計計算用
	CString		strPageKei[CALCKEI_MAXCOUNT];		// 頁計計算用
	CString		strRuiKei[CALCKEI_MAXCOUNT];		// 累計計算用
	BOOL		bSyoKei[CALCKEI_MAXCOUNT];			// 小計用０表示フラグ
	BOOL		bChuKei[CALCKEI_MAXCOUNT];			// 中計用０表示フラグ
	BOOL		bPageKei[CALCKEI_MAXCOUNT];			// 頁計用０表示フラグ
	BOOL		bRuiKei[CALCKEI_MAXCOUNT];			// 累計用０表示フラグ
	CString		strTemp = "";
	int			intMax;
	int			retVal = FUNCTION_OK;
	int			i = 0;

	uCalcInfo = m_uInfo.uCalcInfo;
	intMax = uCalcInfo.intMaxCount;

	// ０円or空欄を出力しないサイン取得
	CdbUcInfSub mfcSub(m_pDB);
	mfcSub.RequeryFormSeq(m_uInfo.intFormSeq);
	CCtrlSubGeneralVar cs(mfcSub);
	int OutZeroNull = 0;

	if(cs.bPrintZeroData)	OutZeroNull |= BIT_D0;
	if(cs.bPrintNullData)	OutZeroNull |= BIT_D1;

// 14-1821_15-1030 add -->
// 211021 del -->
	//// 一行でも小計を挿入する
	//int				OutSyokei = (int)mfcSub.m_OpOutSyokei;
// 211021 del <--
// 211021 add -->
	// 「小計」を自動挿入するON かつ 一行でも小計を挿入するON
	int				OutSyokei = 1;
	if(mfcSub.m_OpSykeiAuto != 0 && mfcSub.m_OpOutSyokei == 0)	{
		OutSyokei = 0;
	}
// 211021 add <--
	ITEMSORT_INFO	uItemSort;			// 第１ソート項目のソート情報
	CString			strOldItem1 = "";	// 第１ソート項目の値（old）
	CString			strNewItem1 = "";	// 第１ソート項目の値（new）
	CString			strOldItem2 = "";	// 第１ソート項目の値（old）
	CString			strNewItem2 = "";	// 第１ソート項目の値（new）
// 22-0003 add -->
	BOOL			bFirst=TRUE;
// 22-0003 add <--
	if(OutSyokei != 1)	{
		// uc_lst_item_sortを参照して必要情報を取得
		CmnUcLstItemSortGetData(mfcSub.m_Sort1,&uItemSort);
	}
// 14-1821_15-1030 add <--

	mfcSub.Fin();

	// 帳表毎のテーブルクラスを"New"する
// 14-1821_15-1030 del -->
	//CdbUc000Common*		rs;
	//rs = virGetDataTableObject();				// 全レコード取得用
	//CdbUc000Common*		rsData;
	//rsData = virGetDataTableObject();			// 集計値更新用：Field名称可変で集計値を更新するため
// 14-1821_15-1030 del <--
// 14-1821_15-1030 add -->
	CdbUc000Common*		rs;
	if(OutSyokei != 1 && m_uInfo.intFormSeq == ID_FORMNO_021)	{
		// ②受取手形の内訳書にKnOrderが"Val"より後ろに追加されたため、GetFieldValueでエラーが発生する。
		// 原因はGetFieldValueがインデックスを昇順でのみ値を取得するからである。
		rs = (CdbUc000Common*)( new CdbUc021UketoritegataEx( m_pDB ) );
	}
// 改良No.21-0086,21-0529 add -->
	else if(OutSyokei != 1 && m_uInfo.intFormSeq == ID_FORMNO_081)	{
		// ②受取手形の内訳書にKnOrderが"Val"より後ろに追加されたため、GetFieldValueでエラーが発生する。
		// 原因はGetFieldValueがインデックスを昇順でのみ値を取得するからである。
		rs = (CdbUc000Common*)( new CdbUc081SiharaitegataEx( m_pDB ) );
	}
// 改良No.21-0086,21-0529 add <--
	else	{
		rs = virGetDataTableObject();		// 全レコード取得用
	}
	CdbUc000Common*		rsData;
	rsData = virGetDataTableObject();		// 集計値更新用：Field名称可変で集計値を更新するため
// 14-1821_15-1030 add <--

	// 頁番号の昇順、行番号の昇順でソートされたレコードセットを取得する。
	rs->RequeryFgShowSortPageRow(ID_FGSHOW_OFF);

	BOOL isZeroNullData = 0;
	CString strTemp2[CALCKEI_MAXCOUNT] = {0};
	// 初期化
	for (int i = 0; i < CALCKEI_MAXCOUNT; i++) {
		uCalcInfo.strKei[i] = "";

		strSyoKei[i].Format("%d", 0);
		strChuKei[i].Format("%d", 0);
		strPageKei[i].Format("%d", 0);
		strRuiKei[i].Format("%d", 0);

		bSyoKei[i]	= FALSE;
		bChuKei[i]	= FALSE;
		bPageKei[i] = FALSE;
		bRuiKei[i]	= FALSE;
	}

// 14-1821_15-1030 add -->
	if(OutSyokei != 1)	{
// 22-0003 del -->
		//// １件目の第１ソート項目の値を"old"へセット
		//strOldItem1 = rs->GetFieldValueString(uItemSort.strFieldSykei);
		//strOldItem2 = rs->GetFieldValueString(uItemSort.strFieldSykei2);
// 22-0003 del <--
// 22-0003 add -->
		// 同じデータに２回 GetFieldValueString すると２回目取得できないため
		bFirst=TRUE;
// 22-0003 add <--
	}
// 14-1821_15-1030 add <--
	// 全レコードに対し１行目から計算していく
	while (!rs->IsEOF()) {
		switch (rs->m_FgFunc) {
		case ID_FGFUNC_SYOKEI:		// 小計値をセット
			// 金額欄へ登録＋小計値を初期化
			for (int i = 0; i < intMax; i++) {
				uCalcInfo.strKei[i] = strSyoKei[i].TrimLeft();		// 集計値で更新
				strSyoKei[i].Format("%d", 0);						// クリア

				uCalcInfo.bExistNum[i] = bSyoKei[i];				// フラグ更新
				bSyoKei[i] = FALSE;									// クリア
			}
// midori 156189,156190,156191 del -->
			//rsData->UpdateCalcKei(rs->m_Seq, uCalcInfo);			// テーブルへ登録
// midori 156189,156190,156191 del <--
// midori 156189,156190,156191 add -->
			if(rs->m_ShowKeiZero & BIT_D7) {
				rsData->UpdateCalcKei(rs->m_Seq, uCalcInfo,1);			// テーブルへ登録
			}
			else {
				rsData->UpdateCalcKei(rs->m_Seq, uCalcInfo,0);			// テーブルへ登録
			}
// midori 156189,156190,156191 add <--
			break;

		case ID_FGFUNC_SYOKEINULL:
			// 小計値を初期化
			for (int i = 0; i < intMax; i++) {
				strSyoKei[i].Format("%d", 0);						// クリア
				bSyoKei[i] = FALSE;									// クリア
			}
			break;

		case ID_FGFUNC_CHUKEI:		// 中計値をセット
			// 金額欄へ登録＋中計値を初期化
			for (int i = 0; i < intMax; i++) {
				uCalcInfo.strKei[i] = strChuKei[i].TrimLeft();		// 集計値で更新
				strChuKei[i].Format("%d", 0);						// クリア

				uCalcInfo.bExistNum[i] = bChuKei[i];				// フラグ更新
				bChuKei[i] = FALSE;									// クリア
// midori M-17061302 add -->
				// 小計をクリアする
				strSyoKei[i].Format("%d", 0);						// クリア
				bSyoKei[i] = FALSE;									// クリア
// midori M-17061302 add <--
			}
// midori 157046 del -->
			//rsData->UpdateCalcKei(rs->m_Seq, uCalcInfo);			// テーブルへ登録
// midori 157046 del <--
// midori 157046 add -->
			if(rs->m_ShowKeiZero & BIT_D7) {
				rsData->UpdateCalcKei(rs->m_Seq, uCalcInfo,1);			// テーブルへ登録
			}
			else {
				rsData->UpdateCalcKei(rs->m_Seq, uCalcInfo,0);			// テーブルへ登録
			}
// midori 157046 add <--
			break;

		case ID_FGFUNC_CHUKEINULL:
			// 中計値を初期化
			for (int i = 0; i < intMax; i++) {
				strChuKei[i].Format("%d", 0);						// クリア
				bChuKei[i] = FALSE;									// クリア
			}
			break;

		case ID_FGFUNC_PAGEKEI:		// 頁計値をセット
			// 金額欄へ登録＋頁計値を初期化
			for (int i = 0; i < intMax; i++) {
				switch (m_uInfo.intOutKei) {
				case ID_OUTKEI_OFF:			// なし
				case ID_OUTKEI_RUIKEI:		// 累計のみを挿入する
					uCalcInfo.strKei[i] = _T("");
					uCalcInfo.bExistNum[i] = FALSE;
					break;
				case ID_OUTKEI_PAGEKEI:		// 頁計のみを挿入する
				case ID_OUTKEI_BOTH:		// 累計と累計の両方を挿入する
					uCalcInfo.strKei[i] = strPageKei[i].TrimLeft();;	// 集計値で更新
					uCalcInfo.bExistNum[i] = bPageKei[i];				// フラグ更新
					break;
				}
				strPageKei[i].Format("%d", 0);						// クリア
				bPageKei[i] = FALSE;								// クリア
			}
			rsData->UpdateCalcKei(rs->m_Seq, uCalcInfo);			// テーブルへ登録
			break;

		case ID_FGFUNC_RUIKEI:		// 累計値をセット
			// 金額欄へ登録＋クリアしない
			for (int i = 0; i < intMax; i++) {
				switch (m_uInfo.intOutKei) {
				case ID_OUTKEI_OFF:			// なし
				case ID_OUTKEI_PAGEKEI:		// 頁計のみを挿入する
					uCalcInfo.strKei[i] = _T("");
					uCalcInfo.bExistNum[i] = FALSE;
					break;
				case ID_OUTKEI_RUIKEI:		// 累計のみを挿入する
				case ID_OUTKEI_BOTH:		// 累計と累計の両方を挿入する
					uCalcInfo.strKei[i] = strRuiKei[i].TrimLeft();;		// 集計値で更新
					uCalcInfo.bExistNum[i] = bRuiKei[i];				// フラグ更新
					break;
				}
			}
			rsData->UpdateCalcKei(rs->m_Seq, uCalcInfo);			// テーブルへ登録
			break;

		default:
// 14-1821_15-1030 add -->
			if(OutSyokei != 1)	{
				strNewItem1 = rs->GetFieldValueString(uItemSort.strFieldSykei);
				strNewItem2 = rs->GetFieldValueString(uItemSort.strFieldSykei2);
// 22-0003 add -->
				if(bFirst == TRUE)	{
					strOldItem1 = strNewItem1;
					strOldItem2 = strNewItem2;
					bFirst = FALSE;
				}
// 22-0003 add <--
				if(rs->m_NumPage > 1 && rs->m_NumRow == 1)	{
					// ２頁以降の１行目で、前データと第１ソートキーが変わっていれば
					if((strOldItem1 != strNewItem1) || (strOldItem2 != strNewItem2))	{
						// 小計値を初期化
						for(int i=0; i<intMax; i++)	{
							strSyoKei[i].Format("%d",0);
							bSyoKei[i] = FALSE;
						}
					}
				}
			}
// 14-1821_15-1030 add <--
			isZeroNullData = 0;

			// 小計、中計、頁計、累計を足しこむ
			for (int i = 0; i < intMax; i++) {
				// フィールド名称から値を取得
				strTemp = rs->GetFieldValueString(uCalcInfo.strField[i]);
				strTemp = strTemp.TrimLeft();
				strTemp2[i] = strTemp;	// 下のforループでGetFieldValueStringをコールすると記述子インデックスが正しくありませんと例外を吐くので仕方なく

				// 加算
				CmnKingakuAddFunc(&strSyoKei[i], strTemp);
				CmnKingakuAddFunc(&strChuKei[i], strTemp);
				CmnKingakuAddFunc(&strPageKei[i], strTemp);
				CmnKingakuAddFunc(&strRuiKei[i], strTemp);

				if((OutZeroNull & BIT_D0) && (OutZeroNull & BIT_D1)){
					if(!(strTemp.IsEmpty() || (strTemp.Compare("0") == 0)))	isZeroNullData++;
				}
				else if(OutZeroNull & BIT_D0){
					if(strTemp.Compare("0") != 0)			isZeroNullData++;
				}
				else if(OutZeroNull & BIT_D1){
					if(!(strTemp.IsEmpty()))				isZeroNullData++;
				}
				else 		isZeroNullData++;
			}

// 22-0003 add -->
			if(OutSyokei != 1 && rs->m_FgFunc == ID_FGFUNC_DATA)	{
				// １件目の第１ソート項目の値を"old"へセット
				strOldItem1 = strNewItem1;
				strOldItem2 = strNewItem2;
			}
// 22-0003 add <--

			if(isZeroNullData == 0)		break;

			for (int i = 0; i < intMax; i++) {
				// 集計対象に1つでも入力があれば、計０円表示フラグを立てる
				if( bSyoKei[i] == FALSE )	bSyoKei[i]	= GetShowKeiZero( strTemp2[i]);		// 一回でもTRUEになったら書き換えない
				if( bChuKei[i] == FALSE )	bChuKei[i]	= GetShowKeiZero( strTemp2[i]);
				if( bPageKei[i] == FALSE )	bPageKei[i] = GetShowKeiZero( strTemp2[i]);
				if( bRuiKei[i] == FALSE )	bRuiKei[i]	= GetShowKeiZero( strTemp2[i]);
			}
// 22-0003 del -->
//// 14-1821_15-1030 add -->
//			if(OutSyokei != 1)	{
//				// １件目の第１ソート項目の値を"old"へセット
//				strOldItem1 = strNewItem1;
//				strOldItem2 = strNewItem2;
//			}
//// 14-1821_15-1030 add <--
// 22-0003 del <--
			break;
		}

		rs->MoveNext();
	}

	rs->Fin();
	rsData->Fin();

	delete rs;
	delete rsData;

	return retVal;
}

// 14-1821_15-1030 del -->
///*------------------------------------------------------------
//	<引数>
//		なし
//	<戻値>
//		 0	:	成功
//		-1	:	失敗
//	<機能>
//		小計、中計、頁計、累計の合計を求める（ソート後）
//------------------------------------------------------------*/
//int CfrmUc000Common::CmnTblCalcKeiAfterSort(CdlgSort* pCdlgSort)
//{
//	// 帳表毎のテーブルクラスを"New"する
//	CdbUc000Common*	rs;
//	CALCKEI_INFO	uCalcInfo;
//	ITEMSORT_INFO	uItemSort;					// 第１ソート項目のソート情報
//
//	// 共通データ取得
//	uCalcInfo = m_uInfo.uCalcInfo;
//
//	// DBオブジェクト作成
//	rs = virGetDataTableObject();
//
//	// uc_lst_item_sortを参照して必要情報を取得
//	CmnUcLstItemSortGetData(pCdlgSort->m_Sort1, &uItemSort);
//
//	// 集計後、値セット
//// 20-0450 del -->
//	//rs->CalcSykeiAfterSort(uCalcInfo, uItemSort.strFieldSykei, uItemSort.strFieldSykei2);
//// 20-0450 del <--
//// 20-0450 add -->
//	int		sw=0;
//	if(bG_Kanso == TRUE && m_uInfo.intFormSeq == ID_FORMNO_061 && 
//		uItemSort.strFieldSykei.Compare(_T("Syurui")) == 0)	{
//		if(uItemSort.intItemSeq == 2)	{
//			// 種類
//			sw=1;
//		}
//		else	{
//			// 銘柄
//			sw=2;
//		}
//	}
//	rs->CalcSykeiAfterSort(uCalcInfo, uItemSort.strFieldSykei, uItemSort.strFieldSykei2, sw);
//// 20-0450 add <--
//
//	delete rs;
//
//	return FUNCTION_OK;
//}
// 14-1821_15-1030 del <--

// 14-1821_15-1030 del -->
//#include "dbUc021UketoritegataEx.h"
// 14-1821_15-1030 del <--

//********************************************************************************
//	ソート：ソート処理（メイン）
//		IN		CdlgSort*		ソートダイアログ情報
//		RET		int				実行結果（FUNCTION_OK, FUNCTION_NG）
//********************************************************************************
int CfrmUc000Common::CmnTblSortMainProc(CdlgSort* pCdlgSort)
{
	// ソート実行前のプレチェック
	if (CmnTblSortSubPreCheck(pCdlgSort) != FUNCTION_OK) {
		return	FUNCTION_NG;
	}
	
	// ソート実行の前処理 ･･･ 小計/中計等の不要データ削除＋一括データを通常データに戻す
	if (CmnTblSortSubSortBeforeProc(pCdlgSort) != FUNCTION_OK) {
		return	FUNCTION_NG;
	}

	// 帳表毎のテーブルクラスを"New"する
	CdbUc000Common*		rsData;
	if( m_uInfo.intFormSeq == ID_FORMNO_021){
		// ②受取手形の内訳書にKnOrderが"Val"より後ろに追加されたため、GetFieldValueでエラーが発生する。
		// 原因はGetFieldValueがインデックスを昇順でのみ値を取得するからである。
		rsData = (CdbUc000Common*)( new CdbUc021UketoritegataEx( m_pDB ) );
	}
// 改良No.21-0086,21-0529 add -->
	else if(m_uInfo.intFormSeq == ID_FORMNO_081) {
		rsData = (CdbUc000Common*)( new CdbUc081SiharaitegataEx( m_pDB ) );
	}
// 改良No.21-0086,21-0529 add <--
	else{
		rsData = virGetDataTableObject();
	}

	// ソート実行
	if (virTblSortSubSortProc(pCdlgSort, rsData) != FUNCTION_OK) {
		rsData->Fin();
		delete rsData;
		return	FUNCTION_NG;
	}

	// ソート実行の後処理･･･改頁挿入，小計行挿入，暫定頁設定
	if (CmnTblSortSubSortAfterProc(pCdlgSort, rsData) != FUNCTION_OK) {
		rsData->Fin();
		delete rsData;
		return	FUNCTION_NG;
	}

	rsData->Fin();
	delete rsData;
	return	FUNCTION_OK;
}

//********************************************************************************
//	ソート：処理実行前のプレチェック
//		IN		CdlgSort*		ソートダイアログ情報
//		RET		int				実行結果（FUNCTION_OK, FUNCTION_NG）
//********************************************************************************
int CfrmUc000Common::CmnTblSortSubPreCheck(CdlgSort* pCdlgSort)
{
	ITEMSORT_INFO	uItemSort;
	int				intRet = FUNCTION_NG;

	// 第１ソート項目：必須
	if (pCdlgSort->m_Sort1 <= 0) {
		return FUNCTION_NG;
	}

	// 第１ソート項目：uc_lst_item_sortのソート条件登録済み？
	if (CmnUcLstItemSortGetData(pCdlgSort->m_Sort1, &uItemSort) == TRUE) {
		if (uItemSort.strOrderStr != "") {
			intRet = FUNCTION_OK;
		}
	}
	return intRet;
}

//********************************************************************************
//	ソート：初期データ設定
//		IN		CdlgSort*		ソートダイアログ情報
//		RET		int				実行結果（FUNCTION_OK, FUNCTION_NG）
//********************************************************************************
int CfrmUc000Common::CmnTblSortSubSortBeforeProc(CdlgSort* pCdlgSort)
{
	CdbUc000Common*		rsData;
	CString				str1;

	// 帳表テーブルをデータのみとする
	//	→ "FgFunc>200"は帳表固有データのため扱いに注意する事
	CmnTblDeleteFgFunc(ID_FGFUNC_NULL);			// 空行
	CmnTblDeleteFgFunc(ID_FGFUNC_SYOKEI);		// 小計行
	CmnTblDeleteFgFunc(ID_FGFUNC_SYOKEINULL);	// 小計行（空行）
	CmnTblDeleteFgFunc(ID_FGFUNC_CHUKEI);		// 中計行
	CmnTblDeleteFgFunc(ID_FGFUNC_CHUKEINULL);	// 中計行（空行）
	CmnTblDeleteFgFunc(ID_FGFUNC_RUIKEI);		// 累計行
	CmnTblDeleteFgFunc(ID_FGFUNC_PAGEKEI);		// 頁計行
//	CmnTblDeleteFgFunc(ID_FGFUNC_IKKATUMANUAL);	// 一括集計金額行（手動）･･･ 現仕様では削除しない
	CmnTblDeleteFgFunc(ID_FGFUNC_IKKATUAUTO);	// 一括集計金額行（自動）
// midori 190505 add -->
	CmnTblDeleteFgFunc(ID_FGFUNC_KAMOKU);		// 科目行（見出し）
// midori 190505 add <--

	// 帳表毎のテーブルクラスを"New"する
	rsData = virGetDataTableObject();

	// 一括データを表示データとする（FgShow:2→0）･･･ 一時保管データは対象外
	// ※一括データだったデータが必ず後ろに並ぶように、頁番号を(最大値 + 1)に更新
	if (rsData->UpdateFgShowPageRow(ID_FGSHOW_IKKATU, ID_FGSHOW_OFF, ID_DATA_PAGE_IKKATU_TEMP, 0) != DB_ERR_OK) {
		rsData->Fin();
		delete rsData;
		return FUNCTION_NG;
	}

	// グループ番号をクリア
	//	　本来は"FgShow=2"だけ実行すれば良いが、一括集計処理でグループ番号を使って
	//	　抽出するため、より安全のために"FgShow:2→0"実行後に行う
	if (rsData->UpdateFgShowClearGroup(ID_FGSHOW_OFF) != DB_ERR_OK) {
		rsData->Fin();
		delete rsData;
		return FUNCTION_NG;
	}
	rsData->Fin();
	delete rsData;
	return	FUNCTION_OK;
}

//********************************************************************************
//	ソート：ソート条件取得
//		IN		CdlgSort*		ソートダイアログ情報
//				CString*		フィルタ条件
//				CString*		ソート条件
//		RET		int				実行結果（FUNCTION_OK, FUNCTION_NG）
//********************************************************************************
void CfrmUc000Common::CmnTblSortSubGetSortParam(CdlgSort* pCdlgSort, CString* str1, CString* str2)
{
	int					nTemp;
// midori 152137 add -->
	int					kmksort = 0;	// 参照ダイアログ::表示順（科目）	0:番号順	1:50音順
	int					sw = 0;
// midori 152137 add <--
	CString				strFilter;
	CString				strSort;
// midori 152137 add -->
	CString				strBuf=_T("");
// midori 152137 add <--
	ITEMSORT_INFO		uItemSort;
	BYTE				FgItemSort1;
	BOOL				blnFlag = FALSE;

	// 第１ソート項目の項目種類 ･･･ 他でも使っているけどまたここで取得
	CmnUcLstItemSortGetData(pCdlgSort->m_Sort1, &uItemSort);

// midori 152137 add -->
	// 参照ダイアログ::表示順の取得
	kmksort = m_clsFunc.IsKamokuSort(m_uInfo.intFormSeq,m_pDB);
// midori 152137 add <--

	// 第ｎソート項目からソート順を取得
	strSort = "";
	for (int i = 0; i < 3; i++) {
		// ソート指定項目を取得
		switch (i) {
		case	0:
			nTemp = pCdlgSort->m_Sort1;

			// 第１ソート項目のFgItem(項目種)を格納
			FgItemSort1 = uItemSort.bytFgItem;
			break;
		case	1:
			// ダイアログで同じソート順が選択されるとSQLでエラーとなる
			nTemp = 0;
			if (pCdlgSort->m_Sort1 != pCdlgSort->m_Sort2) {
				nTemp = pCdlgSort->m_Sort2;
			}
			break;
		case	2:
			// ダイアログで同じソート順が選択されるとSQLでエラーとなる
			nTemp = 0;
			if ((pCdlgSort->m_Sort1 != pCdlgSort->m_Sort3) && (pCdlgSort->m_Sort2 != pCdlgSort->m_Sort3)) {
				nTemp = pCdlgSort->m_Sort3;
			}
			break;
		}
		
// midori 180537 del -->
//		// ソート指定ありの場合uc_lst_item_sortのソート条件を取得
//		if (nTemp > 0) {
// midori 180537 del <--
// midori 180537 add -->
		// 「なし」が選択されている場合、処理から抜ける
		if(nTemp == 0)	break;
// midori 180537 add <--
		CmnUcLstItemSortGetData(nTemp, &uItemSort);
// midori 152137 del -->
		//if (uItemSort.strOrderStr != "") {
		//	// ソート条件を設定
		//	if (strSort == "") {
		//		strSort = uItemSort.strOrderStr;
		//	}
		//	else {
		//		strSort = strSort + ", " + uItemSort.strOrderStr;
		//	}
		//}
		//else {
		//	break;
		//}
// midori 152137 del <--
// midori 152137 add -->
		sw = 0;
		switch(m_clsFunc.GetSortItem(m_uInfo.intFormSeq,nTemp))	{
		case 1:	// 科目（カナ入力有）
			// 0:番号順 1:50音順
			if(kmksort == 1)				sw = 1;
			break;
		case 2:	// 銀行（カナ入力有）
			// 0:番号順 1:50音順
			if(m_BankSort == 1)				sw = 1;
			break;
		case 3:	// 取引先（カナ入力有）
			// 0:番号順 1:50音順
// midori 152745 del -->
//				if(m_AddressSort == 1)			sw = 1;
// midori 152745 del <--
// midori 152745 add -->
			switch(m_uInfo.intFormSeq)	{
				// ② 受取手形の内訳書
				case	ID_FORMNO_021:
				// ③ 売掛金（未収入金）の内訳書
				case	ID_FORMNO_031:
				// ④－２ 貸付金及び受取利息の内訳書
				case	ID_FORMNO_042:
				// ⑩－１ 仮受金（前受金・預り金）の内訳書
				case	ID_FORMNO_101:
				// ⑯－２ 雑損失等の内訳書
				case	ID_FORMNO_162:
					// 得意先を参照
					if(m_AddressSort[1] == 1)			sw = 1;
					break;
				// ④－１ 仮払金（前渡金）の内訳書
				// ⑧ 支払手形の内訳書
				// ⑨ 買掛金（未払金・未払費用）の内訳書
				// ⑯－１ 雑益、雑損失等の内訳書
				default:
					// 仕入先を参照
					if(m_AddressSort[0] == 1)			sw = 1;
					break;
			}
// midori 152745 add <--
			break;
		default:
			break;
		}

		if(sw == 0)		strBuf = uItemSort.strOrderStr;
		else			strBuf = uItemSort.strOrderStr2;
		if (strBuf != "") {
			// ソート条件を設定
			if (strSort == "") {
				strSort = strBuf;
			}
			else {
				strSort = strSort + ", " + strBuf;
			}
		}
		else {
			break;
		}
// midori 152137 add <--
// midori 180537 del -->
//		}
// midori 180537 del <--

		// 第一ソート項目が科目だった場合、次のソート条件にFgFuncを指定
		if ( ( i == 0 ) && ( FgItemSort1 == ID_FGITEM_KAMOKU ) ){
			// 第１ソート項目が科目の場合
			if (strSort == "") {
				strSort = _T("FgFunc ASC");
			}
			else {
				strSort = strSort + _T(", FgFunc ASC");
			}
			// フラグをON(以下の処理ではもう"FgFunc ASC"は付加しない
			blnFlag = TRUE;
		}
	}
	
	// 表示データのみでソート実行（FgShow=0）
	//	　科目指定：手動一括集計金額(21)を通常データ(0)の科目が一致するがでも最後に表示（"FgFunc"が後で必要）
	//	　科目なし：通常データ(0)→手動一括集計金額(21)として手動データは最後に表示（"FgFunc"が先に必要）
	strFilter.Format("FgShow = %d", ID_FGSHOW_OFF);
	if (FgItemSort1 == ID_FGITEM_KAMOKU) {
		// 既に"FgFunc ASC"を付加済み?
		if ( blnFlag != TRUE ){
			// 第１ソート項目が科目の場合
			if (strSort == "") {
				strSort = _T("FgFunc ASC");
			}
			else {
				strSort = strSort + _T(", FgFunc ASC");
			}
		}
	}
	else{
		// 第１ソート項目が科目以外の場合
		if (strSort == "") {
			strSort = _T("FgFunc ASC");
		}
		else {
			strSort = _T("FgFunc ASC, ") + strSort;
		}
	}

	// ソートしたデータが、入力順に並ぶよう対応
	// ※指定しない場合、並び順が保障されないため
	if (strSort == "") {
		strSort = _T("NumPage ASC, NumRow ASC");
	}
	else {
		strSort = strSort + _T(", NumPage ASC, NumRow ASC");
	}

	// ソート条件
	*str1 = strFilter;
	*str2 = strSort;
}

//********************************************************************************
//	ソート：ソート後の一括集計金額行＋小計行＋改頁挿入処理
//		IN		CdlgSort*		ソートダイアログ情報
//				CdbUc000Common* テーブル情報（ソート実行済み状態）
//		RET		int				実行結果（FUNCTION_OK, FUNCTION_NG）
//********************************************************************************
int CfrmUc000Common::CmnTblSortSubSortAfterProc(CdlgSort* pCdlgSort, CdbUc000Common* rsData)
{
	ITEMSORT_INFO		uItemSort;					// 第１ソート項目のソート情報
	CString				strOldItem1 = "";			// 第１ソート項目の値（old）
	CString				strNewItem1 = "";			// 第１ソート項目の値（new）
	CString				strOldItem2 = "";			// 第１ソート項目の値（old）
	CString				strNewItem2 = "";			// 第１ソート項目の値（new）
	BOOL				bChangeItem = FALSE;		// 第１ソート項目の変化フラグ
// midori 190505 add -->
	BOOL				bKmkTitle = FALSE;			// 科目(見出し)行の挿入サイン
	CString				strKmkName = "";			// 科目(見出し)行に付加する科目名
// midori 190505 add <--
// 157154 add -->
	CString				strkei1=_T("");				// 科目名付き小計名称（1行目）
	CString				strkei2=_T("");				// 科目名付き小計名称（2行目）
// 157154 add <--
	SORTKAMOKU_INFO		uOldKamoku;					// 前レコードの科目情報
	SORTKAMOKU_INFO		uNullKamoku;				// 空の科目情報
	CString				strSykeiName = "";			// カッコ付き小計名称
	BOOL				bIkkatu = FALSE;			// 一括集計対象の判断
	CString				strVal;						// 一括集計で使用する金額値
	CString				strTemp;
	int					nNumGroup = 1;				// 一括集計で使用するグループ番号
	int					nIkkatuLine = 0;			// 一括集計の対象行
	int					nPage = 1;					// 暫定頁設定で使用する頁番号
	int					nRow = 0;					// 暫定頁設定で使用する行番号
	int					nSyokeiLine = 0;			// 小計対象行数
	int					nKaiPageLine = 0;			// 改頁対象行数
	int					i = 0;
// 158110 add -->
	int					bikt=FALSE;
	int					nIkkatuLineAll=0;			// 一括集計の対象行（②、⑧用）
// 158110 add <--
// 20-0450 add -->
	int					sw=0;
	CUcFunctionCommon	uc;
	CStringArray		sa1;
	CStringArray		sa2;
// 20-0450 add <--
// 20-0873 add -->
	int					amsw=0;
// 158063 del -->
	//CdlgTorihikiAmount1	amount1;
	//CdlgTorihikiAmount2	amount2;
// 158063 del <--
// 20-0873 add <--

	// 全行が空行なら"EOF"のため処理しない
	if (rsData->IsEOF()) {
		return	FUNCTION_OK;
	}

	// uc_lst_item_sortを参照して必要情報を取得
	CmnUcLstItemSortGetData(pCdlgSort->m_Sort1, &uItemSort);

	// カッコ付き小計名称を取得
	strSykeiName = CmnGetSykeiNameWithKakko(pCdlgSort->m_OpSykeiName, pCdlgSort->m_OpLeft, pCdlgSort->m_OpRight);

	// 科目情報を初期化
	uOldKamoku.intKnSeq = 0;
	uOldKamoku.intKnOrder = 0;
	uOldKamoku.strKnName = _T("");
// midori 152137 add -->
	uOldKamoku.strKnKana = _T("");
// midori 152137 add <--

	uNullKamoku = uOldKamoku;

// midori 190505 add -->
	bKmkTitle = FALSE;
// midori 190505 add <--

	// １件目の第１ソート項目の値を"old"へセット
	strOldItem1 = rsData->GetFieldValueString(uItemSort.strFieldSykei);
	strOldItem2 = rsData->GetFieldValueString(uItemSort.strFieldSykei2);

// 20-0873 add -->
	if(pCdlgSort->m_OpIkkatu == 1)	{
		amsw = pCdlgSort->IsTorihikiAmount(m_uInfo.intFormSeq);
		if(amsw != 0)	{
			// ②受取手形の内訳書、⑧支払手形の内訳書は総額のマップを作成する
// 158063 del -->
			//if(amsw == 1)	amount1.MakeMap(m_pDB,m_uInfo.intFormSeq);
			//else			amount2.MakeMap(m_pDB,m_uInfo.intFormSeq);
// 158063 del <--
// 158063 add -->
			if(amsw == 1)	uc.MakeAmountMap1(m_pDB,m_uInfo.intFormSeq);
			else			uc.MakeAmountMap2(m_pDB,m_uInfo.intFormSeq);
// 158063 add <--
		}
	}
// 20-0873 add <--

	while (!rsData->IsEOF()) {
		// 暫定：頁設定（改ページによる空行の数を把握したい）→ 後で正式な頁設定を行う
		if (rsData->m_FgShow == ID_FGSHOW_OFF) {
			// 第１ソート項目が科目なら、科目情報を保存しておく ･･･ 科目でソートなら一括集計行の科目を付加する＋⑪は小計名称に科目名を入れる
			if (uItemSort.bytFgItem == ID_FGITEM_KAMOKU) {
				// フィールド名が統一されているので、これで問題なし
				virTblGetKamokuInfo(rsData, &uOldKamoku);
			}

			// 一括集計対象を判断 ･･･ 手動一括集計金額行は対象外とする
			bIkkatu = FALSE;
			if ((pCdlgSort->m_OpIkkatu == 1) && (rsData->m_FgFunc != ID_FGFUNC_IKKATUMANUAL)) {
				// 対象フィールドから値を取り出し、金額の大小判断を行う（未満で判断）
// 158096 del -->
//// 20-0873 del -->
//				//strVal = rsData->GetFieldValueString(SORT_IKKATU_FIELD);
//// 20-0873 del <--
//// 20-0873 add -->
//				if(amsw != 0 && (m_uInfo.intFormSeq == ID_FORMNO_021 || m_uInfo.intFormSeq == ID_FORMNO_081))	{
//					// 取引先毎の総額をマップから取得する
//// 158063 del -->
//					//if(amsw == 1)	strVal = amount1.GetAmount(virTblSortGetTorihikisakiCode(rsData),virTblSortGetTorihikisakiName(rsData));
//					//else			strVal = amount2.GetAmount(virTblSortGetTorihikisakiCode(rsData),virTblSortGetTorihikisakiName(rsData),virTblSortGetKamokuCode(rsData));
//// 158063 del <--
//// 158063 add -->
//					if(amsw == 1)	strVal = uc.GetAmount1(virTblSortGetTorihikisakiCode(rsData),virTblSortGetTorihikisakiName(rsData));
//					else			strVal = uc.GetAmount2(virTblSortGetTorihikisakiCode(rsData),virTblSortGetTorihikisakiName(rsData),virTblSortGetKamokuCode(rsData));
//// 158063 add <--
//				}
//				else	{
//					strVal = rsData->GetFieldValueString(SORT_IKKATU_FIELD);
//				}
//// 20-0873 add <--
//				CString ikkatu_val = pCdlgSort->GetIkkatuVal(rsData->GetSortIkkatuOrder());
//
//				if (CmnKingakuCmpFunc(strVal, ikkatu_val) < 0) {
//					// 一括集計対象
//					bIkkatu = TRUE;
//					nIkkatuLine = nIkkatuLine + 1;
//				}
// 158096 del <--
// 158096 add -->
				CString	sougaku=_T("");
				strVal = rsData->GetFieldValueString(SORT_IKKATU_FIELD);
				if(amsw != 0 && (m_uInfo.intFormSeq == ID_FORMNO_021 || m_uInfo.intFormSeq == ID_FORMNO_081))	{
					// 取引先毎の総額をマップから取得する
					if(amsw == 1)	sougaku = uc.GetAmount1(virTblSortGetTorihikisakiCode(rsData),virTblSortGetTorihikisakiName(rsData));
					else			sougaku = uc.GetAmount2(virTblSortGetTorihikisakiCode(rsData),virTblSortGetTorihikisakiName(rsData),virTblSortGetKamokuCode(rsData));
				}
				else	{
					sougaku = strVal;
				}
				CString ikkatu_val = pCdlgSort->GetIkkatuVal(rsData->GetSortIkkatuOrder());
				if(CmnKingakuCmpFunc(sougaku,ikkatu_val,strVal) < 0)	{
					// 一括集計対象
					bIkkatu = TRUE;
					nIkkatuLine = nIkkatuLine + 1;
// 158110 add -->
					bikt=TRUE;
// 158110 add <--
				}
// 158096 add <--
			}

// midori 190505 add -->
			// 科目(見出し)行の挿入
// 改良No.21-0086,21-0529 cor -->
//// midori 157112 add -->
//			if(KamokuRowEnableSgn(m_pDB,0) == 1) {
//// midori 157112 add <--
//				if((m_uInfo.intFormSeq == ID_FORMNO_021) || (m_uInfo.intFormSeq == ID_FORMNO_111))	{
// ------------------------------
			if(KamokuRowEnableSgn(m_pDB, 0, m_uInfo.intFormSeq) == 1)	{
				if(m_uInfo.intFormSeq == ID_FORMNO_021 || (bG_InvNo == TRUE && m_uInfo.intFormSeq == ID_FORMNO_081) || m_uInfo.intFormSeq == ID_FORMNO_111)	{
// 改良No.21-0086,21-0529 cor <--
					if((uItemSort.bytFgItem == ID_FGITEM_KAMOKU) && (pCdlgSort->m_OpKmkTitle == 1))	{
						// ※一括行に該当するデータは【小計】を挿入する場合のみ
						if(bKmkTitle == FALSE && (bIkkatu == FALSE || pCdlgSort->m_OpSykeiAuto != 0))	{
							// 科目(見出し)データ：頁＋行の連番を設定
							CmnGetNextPageRow(&nPage,&nRow);
							// 科目名を取得
							strKmkName = virTblSortGetKamokuName(rsData);
							// 科目行追加
							CmnTblInsertSpecialLine(nPage,nRow,ID_FGFUNC_KAMOKU,strKmkName);
							// 科目(見出し)行の挿入をONにする
							bKmkTitle = TRUE;
						}
					}
				}
// midori 157112 add -->
			}
// midori 157112 add <--
// midori 190505 add <--

			if (bIkkatu == TRUE) {
				// 一括対象データ：頁＋行はクリア，更にグループ番号を設定
				rsData->Edit();
				rsData->m_NumPage = 0;
				rsData->m_NumRow = 0;
				rsData->m_FgShow = ID_FGSHOW_IKKATU;
				rsData->m_NumGroup = nNumGroup;
				rsData->Update();
			}
			else {
				// 通常データ：頁＋行の連番を設定
				CmnGetNextPageRow(&nPage, &nRow);

				rsData->Edit();
				rsData->m_NumPage = nPage;
				rsData->m_NumRow = nRow;
				rsData->Update();

				nSyokeiLine = nSyokeiLine + 1;			// 小計対象行数を加算
				nKaiPageLine = nKaiPageLine + 1;		// 改頁対象行数を加算
			}
		}

		// 次のレコードへ
		rsData->MoveNext();
		
		// 第１ソート項目の値を"new"へセット
		if (!rsData->IsEOF()) {

// 158110 add -->
			// 以下の条件をすべて満たす場合、次のレコードが一括金額の対象となる可能性があるため一括金額対象外のデータまで読み込む
			// ・②受取手形の内訳書、もしくは⑧支払手形の内訳書
			// ・第１ソート項目が、②受取手形の場合は「振出人」、⑧支払手形の場合は「支払先」
			// ・一括金額を行うチェックＯＮ
			// ・詳細設定を使用するチェックＯＮ
			// 例)	詳細設定	受取手形　1,000,000
			//					割引手形　        0
			//					裏書手形　  500,000
			//		データ		1行目　割引手形　振出人Ａ    50,000 → 一括金額対象外
			// 					2行目　裏書手形　振出人Ａ   100,000 → 一括金額対象
			//					3行目　受取手形　振出人Ｂ 1,200,000 → 一括金額対象外
			// 上記の場合、2行目のデータは出力されないので1行目と3行目を比較して第１ソート項目が変わったかの判定を行う必要がある
			int		iseof=0;
			int		rcnt=0;
			if(amsw == 2 && uItemSort.intItemSeq == 1)	{
				// 現状、amsw=2 になるのは②受取手形の内訳書か⑧支払手形の内訳書で詳細設定をＯＮにしている場合のみ
				CString	sv=_T(""),mv=_T(""),iv=_T("");
				while(!rsData->IsEOF())	{
					// 明細金額を取得する（一括金額に０円を含まないチェックＯＮの場合に必要）
// 改良No.21-0086,21-0529 cor -->
					//mv = ((CdbUc021UketoritegataEx *)rsData)->m_Val;
// ------------------------------
					if(m_uInfo.intFormSeq == ID_FORMNO_021) {
						mv = ((CdbUc021UketoritegataEx *)rsData)->m_Val;
					}
					else if(m_uInfo.intFormSeq == ID_FORMNO_081) {
						mv = ((CdbUc081SiharaitegataEx *)rsData)->m_Val;
					}
// 改良No.21-0086,21-0529 cor <--
					// 取引先毎の総額をマップから取得する
					sv = uc.GetAmount2(virTblSortGetTorihikisakiCode(rsData),virTblSortGetTorihikisakiName(rsData),virTblSortGetKamokuCode(rsData));
					// 一括金額を取得する
					iv = pCdlgSort->GetIkkatuVal(rsData->GetSortIkkatuOrder());
					if(CmnKingakuCmpFunc(sv,iv,mv) >= 0)	{
						// 一括金額の対象外
						break;
					}
					rsData->MoveNext();
					rcnt++;
				}
				if(rsData->IsEOF())	{
					iseof=1;
				}
			}
			if(iseof != 0)	{
				// 以降のデータがすべて一括金額対象の場合、１ソート項目が変わった事にしておく
				bChangeItem = TRUE;
			}
			else	{
// 158110 add <--
				strNewItem1 = rsData->GetFieldValueString(uItemSort.strFieldSykei);
				strNewItem2 = rsData->GetFieldValueString(uItemSort.strFieldSykei2);

				// "old"と"new"を比較：第１ソート項目が変わった？
				bChangeItem = FALSE;
				sw = 0;
				if(bG_Kanso == TRUE && m_uInfo.intFormSeq == ID_FORMNO_061 && 
					uItemSort.strFieldSykei.Compare(_T("Syurui")) == 0)	{
					// 簡素化様式の⑥有価証券の内訳書は種類・銘柄別に比較する
					uc.StrDivision(strOldItem1,&sa1,2);
					uc.StrDivision(strNewItem1,&sa2,2);
					if(uItemSort.intItemSeq == 2)	{
						// 種類
						if(sa1.GetAt(0) != sa2.GetAt(0))	{
							sw = 1;
						}
					}
					else	{
						// 銘柄
						if(sa1.GetAt(1) != sa2.GetAt(1))	{
							sw = 1;
						}
					}
				}
				else	{
					if((strOldItem1 != strNewItem1) || (strOldItem2 != strNewItem2))	{
						sw = 1;
					}
				}
				if(sw != 0)	{
					// 変化した
					bChangeItem = TRUE;
					bKmkTitle = FALSE;	// 科目(見出し)行の挿入をOFFにする
				}
// 158110 add -->
			}
			if(rcnt > 0)	{
				// レコードを先に進めた場合は戻す
				for(int ii=0; ii<rcnt; ii++)	{
					rsData->MovePrev();
				}
				// 比較が終わったので戻しておく
				strNewItem1 = rsData->GetFieldValueString(uItemSort.strFieldSykei);
				strNewItem2 = rsData->GetFieldValueString(uItemSort.strFieldSykei2);
			}
// 158110 add <--

		}
		else {
			// EOFの場合、１ソート項目が変わった事にしておく
			bChangeItem = TRUE;
		}

// 158110 del -->
		//// 第１ソート項目が変わった？orレコード終了？
		//if ((bChangeItem == TRUE) || (rsData->IsEOF())) {
// 158110 del <--
// 158110 add -->
		// 第１ソート項目が変わった？orレコード終了？
		sw=0;
		if((bChangeItem == TRUE) || (rsData->IsEOF()))	{
			sw=1;
		}
// 158124 del -->
		//if(sw != 0)	{
		//	// ②受取手形の内訳書、⑧支払手形の内訳書で第１ソート項目を「振出人（支払先）」にしている場合
		//	// 一括金額は最終ラインに挿入するため、項目が変わったと判定しない
		//	if(amsw != 0 && (m_uInfo.intFormSeq == ID_FORMNO_021 || m_uInfo.intFormSeq == ID_FORMNO_081))	{
		//		if(uItemSort.intItemSeq == 1)	{
		//			if(bikt != FALSE)	{
		//				sw=0;
		//				bikt=FALSE;
		//				strOldItem1 = strNewItem1;
		//				strOldItem2 = strNewItem2;
		//				nIkkatuLineAll = nIkkatuLineAll + nIkkatuLine;
		//				nIkkatuLine = 0;
		//			}
		//		}
		//	}
		//}
// 158124 del <--
// 158124 add -->
		// 一括金額の対象であれば
		if(bikt != FALSE)	{
			// ②受取手形の内訳書、⑧支払手形の内訳書で第１ソート項目を「振出人（支払先）」にしている場合
			if(amsw != 0 && (m_uInfo.intFormSeq == ID_FORMNO_021 || m_uInfo.intFormSeq == ID_FORMNO_081))	{
				if(uItemSort.intItemSeq == 1)	{
					// 一括金額は最終ラインに挿入するため、項目が変わったと判定しない
					if(sw != 0)	{
						sw=0;
						// "new"→"old"
						if (!rsData->IsEOF()) {
							strOldItem1 = strNewItem1;
							strOldItem2 = strNewItem2;
						}
					}
					// 「その他○件」を加算
					nIkkatuLineAll = nIkkatuLineAll + nIkkatuLine;
					nIkkatuLine = 0;
				}
			}
			// 一括金額判定フラグをOFFにする
			bikt=FALSE;
		}
// 158124 add <--
		if(sw != 0)	{
// 158110 add <--
			// 自動一括集計金額行の挿入：ダイアログの設定あり
			if ((pCdlgSort->m_OpIkkatu == 1) && (nIkkatuLine > 0)) {
				// 小計あり：１ソート項目が変化したタイミングで挿入
				// 小計なし：最後レコードに挿入
				if (((pCdlgSort->m_OpSykeiAuto == 1) && (bChangeItem == TRUE)) || (rsData->IsEOF())) {
// midori 151400 add -->
					// 小計無し
					if(pCdlgSort->m_OpSykeiAuto == 0)	{
						// 改頁の挿入：ダイアログの設定あり
						if ((pCdlgSort->m_OpKaiPage == 1) && (nKaiPageLine > 0)) {
							// 
							for (i = nRow + 1; i <= m_uInfo.intRowMaxData; i++) {
								// 空行の頁と行番号を取得
								CmnGetNextPageRow(&nPage, &nRow);
								
								// 対象頁を空行で埋める
								CmnTblInsertNullLine(nPage, nRow, 1);
							}

							// 改頁対象行数をクリア
							nKaiPageLine = 0;
						}
					}
// midori 151400 add <--
					// 一括対象レコード作成
					if ((pCdlgSort->m_OpSykeiAuto == 1) && (uItemSort.bytFgItem == ID_FGITEM_KAMOKU)) {
						// 小計ありand科目指定の場合、一括レコードは科目情報付きとする
						CmnTblIkkatuAutoUpdateProc(0, nNumGroup, &nPage, &nRow, uOldKamoku);
					}
					else {
// midori 190505 add -->

// 改良No.21-0086,21-0529 cor -->
//// midori 157112 add -->
//						if(KamokuRowEnableSgn(m_pDB,0) == 1) {
//// midori 157112 add <--
//							// 最終に出力される［その他○件］従来の仕様に合わせて科目の見出しを出力する
//							if((m_uInfo.intFormSeq == ID_FORMNO_021) || (m_uInfo.intFormSeq == ID_FORMNO_111))	{
// ------------------------------
						if(KamokuRowEnableSgn(m_pDB, 0, m_uInfo.intFormSeq) == 1)	{
							// 最終に出力される［その他○件］従来の仕様に合わせて科目の見出しを出力する
							if(m_uInfo.intFormSeq == ID_FORMNO_021 || (bG_InvNo == TRUE && m_uInfo.intFormSeq == ID_FORMNO_081) || m_uInfo.intFormSeq == ID_FORMNO_111)	{
// 改良No.21-0086,21-0529 cor <--
								if((uItemSort.bytFgItem == ID_FGITEM_KAMOKU) && (pCdlgSort->m_OpKmkTitle == 1))	{
									// 科目(見出し)データ：頁＋行の連番を設定
									CmnGetNextPageRow(&nPage,&nRow);
									// 科目行追加
									CmnTblInsertSpecialLine(nPage,nRow,ID_FGFUNC_KAMOKU,_T(""));
								}
							}
// midori 157112 add -->
						}
// midori 157112 add <--
// midori 190505 add <--
						// 一括レコードに科目情報は不要
						CmnTblIkkatuAutoUpdateProc(0, nNumGroup, &nPage, &nRow, uNullKamoku);
					}

					// 次のデータに向けてクリア
					nNumGroup = nNumGroup + 1;			// 一括対象グループ番号を加算
					nIkkatuLine = 0;					// 一括対象行数をクリア

					nSyokeiLine = nSyokeiLine + 1;			// 小計対象行数を加算
					nKaiPageLine = nKaiPageLine + 1;		// 改頁対象行数を加算
				}
			}

			// 小計行の挿入：ダイアログの設定あり＋小計対象項目である
			if ((pCdlgSort->m_OpSykeiAuto == 1) && (uItemSort.bytFgSykei == 1)) {
				// 特例処理：⑪借入金（2016/01/06②受取手形の内訳書も追加）且つ第１ソートが科目の場合、小計名称を科目名付きとする	
// 改良No.21-0086,21-0529 cor -->
				//if (((m_uInfo.intFormSeq == ID_FORMNO_111) || (m_uInfo.intFormSeq == ID_FORMNO_021)) && (uItemSort.bytFgItem == ID_FGITEM_KAMOKU)) {
// ------------------------------
				if((m_uInfo.intFormSeq == ID_FORMNO_021 || (bG_InvNo == TRUE && m_uInfo.intFormSeq == ID_FORMNO_081) || m_uInfo.intFormSeq == ID_FORMNO_111) 
				&& (uItemSort.bytFgItem == ID_FGITEM_KAMOKU))	{
// 改良No.21-0086,21-0529 cor <--
					CdbUcInfSub mfcRecSub(m_pDB);
					if( mfcRecSub.RequeryFormSeq( m_uInfo.intFormSeq ) == DB_ERR_OK ){
						if( !(mfcRecSub.IsEOF()) ){
							CCtrlSubGeneralVar cs( mfcRecSub );
							if( mfcRecSub.m_GeneralConstVal[15] & BIT_D1 ){
// midori 157099,157119 del -->
//// midori 157042 del -->
////// midori 190505 del -->
////								//if( !(cs.bOutKamokuRow) ){
////// midori 190505 del <--
////// midori 190505 add -->
////								if( !(cs.bKamokuTitle) ){
////// midori 190505 add <--
//// midori 157042 del <--
//// midori 157042 add -->
//								if( !(cs.bOutKamokuRow) ){
//// midori 157042 add <--
// midori 157099,157119 del <--
// midori 157099,157119 add -->
								if( !(cs.bKamokuTitle) ){
// midori 157099,157119 add <--
									strSykeiName = CmnGetSykeiNameWithKakko(pCdlgSort->m_OpSykeiName, pCdlgSort->m_OpLeft, 
										pCdlgSort->m_OpRight, uOldKamoku.strKnName);
// 157154 add -->
									m_clsFunc.KeiStrSprit(strSykeiName, &strkei1, &strkei2);
									if (strkei2.IsEmpty() == FALSE)		strSykeiName = strkei1 + _T("\r\n") + strkei2;
// 157154 add <--
								}
							}
						}

						mfcRecSub.Fin();
					}
				}

				// データ２行以上 or ダイアログ：１行でも小計挿入オン
				if ((nSyokeiLine >= 2) || (nSyokeiLine == 1 && pCdlgSort->m_OpOutSyokei == 1)) {
					// 小計行の挿入：Sum値は後で集計する
					CmnGetNextPageRow(&nPage, &nRow);
					CmnTblInsertSpecialLine(nPage, nRow, ID_FGFUNC_SYOKEI, strSykeiName);

					// 空行の挿入：ダイアログで空行挿入する＋先頭行なら挿入しない
					if ((pCdlgSort->m_OpOutNullLine == 1) && (CmnGetNextFirstRow(nRow) == FALSE)) {
						// 小計行の後に空行を挿入
						CmnGetNextPageRow(&nPage, &nRow);
						CmnTblInsertNullLine(nPage, nRow, 1);
					}
				}
				// ダイアログ：１行でも小計挿入オフは、小計（空行）を挿入する
				else {
					// 小計（空行）行の挿入：Sum値は後で集計する
					CmnGetNextPageRow(&nPage, &nRow);
					// ページの頭に小計（空行）を挿入しない
					if( nRow != 1 ){
						CmnTblInsertSpecialLine(nPage, nRow, ID_FGFUNC_SYOKEINULL, "");
					}
// 14-1821_15-1030 del -->
					//else{
					//	m_NullSyokeiRow = TRUE;
					//}
// 14-1821_15-1030 del <--
				}

				// 小計対象行数をクリア
				nSyokeiLine = 0;
			}

			// 改頁の挿入：ダイアログの設定あり
			if ((pCdlgSort->m_OpKaiPage == 1) && (nKaiPageLine > 0)) {
				// 
				for (i = nRow + 1; i <= m_uInfo.intRowMaxData; i++) {
					// 空行の頁と行番号を取得
					CmnGetNextPageRow(&nPage, &nRow);
					
					// 対象頁を空行で埋める
					CmnTblInsertNullLine(nPage, nRow, 1);
				}

				// 改頁対象行数をクリア
				nKaiPageLine = 0;
			}

			// "new"→"old"
			if (!rsData->IsEOF()) {
				strOldItem1 = strNewItem1;
				strOldItem2 = strNewItem2;
			}
		}
	}
// 158110 add -->
	// ②受取手形の内訳書、⑧支払手形の内訳書の２様式は第１ソート項目が
	// 振出人（支払先）の場合、一括金額を最終ラインに挿入する
	if((pCdlgSort->m_OpIkkatu == 1) && (nIkkatuLineAll > 0))	{
		// 小計無し
		if(pCdlgSort->m_OpSykeiAuto == 0)	{
			// 改頁の挿入：ダイアログの設定あり
			if((pCdlgSort->m_OpKaiPage == 1) && (nKaiPageLine > 0))	{
				for(i=nRow + 1; i<=m_uInfo.intRowMaxData; i++)	{
					// 空行の頁と行番号を取得
					CmnGetNextPageRow(&nPage,&nRow);
					// 対象頁を空行で埋める
					CmnTblInsertNullLine(nPage,nRow,1);
				}
				// 改頁対象行数をクリア
				nKaiPageLine = 0;
			}
		}
		// 一括対象レコード作成
		CmnTblIkkatuAutoUpdateProc(0,nNumGroup,&nPage,&nRow,uNullKamoku);
	}
// 158110 add <--
	return	FUNCTION_OK;
}

//******************************************************************************************
//	一括金額追加
//	【引数】	なし
//	【戻値】	なし
//****************************************************************************************
void CfrmUc000Common::CmnTblIkkatuAddProc()
{
	CdbUc000Common*		rs;
	int intInsertType = 0;	// 1:一括金額　2:小計/小計空行 3:最終データ行
	int intOutPage;			// 自動一括金額行の加算・追加先の対象となる頁
	int intOutRow;			// 自動一括金額行の加算・追加先の対象となる行
	int intOutGroup;		// 自動一括行の対象グループ番号
	SORTKAMOKU_INFO		uKamoku;
	
	// カレント行（元データ）の科目情報を取得：自動一括金額を新規追加する場合に必要
	CmnTblIkkatuAddGetKamokuInfo(m_uInfo.intCurPage, m_uInfo.intCurRow, &uKamoku);

	rs = virGetDataTableObject();				// 全レコード取得用

	// カレント行以降のレコード抽出
	rs->RequeryPageRowAfterRecord( m_uInfo.intCurPage , m_uInfo.intCurRow );

	while (!rs->IsEOF()) {

		// 行種確認
		switch(rs->m_FgFunc)
		{
		// 自動一括金額行
		case ID_FGFUNC_IKKATUAUTO:
			intOutPage = rs->m_NumPage;
			intOutRow = rs->m_NumRow;
			intOutGroup = rs->m_NumGroup;
			intInsertType = 1;
			break;
		// 小計or小計空行
		case ID_FGFUNC_SYOKEI:
		case ID_FGFUNC_SYOKEINULL:
			intOutPage = rs->m_NumPage;
			intOutRow = rs->m_NumRow;
			intInsertType = 2;
			break;
		// データ行 or 中計行/中計空行 or 手動一括金額
		case ID_FGFUNC_DATA:
		case ID_FGFUNC_CHUKEI:
		case ID_FGFUNC_CHUKEINULL:
		case ID_FGFUNC_IKKATUMANUAL:
			intOutPage = rs->m_NumPage;
			intOutRow = rs->m_NumRow;
			intInsertType = 3;
			break;
		default:
			break;
		}

		// 自動一括金額行 or 小計行/小計空行ならばループを抜ける
		if (( intInsertType == 1 ) || ( intInsertType == 2 )){
			break;
		}
		// 次レコードへ
		rs->MoveNext();
	}
	rs->Fin();
	delete rs;

	// レコードが取得できなかった場合は処理を抜ける
	if ( ( intInsertType < 1  ) && ( intInsertType > 3 )){
		return;
	}

// midori 156968 add -->
	InsertNull();
// midori 156968 add <--

	// 分岐処理
	switch( intInsertType ){

	// カレント行を対象自動一括金額行に加算
	case 1:
		// カレント行を自動一括金額対象＆非表示に変更
		CmnTblIkkatuAddCurrentToIkkatu(m_uInfo.intCurPage, m_uInfo.intCurRow, intOutGroup);
		
		// 自動一括金額行を更新
		CmnTblIkkatuAutoUpdateProc( 1 , intOutGroup , &intOutPage , &intOutRow , uKamoku ); 
		break;
	// カレント行を小計行/小計空行の前に自動一括金額行として追加
	case 2:
	// カレント行を最終データ行の後に自動一括金額行として追加
	case 3:
		// 新規グループ番号を取得（最大グループ番号＋１）
		rs = virGetDataTableObject();				// 全レコード取得用
		intOutGroup = rs->GetNumGroup() + 1;
		rs->Fin();
		delete rs;

		// カレント行を自動一括金額対象＆非表示に変更
		CmnTblIkkatuAddCurrentToIkkatu(m_uInfo.intCurPage, m_uInfo.intCurRow, intOutGroup);

		// Case2: 小計行の位置に追加する場合は、既に挿入位置がわかっている
		// Case3: 最終データ行の後に追加する場合は、挿入位置を探す必要がある
		if (intInsertType == 3) {
			CmnGetNextPageRow(&intOutPage, &intOutRow);
		}

		// 自動一括金額行の追加前に、頁番号/行番号振り直し
		CmnTblUpdatePageNumRowNumAdd(intOutPage, intOutRow, 1);

		// 自動一括金額行を追加
		CmnTblIkkatuAutoUpdateProc(2, intOutGroup, &intOutPage, &intOutRow, uKamoku);
		break;
	}

	// 頁番号を振りなおす(カレント行を詰める)
	CmnTblDeleteFgFunc(ID_FGFUNC_PAGEKEI);											// 頁計の削除
	CmnTblDeleteFgFunc(ID_FGFUNC_RUIKEI);											// 累計の削除
	CmnTblUpdatePageNumRowNumAdd(m_uInfo.intCurPage, m_uInfo.intCurRow, -1);	// 頁番号/行番号振り直し
	CmnTblUpdateEndPageSetting();													// 最終頁を調整する
	CmnTblInsertPageKeiRuiKei();													// 頁計/累計挿入（最終ページの削除or空行追加）
	
	// 小計/中計/頁計/累計の再計算
	CmnTblCalcKei();

// midori 156968 add -->
	// 先頭行が空行の場合削除する
	CmnTblUpdatePageRowAllResetMain(6);
// midori 156968 add <--

	// １ページ再表示
	virUpdateControlTblData();
	

}
//********************************************************************************
//	通常/個別切り替えの有無を判断
//		IN		なし
//		RET		BOOL			TRUE:切り替えあり，FALSE:なし
//********************************************************************************
BOOL CfrmUc000Common::CmnGetChangeFormType()
{
	BOOL			bRet = FALSE;

	// uc_inf_form_typeを参照した方が汎用性があるかも
	switch (m_uInfo.intFormSeq) {
	case ID_FORMNO_041:			//	④-1
	case ID_FORMNO_042:			//	④-2
	case ID_FORMNO_101:			//	⑩-1
	case ID_FORMNO_102:			//	⑩-2
	case ID_FORMNO_141:			//	⑭-1
	case ID_FORMNO_142:			//	⑭-2
	case ID_FORMNO_151:			//	⑮-1
	case ID_FORMNO_152:			//	⑮-2
	case ID_FORMNO_153:			//	⑮-3
	case ID_FORMNO_161:			//	⑯-1
	case ID_FORMNO_162:			//	⑯-2
		// 通常/個別切り替え：あり
		bRet = TRUE;
		break;
	}
	return bRet;
}

//********************************************************************************
//	uc_inf_main：FormSeq番号情報の取得
//		IN		なし
//		RET		TRUE:成功，FALSE:失敗
//********************************************************************************
BOOL CfrmUc000Common::CmnUcInfMainGetInitInfo()
{
	CdbUcInfMain	rsMain(m_pDB);
	BOOL			blnRet = FALSE;			// 戻り値

	// uc_inf_sub参照
	if (rsMain.Init() == DB_ERR_OK) {
		if (!rsMain.IsEOF()) {
			// 初期情報を取得
			m_uInfo.intFormSeq = rsMain.m_FormSeq;							// FormSeq番号

			blnRet = TRUE;
// midori 160612 add -->
			// 「入力データを確定する」サインを取得する
			// ※データ表示までに必要なため、本来取得している場所とは別で、先に取得する
			m_DataKakutei = rsMain.m_GeneralVar[1] & BIT_D2;
// midori 160612 add <--
		}
	}
	rsMain.Fin();

	return blnRet;
}

//********************************************************************************
//	uc_inf_sub：カレント頁，行，列，計設定情報の取得
//		IN		なし
//		RET		TRUE:成功，FALSE:失敗
//********************************************************************************
BOOL CfrmUc000Common::CmnUcInfSubGetInitInfo()
{
	CdbUcInfSub		rsSub(m_pDB);
	BOOL			blnRet = FALSE;			// 戻り値

	// uc_inf_sub参照
	if (rsSub.RequeryFormSeq(m_uInfo.intFormSeq) == DB_ERR_OK) {
		if (!rsSub.IsEOF()) {
			// 初期情報を取得
//			m_uInfo.intCurPage = rsSub.m_CurPage;							// カレント頁番号
//			m_uInfo.intCurRow = rsSub.m_CurRow;								// カレント行番号
//			m_uInfo.intCurCol = rsSub.m_CurField;							// カレント列番号

			m_uInfo.intOutKei = rsSub.m_OutKei;								// 計設定内容
			m_uInfo.intFormNo = rsSub.m_FormNo;								// FormNo.
			m_uInfo.intFormNo2 = rsSub.m_FormNo2;							// FormNo2.

			m_uInfo.strTitleName = rsSub.m_Title;							// 帳表タイトル
			m_uInfo.strTitleNumber = rsSub.m_TitleNo + rsSub.m_TitleNo2;	// 帳表番号

			blnRet = TRUE;

			CCtrlSubGeneralVar cs(rsSub);
			m_IkkatuOutZero = cs.bIkkatuOutZero;
			m_NameKei		= cs.bNameKei;
		}
	}
	rsSub.Fin();

	return blnRet;
}

//********************************************************************************
//	uc_inf_sub：カレント頁，行，列，コントロール情報を保存
//		IN		なし
//		RET		なし
//********************************************************************************
void CfrmUc000Common::CmnUcInfSubSetCurrentInfo()
{
	CdbUcInfSub		uInfSub( m_pDB );	// uc_inf_sub
	
	// 対象様式のテーブルを開く
	if ( uInfSub.RequeryFormSeq( m_uInfo.intFormSeq ) == DB_ERR_OK ){
		// 前回起動時の情報を保存
		if ( !uInfSub.IsEOF() ) {

			uInfSub.Edit();

			uInfSub.m_CurPage = m_uInfo.intCurPage;				// カレント頁番号
			uInfSub.m_CurRow = m_uInfo.intCurRow;				// カレント行番号
			uInfSub.m_CurField = m_uInfo.intCurCol;				// カレント列番号
			uInfSub.m_CurCtlIndex = m_uInfo.intCurCtlIndex;		// カレントコントロールのインデックス番号

			// 更新
			uInfSub.Update();

			// 閉じる
			uInfSub.Fin();
		}
	}
}

//********************************************************************************
//	uc_inf_sub：タイトルを保存／タイトルを戻す
//		IN		BOOL			TRUE :タイトルを保存
//								FALSE:タイトルを戻す（出力形式の切り替えを含む）
//		RET		なし
//********************************************************************************
void CfrmUc000Common::CmnUcInfSubSetTitleInfo(BOOL bSetFlag)
{
	CdbUcInfSub		uInfSub( m_pDB );	// uc_inf_sub
	BOOL			bRet = TRUE;

	// 対象様式のテーブルを開く
	if (uInfSub.RequeryFormSeq(m_uInfo.intFormSeq) == DB_ERR_OK) {
		if (!uInfSub.IsEOF()) {
			uInfSub.Edit();

			if (bSetFlag == TRUE) {
				// タイトルを保存
				uInfSub.m_Title = m_clsFunc.DeleteRightSpace( m_uInfo.strTitleName );
				// 編集タイトル格納フィールドにタイトルを保存
				if (CmnGetChangeFormType() == TRUE) {
					// ④，⑩，⑭～⑯：出力形式の設定で処理が異なる（通常/個別）
					if (m_uInfo.intFormType == ID_FORMTYPE_KOBETU) {
						uInfSub.m_TitleSave = m_uInfo.strTitleName;			// 個別
					}
					else {
						uInfSub.m_TitleSaveTujo = m_uInfo.strTitleName;		// 通常
					}
				}
			}
			else {
				// タイトルを戻す
				if (CmnGetChangeFormType() == TRUE) {
					// ④，⑩，⑭～⑯：出力形式の設定で処理が異なる（通常/個別）
					if (m_uInfo.intFormType == ID_FORMTYPE_KOBETU) {
						uInfSub.m_Title = uInfSub.m_TitleOrg;			// 個別
					}
					else {
						uInfSub.m_Title = uInfSub.m_TitleOrgTujo;		// 通常
					}
				}
				else {
					// それ以外：個別タイトルで戻す
					uInfSub.m_Title = uInfSub.m_TitleOrg;
				}
				m_uInfo.strTitleName = uInfSub.m_Title;
			}
			// 更新
			uInfSub.Update();
		}
	}
	uInfSub.Fin();
}

//********************************************************************************
//	uc_inf_form_type：出力形式の取得
//		IN		なし
//		RET		BOOL		TRUE:正常終了
//********************************************************************************
BOOL CfrmUc000Common::CmnUcInfFormTypeGetFormType()
{
	CdbUcInfFormType	rsFormType(m_pDB);
	int					intRet = ID_FORMTYPE_KOBETU;	// デフォルト：個別

	// 出力形式を取得：切替不可の場合はレコード未登録
	if (rsFormType.Init(m_uInfo.intFormNo) == DB_ERR_OK) {
		if (!rsFormType.IsEOF()) {
			// 設定値より取得
			intRet = rsFormType.m_FormType;
		}
		rsFormType.Fin();
		m_uInfo.intFormType = intRet;
	}
	else{
		return FALSE;
	}

	return TRUE;
}

//********************************************************************************
//	uc_ren_main：財務連動実行可/不可情報の取得
//		IN		なし
//		RET		BOOL		TRUE:正常終了
//							FALSE:エラー終了
//********************************************************************************
BOOL CfrmUc000Common::CmnUcRenMainGetRendoInfo(){

	BYTE			bEzFlag = 0;

	// 業種が「法人，運送，建設，病院」のみ実行可能（個人は実行不可）
	// また、学校，公益･･･なども実行不可
	switch (m_RenZvol.CoType) {
	case ID_ICS_COTYPE_HOJIN:		// 0x00	法人
	case ID_ICS_COTYPE_UNSO:		// 0x01	運送
	case ID_ICS_COTYPE_KENSETU:		// 0x20	建設
	case ID_ICS_COTYPE_BYOIN:		// 0x30	病院
		// uc_ren_main取得
		CdbUcRenMain	rsRenMain(m_pDB);
		if (rsRenMain.RequeryFormSeq(m_uInfo.intFormSeq) == DB_ERR_OK) {
			if (!rsRenMain.IsEOF()) {
				// 枝番残高は、会社情報の枝番あり＋uc_ren_mainの枝番ありで実行可能
				if ((m_RenZvol.EzFlag & ID_ICS_EZFLAG_TRUE) && (rsRenMain.m_OpRenTypeEz == 1)) {
					// 枝番残高実行可能
					bEzFlag = 1;
				}

				// 財務連動の実行可能を判断
				if ((rsRenMain.m_OpRenTypeKz == 1) || (bEzFlag == 1) || 
					(rsRenMain.m_OpRenTypeDa == 1) || (rsRenMain.m_OpRenTypeKs == 1)) {
					// 財務連動実行可能
					m_uInfo.bRenFlg = TRUE;
				}
			}
			rsRenMain.Fin();
		}
		else{
			return FALSE;
		}
		break;
	}

	return TRUE;
}

//********************************************************************************
//	uc_lst_kamoku：ComboBoxのリストを取得
//		IN		作成されたComboBoxのリストを格納するポインタ
//				科目名が9文字を超えるかどうかのフラグ			
//		RET		TRUE　	…	成功
//				FALSE	…	失敗
//********************************************************************************
BOOL CfrmUc000Common::CmnUcLstKamokuGetComboList( CString *strRetCombo, BOOL& bFontSizeChange)
{
// midori 152137 add -->
	int				sort=0;
	short			cnt=0;		// 追加するコンボボックスのインデックス
// midori 152137 add <--
	CdbUcLstKamoku	Kamoku( m_pDB );
	CString			strCombo = "";
	CString			strDivideKamoku1, strDivideKamoku2;

// midori 161106 add -->
	m_CmbCnt = 0;
// midori 161106 add <--
// midori 152137 add -->
	m_SortMap1.RemoveAll();
	m_SortMap2.RemoveAll();
// midori 152137 add <--
	if (m_uInfo.intFormSeq > 0) {
		// 科目テーブルから対象様式のリスト取得
// midori 152137 del -->
//		if (Kamoku.Init(m_uInfo.intFormSeq) == DB_ERR_OK) {
// midori 152137 del <--
// midori 152137 add -->
		sort = m_clsFunc.IsKamokuSort(m_uInfo.intFormSeq,m_pDB);
		// InitをInit_Mに変更。渡す引数のソート順をOwntbから取得（option_sortから取得）
		if (Kamoku.Init_M(m_uInfo.intFormSeq,sort) == DB_ERR_OK) {
// midori 152137 add <--
			CharController cc;
			// 科目リストから選択項目全取得
			// 項目を区切るには"\r\n"(CR/LF)を使用
			strCombo = "\r\n";
// midori 152137 add -->
			cnt = 0;
			// KnOrderと科目コンボボックスのインデックスを関連付けするMapを作成
			// 1データ目に空白をセット
			// Key:KnOrder Value:コンボボックスのインデックス
			m_SortMap1.SetAt(0,cnt);
			// Key:コンボボックスのインデックス Value:KnOrder
			m_SortMap2.SetAt(cnt,0);
			cnt++;
// midori 152137 add <--
			while( !Kamoku.IsEOF()){
				strCombo = strCombo + Kamoku.m_KnName + "\r\n";

				// 科目文字列9文字以上？
				cc.Divide(Kamoku.m_KnName, strDivideKamoku1, strDivideKamoku2, DR_INPUTLENGTH_NORMAL_OLD);
				if( !strDivideKamoku2.IsEmpty() ){
					bFontSizeChange = TRUE;
				}

// midori 152137 add -->
				// KnOrderと科目コンボボックスのインデックスを関連付けするMapを作成
				// Key:KnOrder Value:コンボボックスのインデックス
				m_SortMap1.SetAt(Kamoku.m_KnOrder,cnt);
				// Key:コンボボックスのインデックス Value:KnOrder
				m_SortMap2.SetAt(cnt,Kamoku.m_KnOrder);
				cnt++;
// midori 152137 add <--

				Kamoku.MoveNext();
			}
// midori 161106 add -->
			// 新規追加用の選択を最後に追加
			if (m_uInfo.intFormSeq == ID_FORMNO_102) {
				// 様式10-2
				strCombo = strCombo + _T("【新規】") + "\r\n";
			}
			else	{
				// 様式10-2以外
				strCombo = strCombo + _T("【新規追加】") + "\r\n";
			}
			// 実際の登録件数をセット
			m_CmbCnt = (int)Kamoku.GetRecordCount();
// midori 161106 add <--
// midori 152137 add -->
			// KnOrderと科目コンボボックスのインデックスを関連付けするMapを作成
			// 最終行に新規追加をセット
			// Key:KnOrder Value:コンボボックスのインデックス
			m_SortMap1.SetAt((long)cnt,cnt);
			// Key:コンボボックスのインデックス Value:KnOrder
			m_SortMap2.SetAt(cnt,(long)cnt);
			cnt++;
// midori 152137 add <--
			Kamoku.Fin();
		}
		else{
			return FALSE;
		}
	}

	// 戻り値格納
	*strRetCombo = strCombo;

	return TRUE;
}

//********************************************************************************
//	uc_lst_item_sort：データ取得
//		IN		int				項目番号
//				ITEMSORT_INFO*	取得情報の格納領域
//		RET		BOOL			TRUE:正常終了，FALSE:エラー
//********************************************************************************
BOOL CfrmUc000Common::CmnUcLstItemSortGetData(int intItemSeq, ITEMSORT_INFO* uItemSort)
{
	CdbUcLstItemSort	rsLstIS( m_pDB );
	BOOL				bRet = FALSE;

	// データ初期化
	uItemSort->bytFgSykei		= 0;			// 小計の対象である/ない
	uItemSort->bytFgItem		= 0;			// 項目の種類を示す番号
	uItemSort->strOrderStr		= "";			// 並び替え用SQL文字列
	uItemSort->strFieldSykei	= "";			// 小計のグループ判定に使用するフィールド名
	uItemSort->strFieldSykei2	= "";			// 小計のグループ判定に使用するフィールド名

	// パラメータチェック
	if ((m_uInfo.intFormSeq <= 0) || (intItemSeq <= 0)) {
		rsLstIS.Fin();
		return FALSE;
	}

	// クエリー実行
	if (rsLstIS.RequeryFormItem(m_uInfo.intFormSeq, intItemSeq) == DB_ERR_OK) {
		if (!rsLstIS.IsEOF()) {
			// 必要情報をセット
			uItemSort->bytFgSykei		= rsLstIS.m_FgSykei;		// 小計の対象である/ない
			uItemSort->bytFgItem		= rsLstIS.m_FgItem;			// 項目の種類を示す番号
			uItemSort->strOrderStr		= rsLstIS.m_OrderStr;		// 並び替え用SQL文字列
			uItemSort->strFieldSykei	= rsLstIS.m_FieldSykei;		// 小計のグループ判定に使用するフィールド名
			uItemSort->strFieldSykei2	= rsLstIS.m_FieldSykei2;	// 小計のグループ判定に使用するフィールド名
// midori 152137 add -->
			uItemSort->strOrderStr2		= rsLstIS.m_OrderStr2;		// 50音並び替え用SQL文字列
// midori 152137 add <--
// 20-0450 add -->
			uItemSort->intItemSeq		= rsLstIS.m_ItemSeq;
// 20-0450 add <--

			bRet = TRUE;
		}
	}
	rsLstIS.Fin();

	return bRet;
}

// midori 190505 add -->
// ---------------------------------------------------------------------------------------------------------------------
//	uc_lst_item_sort：データ取得
//		IN		int				様式シーケンス番号
//				int				項目番号
//				ITEMSORT_INFO*	取得情報の格納領域
//		RET		BOOL			TRUE:正常終了，FALSE:エラー
// ---------------------------------------------------------------------------------------------------------------------
BOOL CfrmUc000Common::CmnUcLstItemSortGetData2(int nFormSeq,int intItemSeq, ITEMSORT_INFO* uItemSort)
{
	CdbUcLstItemSort	rsLstIS( m_pDB );
	BOOL				bRet = FALSE;

	// データ初期化
	uItemSort->bytFgSykei		= 0;			// 小計の対象である/ない
	uItemSort->bytFgItem		= 0;			// 項目の種類を示す番号
	uItemSort->strOrderStr		= "";			// 並び替え用SQL文字列
	uItemSort->strFieldSykei	= "";			// 小計のグループ判定に使用するフィールド名
	uItemSort->strFieldSykei2	= "";			// 小計のグループ判定に使用するフィールド名

	// パラメータチェック
	if ((nFormSeq <= 0) || (intItemSeq <= 0)) {
		rsLstIS.Fin();
		return FALSE;
	}

	// クエリー実行
	if (rsLstIS.RequeryFormItem(nFormSeq, intItemSeq) == DB_ERR_OK) {
		if (!rsLstIS.IsEOF()) {
			// 必要情報をセット
			uItemSort->bytFgSykei		= rsLstIS.m_FgSykei;		// 小計の対象である/ない
			uItemSort->bytFgItem		= rsLstIS.m_FgItem;			// 項目の種類を示す番号
			uItemSort->strOrderStr		= rsLstIS.m_OrderStr;		// 並び替え用SQL文字列
			uItemSort->strFieldSykei	= rsLstIS.m_FieldSykei;		// 小計のグループ判定に使用するフィールド名
			uItemSort->strFieldSykei2	= rsLstIS.m_FieldSykei2;	// 小計のグループ判定に使用するフィールド名
			uItemSort->strOrderStr2		= rsLstIS.m_OrderStr2;		// 50音並び替え用SQL文字列
// 20-0450 add -->
			uItemSort->intItemSeq		= rsLstIS.m_ItemSeq;
// 20-0450 add <--

			bRet = TRUE;
		}
	}
	rsLstIS.Fin();

	return bRet;
}
// midori 190505 del <--

//**************************************************
//	最小化／最大化ボックスを無効に設定
//		※OnInitialUpdate()イベント等で呼び出す
//		※親ウィンドウハンドルは各ビューで「GetParent()」を実行して取得する
//	【引数】	pParent		…	親ウィンドウハンドル
//	【戻値】	なし
//**************************************************
void CfrmUc000Common::CmnEnableMinMaxBox( CWnd* pParent )
{	
	//	最大化／最小化無効
	pParent->ModifyStyle( WS_MAXIMIZEBOX | WS_MINIMIZEBOX, 0 );
	
/*OnInitialUpdate()イベントで呼ばれるから不要？
	//	クライアント領域の更新
	pParent->RedrawWindow( NULL, NULL, RDW_INVALIDATE | RDW_FRAME );
*/
}

//********************************************************************************
//	Tabキー押下時に使用するShiftキー有無のチェックを行う
//		IN		なし
//		RET		BOOL		TRUE:Shiftキーあり，FALSE:なし
//********************************************************************************
BOOL CfrmUc000Common::CmnCheckShiftKey()
{
	BOOL		bRet = FALSE;

	// Shiftキーの有無を取得
	short nShift = ::GetKeyState(VK_SHIFT);
	if (nShift & 0x80) {
		bRet = TRUE;
	}
	return bRet;
}

//************************************************************
//	入力項目を強制的に確定
//		※入力項目で入力後、「Enter／Tab」でデータを確定せずに
//	　	　マウスで他の項目に移動したら設定した入力形式にフォーマットされない問題対応
//	【引数】	pDiag	…	ICSDiagオブジェクトのポインタ
//				nRow	…	行
//				nCol	…	列
//	【戻値】	0		…	成功
//				-1		…	失敗
//************************************************************
int CfrmUc000Common::CmnDiagSetEnterReDraw( CICSDiag* pDiag, int nRow, int nCol )
{	
	int					nRet = 0;	//	戻値
	int					nIndex;		//	コントロールインデックス
	
	//	コントロールインデックス取得
	nIndex = CmnGetControlIndex( nRow, nCol );
	//	入力項目を強制的に確定
	nRet = CmnDiagSetEnterReDraw( pDiag, nIndex );

	//	戻値を返す
	return( nRet );
}

//************************************************************
//	入力項目を強制的に確定
//		※入力項目で入力後、「Enter／Tab」でデータを確定せずに
//	　	　マウスで他の項目に移動したら設定した入力形式にフォーマットされない問題対応
//	【引数】	pDiag	…	ICSDiagオブジェクトのポインタ
//				nIndex	…	コントロールのインデックス
//	【戻値】	0		…	成功
//				-1		…	失敗
//************************************************************
int CfrmUc000Common::CmnDiagSetEnterReDraw( CICSDiag* pDiag, int nIndex )
{
	int					nRet = 0;	//	戻値
	DIAGRAM_DATA		udData;		//	DIAGRAM_DATA構造体
	DIAGRAM_ATTRIBUTE	udAttr;		//	DIAGRAM_ATTRIBUTE構造体
	CURRENT_INFO		uCurInfo;

	// 共通処理から帳表のカレント情報を取得
	virGetCurrentInfo( &uCurInfo );

	// 特殊行の場合、当処理を実行しない
	// これは、小計行などの金額欄が"********"表示で、EditOffで"0"になる対応
	switch ( uCurInfo.intFgFunc ) {
// midori 156189,156190,156191 del -->
	//case ID_FGFUNC_SYOKEI:			//	10	小計行
// midori 156189,156190,156191 del <--
	case ID_FGFUNC_SYOKEINULL:		//	11	小計行（空行）
// midori 157046 del -->
	//case ID_FGFUNC_CHUKEI:			//	12	中計行
// midori 157046 del <--
	case ID_FGFUNC_CHUKEINULL:		//	13	中計行（空行）
	case ID_FGFUNC_IKKATUAUTO:		//	22	一括集計金額行（自動）
// midori 190505 add -->
	case ID_FGFUNC_KAMOKU:			//	 3	科目行（見出し）
// midori 190505 add <--
		// 抜ける
		return nRet;
	}
// midori 156189,156190,156191 add -->
// midori 157046 del -->
	//if(uCurInfo.intFgFunc == ID_FGFUNC_SYOKEI || uCurInfo.intFgFunc == ID_FGFUNC_KAMOKU) {
// midori 157046 del <--
// midori 157046 add -->
	if(uCurInfo.intFgFunc == ID_FGFUNC_SYOKEI || uCurInfo.intFgFunc == ID_FGFUNC_CHUKEI || uCurInfo.intFgFunc == ID_FGFUNC_KAMOKU) {
// midori 157046 add <--
		//if(KamokuRowEnableSgn(m_pDB,1) != 1)	{						// 改良No.21-0086,21-0529 del
		if(KamokuRowEnableSgn(m_pDB, 1, m_uInfo.intFormSeq) != 1)	{	// 改良No.21-0086,21-0529 add
			return(nRet);
		}
		else	{
// 改良No.21-0086,21-0529 cor -->
			//// ②受取手形の内訳書、⑪借入金及び支払利子の内訳書以外は処理を行わない
			//if(m_uInfo.intFormSeq != ID_FORMNO_021 && m_uInfo.intFormSeq != ID_FORMNO_111){
// ------------------------------
			// ②受取手形の内訳書、登録番号追加版の⑧支払手形の内訳書、⑪借入金及び支払利子の内訳書以外は処理を行わない
			if( !(m_uInfo.intFormSeq == ID_FORMNO_021 || (bG_InvNo == TRUE && m_uInfo.intFormSeq == ID_FORMNO_081) || m_uInfo.intFormSeq == ID_FORMNO_111))	{
// 改良No.21-0086,21-0529 cor <--
				// 抜ける
				return(nRet);
			}
			// ②受取手形の内訳書
			if(m_uInfo.intFormSeq == ID_FORMNO_021) {
				// 小計名称入力欄（振出人）以外は処理を行わない
// インボイス登録番号追加対応_23/11/06 del -->
				//if(m_uInfo.intCurCol != ID_COL_021_ADNAME1) {
// インボイス登録番号追加対応_23/11/06 del <--
// インボイス登録番号追加対応_23/11/06 add -->
				if((bG_InvNo == FALSE && m_uInfo.intCurCol != ID_COL_021_ADNAME1)
				|| (bG_InvNo == TRUE && m_uInfo.intCurCol != ID_COL_021I_ADNAME1))	{
// インボイス登録番号追加対応_23/11/06 add <--
					// 抜ける
					return(nRet);
				}
			}

// 改良No.21-0086,21-0529 add -->
			// ⑧支払手形の内訳書
			if(bG_InvNo == TRUE && m_uInfo.intFormSeq == ID_FORMNO_081)	{
				// 小計名称入力欄（支払先）以外は処理を行わない
				if(m_uInfo.intCurCol != ID_COL_081I_ADNAME1)	{
					// 抜ける
					return(nRet);
				}
			}
// 改良No.21-0086,21-0529 add <--

			// ⑪借入金及び支払利子
			if(m_uInfo.intFormSeq == ID_FORMNO_111) {
				// 小計名称入力欄（借入先）以外は処理を行わない
				if(m_uInfo.intCurCol != ID_COL_111_ADNAME1) {
					// 抜ける
					return(nRet);
				}
			}
		}
	}
// midori 156189,156190,156191 add <--

	//	DIAGRAM_DATA構造体初期化
	m_clsFunc.DiagInit( &udData );
	//	項目の属性取得
	pDiag->GetAttribute( nIndex, ( LPUNKNOWN )&udAttr );	
	//	項目のデータ取得
	nRet = pDiag->GetData( nIndex, ( LPUNKNOWN )&udData );

	//	データ取得成功？かつ入力がヌルじゃない？
	if (( nRet == 0 ) && ( udData.data_edit != "" )){

		//	取得したデータをそのまま設定（これで再描画がかかる）
		nRet = pDiag->SetData( nIndex, ( LPUNKNOWN )&udData );
	}
	else{
		
		//	入力種別がエディット？
		if ( udAttr.attr_inpkind == 0 ){
			//	データをクリア
			pDiag->DataClear( nIndex, TRUE );
		}
	}

	//	戻値を返す
	return( nRet );
}

//**************************************************
//	計行か確認
//	【引数】	なし
//	【戻値】	TRUE	…	計行
//				FALSE	…	データ行
//**************************************************
BOOL CfrmUc000Common::CmnCheckTotalRow()
{
	BOOL	fRet = FALSE;	//	戻値
	
	//	カレントインデックスが計行に居る？
	if ( m_uInfo.intCurCtlIndex >= m_uInfo.intT1StartIndex ){
		//	戻値を計行に設定
		fRet = TRUE;
	}

	//	戻値を返す
	return( fRet );
}
/*
CUcFunctionCommon::StrDivision()へ移行
//******************************************************************************************
//	文字列の分割関数(用途：名称→名称１、名称２への分割)
//	【引数】	CString			strTaget		:分割を行う文字列
//				CStringArray	pActivateView	:分割された文字列を格納する文字列のポインタ
//				int				nSize			:分割行数(2行に分割したい場合は2を指定)
//	【戻値】	なし
//****************************************************************************************
void CfrmUc000Common::CmnStrDivision(CString strTarget, CStringArray* pstrArray, int nSize)
{
	// 不当なサイズなら何もしない
	if( nSize <= 0 ){
		return;
	}

	int	nChCode	= 0;							// 改行コードの位置
	int nLen	= 0;							// 文字列の長さ
	int	iCnt  = 0;							// 編集先のインデックス
	int	iLast	= (nSize - 1);					// 格納先の最終インデックス
	CString	strWork;							// 作業用

	// 格納先の初期化
	pstrArray->RemoveAll();
	pstrArray->SetSize( nSize );

	while( 1 ){

		// 改行コードの検索
		nChCode = strTarget.Find( PRT_FORMAT_CHCODE );

		// 改行コードが見つからない場合は、文字列をそのまま格納
		if( nChCode < 0 ){
			pstrArray->SetAt( iCnt , strTarget );
			break;
		}

		// 改行コードより前の文字列を格納
		pstrArray->SetAt( iCnt, strTarget.Left( nChCode ) );

		// 次のターゲット文字列を作成
		nLen = strTarget.GetLength();
		strTarget = strTarget.Right( nLen - (nChCode + 2) );	// 「2」は、改行コードの文字数

		if ( iCnt == iLast ){
			// 最終行なら終了
			break;
		}
		else{
			// 次行へ
			iCnt++;
		}
	}
}
*/
/*
CUcFunctionCommon::StrDivisionEx()へ移行
//****************************************************************
//	文字列を最大入力文字で分割
//【引数】	chrData	…	分割元の文字列
//			strArry …	分割文字列の格納先
//			intMax	…	分割を行う文字数(1行の文字数)
//			intRow	…	分割行数
//			blnChCod…	分割した文字列に改行コード付加(TRUE)/改行コード無し(FALSE)
//【戻値】	なし
//*****************************************************************
void CfrmUc000Common::CmnStrDivisionEx( CString strData , CStringArray* strArry , int intMax , int intRow , BOOL blnChCod )
{
	CString strBuff		= "";	// バッファ
	char	chrBuff[200];		// 文字化け防止のためのバッファ
	int		intTrueMax	= 0;	// 全角/半角を考慮した分割位置数
	int		intStart	= 0;	// 取り出し位置
	int		intLen		= 0;	// 文字列のサイズ
	int		intCnt		= 0;	// カウント変数1
	int		intbbRet	= 0;	// _ismbbleadの戻り値(0以外：文字がマルチバイト文字の先行バイト　0：文字が先行バイト以外)
	int		intbsRet	= 0;	// _ismbsleadの戻り値(-1:文字が先行バイト -1以外：文字が先行バイト以外)

	// 格納先の初期化
	strArry->RemoveAll();
	if ( intRow <= 0 ){
		strArry->SetSize( 1 );
	}
	else{
		strArry->SetSize( intRow );
	}

	// 行数分ループ
	for ( intCnt = 0;intCnt < intRow;intCnt++ ){

		// 空欄は空白のみ入れる(改行文字は無し)
		if ( strData != "" ){
		
			// バッファクリア
			ZeroMemory( chrBuff , sizeof(char[200]));

			strcpy_s( chrBuff , sizeof(char[200]) , (const char *)strData );

			// 分割位置の文字列がマルチバイト文字の先行バイト?(0以外?)
			intbbRet = _ismbblead( (unsigned int)chrBuff[(intMax*2)-1] );

			// 分割位置の文字が先行バイト?(-1?)
			intbsRet = _ismbslead( (const unsigned char*)&chrBuff[0] , (const unsigned char*)&chrBuff[(intMax*2)-1] );

			// 文字列判断(全角?半角?)
			if ( ( intbbRet != 0 ) && ( intbsRet == -1 ) ){
				// 分割位置の文字列が全角2バイト文字の1バイト目なら1バイト前の文字で分割
				intTrueMax = ( intMax * 2 ) - 1;
			}
			else{
				intTrueMax = ( intMax * 2 );
			}

			// 指定文字数分取得
			strBuff = strData.Left( intTrueMax );

			// 現在の文字列のサイズを取得
			intLen = strData.GetLength();

			// 格納したデータ分は切り詰める
			strData = strData.Mid( intTrueMax  , intLen );

			// 改行フラグON && 最終行以外なら改行付加
			if ( ( blnChCod == TRUE ) && ( intCnt != ( intRow - 1 ) ) && ( strData != "") ){
				strBuff += "\r\n";
			}

			// 「intCnt」行目のデータを格納
			strArry->SetAt( intCnt , strBuff );
		}
		else{
			strArry->SetAt( intCnt , "" );
		}	
	}

}
*/
/*
CUcFunctionCommon::StrDocking()へ移行
//******************************************************************************************
//	文字列の結合関数(用途：名称１、名称２→名称への結合)
//	【引数】	CString			strTaget1		:分割を行う文字列1
//				CString			strTaget2		:分割を行う文字列2
//	【戻値】	結合後の文字列
//****************************************************************************************
CString CfrmUc000Common::CmnStrDocking(CString strTarget1, CString strTarget2)
{
	CString strRet;

	// 全ての文字列がNULLの場合、改行文字は付加しない
	if ( strTarget1 == "" && strTarget2 == "" ){
		strRet = "";
	}
	else{
		strRet = strTarget1 + "\r\n" + strTarget2;
	}

	return ( strRet );
}
*/

//******************************************************************************************
//******************************************************************************************
void CfrmUc000Common::OnActivateFrame(UINT nState, CFrameWnd* pDeactivateFrame)
{
	// フレームのアクティブイベント時に、フォーカスセットを行う。
	// 他アプリ起動→終了して、内訳書アプリがアクティブになった時に、カレントセルにフォーカス移動するための対応。
//	if (nState == TRUE) {
	switch (nState) {
	case	WA_ACTIVE:			// アクティブ化
	case	WA_CLICKACTIVE:		// マウスクリックでアクティブ
		if (m_ActiveNonFlag == FALSE)	{
			if(m_EndView == FALSE)	{
// 修正No.168516 del -->
//// 修正No.168364 add -->
//				// 登録番号ダイアログを更新
//				UpdateZmSel();
//// 修正No.168364 add <--
// 修正No.168516 del <--

				// データ入力欄にフォーカスがある事
				//if(m_DataKakutei == FALSE)	{							// 修正No.168358,168393 add	修正No.168544 del
					CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
				//}															// 修正No.168358,168393 add 修正No.168544 del
			}
		}
		break;

// 修正No.157633_元号日付 add -->
	case WA_INACTIVE:			// 非アクティブ化
		if(m_EndView == FALSE)	{
			virActivateFrameK(nState, pDeactivateFrame);	// 要通知案件修正 21/07/09 add
			virActivateFrame(nState, pDeactivateFrame);
		}
		break;
// 修正No.157633_元号日付 add <--
	}

	ICSFormView::OnActivateFrame(nState, pDeactivateFrame);
}

//******************************************************************************************
//	フォームビューのアクティブイベント関数
//	【引数】	BOOL bActivate			:アクティブフラグ(TRUE:アクティブ、FALSE:非アクティブ)
//				CView* pActivateView	:アクティブになるビュー
//				CView* pDeactiveView	:非アクティブになるビュー
//	【戻値】	なし
//******************************************************************************************
void CfrmUc000Common::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	// 様式切替直後の初回のアクティブイベント時にフォーカスセット処理を行う。
	// 様式切替直後に左上端セルが選択状態にならない現象の対応のため。
	// 注）非アクティブイベントではフォーカスセット処理を行わないこと！ちらつきが目立つため。
	if((bActivate == TRUE) && (pActivateView == this) && (m_CursorSetFlag == TRUE))	{
		// カーソル移動
		m_CursorSetFlag = FALSE;
// 修正No.168393 del -->
		//CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
// 修正No.168393 del <--
// 修正No.168393 add -->
		// データ入力欄にフォーカスがある事
		//if(m_DataKakutei == FALSE)	{	// 修正No.168544 del
			CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
		//}									// 修正No.168544 del
// 修正No.168393 add <--
	}

	ICSFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//**************************************************
//	ビュー終了処理
//	【引数】	messagem	…	メッセージ
//				wParamm		…	パラメータ
//	【戻値】	TRUE 　… 終了する
//				FALSE  … 終了しない
//**************************************************
BOOL CfrmUc000Common::CmnEndView(UINT message, WPARAM wParam)
{
	BOOL blnFlag = FALSE;

	// 実行前の準備処理
	CmnEventStartProc(ID_BUTTON_END);

	// 2009.10.23.h.yamashita
	// WM_CLOSEのパラメータが「wParam = 0(正常終了)」の場合のみ
	// 空ページ削除のチェック処理を実行する
	//
	// 【理由】
	//　・業務選択ツールバーからの一括終了時(wParam=100)に
	//	　裏のアプリがダイアログ or メッセージを出すと終了できないため
	//	  ※終了元のアプリが終了せず、フリーズしたように見える
	//	  ※「wParam=100」以外のパターンもあるかもしれないので「0」かどうかで判断
	if ( wParam == 0 ){
		// 空行データなら削除する?
		blnFlag = CmnTblCheckDeleteData();
	}

	// 終了メッセージ
//	if( ICSMessageBox( _T("処理を終了してもよろしいですか？"), MB_YESNO | MB_ICONQUESTION ) == IDNO ){
	if( ICSJobEndBox( _T("勘定科目内訳書を終了してもよろしいですか？"), 0, NULL, 0, 0, this ) == IDNO ){
// midori M-16090701 add -->
		// カーソルセット
		PostMessage(WM_USER_FOCUSSET,0,0);
// midori M-16090701 add <--
		return FALSE;
	}

	// アクティブイベントにてカーソルセットする／しないフラグ（TRUE:する、FALSE:しない）
	//m_CursorSetFlag = FALSE;

	// カレント情報などを保存
	CmnUcInfSubSetCurrentInfo();

	// 空行データの削除処理
	if ( blnFlag == TRUE ){
		CmnTblEndDeleteData();
	}

	// Viewに対して終了処理を実行させる(2009.09.08 TS自動ログオフ対応)
	CMainFrame*		pFrame		= (CMainFrame*)AfxGetMainWnd();
	pFrame->CloseView(message, wParam);

	// ドキュメントを閉じる ･･･ これをやらないとアプリエラー発生する
//	CmnCloseActiveDocument();
	((CUCHIWAKEApp*)AfxGetApp())->CloseDocument();

	return TRUE;
}

//**************************************************
//	初期処理(OnInitUpdate)実行前に必要なテーブルがオープン可かチェックする
//	【引数】	なし
//	【戻値】	TRUE 　… オープン可
//				FALSE  … オープン不可
//**************************************************
BOOL CfrmUc000Common::CmnCheckTableInit(){

	BOOL blnRet = FALSE;	// 戻り値(TRUE:オープン可/FALSE:オープン不可)

	//**************** 対象帳表のテーブルオープン ***************//

	CdbUc000Common* mfcCmn = NULL;
	mfcCmn = virGetDataTableObject();

	// テーブルオープン可?
	blnRet =  m_clsFunc.CheckTableOpen( (CdbBase*)mfcCmn );
	// メモリ開放
	if ( mfcCmn != NULL ){
		delete mfcCmn;
	}

	// オープンに失敗?
	if ( blnRet != TRUE ){
		return FALSE;
	}

	return TRUE;
}

//**************************************************
//	参照処理実行前に必要なテーブルがオープン可かチェックする
//	【引数】	intRefType	…	参照の種類
//	【戻値】	TRUE 　… オープン可
//				FALSE  … オープン不可
//**************************************************
BOOL CfrmUc000Common::CmnCheckTableReference( int intRefType ){

	CdbUcLstBank mfcBank( m_pDB );		// 金融機関テーブル
	CdbUcLstAddress mfcAdd( m_pDB );	// 相手先テーブル
	CdbUcLstKamoku mfcKamoku( m_pDB );	// 科目テーブル

	// 参照の種類によってチェックするテーブルを分ける
	switch( intRefType ){

	// 金融機関(参照のみも含む)
	case ID_DLGTYPE_BANK:
	case ID_DLGTYPE_BANK_REFERENCE:
		// 金融機関テーブルのチェック
		if ( m_clsFunc.CheckTableOpen( (CdbBase*)&mfcBank ) != TRUE ){
			return FALSE;
		}
		break;
	// 相手先(参照のみも含む)
	case ID_DLGTYPE_ADDRESS:
	case ID_DLGTYPE_ADDRESS_REFERENCE:
		// 相手先テーブルのチェック
		if ( m_clsFunc.CheckTableOpen( (CdbBase*)&mfcAdd ) != TRUE ){
			return FALSE;
		}
		break;
/*	↓科目は初期処理(virStatJob)内でチェック
	// 科目 
	case ID_DLGTYPE_KAMOKU:
		// 科目テーブルのチェック
		if ( m_clsFunc.CheckTableOpen( (CdbBase*)&mfcKamoku ) != TRUE ){
			return FALSE;
		}
		break;*/
	}

	return TRUE;
}

//**************************************************
//	財務連動処理(OnMenuZaimuRendo)実行前に必要なテーブルがオープン可かチェックする
//	【引数】	なし
//	【戻値】	TRUE 　… オープン可
//				FALSE  … オープン不可
//**************************************************
BOOL CfrmUc000Common::CmnCheckTableZaimuRendo(void)
{
	CDBZmSub* pZmSub = NULL;	// DB財務クラスのポインタ

	// ICS財務処理：ZmSub参照の準備処理
	pZmSub = ((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub;		// 財務クラス取得

	//****** 財務クラス(DBZmSub)テーブルのチェック ******//

//2008.03.05 ICS様からの修正依頼の対応
//	科目残高＆枝番残高のOpen時には、ボリュームラベルのOpenが必須
//	（総額一括税抜き対応によるモジュール側の仕様変更のため）
	if ( pZmSub->VolumeOpen() == 0 ){
		// 科目残高テーブルのチェック
		if ( pZmSub->KamokuZanOpen() != 0 ){
			// DBエラー対応
			m_clsFunc.ZmSubErrMsg( pZmSub );
			return FALSE;
		}
		pZmSub->KamokuZanClose();

		// 枝番残高テーブルのチェック
		if ( pZmSub->EdabanOpen() != 0 ){
			// DBエラー対応
			m_clsFunc.ZmSubErrMsg( pZmSub );
			return FALSE;
		}
		pZmSub->EdabanClose();
// 230227 add -->
		m_S_sgn1 = pZmSub->zvol->s_sgn1;
// 230227 add <--
		// ボリュームラベルClose
		pZmSub->VolumeClose();
	}
	else {
		// DBエラー対応
		m_clsFunc.ZmSubErrMsg( pZmSub );
		return FALSE;
	}

	// 科目名称テーブルのチェック
	if ( pZmSub->KamokuMeisyoOpen() != 0 ){
		// DBエラー対応
		m_clsFunc.ZmSubErrMsg( pZmSub );
		return FALSE;
	}
	pZmSub->KamokuMeisyoClose();

	// 合計名称テーブルのチェック
	if ( pZmSub->GokeiMeisyoOpen() != 0 ){
		// DBエラー対応
		m_clsFunc.ZmSubErrMsg( pZmSub );
		return FALSE;
	}
	pZmSub->GokeiMeisyoClose();


	// 仕訳データテーブルのチェック
	if ( pZmSub->DataOpen() != 0 ){
		// DBエラー対応
		m_clsFunc.ZmSubErrMsg( pZmSub );
		return FALSE;
	}
	pZmSub->DataClose();

	// 摘要テーブルのチェック
	if ( pZmSub->KanaTekiyoOpen() != 0 ){
		// DBエラー対応
		m_clsFunc.ZmSubErrMsg( pZmSub );
		return FALSE;
	}
	pZmSub->KanaTekiyoClose();

	// 当期月別情報テーブルオープン
	if ( pZmSub->MinfoOpen() != 0 ){
		// DBエラー対応
		m_clsFunc.ZmSubErrMsg( pZmSub );
		return FALSE;
	}
	pZmSub->MinfoClose();

	//****** 内訳書オリジナルテーブルのチェック ******//
/*  ↓のテーブルはOnUpdateMenuZaimuRendoにてチェック
	// 財務連動の管理情報テーブルのチェック
	CdbUcRenMain mfcRenMain( m_pDB );
	if ( m_clsFunc.CheckTableOpen( (CdbBase*)&mfcRenMain ) != TRUE ){
		return FALSE;
	}
*/
	// 財務連動の科目リストテーブルのチェック
	CdbUcRenKamoku mfcRenKamoku( m_pDB );
	if ( m_clsFunc.CheckTableOpen( (CdbBase*)&mfcRenKamoku ) != TRUE ){
		return FALSE;
	}

	// 財務連動仕訳データテンポラリテーブルのチェック
	CdbUcRenDarecTemp mfcRenTemp( m_pDB );
	if ( m_clsFunc.CheckTableOpen( (CdbBase*)&mfcRenTemp ) != TRUE ){
		return FALSE;
	}

	return TRUE;
}

//**************************************************
//	印刷設定(OnMenuPrintSetup)実行前に必要なテーブルがオープン可かチェックする
//	【引数】	なし
//	【戻値】	TRUE 　… オープン可
//				FALSE  … オープン不可
//**************************************************
BOOL CfrmUc000Common::CmnCheckTablePrintSetup(){

	// 文字省略等の管理情報テーブルのチェック
	CdbUcInfSubOmit mfcOmit( m_pDB );
	if ( m_clsFunc.CheckTableOpen( (CdbBase*)&mfcOmit ) != TRUE ){
		return FALSE;
	}

	return TRUE;
}

//**************************************************
//	ソート(OnMenuSort)実行前に必要なテーブルがオープン可かチェックする
//	【引数】	なし
//	【戻値】	TRUE 　… オープン可
//				FALSE  … オープン不可
//**************************************************
BOOL CfrmUc000Common::CmnCheckTableSort(){

	// ソート項目情報リストテーブルのチェック
	CdbUcLstItemSort mfcSort( m_pDB );
	if ( m_clsFunc.CheckTableOpen( (CdbBase*)&mfcSort ) != TRUE ){
		return FALSE;
	}

	return TRUE;
}

//**************************************************
//	日付の入力値が変わったかチェックする
//		IN		CICSDiag*		対象コントロール
//				int				対象コントロールのインデックス番号
//				int				日付変換種別（DC_DATE_WMD, DC_DATE_GWM, ･･･）
//				int				変更前の日付
//		RET		BOOL			TRUE:変更あり　FALSE:変更なし
//**************************************************
BOOL CfrmUc000Common::CmnCheckDate( CICSDiag* pDiag, int intIndex, int intFlg, int intDate )
{
	DIAGRAM_DATA	diadata;		// ICSDiag構造体
	CdateConvert	cDC;			// 日付変換クラス
	char			chrDate[4] = "";// 変更前の日付格納

	// 初期化
	m_clsFunc.DiagInit( &diadata );
	memset( chrDate,'\0', sizeof(chrDate));

	// 日付取得
	pDiag->GetData( intIndex , (LPUNKNOWN)&diadata );

	// 変更前の日付を「和暦.月.日」に変換)
	cDC.Convert( intDate, chrDate, intFlg );

	// 変更前の「和暦.月.日」と現在の「和暦.月.日」を比較
	if ( chrDate[0] == diadata.data_day[0] &&
		 chrDate[1] == diadata.data_day[1] &&
		 chrDate[2] == diadata.data_day[2] &&
		 chrDate[3] == diadata.data_day[3] ){
		return FALSE;
	}
	// 「和暦.月.日」が一致しなければ通常の処理(CmnDiagGetDate)を実行
	else{
		return TRUE;
	}
	
}

BOOL CfrmUc000Common::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if ( pMsg->message == WM_KEYDOWN ) {
		switch ( pMsg->wParam ) {
		case VK_F2:
			keybd_event(VK_SHIFT, 0, 0, 0);					//	Shiftキー押している
			keybd_event(VK_TAB, 0, 0, 0);					//	Tabキー押している
			keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);		//	Tabキー放した（※放さないと押っぱなしになる）
			keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);	//	Shiftキー放した（※放さないと押っぱなしになる）
			return 1;
		}
	}
// 要通知案件修正 21/07/09 add -->
	// マウス左クリック
	else if(pMsg->message == WM_LBUTTONDOWN)	{
		int		id=-1;
		POINT	pt;

		// 現在のカーソルポジションを取得する
		GetCursorPos(&pt);
		ScreenToClient(&pt);

		id = CmnWherePushButton(pt);
		if(id != -1)	{
// 要通知案件 修正No.158229 add -->
			m_SaveBtnID = id;
// 要通知案件 修正No.158229 add <--

			// 画面上部のボタン(行コピーボタン等)をクリックした
			CmnEventStartProc(id);
		}
		else	{
// 修正No.168480,168515 del -->
//			RECT	rt1;
//			// 科目コンボボックス等で【新規追加】が選択されたままタイトル部やページ番号部にカーソルがセットされた時、
//			// コンボボックスの値を元に戻す
//// 要通知案件 修正No.158238 del -->
//			//for(int ii=0; ii<2; ii++)	{
//// 要通知案件 修正No.158238 del <--
//// 要通知案件 修正No.158238 add -->
//			for(int ii=0; ii<3; ii++)	{
//// 要通知案件 修正No.158238 add <--
//				if(ii == 0)	m_pTitleDiag->GetWindowRect(&rt1);		// タイトル部
//// 要通知案件 修正No.158238 del -->
//				//else		m_pPageDiag->GetWindowRect(&rt1);		// ページ部
//// 要通知案件 修正No.158238 del <--
//// 要通知案件 修正No.158238 add -->
//				else if(ii == 1)	m_pPageDiag->GetWindowRect(&rt1);		// ページ部
//				else				m_pDiag->GetWindowRect(&rt1);			// データ部
//// 要通知案件 修正No.158238 add <--
//				ScreenToClient(&rt1);
//
//// 修正No.168447 add -->
//				if( ii == 0 || ii == 1 ||
//				   (ii == 2 && (pt.x < rt1.left || pt.x > rt1.right || pt.y < rt1.top || pt.y < rt1.bottom))) {
//					// ----- 登録番号一覧関連 ----->
//					if(pZmSel && pZmSel->IsDisplay() != FALSE) {//
//						pZmSel->DialogOFF();
//					}
//					// <---- 登録番号一覧関連 ------
//				}
//// 修正No.168447 add <--
//
//				if(pt.x >= rt1.left && pt.x <= rt1.right && pt.y >= rt1.top && pt.y <= rt1.bottom)	{
//					virKamokuAddCancel();
//					break;
//				}
//			}
// 修正No.168480,168515 del <--
// 修正No.168480,168515 add -->
			RECT	rt1;

			for(int ii=0; ii<3; ii++)	{
				if(ii == 0)			m_pTitleDiag->GetWindowRect(&rt1);		// タイトル部
				else if(ii == 1)	m_pPageDiag->GetWindowRect(&rt1);		// ページ部
				else				m_pDiag->GetWindowRect(&rt1);			// データ部
				ScreenToClient(&rt1);

// 修正No.168531 del -->
				//if(bG_InvNo == TRUE) {
				//	if(ii == 2 && (pt.x < rt1.left || pt.x > rt1.right || pt.y < rt1.top || pt.y > rt1.bottom))	{
				//		// データ部から外れた時、登録番号一覧を非表示にする
				//		// ----- 登録番号一覧関連 ----->
				//		if(pZmSel && pZmSel->IsDisplay() != FALSE) {
				//			pZmSel->DialogOFF();
				//		}
				//		// <---- 登録番号一覧関連 ------
				//	}
				//}
// 修正No.168531 del -->

				// 科目コンボボックス等で【新規追加】が選択されたままタイトル部やページ番号部にカーソルがセットされた時、
				// コンボボックスの値を元に戻す
				if(pt.x >= rt1.left && pt.x <= rt1.right && pt.y >= rt1.top && pt.y <= rt1.bottom)	{
					virKamokuAddCancel();
					break;
				}
			}

			if(bG_InvNo == TRUE) {
				RECT		dr, cr;
				SCROLLINFO	ScrollInfo = {0};
				// 現在のカーソルポジションを取得する
				GetCursorPos(&pt);
				ScreenToClient(&pt);
				m_pDiag->GetWindowRect(&dr);
				ScreenToClient(&dr);

				m_pDiag->GetScrollInfo(SB_VERT, &ScrollInfo);

				// スクロールバーの座標（になるはず…）
				cr.left = dr.right - ScrollInfo.cbSize;
				cr.right = dr.right;
				cr.top = dr.top;
				cr.bottom = dr.bottom;
				if((pt.x >= cr.left) && (pt.x <= cr.right) && (pt.y >= cr.top) && (pt.y <= cr.bottom)) {
					// データ部から外れた時、登録番号一覧を非表示にする
// 修正No.168520 add/修正No.168532 del -->
					//// ----- 登録番号一覧関連 ----->
					//if(pZmSel && pZmSel->IsDisplay() != FALSE) {
					//	pZmSel->DialogOFF();
					//}
					//// <---- 登録番号一覧関連 ------
// 修正No.168520 add/修正No.168532 del <--
					virCallChkInvoiceNum(3, m_uInfo.intCurPage, m_uInfo.intCurRow);
				}
			}
// 修正No.168480,168515 add <--
		}
	}
// 要通知案件修正 21/07/09 add <--
// 修正No.168518,168520 add -->
	// マウス右クリック
	else if(pMsg->message == WM_RBUTTONDOWN)	{
		if(bG_InvNo == TRUE)	{
			POINT		pt;
			RECT		rt1;
			RECT		cr;
			SCROLLINFO	ScrollInfo = {0};

			// 現在のカーソルポジションを取得する
			GetCursorPos(&pt);
			ScreenToClient(&pt);

			m_pDiag->GetWindowRect(&rt1);			// データ部
			ScreenToClient(&rt1);
// 修正No.168523 del -->
			//if(pt.x < rt1.left || pt.x > rt1.right || pt.y < rt1.top || pt.y > rt1.bottom)	{
			//	// データ部から外れた時、登録番号一覧を非表示にする
			//	// ----- 登録番号一覧関連 ----->
			//	if(pZmSel && pZmSel->IsDisplay() != FALSE) {
			//		pZmSel->DialogOFF();
			//	}
			//	// <---- 登録番号一覧関連 ------
			//}
// 修正No.168523 del <--
			
			m_pDiag->GetScrollInfo(SB_VERT, &ScrollInfo);

			// スクロールバーの座標（になるはず…）
			cr.left		= rt1.right - ScrollInfo.cbSize;
			cr.right	= rt1.right;
			cr.top		= rt1.top;
			cr.bottom	= rt1.bottom;
			if((pt.x >= cr.left) && (pt.x <= cr.right) && (pt.y >= cr.top) && (pt.y <= cr.bottom)) {
				// データ部から外れた時、登録番号一覧を非表示にする
// 修正No.168523 del -->
				// ----- 登録番号一覧関連 ----->
				//if(pZmSel && pZmSel->IsDisplay() != FALSE) {
				//	pZmSel->DialogOFF();
				//}
				// <---- 登録番号一覧関連 ------
// 修正No.168523 del <--
				virCallChkInvoiceNum(3, m_uInfo.intCurPage, m_uInfo.intCurRow);
			}
		}
	}
// 修正No.168518,168520 add <--

	return ICSFormView::PreTranslateMessage(pMsg);
}

// 要通知案件修正 21/07/09 add -->
// クリックされたボタンを調べる
int	CfrmUc000Common::CmnWherePushButton(CPoint point)
{
	int		id = -1;
	POINT	pt;
	RECT	rt;
	
	pt = point;

	for(int ii = 0; ii < 9; ii++)	{
		if(ii == 0)			m_btnPageBack.GetWindowRect(&rt);						// 前頁ボタン：IDC_PAGEBACK_BUTTON1
		else if(ii == 1)	m_btnPageNext.GetWindowRect(&rt);						// 次頁ボタン：IDC_PAGENEXT_BUTTON1
		else if(ii == 2)	m_btnPageNew.GetWindowRect(&rt);						// 改頁ボタン：IDC_PAGENEW_BUTTON1
		else if(ii == 3)	m_btnPageInsert.GetWindowRect(&rt);						// 頁挿入ボタン：IDC_PAGEINSERT_BUTTON1
		else if(ii == 4)	m_btnRowCopy.GetWindowRect(&rt);						// 行コピーボタン：IDC_ROWCOPY_BUTTON1
		else if(ii == 5)	m_btnRowPaste.GetWindowRect(&rt);						// 行貼付ボタン：IDC_ROWPASTE_BUTTON1
		else if(ii == 6)	m_btnRowInsert.GetWindowRect(&rt);						// 行挿入貼付ボタン：IDC_ROWINSERT_BUTTON1
		else if(ii == 7)	m_btnYousikiBack.GetWindowRect(&rt);					// 前様式ボタン：IDC_YOUSHIKIBACK_BUTTON1
		else if(ii == 8)	m_btnYousikiNext.GetWindowRect(&rt);					// 次様式ボタン：IDC_YOUSHIKINEXT_BUTTON1

		ScreenToClient(&rt);

		if(pt.x >= rt.left && pt.x <= rt.right && pt.y >= rt.top && pt.y <= rt.bottom)	{
			if(ii == 0)			id = IDC_PAGEBACK_BUTTON1;		// 前頁ボタン
			else if(ii == 1)	id = IDC_PAGENEXT_BUTTON1;		// 次頁ボタン
			else if(ii == 2)	id = IDC_PAGENEW_BUTTON1;		// 改頁ボタン
			else if(ii == 3)	id = IDC_PAGEINSERT_BUTTON1;	// 頁挿入ボタン
			else if(ii == 4)	id = IDC_ROWCOPY_BUTTON1;		// 行コピーボタン
			else if(ii == 5)	id = IDC_ROWPASTE_BUTTON1;		// 行貼付ボタン
			else if(ii == 6)	id = IDC_ROWINSERT_BUTTON1;		// 行挿入貼付ボタン
			else if(ii == 7)	id = IDC_YOUSHIKIBACK_BUTTON1;	// 前様式ボタン
			else if(ii == 8)	id = IDC_YOUSHIKINEXT_BUTTON1;	// 次様式ボタン

			break;
		}
	}

	return(id);
}
// 要通知案件修正 21/07/09 add <--

// 修正No.157627 add -->
//***************************************************************************************
//	カレント行の色をインバースするまえに、現在の文字色背景色を保存する
//	【引数】	BOOL blnFlag			:TRUE:インバースをかける　FALSE:インバース前の色に戻す
//				CURRENT_INFO *curInfo	:カレント行情報格納構造体のポインタ
//	【戻値】	なし
//***************************************************************************************
void CfrmUc000Common::CmnRowBeforeInverse(CURRENT_INFO *curInfo)
{
	DIAGRAM_ATTRIBUTE	diaatt;				// ICSDIAG属性取得構造体
	BOOL				blnFgInput=TRUE;	// 自動計算フラグ取得
	int					intStartIndex=0;	// カレント行の開始インデックス
	int					intEndIndex=0;		// カレント行の終了インデックス
	int					ii=0, jj=0;			// カウント用

	// データ入力欄以外にフォーカスがある時は処理を抜ける
	if((curInfo->intFgFunc == ID_FGFUNC_RUIKEI) || (curInfo->intFgFunc == ID_FGFUNC_PAGEKEI)){
		return;
	}
// 要通知案件修正 21/07/06 add -->
	// コンボボックスからの処理の仕方によってこの関数が2回入り、誤った文字色背景色になっている為、
	// フラグで処理の管理をする
	if(m_bInitColor == FALSE)	{
		return;
	}
// 要通知案件修正 21/07/06 add <--

	for(jj = 0; jj < 128; jj++) {
		m_BkColor[jj] = (COLORREF)RGB(0xff,0xff,0xff);	//	白色
		m_FcColor[jj] = (COLORREF)RGB(0x00,0x00,0x00);	//	黒色
	}

	// 指定行から、開始/終了のコントロール番号を取得
	intStartIndex = (m_uInfo.intCurRow - 1) * m_uInfo.intColMax;
	intEndIndex = m_uInfo.intCurRow * m_uInfo.intColMax - 1;

	// 行色を変更
	for(ii=intStartIndex, jj=0; ii<=intEndIndex; ii++, jj++)	{
		// 属性を取得
		m_pDiag->GetAttribute(ii, (LPUNKNOWN)&diaatt);

		// 背景色を設定
		memcpy(&m_BkColor[jj], &diaatt.attr_bcolor, sizeof(COLORREF));
		memcpy(&m_FcColor[jj], &diaatt.attr_fcolor, sizeof(COLORREF));
	}

// 要通知案件修正 21/07/06 add -->
	m_bInitColor = FALSE;
// 要通知案件修正 21/07/06 add <--
}

//***************************************************************************************
//	インバースされたカレント行の色を元に戻す
//	【引数】	BOOL blnFlag			:TRUE:インバースをかける　FALSE:インバース前の色に戻す
//				CURRENT_INFO *curInfo	:カレント行情報格納構造体のポインタ
//	【戻値】	なし
//***************************************************************************************
void CfrmUc000Common::CmnRowAfterInverse(CURRENT_INFO *curInfo)
{
	DIAGRAM_ATTRIBUTE	diaatt;				// ICSDIAG属性取得構造体
	COLORREF			clrRGB[2];			// 色変更値　0:背景色&フレーム色 1:文字色
	BOOL				blnFgInput=TRUE;	// 自動計算フラグ取得
	int					intStartIndex=0;	// カレント行の開始インデックス
	int					intEndIndex=0;		// カレント行の終了インデックス
	int					ii=0, jj=0;			// カウント用

// 要通知案件修正 21/07/06 add -->
	// コンボボックスからの処理の仕方によってこの関数が2回入り、誤った文字色背景色になっている為、
	// フラグで処理の管理をする
	m_bInitColor = TRUE;
// 要通知案件修正 21/07/06 add <--

	// データ入力欄以外にフォーカスがある時は処理を抜ける
	if((curInfo->intFgFunc == ID_FGFUNC_RUIKEI) || (curInfo->intFgFunc == ID_FGFUNC_PAGEKEI)){
		return;
	}

	// 指定行から、開始/終了のコントロール番号を取得
	intStartIndex = (m_uInfo.intCurRow - 1) * m_uInfo.intColMax;
	intEndIndex = m_uInfo.intCurRow * m_uInfo.intColMax - 1;

	// 行色を変更
	for(ii = intStartIndex, jj = 0; ii <= intEndIndex; ii++, jj++)	{
		// 属性を取得
		m_pDiag->GetAttribute(ii, (LPUNKNOWN)&diaatt);

		// 背景色を設定
		memcpy(&clrRGB[0], &diaatt.attr_bcolor, sizeof(COLORREF));
		memcpy(&clrRGB[1], &diaatt.attr_fcolor, sizeof(COLORREF));

		if(clrRGB[1] != m_FcColor[jj]) {
			// 文字色を元に戻す
			memcpy(&diaatt.attr_fcolor, &m_FcColor[jj], sizeof(COLORREF));

			// 属性を変更
			m_pDiag->SetAttribute(ii, (LPUNKNOWN)&diaatt, TRUE);	
		}
	}
}
// 修正No.157627 add <--

//***************************************************************************************
//	カレント行の色をインバースをかける。
//	【引数】	BOOL blnFlag			:TRUE:インバースをかける　FALSE:インバース前の色に戻す
//				CURRENT_INFO *curInfo	:カレント行情報格納構造体のポインタ
//	【戻値】	なし
//***************************************************************************************
void CfrmUc000Common::CmnRowInverse( BOOL blnFlag , CURRENT_INFO *curInfo )
{
	DIAGRAM_ATTRIBUTE diaatt;	// ICSDIAG属性取得構造体
	COLORREF clrRGB[2];			// 色変更値　0:背景色&フレーム色 1:文字色
	BOOL	 blnFgInput = TRUE;	// 自動計算フラグ取得
	int		 intStartIndex = 0;	// カレント行の開始インデックス
	int		 intEndIndex = 0;	// カレント行の終了インデックス
	int		 i = 0;				// カウント用

	// データ入力欄以外にフォーカスがある時は処理を抜ける
	if ( ( curInfo->intFgFunc == ID_FGFUNC_RUIKEI ) ||
		 ( curInfo->intFgFunc == ID_FGFUNC_PAGEKEI ) ){
	}
	else{
		// 色変更ON
		if ( blnFlag == TRUE ){
			clrRGB[0] = COLOR_INVERSE;
			clrRGB[1] = COLOR_CHAR_INVERSE;
		}
		// 色変更OFF
		else{
			// 行種判断
			switch( curInfo->intFgFunc ){

			// データ行・空行
			case ID_FGFUNC_NULL:
			case ID_FGFUNC_DATA:
				clrRGB[0] = COLOR_DATA;
				clrRGB[1] = COLOR_CHAR_NOINVERSE;
				break;
			// 小計行・小計空行
			case ID_FGFUNC_SYOKEI:
			case ID_FGFUNC_SYOKEINULL:
				clrRGB[0] = COLOR_SYOKEI;
				clrRGB[1] = COLOR_CHAR_NOINVERSE;
				break;
			// 中計行・中計空行
			case ID_FGFUNC_CHUKEI:
			case ID_FGFUNC_CHUKEINULL:
				clrRGB[0] = COLOR_CHUKEI;
				clrRGB[1] = COLOR_CHAR_NOINVERSE;
				break;
			// 一括金額（自動）
			case ID_FGFUNC_IKKATUAUTO:
				clrRGB[0] = COLOR_IKKATUAUTO;
				clrRGB[1] = COLOR_CHAR_NOINVERSE;
				break;
			// 一括金額（手動）
			case ID_FGFUNC_IKKATUMANUAL:
				clrRGB[0] = COLOR_IKKATUMANUAL;
				clrRGB[1] = COLOR_CHAR_NOINVERSE;
				break;
// midori 190505 add -->
			// 科目行（見出し）
			case ID_FGFUNC_KAMOKU:
				clrRGB[0] = COLOR_KAMOKU;
				clrRGB[1] = COLOR_CHAR_NOINVERSE;
				break;
// midori 190505 add <--
			default:
				break;
			}
		}
		
		// 指定行から、開始/終了のコントロール番号を取得
		intStartIndex = (m_uInfo.intCurRow - 1) * m_uInfo.intColMax;
		intEndIndex = m_uInfo.intCurRow * m_uInfo.intColMax - 1;

		int sw = 0;

		// 行色を変更
		for (i = intStartIndex; i <= intEndIndex; i++) {
			// 属性を取得
			m_pDiag->GetAttribute(i, (LPUNKNOWN)&diaatt);

			// 背景色を設定
			memcpy(&diaatt.attr_bcolor, &clrRGB[0], sizeof(COLORREF));
			memcpy(&diaatt.attr_fcolor, &clrRGB[1], sizeof(COLORREF));

			// 入力コントロールのみフレーム色を設定：これをしないと白枠が表示される
			if ((i >= intStartIndex + m_uInfo.intCtlStartIndex) && (i < intStartIndex + m_uInfo.intCtlStartIndex + m_uInfo.intColMaxData)) {
// インボイス登録番号追加対応_23/11/27 del -->
				//memcpy(&diaatt.attr_frcolor, &clrRGB[0], sizeof(COLORREF));
// インボイス登録番号追加対応_23/11/27 del <--
// インボイス登録番号追加対応_23/11/27 add -->
				sw = 0;
				if(bG_InvNo == TRUE)	{
					if(m_uInfo.intFormSeq == ID_FORMNO_021)	{
						// 直接入力に変更したため、フレームの色変更を行わない
						if(i == CmnGetControlIndex(m_uInfo.intCurRow, ID_COL_021I_SPDATE) ||
						   i == CmnGetControlIndex(m_uInfo.intCurRow, ID_COL_021I_PMDATE) ||
						   i == CmnGetControlIndex(m_uInfo.intCurRow, ID_COL_021I_TEKI)) {
							sw = 1;
						}
					}
					else if(m_uInfo.intFormSeq == ID_FORMNO_153)	{
						// 直接入力に変更したため、フレームの色変更を行わない
						if(i == CmnGetControlIndex(m_uInfo.intCurRow, ID_COL_153I_HDNAME) ||
						   i == CmnGetControlIndex(m_uInfo.intCurRow, ID_COL_153I_ADNAME1) ||
						   i == CmnGetControlIndex(m_uInfo.intCurRow, ID_COL_153I_ADADD1) ||
						   i == CmnGetControlIndex(m_uInfo.intCurRow, ID_COL_153I_ADADD2) ||
						   i == CmnGetControlIndex(m_uInfo.intCurRow, ID_COL_153I_TEKI)) {
							sw = 1;
						}
					}
				}
			
				if(sw == 0)		memcpy(&diaatt.attr_frcolor, &clrRGB[0], sizeof(COLORREF));
// インボイス登録番号追加対応_23/11/27 add <--
			}

// インボイス登録番号追加対応_23/11/27 del -->
			// ⑮-3のみ、住所の区切り線の色も変更する
			if(bG_InvNo == TRUE && m_uInfo.intFormSeq == ID_FORMNO_153 && i == CmnGetControlIndex(m_uInfo.intCurRow, 14)) {
				memcpy(&diaatt.attr_lfcolor, &clrRGB[0], sizeof(COLORREF));	// 線の色
				memcpy(&diaatt.attr_lbcolor, &clrRGB[0], sizeof(COLORREF));	// 線の隙間の色
			}
// インボイス登録番号追加対応_23/11/27 del <--

			// 属性を変更
			m_pDiag->SetAttribute(i, (LPUNKNOWN)&diaatt, TRUE);	
		}

		// 自動計算フラグが手入力になっている?
		if(blnFlag == FALSE) {
			if(curInfo->intFgInput == ID_AUTOCALC_INPUT) {
				// カレント行内の手入力セルの色を変更
				virInitialControlFgInputCell(m_uInfo.intCurRow, curInfo->intFgFunc, curInfo->intFgInput);
			}
		}
	}
}

//********************************************************************************
//	一括金額追加：対象行を一括金額の非表示データへ更新する
//		IN		int			対象ページ
//				int			対象行
//				int			更新するグループ
//		RET		int			FUNCTION_OK:正常終了
//********************************************************************************
int CfrmUc000Common::CmnTblIkkatuAddCurrentToIkkatu(int nPage, int nRow, int nNumGroup)
{
	CdbUc000Common*		rsData;
	rsData = virGetDataTableObject();

	// 頁と行からレコード抽出
	rsData->RequeryPageRow(nPage, nRow);

	// 入れ替え先が取得できない場合は終了
	if (!rsData->IsEOF()) {
		// 一時保管データへ編集
		rsData->Edit();
		rsData->m_NumPage = 0;						// 頁クリア
		rsData->m_NumRow = 0;						// 行クリア
		rsData->m_FgShow = ID_FGSHOW_IKKATU;		// 一括非表示
		rsData->m_NumGroup = nNumGroup;				// グループ
		rsData->Update();
	}
	rsData->Fin();
	delete rsData;

	return FUNCTION_OK;
}

//********************************************************************************
//	一括金額追加：前回のソート条件によりカレント行の科目情報を取得する
//		IN		int					対象ページ
//				int					対象行
//				SORTKAMOKU_INFO*	科目情報
//		RET		BOOL				TRUE:科目情報必要，FALSE:不要
//********************************************************************************
int CfrmUc000Common::CmnTblIkkatuAddGetKamokuInfo(int nPage, int nRow, SORTKAMOKU_INFO* uKamoku)
{
	CdbUcInfSub		uInfSub( m_pDB );	// uc_inf_sub
	ITEMSORT_INFO	uItemSort;			// 第１ソート項目のソート情報
	BOOL			bKamoku = FALSE;	// 科目情報必要フラグ

	// 準備：科目情報をクリア
	uKamoku->intKnSeq = 0;
	uKamoku->intKnOrder = 0;
	uKamoku->strKnName = "";
// midori 152137 add -->
	uKamoku->strKnKana = "";
// midori 152137 add <--

	// 対象様式のテーブルを開く
	if (uInfSub.RequeryFormSeq(m_uInfo.intFormSeq) == DB_ERR_OK) {
		// 前回のソート条件を取得
		if (!uInfSub.IsEOF()) {
			// uc_lst_item_sortを参照して必要情報を取得
			CmnUcLstItemSortGetData(uInfSub.m_Sort1, &uItemSort);

			// 科目情報付加の判断：第１ソート項目が科目＋小計あり＋一括あり
			if ((uItemSort.bytFgItem == 1) && (uInfSub.m_OpSykeiAuto == 1) && (uInfSub.m_OpIkkatu == 1)) {
				// 科目情報が必要
				bKamoku = TRUE;
			}
		}
		// 閉じる
		uInfSub.Fin();
	}

	// 科目情報の取得
	if (bKamoku == TRUE) {
		CdbUc000Common*		rsData;
		rsData = virGetDataTableObject();

		// 頁と行からレコード抽出
		rsData->RequeryPageRow(nPage, nRow);

		// 科目情報の取得
		virTblGetKamokuInfo(rsData, uKamoku);

		rsData->Fin();
		delete rsData;
	}

	return bKamoku;
}

//********************************************************************************
//	財務連動：「金額のみ再連動」の開始処理
//		IN		CdlgRenMain*	財務連動ダイアログ情報
//				CdbUc000Common* テーブル情報
//				RENTEMP_INFO*	財務連動情報
//		RET		int				実行結果（FUNCTION_OK, FUNCTION_NG）
//********************************************************************************
int CfrmUc000Common::CmnTblRendoUpdValStartProc(CdlgRenMain* pCdlgRenMain,
												RENTEMP_INFO* uRenInfo,
												CdbUc000Common* rsData)
{
	// 財務連動テンポラリ情報をクリア
	rsData->UpdateRenFgTempClear();

	// 行削除に備えて、頁計累計を削除する
	CmnTblDeleteFgFunc(ID_FGFUNC_PAGEKEI);				// 頁計の削除
	CmnTblDeleteFgFunc(ID_FGFUNC_RUIKEI);				// 累計の削除

	// 最終データ行を検索
	rsData->RequeryLastNotNullLine();
	if (!rsData->IsEOF()) {
		// 最終データ行以降の空行を全て削除
		CdbUc000Common*		rsTemp;
		rsTemp = virGetDataTableObject();
		rsTemp->DeleteNullLinePageRowAfterAll(rsData->m_NumPage, rsData->m_NumRow);
		delete rsTemp;

		// 頁＋行の開始位置を更新
		uRenInfo->nPage = rsData->m_NumPage;
		uRenInfo->nRow = rsData->m_NumRow;
	}
	else {
		// データ無しなら一時保管データ以外の全データ削除
		rsData->DeleteFgShow(ID_FGSHOW_OFF);			// 表示データ
		rsData->DeleteFgShow(ID_FGSHOW_IKKATU);			// 一括金額（非表示分）
		rsData->DeleteFgShow(ID_FGSHOW_RET_HOKAN);		// 一時保管からの戻し対象
		rsData->DeleteFgShow(ID_FGSHOW_RET_IKKATU);		// 一括金額からの戻し対象
	}

	return FUNCTION_OK;
}

//********************************************************************************
//	財務連動：「金額のみ再連動」の終了処理
//		IN		CdlgRenMain*	財務連動ダイアログ情報
//				CdbUc000Common* テーブル情報
//				RENTEMP_INFO*	財務連動情報
//		RET		int				実行結果（FUNCTION_OK, FUNCTION_NG）
//********************************************************************************
int CfrmUc000Common::CmnTblRendoUpdValEndProc(CdlgRenMain* pCdlgRenMain,
											  RENTEMP_INFO* uRenInfo,
											  CdbUc000Common* rsData)
{
	SORTKAMOKU_INFO		uKamoku;
	int					nGroup = 0;

	//----------------------------------------------------------------
	//	後処理
	//	　①財務側で連動データがなくなったら、一時保管へ移動する
	//	　②一括金額の元データ更新を考慮して、自動一括金額行の再計算
	//----------------------------------------------------------------

	// 財務連動元の情報が設定済みで、"RenFgTemp"未更新データの抽出
	rsData->RequeryRenFgTempUpdateNone();
	while (!rsData->IsEOF()) {
		switch (rsData->m_FgShow) {
		case ID_FGSHOW_OFF:				// 通常データ
		case ID_FGSHOW_IKKATU:			// 一括金額データ
			// 一時保管へ移動
			if (rsData->m_FgFunc == ID_FGFUNC_DATA) {
				// 注意）このパターンのデータの作成方法がわからなかったので、非常に不安定
				// 保管テーブルに追加
				CmnTblAddToHokanOneLine(m_uInfo.intFormSeq, rsData->m_NumPage, rsData->m_NumRow);

				// 一時保管データへ編集
				rsData->Edit();
				rsData->m_NumPage = 0;					// 頁をクリア
				rsData->m_NumRow = 0;					// 行をクリア
				rsData->m_FgShow = ID_FGSHOW_HOKAN;		// 一時保管
				rsData->Update();
			}			
			break;
		case ID_FGSHOW_HOKAN:			// 一時保管データ
			// 既に移動済みなので何もしない
			break;
		}
		rsData->MoveNext();
	}

	// 一括金額行の抽出
	rsData->RequeryFgShowIkkatu();
	while (!rsData->IsEOF()) {
		if (rsData->m_NumGroup > 0) {
			// 一括金額行の更新
			CmnTblIkkatuAutoUpdateProc(1, rsData->m_NumGroup, 0, 0, uKamoku);
		}
		rsData->MoveNext();
	}

	return FUNCTION_OK;
}


//********************************************************************************
//	財務連動：データ取込処理（メイン）
//		IN		CdlgRenMain*	財務連動ダイアログ情報
//				CdbUc000Common* テーブル情報
//		RET		int				実行結果（FUNCTION_OK, FUNCTION_NG）
//********************************************************************************
int CfrmUc000Common::CmnTblRendoMainProc(CdlgRenMain* pCdlgRenMain,
										 CdbUc000Common* rsData
										 )
{
	CDBZmSub*		pZmSub = NULL;		// DB財務クラス情報
	CdbUcRenKamoku	rsRenKamoku(m_pDB);	// uc_ren_kamokuテーブル
	RENTEMP_INFO	uRenInfo;			// 財務連動情報
	BOOL			bRet;

	// 初期値設定
	uRenInfo.nPage = 1;										// 頁番号
	uRenInfo.nRow = 0;										// 行番号
	uRenInfo.strRenKcd = _T("");							// 財務連動元（科目）
	uRenInfo.lngRenEcd = 0;									// 財務連動元（枝番）
	uRenInfo.intOpRenType = pCdlgRenMain->m_OpRenType;		// 連動元データ選択情報
	uRenInfo.intOpUpdType = pCdlgRenMain->m_OpUpdType;		// 連動方法
	uRenInfo.intKimatsu = 0;								// 期末決算月オフセット
	uRenInfo.intStartOffset = 0;							// 集計期間開始オフセット
	uRenInfo.intEndOffset = 0;								// 集計期間終了オフセット
	ZeroMemory( &uRenInfo.blnEndKes[0] , sizeof(uRenInfo.blnEndKes));	// 集計対象フラグ
	ZeroMemory( &uRenInfo.blnSykFlg[0] , sizeof(uRenInfo.blnSykFlg));	// 集計対象フラグ
// midori 152132 add -->
	uRenInfo.byteEdaren = pCdlgRenMain->m_OpEdaren;			// 枝番未登録･未入力のデータを連動するOn/Offフラグ
	uRenInfo.byteKmkName = pCdlgRenMain->m_OpKmkName;		// 科目名称を取引先、金融機関名称に転記するOn/Offフラグ
	uRenInfo.byteF9Upd = pCdlgRenMain->m_OpF9Upd;			// 取引先、金融機関の[F9 参照]に登録するOn/Offフラグ
	uRenInfo.byteEdasgn = FALSE;							// 枝番残高取込時、現在取込を行っているデータが枝番取込か科目取込かのフラグ
// midori 152132 add <--
// midori 152137 add -->
	uRenInfo.strAdKana = _T("");
// midori 152137 add <--
	uRenInfo.strInvNo = _T("");		// インボイス登録番号追加対応_23/11/27 add

	// 財務連動の開始処理
	if (pCdlgRenMain->m_OpUpdType == ID_OPUPDTYPE_VALRENDO) {
		// 金額のみ再連動する
		CmnTblRendoUpdValStartProc(pCdlgRenMain, &uRenInfo, rsData);
	}
	else {
// midori 152713 del -->
		//// 全データクリア後に財務連動を行う：内訳書帳表の全レコード削除
		//rsData->DeleteAllRecord();
		//// 保管テーブルも削除
		//CdbUcLstHokan mfcRec( m_pDB );
		//mfcRec.DeleteFormSeq( m_uInfo.intFormSeq );
		//mfcRec.Fin();
// midori 152713 del <--
// midori 152713 add -->
		// 全データクリア後に財務連動を行う：内訳書帳表の保管レコード以外を削除
		rsData->DeleteVisualRecord();
// midori 152713 add <--
	}

	// ICS財務処理：ZmSub参照の準備処理
	pZmSub = ((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub;		// 財務クラス取得

	// 集計期間取得
	pCdlgRenMain->RendoGetSyukeiTerm( &uRenInfo );

	// 期末決算月オフセット＆開始月～期末決算月の集計対象判断値を取得
	CmnICSGetMinfoData( pZmSub, &uRenInfo );

	// 財務連動の対象となる財務科目をuc_ren_kamokuから取得
	rsRenKamoku.RequeryFormSeqFgCoType(m_uInfo.intFormSeq, m_RenZvol.CoType, 1);

// 230227 add -->
	m_Invoice = m_ZmGen.IsInvoiceVupMaster(pZmSub);
// 230227 add <--

	//2008.03.05 ICS様からの修正依頼の対応
	// 科目残高＆枝番残高のOpen時には、ボリュームラベルのOpenが必須
	//	（既にOpenチェックは実施しているので、エラー処理はしない）
	pZmSub->VolumeOpen();

	while (!rsRenKamoku.IsEOF()) {
		// 内訳書科目を取得
		CdbUcLstKamoku	rsLstKamoku(m_pDB);
		uRenInfo.lngKnSeq = 0;
		if (rsLstKamoku.RequeryKnSeq(m_uInfo.intFormSeq, rsRenKamoku.m_KnSeq) == DB_ERR_OK) {
			// uc_lst_kamokuのチェックを含む
			uRenInfo.lngKnSeq = rsLstKamoku.m_KnSeq;
		}
		rsLstKamoku.Fin();

		// 財務科目の範囲を取得
		bRet = m_clsFunc.GetKcdFromTo(rsRenKamoku.m_KcdStart, &uRenInfo.strKcdFrom, &uRenInfo.strKcdTo);

		// 財務データから内訳書データ作成
		if ((uRenInfo.lngKnSeq > 0) && (bRet == TRUE)) {
			switch (pCdlgRenMain->m_OpRenType) {
			case ID_RENDATA_KZ:
				// 科目残高
				CmnTblRendoSubKzrecProc(pCdlgRenMain, pZmSub, &uRenInfo, rsData);
				break;
			case ID_RENDATA_EZ:
				// 枝番残高
				CmnTblRendoSubEzrecProc(pCdlgRenMain, pZmSub, &uRenInfo, rsData);
				break;
			case ID_RENDATA_DA:
				// 仕訳データ
				CmnTblRendoSubDarecProc(pCdlgRenMain, pZmSub, &uRenInfo, rsData);
				break;
			case ID_RENDATA_KS:
				// 科目残高集計
				CmnTblRendoSubKzSumProc(pCdlgRenMain, pZmSub, &uRenInfo, rsData);
				break;
			}
		}
		// 次のレコード参照
		rsRenKamoku.MoveNext();
	}
	rsRenKamoku.Fin();

	//2008.03.05 ボリュームラベルClose
	pZmSub->VolumeClose();


	// 財務連動の開始処理
	if (pCdlgRenMain->m_OpUpdType == ID_OPUPDTYPE_VALRENDO) {
		// 金額のみ再連動する
		CmnTblRendoUpdValEndProc(pCdlgRenMain, &uRenInfo, rsData);
	}

	// 財務連動テンポラリ情報をクリア
	rsData->UpdateRenFgTempClear();

	return FUNCTION_OK;
}

//********************************************************************************
//	財務連動：データ取込処理（科目残高）･･･ 科目残高集計でも呼ばれる
//		IN		CdlgRenMain*	財務連動ダイアログ情報
//				CDBZmSub*		ICS財務データ情報
//				RENTEMP_INFO*	財務連動で使用するアプリ情報
//				CdbUc000Common* テーブル情報（ソート実行済み状態）
//		RET		int				実行結果（FUNCTION_OK, FUNCTION_NG）
//********************************************************************************
int CfrmUc000Common::CmnTblRendoSubKzrecProc(CdlgRenMain* pCdlgRenMain,
											 CDBZmSub* pZmSub,
											 RENTEMP_INFO* uRenInfo,
											 CdbUc000Common* rsData
											 )
{
	CString		strFilter = _T("");
	CString		strSum = _T("");
	BOOL		bValue;
// No.157994 add -->
	BOOL		bVal1,bVal2;
// No.157994 add <--
	BOOL		bUpdateVal;
	int			i;

	// 科目残高テーブルの抽出条件（kzrec）
	strFilter.Format("kzkcd >= '%s' AND kzkcd <= '%s'", uRenInfo->strKcdFrom, uRenInfo->strKcdTo);

	// 科目残高テーブルを取得（kzrec）
	if ( pZmSub->KamokuZanOpen(strFilter, 0) == 0 ){
		// 科目の範囲により複数レコード存在する
		for (i = 0; i < pZmSub->kzrec->GetRecordCount(); i++) {
// No.157994 del -->
//			// 科目残高の集計
//			bValue = CmnICSGetSumKzrec(pZmSub, uRenInfo, &strSum);
//
//// 修正No.157858 add -->
//			// 仕訳データに0円以上の仕訳で作成されているかどうかをチェック
//			bValue = CmnICSChkDatarec(pZmSub, uRenInfo, pZmSub->kzrec->kzkcd, 0);
//// 修正No.157858 add <--
// No.157994 del <--
// No.157994 add -->
			// 科目残高の集計
			bVal1 = CmnICSGetSumKzrec(pZmSub, uRenInfo, &strSum);
			// 仕訳データに0円以上の仕訳で作成されているかどうかをチェック
			bVal2 = CmnICSChkDatarec(pZmSub, uRenInfo, pZmSub->kzrec->kzkcd, -1);

			// どちらかがTRUEなら連動を行う
			bValue = FALSE;
			if(bVal1 == TRUE || bVal2 == TRUE) {
				bValue = TRUE;
			}
// No.157994 add <--

			// 財務連動元の情報
			uRenInfo->strRenKcd = pZmSub->kzrec->kzkcd;									// 財務連動元（科目）
			uRenInfo->lngRenEcd = 0;													// 財務連動元（枝番）
			uRenInfo->strAdName = CmnICSGetKnrecKnnam(pZmSub, pZmSub->kzrec->kzkcd);	// 取引先名称
			uRenInfo->strVal    = strSum;												// 金額
// midori 152137 add -->
			uRenInfo->strAdKana = CmnICSGetKnrecKnKana(pZmSub, pZmSub->kzrec->kzkcd);	// 取引先カナ
// midori 152137 add <--
			uRenInfo->strInvNo  = _T("");												// 登録番号		インボイス登録番号追加対応_23/11/27 add
			bUpdateVal = FALSE;

			//----------------------------------------
			//	科目残高＋金額のみ再連動する場合
			//----------------------------------------
			if ((uRenInfo->intOpRenType == ID_RENDATA_KZ) && (uRenInfo->intOpUpdType == ID_OPUPDTYPE_VALRENDO)) {
				// 財務連動元の情報からデータを検索
				rsData->RequeryRenKcdRenEcd(uRenInfo->strRenKcd, uRenInfo->lngRenEcd);
				if (!rsData->IsEOF()) {
					// 内訳書データ更新
					virRendoUpdateData(rsData, uRenInfo);
					bUpdateVal = TRUE;

					// 合計欄をクリア
					strSum = _T("0");
				}
			}

			//----------------------------------------
			//	財務連動データを新規作成する場合 ･･･ 金額のみ再連動でも、更新データ無しの場合はデータ追加する
			//----------------------------------------
			if (bUpdateVal == FALSE) {
				// 金額が発生していない場合は、データ作成しない
				// 金額が発生して、残高０円の場合は、ダイアログの設定に従う
				if (bValue == TRUE) {
					if ((strSum.Trim() != "0") || (pCdlgRenMain->m_OpOutZeroEn == 1)) {
						// 内訳書データ作成
						if (uRenInfo->intOpRenType == ID_RENDATA_KS) {
							virRendoSumData(rsData, uRenInfo);				// 科目残高集計
						}
						else {
							virRendoCreateData(rsData, uRenInfo);			// 科目残高
						}

						// 合計欄をクリア
						strSum = _T("0");
					}
				}
			}
			// 次レコードを参照
			pZmSub->kzrec->MoveNext();
		}
		// 枝番残高テーブル閉じる
		pZmSub->KamokuZanClose();

		return FUNCTION_OK;
	}
	else{
		// 2007.03 DBエラー対応
		m_clsFunc.ZmSubErrMsg( pZmSub );
		return FUNCTION_NG;
	}

	return FUNCTION_NG;
}

//********************************************************************************
//	財務連動：データ取込処理（科目残高集計）
//		IN		CdlgRenMain*	財務連動ダイアログ情報
//				CDBZmSub*		ICS財務データ情報
//				RENTEMP_INFO*	財務連動で使用するアプリ情報
//				CdbUc000Common* テーブル情報（ソート実行済み状態）
//		RET		int				実行結果（FUNCTION_OK, FUNCTION_NG）
//********************************************************************************
int CfrmUc000Common::CmnTblRendoSubKzSumProc(CdlgRenMain* pCdlgRenMain,
											 CDBZmSub* pZmSub,
											 RENTEMP_INFO* uRenInfo,
											 CdbUc000Common* rsData
											 )
{
	int			intRet;
	int			i;

	// 当処理は「⑭-2人件費」専用処理です
	// 「⑭-2人件費」は３行固定であるため、まず最初に３行分のデータを作成する
	// その後、対象科目の科目残高を求めて、内訳書へ金額を加算する

	// ⑭-2人件費？
	if (m_uInfo.intFormSeq != ID_FORMNO_142) {
		return FUNCTION_NG;
	}

	// ⑭-2人件費の内訳書データ作成（３行固定の空データ）
	for (i = 1; i <= m_uInfo.intRowMaxData; i++) {
		// 対象行のデータ有無チェック
		rsData->RequeryPageRow(1, i);
		if (rsData->IsEOF()) {
			// 内訳書データが存在しない場合、新規レコード作成
			rsData->CreateNewRecord();					// 新規レコード追加（初期化済み）

			rsData->Edit();
			rsData->m_NumPage	= 1;					// 頁番号
			rsData->m_NumRow	= i;					// 行番号
			rsData->m_FgFunc	= ID_FGFUNC_DATA;		// 特殊行フラグ
			rsData->m_FgShow	= ID_FGSHOW_OFF;		// 表示フラグ

			rsData->Update();
		}
	}

	// 科目残高を取得して、金額を加算
	//		科目残高の取得：データ取込処理（科目残高）で実現
	//		取得残高の加算：Virtual関数で実現
	intRet = CmnTblRendoSubKzrecProc(pCdlgRenMain, pZmSub, uRenInfo, rsData);

	return intRet;
}

//********************************************************************************
//	財務連動：データ取込処理（枝番残高）
//		IN		CdlgRenMain*	財務連動ダイアログ情報
//				CDBZmSub*		ICS財務データ情報
//				RENTEMP_INFO*	財務連動で使用するアプリ情報
//				CdbUc000Common* テーブル情報（ソート実行済み状態）
//		RET		int				実行結果（FUNCTION_OK, FUNCTION_NG）
//********************************************************************************
int CfrmUc000Common::CmnTblRendoSubEzrecProc(CdlgRenMain* pCdlgRenMain,
											 CDBZmSub* pZmSub,
											 RENTEMP_INFO* uRenInfo,
											 CdbUc000Common* rsData
											 )
{
	CString		strFilter = _T("");
	CString		strSum = _T("");
	CString		strDividedName;
	BOOL		bValue;
// No.157994 add -->
	BOOL		bVal1,bVal2;
// No.157994 add <--
	BOOL		bUpdateVal;
	int			i;
	CharController cc;
// midori 161110_2 add -->
	BOOL		bL_EndSw=FALSE;
	CString		strGokei=_T("");

	// 初期化
	bL_EndSw = FALSE;
	strGokei.Empty();
	m_mapEdaZan.RemoveAll();
// midori 161110_2 add <--

	// 枝番残高テーブルの抽出条件（ezrec）
	strFilter.Format("ezkcd >= '%s' AND ezkcd <= '%s'", uRenInfo->strKcdFrom, uRenInfo->strKcdTo);

	// 枝番残高テーブルを取得（ezrec）
	if ( pZmSub->EdabanOpen(strFilter, 0) == 0 ){
		// 科目の範囲＋枝番により複数レコード存在する
		for (i = 0; i < pZmSub->ezrec->GetRecordCount(); i++) {
// No.157994 del -->
//			// 枝番残高の集計
//			bValue = CmnICSGetSumEzrec(pZmSub, uRenInfo, &strSum);
//
//// 修正No.157858 add -->
//			// 仕訳データに0円以上の仕訳で作成されているかどうかをチェック
//			bValue = CmnICSChkDatarec(pZmSub, uRenInfo, pZmSub->ezrec->ezkcd, pZmSub->ezrec->ezecd);
//// 修正No.157858 add <--
// No.157994 del <--
// No.157994 add -->
			// 枝番残高の集計
			bVal1 = CmnICSGetSumEzrec(pZmSub, uRenInfo, &strSum);
			// 仕訳データに0円以上の仕訳で作成されているかどうかをチェック
			bVal2 = CmnICSChkDatarec(pZmSub, uRenInfo, pZmSub->ezrec->ezkcd, pZmSub->ezrec->ezecd);

			// どちらかがTRUEなら連動を行う
			bValue = FALSE;
			if(bVal1 == TRUE || bVal2 == TRUE)	{
				bValue = TRUE;
			}
// No.157994 add <--

			// 財務連動元の情報
			uRenInfo->strRenKcd	= pZmSub->ezrec->ezkcd;						// 財務連動元（科目）
			uRenInfo->lngRenEcd	= pZmSub->ezrec->ezecd;						// 財務連動元（枝番）
			cc.Divide( CmnICSGetEzrecEzname(pZmSub), strDividedName, 10 );	//（科目摘要11文字以上はカット）
			uRenInfo->strAdName	= strDividedName;							// 取引先名称
			uRenInfo->strVal	= strSum;									// 金額
// midori 152132 add -->
			uRenInfo->byteEdasgn = TRUE;
// midori 152132 add <--
// midori 152137 add -->
			uRenInfo->strAdKana = CmnICSGetEzrecEzkana(pZmSub);				// カナ
// midori 152137 add <--
			uRenInfo->strInvNo  = CmnICSGetTkrecInvno(pZmSub);				// 登録番号		インボイス登録番号追加対応_23/11/27 add

			bUpdateVal = FALSE;

// midori 161110_2 add -->
			strGokei.Empty();
			if(m_mapEdaZan.Lookup(_tstoi(pZmSub->ezrec->ezkcd), strGokei) == TRUE)	{
				CmnKingakuAddFunc(&strGokei, uRenInfo->strVal, 1);		// 金額を加算
			}
			else	{
				strGokei = uRenInfo->strVal;							// 金額をセット
			}
			m_mapEdaZan.SetAt(_tstoi(pZmSub->ezrec->ezkcd), strGokei);
// midori 161110_2 add <--

			//----------------------------------------
			//	金額のみ再連動する場合
			//----------------------------------------
			if (uRenInfo->intOpUpdType == ID_OPUPDTYPE_VALRENDO) {
				// 財務連動元の情報からデータを検索
				rsData->RequeryRenKcdRenEcd(uRenInfo->strRenKcd, uRenInfo->lngRenEcd);
				if (!rsData->IsEOF()) {
					// 内訳書データ更新
					virRendoUpdateData(rsData, uRenInfo);
					bUpdateVal = TRUE;

					// 合計欄をクリア
					strSum = _T("0");
				}
			}

			//----------------------------------------
			//	財務連動データを新規作成する場合 ･･･ 金額のみ再連動でも、更新データ無しの場合はデータ追加する
			//----------------------------------------
			if (bUpdateVal == FALSE) {
				// 金額が発生していない場合は、データ作成しない
				// 金額が発生して、残高０円の場合は、ダイアログの設定に従う
				if (bValue == TRUE) {
					if ((strSum.Trim() != "0") || (pCdlgRenMain->m_OpOutZeroEn == 1)) {
						// 内訳書データ作成
						virRendoCreateData(rsData, uRenInfo);

						// 合計欄をクリア
						strSum = _T("0");
					}
				}
			}
			// 次レコードを参照
			pZmSub->ezrec->MoveNext();
		}
		// 枝番残高テーブル閉じる
		pZmSub->EdabanClose();

// midori 161110_2 del -->
		//return FUNCTION_OK;
// midori 161110_2 del <--
// midori 161110_2 add -->
		bL_EndSw = TRUE;
// midori 161110_2 add <--
	}
	else{
		// 2007.03 DBエラー対応
		m_clsFunc.ZmSubErrMsg( pZmSub );
		return FUNCTION_NG;
	}

// midori 161110_2 add -->
	// 「枝番未登録･未入力のデータを連動する」にチェックが入っている場合、
	// 科目残高を取得し、上記で取得した枝番残高を引いた金額を取ってくる
	if(bL_EndSw == TRUE && pCdlgRenMain->m_OpEdaren == TRUE)	{

		// db仕訳テーブルのオープン
		if ( pZmSub->DataOpen(_T("")) != 0 ){
			m_clsFunc.ZmSubErrMsg( pZmSub );
			return FUNCTION_NG;
		}
		// 科目残高テーブルの抽出条件（kzrec）
		strFilter.Format("kzkcd >= '%s' AND kzkcd <= '%s'", uRenInfo->strKcdFrom, uRenInfo->strKcdTo);

		// 科目残高テーブルを取得（kzrec）
		if ( pZmSub->KamokuZanOpen(strFilter, 0) == 0 ){
			// 科目の範囲により複数レコード存在する
			for (i = 0; i < pZmSub->kzrec->GetRecordCount(); i++) {
				// 科目残高の集計
// No.157994 del -->
//				bValue = CmnICSGetSumKzrec(pZmSub, uRenInfo, &strSum);
// No.157994 del <--
// No.157994 add -->
				bVal1 = CmnICSGetSumKzrec(pZmSub, uRenInfo, &strSum);
// No.157994 add <--

				strGokei.Empty();
				if(m_mapEdaZan.Lookup(_tstoi(pZmSub->kzrec->kzkcd), strGokei) == TRUE)	{
					CmnKingakuAddFunc(&strSum, strGokei, -1);		// 金額を減算
				}

// No.157994 del -->
//// 修正No.157858 add -->
//				// 仕訳データに0円以上の仕訳で作成されているかどうかをチェック
//				bValue = CmnICSChkDatarec(pZmSub, uRenInfo, pZmSub->kzrec->kzkcd, 0);
//// 修正No.157858 add <--
// No.157994 del <--
// No.157994 add -->
				// 仕訳データに0円以上の仕訳で作成されているかどうかをチェック
				bVal2 = CmnICSChkDatarec(pZmSub, uRenInfo, pZmSub->kzrec->kzkcd, -1);

				// どちらかがTRUEなら連動を行う
				bValue = FALSE;
				if(bVal1 == TRUE || bVal2 == TRUE) {
					bValue = TRUE;
				}
// No.157994 add <--

				// 財務連動元の情報
				uRenInfo->strRenKcd = pZmSub->kzrec->kzkcd;									// 財務連動元（科目）
// 修正No.157630 del -->
				//uRenInfo->lngRenEcd = 0;													// 財務連動元（枝番）
// 修正No.157630 del <--
// 修正No.157630 add -->
				uRenInfo->lngRenEcd = -1;													// 財務連動元（枝番）
// 修正No.157630 add <--
				uRenInfo->strAdName = CmnICSGetKnrecKnnam(pZmSub, pZmSub->kzrec->kzkcd);	// 取引先名称
				uRenInfo->strVal    = strSum;												// 金額
// midori 152132 add -->
				uRenInfo->byteEdasgn = FALSE;
// midori 152132 add <--
// midori 152137 add -->
				uRenInfo->strAdKana = CmnICSGetKnrecKnKana(pZmSub, pZmSub->kzrec->kzkcd);	// 取引先カナ
// midori 152137 add <--
				uRenInfo->strInvNo  = _T("");												// 登録番号		インボイス登録番号追加対応_23/11/27 add

				bUpdateVal = FALSE;

				//----------------------------------------
				//	金額のみ再連動する場合
				//----------------------------------------
				if (uRenInfo->intOpUpdType == ID_OPUPDTYPE_VALRENDO) {
					// 財務連動元の情報からデータを検索
					rsData->RequeryRenKcdRenEcd(uRenInfo->strRenKcd, uRenInfo->lngRenEcd);
					if (!rsData->IsEOF()) {
						// 内訳書データ更新
						virRendoUpdateData(rsData, uRenInfo);
						bUpdateVal = TRUE;

						// 合計欄をクリア
						strSum = _T("0");
					}
				}

				//----------------------------------------
				//	財務連動データを新規作成する場合 ･･･ 金額のみ再連動でも、更新データ無しの場合はデータ追加する
				//----------------------------------------
				if (bUpdateVal == FALSE) {
					// 科目残高から枝番残高を減算した結果０円の場合はデータ作成しない
					// 減算した結果、借方、貸方に同額があって０円の場合は、ダイアログの設定に従う
					// 枝番未入力の仕訳の科目残高の借方、貸方共に０円の場合はデータ作成しない。
					// その判断は枝番未入力の仕訳の発生有り無しで行う。
					if(_tstoi(strSum) == 0)	{
						// 次レコードを参照
						strFilter.Empty();
						strFilter.Format(_T("((dbt = '%s' AND dbr is null) OR (cre = '%s' AND cbr is null)) AND (mofs >= %d AND mofs <= %d) AND (dbo.sbit_check(dsign,0x01000000,0x01000000)=0) AND val != 0"),
										pZmSub->kzrec->kzkcd,pZmSub->kzrec->kzkcd,uRenInfo->intStartOffset,uRenInfo->intEndOffset);
						pZmSub->data->Requery(strFilter,0,0);
						if(pZmSub->data->GetRecordCount() == 0)	bValue = FALSE;
					}

					// 金額が発生していない場合は、データ作成しない
					// 金額が発生して、残高０円の場合は、ダイアログの設定に従う
					if (bValue == TRUE) {
						if ((strSum.Trim() != "0") || (pCdlgRenMain->m_OpOutZeroEn == 1)) {
							// 内訳書データ作成
							virRendoCreateData(rsData, uRenInfo);			// 科目残高
							// 合計欄をクリア
							strSum = _T("0");
						}
					}
				}

				// 次レコードを参照
				pZmSub->kzrec->MoveNext();
			}
			// db仕訳テーブルのクローズ
			pZmSub->DataClose();

			// 枝番残高テーブル閉じる
			pZmSub->KamokuZanClose();

			return FUNCTION_OK;
		}
		else{
			// 2007.03 DBエラー対応
			m_clsFunc.ZmSubErrMsg( pZmSub );
			return FUNCTION_NG;
		}
	}
// midori 161110_2 add <--

	return FUNCTION_NG;
}

//********************************************************************************
//	財務連動：データ取込処理（仕訳データ：メイン）
//		IN		CdlgRenMain*	財務連動ダイアログ情報
//				CDBZmSub*		ICS財務データ情報
//				RENTEMP_INFO*	財務連動で使用するアプリ情報
//				CdbUc000Common* テーブル情報（ソート実行済み状態）
//		RET		int				実行結果（FUNCTION_OK, FUNCTION_NG）
//********************************************************************************
int CfrmUc000Common::CmnTblRendoSubDarecProc(CdlgRenMain* pCdlgRenMain,
											 CDBZmSub* pZmSub,
											 RENTEMP_INFO* uRenInfo,
											 CdbUc000Common* rsData
											 )
{
	CdbUcRenDarecTemp	rsTemp(m_pDB);		// uc_ren_darec_temp
	CString		strKnFilter;				// knrecの抽出条件
	CString		strDaFilter;				// darecの抽出条件
	CString		strDaFilter2;				// darecの抽出条件2(月範囲指定)
	CString		strDaFilter3;				// darecの抽出条件3(集計対象外月の除外)
	int			i;
	int			j;

	//【注意】
	//	仕訳データ取込では、"darec"→(科目で抽出)→"uc_ren_darec_temp"→(摘要で抽出)→"内訳書"とする
	//	当メイン関数では、"uc_ren_darec_temp"作成まで行う
	//	以降の処理は、サブ関数で実現する

	// 仕訳データの抽出条件2(月範囲指定)
	strDaFilter2.Empty();
	strDaFilter2.Format("(mofs >= %d AND mofs <= %d)", uRenInfo->intStartOffset, uRenInfo->intEndOffset);	// 0 -> intStartOffset intKimatsu -> intEndOffset

	// 仕訳データの抽出条件3(集計対象外月の除外)
	strDaFilter3.Empty();
	strDaFilter3 = CmnICSGetSykMonthQuery( pZmSub, uRenInfo );

	// 科目名称テーブルの抽出条件（knrec）
	strKnFilter.Format("kncod >= '%s' AND kncod <= '%s'", uRenInfo->strKcdFrom, uRenInfo->strKcdTo);

	// 科目名称テーブルを取得（knrec）
	if ( pZmSub->KamokuMeisyoOpen(strKnFilter, 0) == 0 ){
		for (i = 0; i < pZmSub->knrec->GetRecordCount(); i++) {
			// Tempテーブルクリア
			rsTemp.DeleteAllRecord();

			// 仕訳データの抽出条件（darec/dtrec）
			strDaFilter.Empty();
			strDaFilter.Format("(dbt = '%s' OR cre = '%s')", pZmSub->knrec->kncod, pZmSub->knrec->kncod);

			// 仕訳データの抽出条件の結合(1+2)
			strDaFilter += _T(" AND ");
			strDaFilter += strDaFilter2;

			// 仕訳データの抽出条件の結合(1+2+3)
			if ( strDaFilter3 != "" ){
				strDaFilter += _T(" AND ");
				strDaFilter += strDaFilter3;
			}

			// 仕訳データを取得（darec/dtrec）→ Tempテーブル作成
			if(pZmSub->DataOpen(strDaFilter, 0, 1) == 0) {
				for(j=0; j<pZmSub->data->GetRecordCount(); j++) {
					// 仕訳データの取消分は不要
					if(CmnICSGetDarecDeleteRec(pZmSub) == FALSE) {
						// Tempテーブルのデータ作成
						if(rsTemp.CreateNewRecord(pZmSub->data->seq) == DB_ERR_OK) {
							rsTemp.Edit();
							rsTemp.m_ddate	= pZmSub->data->ddate;
							rsTemp.m_dbt	= pZmSub->data->dbt.Trim();
							rsTemp.m_cre	= pZmSub->data->cre.Trim();
// 230227 cor -->
//							rsTemp.m_val	= pZmSub->data->val.Trim();
// --------------
							rsTemp.m_val	= InvoiceCalc(pZmSub);
// 230227 cor <--
							rsTemp.m_tekiyo	= pZmSub->data->tekiyo.Trim();
							rsTemp.m_invono	= pZmSub->data->invno.TrimRight();	// インボイス登録番号追加対応_23/11/27 add

							rsTemp.Update();
						}
					}
					// 次レコードを参照（darec/dtrec）
					pZmSub->data->MoveNext();
				}
			}
			else{
				// 2007.03 DBエラー対応
				m_clsFunc.ZmSubErrMsg( pZmSub );
				return FUNCTION_NG;
			}
			rsTemp.Fin();

			// 仕訳データ閉じる（darec/dtrec）
			pZmSub->DataClose();

			// 残高集計～内訳書データ作成
			CmnTblRendoSubDarecSub(pCdlgRenMain, pZmSub, uRenInfo, rsData);

			// 次レコードを参照（knrec）
			pZmSub->knrec->MoveNext();
		}
		// 科目名称テーブル閉じる（knrec）
		pZmSub->KamokuMeisyoClose();
	}
	else{
		// 2007.03 DBエラー対応
		m_clsFunc.ZmSubErrMsg( pZmSub );
		return FUNCTION_NG;
	}

	return FUNCTION_OK;
}

//********************************************************************************
//	財務連動：データ取込処理（仕訳データ：サブ）
//		IN		CdlgRenMain*	財務連動ダイアログ情報
//				CDBZmSub*		ICS財務データ情報
//				RENTEMP_INFO*	財務連動で使用するアプリ情報
//				CdbUc000Common* テーブル情報（ソート実行済み状態）
//		RET		int				実行結果（FUNCTION_OK, FUNCTION_NG）
//********************************************************************************
int CfrmUc000Common::CmnTblRendoSubDarecSub(CdlgRenMain* pCdlgRenMain,
											CDBZmSub* pZmSub,
											RENTEMP_INFO* uRenInfo,
											CdbUc000Common* rsData
											)
{
	CdbUcRenDarecTemp	rsTemp(m_pDB);		// uc_ren_darec_temp
	CString		strTkFilter;				// tkrecの抽出条件
	CString		strTkName;					// tkrecの名称
// midori 152137 add -->
	CString		strTkKana=_T("");			// カナ
// midori 152137 add <--
	CString		strTkInvNo=_T("");			// 登録番号		インボイス登録番号追加対応_23/11/27 add
	CString		strSum = _T("0");			// 金額
// 23/12/20_168441 add -->
	CString		strSave=_T("");
	BOOL		bMixed=FALSE;
// 23/12/20_168441 add <--

	BOOL		bValue = FALSE;				// 集計フラグ（TRUE:合算必要，FALSE:不要）
	BOOL		bTemp;
	int			j;
	int			intRet;

	// 50音摘要テーブルの抽出条件（tkrec）
	strTkFilter.Format("tkcod >= %d AND tkcod <= %d", pCdlgRenMain->m_OpTkcodFrom, pCdlgRenMain->m_OpTkcodTo);

	// 50音摘要テーブルを取得（tkrec）
	if ( pZmSub->KanaTekiyoOpen(strTkFilter, 0) == 0 ){
		for (j = 0; j < pZmSub->tkrec->GetRecordCount(); j++) {
			// 50音摘要テーブルの名称がNULL以外
			strTkName = pZmSub->tkrec->tkname.Trim();
// midori 152137 add -->
			// カナを取得
			strTkKana = pZmSub->tkrec->tkana.TrimRight();
// midori 152137 add <--
			//strTkInvNo = pZmSub->tkrec->invno.TrimRight();					// 登録番号		インボイス登録番号追加対応_23/11/27 add
			if (strTkName != "") {
				bValue = FALSE;

				// Tempテーブルを摘要で抽出
				intRet = rsTemp.RequeryFgConvLikeTekiyo(0, strTkName);
				if (intRet == DB_ERR_OK) {
// 23/12/20_168441 add -->
					strSave = rsTemp.m_invono;
					bMixed = FALSE;
// 23/12/20_168441 add <--
					while (!rsTemp.IsEOF()) {
						// Tempテーブルの残高取得
						//bTemp = CmnICSGetSumDarec(&rsTemp, pZmSub->knrec, &strSum);
						bTemp = CmnICSGetSumDarec(&rsTemp, pZmSub->knrec, &strSum, &strTkInvNo);
						if ((bTemp == TRUE) || (pCdlgRenMain->m_OpOutZeroEn == 1)) {
							// 発生金額あり or 発生０円でもデータ作成する
							if (pCdlgRenMain->m_OpGassan == 0) {
								// 必要情報の取得
								uRenInfo->strAdName = strTkName;					// 取引先名称
								uRenInfo->strVal    = strSum;						// 金額
// midori 152137 add -->
								uRenInfo->strAdKana = strTkKana;					// カナ
// midori 152137 add <--
								uRenInfo->strInvNo  = strTkInvNo;					// 登録番号		インボイス登録番号追加対応_23/11/27 add

								// 内訳書データ作成
								virRendoCreateData(rsData, uRenInfo);				// データ作成
								strSum = _T("0");									// 合計欄をクリア
								strTkInvNo = _T("");								// 登録番号をクリア
							}
							else {
								// 後で合算データ作成が必要
								bValue = TRUE;
// 23/12/20_168441 add -->
								if(strSave.IsEmpty() == FALSE && strTkInvNo.IsEmpty() == FALSE)	{
									if(strSave != strTkInvNo)	{
										bMixed = TRUE;
									}
									strSave = strTkInvNo;
								}
// 23/12/20_168441 add <--
							}
						}

						// データ取込フラグをON（同じデータを２重計上させない）
						rsTemp.Edit();
						rsTemp.m_FgConv = 1;
						rsTemp.Update();

						// 次レコードを参照
						rsTemp.MoveNext();
					}

					// 合算する場合は、ここでデータ作成
					if ((pCdlgRenMain->m_OpGassan != 0) && (bValue == TRUE)) {
						// 合算時は、合計０円なら０円データ作成の設定に従う
						if ((strSum.Trim() != "0") || (pCdlgRenMain->m_OpOutZeroEn == 1)) {
							// 必要情報の取得
							uRenInfo->strAdName = strTkName;					// 取引先名称
							uRenInfo->strVal    = strSum;						// 金額
// midori 152137 add -->
							uRenInfo->strAdKana = strTkKana;					// カナ
// midori 152137 add <--
// 23/12/20_168441 add -->
							// 合算時に登録番号が異なる場合は空欄で作成する
							if(bMixed == TRUE)	{
								strTkInvNo=_T("");
							}
// 23/12/20_168441 add <--
							uRenInfo->strInvNo  = strTkInvNo;					// 登録番号		インボイス登録番号追加対応_23/11/27 add

							// 内訳書データ作成
							virRendoCreateData(rsData, uRenInfo);				// データ作成
							strSum = _T("0");									// 合計欄をクリア
							strTkInvNo = _T("");								// 登録番号をクリア
						}
					}
				}
			}
			// 次レコードを参照
			pZmSub->tkrec->MoveNext();
		}
		// 50音摘要テーブル閉じる
		pZmSub->KanaTekiyoClose();

		// データ取込漏れ（仕訳データの集計漏れを防ぐ）
		intRet = rsTemp.RequeryFgConv(0);
		if (intRet == DB_ERR_OK) {
			while (!rsTemp.IsEOF()) {
				// 摘要が一致しなかったデータは、明細ごとに出力するので、必ずクリアする
				strSum = _T("0");
				strTkInvNo = _T("");

				// 仕訳データの残高取得（Tempテーブル）：対象漏れデータは合算しない
				//bTemp = CmnICSGetSumDarec(&rsTemp, pZmSub->knrec, &strSum);
				bTemp = CmnICSGetSumDarec(&rsTemp, pZmSub->knrec, &strSum, &strTkInvNo);
				if ((bTemp == TRUE) || (pCdlgRenMain->m_OpOutZeroEn == 1)) {
					// 必要情報の取得
					uRenInfo->strAdName = _T("");						// 取引先名称
					uRenInfo->strVal    = strSum;						// 金額
// midori 152137 add -->
					uRenInfo->strAdKana = _T("");
// midori 152137 add <--
					uRenInfo->strInvNo  = strTkInvNo;					// 登録番号		インボイス登録番号追加対応_23/11/27 add

					// 内訳書データ作成
					virRendoCreateData(rsData, uRenInfo);				// データ作成
					strSum = _T("0");									// 合計欄をクリア
					strTkInvNo = _T("");								// 登録番号をクリア
				}
				// 次レコードを参照
				rsTemp.MoveNext();
			}
		}
	}
	else{
		// 2007.03 DBエラー対応
		m_clsFunc.ZmSubErrMsg( pZmSub );
		return FUNCTION_NG;
	}

	rsTemp.Fin();

	return FUNCTION_OK;
}

//********************************************************************************
//	一括で一時保管へ移動：金額欄０円行、空欄行を一時保管移動処理（メイン）
//		IN		
//		RET		int				実行結果（FUNCTION_OK, FUNCTION_NG）
//********************************************************************************
int CfrmUc000Common::CmnTblZeroMoneyMoveMainProc( HOKAN_DATA *hData )
{
	CfrmUc000Common*	pView = NULL;			// FormViewオブジェクト
	CdbUc000Common*		pData1 = NULL;

	// 帳表分ループ
	for ( int i = 0; i < UC_ID_FORMNO_MAX; i++ ){

		// ０円行も、空欄行チェックが最低１つチェックされている時のみ実行
		if ( ( hData[i].blnZeroFg == TRUE ) || ( hData[i].blnNullFg == TRUE ) ){
			// 一応、FormSeq番号が取れているかも見る
			if ( hData[i].intFormSeq > 0 ){

				//******************************************************
				// 様式分ループする。なお、主な処理は以下の２つ
				// 　①データ行を一時保管へ移動させる
				//	 ②頁調整する
				//*******************************************************

				// 各帳表のFormViewオブジェクト取得
				pView = ((CUCHIWAKEApp*)AfxGetApp())->UchiwakeCreateFormViewObject(hData[i].intFormSeq);		// FormViewオブジェクト
				if ( pView->CmnSetInitialUchiwakeInfo(hData[i].intFormSeq) != TRUE ){	// 初期設定（共通データ）
					if ( pView != NULL ){
						delete pView;
					}
					return FUNCTION_NG;
				}
				pView->virSetInitialValue();									// 初期設定（帳表固有データ）

// midori 156968 add -->
				// 最終行（頁の最大行）が小計の場合、小計の下に空行を追加する
				pView->InsertNull();
// midori 156968 add <--

				// dbオブジェクト取得
				pData1 = pView->virGetDataTableObject();

				// ①データ行を一時保管へ移動させる
				if ( pData1->UpdateZeroMoneyMoveToHokan( &hData[i] ) != DB_ERR_OK ){
					delete pView;
					delete pData1;
					return FUNCTION_NG;
				}

// midori 152150 add -->
				// ・一時保管へ移動した結果、０円になった【小計】を削除する
				if(DeleteZeroSyokei(pData1) != DB_ERR_OK)	{
					delete pView;
					delete pData1;
					return FUNCTION_NG;
				}
// midori 152150 add <--

				// ②頁調整する
				pView->CmnTblUpdatePageRowAllResetMain();

// midori 156968 add -->
				// 先頭行が空行の場合削除する
				pView->CmnTblUpdatePageRowAllResetMain(6);
// midori 156968 add <--

				// ビュー＆テーブルのメモリ領域を解放
				if (pData1 != NULL) {
					delete pData1;
				}
				if (pView != NULL) {
					delete pView;
				}

			}
		}
	}
	
	return FUNCTION_OK;
}

// midori 152150 add -->
//********************************************************************************
//	一時保管へ移動した結果、不要になった小計(中計)行を削除する
//	【引数】	CdbUc000Common*	各様式テーブル
//	【戻り値】	int				実行結果（FUNCTION_OK, FUNCTION_NG）
//********************************************************************************
int CfrmUc000Common::DeleteZeroSyokei(CdbUc000Common* rsData)
{
	int				intMax=0;
	int				ii=0;
	int				sw=0;
	int				fcsv=0;
	int				scnt=0;			// 小計 ～ 小計のデータ数
	int				tcnt=0;			// 中計 ～ 中計のデータ数
// midori 156189,156190,156191 del -->
//// midori 190505 add -->
//	int				nKnOrder=0;
//// midori 190505 add <--
// midori 156189,156190,156191 del <--
	CString			strTemp=_T("");
	CALCKEI_INFO	uCalcInfo;
// midori 156189,156190,156191 del -->
//// midori 190505 add -->
//	SORTKAMOKU_INFO	uKamoku;
//// midori 190505 add <--
// midori 156189,156190,156191 del <--

	uCalcInfo = m_uInfo.uCalcInfo;
	intMax = uCalcInfo.intMaxCount;

	// ①金額０円の小計、中計を削除する
	scnt = 0;
	tcnt = 0;
	sw = 0;
	rsData->RequeryFgShowSortPageRow(ID_FGSHOW_OFF);
	while(!rsData->IsEOF())	{
		switch(rsData->m_FgFunc)	{
			case	ID_FGFUNC_NULL:			// 空行
				if(sw != 0)	{
					rsData->Delete();
				}
				sw = 0;
				break;
			case	ID_FGFUNC_DATA:			// データ行
			case	ID_FGFUNC_IKKATUMANUAL:	// 一括集計金額行（手動）
			case	ID_FGFUNC_IKKATUAUTO:	// 一括集計金額行（自動）
				scnt++;
				tcnt++;
				sw = 0;
				break;
			case	ID_FGFUNC_SYOKEI:		// 小計行
			case	ID_FGFUNC_SYOKEINULL:	// 小計行（空行）
				if(scnt == 0)	{
					// データの発生が無ければ削除する
					rsData->Delete();
					sw = 1;
				}
				scnt = 0;
				break;
			case	ID_FGFUNC_CHUKEI:		// 中計行
			case	ID_FGFUNC_CHUKEINULL:	// 中計行（空行）
				if(tcnt == 0)	{
					// データの発生が無ければ削除する
					rsData->Delete();
					sw = 1;
				}
				tcnt = 0;
				break;
			default:
				sw = 0;
				break;
		}
		rsData->MoveNext();
	}
// midori 190505 add -->
	// ②金額０円の科目行を削除する
// 改良No.21-0086,21-0529 cor -->
	//if(m_uInfo.intFormSeq == ID_FORMNO_021 || m_uInfo.intFormSeq == ID_FORMNO_111){
// ------------------------------
	if(m_uInfo.intFormSeq == ID_FORMNO_021 || (bG_InvNo == TRUE && m_uInfo.intFormSeq == ID_FORMNO_081) || m_uInfo.intFormSeq == ID_FORMNO_111)	{
// 改良No.21-0086,21-0529 cor <--
		scnt = 0;
		rsData->RequeryFgShowSortPageRow(ID_FGSHOW_OFF);
		rsData->MoveLast();
		while(!rsData->IsBOF())	{
			switch(rsData->m_FgFunc)	{
				case	ID_FGFUNC_DATA:			// データ行
				case	ID_FGFUNC_IKKATUMANUAL:	// 一括集計金額行（手動）
				case	ID_FGFUNC_IKKATUAUTO:	// 一括集計金額行（自動）
// midori 156189,156190,156191 del -->
					//// 科目データの取得
					//virTblGetKamokuInfo(rsData, &uKamoku);
					//if(nKnOrder != uKamoku.intKnOrder) {
					//	scnt = 0;
					//}
					//nKnOrder = uKamoku.intKnOrder;
// midori 156189,156190,156191 del <--
					scnt++;
					break;
				case	ID_FGFUNC_KAMOKU:		// 科目行
// midori 156189,156190,156191 del -->
					//virTblGetKamokuInfo(rsData, &uKamoku);
					//if(nKnOrder != uKamoku.intKnOrder) {
					//	scnt = 0;
					//}
					//nKnOrder = uKamoku.intKnOrder;
// midori 156189,156190,156191 del <--
					if(scnt == 0)	{
						// データの発生が無ければ削除する
						rsData->Delete();
					}
					scnt = 0;
					break;
				default:
					break;
			}
			rsData->MovePrev();
		}
	}
// midori 190505 add <--

	//// ②不要な小計行[空行]、中計行[空行]を削除する
	//// （※前の行が通常(1)、一括集計金額行[手動](21)、一括集計金額行[自動](22) 以外は不要と判断する）
	//fcsv=-1;
	//rsData->RequeryFgShowSortPageRow(ID_FGSHOW_OFF);
	//while(!rsData->IsEOF())	{
	//	if(rsData->m_FgFunc == ID_FGFUNC_SYOKEINULL || rsData->m_FgFunc == ID_FGFUNC_CHUKEINULL)	{
	//		if( (fcsv == -1) || 
	//			(fcsv != ID_FGFUNC_DATA && fcsv != ID_FGFUNC_IKKATUMANUAL && fcsv != ID_FGFUNC_IKKATUAUTO))	{
	//			// 削除する
	//			rsData->Delete();
	//		}
	//	}
	//	fcsv = rsData->m_FgFunc;
	//	rsData->MoveNext();
	//}

	return(FUNCTION_OK);
}
// midori 152150 add <--

// midori 170502 del -->
////**************************************************
////	データインポートの後処理(頁調整)を行う
////		IN		
////		RET		int				実行結果（FUNCTION_OK, FUNCTION_NG）
////**************************************************
//int CfrmUc000Common::CmnDataImportPostProcess()
//{
//	CfrmUc000Common*	pView = NULL;			// FormViewオブジェクト
//	int					intFormSeq = 0;
//
//	// 2009.08.25 Update
//	//	インポート対象は、1～16の内訳書（17～18:その他は対象外）
//	//	　→ 17＆18のページ調整を実施した場合、Primary Keyエラーが発生する事があるため対象外とする
//	for ( intFormSeq = ID_FORMNO_011; intFormSeq <= ID_FORMNO_162; intFormSeq++ ){
//
//		// 各帳表のFormViewオブジェクト取得
//		pView = ((CUCHIWAKEApp*)AfxGetApp())->UchiwakeCreateFormViewObject(intFormSeq);		// FormViewオブジェクト
//		if ( pView->CmnSetInitialUchiwakeInfo(intFormSeq) != TRUE ){	// 初期設定（共通データ）
//			if ( pView != NULL ){
//				delete pView;
//			}
//			return FUNCTION_NG;
//		}
//		pView->virSetInitialValue();											// 初期設定（帳表固有データ）
//
//		// 頁調整する
//		pView->CmnTblUpdatePageRowAllResetMain();
//
//		// ビューのメモリ領域を解放
//		if (pView != NULL) {
//			delete pView;
//		}
//	}
//	
//	return FUNCTION_OK;
//}
// midori 170502 del <--

// 修正No.157858 add -->
//********************************************************************************
//	ICS財務処理：仕訳データの有無（Datarec）
//		IN		CDBZmSub*		財務DBクラス
//				RENTEMP_INFO*	財務連動で使用するアプリ情報
//				CString			科目コード
//				int				枝番コード
//		RET		BOOL		TRUE :発生あり
//							FALSE:なし
//********************************************************************************
BOOL CfrmUc000Common::CmnICSChkDatarec(CDBZmSub* pZmSub, RENTEMP_INFO* uRenInfo, CString pKcod, int pEcod)
{
	BOOL		bRet = FALSE;
	CString		strFilter="", strFilter2="", strFilter3="";

	// 仕訳データの抽出条件（科目指定）
// No.157994 del -->
//	if(pEcod == 0) {
// No.157994 del <--
// No.157994 add -->
	if(pEcod == -1)	{
// No.157994 add <--
		strFilter.Empty();
		strFilter.Format("(dbt = '%s' OR cre = '%s')", pKcod, pKcod);
	}
	else	{
		strFilter.Empty();
		strFilter.Format("((dbt = '%s' AND dbr = %d) OR (cre = '%s' AND cbr=%d))", pKcod, pEcod, pKcod, pEcod);
	}

	// 仕訳データの抽出条件2(月範囲指定)
	strFilter2.Empty();
	strFilter2.Format("(mofs >= %d AND mofs <= %d)", uRenInfo->intStartOffset, uRenInfo->intEndOffset);

	// 条件文を結合
	strFilter += _T(" AND ");
	strFilter += strFilter2;

	// 仕訳データの抽出条件3(集計対象外月の除外)
	strFilter3.Empty();
	strFilter3 = CmnICSGetSykMonthQuery(pZmSub, uRenInfo);

	// 条件文を結合
	if(strFilter3.IsEmpty() == FALSE) {
		strFilter += _T(" AND ");
		strFilter += strFilter3;
	}

	// 削除仕訳を省く条件文を追加
	strFilter += _T(" AND (dbo.sbit_check(dsign,0x01000000,0x01000000)=0)");

	// 金額0以上の仕訳を取得する条件を追加
	strFilter += _T(" AND val != 0");

	// 返送値
	bRet = FALSE;

	// 仕訳データを取得（datarec）
	if(pZmSub->DataOpen(strFilter, 0, 1) == 0)	{
		if(pZmSub->data->GetRecordCount() > 0)	{
			bRet = TRUE;
		}
	}
	else	{
		m_clsFunc.ZmSubErrMsg( pZmSub );
		return(FALSE);
	}

	return(bRet);
}
// 修正No.157858 add <--

//********************************************************************************
//	ICS財務処理：科目残高の残高取得（kzrec）
//		IN		CDBZmSub*		財務DBクラス
//				RENTEMP_INFO*	財務連動で使用するアプリ情報
//				CString*	集計残高（集計結果で更新）
//		RET		BOOL		TRUE: 発生金額あり
//							FALSE:なし
//********************************************************************************
BOOL CfrmUc000Common::CmnICSGetSumKzrec( CDBZmSub* pZmSub, RENTEMP_INFO* uRenInfo, CString* pSumZan)
{
	CString		strSum_l = _T("0");			// 借方合計
	CString		strSum_r = _T("0");			// 貸方合計
	CString		strResult = _T("0");		// 集計結果
	CString		strTemp1;
	CString		strTemp2;
	BOOL		bValue = FALSE;
	int			i;

	*pSumZan = _T("0");

	// 借方発生金額と貸方発生金額を集計（0～15：12ヶ月＋四半期毎の決算フィールド）→集計期間内
	for (i = uRenInfo->intStartOffset; i <= uRenInfo->intEndOffset; i++) {
		// 集計対象なら加算
		if ( uRenInfo->blnSykFlg[i] == TRUE ){
			// 借方発生金額を加算
			strTemp1 = pZmSub->kzrec->lhs[i].Trim();
			CmnKingakuAddFunc(&strSum_l, strTemp1, 1);				// 加算

			// 貸方発生金額を加算
			strTemp2 = pZmSub->kzrec->rhs[i].Trim();
			CmnKingakuAddFunc(&strSum_r, strTemp2, 1);				// 加算

			// 発生金額あり？
			if ((strTemp1 != "0") || (strTemp2 != "0")) {
				bValue = TRUE;
			}
		}
	}

	// 期首残高取得＋発生金額あり？
	strTemp1 = pZmSub->kzrec->kzzan.Trim();
	if (strTemp1 != "0") {
		bValue = TRUE;
	}

	// 科目残高の集計
	if ((pZmSub->kzrec->kzatt & 0x01) != 0x01) {						// 0x01=貸方
		// 借方科目：期首残高＋借方発生合計－貸方発生合計
		CmnKingakuAddFunc(&strResult, strTemp1, 1);				// 期首残高：加算
		CmnKingakuAddFunc(&strResult, strSum_l, 1);				// 借方合計：加算
		CmnKingakuAddFunc(&strResult, strSum_r, -1);			// 貸方合計：減算
	}
	else {	
		// 貸方科目：期首残高－借方発生合計＋貸方発生合計
		CmnKingakuAddFunc(&strResult, strTemp1, 1);				// 期首残高：加算
		CmnKingakuAddFunc(&strResult, strSum_l, -1);			// 借方合計：減算
		CmnKingakuAddFunc(&strResult, strSum_r, 1);				// 貸方合計：加算
	}

	*pSumZan = strResult;
	return (bValue);
}

//********************************************************************************
//	ICS財務処理：枝番残高の残高取得（Ezrec）･･･	科目残高集計とほぼ同等の処理
//		IN		CDBZmSub*		DB財務クラス
//				RENTEMP_INFO*	財務連動で使用するアプリ情報
//		RET		BOOL		TRUE: 発生金額あり
//							FALSE:なし
//********************************************************************************
BOOL CfrmUc000Common::CmnICSGetSumEzrec(CDBZmSub* pZmSub, RENTEMP_INFO* uRenInfo, CString* pSumZan)
{
	CString		strSum_l = _T("0");			// 借方合計
	CString		strSum_r = _T("0");			// 貸方合計
	CString		strResult = _T("0");		// 集計結果
	CString		strTemp1;
	CString		strTemp2;
	BOOL		bValue = FALSE;
	int			i;

	*pSumZan = _T("0");

	// 借方発生金額と貸方発生金額を集計（0～15：12ヶ月＋四半期毎の決算フィールド）→集計期間内
	for (i = uRenInfo->intStartOffset; i <= uRenInfo->intEndOffset; i++) {
		// 集計対象なら加算
		if ( uRenInfo->blnSykFlg[i] == TRUE ){
			// 借方発生金額を加算
			strTemp1 = pZmSub->ezrec->lhs[i].Trim();
			CmnKingakuAddFunc(&strSum_l, strTemp1, 1);				// 加算

			// 貸方発生金額を加算
			strTemp2 = pZmSub->ezrec->rhs[i].Trim();
			CmnKingakuAddFunc(&strSum_r, strTemp2, 1);				// 加算

			// 発生金額あり？
			if ((strTemp1 != "0") || (strTemp2 != "0")) {
				bValue = TRUE;
			}
		}
	}

	// 期首残高取得
	strTemp1 = pZmSub->ezrec->ezzan.Trim();
	if (strTemp1 != "0") {
		bValue = TRUE;
	}

	// 科目残高の集計
	if ((pZmSub->ezrec->ezatt & ID_ICS_KNATT_KASI) != ID_ICS_KNATT_KASI) {			// 0x01=貸方
		// 借方科目：期首残高＋借方発生合計－貸方発生合計
		CmnKingakuAddFunc(&strResult, strTemp1, 1);				// 期首残高：加算
		CmnKingakuAddFunc(&strResult, strSum_l, 1);				// 借方合計：加算
		CmnKingakuAddFunc(&strResult, strSum_r, -1);			// 貸方合計：減算
	}
	else {
		// 貸方科目：期首残高－借方発生合計＋貸方発生合計
		CmnKingakuAddFunc(&strResult, strTemp1, 1);				// 期首残高：加算
		CmnKingakuAddFunc(&strResult, strSum_l, -1);			// 借方合計：減算
		CmnKingakuAddFunc(&strResult, strSum_r, 1);				// 貸方合計：加算
	}

	*pSumZan = strResult;
	return (bValue);
}

//********************************************************************************
//	ICS財務処理：仕訳データの残高取得（darec/dtrec）
//		IN		CDATA*		仕訳データ
//				CKNREC*		対象科目情報
//				CString*	集計残高（集計結果で更新）
//				CString*	登録番号（集計結果で更新）
//		RET		BOOL		TRUE: 発生金額あり
//							FALSE:なし
//********************************************************************************
//BOOL CfrmUc000Common::CmnICSGetSumDarec(CdbUcRenDarecTemp* pTemp, CKNREC* pKnrec, CString* pSumZan)					// インボイス登録番号追加対応_23/11/27 del
BOOL CfrmUc000Common::CmnICSGetSumDarec(CdbUcRenDarecTemp* pTemp, CKNREC* pKnrec, CString* pSumZan, CString* pInvno)	// インボイス登録番号追加対応_23/11/27 add
{
	CString		strTemp;
	BOOL		bKnatt;				// 対象科目の貸借区分
	BOOL		bValue = FALSE;		// 発生金額有無

	// 対象科目の貸借区分を取得
	bKnatt = pKnrec->knatt & ID_ICS_KNATT_KASI;

	// 発生金額取得
	strTemp = pTemp->m_val.Trim();
	if (strTemp != "0") {
		bValue = TRUE;				// 発生金額あり

		// 残高集計（加算/減算）
		if (((bKnatt != ID_ICS_KNATT_KASI) && (pTemp->m_dbt == pKnrec->kncod)) || 
			((bKnatt == ID_ICS_KNATT_KASI) && (pTemp->m_cre == pKnrec->kncod))) {
			// 借方科目で借方発生 or 貸方科目で貸方発生
			CmnKingakuAddFunc(pSumZan, strTemp, 1);				// 加算
		}
		else {
			// 借方科目で貸方発生 or 貸方科目で借方発生
			CmnKingakuAddFunc(pSumZan, strTemp, -1);			// 減算
		}
	}

	// 登録番号もここで返す
	*pInvno = pTemp->m_invono.TrimRight();

	return (bValue);
}

//********************************************************************************
//	ICS財務処理:仕訳データの削除レコードか判断を行う
//		IN		CDBZmSub*			ICS財務クラス（仕訳データを参照できる状態）
//		RET		BOOL				TRUE=削除レコード, FALSE=通常レコード
//********************************************************************************
BOOL CfrmUc000Common::CmnICSGetDarecDeleteRec(CDBZmSub* pZmSub)
{
	//----------------------------------------------------------------
	//	詳細は「財務データベース仕様書1_20070129.doc」を参照
	//	・darec->dsign[0]
	//			BIT0	　：取消し（1=取消し，0=通常）
	//			BIT1～BIT7：別途目的で使用
	//----------------------------------------------------------------
	// dsign[0]を参照
	BYTE cDsign0 = pZmSub->data->dsign.GetAt(0);
	if ((cDsign0 & 0x01) == 0x01) {
		// 削除データ
		return TRUE;
	}

	return FALSE;	
}

//********************************************************************************
//	ICS財務処理：財務科目コードから名称を取得
//		IN		CDBZmSub*		DB財務クラス
//				CString			財務科目コード
//		RET		CString			財務科目名称
//********************************************************************************
CString CfrmUc000Common::CmnICSGetKnrecKnnam(CDBZmSub* pZmSub, CString strKcd)
{
	CString		strFilter = _T("");
	CString		strName = _T("");

	// 科目名称テーブルの抽出条件（knrec）
	if (strKcd != "") {
		strFilter.Format("kncod = '%s'", strKcd);
	}

	// 科目名称テーブルを取得（knrec）
	if ( pZmSub->KamokuMeisyoOpen( strFilter , 0 ) == 0 ){
		// 科目名称を取得
		strName = pZmSub->knrec->knnam;

		// 科目名称テーブル閉じる
		pZmSub->KamokuMeisyoClose();
	}
	else{
		// 2007.03 DBエラー対応
		m_clsFunc.ZmSubErrMsg( pZmSub );
		return strName;
	}

	// 科目名称テーブル開放
	pZmSub->KamokuMeisyoFree();

	return strName;
}

// midori 152137 add -->
//********************************************************************************
//	ICS財務処理：財務科目コードからカナを取得
//		IN		CDBZmSub*		DB財務クラス
//				CString			財務科目コード
//		RET		CString			財務科目カナ
//********************************************************************************
CString CfrmUc000Common::CmnICSGetKnrecKnKana(CDBZmSub* pZmSub, CString strKcd)
{
	CString		strFilter = _T("");
	CString		strName = _T("");

	// 科目名称テーブルの抽出条件（knrec）
	if (strKcd != "") {
		strFilter.Format("kncod = '%s'", strKcd);
	}

	// 科目名称テーブルを取得（knrec）
	if ( pZmSub->KamokuMeisyoOpen( strFilter , 0 ) == 0 ){
		// 科目名称を取得
		strName = pZmSub->knrec->knkana;

		// 科目名称テーブル閉じる
		pZmSub->KamokuMeisyoClose();
	}
	else{
		// 2007.03 DBエラー対応
		m_clsFunc.ZmSubErrMsg( pZmSub );
		return(strName);
	}

	// 科目名称テーブル開放
	pZmSub->KamokuMeisyoFree();

	strName.TrimRight();
	return(strName);
}
// midori 152137 add <--

//********************************************************************************
//	ICS財務処理：枝番残高から名称を取得
//		IN		CDBZmSub*		DB財務クラス
//		RET		CString			財務科目名称
//********************************************************************************
CString CfrmUc000Common::CmnICSGetEzrecEzname(CDBZmSub* pZmSub)
{
	CString		strFilter = _T("");
	CString		strName = _T("");
	int			intRet;

	//	【大前提】
	// 当処理実行時に、pZmSub->ezrec は対象レコードを指していること

	// 摘要コードから名称を取得
	if (pZmSub->ezrec->eztcd >= 0) {
		// 50音摘要テーブルの抽出条件（tkrec）
		strFilter.Format("tkcod = %d", pZmSub->ezrec->eztcd);

		// 50音摘要テーブルを取得（tkrec）
		intRet = m_clsFunc.GetTekiyoData( pZmSub , pZmSub->ezrec->eztcd , &strName );
		if (intRet == 0) {
			return strName;
		}
	}

	// 枝番残高の名称を使用する
	return pZmSub->ezrec->ezname;
}

// midori 152137 add -->
//********************************************************************************
//	ICS財務処理：枝番残高からカナを取得
//		IN		CDBZmSub*		DB財務クラス
//		RET		CString			枝番カナ
//********************************************************************************
CString CfrmUc000Common::CmnICSGetEzrecEzkana(CDBZmSub* pZmSub)
{
	CString		strFilter = _T("");
	CString		strName = _T("");
	int			intRet;

	//	【大前提】
	// 当処理実行時に、pZmSub->ezrec は対象レコードを指していること

	// 摘要コードから名称を取得
	if (pZmSub->ezrec->eztcd >= 0) {
		// 50音摘要テーブルの抽出条件（tkrec）
		strFilter.Format("tkcod = %d", pZmSub->ezrec->eztcd);

		// 50音摘要テーブルを取得（tkrec）
		intRet = m_clsFunc.GetTekiyoKana( pZmSub , pZmSub->ezrec->eztcd , &strName );
		if (intRet == 0) {
			strName.TrimRight();
			return(strName);
		}
	}

	// 枝番残高のカナを使用する
	strName = pZmSub->ezrec->ezkana;
	strName.TrimRight();
	return(strName);
}
// midori 152137 add <--

// インボイス登録番号追加対応_23/11/27 add -->
//********************************************************************************
//	ICS財務処理：枝番残高→摘要テーブルから登録番号を取得
//		IN		CDBZmSub*		DB財務クラス
//		RET		CString			枝番カナ
//********************************************************************************
CString CfrmUc000Common::CmnICSGetTkrecInvno(CDBZmSub* pZmSub)
{
	CString		strFilter = _T("");
	CString		strName = _T("");
	int			intRet;

	//	【大前提】
	// 当処理実行時に、pZmSub->ezrec は対象レコードを指していること

	// 摘要コードから名称を取得
	if(pZmSub->ezrec->eztcd >= 0)	{
		// 50音摘要テーブルの抽出条件（tkrec）
		strFilter.Format("tkcod = %d", pZmSub->ezrec->eztcd);

		// 摘要名取得
		if(pZmSub->KanaTekiyoOpen(strFilter, 0) == 0)	{
			// 取得に成功すれば取得数分読込み
			if(pZmSub->tkrec->GetRecordCount() > 0) {
				// 登録番号取得
				strName = pZmSub->tkrec->invno;
			}
			// 摘要名称テーブル閉じる
			pZmSub->KanaTekiyoClose();
		}
		else{
			m_clsFunc.ZmSubErrMsg(pZmSub);
			return("");
		}
	}

	return(strName);
}
// インボイス登録番号追加対応_23/11/27 add <--

//********************************************************************************
//	ICS財務処理：当期月別情報テーブル(month_info)の情報から指定月の集計対象の有無を判断
//		IN		CDBZmSub*		DB財務クラス
//				int				集計対象の有無を判断する月のオフセット
//				RENTEMP_INFO*	月指定コントロールのオフセット
//		RET		BOOL			TRUE	…	集計対象
//								FALSE	…	集計対象外
//********************************************************************************
BOOL CfrmUc000Common::CmnICSGetSykMonth(CDBZmSub* pZmSub, int intOffset, RENTEMP_INFO* uRenInfo)
{
	CVolDateDB		icsVolDate;			// 期間モジュールクラス
	int				intKesSgn = 0;		// 決算修正サイン
	int				intKesSykCut = 0;	// 中間決算集計　除外サイン
	int				intEmdate = 0;		// db_vd_emdate戻り値
// midori 181101 add -->
	int				gengo=0;
// midori 181101 add <--
	CString			strQuery;		// クエリ文
	BOOL			blnRet = FALSE;	// 戻り値

	// 変数初期化
	strQuery.Empty();

	// 指定月の決算修正サインを取得
// midori 181101 del -->
//	intEmdate = icsVolDate.db_vd_emdate( ID_ICS_KI_OFFSET_TOUKI , intOffset , TRUE , &intKesSgn , pZmSub );
// midori 181101 del <--
// midori 181101 add -->
	intEmdate = icsVolDate.db_vd_emdateGen( ID_ICS_KI_OFFSET_TOUKI , intOffset , TRUE , &intKesSgn, &gengo, pZmSub );
// midori 181101 add <--

	// 2007.12.20：戻り値＝-2（入力不可月）は"期中発生残高"を集計する必要があるので集計対象とする
	if ( intEmdate == -1 ){
		// 設定エラー or 無効月(期末決算なのに第一四半期のオフセットを指定した場合等)
		return blnRet;
	}

	// 期間集計対応により、期間外は集計対象外とする
	if( uRenInfo->intStartOffset > intOffset || uRenInfo->intEndOffset < intOffset ){
		return blnRet;
	}

	// 決修月ならば集計対象処理実行
	if ( intKesSgn != ID_ICS_KESSIGN_NORMAL ){

		//**************** 除外サインの判定 ***************//
		if( uRenInfo->blnEndKes[intOffset] != FALSE ){
			// 集計期間終了オフセットの月が決算修正月だった場合は除外サインに関わらず必ず集計する
			return TRUE;
		}

		// 指定月のみのレコード取得クエリ文作成
		strQuery.Format( "ofset = %d" , intOffset );

		// 当期月別情報テーブルオープン
		if ( pZmSub->MinfoOpen( strQuery ) == 0 ){
			// レコード1件以上見つかった?
			if ( pZmSub->minfo->GetRecordCount() > 0 ){			
				// 除外サイン取得
				intKesSykCut = pZmSub->minfo->kes_syk_cut;
			}

			// 当期月別情報テーブルクローズ
			pZmSub->MinfoClose();

			// 除外サインがON(1)以外なら集計対象
			if ( intKesSykCut != ID_ICS_KESSYKCUT_ON ){
				blnRet = TRUE;
			}
		}
		else{
			// DBエラー対応
			m_clsFunc.ZmSubErrMsg( pZmSub );
			return blnRet;
		}
	}
	// 通常月なので集計対象
	else{
		// ※2007/04/19 ここに0～期末決算月の範囲内かどうかの
		// チェックを入れたい(範囲外ならFALSE)
		blnRet = TRUE;
	}

	return blnRet;
}

//********************************************************************************
//	ICS財務処理：当期月別情報テーブル(month_info)の情報から期末決算月オフセットと
//				 開始月～期末決算月の集計対象判断値を取得
//		IN		CDBZmSub*		DB財務クラス
//				RENTEMP_INFO*	集計対象の有無を判断する月のオフセット
//		RET		int				実行結果（FUNCTION_OK, FUNCTION_NG）
//********************************************************************************
int CfrmUc000Common::CmnICSGetMinfoData(CDBZmSub* pZmSub, RENTEMP_INFO* uRenInfo )
{
	CVolDateDB	icsVolDate;		// 期間モジュールクラス

	// 期末決算月のオフセットを取得
	//uRenInfo->intKimatsu = icsVolDate.db_vd_qmoffset( ID_ICS_KI_OFFSET_TOUKI , ID_ICS_KESSIGN_KIMATSU , pZmSub ); 
	int nMonthOffset = 0;
	// 集計期間終了オフセットが決算修正月かチェック
	for(int i=1; i<5; i++){	// 第一四半期～期末決算
		nMonthOffset = icsVolDate.db_vd_qmoffset( ID_ICS_KI_OFFSET_TOUKI , i , pZmSub );
		if( uRenInfo->intEndOffset == nMonthOffset ){
			uRenInfo->blnEndKes[uRenInfo->intEndOffset] = TRUE;
		}
	}

	// 開始月～終了集計判断
	for (int i = uRenInfo->intStartOffset; i <= uRenInfo->intEndOffset; i++) {
		uRenInfo->blnSykFlg[i] = CmnICSGetSykMonth( pZmSub , i , uRenInfo );
	}
	
	return FUNCTION_OK;
}

//********************************************************************************
//	ICS財務処理：当期月別情報テーブル(month_info)の情報から集計対象外を除外するクエリ文を作成
//		IN		CDBZmSub*		DB財務クラス
//				RENTEMP_INFO*	集計対象の有無を判断する月のオフセット
//		RET		CString			文字列	…	仕訳データから集計除外月を省くクエリ文
//								""		…	集計除外月無し
//********************************************************************************
CString CfrmUc000Common::CmnICSGetSykMonthQuery( CDBZmSub* pZmSub, RENTEMP_INFO* uRenInfo )
{
	CString		strFilter;			// 作成クエリ文
	CString		strFilterSub;		// サブ作成クエリ文
	int			intOffsetCnt = 0;	// 集計判断する月オフセット

	// 初期化
	strFilter.Empty();
	strFilterSub.Empty();

	// 0～期末決算月までループ
	for (intOffsetCnt = uRenInfo->intStartOffset; intOffsetCnt <= uRenInfo->intEndOffset; intOffsetCnt++) {
		// 集計対象外?
		if ( uRenInfo->blnSykFlg[intOffsetCnt] == FALSE ){
			// 最初なら"("を付加、途中なら"AND"を付加
			if ( strFilter != "" ){
				strFilter += _T(" AND ");
			}
			else{
				strFilter += _T("(");
			}
			// 抽出条件追加
			strFilterSub.Format("mofs <> %d" , intOffsetCnt);
			strFilter += strFilterSub;
		}
	}
	// 抽出条件が追加されていれば")"を付加
	if ( strFilter != "" ){
		strFilter += ")";
	}

	return strFilter;
}


//***************************************************************************
//（2009.09.08 TS自動ログオフ対応）
//	終了メッセージ処理
//		・当関数は、MainFrameから呼び出すこと。
//
//	【経緯】
//	・アプリ起動中にWindowsログオフをすると、アプリが正常に終了しない場合がある。
//	　これは、Viewクラスのデストラクタに処理が実装されていると発生する。
//	　よって、デストラクタの処理を別の場所に移動する必要がある。
//
//	【対処】
//	・アプリ終了時     ：WM_CLOSE メッセージハンドにて終了処理を行う
//	・Windowsログオフ時：WM_QUERYENDSESSION メッセージハンドにて終了処理を行う
//
//	【補足】
//	・Viewクラスの WM_CLOSE や WM_QUERYENDSESSION は、実装しても処理が実行されない。
//	　よって、MainFrameの WM_CLOSE や WM_QUERYENDSESSION が実行されたときに、
//	　View側の終了処理を明示的に呼び出す必要がある。
//
//	IN		UINT				メッセージ
//			WPARAM				パラメータ
//	RET		BOOL				TRUE=エラー，FALSE=正常
//***************************************************************************
BOOL CfrmUc000Common::EndApp(UINT message, WPARAM wParam)
{
	TRACE(_T("***** CfrmUc000Common::EndApp\n"));

	switch (message) {
	case WM_CLOSE:				// アプリ終了時
	case WM_QUERYENDSESSION:	// Windowsセッション終了時
		// Viewの終了処理
		virEndProc();
		break;
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
BOOL CfrmUc000Common::virEndProc(void)
{
	TRACE(_T("***** CfrmUc000Common::virEndProc\n"));

	// 処理なし→印刷ダイアログの破棄
	if(m_prtJob != NULL ){
		delete m_prtJob;
		m_prtJob = NULL;
	}

	return TRUE;
}

//********************************************************************************
//	「前の様式へ」、「次の様式へ」のボタンの監視
//		【引数】	mode	TRUE:無条件に両隣の様式のチェック/ FALSE:データがある様式
//		【戻値】	なし
//********************************************************************************
void CfrmUc000Common::UpdateYoushikiMoveButton(BOOL mode)
{
	BOOL			bBack			= TRUE;
	BOOL			bNext			= TRUE;
	int				nNowFormOrder	= 0;

	if( mode == FALSE ){
		if(m_uInfo.intFormSeq == ID_FORMNO_011){	// 様式が始端(様式番号：①のときは「前の様式へ」ボタンをグレーアウト
			bBack = FALSE;
		}
		if(m_uInfo.intFormSeq == ID_FORMNO_1820){	// 様式が終端(様式番号：⑱－２０)のときは「次の様式へ」ボタンをグレーアウト
			bNext = FALSE;
		}
	}
	else{
		if( m_clsFunc.IsExistData(m_pDB, m_uInfo.intFormSeq, TRUE, 1) == -1 ){		// 現在の様式より前の様式の中にデータ行がある様式が存在する？
		
			bBack = FALSE;
		}
		if( m_clsFunc.IsExistData(m_pDB, m_uInfo.intFormSeq, FALSE, 1) == -1 ){		// 現在の様式より後の様式の中にデータ行がある様式が存在する？
			bNext = FALSE;
		}
	}

	GetDlgItem( IDC_YOUSHIKIBACK_BUTTON1 )->EnableWindow( bBack );	// 前様式ボタン
	GetDlgItem( IDC_YOUSHIKINEXT_BUTTON1 )->EnableWindow( bNext );	// 次様式ボタン
}

CprtJobFunction* CfrmUc000Common::CreatePrintObject(int nSchList)
{
	return new CprtJobFunction();
}

//********************************************************************************
//	前/次の様式のシーケンス番号を取得する
//		【引数】	BOOL dest; TRUE:前へ/FALSE:次へ
//		【戻値】	short 移動先の様式のシーケンス番号
//********************************************************************************
int CfrmUc000Common::CmnGetDestinationYoushiki( BOOL dest )
{
	int nRet = 0;

	if( dest != FALSE ){	// 前の様式へ
		switch( m_uInfo.intFormSeq ){
			case ID_FORMNO_171:
				nRet = ID_FORMNO_162;
				break;
			case ID_FORMNO_181:
				nRet = ID_FORMNO_1720;
				break;
			default:
				nRet = m_uInfo.intFormSeq - 1;
				break;
		}
	}
	else{	// 次の様式へ
		switch( m_uInfo.intFormSeq ){
			case ID_FORMNO_162:
				nRet = ID_FORMNO_171;
				break;
			case ID_FORMNO_1720:
				nRet = ID_FORMNO_181;
				break;
			case ID_FORMNO_1820:
				nRet = ID_FORMNO_1820;
				break;
			default:
				nRet = m_uInfo.intFormSeq + 1;
				break;
		}
	}

	return nRet;
}

//********************************************************************************
//	０円計の表示フラグをゲットする
//		【引数】	CString データ行の金額
//					
//		【戻値】	BOOL* ０円計表示フラグ
//********************************************************************************
BOOL CfrmUc000Common::GetShowKeiZero(CString strTemp)
{
	BOOL nRet = TRUE;
	if(strTemp.IsEmpty() != FALSE)	nRet = FALSE;

	return nRet;
}

//********************************************************************************
//	出力形式、計ありなしに応じて使用するダイアログを取得する
//		【引数】	CICSDiag*	計1行のダイアログ
//					CICSDiag*	計2行のダイアログ
//					CICSDiag*	計0行（計なし様式）のダイアログ
//					
//		【戻値】	BOOL 
//********************************************************************************
CICSDiag* CfrmUc000Common::CmnGetDiagType(CICSDiag* diag1, CICSDiag* diag2, CICSDiag* diag0/*=NULL*/)
{
	CICSDiag*	retDiag			= diag1;
	int			intTotalType	= CmnGetControlTotalType();		// カレント頁の合計行の種別を取得

	if( m_uInfo.intRowKei == 1 ){
		// 計あり様式

		switch( m_uInfo.intOutKei ){
		case ID_OUTKEI_OFF:				// なし
		case ID_OUTKEI_PAGEKEI:			// 頁計
		case ID_OUTKEI_RUIKEI:			// 累計
			// 計が1行ある
			retDiag = diag1;
			diag1->ShowWindow( TRUE );
			diag2->ShowWindow( FALSE );
			break;
		case ID_OUTKEI_BOTH:			// 頁計と累計
			switch( intTotalType ){
			case ID_OUTKEI_OFF:			// 必要なし？
				// 計なし
				if( diag0 != NULL ){
					retDiag = diag0;
					diag0->ShowWindow( TRUE );
				}
				diag1->ShowWindow( FALSE );
				diag2->ShowWindow( FALSE );
				break;
			case ID_OUTKEI_PAGEKEI:
			case ID_OUTKEI_RUIKEI:
				// 計が1行ある
				retDiag = diag1;
				diag1->ShowWindow( TRUE );
				diag2->ShowWindow( FALSE );
				if( diag0 != NULL ){
					diag0->ShowWindow( FALSE );
				}
				break;
			case ID_OUTKEI_BOTH:
				// 計が2行ある
				retDiag = diag2;
				diag1->ShowWindow( FALSE );
				diag2->ShowWindow( TRUE );
				if( diag0 != NULL ){
					diag0->ShowWindow( FALSE );
				}
				break;
			}
			//// 計が2行ある
			//retDiag = diag2;
			//diag1->ShowWindow( FALSE );
			//diag2->ShowWindow( TRUE );
			//break;
		}
	}
	else{
		// 計なし様式
		switch( m_uInfo.intOutKei ){	// m_uInfo.intOutKeiは計設定
		case ID_OUTKEI_OFF:				// なし
			// 計なし
			if( diag0 != NULL ){
				retDiag = diag0;
				diag0->ShowWindow( TRUE );
			}
			diag1->ShowWindow( FALSE );
			diag2->ShowWindow( FALSE );
			break;
		case ID_OUTKEI_PAGEKEI:			// 頁計
			// 計が1行ある
			retDiag = diag1;
			diag1->ShowWindow( TRUE );
			diag2->ShowWindow( FALSE );
			if( diag0 != NULL ){
				diag0->ShowWindow( FALSE );
			}
			break;
		case ID_OUTKEI_RUIKEI:			// 累計
			if( intTotalType == ID_OUTKEI_OFF ){
				// 計なし様式 && 計設定「累計」 && !最終頁のときは計なしダイアグラム
				if( diag0 != NULL ){
					retDiag = diag0;
					diag0->ShowWindow( TRUE );
				}				
				diag1->ShowWindow( FALSE );
				diag2->ShowWindow( FALSE );				
			}
			else{
				// 計が1行ある
				retDiag = diag1;
				diag1->ShowWindow( TRUE );
				diag2->ShowWindow( FALSE );
				if( diag0 != NULL ){
					diag0->ShowWindow( FALSE );
				}
			}
			break;
		// 計設定が「頁計と累計」の場合でも、最終頁のみ計行が2行．つまり最終頁でないページは計を1行表示しなければならない
		case ID_OUTKEI_BOTH:			// 頁計と累計
			switch( intTotalType ){
			case ID_OUTKEI_OFF:			// 必要なし？
				// 計なし
				if( diag0 != NULL ){
					retDiag = diag0;
					diag0->ShowWindow( TRUE );
				}
				diag1->ShowWindow( FALSE );
				diag2->ShowWindow( FALSE );
				break;
			case ID_OUTKEI_PAGEKEI:
			case ID_OUTKEI_RUIKEI:
				// 計が1行ある
				retDiag = diag1;
				diag1->ShowWindow( TRUE );
				diag2->ShowWindow( FALSE );
				if( diag0 != NULL ){
					diag0->ShowWindow( FALSE );
				}
				break;
			case ID_OUTKEI_BOTH:
				// 計が2行ある
				retDiag = diag2;
				diag1->ShowWindow( FALSE );
				diag2->ShowWindow( TRUE );
				if( diag0 != NULL ){
					diag0->ShowWindow( FALSE );
				}
				break;
			}
		}
	}
	return retDiag;
}

//********************************************************************************
//	計ダイアログの位置を修正する
//		【引数】	CICSDiag*	明細ダイアログ
//					CICSDiag*	計ダイアログ
//					
//		【戻値】	BOOL 
//********************************************************************************
int CfrmUc000Common::SetKeiDialogPosition(CICSDiag* pDiagData, CICSDiag* pDiagKei)
{
	int		dist;
	RECT	rectData,rectKei;
	LPRECT	lprectData = &rectData, lprectKei = &rectKei;

	// ダイアログのRECT情報を取得
	pDiagData->GetWindowRect( lprectData );
	pDiagKei->GetWindowRect( lprectKei );

	// 計ダイアログのbottomの調整
	dist = lprectKei->top - lprectData->bottom;
	lprectKei->bottom -= dist;

	// 計ダイアログのtopに明細ダイアログのbottomを設定
	lprectKei->top = lprectData->bottom;
	pDiagKei->MoveWindow( lprectKei );
	
	return 0;
}

//********************************************************************************
//	指定されたカラムの計ダイアログのインデックスを取得する
//		【引数】	CICSDiag*	明細ダイアログ
//					CICSDiag*	計ダイアログ
//					
//		【戻値】	BOOL 
//********************************************************************************
int CfrmUc000Common::CmnGetKeiDiagIndex(int intTotalType, int nCol)
{
	int nRet = 0;
	
	if( intTotalType == 1 ){
		nRet = m_uInfo.intKei2StartIndex;
	}
	else{
		nRet = m_uInfo.intKei1StartIndex;
	}

	nRet = nRet + nCol;

	return nRet;
}

/*------------------------------------------------------------
	<引数>
		nFormSeq:	現在の様式シーケンス
		Seq		:	現在のレコードのシーケンス
		inPage	:	一時保管の対象頁
		inRow	:	一時保管の対象行
	<戻値>
		 0	:	成功
		-1	:	失敗
	<機能>
		パラメータで指定された頁/行のデータを保管テーブルに追加する
------------------------------------------------------------*/
int CfrmUc000Common::CmnTblAddToHokanOneLine(int nFormSeq, int inPage, int inRow)
{
	int					nOrderNum = 0;
	CString				strOrderNum;
	CdbUc000Common*		rsData;
	CdbUcLstHokan		rsHokan(m_pDB);
	CRecordset			rs(m_pDB);

	rsData = virGetDataTableObject();

	// 頁と行からレコード抽出
	rsData->RequeryPageRow(inPage, inRow);

	// 現在の様式で保管されているレコードのオーダーの最大値を取得する
	CString strSQL;
	strSQL.Format(_T("SELECT max(OrderNum) AS MaxNum FROM [uc_lst_hokan] WHERE FormSeq = %d"), nFormSeq);
	rs.Open( CRecordset::forwardOnly, strSQL);
	rs.GetFieldValue(_T("MaxNum"), strOrderNum);
	sscanf_s( strOrderNum, _T("%d"), &nOrderNum);
	rs.Close();

	// 指定レコードを保管テーブルに追加する
	while( !rsData->IsEOF() ){
		rsHokan.CreateNewRecord();

		rsHokan.Edit();
		rsHokan.m_FormSeq = nFormSeq;
		rsHokan.m_Seq = rsData->m_Seq;
		rsHokan.m_OrderNum = nOrderNum + 1;
		rsHokan.Update();

		// 次のレコードへ
		rsData->MoveNext();
	}

	rsHokan.Fin();
	rsData->Fin();
	delete rsData;

	return FUNCTION_OK;
}

//********************************************************************************
//	保管データが戻される際に保管テーブルから保管データの削除と
//　保管データのオーダー整理を行う
//
//		【引数】	CdbUc000Common*	　各様式テーブル
//					
//		【戻値】	BOOL 
//********************************************************************************
int CfrmUc000Common::DoProcAfterReturnHokanTable(CdbUc000Common* rsData)
{
	// 保管テーブルから指定レコードを削除
	CdbUcLstHokan mfcRec( m_pDB );
	while( !rsData->IsEOF() ){
		// 指定レコードを抽出
		mfcRec.RequerySeqFormSeq( m_uInfo.intFormSeq, rsData->m_Seq );

		if( !mfcRec.IsEOF() ){
			// 削除
			mfcRec.Delete();
		}

		// 次のレコードへ
		rsData->MoveNext();
	}

	// 様式レコードをトップに戻す
	rsData->MoveFirst();

	// オーダーの再構築
	int nOrderNum = 1;
	mfcRec.RequeryFormSeq( m_uInfo.intFormSeq );
	while( !mfcRec.IsEOF() ){
		mfcRec.Edit();
		mfcRec.m_OrderNum = nOrderNum;
		mfcRec.Update();

		// オーダーインクリメント
		nOrderNum++;

		// 次のレコードへ
		mfcRec.MoveNext();
	}

	mfcRec.Fin();

	return FUNCTION_OK;
}

//********************************************************************************
//	保管戻り状態(NumPage == NumRow == 0 FgShow = ID_FGSHOW_RET_HOKAN)の
//  レコードに、指定頁、行を付番する
//
//		【引数】	int			開始頁番号
//					int			開始行番号
//					int			頁を跨ぐ行数
//		【戻値】	BOOL 
//********************************************************************************
int CfrmUc000Common::SetPageRowNumToReturnDataByOrder(CdbUc000Common* rsData, int nPage, int nRow, int nBaseRow)
{
	CdbUcLstHokan mfcRec( m_pDB );
	mfcRec.RequeryFormSeq( m_uInfo.intFormSeq );

	while( !mfcRec.IsEOF() ){
		while( !rsData->IsEOF() ){
			if( mfcRec.m_Seq == rsData->m_Seq ){

				rsData->Edit();
				rsData->m_NumPage = nPage;
				rsData->m_NumRow = nRow++;
				rsData->m_FgShow = ID_FGSHOW_OFF;	// 表示
				rsData->m_NumGroup = 0;				// グループ番号
				rsData->Update();

				// 次の行が。最終行＋1であれば、次頁へ
				if( nRow >= nBaseRow + 1 ){
					nPage++;
					nRow = 1;
				}

				break;
			}

			// 様式レコードを次のレコードへ
			rsData->MoveNext();
		}
		// 保管レコードを次のレコードへ
		mfcRec.MoveNext();
		// 様式レコードをトップへ
		rsData->MoveFirst();
	}

	mfcRec.Fin();

	return FUNCTION_OK;
}

//********************************************************************************
//	既存の保管データを保管テーブルに登録するかチェックする
//
//		【引数】	なし
//
//		【戻値】	BOOL 
//********************************************************************************
BOOL CfrmUc000Common::IsRegExistHokanData()
{
	BOOL nRet	= FALSE;
	CdbUcInfSub	mfcRec( m_pDB );

	if( mfcRec.RequeryFormSeq( m_uInfo.intFormSeq ) != DB_ERR_OK ){
		return FALSE;
	}

	CCtrlSubGeneralVar cs( mfcRec );

	// フラグチェック
	if( !(cs.bAddHokan) ){
		nRet = TRUE;

		mfcRec.Edit();
		// [0]->D3ビットを落とさない
		cs.bAddHokan = TRUE;
		cs.Set( mfcRec );
		
		mfcRec.Update();
	}

	// テーブル終了
	mfcRec.Fin();

	return nRet;
}
//********************************************************************************
//	既存の保管データを保管テーブルにコピーする
//
//		【引数】	int			開始頁番号
//					int			開始行番号
//					int			頁を跨ぐ行数
//		【戻値】	BOOL 
//********************************************************************************
int CfrmUc000Common::CopyHokanDataToHokanTable(int nFormSeq)
{
	int nOrder = 1;
	CdbUc000Common* rsData;
	CdbUcLstHokan	mfcRec( m_pDB );
	
	// 様式テーブル取得
	rsData = virGetDataTableObject();

	// 保管レコードを抽出
// midori 180537 del -->
//	if( rsData->RequeryFgShow( ID_FGSHOW_HOKAN ) != DB_ERR_OK ){
// midori 180537 del <--
// midori 180537 add -->
	if( rsData->RequeryFgShow( ID_FGSHOW_HOKAN,1 ) != DB_ERR_OK ){
// midori 180537 add <--
		rsData->Fin();
		delete rsData;
		return FUNCTION_NG;
	}

	while( !rsData->IsEOF() ){
		// 保管レコード新規追加
		mfcRec.CreateNewRecord();

		mfcRec.Edit();
		mfcRec.m_FormSeq	= nFormSeq;
		mfcRec.m_Seq		= rsData->m_Seq;
		mfcRec.m_OrderNum	= nOrder;
		mfcRec.Update();

		// オーダーインクリメント
		nOrder++;

		// 次のレコードへ
		rsData->MoveNext();
	}

	rsData->Fin();
	delete rsData;

	return FUNCTION_OK;
}

//********************************************************************************
//	一度だけ金融機関、科目、取引先（得意先）、取引先（仕入先）にデフォルトの振り仮名を振る
//	※自動変換の為、必ずしも正確とは限らない
//
//		【引数】	int			開始頁番号
//					int			開始行番号
//					int			頁を跨ぐ行数
//		【戻値】	BOOL 
//******************************************************************************
BOOL CfrmUc000Common::CreateReferenceDlgKana()
{
	BOOL				nRet = FALSE;
	CdbUcInfMain		mfcRec( m_pDB );	//	uc_inf_mainテーブルクラス
	CFuriganaConverter  fc;		// 漢字からふりがなを導出するクラス

	//	初期化成功？
	if ( mfcRec.Init() == DB_ERR_OK ){
		//	レコードあり？
		if ( !( mfcRec.IsEOF() ) ){

			CCtrlMainGeneralVar cm(mfcRec);
			if( !(cm.bKanaDone) ){
//				CRevConv	StrConv;			// 文字列処理クラス
				CdbUcLstBank mfcBank(m_pDB);
				
				// 初期化
				if( mfcBank.Init() != DB_ERR_OK ){
					return nRet;
				}

				while( !(mfcBank.IsEOF()) ){					
					mfcBank.Edit();

					// 振り仮名取得
					mfcBank.m_BkKana1 = fc.Convert(mfcBank.m_BkName1);	// 金融機関名
					mfcBank.m_BkKana2 = fc.Convert(mfcBank.m_BkName2);	// 支店名

					mfcBank.Update();

					mfcBank.MoveNext();
				}
				mfcBank.Fin();

				CdbUcLstAddress mfcAddress(m_pDB);

				// 初期化
				if( mfcAddress.Init() != DB_ERR_OK ){
					return nRet;
				}

				while( !(mfcAddress.IsEOF()) ){					
					mfcAddress.Edit();

					// 振り仮名取得
					mfcAddress.m_AdKana = fc.Convert(mfcAddress.m_AdName1);	// 取引先名

					mfcAddress.Update();

					mfcAddress.MoveNext();
				}
				mfcAddress.Fin();

				CdbUcLstKamoku mfcKamoku(m_pDB);

				// 初期化
				if( mfcKamoku.Init() != DB_ERR_OK ){
					return nRet;
				}

				while( !(mfcKamoku.IsEOF()) ){
					// デフォルト科目はテーブル更新時に振り仮名生成済み
					if( mfcKamoku.m_FgDft ==  0 ){
						mfcKamoku.Edit();

						// 振り仮名取得
						mfcKamoku.m_KnKana = fc.Convert(mfcKamoku.m_KnName);	// 科目名

						mfcKamoku.Update();
					}

					mfcKamoku.MoveNext();
				}
				mfcKamoku.Fin();


				mfcRec.Edit();
				cm.bKanaDone = TRUE;
				cm.Set(mfcRec);
				mfcRec.Update();
				mfcRec.Fin();
			}
		}
	}

	return nRet;
}

//********************************************************************************
//	入力された文字列を半角カタカナにして振り仮名として返す。
//	振り仮名の文字列長は6バイト
//
//		【引数】	CString		振り仮名を取得する元となる文字列
//					CRevConv	文字変換クラス（大量のレコードを変更したとき、Runtimeエラーを出していたので
//								呼び出し元で一つだけインスタンスを作って使いまわす
//		【戻値】	CString		振り仮名。取得失敗の場合はNULLを返す
//******************************************************************************
//#define LENGTH_FURIGANA		6
//CString CfrmUc000Common::ConvertFurigana(CRevConv& StrConv, CString strBase)
//{
//	// 空なら何もせず終了
//	if( strBase.IsEmpty() ){
//		return "";
//	}
//
//	CString		strResult;
//	char chResult[6+1] = {};
//
//	// 文字列が改行コード、スペース（全角・半角）のみ、まはたNULLの場合
//	strBase.Replace( _T( "　" ), _T( "" ) );	// 全角スペースは、Trimで削除されない
//	strBase.Trim();							// 改行コード、半角半角スペースを削除
//
//	// 漢字→全角カタカナ変換
//	if( StrConv.GetFurigana(RVF_KATAKANA, strBase, strBase) == -1 ){
//		return "";
//	}
//
//	// 切り詰め
//	m_clsFunc.GetDividedName(strBase, strBase, LENGTH_FURIGANA);
//
//	// 全角カタカナ→半角カタカナ変換
//	LCMapString(LOCALE_SYSTEM_DEFAULT, LCMAP_HALFWIDTH, strBase, strBase.GetLength(), chResult, LENGTH_FURIGANA );
//
//	strResult = chResult;
//
//	return strResult;
//}

//********************************************************************************
//	指定されたインデックスのフォントサイズを変更する
//
//		【引数】	CString		振り仮名を取得する元となる文字列
//		【戻値】	CString		振り仮名。取得失敗の場合はNULLを返す
//******************************************************************************
short CfrmUc000Common::SetComboFontSize(CICSDiag* pDiag, int index, BOOL bFlg)
{
	int nFontSize = 0;

	// フォントサイズ
	if( bFlg != FALSE ){
		// 科目名が1つでも9文字を超えていたら、フォントサイズ変更
		nFontSize = 75;
	}
	else{
		nFontSize = 100;
	}
	// ? これを通さないとOnMessageCloseから呼び出しで落ちる→OnMessageCloseから呼ばないよう変更
	//pDiag->ComboxFontMode(TRUE);

	// フォントサイズ設定
	try{
		pDiag->ChangeFontSize( index, nFontSize );
	}
	catch(CException* e){
		e->Delete();
	}

	return 0;
}

// midori 190505 del -->
////********************************************************************************
////	印刷設定ダイアログで「科目行を出力する」にチェックが入っている場合、
////	【小計（○○）】の"（○○）"を削除する
////
////		【引数】	CdbUcInfSub*
////		【戻値】	int				0:変更なし
////									1:変更あり
////******************************************************************************
//// midori 151397 cor -->
////int CfrmUc000Common::DeleteKnNameBehindSyokei(const CdbUcInfSub* mfcRec)
////{
////	CdbUc000Common*	rsData = NULL;
////	int				nLParen	= 0;	// 左かっこの位置
////	int				nRParen	= 0;	// 右かっこの位置
////	int				nKnLen	= 0;	// （）を含めた科目名文字列長
////	int				nRet	= 0;
////	int				nOffset = 0;	// 小計のかっこが"（）"の場合のオフセット
////	int				nParenType = 2;	// デフォルトで全角
////	CString			strTmpSy;		// 小計名称
////
////	// DBオブジェクト取得
////	rsData = m_clsFunc.CreateDbObject(ID_FORMNO_111, m_pDB);
////	if( rsData == NULL )	return nRet;
////
////	// 小計行のみリクエリ
////	rsData->RequeryFgFunc(ID_FGFUNC_SYOKEI, 0);
////	
////	// 科目名削除
////	while( !(rsData->IsEOF()) ){
////		strTmpSy = rsData->m_KeiStr;
////		// 小計全体を囲むかっこが半角？全角？
////		if( mfcRec->m_OpSykeiKakko > 9 ){
////			// 半角
////			nParenType = 1;
////		}
////		// 小計全体を囲むかっこが（）の場合はオフセットをかませて無視する
////		else if( mfcRec->m_OpSykeiKakko == 4 ){
////			// "（"削除
////			strTmpSy.Delete( 0, 2 );
////
////			nOffset = 2;
////		}
////		else if( mfcRec->m_OpSykeiKakko == 0 ){
////			nParenType = 0;
////		}
////		
////
////		// "（"の位置取得
////		nLParen = strTmpSy.Find(_T("（"));
////		if( nLParen != -1 ){
////			// "）"の位置取得
////			nRParen = strTmpSy.GetLength() - nParenType;
////			nKnLen = nRParen - nLParen;
////
////			if( nKnLen < 0 ){
////				break;
////			}
////
////			// 変更あり
////			nRet = 1;
////
////			rsData->Edit();
////			// "（"から"）"まで削除
////			rsData->m_KeiStr.Delete( nLParen + nOffset, nKnLen );
////			rsData->Update();
////		}
////		rsData->MoveNext();
////	}
////
////	rsData->Fin();
////	delete rsData;
////
////	return nRet;
////}
//// ---------------------
//int CfrmUc000Common::DeleteKnNameBehindSyokei(const CdbUcInfSub* mfcRec)
//{
//	CdbUc000Common*	rsData = NULL;
//	int				nLParen	= 0;	// 左かっこの位置
//	int				nRParen	= 0;	// 右かっこの位置
//	int				nKnLen	= 0;	// （）を含めた科目名文字列長
//	int				nRet	= 0;
//	int				nOffset = 0;	// 小計のかっこが"（）"の場合のオフセット
//	int				nParenType = 2;	// デフォルトで全角
//	int				pos=0;
//	CString			strTmpSy;		// 小計名称
//
//	// DBオブジェクト取得
//	rsData = m_clsFunc.CreateDbObject(ID_FORMNO_111, m_pDB);
//	if( rsData == NULL )	return nRet;
//
//	// 小計行のみリクエリ
//	rsData->RequeryFgFunc(ID_FGFUNC_SYOKEI, 0);
//	
//	// 科目名削除
//	//  → 【小計（○○）】の"（○○）"を削除する
//	while( !(rsData->IsEOF()) ){
//		strTmpSy = rsData->m_KeiStr;
//		pos=0;
//		// １文字目が "（" の場合、削除する
//		if(strTmpSy.Left(2) == _T("（"))	{
//			strTmpSy.Delete(0,2);
//			strTmpSy.Delete(strTmpSy.GetLength() - 2,2);
//			pos=2;
//		}
//		// "（" と "）" の位置を検索する
//		nLParen = strTmpSy.Find(_T("（"));
//		nRParen = strTmpSy.Find(_T("）"));
//		if(nLParen != -1 && nRParen != -1)	{
//			// 変更あり
//			nRet = 1;
//			rsData->Edit();
//			// "（" から "）" まで削除
//			rsData->m_KeiStr.Delete(nLParen + pos,nRParen - nLParen + 2);
//			rsData->Update();
//		}
//		rsData->MoveNext();
//	}
//
//	rsData->Fin();
//	delete rsData;
//
//	return nRet;
//}
//// midori 151397 cor <--
// midori 190505 del <--
// midori 190505 add -->
// -------------------------------------------------------------------------------------------------------
// 小計行に科目名称を追加する
// int				様式番号
// int				挿入した科目行の頁番号
// int				挿入した科目行の行番号
// int				0:小計行に科目名を追加
//					1:小計行から科目名を削除
// -------------------------------------------------------------------------------------------------------
void CfrmUc000Common::SetKnNameSyokei(int nFormSeq,int ppage,int prow,int pSw)
{
// midori 157118 add -->
	int					sw = 0;
// midori 157118 add <--
// 157154 add -->
	int					nKmkRowSgn = 0;
// 157154 add <--
	SORTKAMOKU_INFO		uKamoku;
	ITEMSORT_INFO		uItemSort;			// 第１ソート項目のソート情報
	CString				strfilter = _T("");
	CString				strsort = _T("");
	CString				strKei = _T("");
// midori 157118 add -->
	CString				strdef = _T("");
	CString				strbuf = _T("");
	CString				strbuf2 = _T("");
	CString				strbuf3 = _T("");
// midori 157118 add <--
	CdbUcInfSub			mfcRec( m_pDB );	//	uc_inf_subテーブルクラス
	CdbUc000Common*		rsData = NULL;

// 改良No.21-0086,21-0529 cor -->
	//// 様式が②受取手形の内訳書、⑪借入金及び支払利子の内訳書でなければ処理を行わない
	//if(nFormSeq != ID_FORMNO_021 && nFormSeq != ID_FORMNO_111) {
// ------------------------------
	// 様式が②受取手形の内訳書、登録番号追加版の⑧支払手形の内訳書、⑪借入金及び支払利子の内訳書でなければ処理を行わない
	if( !(nFormSeq == ID_FORMNO_021 || (bG_InvNo == TRUE && nFormSeq == ID_FORMNO_081) || nFormSeq != ID_FORMNO_111))	{
// 改良No.21-0086,21-0529 cor <--
		return;
	}
	// CdbUcInfSub
	//	初期化成功？
	if ( mfcRec.RequeryFormSeq( nFormSeq ) != DB_ERR_OK ){
		return;
	}
	//	レコードあり？
	if ( mfcRec.IsEOF() ){
		mfcRec.Fin();
		return;
	}
// midori 157118 add -->
	// 小計文言
	strdef.Format(_T("%s%s%s"), BRACKET_KIND[mfcRec.m_OpSykeiKakko].szLeft,
								mfcRec.m_OpSykeiName,
								BRACKET_KIND[mfcRec.m_OpSykeiKakko].szRight);
// midori 157118 add <--
	// uc_lst_item_sortを参照して必要情報を取得
	CmnUcLstItemSortGetData(mfcRec.m_Sort1, &uItemSort);
	// 第１ソート項目が科目以外の場合、処理を行わない
	if(mfcRec.m_Sort1 > 0 && uItemSort.bytFgItem != ID_FGITEM_KAMOKU)	{
		mfcRec.Fin();
		return;
	}
	mfcRec.Fin();

// 157154 add -->
	//nKmkRowSgn = KamokuRowEnableSgn(m_pDB,0);				// 改良No.21-0086,21-0529 del
	nKmkRowSgn = KamokuRowEnableSgn(m_pDB, 0, nFormSeq);	// 改良No.21-0086,21-0529 add
// 157154 add <--

	rsData = virGetDataTableObject();

	// 頁、行順にデータを取得
	strfilter.Format(_T("(NumPage = %d and NumRow >= %d) or (NumPage > %d)"),ppage,prow,ppage);
	strsort = _T("NumPage ASC, NumRow ASC");
	rsData->RequerySortParameter(strfilter,strsort);
	// 科目データ構造体クリア
	uKamoku.intKnSeq = 0;
	uKamoku.intKnOrder = 0;
	uKamoku.strKnName.Empty();
	uKamoku.strKnKana.Empty();
	while (!rsData->IsEOF()) {
// midori 157118 del -->
//		// 小計に科目名を追加
//		if(pSw == 0){
//			// 明細行、一括金額（手動）、一括金額（自動）行の場合科目名称を取得
//			if(rsData->m_FgFunc == ID_FGFUNC_DATA || rsData->m_FgFunc == ID_FGFUNC_IKKATUMANUAL || rsData->m_FgFunc == ID_FGFUNC_IKKATUAUTO ) {
//				// 科目データの取得
//				virTblGetKamokuInfo(rsData, &uKamoku);
//			}
//		}
//		// 小計
//		if(rsData->m_FgFunc == 10) {
//// midori 156189,156190,156191 add -->
//			if(!(rsData->m_ShowKeiZero & BIT_D7)) {
//// midori 156189,156190,156191 add <--
//				if(pSw == 0) {
//					// 小計に科目名を追加して取得する
//					GetKamokuKeiStr(rsData->m_KeiStr,uKamoku.strKnName,&strKei);
//				}
//				else {
//					//小計から科目名を削除して取得する
//					DeleteKamokuKeiStr(rsData->m_KeiStr,&strKei);
//				}
//				// 科目名称を更新
//				rsData->Edit();
//				rsData->m_KeiStr = strKei;
//				rsData->Update();
//// midori 156189,156190,156191 add -->
//			}
//// midori 156189,156190,156191 add <--
// midori 157118 del <--
// midori 157118 add -->
		// 明細行、一括金額（手動）、一括金額（自動）行の場合科目名称を取得
		if(rsData->m_FgFunc == ID_FGFUNC_DATA || rsData->m_FgFunc == ID_FGFUNC_IKKATUMANUAL || rsData->m_FgFunc == ID_FGFUNC_IKKATUAUTO ) {
			// 科目データの取得
			virTblGetKamokuInfo(rsData, &uKamoku);
		}
		// 小計
		if(rsData->m_FgFunc == 10) {
			sw = 0;
			strKei.Empty();
			if (pSw == 0) {
				if (rsData->m_KeiStr == strdef) {
					// デフォルトの小計に科目名を追加して取得する
					GetKamokuKeiStr(strdef, uKamoku.strKnName, &strbuf);
					sw = 1;
					strKei = strbuf;
				}
			}
			else {
				// デフォルトの小計に科目名を追加して取得する
				GetKamokuKeiStr(strdef, uKamoku.strKnName, &strbuf);
				// 取得したデフォルトの小計名称を分割する
				m_clsFunc.KeiStrSprit(strbuf, &strbuf2, &strbuf3);
				strbuf = strbuf2 + strbuf3;
// 157154 del -->
				//if ((rsData->m_ShowKeiZero & BIT_D7)) {
// 157154 del <--
// 157154 add -->
				if(nKmkRowSgn == 1) {
// 157154 add <--
					// 改行を除いた文字列を取得
					strbuf2 = DelCrLfString(rsData->m_KeiStr);
				}
				else {
					// 小計名称を分割する
					m_clsFunc.KeiStrSprit(rsData->m_KeiStr, &strbuf2, &strbuf3);
					strbuf2 = strbuf2 + strbuf3;
				}

				if (strbuf == strbuf2) {
					sw = 1;
					strKei = strdef;
				}
			}

			if (sw > 0) {
				// 科目名称を更新
				rsData->Edit();
				rsData->m_KeiStr = strKei;
// 157154 del -->
				//rsData->m_ShowKeiZero &= ~BIT_D7;
// 157154 del <--
				rsData->Update();
			}
// midori 157118 add <--
			break;
		}
		rsData->MoveNext();
	}

	rsData->Fin();
	delete rsData;
}

// -------------------------------------------------------------------------------------------------------
// 小計名称から科目名称を削除する
// CString szSrc			小計名称
// CString* szDst			科目名を削除した小計名称（返送値）
// -------------------------------------------------------------------------------------------------------
void CfrmUc000Common::DeleteKamokuKeiStr(CString szSrc, CString* szDst)
{
	int				pos = 0;
	int				nLParen=0;
	int				nRParen=0;
	CString			strTmpSy;		// 小計名称

	szDst->Empty();

	// 小計名称が未入力の場合処理を行わない
	if(szSrc.IsEmpty() == TRUE)	{
		return;
	}

	*szDst = szSrc;
	strTmpSy = szSrc;
	pos=0;
	// １文字目が "(" の場合、"(" と ")" を削除する
	if(strTmpSy.Left(2) == _T("("))	{
		strTmpSy.Delete(0,1);
		strTmpSy.Delete(strTmpSy.GetLength() - 1,1);
		pos=1;
	}

	// "(" と ")" の位置を検索する
	nLParen = strTmpSy.Find(_T("("));
	nRParen = strTmpSy.Find(_T(")"));
	if(nLParen != -1 && nRParen != -1)	{
		// "(" から ")" まで削除
		szDst->Delete(nLParen + pos,nRParen - nLParen + 1);
	}
}

// -------------------------------------------------------------------------------------------------------
// 引数で渡された小計名に科目名称を追加する
// -------------------------------------------------------------------------------------------------------
void CfrmUc000Common::GetKamokuKeiStr(CString szSrc,CString sKamoku, CString* szDst)
{
	int				tp = 0;
	int				ii = 0;
	int				nbf = 0;
	int				nbf2 = 0;
	int				nLParen=0;
	int				nRParen=0;
	char			buf[128]={0};
	CString			sykakkoname = _T("");
	CString			strTmpSy;		// 小計名称

	szDst->Empty();

	// 小計名称が未入力の場合処理を行わない
	if(szSrc.IsEmpty() == TRUE)	{
		return;
	}

	*szDst = szSrc;
	// 科目名称が未入力の場合処理を行わない
	if(sKamoku.IsEmpty() == TRUE)	{
		return;
	}

	// 既に科目名称が入力されている場合、科目名称の追加を行わない
	strTmpSy = szSrc;
	// １文字目が "(" の場合、"(" と ")" を削除する
	if(strTmpSy.Left(2) == _T("("))	{
		strTmpSy.Delete(0,1);
		strTmpSy.Delete(strTmpSy.GetLength() - 1,1);
	}

	// "(" と ")" の位置を検索する
	nLParen = strTmpSy.Find(_T("("));
	nRParen = strTmpSy.Find(_T(")"));
	// 存在する場合、科目名称ありとして処理を行わない
	if(nLParen != -1 && nRParen != -1)	{
		return;
	}

	// 小計に科目名称を追加する
	memset(buf,0,128);
	strcpy_s(buf,128,szSrc);
	// 右の括弧を取得
	tp = 1;
	if (_mbsbtype((LPBYTE)buf, (szSrc.GetLength() - 1)) == _MBC_TRAIL) {
		tp++;
	}
	sykakkoname = szSrc.Right(tp);

	nbf = szSrc.GetLength();
	nbf2 = 0;
	for (ii = 0; ii < ID_BRACKET_MAX; ii++) {
		// 右括弧を比較
		if (sykakkoname.Compare(BRACKET_KIND[ii].szRight) == 0) {
			nbf2 = sykakkoname.GetLength();
		}
		if (nbf2 > 0) break;
	}

	// 小計の括弧あり
	if(nbf2 > 0) {
		// 小計名をセット
		strTmpSy = szSrc;
		// 小計名称の後ろの位置を取得
		nbf = nbf - nbf2;
		// 小計名称後ろの括弧をカット
		strTmpSy.Delete(nbf,nbf2);
		// 小計名称に科目名を追加
		*szDst = strTmpSy + _T("(") + sKamoku + _T(")") + sykakkoname;
	}
	return;
}
// midori 190505 add <--

//********************************************************************************
//	様式終了（会社切替）フラグのセッタ
//
//		【引数】	BOOL		セットする値
//		【戻値】	なし
//********************************************************************************
void CfrmUc000Common::SetEndViewFlag(BOOL state)
{
	m_EndView = state;
}

//********************************************************************************
//	様式終了（会社切替）フラグのセッタ
//
//		【引数】	BOOL		セットする値
//		【戻値】	なし
//********************************************************************************
void CfrmUc000Common::GetCommonInfo(CCtrlMainGeneralVar& cm)
{
	m_DispIkkatDlg = cm.bDispIkkatDlg;
	m_YoushikiSkip = cm.bYoushikiSkip;
	m_DispYoushikiSelect = cm.bDispYoushikiSelect;
	m_ReturnMoveDown = cm.bReturnMoveDown;
	m_RegAutoRef = cm.bRegAutoRef;
// midori 160612 add -->
	m_DataKakutei = cm.bDataKakutei;
// midori 160612 add <--
// midori 152135,152136 add -->
	if(cm.bBankSort)	m_BankSort = 1;
	else				m_BankSort = 0;
// midori 152745 del -->
//	if(cm.bAddressSort)	m_AddressSort = 1;
//	else				m_AddressSort = 0;
// midori 152745 del <--
// midori 152745 add -->
	if(cm.bAddressSort)		m_AddressSort[0] = 1;
	else					m_AddressSort[0] = 0;
	if(cm.bAddressSort2)	m_AddressSort[1] = 1;
	else					m_AddressSort[1] = 0;
// midori 152745 add <--
// midori 152135,152136 add <--
}

//********************************************************************************
//	過去に失敗してowntblに持っていたデータをこの機にuc_inf_mainとuc_inf_subに移す
//  
//
//		【引数】	BOOL		セットする値
//		【戻値】	なし
//********************************************************************************
void CfrmUc000Common::CopyOwntblData()
{
	// 一度コピーしているなら何もしない
	CdbUcInfMain mfcMain(m_pDB);
	if (mfcMain.Init() != DB_ERR_OK) {
		mfcMain.Fin();
		return;
	}

	CCtrlMainGeneralVar cm(mfcMain);
	if(cm.bCopyOwnData){
		mfcMain.Fin();
		return;
	}

	CDBZmSub *pZmSub = ((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub;
	CCopyOwnData copy_own(pZmSub);
	copy_own.Copy();
}

//********************************************************************************
//	共通だった「金額０円データ及び空欄データを表示しない」を各様式別に持たせる為、
//	テーブル更新後の初起動時のみコピーする
//
//		【引数】	BOOL		セットする値
//		【戻値】	なし
//********************************************************************************
BOOL CfrmUc000Common::UpdateVer1_0_2_1(CString& strErr)
{
	// 一度コピーしているなら何もしない
	CdbUcInfMain mfcMain(m_pDB);
	if (mfcMain.Init() != DB_ERR_OK) {
		mfcMain.Fin();
		return FALSE;
	}

	CCtrlMainGeneralVar cm(mfcMain);
	if(cm.bDoneUpdate1_0_2_1){
		mfcMain.Fin();
		return TRUE;
	}

	int st = CopyCommonPrintZero(mfcMain);
	if(st != DB_ERR_OK){
		strErr = "印刷設定のコピー中にエラーが発生しました。\n処理を終了します。";
		return FALSE;
	}

	st = UpdateReferenceData();
	if(st != DB_ERR_OK){
		strErr = "参照データ更新中にエラーが発生しました。\n処理を終了します。";
		return FALSE;
	}

	cm.bDoneUpdate1_0_2_1 = TRUE;
	mfcMain.Edit();
	cm.Set(mfcMain);
	mfcMain.Update();
	mfcMain.Fin();
	return TRUE;
}

//********************************************************************************
//	印刷設定コピーの実働関数
//
//		【引数】	BOOL		セットする値
//		【戻値】	なし
//********************************************************************************
int CfrmUc000Common::CopyCommonPrintZero(const CdbUcInfMain& rc)
{
	CString	sql;
	BOOL	isPrintZero	= rc.m_OutKingaku & BIT_D1;
	BOOL	isHideZero	= rc.m_OutKingaku & BIT_D2;	// 注意！このフラグはチェックがONの時に0,OFFの時!0になる！

	CdbUcInfSub mfcSub(m_pDB);
	
	for(int i=0; i<2; i++){
		switch(i){
			case 0:	// 金額０円データ及び空欄データを出力しない
				if(!isPrintZero) continue;
				sql = GetSQLCopyCommonPrintZero(BIT_D1);
				break;
			case 1:	// 金額０円を印字しない
				if(!isHideZero) continue;
				sql = GetSQLCopyCommonPrintZero(BIT_D2);
				break;
		}

		TRY{
			if(m_pDB != NULL)	m_pDB->ExecuteSQL(sql);
		}
		CATCH(CDBException, e){
			mfcSub.Fin();
			int ret = e->m_nRetCode;
			return ret;
		}
		END_CATCH
	}
	
	mfcSub.Fin();
	return DB_ERR_OK;
}

CString CfrmUc000Common::GetSQLCopyCommonPrintZero(int bit)
{
	CString sql;
	if(bit == BIT_D1){
		sql.Format("UPDATE [dbo].[uc_inf_sub] SET GeneralVar = [dbo].[set_byte2](GeneralVar, 0, %d, 1) ", BIT_D7);
		sql.Format(sql + "UPDATE [dbo].[uc_inf_sub] SET GeneralVar = [dbo].[set_byte2](GeneralVar, 1, %d, 1) ", BIT_D0);
	}
	else if(bit == BIT_D2)	sql.Format("UPDATE [dbo].[uc_inf_sub] SET GeneralVar = [dbo].[set_byte2](GeneralVar, 1, %d, 1) ", BIT_D1);

	return sql;
}

int CfrmUc000Common::UpdateReferenceData()
{
	for(int i=0; i<4; i++){
		switch(i){
		case 0:
			UpdateAdDataFromAddress(STR_TBL_NAME_042);
			break;
		case 1:
			UpdateAdDataFromBank(STR_TBL_NAME_111);
			break;
		case 2:
			UpdateAdDataFromAddress(STR_TBL_NAME_161);
			break;
		case 3:
			UpdateAdDataFromAddress(STR_TBL_NAME_162);
			break;
		}
	}
	return DB_ERR_OK;
}

//********************************************************************************
//	取引先テーブルからAdSeq,AdOrderを更新する
//		【引数】	なし
//					
//		【戻値】	CString 更新先テーブル
//********************************************************************************
int CfrmUc000Common::UpdateAdDataFromAddress(CString table_name)
{
	CString sql;
	// 一時テーブルが既にあれば削除
	sql += "IF OBJECT_ID('tempdb..#Ad_data_update_tbl', 'u') IS NOT NULL DROP TABLE #Ad_data_update_tbl ";
	// 一時テーブルに更新先テーブルをコピー
	sql += "SELECT * INTO #Ad_data_update_tbl FROM " + table_name + " ";
	// 未入力データが取引先テーブルは空欄で、更新先テーブルはNULLとなっているので、一時テーブルのNULLを空欄に置き換え
	sql += "UPDATE #Ad_data_update_tbl SET AdName1 = '' WHERE AdName1 IS NULL AND FgFunc = 1 ";
	sql += "UPDATE #Ad_data_update_tbl SET AdName2 = '' WHERE AdName2 IS NULL AND FgFunc = 1 ";
	sql += "UPDATE #Ad_data_update_tbl SET AdAdd1 = '' WHERE AdAdd1 IS NULL AND FgFunc = 1 ";
	sql += "UPDATE #Ad_data_update_tbl SET AdAdd2 = '' WHERE AdAdd2 IS NULL AND FgFunc = 1 ";
	// 取引先の名称・所在地が一致するレコードに対してAdSeq,AdOrderを更新
	sql += "UPDATE #Ad_data_update_tbl SET #Ad_data_update_tbl.AdSeq = uc_lst_address.AdSeq, #Ad_data_update_tbl.AdOrder = uc_lst_address.OrderNum ";
	sql += "FROM uc_lst_address ";
	sql += "WHERE #Ad_data_update_tbl.FgFunc = 1 AND #Ad_data_update_tbl.AdName1 = uc_lst_address.AdName1 AND #Ad_data_update_tbl.AdName2 = uc_lst_address.Adname2 AND #Ad_data_update_tbl.AdAdd1 = uc_lst_address.AdAdd1 AND #Ad_data_update_tbl.AdAdd2 = uc_lst_address.AdAdd2 ";
	// 一時テーブルの値で実際のテーブルを更新
	sql += "UPDATE " + table_name + " SET " + table_name + ".AdSeq = #Ad_data_update_tbl.AdSeq, " + table_name + ".AdOrder = #Ad_data_update_tbl.AdOrder ";
	sql += "FROM #Ad_data_update_tbl WHERE " + table_name + ".Seq = #Ad_data_update_tbl.Seq ";

	try{
		m_pDB->ExecuteSQL(sql);
	}
	catch(CDBException* e ){
		int ret = e->m_nRetCode;
		return ret;
	}

	return DB_ERR_OK;
}

//********************************************************************************
//	銀行テーブルからAdSeq,AdOrderを更新する
//		【引数】	CString 更新先テーブル
//					
//		【戻値】	CString 更新先テーブル
//********************************************************************************
int CfrmUc000Common::UpdateAdDataFromBank(CString table_name)
{
	CString sql;
	// 一時テーブルが既にあれば削除
	sql += "IF OBJECT_ID('tempdb..#Ad_data_update_tbl', 'u') IS NOT NULL DROP TABLE #Ad_data_update_tbl ";
	// 一時テーブルに更新先テーブルをコピー
	sql += "SELECT * INTO #Ad_data_update_tbl FROM " + table_name + " ";
	// 未入力データが取引先テーブルは空欄で、更新先テーブルはNULLとなっているので、一時テーブルのNULLを空欄に置き換え
	sql += "UPDATE #Ad_data_update_tbl SET AdName1 = '' WHERE AdName1 IS NULL AND FgFunc = 1 ";
	sql += "UPDATE #Ad_data_update_tbl SET AdName2 = '' WHERE AdName2 IS NULL AND FgFunc = 1 ";
	// 銀行名・支店名が一致するレコードに対してAdSeq,AdOrderを更新
	sql += "UPDATE #Ad_data_update_tbl ";
	sql += "SET #Ad_data_update_tbl.AdSeq = [dbo].[uc_lst_bank].Seq, #Ad_data_update_tbl.AdOrder = [dbo].[uc_lst_bank].OrderNum ";
	sql += "FROM [dbo].[uc_lst_bank] ";
	sql += "WHERE #Ad_data_update_tbl.FgFunc = 1 and #Ad_data_update_tbl.AdName1 = [dbo].[uc_lst_bank].BkName1 and #Ad_data_update_tbl.AdName2 = [dbo].[uc_lst_bank].BkName2 ";
	// 一時テーブルの値で実際のテーブルを更新
	sql += "UPDATE " + table_name + " SET " + table_name + ".AdSeq = #Ad_data_update_tbl.AdSeq, " + table_name + ".AdOrder = #Ad_data_update_tbl.AdOrder ";
	sql += "FROM #Ad_data_update_tbl WHERE " + table_name + ".Seq = #Ad_data_update_tbl.Seq ";

	try{
		// SQL実行
		m_pDB->ExecuteSQL(sql);
	}
	catch(CDBException* e ){
		int ret = e->m_nRetCode;
		return ret;
	}

	return DB_ERR_OK;
}

//********************************************************************************
//	様式選択ダイアログ実装部
//		【引数】	int		0:Insertキーからコール　1:起動時にコール
//					
//		【戻値】	なし
//********************************************************************************
LRESULT CfrmUc000Common::DoButtonInsert( WPARAM wp, LPARAM lp )
{
	CdlgYoushikiSelect		dlgSel( this );		//	様式選択ダイアログ
	CdbUcInfMain			mfcRec( m_pDB );	//	uc_inf_mainテーブル
	CdbUcInfSub				mfcRecSub( m_pDB );	//	uc_inf_mainテーブル
	int						nNowFormSeq		= 0;
	int						nNowFormOrder	= 0;

	// 実行前の準備処理
	if(wp == FROM_INSERT_KEY)	CmnEventStartProc(ID_BUTTON_INSERT);

	try{
		//	初期化成功？
		if ( mfcRec.Init() == DB_ERR_OK ){
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				mfcRec.MoveFirst();				//	レコードを先頭に移動
				nNowFormSeq = mfcRec.m_FormSeq;	//	現在選択されている様式のシーケンス番号取得
			}
			//	レコード閉じる
			mfcRec.Fin();
		}

		// FormOrder番号を取得
		if ( mfcRecSub.RequeryFormSeq( nNowFormSeq ) == DB_ERR_OK ) {
			//	レコードあり？
			if ( !( mfcRecSub.IsEOF() ) ){
				mfcRecSub.MoveFirst();					//	レコードを先頭に移動			
				nNowFormOrder = mfcRecSub.m_FormOrder;	//	現在選択されている様式のオーダー番号取得
			}
			//	レコード閉じる
			mfcRecSub.Fin();
		}

	}
	catch(...){
		//	レコード開いてる？
		if ( mfcRec.IsOpen() ){
			//	レコード閉じる
			mfcRec.Fin();
		}
	}

// 修正No.157844 add -->
	int		colmsv = -1, colmset = -1;
	int		rowsv = -1, rowset = -1;
	int		sign = 0;

// 修正No.158301 del -->
//	if(m_uInfo.intFormSeq == ID_FORMNO_102)	{
//// 修正No.157883,157884 del -->
//		//if(m_uInfo.intCurCol == ID_COL_102_SPDATE )	{
//// 修正No.157883,157884 del <--
//// 修正No.157883,157884 add -->
//		if(m_uInfo.intCurCol == ID_COL_102_SPDATE || m_uInfo.intCurCol == ID_COL_102_VAL)	{
//// 修正No.157883,157884 add <--
//			rowset = 1;
//			colmset = ID_COL_102_KNORDER;
//			sign = 1;
//		}
//	}
//	if(sign == 1)	{
//		colmsv = m_uInfo.intCurCol;
//		rowsv = m_uInfo.intCurRow;
//		CmnDiagSetFocus(m_pDiag, rowset, colmset);
//	}
// 修正No.158301 del <--
// 修正No.158301 add -->
	sign = CmnTblDataToFirstCol(&colmset);
	if(sign == 1)	{
		colmsv = m_uInfo.intCurCol;
		rowsv = m_uInfo.intCurRow;
		CmnDiagSetFocus(m_pDiag, rowsv, colmset);
	}
// 修正No.158301 add <--
// 修正No.157844 add <--

	//	様式が変更された？
	if ( dlgSel.ShowDialog( m_YoushikiSkip, m_pDB ) == ID_DLG_OK ){

		// カレント情報などを保存
		CmnUcInfSubSetCurrentInfo();

		// 空行データの削除処理
		if ( CmnTblCheckDeleteData() == TRUE ){
			CmnTblEndDeleteData();
		}

		// 表示する様式の切替を行う
		((CUCHIWAKEApp*)AfxGetApp())->ChangeDocument( dlgSel.m_FormSeq );

// 要通知案件修正 21/07/06 del -->
//// 修正No.157633_元号日付 add -->
//		if(bG_Kanso == TRUE)	{
//				m_CursorSetFlag = TRUE;
//		}
//// 修正No.157633_元号日付 add <--
// 要通知案件修正 21/07/06 del <--

		return 1;	//	←このリターン重要
	}
	//	キャンセルが押された or 同じ様式が選択された
	else{
		// 位置情報保存
		CmnUcInfSubSetCurrentInfo();
		// カーソル移動
		// OnActivateViewイベントでフォーカス再設定するようにした
		//CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);

// 修正No.157844 add -->
		if(colmsv != -1 || rowsv != -1)	{
			CmnDiagSetFocus(m_pDiag, rowsv, colmsv);
		}
// 修正No.157844 add <--
	}

	return 1;
}

// 未使用
//BOOL CfrmUc000Common::IsSaveMoveReference(int col_to)
//{
//	if(m_uInfo.OldRow != m_uInfo.intCurRow)	return TRUE;	// 行が異なれば必ず保存
//	if(m_isF6 != FALSE)						return FALSE;	// F6項目複写された場合、EditOnが走り、col_toのカラムが未設定のまま保存されてしまうのを防ぐ
//
//	return !m_uInfo.OldCol == col_to;
//}

BOOL CfrmUc000Common::IsSaveMoveReference(int col_from, int col_to)
{
	if(m_uInfo.OldRow != m_uInfo.intCurRow)	return TRUE;	// 行が異なれば必ず保存
	if(m_isF6 != FALSE)						return FALSE;	// F6項目複写された場合、EditOnが走り、col_toのカラムが未設定のまま保存されてしまうのを防ぐ

	return !(((m_uInfo.OldCol == col_from) && (m_uInfo.intCurCol == col_to)) || ((m_uInfo.OldCol == col_to) && (m_uInfo.intCurCol == col_from)));
}

//*******************************************************************************
//	ICS-Controlの対象コントロールの文字列の末尾にカーソル設定
//		IN		CICSDiag*		対象コントロール
//				int				行番号
//				int				列番号
//		RET		なし
//*******************************************************************************
void CfrmUc000Common::CmnDiagSetCursorPos(CICSDiag* pDiag, int intRow, int intCol)
{
	if(pDiag == NULL)	return;

	// インデックス取得
	int intIndex = CmnGetControlIndex( intRow , intCol );

	// ダイアグラムの入力値取得
	CString str = m_clsFunc.DiagGetString(pDiag, intIndex);

	// 文字列長を取得し、末尾にカーソルをセット
	int len = _tcsclen(str);
	if(len < 1)		return;

	pDiag->SetCursorPos(intIndex, len);
}

// midori 160607 add -->
//********************************************************************************
//	「前の様式へ」、「次の様式へ」のボタンの制御（業務起動時に呼ばれる）
//********************************************************************************
void CfrmUc000Common::UpdateYoushikiMoveButton2( void )
{
	UpdateYoushikiMoveButton(m_YoushikiSkip);
}
// midori 160607 add <--

// midori 160612 add -->
//	--------------------------------------------------------------------------------------------------------
//	ダイアグラムの入力項目を入力可又は、不可にする
//	--------------------------------------------------------------------------------------------------------
void CfrmUc000Common::DlgSet_Attr( CICSDiag * pDiag, short pIndex, BOOL pBrv)
{
	int					flg;			//	属性変更フラグ

	switch( pBrv )	{
	case	FALSE:
			//	入力項目を入力不可にする
			flg		= DIAG_MDFY_READONLY ;
			break;
	default	:
			//	入力項目を入力可にする
			flg		= DIAG_MDFY_EDIT ;

// 修正No.157633_元号日付 add -->
			if(bG_Kanso == TRUE) {
				int		col = 0, per = 0;

				per = (pIndex % m_uInfo.intColMax);
				col = per - m_uInfo.intCtlStartIndex + 1;

				switch( m_uInfo.intFormSeq )	{
				case	ID_FORMNO_021:	// 様式②：振出年月日、支払期日
// インボイス登録番号追加対応_23/09/11 del -->
					//if(col == ID_COL_021_SPDATE || col == ID_COL_021_PMDATE) {
// インボイス登録番号追加対応_23/09/11 del <--
// インボイス登録番号追加対応_23/09/11 add -->
					if((bG_InvNo == FALSE && (col == ID_COL_021_SPDATE || col == ID_COL_021_PMDATE))
					|| (bG_InvNo == TRUE && (col == ID_COL_021I_SPDATE || col == ID_COL_021I_PMDATE)))	{
// インボイス登録番号追加対応_23/09/11 add <--
						flg = DIAG_MDFY_EDIT | MODIFY_GENGODEF;
					}
					break;
				case	ID_FORMNO_061:	// 様式⑥：異動年月日
					if(col == ID_COL_061K_SPDATE) {
						flg = DIAG_MDFY_EDIT | MODIFY_GENGODEF;
					}
					break;
				case	ID_FORMNO_071:	// 様式⑦：異動年月日
// インボイス登録番号追加対応_23/09/11 del -->
					//if(col == ID_COL_071_SPDATE || col == ID_COL_071_SPDATE2) {
// インボイス登録番号追加対応_23/09/11 del <--
// インボイス登録番号追加対応_23/09/11 add -->
					if((bG_InvNo == FALSE && (col == ID_COL_071_SPDATE || col == ID_COL_071_SPDATE2))
					|| (bG_InvNo == TRUE && (col == ID_COL_071I_SPDATE || col == ID_COL_071I_SPDATE2)))	{
// インボイス登録番号追加対応_23/09/11 add <--
						flg = DIAG_MDFY_EDIT | MODIFY_GENGODEF;
					}
					break;
				case	ID_FORMNO_081:	// 様式⑧：振出年月日、支払期日
// インボイス登録番号追加対応_23/09/11 del -->
					//if(col == ID_COL_081_SPDATE || col == ID_COL_081_PMDATE)	{
// インボイス登録番号追加対応_23/09/11 del <--
// インボイス登録番号追加対応_23/09/11 add -->
					if( (bG_InvNo == FALSE && (col == ID_COL_081_SPDATE || col == ID_COL_081_PMDATE))
					 || (bG_InvNo == TRUE && (col == ID_COL_081I_SPDATE || col == ID_COL_081I_PMDATE)) )	{
// インボイス登録番号追加対応_23/09/11 add -->
						flg = DIAG_MDFY_EDIT | MODIFY_GENGODEF;
					}
					break;
				case	ID_FORMNO_102:	// 様式⑩-2：支払年月
					if(col == ID_COL_102_SPDATE)	{
						flg = DIAG_MDFY_EDIT | MODIFY_GENGODEF;
					}
					break;
				case	ID_FORMNO_121:	// 様式⑫：売上年月
// インボイス登録番号追加対応_23/09/11 del -->
					//if(col == ID_COL_121_SPDATE)	{
// インボイス登録番号追加対応_23/09/11 del <--
// インボイス登録番号追加対応_23/09/11 add -->
					if( (bG_InvNo == FALSE && col == ID_COL_121_SPDATE) || (bG_InvNo == TRUE && col == ID_COL_121I_SPDATE) )	{
// インボイス登録番号追加対応_23/09/11 add <--
						flg = DIAG_MDFY_EDIT | MODIFY_GENGODEF;
					}
					break;
				case	ID_FORMNO_151:	// 様式⑮-1：支払対象期間1、支払対象期間2
// インボイス登録番号追加対応_23/09/11 del -->
					//if(col == ID_COL_151_SPDATE || col == ID_COL_151_SPDATE2)	{
// インボイス登録番号追加対応_23/09/11 del <--
// インボイス登録番号追加対応_23/09/11 add -->
					if( (bG_InvNo == FALSE && (col == ID_COL_151_SPDATE || col == ID_COL_151_SPDATE2))
					 || (bG_InvNo == TRUE && (col == ID_COL_151I_SPDATE || col == ID_COL_151I_SPDATE2)) )	{
// インボイス登録番号追加対応_23/09/11 add <--
						flg = DIAG_MDFY_EDIT | MODIFY_GENGODEF;
					}
					break;
				case	ID_FORMNO_152:	// 様式⑮-2：支払年月日
// インボイス登録番号追加対応_23/09/11 del -->
					//if(col == ID_COL_152_SPDATE)	{
// インボイス登録番号追加対応_23/09/11 del <--
// インボイス登録番号追加対応_23/09/11 add -->
					if( (bG_InvNo == FALSE && col == ID_COL_152_SPDATE) || (bG_InvNo == TRUE && col == ID_COL_152I_SPDATE) )	{
// インボイス登録番号追加対応_23/09/11 add <--
						flg = DIAG_MDFY_EDIT | MODIFY_GENGODEF;
					}
					break;
				case	ID_FORMNO_153:	// 様式⑮-3：契約期間1、契約期間2、支払対象期間1、支払対象期間2
// インボイス登録番号追加対応_23/09/11 del -->
					//if( col == ID_COL_153_KYDATE || col == ID_COL_153_KYDATE2 ||
					//	col == ID_COL_153_SPDATE || col == ID_COL_153_SPDATE2)	{
// インボイス登録番号追加対応_23/09/11 del <--
// インボイス登録番号追加対応_23/09/11 add -->
					if( (bG_InvNo == FALSE && (col == ID_COL_153_KYDATE || col == ID_COL_153_KYDATE2 || col == ID_COL_153_SPDATE || col == ID_COL_153_SPDATE2))
					 || (bG_InvNo == TRUE && (col == ID_COL_153I_KYDATE || col == ID_COL_153I_KYDATE2 || col == ID_COL_153I_SPDATE || col == ID_COL_153I_SPDATE2)) )	{
// インボイス登録番号追加対応_23/09/11 add <--
						flg = DIAG_MDFY_EDIT | MODIFY_GENGODEF;
					}
					break;
				default	:
					break;
				}
			}
// 修正No.157633_元号日付 add <--

			break;
	}
	//	入力項目を入力可又は、不可にする
	pDiag->ModifyItem(pIndex,flg);
}
// midori 160612 add <--

// midori 152201 add -->
//**************************************************
//	KnOderを取得しコンボボックスのインデックスを返送する
//	【引数】	intFormSeq	…	帳表FormSeq番号
//				inPage		…	KnOrderを取得するページ
//				inRow		…	KnOrderを取得する行
//	【戻値】	-1			…	エラー
//				0以上		…	コンボボックスのインデックス
//**************************************************
short CfrmUc000Common::GetKnOrder(int intFormSeq,int inPage,int inRow)
{
	long					knorder=0;
	short					ret=0;			// 戻り値

	CdbUc000Common*			rsData;

	// 様式テーブル取得
	rsData = m_clsFunc.CreateDbObject(intFormSeq,m_pDB);

	// ページ、行で絞り込む
	rsData->RequeryPageRow(inPage,inRow);

	switch (intFormSeq) {
	case ID_FORMNO_011:	// ① 預貯金等の内訳書
		knorder = ((CdbUc011Yotyokin*)rsData)->m_KnOrder;
		break;
	case ID_FORMNO_021:	// ② 受取手形の内訳書
		knorder = ((CdbUc021Uketoritegata*)rsData)->m_KnOrder;
		break;
	case ID_FORMNO_031:	// ③ 売掛金（未収入金）の内訳書
		knorder = ((CdbUc031Urikakekin*)rsData)->m_KnOrder;
		break;
	case ID_FORMNO_041: // ④－１ 仮払金（前私金）の内訳書
		knorder = ((CdbUc041Karibarai*)rsData)->m_KnOrder;
		break;
	case ID_FORMNO_051: // ⑤ 棚卸資産（商品又は製品、半製品、仕掛品、原材料、貯蔵品）の内訳書
		knorder = ((CdbUc051Tanaoroshi*)rsData)->m_KnOrder;
		break;
	case ID_FORMNO_061: // ⑥ 有価証券の内訳書
		knorder = ((CdbUc061Yuukasyouken*)rsData)->m_KnOrder;
		break;
// 改良No.21-0086,21-0529 add -->
	case ID_FORMNO_081:	// ⑧ 支払手形の内訳書
		knorder = ((CdbUc081Siharaitegata*)rsData)->m_KnOrder;
// 改良No.21-0086,21-0529 add <--
	case ID_FORMNO_091: // ⑨ 買掛金（未払金・未払費用）の内訳書
		knorder = ((CdbUc091Kaikakekin*)rsData)->m_KnOrder;
		break;
	case ID_FORMNO_101: // ⑩－１ 仮受金（前受金・預り金）の内訳書
		knorder = ((CdbUc101Kariukekin*)rsData)->m_KnOrder;
		break;
	case ID_FORMNO_102: // ⑩－２ 源泉取得税預り金の内訳
		knorder = ((CdbUc102Gensen*)rsData)->m_KnOrder;
		break;
	case ID_FORMNO_111: // ⑪ 借入金及び支払利子の内訳書
		knorder = ((CdbUc111Kariirekin*)rsData)->m_KnOrder;
		break;
	case ID_FORMNO_121: // ⑫ 土地の売上高等の内訳書
		knorder = ((CdbUc121Totiuriage*)rsData)->m_KnOrder;
		break;
	case ID_FORMNO_151: // ⑮ 地代家賃等の内訳書
		knorder = ((CdbUc151Tidaiyatin*)rsData)->m_KnOrder;
		break;
	case ID_FORMNO_161: // ⑯－１ 雑益、雑損失等の内訳書
		knorder = ((CdbUc161Zatueki*)rsData)->m_KnOrder;
		break;
	case ID_FORMNO_162: // ⑯－２ 雑損失等の内訳書
		knorder = ((CdbUc162Zatuson*)rsData)->m_KnOrder;
		break;
	case ID_FORMNO_171: // ⑰－１	その他科目の内訳書①－１
	case ID_FORMNO_172:
	case ID_FORMNO_173:
	case ID_FORMNO_174:
	case ID_FORMNO_175:
	case ID_FORMNO_176:
	case ID_FORMNO_177:
	case ID_FORMNO_178:
	case ID_FORMNO_179:
	case ID_FORMNO_1710:
	case ID_FORMNO_1711:
	case ID_FORMNO_1712:
	case ID_FORMNO_1713:
	case ID_FORMNO_1714:
	case ID_FORMNO_1715:
	case ID_FORMNO_1716:
	case ID_FORMNO_1717:
	case ID_FORMNO_1718:
	case ID_FORMNO_1719:
	case ID_FORMNO_1720:
		knorder = ((CdbUc171Sonota1*)rsData)->m_KnOrder;
		break;

	case ID_FORMNO_181: // ⑱－１	その他科目の内訳書②－１
	case ID_FORMNO_182:
	case ID_FORMNO_183:
	case ID_FORMNO_184:
	case ID_FORMNO_185:
	case ID_FORMNO_186:
	case ID_FORMNO_187:
	case ID_FORMNO_188:
	case ID_FORMNO_189:
	case ID_FORMNO_1810:
	case ID_FORMNO_1811:
	case ID_FORMNO_1812:
	case ID_FORMNO_1813:
	case ID_FORMNO_1814:
	case ID_FORMNO_1815:
	case ID_FORMNO_1816:
	case ID_FORMNO_1817:
	case ID_FORMNO_1818:
	case ID_FORMNO_1819:
	case ID_FORMNO_1820:
		knorder = ((CdbUc181Sonota2*)rsData)->m_KnOrder;
		break;

	default:
		break;
	}

	m_SortMap1.Lookup(knorder,ret);

	return(ret);
}
// midori 152201 add <--

// midori M-16090701 add -->
LRESULT CfrmUc000Common::OnUserFocusset( WPARAM wParam, LPARAM lParam )
{
	// カーソル移動
// 修正No.162446 del -->
//// 修正No.162381,162382 del -->
//	//CmnDiagSetFocus(m_pDiag,m_uInfo.intCurRow,m_uInfo.intCurCol);
//// 修正No.162381,162382 del <--
//// 修正No.162381,162382 add -->
//	if(wParam > 0 && lParam > 0)	CmnDiagSetFocus(m_pDiag, wParam, lParam);
//	else							CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
//// 修正No.162381,162382 add <--
//// 修正No.162446 del <--
// 修正No.162446 add -->
	CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
// 修正No.162446 add <--

	return(0);
}
// midori M-16090701 add <--

// 修正No.168393 add -->
LRESULT CfrmUc000Common::OnUserFocusset4(WPARAM wParam, LPARAM lParam)
{
	// 指定された場所にカーソル移動
	CmnDiagSetFocus(m_pDiag, wParam, lParam);
	return(0);
}
// 修正No.168393 add <--

// midori 152364 add -->
//	--------------------------------------------------------------------------------------------------------
//	渡された区分が特殊行の区分であるかの判断
//	--------------------------------------------------------------------------------------------------------
//		IN		BYTE			区分
//		RET		1:特殊行
//	--------------------------------------------------------------------------------------------------------
int CfrmUc000Common::IsSpecialRow( BYTE pSw )
{
	int		ret=0;

	switch(pSw)	{
		case	ID_FGFUNC_SYOKEI:
		case	ID_FGFUNC_SYOKEINULL:
		case	ID_FGFUNC_CHUKEI:
		case	ID_FGFUNC_CHUKEINULL:
		case	ID_FGFUNC_IKKATUAUTO:
// midori 190505 add -->
		case	ID_FGFUNC_KAMOKU:
// midori 190505 add <--
			ret=1;
			break;
	}
	return(ret);
}
// midori 152364 add <--

// 修正No.157898 add -->
//	--------------------------------------------------------------------------------------------------------
//	渡された区分が一括自動であるかの判断（但し簡素化様式の場合のみ）
//	--------------------------------------------------------------------------------------------------------
//		IN		BYTE			区分
//		RET		1:特殊行
//	--------------------------------------------------------------------------------------------------------
int CfrmUc000Common::IsSpecialRow2( BYTE pSw )
{
	int		ret=0;

	switch(pSw)	{
		case	ID_FGFUNC_IKKATUAUTO:
// 改良No.21-0086,21-0529 cor -->
			//if(m_KmkRowSgn == 1)	{
// ------------------------------
			if(KamokuRowEnableSgn(m_pDB, 0, m_uInfo.intFormSeq) == 1)	{
// 改良No.21-0086,21-0529 cor <--
				ret=1;
			}
			break;
	}

	return(ret);
}
// 修正No.157898 add <--

// midori 190505 add -->
//// midori 157043 del -->
////	--------------------------------------------------------------------------------------------------------
////	印刷設定「科目行を出力する」にチェックが付いているか確認
////	--------------------------------------------------------------------------------------------------------
//void CfrmUc000Common::KamokuRowChk(void)
//{
//// midori 156186,156187,156188 del -->
////	int				iFormSeq=0;
////	BOOL			b021=FALSE,b111=FALSE;
////	CString			strMsg=_T("");
////	CdbUcInfSub		mfcRecSub(m_pDB);	//	uc_inf_subテーブルクラス
////	ITEMSORT_INFO	uItemSort;			// 第１ソート項目のソート情報
//// midori 156186,156187,156188 del <--
//// midori 156186,156187,156188 add -->
//	CString			strMsg=_T("");
//	CdbUcInfMain	mfcRecMain(m_pDB);	//	uc_inf_mainテーブルクラス
//// midori 156186,156187,156188 add <--
//
//// midori 156188_4 add -->
//	if(KamokuRowEnableSgn(m_pDB,0) == 0) {
//		return;
//	}
//// midori 156188_4 add <--
//
//// midori 156186 add -->
//	if( mfcRecMain.Init() == DB_ERR_OK ){
//		CCtrlMainGeneralVar cm(mfcRecMain);
//		// 科目行処理方法変更メッセージ表示サインがONの場合処理を行わない
//		if(cm.bKmkRowChk) {
//			// uc_inf_mainテーブルのクローズ
//			mfcRecMain.Fin();
//			return;
//		}
//		// 科目行処理方法変更メッセージ表示サインをONにする
//		mfcRecMain.Edit();
//		cm.bKmkRowChk = TRUE;
//		cm.Set(mfcRecMain);
//		mfcRecMain.Update();
//		// uc_inf_mainテーブルのクローズ
//		mfcRecMain.Fin();
//	}
//// midori 156186 add <--
//
//// midori 156186,156187,156188 del -->
//	//for(int ii=0; ii<2; ii++)	{
//	//	if(ii == 0)	iFormSeq = ID_FORMNO_021;
//	//	else		iFormSeq = ID_FORMNO_111;
//
//	//	// 様式⑪でリクエリ
//	//	if(mfcRecSub.RequeryFormSeq( iFormSeq ) == DB_ERR_OK)	{
//	//		if( !(mfcRecSub.IsEOF()) ){
//	//			// 「科目行を出力する」チェックついてる？
//	//			if(mfcRecSub.m_GeneralConstVal[15] & BIT_D1)	{
//	//				CCtrlSubGeneralVar	cs( mfcRecSub );
//	//				if( cs.bOutKamokuRow )	{
//	//					if(ii == 0)	b021 = TRUE;
//	//					else		b111 = TRUE;
//	//				}
//	//			}
//	//		}
//	//		mfcRecSub.Fin();
//	//	}
//	//}
//	//// チェックが付いていた
//	//if(b021 == TRUE || b111 == TRUE)	{
//	//	strMsg  = _T("当バージョンより、以下の様式の「科目行」の処理が変わりました。\r\n");
//	//	strMsg += _T("　・②受取手形の内訳書\r\n");
//	//	strMsg += _T("　・⑪借入金及び支払利子の内訳書\r\n");
//	//	strMsg += _T("「科目行」は印刷時に出力していましたが、当バージョンより画面に挿入できるようになりました。\r\n");
//	//	strMsg += _T("「科目行」は、編集ーソートの画面で「科目行を挿入する」にチェックを付け「実行」ボタンを押す、\r\n");
//	//	strMsg += _T("または、各科目の先頭行で編集ー特殊行挿入を選択し科目行を選んで「挿入」ボタンを押すと、画面に挿入されます。");
//	//	ICSMessageBox(strMsg, MB_OK | MB_ICONEXCLAMATION);
//	//	// チェックOFF
//	//	if(b021 == TRUE)	{
//	//		if(mfcRecSub.RequeryFormSeq( ID_FORMNO_021 ) == DB_ERR_OK)	{
//	//			CCtrlSubGeneralVar cs( mfcRecSub );
//	//			mfcRecSub.Edit();
//	//			cs.bOutKamokuRow = FALSE;
//	//			// ソートの第1ソートキーが科目の場合、[ソート]-[科目行を挿入する]のチェックをONにする
//	//			// uc_lst_item_sortを参照して必要情報を取得
//	//			CmnUcLstItemSortGetData2(ID_FORMNO_021,mfcRecSub.m_Sort1, &uItemSort);
//	//			// ソートダイアログを開いていないまたは第１ソートキーが科目の場合
//	//			if(mfcRecSub.m_Sort1 == 0 || uItemSort.bytFgItem == ID_FGITEM_KAMOKU) {
//	//				cs.bKamokuTitle = TRUE;
//	//			}
//	//			cs.Set( mfcRecSub );
//	//			// 科目指定を行うにチェックが有る場合、[科目指定]-[科目行を自動挿入する]のチェックをONにする
//	//			// 科目指定を行うチェックON
//	//			if(mfcRecSub.m_GeneralVar[1] & BIT_D3) {
//	//				// [科目指定]-[科目行を自動挿入する]チェックON
//	//				mfcRecSub.m_GeneralVar[2] |= BIT_D3;
//	//			}
//	//			//	更新
//	//			mfcRecSub.Update();
//	//			// 閉じる
//	//			mfcRecSub.Fin();
//	//		}
//	//	}
//	//	if(b111 == TRUE)	{
//	//		if(mfcRecSub.RequeryFormSeq( ID_FORMNO_111 ) == DB_ERR_OK)	{
//	//			CCtrlSubGeneralVar cs( mfcRecSub );
//	//			mfcRecSub.Edit();
//	//			cs.bOutKamokuRow = FALSE;
//	//			// ソートの第1ソートキーが科目の場合、[ソート]-[科目行を挿入する]のチェックをONにする
//	//			// uc_lst_item_sortを参照して必要情報を取得
//	//			CmnUcLstItemSortGetData2(ID_FORMNO_111,mfcRecSub.m_Sort1, &uItemSort);
//	//			// ソートダイアログを開いていないまたは第１ソートキーが科目の場合
//	//			if(mfcRecSub.m_Sort1 == 0 || uItemSort.bytFgItem == ID_FGITEM_KAMOKU) {
//	//				cs.bKamokuTitle = TRUE;
//	//			}
//	//			cs.Set( mfcRecSub );
//	//			// 科目指定を行うにチェックが有る場合、[科目指定]-[科目行を自動挿入する]のチェックをONにする
//	//			// 科目指定を行うチェックON
//	//			if(mfcRecSub.m_GeneralVar[1] & BIT_D3) {
//	//				// [科目指定]-[科目行を自動挿入する]チェックON
//	//				mfcRecSub.m_GeneralVar[2] |= BIT_D3;
//	//			}
//	//			//	更新
//	//			mfcRecSub.Update();
//	//			// 閉じる
//	//			mfcRecSub.Fin();
//	//		}
//	//	}
//	//}
//// midori 156186,156187,156188 del <--
//// midori 156186,156187,156188 add -->
//// midori 156916 del -->
//	//strMsg  = _T("当バージョンより、以下の様式の「科目行」の処理が変わりました。\r\n");
//	//strMsg += _T("　・②受取手形の内訳書\r\n");
//	//strMsg += _T("　・⑪借入金及び支払利子の内訳書\r\n");
//	//strMsg += _T("「科目行」は印刷時に出力していましたが、当バージョンより画面に表示できるようになりました。\r\n");
//	//strMsg += _T("「科目行」は、編集－ソートの画面で「科目行を挿入する」にチェックを付け「実行」ボタンを押す、\r\n");
//	//strMsg += _T("または、各科目の先頭行で編集－特殊行挿入を選択し科目行を選んで「挿入」ボタンを押すと、画面に挿入されます。");
//// midori 156916 del <--
//// midori 156916 add -->
//	strMsg  = _T("以下の様式の「科目行」の処理について\r\n");
//	strMsg += _T("　・②受取手形の内訳書\r\n");
//	strMsg += _T("　・⑪借入金及び支払利子の内訳書\r\n");
//	strMsg += _T("「科目行」は印刷時に表示していましたが、画面にも表示できるように対応しました。\r\n");
//	strMsg += _T("編集－ソートの「科目行を挿入する」にチェックを付けて［実行］ボタンを押す、または、\r\n");
//	strMsg += _T("各科目の先頭行で編集－特殊行挿入の「科目行」を選択して［挿入］ボタンを押すと、画面に挿入されます。");
//// midori 156916 add <--
//	ICSMessageBox(strMsg, MB_OK | MB_ICONEXCLAMATION);
//// midori 156186,156187,156188 add <--
//}
// midori 157043 del <--

// -----------------------------------------------------------------
// 小計名称を取得する
// -----------------------------------------------------------------
CString CfrmUc000Common::GetSyokeiName(int nFormSeq,int ppage,int prow,CString pstrKei)
{
// midori 156189,156190,156191 del -->
	//int					knorder = -1;
// midori 156189,156190,156191 del <--
	int					sw = 0;				// 0:条件に一致しない 1:小計名称に科目名を追加しない	2:小計名称に科目名を追加する
	ITEMSORT_INFO		uItemSort;			// 第１ソート項目のソート情報
	SORTKAMOKU_INFO		uKamoku;
	CString				KName=_T("");
	CString				strKei=_T("");
	CString				strfilter=_T("");
	CString				strsort=_T("");
	CdbUc000Common*		rsData;
	CdbUcInfSub			mfcRec( m_pDB );	//	uc_inf_subテーブルクラス

	// データ初期化
	uItemSort.bytFgSykei		= 0;			// 小計の対象である/ない
	uItemSort.bytFgItem			= 0;			// 項目の種類を示す番号
	uItemSort.strOrderStr		= "";			// 並び替え用SQL文字列
	uItemSort.strFieldSykei		= "";			// 小計のグループ判定に使用するフィールド名
	uItemSort.strFieldSykei2	= "";			// 小計のグループ判定に使用するフィールド名
	uItemSort.strOrderStr2		= _T("");		// 50音並び替え用SQL文字列
// 20-0450 add -->
	uItemSort.intItemSeq		= 0;
// 20-0450 add <--

// 改良No.21-0086,21-0529 cor -->
	//// 様式が②受取手形の内訳書、⑪借入金及び支払利子の内訳書でなければ処理を行わない
	//if(nFormSeq != ID_FORMNO_021 && nFormSeq != ID_FORMNO_111) {
// ------------------------------
	// 様式が②受取手形の内訳書、登録番号追加版の⑧支払手形の内訳書、⑪借入金及び支払利子の内訳書でなければ処理を行わない
	if( !(nFormSeq == ID_FORMNO_021 || (bG_InvNo == TRUE && nFormSeq == ID_FORMNO_081) || nFormSeq == ID_FORMNO_111))	{
// 改良No.21-0086,21-0529 cor <--
		return(pstrKei);
	}

	// CdbUcInfSub
	//	初期化成功？
	if ( mfcRec.RequeryFormSeq( nFormSeq ) != DB_ERR_OK ){
		return(pstrKei);
	}
	//	レコードあり？
	if ( mfcRec.IsEOF() ){
		mfcRec.Fin();
		return(pstrKei);
	}
	// uc_lst_item_sortを参照して必要情報を取得
	CmnUcLstItemSortGetData(mfcRec.m_Sort1, &uItemSort);
	// 第１ソート項目が科目以外の場合、処理を行わない
	if(mfcRec.m_Sort1 > 0 && uItemSort.bytFgItem != ID_FGITEM_KAMOKU)	{
		mfcRec.Fin();
		return(pstrKei);
	}
	mfcRec.Fin();

	rsData = virGetDataTableObject();	// 引数に pdb 必要??

	// 指定位置より前のデータを取得
	strfilter.Format(_T("((NumPage = %d AND NumRow < %d) OR (NumPage < %d)) and FgShow = %d and (fgfunc <> %d and fgfunc <> %d and fgfunc <> %d) ")
					,ppage,prow,ppage,ID_FGSHOW_OFF,ID_FGFUNC_NULL,ID_FGFUNC_RUIKEI,ID_FGFUNC_PAGEKEI);
	strsort = _T("NumPage ASC, NumRow ASC, Seq DESC");
	rsData->RequerySortParameter(strfilter,strsort);

	// データが無い場合処理を行わない
	if(rsData->IsEOF()) {
		rsData->Fin();
		delete rsData;
		return(pstrKei);
	}

	rsData->MoveLast();
	while(!rsData->IsBOF()) {
		// 科目の取得無し
// midori 156189,156190,156191 del -->
		//if(knorder == -1) {
// midori 156189,156190,156191 del <--
// midori 156189,156190,156191 add -->
		if(KName.IsEmpty() == TRUE) {
// midori 156189,156190,156191 add <--
			// 明細行または一括金額の行
			if( rsData->m_FgFunc == ID_FGFUNC_DATA || 
				rsData->m_FgFunc == ID_FGFUNC_IKKATUMANUAL || 
				rsData->m_FgFunc == ID_FGFUNC_IKKATUAUTO) {
				// 科目データの取得
				virTblGetKamokuInfo(rsData, &uKamoku);
				if(uKamoku.intKnOrder == 0) {
					// 小計名称に科目名を追加しない
					sw = 1;
					break;
				}
				else {
// midori 156189,156190,156191 del -->
					//knorder = uKamoku.intKnOrder;
// midori 156189,156190,156191 del <--
					KName = uKamoku.strKnName;
				}
			}
			else {
				// 小計名称に科目名を追加しない
				sw = 1;
				break;
			}
		}
		// 科目の取得あり
		else {
// midori 156189,156190,156191 del -->
			//// 科目データの取得
			//virTblGetKamokuInfo(rsData, &uKamoku);
			//// 科目行
			//if(rsData->m_FgFunc == ID_FGFUNC_KAMOKU) {
			//	// 科目が同じ
			//	if(knorder == uKamoku.intKnOrder) {
			//		// 小計名称に科目名を追加しない
			//		sw = 1;
			//		break;
			//	}
			//	else {
			//		// 小計名称に科目名を追加する
			//		sw = 2;
			//		break;
			//	}
			//}
			//// 科目行以外
			//// 小計、小計（空白）、中計、中計（空白）行のKnOrderは0のため、
			//// knorder != uKamoku.intKnOrderで判断可能
			//else {
			//	// 科目が変更された
			//	if(knorder != uKamoku.intKnOrder) {
			//		// 小計名称に科目名を追加する
			//		sw = 2;
			//		break;
			//	}
			//}
			//knorder = uKamoku.intKnOrder;
// midori 156189,156190,156191 del <--
// midori 156189,156190,156191 add -->
			if(rsData->m_FgFunc == ID_FGFUNC_KAMOKU) {
				sw = 1;
				break;
			}
			// 小計、小計（空白）、中計、中計（空白）
			if( rsData->m_FgFunc == ID_FGFUNC_SYOKEI ||
				rsData->m_FgFunc == ID_FGFUNC_SYOKEINULL ||
				rsData->m_FgFunc == ID_FGFUNC_CHUKEI ||
				rsData->m_FgFunc == ID_FGFUNC_CHUKEINULL) {
				break;
			}
// midori 156189,156190,156191 add <--
		}
		rsData->MovePrev();
	}

	strKei = pstrKei;
	// 小計名称に科目名を追加する
// midori 156189,156190,156191 del -->
	//if((sw == 0 && knorder > 0) || sw == 2) {
// midori 156189,156190,156191 del <--
// midori 156189,156190,156191 add -->
	if(sw == 0 && KName.IsEmpty() == FALSE) {
// midori 156189,156190,156191 add <--
		strKei.Empty();
		GetKamokuKeiStr(pstrKei,KName,&strKei);
	}

	rsData->Fin();
	delete rsData;

	return(strKei);
}

// midori 156950 del -->
//int CfrmUc000Common::SortPrintMsg(int nFormSeq, int pSw)
// midori 156950 del <--
// midori 156950 add -->
int CfrmUc000Common::SortPrintMsg(int nFormSeq)
// midori 156950 add <--
{
	int					rv = 0;
// midori 156950 del -->
//int					flg = 0;	// 0:確認メッセージの表示なし
//									// 1:該当様式の確認メッセージを表示
//									// 2:複数様式が同時に出力される場合、自身以外の様式に対して確認メッセージを表示
//									// 3:複数様式が同時に出力される場合、自身と自身以外の様式に対して確認メッセージを表示
//									// 9:一括印刷ダイアログ用の確認メッセージを表示
//int					flg1=0,flg2=0,flg3=0;
// midori 156950 del <--
// midori 156950 add -->
	int					ret=0;
	int					flg=0;		//	0:確認メッセージの表示なし
									//	1:印刷時ソート確認メッセージ、自身１様式が対象の場合の確認メッセージを表示
									//	2:印刷時ソート確認メッセージ、自身と相手１様式が対象の場合の確認メッセージを表示
									//	3:印刷時ソート確認メッセージ、自身と相手２様式が対象の場合の確認メッセージを表示
									//	4:印刷時ソート確認メッセージ、相手１様式が対象の場合の確認メッセージを表示
									//	5:印刷時ソート確認メッセージ、相手２様式が対象の場合の確認メッセージを表示
	int					flg1=0;		// 自身が確認ダイアログ表示対象か
	int					flg2=0;		// 相手１様式目が確認ダイアログ表示対象か
// midori 156950 add <--
	int					cnt=0;
	int					fs=0,fs2=0;
	int					type=ID_FORMTYPE_KOBETU;
	CString				cst=_T("");
	CString				strSQL=_T("");
	CdbUcInfSub			mfcSub( m_pDB );			//	uc_inf_subテーブルクラス
	CdbUcInfFormType	mfcFormType( m_pDB );		//	CdbUcInfFormTypeテーブルクラス

	try{
// midori 20/10/08_2 add -->
		// 確認ダイログを表示しない場合は下記の処理を行わない
		if(prnConfimationSw == FALSE) {
			return(0);
		}
// midori 20/10/08_2 add <--

		// ①事業期間の期首が平成31年4月1日以降
		// ②過年度確定で無い
		//if(KamokuRowEnableSgn(m_pDB, 0) != 0) {			// 改良No.21-0086,21-0529 del
		if(KamokuRowEnableSgn(m_pDB, 0, nFormSeq) != 0)	{	// 改良No.21-0086,21-0529 add
// midori 156950 del -->
			//// 選択されている様式のみチェック
			//if(pSw == 0) {
			//	if ( mfcSub.RequeryFormSeq( nFormSeq ) == DB_ERR_OK ){
			//		if(!mfcSub.IsEOF())	{
			//			// 出力形式を取得
			//			mfcFormType.Init(mfcSub.m_FormNo);
			//			if(!mfcFormType.IsEOF())	{
			//				type = mfcFormType.m_FormType;
			//			}
			//			mfcFormType.Fin();
			//		}
			//	}
			//	mfcSub.Fin();
			//	// 個別帳表
			//	if(type == ID_FORMTYPE_KOBETU)	{
			//		flg = SortPrintMsgSub(nFormSeq);
			//	}
			//	// 通常帳表
			//	else	{
			//		switch (nFormSeq) {
			//		case ID_FORMNO_041:
			//		case ID_FORMNO_042:
			//		case ID_FORMNO_101:
			//		case ID_FORMNO_102:
			//		case ID_FORMNO_161:
			//		case ID_FORMNO_162:
			//			if(nFormSeq == ID_FORMNO_041 || nFormSeq == ID_FORMNO_042)	{
			//				// ④-1 or ④-2
			//				flg1 = SortPrintMsgSub(ID_FORMNO_041);
			//				flg2 = SortPrintMsgSub(ID_FORMNO_042);
			//			}
			//			else if(nFormSeq == ID_FORMNO_101 || nFormSeq == ID_FORMNO_102)	{
			//				// ⑩-1 or ⑩-2
			//				flg1 = SortPrintMsgSub(ID_FORMNO_101);
			//				flg2 = SortPrintMsgSub(ID_FORMNO_102);
			//			}
			//			else if(nFormSeq == ID_FORMNO_161 || nFormSeq == ID_FORMNO_162)	{
			//				// ⑯-1 or ⑯-2
			//				flg1 = SortPrintMsgSub(ID_FORMNO_161);
			//				flg2 = SortPrintMsgSub(ID_FORMNO_162);
			//			}
			//			if(flg1 != 0 || flg2 != 0)	{
			//				flg = 1;
			//				if(flg1 != 0 && flg2 != 0)	{
			//					flg = 3;
			//				}
			//				else if(flg1 != 0)	{
			//					if(nFormSeq == ID_FORMNO_042 || nFormSeq == ID_FORMNO_102 || nFormSeq == ID_FORMNO_162)	{
			//						flg = 2;
			//					}
			//				}
			//				else if(flg2 != 0)	{
			//					if(nFormSeq == ID_FORMNO_041 || nFormSeq == ID_FORMNO_101 || nFormSeq == ID_FORMNO_161)	{
			//						flg = 2;
			//					}
			//				}
			//			}
			//			break;

			//		case ID_FORMNO_141:
			//		case ID_FORMNO_142:
			//			// ⑭-1 or ⑭-2（※⑭-2は対象外）
			//			flg = SortPrintMsgSub(ID_FORMNO_141);
			//			if(flg != 0 && nFormSeq == ID_FORMNO_142)	flg = 2;
			//			break;

			//		case ID_FORMNO_151:
			//		case ID_FORMNO_152:
			//		case ID_FORMNO_153:
			//			// ⑮-1 or ⑮-2 or ⑮-3
			//			flg1 = SortPrintMsgSub(ID_FORMNO_151);
			//			flg2 = SortPrintMsgSub(ID_FORMNO_152);
			//			flg3 = SortPrintMsgSub(ID_FORMNO_153);
			//			cnt=0;
			//			if(flg1 != 0)	cnt++;
			//			if(flg2 != 0)	cnt++;
			//			if(flg3 != 0)	cnt++;
			//			// 該当する様式が２件以上の場合
			//			if(cnt >= 2)	{
			//				// 自身が該当していれば、ダイアログ３を表示
			//				if(flg1 != 0 && nFormSeq == ID_FORMNO_151)		flg = 3;
			//				else if(flg2 != 0 && nFormSeq == ID_FORMNO_152)	flg = 3;
			//				else if(flg3 != 0 && nFormSeq == ID_FORMNO_153)	flg = 3;
			//				// 自身が該当していなければ、ダイアログ２を表示
			//				else											flg = 2;
			//			}
			//			// 該当する様式が１件の場合
			//			else if(cnt == 1)	{
			//				if(flg1 != 0)	{
			//					// 該当するのが自身以外の場合は、ダイアログ２を表示
			//					if(nFormSeq != ID_FORMNO_151)	flg = 2;
			//					// 該当するのが自身の場合は、ダイアログ１を表示
			//					else							flg = 1;
			//				}
			//				if(flg2 != 0)	{
			//					if(nFormSeq != ID_FORMNO_152)	flg = 2;
			//					else							flg = 1;
			//				}
			//				if(flg3 != 0)	{
			//					if(nFormSeq != ID_FORMNO_153)	flg = 2;
			//					else							flg = 1;
			//				}
			//			}
			//			// 該当する様式がない場合は、ダイアログを表示しない
			//			else	{
			//				flg = 0;
			//			}
			//			break;
			//		}
			//	}
			//}
			//// 全ての様式にチェックを掛ける
			//else {
			//	flg = 9;
			//}
// midori 156950 del <--
// midori 156950 add -->
			if ( mfcSub.RequeryFormSeq( nFormSeq ) == DB_ERR_OK ){
				if(!mfcSub.IsEOF())	{
					// 出力形式を取得
					mfcFormType.Init(mfcSub.m_FormNo);
					if(!mfcFormType.IsEOF())	{
						type = mfcFormType.m_FormType;
					}
					mfcFormType.Fin();
				}
			}
			mfcSub.Fin();
			flg1 = 0;
			flg2 = 0;
			// 個別帳表
			if(type == ID_FORMTYPE_KOBETU)	{
				flg = SortPrintMsgSub(nFormSeq);
			}
			// 通常帳表
			else	{
				switch (nFormSeq) {
				case ID_FORMNO_041:
						// ④-１
						flg1 = SortPrintMsgSub(ID_FORMNO_041);
						break;
				case ID_FORMNO_042:
						// ④-１
						flg2 = SortPrintMsgSub(ID_FORMNO_041);
						break;
				case ID_FORMNO_101:
						// ⑩－１
						flg1 = SortPrintMsgSub(ID_FORMNO_101);
						break;
				case ID_FORMNO_102:
						// ⑩－１
						flg2 = SortPrintMsgSub(ID_FORMNO_101);
						break;
				case ID_FORMNO_161:
						// ⑯－１
						flg1 = SortPrintMsgSub(ID_FORMNO_161);
						// ⑯－２
						flg2 = SortPrintMsgSub(ID_FORMNO_162);
						break;
				case ID_FORMNO_162:
						// ⑯－２
						flg1 = SortPrintMsgSub(ID_FORMNO_162);
						// ⑯－１
						flg2 = SortPrintMsgSub(ID_FORMNO_161);
						break;
				case ID_FORMNO_141:
						// ⑭-１
						flg1 = SortPrintMsgSub(ID_FORMNO_141);
						break;
				case ID_FORMNO_142:
						// ⑭-１
						flg2 = SortPrintMsgSub(ID_FORMNO_141);
						break;
				case ID_FORMNO_151:
						// ⑮-１
						flg1 = SortPrintMsgSub(ID_FORMNO_151);
						break;
				case ID_FORMNO_152:
						// ⑮-１
						flg2 = SortPrintMsgSub(ID_FORMNO_151);
						break;
				case ID_FORMNO_153:
						// ⑮-１
						flg2 = SortPrintMsgSub(ID_FORMNO_151);
						break;
				}

				// flg		0:確認メッセージの表示なし
				//			1:印刷時ソート確認メッセージ、自身１様式が対象の場合の確認メッセージを表示
				//			2:印刷時ソート確認メッセージ、自身と相手１様式が対象の場合の確認メッセージを表示
				//			3:印刷時ソート確認メッセージ、相手１様式が対象の場合の確認メッセージを表示
				// 自身が確認メッセージ表示対象
				if(flg1 != 0) {
					// 自身が表示対象
					flg = 1;
					// 自身と相手１様式が表示対象
					if(flg2 != 0) {
						flg = 2;
					}
				}
				// 相手が確認メッセージ表示対象
				else if(flg2 != 0) {
					// 相手１様式が表示対象
					flg = 3;
				}
			}
// midori 156950 add <--
		}
	}catch(CDBException* e) {
		ICSMessageBox(e->m_strError);
		delete e;
		return(-1);
	}

// midori 156950 del -->
	//if(flg > 0) {
	//	if(flg != 9)	{
	//		// 確認ダイアログを表示
	//		int			ret=0;

	//		// 1:該当様式の確認メッセージを表示
	//		if(flg == 1)	{
	//			CdlgPrintConfirmation	dlg2(this);
	//			ret = dlg2.ShowDialog(m_pDB,nFormSeq);
	//		}
	//		// 2:複数様式が同時に出力される場合、自身以外の様式に対して確認メッセージを表示
	//		else if(flg == 2)	{
	//			CdlgPrintConfirmation2	dlg2(this);
	//			ret = dlg2.ShowDialog(m_pDB,nFormSeq);
	//		}
	//		// 3:複数様式が同時に出力される場合、自身と自身以外の様式に対して確認メッセージを表示
	//		// (⑯－１ or ⑯－２ のみ)
	//		else	{
	//			CdlgPrintConfirmation3	dlg2(this);
	//			ret = dlg2.ShowDialog(m_pDB,nFormSeq);
	//		}
	//		// 
	//		if(ret == ID_DLG_CANCEL)	{
	//			return(1);
	//		}
	//	}
	//}
	//
	//return(0);
// midori 156950 del <--
// midori 156950 add -->
	// flg		0:確認メッセージの表示なし
	//			1:印刷時ソート確認メッセージ、自身１様式が対象の場合の確認メッセージを表示
	//			3:印刷時ソート確認メッセージ、自身と相手１様式が対象の場合の確認メッセージを表示
	//			4:印刷時ソート確認メッセージ、自身と相手２様式が対象の場合の確認メッセージを表示
	//			2:印刷時ソート確認メッセージ、相手１様式が対象の場合の確認メッセージを表示
	//			5:印刷時ソート確認メッセージ、相手２様式が対象の場合の確認メッセージを表示
	ret = 0;
	if(flg > 0)	{
		//	1:自身１様式が対象の場合の確認メッセージを表示
		if(flg == 1) {
			CdlgPrintConfirmation	dlg2(this);
			ret = dlg2.ShowDialog(m_pDB,nFormSeq);
		}
		//	2:自身と相手１様式が対象の場合の確認メッセージを表示
		else if(flg == 2) {
			CdlgPrintConfirmation4	dlg2(this);
			ret = dlg2.ShowDialog(m_pDB,nFormSeq);
		}
		//	4:相手１様式が対象の場合の確認メッセージを表示
		else if(flg == 3) {
			CdlgPrintConfirmation2	dlg2(this);
			ret = dlg2.ShowDialog(m_pDB,nFormSeq);
		}
		// 
		if(ret == ID_DLG_CANCEL)	{
			return(1);
		}
	}
	return(0);
// midori 156950 add <--
}

int CfrmUc000Common::SortPrintMsgSub(int nFormSeq)
{
	int				ret=0;
	CdbUcInfSub		mfcSub( m_pDB );		//	uc_inf_subテーブルクラス

	if ( mfcSub.RequeryFormSeq( nFormSeq ) == DB_ERR_OK ){
		if ( !( mfcSub.IsEOF() ) ){
			ret = ChkIkkatuSort(m_pDB,&mfcSub);
		}
		mfcSub.Fin();
	}

	return(ret);
}
// midori 190505 add <--

// midori 155879 add -->
LRESULT CfrmUc000Common::OnUserControlPage(WPARAM wParam, LPARAM lParam)
{
	GetDlgItem( IDC_PAGEBACK_BUTTON1 )->EnableWindow( wParam );	// 前頁ボタン
	GetDlgItem( IDC_PAGENEXT_BUTTON1 )->EnableWindow( lParam );	// 次頁ボタン
	return(0);
}
// midori 155879 add <--

// midori 155906 add -->
// 印刷用一時テーブルの削除
void CfrmUc000Common::ClearTempTbl(void)
{
	int		ii=0;
	CString strTempUtiwakeTbl=_T("");
	CString strSQL=_T("");

	// 様式①～⑯-2
	for(ii = ID_FORMNO_011; ii<=ID_FORMNO_162; ii++){
#ifdef _DEBUG
		strTempUtiwakeTbl.Format(_T("##temp_utiwake_tbl_%d"),ii);
#else
		strTempUtiwakeTbl.Format(_T("#temp_utiwake_tbl_%d"),ii);
#endif
		strSQL = _T("if object_id('tempdb..") + strTempUtiwakeTbl + _T("', 'u') is not null drop table ") + strTempUtiwakeTbl;

		// SQL実行
		m_pDB->ExecuteSQL(strSQL);
	}

	// 様式⑰-1～⑱-20
	for(ii=0; ii<20; ii++){
		// 様式⑰-1～⑰-20
#ifdef _DEBUG
		strTempUtiwakeTbl.Format(_T("##temp_utiwake_tbl_%d"), ID_FORMNO_171+ii);
#else
		strTempUtiwakeTbl.Format(_T("#temp_utiwake_tbl_%d"), ID_FORMNO_171+ii);
#endif
		strSQL = _T("if object_id('tempdb..") + strTempUtiwakeTbl + _T("', 'u') is not null drop table ") + strTempUtiwakeTbl;

		// SQL実行
		m_pDB->ExecuteSQL(strSQL);

		// 様式⑱-1～⑱-20
#ifdef _DEBUG
		strTempUtiwakeTbl.Format(_T("##temp_utiwake_tbl_%d"), ID_FORMNO_181+ii);
#else
		strTempUtiwakeTbl.Format(_T("#temp_utiwake_tbl_%d"), ID_FORMNO_181+ii);
#endif
		strSQL = _T("if object_id('tempdb..") + strTempUtiwakeTbl + _T("', 'u') is not null drop table ") + strTempUtiwakeTbl;

		// SQL実行
		m_pDB->ExecuteSQL(strSQL);
	}
}
// midori 155906 add <--

// No.200601 add -->
//********************************************************************************
//	帳表コントロールの電子申告の文字数と比較する対象セル初期化
//	【引数】	nRow		…	行番号
//				nFgFunc		…	特殊行フラグ
//				bFgInput	…	自動計算フラグ
//				nMaxLen		…	比較する桁数（電子帳票の最大文字数）
//				sData		…	比較する文字列
//	【戻値】	int				ID_AUTOCHAR_SAFE	電子帳票の文字数を超えていない
//								ID_AUTOCHAR_OUT		電子帳票の文字数を超えている
//********************************************************************************
int CfrmUc000Common::CmnInitialControlFgStringCell(CICSDiag *pDiag, int nRow, int nCol, int nFgFunc,int nMaxLen, CString sData )
{
	int			sign=0;
	int			nDataPos=0;		//	データ位置

	// 電子申告の文字数チェックを行う
	sign = CmnStringLenChk(nMaxLen, sData);

	//	電子申告の文字数と比較する対象セルの属性設定を行う
	nDataPos = CmnGetControlIndex(nRow,nCol);
	CmnDiagSetAttrFgStringCell( pDiag, nDataPos, nFgFunc, sign );

	return(sign);
}

//********************************************************************************
// 最大文字数を超えていないかのチェックを行う
//********************************************************************************
int CfrmUc000Common::CmnStringLenChk(int nMaxLen,CString sData)
{
	CStringArray strArray;
	int			sign=0;
	int			ii=0;
	size_t		len=0;
	CString		cst=_T("");

	// 入力されたデータの文字数を確認
	cst.Empty();
	cst = sData;

	sign = ID_AUTOCHAR_SAFE;

	// 改行文字を省く
	m_clsFunc.StrDivision( cst, &strArray, 3, TRUE, TRUE );
	cst.Empty();
	for(ii = 0;ii < strArray.GetCount();ii++) {
		cst += strArray.GetAt(ii);
	}
// 修正No.157628 add -->
	ReplaceHankakuKana(cst);
// 修正No.157628 add <--
	len = _tcsclen(cst.GetBuffer());

	// 文字列が電子申告の文字数を超えているか
	if(len > nMaxLen)	{
		sign = ID_AUTOCHAR_OUT;
	}

	return(sign);
}

//********************************************************************************
//	項目タイトル(ヘッダ部)の再設定
//		IN		CICSDiag*		対象コントロール
//				int				様式番号
//		RET		なし
//********************************************************************************
void CfrmUc000Common::CmnHeadDiagSetTitle(CICSDiag *pDiag,int prow,CString ptitle,int pSgn)
{
	DIAGRAM_DATA		diadata;		// ICSDiag構造体
	DIAGRAM_ATTRIBUTE	udAttr;			// DIAGRAM_ATTRIBUTE構造体
	COLORREF			udColor1;		// RGB構造体(種類)

	// 電子申告と文字数を比較する項目のタイトルに、電子申告の文字数を追記する
	m_clsFunc.DiagSetString(pDiag, prow, ID_DIAGDATA_DISP, ptitle);

	// 電子申告の文字数を超えるデータが登録された項目がある場合は、
	// 文字色を電子申告の文字数を超えた色に変更
	udColor1 = RGB(0x00, 0x00, 0x00);	// 黒
	if(pSgn == 1)	udColor1 = COLOR_AUTOCHAR_OUT;

	// 列の文字色を変更する
	pDiag->GetAttribute(prow, ( LPUNKNOWN )&udAttr);			// 属性を取得
	memcpy(&udAttr.attr_fcolor, &udColor1, sizeof(COLORREF));	// 文字色を設定
	pDiag->SetAttribute(prow, ( LPUNKNOWN )&udAttr, TRUE);		// 属性を変更
}

//********************************************************************************
//	項目タイトル(ヘッダ部)の再設定（アライメント）
//		IN		CICSDiag*		対象コントロール
//				int				インデックス番号
//				int				アライメント設定 0:左寄せ、1:中央寄せ、2右寄せ、3:均等割
//		RET		なし
//********************************************************************************
void CfrmUc000Common::CmnHeadDiagChangeAlign(CICSDiag *pDiag, int pidx, int pSign)
{
	DIAGRAM_DATA		diadata;		// ICSDiag構造体
	DIAGRAM_ATTRIBUTE	udAttr;			// DIAGRAM_ATTRIBUTE構造体
	COLORREF			udColor1;		// RGB構造体(種類)

	// 列のアライメントを変更する
	pDiag->GetAttribute(pidx, ( LPUNKNOWN )&udAttr);			// 属性を取得
	udAttr.attr_align = pSign;
	pDiag->SetAttribute(pidx, ( LPUNKNOWN )&udAttr, TRUE);		// 属性を変更
}
// No.200601 add <--

// midori 157118 add -->
// ----------------------------------------------------------------------------------------
// DelCrLfString()
//	改行文字列削除
//	引数	szBuf	…	文字列
//
//	戻値	改行文字列削除文字列
// ----------------------------------------------------------------------------------------
CString CfrmUc000Common::DelCrLfString(CString szBuf)
{
	CString		szRet;		//	戻値
	CString		szCrLf;		//	改行文字列
	int			nPos = -1;	//	改行位置
	int			nLen = -1;	//	文字列長
	int			nCrLf = -1;	//	改行の文字列長

	//	初期化
	szRet.Empty();
	szCrLf.Empty();
	//	改行文字列作成
	szCrLf = '\r';
	szCrLf += '\n';

	//	改行文字列検索
	nPos = szBuf.Find(szCrLf);

	//	改行あり？
	if (nPos >= 0) {
		nLen = szBuf.GetLength();							//	文字列長取得
		nCrLf = szCrLf.GetLength();							//	改行の文字列長取得
		szRet = szBuf.Left(nPos);							//	改行文字までの文字列取得
		szRet += szBuf.Right(nLen - (nPos + nCrLf));	//	改行文字列以降の文字列取得
	}
	//	改行なし
	else {
		//	引数を戻値にセット
		szRet = szBuf;
	}

	//	戻値を返す
	return(szRet);
}
// midori 157118 add <--

// midori 156968_3 add -->
void CfrmUc000Common::InsertNull(void)
{
	int					sw = 0;
	int					cnt = 0;
	int					nBaseRow=0;
// 157153 add -->
	char				sgn = 0x00;
	ITEMSORT_INFO		uItemSort;
// 157153 add <--
	CString				strSQL=_T("");
// 157153 add -->
	CString				strTblSQL={_T("")};
	CString				strTblSQL2={_T("")};
// 157153 add <--
	CString				strTable=_T("");
	CString				strData=_T("");
// 157153 add -->
	CString				strField[2]={_T("")};
	CString				strSEL = _T("");
	CString				strWhere = _T("");
// 157153 add <--
	CRecordset			rs(m_pDB);
	CdbUc000Common*		rsData;
	CdbUcInfSub			mfcRec( m_pDB );	//	uc_inf_subテーブルクラス

	// 簡素化様式対応前は処理を行わない
	if (bG_Kanso == FALSE) {
		return;
	}

// 157153 del -->
	//sw = 0;
	//if(mfcRec.RequeryFormSeq(m_uInfo.intFormSeq) == DB_ERR_OK)	{
	//	if(mfcRec.m_OpOutNullLine == 1)	{
	//		sw = 1;
	//	}
	//}
	//mfcRec.Fin();
	//if(sw == 0)	{
	//	return;
	//}

	//rsData = virGetDataTableObject();
	//strTable = rsData->GetDefaultSQL();

	//// 行番号を振り直すための基準となる行数を取得
	//nBaseRow = CmnTblCountMaxRow();

	//// 現在フォーカスのある位置以降で最終行に小計行または中計がある場合のみ処理を行う
	//strSQL.Format(_T("select count(seq) from %s WHERE (NumPage > %d or (NumPage = %d and NumRow >= %d )) and numrow = %d and (fgfunc = %d or fgfunc = %d)"),
	//	strTable,m_uInfo.intCurPage,m_uInfo.intCurPage,m_uInfo.intCurRow,nBaseRow,ID_FGFUNC_SYOKEI,ID_FGFUNC_CHUKEI);
	//rs.Open(CRecordset::forwardOnly,strSQL);
	//cnt = 0;
	//if(!rs.IsEOF()) {
	//	rs.GetFieldValue((short)0,strData);
	//	cnt = _tstoi(strData);
	//}
	//rs.Close();
	//if(cnt == 0) {
	//	return;
	//}
// 157153 del <--
// 157153 add -->
	sgn = 0x00;
	if(mfcRec.RequeryFormSeq(m_uInfo.intFormSeq) == DB_ERR_OK)	{
		// 自動小計フラグ[FgSykei]がONの第１ソート項目が選択されている場合のみ処理を行う
		CmnUcLstItemSortGetData(mfcRec.m_Sort1, &uItemSort);
		strField[0] = uItemSort.strFieldSykei;
		strField[1] = uItemSort.strFieldSykei2;
		if(mfcRec.m_OpOutNullLine == 1 && uItemSort.bytFgSykei == 1)	{
			sgn |= 0x01;
		}
		if(mfcRec.m_OpOutSyokei == 0 && uItemSort.bytFgSykei == 1)	{
			sgn |= 0x02;
		}
	}
	mfcRec.Fin();
	if(sgn == 0x00) {
		return;
	}

	rsData = virGetDataTableObject();
	strTable = rsData->GetDefaultSQL();

	// 行番号を振り直すための基準となる行数を取得
	nBaseRow = CmnTblCountMaxRow();

	// 小計行の下にNULLを挿入するにチェックあり
	if((sgn & 0x01) == 0x01) { 
		// 現在フォーカスのある位置以降で最終行に小計行または中計がある場合のみ処理を行う
// 20210407 del -->
		//strSQL.Format(_T("select count(seq) from %s WHERE (NumPage > %d or (NumPage = %d and NumRow >= %d )) and numrow = %d and (fgfunc = %d or fgfunc = %d)"),
		//	strTable,m_uInfo.intCurPage,m_uInfo.intCurPage,m_uInfo.intCurRow,nBaseRow,ID_FGFUNC_SYOKEI,ID_FGFUNC_CHUKEI);
// 20210407 del <--
// 20210407 add -->
// 最終の【小計】を削除する場合も空行が挿入されていた
		strSQL.Format(_T("select count(seq) from %s WHERE (NumPage > %d or (NumPage = %d and NumRow > %d )) and numrow = %d and (fgfunc = %d or fgfunc = %d)"),
			strTable,m_uInfo.intCurPage,m_uInfo.intCurPage,m_uInfo.intCurRow,nBaseRow,ID_FGFUNC_SYOKEI,ID_FGFUNC_CHUKEI);
// 20210407 add <--
		rs.Open(CRecordset::forwardOnly,strSQL);
		cnt = 0;
		if(!rs.IsEOF()) {
			rs.GetFieldValue((short)0,strData);
			if(_tstoi(strData) > 0) {
				sw = 1;
			}
		}
		rs.Close();
	}
	if(sw == 0) {
		// 一行でも小計を挿入するにチェックが無い
		if((sgn & 0x02) == 0x02) {
			// select文
			strSEL = _T("select count(tbl1.seq) from");

			// 現在フォーカスのある位置以降で最終行かつデータ行のレコードのみ抽出した一時テーブルを作成
			strTblSQL.Format(_T("(select * from %s where (NumPage > %d or (NumPage = %d and NumRow >= %d )) and numrow = %d and fgshow = %d and (fgfunc = %d or fgfunc = %d or fgfunc = %d)) tbl1"),
				strTable,m_uInfo.intCurPage,m_uInfo.intCurPage,m_uInfo.intCurRow,nBaseRow,ID_FGSHOW_OFF,ID_FGFUNC_DATA,ID_FGFUNC_IKKATUMANUAL,ID_FGFUNC_IKKATUAUTO);
			// 現在フォーカスのある位置以降で頁の先頭行かつデータ行のレコードのみ抽出した一時テーブルを作成
			strTblSQL2.Format(_T("(select * from %s where (NumPage > %d or (NumPage = %d and NumRow >= %d )) and numrow = 1 and fgshow = %d and (fgfunc = %d or fgfunc = %d or fgfunc = %d)) tbl2"),
				strTable,m_uInfo.intCurPage,m_uInfo.intCurPage,m_uInfo.intCurRow,ID_FGSHOW_OFF,ID_FGFUNC_DATA,ID_FGFUNC_IKKATUMANUAL,ID_FGFUNC_IKKATUAUTO);

			// where文
			strWhere = _T("where tbl1.numpage + 1 = tbl2.numpage ");
			if(strField[1].IsEmpty() == FALSE && strField[0] != strField[1]) {
				strWhere += _T("and (");
				strWhere += _T("tbl1.") + strField[0] +  _T(" <> tbl2.") + strField[0] + _T(" ");
				strWhere += _T("or tbl1.") + strField[1] +  _T(" <> tbl2.") + strField[1] + _T(" ");
				strWhere += _T(") ");
			}
			else {
				strWhere += _T("and tbl1.") + strField[0] +  _T(" <> tbl2.") + strField[0] + _T(" ");
			}

			strSQL = strSEL + _T(" ") + strTblSQL + _T(",") + strTblSQL2 + _T(" ") + strWhere;

			rs.Open(CRecordset::forwardOnly,strSQL);
			cnt = 0;
			if(!rs.IsEOF()) {
				rs.GetFieldValue((short)0,strData);
				if(_tstoi(strData) > 0) {
					sw = 1;
				}
			}
			rs.Close();
		}
	}

	if(sw == 0) {
		return;
	}
// 157153 add <--

	// 小計行の下に空白行を挿入する
	CmnTblUpdatePageRowAllResetMain(5);
}
// midori 156968_3 add <--

// 修正No.157628 add -->
//********************************************************************************
//	渡されたカラム名の文字列に対して以下の置き換えをする
//　　1.NULL→""、2.CR+LF→""、3."ﾊﾟﾋﾟﾌﾟﾍﾟﾎﾟ"→"パピプペポ"
//		IN		CString		変換対象のカラム名
//		RET		CString		変換対象のカラム名置き換えるＳＱＬ文
//********************************************************************************
CString CfrmUc000Common::MakeReplaceSent( CString pStr )
{
	CString		cs1=_T("");
	CString		rcs=_T("");

	rcs  = _T("replace(replace(replace(replace(replace(");	// ガ行
	rcs += _T("replace(replace(replace(replace(replace(");	// ザ行
	rcs += _T("replace(replace(replace(replace(replace(");	// ダ行
	rcs += _T("replace(replace(replace(replace(replace(");	// バ行
	rcs += _T("replace(replace(replace(replace(replace(");	// パ行

// 修正No.157891 del -->
	//cs1.Format(_T("replace(isnull(%s,''),CHAR(13)+CHAR(10),'')"),pStr);
// 修正No.157891 del <--
// 修正No.157891 add -->
	cs1.Format(_T("replace(isnull(%s COLLATE Japanese_BIN,''),CHAR(13)+CHAR(10),'')"),pStr);
// 修正No.157891 add <--
	rcs += cs1;

	rcs += _T(",'ｶﾞ','ガ'),'ｷﾞ','ギ'),'ｸﾞ','グ'),'ｹﾞ','ゲ'),'ｺﾞ','ゴ')");	// ガ行
	rcs += _T(",'ｻﾞ','ザ'),'ｼﾞ','ジ'),'ｽﾞ','ズ'),'ｾﾞ','ゼ'),'ｿﾞ','ゾ')");	// ザ行
	rcs += _T(",'ﾀﾞ','ダ'),'ﾁﾞ','ヂ'),'ﾂﾞ','ヅ'),'ﾃﾞ','デ'),'ﾄﾞ','ド')");	// ダ行
	rcs += _T(",'ﾊﾞ','バ'),'ﾋﾞ','ビ'),'ﾌﾞ','ブ'),'ﾍﾞ','ベ'),'ﾎﾞ','ボ')");	// バ行
	rcs += _T(",'ﾊﾟ','パ'),'ﾋﾟ','ピ'),'ﾌﾟ','プ'),'ﾍﾟ','ペ'),'ﾎﾟ','ポ')");	// パ行

	return(rcs);
}

//********************************************************************************
//	渡されたカラム名の文字列に対して以下の置き換えをする
//	※２つの項目を組み合わせて文字数を確認する用
//　　1.NULL→""、2.CR+LF→""、3."ﾊﾟﾋﾟﾌﾟﾍﾟﾎﾟ"→"パピプペポ"
//		IN		CString		変換対象のカラム名
//		RET		CString		変換対象のカラム名置き換えるＳＱＬ文
//********************************************************************************
CString CfrmUc000Common::MakeReplaceSent2(CString pStr, CString pStr2)
{
	CString		cs1=_T("");
	CString		rcs=_T("");

	rcs  = _T("replace(replace(replace(replace(replace(");	// ガ行
	rcs += _T("replace(replace(replace(replace(replace(");	// ザ行
	rcs += _T("replace(replace(replace(replace(replace(");	// ダ行
	rcs += _T("replace(replace(replace(replace(replace(");	// バ行
	rcs += _T("replace(replace(replace(replace(replace(");	// パ行

// 修正No.157891 del -->
	//cs1.Format(_T("replace(isnull(%s,'') + isnull(%s,''),CHAR(13)+CHAR(10),'')"), pStr, pStr2);
// 修正No.157891 del <--
// 修正No.157891 del -->
	cs1.Format(_T("replace(isnull(%s COLLATE Japanese_BIN,'') + isnull(%s COLLATE Japanese_BIN,''),CHAR(13)+CHAR(10),'')"), pStr, pStr2);
// 修正No.157891 del <--
	rcs += cs1;

	rcs += _T(",'ｶﾞ','ガ'),'ｷﾞ','ギ'),'ｸﾞ','グ'),'ｹﾞ','ゲ'),'ｺﾞ','ゴ')");	// ガ行
	rcs += _T(",'ｻﾞ','ザ'),'ｼﾞ','ジ'),'ｽﾞ','ズ'),'ｾﾞ','ゼ'),'ｿﾞ','ゾ')");	// ザ行
	rcs += _T(",'ﾀﾞ','ダ'),'ﾁﾞ','ヂ'),'ﾂﾞ','ヅ'),'ﾃﾞ','デ'),'ﾄﾞ','ド')");	// ダ行
	rcs += _T(",'ﾊﾞ','バ'),'ﾋﾞ','ビ'),'ﾌﾞ','ブ'),'ﾍﾞ','ベ'),'ﾎﾞ','ボ')");	// バ行
	rcs += _T(",'ﾊﾟ','パ'),'ﾋﾟ','ピ'),'ﾌﾟ','プ'),'ﾍﾟ','ペ'),'ﾎﾟ','ポ')");	// パ行

	return(rcs);
}

//********************************************************************************
//	文字列中の半角濁音半濁音を全角濁音半濁音に置き換える
//	（半角の場合2文字でカウントされることを防ぐため）
//	例)"ﾊﾟﾋﾟﾌﾟﾍﾟﾎﾟ"→"パピプペポ"
//		IN		CString		変換対象の文字列
//		RET		CString		変換後文字列
//********************************************************************************
void CfrmUc000Common::ReplaceHankakuKana( CString& pStr )
{
	// 濁音
	pStr.Replace(_T("ｶﾞ"),_T("ガ"));
	pStr.Replace(_T("ｷﾞ"),_T("ギ"));
	pStr.Replace(_T("ｸﾞ"),_T("グ"));
	pStr.Replace(_T("ｹﾞ"),_T("ゲ"));
	pStr.Replace(_T("ｺﾞ"),_T("ゴ"));
	pStr.Replace(_T("ｻﾞ"),_T("ザ"));
	pStr.Replace(_T("ｼﾞ"),_T("ジ"));
	pStr.Replace(_T("ｽﾞ"),_T("ズ"));
	pStr.Replace(_T("ｾﾞ"),_T("ゼ"));
	pStr.Replace(_T("ｿﾞ"),_T("ゾ"));
	pStr.Replace(_T("ﾀﾞ"),_T("ダ"));
	pStr.Replace(_T("ﾁﾞ"),_T("ヂ"));
	pStr.Replace(_T("ﾂﾞ"),_T("ヅ"));
	pStr.Replace(_T("ﾃﾞ"),_T("デ"));
	pStr.Replace(_T("ﾄﾞ"),_T("ド"));
	pStr.Replace(_T("ﾊﾞ"),_T("バ"));
	pStr.Replace(_T("ﾋﾞ"),_T("ビ"));
	pStr.Replace(_T("ﾌﾞ"),_T("ブ"));
	pStr.Replace(_T("ﾍﾞ"),_T("ベ"));
	pStr.Replace(_T("ﾎﾞ"),_T("ボ"));
	// 半濁音
	pStr.Replace(_T("ﾊﾟ"),_T("パ"));
	pStr.Replace(_T("ﾋﾟ"),_T("ピ"));
	pStr.Replace(_T("ﾌﾟ"),_T("プ"));
	pStr.Replace(_T("ﾍﾟ"),_T("ペ"));
	pStr.Replace(_T("ﾎﾟ"),_T("ポ"));
}
// 修正No.157628 add <--

// 21/07/09 add -->
//********************************************************************************
//	日付入力項目にカーソルがセットされている時に退避させる
//	【引数】	int*	移動先のカラム値
//	【戻値】	int		1＝該当するので退避させる
//********************************************************************************
int CfrmUc000Common::CmnTblDataToFirstCol(int* pCol)
{
	int		ret = 0;
	int		col = -1;

	if(m_uInfo.intFormSeq == ID_FORMNO_021)	{
// インボイス登録番号追加対応_23/09/11 del -->
		//if(m_uInfo.intCurCol == ID_COL_021_SPDATE || m_uInfo.intCurCol == ID_COL_021_PMDATE)	{
		//	*pCol = ID_COL_021_KAMOKU;
		//	ret = 1;
		//}
// インボイス登録番号追加対応_23/09/11 del <--
// インボイス登録番号追加対応_23/09/11 add -->
		if(bG_InvNo == TRUE) {
			if(m_uInfo.intCurCol == ID_COL_021I_SPDATE || m_uInfo.intCurCol == ID_COL_021I_PMDATE) {
				*pCol = ID_COL_021I_KAMOKU;
				ret = 1;
			}
		}
		else {
			if(m_uInfo.intCurCol == ID_COL_021_SPDATE || m_uInfo.intCurCol == ID_COL_021_PMDATE) {
				*pCol = ID_COL_021_KAMOKU;
				ret = 1;
			}
		}
// インボイス登録番号追加対応_23/09/11 add <--
	}
	else if(m_uInfo.intFormSeq == ID_FORMNO_061)	{
		if(bG_Kanso == TRUE && m_uInfo.intCurCol == ID_COL_061K_SPDATE)	{
			*pCol = ID_COL_061K_KUBUN;
			ret = 1;
		}
		else if(bG_Kanso == FALSE && m_uInfo.intCurCol == ID_COL_061_SPDATE)	{
// 修正No.158301 del -->
			//*pCol = ID_COL_061K_KUBUN;
// 修正No.158301 del <--
// 修正No.158301 add -->
			*pCol = ID_COL_061_KUBUN;
// 修正No.158301 add <--
			ret = 1;
		}
	}
	else if(m_uInfo.intFormSeq == ID_FORMNO_071)	{
// インボイス登録番号追加対応_23/09/11 del -->
		//if(m_uInfo.intCurCol == ID_COL_071_SPDATE || m_uInfo.intCurCol == ID_COL_071_SPDATE2)	{
		//	*pCol = ID_COL_071_SYURUI;
		//	ret = 1;
		//}
// インボイス登録番号追加対応_23/09/11 del <--
// インボイス登録番号追加対応_23/09/11 add -->
		if(bG_InvNo == TRUE) {
			if(m_uInfo.intCurCol == ID_COL_071I_SPDATE || m_uInfo.intCurCol == ID_COL_071I_SPDATE2) {
				*pCol = ID_COL_071I_SYURUI;
				ret = 1;
			}
		}
		else	{
			if(m_uInfo.intCurCol == ID_COL_071_SPDATE || m_uInfo.intCurCol == ID_COL_071_SPDATE2) {
				*pCol = ID_COL_071_SYURUI;
				ret = 1;
			}
		}
// インボイス登録番号追加対応_23/09/11 add <--
	}
	else if(m_uInfo.intFormSeq == ID_FORMNO_081) {
// インボイス登録番号追加対応_23/09/11 del -->
		//if(m_uInfo.intCurCol == ID_COL_081_SPDATE || m_uInfo.intCurCol == ID_COL_081_PMDATE)	{
		//	*pCol = ID_COL_081_ADNAME1;
		//	ret = 1;
		//}
// インボイス登録番号追加対応_23/09/11 del <--
// インボイス登録番号追加対応_23/09/11 del -->
		if(bG_InvNo == TRUE) {
			if(m_uInfo.intCurCol == ID_COL_081I_SPDATE || m_uInfo.intCurCol == ID_COL_081I_PMDATE) {
				*pCol = ID_COL_081I_ADNAME1;
				ret = 1;
			}
		}
		else {
			if(m_uInfo.intCurCol == ID_COL_081_SPDATE || m_uInfo.intCurCol == ID_COL_081_PMDATE) {
				*pCol = ID_COL_081_ADNAME1;
				ret = 1;
			}
		}
// インボイス登録番号追加対応_23/09/11 del <--
	}
	else if(m_uInfo.intFormSeq == ID_FORMNO_102)	{
		if(m_uInfo.intCurCol == ID_COL_102_SPDATE || m_uInfo.intCurCol == ID_COL_102_VAL)	{
			*pCol = ID_COL_102_KNORDER;
			ret = 1;
		}
	}
	else if(m_uInfo.intFormSeq == ID_FORMNO_121)	{
// インボイス登録番号追加対応_23/09/11 del -->
		//if(m_uInfo.intCurCol == ID_COL_121_SPDATE)	{
		//	*pCol = ID_COL_121_KUBUN;
		//	ret = 1;
		//}
// インボイス登録番号追加対応_23/09/11 del <--
// インボイス登録番号追加対応_23/09/11 del -->
		if(bG_InvNo == TRUE && m_uInfo.intCurCol == ID_COL_121I_SPDATE) {
			*pCol = ID_COL_121I_KUBUN;
			ret = 1;
		}
		else if(bG_InvNo == FALSE && m_uInfo.intCurCol == ID_COL_121_SPDATE)	{
			*pCol = ID_COL_121_KUBUN;
			ret = 1;
		}
// インボイス登録番号追加対応_23/09/11 del <--
	}
	else if(m_uInfo.intFormSeq == ID_FORMNO_151)	{
// インボイス登録番号追加対応_23/09/11 del -->
		//if(m_uInfo.intCurCol == ID_COL_151_SPDATE || m_uInfo.intCurCol == ID_COL_151_SPDATE2)	{
		//	*pCol = ID_COL_151_KUBUN;
		//	ret = 1;
		//}
// インボイス登録番号追加対応_23/09/11 del <--
// インボイス登録番号追加対応_23/09/11 add -->
		if(bG_InvNo == TRUE) {
			if(m_uInfo.intCurCol == ID_COL_151I_SPDATE || m_uInfo.intCurCol == ID_COL_151I_SPDATE2) {
				*pCol = ID_COL_151I_KUBUN;
				ret = 1;
			}
		}
		else {
			if(m_uInfo.intCurCol == ID_COL_151_SPDATE || m_uInfo.intCurCol == ID_COL_151_SPDATE2) {
				*pCol = ID_COL_151_KUBUN;
				ret = 1;
			}
		}
// インボイス登録番号追加対応_23/09/11 add <--
	}
	else if(m_uInfo.intFormSeq == ID_FORMNO_152)	{
// インボイス登録番号追加対応_23/09/11 del -->
		//if(m_uInfo.intCurCol == ID_COL_152_SPDATE)	{
		//	*pCol = ID_COL_152_ADNAME1;
		//	ret = 1;
		//}
// インボイス登録番号追加対応_23/09/11 del <--
// インボイス登録番号追加対応_23/09/11 add -->
		if(bG_InvNo == TRUE && m_uInfo.intCurCol == ID_COL_152I_SPDATE) {
			*pCol = ID_COL_152I_ADNAME1;
			ret = 1;
		}
		else if(bG_InvNo == FALSE && m_uInfo.intCurCol == ID_COL_152_SPDATE)	{
			*pCol = ID_COL_152_ADNAME1;
			ret = 1;
		}
// インボイス登録番号追加対応_23/09/11 add <--
	}
	else if(m_uInfo.intFormSeq == ID_FORMNO_153)	{
// インボイス登録番号追加対応_23/09/11 del -->
		//if( m_uInfo.intCurCol == ID_COL_153_KYDATE || m_uInfo.intCurCol == ID_COL_153_KYDATE2 ||
		//	m_uInfo.intCurCol == ID_COL_153_SPDATE || m_uInfo.intCurCol == ID_COL_153_SPDATE2 )	{
		//	*pCol = ID_COL_153_HDNAME;
		//	ret = 1;
		//}
// インボイス登録番号追加対応_23/09/11 del <--
// インボイス登録番号追加対応_23/09/11 del -->
		if(bG_InvNo == TRUE) {
			if( m_uInfo.intCurCol == ID_COL_153I_KYDATE || m_uInfo.intCurCol == ID_COL_153I_KYDATE2 ||
				m_uInfo.intCurCol == ID_COL_153I_SPDATE || m_uInfo.intCurCol == ID_COL_153I_SPDATE2)	{
				*pCol = ID_COL_153I_HDNAME;
				ret = 1;
			}
		}
		else {
			if( m_uInfo.intCurCol == ID_COL_153_KYDATE || m_uInfo.intCurCol == ID_COL_153_KYDATE2 ||
				m_uInfo.intCurCol == ID_COL_153_SPDATE || m_uInfo.intCurCol == ID_COL_153_SPDATE2) {
				*pCol = ID_COL_153_HDNAME;
				ret = 1;
			}
		}
// インボイス登録番号追加対応_23/09/11 del <--
	}

	return(ret);
}
// 21/07/09 add <--

// 230227 add -->
//********************************************************************************
//	免税事業者からの課税仕入の場合、経過措置を考慮した金額を返す
//********************************************************************************
//	【引数】	CDBZmSub*		財務クラス
//	【戻値】	CString			本体金額
//********************************************************************************
CString CfrmUc000Common::InvoiceCalc( CDBZmSub* pZmSub )
{
	CString		rv=_T("");
	UCHAR		zei[6]={0};
	UCHAR		val[6]={0};

	CmnChangeFieldValData((char*)val,pZmSub->data->val.Trim());
	CmnChangeFieldValData((char*)zei,pZmSub->data->zei.Trim());
	// 課税取引のみ対象
	if((pZmSub->data->dsign[2] & 0x0f) == 0x01)	{
		int		def=l_retn();
		l_defn(0x16);
		// 科目が対象外
		if((pZmSub->knrec->knsgn[2] & 0x0f) == 0x00)	{
			// 外税
			if(!(pZmSub->data->dsign[1] & 0x10))	{
				// 税を加算する
				l_add(val,val,zei);
			}
		}
		else	{
			// (条件)
			// ・インボイスコンバージョン済み
			// ・2023年10月1日以降
			// ・免税事業者からの課税仕入
			if((m_Invoice == TRUE) && (pZmSub->data->ddate >= 20231001) && (pZmSub->data->dsign[6] & 0x20))	{
				UCHAR			mzei[6]={0};
				CSyzInvoice		*psi;
				// -----------------------------------------------------------------------
				// 免税事業者の税額を取得
				// -----------------------------------------------------------------------
				// 令和５年１０月１日～令和　８年９月３０日まで		仕入税額相当額の80％
				// 令和８年１０月１日～令和１１年９月３０日まで		仕入税額相当額の50％
				// -----------------------------------------------------------------------
				psi = new CSyzInvoice(pZmSub);
				psi->CalcMenzeiTax(mzei,zei,pZmSub->data->ddate,&pZmSub->data->dsign);
				delete psi;
				// -----------------------------------------------------------------------
				// 税額を置き換える
				// -----------------------------------------------------------------------
				// 外税
				if(!(pZmSub->data->dsign[1] & 0x10))	{
					// val=100,000 → 102,000
					// zei= 10,000 →   8,000
					UCHAR	sagaku[6]={0};
					l_sub(sagaku,zei,mzei);
					l_add(val,val,sagaku);
					memcpy(zei,mzei,6);
				}
				// 内税
				else	{
					// val=110,000 → 110,000
					// zei= 10,000 →   8,000
					memcpy(zei,mzei,6);
				}
			}
			// 税込み
			if((m_S_sgn1 & 0xc0) == 0x00)	{
				// 外税なら税を加算
				if(!(pZmSub->data->dsign[1] & 0x10))	{
					l_add(val,val,zei);
				}
			}
			// 税抜き・一括税抜き
			else	{
// インボイス_23/08/08 cor -->
				//// 内税なら税を減算
				//if(pZmSub->data->dsign[1] & 0x10)	{
				//	l_sub(val,val,zei);
				//}
// ---------------------------
				// 日付が課税期間開始日より前の場合
				if(pZmSub->data->ddate < pZmSub->zvol->to_kz_ymd)	{
					// 免税として税込みの金額を連動する
					// 外税なら税を加算
					if(!(pZmSub->data->dsign[1] & 0x10))	{
						l_add(val,val,zei);
					}
				}
				else	{
					// 内税なら税を減算
					if(pZmSub->data->dsign[1] & 0x10)	{
						l_sub(val,val,zei);
					}
				}
// インボイス_23/08/08 cor <--
			}
		}
		l_defn(def);
	}

	CmnGetNotCommaValData(&rv,(char*)val);
	return(rv);
}
// 230227 add <--

// 改良No.22-0811 add -->
// 財務連動メニューの切り替えテスト
void CfrmUc000Common::CmnMenuEnableSwitch(void)
{
	CString		strGrNum = m_clsFunc.GetGrNo();	// グループNOを取得する
	BOOL		bZaimu = FALSE;

	// 財務db以外はリターン
	if(strGrNum.CompareNoCase(_T("1B")) == 0)	{
		if(((CUCHIWAKEApp*)AfxGetApp())->m_swUtwTandoku == 0)	{
			bZaimu = TRUE;
		}
	}
	if(bZaimu != TRUE)	{
		return;
	}

	// 財務連動を行える様式のみ、メニュー「財務連動(&Z)」をEnableにする
	CMainFrame*		pFrame;
	ICSToolBar*		pTool;
	CXTPControl*	pCtrl;

	pFrame = (CMainFrame*)AfxGetMainWnd();
	if(pFrame != NULL)	{
		pTool = pFrame->GetToolBar(0);			// 「0:メニュー」ツールバーを取得
		if(pTool != NULL)	{
			pCtrl = pTool->GetControl(6);		// 「6:財務連動」メニューを取得
			if(pCtrl != NULL)	{
				if(m_uInfo.bRenFlg && m_DataKakutei == FALSE)	{
					pCtrl->SetEnabled(TRUE);
				}
				else	{
					pCtrl->SetEnabled(FALSE);
				}
			}
		}
	}
}
// 改良No.22-0811 add <--

// インボイス登録番号追加対応_23/11/06 add -->
//*************************************************************************************
// ※未使用
//	インボイス登録番号追加対応
//	指定された登録番号（適格請求書事業者番号）または法人番号より特定の情報を取得する
//*************************************************************************************
//	【引数】	CDatabase* pDB		データベース
//				int inGrSeq			グループ番号
//				CString pInvno		登録番号or法人番号
//				CString* rName		(返送値)名称
//				CString* rAdd1		(返送値)所在地１
//				CString* rAdd2		(返送値)所在地２
//				CString* rErrMsg	(返送値)エラーメッセージ
//				int pSw				1=参照ダイアログを見ない
//	【返送値】	int					0:登録番号で該当、-1:エラー
//*************************************************************************************
int CfrmUc000Common::GetBillingProviderHojinInfo(CDatabase* pDB, int inGrSeq, CString pInvno, CString* rName, CString* rAdd1, CString* rAdd2, CString* rErrMsg, int pSw/*=0*/)
{
	int					rt = 2;
	int					ret=0;
	CString				invono;
	CString				sErr;
	CString				cst;
	CString				str1,str2,str3;
	EBG_BASE_INF		ebgData;
	CdbUcLstAddress		rsAdd(pDB);

	*rErrMsg = _T("");

	// F9:参照に登録されているかチェック。登録済みの場合は、uc_lst_addressから名称を取得する
	if(pSw == 0)	{
		for(int ii=0; ii<2; ii++)	{
			if(ii == 0)	invono.Format(_T("T%s"), pInvno);
			else		invono.Format(_T(" %s"), pInvno);

			ret = rsAdd.GetAddressInf_Inv(inGrSeq, invono);
			if(ret == 0)	{			//	正常終了（データ取得済み）
				*rName = rsAdd.m_AdName1 + rsAdd.m_AdName2;		// 名称
				*rAdd1 = rsAdd.m_AdAdd1;						// 所在地１
				*rAdd2 = rsAdd.m_AdAdd2;						// 所在地２

				rt = ii;
				break;
			}
			else if(ret < 0)	{
				rt = -1;
				break;
			}
		}
		if(rt != 2) {
			if(rt < 0)	{
				//cst  = _T("登録番号（法人番号）に誤りがあります。\r\n誤った番号は保存されません。\r\n\r\n");	// 修正No.168378 del
				cst  = _T("登録番号（法人番号）に誤りがあります。\r\n");										// 修正No.168378 add
				cst += _T("登録番号（法人番号）を再入力してください。");
				*rErrMsg = cst;
			}
			return(rt);
		}
	}

	// Web-APIを使用して情報の取得
	invono.Format(_T("T%s"), pInvno);
	memset(&ebgData, 0, sizeof(EBG_BASE_INF));
	// 指定された適格請求書事業者番号より特定の情報を取得する
	ret = _GetEgBillingProviderInfo(0, invono, ebgData, sErr);
	if(ret == 0) {			//	正常終了（データ取得済み）
		*rName = ebgData.name;			// 名称
		cst = ebgData.address;	// 所在地
		cst.TrimRight();
		if(cst.GetLength() > 40)	{
			str2 = m_clsFunc.GetSpritString(cst, &str1, 40);
			if(str2.GetLength() > 40)	{
				cst = str2;
				str3 = m_clsFunc.GetSpritString(cst, &str2, 40);
			}
		}
		else {
			str1 = cst;
		}
		*rAdd1 = str1;
		*rAdd2 = str2;

		rt = 0;
	}
	else if(ret == 1)	{	//	正常終了（該当データなし）
		*rName = _T("");				// 名称
		*rAdd1 = _T("");				// 所在地１
		*rAdd2 = _T("");				// 所在地２

		rt = 1;
	}
	else	{
		if(ret == -3)		cst = _T("登録番号（法人番号）の文字数に誤りがあります。\r\n登録番号（法人番号）を再入力してください。");
		else if(ret == -4)	cst = _T("インターネットに接続されていません。\r\nインターネットに接続された環境で検索してください。");
		else	{
			//cst  = _T("登録番号（法人番号）に誤りがあります。\r\n誤った番号は保存されません。\r\n\r\n");	// 修正No.168378 del
			cst  = _T("登録番号（法人番号）に誤りがあります。\r\n");										// 修正No.168378 add
			cst += _T("登録番号（法人番号）を再入力してください。");
		}
		*rErrMsg = cst;

		rt = -1;
	}

	return(rt);
}

// --------------------------------------------------------------------------------------------------------
//	登録番号を分割して返す（Tor空白 と 番号）
// --------------------------------------------------------------------------------------------------------
void CfrmUc000Common::InvoNoSplit(CString pInvno, CString* rtData1, CString* rtData2)
{
	CString		cst;
	CString		cc;

	cst = pInvno;
	*rtData1 = _T("");
	*rtData2 = _T("");

	cc = cst.Left(1);
	if(strcmp(cc, _T("T")) == 0 || strcmp(cc, _T(" ")) == 0)	{
		*rtData1 = cc;
		*rtData2 = cst.Mid(1, 13);
	}
	else	{
		*rtData2 = cst;
	}
}

// --------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------
void CALLBACK callback(UINT n, void* p, CWnd* pwnd)
{
	CfrmUc000Common* pBaseDlg = (CfrmUc000Common*)pwnd;

	if(n == IDOK)	{
		ZmselDBData* zd = (ZmselDBData*)p;
		
		//TRACE( "*** callback IDOK mode = %d, errflg %d\n", zd->selmode, zd->errflg );
		if(zd->selmode == ZSEL_INVNO)	{
			if(strlen(zd->sel_name) > 0)	{
				CFrameWnd*			pFrame	= (CFrameWnd*)AfxGetMainWnd();
				CMDIChildWnd*		pChild	= (CMDIChildWnd *)pFrame->GetActiveFrame();
				CfrmUc000Common*	frmView	= (CfrmUc000Common*)pChild->GetActiveView();
				CString				strInvoNo, strHjnName, strHjnAdd1, strHjnAdd2, strHjnKana;

				strInvoNo.Format(_T("%s"), zd->sel_name);	// 登録番号
				//strHjnName.Format(_T("%s"), zd->tkstr);	// 摘要名称
				strHjnName.Empty();							// ※名称は返さない
				strHjnAdd1.Empty();
				strHjnAdd2.Empty();
				strHjnKana.Empty();
				// 取得したデータを各様式の画面に表示
				frmView->virDiagSetHjnData(1, strInvoNo, strHjnName, strHjnAdd1, strHjnAdd2, strHjnKana);
			}
		}
	}
}

// --------------------------------------------------------------------------------------------------------
//	ZmSel（登録番号一覧）の初期化
// --------------------------------------------------------------------------------------------------------
// 返送値
//		0			:			正常終了
//		-1			:			エラー
// --------------------------------------------------------------------------------------------------------
void CfrmUc000Common::InitZmSel(void)
{
	int			opsw[8];
	CDBZmSub*	pZmSub = NULL;

	// インボイス登録番号を使用しない場合は無視
	if(bG_InvNo == FALSE)	return;

	for(int ii=0; ii<8; ii++)	{
		opsw[ii] = 0;
	}

	pZmSub = ((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub;
	// 必要なテーブルオープン（Zvol）
	if(pZmSub->zvol == NULL) {
		pZmSub->VolumeOpen();	opsw[0] = 1;
	}

	pZmSel = new CZmselDB();

	if(pZmSel != NULL) {
		pZmSel->SetDialogResize(FALSE);	//科目選択を小さく表示

		// 必要なテーブルオープン（Zvol以外）
		if(pZmSub->pKnrec == NULL) {
			pZmSub->KamokuMeisyoRead();		opsw[1] = 1;
		}
		if(pZmSub->knrec == NULL) {
			pZmSub->KamokuMeisyoOpen();		opsw[2] = 1;
		}
		if(pZmSub->ezrec == NULL) {
			pZmSub->EdabanOpen();			opsw[3] = 1;
		}
		if(pZmSub->bmname == NULL) {
			pZmSub->BmnameOpen();			opsw[4] = 1;
		}
		if(pZmSub->rtrec == NULL) {
			pZmSub->RensoTekiyoOpen();		opsw[5] = 1;
		}
		if(pZmSub->tkrec == NULL) {
			pZmSub->KanaTekiyoOpen();		opsw[6] = 1;
		}
		if(pZmSub->strec == NULL) {
			pZmSub->SiwakeTaioTekiyoOpen();	opsw[7] = 1;
		}

		// クラスモジュールの初期設定
		pZmSel->SetPara(pZmSub, this, NULL, callback);

		// 一緒に初期化
		pSyzInvoice.SetDBZmSub(pZmSub);

		// テーブルクローズ
		if(opsw[1] == 1)	pZmSub->KamokuMeisyoFree();			opsw[1] = 0;
		if(opsw[2] == 1)	pZmSub->KamokuMeisyoClose();		opsw[2] = 0;
		if(opsw[3] == 1)	pZmSub->EdabanClose();				opsw[3] = 0;
		if(opsw[4] == 1)	pZmSub->BmnameClose();				opsw[4] = 0;
		if(opsw[5] == 1)	pZmSub->RensoTekiyoClose();			opsw[5] = 0;
		if(opsw[6] == 1)	pZmSub->KanaTekiyoClose();			opsw[6] = 0;
		if(opsw[7] == 1)	pZmSub->SiwakeTaioTekiyoClose();	opsw[7] = 0;
	}

	if(opsw[0] == 1)	pZmSub->VolumeClose();		opsw[0] = 0;
}

// 修正No.168364 add -->
// --------------------------------------------------------------------------------------------------------
//	登録番号一覧を更新
// --------------------------------------------------------------------------------------------------------
// 返送値
//		0			:			正常終了
//		-1			:			エラー
// --------------------------------------------------------------------------------------------------------
void CfrmUc000Common::UpdateZmSel(void)
{
	int			opsw[8];
	CDBZmSub*	pZmSub = NULL;

	// インボイス登録番号を使用しない場合は無視
	if(bG_InvNo == FALSE)	return;

	if(pZmSel == NULL)		return;

	for(int ii=0; ii<8; ii++)	{
		opsw[ii] = 0;
	}

	pZmSub = ((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub;
	// 必要なテーブルオープン
	if(pZmSub->zvol == NULL) {
		pZmSub->VolumeOpen();			opsw[0] = 1;
	}
	if(pZmSub->pKnrec == NULL) {
		pZmSub->KamokuMeisyoRead();		opsw[1] = 1;
	}
	if(pZmSub->knrec == NULL) {
		pZmSub->KamokuMeisyoOpen();		opsw[2] = 1;
	}
	if(pZmSub->ezrec == NULL) {
		pZmSub->EdabanOpen();			opsw[3] = 1;
	}
	if(pZmSub->bmname == NULL) {
		pZmSub->BmnameOpen();			opsw[4] = 1;
	}
	if(pZmSub->rtrec == NULL) {
		pZmSub->RensoTekiyoOpen();		opsw[5] = 1;
	}
	if(pZmSub->tkrec == NULL) {
		pZmSub->KanaTekiyoOpen();		opsw[6] = 1;
	}
	if(pZmSub->strec == NULL) {
		pZmSub->SiwakeTaioTekiyoOpen();	opsw[7] = 1;
	}

	// 登録番号ダイアログを更新
	//if(pZmSel && pZmSel->IsDisplay() != FALSE)	{	// 修正No.168477 del
	if(pZmSel)	{										// 修正No.168477 add
		pZmSel->ZmselUpdate();
	}

	// テーブルクローズ
	if(opsw[7] == 1)	pZmSub->SiwakeTaioTekiyoClose();	opsw[7] = 0;
	if(opsw[6] == 1)	pZmSub->KanaTekiyoClose();			opsw[6] = 0;
	if(opsw[5] == 1)	pZmSub->RensoTekiyoClose();			opsw[5] = 0;
	if(opsw[4] == 1)	pZmSub->BmnameClose();				opsw[4] = 0;
	if(opsw[3] == 1)	pZmSub->EdabanClose();				opsw[3] = 0;
	if(opsw[2] == 1)	pZmSub->KamokuMeisyoClose();		opsw[2] = 0;
	if(opsw[1] == 1)	pZmSub->KamokuMeisyoFree();			opsw[1] = 0;
	if(opsw[0] == 1)	pZmSub->VolumeClose();				opsw[0] = 0;
}
// 修正No.168364 add <--

// --------------------------------------------------------------------------------------------------------
//	ZmSel（登録番号一覧）を表示する
// --------------------------------------------------------------------------------------------------------
LRESULT CfrmUc000Common::OnShowZmselDialog(WPARAM wParam, LPARAM lParam)
{
	int		opsw[8];

	// インボイス登録番号を使用しない場合は無視
	if(bG_InvNo == FALSE)	return(0);

	for(int ii=0; ii<8; ii++)	{
		opsw[ii] = 0;
	}

	CDBZmSub*	pZmSub = NULL;
	pZmSub = ((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub;

	// 必要なテーブルオープン（Zvol）
	if(pZmSub->zvol == NULL) {
		pZmSub->VolumeOpen();			opsw[0] = 1;
	}

	if(pZmSel && (int)wParam != 0)	{
		int index = (int)lParam;

		// 登録番号一覧が表示されていたら消す
		if(pZmSel && pZmSel->IsDisplay() != FALSE)	{
			pZmSel->DialogOFF();
		}

		// 必要なテーブルオープン（Zvol以外）
		if(pZmSub->pKnrec == NULL) {
			pZmSub->KamokuMeisyoRead();		opsw[1] = 1;
		}
		if(pZmSub->knrec == NULL) {
			pZmSub->KamokuMeisyoOpen();		opsw[2] = 1;
		}
		if(pZmSub->ezrec == NULL) {
			pZmSub->EdabanOpen();			opsw[3] = 1;
		}
		if(pZmSub->bmname == NULL) {
			pZmSub->BmnameOpen();			opsw[4] = 1;
		}
		if(pZmSub->rtrec == NULL) {
			pZmSub->RensoTekiyoOpen();		opsw[5] = 1;
		}
		if(pZmSub->tkrec == NULL) {
			pZmSub->KanaTekiyoOpen();		opsw[6] = 1;
		}
		if(pZmSub->strec == NULL) {
			pZmSub->SiwakeTaioTekiyoOpen();	opsw[7] = 1;
		}

		// 非連動dbは科目設定を開けないため、摘要に登録番号を入力することはないので登録番号選択は表示しない。
		// また、会社登録で「インボイス登録番号の表示」を「表示しない」に設定している場合も出さない。
		if(((CUCHIWAKEApp*)AfxGetApp())->m_swUtwTandoku == 0 && !pZmSub->zvol->s_sgn8) {
			// 位置調整
// 修正No.168481,168502,168517 del -->
			//CRect		diagrect,zmselrect;
			//RECT		fr, dr, hr, hr2, hr3;
			//CICSDiag*	diag = m_pDiag;
			//int			x1, y1;
			//int			kx=::GetSystemMetrics(SM_CXSCREEN);	// Ｘ軸解像度
			//int			ky=::GetSystemMetrics(SM_CYSCREEN);	// Ｙ軸解像度

			//diag->GetRect(index,(LPUNKNOWN)&diagrect);
			//ScreenToClient(&diagrect);
			//pZmSel->GetScreenRect(zmselrect);

			//// 上下
			//if((diagrect.top - zmselrect.Height() - 10) > 0)	{
			//	zmselrect.top = diagrect.top - 10;
			//}
			//else	{
			//	zmselrect.top = diagrect.bottom + zmselrect.Height() + 10;
			//}

			//// 左右
			//LONG	wid = zmselrect.right - zmselrect.left;
			//LONG	hei = zmselrect.bottom - zmselrect.top;

			//zmselrect.left = diagrect.left;
			//zmselrect.right = zmselrect.left + wid;

			//// Diagのサイズを取得
			//diag->GetWindowRect(&hr);
			//ScreenToClient(&hr);

			//if(zmselrect.left < hr.left)	{		// ダイアログの左が切れる
			//	zmselrect.left = hr.left;
			//}
			//if(zmselrect.right > hr.right)	{		// ダイアログの右が切れる
			//	zmselrect.left = hr.right - wid;
			//}
			//if(zmselrect.top < hr.top)	{			// ダイアログの上が切れる
			//	zmselrect.top = hr.top;
			//}
			//if(zmselrect.bottom > hr.bottom)	{	// ダイアログの下が切れる
			//	zmselrect.top = hr.bottom - hei;
			//}
// 修正No.168481,168502,168517 del <--
// 修正No.168481,168502,168517 add -->
			CRect		diagrect,zmselrect;
			RECT		fr, hr, hr2, hr3;
			CICSDiag*	diag = m_pDiag;

			diag->GetRect(index,(LPUNKNOWN)&diagrect);
			ScreenToClient(&diagrect);
			pZmSel->GetScreenRect(zmselrect);

			// 幅と高さ
// 修正No.168547 del -->
			//int		wid = zmselrect.Width();
			//int		hei = zmselrect.Height();
// 修正No.168547 del <--
// 修正No.168547 add -->
			if(ZselWid != zmselrect.Width() && zmselrect.Width() > 0)	ZselWid = zmselrect.Width();
			if(ZselHei != zmselrect.Height() && zmselrect.Height() > 0)	ZselHei = zmselrect.Height();
// 修正No.168547 add <--

			// 上下の位置
			zmselrect.top = diagrect.bottom + 5;
			//zmselrect.bottom = zmselrect.top + hei;		// 修正No.168547 del
			zmselrect.bottom = zmselrect.top + ZselHei;		// 修正No.168547 add
			// 左右の位置
			zmselrect.left = diagrect.left;
			//zmselrect.right = zmselrect.left + wid;		// 修正No.168547 del
			zmselrect.right = zmselrect.left + ZselWid;		// 修正No.168547 add

			// モニタのサイズを取得
			GetWindowRect(&hr);
			ScreenToClient(&hr);
			// ヘッダ部のサイズを取得
			m_pHeadDiag->GetWindowRect(&hr2);
			ScreenToClient(&hr2);
			// 計行部のサイズを取得
			m_pDiagKei2->GetWindowRect(&hr3);
			ScreenToClient(&hr3);

			fr.left		= hr.left + 5;
			fr.right	= hr.right - 5;
			fr.top		= hr2.top + 5;
			//fr.bottom	= hr3.bottom - 5;	// 修正No.168517 del
			fr.bottom	= hr.bottom - 5;	// 修正No.168517 add

			if(zmselrect.left < fr.left) {		// ダイアログの左が切れる
				zmselrect.left = fr.left;
				//zmselrect.right = zmselrect.left + wid;		// 修正No.168547 del
				zmselrect.right = zmselrect.left + ZselWid;		// 修正No.168547 add
			}
			if(zmselrect.right > fr.right) {	// ダイアログの右が切れる
				zmselrect.right = fr.right;
				//zmselrect.left = zmselrect.right - wid;		// 修正No.168547 del
				zmselrect.left = zmselrect.right - ZselWid;		// 修正No.168547 add
			}
			if(zmselrect.top < fr.top) {		// ダイアログの上が切れる
				zmselrect.top = diagrect.bottom + 5;
				//zmselrect.bottom = zmselrect.top + hei;		// 修正No.168547 del
				zmselrect.bottom = zmselrect.top + ZselHei;		// 修正No.168547 add
			}
			if(zmselrect.bottom > fr.bottom) {	// ダイアログの下が切れる
				zmselrect.bottom = diagrect.top - 5;
				//zmselrect.top = zmselrect.bottom - hei;		// 修正No.168547 del
				zmselrect.top = zmselrect.bottom - ZselHei;		// 修正No.168547 add
			}
// 修正No.168481,168502,168517 add <--

			pZmSel->DispCloseButton(1);

			try	{
				//pZmSel->DialogON(ZSEL_INVNO, ZSEL_TOPLEFT, &zmselrect);	// 修正No.168481,168502 del
				pZmSel->DialogON(ZSEL_INVNO, ZSEL_CLIRECT, &zmselrect);		// 修正No.168481,168502 add
			}
			catch(CDBException *e)	{
				ICSMessageBox(e->m_strError,MB_ICONSTOP);
				e->Delete();
			}
		}

		// テーブルクローズ（Zvol以外）
		if(opsw[1] == 1)	pZmSub->KamokuMeisyoFree();			opsw[1] = 0;
		if(opsw[2] == 1)	pZmSub->KamokuMeisyoClose();		opsw[2] = 0;
		if(opsw[3] == 1)	pZmSub->EdabanClose();				opsw[3] = 0;
		if(opsw[4] == 1)	pZmSub->BmnameClose();				opsw[4] = 0;
		if(opsw[5] == 1)	pZmSub->RensoTekiyoClose();			opsw[5] = 0;
		if(opsw[6] == 1)	pZmSub->KanaTekiyoClose();			opsw[6] = 0;
		if(opsw[7] == 1)	pZmSub->SiwakeTaioTekiyoClose();	opsw[7] = 0;
	}
	else	{
		if(pZmSel && pZmSel->IsDisplay() != FALSE)	{
			pZmSel->DialogOFF();
		}
	}

	// テーブルクローズ（Zvol）
	if(opsw[0] == 1)	pZmSub->VolumeClose();				opsw[0] = 0;

	return(0);
}

// --------------------------------------------------------------------------------------------------------
//	今のカラムが「登録番号」のカラムかどうかチェック
// --------------------------------------------------------------------------------------------------------
BOOL CfrmUc000Common::InvNoClmChk(void)
{
	BOOL	bRet = FALSE;

	if(bG_InvNo == FALSE)	return(FALSE);

	//if((m_uInfo.intFormSeq == ID_FORMNO_021 && (m_uInfo.intCurCol == ID_COL_021I_INVONOT || m_uInfo.intCurCol == ID_COL_021I_INVONO)) ||
	//   (m_uInfo.intFormSeq == ID_FORMNO_031 && (m_uInfo.intCurCol == ID_COL_031I_INVONOT || m_uInfo.intCurCol == ID_COL_031I_INVONO)) ||
	//   (m_uInfo.intFormSeq == ID_FORMNO_041 && (m_uInfo.intCurCol == ID_COL_041I_INVONOT || m_uInfo.intCurCol == ID_COL_041I_INVONO)) ||
	//   (m_uInfo.intFormSeq == ID_FORMNO_042 && (m_uInfo.intCurCol == ID_COL_042I_INVONOT || m_uInfo.intCurCol == ID_COL_042I_INVONO)) ||
	//   (m_uInfo.intFormSeq == ID_FORMNO_071 && (m_uInfo.intCurCol == ID_COL_071I_INVONOT || m_uInfo.intCurCol == ID_COL_071I_INVONO)) ||
	//   (m_uInfo.intFormSeq == ID_FORMNO_081 && (m_uInfo.intCurCol == ID_COL_081I_INVONOT || m_uInfo.intCurCol == ID_COL_081I_INVONO)) ||
	//   (m_uInfo.intFormSeq == ID_FORMNO_091 && (m_uInfo.intCurCol == ID_COL_091I_INVONOT || m_uInfo.intCurCol == ID_COL_091I_INVONO)) ||
	//   (m_uInfo.intFormSeq == ID_FORMNO_101 && (m_uInfo.intCurCol == ID_COL_101I_INVONOT || m_uInfo.intCurCol == ID_COL_101I_INVONO)) ||
	//   (m_uInfo.intFormSeq == ID_FORMNO_121 && (m_uInfo.intCurCol == ID_COL_121I_INVONOT || m_uInfo.intCurCol == ID_COL_121I_INVONO)) ||
	//   (m_uInfo.intFormSeq == ID_FORMNO_151 && (m_uInfo.intCurCol == ID_COL_151I_INVONOT || m_uInfo.intCurCol == ID_COL_151I_INVONO)) ||
	//   (m_uInfo.intFormSeq == ID_FORMNO_152 && (m_uInfo.intCurCol == ID_COL_152I_INVONOT || m_uInfo.intCurCol == ID_COL_152I_INVONO)) ||
	//   (m_uInfo.intFormSeq == ID_FORMNO_153 && (m_uInfo.intCurCol == ID_COL_153I_INVONOT || m_uInfo.intCurCol == ID_COL_153I_INVONO)) ||
	//   (m_uInfo.intFormSeq == ID_FORMNO_161 && (m_uInfo.intCurCol == ID_COL_161I_INVONOT || m_uInfo.intCurCol == ID_COL_161I_INVONO)) ||
	//   (m_uInfo.intFormSeq == ID_FORMNO_162 && (m_uInfo.intCurCol == ID_COL_162I_INVONOT || m_uInfo.intCurCol == ID_COL_162I_INVONO)) ||
	//   (m_uInfo.intFormSeq >= ID_FORMNO_171 && m_uInfo.intFormSeq <= ID_FORMNO_1720 &&
	//		(m_uInfo.intCurCol == ID_COL_171I_INVONOT || m_uInfo.intCurCol == ID_COL_171I_INVONO)) )
	if((m_uInfo.intFormSeq == ID_FORMNO_021 && m_uInfo.intCurCol == ID_COL_021I_INVONO) ||
	   (m_uInfo.intFormSeq == ID_FORMNO_031 && m_uInfo.intCurCol == ID_COL_031I_INVONO) ||
	   (m_uInfo.intFormSeq == ID_FORMNO_041 && m_uInfo.intCurCol == ID_COL_041I_INVONO) ||
	   (m_uInfo.intFormSeq == ID_FORMNO_042 && m_uInfo.intCurCol == ID_COL_042I_INVONO) ||
	   (m_uInfo.intFormSeq == ID_FORMNO_071 && m_uInfo.intCurCol == ID_COL_071I_INVONO) ||
	   (m_uInfo.intFormSeq == ID_FORMNO_081 && m_uInfo.intCurCol == ID_COL_081I_INVONO) ||
	   (m_uInfo.intFormSeq == ID_FORMNO_091 && m_uInfo.intCurCol == ID_COL_091I_INVONO) ||
	   (m_uInfo.intFormSeq == ID_FORMNO_101 && m_uInfo.intCurCol == ID_COL_101I_INVONO) ||
	   (m_uInfo.intFormSeq == ID_FORMNO_121 && m_uInfo.intCurCol == ID_COL_121I_INVONO) ||
	   (m_uInfo.intFormSeq == ID_FORMNO_151 && m_uInfo.intCurCol == ID_COL_151I_INVONO) ||
	   (m_uInfo.intFormSeq == ID_FORMNO_152 && m_uInfo.intCurCol == ID_COL_152I_INVONO) ||
	   (m_uInfo.intFormSeq == ID_FORMNO_153 && m_uInfo.intCurCol == ID_COL_153I_INVONO) ||
	   (m_uInfo.intFormSeq == ID_FORMNO_161 && m_uInfo.intCurCol == ID_COL_161I_INVONO) ||
	   (m_uInfo.intFormSeq == ID_FORMNO_162 && m_uInfo.intCurCol == ID_COL_162I_INVONO) ||
	   (m_uInfo.intFormSeq >= ID_FORMNO_171 && m_uInfo.intFormSeq <= ID_FORMNO_1720 && m_uInfo.intCurCol == ID_COL_171I_INVONO) )
	{
		bRet = TRUE;
	}
	return(bRet);
}

// ---------------------------------------------------------------------------------------------------------------------------------------------
//	サイズが変更されると呼び出されます。
// ---------------------------------------------------------------------------------------------------------------------------------------------
void CfrmUc000Common::OnSize(UINT nType, int cx, int cy)
{
	ICSFormView::OnSize(nType, cx, cy);

	m_TselFSize = 110;	// T選択用フォントサイズ
	// ※Diagのコンボボックス選択が2択の場合、画面のリサイズの影響で１択分しか表示されない不具合の対策。
	// 　後々、現状のフォントサイズ（11）でもT選択のコンボが表示されない不具合が出たら、
	// 　ここで画面のサイズや解像度を見てフォントサイズを設定するようにする

	//// T選択のフォントサイズを設定するために画面のサイズを取得
	//CRect		rec;
	//GetWindowRect(&rec);
	//ScreenToClient(rec);

// 修正No.168548,168552 add -->
	// 画面を最小化した時に登録番号一覧の挙動がおかしくなるため修正
	if(nType == SIZE_RESTORED && cx == 0 && cy == 0)	{
		if(InvNoClmChk() == TRUE)	{
			// ---- 登録番号一覧関連 ----->
			if(pZmSel && pZmSel->IsDisplay() != FALSE) {
				pZmSel->DialogOFF();
			}
			// ---- 登録番号一覧関連 <-----
			ResizeSw = 1;
			ResizeClm = m_uInfo.intCurCol;

			m_uInfo.intCurCol = m_uInfo.intCurCol - 1;
			CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
		}
	}
	else {
		if(ResizeSw == 1) {
			m_uInfo.intCurCol = ResizeClm;
			CmnDiagSetFocus(m_pDiag, m_uInfo.intCurRow, m_uInfo.intCurCol);
		}

		ResizeSw = 0;
		ResizeClm = -1;
	}
// 修正No.168548,168552 add <--
}
// インボイス登録番号追加対応_23/11/06 add <--
