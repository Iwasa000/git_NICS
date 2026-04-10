//yokono
#pragma once
#include "icsdiagctrl.h"
#include "icsdisp8ctrl1.h"

// CShinSpecific2_31 ダイアログ

class CShinSpecific2_31 : public CSpcBaseDlg
{
	DECLARE_DYNAMIC(CShinSpecific2_31)

public:
	CShinSpecific2_31(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CShinSpecific2_31();

// ダイアログ データ
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_K21_31 };
//#endif
private:
	int	m_TableNo;
	long m_Spage ;
	int m_FcsUpEnd ; //入力可能な範囲の上端
	int m_FcsDownEnd ; //入力可能な範囲の下端
	int m_FcsRightEnd ; //入力可能な範囲の右端
	int m_FcsLeftEnd ; //入力可能な範囲の左端

	int m_curwnd ;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

public:
	//初期設定
	void SpecificInit21() ;
	//カーソルの位置をセット
	void SetItemPosition(BOOL IsInEnd = FALSE) ;
	//入力の可不可
	void IndexControl() ;
	//入力可能な範囲を調べる
	void SetFocusRange();			// 修正No.168722 del
	//virtual void SetFocusRange();	// 修正No.168722 add
	// Tblhdl 上のレコード位置を取得
	int GetTableNo( int index );
	// カラム番号の取得
	int GetColmNo( int nowIdx );
	// 画面表示
	void SpecificDisp21();
	// 計算処理
	void Specific21_Calq();
	// 登録数表示
	void DispRegCount();
	// テーブル登録削除
	int LineDelete( int rwno );
	// テーブル番号挿入
	int LineInsert( int rwno );
	// 該当業の内容列のダイアグラムインデックスを取得
	int findindexEX2( int line );

	DECLARE_MESSAGE_MAP()
public:
	CIcsdiagctrl m_Specific21;
	CIcsdiagctrl m_Specific210;
	CIcsdiagctrl m_Specific211;
	CIcsdisp8ctrl1 m_ICSDisp1;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_EVENTSINK_MAP()
	void TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data);
	void EditOFFIcsdiag8ctrl1(short index);
	void EditONIcsdiag8ctrl1(short index);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnButtonF2();
	afx_msg void OnButtonF3();
	virtual void OnOK();
	virtual void OnCancel();
};
