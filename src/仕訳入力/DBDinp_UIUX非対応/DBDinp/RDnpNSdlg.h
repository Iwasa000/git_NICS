#pragma once


#include "RDdnpBaseDlg.h"

// CRDnpNSdlg ダイアログ

class CRDnpNSdlg : public CRDdnpBaseDlg
{
	DECLARE_DYNAMIC(CRDnpNSdlg)

public:
	CRDnpNSdlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CRDnpNSdlg();

// ダイアログ データ
	enum { IDD = IDD_DLG_RDNPNSKIN };

	CIcsdiag8ctrl	m_diag[3];
	CWnd*	m_pParent;
	int		m_Type;

	virtual	void	diag_inz();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	void	SetDenpyoType( int type );
	void	ChangeDenpType( int type );
};
