// HusenDlg.h : ヘッダー ファイル
//

//#include "ICSDialog.h"
//{{AFX_INCLUDES()
#include "icsdbedt.h"
//}}AFX_INCLUDES

/////////////////////////////////////////////////////////////////////////////
// CHusenDlg ダイアログ
#ifdef	_ICSPEN_	// 01.20 /01
//#include "ICSPen.h"
#endif

#include "HusenCombo.h"


class CHusenDlg : public CDialog
{
// コンストラクション
public:
	CHusenDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CHusenDlg)
	enum { IDD = IDD_HUSEN_DLG };
	CICSDBEDT	m_DBedt;
	//}}AFX_DATA
	CHusenCombo	m_Hcbo;

	BOOL Create();
	void get_originalsize( double& w, double& h );
	void set_fastfocus( BOOL modefy_sg, int pn );
	void HusenInp_Init( CDBipDataRec* crec, CDBipDataRec* srec, struct _DATA_LINE* dtline );

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CHusenDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	int		m_nID;
	CWnd*	m_pParent;
	int		m_Mode;		// 0 = 部門, 1 = 工事
	CDBipDataRec* m_pCREC, *m_pSREC;
	struct _DATA_LINE* m_Dtline;

	RECT	m_original_rect;
	BOOL	m_MODEFY_OK;
	BOOL	m_THIS_DIALOG_OK;

	// Now ポジション
	int m_NOW_PN;
	//直前のポジション
	int m_PREV_PN;

protected:
	struct _InpCtl *getINP_CTL( int pn );
	void set_nowfocus();
	void set_focus( int pn );
	int set_nowpn( int pn );
	int get_nowpn( void );
	int get_prevpn();

	void TerminationHusenComment(long nChar, long inplen, long kst);

	long TermWork( int pn, long nChar );
	void to_parent( long nChar );

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CHusenDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeHsCombo();
	afx_msg void OnSetfocusHsCombo();
	afx_msg void OnKillfocusHsCombo();
	afx_msg void OnSetFocusIcsdbedtctrl1();
	afx_msg void OnTerminationIcsdbedtctrl1(long nChar, long inplen, long kst);
	afx_msg void OnKillFocusIcsdbedtctrl1(long inplen);
	afx_msg void OnImeEndCompositionIcsdbedtctrl1(short nChar, LPCTSTR string, LPCTSTR ystring);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	LRESULT RetroFocus( WPARAM wParam, LPARAM lParam );

private:
	HACCEL m_hAccelTable;

#ifdef	_ICSPEN_	// 01.20 /01
private:
//	ICSPen*	icspen;
	BOOL	flgPen;
	CRect	rectPen;
public:
//	short	SetPen( BOOL flg, ICSPen* ppen );
	void	SetPenRect( CRect& rect );
	void	set_nextpos();
#endif

};
