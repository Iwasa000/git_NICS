// DBSyzShinMain.cpp : アプリケーションのクラス動作を定義します。
//

#include "stdafx.h"
#include "DBSyzShinMain.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "DBSyzShinMainDoc.h"
#include "DBSyzShinMainView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDBSyzShinMainApp

BEGIN_MESSAGE_MAP(CDBSyzShinMainApp, ICSWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, &ICSWinApp::OnFileNew)
END_MESSAGE_MAP()


// CDBSyzShinMainApp コンストラクション

CDBSyzShinMainApp::CDBSyzShinMainApp()
{
	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}

// 唯一の CDBSyzShinMainApp オブジェクトです。

CDBSyzShinMainApp theApp;

// CDBSyzShinMainApp 初期化

int CDBSyzShinMainApp::BeginApp()
{
	return	ICSWinApp::BeginApp();
}

BOOL CDBSyzShinMainApp::InitInstance()
{
	// アプリケーション　マニフェストが　visual スタイルを有効にするために、
	// ComCtl32.dll バージョン 6　以降の使用を指定する場合は、
	// Windows XP に　InitCommonControls() が必要です。さもなければ、ウィンドウ作成はすべて失敗します。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// アプリケーションで使用するすべてのコモン コントロール クラスを含めるには、
	// これを設定します。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	ICSWinApp::InitInstance();

	// OLE ライブラリを初期化します。
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 標準初期化
	// これらの機能を使わずに、最終的な実行可能ファイルのサイズを縮小したい場合は、
	// 以下から、不要な初期化ルーチンを
	// 削除してください。
	// 設定が格納されているレジストリ キーを変更します。
	// TODO: この文字列を、会社名または組織名などの、
	// 適切な文字列に変更してください。
	SetRegistryKey(_T("日本ICS株式会社アプリケーション"));
	LoadStdProfileSettings(4);  // 標準の INI ファイルのオプションをロードします (MRU を含む)
	// アプリケーション用のドキュメント テンプレートを登録します。ドキュメント テンプレート
	//  はドキュメント、フレーム ウィンドウとビューを結合するために機能します。

	CreateDocTemplate( IDR_DBSYZSHINMAINTYPE,
		RUNTIME_CLASS(CDBSyzShinMainDoc),
		RUNTIME_CLASS(CChildFrame), // カスタム MDI 子フレーム
		RUNTIME_CLASS(CDBSyzShinMainView));

	// 31テスト--->
	CreateDocTemplate( IDR_DBSYZSHINTYPE5, 
		RUNTIME_CLASS(CH31HyoDoc),
		RUNTIME_CLASS(CChildFrame), // カスタム MDI 子フレーム
		RUNTIME_CLASS(CH31HyoView));
	//<---

	CreateDocTemplate( IDR_DBSYZSHINTYPE5, 
//		RUNTIME_CLASS(CDBSyzShinMainDoc),
		RUNTIME_CLASS(CH26HyoDoc),
		RUNTIME_CLASS(CChildFrame), // カスタム MDI 子フレーム
		RUNTIME_CLASS(CH26HyoView));
	
	CreateDocTemplate( IDR_DBSYZSHINTYPE3, 
//		RUNTIME_CLASS(CDBSyzShinMainDoc),
		RUNTIME_CLASS(CTaxListDoc),
		RUNTIME_CLASS(CChildFrame), // カスタム MDI 子フレーム
		RUNTIME_CLASS(CTaxationListView));

	CreateDocTemplate( IDR_DBSYZSHINTYPE3, 
//		RUNTIME_CLASS(CDBSyzShinMainDoc),
		RUNTIME_CLASS(CTaxList31Doc),
		RUNTIME_CLASS(CChildFrame), // カスタム MDI 子フレーム
		RUNTIME_CLASS(CTaxationList31View));

	// メイン MDI フレーム ウィンドウを作成します。
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// 接尾辞が存在する場合にのみ DragAcceptFiles を呼び出します。
	//  MDI アプリケーションでは、この呼び出しは、m_pMainWnd を設定した直後に発生しなければなりません。
	// DDE、file open など標準のシェル コマンドのコマンドラインを解析します。
	ICSCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// コマンド ラインで指定されたディスパッチ コマンドです。アプリケーションが
	// /RegServer、/Register、/Unregserver または /Unregister で起動された場合、 False を返します。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// メイン ウィンドウが初期化されたので、表示と更新を行います。
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	BeginApp();
	
	return TRUE;
}



// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();

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

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	
	CString	tmp;
	CString	AppTitle;
	AppTitle.LoadString( AFX_IDS_APP_TITLE );
	tmp.Format( "ﾊﾞｰｼﾞｮﾝ情報(%s)", AppTitle );
	SetWindowText( tmp );

	ICSVersionInfo	cv( AfxGetInstanceHandle() );

	tmp.Format( "%s Version %s", AppTitle, cv.FixedFileVersion() );
	GetDlgItem( IDC_STATICVERS )->SetWindowText( tmp );

	GetDlgItem( IDC_STATICCPY )->SetWindowText( cv.LegalCopyright() );

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// ダイアログを実行するためのアプリケーション コマンド
void CDBSyzShinMainApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CDBSyzShinMainApp メッセージ ハンドラ
CWnd* CDBSyzShinMainApp::GetHonHyoView()
{
	CWnd* wnd = NULL;

#ifdef _DBG_VIEW_START_
	CString	hoge;
	hoge.Format("Kaisei_VER:%d\n", Kaisei_VER);
	DbgViewTrace(hoge);
#endif

	if(Kaisei_VER == ID_VER_SYZ31)
		wnd = GetDocumentTemplate()->GetViewWnd(H31_HONPYO_WND_IDX);	// 31
	else
		wnd = GetDocumentTemplate()->GetViewWnd(H26_HONPYO_WND_IDX);	// 26

	return wnd;
}

CWnd* CDBSyzShinMainApp::GetHonHyoView( int version )
{
	CWnd* wnd = NULL;

#ifdef _DBG_VIEW_START_
	CString	hoge;
	hoge.Format("Kaisei_VER:%d\n", Kaisei_VER);
	DbgViewTrace(hoge);
#endif

	if( version == ID_VER_SYZ31 ){
		wnd = GetDocumentTemplate()->GetViewWnd(H31_HONPYO_WND_IDX);	// 31
	}
	else if( version == ID_VER_SYZ26 ){
		wnd = GetDocumentTemplate()->GetViewWnd(H26_HONPYO_WND_IDX);	// 26
	}

	return wnd;
}

CWnd* CDBSyzShinMainApp::GetTaxListView()
{
	CWnd* wnd = NULL;

	// 個人マスター
	//--->yoshida190808
	//旧26期間で付表２特定収入のボタンからリソース表示されなかったのを修正
	if (((CMainFrame*)m_pMainWnd)->m_sgBasic || *((CMainFrame*)m_pMainWnd)->m_ShinInfo.pSgSpc )
	{
		if (Kaisei_VER == ID_VER_SYZ31) {
			wnd = GetDocumentTemplate()->GetViewWnd(H31_KZTORIHIKI_WND_IDX);	// 31

#ifdef _20190930_SYUKA
			if ( *((CMainFrame*)m_pMainWnd)->m_ShinInfo.pSgSpc ) {
				wnd = NULL;
			}
#endif
		}
		else {
			wnd = GetDocumentTemplate()->GetViewWnd(H26_KZTORIHIKI_WND_IDX);	// 26
		}
	}
	else {
		wnd = NULL;
	}
	//----------------
	//if ( ( ( CMainFrame* )m_pMainWnd )->m_sgBasic ) {
	//	if(Kaisei_VER == ID_VER_SYZ31)
	//		wnd = GetDocumentTemplate()->GetViewWnd(H31_KZTORIHIKI_WND_IDX);	// 31
	//	else
	//		wnd = GetDocumentTemplate()->GetViewWnd(H26_KZTORIHIKI_WND_IDX);	// 26
	//}
	//else {
	//	wnd = NULL;
	//}
	//<------

	return wnd;
}

CWnd* CDBSyzShinMainApp::GetTaxListView(int version)
{
	CWnd* wnd = NULL;

	if( ((CMainFrame*)m_pMainWnd)->m_sgBasic || *((CMainFrame*)m_pMainWnd)->m_ShinInfo.pSgSpc ){

		if( version == ID_VER_SYZ31 ){
			wnd = GetDocumentTemplate()->GetViewWnd(H31_KZTORIHIKI_WND_IDX);	// 31

#ifdef _20190930_SYUKA
			if ( *((CMainFrame*)m_pMainWnd)->m_ShinInfo.pSgSpc ) {
				wnd = NULL;
			}
#endif
		}
		else if( version == ID_VER_SYZ26 ){
			wnd = GetDocumentTemplate()->GetViewWnd(H26_KZTORIHIKI_WND_IDX);	// 26
		}
	}
	else {
		wnd = NULL;
	}

	return wnd;
}

void CDBSyzShinMainApp::SetKaiseiVer(int ver)
{
#ifdef _DBG_VIEW_START_
	CString	hoge;
	hoge.Format("※※※  CDBSyzShinMainApp::SetKaiseiVer_START\n");
	DbgViewTrace(hoge);
#endif

	if( Kaisei_VER != ver ){
		// 切り替わりのタイミングでデーターの引き継ぎが必要。
		/*if( m_pMainWnd ){	// 会社切替でも動作してしまうので、移動
			((CMainFrame*)m_pMainWnd)->TakeOverData( Kaisei_VER, ver );
		}*/
	}
	Kaisei_VER = ver;

#ifdef _DBG_VIEW_START_
	hoge.Format("※※※  Kaisei_VER：%d\n",Kaisei_VER);
	DbgViewTrace(hoge);

	hoge.Format("※※※  CDBSyzShinMainApp::SetKaiseiVer_END\n");
	DbgViewTrace(hoge);
#endif
}

