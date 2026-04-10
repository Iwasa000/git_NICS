// CNPautoDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBDinp.h"
#include "CNPautoDlg.h"


extern CDipZmSub* pDBzm;


// CNPautoDlg ダイアログ
IMPLEMENT_DYNAMIC(CNPautoDlg, ICSDialog)

CNPautoDlg::CNPautoDlg(CWnd* pParent /*=nullptr*/)
	: ICSDialog(IDD_DLG_NPAUTO, pParent)
{
	// 09.03 /03
	m_brBack.CreateSolidBrush(RGB_DARK_GREEN);
	m_brWhite.CreateSolidBrush(RGB_WHITE);

	ICSColorInfo info;
	((CDBDinpApp*)AfxGetApp())->GetStanderdColor(info);
	m_TitleBackCol = info.TitleColor();
	m_TitleTextCol = info.CaptionTextColor();
	m_brTitle.CreateSolidBrush(m_TitleBackCol);

	m_SelIndex = -1;

	line_colordsp(-1, FALSE, TRUE);
}

CNPautoDlg::~CNPautoDlg()
{
}

void CNPautoDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSLISTINPCTRL1, m_list);
}


BEGIN_MESSAGE_MAP(CNPautoDlg, ICSDialog)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CNPautoDlg メッセージ ハンドラー
BOOL CNPautoDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO: ここに初期化を追加してください
	Makelistinp();
	NPautoLinpSetSub();

	NPsikinDataSetSub();

//	line_colordsp(0, TRUE);

	ICSDialog::OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


//	仕訳登録部分
//	表形式入力　設定
void CNPautoDlg::Makelistinp()
{
	LINP_COLUMN LC[4];
	LINP_VCOLORINFO	LVCOL[4];

	//番号
	LC[ACLM_NO].clm_kind = LINP_KIND_STRING;
	LC[ACLM_NO].clm_max = 5;
	LC[ACLM_NO].clm_attr = LINP_ATTR_READONLY | LINP_ATTR_CENTER | LINP_ATTR_BOTTOM | LINP_ATTR_READONLY;
	LC[ACLM_NO].clm_width = 30;
	LC[ACLM_NO].clm_title = _T("No.");
	LC[ACLM_NO].clm_tbcolor = RGB_DARK_GREEN;
	LC[ACLM_NO].clm_ttcolor = RGB_WHITE;

	LC[ACLM_DEBT].clm_kind = /*LINP_KIND_KAMOKU*/LINP_KIND_STRING;
	LC[ACLM_DEBT].clm_max = 14;
	LC[ACLM_DEBT].clm_attr = LINP_ATTR_CENTER | LINP_ATTR_BOTTOM | LINP_ATTR_ANK | LINP_ATTR_READONLY;

	LC[ACLM_DEBT].clm_width = 105;
	LC[ACLM_DEBT].clm_title = "借 方 科 目";
	LC[ACLM_DEBT].clm_tbcolor = RGB_DARK_GREEN;
	LC[ACLM_DEBT].clm_ttcolor = RGB_WHITE;

	LC[ACLM_CRED].clm_kind = /*LINP_KIND_KAMOKU*/LINP_KIND_STRING;
	LC[ACLM_CRED].clm_max = 14;
	LC[ACLM_CRED].clm_attr = LINP_ATTR_CENTER | LINP_ATTR_BOTTOM | LINP_ATTR_ANK |  LINP_ATTR_READONLY;
	LC[ACLM_CRED].clm_width = 105;
	LC[ACLM_CRED].clm_title = "貸 方 科 目";
	LC[ACLM_CRED].clm_tbcolor = RGB_DARK_GREEN;
	LC[ACLM_CRED].clm_ttcolor = RGB_WHITE;

	linp_setclm(IDC_ICSLISTINPCTRL1, 3, LC, CICSListInp);

	// 表形式入力　グリッド線の設定	
	linp_horzgrid(IDC_ICSLISTINPCTRL1, 5, RGB_RED, LINP_GRID_SOLID, CICSListInp);

	LVCOL[0].pos = 0;
	LVCOL[0].color = RGB_DARK_GREEN;
	LVCOL[0].type = LINP_GRID_SOLID;

	LVCOL[1].pos = 1;
	LVCOL[1].color = RGB_GRAY;
	LVCOL[1].type = LINP_GRID_SOLID;

	linp_vartgrid(IDC_ICSLISTINPCTRL1, 2, (LPUNKNOWN)LVCOL, CICSListInp);
}


//-------------------------------------------------------
//	自動仕訳をリストにセット
//
//-------------------------------------------------------
void CNPautoDlg::NPautoLinpSetSub()
{
	DBKNREC*	pKn;

	LINP_DATA	LD[10];
	LPUNKNOWN	lp;
	lp = (LPUNKNOWN)&LD[0];

	CString buf;

	int max, lcnt, idx;
	max = lcnt = 0;
	max = pNpsame->GetSize();
	lcnt = max;

	m_list.RemoveAll();
	m_list.RedrawControl();

	for (idx = 0; idx < max; idx++) {

		NPAUTO_SWK* pswk = &(*pNpsame)[idx];

		// 番号
		LD[ACLM_NO].dat_string.Format("%d", idx + 1);
		LD[ACLM_NO].dat_attr = LINP_DISP_COLOR;
		LD[ACLM_NO].dat_fc = RGB_BLACK;
		LD[ACLM_NO].dat_bc = RGB_WHITE;
		LD[ACLM_NO].dat_initflg = FALSE;

		for (int n = ACLM_DEBT; n <= ACLM_CRED; n++) {
			// 借・貸 科目
			LD[n].dat_string.Empty();
			LD[n].dat_attr = LINP_DISP_COLOR;
			LD[n].dat_fc = RGB_BLACK;
			LD[n].dat_bc = RGB_WHITE;

			DWORD dwCode = (n == ACLM_DEBT) ? pswk->autDeb : pswk->autCre;
			CString code;
			code.Format("%08x", dwCode);

			if (!dwCode)
				LD[n].dat_initflg = TRUE;
			else {
				LD[n].dat_initflg = FALSE;

				pKn = pDBzm->DB_PjisToKnrec(code);

				if (pKn) {
					LD[n].dat_string.Format("%.14s", pKn->knnam);
					LD[n].dat_kamoku.kamoku_code = 0;
					LD[n].dat_kamoku.kamoku_edaban = 0xffff;
					LD[n].dat_kamoku.kamoku_num = 0;
				}
				else LD[n].dat_initflg = TRUE;
			}
		}

		m_list.SetData(idx, (LPUNKNOWN)LD, TRUE);

	}
}

//-------------------------------------------------------
//	資金仕訳をセット
//
//-------------------------------------------------------
void CNPautoDlg::NPsikinDataSetSub()
{
	DBKNREC*	pKn;

	for (int n = ACLM_DEBT; n <= ACLM_CRED; n++) {
		// 借・貸 科目
		CString code;
		code = (n == ACLM_DEBT) ? pCrec->m_dbt : pCrec->m_cre;

		if (code.IsEmpty()) {
			pKn = NULL;
		}
		else {
			pKn = pDBzm->DB_PjisToKnrec(code);
		}
		CString str;
		if (pKn) {
			str.Format("%.14s", pKn->knnam);
		}
		else{
			str.Empty();
		}
		UINT nid = (n == ACLM_DEBT) ? IDC_ST_SIKINDBT2 : IDC_ST_SIKINCRE2;
		GetDlgItem(nid)->SetWindowText(str);
	}
}


HBRUSH CNPautoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = ICSDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: この位置で DC のアトリビュートを変更してください

	if (nCtlColor == CTLCOLOR_STATIC) {
		// 09.03 /03
		if (pWnd) {
			int nID = pWnd->GetDlgCtrlID();

			if (nID == IDC_ST_AUTO) {
				pDC->SetTextColor(RGB_WHITE);
				hbr = (HBRUSH)m_brBack;
			}
			else if (nID == IDC_ST_SIKIN || nID == IDC_ST_SIKINDBT || nID == IDC_ST_SIKINCRE ) {
				pDC->SetTextColor(m_TitleTextCol);
				hbr = (HBRUSH)m_brTitle;
			}
			else if (nID == IDC_ST_SIKINDBT2 || nID == IDC_ST_SIKINCRE2) {
				hbr = (HBRUSH)m_brWhite;
			}
		}
	}

	// TODO: デフォルトのブラシが望みのものでない場合には、違うブラシを返してください
	return hbr;
}


BEGIN_EVENTSINK_MAP(CNPautoDlg, ICSDialog)
	//{{AFX_EVENTSINK_MAP(CSelImgDlg)
	ON_EVENT(CNPautoDlg, IDC_ICSLISTINPCTRL1, 4 /* KeyIn */, OnKeyInIcslistinpctrl1, VTS_I2 VTS_I2 VTS_I2)
	ON_EVENT(CNPautoDlg, IDC_ICSLISTINPCTRL1, 7 /* LButtonDblClick */, OnLButtonDblClickIcslistinpctrl1, VTS_I2)
	ON_EVENT(CNPautoDlg, IDC_ICSLISTINPCTRL1, 9 /* Focused */, OnFocusedIcslistinpctrl1, VTS_NONE)
	ON_EVENT(CNPautoDlg, IDC_ICSLISTINPCTRL1, 10, CNPautoDlg::OnSelchangeIcslistinpctrl1, VTS_I2)
	//}}AFX_EVENTSINK_MAP
	ON_EVENT(CNPautoDlg, IDC_ICSLISTINPCTRL1, 6, CNPautoDlg::OnLbuttonclickIcslistinpctrl1, VTS_I2)
END_EVENTSINK_MAP()


void CNPautoDlg::OnKeyInIcslistinpctrl1(short nChar, short previndex, short index) 
{
	HWND hwnd = GetDlgItem(IDC_ICSLISTINPCTRL1)->m_hWnd;
	if (::IsWindow(hwnd) == FALSE)
		return;

	if (nChar == VK_RETURN)
	{
		GotoDlgCtrl( GetDlgItem(IDOK));
		return;
	}
	else if (nChar == VK_ESCAPE) {
		PostMessage(WM_COMMAND, MAKELONG(IDCANCEL, BN_CLICKED),
			(LPARAM)GetDlgItem(IDCANCEL)->m_hWnd);
		return;
	}
	else if (nChar == VK_UP || nChar == VK_DOWN ) {
		//int top = m_list.GetTopIndex();
		//int idx = m_list.GetIndex();
		//int nowidx = top + idx;
		//line_colordsp(nowidx, TRUE);
	}

}

void CNPautoDlg::OnLButtonDblClickIcslistinpctrl1(short index)
{
	HWND hwnd = GetDlgItem(IDC_ICSLISTINPCTRL1)->m_hWnd;
	if (::IsWindow(hwnd) == FALSE)
		return;

	PostMessage(WM_COMMAND, MAKELONG(IDOK, BN_CLICKED), (LPARAM)GetDlgItem(IDOK)->m_hWnd);
}

void CNPautoDlg::OnFocusedIcslistinpctrl1()
{
	int top = m_list.GetTopIndex();
	int idx = m_list.GetIndex();

	line_colordsp(top + idx, FALSE);
}

void CNPautoDlg::OnSelchangeIcslistinpctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
//	line_colordsp(index, TRUE);
}

void CNPautoDlg::OnLbuttonclickIcslistinpctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
//	line_colordsp(index, TRUE);
}

void CNPautoDlg::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	int top = m_list.GetTopIndex();
	int idx = m_list.GetIndex();

	m_SelIndex = top + idx;

	ICSDialog::OnOK();
}


// 選択された行の表示

void CNPautoDlg::line_colordsp(int idx, BOOL sgn, BOOL initflg /* = FALSE*/)
{
	static int prev_idx;

//	MyTrace("line_colordsp top idx = %d, sgn = %d\n", idx, sgn);

	if (initflg) {
		prev_idx = -1;	return;
	}
	LINP_DATA	ldata[10];

	if (prev_idx != -1 && prev_idx != idx) {
		// 選択行の 色のOFF

		m_list.GetData(prev_idx, (LPUNKNOWN)ldata);

		for (int pn = ACLM_NO; pn <= ACLM_CRED; pn++) {
			ldata[pn].dat_attr = 0;
			ldata[pn].dat_fc = 0;
			ldata[pn].dat_bc = 0;
			ldata[pn].dat_initflg = FALSE;
		}
		m_list.SetData(prev_idx, (LPUNKNOWN)ldata, TRUE);
	}

	prev_idx = idx;

	// 選択されている行の色を表示
	if (sgn) {
		m_list.GetData(idx, (LPUNKNOWN)ldata);

		for (int pn = ACLM_NO; pn <= ACLM_CRED; pn++) {
			ldata[pn].dat_attr = LINP_DISP_COLOR;
			ldata[pn].dat_fc = RGB_BLACK;
		//	ldata[pn].dat_bc = SELECT_BACK_COL;
			ldata[pn].dat_bc = RGB_YELLOW1;
			ldata[pn].dat_initflg = FALSE;
		}
		m_list.SetData(idx, (LPUNKNOWN)ldata, TRUE);
	}
}



BOOL CNPautoDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	if (pMsg->hwnd == m_list.m_hWnd) {
//		MyTrace("pMsg->message = %08x\n", pMsg->message);
		if (pMsg->message == WM_NCMOUSELEAVE || pMsg->message == WM_NCMOUSEMOVE || pMsg->message == WM_MOUSEMOVE) {
			int top = m_list.GetTopIndex();
			int idx = m_list.GetIndex();
//MyTrace("pMsg->message(WM_VSCROLL), top = %d, idx = %d\n", top, idx);
			line_colordsp(top + idx, FALSE);
		}
	}
	CWnd* pwnd = GetFocus();
	if (pwnd != NULL && pwnd->m_hWnd != m_list.m_hWnd) {
		int top = m_list.GetTopIndex();
		int idx = m_list.GetIndex();
//MyTrace("pMsg->message( focus がm_listでない), top = %d, idx = %d\n", top, idx);
		line_colordsp(top + idx, TRUE);
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}
