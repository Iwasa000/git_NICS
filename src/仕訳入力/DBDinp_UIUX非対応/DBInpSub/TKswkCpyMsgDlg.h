#pragma once


// CTKswkCpyMsgDlg ダイアログ

class CTKswkCpyMsgDlg : public ICSDialog
{
	DECLARE_DYNAMIC(CTKswkCpyMsgDlg)

public:
	CTKswkCpyMsgDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CTKswkCpyMsgDlg();

// ダイアログ データ
	enum { IDD = IDD_TKSWK_CPY };

	// 0:一覧選択, 1:上段複写
	int		m_Mode;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	BOOL m_Radio;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
