#pragma once
//#include "COCX.h"

// 24/03/21_税務代理書面 add -->
#include "PrnPrintSubIdx.h"
// 24/03/21_税務代理書面 add <--

// PrintTokuteiSub ダイアログ

class PrintTokuteiSub : public ICSDialog
{
	DECLARE_DYNAMIC(PrintTokuteiSub)

public:
	PrintTokuteiSub(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~PrintTokuteiSub();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PRN_TOKUTEI_SUB };
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


	int		Sn_PrintOutSgn_TSub;		//特定収入計算表個別指定　　　出力サイン　チェックボックス
	int 	Sn_PrintOutSgn_TNum;		//特定収入計算表個別指定　部数	
	long	Sn_PrintOutSgn_Tokutei;	// 特定収入計算表の出力サイン ('24.03.08)
									// D0=ON:計算式１
									// D1=ON:計算式２（１）
									// D2=ON:計算式２（２）
									// D3=ON:計算式３，４
									// D4=ON:計算式５（１）
									// D5=ON:計算式５（２） 
									// D6=ON:計算式５（３）
									// D7=ON:計算式５－２（１）

									// D8=ON:計算式５－２（２）
									// D9=ON:計算式５－２（３）－１
									// D10=ON:計算式５－２（３）－２
									// D11=ON:計算式５－２（３）－３
									// D12=ON:計算式５－２（４）－１
									// D13=ON:計算式５－２（４）－２
									// D14=ON:計算式５－２（４）－３
									// D15=ON:
// 24/03/22_168718 add -->
	int				m_Sel[256];
// 24/03/22_168718 add <--

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
	//入力項目色変更
	void ChangeColor(unsigned short id, short index, int sign);

	//// 印刷用情報のセット
	//void SetPrintInfo();
	// 全選択＆全解除
	BOOL ChoiceItem(int pSw);

// 24/07/10_個別タブから出力 cor -->
	////汎用
	//int SetInputItemSub(int CtIDCh, int CtIDNum, int CtIDSp, int CtIDEp, PRNEXSUB_INF *pSubInf, DIAGRAM_DATA *pdd);
	//int SetColorItemSub(int ck, int nflg, int CtIDCh, int CtIDNum, int CtIDSp, int CtIDEp, int op);
	////データをMapに取得
	//int GetInputItemDataSub(int CtIDCh, int CtIDNum, int CtIDSp, int CtIDEp, PRNEXSUB_INF *SubInf, DIAGRAM_DATA	*pdd);
// ---------------------------------
	// 汎用
	int SetInputItemSub(int nflg, int CtIDCh, int CtIDBus, int CtIDNum, int CtIDSp, int CtIDEp, PRNEXSUB_INF *pSubInf, DIAGRAM_DATA *pdd);
	int SetColorItemSub(int ck, int nflg, int CtIDCh, int CtIDBus, int CtIDNum, int CtIDSp, int CtIDEp, int op);
	// データをMapに取得
	int GetInputItemDataSub(int CtIDCh, int CtIDBus, int CtIDNum, int CtIDSp, int CtIDEp, PRNEXSUB_INF *SubInf, DIAGRAM_DATA *pdd);
// 24/07/10_個別タブから出力 cor <--

	//枚数の列再セット
	void ReSetAllNumDg();
// 24/03/22_背景色 add -->
	bool IsAllCheckOn( void );
// 24/03/22_背景色 add <--
// 24/04/03_checkon add -->
	void AllCheckOn( void );
// 24/04/03_checkon add <--

// 24/03/21_印刷枚数 add -->
	// 印刷枚数の取得
	void GetBunbo( int pSw );

	int		m_Bunsi[TOK_MAXCOL];
	int		m_Bunbo[TOK_MAXCOL];
// 24/03/21_印刷枚数 add <--

	int		m_Ctlsw;			//コントロール状態情報
	int		m_PoutKcode;		// 会社コード出力

public:
	DECLARE_EVENTSINK_MAP()
	void EditOFFPrnTokuteiSub(short index);
	void TerminationPrnTokuteiSub(short index, short nChar, short length, LPUNKNOWN data);

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton7();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CIcsdiagctrl m_Diag_Tok;
	CButton m_Check_Tok;
	afx_msg void OnBnClickedTokuteicode();
	afx_msg LRESULT OnMyFocusControl(WPARAM wParam, LPARAM lParam);
	void CheckButtonPrnTokuteiSubDg(short index, LPUNKNOWN data);
public:
	afx_msg void OnBnClickedTokAll();
};
