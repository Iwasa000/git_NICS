// インボイス登録番号追加対応_23/09/11 add -->
//--------------------------------------------------
//	frmUc151TidaiyatinI.h
//--------------------------------------------------

#pragma once

#include "frmUc000Common.h"
#include "frmUc151Tidaiyatin.h"
#include "dbUc151Tidaiyatin.h"
#include "prtUc151Tidaiyatin.h"
#include "prtUc150TidaiyatinKenrikinKougyou.h"

/****************************************************************
	「地代家賃」関連
*****************************************************************/
//#define	ID_ROWNORMAL_151		8		// 通常時：１頁内の合計行を含む全体の行数
//#define	ID_ROWKOBETU_151		19		// 個別時：１頁内の合計行を含む全体の行数
//#define	ID_ROWKEI_151			0		// 帳表固定の合計行数（1:合計あり，0:なし）
//#define	ID_COLMAX_151I			26		// １行内のコントロール個数
#define	ID_COLMAX_151I			23			// １行内のコントロール個数
#define	ID_COLMAXDATA_151I		13			// １行内の入力コントロール数

#define	ID_BACKCOLMAX_151I		7			// １行内の背景用表示項目数（～含む）
#define	ID_LINECOLMAX_151I		3			// １行内の罫線項目数

//#define	ID_CTLSTARTINDEX_151I	10		// １行目の入力項目の先頭インデックス番号
#define	ID_CTLSTARTINDEX_151I	7			// １行目の入力項目の先頭インデックス番号
//#define ID_T1STARTNORMAL_151I	/*184*/208	// 通常時：合計行①の先頭インデックス番号
//#define ID_T2STARTNORMAL_151I	/*207*/234	// 通常時：合計行②の先頭インデックス番号
//#define ID_T1STARTKOBETU_151I	/*437*/494	// 個別時：合計行①の先頭インデックス番号
//#define ID_T2STARTKOBETU_151I	/*460*/520	// 個別時：合計行②の先頭インデックス番号
#define ID_T1STARTNORMAL_151I	184			// 通常時：合計行①の先頭インデックス番号
#define ID_T2STARTNORMAL_151I	207			// 通常時：合計行②の先頭インデックス番号
#define ID_T1STARTKOBETU_151I	437			// 個別時：合計行①の先頭インデックス番号
#define ID_T2STARTKOBETU_151I	460			// 個別時：合計行②の先頭インデックス番号

//// 「地代家賃」テーブル名称
////	正式版では"CdbUc151Tidaiyatin"のメンバー変数で参照可能としたい
//#define	TBL_NAME_151		_T("uc_151_Tidaiyatin")

//////////////////////////////////////////////////////////////////
//	「地代家賃」の内訳書列番号（帳表入力画面で使用）
typedef enum {
	ID_COL_151I_KUBUN = 1,				// 1	地代・家賃の区分
	ID_COL_151I_YOUTO,					// 2	借地物件の用途
	ID_COL_151I_AD2ADD1,				// 3	所在地1
	ID_COL_151I_AD2ADD2,				// 4	所在地2
	ID_COL_151I_INVONOT,				// 5	登録番号（T選択）
	ID_COL_151I_INVONO,					// 6	登録番号
	ID_COL_151I_ADNAME1,				// 7	貸主の名称1
	ID_COL_151I_ADADD1,					// 8	貸主の所在地1
	ID_COL_151I_ADADD2,					// 9	貸主の所在地2
	ID_COL_151I_SPDATE,					// 10	支払対象期間1
	ID_COL_151I_SPDATE2,				// 11	支払対象期間2
	ID_COL_151I_VAL,					// 12	支払貸借料
	ID_COL_151I_TEKI,					// 13	摘要
} EnumIdFormCol151I;

////////////////////////////////////////////////////////////////////
////	「地代家賃」の内訳書_集計項目
//#define	ID_CALC_151_MAX		1			// 集計フィールド件数
//#define	ID_CALC_151_FIELD1	_T("Val")	// 集計フィールド名称1

//////////////////////////////////////////////////////////////////
//	「仮受金」の内訳書_計ダイアグラムの入力インデックス
#define ID_COL_151I_KEI1_MEISYO	0				// 計ダイアログ1行目の計名称（計or合計、頁計、累計）
#define ID_COL_151I_KEI1_VAL	/*7*/8			// 計ダイアログ1行目の支払貸借料
#define ID_COL_151I_KEI2_MEISYO	/*12*/13		// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
#define ID_COL_151I_KEI2_VAL	/*19*/21		// 計ダイアログ2行目の支払貸借料

////////////////////////////////////////////////////////////////////
////	1レコード分の構造体
//typedef struct tagRecuc_151_Tidaiyatin_151
//{
//	long	m_Seq;						// シーケンス番号
//	int		m_NumPage;					// 頁番号
//	BYTE	m_NumRow;					// 行番号
//	BYTE	m_FgFunc;					// 特殊行フラグ
//	BYTE	m_FgShow;					// 表示フラグ
//	CString	m_KeiStr;					// 計
//	long	m_NumGroup;					// グループ番号
//	long	m_KnSeq;					// 地代・家賃の区分Seq番号
//	long	m_KnOrder;					// 地代・家賃の区分Order番号
//	CString	m_KnName;					// 地代・家賃の区分名
//	CString	m_Youto;					// 借地物件の用途
//	long	m_AdSeq;					// 取引先（シーケンス番号）
//	long	m_AdOrder;					// 取引先（並び順）
//	CString	m_AdName1;					// 貸主（名称：上段）
//	CString	m_AdName2;					// 貸主（名称：下段）
//	CString	m_AdAdd1;					// 貸主（所在地：上段）
//	CString	m_AdAdd2;					// 貸主（所在地：下段）
//	long	m_SpDate;					// 支払対象期間（開始）
//	long	m_SpDate2;					// 支払対象期間（終了）
//	CString	m_Val;						// 支払貸借料
//	CString	m_Teki;						// 摘要
//	CString	m_Ad2Add1;					// 所在地1
//	CString	m_Ad2Add2;					// 所在地2
//	int		m_ShowKeiZero;				// ０円表示フラグ
//	CString	m_KnKana;					// 地代・家賃の区分
//} REC_UC_151_TIDAIYATIN, *P_REC_UC_151_TIDAIYATIN;

class CfrmUc151TidaiyatinI: public CfrmUc000Common
{
	DECLARE_DYNCREATE( CfrmUc151TidaiyatinI )

public:
	CfrmUc151TidaiyatinI();
	virtual	~CfrmUc151TidaiyatinI();
	void	virSetInitialValue();
	int		UpdateTbl151ChangeFormType(UCHIWAKE_INFO pInfoSv);

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
	enum{ IDD = IDD_YOUSHIKI151I };
	CICSDiag	m_TitleDiag;
	CICSDiag	m_PageDiag;
	CICSDiag	m_NumberDiag;
	CICSDiag	m_SaveDataDiag;
	CICSDiag	m_HeadDiag;
	CICSDiag	m_DiagNormal;
	CICSDiag	m_DiagNormal2;
	CICSDiag	m_DiagNormal3;
	CICSDiag	m_DiagKobetu;
	CICSDiag	m_DiagKobetu2;
	CICSDiag	m_DiagKobetu3;
	CICSDiag	m_DiagKei1;				// 計が1行の計専用ダイアグラム
	CICSDiag	m_DiagKei2;				// 計が2行専用のダイアグラム
	CICSDisp	m_Guide1;				// 欄外のガイド表示
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
	BOOL	virUpdateControlTblData2();
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
	BOOL	virUpdateRelationTblChengeFormTypeSave();
	void	virActivateFrame(UINT nState, CFrameWnd* pDeactivateFrame);
	void	virActivateFrameK(UINT nState, CFrameWnd* pDeactivateFrame);
	void	virKamokuAddCancel();

	int		GetFocusPosition( int nChar );	// フォーカス移動位置取得関数
	int		GetTotalRowIndex( int nChar , int intIndex , int intNext );	// 行移動制御関数
	//BOOL	CheckShiftKey();

	int		virTblSortSubSortProc(CdlgSort* pCdlgSort, CdbUc000Common* rsData);
	void	virTblGetKamokuInfo(CdbUc000Common* rsData, SORTKAMOKU_INFO* uKamoku);
	void	virUpdateTitle();
	void	virInputDecisionEditOFF();

	void	SetInputMode( int nRow, long nMode );
	void	SetDiagBackColor();
	int		CursorControl(short nChar, int sw);

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

public:
	void ComboNew(short index);
	void VScrollYoushikidiag1(short mode);
	void ComboSelYoushikidiag1(short index, LPUNKNOWN data);
	void TitleColorChg(void);
	void Date_ChkAndSet(short, int, short);
	LRESULT OnUserReDrawView(WPARAM wParam, LPARAM lParam);

	void	virDiagGetHjnData(CDatabase* pDB, HJN_REC_NEW* phjnRec);
	void	virDiagSetHjnData(int pSw, CString rtInvNo, CString rtName, CString rtAdrs1, CString rtAdrs2, CString rtKana);
	int		virCallChkInvoiceNum(int pSw, int inPage, int inRow);

	// 入力された登録番号から参照ダイアログを検索し、登録済みの場合はデータを取得して画面に表示
	int		ChkInvoiceNum(CDatabase* pDB, int pSw, int inPage, int inRow, CString pInvno="", CString pName="", CString pAdrs1="", CString pAdrs2="");

	// エラーメッセージの表示
	//LRESULT		OnUserMessageSet(WPARAM wParam, LPARAM lParam);
};
// インボイス登録番号追加対応_23/09/11 add <--
