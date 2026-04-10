// OwnerButton.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "IconOwnerButton.h"
#include "resource.h"       // メイン シンボル

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIconOwnerButton

static const COLORREF DKBLUE	= RGB( 0, 0, 128 );
static const COLORREF WHITE		= RGB( 255, 255, 255 );
static const COLORREF DKWHITE	= PALETTERGB( 218, 218, 218 );
static const COLORREF BLACK		= RGB( 1, 1, 1 );
static const COLORREF LTGRAY	= RGB( 192, 192, 192 );
static const COLORREF DKGRAY	= RGB( 128, 128, 128 );

static const COLORREF CHK_LTBLUE	=	RGB( 0,191,255);


CIconOwnerButton::CIconOwnerButton()
	: m_ColorBtnStyle(ID_NORMAL_COLORBTN_STYLE)
	, m_bIsCheckBox(FALSE)
	, m_nCheck(0)
	, m_bIsDefault(FALSE)
	, m_nTypeStyle(0)
	, m_bIsPressed(FALSE)
{
}

CIconOwnerButton::~CIconOwnerButton()
{
}


BEGIN_MESSAGE_MAP(CIconOwnerButton, CButton)
	//{{AFX_MSG_MAP(CIconOwnerButton)
	//}}AFX_MSG_MAP
	ON_MESSAGE(BM_SETCHECK, OnSetCheck)
	ON_MESSAGE(BM_GETCHECK, OnGetCheck)
	ON_CONTROL_REFLECT_EX(BN_CLICKED, &CIconOwnerButton::OnBnClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIconOwnerButton メッセージ ハンドラ

#ifdef _20120929_EXTEND_
void CIconOwnerButton::DrawFrame(CDC* pDC, CRect rc, BOOL bSelected)
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
#else
void CIconOwnerButton::DrawFrame(CDC* pDC, CRect rc, BOOL bSelected)
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
		// 四つ角を丸角にする為に、最大領域で描画しない
		DrawLine( pDC, rc.left, (rc.top+2), rc.left, (rc.bottom-2), upcol );
		DrawLine( pDC, rc.left+2, rc.top, rc.right-2, rc.top, upcol );

		DrawLine( pDC, rc.left+2, (rc.bottom-1), rc.right-2, (rc.bottom-1), dncol );
		DrawLine( pDC, (rc.right-1), (rc.top+2), (rc.right-1), (rc.bottom-2), dncol );

		rc.InflateRect( -1, -1 );
	}
}
#endif

//-----------------------------------------------------------------------------
// 丸角を描画する
//-----------------------------------------------------------------------------
// 引数	pDC			：	デバイスコンテキスト
//		rc			：	描画領域
//		bSelected	：	選択されているか
//-----------------------------------------------------------------------------
void CIconOwnerButton::DrawRoundRect(CDC* pDC, CRect rc, BOOL bSelected )
{

	COLORREF	upcol, dncol;
	int			nUp, nDown;
	CBrush		upBrush, dnBrush;
	if( bSelected == FALSE ) {
		nUp = COLOR_3DHILIGHT;		// 上段の縁
		nDown = COLOR_3DDKSHADOW;	// 下段の縁
		upcol = ::GetSysColor(nUp);
		dncol = ::GetSysColor(nDown);
		upBrush.CreateSolidBrush( ::GetSysColor(COLOR_3DFACE) );
		dnBrush.CreateSolidBrush( ::GetSysColor(COLOR_3DLIGHT) );
	}
	else {
		nUp = COLOR_3DDKSHADOW;
		nDown = COLOR_3DHILIGHT;
		upcol = ::GetSysColor(nUp);
		dncol = ::GetSysColor(nDown);
		upBrush.CreateSolidBrush( RGB(0,191,255) );
		dnBrush.CreateSolidBrush( RGB(0, 178, 243) );
	}

	// 色のセット
	CPen NewPen;
	NewPen.CreatePen( PS_SOLID, 1, upcol );
	CPen* pOldPen = pDC->SelectObject( &NewPen );
	pDC->SelectObject( upBrush );

	// 左上
	CRect	roundRect = rc;
	roundRect.right = rc.left + 10;
	roundRect.bottom = rc.top + 10;
	pDC->RoundRect( roundRect, CPoint(5,5) );

	// 余分な線の塗りつぶし
	CRect	fillRect;
	fillRect.top = roundRect.top + 1;
	fillRect.bottom = roundRect.bottom;
	fillRect.left = roundRect.right - 2;
	fillRect.right = roundRect.right + 2;
	pDC->FillRect( fillRect, &upBrush );

	fillRect.top = roundRect.bottom - 2;
	fillRect.bottom = roundRect.bottom + 2;
	fillRect.left = roundRect.left + 1;
	fillRect.right = roundRect.right;
	pDC->FillRect( fillRect, &upBrush );

	// 右上
	roundRect = rc;
	roundRect.left = rc.right - 10;
	roundRect.bottom = rc.top + 10;
	pDC->RoundRect( roundRect, CPoint(5,5) );

	// 余分な線の塗りつぶし
	fillRect.top = roundRect.top + 1;
	fillRect.bottom = roundRect.bottom;
	fillRect.left = roundRect.left - 2;
	fillRect.right = roundRect.left + 2;
	pDC->FillRect( fillRect, &upBrush );

	fillRect.top = roundRect.bottom - 2;
	fillRect.bottom = roundRect.bottom + 2;
	fillRect.left = roundRect.left + 1;
	fillRect.right = roundRect.right;
	pDC->FillRect( fillRect, &upBrush );

	// 左下
	pDC->SelectObject( dnBrush );
	roundRect = rc;
	roundRect.right = rc.left + 10;
	roundRect.top = rc.bottom - 10;
	pDC->RoundRect( roundRect, CPoint(5,5) );

	// 余分な線の塗りつぶし
	fillRect.top = roundRect.top;
	fillRect.bottom = roundRect.bottom - 1;
	fillRect.left = roundRect.right - 2;
	fillRect.right = roundRect.right + 2;
	pDC->FillRect( fillRect, &dnBrush );

	fillRect.top = roundRect.top -2;
	fillRect.bottom = roundRect.top + 2;
	fillRect.left = roundRect.left + 1;
	fillRect.right = roundRect.right;
	pDC->FillRect( fillRect, &dnBrush );

	// 右下
	NewPen.DeleteObject();
	NewPen.CreatePen( PS_SOLID, 1, dncol );
	pOldPen = pDC->SelectObject( &NewPen );
	roundRect = rc;
	roundRect.left = rc.right - 10;
	roundRect.top = rc.bottom - 10;
	pDC->RoundRect( roundRect, CPoint(5,5) );

	// 余分な線の塗りつぶし
	fillRect.top = roundRect.top;
	fillRect.bottom = roundRect.bottom - 1;
	fillRect.left = roundRect.left - 2;
	fillRect.right = roundRect.left + 2;
	pDC->FillRect( fillRect, &dnBrush );

	fillRect.top = roundRect.top - 2;
	fillRect.bottom = roundRect.top + 2;
	fillRect.left = roundRect.left;
	fillRect.right = roundRect.right - 1;
	pDC->FillRect( fillRect, &dnBrush );

	NewPen.DeleteObject();
	upBrush.DeleteObject();
	dnBrush.DeleteObject();
}

void CIconOwnerButton::DrawRect(CDC* pDC, CRect rc, BOOL bSelected )
{

	CBrush	upBrush, dnBrush;
	CPen	upPen, dnPen;

	// 上半分
	if( bSelected == FALSE ){
		upBrush.CreateSolidBrush( ::GetSysColor(COLOR_3DFACE) );
		upPen.CreatePen( PS_SOLID, 1, ::GetSysColor(COLOR_3DFACE) );
	}
	else{
		upBrush.CreateSolidBrush( RGB(0,191,255) );
		upPen.CreatePen( PS_SOLID, 1, RGB(0,191,255) );
	}
	CRect	halfRect;
	halfRect.top = rc.top + 3;
	halfRect.left = rc.left;
	halfRect.right = rc.right;
	halfRect.bottom = rc.top + rc.Height()/2;
	pDC->FillRect( halfRect, &upBrush );

	pDC->SelectObject( upBrush );
	pDC->SelectObject( upPen );
	halfRect.top = rc.top;
	halfRect.bottom = rc.top + 5;
	pDC->RoundRect( halfRect, CPoint(5,5) );

	// 下半分
	if( bSelected == FALSE ){
		dnBrush.CreateSolidBrush( ::GetSysColor(COLOR_3DLIGHT) );
		dnPen.CreatePen( PS_SOLID, 1, ::GetSysColor(COLOR_3DLIGHT) );
	}
	else{
		dnBrush.CreateSolidBrush( RGB(0, 178, 243) );
		dnPen.CreatePen( PS_SOLID, 1, RGB(0, 178, 243) );
	}
	halfRect;
	halfRect.top = rc.top + rc.Height()/2;
	halfRect.left = rc.left;
	halfRect.right = rc.right;
	halfRect.bottom = rc.bottom - 3;
	pDC->FillRect( halfRect, &dnBrush );

	pDC->SelectObject( dnBrush );
	pDC->SelectObject( dnPen );
	halfRect.top = rc.bottom - 5;
	halfRect.bottom = rc.bottom;
	pDC->RoundRect( halfRect, CPoint(5,5) );

	upBrush.DeleteObject();
	dnBrush.DeleteObject();
	upPen.DeleteObject();
	dnPen.DeleteObject();
}

void CIconOwnerButton::DrawRect(CDC* pDC, CRect rc, COLORREF color )
{
	CBrush B;
	B.CreateSolidBrush(color);
	pDC->FillRect(rc, &B);
	B.DeleteObject();
}

void CIconOwnerButton::DrawLine(CDC* pDC, CRect	end, COLORREF color )
{
	CPen NewPen;
	NewPen.CreatePen( PS_SOLID, 1, color );
	CPen* pOldPen = pDC->SelectObject( &NewPen );
	pDC->MoveTo( end.left, end.top );
	pDC->LineTo( end.right, end.bottom );
	pDC->SelectObject( pOldPen );
	NewPen.DeleteObject();
}

void CIconOwnerButton::DrawLine(CDC* pDC, long left, long top, long right, long bottom, COLORREF color )
{
	CPen NewPen;
	NewPen.CreatePen( PS_SOLID, 1, color );
	CPen* pOldPen = pDC->SelectObject( &NewPen );
	pDC->MoveTo( left, top );
	pDC->LineTo( right, bottom );
	pDC->SelectObject( pOldPen );
	NewPen.DeleteObject();
}

void CIconOwnerButton::DrawText(CDC* pDC, CRect rc, const char* pBuf, BOOL bSelect, COLORREF textcolor )
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
/*- '12.10.02 -*/
//		pDC->DrawText( pBuf, strlen( pBuf ), cpyrc, DT_CENTER|DT_VCENTER|DT_SINGLELINE );
/*-------------*/
		pDC->DrawText( pBuf, strlen( pBuf ), cpyrc, DT_LEFT|DT_VCENTER|DT_SINGLELINE );
/*-------------*/
		pDC->SetTextColor( textcolor );
	}

/*- '12.10.02 -*/
//	pDC->DrawText( pBuf, strlen( pBuf ), rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE );
/*-------------*/
	pDC->DrawText( pBuf, strlen( pBuf ), rc, DT_LEFT|DT_VCENTER|DT_SINGLELINE );
/*-------------*/

	pDC->SetTextColor(prvcolor);
#ifdef OLD_CLOSE
	pDC->SelectObject( pOld );
#endif
}


void CIconOwnerButton::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
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
/*- '12.09.29 -*/
//			DrawRect( pDC, focus, CHK_LTBLUE );
//			DrawFrame( pDC, btn, TRUE );
//			DrawText( pDC, btn, buf, TRUE, WHITE );
/*-------------*/
			DrawRect( pDC, btn, TRUE );
			DrawRoundRect( pDC, btn, TRUE );
			DrawFrame( pDC, btn, TRUE );
			DrawText( pDC, btn, buf, TRUE, WHITE );
/*-------------*/
		}
		else {
/*- '12.09.29 -*/
//			DrawRect( pDC, btn, ::GetSysColor(COLOR_BTNFACE) );
//			DrawFrame( pDC, btn, FALSE );
//			DrawText( pDC, btn, buf, FALSE, c_txt );
/*-------------*/
			DrawRect( pDC, btn, FALSE );
			DrawRoundRect( pDC, btn, FALSE );
			DrawFrame( pDC, btn, FALSE );
			DrawText( pDC, btn, buf, FALSE, c_txt );
/*-------------*/
		}
		if( (state&ODS_FOCUS) ) {
//			DrawRect( pDC, focus, ::GetSysColor(COLOR_BTNFACE) );
			pDC->DrawFocusRect( &focus );
		}	
	}
	else {
//--> '12.10.02 INS START
		// テキスト描画用の領域取得
		CRect	txtRc;
		txtRc = btn;
		txtRc.left = btn.left + ((btn.Width()/5)*3);
//<-- '12.10.02 INS END


		if( (state&ODS_FOCUS) ) {
/*- '12.10.01 -*/
//			DrawRect( pDC, focus, ::GetSysColor(COLOR_BTNFACE) );
/*-------------*/
			if( m_ColorBtnStyle == ID_NORMAL_COLORBTN_STYLE ){
				DrawRect( pDC, btn, ::GetSysColor(COLOR_3DFACE), ::GetSysColor(COLOR_3DLIGHT) );
			}
			else{
				DrawRect( pDC, btn, RGB(0xff, 0xff, 0xc4), RGB(0xf2, 0xf2, 0xb7) );
			}
/*-------------*/
			pDC->DrawFocusRect( &focus );
		//	DrawRect( pDC, focus, DKBLUE );
/*- '12.10.01_CUT -*/
//			DrawFrame( pDC, btn, TRUE );
/*-----------------*/
/*- '12.10.02 -*/
//			DrawText( pDC, btn, buf, TRUE, c_txt );
/*-------------*/
			DrawText( pDC, txtRc, buf, TRUE, c_txt );
/*-------------*/
		}
		else {
/*- '12.10.01 -*/
//			DrawRect( pDC, btn, ::GetSysColor(COLOR_BTNFACE) );
/*-------------*/
			if( m_ColorBtnStyle == ID_NORMAL_COLORBTN_STYLE ){
				DrawRect( pDC, btn, ::GetSysColor(COLOR_3DFACE), ::GetSysColor(COLOR_3DLIGHT) );
			}
			else{
				DrawRect( pDC, btn, RGB(0xff, 0xff, 0xc4), RGB(0xf2, 0xf2, 0xb7) );
			}
/*-------------*/
/*- '12.10.01_CUT -*/
//			DrawFrame( pDC, btn, FALSE );
/*-----------------*/
/*- '12.10.02 -*/
//			DrawText( pDC, btn, buf, FALSE, c_txt );
/*-------------*/
			DrawText( pDC, txtRc, buf, FALSE, c_txt );
/*-------------*/
		}
	}

	HICON	hDispIcon = NULL;
	if( m_bIsCheckBox ){
		hDispIcon = GetIcon();
		if( hDispIcon ){
			if( m_bIsPressed ) {
/*- '12.10.01 -*/
//				pDC->DrawIcon( btn.Width()/2 - 16, 1, hDispIcon );
/*-------------*/
				if( btn.Height() < 32 ){
					DrawIconEx( pDC->m_hDC, (btn.Width()/2-12), 1, hDispIcon, 25, 24, 0, NULL, DI_IMAGE|DI_MASK );
				}
				else{
					DrawIconEx( pDC->m_hDC, (btn.Width()/2-16), 1, hDispIcon, 32, 32, 0, NULL, DI_IMAGE|DI_MASK );
				}
/*-------------*/
			}
			else{
/*- '12.10.01 -*/
//				pDC->DrawIcon( btn.Width()/2 - 17, 1, hDispIcon );
/*-------------*/
				if( btn.Height() < 32 ){
					DrawIconEx( pDC->m_hDC, (btn.Width()/2-13), 1, hDispIcon, 25, 24, 0, NULL, DI_IMAGE|DI_MASK );
				}
				else{
					DrawIconEx( pDC->m_hDC, (btn.Width()/2-17), 1, hDispIcon, 32, 32, 0, NULL, DI_IMAGE|DI_MASK );
				}
/*-------------*/
			}
		}
	}
	else{		
		hDispIcon = GetIcon();
		if( hDispIcon ){
			char	buf[256] = {0};
			GetWindowText( buf, sizeof(buf) );
/*- '12.10.01 -*/
//			if( strlen(buf) ){
//				pDC->DrawIcon( 2, 2, hDispIcon );
//			}
//			else{
//				pDC->DrawIcon( btn.Width()/2 - 17, 2, hDispIcon );
//			}
/*-------------*/
			if( strlen(buf) ){
				if( btn.Height() < 32 ){
					DrawIconEx( pDC->m_hDC, 2, 1, hDispIcon, 25, 24, 0, NULL, DI_IMAGE|DI_MASK );
				}
				else{
					DrawIconEx( pDC->m_hDC, 2, 1, hDispIcon, 32, 32, 0, NULL, DI_IMAGE|DI_MASK );
				}
			}
			else{
				if( btn.Height() < 32 ){
					DrawIconEx( pDC->m_hDC, (btn.Width()/2-13), 1, hDispIcon, 25, 24, 0, NULL, DI_IMAGE|DI_MASK );
				}
				else{
					DrawIconEx( pDC->m_hDC, (btn.Width()/2-17), 1, hDispIcon, 32, 32, 0, NULL, DI_IMAGE|DI_MASK );
				}
			}
/*-------------*/
		}
	}
}

BOOL CIconOwnerButton::PreTranslateMessage(MSG* pMsg) 
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

void CIconOwnerButton::PreSubclassWindow()
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




LRESULT CIconOwnerButton::OnSetCheck(WPARAM wParam, LPARAM lParam)
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


LRESULT CIconOwnerButton::OnGetCheck(WPARAM wParam, LPARAM lParam)
{
	ASSERT(m_bIsCheckBox);
	return GetCheck();
} // End of OnGetCheck


DWORD CIconOwnerButton::SetCheck(int nCheck, BOOL bRepaint/* = TRUE*/)
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

int CIconOwnerButton::GetCheck()
{
	return m_nCheck;
} // End of GetCheck


BOOL CIconOwnerButton::OnBnClicked()
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

//-----------------------------------------------------------------------------
// 枠の中身を塗る ('12.10.01)
//-----------------------------------------------------------------------------
void CIconOwnerButton::DrawRect(CDC* pDC, CRect rc, COLORREF upClr, COLORREF dnClr )
{
	CBrush	upBrush, dnBrush;
	CPen	upPen, dnPen;

	// 上半分
	upBrush.CreateSolidBrush( upClr );
	upPen.CreatePen( PS_SOLID, 1, ::GetSysColor(COLOR_GRAYTEXT) );
	pDC->SelectObject( upBrush );
	pDC->SelectObject( upPen );
	pDC->RoundRect( rc, CPoint(5,5) );

	// 下半分
	CRect	halfRect;
	halfRect.top = rc.top + rc.Height()/2;
	halfRect.left = rc.left+1;
	halfRect.right = rc.right-1;
	halfRect.bottom = rc.bottom - 2;

	dnBrush.CreateSolidBrush( dnClr );
	pDC->FillRect( halfRect, &dnBrush );
}

//-----------------------------------------------------------------------------
// ボタンの表示タイプをセットする ('12.10.01)
//-----------------------------------------------------------------------------
int CIconOwnerButton::SetBtnDispStyle( EnumIdColorBtnStyle style )
{
	if( style != m_ColorBtnStyle ){
		m_ColorBtnStyle = style;
		UpdateWindow();
	}

	return 0;
}
