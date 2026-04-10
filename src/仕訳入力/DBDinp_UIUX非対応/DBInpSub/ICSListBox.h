// ICSListBox.h : ヘッダー ファイル
//
#ifndef INCLUDE_ICSLISTBOX_H
#define INCLUDE_ICSLISTBOX_H

/////////////////////////////////////////////////////////////////////////////
// ICSListBox ウィンドウ

class ICSListBox : public CListBox
{
// コンストラクション
public:
	ICSListBox();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(ICSListBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~ICSListBox();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(ICSListBox)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	short	Mode;
	int		Position;
	BOOL	ClickDown;

public:
	enum {
		CLICKMODE_ONECLICK	=	1,
		CLICKMODE_TWOCLICK	=	2,
	};
	void	ClickMode( short mode ) { Mode	=	mode; }
};

/////////////////////////////////////////////////////////////////////////////



#endif // INCLUDE_ICSLISTBOX_H