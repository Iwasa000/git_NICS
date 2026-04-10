#pragma once


// CTransStatic

class CTransStatic : public CWnd
{
	DECLARE_DYNAMIC(CTransStatic)

public:
	CTransStatic();
	virtual ~CTransStatic();

private:
	long		m_lfWeight;		// フォントウェイト
	COLORREF	m_textClr;		// 文字色
	int			m_SmallFont;	// 小さいフォントで表示

public:
	// フォントウェイトの設定
	void SetFontWeight( long lfWeight );
	// 文字色の設定
	void SetTextColor( COLORREF textClr );
	// 小さいフォントで表示するかの設定
	void IsSmallFont( int sw );

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


