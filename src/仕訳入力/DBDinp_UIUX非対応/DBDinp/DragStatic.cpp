// DragStatic.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBDinp.h"
#include "DragStatic.h"

#include "BlockDragWnd.h"

// CDragStatic

IMPLEMENT_DYNAMIC(CDragStatic, CBmpDspWnd)

CDragStatic::CDragStatic(CWnd* pParent)
		: CBmpDspWnd( pParent )
{
	m_nOldMouseX			= 0;
	m_nOldMouseY			= 0;
//	m_nFadeWhileDragging	= NOFADE;
	m_nFadeWhileDragging	= 128;
	m_bDrag					= FALSE;
	m_bDragLeft				= TRUE;
	m_bDragRight			= FALSE;
//	m_bDragLeft				= FALSE;
//	m_bDragRight			= TRUE;
//	m_bKeepDialogInScreen	= FALSE;
	m_bKeepDialogInScreen	= TRUE;
	m_bUseDragCursor		= FALSE;
	m_hCursor				= NULL;

	m_bBkTransMode = TRUE;
	m_pParentWnd = pParent;

	UpdateScreenMetrics();

	m_pRect = NULL;

	m_Back = RGB(0,0,0);

	m_nBmpID = 0;

	//CBmpDspWnd フラグ無効(移動用のカーソルに変更するため)
	m_bCursor = FALSE;
}

CDragStatic::~CDragStatic()
{
}



BEGIN_MESSAGE_MAP(CDragStatic, CBmpDspWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
//	ON_WM_DRAWITEM()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDragStatic メッセージ ハンドラ
BOOL CDragStatic::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	switch(pMsg->message)
	{
	case WM_LBUTTONUP:
		if(m_bDrag && m_bDragLeft)	//if leftmousebutton is up, the user is currently dragging and
									//the dragging is set to the left mousebutton
			DeactivateDrag();

TRACE( "---> @PreTrans WM_LBUTTONUP\n" );
		break;
	case WM_RBUTTONUP:
		if(m_bDrag && m_bDragRight)	//if rightmousebutton is up, the user is currently dragging and
									//the dragging is set to the right mousebutton
			DeactivateDrag();
		break;
	default:
		break;
	}

	return CBmpDspWnd::PreTranslateMessage(pMsg);
}

void CDragStatic::OnLButtonDown(UINT nFlags, CPoint point)
{
//	if (m_bDragLeft)
//		ActivateDrag(); //if dragging is set to the left mousebutton, dragging will be activated

	CBmpDspWnd::OnLButtonDown(nFlags, point);
}

void CDragStatic::OnRButtonDown(UINT nFlags, CPoint point)
{
//	if (m_bDragRight)
//		ActivateDrag(); //if dragging is set to the right mousebutton, dragging will be activated

	CBmpDspWnd::OnRButtonDown(nFlags, point);
}

void CDragStatic::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bDrag) //If dragging is activated
	{
		WINDOWPLACEMENT placement;
		if (!GetWindowPlacement(&placement)) //Get placement of the dialog
			return;
		int nTop = placement.rcNormalPosition.top;
		int nBottom = placement.rcNormalPosition.bottom;
		int nLeft = placement.rcNormalPosition.left;
		int nRight = placement.rcNormalPosition.right;

		CPoint ptGlobPos;
		if (!GetCursorPos(&ptGlobPos)) //Get coordinates of the mouse (complete screen)
			return;
			
		int nDiffX = ptGlobPos.x - m_nOldMouseX;	//Calculate the difference between old 
													//and new X pos of the mouse.
		
		int nDiffY = ptGlobPos.y - m_nOldMouseY;	//Calculate the difference between old 
													//and new Y pos of the mouse.

		nLeft = nLeft + nDiffX; //Calculate the new windowposition
		nTop = nTop + nDiffY;
		nRight = nRight + nDiffX;
		nBottom = nBottom + nDiffY;

		int nWidth	= nRight-nLeft; //Calculate the width and height of the window
		int nHeight	= nBottom-nTop;

		CRect rect;
		m_pParentWnd->GetWindowRect( rect );

		if (m_bKeepDialogInScreen)	//If dialog should stay in screen the coordinates would be
									//corrected if necessary
		{
			if (nLeft< m_nCtrlX)
				nLeft = m_nCtrlX;
			if (nRight > rect.right )
				nLeft = rect.right - nWidth;
			if (nTop < (m_nCtrlY - nHeight +10) )
				nTop = (m_nCtrlY - nHeight +10);
			if (nBottom > rect.bottom )
				nTop = (m_nCtrlY + rect.Height() ) - nHeight;
		}

		m_nOldMouseX = ptGlobPos.x; //Save actual Mousepos for the next call of OnMouseMove()
		m_nOldMouseY = ptGlobPos.y;

		MoveWindow(nLeft, nTop, nWidth, nHeight, TRUE); //And finally move the window to new coordinates.

		m_lPara = MAKELPARAM( ptGlobPos.y, ptGlobPos.x );
		m_pParentWnd->SendMessage( BDW_MESSAGE, BDWM_MOUSEMOVE, m_lPara );
	}

	CBmpDspWnd::OnMouseMove(nFlags, point);
}



void CDragStatic::SetDragButtons(BOOL bDragLeft, BOOL bDragRight) //Set the drag-mousebutton(s)
{
	m_bDragLeft = bDragLeft;
	m_bDragRight = bDragRight;
}

BOOL CDragStatic::ActivateDrag()
{
	if (m_bKeepDialogInScreen) //If the dialog should stay in screen, we update the ScreenMetrics
		UpdateScreenMetrics();
	
	CPoint ptGlobPos;
	if (!GetCursorPos(&ptGlobPos)) //Get the mouseposition
		return FALSE;

	m_nOldMouseX = ptGlobPos.x;
	m_nOldMouseY = ptGlobPos.y;

	SetCapture(); //Get Mousecapture

	if (m_bUseDragCursor) //Should be used another mousecursor for dragging? Yes? DrawIt!
		DrawDragCursor();

	CRect rect;
	GetWindowRect( rect );

	// トップレベルウィンドウにする場合のスタイルの設定
	::SetParent(m_hWnd, NULL);

	DWORD lStyle = GetStyle();
	ModifyStyle(WS_CHILD, WS_POPUP);

	// トップレベルウィンドウにすると移動するので、現在の位置に
	MoveWindow( rect );

	m_bDrag = TRUE; //Dragging is now activated

	if (m_nFadeWhileDragging != NOFADE) //If fading is activated, fade the Dialog
		FadeDialog(TRUE);

	Invalidate();
	UpdateWindow();

	return TRUE;
}

BOOL CDragStatic::DeactivateDrag()
{
	m_bDrag = FALSE; // Dragging is deactivated

	m_pRect = NULL;
	
//	if (!ReleaseCapture()) //Release Mousecapture
//		return FALSE;
	ReleaseCapture();

	CRect rect;
	GetWindowRect( rect );

	if (m_nFadeWhileDragging != NOFADE) //If fading was activated, draw the dialog normal
		FadeDialog(FALSE);

#ifdef CLOSE
	// トップレベルウィンドウ -> 子ウィンドウにする
	DWORD lStyle = GetStyle();
	ModifyStyle( WS_POPUP, WS_CHILD );

	::SetParent(m_hWnd, m_pParentWnd->m_hWnd);
	SetOwner(m_pParentWnd);

//	m_pParentWnd->ScreenToClient( rect );
	SetWindowPos( &wndNoTopMost, 0,0,100,100, SWP_SHOWWINDOW );
//	MoveWindow( 0,0,100,100 );
	ShowWindow(SW_SHOW);
	GetWindowRect( rect );
	m_pParentWnd->ScreenToClient( rect );
TRACE( "L=%d, T=%d, R=%d, B=%d\n", rect.left, rect.top, rect.right, rect.bottom );
#endif

	CPoint pt;
	GetCursorPos( &pt );
	m_lPara = MAKELPARAM( pt.y, pt.x );
	m_pParentWnd->PostMessage( BDW_MESSAGE, BDWM_DEACTIVEDRAG, m_lPara );

	return TRUE;
}

void CDragStatic::KeepDialogInScreen(BOOL bKeepDialogInScreen)
{
	m_bKeepDialogInScreen = bKeepDialogInScreen;
}

void CDragStatic::UpdateScreenMetrics()
{
//	m_nScreenX = GetSystemMetrics(SM_CXSCREEN); // You get the width of the users monitor
//	m_nScreenY = GetSystemMetrics(SM_CYSCREEN); // You get the height of the users monitor

	CRect rect;
	if( m_pParentWnd != NULL ) {
		m_pParentWnd->GetWindowRect( &rect );

		m_nCtrlX = rect.left;
		m_nCtrlY = rect.top;
	}
}


void CDragStatic::UseDragCursor(BOOL bUseDragCursor)
{
	m_bUseDragCursor = bUseDragCursor;
}

BOOL CDragStatic::SetDragCursor(UINT nID)
{
//	m_hCursor = AfxGetApp()->LoadCursor(nID); //Load the new dragcursor to an HCURSOR variable
	m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_SIZEALL); //Load the new dragcursor to an HCURSOR variable
	if (!m_hCursor)
		return FALSE;
	UseDragCursor(TRUE);
	return TRUE;
}

void CDragStatic::DrawDragCursor()
{
	if (m_hCursor)
		SetCursor(m_hCursor);
}

BOOL CDragStatic::SetFadeWhileDragging(int nFade)
{
	if ((LOBYTE(LOWORD(GetVersion()))) <= 4)
	{
		m_nFadeWhileDragging = NOFADE;
		return FALSE; // 'Cause Windows versions lower than 5 don't support fading
	}
	if (nFade < 0 || nFade > NOFADE)
		return FALSE; // You can only fade between 0 and 255
	m_nFadeWhileDragging = nFade;
	return TRUE;
}

// SetLayeredWindowAttributes function prototype
typedef BOOL (WINAPI *lpfnSetLayeredWindowAttributes)
(
	HWND hwnd,	
	COLORREF crKey,	
	BYTE bAlpha,	
	DWORD dwFlags
);


// WS_CHILD のウィンドウは 半透明にならないため、WS_POPUPにする
void CDragStatic::FadeDialog(BOOL bFade)
{
	int nFade = m_nFadeWhileDragging;
	if (!bFade)
		nFade = NOFADE;

	lpfnSetLayeredWindowAttributes pSetLayeredWindowAttributes;
	pSetLayeredWindowAttributes=NULL;
	// Function for fading is in USER32.DLL
	HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
	if (!hUser32)
		return;
	pSetLayeredWindowAttributes = (lpfnSetLayeredWindowAttributes)GetProcAddress(hUser32, "SetLayeredWindowAttributes");    
	
	if (nFade >= NOFADE)
		SetWindowLong(*this, GWL_EXSTYLE, GetWindowLong(*this, GWL_EXSTYLE));
	else
		SetWindowLong(*this, GWL_EXSTYLE, GetWindowLong(*this, GWL_EXSTYLE) | WS_EX_LAYERED);
	// Call the function to fade the dialog.
	pSetLayeredWindowAttributes(*this, 0, nFade, ULW_ALPHA);
}



//void CDragStatic::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
//{
//	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
//
//	CBmpDspWnd::OnDrawItem(nIDCtl, lpDrawItemStruct);
//}

void CDragStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	// 描画メッセージで CBmpDspWnd::OnPaint() を呼び出さないでください。

	PaintBmpSub( &dc );

//*/
//#ifdef CLOSE
//#ifndef TITLE_TIP_BACKGROUND_NO_TRANSPARENT //背景の透過
//		CRect bm_rect;
//		CDC* pMemDC, *pParentDC;
//
//		if( m_bBkTransMode && m_bDrag ) {
//			pMemDC = new CDC;
//			pParentDC = m_pParentWnd->GetDC();
//			pMemDC->CreateCompatibleDC( pParentDC );
//			CBitmap bitmap, *pOldbitmap;
//			bitmap.CreateCompatibleBitmap(pParentDC, rect.Width(), rect.Height());
//			pOldbitmap = (CBitmap*)pMemDC->SelectObject( &bitmap );
//			bm_rect = rectDisplay;
//			m_pParentWnd->ScreenToClient( bm_rect );
//
//			pMemDC->BitBlt( 0, 0, rectDisplay.Width(), rectDisplay.Height(),
//						pParentDC, bm_rect.left, bm_rect.top, SRCCOPY);
//			
//			pParentDC->SelectObject(pOldbitmap);
//			m_pParentWnd->ReleaseDC( pParentDC );
//		}
//#endif
//
//#ifndef TITLE_TIP_BACKGROUND_NO_TRANSPARENT
//		if( m_bBkTransMode && m_bDrag ) {
//			rect.SubtractRect( rectDisplay, rectTitle );
//			// 枠線分 -1 している
//			if( rectDisplay.left < rectTitle.left ) {
//				AlphaBitBlt( dc.m_hDC, -1, -1, rect.Width(), rect.Height(), pMemDC->m_hDC, 0, 0, 70 );
//			}
//			else {
//				int n = rectTitle.Width();
//				AlphaBitBlt( dc.m_hDC, n-1, -1, rect.Width(), rect.Height(), pMemDC->m_hDC, n, 0, 70 );
//			}
//	//		dc.BitBlt( -1, 0, rectDisplay.Width(), rectDisplay.Height(), pMemDC, 0, 0, SRCCOPY);
//			delete pMemDC;
//		}
//#endif
//
//#endif
}




