// ExtTab.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "resource.h"
#include "ExtTab.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtTab

#define WHITECOL	RGB(255,255,255)


CExtTab::CExtTab()
{
	m_crSelColour = RGB(0,0,255);
	m_crUnselColour = RGB(0,0,0);
}

CExtTab::~CExtTab()
{
}


BEGIN_MESSAGE_MAP(CExtTab, CTabCtrl)
	//{{AFX_MSG_MAP(CExtTab)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtTab メッセージ ハンドラ

int CExtTab::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTabCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: この位置に固有の作成用コードを追加してください
	ModifyStyle(0, TCS_OWNERDRAWFIXED);

	return 0;
}

void CExtTab::PreSubclassWindow() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください	
	CTabCtrl::PreSubclassWindow();
	ModifyStyle(0, TCS_OWNERDRAWFIXED);
}



void CExtTab::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CRect rect = lpDIS->rcItem;
	int nTabIndex = lpDIS->itemID;
	if (nTabIndex < 0) return;
	BOOL bSelected = (nTabIndex == GetCurSel());

	char label[64];
	TC_ITEM tci;
	tci.mask = TCIF_TEXT|TCIF_IMAGE;
	tci.pszText = label;     
	tci.cchTextMax = (sizeof(label)-1);
	if (!GetItem(nTabIndex, &tci )) return;

	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	if (!pDC) return;
	int nSavedDC = pDC->SaveDC();

	// For some bizarre reason the rcItem you get extends above the actual
	// drawing area. We have to workaround this "feature".
	rect.top += ::GetSystemMetrics(SM_CYEDGE);

	pDC->SetBkMode(TRANSPARENT);
#ifdef CLOSE
	if (bSelected) {
		pDC->FillSolidRect(rect,  WHITECOL );
		pDC->Draw3dRect(rect,  WHITECOL, ::GetSysColor(COLOR_BTNFACE)); // 影をつけて四角形を整える
	}
	else {
		pDC->FillSolidRect(rect, ::GetSysColor(COLOR_BTNFACE));
	}
#endif
	pDC->FillSolidRect(rect, ::GetSysColor(COLOR_BTNFACE));

	// Draw image
	CImageList* pImageList = GetImageList();
	if (pImageList && tci.iImage >= 0) {

		rect.left += pDC->GetTextExtent(_T(" ")).cx;		// Margin

		// Get height of image so we 
		IMAGEINFO info;
		pImageList->GetImageInfo(tci.iImage, &info);
		CRect ImageRect(info.rcImage);
		int nYpos = rect.top;

		pImageList->Draw(pDC, tci.iImage, CPoint(rect.left, nYpos), ILD_TRANSPARENT);
		rect.left += ImageRect.Width();
	}

	CFont* pFont, *pOldFont;
	CFont dspfont;
	pFont = GetFont();
	LOGFONT lf;
	pFont->GetLogFont(&lf);
	lf.lfWeight = FW_SEMIBOLD;
	dspfont.CreateFontIndirect(&lf);
	pOldFont = pDC->SelectObject(&dspfont);

	if (bSelected) {
		pDC->SetTextColor(m_crSelColour);
		rect.top -= ::GetSystemMetrics(SM_CYEDGE);
		pDC->DrawText(label, rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	} else {
		pDC->SetTextColor(m_crUnselColour);
		pDC->DrawText(label, rect, DT_SINGLELINE|DT_BOTTOM|DT_CENTER);
	}
	pDC->SelectObject(pOldFont);


	pDC->RestoreDC(nSavedDC);
}



BOOL CExtTab::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

	if( pMsg->message == WM_KEYDOWN ) {
		// タブにフォーカスがあるときに Enter Down -> は 中のダイアログにフォーカスセット
		if( pMsg->wParam == VK_RETURN || pMsg->wParam == VK_DOWN ) { 
			CWnd* pParent;
			pParent = GetParent();
			if( pParent ) {
				pParent->PostMessage( WM_MYTABCHANGE, 2, 0 );
			}
		}
	}
	else if( pMsg->message >= WM_APP ) {
		CWnd* pParent;
		pParent = GetParent();
		if( pParent ) {
			pParent->PostMessage( pMsg->message, pMsg->wParam, pMsg->lParam );
		}
	}

	return CTabCtrl::PreTranslateMessage(pMsg);
}

