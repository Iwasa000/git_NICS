// RevTest8Dlg.h : ヘッダー ファイル
//

#pragma once
#include "afxwin.h"


// CRevTest8Dlg ダイアログ
class CRevTest8Dlg : public CDialog
{
// コンストラクション
public:
	CRevTest8Dlg(CWnd* pParent = NULL);	// 標準コンストラクタ

// ダイアログ データ
	enum { IDD = IDD_REVTEST8_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


	static CRevConv	m_RV;
	static CRevConv	m_RV2;

// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_Edit;
	BOOL m_HanChk;
	BOOL m_KanaChk;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
