//--------------------------------------------------
//	frmUc051Tanaorosi.h
//
//	2006.02.23～
//--------------------------------------------------

#ifndef	__FRMUC051TANAOROSI_H__
#define	__FRMUC051TANAOROSI_H__

#pragma once

#include "frmUc000Common.h"
#include "dbUc051Tanaoroshi.h"
#include "dlgTanaoroshi.h"
#include "prtUc051Tanaorosi.h"

#define ID_ROWNORMAL_051		25		// 通常時：１頁内の合計行を含む全体の行数
#define ID_ROWKOBETU_051		25		// 個別時：１頁内の合計行を含む全体の行数
#define ID_ROWKEI_051			1		// 帳表固定の合計行数（1:合計あり，0:なし）
#define ID_COLMAX_051			13		// １行内のコントロール個数
#define ID_COLMAXDATA_051		7		// １行内の入力コントロール数
#define ID_CTLSTARTINDEX_051	6		// １行目の入力項目の先頭インデックス番号
#define ID_T1STARTNORMAL_051	312		// 合計行①の先頭インデックス番号（通常）
#define ID_T2STARTNORMAL_051	325		// 合計行②の先頭インデックス番号（通常）
#define ID_T1STARTKOBETU_051	312		// 合計行①の先頭インデックス番号（個別）
#define ID_T2STARTKOBETU_051	325		// 合計行②の先頭インデックス番号（個別）

//	金融機関/科目のUpdateDataTable()で使用
#define TBL_NAME_051		_T("uc_051_tanaoroshi")

//	列番号（帳表入力画面で使用）
typedef enum {
	ID_COL_051_KAMOKU = 1,	//	科目
	ID_COL_051_HINNMOKU,	//	品目
	ID_COL_051_SUURYOU,		//	数量
	ID_COL_051_TANNI,		//　単位
	ID_COL_051_TANNKA,		//	単価
	ID_COL_051_KIMATU,		//	期末現在高
	ID_COL_051_TEKIYOU		//	摘要
} EnumIdFormCol051;

//	集計項目
#define ID_CALC_051_MAX			1				// 集計フィールド件数
#define ID_CALC_051_FIELD1		_T("Val")		// 集計フィールド名称１

//	計ダイアグラムの入力インデックス
#define ID_COL_051_KEI1_MEISYO	0				// 計ダイアログ1行目の計名称（計or合計、頁計、累計）
// midori 152503 del -->
//#define ID_COL_051_KEI1_KIMATU	4				// 計ダイアログ1行目の期末現在高
//#define ID_COL_051_KEI2_MEISYO	9				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
//#define ID_COL_051_KEI2_KIMATU	13				// 計ダイアログ2行目の期末現在高
// midori 152503 del <--
// midori 152503 add -->
#define ID_COL_051_KEI1_KIMATU	5				// 計ダイアログ1行目の期末現在高
#define ID_COL_051_KEI2_MEISYO	10				// 計ダイアログ2行目の計名称（計or合計、頁計、累計）
#define ID_COL_051_KEI2_KIMATU	15				// 計ダイアログ2行目の期末現在高
// midori 152503 add <--



//	1レコード分の構造体
typedef struct tagRecUc051tanaorosi
{
	//	共通
	long	m_Seq;			//	シーケンス番号	
	int		m_NumPage;		//	頁番号
	BYTE	m_NumRow;		//	行番号
	BYTE	m_FgFunc;		//	特殊行フラグ
	BYTE	m_FgShow;		//	表示フラグ
	CString	m_KeiStr;		//	計
	long	m_NumGroup;		//	グループ番号

	//	固有
	long	m_KnSeq;		//	科目（シーケンス番号）
	long	m_KnOrder;		//	科目（並び順）
	CString	m_KnName;		//	科目（名称）
	CString	m_HdName;		//	品目
	CString	m_Suuryou;		//	数量
	CString	m_Tanni;		//	単位
	CString	m_Price;		//	単価
	CString	m_Val;			//	期末現在高
	CString	m_Teki;			//	摘要
	BYTE	m_FgInput;		//	期末現在高チェックフラグ
	int		m_ShowKeiZero;	//　０円表示フラグ
// midori 152137 add -->
	CString	m_KnKana;		//	科目
// midori 152137 add <--
}REC_UC_051_TANAOROSI, *P_REC_UC_051_TANAOROSI;

class CfrmUc051Tanaorosi: public CfrmUc000Common
{
	DECLARE_DYNCREATE(CfrmUc051Tanaorosi)

public:
	CfrmUc051Tanaorosi();										//	コンストラクタ
	virtual ~CfrmUc051Tanaorosi();								//	デストラクタ
	void	virSetInitialValue();								//	帳表固有変数の初期値設定

public:
	enum{ IDD = IDD_YOUSHIKI051 };
	CICSDiag m_TitleDiag;										//	タイトル欄
	CICSDiag m_PageDiag;										//	頁切替欄
	CICSDiag m_NumberDiag;										//	様式番号欄
// midori 161108 add -->
	CICSDiag	m_SaveDataDiag;
// midori 161108 add <--
	CICSDiag m_HeadDiag;
	CICSDiag*	m_Diag;											// m_Diag1とm_Diag2を受けるポインタ
	CICSDiag	m_Diag1;										// 計が1行の明細ダイアログ
	CICSDiag	m_Diag2;										// 計が2行の明細ダイアログ
	CICSDiag*	m_DiagKei;										// m_DiagKei1とm_DiagKei2を受けるポインタ
	CICSDiag	m_DiagKei1;										// 計が1行の計専用ダイアグラム
	CICSDiag	m_DiagKei2;										// 計が2行専用のダイアグラム
//	CICSDiag m_Diag3											// 計あり様式なので、計が0行は存在しない
// 修正No.157626 add -->
	CICSDisp	m_Guide1;										// 欄外のガイド表示
// 修正No.157626 add <--
	ICSButton	m_btnMarjinal;									//	棚卸方法登録ボタン（旧：欄外登録）

//	ローカル変数
private:
	REC_UC_051_TANAOROSI	m_ReadData[ID_ROWNORMAL_051 + 1];	//	1頁分のデータ
	REC_UC_051_TANAOROSI	m_CopyData;							//	行コピー用バッファ
	int						m_nCopyFlag;						//	コピーフラグ（0：コピーデータがない／1：コピーデータがある）

//	ローカル関数
private:
	BOOL virStartJob();											//	初期処理
	void virSetInitialUchiwakeInfo();							//	共通データの初期値設定
	void virSetInitialDiag();									//	ICSDiagコントロールを通常／個別で切り替える
	int RecBufClearData( P_REC_UC_051_TANAOROSI pudRec );		//	1レコード分のデータを初期化する
	int RecBufClearAllData();									//	ローカルの1ページ分のレコードバッファを初期化する
	void virUpdateControlTitle();								//	帳表タイトル情報の表示	
	BOOL virUpdateControlTblData();								//	帳表データの表示（１ページ分）
// midori 161108 add -->
	BOOL	virUpdateControlTblData2();
// midori 161108 add <--
	void UpdateControlPage();									//	帳表ページ情報の表示
	BOOL InitialControlDataArea();								//	帳表コントロールの初期化（データ領域）
	BOOL InitialControlComboBox();								//	帳表コントロールの初期化（ComboBox）
	void InitialControlTotalArea();								//	帳表コントロールの初期化（頁計/累計領域）
	int RecBufSetData(CdbUc051Tanaoroshi* pmfcRec);				//	ローカルのレコードバッファに、1行分のデータを格納する
	void InitialControlSpecialRow(int intRow, int intFgFunc);	//	帳表コントロールの特殊行初期化
	void virInitialControlFgInputCell( int nRow, int nFgFunc, 
									BYTE bFgInput );			//	帳表コントロールの自動計算セル初期化
	void UpdateControlRowTotal( CdbUc051Tanaoroshi* mfcRec );	//	帳表データの１行表示（累計，頁計行）
	void UpdateControlRowData( CdbUc051Tanaoroshi* mfcRec );	//	帳表データの１行表示（空行，データ行，特殊行）
	CdbUc000Common* virGetDataTableObject();					//	預貯金テーブルのオブジェクトを生成し、ポインタを返す	

	//>>>>>>>>>>< OnButtonF2/OnButtonF2処理><<<<<<<<<<
	int virRecBufSaveData(int inPage, int inRow);//	指定された行のデータをテーブルに保存
	int RecBufSaveDataSub(int inPage, int inRow, 
						  P_REC_UC_051_TANAOROSI inSaveData);	//	指定された行のデータをテーブルに保存(補助ルーチン)
	
	//>>>>>>>>>>< OnButtonF6処理><<<<<<<<<<
	void virFncCellCopyProc( CdbUc000Common* rsData );			//	１つ前のデータ表示処理
	
	//>>>>>>>>>>< OnButtonF12処理><<<<<<<<<<
	void virTblEditIkkatuLine(int nType, CdbUc000Common* rsData, 
							  CString strName, CALCKEI_INFO uCalcKei, SORTKAMOKU_INFO uKamoku);
	int virTblSortSubSortProc(CdlgSort* pCdlgSort, CdbUc000Common* rsData);					//	ソート後に改頁挿入，小計行挿入処理
		
	//>>>>>>>>>>< OnRowCopy/OnRowPaste/OnRowInsert処理><<<<<<<<<<
	void virRowButtonCopyProc();								//	行コピー処理
	void virTblGetKamokuInfo(CdbUc000Common* rsData, SORTKAMOKU_INFO* uKamoku);
	void virGetCurrentInfo(CURRENT_INFO* uCurInfo);
	int virRecBufSaveDataForPasteInsert(int inPage, int inRow);	//	指定された行のデータをテーブルに保存(挿入貼付作業用)
	int	virRecBufSaveDataForPaste( int inPage, int inRow );
	
	//>>>>>>>>>>< フォーカス移動処理 ><<<<<<<<<<
	int	GetFocusPosition(int nChar);								//	フォーカス移動処理（メイン）
	int GetTotalRowIndex(int nCharOrg ,int intIndex ,int intNext);	//	フォーカス移動処理（合計欄を考慮）

	void CheckFgInput();										//	自動計算フラグ確認
	void SetAutoCalc();											//	自動計算結果を設定
	CString GetMulVal( CString szVal1, CString szVal2 );		//	掛け算した値を取得
	
	//>>>>>>>>>>< F11参照ダイアログ処理 ><<<<<<<<<<
	int virFncReferenceGetType(int intCol, int* intGrSeq, long* lngItemSeq, int* intAdd1, int* intAdd2, int* intAdd1Col, int* intAdd2Col, REFERENCE_INFO* pudReference = 0);
	void virFncReferenceSetData(CdlgReference* dlgRef, int intRefType, int intCol);
	void virFncReferenceUpdateTbl(int intRefType, int nRefMode = 0);
// 要通知案件修正 21/07/09 add -->
	void	virActivateFrameK(UINT nState, CFrameWnd* pDeactivateFrame);
	void	virKamokuAddCancel();
// 要通知案件修正 21/07/09 add <--

	void virOnUpdateButtonF6(BOOL* bFncEnable);
	void	virUpdateTitle();
	void	virInputDecisionEditOFF();

	void	SetInputMode( int nRow, long nMode );
	void	SetDiagBackColor();
// midori 151405 add -->
	int		CursorControl(short nChar, int sw);
// midori 151405 add <--

//	CString CheckOutsideValue( CICSDiag* pDiag, int nIndex );
//	BOOL	CheckOutsideValue( CICSDiag* pDiag, int nIndex, int nRow );
//	BOOL	CheckOutsideValueSub( CICSDiag* pDiag, int nIndex );

	CString GetAutoCalc( int nRow );
	BOOL CheckRowNullData();
	BOOL CheckTableMarginal();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnInitialUpdate();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	DECLARE_EVENTSINK_MAP()
public:
	void TerminationYoushikidiag1(short index, short nChar, short length, LPUNKNOWN data);
public:
	void EditONYoushikidiag1(short index);
public:
	void EditOFFYoushikidiag1(short index);
public:
	void TerminationTitlediag1(short index, short nChar, short length, LPUNKNOWN data);
public:
	void EditOFFTitlediag1(short index);
	void EditONTitlediag1(short index);
public:
	void FocusTitlediag1();
public:
	void TerminationPagediag1(short index, short nChar, short length, LPUNKNOWN data);
public:
	void EditOFFPagediag1(short index);
	void EditONPagediag1(short index);
public:
	void FocusPagediag1();
public:
	afx_msg void OnBnClickedMarginalButton1();

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
	LRESULT OnUserReDrawView(WPARAM wParam, LPARAM lParam);	// 修正No.162446 add
};

#endif	//	__FRMUC051TANAOROSI_H__