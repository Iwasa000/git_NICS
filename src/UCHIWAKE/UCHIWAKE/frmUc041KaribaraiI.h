// インボイス登録番号追加対応_23/09/11 add -->
//--------------------------------------------------
//	frmUc041KaribaraiI.h
//--------------------------------------------------

#pragma once

#include "frmUc000Common.h"
#include "frmUc041Karibarai.h"
#include "dbUc041Karibarai.h"
#include "prtUc041Karibarai.h"
#include "prtUc040KaribaraiKasituke.h"

/****************************************************************
	「仮払金」関連
//*****************************************************************/
//#define	ID_ROWNORMAL_041		11		// 通常時：１頁内の合計行を含む全体の行数
//#define	ID_ROWKOBETU_041		29		// 個別時：１頁内の合計行を含む全体の行数
//#define	ID_ROWKEI_041			0		// 帳表固定の合計行数（1:合計あり，0:なし）
#define	ID_COLMAX_041I			16			// １行内のコントロール個数
#define	ID_COLMAXDATA_041I		9			// １行内の入力コントロール数
#define	ID_CTLSTARTINDEX_041I	7			// １行目の入力項目の先頭インデックス番号
#define	ID_T1STARTNORMAL_041I	/*143*/176	// 合計行①の先頭インデックス番号（通常）
#define	ID_T2STARTNORMAL_041I	/*156*/192	// 合計行②の先頭インデックス番号（通常）
#define	ID_T1STARTKOBETU_041I	/*377*/464	// 合計行①の先頭インデックス番号（個別）
#define	ID_T2STARTKOBETU_041I	/*390*/480	// 合計行②の先頭インデックス番号（個別）

//// 「仮払金」テーブル名称
////	正式版では"CdbUc041Karibarai"のメンバー変数で参照可能としたい
//#define	TBL_NAME_041		_T("uc_041_Karibarai")

//////////////////////////////////////////////////////////////////
//	「仮払金」の内訳書列番号（帳表入力画面で使用）
typedef enum {
	ID_COL_041I_ADSEQ = 1,				// 1	科目
	ID_COL_041I_INVONOT,				// 2	登録番号（T選択）
	ID_COL_041I_INVONO,					// 2	登録番号
	ID_COL_041I_ADNAME1,				// 3	名称1(氏名)
	ID_COL_041I_ADADD1,					// 4	所在地1(住所)
	ID_COL_041I_ADADD2,					// 5	所在地2(住所)
	ID_COL_041I_KANKEI,					// 6	法人・代表者との関係
	ID_COL_041I_VAL,					// 7	期末現在高
	ID_COL_041I_NAIYOU,					// 8	取引の内容
} EnumIdFormCol041I;

////////////////////////////////////////////////////////////////////
////	「仮払金」の内訳書_集計項目
//#define	ID_CALC_041_MAX		1	// 集計フィールド件数
//#define	ID_CALC_041_FIELD1	_T("Val")	// 集計フィールド名称1

//////////////////////////////////////////////////////////////////
//	「仮払金」の内訳書_計ダイアグラムの入力インデックス
#define ID_COL_041I_KEI1_MEISYO		0			// 計ダイアログ1行目の計名称（計or合計、頁計、累計）
#define ID_COL_041I_KEI1_VAL		/*5*/6		// 計ダイアログ1行目の期末現在高
#define ID_COL_041I_KEI2_MEISYO		/*7*/8		// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
#define ID_COL_041I_KEI2_VAL		/*12*/14	// 計ダイアログ2行目の期末現在高

////////////////////////////////////////////////////////////////////
////	1レコード分の構造体
//typedef struct tagRecuc_041_Karibarai_041
//{
//	long	m_Seq;
//	int	m_NumPage;
//	BYTE	m_NumRow;
//	BYTE	m_FgFunc;
//	BYTE	m_FgShow;
//	CString	m_KeiStr;
//	long	m_NumGroup;
//	long	m_KnSeq;
//	long	m_KnOrder;
//	CString	m_KnName;
//	long	m_AdSeq;
//	long	m_AdOrder;
//	CString	m_AdName1;
//	CString	m_AdName2;
//	CString	m_AdAdd1;
//	CString	m_AdAdd2;
//	CString	m_Kankei;
//	CString	m_Val;
//	CString	m_Naiyou;
//	int		m_ShowKeiZero;
//	CString	m_KnKana;
//	CString	m_AdKana;
//} REC_UC_041_KARIBARAI, *P_REC_UC_041_KARIBARAI;

class CfrmUc041KaribaraiI: public CfrmUc000Common
{
	DECLARE_DYNCREATE( CfrmUc041KaribaraiI )

public:
	CfrmUc041KaribaraiI();
	virtual	~CfrmUc041KaribaraiI();
	void	virSetInitialValue();
	int		UpdateTbl041ChangeFormType(UCHIWAKE_INFO pInfoSv);

private:
	void	virRowButtonCopyProc();
	int		virRecBufSaveData( int inPage, int inRow );
	int		virRecBufSaveDataForPasteInsert( int inPage, int inRow );
	int		virRecBufSaveDataForPaste( int inPage, int inRow );
	int		RecBufClearData( P_REC_UC_041_KARIBARAI inRecData );
	int		RecBufSaveDataSub( int inPage, int inRow, P_REC_UC_041_KARIBARAI inSaveData );
	int		RecBufSetData( CdbUc041Karibarai* inTbl );
	int		RecBufClearAllData();

	// 1ページ分のデータ格納用配列
	// 配列の添え字と、行番号を合わせるため、最大行+1とした
	REC_UC_041_KARIBARAI	m_ReadData[ID_ROWKOBETU_041 + 1];
	REC_UC_041_KARIBARAI	m_CopyData;		// 行コピー用バッファ
	int		m_nCopyFlag;					// コピーフラグ（0：コピーデータがない／1：コピーデータがある）

	int		m_intT1StartIndexHide;			// 通常/個別帳表にて、非表示にしているDiagの合計行①の先頭インデックス
	int		m_intT2StartIndexHide;			// 通常/個別帳表にて、非表示にしているDiagの合計行②の先頭インデックス

public:
	enum{ IDD = IDD_YOUSHIKI041I };
	CICSDiag	m_TitleDiag;
	CICSDiag	m_PageDiag;
	CICSDiag	m_NumberDiag;
	CICSDiag	m_SaveDataDiag;
	CICSDiag	m_HeadDiag;
	CICSDiag	m_DiagNormal;
	CICSDiag	m_DiagKobetu;
	CICSDiag	m_DiagKobetu2;
	CICSDiag	m_DiagKobetu3;
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
	void	UpdateControlRowData( CdbUc041Karibarai* inTbl );
	void	UpdateControlRowTotal( CdbUc041Karibarai* inTbl );
	CdbUc000Common*	virGetDataTableObject();
	void	virGetCurrentInfo(CURRENT_INFO* uCurInfo);
	void	virFncCellCopyProc(CdbUc000Common* rsData);									// F6 :項目複写
	int		virFncReferenceGetType(int intCol, int* intGrSeq, long* lngItemSeq, int* intAdd1, int* intAdd2, int* intAdd1Col, int* intAdd2Col, REFERENCE_INFO* pudReference = 0);	// F9 :参照
	void	virFncReferenceSetData(CdlgReference* dlgRef, int intRefType, int intCol);	// F9 :参照
	void	virFncReferenceUpdateTbl(int intRefType, int nRefMode = 0);					// F9 :参照
	int		virUpdateRelationTblChengeFormType();
	BOOL	virUpdateRelationTblChengeFormTypeSave();
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
	afx_msg	void	OnSize( UINT nType, int cx, int cy );

public:
	virtual	BOOL	DestroyWindow();

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
