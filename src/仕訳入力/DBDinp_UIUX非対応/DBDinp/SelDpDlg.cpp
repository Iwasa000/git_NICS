// SelDpDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBDinp.h"
#include "SelDpDlg.h"


// SelDpDlg ダイアログ

IMPLEMENT_DYNAMIC(SelDpDlg, ICSDialog)

SelDpDlg::SelDpDlg(CWnd* pParent /*=NULL*/)
	: ICSDialog(SelDpDlg::IDD, pParent)
{

}

SelDpDlg::~SelDpDlg()
{
}

void SelDpDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_btn1);
	DDX_Control(pDX, IDC_BUTTON2, m_btn2);
	DDX_Control(pDX, IDC_BUTTON3, m_btn3);
}


BEGIN_MESSAGE_MAP(SelDpDlg, ICSDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &SelDpDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &SelDpDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &SelDpDlg::OnBnClickedButton3)
END_MESSAGE_MAP()



BOOL SelDpDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	m_btn1.SetBitmap( IDB_FURIBITMAP, 100, 50 );
	m_btn2.SetBitmap( IDB_NKINBITMAP, 100, 50 );
	m_btn3.SetBitmap( IDB_SKINBITMAP, 100, 50 );

	m_btn1.SetWindowText( _T("　　　 振替伝票...F4") );
	m_btn2.SetWindowText( _T("　　　 入金伝票...F5") );
	m_btn3.SetWindowText( _T("　　　 出金伝票...F6") );

	ICSDialog::OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


// SelDpDlg メッセージ ハンドラ

void SelDpDlg::OnBnClickedButton1()
{
	EndDialog( 101 );
}

void SelDpDlg::OnBnClickedButton2()
{
	EndDialog( 102 );
}

void SelDpDlg::OnBnClickedButton3()
{
	EndDialog( 103 );
}


BOOL SelDpDlg::PreTranslateMessage(MSG* pMsg)
{
	switch( pMsg->message ) {
		case	WM_KEYDOWN:
			switch( pMsg->wParam ) {
				case	VK_F4:	OnBnClickedButton1();	return	1L;
					break;
				case	VK_F5:	OnBnClickedButton2();	return	1L;
					break;
				case	VK_F6:	OnBnClickedButton3();	return	1L;
					break;
			}
			break;
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}


void SelDpDlg::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	CWnd	*pWnd	=	GetFocus();
	if( pWnd ) {
		if( pWnd->GetSafeHwnd() == m_btn1.m_hWnd )	EndDialog( 101 );
		else if( pWnd->GetSafeHwnd() == m_btn2.m_hWnd )	EndDialog( 102 );
		else if( pWnd->GetSafeHwnd() == m_btn3.m_hWnd )	EndDialog( 103 );
	}
}
