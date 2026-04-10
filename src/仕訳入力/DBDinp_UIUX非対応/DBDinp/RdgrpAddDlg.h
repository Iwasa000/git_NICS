#pragma once


// CRdgrpAddDlg ダイアログ

class CRdgrpAddDlg : public ICSDialog
{
	DECLARE_DYNAMIC(CRdgrpAddDlg)

public:
	CRdgrpAddDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CRdgrpAddDlg();

// ダイアログ データ
	enum { IDD = IDD_DLG_RDGRPADD };

public:
	int m_CmnId;
	int	m_LbCnt;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	DECLARE_EVENTSINK_MAP()
	void TerminationIcsdbedtctrl1(long nChar, long inplen, long kst);
};
