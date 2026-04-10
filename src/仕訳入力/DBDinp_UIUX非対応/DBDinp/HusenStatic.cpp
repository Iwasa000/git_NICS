#include "stdafx.h"
#include "DBDinp.h"
#include "HusenStatic.h"



IMPLEMENT_DYNAMIC(CHusenStatic, CXTCaption)

CHusenStatic::CHusenStatic()
{
	m_imglist.Create( IDB_HUSEN_BMP, 32, 0, RGB(0,0,0) );
	m_index = 0;

	m_hNowIcon = NULL;
}

CHusenStatic::~CHusenStatic()
{
}


BEGIN_MESSAGE_MAP(CHusenStatic, ICSStatic)
END_MESSAGE_MAP()

//------------------------------------------------------
// 付箋の描画インデックス
//	int index	0 = 付箋なし, 1 ～ 5 は付箋番号 
//
//------------------------------------------------------
void CHusenStatic::SetIconIndex( int index )
{
	if( m_hNowIcon != NULL ) {
		::DestroyIcon( m_hNowIcon );
		m_hNowIcon = NULL;
	}

	if( index == 0 ) {
		m_hNowIcon = NULL;
		SetIcon( m_hNowIcon );
	}
	else {
		if( index <= m_imglist.GetImageCount() ) {
			m_hNowIcon = m_imglist.ExtractIcon( (index-1) );

			SetIcon( m_hNowIcon );
		}
	}
	m_index = index;
}


void CHusenStatic::DrawCaptionText( CDC* pDC )
{
	ICSStatic::DrawCaptionText( pDC );
}


void CHusenStatic::DrawCaptionIcon(CDC* pDC, CRect& rcItem)
{
//	::DrawIconEx( pDC->m_hDC, p.x+2, p.y-2, hi, 22,22, 0,NULL,DI_NORMAL );
//	::DestroyIcon( hi );

	// 左端 に余裕を持たせる
	rcItem.left += 3;

	ICSStatic::DrawCaptionIcon( pDC, rcItem );
}


void CHusenStatic::PreSubclassWindow()
{
	ICSStatic::PreSubclassWindow();
}

