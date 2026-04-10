#include "stdafx.h"
#include "ColoredComboDD.h"



CColoredComboDD::CColoredComboDD()
:	m_bDropDownListStyle(false),
	m_bIsButtonClick(false),
	m_clrText(RGB_BLACK),
	m_clrBk(RGB_WHITE),
	m_Edit(this)
{
	m_Brush.CreateSolidBrush( m_clrBk );
	m_filtersw = -1;
	m_bSheetType = TRUE;
}

CColoredComboDD::~CColoredComboDD()
{
}


BEGIN_MESSAGE_MAP(CColoredComboDD, CComboBox)
	//{{AFX_MSG_MAP(CColoredComboDD)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	//ON_WM_SETFOCUS()
	//ON_WM_KILLFOCUS()
	ON_CONTROL_REFLECT(CBN_SETFOCUS, OnSetFocus)
	ON_CONTROL_REFLECT(CBN_KILLFOCUS, OnKillFocus)
	ON_WM_ENABLE()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CColoredComboDD message handlers

HBRUSH CColoredComboDD::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//	if (nCtlColor == CTLCOLOR_LISTBOX)
//   {
//      if( ! m_ListBox.GetSafeHwnd() )
//		{
//         VERIFY( m_ListBox.SubclassWindow(pWnd->GetSafeHwnd()) );
//		}
//   }

	if(nCtlColor == CTLCOLOR_EDIT || nCtlColor == CTLCOLOR_LISTBOX)
   {
      pDC->SetBkColor( m_clrBk );
		pDC->SetTextColor( m_clrText );
      return m_Brush;
   }

	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}

void CColoredComboDD::OnPaint() 
{
	static int nCounter(0);
	TRACE("%d. OnPaint()\n", nCounter++);
	
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect ClientRect;
	GetClientRect( &ClientRect );
	
	//draw the 2- 3D Rects around the combo
	dc.Draw3dRect(&ClientRect,
		::GetSysColor(COLOR_3DSHADOW),
		RGB_WHITE );
	ClientRect.DeflateRect(1,1);
	dc.Draw3dRect(&ClientRect,
		::GetSysColor(COLOR_3DDKSHADOW),
		::GetSysColor(COLOR_3DLIGHT) );
	ClientRect.DeflateRect(1,1);

	//fill the rect in the brush's color
	CBrush* pOldBrush = dc.SelectObject( &m_Brush );
	dc.FillRect(ClientRect, &m_Brush);

//	dc.SetBkColor( m_clrBk );
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor( m_clrText );
	CString sText;

	CFont *pBasefont, *pOldfont;

	pBasefont = GetFont();
	if( pBasefont != NULL ) {
		pOldfont = dc.SelectObject(pBasefont);        // フォントを設定。
	}
	int sel = this->GetCurSel();
	if( sel != -1 ) {
		this->GetLBText( this->GetCurSel(), sText );

		CSize	testSize;
		int		lngs;
		// コントロール幅に収まらない文字をカット ('17.05.12)
		while(1){
			lngs = sText.GetLength();
			if( !lngs ) break;
			GetTextExtentPoint32( dc, sText, lngs, &testSize );
			if( testSize.cx > ClientRect.Width() ) {
				if (_mbsbtype((unsigned char*)(LPCTSTR)sText, lngs-1) == _MBC_TRAIL) {
					sText = sText.Left(lngs-2);
				}
				else {
					sText = sText.Left(lngs-1);
				}
			}
			else break;
		}
		
		dc.TextOut( 3, ClientRect.top += 2, sText ); 
	}
	if( pBasefont != NULL )
		dc.SelectObject(pOldfont);                // フォントを元に戻す。


	//since the flags are not expon. (1,2,4,8...), i need to do like this:
	DWORD dwComboStyle = GetStyle();
	BYTE byteComboStyle = (BYTE)dwComboStyle;
	//in SIMPLE style, there is no button
	if( byteComboStyle == CBS_SIMPLE )
	{
		dc.SelectObject( pOldBrush );
		return;
	}

	/*******		only when Button is needed:	*****************/

	int nButtonWidth = ::GetSystemMetrics(SM_CYSIZE);

	//resize the ClientRect to the size of the buttom:
	DWORD dwExStyles = GetExStyle();
	if(dwExStyles & WS_EX_RIGHT)	//the button is on the left, WS_EX_RIGHT
	{
		ClientRect.right = ClientRect.left + nButtonWidth;
	} 
	else	//the button is on the right. WS_EX_LEFT
	{
		ClientRect.left = ClientRect.right - nButtonWidth + 1;	//+1 very important - dont lose a pixel
	}

	CBrush brush;
	brush.CreateSolidBrush( ::GetSysColor(COLOR_BTNFACE) );
	dc.SelectObject( &brush );
	dc.FillRect(ClientRect, &brush);
	brush.DeleteObject();

	//paint the ****ing arrow:
	CPoint CenterPoint( ClientRect.CenterPoint() );
	++CenterPoint.y;
	CPoint UpperPoint(CenterPoint.x, CenterPoint.y - 4);
	CPen Pen(PS_SOLID, 1, m_clrText);
	CPen* pOldPen = dc.SelectObject(&Pen);
	for(int i = 0; i < 4; ++i)
	{
		dc.MoveTo(CenterPoint);
		VERIFY( dc.LineTo(UpperPoint) );

		--CenterPoint.x;
		--CenterPoint.y;
		--UpperPoint.x;
	}
	
	CenterPoint = ClientRect.CenterPoint();
	++CenterPoint.y;
	UpperPoint = CenterPoint;
	UpperPoint.y -= 4;
	for(int i = 0; i < 4; ++i)
	{
		dc.MoveTo(CenterPoint);
		VERIFY( dc.LineTo(UpperPoint) );

		++CenterPoint.x;
		--CenterPoint.y;
		++UpperPoint.x;
	}
	//until here - painting the arrow:

	//draw the rect of the button:
	if( ! m_bIsButtonClick )	//regular state - btn NOT pressed
	{
		dc.Draw3dRect(&ClientRect,
			::GetSysColor(COLOR_3DLIGHT),
			::GetSysColor(COLOR_3DDKSHADOW) );
		ClientRect.DeflateRect(1,1);
		dc.Draw3dRect(&ClientRect,
			RGB_WHITE,
			::GetSysColor(COLOR_3DSHADOW) );
	}
	else		//btn is pressed
	{
		m_bIsButtonClick = false;

		dc.Draw3dRect(&ClientRect,
			::GetSysColor(COLOR_3DSHADOW),
			::GetSysColor(COLOR_3DSHADOW) );
	}

	dc.SelectObject( pOldBrush );
	dc.SelectObject( pOldPen );
	
	// Do not call CComboBox::OnPaint() for painting messages
}

void CColoredComboDD::PreSubclassWindow() 
{
#ifdef _CUT
	HWND hEdit = ::GetTopWindow(m_hWnd);
   VERIFY( m_Edit.SubclassWindow( hEdit ) );
	m_Edit.SetReadOnly( m_bDropDownListStyle );
   
	/*********************************************************************/
	//in case of "drop List", there is no EditBox, but Static, and window
	//paints the static AFTER my OnPaint, so i cannot paint it. so i do NOT allow
	//to work in this mode, but i edit it myself (ASSERT...)
	
	//since the flags are not expon. (1,2,4,8...), i need to do like this:
	DWORD dwComboStyle = GetStyle();
	BYTE byteComboStyle = (BYTE)dwComboStyle;
	if( byteComboStyle == CBS_DROPDOWNLIST )
	{
		//can NOT change the ctrl's style to drop down, so this code will NOT work:
//		ModifyStyle(CBS_DROPDOWNLIST, CBS_DROPDOWN);
//		//dwComboStyle = GetStyle();		//make sure... :)
		//so instead, :
		ASSERT( !"cannot paint DropDown List !!!");
	}
#endif
	CComboBox::PreSubclassWindow();
}

void CColoredComboDD::SetBkColor(COLORREF clrBk)
{	
	m_clrBk = clrBk;
	m_Brush.DeleteObject();
	VERIFY( m_Brush.CreateSolidBrush( m_clrBk ) );
	m_Edit.SetBkColor( clrBk ); 
}

/****************************************************************/

//the combo gets this message when its Button is pressed
void CColoredComboDD::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//the combo gets this message when its Button is pressed
	m_bIsButtonClick = true;

	if (!m_bSheetType) {
		if (m_filtersw & 0x01 && GetCount() == 0) {	// 検索結果なし
			Refresh(1);
			SetCurSel(0);
			m_filtersw &= (~0x01);
			GetParent()->SendMessage(WM_USER_SELCHG_MSG);
		}
	}
	
	CComboBox::OnLButtonDown(nFlags, point);
}

void CColoredComboDD::OnSetFocus(/*CWnd* pOldWnd*/)
{
	//CComboBox::OnSetFocus(pOldWnd);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if (m_bSheetType) {
		SetBkColor(RGB_YELLOW);
	}
	else {
		SetBkColor(RGB_YELLOW1); //薄めの黄色
	}
	SetTextColor(RGB_BLACK );

	HIMC hIMC = ImmGetContext( m_hWnd );
	DWORD dwConv;
	DWORD dwSent;
	ImmGetConversionStatus(hIMC, &dwConv, &dwSent);

	dwConv = IME_CMODE_NATIVE | IME_CMODE_FULLSHAPE;	//ひらがなに設定
	ImmSetConversionStatus(hIMC, dwConv, dwSent);

	ImmSetOpenStatus( hIMC, TRUE );	// IMEをON

	ImmReleaseContext( m_hWnd, hIMC );
}

void CColoredComboDD::OnKillFocus(/*CWnd* pNewWnd*/)
{
	//CComboBox::OnKillFocus(pNewWnd);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	int i, max, sel;
	if( (sel = GetCurSel()) == -1 ) {
		SetCurSel( 0 );	
		if (!(m_filtersw & 0x01)) {
			GetParent()->SendMessage(WM_USER_SELCHG_MSG);
		}
	}

	if( m_filtersw&0x01 ) {	// 絞り込み解除
		int d = GetItemData(sel);
		Refresh(1);
		max = m_data.GetCount();
		for (i=0; i<max; i++) {
			if( m_data[i] == d ) break;
		}
		//検索結果なしのときは初期値へ
		if (i >= max)	i = 0;

		SetCurSel( i );
		m_filtersw &= (~0x01);
		GetParent()->SendMessage(WM_USER_SELCHG_MSG);
	}
	
	SetBkColor(RGB_WHITE );
	SetTextColor(RGB_BLACK );

	HIMC hIMC = ImmGetContext(m_hWnd);
	DWORD dwConv;
	DWORD dwSent;
	ImmGetConversionStatus(hIMC, &dwConv, &dwSent);
	ImmSetOpenStatus(hIMC, FALSE);	// IMEをOFF
	ImmReleaseContext(m_hWnd, hIMC);
}

void CColoredComboDD::OnEnable(BOOL bEnable)
{
	CComboBox::OnEnable(bEnable);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( bEnable == FALSE ) {
		SetBkColor(RGB_GRAY );
		SetTextColor( ::GetSysColor(COLOR_GRAYTEXT) );
	}
	else {
		SetBkColor(RGB_WHITE );
		SetTextColor(RGB_BLACK );
	}
}

BOOL CColoredComboDD::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN ) {
		CString	str;
		switch( pMsg->wParam ) {

		case	VK_RETURN:
			GetWindowText(str);
			if( GetCurSel() == -1 ) {
				AddList(1, str);
				if( GetCount() > 1 ) {
					SetCurSel(m_bSheetType ? 1 : 0); 
					ShowDropDown(1);	// 検索結果がある場合は、リストを開く
				}
				else { 
					SetCurSel(0); 
				}
				if (!m_bSheetType) {
					GetParent()->SendMessage( WM_USER_SELCHG_MSG );
				}
				return TRUE;
			}
			else {
				if (!m_bSheetType) {
					GetParent()->SendMessage(WM_USER_SELCHG_MSG, WM_KEYDOWN, VK_RETURN );
					return TRUE;
				}
			}
			break;
		case	VK_DELETE:
			AddList(0, NULL);
			SetCurSel(0);
			GetParent()->SendMessage( WM_USER_SELCHG_MSG );
			return TRUE;
			break;
		}
	}
	return CComboBox::PreTranslateMessage(pMsg);
}

//-----------------------------------------------------------------------------
// コンボボックスを作成（初期値をセット）
//-----------------------------------------------------------------------------
// 引数	Text	：	表示文字列
//		data	：	ItemData
//-----------------------------------------------------------------------------
void CColoredComboDD::MakeComboList(CStringArray& Text, CDWordArray& data)
{
	m_Text.RemoveAll();
	m_Text.Copy(Text);

	m_data.RemoveAll();
	if(data.GetCount() > 0)
		m_data.Copy(data);

	Refresh(1);

	return;
}

//-----------------------------------------------------------------------------
// コンボボックスを初期値に戻す（絞り込みを解除）
//-----------------------------------------------------------------------------
// 引数	flg		：	D0:コンボボックスを再作成する
//						（OFF：絞り込みがされている場合のみ再作成）
//-----------------------------------------------------------------------------
void CColoredComboDD::Refresh(int flg)
{
	AddList(flg, NULL);
}

//-----------------------------------------------------------------------------
// コンボボックスを作成（絞り込み）
//-----------------------------------------------------------------------------
// 引数	flg		：	D0:コンボボックスを再作成する
//		filter	：	検索文字列
//-----------------------------------------------------------------------------
void CColoredComboDD::AddList(int flg, CString filter)
{
	CString	moji;
	int		i, idx;

	if( ((flg&0x01) == 0) && (m_filtersw == 0) )
		return;	// 絞り込みが行われていないので、再作成不要

	m_filtersw = (flg&0x01);

	ResetContent();

	int tcnt = m_Text.GetCount();
	int dcnt = m_data.GetCount();

	for( i = 0, idx = 0; i < tcnt; i++ ) {
		moji = m_Text.GetAt(i);

		if( flg ) {
			if (m_bSheetType) {
				if (i != 1) {	// インデックス 1（-設定-）は常に表示
					if (moji.Find(filter) == -1)
						continue;
				}
			}
			else {
				if (moji.Find(filter) == -1)
					continue;
			}
		}
		AddString( moji );
		if( dcnt ) {
			SetItemData( idx, m_data.GetAt(i) );
			idx++;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColoredEdit

CColoredEdit::CColoredEdit()
{
	m_Brush.CreateSolidBrush(RGB(250, 0, 0));
}

CColoredEdit::~CColoredEdit()
{
}


BEGIN_MESSAGE_MAP(CColoredEdit, CEdit)
	//{{AFX_MSG_MAP(CColoredEdit)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColoredEdit message handlers

HBRUSH CColoredEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// TODO: Change any attributes of the DC here
	pDC->SetBkColor(m_clrBk);
	pDC->SetTextColor(m_clrText);

	// TODO: Return a non-NULL brush if the parent's handler should not be called
	return m_Brush;
}

/////////////////////////////////////////////////////////////////////////////
// CColoredEditInCombo

CColoredEditInCombo::CColoredEditInCombo(CColoredComboDD* pOwner)
	: m_pOwner(pOwner),
	m_bDropDownListStyle(false)
{
}

CColoredEditInCombo::~CColoredEditInCombo()
{
}


BEGIN_MESSAGE_MAP(CColoredEditInCombo, CColoredEdit)
	//{{AFX_MSG_MAP(CColoredEditInCombo)
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColoredEditInCombo message handlers

void CColoredEditInCombo::OnRButtonDown(UINT nFlags, CPoint point)
{
	//enable context menu only in Drop Down, not in LIST style
	if (m_bDropDownListStyle)
		return;

	CColoredEdit::OnRButtonDown(nFlags, point);
}

void CColoredEditInCombo::OnLButtonDown(UINT nFlags, CPoint point)
{
	//clicking on the edit box in drop down LIST should open the list:
	if (m_bDropDownListStyle)
	{
		m_pOwner->ShowDropDown();
		return;
	}

	CColoredEdit::OnLButtonDown(nFlags, point);
}

void CColoredEditInCombo::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//when the list is open, enter should make the selection:
	if (m_pOwner->GetDroppedState())
	{
		if (nChar == VK_RETURN)
			m_pOwner->ShowDropDown(FALSE);
	}

	CColoredEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

