// RevTest8Dlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "RevTest8.h"
#include "RevTest8Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CRevTest8Dlg ダイアログ

CRevConv	CRevTest8Dlg::m_RV;
CRevConv	CRevTest8Dlg::m_RV2;

CRevTest8Dlg::CRevTest8Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRevTest8Dlg::IDD, pParent)
	, m_HanChk(FALSE)
	, m_KanaChk(FALSE)
{
	m_Edit = _T("逆変換の結果を表示します。");
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRevTest8Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Edit);
	DDX_Check(pDX, IDC_CHECK1, m_HanChk);
	DDX_Check(pDX, IDC_CHECK2, m_KanaChk);
}

BEGIN_MESSAGE_MAP(CRevTest8Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CRevTest8Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CRevTest8Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CRevTest8Dlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CRevTest8Dlg メッセージ ハンドラ

BOOL CRevTest8Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CRevTest8Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CRevTest8Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CRevTest8Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CRevTest8Dlg::OnBnClickedButton1()
{
	UpdateData();
	CString kana;

	for( int i = 0; i < 2500; i++ ) {
		DWORD	dwOpt = 0;
		if( m_HanChk )		dwOpt |= RVF_HANKAKU;
		if( m_KanaChk )	dwOpt |= RVF_KATAKANA;

		int ret = m_RV.GetFurigana( dwOpt, m_Edit, kana );
		GetDlgItem( IDC_STATIC1 )->SetWindowText( kana );

		CString msg;
		msg.Format( "(i=%d) return = %d", i, ret );
		GetDlgItem( IDC_STATIC3 )->SetWindowText( msg );
	}
}

void CRevTest8Dlg::OnBnClickedButton2()
{
	UpdateData();	
	CString kana;

	CRevConv	rv;

	for( int i = 0; i < 2500; i++ ) {
		CRevConv	rv;
		DWORD	dwOpt = 0;
		if( m_HanChk )		dwOpt |= RVF_HANKAKU;
		if( m_KanaChk )	dwOpt |= RVF_KATAKANA;

		int ret = rv.GetFurigana( dwOpt, m_Edit, kana );
		GetDlgItem( IDC_STATIC2 )->SetWindowText( kana );

		CString msg;
		msg.Format( "(i=%d) return = %d", i, ret );
		GetDlgItem( IDC_STATIC4 )->SetWindowText( msg );
	}
}

BOOL CRevTest8Dlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
//	if( pMsg->message == WM_KEYDOWN ) {
//		if( pMsg->wParam == VK_RETURN ) {
//			if( ::GetDlgCtrlID( pMsg->hwnd ) == IDC_EDIT1 ) {
//				pMsg->wParam = VK_TAB;
//			}
//		}
//	}

	return CDialog::PreTranslateMessage(pMsg);
}
