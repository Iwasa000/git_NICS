// SkbnIn.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CSkbnIn ダイアログ

//#include "icsdialog.h"
#include "icsdbedt.h"
#include <SyzInvoice.h>

//typedef struct _INVOICE_INFO
//{
//	int		inv_tkNo;		// 摘要番号
//	char	inv_tkNam[64];	// 摘要名称
//	char	inv_invNo[16];	// 登録番号
//} INVOICE_INFO;



class CSkbnIn : public ICSDialog
{
// コンストラクション
public:
	CSkbnIn(CWnd* pParent = NULL, BOOL bToku = FALSE, BOOL bResize = TRUE, bool bInvMaster = false, bool bInvUse = false, 
				bool bInvDisp = false, int inpMode = INPMODE_NORMMAL, int koujo8050 = SYZ_MEN_NOT );   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSkbnIn)
	enum { IDD = IDD_SYKBN_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	CICSDBEDT	m_DBedt1;
	CICSDBEDT	m_DBedt2;
	CICSDBEDT	m_DBedt3;
	CICSDBEDT	m_DBedt4;
	CICSDBEDT	m_DBedt5;
	CICSDBEDT	m_DBedt6;
	CICSDBEDT	m_DBedtToku;
	CICSDBEDT	m_DBedtInv;
	CICSDBEDT	m_MenWari;
	CICSDBEDT	m_DBedtInvDisp;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CSkbnIn)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

public:
	CWnd*		m_pParent;
	CDBZmSub*	m_pZm;
	CDBSyohi*	m_pSy;

	// インボイス対応
	CSyzInvoice	syzInvoice;
	CInvnoMng	invmng;

	CRect		m_DlgRect, m_orgRect;
	BOOL		m_Dispflg;

	BOOL	m_MODEFY_OK;
	int		m_THIS_DIALOG_OK;

	BOOL		m_bToku;
	BOOL		m_bResize;
	DWORD		m_defSwkKbn;
	BOOL		m_bMsgDsp;
	BOOL		m_bIgnoreFocus; //フォーカス無視

//	ZmselDBData	zdbsel;
//	void	(CALLBACK* callfunc)( UINT, void*, CWnd* pWnd );

	void set_fastfocus( BOOL modefy_sg, int pn );
	BOOL skbn_dataset( int *set_pn );
	int	 SetSkbnSelNo( int no );
	void set_nextpos();
	void set_nowfocus();

	int  CheckSyzPn( int vect );

	void DlgRectCheck( BOOL& bVert, BOOL& bHorz, CRect* zkbnRect, int* width);
	int GetDlgWidth();
	int GetDlgHeight();
	void SetInvPageCount(int count) {
		invmng.m_invPGCnt = count;
	}
	void SetInvArray(CArray<INVOICE_INFO> *ary)
	{
		invmng.SetInvArray(ary);
	}
	void RenewInvArray(CArray<INVOICE_INFO> *ary)
	{
		invmng.SetInvArray(ary);
		if( get_nowpn() == SY_INV_PN ) {
			invmng.InvDispPage(0);
		}
	}
	void CancelInvNoIME()
	{
		invmng.CancelIME();
	}
	BOOL CheckInvnoInput();

private:
	// インボイス対応
	bool m_bInvMaster;	// インボイスバージョンアップマスター
	bool m_bInvUse;		// インボイス期間マスター
	bool m_bInvDisp;	// インボイス登録番号表示フラグ
	int m_InpMode;		// 入力形式（通常入力：伝票入力）

	int	 m_Koujo80_50;

private:
	// インボイスコントロールの移動
	void MoveInvCtrl();
	void MoveInvCtrl( bool bAll );

	// インボイスコントロール表示・非表示切り替え
	void ShowInvCtrl();
	void ShowInvCtrl( bool bAll );
	// 免税事業者からの課税仕入れのチェック切り替え
	void SetMenzeiCheck();


// インプリメンテーション
protected:

	struct _InpCtl *getINP_CTL( int pn );
	void set_focus( int pn );

	void	MoveCtrl();

	BOOL	sydata_set( int *set_pn );
	void	kbn_select_make( struct _SY_MSG_TBL *sel, int pn );

	int		set_nowpn( int pn );
	int		get_nowpn( void );
	int		get_prevpn();
	int		get_nextpn( int now, int vk = 0 );
	int		get_prevpn( int now );
	void	to_parent( CWnd *parent, long nChar );

	BOOL	dc_kmdefcheck( int sgn );

	struct _SY_MSG_PACK2 m_smpk;
	int		m_selbox_user;

	int		OnTerminationDBEdt1(long nChar, long inplen, long kst);
	int		OnTerminationDBEdt2(long nChar, long inplen, long kst);
	int		OnTerminationDBEdt3(long nChar, long inplen, long kst);
	int		OnTerminationDBEdt4(long nChar, long inplen, long kst);
	int		OnTerminationDBEdt5(long nChar, long inplen, long kst);
	int		OnTerminationDBEdt6(long nChar, long inplen, long kst);
	int		OnTerminationDBEdtToku(long nChar, long inplen, long kst);
	int		OnTerminationDBEdtInv( long nChar, long inplen, long kst );
	int		OnTerminationCheckMenzei( DWORD nChar );
	HACCEL	m_hAccelTable;

	CBrush	m_brBk, m_brDefBk;

	long	CheckDataDBeditInv(long nChar, LPCTSTR string);
	CString m_ErrMsg;

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSkbnIn)
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	LRESULT	RetroFocus( WPARAM wParam, LPARAM lParam );

	afx_msg void OnSetFocusIcsdbedtctrl(UINT ID);
	afx_msg void OnKillFocusIcsdbedtctrl(UINT ID,long inplen);
	afx_msg void OnTerminationIcsdbedtctrl(UINT ID,long nChar, long inplen, long kst);
	afx_msg void ImeEndCompositionIcsdbedtctrl(UINT ID, short nChar, LPCTSTR string, LPCTSTR ystring);

public:
	CButton		m_MenzeiCheck;
	void	OnBnClickedMenzei();

	void	MenWariDBEdit(BOOL bEnable);
	void	MenWariSetDBedtData();
	int		OnTerminationDBEdtMen(long nChar, long inplen, long kst);
	afx_msg void OnSetfocusCheckMenzei();

	void	MenCheckButtonJob(long nChar);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	LRESULT OnMyMessage(WPARAM wp, LPARAM lp);
};
