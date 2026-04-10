/*=====================

 	data scan sub 
	scan.h

=======================*/

//ページ戻り用の テーブル

typedef struct _DB_PGTBL
{
int	s_seq,		// 開始SEQ
	s_ddate,	// 開始日付
	s_ksign;	// 開始決修サイン
int	e_seq,		// 終了SEQ
	e_ddate,	// 終了日付
	e_ksign;	// 終了決修サイン

} DB_PGTBL;

// 日付順のとき、日付が変更されると、順番のチェックがおかしくなるので、
// 変更前の状態を保存しておく
typedef struct _DB_MDFYDATE
{
int	mo_seq,		// 修正前の SEQ
	mo_ddate,	// 　　　　 日付
	mo_ksign,	// 　　　　 決修サイン
	mo_cno;		//			伝票

} DB_MDFYDATE;

typedef CArray<DB_PGTBL, DB_PGTBL&>			CPGTblArray;
typedef CArray<DB_MDFYDATE, DB_MDFYDATE&>	CMfdtArray;


#define	INS_KEYSCAN_SORT	100	//カラムソートをしていいない状態で、仕訳挿入をした場合。


typedef struct _DB_SCANINFO
{
int			data_owner;	// データオーナ Requery の引数参照
int			data_mode;	// ソート順		0 SEQ 順
						//				1 日付順
int			tbl_end;	// テーブルが最後まで作成されたか？
int			mk_datetbl;	// 日付順の場合の SEQ テーブルを作成したか？

int			sort_id;	// ソートしたカラムID
int			sort_type;	// 昇順[ = DCI_SORTASCENDING] or 降順[ = DCI_SORTDESCENDING]
int			sort_cnt;	// ソートしたデータ数
int			mk_dbdata;	// dbdataのSEQテーブルを作成したか？

//仕訳挿入を行って、スキャンデータの並びに追加する分
// いったん仕訳挿入を終了後は、並びに追加してから 消去する。
int			ins_data;	//挿入データ数
CDWordArray	insSeq;		//挿入データのSEQ 配列
// 上記の挿入仕訳を消去しても、スキャンが継続している場合は、そのスキャン中に保存した挿入仕訳を保存。
CDWordArray	scanInsSeq;	//スキャン中のすべての挿入データSEQ
int			ins_query;	//挿入データを含めて Requeryをする

CString		filter;		// もとのフィルター文字列
// カラムクリック時のソート順保存
CString		sortOrder;

} DB_SCANINFO;


void set_order_num( int o_num );
int get_order_num();

void sdat_sft( CDBINPDataRec *, int );
void scroll_up( struct _DATA_LINE *, int, int );
void scroll_down( struct _DATA_LINE *, int, int );
int data_for( CDBINPDataRec *, CDBINPDataRec *, int );
int data_nxt_for( CDBINPDataRec *, CDBINPDataRec *, int );
int data_back( CDBINPDataRec *, CDBINPDataRec *, int, int seq = 0 );
int data_nxt_back( CDBINPDataRec *, CDBINPDataRec *, int, int seq = 0 );
// 07.30 /08
int data_search( CDBINPDataRec *, CDBINPDataRec *, int, int seq = 0 );

int make_scandateseq();

int datarec_seqcmp( int mode, int seq, int seq2 );

int dbip_data_cmp( int mode, int seq, int ddate, int ksign, int seq2, int ddate2, int ksign2 );
int dbip_data_cmp( int mode, int seq, int ddate, int seq2, int ddate2 );
int dbip_data_cmp( int mode, CDBINPDataRec* rec1, CDBINPDataRec* rec2 );

int scandat_getn( CDBINPDataRec *, CDBINPDataRec *, int, int seq = 0 );
// 03.01 /12
int endkeyscan_getn( CDBINPDataRec *getrec, CDBINPDataRec *keyrec, int getcnt, int line, int seq = 0 );

void scandat_dspn( CDBINPDataRec *, struct _DATA_LINE *, int );

void endscan(void);

// データテーブル再表示
void scandat_rdsp( CDBINPDataRec *dsprec, struct _DATA_LINE *ddata, int max );


//スキャンテーブル作成
int mk_scantbl( int rno, int max, int page_ln );
//スキャン読み出し開始ＳＥＱ
unsigned short get_topseq();
//検索総数セット
void set_scan_datan( int n );
//検索総数ゲット
int get_scan_datan();


// チェックリストテーブルﾌｧｲﾙ作成
void mk_chklst( struct _MFP fp /*CFile *fp*/ );
// チェックリストデータスレッド開始チェック
BOOL is_thread2_chk();
// チェックリストデータスレッド終了チェック
void end_thread2_chk();
// ﾌｧｲﾙの削除
void del_file( char *pFileName );

//#* 03.15 /03
int SeqtblRead( LPCTSTR path );

// 05.07 /13
int scaninsdat_getn( CDBINPDataRec *getrec, int keyseq, int getcnt, int seq = 0 );

// 06.07 /13
int data_keyscan_back( CDBINPDataRec *key, CDBINPDataRec *data,int nl, int seq = 0 );

// 07.18 /13
int make_scanorder( int order_id, int ad_typ );

// 12.15 /17
int insert_keyscanseq( int seq, int bseq );
void check_scan_insertquery();
void get_scanfilter( CString& filter );

void query_denpend( int icno, int seq );