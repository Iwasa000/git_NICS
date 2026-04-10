#pragma once
#pragma once
#include "icsdiagctrl.h"
#include "icsdisp8ctrl1.h"

// CShinSpecific22_2_31 ダイアログ

class CShinSpecific22_2_31 : public CSpcBaseDlg
{
	DECLARE_DYNAMIC(CShinSpecific22_2_31)

public:
	CShinSpecific22_2_31(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CShinSpecific22_2_31();

	// ダイアログ データ
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_K22_2_31 };
//#endif
private:
	long				m_Spage;
	int					m_TableNo;
	int					m_zeiType;		// 税率タイプ
	CArray<int>			m_TblIdxAry;	// テーブル上のインデックス保存

	int					m_FcsUpEnd;		//入力可能なセル(上移動での末端)
	int					m_FcsLeftEnd;	//入力可能なセル(左移動での末端)
	int					m_FcsDownEnd;	//入力可能なセル(下移動での末端)
	int					m_FcsRightEnd;	//入力可能なセル(右移動での末端)

	int					m_curwnd;		

	CSyzUtil			m_Util;

public:
	//初期設定
	void SpecificInit22_2() ;
	// 表示
	void SpecificDisp22_2();
	// 再計算
	void Specific22_2_Calq();
	// テーブル番号の取得
	int GetTableNo ( int index );
	int GetFocusNo( int index );
	// インデックス取得
	int findindex  (int line );
	// カラム番号の取得
	int GetColmNo ( int index );
	//入力の可不可
	void IndexControl() ;
	//カーソルの位置をセット
	void SetItemPosition ( BOOL IsInEnd = FALSE );
	// ポジションの末端設定
	void SetFocusRange();			// 修正No.168722 del
	//virtual void SetFocusRange();	// 修正No.168722 add
	// 該当税率データかどうか
	BOOL IsNowZrtData( TSREC *pTsrec );

	void CellClear ( int index );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CIcsdiagctrl m_Specific22_22;
	CIcsdiagctrl m_Specific22_21;
	CIcsdiagctrl m_Specific22_20;
	CIcsdisp8ctrl1 m_ICSDisp1;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiag8ctrl1(short index);
	void EditOFFIcsdiag8ctrl1(short index);
	void TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data);
	afx_msg void OnButtonF1();
	afx_msg void OnButtonF2();
};
