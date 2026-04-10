// ScanView.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CScanView フォーム ビュー

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "DataListCtrl.h"
#include "icsdisp.h"


//１スキャン行データ
struct _SCAN_LINE {
	unsigned short		ZREC_NO;	//ZREC[ZREC_NO-1]

	DTLIST_DATA			Data;

	int					ImgSEQ;	// イメージ番号(なしのときは 0)
 
	struct {
		short			INP_sg;		// 入力済みサイン
		char			IMG_sg;		// イメージサイン
		char			ETC_sg[3];	// 他サイン
	} LINE_CND[ LNCND_MAX ];

	char	Denp[4];
};



class CScanView : public ICSFormView
{
public:
	CScanView();           // 動的生成に使用される コンストラクタ。

protected:
	DECLARE_DYNCREATE(CScanView)

// フォーム データ
public:
	//{{AFX_DATA(CScanView)
	enum { IDD = IDD_SCAN_FORM };
		// メモ: ClassWizard はこの位置にデータメンバを追加します。
	CButton	m_Btn1;
	CButton	m_Btn2;
	CButton	m_Btn3;
	//}}AFX_DATA
	CDataListCtrl	m_ScList;
	CICSDBEDT		m_NumInp;
	CICSDisp	m_Waning;

// アトリビュート
public:


protected:
	CString		SEQ_PATH;
	CString		RequeryStr;
	
	BOOL		m_bChkList;
	int			m_ScanSeq;
	int			m_OldIndex;	//リスト上の移動前カレントライン

	int			m_moveSeq;

#ifdef _SLIP_ACCOUNTS_
	CImgMng			*m_pImgmng;			// イメージ表示用クラス
	int				m_Imgcnt;			// イメージカウンタ
	BOOL			m_IsAbleImg;		// イメージ表示可能
	IMG_DISPOPTION	m_ImgDispOption;	// イメージ表示オプション
	BOOL			m_bSLIP_MASTER;		// 原票保存マスター？ ('12.09.25)
	BOOL			m_bOCRS_ONLY;		// OCR-Sのみ保存マスター？
#endif

	BOOL		m_bDspScList;
	BOOL		m_bSendLineMove;

	BOOL		m_callbyGenpyo;

	// scan_onsizedispを行うか？
	BOOL		m_bIgnoreOnsizeRedisp;

// オペレーション
public:
	void OnInitialize( LPCTSTR seq_path );
	void SetScanListUseDefColor( BOOL bDefUse );
	void SetListColumn( int line );

	void scan_init_rd( int seq, int scan_sgn, int requery = 1, int dispseq = -1 );
	void scan_init_seqrd( int seq );
	void scan_now_dsp( int ofs );

	int scan_zrec_set( CDBINPDataRec *dta, int ofs );
	BOOL seq_data_rd( CDBINPDataRec *data, int seq, int ofs, int vect );

	int scan_data_rd( CDBINPDataRec *dta, int ln_max, int ln );
	int init_dataread( CDBINPDataRec *data, int nl );

	int _init_dataread( CDBINPDataRec *data, int nl );
	int _init_sortread( CDBINPDataRec *data, int nl );

	int scan_get_rdatan();
	int scan_rdpnt();

	void	ChecklistUcomSet( int type );
	BOOL&	IsCheckListPrint() { return m_bChkList; }

	void	ForkUcomSet();
	int		ReloadView();

	void	OnActivateJob(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	void	UpdateDataLineDisp();

#ifdef _SLIP_ACCOUNTS_
	int	TabLbuttonDownJob();
	// イメージ表示オプション変更 ('12.09.26)
	int ImageOptionChanged();
#endif

	int		GetScListCount();
	BOOL	DenpNoInpCheck( int denp_no );

	BOOL&	IsCallByGenpyo() { return m_callbyGenpyo; };
	void	set_insdata_toscan( int ins_seq, int btm_seq );
	void	set_scan_bottom( int ins_seq );
	int		denpend_keyscan_search( int seq );

	int		Reload_DataScan();
	void	scan_init_wizread(CString filter);

protected:
	int scan_dpage_down();
	int scan_dpage_up();
	void remain_line_move( int max, int trn );

	int scan_dscroll_down( int scrol_nl );
	int scan_dscroll_up( int scrol_nl );

	void scan_scroll_down( struct _SCAN_LINE *ldta, int max, int ln );
	void scan_scroll_up( struct _SCAN_LINE *ldta, int max, int ln );

	int _dline_chg( int idx, int focs_sgn = 0 );
	void Set_SeqInp( int index );
	void set_focus( int pn );

	int _EnterEnable();
	int EnterEnable( struct tagMSG* pMsg );

	BOOL tomdfy();
	void tomdfy2( int seq, int now_seq );

	void delete_job();

	void scan_init_dsp();
	void waning( BOOL direct );

	void mk_dspdata( struct _SCAN_LINE *ldta, CDBINPDataRec *data, int data_n, int nline );
	void mk_1dspdata( struct _SCAN_LINE *ldta, CDBINPDataRec *data );
	void scan_recset( struct _SCAN_LINE *ldta, CDBINPDataRec *dta );
	void mk_zrolindt( struct _SCAN_LINE *ldta );
	void mk_zrodata( struct _SCAN_LINE *ldta );
#ifdef BIGIMG	// 03.16 /00
	void set_imgtky( struct _SCAN_LINE *data, int index );
#else
	void set_imgtky( struct _SCAN_LINE *data );
#endif

	void scan_ddata_dsp( struct _SCAN_LINE *ldta, int nline, int vect );
	void scan_line_dsp( struct _SCAN_LINE *data, int n, BOOL bRedraw = FALSE );

	void scan_redisp();
	void scan_onsizedisp();

	BOOL NoInpBrnCheck( CString& code, int brn );
	BOOL BmnNoInpCheck( CString& code, int bmn, CString& paircd );
	BOOL BmnNoInpBrnCheck( CString& code, int brn, int bmn );
	BOOL KojiNoInpCheck( CString& code, CString& kno );

	void Rdadd_job();

	void sel_shiftjob( int ln, long nChar, BOOL bPress );
	void sel_lineupdate();
	void click_seljob( int line, long nChar );
	void RButtonMenuJob( UINT nID, CPoint point );
	BOOL CheckCopySeq( int line );

	void scan_seqchg_read( int scan_sgn, int dispseq = -1 );
	void seqchg_curline();
	BOOL seqchg_swkcheck( int aryCnt );

	void scan_ctrlmove();
	void denpend_redisp( int seq );
	int  ReDispGenpyoImg();
	void GetImgDispRECT(RECT* dipsRect);

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CScanView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CScanView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CScanView)
	afx_msg void OnButtonEnd();
	afx_msg void OnButton1();
	afx_msg void OnButtonF8();
	afx_msg void OnUpdateButtonF8(CCmdUI* pCmdUI);
	afx_msg void OnButtonF9();
	afx_msg void OnButtonDmyf42();
	afx_msg void OnUpdateButtonDmyf42(CCmdUI* pCmdUI);
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonHome();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void	OnScanListSelChanged( NMHDR* pNMHDR, LRESULT* pResult );
	void	OnScanListDblClick( NMHDR* pNMHDR, LRESULT* pResult );
	void	OnScanListColumnClick( NMHDR* pNMHDR, LRESULT* pResult );

	LRESULT DspScanTBL( WPARAM wParam, LPARAM lParam);

	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo );
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo );
	virtual void OnPrint(CDC* pDC, CPrintInfo*);

#ifdef	_ICSPEN_	// 01.20 /01
protected:
private:
	BOOL	m_checkfirst;
	BOOL&	FirstTime() { return	m_checkfirst; }

//	short	SetPen( BOOL flg );

//	BOOL	flgPen,fPen;
//	ICSPen*	icspen;
//	void OnICSInputFocus( );

public:
//	BOOL&	IsPen() { return fPen; }

#endif

public:
	BOOL	EndApp( WPARAM wParam = 0, BOOL bCosel = FALSE );
	void	ScanCloseWork();
	BOOL	SlipCheckJob();

public:
	afx_msg void OnBnClickedButton2();
public:
	afx_msg void OnBnClickedButton3();
public:
	DECLARE_EVENTSINK_MAP()
public:
	void TerminationIcsdbedtctrl1(long nChar, long inplen, long kst);
public:
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnButtonF5();
public:
	afx_msg void OnUpdateButtonF5(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonDelete(CCmdUI *pCmdUI);

	afx_msg void OnButtonF3();
	afx_msg void OnUpdateButtonF3(CCmdUI *pCmdUI);

	afx_msg void OnButtonF7();
	afx_msg void OnUpdateButtonF7(CCmdUI *pCmdUI);
	afx_msg void OnButtonF11();
	afx_msg void OnUpdateButtonF11(CCmdUI *pCmdUI);
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void OnSwkCopy();
#ifdef _SLIP_ACCOUNTS_
	afx_msg void OnMenuSlip();
	afx_msg void OnUpdateMenuSlip(CCmdUI *pCmdUI);
#endif
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

/////////////////////////////////////////////////////////////////////////////
