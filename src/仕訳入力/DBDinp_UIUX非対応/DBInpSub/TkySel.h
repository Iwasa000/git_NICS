#pragma once

#include "TkEdit.h"

// CTkySel ダイアログ

class CTkySel : public ICSDialog
{
	DECLARE_DYNAMIC(CTkySel)

public:
	CTkySel(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CTkySel();

// ダイアログ データ
	enum { IDD = IDD_DLG_TKYSEL };
	CTkEdit	m_Edit;
	CButton	m_EBLeft;
	CButton	m_EBRight;
	CButton	m_EBAdd;
	CButton	m_EBSub;
	CButton m_EBHome;

	CBrush	m_White;
	char	m_Teki[128];

	int		m_TKYO_LEN;

	static	CString	m_orgTky;
	static	int		m_orgPos;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnHomebutton();
	afx_msg void OnLeftbutton();
	afx_msg void OnRightbutton();
	afx_msg void OnSubbutton();
	afx_msg void OnAddbutton();
protected:
	virtual void OnOK();
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
