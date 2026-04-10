//--------------------------------------------------
//	frmUc142Jinkenhi.h
//--------------------------------------------------

#pragma once

#include "frmUc000Common.h"
#include "dbUc142Jinkenhi.h"
#include "prtUc142Jinkenhi.h"
#include "prtUc140YakuinJinkenhi.h"

/****************************************************************
	「人件費」関連
*****************************************************************/
#define	ID_ROWNORMAL_142	4		// 通常時：１頁内の合計行を含む全体の行数
#define	ID_ROWKOBETU_142	4		// 個別時：１頁内の合計行を含む全体の行数
#define	ID_ROWKEI_142	1		// 帳表固定の合計行数（1:合計あり，0:なし）
#define	ID_COLMAX_142	6		// １行内のコントロール個数
#define	ID_COLMAXDATA_142	2		// １行内の入力コントロール数
#define	ID_CTLSTARTINDEX_142	4		// １行目の入力項目の先頭インデックス番号
#define ID_T1STARTNORMAL_142	18	// 通常時：合計行①の先頭インデックス番号
#define ID_T2STARTNORMAL_142	18	// 通常時：合計行②の先頭インデックス番号
#define ID_T1STARTKOBETU_142	18	// 個別時：合計行①の先頭インデックス番号
#define ID_T2STARTKOBETU_142	18	// 個別時：合計行②の先頭インデックス番号

// 「人件費」テーブル名称
//	正式版では"CdbUc142Jinkenhi"のメンバー変数で参照可能としたい
#define	TBL_NAME_142		_T("uc_142_Jinkenhi")

//////////////////////////////////////////////////////////////////
//	「人件費」の内訳書列番号（帳表入力画面で使用）
typedef enum {
	ID_COL_142_VAL = 1,	// 1	総額
	ID_COL_142_VAL2,	// 2	総額のうち代表者及びその家族分
} EnumIdFormCol142;

//////////////////////////////////////////////////////////////////
//	「人件費」の内訳書_集計項目
#define	ID_CALC_142_MAX		2	// 集計フィールド件数
#define	ID_CALC_142_FIELD1	_T("Val")	// 集計フィールド名称1
#define	ID_CALC_142_FIELD2	_T("Val2")	// 集計フィールド名称2

//////////////////////////////////////////////////////////////////
//	「人件費」の内訳書_金額欄の最大桁数
#define	ID_VAL_142_DATA	11	// 総額nのデータ行の最大桁数
#define	ID_VAL_142_TOTAL	12	// 総額の計行の最大桁数
#define	ID_VAL2_142_DATA	11	// 総額のうち代表者及びその家族分nのデータ行の最大桁数
#define	ID_VAL2_142_TOTAL	12	// 総額のうち代表者及びその家族分の計行の最大桁数
// No.200903 add -->
#define	ID_VAL_142_K_DATA	13	// 総額nのデータ行の最大桁数
#define	ID_VAL_142_K_TOTAL	13	// 総額の計行の最大桁数
#define	ID_VAL2_142_K_DATA	13	// 総額のうち代表者及びその家族分nのデータ行の最大桁数
#define	ID_VAL2_142_K_TOTAL	13	// 総額のうち代表者及びその家族分の計行の最大桁数
// No.200903 add <--

//////////////////////////////////////////////////////////////////
//	1レコード分の構造体
typedef struct tagRecuc_142_Jinkenhi_142
{
	long	m_Seq;	// シーケンス番号
	int	m_NumPage;	// 頁番号
	BYTE	m_NumRow;	// 行番号
	BYTE	m_FgFunc;	// 特殊行フラグ
	BYTE	m_FgShow;	// 表示フラグ
	CString	m_KeiStr;	// 計
	long	m_NumGroup;	// グループ番号
	CString	m_Val;	// 総額
	CString	m_Val2;	// 総額のうち代表者及びその家族分
	int		m_ShowKeiZero;	// ０円表示フラグ
} REC_UC_142_JINKENHI, *P_REC_UC_142_JINKENHI;

class CfrmUc142Jinkenhi: public CfrmUc000Common
{
	DECLARE_DYNCREATE( CfrmUc142Jinkenhi )

public:
	CfrmUc142Jinkenhi();
	virtual	~CfrmUc142Jinkenhi();
	void	virSetInitialValue();
	int		UpdateTbl142ChangeFormType();

private:
	int		virRecBufSaveData( int inPage, int inRow );
	int		RecBufClearData( P_REC_UC_142_JINKENHI inRecData );
	int		RecBufSaveDataSub( int inPage, int inRow, P_REC_UC_142_JINKENHI inSaveData );
	int		RecBufSetData( CdbUc142Jinkenhi* inTbl );
	int		RecBufClearAllData();

	// 1ページ分のデータ格納用配列
	// 配列の添え字と、行番号を合わせるため、最大行+1とした
	REC_UC_142_JINKENHI	m_ReadData[ID_ROWKOBETU_142 + 1];
	REC_UC_142_JINKENHI	m_CopyData;		// 行コピー用バッファ
	int	m_nCopyFlag;					// コピーフラグ（0：コピーデータがない／1：コピーデータがある）

public:
	enum{ IDD = IDD_YOUSHIKI142 };
	CICSDiag	m_TitleDiag;
	CICSDiag	m_PageDiag;
	CICSDiag	m_NumberDiag;
	CICSDiag	m_HeadDiag;
	CICSDiag	m_Diag;

private:
	BOOL	virStartJob();
	void	virSetInitialUchiwakeInfo();
	void	virSetInitialDiag();
	BOOL	InitialControlDataArea();
	void	InitialControlTotalArea();
	void	virUpdateControlTitle();
	void	UpdateControlPage();
	BOOL	virUpdateControlTblData();
	void	UpdateControlRowData( CdbUc142Jinkenhi* inTbl );
	void	UpdateControlRowTotal( CdbUc142Jinkenhi* inTbl );
	CdbUc000Common*	virGetDataTableObject();
	void	virGetCurrentInfo(CURRENT_INFO* uCurInfo);
	int		virUpdateRelationTblChengeFormType();
	void	virRendoSumData(CdbUc000Common* rsData, RENTEMP_INFO* uRenInfo);

	int		GetFocusPosition( int nChar );	// フォーカス移動位置取得関数
	BOOL	CheckShiftKey();
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
// midori 160612 add -->
	void	fm142DiagSetAttrSpecialRow( void );
// midori 160612 add <--

public:
	virtual	BOOL	PreTranslateMessage( MSG* pMsg );

// 2009.09.08 TS自動ログオフ対応
protected:
	virtual	BOOL	virEndProc(void);
};

