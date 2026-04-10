#pragma once


#define CLM_CHECK		0
#define CLM_SOZOKUNAM	1


// CSozokuSel ダイアログ

class CSozokuSel : public ICSDialog
{
	DECLARE_DYNAMIC(CSozokuSel)

public:
	CSozokuSel(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CSozokuSel();

// ダイアログ データ
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SOZOKU_SEL };
//#endif

	// データ初期化
	int		InitInfo( NOHFUSHO_INFO *nohfuinfo, NOHFUSHO_DATA *nohfudata, CArray<int> &SozokuNoAry, int nohfuinfomax );
	// レポート作成
	int		CreateReport();
	// データセット
	int		SetReportData();
	// 全選択/全解除
	void	ChangeSelect();

	CRect		m_rctRep;		// report rect
	int			m_sozokumax;	// 相続人最大数
	int			m_chgsel_sgn;	// 0:全選択 / 1:全解除

	NOHFUSHO_INFO	NOHFU_INFO[40];	// 相続人情報が入った構造体
	NOHFUSHO_DATA	NOHFU_DATA[40];	// 相続人情報が入った構造体
	CArray<int>		m_SozokuNoAry;	// 相続人番号（syz_sozoku.Sn_NO）

	NOHFUSHO_INFO	SELECT_INFO[40];// 選択された相続人情報
	NOHFUSHO_DATA	SELECT_DATA[40];// 選択された相続人情報
	CArray<int>		m_SelectNoAry;	// 選択された相続人番号（syz_sozoku.Sn_NO）

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	ICSReportControl m_report;
	afx_msg void OnBnClickedChgselBtn();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
