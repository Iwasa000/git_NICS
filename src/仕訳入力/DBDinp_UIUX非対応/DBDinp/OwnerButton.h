// OwnerButton.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// COwnerButton ウィンドウ

#ifndef _MYOWNERBTN
#define _MYOWNERBTN


class COwnerButton : public CButton
{
// コンストラクション
public:
	COwnerButton();

// アトリビュート
public:

// オペレーション
public:
	void DrawRect(CDC*, CRect, COLORREF );
	void DrawLine(CDC*, CRect, COLORREF);
	void DrawLine(CDC*, long, long, long, long, COLORREF );
	void DrawText(CDC*, CRect, const char*, BOOL, COLORREF);
	void DrawFrame(CDC*, CRect, BOOL);


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(COwnerButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL


// インプリメンテーション
public:
	virtual ~COwnerButton();

	DWORD	SetCheck(int nCheck, BOOL bRepaint = TRUE );
	int		GetCheck();


	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(COwnerButton)
	//}}AFX_MSG
	LRESULT OnSetCheck(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetCheck(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();

protected:
	BOOL	m_bIsCheckBox;
	int		m_nCheck;
	BOOL	m_bIsDefault;		// Is default button?
	UINT	m_nTypeStyle;
	BOOL	m_bIsPressed;
public:
	afx_msg BOOL OnBnClicked();
};




#endif

/////////////////////////////////////////////////////////////////////////////
