//--------------------------------------------------
//	frmUc091Kaikakekin.h
//--------------------------------------------------

#pragma once

#include "frmUc000Common.h"
#include "dbUc091Kaikakekin.h"
#include "prtUc091Kaikakekin.h"
#include "dlgKaikakekin.h"			//	⑨買掛金の欄外登録ダイアログクラス

/****************************************************************
	「買掛金」関連
*****************************************************************/
#define	ID_ROWNORMAL_091		24		// 通常時：１頁内の合計行を含む全体の行数
#define	ID_ROWKOBETU_091		24		// 個別時：１頁内の合計行を含む全体の行数
#define	ID_ROWKEI_091			1		// 帳表固定の合計行数（1:合計あり，0:なし）
#define	ID_COLMAX_091			11		// １行内のコントロール個数
#define	ID_COLMAXDATA_091		6		// １行内の入力コントロール数
#define	ID_CTLSTARTINDEX_091	5		// １行目の入力項目の先頭インデックス番号
#define ID_T1STARTNORMAL_091	253		// 通常時：合計行①の先頭インデックス番号
#define ID_T2STARTNORMAL_091	264		// 通常時：合計行②の先頭インデックス番号
#define ID_T1STARTKOBETU_091	253		// 個別時：合計行①の先頭インデックス番号
#define ID_T2STARTKOBETU_091	264		// 個別時：合計行②の先頭インデックス番号

// 「買掛金」テーブル名称
//	正式版では"CdbUc091Kaikakekin"のメンバー変数で参照可能としたい
#define	TBL_NAME_091		_T("uc_091_Kaikakekin")

//////////////////////////////////////////////////////////////////
//	「買掛金」の内訳書列番号（帳表入力画面で使用）
typedef enum {
	ID_COL_091_KNORDER = 1,				// 1	科目
	ID_COL_091_ADNAME1,					// 2	名称1
	ID_COL_091_ADADD1,					// 3	所在地1
	ID_COL_091_ADADD2,					// 4	所在地2
	ID_COL_091_VAL,						// 5	金額
	ID_COL_091_TEKI,					// 6	摘要
} EnumIdFormCol091;

//////////////////////////////////////////////////////////////////
//	「買掛金」の内訳書_集計項目
#define	ID_CALC_091_MAX		1			// 集計フィールド件数
#define	ID_CALC_091_FIELD1	_T("Val")	// 集計フィールド名称1

//////////////////////////////////////////////////////////////////
//	「買掛金」の内訳書_計ダイアグラムの入力インデックス
#define ID_COL_091_KEI1_MEISYO	0				// 計ダイアログ1行目の計名称（計or合計、頁計、累計）
// midori 152503 del -->
//#define ID_COL_091_KEI1_VAL		3				// 計ダイアログ1行目の金額
//#define ID_COL_091_KEI2_MEISYO	7				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
//#define ID_COL_091_KEI2_VAL		10				// 計ダイアログ2行目の金額
// midori 152503 del <--
// midori 152503 add -->
#define ID_COL_091_KEI1_VAL		4				// 計ダイアログ1行目の金額
#define ID_COL_091_KEI2_MEISYO	8				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
#define ID_COL_091_KEI2_VAL		12				// 計ダイアログ2行目の金額
// midori 152503 add <--

//////////////////////////////////////////////////////////////////
//	1レコード分の構造体
typedef struct tagRecuc_091_Kaikakekin_091
{
	long	m_Seq;
	int		m_NumPage;
	BYTE	m_NumRow;
	BYTE	m_FgFunc;
	BYTE	m_FgShow;
	CString	m_KeiStr;
	long	m_NumGroup;
	long	m_KnSeq;
	long	m_KnOrder;
	CString	m_KnName;
	long	m_AdSeq;
	long	m_AdOrder;
	CString	m_AdName1;
	CString	m_AdName2;
	CString	m_AdAdd1;
	CString	m_AdAdd2;
	CString	m_Val;
	CString	m_Teki;
	int		m_ShowKeiZero;
// midori 152137 add -->
	CString	m_KnKana;			// 科目
	CString	m_AdKana;			// 相手先
// midori 152137 add <--
	CString	m_InvNo;			// 登録番号(法人番号)	インボイス登録番号追加対応_23/10/20 add
} REC_UC_091_KAIKAKEKIN, *P_REC_UC_091_KAIKAKEKIN;

class CfrmUc091Kaikakekin: public CfrmUc000Common
{
	DECLARE_DYNCREATE( CfrmUc091Kaikakekin )

public:
	CfrmUc091Kaikakekin();
	virtual	~CfrmUc091Kaikakekin();
	void	virSetInitialValue();

private:
	void	virRowButtonCopyProc();
	int		virRecBufSaveData( int inPage, int inRow );
	int		virRecBufSaveDataForPasteInsert( int inPage, int inRow );
	int		virRecBufSaveDataForPaste( int inPage, int inRow );
	int		RecBufClearData( P_REC_UC_091_KAIKAKEKIN inRecData );
	int		RecBufSaveDataSub( int inPage, int inRow, P_REC_UC_091_KAIKAKEKIN inSaveData );
	int		RecBufSetData( CdbUc091Kaikakekin* inTbl );
	int		RecBufClearAllData();

	// 1ページ分のデータ格納用配列
	// 配列の添え字と、行番号を合わせるため、最大行+1とした
	REC_UC_091_KAIKAKEKIN	m_ReadData[ID_ROWNORMAL_091 + 1];
	REC_UC_091_KAIKAKEKIN	m_CopyData;		// 行コピー用バッファ
	int	m_nCopyFlag;					// コピーフラグ（0：コピーデータがない／1：コピーデータがある）

public:
	enum{ IDD = IDD_YOUSHIKI091 };
	CICSDiag	m_TitleDiag;
	CICSDiag	m_PageDiag;
	CICSDiag	m_NumberDiag;
// midori 161108 add -->
	CICSDiag	m_SaveDataDiag;
// midori 161108 add <--
	CICSDiag	m_HeadDiag;
	CICSDiag*	m_Diag;											// m_Diag1とm_Diag2を受けるポインタ
	CICSDiag	m_Diag1;										// 計が1行の明細ダイアログ
	CICSDiag	m_Diag2;										// 計が2行の明細ダイアログ
	CICSDiag*	m_DiagKei;										// m_DiagKei1とm_DiagKei2を受けるポインタ
	CICSDiag	m_DiagKei1;										// 計が1行の計専用ダイアグラム
	CICSDiag	m_DiagKei2;										// 計が2行専用のダイアグラム
//	CICSDiag m_Diag3											// 計あり様式なので、計が0行は存在しない
// 修正No.157626 add -->
	CICSDisp	m_Guide1;										// 欄外のガイド表示
// 修正No.157626 add <--
	ICSButton	m_btnMarjinal;									//	欄外登録ボタン

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
	void	UpdateControlRowData( CdbUc091Kaikakekin* inTbl );
	void	UpdateControlRowTotal( CdbUc091Kaikakekin* inTbl );
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
	int		virTblSortSubSortProc(CdlgSort* pCdlgSort, CdbUc000Common* rsData);
	void	virTblGetKamokuInfo(CdbUc000Common* rsData, SORTKAMOKU_INFO* uKamoku);
	void	virUpdateTitle();
	void	virInputDecisionEditOFF();
	
	void	SetInputMode( int nRow, long nMode );
	void	SetDiagBackColor();
	void	virRendoCreateData(CdbUc000Common* rsData, RENTEMP_INFO* uRenInfo);
	void	virRendoUpdateData(CdbUc000Common* rsData, RENTEMP_INFO* uRenInfo);
	void	virFncReferenceSetICSData(CdlgAddressSearch* dlgAddd);	// F9:参照

	BOOL	CheckTableMarginal();
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
public:
	afx_msg void OnBnClickedMarginalButton1();

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
// No.200601 add -->
	void TitleColorChg(void);
// No.200601 add <--
	LRESULT OnUserReDrawView(WPARAM wParam, LPARAM lParam);	// 修正No.162446
};
