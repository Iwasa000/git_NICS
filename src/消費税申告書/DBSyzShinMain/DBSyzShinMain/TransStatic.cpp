// TransStatic.cpp : 実装ファイル
//

#include "stdafx.h"
//#include "CoSetDB.h"
#include "TransStatic.h"


// CTransStatic

IMPLEMENT_DYNAMIC(CTransStatic, CWnd)

CTransStatic::CTransStatic()
{
	m_lfWeight = FW_NORMAL;
	m_textClr = RGB( 0x00, 0x00, 0x00 );
	m_SmallFont = 0;
}

CTransStatic::~CTransStatic()
{
}


BEGIN_MESSAGE_MAP(CTransStatic, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CTransStatic メッセージ ハンドラ

void CTransStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	// 描画メッセージで CWnd::OnPaint() を呼び出さないでください。

	// 描画領域取得
	CRect client_rect;
	GetClientRect( client_rect );

	// 文字列取得
	CString szText;
	GetWindowText( szText );

	// フォント情報取得
	CFont *pFont, *pOldFont;
	pFont = GetFont();
/*- '13.12.27 -*/
//	pOldFont = dc.SelectObject( pFont );
/*-------------*/
	LOGFONT	lgFont;
	pFont->GetLogFont( &lgFont );
	lgFont.lfWeight = m_lfWeight;
	if( m_SmallFont ){
		lgFont.lfHeight = lgFont.lfHeight* 86 / 100;
		lgFont.lfWidth  = lgFont.lfWidth * 86 / 100;
	}
	CFont	tmpFont;
	tmpFont.CreateFontIndirect( &lgFont );
	pOldFont = dc.SelectObject( &tmpFont );
/*-------------*/

	// staticスタイルをdrawtextスタイルに変更
#define MAP_STYLE(src, dest) if(dwStyle & (src)) dwText |= (dest)
#define NMAP_STYLE(src, dest) if(!(dwStyle & (src))) dwText |= (dest)

	DWORD dwStyle = GetStyle(), dwText = 0;

	MAP_STYLE( SS_RIGHT, DT_RIGHT );
	MAP_STYLE( SS_CENTER, DT_CENTER );
	MAP_STYLE( SS_CENTERIMAGE, (DT_VCENTER|DT_SINGLELINE) );
	MAP_STYLE( SS_NOPREFIX, DT_NOPREFIX );
	MAP_STYLE( SS_WORDELLIPSIS,	DT_WORD_ELLIPSIS );
	MAP_STYLE( SS_ENDELLIPSIS, DT_END_ELLIPSIS );
	MAP_STYLE( SS_PATHELLIPSIS,	DT_PATH_ELLIPSIS );

	NMAP_STYLE(	(SS_LEFTNOWORDWRAP|SS_CENTERIMAGE|SS_WORDELLIPSIS|SS_ENDELLIPSIS|SS_PATHELLIPSIS),	DT_WORDBREAK );

	// 背景描画モードセット
	dc.SetBkMode( TRANSPARENT );

	// 文字色
	COLORREF nowClr = dc.SetTextColor( m_textClr );

	// 文字列描画
	dc.DrawText( szText, client_rect, dwText );

	// オブジェクト選択
	dc.SelectObject( pOldFont );
	dc.SetTextColor( nowClr );
}

//-----------------------------------------------------------------------------
// フォントウェイトの設定
//-----------------------------------------------------------------------------
// 引数	lfWeight	：	設定するフォントウェイト
//-----------------------------------------------------------------------------
void CTransStatic::SetFontWeight( long lfWeight )
{
	m_lfWeight = lfWeight;
	RedrawWindow();
}

//-----------------------------------------------------------------------------
// 文字色の設定
//-----------------------------------------------------------------------------
// 引数	textClr		：	設定する文字色
//-----------------------------------------------------------------------------
void CTransStatic::SetTextColor( COLORREF textClr )
{
	m_textClr = textClr;
	RedrawWindow();
}

//-----------------------------------------------------------------------------
// 小さいフォントで表示するかの設定
//-----------------------------------------------------------------------------
// 引数	sw			：	1 する　/　0 しない
//-----------------------------------------------------------------------------
void CTransStatic::IsSmallFont( int sw )
{
	m_SmallFont = sw;
	RedrawWindow();
}

