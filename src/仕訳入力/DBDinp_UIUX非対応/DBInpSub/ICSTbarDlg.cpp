// ICSTbarDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "ICSTbarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CICSTbarDlg ダイアログ


CICSTbarDlg::CICSTbarDlg(UINT dlgID, UINT barID, int border, CWnd* pParent /*=NULL*/)
	: ICSDialog(dlgID, pParent)
{
	//{{AFX_DATA_INIT(CICSTbarDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
	m_DlgID  = dlgID;
	m_toolBarID = barID;
	m_Border = border;
	m_pParent = pParent;

	m_Xrate = m_Yrate = -1;
}


void CICSTbarDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CICSTbarDlg)
                // メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CICSTbarDlg, ICSDialog)
	//{{AFX_MSG_MAP(CICSTbarDlg)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipNotify )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CICSTbarDlg メッセージ ハンドラ

BOOL CICSTbarDlg::OnInitDialog() 
{
//	SizeChange();
// TODO: この位置に初期化の補足処理を追加してください

	ICSDialog::OnInitDialog();

#ifdef TEST_CLOSE
	SizeChange();
	
	if (m_toolBarID)
	{
		if( ToolBarCreate() != 0 )
			EndDialog(-1);
	}
#endif

	ICSDialog::OnInitDialogEX();
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}



int CICSTbarDlg::ToolBarCreate()
{
	// Create toolbar at the top of the dialog window
	if (!m_toolBar.Create(this) || 
		!m_toolBar.LoadToolBar(m_toolBarID))
	{
		TRACE0("Crete ToolBar Failure");
	    return -1;
	}             
//TRACE( "getbarstyle %04x\n", m_toolBar.GetBarStyle() ); 
	DWORD dwStyle;
	dwStyle = (m_toolBar.GetBarStyle()|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC);
	if( m_Border ) dwStyle |= CBRS_BORDER_BOTTOM;

	m_toolBar.SetBarStyle( dwStyle );
	
    // We need to resize the dialog to make room for control bars.
	// First, figure out how big the control bars are.
	CRect rcClientStart;
	CRect rcClientNow;
	GetClientRect(rcClientStart);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 
				   0, reposQuery, rcClientNow);
	
	// Now move all the controls so they are in the same relative
	// position within the remaining client area as they would be
	// with no control bars.
	CPoint ptOffset(rcClientNow.left - rcClientStart.left,
					rcClientNow.top - rcClientStart.top); 

	CRect  rcChild;					
	CWnd* pwndChild = GetWindow(GW_CHILD);
	while (pwndChild)
	{                               
		pwndChild->GetWindowRect(rcChild);
		ScreenToClient(rcChild);
		rcChild.OffsetRect(ptOffset);
		pwndChild->MoveWindow(rcChild, FALSE);
		pwndChild = pwndChild->GetNextWindow();
	}

	// Adjust the dialog window dimensions
	CRect rcWindow;
	GetWindowRect(rcWindow);
	rcWindow.right += rcClientStart.Width() - rcClientNow.Width();
	rcWindow.bottom += rcClientStart.Height() - rcClientNow.Height();
	MoveWindow(rcWindow, FALSE);
	
	// And position the control bars
//	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);	
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, m_DlgID);	

	return 0;
}



// ﾂｰﾙﾁｯﾌﾟ表示
BOOL CICSTbarDlg::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * result )
{
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
	UINT nID = pNMHDR->idFrom;
//TRACE( "nID = %d, id = %d\n", nID, id);
	if (pTTT->uFlags & TTF_IDISHWND) {
		// idFrom is actually the HWND of the tool
		nID = ::GetDlgCtrlID((HWND)nID);
	}	
	if( nID ){
		pTTT->lpszText = MAKEINTRESOURCE(nID);
		pTTT->hinst = AfxGetResourceHandle();
		return(TRUE);
	}
	else	return(FALSE);
}


// ダイアログのサイズ変更
void CICSTbarDlg::SizeChange()
{
	if( m_Xrate == -1 || m_Yrate == -1 ) return;

//TRACE( "X %E, Y %E\n", m_Xrate, m_Yrate );
	if( m_Xrate == 1 ) m_Xrate = 1.01;
	if( m_Yrate == 1 ) m_Yrate = 1.01;

	RECT	newrect;

	GetWindowRect( &newrect );
	// 高さ
	newrect.bottom = newrect.top + ((newrect.bottom - newrect.top ) * m_Yrate);
	// 幅
	newrect.right = newrect.left + ((newrect.right - newrect.left ) * m_Xrate);

	SetWindowPos( 0, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_NOMOVE|SWP_NOZORDER );

}

