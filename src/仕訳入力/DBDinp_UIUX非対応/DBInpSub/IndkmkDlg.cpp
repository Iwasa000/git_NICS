// IndkmkDlg.cpp : 実装ファイル
//

#include "stdafx.h"

#include "resource.h"
#include "IndkmkDlg.h"


// CIndkmkDlg ダイアログ

IMPLEMENT_DYNAMIC(CIndkmkDlg, ICSDialog)

CIndkmkDlg::CIndkmkDlg(CWnd* pParent /*=NULL*/)
	: ICSDialog(CIndkmkDlg::IDD, pParent)
	, m_chkIppan(FALSE)
	, m_chkHudo(FALSE)
	, m_chkNogyo(FALSE)
	, m_chkAll(FALSE)
{

}

CIndkmkDlg::~CIndkmkDlg()
{
}

void CIndkmkDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_chkIppan);
	DDX_Check(pDX, IDC_CHECK2, m_chkHudo);
	DDX_Check(pDX, IDC_CHECK3, m_chkNogyo);
	DDX_Check(pDX, IDC_CHECK4, m_chkAll);
/*
	DDX_Control(pDX, IDC_CHECK1, m_Button[0]);
	DDX_Control(pDX, IDC_CHECK2, m_Button[1]);
	DDX_Control(pDX, IDC_CHECK3, m_Button[2]);
	DDX_Control(pDX, IDC_CHECK4, m_Button[3]);
*/
}


BEGIN_MESSAGE_MAP(CIndkmkDlg, ICSDialog)
	ON_BN_CLICKED(IDC_CHECK1, &CIndkmkDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CIndkmkDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CIndkmkDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CIndkmkDlg::OnBnClickedCheck4)
END_MESSAGE_MAP()


// CIndkmkDlg メッセージ ハンドラ

BOOL CIndkmkDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	int type = IsKojinGyosyuMaster( m_pZm );

	if( ! (type & 0x01) ) {
		GetDlgItem( IDC_CHECK1 )->EnableWindow( FALSE );
	}

	if( ! (type & 0x02) ) {
		GetDlgItem( IDC_CHECK2 )->EnableWindow( FALSE );
	}

	if( ! (type & 0x04) ) {
		GetDlgItem( IDC_CHECK3 )->EnableWindow( FALSE );
	}

	ICSDialog::OnInitDialogEX();
/*
	m_Button[0].SetCheck( m_chkIppan ? 1 : 0 );
	m_Button[1].SetCheck( m_chkHudo ? 1 : 0 );
	m_Button[2].SetCheck( m_chkNogyo ? 1 : 0 );
	m_Button[3].SetCheck( m_chkAll ? 1 : 0 );
*/
	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

BOOL CIndkmkDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( pMsg->message == WM_KEYDOWN ) {
		int id = ::GetDlgCtrlID( pMsg->hwnd );

		switch( id ) {
		case IDC_CHECK1:
		case IDC_CHECK2:
		case IDC_CHECK3:
		case IDC_CHECK4:
			if( pMsg->wParam == VK_RETURN ) {
				pMsg->wParam = VK_TAB;
			}
			break;
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}


void CIndkmkDlg::OnBnClickedCheck1()
{
	ctrl_set( IDC_CHECK1 );
}

void CIndkmkDlg::OnBnClickedCheck2()
{
	ctrl_set( IDC_CHECK2 );
}

void CIndkmkDlg::OnBnClickedCheck3()
{
	ctrl_set( IDC_CHECK3 );
}

void CIndkmkDlg::OnBnClickedCheck4()
{
	ctrl_set( IDC_CHECK4 );
}

// ラジオボタンのような動作
void CIndkmkDlg::ctrl_set( UINT id )
{
	CButton* pBtn;

	pBtn = (CButton*)GetDlgItem( id );

	int st;
	st = pBtn->GetCheck();
	
	if( st ) {
		int nID[] = {
			IDC_CHECK1,	IDC_CHECK2,	IDC_CHECK3,	IDC_CHECK4, 0
		};

		for( int i = 0; nID[i] != 0; i++ ) {
			if( nID[i] == id )	continue;

			((CButton*)GetDlgItem( nID[i] ))->SetCheck(0);
		}
	}
	else {
		pBtn->SetCheck(1);
	}
}