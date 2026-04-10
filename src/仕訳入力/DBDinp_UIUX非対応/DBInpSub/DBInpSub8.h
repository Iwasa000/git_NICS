/*========================================================================
		入力で使用する 関数群 
		Lib:	DBInpSub.lib
		DLL:	DBInpSub.dll

  ========================================================================*/
#ifndef INCLUDE_DBINPSUB_H
#define INCLUDE_DBINPSUB_H


#ifdef _DBINPSUB
#define DBSUB_FUNC	__declspec( dllexport )
#else
#define	DBSUB_FUNC	__declspec( dllimport )
#endif

#include <vector>

// 変換 タイプコード
#define CONV1_2		0	// 第 1引数から 第 2引数
#define CONV2_1		1	// 第 2引数から 第 1引数

// 決修サインを BCD にセットする(月の部分)
// 
#define KESY_SG_GET( x )		( (x & 0xe0)>>5 )
#define KESY_SG_SET( x, sgn )	x |= (sgn << 5) 


// 枝番登録 structure
typedef struct _EDA_RECORD
{
	int		ed_bmn;			// -1 のときは科目枝番を登録
	char	ed_kcod[10];
	int		ed_ecod,
			ed_tkcod;
	char	ed_name[32],
			ed_kana[10];

	char	ed_zan[6];

} EDA_RECORD;


// 登録処理タイプ
#define TP_DIC		0
#define TP_KAMOK	1
#define TP_SIWAKE	2
#define TP_SELECT	3	// 選択ダイアログを表示


// 摘要登録 structure
typedef struct _TKY_REGPAR
{
	char	tr_debt[10],	// 借方コード
			tr_cred[10];	// 貸方コード
	short	tr_dcsw;		// 科目対応摘要 処理の 借方(0), 貸方(1) サイン

	char	tr_tky[82];		// 摘要

	int		tr_pos;			// 初期カーソル位置(バイト単位)
	int		tr_jobtyp;		// 登録 処理タイプ

	char	tr_invno[16];	// インボイス登録番号
	BYTE	tr_menchk;
	char	tr_rsv[3];

} TKY_REGPAR;


// 摘要登録 Version2 パラメータ
typedef struct _TKY_VER2
{
	char	t2_debt[10],	// 借方コード
			t2_cred[10];	// 貸方コード

	char	t2_tky[82];		// 摘要
	DWORD	t2_sel;			// 摘要選択
	int		t2_pos;			// 初期カーソル位置(バイト単位)

	WORD	t2_brnsgn;		// D0:借方枝番セット, D1:貸方枝番セット
	int		t2_brndeb,		// セットする 借方枝番コード
			t2_brncre;		// セットする 貸方枝番コード

	char	t2_invno[16];	// インボイス登録番号
	BYTE	t2_menchk;		// 免税
	char	t2_dmy[126-10-16-1];

} TKY_VER2;

// 摘要登録切り替え関数用 
typedef struct _TKY_REGPAREX
{
	char	tex_debt[10],	// 借方コード
			tex_cred[10];	// 貸方コード
	short	tex_dcsw;		// 科目対応摘要 処理の 借方(0), 貸方(1) サイン

	char	tex_tky[82];	// 摘要
	DWORD	tex_sel;		// 摘要選択
	int		tex_pos;		// 初期カーソル位置(バイト単位)
	int		tex_jobtyp;		// 登録 処理タイプ

//	char	tex_dmy[80];
	//以下 モジュールでセットする。
	WORD	tex_brnsgn;		// D0:借方枝番セット, D1:貸方枝番セット
	int		tex_brndeb,		// セットする 借方枝番コード
			tex_brncre;		// セットする 貸方枝番コード

	char	tex_invno[16];	// インボイス登録番号
	BYTE	tex_menchk;		// 免税の仕訳の場合は=1
	char	tex_dmy[70-16-1];
} TKY_REGPAREX;


// 伝票修正ダイアログ関数用 パラメータ
typedef struct _DENP_INPUTPAR
{
	CDBZmSub*	pZmsub;
	CDBSyohi*	pSyohi;

	CWnd*		parent;
	ICSWinApp*	winApp;

	int			seq;		//修正する仕訳番号
	char		nosyzmdf;
	BYTE		modeless;	// 1: モードレスダイアログで作成し、モーダルダイアログのように表示する
							//    ダイアログ破棄の関数をプログラム終了時に別途呼び出す必要がある。
	char		dmy[10];

	std::vector<int>	addseq;		//追加したSEQ
	std::vector<int>	addins_seq;	//追加するときの挿入SEQ
	std::vector<int>	mdfseq;	//修正したSEQ
	std::vector<int>	syzseq;	//消費税を修正したSEQ

	char		rsv[128];

} DENP_INPUTPAR;



// ScanItemDialog() の nFlag
#define SCF_KMKCODE		0x0001		// 科目コードを表示
#define SCF_BRNTKYO		0x0002		// 枝番摘要名称を表示
#define SCF_SYOGBRN		0x0004		// 諸口枝番名称を表示
#define SCF_TKYSWK		0x0008		// 仕訳摘要 の表示を優先

// 検索順
#define	SCT_ORDERSEQ	0
#define SCT_ORDERDATE	1
#define SCT_TKYONLY		0x80
// 検索データ
#define SCD_CURUESR		0
#define SCD_ALLDATA		1
// 検索タイプ
#define SCS_AND			0	//AND
#define SCS_OR			1	//OR

// 検索オプション
#define SC_NOTSEARCH	0x01		// NOT 検索
#define SC_ITEMNONE		0x02		// 項目なし 状態
#define SC_KMKHANI		0x04		// 科目範囲検索

#define SC_FURIDEN		0x0100		// 振替伝票
#define SC_NYUKIN		0x0200		// 入金伝票
#define SC_SYUKKIN		0x0400		// 出金伝票

// 消費税サイン
#define SCZ_NONSELECT	0	// 未選択サイン

//--- 消費税仕訳区分サイン
#define	SCZ_KEHIURI		1
#define SCZ_KASISON		2
#define SCZ_KASISYU		3
#define SCZ_S_IMPORT	4
#define SCZ_H_EXPORT	5
#define SCZ_HAB_HIYO	6
#define	SCZ_HAB_SYUN	7
#define SCZ_URIWARI		8
#define SCZ_SHIWARI		9
#define SCZ_TANA_PL		10
#define SCZ_TANA_MN		11
#define	SCZ_TOKT_SYU	12

//--- 税率 サイン
#define	SCZ_ZR_5		1
#define	SCZ_ZR_3		2
#define SCZ_ZR_0		3
//--- 消費税区分サイン
#define	SCZ_SK_SOTO		1
#define	SCZ_SK_UCHI		2	
#define	SCZ_SK_BETU		3
#define	SCZ_SK_HIKZ		4
#define	SCZ_SK_KAZI		5
#define	SCZ_SK_HUKA		6
//--- 売上業種区分サイン
#define	SCZ_GY_OROSI	1
#define	SCZ_GY_KOURI	2
#define	SCZ_GY_SEIZO	3
#define	SCZ_GY_SRVCE	5
#define	SCZ_GY_ETC		4
//--- 仕入区分サイン
#define	SCZ_URI_KAZEI	1
#define	SCZ_URI_HIKAZEI	2
#define	SCZ_URI_KYOTU	3

// 摘要検索オプション
#define STK_NORMAL		0	// 通常摘要検索
#define STK_IMG			1	// イメージ摘要のみ検索
#define STK_STR			2	// 文字摘要のみ検索
#define STK_STRNONE		3	// 文字未入力摘要の検索

/*---VER2----*/
// 入力漏れ
#define SC_NOINP_BRN	1	// 枝番入力漏れ
#define SC_NOINP_BMON	2	// 部門入力漏れ
#define SC_NOINP_KOJI	3	// 工事入力漏れ 
#define SC_NOINP_BMNBRN	4	// 部門+枝番入力漏れ

#define SC_NOINP_DENP	5	// 伝票番号入力漏れ
#define SC_NOINP_BMORBR	7	// 部門or枝番入力漏れ

#define SC_NOINP_INVNO	8	// 登録番号


// 業種の区分
#define SC_GYOKBN_IPPAN		0x01	// 一般
#define SC_GYOKBN_HUDOSAN	0x02	// 不動産
#define SC_GYOKBN_NOGYO		0x04	// 農業


//変動事由・振替区分検索用
#define HJIYU_MITOROK	99
#define HJIYU_TAISYO	100

//免税事業者からの課税仕入　控除割合サイン
#define SYZ_MEN_NOT		-1
#define SYZ_MEN_80		0	//80%控除の期間
#define SYZ_MEN_8050	1	//80%・50%控除が混在する期間
#define SYZ_MEN_50		2	//50%控除 の期間


// 検索項目 構造体
typedef struct _SCAN_ITEM
{
	BYTE	sc_type,	// 検索順
			sc_duser,	// 検索対象データ
			sc_stype,	// 検索タイプ
			sc_dmy1;
	
	int		sc_seq1, sc_seq2;
	WORD	sc_seqopt;
	BYTE 	sc_date1[2], sc_date2[2];
	WORD	sc_dateopt;
	int		sc_denp1, sc_denp2;
	WORD	sc_denpopt;
	int		sc_dbmn1, sc_dbmn2;
	WORD	sc_dbmnopt;
	int		sc_cbmn1, sc_cbmn2;
	WORD	sc_cbmnopt;

	char	sc_dkji1[14], sc_dkji2[14];
	WORD	sc_dkjiopt;
	char	sc_ckji1[14], sc_ckji2[14];
	WORD	sc_ckjiopt;

	char	sc_dbt[10];
	WORD	sc_dbtopt;
	int		sc_dbr1;
	int		sc_dbr2;
	WORD	sc_dbropt;
			
	char	sc_cre[10];
	WORD	sc_creopt;
	int		sc_cbr1;
	int		sc_cbr2;
	WORD	sc_cbropt;

	char	sc_dbcr[10];
	WORD	sc_dbcropt;
	int		sc_dcbrn1;
	int		sc_dcbrn2;
	WORD	sc_dcbropt;

	char	sc_val1[6],	sc_val2[6];
	BYTE	sc_valsgn[2];		// 1 = 入力あり
	WORD	sc_valopt;
	char	sc_zei1[6],	sc_zei2[6];
	BYTE	sc_zeisgn[2];		// 1 = 入力あり
	WORD	sc_zeiopt;
	char	sc_tai1[6],	sc_tai2[6];
	BYTE	sc_taisgn[2];		// 1 = 入力あり
	WORD	sc_taiopt;

	// 消費税関係
	short	sc_syzsw, sc_zritu, sc_skbn, sc_urisire;
	WORD	sc_syzswopt, sc_zrituopt, sc_skbnopt, sc_urisireopt;

	char	sc_tky[82];
	WORD	sc_tkytyp, sc_tkyopt;

	char	sc_snum1[10], sc_snum2[10];
	WORD	sc_snumopt;

	int		sc_tgdate1, sc_tgdate2;
	WORD	sc_tgdtopt;

	short	sc_husen;
	char	sc_hucmnt[12];
	WORD	sc_husenopt;

	short	sc_noinp;
	short	sc_hjiyu;

	short	sc_sofs, sc_eofs;
	// 特定収入
	short	sc_tokus;
	WORD	sc_tokusopt;

	short	sc_gyokbn;
	char	sc_dbt2[10];
	char	sc_cre2[10];
	char	sc_dbcr2[10];

	// 証憑番号 11.26 /12 追加
	char	sc_docev1[10], sc_docev2[10];
	WORD	sc_docevopt;

	// 貸借部門		05.14 /13
	int		sc_dcbmn1, sc_dcbmn2;
	WORD	sc_dcbmnopt;
	// 貸借工事		05.14 /13
	char	sc_dckji1[14], sc_dckji2[14];
	WORD	sc_dckjiopt;

//	char	sc_rsv[118-30];
//	char	sc_rsv[118-52];
	char	sc_rsv[118-92];

} SCAN_ITEM;


// 検索項目 構造体 拡張版
typedef struct _SCAN_ITEMEX
{
	DWORD	sc_size;	// 構造体サイズ

	BYTE	sc_type,	// 検索順
			sc_duser,	// 検索対象データ
			sc_stype,	// 検索タイプ
			sc_dtype;	// データタイプ
	
	int		sc_seq1, sc_seq2;
	WORD	sc_seqopt;
	BYTE 	sc_date1[2], sc_date2[2];
	WORD	sc_dateopt;
	int		sc_denp1, sc_denp2;
	WORD	sc_denpopt;
	int		sc_dbmn1, sc_dbmn2;
	WORD	sc_dbmnopt;
	int		sc_cbmn1, sc_cbmn2;
	WORD	sc_cbmnopt;

	char	sc_dkji1[14], sc_dkji2[14];
	WORD	sc_dkjiopt;
	char	sc_ckji1[14], sc_ckji2[14];
	WORD	sc_ckjiopt;

	char	sc_dbt[10];
	WORD	sc_dbtopt;
	int		sc_dbr1;
	int		sc_dbr2;
	WORD	sc_dbropt;
			
	char	sc_cre[10];
	WORD	sc_creopt;
	int		sc_cbr1;
	int		sc_cbr2;
	WORD	sc_cbropt;

	char	sc_dbcr[10];
	WORD	sc_dbcropt;
	int		sc_dcbrn1;
	int		sc_dcbrn2;
	WORD	sc_dcbropt;

	char	sc_val1[6],	sc_val2[6];
	BYTE	sc_valsgn[2];		// 1 = 入力あり
	WORD	sc_valopt;
	char	sc_zei1[6],	sc_zei2[6];
	BYTE	sc_zeisgn[2];		// 1 = 入力あり
	WORD	sc_zeiopt;
	char	sc_tai1[6],	sc_tai2[6];
	BYTE	sc_taisgn[2];		// 1 = 入力あり
	WORD	sc_taiopt;

	// 消費税関係
	short	sc_syzsw, sc_zritu, sc_skbn, sc_urisire;
	WORD	sc_syzswopt, sc_zrituopt, sc_skbnopt, sc_urisireopt;

	char	sc_tky[82];
	WORD	sc_tkytyp, sc_tkyopt;

	char	sc_snum1__[10], sc_snum2__[10]; //電子記録債権で廃止する
	WORD	sc_snumopt;

	int		sc_tgdate1, sc_tgdate2;
	WORD	sc_tgdtopt;

	short	sc_husen;
	char	sc_hucmnt[12];
	WORD	sc_husenopt;

	short	sc_noinp;
	short	sc_hjiyu;

	short	sc_sofs, sc_eofs;
	// 特定収入
	short	sc_tokus;
	WORD	sc_tokusopt;

	short	sc_gyokbn;
	char	sc_dbt2[10];
	char	sc_cre2[10];
	char	sc_dbcr2[10];

	// 証憑番号 11.26 /12 追加
	char	sc_docev1[10], sc_docev2[10];
	WORD	sc_docevopt;

	// 貸借部門		05.14 /13
	int		sc_dcbmn1, sc_dcbmn2;
	WORD	sc_dcbmnopt;
	// 貸借工事		05.14 /13
	char	sc_dckji1[14], sc_dckji2[14];
	WORD	sc_dckjiopt;

	char	sc_hucmntEX[42];
	// 文書番号・帳票種別 01.14 /16
	int		sc_docseq1, sc_docseq2;
	WORD	sc_docseqopt;
	short	sc_docshtyp;
	// 資産 09.01 /16
	BYTE	sc_syzktsn;
	// 未確定仕訳
	BYTE	sc_mikad;

	// インボイス対応
	// 登録番号
	char	sc_invno[16];
	BYTE	sc_invsgn;
	WORD	sc_invnoopt;

	// 免税事業者からの課税仕入れ
	BYTE	sc_menzei;
	BYTE	sc_menzeiopt;
	// 免税事業者 控除割合
	BYTE	sc_menwari;

	char	sc_rnum1[20], sc_rnum2[20]; //電子記録債権対応


	//char	sc_rsv[256-14];
	//char	sc_rsv[256-35];		// 登録番号( 16Byte + 1Byte + 2Byte )・免税事業者からの課税仕入れ( 2Byte ) 計21Byte追加
	//char	sc_rsv[256 - 36];	// 免税事業者 控除割合(1Byte)
	char	sc_rsv[256 - 76];	// 電子記録債権

} SCAN_ITEMEX;




// ICSDBEdt にデータをセット
DBSUB_FUNC long WINAPI DBdata_set( CWnd* pwnd, int id, VARIANT* data, int type, int bcdlen );

// ICSDBEdt からデータをゲット
DBSUB_FUNC long WINAPI DBdata_get( CWnd* pwnd, int id, VARIANT* data, int type, int bcdlen );

// ICSDBEdt に表示文字をセット
DBSUB_FUNC void WINAPI DBSetDispString( CWnd* pwnd, int id, LPCTSTR string );

// ICSDBEdt から科目データをゲット
DBSUB_FUNC int WINAPI DBkamokudata_get( CWnd* pwnd, int id, struct _KamokuData* data, CDBZmSub* pZm );

// ICSDBEdt に科目データをセット
DBSUB_FUNC int WINAPI DBkamokudata_set( CWnd* pwnd, int id, struct _KamokuData* data, CDBZmSub* pZm );

// 手形期日日付チェック
DBSUB_FUNC short WINAPI TgDateInputCheck( char* check, char* result, int re_sz, int c_ddate, int c_tdate, int s_tdate );

// うるう年かどうか？
DBSUB_FUNC BOOL WINAPI uru_year( int year );

DBSUB_FUNC int WINAPI EdabanTorokDialog( EDA_RECORD* pEd, CDBZmSub* pZm, CWnd* pwnd, BOOL* teki_query = NULL );
DBSUB_FUNC int WINAPI TekiyoTorokDialog( TKY_REGPAR* pTk, CDBZmSub* pZm, CDBSyohi* pSy, CWnd* pwnd, BOOL* teki_query = NULL );
DBSUB_FUNC int WINAPI TekiyoRegV2Dialog( TKY_VER2* pTkpar, CDBZmSub* pZm, CDBSyohi* pSy, CWnd* pwnd, BOOL* teki_query = NULL );
DBSUB_FUNC int WINAPI TekiyoTorokDialogEX( TKY_REGPAREX* pTkpar, CDBZmSub* pZm, CDBSyohi* pSy, CWnd* pwnd, BOOL* teki_query = NULL );
DBSUB_FUNC int WINAPI TkyEdaTorokDialog( TKY_REGPAREX* pTkpar, CDBZmSub* pZm, CDBSyohi* pSy, CWnd* pwnd, BOOL* teki_query = NULL );


DBSUB_FUNC int WINAPI ScanItemDialog( SCAN_ITEM* pSI, DWORD nFlag, CDBZmSub* pZm, CWnd* pwnd );
DBSUB_FUNC void WINAPI scanitem_init( SCAN_ITEM* si );

DBSUB_FUNC void WINAPI scanitem_initEX( SCAN_ITEMEX* si );

DBSUB_FUNC int WINAPI swktky_search( DWORD flg, CString deb, CString cre, CDBZmSub* pZm );

DBSUB_FUNC void WINAPI DWORDcodeToSTRcode( DWORD kncod, char* string, int strsize );

DBSUB_FUNC int WINAPI Myvd_sign( BYTE* date, BYTE* dst_date, CDBZmSub* pZm );
DBSUB_FUNC int	WINAPI DateConv( BYTE* bcddate, int* date, int sgn );
DBSUB_FUNC int	WINAPI YmdConv( BYTE* bcdymd, int* date, int sgn );

DBSUB_FUNC void WINAPI BcdDateToString( char* str, int strsize, BYTE* bcddate, BOOL bMnthFix = FALSE );
DBSUB_FUNC void WINAPI BcdDateToString( CString& string, BYTE* bcddate, BOOL bMnthFix = FALSE );
DBSUB_FUNC void WINAPI IntsrekitoGenstr( char* str, int strsize, int ddate );

DBSUB_FUNC int WINAPI _op_max_calq( int keta );
DBSUB_FUNC BOOL WINAPI isdigitstr( char* p );
DBSUB_FUNC int WINAPI kana_chk( CString *bf, int ln );
DBSUB_FUNC void WINAPI trim_zero( char *bf, int sizebf, int ln );
DBSUB_FUNC int WINAPI ksp_cut( char *des, char *sou, int n );

DBSUB_FUNC void WINAPI kana_copy( char *copybuf, LPCTSTR kana );
DBSUB_FUNC void WINAPI kana_check(char *copybuf, LPCTSTR kana);
/*---VER2----*/
DBSUB_FUNC int WINAPI kmkcode_string( int type, DBKNREC* pKn, char *buf, int buflength );
DBSUB_FUNC BOOL IsIncludeString( LPCTSTR string, LPCTSTR chk );

DBSUB_FUNC BOOL WINAPI IsBillCode( LPCTSTR kcd, BYTE apno );
DBSUB_FUNC BOOL WINAPI IsBillCodeEx(LPCTSTR kcd, CDBZmSub* pzm);

DBSUB_FUNC int WINAPI get_khkbntype( CString dbt, CString cre, CDBZmSub* pZm );
DBSUB_FUNC BOOL WINAPI IsHenJiyuCodeRight( int h_code, int h_type );

DBSUB_FUNC int WINAPI IsKojinGyosyuMaster( CDBZmSub* pZm );
DBSUB_FUNC BOOL WINAPI IsKojinKengyoMaster( int gyosgn );
DBSUB_FUNC BOOL WINAPI CheckKamokuRestrict( CDBZmSub* pZm, DBKNREC* pKn );
DBSUB_FUNC int WINAPI indkmk_dialog( CWnd* pwnd, DWORD& dwSign, CDBZmSub* pZm );

// 11.07 /08
DBSUB_FUNC void WINAPI KamokucodeSign( int sgn );


// dbzmgdtsub.cpp
DBSUB_FUNC unsigned short WINAPI hl_lev( short wd );
DBSUB_FUNC long WINAPI itoHEX( unsigned long integer );
DBSUB_FUNC long WINAPI HEXtoi( unsigned long integer );
DBSUB_FUNC BOOL WINAPI isSYOGT_CODE( LPCTSTR code, int nFlag = -1 ); // nFlag は 未使用
DBSUB_FUNC BOOL WINAPI isGNKIN_CODE( LPCTSTR code, int nFlag = -1 ); // nFlag は 未使用
DBSUB_FUNC LPCSTR WINAPI ffind( LPCSTR path, UINT sw=0 );

/*--- 公益 ('08.12.01) ---*/
DBSUB_FUNC int WINAPI IsKouekiMaster( CDBZmSub* pZm );
DBSUB_FUNC int WINAPI kpubkmk_dialog( CWnd* pwnd, unsigned long* pJgykcd, unsigned long* pKnrkcd, CDBZmSub* pZm );
DBSUB_FUNC int WINAPI IsExtendJgyOrKnrMaster( CDBZmSub* pZm );
DBSUB_FUNC int WINAPI GetJgyKnrDspstr( CDBZmSub *pZm, unsigned long jgykcd, unsigned long knrkcd, CString *pDspstr );

// 07.27 09
DBSUB_FUNC int WINAPI AdjustSQLsearch( CString& search );

// 07.20 /11
DBSUB_FUNC LPCTSTR WINAPI GNKINCode( int apno );

DBSUB_FUNC BOOL WINAPI IsSyafuKaiseiAnbun( CDBZmSub* pZm );

// 12.17 /12
DBSUB_FUNC void WINAPI DocEviAutoString( char* pDEstr, int strsize, int keta );

// 02.12 /13
DBSUB_FUNC int WINAPI IsGakkouMaster( CDBZmSub* pZm );
DBSUB_FUNC int WINAPI IsSyusiKamokuMaster( CDBZmSub* pZm );

// 09.04 /13
DBSUB_FUNC int WINAPI GetMasterVver( CDBZmSub* pZm );

DBSUB_FUNC int WINAPI GetTekiyoLength( CDBZmSub* pZm );

// 12.25 /14
DBSUB_FUNC int WINAPI IsGakkouH27Master( CDBZmSub* pZm );

// 08.97 /17
DBSUB_FUNC BOOL WINAPI IsIryoHjiyuCodeRight( int h_code, int h_type );

DBSUB_FUNC int WINAPI teki_stringcheck(char* str, int byte);

DBSUB_FUNC int WINAPI DenpInputDialog( DENP_INPUTPAR* para );

DBSUB_FUNC int WINAPI DenpTypeString( BYTE denp, CString& str );

DBSUB_FUNC int WINAPI TgDateBcdToSeireki( BYTE* bcd, CDBZmSub* pZm );

DBSUB_FUNC int WINAPI DBDinpSignUpdate(CDBZmSub* pZm, DWORD nFlag);

DBSUB_FUNC int WINAPI isBrnInpstr(char* chk);
DBSUB_FUNC int WINAPI lastBrnInpStr(LPCTSTR string, char* numstr, int strsize);

DBSUB_FUNC int WINAPI GetKanaSearchType();

DBSUB_FUNC int WINAPI issykamok(DBKNREC* pKn);
DBSUB_FUNC int WINAPI is_sisan_syutok(DBKNREC* pKn);
DBSUB_FUNC int WINAPI is_sisan_jyoto(DBKNREC* pKn);
DBSUB_FUNC int WINAPI is_kasitaore(DBKNREC* pKn);
DBSUB_FUNC int WINAPI uri_attr_igai(DBKNREC* pKn);
DBSUB_FUNC int WINAPI sire_attr(DBKNREC* pKn);

DBSUB_FUNC void WINAPI sql_sire(CString& sql);
DBSUB_FUNC void WINAPI sql_sire_EX(CString& sql);
DBSUB_FUNC int WINAPI GetMenzeiWariai(CZVOLUME* zvol);

//消費税欄に毎回止まるかどうかを取得する
//０：止まらない
//１：消費税欄に毎回止まる
DBSUB_FUNC int WINAPI GetSyzStopMode();
//消費税ダイアログ、登録番号の一桁入力用関数
DBSUB_FUNC void WINAPI func_invketachk(UINT idc, CWnd* pwnd, LPCTSTR imestring);

DBSUB_FUNC CSize WINAPI DBEditTextSize(UINT dbedtID, CWnd* pwnd, int bytes);



// データレコードクラス
// 配列などにしたい場合のため
//  DBDinpRec.h の CDBipDataRec から こちらに変更 (2024/07/24)
//	手形番号を拡張するのに、サイズが足らなくなったため
//
class DBSUB_FUNC CDBINPDataRec
{
public:
	CDBINPDataRec();
	~CDBINPDataRec();

	void Reset();

	void operator=(const CDBINPDataRec& data);
	BOOL operator==(const CDBINPDataRec& data);
	BOOL operator!=(const CDBINPDataRec& data);

public:
	int			m_seq;
	BYTE		m_ksign;
	int			m_ddate;
	BYTE		m_mofs;
	int			m_cno;
	int			m_dbmn;
	CString		m_dkno;
	CString		m_dbt;
	int			m_dbr;

	int			m_cbmn;
	CString		m_ckno;
	CString		m_cre;
	int			m_cbr;
	char		m_val[6],
		m_zei[6];

	CByteArray	m_dsign;
	int			m_owner;
	BYTE		m_dtype;

	CString		m_tekiyo;
	int			m_imgsq;
	CString		m_snumber;
	int			m_tdate;
	int			m_edate;
	int			m_udate;
	long		m_icno;		// 内部伝票番号

	CString		m_skn;		// 資金繰り科目
	CString		m_tag;		// 付箋コメント
	CByteArray	m_tsign;

	BYTE		m_hjiyu;		//変動事由
	char		m_taika[6];		//対価
	int			m_nbcd;			//内部取引用コード[01.25 /12]

	char		m_invno[16];	//インボイス登録番号
	CString		m_rnumber;		//債権記録番号

	char		rsv[256];
};










#endif // INCLUDE_DBINPSUB_H



