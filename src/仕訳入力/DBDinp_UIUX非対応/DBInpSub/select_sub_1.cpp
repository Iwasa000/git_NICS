#include "stdafx.h"

#include "DBInpSubFunc.h"
#include "BrnTky.h"
#include "KJNamRec.h"

#include "DBDinpRec.h"
#include "ImgMng8.h"	// _SLIP_ACCOUNTS_


#define AC_MAX		32
#define PG_MAX		100
#define DP_MAX		16

//#define SEL_LEN		21
#define SEL_LEN		41

#define KPG_MAX		((PG_MAX*100)+32)

#define IMM_OFSET	33
#define AGR_OFSET	66

//工事チェック用配列
struct KJICHK
{
	CString	kjcode;		//工事コード
	int		kj_kymd;	//工事完成日
};


extern
int get_strcnt(char* str, int byte);

void init_brntkypar( BRNTKY_PAR* par )
{
	par->bt_bmn = -1;
	par->bt_code.Empty();
	par->bt_kana.Empty();
	par->bt_page = 0;
	par->bt_txtcol = 0;
}

typedef struct _KMKANA_DTA
{
	char	kd_kana[6];
	int		kd_seq;
} KMKANA_DTA;


typedef struct _INDKMK_DTA
{
	char	ind_code[10];

	BYTE	ind_ctg;
	BYTE	rsv8;
	short	ind_prf1;
	short	ind_prf2;
	short	ind_prf3;
} INDKMK_DTA;


//内部取引消去 名称
typedef struct _NBTORI_DTA
{
	int		nbt_code;
	char	nbt_name[40];

} NBTORI_DTA;



// 内部取引消去 階層区分
typedef struct _NaibuLevel
{
	int		nb_code;
	char*	nb_name;
} NaibuLevel;

NaibuLevel	NLdata[] = {
	{ 0, "なし" },
	{ 1, "事業区分間" },
	{ 2, "拠点区分間" },
	{ 3, "ｻｰﾋﾞｽ区分間" },
#ifdef NAIBU_VER2
	{ 4, "ｻｰﾋﾞｽ区分内細区分1" },
	{ 5, "ｻｰﾋﾞｽ区分内細区分2" },
#endif
	{ -1, "" },
};

///////////////////////////////////////////////////////

class CSelectSub
{

public:
	CSelectSub();
	~CSelectSub();

public:
	int	InitSub( CDBZmSub* pZm, CWnd* pwnd, SELECTDATAPROC fix, SELECTDATAPROC sel32 );
	int	InitSub( CDBNpSub* pZm, CWnd* pwnd, SELECTDATAPROC fix, SELECTDATAPROC sel32 );

	void SetWindow( CWnd* pwnd );
	void CloseSub();
	int	DispOptChange( int opt );

	int		_BmonSelect( int nChar, BOOL bNowPage = FALSE );
	void	_BmonSelectSub( int page );
	int		_KojiSelect( int nChar, BOOL bNowPage = FALSE );
	void	_KojiSelectSub( int page );
	BOOL	_koji_search();
	BOOL	_Kojicode_Ok( CString& kjcode );

	BOOL	_Kojicode_NoInp( CString kno, CString kmkcode );
	BOOL	_Kojicode_KanChk( CString kno, CString kmkcode );

	int		_KamokuSelect( int nChar );
//	int		_BrnNameSelect( int nChar );
//	int		_TkyoSelect( int nChar );
	// 変動事由
	int		_HjiyuSelect( int nChar, BOOL bNowPage = FALSE );
	void	_HjiyuSelectSub( int page, int typ );
	BOOL	_GetHjiyuSelect( SelResult* result, int page, int sel_no );
	// 公益 振替科目
	BOOL	is_npfuridata( LPCTSTR deb, LPCTSTR cre );
	int		_NPfuriSelect( int nChar, BOOL bNowPage = FALSE );
	BOOL	_GetNPfuriSelect( SelResult* result, int page, int sel_no );

	int		_SOWNaibuSelect( int nChar, BOOL bNowPage = FALSE );
	void	_NaibuSelectSub( int page );
	BOOL	_GetNaibuSelect( SelResult* result, int page, int sel_no );
	BOOL	_GetNaibuString( int type, int nbcd, CString& str );

/*- '09.03.11 -*/
//	int		_SelectJob( SelPar* par, int nChar = -1 );
/*-------------*/
	int		_SelectJob( SelPar* par, int nChar = -1, SelParEx* parex = NULL );
/*-------------*/
	int		_SelectUpdate();

	void _SetTekiyoMode( int mode ) { m_TkyMode = mode; };
	void _SetKmDispMode( int mode ) { m_KmkMode = mode; };
	void _SetBrnTkyMode( int mode ) { m_BrnMode = mode; };
	void _SetKanaDspMode( int mode ) { m_KjunDsp = mode; };
	void _SetKamokuPageHold( BOOL bHold ) { m_bKmkHold = bHold; };

	int _GetTekiyoMode() {	return m_TkyMode; };
	int _GetKmDispMode() {	return m_KmkMode; };
	int _GetBrnTkyMode() {	return m_BrnMode; };
	int _GetKanaDspMode() {	return m_KjunDsp; };
	BOOL _GetKamokuPageHold() {	return m_bKmkHold; };

	BRNTKY_PAR _GetBrnTkyPar() { return m_BRNTKY; };

	struct _SelTable* _GetNowFix8Select();
	struct _SelTable* _GetNowSel32Select();

	COLORREF _GetSel32TextColor();
	COLORREF _GetSel32BackColor();

	BOOL _InputItemCheck(struct _KamokuData& ans, struct _KamokuData& check, int _dcsw, BOOL bSelClick = FALSE );
	BOOL _GetSelResult( SelResult* result, char* selstr );
	BOOL _GetBmonSelect( SelResult* result, int page, int sel_no );
	BOOL _GetKojiSelect( SelResult* result, int page, int sel_no );
	
	BOOL IsHenJiyuCodeRegisted( int h_code );
	BOOL GetJiyuMeisyo( BYTE jycode, char* pJynam );
	int  ScanKHKbnMake();
	int  GetScanKHKbn( int index, KSH_SCAN* pKshScan );
	int		GetHjiyuCount(int sgn);
	const	KSH_NAMDTA*	GetHjiyuData(int sgn, int index);

	BOOL _GetKojiName( CString& kjcode, CString& name,BOOL bFull );
	void	tekiyo_update();

	BOOL get_npfuristr( int	frcode, CString& str );
	BOOL is_npfuricode( int	frcode );
	void NPfuriHani( DWORD& scod, DWORD& ecod );
	int	 GetNPfuriCount();
	const NP_FURIDTA*	GetNPfuriData(int index);

	BOOL _NP_InputItemCheck(struct _KamokuData& ans, struct _KamokuData& check, int _dcsw, BOOL bSelClick = FALSE );

	void _KamokuRestrict( DWORD dwFlag );
	void _OrderKamokuRestrict( int order ); //091228
	void _SetOrder( int order, int old_order );				//091228
	BOOL check_kamokurestrict( int knpos );
	BOOL check_kamokurestrict( DBKNREC* pKn );
	BOOL _check_kamokurestrict( int kjn_type, DBKNREC* pKn/*, INDKMK_DTA* pInd*/ );

	void _NpKamokuRestrict( DWORD Jgykcd, DWORD Knrkcd );	// '08.12.01
	BOOL _check_npkamokurestrict( DBKNREC* pKn );			// '08.12.01

	int		restrict_resetpage();
	BOOL	_chk_kamokuctg( int page );

	int		NP_restrict_resetpage( DWORD oldJgy, DWORD oldKnri );	// 04.01 /09

	int		_BrnTekClear();

	BOOL	_IsNaibuData( LPCTSTR deb, LPCTSTR cre );
	BOOL	IsMasterNpdata();

#ifdef _SLIP_ACCOUNTS_
	// 原票会計
	void	SetImgMng( CImgMng *pImgMng );
	//	選択結果を取得
	BOOL _GetImgStrSelect( SelResult* result, int page, int sel_no );
#endif

	void  SetSelOptMode( DWORD opt );
	DWORD GetSelOptMode();

	void  SetBrnSelDispMode( int mode );
	void  ResetTkallPage();

protected:
	void	_init_kamoku(int update = 0);
	void	_init_kamoku_oder(int update = 0, DWORD dwStart = 0, DWORD dwEnd = 0 ); //091228
	void	_init_bumon();
	int		_init_hjiyu();
	int		_init_koji();
	void	_init_kengyokmk();	// 05.26 /10

	void	_init_kengyokmk_kaitest();

	int		UpdateSelectData();

	int		resetpag( int page );
	int		skip_resetpag( int& page, int vect );
	void	fixcreat(int *fix);
	int		km_dsp( int *knp  );
	int		kmk_50search( char* kana );
	void	kn50dsp( KMKANA_DTA *knp, int syusgn = 0 );
	int		get_hyojikncod( int n );
	char	*get_hyojiknam( int n, int syusgn = 0 );

	// '09.02.24
	int		kmk_inpcodesearch( long inpcd, int onedspsw = 0 );
	int		Syus_inpcodesearch( long inpcd );

	int		brntky_sch( BRNTKY_PAR *tkbrn, struct _SelTable *sel32D, int bmn, CString& code, char* kana = NULL, BOOL bDISP = FALSE );
	int		_brntky_sch( BRNTKY_PAR *tkbrn, int bmn, CString& code, char* kana, BOOL bDISP = FALSE );
	void	_brntky_dsp( BRNTKY_PAR *tkbrn, struct _SelTable *sel32D,int start_no );
	int		brntky_page( BRNTKY_PAR *tkbrn, struct _SelTable *sel32D, int page );
	int		brntky_get( int *bmn, CString *code, int *brn, BRNTKY_PAR *tkbrn, int no );

	void	SelDspRst();
	int		SelTkyDsp( int nChar );
	void	tky50disp( int pg, int cnt );
	BOOL	tky_select50_dsp( char *data, BOOL bDISP = TRUE );
	BOOL	make_50tky();
	int		tkrsrch( char *akbuf );
	int		rentky_srdsp( CString debt, CString cred );
	void	_rentky_str( CString debt, CString cred );
	int		rentkydsp( void );
	int		rentksr( short arg2 );

	void	rentdsp( int pg, int dc_sw );
	int		RTpmax_Get();

	BOOL	make_autotky( BOOL bLastDisp = FALSE );
	void	tkauto_dsp( int pg );
	int		tky_atswk_dsp( CString deb, CString cre, BOOL bDISP = TRUE );

	void	tky50_reload();
	void	tky50_clear();

	// 摘要処理番号 返送
	int		get_tkyotypeno( int type );

	int		DB_TekiyoNoRead( int tkcod );
	int		DB_KanaTekiyoNoRead( int tkcod );

	BOOL	bTkyKana;
	BOOL&	tkrec_kanamode() { return bTkyKana; }

	BOOL	m_bKmkHold;

	/* 公益法人 */
	int		_init_npsub( int sgn );
	void	npdata_set();
	void	update_npdata();
	int		_close_npsub();

	int		_SyusSelect( int nChar );
	int		reset_syus( int page );
	int		Syus_50search( char* kana );

	void	nbtori_set();

#ifdef _SLIP_ACCOUNTS_
	// 原票イメージによる摘要表示
	int		_SlipTkySelect( int nChar, BOOL bNowPage=FALSE );
	// 原票イメージによる摘要表示
	int		slip_tky_dsp( int page, BOOL bSameImg=FALSE );
#endif

protected:

	struct _SelTable SelFix8[2*4];
	struct _SelTable SelVal32[8*5];

	UINT	FIXTBL[8];
	UINT	KDPAG[PG_MAX][40];

	char	svKana[20], svTkyKana[20];
	char	svBmnKana[20];
//	int			KCD50C[AC_MAX*PG_MAX];
	KMKANA_DTA	KCD50C[AC_MAX*PG_MAX];

	BRNTKY_PAR	m_BRNTKY;
	SelPar		m_NowPar;
	SelParEx	m_NowParEx;	// '09.03.11
	SelSetProcPar m_spp;

	CWnd* m_pWnd;
	SELECTDATAPROC m_fixproc;
	SELECTDATAPROC m_sel32proc;

	COLORREF m_Sel32txt, m_Sel32bak;

	int		TKSEL_PG[36];	// 摘要ページ(部門でも使用)
	CString KOJI_PG[36];	// 工事番号ページ

	int		m_PageMax;	// 選択1ページあたりの表示数
	int		m_CDPG, m_KPGcnt;
	int		m_ACsel, m_ACcnt;
	int		m_BmnPG;
	int		m_KojiPG;
	int		m_KjKana;		// 工事カナ検索(1) 時 工事全検索(0)
//	int		m_KjKanaPG;

	int		m_BrnTkySel;
	int		m_TkyMode, m_TkySign;
	int		m_RTpage, m_RTKcnt[2], m_RTKblk[2];
	int		m_T50page, m_T50pmax, m_SW_TK50;
	int		m_TATpage, m_TATpmax, m_SW_AutoTK; 
	int		m_KmkMode;
	int		m_BrnMode, m_BrnCurMode;
	int		m_KjunDsp;
	int		m_BmnMax;
	int		m_KojiMax, m_KjKanaMax;
	BOOL	m_bHjiyu;
	int		m_HjiyuPG, m_HjiyuMax;
	int		m_NaibuPage;

	CString		m_Debcd, m_Crecd;
	char		m_save_50kana[10];
	int			m_Brn;
	char		m_BrnKana[10];

	CArray<KSH_NAMDTA, KSH_NAMDTA&> m_KabNam;
	CArray<KSH_NAMDTA, KSH_NAMDTA&> m_IgaiNam;
	// スキャン項目用
	CArray<KSH_SCAN, KSH_SCAN&>	m_KshScan;

	CDBZmSub*	m_pZm;
	CBrnTky		m_BrnTkyo;
	CKJnamAccess	m_Kjn;
	// 公益法人クラス
	CDBNpSub*	m_pNPb;
	BOOL		m_bNpbAlloc;

	int		m_NPpage;
	DWORD	m_NPfcd[2];
	CArray<NP_FURIDTA, NP_FURIDTA&>	m_NPfuri;
	CDWordArray		m_SCHskn;

	int		m_SyusPG, m_SyusCnt;
	int		m_SyusKanaPG, m_SyusKanaCnt;

	//表示制限用フラグ
//	CArray<INDKMK_DTA, INDKMK_DTA&>	m_INDkmk;
	DWORD	m_DRsgn;

	// 表示制限用科目（公益）
	DWORD	m_DJgykcd;
	DWORD	m_DKnrkcd;

	//-091228
	int Order_flg;	//　現在のフラグ
	int Order_flg2;	//　前回のフラグ
	void	fixcreat_order(int *fix, int *k_fix);

	//工事チェック用
	CArray<KJICHK, KJICHK&>	m_KjiChk;

	//内部取引消去
	CArray<NBTORI_DTA, NBTORI_DTA&>	m_NBdta;
	DWORD	m_NPopn;

	//按分の登録があるか？
	BOOL	m_bNPanbn;

	// 摘要カナ検索
	CArray<DBTKREC, DBTKREC&>	m_KanaTKrec;

#ifdef _SLIP_ACCOUNTS_
	CImgMng*		m_pImgMng;		// イメージ管理クラス
	CStringArray	m_MatchStrAry;	// イメージダイアログ内文字列保存
	int				m_SlipTkyCnt;	// イメージ文字列総数
	int				m_SlipTkyPage;	// イメージ文字列表示ページ
#endif

	// 科目範囲
	DWORD	m_dwStart, m_dwEnd;
	// 摘要全表示時のページ
	int		m_tallPG;

	// 選択表示のオプション
	DWORD	m_dwOPT;		// D0: ON=仕訳対応摘要で登録通りの表示, OFF=単独科目
							// D1: ON=部門摘要枝番を使用する。		OFF=科目摘要枝番(DEFAULT)
							// D2: ON=対応摘要をカナ順で表示。		OFF=登録順で表示(DEFAULT)

	int		m_TKYO_LEN;

	int		m_BrnSelMode;

#ifdef CLOSE
	BOOL	fix8sg;
	BOOL	fixvalsg;

	CICSSel* m_pfix8
	CICSSel* m_pfix32
	
	CICSDisp*	m_pTilte;
	CICSDisp* 	m_pTitleBase;
	
	CWnd*	m_pParent;
	double	m_Xrate, m_Yrate;
#endif

};



CSelectSub::CSelectSub()
{
	::ZeroMemory( FIXTBL, sizeof FIXTBL );
	::ZeroMemory( KDPAG, sizeof KDPAG );
	::ZeroMemory( KCD50C, sizeof KCD50C );
	::ZeroMemory( svKana, sizeof svKana );
	::ZeroMemory( svTkyKana, sizeof svTkyKana );

	::ZeroMemory( svBmnKana, sizeof svBmnKana );

	::ZeroMemory( TKSEL_PG, sizeof TKSEL_PG );

	m_spp.sp_nowpn = m_spp.sp_prevpn = -1;
	::ZeroMemory( m_spp.seljob, sizeof m_spp.seljob );//	fix8sg = fixvalsg = FALSE;

	m_BrnTkySel = 0;
	m_TkyMode = SL_TKY_RENSO;
	m_KmkMode = SL_KMK_CODE;
	m_BrnMode = m_BrnCurMode = 0;
	m_KjunDsp = SL_NORMAL_MODE;

	m_BmnMax = m_KojiMax = 0;

	m_TkySign = 0;
	m_PageMax = AC_MAX;

	m_ACcnt = m_ACsel = 0;
	m_CDPG = 1;

	m_bKmkHold	= FALSE;
	m_bHjiyu	= FALSE;

	m_pNPb		= NULL;
	m_bNpbAlloc	= FALSE;
	m_NPpage	= 0;

	m_SyusPG	= 1;
	m_SyusCnt	= 0;
	m_SyusKanaPG = m_SyusKanaCnt = 0;

	m_DRsgn		= 0;

	//- 091228
	Order_flg = Order_flg2 = 0;

	m_NaibuPage = 0;
	m_NPopn = 0;

	m_bNPanbn = FALSE;

#ifdef _SLIP_ACCOUNTS_
	m_pImgMng = NULL;
	m_SlipTkyPage = 1;
	m_SlipTkyCnt = 0;
#endif

	m_dwStart = m_dwEnd = 0;

	m_tallPG = 1;
	::ZeroMemory( m_save_50kana, sizeof m_save_50kana );

	m_TKYO_LEN = TKYO_LEN;

	m_BrnSelMode = SL_NORMAL_MODE;
}


CSelectSub::~CSelectSub()
{
//TRACE("~CSelectSub()\n");

}


// クラス初期化
int CSelectSub::InitSub( CDBZmSub* pZm, CWnd* pwnd, SELECTDATAPROC fix, SELECTDATAPROC sel32 )
{
	m_pZm = pZm;
	m_pWnd = pwnd;

	// 科目名称
	_init_kamoku();
	// 部門
	_init_bumon();

	m_pZm->tkrec->Requery( "",  0 );
	tkrec_kanamode() = FALSE;

	m_RTKcnt[0] = 0;
	m_RTKcnt[1] = 0; 
	m_RTKblk[0] = 0;
	m_RTKblk[1] = 0;
	m_RTpage = 0;

	m_T50page = m_T50pmax = m_SW_TK50 = 0;
	m_TATpage = m_TATpmax = m_SW_AutoTK = 0;	

	m_Debcd.Empty();
	m_Crecd.Empty();

	m_Brn = -1;
	::ZeroMemory( m_BrnKana, sizeof m_BrnKana );

	m_BrnTkyo.brntky_iniz( m_pZm );
	m_BrnTkyo.brtek_free();

	m_fixproc = fix;
	m_sel32proc = sel32;

	//変動事由
	if( _init_hjiyu() != 0 )
		return -1;

	// 工事名称
	_init_koji();

	//振替科目[公益]
	if( _init_npsub(0) != 0 )
		return -1;

	int len = GetTekiyoLength( m_pZm );
	if( len > 0 )	m_TKYO_LEN = len;

	return 0;
}



// 公益法人クラスでの クラス初期化
int CSelectSub::InitSub( CDBNpSub* pNpSub, CWnd* pwnd, SELECTDATAPROC fix, SELECTDATAPROC sel32 )
{
	m_pZm	= (CDBZmSub*)pNpSub;
	m_pNPb	=	pNpSub;
	m_pWnd = pwnd;

	// 科目名称
	_init_kamoku();
	// 部門
	_init_bumon();

	m_pZm->tkrec->Requery( "",  0 );
	tkrec_kanamode() = FALSE;

	m_RTKcnt[0] = 0;
	m_RTKcnt[1] = 0; 
	m_RTKblk[0] = 0;
	m_RTKblk[1] = 0;
	m_RTpage = 0;

	m_T50page = m_T50pmax = m_SW_TK50 = 0;
	m_TATpage = m_TATpmax = m_SW_AutoTK = 0;	

	m_Debcd.Empty();
	m_Crecd.Empty();

	m_Brn = -1;
	::ZeroMemory( m_BrnKana, sizeof m_BrnKana );

	m_BrnTkyo.brntky_iniz( m_pZm );

	m_fixproc = fix;
	m_sel32proc = sel32;

	//変動事由
	if( _init_hjiyu() != 0 )
		return -1;

	// 工事名称
	_init_koji();

	//振替科目[公益]
	if( _init_npsub(1) != 0 )
		return -1;

	int len = GetTekiyoLength( m_pZm );
	if( len > 0 )	m_TKYO_LEN = len;

	return 0;
}


//----------------------------------------------------------------------
// １つのオブジェクトで複数のビューでの選択設定をサポートするため
// ウィンドウのセット
//----------------------------------------------------------------------
void CSelectSub::SetWindow( CWnd* pwnd )
{
	m_pWnd = pwnd;
}


// 科目データ読み込み
void CSelectSub::_init_kamoku( int update/*= 0*/)
{

	char	_cnt[PG_MAX];
	int		n, ofs;
	BOOL	bKengyo = FALSE;

	::ZeroMemory( FIXTBL, sizeof FIXTBL );
	::ZeroMemory( KDPAG, sizeof KDPAG );
	::ZeroMemory( KCD50C, sizeof KCD50C );

	::ZeroMemory( _cnt, sizeof( _cnt ) );

	int type =  IsKojinGyosyuMaster( m_pZm );
	if( type ) {
		if( IsKojinKengyoMaster( type ) ) {
			bKengyo = TRUE;
		}
	}

	if( ! update ) {
		int type =  IsKojinGyosyuMaster( m_pZm );
		if( type ) {
			if( IsKojinKengyoMaster( type ) ) {
			//	if( type & 0x01 )	m_DRsgn |= SL_IND_IPPAN;
			//	if( type & 0x02 )	m_DRsgn	|= SL_IND_HUDOSAN;
			//	if( type & 0x04 )	m_DRsgn |= SL_IND_NOGYO;

				m_DRsgn = SL_IND_ALL;
			}
			else {
				// 単一の業種
				m_DRsgn = (type&0x0f);
			}
		}

		m_DJgykcd = m_DKnrkcd = 0xffffffff;
	}

	//兼業で全業種表示
	if( m_DRsgn == SL_IND_ALL ) {
		if( bKengyo && !(m_pZm->zvol->apno == 0x10 && (m_pZm->zvol->ind_table&0x01)) ) {
			_init_kengyokmk();
			return;
		}
	}

	struct _DBKNREC	*pKn;
	for( int i = 0; i < m_pZm->knm_info->reccount; i++ ) {

		pKn = m_pZm->pKnrec + i;

		if( (pKn->knvoid & 0x01) )
			continue;

		if( ! check_kamokurestrict( pKn ) )
			continue;

		if( pKn->knfkn >= 1 && pKn->knfkn <= 8 ) {
			if( bKengyo ) {
				if( FIXTBL[ pKn->knfkn-1 ] == 0 )
					FIXTBL[ pKn->knfkn-1 ] = i+1;
			}
			else {
				//兼業でないときは、最後の設定科目優先
				FIXTBL[ pKn->knfkn-1 ] = i+1;
			}
		}

		if( pKn->knkpg >= 101 && pKn->knkpg <= KPG_MAX ) {
			n	= (pKn->knkpg / 100);
			ofs	= (pKn->knkpg % 100);
			ofs--;
			//<-- mmm add No.16-0212
			if( !(m_DRsgn & SL_IND_ALL) ) {
				if( type && ( m_DRsgn != pKn->ind_ctg ) ){
					if( !pKn->ind_ctg ){
						if( m_DRsgn != ( m_pZm->zvol->ind_sgn2 & 0x07 ) )
							continue;
					}
					else	continue;	
				}
			}
			//--> mmm add
			if( _cnt[ n-1 ] <= 32 ) {
				if( bKengyo ) {
					if( KDPAG[ n-1 ][ ofs ] == 0 ) {
						KDPAG[ n-1 ][ ofs ] = i+1;
						_cnt[ n-1 ] += 1;
					}
				}
				else {
					//兼業でないときは、最後の設定科目優先
					KDPAG[ n-1 ][ ofs ] = i+1;
					if( KDPAG[ n-1 ][ ofs ] == 0 )
						_cnt[ n-1 ] += 1;
				}
			}
		}
	}
}

// 部門データ読み込み
void CSelectSub::_init_bumon()
{
	if( (m_pZm->zvol->sub_sw&0x02) ) {
		m_pZm->bmname->Requery( "", "" );	
		m_pZm->bmname->MoveFirst();
		do {

			if( ! m_bNPanbn/*IsSyafuKaisei( m_pZm->zvol )*/ ) { 
				// 合計部門は除く
				if( (m_pZm->bmname->bnhsg[0] & 0x02) )
					continue;
			}

			m_BmnMax++;
		} while( m_pZm->bmname->MoveNext() == 0 );
	}
	else {
		m_BmnMax = 0;
	}
}

// 変動事由データ読み込み
int CSelectSub::_init_hjiyu()
{
	//変動事由
	if( m_pZm->kshctl ) {
		if( m_pZm->kshctl->hcvers >= 0x10 )
			return -1;
		
		KSH_NAMDTA ksh;
//		CArray<KSH_NAMDTA, KSH_NAMDTA&> kabtmp;
//		CArray<KSH_NAMDTA, KSH_NAMDTA&> igaitmp;

		m_pZm->kshnam->MoveFirst();

		m_KabNam.RemoveAll();
		m_IgaiNam.RemoveAll();

		switch( m_pZm->zvol->apno ) {
		case 0x00:
		case 0x01:
		case 0x20:
			do {
				if( m_pZm->kshnam->kofs > 0 )
					break;

				ksh.hn_cod = m_pZm->kshnam->hnjcod;
				ksh.hn_name = m_pZm->kshnam->hnname;
				ksh.hn_pflg = m_pZm->kshnam->hnpflg;

				if( (ksh.hn_cod >=  1 && ksh.hn_cod <= 20) ||
					(ksh.hn_cod >= 31 && ksh.hn_cod <= 50) ) {
					if( (ksh.hn_pflg & 0x01)  ) {
						m_KabNam.Add( ksh );
					}
				}
				else if( (ksh.hn_cod >= 61 && ksh.hn_cod <= 80) ) {
					if( (ksh.hn_pflg & 0x01) ) {
						m_IgaiNam.Add( ksh );
					}
				}
				else {
#ifdef SOKYU_CLOSE
					// 遡及事由設定対応
					if( m_pZm->kshctl->hcdmy2 == 1 ) {
						if( (ksh.hn_cod >= 86 && ksh.hn_cod <= 90) ) {
							if( (ksh.hn_pflg & 0x01) ) {
								m_KabNam.Add( ksh );
								m_IgaiNam.Add( ksh );
							}
						}
					}
#endif
				}

			} while( m_pZm->kshnam->MoveNext() == 0 );
			m_bHjiyu = TRUE;
			break;
		default:
			m_bHjiyu = FALSE;
		}
	}

	return 0;
}


// 工事名称データ読み込み
int CSelectSub::_init_koji()
{
	// 工事名称
	m_KojiMax	= 0;
	m_KjKanaMax	= 0;
	m_KjKana	= -1;

	m_KjiChk.RemoveAll();

	if( m_pZm->zvol->apno == 0x20 ) {
		m_Kjn.SetDBZmSub( m_pZm );
		if( m_Kjn.KjNameOpen() != 0 )
			return -1;

		struct KJICHK	kjchk;
		int cnt = 0;
		
		if( m_Kjn.kjnam != NULL ) {
		//	m_KojiMax = m_Kjn.kjnam->GetRecordCount();
			m_Kjn.kjnam->MoveFirst();
			if( m_Kjn.kjnam->st != -1 ) {
				do {
					kjchk.kjcode	= m_Kjn.kjnam->KKI_KJCD;
					kjchk.kj_kymd	= m_Kjn.kjnam->KKI_KYMD;
	
					m_KjiChk.Add( kjchk );
					cnt++;
				} while( m_Kjn.kjnam->MoveNext() == 0 );
			}
		}

		m_KojiMax = cnt;
	}

	return 0;
}

// 公益法人 振替科目
int CSelectSub::_init_npsub( int sgn )
{
	if( ! IsMasterNpdata() )
		return 0;

	if( ! sgn ) {	// 財務クラスでの呼び出しの場合、当モジュールで 公益クラス作成
		m_bNpbAlloc	=	TRUE;

		if( m_pNPb != NULL ) {
			delete m_pNPb;
			m_pNPb = NULL;
		}

		if( m_pNPb == NULL ) {
			try {
				m_pNPb = new CDBNpSub;
			}
			catch( CMemoryException* me )
			{
				me->Delete();
				return -1;
			}
			m_pNPb->ExitMode(1);

			// FOR DB
			char	sqlsvr[256],apno[20],cocode[20],kikan[20];
			if( PCOM_GetString( "SELECTCODSN", sqlsvr, apno, cocode, kikan ) == 4 )	{
				if( m_pNPb->SetMasterDB( SMDB_VERS, sqlsvr, atoi( apno) , atoi( cocode) , atoi( kikan )) == -1 ) {
				//	ermset( -1, (char*)(LPCTSTR)m_pNPb->GetZmErrMsg() );
					return -1;
				}
			}
			else {
			//	ermset( ERROR_STOP, "DB 情報[SELECTCODSN]を取得できないので 処理を中止します。" );
				return -1;
			}

			// 1.非営利法人ボリュームラベルテーブル
			if( m_pNPb->NpVolumeOpen() == ERR ) {
				return -1;
			}

			// 8.指定正味財産科目名称＆残高テーブル
			if( m_pNPb->NpShiteiKamokuZanOpen() == ERR ) {
				return -1;
			}

			// 収支科目名称リード
		//	if( m_pZm->zvol->apno == 0x50 ) {
			if( IsSyusiKamokuMaster( m_pZm ) ) {
				if( m_pNPb->SyusiKamokuMeisyoRead() == ERR ) {
					return -1;
				}
			}
			// 非営利 科目名称テーブル
			if( m_pNPb->NpKamokuMeisyoOpen() == ERR ) {
				return -1;
			}
		}
	}
	else {
		ASSERT( m_pNPb != NULL );
		m_bNpbAlloc	=	FALSE;

		if( m_pNPb->m_pNpvol == NULL ) {
			// 1.非営利法人ボリュームラベルテーブル
			if( m_pNPb->NpVolumeOpen() == ERR ) {
				return -1;
			}
			m_NPopn |= 0x01;
		}

		if( m_pNPb->m_pNpsszrc == NULL ) {
			// 8.指定正味財産科目名称＆残高テーブル
			if( m_pNPb->NpShiteiKamokuZanOpen() == ERR ) {
				return -1;
			}
			m_NPopn |= 0x02;
		}

	//	if( m_pZm->zvol->apno == 0x50 ) {
		if( IsSyusiKamokuMaster( m_pNPb ) ) {
			if( m_pNPb->pKsrec == NULL ) {
				if( m_pNPb->SyusiKamokuMeisyoRead() == ERR ) {
					return -1;
				}
				m_NPopn |= 0x04;
			}
		}
		// 非営利 科目名称テーブル
		if( m_pNPb->m_pNpknrec == NULL ) {
			if( m_pNPb->NpKamokuMeisyoOpen() == ERR ) {
				return -1;
			}
		}
	}

	m_NPpage	=	0;

	int i, n;
	i = n = 0;
//	if( m_pZm->zvol->apno == 0x50 ) {
	if( IsSyusiKamokuMaster( m_pZm ) ) {
		for( i = n = 0; i < m_pNPb->sknm_info->reccount; i++ ) {

			DBKNREC* pSyrec = (m_pNPb->pKsrec+i);

			DWORD dwCode = pSyrec->kncod;
			hl_rev( &dwCode, 4 );

			if( dwCode < 0x08000000 ) {
				continue;
			}

			if( (pSyrec->knvoid & 0x01) )
				continue;

			n++;
		}
	}

	m_SyusCnt = n;

	npdata_set();

	nbtori_set();

	return 0;
}

// データベースよりデータをセット
void CSelectSub::npdata_set()
{
	m_NPfcd[0]	=	0;
	m_NPfcd[1]	=	0;
	m_NPfuri.RemoveAll();

	m_NPfcd[0]	=	_atoh( (LPSTR)(LPCTSTR)m_pNPb->m_pNpvol->m_npsbsc );
	m_NPfcd[1]	=	_atoh( (LPSTR)(LPCTSTR)m_pNPb->m_pNpvol->m_npsbec );

	NP_FURIDTA	np = {0};

	if( m_pNPb->m_pNpsszrc->MoveFirstEx() == 0 ){

		do {
			np.nf_swno	=	m_pNPb->m_pNpsszrc->m_npscno;
			np.nf_scod	=	_atoh( (LPSTR)(LPCTSTR)m_pNPb->m_pNpsszrc->m_npscod );
			np.nf_att	=	m_pNPb->m_pNpsszrc->m_npsatt;
			np.nf_sg0	=	m_pNPb->m_pNpsszrc->m_npssg[0];
			::ZeroMemory( np.nf_name, sizeof np.nf_name );
			strcpy_s( np.nf_name, sizeof np.nf_name, m_pNPb->m_pNpsszrc->m_npsnam );

			np.nf_sszc	=	_atoh( (LPSTR)(LPCTSTR)m_pNPb->m_pNpsszrc->m_npsszc );
			np.nf_sifc	=	_atoh( (LPSTR)(LPCTSTR)m_pNPb->m_pNpsszrc->m_npsifc );

			m_NPfuri.Add( np );
		} while( m_pNPb->m_pNpsszrc->MoveNextEx() == 0 );
	}

	m_SCHskn.RemoveAll();
	DWORD	dwCode;

	if( IsGakkouH27Master(m_pZm) ) {
		if( m_pNPb->m_pNpknrec->MoveFirstEx() == 0 ){
			do {
				if( (m_pNPb->m_pNpknrec->m_npksg[8]&0x01) ) {
					dwCode	=	_atoh( (LPSTR)(LPCTSTR)m_pNPb->m_pNpknrec->m_npkncd );
					m_SCHskn.Add( dwCode );
				}
			} while( m_pNPb->m_pNpknrec->MoveNextEx() == 0 );
		}
	}
}

// 内部取引 名称セット
void CSelectSub::nbtori_set()
{
	m_bNPanbn = FALSE;

	if( IsSyafuKaisei( m_pZm->zvol ) ) { 
		m_NBdta.RemoveAll();
		NBTORI_DTA	nbd = {0};

#ifdef OLD_CLOSE // 05.16 /12
		if( m_pNPb->m_pNpsszrc->MoveFirstEx() == 0 ){

			do {
				nbd.nbt_code	=	m_pNPb->m_pNpsszrc->m_npscno;
				::ZeroMemory( nbd.nbt_name, sizeof nbd.nbt_name );
				strcpy_s( nbd.nbt_name, sizeof nbd.nbt_name, m_pNPb->m_pNpsszrc->m_npsnam );
#ifndef NAIBU_VER2
				if( nbd.nbt_code >= 4 )	continue;
#endif
				m_NBdta.Add( nbd );
			} while( m_pNPb->m_pNpsszrc->MoveNextEx() == 0 );
		}
#endif
		for( int i = 0; NLdata[i].nb_code != -1; i++ ) {
			nbd.nbt_code = NLdata[i].nb_code;
			strcpy_s( nbd.nbt_name, sizeof nbd.nbt_name,  NLdata[i].nb_name );
			m_NBdta.Add( nbd );
		}

		if( IsSyafuKaiseiAnbun( m_pZm ) ) {
			m_bNPanbn = TRUE;
		}
	}
}

BOOL CSelectSub::IsMasterNpdata()
{
	if( ! IsKouekiMaster( m_pZm ) && ! IsSyafuKaisei( m_pZm->zvol ) &&
		! IsGakkouMaster( m_pZm) ) {
		return FALSE;
	}
	return TRUE;
}


// 公益 データ更新
void CSelectSub::update_npdata()
{
	m_bNPanbn = FALSE;

	if( m_pNPb == NULL )
		return;

	if( ! IsMasterNpdata() )
		return;

	m_pNPb->m_pNpvol->RequeryEx();
	m_pNPb->m_pNpsszrc->RequeryEx();
	m_pNPb->m_pNpknrec->RequeryEx();

	npdata_set();
	nbtori_set();
}

// 公益クラス　クローズ
int CSelectSub::_close_npsub()
{
	if( m_pNPb == NULL )
		return 0;

	// 当モジュール内でメモリー確保か？
	if( ! m_bNpbAlloc ) {
		if( m_NPopn & 0x01 ) {
			if( m_pNPb->NpVolumeClose() == ERR ) {
				return -1;
			}
		}
		if( m_NPopn & 0x02 ) {
			if( m_pNPb->NpShiteiKamokuZanClose() == ERR ) {
				return -1;
			}
		}
		return 0;
	}

	if( m_pNPb->NpVolumeClose() == ERR ) {
		return -1;
	}

	if( m_pNPb->NpShiteiKamokuZanClose() == ERR ) {
		return -1;
	}

	if( m_pNPb != NULL ) {
		delete m_pNPb;
		m_pNPb = NULL;
	}
	
	return 0;
}


// 内部データ更新処理
int CSelectSub::UpdateSelectData()
{
	// 科目名称
	_init_kamoku(1);
	// 部門
	_init_bumon();

	// 変動事由
	if( _init_hjiyu() != 0 )
		return -1;

	// 工事名称
	if( _init_koji() != 0 )
		return -1;

	// 公益法人
	update_npdata();

	return 0;
}


// 終了処理
void CSelectSub::CloseSub()
{
	m_BrnTkyo.brntky_close();
	m_Kjn.KjNameClose();

	_close_npsub();
}


// 摘要処理番号 返送
int CSelectSub::get_tkyotypeno( int type ) 
{
	if( type == 0 ) {	// 連想摘要
		return (m_TkyMode == SL_TKY_RENSO) ? 1 : 2;
	}
	else {				// 仕訳対応摘要
		return (m_TkyMode == SL_TKY_SIWAKE) ? 1 : 2;
	}
}

#define RENTKNO		get_tkyotypeno( 0 )
#define SWKTKNO		get_tkyotypeno( 1 )
#define KANATKNO	3

struct _SelTable* CSelectSub::_GetNowFix8Select()
{
	return SelFix8;
}

struct _SelTable* CSelectSub::_GetNowSel32Select()
{
	return SelVal32;
}

COLORREF CSelectSub::_GetSel32TextColor()
{
	if( m_spp.sp_nowpn == SL_BRNTKY_PN ) {
		return m_BRNTKY.bt_txtcol;
	}
	else {
		return RGB_VAL32_FORE;
	}
}

COLORREF CSelectSub::_GetSel32BackColor()
{
	if( m_spp.sp_nowpn == SL_BRNTKY_PN ) {
		return BRTEK_BackCOL;
	}
	else {
		return RGB_VAL32_BACK;
	}
}

//-------------------------------------------------------------------
//	科目選択のデータをチェック
//	struct _KamokuData& ans ..... 選択結果格納
//	struct _KamokuData& check ... 選択文字列情報
//	int _dcsw ................... 0 = 借方, 1 = 貸方
//	BOOL bSelClick .............. 選択欄をクリック
//
//-------------------------------------------------------------------
BOOL CSelectSub::_InputItemCheck(struct _KamokuData& ans, struct _KamokuData& check, int _dcsw, BOOL bSelClick/*= FALSE*/ )
{
	int found_i;
//	unsigned short SCOD;
//	unsigned short code, brn;
	CString code;
	int brn, i, bmn;
	int seln;
	int dcsw = _dcsw > 0 ? 1 : 0;
	CString km_cod;

	found_i = 1;
//	set_brn( 0xffff );

	// 06.30 /04 -- コード表示時は 2桁 入力でも コード入力として扱う
	BOOL bCodeInput = FALSE;
	BOOL bKamokuCode = (m_KmkMode == SL_KMK_CODE);

	int kamoku = atoi( check.kd_code );
	::ZeroMemory( &ans, sizeof(struct _KamokuData) );

	if( bKamokuCode && ! bSelClick ) {
		if( ! m_BrnTkySel || ( m_BrnTkySel && kamoku < 9 ) ) {
			if( check.kd_sgn == 1 || check.kd_sgn == 5 ) {
				if( m_NowPar.sel_pn != SL_KMINPCD_PN ) {	// 03.28 /09
					bCodeInput = TRUE;
					char tmp[20];
					// kamokucode check
					sprintf_s( tmp, sizeof tmp, "%08d", kamoku );
					memcpy( check.kd_code, tmp, KMKCD_LN );
				}
			}
		}
	}

	DBKNREC* pKn = NULL;

	// コード入力
	if( !check.kd_sgn || check.kd_sgn == 4 || bCodeInput )
	{
		int inpcd = atoi( check.kd_code );
	
/*- '09.02.23 -*/
//		for( i = 0; i < m_pZm->knm_info->reccount; i++ ) {
//			if( inpcd == (m_pZm->pKnrec + i)->knicod ) {
//				if( ! ((m_pZm->pKnrec+i)->knvoid & 0x01) ) {
//					pKn = (m_pZm->pKnrec + i);
//					break;
//				}
//			}
//		}
/*-------------*/
		SelPar par;
		par.sel_pn = SL_KMINPCD_PN;
		::ZeroMemory( par.kmk_kana, sizeof par.kmk_kana );
		SelParEx parex;
		::ZeroMemory( &parex, sizeof SelParEx );
		parex.kmk_inpcd = inpcd;
		// 表示する科目が一つだけの時には、その科目名称レコードを返す
		int st = 0;
		if( (st=_SelectJob(&par, -1, &parex)) == 1 ){
			for( i = 0; i < m_pZm->knm_info->reccount; i++ ) {
				// 業種の制限
				if( inpcd == (m_pZm->pKnrec + i)->knicod ) {
					if( !((m_pZm->pKnrec+i)->knvoid & 0x01) && check_kamokurestrict((m_pZm->pKnrec+i)) ){
						pKn = (m_pZm->pKnrec + i);
						break;
					}
				}
			}
		}
		else if( st == 0 ){
			// 複数サイン
			ans.kd_code[0] = (char)0xff;
		}
/*-------------*/
	}
	//番号選択
	else if( check.kd_sgn == 1 || check.kd_sgn == 5 )
	{
	//	seln = check.chk_kmk.kamoku;
		seln = atoi( check.kd_code );
		int kmseq = 0;

		if( !m_BrnTkySel || seln < 9 )
		{
			if( seln < 1 )
				found_i = 0;
			else if( seln > 8 && m_ACsel ) { /* KN50 search */
				if(Order_flg){ //091228
					if( seln >= m_ACcnt+1-AC_MAX*(m_ACsel-1) )
						found_i = 0;
					else
						kmseq = KCD50C[seln-1 + AC_MAX*(m_ACsel-1)].kd_seq;
				}
				else{
					if( seln >= m_ACcnt+9-AC_MAX*(m_ACsel-1) )
						found_i = 0;
					else
						kmseq = KCD50C[seln-9 + AC_MAX*(m_ACsel-1)].kd_seq;
				}
			}
			else {
				if(Order_flg){ //091228
					if( seln <= 8 && m_ACsel){
						if( seln >= m_ACcnt+1-AC_MAX*(m_ACsel-1) )
						found_i = 0;
						else
						kmseq = KCD50C[seln-1 + AC_MAX*(m_ACsel-1)].kd_seq;
					///	kmseq = KCD50C[seln-1 + AC_MAX*(m_ACsel-1)].kd_seq;
					}
					else {
						if( seln > 40/*m_KPGcnt + 9*/ )
							found_i = 0;
						else
							kmseq = KDPAG[m_CDPG-1][seln-1];

					}
				}
				else{
					if( seln <= 8 )
						kmseq = FIXTBL[seln-1];
					else {
						if( seln > 40/*m_KPGcnt + 9*/ )
							found_i = 0;
						else
							kmseq = KDPAG[m_CDPG-1][seln-9];
					}
				}
			}
			if( found_i ) {
				for( i = 0; i < m_pZm->knm_info->reccount; i++ ) {
					if( kmseq-1 == i ) {
						pKn = (m_pZm->pKnrec + i);
						break;
					}
				}
			}
		}
		else
		{
			if( seln > 8 )
			{
				if( brntky_get( &bmn, &code, &brn, &m_BRNTKY, seln - 8 ) )
				{
					pKn = DB_PjisToKnrec( code, m_pZm );
			//		KCOD[dcsw] = pjsksrc( code );
					check.kd_eda = brn;
			//		set_brn( brn );
				}
				else
					found_i = 0;
			}
			else
				found_i = 0;

		}
	}
	// 枝番のみ
	else	// check.IP_CHKCdNumSg == 2
	{
///		SCOD = check.kd_code;
		km_cod = check.kd_code;

		// 02.22 /02
//		if( INP_mode == _APPEND && ! check.kd_code[0] ) {
//			CString tmp_str;
//			tmp_str = !dcsw ? SREC.m_dbt : SREC.m_cre;
//			km_cod = tmp_str;
//		}
		
		pKn = DB_PjisToKnrec( km_cod, m_pZm );
	//	KCOD[dcsw] = pjsksrc( km_cod );
	}

	if( ! pKn || !found_i )
	{
		return( FALSE );
	}
	else
	{
		if( ! check_kamokurestrict( pKn ) )
			return FALSE;

		if( getKamokucodeSign() ) {
			kmkcode_string( 2, pKn, ans.kd_name, sizeof ans.kd_name );
		}
		else {
			memmove( (char *)ans.kd_name, (LPCTSTR)pKn->knnam, KMNLEN );
		}
		DWORDcodeToSTRcode( pKn->kncod, ans.kd_code, sizeof ans.kd_code );
	//	memcpy( ans.kd_code, pKn->kncod, KMKCD_LN );
		ans.kd_eda = check.kd_eda;

		return( TRUE );
	}

	return FALSE;
}


//
//	選択データ取得
//	return TRUE --- 選択結果取得OK
//
BOOL CSelectSub::_GetSelResult( SelResult* result, char* selstr )
{
	int sel_no, sel_ok;
	int max, bmn;
	BOOL ret = FALSE;
	
	switch( result->sel_pn ) {
	case SL_BRNTKY_PN:
		if( m_BrnTkySel ) {
			CString code;
			int brn, no;

			no = atoi( selstr )- 8;	

			if( brntky_get( &bmn, &code, &brn, &m_BRNTKY, no) ) {
				result->brn_bmn = bmn;
				result->km_code = code;
				result->brn_no = brn;
		
				BRN_NAME* bnam;
				if( bnam = m_BrnTkyo.get_brtekmsg( bmn, code, brn ) ) {
					result->brn_name = bnam->Name;
				}
				ret = TRUE;
			}
		}
		break;
	case SL_BUMON_PN:	
		sel_no = atoi( selstr );
		if( (sel_no - /*(isEntrySys() ? 9 : 1)*/ 1) >= 0 )
		{
			if( _GetBmonSelect( result, m_BmnPG, sel_no ) )
				ret = TRUE;
		}
		break;
	case SL_KOJI_PN	:
		sel_no = atoi( selstr );
		if( (sel_no - /*(isEntrySys() ? 9 : 1)*/ 1) >= 0 )
		{
			if( _GetKojiSelect( result, m_KojiPG, sel_no ) )
				ret = TRUE;
		}
		break;
			break;
	case SL_TKY_PN:
	case SL_TKYKANA_PN:
	case SL_TKYALL_PN:
		//摘要コード
		sel_ok = 1;
		if( strlen( selstr ) > 2 )
		{
			sel_ok++;	//摘要コード入力
			if( (sel_no = atoi( selstr )) <= 0 )
			{
				sel_ok = 0;
			}
		}
		else	//番号選択
		{
			max = sizeof TKSEL_PG / sizeof TKSEL_PG[0];
		
			if( (sel_no = atoi( selstr ) - /*(isEntrySys() ? 9 : 1)*/ 1 ) >= 0 )
			{
				if( sel_no > max ) sel_ok = 0;
				else {
					if( m_TkySign == RENTKNO ) { /* ﾚﾝｿｳ ﾃｷﾖｳ ﾋｮｳｼﾞ */
						sel_no = TKSEL_PG[sel_no];
					}
					else if( m_TkySign == SWKTKNO ) {	 /* ｼﾞﾄﾞｳｼﾜｹﾃｷﾖｳ */
						if( m_SW_AutoTK == 0 /*|| sel_no >= m_TATdcnt*/ ) 
							sel_ok = 0;
						else	sel_no = TKSEL_PG[sel_no];
					}
					else if( m_TkySign == KANATKNO ) {  /* 50ｵﾝ ﾃｷﾖｳ ﾋｮｳｼﾞ */
						if( m_SW_TK50 == 0 /*|| sel_no >= m_T50dcnt*/ ) 
							;
						sel_no = TKSEL_PG[sel_no];
					}	
				}
			}
			else {
				sel_ok = 0;
			}
		}
		if( sel_ok ) {

			if( m_TkySign == KANATKNO ) {  /* 50ｵﾝ ﾃｷﾖｳ ﾋｮｳｼﾞ */
				if( sel_ok == 1 ) {
					if( DB_KanaTekiyoNoRead( sel_no ) == 0 ) {
						result->tky_code = m_pZm->tkrec->tkcod;
						result->tky = m_pZm->tkrec->tkname;
						// スペースカット
						LPSTR	p = result->tky.GetBuffer(256);
						int n = kjlen( p, m_TKYO_LEN );
						*(p + n) = '\0';
						result->tky.ReleaseBuffer();

						ret = TRUE;
					}
				}
				else {
					if( DB_TekiyoNoRead( sel_no ) == 0 ) {
						result->tky_code = m_pZm->tkrec->tkcod;
						result->tky = m_pZm->tkrec->tkname;
						// スペースカット
						LPSTR	p = result->tky.GetBuffer(256);
						int n = kjlen( p, m_TKYO_LEN );
						*(p + n) = '\0';
						result->tky.ReleaseBuffer();

						ret = TRUE;
					}
				}
			}
			else {
				// データベース 摘要読み込み
				if( DB_TekiyoNoRead( sel_no ) == 0 ) {
					result->tky_code = m_pZm->tkrec->tkcod;
					result->tky = m_pZm->tkrec->tkname;
					// スペースカット
					LPSTR	p = result->tky.GetBuffer(256);
					int n = kjlen( p, m_TKYO_LEN );
					*(p + n) = '\0';
					result->tky.ReleaseBuffer();

					ret = TRUE;
				}
			}
		}
		break;

	case SL_HJIYU_PN:
		if( (sel_no = atoi( selstr )) > 0 )
		{
			if( _GetHjiyuSelect( result, m_HjiyuPG, sel_no ) )
				ret = TRUE;
		}
		break;
	// 公益 振替区分
	case SL_NPFURI_PN:
		if( (sel_no = atoi( selstr )) > 0 )
		{
			if( _GetNPfuriSelect( result, m_NPpage, sel_no ) )
				ret = TRUE;
		}
		break;
	// 内部取引消去
	case SL_NAIBU_PN:
		if( (sel_no = atoi( selstr )) > 0 )
		{
			if( _GetNaibuSelect( result, m_NaibuPage, sel_no ) )
				ret = TRUE;
		}
		break;
#ifdef _SLIP_ACCOUNTS_
	// 原票会計
	case SL_SLIP_TKY_PN:
/*- '12.10.24 -*/
//		if( (sel_no = atoi( selstr )) > 0 ){
//			if( _GetImgStrSelect( result, m_SlipTkyPage, sel_no ) )
//				ret = TRUE;
//		}
/*-------------*/
		sel_ok = 1;
		if( strlen( selstr ) > 2 )
		{
			sel_ok++;	//摘要コード入力
			if( (sel_no = atoi( selstr )) <= 0 )
			{
				sel_ok = 0;
			}
		}
		else	//番号選択
		{
			max = sizeof TKSEL_PG / sizeof TKSEL_PG[0];
		
			if( (sel_no = atoi( selstr )) > 0 ){
				if( _GetImgStrSelect( result, m_SlipTkyPage, sel_no ) ){
					ret = TRUE;
			//		sel_ok = 0;
				}
				sel_ok = 0;
			}
		}
		if( sel_ok ) {

			if( m_TkySign == KANATKNO ) {  /* 50ｵﾝ ﾃｷﾖｳ ﾋｮｳｼﾞ */
				if( sel_ok == 1 ) {
					if( DB_KanaTekiyoNoRead( sel_no ) == 0 ) {
						result->tky_code = m_pZm->tkrec->tkcod;
						result->tky = m_pZm->tkrec->tkname;
						// スペースカット
						LPSTR	p = result->tky.GetBuffer(256);
						int n = kjlen( p, m_TKYO_LEN );
						*(p + n) = '\0';
						result->tky.ReleaseBuffer();

						ret = TRUE;
					}
				}
				else {
					if( DB_TekiyoNoRead( sel_no ) == 0 ) {
						result->tky_code = m_pZm->tkrec->tkcod;
						result->tky = m_pZm->tkrec->tkname;
						// スペースカット
						LPSTR	p = result->tky.GetBuffer(256);
						int n = kjlen( p, m_TKYO_LEN );
						*(p + n) = '\0';
						result->tky.ReleaseBuffer();

						ret = TRUE;
					}
				}
			}
			else {
				// データベース 摘要読み込み
				if( DB_TekiyoNoRead( sel_no ) == 0 ) {
					result->tky_code = m_pZm->tkrec->tkcod;
					result->tky = m_pZm->tkrec->tkname;
					// スペースカット
					LPSTR	p = result->tky.GetBuffer(256);
					int n = kjlen( p, m_TKYO_LEN );
					*(p + n) = '\0';
					result->tky.ReleaseBuffer();

					ret = TRUE;
				}
			}
		}
/*-------------*/
		break;
#endif

	}

	return ret;
}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
//		当面、部門枝番を選択画面には表示しない
//		そのため、枝番名称を検索する際の 部門コードには、-1を指定
//
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※

#ifdef _20090309_EXTEND_
//--------------------------------------------------------------
//	選択処理を行う
//	SelPar* par ----- 選択表示パラメータ
//	int nChar　------ キー
//
//	返送値 科目カナ検索で、該当なしの場合は、-1
//--------------------------------------------------------------
int CSelectSub::_SelectJob( SelPar* par, int nChar/*=-1*/ )
{
	int prev_pn, rval;
	prev_pn = m_spp.sp_nowpn;
	m_spp.sp_prevpn = prev_pn;
	m_spp.sp_nowpn = par->sel_pn;
	BOOL ret, sg;

	rval = 0;
	// 現在処理情報をセーブ
	m_NowPar = *par;

	switch( par->sel_pn ) {
	case SL_KAMOKU_PN:
		_KamokuSelect( nChar );
		break;
	case SL_KMKANA_PN:
		if( nChar == -1 ) {
			if( kmk_50search( par->kmk_kana ) ) {
				::ZeroMemory( svKana, sizeof svKana );
				memcpy( svKana, par->kmk_kana, sizeof par->kmk_kana );
				m_spp.sp_prevpn = prev_pn;
				m_spp.sp_nowpn = m_spp.sp_nowpn;
				m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
				m_BrnTkySel = 0;
			}
			else {
				// ::ZeroMemory( svKana, sizeof svKana );
				// 検索結果なしの場合は、デフォルトの表示にもどす
				//	_KamokuSelect( -1 );
				rval = -1;
			}
		}
		else {
			_KamokuSelect( nChar );
		}
		break;
	case SL_KMINPCD_PN:
		if( nChar == -1 ) {
			int st;
			if( (st=kmk_inpcodesearch(par->kmk_inpcd)) == 0 ){
				::ZeroMemory( svKana, sizeof svKana );
				memcpy( svKana, par->kmk_kana, sizeof par->kmk_kana );
				m_spp.sp_prevpn = prev_pn;
				m_spp.sp_nowpn = m_spp.sp_nowpn;
				m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
				m_BrnTkySel = 0;
			}
			else if( st == 1 ){
				rval = 1;
			}
			else {
				// ::ZeroMemory( svKana, sizeof svKana );
				// 検索結果なしの場合は、デフォルトの表示にもどす
				//	_KamokuSelect( -1 );
				rval = -2;
			}
		}
		else {
			_KamokuSelect( nChar );
		}
		break;
	case SL_BRNTKY_PN:
		if( nChar == -1 ) {
			if( m_BrnTkySel && m_BRNTKY.bt_code == par->tkybrn ) {
				if( strcmp( m_BRNTKY.bt_kana, par->brn_kana ) != 0 ) 
					ret = brntky_sch( &m_BRNTKY, SelVal32, -1/*par->brn_bmn*/, par->tkybrn, par->brn_kana, par->sel_flg );
				else
					ret = brntky_page( &m_BRNTKY, SelVal32, m_BRNTKY.bt_page );
			}
			else {
				ret = brntky_sch( &m_BRNTKY, SelVal32, -1/*par->brn_bmn*/, par->tkybrn, par->brn_kana, par->sel_flg );
			}
			
			if( ! ret )
				sg = FALSE;
			else {
				sg = TRUE;
				m_BrnTkySel = 1;
			}
			// NOW パラメータセット 10.09 /08
			if( sg == FALSE ) {
				m_spp.sp_prevpn = SL_BRNTKY_PN;
				m_spp.sp_nowpn = SL_KAMOKU_PN;
				m_NowPar = *par;
			}

			if( sg == FALSE && m_BrnTkySel )
			{
				m_BrnTkySel = 0;
			//	fixvalcnd( !isfixvalcnd( pn ));
			//	disp_change();

				m_spp.sp_prevpn = SL_BRNTKY_PN;
				m_spp.sp_nowpn = SL_KAMOKU_PN;
				//科目・摘要選択画面リセット
				SelDspRst();
			}
		}
		else {
			_KamokuSelect( nChar );
		}
		break;
	case SL_BUMON_PN:	
		_BmonSelect( nChar );
		break;
	case SL_KOJI_PN:
		::ZeroMemory( svKana, sizeof svKana );
		if( nChar == -1 )
			_koji_search();

		_KojiSelect( nChar );
		break;
	case SL_KOJIKANA_PN:
		::ZeroMemory( svKana, sizeof svKana );
		memcpy( svKana, m_NowPar.kmk_kana, sizeof m_NowPar.kmk_kana );

		if( nChar == -1 )
			_koji_search();
		_KojiSelect( nChar );
		break;

	case SL_TKY_PN:	
	case SL_TKYKANA_PN:
	case SL_TKYALL_PN:
		if( nChar == -1 ) {
			if( par->sel_pn == SL_TKYALL_PN ) {
				::ZeroMemory( svTkyKana, sizeof svTkyKana );
				tky_select50_dsp( NULL );
				m_Debcd = par->dbt;
				m_Crecd = par->cre;
			}
			else {
				if( par->tky_kana[0] ) {
					::ZeroMemory( svTkyKana, sizeof svTkyKana );
					memcpy( svTkyKana, par->tky_kana, sizeof par->tky_kana );

					tky_select50_dsp( par->tky_kana );
				}
				else {
					::ZeroMemory( svTkyKana, sizeof svTkyKana );
					m_TkySign = 0;
					m_Debcd = par->dbt;
					m_Crecd = par->cre;
					if( rentky_srdsp( par->dbt, par->cre ) == -1 ) //連想摘要がない場合は50音を表示
						SelTkyDsp( VK_HOME );
				}
			}
		}
		else  {
			SelTkyDsp( nChar );
		}
		break;

	case SL_HJIYU_PN:
		_HjiyuSelect( nChar );
		break;
	// 公益法人 振替科目
	case SL_NPFURI_PN:
		_NPfuriSelect( nChar );
		break;
	// 公益法人 収支科目
	case SL_NPSYUS_PN:
		_SyusSelect( nChar );
		break;
	// 公益法人 収支カナ検索
	case SL_NPSYKANA_PN:
		if( nChar == -1 ) {
			if( Syus_50search( par->kmk_kana ) ) {
				::ZeroMemory( svKana, sizeof svKana );
				memcpy( svKana, par->kmk_kana, sizeof par->kmk_kana );
				m_spp.sp_prevpn = prev_pn;
				m_spp.sp_nowpn = m_spp.sp_nowpn;
				m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
			}
			else {
				// ::ZeroMemory( svKana, sizeof svKana );
				// 検索結果なしの場合は、デフォルトの表示にもどす
				//	_KamokuSelect( -1 );
				rval = -1;
			}
		}
		else {
			_SyusSelect( nChar );
		}
		break;
	case SL_NPSKMINPCD_PN:
		if( nChar == -1 ) {
			int st;
			if( (st=Syus_inpcodesearch(par->kmk_inpcd)) == 0 ){
				::ZeroMemory( svKana, sizeof svKana );
				memcpy( svKana, par->kmk_kana, sizeof par->kmk_kana );
				m_spp.sp_prevpn = prev_pn;
				m_spp.sp_nowpn = m_spp.sp_nowpn;
				m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
			}
			else if( st == 1 ){
				rval = 1;
			}
			else {
				// ::ZeroMemory( svKana, sizeof svKana );
				// 検索結果なしの場合は、デフォルトの表示にもどす
				//	_KamokuSelect( -1 );
				rval = -2;
			}
		}
		else {
			_SyusSelect( nChar );
		}
		break;

	case SL_RESET:
		m_spp.sp_nowpn = prev_pn;
		SelDspRst();
		break;
	default:
		m_spp.sp_nowpn = prev_pn;
		break;
	}

	// カナがないなど前のポジションに戻った場合
	m_NowPar.sel_pn = m_spp.sp_nowpn;

	if( nChar != -1 ) {
		memcpy( m_NowPar.kmk_kana, svKana, sizeof m_NowPar.kmk_kana );
		memcpy( m_NowPar.tky_kana, svTkyKana, sizeof m_NowPar.tky_kana );
	}

	return rval;
}
#endif

//--------------------------------------------------------------
//	選択処理を行う
//	SelPar* par		----- 選択表示パラメータ
//	int nChar　		----- キー
//	SelParEx* parex	----- 選択表示パラメータ拡張
//
//	返送値 科目カナ検索で、該当なしの場合は、-1
//　　　　 科目入力検索で、該当科目が一科目のみの場合は、1
//　　　　 科目入力検索で、該当なしの場合は、-2
//--------------------------------------------------------------
int CSelectSub::_SelectJob( SelPar* par, int nChar/*=-1*/, SelParEx* parex/* = NULL*/ )
{
	int prev_pn, rval;
	prev_pn = m_spp.sp_nowpn;
	m_spp.sp_prevpn = prev_pn;
	m_spp.sp_nowpn = par->sel_pn;
	BOOL ret, sg;

	rval = 0;
	// 現在処理情報をセーブ
	m_NowPar = *par;
	if( parex ){
		m_NowParEx = *parex;
	}
	else{
		::ZeroMemory( &m_NowParEx, sizeof SelParEx );
	}

	switch( par->sel_pn ) {
	case SL_KAMOKU_PN:

		_KamokuSelect( nChar );
		break;
	case SL_KMKANA_PN:
		if( nChar == -1 ) {
			if( kmk_50search( par->kmk_kana ) ) {
				::ZeroMemory( svKana, sizeof svKana );
				memcpy( svKana, par->kmk_kana, sizeof par->kmk_kana );
				m_spp.sp_prevpn = prev_pn;
				m_spp.sp_nowpn = m_spp.sp_nowpn;
				if(Order_flg)	m_fixproc( SelFix8, &m_spp, 8, m_pWnd );//091228
				m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
				m_BrnTkySel = 0;
			}
			else {
				// ::ZeroMemory( svKana, sizeof svKana );
				// 検索結果なしの場合は、デフォルトの表示にもどす
				//	_KamokuSelect( -1 );
				rval = -1;
			}
		}
		else {
			_KamokuSelect( nChar );
		}
		break;
	case SL_KMINPCD_PN:
		if( nChar == -1 ) {
			int st;

			if( (st=kmk_inpcodesearch(m_NowParEx.kmk_inpcd)) == 0 ){
				::ZeroMemory( svKana, sizeof svKana );
				memcpy( svKana, par->kmk_kana, sizeof par->kmk_kana );
				m_spp.sp_prevpn = prev_pn;
				m_spp.sp_nowpn = m_spp.sp_nowpn;
				if(Order_flg)	m_fixproc( SelFix8, &m_spp, 8, m_pWnd );//091228
				m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
				m_BrnTkySel = 0;
			}
			else if( st == 1 ){
				rval = 1;
			}
			else {
				// ::ZeroMemory( svKana, sizeof svKana );
				// 検索結果なしの場合は、デフォルトの表示にもどす
				//	_KamokuSelect( -1 );
				rval = -2;
			}
		}
		else {
			_KamokuSelect( nChar );
		}
		break;
	case SL_BRNTKY_PN:
		if( nChar == -1 ) {
			// 02.08 /13
			int bmncd = -1;
			if( (m_dwOPT&0x02))	bmncd = par->brn_bmn;

			if( m_BrnTkySel && m_BRNTKY.bt_code == par->tkybrn ) {
				if( strcmp( m_BRNTKY.bt_kana, par->brn_kana ) != 0 ) 
					ret = brntky_sch( &m_BRNTKY, SelVal32, bmncd/*par->brn_bmn*/, par->tkybrn, par->brn_kana, par->sel_flg );
				else
					ret = brntky_page( &m_BRNTKY, SelVal32, m_BRNTKY.bt_page );
			}
			else {
				ret = brntky_sch( &m_BRNTKY, SelVal32, bmncd/*par->brn_bmn*/, par->tkybrn, par->brn_kana, par->sel_flg );
			}

			if( ! ret )
				sg = FALSE;
			else {
				sg = TRUE;
				m_BrnTkySel = 1;
			}
			// NOW パラメータセット 10.09 /08
			if( sg == FALSE ) {
				m_spp.sp_prevpn = SL_BRNTKY_PN;
				m_spp.sp_nowpn = SL_KAMOKU_PN;
				m_NowPar = *par;
			}

			if( sg == FALSE && m_BrnTkySel )
			{
				m_BrnTkySel = 0;
			//	fixvalcnd( !isfixvalcnd( pn ));
			//	disp_change();

				m_spp.sp_prevpn = SL_BRNTKY_PN;
				m_spp.sp_nowpn = SL_KAMOKU_PN;
				//科目・摘要選択画面リセット
				SelDspRst();
			}

		}
		else {
			_KamokuSelect( nChar );
		}
		break;
	case SL_BUMON_PN:
		_BmonSelect( nChar );
		break;
	case SL_BMNKANA_PN:
		if( nChar == -1 ) {
			::ZeroMemory( svBmnKana, sizeof svBmnKana );
			memcpy( svBmnKana, m_NowPar.kmk_kana, sizeof m_NowPar.kmk_kana );
		}

		_BmonSelect( nChar );
		break;

	case SL_KOJI_PN:
		::ZeroMemory( svKana, sizeof svKana );
		if( nChar == -1 )
			_koji_search();

		_KojiSelect( nChar );
		break;
	case SL_KOJIKANA_PN:
		::ZeroMemory( svKana, sizeof svKana );
		memcpy( svKana, m_NowPar.kmk_kana, sizeof m_NowPar.kmk_kana );
		if( nChar == -1 ) {
			m_KjKana = 0;
			_koji_search();
		}
		else if( nChar == VK_HOME ) {
			m_KjKana = 1;
		}
		_KojiSelect( nChar );
		break;

	case SL_TKY_PN:	
	case SL_TKYKANA_PN:
	case SL_TKYALL_PN:
#ifdef _SLIP_ACCOUNTS_
	case SL_SLIP_TKY_PN:
#endif
		if( nChar == -2 ) {	// 12.04 /12
			if( par->sel_pn == SL_TKYALL_PN ) {
				::ZeroMemory( svTkyKana, sizeof svTkyKana );
				m_Debcd = par->dbt;
				m_Crecd = par->cre;

				// 連想摘要 か 仕訳摘要
				for( int dcsw = 0; dcsw < 2; dcsw++ ) {
					m_RTKblk[dcsw] = -1;
					m_RTKcnt[dcsw] = 0;
				}
				tky_atswk_dsp( m_Debcd, m_Crecd, FALSE );
				_rentky_str( m_Debcd, m_Crecd);
			}
		}
		else if( nChar == -1 ) {
			if( par->sel_pn == SL_TKYALL_PN ) {
				::ZeroMemory( svTkyKana, sizeof svTkyKana );
				tky_select50_dsp( NULL );
				m_Debcd = par->dbt;
				m_Crecd = par->cre;

				// 連想摘要・仕訳摘要 更新　[12.05 /12]
				for( int dcsw = 0; dcsw < 2; dcsw++ ) {
					m_RTKblk[dcsw] = -1;
					m_RTKcnt[dcsw] = 0;
				}
				tky_atswk_dsp( m_Debcd, m_Crecd, FALSE );
				_rentky_str( m_Debcd, m_Crecd);
			}
#ifdef _SLIP_ACCOUNTS_
			else if( par->sel_pn == SL_SLIP_TKY_PN ){
				// この内部でイメージ管理クラスから表示用文字列を取得
				::ZeroMemory( svTkyKana, sizeof svTkyKana );
				_SlipTkySelect( nChar );
				m_Debcd = par->dbt;
				m_Crecd = par->cre;
			}
#endif
			else {
				if( par->tky_kana[0] ) {
					::ZeroMemory( svTkyKana, sizeof svTkyKana );
					memcpy( svTkyKana, par->tky_kana, sizeof par->tky_kana );

					tky_select50_dsp( par->tky_kana );
				}
				else {
					::ZeroMemory( svTkyKana, sizeof svTkyKana );
					m_TkySign = 0;
					m_Debcd = par->dbt;
					m_Crecd = par->cre;
					if( rentky_srdsp( par->dbt, par->cre ) == -1 ) //連想摘要がない場合は50音を表示
						SelTkyDsp( VK_HOME );
				}
			}
		}
		else  {
#ifndef _SLIP_ACCOUNTS_
			SelTkyDsp( nChar );
#else
			if( par->sel_pn != SL_SLIP_TKY_PN ){
				SelTkyDsp( nChar );
			}
			else{
				_SlipTkySelect( nChar );
			}
#endif
		}
		break;

	case SL_HJIYU_PN:
		_HjiyuSelect( nChar );
		break;
	// 公益法人 振替科目
	case SL_NPFURI_PN:
		_NPfuriSelect( nChar );
		break;
	// 公益法人 収支科目
	case SL_NPSYUS_PN:
		_SyusSelect( nChar );
		break;
	// 公益法人 収支カナ検索
	case SL_NPSYKANA_PN:
		if( nChar == -1 ) {
			if( Syus_50search( par->kmk_kana ) ) {
				::ZeroMemory( svKana, sizeof svKana );
				memcpy( svKana, par->kmk_kana, sizeof par->kmk_kana );
				m_spp.sp_prevpn = prev_pn;
				m_spp.sp_nowpn = m_spp.sp_nowpn;
				if(Order_flg)	m_fixproc( SelFix8, &m_spp, 8, m_pWnd );//091228
				m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
			}
			else {
				// ::ZeroMemory( svKana, sizeof svKana );
				// 検索結果なしの場合は、デフォルトの表示にもどす
				//	_KamokuSelect( -1 );
				rval = -1;
			}
		}
		else {
			_SyusSelect( nChar );
		}
		break;
	case SL_NPSKMINPCD_PN:
		if( nChar == -1 ) {
			int st;
			if( (st=Syus_inpcodesearch(m_NowParEx.kmk_inpcd)) == 0 ){
				::ZeroMemory( svKana, sizeof svKana );
				memcpy( svKana, par->kmk_kana, sizeof par->kmk_kana );
				m_spp.sp_prevpn = prev_pn;
				m_spp.sp_nowpn = m_spp.sp_nowpn;
				if(Order_flg)	m_fixproc( SelFix8, &m_spp, 8, m_pWnd ); //091228
				m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
			}
			else if( st == 1 ){
				rval = 1;
			}
			else {
				// ::ZeroMemory( svKana, sizeof svKana );
				// 検索結果なしの場合は、デフォルトの表示にもどす
				//	_KamokuSelect( -1 );
				rval = -2;
			}
		}
		else {
			_SyusSelect( nChar );
		}
		break;

	case SL_NAIBU_PN:
		_SOWNaibuSelect( nChar );
		break;

	case SL_KMHANI_PN:
		_SetOrder(1,-1);
		if( nChar == -1 ) {
			m_dwStart = _atoh( (LPSTR)(LPCTSTR)par->dbt );
			m_dwEnd   = _atoh( (LPSTR)(LPCTSTR)par->cre );

			_init_kamoku_oder( 0, m_dwStart, m_dwEnd );

			m_spp.sp_prevpn = prev_pn;
			m_spp.sp_nowpn = m_spp.sp_nowpn;
			::ZeroMemory( m_spp.seljob, sizeof m_spp.seljob );

			resetpag(1);

			if(Order_flg)	m_fixproc( SelFix8, &m_spp, 8, m_pWnd ); //091228
			m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
		}
		else {
			_KamokuSelect( nChar );
		}
		break;

	case SL_RESET:
		m_spp.sp_nowpn = prev_pn;
		SelDspRst();
		break;
	default:
		m_spp.sp_nowpn = prev_pn;
		break;
	}

	// カナがないなど前のポジションに戻った場合
	m_NowPar.sel_pn = m_spp.sp_nowpn;

	if( nChar != -1 ) {
		memcpy( m_NowPar.kmk_kana, svKana, sizeof m_NowPar.kmk_kana );
		memcpy( m_NowPar.tky_kana, svTkyKana, sizeof m_NowPar.tky_kana );
	}

	return rval;
}

//-------------------------------
//	現在の選択画面をUpdate
//
//-------------------------------
int	CSelectSub::_SelectUpdate()
{
	int page, ret;

	// 枝番名称・諸口枝番エリアをクリア
	m_BrnTkyo.brtek_free();
	init_brntkypar( &m_BRNTKY );

	// 工事名称の 再オープン
	if( m_pZm->zvol->apno == 0x20 ) {
		m_Kjn.KjNameClose();
		m_Kjn.KjNameOpen();
	}
	// 固定科目・選択科目の更新
	UpdateSelectData();

	switch( m_NowPar.sel_pn ) {

	case SL_KAMOKU_PN:
	case SL_KMKANA_PN:
	case SL_BUMON_PN:	
	case SL_KOJI_PN:
	case SL_KMINPCD_PN:
		SelDspRst();
		break;
	case SL_TKY_PN:	
	case SL_TKYKANA_PN:
	case SL_TKYALL_PN:
		tekiyo_update();
		break;

	case SL_BRNTKY_PN:
		// 枝番摘要再読み込み
		if( m_BrnTkySel ) {
			// 固定科目分 リセット
			fixcreat( (int *)&FIXTBL[0] );
			m_fixproc( SelFix8, &m_spp, 8, m_pWnd );

			page = m_BRNTKY.bt_page;

			// 02.08 /13
			int bmncd = -1;
			if( (m_dwOPT&0x02))	bmncd = m_NowPar.brn_bmn;

			ret = brntky_sch( &m_BRNTKY, SelVal32, bmncd/*m_NowPar.brn_bmn*/, m_NowPar.tkybrn, m_NowPar.brn_kana, m_NowPar.sel_flg );

			if( ! ret ) {
				m_BrnTkySel = 0;
				m_spp.sp_prevpn = SL_BRNTKY_PN;
				m_spp.sp_nowpn = SL_KAMOKU_PN;
				//科目・摘要選択画面リセット
				SelDspRst();
			}
			else {
				for( int i = page; i > 0; i-- ) {
					if( ret = brntky_page( &m_BRNTKY, SelVal32, page ) )
						break;
				}
			}
		}
		else {
			SelDspRst();
		}
		break;

	default:
		break;
	}

	return 0;

}


//----------------------------------------------------------
//	部門選択部分
//	int nChar ....... キーコード(-1で初期表示)
//	BOOL bNowPage ... TRUE で現在ページを表示する
//----------------------------------------------------------
int	CSelectSub::_BmonSelect( int nChar, BOOL bNowPage/*=FALSE*/ )
{
	int pg, pgcnt;

	if( nChar == -1 ) {
		if( ! bNowPage ) 
			m_BmnPG = 1;
		_BmonSelectSub( m_BmnPG );
	}
	else if( nChar == VK_NEXT ) {
		pg = m_BmnPG;
		pg++;

		int bmnmax = 0;

		// 仕訳で増えている恐れがあるので 数えなおす
		if( (m_pZm->zvol->sub_sw&0x02) ) {
		//	m_BmnMax = 0;
			m_pZm->bmname->MoveFirst();
			do {
				if( ! m_bNPanbn/*IsSyafuKaisei( m_pZm->zvol )*/ ) { 
					// 合計部門は除く
					if( (m_pZm->bmname->bnhsg[0] & 0x02) )
						continue;
				}

				// 部門カナ検索
				if( m_spp.sp_nowpn == SL_BMNKANA_PN ) {
					if( svBmnKana[0] != '\0' ) {
						int kct = strlen( svBmnKana );

						if( _strnicmp( m_pZm->bmname->bnkidx, svBmnKana, kct ) != 0 ) {
							continue;
						}
					}
				}
				bmnmax++;
			//	m_BmnMax++;
			} while( m_pZm->bmname->MoveNext() == 0 );
		}

#ifdef BMON_KANA_CLOSE
		pgcnt = (m_BmnMax / m_PageMax);
		if( (m_BmnMax % m_PageMax) )	pgcnt++;

		if( pgcnt >= pg ) { 
			m_BmnPG = pg;
			_BmonSelectSub( m_BmnPG );
		}
#endif
		pgcnt = (bmnmax / m_PageMax);
		if( (bmnmax % m_PageMax) )	pgcnt++;

		if( pgcnt >= pg ) { 
			m_BmnPG = pg;
			_BmonSelectSub( m_BmnPG );
		}
	}
	else if( nChar == VK_PRIOR ) {
		if( m_BmnPG > 1 ) {
			m_BmnPG--;
			_BmonSelectSub( m_BmnPG );
		}
	}
	else if( nChar == VK_HOME ) {	// 全検索に戻る
		::ZeroMemory( svBmnKana, sizeof svBmnKana );
		if( ! bNowPage ) 
			m_BmnPG = 1;
		_BmonSelectSub( m_BmnPG );
	}


	return 0;
}

//----------------------------------------------------------
//	部門選択データ読み込み部分
//	int page ....... 表示ページ
//----------------------------------------------------------
void CSelectSub::_BmonSelectSub( int page )
{
	CBMNAME* bn;
	int i, cnt;
	i = cnt = 0;
	char bf[36], tmp[128], fmt[36];

	strcpy_s( m_spp.seljob, sizeof m_spp.seljob, "《部門コード》" );

	if( ! (m_pZm->zvol->sub_sw&0x02) || m_pZm->bmname == NULL ) {
		goto END_TAG;
	}

	bn = m_pZm->bmname;
	bn->MoveFirst();
	if( bn->st == -1 )	goto END_TAG;

	do {
		if( ! m_bNPanbn/*IsSyafuKaisei( m_pZm->zvol )*/ ) { 
			if( (bn->bnhsg[0] & 0x02) )
				continue;
		}

		// 部門カナ検索
		if( m_spp.sp_nowpn == SL_BMNKANA_PN ) {
			if( svBmnKana[0] != '\0' ) {
				int kct = strlen( svBmnKana );

				if( _strnicmp( bn->bnkidx, svBmnKana, kct ) != 0 ) {
					continue;
				}
			}
		}

		i++;

		if( i > ((page-1) * m_PageMax) ) {
			cnt++;
			if( cnt > m_PageMax )
				break;
		
			set_codestr( bf, sizeof bf, bn->bnbcd, m_pZm->zvol->bmcol );
			sprintf_s( tmp, sizeof tmp, "%s%s", bf, bn->bnbnm );
			sprintf_s( fmt, sizeof fmt, "%%.%ds", get_strcnt( tmp, SEL_LEN ) );

			SelVal32[cnt-1].number = cnt;
			SelVal32[cnt-1].Data.Format( fmt, tmp );

		}

	} while( bn->MoveNext() == 0 );

END_TAG:

	if( cnt < m_PageMax ) {
		for( i = cnt; i < m_PageMax; i++ ) { 
			SelVal32[i].number = i+1;
			SelVal32[i].Data = "";
		}
	}

	m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
}

//-----------------------------------------------------
//	選択結果を取得
//	SelResult* result ...	選択結果格納
//	int page　...........	選択ページ
//	int sel_idx .........	選択番号(1～)
//-----------------------------------------------------
BOOL CSelectSub::_GetBmonSelect( SelResult* result, int page, int sel_no )
{
	CBMNAME* bn;

	bn = m_pZm->bmname;
	bn->MoveFirst();

	BOOL ret = FALSE;
	int i, cnt;
	i = cnt = ret = 0;

	if( sel_no > m_PageMax )
		return FALSE;

	if( bn->st == -1 )
		return FALSE;

	do {
		if( ! m_bNPanbn/*IsSyafuKaisei( m_pZm->zvol )*/ ) { 
			if( (bn->bnhsg[0] & 0x02) )
				continue;
		}

		// 部門カナ検索
		if( m_spp.sp_nowpn == SL_BMNKANA_PN ) {
			if( svBmnKana[0] != '\0' ) {
				int kct = strlen( svBmnKana );

				if( _strnicmp( bn->bnkidx, svBmnKana, kct ) != 0 ) {
					continue;
				}
			}
		}

		i++;

		if( i > ((page-1) * m_PageMax) ) {
			cnt++;
			if( cnt > m_PageMax )
				break;

			if( sel_no == cnt ) {
				ret = TRUE;
				result->bmn = bn->bnbcd;
				result->selname = bn->bnbnm;
				break;
			}
		}
	} while( bn->MoveNext() == 0 );

	return ret;
}



//----------------------------------------------------------
//	工事選択部分
//	int nChar ....... キーコード(-1で初期表示)
//	BOOL bNowPage ... TRUE で現在ページを表示する
//----------------------------------------------------------
int	CSelectSub::_KojiSelect( int nChar, BOOL bNowPage/*=FALSE*/ )
{
	int pg, pgcnt;

	if( nChar == -1 ) {
		if( ! bNowPage ) 
			m_KojiPG = 1;
		_KojiSelectSub( m_KojiPG );
	}
	else if( nChar == VK_NEXT ) {
		pg = m_KojiPG;
		pg++;

		int max;
		max = m_KjKana ? m_KjKanaMax : m_KojiMax;
		pgcnt = (max / m_PageMax);
		if( (max % m_PageMax) )	pgcnt++;

		if( pgcnt >= pg ) { 
			m_KojiPG = pg;
			_KojiSelectSub( m_KojiPG );
		}
	}
	else if( nChar == VK_PRIOR ) {
		if( m_KojiPG > 0 ) {
			m_KojiPG--;
			_KojiSelectSub( m_KojiPG );
		}
	}
	else if( nChar == VK_HOME ) {	// 全検索に戻る
		if( m_KjKana ) {
			::ZeroMemory( svKana, sizeof svKana );
			_koji_search();
			m_KojiPG = 1;
			_KojiSelectSub( m_KojiPG );
		}
	}


	return 0;
}

//----------------------------------------------------------
//	工事番号選択データ読み込み部分
//	int page ....... 表示ページ
//----------------------------------------------------------
void CSelectSub::_KojiSelectSub( int page )
{
	int i, cnt;
	i = cnt = 0;
	char bf[64], tmp[128], fmt[36];
	CString	string;

	strcpy_s( m_spp.seljob, sizeof m_spp.seljob, "《工事番号》" );

	if( ! (m_pZm->zvol->sub_sw&0x04) || m_Kjn.kjnam == NULL ) {
		goto END_TAG;
	}

	CKJKKI* kj;
	kj = m_Kjn.kjnam;
	kj->MoveFirst();
	// 工事番号桁数
	int klen = m_pZm->zvol->kjcol;

	if( kj->st == -1 )
		goto END_TAG;

	do {
		
		i++;

		if( i > ((page-1) * m_PageMax) ) {
			cnt++;
			if( cnt > m_PageMax )
				break;
		
			string = kj->KKI_KJCD;
			string.TrimRight();
			string.TrimLeft();
			set_codestring( bf, sizeof bf, (char*)(LPCTSTR)string, klen );
			bf[ klen ] = '\0';

			if( kj->KKI_KYMD > 0 )	//完成工事
				sprintf_s( tmp, sizeof tmp, "%s*%s", bf, kj->KKI_RYNM );
			else
				sprintf_s( tmp, sizeof tmp, "%s %s", bf, kj->KKI_RYNM );

			sprintf_s( fmt, sizeof fmt, "%%.%ds", get_strcnt( tmp, SEL_LEN ) );

			SelVal32[cnt-1].number = cnt;
			SelVal32[cnt-1].Data.Format( fmt, tmp );

		}

	} while( kj->MoveNext() == 0 );

END_TAG:

	if( cnt < m_PageMax ) {
		for( i = cnt; i < m_PageMax; i++ ) { 
			SelVal32[i].number = i+1;
			SelVal32[i].Data = "";
		}
	}

	m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
}


//-----------------------------------------------------
//	工事番号 検索
//	
//-----------------------------------------------------
BOOL CSelectSub::_koji_search()
{
	BOOL bRet = FALSE;

	if( svKana[0] == '\0' ) {
		if( m_KjKana != 0 ) {
			m_KjKana = 0;
			
			if( m_Kjn.kjnam != NULL ) {
				m_Kjn.kjnam->Requery(_T(""));
				m_KojiMax = m_Kjn.kjnam->GetRecordCount();
			}
			else {
				m_KojiMax = 0;
			}
			bRet = (m_KojiMax > 0);
		}
	}
	else {
		if( ! m_KjKana ) {
			m_KjKana = 1;
			CString filter;
			filter.Format( _T("KKI_KN50 LIKE '%s%%'"), svKana );

			if( m_Kjn.kjnam != NULL ) {
				m_Kjn.kjnam->Requery( filter );
				m_KjKanaMax = m_Kjn.kjnam->GetRecordCount();
			}
			else {
				m_KjKanaMax = 0;
				m_KojiMax	= 0;
			}
			bRet = (m_KjKanaMax > 0);
		}
	}

	return bRet;
}


//-----------------------------------------------------
//	工事番号 選択結果を取得
//	SelResult* result ...	選択結果格納
//	int page　...........	選択ページ
//	int sel_idx .........	選択インデックス
//-----------------------------------------------------
BOOL CSelectSub::_GetKojiSelect( SelResult* result, int page, int sel_no )
{
	CKJKKI* kj;
	kj = m_Kjn.kjnam;

	if( ! kj )	return FALSE;

	kj->MoveFirst();

	if( !(m_pZm->zvol->sub_sw&0x04) )
		return FALSE;

	BOOL ret = FALSE;
	int i, cnt;
	i = cnt = ret = 0;

	if( sel_no > m_PageMax )
		return FALSE;

	char	buf[64];
	int klen = m_pZm->zvol->kjcol;

	do {
		i++;

		if( i > ((page-1) * m_PageMax) ) {
			cnt++;
			if( cnt > m_PageMax )
				break;

			if( sel_no == cnt ) {
				ret = TRUE;

				strcpy_s( buf, sizeof buf, kj->KKI_KJCD );
				buf[ klen ] = '\0';

				result->kjcode	= buf;
				result->selname	= kj->KKI_RYNM;
				break;
			}
		}
	} while( kj->MoveNext() == 0 );

	return ret;
}


//---------------------------------------------------------
//	工事コード登録チェック
//
//---------------------------------------------------------
BOOL CSelectSub::_Kojicode_Ok( CString& kjcode )
{
	BOOL ret = FALSE;

	if( m_KjKana != 0 ) {
		m_KjKana = 0;
		
		if( m_Kjn.kjnam != NULL ) {
			m_Kjn.kjnam->Requery(_T(""));
			m_KojiMax = m_Kjn.kjnam->GetRecordCount();
		}
		else {
			m_KojiMax = 0;
		}
	}

	if( m_Kjn.kjnam != NULL ) {
		CString kj1, kj2;

		kj1 = kjcode;
		kj1.TrimRight();

		m_Kjn.kjnam->MoveFirst();

		do {
			kj2 = m_Kjn.kjnam->KKI_KJCD;
			kj2.TrimRight();

			if( kj1.CompareNoCase( kj2 ) == 0 ) {
				ret = TRUE;
				break;
			}

		} while( m_Kjn.kjnam->MoveNext() == 0 );
	}

	return ret;
}


//---------------------------------------------------------
//	工事コード未入力チェック
//
//---------------------------------------------------------
BOOL CSelectSub::_Kojicode_NoInp( CString kno, CString kmkcode )
{
	BOOL ret = FALSE;

	if( (m_pZm->zvol->apno != 0x20) )
		return FALSE;

	if( !(m_pZm->zvol->sub_sw&0x04) )
		return FALSE;

	DWORD dwCode = _atoh( (char*)(LPCTSTR)kmkcode );

	if( (dwCode& 0x0000f000) >= 0x00008000 ) {
		DBGKREC* pGk;
		DWORD dw1 = (dwCode&0xFFFFFF00);
		hl_rev( &dw1, 4 );
		dwCode = dw1;
	//	hl_rev( &dwCode, 4 );
		pGk = get_gksrc(dwCode, m_pZm);
		if( pGk ) {
			dwCode = pGk->gksrc;
			hl_rev( &dwCode, 4 );
		}
	}

	if( kno.IsEmpty() ) {
		if( (dwCode&0xffffff00) == 0x01040300 || (dwCode&0xffffff00) == 0x01060200 ||
			(dwCode&0xffffff00) == 0x04050200 || (dwCode&0xffffff00) == 0x08010100 ||
			(dwCode&0xffffff00) == 0x08020100 || (dwCode&0xff000000) == 0x0F000000 ) {

			ret = TRUE;
		}
	}

	return ret;
}

//---------------------------------------------------------
//	完成工事がしていされているのに、
//	未成工事支出金・未成工事受入金を使用しているか？
//
//---------------------------------------------------------
BOOL CSelectSub::_Kojicode_KanChk( CString kno, CString kmkcode )
{
	BOOL ret = FALSE;

	if( (m_pZm->zvol->apno != 0x20) )
		return FALSE;

	if( !(m_pZm->zvol->sub_sw&0x04) )
		return FALSE;

	DWORD dwCode = _atoh( (char*)(LPCTSTR)kmkcode );

	if( (dwCode& 0x0000f000) >= 0x00008000 ) {
		DBGKREC* pGk;

		DWORD dw1 = (dwCode&0xFFFFFF00);
		hl_rev( &dw1, 4 );
		dwCode = dw1;
	//	hl_rev( &dwCode, 4 );
		pGk = get_gksrc(dwCode, m_pZm);
		if( pGk ) {
			dwCode = pGk->gksrc;
			hl_rev( &dwCode, 4 );
		}
	}
	
	INT_PTR cnt;
	int i;
	cnt = m_KjiChk.GetCount();

	CString kj1, kj2;
	kj1 = kno;
	kj1.TrimRight();

	for( i = 0; i < cnt; i++ ) {
		if( m_KjiChk[i].kj_kymd > 0 && m_KjiChk[i].kj_kymd < m_pZm->zvol->ss_ymd) {
			kj2 = m_KjiChk[i].kjcode;
			kj2.TrimRight();

			//前期以前の完成工事で、未成の科目を使用か
			if( kj1 == kj2 ) {
				if( (dwCode & 0xffffff00) == 0x01060200 || (dwCode & 0xffffff00) == 0x04050200 ) {
					ret = TRUE;
				}
				break;
			}
		}
	}

	return ret;
}


//---------------------------------------------------------
//	科目選択
//	int nChar ... キーコード(-1で初期表示)
//
//---------------------------------------------------------
int	CSelectSub::_KamokuSelect( int nChar )
{
	if( nChar == -1 ) {	// 初期表示
		if( !m_bKmkHold ) m_CDPG = 1;
	//	resetpag( m_CDPG );
		skip_resetpag( m_CDPG, 1 );
		m_BrnTkySel = 0;
		m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
		return 0;
	}
	else {
		if( nChar == VK_HOME )
		{
			if( !m_BrnTkySel )
			{
				if( ! m_ACsel ) {
				//	if( m_CDPG == 1 ) return(0);
					m_CDPG = 1;
				}
			//	resetpag( m_CDPG );
				skip_resetpag( m_CDPG, 1 );

				m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
			}
			else
			{
				if( m_BRNTKY.bt_page )
					brntky_page( &m_BRNTKY, SelVal32, 0 );
				else if( ! (m_BrnMode & SL_BRNTKY_ONLY) )
				{
					m_BrnTkySel = 0;
				//	BRNTKY_sel_set( 0 );

					//科目・摘要選択画面リセット
					SelDspRst();
				//	enable_fixval( fixvalcnd() );	// 元背景色強制描画のため
				}
			}
			return(0);
		}
		else if( nChar == VK_NEXT )
		{
			if( !m_BrnTkySel )
			{
				if( ! m_ACsel ) { /* page sel */
					if( m_CDPG < PG_MAX ) m_CDPG++;
					else m_CDPG = 1;
				//	resetpag( m_CDPG );
					skip_resetpag( m_CDPG, 1 );
				}
				else { /* 50 */
					if( m_ACsel == PG_MAX || m_ACcnt <= (AC_MAX*m_ACsel) )
						return(0);
					m_ACsel++;
					kn50dsp( &KCD50C[AC_MAX*(m_ACsel-1)] );
				}


				if(Order_flg)	m_fixproc( SelFix8, &m_spp, 8, m_pWnd ); //091228
				m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
			}
			else
			{
				brntky_page( &m_BRNTKY, SelVal32, m_BRNTKY.bt_page + 1 );
			}
			return(0);
		}
		else if( nChar == VK_PRIOR )
		{
			if( !m_BrnTkySel )
			{
				if( ! m_ACsel ) {
					if( m_CDPG-1 >= 1 ) m_CDPG--;
					else				m_CDPG = PG_MAX;
				//	resetpag( m_CDPG );

					skip_resetpag( m_CDPG, -1 );
				}
				else { /* 50 on */

					if( m_ACsel == 1 || m_ACcnt <= AC_MAX )
						return(0);
					m_ACsel--;
					kn50dsp( &KCD50C[AC_MAX*(m_ACsel-1)] );
				}

				if(Order_flg)	m_fixproc( SelFix8, &m_spp, 8, m_pWnd ); //091228
				m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
			}
			else
			{
				// 枝番摘要
				brntky_page( &m_BRNTKY, SelVal32, m_BRNTKY.bt_page - 1 );
			}
			return(0);
		}
		return( nChar );
	}
}


//科目・摘要選択画面リセット
void CSelectSub::SelDspRst()
{
	int i,j;

	//科目名称選択表示
	if( m_spp.sp_nowpn != SL_TKY_PN )
	{
		if( m_spp.sp_nowpn == SL_BUMON_PN || m_spp.sp_nowpn == SL_BMNKANA_PN ) {
			// 部門選択
			_BmonSelect( -1, TRUE );
		//	page = m_pZm->get_bmnnowpg();
		//	bmnsel_page( page, SelVal32, &m_selval32 );
		}
		else if( m_spp.sp_nowpn == SL_KOJI_PN || m_spp.sp_nowpn == SL_KOJIKANA_PN) {
			// 工事選択
			_KojiSelect( -1, TRUE );
		}
		else if( m_spp.sp_nowpn == SL_HJIYU_PN ) {
			// 変動事由選択
			_HjiyuSelect( -1, TRUE );
		}
		else if( m_spp.sp_nowpn == SL_NPFURI_PN ) {
			// 公益 振替区分
			_NPfuriSelect( -1, TRUE );
		}
		else {
			if( m_ACsel )
			{
				j = m_ACsel - 1;
				if( kmk_50search( m_NowPar.kmk_kana ) == FALSE )
					m_ACsel = 0;
				else {
					for( i = 1 ; i < j ; ++i )	// 元表次ページまで送る
						_KamokuSelect( VK_NEXT );
				}
			}

			if( !m_ACsel )
			{
			//	j = m_CDPG;
			//	m_CDPG = 1;
			//	resetpag( m_CDPG );
				skip_resetpag( m_CDPG, 1 );
			}

			m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );

		}
	}
	else
	{			
		if( m_TkySign >= KANATKNO )
		{
			//５０音摘要表示
			j = m_T50page;
			if( tky_select50_dsp( m_save_50kana ) ) {
				if( j > m_T50pmax )
					j = m_T50pmax;

				for( i = 1 ; i < j ; ++i )
					SelTkyDsp( VK_NEXT );
			}
		}
		if( m_TkySign < KANATKNO )
		{
			// 連想摘要 か 仕訳摘要
			m_TkySign = 0;
			rentky_srdsp( m_Debcd, m_Crecd );
		}

		m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
	}
}

//
//	摘要表示 更新
//
void CSelectSub::tekiyo_update()
{
	// 連想摘要 か 仕訳摘要
	for( int dcsw = 0; dcsw < 2; dcsw++ ) {
		m_RTKblk[dcsw] = -1;
		m_RTKcnt[dcsw] = 0;
	}
	CString debt, cred;
	debt = m_Debcd;
	cred = m_Crecd;

	// 09.30 /08
	m_pZm->rtrec->Requery("");

	int svpage, n;
	if( m_TkySign == RENTKNO ) {
		svpage	= m_RTpage;
		tky_atswk_dsp( debt, cred, FALSE );
		_rentky_str( debt, cred );
		//以前のページまで表示
		if( svpage > RTpmax_Get() )
			m_RTpage = RTpmax_Get();
		else
			m_RTpage = svpage;
		//連想摘要表示
		rentkydsp();
	}
	else if( m_TkySign == SWKTKNO )  {
		_rentky_str( debt, cred );

		svpage = m_TATpage;

		if( tky_atswk_dsp( debt, cred ) ) {
			//以前のページまで表示
			if( svpage > m_TATpmax )
				svpage = m_TATpmax;

			for( n = 0; n < (svpage-1); n++ ) {
				SelTkyDsp( VK_NEXT );
			}
		}
	}
	else {
		// 摘要カナ検索
		tky_atswk_dsp( debt, cred, FALSE );
		_rentky_str( debt, cred );

		// 09.21 /11
		svpage = m_T50page;
		if( tky_select50_dsp( m_save_50kana ) ) {
			//以前のページまで表示
			if( svpage > m_T50pmax )
				svpage = m_T50pmax;

			for( n = 0; n < (svpage-1); n++ ) {
				SelTkyDsp( VK_NEXT );
			}
		}
	}
}



//091228 関数内場合分け
int CSelectSub::resetpag( int page )
{
	int	r;

	if(Order_flg){

		///
		memcpy(&FIXTBL[0], KDPAG[page-1], 4 * 8);
		fixcreat_order( (int *)&FIXTBL[0], (int *)KDPAG[page-1]  );//第一引数はつかっていない？
///		m_spp.sp_nowpn = SL_KAMOKU_PN;
		::ZeroMemory( m_spp.seljob, sizeof m_spp.seljob );
		m_fixproc( SelFix8, &m_spp, 8, m_pWnd );

		m_ACsel	= 0;
		m_BmnPG		= 0;

	//	m_BrnTkySel = 0;
	//	memset( &m_BRNTKY, '\0', sizeof( m_BRNTKY ) );
	//	m_BRNTKY.TS_Reset();
		init_brntkypar( &m_BRNTKY );

		m_BrnTkySel	= 0;

		r = km_dsp( (int *)(&KDPAG[page-1][8]) ); // 8つ目までは固定枠に入っていたデータとしてもっている
		//科目順の場合32個に表示データがないとしても　固定枠にデータがあればよいのでｒ=0だとしてもデータがある場合がある
		int i = 0;
		for(i = 0 ; i < 8 ; i++){
			if(FIXTBL[i]) r++;
		}

		m_KPGcnt = r;


	}
	else{
		fixcreat( (int *)&FIXTBL[0] );

		//100106 ifの判定分の追加 切替時に戻ったとき科目ｺｰﾄﾞになってしまうため
		if(Order_flg == Order_flg2){
			m_spp.sp_nowpn = SL_KAMOKU_PN;
		}
		::ZeroMemory( m_spp.seljob, sizeof m_spp.seljob );
		m_fixproc( SelFix8, &m_spp, 8, m_pWnd );

		m_ACsel	= 0;
		m_BmnPG		= 0;

	//	m_BrnTkySel = 0;
	//	memset( &m_BRNTKY, '\0', sizeof( m_BRNTKY ) );
	//	m_BRNTKY.TS_Reset();
		init_brntkypar( &m_BRNTKY );

		m_BrnTkySel	= 0;

		r = km_dsp( (int *)KDPAG[page-1] );

		m_KPGcnt = r;
	}

	return( r );
}

//
// 指定ページを表示して、そのページに科目がない場合、科目があるページまでスキップする
//
int CSelectSub::skip_resetpag( int& page, int vect )
{
	int	disppage, nowpage;
	disppage = nowpage = page;

	int cnt = resetpag( disppage );
	int i;

	if( cnt <= 0 ) {
		if( vect > 0 ){
			// 送り
			for( i = disppage; i <= PG_MAX; i++ ) {
				cnt = resetpag( i );
				if( cnt > 0 ) {
					nowpage = i;
					break;
				}
			}
			if( cnt <= 0 ) {
				if( disppage > 1 ) {
					for( i = 1; i < disppage; i++ ) {
						cnt = resetpag( i );
						if( cnt > 0 ) {
							nowpage = i;
							break;
						}
					}
				}
			}
		}
		else {
			// 戻り
			for( i = disppage; i >= 1; i-- ) {
				cnt = resetpag( i );
				if( cnt > 0 ) {
					nowpage = i;
					break;
				}
			}
			if( cnt <= 0 ) {
				if( disppage < PG_MAX ) {
					for( i = PG_MAX; i > disppage; i-- ) {
						cnt = resetpag( i );
						if( cnt > 0 ) {
							nowpage = i;
							break;
						}
					}
				}
			}
		}
	}

	page = nowpage;

	return cnt;
}


// 固定科目作成
void CSelectSub::fixcreat(int *fix)
{
	int		seq, j, k, l;
	BOOL	bKcod;
	bKcod = (m_KmkMode == SL_KMK_CODE);

#define FIX_l	2

	for( k = seq = 0; k < 4; k++ ) {
		for( j = FIX_l; j < FIX_l+2; j++ ) {

			if( (l = fix[seq++] - 1) < 0 ) {
				SelFix8[seq-1].number = bKcod ? -1 : seq;
				SelFix8[seq-1].Data = "";
			//	SelFix8[seq-1].Data.Format( "%2d", seq );
				continue;
			}
			else if( ! check_kamokurestrict( l ) ) {
				SelFix8[seq-1].number = bKcod ? -1 : seq;
				SelFix8[seq-1].Data = "";
				continue;
			}

#ifndef KMCOD_OLD_KEEP
			SelFix8[seq-1].number = seq;
#else
			SelFix8[seq-1].number = bKcod ? get_hyojikncod( l ) : seq+9;
#endif
			SelFix8[seq-1].Data = get_hyojiknam( l );
		}
	}
}


int CSelectSub::km_dsp( int *knp  )
{
	int		i, j;
	char *p;
	BOOL bKcod;
	bKcod = (m_KmkMode == SL_KMK_CODE);

	struct _SelTable *pSelVal;
	pSelVal = SelVal32;

	for( i=j=0; i < 32; i++ ) {

		if( *(knp + i) ) {
			if( ! check_kamokurestrict( *(knp + i)-1 ) ) {
				p = "";
				(pSelVal+i)->number = bKcod ? -1 : i+9;
			}
			else {
				p = get_hyojiknam( *(knp + i)-1 );
				j++;

#ifndef KMCOD_OLD_KEEP
				(pSelVal+i)->number = i+9;
#else
				(pSelVal+i)->number = bKcod ? get_hyojikncod(*(knp+i)-1) : i+9;
#endif
			}
		}
		else	{
			p = "";
			(pSelVal+i)->number = bKcod ? -1 : i+9;
		}

		(pSelVal+i)->Data = p;
	}

	return j;
}


// 科目カナソート
static
int __cdecl kmkanacompare(/*void *context,*/ const void *ele1, const void *ele2 )
{
	int ret;

	KMKANA_DTA* dw1, *dw2;

	dw1 = (KMKANA_DTA*)ele1;
	dw2 = (KMKANA_DTA*)ele2;

	ret = memcmp( dw1->kd_kana, dw2->kd_kana, sizeof dw1->kd_kana );
	//カナが同じ場合は、科目順
	if( ret == 0 ) {
		if( dw1->kd_seq > dw2->kd_seq )
			ret = 1;
		else if( dw1->kd_seq < dw2->kd_seq )
			ret = -1;
	}

	return ret;
}

// ソートの順をアイウエオ順にするため
void km_kana_check( char* kana, int size )
{
	for( int i = 0; i < size; i++ ) {

		switch( *(kana+i) ) {
		case 'ｧ':	*(kana+i)	=	'ｱ';	break;
		case 'ｨ':	*(kana+i)	=	'ｲ';	break;
		case 'ｩ':	*(kana+i)	=	'ｳ';	break;
		case 'ｪ':	*(kana+i)	=	'ｴ';	break;
		case 'ｫ':	*(kana+i)	=	'ｵ';	break;
		case 'ｬ':	*(kana+i)	=	'ﾔ';	break;
		case 'ｭ':	*(kana+i)	=	'ﾕ';	break;
		case 'ｮ':	*(kana+i)	=	'ﾖ';	break;
		case 'ｯ':	*(kana+i)	=	'ﾂ';	break;
		}
	}
}


int CSelectSub::kmk_50search( char* kana )
{
	int i, j;
	CString sKana;
	sKana.Format( "%s", kana );

	struct _DBKNREC	*pKn;
	for( i = j = 0; i < m_pZm->knm_info->reccount; i++ ) {
		pKn = (m_pZm->pKnrec + i);
		
		// 不使用科目
		if( (pKn->knvoid & 0x01) )
			continue;
		// 業種の制限
		if( ! check_kamokurestrict( pKn ) )
			continue;

		CString reckana;
		reckana = pKn->knkana;
		reckana.TrimRight();
		if( reckana.Find( sKana ) == 0 ) {
			if( j >= AC_MAX * PG_MAX ) break;
			memcpy( KCD50C[j].kd_kana, pKn->knkana, sizeof KCD50C[j].kd_kana );
			km_kana_check( KCD50C[j].kd_kana, sizeof sizeof KCD50C[j].kd_kana );

			KCD50C[j++].kd_seq = i+1;
		}
	}
	


	if( j ) {

		if( m_KjunDsp == SL_KJUN_DSP )
			qsort( KCD50C, j, sizeof KCD50C[0], kmkanacompare );

		m_ACcnt = j;
		m_ACsel = 1;
		for( ; j < AC_MAX * PG_MAX; j++ ) {
			::ZeroMemory( &KCD50C[j], sizeof KCD50C[0] );
		}

		kn50dsp( &KCD50C[0] );
		return( TRUE );
	}
	else
		return( FALSE );

}

//-----------------------------------------------------------------------------
// 入力コードによる科目のサーチ
//-----------------------------------------------------------------------------
// 引数	inpcd		：	入力コード
//		onedspsw	：	１科目でも表示させるかどうか
//-----------------------------------------------------------------------------
// 返送値	0		：	サーチ結果の表示
//			1		：	該当科目が１科目のみ
//			-1		：	該当科目無し
//-----------------------------------------------------------------------------
int CSelectSub::kmk_inpcodesearch( long inpcd, int onedspsw/*0*/ )
{
	int	i, j;
	struct _DBKNREC	*pKn;

	CArray<KMKANA_DTA, KMKANA_DTA&> bak;
	// カナ検索情報をバックアップ
	if( m_ACsel ) {
		for( i = 0; i < (AC_MAX*PG_MAX); i++ ) {
			bak.Add( KCD50C[i] );
		}
	}

	for( i=j=0; i<m_pZm->knm_info->reccount; i++ ){
		pKn = (m_pZm->pKnrec + i);
		
		// 不使用科目
		if( (pKn->knvoid&0x01) ){
			continue;
		}
		// 業種の制限
		if( !check_kamokurestrict( pKn ) ){
			continue;
		}

		if( pKn->knicod == inpcd ){
			if( j >= (AC_MAX*PG_MAX) ){
				break;
			}
			memcpy( KCD50C[j].kd_kana, pKn->knkana, sizeof KCD50C[j].kd_kana );
			KCD50C[j++].kd_seq = i+1;
		}
	}

/*- '09.03.11 -*/
//	if( j == 1 ){
//		return 1;
//	}
//	else if( j ){
//
//		if( m_KjunDsp == SL_KJUN_DSP ){
//			qsort( KCD50C, j, sizeof KCD50C[0], kmkanacompare );
//		}
//
//		m_ACcnt = j;
//		m_ACsel = 1;
//		for( ;j<(AC_MAX*PG_MAX); j++ ){
//			::ZeroMemory( &KCD50C[j], sizeof KCD50C[0] );
//		}
//
//		kn50dsp( &KCD50C[0] );
//
//		return 0;
//	}
//	else{
//		return -1;
//	}
/*-------------*/
	if( j ){

		if( (j>1) || ((j==1)&&(onedspsw==1)) ){
			if( m_KjunDsp == SL_KJUN_DSP ){
				qsort( KCD50C, j, sizeof KCD50C[0], kmkanacompare );
			}

			m_ACcnt = j;
			m_ACsel = 1;
			for( ;j<(AC_MAX*PG_MAX); j++ ){
				::ZeroMemory( &KCD50C[j], sizeof KCD50C[0] );
			}

			kn50dsp( &KCD50C[0] );
		}
		else {
			if( m_ACsel ) {
				for( i = 0; i < (AC_MAX*PG_MAX); i++ ) {
					KCD50C[i] = bak[i];
				}
			}
		}

		if( j == 1 ){
			return 1;
		}
		else{
			return 0;
		}
	}
	else{
		return -1;
	}
/*-------------*/
}

void CSelectSub::kn50dsp( KMKANA_DTA *knp, int syusgn/*=0*/ )
{
	int		j, k, l, seq, seq8, i, r_num ; //091228 seq8 i r_num追加

	BOOL bKcod;
	bKcod = (m_KmkMode == SL_KMK_CODE);

	seq = 0;
	seq8 = 0;
	r_num = 4;
	if(Order_flg){
		r_num = 5;
	/*	for(i = 0 ; i < 40 ; i++){
			if(i < 8){
				SelFix8[i].Data = "";
				SelFix8[i].number = i+1 ;
			}
			SelVal32[i].Data = "";
			SelVal32[i].number = i+9 ;
		}
		*/
	}

	for( k = 0; k < r_num; k++ ) {
		for( j = 0; j < 8; j++, knp++ ) {

			if( (l = knp->kd_seq - 1) < 0 ) {
				//091228
				if(Order_flg){

					if(seq8 < 8){
						SelFix8[seq8].number = bKcod ? -1 : seq8+1;
						SelFix8[seq8++].Data = "";
					}
					else{
						SelVal32[seq].number = bKcod ? -1 : seq+9;
						SelVal32[seq++].Data = "";
					}
					continue;
				}
				else{
					if( ! syusgn )	SelVal32[seq].number = bKcod ? -1 : seq+9;
					else			SelVal32[seq].number = bKcod ? -1 : seq+1;
				}

				SelVal32[seq++].Data = "";
				continue;
			}

#ifndef KMCOD_OLD_KEEP
			if( ! syusgn )	SelVal32[seq].number = seq+9;
			else			SelVal32[seq].number = seq+1;

			if(Order_flg)	SelFix8[seq8].number = seq8+1;
#else
			SelVal32[seq].number = bKcod ? get_hyojikncod( l ) : seq+9;
#endif



	///		CString sss;
	///		sss.Format("syusgn = %s", syusgn);
	///		AfxMessageBox(sss);
			//091228
			if(Order_flg){
				if(seq8 < 8)	SelFix8[seq8++].Data = get_hyojiknam( l, syusgn );
				else			SelVal32[seq++].Data = get_hyojiknam( l, syusgn );
					
			}
			else SelVal32[seq++].Data = get_hyojiknam( l, syusgn );
		}
	}
}


int CSelectSub::get_hyojikncod( int n )
{
	int kmcod = -1;

	struct _DBKNREC* pKn;
	pKn = m_pZm->pKnrec + n ;
	kmcod = pKn->knicod;

	return kmcod;
}

//
// 選択表示用科目名称作成（消費税属性付）
//										04.17 /98 追加
char* CSelectSub::get_hyojiknam( int n, int syusgn/*=0*/ )
{
	static CString pbuf;
	CString fmt;
	unsigned char	ch;
	char *chmsg;
	BOOL bKcod;
	char codbuf[20] = {0};
	int kcod;

	struct _DBKNREC* pKn;
	if( ! syusgn ) {
		pKn = m_pZm->pKnrec + n ;
	}
	else {
		pKn = m_pNPb->pKsrec + n;	// 収支科目
	}

	fmt.Format( _T("%%c%%.%ds%%s"), KMNLEN );

	// 03.27 /09
	if( m_NowPar.sel_pn == SL_KMINPCD_PN || m_NowPar.sel_pn == SL_NPSKMINPCD_PN )
		bKcod = FALSE;
	else
		bKcod = (m_KmkMode == SL_KMK_CODE);

	if( bKcod ) {	//02.20 /02 -- 科目コード表示
		kcod = pKn->knicod;
		
		if( kcod == -1 )
			strcpy_s( codbuf, sizeof codbuf, "    " );
		else
			sprintf_s( codbuf, sizeof codbuf, "%04d", kcod );
		
		fmt.Format( _T("%%s%%c%%.%ds%%s"), KMNLEN );

#ifdef CUT //#* 08.05 /02
		// 科目コードが3桁の時 ｽﾍﾟｰｽをｾｯﾄ
		if( !(kcod /1000) ) {
			if( !(kcod /100) ) {
				if( !(kcod /10) ) {
					fmt.Format( _T("   %%c%%.%ds%%s"), KMNLEN );
				}
				else {
					fmt.Format( _T("  %%c%%.%ds%%s"), KMNLEN );	
				}
			}
			else {
				fmt.Format( _T(" %%c%%.%ds%%s"), KMNLEN );		
			}
		}
#endif
	}

	if( ! syusgn ) {
		switch(  (pKn->knsgn[2] & 0x0f) )
		{
		case 0x01:	ch = '*'; chmsg = bKcod ? _T(" /不") : _T("/不課"); break;
		case 0x03:	ch = '*'; chmsg = bKcod ? _T(" /非") : _T("/非課"); break;
		default:	ch = ' '; chmsg = _T(""); break;
		}
	}
	else {
		// 収支科目
		ch = ' '; chmsg = _T(""); 
	}

	if( !bKcod ) pbuf.Format( fmt, ch, pKn->knnam, chmsg );	
	else		 pbuf.Format( fmt, codbuf, ch, pKn->knnam, chmsg );

	return (char*)(LPCTSTR)pbuf;

}


// 枝番摘要検索＆表示
int CSelectSub::brntky_sch( BRNTKY_PAR *tkbrn, struct _SelTable *sel32D, int bmn, CString& code, char* kana/*=NULL*/, BOOL bDISP/*=FALSE*/ )
{
	int ret;
		
	if( ret = _brntky_sch( tkbrn, bmn, code, kana, bDISP ) )
	{
		// 表示
		_brntky_dsp( tkbrn, sel32D, 9 );

	//	m_Sel32txt = tkbrn->bt_txtcol;
	//	m_Sel32bak = BRTEK_BackCOL;
		m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
	//	_brntky_col( tkbrn, sel32C, );
	//!	sel32C->SetData( (long)sel32D );
	//	p_DBDinpView->SetICSSel32( (long)sel32D );
	}
	return ret;
}

int CSelectSub::_brntky_sch( BRNTKY_PAR *tkbrn, int bmn, CString& code, char* kana, BOOL bDISP/*=FALSE*/ )
{
	BRN_TB *ptbl;
	BOOL ret;
	CString sKana;
	if( kana == NULL ) sKana.Empty();
	else {
		sKana = kana;
	}

//TRACE( "CSelectSub sKana.IsEmpty() %d, length %d\n", sKana.IsEmpty(), sKana.GetLength() );
	if( ! bDISP )
		m_BrnCurMode = (m_BrnMode & 0x000000ff);
	else
		m_BrnCurMode = (BT_NOMALBRN_TEK|BT_SYOGTBRN_TEK);	// TABなど必ず表示

	ret = m_BrnTkyo.get_brtektb( &ptbl, bmn, code, sKana, AC_MAX, 0, FALSE, m_BrnCurMode );




	if( ret )
	{
		tkbrn->bt_bmn = ptbl->bmn;
		tkbrn->bt_code = ptbl->code;
		tkbrn->bt_kana = ptbl->brn_kn50;
		tkbrn->bt_page = ptbl->page_now-1;
		tkbrn->bt_txtcol = ptbl->ForeCol;

		return 1;	// found
	}
	else
	{
//		memset( tkbrn, '\0', sizeof( BRNTKY_PAR) );
//		tkbrn->TS_Reset();
		init_brntkypar( &m_BRNTKY );

		return 0;	// Not found
	}
}

//
// 枝番摘要表示ページセット
void CSelectSub::_brntky_dsp( BRNTKY_PAR *tkbrn, struct _SelTable *sel32D, int start_no )
{
	BRN_TB *ptbl;
	BOOL ret;
	int i;
	char fmt[20];
	int mode = m_BrnCurMode;

	ret = m_BrnTkyo.get_brtektb( &ptbl, tkbrn->bt_bmn, tkbrn->bt_code, tkbrn->bt_kana, AC_MAX, tkbrn->bt_page, FALSE, mode );
	
	if( ret )
	{

		for( i = 0 ; i < AC_MAX ; ++i )
		{
			(sel32D+i)->number = start_no + i;
			if( i < ptbl->rec_n )
			{
				wsprintf( fmt, "%%.%ds", get_strcnt( (char*)(LPCTSTR)(ptbl->pBRN_REC+i)->brmsg, SEL_LEN ));
				(sel32D+i)->Data.Format( fmt, (ptbl->pBRN_REC + i)->brmsg );
			}
			else
				(sel32D+i)->Data = "";

		}
	}
}

//
// 枝番摘要ページアクセス
int CSelectSub::brntky_page( BRNTKY_PAR *tkbrn, struct _SelTable *sel32D, int page )
{
	BRN_TB *ptbl;
	BOOL ret;
	int mode = m_BrnCurMode;

	ret = m_BrnTkyo.get_brtektb( &ptbl, tkbrn->bt_bmn, tkbrn->bt_code, tkbrn->bt_kana, AC_MAX, page, FALSE, mode );

	if( ret )
	{
		tkbrn->bt_page = ptbl->page_now-1;
		// 表示
		_brntky_dsp( tkbrn, sel32D,  9 );

	//	m_Sel32txt = tkbrn->bt_txtcol;
	//	m_Sel32bak = BRTEK_BackCOL;
		m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );

	//	_brntky_col( tkbrn, sel32C, );
	//!	sel32C->SetData( (long)sel32D );
	//	p_DBDinpView->SetICSSel32( (long)sel32D );

		return 1;	// OK
	}
	else
		return 0;	// over
}

//
// 科目.枝番リード
int CSelectSub::brntky_get( int* bmn, CString *code, int *brn, BRNTKY_PAR *tkbrn, int no )
{
	BRN_TB *ptbl;
	BOOL ret;
	int mode = m_BrnCurMode;

	ret = m_BrnTkyo.get_brtektb( &ptbl, tkbrn->bt_bmn, tkbrn->bt_code, tkbrn->bt_kana, AC_MAX, tkbrn->bt_page, FALSE, mode );

	if( ret && no > 0 && no <= ptbl->rec_n )
	{
		*bmn	= ptbl->bmn;
		*code	= ptbl->code;
		*brn	= (ptbl->pBRN_REC + (no - 1))->brn;

		return 1;	// OK
	}
	else
		return 0;	// NO

}


//	int CSelectSub::_BrnNameSelect( int keycode )
//{
//}

//	int	CSelectSub::_TkyoSelect( int keycode )
//{
//}

//摘要表示欄ページめくり
int CSelectSub::SelTkyDsp( int nChar )
{
	int dcnt = 0;
	m_spp.sp_prevpn = -1;
	m_spp.sp_nowpn = m_spp.sp_nowpn;

	if( nChar != VK_HOME ) {
		if( m_TkySign == KANATKNO ) {
			tky50_reload();
		}
	}

	if( nChar == VK_NEXT )
	{
		/* 50 on next page */
		if( m_TkySign == RENTKNO ) {
			if( (m_RTpage+1) <= RTpmax_Get() ) {
				m_RTpage++;
				rentdsp( m_RTpage, 0 );
				rentdsp( m_RTpage, 1 );
				m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
			}
			else {
				// 仕訳摘要を表示する
				if( make_autotky() ) {
					m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
				}
			}

			return 0;
		}
		//#* 12.09 /02
		if( m_TkySign == SWKTKNO ) {
			if( m_SW_AutoTK <= AC_MAX || m_TATpage+1 > m_TATpmax ) {
				// 科目対応摘要を表示する
				m_RTpage = 1;	// 最初のページ
				rentdsp( m_RTpage, 0 );
				rentdsp( m_RTpage, 1 );
				m_TkySign = RENTKNO;
				m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );

			//	renttl();
			}
			else {			
				if( (++m_TATpage) == m_TATpmax )
					dcnt = m_SW_AutoTK - ( AC_MAX * (m_TATpage - 1) );
				else dcnt = AC_MAX;
				tkauto_dsp( m_TATpage );
				m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
			
			}
			return 0;
		}

		if( m_TkySign != KANATKNO || m_SW_TK50 <= AC_MAX || m_T50page + 1 > m_T50pmax )
			return 0;

		if( (++m_T50page) == m_T50pmax )
			dcnt = m_SW_TK50 - ( AC_MAX * (m_T50page - 1) );
		else dcnt = AC_MAX;
		tky50disp( m_T50page, dcnt );

		m_tallPG = m_T50page;

		m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
		
		return 0;
	}
	else if( nChar == VK_PRIOR )
	{
		if( m_TkySign == RENTKNO ) {
			if( m_RTpage > 1 ) {
				m_RTpage--;
				rentdsp( m_RTpage, 0 );
				rentdsp( m_RTpage, 1 );
			}
			else {
				// 仕訳対応摘要の 最終ページを表示
				make_autotky( TRUE );
			}
			m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );

			return 0;
		}

		//#* 12.09 /02
		if( m_TkySign == SWKTKNO ) {
			
			if( m_SW_AutoTK <= AC_MAX || m_TATpage-1 <= 0 ) {
				// 科目対応摘要ページへ
				//最終ページに
				m_RTpage = RTpmax_Get();
				rentdsp( m_RTpage, 0 );
				rentdsp( m_RTpage, 1 );
				m_TkySign = RENTKNO;
				m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
			//	renttl();
				return 0;
			}
			
			if( (--m_TATpage) == m_TATpmax )
				dcnt = m_SW_AutoTK - ( AC_MAX * (m_TATpage - 1) );
			else dcnt = AC_MAX;
			tkauto_dsp( m_TATpage );
			m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );

			return 0;
		}

		if( (m_TkySign != KANATKNO) || (m_SW_TK50 <= AC_MAX) ||
			(m_T50page - 1 <= 0 ) )
			return 0;

		if( (--m_T50page) == m_T50pmax ) 
				dcnt = m_SW_TK50 - (AC_MAX * (m_T50page - 1) );
		else	dcnt = AC_MAX;
		tky50disp( m_T50page, dcnt );

		m_tallPG = m_T50page;

		m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );

		return 0;
	}
	else if( nChar == VK_HOME )
	{
		// 仕訳摘要 を表示 ... 09.17 /03
		if( m_TkyMode == SL_TKY_SIWAKE ) {
			// 仕訳摘要 -> 科目摘要 -> 50音摘要
			if( m_TkySign == RENTKNO ) {
				if( make_50tky() ) {
					m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
				//	SetICSSel32( (long)SelVal32 );
				}
				else if( make_autotky() ) {
					m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
				//	SetICSSel32( (long)SelVal32 );
				}
			}
			else if( m_TkySign == SWKTKNO ) {
				rentdsp( m_RTpage, 0 );
				rentdsp( m_RTpage, 1 );
				m_TkySign = RENTKNO;
				m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
			//	renttl();
			//	SetICSSel32( (long)SelVal32 );
			}
			else {
				if( make_autotky() ) {
					m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
			//		SetICSSel32( (long)SelVal32 );
				}				
				else {
					rentdsp( m_RTpage, 0 );
					rentdsp( m_RTpage, 1 );
					m_TkySign = RENTKNO;
					m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
			//		renttl();
			//		SetICSSel32( (long)SelVal32 );
				}
			}
		}
		else {
			// 科目摘要を表示 ... 09.17 /03
			// 科目摘要 -> 仕訳摘要 -> 50音摘要
			if( m_TkySign == RENTKNO ) {
				if( make_autotky() ) {
					m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
			//		SetICSSel32( (long)SelVal32 );
				}
				else if( make_50tky() ) {
					m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
			//		SetICSSel32( (long)SelVal32 );
				}
			}
			else if( m_TkySign == SWKTKNO ) {
				if( make_50tky() ) {
					m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
			//		SetICSSel32( (long)SelVal32 );
				}
				else {
					rentdsp( m_RTpage, 0 );
					rentdsp( m_RTpage, 1 );
					m_TkySign = RENTKNO;
					m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
				//	renttl();
				//	SetICSSel32( (long)SelVal32 );
				}
			}
			else {
				rentdsp( m_RTpage, 0 );
				rentdsp( m_RTpage, 1 );
				m_TkySign = RENTKNO;
				m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
			//	renttl();
			//	SetICSSel32( (long)SelVal32 );
			}
		}

		return 0;
	}

	return nChar;
}


/*------------------------
	ﾚﾝｿｳ ﾃｷﾖｳ ﾋｮｳｼﾞ
--------------------------*/
void CSelectSub::rentdsp( int pg, int dc_sw )
{
	char	temp[128];
	int		i, n, tkln, l, selpnt;
	CString tmp;

	selpnt = ! dc_sw ? 0 : DP_MAX;

	strcpy_s( m_spp.seljob, sizeof m_spp.seljob,  "《科目対応摘要》" );

	::ZeroMemory( &TKSEL_PG[selpnt], sizeof( TKSEL_PG[0] ) * DP_MAX );

	m_pZm->rtrec->MoveFirst();
	i = n = 0;
	do {
		if( m_RTKblk[dc_sw] == m_pZm->rtrec->rtpgn ) {
			if( (m_pZm->rtrec->rtcod < 1) ) continue;
			//指定ページまで飛ばす
			if( ( DP_MAX * (pg-1) ) >= ++i ) {
				continue;
			}

<<<<<<< .working
			if( DB_TekiyoNoRead( m_pZm->rtrec->rtcod ) != 0 ) {
			//	ICSMessageBox("摘要レコードが読めません！");
			//	return;
				continue;
||||||| .merge-left.r50
	i = n = max = 0;

	if( m_RTSWKkana ) {
		m_TKYkana.RemoveAll();
		
		do {
			DBTKREC	rec = {0};
			if( m_RTKblk[dc_sw] == m_pZm->rtrec->rtpgn ) {
				if( DB_TekiyoNoRead( m_pZm->rtrec->rtcod ) != 0 ) {
					continue;
				}

				rec.TKcod	= m_pZm->tkrec->tkcod;
				sprintf_s( temp, sizeof temp, "%s", m_pZm->tkrec->tkana );
				memcpy( rec.TKana, temp, sizeof rec.TKana );

				rec.TKcsg	= m_pZm->tkrec->tksgn[0];
				rec.TKszk	= m_pZm->tkrec->tksgn[1];

				sprintf_s( temp, sizeof temp, "%s",  m_pZm->tkrec->tkname );
				memcpy( rec.TKnam, temp, sizeof rec.TKnam );

				rec.tkari	= _atoh( (LPSTR)(LPCTSTR)m_pZm->tkrec->tkari );
				rec.tkasi	= _atoh( (LPSTR)(LPCTSTR)m_pZm->tkrec->tkasi );

				m_TKYkana.Add( rec );
				max++;
=======
	i = n = max = 0;

MyTrace( "rentdsp m_dwOPT = %04x\n", m_dwOPT );

	if( m_dwOPT&0x04 ) {
		m_TKYkana.RemoveAll();
		
		do {
			DBTKREC	rec = {0};
			if( m_RTKblk[dc_sw] == m_pZm->rtrec->rtpgn ) {
				if( DB_TekiyoNoRead( m_pZm->rtrec->rtcod ) != 0 ) {
					continue;
				}

				rec.TKcod	= m_pZm->tkrec->tkcod;
				sprintf_s( temp, sizeof temp, "%s", m_pZm->tkrec->tkana );
				memcpy( rec.TKana, temp, sizeof rec.TKana );

				rec.TKcsg	= m_pZm->tkrec->tksgn[0];
				rec.TKszk	= m_pZm->tkrec->tksgn[1];

				sprintf_s( temp, sizeof temp, "%s",  m_pZm->tkrec->tkname );
				memcpy( rec.TKnam, temp, sizeof rec.TKnam );

				rec.tkari	= _atoh( (LPSTR)(LPCTSTR)m_pZm->tkrec->tkari );
				rec.tkasi	= _atoh( (LPSTR)(LPCTSTR)m_pZm->tkrec->tkasi );

				m_TKYkana.Add( rec );
				max++;
>>>>>>> .merge-right.r51
			}

			tmp = m_pZm->tkrec->tkname;
			tkln = tmp.GetLength();
			memcpy(temp, tmp, tkln);
			temp[tkln] = '\0';
			l = get_strcnt( (char*)(LPCTSTR)tmp, SEL_LEN/*tkln*/ );
			temp[l] = '\0';

			TKSEL_PG[selpnt] = m_pZm->rtrec->rtcod;
			SelVal32[selpnt].number = selpnt+ /*(isEntrySys() ? 9 : 1) */ 1;
			SelVal32[selpnt++].Data = temp;

			if( ++n >= DP_MAX )
				break;
		}
	} while( m_pZm->rtrec->MoveNext() == 0 );

	if( n < DP_MAX ) {
		temp[0] = '\0';
		for( i = 1; (i+n) <= DP_MAX; i++ ) {
			SelVal32[selpnt].number = selpnt+ /*(isEntrySys() ? 9 : 1) */ 1;
			SelVal32[selpnt++].Data = temp;
		}
	}

}



BOOL CSelectSub::tky_select50_dsp( char *data, BOOL bDISP/*=TRUE*/ )
{
	int i,n;
	BOOL ret = FALSE;
	unsigned char *p = NULL;
	char str[50], fmt[10];

	if( data != NULL ) {
		strcpy_s( str, sizeof str, (char *)data );
		p = (unsigned char *)str;
		// 全表示
		if( data[0] == '\0' && m_NowPar.sel_pn == SL_TKYALL_PN )
			p = NULL;
	}

	i = 0;

	if( p == NULL ) {
		i = tkrsrch( NULL );
		ret = TRUE;
	}
	else {
		if( !isdigitstr( (char*)p ) )	// 数字文字列でない
		{
			// ASCII to ANK
			for( ; *p ; ++p )
			{
				if( (unsigned char)*p == (unsigned char)'ｰ' )
					*p = '-';
				else if( (unsigned char)*p == (unsigned char)' ' )
					*p = ' ';
			}

			// カナサーチ
			sprintf_s( fmt, sizeof fmt, "%%.%ds", sizeof( m_save_50kana ) - 1 );
			n = sprintf_s( m_save_50kana, sizeof m_save_50kana, fmt, str );
			i = tkrsrch( m_save_50kana );

			ret = TRUE;
		}
	}

	if( i )
	{
		m_SW_TK50 = i;

		if( bDISP ) {
			if( make_50tky() ) {	// <- use to m_SW_TK50
				m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
			}
		}
	}

	return ret;
}


/*-------------------------------------------
	TEKIYO search
	return count
--------------------------------------------*/
int CSelectSub::tkrsrch( char *akbuf )
{
	CString filter;
	int i = 0;
	int sort;

 	if( akbuf == NULL ) {
		m_KanaTKrec.RemoveAll();
		// 全摘要
		if( m_pZm->tkrec->Requery( "", 0 ) == 0 ) {
#ifdef TKYARY_CLOSE
			i = m_pZm->tkrec->GetRecordCount();
#else
			DBTKREC	rec = {0};
			char buf[128];

			m_pZm->tkrec->MoveFirst();
			if( m_pZm->tkrec->st != -1 ) {
				do {
					rec.TKcod	= m_pZm->tkrec->tkcod;

					sprintf_s( buf, sizeof buf, "%s",  m_pZm->tkrec->tkana );
					memcpy( rec.TKana, buf, sizeof rec.TKana );

					rec.TKcsg	= m_pZm->tkrec->tksgn[0];
					rec.TKszk	= m_pZm->tkrec->tksgn[1];

					sprintf_s( buf, sizeof buf, "%s",  m_pZm->tkrec->tkname );
					memcpy( rec.TKnam, buf, sizeof rec.TKnam );

					rec.tkari	= _atoh( (LPSTR)(LPCTSTR)m_pZm->tkrec->tkari );
					rec.tkasi	= _atoh( (LPSTR)(LPCTSTR)m_pZm->tkrec->tkasi );

					m_KanaTKrec.Add( rec );
					i++;
				} while( m_pZm->tkrec->MoveNext() == 0 );
			}
#endif
		}
		tkrec_kanamode() = FALSE;

		m_BrnTkyo.set_tkquery( FALSE );
	}
	else {
		filter.Format( "tkana LIKE '%s%%' COLLATE Japanese_BIN", akbuf );
//		filter.Format( "tkana LIKE '%s%%' COLLATE Japanese_CS_AI", akbuf );

		sort = (m_KjunDsp == SL_NORMAL_MODE) ? 0 : 1;

		m_KanaTKrec.RemoveAll();

		if( m_pZm->tkrec->Requery( filter, sort ) == 0 ) {
#ifdef TKYARY_CLOSE
			i = m_pZm->tkrec->GetRecordCount();
#else
			DBTKREC	rec = {0};
			char buf[128];

			m_pZm->tkrec->MoveFirst();
			if( m_pZm->tkrec->st != -1 ) {
				do {
					rec.TKcod	= m_pZm->tkrec->tkcod;

					sprintf_s( buf, sizeof buf, "%s",  m_pZm->tkrec->tkana );
					memcpy( rec.TKana, buf, sizeof rec.TKana );

					rec.TKcsg	= m_pZm->tkrec->tksgn[0];
					rec.TKszk	= m_pZm->tkrec->tksgn[1];

					sprintf_s( buf, sizeof buf, "%s",  m_pZm->tkrec->tkname );
					memcpy( rec.TKnam, buf, sizeof rec.TKnam );

					rec.tkari	= _atoh( (LPSTR)(LPCTSTR)m_pZm->tkrec->tkari );
					rec.tkasi	= _atoh( (LPSTR)(LPCTSTR)m_pZm->tkrec->tkasi );

					m_KanaTKrec.Add( rec );
					i++;
				} while( m_pZm->tkrec->MoveNext() == 0 );
			}
#endif
		}

#ifdef TKYARY_CLOSE
		tkrec_kanamode() = TRUE;
		m_BrnTkyo.set_tkquery( TRUE );
#else
		m_pZm->tkrec->Requery( "", 0 );
		tkrec_kanamode() = FALSE;
		m_BrnTkyo.set_tkquery( FALSE );
#endif
	}

	return i;
}

/*===============================
	五十音摘要表示テーブル作成
================================*/
BOOL CSelectSub::make_50tky()
{
	if( m_SW_TK50 ) {
		tky50_reload();
		if( ! m_SW_TK50 )	return FALSE;

		m_TkySign = KANATKNO;

		m_T50page = 1;
		m_T50pmax = m_SW_TK50 / AC_MAX;
		if( m_SW_TK50 % AC_MAX ) m_T50pmax++;

		// 全摘要時 ページ保持
		if( m_NowPar.sel_pn == SL_TKYALL_PN ) {
			if( m_tallPG > m_T50pmax )	m_tallPG = m_T50pmax;
			m_T50page = m_tallPG;
		}

		int dcnt = ( m_T50pmax == 1 ) ? m_SW_TK50 : AC_MAX;
		tky50disp( m_T50page, dcnt );

		return TRUE;
	}
	return FALSE;
}


/*------------------------
	50ｵﾝ ﾃｷﾖｳ 再リード
-------------------------*/
void CSelectSub::tky50_reload()
{
	if( m_NowPar.sel_pn == SL_TKYALL_PN )
		return;

	int cnt = m_KanaTKrec.GetCount();

	CString filter;
	filter = _T( "seq = 7" );	// = 'tkrec'
	m_pZm->dcntl->Requery( filter );

	// 自動仕訳で全摘要リードされていたら、カナで再リード
	if( cnt >= m_pZm->dcntl->cnt ) {
		if( m_save_50kana[0] != 0 ) {
			m_SW_TK50 = tkrsrch( m_save_50kana );
		}
		else {
			tky50_clear();
		}
	}
}


/*------------------------
	50ｵﾝ ﾃｷﾖｳ クリア
-------------------------*/
void CSelectSub::tky50_clear()
{
	m_KanaTKrec.RemoveAll();
	m_T50page = m_T50pmax = m_SW_TK50 = 0;
}



/*------------------------
	50ｵﾝ ﾃｷﾖｳ ﾋｮｳｼﾞ
-------------------------*/
void CSelectSub::tky50disp( int pg, int cnt )
{
	int	i, n, l, selpnt, tkln;
	char temp[64];
	CString tmp;

//	p_DBDinpView->set_valtitle( "《摘要辞書》" );
	strcpy_s( m_spp.seljob, sizeof m_spp.seljob, "《摘要辞書》" );

	n = selpnt = 0;

	::ZeroMemory( TKSEL_PG, sizeof TKSEL_PG );

#ifdef TKYARY_CLOSE
	m_pZm->tkrec->MoveFirst();
	i = 0;
	do {
		if( m_pZm->tkrec->st != 0 )	break;

		if( ( AC_MAX * (pg-1) ) > i++ )
			continue;

		tmp = m_pZm->tkrec->tkname;
		tkln = tmp.GetLength();
		memcpy(temp, tmp, tkln);
		temp[tkln] = '\0';
		l = get_strcnt( (char*)(LPCTSTR)tmp, SEL_LEN/*tkln*/ );
		temp[l] = '\0';

		TKSEL_PG[selpnt] = m_pZm->tkrec->tkcod;

		SelVal32[selpnt].number = selpnt+/*(isEntrySys() ? 9 : 1) */ 1;
		SelVal32[selpnt++].Data = temp;

		if( ++n >= AC_MAX )
			break;
	//	i++;

	} while( m_pZm->tkrec->MoveNext() == 0 );
#endif

	INT_PTR	tkMax = m_KanaTKrec.GetCount();
	for( i = 0; i < tkMax; i++ ) {
		if( ( AC_MAX * (pg-1) ) > i )
			continue;

		tmp = m_KanaTKrec[i].TKnam;
		tkln = tmp.GetLength();
		memcpy(temp, tmp, tkln);
		temp[tkln] = '\0';
		l = get_strcnt( (char*)(LPCTSTR)tmp, SEL_LEN/*tkln*/ );
		temp[l] = '\0';

		TKSEL_PG[selpnt] = m_KanaTKrec[i].TKcod;

		SelVal32[selpnt].number = selpnt+/*(isEntrySys() ? 9 : 1) */ 1;
		SelVal32[selpnt++].Data = temp;

		if( ++n >= AC_MAX )
			break;
	}

	if( selpnt < AC_MAX ) {
		for( i = selpnt; i < AC_MAX; i++ ) {
			SelVal32[i].number = i+/*(isEntrySys() ? 9 : 1) */ 1;
			SelVal32[i].Data = "";
		}
	}
}


/***********************************
	連想摘要表示
************************************/
int CSelectSub::rentkydsp( void )	
{
	/* 連想摘要表示 */
	if( m_pZm->zvol->tk_ln /*&& !((uchar)CREC.darec._DSGN & 0x80)*/ ) {

#ifdef CLOSE	// 10.27 /98
		if( m_RTKcnt[0] + m_RTKcnt[1] ) {
#endif
			rentdsp( m_RTpage, 0 );
			rentdsp( m_RTpage, 1 );

			m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
		//	renttl();
			m_TkySign = RENTKNO;

#ifdef CLOSE	// 10.27 /98
		}
#endif
	}

#ifndef CLOSE	// 11.11 /98
	return m_RTKcnt[0] || m_RTKcnt[1] ? 0 : -1;
#endif
}


//連想摘要検索＆表示
int CSelectSub::rentky_srdsp( CString debt, CString cred )
{

	if( !m_TkySign ) m_TkySign = 1;

	if( m_TkySign == RENTKNO ) {
		tky_atswk_dsp( debt, cred, FALSE ); //#* 12.09 /02
		_rentky_str( debt, cred );	
	}
	else if( m_TkySign == SWKTKNO )  {
		_rentky_str( debt, cred );	
		return tky_atswk_dsp( debt, cred ); //#* 12.09 /02
	}
	//表示
#ifndef CLOSE	// 11.11 /98
	return rentkydsp();
#else
	rentkydsp();
#endif
}

// 03.06 /02
// 連想摘要のデータをバッファにセット(m_RTKblk, m_RTKcnt, etc)
void CSelectSub::_rentky_str( CString debt, CString cred )
{
	int dcsw, rtb_no;
	CString kncode;

	m_RTpage = 1;

	for( dcsw = 0 ; dcsw < 2 ; ++dcsw )
	{
		kncode = !dcsw ? debt : cred;

		struct	_DBKNREC* pKn;
		if( pKn = DB_PjisToKnrec( kncode, m_pZm ) ) {
			rtb_no = pKn->knrtb;
		}
		else
			rtb_no = 0;

		/* ﾚﾝｿｳ ﾃｷﾖｳ ﾌﾞﾛｯｸ ﾁｪｯｸ */
		if( m_pZm->zvol->tk_ln /*&& INP_mode != _RETREV*/ ) {
			if( ! rtb_no ) {
				m_RTKblk[dcsw] = -1;
				m_RTKcnt[dcsw] = 0;
			}
			else if( m_RTKblk[dcsw] != rtb_no ) {
				m_RTKblk[dcsw] = rtb_no;
				m_RTKcnt[dcsw] = rentksr( m_RTKblk[dcsw] );
			}
		}
		else m_RTKcnt[dcsw] = 0;

	}
}

//------------------------------------------
//	連想摘要数を返送
//
//
//------------------------------------------
int CSelectSub::rentksr( short arg2 )
{
	int		i, rtno;	

	rtno = arg2;

	m_pZm->rtrec->MoveFirst();
	i = 0;
	do {
		if( rtno == m_pZm->rtrec->rtpgn ) {
			if( (m_pZm->rtrec->rtcod < 1) ) continue;
			
			i++;
		}
	} while( m_pZm->rtrec->MoveNext() == 0 );

	return i;
}

// 連想摘要マックスページを取得
int CSelectSub::RTpmax_Get()
{
	int max, rtmax = 0;

	for( int i = 0; i < 2; i++ ) {
		max = ( m_RTKcnt[i] / DP_MAX );
		if( max <= 0 )	max = 1;
		else {
			if( m_RTKcnt[i] % DP_MAX )
				max++;
		}
		if( max > rtmax )
			rtmax = max;
	}

	return rtmax;
}



/*================================
	12.09 /02
	自動仕訳ファイルより摘要表示
==================================*/
int CSelectSub::tky_atswk_dsp( CString deb, CString cre, BOOL bDISP/*=TRUE*/ )
{
	int cnt;

	{
	DWORD	dwFlg = 0;

	if( (m_dwOPT&0x01) ) dwFlg = 1;

	//	if( GET_MASTERTYPE() != DRIVE_FD )
			cnt = swktky_search( dwFlg, deb, cre, m_pZm );
	//	else
	//		cnt = 0;

		m_SW_AutoTK = cnt;	
	//	if( cnt )
	//	{
			if( bDISP ) {
				if( make_autotky() ) {	// <- use to m_SW_AutoTK
					m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
				}
			}
	//	}
		return 0;
	}
	return -1;
}

/*====================================================================
	12.09 /02
	自動仕訳摘要表示テーブル作成
	
	BOOL bLastDisp .... TRUEのとき 最終データを表示
						FALSE のとき 1ページ目より
  ====================================================================*/
BOOL CSelectSub::make_autotky( BOOL bLastDisp /*= FALSE*/ )
{
//	if( m_SW_AutoTK ) {
		m_TkySign = SWKTKNO; 
		m_TATpage = 1;
		m_TATpmax = m_SW_AutoTK / AC_MAX;
		if( m_SW_AutoTK % AC_MAX ) m_TATpmax++;

		if( bLastDisp ) {
//			int data_pos = 0;

			// 最終ページを表示
			if( m_TATpmax > 0 ) {
				m_TATpage = m_TATpmax;
//				data_pos = ( AC_MAX*(m_TATpage-1) );
			}

			tkauto_dsp( m_TATpage );
		}
		else {
			tkauto_dsp( m_TATpage );
		}

		return TRUE;
//	}
//	return FALSE;
}


/*=============================================
	自動仕訳摘要を 選択ﾊﾞｯﾌｧ に ｾｯﾄ
	int pg ... 表示データページ 
  =============================================*/
void CSelectSub::tkauto_dsp( int pg )
{
	int		i, n, tkln, l;
	int selpnt;
	char temp[64];
	CString tmp;

//	fixclr();
//	p_DBDinpView->set_valtitle( "《仕訳対応摘要》" );
	strcpy_s( m_spp.seljob, sizeof m_spp.seljob, "《仕訳対応摘要》" );

	n = selpnt = 0;

	m_pZm->strec->MoveFirst();
	::ZeroMemory( TKSEL_PG, sizeof TKSEL_PG );
	i = 0;

<<<<<<< .working
	do {
		if( m_pZm->strec->st != 0 )	break;
||||||| .merge-left.r50
	if( m_RTSWKkana ) {
		m_TKYkana.RemoveAll();
=======
	if( m_dwOPT&0x04 ) {
		m_TKYkana.RemoveAll();
>>>>>>> .merge-right.r51

		if( ( AC_MAX * (pg-1) ) > i++ )
			continue;

		if( DB_TekiyoNoRead( m_pZm->strec->stcod ) == 0 ) {
			tmp = m_pZm->tkrec->tkname;
			tkln = tmp.GetLength();
			memcpy(temp, tmp, tkln);
			temp[tkln] = '\0';
			l = get_strcnt( (char*)(LPCTSTR)tmp, SEL_LEN/*tkln*/ );
			temp[l] = '\0';

			TKSEL_PG[selpnt] = m_pZm->tkrec->tkcod;

			SelVal32[selpnt].number = selpnt+ /*(isEntrySys() ? 9 : 1)*/ 1;
			SelVal32[selpnt++].Data = temp;

			if( ++n >= AC_MAX )
				break;
			//	i++;
		}
	} while( m_pZm->strec->MoveNext() == 0 );

	if( selpnt < AC_MAX ) {
		for( i = selpnt; i < AC_MAX; i++ ) {
			SelVal32[i].number = i+/*(isEntrySys() ? 9 : 1) */ 1;
			SelVal32[i].Data = "";
		}
	}
}


int CSelectSub::DB_TekiyoNoRead( int tkcod )
{
	int ret = -1;
	if( tkcod < 1 )	return -1;

	if( tkrec_kanamode() ) {
		m_pZm->tkrec->Requery( "", 0 );
		tkrec_kanamode() = FALSE;
	}
	if( m_pZm->tkrec->Find( tkcod ) == 1 ) ret = 0;
/*	if( m_pZm->tkrec->MoveFirst() == 0 ) {
		do {
			if( tkcod == m_pZm->tkrec->tkcod ) {
				ret = 0;
				break;
			}
		} while( m_pZm->tkrec->MoveNext() == 0 );
	}*/

	return ret;

}


int CSelectSub::DB_KanaTekiyoNoRead( int tkcod )
{
	int ret = -1;
	if( tkcod < 1 )	return -1;

	if( m_pZm->tkrec->Find( tkcod ) == 1 ) ret = 0;

	return ret;
}



//----------------------------------------------------------
//	変動事由選択部分
//	int nChar ....... キーコード(-1で初期表示)
//	BOOL bNowPage ... TRUE で現在ページを表示する
//
//	返送値	-1 = 該当仕訳が変動事由対象仕訳でない。
//----------------------------------------------------------
int	CSelectSub::_HjiyuSelect( int nChar, BOOL bNowPage/*=FALSE*/ )
{
	int pg, pgcnt, typ, max;

	//変動事由タイプ
	typ = get_khkbntype( m_NowPar.dbt, m_NowPar.cre, m_pZm );
	
	if( typ == -1 ) {
		strcpy_s( m_spp.seljob, sizeof m_spp.seljob, "《変動事由》" );
		for( int i = 0; i < m_PageMax; i++ ) { 
			SelVal32[i].number = i+1;
			SelVal32[i].Data = "";
		}
		m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
		return -1;
	}

	if( typ == 0 ) {
		max = m_KabNam.GetCount();
	}
	else {
		max = m_IgaiNam.GetCount();
	}

	if( nChar == -1 ) {
		if( ! bNowPage ) 
			m_HjiyuPG = 1;
		_HjiyuSelectSub( m_HjiyuPG, typ );
	}
	else if( nChar == VK_NEXT ) {
		pg = m_HjiyuPG;
		pg++;

		pgcnt = (max / m_PageMax);
		if( (max % m_PageMax) )	pgcnt++;

		if( pgcnt >= pg ) { 
			m_HjiyuPG = pg;
			_HjiyuSelectSub( m_HjiyuPG, typ );
		}
	}
	else if( nChar == VK_PRIOR ) {
		if( m_HjiyuPG > 0 ) {
			m_HjiyuPG--;
			_HjiyuSelectSub( m_HjiyuPG, typ );
		}
	}

	return 0;
}

//----------------------------------------------------------
//	変動事由データ読み込み部分
//	int page ....... 表示ページ
//----------------------------------------------------------
void CSelectSub::_HjiyuSelectSub( int page, int typ )
{
	int i, cnt;
	i = cnt = 0;
	char tmp[128], fmt[36];

	if( typ == 0 ) {
		strcpy_s( m_spp.seljob, sizeof m_spp.seljob, "《株主資本の変動事由》" );

		for( i = 0; i < m_KabNam.GetCount(); i++ ) {
			if( (i+1) > ((page-1) * m_PageMax) ) {
				cnt++;
				if( cnt > m_PageMax )
					break;
			
				sprintf_s( tmp, sizeof tmp, "%s", m_KabNam[i].hn_name );
				sprintf_s( fmt, sizeof fmt, "%%.%ds", get_strcnt( tmp, SEL_LEN ) );

				SelVal32[cnt-1].number = cnt;
				SelVal32[cnt-1].Data.Format( fmt, m_KabNam[i].hn_name );
			}
		}
	}
	else {
		strcpy_s( m_spp.seljob, sizeof m_spp.seljob, "《株主資本以外の変動事由》" );

		for( i = 0; i < m_IgaiNam.GetCount(); i++ ) {
			if( (i+1) > ((page-1) * m_PageMax) ) {
				cnt++;
				if( cnt > m_PageMax )
					break;
			
				sprintf_s( tmp, sizeof tmp, "%s", m_IgaiNam[i].hn_name );
				sprintf_s( fmt, sizeof fmt, "%%.%ds", get_strcnt( tmp, SEL_LEN ) );

				SelVal32[cnt-1].number = cnt;
				SelVal32[cnt-1].Data.Format( fmt, m_IgaiNam[i].hn_name );
			}
		}
	}

	if( cnt < m_PageMax ) {
		for( i = cnt; i < m_PageMax; i++ ) { 
			SelVal32[i].number = i+1;
			SelVal32[i].Data = "";
		}
	}

	m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
}

//-----------------------------------------------------
//	選択結果を取得
//	SelResult* result ...	選択結果格納
//	int page　...........	選択ページ
//	int sel_idx .........	選択番号(1～)
//-----------------------------------------------------
BOOL CSelectSub::_GetHjiyuSelect( SelResult* result, int page, int sel_no )
{
	BOOL ret = FALSE;
	int i, cnt, typ, max;
	KSH_NAMDTA* pKsh;
	i = cnt = 0;

	if( sel_no > m_PageMax )
		return FALSE;

	//変動事由タイプ
	typ = get_khkbntype( m_NowPar.dbt, m_NowPar.cre, m_pZm );
	
	if( typ == -1 )	return FALSE;

	if( typ == 0 ) {
		max = m_KabNam.GetCount();
	}
	else {
		max = m_IgaiNam.GetCount();
	}

	for( i = 0; i < max; i++ ) {

		if( typ == 0 ) {
			pKsh = &m_KabNam[i];
		}
		else {
			pKsh = &m_IgaiNam[i];
		}

		if( (i+1) > ((page-1) * m_PageMax) ) {
			cnt++;
			if( cnt > m_PageMax )
				break;

			if( sel_no == cnt ) {
				ret = TRUE;
				result->code	= pKsh->hn_cod;
				result->selname	= pKsh->hn_name;
				break;
			}
		}
	}

	return ret;
}


//--------------------------------------------------------------------
//	変動事由コードが変動事由ファイルに登録されているか？
//	　データ抽出等で 変動事由ファイルにないコードが仕訳に登録
//	　されている場合の修正をするため
//	int h_code ... 変動事由コード
//
//	return TRUE 事由が登録されている, FALSE 事由未登録
//--------------------------------------------------------------------
BOOL CSelectSub::IsHenJiyuCodeRegisted( int h_code )
{
	int max, i;
	BOOL bRet = FALSE;

	if( h_code <= 0 )	return FALSE;

	max = m_KabNam.GetCount();

	for( i = 0; i < max; i++ ) {
		if( m_KabNam[i].hn_cod == h_code ) {
			bRet = TRUE;
			break;
		}
	}

	if( ! bRet ) {
		max = m_IgaiNam.GetCount();
		for( i = 0; i < max; i++ ) {
			if( m_IgaiNam[i].hn_cod == h_code ) {
				bRet = TRUE;
				break;
			}
		}
	}

	return bRet;

}

//事由コードより名称を取得
//	BYTE	jycode ---- 変動事由コード
//	char*	pJynam ---- 変動事由名称バッファ(40Byte以上を指定)
//
BOOL CSelectSub::GetJiyuMeisyo( BYTE jycode, char* pJynam )
{
	int i, len;
	BOOL bRet = FALSE;

	if( (jycode >=  1 && jycode <= 20) ||
		(jycode >= 31 && jycode <= 50) ) {
		// 株主資本の変動事由
		for( i = 0; i < m_KabNam.GetCount(); i++ ) {
			if( m_KabNam[i].hn_cod == jycode ) {
				if( ! m_KabNam[i].hn_name.IsEmpty() ) {
					len = m_KabNam[i].hn_name.GetLength();
					memcpy( pJynam, m_KabNam[i].hn_name, len );
					pJynam[len] = '\0';
					bRet = TRUE;
				}
				break;
			}
		}
	}
	else if( (jycode >= 61 && jycode <= 80) ) {
		// 株主資本以外の変動事由
		for( i = 0; i < m_IgaiNam.GetCount(); i++ ) {
			if( m_IgaiNam[i].hn_cod == jycode ) {
				if( ! m_IgaiNam[i].hn_name.IsEmpty() ) {
					len = m_IgaiNam[i].hn_name.GetLength();
					memcpy( pJynam, m_IgaiNam[i].hn_name, len );
					pJynam[len] = '\0';
					bRet = TRUE;
				}
				break;
			}
		}
	}
#ifdef SOKYU_CLOSE
	else if( (jycode >= 86 && jycode <= 90) ) {
		// 遡及事由
		for( i = 0; i < m_KabNam.GetCount(); i++ ) {
			if( m_KabNam[i].hn_cod == jycode ) {
				if( ! m_KabNam[i].hn_name.IsEmpty() ) {
					len = m_KabNam[i].hn_name.GetLength();
					memcpy( pJynam, m_KabNam[i].hn_name, len );
					pJynam[len] = '\0';
					bRet = TRUE;
				}
				break;
			}
		}
	}
#endif

	return bRet;
}


//------------------------------------------
//	スキャン用変動事由データ作成
//		khkbn_read()呼び出し以後使用
//	return 1以上	スキャン項目数
//	return 0		変動事由なしマスター
//------------------------------------------
int  CSelectSub::ScanKHKbnMake()
{
	int i, max, cnt = 0;
	KSH_SCAN	ksh;
	m_KshScan.RemoveAll();

	if( ! m_bHjiyu ) {
		//公益法人の振替科目
		if( IsKouekiMaster( m_pZm )/*m_pZm->zvol->apno == 0x50*/ ) {
			ksh.item = 0;
			strcpy_s( ksh.ks_name, sizeof ksh.ks_name, "---------------------" );
			m_KshScan.Add( ksh );

			ksh.item = 100;
			strcpy_s( ksh.ks_name, sizeof ksh.ks_name, "   未登録仕訳" );
			m_KshScan.Add( ksh );

			ksh.item = 101;
			strcpy_s( ksh.ks_name, sizeof ksh.ks_name, "   振替科目対象仕訳" );
			m_KshScan.Add( ksh );

			max = m_NPfuri.GetCount();
			for( i = 0; i < max; i++ ) { 
				::ZeroMemory( &ksh, sizeof ksh );
				ksh.item = m_NPfuri[i].nf_swno;
				sprintf_s( ksh.ks_name, sizeof ksh.ks_name, "%2d.%.32s", m_NPfuri[i].nf_swno, m_NPfuri[i].nf_name );
				m_KshScan.Add( ksh );
			}
			cnt = m_KshScan.GetCount();
		}
		else if( IsSyafuKaisei( m_pZm->zvol ) ) {
			ksh.item = 0;
			strcpy_s( ksh.ks_name, sizeof ksh.ks_name, "---------------------" );
			m_KshScan.Add( ksh );

#ifdef NAIBU_VER2
			ksh.item = 100;
			strcpy_s( ksh.ks_name, sizeof ksh.ks_name, " 内部取引・未登録仕訳" );
			m_KshScan.Add( ksh );

			ksh.item = 101;
			strcpy_s( ksh.ks_name, sizeof ksh.ks_name, " 内部取引・該当区分" );
			m_KshScan.Add( ksh );

			ksh.item = 102;
			strcpy_s( ksh.ks_name, sizeof ksh.ks_name, " 内部取引・相手部門" );
			m_KshScan.Add( ksh );
#else
			ksh.item = 100;
			strcpy_s( ksh.ks_name, sizeof ksh.ks_name, " 内部取引・未登録仕訳" );
			m_KshScan.Add( ksh );

			ksh.item = 101;
			strcpy_s( ksh.ks_name, sizeof ksh.ks_name, " 内部取引・対象仕訳" );
			m_KshScan.Add( ksh );

			INT_PTR	max = m_NBdta.GetCount();

			for( i = 0; i < max; i++ ) {
				if( m_NBdta[i].nbt_code != 0 ) {
					ksh.item = 110 + m_NBdta[i].nbt_code;
					sprintf_s(  ksh.ks_name, sizeof ksh.ks_name, "　%s", m_NBdta[i].nbt_name );
					m_KshScan.Add( ksh );
				}
			}
#endif

			cnt = m_KshScan.GetCount();
		}
		//学校法人の振替科目
		else if( IsGakkouH27Master( m_pZm ) ) {
			ksh.item = 0;
			strcpy_s( ksh.ks_name, sizeof ksh.ks_name, "---------------------" );
			m_KshScan.Add( ksh );

			ksh.item = 100;
			strcpy_s( ksh.ks_name, sizeof ksh.ks_name, "   未登録仕訳" );
			m_KshScan.Add( ksh );

			ksh.item = 101;
			strcpy_s( ksh.ks_name, sizeof ksh.ks_name, "   資金調整科目対象仕訳" );
			m_KshScan.Add( ksh );

			max = m_NPfuri.GetCount();
			for( i = 0; i < max; i++ ) { 
				::ZeroMemory( &ksh, sizeof ksh );
				ksh.item = m_NPfuri[i].nf_swno;
				sprintf_s( ksh.ks_name, sizeof ksh.ks_name, "%2d.%.32s", m_NPfuri[i].nf_swno, m_NPfuri[i].nf_name );
				m_KshScan.Add( ksh );
			}
			cnt = m_KshScan.GetCount();
		}
	}
	else {
		ksh.item = 0;
		strcpy_s( ksh.ks_name, sizeof ksh.ks_name, "---------------------" );
		m_KshScan.Add( ksh );

		ksh.item = 100;
		strcpy_s( ksh.ks_name, sizeof ksh.ks_name, "   未登録仕訳" );
		m_KshScan.Add( ksh );

		ksh.item = 101;
		strcpy_s( ksh.ks_name, sizeof ksh.ks_name, "   変動事由対象仕訳" );
		m_KshScan.Add( ksh );

		//株主資本の変動事由
		max = m_KabNam.GetCount();
		for( i = 0; i < max; i++ ) { 
			::ZeroMemory( &ksh, sizeof ksh );
			ksh.item = m_KabNam[i].hn_cod;
#ifdef SOKYU_CLOSE
			if( (ksh.item >= 86 && ksh.item <= 90) ) { //遡及事由は『株主資本以外の変動事由』で セットする
				continue;
			}
#endif
			sprintf_s( ksh.ks_name, sizeof ksh.ks_name, "%2d.%.40s", m_KabNam[i].hn_cod, m_KabNam[i].hn_name );
			m_KshScan.Add( ksh );
		}
		//株主資本以外の変動事由
		max = m_IgaiNam.GetCount();
		for( i = 0; i < max; i++ ) { 
			::ZeroMemory( &ksh, sizeof ksh );
			ksh.item = m_IgaiNam[i].hn_cod;
			sprintf_s( ksh.ks_name, sizeof ksh.ks_name, "%2d.%.40s", m_IgaiNam[i].hn_cod, m_IgaiNam[i].hn_name );
			m_KshScan.Add( ksh );
		}

		cnt = m_KshScan.GetCount();
	}

	return cnt;
}

//------------------------------------------
//	スキャン用変動事由・振替科目 データ取得
//		ScanKHKbnMake()呼び出し以後使用
//	int index .... インデックス 
//	KSH_SCAN* .... pKshScan
//	
//	return -1	該当レコードなし
//	return  0	成功
//------------------------------------------
int  CSelectSub::GetScanKHKbn( int index, KSH_SCAN* pKshScan )
{
	if( index >= m_KshScan.GetCount() )
		return -1;

	*pKshScan = m_KshScan[index];

	return 0;
}


//------------------------------------------
//	変動事由データ数
//
//	int sgn		0:	株式
//				1:	株式以外
//	
//------------------------------------------
int  CSelectSub::GetHjiyuCount( int sgn )
{
	int cnt;

	if( ! m_bHjiyu )	return -1;

	if(sgn == 0) {
		cnt = m_KabNam.GetCount();
	}
	else {
		cnt = m_IgaiNam.GetCount();
	}

	return cnt;
}

//------------------------------------------
//	変動事由データを返送
//
//	int sgn		0:	株式
//				1:	株式以外
//	int index	該当データインデックス
//------------------------------------------
const KSH_NAMDTA* CSelectSub::GetHjiyuData( int sgn, int index )
{
	if( ! m_bHjiyu )	return NULL;

	if( sgn == 0 ) {
		return &m_KabNam[index];
	}
	else {
		return &m_IgaiNam[index];
	}
}


// 工事名称を取得
BOOL CSelectSub::_GetKojiName( CString& kjcode, CString& name, BOOL bFull )
{
	if( m_Kjn.kjnam == NULL )
		return FALSE;

	CString tmp;
	BOOL ret = FALSE;

	m_Kjn.kjnam->MoveFirst();
	if( m_Kjn.kjnam->st == -1 )	return FALSE;

	int klen;
	klen = m_pZm->zvol->kjcol;

	do {
		tmp = m_Kjn.kjnam->KKI_KJCD;
		tmp.TrimRight();

		LPSTR p = tmp.GetBuffer(256);
		p[ klen ] = '\0';

		if( tmp.CompareNoCase( kjcode ) == 0 ) {
			if( bFull )	name = m_Kjn.kjnam->KKI_NAME;	//正式名称
			else		name = m_Kjn.kjnam->KKI_RYNM;	//略称名称

			// ALL スペースチェック
			if( kjlen( (void*)(LPCTSTR)name, name.GetLength() ) )
				ret = TRUE;

			break;
		}
	} while( m_Kjn.kjnam->MoveNext() == 0 );

	return ret;
}

//
//	公益法人 振替科目？
//
BOOL CSelectSub::is_npfuridata( LPCTSTR deb, LPCTSTR cre )
{
	if( m_pNPb == NULL )	return FALSE;

	DWORD dwDeb, dwCre;

	dwDeb = _atoh( (LPSTR)deb );
	dwCre = _atoh( (LPSTR)cre );

	if( IsGakkouH27Master(m_pZm) ) {
		BOOL bRet = FALSE;

		for( int i = 0; i < m_SCHskn.GetCount(); i++ ) {
			if( dwDeb == m_SCHskn[i] || dwCre == m_SCHskn[i] ) {
				bRet = TRUE;
				break;
			}
		}
		return bRet;
	}

	if( dwDeb >= m_NPfcd[0] && dwDeb <= m_NPfcd[1] )
		return TRUE;

	if( dwCre >= m_NPfcd[0] && dwCre <= m_NPfcd[1] )
		return TRUE;

	return FALSE;
}

// 振替区分コードより 名称を取得
BOOL CSelectSub::get_npfuristr( int	frcode, CString& str )
{
	if( m_pNPb == NULL )	return FALSE;

	int i, max;
	BOOL	ret = FALSE;
	max = m_NPfuri.GetCount();

	for( i = 0; i < max; i++ ) {
		if( frcode == m_NPfuri[i].nf_swno ) {
			str = m_NPfuri[i].nf_name;
			ret = TRUE;
			break;
		}
	}

	return ret;
}

// 振替区分コードが、有効か？
BOOL CSelectSub::is_npfuricode( int	frcode )
{
	if( m_pNPb == NULL )	return FALSE;

	int i, max;
	BOOL	ret = FALSE;
	max = m_NPfuri.GetCount();

	for( i = 0; i < max; i++ ) {
		if( frcode == m_NPfuri[i].nf_swno ) {
			ret = TRUE;
			break;
		}
	}

	return ret;
}

// 振替科目を設定する科目の範囲
void CSelectSub::NPfuriHani( DWORD& scod, DWORD& ecod )
{
	scod = m_NPfcd[0];
	ecod = m_NPfcd[1];
}

// 振替科目データの個数
int	 CSelectSub::GetNPfuriCount()
{
	return m_NPfuri.GetCount();
}

// 振替科目データを取得
const NP_FURIDTA*	CSelectSub::GetNPfuriData(int index)
{
	int cnt;
	cnt = m_NPfuri.GetCount();

	if( cnt == 0 || index >= cnt )	return NULL;

	return &m_NPfuri[index];
}


//----------------------------------------------------------
//	公益法人 振替科目選択部分
//	int nChar ....... キーコード(-1で初期表示)
//	BOOL bNowPage ... TRUE で現在ページを表示する
//
//	返送値	-1 = 該当仕訳が振替科目対象仕訳でない。
//----------------------------------------------------------
int	CSelectSub::_NPfuriSelect( int nChar, BOOL bNowPage/*=FALSE*/ )
{
	int pg, pgcnt, max, i, cnt;
	char tmp[128], fmt[36];
	BOOL bDisp = FALSE;

	char* pFuriKmk	= "《振替科目区分》";
	char* pSknKmk	= "《資金調整科目区分》";

	if( ! is_npfuridata( m_NowPar.dbt, m_NowPar.cre ) ) {
		if( IsGakkouH27Master(m_pZm) ) {
			strcpy_s( m_spp.seljob, sizeof m_spp.seljob, pSknKmk );
		}
		else {
			strcpy_s( m_spp.seljob, sizeof m_spp.seljob, pFuriKmk );
		}
		for( int i = 0; i < m_PageMax; i++ ) { 
			SelVal32[i].number = i+1;
			SelVal32[i].Data = "";
		}
		m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
		return -1;
	}

	max = m_NPfuri.GetCount();

	if( nChar == -1 ) {
		if( ! bNowPage )	m_NPpage = 1;

		bDisp = TRUE;
	}
	else if( nChar == VK_NEXT ) {
		i = m_NPpage;
		i++;

		pgcnt = (max / m_PageMax);
		if( (max % m_PageMax) )	pgcnt++;

		if( pgcnt >= i ) { 
			m_NPpage = i;
			bDisp = TRUE;
		}
	}
	else if( nChar == VK_PRIOR ) {
		if( m_NPpage > 0 ) {
			m_NPpage--;
			bDisp = TRUE;
		}
	}

	i = cnt = 0;

	if( bDisp ) {
		pg = m_NPpage;

		if( IsGakkouH27Master(m_pZm) ) {
			strcpy_s( m_spp.seljob, sizeof m_spp.seljob, pSknKmk );
		}
		else {
			strcpy_s( m_spp.seljob, sizeof m_spp.seljob, pFuriKmk );
		}

		for( i = 0; i < max; i++ ) {
			if( (i+1) > ((pg-1) * m_PageMax) ) {
				cnt++;
				if( cnt > m_PageMax )
					break;
			
				sprintf_s( tmp, sizeof tmp, "%s", m_NPfuri[i].nf_name );
				sprintf_s( fmt, sizeof fmt, "%%.%ds", get_strcnt( tmp, SEL_LEN ) );

				SelVal32[cnt-1].number = cnt;
				SelVal32[cnt-1].Data.Format( fmt, m_NPfuri[i].nf_name );
			}
		}
	}

	if( cnt < m_PageMax ) {
		for( i = cnt; i < m_PageMax; i++ ) { 
			SelVal32[i].number = i+1;
			SelVal32[i].Data = "";
		}
	}

	m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );

	return 0;
}

//-----------------------------------------------------
//	選択結果を取得
//	SelResult* result ...	選択結果格納
//	int page　...........	選択ページ
//	int sel_idx .........	選択番号(1～)
//-----------------------------------------------------
BOOL CSelectSub::_GetNPfuriSelect( SelResult* result, int page, int sel_no )
{
	BOOL ret = FALSE;
	int i, cnt, max;
	i = cnt = 0;

	if( sel_no > m_PageMax )
		return FALSE;

	//変動事由タイプ
	if( ! is_npfuridata( m_NowPar.dbt, m_NowPar.cre ) )
		return FALSE;

	max = m_NPfuri.GetCount();

	for( i = 0; i < max; i++ ) {

		if( (i+1) > ((page-1) * m_PageMax) ) {
			cnt++;
			if( cnt > m_PageMax )
				break;

			if( sel_no == cnt ) {
				ret = TRUE;
				result->code	= m_NPfuri[i].nf_swno;
				result->selname	= m_NPfuri[i].nf_name;
				break;
			}
		}
	}

	return ret;
}


//---------------------------------------------------------
//	公益 収支科目選択
//	int nChar ... キーコード(-1で初期表示)
//
//---------------------------------------------------------
int	CSelectSub::_SyusSelect( int nChar )
{
	int pg, pgcnt;

	if( nChar == -1 ) {	// 初期表示

		reset_syus( m_SyusPG );
		m_BrnTkySel = 0;

		return 0;
	}
	else {
		if( nChar == VK_HOME )
		{
			if( ! m_SyusKanaPG ) {
				if( m_SyusPG == 1 ) return(0);
				m_SyusPG = 1;
			}
			reset_syus( m_SyusPG );

			return(0);
		}
		else if( nChar == VK_NEXT )
		{
			if( ! m_SyusKanaPG ) {
				pg = m_SyusPG;
				pg++;

				pgcnt = (m_SyusCnt / m_PageMax);
				if( (m_SyusCnt % m_PageMax) )	pgcnt++;

				if( pgcnt >= pg ) {
					m_SyusPG = pg;
				}
				else
					m_SyusPG = 1;

				reset_syus( m_SyusPG );
			}
			else {
				// 50音
				if( m_SyusKanaPG == PG_MAX || m_SyusKanaCnt <= (AC_MAX*m_SyusKanaPG) )
					return(0);
				m_SyusKanaPG++;
				kn50dsp( &KCD50C[AC_MAX*(m_SyusKanaPG-1)], 1 );
				m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
			}

			return(0);
		}
		else if( nChar == VK_PRIOR )
		{
			if( ! m_SyusKanaPG ) {
				if( m_SyusPG-1 >= 1 ) m_SyusPG--;
				else {
					pgcnt = (m_SyusCnt / m_PageMax);
					if( (m_SyusCnt % m_PageMax) )	pgcnt++;

					m_SyusPG = pgcnt;
				}
				
				reset_syus( m_SyusPG );
			}
			else {
				// 50音
				if( m_SyusKanaPG == 1 || m_SyusKanaCnt <= AC_MAX )
					return(0);

				m_SyusKanaPG--;
				kn50dsp( &KCD50C[AC_MAX*(m_SyusKanaPG-1)], 1 );
				m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
			}

			return(0);
		}
		return( nChar );
	}
}

// 収支科目 ページ単位表示
int CSelectSub::reset_syus( int page )
{
	int	i, n, cnt;

	m_spp.sp_nowpn = SL_NPSYUS_PN;
	strcpy_s( m_spp.seljob, sizeof m_spp.seljob, "《収支科目》" );

	m_SyusKanaPG		=	0;
	m_SyusKanaCnt	=	0;

	m_BmnPG		= 0;
	m_BrnTkySel	= 0;

	// 指定ページのデータ作成

	DBKNREC*	pSyrec;
	DWORD		dwCode;

	if( m_pNPb->pKsrec == NULL )
		goto END_TAG;

	n = cnt = 0;
	for( i = 0; i < m_pNPb->sknm_info->reccount; i++ ) {

		pSyrec = (m_pNPb->pKsrec+i);

		dwCode = pSyrec->kncod;
		hl_rev( &dwCode, 4 );

		if( dwCode < 0x08000000 ) {
			continue;
		}

		if( (pSyrec->knvoid & 0x01) )
			continue;

		n++;

		if( n > ((page-1) * m_PageMax) ) {
			cnt++;
			if( cnt > m_PageMax )
				break;

			SelVal32[cnt-1].number = cnt;
			SelVal32[cnt-1].Data = get_hyojiknam( i, 1 );
		}
	}

END_TAG:

	if( cnt < m_PageMax ) {
		for( i = cnt; i < m_PageMax; i++ ) { 
			SelVal32[i].number = i+1;
			SelVal32[i].Data = "";
		}
	}

	m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );

	return( cnt );
}


// 収支５０音検索
int CSelectSub::Syus_50search( char* kana )
{
	int i, j;
	CString sKana;
	
	sKana.Format( "%s", kana );

	struct _DBKNREC	*pKn;
	DWORD		dwCode;

	if( m_pNPb == NULL )	return FALSE;
	if( m_pNPb->pKsrec == NULL )	return FALSE;

	for( i = j = 0; i < m_pNPb->sknm_info->reccount; i++ ) {
		pKn = (m_pZm->pKsrec + i);
		
		// 不使用科目
		if( (pKn->knvoid & 0x01) )
			continue;

		dwCode = pKn->kncod;
		hl_rev( &dwCode, 4 );

		if( dwCode < 0x08000000 )
			continue;

		CString reckana;
		reckana = pKn->knkana;
		reckana.TrimRight();
		if( reckana.Find( sKana ) == 0 ) {
			if( j >= AC_MAX * PG_MAX ) break;
			memcpy( KCD50C[j].kd_kana, pKn->knkana, sizeof KCD50C[j].kd_kana );
			km_kana_check( KCD50C[j].kd_kana, sizeof sizeof KCD50C[j].kd_kana );

			KCD50C[j++].kd_seq = i+1;
		}
	}
	
	if( j ) {

		if( m_KjunDsp == SL_KJUN_DSP )
			qsort( KCD50C, j, sizeof KCD50C[0], kmkanacompare );

		m_SyusKanaPG	= 1;
		m_SyusKanaCnt	= j;

		for( ; j < AC_MAX * PG_MAX; j++ ) {
			::ZeroMemory( &KCD50C[j], sizeof KCD50C[0] );
		}

		kn50dsp( &KCD50C[0], 1 );
		return( TRUE );
	}
	else
		return( FALSE );

}

//-----------------------------------------------------------------------------
// 収支入力コード検索
//-----------------------------------------------------------------------------
// 引数	inpcd	：	入力コード
//-----------------------------------------------------------------------------
// 返送値	0	：	サーチ結果の表示
//			1	：	該当科目が１科目のみ
//			-1	：	該当科目無し
//-----------------------------------------------------------------------------
int CSelectSub::Syus_inpcodesearch( long inpcd )
{
	int i, j;
	struct _DBKNREC	*pKn;
	DWORD		dwCode;

	if( m_pNPb == NULL )	return FALSE;
	if( m_pNPb->pKsrec == NULL )	return FALSE;

	for( i=j=0; i<m_pNPb->sknm_info->reccount; i++ ){
		pKn = (m_pZm->pKsrec + i);
		
		// 不使用科目
		if( pKn->knvoid&0x01 ){
			continue;
		}

/*--*/
//		// 収支科目の業種制限は、しなくて良いか？
//		// 業種の制限
//		if( !check_kamokurestrict( pKn ) ){
//			continue;
//		}
/*--*/

		dwCode = pKn->kncod;
		hl_rev( &dwCode, 4 );
		if( dwCode < 0x08000000 ){
			continue;
		}

		if( pKn->knicod == inpcd ){
			if( j >= (AC_MAX*PG_MAX) ){
				break;
			}
			memcpy( KCD50C[j].kd_kana, pKn->knkana, sizeof KCD50C[j].kd_kana );
			KCD50C[j++].kd_seq = i+1;
		}
	}
	
	if( j == 1 ){
		return 1;
	}
	else if( j ){

		if( m_KjunDsp == SL_KJUN_DSP ){
			qsort( KCD50C, j, sizeof KCD50C[0], kmkanacompare );
		}

		m_SyusKanaPG	= 1;
		m_SyusKanaCnt	= j;

		for( ; j<(AC_MAX*PG_MAX); j++ ){
			::ZeroMemory( &KCD50C[j], sizeof KCD50C[0] );
		}

		kn50dsp( &KCD50C[0], 1 );
		return 0;
	}
	else{
		return -1;
	}
}

//
//	公益の収支科目用 アイテムチェック
//
BOOL CSelectSub::_NP_InputItemCheck(struct _KamokuData& ans, struct _KamokuData& check, int _dcsw, BOOL bSelClick/*= FALSE*/ )
{
	int found_i;
	int i, seln;
	int dcsw = _dcsw > 0 ? 1 : 0;
	CString code, km_cod;
	DWORD	dwCode;

	found_i = 1;

	// 06.30 /04 -- コード表示時は 2桁 入力でも コード入力として扱う
	BOOL bCodeInput = FALSE;
	BOOL bKamokuCode = (m_KmkMode == SL_KMK_CODE);

	int kamoku = atoi( check.kd_code );
	::ZeroMemory( &ans, sizeof(struct _KamokuData) );

	if( bKamokuCode && ! bSelClick ) {
		if( check.kd_sgn == 1 || check.kd_sgn == 5 ) {
			if( m_NowPar.sel_pn != SL_NPSKMINPCD_PN ) {		//03.28 /09
				bCodeInput = TRUE;
				char tmp[20];
				// kamokucode check
				sprintf_s( tmp, sizeof tmp, "%08d", kamoku );
				memcpy( check.kd_code, tmp, KMKCD_LN );
			}
		}
	}

	DBKNREC* pSyrec;
	DBKNREC* pKn = NULL;

	// コード入力
	if( !check.kd_sgn || check.kd_sgn == 4 || bCodeInput )
	{
		int inpcd = atoi( check.kd_code );
	
/*- '09.02.24 -*/
//		for( i = 0; i < m_pZm->sknm_info->reccount; i++ ) {
//			if( inpcd == (m_pZm->pKsrec + i)->knicod ) {
//				if( ! ((m_pZm->pKsrec+i)->knvoid & 0x01) ) {
//					pKn = (m_pZm->pKsrec + i);
//					break;
//				}
//			}
//		}
/*-------------*/
		SelPar par;
		par.sel_pn = SL_NPSKMINPCD_PN;
		::ZeroMemory( par.kmk_kana, sizeof par.kmk_kana );
		SelParEx parex;
		::ZeroMemory( &parex, sizeof SelParEx );
		parex.kmk_inpcd = inpcd;
		// 表示する科目が一つだけの時には、その科目名称レコードを返す
		if( _SelectJob(&par, -1, &parex) == 1 ){
			for( i = 0; i < m_pZm->sknm_info->reccount; i++ ) {
				if( inpcd == (m_pZm->pKsrec + i)->knicod ) {
					if( ! ((m_pZm->pKsrec+i)->knvoid & 0x01) ) {
						pKn = (m_pZm->pKsrec + i);
						break;
					}
				}
			}
		}
/*-------------*/
	}
	//番号選択
	else if( check.kd_sgn == 1 || check.kd_sgn == 5 )
	{
		seln = atoi( check.kd_code );
		int kmseq = 0;

		{
			if( seln < 1 )
				found_i = 0;

			if( m_SyusKanaPG ) {
				// カナ検索
				if( seln >= m_SyusKanaCnt+1-AC_MAX*(m_SyusKanaPG-1) )
					found_i = 0;
				else
					kmseq = KCD50C[seln-1 + AC_MAX*(m_SyusKanaPG-1)].kd_seq;

			}
			else {
				int n, cnt;
				n = cnt = 0;
				for( i = 0; i < m_pZm->sknm_info->reccount; i++ ) {
					pSyrec = (m_pZm->pKsrec+i);

					dwCode = pSyrec->kncod;
					hl_rev( &dwCode, 4 );

					if( dwCode < 0x08000000 ) {
						continue;
					}

					if( (pSyrec->knvoid & 0x01) )
						continue;

					n++;

					if( n > ((m_SyusPG-1) * m_PageMax) ) {
						cnt++;
						if( cnt > m_PageMax )
							break;

						if( cnt == seln ) {
							pKn = pSyrec;
							break;
						}
					}
				}
			}

			if( found_i && ! pKn ) {
				for( i = 0; i <  m_pZm->sknm_info->reccount; i++ ) {
					if( kmseq-1 == i ) {
						pKn = (m_pZm->pKsrec + i);
						break;
					}
				}
			}
		}
	}
	// 枝番のみ
	else
	{
		km_cod = check.kd_code;

		pKn = DB_SyusiPjisToKnrec( km_cod, m_pZm );
	}

	if( ! pKn || !found_i )
	{
		return( FALSE );
	}
	else
	{
		memmove( (char *)ans.kd_name, (LPCTSTR)pKn->knnam, KMNLEN );
		DWORDcodeToSTRcode( pKn->kncod, ans.kd_code, sizeof ans.kd_code );
		ans.kd_eda = check.kd_eda;

		return( TRUE );
	}

	return FALSE;
}


//  科目の表示制限
void CSelectSub::_KamokuRestrict( DWORD dwFlag )
{
	m_DRsgn	=	dwFlag;
	int i,j;

	// 現在表示中の科目を更新
	switch( m_NowPar.sel_pn ) {
	case SL_KAMOKU_PN:
	case SL_KMKANA_PN:
	case SL_KMINPCD_PN:
	case SL_KMKODER_PN: //-ver 1.0.3.14  //091228
	case SL_KMHANI_PN:  // 科目範囲

	///	_init_kamoku(1);	// 固定科目等を再セット
	
		if(Order_flg) {//-ver 1.0.3.14  //100106
			int no_data = 0;

			DWORD start, end;
			start = end = 0;
			if( m_NowPar.sel_pn == SL_KMHANI_PN ) {
				start	= m_dwStart;
				end		= m_dwEnd;
			}
			_init_kamoku_oder(1, start, end);	// 固定科目等を再セット

			if( m_ACsel )
			{
				j = m_ACsel - 1;
				if( kmk_50search( m_NowPar.kmk_kana ) == FALSE ) no_data = m_ACsel;
		///			m_ACsel = 0;
				else {
					fixcreat( (int *)&FIXTBL[0] );
					m_fixproc( SelFix8, &m_spp, 8, m_pWnd );
				}

				for( i = 1 ; i < j ; ++i )	// 元表次ページまで送る
					_KamokuSelect( VK_NEXT );
			}

			if( !m_ACsel || no_data)
			{
			//	j = m_CDPG;
			//	m_CDPG = 1;
				restrict_resetpage();
				if(no_data) m_ACsel = no_data;
			}
			m_fixproc( SelFix8, &m_spp, 8, m_pWnd ); //091228
			m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
		}
		else {
			_init_kamoku(1);	// 固定科目等を再セット


			if( m_ACsel )
			{
				j = m_ACsel - 1;
				if( kmk_50search( m_NowPar.kmk_kana ) == FALSE )
					m_ACsel = 0;
				else {
					fixcreat( (int *)&FIXTBL[0] );
					m_fixproc( SelFix8, &m_spp, 8, m_pWnd );
				}

				for( i = 1 ; i < j ; ++i )	// 元表次ページまで送る
					_KamokuSelect( VK_NEXT );
			}

			if( !m_ACsel )
			{
			//	j = m_CDPG;
			//	m_CDPG = 1;
				restrict_resetpage();
			}

			m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
		}

		break;
	default:
		_init_kamoku(1);
		break;
	}
}

/*====================================================================================
	科目の表示制限（公益 '08.12.01）
  ------------------------------------------------------------------------------------
	引数	Jgykcd	：	事業費コード
			Knrkcd	：	管理費コード
  ====================================================================================*/
void CSelectSub::_NpKamokuRestrict( DWORD Jgykcd, DWORD Knrkcd )
{
	DWORD	oldJgy, oldKnri;

	oldJgy	=	m_DJgykcd;
	oldKnri	=	m_DKnrkcd;

	m_DJgykcd = Jgykcd;
	m_DKnrkcd = Knrkcd;

	int i,j;

	// 現在表示中の科目を更新
	switch( m_NowPar.sel_pn ) {
	case SL_KAMOKU_PN:
	case SL_KMKANA_PN:
	case SL_KMINPCD_PN:
	case SL_KMKODER_PN: //-ver 1.0.3.14
	case SL_KMHANI_PN:	// 科目範囲
	///	_init_kamoku(1);	// 固定科目等を再セット
	
		if(Order_flg) {//-ver 1.0.3.14
			DWORD start, end;
			start = end = 0;
			if( m_NowPar.sel_pn == SL_KMHANI_PN ) {
				start	= m_dwStart;
				end		= m_dwEnd;
			}
			_init_kamoku_oder(1, start, end);	// 固定科目等を再セット
		}
		else {
			_init_kamoku(1);	// 固定科目等を再セット
		}

		if( m_ACsel )
		{
			j = m_ACsel - 1;

/*- '09.03.11 -*/
//			if( kmk_50search( m_NowPar.kmk_kana ) == FALSE )
//				m_ACsel = 0;
//			else {
//				fixcreat( (int *)&FIXTBL[0] );
//				m_fixproc( SelFix8, &m_spp, 8, m_pWnd );
//			}
/*-------------*/

			if( m_NowPar.kmk_kana[0] ){
				if( kmk_50search( m_NowPar.kmk_kana ) == FALSE ){
					m_ACsel = 0;
				}
				else {
					if(Order_flg == 0){ //091228
						fixcreat( (int *)&FIXTBL[0] );
						m_fixproc( SelFix8, &m_spp, 8, m_pWnd );
					}
				}
			}
			else{
				int st;
				if( (st=kmk_inpcodesearch(m_NowParEx.kmk_inpcd)) < 0 ){
					m_ACsel = 0;
				}
				else{
					if(Order_flg == 0){ //091228
						fixcreat( (int *)&FIXTBL[0] );
						m_fixproc( SelFix8, &m_spp, 8, m_pWnd );
					}
				}
			}
/*-------------*/

			for( i = 1 ; i < j ; ++i )	// 元表次ページまで送る
				_KamokuSelect( VK_NEXT );
		}

		if( !m_ACsel )
		{
		//	j = m_CDPG;
		//	m_CDPG = 1;
	///-		if(!Order_flg){ // 091228
			
			if(Order_flg != Order_flg2)	NP_restrict_resetpage( 0, 0);				//通常と科目順を切り替えた場合常に変更なしとなってしまうため
			else if(Order_flg)			NP_restrict_resetpage( oldJgy, oldKnri);	//科目順となっている場合
			else						NP_restrict_resetpage( oldJgy, oldKnri);
	///-		}
	///-		else{
	///-			m_CDPG = 1;//091228
	///-			restrict_resetpage();
	///-		}
		}

		if(Order_flg)						m_fixproc( SelFix8, &m_spp, 8, m_pWnd );
		else if(Order_flg != Order_flg2)	m_fixproc( SelFix8, &m_spp, 8, m_pWnd );

		m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );

		break;
	

	default:
		_init_kamoku(1);
		break;
	}
}

//
// 指定ページを表示して、そのページに科目がない場合、科目があるページまでスキップする
//
int CSelectSub::restrict_resetpage()
{
	int	disppage, nowpage;
	disppage = nowpage = 1;

	int cnt = resetpag( nowpage );
	int i;

	if( cnt <= 0 || ! _chk_kamokuctg( nowpage ) ) {
		// 送り
		disppage++;
		for( i = disppage; i <= PG_MAX; i++ ) {
			cnt = resetpag( i );
			if( cnt > 0 && _chk_kamokuctg( i ) ) {
				nowpage = i;
				break;
			}
		}
	}

	m_CDPG = nowpage;
	return cnt;
}

//
//	貸借が共通のとき、表示するページを検索してから表示
//
BOOL CSelectSub::_chk_kamokuctg( int page )
{
	int kjn_type;
	kjn_type = IsKojinGyosyuMaster( m_pZm );
	if( kjn_type == 0 )	return TRUE;

	if( (m_pZm->zvol->ind_sgn2 & 0x10) )	return TRUE;

	int i, knpos;
	BOOL bRet = FALSE;
	DBKNREC* pKn;
	int chk;
	if( m_DRsgn == SL_IND_ALL )
		chk = (m_pZm->zvol->ind_sgn2&0x0f);
	else
		chk = m_DRsgn;

	for( i = 0; i < 32; i++ ) {
		knpos = KDPAG[page-1][i];

		if( knpos ) {
			pKn		= m_pZm->pKnrec + (knpos-1);
			if( (pKn->ind_ctg&0x0f) == chk ) {
				bRet = TRUE;
				break;
			}
		}
	}

	return bRet;
}


// [公益用]
// 指定ページを表示して、そのページに科目がない場合、科目があるページまでスキップする
//
//
int CSelectSub::NP_restrict_resetpage( DWORD oldJgy, DWORD oldKnri )
{
	int	disppage, nowpage, cnt;
	int i, n, knpos;
	DBKNREC* pKn;
	DWORD	chkcode = 0;
	BYTE	chk_bcod = 0;

#define ALL_JIKAN	(DWORD)0xffffffff

	// 指定ページの科目は、事業費 or 管理費 ?
	if( (oldJgy == ALL_JIKAN && m_DJgykcd != ALL_JIKAN) ||
		(oldJgy != ALL_JIKAN && m_DJgykcd == ALL_JIKAN) ||
		(oldJgy != m_DJgykcd) ) {

		chkcode		= m_DJgykcd;
		chk_bcod	= 0x09;
	}
	else if( (oldKnri == ALL_JIKAN && m_DKnrkcd != ALL_JIKAN) ||
			 (oldKnri != ALL_JIKAN && m_DKnrkcd == ALL_JIKAN) ||
			 (oldKnri != m_DKnrkcd) ) 
	{
		chkcode		= m_DKnrkcd;
		chk_bcod	= 0x0a;
	}
	//変更なし
	if( chkcode == 0 ) {
		cnt = resetpag( m_CDPG );
		return cnt;
	}

	disppage = nowpage = 1;

	// 送り
	BOOL bRet = FALSE;

	int entry_cnt = 0; //091228

	if(Order_flg)	entry_cnt = 40;
	else			entry_cnt = 32;

	for( i = disppage; i <= PG_MAX; i++ ) {
		cnt = resetpag( i );
		if( cnt > 0 ) {
			// 表示科目のチェック
			for( n = 0; n < entry_cnt; n++ ) { //091228  32固定→entry_cntに変更
				knpos = KDPAG[i-1][n];

				if( knpos ) {
					pKn		= m_pZm->pKnrec + (knpos-1);

					if( pKn->knbcod == chk_bcod ) {
						if( chkcode == 0xffffffff ) {
							bRet = TRUE;
							break;
						}
						else if( (pKn->kncod&0x000000ff) == (chkcode&0x000000ff) ) {
							bRet = TRUE;
							break;
						}
					}
				}
			}
			if( bRet ) {
				nowpage = i;
				break;
			}
		}
	}

	m_CDPG = nowpage;
	return cnt;
}


#ifdef _20081201_NP_EXTEND_
// 科目の表示制限チェック
BOOL CSelectSub::check_kamokurestrict( int knpos )
{
	struct _DBKNREC* pKn;

	// 個人兼業でない。
	int kjn_type;
	kjn_type = IsKojinGyosyuMaster( m_pZm );

	if( kjn_type == 0 )	return TRUE;

	// 単一業種

//	if( m_INDkmk.GetCount() == 0 )
//		return TRUE;

//	INDKMK_DTA* pind;

	pKn		= m_pZm->pKnrec + knpos;
//	pind	= &m_INDkmk[knpos];

//#ifdef _DEBUG
//	DWORD dwKmkCode = _atoh( pind->ind_code );
//	hl_rev( &dwKmkCode, 4 );
//	ASSERT( dwKmkCode == pKn->kncod );
//#endif

	return _check_kamokurestrict( kjn_type, pKn/*, pind*/ );
}
#endif

// 科目の表示制限チェック
BOOL CSelectSub::check_kamokurestrict( int knpos )
{
	struct _DBKNREC* pKn;

	// 個人兼業でない。
	int kjn_type;
	kjn_type = IsKojinGyosyuMaster( m_pZm );

	if( kjn_type != 0 ){
		pKn = m_pZm->pKnrec + knpos;

//#ifdef _DEBUG
//		DWORD dwKmkCode = _atoh( pind->ind_code );
//		hl_rev( &dwKmkCode, 4 );
//		ASSERT( dwKmkCode == pKn->kncod );
//#endif

		return _check_kamokurestrict( kjn_type, pKn/*, pind*/ );
	}
	else if( IsKouekiMaster(m_pZm) ){
		pKn = m_pZm->pKnrec + knpos;

		return _check_npkamokurestrict( pKn );
	}
	else{
		return TRUE;
	}
}

#ifdef _20081201_NP_EXTEND_
// 科目の表示制限チェック
BOOL CSelectSub::check_kamokurestrict( struct _DBKNREC* pKn )
{
	// 個人兼業でない。
	int kjn_type;
	kjn_type = IsKojinGyosyuMaster( m_pZm );

	if( kjn_type == 0 )	return TRUE;

#ifdef CLOSE
	INDKMK_DTA* pind;

	for( int n = 0; n < m_INDkmk.GetCount(); n++ ) {
		pind	= &m_INDkmk[n];

		DWORD dwKmkCode = _atoh( pind->ind_code );
		hl_rev( &dwKmkCode, 4 );
		if( dwKmkCode == pKn->kncod )
			break;

		pind = NULL;
	}
#endif

	BOOL bRet = TRUE;

#ifdef CLOSE
	if( pind != NULL )
		bRet = _check_kamokurestrict( kjn_type, pKn, pind );
#else
	bRet = _check_kamokurestrict( kjn_type, pKn );
#endif

	return bRet;

}
#endif

// 科目の表示制限チェック
BOOL CSelectSub::check_kamokurestrict( struct _DBKNREC* pKn )
{
	// 個人兼業でない。
	int kjn_type;
	kjn_type = IsKojinGyosyuMaster( m_pZm );

	BOOL bRet = TRUE;
	if( kjn_type != 0 ){
		bRet = _check_kamokurestrict( kjn_type, pKn );
	}
	else if( IsKouekiMaster(m_pZm) ){
		bRet = _check_npkamokurestrict( pKn );
	}

	return bRet;
}

//
// 実際の科目の表示制限チェック
// 
BOOL CSelectSub::_check_kamokurestrict( int kjn_type, DBKNREC* pKn/*, INDKMK_DTA* pInd*/ )
{
//	if( m_INDkmk.GetCount() == 0 )
//		return TRUE;

	DWORD	dwCode;

//#ifdef _DEBUG
//	DWORD dwKmkCode = _atoh( pInd->ind_code );
//	hl_rev( &dwKmkCode, 4 );
//	ASSERT( dwKmkCode == pKn->kncod );
//#endif

	int type = (kjn_type & 0x0f);

	if( pKn ) {
		dwCode	=	pKn->kncod;
		hl_rev( &dwCode, 4 );
	}
	else {
		return FALSE;
	}
//MyTrace( "_check_kamokurestrict type=%02x, (%08x), ind_ctg = %02x\n", type, dwCode, pKn->ind_ctg );

	// 単一の業種
	if( type == 0x01 || type == 0x02 || type == 0x04 ) {
		// 貸借と損益で扱いが異なる
		if( dwCode < 0x08000000 ) {
			if( pKn->ind_ctg == 0x00 || (pKn->ind_ctg & type) )
				return TRUE;
		}
		else {
			if( pKn->ind_ctg == (BYTE)type )
				return TRUE;
		}
	}
	else {
		// 兼業
		DWORD	chk;

		if( m_DRsgn == 0 ) {
			chk = 0;
		//	chk = type;
		//	if( chk == 0 )	chk = 0x01;
		}
		else {
			if( m_DRsgn & SL_IND_ALL )
				chk	=	type;
			else
				chk = m_DRsgn;
		}

		if( dwCode < 0x08000000 ) {
			if( (m_pZm->zvol->ind_sgn2 & 0x10) ) {
				// 貸借個別
				if( pKn->ind_ctg == 0x00 || (pKn->ind_ctg & chk) )
					return TRUE;
			}
			else {
#ifdef CLOSE
				// 貸借共通 各メインの科目を出すため、メイン以外のサインは落とす。
				BYTE	xorbit;
				xorbit = (m_pZm->zvol->ind_sgn2 ^ 0x07);
				chk &= ~(xorbit);

				if( pKn->ind_ctg == 0x00 || (pKn->ind_ctg & chk) )
					return TRUE;
#endif
				// 貸借共通は、絞込む使いにくいので、メインの科目を常に表示する
				// 100105 order
				if( pKn->ind_ctg == 0x00 || pKn->ind_ctg == 0x07 || (pKn->ind_ctg&0x0f) == m_pZm->zvol->ind_sgn2 ){

					if(Order_flg){
						if(pKn->ind_ctg == chk)					return TRUE;
						else if( chk == m_pZm->zvol->ind_type)	return TRUE;
						else if(pKn->ind_ctg == 0)				return TRUE;
						else if(chk == 0x07)					return TRUE;
						else									return FALSE;

					}
					return TRUE;
				}
			}
		}
		else {
			if( pKn->ind_ctg & (BYTE)chk )
				return TRUE;
		}
	}

	return FALSE;
}

/*====================================================================================
	実際の科目の表示制限チェック（公益 '08.12.01）
  ------------------------------------------------------------------------------------
	引数	pKn		：	チェック対象の科目名称レコード
  ------------------------------------------------------------------------------------
	返送値	TRUE	：	表示
			FALSE	：	非表示
  ====================================================================================*/
BOOL CSelectSub::_check_npkamokurestrict( DBKNREC* pKn )
{
	ASSERT( pKn );
	if( pKn == NULL ){
		return FALSE;
	}

	BOOL rt = TRUE;

	if( pKn->knbcod == 0x09 ){		// 事業費
		if( m_DJgykcd == 0xffffffff ){
			rt = TRUE;
		}
		else if( (m_DJgykcd&0x000000ff) != (pKn->kncod&0x000000ff) ){
			rt = FALSE;
		}
	}
	else if( pKn->knbcod == 0x0a ){	// 管理費
		if( m_DKnrkcd == 0xffffffff ){
			rt = TRUE;
		}
		else if( (m_DKnrkcd&0x000000ff) != (pKn->kncod&0x000000ff) ){
			rt = FALSE;
		}
	}

	return rt;
}

#ifdef SEL_KEEP /////////////////////////////////////////////////////////////////////////


// 固定科目選択画面状態ＳＷ
BOOL CSelectSub::fix8cnd()
{
	return fix8sg;
}

void CSelectSub::fix8cnd( BOOL sg )
{
	fix8sg = sg;
}

// 選択画面表示色状態
BOOL CSelectSub::fixvalcnd()
{
	return fixvalsg;
}

void CSelectSub::fixvalcnd( BOOL sg )
{
	fixvalsg = sg;
}


//
// 固定項目の表示
void CSelectSub::fix8disp( BOOL sg )
{
	if( sg )
	{
		// 固定科目選択のディセーブル／イネーブル
		enable_fix8( sg );

		// 選択タイトル表示のディセーブル／イネーブル
		valtitle( !sg );
	}
	else
	{
		// 選択タイトル表示のディセーブル／イネーブル
		valtitle( !sg );

		// 固定科目選択のディセーブル／イネーブル
		enable_fix8( sg );
	}

	fix8cnd( sg );

}

// 固定科目選択のディセーブル／イネーブル
void CSelectSub::enable_fix8( BOOL sg )
{
	if( flgPen )
	{
		RECT rect1,rect2;
		CWnd *m_pfix8,*m_pfix32;

//		VERIFY(m_pfix8 = GetDlgItem( IDC_ICSSELCTRL1 ));
//		VERIFY(m_pfix32 = GetDlgItem( IDC_ICSSELCTRL2 ));
		m_pfix8->GetWindowRect( &rect1 );
		pParent->ScreenToClient( &rect1 );
		m_pfix32->GetWindowRect( &rect2 );
		pParent->ScreenToClient( &rect2 );

		// New Size
		if( sg == TRUE )
			rect2.top = rect1.bottom/* + 1*/;
		else
			rect2.top = rect1.top;

		m_pfix32->SetWindowPos( 0, rect2.left, rect2.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER );

	}
	else
	{
		RECT newrect;
		double rate_w, rate_h;

		get_viewrate( rate_w, rate_h );	//ビューサイズ比率をゲット

		CWnd *m_pfix8;
//		VERIFY(m_pfix8 = GetDlgItem( IDC_ICSSELCTRL1 ));

		// New Size
		if( sg == TRUE )
		{
			newrect.top = 0;
			newrect.left = 0;
			newrect.bottom = m_selfix8_rect.bottom * rate_h;
			newrect.right = m_selfix8_rect.right * rate_w;
		}
		else
		{
			newrect.top = 0;
			newrect.left = 0;
			newrect.bottom = 0;
			newrect.right = 0;
		}
		m_pfix8->SetWindowPos( 0, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE );
	//	m_pfix8->UpdateWindow();
	}

}

//
// 選択画面の表示色
void CSelectSub::enable_fixval( BOOL sg )
{

	// New Size
	if( sg )
	{
		// 科目選択画面背景色の設定
		m_pfix8->SetBackColor( RGB_FIX8_BACK );
		m_pfix32->SetBackColor( RGB_VAL32_BACK );

		// 科目選択画面文字色の設定
		m_pfix8->SetForeColor( RGB_FIX8_FORE );
		m_pfix32->SetForeColor( RGB_VAL32_FORE );
		// 科目選択画面枠の色
		m_pfix8->SetLineColor( RGB_GRAY );
		m_pfix32->SetLineColor( RGB_GRAY );

		// 選択タイトル背景色、文字色
		ttlbaseorg->SetBackColor( RGB_VAL32_BACK );
		ttlorg->SetBackColor( RGB_VAL32_BACK );
		ttlorg->SetForeColor( RGB_VAL32_FORE );
	}
	else
	{
		// 科目選択画面背景色の設定
		m_pfix8->SetBackColor( RGB_FIX8_BACK_UNSEL );
		m_pfix32->SetBackColor( RGB_VAL32_BACK_UNSEL );
		// 科目選択画面文字色の設定
		m_pfix8->SetForeColor( RGB_FIX8_FORE_UNSEL );
		m_pfix32->SetForeColor( RGB_VAL32_FORE_UNSEL );
		// 科目選択画面枠の色
		m_pfix8->SetLineColor( RGB_GRAY );
		m_pfix32->SetLineColor( RGB_GRAY );

		// 選択タイトル背景色、文字色
		ttlbaseorg->SetBackColor( RGB_VAL32_BACK_UNSEL );
		ttlorg->SetBackColor( RGB_VAL32_BACK_UNSEL );
		ttlorg->SetForeColor( RGB_VAL32_FORE_UNSEL );

	}

//	// 表示
//	m_pfix8->UpdateWindow();
//	m_pfix32->UpdateWindow();
//	ttlbaseorg->UpdateWindow();
//	ttlorg->UpdateWindow();

	fixvalcnd( sg );
}

// 選択タイトルテキストセット
void CSelectSub::set_valtitle( char *msg )
{
	puts( IDC_VALTITLE, msg, CICSDisp );
}

// 選択タイトル表示のディセーブル／イネーブル
void CSelectSub::valtitle( BOOL sg )
{
	CWnd *m_pTitleBase,*m_pTilte;
	CWnd *m_pfix8,*m_pfix32;
	RECT rect8,rect32,newrect,newrect2;
	LONG v_h, v_w;
	double rate_w, rate_h;
	WINDOWPLACEMENT place;

	get_viewrate( rate_w, rate_h );	//ビューサイズ比率をゲット

/*
#ifdef	_ICSPEN_	// 01.20 /01
	//Penの時は表示無し
	if( IsPen() ) {
		m_pTitleBase->MoveWindow( 0,0,0,0 );
		m_pTilte->MoveWindow( 0,0,0,0 );
		return;
	}
#endif	//end of _ICSPEN_
*/
	// New Size
	if( sg == TRUE )
	{
		VERIFY(m_pfix8 = GetDlgItem( IDC_ICSSELCTRL1));
		VERIFY(m_pfix32 = GetDlgItem( IDC_ICSSELCTRL2));
		m_pfix8->GetWindowRect( &rect8 );
		m_pfix32->GetWindowRect( &rect32 );

		newrect.top = rect8.top;
		newrect.left = rect8.left;
		newrect.bottom = rect8.bottom;	// m_selfix8_rect.bottom * rate_h;
		newrect.right = rect8.right;		// m_selfix8_rect.right * rate_w;
		m_pTitleBase->SetWindowPos( 0, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE );

		newrect2 = m_valtitle_rect;

		newrect2.bottom = newrect2.bottom * rate_h;
		newrect2.right = newrect2.right * rate_w;

		v_h = (newrect.bottom - newrect.top) / 3;
		v_w = ((newrect.right - newrect.left) / 12);
		m_pTitleBase->GetWindowPlacement( &place );
		newrect2.top = place.rcNormalPosition.top + v_h;
		newrect2.left = place.rcNormalPosition.left + v_w;
		newrect2.bottom = newrect2.top + v_h;
		newrect2.right = newrect2.left + (v_w*10);

		m_pTilte->SetWindowPos( 0, newrect2.left, newrect2.top, newrect2.right - newrect2.left, newrect2.bottom - newrect2.top, SWP_NOZORDER|SWP_NOACTIVATE );
	}
	else
	{
		newrect.top = 0;
		newrect.left = 0;
		newrect.bottom = 0;
		newrect.right = 0;

		m_pTitleBase->SetWindowPos( 0, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE );
		m_pTilte->SetWindowPos( 0, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_NOZORDER|SWP_NOACTIVATE );
	}

//	m_pTitleBase->UpdateWindow();
//	m_pTilte->UpdateWindow();
}


#endif



//////////////////////////////////////////////////////////////////////////////////////////////
//class CSelectData

CSelectData::CSelectData()
{

	m_pSub = new CSelectSub;
}

CSelectData::~CSelectData()
{
	if( m_pSub != NULL ) {
		delete m_pSub;
	}

	m_pSub = NULL;
}

int CSelectData::Init( CDBZmSub* pZm, CWnd* pwnd, SELECTDATAPROC fix, SELECTDATAPROC sel32 )
{
	if( m_pSub == NULL )
		return -1;

	return m_pSub->InitSub( pZm, pwnd, fix, sel32 );
}

// 公益法人用
int CSelectData::Init( CDBNpSub* pNpSub, CWnd* pwnd, SELECTDATAPROC fix, SELECTDATAPROC sel32 )
{
	if( m_pSub == NULL )
		return -1;

	return m_pSub->InitSub( pNpSub, pwnd, fix, sel32 );
}

//
void CSelectData::Close()
{
	if( m_pSub == NULL )
		return;

//MyTrace("CSelectData::Close m_pSub %08x\n");

	m_pSub->CloseSub();

//MyTrace("CSelectData::Close 02\n");
}

void CSelectData::SetTekiyoMode( int mode )
{
	m_pSub->_SetTekiyoMode( mode );
}

void CSelectData::SetKmDispMode( int mode )
{
	m_pSub->_SetKmDispMode( mode );
}

void CSelectData::SetBrnTkyMode( int mode )
{
	m_pSub->_SetBrnTkyMode( mode );
}

void CSelectData::SetKanaDspMode( int mode )
{
	m_pSub->_SetKanaDspMode( mode );
}

void CSelectData::SetKamokuPageHold( BOOL bHold )
{
	m_pSub->_SetKamokuPageHold( bHold );
}

// 設定取得
int CSelectData::GetTekiyoMode()
{
	return m_pSub->_GetTekiyoMode();
}

int CSelectData::GetKmDispMode()
{
	return m_pSub->_GetKmDispMode();
}

int CSelectData::GetBrnTkyMode()
{
	return m_pSub->_GetBrnTkyMode();
}

int CSelectData::GetKanaDspMode()
{
	return m_pSub->_GetKanaDspMode();
}

BOOL CSelectData::GetKamokuPageHold()
{
	return m_pSub->_GetKamokuPageHold();
}


BRNTKY_PAR CSelectData::GetBrnTkyPar()
{
	return m_pSub->_GetBrnTkyPar();
}

int	CSelectData::SelectJob( SelPar* par, int nChar )
{
	return m_pSub->_SelectJob( par, nChar );
}

int	CSelectData::SelectJob( SelPar* par, int nChar, SelParEx* parex )
{
	return m_pSub->_SelectJob( par, nChar, parex );
}

int CSelectData::SelectUpdate()
{
	return m_pSub->_SelectUpdate();
}

struct _SelTable* CSelectData::GetNowFix8Select()
{
	return m_pSub->_GetNowFix8Select();
}

struct _SelTable* CSelectData::GetNowSel32Select()
{
	return m_pSub->_GetNowSel32Select();
}


COLORREF CSelectData::GetSel32TextColor()
{
	return m_pSub->_GetSel32TextColor();
}

COLORREF CSelectData::GetSel32BackColor()
{
	return m_pSub->_GetSel32BackColor();
}


BOOL CSelectData::InputItemCheck(struct _KamokuData& ans, struct _KamokuData& check, int _dcsw, BOOL bSelClick/*= FALSE*/ )
{
	return m_pSub->_InputItemCheck( ans, check, _dcsw, bSelClick );
}

BOOL CSelectData::GetSelResult( SelResult* result, char* selstr )
{
	return m_pSub->_GetSelResult( result, selstr );
}

//変動事由関係
BOOL CSelectData::IsHenJiyuCodeRegisted( int h_code )
{
	return m_pSub->IsHenJiyuCodeRegisted( h_code );
}

BOOL CSelectData::GetJiyuMeisyo( BYTE jycode, char* pJynam )
{
	return m_pSub->GetJiyuMeisyo( jycode, pJynam );
}

int  CSelectData::ScanKHKbnMake()
{
	return m_pSub->ScanKHKbnMake();
}

int  CSelectData::GetScanKHKbn( int index, KSH_SCAN* pKshScan )
{
	return m_pSub->GetScanKHKbn( index, pKshScan );
}


int	 CSelectData::GetHjiyuCount(int sgn)
{
	return	m_pSub->GetHjiyuCount( sgn );
}

const KSH_NAMDTA*	CSelectData::GetHjiyuData(int sgn, int index)
{
	return	m_pSub->GetHjiyuData( sgn, index );
}


BOOL CSelectData::GetKojiName( CString& kjcode, CString& name,BOOL bFull/*=FALSE*/ )
{
	return m_pSub->_GetKojiName( kjcode, name, bFull );
}

BOOL CSelectData::Kojicode_Ok( CString& kjcode )
{
	return m_pSub->_Kojicode_Ok( kjcode );
}


void CSelectData::tekiyo_update()
{
	m_pSub->tekiyo_update();
}


// 公益法人　振替科目
BOOL CSelectData::IsNPfuridata( LPCTSTR deb, LPCTSTR cre )
{
	return m_pSub->is_npfuridata( deb, cre );
}

// 振替名称を取得
BOOL CSelectData::GetNPfuriStr( int	frcode, CString& str )
{
	return m_pSub->get_npfuristr( frcode, str );
}

// 振替区分コードが有効か？
BOOL CSelectData::IsNPfuricode( int frcode )
{
	return m_pSub->is_npfuricode( frcode );
}

// ウィンドウと選択プロシージャのセット
void CSelectData::SetWindow( CWnd* pwnd )
{
	m_pSub->SetWindow( pwnd );
}

// 振替科目の範囲
void CSelectData::NPfuriHani( DWORD& scod, DWORD& ecod )
{
	m_pSub->NPfuriHani( scod, ecod );
}

// 振替科目の個数
int	 CSelectData::GetNPfuriCount()
{
	return m_pSub->GetNPfuriCount();
}

// 振替科目データを取得
const NP_FURIDTA*	CSelectData::GetNPfuriData(int index)
{
	return m_pSub->GetNPfuriData( index );
}

// 科目の表示制限
void CSelectData::KamokuRestrict( DWORD dwFlag )
{
	return m_pSub->_KamokuRestrict( dwFlag );
}

// 収支用の科目チェック
BOOL CSelectData::NP_InputItemCheck(struct _KamokuData& ans, struct _KamokuData& check, int _dcsw, BOOL bSelClick/*= FALSE*/ )
{
	return m_pSub->_NP_InputItemCheck( ans, check, _dcsw, bSelClick );
}

// 科目の表示制限[公益] ('08.12.01)
void CSelectData::NpKamokuRestrict( DWORD Jgykcd, DWORD Knrkcd )
{
	return m_pSub->_NpKamokuRestrict( Jgykcd, Knrkcd );
}

// 工事未入力チェック用
BOOL CSelectData::Kojicode_NoInp( CString kno, CString kmkcode )
{
	return m_pSub->_Kojicode_NoInp( kno, kmkcode );
}

// 完成工事 チェック用
BOOL CSelectData::Kojicode_KanChk( CString kno, CString kmkcode )
{
	return m_pSub->_Kojicode_KanChk( kno, kmkcode );
}

// 枝番名称クリア
int CSelectData::BrnTekClear()
{
	return m_pSub->_BrnTekClear();
}

// 内部取引の文字列取得
BOOL CSelectData::GetNaibuString( int type, int nbcd, CString& str )
{
	return m_pSub->_GetNaibuString( type, nbcd, str );
}

// 内部取引の仕訳か？
BOOL CSelectData::IsNaibuData( LPCTSTR deb, LPCTSTR cre )
{
	return m_pSub->_IsNaibuData( deb, cre );
}

#ifdef _SLIP_ACCOUNTS_
//-----------------------------------------------------------------------------
// イメージ管理クラスをセット
//-----------------------------------------------------------------------------
// 引数	pImgMng	：	セットするイメージ管理クラス
//-----------------------------------------------------------------------------
void CSelectData::SetImgMng( CImgMng *pImgMng )
{
	m_pSub->SetImgMng( pImgMng );
}
#endif

// 選択表示のオプションセット
void  CSelectData::SetSelOptMode( DWORD opt )
{
	m_pSub->SetSelOptMode( opt );
}

// 選択表示のオプションゲット
DWORD CSelectData::GetSelOptMode()
{
	return 	m_pSub->GetSelOptMode();
}


void CSelectData::SetBrnSelDispMode( int mode )
{
	return 	m_pSub->SetBrnSelDispMode(mode);
}

///////////////////////////////////////////////////////////////////////////////////

//091228
// 科目データ読み込み(科目順)
//	科目範囲チェック用に引数追加 [07.12 /12]
//
void CSelectSub::_init_kamoku_oder( int update/*= 0*/, DWORD dwStart/*= 0*/, DWORD dwEnd/*= 0*/)
{
	char	_cnt[PG_MAX];
	int		n, ofs;
	BOOL	bKengyo = FALSE;

	::ZeroMemory( FIXTBL, sizeof FIXTBL );
	::ZeroMemory( KDPAG, sizeof KDPAG );
	::ZeroMemory( KCD50C, sizeof KCD50C );

	::ZeroMemory( _cnt, sizeof( _cnt ) );

	int type =  IsKojinGyosyuMaster( m_pZm );
	if( type ) {
		if( IsKojinKengyoMaster( type ) ) {
			bKengyo = TRUE;
		}
	}

	if( ! update ) {
		int type =  IsKojinGyosyuMaster( m_pZm );
		if( type ) {
			if( IsKojinKengyoMaster( type ) ) {
				m_DRsgn = SL_IND_ALL;
			}
			else {
				// 単一の業種
				m_DRsgn = (type&0x0f);
			}
		}
		m_DJgykcd = m_DKnrkcd = 0xffffffff;
	}

	struct _DBKNREC	*pKn;
	int Fix_cnt = 0, Kdpag_cnt = 0; 
	ofs = 0; n = 1;

	DWORD dwCode;

	for( int i = 0; i < m_pZm->knm_info->reccount; i++ ) {

		pKn = m_pZm->pKnrec + i;

		if( (pKn->knvoid & 0x01) )
			continue;

		if( ! check_kamokurestrict( pKn ) )
			continue;

		dwCode = pKn->kncod;
		hl_rev( &dwCode, 4 );

		if( dwStart != 0 ) {
			if( dwCode < dwStart ) continue;
		}
		if( dwEnd != 0 ) {
			if( dwCode > dwEnd ) continue;
		}

		if(Fix_cnt < 8){
			if( bKengyo ) {
///				if( FIXTBL[ pKn->knfkn-1 ] == 0 )
///					FIXTBL[ pKn->knfkn-1 ] = i+1;
				if( FIXTBL[ Fix_cnt ] == 0 )
					FIXTBL[ Fix_cnt ] = i+1;
			}
			else {
				//兼業でないときは、最後の設定科目優先
				FIXTBL[ Fix_cnt ] = i+1;
			}
			Fix_cnt++;
		}

		if(Kdpag_cnt >= 40) {
			n++;
			ofs = 0;
			Kdpag_cnt = 0;
		}

		if( bKengyo ) {
			if( KDPAG[ n-1 ][ ofs ] == 0 ) {
				KDPAG[ n-1 ][ ofs ] = i+1;
				_cnt[ n-1 ] += 1;
			}
		}
		else {
			//兼業でないときは、最後の設定科目優先
			KDPAG[ n-1 ][ ofs ] = i+1;
			if( KDPAG[ n-1 ][ ofs ] == 0 )
				_cnt[ n-1 ] += 1;
		}

		ofs++;
		Kdpag_cnt++;

	}
}


// 科目の表示制限[公益] ('08.12.01)
void CSelectData::OrderKamokuRestrict( int order  )
{
	return m_pSub->_OrderKamokuRestrict( order );
}

// 科目の表示制限[公益] ('08.12.01)
void CSelectData::SetOrder( int order , int old_order )
{
	return m_pSub->_SetOrder( order, old_order );
}


// 摘要全選択 ページリセット
void CSelectData::ResetTkallPage()
{
	return m_pSub->ResetTkallPage();
}


///////////////////////////////////////////////////////////////////////////////////////////////

// 科目の表示制限[公益] ('08.12.01)
void CSelectSub::_SetOrder( int order, int old_order  )
{
	if(old_order == -1)	Order_flg2 = order;
	else				Order_flg2 = old_order;

	Order_flg = order;
}
//  科目の表示制限 _SetOrder実装によりorder引数を使用しなくなった
void CSelectSub::_OrderKamokuRestrict( int order )
{
///	m_DRsgn	=	dwFlag;
	int i,j, no_data;
	

	no_data = 0;
	m_CDPG = 1;
	// 現在表示中の科目を更新
	switch( m_NowPar.sel_pn ) {
	case SL_KAMOKU_PN:
	case SL_KMKANA_PN:
	case SL_KMINPCD_PN:
	case SL_KMKODER_PN: //-ver 1.0.3.14  //091228
	case SL_KMHANI_PN:  // 科目範囲 [07.12 /12]
	///	_init_kamoku(1);	// 固定科目等を再セット
	
		if(Order_flg) {//-ver 1.0.3.14  //091228
			DWORD start, end;
			start = end = 0;
			if( m_NowPar.sel_pn == SL_KMHANI_PN ) {
				start	= m_dwStart;
				end		= m_dwEnd;
			}
			_init_kamoku_oder(1, start, end);	// 固定科目等を再セット
		}
		else {
			_init_kamoku(1);	// 固定科目等を再セット
		}
		if( m_ACsel )
		{
			j = m_ACsel - 1;

#ifdef OLD_CLOSE // 08.25 /11
			if( kmk_50search( m_NowPar.kmk_kana ) == FALSE ) no_data = m_ACsel;
			///100106 この行のコメントアウト	m_ACsel = 0;    追加→no_data = 1
			///個人標準の場合は科目順の場合絞り込みで0になる場合が多々ある（設定順は貸借共通科目は常に出している、科目順で出すのはおかしい気がする）
			///
	///		else {
	///			fixcreat( (int *)&FIXTBL[0] );
	///			m_fixproc( SelFix8, &m_spp, 8, m_pWnd );
	///		}
#endif

//MyTrace( "-->@_OrderKamokuRestrict kmk_kana %s, kmk_inpcd = %d\n", m_NowPar.kmk_kana, m_NowParEx.kmk_inpcd );

			if( m_NowPar.kmk_kana[0] ) {
				if( kmk_50search( m_NowPar.kmk_kana ) == FALSE ) no_data = m_ACsel;
			}
			else {
				int st;
				if( (st=kmk_inpcodesearch(m_NowParEx.kmk_inpcd)) < 0 ){
					m_ACsel = 0;
				}
				m_ACsel = 0;
			//	else {
			//		no_data = m_ACsel;
			//	}
			}

			if(Order_flg == 0){
				fixcreat( (int *)&FIXTBL[0] );
				m_fixproc( SelFix8, &m_spp, 8, m_pWnd );
			}

			for( i = 1 ; i < j ; ++i )	// 元表次ページまで送る
				_KamokuSelect( VK_NEXT );
		}

		if( !m_ACsel || no_data)
		{
		//	j = m_CDPG;
		//	m_CDPG = 1;
			restrict_resetpage();

			// データがない場合はここを通るようにしているが内部でm_ACsel = 0とされているため戻す処理がいる
			if(no_data) m_ACsel = no_data;
		}

		m_fixproc( SelFix8, &m_spp, 8, m_pWnd );
		m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );

		break;
	default:
		_init_kamoku(1);
		break;
	}
	
}

// 固定科目作成(科目順)
void CSelectSub::fixcreat_order(int *fix, int *k_fix)
{
	int		seq, j, k, l; 
	BOOL	bKcod;
	bKcod = (m_KmkMode == SL_KMK_CODE);

#define FIX_l	2

///KDPAG[ n-1 ][ ofs ] 
	for( k = seq = 0; k < 4; k++ ) {
		for( j = FIX_l; j < FIX_l+2; j++ ) {
			if( (l = *(k_fix + seq++)-1) < 0 ) {
				SelFix8[seq-1].number = bKcod ? -1 : seq;
				SelFix8[seq-1].Data = "";
			//	SelFix8[seq-1].Data.Format( "%2d", seq );
				continue;

			}
			else if( ! check_kamokurestrict( l ) ) {
				SelFix8[seq-1].number = bKcod ? -1 : seq;
				SelFix8[seq-1].Data = "";
				continue;

			}

#ifndef KMCOD_OLD_KEEP
			SelFix8[seq-1].number = seq;
#else
			SelFix8[seq-1].number = bKcod ? get_hyojikncod( l ) : seq+9;
#endif
			SelFix8[seq-1].Data = get_hyojiknam( l );
		}
	}
}




typedef struct _KMKPG_SEL
{
	int	knkpg,
		knseq,
		knofst;
} KMKPG_SEL;


// 標準科目体系で、兼業のとき、全科目を指定されたらすべてを表示する
static
int __cdecl kmkpgcompare(/*void *context,*/ const void *ele1, const void *ele2 )
{
	int ret = 0;

	KMKPG_SEL* dw1, *dw2;

	dw1 = (KMKPG_SEL*)ele1;
	dw2 = (KMKPG_SEL*)ele2;

	if( dw1->knkpg > dw2->knkpg )
		ret = 1;
	else if( dw1->knkpg < dw2->knkpg )
		ret = -1;
	else {
		//同じ場合は、科目順
		if( dw1->knseq > dw2->knseq )
			ret = 1;
		else if( dw1->knseq < dw2->knseq )
			ret = -1;
	}

	return ret;
}


//
// 兼業時で、全業種指定でタブって登録されている分
//
void CSelectSub::_init_kengyokmk_kaitest()
{
struct _DBKNREC	*pKn;
int i, cnt = 0;
int n, ofs;

	for( int i = 0; i < m_pZm->knm_info->reccount; i++ ) {
		pKn = m_pZm->pKnrec + i;

		if( (pKn->knvoid & 0x01) )
			continue;

		if( pKn->knfkn >= 1 && pKn->knfkn <= 8 ) {
		//	if( bKengyo ) {
				if( FIXTBL[ pKn->knfkn-1 ] == 0 )
					FIXTBL[ pKn->knfkn-1 ] = i+1;
		//	}
		//	else {
		//		//兼業でないときは、最後の設定科目優先
		//		FIXTBL[ pKn->knfkn-1 ] = i+1;
		//	}
		}

		if( pKn->knkpg >= 101 && pKn->knkpg <= KPG_MAX ) {
			//<--- mmm add No.16-0221
			n	= ( pKn->knkpg / 100 );
			ofs = ( pKn->knkpg % 100 );
			ofs--;

			if( pKn->ind_ctg == 0 ){
				if( int st = ( m_pZm->zvol->ind_sgn2 & 0x07 ) ){
					if( st == 0x02 )		n += ( IMM_OFSET );
					else if( st == 0x04 )	n += ( AGR_OFSET );
				}
			}
			else if( pKn->ind_ctg == 0x02 )	n += ( IMM_OFSET );
			else if( pKn->ind_ctg == 0x04 )	n += ( AGR_OFSET );

			if( KDPAG[ n-1 ][ ofs ] == 0 ){
				KDPAG[ n-1 ][ ofs ] = i+1;
			}			
			//---> mmm add

		}
	}
}
//
// 兼業時で、全業種指定でタブって登録されている分
//
void CSelectSub::_init_kengyokmk()
{
CArray<KMKPG_SEL,KMKPG_SEL&>	dbKn;
struct _DBKNREC	*pKn;
struct _KMKPG_SEL	kpgs;
int i, cnt = 0;

	for( int i = 0; i < m_pZm->knm_info->reccount; i++ ) {
		pKn = m_pZm->pKnrec + i;

		if( (pKn->knvoid & 0x01) )
			continue;

		if( pKn->knfkn >= 1 && pKn->knfkn <= 8 ) {
		//	if( bKengyo ) {
				if( FIXTBL[ pKn->knfkn-1 ] == 0 )
					FIXTBL[ pKn->knfkn-1 ] = i+1;
		//	}
		//	else {
		//		//兼業でないときは、最後の設定科目優先
		//		FIXTBL[ pKn->knfkn-1 ] = i+1;
		//	}
		}

		if( pKn->knkpg >= 101 && pKn->knkpg <= KPG_MAX ) {

			kpgs.knkpg = pKn->knkpg;
			kpgs.knseq = pKn->knseq;
			kpgs.knofst = i+1;

			dbKn.Add( kpgs );
			cnt++;
		}
	}

	if( cnt > 0 ) {
		qsort( &dbKn[0], cnt, sizeof dbKn[0], kmkpgcompare );

		int nPage, ofs;
		int kpg, oldkpg;

		oldkpg = kpg = (dbKn[0].knkpg / 100);

		nPage = ofs = 0;
		for( i = 0; i < cnt; i++ ) {

			kpg = (dbKn[i].knkpg / 100);
			if( kpg != oldkpg ) {
				nPage++;
				ofs = 0;
			}

			KDPAG[ nPage ][ ofs ] = dbKn[i].knofst;
			ofs++;

			if( ofs >= 32 ) {
				nPage++;
				ofs = 0;
			}

			oldkpg = (dbKn[i].knkpg / 100);
		}
	}
}


//----------------------------------------------
//	枝番摘要名称 情報クリア
//	  枝番名称を
//
//----------------------------------------------
int CSelectSub::_BrnTekClear()
{
	// 枝番名称・諸口枝番エリアをクリア
	int ret = m_BrnTkyo.brtek_free();
	init_brntkypar( &m_BRNTKY );

	return ret;
}


//----------------------------------------------------------
//	社会福祉改正対応　内部取引消去
//	int nChar ....... キーコード(-1で初期表示)
//	BOOL bNowPage ... TRUE で現在ページを表示する
//
//	返送値	0
//----------------------------------------------------------
int	CSelectSub::_SOWNaibuSelect( int nChar, BOOL bNowPage/*=FALSE*/ )
{
	int pg, pgcnt, max, i, cnt;
	char tmp[128], fmt[36];
	BOOL bDisp = FALSE;

	int aBmnMax = m_PageMax-8;

	if( nChar == -1 ) {
		if( ! bNowPage )	m_NaibuPage = 1;

		_NaibuSelectSub(m_NaibuPage);
	}
#ifdef NAIBU_VER2
	else if( nChar == VK_NEXT ) {
		i = m_NaibuPage;
		i++;

		// 仕訳で増えている恐れがあるので 数えなおす
		if( (m_pZm->zvol->sub_sw&0x02) ) {
			m_BmnMax = 0;
			m_pZm->bmname->MoveFirst();
			do {
				if( ! IsSyafuKaisei( m_pZm->zvol ) ) { 
					// 合計部門は除く
					if( (m_pZm->bmname->bnhsg[0] & 0x02) )
						continue;
				}
				else {
					//総合計 除く
					if( (m_pZm->bmname->bnhsg[0] & 0x04) )
						continue;
				}

				m_BmnMax++;
			} while( m_pZm->bmname->MoveNext() == 0 );
		}

		pgcnt = (m_BmnMax / aBmnMax);
		if( (m_BmnMax % aBmnMax) )	pgcnt++;

		if( pgcnt >= i ) { 
			m_NaibuPage = i;
			_NaibuSelectSub(m_NaibuPage);
		}
	}
	else if( nChar == VK_PRIOR ) {
		if( m_NaibuPage > 0 ) {
			m_NaibuPage--;
			_NaibuSelectSub(m_NaibuPage);
		}
	}
#endif

	return 0;
}


//----------------------------------------------------------
//	内部取引消去 読み込み部分
//	int page ....... 表示ページ
//----------------------------------------------------------
void CSelectSub::_NaibuSelectSub( int page )
{
	CBMNAME* bn;
	int i, cnt, max, LVmax;
	char bf[36], tmp[128], fmt[36];

	strcpy_s( m_spp.seljob, sizeof m_spp.seljob, "《内部取引消去》" );

//	max = sizeof NLdata / sizeof NLdata[0];
	max = m_NBdta.GetCount();
	char* p;
	CString	descript;
	LVmax = 8;

	for( i = cnt = 0; i < max; i++ ) {
		p = m_NBdta[i].nbt_name;
		sprintf_s( tmp, sizeof tmp, "%s", p );
		sprintf_s( fmt, sizeof fmt, "%%.%ds", get_strcnt( tmp, SEL_LEN ) );

		if( ++cnt > max )	break;

		SelVal32[cnt-1].number = i+1;
		SelVal32[cnt-1].Data.Format( fmt, tmp );
	}
	if( cnt < LVmax ) {
		for( i = cnt; i < LVmax; i++ ) { 
			SelVal32[i].number = i+1;
			SelVal32[i].Data = "";
		}
	}

	// 相手部門
	int aBmnMax = (m_PageMax-8);
	i = cnt = 0;

#ifdef NAIBU_VER2
	if( ! (m_pZm->zvol->sub_sw&0x02) || m_pZm->bmname == NULL ) {
		goto END_TAG;
	}

	bn = m_pZm->bmname;
	bn->MoveFirst();
	if( bn->st == -1 )	goto END_TAG;

	do {
		if( ! IsSyafuKaisei( m_pZm->zvol ) ) { 
			if( (bn->bnhsg[0] & 0x02) )
				continue;
		}
		else {
			//総合計 除く
			if( (bn->bnhsg[0] & 0x04) )
				continue;
		}
		
		i++;

		if( i > ((page-1) * aBmnMax) ) {
			cnt++;
			if( cnt > aBmnMax )
				break;
		
			set_codestr( bf, sizeof bf, bn->bnbcd, m_pZm->zvol->bmcol );
			sprintf_s( tmp, sizeof tmp, "%s%s", bf, bn->bnbnm );
			sprintf_s( fmt, sizeof fmt, "%%.%ds", get_strcnt( tmp, SEL_LEN ) );

			SelVal32[cnt+8-1].number = cnt+8;
			SelVal32[cnt+8-1].Data.Format( fmt, tmp );

		}

	} while( bn->MoveNext() == 0 );
#endif

END_TAG:

	if( cnt < aBmnMax ) {
		for( i = cnt; i < aBmnMax; i++ ) { 
			SelVal32[i+8].number = i+8+1;
			SelVal32[i+8].Data = "";
		}
	}

	m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
}


//----------------------------------------------------------------
//		内部取引消去 選択データを取得する。
//
//----------------------------------------------------------------
BOOL CSelectSub::_GetNaibuSelect( SelResult* result, int page, int sel_no )
{
	CBMNAME* bn;

	int aBmnMax = (m_PageMax-8);
	int idx, max;
	BOOL ret = FALSE;

#ifndef NAIBU_VER2
	// 階層選択
//	max = sizeof NLdata / sizeof NLdata[0];
	max = m_NBdta.GetCount();
	idx = sel_no - 1;

	if( sel_no <= max ) {
		result->code	= m_NBdta[idx].nbt_code;
		result->bmn		= -1;
		result->selname = m_NBdta[idx].nbt_name;
		if( result->code == 0 )	result->selname.Empty();
		ret = TRUE;
	}
#else
	if( sel_no <= 8 ) {
		// 階層選択
	//	max = sizeof NLdata / sizeof NLdata[0];
		max = m_NBdta.GetCount();
		idx = sel_no - 1;

		if( sel_no <= max ) {
			result->code	= 1;
			result->bmn		= m_NBdta[idx].nbt_code;
			result->selname = m_NBdta[idx].nbt_name;

		//	if( result->bmn == 0 ) result->code = 0; // なし

			ret = TRUE;
		}
	}
	else {
		// 相手部門
		bn = m_pZm->bmname;
		bn->MoveFirst();

		int i, cnt;
		i = cnt = ret = 0;

		if( sel_no > m_PageMax )
			return FALSE;

		if( bn->st == -1 )
			return FALSE;

		do {
			if( ! IsSyafuKaisei( m_pZm->zvol ) ) { 
				if( (bn->bnhsg[0] & 0x02) )
					continue;
			}
			else {
				//総合計 除く
				if( (bn->bnhsg[0] & 0x04) )
					continue;
			}

			i++;

			if( i > ((page-1) * aBmnMax) ) {
				cnt++;
				if( cnt > aBmnMax )
					break;

				if( sel_no == (cnt+8) ) {
					ret = TRUE;
					result->code	= 2;
					result->bmn		= bn->bnbcd;
					result->selname = bn->bnbnm;
					break;
				}
			}
		} while( bn->MoveNext() == 0 );
	}
#endif

	return ret;
}


// 振替区分コードより 名称を取得
BOOL CSelectSub::_GetNaibuString( int type, int nbcd, CString& str )
{
	BOOL ret = FALSE;
	int i, max;

#ifndef NAIBU_VER2
	max = m_NBdta.GetCount();

	for( i = 0; i < max; i++ ) {
		if( type == m_NBdta[i].nbt_code ) {
			str =  m_NBdta[i].nbt_name;
			if( type == 0 )	str.Empty();

			ret = TRUE;
			break;
		}
	}
#else
	if( type == 1 ) {
	//	max = sizeof NLdata / sizeof NLdata[0];
		max = m_NBdta.GetCount();

		for( i = 0; i < max; i++ ) {
			if( nbcd == m_NBdta[i].nbt_code ) {
				str =  m_NBdta[i].nbt_name;
				ret = TRUE;
				break;
			}
		}
	}
	else if( type == 2 ) {
		CBMNAME* bn;
		bn = m_pZm->bmname;
		bn->MoveFirst();

		if( bn->st == -1 )
			return FALSE;

		do {
			if( ! IsSyafuKaisei( m_pZm->zvol ) ) { 
				if( (bn->bnhsg[0] & 0x02) )
					continue;
			}
			else {
				//総合計 除く
				if( (bn->bnhsg[0] & 0x04) )
					continue;
			}

			if( nbcd == bn->bnbcd ) {
				str = bn->bnbnm;
				ret = TRUE;
				break;
			}
		} while( bn->MoveNext() == 0 );
	}
#endif

	return ret;
}




BOOL CSelectSub::_IsNaibuData( LPCTSTR deb, LPCTSTR cre )
{
	BOOL	ret = FALSE;
	DWORD	dwCode, dwGkcd;

	dwCode = _atoh( (LPSTR)deb );
	dwGkcd = (dwCode&0xffffff00);
	hl_rev( &dwGkcd, 4 );

//	DBGKREC* pGk = m_pZm->get_gksrc( dwGkcd );
	DBGKREC* pGk = m_pZm->PjisToGkrec( dwGkcd );

	if( pGk != NULL ) {
		if( (pGk->gksgn& 0x80) ) {
			ret = TRUE;
		}
	}

	if( ! ret) {
		dwCode = _atoh( (LPSTR)cre );
		dwGkcd = (dwCode&0xffffff00);
		hl_rev( &dwGkcd, 4 );
		
//		pGk = m_pZm->get_gksrc( dwGkcd );
		pGk = m_pZm->PjisToGkrec( dwGkcd );

		if( pGk != NULL ) {
			if( (pGk->gksgn& 0x80) ) {
				ret = TRUE;
			}
		}
	}
	return ret;
}

#ifdef _SLIP_ACCOUNTS_

//-----------------------------------------------------------------------------
// 原票イメージによる摘要表示
//-----------------------------------------------------------------------------
// 引数	nChar		：	キーコード(-1で初期表示)
//		bNowPage	：	TRUE で現在ページを表示する
//-----------------------------------------------------------------------------
//	返送値		0
//----------------------------------------------------------
int	CSelectSub::_SlipTkySelect( int nChar, BOOL bNowPage/*=FALSE*/ )
{
/*	int pg, pgcnt, max, i, cnt;
	char tmp[128], fmt[36];
	BOOL bDisp = FALSE;

	int aBmnMax = m_PageMax-8;
*/
	if( nChar == -1 ) {
		if( !bNowPage ){
			m_SlipTkyPage = 1;
		}

		slip_tky_dsp( m_SlipTkyPage );
	}
	else if( nChar == VK_NEXT ){

		if( m_SlipTkyCnt > (m_SlipTkyPage*AC_MAX) ){
			m_SlipTkyPage++;
			slip_tky_dsp( m_SlipTkyPage, TRUE );
		}
	}
	else if( nChar == VK_PRIOR ) {
		if( m_SlipTkyPage > 1 ) {
			m_SlipTkyPage--;
			slip_tky_dsp( m_SlipTkyPage, TRUE );
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 原票イメージによる摘要表示
//-----------------------------------------------------------------------------
// 引数	page		：	表示ページ
//		bSameImg	：	同一イメージ？
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
int CSelectSub::slip_tky_dsp( int page, BOOL bSameImg/*=FALSE*/ )
{
	// 摘要文字サイズ
	const	int	TKY_INPMOJI_MAX	=	56;	// 摘要入力可能文字 最大数

	// CImgMng から表示中の文字列を取得
	ASSERT( m_pImgMng );
	if( m_pImgMng == NULL ){
		return 0;
	}
	
	// 初期化
	if( bSameImg == FALSE ){
		m_MatchStrAry.RemoveAll();
		m_pImgMng->GetMatchedStrings( ID_ALL_SEARCH, m_MatchStrAry );
		m_SlipTkyCnt = (int)m_MatchStrAry.GetCount();
	}

	int i, cnt;
	i = cnt = 0;
	char tmp[128], fmt[36];
	int selpnt;
	selpnt = 0;
	int	tmplen = 0;
	int	toBufSize = TKY_INPMOJI_MAX;

	strcpy_s( m_spp.seljob, sizeof m_spp.seljob, "《イメージ文字列》" );
	for( i = 0; i < m_SlipTkyCnt; i++ ) {
		if( (i+1) > ((page-1) * m_PageMax) ) {
			cnt++;
			if( cnt > m_PageMax )
				break;

			// 文字列カット
			tmplen = m_MatchStrAry[i].GetLength();
			if( tmplen >= toBufSize ){
				int kst = IsKindOfLetter( m_MatchStrAry[i].GetBuffer(), (toBufSize-1-1) );
				if( kst == 2 ){
					tmplen = toBufSize-1-1;
				}
				else{
					tmplen = toBufSize-1;
				}
				CString	tmpStr;
				tmpStr = m_MatchStrAry[i].Left( tmplen );
				m_MatchStrAry[i] = tmpStr;
			}
		
			sprintf_s( tmp, sizeof tmp, "%s", m_MatchStrAry[i] );
			sprintf_s( fmt, sizeof fmt, "%%.%ds", get_strcnt( tmp, SEL_LEN ) );

			SelVal32[cnt-1].number = cnt;
			SelVal32[cnt-1].Data.Format( fmt, m_MatchStrAry[i] );
			selpnt++;
		}
	}

	if( selpnt < AC_MAX ){
		for( i = selpnt; i < AC_MAX; i++ ) {
			SelVal32[i].number = i+/*(isEntrySys() ? 9 : 1) */ 1;
			SelVal32[i].Data = "";
		}
		m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
	}
	else{
		m_sel32proc( SelVal32, &m_spp, AC_MAX, m_pWnd );
	}

	return 0;
}

//-----------------------------------------------------------------------------
// イメージ管理クラスをセット
//-----------------------------------------------------------------------------
// 引数	pImgMng	：	セット用のイメージ管理クラス
//-----------------------------------------------------------------------------
void CSelectSub::SetImgMng( CImgMng *pImgMng )
{
	m_pImgMng = pImgMng;
}

//-----------------------------------------------------
//	選択結果を取得
//	SelResult* result ...	選択結果格納
//	int page　...........	選択ページ
//	int sel_idx .........	選択番号(1～)
//-----------------------------------------------------
BOOL CSelectSub::_GetImgStrSelect( SelResult* result, int page, int sel_no )
{
	BOOL ret = FALSE;
	int i, cnt, max;
	i = cnt = 0;

	if( sel_no > m_PageMax )
		return FALSE;

	for( i = 0; i < m_SlipTkyCnt; i++ ) {

		if( (i+1) > ((page-1) * m_PageMax) ) {
			cnt++;
			if( cnt > m_PageMax )
				break;

			if( sel_no == cnt ) {
				ret = TRUE;
				result->tky_code	= sel_no;
/*- '12.10.06 -*/
//				result->tky			= m_MatchStrAry[cnt-1];
/*-------------*/
				result->tky			= m_MatchStrAry[i];
/*-------------*/
				break;
			}
		}
	}

	return ret;
}
#endif



// 選択表示のオプションセット
void  CSelectSub::SetSelOptMode( DWORD opt )
{
	m_dwOPT = opt;
}

// 選択表示のオプションゲット
DWORD CSelectSub::GetSelOptMode()
{
	return 	m_dwOPT;
}


//枝番カナ順
void CSelectSub::SetBrnSelDispMode( int mode )
{
	m_BrnSelMode = mode;
	m_BrnTkyo.set_brnKanaJun( m_BrnSelMode == SL_KJUN_DSP ? TRUE : FALSE );
}

//全摘要の開始ページを 1 にする
void CSelectSub::ResetTkallPage()
{
	m_tallPG = 1;
}

