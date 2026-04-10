//yokono
#pragma once


// CShinSpecific51_31 ダイアログ

class CShinSpecific51_31 : public CSpcBaseDlg
{
	DECLARE_DYNAMIC(CShinSpecific51_31)

public:
	CShinSpecific51_31(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CShinSpecific51_31();

private:
	int		m_wPos;
	int		m_cmPos;
	int		m_ScW;

// ダイアログ データ
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_K51_31 };
//#endif
public:
	//初期設定
	void SpecificInit51() ;
	//カーソルの位置をセット
	void SetItemPosition() ;
	// 表示
	void SpecificDisp51();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CIcsdiagctrl m_Specific51;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_EVENTSINK_MAP()
	void TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
// 24/04/19特定収入追加 add -->
	afx_msg void OnBnClickedButton1();
	void Specific51_Calq( void );
// 24/04/19特定収入追加 add <--
};
