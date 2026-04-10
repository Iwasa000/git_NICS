#pragma once


// CPrnItem ダイアログ

class CPrnItem : public ICSDialog
{
	DECLARE_DYNAMIC(CPrnItem)

public:
	CPrnItem(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CPrnItem();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_PRN_ITEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
