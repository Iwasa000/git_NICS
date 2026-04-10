// midori 156950 add -->
#pragma once

// CdlgPrintConfirmation4 ダイアログ

class CdlgPrintConfirmation4 : public ICSDialog
{
	DECLARE_DYNAMIC(CdlgPrintConfirmation4)

public:
	CdlgPrintConfirmation4(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CdlgPrintConfirmation4();

// ダイアログ データ
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PRINT_CONFIRMATION4 };
//#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

public:
	CButton				m_Prt4Radio1;						// １様式目：改頁･空行などの編集を詰めて出力
	CButton				m_Prt4Radio2;						// １様式目：編集－ソート設定の第１ソート項目でソート改頁して出力
	CButton				m_Prt4Radio3;						// ２様式目：改頁･空行などの編集を詰めて出力
	CButton				m_Prt4Radio4;						// ２様式目：編集－ソート設定の第１ソート項目でソート改頁して出力
	ICSButton			m_Prt4btnOk;						// 「OK」ボタン
	ICSButton			m_Prt4btnCan;						// 「ｷｬﾝｾﾙ」ボタン
	CStatic				m_Prt4icon;							// アイコン
	CStatic				m_Prt4Static3;						// ガイド（１つ目の様式名）
	CStatic				m_Prt4Static4;						// ガイド２（２つ目の様式名）

private:
	int					m_nRet;								// 戻り値
	CDatabase*			m_pDB;								// データベースハンドル
	CUcFunctionCommon	m_clsFunc;							// 共通関数クラス
	int					m_nFormSeq[2];						// FormSeq番号				[0]:１つ目の様式		[1]:２つ目の様式
	int					m_SHoho[2];							// 「出力方法」の設定		[0]:１つ目の様式		[1]:２つ目の様式
	CWnd*				m_pOwner;

public:
	int				ShowDialog(CDatabase* pDB, int nFormSeq);
	int				InitDisp(void);

	virtual BOOL	OnInitDialog();
	virtual BOOL	PreTranslateMessage(MSG* pMsg);

public:
	afx_msg void OnBnClickedPrintOkButton();
	afx_msg void OnBnClickedPrintCancelButton();
};
// midori 156950 add <--
