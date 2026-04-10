#pragma once

#include "icsdiagctrl.h"
#include "icsdisp8ctrl1.h"

// CShinSpecific21 ダイアログ

class CShinSpecific21 : public CSpcBaseDlg
{
	DECLARE_DYNAMIC(CShinSpecific21)

public:
	CShinSpecific21(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinSpecific21();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_K21 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

private:
	int		m_TableNo;	// 
	long	m_Spage;	// 

public:
	// 再描画
	void sykscroll( int mpos );
	// セットポジション
	void SetItemPosition( int IndexNo );
	// 初期設定
	void SpecificInit21();
	// 表示
	void SpecificDisp21();
	// 再計算
	void Specific21_Calq();
	// テーブル番号の取得
	int GetTableNo( int index );
	// カラム番号の取得
	int GetColmNo( int index );
	// テーブル番号の消去
	int LineDelete( int rwno );
	// テーブル番号の挿入
	int LineInsert( int rwno );


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
