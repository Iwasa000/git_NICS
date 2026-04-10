#pragma once
#include "afxcmn.h"


// CSCopyMsgDlg ダイアログ

class CSCopyMsgDlg : public ICSDialog
{
	DECLARE_DYNAMIC(CSCopyMsgDlg)

public:
//	CSCopyMsgDlg(CWnd* pParent = NULL);   // 標準コンストラクタ

	CSCopyMsgDlg(LPCTSTR ermsg, CWnd* pParent = NULL);   // 標準コンストラクタ
	CSCopyMsgDlg();
	virtual ~CSCopyMsgDlg();

	BOOL DestroyWindow();
	BOOL Create(CWnd *pParent);

	void PumpMessages();
	BOOL CheckCancelButton();

	void SetMessageText( LPCTSTR setmsg );
	void SetCopyErrorData(LPCTSTR errmsg);

// ダイアログ データ
	enum { IDD = IDD_DLG_DCOPY };


public:
	CString	m_strOK;
	CString	m_strCANCEL;
	CString	m_strMsg;

	int		m_btnSW[2];
	BOOL	m_bMsgMode;

protected:
	BOOL m_bCancel;
	BOOL m_bParentDisabled;

	void ReEnableParent();
	void MoveCenterWindow(int nID, BOOL bRepaint);

	virtual void OnCancel();
	virtual void OnOK(); 

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CProgressCtrl m_PrgRcnt;
};
