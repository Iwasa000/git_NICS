// CDupliDlg.cpp : 実装ファイル
//
#include "stdafx.h"
#include "DBDinp.h"
#include "CDupliDlg.h"


// CDupliDlg ダイアログ

IMPLEMENT_DYNAMIC(CDupliDlg, ICSDialog)

CDupliDlg::CDupliDlg(CWnd* pParent /*=nullptr*/)
	: ICSDialog(IDD_DLG_DUPLI, pParent)
{
	koji_mast = bmon_mast = 0;
	m_dwChkBit = 0;
}

CDupliDlg::~CDupliDlg()
{
}

void CDupliDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDupliDlg, ICSDialog)
END_MESSAGE_MAP()


// CDupliDlg メッセージ ハンドラー
BOOL CDupliDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	CButton* pb;

	pb = (CButton*)GetDlgItem(IDC_CHK_CNO);
	// TODO: ここに初期化を追加してください
	if( m_dwChkBit & DS_DENP_BIT ) {
		pb->SetCheck(BST_CHECKED);
	}

	pb = (CButton*)GetDlgItem(IDC_CHK_BMN);
	if( bmon_mast ) {
		if( m_dwChkBit & DS_BMN_BIT ) {
			pb->SetCheck(BST_CHECKED);
		}
	}
	else {
		pb->EnableWindow(FALSE);
	}
	pb = (CButton*)GetDlgItem(IDC_CHK_KOJI);
	if( koji_mast ) {
		if( m_dwChkBit & DS_KOJI_BIT ) {
			pb->SetCheck(BST_CHECKED);
		}
	}
	else {
		pb->EnableWindow(FALSE);
	}

	pb = (CButton*)GetDlgItem(IDC_CHK_TKY);
	if( m_dwChkBit & DS_TKY_BIT ) {
		pb->SetCheck(BST_CHECKED);
	}

	ICSDialog::OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}




void CDupliDlg::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	CButton* pb = (CButton*)GetDlgItem(IDC_CHK_CNO);
	// TODO: ここに初期化を追加してください
	int st = pb->GetCheck();
	if( st == BST_CHECKED ) {
		m_dwChkBit |= DS_DENP_BIT;
	}
	else{
		m_dwChkBit &= ~DS_DENP_BIT;
	}

	pb = (CButton*)GetDlgItem(IDC_CHK_BMN);
	if( bmon_mast ) {
		st = pb->GetCheck();
		if( st == BST_CHECKED ) {
			m_dwChkBit |= DS_BMN_BIT;
		}
		else {
			m_dwChkBit &= ~DS_BMN_BIT;
		}
	}
	else {
		m_dwChkBit &= ~DS_BMN_BIT;
	}
	pb = (CButton*)GetDlgItem(IDC_CHK_KOJI);
	if( koji_mast ) {
		st = pb->GetCheck();
		if( st == BST_CHECKED ) {
			m_dwChkBit |= DS_KOJI_BIT;
		}
		else {
			m_dwChkBit &= ~DS_KOJI_BIT;
		}
	}
	else {
		m_dwChkBit &= ~DS_KOJI_BIT;
	}

	pb = (CButton*)GetDlgItem(IDC_CHK_TKY);
	st = pb->GetCheck();
	if( st == BST_CHECKED ) {
		m_dwChkBit |= DS_TKY_BIT;
	}
	else {
		m_dwChkBit &= ~DS_TKY_BIT;
	}

	ICSDialog::OnOK();
}


BOOL CDupliDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	if( pMsg->message == WM_KEYDOWN ) {
		if( pMsg->wParam == VK_RETURN ) {
			HWND h1 = GetDlgItem(IDC_CHK_CNO)->m_hWnd;
			HWND h2 = GetDlgItem(IDC_CHK_BMN)->m_hWnd;
			HWND h3 = GetDlgItem(IDC_CHK_KOJI)->m_hWnd;
			HWND h4 = GetDlgItem(IDC_CHK_TKY)->m_hWnd;

			if( pMsg->hwnd == h1 || pMsg->hwnd == h2 || pMsg->hwnd == h3 || pMsg->hwnd == h4 ) {
				NextDlgCtrl();
				return TRUE;
			}
		}
		else if( pMsg->wParam == 'Y' || pMsg->wParam == 'y' ) {
			PostMessage(WM_COMMAND, MAKELONG(IDOK, BN_CLICKED),	(LPARAM)GetDlgItem(IDOK)->m_hWnd);
			return TRUE;
		}
		else if( pMsg->wParam == 'N' || pMsg->wParam == 'n' ) {
			PostMessage(WM_COMMAND, MAKELONG(IDCANCEL, BN_CLICKED), (LPARAM)GetDlgItem(IDCANCEL)->m_hWnd);
			return TRUE;
		}

	}

	return ICSDialog::PreTranslateMessage(pMsg);
}
