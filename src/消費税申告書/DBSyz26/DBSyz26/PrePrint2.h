#pragma once
#include "afxwin.h"
#include "ExEdit.h"

// CPrePrint2 ダイアログ

class CPrePrint2 : public CDialog
{
	DECLARE_DYNAMIC(CPrePrint2)

public:
	CPrePrint2(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CPrePrint2();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_PREPRINT2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	char	KAIGYO;
	char	MEMO[186];

	//CEdit	m_Edit_Memo;
	CExEdit	m_Edit_Memo;
	CButton m_Check_Kaigyo;

	virtual BOOL OnInitDialog();

	// 初期情報のセット
	int InitInfo( CSnHeadData *pSnHeadData );

private:
	//== 帳票データ本体 ==
	CSnHeadData		*m_pSnHeadData;		// 消費税ヘッダ情報

protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg void OnBnClickedOk();
};
