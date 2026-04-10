//--------------------------------------------------
//	frmUc081Siharaitegata.h
//--------------------------------------------------

#pragma once

#include "frmUc000Common.h"
#include "dbUc081Siharaitegata.h"
#include "prtUc081Siharaitegata.h"

/****************************************************************
	「支払手形」関連
*****************************************************************/
#define	ID_ROWNORMAL_081		24		// 通常時：１頁内の合計行を含む全体の行数
#define	ID_ROWKOBETU_081		24		// 個別時：１頁内の合計行を含む全体の行数
#define	ID_ROWKEI_081			1		// 帳表固定の合計行数（1:合計あり，0:なし）
#define	ID_COLMAX_081			16		// １行内のコントロール個数
#define	ID_COLMAXDATA_081		7		// １行内の入力コントロール数
#define	ID_CTLSTARTINDEX_081	7		// １行目の入力項目の先頭インデックス番号
#define ID_T1STARTNORMAL_081	368		// 通常時：合計行①の先頭インデックス番号
#define ID_T2STARTNORMAL_081	384		// 通常時：合計行②の先頭インデックス番号
#define ID_T1STARTKOBETU_081	368		// 個別時：合計行①の先頭インデックス番号
#define ID_T2STARTKOBETU_081	384		// 個別時：合計行②の先頭インデックス番号

// 「支払手形」テーブル名称
//	正式版では"CdbUc081Siharaitegata"のメンバー変数で参照可能としたい
#define	TBL_NAME_081		_T("uc_081_Siharaitegata")

//////////////////////////////////////////////////////////////////
//	「支払手形」の内訳書列番号（帳表入力画面で使用）
typedef enum {
	ID_COL_081_ADNAME1 = 1,				// 1	支払先1
	ID_COL_081_SPDATE,					// 2	振出年月日
	ID_COL_081_PMDATE,					// 3	支払期日
	ID_COL_081_BKNAME1,					// 4	支払銀行名
	ID_COL_081_BKNAME2,					// 5	支払支店名
	ID_COL_081_VAL,						// 6	金額
	ID_COL_081_TEKI,					// 7	摘要
} EnumIdFormCol081;

//////////////////////////////////////////////////////////////////
//	「支払手形」の内訳書_集計項目
#define	ID_CALC_081_MAX		1			// 集計フィールド件数
#define	ID_CALC_081_FIELD1	_T("Val")	// 集計フィールド名称1

//////////////////////////////////////////////////////////////////
//	「支払手形」の内訳書_計ダイアグラムの入力インデックス
#define ID_COL_081_KEI1_MEISYO	0				// 計ダイアログ1行目の計名称（計or合計、頁計、累計）
// midori 152503 del -->
//#define ID_COL_081_KEI1_VAL		3				// 計ダイアログ1行目の金額
//#define ID_COL_081_KEI2_MEISYO	7				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
//#define ID_COL_081_KEI2_VAL		10				// 計ダイアログ2行目の金額
// midori 152503 del <--
// midori 152503 add -->
#define ID_COL_081_KEI1_VAL		4				// 計ダイアログ1行目の金額
#define ID_COL_081_KEI2_MEISYO	8				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
#define ID_COL_081_KEI2_VAL		12				// 計ダイアログ2行目の金額
// midori 152503 add <--

//////////////////////////////////////////////////////////////////
//	1レコード分の構造体
typedef struct tagRecuc_081_Siharaitegata_081
{
	long	m_Seq;					// シーケンス番号
	int		m_NumPage;				// 頁番号
	BYTE	m_NumRow;				// 行番号
	BYTE	m_FgFunc;				// 特殊行フラグ
	BYTE	m_FgShow;				// 表示フラグ
	CString	m_KeiStr;				// 計
	long	m_NumGroup;				// グループ番号
	long	m_AdSeq;				// 支払先：取引先（シーケンス番号）
	long	m_AdOrder;				// 支払先：取引先（並び順）
	CString	m_AdName1;				// 支払先（名称：上段）
	CString	m_AdName2;				// 支払先（名称：下段）
	long	m_SpDate;				// 振出年月日
	long	m_PmDate;				// 支払期日
	long	m_BkSeq;				// 金融機関（シーケンス番号）
	long	m_BkOrder;				// 金融機関（並び順）
	CString	m_BkName1;				// 支払銀行名（銀行名）
	CString	m_BkName2;				// 支払銀行名（支店名）
	CString	m_Val;					// 金額
	CString	m_Teki;					// 摘要
	int		m_ShowKeiZero;			// ０円表示フラグ
// midori 152137 add -->
	CString	m_AdKana;				// 支払先
// midori 152137 add <--
	CString	m_InvNo;				// 登録番号(法人番号)	インボイス登録番号追加対応_23/10/20 add
// 改良No.21-0086,21-0529 add -->
	long	m_KnSeq;				// 科目シーケンス
	long	m_KnOrder;				// 科目オーダー
	CString	m_KnName;				// 科目名
	CString	m_KnKana;				// 科目カナ
// 改良No.21-0086,21-0529 add <--
} REC_UC_081_SIHARAITEGATA, *P_REC_UC_081_SIHARAITEGATA;

class CfrmUc081Siharaitegata: public CfrmUc000Common
{
	DECLARE_DYNCREATE( CfrmUc081Siharaitegata )

public:
	CfrmUc081Siharaitegata();
	virtual	~CfrmUc081Siharaitegata();
	void	virSetInitialValue();

private:
	void	virRowButtonCopyProc();
	int		virRecBufSaveData( int inPage, int inRow );
	int		virRecBufSaveDataForPasteInsert( int inPage, int inRow );
	int		virRecBufSaveDataForPaste( int inPage, int inRow );
	int		RecBufClearData( P_REC_UC_081_SIHARAITEGATA inRecData );
	int		RecBufSaveDataSub( int inPage, int inRow, P_REC_UC_081_SIHARAITEGATA inSaveData );
	int		RecBufSetData( CdbUc081Siharaitegata* inTbl );
	int		RecBufClearAllData();

	// 1ページ分のデータ格納用配列
	// 配列の添え字と、行番号を合わせるため、最大行+1とした
	REC_UC_081_SIHARAITEGATA	m_ReadData[ID_ROWKOBETU_081 + 1];
	REC_UC_081_SIHARAITEGATA	m_CopyData;		// 行コピー用バッファ
	int	m_nCopyFlag;					// コピーフラグ（0：コピーデータがない／1：コピーデータがある）

public:
	enum{ IDD = IDD_YOUSHIKI081 };
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
	void	UpdateControlRowData( CdbUc081Siharaitegata* inTbl );
	void	UpdateControlRowTotal( CdbUc081Siharaitegata* inTbl );
	CdbUc000Common*	virGetDataTableObject();
	void	virGetCurrentInfo(CURRENT_INFO* uCurInfo);
	void	virFncCellCopyProc(CdbUc000Common* rsData);									// F6 :項目複写
	int		virFncReferenceGetType(int intCol, int* intGrSeq, long* lngItemSeq, int* intAdd1, int* intAdd2, int* intAdd1Col, int* intAdd2Col, REFERENCE_INFO* pudReference = 0);	// F9 :参照
	void	virFncReferenceSetData(CdlgReference* dlgRef, int intRefType, int intCol);	// F9 :参照
	// void	virFncReferenceSetICSData(CdlgAddressSearch* dlgAddd);						// F9 :参照
	void	virFncReferenceUpdateTbl(int intRefType, int nRefMode = 0);					// F9 :参照

	int		GetFocusPosition( int nChar );	// フォーカス移動位置取得関数
	int		GetTotalRowIndex( int nChar , int intIndex , int intNext );	// 行移動制御関数
	BOOL	CheckShiftKey();

	void	virTblEditIkkatuLine(int nType, CdbUc000Common* rsData, CString strName, CALCKEI_INFO uCalcKei, SORTKAMOKU_INFO uKamoku);
	int		virTblSortSubSortProc(CdlgSort* pCdlgSort, CdbUc000Common* rsData);
	void	virUpdateTitle();
	void	virInputDecisionEditOFF();

	void	SetInputMode( int nRow, long nMode );
	void	SetDiagBackColor();
	void	virRendoCreateData(CdbUc000Common* rsData, RENTEMP_INFO* uRenInfo);
	void	virRendoUpdateData(CdbUc000Common* rsData, RENTEMP_INFO* uRenInfo);
// midori 151405 add -->
	int		CursorControl(short nChar, int sw);
// midori 151405 add <--
// 20-0873 add -->
	int		virTblSortGetTorihikisakiCode( CdbUc000Common* rsData );
	CString	virTblSortGetTorihikisakiName( CdbUc000Common* rsData );
// 20-0873 add <--

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
// midori 151405 add -->
public:
	void VScrollYoushikidiag1(short mode);
// midori 151405 add <--
	LRESULT OnUserReDrawView(WPARAM wParam, LPARAM lParam);	// 修正No.162446
};

