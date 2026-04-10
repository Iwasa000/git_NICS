// SimKuriwariDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "SimulateKuriwari.h"
#include "SimKuriwariDlg.h"
#include "afxdialogex.h"
#include "SyukeiTenkiDlg.h"

// CSimKuriwariDlg ダイアログ

IMPLEMENT_DYNAMIC(CSimKuriwariDlg, ICSDialog)

CSimKuriwariDlg::CSimKuriwariDlg(CDBZmSub* pZmSub, CWnd* pParent /*=nullptr*/)
	: ICSDialog(IDD_SIMULATEKURIWARI_DLG, pParent)
{
    m_pZmSub = pZmSub;
	m_MngTables.m_pZmSub = m_pZmSub;
	m_MngTables.Init();
	m_Syukei.m_pZmSub = m_pZmSub;
	m_Syukei.Init();
    m_CalendarEx.m_pZmSub = m_pZmSub;
    m_CalendarEx.m_pMngTables = &m_MngTables;
    m_CalendarEx.m_pMngTables->m_pZmSub = m_pZmSub;
    m_pSettingHolidaysDlg = NULL;
	m_ArithEx.l_defn(0x16);

    BOOL bOpenFlg = FALSE;
    if( m_pZmSub->zvol == NULL ) {
        m_pZmSub->VolumeOpen();
	}
	m_kazeihousiki = m_SyzUtil.GetKazeihoushiki(m_pZmSub->zvol);

    if( bOpenFlg ) {
        m_pZmSub->VolumeClose();
	}
}

void CSimKuriwariDlg::EndSettingHolidaysDlg(int nResult)
{
    if( m_pSettingHolidaysDlg ) {
		m_pSettingHolidaysDlg->EndDialog(nResult);
	}
}

void CSimKuriwariDlg::EndSyukeiTenkiDlg(int nResult)
{
    if( m_pSyukeiTenkiDlg ) {
		m_pSyukeiTenkiDlg->EndDialog(nResult);
	}
}

CSimKuriwariDlg::~CSimKuriwariDlg()
{
	if (m_pSettingHolidaysDlg) {
		delete m_pSettingHolidaysDlg;
		m_pSettingHolidaysDlg = NULL;
	}
}

void CSimKuriwariDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM_CALENDAREX, m_CalendarEx);
	DDX_Control(pDX, IDC_BUTTON_SETTING, m_ButtonSetting);
	DDX_Control(pDX, IDC_BUTTON_SYUKEI, m_ButtonSyukei);
	DDX_Control(pDX, IDOK, m_ButtonOK);
	DDX_Control(pDX, IDCANCEL, m_ButtonCancel);
	DDX_Control(pDX, IDC_STATIC_GUIDE2, m_StaticGuide2);
}

void CSimKuriwariDlg::InitCalendar()
{
	m_CalendarEx.m_s_year = m_pZmSub->zvol->ss_ymd / 10000;
	m_CalendarEx.m_s_month = (m_pZmSub->zvol->ss_ymd % 10000) / 100;
	m_CalendarEx.m_ss_ymd = m_pZmSub->zvol->ss_ymd;
	m_CalendarEx.m_ee_ymd = m_pZmSub->zvol->ee_ymd;

	int e_year = m_pZmSub->zvol->ee_ymd / 10000;
	int e_month = (m_pZmSub->zvol->ee_ymd % 10000) / 100;

	CRect rc;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
	m_CalendarEx.m_ratio = (double)rc.Width() / RESOLUTIONDEFAULT;

    if( (m_CalendarEx.m_s_year != e_year) && (m_CalendarEx.m_s_month == e_month) ) {
        //20日締め等,12ケ月を超える場合は7ケ月x2段表示にする.
		m_CalendarEx.m_DispMonth14 = true;
		InitWindow2();
	}
    else {
		m_CalendarEx.m_DispMonth14 = false;
		InitWindow();
	}

}

void CSimKuriwariDlg::InitWindow()
{
	CRect rc;
	GetWindowRect(rc);
	rc.right = (long)(rc.left + rc.Width() * m_CalendarEx.m_ratio);
	MoveWindow(rc);

	int x, y, nWidth, nHeight;
	x = CALENDAR_WIDTH_MARGIN * 2;
	y = CALENDAR_HEIGHT_MARGIN * 2;
	nWidth = (int)((CALENDAR_DAY_WIDTH * WEEKDAY * 6) * m_CalendarEx.m_ratio + CALENDAR_WIDTH_MARGIN * 7);
	nHeight = CALENDAR_DAY_HEIGHT * MONTH_LINES * 2;
	m_CalendarEx.MoveWindow(x, y, nWidth, nHeight);

	m_ButtonSetting.GetWindowRect(rc);
	nWidth = int(rc.Width() * m_CalendarEx.m_ratio);
	m_ButtonSetting.MoveWindow(x * 2, y * 2 + nHeight, nWidth, rc.Height());

	int width2;
	width2 = nWidth + x * 3;
	m_StaticGuide2.GetWindowRect(rc);
	nWidth = (int)(rc.Width() * m_CalendarEx.m_ratio);
	m_StaticGuide2.MoveWindow(width2, y * 3 + nHeight, nWidth, rc.Height());

	width2 = int((CALENDAR_DAY_WIDTH * WEEKDAY * 450 / 100) * m_CalendarEx.m_ratio + int(CALENDAR_WIDTH_MARGIN * m_CalendarEx.m_ratio * 7));
	m_ButtonOK.GetWindowRect(rc);
	nWidth = int(rc.Width() * m_CalendarEx.m_ratio);
	m_ButtonOK.MoveWindow(width2 + x + nWidth, y * 8 + nHeight, nWidth, rc.Height());

}

void CSimKuriwariDlg::InitWindow2()
{
	CRect rc;
	GetWindowRect(rc);
	rc.right = int(rc.left + rc.Width() * m_CalendarEx.m_ratio);
	MoveWindow(rc);

	int x, y, nWidth, nHeight;
	x = CALENDAR_WIDTH_MARGIN * 2;
	y = CALENDAR_HEIGHT_MARGIN * 2;
	nWidth = int((CALENDAR_DAY_WIDTH * WEEKDAY * 6) * m_CalendarEx.m_ratio + CALENDAR_WIDTH_MARGIN * 7);
	nHeight = CALENDAR_DAY_HEIGHT * MONTH_LINES * 2;
	m_CalendarEx.MoveWindow(x, y, nWidth, nHeight);

	m_ButtonSetting.GetWindowRect(rc);
	nWidth = int(rc.Width() * m_CalendarEx.m_ratio);
	m_ButtonSetting.MoveWindow(x * 2, y * 2 + nHeight, nWidth, rc.Height());

	m_ButtonSyukei.GetWindowRect(rc);
	nWidth = int(rc.Width() * m_CalendarEx.m_ratio);
	m_ButtonSyukei.MoveWindow(x * 3 + nWidth, y * 2 + nHeight, nWidth, rc.Height());

	int width2;
	width2 = int((CALENDAR_DAY_WIDTH * WEEKDAY * 450 / 100) * m_CalendarEx.m_ratio + int(CALENDAR_WIDTH_MARGIN * m_CalendarEx.m_ratio * 7));
	m_ButtonOK.GetWindowRect(rc);
	nWidth = int(rc.Width() * m_CalendarEx.m_ratio);
	m_ButtonOK.MoveWindow(width2 + x + nWidth, y * 8 + nHeight, nWidth, rc.Height());

}

BEGIN_MESSAGE_MAP(CSimKuriwariDlg, ICSDialog)
	ON_BN_CLICKED(IDC_BUTTON_SYUKEI, &CSimKuriwariDlg::OnBnClickedSyukeiTenki)
	ON_BN_CLICKED(IDC_BUTTON_SETTING, &CSimKuriwariDlg::OnBnClickedButtonSetting)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDOK, &CSimKuriwariDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CSimKuriwariDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO: ここに初期化を追加してください
	m_MngTables.CreateEigyobiTable();
	m_MngTables.CreateSyukujitsuTable();
	m_MngTables.CreateKyujitsuTable();
	m_MngTables.CreateSettingHolidayTable();
	m_MngTables.CreateSyukeiTenkiTable();

	m_MngTables.ReadSyukujitsu();
	m_MngTables.ReadKyujitsu();

    InitCalendar();

    m_ButtonSyukei.ShowWindow(SW_HIDE);
	m_ButtonCancel.ShowWindow(SW_HIDE);
    
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// CSimKuriwariDlg メッセージ ハンドラー
void CSimKuriwariDlg::OnBnClickedSyukeiTenki()
{
#if 0
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
    m_MngTables.CreateTmpTable(true);

    //集計データ生成 & 取得.
    m_Syukei.GetDbData();

    //日毎集計.
    m_Syukei.SyukeiDay();

    //10営業日毎に集計.
	m_Syukei.SyukeiRenzoku10Eigyobi();

    //集計ダイアログ表示
    if( m_pSyukeiTenkiDlg ) {
        delete m_pSyukeiTenkiDlg;
        m_pSyukeiTenkiDlg = NULL;
	}

    m_pSyukeiTenkiDlg = new CSyukeiTenkiDlg(&m_Syukei, &m_MngTables, m_kazeihousiki);
    if( m_pSyukeiTenkiDlg->DoModal() != IDOK ) {
		m_pSyukeiTenkiDlg->DestroyWindow();
        delete m_pSyukeiTenkiDlg;
        m_pSyukeiTenkiDlg = NULL;
    }
#endif
    return;
}

void CSimKuriwariDlg::OnBnClickedButtonSetting()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (m_pSettingHolidaysDlg) {
		delete m_pSettingHolidaysDlg;
		m_pSettingHolidaysDlg = NULL;
	}

    m_pSettingHolidaysDlg = new CSettingHolidaysDlg(m_pZmSub, &m_MngTables, this);

    if( m_pSettingHolidaysDlg->DoModal() == IDOK ) {
       //カレンダーの再描画.
       m_CalendarEx.Invalidate();
    }
}


void CSimKuriwariDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	ICSDialog::OnLButtonDown(nFlags, point);
}


void CSimKuriwariDlg::OnBnClickedOk()
{

	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	m_MngTables.UpdateKyujitsuTable();

	ICSDialog::OnOK();

}
