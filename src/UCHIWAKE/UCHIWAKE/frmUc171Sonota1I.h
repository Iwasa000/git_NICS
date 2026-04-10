// インボイス登録番号追加対応_23/09/11 add -->
//--------------------------------------------------
//	frmUc171Sonota1.h
//--------------------------------------------------

#pragma once

#include "frmUc000Common.h"
#include "dbUc171Sonota1.h"
#include "frmUc171Sonota1.h"
#include "prtUc171Sonota1.h"

/****************************************************************
	「その他1」関連
*****************************************************************/
#define	ID_COLMAX_171I			14		// １行内のコントロール個数
#define	ID_COLMAXDATA_171I		8		// １行内の入力コントロール数
#define	ID_CTLSTARTINDEX_171I	6		// １行目の入力項目の先頭インデックス番号
//#define ID_T1STARTNORMAL_171I	286		// 通常時：合計行①の先頭インデックス番号
//#define ID_T2STARTNORMAL_171I	297		// 通常時：合計行②の先頭インデックス番号
//#define ID_T1STARTKOBETU_171I	286		// 個別時：合計行①の先頭インデックス番号
//#define ID_T2STARTKOBETU_171I	297		// 個別時：合計行②の先頭インデックス番号
#define ID_T1STARTNORMAL_171I	364		// 通常時：合計行①の先頭インデックス番号
#define ID_T2STARTNORMAL_171I	378		// 通常時：合計行②の先頭インデックス番号
#define ID_T1STARTKOBETU_171I	364		// 個別時：合計行①の先頭インデックス番号
#define ID_T2STARTKOBETU_171I	378		// 個別時：合計行②の先頭インデックス番号

//// 「その他1」テーブル名称
////	正式版では"CdbUc171Sonota1"のメンバー変数で参照可能としたい
//#define	TBL_NAME_171		_T("uc_171_Sonota1")

//////////////////////////////////////////////////////////////////
//	「その他1」の内訳書列番号（帳表入力画面で使用）
typedef enum {
	ID_COL_171I_KNORDER = 1,			// 1	科目
	ID_COL_171I_INVONOT,				// 2	登録番号（T選択）
	ID_COL_171I_INVONO,					// 3	登録番号
	ID_COL_171I_ADNAME1,				// 4	名称(氏名)
	ID_COL_171I_ADADD1,					// 5	所在地(上段)
	ID_COL_171I_ADADD2,					// 6	所在地(下段)
	ID_COL_171I_VAL,					// 7	期末現在高
	ID_COL_171I_TEKI,					// 8	摘要
} EnumIdFormCol171I;

////////////////////////////////////////////////////////////////////
////	「その他1」の内訳書_集計項目
//#define	ID_CALC_171_MAX		1	// 集計フィールド件数
//#define	ID_CALC_171_FIELD1	_T("Val")	// 集計フィールド名称1

////////////////////////////////////////////////////////////////////
////	「その他1」の内訳書_金額欄の最大桁数
//#define	ID_VAL_171_DATA		11			// 期末現在高nのデータ行の最大桁数
//#define	ID_VAL_171_TOTAL	12			// 期末現在高の計行の最大桁数
//#define	ID_VAL_171_K_DATA	13			// 期末現在高nのデータ行の最大桁数
//#define	ID_VAL_171_K_TOTAL	13			// 期末現在高の計行の最大桁数

//////////////////////////////////////////////////////////////////
//	「売上高」の内訳書_計ダイアグラムの入力インデックス
#define ID_COL_171I_KEI1_MEISYO		0			// 計ダイアログ1行目の計名称（計or合計、頁計、累計）
#define ID_COL_171I_KEI1_VAL		/*4*/5		// 計ダイアログ1行目の期末現在高
#define ID_COL_171I_KEI2_MEISYO		/*8*/10		// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
#define ID_COL_171I_KEI2_VAL		/*12*/15	// 計ダイアログ2行目の期末現在高

////////////////////////////////////////////////////////////////////
////	1レコード分の構造体
//typedef struct tagRecuc_171_Sonota1_171
//{
//	long	m_Seq;						// シーケンス番号
//	int		m_NumPage;					// 頁番号
//	BYTE	m_NumRow;					// 行番号
//	BYTE	m_FgFunc;					// 特殊行フラグ
//	BYTE	m_FgShow;					// 表示フラグ
//	CString	m_KeiStr;					// 計
//	long	m_NumGroup;					// グループ番号
//	int		m_FormSeq;					// 様式シーケンス番号
//	long	m_KnSeq;					// 科目（シーケンス番号）
//	long	m_KnOrder;					// 科目（並び順）
//	CString	m_KnName;					// 科目（名称）
//	long	m_AdSeq;					// 相手先：取引先（シーケンス番号）
//	long	m_AdOrder;					// 相手先：取引先（並び順）
//	CString	m_AdName1;					// 相手先（名称：上段）
//	CString	m_AdName2;					// 相手先（名称：下段）
//	CString	m_AdAdd1;					// 相手先（所在地：上段）
//	CString	m_AdAdd2;					// 相手先（所在地：下段）
//	CString	m_Val;						// 期末現在高
//	CString	m_Teki;						// 摘要
//	int		m_ShowKeiZero;				// ０円表示フラグ
//	CString	m_KnKana;					// 科目
//} REC_UC_171_SONOTA1, *P_REC_UC_171_SONOTA1;

class CfrmUc171Sonota1I: public CfrmUc000Common
{
	DECLARE_DYNCREATE( CfrmUc171Sonota1I )

public:
	CfrmUc171Sonota1I();
	virtual	~CfrmUc171Sonota1I();
	void	virSetInitialValue();

private:
	void	virRowButtonCopyProc();
	int		virRecBufSaveData( int inPage, int inRow );
	int		virRecBufSaveDataForPasteInsert( int inPage, int inRow );
	int		virRecBufSaveDataForPaste( int inPage, int inRow );
	int		RecBufClearData( P_REC_UC_171_SONOTA1 inRecData );
	int		RecBufSaveDataSub( int inPage, int inRow, P_REC_UC_171_SONOTA1 inSaveData );
	int		RecBufSetData( CdbUc171Sonota1* inTbl );
	int		RecBufClearAllData();

	// 1ページ分のデータ格納用配列
	// 配列の添え字と、行番号を合わせるため、最大行+1とした
	REC_UC_171_SONOTA1	m_ReadData[ID_ROWKOBETU_171 + 1];
	REC_UC_171_SONOTA1	m_CopyData;		// 行コピー用バッファ
	int	m_nCopyFlag;					// コピーフラグ（0：コピーデータがない／1：コピーデータがある）

public:
	enum{ IDD = IDD_YOUSHIKI171I };
	CICSDiag	m_TitleDiag;
	CICSDiag	m_PageDiag;
	CICSDiag	m_NumberDiag;
	CICSDiag	m_SaveDataDiag;
	CICSDiag	m_HeadDiag;
	CICSDiag*	m_Diag;								// m_Diag1とm_Diag2を受けるポインタ
	CICSDiag	m_Diag1;							// 計が1行の明細ダイアログ
	CICSDiag	m_Diag2;							// 計が2行の明細ダイアログ
	CICSDiag*	m_DiagKei;							// m_DiagKei1とm_DiagKei2を受けるポインタ
	CICSDiag	m_DiagKei1;							// 計が1行の計専用ダイアグラム
	CICSDiag	m_DiagKei2;							// 計が2行専用のダイアグラム

	
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
	BOOL	virUpdateControlTblData2();
	void	UpdateControlRowData( CdbUc171Sonota1* inTbl );
	void	UpdateControlRowTotal( CdbUc171Sonota1* inTbl );
	CdbUc000Common*	virGetDataTableObject();
	void	virGetCurrentInfo(CURRENT_INFO* uCurInfo);
	void	virFncCellCopyProc(CdbUc000Common* rsData);									// F6 :項目複写
	int		virFncReferenceGetType(int intCol, int* intGrSeq, long* lngItemSeq, int* intAdd1, int* intAdd2, int* intAdd1Col, int* intAdd2Col, REFERENCE_INFO* pudReference = 0);	// F9 :参照
	void	virFncReferenceSetData(CdlgReference* dlgRef, int intRefType, int intCol);	// F9 :参照
	void	virFncReferenceSetICSData(CdlgAddressSearch* dlgAddd);						// F9 :参照
	void	virFncReferenceUpdateTbl(int intRefType, int nRefMode = 0);					// F9 :参照
	void	virActivateFrameK(UINT nState, CFrameWnd* pDeactivateFrame);
	void	virKamokuAddCancel();

	int		GetFocusPosition( int nChar );	// フォーカス移動位置取得関数
	int		GetTotalRowIndex( int nChar , int intIndex , int intNext );	// 行移動制御関数
	//BOOL	CheckShiftKey();

	void	virTblEditIkkatuLine(int nType, CdbUc000Common* rsData, CString strName, CALCKEI_INFO uCalcKei, SORTKAMOKU_INFO uKamoku);
	int		virTblSortSubSortProc(CdlgSort* pCdlgSort, CdbUc000Common* rsData);
	void	virTblGetKamokuInfo(CdbUc000Common* rsData, SORTKAMOKU_INFO* uKamoku);
	void	virUpdateTitle();
	void	virInputDecisionEditOFF();

	void	SetInputMode( int nRow, long nMode );
	void	SetDiagBackColor();
	void	virRendoCreateData(CdbUc000Common* rsData, RENTEMP_INFO* uRenInfo);
	void	virRendoUpdateData(CdbUc000Common* rsData, RENTEMP_INFO* uRenInfo);
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
