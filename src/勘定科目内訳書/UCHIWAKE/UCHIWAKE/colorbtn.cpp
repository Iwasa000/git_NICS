/*- colorbtn.cpp -*/

#include "stdafx.h"
#include "colorbtn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CColorButton, CButton)

CColorButton::CColorButton() 
{
	// ディフォルトカラー
	coltext = RGB(0, 0, 0);				// #0x00,0x00,0x00
	colbackground = RGB(192, 192, 192);	// #0xC0,0xC0,0xC0
	coldisabled = RGB(128, 128, 128);	// #0x80,0x80,0x80
}

CColorButton::~CColorButton(){}

// ボタンをCColorButtonアタッチにする
BOOL CColorButton::AutoLoad(const UINT nID, CWnd* pParent)
{
	// ダイナミックサブクラス化
	if (!SubclassDlgItem(nID, pParent))
		return false;

	return true;
}

// オーナー描画する
void CColorButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	CString strCaption;

	UINT nState = lpDIS->itemState; 
	CRect focusRect, btnRect;
	focusRect.CopyRect(&lpDIS->rcItem);
	btnRect.CopyRect(&lpDIS->rcItem);

	// フォーカス矩形領域の作成
	focusRect.left += 4;
	focusRect.right -= 4;
	focusRect.top += 4;
	focusRect.bottom -= 4;

	// ボタンのキャプションを取得
	GetWindowText(strCaption);

	// ボタンの描画(背景色、エッジ、キャプション)
	DrawBkGround(pDC, btnRect, GetBackGroundColor()); 
  	DrawCaption(pDC, btnRect, GetTextColor(),strCaption);
	pDC->DrawEdge(btnRect,EDGE_ETCHED,BF_RECT);

	// フォーカスを得たときや選択されているとき
	if (nState & ODS_FOCUS)
	{
		if (nState & ODS_SELECTED)
		{ 
			pDC->DrawEdge(btnRect,EDGE_SUNKEN,BF_RECT);
		}
		pDC->DrawFocusRect(focusRect);
	}// ボタンが無効であるとき
	else if(nState & ODS_DISABLED)
	{
  		DrawCaption(pDC, btnRect, GetDisabledColor(), strCaption);
    }
}

// 背景色描画
void CColorButton::DrawBkGround(CDC *pDC, CRect rect,const COLORREF color)
{ 
	CBrush brush(color);
	pDC->FillRect(rect, &brush);
}

// キャプション描画
void CColorButton::DrawCaption(CDC *pDC, CRect rect,const COLORREF color,const CString& str)
{
	COLORREF oldcolor = pDC->SetTextColor(color);
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(str,rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	pDC->SetTextColor(oldcolor);
}

BOOL CColorButton::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	UINT	style	=	GetButtonStyle();
	if( style != BS_PUSHBUTTON && style != BS_DEFPUSHBUTTON ) {
		switch( pMsg->message ) {
			case	WM_KEYDOWN:
				switch( pMsg->wParam ) {
					case	VK_RETURN:
						CButton::PreTranslateMessage(pMsg);
						pMsg->wParam	=	VK_TAB;
						break;
				}
				break;
		}
	}
	return CButton::PreTranslateMessage(pMsg);
}
