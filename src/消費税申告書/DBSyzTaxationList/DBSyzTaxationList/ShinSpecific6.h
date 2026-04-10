#pragma once


// CShinSpecific6 ダイアログ

class CShinSpecific6 : public CSpcBaseDlg
{
	DECLARE_DYNAMIC(CShinSpecific6)

public:
	CShinSpecific6(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinSpecific6();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_K6 };

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
	void SpecificInit6();
	// 表示
	void SpecificDisp6();
	// 再計算
	void Specific6_Calq();
	// 
	void IndexControl();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	DECLARE_EVENTSINK_MAP()
	void EditOFFIcsdiag8ctrl1(short index);
	void TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data);
	CIcsdiagctrl m_Specific6;
protected:
	virtual void OnOK();
	virtual void OnCancel();
};
