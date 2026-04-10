#pragma once
#include "afxwin.h"

class CColorComboBox :
	public CComboBox
{
	int m_ColorChange;

public:
	int& GetSetColorChange(){ return m_ColorChange; }

	CColorComboBox();
	virtual ~CColorComboBox();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
