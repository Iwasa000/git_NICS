#pragma once
#include "afxwin.h"

// CShinKskRnd ダイアログ

class CShinKskRnd : public ICSDialog
{
	DECLARE_DYNAMIC(CShinKskRnd)

public:
	CShinKskRnd(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CShinKskRnd();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_KSKRND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	// 初期情報のセット
	int InitInfo(CH31SnKskData *pSnKskData);

public:
	int				m_Rsw;
	int				m_Ksw;
	CH31SnKskData	*m_pSnKskData;		// 更正の請求データ

	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	CButton m_RADIO1;
	CButton m_RADIO2;
	CButton m_CHECK1;
	afx_msg void OnBnClickedCheck1();
	virtual BOOL OnInitDialog();
};
