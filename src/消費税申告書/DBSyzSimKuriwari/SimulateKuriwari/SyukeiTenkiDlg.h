#pragma once

#include "SimKSyukei.h"

// CSyukeiTenkiDlg ダイアログ

class CSyukeiTenkiDlg : public ICSDialog
{
	DECLARE_DYNAMIC(CSyukeiTenkiDlg)

private:
	ICSReportControl m_Report;
	CMngTables* m_pMngTables;
	CSimKSyukei* m_pSyukei;
	CArithEx m_ArithEx;
	EnumIdIcsShKazeihoushiki m_kazeihousiki;
	SIMKSIREWARIZREC* m_pSirewariRec;
	SIMKURIWARIZREC* m_pUriwariRec;

	CStatic m_StaticText;
	CButton m_check_nashi;
	CButton m_check_oroshi;
	CButton m_check_kouri;
	CButton m_check_seizo;
	CButton m_check_sonota;
	CButton m_check_service;
	CButton m_check_hudosan;
    char m_check_tenkikbn[JIGYOKUBUN_MAX]; //転記ON/転記OFF
    bool m_check_st[JIGYOKUBUN_MAX];       //有効/無効

public:
	CSyukeiTenkiDlg(CWnd* pParent = nullptr);   // 標準コンストラクター
	CSyukeiTenkiDlg(CWnd* pParent,
		CMngTables* pMngTables,
		CSimKSyukei* pSyukei,
		EnumIdIcsShKazeihoushiki kazeihousiki = ID_ICSSH_GENSOKU);
	//CSyukeiTenkiDlg(CSimKSyukei* pSyukei, CMngTables* pMngTables, EnumIdIcsShKazeihoushiki kazeihousiki = ID_ICSSH_GENSOKU, CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CSyukeiTenkiDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SYUKEI_DLG };
#endif

    void InitCheck();
    void InitReport();
	void AddSyukeiDataReport();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckKbnnashi();
	afx_msg void OnBnClickedCheckOroshi();
	afx_msg void OnBnClickedCheckKouri();
	afx_msg void OnBnClickedCheckSeizo();
	afx_msg void OnBnClickedCheckSonota();
	afx_msg void OnBnClickedCheckService();
	afx_msg void OnBnClickedCheckHudosan();
};
