
#pragma once

#define		BVW_SYZ_PN	90
#define		EXPAN_MAX	(DP_SCROLL_NL * 2 * 3)

#include "DenpRec.h"
#include "DenpScan.h"
#include "DnpBaseDlg.h"
#include "BmnChk.h"
//#include "FuriDlg.h"
//#include "NSDenpDlg.h"

class	CFuriDlg;
class	CNSDenpDlg;

class CDnpBaseClass
{
public:
	CDnpBaseClass();
	virtual ~CDnpBaseClass();

public:
	BOOL		m_PRO_OK;
	CWnd*		m_pDnpParent;
	CWnd*		m_pCallParent;
	ICSWinApp*	m_pWinApp;

	CICSSel*	m_pselFix8;
	CICSSel*	m_pselSel32;
	
	DENP_INPUTPAR*	m_pDENPPAR;

	CDInputSub	m_DI;

	CDBZmSub*	m_pZm;
	CDBSyohi*	m_pSy;
	CSelectData		m_Seldata;

	CDinpINI		m_DinpSW;
	CDenpScan		m_DPscan;
	CBmnHaniCheck	m_BmChk;
	CKojiDataCheck	m_KjChk;

	BOOL		BRNTKY_sel;
	BOOL		bKamokuCode;
	int			m_SelDispPN;
	int			m_SET_SYZINP_PN;	// 消費税ダイアログ表示時に最初にカーソル置いておきたい場所
	char		m_tky_50kana[20];	// 摘要５０音検索用 カナ

	BOOL		m_bNotNumber;

	CSyzInp*	m_pSyzInp;
	CHusenInp*	m_pHusenInp;
	CHjiyuInp*	m_pHjiyuInp;

	struct _AUTOSEL *m_pAsel;

	int			m_DataSeq;	//修正する仕訳番号

	RECT		m_original_rect;
	RECT		m_selfix8_rect;
	RECT		m_valtitle_rect;
	COLORREF	m_vwColor;

	int			m_Errcd;
	CString		m_ErrMsg;

	int			m_TopIdx;
	BOOL		m_bIgnoreFocus;
	BOOL		m_bDispLno;
	BOOL		m_bDLineMMinpIgnore;
	BOOL		m_bDLineChanging;
	int			m_InsideCno;	// 内部伝票番号
	BOOL		m_bSyzYunyu[2];	// 輸入仕入れに今回変更したか？サイン

	int			m_FuriMode;
	BOOL		m_syogMode;

	// 伝票番号・日付 input buff.
	struct _DP_DATE_LINE	DENP_DATE_LINE;
	// 仕訳データ buff.
	struct _DP_DATA_LINE	DNPDT_LINE[ DP_SCROLL_MAX ];
	// 仕訳データマップ
	short DATA_MAP[DP_SCROLL_MAX][2];
	// 借方・貸方合計 buff.
	struct _DenpData DEBT_TOTAL, CRED_TOTAL;
	struct _DNPLINE_CND DEBT_TOTAL_CND, CRED_TOTAL_CND;
	//     持ち越し用　buff.
	struct _DenpData ZDEBT_TOTAL, ZCRED_TOTAL;
	struct _DNPLINE_CND ZDEBT_TOTAL_CND, ZCRED_TOTAL_CND;

	// ダミーSEQ番号
	unsigned long DMY_SEQ;
	unsigned long SEQ_DATA[ DP_SCROLL_MAX ];
	// 諸口貸借
	char SYOG_TAI[6];			// 諸口貸借
	char SHEET_SYOG_TAI[6];		// １シート分諸口貸借

	int		m_SYdataCnt;	// m_CREC の有効データ数
	int		KindInput;
	int		SiwakeType;
	int		SiwakeMode;
	int		JobMode;
	int		DefaultType;
	int		nSknBrnSign;
	BOOL	bImgStaticUpdate;
	int		val_sysflg;
	int		tky_sysflg;
	int		tktab_sysflg;
	int		kana_sysflg;
	int     show_flg;
	int		SyzStop_sysflg;

	// 現在処理行の 消費税モジュールのための仕訳データ
	CDBINPDataRec	m_CREC[2];
	// 仕訳書き込み時に、展開する仕訳データ
	CDBINPDataRec	EXPAN_TBL[ EXPAN_MAX ];

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

	char			saveITEM50kana[16];

	int			m_DENP_PN, m_DPDATA_LN;
	CFuriDlg*	m_pFuri;
	CNSDenpDlg*	m_pNSdlg;
	int			m_NOW_PN;

	BOOL		m_bLineCopy, m_bLinePaste;
	int			m_nCopyLine;

	map<CString, int>	edamap1;
	map<CString, int>	edamap2;

	// 摘要複数自動仕訳
	CDBipTKREC	m_autoTKrec;
	BOOL	m_bAutoLoop;
	int		m_AutoCnt;

	int		edaLine, edaPn;

public:
	static int CALLBACK SetSelectFix8( _SelTable* pSel, SelSetProcPar* pSetProc, int cnt, CWnd* lp );
	static int CALLBACK SetSelectSel32( _SelTable* pSel, SelSetProcPar* pSetProc, int cnt, CWnd* lp );

	void enable_fix8( BOOL sg );
	void enable_fixval( BOOL sg );

	int  CheckBumonInput( int bmn );

	int  InitWork( BOOL oninitdlg);
	void EndWork();
	void CloseWork();
	void ModelessCloseJob();
	void UpdateSelectPar();
	void SetSelectWindow();
	void ResetSelect();
	BOOL tky_50kana_chk( char *data );
	int  tky_select( CDBipTKREC* ptkrec, char *data);
	void SyzInpMake();
	void SyzInpDelete();
	void Destroy_SyzInp();
	void DispSyzIchiran( WPARAM wParam, LPARAM lParam );
	void HusenInpMake();
	void HusenInpDelete();
	void Destroy_HusenInp();
	void HjiyuInpMake();
	void HjiyuInpDelete();
	void Destroy_HjiyuInp();
	BOOL InputItemCheck(struct _KamokuData& ans, struct _KamokuData& check, int _dcsw, BOOL bSelClick = FALSE );
	BOOL EdabanDispReload( CString& kmkcd );

	BOOL bmncode_todata( CDBINPDataRec* data, int number, int dc_sw, int hchk = 0, CString* bmnname = NULL );
	BOOL kojicode_todata( CDBINPDataRec* data, CString* kjnam, int number, int dc_sw );

	BOOL koji_noinpcheck( CString& kno, CString& code, int dbcr_sg );
	BOOL koji_kanseicheck( CString& kno, CString& code, int dbcr_sg );
	BOOL bmn_noinpcheck( int bmn, CString& code, int dbcr_sg, CString* pMsg = NULL );

	BOOL Kojicode_Ok( CString& kjcode );
	int  bmnzan_count(CDatabase* database, CString& code, CString& errmsg);

	long SelNaibuDsp( long nChar );
	long SelBmonDsp( long nChar );
	long SelKojiDsp( long nChar );
	long SelTkyDsp( long nChar );
	BOOL IsEnableHjiyu( CDBINPDataRec *pcrec );

	BOOL& NotSelNumber() { return m_bNotNumber; };

	void set_valtitle( char *msg );
	void valtitle( BOOL sg );
	void fix8disp( BOOL sg );

	long ErrMsgSet( int ercd, char* msg );

	int		GetDnpTopIndex() { return m_TopIdx; };
	BOOL	IsFuriSyogBrn();
	BOOL	IsZeigkMdfy();
	void	diagctrl_job();
	void	inpctl_inz();
	
	int&	IsKind() { return	KindInput; }
	int&	IsType() { return	SiwakeType; }
	int&	IsMode() { return	SiwakeMode; }
	int&	IsJob() { return JobMode; }
	int&	IsDefaultType() { return DefaultType; }

	int&	SknBrnSign() { return nSknBrnSign; }
	int&	TekiyoKeyMode();
	int&	ValEnterCopyMode();
	int&	TKtabMode();
	int&	SyzStopMode();

	struct _DP_DATA_LINE *getLINE_DATA( int line );
	struct _DenpData *_getPRDATA( int ln, int pn );
	struct _DenpData *getDATA( int pn );
	struct _DenpData *_getDATA( int ln, int pn );
	struct _DenpData *_getDATAs( struct _DP_DATA_LINE *pd, int pn );
	struct _DNPLINE_CND *getCND( int pn );
	struct _DNPLINE_CND *_getCND( int ln, int pn );
	struct _DNPLINE_CND *_getCNDs( struct _DP_DATA_LINE *pd, int pn );

	void	inzMAP();
	int		cvtMAPdb( int pn );
	void	setMAP( int pn );
	void	_setMAP( int line, int pn );
	int		getMAP( int pn );
	int		_getMAP( int ln, int pn );
	void	delMAP( int pn );
	void	_delMAP( int line, int pn );
	struct _DenpData *	getMAPtoDATA( int pn );
	struct _DenpData *	_getMAPtoDATA( int ln, int pn );
	void	_insMAP( int line );
	void	_delLineMAP( int line );

	int		get_MaxLine();
	int		getSCROLL_NL();

	void	get_linekamokucode( int ln, CString& dbt, CString& cre );
	void	get_linekamokucodeEX( int ln, CString& dbt, CString& cre, int& dbtln, int& creln );
	int		get_dataline();
	int		get_dataline( int ln );

	int		ImgTekiyoSet( CDnpBaseDlg* pDlg, int dt_ln, int ln );

	DNP_POS* GetDNP_POS( int ln, int pn );

	BOOL	CheckKamoku( long nChar, LPCTSTR string, struct _KamokuData* pKmdata );

	int		GetBmnCode(int ln, int pn);
	long	ItemInp( int ln, int pn, long nChar, struct _KamokuData* pKm );
	int		ItemSet( int ln, int pn, long nChar, struct _KamokuData* pKd, BOOL eda = FALSE );
	int		ItemDel( int ln, int pn );
	void	ItemCopy( int ln, int pn );
	int		SyzDel( int ln, int pn );
	void	KnamRedisp( int ln, int pn );
	int		KanaSearchKeta();

	int		FukuKanaSearch(MSG* pMsg, int edaonly = 0);
	int		KanaCheckVK_BACK(MSG* pMsg);
	BOOL	IsKamokuPosition();

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
	void	ValSet(int ln, int pn, long inplen, char* val );
	long	ValDel(int ln, int pn );
	void	ValCopy( int ln, int pn );
	BOOL	IsSameValue( int ln, int pn );
	void	SetTandokuSameValue( int ln, int pn );

	long	ZeiInp(int ln, int pn, long nChar, long inplen, char* val );
	void	ZeiSet(int ln, int pn, long inplen, char* val );
	long	ZeiDel(int ln, int pn );

	void	TgNumInp( int ln, int pn, long nChar, long inplen, CString& data );
	void	TerminationTgNumDBedit( UINT id, long nChar, long inplen, long kst );
	void	TgDateInp( int ln, int pn, long nChar, long inplen, char* tgdate );

	void	ImeEndJob(UINT ID, long nChar, LPCTSTR string, LPCTSTR ystring);
	void	TerminationTekiyo( UINT id, long nChar, long inplen, long kst );
	BOOL	TekiyoCopy( int ln, CString& tekiyo );

	void	_KanaTermIcsinputTky(LPCTSTR data, BOOL bTkcut);
	int		tekiyo_selset(char *data, BOOL bTKcut, int kanaterm = 0 );
	void	tky_select_dsp( CDBINPDataRec *drec, struct _DP_DATA_LINE *pdat, int dc_sw, CDBipTKREC *ptk, BOOL bTKcut );
	void	MakeAutoSiwake( CDBipTKREC* tkrec );

	int		GetFuriMode() { return m_FuriMode; };
	BOOL&	SyogSiwakeMode() {	return m_syogMode; }
	int		FuriSyogPosition( int ln, int pn );

	BOOL	IsNotFuriTanitsu( int ln );
	int		mdfy_bmn_noinp_check( int bmn, CString& code, int dbcr_sg, CString* pMsg );
	int		ModifyCheckBmnNoInp( CDBINPDataRec* drec, int cnt, int& chk_ln, int& chk_pn, BOOL bMsg );
	void	LastDenpNoSet();

	int		get_delpn();
	int		get_nowpn();
	void	set_nowpn(int pn);

	int		set_position( int ln, int pn, int vect = 1 );

	void	set_nowln(int ln);
	int		get_nowln();

	int		GetDelSgn(int ln );
	void	SetDelSgn(int ln, BOOL sw );
	void	InsDelSgn( int ln );
	void	delLineDelSgn(int ln);

	int		get_MaxLineEX();
	int		DnpLine_Chg( int vect, int nofocus = 0 );
	int		Vs_Linemove( int direction );
	void	lastinput_get();

	int		daexpan( CDBINPDataRec *dtbl );

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

	BOOL	MouseHookRbuttonJob(MOUSEHOOKSTRUCT *pmh);
	void	OnLineCopy( int ln );
	void	OnLinePaste( int ln );
	void	OnLineDelete( int ln );
	void	LineCopyMsgJob( MSG* pMsg );

	void	data1line_clear( int ln );
	BOOL	ButtonDeleteJob();

	void	BmnIMEkeydownJob( long nChar );
	BOOL	BmnKojiIMEcheck();

	BOOL	IsTransDenpyo();
	void	LineCopyClear();

	void get_hjiyustr( int h_code, CString& str );
	BOOL _get_hjiyustr( int hjcd, int nbcd, CString& str );

	void	KamokuString( DBKNREC* pKn, CString& string );
	void	KamokuString( DBKNREC* pKn, char* buf, int bufsize );

	int		get_apno() { return m_pZm->zvol->apno; };

	BOOL	CheckConfirmBkColor( COLORREF back );

	int		DateCheck_and_action( BOOL bMsg = TRUE );
	int		DateCheck();
	void	date_syzcheck( CDBINPDataRec* prec );

	void	SelectDisp( int dnp_ln, int dnp_pn );
	int		OnFocusJob( int displn, int pn, CRect& rect );
	int		LineChgJob( CDBINPDataRec *data, int ln, int pn );
	int		mkSY_CREC( CDBINPDataRec *pdata, int ln, int lntype = 0, int mkauto = 0);
	void	DnpSyzInit( CDBINPDataRec *data, int dcnt, int nowpn, int prevpn );

	CDnpBaseDlg* GetNowDnpDialog();
	int		SkbnInDisp( int ln, int pn, CRect rect );

	int		FocusHjiyu( int ln, CRect rect );
	void	HjiyuInpDisp( CRect rect );

	int		FocusHusen( int ln, CRect rect );
	void	HusenInpDisp( CRect rect );
	void	HusenDataSet( int ln );
	LRESULT OnHusenInpMsg( WPARAM w, LPARAM l );

	BOOL	IsTekiyoImage( int ln );
	void	SetFocusToImage( int ln, int sgn );
	void	KasiImageDisplay( int ln, BOOL sgn );
	void	ImagePaintSw( int ln, int cnt, int sgn );
	void	ImageBackColor( int ln, BOOL sgn );
	void	KasiTekiyoDisplay( BOOL sgn );

	void	disp_change();
	void	_disp_change();

	void	set_focus( int pn );
	void	_set_focus( int ln, int pn );
	LRESULT OnRetroFocus( WPARAM wParam, LPARAM lParam );
	LRESULT OnRetroFocus2( WPARAM wParam, LPARAM lParam );

	void	syogdsp();
	void	syog_dsp( char *buf );
	void	syog_clr();

	void	total_dsp();
	void	total_clq();
	int		total_chk();

	BOOL	IsAutoSiwake( int line );

	long	SelItemDsp( long nChar );
	int		BRNTKY_select( int bmn, CString& code, char* kana = NULL, BOOL bDisp = FALSE );
	BOOL	IsHjiyuOrNPfuri( LPCTSTR dbt, LPCTSTR cre );

	BOOL	brnitm_to_tky( int ln, int bmn, CString& code, int brn );

	void 	tkyexpan( int ln, CRect* rc, BOOL exp_sw, BOOL bOnSize = FALSE );	// ln = 該当ライン
	void 	kjedit_expan( int ln, int pn, CRect* rc, BOOL exp_sw, BOOL bOnSize = FALSE  );
	void 	tgnum_expan( int ln, CRect* rc, BOOL exp_sw, BOOL bOnSize = FALSE );
	void 	bmnedit_expan( int ln, int pn, CRect* rc, BOOL exp_sw, BOOL bOnSize = FALSE );
	void	syonum_expan(int ln, CRect* rc, BOOL exp_sw, BOOL bOnSize = FALSE);
	void 	kamoku_expan(int ln, int pn, CRect* rc, BOOL exp_sw, BOOL bOnSize = FALSE);

	void	TkyDBedtEnable(BOOL bEnable);

	BOOL	IsScanLine( int dt_ln );
	void	ChangeType( int type, BOOL bLnChg = FALSE );
	int		setDATE_CREC( CDBINPDataRec *pdata );
	int		setSY_CREC( CDBINPDataRec *pdata, int dcsw, int ln, int lntype = 0 );

	int		get_dcsgn();
	int		_get_dcsgn( int pn );

	void	ScanAllTotalClc();
	void	ScanTotalClc( unsigned char* kari, unsigned char* kasi );

	void	PutsSyz( int ln, int pn, char* syztxt, char sy_chg );
	LPCTSTR sy_chk_txt( CDBINPDataRec *data, struct _SY_MSG_PACK2* smp );

	COLORREF	GetConfirmBackColor( char sgn );
	COLORREF	GetLineBackColor( int dt_ln );

	long	TerminationSyz( long nChar );

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

	LRESULT OnHjiyuInEnd( WPARAM w, LPARAM l );
	LRESULT OnHjiyuSel( WPARAM w, LPARAM l );
	void	_hjiyu_selectsub( int number, int selclick, CString* pStr );
	void	hjiyu_to_tekiyo( CString* jiyu );

	void	kesyu_chkdsp();

	BOOL	GetBmnEtc( struct _SET_KMKETC* psk );
	BOOL	GetKojiEtc( struct _SET_KMKETC* psk );

	BOOL	_set_kojietc( struct _SET_KMKETC *sk );

	int		AutoSiwakeCheck(int ln);

	void	FuriHendJiyuCheck( CDBINPDataRec *pcrec, int dcnt, int dt_ln, int clear = 0 );
	void	HendJiyuCheck( CDBINPDataRec *pcrec, int dt_ln, int clear = 0 );

	int		tky_select_to_drec( CDBINPDataRec *drec, CDBipTKREC *ptk, char *dbr_inptype, char *cbr_inptype, int dataLn = 0);
	void	set_skbn_selsw( CDBINPDataRec* pdata, struct _DenpData* pkey, int zeipn );

	int		dnp_insert_job( int ln );
	void	OnButtonInsert();
	BOOL	ButtonInsertJob();

	void	dbedt_putback();
	LPCTSTR get_hjnptitle();

	BOOL	FuriModeChange( int mode );
	BOOL	IsTanBmnEnable();
	void	DispBmnKojiData( int mode );

	int		ChangeDnpyoInput();
	void	DenpDlgDisp( int type );
	void	sheet_clr( int mode = SHEET_ALL, int modelessEnd = 0);

	void	inpdata_inz();
	void	inpdata_dsp();

	void	motikosi_inz( int motikosi );
	void	total_inz( int mode = SHEET_ALL  );

	void	setDATA_LINE_COLOR( int ln, COLORREF col, BOOL fore );
	int		CheckDenpyoData( int vect );
	void	ChangeJob( int mode );
	void	DisplayScanData();
	void	OnButtonChangeKind( UINT kind, BOOL bNoCheck = FALSE );
	int		CheckFuriBmnKoji();
	void	SetScanData( CDBINPDataRec& data, int ln, int karikasi );

	void	TransLineSign( CDBINPDataRec* pdt, int ln );
	void	TransLine( int dt_ln, int ln, char* sy_chg );
	BOOL	IsTransLine( int ln, char& edt_sg );

	void	DispBaseKmk();
	void	ChangeKind( int nkind );

	BOOL	is_lastdenp_contents();
	BOOL	is_line_contents( int ln, int max_pn );

	BOOL	DaWrite();

	BOOL	IsUsedDenpNo( int inpcno );
	int		CheckEdabanInput( int& chk_ln, int& chk_pn );
	int		NoinpScrolChk( int chk_ln, int chk_pn );
	int		CheckKOJI( int& chk_ln, int& chk_pn );
	int		CheckBmnNoInp( int& chk_ln, int& chk_pn );
	int		CheckBmnHani( int& chk_ln, int& chk_pn );
	int		CheckKojiKmk( int& chk_ln, int& chk_pn );
	int		CheckBmnTorok( int& chk_ln, int& chk_pn );
	int		CheckKojiTorok( int& chk_ln, int& chk_pn );

	int		AppendData( CDBINPDataRec *dat, int max, ADDNEWTBL* antbl );

	int		CheckAppendKojiKmk( CDBINPDataRec* rec );
	int		CheckAppendBumon( CDBINPDataRec* rec );
	BOOL	BmnTorokMessage( int bmn, vector<int>&	bmnvec, BOOL bMsg = TRUE );
	BOOL	KojiTorokMessage( CString koji, vector<CString>&	kojivec, BOOL bMsg = TRUE );

	BOOL	syog_next();
	void	ProfileSet();
	int		EndScan( BOOL bUpdate = TRUE );
	int		UpdatePage( int mode );

	int		ScanTotalCheck( int mode, BOOL bMsg = TRUE );

	int		MakeNewData( int mode, int ln, CDBINPDataRec* drec );
	void	ModifyTaisyakuData( int mode, int ln, int& newcnt, CDBINPDataRec* newrec, int& lcnt, CDBINPDataRec* drec );
	void	ModifyDebtData( int mode, int ln, int& newcnt, CDBINPDataRec* newrec, int& lcnt, CDBINPDataRec* drec );
	void	ModifyCredData( int mode, int ln, int& newcnt, CDBINPDataRec* newrec, int& lcnt, CDBINPDataRec* drec );
	int		ModifyData( CDBINPDataRec* drec, int cnt );

	int		da_make( int ln, CDBINPDataRec *dtbl );
	int		siwake_mak( struct _DP_DATA_LINE *datp, CDBINPDataRec *dtbl0, int sgn );
	int		sydata_mak( CDBINPDataRec *dtbl0, CDBINPDataRec *wktbl, int wktbl_n, struct _DP_DATA_LINE *datp );

	int		ModifyCheckKojikmk( CDBINPDataRec* drec, int cnt, int& chk_ln, int& chk_pn, BOOL bMsg );
	int		ModifyCheckKojiTorok( CDBINPDataRec* drec, int cnt, int& chk_ln, int& chk_pn, BOOL bMsg );
	int		ModifyCheckKoji( CDBINPDataRec* drec, int cnt, int& chk_ln, int& chk_pn, BOOL bMsg );
	int		ModifyCheckBmnHani( CDBINPDataRec* drec, int cnt, int& chk_ln, int& chk_pn, BOOL bMsg );
	int		ModifyCheckEdabanInput( CDBINPDataRec* drec, int cnt, int& chk_ln, int& chk_pn, BOOL bMsg );
	int		ModifyCheckBmnTorok( CDBINPDataRec* drec, int cnt, int& chk_ln, int& chk_pn, BOOL bMsg );

	int		TaikaInputCheck( int ln, int syzpn );
	int		kojimsg_make( CString& msg, int dc_sw, CDBINPDataRec* drec );
	int		bmnmsg_make( CString& msg, int dc_sw, CDBINPDataRec* drec );
	int		edabanmsg_make( CString& msg, int dc_sw, CDBINPDataRec* drec, int ln );
	void	edabanmsg_addjob( CString& addmsg, CString edamsg, int nMsg, int& MsgCnt, const int MsgMax );

	BOOL	bmncode_todata2( CDBINPDataRec* data, int number, int dc_sw );
	void	SelectedIcsselctrl(short position, long Number, int place );

	void	SetSkbnLineDisp();
	BOOL	IsEnableToolBar( int nID );
	BOOL	EnableButtonF3();
	BOOL	EnableButtonF4();
	BOOL	EnableButtonF12();
	BOOL	EnableButtonIns();
	BOOL	EnableButtonDel();
	BOOL	EnableButtonF7();

	void	ScanDeleteLine();

	int		ToolF3Job();
	int		ToolF4Job();
	int		ToolF12Job();
	int 	ToolINSJob();
	int 	ToolDELJob();
	int 	ToolF7Job();

	void	Lineattr_dsp();
	BOOL	GetSyzMdfySign( int seq );
	void	TkyEda_Redisp(int dc_sw, struct _DP_DATA_LINE *pdat);
	void	SetDenpyoLine( int seq );
	void	TkyInpEndUpdate();
	void	KamokuUpdate( int ln, int pn );
	BOOL	ChangeZeiritsu();
	BOOL	ChangeSotoMenZei();

	BOOL	tekiyp_F2job();

	void	syobrnedit_expan(int ln, int pn, CRect* rc, BOOL exp_sw, BOOL bOnSize = FALSE);
	void	TerminationSyoBrnDBedit(UINT id, long nChar, long inplen, long kst);

	void	TerminationSyoNumDBedit(UINT id, long nChar, long inplen, long kst);
	void	OnTermSyoNum(int ln, long nChar);

	void	TerminationKmkinp(UINT id, long nChar, long inplen, long kst);
	int		CheckImeEndKamoku(long nChar, LPCTSTR string);
	HWND	GetKamokuCtrlHWND();

	void	bmneda_update(int displn, int pn);
	void	swktekiyo_dspsub(CString tky, int dbedt);
	void	tekiyo_selset_subfunc(int dt_ln, CDBipTKREC* ptkrec, BOOL bTKcut);

	void	TKsgnToSYDATA(struct __SYDATA* syd, CDBipTKREC* ptk);
	BOOL	IsTekiyoSyzChg(struct __SYDATA* syd);
	void	SetSyTkrec(_SY_TKREC_ *psyTkRec, struct __SYDATA* syd);
	void	SyTkrecMenCheck(_SY_TKREC_ *psyTkRec, CDBINPDataRec* data);
	BOOL	IsKamokuSyattr(int ln, int pn);
	BOOL	IsSameFuriSyzKbn(int ln);

	void	FocusCountClear(int line);
	void	set_sotomen_sign(int dt_ln, int pn, int sotomen);

	void	CancelInvNoIME();
	void	_DSGN_MenWariFnc(struct _DenpData *keybuf);
};
