//--------------------------------------------------
//	dlgKaikakekin.h
//
//	2006.03.23	～
//--------------------------------------------------
#ifndef	__DLGKAIKAKEKIN_H__
#define	__DLGKAIKAKEKIN_H__

#pragma once
#include "icsdiag.h"
#include "icsbutton.h"
#include "dbUc092Kaikakekin2.h"
#include "dateConvert.h"
#include "afxwin.h"

#define	DK_INPUTINDEX_FIRST			14					//	ICSDiagの最初の入力項目のインデックス
#define	DK_INPUTINDEX_LAST			21					//	ICSDiagの最後の入力項目のインデックス
#define DK_ROWKOBETU_092			4					//	⑨買掛金(欄外)の最大行数
#define DK_COLMAXDATA_092			2				// １行内の入力コントロール数

//	買掛金(欄外)の内訳書列番号
typedef enum {
	DK_COL_092_SPDATE = 1,	//	 1	支払確定年月日
	DK_COL_092_KIMATU,		//	 2	期末現在高
} EnumIdFormCol092;

// CdlgKaikakekin ダイアログ

typedef struct _RANGAI_KAIKAKE_DATA{
	_RANGAI_KAIKAKE_DATA()
	{
		clear();
		isPrint = 1;
	}

	_RANGAI_KAIKAKE_DATA(int row)
	{
		clear();
		isPrint = 1;
		Row = row;
	}

	_RANGAI_KAIKAKE_DATA(const CdbUc092Kaikakekin2& mfc)
	{
		set(mfc);
	}

	//_RANGAI_KAIKAKE_DATA(const _RANGAI_KAIKAKE_DATA& obj){}

	_RANGAI_KAIKAKE_DATA(BYTE row, long date, CString val)
		:Row(row), SpData(date), Val(val){}

	void clear()
	{
		isPrint = FALSE;
		Row = 0;
		SpData = 0;
		Val.Empty();
	}

	void set(const CdbUc092Kaikakekin2& db)
	{
		if(db.m_FgShow)	isPrint = FALSE;
		else			isPrint = TRUE;
		Row = db.m_NumRow;
		SpData = db.m_SpDate;
		Val = db.m_Val;
	}

	BOOL isPrint;
	BYTE Row;
	long SpData;
	CString  Val;
}RANGAI_KAIKAKE_DATA;

class RangaiKaikake{
public:
	RangaiKaikake()
	{
		for(int i=0; i<DK_ROWKOBETU_092; i++){
			RANGAI_KAIKAKE_DATA rkd(i + 1);
			vec_rkd.push_back(rkd);
		}

		isPrint = 1;
	}

	RangaiKaikake(CdbUc092Kaikakekin2& db)
	{
		db.MoveFirst();
		while(!db.IsEOF()){
			RANGAI_KAIKAKE_DATA rkd(db);
			vec_rkd.push_back(rkd);

			db.MoveNext(); 
		}

		if(db.m_FgShow) isPrint = 0;
		else			isPrint = 1;
	}

	BOOL isPrint;
	vector<RANGAI_KAIKAKE_DATA> vec_rkd;
};

class CdlgKaikakekin : public ICSDialog
{
	DECLARE_DYNAMIC(CdlgKaikakekin)

public:
	CdlgKaikakekin(CWnd* pParent = NULL);				//	標準コンストラクタ
	virtual ~CdlgKaikakekin();							//	デストラクタ

// ダイアログ データ
	enum { IDD = IDD_DIALOG_KAIKAKEKIN };
	CICSDiag m_diagInput;
	ICSButton m_btnOK;
	ICSButton m_btnCancel;
	ICSStatic m_lblNote;
	CButton m_chkPrint;
	CComboBox m_cmbPrint;
	ICSButton m_btnMovePrev;
	ICSButton m_btnMoveNext;
	CStatic m_txtCurrent;
	CStatic m_txtMax;
	CButton m_rdoCommon;
	CButton m_rdoIndiv;

private:
	CDatabase*		m_pDB;							//	データベースハンドル
	int				m_nRet;							//	戻値
	int				m_bSpDate[DK_ROWKOBETU_092];	// 前回入力した日付
	COLORREF		m_ViewColor;					//	ダイアログの背景色
	CToolBar		m_ToolBar;						//	ツールバー
	vector<RangaiKaikake> m_vecRangai;				// 実データ
	int				m_CurrentPage;					// 現在画面に表示しているページ
	size_t			m_MaxPage;						// 最大ページ数
	int				m_MaxPageDB;					// 読み込み時のDBに登録されている最大ページ数
	map<int, int>	m_mapIndex;						// ダイアグラムのインデックスから行番号を紐づけるマップ
	CUcFunctionCommon	m_clsFunc;
	int				m_PageMaxFrom;
// midori 160612 add -->
	BOOL			m_DataKakutei;					// 入力データを確定する
// midori 160612 add <--

// 修正No.157856 add -->
	int				m_bReDate[DK_ROWKOBETU_092];	// 日付チェック用
// 修正No.157856 add <--

// 修正No.157633_元号日付 add -->
	int				m_OldDate[5];					// 日付データ入力更新チェック用の値
	int				m_OldGengo[5];					// 日付(元号番号)データ入力更新チェック用の値
	int				m_GenListSw;					// 
	int				m_EditSign;						// EditON⇔EditOFFがペアで通ってるか確認するためのサイン
	int				m_CurCol;						// 現在編集中のカラム
	int				m_OldCol;						// 前回編集していたカラム
// 修正No.157633_元号日付 add <--

private:
	int SetData();													//	データ設定
	void SetDate( CICSDiag* pDiag, int nIndex, int nDate );			//	日付設定
	void SetMoney( CICSDiag* pDiag, int nIndex, CString szMoney );	//	金額設定
	int GetDate( CICSDiag* pDiag, int nIndex, int nRow );			//	日付取得
// 修正No.157633_元号日付 add -->
	int DiagGetDate( CICSDiag* pDiag, int nIndex, int intFlg );		//	日付取得
// 修正No.157633_元号日付 add <--
	CString GetMoney( CICSDiag* pDiag, int nIndex );				//	金額取得
	int	SetEnterReDraw( CICSDiag* pDiag, int nIndex );				//	入力項目を強制的に確定
	void InitDiag( DIAGRAM_DATA *diadata );							//	DIAGRAM_DATA構造体初期化関数
	void InitAttr( DIAGRAM_ATTRIBUTE *diaatt );						//	DIAGRAM_ATTRIBUTE構造体初期化関数
	void ChangeFieldValData(char* chrVal, CString strVal);			//	金額値→倍長フィールド変換関数
	int GetFocusControlID( CWnd* pParent = NULL );					//	フォーカスのあるコントロールのID取得
	void GetNotCommaValData(CString* strVal, char* chrVal);			//	カンマ省略金額値取得関数
	int	 GetControlCol( int nIndex );								//	列番号取得
	void CheckPrintStatus();										//	印字するチェックボックス確認
	void SetAllControl( BOOL fFlag );								//	全てのコントロール設定
	int	SetAllBackColor( COLORREF nColor);							//　全てのダイアグラムの入力部分の背景を変更
	void SetPrintTypeControlls(BOOL fFlag);							//　出力形式（共通or個別）コントロール設定
	void SetPageControlls(BOOL fFlag);								//　ページ関連のコントロール設定
	int SetToolBar();
	void Disp(int page);
	void DispPage(int page);
	void AddVec(int page, CdbUc092Kaikakekin2& mfcRec);
	void Read();
	int Write();
	int UpdateRecord(CdbUc092Kaikakekin2& db, vector<RangaiKaikake>::iterator it, int page);
	void UpdateVector(int index);
	void UpdateVectorAll();
	void SetToolBarState();
	void SetMoveButtonState();
	void SetMovePrevButtonState();
	void SetMoveNextButtonState();
	void MovePage(int page);
	void OnBnClickedRadioCommonSub();
	void OnCbnSelchangePrintComboSub(BOOL mode);
	void AddPage();
	void SyncPage();

	void OnToolButtonF7();
	void OnToolButtonF9();

public:
// midori 160612 cor -->
//	int	ShowDialog( CDatabase* pDB, const UCHIWAKE_INFO& page_info );								//	ダイアログ表示
// ---------------------
	int	ShowDialog( CDatabase* pDB, const UCHIWAKE_INFO& page_info, BOOL pDataKakutei );			//	ダイアログ表示
// midori 160612 cor <--

// 修正No.157633_元号日付 add -->
	void Date_ChkAndSet(CICSDiag* , short, int, short);			// 元号+年月日の入力状況を取得
	LRESULT OnUserFocusset2(WPARAM wParam, LPARAM lParam);
// 修正No.157633_元号日付 add <--
// 修正No.157856 add -->
	int DataCheck(int pPos);									// 日付が変更されたかをチェック
// 修正No.157856 add <--

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	DECLARE_MESSAGE_MAP()	
public:
	virtual BOOL OnInitDialog();
public:
	DECLARE_EVENTSINK_MAP()
public:
	void EditONKaikakekinDiag(short index);
	void EditOFFKaikakekinDiag(short index);
	void TerminationKaikakekinDiag(short index, short nChar, short length, LPUNKNOWN data);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangePrintCombo();
	afx_msg void OnBnClickedRadioCommon();
	afx_msg void OnBnClickedRadioIndiv();
	afx_msg void OnBnClickedPrintCheck();
	afx_msg void OnBnClickedButtonPrev();
	afx_msg void OnBnClickedButtonNext();
protected:
	virtual void OnOK();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

// 修正No.157633_元号日付 add -->
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
// 修正No.157633_元号日付 add <--
};

#endif	//	__DLGKAIKAKEKIN_H__