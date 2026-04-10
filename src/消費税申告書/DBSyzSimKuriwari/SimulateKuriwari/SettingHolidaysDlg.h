#pragma once

// CSettingHolidaysDlg ダイアログ

class CSettingHolidaysDlg : public ICSDialog
{
	DECLARE_DYNAMIC(CSettingHolidaysDlg)

public:
	CDBZmSub* m_pZmSub;
    CMngTables* m_pMngTables;
	CVolDateDB m_Voldate;

	CButton m_check_doff1;
	CButton m_check_doff2;
	CButton m_check_doff3;
	CButton m_check_doff4;
	CButton m_check_doff5;
	CButton m_check_doff6;
	CButton m_check_doff0;
    char m_check_wday[WEEKDAY] = {0};

	ICSButton m_ButtonSA;
	ICSButton m_ButtonSD;
	ICSReportControl m_ReportS;

	ICSButton m_ButtonKA;
	ICSButton m_ButtonKD;
	ICSReportControl m_ReportK;

public:
	CSettingHolidaysDlg(CWnd* pParent = nullptr);   // 標準コンストラクター
	CSettingHolidaysDlg(CDBZmSub* pZmSub, CMngTables* pMngTables, CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CSettingHolidaysDlg();

	void InitCheckWday();
	void InitReport(ICSReportControl* p);
    void SetReport(ICSReportControl* p, std::vector<HOLIDAYREC>* pVec);
	void AddRow(ICSReportControl* p);
	void DeleteRow(ICSReportControl* p);
	void ClearReport(ICSReportControl* p);
	void EditRightItemReport(ICSReportControl* p, int idxRow, int idxColumn);
	void EditLeftItemReport(ICSReportControl* p, int idxRow, int idxColumn);

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTING_HOLIDAYS_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedCheckDayoff1();
	afx_msg void OnBnClickedCheckDayoff2();
	afx_msg void OnBnClickedCheckDayoff3();
	afx_msg void OnBnClickedCheckDayoff4();
	afx_msg void OnBnClickedCheckDayoff5();
	afx_msg void OnBnClickedCheckDayoff6();
	afx_msg void OnBnClickedCheckDayoff0();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSAdd();
	afx_msg void OnBnClickedButtonSDelete();
	afx_msg void OnBnClickedButtonKAdd();
	afx_msg void OnBnClickedButtonKDelete();
	afx_msg void OnBnClickedButtonInit();
	void OnNMReportValueChangedReportS(NMHDR * pNotifyStruct, LRESULT * result);
	void OnNMReportEditkeydownReportS(NMHDR * pNotifyStruct, LRESULT * result);
	void OnNMReportValueChangedReportK(NMHDR * pNotifyStruct, LRESULT * result);
	void OnNMReportEditkeydownReportK(NMHDR * pNotifyStruct, LRESULT * result);

};
