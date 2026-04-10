//--------------------------------------------------
//	frmUc101Kariukekin.h
//--------------------------------------------------

#pragma once

#include "frmUc000Common.h"
#include "dbUc101Kariukekin.h"
#include "prtUc101Kariukekin.h"
#include "prtUc100KariukekinGensen.h"

/****************************************************************
	「仮受金」関連
*****************************************************************/
#define	ID_ROWNORMAL_101		12		// 通常時：１頁内の合計行を含む全体の行数
#define	ID_ROWKOBETU_101		24		// 個別時：１頁内の合計行を含む全体の行数
#define	ID_ROWKEI_101			0		// 帳表固定の合計行数（1:合計あり，0:なし）
#define	ID_COLMAX_101			13		// １行内のコントロール個数
#define	ID_COLMAXDATA_101		7		// １行内の入力コントロール数
#define	ID_CTLSTARTINDEX_101	6		// １行目の入力項目の先頭インデックス番号
#define	ID_T1STARTNORMAL_101	156		// 合計行①の先頭インデックス番号（通常）
#define	ID_T2STARTNORMAL_101	169		// 合計行②の先頭インデックス番号（通常）
#define	ID_T1STARTKOBETU_101	312		// 合計行①の先頭インデックス番号（個別）
#define	ID_T2STARTKOBETU_101	325		// 合計行②の先頭インデックス番号（個別）

// 「仮受金」テーブル名称
//	正式版では"CdbUc101Kariukekin"のメンバー変数で参照可能としたい
#define	TBL_NAME_101		_T("uc_101_Kariukekin")

//////////////////////////////////////////////////////////////////
//	「仮受金」の内訳書列番号（帳表入力画面で使用）
typedef enum {
	ID_COL_101_KNSEQ = 1,				// 1	科目
	ID_COL_101_ADNAME1,					// 2	名称1
	ID_COL_101_ADADD1,					// 3	所在地1
	ID_COL_101_ADADD2,					// 4	所在地2
	ID_COL_101_KANKEI,					// 5	法人・代表者との関係
	ID_COL_101_VAL,						// 6	金額
	ID_COL_101_NAIYOU,					// 7	取引の内容
} EnumIdFormCol101;

//////////////////////////////////////////////////////////////////
//	「仮受金」の内訳書_集計項目
#define	ID_CALC_101_MAX		1	// 集計フィールド件数
#define	ID_CALC_101_FIELD1	_T("Val")	// 集計フィールド名称1

//////////////////////////////////////////////////////////////////
//	「仮受金」の内訳書_計ダイアグラムの入力インデックス
#define ID_COL_101_KEI1_MEISYO	0				// 計ダイアログ1行目の計名称（計or合計、頁計、累計）
// midori 152503 del -->
//#define ID_COL_101_KEI1_VAL		4				// 計ダイアログ1行目の金額
//#define ID_COL_101_KEI2_MEISYO	6				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
//#define ID_COL_101_KEI2_VAL		10				// 計ダイアログ2行目の金額
// midori 152503 del <--
// midori 152503 add -->
#define ID_COL_101_KEI1_VAL		5				// 計ダイアログ1行目の金額
#define ID_COL_101_KEI2_MEISYO	7				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
#define ID_COL_101_KEI2_VAL		12				// 計ダイアログ2行目の金額
// midori 152503 add <--

//////////////////////////////////////////////////////////////////
//	1レコード分の構造体
typedef struct tagRecuc_101_Kariukekin_101
{
	long	m_Seq;
	int		m_NumPage;
	BYTE	m_NumRow;
	BYTE	m_FgFunc;
	BYTE	m_FgShow;
	CString	m_KeiStr;
	long	m_NumGroup;
	long	m_KnSeq;
	long	m_KnOrder;
	CString	m_KnName;
	long	m_AdSeq;
	long	m_AdOrder;
	CString	m_AdName1;
	CString	m_AdName2;
	CString	m_AdAdd1;
	CString	m_AdAdd2;
	CString	m_Kankei;
	CString	m_Val;
	CString	m_Naiyou;
	int		m_ShowKeiZero;
// midori 152137 add -->
	CString	m_KnKana;		// 科目
	CString	m_AdKana;		// 振出人
// midori 152137 add <--
	CString	m_InvNo;		// 登録番号(法人番号)	インボイス登録番号追加対応_23/10/20 add
} REC_UC_101_KARIUKEKIN, *P_REC_UC_101_KARIUKEKIN;

class CfrmUc101Kariukekin: public CfrmUc000Common
{
	DECLARE_DYNCREATE( CfrmUc101Kariukekin )

public:
	CfrmUc101Kariukekin();
	virtual	~CfrmUc101Kariukekin();
	void	virSetInitialValue();
// midori 152770 del -->
//	int		UpdateTbl101ChangeFormType();
// midori 152770 del <--
// midori 152770 add -->
	int		UpdateTbl101ChangeFormType(UCHIWAKE_INFO pInfoSv);
// midori 152770 add <--

private:
	void	virRowButtonCopyProc();
	int		virRecBufSaveData( int inPage, int inRow );
	int		virRecBufSaveDataForPasteInsert( int inPage, int inRow );
	int		virRecBufSaveDataForPaste( int inPage, int inRow );
	int		RecBufClearData( P_REC_UC_101_KARIUKEKIN inRecData );
	int		RecBufSaveDataSub( int inPage, int inRow, P_REC_UC_101_KARIUKEKIN inSaveData );
	int		RecBufSetData( CdbUc101Kariukekin* inTbl );
	int		RecBufClearAllData();

	// 1ページ分のデータ格納用配列
	// 配列の添え字と、行番号を合わせるため、最大行+1とした
	REC_UC_101_KARIUKEKIN	m_ReadData[ID_ROWKOBETU_101 + 1];
	REC_UC_101_KARIUKEKIN	m_CopyData;		// 行コピー用バッファ
	int	m_nCopyFlag;					// コピーフラグ（0：コピーデータがない／1：コピーデータがある）

	int	m_intT1StartIndexHide;			// 通常/個別帳表にて、非表示にしているDiagの合計行①の先頭インデックス
	int	m_intT2StartIndexHide;			// 通常/個別帳表にて、非表示にしているDiagの合計行②の先頭インデックス

public:
	enum{ IDD = IDD_YOUSHIKI101 };
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
	void	UpdateControlRowData( CdbUc101Kariukekin* inTbl );
	void	UpdateControlRowTotal( CdbUc101Kariukekin* inTbl );
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
// 要通知案件修正 21/07/09 add -->
	void	virActivateFrameK(UINT nState, CFrameWnd* pDeactivateFrame);
	void	virKamokuAddCancel();
// 要通知案件修正 21/07/09 add <--

	int		GetFocusPosition( int nChar );	// フォーカス移動位置取得関数
	int		GetTotalRowIndex( int nChar , int intIndex , int intNext );	// 行移動制御関数
	BOOL	CheckShiftKey();

	void	virTblEditIkkatuLine(int nType, CdbUc000Common* rsData, CString strName, CALCKEI_INFO uCalcKei, SORTKAMOKU_INFO uKamoku);
	int		virTblSortSubSortProc(CdlgSort* pCdlgSort, CdbUc000Common* rsData);
	void	virTblGetKamokuInfo(CdbUc000Common* rsData, SORTKAMOKU_INFO* uKamoku);
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

	LRESULT OnUserReDrawView(WPARAM wParam, LPARAM lParam);	// 修正No.162446
};
