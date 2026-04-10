#pragma once


// CRdremainDlg ダイアログ

class CRdremainDlg : public ICSDialog
{
	DECLARE_DYNAMIC(CRdremainDlg)

public:
	CRdremainDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CRdremainDlg();

// ダイアログ データ
	enum { IDD = IDD_DLG_RDREMAIN };

	CWnd*	m_pParent;
	void	SetRemainString(CString& str, int height);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void PostNcDestroy();
	virtual void PreSubclassWindow();
};
