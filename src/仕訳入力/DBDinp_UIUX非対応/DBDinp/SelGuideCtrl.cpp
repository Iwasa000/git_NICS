// SelGuideCtrl.cpp : 実装ファイル
//

#include "stdafx.h"
#include "SelGuideCtrl.h"


// CSelGuideCtrl

IMPLEMENT_DYNAMIC(CSelGuideCtrl, CWnd)

CSelGuideCtrl::CSelGuideCtrl( BOOL bClickActive/* = TRUE*/ )
{
	RegisterWindowClass();

	m_bPressU = m_bPressD = FALSE;
	m_htScrol = -1;

	m_bClickActive = bClickActive;
}

CSelGuideCtrl::~CSelGuideCtrl()
{
}


BOOL CSelGuideCtrl::RegisterWindowClass()
{

    WNDCLASS wndcls;
    //HINSTANCE hInst = AfxGetInstanceHandle();
    HINSTANCE hInst = AfxGetResourceHandle();

    if (!(::GetClassInfo(hInst, SELGUIDE_CLASSNAME, &wndcls)))
    {
        // otherwise we need to register a new class
        wndcls.style            = CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNCLIENT;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
#ifndef _WIN32_WCE_NO_CURSOR
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
#else
        wndcls.hCursor          = 0;
#endif
    	wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
	//	wndcls.hbrBackground    = (HBRUSH) (COLOR_HIGHLIGHT);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = SELGUIDE_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;

}



BEGIN_MESSAGE_MAP(CSelGuideCtrl, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()

	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()


// CSelGuideCtrl メッセージ ハンドラ


void CSelGuideCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	// 描画メッセージで CWnd::OnPaint() を呼び出さないでください。

	// 縦スクロールバーのような描画
	CRect rc, rcPos;
	GetClientRect(&rc);

	CBrush BR;
	COLORREF bak = ::GetSysColor( COLOR_3DFACE );

	BR.CreateSolidBrush( bak );
	dc.FillRect( &rc, &BR ); // 四角形 RE を、ブラシ BR で描く

	rcPos.CopyRect( rc );

	if( m_htScrol == -1 ) {
		m_htScrol = rc.Height() / 8;
	}
	rcPos.bottom = rcPos.top + m_htScrol;

	int nState;

	nState = DFCS_SCROLLUP;
	if( m_bPressU )	nState |= DFCS_PUSHED;
	dc.DrawFrameControl( &rcPos, DFC_SCROLL, nState );

	rcPos.CopyRect( rc );
	rcPos.top = rcPos.bottom - m_htScrol;

	nState = DFCS_SCROLLDOWN;
	if( m_bPressD )	nState |= DFCS_PUSHED;
	dc.DrawFrameControl( &rcPos, DFC_SCROLL, nState );

	CFont* pf = GetParent()->GetFont();
	CFont* pOldFont;

	pOldFont = dc.SelectObject( pf );

	CString str;
	CSize	sz;

	str = "Pg";
	sz = dc.GetTextExtent( str );

	int wd = rc.Width();

	int x, y;

	COLORREF old = dc.SetBkColor( bak );

	x = 0;
	if( (wd - sz.cx) > 0 ) {
		x = (wd - sz.cx)/2;
	}
	y = m_htScrol;

	dc.TextOut( x, y, str );

	y += sz.cy;
	str = "UP";
	sz = dc.GetTextExtent( str );
	x = 0;
	if( (wd - sz.cx) > 0 ) {
		x = (wd - sz.cx)/2;
	}
	dc.TextOut( x, y, str );

	str = "DN";
	x = 0;
	sz = dc.GetTextExtent( str );
	if( (wd - sz.cx) > 0 ) {
		x = (wd - sz.cx)/2;
	}
	y = rc.bottom - m_htScrol - sz.cy;

	dc.TextOut( x, y, str );

	y -= sz.cy;
	str = "Pg";
	sz = dc.GetTextExtent( str );
	x = 0;
	if( (wd - sz.cx) > 0 ) {
		x = (wd - sz.cx)/2;
	}
	dc.TextOut( x, y, str );

	dc.SelectObject( pOldFont );
	dc.SetBkColor( old );
}

void CSelGuideCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	CRect rc, upRect, dnRect;

	GetClientRect(&rc);
	upRect.CopyRect( rc );

	if( m_htScrol == -1 ) {
		m_htScrol = rc.Height() / 8;
	}
	upRect.bottom = upRect.top + m_htScrol;

	dnRect.CopyRect( rc );
	dnRect.top = dnRect.bottom - m_htScrol;

	CClientDC  dc(this);

	if (upRect.PtInRect(point))
	{
		dc.DrawFrameControl(upRect,DFC_SCROLL,DFCS_SCROLLUP|DFCS_PUSHED);
		m_bPressU=TRUE;
		RedrawWindow();
		SetTimer(1,100,NULL);
		m_bPressD=FALSE;

		GetParent()->PostMessageA( WM_SELGUIDEMSG, VK_PRIOR );
	}
	else if (dnRect.PtInRect(point))
	{
		dc.DrawFrameControl(dnRect,DFC_SCROLL,DFCS_SCROLLDOWN|DFCS_PUSHED);
		m_bPressD=TRUE;
		RedrawWindow();
		SetTimer(1,100,NULL);
		m_bPressU=FALSE;

		GetParent()->PostMessageA( WM_SELGUIDEMSG, VK_NEXT );
	}

	CWnd::OnLButtonDown(nFlags, point);
}


void CSelGuideCtrl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	if (nIDEvent == 1)
	{
		CPoint pt(GetMessagePos());
		CRect m_rc;
		ScreenToClient(&pt);
		GetClientRect(m_rc);
		if (!m_rc.PtInRect(pt))
		{
			KillTimer(1);
			CClientDC dc(this);

			CRect rc, upRect, dnRect;

			GetClientRect(&rc);
			upRect.CopyRect( rc );

			if( m_htScrol == -1 ) {
				m_htScrol = rc.Height() / 8;
			}
			upRect.bottom = upRect.top + m_htScrol;

			dnRect.CopyRect( rc );
			dnRect.top = dnRect.bottom - m_htScrol;

			if (m_bPressU==TRUE) {
				dc.DrawFrameControl(upRect,DFC_SCROLL,DFCS_SCROLLUP);
				m_bPressU = FALSE;
			}
			if (m_bPressD==TRUE) {
				dc.DrawFrameControl(dnRect,DFC_SCROLL,DFCS_SCROLLDOWN);
				m_bPressD = FALSE;
			}
		}
	}

	CWnd::OnTimer(nIDEvent);
}


void CSelGuideCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。

	CRect rc;
	GetClientRect(&rc);

	if( m_htScrol == -1 ) {
		m_htScrol = rc.Height() / 8;
	}
}


void CSelGuideCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	CRect rc, upRect, dnRect;

	GetClientRect(&rc);
	upRect.CopyRect( rc );

	if( m_htScrol == -1 ) {
		m_htScrol = rc.Height() / 8;
	}
	upRect.bottom = upRect.top + m_htScrol;

	dnRect.CopyRect( rc );
	dnRect.top = dnRect.bottom - m_htScrol;

	if (m_bPressU == TRUE)
	{
		if (upRect.PtInRect(point))
		{
		//	dc.DrawFrameControl(upRect,DFC_SCROLL,DFCS_SCROLLUP);
			m_bPressU = FALSE;
			RedrawWindow();
		}
	}

	if (m_bPressD == TRUE)
	{
		if (dnRect.PtInRect(point))
		{
		//	dc.DrawFrameControl(dnRect,DFC_SCROLL,DFCS_SCROLLDOWN);
			m_bPressD = FALSE;
			RedrawWindow();
		}
	}

	CWnd::OnLButtonUp(nFlags, point);
}


int CSelGuideCtrl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	if( ! m_bClickActive && nHitTest == HTCLIENT ) {
		if( message == WM_LBUTTONDOWN ) {
			POINT	pt;
			CRect	rect;
			BOOL	bCheck = FALSE;

			if( GetCursorPos( &pt ) ) {

				CRect rc, upRect, dnRect;

				ScreenToClient( &pt );

				GetClientRect(&rc);
				upRect.CopyRect( rc );

				if( m_htScrol == -1 ) {
					m_htScrol = rc.Height() / 8;
				}
				upRect.bottom = upRect.top + m_htScrol;

				dnRect.CopyRect( rc );
				dnRect.top = dnRect.bottom - m_htScrol;

				CClientDC dc(this);

				if (upRect.PtInRect( pt ))
				{
					dc.DrawFrameControl(upRect,DFC_SCROLL,DFCS_SCROLLUP|DFCS_PUSHED);
					m_bPressU=TRUE;
					RedrawWindow();
					SetTimer(1,100,NULL);
					m_bPressD=FALSE;

					GetParent()->PostMessageA( WM_SELGUIDEMSG, VK_PRIOR );
				}
				else if (dnRect.PtInRect( pt ))
				{
					dc.DrawFrameControl(dnRect,DFC_SCROLL,DFCS_SCROLLDOWN|DFCS_PUSHED);
					m_bPressD=TRUE;
					RedrawWindow();
					SetTimer(1,100,NULL);
					m_bPressU=FALSE;

					GetParent()->PostMessageA( WM_SELGUIDEMSG, VK_NEXT );
				}
			}
		}
		return MA_NOACTIVATEANDEAT;
	}

	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}
