//--------------------------------------------------
//	frmUc102Gensen.h
//--------------------------------------------------

#pragma once

#include "frmUc000Common.h"
#include "dbUc102Gensen.h"
#include "prtUc102Gensen.h"
#include "prtUc100KariukekinGensen.h"

/****************************************************************
	「源泉所得」関連
*****************************************************************/
#define	ID_ROWNORMAL_102	14		// 通常時：１頁内の合計行を含む全体の行数
#define	ID_ROWKOBETU_102	52		// 個別時：１頁内の合計行を含む全体の行数
#define	ID_ROWKEI_102	0		// 帳表固定の合計行数（1:合計あり，0:なし）
#define	ID_COLMAX_102	6		// １行内のコントロール個数
#define	ID_COLMAXDATA_102	3		// １行内の入力コントロール数
#define	ID_CTLSTARTINDEX_102	3		// １行目の入力項目の先頭インデックス番号
#define ID_T1STARTNORMAL_102	84	// 通常時：合計行①の先頭インデックス番号
#define ID_T2STARTNORMAL_102	90	// 通常時：合計行②の先頭インデックス番号
#define ID_T1STARTKOBETU_102	312	// 個別時：合計行①の先頭インデックス番号
#define ID_T2STARTKOBETU_102	318	// 個別時：合計行②の先頭インデックス番号

// 「源泉所得」テーブル名称
//	正式版では"CdbUc102Gensen"のメンバー変数で参照可能としたい
#define	TBL_NAME_102		_T("uc_102_Gensen")

//////////////////////////////////////////////////////////////////
//	「源泉所得」の内訳書列番号（帳表入力画面で使用）
typedef enum {
	ID_COL_102_SPDATE = 1,	// 1	年月日
	ID_COL_102_KNORDER,	// 2	取得の種類
	ID_COL_102_VAL,	// 3	期末現在高
} EnumIdFormCol102;

//////////////////////////////////////////////////////////////////
//	「源泉所得」の内訳書_集計項目
#define	ID_CALC_102_MAX		1	// 集計フィールド件数
#define	ID_CALC_102_FIELD1	_T("Val")	// 集計フィールド名称1

//////////////////////////////////////////////////////////////////
//	「源泉所得」の内訳書_計ダイアグラムの入力インデックス
#define ID_COL_102_KEI1_MEISYO	0				// 計ダイアログ1行目の計名称（計or合計、頁計、累計）
// midori 152503 del -->
//#define ID_COL_102_KEI1_VAL		2				// 計ダイアログ1行目の期末現在高
//#define ID_COL_102_KEI2_MEISYO	3				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
//#define ID_COL_102_KEI2_VAL		5				// 計ダイアログ2行目の期末現在高
// midori 152503 del <--
// midori 152503 add -->
#define ID_COL_102_KEI1_VAL		3				// 計ダイアログ1行目の期末現在高
#define ID_COL_102_KEI2_MEISYO	4				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
#define ID_COL_102_KEI2_VAL		7				// 計ダイアログ2行目の期末現在高
// midori 152503 add <--

//////////////////////////////////////////////////////////////////
//	1レコード分の構造体
typedef struct tagRecuc_102_Gensen_102
{
	long	m_Seq;	// シーケンス番号
	int	m_NumPage;	// 頁番号
	BYTE	m_NumRow;	// 行番号
	BYTE	m_FgFunc;	// 特殊行フラグ
	BYTE	m_FgShow;	// 表示フラグ
	CString	m_KeiStr;	// 計
	long	m_NumGroup;	// グループ番号
	long	m_SpDate;	// 年月日
	long	m_KnSeq;	// 所得の種類：科目（シーケンス番号）
	long	m_KnOrder;	// 所得の種類：科目（並び順）
	CString	m_KnName;	// 所得の種類：科目（名称）
	CString	m_Val;	// 期末現在高
	int		m_ShowKeiZero;
// midori 152137 add -->
	CString m_KnKana;	// 所得の種類
// midori 152137 add <--
} REC_UC_102_GENSEN, *P_REC_UC_102_GENSEN;

class CfrmUc102Gensen: public CfrmUc000Common
{
	DECLARE_DYNCREATE( CfrmUc102Gensen )

public:
	CfrmUc102Gensen();
	virtual	~CfrmUc102Gensen();
	void	virSetInitialValue();
// midori 152770 del -->
//	int		UpdateTbl102ChangeFormType();
// midori 152770 del <--
// midori 152770 add -->
	int		UpdateTbl102ChangeFormType(UCHIWAKE_INFO pInfoSv);
// midori 152770 add <--

private:
	void	virRowButtonCopyProc();
	int		virRecBufSaveData( int inPage, int inRow );
	int		virRecBufSaveDataForPasteInsert( int inPage, int inRow );
	int		virRecBufSaveDataForPaste( int inPage, int inRow );
	int		RecBufClearData( P_REC_UC_102_GENSEN inRecData );
	int		RecBufSaveDataSub( int inPage, int inRow, P_REC_UC_102_GENSEN inSaveData );
	int		RecBufSetData( CdbUc102Gensen* inTbl );
	int		RecBufClearAllData();

	// 1ページ分のデータ格納用配列
	// 配列の添え字と、行番号を合わせるため、最大行+1とした
	REC_UC_102_GENSEN	m_ReadData[ID_ROWKOBETU_102 + 1];
	REC_UC_102_GENSEN	m_CopyData;		// 行コピー用バッファ
	int	m_nCopyFlag;					// コピーフラグ（0：コピーデータがない／1：コピーデータがある）

	int	m_intT1StartIndexHide;			// 通常/個別帳表にて、非表示にしているDiagの合計行①の先頭インデックス
	int	m_intT2StartIndexHide;			// 通常/個別帳表にて、非表示にしているDiagの合計行②の先頭インデックス

public:
	enum{ IDD = IDD_YOUSHIKI102 };
	CICSDiag	m_TitleDiag;
	CICSDiag	m_PageDiag;
	CICSDiag	m_NumberDiag;
// midori 161108 add -->
	CICSDiag	m_SaveDataDiag;
// midori 161108 add <--
	CICSDiag	m_HeadDiag;
	CICSDiag	m_DiagNormal;
	CICSDiag	m_DiagKobetu;
	CICSDiag	m_DiagKobetu2;
	CICSDiag	m_DiagKobetu3;
	CICSDiag	m_DiagKei1;				// 計が1行の計専用ダイアグラム
	CICSDiag	m_DiagKei2;				// 計が2行専用のダイアグラム

	CICSDiag*	m_pDiagHide;			// 通常/個別帳表にて、非表示にしているDiag

private:
	BOOL	virStartJob();
	void	virSetInitialUchiwakeInfo();
	void	virSetInitialDiag();
	BOOL	InitialControlDataArea();
	BOOL	InitialControlDataAreaHide( CICSDiag* pDiag );
	BOOL	InitialControlComboBox();
	void	InitialControlTotalArea();
	void	InitialControlTotalAreaHide( CICSDiag* pDiag );
	void	InitialControlSpecialRow( int intRow, int intFgFunc );
	void	virUpdateControlTitle();
	void	UpdateControlPage();
	BOOL	virUpdateControlTblData();
// midori 161108 add -->
	BOOL	virUpdateControlTblData2();
// midori 161108 add <--
	void	UpdateControlRowData( CdbUc102Gensen* inTbl );
	void	UpdateControlRowTotal( CdbUc102Gensen* inTbl );
	CdbUc000Common*	virGetDataTableObject();
	void	virGetCurrentInfo(CURRENT_INFO* uCurInfo);
	void	virFncCellCopyProc(CdbUc000Common* rsData);									// F6 :項目複写
	int		virFncReferenceGetType(int intCol, int* intGrSeq, long* lngItemSeq, int* intAdd1, int* intAdd2, int* intAdd1Col, int* intAdd2Col, REFERENCE_INFO* pudReference = 0);	// F9 :参照
	void	virFncReferenceSetData(CdlgReference* dlgRef, int intRefType, int intCol);	// F9 :参照
	// void	virFncReferenceSetICSData(CdlgAddressSearch* dlgAddd);						// F9 :参照
	void	virFncReferenceUpdateTbl(int intRefType, int nRefMode = 0);					// F9 :参照
	int		virUpdateRelationTblChengeFormType();
// midori 152770 add -->
	BOOL	virUpdateRelationTblChengeFormTypeSave();
// midori 152770 add <--
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

// 修正No.157633_元号日付 add -->
	void Date_ChkAndSet(short, int, short);			// 元号+年月日の入力状況を取得
// 修正No.158469 del -->
	//LRESULT OnUserFocusset2(WPARAM wParam, LPARAM lParam);
// 修正No.158469 del <--
// 修正No.157633_元号日付 add <--

// midori 151405 add -->
	void VScrollYoushikidiag1(short mode);
// midori 151405 add <--
// midori 152134 add -->
	void ComboSelYoushikidiag1(short index, LPUNKNOWN data);
// midori 152134 add <--
	LRESULT OnUserReDrawView(WPARAM wParam, LPARAM lParam);	// 修正No.162446
};

