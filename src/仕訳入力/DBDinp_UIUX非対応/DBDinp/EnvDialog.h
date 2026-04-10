#pragma once
#include "afxwin.h"


// EnvDialog ダイアログ

class EnvDialog : public ICSDialog
{
	DECLARE_DYNAMIC(EnvDialog)

public:
	EnvDialog(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~EnvDialog();

public:
	CStatic m_text;
	CButton	m_fukugo;
	CButton	m_tanitu;
	CButton	m_SknInp;
	CButton	m_SknNot;
	CButton	m_FuriNormal;
	CButton	m_FuriSyog;
	CButton	m_DnpNoNone;
	CButton	m_DnpNoAuto;
	CButton	m_DnpNoChk;

	CButton	m_BmnClr;
	CButton	m_BmnNotClr;

	BYTE	m_APno;
	BOOL	m_bScan;
	BOOL	m_bBmnTan;

// ダイアログ データ
	enum { IDD = IDD_ENVDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadioSkn1();
	afx_msg void OnBnClickedRadioSkn2();
	afx_msg void OnBnClickedRadioFURI1();
	afx_msg void OnBnClickedRadioFURI2();
	afx_msg void OnBnClickedRadioDenpNoChk();
	afx_msg void OnBnClickedRadioDenpNoChk2();
	afx_msg void OnBnClickedRadioDenpNoAuto();
	afx_msg void OnBnClickedRadioBmn1();
	afx_msg void OnBnClickedRadioBmn2();

public:
	afx_msg void OnReturn();
protected:
	virtual void OnOK();

private:
	int		typeSiwake;
	HACCEL	hAccel;
	void	SetExplain( int mode );

	int		sgnSknBrn;
	int		syogMode;
	int		sgnDenpNo;
	int		sgnBmnClr;	//0=クリアしない, 1=クリアする(デフォルト)

public:
	void	SetSiwakeType( int type ) { typeSiwake	=	type; }
	int		GetSiwakeType() { return typeSiwake; }

	void	SetSknBrnSign( int sgn ) { sgnSknBrn	=	sgn; }
	int		GetSknBrnSign() { return sgnSknBrn; }

	void	SetSyogSiwakeMode( BOOL mode ) { syogMode	=	mode; }
	int		GetSyogSiwakeMode() { return syogMode; }

	void	SetDenpNoCheck( int sgn ) { sgnDenpNo =	sgn; }
	int		GetDenpNoCheck() { return sgnDenpNo; }

	void	SetBmnClrSign(int sgn) { sgnBmnClr = sgn; }
	int		GetBmnClrSign() { return sgnBmnClr; }
};
