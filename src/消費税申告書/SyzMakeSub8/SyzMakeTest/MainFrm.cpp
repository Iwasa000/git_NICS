// MainFrm.cpp : CMainFrame クラスの実装
//

#include "stdafx.h"
#include "SyzMakeTest.h"

#include "MainFrm.h"
#include "SyzMakeTestDoc.h"
#include "SyzMakeTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, ICSMainFrame)

BEGIN_MESSAGE_MAP(CMainFrame, ICSMainFrame)
	ON_WM_CREATE()
	ON_MESSAGE(ICS_MAINFRAME_MESSAGE_CALL,OnCall)
	ON_MESSAGE(ICS_MAINFRAME_MESSAGE_JOBSEL,OnFromForkChain)
	ON_MESSAGE(ICS_MAINFRAME_MESSAGE_COSEL,OnFromParentCosel)
	ON_MESSAGE(WM_CLOSE,OnMessageClose)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ステータス ライン インジケータ
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame コンストラクション/デストラクション

CMainFrame::CMainFrame()
{
	SetCommandBarsID( IDR_SYZMAKETESTTYPE );
	// TODO: メンバ初期化コードをここに追加してください。
#ifdef _DEBUG
	m_mon = new ICSMon(GetCurrentProcessId());
	SET_MonitorVersion( 1 );
#endif
}

CMainFrame::~CMainFrame()
{
#ifdef _DEBUG
	delete m_mon;
#endif
}

int CMainFrame::BeginApp()
{
	// タブを非表示に
	HideMDITabs();

	// 会社確認
	if( m_cosel.co_ok() != IDOK ){
		ICSExit( 0, _T("処理を中止します") );
		return 0;
	}

//	int st = m_cosel.co_ok();

	ICSMainFrame::BeginApp();

	CMDIChildWnd *pChild = (CMDIChildWnd *) this->GetActiveFrame();
	if( pChild != NULL ){
		// Get the active view attached to the active MDI childwindow.
		CSyzMakeTestView *pView = (CSyzMakeTestView *) pChild->GetActiveView();
		if( pView != NULL ){
			pView->InitApl();
		}
	}
	
	return 0;
}

LRESULT	CMainFrame::OnMessageClose( WPARAM w, LPARAM l )
{
	//WPARAMが何れの値であっても、クローズ処理などは必要です。
	//下記分岐が必要なのは、正常終了メッセージや、エラーメッセージを表示する等です。
	//エラー終了時に特段の処理が無い場合は、０とそれ以外という分岐で構いません。
	if( w == 0 ) {
		//正常終了
		//終了ボタンやENDキー等、通常のWM_CLOSE

	}
	else if( w == 1 ) {
		//エラー終了
		//ICSExit()等の強制終了関数で、エラーとしてのWM_CLOSE
	}
	else {
		//それ以外
		//モジュールからの一括終了等、特殊用途のWM_CLOSE
		//100:一括終了
	}






	
	return	ICSMainFrame::OnMessageClose( w, l );

}

int CMainFrame::TabLButtonDown()
{
	ICSMainFrame::TabLButtonDown();
	return	0;
}

int CMainFrame::TabRButtonDown()
{
	ICSMainFrame::TabRButtonDown();
	return	0;
}

void CMainFrame::ChildFrameActive( CWnd* pChild )
{
	ICSMainFrame::ChildFrameActive( pChild );
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (ICSMainFrame::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !ICSMainFrame::PreCreateWindow(cs) )
		return FALSE;

	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return TRUE;
}


// CMainFrame 診断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	ICSMainFrame::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	ICSMainFrame::Dump(dc);
}

#endif //_DEBUG


// CMainFrame メッセージ ハンドラ

LRESULT CMainFrame::OnCall( WPARAM wParam, LPARAM lParam )
{
//	return	ICSMainFrame::OnCall( wParam, lParam );

	//プロセス呼び出し開始
	if( wParam == ICS_MAINFRAME_PARAM_CALLSTART ) {
		char*	pStr	=	(char*)lParam;
		//呼び出しを許可しない場合０以外を返す
		//return 1L;
	}
	//プロセス呼び出しに成功
	else if( wParam == ICS_MAINFRAME_PARAM_CALLOK ) {
		//CHAINの場合で自分自身を終了させる場合は０以外を返す
		//return 1L;
	}
	//プロセス呼び出しに失敗
	else if( wParam == ICS_MAINFRAME_PARAM_CALLERROR ) {
		//CHAINの場合で自分自身を終了させる場合は０以外を返す
		//return 1L;
	}
	return	0L;
}

LRESULT CMainFrame::OnFromForkChain( WPARAM wParam, LPARAM lParam )
{
	LRESULT	st	=	ICSMainFrame::OnFromForkChain( wParam, lParam );

	//プロセスが終了
	if( wParam == ICS_MAINFRAME_PARAM_FORKEND ) {
	}
	//自身がFORK済みでアクティブになったとき
	else if( wParam == ICS_MAINFRAME_PARAM_FORKCHANGEACTIVE ) {
		//他のプロセスでマスターに変更が加えられた為、再リードが必要
		if( st ) {
		}
	}
	//自身がアクティブでボタン押下により、他のプロセスへ切り替えられようとしているとき
	else if( wParam == ICS_MAINFRAME_PARAM_FORKCHANGING ) {
		//他のプロセスで再リードが必要な場合は「1」を返す。
	}
	//自身が初めてプロセス起動されたとき
	else if( wParam == ICS_MAINFRAME_PARAM_FORKCALLEDSTART ) {
	}
	//起動されている自身以外のプロセスがアクティブに切り替えられた時
	else if( wParam == ICS_MAINFRAME_PARAM_FORKCHANGE ) {
	}
	
	return	st;
}

LRESULT CMainFrame::OnFromParentCosel( WPARAM wParam, LPARAM lParam )
{
	//他のプロセスで会社選択ツールバーで会社が変更された
	if( wParam == 1 ) {
	}
	//自分自身の会社選択ツールバーが押された
	else {
	}
	return	0;
}
