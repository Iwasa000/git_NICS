//--------------------------------------------------
//	frmUc131Uriagedaka.h
//--------------------------------------------------

#pragma once

#include "frmUc000Common.h"
#include "dbUc131Uriagedaka.h"
#include "prtUc131Uriagedaka.h"

/****************************************************************
	「売上高」関連
*****************************************************************/
#define	ID_ROWNORMAL_131	15		// 通常時：１頁内の合計行を含む全体の行数
#define	ID_ROWKOBETU_131	15		// 個別時：１頁内の合計行を含む全体の行数
#define	ID_ROWKEI_131	1		// 帳表固定の合計行数（1:合計あり，0:なし）
#define	ID_COLMAX_131	26		// １行内のコントロール個数
#define	ID_COLMAXDATA_131	12		// １行内の入力コントロール数
#define	ID_CTLSTARTINDEX_131	11		// １行目の入力項目の先頭インデックス番号
#define ID_T1STARTNORMAL_131	364	// 通常時：合計行①の先頭インデックス番号
#define ID_T2STARTNORMAL_131	390	// 通常時：合計行②の先頭インデックス番号
#define ID_T1STARTKOBETU_131	364	// 個別時：合計行①の先頭インデックス番号
#define ID_T2STARTKOBETU_131	390	// 個別時：合計行②の先頭インデックス番号

// 「売上高」テーブル名称
//	正式版では"CdbUc131Uriagedaka"のメンバー変数で参照可能としたい
#define	TBL_NAME_131		_T("uc_131_Uriagedaka")

//////////////////////////////////////////////////////////////////
//	「売上高」の内訳書列番号（帳表入力画面で使用）
typedef enum {
	ID_COL_131_ADNAME1 = 1,	// 1	事業所の名称1
	ID_COL_131_ADADD1,	// 2	所在地1
	ID_COL_131_ADADD2,	// 3	所在地2
	ID_COL_131_HDNAME,	// 4	責任者氏名
	ID_COL_131_KANKEI,	// 5	代表者との関係
	ID_COL_131_NAIYOU,	// 6	事業所の内容
	ID_COL_131_VAL,		// 7	売上高
	ID_COL_131_VAL2,	// 8	期末現在高
	ID_COL_131_WORKER,	// 9	期末従業員数
	ID_COL_131_AREA,	// 10	使用建物の延面積
	ID_COL_131_HDNAME2,	// 11	源泉取得納税付署
	ID_COL_131_TEKI,	// 12	摘要
} EnumIdFormCol131;

//////////////////////////////////////////////////////////////////
//	「売上高」の内訳書_集計項目
#define	ID_CALC_131_MAX		4	// 集計フィールド件数
#define	ID_CALC_131_FIELD1	_T("Val")	// 集計フィールド名称1
#define	ID_CALC_131_FIELD2	_T("Val2")	// 集計フィールド名称2
#define	ID_CALC_131_FIELD3	_T("Worker")	// 集計フィールド名称3
#define	ID_CALC_131_FIELD4	_T("Area")	// 集計フィールド名称4

//////////////////////////////////////////////////////////////////
//	「売上高」の内訳書_計ダイアグラムの入力インデックス
#define ID_COL_131_KEI1_MEISYO	0				// 計ダイアログ1行目の計名称（計or合計、頁計、累計）
// midori 152503 del -->
//#define ID_COL_131_KEI1_VAL		3				// 計ダイアログ1行目の売上高
//#define ID_COL_131_KEI1_VAL2	4				// 計ダイアログ1行目の期末現在高
//#define ID_COL_131_KEI1_WORKER	5				// 計ダイアログ1行目の期末従業員数
//#define ID_COL_131_KEI1_AREA	6				// 計ダイアログ1行目の使用建物の延面積
//#define ID_COL_131_KEI2_MEISYO	12				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
//#define ID_COL_131_KEI2_VAL		15				// 計ダイアログ2行目の売上高
//#define ID_COL_131_KEI2_VAL2	16				// 計ダイアログ2行目の期末現在高
//#define ID_COL_131_KEI2_WORKER	17				// 計ダイアログ2行目の期末従業員数
//#define ID_COL_131_KEI2_AREA	18				// 計ダイアログ2行目の使用建物の延面積
// midori 152503 del <--
// midori 152503 add -->
#define ID_COL_131_KEI1_VAL		4				// 計ダイアログ1行目の売上高
#define ID_COL_131_KEI1_VAL2	6				// 計ダイアログ1行目の期末現在高
#define ID_COL_131_KEI1_WORKER	9				// 計ダイアログ1行目の期末従業員数
#define ID_COL_131_KEI1_AREA	10				// 計ダイアログ1行目の使用建物の延面積
#define ID_COL_131_KEI2_MEISYO	16				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
#define ID_COL_131_KEI2_VAL		20				// 計ダイアログ2行目の売上高
#define ID_COL_131_KEI2_VAL2	22				// 計ダイアログ2行目の期末現在高
#define ID_COL_131_KEI2_WORKER	25				// 計ダイアログ2行目の期末従業員数
#define ID_COL_131_KEI2_AREA	26				// 計ダイアログ2行目の使用建物の延面積
// midori 152503 add <--

//////////////////////////////////////////////////////////////////
//	1レコード分の構造体
typedef struct tagRecuc_131_Uriagedaka_131
{
	long	m_Seq;	// シーケンス番号
	int	m_NumPage;	// 頁番号
	BYTE	m_NumRow;	// 行番号
	BYTE	m_FgFunc;	// 特殊行フラグ
	BYTE	m_FgShow;	// 表示フラグ
	CString	m_KeiStr;	// 計
	long	m_NumGroup;	// グループ番号
	long	m_AdSeq;	// 取引先（シーケンス番号）
	long	m_AdOrder;	// 取引先（並び順）
	CString	m_AdName1;	// 事業所の名称（名称１）
	CString	m_AdName2;	// 事業所の名称（名称２）
	CString	m_AdAdd1;	// 所在地（所在地：上段）
	CString	m_AdAdd2;	// 所在地（所在地：下段）
	CString	m_HdName;	// 責任者氏名
	CString	m_Kankei;	// 代表者との関係
	CString	m_Naiyou;	// 事業等の内容
	CString	m_Val;	// 売上高
	CString	m_Val2;	// 期末棚卸高
	CString	m_Worker;	// 期末従業員数
	CString	m_Area;	// 使用建物の延面積
	CString	m_HdName2;	// 源泉取得納税付署
	CString	m_Teki;	// 摘要
	int		m_ShowKeiZero;	// ０円表示フラグ
} REC_UC_131_URIAGEDATA, *P_REC_UC_131_URIAGEDATA;

class CfrmUc131Uriagedaka: public CfrmUc000Common
{
	DECLARE_DYNCREATE( CfrmUc131Uriagedaka )

public:
	CfrmUc131Uriagedaka();
	virtual	~CfrmUc131Uriagedaka();
	void	virSetInitialValue();

private:
	void	virRowButtonCopyProc();
	int		virRecBufSaveData( int inPage, int inRow );
	int		virRecBufSaveDataForPasteInsert( int inPage, int inRow );
	int		virRecBufSaveDataForPaste( int inPage, int inRow );
	int		RecBufClearData( P_REC_UC_131_URIAGEDATA inRecData );
	int		RecBufSaveDataSub( int inPage, int inRow, P_REC_UC_131_URIAGEDATA inSaveData );
	int		RecBufSetData( CdbUc131Uriagedaka* inTbl );
	int		RecBufClearAllData();

	// 1ページ分のデータ格納用配列
	// 配列の添え字と、行番号を合わせるため、最大行+1とした
	REC_UC_131_URIAGEDATA	m_ReadData[ID_ROWKOBETU_131 + 1];
	REC_UC_131_URIAGEDATA	m_CopyData;		// 行コピー用バッファ
	int	m_nCopyFlag;					// コピーフラグ（0：コピーデータがない／1：コピーデータがある）

public:
	enum{ IDD = IDD_YOUSHIKI131 };
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
	void	UpdateControlRowData( CdbUc131Uriagedaka* inTbl );
	void	UpdateControlRowTotal( CdbUc131Uriagedaka* inTbl );
	CdbUc000Common*	virGetDataTableObject();
	void	virGetCurrentInfo(CURRENT_INFO* uCurInfo);
	void	virFncCellCopyProc(CdbUc000Common* rsData);				// F6 :項目複写
	int		virFncReferenceGetType(int intCol, int* intGrSeq, long* lngItemSeq, int* intAdd1, int* intAdd2, int* intAdd1Col, int* intAdd2Col, REFERENCE_INFO* pudReference = 0);	// F9 :参照
	void	virFncReferenceSetICSData(CdlgAddressSearch* dlgAddd);	// F9 :参照

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

