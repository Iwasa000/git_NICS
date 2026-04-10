#pragma once

#include "icsdisp8ctrl1.h"

// CShinSpecific22 ダイアログ

class CShinSpecific22 : public CSpcBaseDlg
{
	DECLARE_DYNAMIC(CShinSpecific22)

public:
	CShinSpecific22(CWnd* pParent = NULL);   // 標準コンストラクタ
	CShinSpecific22(int zeitype, CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CShinSpecific22();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_K22 };

private:
	int				m_zeiType;		// 税率タイプ
	CArray<int>		m_TblIdxAry;	// テーブル上のインデックス保存

	//------->yoshida150316
	CSyzUtil			m_Util;

	int					m_FcsUpEnd;		//入力可能なセル(上移動での末端)
	int					m_FcsLeftEnd;	//入力可能なセル(左移動での末端)
	int					m_FcsDownEnd;	//入力可能なセル(下移動での末端)
	int					m_FcsRightEnd;	//入力可能なセル(右移動での末端)

	int					m_curwnd;		
	//<--------------------

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

private:
	int		m_TableNo;	// 
	long	m_Spage;	// 

public:
	// 再描画
	void sykscroll( int mpos );
	// セットポジション
//	void SetItemPosition( int IndexNo );//yoshida150316
	// 初期設定
	void SpecificInit22();
	// 表示
	void SpecificDisp22();
	// 再計算
	void Specific22_Calq();
	// テーブル番号の取得
	int GetTableNo( int index );
	// カラム番号の取得
	int GetColmNo( int index );
	// テーブル番号の挿入
	int LineInsert( int rwno );
	// テーブル番号の消去
	int LineDelete( int rwno );
	// 該当税率データ？
	BOOL IsNowZrtData( TSREC *pTsrec );
	
	//-------->yoshida150316
	void IndexControl();
	// セットポジション
	void SetItemPosition( BOOL IsInEnd=FALSE );
	// 次フォーカスの場所検索（上下左右動作）
	int FocusMoveCheck(int Index , int Move);
	// ポジションの末端設定
	void SetFocusRange();
	//<---------------------

//2015.03.05 INSERT START
	void CellClear( int index );
//2015.03.05 INSERT END

	//----->yoshida150427
	//フォーカス移動用に作成
	int GetFocusNo( int index );
	//<------------------


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
	CIcsdiagctrl m_Specific22;
	CIcsdiagctrl m_Specific220;
	CIcsdiagctrl m_Specific221;
	CIcsdisp8ctrl1 m_ICSDisp1;
protected:
	virtual void OnOK();
	virtual void OnCancel();
};
