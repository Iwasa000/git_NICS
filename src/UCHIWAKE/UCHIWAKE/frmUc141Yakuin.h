//--------------------------------------------------
//	frmUc141Yakuin.h
//--------------------------------------------------

#pragma once

#include "frmUc000Common.h"
#include "dbUc141Yakuin.h"
#include "prtUc141Yakuin.h"
#include "prtUc140YakuinJinkenhi.h"

/****************************************************************
	「役員報酬」関連
*****************************************************************/
#define	ID_ROWNORMAL_141	11		// 通常時：１頁内の合計行を含む全体の行数
#define	ID_ROWKOBETU_141	15		// 個別時：１頁内の合計行を含む全体の行数
#define	ID_ROWKEI_141	1		// 帳表固定の合計行数（1:合計あり，0:なし）
#define	ID_COLMAX_141	31		// １行内のコントロール個数
#define	ID_COLMAXDATA_141	14		// １行内の入力コントロール数
#define	ID_CTLSTARTINDEX_141	13		// １行目の入力項目の先頭インデックス番号
#define ID_T1STARTNORMAL_141	310	// 通常時：合計行①の先頭インデックス番号
#define ID_T2STARTNORMAL_141	341	// 通常時：合計行②の先頭インデックス番号
#define ID_T1STARTKOBETU_141	434	// 個別時：合計行①の先頭インデックス番号
#define ID_T2STARTKOBETU_141	465	// 個別時：合計行②の先頭インデックス番号

// 「役員報酬」テーブル名称
//	正式版では"CdbUc141Yakuin"のメンバー変数で参照可能としたい
#define	TBL_NAME_141		_T("uc_141_Yakuin")

//////////////////////////////////////////////////////////////////
//	「役員報酬」の内訳書列番号（帳表入力画面で使用）
typedef enum {
	ID_COL_141_NAMEYAKU = 1,	// 1	役職名
	ID_COL_141_NAMETANT,		// 2	担当業務
	ID_COL_141_ADNAME1,			// 3	氏名1
	ID_COL_141_KANKEI,			// 4	代表者との関係
	ID_COL_141_ADADD1,			// 5	住所1
	ID_COL_141_ADADD2,			// 6	住所2
	ID_COL_141_FPTIME,			// 7	常勤・非常勤の区別
/*	ID_COL_141_PYVAL,			// 8	報酬
	ID_COL_141_LSVAL,			// 9	損金経理（上段）
	ID_COL_141_LSVAL2,			// 10	損金経理（下段）
	ID_COL_141_DSVAL,			// 11	利益金処分
	ID_COL_141_VAL,				// 12	計（上段）
	ID_COL_141_VAL2,			// 13	計（下段）*/	// 仕様変更により以下↓の定数に差し替え
	ID_COL_141_EMVAL,			// 8	使用人職務分
	ID_COL_141_PRVAL,			// 9	定期同額給与
	ID_COL_141_BFVAL,			// 10	事前確定届出給与
	ID_COL_141_PFVAL,			// 11	利益連動給与
	ID_COL_141_ETVAL,			// 12	その他
	ID_COL_141_RTVAL,			// 13	退職給与
	ID_COL_141_VAL				// 14	役員給与計

} EnumIdFormCol141;

//////////////////////////////////////////////////////////////////
//	「役員報酬」の内訳書_集計項目
// 2006.10.23 Update start：新会社法対応
#define	ID_CALC_141_MAX		7	// 集計フィールド件数
#define	ID_CALC_141_FIELD1	_T("EmVal")		// 集計フィールド名称1
#define	ID_CALC_141_FIELD2	_T("PrVal")		// 集計フィールド名称2
#define	ID_CALC_141_FIELD3	_T("BfVal")		// 集計フィールド名称3
#define	ID_CALC_141_FIELD4	_T("PfVal")		// 集計フィールド名称4
#define	ID_CALC_141_FIELD5	_T("EtVal")		// 集計フィールド名称5
#define	ID_CALC_141_FIELD6	_T("RtVal")		// 集計フィールド名称6
#define	ID_CALC_141_FIELD7	_T("Val")		// 集計フィールド名称7
// 2006.10.23 Update end：新会社法対応

//////////////////////////////////////////////////////////////////
//	「役員報酬」の内訳書_計ダイアグラムの入力インデックス
#define ID_COL_141_KEI1_MEISYO	0				// 計ダイアログ1行目の計名称（計or合計、頁計、累計）
// midori 152503 del -->
//#define ID_COL_141_KEI1_EMVAL	3				// 計ダイアログ1行目の使用人職務分
//#define ID_COL_141_KEI1_PRVAL	4				// 計ダイアログ1行目の定期同額給与
//#define ID_COL_141_KEI1_BFVAL	5				// 計ダイアログ1行目の事前確定届出給与
//#define ID_COL_141_KEI1_PFVAL	6				// 計ダイアログ1行目の利益連動給与
//#define ID_COL_141_KEI1_ETVAL	7				// 計ダイアログ1行目のその他
//#define ID_COL_141_KEI1_RTVAL	8				// 計ダイアログ1行目の退職給与
//#define ID_COL_141_KEI1_VAL		9				// 計ダイアログ1行目の役員給与計
//#define ID_COL_141_KEI2_MEISYO	13				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
//#define ID_COL_141_KEI2_EMVAL	16				// 計ダイアログ2行目の使用人職務分
//#define ID_COL_141_KEI2_PRVAL	17				// 計ダイアログ2行目の定期同額給与
//#define ID_COL_141_KEI2_BFVAL	18				// 計ダイアログ2行目の事前確定届出給与
//#define ID_COL_141_KEI2_PFVAL	19				// 計ダイアログ2行目の利益連動給与
//#define ID_COL_141_KEI2_ETVAL	20				// 計ダイアログ2行目のその他
//#define ID_COL_141_KEI2_RTVAL	21				// 計ダイアログ2行目の退職給与
//#define ID_COL_141_KEI2_VAL		22				// 計ダイアログ2行目の役員給与計
// midori 152503 del <--
// midori 152503 add -->
#define ID_COL_141_KEI1_EMVAL	4				// 計ダイアログ1行目の使用人職務分
#define ID_COL_141_KEI1_PRVAL	6				// 計ダイアログ1行目の定期同額給与
#define ID_COL_141_KEI1_BFVAL	8				// 計ダイアログ1行目の事前確定届出給与
#define ID_COL_141_KEI1_PFVAL	10				// 計ダイアログ1行目の利益連動給与
#define ID_COL_141_KEI1_ETVAL	12				// 計ダイアログ1行目のその他
#define ID_COL_141_KEI1_RTVAL	15				// 計ダイアログ1行目の退職給与
#define ID_COL_141_KEI1_VAL		16				// 計ダイアログ1行目の役員給与計
#define ID_COL_141_KEI2_MEISYO	20				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
#define ID_COL_141_KEI2_EMVAL	24				// 計ダイアログ2行目の使用人職務分
#define ID_COL_141_KEI2_PRVAL	26				// 計ダイアログ2行目の定期同額給与
#define ID_COL_141_KEI2_BFVAL	28				// 計ダイアログ2行目の事前確定届出給与
#define ID_COL_141_KEI2_PFVAL	30				// 計ダイアログ2行目の利益連動給与
#define ID_COL_141_KEI2_ETVAL	32				// 計ダイアログ2行目のその他
#define ID_COL_141_KEI2_RTVAL	35				// 計ダイアログ2行目の退職給与
#define ID_COL_141_KEI2_VAL		36				// 計ダイアログ2行目の役員給与計
// midori 152503 add <--

//////////////////////////////////////////////////////////////////
//	1レコード分の構造体
typedef struct tagRecuc_141_Yakuin_141
{
	long	m_Seq;	// シーケンス番号
	int	m_NumPage;	// 頁番号
	BYTE	m_NumRow;	// 行番号
	BYTE	m_FgFunc;	// 特殊行フラグ
	BYTE	m_FgShow;	// 表示フラグ
	CString	m_KeiStr;	// 計
	long	m_NumGroup;	// グループ番号
	CString	m_NameYaku;	// 役職名
	CString	m_NameTant;	// 担当業務
	long	m_AdSeq;	// 氏名/住所：取引先（シーケンス番号）
	long	m_AdOrder;	// 氏名/住所：取引先（並び順）
	CString	m_AdName1;	// 氏名（名称：上段）
	CString	m_AdName2;	// 氏名（名称：下段）
	CString	m_AdAdd1;	// 住所（所在地：上段）
	CString	m_AdAdd2;	// 住所（所在地：下段）
	CString	m_Kankei;	// 代表者との関係
	BYTE	m_FpTime;	// 常勤・非常勤の別
	CString	m_EmVal;	// 使用人職務分				// 2006.10.23：新会社法対応
	CString	m_PrVal;	// 定期同額給与				// 2006.10.23：新会社法対応
	CString	m_BfVal;	// 事前確定届出給与			// 2006.10.23：新会社法対応
	CString	m_PfVal;	// 利益連動給与				// 2006.10.23：新会社法対応
	CString	m_EtVal;	// その他					// 2006.10.23：新会社法対応
	CString	m_RtVal;	// 退職給与					// 2006.10.23：新会社法対応
	CString	m_Val;		// 役員給与計				// 2006.10.23：新会社法対応
	BYTE	m_FgInput;	// ｢役員給与計｣ チェックフラグ
	int		m_ShowKeiZero;	// ０円表示フラグ
} REC_UC_141_YAKUIN, *P_REC_UC_141_YAKUIN;

class CfrmUc141Yakuin: public CfrmUc000Common
{
	DECLARE_DYNCREATE( CfrmUc141Yakuin )

public:
	CfrmUc141Yakuin();
	virtual	~CfrmUc141Yakuin();
	void	virSetInitialValue();
	int		UpdateTbl141ChangeFormType();

private:
	void	virRowButtonCopyProc();
	int		virRecBufSaveData( int inPage, int inRow );
	int		virRecBufSaveDataForPasteInsert( int inPage, int inRow );
	int		virRecBufSaveDataForPaste( int inPage, int inRow );
	int		RecBufClearData( P_REC_UC_141_YAKUIN inRecData );
	int		RecBufSaveDataSub( int inPage, int inRow, P_REC_UC_141_YAKUIN inSaveData );
	int		RecBufSetData( CdbUc141Yakuin* inTbl );
	int		RecBufClearAllData();

	// 1ページ分のデータ格納用配列
	// 配列の添え字と、行番号を合わせるため、最大行+1とした
	REC_UC_141_YAKUIN	m_ReadData[ID_ROWKOBETU_141 + 1];
	REC_UC_141_YAKUIN	m_CopyData;		// 行コピー用バッファ
	int	m_nCopyFlag;					// コピーフラグ（0：コピーデータがない／1：コピーデータがある）

	int	m_intT1StartIndexHide;			// 通常/個別帳表にて、非表示にしているDiagの合計行①の先頭インデックス
	int	m_intT2StartIndexHide;			// 通常/個別帳表にて、非表示にしているDiagの合計行②の先頭インデックス

public:
	enum{ IDD = IDD_YOUSHIKI141 };
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
	void	virInitialControlFgInputCell( int nRow, int nFgFunc, BYTE bFgInput );
	void	virUpdateControlTitle();
	void	UpdateControlPage();
	BOOL	virUpdateControlTblData();
// midori 161108 add -->
	BOOL	virUpdateControlTblData2();
// midori 161108 add <--
	void	UpdateControlRowData( CdbUc141Yakuin* inTbl );
	void	UpdateControlRowTotal( CdbUc141Yakuin* inTbl );
	CdbUc000Common*	virGetDataTableObject();
	void	virGetCurrentInfo(CURRENT_INFO* uCurInfo);
	void	virFncCellCopyProc(CdbUc000Common* rsData);				// F6 :項目複写
	int		virFncReferenceGetType(int intCol, int* intGrSeq, long* lngItemSeq, int* intAdd1, int* intAdd2, int* intAdd1Col, int* intAdd2Col, REFERENCE_INFO* pudReference = 0);	// F9 :参照
	void	virFncReferenceSetICSData(CdlgAddressSearch* dlgAddd);	// F9:参照
	void	virFncReferenceSetICSData(CdlgYakuZokuSelect* dlg, int type);
	int		virUpdateRelationTblChengeFormType();

	int		GetFocusPosition( int nChar );	// フォーカス移動位置取得関数
	int		GetTotalRowIndex( int nChar , int intIndex , int intNext );	// 行移動制御関数
	BOOL	CheckShiftKey();

	void	CheckFgInput();
	void	SetAutoCalc();
	CString GetAddVal( CString szVal1, CString szVal2, CString szVal3, CString szVal4, CString szVal5 );

	void	virOnUpdateButtonF6(BOOL* bFncEnable);
	void	virUpdateTitle();
	void	virInputDecisionEditOFF();

//	CString CheckOutsideValue( CICSDiag* pDiag, int nIndex );
//	BOOL	CheckOutsideValue( CICSDiag* pDiag, int nIndex, int nRow );
//	BOOL	CheckOutsideValueSub( CICSDiag* pDiag, int nIndex );

	void	SetInputMode( int nRow, long nMode );
	void	SetDiagBackColor();
	CString GetAutoCalc( int nRow , CString strVal1 , CString strVal2 , CString strVal3 , CString strVal4 , CString strVal5 );
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

