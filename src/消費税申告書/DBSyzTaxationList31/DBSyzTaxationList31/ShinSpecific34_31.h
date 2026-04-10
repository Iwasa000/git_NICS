//yokono
#pragma once


// CShinSpecific34_31 ダイアログ

class CShinSpecific34_31 : public CSpcBaseDlg
{
	DECLARE_DYNAMIC(CShinSpecific34_31)

public:
	CShinSpecific34_31(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CShinSpecific34_31();

// ダイアログ データ
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_K34_31 };
//#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
private:
	int		m_wPos;
	int		m_cmPos;
	int		m_ScW;	

public:
	//初期設定
	void SpecificInit34() ;
	//カーソルの初期位置をセット
	void SetItemPosition( int IndexNo );
	//スクロールバーの再描画
	void sykscroll( int mpos );
	// 表示
	void SpecificDisp34();
	// 再計算
	void Specific34_Calq();

public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg) ;
	CIcsdiagctrl m_Specific34; //ダイアグラムの変数
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
