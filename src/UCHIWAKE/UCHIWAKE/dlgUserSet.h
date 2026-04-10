#pragma once
#include "icsbutton.h"
#include "afxwin.h"


// CdlgUserSet ダイアログ
// 現在・オプション設定ダイアログ　元々ユーザー設定ダイアログだった

class CdlgUserSet : public ICSDialog
{
	DECLARE_DYNAMIC(CdlgUserSet)

public:
	CdlgUserSet(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CdlgUserSet();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_OPTION_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

private:
	int			m_nRet;					//　戻値	
	USER_OPTION_INFO m_uoi;
	CDatabase*	m_pDB;					//	データベースハンドル

public:
	ICSButton m_btnOk;
	ICSButton m_btnCancel;
	CButton	m_chkMoveMode;
	CButton	m_chkPrintDlg;
	CButton	m_chkDispYoushikiSelect;

public:
	afx_msg		void OnBnClickedButtonOk();
	afx_msg		void OnBnClickedButtonCancel();	
	virtual		BOOL OnInitDialog();
	int			ShowDialog(USER_OPTION_INFO& uoi, CDatabase* pDB = NULL);

private:
	//virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	virtual void OnOK();
public:
	CButton m_chkEnterMoveDown;
// midori 160612 add -->
	CButton m_chkDataKakutei;
// midori 160612 add <--
// midori 190301 add -->
	CButton m_chkUseStyle;
// midori 190301 add <--
	CButton m_chkUseStyle2;		// インボイス登録番号追加対応_23/11/06 add

// midori 151401 add -->
	afx_msg void OnBnClickedCheckDataKakutei();
// midori 151401 add <--
// midori UC_0047 add -->
	afx_msg void OnBnClickedCheckUseStyle();
// midori UC_0047 add <--
	afx_msg void OnBnClickedCheckUseStyle2();	// インボイス登録番号追加対応_23/11/06 add
};
