#ifndef INCLUDE_ZINPSUB_H
#define INCLUDE_ZINPSUB_H


//--追加名称取得用構造体****************
//	科目＋枝番時 の枝番摘要名称
//	部門 の部門名称
//	工事 の工事名称
//**************************************
struct _SET_KMKETC
{
	int			bmn;		//部門
	CString		kno;		//工事番号
	CString		code;		//科目コード
	int			brn;		//枝番コード
	CString		etc;		//追加名称格納
	COLORREF	etc_col;	//追加名称表示文字色
};


int ispread( int locate, CDBINPDataRec * );
int cdata_read( int locate, CDBINPDataRec *buff );
void nline_del( struct _DATA_LINE *, int );

#ifdef CUT_CLOSE
void kn50dsp( short * );
void fixclr( void );
void fixcrt( int * );
void _fixcreat(int *fix);	// 02.20 /02
#endif

char *get_hyojiknam( int n );
// 科目コードをint で返送 ... 02.20 /02
int get_hyojikncod( int n );

int pjsksrc( LPCTSTR );
struct _DBKNREC* pjs_knrec( LPCTSTR pjis_code );

//int rentksr( short , CDBipTKREC * );
int rentksr( short arg2 );

#ifdef CUT_CLOSE
void renttl(void);
void rentdsp( CDBipTKREC *, int, int, int, int);
void rentdsp( int pg, int dc_sw );
#endif

void recdsp( CDBINPDataRec *, int, struct _DATA_LINE *, int );
void _recdsp(CDBINPDataRec *data, struct _DATA_LINE *ddata, BOOL bEdaDisp = TRUE, BOOL invDisp = FALSE );

#ifdef REGDATA	// 08.07 /00
int _rd_recdsp( CDBINPDataRec *data, struct _DATA_LINE *ddata, BOOL* cnd );
#endif

void set_kamoku( CDBINPDataRec *data, struct _DATA_LINE *ddata, int pn, BOOL bEdaDisp = TRUE );
BOOL _set_kmketc( struct _SET_KMKETC* psk, BOOL full = TRUE );
void _set_etcdsp( struct _DATA_LINE* dtline, int pn, struct _LINE_CND* lcnd, BOOL full = TRUE );
BOOL _dsp_etc( CWnd *pwnd, struct _ItemData *data, struct _LINE_CND *cnd, struct _InpCtl *ctl, double x_rate, BOOL dspsw = TRUE, BOOL focusoff_disp = TRUE );

void _dsp_etcwnd( struct _SET_KMKETC* sk, CWnd *pwnd, ICSStatic* pst, UINT nID, BOOL focusoff_disp );

BOOL _set_bmnetc( struct _SET_KMKETC *sk );
BOOL _set_kojietc( struct _SET_KMKETC *sk );
BOOL _get_hjiyustr( int hjcd, int nbcd, CString& str );
void NPfuri_Query( int npcd, CString& qstr );

int tkrsrch( int, char *);
int tkrsrch( unsigned short *, int, char *);
void tkrdsp( int pg, int cnt );
void tkrdsp( unsigned short *, int );

#ifdef _SPEED_FIX_1207_07_
int calq_gstotal( int zanread );
#endif
int g_stotal( LPCTSTR, char *, char *, char *);
void syogdsp( char * );
void genkdsp( char * );

void TkNamTrimRight( CDBipTKREC *tek );
int get_inpl( unsigned char *, int );
int get_tklen( unsigned char * );

char *val_txt( char *txt, int txtsiz, char *bin, int t_l=0, char *fronttxt=0, char *fmt=0, char *endtxt=0 );
void val_disp( CWnd *pwnd, char *bin, OLE_COLOR def_fore, int t_l=0, char *fronttxt=0, char *fmt=0, char *endtxt=0 );
void kakudai(CWnd* wnd);

//#* 02.09 /02
int get_strcnt(char* str, int byte);
//#* 08.19 /02
void _set_bmndsp( struct _DATA_LINE *line, int pn, BOOL full = TRUE );

COLORREF PixelAlpha(COLORREF clrSrc, double src_darken, COLORREF clrDest, double dest_darken);

#ifdef OLD_CLOSE
//#* 12.09 /02
int tkat_srch( short flg, CString deb, CString cre, unsigned short *tkr );
void tkauto_dsp( int pg, int cnt );

//#* 自動分離修正仕訳をセット 06.20 /03
void update_zrec_line( CDBINPDataRec* wrtdata );
void set_bnwrtdata( CDBINPDataRec* wrtdata );
int write_bndata();
#endif

void set_brn( int brn );
int get_brn();

void KamokuString( DBKNREC* pKn, CString& string );
void KamokuString( DBKNREC* pKn, char* buf, int bufsize );

int csvstr_check( char* wk, int wksize, LPCTSTR bf, int valsg = 0 );
void _clr_kmketc( struct _SET_KMKETC* psk );

BOOL IsKobetsuKamoku( DBKNREC* pKn, BYTE dsgn2 );
BOOL IsSyafuKaisei( CZVOLUME* zv );
BYTE KaniDefaultKubun( CZVOLUME* zv );

int mkrd_data_requery(int lbnum);

#endif // INCLUDE_ZINPSUB_H