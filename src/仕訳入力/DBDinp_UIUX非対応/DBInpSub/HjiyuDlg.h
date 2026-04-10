#pragma once


// CHjiyuDlg ダイアログ

class CHjiyuDlg : public ICSDialog
{
	DECLARE_DYNAMIC(CHjiyuDlg)

public:
	CHjiyuDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CHjiyuDlg();

// ダイアログ データ
	enum { IDD = IDD_HJIYU_DLG };

public:
	CRect		m_DlgRect;
	BOOL		m_Dispflg;
	HJIYU_DATA	m_Hdata;

	void	to_parent( CWnd *parent, short nChar );
	int		TerminationHjiyuedit(short nChar);
	int		SetHjiyuData( HJIYU_DATA* data, BOOL bUpdate );
	void	set_focus();
	void	set_fastfocus( BOOL bOK );
	void	set_mode( int mode );
	void	mode_string();

protected:
	int		m_nID, m_mode;
	CWnd*	m_pParent;

	RECT	m_original_rect;
	BOOL	m_MODEFY_OK;
	BOOL	m_THIS_DIALOG_OK;

	HACCEL m_hAccelTable;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	LRESULT	RetroFocus( WPARAM wParam, LPARAM lParam );

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnEnSetfocusEdit1();
public:
	afx_msg void OnEnKillfocusEdit1();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
