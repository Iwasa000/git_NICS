// midori 152718 add -->
#pragma once
#include "icsbutton.h"
#include "afxwin.h"


// CdlgKakuteiKaijyo ダイアログ
// 現在・オプション設定ダイアログ　元々ユーザー設定ダイアログだった

class CdlgKakuteiKaijyo : public ICSDialog
{
	DECLARE_DYNAMIC(CdlgKakuteiKaijyo)

public:
	CdlgKakuteiKaijyo(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CdlgKakuteiKaijyo();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_KAKUTEI_KAIJYO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

private:
	int			m_nRet;					// 戻値
	CDatabase*	m_pDB;					// データベースハンドル
	BOOL		m_Kakutei;				// オプション設定：「入力データを確定する」と共有

public:
	CButton		m_chkDataKakutei;
	ICSButton	m_btnOk;
	ICSButton	m_btnCancel;

public:
	virtual		BOOL OnInitDialog();
	int			ShowDialog(BOOL* bkakutei, CDatabase* pDB = NULL);

protected:
	virtual void OnOK();

public:
	afx_msg		void OnBnClickedButtonOk();
	afx_msg		void OnBnClickedButtonCancel();
};
// midori 152718 add <--
