#include "StdAfx.h"
#include "ColorComboBox.h"

CColorComboBox::CColorComboBox(void) : m_ColorChange(0)
{
}

CColorComboBox::~CColorComboBox(void) 
{
}

BEGIN_MESSAGE_MAP(CColorComboBox, CComboBox)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

void CColorComboBox::OnPaint()
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	// 描画メッセージで CComboBox::OnPaint() を呼び出さないでください。
	Default();
	
	if( !m_ColorChange ) return;

	CClientDC dc(this);

	LOGFONT lf;
	CFont f;
	GetFont()->GetLogFont(&lf);
	f.CreateFontIndirect(&lf);
	CFont* pOldFont = dc.SelectObject(&f);

	int cur = GetCurSel();
	if( cur < 0 ) return;

	CString str;
	GetLBText( cur, str );

	dc.SetBkMode( TRANSPARENT );
	dc.SetTextColor( RGB(0,0,255) );			

	int x = GetSystemMetrics(SM_CXFIXEDFRAME) + 1;
	int y = GetSystemMetrics(SM_CYFIXEDFRAME) + 1;

	dc.TextOut( x, y, str );
	dc.SelectObject(pOldFont);
}

HBRUSH CColorComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);

	if( !m_ColorChange ) return hbr;

	switch( nCtlColor )
    {
    case CTLCOLOR_EDIT:
    case CTLCOLOR_LISTBOX:
        pDC->SetBkMode( TRANSPARENT );
        pDC->SetTextColor( RGB(0,0,255) );
        return hbr;
    }

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}
