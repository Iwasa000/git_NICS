#pragma once

#include "MngTables.h"

#define CALENDAR_DAY_WIDTH     0x20
#define CALENDAR_WIDTH_MARGIN  0x08
#define CALENDAR_HEIGHT_MARGIN 0x05
#define CALENDAR_DAY_HEIGHT    0x15
#define CALENDAR_UPPER_DISPMAX 0x06
#define WEEKDAY                0x07
#define DAYSOFFSET_SUNDAY      0x00
#define DAYSOFFSET_SATURDAY    0x06
#define MONTH_NUM              0x0c
#define MONTH_LINES            0x08
#define RESOLUTIONDEFAULT      0x780 //1920

#define CALENDAR14_UPPER_DISPMAX 0x07
// CCalendarEx

#define CCALENDAREX_CLASSNAME    _T("CCalendarExCtrl")  // Window class name

typedef struct _DAYSPT{
    int ymd;
    int wday;
    CRect r;
} DAYSPT;

class CCalendarEx : public CWnd
{
	DECLARE_DYNAMIC(CCalendarEx)

public:
	CDBZmSub* m_pZmSub;
    CMngTables* m_pMngTables;
	int m_s_year;
	int m_s_month;
    long m_ss_ymd;
    long m_ee_ymd;
    bool m_DispMonth14;
    double m_ratio;

private:
    std::vector<DAYSPT> m_dayspt_list;

public:
	CCalendarEx();
	BOOL RegisterWindowClass();
	void DrawMonthBase(int dofs);
	void DrawMonthBase14(int dofs);
	void DrawMonthDays(int dofs);
	void DrawMonthDays14(int dofs);
	int GetOffsetDay(int year, int month, int day);
	int GetLastDay(int year, int month, int day);
	bool IsPtInDate(POINT pt, DAYSPT* pDayspt);
	virtual ~CCalendarEx();

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


