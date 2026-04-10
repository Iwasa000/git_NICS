#pragma once


// CACInpSupDlg ダイアログ

class CACInpSupDlg : public ICSDialog
{
	DECLARE_DYNAMIC(CACInpSupDlg)

public:
	CACInpSupDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CACInpSupDlg();

// ダイアログ データ
	enum { IDD = IDD_ACINPSUP_DLG };

	CButton	m_OK;
	int		m_ValFix;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void OnOK();

	void	OnBnClickedChkValcopy();
	void	OnBnClickedChkValnot();
	void	ValCheckCtrl( UINT ID );
};
