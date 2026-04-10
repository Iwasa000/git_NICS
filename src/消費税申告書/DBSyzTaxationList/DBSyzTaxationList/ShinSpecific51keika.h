#pragma once


// CShinSpecific51keika ダイアログ

class CShinSpecific51keika : public CSpcBaseDlg
{
	DECLARE_DYNAMIC(CShinSpecific51keika)

public:
	CShinSpecific51keika(CWnd* pParent = NULL);   // 標準コンストラクタ
	CShinSpecific51keika(int zeitype, CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinSpecific51keika();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_K51_KEIKA };

private:
	int		m_zeiType;		// 税率タイプ

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

private:

	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiag8ctrl1(short index);
	void EditOFFIcsdiag8ctrl1(short index);
	void TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data);

	int		m_wPos;			// 
	int		m_cmPos;		// 
	int		m_ScW;			// 

public:
	// 再描画
	void sykscroll( int mpos );
	// セットポジション
	//void SetItemPosition( int IndexNo );//yoshida150316
	// 初期設定
	void SpecificInit51();
	// 表示
	void SpecificDisp51();
	// 再計算
	void Specific51_Calq();
	// 
	void IndexControl();

	//-------->yoshida150316
	void SetItemPosition();//yoshida150316
	//<---------------------


public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CIcsdiagctrl m_Specific51;
protected:
	virtual void OnOK();
	virtual void OnCancel();
};
