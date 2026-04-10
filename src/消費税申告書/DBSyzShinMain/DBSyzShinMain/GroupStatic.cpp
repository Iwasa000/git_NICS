// H2Static.cpp : 実装ファイル
//

#include "stdafx.h"
//#include "CoSetDB.h"
#include "GroupStatic.h"


// CGroupStatic

IMPLEMENT_DYNAMIC(CGroupStatic, CStatic)

CGroupStatic::CGroupStatic()
{
	// 色情報取得
	if ( ((ICSWinApp*)AfxGetApp())->GetStanderdColor( m_cinf ) != 0 ) {
		m_cinf.m_swOnOff = 0;
	}

	m_frame = HSTATIC_COLOR_FRAME;
	m_bcolor = HSTATIC_COLOR_BCOLOR;
	m_ecolor = HSTATIC_COLOR_ECOLOR;

	m_tbcolor = HSTATIC_COLOR_TBCOLOR;
	m_tecolor = HSTATIC_COLOR_TECOLOR;

	m_elsize = 30;
}

CGroupStatic::~CGroupStatic()
{
}


BEGIN_MESSAGE_MAP(CGroupStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CGroupStatic メッセージ ハンドラ



void CGroupStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	// 描画メッセージで CStatic::OnPaint() を呼び出さないでください。

	// 描画領域の取得
    RECT rect;
    GetWindowRect( &rect );
	ScreenToClient( &rect );

	// 背景色の取得・セット
	if ( m_cinf.m_swOnOff == 1 ) {
		CBrush brush;
		brush.CreateSolidBrush( m_cinf.ViewColor() );
		dc.FillRect( &rect, &brush );
		brush.DeleteObject();
	}

	// フォントサイズ取得
	LOGFONT fnt = { 0 }, ofnt = { 0 };
	CFont *pFont = NULL, *pOldFont = NULL, font;
	pFont = GetFont();

	if ( pFont != NULL ) {
		pFont->GetLogFont( &fnt );
		fnt.lfWeight |= FW_BOLD;
		m_thsize = abs(fnt.lfHeight)+(abs(fnt.lfHeight)/2);
		font.CreateFontIndirect( &fnt );
		pOldFont = (CFont*)dc.SelectObject( &font );
	}

	// ブラシ作成
	CBrush *pOldBrush, flame_brush;
	flame_brush.CreateSolidBrush( m_frame );
	pOldBrush = dc.GetCurrentBrush();

	// タイトル部作成
	if ( m_title.IsEmpty() == FALSE ) {
		CRgn rgn, rgnl, rgnr;
		CBrush brushl, brushr;
		CRect rect1;

		rgn.CreateRoundRectRgn( rect.left, rect.top, rect.right, m_thsize*2, m_elsize, m_elsize );
		rgnl.CreateRoundRectRgn( rect.left, rect.top, rect.right/2, m_thsize*2, m_elsize, m_elsize );
		rgnr.CreateRoundRectRgn( (rect.right/2+1), rect.top, rect.right, m_thsize*2, m_elsize, m_elsize );

		brushl.CreateSolidBrush( m_tbcolor );
		dc.SelectObject( brushl );
		dc.PaintRgn( &rgnl );
		brushl.DeleteObject();

		brushr.CreateSolidBrush( m_tecolor );
		dc.SelectObject( brushr );
		dc.PaintRgn( &rgnr );
		brushr.DeleteObject();

		rect1.left = rect.left+(m_elsize/2);
		rect1.right = rect.right-(m_elsize/2);
		rect1.top = rect.top;
		rect1.bottom = rect.top+(m_thsize*2)-1;
		lc_GradientFill( dc.m_hDC, &rect1, m_tbcolor, m_tecolor, 1 );

		dc.FrameRgn( &rgn, &flame_brush, 1, 1 );
	}

	// タイトル文字列作成
	if ( m_title.IsEmpty() == FALSE ) {
		CRect textrect;
		textrect.CopyRect( &rect );
		textrect.left += (m_elsize/2+5);
		textrect.bottom = m_thsize;
		dc.SetBkMode( TRANSPARENT );
		dc.DrawText( m_title, textrect, (DT_VCENTER|DT_SINGLELINE) );

		if ( pFont != NULL )
			font.DeleteObject();
	}

	// データ部作成
	if ( m_title.IsEmpty() == FALSE ) {
		CRgn rgn, rgnb;
		CBrush brushb;
		CRect rect1;

		rgn.CreateRoundRectRgn( rect.left, rect.top, rect.right, rect.bottom, m_elsize, m_elsize );
		rgnb.CreateRoundRectRgn( rect.left, rect.bottom-m_thsize*2, rect.right, rect.bottom, m_elsize, m_elsize );

		brushb.CreateSolidBrush( m_ecolor );
		dc.SelectObject( brushb );
		dc.PaintRgn( &rgnb );
		brushb.DeleteObject();

		rect1.left = rect.left;
		rect1.right = rect.right-1;
		rect1.top = rect.top+m_thsize;
		rect1.bottom = rect.bottom-m_elsize;
		lc_GradientFill( dc.m_hDC, &rect1, m_bcolor, m_ecolor, 0 );

		dc.FrameRgn( &rgn, &flame_brush, 1, 1 );
	}
	else {
		CRgn rgn, rgnl, rgnr;
		CBrush brushl, brushr;
		CRect rect1;

		m_bcolor = HSTATIC_COLOR_DBLUE;
		m_ecolor = HSTATIC_COLOR_DBWHITE;

		rgn.CreateRoundRectRgn( rect.left, rect.top, rect.right, rect.bottom, m_elsize, m_elsize );
		rgnl.CreateRoundRectRgn( rect.left, rect.top, rect.right/2, rect.bottom, m_elsize, m_elsize );
		rgnr.CreateRoundRectRgn( (rect.right/2+1), rect.top, rect.right, rect.bottom, m_elsize, m_elsize );

		brushl.CreateSolidBrush( m_bcolor );
		dc.SelectObject( brushl );
		dc.PaintRgn( &rgnl );
		brushl.DeleteObject();

		brushr.CreateSolidBrush( m_ecolor );
		dc.SelectObject( brushr );
		dc.PaintRgn( &rgnr );
		brushr.DeleteObject();

		rect1.left = rect.left+(m_elsize/2);
		rect1.right = rect.right-(m_elsize/2);
		rect1.top = rect.top;
		rect1.bottom = rect.bottom-1;
		lc_GradientFill( dc.m_hDC, &rect1, m_bcolor, m_ecolor, 1 );

		dc.FrameRgn( &rgn, &flame_brush, 1, 1 );
	}

	// タイトル部区切り線
	CPen* pOldPen = dc.GetCurrentPen();
	if ( m_title.IsEmpty() == FALSE ) {
		LOGPEN lpen = { 0 };
		CPen pen;
		pOldPen->GetLogPen( &lpen );
		lpen.lopnColor = m_frame;
		pen.CreatePenIndirect( &lpen );
		(CPen*)dc.SelectObject( &pen );
		dc.MoveTo( rect.left, rect.top+m_thsize );
		dc.LineTo( rect.right, rect.top+m_thsize );
		pen.DeleteObject();
	}

	// オブジェクトを元に戻す
	if ( pOldFont != NULL )
		dc.SelectObject( pOldFont );

	if ( pOldBrush != NULL )
		dc.SelectObject( pOldBrush );

	if ( pOldPen != NULL )
		dc.SelectObject( pOldPen );

	return;
}

//-----------------------------------------------------------------------------------
// 指定領域をグラデーションで塗りつぶし
//		const HDC&		hdc				デバイスコンテキスト
//		const CRect&	rect			指定領域
//		COLOR16			r,g,b			開始ＲＧＢ色
//		COLOR16			r2,g2,b2		終了ＲＧＢ色
//		BOOL			is_horizontal	0	縦方向
//										1	横方向
//-----------------------------------------------------------------------------------
void CGroupStatic::lc_GradientFill(const HDC& hdc, const CRect& rect, COLOR16 r, COLOR16 g, COLOR16 b, COLOR16 r2, COLOR16 g2, COLOR16 b2, BOOL is_horizontal)
{
	TRIVERTEX vt[] = { 
		{ rect.left, rect.top, r, g, b, 0 },
		{ rect.left+rect.Width(), rect.top+rect.Height(), r2, g2, b2, 0 },
	};

	GRADIENT_RECT arr_gr[] = { {0, 1} };

	GradientFill( hdc, vt, 2, &arr_gr, 1, (is_horizontal == TRUE) ? GRADIENT_FILL_RECT_H : GRADIENT_FILL_RECT_V );

	return;
}

void CGroupStatic::lc_GradientFill(const HDC& hdc, const CRect& rect, COLORREF bcolor, COLORREF ecolor, BOOL is_horizontal)
{
	COLOR16 r, g, b, r2, g2, b2;
	r = GetRValue( bcolor )+(GetRValue( bcolor )*256);
	g = GetGValue( bcolor )+(GetGValue( bcolor )*256);
	b = GetBValue( bcolor )+(GetBValue( bcolor )*256);
	r2 = GetRValue( ecolor )+(GetRValue( ecolor )*256);
	g2 = GetGValue( ecolor )+(GetGValue( ecolor )*256);
	b2 = GetBValue( ecolor )+(GetBValue( ecolor )*256);

	lc_GradientFill( hdc, rect, r, g, b, r2, g2, b2, is_horizontal );
}

//-----------------------------------------------------------------------
// タイトル文字列のセット
//		LPCTSTR		title		タイトル文字列
//-----------------------------------------------------------------------
void CGroupStatic::SetTitleString(LPCTSTR title)
{
	if ( title != NULL )
		m_title.Format( _T("%s"), title );
}

//-----------------------------------------------------------------------
// 指定背景色の変更
//-----------------------------------------------------------------------
// 引数	bcolor	：	指定背景色
//-----------------------------------------------------------------------
void CGroupStatic::SetBkColor( COLORREF bcolor )
{
	m_bcolor = bcolor;
}

