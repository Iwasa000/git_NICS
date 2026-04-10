#pragma once
#include "afxwin.h"


// CKoketsuMeisyo ダイアログ

class CKoketsuMeisyo : public ICSDialog
{
	DECLARE_DYNAMIC(CKoketsuMeisyo)

public:
	CKoketsuMeisyo(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CKoketsuMeisyo();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_KOKETSUMEISYO };

public:
	int m_KHkoke; //個人決算書名称変更取得 0:しない　1:する

public:
	// 初期化
	int Init();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedKoketsumeisyoY();
	afx_msg void OnBnClickedKoketsumeisyoN();
	virtual BOOL OnInitDialog();
	CButton m_RadioY;
	afx_msg void OnBnClickedOk();
};
