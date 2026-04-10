// BmpDspWnd.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBDinp.h"
#include "BmpDspWnd.h"

#include "BlockDragWnd.h"


#ifdef CLOSE
// 透明化
// From CPPDrawManager クラス( CPPToolTip v2.1, www.codeproject.com )
//
COLORREF PixelAlpha(COLORREF clrSrc, double src_darken, COLORREF clrDest, double dest_darken)
{
	return RGB (GetRValue (clrSrc) * src_darken + GetRValue (clrDest) * dest_darken, 
				GetGValue (clrSrc) * src_darken + GetGValue (clrDest) * dest_darken, 
				GetBValue (clrSrc) * src_darken + GetBValue (clrDest) * dest_darken);
	
} //End PixelAlpha

// 透明化
// From CPPDrawManager クラス( CPPToolTip v2.1, www.codeproject.com )
//
void AlphaBitBlt(HDC hDestDC, int nDestX, int nDestY, DWORD dwWidth, DWORD dwHeight, HDC hSrcDC, int nSrcX, int nSrcY, int percent /* = 100 */)
{
	_ASSERT ((NULL != hDestDC) || (NULL != hSrcDC));

	if (percent >= 100)
	{
		::BitBlt(hDestDC, nDestX, nDestY, dwWidth, dwHeight, hSrcDC, nSrcX, nSrcY, SRCCOPY);
		return;
	} //if

	HDC hTempDC = ::CreateCompatibleDC(hDestDC);
	if (NULL == hTempDC)
		return;
	
	//Creates Source DIB
	LPBITMAPINFO lpbiSrc;
	// Fill in the BITMAPINFOHEADER
	lpbiSrc = (LPBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER)];
	lpbiSrc->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbiSrc->bmiHeader.biWidth = dwWidth;
	lpbiSrc->bmiHeader.biHeight = dwHeight;
	lpbiSrc->bmiHeader.biPlanes = 1;
	lpbiSrc->bmiHeader.biBitCount = 32;
	lpbiSrc->bmiHeader.biCompression = BI_RGB;
	lpbiSrc->bmiHeader.biSizeImage = dwWidth * dwHeight;
	lpbiSrc->bmiHeader.biXPelsPerMeter = 0;
	lpbiSrc->bmiHeader.biYPelsPerMeter = 0;
	lpbiSrc->bmiHeader.biClrUsed = 0;
	lpbiSrc->bmiHeader.biClrImportant = 0;
	
	COLORREF* pSrcBits = NULL;
	HBITMAP hSrcDib = CreateDIBSection (
		hSrcDC, lpbiSrc, DIB_RGB_COLORS, (void **)&pSrcBits,
		NULL, NULL);
	
	if ((NULL != hSrcDib) && (NULL != pSrcBits))
	{
		HBITMAP hOldTempBmp = (HBITMAP)::SelectObject (hTempDC, hSrcDib);
		::BitBlt (hTempDC, 0, 0, dwWidth, dwHeight, hSrcDC, nSrcX, nSrcY, SRCCOPY);
		::SelectObject (hTempDC, hOldTempBmp);
		
		//Creates Destination DIB
		LPBITMAPINFO lpbiDest;
		// Fill in the BITMAPINFOHEADER
		lpbiDest = (LPBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER)];
		lpbiDest->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		lpbiDest->bmiHeader.biWidth = dwWidth;
		lpbiDest->bmiHeader.biHeight = dwHeight;
		lpbiDest->bmiHeader.biPlanes = 1;
		lpbiDest->bmiHeader.biBitCount = 32;
		lpbiDest->bmiHeader.biCompression = BI_RGB;
		lpbiDest->bmiHeader.biSizeImage = dwWidth * dwHeight;
		lpbiDest->bmiHeader.biXPelsPerMeter = 0;
		lpbiDest->bmiHeader.biYPelsPerMeter = 0;
		lpbiDest->bmiHeader.biClrUsed = 0;
		lpbiDest->bmiHeader.biClrImportant = 0;
		
		COLORREF* pDestBits = NULL;
		HBITMAP hDestDib = CreateDIBSection (
			hDestDC, lpbiDest, DIB_RGB_COLORS, (void **)&pDestBits,
			NULL, NULL);
		
		if ((NULL != hDestDib) && (NULL != pDestBits))
		{
			::SelectObject (hTempDC, hDestDib);
			::BitBlt (hTempDC, 0, 0, dwWidth, dwHeight, hDestDC, nDestX, nDestY, SRCCOPY);
			::SelectObject (hTempDC, hOldTempBmp);

			double src_darken = (double)percent / 100.0;
			double dest_darken = 1.0 - src_darken;
			
			for (DWORD pixel = 0; pixel < dwWidth * dwHeight; pixel++, pSrcBits++, pDestBits++)
			{
				*pDestBits = PixelAlpha(*pSrcBits, src_darken, *pDestBits, dest_darken);
			} //for

			::SelectObject (hTempDC, hDestDib);
			::BitBlt (hDestDC, nDestX, nDestY, dwWidth, dwHeight, hTempDC, 0, 0, SRCCOPY);



DWORD dwSize,dwFSize,dwLength;
HANDLE fh;
LPBITMAPFILEHEADER lpHead;
LPBITMAPINFOHEADER lpInfo;
LPBYTE lpBuf,lpPixel;
HDC hdc,hdcMem;
HBITMAP hBMP,hOld;

if ((dwWidth*3) % 4==0) /* バッファの１ラインの長さを計算 */
  dwLength=dwWidth*3;
else
  dwLength=dwWidth*3+(4-(dwWidth*3) % 4);

/* 書き込み用バッファのサイズ計算 */
dwFSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+dwLength*dwHeight;

/* バッファ確保とポインタ設定 */
lpBuf=(LPBYTE)GlobalAlloc(GPTR,dwFSize);
lpHead=(LPBITMAPFILEHEADER)lpBuf;
lpInfo=(LPBITMAPINFOHEADER)(lpBuf+sizeof(BITMAPFILEHEADER));
lpPixel=lpBuf+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

/* 24ビットBMPファイルのヘッダ作成 */
lpHead->bfType='M'*256+'B';
lpHead->bfSize=dwFSize;
lpHead->bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
lpInfo->biSize=sizeof(BITMAPINFOHEADER);
lpInfo->biWidth=dwWidth;
lpInfo->biHeight=dwHeight;
lpInfo->biPlanes=1;
lpInfo->biBitCount=24;

/* ウインドウのデバイスコンテキスト取得 */
//hdc=GetDC(hwnd);
/* ウインドウのデバイスコンテキスト互換のBITMAP作成 */
//hBMP=CreateCompatibleBitmap(hdc,dwWidth,dwHeight);
/* BITMAPにウインドウのクライアント領域をコピー */
//hdcMem=CreateCompatibleDC(hdc);
//hOld=(HBITMAP)SelectObject(hdcMem,hBMP);
//BitBlt(hdcMem,0,0,dwWidth,dwHeight,hdc,0,0,SRCCOPY);
//SelectObject(hdcMem,hOld);
GetDIBits(hDestDC,hDestDib,0,dwHeight,lpPixel,(LPBITMAPINFO)lpInfo,DIB_RGB_COLORS);

//ReleaseDC(hwnd,hdc);
//DeleteObject(hBMP);
//DeleteObject(hdcMem);

/* バッファをファイルに書き出す */
fh=CreateFile("E:\\sv1.bmp",GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
WriteFile(fh,lpBuf,dwFSize,&dwSize,NULL);
CloseHandle(fh);

GlobalFree(lpBuf);


			::SelectObject (hTempDC, hOldTempBmp);

			delete lpbiDest;
			::DeleteObject(hDestDib);
		} //if
		delete lpbiSrc;
		::DeleteObject(hSrcDib);
	} //if

	::DeleteDC(hTempDC);
} //End AlphaBitBlt

#endif



// CBmpDspWnd

IMPLEMENT_DYNAMIC(CBmpDspWnd, CWnd)

CBmpDspWnd::CBmpDspWnd( CWnd* pParent )
{
	m_pParentWnd	= pParent;
	m_nBmpID		= 0;
	m_bSelect		= FALSE;

	m_Back = GetSysColor( COLOR_3DFACE );
//	m_Waku = RGB(255,0,128);
	m_Waku = RGB(255,102,255);

	CursorChangeFlg() = TRUE;
	m_hHandCursor = AfxGetApp()->LoadStandardCursor(IDC_HAND);

	m_bMouseOver = FALSE;
}

CBmpDspWnd::~CBmpDspWnd()
{
}


BEGIN_MESSAGE_MAP(CBmpDspWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
//	ON_WM_NCMOUSEMOVE()
END_MESSAGE_MAP()


void CBmpDspWnd::SetBitmapID( int bmpid )
{
	m_nBmpID = bmpid;
}

void  CBmpDspWnd::SetDispBitmap( CBitmap* pBmp, CRect bmRect, CString& txt )
{
	m_pDispBmp = pBmp;
	m_bmRect.CopyRect( bmRect );

	m_Txt = txt;
}


void CBmpDspWnd::PaintBmpSub( CPaintDC* pDC )
{
	pDC->SetStretchBltMode( HALFTONE );
	CRect	rc;
	GetClientRect( rc );

	CBrush BR;
	BR.CreateSolidBrush( m_Back );
	pDC->FillRect( &rc, &BR ); // 四角形 RE を、ブラシ BR で描く

	CRect rectDisplay, rectTitle;

	GetWindowRect( rectDisplay );
	rectTitle = rectDisplay;
	rectTitle.SetRectEmpty();

	CBitmap	bitmap;
	CDC MemDC;
	CBitmap *pTmpBitmap;

	if( m_nBmpID != 0 ) {
		bitmap.LoadBitmap( m_nBmpID );
		MemDC.CreateCompatibleDC( pDC );
		pTmpBitmap = MemDC.GetCurrentBitmap();
		MemDC.SelectObject(&bitmap);

//		pDC->BitBlt( -1, 0, rectDisplay.Width(), rectDisplay.Height(), &MemDC, 0, 0, SRCCOPY);
		BITMAP bi;
		bitmap.GetBitmap( &bi );
		pDC->StretchBlt( 0, 0, rectDisplay.Width(), rectDisplay.Height(), &MemDC, 0, 0, bi.bmWidth, bi.bmHeight, SRCCOPY);
		MemDC.SelectObject(pTmpBitmap);
	}
	else if( m_pDispBmp != NULL ) {
		MemDC.CreateCompatibleDC( pDC );
		pTmpBitmap = MemDC.GetCurrentBitmap();
		MemDC.SelectObject( m_pDispBmp );

		BITMAP bi;
		m_pDispBmp->GetBitmap( &bi );

		pDC->StretchBlt( 0, 0, rectDisplay.Width(), rectDisplay.Height(), &MemDC, 0, m_bmRect.top, 
																					bi.bmWidth, m_bmRect.Height(), SRCCOPY);

		MemDC.SelectObject(pTmpBitmap);
	}
	else {
		// テキストで表現
		COLORREF old;
		old = pDC->SetBkColor(m_Back);

		CRect rc;
		GetClientRect( &rc );

		int h, w, xpos, ypos;

		h = rc.Height();
		w = rc.Width();
		CSize sz = pDC->GetTextExtent( m_Txt );

		ypos = h/2;
		if( (ypos+sz.cy) > h ) ypos = 0;

		xpos = (w/3);
		if( (xpos+sz.cx) > w ) xpos = 0;

		pDC->TextOut( xpos, ypos, m_Txt );

		old = pDC->SetBkColor(old);
	}
}


// CBmpDspWnd メッセージ ハンドラ
void CBmpDspWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	// 描画メッセージで CWnd::OnPaint() を呼び出さないでください。

	PaintBmpSub( &dc );

//	CString str;
//	GetWindowText( str );
//	dc.TextOut( 0,20,str );
	CBrush wakuBrush;
	CRect	rc;
	GetClientRect( rc );

	if( m_bSelect ) {
		wakuBrush.CreateSolidBrush( m_Waku );

		dc.DrawEdge( rc, EDGE_RAISED, BF_RECT );
		dc.FrameRect( rc, &wakuBrush );
	}
	else {
		if( m_bMouseOver ) {
			wakuBrush.CreateSolidBrush( RGB(255,102,0) );
		}
		else {
			wakuBrush.CreateSolidBrush( RGB(128,128,128) );
		}
		dc.FrameRect( rc, &wakuBrush );
	//	dc.DrawEdge( rc, EDGE_BUMP, BF_RECT );
	//	dc.FrameRect( rc, &BR );
	}

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

void CBmpDspWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	ClientToScreen( &point );
	LPARAM lpa = MAKELPARAM( point.y, point.x );

	if( m_bMouseOver ) {
		ReleaseCapture();
		m_bMouseOver = FALSE;
	}

	m_pParentWnd->SendMessage( BDW_MESSAGE, BDWM_LBUTTONDOWN, lpa );

	CWnd::OnLButtonDown(nFlags, point);
}

void CBmpDspWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	CRect rect;
	GetClientRect(rect);

	if(PtInRect(rect, point)){
		if( ! m_bMouseOver ) {
			if( CursorChangeFlg() ) {
				SetCapture();
				m_bMouseOver = TRUE;
				RedrawWindow();
			}
		}
	}
	else {
		if( m_bMouseOver ) {
			if( CursorChangeFlg() ) {
				ReleaseCapture();
				m_bMouseOver = FALSE;
				RedrawWindow();
			}
		}
	}
	if( m_hHandCursor && CursorChangeFlg() ) {
		SetCursor( m_hHandCursor );
	}
	else {
	}
	CWnd::OnMouseMove(nFlags, point);
}

//void CBmpDspWnd::OnNcMouseMove(UINT nHitTest, CPoint point)
//{
//	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
//	if( m_bMouseOver ) {
//		m_bMouseOver = FALSE;
//		Invalidate();
//		UpdateWindow();
//	}
//
//	CWnd::OnNcMouseMove(nHitTest, point);
//}
