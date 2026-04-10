//--------------------------------------------------
//	frmUc042Kasituke.h
//--------------------------------------------------

#pragma once

#include "frmUc000Common.h"
// midori kanso_0318 add -->
#include "frmUc042Kasituke.h"
// midori kanso_0318 add <--
#include "dbUc042Kasituke.h"
#include "prtUc042Kasituke.h"
#include "prtUc040KaribaraiKasituke.h"

// midori kanso_0318 del -->
///****************************************************************
//	「貸付金」関連
//*****************************************************************/
//#define	ID_ROWNORMAL_042		8		// 通常時：１頁内の合計行を含む全体の行数
//#define	ID_ROWKOBETU_042		19		// 個別時：１頁内の合計行を含む全体の行数
//#define	ID_ROWKEI_042			1		// 帳表固定の合計行数（1:合計あり，0:なし）
//#define	ID_COLMAX_042			20		// １行内のコントロール個数
//#define	ID_COLMAXDATA_042		9		// １行内の入力コントロール数
//#define	ID_CTLSTARTINDEX_042	8		// １行目の入力項目の先頭インデックス番号
//#define ID_T1STARTNORMAL_042	140		// 通常時：合計行①の先頭インデックス番号
//#define ID_T2STARTNORMAL_042	160		// 通常時：合計行②の先頭インデックス番号
//#define ID_T1STARTKOBETU_042	360		// 個別時：合計行①の先頭インデックス番号
//#define ID_T2STARTKOBETU_042	380		// 個別時：合計行②の先頭インデックス番号
//
//// 「貸付金」テーブル名称
////	正式版では"CdbUc042Kasituke"のメンバー変数で参照可能としたい
//#define	TBL_NAME_042		_T("uc_042_Kasituke")
//
////////////////////////////////////////////////////////////////////
////	「貸付金」の内訳書列番号（帳表入力画面で使用）
//typedef enum {
//	ID_COL_042_ADNAME1 = 1,				// 1	貸付先1
//	ID_COL_042_KANKEI,					// 2	法人・代表者との関係
//	ID_COL_042_ADADD1,					// 3	所在地1（住所）
//	ID_COL_042_ADADD2,					// 4	所在地2（住所）
//	ID_COL_042_VAL,						// 5	期末現在高
//	ID_COL_042_RISOKU,					// 6	期中の受取利息額
//	ID_COL_042_RATE,					// 7	利率
//	ID_COL_042_RIYUU,					// 8	貸付理由
//	ID_COL_042_NAIYOU,					// 9	担保の内容
//} EnumIdFormCol042;
//
////////////////////////////////////////////////////////////////////
////	「貸付金」の内訳書_集計項目
//#define	ID_CALC_042_MAX		2				// 集計フィールド件数
//#define	ID_CALC_042_FIELD1	_T("Val")		// 集計フィールド名称1
//#define	ID_CALC_042_FIELD2	_T("Risoku")	// 集計フィールド名称2
//
////////////////////////////////////////////////////////////////////
////	「役員報酬」の内訳書_計ダイアグラムの入力インデックス
//#define ID_COL_042_KEI1_MEISYO	0				// 計ダイアログ1行目の計名称（計or合計、頁計、累計）
//// midori 152503 del -->
////#define ID_COL_042_KEI1_VAL		1				// 計ダイアログ1行目の期末現在高
////#define ID_COL_042_KEI1_RISOKU	2				// 計ダイアログ1行目の期中の受取利息額
////#define ID_COL_042_KEI2_MEISYO	10				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
////#define ID_COL_042_KEI2_VAL		11				// 計ダイアログ2行目の期末現在高
////#define ID_COL_042_KEI2_RISOKU	12				// 計ダイアログ2行目の期中の受取利息額
//// midori 152503 del <--
//// midori 152503 add -->
//#define ID_COL_042_KEI1_VAL		2				// 計ダイアログ1行目の期末現在高
//#define ID_COL_042_KEI1_RISOKU	5				// 計ダイアログ1行目の期中の受取利息額
//#define ID_COL_042_KEI2_MEISYO	12				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
//#define ID_COL_042_KEI2_VAL		14				// 計ダイアログ2行目の期末現在高
//#define ID_COL_042_KEI2_RISOKU	17				// 計ダイアログ2行目の期中の受取利息額
//// midori 152503 add <--
// midori kanso_0318 del <--
// midori kanso_0318 add -->
#define	ID_COLMAX_042K			17		// １行内のコントロール個数
#define	ID_COLMAXDATA_042K		9		// １行内の入力コントロール数
#define	ID_CTLSTARTINDEX_042K	8		// １行目の入力項目の先頭インデックス番号
#define ID_T1STARTNORMAL_042K	119/*140*/		// 通常時：合計行①の先頭インデックス番号
#define ID_T2STARTNORMAL_042K	136/*160*/		// 通常時：合計行②の先頭インデックス番号
#define ID_T1STARTKOBETU_042K	306/*360*/		// 個別時：合計行①の先頭インデックス番号
#define ID_T2STARTKOBETU_042K	323/*380*/		// 個別時：合計行②の先頭インデックス番号

//	「貸付金」の内訳書列番号（帳表入力画面で使用）
typedef enum {
	ID_COL_042K_ADNAME1 = 1,			// 1	貸付先1
	ID_COL_042K_ADADD1,					// 2	所在地1（住所）
	ID_COL_042K_ADADD2,					// 3	所在地2（住所）
	ID_COL_042K_KANKEI,					// 4	法人・代表者との関係
	ID_COL_042K_VAL,					// 5	期末現在高
	ID_COL_042K_RISOKU,					// 6	期中の受取利息額
	ID_COL_042K_RATE,					// 7	利率
	ID_COL_042K_RIYUU,					// 8	貸付理由
	ID_COL_042K_NAIYOU,					// 9	担保の内容
} EnumIdFormCol042K;

#define ID_COL_042K_KEI1_MEISYO		0		// 計ダイアログ1行目の計名称（計or合計、頁計、累計）
//#define ID_COL_042K_KEI1_VAL		4		// 計ダイアログ1行目の期末現在高
//#define ID_COL_042K_KEI1_RISOKU		6		// 計ダイアログ1行目の期中の受取利息額
//#define ID_COL_042K_KEI2_MEISYO		13		// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
//#define ID_COL_042K_KEI2_VAL		17		// 計ダイアログ2行目の期末現在高
//#define ID_COL_042K_KEI2_RISOKU		19		// 計ダイアログ2行目の期中の受取利息額
#define ID_COL_042K_KEI1_VAL		2		// 計ダイアログ1行目の期末現在高
#define ID_COL_042K_KEI1_RISOKU		4		// 計ダイアログ1行目の期中の受取利息額
#define ID_COL_042K_KEI2_MEISYO		9		// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
#define ID_COL_042K_KEI2_VAL		11		// 計ダイアログ2行目の期末現在高
#define ID_COL_042K_KEI2_RISOKU		13		// 計ダイアログ2行目の期中の受取利息額
// midori kanso_0318 add <--

// midori kanso_0318 del -->
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
//// midori 152137 add -->
//	CString	m_AdKana;					// 貸付先
//// midori 152137 add <--
//} REC_UC_042_KASITUKE, *P_REC_UC_042_KASITUKE;
// midori kanso_0318 del <--

class CfrmUc042KasitukeK: public CfrmUc000Common
{
	DECLARE_DYNCREATE( CfrmUc042KasitukeK )

public:
	CfrmUc042KasitukeK();
	virtual	~CfrmUc042KasitukeK();
	void	virSetInitialValue();
// midori 152770 del -->
//	int		UpdateTbl042ChangeFormType();
// midori 152770 del <--
// midori 152770 add -->
	int		UpdateTbl042ChangeFormType(UCHIWAKE_INFO pInfoSv);
// midori 152770 add <--

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
	enum{ IDD = IDD_YOUSHIKI042K };

	CICSDiag	m_TitleDiag;
	CICSDiag	m_PageDiag;
	CICSDiag	m_NumberDiag;
// midori 161108 add -->
	CICSDiag	m_SaveDataDiag;
// midori 161108 add <--
	CICSDiag	m_HeadDiag;
	CICSDiag	m_DiagNormal;
	CICSDiag	m_DiagNormal2;
	CICSDiag	m_DiagKobetu;
	CICSDiag	m_DiagKobetu2;
	CICSDiag	m_DiagKei1;				// 計が1行の計専用ダイアグラム
	CICSDiag	m_DiagKei2;				// 計が2行専用のダイアグラム
	CICSDiag*	m_pDiagHide;			// 通常/個別帳表にて、非表示にしているDiag
// 修正No.157626 add -->
	CICSDisp	m_Guide1;				// 欄外のガイド表示
// 修正No.157626 add <--

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
	void	UpdateControlRowData( CdbUc042Kasituke* inTbl );
	void	UpdateControlRowTotal( CdbUc042Kasituke* inTbl );
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

	int		GetFocusPosition( int nChar );	// フォーカス移動位置取得関数
	int		GetTotalRowIndex( int nChar , int intIndex , int intNext );	// 行移動制御関数
	BOOL	CheckShiftKey();

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
// midori 151405 add -->
	int		CursorControl(short nChar, int sw);
// midori 151405 add <--
// midori 190301 add -->
	//void	DataConver( void );
// midori 190301 add <--

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
	LRESULT OnUserReDrawView(WPARAM wParam, LPARAM lParam);	// 修正No.162446
};
