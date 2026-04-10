// BaseDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "resource.h"

#include "BaseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseDlg ダイアログ


CBaseDlg::CBaseDlg(UINT dlgID, CWnd* pParent /*=NULL*/)
	: ICSDialog(dlgID, pParent)
{
	//{{AFX_DATA_INIT(CBaseDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
	m_pZm = NULL;

	m_DlgID = dlgID;
	m_pParent = pParent;	// 親ウィンドウは タブ

#ifdef CLOSE
	// アクセラレータのロード
	m_hAccelTable = ::LoadAccelerators( AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
#endif
	m_Xrate = m_Yrate = 1.0;

	m_bDBupdate = FALSE;
}


void CBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBaseDlg)
                // メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBaseDlg, ICSDialog)
	//{{AFX_MSG_MAP(CBaseDlg)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_USER_1, OnUser1_Msg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBaseDlg メッセージ ハンドラ


BOOL CBaseDlg::OnInitDialog() 
{
//	ControlInit( m_Xdl, m_Ydl, -1 );
	
	ICSDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
ASSERT( m_pZm != NULL );
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}


BOOL CBaseDlg::CreateDlg()
{
	return ICSDialog::Create(m_DlgID, m_pParent);
}


void CBaseDlg::PostNcDestroy() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	
	delete this;
}

// ダイアログの表示
// 
void CBaseDlg::WndShow(BOOL bShow)
{
	int showsgn;

	showsgn = bShow ? SW_SHOW : SW_HIDE;

	if( !bShow ) {
	//	m_Dw.DelInputBox();
	//	LastDataGet();		// 最終に入力ボックスがあったデータ取得
	}

	ShowWindow( showsgn );

	if( bShow ) {
		DlgWndMove( m_Xrate, m_Yrate );
	}
}

// 追加時のダイアログの表示
// 
void CBaseDlg::WndShow_Add(BOOL bShow)
{
	WndShow( bShow );
}


// ダイアログの位置調整(親ウィンドウに対して)
// 
void CBaseDlg::DlgWndMove( double x, double y )
{
	CRect rc;

#ifdef _CTABCTRL
	CTabCtrl* pTab;

	pTab = (CTabCtrl*)m_pParent;
	pTab->GetWindowRect(rc);
	pTab->AdjustRect( FALSE, rc );
	rc.top += 3;
	rc.bottom -= 3;

	pTab->ScreenToClient( rc );

	MoveWindow( rc );
#endif

	// リサイズ比率格納
	m_Xrate = x;
	m_Yrate = y;
}


BOOL CBaseDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	CWnd* pwnd = GetParentOwner();
	HWND hwnd = NULL;
	if( pwnd ) hwnd = pwnd->m_hWnd;

#ifdef CLOSE
	// アクセラレータを転送
	if( (m_hAccelTable != NULL) && ::TranslateAccelerator( hwnd, m_hAccelTable, pMsg))
	{
		return TRUE;
	}
#endif
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE )
		return TRUE;

	if( pMsg->message == WM_KEYDOWN ) {
		switch( pMsg->wParam ) {
		case VK_F3:
		case VK_F8:
		case VK_F9:
		case VK_END:
			GetParentOwner()->PostMessage( pMsg->message, pMsg->wParam, pMsg->lParam );
			break;
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

// 財務クラスのセット
void CBaseDlg::Set_Class( CDBZmSub* pZm, CDBSyohi* pSy )
{
	m_pZm = pZm;
	m_pSy = pSy;
}


// ユーザーメッセージ
LRESULT CBaseDlg::OnUser1_Msg( WPARAM wParam, LPARAM lParam )
{
	return 1;
}
