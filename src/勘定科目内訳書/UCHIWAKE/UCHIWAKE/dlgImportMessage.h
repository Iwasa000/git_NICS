#pragma once
#include "icsstatic.h"


// CdlgImportMessage ダイアログ

class CdlgImportMessage : public ICSDialog
{
	DECLARE_DYNAMIC(CdlgImportMessage)

public:
	CdlgImportMessage(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CdlgImportMessage();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_IMPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	ICSStatic m_lblMessage;
	virtual BOOL OnInitDialog();

public:
	// 2010/08/30 業務統合メニュー マルチウィンドウ対応
	// 本メッセージハンドルが無いため、デスクトップ中央に表示される現象対応
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
