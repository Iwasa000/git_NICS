#pragma once
#include "icsdiagctrl.h"

// CShinSpecific1_31Ex ダイアログ

class CShinSpecific1_31Ex : public CSpcBaseDlg
{
	DECLARE_DYNAMIC(CShinSpecific1_31Ex)

public:
	CShinSpecific1_31Ex(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CShinSpecific1_31Ex();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_K1_31_ex };

private:
	int					m_curwnd;		//現在のウィンドウのハンドル
	int					m_FcsUpEnd;		//入力可能なセル(上移動での末端)
	int					m_FcsLeftEnd;	//入力可能なセル(左移動での末端)
	int					m_FcsDownEnd;	//入力可能なセル(下移動での末端)
	int					m_FcsRightEnd;	//入力可能なセル(右移動での末端)

public:
	// 次フォーカスの場所検索（上下左右動作）
	int FocusMoveCheck(int Index, int Move);
	//初期設定
	void SpecificInit1Ex();
	//金額の表示
	void SpecificDisp1Ex(void);
	//再計算
	void Specific1Ex_Calq(void);
	//入力可能な範囲を調べる
	void SetFocusRange();			// 修正No.168722 del
	//virtual void SetFocusRange();	// 修正No.168722 add
	//入力の可、不可の設定
	void IndexControl();
	//カーソルを初期位置にセット
	void SetItemPosition();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog(); 
	//キーを押した際、カーソルを移動
	void TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data);
	void EditONIcsdiag8ctrl1(short index);
	void EditOFFIcsdiag8ctrl1(short index);
	CIcsdiagctrl m_Specific1Ex; //ダイアグラムの変数
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};