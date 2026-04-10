
#pragma once

#include "frmUc000Common.h"
// midori 190301 add -->
#include "frmUc011Yotyokin.h"
// midori 190301 add <--
#include "dbUc011Yotyokin.h"
#include "prtUc011Yotyokin.h"
#include "afxwin.h"

/****************************************************************
	011 預貯金関連
*****************************************************************/
// midori kanso_0318 del -->
//#define ID_ROWNORMAL_011		24		// 通常時：１頁内の合計行を含む全体の行数
//#define ID_ROWKOBETU_011		24		// 個別時：１頁内の合計行を含む全体の行数
////#define ID_ROWNORMAL_011		23		// 通常時：１頁内の合計行を含む全体の行数　合計行を別ダイアグラムに移行
////#define ID_ROWKOBETU_011		23		// 個別時：１頁内の合計行を含む全体の行数　合計行を別ダイアグラムに移行
//#define ID_ROWKEI_011			1		// 帳表固定の合計行数（1:合計あり，0:なし）
////#define ID_COLMAX_011			13		// １行内のコントロール個数
// midori kanso_0318 del <--
#define ID_COLMAX_011K			12		// １行内のコントロール個数
// midori kanso_0318 del -->
//#define ID_COLMAXDATA_011		6		// １行内の入力コントロール数
//#define ID_CTLSTARTINDEX_011	6		// １行目の入力項目の先頭インデックス番号
//#define ID_T1STARTNORMAL_011	299		// 合計行①の先頭インデックス番号（通常）
//#define ID_T2STARTNORMAL_011	312		// 合計行②の先頭インデックス番号（通常）
//#define ID_T1STARTKOBETU_011	299		// 合計行①の先頭インデックス番号（個別）
//#define ID_T2STARTKOBETU_011	312		// 合計行②の先頭インデックス番号（個別）
#define ID_T1STARTNORMAL_011K	276		// 合計行①の先頭インデックス番号（通常）
#define ID_T2STARTNORMAL_011K	288		// 合計行②の先頭インデックス番号（通常）
#define ID_T1STARTKOBETU_011K	276		// 合計行①の先頭インデックス番号（個別）
#define ID_T2STARTKOBETU_011K	388		// 合計行②の先頭インデックス番号（個別）
//
////	預貯金テーブル名称 → 金融機関/科目のUpdateDataTable()で使用
////		正式版では"CdbUcYotyokin"のメンバー変数で参照可能としたい
//#define TBL_NAME_011		_T("uc_011_Yotyokin")
//
////////////////////////////////////////////////////////////////////
////	預貯金等の内訳書列番号（帳表入力画面で使用）
//typedef enum {
//	ID_COL_011_KINYU = 1,	//	 1	金融機関名
//	ID_COL_011_KINYUSUB,	//	 2	金融機関支店名
//	ID_COL_011_SYURUI,		//	 3	種類
//	ID_COL_011_KOUZA,		//	 4	口座番号
//	ID_COL_011_KIMATU,		//	 5	期末現在高
//	ID_COL_011_TEKIYOU		//	 6	摘要
//} EnumIdFormCol011K;
//
////////////////////////////////////////////////////////////////////
////	預貯金等の内訳書_集計項目
//#define ID_CALC_011_MAX			1				// 集計フィールド件数
//#define ID_CALC_011_FIELD1		_T("Val")		// 集計フィールド名称１
//
////////////////////////////////////////////////////////////////////
////	預貯金等の内訳書_計ダイアグラムの入力インデックス
//#define ID_COL_011_KEI1_MEISYO	0				// 計ダイアログ1行目の計名称（計or合計、頁計、累計）
//// midori 152503 del -->
////#define ID_COL_011_KEI1_KIMATU  3				// 計ダイアログ1行目の期末現在高
////#define ID_COL_011_KEI2_MEISYO	5				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
////#define ID_COL_011_KEI2_KIMATU  8				// 計ダイアログ2行目の期末現在高
//// midori 152503 del <--
//// midori 152503 add -->
// midori kanso_0318 del <--
//#define ID_COL_011K_KEI1_KIMATU	5				// 計ダイアログ1行目の期末現在高
//#define ID_COL_011K_KEI2_MEISYO	7				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
#define ID_COL_011K_KEI1_KIMATU	4				// 計ダイアログ1行目の期末現在高
#define ID_COL_011K_KEI2_MEISYO	8				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
#define ID_COL_011K_KEI2_KIMATU	12				// 計ダイアログ2行目の期末現在高
// midori kanso_0318 del -->
//// midori 152503 add <--
//
////////////////////////////////////////////////////////////////////
////	1レコード分の構造体
//typedef struct tagRecUc011Kyotyokin
//{
//	long	m_Seq;
//	int		m_NumPage;
//	BYTE	m_NumRow;
//	BYTE	m_FgFunc;
//	BYTE	m_FgShow;
//	CString	m_KeiStr;
//	long	m_NumGroup;
//
//	CString	m_AcNum;
//	CString	m_BkName1;
//	CString	m_BkName2;
//	long	m_BkOrder;
//	long	m_BkSeq;
//	CString	m_KnName;
//	long	m_KnOrder;
//	long	m_KnSeq;
//	CString	m_Teki;
//	CString	m_Val;
//	int		m_ShowKeiZero;
//// midori 152137 add -->
//	CString	m_BkKana1;
//	CString	m_BkKana2;
//	CString m_KnKana;
//// midori 152137 add <--
//}REC_UC_011_YOTYOKIN, *P_REC_UC_011_YOTYOKIN;
// midori kanso_0318 del <--

class CfrmUc011YotyokinK: public CfrmUc000Common
{
	DECLARE_DYNCREATE(CfrmUc011YotyokinK)

public:
	CfrmUc011YotyokinK();
	virtual	~CfrmUc011YotyokinK();
	void	virSetInitialValue();

private:
	void	virRowButtonCopyProc();
	int		virRecBufSaveData(int inPage, int inRow);
	int		virRecBufSaveDataForPasteInsert(int inPage, int inRow);
	int		virRecBufSaveDataForPaste(int inPage, int inRow);
	int		RecBufClearData( P_REC_UC_011_YOTYOKIN inRecData );
	int		RecBufSaveDataSub(int inPage, int inRow, P_REC_UC_011_YOTYOKIN inSaveData);
	int		RecBufSetData(CdbUc011Yotyokin* Yotyokin);
	int		RecBufClearAllData();

	// 1ページ分のデータ格納用配列
	// 配列の添え字と、行番号を合わせるため、最大行+1とした
	REC_UC_011_YOTYOKIN m_ReadData[ID_ROWNORMAL_011 + 1];	
	REC_UC_011_YOTYOKIN m_CopyData;			// 行コピー用バッファ
	int		m_nCopyFlag;					// コピーフラグ（0：コピーデータがない／1：コピーデータがある）

public:
	enum{ IDD = IDD_YOUSHIKI011K };
	CICSDiag	m_TitleDiag;
	CICSDiag	m_PageDiag;
	CICSDiag	m_NumberDiag;
// midori 161108 add -->
	CICSDiag	m_SaveDataDiag;
// midori 161108 add <--
	CICSDiag	m_HeadDiag;
	CICSDiag*	m_Diag;						// m_Diag1とm_Diag2を受けるポインタ
	CICSDiag	m_Diag1;					// 計が1行の明細ダイアログ
	CICSDiag	m_Diag2;					// 計が2行の明細ダイアログ
	CICSDiag*	m_DiagKei;					// m_DiagKei1とm_DiagKei2を受けるポインタ
	CICSDiag	m_DiagKei1;					// 計が1行の計専用ダイアグラム
	CICSDiag	m_DiagKei2;					// 計が2行専用のダイアグラム
//	CICSDiag m_Diag3						// 計あり様式なので、計が0行は存在しない
// 修正No.157626 add -->
	CICSDisp	m_Guide1;					// 欄外のガイド表示
// 修正No.157626 add <--

private:
	BOOL	virStartJob();
	void	virSetInitialUchiwakeInfo();
	void	virSetInitialDiag();
	BOOL	InitialControlDataArea();
	BOOL	InitialControlComboBox();
	void	InitialControlTotalArea();
	void	InitialControlSpecialRow(int intRow, int intFgFunc);
	void	virUpdateControlTitle();
	void	UpdateControlPage();
	BOOL	virUpdateControlTblData();
// midori 161108 add -->
	BOOL	virUpdateControlTblData2();
// midori 161108 add <--
	void	UpdateControlRowData(CdbUc011Yotyokin* Yotyokin);
	void	UpdateControlRowTotal(CdbUc011Yotyokin* Yotyokin);

	CdbUc000Common*	virGetDataTableObject();
	void	virGetCurrentInfo(CURRENT_INFO* uCurInfo);
	void	virFncCellCopyProc(CdbUc000Common* rsData);										// F6 :項目複写
	int		virFncReferenceGetType(int intCol, int* intGrSeq, long* lngItemSeq, int* intAdd1, int* intAdd2, int* intAdd1Col, int* intAdd2Col, REFERENCE_INFO* pudReference = 0);	// F9 :参照
	void	virFncReferenceSetData(CdlgReference* dlgRef, int intRefType, int intCol);		// F9 :参照
//	void	virFncReferenceSetICSData(CdlgAddressSearch* dlgAddd);							// F9 :参照
	void	virFncReferenceUpdateTbl(int intRefType, int nRefMode = 0);						// F9 :参照
// 要通知案件修正 21/07/09 add -->
	void	virActivateFrameK(UINT nState, CFrameWnd* pDeactivateFrame);
	void	virKamokuAddCancel();
// 要通知案件修正 21/07/09 add <--

	int		GetFocusPosition( int nChar );												// フォーカス移動位置取得関数
	int		GetTotalRowIndex( int nChar , int intIndex , int intNext );					// 行移動制御関数　 
	BOOL	CheckShiftKey();

	int		virTblSortSubSortProc(CdlgSort* pCdlgSort, CdbUc000Common* rsData);
	void	virTblGetKamokuInfo(CdbUc000Common* rsData, SORTKAMOKU_INFO* uKamoku);
	void	virUpdateTitle();
	void	virInputDecisionEditOFF();

	void	SetInputMode( int nRow, long nMode );
	void	SetDiagBackColor();
	void	virRendoCreateData(CdbUc000Common* rsData, RENTEMP_INFO* uRenInfo);
	void	virRendoUpdateData(CdbUc000Common* rsData, RENTEMP_INFO* uRenInfo);
// midori 151405 add -->
	int		CursorControl(short nChar, int sw);
// midori 151405 add <--

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnInitialUpdate();

	DECLARE_MESSAGE_MAP()

public:
	DECLARE_EVENTSINK_MAP()
public:
	void TerminationYoushikidiag1(short index, short nChar, short length, LPUNKNOWN data);
	void EditOFFYoushikidiag1(short index);
	void EditONYoushikidiag1(short index);
	void TerminationTitlediag1(short index, short nChar, short length, LPUNKNOWN data);
	void EditOFFTitlediag1(short index);
	void EditONTitlediag1(short index);
	void FocusTitlediag1();
	void TerminationPagediag1(short index, short nChar, short length, LPUNKNOWN data);
	void EditOFFPagediag1(short index);
	void EditONPagediag1(short index);
	void FocusPagediag1();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	virtual BOOL DestroyWindow();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// 2009.09.08 TS自動ログオフ対応
protected:
	virtual	BOOL	virEndProc(void);

// midori 161106 add -->
public:
	void ComboNew(short index);
// midori 161106 add <--
// midori 151405 add -->
	void VScrollYoushikidiag1(short mode);
// midori 151405 add <--
// midori 152134 add -->
	void ComboSelYoushikidiag1(short index, LPUNKNOWN data);
// midori 152134 add <--
// midori 190301 add -->
	//void	DataConver( void );
// midori 190301 add <--
// No.200601 add -->
	void BankCellChg(int nRow,int nCol);
	void TitleColorChg(void);
// No.200601 add <--

	LRESULT OnUserReDrawView(WPARAM wParam, LPARAM lParam);	// 修正No.162446 add
};
