#pragma once


// CBmpDspWnd

class CBmpDspWnd : public CWnd
{
	DECLARE_DYNAMIC(CBmpDspWnd)

public:
	CBmpDspWnd( CWnd* pParent );
	virtual ~CBmpDspWnd();

	void SetBitmapID( int bmpid );
	int  GetBitmapID() { return m_nBmpID; };

	BOOL&	DspWndSelected() { return m_bSelect; };
	BOOL&	CursorChangeFlg() { return m_bCursor; };
	void	SetDispBitmap( CBitmap* pBmp, CRect bmRect, CString& txt );

	void	PaintBmpSub( CPaintDC* pDC );
	void	GetBitmapRect( CRect& bmRC ) { bmRC = m_bmRect; };

	COLORREF	m_Back, m_Waku;
	CString		m_Txt;

	int			m_nID;

protected:
	DECLARE_MESSAGE_MAP()

	CWnd*		m_pParentWnd;
	int			m_nBmpID;
	CBitmap*	m_pDispBmp;
	CRect		m_bmRect;

	BOOL		m_bSelect;

	BOOL		m_bCursor;
	HCURSOR		m_hHandCursor;

	BOOL		m_bMouseOver;

public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
};


