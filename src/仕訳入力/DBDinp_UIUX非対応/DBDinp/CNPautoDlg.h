#pragma once

#include "icslistinp.h"
#include "SwkJob.h"

// CNPautoDlg ダイアログ

// 仕訳リスト カラム
enum autoClm {
	ACLM_NO = 0,
	ACLM_DEBT,
	ACLM_CRED
};

const int ACLM_MAX = (ACLM_CRED + 1);


class CNPautoDlg : public ICSDialog
{
	DECLARE_DYNAMIC(CNPautoDlg)

public:
	CNPautoDlg(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CNPautoDlg();

	CICSListInp	m_list;

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_NPAUTO };
#endif
	void	Makelistinp();

	void	NPsikinDataSetSub();
	void	NPautoLinpSetSub();
	void	line_colordsp(int idx, BOOL sgn, BOOL initflg = FALSE);

	CArray<NPAUTO_SWK, NPAUTO_SWK&>*	pNpsame;
	CDBINPDataRec*	pCrec;
	int		m_SelIndex;

	CBrush	m_brBack, m_brTitle, m_brWhite;
	COLORREF m_TitleBackCol;
	COLORREF m_TitleTextCol;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnKeyInIcslistinpctrl1(short nChar, short previndex, short index);
	afx_msg void OnLButtonDblClickIcslistinpctrl1(short index);
	afx_msg void OnFocusedIcslistinpctrl1();

	virtual void OnOK();
	void OnSelchangeIcslistinpctrl1(short index);
	void OnLbuttonclickIcslistinpctrl1(short index);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
