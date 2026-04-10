// ICSListBox.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "ICSListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ICSListBox

ICSListBox::ICSListBox() : Mode(CLICKMODE_ONECLICK),Position(-1)
{
	ClickDown = FALSE;
}

ICSListBox::~ICSListBox()
{
}


BEGIN_MESSAGE_MAP(ICSListBox, CListBox)
	//{{AFX_MSG_MAP(ICSListBox)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ICSListBox メッセージ ハンドラ
BOOL ICSListBox::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	switch( pMsg->message ) {
		case	WM_KEYDOWN:
			switch( pMsg->wParam ) {
				case	VK_RETURN:
					PostMessage( WM_KEYDOWN, VK_TAB );
					break;
			}
			break;
	}
	return CListBox::PreTranslateMessage(pMsg);
}

void ICSListBox::OnMouseMove(UINT nFlags, CPoint point) 
{
#ifdef	CUT
	if( Mode == CLICKMODE_ONECLICK ) {
		if( nFlags & MK_LBUTTON )	return;
	}
#endif

	CListBox::OnMouseMove(nFlags, point);
}


void ICSListBox::OnLButtonDown(UINT nFlags, CPoint point) 
{

	CListBox::OnLButtonDown(nFlags, point);

	ClickDown = FALSE;
	int i = GetCount();
	if( i > 0 ) {
		CRect	rc;
		GetItemRect( i-1, rc );
		if( point.y > rc.bottom ) return;
	}
	ClickDown = TRUE;

#ifdef	DOWNSELECT
	if( Mode == CLICKMODE_ONECLICK ) {
		PostMessage( WM_LBUTTONUP, MAKELONG( point.x, point.y ));
		PostMessage( WM_LBUTTONDBLCLK, MAKELONG( point.x, point.y ));
		PostMessage( WM_LBUTTONUP, MAKELONG( point.x, point.y ));
	}
	else if( Mode == CLICKMODE_TWOCLICK ) {
		if( Position >= 0 ) {
			if( GetCurSel() == Position ) {
				Position	=	-1;
				PostMessage( WM_LBUTTONUP, MAKELONG( point.x, point.y ));
				PostMessage( WM_LBUTTONDBLCLK, MAKELONG( point.x, point.y ));
				PostMessage( WM_LBUTTONUP, MAKELONG( point.x, point.y ));
			}
			else {
				Position	=	GetCurSel();
			}
		}
		else {
			Position	=	GetCurSel();
		}
	}
#endif

}


void ICSListBox::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください


	CListBox::OnLButtonUp(nFlags, point);

	if( ClickDown == FALSE ) return;


#ifdef	DOWNSELECT
	if( Mode == CLICKMODE_TWOCLICK ) {
		Position = GetCurSel();
	}
#else
	if( Mode == CLICKMODE_ONECLICK ) {
		PostMessage( WM_LBUTTONDBLCLK, MAKELONG( point.x, point.y ));
	}
	else if( Mode == CLICKMODE_TWOCLICK ) {
		if( Position >= 0 ) {
			if( GetCurSel() == Position ) {
				Position	=	-1;
				PostMessage( WM_LBUTTONDBLCLK, MAKELONG( point.x, point.y ));
			}
			else {
				Position	=	GetCurSel();
			}
		}
		else {
			Position	=	GetCurSel();
		}
	}
#endif

}


void ICSListBox::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CListBox::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください

	if( (nState == WA_INACTIVE) && (Mode == CLICKMODE_TWOCLICK) ) {
		Position = -1;
	}
	
}
