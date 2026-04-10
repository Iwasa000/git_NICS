#pragma once


// CPrnTune ダイアログ

class CPrnTune : public ICSDialog
{
	DECLARE_DYNAMIC(CPrnTune)

public:
	CPrnTune(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CPrnTune();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_PRN_TUNE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
