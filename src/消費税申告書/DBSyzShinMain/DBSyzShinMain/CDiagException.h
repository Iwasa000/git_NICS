#pragma once


// CDiagException ダイアログ

class CDiagException : public ICSDialog
{
	DECLARE_DYNAMIC(CDiagException)

public:
	CDiagException(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CDiagException();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_EXCEPTION_CAL };
#endif

	void Dlg_SetCtl();
	void Dlg_GetCtl();
	void Dlg_CheckCtl();

	//void InitInfo(CSnHeadData *pSnHeadData, long symd);
	void InitInfo(CSnHeadData *pSnHeadData, long symd, long eymd);



public:
	


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CButton m_checkuri1;
	CButton m_checkuri2;
	CButton m_checksii1;
	CStatic m_sturi1;
	CStatic m_sturi2;
	CStatic m_stsii1;
	CButton m_ok;
	afx_msg void OnBnClickedCheckUri1();
	afx_msg void OnBnClickedCheckUri2();
	afx_msg void OnBnClickedCheckSii();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();


private:
	CSnHeadData		*m_pSnHeadData;
	long m_symd;
	long m_eymd;

public:
	CStatic m_emsg1;
	CStatic m_emsg2;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CStatic m_wmsg1;
	CButton m_HChk;
	CStatic m_HSt;
	CStatic m_wmsg2;
};
