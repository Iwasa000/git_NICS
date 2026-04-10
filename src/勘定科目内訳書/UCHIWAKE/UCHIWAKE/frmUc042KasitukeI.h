// インボイス登録番号追加対応_23/09/11 add -->
//--------------------------------------------------
//	frmUc042KasitukeI.h
//--------------------------------------------------

#pragma once

#include "frmUc000Common.h"
#include "frmUc042Kasituke.h"
#include "dbUc042Kasituke.h"
#include "prtUc042Kasituke.h"
#include "prtUc040KaribaraiKasituke.h"

///****************************************************************
//	「貸付金」関連
//*****************************************************************/
//#define	ID_ROWNORMAL_042		8		// 通常時：１頁内の合計行を含む全体の行数
//#define	ID_ROWKOBETU_042		19		// 個別時：１頁内の合計行を含む全体の行数
//#define	ID_ROWKEI_042			1		// 帳表固定の合計行数（1:合計あり，0:なし）
#define	ID_COLMAX_042I			20			// １行内のコントロール個数
#define	ID_COLMAXDATA_042I		11			// １行内の入力コントロール数
#define	ID_CTLSTARTINDEX_042I	9			// １行目の入力項目の先頭インデックス番号
#define ID_T1STARTNORMAL_042I	/*119*/140	// 通常時：合計行①の先頭インデックス番号
#define ID_T2STARTNORMAL_042I	/*136*/160	// 通常時：合計行②の先頭インデックス番号
#define ID_T1STARTKOBETU_042I	/*306*/360	// 個別時：合計行①の先頭インデックス番号
#define ID_T2STARTKOBETU_042I	/*323*/380	// 個別時：合計行②の先頭インデックス番号
//
//// 「貸付金」テーブル名称
////	正式版では"CdbUc042Kasituke"のメンバー変数で参照可能としたい
//#define	TBL_NAME_042		_T("uc_042_Kasituke")
//
////////////////////////////////////////////////////////////////////
//	「貸付金」の内訳書列番号（帳表入力画面で使用）
typedef enum {
	ID_COL_042I_INVONOT = 1,			// 1	登録番号（T選択
	ID_COL_042I_INVONO,					// 2	登録番号
	ID_COL_042I_ADNAME1,				// 3	貸付先1
	ID_COL_042I_ADADD1,					// 4	所在地1（住所）
	ID_COL_042I_ADADD2,					// 5	所在地2（住所）
	ID_COL_042I_KANKEI,					// 6	法人・代表者との関係
	ID_COL_042I_VAL,					// 7	期末現在高
	ID_COL_042I_RISOKU,					// 8	期中の受取利息額
	ID_COL_042I_RATE,					// 9	利率
	ID_COL_042I_RIYUU,					// 10	貸付理由
	ID_COL_042I_NAIYOU,					// 11	担保の内容
} EnumIdFormCol042I;
//
////////////////////////////////////////////////////////////////////
////	「貸付金」の内訳書_集計項目
//#define	ID_CALC_042_MAX		2				// 集計フィールド件数
//#define	ID_CALC_042_FIELD1	_T("Val")		// 集計フィールド名称1
//#define	ID_CALC_042_FIELD2	_T("Risoku")	// 集計フィールド名称2
//
////////////////////////////////////////////////////////////////////
////	「貸付金」の内訳書_計ダイアグラムの入力インデックス
#define ID_COL_042I_KEI1_MEISYO		/*0*/1			// 計ダイアログ1行目の計名称（計or合計、頁計、累計）
#define ID_COL_042I_KEI1_VAL		/*2*/3			// 計ダイアログ1行目の期末現在高
#define ID_COL_042I_KEI1_RISOKU		/*4*/5			// 計ダイアログ1行目の期中の受取利息額
#define ID_COL_042I_KEI2_MEISYO		/*9*/12			// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
#define ID_COL_042I_KEI2_VAL		/*11*/14		// 計ダイアログ2行目の期末現在高
#define ID_COL_042I_KEI2_RISOKU		/*13*/16		// 計ダイアログ2行目の期中の受取利息額

////////////////////////////////////////////////////////////////////
////	1レコード分の構造体
//typedef struct tagRecuc_042_Kasituke_042
//{
//	long	m_Seq;						// シーケンス番号
//	int		m_NumPage;					// 頁番号
//	BYTE	m_NumRow;					// 行番号
//	BYTE	m_FgFunc;					// 特殊行フラグ
//	BYTE	m_FgShow;					// 表示フラグ
//	CString	m_KeiStr;					// 計
//	long	m_NumGroup;d					// グループ番号
//	long	m_AdSeq;					// 取引先（シーケンス番号）
//	long	m_AdOrder;					// 取引先（並び順）
//	CString	m_AdName1;					// 貸付先（名称：上段）
//	CString	m_AdName2;					// 貸付先（名称：下段）
//	CString	m_AdAdd1;					// 貸付先（所在地：上段）
//	CString	m_AdAdd2;					// 貸付先（所在地：下段）
//	CString	m_Kankei;					// 貸付先（代表者との関係）
//	CString	m_Val;						// 期末現在高
//	CString	m_Risoku;					// 期中の受取利息額
//	CString	m_Rate;						// 利率
//	CString	m_Riyuu;					// 貸付理由
//	CString	m_Naiyou;					// 担保の内容
//	int		m_ShowKeiZero;				// ０円表示フラグ
//	CString	m_AdKana;					// 貸付先
//} REC_UC_042_KASITUKE, *P_REC_UC_042_KASITUKE;

class CfrmUc042KasitukeI: public CfrmUc000Common
{
	DECLARE_DYNCREATE( CfrmUc042KasitukeI )

public:
	CfrmUc042KasitukeI();
	virtual	~CfrmUc042KasitukeI();
	void	virSetInitialValue();
	int		UpdateTbl042ChangeFormType(UCHIWAKE_INFO pInfoSv);

private:
	void	virRowButtonCopyProc();
	int		virRecBufSaveData( int inPage, int inRow );
	int		virRecBufSaveDataForPasteInsert( int inPage, int inRow );
	int		virRecBufSaveDataForPaste( int inPage, int inRow );
	int		RecBufClearData( P_REC_UC_042_KASITUKE inRecData );
	int		RecBufSaveDataSub( int inPage, int inRow, P_REC_UC_042_KASITUKE inSaveData );
	int		RecBufSetData( CdbUc042Kasituke* inTbl );
	int		RecBufClearAllData();

	// 1ページ分のデータ格納用配列
	// 配列の添え字と、行番号を合わせるため、最大行+1とした
	REC_UC_042_KASITUKE	m_ReadData[ID_ROWKOBETU_042 + 1];
	REC_UC_042_KASITUKE	m_CopyData;		// 行コピー用バッファ
	int	m_nCopyFlag;					// コピーフラグ（0：コピーデータがない／1：コピーデータがある）

	int	m_intT1StartIndexHide;			// 通常/個別帳表にて、非表示にしているDiagの合計行①の先頭インデックス
	int	m_intT2StartIndexHide;			// 通常/個別帳表にて、非表示にしているDiagの合計行②の先頭インデックス

public:
	enum{ IDD = IDD_YOUSHIKI042I };

	CICSDiag	m_TitleDiag;
	CICSDiag	m_PageDiag;
	CICSDiag	m_NumberDiag;
	CICSDiag	m_SaveDataDiag;
	CICSDiag	m_HeadDiag;
	CICSDiag	m_DiagNormal;
	CICSDiag	m_DiagNormal2;
	CICSDiag	m_DiagKobetu;
	CICSDiag	m_DiagKobetu2;
	CICSDiag	m_DiagKei1;				// 計が1行の計専用ダイアグラム
	CICSDiag	m_DiagKei2;				// 計が2行専用のダイアグラム
	CICSDiag*	m_pDiagHide;			// 通常/個別帳表にて、非表示にしているDiag
	CICSDisp	m_Guide1;				// 欄外のガイド表示

	
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
	void	UpdateControlRowData( CdbUc042Kasituke* inTbl );
	void	UpdateControlRowTotal( CdbUc042Kasituke* inTbl );
	CdbUc000Common*	virGetDataTableObject();
	void	virGetCurrentInfo(CURRENT_INFO* uCurInfo);
	void	virFncCellCopyProc(CdbUc000Common* rsData);									// F6 :項目複写
	int		virFncReferenceGetType(int intCol, int* intGrSeq, long* lngItemSeq, int* intAdd1, int* intAdd2, int* intAdd1Col, int* intAdd2Col, REFERENCE_INFO* pudReference = 0);	// F9 :参照
	void	virFncReferenceSetData(CdlgReference* dlgRef, int intRefType, int intCol);	// F9 :参照
	void	virFncReferenceUpdateTbl(int intRefType, int nRefMode = 0);					// F9 :参照
	int		virUpdateRelationTblChengeFormType();
	BOOL	virUpdateRelationTblChengeFormTypeSave();

	int		GetFocusPosition( int nChar );	// フォーカス移動位置取得関数
	int		GetTotalRowIndex( int nChar , int intIndex , int intNext );	// 行移動制御関数
	//BOOL	CheckShiftKey();

	void	virTblGetIkkatuSumValue(CdbUc000Common* rsData, int nFgShow, int nGroup, CALCKEI_INFO* uCalcKei);
	void	virTblEditIkkatuLine(int nType, CdbUc000Common* rsData, CString strName, CALCKEI_INFO uCalcKei, SORTKAMOKU_INFO uKamoku);
	int		virTblSortSubSortProc(CdlgSort* pCdlgSort, CdbUc000Common* rsData);
	void	virUpdateTitle();
	void	virInputDecisionEditOFF();

	void	SetInputMode( int nRow, long nMode );
	void	SetDiagBackColor();
	void	virRendoCreateData(CdbUc000Common* rsData, RENTEMP_INFO* uRenInfo);
	void	virRendoUpdateData(CdbUc000Common* rsData, RENTEMP_INFO* uRenInfo);
	void	virFncReferenceSetICSData(CdlgAddressSearch* dlgAddd);	// F9:参照
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
	void VScrollYoushikidiag1(short mode);
	void TitleColorChg(void);
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
