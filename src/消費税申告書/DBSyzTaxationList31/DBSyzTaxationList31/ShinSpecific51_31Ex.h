#pragma once

// CShinSpecific51_31Ex ダイアログ

class CShinSpecific51_31Ex : public CSpcBaseDlg
{
	DECLARE_DYNAMIC(CShinSpecific51_31Ex)

public:
	CShinSpecific51_31Ex(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CShinSpecific51_31Ex();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_K51_31_ex };

public:
	void SpecificInit51Ex();
	void SetItemPosition();
	// 表示
	void SpecificDisp51Ex();

private:
	int		m_wPos;
	int		m_cmPos;
	int		m_ScW;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CIcsdiagctrl m_Specific51Ex;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_EVENTSINK_MAP()
	void TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data);
};
