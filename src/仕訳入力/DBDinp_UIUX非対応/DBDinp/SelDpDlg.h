#pragma once
#include "afxwin.h"


// SelDpDlg ダイアログ

class SelDpDlg : public ICSDialog
{
	DECLARE_DYNAMIC(SelDpDlg)

public:
	SelDpDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~SelDpDlg();

// ダイアログ データ
	enum { IDD = IDD_DNPSELDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnBnClickedButton2();
public:
	afx_msg void OnBnClickedButton3();
public:
	virtual BOOL OnInitDialog();

public:
	ICSButton m_btn1;
	ICSButton m_btn2;
	ICSButton m_btn3;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void OnOK();
};
