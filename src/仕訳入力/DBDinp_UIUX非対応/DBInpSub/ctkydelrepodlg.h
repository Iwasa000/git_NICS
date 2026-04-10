#pragma once


// CTkyDelRepoDlg ダイアログ

class CTkyDelRepoDlg : public ICSDialog
{
	DECLARE_DYNAMIC(CTkyDelRepoDlg)

public:
#if _MSC_VER < 1900
	CTkyDelRepoDlg(CWnd* pParent = NULL);   // 標準コンストラクター
#else
	CTkyDelRepoDlg(CWnd* pParent = nullptr);   // 標準コンストラクター
#endif
	virtual ~CTkyDelRepoDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_DELTKY };
#endif

	ICSReportControl	m_Report;
	CDBZmSub*			m_pZm;
	CArray <KMK_REC, KMK_REC>* m_pKmAry;
	CString m_str;
	BOOL	m_bBmn, m_bSyusi;

	void GetBtnLogFont(LOGFONT* plf);
	void InitTkyDelReport();
	void KamokuReportDisp();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
