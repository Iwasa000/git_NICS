#pragma once
#include "afxwin.h"

// CJobEnd ダイアログ

class CDlgJobEnd2 : public ICSDialog
{
	DECLARE_DYNAMIC(CDlgJobEnd2)

public:
	CDlgJobEnd2(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CDlgJobEnd2();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_JOBEND2 };

	int				m_nRet;			// ダイアログ表示処理の戻り値

	int	ShowDialog(LPCTSTR title, LPCTSTR text, LPCTSTR btn1text, LPCTSTR btn2text, LPCTSTR btn3text, LPCTSTR btn4text, UINT type, UINT help, short point, CWnd* pParent);

	CStatic			m_IconPlace;		// アイコン
	ICSStatic		m_StaticMsg2;

private:
	CString			m_strTitle;
	CString			m_strText;

private:
	void ResetDlgWindow( CRect rcOrg, CRect rcResize );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	virtual BOOL OnInitDialog();

};
