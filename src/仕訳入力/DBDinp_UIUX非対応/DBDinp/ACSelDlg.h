// ACSelDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CACSelDlg ダイアログ

#include "ICSDialog.h"
//{{AFX_INCLUDES()
#include "icsdbedt.h"
//}}AFX_INCLUDES

#define AS_BMON_PN		0
#define	AS_KOJI_PN		1
#define	AS_KAMOKU_PN	2
#define	AS_DATE_PN		3

#define AS_SEL_PN		4
#define AS_OK_PN		5
#define AS_CANCEL_PN	6



class CACSelDlg : public ICSDialog
{
// コンストラクション
public:
	CACSelDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ
	virtual ~CACSelDlg();

// ダイアログ データ
	//{{AFX_DATA(CACSelDlg)
	enum { IDD = IDD_DLG_ACSEL };
	CICSDBEDT	m_DBkmk;
	CICSDBEDT	m_DBkoji;
	CICSDBEDT	m_DBdate;
	CICSDBEDT	m_DBbmn;
	//}}AFX_DATA

	CDipZmSub*	m_pZm;
	CDBSyohi*	m_pSy;
	CString		m_code, m_koji;
	int			m_brn, m_bmn;
	int			m_date, m_ofset, m_ksign;
	char		m_datestr[20];

	CZmselDB*	m_pZsel;

	short KMCD_Disp, BRNNAM_Disp;
	short BRNTKY_sel;

	int		NowPN;
	BOOL	m_bDIALOG_OK;
	BOOL	m_bSEL_DISP;
	BOOL	m_bBM_MAST, m_bKJ_MAST;

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CACSelDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	static void CALLBACK callback(UINT n,void* p,CWnd* pwnd );
	long CheckDataKamoku( LPCTSTR string );
	void TerminationKamoku( long nChar, long inplen, long kst, ZmselDBData* zd = NULL );

	int CheckDataDate( long nChar );
	void TerminationDate( long nChar, long inplen, long kst );

	void TerminationBumon( long nChar, long inplen, long kst, ZmselDBData* zd = NULL );
	void TerminationKoji( long nChar, long inplen, long kst, ZmselDBData* zd = NULL );

	void ZmSelSet( ZmselDBData* zd );
	void ZmSelBackFocus();
	
	void set_focus( UINT id );
	void set_nowpn( int pn );
	int	get_nowpn();
	BOOL PreTransKeyDown(int id, UINT key);

	void SelDialogON( int selmode = -1 );

	LRESULT OnRetroFocus( WPARAM wParam, LPARAM lParam );

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CACSelDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSetFocusIcsdbedtkmk();
	afx_msg void OnImeEndCompositionIcsdbedtkmk(short nChar, LPCTSTR string, LPCTSTR ystring);
	afx_msg void OnKanaTerminationIcsdbedtkmk(LPCTSTR Kana);
	afx_msg void OnTerminationIcsdbedtkmk(long nChar, long inplen, long kst);
	afx_msg void OnSetFocusIcsdbedtdate();
	afx_msg void OnKillFocusIcsdbedtdate(long inplen);
	afx_msg void OnTerminationIcsdbedtdate(long nChar, long inplen, long kst);
	afx_msg void OnKillFocusIcsdbedtkmk(long inplen);
	afx_msg void OnButton1();
	afx_msg void OnSetFocusIcsdbedtBmn();
	afx_msg void OnTerminationIcsdbedtBmn(long nChar, long inplen, long kst);
	afx_msg void OnKillFocusIcsdbedtBmn(long inplen);
	afx_msg void OnSetFocusIcsdbedtKoji();
	afx_msg void OnKillFocusIcsdbedtKoji(long inplen);
	afx_msg void OnTerminationIcsdbedtKoji(long nChar, long inplen, long kst);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
