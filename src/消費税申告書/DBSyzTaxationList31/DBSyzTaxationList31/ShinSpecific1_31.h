#pragma once
//yokono
#include "icsdiagctrl.h"

// CShinSpecific1_31 ダイアログ

class CShinSpecific1_31 : public CSpcBaseDlg
{
	DECLARE_DYNAMIC(CShinSpecific1_31)

public:
	CShinSpecific1_31(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CShinSpecific1_31();

// ダイアログ データ
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_K1_31 };
//#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

private:
	int m_curwnd ; //現在のウィンドウのハンドル

	int					m_FcsUpEnd;		//入力可能なセル(上移動での末端)
	int					m_FcsLeftEnd;	//入力可能なセル(左移動での末端)
	int					m_FcsDownEnd;	//入力可能なセル(下移動での末端)
	int					m_FcsRightEnd;	//入力可能なセル(右移動での末端)

public:
	// 次フォーカスの場所検索（上下左右動作）
	int FocusMoveCheck(int Index , int Move);
	//初期設定
	void SpecificInit1() ;
	//金額の表示
	void SpecificDisp1(void) ;
	//再計算
	void Specific1_Calq(void) ;
	//入力の可、不可の設定
	void IndexControl() ;
	//入力可能な範囲を調べる
	void SetFocusRange() ;		// 修正No.168722 del
	//virtual void SetFocusRange() ;	// 修正No.168722 add
	//カーソルを初期位置にセット
	void SetItemPosition();
	//再描画
	//void sykscroll( int mpos ) ;
	// 入力金額取得
	//void GetDiagData();

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog() ; 
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_EVENTSINK_MAP()

	CIcsdiagctrl m_Specific1; //ダイアグラムの変数
	CIcsdiagctrl m_diag_dmy1;
	CIcsdiagctrl m_diag_dmy2;
	//キーを押した際、カーソルを移動
	void TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data);
	void EditONIcsdiag8ctrl1(short index);
	void EditOFFIcsdiag8ctrl1(short index);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};