#pragma once
#include "afxwin.h"


// CGyousyu ダイアログ

class CGyousyu : public ICSDialog
{
	DECLARE_DYNAMIC(CGyousyu)

public:
	CGyousyu(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CGyousyu();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_GYOUSYU };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

public:
	int	m_Gyousyu;
	int	m_Refund;

	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	CButton m_RADIO1;
	CButton m_RADIO2;
	CButton m_RADIO3;
	CStatic m_STATICTYPE;
};
