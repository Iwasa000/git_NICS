// UCHIWAKE.cpp : アプリケーションのクラス動作を定義します。
//

#include "stdafx.h"
#include "UCHIWAKE.h"
#include "SecurePwdNfc.h"
//#include "UcUpdateYokuki.h"
// midori 180104 del -->
//// midori UC_0010 add -->
//#include "..\..\UCHIIMP\UCHIIMP\DBUchiImp8.h"
//// midori UC_0010 add <--
// midori 180104 del <--
// midori 180104 add -->
#include "..\UCHIIMP\DBUchiImp8.h"
// midori 180104 add <--

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 2010/07/XX 一括印刷対応
CBatchPrt g_BatchPrt;		// 一括印刷管理クラス

// midori 152151 add -->
int	NoActSw = 0;			// 1=ICSChildFrame::OnChildActivate を発行しない
// midori 152151 add <--

// midori 181002 del -->
////// midori 180104 add -->
//BOOL	CALLBACK	_DBUtwCoselCallBack( void *pctl, int a, int b );
////// midori UC_0015 del -->
// midori 181002 del <--
//int					bTantai;	// TRUE:単体マスター
// midori UC_0015 del <--
// midori 180104 add <--

// midori 190301 add -->
BOOL	bG_Kanso;
BOOL	bG_KansoLock;
BOOL	bG_KansoMessage;
// midori 190301 add <--

// インボイス登録番号追加対応_23/09/11 add -->
BOOL	bG_InvNo;				// インボイス番号使用サイン（決算期末が令和6年3月1日を超えている）
BOOL	bG_InvNoLock;
BOOL	bG_InvnoMessage;
// インボイス登録番号追加対応_23/09/11 add <--

// 改良No.21-0086,21-0529 cor -->
//// midori 156189,156190,156191 add -->
//int		KamokuRowEnableSgn(CDatabase* pDB, int pSw);
//// midori 156189,156190,156191 add <--
// ------------------------------
bool	m_Invoice;

BOOL	bG_KmkAdd1;				// 様式⑧に科目追加に関するデータ追加をしたサイン
BOOL	bG_KmkAdd2;				// 新規マスターとして様式②、⑯-2に「電子記録○○」等の科目を追加したサイン

BOOL	m_NewMaster;			// 新規にマスターを作成した

int		KamokuRowEnableSgn(CDatabase* pDB, int pSw, int nFormSeq);
// 改良No.21-0086,21-0529 cor <--

// midori 190505 add -->
int ChkIkkatuSort(CDatabase* pDB, CdbUcInfSub* pfcRec);
// midori 190505 add <--

// 24/01/30_23-0845 add -->
using namespace	Jz2CloudApi;
// 24/01/30_23-0845 add <--

// CUCHIWAKEApp

BEGIN_MESSAGE_MAP(CUCHIWAKEApp, ICSWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, &ICSWinApp::OnFileNew)
END_MESSAGE_MAP()


// CUCHIWAKEApp コンストラクション

CUCHIWAKEApp::CUCHIWAKEApp()
{
	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
	m_intCoSelFormOrder = -1;
// midori 152151 add -->
	NoActSw = 0;
// midori 152151 add <--
// midori UC_0015 add -->
	m_swUtwInfoRegister = 0;
// midori UC_0015 add <--
// midori 154237,154238 del -->
//// midori 154190 add -->
//	m_YsChg = 0;
//// midori 154190 add <--
// midori 154237,154238 del <--
// midori UC_0037 add -->
	m_swUtwTandoku = 0;
// midori UC_0037 add <--
// 157759 add -->
	m_swIcspCoop = 0;
// 157759 add <--
// 157768 add -->
	m_cmdlin = _T("");
// 157768 add <--
// 24/01/30_23-0845 add -->
	m_CloudSansyoFlg = FALSE;
	m_CloudSansyoFlg2 = FALSE;
// 24/01/30_23-0845 add <--
// 改良No.21-0086,21-0529 add -->
	bG_KmkAdd1 = FALSE;
	bG_KmkAdd2 = FALSE;
	m_NewMaster = FALSE;
// 改良No.21-0086,21-0529 add <--
}

CUCHIWAKEApp::~CUCHIWAKEApp()
{
}

// 唯一の CUCHIWAKEApp オブジェクトです。

CUCHIWAKEApp theApp;

// midori UC_0015 add -->
/////////////////////////////////////////////////////////////////////////////
// コマンドライン解析のためのクラス
class	CMyCommandLineInfo : public ICSCommandLineInfo //CCommandLineInfo
{
public:
	virtual void ParseParam( LPCTSTR lpszParam, BOOL bFlag, BOOL bLast );
};

void CMyCommandLineInfo::ParseParam( LPCTSTR lpszParam, BOOL bFlag, BOOL bLast )
{
	//MFCの規約で解析する場合は以下の行をそのまま使用します。
	//独自に解析する場合は以下の行を削除し、任意のコードを記述して下さい
//	CCommandLineInfo::ParseParam( lpszParam, bFlag, bLast );

	if ( bFlag == TRUE ) {
		if( !strcmp(lpszParam, _T("UtwInfoRegister")) ) {
			theApp.m_swUtwInfoRegister = 1;
		}
// 157768 del -->
//// 157759 add -->
//		else if(!strcmp(lpszParam,_T("ICSPDataImport")))	{
//			theApp.m_swIcspCoop = 1;
//		}
//// 157759 add <--
// 157768 del <--
// 157768 add -->
		if(!strcmp(lpszParam,_T("ICSPDataImport")))	{
			theApp.m_swIcspCoop = 1;
		}
		if(!strcmp(lpszParam,_T("SimpleCompanySale")))	{
			theApp.m_cmdlin = lpszParam;
		}
// 157768 add <--
	}

}
// midori UC_0015 add <--

// CUCHIWAKEApp 初期化

int CUCHIWAKEApp::BeginApp()
{
	// 通常は、"return ICSWinApp::BeginApp();"でOK。
	// ただし、ツールバー会社選択で会社切替を行った場合、当処理を実行しているが、
	// "ICSWinApp::BeginApp()"を実行すると、先頭(0)でビューを表示するようなので
	// その後に"SetActiveChild()"を実行する事にした
	//		参照 ･･･ ICSGEN80_20070326.doc
	int intRet = ICSWinApp::BeginApp();

	if (m_intCoSelFormOrder != -1) {
		// 【重要】
		// ツールバー会社選択実行時は、"ICSWinApp::BeginApp()"の後で画面切替が必要
// midori 190301 del -->
		//((CMainFrame*)AfxGetMainWnd())->SetActiveChild(m_uDocInfo[m_intCoSelFormOrder - 1].nID);
// midori 190301 del <--
// midori 190301 add -->
		int	nOfs=0;
// インボイス登録番号追加対応_23/09/11 del -->
		//if(bG_Kanso == TRUE && m_intCoSelFormOrder <= 22)	nOfs=UC_ID_FORMNO_MAX;
		//else												nOfs=0;
// インボイス登録番号追加対応_23/09/11 del <--
// インボイス登録番号追加対応_23/09/11 add -->
		if(bG_InvNo == TRUE && m_intCoSelFormOrder <= 42)		nOfs = UC_ID_FORMNO_MAX_KANSO;
		else if(bG_Kanso == TRUE && m_intCoSelFormOrder <= 22)	nOfs = UC_ID_FORMNO_MAX;
		else													nOfs = 0;
// インボイス登録番号追加対応_23/09/11 add <--
		((CMainFrame*)AfxGetMainWnd())->SetActiveChild(m_uDocInfo[(m_intCoSelFormOrder - 1)+nOfs].nID);
// midori 190301 add <--
		m_intCoSelFormOrder = -1;
	}
	return	intRet;
}

BOOL CUCHIWAKEApp::InitInstance()
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

	//CreateDocTemplate( IDR_UCHIWAKETYPE,
	//	RUNTIME_CLASS(CUCHIWAKEDoc),
	//	RUNTIME_CLASS(CChildFrame), // カスタム MDI 子フレーム
	//	RUNTIME_CLASS(CUCHIWAKEView));

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
// midori UC_0015 del -->
//	ICSCommandLineInfo cmdInfo;
// midori UC_0015 del <--
// midori UC_0015 add -->
	CMyCommandLineInfo cmdInfo;
// midori UC_0015 add <--
	ParseCommandLine(cmdInfo);
	// コマンド ラインで指定されたディスパッチ コマンドです。アプリケーションが
	// /RegServer、/Register、/Unregserver または /Unregister で起動された場合、 False を返します。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
// 157768 add -->
	if(m_cmdlin == _T("SimpleCompanySale"))	{
		pMainFrame->JobToolbarMode(0x01);	// 業務切替バーを非表示
	}
// 157768 add <--
	// メイン ウィンドウが初期化されたので、表示と更新を行います。
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	BeginApp();

	// 起動時に実行キャンセルを行った場合、メモリリークが発生するため
	// ↓以下の処理はMainFrameにて行う
/*	if (BeginApp() == -1) {
		// アプリケーションの終了処理(後始末)
		pMainFrame->EndApp();
		// 会社選択にて、キャンセルまたはエラーとなった場合はアプリ終了
		return FALSE;
	}

// 2006.09.14：タブ表示すると画面表示が可能？
	//	タブを非表示に設定
	pMainFrame->HideMDITabs();*/

	return TRUE;
 }

// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

// バージョン表示画面の拡大表示対応
class CAboutDlg : public CDialog		// 通常
//class CAboutDlg : public ICSDialog	// 拡大
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

// バージョン表示画面の拡大表示対応
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)		// 通常
//CAboutDlg::CAboutDlg() : ICSDialog(CAboutDlg::IDD)	// 拡大
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	// バージョン表示画面の拡大表示対応
	CDialog::DoDataExchange(pDX);		// 通常
	//ICSDialog::DoDataExchange(pDX);	// 拡大
}

BOOL CAboutDlg::OnInitDialog() 
{
	// バージョン表示画面の拡大表示対応
	CDialog::OnInitDialog();		// 通常
	//ICSDialog::OnInitDialog();	// 拡大
	
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

	// バージョン表示画面の拡大表示対応
	//ICSDialog::OnInitDialogEX();	// 拡大

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// バージョン表示画面の拡大表示対応
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)		// 通常
//BEGIN_MESSAGE_MAP(CAboutDlg, ICSDialog)	// 拡大
END_MESSAGE_MAP()

// ダイアログを実行するためのアプリケーション コマンド
void CUCHIWAKEApp::OnAppAbout()
{
	CAboutDlg aboutDlg;

// 要通知案件修正 21/07/09 add -->
	CFrameWnd*			pFrame	= (CFrameWnd*)AfxGetMainWnd();
	CMDIChildWnd*		pChild	= (CMDIChildWnd *)pFrame->GetActiveFrame();
	CfrmUc000Common*	frmView	= (CfrmUc000Common*)pChild->GetActiveView();
	if(frmView != NULL)	{
		frmView->CmnEventStartProc(ID_BUTTON_HOME);
	}
// 要通知案件修正 21/07/09 add <--

	aboutDlg.DoModal();
}

// CUCHIWAKEApp メッセージ ハンドラ

/************************************************************************
	MakeDatabaseName()
	データベース名を作成
	  PCOM_GetString() で取得した値をパラメータを受け取り、生成する
	  (Z00_C12345678_Y1234 の形式で作成)

	引数：	char*	inApno			アプリケーション番号
			char*	inCoCode		会社番号
			char*	inKikan			期末年月日

	戻値：	CString					生成したデータベース名

	例）
	PCOM_GetString( "SELECTCODSN", szSQLServer, szApno, szCoCode, szKikan);
	MakeDatabaseName(szApno, szCoCode, szKikan);

*************************************************************************/
CString CUCHIWAKEApp::MakeDatabaseName	( char* inApno
										, char* inCoCode
										, char* inKikan
										)
{
	CString	tmpKikan(inKikan);
	CString retDatabaseName;

	retDatabaseName.Format("Z%02x_C%08d_Y%04d",
				atoi(inApno), atoi(inCoCode), atoi(tmpKikan.Mid(2, 4)) );

	return retDatabaseName;
}

/************************************************************************
	MakeDatabaseName()
	データベース名を作成
	  PCOM_GetString() で取得した値をパラメータを受け取り、生成する
	  (Z00_C12345678_Y1234 の形式で作成)

	引数：	char*	inApno			アプリケーション番号
			char*	inCoCode		会社番号
			char*	inKikan			期末年月日

	戻値：	CString					生成したデータベース名

	例）
	PCOM_GetString( "SELECTCODSN", szSQLServer, szApno, szCoCode, szKikan);
	MakeDatabaseName(szApno, szCoCode, szKikan);

*************************************************************************/
CString CUCHIWAKEApp::MakeDatabaseNamePublic	( char* inApno
												, char* inCoCode
												, char* inKikan
												)
{
	return MakeDatabaseName(inApno, inCoCode, inKikan);
}

// midori UC_0006 del -->
//// midori 171108 del -->
/////************************************************************************
////	CheckAndMakeTableSub()
////		SQLスクリプト(ファイル)を実行
////
////		sqlcmdコマンドラインアプリを使用して実行する。	（ver1.0.1.12から　それ以前はosql）
////
////	引数：	CString inFileName			ファイル名
////			CString inSQLServer			SQLサーバ名
////			CString	inDatabaseName		データベース名
////			CString inSQLUse			ファイルの用途(例："作成/初期化用")
////			int     mode				0:SQL作成 1:SQL更新
////
////	戻値：	BOOL						成功(TRUE) / 失敗(FALSE) を返す
////*************************************************************************/
////BOOL CUCHIWAKEApp::CheckAndMakeTableSub	( CString inFileName
////										, CString inSQLServer
////										, CString inDatabaseName
////										, CString inSQLUse
////										, int	  mode
////										)
////{
////	CFileFind FileFind;
////
////	// ファイル存在チェック
////	if ( !FileFind.FindFile( inFileName ) ){
////		
////		// 業務ツールバーからの会社切り替え時はエラーメッセージ無し
////		if ( m_intCoSelType != 2 ){
////			CString strMsg;
////	//		strMsg.Format( "ファイルがありません。\r\n(%s)", inFileName );
////	//		strMsg = "勘定科目内訳書マスターの「作成／初期化用ファイル」\r\nを検索できませんでした。";
////			strMsg = "勘定科目内訳書マスターの「" + inSQLUse + "ファイル」\r\nを検索出来ませんでした。";
////			ICSMessageBox( strMsg );
////		}
////		return FALSE;
////	}
////
////	// SQL実行コマンド作成 ( sqlcmd アプリを実行　バージョン1.0.1.11まではosqlだった）
////	CString	ExeCommand;
////	CString dbpass = SecureICSSaPwd();
////	CString ExecMode;
////	if(mode == 0)		ExecMode = "-v ExecMode = 0";	// コピー時は-vで引数を与える　更新時は与えない（SQLファイル内で与えられた引数を使用しなくても問題ないようだ）
////	ExeCommand.Format	( "sqlcmd -U %s -P %s -S %s -d %s -i %s %s"
////	//ExeCommand.Format	( "osql -U %s -P %s -S %s -d %s -i %s"
////						, DB_STR_UNAME				//ユーザ名
////						, dbpass					//パスワード
////						, inSQLServer				//サーバ名
////						, inDatabaseName			//データベース名
////						, inFileName				//ファイル名
////						, ExecMode					//引数
////						);
////
////	// 外部アプリを呼び出し、実行する
////	STARTUPINFO			si;
////	PROCESS_INFORMATION	pi;
////
////	ZeroMemory( &pi, sizeof( PROCESS_INFORMATION ));
////	ZeroMemory( &si, sizeof( STARTUPINFO ));
////	si.cb          = sizeof( STARTUPINFO );
////	if ( CreateProcess(	  NULL						//1
////						, ExeCommand.GetBuffer()	//2 コマンドライン文字列
////						, NULL						//3
////						, NULL						//4
////						, FALSE						//5
////						, (NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW)	//6
////						, NULL						//7
////						, NULL						//8
////						, &si						//9
////						, &pi						//10
////						) )
////	{
////		//?? ここで、ユーザにわかるダイアログを出したい（実行中がわかるように！）
////		//ICSProgressWnd	icsPro( AfxGetMainWnd() );	//	プログレスバークラス
////		AfxGetApp()->DoWaitCursor(1);	//	砂時計を表示
////
////		//	プログレスバー開始
////		//icsPro.BeginProgress();
////
////		//  スレッドハンドル解放
////		CloseHandle(pi.hThread);
////
////		//  起動したプロセスの終了を待つ
////		WaitForSingleObject( pi.hProcess, INFINITE );
////
////		// プロセスハンドル解放
////		CloseHandle(pi.hProcess);
////		
////		//	プログレスバー終了
////		//icsPro.EndProgress(); 
////		//?? ここで、ユーザにわかるダイアログを消したい
////		AfxGetApp()->DoWaitCursor(-1);	//	矢印カーソルに戻す
////	}
////	else {
////		return FALSE;
////	}
////
////	return TRUE;
////}
//// midori 171108 del <--
//// midori 171108 add -->
///************************************************************************
//	CheckAndMakeTableSub()
//		SQLスクリプト(ファイル)を実行
//
//		sqlcmdコマンドラインアプリを使用して実行する。	（ver1.0.1.12から　それ以前はosql）
//
//	引数：	CString inFileName			ファイル名
//			CString inSQLServer			SQLサーバ名
//			CString	inDatabaseName		データベース名
//			CString inSQLUse			ファイルの用途(例："作成/初期化用")
//			int     mode				0:SQL作成 1:SQL更新
//
//	戻値：	BOOL						成功(TRUE) / 失敗(FALSE) を返す
//*************************************************************************/
//BOOL CUCHIWAKEApp::CheckAndMakeTableSub	( CString inFileName
//										, CString inSQLServer
//										, CString inDatabaseName
//										, CString inSQLUse
//										, int	  mode
//										)
//{
//	CFileFind	FileFind;
//	CString		errStr=_T("");
//
//	// ファイル存在チェック
//	if ( !FileFind.FindFile( inFileName ) ){
//		
//		// 業務ツールバーからの会社切り替え時はエラーメッセージ無し
//		if ( m_intCoSelType != 2 ){
//			CString strMsg;
//			strMsg = "勘定科目内訳書マスターの「" + inSQLUse + "ファイル」\r\nを検索出来ませんでした。";
//			ICSMessageBox( strMsg );
//		}
//		return FALSE;
//	}
//
//	if(ExecuteSqlBySMO(inSQLServer,inDatabaseName,inFileName,errStr) == -1)	{
//		ICSMessageBox( errStr );
//		return(FALSE);
//	}
//
//	return TRUE;
//}
//// midori 171108 add <--
//
///************************************************************************
//	CheckAndMakeTable()
//
//		内訳書で必要なテーブル、初期データを作成する
//
//	引数：
//			CString inSQLServer			SQLサーバ名
//			CString	inDatabaseName		データベース名
//			CString inFileName			ファイル名(フルパス)
//			CString inSQLUse			ファイルの用途(例："作成/初期化用")
//			int     mode				0:SQL作成 1:SQL更新
//	戻値：	BOOL						成功(TRUE) / 失敗(FALSE) を返す
//*************************************************************************/
//BOOL CUCHIWAKEApp::CheckAndMakeTable( CString inSQLServer
//									, CString inDatabaseName
//									, CString inFileName
//									, CString inSQLUse
//									, int     mode
//									)
//{
//	BOOL bRet;
//
//	// 内訳書テーブル作成～初期値データ登録 SQL
//	bRet = CheckAndMakeTableSub( inFileName, inSQLServer, inDatabaseName, inSQLUse, mode );
//
//	if ( bRet != TRUE) {
//		// 失敗した場合
//		return bRet;
//	}
//
//	// 内訳書 初期値データ 登録 SQL
////	bRet = CheckAndMakeTableSub( (m_strPath + DB_STR_SQLFILE_DATA), inSQLServer, inDatabaseName );
//
//	return bRet;
//}
// midori UC_0006 del <--
// midori UC_0006 add -->
/************************************************************************
	CheckAndMakeTableSub()
		SQLスクリプト(ファイル)を実行

		sqlcmdコマンドラインアプリを使用して実行する。	（ver1.0.1.12から　それ以前はosql）

	引数：	CString inFileName			ファイル名
			CString inSQLServer			SQLサーバ名
			CString	inDatabaseName		データベース名
			CString inSQLUse			ファイルの用途(例："作成/初期化用")
			int     mode				0:SQL作成 1:SQL更新

	戻値：	int							-1:ファイルオープンエラー -2:SQLファイル実行エラー 0:正常
*************************************************************************/
int CUCHIWAKEApp::CheckAndMakeTableSub	( CString inFileName
										, CString inSQLServer
										, CString inDatabaseName
										, CString inSQLUse
										, int	  mode
										)
{
	CFileFind	FileFind;
	CString		errStr=_T("");

	// ファイル存在チェック
	if ( !FileFind.FindFile( inFileName ) ){
		
		// 業務ツールバーからの会社切り替え時はエラーメッセージ無し
		if ( m_intCoSelType != 2 ){
			CString strMsg;
			strMsg = "勘定科目内訳書マスターの「" + inSQLUse + "ファイル」\r\nを検索出来ませんでした。";
			ICSMessageBox( strMsg );
		}
		return(-1);
	}

	if(ExecuteSqlBySMO(inSQLServer,inDatabaseName,inFileName,errStr) == -1)	{
		return(-2);
	}

	return(0);
}

/************************************************************************
	CheckAndMakeTable()

		内訳書で必要なテーブル、初期データを作成する

	引数：
			CString inSQLServer			SQLサーバ名
			CString	inDatabaseName		データベース名
			CString inFileName			ファイル名(フルパス)
			CString inSQLUse			ファイルの用途(例："作成/初期化用")
			int     mode				0:SQL作成 1:SQL更新
	戻値：	int							-1:ファイルオープンエラー -2:SQLファイル実行エラー 0:正常
*************************************************************************/
int CUCHIWAKEApp::CheckAndMakeTable( CString inSQLServer
									, CString inDatabaseName
									, CString inFileName
									, CString inSQLUse
									, int     mode
									)
{
	int nRet;

	// 内訳書テーブル作成～初期値データ登録 SQL
	nRet = CheckAndMakeTableSub( inFileName, inSQLServer, inDatabaseName, inSQLUse, mode );

	return(nRet);
}

/************************************************************************
	CheckTable()
		テーブルの重複を修正

	引数：	なし

	戻値：	0:正常 1:エラー
*************************************************************************/
int CUCHIWAKEApp::CheckTable()
{
	int					ii=0;
// midori UC_0007 add -->
	int					id=0;
	CdbUcInfSub			mfcRecSub(m_pDB);	// uc_inf_subテーブルクラス
// midori UC_0007 add <--
	CString				strQuery=_T("");
	CString				strData[6];
	CRecordset			rs(m_pZmSub->m_database);

	// uc_lst_item_sort
	for(ii=0;ii<6;ii++)	{
		strData[ii].Empty();
	}
	try	{
		strQuery = _T("SELECT COUNT(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 2 and ItemSeq = 8 GROUP BY FormSeq,ItemSeq");
		rs.Open(CRecordset::forwardOnly, strQuery);
		rs.GetFieldValue((short)0,strData[0]);
		rs.Close();
	}
	catch(CException* e)	{
		e->Delete();
		return(-1);
	}

	if(_tstoi(strData[0]) > 1)	{
		// レコードを削除し、再作成
		m_pZmSub->m_database->ExecuteSQL(_T("DELETE FROM uc_lst_item_sort WHERE FormSeq = 2 and ItemSeq = 8"));

		strQuery = _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strQuery += _T("VALUES(2, 8, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC')");
		m_pZmSub->m_database->ExecuteSQL(strQuery);
	}

	// uc_inf_sub_omit
	for(ii=0;ii<6;ii++)	{
		strData[ii].Empty();
	}
	try	{
		strQuery = _T("SELECT COUNT(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 16 and ItemSeq = 4 GROUP BY FormSeq,ItemSeq");
		rs.Open(CRecordset::forwardOnly, strQuery);
		rs.GetFieldValue((short)0,strData[0]);
		rs.Close();
	}
	catch(CException* e)	{
		e->Delete();
		return(-1);
	}

	if(_tstoi(strData[0]) > 1)	{
		// データを取得
		strData[0].Empty();
		try	{
			strQuery = _T("SELECT FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen FROM uc_inf_sub_omit WHERE FormSeq = 16 and ItemSeq = 4");
			rs.Open(CRecordset::forwardOnly, strQuery);
			for(ii=0;ii<6;ii++)	{
				rs.GetFieldValue((short)ii,strData[ii]);
			}
			rs.Close();
		}
		catch(CException* e)	{
			e->Delete();
			return(-1);
		}
		// レコードを削除し、再作成
		m_pZmSub->m_database->ExecuteSQL(_T("DELETE FROM uc_inf_sub_omit WHERE FormSeq = 16 and ItemSeq = 4"));

		strQuery = _T("INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) ");
		strQuery.Format(strQuery + _T("VALUES( %d, %d, '%s', %d, '%s', %d )"),_tstoi(strData[0]),_tstoi(strData[1]),strData[2],_tstoi(strData[3]),strData[4],_tstoi(strData[5]));
		m_pZmSub->m_database->ExecuteSQL(strQuery);
	}

// midori UC_0007 add -->
	// -------------------------------------------------------------------------------------------
	// 科目行を出力するの有効サイン(uc_inf_sub.GeneralConstVal[15] & 0x02)がOFFであればONにする
	// -------------------------------------------------------------------------------------------
	for(ii=0; ii<2; ii++)	{
		// ②受取手形の内訳書
		if(ii == 0)	id = ID_FORMNO_021;
		// ⑪借入金及び支払利子の内訳書
		else		id = ID_FORMNO_111;
		if(mfcRecSub.RequeryFormSeq(id) == DB_ERR_OK)	{
			if(!(mfcRecSub.IsEOF()))	{
				if(!(mfcRecSub.m_GeneralConstVal[15] & BIT_D1))	{
					mfcRecSub.Edit();
					mfcRecSub.m_GeneralConstVal[15] |= BIT_D1;
					mfcRecSub.Update();
				}
			}
			mfcRecSub.Fin();
		}
	}
// midori UC_0007 add <--

	return(0);
}
// midori UC_0006 add <--

// midori 180104 del -->
//// 会社選択
//int CUCHIWAKEApp::CorporationSelect()
// midori 180104 del <--
// midori 180104 add -->
//**************************************************
// 会社選択
//	【引数】	intCoSelType　…　会社選択タイプ ( 0:会社起動時　1:会社選択ツールバーからの切り替え時　2:業務ツールバーからの会社切り替え時)
//	【戻値】	-1　…　エラー
//**************************************************
int CUCHIWAKEApp::CorporationSelect( int intCoSelType )
// midori 180104 add <--
{
// midori M-17061301 del -->
//	CoSel	cs;			// 会社選択クラス
// midori M-17061301 del <--
	int		nRetCode;	// 返信コード
// midori 181002 add -->
	int		rv = 0;
	CString cst=_T("");
// midori 181002 add <--
// midori UC_0010 add -->
	CDBUchiImp	clsImport;
// midori UC_0010 add <--
// midori UC_0037 add -->
	CUcFunctionCommon	ufc;
// midori UC_0037 add <--

	//---------------------------------
	// (1)会社選択画面を表示
	//---------------------------------
// midori 180104 del -->
//// midori M-17061301 del -->
////	nRetCode = cs.co_ok();	// ここで選択された会社情報が、
////							// C:\ICSWin\COMM\ログイン名\PCOM.INF
////							// セットされる。
//// midori M-17061301 del -->
//// midori M-17061301 add -->
//	nRetCode = m_Cs.co_ok();	// ここで選択された会社情報が、
//								// C:\ICSWin\COMM\ログイン名\PCOM.INF
//								// セットされる。
//// midori M-17061301 add -->
//
//	if (nRetCode != IDOK) {
//		// 会社選択にて「キャンセル」または「エラー」の場合
//		SetExitMessage( m_intCoSelType ,  "処理を終了します。" );
//		return 0;
//	}
// midori 180104 del <--
// midori 180104 add -->
	if(intCoSelType == 0)	{
		// 1:会社選択ツールバーからの切り替え時, 2:業務ツールバーからの会社切り替え時は
		// OnFromParentCosel で co_ok が呼ばれているので、ここでは行わない
		nRetCode = co_ok_ex(0);
		if (nRetCode != IDOK)	{
			// 会社選択にて「キャンセル」または「エラー」の場合
			SetExitMessage( m_intCoSelType ,  "処理を終了します。" );
			return 0;
		}
	}
// midori 180104 add <--

	//-------------------------------
	//	(2)選択された会社の情報を取得
	//-------------------------------
	char	szSQLServer[30];		// SQLサーバ名
	char	szApno[20];				// アプリケーション番号
	char	szCoCode[20];			// 会社コード
	char	szKikan[20];			// 期末年月日

	if( GetPCOMSELECTCODSN(szSQLServer, szApno, szCoCode, szKikan) == FALSE ){
		return 0;
	}

// midori UC_0037 add -->
	m_swUtwTandoku = GetUcTandokuSw(atoi(szApno),atoi(szCoCode),atoi(szKikan));
	if(m_swUtwTandoku == -1) {
		return(-1);
	}
// midori 155028 add -->
	else if(m_swUtwTandoku != 0 && m_swUtwTandoku != 1)	{
		// 財務ｄｂマスターでも内訳書(非連動)マスターでもない場合
		cst = _T("選択した事業年度に財務処理ｄｂマスターが存在しません。\n");
		cst+= _T("処理を行う事業期間（期末）を正しく選択してください。");
		ICSMessageBox(cst);
		SetExitMessage(m_intCoSelType,"処理を終了します。");
		return(-1);
	}
// midori 155028 add <--

	cst = ufc.GetGrNo();
	ICSToolBar *pTool = ((CMainFrame*)AfxGetMainWnd())->GetToolBar(0);
	CMenu menu;
	menu.LoadMenu( IDR_UCHIWAKETYPE );
	if(m_swUtwTandoku == 1 || cst.CompareNoCase( _T("1B") ) != 0)	{
		menu.RemoveMenu(6,MF_BYPOSITION);
	}
	else	{
		menu.InsertMenu(-1,MF_BYPOSITION);
	}
	pTool->LoadMenu(&menu);
// midori UC_0037 add <--

// midori UC_0026 del -->
//// midori 181101 add -->
//	m_APNO = atoi(szApno);
//// midori 181101 add -->
// midori UC_0026 del <--
	// 取得したデータより、データベース名を作成する
	m_DatabaseName = MakeDatabaseName(szApno, szCoCode, szKikan);

	//-------------------------------
	//	(3)DB接続
	//-------------------------------
	//	DB接続(ICS様のCDBZmSubオブジェクト経由の場合)
	// vers		:ICSマスターバージョン（本アプリの定数：DB_ICS_VERSION で定義）
	// server	:サーバ名
	// apno		:アプリケーション番号
	// cono		:会社番号
	// ymd		:期末年月日
	// ntype	:(今回省略) 認証タイプ（0=ICS認証、1=SQL認証、2=windows認証）
	// id		:(今回省略) SQL認証の際のID
	// pwd		:(今回省略) SQL認証の際のパスワード
	//-------------------------------
	// [versについて]
	// バージョンが０以下のマスターのみ扱うアプリである事を示す。
	// 将来、全マスターを動作不可にしたい場合、マスターバージョンを１にする事により動作不可となる。
	// その際、何らかの対応を行なったアプリは、当引数は1にする事により動作可能となる
	//-------------------------------
	//nRetCode = m_ZmSub.SetMasterDB( DB_ICS_VERSION, szSQLServer, atoi(szApno), atoi(szCoCode), atoi(szKikan), DB_NTYPE, DB_STR_UNAME, DB_STR_PNAME );
//	nRetCode = m_ZmSub.SetMasterDB( DB_ICS_VERSION, szSQLServer, atoi(szApno), atoi(szCoCode), atoi(szKikan) );

	// 2006.09.13：財務クラスはnew/deleteで使用するのを共通仕様とする
// midori 151730,151263 class del -->
	//m_pZmSub = new CDBZmSub;
// midori 151730,151263 class del <--
// midori 151730,151263 class add -->
	if(isHIEIRI(atoi(szApno)))	{
		m_pZmSub = new CDBNpSub;
	}
	else	{
		m_pZmSub = (CDBNpSub *)new CDBZmSub;
	}
// midori 151730,151263 class add <--

	// 2007.03 DBエラー対応
	m_pZmSub->ExitMode(1);

	nRetCode = m_pZmSub->SetMasterDB( DB_ICS_VERSION, szSQLServer, atoi(szApno), atoi(szCoCode), atoi(szKikan) );

	if ( nRetCode != 0) {
		// データベースのバージョン違いの場合
		// 2007.03 DBエラー対応
		m_clsFunc.ZmSubErrMsg( m_pZmSub );
		return -1;
	}

	// 消費税8%対応(2013/09/10)
	CZmGen8 ZmGen;
	CString strSyzMessage;
	ZMGEN_CHKINFO chkinfo = {0};
// midori UC_0051 add -->
	chkinfo.verupsub = 0x800;
// midori UC_0051 add <--
	int ret = ZmGen.CheckDBMasterState( m_pZmSub, strSyzMessage, chkinfo );
	if(( ret == -1 ) || ( ret == -2 )){	// エラー
		ICSMessageBox( ZmGen.GetErrorMsg(), MB_ICONSTOP );
		return -1;
	}
	else if( ret == 1 ){	// メッセージを表示する必要がある正常終了
		// 注意喚起のメッセージを表示して処理継続
		ICSMessageBox( strSyzMessage );
	}

	m_pDB = NULL;
//	m_pDB = m_ZmSub.m_database;
	m_pDB = m_pZmSub->m_database;

	// Ctrlキー押しながら選択した？
	char	cCtrl = 0;		//	SHIFTキー状態
	cCtrl = ( char )GetKeyState( VK_CONTROL );

// 改良No.21-0086,21-0529 add -->
	m_NewMaster = FALSE;

	char	syssvr[128]={0};
	GCOM_GetString(_T("SystemDataBase"), syssvr);
	if(m_pZmSub->SysCtlDB(syssvr) == -1)	{
		ICSMessageBox(ZmGen.GetErrorMsg(), MB_ICONSTOP);
		return -1;
	}
// 改良No.21-0086,21-0529 add <--

	// 会社情報の取得
	if ( ICSGetZvolRenData( m_pZmSub , &m_RenZvol ) != FUNCTION_OK ){
		return 0;
	}
	
	//*****************************************************************
	// マスター移動業務のフィルタが内訳書テーブルは"uc_"で始まっている
	// としているので、今後テーブルを追加する際も、テーブル名は
	// "uc_○○"といった具合で追加すること
	//*****************************************************************
	///////////////////////////////////////////////////////////////////
	// 内訳書テーブルバージョンチェック
	//	(1) テーブルなし
	//			→ テーブルを作成、初期データ登録
	//	(2) アプリで扱えるバージョンより、テーブルバージョンが古い
	//			→ テーブルを更新する
	//	(3) アプリで扱えるバージョンより、テーブルバージョンが新しい
	//			→ アプリを更新する
	//	(4) アプリで扱えるバージョンと、テーブルバージョンが同じ
	//			→ OK!
	{
//		CdbIcsVtbl	vtbl(m_ZmSub.m_database);
		CdbIcsVtbl	vtbl(m_pZmSub->m_database);
		int			version = DB_UC_TABLE_VERSION;
		int			retVer;

		retVer = vtbl.VersionCheck(DB_UC_VTBL_TYPE, DB_UC_VTBL_OBJNAME);

		// Ctrlキー＋Altキーが押されていたら、内訳書マスター削除（公開はしない）
		if( (retVer > 0) && (cCtrl & 0x80) && GetKeyState(VK_MENU)&0x8000 ){
// midori 16 del -->
//// midori 153831 del ------------------>
////			if( ICSMessageBox( _T("内訳書マスターを削除します。よろしいですか？"), MB_OKCANCEL ) == IDOK ){
//// midori 153831 del <------------------
//// midori 153831 add ------------------>
//			if( ICSMessageBox( _T("内訳書マスターを削除します。よろしいですか？"), (MB_OKCANCEL | MB_DEFBUTTON2) ) == IDOK ){
//// midori 153831 add <------------------
// midori 16 del <--
// midori 16 add -->
			if( ICSMessageBox( _T("勘定科目内訳書マスターを削除します。よろしいですか？"), (MB_OKCANCEL | MB_DEFBUTTON2) ) == IDOK ){
// midori 16 add <--
// midori 6 del -->
//				DeleteAllTables();
// midori 6 del <--
// midori 6 add -->
// midori 181002 del -->
				//if(DeleteAllTables() == FALSE)	{
				//	return(0);
				//}
// midori 181002 del <--
// midori 181002 add -->
// midori UC_0037 del -->
				//if(DeleteUtiTables(m_pZmSub,m_swUtwInfoRegister,0) == -1)		{
// midori UC_0037 del <--
// midori UC_0037 add -->
				if(DeleteUtiTables(m_pZmSub,m_swUtwTandoku,0) == -1)		{
// midori UC_0037 add <--
					ZenGetErrorMsg(cst);
					ICSMessageBox(cst);
					return(-1);
				}
// midori 181002 add <--
// midori 6 add <--

// midori UC_0035 add -->
				m_pZmSub->VolumeOpen();
				// corec::c_subswのD5(0x20)をOFFにする
				if(SetCoRec2(m_pZmSub->zvol->apno,m_pZmSub->zvol->v_cod,m_pZmSub->zvol->ee_ymd,0,&cst) == -1) {
					ICSMessageBox(cst);
					return(-1);
				}
				m_pZmSub->VolumeClose();
// midori UC_0035 add <--

				retVer = DB_ERR_EOF;
			}
		}

		if (retVer < 0) {	// DBエラー関連（Vtblアクセス時）

			if (retVer == DB_ERR_EOF) {	// レコードが無い
				// 内訳書用テーブルが存在していないと判断し、
				// テーブル作成、初期値登録を実施
			
				// (1)テーブルが無い
//				ICSMessageBox( "勘定科目内訳書マスターがありません。マスターを作成します。" );
// midori 155051 del -->
//// midori 180104 del -->
//				//// 業務ツールバーからの会社切り替え時はエラーメッセージ無しで実行
//				//if ( m_intCoSelType != 2 ){
//// midori 180104 del <--
//// midori 180104 add -->
//				// 業務ツールバーからの会社切り替え時、単体マスターで初回起動時は
//				// メッセージ無しで実行
//// midori UC_0015 del -->
//				//if(m_intCoSelType != 2 && bTantai == FALSE)	{
//// midori UC_0015 del <--
//// midori UC_0015 add -->
//				if(m_intCoSelType != 2 && m_swUtwInfoRegister == 0)	{
//// midori UC_0015 add <--
//// midori 180104 add <--
// midori 155051 del <--
// midori 155051 add -->
				// 業務ツールバーからの会社切り替え時、単体マスター作成時はメッセージ無しで実行
				if(m_intCoSelType != 2 && m_swUtwTandoku != 1)	{
// midori 155051 add <--
					SetNonActiveFlag(TRUE);
					if (ICSMessageBox( "勘定科目内訳書マスターを作成します。", MB_OKCANCEL ) == IDCANCEL) {
						SetNonActiveFlag(FALSE);
// 2022/12/28 以下の手順で処理を行うと不正するのを修正
// 1.既に勘定科目内訳書マスターを作成している会社を選択
// 2.業務内の会社切替ボタンで一度も勘定科目内訳書を起動していない会社を選択
// 3.「勘定科目内訳書マスターを作成します。」の確認メッセージでキャンセルを選択
//						return 0;
						return(-1);
					}
					SetNonActiveFlag(FALSE);
				}

				// テーブル作成、データ登録を実行する				
// midori UC_0006 del -->
				//BOOL bRet = CheckAndMakeTable(szSQLServer, m_DatabaseName, (m_strPath + DB_STR_SQLFILE_TABLE), DB_STR_SQLUSE_TABLE, SQL_EXEC_CREATE);

				//if (bRet == FALSE) {
				//	SetExitMessage( m_intCoSelType , "勘定科目内訳書マスター作成に失敗しました。" );
				//	return 0;
				//}
// midori UC_0006 del <--
// midori UC_0006 add -->
// midori UC_0044 del -->
				//int nRet = CheckAndMakeTable(szSQLServer, m_DatabaseName, (m_strPath + DB_STR_SQLFILE_TABLE), DB_STR_SQLUSE_TABLE, SQL_EXEC_CREATE);
// midori UC_0044 del <--
// midori UC_0044 add -->
				CFileFind	FileFind;
				int			rv1=0,rv2=0;
				int			nRet=0;
				rv1 = FileFind.FindFile(m_strPath + DB_STR_SQLFILE_TABLE1);
				rv2 = FileFind.FindFile(m_strPath + DB_STR_SQLFILE_TABLE2);
				if(rv1 == 0 || rv2 == 0) {
					nRet = CheckAndMakeTable(szSQLServer,m_DatabaseName,(m_strPath + DB_STR_SQLFILE_TABLE),DB_STR_SQLUSE_TABLE,SQL_EXEC_CREATE);
				}
				else	{
					nRet = CheckAndMakeTable(szSQLServer,m_DatabaseName,(m_strPath + DB_STR_SQLFILE_TABLE1),DB_STR_SQLUSE_TABLE,SQL_EXEC_CREATE);
					if(nRet == 0) {
						nRet = CheckAndMakeTable(szSQLServer,m_DatabaseName,(m_strPath + DB_STR_SQLFILE_TABLE2),DB_STR_SQLUSE_TABLE,SQL_EXEC_CREATE);
					}
				}
// midori UC_0044 add <--
				if (nRet == -1) {
					SetExitMessage( m_intCoSelType , "勘定科目内訳書マスター作成に失敗しました。" );
					return(0);
				}
				else if(nRet == -2)	{
					return(0);
				}
// midori UC_0006 add <--

				// 内訳書テーブル作成サインON
				if ( m_pZmSub->VolumeOpen() == 0 ) {
					m_pZmSub->zvol->Edit();
// midori 181002_2 del -->
//// midori 181002 del -->
////					m_pZmSub->zvol->sub_sw |= DB_UC_TABLE_CREATE_ON;
//// midori 181002 del <--
//// midori 181002 add -->
//					if(m_swUtwInfoRegister == 0)	m_pZmSub->zvol->sub_sw |= DB_UC_TABLE_CREATE_ON;
//					else							m_pZmSub->zvol->sub_sw |= DB_UC_TABLE_CREATE_ON_T;
//// midori 181002 add <--
// midori 181002_2 del <--
// midori 181002_2 add -->
					m_pZmSub->zvol->sub_sw |= DB_UC_TABLE_CREATE_ON;
// midori UC_0037 del -->
					//if(m_swUtwInfoRegister != 0)	{
// midori UC_0037 del <--
// midori UC_0037 add -->
					if(m_swUtwTandoku != 0)	{
// midori UC_0037 add <--
						m_pZmSub->zvol->sub_sw |= DB_UC_TABLE_CREATE_ON_T;
					}
// midori 181002_2 add <--
					m_pZmSub->zvol->Update();
// midori 181002 add -->
					// 非連動の場合CoRecの勘定科目内訳書作成サインをONにする
// midori UC_0037 del -->
					//if(m_swUtwInfoRegister == 1) {
// midori UC_0037 del <--
// midori UC_0037 add -->
					if(m_swUtwTandoku == 1) {
// midori UC_0037 add <--
						if(SetCoRec(m_pZmSub->zvol->apno,m_pZmSub->zvol->v_cod,m_pZmSub->zvol->ee_ymd,1,&cst) == -1) {
							ICSMessageBox(cst);
							return(-1);
						}
					}
// midori 181002 add <--
					m_pZmSub->VolumeClose();
				}
				else{
					// 2007.03 DBエラー対応
					m_clsFunc.ZmSubErrMsg( m_pZmSub );
					return 0;
				}

//				ICSMessageBox( "勘定科目内訳書マスターを作成しました。" );

				// 作成に成功した場合は、(4)と同じ流れとなるので、return せず
// midori UC_0010 add -->
				// バージョンアップ時はVersionUp内で行うようにしたため
				// 新規作成のみここで行うようにする
				if(CheckTable() == -1)	{
					return(0);
				}
// midori UC_0010 add <--

				m_NewMaster = TRUE;		// 改良No.21-0086,21-0529 add
			}
			else {	// Vtblアクセス時に何らかのエラー発生
				//DBエラー対応にてエラーメッセージ実装のためコメントアウト
				//SetExitMessage( m_intCoSelType ,  "作業を中止します。\r\nバージョン管理テーブル vtbl アクセス時にエラーが発生しました。"  );
				return 0;
			}
		}
		else if (retVer < version) {
			//	(2) アプリで扱えるバージョンより、テーブルバージョンが古い

// midori UC_0010 del -->
//			// テーブル作成、データ登録を実行する
//// midori UC_0006 del -->
//			//BOOL bRet = CheckAndMakeTable(szSQLServer, m_DatabaseName, (m_strPath + DB_STR_SQLFILE_UPDATE), DB_STR_SQLUSE_UPDATE, SQL_EXEC_UPDPATE);
//
//			//if (bRet == FALSE) {
//			//	SetExitMessage( m_intCoSelType , "勘定科目内訳書マスター更新に失敗しました。" );
//			//	return 0;
//			//}
//// midori UC_0006 del <--
//// midori UC_0006 add -->
//			int nRet = CheckAndMakeTable(szSQLServer, m_DatabaseName, (m_strPath + DB_STR_SQLFILE_UPDATE), DB_STR_SQLUSE_UPDATE, SQL_EXEC_UPDPATE);
//
//			if (nRet == -1) {
//				SetExitMessage( m_intCoSelType , "勘定科目内訳書マスター更新に失敗しました。" );
//				return(0);
//			}
//			else if(nRet == -2)	{
//				return(0);
//			}
//// midori UC_0006 add <--
//
//			// 2007.08.24 テーブル更新SQL実装により、メッセージの表示は不要
//			//SetExitMessage( m_intCoSelType ,  "勘定科目内訳書マスターが古いバージョンです。\r\n勘定科目内訳書マスターを更新して下さい。"  );
//			//return FALSE;
// midori UC_0010 del <--
// midori UC_0010 add -->
			int nRet = VersionUp(m_pZmSub,szSQLServer,atoi(szApno),atoi(szCoCode),atoi(szKikan),m_intCoSelType);
			if(nRet == -1)	{
				// アプリ起動時の場合は業務終了
				if(m_intCoSelType == 0)	{
					// 終了メッセージ送信
					AfxGetMainWnd()->PostMessage(WM_CLOSE,1,0);
				}
				return(0);
			}
			else if(nRet == -2)	{
				return(0);
			}
// midori UC_0010 add <--
		}
		else if (retVer > version) {
			//	(3) アプリで扱えるバージョンより、テーブルバージョンが新しい
			SetExitMessage( m_intCoSelType ,   "勘定科目内訳書マスターが更新されています。\r\nアプリケーションをバージョンアップして下さい。"  );
			return 0;
		}
	}

// midori UC_0010 del -->
//// midori UC_0006 add -->
//	if(CheckTable() == -1)	{
//		return(0);
//	}
//// midori UC_0006 add <--
// midori UC_0010 del <--

// 2010.04.19 Append start
// 内訳書テーブル作成サインON ･･･ 内訳書電子申告で"sub_sw=0"でエラーが発生の対応
	if ( m_pZmSub->VolumeOpen() == 0 ) {
// midori 181002_2 del -->
//// midori 181002 del -->
//		//// 内訳書アプリ起動時に"sub_sw"を設定し直す
//		//if (m_pZmSub->zvol->sub_sw & DB_UC_TABLE_CREATE_ON) {
//		//}
//		//else {
//		//	m_pZmSub->zvol->Edit();
//		//	m_pZmSub->zvol->sub_sw |= DB_UC_TABLE_CREATE_ON;
//		//	m_pZmSub->zvol->Update();
//		//}
//// midori 181002 del <--
//// midori 181002 add -->
//		if(m_swUtwInfoRegister == 0) {
//			if((m_pZmSub->zvol->sub_sw & DB_UC_TABLE_CREATE_ON) != DB_UC_TABLE_CREATE_ON) {
//				m_pZmSub->zvol->Edit();
//				m_pZmSub->zvol->sub_sw |= DB_UC_TABLE_CREATE_ON;
//				m_pZmSub->zvol->Update();
//			}
//		}
//		else {
//			if((m_pZmSub->zvol->sub_sw & DB_UC_TABLE_CREATE_ON_T) != DB_UC_TABLE_CREATE_ON_T) {
//				m_pZmSub->zvol->Edit();
//				m_pZmSub->zvol->sub_sw |= DB_UC_TABLE_CREATE_ON_T;
//				m_pZmSub->zvol->Update();
//			}
//		}
//// midori 181002 add <--
// midori 181002_2 del <--
// midori 181002_2 add -->
// midori UC_0037 del -->
		//if(m_swUtwInfoRegister == 0) {
// midori UC_0037 del <--
// midori UC_0037 add -->
		if(m_swUtwTandoku == 0) {
// midori UC_0037 add <--
			if((m_pZmSub->zvol->sub_sw & DB_UC_TABLE_CREATE_ON) != DB_UC_TABLE_CREATE_ON) {
				m_pZmSub->zvol->Edit();
				m_pZmSub->zvol->sub_sw |= DB_UC_TABLE_CREATE_ON;
				m_pZmSub->zvol->Update();
			}
		}
		else {
			if((m_pZmSub->zvol->sub_sw & DB_UC_TABLE_CREATE_ON) != DB_UC_TABLE_CREATE_ON || 
				(m_pZmSub->zvol->sub_sw & DB_UC_TABLE_CREATE_ON_T) != DB_UC_TABLE_CREATE_ON_T) {
				m_pZmSub->zvol->Edit();
				m_pZmSub->zvol->sub_sw |= DB_UC_TABLE_CREATE_ON;
				m_pZmSub->zvol->sub_sw |= DB_UC_TABLE_CREATE_ON_T;
				m_pZmSub->zvol->Update();
			}
		}
// midori UC_0035 add -->
		// corec::c_subswのD5(0x20)は、連動・非連動にかかわらず常にONにする
		if(SetCoRec2(m_pZmSub->zvol->apno,m_pZmSub->zvol->v_cod,m_pZmSub->zvol->ee_ymd,1,&cst) == -1) {
			ICSMessageBox(cst);
			return(-1);
		}
// midori UC_0035 add <--
// midori 181002_2 add <--

// 24/01/30_23-0845 add -->
		m_CloudSansyoFlg = IsCloudSansyoMode(m_pZmSub->zvol->apno,m_pZmSub->zvol->v_cod,m_pZmSub->zvol->ee_ymd);
// 24/01/30_23-0845 add <--

		m_pZmSub->VolumeClose();
	}
	else	{
		m_clsFunc.ZmSubErrMsg(m_pZmSub);
		return 0;
	}
// 2010.04.19 Append end

	//	(4) アプリで扱えるバージョンと、テーブルバージョンが同じ
	return 1;
}

//**************************************************
//	アプリケーションの開始（前回終了時に選択された様式を起動）
//	【引数】	intCoSelType　…　会社選択タイプ ( 0:会社起動時　1:会社選択ツールバーからの切り替え時　2:業務ツールバーからの会社切り替え時)
//	【戻値】	なし
//**************************************************
// アプリケーション開始
int CUCHIWAKEApp::StartApp( int intCoSelType )
{
	// 2010/07/XX 一括印刷対応 ログ追加
	const CString	FN = _T("CUCHIWAKEApp::StartApp()");
	CString			strLog;
	DbgWriteLog( FN, _T("START"));


	BOOL			bCoSel = FALSE;			// TRUE:会社切替から実行、FALSE:通常起動
	int				intTemp;
// midori 155314 add -->
	int				rv=0;
// midori 155314 add <--

	// 会社選択タイプを取得
	m_intCoSelType = intCoSelType;

	// 会社切替から呼ばれた場合、既にOpen済みなので、
	// DocTemplate登録不要＋Open済みViewの"m_nID"更新が必要
	if (m_intCoSelType > 0) {
		bCoSel = TRUE;
	}

	//-------------------------------
	//	(0)アプリ実行フォルダのパスを取得
	//-------------------------------
#ifdef _DEBUG

	// Debugの場合は、相対パスを作成
	//m_strPath.Empty();
	//m_strPath = _T( ".\\" );

	char szDevice[4];	// ドライブ文字
	char szGrNum[8];	// GR番号

	m_strPath.Empty();
	ZeroMemory( szDevice, sizeof( szDevice ) );
	ZeroMemory( szGrNum, sizeof( szGrNum ) );

// midori 161113 del -->
//	PCOM_GetString( "ProgramDevice", szDevice );
// midori 161113 del <--
// midori 161113 add -->
	GCOM_GetString( "SystemDrive", szDevice );
// midori 161113 add <--
	PCOM_GetString( "ExecGrJobs", szGrNum );

	m_strPath.Format( "%s:\\ICSWin\\SYS\\GR%s\\", szDevice, szGrNum );

#else

	// Releaseの場合は、絶対パスを作成
	char szDevice[4];	// ドライブ文字
	char szGrNum[8];	// GR番号

	m_strPath.Empty();
	ZeroMemory( szDevice, sizeof( szDevice ) );
	ZeroMemory( szGrNum, sizeof( szGrNum ) );

// midori 161113 del -->
//	PCOM_GetString( "ProgramDevice", szDevice );
// midori 161113 del <--
// midori 161113 add -->
	GCOM_GetString( "SystemDrive", szDevice );
// midori 161113 add <--
	PCOM_GetString( "ExecGrJobs", szGrNum );

	m_strPath.Format( "%s:\\ICSWin\\SYS\\GR%s\\", szDevice, szGrNum );


#endif

#ifdef _DEBUG
//QQQQQQQQ 一括印刷詳細設定
//UCOM_SetString(_T("呼出プログラム"),_T("一括印刷"));
//UCOM_SetString(_T("依頼プログラム"),_T("勘定科目内訳書"),_T("0"));
//UCOM_SetString(_T("依頼項目"),_T("詳細設定"));
//UCOM_SetString(_T("依頼キー"),_T("DBUchiwake1b0001"));
//UCOM_SetString(_T("設定年月日"),_T("-1"),_T("-1"),_T("-1"),_T("-1"));

//QQQQQQQQ 一括印刷印刷
//UCOM_SetString(_T("呼出プログラム"),_T("一括印刷"));
//UCOM_SetString(_T("依頼プログラム"),_T("勘定科目内訳書"),_T("0"));
//UCOM_SetString(_T("依頼項目"),_T("印刷"));
//UCOM_SetString(_T("依頼キー"),_T("DBUchiwake1b0001"));
////UCOM_SetString(_T("依頼詳細"),_T("1"),_T("\\t-1\\EA8"));
//UCOM_SetString(_T("依頼詳細"),_T("1"),_T("iR C2880"));
//UCOM_SetString(_T("出力年月日"),_T("-1"),_T("-1"),_T("-1"),_T("-1"));
#endif

	//-----------------------------------------------------
	// 一括印刷 UCOM 情報取得
	//-----------------------------------------------------
	// 2010/07/XX 一括印刷対応
	if ( FALSE == g_BatchPrt.GetUcomInf() ) {
		// エラー処理
		ICSExitEx(_T("一括印刷情報の取得に失敗しました"));
		return 0;
	}

	//-------------------------------
	//	(1)会社選択
	//-------------------------------
// midori 180104 del -->
	//int nRet = CorporationSelect();
// midori 180104 del <--
// midori 180104 add -->
	int nRet = CorporationSelect(intCoSelType);
// midori 180104 add <--
	if( nRet <= 0 ){
		return nRet;
	}

// インボイス登録番号追加対応_23/11/06 add -->
	// インボイス登録番号カラム拡張
	if(InvNoColumnConvert(m_pDB) != 0)	{
		return(0);
	}
// インボイス登録番号追加対応_23/11/06 add <--

// 改良No.21-0086,21-0529 add -->
	// 様式⑧支払手形の内訳書の科目入力追加、「電子記録○○」等科目追加に関するカラム拡張
	if(KamokuAddColumnConvert1(m_pDB) != 0)	{
		return(0);
	}
// 改良No.21-0086,21-0529 add <--

// midori 190301 add -->
	// 決算期間から簡素化の画面で起動するかどうかを判定する
	KansoCheck();
// midori 190301 add <--

// インボイス登録番号追加対応_23/11/06 add -->
	// 決算期間から登録番号追加版の様式で起動するかどうかを判定する
	InvoNoCheck();
// インボイス登録番号追加対応_23/11/06 add <--

// 20-0450 add -->
	// ⑥有価証券の内訳書　ソート項目の「種類・銘柄」を分割または結合する
	if(SyuruiMeigaraConvert(1,bG_Kanso) != 0)	{
		return(0);
	}
// 20-0450 add <--

// 改良No.21-0086,21-0529 add -->
	// 様式⑧支払手形の内訳書の科目入力追加、「電子記録○○」等科目追加に関するデータ変更等
	if(KamokuAddColumnConvert2(m_pDB) != 0)	{
		return(0);
	}
// 改良No.21-0086,21-0529 add <--

// midori UC_0060 add -->
	CdbUcInfMain	mfcRec( m_pDB );			//	uc_inf_mainテーブルクラス
	//  旧様式のマスターへ新様式の省略文字をコピーした状態で修復を行わないまま
	//  KansoCheckで旧様式から新様式への書き換えが行われ省略文字が破損したマスターに対する修復
	if(mfcRec.Init() == DB_ERR_OK)	{
		if ( !( mfcRec.IsEOF() ) ){
			CCtrlMainGeneralVar cm1(mfcRec);
			if(cm1.bOmitUpd2 == FALSE)	{
				// 新様式の場合のみ、修復処理②を実行する
				if(bG_Kanso == TRUE)	{
					if(KansoOmitRepair(m_pZmSub->m_database) == FALSE)	{
						return(0);
					}
				}
				// 修復済みのサインを立てる
				mfcRec.Edit();
				cm1.bOmitUpd2 = TRUE;
				cm1.Set(mfcRec);
				mfcRec.Update();
				// 元に戻す
				mfcRec.Requery();
			}
// 24/01/30_23-0845 add -->
			// 参照モードの場合は入力確定フラグを立てる
			if(m_CloudSansyoFlg == TRUE && cm1.bDataKakutei == FALSE)	{
				mfcRec.Edit();
				cm1.bDataKakutei = TRUE;
				cm1.Set(mfcRec);
				mfcRec.Update();
				mfcRec.Requery();
				m_CloudSansyoFlg2 = TRUE;
			}
// 24/01/30_23-0845 add <--
		}
	}
	else	{
		return(0);
	}
// midori UC_0060 add <--

	//-------------------------------
	//	(2)ドキュメントテンプレート初期化
	//-------------------------------
	if (bCoSel == FALSE) {
		// 会社切替時は既にテンプレート登録済みのため不要
		if( InitDocTemplate() != TRUE ){
			return 0;
		}
	}

	//-------------------------------
	//	(3)uc_inf_main取得
	//-------------------------------
	int				nFormSeq = ID_FORMNO_011;	//	シーケンス番号
	BYTE			bFgUchiConv = 0;			//	内訳書コンバージョン実行フラグ
	long			lngSSymd = 0;				//	決算期間（期首）
	long			lngEEymd = 0;				//	決算期間（期末）
// midori UC_0060 del -->
	//CdbUcInfMain	mfcRec( m_pDB );			//	uc_inf_mainテーブルクラス
// midori UC_0060 del <--

	//	初期化成功？
	if ( mfcRec.Init() == DB_ERR_OK ){
		//	レコードあり？
		if ( !( mfcRec.IsEOF() ) ){
			mfcRec.MoveFirst();					//	レコードを先頭に移動
			nFormSeq = mfcRec.m_FormSeq;		//	シーケンス番号を取得
			bFgUchiConv = mfcRec.m_FgUchiConv;	//	内訳書コンバージョン実行フラグを取得
			lngSSymd = mfcRec.m_ssymd;			//	決算期間（期首）
			lngEEymd = mfcRec.m_eeymd;			//	決算期間（期末）
		}
	}
	else{
		return 0;
	}

// midori UC_0030 del -->
//// midori 170503 add -->
//	// 各様式のNumPage,NumRowのチェック、修正
//	NumRowCheck();
//// midori 170503 add <--
//
//// midori 153790 add -->
//	OrderCheck();
//// midori 153790 add <--
// midori UC_0030 del <--

	//-------------------------------
	//	(4)内訳書コンバージョン後処理
	//-------------------------------
	if (bFgUchiConv == 1) {
		// 後処理実行
		UchiConvUpdateMainProc();
	}

// midori 190504 add -->
	if(DeleteSpaceMaster(m_pZmSub->m_database) == -1) {
		return (0);
	}
// midori 190504 add <--

// midori UC_0030 add -->
	// 各様式のNumPage,NumRowのチェック、修正
	NumRowCheck();
// midori UC_0038 del -->
	//// 科目、銀行、取引先参照の番号が正しく連番になっているかチェック、修正
	//OrderCheck();
// midori UC_0038 del <--
// midori UC_0030 add <--

// midori UC_0060 del -->
//// midori 155314 add -->
//	// 旧様式から新様式に印刷設定の省略文字がコピーされている
//	// または新様式から旧様式に印刷設定の省略文字がコピーされている場合、
//	// 一度だけ省略文字の修復処理を行う
//	mfcRec.Requery();	// DeleteSpaceMaster で変更される場合があるので再取得
//	if ( !( mfcRec.IsEOF() ) ){
//		CCtrlMainGeneralVar cm1(mfcRec);
//		if(cm1.bOmitUpd == FALSE) {
//			rv = ChkOmit(m_pZmSub->m_database,bG_Kanso);
//			if(rv == -1) {
//				// エラー
//				return(0);
//			}
//			// 自動修復が必要
//			if(rv == 1) {
//				// 新様式
//				if(bG_Kanso == TRUE)	KansoOmitConvert(TRUE);
//				// 旧様式
//				else					KansoOmitConvert(FALSE);
//			}
//			// 修復済みのサインを立てる
//			mfcRec.Edit();
//			cm1.bOmitUpd = TRUE;
//			cm1.Set(mfcRec);
//			mfcRec.Update();
//			// 元に戻す
//			mfcRec.Requery();
//		}
//	}
//// midori 155314 add <--
// midori UC_0060 del <--

// midori 191002 add -->
	KeiStrConvert(m_pDB);
// midori 191002 add <--

// 158138 add -->
	if(Convert210701(m_pDB) == FALSE)	{
		return(0);
	}
// 158138 add <--
// 22-0003_220324 add -->
	if(Convert220324(m_pDB) == FALSE)	{
		return(0);
	}
// 22-0003_220324 add <--

	//-------------------------------
	//	(5)翌期更新時のバグメンテナンス
	//  一度だけ起動
	//-------------------------------
	 MaintenanceHokanData();

// midori 152710 add -->
// ツールバー会社切替の場合、UpdateViewDBInfo()関数を呼ばずに翌期更新ダイアログを表示すると
// システムエラーがでます
	if(bCoSel != FALSE) {
		// ツールバー会社切替の場合、既にオープン済みのため画面切替を行う
		UpdateViewDBInfo();		// オープン済みViewのDB情報を更新する
	}
// midori 152710 add <--

// midori 180104 add -->
	// 顧問先情報の取り込み
	if(IsKanendoMaster() == FALSE)	{
		BeginWaitCursor();
		if( m_clsFunc.SetCpdataByCpinfo(0,this->m_pMainWnd) == -1 ){
			EndWaitCursor();
			return(0);
		}
		EndWaitCursor();
	}
// midori 180104 add <--

	//-------------------------------
	//	(6)翌期更新（ここで実行？）
	//-------------------------------
// midori UC_0048 del -->
//// midori UC_0039 del -->
////// midori 180503 del -->
////	//if ( UchiYokukiUpdateMainProc(lngSSymd, lngEEymd) != FUNCTION_OK ){
////// midori 180503 del <--
////// midori 180503 add -->
////	if(UchiYokukiUpdateMainProc(this->m_pMainWnd,m_pZmSub,lngSSymd,lngEEymd) != FUNCTION_OK)	{
////// midori 180503 add <--
//// midori UC_0039 del <--
//// midori UC_0039 add -->
//	if(UchiYokukiUpdateMainProc(this->m_pMainWnd,m_pZmSub,lngSSymd,lngEEymd,m_swUtwInfoRegister) != FUNCTION_OK)	{
//// midori UC_0039 add <--
// midori UC_0048 del <--
// midori UC_0048 add -->
	if(UchiYokukiUpdateMainProc(this->m_pMainWnd,m_pZmSub,lngSSymd,lngEEymd) != FUNCTION_OK)	{
// midori UC_0048 add <--
		// 翌期更新ダイアログにて「キャンセル」の場合
		SetExitMessage( m_intCoSelType ,  "処理を終了します。" );

		return 0;
	}
// midori 160612 add -->
	else	{
		// 翌期更新で「入力データを確定する」が変更されるので再取得
		mfcRec.Requery();
	}
// midori 160612 add <--

// midori 190301 add -->
	// 翌期更新後に期首が平成31年4月1日以降になる場合は新様式となるためメッセージを表示する
	KansoMessage();
// midori 190301 add <--

// midori 156188 add -->
	KamokuRowConvert(m_pZmSub->m_database);
// midori 156188 add <--

	//-------------------------------
	//	(7)前回の帳表をオープン
	//		通常は、オープン処理を行う
	//		ツールバー会社切替の場合、既にオープンされている事を考慮して切替処理を実行する
	//-------------------------------
	if (bCoSel == FALSE) {
		// 通常はオープン処理を行う
		OpenDocument( nFormSeq );
	}
	else {
		// ツールバー会社切替の場合、既にオープン済みのため画面切替を行う
// midori 152710 del -->
		//UpdateViewDBInfo();								// オープン済みViewのDB情報を更新する
// midori 152710 del <--
		intTemp = ChangeDocument( nFormSeq );			// 画面切替

		// ツールバー会社切替の場合、画面切替の再実行用にFormOrderを保存する
		if (intTemp != -1) {
			m_intCoSelFormOrder = intTemp;
		}
	}

	//-------------------------------
	//	(8)テーブルバージョン更新後、一度だけ行う処理
	//-------------------------------
	CFrameWnd*			pFrame	= (CFrameWnd*)AfxGetMainWnd();
	CMDIChildWnd*		pChild	= (CMDIChildWnd *)pFrame->GetActiveFrame();
	CfrmUc000Common*	frmView	= (CfrmUc000Common*)pChild->GetActiveView();
	if( frmView != NULL ){
		// 金融機関、科目、取引先（得意先）、取引先（仕入先）にデフォルトの振り仮名を振る
		frmView->CreateReferenceDlgKana();

		// 「金額０円データ及び空欄データを表示しない」コピー ＆④－２、⑪、⑯－１、⑯－２のAdSeq,AdOrder更新
		CString strErr;
		int st = frmView->UpdateVer1_0_2_1(strErr);
		if(st == FALSE){
			SetExitMessage(m_intCoSelType ,  strErr);
			return 0;
		}
	}

	//-------------------------------
	//	(9)帳表間で共通の情報取得
	//-------------------------------
	if(frmView != NULL){
		CCtrlMainGeneralVar cm(mfcRec);
		frmView->GetCommonInfo(cm);
	}

// 改良No.22-0811 add -->
	// メニューバー「財務連動」の制御
	frmView->CmnMenuEnableSwitch();
// 改良No.22-0811 add <--

	//	レコード閉じる
	mfcRec.Fin();

// midori 160612 add -->
	frmView->CmnChangeButtonEnable(2);

// midori 152766 del -->
//	AfxGetMainWnd()->PostMessage( WM_USER_UPDATE_INDICATE, 0 );
// midori 152766 del <--
// midori 152766 add -->
	AfxGetMainWnd()->PostMessage( WM_USER_UPDATE_INDICATE, 1 );
// midori 152766 add <--
// midori 160612 add <--
// midori 160607 add -->
	// オプションの設定(データのある様式へ移動する)を取得した後
	// 「前様式」「次様式」ボタンの制御を行う
	frmView->UpdateYoushikiMoveButton2();
// midori 160607 add <--

// midori 180101 add -->
	//-------------------------------
	//	表紙 決算期の確認
	//-------------------------------
	if(KessankiCheck() != 0)	{
		return 0;
	}
// midori 180101 add <--

// midori 157043 del -->
//// midori 190505 add -->
//	//-------------------------------
//	//	印刷設定
//	//-------------------------------
//	// 科目行を出力するサインが立っている場合、メッセージを表示
//	frmView->KamokuRowChk();
//// midori 190505 add <--
// midori 157043 del <--

	//-------------------------------
	//	(10)終了フラグを落とす（会社切替）
	//-------------------------------
	if( frmView != NULL ){
		frmView->SetEndViewFlag( FALSE );
	}

// 修正No.168393 add -->
	// 初回フォーカスセット
	if(bCoSel == FALSE) {
		frmView->CmnDiagSetFocus(frmView->m_pDiag, frmView->m_uInfo.intCurRow, frmView->m_uInfo.intCurCol);
	}
// 修正No.168393 add <--

	return 1;
}

/*
// 会社選択暫定版
void CUCHIWAKEApp::StartApp()
{
	int				nFormSeq = ID_FORMNO_011;	//	シーケンス番号
	CdbUcInfMain	mfcRec( m_pDB );			//	uc_inf_mainテーブルクラス

	try{
		
		//	初期化成功？
		if ( mfcRec.Init() == DB_ERR_OK ){
			//	レコードあり？
			if ( !( mfcRec.IsEOF() ) ){
				mfcRec.MoveFirst();				//	レコードを先頭に移動
				nFormSeq = mfcRec.m_FormSeq;	//	シーケンス番号を取得
			}
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
	catch(...){

		//	レコード開いてる？
		if ( mfcRec.IsOpen() ){
			//	レコード閉じる
			mfcRec.Fin();
		}
	}
	
	//	ドキュメントを開く
	OpenDocument( nFormSeq );
}
*/

//**************************************************
//	ドキュメントテンプレート初期化
//	【引数】	なし
//	【戻値】	TRUE	…	初期化成功
//				FALSE	…	失敗
//**************************************************
// midori 190301 del -->
//BOOL CUCHIWAKEApp::InitDocTemplate()
//{
//	int					nCnt;				//	カウント用
//	CdbUcInfSub			mfcRec( m_pDB );	//	uc_inf_subテーブルクラス
//
//	//	ドキュメント管理情報の初期化
//	for( nCnt = 0; nCnt < UC_ID_FORMNO_MAX; nCnt++ ){
//		m_uDocInfo[nCnt].nID		 = -1;			// ドキュメントテンプレート管理ID（-1:未登録）
//		m_uDocInfo[nCnt].bOpenFlg = FALSE;			// ドキュメントオープン状態　　　（FALSE:未オープン）
//	}
//
//	//	様式数分ループ
//	for( nCnt = 0; nCnt < UC_ID_FORMNO_MAX; nCnt++ ){
//		
//		//	FormOrderで検索
//		if ( mfcRec.RequeryFormOrder( nCnt + 1 ) == DB_ERR_OK ){
//			//	レコードあり？
//			if ( !mfcRec.IsEOF() ){
//				//	先頭行に移動
//				mfcRec.MoveFirst(); 
//				//	ドキュメントテンプレート設定
//				SetDocTemplate( nCnt, mfcRec.m_FormSeq );
//			}
//			//	閉じる
//			mfcRec.Fin();
//		}
//		else{
//			return FALSE;
//		}
//	}
//
//	return TRUE;
//}
// midori 190301 del <--
// midori 190301 add -->
BOOL CUCHIWAKEApp::InitDocTemplate()
{
	int					nCnt=0;
	int					nMax=0;
	int					nTmp=0;
	CdbUcInfSub			mfcRec(m_pDB);

	// ドキュメント管理情報の初期化
	//for(nCnt=0; nCnt<UC_ID_FORMNO_MAX_KANSO; nCnt++)	{		// インボイス登録番号追加対応_23/09/11 del
	for(nCnt=0; nCnt<UC_ID_FORMNO_MAX_INVO; nCnt++)	{			// インボイス登録番号追加対応_23/09/11 add
		m_uDocInfo[nCnt].nID		= -1;		// ドキュメントテンプレート管理ID（-1:未登録）
		m_uDocInfo[nCnt].bOpenFlg	= FALSE;	// ドキュメントオープン状態　　　（FALSE:未オープン）
	}

	// 様式数分ループ
	//nMax = UC_ID_FORMNO_MAX_KANSO;	// 84	// インボイス登録番号追加対応_23/09/11 del
	nMax = UC_ID_FORMNO_MAX_INVO;		// 186	// インボイス登録番号追加対応_23/09/11 add
	for(nCnt=0; nCnt<nMax; nCnt++)	{
		// FormOrderで検索
// インボイス登録番号追加対応_23/09/11 del -->
		//if(nCnt >= UC_ID_FORMNO_MAX)	nTmp = nCnt - UC_ID_FORMNO_MAX;
		//else							nTmp = nCnt;
// インボイス登録番号追加対応_23/09/11 del <--
// インボイス登録番号追加対応_23/09/11 add -->
		if(nCnt >= UC_ID_FORMNO_MAX_KANSO)	nTmp = nCnt - UC_ID_FORMNO_MAX_KANSO;
		else if(nCnt >= UC_ID_FORMNO_MAX)	nTmp = nCnt - UC_ID_FORMNO_MAX;
		else								nTmp = nCnt;
// インボイス登録番号追加対応_23/09/11 add <--

		if(mfcRec.RequeryFormOrder(nTmp + 1) == DB_ERR_OK)	{
			// レコードあり？
			if(!mfcRec.IsEOF())	{
				// 先頭行に移動
				mfcRec.MoveFirst(); 
				// ドキュメントテンプレート設定
				SetDocTemplate(nCnt,mfcRec.m_FormSeq);
			}
			// 閉じる
			mfcRec.Fin();
		}
		else	{
			return(FALSE);
		}
	}
	return(TRUE);
}
// midori 190301 add <--

//**************************************************
//	ドキュメントテンプレート設定
//	【引数】	nCnt		…	カウント
//				nFormSeq	…	シーケンス番号
//	【戻値】	なし
//**************************************************
// midori 190301 del -->
//void CUCHIWAKEApp::SetDocTemplate( int nCnt, int nFormSeq )
//{
//	CRuntimeClass*		pView;				//	CRuntimeClassポインタ
//
//	//	初期化
//	pView = 0;
//	
//	//	様式で分岐
//	switch( nFormSeq ){
//	case ID_FORMNO_011:	//	①.預貯金等
//		pView = RUNTIME_CLASS( CfrmUc011Yotyokin );
//		break;
//	case ID_FORMNO_021:		
//		pView = RUNTIME_CLASS( CfrmUc021Uketoritegata );
//		break;
//	case ID_FORMNO_031:	//	③.売掛金
//		pView = RUNTIME_CLASS( CfrmUc031Urikakekin );
//		break;
//	case ID_FORMNO_041:	//	④-1.仮払金
//		pView = RUNTIME_CLASS( CfrmUc041Karibarai );
//		break;
//	case ID_FORMNO_042:
//		pView = RUNTIME_CLASS( CfrmUc042Kasituke );
//		break;
//	case ID_FORMNO_051:	//	⑤.棚卸資産
//		pView = RUNTIME_CLASS( CfrmUc051Tanaorosi );
//		break;
//	case ID_FORMNO_061:	//	⑥.有価証券
//		pView = RUNTIME_CLASS( CfrmUc061Yuukasyouken );
//		break;
//	case ID_FORMNO_071:	//	⑦.固定資産
//		pView = RUNTIME_CLASS( CfrmUc071Koteisisan );
//		break;
//	case ID_FORMNO_081:
//		pView = RUNTIME_CLASS( CfrmUc081Siharaitegata );
//		break;
//	case ID_FORMNO_091:	//	⑨.買掛金
//		pView = RUNTIME_CLASS( CfrmUc091Kaikakekin );
//		break;
//	case ID_FORMNO_101:	//	⑩-1.仮受金
//		pView = RUNTIME_CLASS( CfrmUc101Kariukekin );
//		break;
//	case ID_FORMNO_102:
//		pView = RUNTIME_CLASS( CfrmUc102Gensen );
//		break;
//	case ID_FORMNO_111:
//		pView = RUNTIME_CLASS( CfrmUc111Kariirekin );
//		break;
//	case ID_FORMNO_121:
//		pView = RUNTIME_CLASS( CfrmUc121Totiuriage );
//		break;
//	case ID_FORMNO_131:
//		pView = RUNTIME_CLASS( CfrmUc131Uriagedaka );
//		break;
//	case ID_FORMNO_141:	//	⑭-1.役員報酬手当金等
//		pView = RUNTIME_CLASS( CfrmUc141Yakuin );
//		break;
//	case ID_FORMNO_142:	//	⑭-2.人件費等
//		pView = RUNTIME_CLASS( CfrmUc142Jinkenhi );
//		break;
//	case ID_FORMNO_151:
//		pView = RUNTIME_CLASS( CfrmUc151Tidaiyatin );
//		break;
//	case ID_FORMNO_152:
//		pView = RUNTIME_CLASS( CfrmUc152Kenrikin );
//		break;
//	case ID_FORMNO_153:
//		pView = RUNTIME_CLASS( CfrmUc153Kougyou );
//		break;
//	case ID_FORMNO_161:
//		pView = RUNTIME_CLASS( CfrmUc161Zatueki );
//		break;
//	case ID_FORMNO_162:
//		pView = RUNTIME_CLASS( CfrmUc162Zatuson );
//		break;
//	case ID_FORMNO_171:
//	case ID_FORMNO_172:
//	case ID_FORMNO_173:
//	case ID_FORMNO_174:
//	case ID_FORMNO_175:
//	case ID_FORMNO_176:
//	case ID_FORMNO_177:
//	case ID_FORMNO_178:
//	case ID_FORMNO_179:
//	case ID_FORMNO_1710:
//	case ID_FORMNO_1711:
//	case ID_FORMNO_1712:
//	case ID_FORMNO_1713:
//	case ID_FORMNO_1714:
//	case ID_FORMNO_1715:
//	case ID_FORMNO_1716:
//	case ID_FORMNO_1717:
//	case ID_FORMNO_1718:
//	case ID_FORMNO_1719:
//	case ID_FORMNO_1720:
//		pView = RUNTIME_CLASS( CfrmUc171Sonota1 );
//		break;
//	case ID_FORMNO_181:
//	case ID_FORMNO_182:
//	case ID_FORMNO_183:
//	case ID_FORMNO_184:
//	case ID_FORMNO_185:
//	case ID_FORMNO_186:
//	case ID_FORMNO_187:
//	case ID_FORMNO_188:
//	case ID_FORMNO_189:
//	case ID_FORMNO_1810:
//	case ID_FORMNO_1811:
//	case ID_FORMNO_1812:
//	case ID_FORMNO_1813:
//	case ID_FORMNO_1814:
//	case ID_FORMNO_1815:
//	case ID_FORMNO_1816:
//	case ID_FORMNO_1817:
//	case ID_FORMNO_1818:
//	case ID_FORMNO_1819:
//	case ID_FORMNO_1820:
//		pView = RUNTIME_CLASS( CfrmUc181Sonota2 );
//		break;
//	}
//	
//	//	ポインタ取得済み？
//	if ( pView != 0 ){
//
//		//	ドキュメントテンプレート作成
//		m_uDocInfo[nCnt].nID = CreateDocTemplate( IDR_UCHIWAKETYPE,
//												  RUNTIME_CLASS(CUCHIWAKEDoc),
//												  RUNTIME_CLASS(CChildFrame),
//												  pView );
//	}
//}
// midori 190301 del <--

// インボイス登録番号追加対応_23/09/11 del -->
//// midori 190301 add -->
//void CUCHIWAKEApp::SetDocTemplate( int nCnt, int nFormSeq )
//{
//	CRuntimeClass*		pView;				//	CRuntimeClassポインタ
//	BOOL				bKanso=FALSE;
//
//	// カウントが62以上であれば、簡素化の様式を作成する
//	if(nCnt >= UC_ID_FORMNO_MAX)	{
//		bKanso=TRUE;
//	}
//	// カウントが61以下であれば、カウントとマスターの状態から判断する
//	else	{
//		bKanso=FALSE;
//	}
//
//	//	初期化
//	pView = 0;
//	
//	//	様式で分岐
//	switch( nFormSeq ){
//	case ID_FORMNO_011:	//	①.預貯金等
//		if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc011YotyokinK);
//		else				pView = RUNTIME_CLASS(CfrmUc011Yotyokin);
//		break;
//	case ID_FORMNO_021:	//	②.受取手形
//		if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc021UketoritegataK);
//		else				pView = RUNTIME_CLASS(CfrmUc021Uketoritegata);
//		break;
//	case ID_FORMNO_031:	//	③.売掛金
//		if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc031Urikakekin);
//		else				pView = RUNTIME_CLASS(CfrmUc031Urikakekin);
//		break;
//	case ID_FORMNO_041:	//	④-1.仮払金
//		if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc041Karibarai);
//		else				pView = RUNTIME_CLASS(CfrmUc041Karibarai);
//		break;
//	case ID_FORMNO_042:	//	④-2.貸付金及び受取利息
//		if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc042KasitukeK);
//		else				pView = RUNTIME_CLASS(CfrmUc042Kasituke);
//		break;
//	case ID_FORMNO_051:	//	⑤.棚卸資産
//		if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc051Tanaorosi);
//		else				pView = RUNTIME_CLASS(CfrmUc051Tanaorosi);
//		break;
//	case ID_FORMNO_061:	//	⑥.有価証券
//		if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc061YuukasyoukenK);
//		else				pView = RUNTIME_CLASS(CfrmUc061Yuukasyouken);
//		break;
//	case ID_FORMNO_071:	//	⑦.固定資産
//		if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc071Koteisisan);
//		else				pView = RUNTIME_CLASS(CfrmUc071Koteisisan);
//		break;
//	case ID_FORMNO_081:	//	⑧.支払手形
//		if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc081SiharaitegataK);
//		else				pView = RUNTIME_CLASS(CfrmUc081Siharaitegata);
//		break;
//	case ID_FORMNO_091:	//	⑨.買掛金
//		if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc091Kaikakekin);
//		else				pView = RUNTIME_CLASS(CfrmUc091Kaikakekin);
//		break;
//	case ID_FORMNO_101:	//	⑩-1.仮受金
//		if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc101Kariukekin);
//		else				pView = RUNTIME_CLASS(CfrmUc101Kariukekin);
//		break;
//	case ID_FORMNO_102:	//	⑩-2.源泉所得税預り金
//		if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc102Gensen);
//		else				pView = RUNTIME_CLASS(CfrmUc102Gensen);
//		break;
//	case ID_FORMNO_111:	//	⑪借入金及び支払利子
//		if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc111KariirekinK);
//		else				pView = RUNTIME_CLASS(CfrmUc111Kariirekin);
//		break;
//	case ID_FORMNO_121:	//	⑫土地の売上高等
//		if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc121Totiuriage);
//		else				pView = RUNTIME_CLASS(CfrmUc121Totiuriage);
//		break;
//	case ID_FORMNO_131:	//	⑬売上高等の事業所別
//		if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc131UriagedakaK);
//		else				pView = RUNTIME_CLASS(CfrmUc131Uriagedaka);
//		break;
//	case ID_FORMNO_141:	//	⑭-1.役員報酬手当金等
//		if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc141YakuinK);
//		else				pView = RUNTIME_CLASS(CfrmUc141Yakuin);
//		break;
//	case ID_FORMNO_142:	//	⑭-2.人件費等
//		if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc142Jinkenhi);
//		else				pView = RUNTIME_CLASS(CfrmUc142Jinkenhi);
//		break;
//	case ID_FORMNO_151:	//	⑮-1.地代家賃等
//		if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc151Tidaiyatin);
//		else				pView = RUNTIME_CLASS(CfrmUc151Tidaiyatin);
//		break;
//	case ID_FORMNO_152:	//	⑮-2.権利金等の期中支払
//		if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc152Kenrikin);
//		else				pView = RUNTIME_CLASS(CfrmUc152Kenrikin);
//		break;
//	case ID_FORMNO_153:	//	⑮-3.工業所有権等の使用料
//		if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc153Kougyou);
//		else				pView = RUNTIME_CLASS(CfrmUc153Kougyou);
//		break;
//	case ID_FORMNO_161:	//	⑯-1.雑益、雑損失等
//		if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc161Zatueki);
//		else				pView = RUNTIME_CLASS(CfrmUc161Zatueki);
//		break;
//	case ID_FORMNO_162:	//	⑯-2.雑損失等
//		if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc162Zatuson);
//		else				pView = RUNTIME_CLASS(CfrmUc162Zatuson);
//		break;
//	case ID_FORMNO_171:
//	case ID_FORMNO_172:
//	case ID_FORMNO_173:
//	case ID_FORMNO_174:
//	case ID_FORMNO_175:
//	case ID_FORMNO_176:
//	case ID_FORMNO_177:
//	case ID_FORMNO_178:
//	case ID_FORMNO_179:
//	case ID_FORMNO_1710:
//	case ID_FORMNO_1711:
//	case ID_FORMNO_1712:
//	case ID_FORMNO_1713:
//	case ID_FORMNO_1714:
//	case ID_FORMNO_1715:
//	case ID_FORMNO_1716:
//	case ID_FORMNO_1717:
//	case ID_FORMNO_1718:
//	case ID_FORMNO_1719:
//	case ID_FORMNO_1720:
//		pView = RUNTIME_CLASS( CfrmUc171Sonota1 );
//		break;
//	case ID_FORMNO_181:
//	case ID_FORMNO_182:
//	case ID_FORMNO_183:
//	case ID_FORMNO_184:
//	case ID_FORMNO_185:
//	case ID_FORMNO_186:
//	case ID_FORMNO_187:
//	case ID_FORMNO_188:
//	case ID_FORMNO_189:
//	case ID_FORMNO_1810:
//	case ID_FORMNO_1811:
//	case ID_FORMNO_1812:
//	case ID_FORMNO_1813:
//	case ID_FORMNO_1814:
//	case ID_FORMNO_1815:
//	case ID_FORMNO_1816:
//	case ID_FORMNO_1817:
//	case ID_FORMNO_1818:
//	case ID_FORMNO_1819:
//	case ID_FORMNO_1820:
//		pView = RUNTIME_CLASS( CfrmUc181Sonota2 );
//		break;
//	}
//	
//	// ポインタ取得済み？
//	if(pView != 0)	{
//		// ドキュメントテンプレート作成
//		m_uDocInfo[nCnt].nID = CreateDocTemplate(IDR_UCHIWAKETYPE,RUNTIME_CLASS(CUCHIWAKEDoc),RUNTIME_CLASS(CChildFrame),pView);
//	}
//}
//// midori 190301 add <--
// インボイス登録番号追加対応_23/09/11 del <--
// インボイス登録番号追加対応_23/09/11 add -->
void CUCHIWAKEApp::SetDocTemplate(int nCnt, int nFormSeq)
{
	CRuntimeClass*		pView;				//	CRuntimeClassポインタ
	BOOL				bKanso=FALSE;
	BOOL				bInvo=FALSE;

	if(nCnt >= UC_ID_FORMNO_MAX_KANSO)	{
		// カウントが84以上であれば、登録番号付きの様式を作成する
		bKanso = FALSE;
		bInvo = TRUE;
	}
	else if(nCnt >= UC_ID_FORMNO_MAX)	{
		// カウントが62以上であれば、簡素化の様式を作成する
		bKanso = TRUE;
		bInvo = FALSE;
	}
	else	{
		// カウントが61以下であれば、カウントとマスターの状態から判断する
		bKanso = FALSE;
		bInvo = FALSE;
	}

	//	初期化
	pView = 0;
	
	//	様式で分岐
	switch( nFormSeq ){
	case ID_FORMNO_011:	//	①.預貯金等
		if(bInvo == TRUE)		pView = RUNTIME_CLASS(CfrmUc011YotyokinK);
		else if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc011YotyokinK);
		else					pView = RUNTIME_CLASS(CfrmUc011Yotyokin);
		break;
	case ID_FORMNO_021:	//	②.受取手形
		if(bInvo == TRUE)		pView = RUNTIME_CLASS(CfrmUc021UketoritegataI);	// 登録番号付き
		else if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc021UketoritegataK);	// 簡素化
		else					pView = RUNTIME_CLASS(CfrmUc021Uketoritegata);
		break;
	case ID_FORMNO_031:	//	③.売掛金
		if(bInvo == TRUE)		pView = RUNTIME_CLASS(CfrmUc031UrikakekinI);	// 登録番号付き
		else if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc031Urikakekin);
		else					pView = RUNTIME_CLASS(CfrmUc031Urikakekin);
		break;
	case ID_FORMNO_041:	//	④-1.仮払金
		if(bInvo == TRUE)		pView = RUNTIME_CLASS(CfrmUc041KaribaraiI);		// 登録番号付き
		else if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc041Karibarai);
		else					pView = RUNTIME_CLASS(CfrmUc041Karibarai);
		break;
	case ID_FORMNO_042:	//	④-2.貸付金及び受取利息
		if(bInvo == TRUE)		pView = RUNTIME_CLASS(CfrmUc042KasitukeI);	// 登録番号付き
		else if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc042KasitukeK);	// 簡素化
		else					pView = RUNTIME_CLASS(CfrmUc042Kasituke);
		break;
	case ID_FORMNO_051:	//	⑤.棚卸資産
		//if(bKanso == TRUE)		pView = RUNTIME_CLASS(CfrmUc051Tanaorosi);
		//else if(bInvo == TRUE)	pView = RUNTIME_CLASS(CfrmUc051Tanaorosi);
		//else						pView = RUNTIME_CLASS(CfrmUc051Tanaorosi);
		pView = RUNTIME_CLASS(CfrmUc051Tanaorosi);
		break;
	case ID_FORMNO_061:	//	⑥.有価証券
		if(bInvo == TRUE)		pView = RUNTIME_CLASS(CfrmUc061YuukasyoukenK);	// 簡素化
		else if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc061YuukasyoukenK);	// 簡素化
		else					pView = RUNTIME_CLASS(CfrmUc061Yuukasyouken);
		break;
	case ID_FORMNO_071:	//	⑦.固定資産
		if(bInvo == TRUE)		pView = RUNTIME_CLASS(CfrmUc071KoteisisanI);	// 登録番号付き
		else if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc071Koteisisan);
		else					pView = RUNTIME_CLASS(CfrmUc071Koteisisan);
		break;
	case ID_FORMNO_081:	//	⑧.支払手形
		if(bInvo == TRUE)		pView = RUNTIME_CLASS(CfrmUc081SiharaitegataI);	// 登録番号付き
		else if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc081SiharaitegataK);	// 簡素化
		else					pView = RUNTIME_CLASS(CfrmUc081Siharaitegata);
		break;
	case ID_FORMNO_091:	//	⑨.買掛金
		if(bInvo == TRUE)		pView = RUNTIME_CLASS(CfrmUc091KaikakekinI);	// 登録番号付き
		else if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc091Kaikakekin);
		else					pView = RUNTIME_CLASS(CfrmUc091Kaikakekin);
		break;
	case ID_FORMNO_101:	//	⑩-1.仮受金
		if(bInvo == TRUE)		pView = RUNTIME_CLASS(CfrmUc101KariukekinI);
		else if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc101Kariukekin);
		else					pView = RUNTIME_CLASS(CfrmUc101Kariukekin);
		break;
	case ID_FORMNO_102:	//	⑩-2.源泉所得税預り金
		//if(bKanso == TRUE)		pView = RUNTIME_CLASS(CfrmUc102Gensen);
		//else if(bInvo == TRUE)	pView = RUNTIME_CLASS(CfrmUc102Gensen);
		//else					pView = RUNTIME_CLASS(CfrmUc102Gensen);
		pView = RUNTIME_CLASS(CfrmUc102Gensen);
		break;
	case ID_FORMNO_111:	//	⑪借入金及び支払利子
		if(bInvo == TRUE)		pView = RUNTIME_CLASS(CfrmUc111KariirekinK);	// 簡素化
		else if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc111KariirekinK);	// 簡素化
		else					pView = RUNTIME_CLASS(CfrmUc111Kariirekin);
		break;
	case ID_FORMNO_121:	//	⑫土地の売上高等
		if(bInvo == TRUE)		pView = RUNTIME_CLASS(CfrmUc121TotiuriageI);	// 登録番号付き
		else if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc121Totiuriage);
		else					pView = RUNTIME_CLASS(CfrmUc121Totiuriage);
		break;
	case ID_FORMNO_131:	//	⑬売上高等の事業所別
		if(bInvo == TRUE)		pView = RUNTIME_CLASS(CfrmUc131UriagedakaK);	// 簡素化
		else if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc131UriagedakaK);	// 簡素化
		else					pView = RUNTIME_CLASS(CfrmUc131Uriagedaka);
		break;
	case ID_FORMNO_141:	//	⑭-1.役員報酬手当金等
		if(bInvo == TRUE)		pView = RUNTIME_CLASS(CfrmUc141YakuinK);		// 簡素化
		else if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc141YakuinK);		// 簡素化
		else					pView = RUNTIME_CLASS(CfrmUc141Yakuin);
		break;
	case ID_FORMNO_142:	//	⑭-2.人件費等
		//if(bInvo == TRUE)		pView = RUNTIME_CLASS(CfrmUc142Jinkenhi);
		//else if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc142Jinkenhi);
		//else					pView = RUNTIME_CLASS(CfrmUc142Jinkenhi);
		pView = RUNTIME_CLASS(CfrmUc142Jinkenhi);
		break;
	case ID_FORMNO_151:	//	⑮-1.地代家賃等
		if(bInvo == TRUE)		pView = RUNTIME_CLASS(CfrmUc151TidaiyatinI);	// 登録番号付き
		else if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc151Tidaiyatin);
		else					pView = RUNTIME_CLASS(CfrmUc151Tidaiyatin);
		break;
	case ID_FORMNO_152:	//	⑮-2.権利金等の期中支払
		if(bInvo == TRUE)		pView = RUNTIME_CLASS(CfrmUc152KenrikinI);		// 登録番号付き
		else if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc152Kenrikin);
		else					pView = RUNTIME_CLASS(CfrmUc152Kenrikin);
		break;
	case ID_FORMNO_153:	//	⑮-3.工業所有権等の使用料
		if(bInvo == TRUE)		pView = RUNTIME_CLASS(CfrmUc153KougyouI);		// 登録番号付き
		else if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc153Kougyou);
		else					pView = RUNTIME_CLASS(CfrmUc153Kougyou);
		break;
	case ID_FORMNO_161:	//	⑯-1.雑益、雑損失等
		if(bInvo == TRUE)		pView = RUNTIME_CLASS(CfrmUc161ZatuekiI);		// 登録番号付き
		else if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc161Zatueki);
		else					pView = RUNTIME_CLASS(CfrmUc161Zatueki);
		break;
	case ID_FORMNO_162:	//	⑯-2.雑損失等
		if(bInvo == TRUE)		pView = RUNTIME_CLASS(CfrmUc162ZatusonI);		// 登録番号付き
		else if(bKanso == TRUE)	pView = RUNTIME_CLASS(CfrmUc162Zatuson);
		else					pView = RUNTIME_CLASS(CfrmUc162Zatuson);
		break;
	case ID_FORMNO_171:
	case ID_FORMNO_172:
	case ID_FORMNO_173:
	case ID_FORMNO_174:
	case ID_FORMNO_175:
	case ID_FORMNO_176:
	case ID_FORMNO_177:
	case ID_FORMNO_178:
	case ID_FORMNO_179:
	case ID_FORMNO_1710:
	case ID_FORMNO_1711:
	case ID_FORMNO_1712:
	case ID_FORMNO_1713:
	case ID_FORMNO_1714:
	case ID_FORMNO_1715:
	case ID_FORMNO_1716:
	case ID_FORMNO_1717:
	case ID_FORMNO_1718:
	case ID_FORMNO_1719:
	case ID_FORMNO_1720:
		if(bInvo == TRUE)		pView = RUNTIME_CLASS( CfrmUc171Sonota1I );		// 登録番号付き
		else if(bKanso == TRUE)	pView = RUNTIME_CLASS( CfrmUc171Sonota1 );
		else					pView = RUNTIME_CLASS( CfrmUc171Sonota1 );
		break;
	case ID_FORMNO_181:
	case ID_FORMNO_182:
	case ID_FORMNO_183:
	case ID_FORMNO_184:
	case ID_FORMNO_185:
	case ID_FORMNO_186:
	case ID_FORMNO_187:
	case ID_FORMNO_188:
	case ID_FORMNO_189:
	case ID_FORMNO_1810:
	case ID_FORMNO_1811:
	case ID_FORMNO_1812:
	case ID_FORMNO_1813:
	case ID_FORMNO_1814:
	case ID_FORMNO_1815:
	case ID_FORMNO_1816:
	case ID_FORMNO_1817:
	case ID_FORMNO_1818:
	case ID_FORMNO_1819:
	case ID_FORMNO_1820:
		pView = RUNTIME_CLASS( CfrmUc181Sonota2 );
		break;
	}
	
	// ポインタ取得済み？
	if(pView != 0)	{
		// ドキュメントテンプレート作成
		m_uDocInfo[nCnt].nID = CreateDocTemplate(IDR_UCHIWAKETYPE,RUNTIME_CLASS(CUCHIWAKEDoc),RUNTIME_CLASS(CChildFrame),pView);
	}
}
// インボイス登録番号追加対応_23/09/11 add <--

//**************************************************
//	アプリケーションの終了（テーブルクローズ、メモリ開放等の後始末）
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CUCHIWAKEApp::EndApp()
{
// 24/01/30_23-0845 add -->
	// 参照モードで確定フラグをONにしているのでOFFにする
	if(m_CloudSansyoFlg2 == TRUE)	{
		CdbUcInfMain	mfcRec(m_pDB);
		if(mfcRec.Init() == DB_ERR_OK)	{
			if(!(mfcRec.IsEOF()))	{
				CCtrlMainGeneralVar cm1(mfcRec);
				mfcRec.Edit();
				cm1.bDataKakutei = FALSE;
				cm1.Set(mfcRec);
				mfcRec.Update();
				mfcRec.Requery();
			}
		}
	}
// 24/01/30_23-0845 add <--
	// 財務クラスはnew/deleteで使用するのを共通仕様とする
	if ( m_pZmSub != NULL ){
		delete m_pZmSub;
		m_pZmSub = NULL;
	}
}

//**************************************************
//	ドキュメントを開く
//	【引数】	nFormSeq	…	シーケンス番号
//	【戻値】	なし
//**************************************************
void CUCHIWAKEApp::OpenDocument( int nFormSeq )
{
	ICSDocTemplate*		pIcsDoc;			//	ICSDocTemplateポインタ
	int					nFormOrder = 0;		//	FormOrder番号
// midori 190301 add -->
	int					nOfs=0;
// midori 190301 add <--

	//	FormOrderを取得
	nFormOrder = GetFormOrder( nFormSeq );

	if (nFormOrder != -1) {
// midori 190301 del -->
		//if (m_uDocInfo[nFormOrder - 1].nID != -1) {
		//	//	ICSDocTemplateのポインタ取得
		//	pIcsDoc = GetDocumentTemplate();

		//	//	ドキュメントを開く
		//	pIcsDoc->Open(m_uDocInfo[nFormOrder - 1].nID);

		//	//	ドキュメントオープン状態を更新
		//	m_uDocInfo[nFormOrder - 1].bOpenFlg = TRUE;
		//}
// midori 190301 del <--
// midori 190301 add -->
// インボイス登録番号追加対応_23/09/11 del -->
		//if(bG_Kanso == TRUE && nFormOrder <= 22)		nOfs=UC_ID_FORMNO_MAX;
		//else											nOfs=0;
// インボイス登録番号追加対応_23/09/11 del <--
// インボイス登録番号追加対応_23/09/11 add -->
		if(bG_InvNo == TRUE && nFormOrder <= 42)		nOfs = UC_ID_FORMNO_MAX_KANSO;
		else if(bG_Kanso == TRUE && nFormOrder <= 22)	nOfs = UC_ID_FORMNO_MAX;
		else											nOfs = 0;
// インボイス登録番号追加対応_23/09/11 add <--
		if(m_uDocInfo[(nFormOrder - 1)+nOfs].nID != -1)	{
			// ICSDocTemplateのポインタ取得
			pIcsDoc = GetDocumentTemplate();
			// ドキュメントを開く
			pIcsDoc->Open(m_uDocInfo[(nFormOrder - 1)+nOfs].nID);
			// ドキュメントオープン状態を更新
			m_uDocInfo[(nFormOrder - 1)+nOfs].bOpenFlg = TRUE;
		}
// midori 190301 add <--
	}
}

// midori 190301 del -->
//// midori 154237,154238 del -->
//////**************************************************
//////	ドキュメントを切り替える
//////	【引数】	nFormSeq	…	シーケンス番号
//////	【戻値】	画面切替を行った順序番号（-1:エラー）
//////**************************************************
////int CUCHIWAKEApp::ChangeDocument( int nFormSeq )
//// midori 154237,154238 del <--
//// midori 154237,154238 add -->
////**************************************************
////	ドキュメントを切り替える
////	【引数】	nFormSeq	…	シーケンス番号
////				pSw			…	1:関数内で LockWindowUpdate を実行しない
////	【戻値】	画面切替を行った順序番号（-1:エラー）
////**************************************************
//int CUCHIWAKEApp::ChangeDocument( int nFormSeq, int pSw )
//// midori 154237,154238 add <--
//{
//	ICSDocTemplate*		pIcsDoc;			//	ICSDocTemplateポインタ
//	int					nFormOrder = 0;		//	FormOrder番号
//	int					nRet = -1;
//
//	//	FormOrderを取得
//	nFormOrder = GetFormOrder( nFormSeq );
//
//	if (nFormOrder != -1) {
//		if (m_uDocInfo[nFormOrder - 1].nID != -1) {
//			// 切替先の帳表が表示済みなら切替、未表示ならオープンをかける
//			if ( m_uDocInfo[nFormOrder - 1].bOpenFlg == FALSE ){
//				// オープンして表示する
//				OpenDocument( nFormSeq );
//
//				// 画面切り替えを行う
//				((CMainFrame*)AfxGetMainWnd())->SetActiveChild(m_uDocInfo[nFormOrder - 1].nID);
//// midori 160612 add -->
//				pIcsDoc = GetDocumentTemplate();
//				((CfrmUc000Common*)pIcsDoc->GetViewWnd(m_uDocInfo[nFormOrder - 1].nID))->CmnChangeButtonEnable(2);
//// midori 160612 add <--
//			}
//			else {
//// midori 154237,154238 del -->
////// midori 152203 add -->
////				((CMainFrame*)AfxGetMainWnd())->LockWindowUpdate();
////// midori 152203 add <--
//// midori 154237,154238 del <--
//// midori 154237,154238 add -->
//				if(pSw == 0)	{
//					((CMainFrame*)AfxGetMainWnd())->LockWindowUpdate();
//				}
//// midori 154237,154238 add <--
//				// 画面切り替えを行う
//				// （特に会社切り替えの場合、先に画面を切り替えておかないと切り替わらない）
//				((CMainFrame*)AfxGetMainWnd())->SetActiveChild(m_uDocInfo[nFormOrder - 1].nID);
//
//				// 会社切り替えの場合、最新データで画面の再描画を行う
//				//	例）通常/個別切り替え実施後
//				//		銀行/取引先名称の更新後 ･･･ など画面表示と最新データが異なる場合がある
//				pIcsDoc = GetDocumentTemplate();
//// midori 154237,154238 del -->
////// midori 154190 add -->
////				m_YsChg = 1;
////// midori 154190 add <--
//// midori 154237,154238 del <--
//				((CfrmUc000Common*)pIcsDoc->GetViewWnd(m_uDocInfo[nFormOrder - 1].nID))->virStartJob();
//// midori 154237,154238 del -->
////// midori 154190 add -->
////				m_YsChg = 0;
////// midori 154190 add <--
//// midori 154237,154238 del <--
//// midori 160612 add -->
//				((CfrmUc000Common*)pIcsDoc->GetViewWnd(m_uDocInfo[nFormOrder - 1].nID))->CmnChangeButtonEnable(2);
//// midori 160612 add <--
//// midori 154237,154238 del -->
////// midori 152203 add -->
////				((CMainFrame*)AfxGetMainWnd())->UnlockWindowUpdate();
////// midori 152203 add <--
//// midori 154237,154238 del <--
//// midori 154237,154238 add -->
//				if(pSw == 0)	{
//					((CMainFrame*)AfxGetMainWnd())->UnlockWindowUpdate();
//				}
//// midori 154237,154238 add <--
//			}
//
//			// 画面切替を実施したので戻り値を更新
//			nRet = nFormOrder;
//		}
//	}
//	
//	return nRet;
//}
// midori 190301 del <--
// midori 190301 add -->
//**************************************************
//	ドキュメントを切り替える
//	【引数】	nFormSeq	…	シーケンス番号
//				pSw			…	1:関数内で LockWindowUpdate を実行しない
//	【戻値】	画面切替を行った順序番号（-1:エラー）
//**************************************************
int CUCHIWAKEApp::ChangeDocument( int nFormSeq, int pSw )
{
	ICSDocTemplate*		pIcsDoc;			//	ICSDocTemplateポインタ
	int					nFormOrder=0;		//	FormOrder番号
	int					nRet=-1;
	int					nOfs=0;

	//	FormOrderを取得
	nFormOrder = GetFormOrder(nFormSeq);

	if(nFormOrder != -1)	{
// インボイス登録番号追加対応_23/09/11 del -->
		//if(bG_Kanso == TRUE && nFormOrder <= 22)		nOfs=UC_ID_FORMNO_MAX;
		//else											nOfs=0;
// インボイス登録番号追加対応_23/09/11 del <--
// インボイス登録番号追加対応_23/09/11 add -->
		if(bG_InvNo == TRUE && nFormOrder <= 42)		nOfs = UC_ID_FORMNO_MAX_KANSO;
		else if(bG_Kanso == TRUE && nFormOrder <= 22)	nOfs = UC_ID_FORMNO_MAX;
		else											nOfs = 0;
// インボイス登録番号追加対応_23/09/11 add <--

		if (m_uDocInfo[(nFormOrder - 1)+nOfs].nID != -1)	{
// インボイス登録番号追加対応_23/12/01 add -->
			((CMainFrame*)AfxGetMainWnd())->LockWindowUpdate();
// インボイス登録番号追加対応_23/12/01 add <--

			// 切替先の帳表が表示済みなら切替、未表示ならオープンをかける
			if(m_uDocInfo[(nFormOrder - 1)+nOfs].bOpenFlg == FALSE)	{
				// オープンして表示する
				OpenDocument(nFormSeq);
				// 画面切り替えを行う
				((CMainFrame*)AfxGetMainWnd())->SetActiveChild(m_uDocInfo[(nFormOrder - 1)+nOfs].nID);
				pIcsDoc = GetDocumentTemplate();
				((CfrmUc000Common*)pIcsDoc->GetViewWnd(m_uDocInfo[(nFormOrder - 1)+nOfs].nID))->CmnChangeButtonEnable(2);
			}
			else	{
// インボイス登録番号追加対応_23/12/01 del -->
				//if(pSw == 0)	{
				//	((CMainFrame*)AfxGetMainWnd())->LockWindowUpdate();
				//}
// インボイス登録番号追加対応_23/12/01 del <--

				// 画面切り替えを行う
				// （特に会社切り替えの場合、先に画面を切り替えておかないと切り替わらない）
				((CMainFrame*)AfxGetMainWnd())->SetActiveChild(m_uDocInfo[(nFormOrder - 1)+nOfs].nID);

				// 会社切り替えの場合、最新データで画面の再描画を行う
				//	例）通常/個別切り替え実施後
				//		銀行/取引先名称の更新後 ･･･ など画面表示と最新データが異なる場合がある
				pIcsDoc = GetDocumentTemplate();
				((CfrmUc000Common*)pIcsDoc->GetViewWnd(m_uDocInfo[(nFormOrder - 1)+nOfs].nID))->virStartJob();
				((CfrmUc000Common*)pIcsDoc->GetViewWnd(m_uDocInfo[(nFormOrder - 1)+nOfs].nID))->CmnChangeButtonEnable(2);
// インボイス登録番号追加対応_23/12/01 del -->
				//if(pSw == 0)	{
				//	((CMainFrame*)AfxGetMainWnd())->UnlockWindowUpdate();
				//}
// インボイス登録番号追加対応_23/12/01 del <--
			}
// インボイス登録番号追加対応_23/12/01 add -->
			((CMainFrame*)AfxGetMainWnd())->UnlockWindowUpdate();
// インボイス登録番号追加対応_23/12/01 add <--

// 改良No.22-0811 add -->
			((CfrmUc000Common*)pIcsDoc->GetViewWnd(m_uDocInfo[(nFormOrder-1)+nOfs].nID))->CmnMenuEnableSwitch();
// 改良No.22-0811 add <--

// 修正No.168393 add -->
			// LockWindowUpdate() と UnlockWindowUpdate() を行うタイミングを変えたことによりカーソルがセットされなくなったので、ここでフォーカスセットを行う
			((CfrmUc000Common*)pIcsDoc->GetViewWnd(m_uDocInfo[(nFormOrder-1)+nOfs].nID))->CmnDiagSetFocus(
				((CfrmUc000Common*)pIcsDoc->GetViewWnd(m_uDocInfo[(nFormOrder-1)+nOfs].nID))->m_pDiag, 
				((CfrmUc000Common*)pIcsDoc->GetViewWnd(m_uDocInfo[(nFormOrder-1)+nOfs].nID))->m_uInfo.intCurRow,
				((CfrmUc000Common*)pIcsDoc->GetViewWnd(m_uDocInfo[(nFormOrder-1)+nOfs].nID))->m_uInfo.intCurCol
			);
// 修正No.168393 add <--

			// 画面切替を実施したので戻り値を更新
			nRet = nFormOrder;
		}
	}
	return(nRet);
}
// midori 190301 add <--

//**************************************************
//	ドキュメントを閉じる
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CUCHIWAKEApp::CloseDocument()
{
	ICSDocTemplate*		pIcsDoc = 0;	//	ICSDocTemplateポインタ
	CMultiDocTemplate*	pMulti = 0;		//	CMultiDocTemplateポインタ
	int					nCnt;

// midori 152151 add -->
	NoActSw = 1;
// midori 152151 add <--

	pIcsDoc = GetDocumentTemplate();									//	ICSDocTemplateのポインタ取得
	//	様式数分ループ
// midori 190301 del -->
//	for( nCnt = 0; nCnt < UC_ID_FORMNO_MAX; nCnt++ ){
// midori 190301 del <--
// midori 190301 add -->
	//for(nCnt=0; nCnt<UC_ID_FORMNO_MAX_KANSO; nCnt++)	{		// インボイス登録番号追加対応_23/09/11 del
	for(nCnt=0; nCnt<UC_ID_FORMNO_MAX_INVO; nCnt++)	{			// インボイス登録番号追加対応_23/09/11 add
// midori 190301 add <--
		//	ID取得あり？
		if (( m_uDocInfo[nCnt].nID != -1 ) && ( m_uDocInfo[nCnt].bOpenFlg == TRUE )){
			pMulti = pIcsDoc->GetDocTemplate( m_uDocInfo[nCnt].nID );	//	CMultiDocTemplateのポインタ取得
			pMulti->CloseAllDocuments( TRUE );							//	ドキュメントを全て閉じる

			//	フラグ更新
			m_uDocInfo[nCnt].bOpenFlg = FALSE;
		}
	}
}

//**************************************************
//	シーケンス番号からオーダー番号取得
//	【引数】	nFormSeq	…	FormSeq番号
//	【戻値】	順序番号（-1:エラー）
//**************************************************
int CUCHIWAKEApp::GetFormOrder( int nFormSeq )
{
	int					nFormOrder = -1;	//	順序番号
	CdbUcInfSub			mfcRec( m_pDB );	//	uc_inf_subテーブルクラス

	//	FormSeqで検索
	if ( mfcRec.RequeryFormSeq( nFormSeq ) == DB_ERR_OK ){
		//	レコードあり？
		if ( !mfcRec.IsEOF() ){
			//	順序番号取得
			nFormOrder = mfcRec.m_FormOrder;
		}
		//	閉じる
		mfcRec.Fin();
	}

	return nFormOrder;
}

//**************************************************
//	オープン済みViewのDB情報を更新する
//	【引数】	なし
//	【戻値】	なし
//**************************************************
void CUCHIWAKEApp::UpdateViewDBInfo()
{
	ICSDocTemplate*		pIcsDoc = 0;	//	ICSDocTemplateポインタ
	int					nCnt;

	//	様式数分ループ
// midori 190301 del -->
//	for( nCnt = 0; nCnt < UC_ID_FORMNO_MAX; nCnt++ ){
// midori 190301 del <--
// midori 190301 add -->
	//for(nCnt=0; nCnt<UC_ID_FORMNO_MAX_KANSO; nCnt++)		{	// インボイス登録番号追加対応_23/09/11 del
	for(nCnt=0; nCnt<UC_ID_FORMNO_MAX_INVO; nCnt++)		{		// インボイス登録番号追加対応_23/09/11 add
// midori 190301 add <--
		//	既にオープン済み？
		if ( m_uDocInfo[nCnt].bOpenFlg == TRUE ){
			//	以前の会社選択の情報が残っているため更新が必要
			pIcsDoc = GetDocumentTemplate();							//	ICSDocTemplateのポインタ取得
			((CfrmUc000Common*)pIcsDoc->GetViewWnd(m_uDocInfo[nCnt].nID))->m_pDB = m_pDB;
		}
	}
}

//**************************************************
//	アプリの起動タイプごとにエラーメッセージの表示方式を変更
//	【引数】	intCoSelType … アプリ起動のタイプ
//				strMsg	   … 表示文言
//				intExitNo　… エラー番号
//	【戻値】	なし
//**************************************************
void CUCHIWAKEApp::SetExitMessage( int intCoSelType , CString strMsg , int intExitNo /* = 0 */ )
{
	char* chrMsg;

	chrMsg = new char[strMsg.GetLength() + 1];
	strcpy_s( chrMsg , strMsg.GetLength() + 1 , strMsg );

	// アプリ起動時の場合
	if ( intCoSelType == 0 ){
		ICSExit( intExitNo , chrMsg );
	}
	// 自身の会社選択ツールバーから会社を切り替えた場合
	else if( intCoSelType == 1 ){
		ICSMessageBox( strMsg );
	}

	delete chrMsg;

}

//********************************************************************************
//	ICS財務処理：財務連動で使用する会社情報の取得（Zvol）
//		IN		CDBZmSub*		DB財務クラス
//				RENZVOL_INFO*	財務連動で使用する会社情報
//		RET		int				FUNCTION_OK:正常終了
//								FUNCTION_NG:エラー
//********************************************************************************
int CUCHIWAKEApp::ICSGetZvolRenData(CDBZmSub* pZmSub, RENZVOL_INFO* uRenZvol)
{
	int			intRet = FUNCTION_NG;

	// 財務会社情報のクリア
	ZeroMemory(uRenZvol, sizeof(RENZVOL_INFO));

	// 会社情報を取得
	if (pZmSub->VolumeOpen() == 0) {
		// 取得成功
		uRenZvol->CoType = pZmSub->zvol->apno;		// 業種コード	（zvol.apno）
		uRenZvol->EzFlag = pZmSub->zvol->br_sw;		// 枝番残高有無	（zvol.br_sw）
		intRet = FUNCTION_OK;

		// 後処理
		pZmSub->VolumeClose();
	}
	else{
		// 2007.03 DBエラー対応
		m_clsFunc.ZmSubErrMsg( pZmSub );
		return FUNCTION_NG;
	}


	return (intRet);
}


//********************************************************************************
//	内訳書コンバージョン：更新後処理（メイン）
//		IN		なし
//		RET		int				FUNCTION_OK:正常終了
//								FUNCTION_NG:エラー
//********************************************************************************
int CUCHIWAKEApp::UchiConvUpdateMainProc()
{
	//----------------------------------------------------------------
	//	内訳書コンバージョンでは、旧財務→新財務へデータ移行を実施
	//	ここでは、Cfrmxxxxxクラスを流用して、
	//	ページ/行のレイアウト調整と、頁計/累計の再計算を行う
	//----------------------------------------------------------------

	AfxGetApp()->DoWaitCursor(1);
	// ①.預貯金～⑯-2.雑損失（その他帳表はコンバージョン対象外）→その他帳票もコンバージョン対象になったので、テーブル更新する
	int			i;
	for (i = ID_FORMNO_011; i <= ID_FORMNO_162; i++) {
		// データテーブル更新
		UchiConvUpdateDataTable(i);
	}
	for (i = 0; i < 20; i++) {
		// ⑰-x.その他１＋⑱-x.その他２
		UchiConvUpdateDataTable(ID_FORMNO_171 + i);
		UchiConvUpdateDataTable(ID_FORMNO_181 + i);
	}

	// コンバート実行後のDBで追加されたカラムの初期値セット
	BOOL bRet = ExecAfterConvert();

	// コンバート後、テーブルアップデートが行われた場合の参照データのSeq,Orderの更新
	CfrmUc000Common* pView = UchiwakeCreateFormViewObject(ID_FORMNO_011);	// 様式は何でもいい
	if(pView){
		//pView->CreateReferenceDlgKana();	// コンバージョン時カナが振られないが、何度もコンバージョンするとその都度カナを振ることになってしまい、修正したカナがあったとしても再び上書きしてしまう為、一旦コメントアウト
		pView->UpdateReferenceData();
	}
	delete pView;
	pView = NULL;

	// uc_inf_mainのコンバージョンフラグ更新（OFF:1→0）
	CdbUcInfMain	rsInfMain(m_pDB);
	rsInfMain.UpdateFgUchiConv(0);
	rsInfMain.Fin();

	AfxGetApp()->DoWaitCursor(-1);

	return FUNCTION_OK;
}

//********************************************************************************
//	内訳書コンバージョン：帳表ごとのデータテーブル更新
//		IN		int				新財務のFormSeq番号
//		RET		int				FUNCTION_OK:正常終了
//								FUNCTION_NG:エラー
//********************************************************************************
int CUCHIWAKEApp::UchiConvUpdateDataTable(int intFormSeq)
{
	CfrmUc000Common*	pView;

	// FormViewオブジェクト作成
	pView = UchiwakeCreateFormViewObject(intFormSeq);

	// 初期設定
	pView->CmnSetInitialUchiwakeInfo(intFormSeq);		// 共通データ
	pView->virSetInitialValue();						// 帳表固有データ

	// データがある場合、頁と行の再設定を行う
	if (pView->m_uInfo.intMaxPage > 0) {
		pView->CmnTblUpdatePageRowAllResetMain();
	}
	delete pView;

	return FUNCTION_OK;
}

//********************************************************************************
//	内訳書全般：FormViewクラスのオブジェクト作成
//		IN		int					新財務のFormSeq番号
//		RET		CfrmUc000Common*	FormViewクラスのポインタ
//********************************************************************************
CfrmUc000Common* CUCHIWAKEApp::UchiwakeCreateFormViewObject(int intFormSeq)
{
// インボイス登録番号追加対応_23/09/11 del -->
//	switch (intFormSeq) {
//	case ID_FORMNO_011:
//// midori 190301 del -->
//		//return (CfrmUc000Common*)(new CfrmUc011Yotyokin);			// ①.預貯金
//// midori 190301 del <--
//// midori 190301 add -->
//		if(bG_Kanso == TRUE)	return (CfrmUc000Common*)(new CfrmUc011YotyokinK);			// ①.預貯金
//		else					return (CfrmUc000Common*)(new CfrmUc011Yotyokin);			// ①.預貯金
//// midori 190301 add <--
//
//	case ID_FORMNO_021:
//// midori 190301 del -->
//		//return (CfrmUc000Common*)(new CfrmUc021Uketoritegata);		// ②.受取手形
//// midori 190301 del <--
//// midori 190301 add -->
//		if(bG_Kanso == TRUE)	return (CfrmUc000Common*)(new CfrmUc021UketoritegataK);			// ②.受取手形
//		else						return (CfrmUc000Common*)(new CfrmUc021Uketoritegata);		// ②.受取手形
//// midori 190301 add <--
//
//	case ID_FORMNO_031:
//		return (CfrmUc000Common*)(new CfrmUc031Urikakekin);									// ③.売掛金
//
//	case ID_FORMNO_041:
//		return (CfrmUc000Common*)(new CfrmUc041Karibarai);			// ④-1.仮払金
//
//	case ID_FORMNO_042:
//// midori 190301 del -->
//		//return (CfrmUc000Common*)(new CfrmUc042Kasituke);			// ④-2.貸付金
//// midori 190301 del <--
//// midori 190301 add -->
//		if(bG_Kanso == TRUE)	return (CfrmUc000Common*)(new CfrmUc042KasitukeK);			// ④-2.貸付金
//		else					return (CfrmUc000Common*)(new CfrmUc042Kasituke);			// ④-2.貸付金
//// midori 190301 add <--
//
//	case ID_FORMNO_051:
//		return (CfrmUc000Common*)(new CfrmUc051Tanaorosi);			// ⑤.棚卸資産
//
//	case ID_FORMNO_061:
//// midori 190301 del -->
//		//return (CfrmUc000Common*)(new CfrmUc061Yuukasyouken);		// ⑥.有価証券
//// midori 190301 del <--
//// midori 190301 add -->
//		if(bG_Kanso == TRUE)	return (CfrmUc000Common*)(new CfrmUc061YuukasyoukenK);		// ⑥.有価証券
//		else					return (CfrmUc000Common*)(new CfrmUc061Yuukasyouken);		// ⑥.有価証券
//// midori 190301 add <--
//
//	case ID_FORMNO_071:
//		return (CfrmUc000Common*)(new CfrmUc071Koteisisan);								// ⑦.固定資産
//
//	case ID_FORMNO_081:
//// midori 190301 del -->
//		//return (CfrmUc000Common*)(new CfrmUc081Siharaitegata);	// ⑧.支払手形
//// midori 190301 del <--
//// midori 190301 del -->
//		if(bG_Kanso == TRUE)	return (CfrmUc000Common*)(new CfrmUc081SiharaitegataK);		// ⑧.支払手形
//		else					return (CfrmUc000Common*)(new CfrmUc081Siharaitegata);		// ⑧.支払手形
//// midori 190301 del <--
//
//	case ID_FORMNO_091:
//		return (CfrmUc000Common*)(new CfrmUc091Kaikakekin);								// ⑨.買掛金
//
//	case ID_FORMNO_101:
//		return (CfrmUc000Common*)(new CfrmUc101Kariukekin);			// ⑩-1.仮受金
//
//	case ID_FORMNO_102:
//		return (CfrmUc000Common*)(new CfrmUc102Gensen);				// ⑩-2.源泉所得税
//
//	case ID_FORMNO_111:
//// midori 190301 del -->
//		//return (CfrmUc000Common*)(new CfrmUc111Kariirekin);			// ⑪.借入金
//// midori 190301 del <--
//// midori 190301 add -->
//		if(bG_Kanso == TRUE)	return (CfrmUc000Common*)(new CfrmUc111KariirekinK);		// ⑪.借入金
//		else					return (CfrmUc000Common*)(new CfrmUc111Kariirekin);			// ⑪.借入金
//// midori 190301 add <--
//
//	case ID_FORMNO_121:
//		return (CfrmUc000Common*)(new CfrmUc121Totiuriage);			// ⑫.土地の売上高
//
//	case ID_FORMNO_131:
//// midori 190301 del -->
//		//return (CfrmUc000Common*)(new CfrmUc131Uriagedaka);			// ⑬.売上高等
//// midori 190301 del <--
//// midori 190301 add -->
//		if(bG_Kanso == TRUE)	return (CfrmUc000Common*)(new CfrmUc131UriagedakaK);		// ⑬.売上高等
//		else					return (CfrmUc000Common*)(new CfrmUc131Uriagedaka);			// ⑬.売上高等
//// midori 190301 add <--
//
//	case ID_FORMNO_141:
//// midori 190301 del -->
//		//return (CfrmUc000Common*)(new CfrmUc141Yakuin);				// ⑭-1.役員報酬
//// midori 190301 del <--
//// midori 190301 add -->
//		if(bG_Kanso == TRUE)	return (CfrmUc000Common*)(new CfrmUc141YakuinK);			// ⑭-1.役員報酬
//		else					return (CfrmUc000Common*)(new CfrmUc141Yakuin);				// ⑭-1.役員報酬
//// midori 190301 add <--
//
//	case ID_FORMNO_142:
//		return (CfrmUc000Common*)(new CfrmUc142Jinkenhi);			// ⑭-2.人件費
//
//	case ID_FORMNO_151:
//		return (CfrmUc000Common*)(new CfrmUc151Tidaiyatin);			// ⑮-1.地代家賃
//
//	case ID_FORMNO_152:
//		return (CfrmUc000Common*)(new CfrmUc152Kenrikin);			// ⑮-2.権利金
//
//	case ID_FORMNO_153:
//		return (CfrmUc000Common*)(new CfrmUc153Kougyou);			// ⑮-3.工業所有権
//
//	case ID_FORMNO_161:
//		return (CfrmUc000Common*)(new CfrmUc161Zatueki);			// ⑯-1.雑益
//
//	case ID_FORMNO_162:
//		return (CfrmUc000Common*)(new CfrmUc162Zatuson);			// ⑯-2.雑損失
//
//	case ID_FORMNO_171:
//	case ID_FORMNO_172:
//	case ID_FORMNO_173:
//	case ID_FORMNO_174:
//	case ID_FORMNO_175:
//	case ID_FORMNO_176:
//	case ID_FORMNO_177:
//	case ID_FORMNO_178:
//	case ID_FORMNO_179:
//	case ID_FORMNO_1710:
//	case ID_FORMNO_1711:
//	case ID_FORMNO_1712:
//	case ID_FORMNO_1713:
//	case ID_FORMNO_1714:
//	case ID_FORMNO_1715:
//	case ID_FORMNO_1716:
//	case ID_FORMNO_1717:
//	case ID_FORMNO_1718:
//	case ID_FORMNO_1719:
//	case ID_FORMNO_1720:
//		return  (CfrmUc000Common*)(new CfrmUc171Sonota1);			// ⑰.その他科目の内訳書①
//
//	case ID_FORMNO_181:
//	case ID_FORMNO_182:
//	case ID_FORMNO_183:
//	case ID_FORMNO_184:
//	case ID_FORMNO_185:
//	case ID_FORMNO_186:
//	case ID_FORMNO_187:
//	case ID_FORMNO_188:
//	case ID_FORMNO_189:
//	case ID_FORMNO_1810:
//	case ID_FORMNO_1811:
//	case ID_FORMNO_1812:
//	case ID_FORMNO_1813:
//	case ID_FORMNO_1814:
//	case ID_FORMNO_1815:
//	case ID_FORMNO_1816:
//	case ID_FORMNO_1817:
//	case ID_FORMNO_1818:
//	case ID_FORMNO_1819:
//	case ID_FORMNO_1820:
//		return (CfrmUc000Common*)(new CfrmUc181Sonota2);			// ⑱.その他科目の内訳書②
//	}
// インボイス登録番号追加対応_23/09/11 del <--
// インボイス登録番号追加対応_23/09/11 add -->
	switch (intFormSeq) {
	case ID_FORMNO_011:
		if(bG_Kanso == TRUE)		return (CfrmUc000Common*)(new CfrmUc011YotyokinK);			// ①.預貯金
		else						return (CfrmUc000Common*)(new CfrmUc011Yotyokin);			// ①.預貯金

	case ID_FORMNO_021:
		if(bG_InvNo == TRUE)		return (CfrmUc000Common*)(new CfrmUc021UketoritegataI);		// ②.受取手形
		else if(bG_Kanso == TRUE)	return (CfrmUc000Common*)(new CfrmUc021UketoritegataK);		// ②.受取手形
		else						return (CfrmUc000Common*)(new CfrmUc021Uketoritegata);		// ②.受取手形

	case ID_FORMNO_031:
		if(bG_InvNo == TRUE)		return (CfrmUc000Common*)(new CfrmUc031UrikakekinI);		// ③.売掛金
		else						return (CfrmUc000Common*)(new CfrmUc031Urikakekin);			// ③.売掛金

	case ID_FORMNO_041:
		if(bG_InvNo == TRUE)		return (CfrmUc000Common*)(new CfrmUc041KaribaraiI);			// ④-1.仮払金
		else						return (CfrmUc000Common*)(new CfrmUc041Karibarai);			// ④-1.仮払金

	case ID_FORMNO_042:
		if(bG_InvNo == TRUE)		return (CfrmUc000Common*)(new CfrmUc042KasitukeI);			// ④-2.貸付金
		else if(bG_Kanso == TRUE)	return (CfrmUc000Common*)(new CfrmUc042KasitukeK);			// ④-2.貸付金
		else						return (CfrmUc000Common*)(new CfrmUc042Kasituke);			// ④-2.貸付金

	case ID_FORMNO_051:
		return (CfrmUc000Common*)(new CfrmUc051Tanaorosi);										// ⑤.棚卸資産

	case ID_FORMNO_061:
		if(bG_Kanso == TRUE)		return (CfrmUc000Common*)(new CfrmUc061YuukasyoukenK);		// ⑥.有価証券
		else						return (CfrmUc000Common*)(new CfrmUc061Yuukasyouken);		// ⑥.有価証券

	case ID_FORMNO_071:
		if(bG_InvNo == TRUE)		return (CfrmUc000Common*)(new CfrmUc071KoteisisanI);		// ⑦.固定資産
		else						return (CfrmUc000Common*)(new CfrmUc071Koteisisan);			// ⑦.固定資産

	case ID_FORMNO_081:
		if(bG_InvNo == TRUE)		return (CfrmUc000Common*)(new CfrmUc081SiharaitegataI);		// ⑧.支払手形
		else if(bG_Kanso == TRUE)	return (CfrmUc000Common*)(new CfrmUc081SiharaitegataK);		// ⑧.支払手形
		else						return (CfrmUc000Common*)(new CfrmUc081Siharaitegata);		// ⑧.支払手形

	case ID_FORMNO_091:
		if(bG_InvNo == TRUE)		return (CfrmUc000Common*)(new CfrmUc091KaikakekinI);		// ⑨.買掛金
		else						return (CfrmUc000Common*)(new CfrmUc091Kaikakekin);			// ⑨.買掛金

	case ID_FORMNO_101:
		if(bG_InvNo == TRUE)		return (CfrmUc000Common*)(new CfrmUc101KariukekinI);		// ⑩-1.仮受金
		else						return (CfrmUc000Common*)(new CfrmUc101Kariukekin);			// ⑩-1.仮受金

	case ID_FORMNO_102:
		return (CfrmUc000Common*)(new CfrmUc102Gensen);											// ⑩-2.源泉所得税

	case ID_FORMNO_111:
		if(bG_Kanso == TRUE)		return (CfrmUc000Common*)(new CfrmUc111KariirekinK);		// ⑪.借入金
		else						return (CfrmUc000Common*)(new CfrmUc111Kariirekin);			// ⑪.借入金

	case ID_FORMNO_121:
		return (CfrmUc000Common*)(new CfrmUc121Totiuriage);										// ⑫.土地の売上高

	case ID_FORMNO_131:
		if(bG_Kanso == TRUE)		return (CfrmUc000Common*)(new CfrmUc131UriagedakaK);		// ⑬.売上高等
		else						return (CfrmUc000Common*)(new CfrmUc131Uriagedaka);			// ⑬.売上高等

	case ID_FORMNO_141:
		if(bG_Kanso == TRUE)		return (CfrmUc000Common*)(new CfrmUc141YakuinK);			// ⑭-1.役員報酬
		else						return (CfrmUc000Common*)(new CfrmUc141Yakuin);				// ⑭-1.役員報酬

	case ID_FORMNO_142:
		return (CfrmUc000Common*)(new CfrmUc142Jinkenhi);										// ⑭-2.人件費

	case ID_FORMNO_151:
		if(bG_InvNo == TRUE)		return (CfrmUc000Common*)(new CfrmUc151TidaiyatinI);		// ⑮-1.地代家賃
		else						return (CfrmUc000Common*)(new CfrmUc151Tidaiyatin);			// ⑮-1.地代家賃

	case ID_FORMNO_152:
		if(bG_InvNo == TRUE)		return (CfrmUc000Common*)(new CfrmUc152KenrikinI);			// ⑮-2.権利金
		else						return (CfrmUc000Common*)(new CfrmUc152Kenrikin);			// ⑮-2.権利金

	case ID_FORMNO_153:
		if(bG_InvNo == TRUE)		return (CfrmUc000Common*)(new CfrmUc153KougyouI);			// ⑮-3.工業所有権
		else						return (CfrmUc000Common*)(new CfrmUc153Kougyou);			// ⑮-3.工業所有権

	case ID_FORMNO_161:
		if(bG_InvNo == TRUE)		return (CfrmUc000Common*)(new CfrmUc161ZatuekiI);			// ⑯-1.雑益
		else						return (CfrmUc000Common*)(new CfrmUc161Zatueki);			// ⑯-1.雑益

	case ID_FORMNO_162:
		if(bG_InvNo == TRUE)		return (CfrmUc000Common*)(new CfrmUc162ZatusonI);			// ⑯-2.雑損失
		else						return (CfrmUc000Common*)(new CfrmUc162Zatuson);			// ⑯-2.雑損失

	case ID_FORMNO_171:
	case ID_FORMNO_172:
	case ID_FORMNO_173:
	case ID_FORMNO_174:
	case ID_FORMNO_175:
	case ID_FORMNO_176:
	case ID_FORMNO_177:
	case ID_FORMNO_178:
	case ID_FORMNO_179:
	case ID_FORMNO_1710:
	case ID_FORMNO_1711:
	case ID_FORMNO_1712:
	case ID_FORMNO_1713:
	case ID_FORMNO_1714:
	case ID_FORMNO_1715:
	case ID_FORMNO_1716:
	case ID_FORMNO_1717:
	case ID_FORMNO_1718:
	case ID_FORMNO_1719:
	case ID_FORMNO_1720:
		if(bG_InvNo == TRUE)		return  (CfrmUc000Common*)(new CfrmUc171Sonota1I);			// ⑰.その他科目の内訳書①
		else						return  (CfrmUc000Common*)(new CfrmUc171Sonota1);			// ⑰.その他科目の内訳書①

	case ID_FORMNO_181:
	case ID_FORMNO_182:
	case ID_FORMNO_183:
	case ID_FORMNO_184:
	case ID_FORMNO_185:
	case ID_FORMNO_186:
	case ID_FORMNO_187:
	case ID_FORMNO_188:
	case ID_FORMNO_189:
	case ID_FORMNO_1810:
	case ID_FORMNO_1811:
	case ID_FORMNO_1812:
	case ID_FORMNO_1813:
	case ID_FORMNO_1814:
	case ID_FORMNO_1815:
	case ID_FORMNO_1816:
	case ID_FORMNO_1817:
	case ID_FORMNO_1818:
	case ID_FORMNO_1819:
	case ID_FORMNO_1820:
		return (CfrmUc000Common*)(new CfrmUc181Sonota2);										// ⑱.その他科目の内訳書②
	}
// インボイス登録番号追加対応_23/09/11 add <--

	// 例外ケース
	return (CfrmUc000Common*)NULL;
}

// midori 180503 del -->
////********************************************************************************
////	翌期更新：メイン処理シーケンス
////		IN		long			内訳書の決算期間（期首）
////				long			内訳書の決算期間（期末）
////		RET		int				FUNCTION_OK:正常終了
////								FUNCTION_NG:エラー
////********************************************************************************
//int CUCHIWAKEApp::UchiYokukiUpdateMainProc(long lngUchiSSymd, long lngUchiEEymd)
//{
//	long			lngZvolSSymd = 0;		// 財務マスターの決算期間（期首）
//	long			lngZvolEEymd = 0;		// 財務マスターの決算期間（期末）
//	int				i;
//// midori 160605 add -->
//	int				rv=0;
//// midori 160605 add <--
//// midori 161103 add -->
//	int				flg=-1;					// ダイアログ表示サイン -1:表示しない 0:翌期更新ダイアログ表示 1:前年度からの更新ダイアログ表示
//	CdbUcInfMain	rsMain(m_pDB);
//// midori 161103 add <--
//
//// midori UC_0004 del -->
////// midori 160612_2 add -->
////	// -----------------------------------------------------------------------------
////	// ①過年度マスターを選択している
////	// ②当年度内訳書マスターが更新されていない
////	// ③当年度内訳書マスターが更新されていない状態で初めて過年度マスターを起動した
////	// →上記の場合、過年度財務マスターの確定サインを、過年度内訳書マスターの
////	// 　確定サインにセットする
////	// -----------------------------------------------------------------------------
////	if(KanendoKakuteiSet() != FUNCTION_OK)	{
////		return(FUNCTION_NG);
////	}
////// midori 160612_2 add <--
//// midori UC_0004 del <--
//// midori UC_0004 add -->
//	if(ChkKakuteiUpd() == 0)	{
//		// -----------------------------------------------------------------------------
//		// ①過年度マスターを選択している
//		// ②当年度内訳書マスターが更新されていない
//		// ③当年度内訳書マスターが更新されていない状態で初めて過年度マスターを起動した
//		// →上記の場合、過年度財務マスターの確定サインを、過年度内訳書マスターの
//		// 　確定サインにセットする
//		// -----------------------------------------------------------------------------
//		if(KanendoKakuteiSet() != FUNCTION_OK)	{
//			return(FUNCTION_NG);
//		}
//	}
//// midori UC_0004 add <--
//
//	// 財務マスターの決算期間を取得
//	UchiYokukiGetZvolumeYmd(&lngZvolSSymd, &lngZvolEEymd);
//
//	// 初めて内訳書を起動した場合は実行しない
//	if ((lngUchiSSymd == 0) && (lngUchiEEymd == 0)) {
//	}
//	else {
//// midori 161103 cor -->
//		//// 決算期間を比較する
//		//if ((lngUchiSSymd == lngZvolSSymd) && (lngUchiEEymd == lngZvolEEymd)) {
//		//	return FUNCTION_OK;
//		//}
//// ---------------------
//		flg = -1;
//		// 決算期間を比較する
//// midori 153101 del -->
//		//if((lngUchiSSymd != lngZvolSSymd) || (lngUchiEEymd != lngZvolEEymd))	flg = 0;
//// midori 153101 del <--
//// midori 153101 add -->
//		// -----------------------------------------------------------------
//		// flg	-1 : 処理を行わない
//		//		0  : 翌期更新処理を行う
//		//		1  : 前年度からの更新処理を行う
//		//		2  : 翌期更新処理を行うがダイアログは表示しない
//		// -----------------------------------------------------------------
//		if((lngUchiSSymd != lngZvolSSymd) || (lngUchiEEymd != lngZvolEEymd))	{
//			flg = 0;
//// midori 153112 del -->
//			//// 入力データが確定されている場合、翌期更新ダイアログを表示しない
//			//if(rsMain.Init() == DB_ERR_OK){
//			//	if((rsMain.m_GeneralVar[1] & BIT_D2) == BIT_D2)	{
//			//		flg = 2;
//			//	}
//			//	rsMain.Fin();
//			//}
//// midori 153112 del <--
//// midori 153112 add -->
//			// 財務と決算期間が違っていても、過年度マスターが選択されている場合、翌期更新ダイアログを表示しない
//			if(IsKanendoMaster() == TRUE)	{
//				flg = 2;
//			}
//// midori 153112 add <--
//		}
//// midori 153101 add <--
//
//		// 翌期更新を行わない場合、前年度からの更新を行うか判断する
//		if(flg == -1)	{
//			// 前年度からの更新フラグを取得する
//			if(rsMain.Init() == DB_ERR_OK){
//				if((rsMain.m_GeneralVar[1] & BIT_D4) == 0x10)	flg = 1;
//				rsMain.Fin();
//			}
//		}
//
//		// 翌期更新、前年度からの更新を行わない場合、処理を抜ける
//		if (flg == -1) {
//			return FUNCTION_OK;
//		}
//// midori 161103 cor <--
//
//		// 2011/12/22 翌期更新実行確認画面削除
//		// 確認メッセージ表示→ キャンセル：抜ける
////		CdlgYokukiMessage	dlgYMess( this->m_pMainWnd );
////		if ( dlgYMess.ShowDialog( lngZvolSSymd , lngZvolEEymd , lngUchiSSymd , lngUchiEEymd ) != ID_DLG_OK ){
////			return FUNCTION_OK;
////		}
//
//		// 翌期更新テーブルがオープン可?
//		CdbUcInfYokuki mfcYokuki( m_pDB );
//		if ( m_clsFunc.CheckTableOpen( (CdbBase*)&mfcYokuki ) != TRUE ){
//			return FUNCTION_NG;
//		}
//
//		// 翌期更新の設定画面→ キャンセル：抜ける
//		CdlgYokukiMain		dlgYMain( this->m_pMainWnd );		// 翌期更新設定ダイアログ
//// midori 160605 cor -->
//		//if (dlgYMain.ShowDialog( m_pDB ) != ID_DLG_OK ) {
//		//	return FUNCTION_NG;
//		//}
//
//		//// 翌期更新の更新処理
//		//for (i = ID_FORMNO_011; i <= ID_FORMNO_162; i++) {
//		//	// ①.預貯金 ～ ⑯-2.雑損失
//		//	if ( UchiYokukiUpdateSubProc(i) != FUNCTION_OK ){
//		//		return FUNCTION_NG;
//		//	}
//		//}
//		//for (i = 0; i < 20; i++) {
//		//	// ⑰-x.その他１＋⑱-x.その他２
//		//	UchiYokukiUpdateSubProc(ID_FORMNO_171 + i);
//		//	UchiYokukiUpdateSubProc(ID_FORMNO_181 + i);
//		//}
//// ---------------------
//// midori 161103 cor -->
////		rv = dlgYMain.ShowDialog(m_pDB);
//// ---------------------
//// midori 153101 del -->
//		//rv = dlgYMain.ShowDialog(m_pDB,flg);
//// midori 153101 del <--
//// midori 153101 add -->
//		if(flg == 2)	{
//			rv = 2;
//		}
//		else	{
//			rv = dlgYMain.ShowDialog(m_pDB,flg);
//		}
//// midori 153101 add <--
//// midori 161103 cor <--
//		// 対象の内訳書が１件もない場合はダイアログを表示せず
//		// "rv" に "ID_DLG_MAX" をセットして戻ります
//		if(rv == ID_DLG_CANCEL)	{
//			// キャンセル
//			return FUNCTION_NG;
//		}
//		else if(rv == ID_DLG_OK)	{
//			// ＯＫ -> 翌期更新の更新処理
//			for (i = ID_FORMNO_011; i <= ID_FORMNO_162; i++) {
//				// ①.預貯金 ～ ⑯-2.雑損失
//				if ( UchiYokukiUpdateSubProc(i) != FUNCTION_OK ){
//					return FUNCTION_NG;
//				}
//			}
//			for (i = 0; i < 20; i++) {
//				// ⑰-x.その他１＋⑱-x.その他２
//				UchiYokukiUpdateSubProc(ID_FORMNO_171 + i);
//				UchiYokukiUpdateSubProc(ID_FORMNO_181 + i);
//			}
//		}
//// midori 160605 cor <--
//
//// midori 161103 cor -->
////// midori 160612 add -->
////		// 過年度財務マスターの「決算確定」を、内訳書の「入力データを確定する」にセットする
////		if(KakuteiSet() != FUNCTION_OK)	{
////			return(FUNCTION_NG);
////		}
////// midori 160612 add <--
//// ---------------------
//		// 翌期更新を行う場合
//// midori 153101 del -->
//		//if (flg == 0) {
//// midori 153101 del <--
//// midori 153101 add -->
//		if (flg == 0 || flg == 2) {
//// midori 153101 add <--
//// midori UC_0004 del -->
//			//// 過年度財務マスターの「決算確定」を、内訳書の「入力データを確定する」にセットする
//			//if(KakuteiSet() != FUNCTION_OK)	{
//			//	return(FUNCTION_NG);
//			//}
//// midori UC_0004 del <--
//// midori UC_0004 add -->
//			if(ChkKakuteiUpd() == 0)	{
//				// 過年度財務マスターの「決算確定」を、内訳書の「入力データを確定する」にセットする
//				if(KakuteiSet() != FUNCTION_OK)	{
//					return(FUNCTION_NG);
//				}
//			}
//// midori UC_0004 add <--
//		}
//		// 前年度からの更新実行サインをOFFにする
//		if(rsMain.Init() == DB_ERR_OK){
//			rsMain.Edit();
//			CCtrlMainGeneralVar cm(rsMain);
//			cm.bZenFlg = FALSE;
//			cm.Set(rsMain);
//			rsMain.Update();
//			rsMain.Fin();
//		}
//// midori 161103 cor <--
//	}
//
//	// 内訳書の決算期間を更新（キャンセルを選択した場合は更新しない）
//// midori 161103 cor -->
////	CdbUcInfMain		rsMain(m_pDB);
////	rsMain.UpdateSSymdEEymd(lngZvolSSymd, lngZvolEEymd);
////	rsMain.Fin();
//// ---------------------
//	if(rsMain.Init() == DB_ERR_OK){
//		rsMain.UpdateSSymdEEymd(lngZvolSSymd, lngZvolEEymd);
//		rsMain.Fin();
//	}
//// midori 161103 cor <--
//
//	return FUNCTION_OK;
//}
//
////********************************************************************************
////	翌期更新：更新処理シーケンス
////		IN		int				FormSeq番号
////		RET		int				FUNCTION_OK:正常終了
////								FUNCTION_NG:エラー
////********************************************************************************
//int CUCHIWAKEApp::UchiYokukiUpdateSubProc(int intFormSeq)
//{
//	CfrmUc000Common*	pView = NULL;			// FormViewオブジェクト
//	CdbUc000Common*		pData1 = NULL;
//	CdbUc000Common*		pData2 = NULL;
//	CdbUcInfYokuki		rsYokuki(m_pDB);		// uc_inf_yokuki
//	SORTKAMOKU_INFO		uKamoku;
//	int					nGroup;
//
//	//----------------------------------------------------------------
//	// 【 翌期更新 】
//	//		①：一括金額行を元に戻す
//	//		②：指定項目を０クリア
//	//		③：現在のデータを全て一時保管へ移動
//	//		④：①～③にて、頁/行、頁計/累計を再設定
//	//----------------------------------------------------------------
//
//	// uc_inf_yokuki参照
//	rsYokuki.RequeryFormSeq(intFormSeq);
//
//	if (!rsYokuki.IsEOF()) {
//		// 翌期更新の対象帳表？
//		if (rsYokuki.m_OpYokuki == 1) {
//			// 各帳表のFormViewオブジェクト取得
//			pView = UchiwakeCreateFormViewObject(intFormSeq);		// FormViewオブジェクト
//			if ( pView->CmnCheckTableInit() != TRUE ){				// 帳表テーブルがオープン可?
//				rsYokuki.Fin();
//				if ( pView != NULL ){
//					delete pView;
//				}
//				return FUNCTION_NG;
//			}
//			if ( pView->CmnSetInitialUchiwakeInfo(intFormSeq) != TRUE ){	// 初期設定（共通データ）
//				rsYokuki.Fin();
//				if ( pView != NULL ){
//					delete pView;
//				}
//				return FUNCTION_NG;
//			}
//			pView->virSetInitialValue();									// 初期設定（帳表固有データ）
//
//			// dbオブジェクト取得
//			pData1 = pView->virGetDataTableObject();
//
//			// ①：一括金額行を元に戻す（必ず実行）
//// midori 152387 del -->
////// midori 152221 cor -->
////			//if (TRUE) {
////// ---------------------
////			// 金額等を０クリア、データを保管へ移動のいずれかに
////			// チェックを付けている場合は一括金額行を元に戻す
////			if(rsYokuki.m_OpClear == 1 || rsYokuki.m_OpHokan == 1)	{
////// midori 152221 cor <--
//// midori 152387 del <--
//// midori 152387 add -->
//			// 金額等を０クリア、データを保管へ移動のいずれかに
//			// チェックを付けている場合は一括金額行を元に戻す
//			if(rsYokuki.m_OpClear == 1 || rsYokuki.m_OpNullClear == 1 || rsYokuki.m_OpHokan == 1)	{
//// midori 152387 add <--
//				// 一括金額行を抽出
//				pData1->RequeryFgFunc(ID_FGFUNC_IKKATUAUTO, 1);		// 一括金額行を降順で抽出
//
//				while (!pData1->IsEOF()) {
//					nGroup = (int)pData1->m_NumGroup;
//
//					// 一括金額行の元データを抽出（現行：ダイアログ側で実施していた処理）
//					pData2 = pView->virGetDataTableObject();
//					pData2->UpdateFgShowRetIkkatu(nGroup);
//					delete pData2;
//
//					// 一括金額行を戻す（現行：フォームView側で実施していた処理）
//					pView->CmnTblUpdateFgShowDataReturn(ID_FGSHOW_IKKATU, pData1->m_NumPage, pData1->m_NumRow, nGroup);
//					pView->CmnTblIkkatuAutoUpdateProc(1, nGroup, 0, 0, uKamoku);				// 元データの一括金額行の削除
//					pView->CmnTblUpdatePageRowAllResetMain();									// 頁と行の再設定
//
//					// 次データへ移動
//					pData1->MoveNext();
//				}
//				pData1->Fin();
//			}
//
//			// ②：指定項目を０クリア
//			if (rsYokuki.m_OpClear == 1) {
//				// 帳表データの０クリア
//// midori 160606 cor -->
//				//pData1->UpdateYokukiZeroClear();
//// ---------------------
//				pData1->UpdateYokukiZeroClear(0);
//// midori 160606 cor <--
//
//				// *******************************************************
//				// OutRangaiを追加した時にバグで更新できなくなっている。
//				// が、特にバグ報告もないので仕様ということにした
//				// *******************************************************
//				// 欄外データの０クリア
//				//switch (intFormSeq) {
//				//case ID_FORMNO_051:
//				//	pData2 = (CdbUc000Common*)(new CdbUc052Tanaoroshi2(m_pDB));
//				//	pData2->UpdateYokukiZeroClear();
//				//	delete pData2;
//				//	break;
//				//case ID_FORMNO_091:
//				//	pData2 = (CdbUc000Common*)(new CdbUc092Kaikakekin2(m_pDB));
//				//	pData2->UpdateYokukiZeroClear();
//				//	delete pData2;
//				//	break;
//				//}
//			}
//
//// midori 152387 add -->
//			// ②－２：指定項目を空白にする
//			if(rsYokuki.m_OpNullClear == 1)	{
//				pData1->UpdateYokukiZeroClear(1);
//			}
//// midori 152387 add <--
//
//			// ③：現在のデータを全て一時保管へ移動
//			if (rsYokuki.m_OpHokan == 1) {
//				// 現在の一時保管データを全て削除（その他不要データも削除）
//				pData1->DeleteFgShow(ID_FGSHOW_HOKAN);			//	1	一時保管
//				pData1->DeleteFgShow(ID_FGSHOW_IKKATU);			//	2	一括金額
//				pData1->DeleteFgShow(ID_FGSHOW_RET_HOKAN);		//  101	一時保管からの戻し対象
//				pData1->DeleteFgShow(ID_FGSHOW_RET_IKKATU);		//  102 一括金額からの戻し対象
//
//				// 空行、小計、中計行等の不要データを削除
//				pData1->DeleteFgFunc(ID_FGFUNC_NULL);			//	 0	空行	
//				pData1->DeleteFgFunc(ID_FGFUNC_SYOKEI);			//	10	小計行
//				pData1->DeleteFgFunc(ID_FGFUNC_SYOKEINULL);		//	11	小計行（空行）
//				pData1->DeleteFgFunc(ID_FGFUNC_CHUKEI);			//	12	中計行
//				pData1->DeleteFgFunc(ID_FGFUNC_CHUKEINULL);		//	13	中計行（空行）
//				pData1->DeleteFgFunc(ID_FGFUNC_RUIKEI);			//	14	累計行
//				pData1->DeleteFgFunc(ID_FGFUNC_PAGEKEI);		//	15	頁計行
//				pData1->DeleteFgFunc(ID_FGFUNC_IKKATUAUTO);		//	22	一括集計金額行（自動）
//
//				// 保管テーブルの保管データを全て削除
//				CdbUcLstHokan mfcRec(m_pDB);
//				mfcRec.DeleteFormSeq(intFormSeq);
//
//				// 残ったデータ行＋一括集計金額行（手動）を一時保管へ
//				pData1->UpdateYokukiMoveToHokan();
//				
//				// 保管テーブルへコピー
//				pView->CopyHokanDataToHokanTable(intFormSeq);
//
//// midori 180537 add -->
//				pData1->UpdateYokukiMoveToHokan2();
//// midori 180537 add <--
//
//// midori 152386 add -->
//				// 欄外データの削除
//				switch (intFormSeq) {
//				case ID_FORMNO_051:
//					pData2 = (CdbUc000Common*)(new CdbUc052Tanaoroshi2(m_pDB));
//					pData2->DeleteAllRecord();
//					delete pData2;
//					break;
//				case ID_FORMNO_091:
//					pData2 = (CdbUc000Common*)(new CdbUc092Kaikakekin2(m_pDB));
//					pData2->DeleteAllRecord();
//					delete pData2;
//					break;
//				}
//// midori 152386 add <--
//			}
//
//			// ④：①～③にて、頁/行、頁計/累計を再設定
//			if (pData1->GetNumPage() > 0) {
//				pView->CmnTblUpdatePageRowAllResetMain();
//			}
//		}
//
//		if (pData1 != NULL) {
//			delete pData1;
//		}
//		if (pView != NULL) {
//			delete pView;
//		}
//	}
//
//	return FUNCTION_OK;
//}
//
////********************************************************************************
////	翌期更新：財務マスターの決算期間を取得
////		IN		int*			決算期間（期首）
////				int*			決算期間（期末）
////		RET		int				FUNCTION_OK:正常終了
////								FUNCTION_NG:エラー
////********************************************************************************
//int CUCHIWAKEApp::UchiYokukiGetZvolumeYmd(long* lngSSymd, long* lngEEymd)
//{
//	// クリア
//	*lngSSymd = 0;
//	*lngEEymd = 0;
//
//	// zvolumeテーブル参照
//	if ( m_pZmSub->VolumeOpen() == 0 ){
//		
//		*lngSSymd = m_pZmSub->zvol->ss_ymd;		// 決算期間(期首)
//		*lngEEymd = m_pZmSub->zvol->ee_ymd;		// 決算期間(期末)
//
//		// テーブルを閉じる
//		m_pZmSub->VolumeClose();
//	}
//	else{
//		// 2007.03 DBエラー対応
//		m_clsFunc.ZmSubErrMsg( m_pZmSub );
//		return FUNCTION_NG;
//	}
//
//	return FUNCTION_OK;
//}
// midori 180503 del <--

//***************************************************************************
//	エラーメッセージ表示＋WM_CLOSE送信
//
//	IN:		CString			エラーメッセージ文言
//
//	RET:	なし			
//***************************************************************************
void ICSExitEx(CString strErrMsg)
{
	// 2回以上の実行を禁止する
	static BOOL flgOnce = FALSE;
	// 2回目以降は、既にWM_CLOSE を送信済のため、実行させない
	// 例外ルートの洗い出し作業は膨大になる事が予想されるため、関数側で対応とする。
	if ( flgOnce ) { return; }
	flgOnce = TRUE;

	// 改行挿入してメッセージを表示
	strErrMsg.Replace(_T("。"), _T("。\r\n"));
	ICSMessageBox(strErrMsg, MB_ICONSTOP);

	// 終了メッセージ送信
	AfxGetMainWnd()->PostMessage(WM_CLOSE, 1, 0);
}
//***************************************************************************
//	内訳書コンバージョンで削除されてしまう、DBで新たに追加した
//	カラムの初期値をセットするスクリプトファイルをコールする。
//
//	IN:		なし
//
//	RET:	BOOL	
//***************************************************************************
BOOL CUCHIWAKEApp::ExecAfterConvert()
{
	char	szSQLServer[30];		// SQLサーバ名
	char	szApno[20];				// アプリケーション番号
	char	szCoCode[20];			// 会社コード
	char	szKikan[20];			// 期末年月日

	// SQLサーバ名,　アプリケーション番号, 会社コード, 期末年月日取得
	if( GetPCOMSELECTCODSN(szSQLServer, szApno, szCoCode, szKikan) == FALSE ){
		return FUNCTION_NG;
	}

	// 取得したデータより、データベース名を作成する
	CString DatabaseName;
	DatabaseName = MakeDatabaseName(szApno, szCoCode, szKikan);

// midori UC_0006 del -->
	//BOOL bRet = CheckAndMakeTable(szSQLServer, DatabaseName, (m_strPath + DB_STR_SQLFILE_UPDATE_CONVERT), DB_STR_SQLUSE_UPDATE, SQL_EXEC_UPDPATE);

	//if (bRet == FALSE) {
	//	SetExitMessage( m_intCoSelType , "勘定科目内訳書マスターのコンバージョン後の処理に失敗しました。" );
	//	return FUNCTION_NG;
	//}
// midori UC_0006 del <--
// midori UC_0006 add -->
	int nRet = CheckAndMakeTable(szSQLServer, DatabaseName, (m_strPath + DB_STR_SQLFILE_UPDATE_CONVERT), DB_STR_SQLUSE_UPDATE, SQL_EXEC_UPDPATE);

	if (nRet == -1) {
		SetExitMessage( m_intCoSelType , "勘定科目内訳書マスターのコンバージョン後の処理に失敗しました。" );
		return(FUNCTION_NG);
	}
	else if(nRet == -2)	{
		AfxGetMainWnd()->PostMessage( WM_CLOSE, 1, 0 );
		return(FUNCTION_NG);
	}
// midori UC_0006 add <--
	 
	return FUNCTION_OK;
}

//***************************************************************************
//	PCOMからSELECTCODSN情報を取得する
//
//	IN:		szSQLServer				SQLサーバ名
//			szApno					アプリケーション番号
//			szCoCode				会社コード
//			szKikan					期末年月日
//
//	RET:	BOOL	
//***************************************************************************
BOOL CUCHIWAKEApp::GetPCOMSELECTCODSN(char* szSQLServer, char* szApno, char* szCoCode, char* szKikan)
{
	int nRetCode = 0;
	memset(szSQLServer,	NULL, sizeof(szSQLServer));
	memset(szApno,		NULL, sizeof(szApno));
	memset(szCoCode,	NULL, sizeof(szCoCode));
	memset(szKikan,		NULL, sizeof(szKikan));

	// C:\ICSWin\COMM\ログイン名\PCOM.INFの内容を読み込む
	nRetCode = PCOM_GetString( "SELECTCODSN", szSQLServer, szApno, szCoCode, szKikan);

	if( nRetCode < 4) {
//	if( nRetCode == -1) {
		// PCOM_GetString() でうまく取得できなかった場合
		// ここで 4 未満としているのは、
		//  -1 が失敗（多分 PCOM_GetString() 自体で何らかの失敗)
		//  正の値は、取得できたパラメータ数
		//    今回の場合、取得項目が4項目なので 4未満の条件ととした。
		//    # 第一引数に、存在しないセクション名を指定しても 0 が返ってくる
		//    # (-1は返ってこない)
		//ICSExit( 0x0205, "情報が不完全です。");
		SetExitMessage( m_intCoSelType , "コモン情報が、取得できません。(SELECTCODSN)" , 0x0205 );
		return FALSE;
	}

	return TRUE;
}

// 21/07/21 add -->
////***************************************************************************
////	PCOMにSELECTCODSN情報をセットする
////
////	IN:		szSQLServer				SQLサーバ名
////			szApno					アプリケーション番号
////			szCoCode				会社コード
////			szKikan					期末年月日
////
////	RET:	BOOL	
////***************************************************************************
//BOOL CUCHIWAKEApp::SetPCOMSELECTCODSN(char* szSQLServer, char* szApno, char* szCoCode, char* szKikan)
//{
//	int nRetCode = 0;
//
//	// C:\ICSWin\COMM\ログイン名\PCOM.INFに書き込む
//	nRetCode = PCOM_SetString("SELECTCODSN", szSQLServer, szApno, szCoCode, szKikan);
//
//	if( nRetCode < 4)	{
//	//	SetExitMessage( m_intCoSelType , "コモン情報が、取得できません。(SELECTCODSN)" , 0x0205 );
//	//	return FALSE;
//	}
//
//	return TRUE;
//}
// 21/07/21 add <--

//***************************************************************************
//	PCOMからSELECTCODSN情報を取得する
//
//	IN:		szSQLServer				SQLサーバ名
//			szApno					アプリケーション番号
//			szCoCode				会社コード
//			szKikan					期末年月日
//
//	RET:	BOOL	
//***************************************************************************
BOOL CUCHIWAKEApp::GetPCOMSELECTCODSNPublic(char* szSQLServer, char* szApno, char* szCoCode, char* szKikan)
{
	return GetPCOMSELECTCODSN(szSQLServer, szApno, szCoCode, szKikan);
}

// 21/07/21 add -->
////***************************************************************************
////	PCOMにSELECTCODSN情報をセットする
////
////	IN:		szSQLServer				SQLサーバ名
////			szApno					アプリケーション番号
////			szCoCode				会社コード
////			szKikan					期末年月日
////
////	RET:	BOOL	
////***************************************************************************
//BOOL CUCHIWAKEApp::SetPCOMSELECTCODSNPublic(char* szSQLServer, char* szApno, char* szCoCode, char* szKikan)
//{
//	return SetPCOMSELECTCODSN(szSQLServer, szApno, szCoCode, szKikan);
//}
// 21/07/21 add <--

//***************************************************************************
//	現在開いている会社のデータベース名を取得する
//
//	IN:		なし
//
//	RET:	CString 	
//***************************************************************************
CString CUCHIWAKEApp::GetTargetDatabaseName()
{
	return m_DatabaseName;
}

// midori 181002 del -->
////***************************************************************************
////	内訳書マスター（内訳書関連の全てのテーブル）を削除し、
////	作成済みスイッチを落とす
////
////	IN:		なし
////
////	RET:	TRUE:成功
////			FALSE:失敗
////***************************************************************************
//BOOL CUCHIWAKEApp::DeleteAllTables()
//{
//	CString strSQL;
//// midori 6 add -->
//	CString	cst=_T("");
//	CDBPrtbl pt;
//// midori 6 add <--
//
//	// テーブル名の頭3文字が"uc_"のテーブルを削除
//	strSQL  = _T("DECLARE @name char(128) ");
//	strSQL += _T("DECLARE cur CURSOR for SELECT name from sys.tables where name like 'uc_%' ");
//	strSQL += _T("OPEN cur FETCH cur INTO @name WHILE @@fetch_status = 0 ");
//	strSQL += _T("BEGIN ");
//	strSQL += _T(	"exec('DROP TABLE ' + @name) ");
//	strSQL += _T(	"FETCH cur INTO @name ");
//	strSQL += _T("END CLOSE cur DEALLOCATE cur ");
//
//	// zvolumeのsub_swのスイッチを落とす
//	strSQL += _T("UPDATE zvolume SET sub_sw = sub_sw - 32 "); // 本来ビット演算でDB_UC_TABLE_CREATE_ONを落としたいが、smallintの為減算で処理
//
//	// vtblの内訳書レコードを削除
//	strSQL.Format( "%s DELETE FROM vtbl WHERE type = %d AND objname = '%s' ", strSQL, DB_UC_VTBL_TYPE, DB_UC_VTBL_OBJNAME);
//
//// midori 180101 add -->
//	// owntb の表紙情報を削除
//	strSQL.Format( "%s DELETE FROM owntb WHERE apl_name = 'UCHIWAKE' AND itm_name = 'HyoshiData' AND itm_seq = 0", strSQL);
//// midori 180101 add <--
//
//	// トランザクション開始（ロック）
//	m_pZmSub->m_database->BeginTrans();
//
//	try{
//		// SQL実行
//		m_pZmSub->m_database->ExecuteSQL( strSQL );
//
//		// トランザクションのコミット
//		m_pZmSub->m_database->CommitTrans();
//	}
//	catch(CException* e){
//		// 失敗したらロールバック
//		m_pZmSub->m_database->Rollback();
//		e->Delete();
//		return FALSE;
//	}
//
//// midori 6 add -->
//	if(m_swUtwInfoRegister == 1)	{
//		// 決算期間情報・会社情報　初期化
//		m_pZmSub->m_database->BeginTrans();
//		try{
//			m_pZmSub->m_database->ExecuteSQL( _T("delete from dbo.prfkin") );
//		}
//		catch( CDBException *e ){
//			m_pZmSub->m_database->Rollback();
//			e->Delete();
//			return FALSE;
//		}
//		m_pZmSub->m_database->CommitTrans();
//
//		pt.SetMasterDB( m_pZmSub->m_database );
//		if( pt.InitProof(m_pZmSub) == -1 ){
//			return FALSE;
//		}
//
//		// 会社情報
//		m_pZmSub->VolumeOpen();
//		cst = m_pZmSub->zvol->c_nam;
//		m_pZmSub->VolumeClose();
//		// SQLコマンド作成
//		strSQL.Format(_T("UPDATE [cpdata] SET cpyg='%s', cpua='', cpda='' "),cst);
//		strSQL += _T("WHERE cpkofs = 0");
//
//		// SQL文を実行
//		m_pZmSub->m_database->BeginTrans();
//		try{
//			m_pZmSub->m_database->ExecuteSQL(strSQL);
//		}
//		catch( CDBException *e ){
//			m_pZmSub->m_database->Rollback();
//			e->Delete();
//			return FALSE;
//		}
//		m_pZmSub->m_database->CommitTrans();
//	}
//// midori 6 add <--
//
//
//	return TRUE;
//}
// midori 181002 del <--

//***************************************************************************
// 翌期更新時の保管データを保管テーブルへコピーし忘れを修復する
// 保管テーブルのFormSeq = 0のレコードも削除する
//***************************************************************************
int CUCHIWAKEApp::MaintenanceHokanData()
{
	// 一度メンテナンスを行っていれば、それ以降行わない
	CdbUcInfMain rsMain(m_pDB);
	if (rsMain.Init() != DB_ERR_OK) {
		rsMain.Fin();
		return FUNCTION_NG;
	}
	if ( rsMain.IsEOF() )	return FUNCTION_NG;

	CCtrlMainGeneralVar cm(rsMain);
	if(cm.bHokanDataMente){
		rsMain.Fin();
		return TRUE;
	}

	CdbUcLstHokan mfcHokan(m_pDB);
	mfcHokan.DeleteFormSeq(0);
	mfcHokan.Fin();

	for (int i = ID_FORMNO_011; i <= ID_FORMNO_162; i++) {
		// ①.預貯金 ～ ⑯-2.雑損失
		if ( MantenanceHokanDataSub(i) != FUNCTION_OK ){
			return FUNCTION_NG;
		}
	}
	for (int i = 0; i < 20; i++) {
		// ⑰-x.その他１＋⑱-x.その他２
		if( MantenanceHokanDataSub(ID_FORMNO_171 + i) != FUNCTION_OK ){
			return FUNCTION_NG;
		}
		if( MantenanceHokanDataSub(ID_FORMNO_181 + i) != FUNCTION_OK ){
			return FUNCTION_NG;
		}
	}

	rsMain.Edit();
	cm.bHokanDataMente = TRUE;
	cm.Set(rsMain);
	rsMain.Update();
	rsMain.Fin();
	return FUNCTION_OK;
}

//***************************************************************************
// 保管テーブル修復の実働部分
// 1.保管データ移動済みサインを全てONにする　
// →個別に管理すると「一括で保管へ移動」時に現在開いていない様式では移動済みサインが立たず、
// その様式でF7等内のCopyHokanDataToHokanTable()で保管データを保管テーブルへコピーしようとして
// 主キー重複が発生する可能性がある
// 2.様式シーケンス0のレコードを削除する
// 3.一旦保管テーブルを全て削除する
// 4.様式テーブル(uc_011_yotyokin～uc_181_sonota2まで)内の保管データ
// (FgShow = ID_FGSHOW_HOKAN)を保管テーブルにコピーする
// 5.メンテナンス済みサインを立てる
//***************************************************************************
BOOL CUCHIWAKEApp::MantenanceHokanDataSub(int nFormSeq)
{
	// 保管テーブルコピー済みサインを全てON
	SetONAddHokanSign(nFormSeq);

	CfrmUc000Common* pView;
	pView = UchiwakeCreateFormViewObject(nFormSeq);
	if( pView == NULL )	return FUNCTION_NG;
	// 初期設定
	pView->CmnSetInitialUchiwakeInfo(nFormSeq);		// 共通データ
	pView->virSetInitialValue();				// 帳表固有データ

	// 様式テーブルと保管テーブルのアンマッチをチェック
	// ※漏れなく全ての様式に対してメンテナンスを行いたいので、チェックをはずす
	//int st = IsUnmatchHokanTable(nFormSeq, pView);
	//if( st == FALSE )	return FUNCTION_OK;

	// 保管テーブル全レコード削除
	CdbUcLstHokan mfcHokan(m_pDB);
	mfcHokan.DeleteFormSeq(nFormSeq);

	// 現在の様式の保管データを保管テーブルへコピー
	pView->CopyHokanDataToHokanTable(nFormSeq);

	delete pView;
	return FUNCTION_OK;
}

//***************************************************************************
// CdbUcInfSubのbAddHokanを強制的にONにする
//***************************************************************************
BOOL CUCHIWAKEApp::SetONAddHokanSign(int nFormSeq)
{
	CdbUcInfSub	mfcSub( m_pDB );
	if ( mfcSub.RequeryFormSeq( nFormSeq ) != DB_ERR_OK ){
		return FUNCTION_NG;
	}
	if ( !(mfcSub.IsEOF()) ){
		mfcSub.MoveFirst();
	}

	CCtrlSubGeneralVar	cs( mfcSub );
	mfcSub.Edit();
	cs.bAddHokan = TRUE;
	cs.Set( mfcSub );
	mfcSub.Update();
	mfcSub.Fin();

	return FUNCTION_OK;
}

//***************************************************************************
// 保管テーブルと各様式テーブルの保管データの個数の比較
// 未使用
// 一致していたらメンテナンス必要なし
// TRUE : 不一致　FALSE : 一致
//***************************************************************************
//BOOL CUCHIWAKEApp::IsUnmatchHokanTable(int nFormSeq, CfrmUc000Common* pView)
//{
//	CdbUcLstHokan mfcHokan( m_pDB );
//	int nCntHokan = mfcHokan.GetCountFormSeq( nFormSeq );
//	if( nCntHokan == -1 )	return TRUE;
//
//	CdbUc000Common*		pData = NULL;
//	pData = pView->virGetDataTableObject();
//	if( pData == NULL )		return TRUE;
//	int nCntYoshiki = pData->GetCountFgShow( ID_FGSHOW_HOKAN );
//
//	int ret = TRUE;
//	if( nCntHokan == nCntYoshiki ){
//		ret = FALSE;
//	}
//
//	delete pData;
//	return ret;
//}

void CUCHIWAKEApp::SetNonActiveFlag(BOOL st)
{
	CFrameWnd*			pFrame	= (CFrameWnd*)AfxGetMainWnd();
	if(pFrame == NULL)	return;
	CMDIChildWnd*		pChild	= (CMDIChildWnd *)pFrame->GetActiveFrame();
	if(pChild == NULL)	return;
	CfrmUc000Common*	frmView	= (CfrmUc000Common*)pChild->GetActiveView();
	if(frmView == NULL)	return;

	frmView->m_ActiveNonFlag = st;
}

// midori 160612 add -->
// midori 180503 del -->
////***************************************************************************
//// 過年度財務マスターの「決算確定」を、内訳書の「入力データを確定する」にセットする
////***************************************************************************
//void CUCHIWAKEApp::CnvDbToBCD( long pYmd, UCHAR* pYear, UCHAR* pMon, UCHAR* pDay )
//{
//	int					yy;
//	int					mm;
//	int					dd;
//	CVolDateDB			vd;
//
//	//	年
//	vd.db_datecnv(( ( pYmd / 10000 ) % 100 ),&yy,0,2);
//	*pYear	= yy ;
//	//	月
//	mm	= ( pYmd % 10000 ) / 100 ;
//	int_bcd(pMon,mm,1);
//	//	日
//	dd	= ( pYmd % 10000 ) % 100 ;
//	int_bcd(pDay,dd,1);
//}
//
//int	CUCHIWAKEApp::KakuteiSet()
//{
//	char			szSQLServer[64];	// SQLサーバ名
//	char			szApno[16];			// アプリケーション番号
//	char			szCoCode[16];		// 会社コード
//	char			szKikan[16];		// 期末年月日
//	char			tmp1[16],tmp2[16],tmp3[16];
//	int				kmst=0;
//	int				max=0;
//	int				nen=0;
//	int				n_kikan=0,z_kikan=0;
//	int				eymd=0;
//	int				rv=0;
//	int				ksw=0;
//	UCHAR			yy,mm,dd;
//	UCHAR			ymd[3]={0},zymd[3]={0};
//	BYTE			apno=0;
//	CString			m_svr=_T("");
//	CString			msg=_T("");
//	ICSMCTL			m_mctl;
//	CO_CTL_DB		coctl={0};
//	CO_CTL_Array	coarray;
//	CDBZmSub		*kzms;
//	CZmGen8			zg;
//	ZMGEN_CHKINFO	zc={0};
//
//	m_pZmSub->VolumeOpen();
//	apno = m_pZmSub->zvol->apno;
//	eymd = m_pZmSub->zvol->ee_ymd1;
//	m_pZmSub->VolumeClose();
//
//	PCOM_GetString(_T("SelectSqlServer"),m_svr.GetBuffer(256));
//	m_svr.ReleaseBuffer();
//	if(m_mctl.SetDatabase(m_svr) < 0)	{
//		ICSExit(0,_T("マスター管理テーブルの読み込みに失敗しました。"));
//		return(FUNCTION_NG);
//	}
//
//	if(m_mctl.vctl_open(MODE_READ,apno,0) == 0)	{
//		PCOM_GetString("SELECTCODSN",szSQLServer,szApno,szCoCode,szKikan);
//		coctl.c_apno = apno;
//		int_bcd(coctl.c_cod,atoi(szCoCode),4);
//		if(m_mctl.vctl_coinf(ICSMCTL::ZAIMU_SW,&coctl,&coarray) == 0)	{
//			n_kikan = atoi(szKikan);
//			nen = (n_kikan / 10000);
//			nen %= 100;
//			CnvDbToBCD(n_kikan,&yy,&mm,&dd);
//
//			int_bcd(&ymd[0],nen,1);
//			ymd[1] = mm;
//			ymd[2] = dd;
//
//			max = (int)coarray.GetCount();
//			if(max > 1)	{
//				if(memcmp(ymd,&coarray[0].c_kikan[3],sizeof(ymd)) == 0)	{
//					memset(tmp1,0,16);
//					memset(tmp2,0,16);
//					memset(tmp3,0,16);
//					sprintf_s(tmp1,16,"%08d",eymd);
//					memcpy(tmp2,&tmp1[2],6);
//					l_pack(tmp3,tmp2,6);
//					if(memcmp(tmp3,&coarray[1].c_kikan[3],3) == 0)	{
//						kmst = 1;
//						memmove(zymd,&coarray[1].c_kikan[3],sizeof(zymd));
//						z_kikan = bcd_int(zymd,3);
//					}
//				}
//			}
//		}
//		m_mctl.vctl_close();
//	}
//	if(kmst)	{
//		kzms = new CDBZmSub;
//		kzms->ExitMode(1);
//		if(kzms->SetMasterDB(DB_ICS_VERSION,szSQLServer,atoi(szApno),atoi(szCoCode),z_kikan) == -1)	{
//			ICSMessageBox(kzms->GetZmErrMsg(),MB_ICONSTOP);
//			return(FUNCTION_NG);
//		}
//		rv = zg.CheckDBMasterState(kzms,msg,zc);
//		if(rv == -1 || rv == -2)	{
//			ICSMessageBox(zg.GetErrorMsg(),MB_ICONSTOP);
//			return(FUNCTION_NG);
//		}
//		else if(rv == 1)	{
//			ICSMessageBox(msg);
//		}
//		// 過年度の確定サインを更新する
//		kzms->VolumeOpen();
//		if(kzms->zvol->tl_cor & 0x10)	ksw=0;	// 財務決算確定ＯＦＦ
//		else							ksw=1;	// 　　 〃 　　ＯＮ
//		kzms->VolumeClose();
//
//// midori UC_0009 del -->
////// midori 160612_2 cor -->
////		//CdbUcInfMain	rsMain(kzms->m_database);
////		//if(rsMain.Init() == DB_ERR_OK){
////		//	rsMain.Edit();
////		//	CCtrlMainGeneralVar cm(rsMain);
////		//	cm.bDataKakutei = ksw;
////		//	cm.Set(rsMain);
////		//	rsMain.Update();
////		//}
////		//rsMain.Fin();
////// -----------------------
////		CdbUcInfMain*	prsMain;
////		prsMain = new CdbUcInfMain(kzms->m_database);
////		if(prsMain->Init() == DB_ERR_OK){
////			if((prsMain->m_GeneralVar[1] & BIT_D3) == 0)	{
////				prsMain->Edit();
////// midori 152366 cor -->
////				//if(ksw)	prsMain->m_GeneralVar[1] |= BIT_D2;
////				//else	prsMain->m_GeneralVar[1] &= ~BIT_D2;
////// ---------------------
////				if((prsMain->m_GeneralVar[1] & BIT_D2) != BIT_D2) {
////					if(ksw)	prsMain->m_GeneralVar[1] |= BIT_D2;
////					else	prsMain->m_GeneralVar[1] &= ~BIT_D2;
////				}
////// midori 152366 cor <--
////				prsMain->m_GeneralVar[1] |= BIT_D3;
////				prsMain->Update();
////			}
////		}
////		prsMain->Fin();
////		delete prsMain;
////// midori 160612_2 cor <--
//// midori UC_0009 del <--
//// midori UC_0009 add -->
//		if(CheckGeneralVar(kzms) != 0)	{
//			CdbUcInfMain*	prsMain;
//			prsMain = new CdbUcInfMain(kzms->m_database);
//			if(prsMain->Init() == DB_ERR_OK){
//				if((prsMain->m_GeneralVar[1] & BIT_D3) == 0)	{
//					prsMain->Edit();
//					if((prsMain->m_GeneralVar[1] & BIT_D2) != BIT_D2) {
//						if(ksw)	prsMain->m_GeneralVar[1] |= BIT_D2;
//					}
//					prsMain->m_GeneralVar[1] |= BIT_D3;
//					prsMain->Update();
//				}
//			}
//			prsMain->Fin();
//			delete prsMain;
//		}
//// midori UC_0009 add <--
//
//		delete kzms;
//
//// midori M-16113007 del -->
//		//// 当期の確定サインを更新する
//		//CdbUcInfMain	rsMain2(m_pDB);
//		//if(rsMain2.Init() == DB_ERR_OK){
//		//	rsMain2.Edit();
//		//	CCtrlMainGeneralVar cm(rsMain2);
//		//	cm.bDataKakutei = 0;
//		//	cm.Set(rsMain2);
//		//	rsMain2.Update();
//		//}
//		//rsMain2.Fin();
//// midori M-16113007 del <--
//	}
//// midori M-16113007 add -->
//	// 当期の確定サインを更新する
//	CdbUcInfMain	rsMain2(m_pDB);
//	if(rsMain2.Init() == DB_ERR_OK){
//		rsMain2.Edit();
//		CCtrlMainGeneralVar cm(rsMain2);
//		cm.bDataKakutei = 0;
//		cm.Set(rsMain2);
//// midori 151402 add -->
//		rsMain2.m_GeneralVar[1] &= ~BIT_D3;
//// midori 151402 add <--
//// midori UC_0004 add -->
//		rsMain2.m_GeneralVar[2] |= BIT_D0;
//// midori UC_0004 add <--
//		rsMain2.Update();
//	}
//	rsMain2.Fin();
//// midori M-16113007 add <--
//
//	return(FUNCTION_OK);
//}
//// midori 160612 add <--
//
//// midori UC_0004 add -->
////***************************************************************************
//// 【戻り値】	0:入力確定の更新不要 1:入力確定の更新必要
////***************************************************************************
//int	CUCHIWAKEApp::ChkKakuteiUpd(void)
//{
//	int				ret=0;				// 戻り値
//	CdbUcInfMain*	pfcRec;				// uc_inf_mainテーブルクラス
//
//	// 一度でも過年度財務マスターから確定サインを転記しているかのフラグを取得
//	pfcRec = new CdbUcInfMain(m_pZmSub->m_database);
//	if(pfcRec->Init() == DB_ERR_OK)	{
//		if(!(pfcRec->IsEOF()))	{
//			pfcRec->MoveFirst();
//			// 入力確定更新サインを取得	0:更新する	1:更新しない
//			if(pfcRec->m_GeneralVar[2] & BIT_D0)	ret = 1;
//		}
//	}
//	pfcRec->Fin();
//	delete pfcRec;
//
//	return(ret);
//}
//// midori UC_0004 add <--

//// midori 152110 cor -->
////// midori 160612_2 add -->
//////***************************************************************************
////// ①過年度マスターを選択している
////// ②当年度内訳書マスターが更新されていない
////// ③当年度内訳書マスターが更新されていない状態で初めて過年度マスターを起動した
////// →上記の場合、過年度財務マスターの確定サインを、過年度内訳書マスターの
////// 　確定サインにセットする
//////***************************************************************************
////int	CUCHIWAKEApp::KanendoKakuteiSet()
////{
////	char			szSQLServer[64];	// SQLサーバ名
////	char			szApno[16];			// アプリケーション番号
////	char			szCoCode[16];		// 会社コード
////	char			szKikan[16];		// 期末年月日
////	int				eymd=0;				// 過年度の終了年月日
////	int				eymd2=0;			// 当年度からみた、１期前の終了年月日
////	int				kmst=0;
////	int				max=0;
////	int				nen=0;
////	int				z_kikan=0,t_kikan=0;
////	int				rv=0;
////	int				zsymd=0,zeymd=0;	// 当期財務の決算期間
////	int				zksgn=0;
////	int				usymd=0,ueymd=0;	// 当期内訳書の決算期間
////	int				uksgn=0;
////	int				sw=0;
////	UCHAR			yy,mm,dd;
////	UCHAR			ymd1[3]={0},ymd2[3]={0};
////	UCHAR			tymd[3]={0};
////	BYTE			apno=0;
////	CString			m_svr=_T("");
////	CString			msg=_T("");
////	CO_CTL_DB		coctl={0};
////	CO_CTL_Array	coarray;
////	ICSMCTL			m_mctl;
////	CDBZmSub		*tzms;
////	CZmGen8			zg;
////	ZMGEN_CHKINFO	zc={0};
////	CdbUcInfMain*	pfcRec;				// uc_inf_mainテーブルクラス
////
////	// ①coarrayの[1]番目と決算期間が同一なら過年度マスター
////	m_pZmSub->VolumeOpen();
////	apno = m_pZmSub->zvol->apno;
////	eymd = m_pZmSub->zvol->ee_ymd;
////	if(m_pZmSub->zvol->tl_cor & 0x10)	zksgn=0;	// 財務決算確定ＯＦＦ
////	else								zksgn=1;	// 　　 〃 　　ＯＮ
////	m_pZmSub->VolumeClose();
////
////	PCOM_GetString(_T("SelectSqlServer"),m_svr.GetBuffer(256));
////	m_svr.ReleaseBuffer();
////	if(m_mctl.SetDatabase(m_svr) < 0)	{
////		ICSExit(0,_T("マスター管理テーブルの読み込みに失敗しました。"));
////		return(FUNCTION_NG);
////	}
////	if(m_mctl.vctl_open(MODE_READ,apno,0) == 0)	{
////		PCOM_GetString("SELECTCODSN",szSQLServer,szApno,szCoCode,szKikan);
////		coctl.c_apno = apno;
////		int_bcd(coctl.c_cod,atoi(szCoCode),4);
////		if(m_mctl.vctl_coinf(ICSMCTL::ZAIMU_SW,&coctl,&coarray) == 0)	{
////			z_kikan = eymd;
////			nen = (z_kikan / 10000);
////			nen %= 100;
////			CnvDbToBCD(z_kikan,&yy,&mm,&dd);
////			int_bcd(&ymd1[0],nen,1);
////			ymd1[1] = mm;
////			ymd1[2] = dd;
////			max = (int)coarray.GetCount();
////			if(max > 1)	{
////				memcpy(ymd2,&coarray[1].c_kikan[3],3);
////				if(memcmp(ymd1,ymd2,3) == 0)	{
////					kmst = 1;
////					memmove(tymd,&coarray[0].c_kikan[3],3);
////					t_kikan = bcd_int(tymd,3);
////				}
////			}
////		}
////		m_mctl.vctl_close();
////	}
////
////	// ②当年度マスターの財務ｄｂに接続する
////	if(kmst)	{
////		// 一度でも過年度財務マスターから確定サインを転記していればここで終了
////		pfcRec = new CdbUcInfMain(m_pZmSub->m_database);
////		if(pfcRec->Init() == DB_ERR_OK)	{
////			if(!(pfcRec->IsEOF()))	{
////				pfcRec->MoveFirst();
////				if(pfcRec->m_GeneralVar[1] & 0x08)	uksgn = 1;
////				else								uksgn = 0;
////			}
////		}
////		pfcRec->Fin();
////		delete pfcRec;
////		if(uksgn)	{
////			return(FUNCTION_OK);
////		}
////		// 当期財務ｄｂに接続
////		tzms = new CDBZmSub;
////		tzms->ExitMode(1);
////		if(tzms->SetMasterDB(DB_ICS_VERSION,szSQLServer,atoi(szApno),atoi(szCoCode),t_kikan) == -1)	{
////			ICSMessageBox(tzms->GetZmErrMsg(),MB_ICONSTOP);
////			return(FUNCTION_NG);
////		}
////		rv = zg.CheckDBMasterState(tzms,msg,zc);
////		if(rv == -1 || rv == -2)	{
////			ICSMessageBox(zg.GetErrorMsg(),MB_ICONSTOP);
////			return(FUNCTION_NG);
////		}
////		else if(rv == 1)	{
////			ICSMessageBox(msg);
////		}
////		// 財務の決算期間
////		tzms->VolumeOpen();
////		zsymd = tzms->zvol->ss_ymd;
////		zeymd = tzms->zvol->ee_ymd;
////		eymd2 = tzms->zvol->ee_ymd1;
////		tzms->VolumeClose();
////		// 内訳書の決算期間
////		pfcRec = new CdbUcInfMain(tzms->m_database);
////		if(pfcRec->Init() == DB_ERR_OK)	{
////			if(!(pfcRec->IsEOF()))	{
////				pfcRec->MoveFirst();
////				usymd = pfcRec->m_ssymd;
////				ueymd = pfcRec->m_eeymd;
////			}
////		}
////		pfcRec->Fin();
////		delete pfcRec;
////		sw=0;
////		// 選択した過年度マスターが、当期からみて１期前で
////		if(eymd == eymd2) {
////			// 当期内訳書が作成されていて
////			if((usymd != 0) || (ueymd != 0)) {
////				// 財務と決算期間が異なる場合のみ
////				if((zsymd != usymd) || (zeymd != ueymd))	{
////					sw=1;
////				}
////			}
////		}
////		if(sw == 1)	{
////			// 内訳書過年度確定サインの更新フラグを参照してフラグがＯＦＦであれば
////			// 過年度財務の確定サインをセットする
////			pfcRec = new CdbUcInfMain(m_pZmSub->m_database);
////			if(pfcRec->Init() == DB_ERR_OK){
////				pfcRec->Edit();
////				if(zksgn)	pfcRec->m_GeneralVar[1] |= BIT_D2;
////				else		pfcRec->m_GeneralVar[1] &= ~BIT_D2;
////				pfcRec->m_GeneralVar[1] |= BIT_D3;
////				pfcRec->Update();
////			}
////			pfcRec->Fin();
////			delete pfcRec;
////		}
////
////		delete tzms;
////	}
////
////	return(FUNCTION_OK);
////}
////// midori 160612_2 add <--
//// ---------------------
//// ******************************************************************************************************************************
//// 処理１
//// マスターによっては財務の翌期更新直後に過年度の確定サインが更新されない場合があるので、そのための修正
//// -----------------------------------------------------------------------------------------------------------------------------
//// 　入力データを確定する				uc_inf_main.GeneralVar[1] & 0x04
//// 　過年度確定サインの更新フラグ		uc_inf_main.GeneralVar[1] & 0x08
//// -----------------------------------------------------------------------------------------------------------------------------
//// 　①一度翌期更新したマスターについて「財務マスターコピー」で過年度を当年度としてコピーする。
////					GeneralVar[1] & 0x04	GeneralVar[1] & 0x08	財務の確定フラグ
////		(過年度)			OFF						ON					OFF
////		(当年度)			OFF						OFF					OFF
////			… 過年度を当年度としてコピーする
////		(当年度)			OFF						ON	※				OFF
//// 　②過年度を当年度としてコピーしたマスターを翌期更新(財務)する。
////		(過年度)			OFF						ON	※				ON
////		(当年度)			OFF						ON	※				OFF
//// 
//// 　※通常、当年度の (GeneralVar[1] & 0x08) がONになっていることはありえないが、過年度を当年度としてコピーするとONになります。
//// 　　当年度の (GeneralVar[1] & 0x08) がONになっている場合は、当年度・過年度ともにOFFにする。
//// 　　(過年度マスターを起動した場合も同じことをする)
//// ******************************************************************************************************************************
//// 処理２
//// 当年度内訳書マスターが更新されていない状態で初めて過年度マスターを起動した場合、過年度財務マスターの確定サインを
//// 過年度内訳書マスターの確定サインにセットする
//// -----------------------------------------------------------------------------------------------------------------------------
//// 　①過年度マスターを選択している
//// 　②当年度内訳書マスターが更新されていない
//// 　③当年度内訳書マスターが更新されていない状態で初めて過年度マスターを起動した
//// 　→上記の場合、過年度財務マスターの確定サインを、過年度内訳書マスターの
//// 　　確定サインにセットする
//// ******************************************************************************************************************************
//int	CUCHIWAKEApp::KanendoKakuteiSet()
//{
//	char			szSQLServer[64];	// SQLサーバ名
//	char			szApno[16];			// アプリケーション番号
//	char			szCoCode[16];		// 会社コード
//	char			szKikan[16];		// 期末年月日
//	int				eymd=0;				// 過年度の終了年月日
//	int				eymd2=0;			// 当年度からみた、１期前の終了年月日
//	int				kmst=0;
//	int				max=0;
//	int				nen=0;
//	int				z_kikan=0,t_kikan=0;
//	int				rv=0;
//	int				zsymd=0,zeymd=0;	// 当期財務の決算期間
//	int				zksgn=0;
//	int				usymd=0,ueymd=0;	// 当期内訳書の決算期間
//	int				uksgn=0;
//	int				sw=0;
//	UCHAR			yy,mm,dd;
//	UCHAR			ymd1[3]={0},ymd2[3]={0};
//	UCHAR			tymd[3]={0};
//	BYTE			apno=0;
//	CString			m_svr=_T("");
//	CString			msg=_T("");
//	CO_CTL_DB		coctl={0};
//	CO_CTL_Array	coarray;
//	ICSMCTL			m_mctl;
//	CDBZmSub		*tzms;
//	CZmGen8			zg;
//	ZMGEN_CHKINFO	zc={0};
//	CdbUcInfMain*	pfcRec;				// uc_inf_mainテーブルクラス
//
//	// ①coarrayの[1]番目と決算期間が同一なら過年度マスター
//	m_pZmSub->VolumeOpen();
//	apno = m_pZmSub->zvol->apno;
//	eymd = m_pZmSub->zvol->ee_ymd;
//	if(m_pZmSub->zvol->tl_cor & 0x10)	zksgn=0;	// 財務決算確定ＯＦＦ
//	else								zksgn=1;	// 　　 〃 　　ＯＮ
//	m_pZmSub->VolumeClose();
//
//	PCOM_GetString(_T("SelectSqlServer"),m_svr.GetBuffer(256));
//	m_svr.ReleaseBuffer();
//	if(m_mctl.SetDatabase(m_svr) < 0)	{
//		ICSExit(0,_T("マスター管理テーブルの読み込みに失敗しました。"));
//		return(FUNCTION_NG);
//	}
//	if(m_mctl.vctl_open(MODE_READ,apno,0) == 0)	{
//		PCOM_GetString("SELECTCODSN",szSQLServer,szApno,szCoCode,szKikan);
//		coctl.c_apno = apno;
//		int_bcd(coctl.c_cod,atoi(szCoCode),4);
//		if(m_mctl.vctl_coinf(ICSMCTL::ZAIMU_SW,&coctl,&coarray) == 0)	{
//			z_kikan = eymd;
//			nen = (z_kikan / 10000);
//			nen %= 100;
//			CnvDbToBCD(z_kikan,&yy,&mm,&dd);
//			int_bcd(&ymd1[0],nen,1);
//			ymd1[1] = mm;
//			ymd1[2] = dd;
//			max = (int)coarray.GetCount();
//			if(max > 1)	{
//				memcpy(ymd2,&coarray[1].c_kikan[3],3);
//				if(memcmp(ymd1,ymd2,3) == 0)	{
//					kmst = 1;
//					memmove(tymd,&coarray[0].c_kikan[3],3);
//					t_kikan = bcd_int(tymd,3);
//				}
//			}
//			// 当年度マスター起動時(kmst!=1)に過年度マスターを参照するため
//			// 期間を取得しておく
//			z_kikan = 0;
//			if(max > 1)	{
//				memcpy(ymd2,&coarray[0].c_kikan[3],3);
//				if(memcmp(ymd1,ymd2,3) == 0)	{
//					memmove(tymd,&coarray[1].c_kikan[3],3);
//					z_kikan = bcd_int(tymd,3);
//				}
//			}
//		}
//		m_mctl.vctl_close();
//	}
//
//	// ②当年度マスターの財務ｄｂに接続する
//	if(kmst)	{
//		// 一度でも過年度財務マスターから確定サインを転記しているかのフラグを取得
//		pfcRec = new CdbUcInfMain(m_pZmSub->m_database);
//		if(pfcRec->Init() == DB_ERR_OK)	{
//			if(!(pfcRec->IsEOF()))	{
//				pfcRec->MoveFirst();
//				if(pfcRec->m_GeneralVar[1] & 0x08)	uksgn = 1;
//				else								uksgn = 0;
//			}
//		}
//		pfcRec->Fin();
//		delete pfcRec;
//		// 当期財務ｄｂに接続
//		tzms = new CDBZmSub;
//		tzms->ExitMode(1);
//		if(tzms->SetMasterDB(DB_ICS_VERSION,szSQLServer,atoi(szApno),atoi(szCoCode),t_kikan) == -1)	{
//			ICSMessageBox(tzms->GetZmErrMsg(),MB_ICONSTOP);
//			return(FUNCTION_NG);
//		}
//		rv = zg.CheckDBMasterState(tzms,msg,zc);
//		if(rv == -1 || rv == -2)	{
//			ICSMessageBox(zg.GetErrorMsg(),MB_ICONSTOP);
//			return(FUNCTION_NG);
//		}
//		else if(rv == 1)	{
//			ICSMessageBox(msg);
//		}
//
//// midori 152375 del -->
//		//// 処理１ >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//		//// 当年度財務の過年度確定サインの更新フラグ(uc_inf_main.GeneralVar[1] & 0x08)を参照する
//		//pfcRec = new CdbUcInfMain(tzms->m_database);
//		//if(pfcRec->Init() == DB_ERR_OK)	{
//		//	if(!(pfcRec->IsEOF()))	{
//		//		pfcRec->MoveFirst();
//		//		// フラグがONになっていれば当年度・過年度ともにOFFにする
//		//		if(pfcRec->m_GeneralVar[1] & BIT_D3)	{
//		//			// -------------------------------------
//		//			// 当年度
//		//			// -------------------------------------
//		//			pfcRec->Edit();
//		//			pfcRec->m_GeneralVar[1] &= ~BIT_D3;
//		//			pfcRec->Update();
//		//			// -------------------------------------
//		//			// 過年度
//		//			// -------------------------------------
//		//			uksgn = 0;
//		//		}
//		//	}
//		//}
//		//pfcRec->Fin();
//		//delete pfcRec;
//		//// 処理１ <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//// midori 152375 del <--
//// midori 152375 add -->
//		// 処理１ >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//		int			retVer=0;
//		CdbIcsVtbl*	vtbl;
//
//		vtbl = new CdbIcsVtbl(tzms->m_database);
//		retVer = vtbl->VersionCheck(DB_UC_VTBL_TYPE,DB_UC_VTBL_OBJNAME);
//		delete vtbl;
//		if(retVer > 0)	{
//			// 当年度財務の過年度確定サインの更新フラグ(uc_inf_main.GeneralVar[1] & 0x08)を参照する
//// midori UC_0009 del -->
//			//pfcRec = new CdbUcInfMain(tzms->m_database);
//			//if(pfcRec->Init() == DB_ERR_OK)	{
//			//	if(!(pfcRec->IsEOF()))	{
//			//		pfcRec->MoveFirst();
//			//		// フラグがONになっていれば当年度・過年度ともにOFFにする
//			//		if(pfcRec->m_GeneralVar[1] & BIT_D3)	{
//			//			// -------------------------------------
//			//			// 当年度
//			//			// -------------------------------------
//			//			pfcRec->Edit();
//			//			pfcRec->m_GeneralVar[1] &= ~BIT_D3;
//			//			pfcRec->Update();
//			//			// -------------------------------------
//			//			// 過年度
//			//			// -------------------------------------
//			//			uksgn = 0;
//			//		}
//			//	}
//			//}
//			//pfcRec->Fin();
//			//delete pfcRec;
//// midori UC_0009 del <--
//// midori UC_0009 add -->
//			if(CheckGeneralVar(tzms) != 0)	{
//				pfcRec = new CdbUcInfMain(tzms->m_database);
//				if(pfcRec->Init() == DB_ERR_OK)	{
//					if(!(pfcRec->IsEOF()))	{
//						pfcRec->MoveFirst();
//						// フラグがONになっていれば当年度・過年度ともにOFFにする
//						if(pfcRec->m_GeneralVar[1] & BIT_D3)	{
//							// -------------------------------------
//							// 当年度
//							// -------------------------------------
//							pfcRec->Edit();
//							pfcRec->m_GeneralVar[1] &= ~BIT_D3;
//							pfcRec->Update();
//							// -------------------------------------
//							// 過年度
//							// -------------------------------------
//							uksgn = 0;
//						}
//					}
//				}
//				pfcRec->Fin();
//				delete pfcRec;
//			}
//// midori UC_0009_2 del -->
//			//else	{
//			//	// 当期の内訳書に uc_inf_main.GeneralVarが作成されていない場合、過年度財務マスターの確定サインは更新しない
//			//	uksgn = 1;
//			//}
//// midori UC_0009_2 del <--
//// midori UC_0009 add <--
//		}
//// midori UC_0009_2 del -->
//		//else	{
//		//	// 当期の内訳書が作成されていない場合、過年度財務マスターの確定サインは更新しない
//		//	uksgn = 1;
//		//}
//// midori UC_0009_2 del <--
//		// 処理１ <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//// midori 152375 add <--
//
//		// 一度でも過年度財務マスターから確定サインを転記していれば以下の処理は行わない
//		if(uksgn != 1)	{
//// midori UC_0009_2 del -->
//			//// 財務の決算期間
//			//tzms->VolumeOpen();
//			//zsymd = tzms->zvol->ss_ymd;
//			//zeymd = tzms->zvol->ee_ymd;
//			//eymd2 = tzms->zvol->ee_ymd1;
//			//tzms->VolumeClose();
//			//// 内訳書の決算期間
//			//pfcRec = new CdbUcInfMain(tzms->m_database);
//			//if(pfcRec->Init() == DB_ERR_OK)	{
//			//	if(!(pfcRec->IsEOF()))	{
//			//		pfcRec->MoveFirst();
//			//		usymd = pfcRec->m_ssymd;
//			//		ueymd = pfcRec->m_eeymd;
//			//	}
//			//}
//			//pfcRec->Fin();
//			//delete pfcRec;
//			//sw=0;
//			//// 選択した過年度マスターが、当期からみて１期前で
//			//if(eymd == eymd2) {
//			//	// 当期内訳書が作成されていて
//			//	if((usymd != 0) || (ueymd != 0)) {
//			//		// 財務と決算期間が異なる場合のみ
//			//		if((zsymd != usymd) || (zeymd != ueymd))	{
//			//			sw=1;
//			//		}
//			//	}
//			//}
//			//if(sw == 1)	{
//// midori UC_0009_2 del <--
//				// 内訳書過年度確定サインの更新フラグを参照してフラグがＯＦＦであれば
//				// 過年度財務の確定サインをセットする
//				pfcRec = new CdbUcInfMain(m_pZmSub->m_database);
//				if(pfcRec->Init() == DB_ERR_OK){
//					pfcRec->Edit();
//// midori 152366 cor -->
//					//if(zksgn)	pfcRec->m_GeneralVar[1] |= BIT_D2;
//					//else		pfcRec->m_GeneralVar[1] &= ~BIT_D2;
//// ---------------------
//					if((pfcRec->m_GeneralVar[1] & BIT_D2) != BIT_D2)	{
//						if(zksgn)	pfcRec->m_GeneralVar[1] |= BIT_D2;
//						else		pfcRec->m_GeneralVar[1] &= ~BIT_D2;
//					}
//// midori 152366 cor <--
//					pfcRec->m_GeneralVar[1] |= BIT_D3;
//					pfcRec->Update();
//				}
//				pfcRec->Fin();
//				delete pfcRec;
//// midori UC_0009_2 del -->
//			//}
//// midori UC_0009_2 del <--
//		}
//		delete tzms;
//// midori UC_0004 add -->
//		// 入力確定更新サインをONにする	0:更新する	1:更新しない
//		pfcRec = new CdbUcInfMain(m_pZmSub->m_database);
//		if(pfcRec->Init() == DB_ERR_OK){
//			pfcRec->Edit();
//			pfcRec->m_GeneralVar[2] |= BIT_D0;
//			pfcRec->Update();
//		}
//		pfcRec->Fin();
//		delete pfcRec;
//// midori UC_0004 add <--
//	}
//	else	{
//		// 処理１ >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//		// 当年度財務の過年度確定サインの更新フラグ(uc_inf_main.GeneralVar[1] & 0x08)を参照する
//		uksgn = 0;
//		pfcRec = new CdbUcInfMain(m_pZmSub->m_database);
//		if(pfcRec->Init() == DB_ERR_OK)	{
//			if(!(pfcRec->IsEOF()))	{
//				pfcRec->MoveFirst();
//				// フラグがONになっていればOFFにする
//				if(pfcRec->m_GeneralVar[1] & BIT_D3)	{
//					// -------------------------------------
//					// 当年度
//					// -------------------------------------
//					pfcRec->Edit();
//					pfcRec->m_GeneralVar[1] &= ~BIT_D3;
//					pfcRec->Update();
//					uksgn = 1;
//				}
//			}
//		}
//		pfcRec->Fin();
//		delete pfcRec;
//
//		// フラグがONになっていれば過年度のフラグもOFFにする
//		if(uksgn != 0 && z_kikan != 0)	{
//			// 過年度マスターに接続
//			tzms = new CDBZmSub;
//			tzms->ExitMode(1);
//			if(tzms->SetMasterDB(DB_ICS_VERSION,szSQLServer,atoi(szApno),atoi(szCoCode),z_kikan) == -1)	{
//				ICSMessageBox(tzms->GetZmErrMsg(),MB_ICONSTOP);
//				return(FUNCTION_NG);
//			}
//			rv = zg.CheckDBMasterState(tzms,msg,zc);
//			if(rv == -1 || rv == -2)	{
//				ICSMessageBox(zg.GetErrorMsg(),MB_ICONSTOP);
//				return(FUNCTION_NG);
//			}
//			else if(rv == 1)	{
//				ICSMessageBox(msg);
//			}
//// midori UC_0009 del -->
//			//pfcRec = new CdbUcInfMain(tzms->m_database);
//			//if(pfcRec->Init() == DB_ERR_OK)	{
//			//	if(!(pfcRec->IsEOF()))	{
//			//		pfcRec->MoveFirst();
//			//		pfcRec->Edit();
//			//		pfcRec->m_GeneralVar[1] &= ~BIT_D3;
//			//		pfcRec->Update();
//			//	}
//			//}
//			//delete tzms;
//// midori UC_0009 del <--
//// midori UC_0009 add -->
//			if(CheckGeneralVar(tzms) != 0)	{
//				pfcRec = new CdbUcInfMain(tzms->m_database);
//				if(pfcRec->Init() == DB_ERR_OK)	{
//					if(!(pfcRec->IsEOF()))	{
//						pfcRec->MoveFirst();
//						pfcRec->Edit();
//						pfcRec->m_GeneralVar[1] &= ~BIT_D3;
//						pfcRec->Update();
//					}
//				}
//				delete tzms;
//			}
//// midori UC_0009 add <--
//		}
//		// 処理１ <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//	}
//
//	return(FUNCTION_OK);
//}
//// midori 152110 cor <--
// midori 180503 del <--

// midori 170503 add -->
//***************************************************************************
// NumRowのチェック、修正
//***************************************************************************
int	CUCHIWAKEApp::NumRowCheck(void)
{
	int					ii=0;
	int					nBaseRow=0;			// 改ページの基準行数
	int					nBaseRow2=0;		// 改ページの基準行数
	BOOL				bflg[2];
	BOOL				bflg2[22];			// 様式①から様式⑯	TRUE:NumRowエラー
	BOOL				bflg3[20];			// 様式⑰			TRUE:NumRowエラー
	BOOL				bflg4[20];			// 様式⑱			TRUE:NumRowエラー
	CString				strQuery=_T("");
	CString				cst[2];
	CString				cst2=_T("");
	CString				tbname[22];			// 様式テーブル名
	CString				strData=_T("");
	CString				errmessage=_T("");	// エラーメッセージ
	CfrmUc000Common*	pView;
	CRecordset			rs(m_pZmSub->m_database);

	// 初期化
	cst[0].Empty();
	cst[1].Empty();
	bflg[0] = FALSE;
	bflg[1] = FALSE;
	for(ii=0;ii<22;ii++)	{
		bflg2[ii] = FALSE;
		tbname[ii].Empty();
	}
	for(ii=0;ii<20;ii++)	{
		bflg3[ii] = FALSE;
		bflg4[ii] = FALSE;
	}
	// 様式テーブル名の取得
	tbname[0] = STR_TBL_NAME_011;
	tbname[1] =	STR_TBL_NAME_021;
	tbname[2] = STR_TBL_NAME_031;
	tbname[3] = STR_TBL_NAME_041;
	tbname[4] = STR_TBL_NAME_042;
	tbname[5] = STR_TBL_NAME_051;
	tbname[6] = STR_TBL_NAME_061;
	tbname[7] = STR_TBL_NAME_071;
	tbname[8] = STR_TBL_NAME_081;
	tbname[9] = STR_TBL_NAME_091;
	tbname[10] = STR_TBL_NAME_101;
	tbname[11] = STR_TBL_NAME_102;
	tbname[12] = STR_TBL_NAME_111;
	tbname[13] = STR_TBL_NAME_121;
	tbname[14] = STR_TBL_NAME_131;
	tbname[15] = STR_TBL_NAME_141;
	tbname[16] = STR_TBL_NAME_142;
	tbname[17] = STR_TBL_NAME_151;
	tbname[18] = STR_TBL_NAME_152;
	tbname[19] = STR_TBL_NAME_153;
	tbname[20] = STR_TBL_NAME_161;
	tbname[21] = STR_TBL_NAME_162;

	// ①.預貯金～⑯-2.雑損失テーブルの頁、行チェック
	for (ii = ID_FORMNO_011; ii <= ID_FORMNO_162; ii++) {
		// FormViewオブジェクト作成
		pView = UchiwakeCreateFormViewObject(ii);
		// 初期設定
		pView->CmnSetInitialUchiwakeInfo(ii);				// 共通データ
		pView->virSetInitialValue();						// 帳表固有データ

		// --------------------------------------------------------------------------------------
		// 修復を行う条件①
		// → 該当様式の最大行数を超える行番号が存在する
		// --------------------------------------------------------------------------------------
		strQuery.Format(_T("select count(seq) from %s "),tbname[ii-1]);
		cst2.Empty();
		// 計有り
		if(pView->m_uInfo.intRowKei == 1)	{
			nBaseRow = pView->m_uInfo.intRowMax;	// 最終頁以外の最大行数（データ）
			nBaseRow2 = pView->m_uInfo.intRowMax;	// 最終頁の最大行数（データ）
			if(pView->m_uInfo.intOutKei == ID_OUTKEI_OFF)	{
				// 最終行に計（合計）あり
				// 最終行-１(計（合計））
				nBaseRow = nBaseRow - 1;
				nBaseRow2 = nBaseRow2 - 1;
			}
			else if(pView->m_uInfo.intOutKei == ID_OUTKEI_PAGEKEI)	{
				// 最終行に頁計あり
				// 最終行-１(計（合計））
				nBaseRow = nBaseRow - 1;
				nBaseRow2 = nBaseRow2 - 1;
			}
			else if(pView->m_uInfo.intOutKei == ID_OUTKEI_RUIKEI)	{
				// 最終行に計（合計）以外あり
				// 最終行-１(計（合計））
				nBaseRow = nBaseRow - 1;
				nBaseRow2 = nBaseRow2 - 1;
			}
			else if(pView->m_uInfo.intOutKei == ID_OUTKEI_BOTH)	{
				// 最終頁以外の最終行に頁計あり
				// 最終頁の最終行-1に頁計あり
				// 最終頁の最終行に計（合計）あり
				// 最終行-１（頁計）
				// 最終行-２（計（合計））
				nBaseRow = nBaseRow - 1;
				if(pView->m_uInfo.intMaxPage > 1)	nBaseRow2 = nBaseRow2 - 2;
				else								nBaseRow2 = nBaseRow2 - 1;
			}
		}
		// 計無し
		else			{
			nBaseRow = pView->m_uInfo.intRowMax;	// 最終頁以外の最大行数（データ）
			nBaseRow2 = pView->m_uInfo.intRowMax;	// 最終頁の最大行数（データ）
			if(pView->m_uInfo.intOutKei == ID_OUTKEI_PAGEKEI)	{
				// 最終行に頁計有り
				// 最終行-１（頁計）
				nBaseRow = nBaseRow - 1;	
				nBaseRow2 = nBaseRow2 - 1;
			}
			else if(pView->m_uInfo.intOutKei == ID_OUTKEI_RUIKEI)	{
				// 最終頁以外の最終行はデータ行
				// 最終頁の最終行に計（合計）あり
				nBaseRow2 = nBaseRow2 - 1;
			}
			else if(pView->m_uInfo.intOutKei == ID_OUTKEI_BOTH)	{
				// 最終頁以外の最終行に頁計あり
				// 最終頁の最終行-1に頁計あり
				// 最終頁の最終行に計（合計）あり
				// 最終行-１（頁計）
				// 最終行-２（計（合計））
				nBaseRow = nBaseRow - 1;
				if(pView->m_uInfo.intMaxPage > 1)	nBaseRow2 = nBaseRow2 - 2;
				else								nBaseRow2 = nBaseRow2 - 1;
			}
		}
		cst2.Format(_T("where ((numpage < %d and numrow > %d) or (numpage = %d and numrow > %d)) and fgshow = %d and (fgfunc <> %d and fgfunc <> %d)"),
			pView->m_uInfo.intMaxPage,nBaseRow,pView->m_uInfo.intMaxPage,nBaseRow2,ID_FGSHOW_OFF,ID_FGFUNC_RUIKEI,ID_FGFUNC_PAGEKEI);
		strQuery = strQuery + cst2;
		try	{
			rs.Open(CRecordset::forwardOnly, strQuery);
			rs.GetFieldValue((short)0,strData);
			rs.Close();
		}
		catch(CException* e)	{
			e->Delete();
			return(-1);
		}
		if(_tstoi(strData) > 0)	{
			errmessage.Format(errmessage + _T("%s %s\r\n"),pView->m_uInfo.strTitleNumber,pView->m_uInfo.strTitleName);
			bflg2[ii-1]=TRUE;
		}
		else	{
			// --------------------------------------------------------------------------------------
			// 修復を行う条件②
			// → データが１件以上存在するが、最大行数と同じ行番号が存在しない
			// --------------------------------------------------------------------------------------
// midori 152149 cor -->
//			strQuery.Format(_T("select count(seq) from %s"),tbname[ii-1]);
// ---------------------
			strQuery.Format(_T("select count(seq) from %s where fgshow = %d"),tbname[ii-1],ID_FGSHOW_OFF);
// midori 152149 cor <--
			try	{
				rs.Open(CRecordset::forwardOnly, strQuery);
				rs.GetFieldValue((short)0,strData);
				rs.Close();
			}
			catch(CException* e)	{
				e->Delete();
				return(-1);
			}
			if(_tstoi(strData) > 0)	{
// midori 152149 cor -->
//				strQuery.Format(_T("select count(seq) from %s where numrow = %d"),tbname[ii-1],nBaseRow);
// ---------------------
				strQuery.Format(_T("select count(seq) from %s where numrow = %d and fgshow = %d"),tbname[ii-1],nBaseRow,ID_FGSHOW_OFF);
// midori 152149 cor <--
				try	{
					rs.Open(CRecordset::forwardOnly, strQuery);
					rs.GetFieldValue((short)0,strData);
					rs.Close();
				}
				catch(CException* e)	{
					e->Delete();
					return(-1);
				}
				if(_tstoi(strData) == 0)	{
					errmessage.Format(errmessage + _T("%s %s\r\n"),pView->m_uInfo.strTitleNumber,pView->m_uInfo.strTitleName);
					bflg2[ii-1]=TRUE;
				}
			}
			// --------------------------------------------------------------------------------------
			// 修復を行う条件③
			// → 様式⑭-2 人件費の内訳書でデータベースにデータがあって計が無い場合
			// --------------------------------------------------------------------------------------
			if(ii == ID_FORMNO_142)	{
				strQuery.Format(_T("select count(seq) from %s where fgshow = %d"),tbname[ii-1],ID_FGSHOW_OFF);
				try	{
					rs.Open(CRecordset::forwardOnly, strQuery);
					rs.GetFieldValue((short)0,strData);
					rs.Close();
				}
				catch(CException* e)	{
					e->Delete();
					return(-1);
				}
				// データ有り
				if(_tstoi(strData) > 0)	{
					strQuery.Format(_T("select count(seq) from %s where fgfunc = %d or fgfunc = %d "),tbname[ii-1],ID_FGFUNC_RUIKEI,ID_FGFUNC_PAGEKEI);
					try	{
						rs.Open(CRecordset::forwardOnly, strQuery);
						rs.GetFieldValue((short)0,strData);
						rs.Close();
					}
					catch(CException* e)	{
						e->Delete();
						return(-1);
					}
					// 計が含まれない場合
					if(_tstoi(strData) < 1)	{
						errmessage.Format(errmessage + _T("%s %s\r\n"),pView->m_uInfo.strTitleNumber,pView->m_uInfo.strTitleName);
						bflg2[ii-1]=TRUE;
					}
				}
			}
		}
		delete pView;
		pView = NULL;
	}
	// ⑰-x.その他１,⑱-x.その他２テーブルの頁、行チェック
	for(ii=0;ii<20;ii++)	{
		// ⑰-x.その他１
		// FormViewオブジェクト作成
		pView = UchiwakeCreateFormViewObject(ID_FORMNO_171+ii);
		// 初期設定
		pView->CmnSetInitialUchiwakeInfo(ID_FORMNO_171+ii);				// 共通データ
		pView->virSetInitialValue();									// 帳表固有データ
		if(ii==0)	{
			cst[0].Format(_T("%s"),pView->m_uInfo.strTitleNumber);
			cst[0].Delete(cst[0].GetLength()-4,4);
			cst[0] = cst[0] + _T(" ") + pView->m_uInfo.strTitleName;
		}
		// --------------------------------------------------------------------------------------
		// 修復を行う条件①
		// → 該当様式の最大行数を超える行番号が存在する
		// --------------------------------------------------------------------------------------
		strQuery = _T("select count(formseq) from uc_171_sonota1 ");
		cst2.Empty();
		// 計有り
		if(pView->m_uInfo.intRowKei == 1)	{
			nBaseRow = pView->m_uInfo.intRowMax;	// 最終頁以外の最大行数（データ）
			nBaseRow2 = pView->m_uInfo.intRowMax;	// 最終頁の最大行数（データ）
			if(pView->m_uInfo.intOutKei == ID_OUTKEI_OFF)	{
				// 最終行に計（合計）あり
				// 最終行-１(計（合計））
				nBaseRow = nBaseRow - 1;
				nBaseRow2 = nBaseRow2 - 1;
			}
			else if(pView->m_uInfo.intOutKei == ID_OUTKEI_PAGEKEI)	{
				// 最終行に頁計あり
				// 最終行-１(計（合計））
				nBaseRow = nBaseRow - 1;
				nBaseRow2 = nBaseRow2 - 1;
			}
			else if(pView->m_uInfo.intOutKei == ID_OUTKEI_RUIKEI)	{
				// 最終行に計（合計）以外あり
				// 最終行-１(計（合計））
				nBaseRow = nBaseRow - 1;
				nBaseRow2 = nBaseRow2 - 1;
			}
			else if(pView->m_uInfo.intOutKei == ID_OUTKEI_BOTH)	{
				// 最終頁以外の最終行に頁計あり
				// 最終頁の最終行-1に頁計あり
				// 最終頁の最終行に計（合計）あり
				// 最終行-１（頁計）
				// 最終行-２（計（合計））
				nBaseRow = nBaseRow - 1;
				if(pView->m_uInfo.intMaxPage > 1)	nBaseRow2 = nBaseRow2 - 2;
				else								nBaseRow2 = nBaseRow2 - 1;
			}
		}
		// 計無し
		else			{
			nBaseRow = pView->m_uInfo.intRowMax;	// 最終頁以外の最大行数（データ）
			nBaseRow2 = pView->m_uInfo.intRowMax;	// 最終頁の最大行数（データ）
			if(pView->m_uInfo.intOutKei == ID_OUTKEI_PAGEKEI)	{
				// 最終行に頁計有り
				// 最終行-１（頁計）
				nBaseRow = nBaseRow - 1;	
				nBaseRow2 = nBaseRow2 - 1;
			}
			else if(pView->m_uInfo.intOutKei == ID_OUTKEI_RUIKEI)	{
				// 最終頁以外の最終行はデータ行
				// 最終頁の最終行に計（合計）あり
				nBaseRow2 = nBaseRow2 - 1;
			}
			else if(pView->m_uInfo.intOutKei == ID_OUTKEI_BOTH)	{
				// 最終頁以外の最終行に頁計あり
				// 最終頁の最終行-1に頁計あり
				// 最終頁の最終行に計（合計）あり
				// 最終行-１（頁計）
				// 最終行-２（計（合計））
				nBaseRow = nBaseRow - 1;
				if(pView->m_uInfo.intMaxPage > 1)	nBaseRow2 = nBaseRow2 - 2;
				else								nBaseRow2 = nBaseRow2 - 1;
			}
		}
		cst2.Format(_T("where formseq=%d and ((numpage < %d and numrow > %d) or (numpage = %d and numrow > %d)) and fgshow = %d and (fgfunc <> %d and fgfunc <> %d)"),
			ID_FORMNO_171+ii,pView->m_uInfo.intMaxPage,nBaseRow,pView->m_uInfo.intMaxPage,nBaseRow2,ID_FGSHOW_OFF,ID_FGFUNC_RUIKEI,ID_FGFUNC_PAGEKEI);
		strQuery = strQuery + cst2;
		try{
			rs.Open(CRecordset::forwardOnly, strQuery);
			rs.GetFieldValue((short)0,strData);
			rs.Close();
		}
		catch(CException* e)	{
			e->Delete();
			return(-1);
		}
		if(_tstoi(strData) > 0)	{
			if(bflg[0] == FALSE)	{
				errmessage.Format(errmessage + _T("%s\r\n"),cst[0]);
				bflg[0] = TRUE;
			}
			bflg3[ii]=TRUE;
		}
// midori 152149 add -->
		else	{
			// --------------------------------------------------------------------------------------
			// 修復を行う条件②
			// → データが１件以上存在するが、最大行数と同じ行番号が存在しない
			// --------------------------------------------------------------------------------------
			strQuery.Format(_T("select count(formseq) from uc_171_sonota1 where formseq=%d and fgshow = %d"),ID_FORMNO_171+ii,ID_FGSHOW_OFF);
			try	{
				rs.Open(CRecordset::forwardOnly, strQuery);
				rs.GetFieldValue((short)0,strData);
				rs.Close();
			}
			catch(CException* e)	{
				e->Delete();
				return(-1);
			}
			if(_tstoi(strData) > 0)	{
				strQuery.Format(_T("select count(formseq) from uc_171_sonota1 where formseq=%d and numrow = %d and fgshow = %d"),ID_FORMNO_171+ii,nBaseRow,ID_FGSHOW_OFF);
				try	{
					rs.Open(CRecordset::forwardOnly, strQuery);
					rs.GetFieldValue((short)0,strData);
					rs.Close();
				}
				catch(CException* e)	{
					e->Delete();
					return(-1);
				}
				if(_tstoi(strData) == 0)	{
					if(bflg[0] == FALSE)	{
						errmessage.Format(errmessage + _T("%s\r\n"),cst[0]);
						bflg[0] = TRUE;
					}
					bflg3[ii]=TRUE;
				}
			}
		}
// midori 152149 add <--
		delete pView;
		pView = NULL;

		// ⑱-x.その他２
		// FormViewオブジェクト作成
		pView = UchiwakeCreateFormViewObject(ID_FORMNO_181+ii);
		// 初期設定
		pView->CmnSetInitialUchiwakeInfo(ID_FORMNO_181+ii);			// 共通データ
		pView->virSetInitialValue();								// 帳表固有データ
		if(ii==0)	{
			cst[1].Format(_T("%s"),pView->m_uInfo.strTitleNumber);
// midori 152149 cor -->
//			cst[1].Delete(cst[0].GetLength()-4,4);
//			cst[1] = cst[0] + _T(" ") + pView->m_uInfo.strTitleName;
// ---------------------
			cst[1].Delete(cst[1].GetLength()-4,4);
			cst[1] = cst[1] + _T(" ") + pView->m_uInfo.strTitleName;
// midori 152149 cor <--
		}

		strQuery = _T("select count(formseq) from uc_181_sonota2 ");
		cst2.Empty();
		// 計有り
		if(pView->m_uInfo.intRowKei == 1)	{
			nBaseRow = pView->m_uInfo.intRowMax;	// 最終頁以外の最大行数（データ）
			nBaseRow2 = pView->m_uInfo.intRowMax;	// 最終頁の最大行数（データ）
			if(pView->m_uInfo.intOutKei == ID_OUTKEI_OFF)	{
				// 最終行に計（合計）あり
				// 最終行-１(計（合計））
				nBaseRow = nBaseRow - 1;
				nBaseRow2 = nBaseRow2 - 1;
			}
			else if(pView->m_uInfo.intOutKei == ID_OUTKEI_PAGEKEI)	{
				// 最終行に頁計あり
				// 最終行-１(計（合計））
				nBaseRow = nBaseRow - 1;
				nBaseRow2 = nBaseRow2 - 1;
			}
			else if(pView->m_uInfo.intOutKei == ID_OUTKEI_RUIKEI)	{
				// 最終行に計（合計）以外あり
				// 最終行-１(計（合計））
				nBaseRow = nBaseRow - 1;
				nBaseRow2 = nBaseRow2 - 1;
			}
			else if(pView->m_uInfo.intOutKei == ID_OUTKEI_BOTH)	{
				// 最終頁以外の最終行に頁計あり
				// 最終頁の最終行-1に頁計あり
				// 最終頁の最終行に計（合計）あり
				// 最終行-１（頁計）
				// 最終行-２（計（合計））
				nBaseRow = nBaseRow - 1;
				if(pView->m_uInfo.intMaxPage > 1)	nBaseRow2 = nBaseRow2 - 2;
				else								nBaseRow2 = nBaseRow2 - 1;
			}
		}
		// 計無し
		else			{
			nBaseRow = pView->m_uInfo.intRowMax;	// 最終頁以外の最大行数（データ）
			nBaseRow2 = pView->m_uInfo.intRowMax;	// 最終頁の最大行数（データ）
			if(pView->m_uInfo.intOutKei == ID_OUTKEI_PAGEKEI)	{
				// 最終行に頁計有り
				// 最終行-１（頁計）
				nBaseRow = nBaseRow - 1;	
				nBaseRow2 = nBaseRow2 - 1;
			}
			else if(pView->m_uInfo.intOutKei == ID_OUTKEI_RUIKEI)	{
				// 最終頁以外の最終行はデータ行
				// 最終頁の最終行に計（合計）あり
				nBaseRow2 = nBaseRow2 - 1;
			}
			else if(pView->m_uInfo.intOutKei == ID_OUTKEI_BOTH)	{
				// 最終頁以外の最終行に頁計あり
				// 最終頁の最終行-1に頁計あり
				// 最終頁の最終行に計（合計）あり
				// 最終行-１（頁計）
				// 最終行-２（計（合計））
				nBaseRow = nBaseRow - 1;
				if(pView->m_uInfo.intMaxPage > 1)	nBaseRow2 = nBaseRow2 - 2;
				else								nBaseRow2 = nBaseRow2 - 1;
			}
		}
		cst2.Format(_T("where formseq=%d and ((numpage < %d and numrow > %d) or (numpage = %d and numrow > %d)) and fgshow = %d and (fgfunc <> %d and fgfunc <> %d)"),
			ID_FORMNO_181+ii,pView->m_uInfo.intMaxPage,nBaseRow,pView->m_uInfo.intMaxPage,nBaseRow2,ID_FGSHOW_OFF,ID_FGFUNC_RUIKEI,ID_FGFUNC_PAGEKEI);
		strQuery = strQuery + cst2;
		try{
			rs.Open(CRecordset::forwardOnly, strQuery);
			rs.GetFieldValue((short)0,strData);
			rs.Close();
		}
		catch(CException* e)	{
			e->Delete();
			return(-1);
		}
		if(_tstoi(strData) > 0)	{
			if(bflg[1] == FALSE)	{
				errmessage.Format(errmessage + _T("%s\r\n"),cst[1]);
				bflg[1] = TRUE;
			}
			bflg4[ii]=TRUE;
		}
// midori 152149 add -->
		else	{
			// --------------------------------------------------------------------------------------
			// 修復を行う条件②
			// → データが１件以上存在するが、最大行数と同じ行番号が存在しない
			// --------------------------------------------------------------------------------------
			strQuery.Format(_T("select count(formseq) from uc_181_sonota2 where formseq=%d and fgshow = %d"),ID_FORMNO_181+ii,ID_FGSHOW_OFF);
			try	{
				rs.Open(CRecordset::forwardOnly, strQuery);
				rs.GetFieldValue((short)0,strData);
				rs.Close();
			}
			catch(CException* e)	{
				e->Delete();
				return(-1);
			}
			if(_tstoi(strData) > 0)	{
				strQuery.Format(_T("select count(formseq) from uc_181_sonota2 where formseq=%d and numrow = %d and fgshow = %d"),ID_FORMNO_181+ii,nBaseRow,ID_FGSHOW_OFF);
				try	{
					rs.Open(CRecordset::forwardOnly, strQuery);
					rs.GetFieldValue((short)0,strData);
					rs.Close();
				}
				catch(CException* e)	{
					e->Delete();
					return(-1);
				}
				if(_tstoi(strData) == 0)	{
					if(bflg[1] == FALSE)	{
						errmessage.Format(errmessage + _T("%s\r\n"),cst[1]);
						bflg[1] = TRUE;
					}
					bflg4[ii]=TRUE;
				}
			}
		}
// midori 152149 add <--
		delete pView;
		pView = NULL;
	}
	if(errmessage.IsEmpty() == FALSE)	{
		errmessage = _T("下記様式のデータに不正な行番号が書き込まれています。\r\n修復処理を行います。\r\n\r\n") + errmessage;
		ICSMessageBox(errmessage,(MB_OK | MB_ICONEXCLAMATION));

		// 行番号を振り直す
		for (ii = ID_FORMNO_011; ii <= ID_FORMNO_162; ii++) {
			if(bflg2[ii-1] == TRUE)	{
				pView = UchiwakeCreateFormViewObject(ii);
				// 初期設定
				pView->CmnSetInitialUchiwakeInfo(ii);				// 共通データ
				pView->virSetInitialValue();						// 帳表固有データ
				pView->CmnTblUpdatePageRowAllResetMain();
				delete pView;
				pView = NULL;
			}
		}
		for(ii=0;ii<20;ii++)	{
			if(bflg3[ii] == TRUE)	{
				pView = UchiwakeCreateFormViewObject(ID_FORMNO_171+ii);
				// 初期設定
				pView->CmnSetInitialUchiwakeInfo(ID_FORMNO_171+ii);				// 共通データ
				pView->virSetInitialValue();						// 帳表固有データ
				pView->CmnTblUpdatePageRowAllResetMain();
				delete pView;
				pView = NULL;
			}
			if(bflg4[ii] == TRUE)	{
				pView = UchiwakeCreateFormViewObject(ID_FORMNO_181+ii);
				// 初期設定
				pView->CmnSetInitialUchiwakeInfo(ID_FORMNO_181+ii);		// 共通データ
				pView->virSetInitialValue();							// 帳表固有データ
				pView->CmnTblUpdatePageRowAllResetMain();
				delete pView;
				pView = NULL;
			}
		}
	}
	return(0);
}
// midori 170503 add <--

// midori 190504 add -->
//----------------------------------------------------------------------------
// 各様式、F9（銀行、取引先）データベース内、文字列最後の空白をカットする
//----------------------------------------------------------------------------
int	CUCHIWAKEApp::DeleteSpaceMaster(CDatabase* pdb)
{
	int						ii = 0;
	int						cnt = 0;
	int						grseq = 0;
	BOOL					regautoref = FALSE;
	CString					strSQL = _T("");
	CStringArray			strArrayTmp;

	CdbUcInfMain			mfcRecMain(pdb);
	CString					strName1 = _T(""),strName2 = _T("");
	CString					strKana1 = _T(""),strKana2 = _T("");
	CString					strAdAdd1 = _T(""),strAdAdd2 = _T("");

	CdbUcLstBank			mfcBank(pdb);
	CdbUcLstAddress			mfcAddress(pdb);


	if( mfcRecMain.Init() != DB_ERR_OK )	return(0);

	CCtrlMainGeneralVar		cm( mfcRecMain );
	// 更新済みの場合処理を行わない
// 157270 del -->
	//if(cm.bMstUpd & BIT_D2)					return(0);
// 157270 del <--
// 157270 add -->
	if(cm.bMstUpd)	return(0);
// 157270 add <--

	regautoref = cm.bRegAutoRef;
	mfcRecMain.Fin();

	try {
		// F9 銀行・支店の末尾のスペースをカット
		// 初期化
		if( mfcBank.Init() == DB_ERR_OK ){
			while( !(mfcBank.IsEOF()) ){					
				mfcBank.Edit();
				mfcBank.m_BkName1 = m_clsFunc.DeleteRightSpace( mfcBank.m_BkName1 );
				mfcBank.m_BkName2 = m_clsFunc.DeleteRightSpace( mfcBank.m_BkName2 );
				mfcBank.m_BkKana1 = m_clsFunc.DeleteRightSpace( mfcBank.m_BkKana1 );
				mfcBank.m_BkKana2 = m_clsFunc.DeleteRightSpace( mfcBank.m_BkKana2 );
				mfcBank.Update();
				mfcBank.MoveNext();
			}
			mfcBank.Fin();
		}
		if(mfcBank.RequeryBkName() == DB_ERR_OK) {
			// スペースカット後重複するレコードを削除
			if(!(mfcBank.IsEOF())) {
				mfcBank.MoveFirst();
				while(!(mfcBank.IsEOF())) {
					if(strName1 == mfcBank.m_BkName1 &&
						strName2 == mfcBank.m_BkName2 &&
						strKana1 == mfcBank.m_BkKana1 &&
						strKana2 == mfcBank.m_BkKana2) {
						mfcBank.Delete();
					} else {
						strName1 = mfcBank.m_BkName1;
						strName2 = mfcBank.m_BkName2;
						strKana1 = mfcBank.m_BkKana1;
						strKana2 = mfcBank.m_BkKana2;
					}
					mfcBank.MoveNext();
				}
			}
			mfcBank.Fin();
		}

		// レコード削除後、番号を更新する
		if(mfcBank.Init() == DB_ERR_OK) {
			cnt = 1;
			if(!mfcBank.IsEOF()) {
				mfcBank.MoveFirst();
				while(!mfcBank.IsEOF()) {
					mfcBank.Edit();
					mfcBank.m_OrderNum = cnt;
					cnt++;
					mfcBank.Update();
					mfcBank.MoveNext();
				}
			}
			mfcBank.Fin();
		}

		// F9 取引先のスペースをカット
		// 初期化
		if( mfcAddress.Init() == DB_ERR_OK ){
			while( !(mfcAddress.IsEOF()) ){					
				mfcAddress.Edit();
				mfcAddress.m_AdName1 = m_clsFunc.DeleteRightSpace( mfcAddress.m_AdName1 );
				mfcAddress.m_AdName2 = m_clsFunc.DeleteRightSpace( mfcAddress.m_AdName2 );
				mfcAddress.m_AdAdd1 = m_clsFunc.DeleteRightSpace( mfcAddress.m_AdAdd1 );
				mfcAddress.m_AdAdd2 = m_clsFunc.DeleteRightSpace( mfcAddress.m_AdAdd2 );
				mfcAddress.m_AdKana = m_clsFunc.DeleteRightSpace( mfcAddress.m_AdKana );
				mfcAddress.Update();

				mfcAddress.MoveNext();
			}

			mfcAddress.Fin();
		}

		for(ii=0;ii<2;ii++) {
			strName1.Empty();
			strName2.Empty();
			strAdAdd1.Empty();
			strAdAdd2.Empty();
			strKana1.Empty();

			if(ii==0)	grseq = ID_ADDRESSGR_URI;		// 得意先
			else		grseq = ID_ADDRESSGR_KAI;		// 仕入先
// midori 155301 del -->
//			if(mfcAddress.RequeryAdName(grseq) == DB_ERR_OK) {
// midori 155301 del <--
// midori 155301 add -->
			if(mfcAddress.RequeryAdName(grseq,0) == DB_ERR_OK) {
// midori 155301 add <--
				// スペースカット後重複するレコードを削除
				if(!(mfcAddress.IsEOF())) {
					mfcAddress.MoveFirst();
					while(!(mfcAddress.IsEOF())) {
						if(strName1 == mfcAddress.m_AdName1 &&
							strName2 == mfcAddress.m_AdName2 &&
							strAdAdd1 == mfcAddress.m_AdAdd1 &&
							strAdAdd2 == mfcAddress.m_AdAdd2 &&
							strKana1 == mfcAddress.m_AdKana) {
							mfcAddress.Delete();
						} else {
							strName1 = mfcAddress.m_AdName1;
							strName2 = mfcAddress.m_AdName2;
							strAdAdd1 = mfcAddress.m_AdAdd1;
							strAdAdd2 = mfcAddress.m_AdAdd2;
							strKana1 = mfcAddress.m_AdKana;
						}
						mfcAddress.MoveNext();
					}
				}
				mfcAddress.Fin();
			}
		}

		for(ii=0;ii<2;ii++) {
			// レコード削除後、番号を更新する
			if(ii==0)	grseq = ID_ADDRESSGR_URI;		// 得意先
			else		grseq = ID_ADDRESSGR_KAI;		// 仕入先
// midori 155301 del -->
//			if(mfcAddress.RequeryAdName(grseq) == DB_ERR_OK) {
// midori 155301 del <--
// midori 155301 add -->
			if(mfcAddress.RequeryAdName(grseq,1) == DB_ERR_OK) {
// midori 155301 add <--
				cnt = 1;
				if(!mfcAddress.IsEOF()) {
					mfcAddress.MoveFirst();
					while(!mfcAddress.IsEOF()) {
						mfcAddress.Edit();
						mfcAddress.m_OrderNum = cnt;
						cnt++;
						mfcAddress.Update();
						mfcAddress.MoveNext();
					}
				}
				mfcAddress.Fin();
			}
		}

		// 011.預貯金等の内訳書
		CdbUc011Yotyokin	db011(pdb);
		db011.RequerySortPageRow();
		while(!db011.IsEOF()) {
			db011.Edit();
			// 末尾のスペースをカット
			// 銀行名
			db011.m_BkName1 = m_clsFunc.DeleteRightSpace( db011.m_BkName1 );
			// 支店名
			//if(bG_Kanso == TRUE) {
			//	m_clsFunc.StrDivision(db011.m_BkName2,&strArrayTmp,2,TRUE,TRUE);
			//	db011.m_BkName2	= m_clsFunc.StrDocking(&strArrayTmp);
			//}
			//else	{
				db011.m_BkName2	= m_clsFunc.DeleteRightSpace(db011.m_BkName2);
			//}
			db011.Update();
			// 次のレコードへ
			db011.MoveNext();
		}
		db011.Fin();

		// 021.受取手形の内訳書
		CdbUc021Uketoritegata	db021(pdb);
		db021.RequerySortPageRow();
		while(!db021.IsEOF()) {
			// 末尾のスペースをカット
			db021.Edit();
			db021.m_AdName1 = m_clsFunc.DeleteRightSpace( db021.m_AdName1 );	// (振出人1)
			db021.m_AdName2 = m_clsFunc.DeleteRightSpace( db021.m_AdName2 );	// (振出人2)
			db021.m_BkName1 = m_clsFunc.DeleteRightSpace( db021.m_BkName1 );		// (支払銀行名)
			db021.m_BkName2 = m_clsFunc.DeleteRightSpace( db021.m_BkName2 );		// (支払銀行名（支店名）)
			db021.m_Bk2Name1 = m_clsFunc.DeleteRightSpace( db021.m_Bk2Name1 );		// (割引銀行名等)
			db021.m_Bk2Name2 = m_clsFunc.DeleteRightSpace( db021.m_Bk2Name2 );		// (割引銀行名等（支店名）)
			db021.Update();
			// 次のレコードへ
			db021.MoveNext();
		}
		db021.Fin();

		// 031.売掛金（未収入金）の内訳書
		CdbUc031Urikakekin	db031(pdb);
		db031.RequerySortPageRow();
		while(!db031.IsEOF()) {
			// 末尾のスペースをカット
			db031.Edit();
			db031.m_AdName1 = m_clsFunc.DeleteRightSpace( db031.m_AdName1 );	// (相手先名称１)
			db031.m_AdName2 = m_clsFunc.DeleteRightSpace( db031.m_AdName2 );	// (相手先名称２)
			db031.m_AdAdd1 = m_clsFunc.DeleteRightSpace( db031.m_AdAdd1 );		// (相手先所在地１)
			db031.m_AdAdd2 = m_clsFunc.DeleteRightSpace( db031.m_AdAdd2 );		// (相手先所在地２)
			db031.Update();
			// 次のレコードへ
			db031.MoveNext();
		}
		db031.Fin();

		// 041.仮払金（前渡金）の内訳書
		CdbUc041Karibarai	db041(pdb);
		db041.RequerySortPageRow();
		while(!db041.IsEOF()) {
			// 末尾のスペースをカット
			db041.Edit();
			db041.m_AdName1 = m_clsFunc.DeleteRightSpace( db041.m_AdName1 );	// (相手先名称1)
			db041.m_AdName2 = m_clsFunc.DeleteRightSpace( db041.m_AdName2 );	// (相手先名称2)
			db041.m_AdAdd1 = m_clsFunc.DeleteRightSpace( db041.m_AdAdd1 );		// (相手先所在地1)
			db041.m_AdAdd2 = m_clsFunc.DeleteRightSpace( db041.m_AdAdd2 );		// (相手先所在地2)
			db041.Update();
			// 次のレコードへ
			db041.MoveNext();
		}
		db041.Fin();

		// 042.貸付金及び受取利息の内訳書
		CdbUc042Kasituke	db042(pdb);
		db042.RequerySortPageRow();
		while(!db042.IsEOF()) {
			// 末尾のスペースをカット
			db042.Edit();
			db042.m_AdName1 = m_clsFunc.DeleteRightSpace( db042.m_AdName1 );	// (貸付先1)
			db042.m_AdName2 = m_clsFunc.DeleteRightSpace( db042.m_AdName2 );	// (貸付先2)
			// (所在地1（住所）)
			//if(bG_Kanso == TRUE) {
			//	strArrayTmp.RemoveAll();
			//	m_clsFunc.StrDivision(db042.m_AdAdd1,&strArrayTmp,2,TRUE,TRUE);
			//	db042.m_AdAdd1 = m_clsFunc.StrDocking(&strArrayTmp);
			//}
			//else	{
				db042.m_AdAdd1 = m_clsFunc.DeleteRightSpace(db042.m_AdAdd1);
			//}
			// (所在地2（住所）)
			//if(bG_Kanso == TRUE) {
			//	strArrayTmp.RemoveAll();
			//	m_clsFunc.StrDivision(db042.m_AdAdd2,&strArrayTmp,2,TRUE,TRUE);
			//	db042.m_AdAdd2 = m_clsFunc.StrDocking(&strArrayTmp);
			//}
			//else	{
				db042.m_AdAdd2 = m_clsFunc.DeleteRightSpace(db042.m_AdAdd2);
			//}
			db042.Update();
			// 次のレコードへ
			db042.MoveNext();
		}
		db042.Fin();

		// 081.支払手形の内訳書
		CdbUc081Siharaitegata	db081(pdb);
		db081.RequerySortPageRow();
		while(!db081.IsEOF()) {
			// 末尾のスペースをカット
			db081.Edit();
			db081.m_AdName1 = m_clsFunc.DeleteRightSpace( db081.m_AdName1 );		// (支払先1)
			db081.m_AdName2 = m_clsFunc.DeleteRightSpace( db081.m_AdName2 );		// (支払先2)
			db081.m_BkName1 = m_clsFunc.DeleteRightSpace( db081.m_BkName1 );		// (支払銀行名)
			db081.m_BkName2 = m_clsFunc.DeleteRightSpace( db081.m_BkName2 );		// (支払銀行名（支店名）)
			db081.Update();
			// 次のレコードへ
			db081.MoveNext();
		}
		db081.Fin();

		// 091.買掛金（未払金・未払費用）の内訳書
		CdbUc091Kaikakekin	db091(pdb);
		db091.RequerySortPageRow();
		while(!db091.IsEOF()) {
			// 末尾のスペースをカット
			db091.Edit();
			db091.m_AdName1 = m_clsFunc.DeleteRightSpace( db091.m_AdName1 );	// (相手先名称１)
			db091.m_AdName2 = m_clsFunc.DeleteRightSpace( db091.m_AdName2 );	// (相手先名称２)
			db091.m_AdAdd1 = m_clsFunc.DeleteRightSpace( db091.m_AdAdd1 );		// (相手先所在地１)
			db091.m_AdAdd2 = m_clsFunc.DeleteRightSpace( db091.m_AdAdd2 );		// (相手先所在地２)
			db091.Update();
			// 次のレコードへ
			db091.MoveNext();
		}
		db091.Fin();

		// 101.仮受金（前受金・預り金）の内訳書
		CdbUc101Kariukekin	db101(pdb);
		db101.RequerySortPageRow();
		while(!db101.IsEOF()) {
			// 末尾のスペースをカット
			db101.Edit();
			db101.m_AdName1 = m_clsFunc.DeleteRightSpace( db101.m_AdName1 );	// (相手先名称１)
			db101.m_AdName2 = m_clsFunc.DeleteRightSpace( db101.m_AdName2 );	// (相手先名称２)
			db101.m_AdAdd1 = m_clsFunc.DeleteRightSpace( db101.m_AdAdd1 );		// (相手先所在地１)
			db101.m_AdAdd2 = m_clsFunc.DeleteRightSpace( db101.m_AdAdd2 );		// (相手先所在地２)
			db101.Update();
			// 次のレコードへ
			db101.MoveNext();
		}
		db101.Fin();

		// 111.借入金及び支払利子の内訳書
		CdbUc111Kariirekin	db111(pdb);
		db111.RequerySortPageRow();
		while(!db111.IsEOF()) {
			// 末尾のスペースをカット
			db111.Edit();
			db111.m_AdName1 = m_clsFunc.DeleteRightSpace( db111.m_AdName1 );	// (借入先1)
			db111.m_AdName2 = m_clsFunc.DeleteRightSpace( db111.m_AdName2 );	// (借入先2)
			db111.Update();
			// 次のレコードへ
			db111.MoveNext();
		}
		db111.Fin();

		// 161.雑益、雑損失等の内訳書
		CdbUc161Zatueki	db161(pdb);
		db161.RequerySortPageRow();
		while(!db161.IsEOF()) {
			// 末尾のスペースをカット
			db161.Edit();
			db161.m_AdName1 = m_clsFunc.DeleteRightSpace( db161.m_AdName1 );	// (相手先1)
			db161.m_AdName2 = m_clsFunc.DeleteRightSpace( db161.m_AdName2 );	// (相手先2)
			db161.m_AdAdd1 = m_clsFunc.DeleteRightSpace( db161.m_AdAdd1 );		// (所在地1)
			db161.m_AdAdd2 = m_clsFunc.DeleteRightSpace( db161.m_AdAdd2 );		// (所在地2)
			db161.Update();
			// 次のレコードへ
			db161.MoveNext();
		}
		db161.Fin();

		// 162.雑損失等の内訳書
		CdbUc162Zatuson	db162(pdb);
		db162.RequerySortPageRow();
		while(!db162.IsEOF()) {
			// 末尾のスペースをカット
			db162.Edit();
			db162.m_AdName1 = m_clsFunc.DeleteRightSpace( db162.m_AdName1 );	// (相手先1)
			db162.m_AdName2 = m_clsFunc.DeleteRightSpace( db162.m_AdName2 );	// (相手先2)
			db162.m_AdAdd1 = m_clsFunc.DeleteRightSpace( db162.m_AdAdd1 );		// (所在地1)
			db162.m_AdAdd2 = m_clsFunc.DeleteRightSpace( db162.m_AdAdd2 );		// (所在地2)
			db162.Update();
			// 次のレコードへ
			db162.MoveNext();
		}
		db162.Fin();

		// 様式のスペースカット後、
		// F9を参照して、様式データの更新を行う
		//if(regautoref != FALSE) {
			if(mfcBank.Init() == DB_ERR_OK) {
				mfcBank.UpdateDataTableAll(0,1);
				mfcBank.Fin();
			}
			if(mfcAddress.Init() == DB_ERR_OK) {
// インボイス登録番号追加対応_23/11/27 del -->
				//mfcAddress.UpdateDataTableAll(0,1);
// インボイス登録番号追加対応_23/11/27 del <--
// インボイス登録番号追加対応_23/11/27 add -->
				if(bG_InvNo == TRUE)	mfcAddress.UpdateDataTableAll_Inv(0,1);
				else					mfcAddress.UpdateDataTableAll(0,1);
// インボイス登録番号追加対応_23/11/27 add <--
				mfcAddress.Fin();
			}
		//}

		// 集計方法のみメインに保存する
		if(mfcRecMain.Init() == DB_ERR_OK){
			mfcRecMain.Edit();
			CCtrlMainGeneralVar cm(mfcRecMain);
			cm.bMstUpd = 1;
			cm.Set(mfcRecMain);
			mfcRecMain.Update();
			mfcRecMain.Fin();
		}
	}
	catch (CDBException *e) {
		ICSMessageBox(e->m_strError,MB_ICONSTOP);
		e->Delete();
		return(-1);
	}
	return(0);
}
// midori 190504 add <--

// midori 180503 del -->
//// midori UC_0009 add -->
////***************************************************************************
//// uc_inf_main に GeneralVar が存在するかのチェック
////***************************************************************************
//int	CUCHIWAKEApp::CheckGeneralVar( CDBZmSub* pZmSub )
//{
//	int				ret=0;
//	short			ind=0;
//	CString			strSQL=_T("");
//	CString			cs=_T("");
//	CRecordset*		prs;
//
//	strSQL = _T("SELECT COLUMNPROPERTY(OBJECT_ID('uc_inf_main'),'GeneralVar','PRECISION')");
//	prs = new CRecordset(pZmSub->m_database);
//	prs->Open(CRecordset::forwardOnly,strSQL);
//	prs->GetFieldValue(ind,cs);
//	ret = _tstoi(cs);
//	prs->Close();
//	delete prs;
//	prs=0;
//
//	return(ret);
//}
//// midori UC_0009 add <--
// midori 180503 del <--

// midori 153112 add -->
//***************************************************************************
// 過年度マスターが選択されているかのチェック
//***************************************************************************
BOOL CUCHIWAKEApp::IsKanendoMaster( void )
{
	BOOL			brv=FALSE;
	char			szSQLServer[64];	// SQLサーバ名
	char			szApno[16];			// アプリケーション番号
	char			szCoCode[16];		// 会社コード
	char			szKikan[16];		// 期末年月日
	int				eymd=0;				// 過年度の終了年月日
	int				nen=0;
	int				kikan=0;
// midori 181101 del -->
//	UCHAR			yy,mm,dd;
// midori 181101 del <--
// midori 181101 add -->
	UCHAR			mm,dd;
// midori 181101 add <--
	UCHAR			ymd1[3]={0},ymd2[3]={0};
	BYTE			apno=0;
	CString			m_svr=_T("");
	CO_CTL_DB		coctl={0};
	CO_CTL_Array	coarray;
	ICSMCTL			m_mctl;

	// 選択した決算期間が、coarrayの[0]番目とより古ければ過年度マスター
	m_pZmSub->VolumeOpen();
	apno = m_pZmSub->zvol->apno;
	eymd = m_pZmSub->zvol->ee_ymd;
	m_pZmSub->VolumeClose();

	kikan = eymd;
	nen = (kikan / 10000);
	nen %= 100;
// midori 181101 del -->
//// midori 180503 del -->
////	CnvDbToBCD(kikan,&yy,&mm,&dd);
//// midori 180503 del <--
//// midori 180503 add -->
//	UchiCnvDbToBCD(kikan,&yy,&mm,&dd);
//// midori 180503 add <--
// midori 181101 del <--
// midori 181101 add -->
	UchiCnvDbToBCD(kikan,&mm,&dd);
// midori 181101 add <--
	int_bcd(&ymd1[0],nen,1);
	ymd1[1] = mm;
	ymd1[2] = dd;

	PCOM_GetString(_T("SelectSqlServer"),m_svr.GetBuffer(256));
	m_svr.ReleaseBuffer();
	if(m_mctl.SetDatabase(m_svr) < 0)	{
		ICSExit(0,_T("マスター管理テーブルの読み込みに失敗しました。"));
		return(FUNCTION_NG);
	}
	if(m_mctl.vctl_open(MODE_READ,apno,0) == 0)	{
		PCOM_GetString("SELECTCODSN",szSQLServer,szApno,szCoCode,szKikan);
		coctl.c_apno = apno;
		int_bcd(coctl.c_cod,atoi(szCoCode),4);
		if(m_mctl.vctl_coinf(ICSMCTL::ZAIMU_SW,&coctl,&coarray) == 0)	{
			if((int)coarray.GetCount() > 1)	{
				memcpy(ymd2,&coarray[0].c_kikan[3],3);
				if(memcmp(ymd1,ymd2,3) < 0)	{
					brv = TRUE;
				}
			}
		}
		m_mctl.vctl_close();
	}

	return(brv);
}
// midori 153112 add <--

// midori 180101 add -->
// -----------------------------------------------------------------------------
//	決算期の連動
// -----------------------------------------------------------------------------
int	CUCHIWAKEApp::KessankiCheck(void)
{
	int				st=0;
	CString			strQuery="";
	CDBPrtbl		pt;

	// zvolumeテーブル参照
	if(m_pZmSub->VolumeOpen() != 0)	{
		return(-1);
	}

	// 決算情報を取得
	pt.SetMasterDB( m_pDB );
	st = pt.PrfkinOpen("pkofs = 0");		// 当期のみ参照
	if(st != 0)	{
		return(-1);
	}
	// 財務dbの決算期と比較
	if( (m_pZmSub->zvol->ki != 0) && (pt.prfkin->pkdkin != m_pZmSub->zvol->ki) )	{
		ICSMessageBox("決算期を会社情報から連動しました。\r\n\r\n表紙設定で決算期を確認してください。", MB_OK | MB_ICONASTERISK);
		pt.prfkin->Edit();
		pt.prfkin->pkdkin = m_pZmSub->zvol->ki;
		pt.prfkin->Update();
	}
	pt.PrfkinClose();

	// zvolumeテーブルを閉じる
	m_pZmSub->VolumeClose();

	return(0);
}
// midori 180101 add <--

// midori 181002 del -->
//// midori 180104 add -->
//// コールバック関数
//BOOL	CALLBACK	_DBUtwCoselCallBack( void *pctl, int a, int b )
//{
//// midori UC_0020 del -->
////// midori UC_0016 del -->
//////	CO_CTL_DB*	coctl = (CO_CTL_DB *)pctl;
//////	BOOL		bNotSLink = TRUE;
//////
//////	if(coctl->c_subsw & 0x200)	bNotSLink = FALSE;
//////
//////	return(bNotSLink);
////// midori UC_0016 del <--
////// midori UC_0016 add -->
////	CO_CTL_DB*	coctl = (CO_CTL_DB *)pctl;
////	BOOL		bNoZaim = TRUE;
////
////	if(coctl->c_exbin[128]&0x1)	{			// 財務マスター未作成
////		if(coctl->c_apno == 0x10)	{		// 業種個人を除く
////			bNoZaim = FALSE;
////		}
////		else if(coctl->c_subsw&0x20000)	{	// 消費税申告書（非連動）のみを除く
////			bNoZaim = FALSE;
////		}
////		else	{
////			bNoZaim = TRUE;
////		}
////	}
////
////	return(bNoZaim);
////// midori UC_0016 add <--
//// midori UC_0020 del <--
//// midori UC_0020 add -->
//	CO_CTL_DB*	coctl = (CO_CTL_DB *)pctl;
//	BOOL		bNoZaim = TRUE;
//
//	if(coctl->c_exbin[128]&0x1)	{			// 財務マスター未作成
//		if(coctl->c_apno != 0x10)	{		// 業種個人を除く
//			if(coctl->c_subsw&0x20)	{	// 勘定科目内訳書（非連動）のみ
//				bNoZaim = TRUE;
//			}
//		}
//	}
//
//	return(bNoZaim);
//// midori UC_0020 add <--
//}
// midori 181002 del <--

//***************************************************************************
// 会社選択(単体マスターに対応)
// 【引数】	int		0:業務起動時
//					1:他プロセスの会社選択ツールバーで会社が変更された
//					2:自分自身の会社選択ツールバーが押された
// 【戻値】	int		1:IDOK	2:IDCANCEL	-1:エラー
//***************************************************************************
int CUCHIWAKEApp::co_ok_ex( int pSyori )
{
	int			rv=0;
// midori 181002 add -->
	int			sw=0;
// midori 181002 add <--
	COPAK		pak;
	COSEL_DATA	cdt;

// midori 181002 del -->
//	if(pSyori == 2)	{
//		pak.sgn0 = 0x10;
//	}
//// midori 180104-2 del -->
////	pak.pksw |= 0x1;
////	pak.cod[0] = pak.cod[1] = pak.cod[2] = pak.cod[3] = 0xff;
////	pak.apno = 0xffff;
////	pak.subs = 0x20;		// 勘定科目内訳書サイン
//////	pak.sgn4ex = 0x20;		// SUBマスターのみでも表示
//////	pak.sgn4ex|= 0x1000;	// 財務マスターを表示
////	pak.apfnc = (void(*)())_DBUtwCoselCallBack;
//// midori 180104-2 del <--
//// midori UC_0015 add -->
//	if(m_swUtwInfoRegister)	{
//		pak.pksw |= 0x1;
//		pak.cod[0] = pak.cod[1] = pak.cod[2] = pak.cod[3] = 0xff;
//		pak.apno = 0xffff;
//		pak.subs = 0x20;		// 勘定科目内訳書サイン
//		pak.apfnc = (void(*)())_DBUtwCoselCallBack;
//		pak.sgn4ex = 0x20;	// SUBマスターのみでも表示
//	}
//// midori UC_0015 add <--
//
//	if(pSyori == 1)	{
//		rv = m_Cs.co_ok();
//	}
//	else	{
//		rv = m_Cs.co_ok(pak);
//	}
//// midori UC_0015 del -->
//	//bTantai = FALSE;
//	//if(rv == IDOK)	{
//	//	if(pSyori != 1)	{
//	//		cdt = pak.svol.GetAt(0);
//	//		if((cdt.seldata.c_subsw & 0x20) && (cdt.seldata.c_exbin[0] & 0x01))	{
//	//			bTantai = TRUE;
//	//		}
//	//	}
//	//}
//// midori UC_0015 del <--
// midori 181002 del <--
// midori 181002 add -->
	if(m_swUtwInfoRegister == 0) {
		if(pSyori == 2)	{
			pak.sgn0 = 0x10;
		}
// midori UC_0037 add -->
#ifndef _DEBUG
// midori 150885 del -->
		//pak.pksw |= 0x1;
		//pak.sgn4ex = 0x1020;
		//pak.subs = 0x100000;
// midori 150885 del <--
// midori 150885 add -->
		CString	strGrNum = m_clsFunc.GetGrNo();
// 157759 del -->
		//if(strGrNum.CompareNoCase(_T("1B")) == 0)	{
// 157759 del <--
// 157759 add -->
		if(strGrNum.CompareNoCase(_T("1B")) == 0 && m_swIcspCoop == 0)	{
// 157759 add <--
			pak.pksw |= 0x1;
			pak.sgn4ex = 0x1020;
			pak.subs = 0x100000;
		}
// midori 150885 add <--
#endif
// midori UC_0037 add <--
		if(pSyori == 1)	{
			rv = m_Cs.co_ok();
		}
		else {
			rv = m_Cs.co_ok(pak);
		}
	}
	else	{
		while( 1 ) {
			pak.sgn0 = 0x10;
			pak.pksw |= 0x1;
			pak.sgn4ex = 0x20;	// SUBマスターのみでも表示
			memset(pak.cod,0xff,sizeof( pak.cod ));
			rv = m_Cs.co_ok(pak);
			if(rv != IDOK)	{
				if(sw != 0)	{
					// 一度当年度に内訳書が無いマスターを選択していると
					// 左上の会社名が内訳書が無いマスターに変わっているため
					// 業務メニューに戻るようにする(枝番別試算表と同じ対応)
					//AfxGetMainWnd()->PostMessage(WM_CLOSE,1,0);
					rv = -1;
				}
				break;
			}
			rv = MasterCheck();
			// エラー
			if(rv == -1) {
				return( -1 );
			}
			// マスター無し
			if(rv == 0) {
				sw = 1;
				continue;
			}
			break;
		}
	}
// midori 181002 add <--

	return(rv);
}
// midori 180104 add <--

// midori UC_0038 del -->
//// midori 153790 add -->
//// ---------------------------------------------------------------------
////（マスター修復関数）
//// 科目、銀行、取引先参照の番号が正しく連番になっているかチェック
//// 連番になっていない場合、修復処理を行う
////
////	戻値
////			0:正常終了	-1:エラー
//// ---------------------------------------------------------------------
//int CUCHIWAKEApp::OrderCheck()
//{
//	int					ii=0;
//	int					cnt=0;
//	int					order=0;			// 科目、銀行、取引先参照の順番
//	int					sw = 0;				// マスター修復サイン			1:マスター修復が必要
//	int					sw2 = 0;			// マスター修復サイン（得意先）	1:マスター修復が必要
//	CfrmUc000Common*	pView;
//	CdbUcLstBank		rsBank(m_pDB);
//	CdbUcLstAddress		rsAddress(m_pDB);
//	CdbUcLstKamoku		rsKmk(m_pDB);
//
//	// 参照関連のテーブルがオープン可かチェック
//	// 科目テーブルのチェック
//	if ( m_clsFunc.CheckTableOpen( (CdbBase*)&rsKmk ) != TRUE ){
//		return(-1);
//	}
//	// 科目参照
//
//	// ①.預貯金～⑯-2.雑損失テーブルのチェック
//	for (ii = ID_FORMNO_011; ii <= ID_FORMNO_162; ii++) {
//		// 連番になっていないかの確認
//		sw = 0;
//		rsKmk.RequeryForm(ii);
//		order=0;
//		while(!rsKmk.IsEOF())	{
//			if((order+1) != rsKmk.m_KnOrder)	{
//				// 連番になっていなければ sw = 1
//				sw = 1;
//				break;
//			}
//			order = rsKmk.m_KnOrder;
//			rsKmk.MoveNext();
//		}
//
//		// 連番になっていなければマスター修復を行う
//		if(sw == 1)	{
//			// マスター修復処理
//			// 番号を再付番する
//			cnt = 1;
//			rsKmk.MoveFirst();
//			while(!rsKmk.IsEOF())	{
//				rsKmk.Edit();
//				rsKmk.m_KnOrder = cnt;
//				cnt++;
//				rsKmk.Update();
//				rsKmk.MoveNext();
//			}
//
//			// FormViewオブジェクト作成
//			pView = UchiwakeCreateFormViewObject(ii);
//
//			// 初期設定
//			pView->CmnSetInitialUchiwakeInfo(ii);						// 共通データ
//			pView->virSetInitialValue();								// 帳表固有データ
//
//			// 各帳表テーブルの更新
//			pView->virFncReferenceUpdateTbl(ID_DLGTYPE_KAMOKU,0);
//		}
//	}
//
//	// ⑰-x.その他１,⑱-x.その他２テーブルのチェック
//	for(ii=0;ii<20;ii++)	{
//
//		// ⑰-1　～ ⑰-20
//		// 連番になっていないかの確認
//		sw = 0;
//		rsKmk.RequeryForm(ID_FORMNO_171+ii);
//		order=0;
//		while(!rsKmk.IsEOF())	{
//			if((order+1) != rsKmk.m_KnOrder)	{
//				// 連番になっていなければ sw = 1
//				sw = 1;
//				break;
//			}
//			order = rsKmk.m_KnOrder;
//			rsKmk.MoveNext();
//		}
//
//		// 連番になっていなければマスター修復を行う
//		if(sw == 1)	{
//			// マスター修復処理
//			// 番号を再付番する
//			cnt = 1;
//			rsKmk.MoveFirst();
//			while(!rsKmk.IsEOF())	{
//				rsKmk.Edit();
//				rsKmk.m_KnOrder = cnt;
//				cnt++;
//				rsKmk.Update();
//				rsKmk.MoveNext();
//			}
//
//			// FormViewオブジェクト作成
//			pView = UchiwakeCreateFormViewObject(ID_FORMNO_171+ii);
//
//			// 初期設定
//			pView->CmnSetInitialUchiwakeInfo(ID_FORMNO_171+ii);				// 共通データ
//			pView->virSetInitialValue();									// 帳表固有データ
//
//			// 各帳表テーブルの更新
//			pView->virFncReferenceUpdateTbl(ID_DLGTYPE_KAMOKU,0);
//		}
//
//		// ⑱-1　～ ⑱-20
//		// 連番になっていないかの確認
//		sw = 0;
//		rsKmk.RequeryForm(ID_FORMNO_181+ii);
//		order=0;
//		while(!rsKmk.IsEOF())	{
//			if((order+1) != rsKmk.m_KnOrder)	{
//				// 連番になっていなければ sw = 1
//				sw = 1;
//				break;
//			}
//			order = rsKmk.m_KnOrder;
//			rsKmk.MoveNext();
//		}
//
//		// 連番になっていなければマスター修復を行う
//		if(sw == 1)	{
//			// マスター修復処理
//			// 番号を再付番する
//			cnt = 1;
//			rsKmk.MoveFirst();
//			while(!rsKmk.IsEOF())	{
//				rsKmk.Edit();
//				rsKmk.m_KnOrder = cnt;
//				cnt++;
//				rsKmk.Update();
//				rsKmk.MoveNext();
//			}
//
//			// FormViewオブジェクト作成
//			pView = UchiwakeCreateFormViewObject(ID_FORMNO_181+ii);
//
//			// 初期設定
//			pView->CmnSetInitialUchiwakeInfo(ID_FORMNO_181+ii);				// 共通データ
//			pView->virSetInitialValue();									// 帳表固有データ
//
//			// 各帳表テーブルの更新
//			pView->virFncReferenceUpdateTbl(ID_DLGTYPE_KAMOKU,0);
//		}
//
//	}
//
//	rsKmk.Fin();
//
//	// 銀行テーブルのチェック
//	if ( m_clsFunc.CheckTableOpen( (CdbBase*)&rsBank ) != TRUE ){
//		return(-1);
//	}
//
//	// 連番になっていないかの確認
//	sw = 0;
//	rsBank.Init();
//	order=0;
//	while(!rsBank.IsEOF())	{
//		if((order+1) != rsBank.m_OrderNum)	{
//			// 連番になっていなければ sw = 1
//			sw = 1;
//			break;
//		}
//		order = rsBank.m_OrderNum;
//		rsBank.MoveNext();
//	}
//
//	// 連番になっていなければマスター修復を行う
//	if(sw == 1)	{
//		// マスター修復処理
//		// 番号を再付番する
//		cnt = 1;
//		rsBank.MoveFirst();
//		while(!rsBank.IsEOF())	{
//			rsBank.Edit();
//			rsBank.m_OrderNum = cnt;
//			cnt++;
//			rsBank.Update();
//			rsBank.MoveNext();
//		}
//
//		rsBank.UpdateDataTableAll(0);
//	}
//
//	// 取引先テーブルのチェック
//	if ( m_clsFunc.CheckTableOpen( (CdbBase*)&rsAddress ) != TRUE ){
//		return(-1);
//	}
//
//	// 連番になっていないかの確認
//	// 仕入先
//	sw = 0;
//	rsAddress.Init(ID_ADDRESSGR_KAI);
//	order=0;
//	while(!rsAddress.IsEOF())	{
//		if((order+1) != rsAddress.m_OrderNum)	{
//			// 連番になっていなければ sw = 1
//			sw = 1;
//			break;
//		}
//		order = rsAddress.m_OrderNum;
//		rsAddress.MoveNext();
//	}
//
//	// 連番になっていなければマスター修復を行う
//	if(sw == 1)	{
//		// マスター修復処理
//		// 番号を再付番する
//		cnt = 1;
//		rsAddress.MoveFirst();
//		while(!rsAddress.IsEOF())	{
//			rsAddress.Edit();
//			rsAddress.m_OrderNum = cnt;
//			cnt++;
//			rsAddress.Update();
//			rsAddress.MoveNext();
//		}
//	}
//
//	// 得意先
//	sw2 = 0;
//	rsAddress.Init(ID_ADDRESSGR_URI);
//	order=0;
//	while(!rsAddress.IsEOF())	{
//		if((order+1) != rsAddress.m_OrderNum)	{
//			// 連番になっていなければ sw2 = 1
//			sw2 = 1;
//			break;
//		}
//		order = rsAddress.m_OrderNum;
//		rsAddress.MoveNext();
//	}
//
//	// 連番になっていなければマスター修復を行う
//	if(sw2 == 1)	{
//		// マスター修復処理
//		// 番号を再付番する
//		cnt = 1;
//		rsAddress.MoveFirst();
//		while(!rsAddress.IsEOF())	{
//			rsAddress.Edit();
//			rsAddress.m_OrderNum = cnt;
//			cnt++;
//			rsAddress.Update();
//			rsAddress.MoveNext();
//		}
//
//	}
//
//	// 仕入先または得意先に修復処理有り
//	if(sw == 1 || sw2 == 1)	{
//		rsAddress.UpdateDataTableAll(0);
//	}
//
//	return(0);
//}
//// midori 153790 add <--
// midori UC_0038 del <--
// midori UC_0038 add -->
// ---------------------------------------------------------------------
//（マスター修復関数）
// 科目、銀行、取引先参照の番号が正しく連番になっているかチェック
// 連番になっていない場合、修復処理を行う
//
//	引数	int			FormSeq番号
//	戻値	0:正常終了	-1:エラー
// ---------------------------------------------------------------------
int CUCHIWAKEApp::OrderCheckEx( int intFormSeq )
{
	int					ii=0;
	int					cnt=0;
	int					order=0;			// 科目、銀行、取引先参照の順番
	int					sw=0;				// マスター修復サイン			1:マスター修復が必要
	int					sw2=0;				// マスター修復サイン（得意先）	1:マスター修復が必要
	int					key=0;
	CfrmUc000Common*	pView;
	CdbUcLstBank		rsBank(m_pDB);
	CdbUcLstAddress		rsAddress(m_pDB);
	CdbUcLstKamoku		rsKmk(m_pDB);
	typedef struct	_UC_LST_KAMOKU	{
		int			KnSeq;
		int			FormSeq;
		int			KnOrder;
		CString		KnCode;
		CString		KnName;
		int			FgDel;
		int			FgDft;
		CString		KnKana;
		CString		IkkatuVal;
		int			PrSign;
	}UC_LST_KAMOKU;
	UC_LST_KAMOKU								ulk_rec;
	CMap<int,int,UC_LST_KAMOKU,UC_LST_KAMOKU>	ulk_map;
	POSITION			pos;
	CString				cs=_T("");

	// ------------------------------------------------------------------------------------------
	// 科目テーブルのチェック①(KnSeqに重複がないか)
	// ------------------------------------------------------------------------------------------
	sw = 0;
	rsKmk.RequeryForm(intFormSeq);
	while(!rsKmk.IsEOF())	{
		ulk_rec.KnSeq		= rsKmk.m_KnSeq;
		ulk_rec.FormSeq		= rsKmk.m_FormSeq;
		ulk_rec.KnOrder		= rsKmk.m_KnOrder;
		ulk_rec.KnCode		= rsKmk.m_KnCode;
		ulk_rec.KnName		= rsKmk.m_KnName;
		ulk_rec.FgDel		= rsKmk.m_FgDel;
		ulk_rec.FgDft		= rsKmk.m_FgDft;
		ulk_rec.KnKana		= rsKmk.m_KnKana;
		ulk_rec.IkkatuVal	= rsKmk.m_IkkatuVal;
		ulk_rec.PrSign		= rsKmk.m_PrSign;
		if(ulk_map.Lookup(ulk_rec.KnSeq,ulk_rec) == TRUE)	{
			sw = 1;
		}
		ulk_map.SetAt(ulk_rec.KnSeq,ulk_rec);
		rsKmk.MoveNext();
	}
	rsKmk.Fin();
	if(sw != 0)	{
		// キーが重複していれば、一旦すべて削除してから重複がない状態を追加する
		// ①‐１ まとめて削除(複数の行が更新されましたのエラーがでるので)
		cs.Format(_T("delete uc_lst_kamoku where FormSeq=%d"),intFormSeq);
		try	{
			m_pDB->ExecuteSQL(cs);
		}
		catch(CDBException *e)	{
			ICSMessageBox(e->m_strError,MB_ICONSTOP);
			e->Delete();
			return(-1);
		}
		// ①‐２ １件ずつ追加
		rsKmk.RequeryForm(intFormSeq);
		pos = ulk_map.GetStartPosition();
		while(pos != NULL)	{
			ulk_map.GetNextAssoc(pos,key,ulk_rec);
			rsKmk.AddNew();
			rsKmk.m_KnSeq		= ulk_rec.KnSeq;
			rsKmk.m_FormSeq		= ulk_rec.FormSeq;
			rsKmk.m_KnOrder		= ulk_rec.KnOrder;
			rsKmk.m_KnCode		= ulk_rec.KnCode;
			rsKmk.m_KnName		= ulk_rec.KnName;
			rsKmk.m_FgDel		= ulk_rec.FgDel;
			rsKmk.m_FgDft		= ulk_rec.FgDft;
			rsKmk.m_KnKana		= ulk_rec.KnKana;
			rsKmk.m_IkkatuVal	= ulk_rec.IkkatuVal;
			rsKmk.m_PrSign		= ulk_rec.PrSign;
			rsKmk.Update();
		}
		rsKmk.Fin();
	}

	// ------------------------------------------------------------------------------------------
	// 科目テーブルのチェック②(KnOrderに重複、抜けがないか)
	// ------------------------------------------------------------------------------------------
	// テーブルがオープン可かチェック
	if(m_clsFunc.CheckTableOpen((CdbBase*)&rsKmk) != TRUE)	{
		return(-1);
	}
	// 連番になっていないかの確認
	sw = 0;
	rsKmk.RequeryForm(intFormSeq);
	order=0;
	while(!rsKmk.IsEOF())	{
		if((order + 1) != rsKmk.m_KnOrder)	{
			// 連番になっていなければ sw = 1
			sw = 1;
			break;
		}
		order = rsKmk.m_KnOrder;
		rsKmk.MoveNext();
	}
	// 連番になっていなければマスター修復を行う
	if(sw == 1)	{
		// マスター修復処理(番号を再付番する)
		cnt = 1;
		rsKmk.MoveFirst();
		while(!rsKmk.IsEOF())	{
			rsKmk.Edit();
			rsKmk.m_KnOrder = cnt;
			cnt++;
			rsKmk.Update();
			rsKmk.MoveNext();
		}
		// FormViewオブジェクト作成
		pView = UchiwakeCreateFormViewObject(intFormSeq);
		// 初期設定
		pView->CmnSetInitialUchiwakeInfo(intFormSeq);				// 共通データ
		pView->virSetInitialValue();								// 帳表固有データ
		// 各帳表テーブルの更新
		pView->virFncReferenceUpdateTbl(ID_DLGTYPE_KAMOKU,0);
	}
	rsKmk.Fin();

	// ------------------------------------------------------------------------------------------------
	// 銀行テーブルのチェック（関連する内訳書のみ）
	// ------------------------------------------------------------------------------------------------
	if( intFormSeq == ID_FORMNO_011 || intFormSeq == ID_FORMNO_021 || intFormSeq == ID_FORMNO_081 || 
		intFormSeq == ID_FORMNO_111 )	{
		// テーブルがオープン可かチェック
		if(m_clsFunc.CheckTableOpen((CdbBase*)&rsBank) != TRUE)	{
			return(-1);
		}
		// 連番になっていないかの確認
		sw = 0;
		rsBank.Init();
		order=0;
		while(!rsBank.IsEOF())	{
			if((order + 1) != rsBank.m_OrderNum)	{
				// 連番になっていなければ sw = 1
				sw = 1;
				break;
			}
			order = rsBank.m_OrderNum;
			rsBank.MoveNext();
		}

		// 連番になっていなければマスター修復を行う
		if(sw == 1)	{
			// マスター修復処理(番号を再付番する)
			cnt = 1;
			rsBank.MoveFirst();
			while(!rsBank.IsEOF())	{
				rsBank.Edit();
				rsBank.m_OrderNum = cnt;
				cnt++;
				rsBank.Update();
				rsBank.MoveNext();
			}
			rsBank.UpdateDataTableAll(0);
		}
	}

	// ------------------------------------------------------------------------------------------------
	// 取引先（関連する内訳書のみ）
	// ------------------------------------------------------------------------------------------------
	if( intFormSeq == ID_FORMNO_021 || intFormSeq == ID_FORMNO_031 || intFormSeq == ID_FORMNO_041 || 
		intFormSeq == ID_FORMNO_081 || intFormSeq == ID_FORMNO_091 || intFormSeq == ID_FORMNO_101 || 
		intFormSeq == ID_FORMNO_042 || intFormSeq == ID_FORMNO_161 || intFormSeq == ID_FORMNO_162 )	{
		// テーブルがオープン可かチェック
		if(m_clsFunc.CheckTableOpen((CdbBase*)&rsAddress) != TRUE)	{
			return(-1);
		}
		// 
		// 仕入先
		// 
		// 連番になっていないかの確認
		sw = 0;
		rsAddress.Init(ID_ADDRESSGR_KAI);
		order=0;
		while(!rsAddress.IsEOF())	{
			if((order + 1) != rsAddress.m_OrderNum)	{
				// 連番になっていなければ sw = 1
				sw = 1;
				break;
			}
			order = rsAddress.m_OrderNum;
			rsAddress.MoveNext();
		}
		// 連番になっていなければマスター修復を行う
		if(sw == 1)	{
			// マスター修復処理(番号を再付番する)
			cnt = 1;
			rsAddress.MoveFirst();
			while(!rsAddress.IsEOF())	{
				rsAddress.Edit();
				rsAddress.m_OrderNum = cnt;
				cnt++;
				rsAddress.Update();
				rsAddress.MoveNext();
			}
		}
		// 
		// 得意先
		// 
		// 連番になっていないかの確認
		sw2 = 0;
		rsAddress.Init(ID_ADDRESSGR_URI);
		order=0;
		while(!rsAddress.IsEOF())	{
			if((order + 1) != rsAddress.m_OrderNum)	{
				// 連番になっていなければ sw2 = 1
				sw2 = 1;
				break;
			}
			order = rsAddress.m_OrderNum;
			rsAddress.MoveNext();
		}
		// 連番になっていなければマスター修復を行う
		if(sw2 == 1)	{
			// マスター修復処理(番号を再付番する)
			cnt = 1;
			rsAddress.MoveFirst();
			while(!rsAddress.IsEOF())	{
				rsAddress.Edit();
				rsAddress.m_OrderNum = cnt;
				cnt++;
				rsAddress.Update();
				rsAddress.MoveNext();
			}
		}
		// 仕入先または得意先に修復処理があれば
		if(sw == 1 || sw2 == 1)	{
			// データを書き換える
// インボイス登録番号追加対応_23/11/27 del -->
			//rsAddress.UpdateDataTableAll(0);
// インボイス登録番号追加対応_23/11/27 del <--
// インボイス登録番号追加対応_23/11/27 add -->
			if(bG_InvNo == TRUE)	rsAddress.UpdateDataTableAll_Inv(0);
			else					rsAddress.UpdateDataTableAll(0);
// インボイス登録番号追加対応_23/11/27 add <--
		}
	}

	return(0);
}
// midori UC_0038 add <--

// midori 190301 add -->
//***************************************************************************
// 決算期間から簡素化の画面で起動するかどうかを判定する
//***************************************************************************
void CUCHIWAKEApp::KansoCheck(void)
{
	int				symd=0,eymd=0;
// midori 154696 add -->
	int				init=0;
// midori 154696 add <--
	CdbUcInfMain	mfcMain(m_pZmSub->m_database);
	BOOL			bcmp=FALSE;
	BOOL			zupd=FALSE;
// midori 154696 190425_2 add -->
	BOOL			kakutei=FALSE;
// midori 154696 190425_2 add <--
	CString			cs=_T("");

	if(mfcMain.Init() == DB_ERR_OK)	{
		CCtrlMainGeneralVar cm(mfcMain);
		if(cm.bUseStyle)	bG_Kanso = TRUE;
		else				bG_Kanso = FALSE;
// midori 154696 190425_2 add -->
		if(cm.bDataKakutei)	kakutei = TRUE;
		else				kakutei = FALSE;
// midori 154696 190425_2 add <--
		// 前年度からの更新直後
		if(mfcMain.m_GeneralVar[2] & BIT_D5)	zupd=TRUE;
// midori 154696 add -->
		if((mfcMain.m_GeneralVar[2] & BIT_D4) == 0)	{
			init = 1;
			mfcMain.Edit();
// midori 154696 190425 del -->
			//mfcMain.m_GeneralVar[2] &= ~BIT_D4;
// midori 154696 190425 del <--
// midori 154696 190425 add -->
			mfcMain.m_GeneralVar[2] |= BIT_D4;
// midori 154696 190425 add <--
			mfcMain.Update();
		}
// midori 154696 add <--
		mfcMain.Fin();
	}
	bcmp=bG_Kanso;

// midori UC_0060 add -->
	// 旧様式から新様式に印刷設定の省略文字がコピーされている
	// または新様式から旧様式に印刷設定の省略文字がコピーされている場合、
	// 一度だけ省略文字の修復処理を行う
	// (2020/08/20)
	//  旧様式のマスターへ新様式の省略文字をコピーした状態で修復を行わないまま
	//  KansoCheck-KansoDataConvertで旧様式から新様式への書き換えが行われると省略文字が破損するため
	//  KansoCheck-KansoDataConvertの前で修復を行うようにしました。
	if(mfcMain.Init() == DB_ERR_OK)	{
		CCtrlMainGeneralVar cm1(mfcMain);
		// １回目の修復処理が行われていない場合、旧様式の場合に限り修復処理を行う
		if(cm1.bOmitUpd == FALSE && bG_Kanso == FALSE) {
			int	rv = ChkOmit(m_pZmSub->m_database,bG_Kanso);
			if(rv == -1) {
				// エラー
				return;
			}
			// 自動修復が必要
			if(rv == 1) {
				// 新様式
				if(bG_Kanso == TRUE)	KansoOmitConvert(TRUE);
				// 旧様式
				else					KansoOmitConvert(FALSE);
			}
			// 修復済みのサインを立てる
			mfcMain.Edit();
			cm1.bOmitUpd = TRUE;
			cm1.bOmitUpd2 = TRUE;
			// 
			cm1.Set(mfcMain);
			mfcMain.Update();
		}
		mfcMain.Fin();
	}
// midori UC_0060 add <--

	m_pZmSub->VolumeOpen();
	symd = m_pZmSub->zvol->ss_ymd;
	eymd = m_pZmSub->zvol->ee_ymd;
	m_pZmSub->VolumeClose();

	// 1.期末が平成31年4月1日より前のマスターは現様式
	if(eymd < 20190401)	{
		//ICSMessageBox(_T("期末が31年4月1日より前なので、現様式で起動します。"));
		bcmp = FALSE;
		bG_KansoLock = TRUE;
	}
	// 2.期首が平成31年4月1日以降のマスターは新様式
	else if(symd >= 20190401)	{
		//ICSMessageBox(_T("期首が31年4月1日以降なので、新様式で起動します。"));
		bcmp = TRUE;
		bG_KansoLock = TRUE;
	}
	// 3.上記以外はプログラム内で様式を切り替えられるようにする
	else	{
// midori 154696 add -->
		if(init != 0)	{
			bcmp = TRUE;
		}
// midori 154696 add <--
		bG_KansoLock = FALSE;
	}

	// UCHIIMPでも様式を参照できるように (uc_inf_main.GeneralVar[2] & 0x20) に書き込む
	bG_KansoMessage = FALSE;
// midori 154696 190425_2 del -->
	//if(bcmp != bG_Kanso || zupd == TRUE)	{
// midori 154696 190425_2 del <--
// midori 154696 190425_2 add -->
	// 1.期間から判定した様式が今まで使用していた様式と異なる または
	// 2.前年度からの更新を行った後で、当年度と過年度の様式が異なる場合 かつ
	// 3.入力確定がＯＦＦである
	if((bcmp != bG_Kanso || zupd == TRUE) && (kakutei == FALSE))	{
// midori 154696 190425_2 add <--
		if(mfcMain.Init() == DB_ERR_OK)	{
			mfcMain.Edit();
			if(bG_Kanso == FALSE)	{
				bG_KansoMessage = TRUE;
			}
			bG_Kanso = bcmp;
			if(bG_Kanso == TRUE)	mfcMain.m_GeneralVar[2] |= BIT_D1;
			else					mfcMain.m_GeneralVar[2] &= ~BIT_D1;
			if(zupd == TRUE)		mfcMain.m_GeneralVar[2] &= ~BIT_D5;
			mfcMain.Update();
			mfcMain.Fin();
			// 様式によっては改行文字数がかわる項目があるのでデータを書き換える
			KansoDataConvert();
		}
	}
}

//***************************************************************************
// 翌期更新後に期首が平成31年4月1日以降になる場合は新様式となるためメッセージを表示する
//***************************************************************************
void CUCHIWAKEApp::KansoMessage(void)
{
	CdbUcInfMain	mfcMain(m_pZmSub->m_database);
	CString			cs=_T("");

// インボイス登録番号追加対応_23/11/06 del -->
//	if(bG_KansoMessage == TRUE)	{
//		if(mfcMain.Init() == DB_ERR_OK)	{
//// midori 190801 del -->
////// midori 154696 del -->
////			//if(mfcMain.m_GeneralVar[2] & BIT_D4)	{
////// midori 154696 del <--
////// midori 154672 del -->
////				//cs = _T("期首年月日が平成31年4月1日以降になりました。\n");
////				//cs+= _T("記載内容の簡素化に対応した様式を使用します。");
////// midori 154672 del <--
////// midori 154672 add -->
////				cs = _T("記載内容の簡素化に対応した様式を使用します。");
////// midori 154672 add <--
//// midori 190801 del <--
//// midori 190801 add -->
//				cs = _T("平成31年4月1日以後終了事業年度分（記載内容の簡素化）の様式を使用します。");
//// midori 190801 add <--
//				ICSMessageBox(cs,MB_ICONINFORMATION);
//
//				mfcMain.Edit();
//// midori 154696 190425 del -->
//				//mfcMain.m_GeneralVar[2] &= ~BIT_D4;
//// midori 154696 190425 del <--
//				mfcMain.Update();
//// midori 154696 del -->
//			//}
//// midori 154696 del <--
//			mfcMain.Fin();
//		}
//	}
// インボイス登録番号追加対応_23/11/06 del <--
// インボイス登録番号追加対応_23/11/06 add -->
	// マスターの状態によって表示するメッセージを変更する
	if(bG_InvnoMessage == TRUE)		{
		cs = _T("令和6年3月1日以後終了事業年度分「登録番号(法人番号)欄の追加」様式を使用します。");
		ICSMessageBox(cs,MB_ICONINFORMATION);
	}
	else if(bG_KansoMessage == TRUE)	{
		cs = _T("平成31年4月1日以後終了事業年度分（記載内容の簡素化）の様式を使用します。");
		ICSMessageBox(cs,MB_ICONINFORMATION);
	}
// インボイス登録番号追加対応_23/11/06 add <--
}

//***************************************************************************
// 新様式⇔旧様式の切り替えにより行数が変化するデータの書き換え
// 【引数】	int … 1:前年度からの更新後の呼び出し(データのみ書き換える)
//***************************************************************************
int CUCHIWAKEApp::KansoDataConvert( void )
{
	int					ii=0;
	int					sw=0;
	int					no=0;
	int					OmitSv=0;
	CString				OmitStrSv=_T("");
	CString				cs=_T("");
	CString				cs1=_T("");
	CString				cs2=_T("");
	CString				cs3=_T("");
	CString				cs4=_T("");
	CUcFunctionCommon	ufc;
	CStringArray		sa;

	// -----------------------------------------------------------------
	// ①預貯金等の内訳書
	// -----------------------------------------------------------------
	CdbUc011Yotyokin	db011(m_pZmSub->m_database);
	db011.Open();
	while(!db011.IsEOF()){		// ファイル終端まで
		// 1.支店名
		sw=0;
		cs = db011.m_BkName2;
		if(cs.IsEmpty() == FALSE)	{
			// 新様式 → 旧様式
			if(bG_Kanso == FALSE)	{
				// 改行マークが入っていれば削除して取り込む
				if(ufc.GetKaigyoCount(cs) > 0)	{
					cs.Replace(PRT_FORMAT_CHCODE,_T(""));
					sw = 1;
				}
			}
			// 旧様式 → 新様式
			else	{
				// 全角５文字を超えていれば改行マークを挿入して取り込む
				if(cs.GetLength() > 10)	{
					cs2 = ufc.GetSpritString(cs,&cs1,10);
					cs = cs2;
					cs3 = ufc.GetSpritString(cs,&cs2,10);
					cs = cs1 + PRT_FORMAT_CHCODE + cs2;
					sw = 1;
				}
			}
		}
		if(sw != 0)	{
			db011.Edit();
			db011.m_BkName2 = cs;
			db011.Update();
		}
		db011.MoveNext();
	}
	db011.Close();

	// 省略文字
	CdbUcInfSubOmit			iso(m_pZmSub->m_database);
	int						ItemSeq=0;
	CString					ItemName=_T("");
	if(iso.Init(ID_FORMNO_011) == DB_ERR_OK)	{
		if(!(iso.IsEOF()))	{
			// 新様式 → 旧様式
			if(bG_Kanso == FALSE)	{
				iso.MoveFirst();	// 昇順
				while(!iso.IsEOF())	{
					sw = 0;
					ItemSeq = iso.m_ItemSeq;
					ItemName = iso.m_ItemName;
					switch(iso.m_ItemSeq)	{
						case	2:
							sw = 2;
							break;
						case	3:
							ItemSeq = 2;
							sw = 1;
							break;
						default:
							break;
					}
					// Update
					if(sw == 1)	{
						iso.Edit();
						iso.m_ItemSeq = ItemSeq;
						iso.m_ItemName = ItemName;
						iso.Update();
					}
					// Delete
					else if(sw == 2)	{
						iso.Delete();
					}
					iso.MoveNext();
				}
			}
			// 旧様式 → 新様式
			else	{
				iso.MoveLast();		// 降順
				OmitSv = 0;
				OmitStrSv = _T("");
				while(!iso.IsBOF())	{
					sw = 0;
					ItemSeq = iso.m_ItemSeq;
					ItemName = iso.m_ItemName;
					switch(iso.m_ItemSeq)	{
						case	2:
							ItemSeq = 3;
							sw = 1;
							break;
						case	1:
							OmitSv = iso.m_Omit;
							OmitStrSv = iso.m_OmitStr;
							break;
						default:
							break;
					}
					// Update
					if(sw == 1)	{
						iso.Edit();
						iso.m_ItemSeq = ItemSeq;
						iso.m_ItemName = ItemName;
						iso.Update();
					}
					iso.MovePrev();
				}
				iso.AddNew();
				iso.m_FormSeq		= 1;
				iso.m_ItemName		= _T("支店名");
				iso.m_ItemSeq		= 2;
				iso.m_Omit			= OmitSv;
				iso.m_OmitStr		= OmitStrSv;
				iso.m_OmitMaxLen	= 8;
				iso.Update();
			}
		}
		iso.Fin();
	}

	// -----------------------------------------------------------------
	// ②受取手形の内訳書
	// -----------------------------------------------------------------
	CdbUc021Uketoritegata	db021(m_pZmSub->m_database);
	db021.Open();
	while(!db021.IsEOF()){		// ファイル終端まで
		// 1.摘要
		sw=0;
		cs=_T("");
		if(db021.m_Teki.IsEmpty() == FALSE)	{
			cs = db021.m_Teki;
			// 新様式 → 旧様式
			if(bG_Kanso == FALSE)	{
// midori 154680 del -->
				//// ３行以上入力している場合、一旦改行マークを取り除いて２行にする
				//if(ufc.GetKaigyoCount(cs) >= 2)	{
				//	sa.RemoveAll();
				//	ufc.StrDivision(cs,&sa,3);
				//	// ３行入力されていても２行目が空白の場合は１行目と３行目をくっつけるだけにする
				//	if(sa.GetAt(1).IsEmpty() == TRUE)	{
				//		cs = sa.GetAt(0) + PRT_FORMAT_CHCODE + sa.GetAt(2);
				//	}
				//	else	{
				//		cs.Replace(PRT_FORMAT_CHCODE,_T(""));
				//		cs2 = ufc.GetSpritString(cs,&cs1,MAX_KETA_021_TEKI);
				//		cs = cs1 + PRT_FORMAT_CHCODE + cs2;
				//	}
				//	sw = 1;
				//}
// midori 154680 del <--
// midori 154680 add -->
				if(ufc.GetKaigyoCount(cs) >= 1) {
					cs.Replace(PRT_FORMAT_CHCODE,_T(""));
					cs2 = ufc.GetSpritString(cs,&cs1,MAX_KETA_021_TEKI);
					cs = cs1 + PRT_FORMAT_CHCODE + cs2;
					sw = 1;
				}
// midori 154680 add <--
			}
			// 旧様式 → 新様式
			else	{
				sa.RemoveAll();
				ufc.StrDivision(cs,&sa,2);
				cs1 = sa.GetAt(0);
				cs2 = sa.GetAt(1);
				// ２行のうちいずれかが全角１１文字を超えている場合３行にする
				if( cs1.GetLength() > 22 || cs2.GetLength() > 22 )	{
					cs = cs1 + cs2;
					cs2 = ufc.GetSpritString(cs,&cs1,22);
					cs = cs2;
					cs3 = ufc.GetSpritString(cs,&cs2,22);
					cs = cs3;
					cs4 = ufc.GetSpritString(cs,&cs3,20);
					cs = cs1;
					if(cs2.IsEmpty() == FALSE)	cs = cs + PRT_FORMAT_CHCODE + cs2;
					if(cs3.IsEmpty() == FALSE)	cs = cs + PRT_FORMAT_CHCODE + cs3;
					sw = 1;
				}
			}
		}
		if(sw != 0)	{
			db021.Edit();
			db021.m_Teki = cs;
			db021.Update();
		}
		db021.MoveNext();
	}
	db021.Close();

	// 省略文字
	if(iso.Init(ID_FORMNO_021) == DB_ERR_OK)	{
		if(!(iso.IsEOF()))	{
			// 新様式 → 旧様式
			if(bG_Kanso == FALSE)	{
				iso.MoveFirst();	// 昇順
				while(!iso.IsEOF())	{
					sw = 0;
					ItemSeq = iso.m_ItemSeq;
					ItemName = iso.m_ItemName;
					switch(iso.m_ItemSeq)	{
						case	2:
							ItemName = _T("支払銀行名");
							sw = 1;
							break;
						case	3:
							sw = 2;
							break;
						case	4:
							ItemSeq = 3;
							ItemName = _T("割引銀行名");
							sw = 1;
							break;
						default:
							break;
					}
					// Update
					if(sw == 1)	{
						iso.Edit();
						iso.m_ItemSeq = ItemSeq;
						iso.m_ItemName = ItemName;
						iso.Update();
					}
					// Delete
					else if(sw == 2)	{
						iso.Delete();
					}
					iso.MoveNext();
				}
			}
			// 旧様式 → 新様式
			else	{
				OmitSv = 0;
				OmitStrSv = _T("");
				iso.MoveLast();		// 降順
				while(!iso.IsBOF())	{
					sw = 0;
					ItemSeq = iso.m_ItemSeq;
					ItemName = iso.m_ItemName;
					switch(iso.m_ItemSeq)	{
						case	3:
							ItemSeq = 4;
							ItemName = _T("割引銀行名及び支店名等");
							sw = 1;
							break;
						case	2:
							ItemName = _T("支払銀行名称");
							OmitSv = iso.m_Omit;
							OmitStrSv = iso.m_OmitStr;
							sw = 1;
							break;
						default:
							break;
					}
					// Update
					if(sw == 1)	{
						iso.Edit();
						iso.m_ItemSeq = ItemSeq;
						iso.m_ItemName = ItemName;
						iso.Update();
					}
					iso.MovePrev();
				}
				iso.AddNew();
				iso.m_FormSeq		= 2;
				iso.m_ItemName		= _T("支払銀行支店名");
				iso.m_ItemSeq		= 3;
				iso.m_Omit			= OmitSv;
				iso.m_OmitStr		= OmitStrSv;
				iso.m_OmitMaxLen	= 8;
				iso.Update();
			}
		}
		iso.Fin();
	}

	// -----------------------------------------------------------------
	// ④－２貸付金及び受取利息の内訳書
	// -----------------------------------------------------------------
	CdbUc042Kasituke		db042(m_pZmSub->m_database);
	db042.Open();
	while(!db042.IsEOF()){		// ファイル終端まで
		// 2.所在地(住所)
		for(ii=0; ii<2; ii++)	{
			sw=0;
			if(ii == 0)	cs = db042.m_AdAdd1;
			else		cs = db042.m_AdAdd2;
			if(cs.IsEmpty() == FALSE)	{
				// 新様式 → 旧様式
				if(bG_Kanso == FALSE)	{
					// 改行マークが入っていれば削除して取り込む
					if(ufc.GetKaigyoCount(cs) > 0)	{
						cs.Replace(PRT_FORMAT_CHCODE,_T(""));
						sw = 1;
					}
				}
				// 旧様式 → 新様式
				else	{
					// 全角１０文字を超えていれば改行マークを挿入して取り込む
					if(cs.GetLength() > 20)	{
						cs2 = ufc.GetSpritString(cs,&cs1,20);
						cs = cs2;
						cs3 = ufc.GetSpritString(cs,&cs2,20);
						cs = cs1 + PRT_FORMAT_CHCODE + cs2;
						sw = 1;
					}
				}
			}
			if(sw != 0)	{
				db042.Edit();
				if(ii == 0)	db042.m_AdAdd1 = cs;
				else		db042.m_AdAdd2 = cs;
				db042.Update();
			}
		}

// midori 154640 del -->
		//// 3.法人・代表者との関係
		//sw=0;
		//cs = db042.m_Kankei;
		//if(cs.IsEmpty() == FALSE)	{
		//	// 新様式 → 旧様式
		//	if(bG_Kanso == FALSE)	{
		//		// 改行マークが入っていれば削除して取り込む
		//		if(ufc.GetKaigyoCount(cs) > 0)	{
		//			cs.Replace(PRT_FORMAT_CHCODE,_T(""));
		//			sw = 1;
		//		}
		//	}
		//	// 旧様式 → 新様式
		//	else	{
		//		// 全角３文字を超えていれば改行マークを挿入して取り込む
		//		if(cs.GetLength() > 6)	{
		//			cs2 = ufc.GetSpritString(cs,&cs1,6);
		//			cs = cs2;
		//			cs3 = ufc.GetSpritString(cs,&cs2,6);
		//			cs = cs1 + PRT_FORMAT_CHCODE + cs2;
		//			sw = 1;
		//		}
		//	}
		//}
		//if(sw != 0)	{
		//	db042.Edit();
		//	db042.m_Kankei = cs;
		//	db042.Update();
		//}
// midori 154640 del <--

		// 一括金額行の場合「その他 999 件」の文言を住所２から貸付先に移行する
		if(db042.m_FgFunc == ID_FGFUNC_IKKATUMANUAL || db042.m_FgFunc == ID_FGFUNC_IKKATUAUTO)	{
			// 新様式 → 旧様式
			if(bG_Kanso == FALSE)	{
				cs = db042.m_AdName1 + db042.m_AdName2;
				cs.Replace(PRT_FORMAT_CHCODE,_T(""));
				db042.Edit();
				db042.m_AdName1 = _T("");
				db042.m_AdName2 = _T("");
				db042.m_AdAdd2 = cs;
				db042.Update();
			}
			// 旧様式 → 新様式
			else	{
				cs = db042.m_AdAdd2;
				cs.Replace(PRT_FORMAT_CHCODE,_T(""));
				// 全角１０文字を超えていれば２分割する
				if(cs.GetLength() > 20)	{
					cs2 = ufc.GetSpritString(cs,&cs1,20);
					cs = cs2;
					cs3 = ufc.GetSpritString(cs,&cs2,20);
				}
				else	{
					cs1 = cs;
					cs2 = _T("");
				}
				db042.Edit();
				db042.m_AdName1 = cs1;
				db042.m_AdName2 = cs2;
				db042.m_AdAdd2 = _T("");
				db042.Update();
			}
		}

// midori 154679 del -->
		//// 4.貸付理由
		//// 旧様式 → 新様式
		//if(bG_Kanso == TRUE)	{
		//	db042.Edit();
		//	db042.m_Riyuu = _T("");
		//	// 消した結果、何もなくなれば空行にする
		//	if ( db042.m_FgFunc <= ID_FGFUNC_DATA ) {	// データ行 または、空行なら
		//		if ( ( db042.m_AdName1 == "" )			// 画面上のデータ格納用(貸付先1)
		//		  && ( db042.m_AdName2 == "" )			// 画面上のデータ格納用(貸付先2)
		//		  && ( db042.m_Kankei == "" )			// 画面上のデータ格納用(法人・代表者との関係)
		//		  && ( db042.m_AdAdd1 == "" )			// 画面上のデータ格納用(所在地1（住所）)
		//		  && ( db042.m_AdAdd2 == "" )			// 画面上のデータ格納用(所在地2（住所）)
		//		  && ( db042.m_Val == "" )				// 画面上のデータ格納用(期末現在高)
		//		  && ( db042.m_Risoku == "" )			// 画面上のデータ格納用(期中の受取利息額)
		//		  && ( db042.m_Rate == "" )				// 画面上のデータ格納用(利率)
		//		  && ( db042.m_Riyuu == "" )			// 画面上のデータ格納用(貸付理由)
		//		  && ( db042.m_Naiyou == "" ) ) {		// 画面上のデータ格納用(担保の内容)
		//			// 空行なら
		//			db042.m_FgFunc = ID_FGFUNC_NULL;
		//		}
		//	}
		//	db042.Update();
		//}
// midori 154679 del <--

		db042.MoveNext();
	}
	db042.Close();

	// 省略文字
	if(iso.Init(ID_FORMNO_042) == DB_ERR_OK)	{
		if(!(iso.IsEOF()))	{
			iso.MoveFirst();	// 昇順
			while(!iso.IsEOF())	{
				sw = 0;
				ItemName = iso.m_ItemName;
				if(iso.m_ItemSeq == 1)	{
					// 新様式 → 旧様式
					if(bG_Kanso == FALSE)	{
						ItemName = _T("貸付先");
					}
					// 旧様式 → 新様式
					else	{
						ItemName = _T("貸付先（名称）");
					}
					sw = 1;
				}
				// Update
				if(sw == 1)	{
					iso.Edit();
					iso.m_ItemName = ItemName;
					iso.Update();
				}
				iso.MoveNext();
			}
		}
		iso.Fin();
	}

// midori 154679 del -->
	//// -----------------------------------------------------------------
	//// ⑤棚卸資産の内訳書
	//// -----------------------------------------------------------------
	//// 旧様式 → 新様式
	//if(bG_Kanso == TRUE)	{
	//	// 欄外データを削除する
	//	m_pZmSub->m_database->ExecuteSQL(_T("DELETE FROM uc_052_tanaoroshi2"));
	//}
// midori 154679 del <--

	// -----------------------------------------------------------------
	// ⑥有価証券の内訳書
	// -----------------------------------------------------------------
	// 省略文字
	if(iso.Init(ID_FORMNO_061) == DB_ERR_OK)	{
		if(!(iso.IsEOF()))	{
			// 新様式 → 旧様式
			if(bG_Kanso == FALSE)	{
				iso.MoveFirst();	// 昇順
				while(!iso.IsEOF())	{
					sw = 0;
					ItemSeq = iso.m_ItemSeq;
					ItemName = iso.m_ItemName;
					switch(iso.m_ItemSeq)	{
						case	2:
							ItemName = _T("種類・銘柄");
							sw = 1;
							break;
						case	3:
							sw = 2;
							break;
						case	4:
						case	5:
						case	6:
							ItemSeq = ItemSeq - 1;
							sw = 1;
							break;
						default:
							break;
					}
					// Update
					if(sw == 1)	{
						iso.Edit();
						iso.m_ItemSeq = ItemSeq;
						iso.m_ItemName = ItemName;
						iso.Update();
					}
					// Delete
					else if(sw == 2)	{
						iso.Delete();
					}
					iso.MoveNext();
				}
			}
			// 旧様式 → 新様式
			else	{
				OmitSv = 0;
				OmitStrSv = _T("");
				iso.MoveLast();		// 降順
				while(!iso.IsBOF())	{
					sw = 0;
					ItemSeq = iso.m_ItemSeq;
					ItemName = iso.m_ItemName;
					switch(iso.m_ItemSeq)	{
						case	5:
						case	4:
						case	3:
							ItemSeq = ItemSeq + 1;
							sw = 1;
							break;
						case	2:
							ItemName = _T("種類");
							OmitSv = iso.m_Omit;
							OmitStrSv = iso.m_OmitStr;
							sw = 1;
							break;
						default:
							break;
					}
					// Update
					if(sw == 1)	{
						iso.Edit();
						iso.m_ItemSeq = ItemSeq;
						iso.m_ItemName = ItemName;
						iso.Update();
					}
					iso.MovePrev();
				}
				iso.AddNew();
				iso.m_FormSeq		= 7;
				iso.m_ItemName		= _T("銘柄");
				iso.m_ItemSeq		= 3;
				iso.m_Omit			= OmitSv;
				iso.m_OmitStr		= OmitStrSv;
				iso.m_OmitMaxLen	= 8;
				iso.Update();
			}
		}
		iso.Fin();
	}

	// -----------------------------------------------------------------
	// ⑧支払手形の内訳書
	// -----------------------------------------------------------------
	CdbUc081Siharaitegata	db081(m_pZmSub->m_database);
	db081.Open();
	while(!db081.IsEOF()){		// ファイル終端まで
		//// 1.支払先
		//sw=0;
		//cs1=_T("");
		//cs2=_T("");
		//if(db081.m_AdName1.IsEmpty() == FALSE || db081.m_AdName2.IsEmpty() == FALSE)	{
		//	// 新様式 → 旧様式
		//	if(bG_Kanso == FALSE)	{
		//		cs = db081.m_AdName1 + db081.m_AdName2;
		//		sa.RemoveAll();
		//		ufc.StrDivision(cs,&sa,3);
		//		// ３行以上入力している場合、一旦改行マークを取り除いて２行にする
		//		if(ufc.GetKaigyoCount(cs) >= 2)	{
		//			// ３行入力されていても２行目が空白の場合は１行目と３行目をセットする
		//			if(sa.GetAt(1).IsEmpty() == TRUE)	{
		//				cs1 = sa.GetAt(0);
		//				cs2 = sa.GetAt(2);
		//			}
		//			else	{
		//				cs.Replace(PRT_FORMAT_CHCODE,_T(""));
		//				cs2 = ufc.GetSpritString(cs,&cs1,MAX_KETA_042_ADNAME1);
		//			}
		//		}
		//		else	{
		//			cs1 = sa.GetAt(0);
		//			cs2 = sa.GetAt(1);

		//		}
		//		sw = 1;
		//	}
		//	// 旧様式 → 新様式
		//	else	{
		//		cs = db081.m_AdName1 + PRT_FORMAT_CHCODE + db081.m_AdName2;
		//		sa.RemoveAll();
		//		ufc.StrDivision(cs,&sa,2);
		//		cs1 = sa.GetAt(0);
		//		cs2 = sa.GetAt(1);
		//		// ２行のうちいずれかが全角７文字を超えている場合３行にする
		//		if(cs1.GetLength() > 14 || cs2.GetLength() > 14)	{
		//			cs = cs1 + cs2;
		//			cs2 = ufc.GetSpritString(cs,&cs1,14);
		//			cs = cs2;
		//			cs3 = ufc.GetSpritString(cs,&cs2,14);
		//			cs = cs1;
		//			if(cs2.IsEmpty() == FALSE)	cs = cs + PRT_FORMAT_CHCODE + cs2;
		//			if(cs3.IsEmpty() == FALSE)	cs = cs + PRT_FORMAT_CHCODE + cs3;
		//			cs2 = ufc.GetSpritString(cs,&cs1,MAX_KETA_081_ADNAME1);
		//			sw = 1;
		//		}
		//		else	{
		//			if(cs1.IsEmpty() == FALSE && cs2.IsEmpty() == FALSE)	{
		//				cs1 = cs1 + PRT_FORMAT_CHCODE;
		//				sw = 1;
		//			}
		//		}
		//	}
		//}
		//if(sw != 0)	{
		//	db081.Edit();
		//	db081.m_AdName1 = cs1;
		//	db081.m_AdName2 = cs2;
		//	db081.Update();
		//}

		// 2.摘要
		sw=0;
		cs = db081.m_Teki;
		if(cs.IsEmpty() == FALSE)	{
			// 新様式 → 旧様式
			if(bG_Kanso == FALSE)	{
// midori 154680 del -->
				//// ３行入力している場合、一旦改行マークを取り除いて２行にする
				//if(ufc.GetKaigyoCount(cs) >= 2)	{
				//	sa.RemoveAll();
				//	ufc.StrDivision(cs,&sa,3);
				//	// ３行入力されていても２行目が空白の場合は１行目と３行目をくっつけるだけにする
				//	if(sa.GetAt(1).IsEmpty() == TRUE)	{
				//		cs = sa.GetAt(0) + PRT_FORMAT_CHCODE + sa.GetAt(2);
				//	}
				//	else	{
				//		cs.Replace(PRT_FORMAT_CHCODE,_T(""));
				//		cs2 = ufc.GetSpritString(cs,&cs1,MAX_KETA_081_TEKI);
				//		cs = cs1 + PRT_FORMAT_CHCODE + cs2;
				//	}
				//	sw = 1;
				//}
// midori 154680 del <--
// midori 154680 add -->
				if(ufc.GetKaigyoCount(cs) >= 1) {
					cs.Replace(PRT_FORMAT_CHCODE,_T(""));
					cs2 = ufc.GetSpritString(cs,&cs1,MAX_KETA_081_TEKI);
					cs = cs1 + PRT_FORMAT_CHCODE + cs2;
					sw = 1;
				}
// midori 154680 add <--
			}
			// 旧様式 → 新様式
			else	{
				sa.RemoveAll();
				ufc.StrDivision(cs,&sa,2);
				cs1 = sa.GetAt(0);
				cs2 = sa.GetAt(1);
				// ２行のうちいずれかが全角１５文字を超えている場合３行にする
				if(cs1.GetLength() > 30 || cs2.GetLength() > 30)	{
					cs = cs1 + cs2;
					cs2 = ufc.GetSpritString(cs,&cs1,30);
					cs = cs2;
					cs3 = ufc.GetSpritString(cs,&cs2,30);
					cs = cs3;
					cs4 = ufc.GetSpritString(cs,&cs3,20);
					cs = cs1;
					if(cs2.IsEmpty() == FALSE)	cs = cs + PRT_FORMAT_CHCODE + cs2;
					if(cs3.IsEmpty() == FALSE)	cs = cs + PRT_FORMAT_CHCODE + cs3;
					sw = 1;
				}
			}
		}
		if(sw != 0)	{
			db081.Edit();
			db081.m_Teki = cs;
			db081.Update();
		}

		db081.MoveNext();
	}
	db081.Close();

	// 省略文字
	if(iso.Init(ID_FORMNO_081) == DB_ERR_OK)	{
		if(!(iso.IsEOF()))	{
			iso.MoveFirst();	// 昇順
			OmitSv = 0;
			OmitStrSv = _T("");
			while(!iso.IsEOF())	{
				sw = 0;
				ItemSeq = iso.m_ItemSeq;
				ItemName = iso.m_ItemName;
				if(iso.m_ItemSeq == 2)	{
					// 新様式 → 旧様式
					if(bG_Kanso == FALSE)	ItemName = _T("支払銀行名");
					else					ItemName = _T("支払銀行名称");
					OmitSv = iso.m_Omit;
					OmitStrSv = iso.m_OmitStr;
					sw = 1;
				}
				else if(iso.m_ItemSeq == 3)	{
					sw = 2;
				}
				// Update
				if(sw == 1)	{
					iso.Edit();
					iso.m_ItemSeq = ItemSeq;
					iso.m_ItemName = ItemName;
					iso.Update();
				}
				// Delete
				else if(sw == 2)	{
					iso.Delete();
				}
				iso.MoveNext();
			}
			// 旧様式 → 新様式
			if(bG_Kanso == TRUE)	{
				iso.AddNew();
				iso.m_FormSeq		= 9;
				iso.m_ItemName		= _T("支払銀行支店名");
				iso.m_ItemSeq		= 3;
				iso.m_Omit			= OmitSv;	// 支払銀行名称に合わせる
				iso.m_OmitStr		= OmitStrSv;
				iso.m_OmitMaxLen	= 8;
				iso.Update();
			}
		}
		iso.Fin();
	}

	// -----------------------------------------------------------------
	// ⑪借入金及び支払利子の内訳書
	// -----------------------------------------------------------------
	CdbUc111Kariirekin		db111(m_pZmSub->m_database);
	db111.Open();
	while(!db111.IsEOF()){		// ファイル終端まで
		//// 1.支払先
		//sw=0;
		//cs1=_T("");
		//cs2=_T("");
		//if(db111.m_AdName1.IsEmpty() == FALSE || db111.m_AdName2.IsEmpty() == FALSE)	{
		//	// 新様式 → 旧様式
		//	if(bG_Kanso == FALSE)	{
		//		cs = db111.m_AdName1 + db111.m_AdName2;
		//		sa.RemoveAll();
		//		ufc.StrDivision(cs,&sa,3);
		//		// ３行以上入力している場合、一旦改行マークを取り除いて２行にする
		//		if(ufc.GetKaigyoCount(cs) >= 2)	{
		//			// ３行入力されていても２行目が空白の場合は１行目と３行目をセットする
		//			if(sa.GetAt(1).IsEmpty() == TRUE)	{
		//				cs1 = sa.GetAt(0);
		//				cs2 = sa.GetAt(2);
		//			}
		//			else	{
		//				cs.Replace(PRT_FORMAT_CHCODE,_T(""));
		//				cs2 = ufc.GetSpritString(cs,&cs1,MAX_KETA_111_ADNAME1);
		//			}
		//		}
		//		else	{
		//			cs1 = sa.GetAt(0);
		//			cs2 = sa.GetAt(1);

		//		}
		//		sw = 1;
		//	}
		//	// 旧様式 → 新様式
		//	else	{
		//		cs = db111.m_AdName1 + PRT_FORMAT_CHCODE + db111.m_AdName2;
		//		sa.RemoveAll();
		//		ufc.StrDivision(cs,&sa,2);
		//		cs1 = sa.GetAt(0);
		//		cs2 = sa.GetAt(1);
		//		// ２行のうちいずれかが全角７文字を超えている場合３行にする
		//		if(cs1.GetLength() > 14 || cs2.GetLength() > 14)	{
		//			cs = cs1 + cs2;
		//			cs2 = ufc.GetSpritString(cs,&cs1,14);
		//			cs = cs2;
		//			cs3 = ufc.GetSpritString(cs,&cs2,14);
		//			cs = cs1;
		//			if(cs2.IsEmpty() == FALSE)	cs = cs + PRT_FORMAT_CHCODE + cs2;
		//			if(cs3.IsEmpty() == FALSE)	cs = cs + PRT_FORMAT_CHCODE + cs3;
		//			cs2 = ufc.GetSpritString(cs,&cs1,MAX_KETA_111_ADNAME1);
		//			sw = 1;
		//		}
		//		else	{
		//			if(cs1.IsEmpty() == FALSE && cs2.IsEmpty() == FALSE)	{
		//				cs1 = cs1 + PRT_FORMAT_CHCODE;
		//				sw = 1;
		//			}
		//		}
		//	}
		//}
		//if(sw != 0)	{
		//	db111.Edit();
		//	db111.m_AdName1 = cs1;
		//	db111.m_AdName2 = cs2;
		//	db111.Update();
		//}

		// 2.所在地(住所)
		for(ii=0; ii<2; ii++)	{
			sw=0;
			if(ii == 0)	cs = db111.m_AdAdd1;
			else		cs = db111.m_AdAdd2;
			if(cs.IsEmpty() == FALSE)	{
				// 新様式 → 旧様式
				if(bG_Kanso == FALSE)	{
					// 改行マークが入っていれば削除して取り込む
					if(ufc.GetKaigyoCount(cs) > 0)	{
						cs.Replace(PRT_FORMAT_CHCODE,_T(""));
						sw = 1;
					}
				}
				// 旧様式 → 新様式
				else	{
					// 全角１０文字を超えていれば改行マークを挿入して取り込む
					if(cs.GetLength() > 20)	{
						cs2 = ufc.GetSpritString(cs,&cs1,20);
						cs = cs2;
						cs3 = ufc.GetSpritString(cs,&cs2,20);
						cs = cs1 + PRT_FORMAT_CHCODE + cs2;
						sw = 1;
					}
				}
			}
			if(sw != 0)	{
				db111.Edit();
				if(ii == 0)	db111.m_AdAdd1 = cs;
				else		db111.m_AdAdd2 = cs;
				db111.Update();
			}
		}

// midori 154640 del -->
		//// 3.法人・代表者との関係
		//sw=0;
		//cs = db111.m_Kankei;
		//if(cs.IsEmpty() == FALSE)	{
		//	// 新様式 → 旧様式
		//	if(bG_Kanso == FALSE)	{
		//		// 改行マークが入っていれば削除して取り込む
		//		if(ufc.GetKaigyoCount(cs) > 0)	{
		//			cs.Replace(PRT_FORMAT_CHCODE,_T(""));
		//			sw = 1;
		//		}
		//	}
		//	// 旧様式 → 新様式
		//	else	{
		//		// 全角３文字を超えていれば改行マークを挿入して取り込む
		//		if(cs.GetLength() > 6)	{
		//			cs2 = ufc.GetSpritString(cs,&cs1,6);
		//			cs = cs2;
		//			cs3 = ufc.GetSpritString(cs,&cs2,6);
		//			cs = cs1 + PRT_FORMAT_CHCODE + cs2;
		//			sw = 1;
		//		}
		//	}
		//}
		//if(sw != 0)	{
		//	db111.Edit();
		//	db111.m_Kankei = cs;
		//	db111.Update();
		//}
// midori 154640 del <--

		// 3.担保の内容
		sw=0;
		cs=_T("");
		if(db111.m_Naiyou.IsEmpty() == FALSE)	{
			cs = db111.m_Naiyou;
			// 新様式 → 旧様式
			if(bG_Kanso == FALSE)	{
// midori 154680 del -->
				//// ３行以上入力している場合、一旦改行マークを取り除いて２行にする
				//if(ufc.GetKaigyoCount(cs) >= 2)	{
				//	sa.RemoveAll();
				//	ufc.StrDivision(cs,&sa,3);
				//	// ３行入力されていても２行目が空白の場合は１行目と３行目をくっつけるだけにする
				//	if(sa.GetAt(1).IsEmpty() == TRUE)	{
				//		cs = sa.GetAt(0) + PRT_FORMAT_CHCODE + sa.GetAt(2);
				//	}
				//	else	{
				//		cs.Replace(PRT_FORMAT_CHCODE,_T(""));
				//		cs2 = ufc.GetSpritString(cs,&cs1,MAX_KETA_111_NAIYOU);
				//		cs = cs1 + PRT_FORMAT_CHCODE + cs2;
				//	}
				//	sw = 1;
				//}
// midori 154680 del <--
// midori 154680 add -->
				if(ufc.GetKaigyoCount(cs) >= 1) {
					cs.Replace(PRT_FORMAT_CHCODE,_T(""));
					cs2 = ufc.GetSpritString(cs,&cs1,MAX_KETA_111_NAIYOU);
					cs = cs1 + PRT_FORMAT_CHCODE + cs2;
					sw = 1;
				}
// midori 154680 add <--
			}
			// 旧様式 → 新様式
			else	{
				sa.RemoveAll();
				ufc.StrDivision(cs,&sa,2);
				cs1 = sa.GetAt(0);
				cs2 = sa.GetAt(1);
				// ２行のうちいずれかが全角１０文字を超えている場合３行にする
				if( cs1.GetLength() > 20 || cs2.GetLength() > 20 )	{
					cs = cs1 + cs2;
					cs2 = ufc.GetSpritString(cs,&cs1,20);
					cs = cs2;
					cs3 = ufc.GetSpritString(cs,&cs2,20);
					cs = cs3;
					cs4 = ufc.GetSpritString(cs,&cs3,12);
					cs = cs1;
					if(cs2.IsEmpty() == FALSE)	cs = cs + PRT_FORMAT_CHCODE + cs2;
					if(cs3.IsEmpty() == FALSE)	cs = cs + PRT_FORMAT_CHCODE + cs3;
					sw = 1;
				}
			}
		}
		if(sw != 0)	{
			db111.Edit();
			db111.m_Naiyou = cs;
			db111.Update();
		}

		// 一括金額行の場合「その他 999 件」の文言を住所２から借入先に移行する
		if(db111.m_FgFunc == ID_FGFUNC_IKKATUMANUAL || db111.m_FgFunc == ID_FGFUNC_IKKATUAUTO)	{
			// 新様式 → 旧様式
			if(bG_Kanso == FALSE)	{
				cs = db111.m_AdName1 + db111.m_AdName2;
				cs.Replace(PRT_FORMAT_CHCODE,_T(""));
				db111.Edit();
				db111.m_AdName1 = _T("");
				db111.m_AdName2 = _T("");
				db111.m_AdAdd2 = cs;
				db111.Update();
			}
			// 旧様式 → 新様式
			else	{
				cs = db111.m_AdAdd2;
				cs.Replace(PRT_FORMAT_CHCODE,_T(""));
				// 全角１０文字を超えていれば２分割する
				if(cs.GetLength() > 20)	{
					cs2 = ufc.GetSpritString(cs,&cs1,20);
					cs = cs2;
					cs3 = ufc.GetSpritString(cs,&cs2,20);
				}
				else	{
					cs1 = cs;
					cs2 = _T("");
				}
				db111.Edit();
				db111.m_AdName1 = cs1;
				db111.m_AdName2 = cs2;
				db111.m_AdAdd2 = _T("");
				db111.Update();
			}
		}

// midori 154679 del -->
		//// 4.借入理由
		//// 旧様式 → 新様式
		//if(bG_Kanso == TRUE)	{
		//	db111.Edit();
		//	db111.m_Riyuu = _T("");
		//	// 消した結果、何もなくなれば空行にする
		//	if ( db111.m_FgFunc <= ID_FGFUNC_DATA ) {	// データ行 または、空行なら
		//		if ( ( db111.m_KnOrder == 0 )			// 画面上のデータ格納用(科目)
		//		  && ( db111.m_AdName1 == "" )			// 画面上のデータ格納用(借入先1)
		//		  && ( db111.m_AdName2 == "" )			// 画面上のデータ格納用(借入先2)
		//		  && ( db111.m_Kankei == "" )			// 画面上のデータ格納用(法人・代表者との関係)
		//		  && ( db111.m_AdAdd1 == "" )			// 画面上のデータ格納用(所在地1)
		//		  && ( db111.m_AdAdd2 == "" )			// 画面上のデータ格納用(所在地2)
		//		  && ( db111.m_Val == "" )				// 画面上のデータ格納用(期末現在高)
		//		  && ( db111.m_Risoku == "" )			// 画面上のデータ格納用(期中の支払利息)
		//		  && ( db111.m_Rate == "" )				// 画面上のデータ格納用(利率)
		//		  && ( db111.m_Riyuu == "" )			// 画面上のデータ格納用(借入理由)
		//		  && ( db111.m_Naiyou == "" ) ) {		// 画面上のデータ格納用(担保の内容)
		//			// 空行なら
		//			db111.m_FgFunc = ID_FGFUNC_NULL;
		//		}
		//	}
		//	db111.Update();
		//}
// midori 154679 del <--

		db111.MoveNext();
	}
	db111.Close();

	// 省略文字
	if(iso.Init(ID_FORMNO_111) == DB_ERR_OK)	{
		if(!(iso.IsEOF()))	{
			iso.MoveFirst();	// 昇順
			while(!iso.IsEOF())	{
				sw = 0;
				ItemName = iso.m_ItemName;
				if(iso.m_ItemSeq == 1)	{
					// 新様式 → 旧様式
					if(bG_Kanso == FALSE)	{
						ItemName = _T("借入先");
					}
					// 旧様式 → 新様式
					else	{
						ItemName = _T("借入先（名称）");
					}
					sw = 1;
				}
				// Update
				if(sw == 1)	{
					iso.Edit();
					iso.m_ItemName = ItemName;
					iso.Update();
				}
				iso.MoveNext();
			}
		}
		iso.Fin();
	}

// midori 154679 del -->
	//// -----------------------------------------------------------------
	//// ⑬売上高等の事業所別内訳書
	//// -----------------------------------------------------------------
	//if(bG_Kanso == TRUE)	{
	//	CdbUc131Uriagedaka	db131(m_pZmSub->m_database);
	//	db131.Open();
	//	while(!db131.IsEOF()){		// ファイル終端まで
	//		db131.Edit();
	//		db131.m_Area = _T("");	// 使用建物の延面積を削除
	//		if ( db131.m_FgFunc <= ID_FGFUNC_DATA ) {	// データ行 または、空行なら
	//			if ( ( db131.m_AdName1 == "" )			// 画面上のデータ格納用(事業所の名称1)
	//			  && ( db131.m_AdAdd1 == "" )			// 画面上のデータ格納用(所在地1)
	//			  && ( db131.m_AdAdd2 == "" )			// 画面上のデータ格納用(所在地2)
	//			  && ( db131.m_HdName == "" )			// 画面上のデータ格納用(責任者氏名)
	//			  && ( db131.m_Kankei == "" )			// 画面上のデータ格納用(代表者との関係)
	//			  && ( db131.m_Naiyou == "" )			// 画面上のデータ格納用(事業所の内容)
	//			  && ( db131.m_Val == "" )				// 画面上のデータ格納用(売上高)
	//			  && ( db131.m_Val2 == "" )				// 画面上のデータ格納用(期末現在高)
	//			  && ( db131.m_Worker == "" )			// 画面上のデータ格納用(期末従業員数)
	//			  && ( db131.m_Area == "" )				// 画面上のデータ格納用(使用建物の延面積)
	//			  && ( db131.m_HdName2 == "" )			// 画面上のデータ格納用(源泉取得納税付署)
	//			  && ( db131.m_Teki == "" ) ) {			// 画面上のデータ格納用(摘要)
	//				// 空行なら
	//				db131.m_FgFunc = ID_FGFUNC_NULL;
	//			}
	//		}
	//		db131.Update();
	//		db131.MoveNext();
	//	}
	//	db131.Close();
	//}
// midori 154679 del <--

	// -----------------------------------------------------------------
	// ⑭－１役員給与等の内訳書
	// -----------------------------------------------------------------
	CdbUcInfSub				uisub(m_pZmSub->m_database);
	if(uisub.RequeryFormSeq(ID_FORMNO_141) == DB_ERR_OK)	{
		if(!uisub.IsEOF())	{
			uisub.Edit();
			// 新様式 → 旧様式
			if(bG_Kanso == FALSE)	{
				uisub.m_TitleOrg		= _T("役員報酬手当等の内訳書");
				uisub.m_TitleOrgTujo	= _T("役員報酬手当等及び人件費の内訳書");
				uisub.m_TitleSave		= _T("役員報酬手当等の内訳書");
				uisub.m_TitleSaveTujo	= _T("役員報酬手当等及び人件費の内訳書");
				uisub.m_Title			= _T("役員報酬手当等及び人件費の内訳書");
			}
			// 旧様式 → 新様式
			else	{
				uisub.m_TitleOrg		= _T("役員給与等の内訳書");
				uisub.m_TitleOrgTujo	= _T("役員給与等の内訳書");
				uisub.m_TitleSave		= _T("役員給与等の内訳書");
				uisub.m_TitleSaveTujo	= _T("役員給与等の内訳書");
				uisub.m_Title			= _T("役員給与等の内訳書");
			}
			// 更新
			uisub.Update();
		}
	}
	uisub.Close();

	// -----------------------------------------------------------------
	// ⑯－１雑役・雑損失等の内訳書
	// ⑯－２雑損失の内訳書
	// -----------------------------------------------------------------
	// 省略文字
	for(ii=0; ii<2; ii++)	{
		if(ii == 0)	no=ID_FORMNO_161;
		else		no=ID_FORMNO_162;
		if(iso.Init(no) == DB_ERR_OK)	{
			if(!(iso.IsEOF()))	{
				iso.MoveFirst();	// 昇順
				while(!iso.IsEOF())	{
					sw = 0;
					ItemName = iso.m_ItemName;
					if(iso.m_ItemSeq == 3)	{
						// 新様式 → 旧様式
						if(bG_Kanso == FALSE)	ItemName = _T("相手先");
						// 旧様式 → 新様式
						else					ItemName = _T("相手先（名称）");
						sw = 1;
					}
					if(iso.m_ItemSeq == 4)	{
						// 新様式 → 旧様式
						if(bG_Kanso == FALSE)	ItemName = _T("所在地");
						// 旧様式 → 新様式
						else					ItemName = _T("相手先（所在地）");
						sw = 1;
					}
					// Update
					if(sw == 1)	{
						iso.Edit();
						iso.m_ItemName = ItemName;
						iso.Update();
					}
					iso.MoveNext();
				}
			}
			iso.Fin();
		}
	}

	return(0);
}
// midori 190301 add <--

// midori UC_0037 add -->
//***************************************************************************
// 選択したマスターが内訳書単独マスターかどうかを調べる
// midori 155028 del -->
//// 【戻り値】	int … 1:単独マスター
// midori 155028 del <--
// midori 155028 add -->
// 【戻り値】	int … 0:財務ｄｂマスター　1:内訳書単独マスター　2:その他の単独マスター
// midori 155028 add <--
//***************************************************************************
int CUCHIWAKEApp::GetUcTandokuSw( int pApno, int pCocod, int pEymd )
{
	int				ii=0;
	int				ymd=0;
	int				ymd2=0;
	int				rv=0;
	INT_PTR			max=0;
	CString			cst=_T("");
	CString			emsg=_T("");
	CO_CTL_DB		co;
	CO_CTL_DB		coctl;
	CO_CTL_Array	coary;
	ICSMCTL*		mctl;

	while(1) {
		// ICSMCTLテーブルのデータベースに接続
		cst.Empty();
		PCOM_GetString(_T("SelectSqlServer"),cst.GetBuffer(256));
		cst.ReleaseBuffer();
		mctl = new ICSMCTL;
		if(mctl->SetDatabase(cst) < 0)	{
			rv = -1;
			emsg = _T("データベースの初期化に失敗しました。");
			break;
		}

		// ICSMCTLテーブルをオープン
		if(mctl->vctl_open(MODE_WRITE,pApno,TYPE_MS) < 0)	{
			rv = -1;
			emsg = _T("マスター管理テーブルのオープンに失敗しました。");
			break;
		}

		// ICSMCTLテーブルから会社情報を取得
		coctl.c_apno = pApno;
		int_bcd(coctl.c_cod,pCocod,4);
		coary.RemoveAll();
		if(mctl->vctl_coinf(&coctl,&coary) < 0)	{
			rv = -1;
			emsg = _T("マスター管理情報の取得に失敗しました。");
			break;
		}
		max = coary.GetSize();
		for(ii=0; ii<max; ii++) {
			co = coary.GetAt(ii);
			ymd = bcd_int(&co.c_kikan[3],3);
			ymd += (co.c_kikan[3] < 0x89) ? 20000000 : 19000000;
			// 現在選択している事業期間の場合、財務マスターが作成されているかどうかを調べる
			if(ymd == pEymd)	{
				rv = (co.c_exbin[128] & 0x01);
// midori 155028 add -->
				if(rv != 0)	{
					if((co.c_subsw & 0x100000) == 0x100000)	{
						rv = 1;
					}
					else	{
						rv = 2;
					}
				}
// midori 155028 add <--
				break;
			}
		}
		break;
	}
	if(rv == -1) {
		ICSMessageBox(emsg,MB_ICONSTOP);
	}
	return(rv);
}
// midori UC_0037 add <--

// midori 155262 add -->
// -------------------------------------------------------------------------------------------------------------------------------------
// 省略文字のコンバージョンを行う
// 【引数】	BOOL … 0:新様式 → 旧様式		1:旧様式 → 新様式
// -------------------------------------------------------------------------------------------------------------------------------------
int CUCHIWAKEApp::KansoOmitConvert( BOOL bKansosw )
{
	int					ii=0;
	int					sw=0;
	int					no=0;
	int					OmitSv=0;
	CString				OmitStrSv=_T("");
	CString				cs=_T("");
	CString				cs1=_T("");
	CString				cs2=_T("");
	CString				cs3=_T("");
	CString				cs4=_T("");
	CUcFunctionCommon	ufc;
	CStringArray		sa;

	// -----------------------------------------------------------------
	// ①預貯金等の内訳書
	// -----------------------------------------------------------------
	// 省略文字
	CdbUcInfSubOmit			iso(m_pZmSub->m_database);
	int						ItemSeq=0;
	CString					ItemName=_T("");
	if(iso.Init(ID_FORMNO_011) == DB_ERR_OK)	{
		if(!(iso.IsEOF()))	{
			// 新様式 → 旧様式
			if(bKansosw == FALSE)	{
				iso.MoveFirst();	// 昇順
				while(!iso.IsEOF())	{
					sw = 0;
					ItemSeq = iso.m_ItemSeq;
					ItemName = iso.m_ItemName;
					switch(iso.m_ItemSeq)	{
						case	2:
							sw = 2;
							break;
						case	3:
							ItemSeq = 2;
							sw = 1;
							break;
						default:
							break;
					}
					// Update
					if(sw == 1)	{
						iso.Edit();
						iso.m_ItemSeq = ItemSeq;
						iso.m_ItemName = ItemName;
						iso.Update();
					}
					// Delete
					else if(sw == 2)	{
						iso.Delete();
					}
					iso.MoveNext();
				}
			}
			// 旧様式 → 新様式
			else	{
				iso.MoveLast();		// 降順
				OmitSv = 0;
				OmitStrSv = _T("");
				while(!iso.IsBOF())	{
					sw = 0;
					ItemSeq = iso.m_ItemSeq;
					ItemName = iso.m_ItemName;
					switch(iso.m_ItemSeq)	{
						case	2:
							ItemSeq = 3;
							sw = 1;
							break;
						case	1:
							OmitSv = iso.m_Omit;
							OmitStrSv = iso.m_OmitStr;
							break;
						default:
							break;
					}
					// Update
					if(sw == 1)	{
						iso.Edit();
						iso.m_ItemSeq = ItemSeq;
						iso.m_ItemName = ItemName;
						iso.Update();
					}
					iso.MovePrev();
				}
				iso.AddNew();
				iso.m_FormSeq		= 1;
				iso.m_ItemName		= _T("支店名");
				iso.m_ItemSeq		= 2;
				iso.m_Omit			= OmitSv;
				iso.m_OmitStr		= OmitStrSv;
				iso.m_OmitMaxLen	= 8;
				iso.Update();
			}
		}
		iso.Fin();
	}

	// -----------------------------------------------------------------
	// ②受取手形の内訳書
	// -----------------------------------------------------------------
	// 省略文字
	if(iso.Init(ID_FORMNO_021) == DB_ERR_OK)	{
		if(!(iso.IsEOF()))	{
			// 新様式 → 旧様式
			if(bKansosw == FALSE)	{
				iso.MoveFirst();	// 昇順
				while(!iso.IsEOF())	{
					sw = 0;
					ItemSeq = iso.m_ItemSeq;
					ItemName = iso.m_ItemName;
					switch(iso.m_ItemSeq)	{
						case	2:
							ItemName = _T("支払銀行名");
							sw = 1;
							break;
						case	3:
							sw = 2;
							break;
						case	4:
							ItemSeq = 3;
							ItemName = _T("割引銀行名");
							sw = 1;
							break;
						default:
							break;
					}
					// Update
					if(sw == 1)	{
						iso.Edit();
						iso.m_ItemSeq = ItemSeq;
						iso.m_ItemName = ItemName;
						iso.Update();
					}
					// Delete
					else if(sw == 2)	{
						iso.Delete();
					}
					iso.MoveNext();
				}
			}
			// 旧様式 → 新様式
			else	{
				OmitSv = 0;
				OmitStrSv = _T("");
				iso.MoveLast();		// 降順
				while(!iso.IsBOF())	{
					sw = 0;
					ItemSeq = iso.m_ItemSeq;
					ItemName = iso.m_ItemName;
					switch(iso.m_ItemSeq)	{
						case	3:
							ItemSeq = 4;
							ItemName = _T("割引銀行名及び支店名等");
							sw = 1;
							break;
						case	2:
							ItemName = _T("支払銀行名称");
							OmitSv = iso.m_Omit;
							OmitStrSv = iso.m_OmitStr;
							sw = 1;
							break;
						default:
							break;
					}
					// Update
					if(sw == 1)	{
						iso.Edit();
						iso.m_ItemSeq = ItemSeq;
						iso.m_ItemName = ItemName;
						iso.Update();
					}
					iso.MovePrev();
				}
				iso.AddNew();
				iso.m_FormSeq		= 2;
				iso.m_ItemName		= _T("支払銀行支店名");
				iso.m_ItemSeq		= 3;
				iso.m_Omit			= OmitSv;
				iso.m_OmitStr		= OmitStrSv;
				iso.m_OmitMaxLen	= 8;
				iso.Update();
			}
		}
		iso.Fin();
	}

	// -----------------------------------------------------------------
	// ④－２貸付金及び受取利息の内訳書
	// -----------------------------------------------------------------
	// 省略文字
	if(iso.Init(ID_FORMNO_042) == DB_ERR_OK)	{
		if(!(iso.IsEOF()))	{
			iso.MoveFirst();	// 昇順
			while(!iso.IsEOF())	{
				sw = 0;
				ItemName = iso.m_ItemName;
				if(iso.m_ItemSeq == 1)	{
					// 新様式 → 旧様式
					if(bKansosw == FALSE)	{
						ItemName = _T("貸付先");
					}
					// 旧様式 → 新様式
					else	{
						ItemName = _T("貸付先（名称）");
					}
					sw = 1;
				}
				// Update
				if(sw == 1)	{
					iso.Edit();
					iso.m_ItemName = ItemName;
					iso.Update();
				}
				iso.MoveNext();
			}
		}
		iso.Fin();
	}

	// -----------------------------------------------------------------
	// ⑥有価証券の内訳書
	// -----------------------------------------------------------------
	// 省略文字
	if(iso.Init(ID_FORMNO_061) == DB_ERR_OK)	{
		if(!(iso.IsEOF()))	{
			// 新様式 → 旧様式
			if(bKansosw == FALSE)	{
				iso.MoveFirst();	// 昇順
				while(!iso.IsEOF())	{
					sw = 0;
					ItemSeq = iso.m_ItemSeq;
					ItemName = iso.m_ItemName;
					switch(iso.m_ItemSeq)	{
						case	2:
							ItemName = _T("種類・銘柄");
							sw = 1;
							break;
						case	3:
							sw = 2;
							break;
						case	4:
						case	5:
						case	6:
							ItemSeq = ItemSeq - 1;
							sw = 1;
							break;
						default:
							break;
					}
					// Update
					if(sw == 1)	{
						iso.Edit();
						iso.m_ItemSeq = ItemSeq;
						iso.m_ItemName = ItemName;
						iso.Update();
					}
					// Delete
					else if(sw == 2)	{
						iso.Delete();
					}
					iso.MoveNext();
				}
			}
			// 旧様式 → 新様式
			else	{
				OmitSv = 0;
				OmitStrSv = _T("");
				iso.MoveLast();		// 降順
				while(!iso.IsBOF())	{
					sw = 0;
					ItemSeq = iso.m_ItemSeq;
					ItemName = iso.m_ItemName;
					switch(iso.m_ItemSeq)	{
						case	5:
						case	4:
						case	3:
							ItemSeq = ItemSeq + 1;
							sw = 1;
							break;
						case	2:
							ItemName = _T("種類");
							OmitSv = iso.m_Omit;
							OmitStrSv = iso.m_OmitStr;
							sw = 1;
							break;
						default:
							break;
					}
					// Update
					if(sw == 1)	{
						iso.Edit();
						iso.m_ItemSeq = ItemSeq;
						iso.m_ItemName = ItemName;
						iso.Update();
					}
					iso.MovePrev();
				}
				iso.AddNew();
				iso.m_FormSeq		= 7;
				iso.m_ItemName		= _T("銘柄");
				iso.m_ItemSeq		= 3;
				iso.m_Omit			= OmitSv;
				iso.m_OmitStr		= OmitStrSv;
				iso.m_OmitMaxLen	= 8;
				iso.Update();
			}
		}
		iso.Fin();
	}

	// -----------------------------------------------------------------
	// ⑧支払手形の内訳書
	// -----------------------------------------------------------------
	// 省略文字
	if(iso.Init(ID_FORMNO_081) == DB_ERR_OK)	{
		if(!(iso.IsEOF()))	{
			iso.MoveFirst();	// 昇順
			OmitSv = 0;
			OmitStrSv = _T("");
			while(!iso.IsEOF())	{
				sw = 0;
				ItemSeq = iso.m_ItemSeq;
				ItemName = iso.m_ItemName;
				if(iso.m_ItemSeq == 2)	{
					// 新様式 → 旧様式
					if(bKansosw == FALSE)	ItemName = _T("支払銀行名");
					else					ItemName = _T("支払銀行名称");
					OmitSv = iso.m_Omit;
					OmitStrSv = iso.m_OmitStr;
					sw = 1;
				}
				else if(iso.m_ItemSeq == 3)	{
					sw = 2;
				}
				// Update
				if(sw == 1)	{
					iso.Edit();
					iso.m_ItemSeq = ItemSeq;
					iso.m_ItemName = ItemName;
					iso.Update();
				}
				// Delete
				else if(sw == 2)	{
					iso.Delete();
				}
				iso.MoveNext();
			}
			// 旧様式 → 新様式
			if(bKansosw == TRUE)	{
				iso.AddNew();
				iso.m_FormSeq		= 9;
				iso.m_ItemName		= _T("支払銀行支店名");
				iso.m_ItemSeq		= 3;
				iso.m_Omit			= OmitSv;	// 支払銀行名称に合わせる
				iso.m_OmitStr		= OmitStrSv;
				iso.m_OmitMaxLen	= 8;
				iso.Update();
			}
		}
		iso.Fin();
	}

	// -----------------------------------------------------------------
	// ⑪借入金及び支払利子の内訳書
	// -----------------------------------------------------------------
	// 省略文字
	if(iso.Init(ID_FORMNO_111) == DB_ERR_OK)	{
		if(!(iso.IsEOF()))	{
			iso.MoveFirst();	// 昇順
			while(!iso.IsEOF())	{
				sw = 0;
				ItemName = iso.m_ItemName;
				if(iso.m_ItemSeq == 1)	{
					// 新様式 → 旧様式
					if(bKansosw == FALSE)	{
						ItemName = _T("借入先");
					}
					// 旧様式 → 新様式
					else	{
						ItemName = _T("借入先（名称）");
					}
					sw = 1;
				}
				// Update
				if(sw == 1)	{
					iso.Edit();
					iso.m_ItemName = ItemName;
					iso.Update();
				}
				iso.MoveNext();
			}
		}
		iso.Fin();
	}

	// -----------------------------------------------------------------
	// ⑯－１雑役・雑損失等の内訳書
	// ⑯－２雑損失の内訳書
	// -----------------------------------------------------------------
	// 省略文字
	for(ii=0; ii<2; ii++)	{
		if(ii == 0)	no=ID_FORMNO_161;
		else		no=ID_FORMNO_162;
		if(iso.Init(no) == DB_ERR_OK)	{
			if(!(iso.IsEOF()))	{
				iso.MoveFirst();	// 昇順
				while(!iso.IsEOF())	{
					sw = 0;
					ItemName = iso.m_ItemName;
					if(iso.m_ItemSeq == 3)	{
						// 新様式 → 旧様式
						if(bKansosw == FALSE)	ItemName = _T("相手先");
						// 旧様式 → 新様式
						else					ItemName = _T("相手先（名称）");
						sw = 1;
					}
					if(iso.m_ItemSeq == 4)	{
						// 新様式 → 旧様式
						if(bKansosw == FALSE)	ItemName = _T("所在地");
						// 旧様式 → 新様式
						else					ItemName = _T("相手先（所在地）");
						sw = 1;
					}
					// Update
					if(sw == 1)	{
						iso.Edit();
						iso.m_ItemName = ItemName;
						iso.Update();
					}
					iso.MoveNext();
				}
			}
			iso.Fin();
		}
	}

	return(0);
}
// midori 155262 add <--

// midori UC_0060 add -->
// -------------------------------------------------------------------------------------------------------------------------------------
// 省略文字の修復を行う
// 【引数】		CDatabase*		… チェックを行うデータベースクラス
// -------------------------------------------------------------------------------------------------------------------------------------
BOOL CUCHIWAKEApp::KansoOmitRepair( CDatabase* pdb )
{
	int				ii=0,jj=0;
	int				formseq=0;
	int				sw=0;
	int				err_cnt=0;
	int				err_form[3];
	CRecordset		rs(pdb);
	CString			strData=_T(""),strData2=_T("");
	CString			strQuery=_T("");

// 2020/08/28追加 -->
	// 新様式のマスターにも関わらず、省略文字が旧様式の状態になっている場合は
	// 従来と同じ修復(メッセージを表示せずに、それまでの設定を引き継ぐ)を行う
	strQuery = _T("SELECT COUNT(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq=1");
	try	{
		rs.Open(CRecordset::forwardOnly, strQuery);
		rs.GetFieldValue((short)0,strData);
		rs.Close();
	}
	catch(CDBException* e)	{
		ICSMessageBox(e->m_strError);
		e->Delete();
		return(FALSE);
	}
	if(_tstoi(strData) == 2)	{
		KansoOmitConvert(TRUE);
		return(TRUE);
	}
// 2020/08/28追加 <--

	// 0 : ①預貯金等の内訳書
	// 1 : ②受取手形の内訳書
	// 2 : ⑥有価証券の内訳書
	for(ii=0; ii<3; ii++)	err_form[ii]=0;
	for(ii=0; ii<3; ii++)	{
		if(ii == 0)	formseq=1;
		if(ii == 1)	formseq=2;
		if(ii == 2)	formseq=7;
		// 件数を取得
		strQuery.Format(_T("SELECT COUNT(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq=%d"),formseq);
		try	{
			rs.Open(CRecordset::forwardOnly, strQuery);
			rs.GetFieldValue((short)0,strData);
			rs.Close();
		}
		catch(CDBException* e)	{
			ICSMessageBox(e->m_strError);
			e->Delete();
			return(FALSE);
		}
		// 最大ItemSeqを取得
		strQuery.Format(_T("SELECT MAX(ItemSeq) FROM uc_inf_sub_omit WHERE FormSeq=%d"),formseq);
		try	{
			rs.Open(CRecordset::forwardOnly, strQuery);
			rs.GetFieldValue((short)0,strData2);
			rs.Close();
		}
		catch(CDBException* e)	{
			ICSMessageBox(e->m_strError);
			e->Delete();
			return(FALSE);
		}
		// 
		sw = 0;
		if(ii == 0)	{
			if(_tstoi(strData) != 3 || _tstoi(strData2) != 3) sw = 1;
		}
		if(ii == 1)	{
			if(_tstoi(strData) != 4 || _tstoi(strData2) != 4) sw = 1;
		}
		if(ii == 2)	{
			if(_tstoi(strData) != 6 || _tstoi(strData2) != 6) sw = 1;
		}
		if(sw != 0)	{
			strQuery.Format(_T("DELETE uc_inf_sub_omit WHERE FormSeq=%d\r\n"),formseq);
			if(ii == 0)	{
				strQuery+= _T("INSERT INTO uc_inf_sub_omit VALUES (1, 1, '金融機関名', 0, '〃', 8)\r\n");
				strQuery+= _T("INSERT INTO uc_inf_sub_omit VALUES (1, 2, '支店名', 0, '〃', 8)\r\n");
				strQuery+= _T("INSERT INTO uc_inf_sub_omit VALUES (1, 3, '種類', 0, '〃', 8)\r\n");
			}
			if(ii == 1)	{
				strQuery+= _T("INSERT INTO uc_inf_sub_omit VALUES (2, 1, '振出人', 0, '〃', 8)\r\n");
				strQuery+= _T("INSERT INTO uc_inf_sub_omit VALUES (2, 2, '支払銀行名称', 0, '〃', 8)\r\n");
// midori 156695 del -->
				//strQuery+= _T("INSERT INTO uc_inf_sub_omit VALUES (2, 3, '割引銀行支店名', 0, '〃', 8)\r\n");
// midori 156695 del <--
// midori 156695 add -->
				strQuery+= _T("INSERT INTO uc_inf_sub_omit VALUES (2, 3, '支払銀行支店名', 0, '〃', 8)\r\n");
// midori 156695 add <--
				strQuery+= _T("INSERT INTO uc_inf_sub_omit VALUES (2, 4, '割引銀行名及び支店名等', 0, '〃', 8)\r\n");
			}
			if(ii == 2)	{
				strQuery+= _T("INSERT INTO uc_inf_sub_omit VALUES (7, 1, '区分', 0, '〃', 8)\r\n");
				strQuery+= _T("INSERT INTO uc_inf_sub_omit VALUES (7, 2, '種類', 0, '〃', 8)\r\n");
				strQuery+= _T("INSERT INTO uc_inf_sub_omit VALUES (7, 3, '銘柄', 0, '〃', 8)\r\n");
				strQuery+= _T("INSERT INTO uc_inf_sub_omit VALUES (7, 4, '異動事由', 0, '〃', 6)\r\n");
// midori 156696 del -->
				//strQuery+= _T("INSERT INTO uc_inf_sub_omit VALUES (7, 5, '売却先(名称）', 0, '〃', 8)\r\n");
// midori 156696 del <--
// midori 156696 add -->
				strQuery+= _T("INSERT INTO uc_inf_sub_omit VALUES (7, 5, '売却先（名称）', 0, '〃', 8)\r\n");
// midori 156696 add <--
				strQuery+= _T("INSERT INTO uc_inf_sub_omit VALUES (7, 6, '売却先（所在地）', 0, '〃', 8)\r\n");
			}
			pdb->ExecuteSQL(strQuery);
			err_cnt = err_cnt + 1;
			err_form[jj] = formseq;
			jj++;
		}
	}
// midori 156696 add -->
	strQuery = _T("UPDATE uc_inf_sub_omit SET ItemName='売却先（名称）' WHERE FormSeq=7 AND ItemSeq=5\r\n");
	strQuery+= _T("UPDATE uc_inf_sub_omit SET ItemName='売却先（名称）' WHERE FormSeq=8 AND ItemSeq=4");
	pdb->ExecuteSQL(strQuery);
// midori 156696 add <--
	if(err_cnt > 0)	{
		CString		str=_T("");
		CdbUcInfSub	mfcRec(pdb);

		str =_T("下記様式の印刷設定－省略文字に不正なデータが書き込まれています。\n");
		str+=_T("修復処理を行います。\n");
		str+=_T("\n");
		for(ii=0; ii<err_cnt; ii++)	{
			if(mfcRec.RequeryFormSeq(err_form[ii]) == DB_ERR_OK)	{
				if(!mfcRec.IsEOF())	{
					mfcRec.MoveFirst();
				}
				str+=_T("　");
				str+=mfcRec.m_TitleNo;
				str+=_T(" ");
				str+=mfcRec.m_Title;
				str+=_T("\n");
				mfcRec.Fin();
			}
		}
		str+=_T("\n");
		str+=_T("※修復処理後、省略文字は初期値に戻ります。");
		ICSMessageBox(str);
	}
	return(TRUE);
}
// midori UC_0060 add <--

// midori 155314 del -->
// -------------------------------------------------------------------------------------------------------------------------------------
// 省略文字の不一致のチェックを行う
// 【引数】		CDatabase*		… チェックを行うデータベースクラス
//				BOOL			… FALSE:旧様式		TRUE:新様式
// 【戻り値】	int				… -1:エラー	 0:正常		1:新様式 & 省略文字が旧様式 or 旧様式 & 省略文字が新様式
// -------------------------------------------------------------------------------------------------------------------------------------
int CUCHIWAKEApp::ChkOmit( CDatabase* pdb,BOOL bKansosw )
{
	int					ret = 0;
	CRecordset			rs(pdb);
	CString				strData = _T("");
	CString				strQuery = _T("");

	// 件数を取得
	try	{
		strQuery = _T("SELECT COUNT(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 1 ");
		rs.Open(CRecordset::forwardOnly, strQuery);
		rs.GetFieldValue((short)0,strData);
		rs.Close();
	}
	catch(CException* e)	{
		e->Delete();
		return(-1);
	}

	// 旧様式
	if(bKansosw == FALSE) {
		// 件数が2件でない場合、旧様式の省略文字になっていないため戻り値に1を返す
		if(_tstoi(strData) != 2) {
			ret = 1;
		}
	}
	else {
		// 件数が3件でない場合、旧様式の省略文字になっていないため戻り値に1を返す
		if(_tstoi(strData) != 3) {
			ret = 1;
		}
	}
	return(ret);
}
// midori 155314 del <--

// midori 191002 add -->
// ------------------------------------------------------------------------------------------------
//	科目名称付き小計行の括弧を全角から半角に変更する
//	【引数】	データベースクラス
//	【戻値】	なし
// ------------------------------------------------------------------------------------------------
void CUCHIWAKEApp::KeiStrConvert(CDatabase* pdb)
{
	int						sw=0;
	int						nbf=0;
	int						nbf2=0;
	int						nbf3=0;
	CString					cst=_T("");
	CString					cst2=_T("");
	CUcFunctionCommon		ufc;
	CdbUcInfMain			mfc(pdb);		// メイン情報テーブルクラス

	if (mfc.Init() == DB_ERR_OK) {
		CCtrlMainGeneralVar cm(mfc);
		if (cm.bKeiUpd)		sw = 1;
		mfc.Fin();
	}
	// 更新済みの場合処理を抜ける
	if (sw == 1)	return;

	// 021.受取手形の内訳書
	CdbUc021Uketoritegata	db021(pdb);
	db021.RequerySortPageRow();
	while(!db021.IsEOF()) {
		if(db021.m_FgFunc == ID_FGFUNC_SYOKEI) {
			if(db021.m_KeiStr.IsEmpty() == FALSE) {
				ufc.ChgKeiStr(db021.m_KeiStr,&cst);
				// 更新
				db021.Edit();
				db021.m_KeiStr = cst;
				db021.Update();
			}
		}
		// 次のレコードへ
		db021.MoveNext();
	}
	db021.Fin();

	// 111.借入金及び支払利子の内訳書
	CdbUc111Kariirekin		db111(pdb);
	db111.RequerySortPageRow();
	while(!db111.IsEOF()) {
		if(db111.m_FgFunc == ID_FGFUNC_SYOKEI) {
			if(db111.m_KeiStr.IsEmpty() == FALSE) {
				ufc.ChgKeiStr(db111.m_KeiStr,&cst);
				// 更新
				db111.Edit();
				db111.m_KeiStr = cst;
				db111.Update();
			}
		}
		// 次のレコードへ
		db111.MoveNext();
	}
	db111.Fin();

	// 更新サインをONにする
	if (mfc.Init() == DB_ERR_OK) {
		mfc.Edit();
		CCtrlMainGeneralVar cm(mfc);
		cm.bKeiUpd = 1;
		cm.Set(mfc);
		mfc.Update();
		mfc.Fin();
	}

}
// midori 191002 add <--

// midori 155327 add -->
// ------------------------------------------------------------------------------------------------
//	F9 参照(銀行) 修復処理
//　下記条件のレコードをF9 参照(取引先)から削除する
//　①名称の重複しているレコードがある
//	②支店名が入力されていない
//　③どの様式にも使用されていない
//
//	【引数】	CDatabase		データベースクラス
//	【戻値】	-1:エラー	0:正常
// ------------------------------------------------------------------------------------------------
int CUCHIWAKEApp::BankConvert(CDatabase* pdb)
{
	int										ii = 0;
	int										jj = 0;
	int										max = 0;
	int										cnt = 0;
	int										rv = 0;
	int										sw = 0;								// 1:様式に銀行の参照データあり
	int										sw2 = 0;							// 1:uc_lst_bankテーブルのレコード削除あり
	int										bkseq =0;
	CArray<int,int>							bkseqarray;
	CString									strcol=_T("");
	CString									strtblname1[4] = { _T("") };
	CString									strSQL = _T("");
	CString									strData =  _T("");
	CString									strbkseq = _T("");					// uc_lst_bankテーブルのシーケンス番号
	CdbUcLstBank							mfcBank(pdb);
	CRecordset								rs(pdb);

	bkseqarray.RemoveAll();

	// ①預貯金等の内訳書
	// ②受取手形の内訳書
	// ⑧支払手形の内訳書
	// ⑪借入金及び支払利子の内訳書
	strtblname1[0] = _T("uc_011_yotyokin");
	strtblname1[1] = _T("uc_021_uketoritegata");
	strtblname1[2] = _T("uc_081_siharaitegata");
	strtblname1[3] = _T("uc_111_kariirekin");

	// 銀行名の重複があり、支店名の入力されていないデータをマップに追加する
	strSQL = _T("select seq from uc_lst_bank a, \r\n");
	strSQL += _T("(select isnull(bkname1,'') as bkname1,count(bkname1) as cnt from uc_lst_bank group by isnull(bkname1,'')) b \r\n");
	strSQL += _T("where isnull(a.bkname1,'') = isnull(b.bkname1,'') \r\n");
	strSQL += _T("and b.cnt > 1 and isnull(a.bkname2,'') = ''");

	try{
		// テーブルオープン
		rv = rs.Open(CRecordset::forwardOnly, strSQL);
		while (!rs.IsEOF()) {
			strData.Empty();
			rs.GetFieldValue((short)0,strData);
			bkseqarray.Add(_tstoi(strData));
			rs.MoveNext();
		}
		rs.Close();

	}catch( CDBException *e){
		ICSMessageBox(e->m_strError);
		// 開放
		e->Delete();
		return(-1);
	}

	// 銀行名の重複があり支店名の入力が無いレコードを削除する
	max = bkseqarray.GetCount();
	if (max > 0) {
		for (ii = 0; ii < max; ii++) {
			bkseq = bkseqarray.GetAt(ii);
			sw = 0;
			for (jj = 0;jj < 4; jj++) {
				strbkseq.Format(_T("%d"),bkseq);
				strSQL = _T("select count(seq) from ") + strtblname1[jj] + _T(" \r\n");
				// ②受取手形の内訳書
				if(jj == 1) {
					strSQL += _T("where bkseq = ") + strbkseq + _T(" or bk2seq = ") + strbkseq;
				}
				// ⑪借入金及び支払利子の内訳書
				else if(jj == 3) {
					strSQL += _T("where adseq = ") + strbkseq;
				}
				// ①預貯金等の内訳書
				// ⑧支払手形の内訳書
				else {
					strSQL += _T("where bkseq = ") + strbkseq;
				}
				try{
					// テーブルオープン
					rv = rs.Open(CRecordset::forwardOnly, strSQL);
					if (!rs.IsEOF()) {
						strData.Empty();
						rs.GetFieldValue((short)0,strData);
						// 様式にデータあり
						if (_tstoi(strData) > 0) {
							sw = 1;
							rs.Close();
							break;
						}
					}
					rs.Close();
				}catch( CDBException *e){
					ICSMessageBox(e->m_strError);
					// 開放
					e->Delete();
					return(-1);
				}
			}
			// 様式にデータが無ければ削除を行う
			if (sw == 0) {
				strbkseq.Format(_T("%d"),bkseq);
				strSQL = _T("delete from uc_lst_bank where seq = ") + strbkseq;
				pdb->ExecuteSQL(strSQL);
				sw2 = 1;
			}
		}
	}
	// uc_lst_bankから削除が行われた場合、OrderNumの番号を詰める
	if (sw2 == 1) {
		// レコード削除後、番号を更新する
		if(mfcBank.Init() == DB_ERR_OK) {
			cnt = 1;
			if(!mfcBank.IsEOF()) {
				mfcBank.MoveFirst();
				while(!mfcBank.IsEOF()) {
					mfcBank.Edit();
					mfcBank.m_OrderNum = cnt;
					cnt++;
					mfcBank.Update();
					mfcBank.MoveNext();
				}
			}
			mfcBank.Fin();
		}
	}
	return(0);
}

// ------------------------------------------------------------------------------------------------
//	F9 参照(取引先) 修復処理
//　下記条件のレコードをF9 参照(取引先)から削除する
//　①名称の重複しているレコードがある
//	②所在地が入力されていない
//　③所属するグループのどの様式にも使用されていない
//
//	【引数】	CDatabase		データベースクラス
//	【戻値】	-1:エラー	0:正常
// ------------------------------------------------------------------------------------------------
int CUCHIWAKEApp::AddressConvert(CDatabase* pdb)
{
	// 削除用取引先マップ
	typedef struct tag_ADRESS_MAP {
		int			adseq;
		int			grseq;
		char		dmy[4];
	} ADRESS_MAP;

	int										ii = 0;
	int										jj = 0;
	int										kk = 0;								// 修正No.168534,168539 add
	int										max = 0;
	int										cnt = 0;
	int										rv = 0;
	int										sw = 0;								// 1:様式に取引先の参照データあり
	int										sw2 = 0;							// 1:uc_lst_addressテーブルのレコード削除あり
	int										grseq =0;
	ADRESS_MAP								adtmp;
	CMap<int,int,ADRESS_MAP,ADRESS_MAP>		admap;
	CString									strtblname1[5] = { _T("") };		// 得意先
	CString									strtblname2[4] = { _T("") };		// 仕入先
	CString									strSQL = _T("");
	CString									strData[2] = { _T("") };
	CString									stradseq = _T("");					// uc_lst_addressテーブルのシーケンス番号
	CdbUcLstAddress							mfcAddress(pdb);
	CRecordset								rs(pdb);

	admap.RemoveAll();

	// 得意先
	// ②	受取手形の内訳書
	// ③	売掛金（未収金）の内訳書
	// ④-2 貸付金及び受取利息の内訳書
	// ⑩-1	仮受金（前受金・預り金）
	// ⑯-2	雑損失等の内訳書
	strtblname1[0] = _T("uc_021_uketoritegata");
	strtblname1[1] = _T("uc_031_urikakekin");
	strtblname1[2] = _T("uc_042_kasituke");
	strtblname1[3] = _T("uc_101_kariukekin");
	strtblname1[4] = _T("uc_162_zatuson");
	// 仕入先
	// ④-1	仮払金（前渡金）の内訳書
	// ⑧	支払手形の内訳書
	// ⑨	買掛金（未払金・未払費用）の内訳書
	// ⑯-1	雑益、雑損失等の内訳書
	strtblname2[0] = _T("uc_041_karibarai");
	strtblname2[1] = _T("uc_081_siharaitegata");
	strtblname2[2] = _T("uc_091_kaikakekin");
	strtblname2[3] = _T("uc_161_zatueki");

// 修正No.168534,168539 add -->
	for(kk = 0; kk < 3; kk++) {
		if(kk == 0) {
// 修正No.168534,168539 add <--
// 修正No.168448 del -->
			//// 取引先名の重複があり、所在地の入力されていないデータをマップに追加する
			//strSQL = _T("select a.adseq,a.grseq from uc_lst_address a, \r\n");
			//strSQL += _T("(select grseq,isnull(adname1,'') as adname1,isnull(adname2,'') as adname2,count(adname1) as cnt from uc_lst_address group by grseq,isnull(adname1,''),isnull(adname2,'')) b \r\n");
			//strSQL += _T("where a.grseq = b.grseq and isnull(a.adname1,'') = isnull(b.adname1,'') and isnull(a.adname2,'') = isnull(b.adname2,'') \r\n");
			//strSQL += _T("and b.cnt > 1 and isnull(a.adadd1,'') = '' and isnull(a.adadd2,'') = ''");
// 修正No.168448 del <--
// 修正No.168448 add -->
			// 取引先名と登録番号の重複があり、所在地の入力されていないデータをマップに追加する
			strSQL  = _T("select a.adseq, a.grseq from uc_lst_address a, \r\n");
			strSQL += _T("(select grseq, isnull(adname1,'') as adname1, isnull(adname2,'') as adname2, Invno, count(adname1) as cnt \r\n");
			strSQL += _T("from uc_lst_address group by grseq, isnull(adname1,''), isnull(adname2,''), Invno) b \r\n");
			strSQL += _T("where a.grseq = b.grseq and isnull(a.adname1,'') = isnull(b.adname1,'') and isnull(a.adname2,'') = isnull(b.adname2,'') \r\n");
			strSQL += _T("and b.cnt > 1 and isnull(a.adadd1,'') = '' and isnull(a.adadd2,'') = '' and a.Invno = b.Invno ");
// 修正No.168448 add <--
// 修正No.168534,168539 add -->
		}
		else if(kk == 1) {
			// 取引先名の重複があり、登録番号、所在地の入力されていないデータをマップに追加する
			strSQL  = _T("select a.adseq, a.grseq from uc_lst_address a, \r\n");
			strSQL += _T("(select grseq, isnull(adname1,'') as adname1, isnull(adname2,'') as adname2, count(adname1) as cnt \r\n");
			strSQL += _T("from uc_lst_address group by grseq, isnull(adname1,''), isnull(adname2,'')) b \r\n");
			strSQL += _T("where a.grseq = b.grseq and isnull(a.adname1,'') = isnull(b.adname1,'') and isnull(a.adname2,'') = isnull(b.adname2,'') \r\n");
			strSQL += _T("and b.cnt > 1 and isnull(a.adadd1,'') = '' and isnull(a.adadd2,'') = '' and isnull(a.Invno,'') = '' ");
		}
		else	{
			// 登録番号の重複があり、取引先名、所在地の入力されていないデータをマップに追加する
			strSQL  = _T("select a.adseq, a.grseq from uc_lst_address a, \r\n");
			strSQL += _T("(select grseq, isnull(Invno,'') as Invno, count(Invno) as cnt \r\n");
			strSQL += _T("from uc_lst_address group by grseq, isnull(Invno,'')) b \r\n");
			strSQL += _T("where a.grseq = b.grseq and isnull(a.Invno,'') = isnull(b.Invno,'') \r\n");
			strSQL += _T("and b.cnt > 1 and isnull(a.adname1,'') = '' and isnull(a.adname2,'') = '' and isnull(a.adadd1,'') = '' and isnull(a.adadd2,'') = '' ");
		}
// 修正No.168534,168539 add <--

		try{
			// テーブルオープン
			rv = rs.Open(CRecordset::forwardOnly, strSQL);
			ii = 0;
			while (!rs.IsEOF()) {
				for (jj = 0; jj < 2; jj++) {
					strData[jj].Empty();
					rs.GetFieldValue((short)jj,strData[jj]);
				}
				memset(&adtmp,0,sizeof(ADRESS_MAP));
				adtmp.adseq = _tstoi(strData[0]);
				adtmp.grseq = _tstoi(strData[1]);
				admap.SetAt(ii,adtmp);
				ii++;
				rs.MoveNext();
			}
			rs.Close();

		}catch( CDBException *e){
			ICSMessageBox(e->m_strError);
			// 開放
			e->Delete();
			return(-1);
		}

		// 取引先名の重複があり住所の入力が無いレコードを削除する
		max = admap.GetCount();
		if (max > 0) {
			for (ii = 0; ii < max; ii++) {
				memset(&adtmp,0,sizeof(ADRESS_MAP));
				admap.Lookup(ii,adtmp);
				// 得意先
				if (adtmp.grseq == ID_ADDRESSGR_URI) {
					// ②	受取手形の内訳書
					// ③	売掛金（未収金）の内訳書
					// ④-2 貸付金及び受取利息の内訳書
					// ⑩-1	仮受金（前受金・預り金）
					// ⑯-2	雑損失等の内訳書
					sw = 0;
					for (jj = 0;jj < 5; jj++) {
						stradseq.Format(_T("%d"),adtmp.adseq);
						strSQL = _T("select count(seq) from ") + strtblname1[jj] + _T(" \r\n");
						strSQL += _T("where adseq = ") + stradseq;
						try{
							// テーブルオープン
							rv = rs.Open(CRecordset::forwardOnly, strSQL);
							if (!rs.IsEOF()) {
								strData[0].Empty();
								rs.GetFieldValue((short)0,strData[0]);
								// 様式にデータあり
								if (_tstoi(strData[0]) > 0) {
									sw = 1;
									rs.Close();
									break;
								}
							}
							rs.Close();
						}catch( CDBException *e){
							ICSMessageBox(e->m_strError);
							// 開放
							e->Delete();
							return(-1);
						}
					}
				}
				// 仕入先
				else {
					// ④-1	仮払金（前渡金）の内訳書
					// ⑧	支払手形の内訳書
					// ⑨	買掛金（未払金・未払費用）の内訳書
					// ⑯-1	雑益、雑損失等の内訳書
					sw = 0;
					for (jj = 0;jj < 4; jj++) {
						stradseq.Format(_T("%d"),adtmp.adseq);
						strSQL = _T("select count(seq) from ") + strtblname2[jj] + _T(" \r\n");
						strSQL += _T("where adseq = ") + stradseq;
						try{
							// テーブルオープン
							rv = rs.Open(CRecordset::forwardOnly, strSQL);
							if (!rs.IsEOF()) {
								strData[0].Empty();
								rs.GetFieldValue((short)0,strData[0]);
								// 様式にデータあり
								if (_tstoi(strData[0]) > 0) {
									sw = 1;
									rs.Close();
									break;
								}
							}
							rs.Close();
						}catch( CDBException *e){
							ICSMessageBox(e->m_strError);
							// 開放
							e->Delete();
							return(-1);
						}
					}
				}
				// 様式にデータが無ければ削除を行う
				if (sw == 0) {
					stradseq.Format(_T("%d"),adtmp.adseq);
					strSQL = _T("delete from uc_lst_address where Adseq = ") + stradseq;
					pdb->ExecuteSQL(strSQL);
					sw2 = 1;
				}
			}
		}
	}	// 修正No.168534,168539 add

	// uc_lst_addressから削除が行われた場合、OrderNumの番号を詰める
	if (sw2 == 1) {
		// グループごとにOrderNumの番号を詰める
		for(ii=0;ii<2;ii++) {
			// レコード削除後、番号を更新する
			if(ii==0)	grseq = ID_ADDRESSGR_URI;		// 得意先
			else		grseq = ID_ADDRESSGR_KAI;		// 仕入先
			if(mfcAddress.RequeryAdName(grseq,1) == DB_ERR_OK) {
				cnt = 1;
				if(!mfcAddress.IsEOF()) {
					mfcAddress.MoveFirst();
					while(!mfcAddress.IsEOF()) {
						mfcAddress.Edit();
						mfcAddress.m_OrderNum = cnt;
						cnt++;
						mfcAddress.Update();
						mfcAddress.MoveNext();
					}
				}
				mfcAddress.Fin();
			}
		}
	}
	return(0);
}
// midori 155327 add <--

// midori 156188 add -->
//	--------------------------------------------------------------------------------------------------------
//	科目行をマスターのデータに挿入する
//	--------------------------------------------------------------------------------------------------------
void CUCHIWAKEApp::KamokuRowConvert(CDatabase* pdb)
{
// 157154 del -->
	//// 一括金額AdOrder更新用構造体
	//typedef struct tag_SYOKEI_DATA {
	//	int			seq;
	//	int			numpage;
	//	int			numrow;
	//	char		keistr[64];
	//	char		dmy[52];
	//} SYOKEI_DATA;
// 157154 del <--

	int					iFormSeq=0;
// 157347 del -->
	//int					nBaseRow = 0;
	//int					ii=0;
	//int					max=0;
	//int					nLParen = 0;
	//int					nRParen = 0;
	//int					sort1=0;
// 157347 del <--
	BOOL				b021=FALSE,b111=FALSE;
// 157347 del -->
	//CString				strSQL=_T("");
// 157347 del <--
	CString				strKei=_T("");
// 157154 add -->
	CString				strKei2=_T("");
// 157154 add <--
// 157347 del -->
	//CString				temp=_T("");
// 157347 del <--
	ITEMSORT_INFO		uItemSort;			// 第１ソート項目のソート情報
	CdbUcInfSub			mfcRecSub(pdb);		//	uc_inf_subテーブルクラス
	CdbUcInfMain		mfcRecMain(pdb);	//	uc_inf_mainテーブルクラス
//	CArray<int,int>		syokeiseq;
// 157154 del -->
	//CMap<int,int,SYOKEI_DATA,SYOKEI_DATA> syokeimap;
	//SYOKEI_DATA			syokeidata;
// 157154 del <--
	CfrmUc000Common*	pView=NULL;		// FormViewオブジェクト
// 157154 del -->
	//CdbUc000Common*		rsData=NULL;
// 157154 del <--
// 157154 add -->
	CdbUc021Uketoritegata	dbRec021( m_pDB );
	CdbUc111Kariirekin		dbRec111( m_pDB );
// 157154 add <--

// 157154 del -->
	//syokeimap.RemoveAll();
// 157154 del <--

// midori 156188_4 add -->
// 改良No.21-0086,21-0529 cor -->
	//if(KamokuRowEnableSgn(pdb,0) == 0) {
// ------------------------------
	if(KamokuRowEnableSgn(pdb, 0, -1) == 0) {
// 改良No.21-0086,21-0529 cor <--
		return;
	}
// midori 156188_4 add <--

	if( mfcRecMain.Init() == DB_ERR_OK ){
		CCtrlMainGeneralVar cm(mfcRecMain);
		// 画面表示「科目行」対応後、科目行挿入済みサインがONの場合、処理を行わない
		if(cm.bKmkRowCnv) {
			// uc_inf_mainテーブルのクローズ
			mfcRecMain.Fin();
			return;
		}
		// 科目行挿入済みサインをONにする
		mfcRecMain.Edit();
		cm.bKmkRowCnv = TRUE;
		cm.Set(mfcRecMain);
		mfcRecMain.Update();
		// uc_inf_mainテーブルのクローズ
		mfcRecMain.Fin();
	}

	// 計名称（KeiStr）を50Byteから56byteに拡張する
	// ②受取手形の内訳書
	pdb->ExecuteSQL(_T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_021_uketoritegata'),'KeiStr','PRECISION')) is not null ALTER TABLE uc_021_uketoritegata ALTER COLUMN KeiStr varchar(56) "));
	// ⑪借入金及び支払利子の内訳書
	pdb->ExecuteSQL(_T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_111_kariirekin'),'KeiStr','PRECISION')) is not null ALTER TABLE uc_111_kariirekin ALTER COLUMN KeiStr varchar(56) "));

// 改良No.21-0086,21-0529 cor -->
	//if(KamokuRowEnableSgn(pdb,1) != 1) {
// ------------------------------
	if(KamokuRowEnableSgn(pdb, 1, -1) != 1) {
// 改良No.21-0086,21-0529 cor <--
		return;
	}

	for(int ii=0; ii<2; ii++)	{
		if(ii == 0)	iFormSeq = ID_FORMNO_021;
		else		iFormSeq = ID_FORMNO_111;
		// 様式でリクエリ
		if(mfcRecSub.RequeryFormSeq( iFormSeq ) == DB_ERR_OK)	{
			if( !(mfcRecSub.IsEOF()) ){
				// 「科目行を出力する」チェックついてる？
				if(mfcRecSub.m_GeneralConstVal[15] & BIT_D1)	{
					CCtrlSubGeneralVar	cs( mfcRecSub );
					if( cs.bOutKamokuRow )	{
						if(ii == 0)	b021 = TRUE;
						else		b111 = TRUE;
					}
				}
			}
			mfcRecSub.Fin();
		}
	}

// 157154 add -->
	// 小計名称が科目名付き、
	// 括弧＋科目名称が大文字12文字を超える場合、改行処理を行う
	// ②受取手形の内訳書
	dbRec021.RequeryFgFunc(ID_FGFUNC_SYOKEI,0);
	while(!dbRec021.IsEOF()) {
		// 小計名称の改行を行う
		m_clsFunc.KeiStrSprit(dbRec021.m_KeiStr,&strKei,&strKei2);
		if(strKei2.IsEmpty() == FALSE) {
			// 小計名を更新する
			dbRec021.Edit();
			dbRec021.m_KeiStr = strKei + _T("\r\n") + strKei2;
			dbRec021.Update();
		}
		dbRec021.MoveNext();
	}

	// ⑪借入金及び支払利子の内訳書
	dbRec111.RequeryFgFunc(ID_FGFUNC_SYOKEI,0);
	while(!dbRec111.IsEOF()) {
		// 小計名称の改行を行う
		m_clsFunc.KeiStrSprit(dbRec111.m_KeiStr,&strKei,&strKei2);
		if(strKei2.IsEmpty() == FALSE) {
			// 小計名を更新する
			dbRec111.Edit();
			dbRec111.m_KeiStr = strKei + _T("\r\n") + strKei2;
			dbRec111.Update();
		}
		dbRec111.MoveNext();
	}
// 157154 add <--

// 157347 del -->
	//// 科目行を画面に挿入する
	//// チェックOFF
	//if(mfcRecSub.RequeryFormSeq( ID_FORMNO_021 ) == DB_ERR_OK)	{
	//	sort1 = mfcRecSub.m_Sort1;
	//	mfcRecSub.Fin();
	//	pView = ((CUCHIWAKEApp*)AfxGetApp())->UchiwakeCreateFormViewObject(ID_FORMNO_021);
	//	// 初期設定
	//	pView->CmnSetInitialUchiwakeInfo(ID_FORMNO_021);		// 共通データ
	//	pView->virSetInitialValue();							// 帳表固有データ
	//	// uc_lst_item_sortを参照して必要情報を取得
	//	pView->CmnUcLstItemSortGetData2(ID_FORMNO_021,sort1, &uItemSort);
	//	// ソートダイアログを開いていないまたは第１ソートキーが科目の場合
	//	if(sort1 == 0 || uItemSort.bytFgItem == ID_FGITEM_KAMOKU) {
	//		if(b021 == TRUE)	{
	//			// 小計が最終行で頁の最後の場合、空白行を挿入する
	//			pView->CmnTblUpdatePageRowAllResetMain(1);
	//			// データ行が最終行で頁の最後の場合、小計NULLを挿入する
	//			pView->CmnTblUpdatePageRowAllResetMain(3);
	//			// 頁と行の調整処理
	//			pView->CmnTblDeleteFgFunc(ID_FGFUNC_PAGEKEI);			// 頁計の削除
	//			pView->CmnTblDeleteFgFunc(ID_FGFUNC_RUIKEI);			// 累計の削除

	//			// 行番号を振り直すための基準となる行数を取得
	//			nBaseRow = pView->CmnTblCountMaxRow();

	//			// 並び順で科目行を明細行の先頭にするため、明細データ、一括金額（手動）、一括金額（自動）のFgFuncを変更
	//			strSQL = _T("update uc_021_uketoritegata set fgfunc = 4 where fgfunc = 1 \r\n");
	//			strSQL += _T("update uc_021_uketoritegata set fgfunc = 5 where fgfunc = 21 \r\n");
	//			strSQL += _T("update uc_021_uketoritegata set fgfunc = 6 where fgfunc = 22 ");
	//			pdb->ExecuteSQL(strSQL);

	//			// 科目行を挿入
	//			strSQL = _T("DECLARE @seq int \r\n");
	//			strSQL += _T("SET @seq = (select max(seq) from uc_021_uketoritegata where fgshow = 0) \r\n");
	//			strSQL += _T("DECLARE cur CURSOR FOR SELECT NumPage, NumRow, FgFunc, KnSeq, KnOrder, KnName, KnKana \r\n");
	//			strSQL += _T("FROM uc_021_uketoritegata \r\n");
	//			strSQL += _T("where fgshow = 0 \r\n");
	//			strSQL += _T("ORDER BY Numpage, NumRow \r\n");
	//			strSQL += _T("DECLARE @page int DECLARE @row int DECLARE @fgfunc int DECLARE @old_knorder int DECLARE @knseq int DECLARE @knorder int DECLARE @knname varchar(30) DECLARE @knkana varchar(6) \r\n");
	//			strSQL += _T("SET @old_knorder = -1 \r\n");
	//			strSQL += _T("OPEN cur FETCH cur INTO @page, @row, @fgfunc, @knseq, @knorder, @knname, @knkana WHILE @@fetch_status = 0 BEGIN \r\n");
	//			strSQL += _T("IF (@knorder <> @old_knorder) AND ((@fgfunc = 4) OR (@fgfunc = 5) OR (@fgfunc = 6)) BEGIN \r\n");
	//			strSQL += _T("SET @seq = @seq + 1 \r\n");
	//			strSQL += _T("INSERT INTO uc_021_uketoritegata VALUES(@seq,@page,@row,3,0,@knname,0,NULL,0,0,0,0,NULL,NULL,0,0,0,0,NULL,NULL,NULL,0,0,NULL,NULL,NULL,0,0,0,NULL,NULL,NULL) \r\n");
	//			strSQL += _T("SET @old_knorder = @knorder \r\n");
	//			strSQL += _T("END  \r\n");
	//			strSQL += _T("FETCH cur INTO @page, @row, @fgfunc, @knseq, @knorder, @knname, @knkana \r\n");
	//			strSQL += _T("END CLOSE cur DEALLOCATE cur ");
	//			pdb->ExecuteSQL(strSQL);

	//			// 頁、行を振り直す
	//			strSQL.Format(_T("declare @per_page_num int set @per_page_num = %d"),nBaseRow);
	//			strSQL += _T(";with rownum_tbl(seq,NumRow) as ( SELECT seq, ROW_NUMBER() OVER ( ORDER BY NumPage,NumRow,FgFunc ASC ) FROM uc_021_uketoritegata where fgshow = 0), \r\n");
	//			strSQL += _T("rowpagenum_tbl(seq,NumRow,NumPage) as ( SELECT seq, NumRow , CAST(( CEILING( CAST( NumRow AS FLOAT ) / CAST( @per_page_num AS FLOAT ) ) ) AS smallint ) FROM rownum_tbl ), \r\n");
	//			strSQL += _T("new_rowpagenum_tbl(seq,NumRow,NumPage) as( SELECT seq, ROW_NUMBER()OVER(PARTITION BY NumPage ORDER BY NumRow), NumPage FROM rowpagenum_tbl ) \r\n");
	//			strSQL += _T("update uc_021_uketoritegata \r\n");
	//			strSQL += _T("set uc_021_uketoritegata.numpage = new_rowpagenum_tbl.numpage,uc_021_uketoritegata.numrow = new_rowpagenum_tbl.numrow \r\n");
	//			strSQL += _T("from uc_021_uketoritegata,new_rowpagenum_tbl \r\n");
	//			strSQL += _T("where uc_021_uketoritegata.seq = new_rowpagenum_tbl.seq and uc_021_uketoritegata.fgshow = 0 ");
	//			pdb->ExecuteSQL(strSQL);

	//			// 変更した、明細データ、一括金額（手動）、一括金額（自動）のFgFuncを元に戻す
	//			strSQL = _T("update uc_021_uketoritegata set fgfunc = 1 where fgfunc = 4 \r\n");
	//			strSQL += _T("update uc_021_uketoritegata set fgfunc = 21 where fgfunc = 5 \r\n");
	//			strSQL += _T("update uc_021_uketoritegata set fgfunc = 22 where fgfunc = 6 ");
	//			pdb->ExecuteSQL(strSQL);

	//			// 最終頁を調整する
	//			pView->CmnTblUpdateEndPageSetting();
	//			// 累計、頁計を追加
	//			pView->CmnTblInsertPageKeiRuiKei();
	//			// 計の金額を再集計
	//			pView->CmnTblCalcKei();

	//			// 先頭行の空白を削除する
	//			pView->CmnTblUpdatePageRowAllResetMain(2);
	//			// 先頭行の小計NULLを削除する
	//			pView->CmnTblUpdatePageRowAllResetMain(4);

	//			// 小計行の科目名称を削除
	//			// 小計をSQLで検索する
	//			strSQL = _T("SELECT * FROM uc_021_uketoritegata WHERE FgFunc = 10");
	//			rsData = pView->virGetDataTableObject();
	//			rsData->RequeryFgFunc(ID_FGFUNC_SYOKEI,0);
	//			while(!rsData->IsEOF()) {
	//				// 小計から科目名を削除して取得する
	//				pView->DeleteKamokuKeiStr(rsData->m_KeiStr,&strKei);
	//				// 科目名称を削除した小計名称で更新する
	//				rsData->Edit();
	//				rsData->m_KeiStr = strKei;
	//				rsData->Update();
	//				rsData->MoveNext();
	//			}
	//		}
	//		else {
	//			// 小計行に科目名称を追加
	//			syokeimap.RemoveAll();
	//			// 小計をSQLで検索、配列にSeq番号をセット
	//			strSQL = _T("SELECT * FROM uc_021_uketoritegata WHERE FgFunc = 10");
	//			rsData = pView->virGetDataTableObject();
	//			rsData->RequeryFgFunc(ID_FGFUNC_SYOKEI,0);
	//			ii=0;
	//			while(!rsData->IsEOF()) {
	//				// 小計名称の入力がある
	//				if(rsData->m_KeiStr.IsEmpty() == FALSE) {
	//					temp = rsData->m_KeiStr;
	//					// １文字目が "(" の場合、"(" と ")" を削除する
	//					if(temp.Left(2) == _T("("))	{
	//						temp.Delete(0,1);
	//						temp.Delete(temp.GetLength() - 1,1);
	//					}
	//					// "(" と ")" の位置を検索する
	//					nLParen = temp.Find(_T("("));
	//					nRParen = temp.Find(_T(")"));
	//					// 存在しない場合、科目名称なしとして処理を行う
	//					if(nLParen == -1 || nRParen == -1)	{
	//						// 科目名称を追加する小計の情報をセット
	//						ZeroMemory(&syokeidata, sizeof(SYOKEI_DATA));
	//						syokeidata.seq = rsData->m_Seq;
	//						syokeidata.numpage = rsData->m_NumPage;
	//						syokeidata.numrow = rsData->m_NumRow;
	//						strcpy_s(syokeidata.keistr,64,rsData->m_KeiStr);
	//						syokeimap.SetAt(ii,syokeidata);
	//						ii++;
	//					}
	//				}
	//				rsData->MoveNext();
	//			}
	//			max = syokeimap.GetCount();
	//			for(ii=0;ii<max;ii++) {
	//				ZeroMemory(&syokeidata, sizeof(SYOKEI_DATA));
	//				syokeimap.Lookup(ii,syokeidata);
	//				// 小計に科目名を追加して取得する
	//				strKei = pView->GetSyokeiName(ID_FORMNO_021,syokeidata.numpage,syokeidata.numrow,syokeidata.keistr);
	//				// 変更した小計名称をデータベースに書き込み
	//				strSQL.Format(_T("UPDATE uc_021_uketoritegata SET KeiStr = '%s' WHERE NumPage = %d AND NumRow = %d"),strKei,syokeidata.numpage,syokeidata.numrow);
	//				pdb->ExecuteSQL(strSQL);
	//			}
	//		}
	//	}
	//	delete pView;
	//	pView = NULL;
	//}
	//if(mfcRecSub.RequeryFormSeq( ID_FORMNO_111 ) == DB_ERR_OK)	{
	//	sort1 = mfcRecSub.m_Sort1;
	//	mfcRecSub.Fin();
	//	pView = ((CUCHIWAKEApp*)AfxGetApp())->UchiwakeCreateFormViewObject(ID_FORMNO_111);
	//	// 初期設定
	//	pView->CmnSetInitialUchiwakeInfo(ID_FORMNO_111);		// 共通データ
	//	pView->virSetInitialValue();							// 帳表固有データ
	//	// uc_lst_item_sortを参照して必要情報を取得
	//	pView->CmnUcLstItemSortGetData2(ID_FORMNO_111,sort1, &uItemSort);
	//	// ソートダイアログを開いていないまたは第１ソートキーが科目の場合
	//	if(sort1 == 0 || uItemSort.bytFgItem == ID_FGITEM_KAMOKU) {
	//		if(b111 == TRUE)	{
	//			// 小計が最終行で頁の最後の場合、空白行を挿入する
	//			pView->CmnTblUpdatePageRowAllResetMain(1);
	//			// データ行が最終行で頁の最後の場合、小計NULLを挿入する
	//			pView->CmnTblUpdatePageRowAllResetMain(3);
	//			// 頁と行の調整処理
	//			pView->CmnTblDeleteFgFunc(ID_FGFUNC_PAGEKEI);			// 頁計の削除
	//			pView->CmnTblDeleteFgFunc(ID_FGFUNC_RUIKEI);			// 累計の削除

	//			// 行番号を振り直すための基準となる行数を取得
	//			nBaseRow = pView->CmnTblCountMaxRow();

	//			// 並び順で科目行を明細行の先頭にするため、明細データ、一括金額（手動）、一括金額（自動）のFgFuncを変更
	//			strSQL = _T("update uc_111_kariirekin set fgfunc = 4 where fgfunc = 1 \r\n");
	//			strSQL += _T("update uc_111_kariirekin set fgfunc = 5 where fgfunc = 21 \r\n");
	//			strSQL += _T("update uc_111_kariirekin set fgfunc = 6 where fgfunc = 22 ");
	//			pdb->ExecuteSQL(strSQL);

	//			// 科目行を挿入
	//			strSQL = _T("DECLARE @seq int \r\n");
	//			strSQL += _T("SET @seq = (select max(seq) from uc_111_kariirekin where fgshow = 0) \r\n");
	//			strSQL += _T("DECLARE cur CURSOR FOR SELECT NumPage, NumRow, FgFunc, KnSeq, KnOrder, KnName, KnKana \r\n");
	//			strSQL += _T("FROM uc_111_kariirekin \r\n");
	//			strSQL += _T("where fgshow = 0 \r\n");
	//			strSQL += _T("ORDER BY Numpage, NumRow \r\n");
	//			strSQL += _T("DECLARE @page int DECLARE @row int DECLARE @fgfunc int DECLARE @old_knorder int DECLARE @knseq int DECLARE @knorder int DECLARE @knname varchar(30) DECLARE @knkana varchar(6) \r\n");
	//			strSQL += _T("SET @old_knorder = -1 \r\n");
	//			strSQL += _T("OPEN cur FETCH cur INTO @page, @row, @fgfunc, @knseq, @knorder, @knname, @knkana WHILE @@fetch_status = 0 BEGIN \r\n");
	//			strSQL += _T("IF (@knorder <> @old_knorder) AND ((@fgfunc = 4) OR (@fgfunc = 5) OR (@fgfunc = 6)) BEGIN \r\n");
	//			strSQL += _T("SET @seq = @seq + 1 \r\n");
	//			strSQL += _T("INSERT INTO uc_111_kariirekin VALUES(@seq,@page,@row,3,0,@knname,0,NULL,0,0,0,0,NULL,0,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,NULL,NULL,NULL) \r\n");
	//			strSQL += _T("SET @old_knorder = @knorder \r\n");
	//			strSQL += _T("END  \r\n");
	//			strSQL += _T("FETCH cur INTO @page, @row, @fgfunc, @knseq, @knorder, @knname, @knkana \r\n");
	//			strSQL += _T("END CLOSE cur DEALLOCATE cur ");
	//			pdb->ExecuteSQL(strSQL);

	//			// 頁、行を振り直す
	//			strSQL.Format(_T("declare @per_page_num int set @per_page_num = %d"),nBaseRow);
	//			strSQL += _T(";with rownum_tbl(seq,NumRow) as ( SELECT seq, ROW_NUMBER() OVER ( ORDER BY NumPage,NumRow,FgFunc ASC ) FROM uc_111_kariirekin where fgshow = 0), \r\n");
	//			strSQL += _T("rowpagenum_tbl(seq,NumRow,NumPage) as ( SELECT seq, NumRow , CAST(( CEILING( CAST( NumRow AS FLOAT ) / CAST( @per_page_num AS FLOAT ) ) ) AS smallint ) FROM rownum_tbl ), \r\n");
	//			strSQL += _T("new_rowpagenum_tbl(seq,NumRow,NumPage) as( SELECT seq, ROW_NUMBER()OVER(PARTITION BY NumPage ORDER BY NumRow), NumPage FROM rowpagenum_tbl ) \r\n");
	//			strSQL += _T("update uc_111_kariirekin \r\n");
	//			strSQL += _T("set uc_111_kariirekin.numpage = new_rowpagenum_tbl.numpage,uc_111_kariirekin.numrow = new_rowpagenum_tbl.numrow \r\n");
	//			strSQL += _T("from uc_111_kariirekin,new_rowpagenum_tbl \r\n");
	//			strSQL += _T("where uc_111_kariirekin.seq = new_rowpagenum_tbl.seq and uc_111_kariirekin.fgshow = 0 ");
	//			pdb->ExecuteSQL(strSQL);

	//			// 変更した、明細データ、一括金額（手動）、一括金額（自動）のFgFuncを元に戻す
	//			strSQL = _T("update uc_111_kariirekin set fgfunc = 1 where fgfunc = 4 \r\n");
	//			strSQL += _T("update uc_111_kariirekin set fgfunc = 21 where fgfunc = 5 \r\n");
	//			strSQL += _T("update uc_111_kariirekin set fgfunc = 22 where fgfunc = 6 ");
	//			pdb->ExecuteSQL(strSQL);

	//			// 最終頁を調整する
	//			pView->CmnTblUpdateEndPageSetting();
	//			// 累計、頁計を追加
	//			pView->CmnTblInsertPageKeiRuiKei();
	//			// 計の金額を再集計
	//			pView->CmnTblCalcKei();

	//			// 先頭行の空白を削除する
	//			pView->CmnTblUpdatePageRowAllResetMain(2);
	//			// 先頭行の小計NULLを削除する
	//			pView->CmnTblUpdatePageRowAllResetMain(4);

	//			// 小計行の科目名称を削除
	//			// 小計をSQLで検索する
	//			strSQL = _T("SELECT * FROM uc_111_kariirekin WHERE FgFunc = 10");
	//			rsData = pView->virGetDataTableObject();
	//			rsData->RequeryFgFunc(ID_FGFUNC_SYOKEI,0);
	//			while(!rsData->IsEOF()) {
	//				// 小計から科目名を削除して取得する
	//				pView->DeleteKamokuKeiStr(rsData->m_KeiStr,&strKei);
	//				// 科目名称を削除した小計名称で更新する
	//				rsData->Edit();
	//				rsData->m_KeiStr = strKei;
	//				rsData->Update();
	//				rsData->MoveNext();
	//			}
	//		}
	//		else {
	//			// 小計行に科目名称を追加
	//			syokeimap.RemoveAll();
	//			// 小計をSQLで検索、配列にSeq番号をセット
	//			strSQL = _T("SELECT * FROM uc_111_kariirekin WHERE FgFunc = 10");
	//			rsData = pView->virGetDataTableObject();
	//			rsData->RequeryFgFunc(ID_FGFUNC_SYOKEI,0);
	//			ii=0;
	//			while(!rsData->IsEOF()) {
	//				// 小計名称の入力がある
	//				if(rsData->m_KeiStr.IsEmpty() == FALSE) {
	//					temp = rsData->m_KeiStr;
	//					// １文字目が "(" の場合、"(" と ")" を削除する
	//					if(temp.Left(2) == _T("("))	{
	//						temp.Delete(0,1);
	//						temp.Delete(temp.GetLength() - 1,1);
	//					}
	//					// "(" と ")" の位置を検索する
	//					nLParen = temp.Find(_T("("));
	//					nRParen = temp.Find(_T(")"));
	//					// 存在しない場合、科目名称なしとして処理を行う
	//					if(nLParen == -1 || nRParen == -1)	{
	//						// 科目名称を追加する小計の情報をセット
	//						ZeroMemory(&syokeidata, sizeof(SYOKEI_DATA));
	//						syokeidata.seq = rsData->m_Seq;
	//						syokeidata.numpage = rsData->m_NumPage;
	//						syokeidata.numrow = rsData->m_NumRow;
	//						strcpy_s(syokeidata.keistr,64,rsData->m_KeiStr);
	//						syokeimap.SetAt(ii,syokeidata);
	//						ii++;
	//					}
	//				}
	//				rsData->MoveNext();
	//			}
	//			max = syokeimap.GetCount();
	//			for(ii=0;ii<max;ii++) {
	//				ZeroMemory(&syokeidata, sizeof(SYOKEI_DATA));
	//				syokeimap.Lookup(ii,syokeidata);
	//				// 小計に科目名を追加して取得する
	//				strKei = pView->GetSyokeiName(ID_FORMNO_111,syokeidata.numpage,syokeidata.numrow,syokeidata.keistr);
	//				// 変更した小計名称をデータベースに書き込み
	//				strSQL.Format(_T("UPDATE uc_111_kariirekin SET KeiStr = '%s' WHERE NumPage = %d AND NumRow = %d"),strKei,syokeidata.numpage,syokeidata.numrow);
	//				pdb->ExecuteSQL(strSQL);
	//			}
	//		}
	//	}
	//	delete pView;
	//	pView = NULL;
	//}
// 157347 del <--

// midori 157042 del -->
	//// [ソート]-[科目行を挿入する]のチェックを更新する
	//if(b021 == TRUE)	{
	//	if(mfcRecSub.RequeryFormSeq( ID_FORMNO_021 ) == DB_ERR_OK)	{
	//		pView = ((CUCHIWAKEApp*)AfxGetApp())->UchiwakeCreateFormViewObject(ID_FORMNO_021);

	//		CCtrlSubGeneralVar cs( mfcRecSub );
	//		mfcRecSub.Edit();
	//		cs.bOutKamokuRow = FALSE;
	//		// ソートの第1ソートキーが科目の場合、[ソート]-[科目行を挿入する]のチェックをONにする
	//		// uc_lst_item_sortを参照して必要情報を取得
	//		pView->CmnUcLstItemSortGetData2(ID_FORMNO_021,mfcRecSub.m_Sort1, &uItemSort);
	//		// ソートダイアログを開いていないまたは第１ソートキーが科目の場合
	//		if(mfcRecSub.m_Sort1 == 0 || uItemSort.bytFgItem == ID_FGITEM_KAMOKU) {
	//			cs.bKamokuTitle = TRUE;
	//		}
	//		cs.Set( mfcRecSub );
	//		// 科目指定を行うにチェックが有る場合、[科目指定]-[科目行を自動挿入する]のチェックをONにする
	//		// 科目指定を行うチェックON
	//		if(mfcRecSub.m_GeneralVar[1] & BIT_D3) {
	//			// [科目指定]-[科目行を自動挿入する]チェックON
	//			mfcRecSub.m_GeneralVar[2] |= BIT_D3;
	//		}
	//		//	更新
	//		mfcRecSub.Update();
	//		// 閉じる
	//		mfcRecSub.Fin();

	//		delete pView;
	//		pView = NULL;
	//	}
	//}
	//if(b111 == TRUE)	{
	//	if(mfcRecSub.RequeryFormSeq( ID_FORMNO_111 ) == DB_ERR_OK)	{
	//		pView = ((CUCHIWAKEApp*)AfxGetApp())->UchiwakeCreateFormViewObject(ID_FORMNO_111);

	//		CCtrlSubGeneralVar cs( mfcRecSub );
	//		mfcRecSub.Edit();
	//		cs.bOutKamokuRow = FALSE;
	//		// ソートの第1ソートキーが科目の場合、[ソート]-[科目行を挿入する]のチェックをONにする
	//		// uc_lst_item_sortを参照して必要情報を取得
	//		pView->CmnUcLstItemSortGetData2(ID_FORMNO_111,mfcRecSub.m_Sort1, &uItemSort);
	//		// ソートダイアログを開いていないまたは第１ソートキーが科目の場合
	//		if(mfcRecSub.m_Sort1 == 0 || uItemSort.bytFgItem == ID_FGITEM_KAMOKU) {
	//			cs.bKamokuTitle = TRUE;
	//		}
	//		cs.Set( mfcRecSub );
	//		// 科目指定を行うにチェックが有る場合、[科目指定]-[科目行を自動挿入する]のチェックをONにする
	//		// 科目指定を行うチェックON
	//		if(mfcRecSub.m_GeneralVar[1] & BIT_D3) {
	//			// [科目指定]-[科目行を自動挿入する]チェックON
	//			mfcRecSub.m_GeneralVar[2] |= BIT_D3;
	//		}
	//		//	更新
	//		mfcRecSub.Update();
	//		// 閉じる
	//		mfcRecSub.Fin();

	//		delete pView;
	//		pView = NULL;
	//	}
	//}
// midori 157042 del <--
// midori 157100 add -->
	// [ソート]-[科目行を挿入する]のチェックを更新する
	if(b021 == TRUE)	{
		if(mfcRecSub.RequeryFormSeq( ID_FORMNO_021 ) == DB_ERR_OK)	{
			pView = ((CUCHIWAKEApp*)AfxGetApp())->UchiwakeCreateFormViewObject(ID_FORMNO_021);

// 157347 del -->
			//CCtrlSubGeneralVar cs( mfcRecSub );
// 157347 del <--
			mfcRecSub.Edit();
// 157347 del -->
//// midori 157099,157119 add -->
//			// ソートの第1ソートキーが科目の場合、[ソート]-[科目行を挿入する]のチェックをONにする
//			// uc_lst_item_sortを参照して必要情報を取得
//			pView->CmnUcLstItemSortGetData2(ID_FORMNO_021,mfcRecSub.m_Sort1, &uItemSort);
//			// ソートダイアログを開いていないまたは第１ソートキーが科目の場合
//			if(mfcRecSub.m_Sort1 == 0 || uItemSort.bytFgItem == ID_FGITEM_KAMOKU) {
//				cs.bKamokuTitle = TRUE;
//			}
//			cs.Set( mfcRecSub );
//// midori 157099,157119 add <--
// 157347 del <--
			// 科目指定を行うにチェックが有る場合、[科目指定]-[科目行を自動挿入する]のチェックをONにする
			// 科目指定を行うチェックON
			if(mfcRecSub.m_GeneralVar[1] & BIT_D3) {
				// [科目指定]-[科目行を自動挿入する]チェックON
				mfcRecSub.m_GeneralVar[2] |= BIT_D3;
			}
			//	更新
			mfcRecSub.Update();
			// 閉じる
			mfcRecSub.Fin();

			delete pView;
			pView = NULL;
		}
	}

	if(b111 == TRUE)	{
		if(mfcRecSub.RequeryFormSeq( ID_FORMNO_111 ) == DB_ERR_OK)	{
			pView = ((CUCHIWAKEApp*)AfxGetApp())->UchiwakeCreateFormViewObject(ID_FORMNO_111);

// 157347 del -->
//// midori 157099,157119 add -->
//			CCtrlSubGeneralVar cs( mfcRecSub );
//// midori 157099,157119 add <--
// 157347 del <--
			mfcRecSub.Edit();
// 157347 del -->
//// midori 157099,157119 add -->
//			// ソートの第1ソートキーが科目の場合、[ソート]-[科目行を挿入する]のチェックをONにする
//			// uc_lst_item_sortを参照して必要情報を取得
//			pView->CmnUcLstItemSortGetData2(ID_FORMNO_111,mfcRecSub.m_Sort1, &uItemSort);
//			// ソートダイアログを開いていないまたは第１ソートキーが科目の場合
//			if(mfcRecSub.m_Sort1 == 0 || uItemSort.bytFgItem == ID_FGITEM_KAMOKU) {
//				cs.bKamokuTitle = TRUE;
//			}
//			cs.Set( mfcRecSub );
//// midori 157099,157119 add <--
// 157347 del <--
			// 科目指定を行うにチェックが有る場合、[科目指定]-[科目行を自動挿入する]のチェックをONにする
			// 科目指定を行うチェックON
			if(mfcRecSub.m_GeneralVar[1] & BIT_D3) {
				// [科目指定]-[科目行を自動挿入する]チェックON
				mfcRecSub.m_GeneralVar[2] |= BIT_D3;
			}
			//	更新
			mfcRecSub.Update();
			// 閉じる
			mfcRecSub.Fin();

			delete pView;
			pView = NULL;
		}
	}
// midori 157100 add <--
}
// midori 156188 add <--

// midori 156189,156190,156191 add -->
// ------------------------------------------------------------------------------
//	「科目行を挿入する」 有効/無効のサインを変更する
//	条件	①事業期間の期首が平成31年4月1日以降
//			②過年度確定で無い
//
//	引数	CDatabase*
//			int		0:過年度確定サインのチェック無し
//					1:過年度確定サインのチェック有り
//			int		様式番号
//			
// ------------------------------------------------------------------------------
// 改良No.21-0086,21-0529 cor -->
//int KamokuRowEnableSgn(CDatabase* pDB,int pSw)
// ------------------------------
int KamokuRowEnableSgn(CDatabase* pDB, int pSw, int nFormSeq)
// 改良No.21-0086,21-0529 cor <--
{
	CdbUcInfMain	mfcRecMain( pDB );	//	「uc_inf_main」テーブル用クラス
	int				ret=0;
// midori 190505 add -->
	CRecordset		rs( pDB );
	CString			strSQL=_T("");
	CString			strData=_T("");

	// uc_inf_mainテーブルが作成されているかチェック
	strSQL = _T("select object_id('uc_inf_main'),'u'");
	rs.Open(CRecordset::forwardOnly,strSQL);
	rs.GetFieldValue((short)0,strData);
	rs.Close();
	if(strData.IsEmpty() == TRUE)	{
		return(ret);
	}
// midori 190505 add <--

	if(mfcRecMain.Init() == DB_ERR_OK)	{
		if(!(mfcRecMain.IsEOF()))	{
			mfcRecMain.MoveFirst();
			// (mfcRecMain.m_GeneralVar[2] & BIT_D1)	チェック有り：簡素化後の様式
			// bG_KansoLock == TRUE						簡素化前⇔簡素化後の変更不可
// 改良No.21-0086,21-0529 cor -->
			//if((mfcRecMain.m_GeneralVar[2] & BIT_D1) && bG_KansoLock == TRUE)	{
			//	ret = 1;
			//}
// ------------------------------
			if(nFormSeq == ID_FORMNO_081)	{
				if((mfcRecMain.m_GeneralVar[2] & BIT_D1) && bG_KansoLock == TRUE && bG_InvNoLock == TRUE)	{
					ret = 1;
				}
			}
			else {
				if((mfcRecMain.m_GeneralVar[2] & BIT_D1) && bG_KansoLock == TRUE) {
					ret = 1;
				}
			}
// 改良No.21-0086,21-0529 cor <--
			if(pSw == 1) {
				// 過年度確定
				if((mfcRecMain.m_GeneralVar[1] & BIT_D2)) {
					ret = 0;
				}
			}
		}
	}
	mfcRecMain.Fin();

	return(ret);
}
// midori 156189,156190,156191 add <--

// midori 190505 add -->
// --------------------------------------------------------------------------------------------------------------------
//	「確認ダイアログを表示して、出力方法の再選択を行う様式」
//	上記の設定を確認する条件に当てはまるかどうか確認
//	①印刷設定ダイアログ「金額０円データを出力しない」「金額空欄データを出力しない」のどちらかにチェックが付いている
//	②データが入力されている
//	③「科目指定を行う」にチェックが付いていない
// --------------------------------------------------------------------------------------------------------------------
int ChkIkkatuSort(CDatabase* pDB, CdbUcInfSub* pfcRec)
{
	int						ret=0;
// midori 20/10/08u del -->
	//int						sw1=0,sw2=0,sw3=0,sw4=0;
// midori 20/10/08u del <--
// midori 20/10/08u add -->
	int						sw1=0,sw2=0,sw3=0,sw4=0,sw5=0;
	int						sort1=0;
// midori 20/10/08u add <--
	int						chk1=0,chk2=0;
	CCtrlSubGeneralVar		cs(*pfcRec);
	CUcFunctionCommon		ufc;
// midori 20/10/08u add -->
	CdbUcLstItemSort		mfcItemSort(pDB);		//	uc_lst_item_sortテーブルクラス
// midori 20/10/08u add <--

// midori 20/10/08_2 add -->
	// 確認ダイアログが非表示の場合、処理不要
	if(prnConfimationSw == FALSE) {
		return(ret);
	}
// midori 20/10/08_2 add <--

	ret = 0;
	sw1 = 0;
	sw2 = 0;
	sw3 = 0;
	sw4 = 0;

	// ⑭－２は対象外なのでリターン
	if(pfcRec->m_FormSeq == ID_FORMNO_142)	{
		return(ret);
	}

	// 新しい期間マスターの場合のみチェックをする
// 改良No.21-0086,21-0529 cor -->
	//if(KamokuRowEnableSgn(pDB, 0) != 0)	{
// ------------------------------
	if(KamokuRowEnableSgn(pDB, 0, pfcRec->m_FormSeq) != 0)	{
// 改良No.21-0086,21-0529 cor <--
		sw1 = 1;

		// 「金額０円データを出力しない」「金額空欄データを出力しない」
		// どちらかにチェックが付いている
		if(cs.bPrintZeroData || cs.bPrintNullData)	sw2 = 1;

		// 金額が0円もしくは空白以外のデータが入力されている かつ
		// 「金額０円データを出力しない」にチェックが付いている場合は、金額が0円のデータが入力されている
		// 「金額空欄データを出力しない」にチェックが付いている場合は、金額が空欄のデータが入力されている
		// 両方チェックが付いている場合は、上記どちらかのデータが入力されている
		CdbUc000Common*		rsData = ufc.CreateDbObject(pfcRec->m_FormSeq, pDB);
		if(rsData != NULL)	{
			chk1 = chk2 = 0;
			if(cs.bPrintZeroData)	chk1 = 1;
			if(cs.bPrintNullData)	chk2 = 1;
// midori 157001 del -->
			//if(rsData->GetCountDataRecord2(pfcRec->m_FormSeq, chk1, chk2) == 1)	{
			//	sw3 = 1;
			//}
// midori 157001 del <--
// midori 157001 add -->
			sw3 = rsData->GetCountDataRecord2(pfcRec->m_FormSeq, chk1, chk2);
// midori 157001 add <--
		}

		// 「科目指定を行う」にチェックが付いている
		if(pfcRec->m_GeneralVar[1] & BIT_D3)	sw4 = 1;
		else									sw4 = 0;

// midori 20/10/08u del -->
		//// 該当条件に当てはまるか
		//if(sw1 == 1 && sw2 == 1 && sw3 == 1 && sw4 == 0)	{
		//	ret = 1;
		//}
// midori 20/10/08u del <--
// midori 20/10/08u add -->
		// 編集－ソート設定の改頁を行うが有効の時のみ
		sw5 = 0;
		sort1 = pfcRec->m_Sort1;
		if(pfcRec->m_Sort1 == 0)	{
			// 一度も編集～ソートを開いていない場合は、第１ソート項目の項目番号が書かれていない
			if(pfcRec->m_FormSeq == 2)			sort1=8;
			else								sort1=1;
		}
		if(mfcItemSort.RequeryFormItem(pfcRec->m_FormSeq,sort1) == DB_ERR_OK)	{
			if(!mfcItemSort.IsEOF())	{
				if(mfcItemSort.m_FgPage != 0) {
					sw5 = pfcRec->m_OpKaiPage;
				}
			}
		}
		// 該当条件に当てはまるか
		if(sw1 == 1 && sw2 == 1 && sw3 == 1 && sw4 == 0 && sw5 == 1)	{
			ret = 1;
		}
// midori 20/10/08u add <--

	}

	return(ret);
}
// midori 190505 add <--

// 20-0450 add -->
// ------------------------------------------------------------------------------------------------
// ⑥有価証券の内訳書　ソート項目の「種類・銘柄」を分割または結合する
// ------------------------------------------------------------------------------------------------
//	【引数】	int			0:オプションからの呼び出し　1:起動時の呼び出し
//				BOOL		第１引数が０の場合のみ有効
//							FALSE:新様式→旧様式		TRUE:旧様式→新様式
//	【戻値】	-1:エラー	0:正常
// ------------------------------------------------------------------------------------------------
int CUCHIWAKEApp::SyuruiMeigaraConvert( int pSw, BOOL pSel )
{
	int					sw=0;
	CdbUcInfMain		mfcMain(m_pZmSub->m_database);
	CdbUcLstItemSort	sort(m_pZmSub->m_database);

	// 起動時に呼び出された場合
	if(pSw != 0)	{
		// uc_inf_main::GeneralVar[4] & BIT_D0(0x01) がONになっていれば処理を行わない
		if(mfcMain.Init() == DB_ERR_OK)	{
			if(mfcMain.m_GeneralVar[4] & BIT_D0)	sw=1;
			mfcMain.Fin();
			if(sw != 0)	{
				return(0);
			}
		}
	}
	// 起動時に呼び出された場合、旧様式であれば何もしない
	if(pSw == 0 || (pSw != 0 && pSel != FALSE))	{
		if(sort.RequeryForm(ID_FORMNO_061) == DB_ERR_OK)	{
			if(!(sort.IsEOF()))	{
				// 旧様式 → 新様式
				if(pSel == TRUE)	{
					sort.MoveLast();
					while(!sort.IsBOF())	{
						if(sort.m_ItemSeq >= 2)	{
							sort.Edit();
							if(sort.m_ItemSeq == 2)	{
								sort.m_ItemName = _T("銘柄");
// 158012 del -->
								////sort.m_OrderStr = _T("RIGHT(Syurui,LEN(Syurui)-CHARINDEX(NCHAR(13),Syurui)-1)");
								//sort.m_OrderStr = _T("case CHARINDEX(NCHAR(13),Syurui) when 0 then NULL");
								//sort.m_OrderStr+= _T(" else RIGHT(Syurui,LEN(Syurui)-CHARINDEX(NCHAR(13),Syurui)-1) end");
// 158012 del <--
// 158012 add -->
								sort.m_OrderStr = _T("RIGHT(Syurui,CASE CHARINDEX(NCHAR(13),Syurui)-1 WHEN -1 THEN LEN(Syurui)");
								sort.m_OrderStr+= _T(" ELSE LEN(Syurui)-CHARINDEX(NCHAR(13),Syurui)-1 END)");
// 158012 add <--
							}
							sort.m_ItemSeq = sort.m_ItemSeq + 1;
							sort.Update();
						}
						sort.MovePrev();
					}
					sort.AddNew();
					sort.m_FormSeq		= 7;
					sort.m_ItemSeq		= 2;
					sort.m_FgPage		= 0;
					sort.m_FgSykei		= 1;
					sort.m_FgItem		= 0;
					sort.m_ItemName		= _T("種類");
// 158012 del -->
					//sort.m_OrderStr		= _T("LEFT(Syurui,CHARINDEX(NCHAR(13),Syurui)-1)");
// 158012 del <--
// 158012 add -->
					sort.m_OrderStr = _T("LEFT(Syurui,CASE CHARINDEX(NCHAR(13),Syurui)-1 WHEN -1 THEN LEN(Syurui)");
					sort.m_OrderStr+= _T(" ELSE CHARINDEX(NCHAR(13),Syurui)-1 END)");
// 158012 add <--
					sort.m_FieldSykei	= _T("Syurui");
					sort.m_FieldSykei2	= _T("");
					sort.m_OrderStr2	= _T("");
					sort.Update();
				}
				// 新様式 → 旧様式
				else	{
					sort.MoveFirst();
					while(!sort.IsEOF())	{
						if(sort.m_ItemSeq == 2)	{
							sort.Delete();
						}
						else if(sort.m_ItemSeq >= 3)	{
							sort.Edit();
							if(sort.m_ItemSeq == 3)	{
								sort.m_ItemName	= _T("種類・銘柄");
								sort.m_OrderStr = _T("Syurui COLLATE Japanese_CS_AS_KS_WS ASC");
							}
							sort.m_ItemSeq = sort.m_ItemSeq - 1;
							sort.Update();
						}
						sort.MoveNext();
					}
				}
// 158058 add -->
				CdbUcInfSub		infsub(m_pZmSub->m_database);

				if(infsub.RequeryFormSeq(ID_FORMNO_061) == DB_ERR_OK)	{
					if(!(infsub.IsEOF()))	{
						if(infsub.m_Sort1 > 2 || infsub.m_Sort2 > 2 || infsub.m_Sort3 > 2)	{
							infsub.Edit();
							if(infsub.m_Sort1 > 2)	{
								if(pSel == TRUE)	infsub.m_Sort1++;	// 旧様式 → 新様式
								else				infsub.m_Sort1--;	// 新様式 → 旧様式
							}
							if(infsub.m_Sort2 > 2)	{
								if(pSel == TRUE)	infsub.m_Sort2++;
								else				infsub.m_Sort2--;
							}
							if(infsub.m_Sort3 > 2)	{
								if(pSel == TRUE)	infsub.m_Sort3++;
								else				infsub.m_Sort3--;
							}
							infsub.Update();
						}
					}
					infsub.Fin();
				}
// 158058 add <--
			}
			sort.Fin();
		}
	}
	// 起動時に呼び出された場合
	if(pSw != 0)	{
		// uc_inf_main::GeneralVar[4] & BIT_D0(0x01) をONにする
		if(mfcMain.Init() == DB_ERR_OK)	{
			mfcMain.Edit();
			mfcMain.m_GeneralVar[4] |= BIT_D0;
			mfcMain.Update();
			mfcMain.Fin();
		}
	}

	return(0);
}
// 20-0450 add <--

// 158138 add -->
BOOL CUCHIWAKEApp::Convert210701( CDatabase* pdb )
{
	CdbUcInfMain					mfcMain(pdb);
	BOOL							regautoref=TRUE;
	int								sw=0;

	// uc_inf_main::GeneralVar[4] & BIT_D1(0x02) がONになっていれば処理を行わない
	if(mfcMain.Init() == DB_ERR_OK)	{
		if(mfcMain.m_GeneralVar[4] & BIT_D1)	sw=1;
		CCtrlMainGeneralVar	cm(mfcMain);
		regautoref = cm.bRegAutoRef;
		mfcMain.Fin();
		if(sw != 0)	{
			return(TRUE);
		}
	}

	typedef struct _CONVERT_REC_	{
		int		AdSeq1;			// 削除するAdSeq
		int		GrSeq;			// 削除するGrSeq
		int		OrderNum;		// 削除するOrderNum（再付番に必要）
		int		AdSeq2;			// 代わりに付番するAdSeq
	}CONV_REC;
	CONV_REC						convrec;
	CMap<int,int,CONV_REC,CONV_REC>	convmap;
	int								ii=0,jj=0,max=0,cnt=0;
	CString							cs=_T(""),tbl=_T("");
	CString							dt[4];
	CRecordset						rs(pdb);
	CdbUcLstAddress					mfcRec(pdb);

	try	{
		// 取引先、金融機関を自動で登録するチェックＯＮ
		if(regautoref == TRUE)	{
			// SQL文の作成（上段が空白で、上段と下段が同じ名称を抽出）
			cs = _T("select a.AdSeq,a.GrSeq,a.OrderNum,b.AdSeq,b.OrderNum from uc_lst_address a");
			cs+= _T(" left join uc_lst_address b on a.GrSeq=b.GrSeq and a.AdName2=b.AdName1");
			cs+= _T(" where (a.AdName1='' OR a.AdName1 IS NULL) and (b.AdSeq IS NOT NULL)");
			// 抽出したデータをマップにロード
			rs.Open(CRecordset::forwardOnly,cs);
			ii=0;
			while(!rs.IsEOF())	{
				rs.GetFieldValue((short)0,dt[0]);
				rs.GetFieldValue((short)1,dt[1]);
				rs.GetFieldValue((short)2,dt[2]);
				rs.GetFieldValue((short)3,dt[3]);
				convrec.AdSeq1		= _tstoi(dt[0]);
				convrec.GrSeq		= _tstoi(dt[1]);
				convrec.OrderNum	= _tstoi(dt[2]);
				convrec.AdSeq2		= _tstoi(dt[3]);
				convmap.SetAt(ii,convrec);
				ii++;
				rs.MoveNext();
			}
			rs.Close();
			// マップからデータを読み込む
			max = (int)convmap.GetCount();
			for(ii=0; ii<max; ii++)	{
				convmap.Lookup(ii,convrec);
				// 下段に上段と同じ名称を持つ取引先レコードを削除する
				if(mfcRec.RequeryAdSeq(convrec.GrSeq,convrec.AdSeq1) == DB_ERR_OK)	{
					if(!(mfcRec.IsEOF()))	{
						mfcRec.MoveFirst();
						mfcRec.Delete();
					}
					mfcRec.Fin();
					// 再付番
					cnt = 0;
					if(mfcRec.Init(convrec.GrSeq) == DB_ERR_OK)	{
						cnt = mfcRec.GetRecordCount();
						mfcRec.Fin();
					}
					if(cnt > 0)	{
						if(mfcRec.ReNumberOrder(convrec.GrSeq,convrec.OrderNum,-1) == DB_ERR_OK)	{
							mfcRec.Fin();
						}
					}
				}
				// 各様式のAdSeqを書き換える
				for(jj=0; jj<9; jj++)	{
					switch(jj)	{
						case	0:	tbl=_T("uc_021_uketoritegata");	break;	// ②受取手形
						case	1:	tbl=_T("uc_031_urikakekin");	break;	// ③売掛金
						case	2:	tbl=_T("uc_041_karibarai");		break;	// ④－１仮払金
						case	3:	tbl=_T("uc_042_kasituke");		break;	// ④－２貸付金
						case	4:	tbl=_T("uc_081_siharaitegata");	break;	// ⑧支払手形
						case	5:	tbl=_T("uc_091_kaikakekin");	break;	// ⑨買掛金
						case	6:	tbl=_T("uc_101_kariukekin");	break;	// ⑩－１仮受金
						case	7:	tbl=_T("uc_161_zatueki");		break;	// ⑯－１雑益
						case	8:	tbl=_T("uc_162_zatuson");		break;	// ⑯－２雑損失
					}
					// ①AdSeqのみを書き換える
					cs.Format(_T("update %s set AdSeq=%d"),tbl,convrec.AdSeq2);
					cs.Format(cs + _T(" where AdSeq=%d"),convrec.AdSeq1);
					pdb->ExecuteSQL(cs);
					// ②書き換えた後のAdSeqと同じマスターの情報に書き換える
					cs.Format(_T("update %s"),tbl);
					cs+=_T(" set AdOrder=lst.OrderNum");
					cs+=_T(",AdName1=lst.AdName1");
					cs+=_T(",AdName2=lst.AdName2");
					cs+=_T(",AdKana=lst.AdKana");
					cs.Format(cs + _T(" from %s AS data JOIN uc_lst_address AS lst"),tbl);
					cs+=_T(" ON data.AdSeq = lst.AdSeq");
					pdb->ExecuteSQL(cs);
				}
			}
		}
		// 取引先、金融機関を自動で登録するチェックＯＦＦ
		else	{
			// 各様式でAdName1=''、AdName2!=''になっているデータを書き換える
			for(jj=0; jj<9; jj++)	{
				switch(jj)	{
					case	0:	tbl=_T("uc_021_uketoritegata");	break;	// ②受取手形
					case	1:	tbl=_T("uc_031_urikakekin");	break;	// ③売掛金
					case	2:	tbl=_T("uc_041_karibarai");		break;	// ④－１仮払金
					case	3:	tbl=_T("uc_042_kasituke");		break;	// ④－２貸付金
					case	4:	tbl=_T("uc_081_siharaitegata");	break;	// ⑧支払手形
					case	5:	tbl=_T("uc_091_kaikakekin");	break;	// ⑨買掛金
					case	6:	tbl=_T("uc_101_kariukekin");	break;	// ⑩－１仮受金
					case	7:	tbl=_T("uc_161_zatueki");		break;	// ⑯－１雑益
					case	8:	tbl=_T("uc_162_zatuson");		break;	// ⑯－２雑損失
				}
				cs.Format(_T("update %s"),tbl);
				cs+=_T(" set AdName1=AdName2,AdName2=NULL");
				cs+=_T(" where (AdName1 = '' or AdName1 IS NULL)");
				cs+=_T(" and (AdName2 != '' and AdName2 IS NOT NULL)");
				pdb->ExecuteSQL(cs);
			}
		}
	}
	catch(CDBException *e)	{
		ICSMessageBox(e->m_strError);
		e->Delete();
		return(FALSE);
	}

	// コンバート処理が終わったので、uc_inf_main::GeneralVar[4] & BIT_D1(0x02) をONにする
	if(mfcMain.Init() == DB_ERR_OK)	{
		mfcMain.Edit();
		mfcMain.m_GeneralVar[4] |= BIT_D1;
		mfcMain.Update();
		mfcMain.Fin();
	}

	return(TRUE);
}
// 158138 add <--

// 22-0003_220324 add -->
// ------------------------------------------------------------------------------------------------
// ある特定の条件下で【小計】の金額がおかしくなったマスターに対して一度だけ修復処理を行う
// ------------------------------------------------------------------------------------------------
//	【引数】	CDatabase*	データベースクラス
//	【戻値】	BOOL		TRUE:正常	FALSE:エラー
// ------------------------------------------------------------------------------------------------
BOOL CUCHIWAKEApp::Convert220324( CDatabase* pdb )
{
	CdbUcInfMain		mfcMain(pdb);
	CfrmUc000Common*	pView;
	int					sw=0;
	int					ii=0;

	// uc_inf_main::GeneralVar[4] & BIT_D3(0x08)がONになっていれば処理を行わない
	if(mfcMain.Init() == DB_ERR_OK)	{
		if(mfcMain.m_GeneralVar[4] & BIT_D3)	sw=1;
		mfcMain.Fin();
		if(sw != 0)	{
			return(TRUE);
		}
	}

	// すべての様式に対して小計、中計、頁計、累計の合計を求める処理を行う
	for(ii=ID_FORMNO_011; ii<=ID_FORMNO_162; ii++)	{
		pView = UchiwakeCreateFormViewObject(ii);
		// 初期設定
		pView->CmnSetInitialUchiwakeInfo(ii);				// 共通データ
		pView->virSetInitialValue();						// 帳表固有データ
		pView->CmnTblCalcKei();
		delete pView;
		pView = NULL;
	}
	for(ii=0;ii<20;ii++)	{
		// ⑰
		pView = UchiwakeCreateFormViewObject(ID_FORMNO_171+ii);
		// 初期設定
		pView->CmnSetInitialUchiwakeInfo(ID_FORMNO_171+ii);	// 共通データ
		pView->virSetInitialValue();						// 帳表固有データ
		pView->CmnTblCalcKei();
		delete pView;
		pView = NULL;
		// ⑱
		pView = UchiwakeCreateFormViewObject(ID_FORMNO_181+ii);
		// 初期設定
		pView->CmnSetInitialUchiwakeInfo(ID_FORMNO_181+ii);	// 共通データ
		pView->virSetInitialValue();						// 帳表固有データ
		pView->CmnTblCalcKei();
		delete pView;
		pView = NULL;
	}

	// コンバート処理が終わったので、uc_inf_main::GeneralVar[4] & BIT_D3(0x08)をONにする
	if(mfcMain.Init() == DB_ERR_OK)	{
		mfcMain.Edit();
		mfcMain.m_GeneralVar[4] |= BIT_D3;
		mfcMain.Update();
		mfcMain.Fin();
	}

	return(TRUE);
}
// 22-0003_220324 add <--


// インボイス登録番号追加対応_23/10/23 add -->
// ----------------------------------------------------------------------------------------------------------
//	インボイス登録番号カラム拡張
// ----------------------------------------------------------------------------------------------------------
int CUCHIWAKEApp::InvNoColumnConvert(CDatabase* pdb)
{
	int					sw=0;
	int					ii=0;
	CString				strSQL;
	CString				sData;
	CdbUcInfMain		mfcMain(pdb);
	CRecordset			rs(m_pZmSub->m_database);
	CdbUc000Common*		rsData;

	//※要通知の件があったのでしばらくコメント
	//// uc_inf_main::GeneralVar[4] & BIT_D6(0x40)がONになっていれば処理を行わない
	//if(mfcMain.Init() == DB_ERR_OK)	{
	//	if(mfcMain.m_GeneralVar[4] & BIT_D6)	sw = 1;
	//	mfcMain.Fin();
	//	if(sw != 0)	{
	//		return(0);
	//	}
	//}

	// 各様式にインボイス登録番号カラムを追加
// 要通知修正_22_24/01/24 del -->
	//// uc_021_uketoritegata (Table update)
	//strSQL  = _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_021_uketoritegata'),'Invno','PRECISION')) is null \r\n");
	//strSQL += _T("ALTER TABLE uc_021_uketoritegata add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");

	//// uc_031_urikakekin (Table update)
	//strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_031_urikakekin'),'Invno','PRECISION')) is null \r\n");
	//strSQL += _T("ALTER TABLE uc_031_urikakekin add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");

	//// uc_041_karibarai (Table update)
	//strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_041_karibarai'),'Invno','PRECISION')) is null \r\n");
	//strSQL += _T("ALTER TABLE uc_041_karibarai add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");

	//// uc_042_kasituke (Table update)
	//strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_042_kasituke'),'Invno','PRECISION')) is null \r\n");
	//strSQL += _T("ALTER TABLE uc_042_kasituke add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");

	//// uc_071_koteishisan (Table update)
	//strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_071_koteishisan'),'Invno','PRECISION')) is null \r\n");
	//strSQL += _T("ALTER TABLE uc_071_koteishisan add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");

	//// uc_081_siharaitegata (Table update)
	//strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_081_siharaitegata'),'Invno','PRECISION')) is null \r\n");
	//strSQL += _T("ALTER TABLE uc_081_siharaitegata add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");

	//// uc_091_kaikakekin (Table update)
	//strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_091_kaikakekin'),'Invno','PRECISION')) is null \r\n");
	//strSQL += _T("ALTER TABLE uc_091_kaikakekin add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");

	//// uc_101_kariukekin (Table update)
	//strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_101_kariukekin'),'Invno','PRECISION')) is null \r\n");
	//strSQL += _T("ALTER TABLE uc_101_kariukekin add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");

	//// uc_121_totiuriage (Table update)
	//strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_121_totiuriage'),'Invno','PRECISION')) is null \r\n");
	//strSQL += _T("ALTER TABLE uc_121_totiuriage add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");

	//// uc_151_tidaiyatin (Table update)
	//strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_151_tidaiyatin'),'Invno','PRECISION')) is null \r\n");
	//strSQL += _T("ALTER TABLE uc_151_tidaiyatin add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");

	//// uc_152_kenrikin (Table update)
	//strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_152_kenrikin'),'Invno','PRECISION')) is null \r\n");
	//strSQL += _T("ALTER TABLE uc_152_kenrikin add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");

	//// uc_153_kougyou (Table update)
	//strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_153_kougyou'),'Invno','PRECISION')) is null \r\n");
	//strSQL += _T("ALTER TABLE uc_153_kougyou add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");

	//// uc_161_zatueki (Table update)
	//strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_161_zatueki'),'Invno','PRECISION')) is null \r\n");
	//strSQL += _T("ALTER TABLE uc_161_zatueki add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");

	//// uc_162_zatuson (Table update)
	//strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_162_zatuson'),'Invno','PRECISION')) is null \r\n");
	//strSQL += _T("ALTER TABLE uc_162_zatuson add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");

	//// uc_171_sonota1 (Table update)
	//strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_171_sonota1'),'Invno','PRECISION')) is null \r\n");
	//strSQL += _T("ALTER TABLE uc_171_sonota1 add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");

	//// uc_lst_addressにインボイス登録番号カラムを追加
	//strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_lst_address'),'Invno','PRECISION')) is null \r\n");
	//strSQL += _T("ALTER TABLE uc_lst_address add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");

	//// uc_ren_darec_tempにインボイス登録番号カラムを追加
	//strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_ren_darec_temp'),'Invno','PRECISION')) is null \r\n");
	//strSQL += _T("ALTER TABLE uc_ren_darec_temp add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
// 要通知修正_22_24/01/24 del <--
// 要通知修正_22_24/01/24 add -->
	strSQL.Empty();

	// uc_021_uketoritegata (Table update)
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_021_uketoritegata]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_021_uketoritegata'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_021_uketoritegata add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");

	// uc_031_urikakekin (Table update)
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_031_urikakekin]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_031_urikakekin'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_031_urikakekin add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");

	// uc_041_karibarai (Table update)
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_041_karibarai]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_041_karibarai'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_041_karibarai add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");

	// uc_042_kasituke (Table update)
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_042_kasituke]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_042_kasituke'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_042_kasituke add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");

	// uc_071_koteishisan (Table update)
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_071_koteishisan]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_071_koteishisan'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_071_koteishisan add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");

	// uc_081_siharaitegata (Table update)
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_081_siharaitegata]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_081_siharaitegata'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_081_siharaitegata add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");

	// uc_091_kaikakekin (Table update)
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_091_kaikakekin]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_091_kaikakekin'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_091_kaikakekin add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");

	// uc_101_kariukekin (Table update)
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_101_kariukekin]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_101_kariukekin'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_101_kariukekin add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");

	// uc_121_totiuriage (Table update)
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_121_totiuriage]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_121_totiuriage'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_121_totiuriage add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");

	// uc_151_tidaiyatin (Table update)
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_151_tidaiyatin]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_151_tidaiyatin'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_151_tidaiyatin add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");

	// uc_152_kenrikin (Table update)
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_152_kenrikin]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_152_kenrikin'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_152_kenrikin add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");

	// uc_153_kougyou (Table update)
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_153_kougyou]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_153_kougyou'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_153_kougyou add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");

	// uc_161_zatueki (Table update)
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_161_zatueki]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_161_zatueki'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_161_zatueki add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");

	// uc_162_zatuson (Table update)
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_162_zatuson]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_162_zatuson'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_162_zatuson add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");

	// uc_171_sonota1 (Table update)
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_171_sonota1]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_171_sonota1'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_171_sonota1 add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");

	// uc_lst_addressにインボイス登録番号カラムを追加
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_lst_address]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_lst_address'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_lst_address add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");

	// uc_ren_darec_tempにインボイス登録番号カラムを追加
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_ren_darec_temp]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_ren_darec_temp'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_ren_darec_temp add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");
// 要通知修正_22_24/01/24 add <--

	try	{
		m_pZmSub->m_database->ExecuteSQL(strSQL);
	}
	catch(CDBException* dbe)	{
		ICSMessageBox(dbe->m_strError);
		dbe->Delete();
		return(-1);
	}

	// -------------------------

	// uc_lst_item_sort 各様式のソートに"登録番号"を追加
	strSQL.Format(_T("SELECT count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 2 AND ItemSeq = 9"));
	try	{
		rs.Open(CRecordset::forwardOnly, strSQL);
		rs.GetFieldValue((short)0, sData);
		rs.Close();
	}
	//catch(...)	{
	//	return(-1);
	//}
	catch(CDBException* dbe)	{
		ICSMessageBox(dbe->m_strError);
		dbe->Delete();
		return(-1);
	}

	if(_tstoi(sData) == 0) {
		strSQL.Empty();

		// 様式２
		strSQL  = _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		//strSQL += _T("VALUES(2, 9, '登録番号(法人番号)', 1, 1, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");		// 修正No.168394 del
		strSQL += _T("VALUES(2, 9, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");		// 修正No.168394 add

		// 様式３
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		//strSQL += _T("VALUES(3, 5, '登録番号(法人番号)', 0, 1, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");		// 修正No.168394 del
		strSQL += _T("VALUES(3, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");		// 修正No.168394 add

		// 様式４－１
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		//strSQL += _T("VALUES(4, 5, '登録番号(法人番号)', 0, 1, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");		// 修正No.168394 del
		strSQL += _T("VALUES(4, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");		// 修正No.168394 add

		// 様式４－２
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		//strSQL += _T("VALUES(5, 4, '登録番号(法人番号)', 0, 1, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");		// 修正No.168394 del
		strSQL += _T("VALUES(5, 4, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");		// 修正No.168394 add

		// 様式７
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(8, 7, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");

		// 様式８
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		//strSQL += _T("VALUES(9, 8, '登録番号(法人番号)', 1, 1, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");		// 修正No.168394 del
		strSQL += _T("VALUES(9, 8, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");		// 修正No.168394 add

		// 様式９
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		//strSQL += _T("VALUES(10, 5, '登録番号(法人番号)', 0, 1, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");	// 修正No.168394 del
		strSQL += _T("VALUES(10, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");		// 修正No.168394 add

		// 様式１０－１
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		//strSQL += _T("VALUES(11, 5, '登録番号(法人番号)', 0, 1, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");	// 修正No.168394 del
		strSQL += _T("VALUES(11, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");		// 修正No.168394 add

		// 様式１２
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(14, 8, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");

		// 様式１５－１
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(18, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");

		// 様式１５－２
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(19, 6, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");

		// 様式１５－３
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(20, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");

		// 様式１６－１
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		//strSQL += _T("VALUES(21, 6, '登録番号(法人番号)', 0, 1, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");	// 修正No.168394 del
		strSQL += _T("VALUES(21, 6, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");		// 修正No.168394 add

		// 様式１６－２
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		//strSQL += _T("VALUES(22, 6, '登録番号(法人番号)', 0, 1, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");	// 修正No.168394 del
		strSQL += _T("VALUES(22, 6, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");		// 修正No.168394 add

		// 様式１７－１～２０
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(101, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(102, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(103, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(104, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(105, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(106, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(107, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(108, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(109, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(110, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(111, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(112, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(113, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(114, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(115, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(116, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(117, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(118, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(119, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(120, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");

		try	{
			m_pZmSub->m_database->ExecuteSQL(strSQL);
		}
		catch(CDBException* dbe)	{
			ICSMessageBox(dbe->m_strError);
			dbe->Delete();
			return(-1);
		}
	}

	// コンバート処理が終わったので、uc_inf_main::GeneralVar[4] & BIT_D6(0x40)をONにする
	if(mfcMain.Init() == DB_ERR_OK)	{
		mfcMain.Edit();
		mfcMain.m_GeneralVar[4] |= BIT_D6;
		mfcMain.Update();
		mfcMain.Fin();
	}

	return(0);
}

// ----------------------------------------------------------------------------------------------------------
// 決算期間から簡素化の画面で起動するかどうかを判定する
// ----------------------------------------------------------------------------------------------------------
void CUCHIWAKEApp::InvoNoCheck(void)
{
	int				init=0;
	int				symd=0,eymd=0;
	//bool			invoice=false;		// 改良No.21-0086,21-0529 del
	BOOL			kakutei=FALSE;
	BOOL			bcmp=FALSE;
	BOOL			zupd=FALSE;
	CZmGen8			ZmGen;
	CdbUcInfMain	mfcMain(m_pZmSub->m_database);

	// インボイス登録番号付き様式を使用するかどうか
	if(mfcMain.Init() == DB_ERR_OK)	{
		CCtrlMainGeneralVar cm(mfcMain);
		if(cm.bUseStyle2)	bG_InvNo = TRUE;
		else				bG_InvNo = FALSE;

		if(cm.bDataKakutei)	kakutei = TRUE;
		else				kakutei = FALSE;

		// 前年度からの更新直後
		//if(mfcMain.m_GeneralVar[2] & BIT_D5)	zupd=TRUE;	// 修正No.168500,168509 del
		if(mfcMain.m_GeneralVar[4] & BIT_D7)	zupd=TRUE;	// 修正No.168500,168509 add

		if((mfcMain.m_GeneralVar[4] & BIT_D4) == 0)	{
			init = 1;
			mfcMain.Edit();
			mfcMain.m_GeneralVar[4] |= BIT_D4;
			mfcMain.Update();
		}
		mfcMain.Fin();
	}

	bcmp = bG_InvNo;

	// 決算期間を取得
	m_pZmSub->VolumeOpen();
	symd = m_pZmSub->zvol->ss_ymd;
	eymd = m_pZmSub->zvol->ee_ymd;
	m_pZmSub->VolumeClose();

// 改良No.21-0086,21-0529 cor -->
	//invoice = ZmGen.IsInvoiceVupMaster(m_pZmSub);
// ------------------------------
	m_Invoice = ZmGen.IsInvoiceVupMaster(m_pZmSub);
// 改良No.21-0086,21-0529 cor <--

	// 1.期末が令和6年3月1日より前のマスター、またはインボイス改正マスターでないマスター　は現様式
// 改良No.21-0086,21-0529 cor -->
	////if(eymd < 20240301)	{						// 修正No.168500,168509 del
	//if(eymd < 20240301 || invoice == false)	{	// 修正No.168500,168509 add
// ------------------------------
	if(eymd < 20240301 || m_Invoice == false)	{
// 改良No.21-0086,21-0529 cor <--
		bcmp = FALSE;
		bG_InvNoLock = TRUE;
	}
	// 2.期首が令和6年3月1日以降且つインボイス改正マスターである マスターは新様式
// 改良No.21-0086,21-0529 cor -->
	//else if(symd >= 20240301 && invoice == true)	{
// ------------------------------
	else if(symd >= 20240301 && m_Invoice == true)	{
// 改良No.21-0086,21-0529 cor <--
		bcmp = TRUE;
		bG_InvNoLock = TRUE;
	}
	// 3.上記以外はプログラム内で様式を切り替えられるようにする
	else	{
		if(init != 0)	{
			bcmp = TRUE;
		}
		bG_InvNoLock = FALSE;
	}

	bG_InvnoMessage = FALSE;
	// 1.期間から判定した様式が今まで使用していた様式と異なる または
	// 2.前年度からの更新を行った後で、当年度と過年度の様式が異なる場合 かつ
	// 3.入力確定がＯＦＦである
	if((bcmp != bG_InvNo || zupd == TRUE) && (kakutei == FALSE))	{
		if(mfcMain.Init() == DB_ERR_OK)	{
			mfcMain.Edit();
			if(bG_InvNo == FALSE)	bG_InvnoMessage = TRUE;

			bG_InvNo = bcmp;
			if(bG_InvNo == TRUE)	mfcMain.m_GeneralVar[4] |= BIT_D5;
			else					mfcMain.m_GeneralVar[4] &= ~BIT_D5;

			//if(zupd == TRUE)		mfcMain.m_GeneralVar[2] &= ~BIT_D5;	// 修正No.168500,168509 del
			if(zupd == TRUE)		mfcMain.m_GeneralVar[4] &= ~BIT_D7;	// 修正No.168500,168509 add

			mfcMain.Update();
			mfcMain.Fin();

			// インボイス登録番号を追加したことにより、様式②と様式⑧の支払銀行、支払銀行支店名に改行マークを差し込む
			BankNameResetConvert();
		}
	}
}

// ----------------------------------------------------------------------------------------------------------
//	インボイス登録番号を追加した様式を使用する場合、様式②と様式⑧の支払銀行、支払支店名に改行マークを差し込む
//	オプションによって切替ができるので、改行マークを差し込む処理と抜く処理を両方記載
// ----------------------------------------------------------------------------------------------------------
BOOL CUCHIWAKEApp::BankNameResetConvert(void)
{
	CdbUc021Uketoritegata	dbRec021(m_pDB);
	CdbUc081Siharaitegata	dbRec081(m_pDB);
	CUcFunctionCommon		ufc;
	int						sw=0;
	CString					cst;
	CString					str1,str2,str3,str4;
	CString					bk1, bk2;

	// 様式②の支払銀行名、支払銀行支店名が5文字を超える場合、改行マークを差し込む
	dbRec021.RequeryFgFunc(ID_FGFUNC_DATA, 0);	// データ行を昇順で抽出
	while(!dbRec021.IsEOF())	{
		str1.Empty();
		str2.Empty();
		str3.Empty();
		str4.Empty();
		bk1.Empty();
		bk2.Empty();
		sw = 0;

		if(bG_InvNo == FALSE)	{	// 新様式 → 旧様式
			// 改行マークが入っていれば削除して取り込む
			// 銀行名
			bk1 = dbRec021.m_BkName1;
			if(ufc.GetKaigyoCount(bk1) > 0)	{
				bk1.Replace(PRT_FORMAT_CHCODE, _T(""));
				sw = 1;
			}
			// 支店
			bk2 = dbRec021.m_BkName2;
			if(ufc.GetKaigyoCount(bk2) > 0)	{
				bk2.Replace(PRT_FORMAT_CHCODE, _T(""));
				sw = 1;
			}
		}
		else	{					// 旧様式 → 新様式
// 修正No.168381 del -->
			//// 銀行名
			//cst = dbRec021.m_BkName1;
			//if(cst.GetLength() > 10)	{
			//	str2 = m_clsFunc.GetSpritString(cst, &str1, 10);
			//	cst = str2;
			//	str3 = m_clsFunc.GetSpritString(cst, &str2, 10);
			//	if(str2.IsEmpty() == FALSE)		bk1 = str1 + PRT_FORMAT_CHCODE + str2;
			//	else							bk1 = str1;

			//	sw = 1;
			//}
			//// 支店名
			//cst = dbRec021.m_BkName2;
			//if(cst.GetLength() > 10) {
			//	str2 = m_clsFunc.GetSpritString(cst, &str1, 10);
			//	cst = str2;
			//	str3 = m_clsFunc.GetSpritString(cst, &str2, 10);
			//	if(str2.IsEmpty() == FALSE)		bk2 = str1 + PRT_FORMAT_CHCODE + str2;
			//	else							bk2 = str1;

			//	sw = 1;
			//}
// 修正No.168381 del <--
// 修正No.168381 add -->
			// 銀行名
			bk1 = dbRec021.m_BkName1;
			if(bk1.GetLength() > 10)	{
				cst = bk1;
				str2 = m_clsFunc.GetSpritString(cst, &str1, 10);
				cst = str2;
				str3 = m_clsFunc.GetSpritString(cst, &str2, 10);
				if(str2.IsEmpty() == FALSE)		bk1 = str1 + PRT_FORMAT_CHCODE + str2;
				else							bk1 = str1;

				sw = 1;
			}
			// 支店名
			bk2 = dbRec021.m_BkName2;
			if(bk2.GetLength() > 10) {
				cst = bk2;
				str2 = m_clsFunc.GetSpritString(cst, &str1, 10);
				cst = str2;
				str3 = m_clsFunc.GetSpritString(cst, &str2, 10);
				if(str2.IsEmpty() == FALSE)		bk2 = str1 + PRT_FORMAT_CHCODE + str2;
				else							bk2 = str1;

				sw = 1;
			}

// 修正No.168381 add <--
		}
		if(sw == 1)	{
			dbRec021.Edit();
			dbRec021.m_BkName1 = bk1;		// 銀行名
			dbRec021.m_BkName2 = bk2;		// 支店名
			dbRec021.Update();
		}
		dbRec021.MoveNext();
	}

	// 様式⑧の支払銀行名、支払銀行支店名が5文字を超える場合、改行マークを差し込む
	dbRec081.RequeryFgFunc(ID_FGFUNC_DATA, 0);	// データ行を昇順で抽出
	while(!dbRec081.IsEOF())	{
		str1.Empty();
		str2.Empty();
		str3.Empty();
		str4.Empty();
		bk1.Empty();
		bk2.Empty();
		sw = 0;

		if(bG_InvNo == FALSE)	{	// 新様式 → 旧様式
			// 改行マークが入っていれば削除して取り込む
			// 銀行名
			bk1 = dbRec081.m_BkName1;
			if(ufc.GetKaigyoCount(bk1) > 0)	{
				bk1.Replace(PRT_FORMAT_CHCODE,_T(""));
				sw = 1;
			}
			// 支店
			bk2 = dbRec081.m_BkName2;
			if(ufc.GetKaigyoCount(bk2) > 0)	{
				bk2.Replace(PRT_FORMAT_CHCODE,_T(""));
				sw = 1;
			}
		}
		else	{					// 旧様式 → 新様式
			// 銀行名
// 修正No.168529 del -->
			//cst = dbRec081.m_BkName1;
			//if(cst.GetLength() > 10) {
			//	str2 = m_clsFunc.GetSpritString(cst, &str1, 10);
			//	cst = str2;
			//	str3 = m_clsFunc.GetSpritString(cst, &str2, 10);
			//	if(str2.IsEmpty() == FALSE)		bk1 = str1 + PRT_FORMAT_CHCODE + str2;
			//	else							bk1 = str1;

			//	sw = 1;
			//}
			//// 支店名
			//cst = dbRec081.m_BkName2;
			//if(cst.GetLength() > 10) {
			//	str2 = m_clsFunc.GetSpritString(cst, &str1, 10);
			//	cst = str2;
			//	str3 = m_clsFunc.GetSpritString(cst, &str2, 10);
			//	if(str2.IsEmpty() == FALSE)		bk2 = str1 + PRT_FORMAT_CHCODE + str2;
			//	else							bk2 = str1;

			//	sw = 1;
			//}
// 修正No.168529 del <--
// 修正No.168529 add -->
			bk1 = dbRec081.m_BkName1;
			if(bk1.GetLength() > 10)	{
				cst = bk1;
				str2 = m_clsFunc.GetSpritString(cst, &str1, 10);
				cst = str2;
				str3 = m_clsFunc.GetSpritString(cst, &str2, 10);
				if(str2.IsEmpty() == FALSE)		bk1 = str1 + PRT_FORMAT_CHCODE + str2;
				else							bk1 = str1;

				sw = 1;
			}
			// 支店名
			bk2 = dbRec081.m_BkName2;
			if(bk2.GetLength() > 10)	{
				cst = bk2;
				str2 = m_clsFunc.GetSpritString(cst, &str1, 10);
				cst = str2;
				str3 = m_clsFunc.GetSpritString(cst, &str2, 10);
				if(str2.IsEmpty() == FALSE)		bk2 = str1 + PRT_FORMAT_CHCODE + str2;
				else							bk2 = str1;

				sw = 1;
			}
// 修正No.168529 add <--
		}
		if(sw == 1)	{
			dbRec081.Edit();
			dbRec081.m_BkName1 = bk1;		// 銀行名
			dbRec081.m_BkName2 = bk2;		// 支店名
			dbRec081.Update();
		}
		dbRec081.MoveNext();
	}

	return(TRUE);
}
// インボイス登録番号追加対応_23/10/23 add <--

// 改良No.21-0086,21-0529 add -->
// ----------------------------------------------------------------------------------------------------------
//	様式⑧支払手形の内訳書の科目入力追加、「電子記録○○」等科目追加に関するカラム拡張
//	※翌期更新用に UCHIIMP.cpp にも KamokuAddColumnConvert3() で同じ内容の関数を作成している
// ----------------------------------------------------------------------------------------------------------
int CUCHIWAKEApp::KamokuAddColumnConvert1(CDatabase* pdb)
{
	int						sw=0;
	CString					strSQL;
	CString					sData;
	CRecordset*				prs;

	// 様式⑧に必要なカラムが追加されているか確認
	strSQL.Empty();
	strSQL = _T("SELECT COLUMNPROPERTY(OBJECT_ID('uc_081_siharaitegata'),'KnSeq','PRECISION') ");
	prs = new CRecordset(m_pZmSub->m_database);
	prs->Open(CRecordset::forwardOnly, strSQL);
	prs->GetFieldValue((short)0, sData);
	sw = _tstoi(sData);
	prs->Close();
	if(sw == 0)	{	// カラムが存在しない
		// 様式⑧に科目に関するカラムを追加
		strSQL.Empty();

		// uc_081_siharaitegata (Table update)
		// KnSeq
		strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_081_siharaitegata]') AND type in (N'U')) begin \r\n");
		strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_081_siharaitegata'),'KnSeq','PRECISION')) is null \r\n");
		strSQL += _T("ALTER TABLE uc_081_siharaitegata add KnSeq int NULL \r\n");
		strSQL += _T("end \r\n");
		// KnOrder
		strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_081_siharaitegata]') AND type in (N'U')) begin \r\n");
		strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_081_siharaitegata'),'KnOrder','PRECISION')) is null \r\n");
		strSQL += _T("ALTER TABLE uc_081_siharaitegata add KnOrder int NULL \r\n");
		strSQL += _T("end \r\n");
		// KnName
		strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_081_siharaitegata]') AND type in (N'U')) begin \r\n");
		strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_081_siharaitegata'),'KnName','PRECISION')) is null \r\n");
		strSQL += _T("ALTER TABLE uc_081_siharaitegata add KnName varchar(30) COLLATE Japanese_CI_AS NULL \r\n");
		strSQL += _T("end \r\n");
		// KnKana
		strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_081_siharaitegata]') AND type in (N'U')) begin \r\n");
		strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_081_siharaitegata'),'KnKana','PRECISION')) is null \r\n");
		strSQL += _T("ALTER TABLE uc_081_siharaitegata add KnKana varchar(6) COLLATE Japanese_CI_AS NULL \r\n");
		strSQL += _T("end \r\n");
		try {
			m_pZmSub->m_database->ExecuteSQL(strSQL);
		}
		catch(CDBException* dbe) {
			ICSMessageBox(dbe->m_strError);
			dbe->Delete();
			return(-1);
		}

		// -------------------------
		strSQL += _T("UPDATE uc_081_siharaitegata SET KnSeq=0, KnOrder=0 \r\n");
		try {
			m_pZmSub->m_database->ExecuteSQL(strSQL);
		}
		catch(CDBException* dbe) {
			ICSMessageBox(dbe->m_strError);
			dbe->Delete();
			return(-1);
		}
	}

	return(0);
}

// ----------------------------------------------------------------------------------------------------------
//	様式⑧支払手形の内訳書の科目入力追加、「電子記録○○」等科目追加に関するデータ追加・変更
// ----------------------------------------------------------------------------------------------------------
int CUCHIWAKEApp::KamokuAddColumnConvert2(CDatabase* pdb)
{
	int						ii=0;
	int						iapno=0x01;
	BOOL					b081=FALSE;
	CString					strSQL=_T("");
	CString					sData=_T("");
	CString					strKei=_T(""), strKei2=_T("");
	CdbUcInfMain			mfcMain(pdb);
	CdbUcInfSub				mfcSub(pdb);
	CdbUc081Siharaitegata	dbRec081(pdb);
	CRecordset				rs(m_pZmSub->m_database);
	CdbUc000Common*			rsData;
	CfrmUc000Common*		pView=NULL;		// FormViewオブジェクト
	CZmGen8					ZmGen;
	EnumIdByoinType			nByoinType=ID_ZMG_NOT_BYOIN_TYPE;

	bG_KmkAdd1 = FALSE;
	bG_KmkAdd2 = FALSE;

	if(m_pZmSub != NULL)	{
		m_pZmSub->VolumeOpen();		// DBボリュームオープン
		iapno = m_pZmSub->zvol->apno;

		// 病院マスターの場合
		if(iapno == 0x30)	{
			// どのタイプの病院マスター？
			nByoinType = ZmGen.CheckByoinType(m_pZmSub);
		}
		m_pZmSub->VolumeClose();	// DBボリュームクローズ
	}

	// uc_inf_main::GeneralVar[5] & BIT_D0(0x01)がONになっていれば処理を行わない
	if(mfcMain.Init() == DB_ERR_OK)	{
		if(mfcMain.m_GeneralVar[5] & BIT_D0)	bG_KmkAdd1 = TRUE;
		mfcMain.Fin();
		
		if(bG_KmkAdd1 == FALSE)	{
			// 様式⑧に科目追加に関するデータ追加
			// -------------------------
			if(KamokuRowEnableSgn(pdb, 0, -1) != 0)	{
				// 計名称（KeiStr）を50Byteから56byteに拡張する
				// ⑧支払手形の内訳書
				pdb->ExecuteSQL(_T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_081_siharaitegata'),'KeiStr','PRECISION')) is not null ALTER TABLE uc_081_siharaitegata ALTER COLUMN KeiStr varchar(56) "));

				// 小計名称が科目名付き、
				// 括弧＋科目名称が大文字12文字を超える場合、改行処理を行う
				dbRec081.RequeryFgFunc(ID_FGFUNC_SYOKEI, 0);
				while(!dbRec081.IsEOF())	{
					// 小計名称の改行を行う
					m_clsFunc.KeiStrSprit(dbRec081.m_KeiStr, &strKei, &strKei2);
					if(strKei2.IsEmpty() == FALSE)	{
						// 小計名を更新する
						dbRec081.Edit();
						dbRec081.m_KeiStr = strKei + _T("\r\n") + strKei2;
						dbRec081.Update();
					}
					dbRec081.MoveNext();
				}

				// -------------------------
				// 科目行を出力するの有効サイン(uc_inf_sub.GeneralConstVal[15] & 0x02)がOFFであればONにする
				if(mfcSub.RequeryFormSeq(ID_FORMNO_081) == DB_ERR_OK)	{
					if(!(mfcSub.IsEOF()))	{
						if(!(mfcSub.m_GeneralConstVal[15] & BIT_D1))	{
							mfcSub.Edit();
							mfcSub.m_GeneralConstVal[15] |= BIT_D1;
							mfcSub.Update();
						}

						// 「科目行を出力する」チェックついてる？
						if(mfcSub.m_GeneralConstVal[15] & BIT_D1)	{
							CCtrlSubGeneralVar	cs(mfcSub);
							if(cs.bOutKamokuRow)	{
								b081 = TRUE;
							}
						}

						// 「科目名タイトル使用フラグ」がOFF？
						if(mfcSub.m_OpTitleKnFg == 0)	{
							mfcSub.Edit();
							mfcSub.m_OpTitleKnFg = 1;
							mfcSub.Update();
						}
					}
					mfcSub.Fin();
				}

				// [ソート]-[科目行を挿入する]のチェックを更新する
				if(b081 == TRUE)	{
					if(mfcSub.RequeryFormSeq(ID_FORMNO_081) == DB_ERR_OK)	{
						pView = ((CUCHIWAKEApp*)AfxGetApp())->UchiwakeCreateFormViewObject(ID_FORMNO_081);
						mfcSub.Edit();
						// 科目指定を行うにチェックが有る場合、[科目指定]-[科目行を自動挿入する]のチェックをONにする
						// 科目指定を行うチェックON
						if(mfcSub.m_GeneralVar[1] & BIT_D3)	{
							// [科目指定]-[科目行を自動挿入する]チェックON
							mfcSub.m_GeneralVar[2] |= BIT_D3;
						}
						// 更新
						mfcSub.Update();
						// 閉じる
						mfcSub.Fin();
						delete pView;
						pView = NULL;
					}
				}

				// -------------------------
				// uc_lst_item_sort 様式⑧のソートに"科目"を追加
				strSQL.Format(_T("SELECT count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 9 AND ItemSeq = 9"));
				try	{
					rs.Open(CRecordset::forwardOnly, strSQL);
					rs.GetFieldValue((short)0, sData);
					rs.Close();
				}
				catch(CDBException* dbe)	{
					ICSMessageBox(dbe->m_strError);
					dbe->Delete();
					if(rs.IsOpen())	{
						rs.Close();
					}
					return(-1);
				}
				if(_tstoi(sData) == 0)	{
					strSQL.Empty();
					// 様式８
					strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
					strSQL += _T("VALUES(9, 9, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC') \r\n");
					try {
						m_pZmSub->m_database->ExecuteSQL(strSQL);
					}
					catch(CDBException* dbe)	{
						ICSMessageBox(dbe->m_strError);
						dbe->Delete();
						return(-1);
					}
				}
			}

			// コンバート処理が終わったので、uc_inf_main::GeneralVar[5] & BIT_D0(0x01)をONにする
			if(mfcMain.Init() == DB_ERR_OK)	{
				mfcMain.Edit();
				mfcMain.m_GeneralVar[5] |= BIT_D0;
				mfcMain.Update();
				mfcMain.Fin();

				bG_KmkAdd1 = TRUE;
			}
		}
	}

	// ====================================================================

// ---------------------------------
//// 確認用。終わったら消す！！
//if(mfcMain.Init() == DB_ERR_OK) {
//	mfcMain.Edit();
//	mfcMain.m_GeneralVar[5] &= ~BIT_D1;
//	mfcMain.Update();
//	mfcMain.Fin();
//}
// ---------------------------------

	// 起動マスターが非営利法人（0x5X）の場合は「電子記録○○」科目は不要のためリターン
	if(isHIEIRI(iapno))	{
		return(0);
	}

	// 「電子記録○○」等の科目を追加したサインが立っているかどうか
	if(mfcMain.Init() == DB_ERR_OK)	{
		if(mfcMain.m_GeneralVar[5] & BIT_D1)	bG_KmkAdd2 = TRUE;
		mfcMain.Fin();
	}
	// 既に「電子記録○○」等の科目を初回追加し終えているマスターの場合はリターン
	if(bG_KmkAdd2 == TRUE)	{
		return(0);
	}

	// sys_zm_verテーブルの kmkver が"2"の時は、「電子記録○○」科目が追加されたマスターになるので、
	// F9科目参照、財務連動科目に「電子記録○○」科目を追加する
	CRecordset		rs2(m_pZmSub->m_sysctl);

	sData.Empty();
	strSQL.Empty();
	strSQL.Format(_T("select kmkver from sys_zm_ver where apno=%d and sysctl_ver=3"), iapno);
	try	{
		rs2.Open(CRecordset::forwardOnly, strSQL);
		if(!rs2.IsEOF()) {
			rs2.GetFieldValue((short)0, sData);
		}
		rs2.Close();
	}
	catch(CDBException* dbe)	{
		ICSMessageBox(dbe->m_strError);
		dbe->Delete();
		if(rs2.IsOpen())	{
			rs2.Close();
		}
		return(-1);
	}
	if(_tstoi(sData) < 2)	{
		return(0);
	}

	//--------------------
	long				addsw[6] = {0};				// 0:②電子記録債権、1:割引電子記録債権、2:②営業(医業/事業)外電子記録債権(※未使用)
													// 3:⑧電子記録債務、4:②営業(医業/事業)外電子記録債務(※未使用)
													// 5:⑯-2電子記録債権売却損
	long				nKnOrder;
	CdbUcLstKamoku		mfcKmkRec(m_pDB);			// uc_lst_kamokuテーブルクラス
	CdbUcRenKamoku		mfcRenRec(m_pDB);			// uc_ren_kamokuテーブルクラス

	// 既存マスター
	if(m_NewMaster == FALSE)	{
		// -------------------------
		// 様式②
		mfcKmkRec.RequeryForm(ID_FORMNO_021);
		while(!mfcKmkRec.IsEOF())	{
			sData = mfcKmkRec.m_KnName;
			sData.Replace("　", "");
			sData.Replace(" ", "");
			sData = sData.Trim();
			if(strcmp(sData, ADDKMK_0211_UCHINAME) == 0)	{		// 電子記録債権
				addsw[0] = mfcKmkRec.m_KnSeq;
			}
			else if(strcmp(sData, ADDKMK_0212_UCHINAME) == 0)	{	// 割引電子記録債権
				addsw[1] = mfcKmkRec.m_KnSeq;
			}
			//else if(((iapno & 0xff) != 0x30) && 
			//		strcmp(sData, ADDKMK_0213_UCHINAME) == 0)	{	// 営業外電子記録債権
			//	addsw[2] = mfcKmkRec.m_KnSeq;
			//}
			//else if(((iapno & 0xff) == 0x30) && (nByoinType < ID_ZMG_H29REV_IRYO_TYPE) && 
			//		strcmp(sData, ADDKMK_0214_UCHINAME) == 0)	{	// 医業外電子記録債権
			//	addsw[2] = mfcKmkRec.m_KnSeq;
			//}
			//else if(((iapno & 0xff) == 0x30) && (nByoinType >= ID_ZMG_H29REV_IRYO_TYPE) && 
			//		strcmp(sData, ADDKMK_0215_UCHINAME) == 0)	{	// 事業外電子記録債権
			//	addsw[2] = mfcKmkRec.m_KnSeq;
			//}
			mfcKmkRec.MoveNext();
		}
		// レコード閉じる
		mfcKmkRec.Fin();

		// 様式⑧
		mfcKmkRec.RequeryForm(ID_FORMNO_081);
		while(!mfcKmkRec.IsEOF())	{
			sData = mfcKmkRec.m_KnName;
			sData.Replace("　", "");
			sData.Replace(" ", "");
			sData = sData.Trim();
			if(strcmp(sData, ADDKMK_0811_UCHINAME) == 0)	{		// 電子記録債務
				addsw[3] = mfcKmkRec.m_KnSeq;
			}
			//else if(((iapno & 0xff) != 0x30) && 
			//		strcmp(sData, ADDKMK_0812_UCHINAME) == 0)	{	// 営業外電子記録債務
			//	addsw[4] = mfcKmkRec.m_KnSeq;
			//}
			//else if(((iapno & 0xff) == 0x30) && (nByoinType < ID_ZMG_H29REV_IRYO_TYPE) && 
			//		strcmp(sData, ADDKMK_0813_UCHINAME) == 0)	{	// 医業外電子記録債務
			//	addsw[4] = mfcKmkRec.m_KnSeq;
			//}
			//else if(((iapno & 0xff) == 0x30) && (nByoinType >= ID_ZMG_H29REV_IRYO_TYPE) && 
			//		strcmp(sData, ADDKMK_0814_UCHINAME) == 0)	{	// 事業外電子記録債務
			//	addsw[4] = mfcKmkRec.m_KnSeq;
			//}
			mfcKmkRec.MoveNext();
		}
		// レコード閉じる
		mfcKmkRec.Fin();

		// 様式⑯－２
		mfcKmkRec.RequeryForm(ID_FORMNO_162);
		while(!mfcKmkRec.IsEOF())	{
			sData = mfcKmkRec.m_KnName;
			sData.Replace("　", "");
			sData.Replace(" ", "");
			sData = sData.Trim();
			if(strcmp(sData, ADDKMK_1621_UCHINAME) == 0)	{
				addsw[5] = mfcKmkRec.m_KnSeq;
			}
			mfcKmkRec.MoveNext();
		}
		// レコード閉じる
		mfcKmkRec.Fin();

		// -------------
		// 様式②「電子記録債権」
		if(addsw[0] == 0)	{
			// uc_lst_kamokuに追加科目のデータを挿入
			sData.Empty();
			strSQL.Format(_T("select max(KnOrder) from uc_lst_kamoku where FormSeq=%d"), ID_FORMNO_021);
			try	{
				// テーブルオープン
				rs.Open(CRecordset::forwardOnly, strSQL);
				if(!rs.IsEOF())	{
					rs.GetFieldValue((short)0, sData);
				}
				rs.Close();
			}
			catch(CDBException *e)	{
				ICSMessageBox(e->m_strError);
				// 開放
				e->Delete();
				if(rs.IsOpen())	{
					rs.Close();
				}
				return(0);
			}
			nKnOrder = _tstoi(sData);
			nKnOrder++;
			try	{
				// 順序番号振り直し成功？
				if(mfcKmkRec.ReNumberOrder(ID_FORMNO_021, nKnOrder, 1) == DB_ERR_OK)	{
					// 新規行追加成功？
					if(mfcKmkRec.CreateNewRecord(ID_FORMNO_021) == DB_ERR_OK)	{
						// レコードあり？
						if(!(mfcKmkRec.IsEOF()))	{
							mfcKmkRec.MoveFirst();						// レコードを先頭に移動
							mfcKmkRec.Edit();
							mfcKmkRec.m_FormSeq	= ID_FORMNO_021;		// 様式シーケンス番号設定
							mfcKmkRec.m_KnOrder	= nKnOrder;				// 順序番号設定
							mfcKmkRec.m_KnCode	= _T("");				// 科目コード設定
							mfcKmkRec.m_KnName	= ADDKMK_0211_UCHINAME;	// 科目名称設定
							mfcKmkRec.m_FgDel	= 0;					// 削除フラグ設定
							mfcKmkRec.m_FgDft	= 0;					// デフォルト値フラグ設定
							mfcKmkRec.m_KnKana	= ADDKMK_0211_UCHIKANA;	// 科目ｶﾅ名設定
							mfcKmkRec.m_PrSign	= 0;					// 科目出力サイン
							mfcKmkRec.Update();

							addsw[0] = mfcKmkRec.m_KnSeq;
						}
						// レコード閉じる
						mfcKmkRec.Fin();
					}
					// レコード閉じる
					mfcKmkRec.Fin();
				}
			}
			catch(...)	{
				// レコード開いてる？
				if(mfcKmkRec.IsOpen())	{
					// レコード閉じる
					mfcKmkRec.Fin();
				}
			}

			// uc_ren_kamokuに追加科目の連動登録を挿入
			sData.Empty();
			strSQL.Format(_T("select KcdStart from uc_ren_kamoku where FormSeq=%d and left(KcdStart,6)='010405' "), ID_FORMNO_021);
			try	{
				// テーブルオープン
				rs.Open(CRecordset::forwardOnly, strSQL);
				if(!rs.IsEOF())	{
					rs.GetFieldValue((short)0, sData);
				}
				rs.Close();
			}
			catch(CDBException *e)	{
				ICSMessageBox(e->m_strError);
				// 開放
				e->Delete();
				if(rs.IsOpen())	{
					rs.Close();
				}
				return(0);
			}
			if(sData.IsEmpty() == TRUE)	{
				// 新規レコード追加
				mfcRenRec.CreateNewRecord(ID_FORMNO_021, iapno);			// レコード初期化済み
				// 特殊行データへ更新
				mfcRenRec.Edit();
				mfcRenRec.m_KcdStart = ADDKMK_0211_KCODE;	// 財務科目コード取得
				mfcRenRec.m_KnSeq = addsw[0];				// 内訳書科目シーケンス保存
				mfcRenRec.Update();
			}
		}

		// 様式②「割引電子記録債権」
		if(addsw[1] == 0)	{
			// uc_lst_kamokuに追加科目のデータを挿入
			sData.Empty();
			strSQL.Format(_T("select max(KnOrder) from uc_lst_kamoku where FormSeq=%d"), ID_FORMNO_021);
			try	{
				// テーブルオープン
				rs.Open(CRecordset::forwardOnly, strSQL);
				if(!rs.IsEOF())	{
					rs.GetFieldValue((short)0, sData);
				}
				rs.Close();
			}
			catch(CDBException *e)	{
				ICSMessageBox(e->m_strError);
				// 開放
				e->Delete();
				if(rs.IsOpen())	{
					rs.Close();
				}
				return(0);
			}
			nKnOrder = _tstoi(sData);
			nKnOrder++;
			try	{
				// 順序番号振り直し成功？
				if(mfcKmkRec.ReNumberOrder(ID_FORMNO_021, nKnOrder, 1) == DB_ERR_OK)	{
					// 新規行追加成功？
					if(mfcKmkRec.CreateNewRecord(ID_FORMNO_021) == DB_ERR_OK)	{
						// レコードあり？
						if(!(mfcKmkRec.IsEOF()))	{
							mfcKmkRec.MoveFirst();						// レコードを先頭に移動
							mfcKmkRec.Edit();
							mfcKmkRec.m_FormSeq	= ID_FORMNO_021;		// 様式シーケンス番号設定
							mfcKmkRec.m_KnOrder	= nKnOrder;				// 順序番号設定
							mfcKmkRec.m_KnCode	= _T("");				// 科目コード設定
							mfcKmkRec.m_KnName	= ADDKMK_0212_UCHINAME;	// 科目名称設定
							mfcKmkRec.m_FgDel	= 0;					// 削除フラグ設定
							mfcKmkRec.m_FgDft	= 0;					// デフォルト値フラグ設定
							mfcKmkRec.m_KnKana	= ADDKMK_0212_UCHIKANA;	// 科目ｶﾅ名設定
							mfcKmkRec.m_PrSign	= 0;					// 科目出力サイン
							mfcKmkRec.Update();

							addsw[1] = mfcKmkRec.m_KnSeq;
						}
						// レコード閉じる
						mfcKmkRec.Fin();
					}
					// レコード閉じる
					mfcKmkRec.Fin();
				}
			}
			catch(...)	{
				// レコード開いてる？
				if(mfcKmkRec.IsOpen())	{
					// レコード閉じる
					mfcKmkRec.Fin();
				}
			}

			// ※割引電子記録債権は財務連動に登録しない
		}

		//// 様式②「営業(医業/事業)外電子記録債権」
		//if(addsw[2] == 0)	{
		//	// uc_lst_kamokuに追加科目のデータを挿入
		//	sData.Empty();
		//	strSQL.Format(_T("select max(KnOrder) from uc_lst_kamoku where FormSeq=%d"), ID_FORMNO_021);
		//	try	{
		//		// テーブルオープン
		//		rs.Open(CRecordset::forwardOnly, strSQL);
		//		if(!rs.IsEOF())	{
		//			rs.GetFieldValue((short)0, sData);
		//		}
		//		rs.Close();
		//	}
		//	catch(CDBException *e)	{
		//		ICSMessageBox(e->m_strError);
		//		// 開放
		//		e->Delete();
		//		if(rs.IsOpen())	{
		//			rs.Close();
		//		}
		//		return(0);
		//	}
		//	nKnOrder = _tstoi(sData);
		//	nKnOrder++;
		//	try	{
		//		// 順序番号振り直し成功？
		//		if(mfcKmkRec.ReNumberOrder(ID_FORMNO_021, nKnOrder, 1) == DB_ERR_OK)	{
		//			// 新規行追加成功？
		//			if(mfcKmkRec.CreateNewRecord(ID_FORMNO_021) == DB_ERR_OK)	{
		//				// レコードあり？
		//				if(!(mfcKmkRec.IsEOF()))	{
		//					mfcKmkRec.MoveFirst();						// レコードを先頭に移動
		//					mfcKmkRec.Edit();
		//					mfcKmkRec.m_FormSeq	= ID_FORMNO_021;		// 様式シーケンス番号設定
		//					mfcKmkRec.m_KnOrder	= nKnOrder;				// 順序番号設定
		//					mfcKmkRec.m_KnCode	= _T("");				// 科目コード設定
		//					if(((iapno & 0xff) != 0x30))	{													// 営業外電子記録債務
		//						mfcKmkRec.m_KnName	= ADDKMK_0213_UCHINAME;	// 科目名称設定
		//						mfcKmkRec.m_KnKana	= ADDKMK_0213_UCHIKANA;	// 科目ｶﾅ名設定
		//					}
		//					else if(((iapno & 0xff) == 0x30) && (nByoinType < ID_ZMG_H29REV_IRYO_TYPE))		{	// 医業外電子記録債務
		//						mfcKmkRec.m_KnName	= ADDKMK_0214_UCHINAME;	// 科目名称設定
		//						mfcKmkRec.m_KnKana	= ADDKMK_0214_UCHIKANA;	// 科目ｶﾅ名設定
		//					}
		//					else if(((iapno & 0xff) == 0x30) && (nByoinType >= ID_ZMG_H29REV_IRYO_TYPE))	{	// 事業外電子記録債務
		//						mfcKmkRec.m_KnName	= ADDKMK_0215_UCHINAME;	// 科目名称設定
		//						mfcKmkRec.m_KnKana	= ADDKMK_0215_UCHIKANA;	// 科目ｶﾅ名設定
		//					}
		//					mfcKmkRec.m_FgDel	= 0;					// 削除フラグ設定
		//					mfcKmkRec.m_FgDft	= 0;					// デフォルト値フラグ設定
		//					mfcKmkRec.m_PrSign	= 0;					// 科目出力サイン
		//					mfcKmkRec.Update();

		//					addsw[2] = mfcKmkRec.m_KnSeq;
		//				}
		//				// レコード閉じる
		//				mfcKmkRec.Fin();
		//			}
		//			// レコード閉じる
		//			mfcKmkRec.Fin();
		//		}
		//	}
		//	catch(...)	{
		//		// レコード開いてる？
		//		if(mfcKmkRec.IsOpen())	{
		//			// レコード閉じる
		//			mfcKmkRec.Fin();
		//		}
		//	}

		//	// uc_ren_kamokuに追加科目の連動登録を挿入
		//	sData.Empty();
		//	strSQL.Format(_T("select KcdStart from uc_ren_kamoku where FormSeq=%d and left(KcdStart,6)='010714' "), ID_FORMNO_021);
		//	try	{
		//		// テーブルオープン
		//		rs.Open(CRecordset::forwardOnly, strSQL);
		//		if(!rs.IsEOF())	{
		//			rs.GetFieldValue((short)0, sData);
		//		}
		//		rs.Close();
		//	}
		//	catch(CDBException *e)	{
		//		ICSMessageBox(e->m_strError);
		//		// 開放
		//		e->Delete();
		//		if(rs.IsOpen())	{
		//			rs.Close();
		//		}
		//		return(0);
		//	}
		//	if(sData.IsEmpty() == TRUE)	{
		//		// 新規レコード追加
		//		mfcRenRec.CreateNewRecord(ID_FORMNO_021, iapno);			// レコード初期化済み
		//		// 特殊行データへ更新
		//		mfcRenRec.Edit();
		//		mfcRenRec.m_KcdStart = ADDKMK_0213_KCODE;	// 財務科目コード取得
		//		mfcRenRec.m_KnSeq = addsw[2];				// 内訳書科目シーケンス保存
		//		mfcRenRec.Update();
		//	}
		//}

		// 様式⑧「電子記録債務」
		if(addsw[3] == 0)	{
			// uc_lst_kamokuに追加科目のデータを挿入
			sData.Empty();
			strSQL.Format(_T("select max(KnOrder) from uc_lst_kamoku where FormSeq=%d"), ID_FORMNO_081);
			try	{
				// テーブルオープン
				rs.Open(CRecordset::forwardOnly, strSQL);
				if(!rs.IsEOF())	{
					rs.GetFieldValue((short)0, sData);
				}
				rs.Close();
			}
			catch(CDBException *e)	{
				ICSMessageBox(e->m_strError);
				// 開放
				e->Delete();
				if(rs.IsOpen())	{
					rs.Close();
				}
				return(0);
			}
			nKnOrder = _tstoi(sData);
			nKnOrder++;
			try	{
				// 順序番号振り直し成功？
				if(mfcKmkRec.ReNumberOrder(ID_FORMNO_081, nKnOrder, 1) == DB_ERR_OK)	{
					// 新規行追加成功？
					if(mfcKmkRec.CreateNewRecord(ID_FORMNO_081) == DB_ERR_OK)	{
						// レコードあり？
						if(!(mfcKmkRec.IsEOF()))	{
							mfcKmkRec.MoveFirst();						// レコードを先頭に移動
							mfcKmkRec.Edit();
							mfcKmkRec.m_FormSeq	= ID_FORMNO_081;		// 様式シーケンス番号設定
							mfcKmkRec.m_KnOrder	= nKnOrder;				// 順序番号設定
							mfcKmkRec.m_KnCode	= _T("");				// 科目コード設定
							mfcKmkRec.m_KnName	= ADDKMK_0811_UCHINAME;	// 科目名称設定
							mfcKmkRec.m_FgDel	= 0;					// 削除フラグ設定
							mfcKmkRec.m_FgDft	= 0;					// デフォルト値フラグ設定
							mfcKmkRec.m_KnKana	= ADDKMK_0811_UCHIKANA;	// 科目ｶﾅ名設定
							mfcKmkRec.m_PrSign	= 0;					// 科目出力サイン
							mfcKmkRec.Update();

							addsw[3] = mfcKmkRec.m_KnSeq;
						}
						// レコード閉じる
						mfcKmkRec.Fin();
					}
					// レコード閉じる
					mfcKmkRec.Fin();
				}
			}
			catch(...)	{
				// レコード開いてる？
				if(mfcKmkRec.IsOpen())	{
					// レコード閉じる
					mfcKmkRec.Fin();
				}
			}

			// uc_ren_kamokuに追加科目の連動登録を挿入
			sData.Empty();
			strSQL.Format(_T("select KcdStart from uc_ren_kamoku where FormSeq=%d and left(KcdStart,6)='040104' "), ID_FORMNO_081);
			try	{
				// テーブルオープン
				rs.Open(CRecordset::forwardOnly, strSQL);
				if(!rs.IsEOF())	{
					rs.GetFieldValue((short)0, sData);
				}
				rs.Close();
			}
			catch(CDBException *e)	{
				ICSMessageBox(e->m_strError);
				// 開放
				e->Delete();
				if(rs.IsOpen())	{
					rs.Close();
				}
				return(0);
			}
			if(sData.IsEmpty() == TRUE)	{
				// 新規レコード追加
				mfcRenRec.CreateNewRecord(ID_FORMNO_081, iapno);			// レコード初期化済み
				// 特殊行データへ更新
				mfcRenRec.Edit();
				mfcRenRec.m_KcdStart = ADDKMK_0811_KCODE;	// 財務科目コード取得
				mfcRenRec.m_KnSeq = addsw[3];				// 内訳書科目シーケンス保存
				mfcRenRec.Update();
			}
		}

		//// 様式⑧「営業(医業/事業)外電子記録債務」
		//if(addsw[4] == 0)	{
		//	// uc_lst_kamokuに追加科目のデータを挿入
		//	sData.Empty();
		//	strSQL.Format(_T("select max(KnOrder) from uc_lst_kamoku where FormSeq=%d"), ID_FORMNO_081);
		//	try	{
		//		// テーブルオープン
		//		rs.Open(CRecordset::forwardOnly, strSQL);
		//		if(!rs.IsEOF())	{
		//			rs.GetFieldValue((short)0, sData);
		//		}
		//		rs.Close();
		//	}
		//	catch(CDBException *e)	{
		//		ICSMessageBox(e->m_strError);
		//		// 開放
		//		e->Delete();
		//		if(rs.IsOpen())	{
		//			rs.Close();
		//		}
		//		return(0);
		//	}
		//	nKnOrder = _tstoi(sData);
		//	nKnOrder++;
		//	try	{
		//		// 順序番号振り直し成功？
		//		if(mfcKmkRec.ReNumberOrder(ID_FORMNO_081, nKnOrder, 1) == DB_ERR_OK)	{
		//			// 新規行追加成功？
		//			if(mfcKmkRec.CreateNewRecord(ID_FORMNO_081) == DB_ERR_OK)	{
		//				// レコードあり？
		//				if(!(mfcKmkRec.IsEOF()))	{
		//					mfcKmkRec.MoveFirst();						// レコードを先頭に移動
		//					mfcKmkRec.Edit();
		//					mfcKmkRec.m_FormSeq	= ID_FORMNO_081;		// 様式シーケンス番号設定
		//					mfcKmkRec.m_KnOrder	= nKnOrder;				// 順序番号設定
		//					mfcKmkRec.m_KnCode	= _T("");				// 科目コード設定
		//					if(((iapno & 0xff) != 0x30))	{													// 営業外電子記録債務
		//						mfcKmkRec.m_KnName	= ADDKMK_0812_UCHINAME;	// 科目名称設定
		//						mfcKmkRec.m_KnKana	= ADDKMK_0812_UCHIKANA;	// 科目ｶﾅ名設定
		//					}
		//					else if(((iapno & 0xff) == 0x30) && (nByoinType < ID_ZMG_H29REV_IRYO_TYPE))		{	// 医業外電子記録債務
		//						mfcKmkRec.m_KnName	= ADDKMK_0813_UCHINAME;	// 科目名称設定
		//						mfcKmkRec.m_KnKana	= ADDKMK_0813_UCHIKANA;	// 科目ｶﾅ名設定
		//					}
		//					else if(((iapno & 0xff) == 0x30) && (nByoinType >= ID_ZMG_H29REV_IRYO_TYPE))	{	// 事業外電子記録債務
		//						mfcKmkRec.m_KnName	= ADDKMK_0814_UCHINAME;	// 科目名称設定
		//						mfcKmkRec.m_KnKana	= ADDKMK_0814_UCHIKANA;	// 科目ｶﾅ名設定
		//					}
		//					mfcKmkRec.m_FgDel	= 0;					// 削除フラグ設定
		//					mfcKmkRec.m_FgDft	= 0;					// デフォルト値フラグ設定
		//					mfcKmkRec.m_PrSign	= 0;					// 科目出力サイン
		//					mfcKmkRec.Update();

		//					addsw[4] = mfcKmkRec.m_KnSeq;
		//				}
		//				// レコード閉じる
		//				mfcKmkRec.Fin();
		//			}
		//			// レコード閉じる
		//			mfcKmkRec.Fin();
		//		}
		//	}
		//	catch(...)	{
		//		// レコード開いてる？
		//		if(mfcKmkRec.IsOpen())	{
		//			// レコード閉じる
		//			mfcKmkRec.Fin();
		//		}
		//	}

		//	// uc_ren_kamokuに追加科目の連動登録を挿入
		//	sData.Empty();
		//	strSQL.Format(_T("select KcdStart from uc_ren_kamoku where FormSeq=%d and left(KcdStart,6)='04051c' "), ID_FORMNO_081);
		//	try	{
		//		// テーブルオープン
		//		rs.Open(CRecordset::forwardOnly, strSQL);
		//		if(!rs.IsEOF())	{
		//			rs.GetFieldValue((short)0, sData);
		//		}
		//		rs.Close();
		//	}
		//	catch(CDBException *e)	{
		//		ICSMessageBox(e->m_strError);
		//		// 開放
		//		e->Delete();
		//		if(rs.IsOpen())	{
		//			rs.Close();
		//		}
		//		return(0);
		//	}
		//	if(sData.IsEmpty() == TRUE)		{
		//		// 新規レコード追加
		//		mfcRenRec.CreateNewRecord(ID_FORMNO_081, iapno);			// レコード初期化済み
		//		// 特殊行データへ更新
		//		mfcRenRec.Edit();
		//		mfcRenRec.m_KcdStart = ADDKMK_0812_KCODE;	// 財務科目コード取得
		//		mfcRenRec.m_KnSeq = addsw[4];				// 内訳書科目シーケンス保存
		//		mfcRenRec.Update();
		//	}
		//}

		// 様式⑯-2「電子記録債権売却損」
		if(addsw[5] == 0)	{
			// uc_lst_kamokuに追加科目のデータを挿入
			sData.Empty();
			strSQL.Format(_T("select max(KnOrder) from uc_lst_kamoku where FormSeq=%d"), ID_FORMNO_162);
			try	{
				// テーブルオープン
				rs.Open(CRecordset::forwardOnly, strSQL);
				if(!rs.IsEOF())	{
					rs.GetFieldValue((short)0, sData);
				}
				rs.Close();
			}
			catch(CDBException *e)	{
				ICSMessageBox(e->m_strError);
				// 開放
				e->Delete();
				if(rs.IsOpen())	{
					rs.Close();
				}
				return(0);
			}
			nKnOrder = _tstoi(sData);
			nKnOrder++;
			try	{
				// 順序番号振り直し成功？
				if(mfcKmkRec.ReNumberOrder(ID_FORMNO_162, nKnOrder, 1) == DB_ERR_OK)	{
					// 新規行追加成功？
					if(mfcKmkRec.CreateNewRecord(ID_FORMNO_162) == DB_ERR_OK)	{
						// レコードあり？
						if(!(mfcKmkRec.IsEOF()))	{
							mfcKmkRec.MoveFirst();						// レコードを先頭に移動
							mfcKmkRec.Edit();
							mfcKmkRec.m_FormSeq	= ID_FORMNO_162;		// 様式シーケンス番号設定
							mfcKmkRec.m_KnOrder	= nKnOrder;				// 順序番号設定
							mfcKmkRec.m_KnCode	= _T("");				// 科目コード設定
							mfcKmkRec.m_KnName	= ADDKMK_1621_UCHINAME;	// 科目名称設定
							mfcKmkRec.m_FgDel	= 0;					// 削除フラグ設定
							mfcKmkRec.m_FgDft	= 0;					// デフォルト値フラグ設定
							mfcKmkRec.m_KnKana	= ADDKMK_1621_UCHIKANA;	// 科目ｶﾅ名設定
							mfcKmkRec.m_PrSign	= 0;					// 科目出力サイン
							mfcKmkRec.Update();

							addsw[5] = mfcKmkRec.m_KnSeq;
						}
						// レコード閉じる
						mfcKmkRec.Fin();
					}
					// レコード閉じる
					mfcKmkRec.Fin();
				}
			}
			catch(...)	{
				// レコード開いてる？
				if(mfcKmkRec.IsOpen())	{
					// レコード閉じる
					mfcKmkRec.Fin();
				}
			}

			// uc_ren_kamokuに追加科目の連動登録を挿入
			sData.Empty();
			strSQL.Format(_T("select KcdStart from uc_ren_kamoku where FormSeq=%d and left(KcdStart,6)='0b020a' "), ID_FORMNO_162);
			try	{
				// テーブルオープン
				rs.Open(CRecordset::forwardOnly, strSQL);
				if(!rs.IsEOF())	{
					rs.GetFieldValue((short)0, sData);
				}
				rs.Close();
			}
			catch(CDBException *e)	{
				ICSMessageBox(e->m_strError);
				// 開放
				e->Delete();
				if(rs.IsOpen())	{
					rs.Close();
				}
				return(0);
			}
			if(sData.IsEmpty() == TRUE)	{
				// 新規レコード追加
				mfcRenRec.CreateNewRecord(ID_FORMNO_162, iapno);			// レコード初期化済み
				// 特殊行データへ更新
				mfcRenRec.Edit();
				mfcRenRec.m_KcdStart = ADDKMK_1621_KCODE;	// 財務科目コード取得
				mfcRenRec.m_KnSeq = addsw[5];				// 内訳書科目シーケンス保存
				mfcRenRec.Update();
			}
		}
	}
	// 新規マスター
	else	{
		// -------------------------
		// uc_lst_kamokuに追加科目のデータを挿入

		// 様式２ ----------
		strSQL.Empty();
		strSQL += _T("INSERT INTO uc_lst_kamoku(KnSeq,FormSeq,KnOrder,KnCode,KnName,FgDel,FgDft,KnKana,IkkatuVal,PrSign) ");
		strSQL.Format(strSQL + _T("VALUES(1000001,2,3,NULL,'%s',0,0,'%s',1000000,0)  \r\n"), ADDKMK_0211_UCHINAME, ADDKMK_0211_UCHIKANA);
		strSQL += _T("INSERT INTO uc_lst_kamoku(KnSeq,FormSeq,KnOrder,KnCode,KnName,FgDel,FgDft,KnKana,IkkatuVal,PrSign) ");
		strSQL.Format(strSQL + _T("VALUES(1000002,2,4,NULL,'%s',0,0,'%s',1000000,0)  \r\n"), ADDKMK_0212_UCHINAME, ADDKMK_0212_UCHIKANA);
		//strSQL += _T("INSERT INTO uc_lst_kamoku(KnSeq,FormSeq,KnOrder,KnCode,KnName,FgDel,FgDft,KnKana,IkkatuVal,PrSign) ");
		//if((iapno & 0xff) == 0x30)	{	// 業種：病院マスター
		//	if(nByoinType >= ID_ZMG_H29REV_IRYO_TYPE)	{	// 医療法人
		//		strSQL.Format(strSQL + _T("VALUES(1000003,2,5,NULL,'%s',0,0,'%s',1000000,0) \r\n"), ADDKMK_0215_UCHINAME, ADDKMK_0215_UCHIKANA);
		//	}
		//	else	{
		//		strSQL.Format(strSQL + _T("VALUES(1000003,2,5,NULL,'%s',0,0,'%s',1000000,0) \r\n"), ADDKMK_0214_UCHINAME, ADDKMK_0214_UCHIKANA);
		//	}
		//}
		//else	{						// 業種：病院以外のマスター
		//	strSQL.Format(strSQL + _T("VALUES(1000003,2,5,NULL,'%s',0,0,'%s',1000000,0) \r\n"), ADDKMK_0213_UCHINAME, ADDKMK_0213_UCHIKANA);
		//}
		try	{
			m_pZmSub->m_database->ExecuteSQL(strSQL);
		}
		catch(CDBException* dbe)	{
			ICSMessageBox(dbe->m_strError);
			dbe->Delete();
			return(-1);
		}

		// 様式８ ----------
		strSQL.Empty();
		strSQL += _T("INSERT INTO uc_lst_kamoku(KnSeq,FormSeq,KnOrder,KnCode,KnName,FgDel,FgDft,KnKana,IkkatuVal,PrSign) ");
		strSQL.Format(strSQL + _T("VALUES(1000004,9,2,NULL,'%s',0,0,'%s',1000000,0) \r\n"), ADDKMK_0811_UCHINAME, ADDKMK_0811_UCHIKANA);
		//strSQL += _T("INSERT INTO uc_lst_kamoku(KnSeq,FormSeq,KnOrder,KnCode,KnName,FgDel,FgDft,KnKana,IkkatuVal,PrSign) ");
		//if((iapno & 0xff) == 0x30)	{	// 業種：病院マスター
		//	if(nByoinType >= ID_ZMG_H29REV_IRYO_TYPE)	{
		//		strSQL.Format(strSQL + _T("VALUES(1000005,9,3,NULL,'%s',0,0,'%s',1000000,0) \r\n"), ADDKMK_0814_UCHINAME, ADDKMK_0814_UCHIKANA);
		//	}
		//	else	{
		//		strSQL.Format(strSQL + _T("VALUES(1000005,9,3,NULL,'%s',0,0,'%s',1000000,0) \r\n"), ADDKMK_0813_UCHINAME, ADDKMK_0813_UCHIKANA);
		//	}
		//}
		//else	{						// 業種：病院以外のマスター
		//	strSQL.Format(strSQL + _T("VALUES(1000005,9,3,NULL,'%s',0,0,'%s',1000000,0) \r\n"), ADDKMK_0812_UCHINAME, ADDKMK_0812_UCHIKANA);
		//}
		try	{
			m_pZmSub->m_database->ExecuteSQL(strSQL);
		}
		catch(CDBException* dbe)	{
			ICSMessageBox(dbe->m_strError);
			dbe->Delete();
			return(-1);
		}

		// 様式１６－２ ----
		strSQL.Empty();
		strSQL += _T("INSERT INTO uc_lst_kamoku(KnSeq,FormSeq,KnOrder,KnCode,KnName,FgDel,FgDft,KnKana,IkkatuVal,PrSign) ");
		strSQL.Format(strSQL + _T("VALUES(1000006,22,5,NULL,'%s',0,0,'%s',100000,0)  \r\n"), ADDKMK_1621_UCHINAME, ADDKMK_1621_UCHIKANA);
		try	{
			m_pZmSub->m_database->ExecuteSQL(strSQL);
		}
		catch(CDBException* dbe)	{
			ICSMessageBox(dbe->m_strError);
			dbe->Delete();
			return(-1);
		}

		// -------------------------
		// uc_ren_kamoku
		int		cotype = 0;
		if((iapno & 0xff) == 0x00)			cotype = 0;		// 法人
		else if((iapno & 0xff) == 0x01)		cotype = 1;		// 運送
		else if((iapno & 0xff) == 0x20)		cotype = 32;	// 建設
		else if((iapno & 0xff) == 0x30)		cotype = 48;	// 病院

		strSQL.Empty();
		// 様式②
		strSQL.Format(strSQL + _T("INSERT INTO uc_ren_kamoku VALUES (1000001, 2, %d, '%s', 1000001, 0)\n"), cotype, ADDKMK_0211_KCODE);
		//strSQL.Format(strSQL + _T("INSERT INTO uc_ren_kamoku VALUES (1000002, 2, %d, '%s', 1000003, 0)\n"), cotype, ADDKMK_0213_KCODE);
		// 様式⑧
		strSQL.Format(strSQL + _T("INSERT INTO uc_ren_kamoku VALUES (1000003, 9, %d, '%s', 1000004, 0)\n"), cotype, ADDKMK_0811_KCODE);
		//strSQL.Format(strSQL + _T("INSERT INTO uc_ren_kamoku VALUES (1000004, 9, %d, '%s', 1000005, 0)\n"), cotype, ADDKMK_0812_KCODE);
		// 様式⑯－２
		strSQL.Format(strSQL + _T("INSERT INTO uc_ren_kamoku VALUES (1000005, 22, %d, '%s', 1000006, 0)\n"), cotype, ADDKMK_1621_KCODE);
		try	{
			m_pZmSub->m_database->ExecuteSQL(strSQL);
		}
		catch(CDBException* dbe)	{
			ICSMessageBox(dbe->m_strError);
			dbe->Delete();
			return(-1);
		}
	}

	// -------------------------
	// コンバート処理が終わったので、uc_inf_main::GeneralVar[5] & BIT_D1(0x02)をONにする
	if(mfcMain.Init() == DB_ERR_OK)	{
		mfcMain.Edit();
		mfcMain.m_GeneralVar[5] |= BIT_D1;
		mfcMain.Update();
		mfcMain.Fin();

		bG_KmkAdd2 = TRUE;
	}

	return(0);
}
// 改良No.21-0086,21-0529 add <--
