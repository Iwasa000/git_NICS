//--------------------------------------------------
//	frmUc181Sonota2.h
//--------------------------------------------------

#pragma once

#include "frmUc000Common.h"
#include "dbUc181Sonota2.h"
#include "prtUc181Sonota2.h"

/****************************************************************
	「その他2」関連
*****************************************************************/
// midori 170504 del -->
//#define	ID_ROWNORMAL_181		27		// 通常時：１頁内の合計行を含む全体の行数
//#define	ID_ROWKOBETU_181		27		// 個別時：１頁内の合計行を含む全体の行数
//#define	ID_ROWKEI_181			1		// 帳表固定の合計行数（1:合計あり，0:なし）
// midori 170504 del <--
#define	ID_COLMAX_181			8		// １行内のコントロール個数
#define	ID_COLMAXDATA_181		4		// １行内の入力コントロール数
#define	ID_CTLSTARTINDEX_181	4		// １行目の入力項目の先頭インデックス番号
#define ID_T1STARTNORMAL_181	208		// 通常時：合計行①の先頭インデックス番号
#define ID_T2STARTNORMAL_181	216		// 通常時：合計行②の先頭インデックス番号
#define ID_T1STARTKOBETU_181	208		// 個別時：合計行①の先頭インデックス番号
#define ID_T2STARTKOBETU_181	216		// 個別時：合計行②の先頭インデックス番号

// 「その他2」テーブル名称
//	正式版では"CdbUc181Sonota2"のメンバー変数で参照可能としたい
#define	TBL_NAME_181		_T("uc_181_Sonota2")

//////////////////////////////////////////////////////////////////
//	「その他2」の内訳書列番号（帳表入力画面で使用）
typedef enum {
	ID_COL_181_KNORDER = 1,				// 1	科目
	ID_COL_181_ITEM,					// 2	内訳（摘要）
	ID_COL_181_VAL,						// 3	金額
	ID_COL_181_TEKI,					// 4	備考
} EnumIdFormCol181;

// midori 170504 del -->
////////////////////////////////////////////////////////////////////
////	預貯金等の入力コントロールの最大桁数
//#define MAX_KETA_181_ITEM		40		//	項目
//#define MAX_KETA_181_TEKI		40		//	摘要
// midori 170504 del <--

//////////////////////////////////////////////////////////////////
//	「その他2」の内訳書_集計項目
#define	ID_CALC_181_MAX		1			// 集計フィールド件数
#define	ID_CALC_181_FIELD1	_T("Val")	// 集計フィールド名称1

//////////////////////////////////////////////////////////////////
//	「その他2」の内訳書_金額欄の最大桁数
// midori 170504 del -->
//#define	ID_VAL_181_DATA		11			// 金額nのデータ行の最大桁数
// midori 170504 del <--
#define	ID_VAL_181_TOTAL		12			// 金額の計行の最大桁数
// No.200903 add -->
#define	ID_VAL_181_K_TOTAL		13			// 金額の計行の最大桁数
// No.200903 add <--

//////////////////////////////////////////////////////////////////
//	「売上高」の内訳書_計ダイアグラムの入力インデックス
#define ID_COL_181_KEI1_MEISYO	0				// 計ダイアログ1行目の計名称（計or合計、頁計、累計）
// midori 152503 del -->
//#define ID_COL_181_KEI1_VAL		2				// 計ダイアログ1行目の金額
//#define ID_COL_181_KEI2_MEISYO	5				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
//#define ID_COL_181_KEI2_VAL		7				// 計ダイアログ2行目の金額
// midori 152503 del <--
// midori 152503 add -->
#define ID_COL_181_KEI1_VAL		3				// 計ダイアログ1行目の金額
#define ID_COL_181_KEI2_MEISYO	6				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
#define ID_COL_181_KEI2_VAL		9				// 計ダイアログ2行目の金額
// midori 152503 add <--

//////////////////////////////////////////////////////////////////
//	1レコード分の構造体
typedef struct tagRecuc_181_Sonota2_181
{
	long	m_Seq;						// シーケンス番号
	int		m_NumPage;					// 頁番号
	BYTE	m_NumRow;					// 行番号
	BYTE	m_FgFunc;					// 特殊行フラグ
	BYTE	m_FgShow;					// 表示フラグ
	CString	m_KeiStr;					// 計
	long	m_NumGroup;					// グループ番号
	int		m_FormSeq;					// 様式シーケンス番号
	long	m_KnSeq;					// 科目（シーケンス番号）
	long	m_KnOrder;					// 科目（並び順）
	CString	m_KnName;					// 科目（名称）
	CString	m_Item;						// 項目
	CString	m_Val;						// 期末現在高
	CString	m_Teki;						// 摘要
	int		m_ShowKeiZero;				// ０円表示フラグ
// midori 152137 add -->
	CString	m_KnKana;					// 科目
// midori 152137 add <--
} REC_UC_181_SONOTA2, *P_REC_UC_181_SONOTA2;

class CfrmUc181Sonota2: public CfrmUc000Common
{
	DECLARE_DYNCREATE( CfrmUc181Sonota2 )

public:
	CfrmUc181Sonota2();
	virtual	~CfrmUc181Sonota2();
	void	virSetInitialValue();

private:
	void	virRowButtonCopyProc();
	int		virRecBufSaveData( int inPage, int inRow );
	int		virRecBufSaveDataForPasteInsert( int inPage, int inRow );
	int		virRecBufSaveDataForPaste( int inPage, int inRow );
	int		RecBufClearData( P_REC_UC_181_SONOTA2 inRecData );
	int		RecBufSaveDataSub( int inPage, int inRow, P_REC_UC_181_SONOTA2 inSaveData );
	int		RecBufSetData( CdbUc181Sonota2* inTbl );
	int		RecBufClearAllData();

	// 1ページ分のデータ格納用配列
	// 配列の添え字と、行番号を合わせるため、最大行+1とした
	REC_UC_181_SONOTA2	m_ReadData[ID_ROWKOBETU_181 + 1];
	REC_UC_181_SONOTA2	m_CopyData;		// 行コピー用バッファ
	int	m_nCopyFlag;					// コピーフラグ（0：コピーデータがない／1：コピーデータがある）

public:
	enum{ IDD = IDD_YOUSHIKI181 };
	CICSDiag	m_TitleDiag;		//	タイトル
	CICSDiag	m_PageDiag;			//	頁
	CICSDiag	m_NumberDiag;		//	番号
// midori 161108 add -->
	CICSDiag	m_SaveDataDiag;
// midori 161108 add <--
	CICSDiag*	m_Diag;				// m_Diag1とm_Diag2を受けるポインタ
	CICSDiag	m_Diag1;			// 計が1行の明細ダイアログ
	CICSDiag	m_Diag2;			// 計が2行の明細ダイアログ
	CICSDiag*	m_DiagKei;			// m_DiagKei1とm_DiagKei2を受けるポインタ
	CICSDiag	m_DiagKei1;			// 計が1行の計専用ダイアグラム
	CICSDiag	m_DiagKei2;			// 計が2行専用のダイアグラム
//	CICSDiag m_Diag3				// 計あり様式なので、計が0行は存在しない
	CICSDiag	m_HeadDiag;			//	ヘッダ

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
	void	UpdateControlRowData( CdbUc181Sonota2* inTbl );
	void	UpdateControlRowTotal( CdbUc181Sonota2* inTbl );
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

	BOOL	UpdateControlItem();
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
	void TerminationTopicdiag1(short index, short nChar, short length, LPUNKNOWN data);
	void EditOFFTopicdiag1(short index);
	void EditONTopicdiag1(short index);
	void FocusTopicdiag1();

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
