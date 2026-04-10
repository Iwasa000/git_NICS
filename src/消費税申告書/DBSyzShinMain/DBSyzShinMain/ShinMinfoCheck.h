#pragma once


// CShinMinfoCheck ダイアログ

class CShinMinfoCheck : public ICSDialog
{
	DECLARE_DYNAMIC(CShinMinfoCheck)
	HICON  hIcon;

public:
	CShinMinfoCheck(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinMinfoCheck();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_MCHECK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
};
