#pragma once


// CNextShinkokuDlg ダイアログ

class CNextShinkokuDlg : public ICSDialog
{
	DECLARE_DYNAMIC(CNextShinkokuDlg)
	CString mes;
	int flg;
public:
	CNextShinkokuDlg(CString m,int f,CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CNextShinkokuDlg();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_NEXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:

	BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedInvalid();
protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
