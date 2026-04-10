#pragma once
#include "icsdiagctrl.h"
#include "afxwin.h"


// CPrnPrint_EX2 ダイアログ

class CPrnPrint_EX2 : public ICSDialog
{
	DECLARE_DYNAMIC(CPrnPrint_EX2)

public:
	CPrnPrint_EX2(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CPrnPrint_EX2();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_PRN_PRINT_EX2 };

	// 初期情報セット
	int InitInfo( CSnHeadData *pSnHeadData, CDBNpSub *pZmSub, SYC_SHININFO *pShinInfo, 
		SH_PRINT_CMINFO *pPrintCmInfo, CArray<char>* pPRtable, CArray<int>* pPGtable, CArray<int>* pPRtable1 );
	// 初期化
	int Init();

	// 入力項目設定
	void SetInputItem( void );
	// 入力項目設定（サブ）
	int SetInputItemData( short idx );
	// 入力項目取得
	int GetInputItemData( short idx );
	// 入力項目色変更
	void ChangeColor( unsigned short id, short index, int sign );

	// 印刷用情報のセット
	//void SetPrintInfo();			// 24/07/10_個別タブから出力 del
	void SetPrintInfo(int seltb);	// 24/07/10_個別タブから出力 add
	// 全選択＆全解除
	BOOL ChoiceItem( int pSw );
	int m_swBeForked;
private:
	int					m_SelSw;			// 全選択／解除サイン
	BOOL				m_isNoneMTForm;		// 付表1-3 / 2-3 新様式で出力

// 24/03/22_背景色 add -->
	CWnd*				m_pParent;
// 24/03/22_背景色 add <--
	CSnHeadData			*m_pSnHeadData;		// 消費税ヘッダ情報
	CDBNpSub			*m_pZmSub;			// 公益クラス
	SYC_SHININFO		*m_pShinInfo;		// 共通情報
	CSyzUtil			m_Util;				// ユーティリティクラス
	SH_PRINT_CMINFO		*m_pPrintCmInfo;	// 印刷共通情報
	EnumIdPrintSelState	m_PrselState;		// 出力指定の状態('15.02.24)
	
	// 部数関連
	CArray<char>*		m_pPRtableEx;
	CArray<int>*		m_pPGtable;
	CArray<int>*		m_pPRtableEx1;

// 24/03/18_税務代理書面 add -->
	long				m_Eymd;
// 24/03/18_税務代理書面 add <--

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CIcsdiagctrl m_Diag1;
	CIcsdiagctrl m_Diag2;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();

	void Button3Control(void);			// 24/02/27wd_特定収入対応 add
	void Button4Control(void);			// 課税取引計算表対応 add
	void Button5Control(void);

	DECLARE_EVENTSINK_MAP()
	void EditOFFIcsdiag8ctrl2(short index);
	void EditONIcsdiag8ctrl2(short index);
	void TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data);
	void CheckButtonIcsdiag8ctrl2(short index, LPUNKNOWN data);
	void ComboSelIcsdiag8ctrl2(short index, LPUNKNOWN data);
	void ComboSelIcsdiag8ctrl2Ex( short pIndex, DIAGRAM_DATA* pData );

protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedCheckAllout();
	CButton m_AlloutCheck;

	// 部数関連
	CButton	m_Radio1;
	CButton	m_Radio2;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	BOOL IsBusu( int pIdx );
	int GetBusuNo( int pIdx );
	BOOL IsCheckOn( int pIdx );
	int GetMaxBusu( void );
	void BusuCheckAndPut( unsigned char pPno, int pSno, int pPage=1 );
	void BusuCheckAndPut3( unsigned char pPno, int pSno );
// 24/03/18_税務代理書面 add -->
	void BusuCheckAndPut4( unsigned char pPno, int pSno );
// 24/03/18_税務代理書面 add <--
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton3();

// 24/03/22_背景色 add -->
// 24/03/25_168691 cor -->
	//void	BackColorControl( short pInd, DIAGRAM_DATA* pData );
// -----------------------
	void	BackColorControl( short pInd, DIAGRAM_DATA* pData=NULL );
// 24/03/25_168691 cor <--
	void	BackColorControl2( int pSw );
// 24/03/22_背景色 add <--
};
