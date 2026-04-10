// ExtTab.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CExtTab ウィンドウ

class CExtTab : public CTabCtrl
{
// コンストラクション
public:
	CExtTab();

// アトリビュート
public:

private:
	COLORREF m_crSelColour, m_crUnselColour;	// 選択・非選択の色


// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CExtTab)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CExtTab();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CExtTab)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
