// CSyukeiTenkiDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "SyukeiTenkiDlg.h"
#include "afxdialogex.h"

#define LINENUM 0x01

// CSyukeiTenkiDlg ダイアログ

IMPLEMENT_DYNAMIC(CSyukeiTenkiDlg, ICSDialog)

CSyukeiTenkiDlg::CSyukeiTenkiDlg(CWnd* pParent /*=nullptr*/)
	: ICSDialog(IDD_SYUKEI_DLG, pParent)
{
}

CSyukeiTenkiDlg::CSyukeiTenkiDlg(CWnd* pParent /*=nullptr*/,
	CMngTables* pMngTables,
	CSimKSyukei* pSyukei,
	EnumIdIcsShKazeihoushiki kazeihousiki)
	: ICSDialog(IDD_SYUKEI_DLG, pParent)
{

	m_pSyukei = pSyukei;
	m_pMngTables = pMngTables;
	m_ArithEx.l_defn(0x16);
	m_kazeihousiki = kazeihousiki;
	for (int idx_jigyokubun = 0; idx_jigyokubun; idx_jigyokubun++) {
		m_check_st[idx_jigyokubun] = 0;
	}

}

//CSyukeiTenkiDlg::CSyukeiTenkiDlg(CSimKSyukei* pSyukei, CMngTables* pMngTables, EnumIdIcsShKazeihoushiki kazeihousiki, CWnd* pParent /*=nullptr*/)
//	: ICSDialog(IDD_SYUKEI_DLG, pParent)
//{
//	m_pSyukei = pSyukei;
//	m_pMngTables = pMngTables;
//	m_ArithEx.l_defn(0x16);
//	m_kazeihousiki = kazeihousiki;
//	for (int idx_jigyokubun = 0; idx_jigyokubun; idx_jigyokubun++) {
//		m_check_st[idx_jigyokubun] = 0;
//	}
//}

CSyukeiTenkiDlg::~CSyukeiTenkiDlg()
{
}

void CSyukeiTenkiDlg::InitCheck()
{

    for( int idx_jigyokbn = 0; idx_jigyokbn < JIGYOKUBUN_MAX; idx_jigyokbn++){
        m_check_tenkikbn[idx_jigyokbn] = m_pMngTables->GetTenkiCheckStateFromSyukeiTenkiTable(idx_jigyokbn);
	}

    m_check_tenkikbn[0] ? m_check_nashi.SetCheck(true) : m_check_nashi.SetCheck(false);
    m_check_tenkikbn[1] ? m_check_oroshi.SetCheck(true) : m_check_oroshi.SetCheck(false);
    m_check_tenkikbn[2] ? m_check_kouri.SetCheck(true) : m_check_kouri.SetCheck(false);
    m_check_tenkikbn[3] ? m_check_seizo.SetCheck(true) : m_check_seizo.SetCheck(false);
    m_check_tenkikbn[4] ? m_check_sonota.SetCheck(true) : m_check_sonota.SetCheck(false);
    m_check_tenkikbn[5] ? m_check_service.SetCheck(true) : m_check_service.SetCheck(false);
    m_check_tenkikbn[6] ? m_check_hudosan.SetCheck(true) : m_check_hudosan.SetCheck(false);

    m_check_nashi.EnableWindow(false);
    m_check_oroshi.EnableWindow(false);
    m_check_kouri.EnableWindow(false);
	m_check_seizo.EnableWindow(false);
	m_check_sonota.EnableWindow(false);
	m_check_service.EnableWindow(false);
	m_check_hudosan.EnableWindow(false);

	if (m_kazeihousiki == ID_ICSSH_GENSOKU || m_kazeihousiki == ID_ICSSH_MENZEI) {
		m_check_oroshi.ShowWindow(false);
		m_check_kouri.ShowWindow(false);
		m_check_seizo.ShowWindow(false);
		m_check_sonota.ShowWindow(false);
		m_check_service.ShowWindow(false);
		m_check_hudosan.ShowWindow(false);
	}

	if (m_kazeihousiki == ID_ICSSH_GENSOKU || m_kazeihousiki == ID_ICSSH_MENZEI) {
        m_StaticText.ShowWindow(false);
	}

}

void CSyukeiTenkiDlg::InitReport()
{
    //既存のデータ削除.
    m_Report.RemoveAllRecords();
    m_Report.RemoveColumn(-1);

    //リサイズ.
    m_Report.Resize(FALSE, 0x00);

    //入力禁止
    m_Report.AllowEdit(FALSE);

    //カラムグリッド.
    m_Report.SetGridPerCount(TRUE, 1, BLUEGREEN_COLOR, RGS_SOLID);
    m_Report.SetGridColor(FALSE, GREY25_COLOR);

	//第5-(1)号様式
	m_Report.AddColumn(new ICSReportColumn(0, _T("事業区分"), 100));
	m_Report.AddColumn(new ICSReportColumn(1, _T("売上(適用期間)"), 100));     //①
	m_Report.AddColumn(new ICSReportColumn(2, _T("自年月日"), 100));           //②自
	m_Report.AddColumn(new ICSReportColumn(3, _T("至年月日"), 100));           //②至
	m_Report.AddColumn(new ICSReportColumn(4, _T("売上(10営業日)"), 100));     //③
	m_Report.AddColumn(new ICSReportColumn(5, _T("軽減売上(10営業日)"), 100)); //④
	m_Report.AddColumn(new ICSReportColumn(6, _T("軽減売上割合"), 100));       //⑤

	m_Report.SetSortable(FALSE);
	m_Report.SetDraggable(FALSE);
	m_Report.SetRemovable(FALSE);

    LOGFONT LogFont;
    CFont* pFont = GetFont();
    pFont->GetLogFont(&LogFont);
	m_Report.SetHeaderFont(LogFont);
	m_Report.SetTextFont(LogFont);

}

void CSyukeiTenkiDlg::AddSyukeiDataReport()
{

	char uriagegoukei[64], keigenuriagegoukei[64];
    char uriage[64], keigenuriage[64], keigenwariai[64];
    char kariuke[MONY_BUF_SIZE] = {0}, keigenkariuke[MONY_BUF_SIZE] = {0};
    char tumiag_keigenuriage[MONY_BUF_SIZE] = {0}, tumiag_uriage[MONY_BUF_SIZE] = {0};

	CString sddatestr, eddatestr, jigyokubunstr;
    char val100[MONY_BUF_SIZE] = {0};
    CString str100 =_T("100");
	m_ArithEx.l_input(&val100, str100.GetBuffer());

	for (int idx_jigyokubun = 0; idx_jigyokubun < JIGYOKUBUN_MAX; idx_jigyokubun++) {

		memset(&uriagegoukei[0], 0, sizeof(uriagegoukei));
		memset(&keigenuriagegoukei[0], 0, sizeof(keigenuriagegoukei));
		memset(&kariuke[0], 0, sizeof(kariuke));
		memset(&keigenkariuke[0], 0, sizeof(keigenkariuke));
		memset(&tumiag_keigenuriage[0], 0, sizeof(tumiag_keigenuriage));
		memset(&tumiag_uriage[0], 0, sizeof(tumiag_uriage));

		for (std::vector<DayRec>::iterator itr = m_pSyukei->m_DayRecList[idx_jigyokubun].begin(); itr != m_pSyukei->m_DayRecList[idx_jigyokubun].end(); itr++) {
			if (!itr->IsZero) {
				m_ArithEx.l_add(uriagegoukei, uriagegoukei, itr->uriage);
				m_ArithEx.l_add(keigenuriagegoukei, keigenuriagegoukei, itr->keigenuriage);
				m_ArithEx.l_add(kariuke, kariuke, itr->kariuke);
				m_ArithEx.l_add(keigenkariuke, keigenkariuke, itr->keigenkariuke);

				char KeigenUriageGoukeiStr[64];
				char KariukeStr[64], KeigenKariukeStr[64];

				m_ArithEx.l_print(KeigenUriageGoukeiStr, keigenuriagegoukei, _T("s,sss,sss,sss,ss9"));
				m_ArithEx.l_print(KariukeStr, kariuke, _T("s,sss,sss,sss,ss9"));
				m_ArithEx.l_print(KeigenKariukeStr, keigenkariuke, _T("s,sss,sss,sss,ss9"));
				m_pMngTables->UpdateSyukeiTenkiTable(idx_jigyokubun, 
                                                     KeigenUriageGoukeiStr,
                                                     KariukeStr, KeigenKariukeStr); 
				m_check_st[idx_jigyokubun] = true;
            }
        }

        if( m_check_st[idx_jigyokubun] == false) {
			m_pMngTables->ClearJigyokbnSyukeiTenkiTable(idx_jigyokubun);
		}

        m_ArithEx.l_print(uriagegoukei, uriagegoukei, _T("s,sss,sss,sss,ss9"));

		if (m_pSyukei->m_TenDayRecList[idx_jigyokubun].empty() == true) {
			m_pMngTables->ClearJigyokbnSyukeiTenkiTable(idx_jigyokubun);
            CButton* pButton[JIGYOKUBUN_MAX] = {&m_check_nashi, &m_check_oroshi, &m_check_kouri, &m_check_seizo, &m_check_sonota, &m_check_service, &m_check_hudosan};
            pButton[idx_jigyokubun]->SetCheck(false);
            m_check_st[idx_jigyokubun] = false;
			continue;
		}

		std::vector<TenDayRec>::iterator itr = m_pSyukei->m_TenDayRecList[idx_jigyokubun].begin();

		for (int line = 0; line < LINENUM; line++) {

            char tmpval[6];
			m_ArithEx.l_div(&tmpval, &itr->keigenwariai[0], val100);
			m_ArithEx.l_print(keigenwariai, &tmpval, _T("ss9"));
			m_ArithEx.l_print(uriage, &itr->uriage[0], _T("s,sss,sss,sss,ss9"));
			m_ArithEx.l_print(keigenuriage, &itr->keigenuriage[0], _T("s,sss,sss,sss,ss9"));

            if( m_ArithEx.l_test(uriagegoukei) ) {
				sddatestr.Format(_T("%ld"), itr->sddate);
				eddatestr.Format(_T("%ld"), itr->eddate);
			}
            else {
				sddatestr.Format(_T("%ld"), 0);
				eddatestr.Format(_T("%ld"), 0);
			}

			switch (idx_jigyokubun) {
			case ID_JIGYOKUBUN_NASHI:
				jigyokubunstr.Format(_T("区分なし"));
				if (m_check_st[idx_jigyokubun]) {
					m_check_nashi.EnableWindow(true);
				}
                else {
					m_check_nashi.SetCheck(false);
				}
				break;
			case ID_JIGYOKUBUN_OROSHI:
				jigyokubunstr.Format(_T("卸売業"));
                if( m_check_st[idx_jigyokubun] ) {
					m_check_oroshi.EnableWindow(true);
				}
                else {
					m_check_oroshi.SetCheck(false);
				}
				break;
			case ID_JIGYOKUBUN_KOURI:
				jigyokubunstr.Format(_T("小売業等"));
				if (m_check_st[idx_jigyokubun]) {
					m_check_kouri.EnableWindow(true);
				}
                else {
					m_check_kouri.SetCheck(false);
				}
				break;
			case ID_JIGYOKUBUN_SEIZO:
				jigyokubunstr.Format(_T("製造業等"));
				if (m_check_st[idx_jigyokubun]) {
					m_check_seizo.EnableWindow(true);
				}
                else {
					m_check_seizo.SetCheck(false);
				}
				break;
			case ID_JIGYOKUBUN_SONOTA:
				jigyokubunstr.Format(_T("その他"));
				if (m_check_st[idx_jigyokubun]) {
					m_check_sonota.EnableWindow(true);
				}
                else {
					m_check_sonota.SetCheck(false);
				}
				break;
			case ID_JIGYOKUBUN_SERVICE:
				jigyokubunstr.Format(_T("サービス業等"));
				if (m_check_st[idx_jigyokubun]) {
					m_check_service.EnableWindow(true);
				}
				else {
					m_check_service.SetCheck(false);
				}
				break;
			case ID_JIGYOKUBUN_HUDOSAN:
				jigyokubunstr.Format(_T("不動産業"));
				if (m_check_st[idx_jigyokubun]) {
					m_check_hudosan.EnableWindow(true);
				}
				else {
					m_check_hudosan.SetCheck(false);
				}
				break;
			default:
				jigyokubunstr.Format(_T("%ld"), idx_jigyokubun);
				break;
			}

			if (m_check_st[idx_jigyokubun]) {
				ICSReportRecord* pRec = m_Report.AddRecord(new ICSReportRecord());
				pRec->AddItem(new ICSReportRecordItemText(jigyokubunstr, ICS_EDS_ZENKAKU, true, true, 32, ICS_EDS_LEFT));
				pRec->AddItem(new ICSReportRecordItemText(uriagegoukei, ICS_EDS_NUMERIC, true, true, 32, ICS_EDS_RIGHT));
				pRec->AddItem(new ICSReportRecordItemText(sddatestr, ICS_EDS_NUMERIC, true, true, 32, ICS_EDS_RIGHT));
				pRec->AddItem(new ICSReportRecordItemText(eddatestr, ICS_EDS_NUMERIC, true, true, 32, ICS_EDS_RIGHT));
				pRec->AddItem(new ICSReportRecordItemText(uriage, ICS_EDS_NUMERIC, true, true, 32, ICS_EDS_RIGHT));
				pRec->AddItem(new ICSReportRecordItemText(keigenuriage, ICS_EDS_NUMERIC, true, true, 32, ICS_EDS_RIGHT));
				pRec->AddItem(new ICSReportRecordItemText(keigenwariai, ICS_EDS_NUMERIC, true, true, 32, ICS_EDS_RIGHT));
			}
		}
	}

}

void CSyukeiTenkiDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SYUKEITENKI_LIST, m_Report);
	DDX_Control(pDX, IDC_CHECK_OROSHI, m_check_oroshi);
	DDX_Control(pDX, IDC_CHECK_KOURI, m_check_kouri);
	DDX_Control(pDX, IDC_CHECK_SEIZO, m_check_seizo);
	DDX_Control(pDX, IDC_CHECK_SONOTA, m_check_sonota);
	DDX_Control(pDX, IDC_CHECK_SERVICE, m_check_service);
	DDX_Control(pDX, IDC_CHECK_HUDOSAN, m_check_hudosan);
	DDX_Control(pDX, IDC_CHECK_KBNNASHI, m_check_nashi);
	DDX_Control(pDX, IDC_STATIC_SYUKEI, m_StaticText);
}


BEGIN_MESSAGE_MAP(CSyukeiTenkiDlg, ICSDialog)
	ON_BN_CLICKED(IDOK, &CSyukeiTenkiDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_KBNNASHI, &CSyukeiTenkiDlg::OnBnClickedCheckKbnnashi)
	ON_BN_CLICKED(IDC_CHECK_OROSHI, &CSyukeiTenkiDlg::OnBnClickedCheckOroshi)
	ON_BN_CLICKED(IDC_CHECK_KOURI, &CSyukeiTenkiDlg::OnBnClickedCheckKouri)
	ON_BN_CLICKED(IDC_CHECK_SEIZO, &CSyukeiTenkiDlg::OnBnClickedCheckSeizo)
	ON_BN_CLICKED(IDC_CHECK_SONOTA, &CSyukeiTenkiDlg::OnBnClickedCheckSonota)
	ON_BN_CLICKED(IDC_CHECK_SERVICE, &CSyukeiTenkiDlg::OnBnClickedCheckService)
	ON_BN_CLICKED(IDC_CHECK_HUDOSAN, &CSyukeiTenkiDlg::OnBnClickedCheckHudosan)
END_MESSAGE_MAP()


// CSyukeiTenkiDlg メッセージ ハンドラー


BOOL CSyukeiTenkiDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO: ここに初期化を追加してください
    InitCheck();
    InitReport();

	//第5-(1)号様式
	AddSyukeiDataReport();

	m_Report.Populate();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CSyukeiTenkiDlg::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
    m_pMngTables->UpdateSyukeiTenkiTable(&m_Report, &m_check_tenkikbn[0], &m_check_st[0]);

    BOOL bIsTenki = FALSE;
    for(int idx_jigyokbn = 0; idx_jigyokbn < JIGYOKUBUN_MAX; idx_jigyokbn++){
        if( m_check_tenkikbn[idx_jigyokbn] ) {
            bIsTenki = TRUE;
            break;
		}
	}

    if( !bIsTenki ) {
		CString msgstr;
        msgstr = _T("転記事業区分にチェックがないため、集計が行えません。\n");
        ICSMessageBox(msgstr, MB_OK, 0);
        return;
	}

	ICSDialog::OnOK();

}


void CSyukeiTenkiDlg::OnBnClickedCheckKbnnashi()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
    if( m_check_nashi.GetCheck() ) {
		m_check_nashi.SetCheck(true);
		m_check_tenkikbn[0] = true;
	}
    else {
		m_check_nashi.SetCheck(false);
		m_check_tenkikbn[0] = false;
	}
}


void CSyukeiTenkiDlg::OnBnClickedCheckOroshi()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (m_check_oroshi.GetCheck()) {
		m_check_oroshi.SetCheck(true);
		m_check_tenkikbn[1] = true;
	}
	else {
		m_check_oroshi.SetCheck(false);
		m_check_tenkikbn[1] = false;
	}
}


void CSyukeiTenkiDlg::OnBnClickedCheckKouri()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (m_check_kouri.GetCheck()) {
		m_check_kouri.SetCheck(true);
		m_check_tenkikbn[2] = true;
	}
	else {
		m_check_kouri.SetCheck(false);
		m_check_tenkikbn[2] = false;
	}
}

void CSyukeiTenkiDlg::OnBnClickedCheckSeizo()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (m_check_seizo.GetCheck()) {
		m_check_seizo.SetCheck(true);
		m_check_tenkikbn[3] = true;
	}
	else {
		m_check_seizo.SetCheck(false);
		m_check_tenkikbn[3] = false;
	}
}

void CSyukeiTenkiDlg::OnBnClickedCheckSonota()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (m_check_sonota.GetCheck()) {
		m_check_sonota.SetCheck(true);
		m_check_tenkikbn[4] = true;
	}
	else {
		m_check_sonota.SetCheck(false);
		m_check_tenkikbn[4] = false;
	}
}


void CSyukeiTenkiDlg::OnBnClickedCheckService()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (m_check_service.GetCheck()) {
		m_check_service.SetCheck(true);
		m_check_tenkikbn[5] = true;
	}
	else {
		m_check_service.SetCheck(false);
		m_check_tenkikbn[5] = false;
	}
}


void CSyukeiTenkiDlg::OnBnClickedCheckHudosan()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (m_check_hudosan.GetCheck()) {
		m_check_hudosan.SetCheck(true);
		m_check_tenkikbn[6] = true;
	}
	else {
		m_check_hudosan.SetCheck(false);
		m_check_tenkikbn[6] = false;
	}

}
