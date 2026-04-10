// 24/02/19wd_特定収入対応 add -->
#pragma once
#include "icsdiagctrl.h"
#include "afxwin.h"


// CPrnPrint_EX_Sub ダイアログ

class CPrnPrint_EX_Sub : public ICSDialog
{
	DECLARE_DYNAMIC(CPrnPrint_EX_Sub)

public:
	CPrnPrint_EX_Sub(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CPrnPrint_EX_Sub();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_PRN_PRINT_EX_SUB };

private:
	CSnHeadData			*m_pSnHeadData;		// 消費税ヘッダ情報
	CDBNpSub			*m_pZmSub;			// 公益クラス
	SYC_SHININFO		*m_pShinInfo;		// 共通情報
	CSyzUtil			m_Util;				// ユーティリティクラス
	SH_PRINT_CMINFO		*m_pPrintCmInfo;	// 印刷共通情報
	EnumIdPrintSelState	m_PrselState;		// 出力指定の状態
	int					m_swBeForked;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	CIcsdiagctrl m_Diag1;
	CButton m_Check1;

	// 初期情報セット
	int InitInfo(CSnHeadData *pSnHeadData, CDBNpSub *pZmSub, SYC_SHININFO *pShinInfo, 
		SH_PRINT_CMINFO *pPrintCmInfo, EnumIdPrintSelState pPrselState, int pswBeForked);
	// 初期化
	int Init();

	// 入力項目設定
	int SetInputItem(void);
	// 入力制限の設定
	int SetEnableItem(int pSw);
	// 入力項目取得
	int GetInputItemData(void);
	// 入力項目色変更
	void ChangeColor(unsigned short id, short index, int sign);

public:
	DECLARE_EVENTSINK_MAP()
	void EditOFFPrnExSubdg01(short index);
	void TerminationPrnExSubdg01(short index, short nChar, short length, LPUNKNOWN data);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedPenExSubck01();
};
// 24/02/19wd_特定収入対応 add <--
