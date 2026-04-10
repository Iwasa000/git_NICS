#include "icssel.h"

void ermset( short, char *msg = 0  );
void ErrExit( short code, LPCTSTR msg = 0 );

int cocod_ok( CoSel* Cook, CWnd *pWin, BOOL co_chg = FALSE );
struct _DBKNREC* kmtbl_get( unsigned int rno );

int tbrasrch( CDBINPDataRec *data, int tkcd, struct _LINE_CND *cnd );
int tbrasrch( CDBINPDataRec *data, int tkcd, char *dbr_inptype, char *cbr_inptype );

int init_open( void );
int mst_open( void );
int mst_reload( void );

int ldata_dsp( void );
int int_seetno( int );

void jb_end( int );
void mst_close( int sgn = 0);
void vol_close( void );
int date_chk( int );

int DB_SyIniz( CDBINPDataRec* data );
char *sy_chk_txt( CDBINPDataRec *data, struct _SY_MSG_PACK2* smpk, int inp_mode );
char *sy_line_txt( CDBINPDataRec *data );
struct _AUTOSEL *get_inpmode();
void set_inpmode( struct _AUTOSEL *autosel );

//dinpsubs.cpp

//ビューサイズ比率ストラクチャ
typedef struct _VIEW_RATE {
	double w, h;
} VIEW_RATE;
void set_viewrate( double w, double h );
void get_viewrate( double &w, double &h );

//int resetpag( int );
//void intcrt( int );

int dataupd( CDBINPDataRec *, CDBINPDataRec *SCAN_rec, int, int, CWnd* pwnd );

void DBDinpAppendParamUpdate( CDBINPDataRec* prec, int bseq );
void DBDinpAppendParamReload( int olddacnt, int newdacnt );
BOOL sotozei_data( CDBINPDataRec *drec );
BOOL _sotozei_data( BYTE *dsign );
BOOL uchizei_data( CDBINPDataRec *drec );
BOOL _uchizei_data( BYTE *dsign );
BOOL _joto_data( BYTE *dsign );
BOOL isdiff_data( CDBINPDataRec *newdata, CDBINPDataRec *moto );
BOOL _isSyzdiff_data( CDBINPDataRec *newdata, CDBINPDataRec *moto );
BOOL isSyzdiff_data( CDBINPDataRec *newdata, CDBINPDataRec *moto );

void syog_genk( CDBINPDataRec *dta, int add_sub );
void datadsp( int zrec_ofs, int sgn, int insln );

int scheck( int seq, int sg );
int scheck( int );

int Confirm_Mode();
COLORREF GetConfirmBkColor();
BOOL CheckConfirmBkColor( COLORREF back );

BOOL IsModifyConfirmData();
BOOL IsEnkakuUser();

int tline_chk( CDBINPDataRec *);
int _tline_chk( CByteArray *dsign );
BOOL modefy_ok( int mode, CDBINPDataRec *drec );

int max_msg( int sg );

void inpmdisp( int );
void inpmode_set( int );
void reupdat( CDBINPDataRec * );
void repldata( void );

void intscan( void );
BOOL isSTRAGE();	// 10.16 /98	電子データ保存マスター?
BOOL isDataDelOK( int delsgn );	// 10.29 /98	取消チェック
char *get_TKS( char *dst, char *tks, int knjn );

void mastclq( CDBINPDataRec *, int );

//
// 枝番摘要選択関係
//
// 枝番摘要検索
int brntky_sch( struct _TKYBRN_SEL *tkbrn, struct _DataTable *sel32D, CICSSel *sel32C, int bmn, CString& code, int kana = 0, BOOL bDISP = FALSE );
//
// 枝番摘要ページアクセス
int brntky_page( struct _TKYBRN_SEL *tkbrn, struct _DataTable *sel32D, CICSSel *sel32C, int page );
//
// 枝番リード
int brntky_get( CString *code, int *brn, struct _TKYBRN_SEL *tkbrn, int no );

// 枝番摘要表示モードゲット
int GetBrTekMode();
int _GetBrTekMode(); //#* 03.18 /02
// 08.30 /04 -- 部門名称 表示モードゲット
BOOL GetBmnDspMode();

// 枝番登録
int brzan_torok( CWnd* pwnd, int bmn, CString code, int brn, BOOL dbsgn );

// 摘要ターミネーションモードを取得
int& TekiyoKeyMode();


// brzan.cpp
void brchk0( CDBINPDataRec *, int );
int brchk1( CDBINPDataRec *, int );
void del_dtaeda( CDBINPDataRec *dta, int dbcr_sg );
int zroeda_torok(CDBINPDataRec *data, int dbcr_sg);
int br_updat (int, struct _EZREC *, CDBINPDataRec *, int, int);

int ac_brchk1( CDBINPDataRec *, int );

int inp_brchk( CString& code, int brn, int mode, int *sg, int dbcr_sg );

// 入力日付をセット
void set_inpdate( CDBINPDataRec* data );

//#* 諸口ブザー ... 04.03 /03
void set_syobz_data( CDBINPDataRec* data );
int	chk_syobz( CDBINPDataRec* data, int pn );
BOOL is_syobuzzer_on( int pn );

// 09.02 /08
int CheckWidthSyohi();
//	金額 Enter 複写 フラグを取得 [09.08 /08]
int& ValEnterCopyMode();
// 10.01 /08
int& TkyRegMode();
// 06.30 /11
int& ZmoveMode();
// 05.15 /19
int& DateTabKeyMode();
// 11.01 /19
int& RDsiwakeZeirituMode();
// 06.02 /22
int& FromReadOCR();

// 部門残高数を返送
int bmnzan_count( CDatabase* database, CString& code, CString& errmsg );

// SREC 内容を Trace
void SREC_Trace( char* pstr );

// 摘要 １文字コピー
BOOL tekyo_tabcopy( CWnd* pwnd, int idc, char *tks );

#define SW_HOJIN		0x0001		//法人
#define SW_KOJIN		0x0002		//個人
#define SW_UNSO			0x0004		//運送
#define SW_KENST		0x0008		//建設
#define SW_BYOIN		0x0010		//病院
#define SW_PUB			0x0020		//公益法人
#define SW_PUB3			0x0040		//公益法人III
#define SW_SOCIAL		0x0080		//社会福祉
#define SW_SOCIAL2		0x0100		//社会福祉II
#define SW_SYUKYO		0x0200		//宗教
#define SW_SCHOOL		0x0400		//学校


BOOL IsOKGyosyu( int apno, DWORD check_bit );

void tkrec_queryAll();

int IsHjiyuItemGyosyu();

// 消費税文字列調整 (/ の手前文字列の空白をカット)
void systr_adj( CString& systr );

// 02.07 /13
int& TKtabMode();
int insseqpos_make( CWnd* pwnd );

// 06.11 /13
BOOL isInsertMaster();

// 09.04 /13
BOOL IsMasterType( DWORD typbit );

// 11.04 /13
int GetRDLabelMax();
int GetRDDataMax();

// 07.16 /14
void RegDataTorokMsg();

// 04.30 /15
int RDdnpDataRepair();
// 04.08 /15
bool IsBuyScannerSave();
// 11.14 /16
int& JZcloudEndKakutei();
BOOL IsCloudSetupEnv();
BOOL IsJozuEnvCloudMaster();

// 12.02 /16
int HaihunCheck( LPCTSTR string, CString& chgStr, int type = 0 );

// 03.10 /17
bool IsBuyOCRScan();

int& DenpTypeMode();
void DenpInputSyzMdfyVectorSet( DENP_INPUTPAR* para );
void DenpEnd_addmdfcheck( DENP_INPUTPAR* para );
BOOL IsDenpyoSiwake( CDBINPDataRec* prec );


// ソースまとめる。(DinpSub.cpp)
BOOL is_SHIFT();
BOOL is_CTRL();
void inpline_inz( struct _DATA_LINE * );
void set_SREC( CDBINPDataRec *srec, CDBINPDataRec *zrec, struct _DATA_LINE *data_line, int scroll_nl, struct  _DATA_LINE *inp_line, int inp_mode );
void inp_errmsg( int pn, int mode = 0 );
int is_symstyp();

BOOL fix8cnd();
void fix8cnd( BOOL sg );
BOOL fixvalcnd();
void fixvalcnd( BOOL sg );
int* fncINP_mode();
#define INP_mode *fncINP_mode()

int SCROLL_NL();

int brn_inptype( int nowtype, CString nowcode, int nowbrn, CString mdfycode, int mdfybrn );
int brn_dfltinptype(  int brn, int denp = 0 );
int is_mdfybrnitem( CString nowcode, int nowbrn, CString mdfycode, int mdfybrn );

BOOL is_syohizeisiwake( CDBDATA* dbdata );
BOOL is_syohizeisiwake( CDBINPDataRec *data );

BOOL isnot_defzeigaku(CDBDATA* dbdata, COLORREF& back);
BOOL isnot_defzeigaku(CDBINPDataRec* data, COLORREF& back);

BOOL is_yusyutsusiwake(CDBDATA* dbdata);
BOOL is_yusyutsusiwake(CDBINPDataRec *data);

// 10.22 /18
BOOL IsNotMultiWindow();
// 02.01 /21
BOOL IsJzSansyo();
void GetConfirmString(CString& str);
// 02.15 /21
BOOL IsMAsterKAKUTEI();

int& KanaSearchType();
int KanaSearchKeta();

int  SyzStopMode();
int& SyzStopModeValue();
BOOL IsMustStopSyz(CDBINPDataRec* data);
BOOL is_sotomensiwake(CDBINPDataRec* data);
BOOL is_sotomensiwake(CDBDATA* dbdata);

BOOL is_karibarai_menzei(BYTE* dsign);

// WIZ 関連の関数
bool IsBuyWizOption();

void ResetWizRenkei();
bool IsMasterWizRenkei(short apno, int code);
bool IsWizRenkeiExist(short apno, int code);
int  RenkeiWizMaster(short apno, int code);
int DBsessionCheck();
int InvalidWizOptionFlg();
int ResetWizOptionFlg();
int ResetMasterWizOptionFlg();
int OpenDocDBMaster(LPCTSTR sqlsvr, LPCTSTR apno, LPCTSTR cocode, LPCTSTR kikan);