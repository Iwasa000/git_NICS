#pragma once


// CShinSpecific52_31Ex2 ダイアログ

class CShinSpecific52_31Ex2 : public CSpcBaseDlg
{
	DECLARE_DYNAMIC(CShinSpecific52_31Ex2)

public:
	CShinSpecific52_31Ex2(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CShinSpecific52_31Ex2();

	// ダイアログ データ
	enum { IDD = IDD_DIALOG_K52_31_ex2 };

public:
	void SpecificInit52Ex2();
	void SetItemPosition();
	// 画面表示
	void SpecificDisp52Ex2();
	// 再計算
	void Specific52Ex2_Calq(void);

	// ダイアグラム初期設定
	void InitDiagAttr();
	// 入力項目色変更
	void ChangeColor(unsigned short id, short Index, int sign);

private:
	int		m_wPos;
	int		m_ScW;
	int		m_cmPos;

	int		m_curwnd;
	BOOL	m_SySgn;	// 集計サイン
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	CIcsdiagctrl m_Specific52Ex2;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiag8ctrl1(short index);
	void EditOFFIcsdiag8ctrl1(short index);
	void TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data);
	afx_msg void OnBnClickedButton1();
};
