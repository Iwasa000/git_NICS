#pragma once
#include "icsdiagctrl.h"
#include "icsdisp8ctrl1.h"
// CShinSpecific2_31Ex ダイアログ

class CShinSpecific2_31Ex : public CSpcBaseDlg
{
	DECLARE_DYNAMIC(CShinSpecific2_31Ex)

public:
	CShinSpecific2_31Ex(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CShinSpecific2_31Ex();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_K21_31_ex };

private:
	int	m_TableNo;
	int m_FcsUpEnd; //入力可能な範囲の上端
	int m_FcsDownEnd; //入力可能な範囲の下端
	int m_FcsRightEnd; //入力可能な範囲の右端
	int m_FcsLeftEnd; //入力可能な範囲の左端

	int m_curwnd ;

public:
	//初期設定
	void SpecificInit21Ex();
	void SetItemPosition(BOOL IsInEnd =FALSE);
	//入力の可不可
	void IndexControl();
	//入力可能な範囲を調べる
	void SetFocusRange();			// 修正No.168722 del
	//virtual void SetFocusRange();	// 修正No.168722 add
	void SpecificDisp21Ex();
	int  GetTableNo(int nowindex);
	int  GetColmNo(int nowIdx);
	int  LineDelete(int rwno);
	int  LineInsert(int rwno);
	// 該当業の内容列のダイアグラムインデックスを取得
	int findindexEX2(int line);
	// 計算処理
	void Specific21Ex_Calq();
	// 登録数表示
	void DispRegCount();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CIcsdiagctrl m_Specific21Ex;
	CIcsdiagctrl m_Specific210Ex;
	CIcsdiagctrl m_Specific211Ex;

	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	DECLARE_EVENTSINK_MAP()
	void TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data);
	void EditOFFIcsdiag8ctrl1(short index);
	void EditONIcsdiag8ctrl1(short index);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
