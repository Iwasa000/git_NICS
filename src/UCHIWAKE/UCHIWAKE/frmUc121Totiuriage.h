//--------------------------------------------------
//	frmUc121Totiuriage.h
//--------------------------------------------------

#pragma once

#include "frmUc000Common.h"
#include "dbUc121Totiuriage.h"
#include "prtUc121Totiuriage.h"

/****************************************************************
	「土地売上」関連
*****************************************************************/
#define	ID_ROWNORMAL_121	10		// 通常時：１頁内の合計行を含む全体の行数
#define	ID_ROWKOBETU_121	10		// 個別時：１頁内の合計行を含む全体の行数
#define	ID_ROWKEI_121	0		// 帳表固定の合計行数（1:合計あり，0:なし）
#define	ID_COLMAX_121	29		// １行内のコントロール個数
#define	ID_COLMAXDATA_121	14		// １行内の入力コントロール数
#define	ID_CTLSTARTINDEX_121	11		// １行目の入力項目の先頭インデックス番号
#define ID_T1STARTNORMAL_121	290	// 通常時：合計行①の先頭インデックス番号
#define ID_T2STARTNORMAL_121	319	// 通常時：合計行②の先頭インデックス番号
#define ID_T1STARTKOBETU_121	290	// 個別時：合計行①の先頭インデックス番号
#define ID_T2STARTKOBETU_121	319	// 個別時：合計行②の先頭インデックス番号

// 「土地売上」テーブル名称
//	正式版では"CdbUc121Totiuriage"のメンバー変数で参照可能としたい
#define	TBL_NAME_121		_T("uc_121_Totiuriage")

//////////////////////////////////////////////////////////////////
//	「土地売上」の内訳書列番号（帳表入力画面で使用）
typedef enum {
	ID_COL_121_KUBUN = 1,	// 1	区分
	ID_COL_121_AD2ADD1,	// 2	商品の所在地1
	ID_COL_121_AD2ADD2,	// 3	商品の所在地2
	ID_COL_121_HDNAME,	// 4	地目
	ID_COL_121_AREA,	// 5	総面積
	ID_COL_121_SPDATE,	// 6	売上年月
	ID_COL_121_ADNAME1,	// 7	氏名1
	ID_COL_121_ADADD1,	// 8	住所1
	ID_COL_121_ADADD2,	// 9	住所2
	ID_COL_121_AREA2,	// 10	売上面積
	ID_COL_121_VAL,		// 11	売上金額
	ID_COL_121_VAL2,	// 12	売上金額
	ID_COL_121_SPDATE2,	// 13	売上商品の取得年(元号)
	ID_COL_121_SPDATE3	// 14	売上商品の取得年(年)
} EnumIdFormCol121;

//////////////////////////////////////////////////////////////////
//	「土地売上」の内訳書_集計項目
#define	ID_CALC_121_MAX		2	// 集計フィールド件数
#define	ID_CALC_121_FIELD1	_T("Val")	// 集計フィールド名称1
#define	ID_CALC_121_FIELD2	_T("Val2")	// 集計フィールド名称2

//////////////////////////////////////////////////////////////////
//	「土地売上」の内訳書_計ダイアグラムの入力インデックス
#define ID_COL_121_KEI1_MEISYO	1				// 計ダイアログ1行目の計名称（計or合計、頁計、累計）
// midori 152503 del -->
//#define ID_COL_121_KEI1_VAL		6				// 計ダイアログ1行目の売上金額
//#define ID_COL_121_KEI1_VAL2	7				// 計ダイアログ1行目の売上金額
//#define ID_COL_121_KEI2_MEISYO	12				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
//#define ID_COL_121_KEI2_VAL		17				// 計ダイアログ2行目の売上金額
//#define ID_COL_121_KEI2_VAL2	18				// 計ダイアログ2行目の売上金額
// midori 152503 del <--
// midori 152503 add -->
#define ID_COL_121_KEI1_VAL		8				// 計ダイアログ1行目の売上金額
#define ID_COL_121_KEI1_VAL2	9				// 計ダイアログ1行目の売上金額
#define ID_COL_121_KEI2_MEISYO	14				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
#define ID_COL_121_KEI2_VAL		21				// 計ダイアログ2行目の売上金額
#define ID_COL_121_KEI2_VAL2	22				// 計ダイアログ2行目の売上金額
// midori 152503 add <--

//////////////////////////////////////////////////////////////////
//	1レコード分の構造体
typedef struct tagRecuc_121_Totiuriage_121
{
	long	m_Seq;			// シーケンス番号
	int		m_NumPage;		// 頁番号
	BYTE	m_NumRow;		// 行番号
	BYTE	m_FgFunc;		// 特殊行フラグ
	BYTE	m_FgShow;		// 表示フラグ
	CString	m_KeiStr;		// 計
	long	m_NumGroup;		// グループ番号
	long	m_KnSeq;		// 区分のSeq番号
	long	m_KnOrder;		// 区分のOrder番号
	CString	m_KnName;		// 区分の文言
	CString	m_HdName;		// 地目
	CString	m_Area;			// 総面積
	long	m_SpDate;		// 売上年月
	long	m_AdSeq;		// 取引先（シーケンス番号）
	long	m_AdOrder;		// 取引先（並び順）
	CString	m_AdName1;		// 売上先（名称１）
	CString	m_AdName2;		// 売上先（名称２）
	CString	m_AdAdd1;		// 売上先（所在地：上段）
	CString	m_AdAdd2;		// 売上先（所在地：下段）
	CString	m_Area2;		// 売上面積
	CString	m_Val;			// 売上金額（上段）
	CString	m_Val2;			// 売上金額（下段）
	long	m_SpDate2;		// 売上商品の取得年
	CString	m_Ad2Add1;		// 商品の所在地（上段）
	CString	m_Ad2Add2;		// 商品の所在地（下段）
	int		m_ShowKeiZero;	// ０円表示フラグ
// midori 152137 add -->
	CString	m_KnKana;		// 区分
// midori 152137 add <--
	CString	m_InvNo;		// 登録番号(法人番号)	インボイス登録番号追加対応_23/10/20 add
} REC_UC_121_TOTIURIAGE, *P_REC_UC_121_TOTIURIAGE;

class CfrmUc121Totiuriage: public CfrmUc000Common
{
	DECLARE_DYNCREATE( CfrmUc121Totiuriage )

public:
	CfrmUc121Totiuriage();
	virtual	~CfrmUc121Totiuriage();
	void	virSetInitialValue();

private:
	void	virRowButtonCopyProc();
	int		virRecBufSaveData( int inPage, int inRow );
	int		virRecBufSaveDataForPasteInsert( int inPage, int inRow );
	int		virRecBufSaveDataForPaste( int inPage, int inRow );
	int		RecBufClearData( P_REC_UC_121_TOTIURIAGE inRecData );
	int		RecBufSaveDataSub( int inPage, int inRow, P_REC_UC_121_TOTIURIAGE inSaveData );
	int		RecBufSetData( CdbUc121Totiuriage* inTbl );
	int		RecBufClearAllData();

	// 1ページ分のデータ格納用配列
	// 配列の添え字と、行番号を合わせるため、最大行+1とした
	REC_UC_121_TOTIURIAGE	m_ReadData[ID_ROWKOBETU_121 + 1];
	REC_UC_121_TOTIURIAGE	m_CopyData;		// 行コピー用バッファ
	int	m_nCopyFlag;					// コピーフラグ（0：コピーデータがない／1：コピーデータがある）
	int	m_nOldCol;								// 旧列番号
	int	m_nOld_combo;								// 売上商品の取得年(元号)

public:
	enum{ IDD = IDD_YOUSHIKI121 };
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
	CICSDiag	m_Diag3;										// 計なしの明細ダイアログ
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
	void	InitialControlSpecialRow( int intRow, int intFgFunc );
	void	virUpdateControlTitle();
	void	UpdateControlPage();
	BOOL	virUpdateControlTblData();
// midori 161108 add -->
	BOOL	virUpdateControlTblData2();
// midori 161108 add <--
	void	UpdateControlRowData( CdbUc121Totiuriage* inTbl );
	void	UpdateControlRowTotal( CdbUc121Totiuriage* inTbl );
	CdbUc000Common*	virGetDataTableObject();
	void	virGetCurrentInfo(CURRENT_INFO* uCurInfo);
	void	virFncCellCopyProc(CdbUc000Common* rsData);									// F6 :項目複写
	int		virFncReferenceGetType(int intCol, int* intGrSeq, long* lngItemSeq, int* intAdd1, int* intAdd2, int* intAdd1Col, int* intAdd2Col, REFERENCE_INFO* pudReference = 0);	// F9 :参照
	void	virFncReferenceSetData(CdlgReference* dlgRef, int intRefType, int intCol);	// F9 :参照
	void	virFncReferenceUpdateTbl(int intRefType, int nRefMode = 0);					// F9 :参照
	void	virFncReferenceSetICSData(CdlgAddressSearch* dlgAddd);						// F9 :参照
// 修正No.157633_元号日付 add -->
	void	virActivateFrame(UINT nState, CFrameWnd* pDeactivateFrame);
// 修正No.157633_元号日付 add <--
// 要通知案件修正 21/07/09 add -->
	void	virActivateFrameK(UINT nState, CFrameWnd* pDeactivateFrame);
	void	virKamokuAddCancel();
// 要通知案件修正 21/07/09 add <--

	int		GetFocusPosition( int nChar );	// フォーカス移動位置取得関数
	int		GetTotalRowIndex( int nChar , int intIndex , int intNext );	// 行移動制御関数
	BOOL	CheckShiftKey();

	int		virTblSortSubSortProc(CdlgSort* pCdlgSort, CdbUc000Common* rsData);
	void	virTblGetKamokuInfo(CdbUc000Common* rsData, SORTKAMOKU_INFO* uKamoku);
	void	ConvertSpDate2( int nIndex );
	void	virUpdateTitle();
	void	virInputDecisionEditOFF();

	void	SetInputMode( int nRow, long nMode );
	void	SetDiagBackColor();

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
// midori 151405 add -->
	int		CursorControl(short nChar, int sw);
// midori 151405 add <--

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
// No.200601 add -->
	void TitleColorChg(void);
// No.200601 add <--

// 修正No.157633_元号日付 add -->
	void Date_ChkAndSet(short, int, short);
// 修正No.157633_元号日付 add <--
	LRESULT OnUserReDrawView(WPARAM wParam, LPARAM lParam);	// 修正No.162446
};
