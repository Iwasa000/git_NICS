#ifndef INCLUDE_IMAGESTATIC_H
#define INCLUDE_IMAGESTATIC_H

// ImageStatic.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// ImageStatic ウィンドウ

class ImageStatic : public CStatic
{
// コンストラクション
public:
	ImageStatic();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(ImageStatic)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~ImageStatic();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(ImageStatic)
	afx_msg void OnPaint();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	char*	pImage;
	int		lImage;
	int		width, height;
	COLORREF	clBack, clFore, clFocus, clFfore, clTrans;
	int		sgnDel;
	int		sgnPaint;
	CString	textImage;
	BOOL	flgFocus;
	BOOL	flgTrans;
	void	ImageDisplay( CDC* pDC );

public:
	void	SetBackColor( int mode );
	void	SetBackTrans( int mode, COLORREF trans_back );
	void	ClearImage();
	void	SetImage( short x, short y, int lng, void* p );
	void	DeleteLine( int mode );
	void	ImagePaintSw( int mode );
protected:
	virtual void PreSubclassWindow();
};

/////////////////////////////////////////////////////////////////////////////



#endif // INCLUDE_IMAGESTATIC_H