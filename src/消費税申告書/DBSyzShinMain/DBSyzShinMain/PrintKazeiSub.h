#pragma once
//#include "COCX.h"

// 24/03/21_税務代理書面 add -->
#include "PrnPrintSubIdx.h"
// 24/03/21_税務代理書面 add <--

// PrintKazeiSub ダイアログ

class PrintKazeiSub : public ICSDialog
{
	DECLARE_DYNAMIC(PrintKazeiSub)

public:
	PrintKazeiSub(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~PrintKazeiSub();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PRN_KAZEI_SUB };
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

	int 	Sn_PrintOutSgn_KSub;	//課税取引金額計算表個別指定　出力サイン　チェックボックス
	int 	Sn_PrintOutSgn_KNum;	//課税取引金額計算表個別指定　部数									
	long	Sn_PrintOutSgn_Kazei;	// 課税取引金額計算表の出力サイン ('24.03.08)
									// D0=ON:課税取引金額計算表　事業所得用 
									// D1=ON:課税取引金額計算表　農業所得用 
									// D2=ON:課税取引金額計算表　不動産所得用
									// D3=ON:課税売上高計算表
									// D4=ON:課税仕入高計算表
									// D5=ON:事業別売上高
									// D6=ON:
									// D7=ON:

	int		m_KazeiType;			// 0:原則　1:簡易課税　2:簡易課税2割特例
	
// 24/03/21_税務代理書面 add -->
	int		m_Bunbo[KAZ_MAXCOL];
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
	//int SetInputItemSub(int CtIDCh, int CtIDNum, PRNEXSUB_INF *pSubInf, DIAGRAM_DATA	*pdd);
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

	int		m_Ctlsw;			//コントロール状態情報
	int		m_PoutKcode;		// 会社コード出力
public:
	DECLARE_EVENTSINK_MAP()
	void EditOFFPrnKazeiSub(short index);
	void TerminationPrnKazeiSub(short index, short nChar, short length, LPUNKNOWN data);

	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton7();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CIcsdiagctrl m_Diag_Kaz;
	CButton m_Check_Kaz;
	afx_msg void OnBnClickedKazeicode();
	afx_msg LRESULT OnMyFocusControl(WPARAM wParam, LPARAM lParam);
	void CheckButtonPrnKazeiSubDg(short index, LPUNKNOWN data);
public:
	afx_msg void OnBnClickedKazAll();
};
