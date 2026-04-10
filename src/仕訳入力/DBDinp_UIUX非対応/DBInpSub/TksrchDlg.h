// TksrchDlg.h : ヘッダー ファイル
//
#ifndef INCLUDE_TKSRCHDLG_H
#define INCLUDE_TKSRCHDLG_H

#include "icsdbedt.h"


/////////////////////////////////////////////////////////////////////////////
// CTksrchDlg ダイアログ


class CTksrchDlg : public ICSDialog
{
// コンストラクション
public:
	CTksrchDlg(CDBZmSub* pZm, CPoint& pt, int psg, CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CTksrchDlg)
	enum { IDD = IDD_TKSRCH_DLG };
	CListBox	m_List;
	CICSDBEDT	m_DBedt1;
	//}}AFX_DATA

	int		m_TKcod;

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CTksrchDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	CDBZmSub* m_pZm;

	LONG	m_Xps;
	LONG	m_Yps;
	int		m_Psg;	// 開始位置サイン

	BOOL m_bInit;

	static char m_Kana[10];

	void KanaTerminationSub( CString& kana );
	virtual void OnOK();

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CTksrchDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTerminationIcsdbedtkana(long nChar, long inplen, long kst);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	LRESULT OnUser_1( WPARAM wParam, LPARAM lParam );

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLbnDblclkList1();
};


#endif // INCLUDE_TKSRCHDLG_H
