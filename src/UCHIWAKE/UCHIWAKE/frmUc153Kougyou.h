//--------------------------------------------------
//	frmUc153Kougyou.h
//--------------------------------------------------

#pragma once

#include "frmUc000Common.h"
#include "dbUc153Kougyou.h"
#include "prtUc153Kougyou.h"
#include "prtUc150TidaiyatinKenrikinKougyou.h"

/****************************************************************
	「工業所有権」関連
*****************************************************************/
#define	ID_ROWNORMAL_153		3		// 通常時：１頁内の合計行を含む全体の行数
#define	ID_ROWKOBETU_153		21		// 個別時：１頁内の合計行を含む全体の行数
#define	ID_ROWKEI_153			0		// 帳表固定の合計行数（1:合計あり，0:なし）
#define	ID_COLMAX_153			20		// １行内のコントロール個数
#define	ID_COLMAXDATA_153		10		// １行内の入力コントロール数
#define	ID_CTLSTARTINDEX_153	9		// １行目の入力項目の先頭インデックス番号
#define ID_T1STARTNORMAL_153	60		// 通常時：合計行①の先頭インデックス番号
#define ID_T2STARTNORMAL_153	80		// 通常時：合計行②の先頭インデックス番号
#define ID_T1STARTKOBETU_153	420		// 個別時：合計行①の先頭インデックス番号
#define ID_T2STARTKOBETU_153	440		// 個別時：合計行②の先頭インデックス番号

// 「工業所有権」テーブル名称
//	正式版では"CdbUc153Kougyou"のメンバー変数で参照可能としたい
#define	TBL_NAME_153		_T("uc_153_Kougyou")

//////////////////////////////////////////////////////////////////
//	「工業所有権」の内訳書列番号（帳表入力画面で使用）
typedef enum {
	ID_COL_153_HDNAME = 1,				// 1	名称
	ID_COL_153_ADNAME1,					// 2	支払先の名称1
//	↓再度使用の可能性があるためコメントにしておく
//	ID_COL_153_ADNAME2,					// 3	支払先の名称2
	ID_COL_153_ADADD1,					// 4	支払先の所在地1
	ID_COL_153_ADADD2,					// 5	支払先の所在地2
	ID_COL_153_KYDATE,					// 6	契約期間1
	ID_COL_153_KYDATE2,					// 7	契約期間2
	ID_COL_153_SPDATE,					// 8	支払対象期間1
	ID_COL_153_SPDATE2,					// 9	支払対象期間2
	ID_COL_153_VAL,						// 10	支払金額
	ID_COL_153_TEKI,					// 11	摘要
} EnumIdFormCol153;

//////////////////////////////////////////////////////////////////
//	「工業所有権」の内訳書_集計項目
#define	ID_CALC_153_MAX		1			// 集計フィールド件数
#define	ID_CALC_153_FIELD1	_T("Val")	// 集計フィールド名称1

//////////////////////////////////////////////////////////////////
//	「工業所有権」の内訳書_計ダイアグラムの入力インデックス
#define ID_COL_153_KEI1_MEISYO	0				// 計ダイアログ1行目の計名称（計or合計、頁計、累計）
// midori 152503 del -->
//#define ID_COL_153_KEI1_VAL		5				// 計ダイアログ1行目の支払金額
//#define ID_COL_153_KEI2_MEISYO	8				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
//#define ID_COL_153_KEI2_VAL		13				// 計ダイアログ2行目の支払金額
// midori 152503 del <--
// midori 152503 add -->
#define ID_COL_153_KEI1_VAL		6				// 計ダイアログ1行目の支払金額
#define ID_COL_153_KEI2_MEISYO	9				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
#define ID_COL_153_KEI2_VAL		15				// 計ダイアログ2行目の支払金額
// midori 152503 add <--

//////////////////////////////////////////////////////////////////
//	1レコード分の構造体
typedef struct tagRecuc_153_Kougyou_153
{
	long	m_Seq;				// シーケンス番号
	int		m_NumPage;			// 頁番号
	BYTE	m_NumRow;			// 行番号
	BYTE	m_FgFunc;			// 特殊行フラグ
	BYTE	m_FgShow;			// 表示フラグ
	CString	m_KeiStr;			// 計
	long	m_NumGroup;			// グループ番号
	CString	m_HdName;			// 名称
	long	m_AdSeq;			// 取引先（シーケンス番号）
	long	m_AdOrder;			// 取引先（並び順）
	CString	m_AdName1;			// 支払先（名称１）
	CString	m_AdName2;			// 支払先（名称２）
	CString	m_AdAdd1;			// 支払先（所在地：上段）
	CString	m_AdAdd2;			// 支払先（所在地：下段）
	long	m_KyDate;			// 契約期間（開始）
	long	m_KyDate2;			// 契約期間（終了）
	long	m_SpDate;			// 支払対象期間（開始）
	long	m_SpDate2;			// 支払対象期間（終了）
	CString	m_Val;				// 支払金額
	CString	m_Teki;				// 摘要
	int		m_ShowKeiZero;		// ０円表示フラグ
	CString	m_InvNo;			// 登録番号(法人番号)	インボイス登録番号追加対応_23/10/20 add
} REC_UC_153_KOUGYOU, *P_REC_UC_153_KOUGYOU;

class CfrmUc153Kougyou: public CfrmUc000Common
{
	DECLARE_DYNCREATE( CfrmUc153Kougyou )

public:
	CfrmUc153Kougyou();
	virtual	~CfrmUc153Kougyou();
	void	virSetInitialValue();
// midori 152770 del -->
//	int		UpdateTbl153ChangeFormType();
// midori 152770 del <--
// midori 152770 add -->
	int		UpdateTbl153ChangeFormType(UCHIWAKE_INFO pInfoSv);
// midori 152770 add <--

private:
	void	virRowButtonCopyProc();
	int		virRecBufSaveData( int inPage, int inRow );
	int		virRecBufSaveDataForPasteInsert( int inPage, int inRow );
	int		virRecBufSaveDataForPaste( int inPage, int inRow );
	int		RecBufClearData( P_REC_UC_153_KOUGYOU inRecData );
	int		RecBufSaveDataSub( int inPage, int inRow, P_REC_UC_153_KOUGYOU inSaveData );
	int		RecBufSetData( CdbUc153Kougyou* inTbl );
	int		RecBufClearAllData();

	// 1ページ分のデータ格納用配列
	// 配列の添え字と、行番号を合わせるため、最大行+1とした
	REC_UC_153_KOUGYOU	m_ReadData[ID_ROWKOBETU_153 + 1];
	REC_UC_153_KOUGYOU	m_CopyData;		// 行コピー用バッファ
	int	m_nCopyFlag;					// コピーフラグ（0：コピーデータがない／1：コピーデータがある）

	int	m_intT1StartIndexHide;			// 通常/個別帳表にて、非表示にしているDiagの合計行①の先頭インデックス
	int	m_intT2StartIndexHide;			// 通常/個別帳表にて、非表示にしているDiagの合計行②の先頭インデックス

public:
	enum{ IDD = IDD_YOUSHIKI153 };
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
	CICSDiag	m_DiagKeiKobetu1;				// 計が1行の計専用ダイアグラム
	CICSDiag	m_DiagKeiKobetu2;				// 計が2行専用のダイアグラム
	CICSDiag	m_DiagKeiNormal1;				// 計が1行専用のダイアグラム
	CICSDiag	m_DiagKeiNormal2;				// 計が2行専用のダイアグラム
// 修正No.157626 add -->
	CICSDisp	m_Guide11;						// 欄外のガイド表示（通常、合計出力:計+頁計用）
	CICSDisp	m_Guide12;						// 欄外のガイド表示（通常、合計出力:オフ用）
	CICSDisp	m_Guide13;						// 欄外のガイド表示（通常、合計出力:頁計or累計用）
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
	void	UpdateControlRowData( CdbUc153Kougyou* inTbl );
	void	UpdateControlRowTotal( CdbUc153Kougyou* inTbl );
	CdbUc000Common*	virGetDataTableObject();
	void	virGetCurrentInfo(CURRENT_INFO* uCurInfo);
	void	virFncCellCopyProc(CdbUc000Common* rsData);				// F6 :項目複写
	int		virFncReferenceGetType(int intCol, int* intGrSeq, long* lngItemSeq, int* intAdd1, int* intAdd2, int* intAdd1Col, int* intAdd2Col, REFERENCE_INFO* pudReference = 0);	// F9 :参照
	void	virFncReferenceSetICSData(CdlgAddressSearch* dlgAddd);	// F9 :参照
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
// No.200601 add -->
	void TitleColorChg(void);
// No.200601 add <--
// 修正No.157633_元号日付 add -->
	void Date_ChkAndSet(short, int, short);			// 元号+年月日の入力状況を取得
// 修正No.157633_元号日付 add <--
	LRESULT OnUserReDrawView(WPARAM wParam, LPARAM lParam);	// 修正No.162446
};
