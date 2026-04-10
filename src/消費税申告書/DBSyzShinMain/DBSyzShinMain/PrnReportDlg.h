#pragma once
#include "icsppst8.h"


// CPrnReportDlg ダイアログ

class	CPrnPrint;
class	CPrnItem;
class	PrintKazeiSub;
class	PrintTokuteiSub;
class	PrintZeitempSub;
class	CPrnTune;
class	CPrnPrintKani;	// '15.02.04

class	CPrnPrint_EX;
class	CPrnPrintKani_EX;
//----->('23.05.01 Add)
class	CPrnPrint_EX2;
class	CPrnPrintKani_EX2;
//<-----

class CPrnReportDlg : public ICSDialog
{
	DECLARE_DYNAMIC(CPrnReportDlg)

public:
	CPrnReportDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CPrnReportDlg();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_PRN_REPORT };

public:
	// 初期情報セット
	int InitInfo( CSnHeadData *pSnHeadData, CDBNpSub *pZmSub, SYC_SHININFO *pShinInfoSH_PRINT_CMINFO, SH_PRINT_CMINFO *pPrintCmInfo, 
		CMainFrame *pMainFrame, CArray<char>* pPRtable, CArray<int>* pPGtable, CArray<int>* pPRtable1 );
	// タブ初期化
	int InitTab();


public:
	BOOL			m_IsPreview;	// ボタン表示「プレビュー」
	int				m_swBeForked;	//一括印刷フラグ
	//20240306 tekemura
	PrintKazeiSub		*m_pPrnKazS;	// 課税取引個別指定
	PrintTokuteiSub		*m_pPrnTokS;	// 特定収入個別指定
	PrintZeitempSub		*m_pPrnZeiS;	// 添付書面個別指定

	void SetFocusTab(int Type);
	void SetTabForcs(int TabNum);

// 24/03/22_背景色 add -->
	bool IsAllCheckOn( int pSw );
	void BackColorControl( int pSw );
// 24/03/22_背景色 add <--
// 24/04/03_checkon add -->
	void AllCheckOn( int pSw );
// 24/04/03_checkon add <--
// 24/07/10_個別タブから出力 add -->
	void CallSetEnableInputItem();
	int ZeitempEnableChk(void);
// 24/07/10_個別タブから出力 add <--

	int m_Kazei_DButton_sw;
	int m_Toku_DButton_sw;
	int m_Zeiri_DButton_sw;

private:
	CPrnPrint		*m_pPrnPrint;		// 申告書出力設定
	CPrnItem		*m_pPrnItem;		// 印刷項目設定


	CPrnTune		*m_pPrnTune;		// 印字位置微調整
	CPrnPrintKani	*m_pPrnPrintKani;	// 申告書出力設定（簡易課税）	'15.02.04
	CSyzUtil		m_Util;				// ユーティリティクラス			'15.02.04

// 24/07/10_個別タブから出力 del -->
	//CPrnPrint_EX		*m_pPrnPrint_EX;	// 申告書出力設定
	//CPrnPrintKani_EX	*m_pPrnPrintKani_EX;// 申告書出力設定（簡易課税）	'15.02.04
	////----->('23.05.01 Add)
	//CPrnPrint_EX2		*m_pPrnPrint_EX2;		// 申告書出力設定
	//CPrnPrintKani_EX2	*m_pPrnPrintKani_EX2;	// 申告書出力設定（簡易課税）
	////<-----
// 24/07/10_個別タブから出力 del <--
	CSnHeadData		*m_pSnHeadData;		// 消費税ヘッダ情報
	CDBNpSub		*m_pZmSub;			// 公益クラス
	SYC_SHININFO	*m_pShinInfo;		// 共通情報

	SH_PRINT_CMINFO	*m_pPrintCmInfo;	// 印刷共通情報

// 240308_h_160357 add -->
	int				m_Tokutei;
// 240308_h_160357 add <--

	// 部数関連
// 24/03/26_税務代理書面 del -->
	//CMainFrame*		m_pMainFrame;		// 親ウインドウ
// 24/03/26_税務代理書面 del <--
	CArray<char>*	m_pPRtableEx;
	CArray<int>*	m_pPGtable;
	CArray<int>*	m_pPRtableEx1;

// 24/03/26_税務代理書面 del -->
public:
	CMainFrame*		m_pMainFrame;		// 親ウインドウ
// 24/03/26_税務代理書面 del <--
// 24/07/10_個別タブから出力 add -->
	CPrnPrint_EX*		m_pPrnPrint_EX;			// 申告書出力設定
	CPrnPrintKani_EX*	m_pPrnPrintKani_EX;		// 申告書出力設定（簡易課税）
	CPrnPrint_EX2*		m_pPrnPrint_EX2;		// 申告書出力設定
	CPrnPrintKani_EX2*	m_pPrnPrintKani_EX2;	// 申告書出力設定（簡易課税）
// 24/07/10_個別タブから出力 add <--

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	ICSTabCtrl m_PrnTab;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void OnTcnSelchangeCustomPrint(NMHDR * pNotifyStruct, LRESULT * result);
};
