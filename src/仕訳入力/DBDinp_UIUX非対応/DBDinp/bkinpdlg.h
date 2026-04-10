// BkInpDlg.h : ヘッダー ファイル
//
//#include "ICSDialog.h"
//{{AFX_INCLUDES()
#include "icsdbedt.h"
//}}AFX_INCLUDES

/////////////////////////////////////////////////////////////////////////////
// CBkInpDlg ダイアログ

#ifdef	_ICSPEN_	// 01.20 /01
//#include "ICSPen.h"
#endif



class CBkInpDlg : public ICSDialog
{
// コンストラクション
public:
	CBkInpDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CBkInpDlg)
	enum { IDD = IDD_BKINP_DLG };
	CICSDBEDT	m_DBedt1;
	CICSDBEDT	m_DBedt2;
	//}}AFX_DATA
	ICSStatic	m_StDbk;
	ICSStatic	m_StCbk;

	BOOL Create();
	void Bkinp_Init( int mode, CDBINPDataRec* data, struct _DATA_LINE* dtline );
	void get_originalsize( double& w, double& h );

	int	GetMode() { return m_Mode; }

	BOOL bk_F2job();
	void set_fastfocus( BOOL modefy_sg, int pn );
	void set_nowfocus();
	void SelectedIcsselctrl( short position, long Number, int place );

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CBkInpDlg)
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
	RECT	m_original_rect;
	BOOL	m_MODEFY_OK;
	BOOL	m_THIS_DIALOG_OK;

	CDBINPDataRec*		m_pData;
	struct _DATA_LINE*	m_Dtline;
	// Now ポジション
	int m_NOW_PN;
	//直前のポジション
	int m_PREV_PN;

	struct _SET_KMKETC m_DebtSK;
	struct _SET_KMKETC m_CredSK;

protected:
	struct _InpCtl *getINP_CTL( int pn );

	void set_focus( int pn );

	int set_nowpn( int pn );
	int get_nowpn( void );
	int get_prevpn();

	void DBeditCond( int mode );
	void set_etcdsp( int pn );

	void bk_select_dsp( int dc_sw, BOOL bTerm );
	void TerminationBumon( UINT ID, long nChar, long inplen, long kst );
	int  CheckImeEndBmon( UINT ID, long nChar, LPCTSTR string );

	void TerminationKoji( UINT ID, long nChar, long inplen, long kst );
	int  CheckImeEndKoji( UINT ID, long nChar, LPCTSTR string );

	void TermWork( int dc_sw, long nChar );
	void to_parent( long nChar );

	void SetBmnEtcDisp( int pn, BOOL bFocus = FALSE );
	BOOL bk_modefy_ok();

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CBkInpDlg)
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
	LRESULT RetroFocus( WPARAM wParam, LPARAM lParam );

	afx_msg void OnSetFocusIcsdbedtctrl(UINT ID);
	afx_msg void OnKillFocusIcsdbedtctrl(UINT ID,long inplen);
	afx_msg void OnTerminationIcsdbedtctrl(UINT ID,long nChar, long inplen, long kst);
	afx_msg void ImeEndCompositionIcsdbedtctrl(UINT ID, short nChar, LPCTSTR string, LPCTSTR ystring);

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
