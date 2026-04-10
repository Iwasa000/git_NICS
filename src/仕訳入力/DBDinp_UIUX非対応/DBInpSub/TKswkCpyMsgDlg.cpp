// TKswkCpyMsgDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "resource.h"

#include "TKswkCpyMsgDlg.h"


// CTKswkCpyMsgDlg ダイアログ

IMPLEMENT_DYNAMIC(CTKswkCpyMsgDlg, ICSDialog)

CTKswkCpyMsgDlg::CTKswkCpyMsgDlg(CWnd* pParent /*=NULL*/)
	: ICSDialog(CTKswkCpyMsgDlg::IDD, pParent)
	, m_Radio(FALSE)
{
	m_Mode = -1;
}

CTKswkCpyMsgDlg::~CTKswkCpyMsgDlg()
{
}

void CTKswkCpyMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_Radio);
}


BEGIN_MESSAGE_MAP(CTKswkCpyMsgDlg, ICSDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CTKswkCpyMsgDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTKswkCpyMsgDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CTKswkCpyMsgDlg メッセージ ハンドラ

BOOL CTKswkCpyMsgDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください


	ICSDialog::OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


// 一覧選択
void CTKswkCpyMsgDlg::OnBnClickedButton1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	UpdateData();
	m_Mode = 0;
	EndDialog(IDOK);
}


// 上段複写
void CTKswkCpyMsgDlg::OnBnClickedButton2()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	UpdateData();
	m_Mode = 1;
	EndDialog(IDOK);
}

BOOL CTKswkCpyMsgDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	if( pMsg->message == WM_KEYDOWN ) {
		CWnd* pwnd;
		pwnd = GetFocus();
		if( pwnd ) {
			int id = pwnd->GetDlgCtrlID();
		
			switch( id ) {
			case IDC_RADIO1:	case IDC_RADIO2:

				switch( pMsg->wParam ) {
				case VK_RETURN : case VK_RIGHT :
					NextDlgCtrl();	return TRUE;
				case VK_LEFT : 
					PrevDlgCtrl();	return TRUE;
				}
				break;
			}
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}
