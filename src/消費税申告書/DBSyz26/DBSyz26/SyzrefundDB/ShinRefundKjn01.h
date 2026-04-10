#pragma once

#include "..\icsdiagctrl.h"


// CShinRefundKjn01 ダイアログ

class CShinRefundKjn01 : public CRefundBaseDialog
{
	DECLARE_DYNAMIC(CShinRefundKjn01)

public:
	CShinRefundKjn01(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinRefundKjn01();

// ダイアログ データ
	enum { IDD = IDD_SYZREFUND_KJ1 };

	// 終了処理
	void EndJob();

//2016.02.23 UPDATE START
//	void	KinyuuDlgKjn( int sw );
	int		KinyuuDlgKjn( int sw );
//2016.02.23 UPDATE END
	// 保存データの更新
	void	ReUpdateData();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CIcsdiagctrl m_RiyuuDiag;
	CIcsdiagctrl m_Meisai1Diag;
	CIcsdiagctrl m_Meisai2Diag;
	CIcsdiagctrl m_YusyutsuDiag;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonDel1();
	afx_msg void OnBnClickedButtonIns1();
	afx_msg void OnBnClickedButtonDel2();
	afx_msg void OnBnClickedButtonIns2();
};
