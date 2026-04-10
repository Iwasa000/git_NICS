//--------------------------------------------------
//	dlgTanaoroshi.h
//
//	2006.03.01～
//--------------------------------------------------

#ifndef	__DLGTANAOROSHI_H__
#define	__DLGTANAOROSHI_H__

#pragma once
#include "afxwin.h"
#include "icsinput.h"
#include "icsknjedit.h"
#include "icsbutton.h"
#include "icsocx.h"

#include "UcCommon.h"				//	定数ヘッダ
#include "dbUc052Tanaoroshi2.h"		//	uc_052_tanaoroshi2テーブルクラス
#include "dateConvert.h"			//	日付変換クラス

// CdlgTanaoroshi ダイアログ

typedef struct _RangaiTana
{
	_RangaiTana()
	{
		clear();
		isPrint = TRUE;
	}

	_RangaiTana(const CdbUc052Tanaoroshi2& db)
	{
		set(db);
	}

	_RangaiTana(BOOL isprint, BYTE method, CString name, long date)
		:isPrint(isprint), Method(method), HdName(name), SpDate(date){}

	void clear()
	{
		isPrint = FALSE;
		Method = 0;
		HdName.Empty();
		SpDate = 0;
	}

	void set(const CdbUc052Tanaoroshi2& db)
	{
		if(db.m_FgShow)	isPrint = FALSE;
		else			isPrint = TRUE;
		Method = db.m_Method;
		HdName = db.m_HdName;
		SpDate = db.m_SpDate;
	}

	BOOL isPrint;
	BYTE Method;
	CString HdName;
	long SpDate;
}RangaiTana;

class CdlgTanaoroshi : public ICSDialog
{
	DECLARE_DYNAMIC(CdlgTanaoroshi)

public:
	CdlgTanaoroshi(CWnd* pParent = NULL);   //	標準コンストラクタ
	virtual ~CdlgTanaoroshi();				//	デストラクタ

// ダイアログ データ
	enum { IDD = IDD_DIALOG_TANAOROSHI };
	CButton m_rdoA;
	CButton m_rdoB;
	CButton m_rdoC;
	CButton m_rdoD;
	CButton m_chkPrint;
	CButton m_rdoCommon;
	CButton m_rdoIndiv;
	CICSInput m_txtDate;
	CICSKNJEdit m_txtName;
	ICSButton m_btnOK;
	ICSButton m_btnCancel;
	ICSStatic m_lblNote;
	ICSStatic m_lblDate;
	ICSButton m_btnMovePrev;
	ICSButton m_btnMoveNext;
	CComboBox m_cmbPrint;
	CStatic m_txtCurrent;
	CStatic m_txtMax;
	

	CToolBar		m_ToolBar;					//	ツールバー

private:
	CDatabase*		m_pDB;						//	データベースハンドル
	int				m_nRet;
	CUcFunctionCommon m_clsFunc;				//	共通関数クラス
	int				m_bSpDate;					//	前回入力した日付
	vector<RangaiTana> m_vecRangai;				// 実データ
	int				m_CurrentPage;				// 現在画面に表示しているページ
	size_t			m_MaxPage;					// 最大ページ数
	int				m_MaxPageDB;				// 読み込み時のDBに登録されている最大ページ数
	map<int, int>	m_mapIndex;					// ダイアグラムのインデックスから行番号を紐づけるマップ
	int				m_PageMaxFrom;
// midori 160612 add -->
	BOOL			m_DataKakutei;				// 入力データを確定する
// midori 160612 add <--

private:
	void SetRadioButton();						//	ラジオボタン設定
	void SetRadioButton( long nID );			//	ラジオボタン設定
	void CheckPrintStatus();					//	印字するチェックボックス確認
	void SetAllControl( BOOL fFlag );			//	全てのコントロール設定
	void SetICSInputDate( int nDate );			//	ICSInputコントロールに日付値を設定
	int GetICSInputDate();						//	ICSInputコントロールから日付値を取得
	void InitInputData( INPUTDATA* pudData );	//	INPUTDATA構造体初期化
	int Read();
	int Write();
	void AddVec(int page, CdbUc052Tanaoroshi2& mfcRec);
	void Disp(int page);
	void DispPage(int page);
	void SetPrintTypeControlls(BOOL fFlag);
	void SetPageControlls(BOOL fFlag);								//　ページ関連のコントロール設定
	int SetToolBar();
	void SetMoveButtonState();
	void SetMovePrevButtonState();
	void SetMoveNextButtonState();
	void SetToolBarState();
	void SetMethod(BYTE method);
	BYTE GetMethod();
	void UpdateVector();
	int UpdateRecord(CdbUc052Tanaoroshi2& db, vector<RangaiTana>::iterator& it, int page);
	void MovePage(int page);
	void SetControl(const CdbUc052Tanaoroshi2& mfcRec);
	void OnCbnSelchangePrintComboSub(BOOL mode);
	void OnBnClickedRadioCommonSub();
	void AddPage();
	void SyncPage();

	void OnToolButtonF7();
	void OnToolButtonF9();
	LRESULT SetCursorBytePosHandler(WPARAM w, LPARAM l);

public:
// midori 160612 cor -->
//	int ShowDialog( CDatabase* pDB, const UCHIWAKE_INFO& page_info  );			//	ダイアログ表示
// ---------------------
	int ShowDialog( CDatabase* pDB, const UCHIWAKE_INFO& page_info, BOOL pDataKakutei );			//	ダイアログ表示
// midori 160612 cor <--

//	イベント系
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedRadioD();
	afx_msg void OnBnClickedRadioC();
	afx_msg void OnBnClickedRadioB();
	afx_msg void OnBnClickedRadioA();	
	afx_msg void OnBnClickedPrintCheck();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonPrev();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnBnClickedRadioCommon();
	afx_msg void OnBnClickedRadioIndiv();
	afx_msg void OnCbnSelchangePrintCombo();
public:
	DECLARE_EVENTSINK_MAP()
public:
	void TerminationDateText(short nChar);
public:
	void CheckDataDateText(LPUNKNOWN data);
protected:
	virtual void OnOK();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#endif	//	__DLGTANAOROSHI_H__