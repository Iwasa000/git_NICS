#pragma once

#include "icsdiagctrl.h"
#include "icsdisp8ctrl1.h"

// CShinSpecific21EX2 ダイアログ

class CShinSpecific21EX2 : public CSpcBaseDlg
{
	DECLARE_DYNAMIC(CShinSpecific21EX2)

public:
	CShinSpecific21EX2(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinSpecific21EX2();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_K21_EX2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

private:
	int		m_TableNo;	// 
	long	m_Spage;	// 

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
//	void SetItemPosition( int IndexNo );
	// 初期設定
	void SpecificInit21();
	// 表示
	void SpecificDisp21();
	// 再計算
	void Specific21_Calq();
	// Tblhdl 上のレコード位置を取得
	int GetTableNo( int index );
	// カラム番号の取得
	int GetColmNo( int nowIdx );
	// テーブル番号の消去
	int LineDelete( int rwno );
	// テーブル番号の挿入
	int LineInsert( int rwno );
	// 登録数表示('15.05.05)
	void DispRegCount();


	void IndexControl();
	void SetItemPosition( BOOL IsInEnd=FALSE );//yoshida150311

	// 次フォーカスの場所検索（上下左右動作）
	int FocusMoveCheck(int Index , int Move);

	// ポジションの末端設定
	void SetFocusRange();



	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiag8ctrl1(short index);
	void EditOFFIcsdiag8ctrl1(short index);
	void TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnButtonF2();
	afx_msg void OnButtonF3();
	CIcsdiagctrl m_Specific21;
	CIcsdiagctrl m_Specific210;
	CIcsdiagctrl m_Specific211;
	CIcsdisp8ctrl1 m_ICSDisp1;
protected:
	virtual void OnOK();
	virtual void OnCancel();
};
