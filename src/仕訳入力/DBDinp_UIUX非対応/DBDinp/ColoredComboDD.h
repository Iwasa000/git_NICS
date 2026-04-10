#pragma once


class CColoredComboDD;

/////////////////////////////////////////////////////////////////////////////
// CColoredEdit window

class CColoredEdit : public CEdit
{
	// Construction
public:
	CColoredEdit();

	// Attributes
public:

	// Operations
public:
	void		SetBkColor(COLORREF clrBk)
	{
		m_clrBk = clrBk;
	}

	void		SetTextColor(COLORREF clrText)
	{
		m_clrText = clrText;
	}

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CColoredEdit)
		//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CColoredEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColoredEdit)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	CBrush	m_Brush;
	COLORREF m_clrBk;
	COLORREF m_clrText;
};


class CColoredEditInCombo : public CColoredEdit
{
	// Construction
public:
	CColoredEditInCombo(CColoredComboDD* pOwner);

	void		SetDropListStyle(bool bDropList = true)
	{
		//udate the flag, and then use it in the OnCtlClr, when subclassing the m_Edit
		m_bDropDownListStyle = bDropList;
	}

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CColoredEditInCombo)
		//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CColoredEditInCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColoredEditInCombo)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	bool				m_bDropDownListStyle;
	CColoredComboDD*	m_pOwner;

	DECLARE_MESSAGE_MAP()

};




/////////////////////////////////////////////////////////////////////////////
// CColoredComboDD window



class CColoredComboDD : public CComboBox
{
// Construction
public:
	CColoredComboDD();
	virtual ~CColoredComboDD();

// Attributes
public:

// Operations
public:
	void		SetBkColor(COLORREF clrBk);

	void		SetTextColor(COLORREF clrText)
	{
		m_clrText = clrText;
		m_Edit.SetTextColor( clrText );
	}

	void		SetDropListStyle(bool bDropList = true)
	{
		//udate the flag, and then use it in the OnCtlClr, when subclassing the m_Edit
		m_bDropDownListStyle = bDropList;
		m_Edit.SetReadOnly( m_bDropDownListStyle );
		m_Edit.SetDropListStyle( bDropList );
	}

	void MakeComboList(CStringArray& Text, CDWordArray& data);
	void Refresh(int flg);

	void SetSheetTypeMode(BOOL bMode) {
		m_bSheetType = bMode;
	};

	int GetCountComboList() {
		return m_Text.GetCount();
	}

private:
	void AddList(int flg, CString filter);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColoredComboDD)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CColoredComboDD)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	CColoredEditInCombo	m_Edit;	//the edit box of the combo. i use it ALWAYS becoz
											//i do not allow the Drop List style, bcoz the
											//static cannot be colored when focus lost.
//	CColoredListBox	m_ListBox;
	bool			m_bDropDownListStyle;
	
	CBrush			m_Brush;
	bool			m_bIsButtonClick;

	COLORREF m_clrBk;
	COLORREF m_clrText;

	CStringArray	m_Text;
	CDWordArray		m_data;

	int				m_filtersw;
	BOOL			m_bSheetType;	//TRUE:èëóﬁéÌï åüçıéû

public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus();
	afx_msg void OnKillFocus();
	afx_msg void OnEnable(BOOL bEnable);
};
