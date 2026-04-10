#pragma once


// CTransparentStatic

class CTransparentStatic : public CStatic
{
	DECLARE_DYNAMIC(CTransparentStatic)

public:
	CTransparentStatic();
	virtual ~CTransparentStatic();

	virtual void PreSubclassWindow();

	CBrush	m_Brush;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


