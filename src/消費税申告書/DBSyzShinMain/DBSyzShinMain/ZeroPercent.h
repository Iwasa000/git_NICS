#pragma once


// CZeroPercent ダイアログ

class CZeroPercent : public ICSDialog
{
	DECLARE_DYNAMIC(CZeroPercent)

public:
	CZeroPercent(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CZeroPercent();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_ZEROPERCENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnBnClickedButton1();
};
