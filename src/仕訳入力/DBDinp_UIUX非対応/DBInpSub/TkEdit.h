// TkEdit.h : ヘッダー ファイル
//
#ifndef __TKEDIT_H__
#define __TKEDIT_H__


/////////////////////////////////////////////////////////////////////////////
// CTkEdit ウィンドウ

//#define _DIC_MAX	TKYO_LEN
#define TKBTN_MAX	5


struct _DICTKY {
	int length;
	int Def_nStart;
	int nStart, nEnd;
	int startSel, endSel;
	char tky[82];
};


class CTkEdit : public CEdit
{
// コンストラクション
public:
	CTkEdit();

// アトリビュート
public:
	int m_DIC_MAX;
	struct _DICTKY m_DICTKY;
		
	// オペレーション
public:
	BOOL OnInitial( int pos, char *tky, int tkyLen, int sel1, int sel2 );
	BOOL InitCtrl( CWnd* parent, UINT* btnID, UINT dspID );

	void LeftFunc();
	void RightFunc();
	void AddFunc();
	void SubFunc();
	void HomeFunc();

	void UpdateSelect();
	BOOL IsSetFocus() { return m_bFocus; };

	void get_dictky( char *buf, int bufsiz );

protected:
	BOOL isSHIFT();
	void indicator();

	void GetSel(int& nStartChar, int& nEndChar);
	void SetSel( int nStartChar, int nEndChar, BOOL bNoScroll = FALSE );

private:
	CWnd*	m_pParent;
	UINT	m_BtnID[TKBTN_MAX];
	UINT	m_DspID;

	BOOL	m_bFocus;
	BOOL	bIsWindowsXPorLater;

	char	m_AnsBuf[64];

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CTkEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CTkEdit();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CTkEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////


#endif //__TKEDIT_H__
