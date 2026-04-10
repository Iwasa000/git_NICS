#pragma once
#include "icsdiagctrl.h"
#include "icsdisp8ctrl1.h"

// CShinSpecific22_2_31Ex ダイアログ

class CShinSpecific22_2_31Ex : public CSpcBaseDlg
{
	DECLARE_DYNAMIC(CShinSpecific22_2_31Ex)

public:
	CShinSpecific22_2_31Ex(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CShinSpecific22_2_31Ex();

	// ダイアログ データ
	enum { IDD = IDD_DIALOG_K22_2_31_ex };
private:
	long				m_Spage;
	int					m_TableNo;
	int					m_zeiType;		// 税率タイプ
	CArray<int>			m_TblIdxAry;	// テーブル上のインデックス保存

	int					m_FcsUpEnd;		//入力可能なセル(上移動での末端)
	int					m_FcsLeftEnd;	//入力可能なセル(左移動での末端)
	int					m_FcsDownEnd;	//入力可能なセル(下移動での末端)
	int					m_FcsRightEnd;	//入力可能なセル(右移動での末端)


public:
	//初期設定
	void SpecificInit22_2Ex();
	// 表示
	void SpecificDisp22_2Ex();
	// 再計算
	void Specific22_2Ex_Calq();
	//入力の可、不可の設定
	void IndexControl();
	//入力可能な範囲を調べる
	void SetFocusRange();			// 修正No.168722 del
	//virtual void SetFocusRange();	// 修正No.168722 add
	//カーソルの位置をセット
	void SetItemPosition(BOOL IsInEnd = FALSE);
	int GetFocusNo(int index);
	// カラム番号の取得
	int GetColmNo(int index);
	// テーブル番号の取得
	int GetTableNo ( int index );
	// インデックス取得
	int findindex  (int line );
	void CellClear(int index);
	// 該当税率データかどうか
	BOOL IsNowZrtData(TSREC *pTsrec);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CIcsdiagctrl m_Specific22_22Ex;
	CIcsdiagctrl m_Specific22_21Ex;
	CIcsdiagctrl m_Specific22_20Ex;

	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_EVENTSINK_MAP()
	void EditONIcsdiag8ctrl1(short index);
	void EditOFFIcsdiag8ctrl1(short index);
	void TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
