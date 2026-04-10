//--------------------------------------------------
//	frmUc151Tidaiyatin.h
//--------------------------------------------------

#pragma once

#include "frmUc000Common.h"
#include "dbUc151Tidaiyatin.h"
#include "prtUc151Tidaiyatin.h"
#include "prtUc150TidaiyatinKenrikinKougyou.h"

/****************************************************************
	「地代家賃」関連
*****************************************************************/
#define	ID_ROWNORMAL_151		8		// 通常時：１頁内の合計行を含む全体の行数
#define	ID_ROWKOBETU_151		19		// 個別時：１頁内の合計行を含む全体の行数
#define	ID_ROWKEI_151			0		// 帳表固定の合計行数（1:合計あり，0:なし）
#define	ID_COLMAX_151			23		// １行内のコントロール個数
#define	ID_COLMAXDATA_151		11		// １行内の入力コントロール数
#define	ID_CTLSTARTINDEX_151	9		// １行目の入力項目の先頭インデックス番号
#define ID_T1STARTNORMAL_151	184		// 通常時：合計行①の先頭インデックス番号
#define ID_T2STARTNORMAL_151	207		// 通常時：合計行②の先頭インデックス番号
#define ID_T1STARTKOBETU_151	437		// 個別時：合計行①の先頭インデックス番号
#define ID_T2STARTKOBETU_151	460		// 個別時：合計行②の先頭インデックス番号

// 「地代家賃」テーブル名称
//	正式版では"CdbUc151Tidaiyatin"のメンバー変数で参照可能としたい
#define	TBL_NAME_151		_T("uc_151_Tidaiyatin")

//////////////////////////////////////////////////////////////////
//	「地代家賃」の内訳書列番号（帳表入力画面で使用）
typedef enum {
	ID_COL_151_KUBUN = 1,				// 1	地代・家賃の区分
	ID_COL_151_YOUTO,					// 2	借地物件の用途
	ID_COL_151_AD2ADD1,					// 3	所在地1
	ID_COL_151_AD2ADD2,					// 4	所在地2
	ID_COL_151_ADNAME1,					// 5	貸主の名称1
	ID_COL_151_ADADD1,					// 6	貸主の所在地1
	ID_COL_151_ADADD2,					// 7	貸主の所在地2
	ID_COL_151_SPDATE,					// 8	支払対象期間1
	ID_COL_151_SPDATE2,					// 9	支払対象期間2
	ID_COL_151_VAL,						// 10	支払貸借料
	ID_COL_151_TEKI,					// 11	摘要
} EnumIdFormCol151;

//////////////////////////////////////////////////////////////////
//	「地代家賃」の内訳書_集計項目
#define	ID_CALC_151_MAX		1			// 集計フィールド件数
#define	ID_CALC_151_FIELD1	_T("Val")	// 集計フィールド名称1

//////////////////////////////////////////////////////////////////
//	「仮受金」の内訳書_計ダイアグラムの入力インデックス
#define ID_COL_151_KEI1_MEISYO	0				// 計ダイアログ1行目の計名称（計or合計、頁計、累計）
// midori 152503 del -->
//#define ID_COL_151_KEI1_VAL		6				// 計ダイアログ1行目の支払貸借料
//#define ID_COL_151_KEI2_MEISYO	11				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
//#define ID_COL_151_KEI2_VAL		17				// 計ダイアログ2行目の支払貸借料
// midori 152503 del <--
// midori 152503 add -->
#define ID_COL_151_KEI1_VAL		7				// 計ダイアログ1行目の支払貸借料
#define ID_COL_151_KEI2_MEISYO	12				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
#define ID_COL_151_KEI2_VAL		19				// 計ダイアログ2行目の支払貸借料
// midori 152503 add <--

//////////////////////////////////////////////////////////////////
//	1レコード分の構造体
typedef struct tagRecuc_151_Tidaiyatin_151
{
	long	m_Seq;				// シーケンス番号
	int		m_NumPage;			// 頁番号
	BYTE	m_NumRow;			// 行番号
	BYTE	m_FgFunc;			// 特殊行フラグ
	BYTE	m_FgShow;			// 表示フラグ
	CString	m_KeiStr;			// 計
	long	m_NumGroup;			// グループ番号
	long	m_KnSeq;			// 地代・家賃の区分Seq番号
	long	m_KnOrder;			// 地代・家賃の区分Order番号
	CString	m_KnName;			// 地代・家賃の区分名
	CString	m_Youto;			// 借地物件の用途
	long	m_AdSeq;			// 取引先（シーケンス番号）
	long	m_AdOrder;			// 取引先（並び順）
	CString	m_AdName1;			// 貸主（名称：上段）
	CString	m_AdName2;			// 貸主（名称：下段）
	CString	m_AdAdd1;			// 貸主（所在地：上段）
	CString	m_AdAdd2;			// 貸主（所在地：下段）
	long	m_SpDate;			// 支払対象期間（開始）
	long	m_SpDate2;			// 支払対象期間（終了）
	CString	m_Val;				// 支払貸借料
	CString	m_Teki;				// 摘要
	CString	m_Ad2Add1;			// 所在地1
	CString	m_Ad2Add2;			// 所在地2
	int		m_ShowKeiZero;		// ０円表示フラグ
// midori 152137 add -->
	CString	m_KnKana;			// 地代・家賃の区分
// midori 152137 add <--
	CString	m_InvNo;			// 登録番号(法人番号)	インボイス登録番号追加対応_23/10/20 add
} REC_UC_151_TIDAIYATIN, *P_REC_UC_151_TIDAIYATIN;

class CfrmUc151Tidaiyatin: public CfrmUc000Common
{
	DECLARE_DYNCREATE( CfrmUc151Tidaiyatin )

public:
	CfrmUc151Tidaiyatin();
	virtual	~CfrmUc151Tidaiyatin();
	void	virSetInitialValue();
// midori 152770 del -->
//	int		UpdateTbl151ChangeFormType();
// midori 152770 del <--
// midori 152770 add -->
	int		UpdateTbl151ChangeFormType(UCHIWAKE_INFO pInfoSv);
// midori 152770 add <--

private:
	void	virRowButtonCopyProc();
	int		virRecBufSaveData( int inPage, int inRow );
	int		virRecBufSaveDataForPasteInsert( int inPage, int inRow );
	int		virRecBufSaveDataForPaste( int inPage, int inRow );
	int		RecBufClearData( P_REC_UC_151_TIDAIYATIN inRecData );
	int		RecBufSaveDataSub( int inPage, int inRow, P_REC_UC_151_TIDAIYATIN inSaveData );
	int		RecBufSetData( CdbUc151Tidaiyatin* inTbl );
	int		RecBufClearAllData();

	// 1ページ分のデータ格納用配列
	// 配列の添え字と、行番号を合わせるため、最大行+1とした
	REC_UC_151_TIDAIYATIN	m_ReadData[ID_ROWKOBETU_151 + 1];
	REC_UC_151_TIDAIYATIN	m_CopyData;		// 行コピー用バッファ
	int	m_nCopyFlag;					// コピーフラグ（0：コピーデータがない／1：コピーデータがある）

	int	m_intT1StartIndexHide;			// 通常/個別帳表にて、非表示にしているDiagの合計行①の先頭インデックス
	int	m_intT2StartIndexHide;			// 通常/個別帳表にて、非表示にしているDiagの合計行②の先頭インデックス

public:
	enum{ IDD = IDD_YOUSHIKI151 };
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
	CICSDiag	m_DiagKei1;				// 計が1行の計専用ダイアグラム
	CICSDiag	m_DiagKei2;				// 計が2行専用のダイアグラム
// 修正No.157626 add -->
	CICSDisp	m_Guide1;				// 欄外のガイド表示
// 修正No.157626 add <--

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
	void	UpdateControlRowData( CdbUc151Tidaiyatin* inTbl );
	void	UpdateControlRowTotal( CdbUc151Tidaiyatin* inTbl );
	CdbUc000Common*	virGetDataTableObject();
	void	virGetCurrentInfo(CURRENT_INFO* uCurInfo);
	void	virFncCellCopyProc(CdbUc000Common* rsData);									// F6 :項目複写
	int		virFncReferenceGetType(int intCol, int* intGrSeq, long* lngItemSeq, int* intAdd1, int* intAdd2, int* intAdd1Col, int* intAdd2Col, REFERENCE_INFO* pudReference = 0);	// F9 :参照
	void	virFncReferenceSetData(CdlgReference* dlgRef, int intRefType, int intCol);	// F9 :参照
	void	virFncReferenceUpdateTbl(int intRefType, int nRefMode = 0);					// F9 :参照
	void	virFncReferenceSetICSData(CdlgAddressSearch* dlgAddd);						// F9 :参照
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
