#pragma once


// CACDspOptDlg ダイアログ

class CACDspOptDlg : public ICSDialog
{
	DECLARE_DYNAMIC(CACDspOptDlg)

public:
	CACDspOptDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CACDspOptDlg();

	int		m_DenpYes;
	int		m_BmnYes;
	int		m_KojiYes;
	int		m_Hjiyu;
	int		m_Zei;
	int		m_Syz;
	int		m_DispDate;
	CString	m_BaseKnam;

	BOOL	m_bChkQuery;

	CButton	m_OK;
	CButton	m_Cancel;

	// 日付データ
//	int	m_Sofs, m_Eofs, m_Sdate, m_Edate;

// ダイアログ データ
	enum { IDD = IDD_ACDSPOPT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL m_chkBumon;
	BOOL m_chkKoji;
	BOOL m_chkEda;
	BOOL m_chkKikan;

	BOOL m_bEnableBmn;
	virtual void OnOK();
};
