#pragma once

#include "icsdiagctrl.h"

// CShinSpecific1EXkeika ダイアログ

class CShinSpecific1EXkeika : public CSpcBaseDlg
{
	DECLARE_DYNAMIC(CShinSpecific1EXkeika)

public:
	CShinSpecific1EXkeika(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinSpecific1EXkeika();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_K1_EX_KEIKA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート


private:

	CSyzUtil			m_Util;

	int					m_FcsUpEnd;		//入力可能なセル(上移動での末端)
	int					m_FcsLeftEnd;	//入力可能なセル(左移動での末端)
	int					m_FcsDownEnd;	//入力可能なセル(下移動での末端)
	int					m_FcsRightEnd;	//入力可能なセル(右移動での末端)

	int					m_curwnd;		




public:
	// 再描画
	void sykscroll( int mpos );
	// セットポジション
//	void SetItemPosition( int IndexNo );//yoshida150310
	// 初期設定
	void SpecificInit1();
	// 表示
	void SpecificDisp1();
	// メインウィンドウにメッセージ送信
	void PostTabInitMes();
	// 再計算
	void Specific1_Calq();
	// 
	void IndexControl();

	// 次フォーカスの場所検索（上下左右動作）
	int FocusMoveCheck(int Index , int Move);

	// ポジションの末端設定
	void SetFocusRange();

	// 初期ポジションセット 
	virtual void SetItemPosition( BOOL IsInEnd=FALSE );

	// 入力金額取得
	void GetDiagData();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiag8ctrl1(short index);//yoshida150310
	void EditOFFIcsdiag8ctrl1(short index);
	void TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data);
	CIcsdiagctrl m_Specific1;
	CIcsdiagctrl m_diag_dmy1;
	CIcsdiagctrl m_diag_dmy2;
protected:
	virtual void OnOK();
	virtual void OnCancel();
};
