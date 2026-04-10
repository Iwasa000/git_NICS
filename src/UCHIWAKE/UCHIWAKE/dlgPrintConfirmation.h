// midori 190505 add -->
#pragma once

// CdlgPrintConfirmation ダイアログ

class CdlgPrintConfirmation : public ICSDialog
{
	DECLARE_DYNAMIC(CdlgPrintConfirmation)

public:
	CdlgPrintConfirmation(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CdlgPrintConfirmation();

// ダイアログ データ
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PRINT_CONFIRMATION };
//#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	CButton				m_PrtrdSHoho1;						// 「出力方法」改頁･空行などの編集を詰めて出力
	CButton				m_PrtrdSHoho2;						// 「出力方法」改頁･空行などの編集を保持して出力
// midori 20/10/08u del -->
	//CButton				m_PrtrdSHoho3;						// 「出力方法」編集－ソート設定の第１ソート項目でソート改頁して出力
// midori 20/10/08u del <--
	ICSButton			m_PrtbtnOk;							// 「OK」ボタン
	ICSButton			m_PrtbtnCan;						// 「ｷｬﾝｾﾙ」ボタン
	CStatic				m_Prticon;							// アイコン

private:
	int					m_nRet;								// 戻り値
	CDatabase*			m_pDB;								// データベースハンドル
	CUcFunctionCommon	m_clsFunc;							// 共通関数クラス
	int					m_nFormSeq;							// FormSeq番号
	int					m_SHoho;							// 「出力方法」の設定
	int					m_EditSHoho;						// 「出力方法」の制御設定 TRUE:3択、FALSE:2択

public:
	int				ShowDialog(CDatabase* pDB, int nFormSeq);
	int				InitDisp(void);

	//	改頁処理を行うサインを取得する
	void			CreateDoModal(void);

	virtual BOOL	OnInitDialog();
	virtual BOOL	PreTranslateMessage(MSG* pMsg);

public:
	afx_msg void OnBnClickedPrintOkButton();
	afx_msg void OnBnClickedPrintCancelButton();
};
// midori 190505 add <--
