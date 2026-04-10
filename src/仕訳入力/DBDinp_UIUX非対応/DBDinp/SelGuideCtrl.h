#pragma once


// CSelGuideCtrl

#define SELGUIDE_CLASSNAME    _T("SelGuideCtrlClass")  // Window class name


class CSelGuideCtrl : public CWnd
{
	DECLARE_DYNAMIC(CSelGuideCtrl)

public:
	CSelGuideCtrl( BOOL bClickActive = TRUE );
	virtual ~CSelGuideCtrl();

	BOOL RegisterWindowClass();

private:
	int		m_htScrol;

	BOOL	m_bPressD, m_bPressU;

	BOOL	m_bClickActive;	//Click で ウィンドウをアクティブにしない。

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


