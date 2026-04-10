#pragma once


// CShinSpecific52_31Ex ダイアログ

class CShinSpecific52_31Ex : public CSpcBaseDlg
{
	DECLARE_DYNAMIC(CShinSpecific52_31Ex)

public:
	CShinSpecific52_31Ex(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CShinSpecific52_31Ex();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_K52_31_ex };

public:
	void SpecificInit52Ex();
	void SetItemPosition();	
	// 画面表示
	void SpecificDisp52Ex();

	// ダイアグラム初期設定
	void InitDiagAttr();
	// 入力項目色変更
	void ChangeColor ( unsigned short id, short Index, int sign );

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
	CIcsdiagctrl m_Specific52Ex;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiag8ctrl1(short index);
	void EditOFFIcsdiag8ctrl1(short index);
	void TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data);
};
