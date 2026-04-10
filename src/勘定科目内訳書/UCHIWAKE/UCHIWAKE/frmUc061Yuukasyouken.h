//--------------------------------------------------
//	frmUc061Yuukasyouken.h
//--------------------------------------------------

#pragma once

#include "frmUc000Common.h"
#include "dbUc061Yuukasyouken.h"
#include "prtUc061Yuukasyouken.h"

/****************************************************************
	「有価証券」関連
*****************************************************************/
#define	ID_ROWNORMAL_061	16		// 通常時：１頁内の合計行を含む全体の行数
#define	ID_ROWKOBETU_061	16		// 個別時：１頁内の合計行を含む全体の行数
#define	ID_ROWKEI_061	1		// 帳表固定の合計行数（1:合計あり，0:なし）
#define	ID_COLMAX_061	32		// １行内のコントロール個数
#define	ID_COLMAXDATA_061	15		// １行内の入力コントロール数
#define	ID_CTLSTARTINDEX_061	12		// １行目の入力項目の先頭インデックス番号
#define ID_T1STARTNORMAL_061	480	// 通常時：合計行①の先頭インデックス番号
#define ID_T2STARTNORMAL_061	512	// 通常時：合計行②の先頭インデックス番号
#define ID_T1STARTKOBETU_061	480	// 個別時：合計行①の先頭インデックス番号
#define ID_T2STARTKOBETU_061	512	// 個別時：合計行②の先頭インデックス番号

// 「有価証券」テーブル名称
//	正式版では"CdbUc061Yuukasyouken"のメンバー変数で参照可能としたい
#define	TBL_NAME_061		_T("uc_061_Yuukasyouken")

//////////////////////////////////////////////////////////////////
//	「有価証券」の内訳書列番号（帳表入力画面で使用）
typedef enum {
	ID_COL_061_KUBUN = 1,	// 1	区分
	ID_COL_061_SYURUI,	// 2	種類
	ID_COL_061_SUURYOU,	// 3	数量
	ID_COL_061_TANNI,	// 4	単位
	ID_COL_061_VAL,		// 5	金額(1)
	ID_COL_061_VAL2,	// 6	金額(2)
	ID_COL_061_SPDATE,	// 7	異動年月日
	ID_COL_061_RIYUU,	// 8	異動事由
	ID_COL_061_SUURYOU2,	// 9	数量2
	ID_COL_061_TANNI2,	// 10	単位2
	ID_COL_061_VAL3,	// 11	金額
	ID_COL_061_ADNAME1,	// 12	売却(買入)先の名称1(氏名)
	ID_COL_061_ADADD1,	// 14	売却(買入)先の所在地1(住所)
	ID_COL_061_ADADD2,	// 15	売却(買入)先の所在地2(住所)
	ID_COL_061_TEKI,	// 16	摘要
} EnumIdFormCol061;

//////////////////////////////////////////////////////////////////
//	「有価証券」の内訳書_集計項目
#define	ID_CALC_061_MAX		2	// 集計フィールド件数
#define	ID_CALC_061_FIELD1	_T("Val2")	// 集計フィールド名称1
#define	ID_CALC_061_FIELD2	_T("Val3")	// 集計フィールド名称2

//////////////////////////////////////////////////////////////////
//	「有価証券」の内訳書_計ダイアグラムの入力インデックス
#define ID_COL_061_KEI1_MEISYO	0				// 計ダイアログ1行目の計名称（計or合計、頁計、累計）
// midori 152503 del -->
//#define ID_COL_061_KEI1_VAL2	2				// 計ダイアログ1行目の金額１
//#define ID_COL_061_KEI1_VAL3	5				// 計ダイアログ1行目の金額２
//#define ID_COL_061_KEI2_MEISYO	13				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
//#define ID_COL_061_KEI2_VAL2	15				// 計ダイアログ2行目の金額１
//#define ID_COL_061_KEI2_VAL3	18				// 計ダイアログ2行目の金額２
// midori 152503 del <--
// midori 152503 add -->
#define ID_COL_061_KEI1_VAL2	3				// 計ダイアログ1行目の金額１
#define ID_COL_061_KEI1_VAL3	7				// 計ダイアログ1行目の金額２
#define ID_COL_061_KEI2_MEISYO	15				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
#define ID_COL_061_KEI2_VAL2	18				// 計ダイアログ2行目の金額１
#define ID_COL_061_KEI2_VAL3	22				// 計ダイアログ2行目の金額２
// midori 152503 add <--

//////////////////////////////////////////////////////////////////
//	1レコード分の構造体
typedef struct tagRecuc_061_Yuukasyouken_061
{
	long	m_Seq;	// シーケンス番号
	int	m_NumPage;	// 頁番号
	BYTE	m_NumRow;	// 行番号
	BYTE	m_FgFunc;	// 特殊行フラグ
	BYTE	m_FgShow;	// 表示フラグ
	CString	m_KeiStr;	// 計
	long	m_NumGroup;	// グループ番号
	long	m_KnSeq;	// 区分のSeq番号
	long	m_KnOrder;	// 区分のOrder番号
	CString	m_KnName;	// 区分の文言
	CString	m_Syurui;	// 種類・銘柄
	CString	m_Suuryou;	// 期末現在高（数量）
	CString	m_Tanni;	// 期末現在高（単位）
	CString	m_Val;	// 期末現在高（金額上段）
	CString	m_Val2;	// 期末現在高（金額下段）
	long	m_SpDate;	// 期中増の明細（異動年月日）
	CString	m_Riyuu;	// 期中増の明細（異動事由）
	CString	m_Suuryou2;	// 期中増の明細（数量）
	CString	m_Tanni2;	// 期中増の明細（単位）
	CString	m_Val3;	// 期中増の明細（金額）
	long	m_AdSeq;	// 取引先（シーケンス番号）
	long	m_AdOrder;	// 取引先（並び順）
	CString	m_AdName1;	// 売却先（名称：上段）
	CString	m_AdName2;	// 売却先（名称：下段）
	CString	m_AdAdd1;	// 売却先（所在地：上段）
	CString	m_AdAdd2;	// 売却先（所在地：下段）
	CString	m_Teki;	// 摘要
	int		m_ShowKeiZero;	// ０円表示フラグ
// midori 152137 add -->
	CString	m_KnKana;	// 区分
// midori 152137 add <--
} REC_UC_061_YUUKASYOUKEN, *P_REC_UC_061_YUUKASYOUKEN;

class CfrmUc061Yuukasyouken: public CfrmUc000Common
{
	DECLARE_DYNCREATE( CfrmUc061Yuukasyouken )

public:
	CfrmUc061Yuukasyouken();
	virtual	~CfrmUc061Yuukasyouken();
	void	virSetInitialValue();

private:
	void	virRowButtonCopyProc();
	int		virRecBufSaveData( int inPage, int inRow );
	int		virRecBufSaveDataForPasteInsert( int inPage, int inRow );
	int		virRecBufSaveDataForPaste( int inPage, int inRow );
	int		RecBufClearData( P_REC_UC_061_YUUKASYOUKEN inRecData );
	int		RecBufSaveDataSub( int inPage, int inRow, P_REC_UC_061_YUUKASYOUKEN inSaveData );
	int		RecBufSetData( CdbUc061Yuukasyouken* inTbl );
	int		RecBufClearAllData();

	// 1ページ分のデータ格納用配列
	// 配列の添え字と、行番号を合わせるため、最大行+1とした
	REC_UC_061_YUUKASYOUKEN	m_ReadData[ID_ROWKOBETU_061 + 1];
	REC_UC_061_YUUKASYOUKEN	m_CopyData;		// 行コピー用バッファ
	int	m_nCopyFlag;					// コピーフラグ（0：コピーデータがない／1：コピーデータがある）

public:
	enum{ IDD = IDD_YOUSHIKI061 };
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
	void	UpdateControlRowData( CdbUc061Yuukasyouken* inTbl );
	void	UpdateControlRowTotal( CdbUc061Yuukasyouken* inTbl );
	CdbUc000Common*	virGetDataTableObject();
	void	virGetCurrentInfo(CURRENT_INFO* uCurInfo);
	void	virFncCellCopyProc(CdbUc000Common* rsData);									// F6 :項目複写
	int		virFncReferenceGetType(int intCol, int* intGrSeq, long* lngItemSeq, int* intAdd1, int* intAdd2, int* intAdd1Col, int* intAdd2Col, REFERENCE_INFO* pudReference = 0);	// F9 :参照
	void	virFncReferenceSetData(CdlgReference* dlgRef, int intRefType, int intCol);	// F9 :参照
	void	virFncReferenceUpdateTbl(int intRefType, int nRefMode = 0);					// F9 :参照
	void	virFncReferenceSetICSData(CdlgAddressSearch* dlgAddd);						// F9 :参照
// 要通知案件修正 21/07/09 add -->
	void	virActivateFrameK(UINT nState, CFrameWnd* pDeactivateFrame);
	void	virKamokuAddCancel();
// 要通知案件修正 21/07/09 add <--

	int		GetFocusPosition( int nChar );	// フォーカス移動位置取得関数
	int		GetTotalRowIndex( int nChar , int intIndex , int intNext );	// 行移動制御関数
	BOOL	CheckShiftKey();

	int		virTblSortSubSortProc(CdlgSort* pCdlgSort, CdbUc000Common* rsData);
	void	virTblGetKamokuInfo(CdbUc000Common* rsData, SORTKAMOKU_INFO* uKamoku);
	//CString	GetKubunName( int inOrder );
	void	virUpdateTitle();
	void	virInputDecisionEditOFF();

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
