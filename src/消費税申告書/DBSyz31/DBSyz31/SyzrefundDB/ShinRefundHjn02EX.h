#pragma once
#include "..\icsdiagctrl.h"

// CShinRefundHjn02EX ダイアログ

class CShinRefundHjn02EX : public CRefundBaseDialog
{
	DECLARE_DYNAMIC(CShinRefundHjn02EX)

public:
	CShinRefundHjn02EX(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinRefundHjn02EX();

// ダイアログ データ
	enum { IDD = IDD_SYZREFUND_HJ2_EX };

	// 終了処理
	void EndJob();
	// 保存データの更新
	void ReUpdateData();
	// diagデータをSetDiagControlMapにセット
	void SetDiagMap();
	// diag間のキー移動の設定
	void SetKeyMove();

	// フォーカス移動
	void MoveFocus();

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
