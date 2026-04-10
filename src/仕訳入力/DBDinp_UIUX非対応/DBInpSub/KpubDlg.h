#pragma once
#include "afxwin.h"


// CKpubDlg ダイアログ

class CKpubDlg : public ICSDialog
{
	DECLARE_DYNAMIC(CKpubDlg)

public:
	CKpubDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CKpubDlg();

// ダイアログ データ
	enum { IDD = IDD_DLG_PUBK };

	CDBZmSub* m_pZm;			// 財務クラス

public:
	// 画面初期化
	int InitDisp();
	// 事業費・管理費大分類名称の読込
	int ReadJigyoKanriMeisyo();
	// 選択事業費＆管理費の取得
	int GetSelJigyoKanri( unsigned long *pJgykcd, unsigned long *pKnrkcd );
	// 初期カーソル科目を設定
	void SetInitKamoku( unsigned long jgykcd, unsigned long knrkcd );

private:
	unsigned long	m_jgykcd;	// 事業費科目コード
	unsigned long	m_knrkcd;	// 管理費科目コード

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_JigyoCmb;
	CComboBox m_KanriCmb;
	afx_msg void OnBnClickedOk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
