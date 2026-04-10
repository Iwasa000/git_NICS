// TksrchDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "resource.h"

#include "DBInpSubFunc.h"
#include "TksrchDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTksrchDlg ダイアログ

char CTksrchDlg::m_Kana[10] = {0};


CTksrchDlg::CTksrchDlg(CDBZmSub* pZm, CPoint& pt, int psg, CWnd* pParent /*=NULL*/)
	: ICSDialog(CTksrchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTksrchDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
	m_pZm = pZm;

	// 開始位置
	m_Xps = pt.x;
	m_Yps = pt.y;
	// 開始位置サイン
	m_Psg = psg;

	m_TKcod = 0;

	m_bInit = FALSE;
}


void CTksrchDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTksrchDlg)
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_ICSDBEDTKANA, m_DBedt1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTksrchDlg, ICSDialog)
	//{{AFX_MSG_MAP(CTksrchDlg)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_USER_1, OnUser_1)
	ON_LBN_DBLCLK(IDC_LIST1, &CTksrchDlg::OnLbnDblclkList1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTksrchDlg メッセージ ハンドラ

BOOL CTksrchDlg::OnInitDialog() 
{
//	ControlInit(142,195,-1);
	
	ICSDialog::OnInitDialog();

	ICSDialog::OnInitDialogEX();

	m_DBedt1.SetFrame( -1, RGB_DARK_GRAY );

	CRect rc, newrect;
	GetWindowRect( &rc );

	if( !m_Psg ) {	// 指定 ｘ座標を右側に
		newrect.right = m_Xps;
		newrect.left = newrect.right - rc.Width();
	}
	else {			// 指定 ｘ座標を左側に
		newrect.left = m_Xps;
		newrect.right = newrect.left + rc.Width();
	}
	newrect.top = m_Yps;
	newrect.bottom = newrect.top + rc.Height();

	MoveWindow( &newrect );

	// TODO: この位置に初期化の補足処理を追加してください

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}


void CTksrchDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	ICSDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	if( bShow == TRUE && !m_bInit ) {
		m_bInit = TRUE;

		PostMessage( WM_USER_1, 0, 0 );
	}
}


LRESULT CTksrchDlg::OnUser_1( WPARAM wParam, LPARAM lParam )
{
	if( wParam == 0 ) {
		CString kana;
		kana.Empty();
		KanaTerminationSub( kana );	// 全表示

		c_set( IDC_ICSDBEDTKANA );
	}

	return 1;
}


BEGIN_EVENTSINK_MAP(CTksrchDlg, ICSDialog)
    //{{AFX_EVENTSINK_MAP(CTksrchDlg)
	ON_EVENT(CTksrchDlg, IDC_ICSDBEDTKANA, 1 /* Termination */, OnTerminationIcsdbedtkana, VTS_I4 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CTksrchDlg::OnTerminationIcsdbedtkana(long nChar, long inplen, long kst) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	VARIANT var;
	CString kana;

	if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
		nChar = VK_F2;

	if( nChar == VK_RETURN ) {
		DBdata_get( this, IDC_ICSDBEDTKANA, &var, ICSDBEDT_TYPE_STRING, 0 );

		kana = (char*)var.pbVal;

		if( ! kana.IsEmpty() ) {
			KanaTerminationSub( kana );
		}
	}
	else if( nChar == VK_TAB ) {
		NextDlgCtrl();
	}
	else if( nChar == VK_F2 || nChar == VK_LEFT ) {
		PrevDlgCtrl();
	}
}


// カナを読み取ってリストへ 摘要セット
void CTksrchDlg::KanaTerminationSub( CString& kana )
{
	BOOL bAll = FALSE;

	if( ! kana.IsEmpty() ) {
		if( tkykana_search( (char *)(LPCTSTR)kana, m_pZm ) == 0 ) {
			::ZeroMemory( m_Kana, sizeof(m_Kana));
			memcpy( m_Kana, kana.GetBuffer(10), kana.GetLength() );
			kana.ReleaseBuffer();
		//	c_set( IDC_ICSLIST_TKSRCH );

		}
		else	bAll = TRUE;
	}
	else {
		::ZeroMemory( m_Kana, sizeof(m_Kana));
		bAll = TRUE;
	}

	if( bAll ) {
		tkdb_init( m_pZm );
		VARIANT var;
		var.pbVal = NULL;
		DBdata_set( this, IDC_ICSDBEDTKANA, &var, ICSDBEDT_TYPE_STRING, 0 );
	}

	m_List.ResetContent();
	m_pZm->tkrec->MoveFirst();
	CString str;
	int i = 0;

	if( m_pZm->tkrec->st != -1 ) {
		do {
			str.Format( "[%6d] %-40.40s", m_pZm->tkrec->tkcod, m_pZm->tkrec->tkname );
			m_List.AddString( str );
			m_List.SetItemData( i, m_pZm->tkrec->tkcod );
			i++;
		} while( m_pZm->tkrec->MoveNext() == 0 );
	}

	m_List.SetFocus();
	m_List.SetCurSel(0);

}


void CTksrchDlg::OnOK()
{
	int sel = m_List.GetCurSel();
	m_TKcod = 0;

	if( sel != LB_ERR ) {
		m_TKcod = m_List.GetItemData( sel );
	}

	ICSDialog::OnOK();	
}



BOOL CTksrchDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	CWnd* pwnd = GetDlgItem(IDC_ICSDBEDTKANA);

	if( pwnd ) {
		if( pMsg->hwnd == pwnd->m_hWnd ) {
			if( pMsg->message == WM_KEYDOWN ) {
				if( pMsg->wParam == VK_ESCAPE ) {
					OnCancel();
					return TRUE;
				}
			}
		}
	}
	if( pMsg->message == WM_KEYDOWN ) {
		if( pMsg->hwnd == m_List.m_hWnd ) {
			if( pMsg->wParam == VK_TAB || pMsg->wParam == VK_LEFT || pMsg->wParam == VK_F2 ) {
				NextDlgCtrl();
				return TRUE;
			}
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}


// リストボックス ダブルクリック
void CTksrchDlg::OnLbnDblclkList1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	OnOK();
}
