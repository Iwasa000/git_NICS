// SettingHolidaysDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "SimulateKuriwari.h"
#include "SettingHolidaysDlg.h"
#include "afxdialogex.h"

#define COLUMN_DATE 0x00
#define COLUMN_NAME 0x01
#define DIGITS_DATE 0x06

// CSettingHolidaysDlg ダイアログ

IMPLEMENT_DYNAMIC(CSettingHolidaysDlg, ICSDialog)

CSettingHolidaysDlg::CSettingHolidaysDlg(CWnd* pParent /*=nullptr*/)
	: ICSDialog(IDD_SETTING_HOLIDAYS_DLG, pParent)
{
}

CSettingHolidaysDlg::CSettingHolidaysDlg(CDBZmSub* pZmSub, CMngTables* pMngTables, CWnd* pParent /*=nullptr*/)
	: ICSDialog(IDD_SETTING_HOLIDAYS_DLG, pParent)
{
    m_pZmSub = pZmSub;
    m_pMngTables = pMngTables;
}

CSettingHolidaysDlg::~CSettingHolidaysDlg()
{
	ClearReport(&m_ReportS);
	ClearReport(&m_ReportK);
}

void CSettingHolidaysDlg::InitCheckWday()
{
    //出勤曜日チェックボックス初期化.
    for(int wday = 0; wday < WEEKDAY; wday++){
        m_check_wday[wday] = m_pMngTables->GetYobiCheckStateFromHolidayTable(wday);
	}

	m_check_wday[0] ? m_check_doff0.SetCheck(true) : m_check_doff0.SetCheck(false);
	m_check_wday[1] ? m_check_doff1.SetCheck(true) : m_check_doff1.SetCheck(false);
	m_check_wday[2] ? m_check_doff2.SetCheck(true) : m_check_doff2.SetCheck(false);
	m_check_wday[3] ? m_check_doff3.SetCheck(true) : m_check_doff3.SetCheck(false);
	m_check_wday[4] ? m_check_doff4.SetCheck(true) : m_check_doff4.SetCheck(false);
	m_check_wday[5] ? m_check_doff5.SetCheck(true) : m_check_doff5.SetCheck(false);
	m_check_wday[6] ? m_check_doff6.SetCheck(true) : m_check_doff6.SetCheck(false);

}

void CSettingHolidaysDlg::InitReport(ICSReportControl* p)
{
	//既存のデータ削除
	ClearReport(p);

	//リサイズ
	p->Resize(FALSE, 0x00);

	//入力禁止
	p->AllowEdit(TRUE);

	//カラムグリッド
	p->SetGridPerCount(TRUE, 1, BLUEGREEN_COLOR, RGS_SOLID);
	p->SetGridColor(FALSE, GREY25_COLOR);

	p->AddColumn(new ICSReportColumn(0, _T("年月日"), 100));
	p->AddColumn(new ICSReportColumn(1, _T("名称"), 200));

    p->SetSortable(FALSE);
    p->SetDraggable(FALSE);
    p->SetRemovable(FALSE);

}

void CSettingHolidaysDlg::SetReport(ICSReportControl* p, std::vector<HOLIDAYREC>* pVec)
{
    p->RemoveAllRecords();

    CString ymdstr, syukukyustr;
	int s_gengo = 0, d_gengo = 0, wyear = 0;

	for (std::vector<HOLIDAYREC>::iterator itr = pVec->begin(); itr != pVec->end(); itr++) {

		ymdstr.Empty();
		syukukyustr.Empty();
		wyear = 0;
		m_Voldate.db_datecnvGen(s_gengo, itr->ymd, &d_gengo, &wyear, 0, 0);
		ymdstr.Format(_T("%06d"), wyear);
		syukukyustr.Format(_T("%s"), itr->sname);

		ICSReportRecord* pRec = p->AddRecord(new ICSReportRecord());
		pRec->AddItem(new ICSReportRecordItemText(ymdstr, ICS_EDS_NUMERIC, true, true, 8, ICS_EDS_LEFT));
		pRec->AddItem(new ICSReportRecordItemText(syukukyustr, ICS_EDS_ZENKAKU, true, true, 32, ICS_EDS_LEFT));

	}

    p->Populate();
}

void CSettingHolidaysDlg::AddRow(ICSReportControl* p)
{

    //フォーカス行の下に追加.
	ICSReportRecord* pRec = new ICSReportRecord();

	ICSReportRow* pRow = p->GetFocusedRow();
    if( pRow ) {
		pRec->AddItem(new ICSReportRecordItemText(_T(""), ICS_EDS_NUMERIC, true, true, 8, ICS_EDS_LEFT));
		pRec->AddItem(new ICSReportRecordItemText(_T(""), ICS_EDS_ZENKAKU, true, true, 32, ICS_EDS_LEFT));
		p->InsertRecordAt(pRow->GetIndex() + 1, pRec);
    }
    else {
		pRec->AddItem(new ICSReportRecordItemText(_T(""), ICS_EDS_NUMERIC, true, true, 8, ICS_EDS_LEFT));
		pRec->AddItem(new ICSReportRecordItemText(_T(""), ICS_EDS_ZENKAKU, true, true, 32, ICS_EDS_LEFT));
		p->InsertRecordAt(0, pRec);
	}
    p->Populate();
}

void CSettingHolidaysDlg::DeleteRow(ICSReportControl* p)
{
    //フォーカス行を削除.
	ICSReportRow* pRow = p->GetFocusedRow();
    if( pRow ) {
        p->RemoveRecordAt(pRow->GetIndex());
    }
	p->Populate();
}

void CSettingHolidaysDlg::ClearReport(ICSReportControl* p)
{
	p->RemoveAllRecords();
	p->RemoveColumn(-1);
}

void CSettingHolidaysDlg::EditRightItemReport(ICSReportControl* p, int idxRow, int idxColumn)
{

	if (idxColumn == COLUMN_NAME) {
		p->EditItem(idxRow + 1, 0, TRUE);
	}
	else {
		p->EditItem(idxRow, 1, TRUE);
	}

    return;
}

void CSettingHolidaysDlg::EditLeftItemReport(ICSReportControl* p, int idxRow, int idxColumn)
{

	if (idxColumn == COLUMN_NAME) {
		p->EditItem(idxRow, 0, TRUE);
	}
	else {
		p->EditItem(idxRow-1, 1, TRUE);
	}

    return;
}

void CSettingHolidaysDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_DAYOFF1, m_check_doff1);
	DDX_Control(pDX, IDC_CHECK_DAYOFF2, m_check_doff2);
	DDX_Control(pDX, IDC_CHECK_DAYOFF3, m_check_doff3);
	DDX_Control(pDX, IDC_CHECK_DAYOFF4, m_check_doff4);
	DDX_Control(pDX, IDC_CHECK_DAYOFF5, m_check_doff5);
	DDX_Control(pDX, IDC_CHECK_DAYOFF6, m_check_doff6);
	DDX_Control(pDX, IDC_CHECK_DAYOFF0, m_check_doff0);
	DDX_Control(pDX, IDC_SYUKUJITSU_LIST, m_ReportS);
	DDX_Control(pDX, IDC_KYUJITSU_LIST, m_ReportK);
	DDX_Control(pDX, IDC_BUTTON_S_ADD, m_ButtonSA);
	DDX_Control(pDX, IDC_BUTTON_S_DELETE, m_ButtonSD);
	DDX_Control(pDX, IDC_BUTTON_K_ADD, m_ButtonKA);
	DDX_Control(pDX, IDC_BUTTON_K_DELETE, m_ButtonKD);
}

BEGIN_MESSAGE_MAP(CSettingHolidaysDlg, ICSDialog)
	ON_BN_CLICKED(IDC_CHECK_DAYOFF1, &CSettingHolidaysDlg::OnBnClickedCheckDayoff1)
	ON_BN_CLICKED(IDC_CHECK_DAYOFF2, &CSettingHolidaysDlg::OnBnClickedCheckDayoff2)
	ON_BN_CLICKED(IDC_CHECK_DAYOFF3, &CSettingHolidaysDlg::OnBnClickedCheckDayoff3)
	ON_BN_CLICKED(IDC_CHECK_DAYOFF4, &CSettingHolidaysDlg::OnBnClickedCheckDayoff4)
	ON_BN_CLICKED(IDC_CHECK_DAYOFF5, &CSettingHolidaysDlg::OnBnClickedCheckDayoff5)
	ON_BN_CLICKED(IDC_CHECK_DAYOFF6, &CSettingHolidaysDlg::OnBnClickedCheckDayoff6)
	ON_BN_CLICKED(IDC_CHECK_DAYOFF0, &CSettingHolidaysDlg::OnBnClickedCheckDayoff0)
	ON_BN_CLICKED(IDOK, &CSettingHolidaysDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_S_ADD, &CSettingHolidaysDlg::OnBnClickedButtonSAdd)
	ON_BN_CLICKED(IDC_BUTTON_S_DELETE, &CSettingHolidaysDlg::OnBnClickedButtonSDelete)
	ON_BN_CLICKED(IDC_BUTTON_K_ADD, &CSettingHolidaysDlg::OnBnClickedButtonKAdd)
	ON_BN_CLICKED(IDC_BUTTON_K_DELETE, &CSettingHolidaysDlg::OnBnClickedButtonKDelete)
	ON_NOTIFY(ICS_NM_REPORT_VALUECHANGED, IDC_SYUKUJITSU_LIST, OnNMReportValueChangedReportS)
	ON_NOTIFY(ICS_NM_REPORT_VALUECHANGED, IDC_KYUJITSU_LIST, OnNMReportValueChangedReportK)
	ON_NOTIFY(ICS_NM_REPORT_EDITKEYDOWN, IDC_SYUKUJITSU_LIST, OnNMReportEditkeydownReportS)
	ON_NOTIFY(ICS_NM_REPORT_EDITKEYDOWN, IDC_KYUJITSU_LIST, OnNMReportEditkeydownReportK)
	ON_BN_CLICKED(IDC_BUTTON_TORIKOMI, &CSettingHolidaysDlg::OnBnClickedButtonInit)
END_MESSAGE_MAP()


// CSettingHolidaysDlg メッセージ ハンドラー

void CSettingHolidaysDlg::OnBnClickedCheckDayoff1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
    if( m_check_doff1.GetCheck() ) {
		m_check_doff1.SetCheck(true);
		m_check_wday[1] = true;
	}
    else {
		m_check_doff1.SetCheck(false);
		m_check_wday[1] = false;
	}
}

void CSettingHolidaysDlg::OnBnClickedCheckDayoff2()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (m_check_doff2.GetCheck()) {
		m_check_doff2.SetCheck(true);
		m_check_wday[2] = true;
	}
	else {
		m_check_doff2.SetCheck(false);
		m_check_wday[2] = false;
	}
}


void CSettingHolidaysDlg::OnBnClickedCheckDayoff3()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (m_check_doff3.GetCheck()) {
		m_check_doff3.SetCheck(true);
		m_check_wday[3] = true;
	}
	else {
		m_check_doff3.SetCheck(false);
		m_check_wday[3] = false;
	}
}


void CSettingHolidaysDlg::OnBnClickedCheckDayoff4()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (m_check_doff4.GetCheck()) {
		m_check_doff4.SetCheck(true);
		m_check_wday[4] = true;
	}
	else {
		m_check_doff4.SetCheck(false);
		m_check_wday[4] = false;
	}
}


void CSettingHolidaysDlg::OnBnClickedCheckDayoff5()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (m_check_doff5.GetCheck()) {
		m_check_doff5.SetCheck(true);
		m_check_wday[5] = true;
	}
	else {
		m_check_doff5.SetCheck(false);
		m_check_wday[5] = false;
	}
}


void CSettingHolidaysDlg::OnBnClickedCheckDayoff6()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (m_check_doff6.GetCheck()) {
		m_check_doff6.SetCheck(true);
		m_check_wday[6] = true;
	}
	else {
		m_check_doff6.SetCheck(false);
		m_check_wday[6] = false;
	}
}


void CSettingHolidaysDlg::OnBnClickedCheckDayoff0()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (m_check_doff0.GetCheck()) {
		m_check_doff0.SetCheck(true);
		m_check_wday[0] = true;
	}
	else {
		m_check_doff0.SetCheck(false);
		m_check_wday[0] = false;
	}
}

void CSettingHolidaysDlg::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	m_pMngTables->RemakeSyukujitsuList(&m_ReportS);
	m_pMngTables->RemakeKyujitsuList(&m_ReportK);

	m_pMngTables->UpdateSettingHolidayTable(&m_check_wday[0]);
	m_pMngTables->UpdateEigyobiTable(&m_check_wday[0]);
	m_pMngTables->UpdateSyukujitsuTable();
	m_pMngTables->UpdateKyujitsuTable();

    ICSDialog::OnOK();
}


BOOL CSettingHolidaysDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO: ここに初期化を追加してください
	InitCheckWday();

	InitReport(&m_ReportS);
	InitReport(&m_ReportK);

	SetReport(&m_ReportS, &m_pMngTables->m_syukujitsu_list);
	SetReport(&m_ReportK, &m_pMngTables->m_kyujitsu_list);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CSettingHolidaysDlg::OnBnClickedButtonSAdd()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	AddRow(&m_ReportS);
}


void CSettingHolidaysDlg::OnBnClickedButtonSDelete()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	DeleteRow(&m_ReportS);
}


void CSettingHolidaysDlg::OnBnClickedButtonKAdd()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	AddRow(&m_ReportK);
}


void CSettingHolidaysDlg::OnBnClickedButtonKDelete()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	DeleteRow(&m_ReportK);
}

void CSettingHolidaysDlg::OnBnClickedButtonInit()
{

	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (ICSMessageBox(_T("祝日を一括で取り込みますか？\n※登録済みの祝日に上書きして取り込みを行います。\n"),
		(MB_YESNO | MB_DEFBUTTON2), 0, 0, this) == IDNO) {
		return;
	}

	//祝日リスト初期化.
    m_pMngTables->ReInitSyukujitsuList();
	SetReport(&m_ReportS, &m_pMngTables->m_syukujitsu_list);

	return;
}

void CSettingHolidaysDlg::OnNMReportValueChangedReportS(NMHDR *pNotifyStruct, LRESULT *result)
{

	ICS_NM_REPORTRECORDITEM* pItemNotify = (ICS_NM_REPORTRECORDITEM*)pNotifyStruct;

	//変更のあったカラム.
	ICSReportColumn* pColumn = pItemNotify->pColumn;
	int indexColumn = pColumn->GetIndex();

	//変更のあった行.
	ICSReportRow* pRow = pItemNotify->pRow;
	int indexRow = pRow->GetIndex();

	//変更のあったアイテム.
	ICSReportRecordItem* pItem = pItemNotify->pItem;
	int indexItem = pItem->GetIndex();

	if (indexColumn == COLUMN_DATE) {
        CString str = pItem->GetCaptionString(NULL);
		int length = str.GetLength();
        if( length > DIGITS_DATE) {
			m_ReportS.SetCaption(indexRow, indexColumn, _T(""));
			m_ReportS.Populate();
            return;
		}
	}
}

void CSettingHolidaysDlg::OnNMReportEditkeydownReportS(NMHDR *pNotifyStruct, LRESULT *result)
{
	ICS_NM_REPORTRECORDITEM* pItemNotify = (ICS_NM_REPORTRECORDITEM*)pNotifyStruct;

	//変更のあったカラム.
	ICSReportColumn* pColumn = pItemNotify->pColumn;
	int indexColumn = pColumn->GetIndex();

	//変更のあった行.
	ICSReportRow* pRow = pItemNotify->pRow;
	int indexRow = pRow->GetIndex();

	//変更のあったアイテム.
	ICSReportRecordItem* pItem = pItemNotify->pItem;
	int indexItem = pItem->GetIndex();

    if(indexColumn == COLUMN_DATE ) {

		CString str = pItem->GetCaptionString(NULL);
		int tmp_ymd = _ttoi(str);

        int year, month, day, offset;
		int s_gengo = 0, d_gengo = 0, d_year = 0;

		m_Voldate.db_datecnvGen(s_gengo, tmp_ymd, &d_gengo, &d_year, 1, 0);
		year = d_year / 10000;
		month = (d_year % 10000) / 100;
		day = (d_year % 100);

		offset = m_pMngTables->GetOffsetDay(year, month, day);
		if ((offset < DAYSOFFSET_SUNDAY) || (offset > DAYSOFFSET_SATURDAY) || (str.GetLength() < DIGITS_DATE)) {
			m_ReportS.SetCaption(indexRow, indexColumn, _T(""));
			m_ReportS.Populate();
            return;
		}

	}

	//ターミネーションキーコード
	UINT nChar = pItemNotify->nChar;

	switch (nChar) {

		case VK_RIGHT:
			EditRightItemReport(&m_ReportS, indexRow, indexColumn);
            break;

		case VK_RETURN:
		case VK_TAB:
			if (GetKeyState(VK_SHIFT) & 0x8000) {
				EditLeftItemReport(&m_ReportS, indexRow, indexColumn);
			}
            else {
				EditRightItemReport(&m_ReportS, indexRow, indexColumn);
			}
            break;

		case VK_LEFT:
			EditLeftItemReport(&m_ReportS, indexRow, indexColumn);
			break;

        default:
            break;
	}
}

void CSettingHolidaysDlg::OnNMReportValueChangedReportK(NMHDR *pNotifyStruct, LRESULT *result)
{

	ICS_NM_REPORTRECORDITEM* pItemNotify = (ICS_NM_REPORTRECORDITEM*)pNotifyStruct;

	//変更のあったカラム.
	ICSReportColumn* pColumn = pItemNotify->pColumn;
	int indexColumn = pColumn->GetIndex();

	//変更のあった行.
	ICSReportRow* pRow = pItemNotify->pRow;
	int indexRow = pRow->GetIndex();

	//変更のあったアイテム.
	ICSReportRecordItem* pItem = pItemNotify->pItem;
	int indexItem = pItem->GetIndex();

	if (indexColumn == COLUMN_DATE) {
		CString str = pItem->GetCaptionString(NULL);
		int length = str.GetLength();
		if (length > DIGITS_DATE) {
			m_ReportK.SetCaption(indexRow, indexColumn, _T(""));
			m_ReportK.Populate();
			return;
		}
	}
}

void CSettingHolidaysDlg::OnNMReportEditkeydownReportK(NMHDR *pNotifyStruct, LRESULT *result)
{
	ICS_NM_REPORTRECORDITEM* pItemNotify = (ICS_NM_REPORTRECORDITEM*)pNotifyStruct;

	//変更のあったカラム.
	ICSReportColumn* pColumn = pItemNotify->pColumn;
	int indexColumn = pColumn->GetIndex();

	//変更のあった行.
	ICSReportRow* pRow = pItemNotify->pRow;
	int indexRow = pRow->GetIndex();

	//変更のあったアイテム.
	ICSReportRecordItem* pItem = pItemNotify->pItem;
	int indexItem = pItem->GetIndex();

	if (indexColumn == COLUMN_DATE) {

		CString str = pItem->GetCaptionString(NULL);
		int tmp_ymd = _ttoi(str);
		int year, month, day, offset;
		int s_gengo = 0, d_gengo = 0, d_year = 0;

		m_Voldate.db_datecnvGen(s_gengo, tmp_ymd, &d_gengo, &d_year, 1, 0);
		year = d_year / 10000;
		month = (d_year % 10000) / 100;
		day = (d_year % 100);

		offset = m_pMngTables->GetOffsetDay(year, month, day);
		if ((offset < DAYSOFFSET_SUNDAY) || (offset > DAYSOFFSET_SATURDAY) || (str.GetLength() < DIGITS_DATE)) {
			m_ReportK.SetCaption(indexRow, indexColumn, _T(""));
			m_ReportK.Populate();
			return;
		}

	}

	//ターミネーションキーコード
	UINT nChar = pItemNotify->nChar;

	switch (nChar) {
	case VK_RIGHT:
		EditRightItemReport(&m_ReportK, indexRow, indexColumn);
		break;

	case VK_RETURN:
	case VK_TAB:
		if (GetKeyState(VK_SHIFT) & 0x8000) {
			EditLeftItemReport(&m_ReportK, indexRow, indexColumn);
		}
		else {
			EditRightItemReport(&m_ReportK, indexRow, indexColumn);
		}
		break;

	case VK_LEFT:
		EditLeftItemReport(&m_ReportK, indexRow, indexColumn);
		break;

	default:
		break;
	}
}

