#pragma once


#include "RDdnpBaseDlg.h"

// CRDnpFuriDlg ダイアログ

class CRDnpFuriDlg : public CRDdnpBaseDlg
{
	DECLARE_DYNAMIC(CRDnpFuriDlg)

public:
	CRDnpFuriDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CRDnpFuriDlg();

	CIcsdiag8ctrl	m_diag[3];
	CWnd*			m_pParent;

// ダイアログ データ
	enum { IDD = IDD_DLG_RDNPFURI };

	virtual void diag_inz();

	void syog_iniz();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

};
