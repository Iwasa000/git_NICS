// SELDLG.h : ヘッダー ファイル
//
#ifndef INCLUDE_SELDLG_H
#define INCLUDE_SELDLG_H

//#include "MylistBox.h"

#ifndef INCLUDE_ICSLISTBOX
	#include "icslistbox.h"
	#define	INCLUDE_ICSLISTBOX	1
#endif

/////////////////////////////////////////////////////////////////////////////
// SELDLG ダイアログ

#include "afxpriv.h"

enum SEL_COLUMN {
	SEL_SYOHI = 0,
	SEL_MENZEI,
	SEL_KUBUN,
};

class SELDLG : public ICSDialog
{
// コンストラクション
public:
	SELDLG(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(SELDLG)
	enum { IDD = IDD_SELDLG1 };
	ICSListBox	m_sylist;
//	CMyListBox	m_sylist;

	//}}AFX_DATA

	int		Lmode;

	CDialogTemplate	CDtmp;
	int		DlgFontSize;
	CFont	m_Fnt;
	CWnd*	m_pParent;
	
	int		sx_pos;
	int		sy_pos;

	int		nx_width;
	int		ny_width;
	
	int		Selno;

	UINT	SelTerm;
	CString	SelItem;

	CString	SelMsg[16];
	void	SetDlgData();
	void	DeleteSelDlg();

//	void	OnOK();
//	void	OnCancel();

	// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(SELDLG)
	public:
	virtual void OnSetFont(CFont* pFont);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(SELDLG)
	afx_msg void OnDblclkSellist();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnKillfocusSellist();
	afx_msg int OnVKeyToItem(UINT nKey, CListBox* pListBox, UINT nIndex);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	// インボイス対応
private:
	int		m_columnFlg;
	// メッセージ取得
	UINT GetMsg();

public:
	// カラムセット
	void	SetColumn( int clm );
};



#endif // INCLUDE_SELDLG_H