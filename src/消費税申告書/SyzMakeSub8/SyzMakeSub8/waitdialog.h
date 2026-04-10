#pragma once


// CWaitDialog ダイアログ

class CWaitDialog : public ICSDialog
{
	DECLARE_DYNAMIC(CWaitDialog)

public:
#if _MSC_VER >= 1900
	CWaitDialog(CWnd* pParent = nullptr);   // 標準コンストラクター
#else
	CWaitDialog(CWnd* pParent = NULL);   // 標準コンストラクター
#endif
	virtual ~CWaitDialog();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WAIT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	ICSStatic m_StaticWait;
	virtual BOOL OnInitDialog();
};
