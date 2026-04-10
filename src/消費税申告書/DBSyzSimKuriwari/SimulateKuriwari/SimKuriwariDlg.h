#pragma once

#include "CalendarEx.h"
#include "MngTables.h"
#include "SimKSyukei.h"
#include "SettingHolidaysDlg.h"
#include "SyukeiTenkiDlg.h"

// CSimKuriwariDlg ダイアログ
class CSimKuriwariDlg : public ICSDialog
{
	DECLARE_DYNAMIC(CSimKuriwariDlg)

public:
    CDBZmSub* m_pZmSub;

private:
	CArithEx m_ArithEx;
    CSimKSyukei m_Syukei;
    CMngTables m_MngTables;
	CCalendarEx m_CalendarEx;
    CSettingHolidaysDlg* m_pSettingHolidaysDlg;
    CSyukeiTenkiDlg* m_pSyukeiTenkiDlg;
	CSyzUtil m_SyzUtil;
	EnumIdIcsShKazeihoushiki m_kazeihousiki;
    char rsv[256];

	CButton m_ButtonSetting;
	CButton m_ButtonSyukei;
	CButton m_ButtonOK;
	CButton m_ButtonCancel;
	CStatic m_StaticGuide2;

public:
	CSimKuriwariDlg(CDBZmSub* pZmsub, CWnd* pParent = nullptr);   // 標準コンストラクター
    void EndSettingHolidaysDlg(int nResult);
    void EndSyukeiTenkiDlg(int nResult);
	virtual ~CSimKuriwariDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SIMULATEKURIWARI_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

private:
	void InitCalendar();
	void InitWindow();
	void InitWindow2();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSyukeiTenki();
	afx_msg void OnBnClickedButtonSetting();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedOk();
};
