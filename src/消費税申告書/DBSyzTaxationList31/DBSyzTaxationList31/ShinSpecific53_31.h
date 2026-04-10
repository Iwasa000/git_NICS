//yokono
#pragma once


// CShinSpecific53_31 ダイアログ

class CShinSpecific53_31 : public CSpcBaseDlg
{
	DECLARE_DYNAMIC(CShinSpecific53_31)

public:
	CShinSpecific53_31(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CShinSpecific53_31();

// ダイアログ データ
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_K53_31 };
//#endif

private:
	int		m_wPos;
	int		m_cmPos;
	int		m_ScW;

	int		m_curwnd;
	BOOL	m_SySgn;	// 集計サイン

public:
	// 初期設定
	void SpecificInit53();
	//カーソルの位置をセット
	void SetItemPosition() ;

	// 金額表示
	void SpecificDisp53();

	// ダイアグラム初期設定
	void InitDiagAttr();
	// 入力項目色変更
	void ChangeColor ( unsigned short id, short Index, int sign );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CIcsdiagctrl m_Specific53;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiag8ctrl1(short index);
	void EditOFFIcsdiag8ctrl1(short index);
	void TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data);
// 24/04/19特定収入追加 add -->
	afx_msg void OnBnClickedButton1();
	void Specific53_Calq( void );
// 24/04/19特定収入追加 add <--
};
