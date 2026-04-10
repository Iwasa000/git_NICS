// EdaRegDlg.h : ヘッダー ファイル
//


//{{AFX_INCLUDES()
#include "icsdbedt.h"
#include "afxwin.h"
//}}AFX_INCLUDES


/////////////////////////////////////////////////////////////////////////////
// CEdaRegDlg ダイアログ


typedef struct _EDAREG_BUF 
{
int		er_tkcod;
char	er_zan[6];

char	er_name[32],
		er_kana[10];

BOOL	er_inpsg[2];

} EDAREG_BUF;


#define EDR_TKCOD		0
#define EDR_ZAN			1


#define WM_ERDRETROFOCUS		(WM_APP+1)


class CEdaRegDlg : public ICSDialog
{
// コンストラクション
public:
	CEdaRegDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CEdaRegDlg)
	enum { IDD = IDD_DLG_EDAREG };
	CListBox	m_TkList;
	CButton	m_OK;
	CButton	m_Cancel;
	CButton m_ChkReg;
	CICSDBEDT	m_DBedt1;
	CICSDBEDT	m_DBedt2;
	CICSDBEDT	m_DBedt3;
	CICSDBEDT	m_DBkana;
	CICSDBEDT	m_DBname;
	CICSDBEDT	m_DBKezan;
	//}}AFX_DATA

	void InitEdaRegDlg( EDA_RECORD* pErec, CDBZmSub* pZm);
	BOOL tkrec_cnd() { return m_bKanaSearch; };

	// 部門枝番時の科目枝番の開始残高
	char	m_KEzan[6];
	BOOL	m_bKEzanFlg, m_bKEzanInp;
	BOOL	m_bDialogOK;
	BOOL	m_bDBupdate;
	int		m_TkMax;

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CEdaRegDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	EDA_RECORD* m_pEDrec;
	EDAREG_BUF	m_ERbuf;
	BOOL		m_bKanaSearch;
	CDBZmSub*	m_pZm;

	long	TkyoNoInp( UINT ID, long nChar, long inplen, long kst );
	long	ValInp( UINT ID, long nChar, long inplen, long kst );
	void	EdaTkyoSet( int tkcod );
	void	set_focus( UINT ID );

	int		EdaNameInp(long inplen);
	int		EdaKanaInp(long inplen);

	long TerminationOnOK( long nChar );
	long TerminationOnCancel( long nChar );
	long TerminationList1( long nChar );

	int  SrchTKname( char* teki, int* tkno );
	int  TkyregJob( int& tkycode );
	int  TkyRegCondition();
	long TerminationCheckTkyReg( long nChar );
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEdaRegDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSetFocusIcsdbedtctrl1();
	afx_msg void OnKillFocusIcsdbedtctrl1(long inplen);
	afx_msg void OnTerminationIcsdbedtctrl1(long nChar, long inplen, long kst);
	afx_msg void OnSetFocusIcsdbedtctrl2();
	afx_msg void OnKillFocusIcsdbedtctrl2(long inplen);
	afx_msg void OnTerminationIcsdbedtctrl2(long nChar, long inplen, long kst);
	afx_msg void OnSetFocusIcsdbedtctrl3();
	afx_msg void OnKillFocusIcsdbedtctrl3(long inplen);
	afx_msg void OnTerminationIcsdbedtctrl3(long nChar, long inplen, long kst);
	afx_msg void OnImeEndCompositionIcsdbedtctrl3(short nChar, LPCTSTR string, LPCTSTR ystring);
	virtual void OnOK();
	afx_msg void OnDblclkList1();
	afx_msg void OnSetFocusIcsdbedtname();
	afx_msg void OnTerminationIcsdbedtname(long nChar, long inplen, long kst);
	afx_msg void OnSetFocusIcsdbedtkana();
	afx_msg void OnTerminationIcsdbedtkana(long nChar, long inplen, long kst);
	afx_msg void OnKillFocusIcsdbedtkana(long inplen);
	afx_msg void OnKillFocusIcsdbedtname(long inplen);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT RetroFocus( WPARAM wParam, LPARAM lParam );
public:
	void SetFocusIcsdbedtKezan();
	void KillFocusIcsdbedtKezan(long inplen);
	void TerminationIcsdbedtKezan(long nChar, long inplen, long kst);
public:
	void ImeEndCompositionIcsdbedtname(short nChar, LPCTSTR string, LPCTSTR ystring);
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

};
