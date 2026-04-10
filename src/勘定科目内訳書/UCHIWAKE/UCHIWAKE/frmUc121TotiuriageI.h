// インボイス登録番号追加対応_23/09/11 add -->
//--------------------------------------------------
//	frmUc121Totiuriage.h
//--------------------------------------------------

#pragma once

#include "frmUc000Common.h"
#include "frmUc121Totiuriage.h"
#include "dbUc121Totiuriage.h"
#include "prtUc121Totiuriage.h"

/****************************************************************
	「土地売上」関連
*****************************************************************/
//#define	ID_ROWNORMAL_121	10			// 通常時：１頁内の合計行を含む全体の行数
//#define	ID_ROWKOBETU_121	10			// 個別時：１頁内の合計行を含む全体の行数
//#define	ID_ROWKEI_121		0			// 帳表固定の合計行数（1:合計あり，0:なし）
#define	ID_COLMAX_121I			32			// １行内のコントロール個数
#define	ID_COLMAXDATA_121I		16			// １行内の入力コントロール数
#define	ID_CTLSTARTINDEX_121I	12			// １行目の入力項目の先頭インデックス番号
#define ID_T1STARTNORMAL_121I	/*290*/320	// 通常時：合計行①の先頭インデックス番号
#define ID_T2STARTNORMAL_121I	/*319*/352	// 通常時：合計行②の先頭インデックス番号
#define ID_T1STARTKOBETU_121I	/*290*/320	// 個別時：合計行①の先頭インデックス番号
#define ID_T2STARTKOBETU_121I	/*319*/352	// 個別時：合計行②の先頭インデックス番号

//// 「土地売上」テーブル名称
////	正式版では"CdbUc121Totiuriage"のメンバー変数で参照可能としたい
//#define	TBL_NAME_121		_T("uc_121_Totiuriage")

//////////////////////////////////////////////////////////////////
//	「土地売上」の内訳書列番号（帳表入力画面で使用）
typedef enum {
	ID_COL_121I_KUBUN = 1,	// 1	区分
	ID_COL_121I_AD2ADD1,	// 2	商品の所在地1
	ID_COL_121I_AD2ADD2,	// 3	商品の所在地2
	ID_COL_121I_HDNAME,		// 4	地目
	ID_COL_121I_AREA,		// 5	総面積
	ID_COL_121I_SPDATE,		// 6	売上年月
	ID_COL_121I_INVONOT,	// 7	登録番号（T選択）
	ID_COL_121I_INVONO,		// 8	登録番号
	ID_COL_121I_ADNAME1,	// 9	氏名1
	ID_COL_121I_ADADD1,		// 10	住所1
	ID_COL_121I_ADADD2,		// 11	住所2
	ID_COL_121I_AREA2,		// 12	売上面積
	ID_COL_121I_VAL,		// 13	売上金額
	ID_COL_121I_VAL2,		// 14	売上金額
	ID_COL_121I_SPDATE2,	// 15	売上商品の取得年(元号)
	ID_COL_121I_SPDATE3		// 16	売上商品の取得年(年)
} EnumIdFormCol121I;

////////////////////////////////////////////////////////////////////
////	「土地売上」の内訳書_集計項目
//#define	ID_CALC_121_MAX		2	// 集計フィールド件数
//#define	ID_CALC_121_FIELD1	_T("Val")	// 集計フィールド名称1
//#define	ID_CALC_121_FIELD2	_T("Val2")	// 集計フィールド名称2

//////////////////////////////////////////////////////////////////
//	「土地売上」の内訳書_計ダイアグラムの入力インデックス
#define ID_COL_121I_KEI1_MEISYO	1				// 計ダイアログ1行目の計名称（計or合計、頁計、累計）
#define ID_COL_121I_KEI1_VAL	/*8*/9			// 計ダイアログ1行目の売上金額
#define ID_COL_121I_KEI1_VAL2	/*9*/10			// 計ダイアログ1行目の売上金額
#define ID_COL_121I_KEI2_MEISYO	15				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
#define ID_COL_121I_KEI2_VAL	/*21*/23		// 計ダイアログ2行目の売上金額
#define ID_COL_121I_KEI2_VAL2	/*22*/24		// 計ダイアログ2行目の売上金額

////////////////////////////////////////////////////////////////////
////	1レコード分の構造体
//typedef struct tagRecuc_121_Totiuriage_121
//{
//	long	m_Seq;	// シーケンス番号
//	int	m_NumPage;	// 頁番号
//	BYTE	m_NumRow;	// 行番号
//	BYTE	m_FgFunc;	// 特殊行フラグ
//	BYTE	m_FgShow;	// 表示フラグ
//	CString	m_KeiStr;	// 計
//	long	m_NumGroup;	// グループ番号
//	long	m_KnSeq;	// 区分のSeq番号
//	long	m_KnOrder;	// 区分のOrder番号
//	CString	m_KnName;	// 区分の文言
//	CString	m_HdName;	// 地目
//	CString	m_Area;	// 総面積
//	long	m_SpDate;	// 売上年月
//	long	m_AdSeq;	// 取引先（シーケンス番号）
//	long	m_AdOrder;	// 取引先（並び順）
//	CString	m_AdName1;	// 売上先（名称１）
//	CString	m_AdName2;	// 売上先（名称２）
//	CString	m_AdAdd1;	// 売上先（所在地：上段）
//	CString	m_AdAdd2;	// 売上先（所在地：下段）
//	CString	m_Area2;	// 売上面積
//	CString	m_Val;	// 売上金額（上段）
//	CString	m_Val2;	// 売上金額（下段）
//	long	m_SpDate2;	// 売上商品の取得年
//	CString	m_Ad2Add1;	// 商品の所在地（上段）
//	CString	m_Ad2Add2;	// 商品の所在地（下段）
//	int		m_ShowKeiZero;	// ０円表示フラグ
//	CString	m_KnKana;	// 区分
//} REC_UC_121_TOTIURIAGE, *P_REC_UC_121_TOTIURIAGE;

class CfrmUc121TotiuriageI: public CfrmUc000Common
{
	DECLARE_DYNCREATE( CfrmUc121TotiuriageI )

public:
	CfrmUc121TotiuriageI();
	virtual	~CfrmUc121TotiuriageI();
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
	int	m_nCopyFlag;						// コピーフラグ（0：コピーデータがない／1：コピーデータがある）
	int	m_nOldCol;							// 旧列番号
	int	m_nOld_combo;						// 売上商品の取得年(元号)

public:
	enum{ IDD = IDD_YOUSHIKI121I };
	CICSDiag	m_TitleDiag;
	CICSDiag	m_PageDiag;
	CICSDiag	m_NumberDiag;
	CICSDiag	m_SaveDataDiag;
	CICSDiag	m_HeadDiag;
	CICSDiag*	m_Diag;						// m_Diag1とm_Diag2を受けるポインタ
	CICSDiag	m_Diag1;					// 計が1行の明細ダイアログ
	CICSDiag	m_Diag2;					// 計が2行の明細ダイアログ
	CICSDiag*	m_DiagKei;					// m_DiagKei1とm_DiagKei2を受けるポインタ
	CICSDiag	m_DiagKei1;					// 計が1行の計専用ダイアグラム
	CICSDiag	m_DiagKei2;					// 計が2行専用のダイアグラム
	CICSDiag	m_Diag3;					// 計なしの明細ダイアログ
	CICSDisp	m_Guide1;					// 欄外のガイド表示

	
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
	void	UpdateControlRowData( CdbUc121Totiuriage* inTbl );
	void	UpdateControlRowTotal( CdbUc121Totiuriage* inTbl );
	CdbUc000Common*	virGetDataTableObject();
	void	virGetCurrentInfo(CURRENT_INFO* uCurInfo);
	void	virFncCellCopyProc(CdbUc000Common* rsData);									// F6 :項目複写
	int		virFncReferenceGetType(int intCol, int* intGrSeq, long* lngItemSeq, int* intAdd1, int* intAdd2, int* intAdd1Col, int* intAdd2Col, REFERENCE_INFO* pudReference = 0);	// F9 :参照
	void	virFncReferenceSetData(CdlgReference* dlgRef, int intRefType, int intCol);	// F9 :参照
	void	virFncReferenceUpdateTbl(int intRefType, int nRefMode = 0);					// F9 :参照
	void	virFncReferenceSetICSData(CdlgAddressSearch* dlgAddd);						// F9 :参照
	void	virActivateFrame(UINT nState, CFrameWnd* pDeactivateFrame);
	void	virActivateFrameK(UINT nState, CFrameWnd* pDeactivateFrame);
	void	virKamokuAddCancel();

	int		GetFocusPosition( int nChar );	// フォーカス移動位置取得関数
	int		GetTotalRowIndex( int nChar , int intIndex , int intNext );	// 行移動制御関数
	//BOOL	CheckShiftKey();

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
	int		CursorControl(short nChar, int sw);

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
