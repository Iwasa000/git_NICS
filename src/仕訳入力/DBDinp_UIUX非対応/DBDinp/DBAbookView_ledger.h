// DBAbookView.h : CDBAbookViewクラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#include "DataListCtrl.h"

#include "AcbookRec.h"
#include "DBbaseView.h"

// データ移動サイン

#define	ACD_SCROLLUP	-1
#define	ACD_SCROLLDOWN	-2
#define	ACD_PAGEUP		-3
#define	ACD_PAGEDOWN	-4


#define	ACREC_MAX	40
#define	acINP_mode	*acfnINP_mode()



//{{AFX_INCLUDES()
#include "icssel.h"	
#include "icsdbedt.h"
#include "icsdisp.h"
//}}AFX_INCLUDES



typedef CArray<AC_SZAN, AC_SZAN&>	CSZanArray;



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
	int		m_TopIndex;
	int		bINP_mode;

	int		m_NowPN, m_NowLine;
	int		m_LASTSEQ;
	int		m_TKsyz, m_TKsyz2;
	int		m_InsLine;		// 挿入時のリスト行番号(１から始まる)
	BOOL	m_bAutoSwk;		// 自動仕訳モード
	int		m_NOBRNCHK_CND;
	BOOL	m_bIgnoreDateTerm;
	CSZanArray	m_SZan;		// 差引残高配列

	ACBOOK_LINE	m_ALine[ACREC_MAX];		// 画面上に表示している 仕訳行

	CDBipDataRec	CREC, SREC, SCAN_rec;

	CString m_BaseCode;
	int		m_BaseEda;
	int		m_BaseBmn;
	CString m_BaseKoji;

	int		m_BaseDate;
	int		m_BaseOfst;
	int		m_BaseKsign;

	void	OnSetFocusBaseBumon();
	void	OnKillFocusBaseBumon(long inplen);
	void	OnTerminationBaseBumon(long nChar, long inplen, long kst);
	void	OnSetFocusBaseKoji();
	void	OnKillFocusBaseKoji(long inplen);
	void	OnTerminationBaseKoji(long nChar, long inplen, long kst);
	void	OnSetFocusBaseKamok();
	void	OnKillFocusBaseKamok(long inplen);
	void	OnTerminationBaseKamok(long nChar, long inplen, long kst);
	void	OnImeEndCompositionBaseKamok(short nChar, LPCTSTR string, LPCTSTR ystring);
	long	CheckDataBaseKamoku( long nChar, LPCTSTR string );

	void	OnSetFocusBaseBrn();
	void	OnKillFocusBaseBrn(long inplen);
	void	OnTerminationBaseBrn(long nChar, long inplen, long kst);

	int  TopDataSet();
	int  InitDataSet();
	int  DataAppendReset( CDBipDataRec* pcrec );

	void ListDataSet();
	void BaseWndMove();
	void acline_init( ACBOOK_LINE* aline, int size );
	void acline_set( ACBOOK_LINE* aline, int idx, int abpos );
	
	BOOL syzvol_kmkchk( CString& dbt, CString& cre );

	int acline_scrolup();
	int acline_scroldown();
	int acline_pageup();
	int acline_pagedown();
	BOOL PageMoveDataUpdate(int nowln );

	void ListClmSet( int wd );
	void DBeditCtrlSet( int id_pn );

	void set_nowpn( int pn );
	int	 get_nowpn();
	void set_nowln( int ln );
	int	 get_nowln();
	BOOL isdeldata();
	BOOL _isdeldata( int ln );

	int* acfnINP_mode();
	int  DB_SyIniz( CDBipDataRec* data );

	BOOL modefy_ok( CDBipDataRec *drec  );
	BOOL _modefy_ok( int darec_seq );

	ACBOOK_LINE* getLINE_DATA();
	ACBOOK_LINE* _getLINE_DATA( int ln );

	char *sy_chk_txt( CDBipDataRec *data,  struct _SY_MSG_PACK* smp, int inp_mode );
	char *sy_line_txt( CDBipDataRec *data );

	int ChangeDebtCred();
	int SzanUpdate( int ln, char* nowval, char* oldval, int amsgn );
	void SzanDataSet( int ln );
	int WriteAndNextData();
	void AutoSiwake( int ln, BOOL bAuto );

	BOOL brnitm_to_tky( int ln, int bmn, CString& code, int brn );

	void set_focus( int pn, int iClick = 0 );
	void _set_focus( int ln, int pn, int iClick = 0 );
	void set_termfocus( int ln, int pn, int vect_n );

	int get_dctrl_id( int pn );
	int dctrlid_toacpn( int id );
	
	void disp_change();
	void set_etcdsp( int ln, int pn );

	int _dline_chg( int ln, int pn, int focs_sw = 0 );
	int dline_chg( long nChar );

	void LineDataClear( int ln );
	BOOL AppendDataCheck( int ln );

	int  RequeryACLine( int nxtseq, int ln );
	int	 InsertSiwake( int ln );
	int  BackNormalAppendMode( int ln );

	void Destroy_ModeLessDlg();

	long CheckDataIcsDate( long nChar );
	void FocusIcsDate( int ln, short click );
	void TerminationIcsDate( long nChar, long inplen, long kst );
	void DateInputChange();
	BOOL AcbookDateCheck( BYTE* bcddate );
	void acbook_datadelete();

	void FocusIcsBumon( int ln, short click );
	void TerminationBumon( int ln, long nChar, long inplen, long kst );

	void FocusIcsKoji( int ln, short click );
	void TerminationKoji( int ln, long nChar, long inplen, long kst );

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

	int  tky_select_to_drec( CDBipDataRec *CREC, CDBipTKREC *ptk );
	void tky_select_dsp( CDBipDataRec *CREC, int dc_sw, CDBipTKREC *ptk, BOOL F12_KEY, BOOL bTkcut = FALSE );
	int  tkybrn_search( CDBipDataRec *data, int tkcd, ABLN_CND* cnd );

	void FocusIcsSyz( int ln, short click );
	int  TerminationSyz( int ln, long nChar, long inplen, long kst );

	int  SyzInpDisp();
	void Hide_SyzInp();

	void TerminationDataSet( int pn, int line = -1 );
	void TerminationDataSet( int pn[], int pncnt, int line = -1 );
	void _TerminationDataSet( DTLIST_DATA* data, ACBOOK_LINE *dtline, int pntbl[], int tblcnt );

	void _OnTerminationIcsdbedtctrl( long nChar, long inplen, long kst );

	void SelectReset();
	void SelectedIcsselctrl(short position, long Number, int place ) ;

	BOOL base_selected( int pn, int Number );
	BOOL check_basebrn();

	int		set_nonICSpn( tagMSG* pMsg );
	long	OnTermBaseDispBtn( long nChar );

	int		Abook_Requery();

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
	afx_msg void OnKanaTerminationIcsdbedtctrl1(LPCTSTR Kana);
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

//注意！これより以下の7行は変更してはいけません
private:
	BOOL	m_checkfirst;
	BOOL&	FirstTime() { return	m_checkfirst; }
public:
	void	BeginApp();
	BOOL	EndApp( WPARAM wParam = 0, BOOL bCosel = FALSE );
//	void	CalcMinMax( MINMAXINFO* info ) { ICSResizeSub::CalcMinMax( info ); }
public:
	afx_msg void OnButtonF9();
	afx_msg void OnButtonF8();
	afx_msg void OnButtonF7();

	void SelectedIcsselctrl1(short position, long Number);
	void SelectedIcsselctrl2(short position, long Number);
public:
public:
public:
	afx_msg void OnButtonF12();
public:
	afx_msg void OnBnClickedButtonDisp();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

//#ifndef _DEBUG  // DBAbookView.cpp ファイルがデバッグ環境の時使用されます。
//inline CDBAbookDoc* CDBAbookView::GetDocument()
//   { return (CDBAbookDoc*)m_pDocument; }
//#endif

/////////////////////////////////////////////////////////////////////////////

