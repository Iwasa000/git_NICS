// インボイス登録番号追加対応_23/09/11 add -->
//--------------------------------------------------
//	frmUc021UketoritegataI.h
//--------------------------------------------------

#pragma once

#include "frmUc000Common.h"
#include "frmUc021Uketoritegata.h"
#include "dbUc021Uketoritegata.h"
#include "prtUc021Uketoritegata.h"

/****************************************************************
	「受取手形」関連
*****************************************************************/
//#define	ID_COLMAX_021I			/*22*/23		// １行内のコントロール個数
#define	ID_COLMAX_021I			/*22*/20		// １行内のコントロール個数
#define	ID_COLMAXDATA_021I		/*11*/12		// １行内の入力コントロール数

#define	ID_BACKCOLMAX_021I		6				// １行内の背景用表示項目数
#define	ID_LINECOLMAX_021I		2				// １行内の罫線項目数

//#define	ID_CTLSTARTINDEX_021I	/*8*/9			// １行目の入力項目の先頭インデックス番号
#define	ID_CTLSTARTINDEX_021I	/*8*/6			// １行目の入力項目の先頭インデックス番号
//#define ID_T1STARTNORMAL_021I	/*506*/529		// 通常時：合計行①の先頭インデックス番号
//#define ID_T2STARTNORMAL_021I	/*530*/552		// 通常時：合計行②の先頭インデックス番号
//#define ID_T1STARTKOBETU_021I	/*506*/529		// 個別時：合計行①の先頭インデックス番号
//#define ID_T2STARTKOBETU_021I	/*530*/552		// 個別時：合計行②の先頭インデックス番号
#define ID_T1STARTNORMAL_021I	/*506*/460		// 通常時：合計行①の先頭インデックス番号
#define ID_T2STARTNORMAL_021I	/*530*/480		// 通常時：合計行②の先頭インデックス番号
#define ID_T1STARTKOBETU_021I	/*506*/460		// 個別時：合計行①の先頭インデックス番号
#define ID_T2STARTKOBETU_021I	/*530*/480		// 個別時：合計行②の先頭インデックス番号
//
//
////////////////////////////////////////////////////////////////////
//	「受取手形」の内訳書_計ダイアグラムの入力インデックス
#define ID_COL_021I_KEI1_MEISYO		0				// 計ダイアログ1行目の計名称（計or合計、頁計、累計）
#define ID_COL_021I_KEI1_VAL		7				// 計ダイアログ1行目の期末現在高
#define ID_COL_021I_KEI2_MEISYO		16				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
#define ID_COL_021I_KEI2_VAL		23				// 計ダイアログ2行目の期末現在高
//

//////////////////////////////////////////////////////////////////
//	「受取手形」の内訳書列番号（帳表入力画面で使用）
typedef enum {
	ID_COL_021I_KAMOKU = 1,		// 1	科目
	ID_COL_021I_INVONOT,		// 2	登録番号（T選択）
	ID_COL_021I_INVONO,			// 3	登録番号
	ID_COL_021I_ADNAME1,		// 4	振出人
	ID_COL_021I_SPDATE,			// 5	振出年月日
	ID_COL_021I_PMDATE,			// 6	支払期日
	ID_COL_021I_BKNAME1,		// 7	支払銀行名
	ID_COL_021I_BKNAME2,		// 8	支払銀行名（支店名）
	ID_COL_021I_VAL,			// 9	金額
	ID_COL_021I_BK2NAME1,		// 10	割引銀行名等
	ID_COL_021I_BK2NAME2,		// 11	割引銀行名等（支店名）
	ID_COL_021I_TEKI,			// 12	摘要
} EnumIdFormCol021I;

class CfrmUc021UketoritegataI: public CfrmUc000Common
{
	DECLARE_DYNCREATE( CfrmUc021UketoritegataI )

public:
	CfrmUc021UketoritegataI();
	virtual	~CfrmUc021UketoritegataI();
	void	virSetInitialValue();

private:
	void	virRowButtonCopyProc();
	int		virRecBufSaveData( int inPage, int inRow );
	int		virRecBufSaveDataForPasteInsert( int inPage, int inRow );
	int		virRecBufSaveDataForPaste( int inPage, int inRow );
	int		RecBufClearData( P_REC_UC_021_UKETORITEGATE inRecData );
	int		RecBufSaveDataSub( int inPage, int inRow, P_REC_UC_021_UKETORITEGATE inSaveData );
	int		RecBufSetData( CdbUc021Uketoritegata* inTbl );
	int		RecBufClearAllData();

	// 1ページ分のデータ格納用配列
	// 配列の添え字と、行番号を合わせるため、最大行+1とした
	REC_UC_021_UKETORITEGATE	m_ReadData[ID_ROWKOBETU_021 + 1];
	REC_UC_021_UKETORITEGATE	m_CopyData;		// 行コピー用バッファ
	int	m_nCopyFlag;					// コピーフラグ（0：コピーデータがない／1：コピーデータがある）

public:
	enum{ IDD = IDD_YOUSHIKI021I };
	CICSDiag	m_TitleDiag;
	CICSDiag	m_PageDiag;
	CICSDiag	m_NumberDiag;
	CICSDiag	m_SaveDataDiag;
	CICSDiag	m_HeadDiag;
	CICSDiag*	m_Diag;						// m_Diag1とm_Diag2を受けるポインタ
	CICSDiag	m_Diag1;					// 計が1行の明細ダイアログ
	CICSDiag	m_Diag2;					// 計が2行の明細ダイアログ
	CICSDiag*	m_DiagKei;					// m_DiagKei1とm_DiagKei2を受けるポインタ
	CICSDiag	m_DiagKei1;					// 計が1行の計専用ダイアグラム
	CICSDiag	m_DiagKei2;					// 計が2行専用のダイアグラム
	CICSDisp	m_Guide1;					// 欄外のガイド表示

	
private:
	BOOL	virStartJob();
	void	virSetInitialUchiwakeInfo();
	void	virSetInitialDiag();
	BOOL	InitialControlDataArea();
	BOOL	InitialControlComboBox();
	void	InitialControlTotalArea();
	void	InitialControlSpecialRow( int intRow, int intFgFunc );
	void	virUpdateControlTitle();
	void	UpdateControlPage();
	BOOL	virUpdateControlTblData();
	BOOL	virUpdateControlTblData2();
	void	UpdateControlRowData( CdbUc021Uketoritegata* inTbl, int pSw );
	void	UpdateControlRowTotal( CdbUc021Uketoritegata* inTbl );
	CdbUc000Common*	virGetDataTableObject();
	void	virGetCurrentInfo(CURRENT_INFO* uCurInfo);
	void	virFncCellCopyProc(CdbUc000Common* rsData);									// F6 :項目複写
	int		virFncReferenceGetType(int intCol, int* intGrSeq, long* lngItemSeq, int* intAdd1, int* intAdd2, int* intAdd1Col, int* intAdd2Col, REFERENCE_INFO* pudReference = 0);	// F9 :参照
	void	virFncReferenceSetData(CdlgReference* dlgRef, int intRefType, int intCol);	// F9 :参照
	void	virFncReferenceUpdateTbl(int intRefType, int nRefMode = 0);					// F9 :参照
	void	virActivateFrame(UINT nState, CFrameWnd* pDeactivateFrame);
	void	virActivateFrameK(UINT nState, CFrameWnd* pDeactivateFrame);
	void	virKamokuAddCancel();

	int		GetFocusPosition( int nChar );	// フォーカス移動位置取得関数
	int		GetTotalRowIndex( int nChar , int intIndex , int intNext );	// 行移動制御関数
	//BOOL	CheckShiftKey();		// 未使用

	void	virTblEditIkkatuLine(int nType, CdbUc000Common* rsData, CString strName, CALCKEI_INFO uCalcKei, SORTKAMOKU_INFO uKamoku);
	void	virTblEditKamokuLine(CdbUc000Common* rsData, int pPage, int pRow, CString strName);
	CString	virTblSortGetKamokuName( CdbUc000Common* rsData );
	int		virTblSortGetTorihikisakiCode( CdbUc000Common* rsData );
	CString	virTblSortGetTorihikisakiName( CdbUc000Common* rsData );
	int		virTblSortGetKamokuCode( CdbUc000Common* rsData );
	int		virTblSortSubSortProc(CdlgSort* pCdlgSort, CdbUc000Common* rsData);
	void	virUpdateTitle();
	void	virInputDecisionEditOFF();

	void	SetInputMode( int nRow, long nMode );
	void	SetDiagBackColor();
	void	virRendoCreateData(CdbUc000Common* rsData, RENTEMP_INFO* uRenInfo);
	void	virRendoUpdateData(CdbUc000Common* rsData, RENTEMP_INFO* uRenInfo);
	void	virTblGetKamokuInfo(CdbUc000Common* rsData, SORTKAMOKU_INFO* uKamoku);
	int		CursorControl(short nChar, int sw);

#ifdef _DEBUG
	virtual	void	AssertValid()	const;
	virtual	void	Dump( CDumpContext& dc )	const;
#endif

protected:
	virtual	void	DoDataExchange( CDataExchange* pDX );
	virtual	void	OnInitialUpdate();

	DECLARE_MESSAGE_MAP()

public:
	DECLARE_EVENTSINK_MAP()

public:
	void	TerminationYoushikidiag1( short index, short nChar, short length, LPUNKNOWN data );
	void	EditOFFYoushikidiag1( short index );
	void	EditONYoushikidiag1( short index );
	void	TerminationTitlediag1( short index, short nChar, short length, LPUNKNOWN data );
	void	EditOFFTitlediag1( short index );
	void	EditONTitlediag1( short index );
	void	FocusTitlediag1();
	void	TerminationPagediag1( short index, short nChar, short length, LPUNKNOWN data );
	void	EditOFFPagediag1( short index );
	void	EditONPagediag1( short index );
	void	FocusPagediag1();

public:
	virtual	BOOL	PreTranslateMessage( MSG* pMsg );

// 2009.09.08 TS自動ログオフ対応
protected:
	virtual	BOOL	virEndProc(void);

public:
	void ComboNew(short index);

	void Date_ChkAndSet(short, int, short);			// 元号+年月日の入力状況を取得

	void VScrollYoushikidiag1(short mode);
	void ComboSelYoushikidiag1(short index, LPUNKNOWN data);
	void BankCellChg(int nRow,int nCol);
	void TitleColorChg(void);

	LRESULT OnUserReDrawView(WPARAM wParam, LPARAM lParam);

	void	virDiagGetHjnData(CDatabase* pDB, HJN_REC_NEW* phjnRec);
	void	virDiagSetHjnData(int pSw, CString rtInvNo, CString rtName, CString rtAdrs1, CString rtAdrs2, CString rtKana);
	int		virCallChkInvoiceNum(int pSw, int inPage, int inRow);

	// 入力された登録番号から参照ダイアログを検索し、登録済みの場合はデータを取得して画面に表示
	int		ChkInvoiceNum(CDatabase* pDB, int pSw, int inPage, int inRow, CString pInvno="", CString pName="", CString pAdrs1="", CString pAdrs2="");
};
// インボイス登録番号追加対応_23/09/11 add <--
