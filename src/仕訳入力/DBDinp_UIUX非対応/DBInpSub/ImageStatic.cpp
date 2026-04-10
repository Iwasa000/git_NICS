// ImageStatic.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "ImageStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ImageStatic

ImageStatic::ImageStatic()
{
	pImage	=	NULL;
	clBack	=	RGB(255,255,255);
	clFore	=	RGB(0,0,0);
	clFocus	=	::GetSysColor( COLOR_ACTIVECAPTION );
	clFocus	=	RGB(0,255,0);
	clTrans=	RGB(255,255,128);
	sgnDel	=	0;
	sgnPaint	=	0;
	flgFocus	=	FALSE;
	flgTrans	=	FALSE;
	textImage.Empty();
}

ImageStatic::~ImageStatic()
{
	if( pImage )	delete	[]pImage;
}


//
//	背景色をセットする( mode = TRUE: でフォーカスあり )
//
void ImageStatic::SetBackColor( int mode )
{
	if( mode == FALSE )	clBack	=	RGB(255,255,255);
	else				clBack	=	RGB(224,255,255);
	Invalidate();
	UpdateWindow();
}

//
//	背景色をセットする( mode = TRUE: 転送あるいは、確定仕訳 )
//
void ImageStatic::SetBackTrans( int mode, COLORREF trans_back )
{
	flgTrans	=	mode;
	clTrans		=	trans_back;
	Invalidate();
	UpdateWindow();
}

void ImageStatic::DeleteLine( int mode )
{
	sgnDel	=	mode;
}

void ImageStatic::ImagePaintSw( int mode )
{
	sgnPaint	=	mode;
	if( !sgnPaint )	Invalidate();
	if( !sgnPaint )	UpdateWindow();
}

void ImageStatic::ClearImage()
{
	width	=	0;
	height	=	0;
	lImage	=	0;
	if( pImage )	delete	[]pImage;
	pImage	=	NULL;
}

void ImageStatic::SetImage( short x, short y, int lng, void* p )
{
//	if( pImage )	return;
	if( pImage ) {
		delete	[]pImage;
		pImage	=	NULL;
	}

	if( p == NULL ) {
		textImage	=	_T("＜イメージ摘要＞");
		return;
	}
	
	textImage.Empty();
	lImage	=	lng;
	pImage	=	new char[lImage];
	memmove( pImage, p, lImage );
	width	=	x;
	height	=	y;

	for( int i = 0; i < lImage; i++ )	*(pImage + i) = ~(*(pImage + i));
}

void ImageStatic::ImageDisplay( CDC* pDC )
{
	int	color	=	pDC->GetDeviceCaps( BITSPIXEL );
	if( color <= 8 ) {
		if( clBack == RGB(224,255,255) ) {
			clBack	=	RGB(0,255,255);
		}
		clTrans=	RGB(255,255,0);
	}
	else {
		if( clBack == RGB(0,255,255) ) {
			clBack	=	RGB(224,255,255);
		}
	//	clTrans	=	RGB(255,255,128);
	//	clTrans	=	GetConfirmBkColor();
	}

	CRect	rect;
	GetClientRect( &rect );
	if( flgFocus )
		pDC->FillSolidRect( rect, clFocus );
	else if( flgTrans )
		pDC->FillSolidRect( rect, clTrans );
	else
		pDC->FillSolidRect( rect, clBack );

	if( pImage == NULL ) {
		if( !textImage.IsEmpty() ) {
			pDC->SetTextColor( clFore );
			if( flgFocus ) {
				pDC->SetBkColor( clFocus );
			}
			else if( flgTrans )
				pDC->SetBkColor( clTrans );
			else
				pDC->SetBkColor( clBack );
			pDC->TextOut( 0, 0, textImage );
		}
		return;
	}

	CDC*		pDMDC	=	new CDC;
	CBitmap*	pBitmap	=	new CBitmap;
	CBitmap*	pOldBitmap;
	BITMAP		bt;
	POINT		pt, ptc;
	RECT		rt;
	char*		p	=	pImage;

	pDC->SetTextColor( clFore );
	if( flgFocus ) {
		pDC->SetBkColor( clFocus );
	}
	else if( flgTrans )
		pDC->SetBkColor( clTrans );
	else
		pDC->SetBkColor( clBack );
	pBitmap->CreateBitmap( (int)width, (int)height, 1, 1, (const void*)pImage );

	pDMDC->CreateCompatibleDC( pDC );
	pDMDC->SetTextColor( clFore );
	if( flgFocus ) {
		pDMDC->SetBkColor( clFocus );
	}
	else if( flgTrans )
		pDMDC->SetBkColor( clTrans );
	else
		pDMDC->SetBkColor( clBack );
	pOldBitmap	=	(CBitmap*)pDMDC->SelectObject( pBitmap );
			
	pBitmap->GetObject( sizeof(BITMAP), &bt );
	pt.x = bt.bmWidth;
	pt.y = bt.bmHeight;
	GetClientRect( &rt );
	ptc.x = rt.right - rt.left;
	ptc.y = rt.bottom - rt.top;
	double	w	=	(double)ptc.y / (double)pt.y;
	ptc.x	=	(int)((double)pt.x * w);
	pDC->StretchBlt( 0, 0, ptc.x, ptc.y, pDMDC, 0, 0, pt.x, pt.y, SRCCOPY );

	if( sgnDel ) {
		CRect	rect	=	rt;
		rect.top	=	rect.top + rect.Height() / 2 - 1;
		rect.bottom	=	rect.top + 2;
		pDC->FillSolidRect( rect, RGB(255,0,0) );
	}

	delete pDMDC->SelectObject( pOldBitmap );
	delete pDMDC;
}


BEGIN_MESSAGE_MAP(ImageStatic, CStatic)
	//{{AFX_MSG_MAP(ImageStatic)
	ON_WM_PAINT()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ImageStatic メッセージ ハンドラ

void ImageStatic::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	
	if( !sgnPaint ) {
		CDC*	pDC	=	GetDC();
		ImageDisplay( pDC );

		ReleaseDC( pDC );
	}
}

void ImageStatic::OnKillFocus(CWnd* pNewWnd) 
{
	CStatic::OnKillFocus(pNewWnd);
//	clFore	=	RGB(0,0,0);
	flgFocus	=	FALSE;

	Invalidate();
	UpdateWindow();

	GetParent()->PostMessage( MESSAGE_IMGST_FOCUS, (WPARAM)GetDlgCtrlID(), -1 );
}

void ImageStatic::OnSetFocus(CWnd* pOldWnd) 
{
	CStatic::OnSetFocus(pOldWnd);
//	clFore	=	RGB(255,255,255);
	flgFocus	=	TRUE;

	Invalidate();
	UpdateWindow();

	GetParent()->PostMessage( MESSAGE_IMGST_FOCUS, (WPARAM)GetDlgCtrlID(), 0 );
}

void ImageStatic::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CStatic::OnLButtonDown(nFlags, point);
	SetFocus();
}

BOOL ImageStatic::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN ) {
		GetParent()->PostMessage( MESSAGE_IMGST_KEY, (WPARAM)GetDlgCtrlID(), pMsg->wParam );
		return	1;
	}
	return CStatic::PreTranslateMessage(pMsg);
}


void ImageStatic::PreSubclassWindow()
{
	ModifyStyle( 0, WS_CLIPSIBLINGS );

	CStatic::PreSubclassWindow();
}
