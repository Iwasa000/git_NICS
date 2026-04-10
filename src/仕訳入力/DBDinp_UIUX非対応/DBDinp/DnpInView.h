#pragma once

#include "DenpRec.h"
#include "DBBaseView.h"
#include "FuriDlg.h"
#include "NSDenpDlg.h"
#include "DenpScan.h"

// CDnpInView フォーム ビュー


#define EXPAN_MAX	(DP_SCROLL_NL * 2 * 3)



class CDnpInView : public CDBbaseView
{
	DECLARE_DYNCREATE(CDnpInView)

protected:
	CDnpInView();           // 動的生成で使用される protected コンストラクタ
	virtual ~CDnpInView();

	ICSDateSelect m_selDate;

public:
	enum { IDD = IDD_DBDNPINVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	int			m_DENP_PN, m_DPDATA_LN;

	CFuriDlg*	m_pFuri;
	CNSDenpDlg*	m_pNSdlg;

	int			m_NOW_PN;

	BOOL		m_bIgnoreFocus;
	int			m_FuriMode;
	BOOL		m_bInitFlg;

	int			m_InsideCno;	// 内部伝票番号

	BOOL		m_bSyzYunyu[2];	// 輸入仕入れに今回変更したか？サイン

	int			m_TopIdx;		// 画面上で表示しているデータの 

	BOOL		m_bDispLno;
	BOOL		m_bDLineMMinpIgnore;
	BOOL		m_bDLineChanging;

	BOOL		m_syogMode;		// 諸口を詰めないモード

	BOOL		m_bDenpChkMsg;	// 異なる伝票表示中メッセージ

private:
	// 現在処理行の 消費税モジュールのための仕訳データ
	CDBINPDataRec	m_CREC[2];
	// 仕訳書き込み時に、展開する仕訳データ
	CDBINPDataRec	EXPAN_TBL[ EXPAN_MAX ];

	CDenpScan		m_DPscan;
	CString			RequeryStr;

	int		m_SYdataCnt;	// m_CREC の有効データ数
	int		KindInput;
	int		SiwakeType;
	int		SiwakeMode;
	int		JobMode;
	int		DefaultType;
	int		nSknBrnSign;
	int		nDenpNoChk;
	int		nBmnClr;

	unsigned char	kariTotal[6], kasiTotal[6];		//検索時の貸借金額
	// 摘要
	unsigned char	tblImage[DP_SCROLL_MAX][2];		//イメージ摘要サイン
	unsigned int	tblImageOfst[DP_SCROLL_MAX][2];	//イメージ摘要オフセット
	CString			tblTekiyo[DP_SCROLL_MAX];		//複合仕訳の貸方摘要
	int				tblDel[DP_SCROLL_MAX];			// 取消し仕訳ライン

	// 複合仕訳で、借方／貸方 異なる場合がある項目を保存
	// 手形番号
	CString			tblTgnum[DP_SCROLL_MAX][2];
	// 手形期日
	int				tblTgdate[DP_SCROLL_MAX][2];
	// 変動事由
	int				tblHjiyu[DP_SCROLL_MAX][2];
	// 付箋
	int				tblHusen[DP_SCROLL_MAX][2];
	CString			tblHcmnt[DP_SCROLL_MAX][2];

	BOOL	bImgStaticUpdate;

	CRect	m_FuriRect, m_NSRect;

	BOOL	m_bLineCopy, m_bLinePaste;
	int		m_nCopyLine;

	BOOL	m_bRDchkVal;
	BOOL	m_bConfirmData;

	map<CString, int>	edamap1;
	map<CString, int>	edamap2;
	//摘要ICSDBEDIT 保存
	UINT	m_tekiEdtID;
	// 摘要複数自動仕訳
	CDBipTKREC	m_autoTKrec;

	BOOL	m_bAutoLoop;
	int		m_AutoCnt;

public:
	int&	IsKind() { return	KindInput; }
	int&	IsType() { return	SiwakeType; }
	int&	IsMode() { return	SiwakeMode; }
	int&	IsJob() { return JobMode; }
	int&	IsDefaultType() { return DefaultType; }

	int&	SknBrnSign() { return nSknBrnSign; }
	int&	DenpNoCheck() {	return nDenpNoChk; }
	int&	BmnClrCheck() { return nBmnClr; }

	int		GetDelSgn(int ln );
	void	SetDelSgn(int ln, BOOL sw );
	void	InsDelSgn( int ln );
	void	delLineDelSgn(int ln);

	int		get_delpn();
	void	get_hjiyustr( int hjiyu, CString* str );
	void	set_bmonstring( CString& str , int bmn );

	LPCTSTR get_hjnptitle();
	LPCTSTR GNKINcode();	// 09.09 /08

	BOOL	IsFuriSyogBrn();
	BOOL	IsZeigkMdfy();
	void	diagctrl_job();

	int		GetDnpTopIndex() { return m_TopIdx; };
	int		IsInputConfirmMaster() { return m_bConfirmData; };

	int		GetFuriMode() { return m_FuriMode; };
	BOOL&	SyogSiwakeMode() {	return m_syogMode; } //TRUE:諸口仕訳詰めない, FALSE:諸口仕訳詰める
	int		FuriSyogPosition( int ln, int pn );

	BOOL	IsNotFuriTanitsu( int ln );
	int		ModifyCheckBmnNoInp( CDBINPDataRec* drec, int cnt, int& chk_ln, int& chk_pn, BOOL bMsg );
	void	LastDenpNoSet();
	void	LastSyoNumSet();
	void	DenpDlgUpdateWindow();

	BOOL	IsKamokuSyattr(int ln, int pn);
	BOOL	IsSameFuriSyzKbn(int ln);

public:
	void	BeginApp();
	BOOL	EndApp( WPARAM wParam = 0, BOOL bCosel = FALSE );
	BOOL	EndChk(int sgn = 0);

	void	OnButtonEnd_Scan();

	void	SelectDisp( int ln, int dnp_pn );
	void	set_nowpn(int pn);
	void	set_nowln(int ln);
	int		get_nowln();

	int		get_dataline();
	int		get_dataline( int ln );

	void	denp_init_rd();
	void	ChangeDnpView();
	void	DispBaseKmk();

	virtual void LastDataGet();
	virtual void ForkUcomSet();
	virtual int  ReloadView();
	virtual void ReCoselSetFocus( BOOL bNewCo );

	virtual void SoundBuzzer() { Buzzer(); };
	virtual BOOL EnableCallPro();
	virtual void Destroy_SyzInpSubFunc();

	void	CancelInvNoIME();

	BOOL	FuriModeChange( int mode );
	BOOL	IsTanBmnEnable();
	void	DispBmnKojiData( int mode );

	void	ToViewDelFocusJob( int ln, int pn );
	int		ToViewFocusJob( int ln, int pn, CRect rect );
	int		ToViewTermJob( int ln, int pn, long nChar );
	void	ToViewKanaTerm( LPCTSTR data );
	BOOL	ToViewAccelerator( UINT ID, long nChar );
	long	ToViewSelectTerm( int pn, long nChar );

	int		ToViewUpsideTermJob( int pn, long nChar );

	BOOL	IsReadOnly( UINT id );
	BOOL	IsDelete( UINT id );

	BOOL	IsBillInput( int line = -1 );
	BOOL	IsHjiyuInput( int line = -1 );
	BOOL	IsInputCharEnable();

	void	ToViewImageStaticKey( UINT id, UINT nChar );
	void	ToViewImageStaticFocus( UINT id );

	void	OnTermDebtVal( int ln, long nChar );
	void	OnTermDebtZei( int ln, long nChar );
	void	OnTermDebtBmn( int ln, long nChar );
	void	OnTermDebtKoji( int ln, long nChar );
	void	OnTermDebt( int ln, long nChar );
	void	OnTermCredVal( int ln, long nChar );
	void	OnTermCredZei( int ln, long nChar );
	void	OnTermCredBmn( int ln, long nChar );
	void	OnTermCredKoji( int ln, long nChar );
	void	OnTermCred( int ln, long nChar );
	void	OnTermTky( int ln, long nChar );
	void	OnTermTgNum( int ln, long nChar );
	void	OnTermTgDate( int ln, long nChar );
	void	OnTermHjiyu( int ln, long nChar );
	void	OnTermDsyog( int ln, long nChar );
	void	OnTermCsyog( int ln, long nChar );

	void	kesyu_chkdsp();
	void	lineN_dsp();
	void	inpdata_dsp();

	void	setDATA_LINE_COLOR( int ln, COLORREF col, BOOL fore );
	COLORREF	GetConfirmBackColor( char sgn );
	COLORREF	GetLineBackColor( int dt_ln );

	DNP_POS* GetDNP_POS( int ln, int pn );

	BOOL	CheckKamoku( long nChar, LPCTSTR string, struct _KamokuData* pKmdata );

	int		GetBmnCode(int ln, int pn);
	long	ItemInp( int ln, int pn, long nChar, struct _KamokuData* pKm );
	int		ItemSet( int ln, int pn, long nChar, struct _KamokuData* pKd, BOOL eda = FALSE );
	int		ItemDel( int ln, int pn );
	void	ItemCopy( int ln, int pn );
	int		SyzDel( int ln, int pn );
	void	KnamRedisp( int ln, int pn );

	BOOL	CheckSyog( long nChar, CString& edit );
	long	SyogInp( int ln, int pn, long nChar, int *syogbrn );
	BOOL	FukugoLine( int dcsw );

	long	DenpInp(int pn, long nChar, int denp);
	void	DenpSet( int pn, int denp );
	void	DenpDel( int pn );

	long	MM_Inp( int pn, long nChar, BYTE mnth );
	void	MM_Set( int pn, BYTE mnth );
	long	DD_Inp( int pn, long nChar, BYTE dd );
	void	DD_Set( int pn, BYTE dd );

	int		CheckImeEndBmon( long nChar, LPCTSTR string );
	void	TerminationBmonDBedit( UINT id, long nChar, long inplen, long kst );

	int		Bmn_Check( int pn, long& nChar, int *bmn, CString* edit );
	long	Bmn_Inp( int pn, long nChar, int *bmn );
	void	Bmn_Copy( int ln, int pn );

	BOOL	Koji_Check( int pn, long nChar, CString* kno, CString* edit );
//	long	Koji_Inp( int pn, long nChar, CString* kno );
//	void	Koji_Copy( int ln, int pn );
	void	Koji_Copy( int ln, int pn, CString& kno );
	void	TerminationKojiDBedit( UINT id, long nChar, long inplen, long kst );
	int		CheckImeEndKoji( long nChar, LPCTSTR string );
	void	bmnkoji_to_tekiyo( CString* kjnam, int type = 0 );

	BOOL	MM_Check( int pn, int mm, BYTE* pBcd );
	BOOL	DD_Check( int pn, BYTE dd, BYTE* pBcd );

	long	ValInp(int ln, int pn, long nChar, long inplen, char* val );
	long	ZeiInp(int ln, int pn, long nChar, long inplen, char* val );

	BOOL	IsSameValue( int ln, int pn );
	void	SetTandokuSameValue( int ln, int pn );

	void	TgNumInp( int ln, int pn, long nChar, long inplen, CString& data );
	void	TerminationTgNumDBedit( UINT id, long nChar, long inplen, long kst );
	void	TgDateInp( int ln, int pn, long nChar, long inplen, char* tgdate );

	void	ImeEndJob(UINT ID, long nChar, LPCTSTR string, LPCTSTR ystring);
	void	TerminationTekiyo( UINT id, long nChar, long inplen, long kst );
	BOOL	TekiyoCopy( int ln, CString& tekiyo );

	struct _DP_DATA_LINE *getLINE_DATA( int line );
	struct _DenpData *_getPRDATA( int ln, int pn );
	struct _DenpData *getDATA( int pn );
	struct _DenpData *_getDATA( int ln, int pn );
	struct _DenpData *_getDATAs( struct _DP_DATA_LINE *pd, int pn );
	struct _DNPLINE_CND *getCND( int pn );
	struct _DNPLINE_CND *_getCND( int ln, int pn );
	struct _DNPLINE_CND *_getCNDs( struct _DP_DATA_LINE *pd, int pn );

	int		TabClickChangedPrevView();
	int		TabLbuttonDownJob();

	int		set_position( int ln, int pn, int vect = 1 );

	BOOL	bmncode_todata2( CDBINPDataRec* data, int number, int dc_sw );
	int		CheckKamokuInput( struct _KamokuData* data );
	int		CheckBumonInput( int bmn );
	int		CheckAppendBumon( CDBINPDataRec* rec );

	int		bmnmsg_make( CString& msg, int dc_sw, CDBINPDataRec* drec );
	int		ModifyCheckBmnHani( CDBINPDataRec* drec, int cnt, int& chk_ln, int& chk_pn, BOOL bMsg );
	int		ModifyCheckBmnTorok( CDBINPDataRec* drec, int cnt, int& chk_ln, int& chk_pn, BOOL bMsg );

	void	sheet_clrRCO( int mode = 0 );
	int		Vs_Linemove( int direction );
	int		NoinpScrolChk( int chk_ln, int chk_pn );

	int		ImgTekiyoSet( CDnpBaseDlg* pDlg, int dt_ln, int ln );
	void	TkyEda_Redisp(int dc_sw, struct _DP_DATA_LINE *pdat);
	int		GetAutoSwkType();

	void	TkyDBedtEnable(BOOL bEnable);
	int		CheckKojiKmk( int& chk_ln, int& chk_pn );
	int		CheckAppendKojiKmk( CDBINPDataRec* rec );
	int		kojimsg_make( CString& msg, int dc_sw, CDBINPDataRec* drec );
	int		ModifyCheckKojikmk( CDBINPDataRec* drec, int cnt, int& chk_ln, int& chk_pn, BOOL bMsg );

	int		ModifyCheckKoji( CDBINPDataRec* drec, int cnt, int& chk_ln, int& chk_pn, BOOL bMsg );

	int		ScanDenpDel();
	int		edabanmsg_make( CString& msg, int dc_sw, CDBINPDataRec* drec, int ln );
	void	edabanmsg_addjob( CString& addmsg, CString edamsg, int nMsg, int& MaxCnt, const int MsgMax );
	int		ModifyCheckEdabanInput( CDBINPDataRec* drec, int cnt, int& chk_ln, int& chk_pn, BOOL bMsg );

	BOOL	ButtonInsertJob();
	BOOL	ButtonDeleteJob();

	BOOL	MouseHookRbuttonJob(MOUSEHOOKSTRUCT *pmh);
	void	OnLineCopy( int ln );
	void	OnLinePaste( int ln );
	void	OnLineDelete( int ln );
	void	LineCopyMsgJob( MSG* pMsg );

	void	BmnIMEkeydownJob( long nChar );
	BOOL	BmnKojiIMEcheck();

	BOOL	IsTransDenpyo();
	void	LineCopyClear();

	int		check_dnplndata();
	int		InpDataToRDDnp( int ln, CRDNPdata* prd );

	BOOL	GetBmnEtc( struct _SET_KMKETC* psk );
	BOOL	GetKojiEtc( struct _SET_KMKETC* psk );

	int		get_MaxLine();
	int		get_MaxLineEX();
	int		setDATE_CREC( CDBINPDataRec *pdata );

	int		RDDnpUpdateJob( CString label, int dtype, BOOL bOverWrite, BOOL bLabelOnly );
	void	RDDnpSetChkVal( BOOL bChkVal ) {	m_bRDchkVal = bChkVal;	};
	void	RDDnpCleardsign( CByteArray&	baDsign );

	int		EndContorlJob();
	LRESULT OnDenpyoMsg(WPARAM wp, LPARAM lp );

	LPCTSTR sy_chk_txt( CDBINPDataRec *data, struct _SY_MSG_PACK2* smp );
	BOOL	ChangeZeiritsu();
	BOOL	ChangeSotoMenZei();

	void	ToolbarTekiyoDisp(int dbeditid);
	void	bumon_syzcheck(int inpdt_ln, int pn, int bmn);
	void	TerminationSyoBrnDBedit(UINT id, long nChar, long inplen, long kst);
	void	TerminationSyoNumDBedit(UINT id, long nChar, long inplen, long kst);
	void	OnTermSyoNum(int ln, long nChar);

	void	TerminationKmkinp(UINT id, long nChar, long inplen, long kst);
	int		CheckImeEndKamoku(long nChar, LPCTSTR string);
	HWND	GetKamokuCtrlHWND();
	BOOL	IsKamokuPosition();

	void	LastDenpBmnSet();
	int		m_DenpBmn;
	CString m_DenpKno;

protected:
	void	DenpDlgDisp( int type );
	void	sheet_update();

	void	set_focus( int pn );
	void	_set_focus( int ln, int pn );

	void	get_linekamokucode( int ln, CString& dbt, CString& cre );
	void	get_linekamokucodeEX( int ln, CString& dbt, CString& cre, int& dbtln, int& creln );

	int		get_nowpn();
	int		get_dcsgn();
	int		_get_dcsgn( int pn );

	void	disp_change();
	void	_disp_change();

	void	KamokuUpdate( int ln, int pn );
	void	BmonUpdate( int ln, int pn );
	void	KojiUpdate( int ln, int pn );

	void	Lineattr_dsp();
	BOOL	brnitm_to_tky( int ln, int bmn, CString& code, int brn );

	BOOL	IsScanLine( int dt_ln );
	BOOL	IsAutoSiwake( int line );
	void	MakeAutoSiwake( CDBipTKREC* tkrec );

	void	ValSet(int ln, int pn, long inplen, char* val );
	long	ValDel(int ln, int pn );
	void	ValCopy( int ln, int pn );

	void	ZeiSet(int ln, int pn, long inplen, char* val );
	long	ZeiDel(int ln, int pn );

	int		OnFocusJob( int ln, int pn, CRect& rect );
	int		LineChgJob( CDBINPDataRec *CREC, int ln, int pn );
	void	DnpSyzInit( CDBINPDataRec *data, int dcnt, int nowpn, int prevpn );
	BOOL	IsDebtPosition( int pn );
	BOOL	IsCredPosition( int pn );
	void	SetTandokuVal( int ln, int pn );;

	int		FocusSkbn();
	long	TerminationSyz( long nChar );
	int		SkbnInDisp( int ln, int pn, CRect rect );

	void	HjiyuInpDisp( CRect rect );
	int		FocusHjiyu( int ln, CRect rect );
	void	hjiyu_to_tekiyo( CString* jiyu );
	void	_hjiyu_selectsub( int number, int selclick, CString* pStr );

	void	HusenInpDisp( CRect rect );
	int		FocusHusen( int ln, CRect rect );
	void	HusenDataSet( int ln );	

	int		mkSY_CREC( CDBINPDataRec *pdata, int ln, int lntype = 0, int mkauto =0);
	int		setSY_CREC( CDBINPDataRec *pdata, int dcsw, int ln, int lntype = 0 );

	void	PutsSyz( int ln, int pn, char* syztxt, char sy_chg );

	void	_KanaTermIcsinputTky(LPCTSTR data, BOOL bTkcut);
	int		tekiyo_selset( char *data, BOOL bTKcut, int kanaterm = 0 );
	void	tekiyo_selset_subfunc(int dt_ln, CDBipTKREC* ptkrec, BOOL bTKcut);
	int		tky_select_to_drec( CDBINPDataRec *drec, CDBipTKREC *ptk, char *dbr_inptype, char *cbr_inptype, int dataLn =0 );
	void	tky_select_dsp( CDBINPDataRec *drec, struct _DP_DATA_LINE *pdat, int dc_sw, CDBipTKREC *ptk, BOOL bTKcut );
	void	set_skbn_selsw( CDBINPDataRec* pdata, struct _DenpData* pkey, int zeipn );
	void	SyTkrecMenCheck(_SY_TKREC_ *psyTkRec, CDBINPDataRec* data);

	void	swktekiyo_dspsub(CString tky, int dbedt);

	void	OnButtonChangeKind( UINT kind, BOOL bNoCheck = FALSE );
	void	ChangeDenpyo();
	void	ChangeKind( int nkind );
	void	ChangeType( int type, BOOL bLnChg = FALSE );
	void	ChangeJob( int mode );

	void	inzMAP();
	int		cvtMAPdb( int pn );
	void	setMAP( int pn );
	void	_setMAP( int ln, int pn );
	int		getMAP( int pn );
	int		_getMAP( int ln, int pn );
	void	delMAP( int pn );
	void	_delMAP( int ln, int pn );
	struct _DenpData *getMAPtoDATA( int pn );
	struct _DenpData *_getMAPtoDATA( int ln, int pn );

	void	_insMAP( int line );
	void	_delLineMAP( int line );

	void	inpctl_inz();
	void	inpdata_inz();

	int		DateCheck_and_action( BOOL bMsg = TRUE );
	int		DateCheck();

	void	date_syzcheck( CDBINPDataRec* prec );

	void	motikosi_inz( int motikosi );
	void	total_inz( int mode = SHEET_ALL );

	void	sheet_clr( int mode = SHEET_ALL );
	BOOL	is_lastdenp_contents();
	BOOL	is_line_contents( int ln, int max_pn );

	void	syogdsp();
	void	syog_dsp( char *buf );
	void	syog_clr();
	BOOL	syog_next();

	void	total_clq();
	int		total_chk();
	void	total_dsp();

	int		daexpan( CDBINPDataRec *dtbl );
	int		da_make( int ln, CDBINPDataRec *dtbl );
	int		siwake_mak( struct _DP_DATA_LINE *datp, CDBINPDataRec *dtbl0, int sgn );
	int		sydata_mak( CDBINPDataRec *dtbl0,  CDBINPDataRec *wktbl, int wktbl_n, struct _DP_DATA_LINE *datp );
	
	BOOL	DaWrite();
	int		AppendData( CDBINPDataRec *dat, int max, ADDNEWTBL* antbl, int& chg_denpno);
	int		CheckEdabanInput( int& chk_ln, int& chk_pn );

	void	OnButtonF4_Scan();
	void	OnButtonF11_Scan();

	void	FukugoToTandoku();

	void	ProfileSet( BOOL sw );
	CDnpBaseDlg* GetNowDnpDialog();

	void	SelectedIcsselctrl(short position, long Number, int place );

	int		getSCROLL_NL();
	void	tkyexpan( int ln, CRect* rc, BOOL exp_sw, BOOL bOnSize = FALSE );				// ln = 該当ライン
	void	kjedit_expan( int ln, int pn, CRect* rc, BOOL exp_sw, BOOL bOnSize = FALSE );
	void	tgnum_expan( int ln, CRect* rc, BOOL exp_sw, BOOL bOnSize = FALSE );
	void	bmnedit_expan( int ln, int pn, CRect* rc, BOOL exp_sw, BOOL bOnSize = FALSE );
	void	syobrnedit_expan(int ln, int pn, CRect* rc, BOOL exp_sw, BOOL bOnSize = FALSE);
	void	syonum_expan(int ln, CRect* rc, BOOL exp_sw, BOOL bOnSize = FALSE);
	void	kamoku_expan(int ln, int pn, CRect* rc, BOOL exp_sw, BOOL bOnSize = FALSE );

	int		DnpLine_Chg( int vect, int nofocus = 0 );

	void	dbedt_putback();
	int		CheckDenpyoData( int vect );
	int		CheckFuriBmnKoji();
	int		ScanNextPage();
	int		ScanPrevPage();

	int		ScanCheck_NextPage();
	int		ScanCheck_PrevPage();

	void	SetConfirmString( CString& msg );

	int		EndScan( BOOL bTabClick = FALSE, BOOL bUpdate = TRUE );
	void	DisplayScanData( int mode = 0 );
	void	SetScanData( CDBINPDataRec& drec, int idx, int karikasi );

	void	ScanTotalClc( unsigned char* kari, unsigned char* kasi );
	int		ScanTotalCheck( int mode, BOOL bMsg = TRUE );
	void	ScanAllTotalClc();
	void	ScanDeleteLine();

	BOOL	IsTekiyoImage( int ln );
	void	SetFocusToImage( int ln, int sgn );
	void	KasiImageDisplay( int ln, BOOL sgn );
	void	ImagePaintSw( int ln, int cnt, int sgn );
	void	ImageBackColor( int ln, BOOL sgn );

	void	KasiTekiyoDisplay( BOOL sgn );

//	void	TransLine( int seq, int ln );
	void	TransLineSign( CDBINPDataRec* pdt, int ln );
	void	TransLine( int dt_ln, int ln, char* sy_chg );
//	void	TransLine( CDBINPDataRec* pdt, int ln );
	BOOL	IsTransLine( int ln, char& edt_sg );

	void	lastinput_get();
	int		UpdatePage( int mode );
	int		MakeNewData( int mode, int ln, CDBINPDataRec* drec );
	void	ModifyTaisyakuData( int mode, int ln, int& newcnt, CDBINPDataRec* newrec, int& lcnt, CDBINPDataRec* drec );
	void	ModifyDebtData( int mode, int ln, int& newcnt, CDBINPDataRec* newrec, int& lcnt, CDBINPDataRec* drec );
	void	ModifyCredData( int mode, int ln, int& newcnt, CDBINPDataRec* newrec, int& lcnt, CDBINPDataRec* drec );

	int		ModifyData( CDBINPDataRec* drec, int cnt );

	int		CheckKOJI( int& chk_ln, int& chk_pn );
	int		CheckBmnHani( int& chk_ln, int& chk_pn );

	int		CheckBmnNoInp( int& chk_ln, int& chk_pn );
	int		CheckBmnTorok( int& chk_ln, int& chk_pn );
	int		BmnTorokMessage( int bmn, vector<int>&	bmnvec, BOOL bMsg = TRUE );

	BOOL	KojiTorokMessage( CString koji, vector<CString>&	kojivec, BOOL bMsg = TRUE );
	int		CheckKojiTorok( int& chk_ln, int& chk_pn );
	int		ModifyCheckKojiTorok( CDBINPDataRec* drec, int cnt, int& chk_ln, int& chk_pn, BOOL bMsg );

	int		AutoSiwakeCheck(int ln);

	void	data1line_clear( int ln );
	int		dnp_insert_job( int ln );
	void	OnButtonInsert();
	void	OnButtonDelete();

	void	RDDnpJob();
	int		RDDnpToInpData( int ln, CRDNPdata* prd );
	int		RDDnpBmonKojiCheck( CRDNPdata* prd );
	void	RDDnpChkdsign( int ddate, CRDNPdata* prd );

	int		TaikaInputCheck( int ln, int syzpn );

	void	FuriHendJiyuCheck(  CDBINPDataRec *pcrec, int dcnt, int dt_ln, int clear = 0 );
	void	HendJiyuCheck( CDBINPDataRec *pcrec, int dt_ln, int clear = 0 );
	BOOL	IsUsedDenpNo( int inpcno );
	void	TkyInpEndUpdate();

	int		mdfy_bmn_noinp_check( int bmn, CString& code, int dbcr_sg, CString* pMsg );

	// datarec cno, seq のマップ
	std::map<int, int>	m_cnoMap;

	int		GetDenpyoNo(int denpno);
	BOOL	tekiyp_F2job();
	void	bmneda_update(int ln, int pn);

	void	TKsgnToSYDATA(struct __SYDATA*, CDBipTKREC* ptk);
	void	CopyTKsgnSYDATA(struct __SYDATA* dst, struct __SYDATA* src);
	BOOL	IsTekiyoSyzChg(struct __SYDATA* syd);
	void	SetSyTkrec(_SY_TKREC_ *psyTkRec, struct __SYDATA* syd);

	void	FocusCountClear(int line);
	void	set_sotomen_sign(int dt_ln, int pn, int sotomen);

	int		edaLine, edaPn;
	void	_DSGN_MenWariFnc(struct _DenpData *keybuf);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()

	LRESULT OnRetroFocus( WPARAM wParam, LPARAM lParam );
	LRESULT OnRetroFocus2( WPARAM wParam, LPARAM lParam );
	LRESULT SetSkbnLineDisp( WPARAM wParam, LPARAM lParam );
	LRESULT OnSkbnEnd( WPARAM wParam, LPARAM lParam );
	LRESULT DspSkbnIchiran( WPARAM wParam, LPARAM lParam );
	LRESULT OnHjiyuInEnd( WPARAM w, LPARAM l );
	LRESULT OnHjiyuSel( WPARAM w, LPARAM l );
	LRESULT OnHusenInpMsg( WPARAM w, LPARAM l );
	LRESULT OnDataMax( WPARAM w, LPARAM l );
	LRESULT OnDateSelectMessageChangeMNTHSEL(WPARAM wParam, LPARAM lParam);
	LRESULT OnDBDinpMsg(WPARAM wp, LPARAM lp);

public:
	DECLARE_EVENTSINK_MAP()
public:
	void SelectedIcsselctrl1(short position, long Number);
	void SelectedIcsselctrl2(short position, long Number);

public:
	afx_msg void OnButtonF9();
	afx_msg void OnUpdateButtonF9(CCmdUI *pCmdUI);
	afx_msg void OnInputtype();

	afx_msg void OnButtonF1();
	afx_msg void OnButtonF3();
	afx_msg void OnButtonF4();
	afx_msg void OnButtonF5();
	afx_msg void OnButtonF6();
	afx_msg void OnButtonF7();
	afx_msg void OnButtonF8();
	afx_msg void OnButtonF11();
	afx_msg void OnButtonEnd();
	afx_msg void OnButtonF2();

	afx_msg void OnUpdateButtonF8(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonF5(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonF6(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonF3(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonF4(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonF7(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonF11(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonF2(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonF1(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonInsert(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonDelete(CCmdUI *pCmdUI);

protected:
//	virtual void OnDraw(CDC* /*pDC*/);
//	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnActivateFrame(UINT nState, CFrameWnd* pDeactivateFrame);

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnButtonF12();
	afx_msg void OnUpdateButtonF12(CCmdUI *pCmdUI);
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	LRESULT OnSelGuideMsg(WPARAM w, LPARAM l );
};


