#pragma once


// CSubDataDlg ダイアログ



class CSubDataDlg : public ICSDialog
{
	DECLARE_DYNAMIC(CSubDataDlg)

public:
	CSubDataDlg(CWnd* pParent = NULL, int type = 0);   // 標準コンストラクタ
	virtual ~CSubDataDlg();

// ダイアログ データ
	enum { IDD = IDD_SUBDATA_DLG };

	struct _InpCtl *getINP_CTL( int pn );
	void	set_fastfocus( int pn );
	void	set_nowfocus();
	void	set_focus( int pn );

	int		set_nowpn( int pn );
	int		get_nowpn( void );
	int		get_prevpn();

	void	set_cdata( BOOL bMdfyOK, CDBINPDataRec* data );
	void	HusenDlgDisp();
	void	to_parent( long nChar );

	long	TgnoInp( long nChar, long inplen, long kst );

	long	CheckDataIcsinputTgDate( long nChar );
	long	TgdateInp( long nChar, long inplen, long kst );

	void	set_billsgn( BOOL bBill ) { m_bBill = bBill; };
	BOOL	get_billsgn() {  return m_bBill; };

public:
	CRect	m_DlgRect;
	BOOL	m_Dispflg;

protected:
	CWnd*	m_pParent;
	HACCEL	m_hAccelTable;

	// Now ポジション
	int NOW_PN;
	//直前のポジション
	int PREV_PN;

	BOOL m_MODEFY_OK;
	BOOL m_THIS_DIALOG_OK;

	// 付箋入力ダイアログ
	CHusenInp*	m_pHusenInp;
	// データレコード
	CDBINPDataRec	m_Data;
	BOOL			m_bBill;

	int		m_Type;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	LRESULT RetroFocus( WPARAM wParam, LPARAM lParam );
	DECLARE_EVENTSINK_MAP()

	LRESULT OnHsInpInEnd( WPARAM wParam, LPARAM lParam );

	void SetFocusIcsdbedtTgno();
	void SetFocusIcsdbedtTgdate();

	void KillFocusIcsdbedtTgno(long inplen);
	void KillFocusIcsdbedtTgdate(long inplen);

	void TerminationIcsdbedtTgno(long nChar, long inplen, long kst);
	void TerminationIcsdbedtTgdate(long nChar, long inplen, long kst);
protected:
	virtual void PostNcDestroy();
};
