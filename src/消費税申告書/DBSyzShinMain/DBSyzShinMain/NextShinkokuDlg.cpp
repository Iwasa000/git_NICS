// NextShinkokuDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBSyzShinMain.h"
#include "NextShinkokuDlg.h"


// CNextShinkokuDlg ダイアログ

IMPLEMENT_DYNAMIC(CNextShinkokuDlg, ICSDialog)

CNextShinkokuDlg::CNextShinkokuDlg(CString m,int f,CWnd* pParent /*=NULL*/)
	: ICSDialog(CNextShinkokuDlg::IDD, pParent) , flg(f), mes(m)
{

}

CNextShinkokuDlg::~CNextShinkokuDlg()
{
}

void CNextShinkokuDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNextShinkokuDlg, ICSDialog)
	ON_BN_CLICKED(IDOK, &CNextShinkokuDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CNextShinkokuDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_INVALID, &CNextShinkokuDlg::OnBnClickedInvalid)
END_MESSAGE_MAP()


// CNextShinkokuDlg メッセージ ハンドラ
BOOL CNextShinkokuDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	ICSDialog::OnInitDialogEX();
	
	SetWindowText(AfxGetAppName());
	GetDlgItem(IDC_STATIC)->SetWindowText(mes);
	
	GetDlgItem(IDOK)->EnableWindow(flg&1);
	GetDlgItem(IDCANCEL)->EnableWindow(flg&2);

	GetDlgItem(IDC_INVALID)->EnableWindow(TRUE);

	SetClassLongPtr(m_hWnd, GCL_STYLE, GetClassLongPtr(m_hWnd, GCL_STYLE) & ~CS_NOCLOSE );
	SetClassLongPtr(m_hWnd, GCL_STYLE, GetClassLongPtr(m_hWnd, GCL_STYLE) | CS_NOCLOSE);

	if( flg )	GotoDlgCtrl( (flg&1) ? GetDlgItem(IDOK) : GetDlgItem(IDCANCEL) );
	else		GotoDlgCtrl( GetDlgItem(IDC_INVALID) );

	return FALSE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CNextShinkokuDlg::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	EndDialog(IDYES);
}

void CNextShinkokuDlg::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	EndDialog(IDNO);
}

void CNextShinkokuDlg::OnBnClickedInvalid()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	EndDialog(IDINVALID_ICS);
}

void CNextShinkokuDlg::OnOK()
{
}

void CNextShinkokuDlg::OnCancel()
{
}

BOOL CNextShinkokuDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( pMsg->message == WM_KEYDOWN ) {
		if( pMsg->wParam == VK_ESCAPE ) return TRUE;
	}
	return ICSDialog::PreTranslateMessage(pMsg);
}
