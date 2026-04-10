#pragma once

#include "OwnerButton.h"

// CIndkmkDlg ダイアログ

class CIndkmkDlg : public ICSDialog
{
	DECLARE_DYNAMIC(CIndkmkDlg)

public:
	CIndkmkDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CIndkmkDlg();

// ダイアログ データ
	enum { IDD = IDD_DLG_IND };

	CDBZmSub* m_pZm;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	void	ctrl_set( UINT id );

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	BOOL m_chkIppan;
	BOOL m_chkHudo;
	BOOL m_chkNogyo;
	BOOL m_chkAll;

//	COwnerButton	m_Button[4];

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
};
