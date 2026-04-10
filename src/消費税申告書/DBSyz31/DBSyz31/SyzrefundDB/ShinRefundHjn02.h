#pragma once
#include "..\icsdiagctrl.h"


// CShinRefundHjn02 ダイアログ

class CShinRefundHjn02 : public CRefundBaseDialog
{
	DECLARE_DYNAMIC(CShinRefundHjn02)

public:
	CShinRefundHjn02(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinRefundHjn02();

// ダイアログ データ
	enum { IDD = IDD_SYZREFUND_HJ2 };

	// 終了処理
	void EndJob();
	// 保存データの更新
	void ReUpdateData();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CIcsdiagctrl m_Meisai3Diag;
	CIcsdiagctrl m_Meisai4Diag;
	CIcsdiagctrl m_TokusyuDiag;
	CIcsdiagctrl m_ShiireDiag;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonDel1();
	afx_msg void OnBnClickedButtonIns1();
	afx_msg void OnBnClickedButtonDel2();
	afx_msg void OnBnClickedButtonIns2();
	afx_msg void OnBnClickedButtonUp1();
	afx_msg void OnBnClickedButtonDown1();
	afx_msg void OnBnClickedButtonUp2();
	afx_msg void OnBnClickedButtonDown2();
};
