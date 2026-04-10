//--------------------------------------------------
//	frmUc071Koteisisan.h
//--------------------------------------------------

#pragma once

#include "frmUc000Common.h"
#include "dbUc071Koteishisan.h"
#include "prtUc071Koteisisan.h"

/****************************************************************
	「固定資産」関連
*****************************************************************/
#define	ID_ROWNORMAL_071		10		// 通常時：１頁内の合計行を含む全体の行数
#define	ID_ROWKOBETU_071		10		// 個別時：１頁内の合計行を含む全体の行数
#define	ID_ROWKEI_071			0		// 帳表固定の合計行数（1:合計あり，0:なし）
#define	ID_COLMAX_071			32		// １行内のコントロール個数
#define	ID_COLMAXDATA_071		14		// １行内の入力コントロール数
#define	ID_CTLSTARTINDEX_071	12		// １行目の入力項目の先頭インデックス番号
#define ID_T1STARTNORMAL_071	320		// 通常時：合計行①の先頭インデックス番号
#define ID_T2STARTNORMAL_071	352		// 通常時：合計行②の先頭インデックス番号
#define ID_T1STARTKOBETU_071	320		// 個別時：合計行①の先頭インデックス番号
#define ID_T2STARTKOBETU_071	352		// 個別時：合計行②の先頭インデックス番号

// 「固定資産」テーブル名称
//	正式版では"CdbUc071Koteishisan"のメンバー変数で参照可能としたい
#define	TBL_NAME_071		_T("uc_071_Koteishisan")

//////////////////////////////////////////////////////////////////
//	「固定資産」の内訳書列番号（帳表入力画面で使用）
typedef enum {
	ID_COL_071_SYURUI = 1,				// 1	種類・構造
	ID_COL_071_YOUTO,					// 2	用途
	ID_COL_071_AREA,					// 3	面積
	ID_COL_071_AD2ADD1,					// 4	物件の所在地1
	ID_COL_071_AD2ADD2,					// 5	物件の所在地2
	ID_COL_071_VAL,						// 6	期末現在高
	ID_COL_071_SPDATE,					// 7	異動年月日
	ID_COL_071_RIYUU,					// 8	異動事由
	ID_COL_071_MSVAL,					// 9	所得(処分)価額
	ID_COL_071_MSVAL2,					// 10	異動直前の帳簿価額
	ID_COL_071_ADNAME1,					// 11	売却(購入)先の名称1(氏名)
	ID_COL_071_ADADD1,					// 12	売却(購入)先の所在地1(住所)
	ID_COL_071_ADADD2,					// 13	売却(購入)先の所在地2(住所)
	ID_COL_071_SPDATE2,					// 14	売却物件の取得年月
} EnumIdFormCol071;

//////////////////////////////////////////////////////////////////
//	「固定資産」の内訳書_集計項目
#define	ID_CALC_071_MAX		3				// 集計フィールド件数
#define	ID_CALC_071_FIELD1	_T("Val")		// 集計フィールド名称1
#define	ID_CALC_071_FIELD2	_T("MsVal")		// 集計フィールド名称2
#define	ID_CALC_071_FIELD3	_T("MsVal2")	// 集計フィールド名称3


//////////////////////////////////////////////////////////////////
//	「固定資産」の内訳書_計ダイアグラムの入力インデックス
#define ID_COL_071_KEI1_MEISYO	0				// 計ダイアログ1行目の計名称（計or合計、頁計、累計）
// midori 152503 del -->
//#define ID_COL_071_KEI1_VAL		1				// 計ダイアログ1行目の期末現在高
//#define ID_COL_071_KEI1_MSVAL	4				// 計ダイアログ1行目の所得(処分)価額
//#define ID_COL_071_KEI1_MSVAL2	5				// 計ダイアログ1行目の異動直前の帳簿価額
//#define ID_COL_071_KEI2_MEISYO	9				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
//#define ID_COL_071_KEI2_VAL		10				// 計ダイアログ2行目の期末現在高
//#define ID_COL_071_KEI2_MSVAL	13				// 計ダイアログ2行目の所得(処分)価額
//#define ID_COL_071_KEI2_MSVAL2	14				// 計ダイアログ2行目の異動直前の帳簿価額
// midori 152503 del <--
// midori 152503 add -->
#define ID_COL_071_KEI1_VAL		2				// 計ダイアログ1行目の期末現在高
#define ID_COL_071_KEI1_MSVAL	7				// 計ダイアログ1行目の所得(処分)価額
#define ID_COL_071_KEI1_MSVAL2	8				// 計ダイアログ1行目の異動直前の帳簿価額
#define ID_COL_071_KEI2_MEISYO	12				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
#define ID_COL_071_KEI2_VAL		14				// 計ダイアログ2行目の期末現在高
#define ID_COL_071_KEI2_MSVAL	19				// 計ダイアログ2行目の所得(処分)価額
#define ID_COL_071_KEI2_MSVAL2	20				// 計ダイアログ2行目の異動直前の帳簿価額
// midori 152503 add <--

//////////////////////////////////////////////////////////////////
//	1レコード分の構造体
typedef struct tagRecuc_071_Koteishisan_071
{
	long	m_Seq;			// シーケンス番号
	int		m_NumPage;		// 頁番号
	BYTE	m_NumRow;		// 行番号
	BYTE	m_FgFunc;		// 特殊行フラグ
	BYTE	m_FgShow;		// 表示フラグ
	CString	m_KeiStr;		// 計
	long	m_NumGroup;		// グループ番号
	CString	m_Syurui;		// 種類・構造
	CString	m_Youto;		// 用途
	CString	m_Area;			// 面積
	CString	m_Ad2Add1;		// 物件の所在地（上段）
	CString	m_Ad2Add2;		// 物件の所在地（下段）
	CString	m_Val;			// 期末現在高
	long	m_SpDate;		// 異動年月日
	CString	m_Riyuu;		// 異動事由
	CString	m_MsVal;		// 取得価額
	CString	m_MsVal2;		// 異動直前の帳簿価額
	long	m_AdSeq;		// 売却先：取引先（シーケンス番号）
	long	m_AdOrder;		// 売却先：取引先（並び順）
	CString	m_AdName1;		// 売却先（名称：上段）
	CString	m_AdName2;		// 売却先（名称：下段）
	CString	m_AdAdd1;		// 売却先（所在地：上段）
	CString	m_AdAdd2;		// 売却先（所在地：下段）
	long	m_SpDate2;		// 売却物件の取得年月
	int		m_ShowKeiZero;	// ０円表示フラグ
	CString	m_InvNo;		// インボイス登録番号	// インボイス登録番号追加対応_23/09/11 add
} REC_UC_071_KOTEISHISAN, *P_REC_UC_071_KOTEISHISAN;

class CfrmUc071Koteisisan: public CfrmUc000Common
{
	DECLARE_DYNCREATE( CfrmUc071Koteisisan )

public:
	CfrmUc071Koteisisan();
	virtual	~CfrmUc071Koteisisan();
	void	virSetInitialValue();

private:
	void	virRowButtonCopyProc();
	int		virRecBufSaveData( int inPage, int inRow );
	int		virRecBufSaveDataForPasteInsert( int inPage, int inRow );
	int		virRecBufSaveDataForPaste( int inPage, int inRow );
	int		RecBufClearData( P_REC_UC_071_KOTEISHISAN inRecData );
	int		RecBufSaveDataSub( int inPage, int inRow, P_REC_UC_071_KOTEISHISAN inSaveData );
	int		RecBufSetData( CdbUc071Koteishisan* inTbl );
	int		RecBufClearAllData();

	// 1ページ分のデータ格納用配列
	// 配列の添え字と、行番号を合わせるため、最大行+1とした
	REC_UC_071_KOTEISHISAN	m_ReadData[ID_ROWKOBETU_071 + 1];
	REC_UC_071_KOTEISHISAN	m_CopyData;		// 行コピー用バッファ
	int	m_nCopyFlag;					// コピーフラグ（0：コピーデータがない／1：コピーデータがある）

public:
	enum{ IDD = IDD_YOUSHIKI071 };
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
	CICSDisp	m_Guide1;										// 欄外のガイド表示
// 修正No.157626 add <--

private:
	BOOL	virStartJob();
	void	virSetInitialUchiwakeInfo();
	void	virSetInitialDiag();
	BOOL	InitialControlDataArea();
	void	InitialControlTotalArea();
	void	InitialControlSpecialRow( int intRow, int intFgFunc );
	void	virUpdateControlTitle();
	void	UpdateControlPage();
	BOOL	virUpdateControlTblData();
// midori 161108 add -->
	BOOL	virUpdateControlTblData2();
// midori 161108 add <--
	void	UpdateControlRowData( CdbUc071Koteishisan* inTbl );
	void	UpdateControlRowTotal( CdbUc071Koteishisan* inTbl );
	CdbUc000Common*	virGetDataTableObject();
	void	virGetCurrentInfo(CURRENT_INFO* uCurInfo);
	void	virFncCellCopyProc(CdbUc000Common* rsData);				// F6 :項目複写
	int		virFncReferenceGetType(int intCol, int* intGrSeq, long* lngItemSeq, int* intAdd1, int* intAdd2, int* intAdd1Col, int* intAdd2Col, REFERENCE_INFO* pudReference = 0);	// F9 :参照
	void	virFncReferenceSetICSData(CdlgAddressSearch* dlgAddd);	// F9 :参照
// 修正No.157633_元号日付 add -->
	void	virActivateFrame(UINT nState, CFrameWnd* pDeactivateFrame);
// 修正No.157633_元号日付 add <--

	int		GetFocusPosition( int nChar );	// フォーカス移動位置取得関数
	int		GetTotalRowIndex( int nChar , int intIndex , int intNext );	// 行移動制御関数
	BOOL	CheckShiftKey();

	int		virTblSortSubSortProc(CdlgSort* pCdlgSort, CdbUc000Common* rsData);
	void	virUpdateTitle();
	void	virInputDecisionEditOFF();

	void	SetInputMode( int nRow, long nMode );
	void	SetDiagBackColor();
// midori 151405 add -->
	int		CursorControl(short nChar, int sw);
// midori 151405 add <--
// No.200601 add -->
	void	TitleColorChg(void);
// No.200601 add <--

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

// 修正No.157633_元号日付 add -->
	void Date_ChkAndSet(short, int, short);			// 元号+年月日の入力状況を取得
// 修正No.157633_元号日付 add <--

// 2009.09.08 TS自動ログオフ対応
protected:
	virtual	BOOL	virEndProc(void);
// midori 151405 add -->
public:
	void VScrollYoushikidiag1(short mode);
// midori 151405 add <--
	LRESULT OnUserReDrawView(WPARAM wParam, LPARAM lParam);	// 修正No.162446
};

