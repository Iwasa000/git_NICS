#pragma once
#include "afxwin.h"


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
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
//	static	CDBSyzShinView	*pSyzShin;


	CButton m_Check50;
	afx_msg void OnBnClickedCheck50();
	CButton m_Check51;
	afx_msg void OnBnClickedCheck51();
	CButton m_Check52;
	afx_msg void OnBnClickedCheck52();
	CButton m_Check53;
	afx_msg void OnBnClickedCheck53();
	CButton m_Check54;
	afx_msg void OnBnClickedCheck54();
	CButton m_Check55;
	afx_msg void OnBnClickedCheck55();
	CButton m_Check56;
	afx_msg void OnBnClickedCheck56();
	CButton m_Check57;
	afx_msg void OnBnClickedCheck57();
	CButton m_Check58;
	afx_msg void OnBnClickedCheck58();
	CButton m_Check59;
	afx_msg void OnBnClickedCheck59();
	CButton m_Check60;
	afx_msg void OnBnClickedCheck60();
	CButton m_Check61;
	afx_msg void OnBnClickedCheck61();
//2016.02.23 INSERT START
	CButton m_Check62;
	afx_msg void OnBnClickedCheck62();
//2016.02.23 INSERT END
	CButton m_Check90;
	afx_msg void OnBnClickedCheck90();
	CButton m_Check91;
	afx_msg void OnBnClickedCheck91();
	CButton m_Check70;
	afx_msg void OnBnClickedCheck70();
	CButton m_Check71;
	afx_msg void OnBnClickedCheck71();
	CButton m_Check72;
	afx_msg void OnBnClickedCheck72();
	CButton m_Check73;
	afx_msg void OnBnClickedCheck73();
	CButton m_Check80;
	afx_msg void OnBnClickedCheck80();
	CButton m_Check81;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();

	// 改良依頼 20-0352 修正 add -->
	CButton m_Check110;
	afx_msg void OnBnClickedCheck110();
	CButton m_Check111;
	afx_msg void OnBnClickedCheck111();
	CButton m_Check112;
	afx_msg void OnBnClickedCheck112();
	CButton m_Check113;
	afx_msg void OnBnClickedCheck113();
	// 改良依頼 20-0352 修正 add <--

private:
	int				m_SelSw;			// 全選択／解除サイン

	CSnHeadData		*m_pSnHeadData;		// 消費税ヘッダ情報
	CDBNpSub		*m_pZmSub;			// 公益クラス
	SYC_SHININFO	*m_pShinInfo;		// 共通情報
	CSyzUtil		m_Util;				// ユーティリティクラス

	SH_PRINT_CMINFO	*m_pPrintCmInfo;	// 印刷共通情報
	int				m_swBeForked;

public:
	// 初期情報セット
	int InitInfo( CSnHeadData *pSnHeadData, CDBNpSub *pZmSub, SYC_SHININFO *pShinInfo, SH_PRINT_CMINFO *pPrintCmInfo, int pBeForked );

	void ChoiceItem( void );	// 全選択＆全解除	
	void ItemInit( void );		// 初期化
	void SetJSMoede();			// 事務所名縮小のセット
	bool IsShowHojinNoChk();	// 法人番号チェックを表示？（'17.03.29）
	
	CButton m_Check63;
	afx_msg void OnBnClickedCheck63();
protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CButton m_Check74;
	CButton m_Check75;
	afx_msg void OnBnClickedCheck74();
	afx_msg void OnBnClickedCheck75();
	CButton m_Check64;
	afx_msg void OnBnClickedCheck64();
	afx_msg void OnBnClickedCheck65();
	CButton m_Check65;
	CButton m_Check101;
	afx_msg void OnBnClickedCheck101();
	CButton m_Check66;
	afx_msg void OnBnClickedCheck66();
	afx_msg void OnBnClickedCheck67();
	CButton m_Check67;
	CButton m_Check68;
	afx_msg void OnBnClickedCheck68();
};