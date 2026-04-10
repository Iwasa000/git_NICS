#pragma once


// CShinSpecific52_32_31 ダイアログ

class CShinSpecific52_32_31 : public CSpcBaseDlg
{
	DECLARE_DYNAMIC(CShinSpecific52_32_31)

public:
	//CShinSpecific52_32_31(CWnd* pParent = nullptr);   // 標準コンストラクター
	CShinSpecific52_32_31(CWnd* pParent = nullptr, TSREC52* pRec = NULL);   // 標準コンストラクター
	virtual ~CShinSpecific52_32_31();

	// ダイアログ データ

	enum { IDD = IDD_DIALOG_K52_32_31 };

public:
	void SpecificInit52_32_31();
	void SetItemPosition();
	// 表示
	void SpecificDisp52_32_31();
	// 次フォーカスの場所検索（上下左右動作）
	int FocusMoveCheck(int Index, int Move);
	//入力可能な範囲を調べる
	void SetFocusRange();
	//入力の可、不可の設定
	void IndexControl();
	// 再計算
	void Specific52_32_31_Calq();
	// ダイアグラム初期設定
	void InitDiagAttr();
	// 入力項目色変更
	void ChangeColor(unsigned short id, short Index, int sign);


	CSyzUtil		m_Util;						// ユーティリティクラス
	FCSUPDOWN_INFO*  fcs;
	FCSUPDOWN_INFO*  fcslr;
	int fcs_cnt;

private:
	CWnd	*m_pParent;			// 親ウィンドウ
	int		m_wPos;
	int		m_cmPos;
	int		m_ScW;
	int		m_curwnd;	//現在のウィンドウのハンドル
	int		m_FcsUpEnd;		//入力可能なセル(上移動での末端)
	int		m_FcsLeftEnd;	//入力可能なセル(左移動での末端)
	int		m_FcsDownEnd;	//入力可能なセル(下移動での末端)
	int		m_FcsRightEnd;	//入力可能なセル(右移動での末端)

	// 24/02/15_特定収入 add -->
	TSREC52*	m_pRec;
	// 24/02/15_特定収入 add <--

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CIcsdiagctrl m_Specific52_32_31;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	DECLARE_EVENTSINK_MAP()
	void EditOFFIcsdiag8ctrl1(short index);
	void EditONIcsdiag8ctrl1(short index);
	void TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data);
	void CheckButtonIcsdiag8ctrl1(short index, LPUNKNOWN data);
};
