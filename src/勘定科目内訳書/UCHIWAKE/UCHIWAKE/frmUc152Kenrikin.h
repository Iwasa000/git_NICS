//--------------------------------------------------
//	frmUc152Kenrikin.h
//--------------------------------------------------

#pragma once

#include "frmUc000Common.h"
#include "dbUc152Kenrikin.h"
#include "prtUc152Kenrikin.h"
#include "prtUc150TidaiyatinKenrikinKougyou.h"

/****************************************************************
	「権利金」関連
*****************************************************************/
#define	ID_ROWNORMAL_152		3		// 通常時：１頁内の合計行を含む全体の行数
#define	ID_ROWKOBETU_152		18		// 個別時：１頁内の合計行を含む全体の行数
#define	ID_ROWKEI_152			0		// 帳表固定の合計行数（1:合計あり，0:なし）
#define	ID_COLMAX_152			14		// １行内のコントロール個数
#define	ID_COLMAXDATA_152		7		// １行内の入力コントロール数
#define	ID_CTLSTARTINDEX_152	6		// １行目の入力項目の先頭インデックス番号
#define ID_T1STARTNORMAL_152	42		// 通常時：合計行①の先頭インデックス番号
#define ID_T2STARTNORMAL_152	56		// 通常時：合計行②の先頭インデックス番号
#define ID_T1STARTKOBETU_152	252		// 個別時：合計行①の先頭インデックス番号
#define ID_T2STARTKOBETU_152	266		// 個別時：合計行②の先頭インデックス番号

// 「権利金」テーブル名称
//	正式版では"CdbUc152Kenrikin"のメンバー変数で参照可能としたい
#define	TBL_NAME_152		_T("uc_152_Kenrikin")

//////////////////////////////////////////////////////////////////
//	「権利金」の内訳書列番号（帳表入力画面で使用）
typedef enum {
	ID_COL_152_ADNAME1 = 1,				// 1	支払先の名称1
	ID_COL_152_ADADD1,					// 2	支払先の所在地1
	ID_COL_152_ADADD2,					// 3	支払先の所在地2
	ID_COL_152_SPDATE,					// 4	支払年月日
	ID_COL_152_VAL,						// 5	支払金額
	ID_COL_152_NAIYOU,					// 6	権利金等の内容
	ID_COL_152_TEKI,					// 7	摘要
} EnumIdFormCol152;

//////////////////////////////////////////////////////////////////
//	「権利金」の内訳書_集計項目
#define	ID_CALC_152_MAX		1	// 集計フィールド件数
#define	ID_CALC_152_FIELD1	_T("Val")	// 集計フィールド名称1

//////////////////////////////////////////////////////////////////
//	「権利金」の内訳書_計ダイアグラムの入力インデックス
// midori 152503 del -->
//#define ID_COL_152_KEI1_MEISYO	0				// 計ダイアログ1行目の計名称（計or合計、頁計、累計）
//#define ID_COL_152_KEI1_VAL		2				// 計ダイアログ1行目の支払金額
//#define ID_COL_152_KEI2_MEISYO	5				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
//#define ID_COL_152_KEI2_VAL		7				// 計ダイアログ2行目の支払金額
// midori 152503 del <--
// midori 152503 add -->
#define ID_COL_152_KEI1_MEISYO	0				// 計ダイアログ1行目の計名称（計or合計、頁計、累計）
#define ID_COL_152_KEI1_VAL		3				// 計ダイアログ1行目の支払金額
#define ID_COL_152_KEI2_MEISYO	6				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
#define ID_COL_152_KEI2_VAL		9				// 計ダイアログ2行目の支払金額
// midori 152503 add <--


//////////////////////////////////////////////////////////////////
//	1レコード分の構造体
typedef struct tagRecuc_152_Kenrikin_152
{
	long	m_Seq;			// シーケンス番号
	int		m_NumPage;		// 頁番号
	BYTE	m_NumRow;		// 行番号
	BYTE	m_FgFunc;		// 特殊行フラグ
	BYTE	m_FgShow;		// 表示フラグ
	CString	m_KeiStr;		// 計
	long	m_NumGroup;		// グループ番号
	long	m_AdSeq;		// 取引先（シーケンス番号）
	long	m_AdOrder;		// 取引先（並び順）
	CString	m_AdName1;		// 支払先（名称１）
	CString	m_AdName2;		// 支払先（名称２）
	CString	m_AdAdd1;		// 支払先（所在地：上段）
	CString	m_AdAdd2;		// 支払先（所在地：下段）
	long	m_SpDate;		// 支払年月日
	CString	m_Val;			// 支払金額
	CString	m_Naiyou;		// 権利金等の内容
	CString	m_Teki;			// 摘要
	int		m_ShowKeiZero;	// ０円表示フラグ
	CString	m_InvNo;		// 登録番号(法人番号)	インボイス登録番号追加対応_23/10/20 add
} REC_UC_152_KENRIKIN, *P_REC_UC_152_KENRIKIN;

class CfrmUc152Kenrikin: public CfrmUc000Common
{
	DECLARE_DYNCREATE( CfrmUc152Kenrikin )

public:
	CfrmUc152Kenrikin();
	virtual	~CfrmUc152Kenrikin();
	void	virSetInitialValue();
	int		UpdateTbl152ChangeFormType();

private:
	void	virRowButtonCopyProc();
	int		virRecBufSaveData( int inPage, int inRow );
	int		virRecBufSaveDataForPasteInsert( int inPage, int inRow );
	int		virRecBufSaveDataForPaste( int inPage, int inRow );
	int		RecBufClearData( P_REC_UC_152_KENRIKIN inRecData );
	int		RecBufSaveDataSub( int inPage, int inRow, P_REC_UC_152_KENRIKIN inSaveData );
	int		RecBufSetData( CdbUc152Kenrikin* inTbl );
	int		RecBufClearAllData();

	// 1ページ分のデータ格納用配列
	// 配列の添え字と、行番号を合わせるため、最大行+1とした
	REC_UC_152_KENRIKIN	m_ReadData[ID_ROWKOBETU_152 + 1];
	REC_UC_152_KENRIKIN	m_CopyData;		// 行コピー用バッファ
	int	m_nCopyFlag;					// コピーフラグ（0：コピーデータがない／1：コピーデータがある）

	int	m_intT1StartIndexHide;			// 通常/個別帳表にて、非表示にしているDiagの合計行①の先頭インデックス
	int	m_intT2StartIndexHide;			// 通常/個別帳表にて、非表示にしているDiagの合計行②の先頭インデックス

public:
	enum{ IDD = IDD_YOUSHIKI152 };
	CICSDiag	m_TitleDiag;
	CICSDiag	m_PageDiag;
	CICSDiag	m_NumberDiag;
// midori 161108 add -->
	CICSDiag	m_SaveDataDiag;
// midori 161108 add <--
	CICSDiag	m_HeadDiag;
	CICSDiag	m_DiagNormal;
	CICSDiag	m_DiagNormal2;
	CICSDiag	m_DiagNormal3;
	CICSDiag	m_DiagKobetu;
	CICSDiag	m_DiagKobetu2;
	CICSDiag	m_DiagKobetu3;
	CICSDiag	m_DiagKeiKobetu;				// 計が1行の計専用ダイアグラム
	CICSDiag	m_DiagKeiKobetu2;				// 計が2行専用のダイアグラム
	CICSDiag	m_DiagKeiNormal;				// 計が1行の計専用ダイアグラム
	CICSDiag	m_DiagKeiNormal2;				// 計が2行の計専用ダイアグラム
// 修正No.157626 add -->
	CICSDisp	m_Guide11;						// 欄外のガイド表示（通常、合計設定:頁計or計用）
	CICSDisp	m_Guide12;						// 欄外のガイド表示（通常、合計設定:オフ用）
	CICSDisp	m_Guide13;						// 欄外のガイド表示（通常、合計設定:頁計+累計用）
	CICSDisp	m_Guide2;						// 欄外のガイド表示（個別用）
// 修正No.157626 add <--

	CICSDiag*	m_pDiagHide;					// 通常/個別帳表にて、非表示にしているDiag

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
	void	UpdateControlRowData( CdbUc152Kenrikin* inTbl );
	void	UpdateControlRowTotal( CdbUc152Kenrikin* inTbl );
	CdbUc000Common*	virGetDataTableObject();
	void	virGetCurrentInfo(CURRENT_INFO* uCurInfo);
	void	virFncCellCopyProc(CdbUc000Common* rsData);									// F6 :項目複写
	int		virFncReferenceGetType(int intCol, int* intGrSeq, long* lngItemSeq, int* intAdd1, int* intAdd2, int* intAdd1Col, int* intAdd2Col, REFERENCE_INFO* pudReference = 0);	// F9 :参照
	//void	virFncReferenceSetData(CdlgReference* dlgRef, int intRefType, int intCol);	// F9 :参照
	void	virFncReferenceSetICSData(CdlgAddressSearch* dlgAddd);						// F9 :参照
	//void	virFncReferenceUpdateTbl(int intRefType);									// F9 :参照
	int		virUpdateRelationTblChengeFormType();
// midori 152770 add -->
	BOOL	virUpdateRelationTblChengeFormTypeSave();
// midori 152770 add <--
// 修正No.157633_元号日付 add -->
	void	virActivateFrame(UINT nState, CFrameWnd* pDeactivateFrame);
// 修正No.157633_元号日付 add <--

	int		GetFocusPosition( int nChar );	// フォーカス移動位置取得関数
	int		GetTotalRowIndex( int nChar , int intIndex , int intNext );	// 行移動制御関数
	BOOL	CheckShiftKey();

	//void	virTblEditIkkatuLine(int nType, CdbUc000Common* rsData, CString strName, CALCKEI_INFO uCalcKei, SORTKAMOKU_INFO uKamoku);
	int		virTblSortSubSortProc(CdlgSort* pCdlgSort, CdbUc000Common* rsData);
	//void	virTblGetKamokuInfo(CdbUc000Common* rsData, SORTKAMOKU_INFO* uKamoku);
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
// No.200601 add -->
	void TitleColorChg(void);
// No.200601 add <--
// 修正No.157633_元号日付 add -->
	void Date_ChkAndSet(short, int, short);			// 元号+年月日の入力状況を取得
// 修正No.157633_元号日付 add <--
	LRESULT OnUserReDrawView(WPARAM wParam, LPARAM lParam);	// 修正No.162446
};
