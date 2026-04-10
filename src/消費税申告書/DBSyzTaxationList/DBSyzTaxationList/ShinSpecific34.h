#pragma once


// CShinSpecific34 ダイアログ

class CShinSpecific34 : public CSpcBaseDlg
{
	DECLARE_DYNAMIC(CShinSpecific34)

public:
	CShinSpecific34(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinSpecific34();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_K34 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

private:
	int		m_wPos;			// 
	int		m_cmPos;		// 
	int		m_ScW;			// 

public:
	// 再描画
	void sykscroll( int mpos );
	// セットポジション
	void SetItemPosition( int IndexNo );
	// 初期設定
	void SpecificInit34();
	// 表示
	void SpecificDisp34();
	// 再計算
	void Specific34_Calq();

public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CIcsdiagctrl m_Specific34;
protected:
	virtual void OnOK();
	virtual void OnCancel();
};
