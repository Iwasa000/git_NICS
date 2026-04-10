#pragma once
#include "icsdiagctrl.h"
#include "afxwin.h"


// CPrnPrint ダイアログ

class CPrnPrint : public ICSDialog
{
	DECLARE_DYNAMIC(CPrnPrint)

public:
	CPrnPrint(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CPrnPrint();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_PRN_PRINT };

	// 初期情報セット
	int InitInfo( CSnHeadData *pSnHeadData, CDBNpSub *pZmSub, SYC_SHININFO *pShinInfo, SH_PRINT_CMINFO *pPrintCmInfo );
	// 初期化
	int Init();

	// 入力項目設定
	void SetInputItem();
	// 入力項目設定（サブ）
	int SetInputItemData( short idx );
	// 入力項目取得
	int GetInputItemData( short idx );
	// 入力項目色変更
	void ChangeColor( unsigned short id, short index, int sign );

	// 印刷用情報のセット
	int SetPrintInfo();
	// 全選択＆全解除
	void ChoiceItem();
	int m_swBeForked;
private:
	int					m_SelSw;			// 全選択／解除サイン

	CSnHeadData			*m_pSnHeadData;		// 消費税ヘッダ情報
	CDBNpSub			*m_pZmSub;			// 公益クラス
	SYC_SHININFO		*m_pShinInfo;		// 共通情報
	CSyzUtil			m_Util;				// ユーティリティクラス
	SH_PRINT_CMINFO		*m_pPrintCmInfo;	// 印刷共通情報
	EnumIdPrintSelState	m_PrselState;		// 出力指定の状態('15.02.24)

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CIcsdiagctrl m_Diag2;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();

	DECLARE_EVENTSINK_MAP()
	void EditOFFIcsdiag8ctrl2(short index);
	void EditONIcsdiag8ctrl2(short index);
	void TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data);
	void CheckButtonIcsdiag8ctrl2(short index, LPUNKNOWN data);
	void ComboSelIcsdiag8ctrl2(short index, LPUNKNOWN data);

protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedCheckAllout();
	CButton m_AlloutCheck;
};
