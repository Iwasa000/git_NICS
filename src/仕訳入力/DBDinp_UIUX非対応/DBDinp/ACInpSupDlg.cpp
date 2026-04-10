// ACInpSupDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBDinp.h"
#include "ACInpSupDlg.h"

#include "mstrw.h"

// CACInpSupDlg ダイアログ

IMPLEMENT_DYNAMIC(CACInpSupDlg, ICSDialog)

CACInpSupDlg::CACInpSupDlg(CWnd* pParent /*=NULL*/)
	: ICSDialog(CACInpSupDlg::IDD, pParent)
{
	m_ValFix = -1;
}

CACInpSupDlg::~CACInpSupDlg()
{
}

void CACInpSupDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDOK, m_OK);
	DDX_Radio(pDX, IDC_RD_NOFIX, m_ValFix);
}


BEGIN_MESSAGE_MAP(CACInpSupDlg, ICSDialog)
	ON_BN_CLICKED(IDC_CHK_VALCOPY, &CACInpSupDlg::OnBnClickedChkValcopy)
	ON_BN_CLICKED(IDC_CHK_VALNOT, &CACInpSupDlg::OnBnClickedChkValnot)
END_MESSAGE_MAP()


// CACInpSupDlg メッセージ ハンドラ

BOOL CACInpSupDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	int valsgn = ValEnterCopyMode();

	if( ! valsgn ) {
		((CButton*)GetDlgItem(IDC_CHK_VALCOPY))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(IDC_CHK_VALNOT ))->SetCheck(BST_UNCHECKED);
	}
	else {
		((CButton*)GetDlgItem(IDC_CHK_VALCOPY))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_CHK_VALNOT ))->SetCheck(BST_CHECKED);
	}
	ICSDialog::OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


BOOL CACInpSupDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( pMsg->message == WM_KEYDOWN ) {
		CWnd* pwnd;
		pwnd = GetFocus();
		if( pwnd ) {
			int id = pwnd->GetDlgCtrlID();
		
			switch( id ) {
			case IDC_RD_NOFIX:		case IDC_RD_NKINFIX: case IDC_RD_SKINFIX:

				switch( pMsg->wParam ) {
				case VK_RETURN : case VK_DOWN :
					NextDlgCtrl();	return TRUE;
				case VK_UP : 
					PrevDlgCtrl();	return TRUE;
				}
				break;

			case IDC_CHK_VALCOPY:	case IDC_CHK_VALNOT:
				if( pMsg->wParam == VK_RETURN ) {
					GotoDlgCtrl( &m_OK ); return TRUE;
				}
				else if( pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN ) {
					if( GetDlgItem(IDC_CHK_VALCOPY) == pwnd )
						GotoDlgCtrl( GetDlgItem(IDC_CHK_VALNOT) );
					else
						GotoDlgCtrl( GetDlgItem(IDC_CHK_VALCOPY) );
					return TRUE;
				}
				break;
			}
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}


void CACInpSupDlg::OnBnClickedChkValcopy()
{
	ValCheckCtrl(IDC_CHK_VALCOPY);
}

void CACInpSupDlg::OnBnClickedChkValnot()
{
	ValCheckCtrl(IDC_CHK_VALNOT);
}


// 金額チェックボックスコントロール
void CACInpSupDlg::ValCheckCtrl( UINT ID )
{
	if( ID == IDC_CHK_VALCOPY ) {
		((CButton*)GetDlgItem(IDC_CHK_VALCOPY))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(IDC_CHK_VALNOT ))->SetCheck(BST_UNCHECKED);
	}
	else if( ID == IDC_CHK_VALNOT ) {
		((CButton*)GetDlgItem(IDC_CHK_VALCOPY))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_CHK_VALNOT ))->SetCheck(BST_CHECKED);
	}
}

void CACInpSupDlg::OnOK()
{
	// 09.18 /12
	int chk = ((CButton*)GetDlgItem(IDC_CHK_VALCOPY))->GetCheck();
	ValEnterCopyMode() = (chk == BST_CHECKED) ? 0 : 1;

	ICSDialog::OnOK();
}
