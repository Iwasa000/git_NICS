#pragma once


// CShinSpecific53keika ダイアログ

class CShinSpecific53keika : public CSpcBaseDlg
{
	DECLARE_DYNAMIC(CShinSpecific53keika)

public:
	CShinSpecific53keika(CWnd* pParent = NULL);   // 標準コンストラクタ
	CShinSpecific53keika(int zeitype, CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinSpecific53keika();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_K53_KEIKA };

private:
	int		m_wPos;		// 
	int		m_cmPos;	// 
	int		m_ScW;		// 
	int		m_zeiType;	// 税率タイプ

public:
	// 再描画
	void sykscroll( int mpos );
	// セットポジション
//	void SetItemPosition( int IndexNo );//yoshida150316
	// 初期設定
	void SpecificInit53();
	// 表示
	void SpecificDisp53();
	// 再計算
	void Specific53_Calq();

	//-------->yoshida150313
	void IndexControl();
	// セットポジション
	void SetItemPosition();//yoshida150316
	//<---------------------

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
	CIcsdiagctrl m_Specific53;
protected:
	virtual void OnOK();
	virtual void OnCancel();
};
