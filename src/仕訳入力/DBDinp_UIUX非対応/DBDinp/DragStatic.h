#pragma once


#include "BmpDspWnd.h"


// CDragStatic
#ifndef WS_EX_LAYERED    
#define WS_EX_LAYERED			0x00080000
#endif
#ifndef ULW_ALPHA    
#define ULW_ALPHA				0x00000002    
#endif
#ifndef NOFADE    
#define NOFADE					255
#endif


class CDragStatic : /*public CStatic*/
					public CBmpDspWnd
{
	DECLARE_DYNAMIC(CDragStatic)

public:
//	CDragStatic();
	CDragStatic( CWnd* pParent );
	virtual ~CDragStatic();

	void SetDragButtons(BOOL bDragLeft, BOOL bDragRight);
	void KeepDialogInScreen(BOOL bKeepDialogInScreen);
	BOOL ActivateDrag();
	BOOL DeactivateDrag();
	void UpdateScreenMetrics();
	void GetDialogBackToScreen();
	BOOL SetDragCursor(UINT nID);
	void UseDragCursor(BOOL bUseDragCursor);
	BOOL SetFadeWhileDragging(int nFade);

protected:
	DECLARE_MESSAGE_MAP()

	void FadeDialog(BOOL bFade);
	void DrawDragCursor();
	int m_nFadeWhileDragging;
	int m_nOldMouseX;
	int m_nOldMouseY;
	int m_nCtrlX;
	int m_nCtrlY;
	BOOL m_bDragLeft;
	BOOL m_bDragRight;
	BOOL m_bKeepDialogInScreen;
	BOOL m_bUseDragCursor;
	BOOL m_bDrag;
	HCURSOR	m_hCursor;

	BOOL	m_bBkTransMode;

	CWnd*	m_pParentWnd;

	CRect	m_OldDragRect;
	LPCRECT	m_pRect;

	LPARAM	m_lPara;


public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnPaint();
};


