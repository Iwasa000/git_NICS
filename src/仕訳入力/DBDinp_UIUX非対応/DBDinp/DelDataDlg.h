#pragma once


// CDelDataDlg ダイアログ

#define WM_STARTSIWAKE		(WM_APP+111)


class CDelDataDlg : public ICSDialog
{
	DECLARE_DYNAMIC(CDelDataDlg)

public:
	CDelDataDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CDelDataDlg();

// ダイアログ データ
	enum { IDD = IDD_DLG_DELSWK };

	CButton	m_Ikat;
	CButton	m_Modify;
	CButton	m_NoMdfy;
	CButton	m_End;
	CDataListCtrl	m_DtData;

	BOOL& IsDataBack() { return m_bDataBack; };

protected:
	BOOL m_bShow;
	BOOL m_bData;	// ﾃﾞｰﾀの有り無し
	BOOL m_bRepeat;	// ﾃﾞｰﾀ一括復帰
	BOOL m_bDataBack;

	CDBINPDataRec	m_Data;
	int	 m_DataPos;	// CData の AbsolutePosition

	void SetListColumn( int line );
	void MakeDspData( CDBINPDataRec* pdt );
	
//	void DspDataClear();
//	int RpDataRead(int start_no);
//	void SetRpdata(int sno, LPCTSTR msg);

	void NextDataSet( int nxt_sg, CMsgDlg* msgdlg );
	void SetBtnCondition(BOOL flg, int all);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	LRESULT OnStartSiwake(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
public:
	afx_msg void OnBnClickedBtnIkatu();
public:
	afx_msg void OnBnClickedBtnEnd();
public:
	afx_msg void OnBnClickedBtnNomdfy();
public:
	afx_msg void OnBnClickedBtnOk();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
