//--------------------------------------------------
//	frmUc021Uketoritegata.h
//--------------------------------------------------

#pragma once

#include "frmUc000Common.h"
#include "dbUc021Uketoritegata.h"
#include "prtUc021Uketoritegata.h"

/****************************************************************
	「受取手形」関連
*****************************************************************/
#define	ID_ROWNORMAL_021		24			// 通常時：１頁内の合計行を含む全体の行数
#define	ID_ROWKOBETU_021		24			// 個別時：１頁内の合計行を含む全体の行数
#define	ID_ROWKEI_021			1			// 帳表固定の合計行数（1:合計あり，0:なし）
#define	ID_COLMAX_021			24/*21*/		// １行内のコントロール個数
#define	ID_COLMAXDATA_021		10/*9*/		// １行内の入力コントロール数
#define	ID_CTLSTARTINDEX_021	10/*9*/		// １行目の入力項目の先頭インデックス番号
#define ID_T1STARTNORMAL_021	552/*528*//*483*/		// 通常時：合計行①の先頭インデックス番号
#define ID_T2STARTNORMAL_021	576/*552*//*504*/		// 通常時：合計行②の先頭インデックス番号
#define ID_T1STARTKOBETU_021	552/*528*//*483*/		// 個別時：合計行①の先頭インデックス番号
#define ID_T2STARTKOBETU_021	576/*552*//*504*/		// 個別時：合計行②の先頭インデックス番号

// 「受取手形」テーブル名称
//	正式版では"CdbUc021Uketoritegata"のメンバー変数で参照可能としたい
#define	TBL_NAME_021		_T("uc_021_Uketoritegata")

//////////////////////////////////////////////////////////////////
//	「受取手形」の内訳書列番号（帳表入力画面で使用）
typedef enum {
	ID_COL_021_KAMOKU = 1,	// 1	科目
	ID_COL_021_ADNAME1,		// 2	差出人1
	ID_COL_021_SPDATE,		// 3	振出年月日
	ID_COL_021_PMDATE,		// 4	支払期日
	ID_COL_021_BKNAME1,		// 5	支払銀行名
	ID_COL_021_BKNAME2,		// 6	支払銀行名（支店名）
	ID_COL_021_VAL,			// 7	金額
	ID_COL_021_BK2NAME1,	// 8	割引銀行名等
	ID_COL_021_BK2NAME2,	// 9	割引銀行名等（支店名）
	ID_COL_021_TEKI,		// 10	摘要
} EnumIdFormCol021;

//////////////////////////////////////////////////////////////////
//	「受取手形」の内訳書_集計項目
#define	ID_CALC_021_MAX		1	// 集計フィールド件数
#define	ID_CALC_021_FIELD1	_T("Val")	// 集計フィールド名称1

//////////////////////////////////////////////////////////////////
//	「受取手形」の内訳書_計ダイアグラムの入力インデックス
// midori 152503 del -->
//#define ID_COL_021_KEI1_MEISYO	0				// 計ダイアログ1行目の計名称（計or合計、頁計、累計）
//#define ID_COL_021_KEI1_VAL		3				// 計ダイアログ1行目の期末現在高
//#define ID_COL_021_KEI2_MEISYO	9				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
//#define ID_COL_021_KEI2_VAL		12				// 計ダイアログ2行目の期末現在高
// midori 152503 del <--
// midori 152503 add -->
#define ID_COL_021_KEI1_MEISYO	0				// 計ダイアログ1行目の計名称（計or合計、頁計、累計）
#define ID_COL_021_KEI1_VAL		4				// 計ダイアログ1行目の期末現在高
#define ID_COL_021_KEI2_MEISYO	10				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
#define ID_COL_021_KEI2_VAL		14				// 計ダイアログ2行目の期末現在高
// midori 152503 add <--

//////////////////////////////////////////////////////////////////
//	1レコード分の構造体
typedef struct tagRecuc_021_Uketoritegata_021
{
	long	m_Seq;			// シーケンス番号
	int		m_NumPage;		// 頁番号
	BYTE	m_NumRow;		// 行番号
	BYTE	m_FgFunc;		// 特殊行フラグ
	BYTE	m_FgShow;		// 表示フラグ
	CString	m_KeiStr;		// 計
	long	m_NumGroup;		// グループ番号
	long	m_AdSeq;		// 取引先 シーケンス番号
	long	m_AdOrder;		// 取引先 並び順
	CString	m_AdName1;		// 振出人（名称：上段）
	CString	m_AdName2;		// 振出人（名称：下段）
	long	m_SpDate;		// 振出年月日
	long	m_PmDate;		// 支払期日
	long	m_BkSeq;		// 金融機関（シーケンス番号）
	long	m_BkOrder;		// 金融機関（並び順）
	CString	m_BkName1;		// 支払銀行名（銀行名）
	CString	m_BkName2;		// 支払銀行名（支店名）
	CString	m_Val;			// 金額
	long	m_Bk2Seq;		// 金融機関（シーケンス番号）
	long	m_Bk2Order;		// 金融機関（並び順）
	CString	m_Bk2Name1;		// 割引銀行名等（銀行名）
	CString	m_Bk2Name2;		// 割引銀行名等（支店名）
	CString	m_Teki;			// 摘要
	int		m_ShowKeiZero;	//０円計表示フラグ
	long	m_KnSeq;		// 科目シーケンス
	long	m_KnOrder;		// 科目オーダー
	CString	m_KnName;		// 科目名
// midori 152137 add -->
	CString	m_AdKana;		// 振出人
	CString	m_KnKana;		// 科目
// midori 152137 add <--
	CString	m_InvNo;		// インボイス登録番号	インボイス登録番号追加対応_23/09/11 add
} REC_UC_021_UKETORITEGATE, *P_REC_UC_021_UKETORITEGATE;

class CfrmUc021Uketoritegata: public CfrmUc000Common
{
	DECLARE_DYNCREATE( CfrmUc021Uketoritegata )

public:
	CfrmUc021Uketoritegata();
	virtual	~CfrmUc021Uketoritegata();
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
	enum{ IDD = IDD_YOUSHIKI021 };
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
// midori 161108 add -->
	BOOL	virUpdateControlTblData2();
// midori 161108 add <--
	void	UpdateControlRowData( CdbUc021Uketoritegata* inTbl );
	void	UpdateControlRowTotal( CdbUc021Uketoritegata* inTbl );
	CdbUc000Common*	virGetDataTableObject();
	void	virGetCurrentInfo(CURRENT_INFO* uCurInfo);
	void	virFncCellCopyProc(CdbUc000Common* rsData);									// F6 :項目複写
	int		virFncReferenceGetType(int intCol, int* intGrSeq, long* lngItemSeq, int* intAdd1, int* intAdd2, int* intAdd1Col, int* intAdd2Col, REFERENCE_INFO* pudReference = 0);	// F9 :参照
	void	virFncReferenceSetData(CdlgReference* dlgRef, int intRefType, int intCol);	// F9 :参照
	// void	virFncReferenceSetICSData(CdlgAddressSearch* dlgAddd);						// F9 :参照
	void	virFncReferenceUpdateTbl(int intRefType, int nRefMode = 0);					// F9 :参照
// 要通知案件修正 21/07/09 add -->
	void	virActivateFrameK(UINT nState, CFrameWnd* pDeactivateFrame);
	void	virKamokuAddCancel();
// 要通知案件修正 21/07/09 add <--

	int		GetFocusPosition( int nChar );	// フォーカス移動位置取得関数
	int		GetTotalRowIndex( int nChar , int intIndex , int intNext );	// 行移動制御関数
	BOOL	CheckShiftKey();

	void	virTblEditIkkatuLine(int nType, CdbUc000Common* rsData, CString strName, CALCKEI_INFO uCalcKei, SORTKAMOKU_INFO uKamoku);
// midori 156189,156190,156191 del -->
//// midori 190505 add -->
//	void	virTblEditKamokuLine(CdbUc000Common* rsData, int pPage, int pRow);
//// midori 190505 add <--
// midori 156189,156190,156191 del <--
// 20-0873 add -->
	int		virTblSortGetTorihikisakiCode( CdbUc000Common* rsData );
	CString	virTblSortGetTorihikisakiName( CdbUc000Common* rsData );
	int		virTblSortGetKamokuCode( CdbUc000Common* rsData );
// 20-0873 add <--
	int		virTblSortSubSortProc(CdlgSort* pCdlgSort, CdbUc000Common* rsData);
	void	virUpdateTitle();
	void	virInputDecisionEditOFF();

	void	SetInputMode( int nRow, long nMode );
	void	SetDiagBackColor();
	void	virRendoCreateData(CdbUc000Common* rsData, RENTEMP_INFO* uRenInfo);
	void	virRendoUpdateData(CdbUc000Common* rsData, RENTEMP_INFO* uRenInfo);
	void	virTblGetKamokuInfo(CdbUc000Common* rsData, SORTKAMOKU_INFO* uKamoku);
// midori 151405 add -->
	int		CursorControl(short nChar, int sw);
// midori 151405 add <--

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

	LRESULT OnUserReDrawView(WPARAM wParam, LPARAM lParam);	// 修正No.162446 add
};
