#pragma once
#include "afxwin.h"


// CTaxtionSelect ダイアログ

class CTaxtionSelect : public CDialog
{
	DECLARE_DYNAMIC(CTaxtionSelect)

public:
	CTaxtionSelect(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CTaxtionSelect();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_TAXSEL };

public:
	int	m_KHstax;	// 税処理方式

public:
	// 初期化
	int Init();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CButton m_Radio1;
	CButton m_Radio2;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	virtual BOOL OnInitDialog();
};
