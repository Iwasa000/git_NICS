//--------------------------------------------------
//	frmUc111Kariirekin.h
//--------------------------------------------------

#pragma once

#include "frmUc000Common.h"
#include "dbUc111Kariirekin.h"
#include "prtUc111Kariirekin.h"

/****************************************************************
	「借入金」関連
*****************************************************************/
#define	ID_ROWNORMAL_111		15		// 通常時：１頁内の合計行を含む全体の行数
#define	ID_ROWKOBETU_111		15		// 個別時：１頁内の合計行を含む全体の行数
#define	ID_ROWKEI_111			1		// 帳表固定の合計行数（1:合計あり，0:なし）
#define	ID_COLMAX_111			22		// １行内のコントロール個数
#define	ID_COLMAXDATA_111		10		// １行内の入力コントロール数
#define	ID_CTLSTARTINDEX_111	9		// １行目の入力項目の先頭インデックス番号
#define ID_T1STARTNORMAL_111	308		// 通常時：合計行①の先頭インデックス番号
#define ID_T2STARTNORMAL_111	330		// 通常時：合計行②の先頭インデックス番号
#define ID_T1STARTKOBETU_111	308		// 個別時：合計行①の先頭インデックス番号
#define ID_T2STARTKOBETU_111	330		// 個別時：合計行②の先頭インデックス番号

// 「借入金」テーブル名称
//	正式版では"CdbUc111Kariirekin"のメンバー変数で参照可能としたい
#define	TBL_NAME_111		_T("uc_111_Kariirekin")

//////////////////////////////////////////////////////////////////
//	「借入金」の内訳書列番号（帳表入力画面で使用）
typedef enum {
	ID_COL_111_KNORDER = 1,				// 1	科目
	ID_COL_111_ADNAME1,					// 2	借入先1
	ID_COL_111_KANKEI,					// 3	法人・代表者との関係
	ID_COL_111_ADADD1,					// 4	所在地1
	ID_COL_111_ADADD2,					// 5	所在地2
	ID_COL_111_VAL,						// 6	期末現在高
	ID_COL_111_RISOKU,					// 7	期中の支払利息
	ID_COL_111_RATE,					// 8	利率
	ID_COL_111_RIYUU,					// 9	借入理由
	ID_COL_111_NAIYOU,					// 10	担保の内容
} EnumIdFormCol111;

//////////////////////////////////////////////////////////////////
//	「借入金」の内訳書_集計項目
#define	ID_CALC_111_MAX		2	// 集計フィールド件数
#define	ID_CALC_111_FIELD1	_T("Val")	// 集計フィールド名称1
#define	ID_CALC_111_FIELD2	_T("Risoku")	// 集計フィールド名称2

//////////////////////////////////////////////////////////////////
//	「買掛金」の内訳書_計ダイアグラムの入力インデックス
#define ID_COL_111_KEI1_MEISYO	1				// 計ダイアログ1行目の計名称（計or合計、頁計、累計）
// midori 152503 del -->
//#define ID_COL_111_KEI1_VAL		2				// 計ダイアログ1行目の期末現在高
//#define ID_COL_111_KEI1_RISOKU	3				// 計ダイアログ1行目の期中の支払利息
//#define ID_COL_111_KEI2_MEISYO	12				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
//#define ID_COL_111_KEI2_VAL		13				// 計ダイアログ2行目の期末現在高
//#define ID_COL_111_KEI2_RISOKU	14				// 計ダイアログ2行目の期中の支払利息
// midori 152503 del <--
// midori 152503 add -->
#define ID_COL_111_KEI1_VAL		3				// 計ダイアログ1行目の期末現在高
#define ID_COL_111_KEI1_RISOKU	6				// 計ダイアログ1行目の期中の支払利息
#define ID_COL_111_KEI2_MEISYO	14				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
#define ID_COL_111_KEI2_VAL		16				// 計ダイアログ2行目の期末現在高
#define ID_COL_111_KEI2_RISOKU	19				// 計ダイアログ2行目の期中の支払利息
// midori 152503 add <--

//////////////////////////////////////////////////////////////////
//	1レコード分の構造体
typedef struct tagRecuc_111_Kariirekin_111
{
	long	m_Seq;	// シーケンス番号
	int	m_NumPage;	// 頁番号
	BYTE	m_NumRow;	// 行番号
	BYTE	m_FgFunc;	// 特殊行フラグ
	BYTE	m_FgShow;	// 表示フラグ
	CString	m_KeiStr;	// 計
	long	m_NumGroup;	// グループ番号
	long	m_KnSeq;	// 科目（シーケンス番号）
	long	m_KnOrder;	// 科目（並び順）
	CString	m_KnName;	// 科目（名称）
	long	m_AdSeq;	// 取引先（シーケンス番号）
	long	m_AdOrder;	// 取引先（並び順）
	CString	m_AdName1;	// 借入先（名称１）
	CString	m_AdName2;	// 借入先（名称２）
	CString	m_AdAdd1;	// 借入先（所在地１）
	CString	m_AdAdd2;	// 借入先（所在地２）
	CString	m_Kankei;	// 法人・代表者との関係
	CString	m_Val;	// 期末現在高
	CString	m_Risoku;	// 期中の支払利子額
	CString	m_Rate;	// 利率
	CString	m_Riyuu;	// 借入理由
	CString	m_Naiyou;	// 担保の内容
	int		m_ShowKeiZero;	// ０円表示フラグ
// midori 152137 add -->
	CString	m_KnKana;	// 科目
	CString	m_AdKana1;	// 借入先（銀行）
	CString	m_AdKana2;	// 借入先（支店）
// midori 152137 add <--
} REC_UC_111_KARIIREKIN, *P_REC_UC_111_KARIIREKIN;

class CfrmUc111Kariirekin: public CfrmUc000Common
{
	DECLARE_DYNCREATE( CfrmUc111Kariirekin )

public:
	CfrmUc111Kariirekin();
	virtual	~CfrmUc111Kariirekin();
	void	virSetInitialValue();

private:
	void	virRowButtonCopyProc();
	int		virRecBufSaveData( int inPage, int inRow );
	int		virRecBufSaveDataForPasteInsert( int inPage, int inRow );
	int		virRecBufSaveDataForPaste( int inPage, int inRow );
	int		RecBufClearData( P_REC_UC_111_KARIIREKIN inRecData );
	int		RecBufSaveDataSub( int inPage, int inRow, P_REC_UC_111_KARIIREKIN inSaveData );
	int		RecBufSetData( CdbUc111Kariirekin* inTbl );
	int		RecBufClearAllData();

	// 1ページ分のデータ格納用配列
	// 配列の添え字と、行番号を合わせるため、最大行+1とした
	REC_UC_111_KARIIREKIN	m_ReadData[ID_ROWKOBETU_111 + 1];
	REC_UC_111_KARIIREKIN	m_CopyData;		// 行コピー用バッファ
	int	m_nCopyFlag;					// コピーフラグ（0：コピーデータがない／1：コピーデータがある）

public:
	enum{ IDD = IDD_YOUSHIKI111 };
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
	void	UpdateControlRowData( CdbUc111Kariirekin* inTbl );
	void	UpdateControlRowTotal( CdbUc111Kariirekin* inTbl );
	CdbUc000Common*	virGetDataTableObject();
	void	virGetCurrentInfo(CURRENT_INFO* uCurInfo);
	void	virFncCellCopyProc(CdbUc000Common* rsData);									// F6 :項目複写
	int		virFncReferenceGetType(int intCol, int* intGrSeq, long* lngItemSeq, int* intAdd1, int* intAdd2, int* intAdd1Col, int* intAdd2Col, REFERENCE_INFO* pudReference = 0);	// F9 :参照
	void	virFncReferenceSetData(CdlgReference* dlgRef, int intRefType, int intCol);	// F9 :参照
	void	virFncReferenceSetICSData(CdlgAddressSearch* dlgAddd);						// F9 :参照
	void	virFncReferenceUpdateTbl(int intRefType, int nRefMode = 0);					// F9 :参照
// 要通知案件修正 21/07/09 add -->
	void	virActivateFrameK(UINT nState, CFrameWnd* pDeactivateFrame);
	void	virKamokuAddCancel();
// 要通知案件修正 21/07/09 add <--

	int		GetFocusPosition( int nChar );	// フォーカス移動位置取得関数
	int		GetTotalRowIndex( int nChar , int intIndex , int intNext );	// 行移動制御関数
	BOOL	CheckShiftKey();

	void	virTblGetIkkatuSumValue(CdbUc000Common* rsData, int nFgShow, int nGroup, CALCKEI_INFO* uCalcKei);
	void	virTblEditIkkatuLine(int nType, CdbUc000Common* rsData, CString strName, CALCKEI_INFO uCalcKei, SORTKAMOKU_INFO uKamoku);
// midori 156189,156190,156191 del -->
//// midori 190505 add -->
//	void	virTblEditKamokuLine(CdbUc000Common* rsData, int pPage, int pRow);
//// midori 190505 add <--
// midori 156189,156190,156191 del <--
	int		virTblSortSubSortProc(CdlgSort* pCdlgSort, CdbUc000Common* rsData);
	void	virTblGetKamokuInfo(CdbUc000Common* rsData, SORTKAMOKU_INFO* uKamoku);
	void	virUpdateTitle();
	void	virInputDecisionEditOFF();
	void	virRendoCreateData(CdbUc000Common* rsData, RENTEMP_INFO* uRenInfo);
	void	virRendoUpdateData(CdbUc000Common* rsData, RENTEMP_INFO* uRenInfo);

	void	SetInputMode( int nRow, long nMode );
	void	SetDiagBackColor();
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
	LRESULT OnUserReDrawView(WPARAM wParam, LPARAM lParam);	// 修正No.162446
};

