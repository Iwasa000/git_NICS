#pragma once
#include "afxwin.h"


// CRDDnpDlg ダイアログ

class CRDDnpDlg : public ICSDialog
{
	DECLARE_DYNAMIC(CRDDnpDlg)

public:
	CRDDnpDlg(BYTE dtype, int cmnId, CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CRDDnpDlg();

	CButton		m_Zritu;

// ダイアログ データ
	enum { IDD = IDD_DLG_RDDNP };

	BOOL	m_chkVal;

	BYTE	m_Dtype;
	int		m_CmnId;

	BOOL	m_bAddJob;		//追加処理へ
	int		m_dnpDataID;	//使用するデータID

	void	DnpLabelSet();
	int		ListSelDataQuery( BOOL bDataNoquery = FALSE );
	void	KeyDownClickedJob( int nID );
	void	OnBnClickedRdValchk();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox m_List;
	afx_msg void OnBnClickedBtnRegdnp();
	afx_msg void OnBnClickedBtnDnpset();
	afx_msg void OnBnClickedBtnDnpdel();
	afx_msg void OnLbnDblclkList1();
	afx_msg void OnBnClickedBtnRegOVW();
	afx_msg void OnBnClickedBtnChgNam();
	afx_msg void OnBnClickedBtnContxt();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnBnClickedChkZeiritsu();
};
