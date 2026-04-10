// CalendarEx.cpp : 実装ファイル
//

#include "stdafx.h"
#include "SimulateKuriwari.h"
#include "CalendarEx.h"

// CCalendarEx

IMPLEMENT_DYNAMIC(CCalendarEx, CWnd)

CCalendarEx::CCalendarEx()
{
	RegisterWindowClass();
	m_dayspt_list.clear();
}

BOOL CCalendarEx::RegisterWindowClass()
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();

	if (::GetClassInfo(hInst, CCALENDAREX_CLASSNAME, &wndcls)) {
		return TRUE;
    }

	wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndcls.lpfnWndProc = ::DefWindowProc;
	wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
	wndcls.hInstance = hInst;
	wndcls.hIcon = NULL;
	wndcls.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	wndcls.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
	wndcls.lpszMenuName = NULL;
	wndcls.lpszClassName = CCALENDAREX_CLASSNAME;

	if (AfxRegisterClass(&wndcls)) {
		return TRUE;
    }

	AfxThrowResourceException();
	return FALSE;
}

void CCalendarEx::DrawMonthBase(int dofs)
{
	HDC hdc = ::GetDC(this->m_hWnd);
    CString yobistr[WEEKDAY] = { _T("日"), _T("月"), _T("火"), _T("水"), _T("木"), _T("金"), _T("土") };

	HBRUSH hBrush, hOldBrush;
	int iOldMode;
	hBrush = CreateSolidBrush(GRAY_COLOR);
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);   
    iOldMode = SetBkMode(hdc,TRANSPARENT);

	DRAWTEXTPARAMS dtp;
	dtp.cbSize = sizeof(DRAWTEXTPARAMS);
	dtp.iLeftMargin = 0;
	dtp.iRightMargin = 0;
	dtp.iTabLength = 0;

	//年月
    CString ymstr;
    int month = m_s_month + dofs;
    int year = m_s_year;
    if( month > MONTH_NUM) {
        month -= MONTH_NUM;
        year++;
	}
	ymstr.Format(_T("%04d年%02d月"), year, month);

	CRect r;
    r.left = (long)(CALENDAR_WIDTH_MARGIN + CALENDAR_DAY_WIDTH * m_ratio * WEEKDAY * dofs + CALENDAR_WIDTH_MARGIN * dofs);
	if (dofs >= CALENDAR_UPPER_DISPMAX) {
		r.left = (long)(CALENDAR_WIDTH_MARGIN + CALENDAR_DAY_WIDTH * m_ratio * WEEKDAY * (dofs - CALENDAR_UPPER_DISPMAX) + CALENDAR_WIDTH_MARGIN * (dofs - CALENDAR_UPPER_DISPMAX));
	}
	r.top = 0;
	if (dofs >= CALENDAR_UPPER_DISPMAX) {
		r.top += long(CALENDAR_DAY_HEIGHT * MONTH_LINES);
	}
	r.right = long(r.left + CALENDAR_DAY_WIDTH * m_ratio * WEEKDAY);
	r.bottom = r.top + CALENDAR_DAY_HEIGHT;

	SetTextColor(hdc, BLACK_COLOR);
	DrawTextEx(hdc, ymstr.GetBuffer(), -1, &r, DT_SINGLELINE | DT_VCENTER | DT_CENTER, &dtp);

	//曜日
	CRect r2;
	r2.left = long(CALENDAR_WIDTH_MARGIN + CALENDAR_DAY_WIDTH * m_ratio * WEEKDAY * dofs + CALENDAR_WIDTH_MARGIN * dofs);
	if (dofs >= CALENDAR_UPPER_DISPMAX) {
		r2.left = long(CALENDAR_WIDTH_MARGIN + CALENDAR_DAY_WIDTH * m_ratio * WEEKDAY * (dofs - CALENDAR_UPPER_DISPMAX) + CALENDAR_WIDTH_MARGIN * (dofs - CALENDAR_UPPER_DISPMAX));
	}
    r2.top = CALENDAR_DAY_HEIGHT;
    if( dofs >= CALENDAR_UPPER_DISPMAX) {
        r2.top += CALENDAR_DAY_HEIGHT * MONTH_LINES;
	}
	r2.right = long(r2.left + CALENDAR_DAY_WIDTH * m_ratio);
	r2.bottom = r2.top + CALENDAR_DAY_HEIGHT;

    for(int idx = 0; idx < WEEKDAY; idx++){

        if( idx == DAYSOFFSET_SUNDAY) {
			SetTextColor(hdc, RED_COLOR);
		}
        else if( idx == DAYSOFFSET_SATURDAY) {
			SetTextColor(hdc, BLUE_COLOR);
		}
        else {
			SetTextColor(hdc, BLACK_COLOR);
		}

		DrawTextEx(hdc, yobistr[idx].GetBuffer(), -1, &r2, DT_SINGLELINE | DT_VCENTER | DT_CENTER, &dtp);
		r2.left += long(CALENDAR_DAY_WIDTH * m_ratio);
		r2.right += long(CALENDAR_DAY_WIDTH * m_ratio);
	}

	DeleteObject(hBrush);
	SelectObject(hdc, hOldBrush);
    SetBkMode(hdc, iOldMode);

}

void CCalendarEx::DrawMonthBase14(int dofs)
{

	HDC hdc = ::GetDC(this->m_hWnd);
	CString yobistr[WEEKDAY] = { _T("日"), _T("月"), _T("火"), _T("水"), _T("木"), _T("金"), _T("土") };

	HBRUSH hBrush, hOldBrush;
	int iOldMode;
	hBrush = CreateSolidBrush(GRAY_COLOR);
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	iOldMode = SetBkMode(hdc, TRANSPARENT);

	DRAWTEXTPARAMS dtp;
	dtp.cbSize = sizeof(DRAWTEXTPARAMS);
	dtp.iLeftMargin = 0;
	dtp.iRightMargin = 0;
	dtp.iTabLength = 0;

	//年月
	CString ymstr;
	int month = m_s_month + dofs;
	int year = m_s_year;
	if (month > MONTH_NUM) {
		month -= MONTH_NUM;
		year++;
	}
	ymstr.Format(_T("%04d年%02d月"), year, month);

	CRect r;
	int calendar_day_width = CALENDAR_DAY_WIDTH * 60 / 70;
	r.left = long(CALENDAR_WIDTH_MARGIN + calendar_day_width * m_ratio * WEEKDAY * dofs + CALENDAR_WIDTH_MARGIN * dofs);
	if (dofs >= CALENDAR14_UPPER_DISPMAX) {
		r.left = long(CALENDAR_WIDTH_MARGIN + calendar_day_width * m_ratio * WEEKDAY * (dofs - CALENDAR14_UPPER_DISPMAX) + CALENDAR_WIDTH_MARGIN * (dofs - CALENDAR14_UPPER_DISPMAX));
	}
	r.top = 0;
	if (dofs >= CALENDAR14_UPPER_DISPMAX) {
		r.top += CALENDAR_DAY_HEIGHT * MONTH_LINES;
	}
	r.right = long(r.left + calendar_day_width * m_ratio * WEEKDAY);
	r.bottom = r.top + CALENDAR_DAY_HEIGHT;

	SetTextColor(hdc, BLACK_COLOR);
	DrawTextEx(hdc, ymstr.GetBuffer(), -1, &r, DT_SINGLELINE | DT_VCENTER | DT_CENTER, &dtp);

	//曜日
	CRect r2;
	r2.left = long(CALENDAR_WIDTH_MARGIN + calendar_day_width * m_ratio * WEEKDAY * dofs + CALENDAR_WIDTH_MARGIN * dofs);
	if (dofs >= CALENDAR14_UPPER_DISPMAX) {
		r2.left = long(CALENDAR_WIDTH_MARGIN + calendar_day_width * m_ratio * WEEKDAY * (dofs - CALENDAR14_UPPER_DISPMAX) + CALENDAR_WIDTH_MARGIN * (dofs - CALENDAR14_UPPER_DISPMAX));
	}
	r2.top = CALENDAR_DAY_HEIGHT;
	if (dofs >= CALENDAR14_UPPER_DISPMAX) {
		r2.top += CALENDAR_DAY_HEIGHT * MONTH_LINES;
	}
	r2.right = long(r2.left + calendar_day_width * m_ratio);
	r2.bottom = r2.top + CALENDAR_DAY_HEIGHT;

	for (int idx = 0; idx < WEEKDAY; idx++) {

		if (idx == DAYSOFFSET_SUNDAY) {
			SetTextColor(hdc, RED_COLOR);
		}
		else if (idx == DAYSOFFSET_SATURDAY) {
			SetTextColor(hdc, BLUE_COLOR);
		}
		else {
			SetTextColor(hdc, BLACK_COLOR);
		}

		DrawTextEx(hdc, yobistr[idx].GetBuffer(), -1, &r2, DT_SINGLELINE | DT_VCENTER | DT_CENTER, &dtp);
		r2.left += long(calendar_day_width * m_ratio);
		r2.right += long(calendar_day_width * m_ratio);
	}

	DeleteObject(hBrush);
	SelectObject(hdc, hOldBrush);
	SetBkMode(hdc, iOldMode);

}

void CCalendarEx::DrawMonthDays(int dofs)
{
	HDC hdc = ::GetDC(this->m_hWnd);

	HBRUSH hBrush, hOldBrush;
	int iOldMode;
	hBrush = CreateSolidBrush(GRAY_COLOR);
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	iOldMode = SetBkMode(hdc, TRANSPARENT);

	DRAWTEXTPARAMS dtp;
	dtp.cbSize = sizeof(DRAWTEXTPARAMS);
	dtp.iLeftMargin = 0;
	dtp.iRightMargin = 0;
	dtp.iTabLength = 0;

	int month = m_s_month + dofs;
	int year = m_s_year;
	if (month > MONTH_NUM) {
		month -= MONTH_NUM;
		year++;
	}

	int days = 1;
    int offset = 0;
    CString daystr;

	DAYSPT dpt;
	dpt.r.left = long(CALENDAR_WIDTH_MARGIN + CALENDAR_DAY_WIDTH * m_ratio * WEEKDAY * dofs + CALENDAR_WIDTH_MARGIN * dofs);
	if (dofs >= CALENDAR_UPPER_DISPMAX) {
		dpt.r.left = long(CALENDAR_WIDTH_MARGIN + CALENDAR_DAY_WIDTH * m_ratio * WEEKDAY * (dofs - CALENDAR_UPPER_DISPMAX) + CALENDAR_WIDTH_MARGIN * (dofs - CALENDAR_UPPER_DISPMAX));
	}
	dpt.r.top = CALENDAR_DAY_HEIGHT * 2;
	if (dofs >= CALENDAR_UPPER_DISPMAX) {
		dpt.r.top += CALENDAR_DAY_HEIGHT * MONTH_LINES;
	}
	dpt.r.right = long(dpt.r.left + CALENDAR_DAY_WIDTH * m_ratio);
	dpt.r.bottom = dpt.r.top + CALENDAR_DAY_HEIGHT;

    for( int y = 0; y < MONTH_LINES - 2; y++) {

		for (int x = 0; x < WEEKDAY; x++) {

			int ymd_tmp = year * 10000 + month * 100 + days;
            bool bOutYmd;
            if( m_ss_ymd <= ymd_tmp && ymd_tmp <= m_ee_ymd){
				bOutYmd = false;
			}
            else{
				bOutYmd = true;
			}

			bool bSyukujitsu = m_pMngTables->IsSyukujitsuFromMemory(ymd_tmp);
			bool bKyujitsu = m_pMngTables->IsKyujitsuFromMemory(ymd_tmp);
            bool bYobiHoliday = m_pMngTables->GetYobiCheckStateFromHolidayTable(x);
			offset = GetOffsetDay(year, month, days);

            if( bOutYmd ) {
				SetTextColor(hdc, GRAY_COLOR);
			}
            else {
				if (bSyukujitsu) {
					SetTextColor(hdc, RED_COLOR);
				}
				else {
					if (offset == DAYSOFFSET_SUNDAY) {
						SetTextColor(hdc, RED_COLOR);
					}
					else if (offset == DAYSOFFSET_SATURDAY) {
						SetTextColor(hdc, BLUE_COLOR);
					}
					else {
						SetTextColor(hdc, BLACK_COLOR);
					}
				}
			}

			if( y == 0 ) {
                if( x >= offset ) {

					if ((bYobiHoliday || bSyukujitsu || bKyujitsu) && !bOutYmd) {
						Rectangle(hdc, dpt.r.left, dpt.r.top, dpt.r.right, dpt.r.bottom);
					}

					daystr.Format(_T("%d"), days++);
					DrawTextEx(hdc, daystr.GetBuffer(), -1, &dpt.r, DT_SINGLELINE | DT_VCENTER | DT_CENTER, &dtp);

					dpt.ymd = ymd_tmp;
					dpt.wday = x;
//DbgViewTrace(_T("【DEBUG】ymd:%d left:%d top:%d right:%d bottom:%d\n"), dpt.ymd, dpt.r.left, dpt.r.top, dpt.r.right, dpt.r.bottom);
					m_dayspt_list.push_back(dpt);
                }
            }
            else {
                if( days <= GetLastDay(year, month, days) ) {

					if ((bYobiHoliday || bSyukujitsu || bKyujitsu) && !bOutYmd) {
						Rectangle(hdc, dpt.r.left, dpt.r.top, dpt.r.right, dpt.r.bottom);
					}

					daystr.Format(_T("%d"), days++);
					DrawTextEx(hdc, daystr.GetBuffer(), -1, &dpt.r, DT_SINGLELINE | DT_VCENTER | DT_CENTER, &dtp);

					dpt.ymd = ymd_tmp;
					dpt.wday = x;
//DbgViewTrace(_T("【DEBUG】ymd:%d left:%d top:%d right:%d bottom:%d\n"), dpt.ymd, dpt.r.left, dpt.r.top, dpt.r.right, dpt.r.bottom);
					m_dayspt_list.push_back(dpt);

                }
			}
			dpt.r.left += long(CALENDAR_DAY_WIDTH * m_ratio);
			dpt.r.right += long(CALENDAR_DAY_WIDTH * m_ratio);
		}

		dpt.r.left = long(CALENDAR_WIDTH_MARGIN + CALENDAR_DAY_WIDTH * m_ratio * WEEKDAY * dofs + CALENDAR_WIDTH_MARGIN * dofs);
		if (dofs >= CALENDAR_UPPER_DISPMAX) {
			dpt.r.left = long(CALENDAR_WIDTH_MARGIN + CALENDAR_DAY_WIDTH * m_ratio * WEEKDAY * (dofs - CALENDAR_UPPER_DISPMAX) + CALENDAR_WIDTH_MARGIN * (dofs - CALENDAR_UPPER_DISPMAX));
		}
		dpt.r.right = long(dpt.r.left + CALENDAR_DAY_WIDTH * m_ratio);
		dpt.r.top += CALENDAR_DAY_HEIGHT;
		dpt.r.bottom += CALENDAR_DAY_HEIGHT;
    }

	DeleteObject(hBrush);
	SelectObject(hdc, hOldBrush);
	SetBkMode(hdc, iOldMode);

}

void CCalendarEx::DrawMonthDays14(int dofs)
{
	HDC hdc = ::GetDC(this->m_hWnd);

	HBRUSH hBrush, hOldBrush;
	int iOldMode;
	hBrush = CreateSolidBrush(GRAY_COLOR);
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	iOldMode = SetBkMode(hdc, TRANSPARENT);

	DRAWTEXTPARAMS dtp;
	dtp.cbSize = sizeof(DRAWTEXTPARAMS);
	dtp.iLeftMargin = 0;
	dtp.iRightMargin = 0;
	dtp.iTabLength = 0;

	int month = m_s_month + dofs;
	int year = m_s_year;
	if (month > MONTH_NUM) {
		month -= MONTH_NUM;
		year++;
	}

	int days = 1;
	int offset = 0;
	CString daystr;

	DAYSPT dpt;
	int calendar_day_width = CALENDAR_DAY_WIDTH * 60 / 70;
	dpt.r.left = long(CALENDAR_WIDTH_MARGIN + calendar_day_width * m_ratio * WEEKDAY * dofs + CALENDAR_WIDTH_MARGIN * dofs);
	if (dofs >= CALENDAR14_UPPER_DISPMAX) {
		dpt.r.left = long(CALENDAR_WIDTH_MARGIN + calendar_day_width * m_ratio * WEEKDAY * (dofs - CALENDAR14_UPPER_DISPMAX) + CALENDAR_WIDTH_MARGIN * (dofs - CALENDAR14_UPPER_DISPMAX));
	}
	dpt.r.top = CALENDAR_DAY_HEIGHT * 2;
	if (dofs >= CALENDAR14_UPPER_DISPMAX) {
		dpt.r.top += CALENDAR_DAY_HEIGHT * MONTH_LINES;
	}
	dpt.r.right = long(dpt.r.left + calendar_day_width * m_ratio);
	dpt.r.bottom = dpt.r.top + CALENDAR_DAY_HEIGHT;

	for (int y = 0; y < MONTH_LINES - 2; y++) {

		for (int x = 0; x < WEEKDAY; x++) {

			int ymd_tmp = year * 10000 + month * 100 + days;
			bool bOutYmd;
			if (m_ss_ymd <= ymd_tmp && ymd_tmp <= m_ee_ymd) {
				bOutYmd = false;
			}
			else {
				bOutYmd = true;
			}
			bool bSyukujitsu = m_pMngTables->IsSyukujitsuFromMemory(ymd_tmp);
			bool bKyujitsu = m_pMngTables->IsKyujitsuFromMemory(ymd_tmp);
            bool bYobiHoliday = m_pMngTables->GetYobiCheckStateFromHolidayTable(x);
			offset = GetOffsetDay(year, month, days);

			if (bOutYmd) {
				SetTextColor(hdc, GRAY_COLOR);
			}
			else {
				if (bSyukujitsu) {
					SetTextColor(hdc, RED_COLOR);
				}
				else {

					if (offset == DAYSOFFSET_SUNDAY) {
						SetTextColor(hdc, RED_COLOR);
					}
					else if (offset == DAYSOFFSET_SATURDAY) {
						SetTextColor(hdc, BLUE_COLOR);
					}
					else {
						SetTextColor(hdc, BLACK_COLOR);
					}

				}
			}

			if (y == 0) {
				if (x >= offset) {

					if ((bYobiHoliday || bSyukujitsu || bKyujitsu) && !bOutYmd) {
						Rectangle(hdc, dpt.r.left, dpt.r.top, dpt.r.right, dpt.r.bottom);
					}

					daystr.Format(_T("%d"), days++);
					DrawTextEx(hdc, daystr.GetBuffer(), -1, &dpt.r, DT_SINGLELINE | DT_VCENTER | DT_CENTER, &dtp);

					dpt.ymd = ymd_tmp;
					dpt.wday = x;
//DbgViewTrace(_T("【DEBUG】ymd:%d left:%d top:%d right:%d bottom:%d\n"), dpt.ymd, dpt.r.left, dpt.r.top, dpt.r.right, dpt.r.bottom);
					m_dayspt_list.push_back(dpt);
				}
			}
			else {
				if (days <= GetLastDay(year, month, days)) {

					if ((bYobiHoliday || bSyukujitsu || bKyujitsu) && !bOutYmd) {
						Rectangle(hdc, dpt.r.left, dpt.r.top, dpt.r.right, dpt.r.bottom);
					}

					daystr.Format(_T("%d"), days++);
					DrawTextEx(hdc, daystr.GetBuffer(), -1, &dpt.r, DT_SINGLELINE | DT_VCENTER | DT_CENTER, &dtp);

					dpt.ymd = ymd_tmp;
					dpt.wday = x;
//DbgViewTrace(_T("【DEBUG】ymd:%d left:%d top:%d right:%d bottom:%d\n"), dpt.ymd, dpt.r.left, dpt.r.top, dpt.r.right, dpt.r.bottom);
					m_dayspt_list.push_back(dpt);

				}
			}
			dpt.r.left += long(calendar_day_width * m_ratio);
			dpt.r.right += long(calendar_day_width * m_ratio);
		}

		dpt.r.left = long(CALENDAR_WIDTH_MARGIN + calendar_day_width * m_ratio * WEEKDAY * dofs + CALENDAR_WIDTH_MARGIN * dofs);
		if (dofs >= CALENDAR14_UPPER_DISPMAX) {
			dpt.r.left = long(CALENDAR_WIDTH_MARGIN + calendar_day_width * m_ratio * WEEKDAY * (dofs - CALENDAR14_UPPER_DISPMAX) + CALENDAR_WIDTH_MARGIN * (dofs - CALENDAR14_UPPER_DISPMAX));
		}
		dpt.r.right = long(dpt.r.left + calendar_day_width * m_ratio);
		dpt.r.top += CALENDAR_DAY_HEIGHT;
		dpt.r.bottom += CALENDAR_DAY_HEIGHT;
	}

	DeleteObject(hBrush);
	SelectObject(hdc, hOldBrush);
	SetBkMode(hdc, iOldMode);
}

int CCalendarEx::GetOffsetDay(int year, int month, int day)
{
    int offset = 0;

	COleDateTime sDate(year, month, day, 0, 0, 0);
    offset = (int)sDate.GetDayOfWeek() - 1;

	//Valid return values range between 1 and 7, where 1 = Sunday, 2 = Monday, and so on.
    return offset;
}

int CCalendarEx::GetLastDay(int year, int month, int day)
{
	bool uru;
	uru = false;

	if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) {
		uru = true;
	}

    int monthlastdays[MONTH_NUM] = { 
                                     31, 28, 31, 30, 31, 30,
                                     31, 31, 30, 31, 30, 31
	                                };

    if( (uru == true) && (month == 2) ) {
        monthlastdays[month-1] = 29;
	}

	return monthlastdays[month-1];
}

bool CCalendarEx::IsPtInDate(POINT pt, DAYSPT* pDayspt)
{
    bool ret = false;
//DbgViewTrace(_T("【DEBUG】pt.x:%d pt.y:%d\n"), pt.x, pt.y);

	for (std::vector<DAYSPT>::iterator itr = m_dayspt_list.begin(); itr != m_dayspt_list.end(); itr++) {
//DbgViewTrace(_T("【DEBUG】ymd:%d left:%d top:%d right:%d bottom:%d\n"), itr->ymd, itr->r.left, itr->r.top, itr->r.right, itr->r.bottom);
        if( (itr->r.left <= pt.x && pt.x <= itr->r.right) && (itr->r.top <= pt.y && pt.y <= itr->r.bottom) ) {
			ret = true;
			pDayspt->ymd = itr->ymd;
			pDayspt->wday = itr->wday;
            pDayspt->r = itr->r;
            break;
		}
    }

    return ret;
}

CCalendarEx::~CCalendarEx()
{
}


BEGIN_MESSAGE_MAP(CCalendarEx, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CCalendarEx メッセージ ハンドラー

void CCalendarEx::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ここにメッセージ ハンドラー コードを追加します。
					   // 描画メッセージで CWnd::OnPaint() を呼び出さないでください。

    CRect r;
    GetClientRect(&r);

    HBRUSH hBrush, hOldBrush;
    hBrush = CreateSolidBrush(WHITE_COLOR);
    hOldBrush = (HBRUSH)SelectObject(dc, hBrush);    
	Rectangle(dc, r.left, r.top, r.right, r.bottom);
    DeleteObject(hBrush);
    SelectObject(dc, hOldBrush);

    //SetDCBrushColor(dc, NULL_BRUSH);

	m_dayspt_list.clear();

    if( m_DispMonth14 ) {
		for (int dofs = 0; dofs < MONTH_NUM+2; dofs++) {
			DrawMonthBase14(dofs);
			DrawMonthDays14(dofs);
		}
	}
    else {
		for (int dofs = 0; dofs < MONTH_NUM; dofs++) {
			DrawMonthBase(dofs);
			DrawMonthDays(dofs);
		}
	}

}


void CCalendarEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(&pt);

    DAYSPT dayspt;
    memset(&dayspt, 0, sizeof(DAYSPT));

    if ( IsPtInDate(pt, &dayspt) ) {

//DbgViewTrace(_T("【DEBUG】find. ymd:%d left:%d top:%d right:%d bottom:%d\n"), dayspt.ymd, dayspt.r.left, dayspt.r.top, dayspt.r.right, dayspt.r.bottom);

        bool bOutYmd;
        if( m_ss_ymd <= dayspt.ymd && dayspt.ymd <= m_ee_ymd){
			bOutYmd = false;
		}
        else {
			bOutYmd = true;
		}

		bool bSyukujitsu = m_pMngTables->IsSyukujitsuFromMemory(dayspt.ymd);
		bool bYobiHoliday = m_pMngTables->GetYobiCheckStateFromHolidayTable(dayspt.wday);
        if( bOutYmd || bSyukujitsu || bYobiHoliday) {
            return;
		}

		HDC hdc = ::GetDC(this->m_hWnd);
		HBRUSH hBrush, hOldBrush;
		int iOldMode;
		hBrush = CreateSolidBrush(YELLOW_COLOR);
		hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		iOldMode = SetBkMode(hdc, TRANSPARENT);

		DRAWTEXTPARAMS dtp;
		dtp.cbSize = sizeof(DRAWTEXTPARAMS);
		dtp.iLeftMargin = 0;
		dtp.iRightMargin = 0;
		dtp.iTabLength = 0;

		int year, month, day;
		CString daystr, msgstr;

		year = dayspt.ymd / 10000;
		month = (dayspt.ymd % 10000) / 100;
		day = (dayspt.ymd % 100);

		daystr.Format(_T("%d"), day);
		Rectangle(hdc, dayspt.r.left, dayspt.r.top, dayspt.r.right, dayspt.r.bottom);
		DrawTextEx(hdc, daystr.GetBuffer(), -1, &dayspt.r, DT_SINGLELINE | DT_VCENTER | DT_CENTER, &dtp);

        bool hflg;
        hflg = m_pMngTables->IsKyujitsuFromMemory(dayspt.ymd);

        if( hflg ) {
			msgstr.Format(_T("%04d年%02d月%02d日は集計除外日に設定されています。集計除外日から削除しますか？"), year, month, day);
		}
        else {
			msgstr.Format(_T("%04d年%02d月%02d日を集計除外日に設定しますか？"), year, month, day);
		}

        if( ICSMessageBox(msgstr, (MB_YESNO | MB_ICONQUESTION)) == IDYES) {

			HOLIDAYREC rec;
			rec.ymd = dayspt.ymd;
			memset(&rec.sname, 0, sizeof(rec.sname));

            if( hflg ){
                m_pMngTables->DeleteKyujitsuFromMemory(dayspt.ymd);
            }
            else {
				m_pMngTables->m_kyujitsu_list.push_back(rec);
				std::sort(m_pMngTables->m_kyujitsu_list.begin(), m_pMngTables->m_kyujitsu_list.end());
			}
            
            m_pMngTables->UpdateKyujitsuEigyobiTable(dayspt.ymd, !hflg);

		}

		DeleteObject(hBrush);
		SelectObject(hdc, hOldBrush);
		SetBkMode(hdc, iOldMode);
		Invalidate();
	}
    else {
//DbgViewTrace(_T("【DEBUG】not find. ymd:%d left:%d top:%d right:%d bottom:%d\n"), dayspt.ymd, dayspt.r.left, dayspt.r.top, dayspt.r.right, dayspt.r.bottom);
        return;
	}
   
	CWnd::OnLButtonDown(nFlags, point);
}
