#pragma once


// CMnthDlg ダイアログ

class CMnthDlg : public ICSDialog
{
	DECLARE_DYNAMIC(CMnthDlg)

public:
	CMnthDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CMnthDlg();

	ICSDateSelect m_selDate;

// ダイアログ データ
	enum { IDD = IDD_MNTH_DLG };
	BOOL	m_bTan;
	CString	m_st1;

	static	int m_TanOfst;		/* = 単月モード時のオフセット */

	BOOL	m_bKikan;

public:
	// 日付データ
	int	m_Sofs, m_Eofs, m_Sdate, m_Edate;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
