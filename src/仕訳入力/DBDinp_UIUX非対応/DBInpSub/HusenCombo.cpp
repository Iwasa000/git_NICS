// HusenCombo.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"

#include "resource.h"
#include "HusenCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CPPDrawManager:: ( www.codeproject.com )
void GetSizeOfIcon(HICON hIcon, LPSIZE pSize)
{
	pSize->cx = 0;
	pSize->cy = 0;
	if (hIcon != NULL)
	{
		ICONINFO ii;
		// Gets icon dimension
		::ZeroMemory(&ii, sizeof(ICONINFO));
		if (::GetIconInfo(hIcon, &ii))
		{
			pSize->cx = (DWORD)(ii.xHotspot * 2);
			pSize->cy = (DWORD)(ii.yHotspot * 2);
			//release icon mask bitmaps
			if(ii.hbmMask)
				::DeleteObject(ii.hbmMask);
			if(ii.hbmColor)
				::DeleteObject(ii.hbmColor);
		} //if
	} //if
} //End GetSizeOfIcon

// CPPDrawManager:: ( www.codeproject.com )
HICON StretchIcon(HICON hIcon, DWORD dwWidth, DWORD dwHeight)
{
	HICON hStretchedIcon = NULL;
	HDC   hMainDC = NULL;
	HDC   hSrcDC = NULL;
	HDC   hDestDC = NULL;
	BITMAP bmp;
	HBITMAP hOldSrcBitmap = NULL;
	HBITMAP hOldDestBitmap = NULL;
	ICONINFO csOriginal, csStretched;
	
	if (!::GetIconInfo(hIcon, &csOriginal))
		return FALSE;
	
	hMainDC = ::GetDC(NULL);
	hSrcDC = ::CreateCompatibleDC(hMainDC);
	hDestDC = ::CreateCompatibleDC(hMainDC);
	
	if ((NULL == hMainDC) || (NULL == hSrcDC) || (NULL == hDestDC))
		return NULL;
	
	if (::GetObject(csOriginal.hbmColor, sizeof(BITMAP), &bmp))
	{
		DWORD	dwWidthOrg = csOriginal.xHotspot * 2;
		DWORD	dwHeightOrg = csOriginal.yHotspot * 2;
		
		csStretched.hbmColor = ::CreateBitmap(dwWidth, dwHeight, bmp.bmPlanes, bmp.bmBitsPixel, NULL);
		if (NULL != csStretched.hbmColor)
		{
			hOldSrcBitmap = (HBITMAP)::SelectObject(hSrcDC, csOriginal.hbmColor);
			hOldDestBitmap = (HBITMAP)::SelectObject(hDestDC, csStretched.hbmColor);
			::StretchBlt(hDestDC, 0, 0, dwWidth, dwHeight, hSrcDC, 0, 0, dwWidthOrg, dwHeightOrg, SRCCOPY);
			if (::GetObject(csOriginal.hbmMask, sizeof(BITMAP), &bmp))
			{
				csStretched.hbmMask = ::CreateBitmap(dwWidth, dwHeight, bmp.bmPlanes, bmp.bmBitsPixel, NULL);
				if (NULL != csStretched.hbmMask)
				{
					::SelectObject(hSrcDC, csOriginal.hbmMask);
					::SelectObject(hDestDC, csStretched.hbmMask);
					::StretchBlt(hDestDC, 0, 0, dwWidth, dwHeight, hSrcDC, 0, 0, dwWidthOrg, dwHeightOrg, SRCCOPY);
				} //if
			} //if
			::SelectObject(hSrcDC, hOldSrcBitmap);
			::SelectObject(hDestDC, hOldDestBitmap);
			csStretched.fIcon = TRUE;
			hStretchedIcon = ::CreateIconIndirect(&csStretched);
		} //if
		::DeleteObject(csStretched.hbmColor);
		::DeleteObject(csStretched.hbmMask);
	} //if
	
	::DeleteObject(csOriginal.hbmColor);
	::DeleteObject(csOriginal.hbmMask);
	::DeleteDC(hSrcDC);
	::DeleteDC(hDestDC);
	::ReleaseDC(NULL, hMainDC);
	
	return hStretchedIcon;
} //End StretchIcon


// CPPDrawManager:: ( www.codeproject.com )
void Hc_DrawIcon(HDC hDC, int x, int y, DWORD dwWidth, DWORD dwHeight, HICON hSrcIcon )
{
	if (NULL == hSrcIcon)
		return;

	SIZE sz;
	GetSizeOfIcon(hSrcIcon, &sz);

	HICON hIcon = NULL;

	if (((DWORD)sz.cx == dwWidth) && ((DWORD)sz.cy == dwHeight))
		hIcon = ::CopyIcon(hSrcIcon);
	else hIcon = StretchIcon(hSrcIcon, dwWidth, dwHeight);
	
	ICONINFO csOriginal;

	if (!::GetIconInfo(hIcon, &csOriginal))
		return;

	DrawIcon( hDC, x, y, hIcon );

	::DestroyIcon(hIcon);

	::DeleteObject(csOriginal.hbmColor);
	::DeleteObject(csOriginal.hbmMask);
} //End DrawIcon



/////////////////////////////////////////////////////////////////////////////
// CHusenCombo

extern HINSTANCE gInstance;


CHusenCombo::CHusenCombo()
{
HINSTANCE h =  AfxGetResourceHandle();
AfxSetResourceHandle( gInstance );

	m_imglist.Create( IDB_HUSEN_BMP, 32, 0, RGB(0,0,0) );

AfxSetResourceHandle( h );
}

CHusenCombo::~CHusenCombo()
{
}


BEGIN_MESSAGE_MAP(CHusenCombo, CComboBox)
	//{{AFX_MSG_MAP(CHusenCombo)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_SETFONT, OnSetFont)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHusenCombo メッセージ ハンドラ

void CHusenCombo::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	// TODO: 指定されたアイテムを描画するためのコードを追加してください

// ※ リソースを切り替えるので関数途中での return はしないように！
HINSTANCE h =  AfxGetResourceHandle();
AfxSetResourceHandle( gInstance );
	
	CString str; //= (LPCSTR)lpDIS->itemData;      //  アイテムのデータを取得

	if( lpDIS->itemID != (UINT)-1 ) {
		GetLBText( lpDIS->itemID, str );
	}
//TRACE( "hscombo %d, %d, --%s--\n", lpDIS->itemID, lpDIS->itemData, str );
    CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	int lvl = 2;
	int itd;
	
	if( lpDIS->itemID == 0 )	itd = lvl;
	else						itd = lvl* 7;
	int x = lpDIS->rcItem.left + itd;      //  インデントから表示位置を算出
	int y = lpDIS->rcItem.top;

	if (lpDIS->itemAction & ODA_DRAWENTIRE)
	{
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(x+itd, y, (LPCSTR)str);
	}
	if ((lpDIS->itemState & ODS_SELECTED) &&        //  選択状態の時の描画
		(lpDIS->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		pDC->FillSolidRect(&lpDIS->rcItem, GetSysColor(COLOR_ACTIVECAPTION));
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetBkMode(TRANSPARENT);
		pDC->TextOut(x+itd, y, (LPCSTR)str);
	}

	if (!(lpDIS->itemState & ODS_SELECTED) &&       //  通常の状態の描画
		(lpDIS->itemAction & ODA_SELECT))
	{
		pDC->FillSolidRect(&lpDIS->rcItem, GetSysColor(COLOR_WINDOW));
		pDC->SetTextColor( GetSysColor(COLOR_WINDOWTEXT) );
		pDC->TextOut(x+itd, y, (LPCSTR)str);
	}

	if( !IsWindowEnabled() ) {
		pDC->SetTextColor( GetSysColor(COLOR_GRAYTEXT) );
		pDC->TextOut(x+itd, y, (LPCSTR)str);
	}

	CPoint p(lpDIS->rcItem.left, lpDIS->rcItem.top);
	if( lpDIS->itemID != (UINT)-1 && lpDIS->itemID > 0 ) {
		if( (int)(lpDIS->itemID-1) <= m_imglist.GetImageCount() ) {
			HICON hi = m_imglist.ExtractIcon( lpDIS->itemID-1 );

			CRect rc = lpDIS->rcItem;
			int h = rc.Height();
//TRACE( "h = %d\n", h );
//		Hc_DrawIcon( pDC->m_hDC, p.x+2, p.y, h, h, hi );
			::DrawIconEx( pDC->m_hDC, p.x+2, p.y-2, hi, 22,22, 0,NULL,DI_NORMAL );
			::DestroyIcon( hi );
		}
	}

AfxSetResourceHandle( h );

}

void CHusenCombo::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: 指定されたアイテムのサイズを調べるためのコードを追加してください
/*	if( ::IsWindow( m_hWnd ) ) {
		CFont* pFont = GetFont();
		int ht = 16;
		if( pFont ) {
			LOGFONT lf;
			if( pFont->GetLogFont( &lf ) ) {
				ht = lf.lfHeight;
				ht += 2;
			}
		}

		lpMeasureItemStruct->itemHeight = ht;
	}
	lpMeasureItemStruct->itemHeight = 22;
*/
}


LRESULT CHusenCombo::OnSetFont( WPARAM wParam, LPARAM lParam )
{
    LRESULT res = Default();

    CRect rc;
    GetWindowRect( &rc );

    WINDOWPOS wp;
    wp.hwnd  = m_hWnd;
    wp.cx    = rc.Width();
    wp.cy    = rc.Height();
    wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
    SendMessage( WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp );

    return res;
}


void CHusenCombo::InitHusenCombo()
{
	ResetContent();
	CString str = "なし";
	this->AddString( str );

	for( int i = 0; i < 5; i++ ) {
		str.Format( "%d", (i+1) );
		this->AddString( str );
	}
}


#ifdef CLOSE

BOOL CHusenCombo::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
//	m_imglist.Create( IDB_BITMAP1, 32, 0, RGB(0,0,0) );
//	cs.style |= (CBS_OWNERDRAWFIXED | CBS_DROPDOWNLIST);

	return CComboBox::PreCreateWindow(cs);
}
#endif

void CHusenCombo::PreSubclassWindow() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
#ifdef KEEP
	DWORD    dwStyle = GetStyle();
	// CBS_OWNERDRAWFIXEDが
	// 指定されていなければStyleに追加する
	if( (dwStyle & (CBS_OWNERDRAWFIXED | CBS_DROPDOWNLIST)) 
			!= (CBS_OWNERDRAWFIXED | CBS_DROPDOWNLIST))
	{
	}
#endif
	ModifyStyle( 0, WS_CLIPSIBLINGS );

	CComboBox::PreSubclassWindow();
}



/////////////////////////////////////////////////////////////////////////////////////////
// CHusenComboEX
CHusenComboEX::CHusenComboEX()
{
HINSTANCE h =  AfxGetResourceHandle();
AfxSetResourceHandle( gInstance );

	m_imglist.Create( IDB_HUSEN_BMP, 32, 0, RGB(0,0,0) );

AfxSetResourceHandle( h );

	m_Mode = HUMD_NORMAL;
}

CHusenComboEX::~CHusenComboEX()
{
}


BEGIN_MESSAGE_MAP(CHusenComboEX, CComboBox)
	//{{AFX_MSG_MAP(CHusenComboEX)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_SETFONT, OnSetFont)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHusenComboEX メッセージ ハンドラ

void CHusenComboEX::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	// TODO: 指定されたアイテムを描画するためのコードを追加してください

// ※ リソースを切り替えるので関数途中での return はしないように！
HINSTANCE h =  AfxGetResourceHandle();
AfxSetResourceHandle( gInstance );
	
	CString str; //= (LPCSTR)lpDIS->itemData;      //  アイテムのデータを取得

	if( lpDIS->itemID != (UINT)-1 ) {
		GetLBText( lpDIS->itemID, str );
	}
//TRACE( "hscombo %d, %d, --%s--\n", lpDIS->itemID, lpDIS->itemData, str );
    CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	int lvl = 2;
	int itd;
	
	if( lpDIS->itemID == 0 )	itd = lvl;
	else {
		if( m_Mode == HUMD_SCAN && lpDIS->itemID == 1 )
			itd = lvl;
		else
			itd = lvl* 7;
	}
	int x = lpDIS->rcItem.left + itd;      //  インデントから表示位置を算出
	int y = lpDIS->rcItem.top;

	if (lpDIS->itemAction & ODA_DRAWENTIRE)
	{
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(x+itd, y, (LPCSTR)str);
	}
	if ((lpDIS->itemState & ODS_SELECTED) &&        //  選択状態の時の描画
		(lpDIS->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		pDC->FillSolidRect(&lpDIS->rcItem, GetSysColor(COLOR_ACTIVECAPTION));
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetBkMode(TRANSPARENT);
		pDC->TextOut(x+itd, y, (LPCSTR)str);
	}

	if (!(lpDIS->itemState & ODS_SELECTED) &&       //  通常の状態の描画
		(lpDIS->itemAction & ODA_SELECT))
	{
		pDC->FillSolidRect(&lpDIS->rcItem, GetSysColor(COLOR_WINDOW));
		pDC->SetTextColor( GetSysColor(COLOR_WINDOWTEXT) );
		pDC->TextOut(x+itd, y, (LPCSTR)str);
	}

	if( !IsWindowEnabled() ) {
		pDC->SetTextColor( GetSysColor(COLOR_GRAYTEXT) );
		pDC->TextOut(x+itd, y, (LPCSTR)str);
	}

	CPoint p(lpDIS->rcItem.left, lpDIS->rcItem.top);

	int sidx;
	sidx = (m_Mode == HUMD_SCAN) ? 1 : 0;

	if( lpDIS->itemID != (UINT)-1 && lpDIS->itemID > sidx ) {
		if( (int)(lpDIS->itemID-(sidx+1) ) <= m_imglist.GetImageCount() ) {
			HICON hi = m_imglist.ExtractIcon( lpDIS->itemID-(sidx+1) );

			CRect rc = lpDIS->rcItem;
			int h = rc.Height();
//TRACE( "h = %d\n", h );
//		Hc_DrawIcon( pDC->m_hDC, p.x+2, p.y, h, h, hi );
			::DrawIconEx( pDC->m_hDC, p.x+2, p.y-2, hi, 22,22, 0,NULL,DI_NORMAL );
			::DestroyIcon( hi );
		}
	}

AfxSetResourceHandle( h );

}

void CHusenComboEX::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: 指定されたアイテムのサイズを調べるためのコードを追加してください
/*	if( ::IsWindow( m_hWnd ) ) {
		CFont* pFont = GetFont();
		int ht = 16;
		if( pFont ) {
			LOGFONT lf;
			if( pFont->GetLogFont( &lf ) ) {
				ht = lf.lfHeight;
				ht += 2;
			}
		}

		lpMeasureItemStruct->itemHeight = ht;
	}
	lpMeasureItemStruct->itemHeight = 22;
*/
}


LRESULT CHusenComboEX::OnSetFont( WPARAM wParam, LPARAM lParam )
{
    LRESULT res = Default();

    CRect rc;
    GetWindowRect( &rc );

    WINDOWPOS wp;
    wp.hwnd  = m_hWnd;
    wp.cx    = rc.Width();
    wp.cy    = rc.Height();
    wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
    SendMessage( WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp );

    return res;
}


void CHusenComboEX::InitHusenComboEX()
{
	ResetContent();
	CString str = "なし";
	this->AddString( str );

	if( m_Mode == HUMD_SCAN ) {
		CString str = "付箋なし";
		this->AddString( str );
	}

	for( int i = 0; i < 5; i++ ) {
		str.Format( "%d", (i+1) );
		this->AddString( str );
	}
}


void CHusenComboEX::SetMode( int mode )
{
	m_Mode = mode;
}



#ifdef CLOSE

BOOL CHusenComboEX::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
//	m_imglist.Create( IDB_BITMAP1, 32, 0, RGB(0,0,0) );
//	cs.style |= (CBS_OWNERDRAWFIXED | CBS_DROPDOWNLIST);

	return CComboBox::PreCreateWindow(cs);
}
#endif

void CHusenComboEX::PreSubclassWindow() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
#ifdef KEEP
	DWORD    dwStyle = GetStyle();
	// CBS_OWNERDRAWFIXEDが
	// 指定されていなければStyleに追加する
	if( (dwStyle & (CBS_OWNERDRAWFIXED | CBS_DROPDOWNLIST)) 
			!= (CBS_OWNERDRAWFIXED | CBS_DROPDOWNLIST))
	{
	}
#endif
	ModifyStyle( 0, WS_CLIPSIBLINGS );

	CComboBox::PreSubclassWindow();
}
