// OwnerButton.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "OwnerButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COwnerButton

static const COLORREF DKBLUE	= RGB( 0, 0, 128 );
static const COLORREF WHITE		= RGB( 255, 255, 255 );
static const COLORREF DKWHITE	= PALETTERGB( 218, 218, 218 );
static const COLORREF BLACK		= RGB( 1, 1, 1 );
static const COLORREF LTGRAY	= RGB( 192, 192, 192 );
static const COLORREF DKGRAY	= RGB( 128, 128, 128 );

static const COLORREF CHK_LTBLUE	=	RGB( 0,191,255);


COwnerButton::COwnerButton()
{
	m_bIsCheckBox	= FALSE;
	m_nCheck		= 0;

	m_bIsDefault	= FALSE;
	m_nTypeStyle	= 0;

	m_bIsPressed	= FALSE;
}

COwnerButton::~COwnerButton()
{
}


BEGIN_MESSAGE_MAP(COwnerButton, CButton)
	//{{AFX_MSG_MAP(COwnerButton)
	//}}AFX_MSG_MAP
	ON_MESSAGE(BM_SETCHECK, OnSetCheck)
	ON_MESSAGE(BM_GETCHECK, OnGetCheck)
	ON_CONTROL_REFLECT_EX(BN_CLICKED, &COwnerButton::OnBnClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COwnerButton メッセージ ハンドラ

void COwnerButton::DrawFrame(CDC* pDC, CRect rc, BOOL bSelected)
{
	COLORREF	upcol, dncol;
	int nUp, nDown;

	for( short i = 0; i < 1; i++ ) {
		// ﾌﾚｰﾑの色の設定
		if( bSelected == FALSE ) {
			nUp = (i == 0) ? COLOR_3DHILIGHT : COLOR_3DLIGHT;	// 上段の縁
			nDown = (i == 0) ? COLOR_3DDKSHADOW : COLOR_3DSHADOW;	// 下段の縁
			upcol = ::GetSysColor(nUp);
			dncol = ::GetSysColor(nDown);
		}
		else {
			nUp = (i == 0) ? COLOR_3DDKSHADOW : COLOR_3DSHADOW;
			nDown = (i == 0) ? COLOR_3DHILIGHT : COLOR_3DLIGHT;
			upcol = ::GetSysColor(nUp);
			dncol = ::GetSysColor(nDown);
		}
		DrawLine( pDC, rc.left, rc.top, rc.left, (rc.bottom-1), upcol );
		DrawLine( pDC, rc.left, rc.top, rc.right, rc.top, upcol );

		DrawLine( pDC, rc.left, (rc.bottom-1), rc.right, (rc.bottom-1), dncol );
		DrawLine( pDC, (rc.right-1), (rc.top+1), (rc.right-1), (rc.bottom-1), dncol );

		rc.InflateRect( -1, -1 );
	}
}

void COwnerButton::DrawRect(CDC* pDC, CRect rc, COLORREF color )
{
	CBrush B;
	B.CreateSolidBrush(color);
	pDC->FillRect(rc, &B);
}

void COwnerButton::DrawLine(CDC* pDC, CRect	end, COLORREF color )
{
	CPen NewPen;
	NewPen.CreatePen( PS_SOLID, 1, color );
	CPen* pOldPen = pDC->SelectObject( &NewPen );
	pDC->MoveTo( end.left, end.top );
	pDC->LineTo( end.right, end.bottom );
	pDC->SelectObject( pOldPen );
	NewPen.DeleteObject();
}

void COwnerButton::DrawLine(CDC* pDC, long left, long top, long right, long bottom, COLORREF color )
{
	CPen NewPen;
	NewPen.CreatePen( PS_SOLID, 1, color );
	CPen* pOldPen = pDC->SelectObject( &NewPen );
	pDC->MoveTo( left, top );
	pDC->LineTo( right, bottom );
	pDC->SelectObject( pOldPen );
	NewPen.DeleteObject();
}

void COwnerButton::DrawText(CDC* pDC, CRect rc, const char* pBuf, BOOL bSelect, COLORREF textcolor )
{
	CFont* pFont = pDC->GetCurrentFont();
//	CFont* pOld;
	CFont newFont;
//	LOGFONT Lf;
	COLORREF prvcolor;
	CRect cpyrc;
	
	cpyrc = rc;

#ifdef OLD_CLOSE
	pFont->GetLogFont( &Lf );
	// 文字の太さ
	Lf.lfWeight = (bSelect == TRUE) ? FW_SEMIBOLD : FW_MEDIUM;
	newFont.CreateFontIndirect( &Lf );
	pOld = pDC->SelectObject( &newFont );
#endif

//	TRACE( "lfHeight = %d\n", Lf.lfHeight );
//	TRACE( "lfWidth = %d\n", Lf.lfWidth );
//	TRACE( "lfWeight = %d\n", Lf.lfWeight );
//	TRACE( "lfFaceName = %s\n", Lf.lfFaceName );

	prvcolor = pDC->SetTextColor( textcolor );
	pDC->SetBkMode( TRANSPARENT );
	// disable -> add white shadow
	if( !IsWindowEnabled() ) {
		cpyrc.OffsetRect(1,1);
		pDC->SetTextColor( WHITE );
		pDC->DrawText( pBuf, strlen( pBuf ), cpyrc, DT_CENTER|DT_VCENTER|DT_SINGLELINE );
		pDC->SetTextColor( textcolor );
	}

	pDC->DrawText( pBuf, strlen( pBuf ), rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE );

	pDC->SetTextColor(prvcolor);
#ifdef OLD_CLOSE
	pDC->SelectObject( pOld );
#endif
}


void COwnerButton::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	// TODO: 指定されたアイテムを描画するためのコードを追加してください
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	COLORREF c_txt;

	CRect	focus, btn;
	focus.CopyRect( &lpDIS->rcItem );
	btn.CopyRect( &lpDIS->rcItem );

	if( IsWindowEnabled() ) c_txt = ::GetSysColor(COLOR_BTNTEXT);
	else					c_txt = ::GetSysColor(COLOR_GRAYTEXT);

	focus.DeflateRect( 2,2 );

	char	buf[256];
	GetWindowText( buf, 256 );

//	UINT act = lpDIS->itemAction;
//	TRACE( "act = %d\n", act );
	UINT state = lpDIS->itemState;

	// Checkbox?
	if (m_bIsCheckBox)
	{
		m_bIsPressed  =  (lpDIS->itemState & ODS_SELECTED) || (m_nCheck != 0);
	} // if
	else	// Normal button OR other button style ...
	{
		m_bIsPressed = (lpDIS->itemState & ODS_SELECTED);
	}

	if( m_bIsCheckBox ) {
		if( (state&ODS_FOCUS) ) {
			DrawRect( pDC, focus, ::GetSysColor(COLOR_BTNFACE) );
		}	
//TRACE( "^^^^@owner m_bIsPressed %d\n", m_bIsPressed );
		if( m_bIsPressed ) {
			DrawRect( pDC, focus, CHK_LTBLUE );
			DrawFrame( pDC, btn, TRUE );
			DrawText( pDC, btn, buf, TRUE, WHITE );
		}
		else {
			DrawRect( pDC, btn, ::GetSysColor(COLOR_BTNFACE) );
			DrawFrame( pDC, btn, FALSE );
			DrawText( pDC, btn, buf, FALSE, c_txt );
		}
		if( (state&ODS_FOCUS) ) {
//			DrawRect( pDC, focus, ::GetSysColor(COLOR_BTNFACE) );
			pDC->DrawFocusRect( &focus );
		}	
	}
	else {
		if( (state&ODS_FOCUS) ) {
			DrawRect( pDC, focus, ::GetSysColor(COLOR_BTNFACE) );
			pDC->DrawFocusRect( &focus );
		//	DrawRect( pDC, focus, DKBLUE );
			DrawFrame( pDC, btn, TRUE );
			DrawText( pDC, btn, buf, TRUE, c_txt );
		}
		else {
			DrawRect( pDC, btn, ::GetSysColor(COLOR_BTNFACE) );
			DrawFrame( pDC, btn, FALSE );
			DrawText( pDC, btn, buf, FALSE, c_txt );
		}
	}
}

BOOL COwnerButton::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
/*
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN ) {
		CWnd* pWnd = GetParent();
		CWnd* pChild = FromHandle( pMsg->hwnd );
		int nID = pChild->GetDlgCtrlID();
		pWnd->PostMessage( WM_COMMAND, MAKELONG(nID, BN_CLICKED), (LPARAM)pMsg->hwnd );
		return TRUE;
	}
*/
	if (pMsg->message == WM_LBUTTONDBLCLK)
		pMsg->message = WM_LBUTTONDOWN;

	return CButton::PreTranslateMessage(pMsg);
}

void COwnerButton::PreSubclassWindow()
{
	UINT nBS;

	nBS = GetButtonStyle();

	// Set initial control type
	m_nTypeStyle = nBS & BS_TYPEMASK;

	// Check if this is a checkbox
	if (nBS & BS_CHECKBOX) m_bIsCheckBox = TRUE;

	// Set initial default state flag
	if (m_nTypeStyle == BS_DEFPUSHBUTTON)
	{
		// Set default state for a default button
		m_bIsDefault = TRUE;

		// Adjust style for default button
		m_nTypeStyle = BS_PUSHBUTTON;
	} // If

#ifdef CLOSE
	// You should not set the Owner Draw before this call
	// (don't use the resource editor "Owner Draw" or
	// ModifyStyle(0, BS_OWNERDRAW) before calling PreSubclassWindow() )
	ASSERT(m_nTypeStyle != BS_OWNERDRAW);
#endif

	// Switch to owner-draw
	ModifyStyle( 0, BS_OWNERDRAW, SWP_FRAMECHANGED);

	CButton::PreSubclassWindow();
}




LRESULT COwnerButton::OnSetCheck(WPARAM wParam, LPARAM lParam)
{
	ASSERT(m_bIsCheckBox);

	switch (wParam)
	{
		case BST_CHECKED:
		case BST_INDETERMINATE:	// Indeterminate state is handled like checked state
			SetCheck(1);
			break;
		default:
			SetCheck(0);
			break;
	} // switch

	return 0;
} // End of OnSetCheck


LRESULT COwnerButton::OnGetCheck(WPARAM wParam, LPARAM lParam)
{
	ASSERT(m_bIsCheckBox);
	return GetCheck();
} // End of OnGetCheck


DWORD COwnerButton::SetCheck(int nCheck, BOOL bRepaint/* = TRUE*/)
{
	if (m_bIsCheckBox)
	{
		if (nCheck == 0) m_nCheck = 0;
		else m_nCheck = 1;

		if (bRepaint) Invalidate();
	} // if

//	return BTNST_OK;
	return 0;
} // End of SetCheck

int COwnerButton::GetCheck()
{
	return m_nCheck;
} // End of GetCheck


BOOL COwnerButton::OnBnClicked()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	SetFocus();

	if (m_bIsCheckBox)
	{
		m_nCheck = !m_nCheck;
		Invalidate();
	} // if

	return FALSE;
}
