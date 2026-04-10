#pragma once
#include "icsppst8.h"


// CPrnReportDlg ダイアログ

class CPrnReportDlg : public ICSDialog
{
	DECLARE_DYNAMIC(CPrnReportDlg)

public:
	CPrnReportDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CPrnReportDlg();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_PRN_REPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	ICSTabCtrl m_PrnMainTab;
	virtual BOOL OnInitDialog();
};
