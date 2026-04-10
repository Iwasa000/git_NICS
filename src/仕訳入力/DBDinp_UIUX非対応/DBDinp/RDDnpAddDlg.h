#pragma once


// CRDDnpAddDlg ダイアログ

class CRDDnpAddDlg : public ICSDialog
{
	DECLARE_DYNAMIC(CRDDnpAddDlg)

public:
	CRDDnpAddDlg(BYTE dtype, BOOL bNameChg = FALSE, CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CRDDnpAddDlg();

// ダイアログ データ
	enum { IDD = IDD_DLG_RDDNPADD };

	BYTE	m_Dtype;

	CString	m_LabelName;
	BOOL	m_bNameChg;
	int		m_AbsPos;

	BOOL	m_bOverWrite;
	CString	m_strOverName;

	void	BackDnpLabel( int back_lbseq );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	DECLARE_EVENTSINK_MAP()
	void TerminationIcsdbedtctrl1(long nChar, long inplen, long kst);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
