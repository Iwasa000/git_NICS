#pragma once
#include "ShinSpecific52_1_31.h"
#include "ShinSpecific52_2_31.h"
#include "ShinSpecific52_31_31.h"
#include "ShinSpecific52_32_31.h"
#include "ShinSpecific52_33_31.h"
#include "ShinSpecific52_41_31.h"
#include "ShinSpecific52_42_31.h"
#include "ShinSpecific52_43_31.h"

// CSpeAddDlg ダイアログ

class CSpeAddDlg : public ICSDialog
{
	DECLARE_DYNAMIC(CSpeAddDlg)

public:
	CSpeAddDlg(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CSpeAddDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SPEADD };
#endif
public:
	void InitInfo(CSnHeadData *pSnHeadData, CH31SpcListData *pSpcListData, CArithEx *pArith, CDBNpSub *pZmSub);
	void ReInitSpcDisp(int selno, int chktype);
	void AddTab(int type);
	void DelTab(int type);
	int chktype;
	ICSTabCtrl m_SpeTab;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

	
	void InitTab();
	CShinSpecific52_1_31* m_ShinSpecific52_1_31;
	CShinSpecific52_2_31* m_ShinSpecific52_2_31;
	CShinSpecific52_31_31* m_ShinSpecific52_31_31;
	CShinSpecific52_32_31* m_ShinSpecific52_32_31;
	CShinSpecific52_33_31* m_ShinSpecific52_33_31;
	CShinSpecific52_41_31* m_ShinSpecific52_41_31;
	CShinSpecific52_42_31* m_ShinSpecific52_42_31;
	CShinSpecific52_43_31* m_ShinSpecific52_43_31;

	CSnHeadData			*m_pSnHeadData;	// 消費税ヘッダ情報
	CH31SpcListData		*m_pSpcListData;	// 特定収入計算表データ
	CArithEx			*m_pArith;			// 倍長演算
	CDBNpSub			*m_pZmSub;			// 公益クラス

// 24/02/14_特定収入 add -->
	CTblhdl				m_TblHdl;			// テーブルハンドル
	struct TB_PAR		m_TsTbl52;			// 特定収入　計算表５－２
	int					m_Type;				// 計算表選択　0:5-2(2) 1:5-2(3) 2:5-2(4)
	int					m_CurPage;			// 現在の頁
public:
	TSREC52				m_TSrec52;
// 24/02/14_特定収入 add <--
// 240308_h_160358 add -->
	int					m_Eymd;
// 240308_h_160358 add <--

public:
	virtual BOOL OnInitDialog();
// 24/02/15_特定収入 del -->
	//afx_msg void OnBnClickedCheck2();
	//afx_msg void OnBnClickedCheck3();
	//afx_msg void OnBnClickedCheck4();
// 24/02/15_特定収入 del <--
	CButton m_Check2;
	CButton m_Check3;
	CButton m_Check4;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void OnTcnSelchangeMainTab(NMHDR * pNotifyStruct, LRESULT * result);
	LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CStatic m_Static2;
// 24/02/14_特定収入 add -->
	LRESULT OnMyMessageSelChange( WPARAM wParam, LPARAM lParam );
	afx_msg void OnBnClickedOk();
	CIcsdiagctrl m_PageDiag;

	void	CmnDiagSetValue( CIcsdiagctrl* pDiag, int intIndex, int intData, int intFlg );
	void	CmnDiagSetString( CIcsdiagctrl* pDiag, int intIndex, CString strData, int intFlg );
	int		CmnDiagGetValue( CIcsdiagctrl* pDiag, int intIndex );
	CString	DiagGetString( CIcsdiagctrl* pDiag, int intIndex );
	void	DiagInit( DIAGRAM_DATA* diadata );
	void	PageButtonCtrl( void );
	void	DialogSetFocus( void );

	int		Disp_WriteLock();

	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton6();
	DECLARE_EVENTSINK_MAP()
	void TerminationIcsdiag8ctrl1(short index,short nChar,short length,LPUNKNOWN data);
// 24/02/14_特定収入 add <--
	void EditOFFIcsdiag8ctrl1(short index);
	CFont			m_Font;
	ICSStatic m_InputLock;
// 240305_160343 add -->
	afx_msg void OnBnClickedButton7();
// 240305_160343 add <--
// 240305_160345 add -->
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();	// 修正No.168722 add

public:
	void	ListChgButtonCtrl();
// 240305_160345 add <--
	afx_msg void OnBnClickedCancel();

	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);		// 修正No.168706 add
};
