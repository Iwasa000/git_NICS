#pragma once


// CRdregDlg ダイアログ

class CRdregDlg : public ICSDialog
{
	DECLARE_DYNAMIC(CRdregDlg)

public:
	CRdregDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CRdregDlg();

	CColoredComboDD	m_RDLBL;
	CDataListCtrl	m_DtData;
	BOOL			m_chkVal;

	CWnd*			m_pParent;

// ダイアログ データ
	enum { IDD = IDD_DLG_RDREG };

#ifdef CLOSE
	CDBINPDataRec*	m_pNowData, m_RegData;
#endif

	CDBINPDataRec	m_RegData[RDADD_MAX];
	int				m_recCnt;

	int		m_CmnId;
	int		m_Type;		//0:通常形式, 1:出納帳形式
	
	int		mkrd_data_requery( int num );
	void	SetRddata( int seq, int idx );

	void	SetListColumn( int line );
	void	ACbookListColumn();
	void	MakeDspData( CDBINPDataRec* pRec, int cnt );
	void	ACBK_DspData( CDBINPDataRec* pRec, int cnt );

	int		m_LbCnt;
	BOOL	m_bShowFlag;
	BOOL	m_bOnOKFlag;

	void	DispCount( int sel );

	int		m_TopIndex;
	void	ListDataSet( int topidx );
	void	MakeMkrdLabelCombo();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedRdValchk();
	afx_msg void OnBnClickedBtnGrpadd();
	afx_msg void OnCbnSelchangeRdlblCmbo();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	void	OnLineChanged( NMHDR* pNMHDR, LRESULT* pResult);
	void	OnGetDispInfo( NMHDR* pNMHDR, LRESULT* pResult);
	LRESULT OnSelChgMsg(WPARAM wp, LPARAM lp);
};
