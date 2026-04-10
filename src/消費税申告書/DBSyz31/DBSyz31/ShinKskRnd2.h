#pragma once
#include "afxwin.h"

// CShinKskRnd2 ダイアログ

class CShinKskRnd2 : public ICSDialog
{
	DECLARE_DYNAMIC(CShinKskRnd2)

public:
	CShinKskRnd2(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CShinKskRnd2();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_KSKRND2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	// 初期情報のセット
	int InitInfo(CH31SnKskData *pSnKskData, CSnHeadData *pSnHeadData);

public:
	int				m_Rsw;
	int				m_Ksw;
	CH31SnKskData	*m_pSnKskData;		// 更正の請求データ
	CSnHeadData		*m_pSnHeadData;		// ヘッダ情報

	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	CButton m_CHECK1;
	afx_msg void OnBnClickedCheck1();
	virtual BOOL OnInitDialog();
};
