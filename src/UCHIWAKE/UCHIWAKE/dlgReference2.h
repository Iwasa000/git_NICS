#pragma once


// CdlgReference2 ダイアログ

static char strPrintAllHeader2[][64] = {
	_T("選択"),
	_T("番号"),
	_T("名称"),
// midori 152137 add -->
	_T("カナ"),
// midori 152137 add <--
};

class CdlgReference2 : public ICSDialog
{
	DECLARE_DYNAMIC(CdlgReference2)

public:
	CdlgReference2(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CdlgReference2();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_REFERENCE2 };

	CComboBox			m_Ref2Combo1;	// 様式選択コンボ
	CButton				m_Ref2Check5;	// 科目指定を行うチェック
	ICSReportControl	m_Ref2Report;	// 科目選択リスト
	CButton				m_Ref2Check1;	// 科目でソートを行うチェック
// midori 151397 cor -->
//	CButton				m_Ref2Check2;	// 改頁を行うチェック
// ---------------------
	CButton				m_Ref2Check2;	// 科目毎に改頁を行うチェック
// midori 151397 cor <--
// midori 190505 add -->
	CButton				m_Ref2Check6;	// 科目行を自動挿入する
// midori 190505 add <--
	CButton				m_Ref2Check3;	// 「小計」を自動挿入するチェック
	CButton				m_Ref2Check4;	// 一括金額を行うチェック
	ICSButton			m_Ref2Button;	// 科目 全選択/全解除ボタン
	ICSButton			m_btnOK;		// ＯＫボタン
	ICSButton			m_btnCancel;	// キャンセルボタン

private:
	CDatabase*			m_pDB;				// データベースハンドル
	short				m_nFormSeq;			// 様式シーケンス番号
	int					m_nRet;				// 戻値
	BOOL				m_isChangePageNum;	// 変更があった？　一括印刷ダイアログ用
	int					m_nFormSeqArray[UC_ID_FORMNO_MAX];
	int					m_KamokuCount;		// 様式ごとの科目件数
											// (科目未入力データを含む)
	BOOL				m_KamokuMinyu;		// TRUE:科目未入力データあり
	COLORREF			m_ViewColor;		// レポートの背景色
	CUcFunctionCommon	m_clsFunc;			// 共通関数クラス
	int					m_SaveCombo;		// 前回の様式コンボ選択値
// midori 151404 add -->
	int					m_isYousiki[UC_ID_FORMNO_MAX];	// 様式毎の変更サイン TRUE:再表示が必要
// midori 151404 add <--
// midori 152137 add -->
	int					m_KamokuSort;		// 参照ダイアログ::表示順（科目）		0:番号順	1:50音順
// midori 152137 add <--
private:
	void	InitReportHeader();
	void	SetGrid();
	void	InitYoushikiCombo( CDatabase* pDB, short *nFormSeq );
	void	ChangeYoushikiCombo( CDatabase* pDB, short nFormSeq );
	int		SetYoushikiData( CDatabase* pDB, short nFormSeq );
	void	SetRef2Check();
	int		CheckYoushikiData( CDatabase* pDB, short nFormSeq );
	void	SetListData( CDatabase* pDB, short nFormSeq );
	void	KamokuSelControl( BOOL pSw );
	void	UpdateStateFromKey(int key);
	int		CheckYoushikiData();

public:
// midori 151404 cor -->
//	int		ShowDialog( BOOL& isChange, CDatabase* pDB, short nFormOrder );
// ---------------------
	int		ShowDialog( BOOL& isChange, BOOL isYousiki[], CDatabase* pDB, short nFormOrder );
// midori 151404 cor <--

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

public:

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnCbnSelchangeRef2Combo1();
	afx_msg void OnBnClickedRef2Check5();
	afx_msg void OnBnClickedRef2Check1();
	afx_msg void OnBnClickedRef2Check2();
// midori 190505 add -->
	afx_msg void OnBnClickedRef2Check6();
// midori 190505 add <--
	afx_msg void OnBnClickedOk();

protected:
	virtual void OnOK();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedRef2Button();
};
