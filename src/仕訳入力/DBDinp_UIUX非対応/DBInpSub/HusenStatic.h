#ifndef INCLUDE_HUSENSTATIC_H
#define INCLUDE_HUSENSTATIC_H


#pragma once


class CHusenStatic : public ICSStatic
{
	DECLARE_DYNAMIC(CHusenStatic)

public:
	CHusenStatic();
	virtual ~CHusenStatic();

	// 付箋の描画インデックス
	void SetIconIndex( int index );


protected:
	CImageList	m_imglist;
	int			m_index;
	HICON		m_hNowIcon;

	DECLARE_MESSAGE_MAP()
public:
	void PreSubclassWindow();

	virtual void DrawCaptionText( CDC* pDC );
	virtual void DrawCaptionIcon(CDC* pDC, CRect& rcItem);
};


#endif	//INCLUDE_HUSENSTATIC_H
