// ShinMinfoCheck.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBSyzShinMain.h"
#include "ShinMinfoCheck.h"

// CShinMinfoCheck ダイアログ

IMPLEMENT_DYNAMIC(CShinMinfoCheck, ICSDialog)

CShinMinfoCheck::CShinMinfoCheck(CWnd* pParent /*=NULL*/)
	: ICSDialog(CShinMinfoCheck::IDD, pParent)
{

}

CShinMinfoCheck::~CShinMinfoCheck()
{
}

void CShinMinfoCheck::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CShinMinfoCheck, ICSDialog)
	ON_BN_CLICKED(IDOK, &CShinMinfoCheck::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CShinMinfoCheck::OnBnClickedCancel)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CShinMinfoCheck メッセージ ハンドラ

void CShinMinfoCheck::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	OnOK();
}

void CShinMinfoCheck::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	OnCancel();
}

BOOL CShinMinfoCheck::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	ICSDialog::OnInitDialogEX();
	// TODO:  ここに初期化を追加してください
	SetWindowText(AfxGetAppName());
	
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	SetClassLongPtr(m_hWnd, GCL_STYLE, GetClassLongPtr(m_hWnd, GCL_STYLE) & ~CS_NOCLOSE );
	SetClassLongPtr(m_hWnd, GCL_STYLE, GetClassLongPtr(m_hWnd, GCL_STYLE) | CS_NOCLOSE);

//	HINSTANCE hDll = LoadLibrary( "SHELL32.dll" );
//	hIcon   = LoadIcon(hDll , MAKEINTRESOURCE( 1 ));
//	hIcon = LoadIcon(NULL,MAKEINTRESOURCE(IDI_WARNING));
	hIcon = LoadIcon(NULL, IDI_WARNING);

	return FALSE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

BOOL CShinMinfoCheck::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( pMsg->message == WM_KEYDOWN )	{
		if (pMsg->wParam == VK_HOME){
			if( !GetDlgItem(IDOK)->IsWindowEnabled() ) {
				GetDlgItem(IDOK)->EnableWindow();
				return TRUE;
			}
		}
	}
	return ICSDialog::PreTranslateMessage(pMsg);
}

void CShinMinfoCheck::OnPaint()
{
	CClientDC    dc(this);
    dc.DrawIcon( GetSystemMetrics(SM_CXHTHUMB), GetSystemMetrics(SM_CXHTHUMB), hIcon);
	ICSDialog::OnPaint();
}
