// OwnerButton.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CIconOwnerButton ウィンドウ

#ifndef _MYICONOWNERBTN
#define _MYICONOWNERBTN

typedef enum	{	
	ID_NORMAL_COLORBTN_STYLE	=	0,	// 通常ボタン 
	ID_LINKDISP_COLORBTN_STYLE			// 関連付け有り リンクボタン
} EnumIdColorBtnStyle;


class CIconOwnerButton : public CButton
{
// コンストラクション
public:
	CIconOwnerButton();

// アトリビュート
public:

// オペレーション
public:
	void DrawRect( CDC*, CRect, COLORREF );
	void DrawLine( CDC*, CRect, COLORREF);
	void DrawLine( CDC*, long, long, long, long, COLORREF );
	void DrawText( CDC*, CRect, const char*, BOOL, COLORREF);
	void DrawFrame( CDC*, CRect, BOOL);
	// 丸角を描画する
	void DrawRoundRect( CDC* pDC, CRect rc, BOOL bSelected );
	void DrawRect( CDC* pDC, CRect rc, BOOL bSelected );
	// 枠の中身を塗る ('12.10.01)
	void DrawRect( CDC* pDC, CRect rc, COLORREF upClr, COLORREF dnClr );

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CIconOwnerButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL


// インプリメンテーション
public:
	virtual ~CIconOwnerButton();

	DWORD	SetCheck(int nCheck, BOOL bRepaint = TRUE );
	int		GetCheck();

	// ボタンの表示タイプをセットする ('12.10.01)
	int	SetBtnDispStyle( EnumIdColorBtnStyle style );


	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CIconOwnerButton)
	//}}AFX_MSG
	LRESULT OnSetCheck(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetCheck(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();

protected:
	BOOL					m_bIsCheckBox;
	int						m_nCheck;
	BOOL					m_bIsDefault;		// Is default button?
	UINT					m_nTypeStyle;
	BOOL					m_bIsPressed;
	EnumIdColorBtnStyle		m_ColorBtnStyle;	// カラーボタンスタイル
public:
	afx_msg BOOL OnBnClicked();
};




#endif

/////////////////////////////////////////////////////////////////////////////
