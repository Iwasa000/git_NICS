#pragma once
//#include "COCX.h"

// 24/03/21_税務代理書面 add -->
#include "PrnPrintSubIdx.h"
// 24/03/21_税務代理書面 add <--

// PrintZeitempSub ダイアログ

class PrintZeitempSub : public ICSDialog
{
	DECLARE_DYNAMIC(PrintZeitempSub)

public:
	PrintZeitempSub(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~PrintZeitempSub();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PRN_ZEITEMP_SUB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()


private:
	CWnd*			m_pParent;
	int				m_SelSw;			// 全選択／解除サイン

	CSnHeadData		*m_pSnHeadData;		// 消費税ヘッダ情報
	CDBNpSub		*m_pZmSub;			// 公益クラス
	SYC_SHININFO	*m_pShinInfo;		// 共通情報
	CSyzUtil		m_Util;				// ユーティリティクラス

	SH_PRINT_CMINFO	*m_pPrintCmInfo;	// 印刷共通情報
	int				m_swBeForked;


	int		Sn_PrintOutSgn_ZSub;		// 税務代理帳票個別指定　　　出力サイン　チェックボックス
	int		Sn_PrintOutSgn_ZNum;		// 税務代理帳票個別指定　部数
	long	Sn_PrintOutSgn_Zeitemp;	// 税務代理帳票の出力サイン('24.03.08)
									// D0=ON:税務代理権限証書
									// D1=ON:税理士法第33条の2第1項に規定する添付書面（第1面）
									// D2=ON:税理士法第33条の2第1項に規定する添付書面（第2面）
									// D3=ON:税理士法第33条の2第1項に規定する添付書面（第3面）
									// D4=ON:税理士法第33条の2第1項に規定する添付書面（第4面　1枚目）
									// D5=ON:税理士法第33条の2第1項に規定する添付書面（第4面　2枚目）
									// D6=ON:税理士法第33条の2第2項に規定する添付書面（第1面）
									// D7=ON:税理士法第33条の2第2項に規定する添付書面（第2面）

									// D8=ON:税理士法第33条の2第2項に規定する添付書面（第3面）
									// D9=ON:税理士法第33条の2第2項に規定する添付書面（第4面　1枚目）
									// D10=ON税理士法第33条の2第2項に規定する添付書面（第4面　2枚目）:
									// D11=ON:
									// D12=ON:
									// D13=ON:
									// D14=ON:
									// D15=ON:

// 24/03/21_税務代理書面 add -->
	int		m_Bunbo[ZEI_MAXCOL];
// 24/03/21_税務代理書面 add <--

	int		m_Yy;					// 先頭行のindex	修正No.168895 add


public:
	// 初期情報セット
	int InitInfo(CSnHeadData *pSnHeadData, CDBNpSub *pZmSub, SYC_SHININFO *pShinInfo, SH_PRINT_CMINFO *pPrintCmInfo, int pBeForked, int Ctlsw);
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

	//// 印刷用情報のセット
	//void SetPrintInfo();
	// 全選択＆全解除
	BOOL ChoiceItem(int pSw);

// 24/07/10_個別タブから出力 cor -->
	////汎用
	//int SetInputItemSub(int CtIDCh, int CtIDNum, PRNEXSUB_INF *pSubInf, DIAGRAM_DATA *pdd);
	//int SetColorItemSub(int ck, int CtIDCh, int CtIDNum);
	////データをMapに取得
	//int GetInputItemDataSub(int CtIDCh, int CtIDNum, PRNEXSUB_INF *SubInf, DIAGRAM_DATA	*pdd);
// ---------------------------------
	// 汎用
	int SetInputItemSub(int CtIDCh, int CtIDBus, int CtIDNum, PRNEXSUB_INF *pSubInf, DIAGRAM_DATA *pdd);
	int SetColorItemSub(int ck, int CtIDCh, int CtIDBus, int CtIDNum);
	// データをMapに取得
	int GetInputItemDataSub(int CtIDCh, int CtIDBus, int CtIDNum, PRNEXSUB_INF *SubInf, DIAGRAM_DATA *pdd);
// 24/07/10_個別タブから出力 cor <--

	//枚数の列再セット
	void ReSetAllNumDg();
// 24/03/22_背景色 add -->
	bool IsAllCheckOn( void );
// 24/03/22_背景色 add <--
// 24/04/03_checkon add -->
	void AllCheckOn( void );
// 24/04/03_checkon add <--
// 24/04/03_開発員3 add -->
	bool IsDiagEnabled( void );
// 24/04/03_開発員3 add <--

	int		m_Ctlsw;			//コントロール状態情報
	int		m_PoutName;			// 
	int		m_PoutAdd;			// 
	int		m_PoutDensi;		// 

public:
	DECLARE_EVENTSINK_MAP()
	void EditOFFPrnZeitempSub(short index);
	void TerminationPrnZeitempSub(short index, short nChar, short length, LPUNKNOWN data);

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton7();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CIcsdiagctrl m_Diag_Zei;
	CButton m_Check_Name;
	CButton m_Check_Add;
	CButton m_Check_Den;
	CButton m_Check_All;
	afx_msg void OnBnClickedZeimudairiname();
	afx_msg void OnBnClickedZeimudairiaddress();
	afx_msg void OnBnClickedZeimudairiDensi();
	afx_msg LRESULT OnMyFocusControl(WPARAM wParam, LPARAM lParam);
	void CheckButtonPrnZeitempSubDg(short index, LPUNKNOWN data);
public:
	afx_msg void OnBnClickedZeiAll();
};
