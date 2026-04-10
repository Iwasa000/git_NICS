#ifndef	__FRMUC000COMMON_H__
#define	__FRMUC000COMMON_H__

#pragma once

#include "dbUc000Common.h"			// テーブル共通クラス
#include "dbUcInfMain.h"			// uc_inf_main
#include "dbUcInfSub.h"				// uc_inf_sub
#include "dbUcInfFormType.h"		// uc_inf_form_type
#include "dbUcLstBank.h"			// uc_lst_bank
#include "dbUcLstKamoku.h"			// uc_lst_kamoku
#include "dbUcLstItemSort.h"		// uc_lst_item_sort
#include "dbUcRenMain.h"			// uc_ren_main
#include "dbUcRenKamoku.h"			// uc_ren_kamoku
#include "dbUcRenDarecTemp.h"		// uc_ren_darec_temp

#include "dlgPrintSetup.h"			// ダイアログ：印刷設定
#include "dlgPrintAll.h"			// ダイアログ：一括印刷
#include "dlgOutputForm.h"			// ダイアログ：出力形式
#include "dlgDisplay.h"				// ダイアログ：一括金額/一時保管参照
// midori 152133 add -->
#include "dlgDisplay2.h"			// ダイアログ：一括金額/一時保管参照（レポート）
// midori 152133 add <--
#include "dlgAmount.h"				// ダイアログ：計設定
#include "dlgSpecialRow.h"			// ダイアログ：特殊行挿入
#include "dlgReference.h"			// ダイアログ：参照
#include "dlgSort.h"				// ダイアログ：ソート
#include "dlgYoushikiSelect.h"		// ダイアログ：様式選択
#include "dlgMessageBox.h"			// ダイアログ：メッセージ表示
#include "dlgRenMain.h"				// ダイアログ：財務連動
#include "dlgAddressSearch.h"		// ダイアログ：ICS様住所検索
#include "dlgZeroMoneyMove.h"		// ダイアログ：一括で一時保管へ移動
#include "dlgImportMessage.h"		// ダイアログ：インポート中のメッセージ
#include "dlgUserSet.h"				// ダイアログ：オプション設定（旧ユーザー設定）
#include "dlgYakuZokuSelect.h"		// ダイアログ：役職名・代表者との関係
#include "dlgCopySettingInfo.h"		// ダイアログ：他マスターから設定情報のコピー
#include "dlgDeleteDataEachYoushiki.h"	// ダイアログ：様式別全データ削除
// midori 160606 add -->
#include "dlgZeroMoneyClear.h"		// ダイアログ：金額等数値０クリア
// midori 160606 add <--

#include "dateConvert.h"			// 日付変換クラス
#include "DBSubFnc8.h"				// 漢字逆変換クラス

#include "UcFunctionCommon.h"		// 内訳書関数郡　共通宣言
#include "FuriganaConverter.h"
// 修正No.168516 del -->
//// 230227 -->
//#include <SyzInvoice.h>
//// 230227 add <--
// 修正No.168516 del <--

class CfrmUc000Common: public ICSFormView
{
	DECLARE_DYNCREATE(CfrmUc000Common)

public:
	CfrmUc000Common();           // 動的生成に使用されるプロテクト コンストラクタ。
	CfrmUc000Common(UINT prmld);
	virtual ~CfrmUc000Common();

public:
	CDatabase*			m_pDB;				// データベースハンドル
	UCHIWAKE_INFO		m_uInfo;			// 共通データ
// midori 152770 add -->
// 保存用の共通データを作成
	UCHIWAKE_INFO		m_uInfoSv;			// 共通データ（出力形式の切替 関連テーブルの空行挿入で使用）
	UCHIWAKE_INFO		m_uInfoSv2;			// 共通データ（出力形式の切替 関連テーブルの空行挿入で使用）
// midori 152770 add <--
	COLORREF			m_ViewColor;		// ビュー/ダイアログの背景色
	CICSDiag*			m_pDiag;
	CICSDiag*			m_pDiagKei;
	CICSDiag*			m_pTitleDiag;		// タイトル入力欄（Commonのみで使用）
// 修正No.168481,168502,168517 add -->
	CICSDiag*			m_pHeadDiag;		// ヘッダ部（登録一覧の表示に使用）
	CICSDiag*			m_pDiagKei2;		// 登録番号表示位置を指定する用
// 修正No.168481,168502,168517 add <--
// midori 152718 add -->
	CICSDiag*			m_pKakuteiDiag;		// 「入力確定中」
	CICSDiag*			m_pPageDiag;
// midori 152718 add <--
	CprtJobFunction*	m_prtJob;			// 印刷処理
	BOOL				m_CursorSetFlag;	// アクティブイベントにてカーソルセットする／しないフラグ（TRUE:する、FALSE:しない）
	BOOL				m_InitialFlag;		// 各帳表のOnInitalUpdate内の初期処理を実行する／しないフラグ（TRUE:する、FALSE:しない）
	BOOL				m_ChangeFont;		// コンボボックスセット時にフォントサイズを変更するか？→ChangeFontSizeのコールにより挙動がおかしくなるため制御
	int					m_IkkatuOutZero;	//　一括金額に０円を含めないフラグ	TRUE:０円を含めない／FALSE:０円を含める（従来通りの挙動）
	long				m_NameKei;			// 計の名称　0:"計"／0以外:"合計"（ビット計算）
	RENZVOL_INFO		m_RenZvol;			//	会社情報
	ICSButton			m_btnPageBack;		//	前頁
	ICSButton			m_btnPageNext;		//	次頁
	ICSButton			m_btnPageNew;		//	改頁
	ICSButton			m_btnPageInsert;	//	頁挿入
	ICSButton			m_btnRowCopy;		//	行コピー
	ICSButton			m_btnRowPaste;		//	行貼付
	ICSButton			m_btnRowInsert;		//	行挿入貼付
	ICSButton			m_btnYousikiNext;	//　次の様式
	ICSButton			m_btnYousikiBack;	//　前の様式

	CUcFunctionCommon	m_clsFunc;			//	共通関数クラス
	BOOL				m_ActiveNonFlag;	//	印刷時＋インポート実行時、内訳書マスター有り→無しの会社切替時は、Activeイベントの処理を実施しない対応（TRUE:処理しない）
// midori 160612 add -->
	static	BOOL		m_DataKakutei;		// 入力データを確定する
// midori 160612 add <--
// midori 161106 add -->
	int					m_CmbCnt;			// 科目コンボボックスの表示件数
	BOOL				m_F9OnSw;			// TRUE:「新規作成」が選択されてF9を呼び出された
// midori 161106 add <--
// midori 152201 add -->
	int					m_ComboSgn;			// 0:ComboSelで科目登録画面の表示無し 1:ComboSelで科目登録画面の表示有り
// midori 152201 add <--
// midori 151405 add -->
	int					nG_MScroll;
	int					m_Titlefocus;		// 1:タイトルダイアログにフォーカス有り
	int					m_Pagefocus;		// 1:ページダイアログにフォーカス有り
	int					m_Headfocus;		// 1:ヘッダダイアログにフォーカス有り（様式⑱-1 その他科目の内訳書②－１～⑱-20 その他科目の内訳書②－２０で使用）
// midori 151405 add <--
// 改良No.21-0086,21-0529 del -->
//※サインではなく、直接関数を呼んでチェックするよう変更
//// No.200905 add -->
//	int					m_KmkRowSgn;		// 0:マスターの期首が平成31年4月1日以前 1:0:マスターの期首が平成31年4月1日より後
//// No.200905 add <--
// 改良No.21-0086,21-0529 del <--
// midori 152137 add -->
	CMap<long,long,short,short> m_SortMap1;		// KnOrderをキーに科目コンボボックスのインデックスを取得 Key:KnOrder Value:コンボボックスのインデックス
	CMap<short,short,long,long> m_SortMap2;		// 科目コンボボックスのインデックスをキーにKnOrderを取得 Key:コンボボックスのインデックス Value:KnOrder
// midori 152137 add <--

// 修正No.157627 add -->
	COLORREF			m_BkColor[128];		// 背景色の一時保存用
	COLORREF			m_FcColor[128];		// 文字色の一時保存用
// 修正No.157627 add <--
// 要通知案件修正 21/07/06 add -->
	BOOL				m_bInitColor;		// コンボボックスからの処理の仕方によってこの関数が2回入り、誤った文字色背景色になっています為、
											// フラグで処理の管理をする
// 要通知案件修正 21/07/06 add <--
// 要通知案件 修正No.158229 add -->
	int					m_SaveBtnID;		// 画面上部のボタンをクリックした時に、ディセーブルになってしまう不具合を解消
// 要通知案件 修正No.158229 add <--

// 修正No.158174 add -->
	int					m_PrnDlgCol;		// 日付にカーソルがある状態でF4:プレビューを実行すると、プレビュー画面内のF7:拡大F8:縮小がキーボードで押せなくなる現象を解消するため、
											// 一旦カーソル位置を退避させる
	int					m_PrnDlgRow;
// 修正No.158174 add <--
// 230227 add -->
	CZmGen8				m_ZmGen;
	//BOOL				m_Invoice;			// 改良No.21-0086,21-0529 del
	BYTE				m_S_sgn1;
// 230227 add <--

// インボイス登録番号追加対応_23/11/06 add -->
	CString				gErrMsg;			// 登録番号エラーメッセージ表示用

	short				m_TselFSize;		// T選択のフォントサイズ設定（解像度を見て変えられるように）

	int					m_InvnoErrFlg;		// 登録番号チェックの結果エラーになった
// インボイス登録番号追加対応_23/11/06 add <--

protected:
// 14-1821_15-1030 add -->
	//BOOL				m_NullSyokeiRow;	// ソート後、1行目に小計空白(FgFunc = ID_FGFUNC_SYOKEINULL)が発生したか（TRUE:発生した）
// 14-1821_15-1030 add <--
	static	BOOL		m_EndView;			// 帳票終了時、EditOffを発生させるか（TRUE:発生させない）
	static	BOOL		m_ImportMode;		// インポート後の処理中？（TRUE:インポート後　FALSE:インポート後ではない）
	static  BOOL		m_YoushikiSkip;		// 様式にデータ行がある様式に飛ぶ／全て空行でも飛ぶ (TRUE:データのある様式へ、FALSE:無条件に前後の様式へ)
	static  BOOL		m_DispIkkatDlg;		// F4,F5押下時に一括印刷ダイアログを出力する？(TRUE:出力する / FALSE:出力しない）
	static	BOOL		m_DispYoushikiSelect;	// 起動時に様式選択ダイアログを表示する？(TRUE:出力する / FALSE:出力しない）
	static	BOOL		m_ReturnMoveDown;	// Enterキーでカーソルを下に移動する？(TRUE:下に移動する / FALSE:右に移動する（従来通り））
	static	BOOL		m_RegAutoRef;		// 参照データを自動で登録する
// midori 152135,152136 add -->
	static	int			m_BankSort;			// 参照ダイアログ::表示順（金融機関）	0:番号順	1:50音順
// midori 152745 del -->
//	static	int			m_AddressSort;		// 参照ダイアログ::表示順（取引先）		0:番号順	1:50音順
// midori 152745 del <--
// midori 152745 add -->
	static	int			m_AddressSort[2];	// 参照ダイアログ::表示順（取引先）		0:番号順	1:50音順	[0]:仕入先 [1]：得意先
// midori 152745 add <--
// midori 152135,152136 add <--
	static	BOOL		m_isF6;				// F6でフォーカス移動する際にTRUE;
// midori 190505 add -->
	static	int			m_DispFlg;
// midori 190505 add <--

public:
	enum{ IDD = IDD_FORMCOMMON };

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnInitialUpdate();

	DECLARE_MESSAGE_MAP()

private:
	BOOL	CmnGetChangeFormType();
	int		CmnGetControlRowSub(int intIndex);
	CString CmnDiagGetDigitFormat(int intDigit, DIAGRAM_ATTRIBUTE* pudAttr,
								  BOOL fKanma = TRUE, BOOL fDecimal = FALSE, int nDecDegit = 0);
	int		CmnGetDestinationYoushiki( BOOL dest );	// TRUE:前の様式へ FALSE:次の様式へ
	BOOL	GetShowKeiZero(CString strTemp);
	CprtJobFunction* CreatePrintObject(int intFormSeq);
	int		DoProcAfterReturnHokanTable(CdbUc000Common* rsData);
	int		SetPageRowNumToReturnDataByOrder(CdbUc000Common* rsData, int nPage, int nRow, int nBaseRow);
	//CString	ConvertFurigana(CRevConv& StrConv, CString strBase);
	int	UpdateAdDataFromAddress(CString table_name);
	int	UpdateAdDataFromBank(CString table_name);
	
public:
	virtual CdbUc000Common* virGetDataTableObject();
	virtual	BOOL	virStartJob();
	virtual	void	virGetCurrentInfo(CURRENT_INFO* uCurInfo);
	virtual	void	virSetInitialDiag();
	virtual void	virSetInitialUchiwakeInfo();
	virtual void	virSetInitialValue();
	virtual	void	virUpdateControlTitle();
	virtual	BOOL	virUpdateControlTblData();
// midori 161108 add -->
	virtual	BOOL	virUpdateControlTblData2();
// midori 161108 add <--
	virtual void	virFncCellCopyProc(CdbUc000Common* rs);
	virtual	int		virFncReferenceGetType(int intCol, int* intGrSeq, long* lngItemSeq, int* intAdd1, int* intAdd2, int* intAdd1Col, int* intAdd2Col, REFERENCE_INFO* pudReference = 0 );
	virtual	void	virFncReferenceSetData(CdlgReference* dlgRef, int intRefType, int intCol);
	virtual void	virFncReferenceSetICSData(CdlgAddressSearch* dlgAdd);
	virtual void	virFncReferenceSetICSData(CdlgYakuZokuSelect* dlg, int type);
	virtual	void	virFncReferenceUpdateTbl(int intRefType, int nRefMode = 0);
	virtual void	virTblEditIkkatuLine(int nType, CdbUc000Common* rsData, CString strName, CALCKEI_INFO uCalcKei, SORTKAMOKU_INFO uKamoku);
// midori 190505 add -->
	virtual void	virTblEditKamokuLine(CdbUc000Common* rsData, int pPage, int pRow, CString strName);
	virtual CString	virTblSortGetKamokuName( CdbUc000Common* rsData );
// midori 190505 add <--
// 20-0873 add -->
	virtual int		virTblSortGetTorihikisakiCode( CdbUc000Common* rsData );
	virtual CString	virTblSortGetTorihikisakiName( CdbUc000Common* rsData );
	virtual int		virTblSortGetKamokuCode( CdbUc000Common* rsData );
// 20-0873 add <--
	virtual	int		virTblSortSubSortProc(CdlgSort* pCdlgSort, CdbUc000Common* rsData);
	virtual	void	virTblGetIkkatuSumValue(CdbUc000Common* rsData, int nFgShow, int nGroup, CALCKEI_INFO* uCalc);
	virtual	void	virTblGetKamokuInfo(CdbUc000Common* rsData, SORTKAMOKU_INFO* uKamoku);
	virtual	int		virUpdateRelationTblChengeFormType();
// midori 152770 add -->
	virtual	BOOL	virUpdateRelationTblChengeFormTypeSave();			// 出力形式変更前に関連テーブルの共通データ(m_uInfo)を保存する
// midori 152770 add <--
	virtual	int		virRecBufSaveData(int inPage, int inRow);
	virtual	int		virRecBufSaveDataForPasteInsert(int inPage, int inRow);
	virtual	int		virRecBufSaveDataForPaste(int inPage, int inRow);
	virtual	void	virRowButtonCopyProc();
	virtual void	virOnUpdateButtonF6(BOOL* bFncEnable);
	virtual void	virUpdateTitle();
	virtual void	virInputDecisionEditOFF();
	virtual void    virInitialControlFgInputCell( int nRow, int nFgFunc, BYTE bFgInput );
	virtual	void	virRendoCreateData(CdbUc000Common* rsData, RENTEMP_INFO* uRenInfo);
	virtual	void	virRendoUpdateData(CdbUc000Common* rsData, RENTEMP_INFO* uRenInfo);
	virtual	void	virRendoSumData(CdbUc000Common* rsData, RENTEMP_INFO* uRenInfo);
	virtual void	virActivateFrame(UINT nState, CFrameWnd* pDeactivateFrame);				// 修正No.157633_元号日付 add
// 要通知案件修正 21/07/09 add -->
	virtual void	virActivateFrameK(UINT nState, CFrameWnd* pDeactivateFrame);
	virtual void	virKamokuAddCancel();
// 要通知案件修正 21/07/09 add <--

// インボイス登録番号追加対応_23/11/06 add -->
	virtual void	virDiagGetHjnData(CDatabase* pDB, HJN_REC_NEW* phjnRec);
	virtual void	virDiagSetHjnData(int pSw, CString rtInvNo, CString rtName, CString rtAdrs1, CString rtAdrs2, CString rtKana);
	virtual int		virCallChkInvoiceNum(int pSw, int inPage, int inRow);
// インボイス登録番号追加対応_23/11/06 add <--

//	void	OnButtonF1();
	void	OnButtonF2();
	void	OnButtonF3();
	void	OnButtonF4();
	void	OnButtonF5();
	void	OnButtonF6();
	void	OnButtonF7();
	void	OnButtonF8();
	void	OnButtonF9();
	void	OnButtonF11();
	void	OnButtonF12();
	void	OnButtonHome();
	void	OnButtonInsert();
//	void	OnButtonEnd();
//	void	OnUpdateButtonF1(CCmdUI* pCmdUI);
	void	OnUpdateButtonF2(CCmdUI* pCmdUI);
//	void	OnUpdateButtonF3(CCmdUI* pCmdUI);
	void	OnUpdateButtonF4(CCmdUI* pCmdUI);
	void	OnUpdateButtonF5(CCmdUI* pCmdUI);
	void	OnUpdateButtonF6(CCmdUI* pCmdUI);
	void	OnUpdateButtonF7(CCmdUI* pCmdUI);
	void	OnUpdateButtonF8(CCmdUI* pCmdUI);
	void	OnUpdateButtonF9(CCmdUI* pCmdUI);
	void	OnUpdateButtonF11(CCmdUI* pCmdUI);
	void	OnUpdateButtonF12(CCmdUI* pCmdUI);
	void	OnUpdateButtonHome(CCmdUI* pCmdUI);
	void	OnUpdateButtonInsert(CCmdUI* pCmdUI);
	void	OnUpdateButtonDelete(CCmdUI* pCmdUI);
	void	OnMenuDataImport();
	void	OnMenuDataExport();
	void	OnMenuPrintSetup();
// midori 160610 add -->
	void	OnMenuReference2();
// midori 160610 add <--
	void	OnMenuPrintAll();
	void	OnMenuFormTypeSetup();
	void	OnMenuSpecialRow();
	void	OnMenuSort();
	void	OnMenuIkkatuKingaku();
	void	OnMenuZeroMoney();
	void	OnMenuZeroMoneyMove();
	void	OnMenuOutKeiSetup();
// midori 160606 add -->
	void	OnMenuClear();
// midori 160606 add <--
	void	OnMenuZaimuRendo();
	void	OnMenuTitleRestore();
	void	OnMenuOptionSetting();
	void	OnMenuCopySettingInfo();
	void	OnMenuDeleteDataEachYoushiki();
// midori 180101 add -->
	void	OnMenuPrintHyoshi();
// midori 180101 add <--
// midori 180104 add -->
	void	OnFromkomonMenu();
// midori 180104 add <--
// midori 160612 add -->
	void	OnUpdateMenuDataImport(CCmdUI *pCmdUI);
// midori 160612 add <--
	void	OnUpdateMenuIkkatuKingaku(CCmdUI* pCmdUI);
	void	OnUpdateMenuZeroMoney(CCmdUI* pCmdUI);
	void	OnUpdateMenuZeroMoneyMove(CCmdUI* pCmdUI);
	void	OnUpdateMenuZaimuRendo(CCmdUI* pCmdUI);
	void	OnUpdateMenuSpecialRow(CCmdUI* pCmdUI);
	void	OnUpdateMenuSort(CCmdUI* pCmdUI);
// midori 160612 add -->
	void	OnUpdateMenuClear(CCmdUI *pCmdUI);
	void	OnUpdateMenuTitleRestore(CCmdUI *pCmdUI);
	void	OnUpdateMenuDeleteDataEachYoushiki(CCmdUI *pCmdUI);
// midori 160612 add <--
// midori 180104 add -->
	void	OnUpdateFromkomonMenu(CCmdUI *pCmdUI);
// midori 180104 add <--
	void	OnPageBack();
	void	OnPageNext();
	void	OnPageNew();
	void	OnPageInsert();
	void	OnRowCopy();
	void	OnRowPaste();
	void	OnRowInsert();
	void	OnMoveYoushikiBack();
	void	OnMoveYoushikiNext();

	void	OnFilePrintPreview();
	virtual void	OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual BOOL	OnPreparePrinting(CPrintInfo* pInfo);
	virtual void	OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void	OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void	OnPrint(CDC* pDC, CPrintInfo* pInfo);


	int		CmnGetControlRow();
	int		CmnGetControlRow(int intIndex);
	int		CmnGetControlCol();
	void	CmnGetControlPosition(int* intRow, int* intCol);
	void	CmnGetControlPosition(int* intRow, int* intCol, int* intOldCol);
	int		CmnGetControlIndex(int intRow, int intCol);
// midori 190301 add -->
	int		CmnGetControlIndexEx(int intRow, int intCol);
// midori 190301 add <--
	int		CmnGetControlTotalIndex(int intTotalNo, int intCol);
	int		CmnGetControlTotalType();

	//void	CmnDiagInit(DIAGRAM_DATA* diadata);
	//CString CmnDiagGetString(CICSDiag* pDiag, int intIndex);
	int		CmnDiagGetValue(CICSDiag* pDiag, int intIndex);
	CString CmnDiagGetKingaku(CICSDiag* pDiag, int intIndex);
	long	CmnDiagGetDate(CICSDiag* pDiag, int intIndex, int intFlg);
// midori 123 add -->
	long	CmnDiagGetDateWM(CICSDiag* pDiag, int intIndex, int intDate );
// midori 123 add <--
	long	CmnDiagGetDateWMD(CICSDiag* pDiag, int intIndex, int intFlg, int intDate );
// midori 200101 add -->
	long	CmnDiagGetDateGWM(CICSDiag* pDiag, int intIndex, int intDate );
	long	CmnDiagGetDateGWMD(CICSDiag* pDiag, int intIndex, int intDate );
// midori 200101 add <--
	void	CmnDiagSetString(CICSDiag* pDiag, int intIndex, CString strData, int intFlg);
	void	CmnDiagSetValue(CICSDiag* pDiag, int intIndex, int intData, int intFlg);
	void	CmnDiagSetCombo(CICSDiag* pDiag, int intIndex, long lngData);
// midori 152137 add -->
	void	CmnDiagSetCombo2(CICSDiag* pDiag, int intIndex, long lngData);
// midori 152137 add <--
// インボイス登録番号追加対応_23/11/17 add -->
	void	CmnDiagSetCombo3(CICSDiag* pDiag, int intIndex, CString lngData);
// インボイス登録番号追加対応_23/11/17 add <--
	void	CmnDiagSetKingaku(CICSDiag* pDiag, int intIndex, CString strData);
	void	CmnDiagSetDate(CICSDiag* pDiag, int intIndex, int intFlg, long intDate);
	void	CmnDiagSetDateWMD(CICSDiag* pDiag, int intIndex, int intFlg, long intDate);
	int		CmnDiagSetEnterReDraw( CICSDiag* pDiag, int nRow, int nCol );
	int		CmnDiagSetEnterReDraw( CICSDiag* pDiag, int nIndex );
	void	CmnDiagSetFocus(CICSDiag* pDiag, int intRow, int intCol);
	void	CmnDiagSetAttrInpmax(CICSDiag* pDiag, int intIndex, int intInpmax);
	void	CmnDiagSetAttrDigit(CICSDiag* pDiag, int intIndex, int intDigit, 
								BOOL fKanma = TRUE, BOOL fDecimal = FALSE, int nDecDegit = 0);
	void	CmnDiagSetAttrColZenHan(CICSDiag* pDiag, int intCol);
	void	CmnDiagSetAttrColor(CICSDiag* pDiag, int intIndex, COLORREF* color);
	void	CmnDiagSetAttrColorAllClear(CICSDiag* pDiag);
	void	CmnDiagSetAttrSpecialRow(CICSDiag* pDiag, int intRow, int FgFunc);
	void	CmnDiagSetAttrFgInputCell( CICSDiag* pDiag, int nBackPos, int nDataPos, int nFgFunc, BYTE bFgInput );
// No.200601 add -->
	void	CmnDiagSetAttrFgStringCell( CICSDiag* pDiag, int nDataPos, int nFgFunc, int nFgInput );
// No.200601 add <--
	void	CmnDiagSetAttrComboToEdit(CICSDiag* pDiag, int intIndex, BOOL bRestore);
	void	CmnDiagSetAttrChangeEdit(CICSDiag* pDiag, int intIndex, BOOL bRestore);
// インボイス登録番号追加対応_23/09/11 add -->
	void	CmnDiagSetAttrChangeEdit2(CICSDiag* pDiag, int pIndex, short pAlign, short pInpmax, short pLinemax, short pEditkind, short pEditattr);
// インボイス登録番号追加対応_23/09/11 add <--
// midori 190301 add -->
	void	CmnDiagSetAttrEditFormat( CICSDiag* pDiag, int intCol, int intInpmax, int intLinemax, CString strEditfmt );
	void	CmnDiagSetAttrDisableHide( CICSDiag* pDiag, int intCol, int pSw );
// midori 190301 add <--
	void	CmnDiagSetCursorPos(CICSDiag* pDiag, int intRow, int intCol);

	//void	CmnEventStartProc(int intEventNo);						// 修正No.168443 del
	void	CmnEventStartProc(int intEventNo, int intForkSel=-1);	// 修正No.168443 add
	BOOL	CmnSetInitialUchiwakeInfo(int intFormSeq = 0);
	void	CmnSetInitialRowMax();
	void	CmnCheckEnableMoveButton(BOOL* bBack, BOOL* bNext);
	BOOL	CmnCheckEnableTitleEdit();
	BOOL	CmnCheckRowButtonProc(int intButtonType);
	void	CmnChangeFieldValData(char* chrVal, CString strVal);
	void	CmnGetNotCommaValData(CString* strVal, char* chrVal);
	void	CmnKingakuAddFunc(CString *inTotalVal, CString inAddVal, int inAddFlag = 1);
//	void	CmnKingakuAddFunc(CString *inTotalVal, CString inAddVal, int inAddFlag = 1, BOOL* bShowZero = NULL);
// 158096 del -->
	//int		CmnKingakuCmpFunc(CString strKingaku1, CString strKingaku2);
// 158096 del <--
// 158096 add -->
	int		CmnKingakuCmpFunc( CString strKingaku1, CString strKingaku2, CString strKingaku3 );
// 158096 add <--
	BOOL	CmnGetLastRowIndex(int index, int shtLast);
	BOOL	CmnGetTotalRowType();
	int		CmnGetNextPageRow(int* pPage, int* pRow);
	BOOL	CmnGetNextFirstRow(int intRow);
	int		CmnGetDlgCtrlID();
	CString	CmnGetSykeiNameWithKakko(CString strSykeiName, CString strLeft, CString strRight);
	CString	CmnGetSykeiNameWithKakko(CString strSykeiName, CString strLeft, CString strRight, CString strKamoku);
	BOOL	CmnCheckShiftKey();
	BOOL	CmnCheckTotalRow();
	void	CmnEnableMinMaxBox( CWnd* pParent );
	void	CmnChangeButtonEnable(int intEventFlag);
	//void	CmnStrDivision(CString strTarget, CStringArray* pstrArray, int nSize);
	//void	CmnStrDivisionEx( CString strData , CStringArray* strArry , int intMax , int intRow , BOOL blnChCod );
	//CString CmnStrDocking(CString strTarget1, CString strTarget2);
	COLORREF CmnGetBackColor();
	BOOL	CmnCheckDate( CICSDiag* pDiag, int intIndex, int intFlg, int intDate );

	void	CmnTblInitialMakeData();
	BOOL	CmnTblCheckDeleteData();
	void	CmnTblEndDeleteData();
	int		CmnTblGetPageMax();
	int		CmnTblInsertNullLine(int nPage, int nRow, int nLine);
	int		CmnTblInsertSpecialLine(int nPage, int nRow, int nType, CString strName);
	int		CmnTblInsertPageKeiRuiKei(void);
	int		CmnTblDeleteFgFunc(int inFgFuncKind);
	BOOL	CmnTblDeleteMainProc( int* pnDelLine );
	int		CmnDeleteNullLine( int nPage, int nRow );
	int		CmnTblDeleteToHokan(int inPage, int inRow);
	int		CmnTblDeleteLine(int inPage, int inRow, int inNum);
// midori 152709 del -->
//	int		CmnTblUpdatePageRowAllResetMain();
// midori 152709 del <--
// midori 152709 add -->
	int		CmnTblUpdatePageRowAllResetMain(int pSw=0);
// midori 152709 add <--
	int		CmnTblUpdatePageRowAllResetMainAfterSort(CdlgSort* pCdlgSort);
	int		CmnTblUpdatePageNumRowNumAdd(int inPage, int inRow, int inAdd);
// midori 154714 add -->
	void	CmnTblUpdateNewPageCheckAndDelete( void );
// midori 154714 add <--
	int		CmnTblUpdatePageNumRowNumReset(int inPage, int inRow);
// midori 152709 add -->
// midori 156968 del -->
	//int		CmnTblUpdatePageNumRowNumReset_Sub(void);
// midori 156968 del <--
// midori 156968 add -->
	int		CmnTblUpdatePageNumRowNumReset_Sub(int pSw = 0);
// midori 156968 add <--
// 157153 add -->
	int		CmnTblUpdatePageNumRowNumReset_Sub_Kanso(int pSw = 0);
// 157153 add <--
// midori 156188 del -->
	//void	CmnTblDeleteFirstNullLine(void);
// midori 156188 del <--
// midori 156188 add -->
// 157153 del -->
	//int		CmnTblUpdatePageNumRowNumReset_Sub2(void);
// 157153 del <--
// midori 156968 del -->
	//void	CmnTblDeleteFirstNullLine(int pSw = 0);
// midori 156968 del <--
// midori 156968 add -->
	void	CmnTblDeleteFirstNullLine(int pSw = 0,int pSw2 = 0);
// midori 156968 add <--
// 157153 add -->
	void	CmnTblDeleteFirstNullLine_Kanso(int pSw2 = 0);
// 157153 add <--
// midori 156188 add <--
// midori 152709 add <--
	int		CmnTblUpdateFgShowDataReturn(EnumIdFgShow nFgShow, int nPage, int nRow, int nGroup);
	int		CmnTblChangeRecord(int* pPage, int* pRow, int nTarget);
	int		CmnTblUpdateEndPageSetting();
	int		CmnTblCalcKei();
// 14-1821_15-1030 del -->
	//int		CmnTblCalcKeiAfterSort(CdlgSort* pCdlgSort);
// 14-1821_15-1030 del <--
	int		CmnTblSortMainProc(CdlgSort* pCdlgSort);
	int		CmnTblSortSubPreCheck(CdlgSort* pCdlgSort);
	int		CmnTblSortSubSortBeforeProc(CdlgSort* pCdlgSort);
	void	CmnTblSortSubGetSortParam(CdlgSort* pCdlgSort, CString* str1, CString* str2);
	int		CmnTblSortSubSortAfterProc(CdlgSort* pCdlgSort, CdbUc000Common* rsData);
// midori 156968 del -->
//// midori 190505 add -->
//	int		CmnTblSortSubSortAfterKaipage(CdlgSort* pCdlgSort, CdbUc000Common* rsData);
//// midori 190505 add -->
// midori 156968 del <--
	int		CmnTblIkkatuAutoUpdateProc(int nType, int nGroup, int* nPage, int* nRow, SORTKAMOKU_INFO uKamoku);
	int		CmnTblIkkatuHokanProc(EnumIdFgShow nFgShow);
	int		CmnTblCountMaxRow();
	void	CmnTblIkkatuAddProc();
	int		CmnTblIkkatuAddCurrentToIkkatu(int nPage, int nRow, int nNumGroup);
	int		CmnTblIkkatuAddGetKamokuInfo(int nPage, int nRow, SORTKAMOKU_INFO* uKamoku);
	int		CmnTblRendoMainProc(CdlgRenMain* pCdlgRenMain, CdbUc000Common* rsData);
	int		CmnTblRendoSubKzrecProc(CdlgRenMain* pCdlgRenMain, CDBZmSub* pZmSub, RENTEMP_INFO* uRendoInfo, CdbUc000Common* rsData);
	int		CmnTblRendoSubKzSumProc(CdlgRenMain* pCdlgRenMain, CDBZmSub* pZmSub, RENTEMP_INFO* uRendoInfo, CdbUc000Common* rsData);
	int		CmnTblRendoSubEzrecProc(CdlgRenMain* pCdlgRenMain, CDBZmSub* pZmSub, RENTEMP_INFO* uRendoInfo, CdbUc000Common* rsData);
	int		CmnTblRendoSubDarecProc(CdlgRenMain* pCdlgRenMain, CDBZmSub* pZmSub, RENTEMP_INFO* uRendoInfo, CdbUc000Common* rsData);
	int		CmnTblRendoSubDarecSub(CdlgRenMain* pCdlgRenMain, CDBZmSub* pZmSub, RENTEMP_INFO* uRenInfo, CdbUc000Common* rsData);
	int		CmnTblRendoUpdValStartProc(CdlgRenMain* pCdlgRenMain, RENTEMP_INFO* uRenInfo, CdbUc000Common* rsData);
	int		CmnTblRendoUpdValEndProc(CdlgRenMain* pCdlgRenMain, RENTEMP_INFO* uRenInfo, CdbUc000Common* rsData);
	int		CmnTblZeroMoneyMoveMainProc( HOKAN_DATA *hData );
// midori 160606 add -->
	int		CmnTblZeroMoneyClearMainProc( CLEAR_DATA *hData );
// midori 160606 add <--
// midori 152150 add -->
	int		DeleteZeroSyokei(CdbUc000Common* rsData);
// midori 152150 add <--
// midori 170502 del -->
//	int		CmnDataImportPostProcess();
// midori 170502 del <--
	void	UpdateYoushikiMoveButton(BOOL fMode);
// midori 160607 add -->
	void	UpdateYoushikiMoveButton2( void );
// midori 160607 add <--
// midori 160612 add -->
	void	ButtonEnableChg( void );
// midori 160612 add <--
	CICSDiag* CmnGetDiagType(CICSDiag* diag1, CICSDiag* diag2, CICSDiag* diag3 = NULL);
	int		SetKeiDialogPosition(CICSDiag* pDiagData, CICSDiag* pDiagKei);
	int		CmnGetKeiDiagIndex(int intTotalType, int nCol);
	int		CmnTblAddToHokanOneLine(int nFormSeq, int nPage, int nRow);
	BOOL	CreateReferenceDlgKana();
	short	SetComboFontSize(CICSDiag* pDiag, int index, BOOL bFlg);
// midori 190505 del -->
//	int		DeleteKnNameBehindSyokei(const CdbUcInfSub* mfcRec);
// midori 190505 del <--
// midori 190505 add -->
	void	SetKnNameSyokei(int nFormSeq,int ppage,int prow,int pSw);
	void	DeleteKamokuKeiStr(CString szSrc, CString* szDst);
	void	GetKamokuKeiStr(CString szSrc,CString sKamoku, CString* szDst);
// midori 190505 add <--
	void	SetEndViewFlag(BOOL state);
	BOOL	IsRegExistHokanData();
	int		CopyHokanDataToHokanTable(int nFormSeq);
// 修正No.158301 del -->
//	void	OnMenuPrintAllSub(int call_by = ID_PRT_IKKATSU);
// 修正No.158301 del <--
// 修正No.158301 add -->
	int		OnMenuPrintAllSub(int call_by = ID_PRT_IKKATSU);
// 修正No.158301 add <--
	void	GetCommonInfo(CCtrlMainGeneralVar& cm);
	void	CopyOwntblData();
	BOOL	UpdateVer1_0_2_1(CString& strErr);
	int		CopyCommonPrintZero(const CdbUcInfMain& rc);
	CString	GetSQLCopyCommonPrintZero(int bit);
	int		UpdateReferenceData();
	BOOL	GetDispYoushikiSelect(){return m_DispYoushikiSelect;}
	LRESULT DoButtonInsert(WPARAM w, LPARAM l);
//	BOOL	IsSaveMoveReference(int col_to);
	BOOL	IsSaveMoveReference(int col_from, int col_to);
// midori 160612 add -->
	//	ダイアグラムの入力項目を入力可又は、不可にする
	void	DlgSet_Attr( CICSDiag *, short, BOOL );
// midori 160612 add <--
// midori 152201 add -->
	short	GetKnOrder(int intFormSeq,int inPage,int inRow);
// midori 152201 add <--
// midori 152364 add -->
	int		IsSpecialRow( BYTE pSw );
// midori 152364 add <--
// 修正No.157898 add -->
	int		IsSpecialRow2( BYTE pSw );
// 修正No.157898 add <--
// midori 190505 add -->
// midori 157043 del -->
	//void	KamokuRowChk(void);
// midori 157043 del <--
	CString GetSyokeiName(int nFormSeq,int ppage,int prow,CString pstrKei);
// midori 190505 add <--

	BOOL	CmnUcInfMainGetInitInfo();
	BOOL	CmnUcInfSubGetInitInfo();
	void	CmnUcInfSubSetCurrentInfo();
	void	CmnUcInfSubSetTitleInfo(BOOL bSetFlag);
	BOOL	CmnUcInfFormTypeGetFormType();
	BOOL	CmnUcRenMainGetRendoInfo();
	BOOL	CmnUcLstKamokuGetComboList( CString *strRetCombo, BOOL &bFontSizeChange );
	BOOL	CmnUcLstItemSortGetData(int intItemSeq, ITEMSORT_INFO* uItemSort);
// midori 190505 add -->
	BOOL	CmnUcLstItemSortGetData2(int nFormSeq,int intItemSeq, ITEMSORT_INFO* uItemSort);
// midori 190505 add <--
	void	CmnRowInverse( BOOL blnFlag , CURRENT_INFO *curInfo );

// 修正No.157627 add -->
	void	CmnRowBeforeInverse(CURRENT_INFO *curInfo);
	void	CmnRowAfterInverse(CURRENT_INFO *curInfo);
// 修正No.157627 add <--

	void	CmnCloseActiveDocument();

// 修正No.157858 add -->
	BOOL	CmnICSChkDatarec(CDBZmSub* pZmSub, RENTEMP_INFO* uRenInfo, CString pKcod, int pEcod);
// 修正No.157858 add <--

	BOOL	CmnICSGetSumKzrec( CDBZmSub* pZmSub, RENTEMP_INFO* uRenInfo, CString* pSumZan);
	BOOL	CmnICSGetSumEzrec(CDBZmSub* pZmSub, RENTEMP_INFO* uRenInfo, CString* pSumZan);
	//BOOL	CmnICSGetSumDarec(CdbUcRenDarecTemp* pTemp, CKNREC* pKnrec, CString* pSumZan);						// インボイス登録番号追加対応_23/11/29 del
	BOOL	CmnICSGetSumDarec(CdbUcRenDarecTemp* pTemp, CKNREC* pKnrec, CString* pSumZan, CString* pInvno);		// インボイス登録番号追加対応_23/11/29 add
	BOOL	CmnICSGetDarecDeleteRec(CDBZmSub* pZmSub);
	CString CmnICSGetKnrecKnnam(CDBZmSub* pZmSub, CString strKcd);
// midori 152137 add -->
	CString CmnICSGetKnrecKnKana(CDBZmSub* pZmSub, CString strKcd);
// midori 152137 add <--
	CString CmnICSGetEzrecEzname(CDBZmSub* pZmSub);
// midori 152137 add -->
	CString CmnICSGetEzrecEzkana(CDBZmSub* pZmSub);
// midori 152137 add <--
	CString CmnICSGetTkrecInvno(CDBZmSub* pZmSub);				// インボイス登録番号追加対応_23/11/27 add
	BOOL	CmnICSGetSykMonth(CDBZmSub* pZmSub, int intOffset, RENTEMP_INFO* uRenInfo);
	int		CmnICSGetMinfoData(CDBZmSub* pZmSub, RENTEMP_INFO* uRenInfo );
	CString CmnICSGetSykMonthQuery( CDBZmSub* pZmSub, RENTEMP_INFO* uRenInfo );

	BOOL	CmnEndView(UINT message, WPARAM wParam);
	BOOL	CmnCheckTableInit();
	BOOL	CmnCheckTableReference( int intRefType );
	BOOL	CmnCheckTableZaimuRendo(void);
	BOOL	CmnCheckTablePrintSetup();
	BOOL	CmnCheckTableSort();
// midori 190505 add -->
// midori 156950 del -->
	//int		SortPrintMsg(int nFormSeq,int pSw);
// midori 156950 del <--
// midori 156950 add -->
	int		SortPrintMsg(int nFormSeq);
// midori 156950 add <--
	int		SortPrintMsgSub(int nFormSeq);
// midori 190505 add <--
// 要通知案件修正 21/07/09 add -->
	int		CmnWherePushButton(CPoint point);		// クリックされたボタンを調べる
// 要通知案件修正 21/07/09 add <--

// midori 161110_2 add -->
public:
	CMap<int, int, CString, CString> m_mapEdaZan;
// midori 161110_2 add <--

protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
protected:
	virtual void OnActivateFrame(UINT nState, CFrameWnd* pDeactivateFrame);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnIkkatuAddMenu();
public:
	afx_msg void OnUpdateIkkatuaddMenu(CCmdUI *pCmdUI);

// 2009.09.08 TS自動ログオフ対応
public:
	BOOL	EndApp(UINT message, WPARAM wParam);
protected:
	virtual	BOOL	virEndProc(void);

// midori 160612 add -->
public:
	LRESULT OnUserFocusset(WPARAM wParam, LPARAM lParam);
// midori 160612 add <--
	LRESULT OnUserFocusset4(WPARAM wParam, LPARAM lParam);	// 修正No.168393 add
// midori 155879 add -->
	LRESULT	OnUserControlPage(WPARAM wParam, LPARAM lParam);
// midori 155879 add <--
// midori 155906 del -->
	void ClearTempTbl(void);
// midori 155906 del <--
// midori157118 add -->
	CString DelCrLfString(CString szBuf);
// midori157118 add <--
// No.200601 add -->
	int	CmnInitialControlFgStringCell(CICSDiag *pDiag, int nRow, int nCol, int nFgFunc,int nMaxLen, CString sData );
	int CmnStringLenChk(int nMaxLen,CString sData);
	void CmnHeadDiagSetTitle(CICSDiag *pDiag,int prow,CString ptitle,int pSgn);
	void CmnHeadDiagChangeAlign(CICSDiag *pDiag, int pidx, int pSign);
// No.200601 add <--
// midori 156968 add -->
	void InsertNull(void);
// midori 156968 add <--
// 修正No.157628 add -->
	CString	MakeReplaceSent(CString pStr);
	CString	MakeReplaceSent2(CString pStr, CString pStr2);
	void	ReplaceHankakuKana(CString& pStr);
// 修正No.157628 add <--

// 21/07/09 add -->
	int		CmnTblDataToFirstCol(int* pCol);			// 日付入力項目にカーソルがセットされている時に退避させる
// 21/07/09 add <--
// 230227 add -->
	CString		InvoiceCalc( CDBZmSub* pZmSub );
// 230227 add <--

// 改良No.22-0811 add -->
	void		CmnMenuEnableSwitch();					// 財務連動メニューの切り替えテスト
// 改良No.22-0811 add <--

// インボイス登録番号追加対応_23/11/06 add -->
	// 指定された登録番号（適格請求書事業者番号）または法人番号より特定の情報を取得する
	int			GetBillingProviderHojinInfo(CDatabase* pDB, int inGrSeq, CString pInvno, CString* rName, CString* rAdd1, CString* rAdd2, CString* rErrMsg, int pSw=0);

	// 登録番号を「T」または空白 と 番号に分ける
	void		InvoNoSplit(CString pInvno, CString* rtData1, CString* rtData2);
	// 今のカラムが「登録番号」のカラムかどうかチェック
	BOOL		InvNoClmChk(void);

	CZmselDB		*pZmSel;
	int				m_EndSyorityu;
	CSyzInvoice		pSyzInvoice;

	// ZmSel（登録番号一覧）の初期化
	void		InitZmSel(void);
	// ZmSel（登録番号一覧）を更新
	void		UpdateZmSel(void);		// 修正No.168364 add
	// ZmSel（登録番号一覧）を表示
	LRESULT		OnShowZmselDialog(WPARAM wParam, LPARAM lParam);
	friend	void CALLBACK callback( UINT sgn, void* p, CWnd* pwnd );

	afx_msg void OnSize(UINT nType, int cx, int cy);
// インボイス登録番号追加対応_23/11/06 add <--

};
#endif
