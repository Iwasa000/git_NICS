// DBAbookView.h : CDBAbookViewクラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#include "DataListCtrl.h"

#include "AcbookRec.h"
#include "DBbaseView.h"

#include "rdtatbl.h"
#include "Rdremaindlg.h"


// データ移動サイン

#define	ACD_SCROLLUP	-1
#define	ACD_SCROLLDOWN	-2
#define	ACD_PAGEUP		-3
#define	ACD_PAGEDOWN	-4


#define	ACREC_MAX	40
#define	acINP_mode	*acfnINP_mode()

// 仕訳検索条件
#define	ACQ_NONE	0
#define ACQ_BUMON	0x01
#define ACQ_KOJI	0x02
#define ACQ_EDA		0x04

//{{AFX_INCLUDES()
#include "icssel.h"	
#include "icsdbedt.h"
#include "icsdisp.h"
#include "icsstatic.h"
//}}AFX_INCLUDES



typedef CArray<CACSzan, CACSzan&>	CSZanArray;

#define ACBFOCUS_DEBUG (1)   //0:Release / 1:Debug



class CDBAbookView: public CDBbaseView
{
protected: // シリアライズ機能のみから作成します。
	CDBAbookView();
	DECLARE_DYNCREATE(CDBAbookView)

public:
	//{{AFX_DATA(CDBAbookView)
	enum { IDD = IDD_DBABOOK_FORM };
	CICSDBEDT	m_Input;

	CICSDBEDT	m_BmnInp;
	CICSDBEDT	m_KojiInp;
	CICSDBEDT	m_KamokInp;
	CICSDBEDT	m_BrnInp;

	//}}AFX_DATA
	CDataListCtrl	m_List;
	//}}AFX_DATA
	ICSDateSelect m_selDate;

// アトリビュート
public:
//	CDBAbookDoc* GetDocument();
	void to_acbkmode();
	void BaseKmkIndCheck();

	virtual void LastDataGet();
	virtual void ForkUcomSet();
	virtual int  ReloadView();
	virtual void ReCoselSetFocus( BOOL bNewCo );

	virtual int TabLbuttonDownJob();

	int		ReCosel_Job();

	int		GetRecordDCsgn( CDBINPDataRec* drec );
	void	GetACRdselBaseStr( CString& bmn, CString& koji, CString& kmk, CString& brn);
	BOOL	IsACRdData( CDBINPDataRec* rec );
	int		reg_sort( int seq1, int seq2 );

	void	MakeRemainDlg();
	void	RemainDlgDelete();
	void	ForkActiveJob();

	BOOL	CalcWindowDelete();
	void	GetAcbookItemWidth(int saveAlways =0 );
	// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CDBAbookView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV のサポート
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo*);
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CDBAbookView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	BOOL	m_bMnthFix;
	BOOL	m_bIgnoreTermVal;
	int		m_TopIndex;
	int		bINP_mode;
	BOOL	m_bSyzYunyu;	// 輸入仕入れに今回変更したか？サイン
	BOOL	m_bSyzSotoMen;	// 外税で免税事業者からの課税仕入れ
	BOOL	m_pnZeiSotoMen;		// 外税で免税事業者からの課税仕入れで税額欄へ フォーカスした前ポジション
	int		m_NowPN, m_NowLine;
	int		m_TKsyz, m_TKsyz2;
	// 摘要消費税 保存レコード
	CDBipTKREC	m_TKattrec;
	// 摘要仕訳のレコード
	CArray < ICS_TKJOUR_PAC, ICS_TKJOUR_PAC& > m_tkjourAry;
	int		m_AutoSwkCnt, m_AutoSwkTrn;
	// 摘要複数自動仕訳
	CDBipTKREC	m_autoTKrec;
	BOOL		m_bAutoKmkSet;

	int		m_InsLine;		// 挿入時のリスト行番号(１から始まる)
	BOOL	m_bAutoSwk;		// 自動仕訳モード
	BOOL	m_bIgnoreDateTerm;
	CSZanArray	m_SZan;		// 差引残高配列

	BOOL	m_bLinChg;
	BOOL	m_bTermValLinChg;

	DWORD	m_dwQuery;		// 出納帳仕訳検索タイプ
							//	D0:	部門
							//	D1:	工事番号
							//	D2:	枝番

	DWORD	m_dwOpt;		// D0 OFF: 日付順, ON:SEQ順
							// D4-D7	0=固定なし, 1=入金欄固定, 2=出金欄固定

	//リストのカラム情報 <カラムID, カラム表示幅(文字数)>
	map<int, int>	m_clmMap;

	int		m_ListCnt;
	ACBOOK_LINE	m_ALine[ACREC_MAX];		// 画面上に表示している 仕訳行

	CDBINPDataRec	m_CREC, m_SREC, m_SCAN_rec, m_appSREC;

	BYTE	m_svInpDate[6];

	struct _SET_KMKETC m_BmnSK;
	struct _SET_KMKETC m_KojiSK;

	CString m_BaseCode;
	int		m_BaseAtt;
	int		m_BaseEda;
	int		m_BaseBmn;
	CString m_BaseKoji;

	int		m_BaseDate, m_BaseEndDate;
	int		m_BaseOfst, m_BaseEndOfs;
	int		m_BaseKsign;
	int		m_NOBRNCHK;
	int		m_ACKOJICHK;

	int		m_sdbtBmn, m_screBmn;

	BOOL	m_bReloadFocus, m_bActiveFocus;

	BOOL	m_bTkyLineChg;
	BOOL	m_bLineDispChg;
	BOOL	m_bMnthFocusIgnore;

	// 仕訳行選択
	CDWordArray	m_SelArray;		// 選択データのSEQ配列
	int			m_SelDirection;	// 0 = 選択なし状態, -1 = 上, 1 = 下
	int			m_SelType;		// 0 = 選択なし,	1 = shiftキー
								//					2 = ctrlキー
	int			m_SelSeq;		// 現在の選択SEQ
	BOOL		m_SelUpdate;	// 選択データ画面再表示

	CString		m_tkyStr;

	CRdremainDlg*	m_pRDremainDlg;

	void	OnSetFocusBaseBumon();
	void	OnKillFocusBaseBumon(long inplen);
	void	OnTerminationBaseBumon(long nChar, long inplen, long kst);
	void	OnSetFocusBaseKoji();
	void	OnKillFocusBaseKoji(long inplen);
	void	OnTerminationBaseKoji(long nChar, long inplen, long kst);
	int		CheckImeEndKoji( int pn, long nChar, LPCTSTR string );

	void	OnSetFocusBaseKamok();
	void	OnKillFocusBaseKamok(long inplen);
	void	OnTerminationBaseKamok(long nChar, long inplen, long kst);
	void	OnImeEndCompositionBaseKamok(short nChar, LPCTSTR string, LPCTSTR ystring);
	long	CheckDataBaseKamoku( long nChar, LPCTSTR string );

	void	OnSetFocusBaseBrn();
	void	OnKillFocusBaseBrn(long inplen);
	void	OnTerminationBaseBrn(long nChar, long inplen, long kst, int priodinp = 0);
	int		CheckImeEndBaseBrn( long nChar, LPCTSTR string );

	int  TopDataSet(int mofs = -1 );
	int  InitDataSet();
	int  DataAppendReset( CDBINPDataRec* pcrec );

	void BaseSzanSet( CACSzan* pas );
	BOOL IsMoveBaseEda();
	BOOL IsMoveBaseKOJI();
	BOOL IsMoveBaseBmon();

	void ListDataSet();
	void BaseWndMove();
	void acline_init( ACBOOK_LINE* aline, int size );
	void acline_set( ACBOOK_LINE* aline, int idx, int abpos );
	void acline_zanset(ACBOOK_LINE* aline, CACSzan* aczan );

	int  UpdateModfyData( int nomsg = 0 );

	int acline_scrolup();
	int acline_scroldown();
	int acline_pageup();
	int acline_pagedown();
	BOOL PageMoveDataUpdate(int nowln );

	int	 Acbook_Requery();
	void acbook_datadelete();
	BOOL syzvol_kmkchk( CString& dbt, CString& cre );

	void ListClmSet( BOOL bOnSize = FALSE );
	void DBeditCtrlSet( int id_pn );

	void set_nowpn( int pn, int reload = 0 );
	int	 get_nowpn();
	void set_nowln( int ln );
	int	 get_nowln();
	BOOL isdeldata();
	BOOL _isdeldata( int ln );

	int* acfnINP_mode();
	int  DB_SyIniz( CDBINPDataRec* data );

	BOOL isNoBRNInpChk();
	BOOL isKojiCheck();
	BOOL isBmnNoInp();

	ACBOOK_LINE* getLINE_DATA();
	ACBOOK_LINE* _getLINE_DATA( int ln );

	char *sy_chk_txt( CDBINPDataRec *data,  struct _SY_MSG_PACK2* smp, int inp_mode );
	char *sy_line_txt( CDBINPDataRec *data );

	BOOL IsSameKamoku();
	BOOL _IsSameKamoku( CDBINPDataRec* drec );

	void SetAcbookKamoku(CDBINPDataRec* rec, struct _KamokuData* pKm );
	int  GetKcodeDCsgn( LPCTSTR code );

	int ChangeDebtCred( int chg/*=0*/);
	int SzanUpdate( int ln, CDBINPDataRec* now, CDBINPDataRec* old );
	void SzanDataSet( int ln );
	int WriteAndNextData();
	void AutoSiwake( int ln, BOOL bAuto );

	void AutoSwkSetJob(CDBINPDataRec *crec, int tkjourseq);

	BOOL brnitm_to_tky( int ln, int bmn, CString& code, int brn );

#if (ACBFOCUS_DEBUG == 1)
	void set_focusTrace(int pn, int iClick = 0, int line = 0, char* func = 0, char* fname = 0);
	void _set_focusTrace(int ln, int pn, int iClick = 0, int line = 0, char* func = 0, char* fname = 0);
#else
	void set_focus( int pn, int iClick = 0 );
	void _set_focus(int ln, int pn, int iClick = 0);
#endif
	void set_termfocus( int ln, int pn, int vect_n );

	int get_dctrl_id( int pn );
	int dctrlid_toacpn( int id );
	
	void disp_change();
	void _disp_change();
	void set_etcdsp( int ln, int pn );
	void line_dispchg( int dispcnt, int oldcnt );

	int _dline_chg( int ln, int pn, int focs_sw = 0 );
	int dline_chg( long nChar );

	void LineDataClear( int ln );
	BOOL AppendDataCheck( int ln );

	int  RequeryACLine( int nxtseq, int ln, int top_zan =0 );
	int	 InsertSiwake( int ln );
	int  BackNormalAppendMode( int ln );

	void Destroy_ModeLessDlg();

	long CheckDataIcsDate( long nChar );
	void FocusIcsDate( int ln, short click );
	void TerminationIcsDate( long nChar, long inplen, long kst );
	void DateInputChange();
	BOOL AcbookDateCheck( BYTE* bcddate );

	void FocusIcsBumon( int ln, short click );
	void TerminationBumon( int ln, long nChar, long inplen, long kst );
	void bmon_select_dsp( int dc_sw, CDBINPDataRec *pCrec, BOOL bTerm );
	BOOL BmnCodeChk( int bmn, int sw, int pn );

	void FocusIcsKoji( int ln, short click );
	void TerminationKoji( int ln, long nChar, long inplen, long kst );
	void koji_select_dsp( int dc_sw, CDBINPDataRec *pCrec, BOOL bTerm );
	BOOL KojiCodeChk( CString koji, int sw, int pn );
	void bmnkoji_to_tekiyo( CDBINPDataRec* rec, CString* kjnam, int type = 0 );

	void FocusIcsKamoku( int ln, short click );
	long CheckDataKamoku( long nChar, LPCTSTR string );
	void TerminationKamoku( int ln, long nChar, long inplen, long kst );
	void KanaTermKamoku(LPCTSTR data);
	int  GetKamokuSign();

	int FocusIcsVal( int ln, int ns_sw, short click );
	void TerminationVal( int ln, int ns_sw, long nChar, long inplen, long kst );

	int FocusIcsZei( int ln, short click );
	void TerminationZei( int ln, long nChar, long inplen, long kst );

	void FocusIcsTekiyo( int ln, short click );
	void TerminationTekiyo( int ln, long nChar, long inplen, long kst );
	void _KanaTermIcsTekiyo(LPCTSTR data, BOOL bTkcut);

	int  tky_select_to_drec( CDBINPDataRec *CREC, CDBipTKREC *ptk, int kanaterm = 0 );
	int  tky_select_to_drec_sub(CDBINPDataRec *crec, CDBipTKREC* ptk, int nowln = -1, int tkbrn = 0);
	void tky_select_dsp( CDBINPDataRec *CREC, int dc_sw, CDBipTKREC *ptk, BOOL F12_KEY, BOOL bTkcut = FALSE, int nowln =-1);
	int  tkybrn_search( CDBINPDataRec *data, int tkcd, ABLN_CND* cnd );

	void FocusIcsSyz( int ln, short click );
	int  TerminationSyz( int ln, long nChar, long inplen, long kst );

	void FocusIcsDenp( int ln, short click );
	void TerminationDenp( int ln, long nChar, long inplen, long kst );

	void FocusIcsDocevi( int ln, short click );
	void TerminationDocevi( int ln, long nChar, long inplen, long kst );

	long TermAppendData( long nChar );

	void SetBmnEtcDisp( BOOL bFocus );
	void SetKojiEtcDisp( BOOL bFocus );

	int  SyzInpDisp();
	void Hide_SyzInp();

	void TerminationDataSet( int pn, int nowln = -1 );
	void TerminationDataSet( int pn[], int pncnt, int nowln = -1 );
	void _TerminationDataSet( DTLIST_DATA* data, ACBOOK_LINE *dtline, int pntbl[], int tblcnt );

	void _OnTerminationIcsdbedtctrl( long nChar, long inplen, long kst );

	void SelectReset();
	void SelectedIcsselctrl(short position, long Number, int place ) ;

	BOOL base_selected( int pn, short position, long Number, int place );
	BOOL check_basebrn();

	int		set_nonICSpn( tagMSG* pMsg );
	long	OnTermBaseDispBtn( long nChar );

	void HusenInpDisp();
	void Destroy_HusenInp();

	void HjiyuInpDisp();
	void Destroy_HjiyuInp();

	void FocusIcsHjiyu( int ln, short click );
	void HendJiyuCheck( CDBINPDataRec *pcrec, int clear = 0 );
//	BOOL IsEnableHjiyu( CDBINPDataRec *pcrec );
	LRESULT OnHjiyuInEnd( WPARAM wParam, LPARAM lParam );
	LRESULT OnHjiyuSel( WPARAM wParam, LPARAM lParam );
	void _hjiyu_select( int selno );

	void SubDataInpDisp( int pn );
	void Destroy_SubDataInp();

	void _OnBnClickedButtonDisp();
	void DataLineUpdate( CDBINPDataRec *rec, int line, int rdsgn = 0 );

	void BaseBmnCtrl();
	BOOL bmncode_todata2( CDBINPDataRec* data, int number, int dc_sw, CString* bmnname );
	BOOL isBmnHaniCheck();

	int CheckKamokuInput( struct _KamokuData* data );
	int CheckBaseBumonInput( int bmn );
	int CheckBaseKamokuInput( struct _KamokuData* data );

	int CheckModifyBmonData( CDBINPDataRec *data, int nowln, int nomsg = 0 );
	int CheckModifyEdabanData( CDBINPDataRec *data, int nowln, int nomsg = 0 );

	void ReloadDataDisp();
	BOOL	isKojiKmkCheck();
	int		CheckBaseKojiInput( CString kno );
	int		CheckKoji_BaseKamokuInput( struct _KamokuData* data );
	int		CheckModifyKojiKmkData( CDBINPDataRec *data, int nowln, int nomsg = 0 );
	void	BaseKojiCtrl();

	void	AutoSelIndicator( int inp_mode );
	void	SetAcbkOptionVal();
	void	StatusBarValGuide();

	BOOL	IsSzanCalq( CDBINPDataRec* pcrec );
	BOOL	IsSameDbtCre( CDBINPDataRec* pcrec );

	int		CheckImeEndBumon( int pn, long nChar, LPCTSTR string );

	int		GetACListCount();
	BOOL	BmnIMEkeydownJob( long nChar );

	void	sel_shiftjob( int ln, long nChar, BOOL bPress );
	void	click_seljob( int line, long nChar );
	void	sel_lineupdate( BOOL bDoUpdate = FALSE );
	void	RButtonMenuJob( UINT nID, CPoint point );
	BOOL	CheckCopySeq( int line );

	void	Rdadd_job();
	BOOL	RdataToInpData( CDBINPDataRec *crec, CDBINPDataRec *srec );
	void	RDRemainDisp( BOOL sw );

	virtual void RDRemainDispJOB( void );
	virtual int TabClickChangedPrevView();
	virtual BOOL EnableCallPro();

	void	UpdateACZanRec( int abs_pos, CDBINPDataRec* rec );
	int		GetClmSize( int clmid);

	int		data_denpdialog( int ln );
	int		IsAbookDiff_Data( CDBINPDataRec *newdata, CDBINPDataRec *moto );
	BOOL	ChangeZeiritsu();
	BOOL	ChangeSotoMenZei();

	NM_DLCTVIEW	m_NMbegin;
	void	InitNMbeginLabel();

	int		RemainSwkdataCheck(CString& strType);
	int		EndRemainAutodata();
	int		EndRemaindata();

	void	BaseBmnEda_update();
	void	bmneda_update(CDBINPDataRec* rec);
	void	datetekiyo_check_tkrec(CDBINPDataRec *CREC);
// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CDBAbookView)
	afx_msg void OnButtonEnd();
	afx_msg void OnButtonF11();
	afx_msg void OnSetFocusIcsdbedtctrl(UINT ID);
	afx_msg void OnKillFocusIcsdbedtctrl(UINT ID, long inplen);
	afx_msg void OnTerminationIcsdbedtctrl(UINT ID, long nChar, long inplen, long kst);
	afx_msg void OnImeEndCompositionIcsdbedtctrl(UINT ID, short nChar, LPCTSTR string, LPCTSTR ystring);
	afx_msg void OnKanaTerminationIcsdbedtctrl(UINT ID, LPCTSTR Kana);

	afx_msg void OnSetFocusIcsdbedtctrl1();
	afx_msg void OnKillFocusIcsdbedtctrl1(long inplen);
	afx_msg void OnTerminationIcsdbedtctrl1(long nChar, long inplen, long kst);
	afx_msg void OnImeEndCompositionIcsdbedtctrl1(short nChar, LPCTSTR string, LPCTSTR ystring);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	LRESULT OnRetroFocus( WPARAM wParam, LPARAM lParam );

	void OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult );
	void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	void OnSetDispInfo( NMHDR* pNMHDR, LRESULT* pResult);

	LRESULT OnSkbnInend( WPARAM wParam, LPARAM lParam );
	LRESULT DspSkbnIchiran( WPARAM wParam, LPARAM lParam );
	LRESULT OnSkbnLineDisp( WPARAM wParam, LPARAM lParam );
	BOOL	IsYunyuMove();

	LRESULT OnHusenInpMsg( WPARAM wParam, LPARAM lParam );
	LRESULT OnSubDataInEnd( WPARAM wParam, LPARAM lParam );

	LRESULT OnDateSelectMessageChangeMNTHSEL(WPARAM wParam, LPARAM lParam);

//注意！これより以下の7行は変更してはいけません
private:
	BOOL	m_checkfirst;
	BOOL&	FirstTime() { return	m_checkfirst; }
public:
	void	BeginApp();
	BOOL	EndApp( WPARAM wParam = 0, BOOL bCosel = FALSE );
	BOOL	EndChk();
	int		GetAutoSwkType();

	int		GetAcbkOption();
	int		GetAcbkKojiWidth();
	BOOL	NextAutoSwkData(CDBINPDataRec *crec, int& edainp);
	int		GetAutoSwkItem(ICS_TKJOUR_PAC* pJour, CString& code, int& eda, int& bmn, CString& kno);
	void	SetAutoSwkItemToRecord(int sgn, CDBINPDataRec* rec, CString code, int eda, int bmn, CString kno);
	HWND	GetKamokuCtrlHWND();

//	void	CalcMinMax( MINMAXINFO* info ) { ICSResizeSub::CalcMinMax( info ); }
public:
	afx_msg void OnButtonF9();
	afx_msg void OnButtonF8();
	afx_msg void OnButtonF7();

	void SelectedIcsselctrl1(short position, long Number);
	void SelectedIcsselctrl2(short position, long Number);

	afx_msg void OnButtonF12();
	afx_msg void OnBnClickedButtonDisp();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnUpdateButtonF4(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonF7(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonF8(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonF9(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonF12(CCmdUI *pCmdUI);
	afx_msg void OnButtonF2();
	afx_msg void OnUpdateButtonF2(CCmdUI *pCmdUI);
public:
	afx_msg void OnButtonInsert();
	afx_msg void OnUpdateButtonInsert(CCmdUI *pCmdUI);
	afx_msg void OnButtonDelete();
	afx_msg void OnUpdateButtonDelete(CCmdUI *pCmdUI);
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
public:
	ICSStatic m_StBmn;
	ICSStatic m_StKoji;
public:
	afx_msg void OnButtonF6();
public:
	afx_msg void OnUpdateButtonF6(CCmdUI *pCmdUI);
	afx_msg void OnButtonF3();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnUpdateButtonF5(CCmdUI *pCmdUI);
	afx_msg void OnButtonF5();
	afx_msg void OnButtonF4();

	LRESULT OnSelGuideMsg(WPARAM w, LPARAM l );
	LRESULT OnSelPosition(WPARAM w, LPARAM l );
	LRESULT OnDBDinpMsg(WPARAM w, LPARAM l );
	LRESULT OnRGDTsel( WPARAM wp, LPARAM lp );

protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void OnRegdata();
	afx_msg void OnRegdlguse();

	virtual void OnActivateJob(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};

//#ifndef _DEBUG  // DBAbookView.cpp ファイルがデバッグ環境の時使用されます。
//inline CDBAbookDoc* CDBAbookView::GetDocument()
//   { return (CDBAbookDoc*)m_pDocument; }
//#endif

/////////////////////////////////////////////////////////////////////////////

