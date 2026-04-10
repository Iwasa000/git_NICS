/*========================================================================
		付箋コンボボックス 
		Lib:	DBInpSub.lib
		DLL:	DBInpSub.dll

  ========================================================================*/
#ifndef INCLUDE_HUSENCOMBO_H
#define INCLUDE_HUSENCOMBO_H

/////////////////////////////////////////////////////////////////////////////
// CHusenCombo ウィンドウ

#ifdef _DBINPSUB
class AFX_EXT_CLASS CHusenCombo : public CComboBox
#else
class CHusenCombo : public CComboBox
#endif
{
// コンストラクション
public:
	CHusenCombo();

// アトリビュート
public:

// オペレーション
public:
	void InitHusenCombo();

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CHusenCombo)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CHusenCombo();

	// 生成されたメッセージ マップ関数
protected:
	CImageList m_imglist;

	//{{AFX_MSG(CHusenCombo)
	//}}AFX_MSG
	afx_msg LRESULT OnSetFont(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};


///////////////////////////////////////////////////////////
// クラスのサイズ変更を避けるため、別に作成 [07.27 /09]
//

#define HUMD_NORMAL		0
#define HUMD_SCAN		1


#ifdef _DBINPSUB
class AFX_EXT_CLASS CHusenComboEX : public CComboBox
#else
class CHusenComboEX : public CComboBox
#endif
{
// コンストラクション
public:
	CHusenComboEX();

// アトリビュート
public:

// オペレーション
public:
	void InitHusenComboEX();
	void SetMode( int mode );

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CHusenComboEX)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CHusenComboEX();

	// 生成されたメッセージ マップ関数
protected:
	CImageList m_imglist;
	int		m_Mode;			// 0=通常, 1=検索用 "付箋なし"を追加
	char	rsv[256];

	//{{AFX_MSG(CHusenCombo)
	//}}AFX_MSG
	afx_msg LRESULT OnSetFont(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};


#endif // INCLUDE_HUSENCOMBO_H

/////////////////////////////////////////////////////////////////////////////
