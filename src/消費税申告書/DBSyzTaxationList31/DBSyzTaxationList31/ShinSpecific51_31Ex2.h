#pragma once


// CShinSpecific51_31Ex2 ダイアログ


class CShinSpecific51_31Ex2 : public CSpcBaseDlg
{
	DECLARE_DYNAMIC(CShinSpecific51_31Ex2)

public:
	CShinSpecific51_31Ex2(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CShinSpecific51_31Ex2();

	// ダイアログ データ
	enum { IDD = IDD_DIALOG_K51_31_ex2 };

public:
	void SpecificInit51Ex2();
	void SetItemPosition();
	// 表示
	void SpecificDisp51Ex2();
	// 再計算
	void Specific51Ex2_Calq(void);

private:
	int		m_wPos;
	int		m_cmPos;
	int		m_ScW;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CIcsdiagctrl m_Specific51Ex2;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_EVENTSINK_MAP()
	void TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data);
	afx_msg void OnBnClickedButton1();
};
