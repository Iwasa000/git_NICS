// 通常入力用 派生した財務クラス

#ifndef INCLUDE_DIPZMSUB_H
#define INCLUDE_DIPZMSUB_H

#include "sql.h"
#include "sqlext.h"
#include "odbcinst.h"
#include <afxdb.h>
#include <afxdao.h>
#include <dbsyohi8.h>


// 部門／工事名称 選択表示
typedef struct _BMNSEL_REC
{
	CString bkcod;		// 部門／工事コード
	CString s_txt;		// 選択表示文字列

} BMNSEL_REC;


// 部門／工事名称 テーブル管理
typedef struct _BMNSEL_TB
{
//	OLE_COLOR		ForeCol;	// 表示文字色

	unsigned short	bninp;		// 部門名称の実登録数
	
	unsigned short	page_max;	// 最大ページ
	unsigned short	page_now;	// 現在ページ
	unsigned short	page_recn;	// 1ページ分のレコード数
	unsigned short	rec_n;		// 現在ページのレコード数

	BMNSEL_REC		*pBSEL_REC;	// 作成テーブル

} BMNSEL_TB;


// 科目名称テーブル
typedef struct _DBKMK_TBL
{
	long	dk_knseq;
	CString	dk_kncod;
	long	dk_knicod;
	BYTE	dk_knatt;
	int		dk_knfix;
	CString	dk_knkana;
	BYTE	dk_knsgn[10];
	int		dk_knrtb;
	int		dk_knkpg;
	int		dk_knfkn;
	BYTE	dk_kngkb;
	CString	dk_knnam;

} DBKMK_TBL;


// 型の違う変数へ変換する関数 サイン
#define CV1_2	0	// 第一引数より第二引数へ
#define CV2_1	1	// 第二引数より第一引数へ

// 決修サインを BCD にセットする(月の部分)
//	1 = 第一４半期, 2 = 第二４半期(中間)
//	3 = 第三４半期, 4 = 第四４半期(決修)
//
#define KESY_SG_GET( x )		( (x & 0xe0)>>5 )
#define KESY_SG_SET( x, sgn )	x |= (sgn << 5) 


// CDBDATA データソートモード
#define	DBDATA_SORT_SEQ		0	// SEQ 順
#define DBDATA_SORT_DATE	1	// 日付順
// CDBDATA ジョブタイプ
#define DBDATA_JOB_NORMAL	0	// 通常時
#define DBDATA_JOB_SCAN		1	// スキャン時
#define DBDATA_JOB_DENP		2	// 伝票時
#define DBDATA_JOB_ACBOOK	3	// 出納帳時

#define SIZE_DSIGN 6


// 部門名称レコード map value 用(key bnbcd)
typedef struct _BMNAME_DATA
{
//	long	bnbcd;
	CString	bnbnm;

	BYTE	bnhsg[5];
	BYTE	dmy;
	long	bnhw[5];
	long	prbcd;
	long	klevel;
	long	pseq;
	long	dseq;

	int		bnsysgn;

} BMNAME_DATA;



class CDipZmSub : public CDBNpSub/*CDBZmSub*/
{
public: 
	CDipZmSub();
	~CDipZmSub();

public:
	// データクラスを 財務クラスの CDBDATA にセット
	void GetCDBData( CDBINPDataRec* rec, CDBDATA* ptr = NULL );
	void GetCData( CDBINPDataRec* rec );
	void SetCDBData( CDBINPDataRec* rec );
	void SetSyTkrec( _SY_TKREC_ *psyTkRec, CDBipTKREC *rec, CDBINPDataRec* data);
	bool CheckTkrec( CDBipTKREC *rec );
	bool CheckTkrec(_SY_TKREC_ *syrec);

	int	DB_TekiyoNoRead( CDBipTKREC* tek_rec, int no );

	int DB_DataCorrect( CDBINPDataRec* data );
	int DB_DataCorrect();
	int DB_DataAppend( CDBINPDataRec* data, int bseq );
	int DBDATA_SetPosition( int seq );

	struct _DBKNREC* DB_PjisToKnrec( CString pjiscode, BOOL bALL = FALSE );
	struct _DBKNREC* DB_PjisToKnrec( DWORD dwPjis, BOOL bALL = FALSE );
	int DB_PjisToKmkOfst( CString pjiscode );
	int DB_PjisToKmkOfst( DWORD pjis );

	int DB_KmkRecPjisSearch( CString pjiscode );
	int DB_KmkRecInpcSearch( CString inpc );
	int DB_KmkRecRnoSearch( int rno );
	int DB_TekiyoEdabanSet( CDBINPDataRec *data, int tkcode, BOOL bBmn );

	LPCTSTR GetDenpFormat();
	LPCTSTR GetBumonFormat();
	LPCTSTR GetKojiFormat();
	int GetEdabanMaxDecimal();
	int GetBumonMaxDecimal();
	int GetKojiMaxDecimal();

	void BumonCodeToStr( char* str, int strsize, int bmoncode );
	void KojiCodeToStr( char* str, int strsize, CString kojicode );
	void EdabanToStr( char* str, int strsize, int edacode );

//	int Myvd_ofset( int ddate );
//	int Myvd_ofset( BYTE* bcddate );
	int Myvd_chek( BYTE* date );
	int Myvd_sign( BYTE* date, BYTE* dst_date );
	
	int	DateConv( BYTE* bcddate, CDBINPDataRec* d_date, int sgn );
	int	DateConv( BYTE* bcddate, int* date, int sgn );
	int	YmdConv( BYTE* bcdymd, int* date, int sgn );
	int BcdDateToYmd( BYTE* bcddate, int* ymd );
	void BcdDateToString( char* str, int strsize, BYTE* bcddate, BOOL bMnthFix = FALSE );
	void BcdDateToString( CString& str, BYTE* bcddate, BOOL bMnthFix = FALSE );
//	void IntsrekitoGenstr( char* str, int ddate );

	void TV_kamokucodeConv( WORD& kmkcode, CString& codestr, int sgn );
	void TV_EdabancodeConv( WORD& edaban, int& new_eda, int sgn );

	int	DB_EdabanMasterSearch( CDBINPDataRec* data, int dbcr_sg );
	int DB_EdabanZanSearch( CString& kmkcode, int brn );
	int DB_EdabanZanSearch2( CString& kmkcode, int tkcod );
	int EdabanZanTorok( CDBipEZREC* ez );

	int DB_BmnEdabanMasterSearch( CDBINPDataRec* data, int dbcr_sg );
	int DB_BmnEdabanMasterSearch( CDBINPDataRec* data, int dbcr_sg, int tkcod );
	int BumonZanTorok( int bmn, CString  kmcod );
	int BmnEdabanZanTorok( CDBipEZREC* ez, int bmn );

	// 仕訳コピー後の処理
	int SwkCopyAfterJob();

#ifdef DB_OLD_CLOSE
	// 部門名称テーブル
	int mk_BmnSelTbl();
	int _mk_BmnSelTbl();
	int _mk_KojiSelTbl();
	void Bm_CloseWork();

	int getBmnSel( BMNSEL_TB **tbl, int page_sz, int page, int type = 0 );
	LPCTSTR get_bmnmsg( int* bmn, BOOL full, int mode );
	LPCTSTR get_kojimsg( CString* koji, BOOL full, int mode );

	WORD get_bmnnowpg() { return m_Btb_PAGE.page_now; };
	WORD get_bmnmaxpg() { return m_Btb_PAGE.page_max; };
	void set_bmnnowpg( WORD nowpg );

	BMNSEL_TB* GetBmnPageTbl() {
		return m_Btb_PAGE.bninp ?  &m_Btb_PAGE : NULL;
	}
	BOOL get_bmncod( int* pbmn, int number );
	BOOL get_kojicod( CString* koji, int number );
#endif

	BOOL IsGokeiBumon( int bmn );
	BOOL bmncode_ok( int bmn );
	// CBDATA 状態
	int& dbdata_Sort()	{ return m_dbSort; }
	int& dbdata_Job()		{ return m_dbJob;	};
	BOOL& dbdata_Requery()	{ return m_dbRequery; };	// 追加をした場合等、Requery の必要あり
	BOOL& dbdata_Speed()	{ return m_dbSpeed; };		// 表示の速度UPするため
	BOOL& dbdata_Reload()	{ return m_dbReload; };		// 他のプログラムで、マスターが更新された場合のとき ON

	BOOL& data_Requery()	{ return m_bRequeryDt; };	// 起動時に仕訳をラインに表示するため
	BOOL& data_speed()		{ return m_bDtSpeed; };		// 表示の速度UPするため

	BOOL& kzrec_Requery()	{ return m_kzRequery; }		// 科目残高 Requery
	BOOL& tkrec_kanamode()	{ return m_bTk_kana; };

	BOOL& insseq_make()		{ return m_bInsSeq; };		// 挿入SEQ順テーブル作成

	BOOL& dbdata_scan_insert()	{ return m_bScanIns; };	// スキャン時の挿入処理か？

	int DI_SetMasetrDB(char *server, unsigned char apno, int cod, int ymd, int ntype = 0, char *id = 0, char *pwd = 0 );
	int DI_SetMasetrDB(int valid_ver, char *server, unsigned char apno, int cod, int ymd, int ntype = 0, char *id = 0, char *pwd = 0 );

	void setuptbl_make();
//	int object_check( int sign, char *objectname );

	void get_datelimit( int& sofs, int& eofs, int& sy, int& ey );
	void set_datelimit( int sofs, int eofs, int sy, int ey );
	BOOL check_datelimit( BYTE* bcddate );
	void get_ofsdate( int ofs, int sgn, BYTE* bcddate );
	void get_ofsdate( int ofs, BYTE* bcddate );

	void MakeInpDate( CDBINPDataRec* rec );
	void GetInpDate( CDBINPDataRec* rec );
	void GetInpDate( BYTE* bcddate );
	void SetInpDate( BYTE* bcddate );

	void get_hjiyustr( BYTE h_code, CString& str );
	int	 km_syattr( DBKNREC* pKn );
	int	 km_syattr( CString& kcode );
	int	 km_zeikbn( DBKNREC* pKn );
	int	 km_zeikbn( CString& kcode );
	int	 km_zeiritsu( DBKNREC* pKn );
	int	 km_zeiritsu( CString& kcode );

	int  km_siire(DBKNREC* pKn);

	int	 km_yusyutsu(DBKNREC* pKn);
	int	 km_yusyutsu(CString& kcode);

	int data_openmode();
	int data_authority();

	int zanview_authority();

	int inpdate_shimechk( BYTE* orgdate, char* check );
	int IsKaniOrKobetsu();
	int IsKaniSyz();
	int IsMenzeiSyz();
	int IsKobetsuSyz();

	LPCTSTR	get_hjiyutitle( int strtype );
	DBGKREC* get_gksrc( DWORD gkcode );

	int MakeUcomSeldate( char* sbf, int sz_sbf, char* ebf, int sz_ebf );

	// 最後に書き込んだデータ
	CDBINPDataRec	m_lastdata;
	int		m_damax;
	int		m_dacnt;
	int		m_edmax;
	int		m_tkmax;
	int		user_att;	//ユーザ属性

	int		m_insCnt;

	int		m_sortMode;	//0:Seq順、1:日付順

	// 仕訳等修正したか？
	BOOL&	IsModify() { return m_bModify;	};
	// 取消仕訳スキャンか？
	BOOL&	IsDelScan() { return m_bDelMode; }
	// 重複仕訳スキャンか？
	BOOL&	IsDupliScan() { return m_bDupliMode; }
	// WIZ連携仕訳スキャンか？
	BOOL&	IsWizScan() { return m_bWizMode; }

	// 建設の工事漏れチェック用 科目コード配列
	CDWordArray	m_KJchk;

	// 個人科目表示制限
	DWORD	m_dwIndSgn;

	// 公益事業費科目制限 ('08.12.01)
	DWORD	m_Jgykcd;
	// 公益管理費科目制限 ('08.12.01)
	DWORD	m_Knrkcd;

	// 枝番を追加したか？
	BOOL&	IsEdaAdd() {return m_bEdaAdd; };

	// 社会福祉 按分の登録があるか
	void	CheckRegisterNphw();
	BOOL	IsRegisterNphw() { return m_bNPanbn; };

	void	SyzKariCode( CString& kbarai, CString& kuke );

	int		GetKariAutoBrnCnt();
	int		KamokuCodeCheck( CString& kmcode );

	void	CheckSiwakeCount();
	int		IsDiffEdabanSyzSgn(char* kmkcd, int brn);
	int		IsKobetsuBmnSyz();

	map<int, BMNAME_DATA>	m_bmnMap;
	int		MakeBmnNameMap();
	DWORD	KobetsuSiwakeCheck(CDBINPDataRec* rec, int bmn);

	BOOL	IsTokuteiSyunyuData(CDBINPDataRec* rec);
	BOOL	is_invnosiwake(CDBINPDataRec* prec);

	int		FuncTekiyoToRecord(CDBipTKREC* tek_rec);

private:
#ifdef OLD_CLOSE
	int m_BMNSEL_MOD;
	BMNSEL_TB	m_Btb, m_BtbKOJI,	// 各データ格納
				m_Btb_PAGE;			// 現在ページデータ
#endif

	DBKMK_TBL*	m_pKmTBL;
	int			m_KmCnt;

	int			m_DBstart,		// CDBDATA の現在のスタートSEQ
				m_DBend;		// CDBDATA のエンドSEQ

	int		m_dbSort, m_dbJob;
	BOOL	m_dbRequery, m_dbSpeed, m_dbReload;

	BOOL	m_bRequeryDt, m_bDtSpeed;
	BOOL	m_kzRequery;
	BOOL	m_bTk_kana;

	int		m_inpSofs, m_inpEofs, m_inpSy, m_inpEy;
	BYTE	m_InpBcd[4];

	BOOL	m_bModify, m_bDelMode, m_bDupliMode, m_bWizMode;

	BOOL	m_bEdaAdd;

	BOOL	m_bNPanbn;

	BOOL	m_bInsSeq;
	BOOL	m_bScanIns;

	CString			m_ErrMsg;
};


#endif // INCLUDE_DIPZMSUB_H