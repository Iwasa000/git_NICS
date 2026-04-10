// CShinCalculateW

#ifndef  __ICS_SHINCALCULATEW__
#define  __ICS_SHINCALCULATEW__

#include "ShinCalculate8.h"
#include "ShinCalculateRec8.h"

//★//[12'03.12]///
//#define	VERSIONUP TRUE
///////////////////

const	int	ERR	= -1;

// 実体クラス
class CShinCalculateW {
public:
	CShinCalculateW();
	~CShinCalculateW();

public:
	CShinCalculate*	pShincalculate;

	CDBNpSub		*pZaimuSub;				// 公益クラス

	CVolDateDB		*pVolDate;				// 期間関数クラス
	CTblhdl			*pTblHdl;				// テーブル関数クラス
	CArithEx		*pArithEx;				// 倍長演算

	char			Jpath[64];				// プログラム・デバイス
	char			Cpath[64];				// 会社情報デバイス
	char			Spath[64];				// 共通・デバイス
	char			SGpath[64];				// 〃
	char			Hpath[64];				// 消費税申告書スケジュール
	char			GroupTh[64];			// グループ名
	char			MDev[4];
	char			MDdm[4];

	// 消費税マスター
	SVHED			TBhead;
	SVHED			SVhead;
	H9SUM			SVsum;
	SN_HEAD			Snh;
	SN_DATA			Snd;	

	// 財務ボリューム
	VOL1			Vol1;
	VOL2			Vol2;

	unsigned char	Start[4];
	unsigned char	End[4];
	int				m_Calcusw;				// 0=全額控除 1=個別対応 2=比例配分 3=簡易課税
	int				m_Check;				// ON=チェック OFF=計算
	int				m_Type;					// 0:各業務　1:試算表・消費税額試算表・消費税申告書・課税売上割合
											// 1:翌期更新・前年度からの更新
	int				m_Kind;					// 法人・個人の別
											// 0:申告書のサインに依存　1=法人事業者 2=個人事業者 [12'02.02]

	unsigned char	m_Compare,				// シミュレーションサイン ON:行う　OFF:行わない
					m_Simple,				// 簡易課税の計算サイン	ON:計算をしない OFF:計算をする
					m_Category,				// 1=卸売 2=小売 3=製造 4=ｻｰﾋﾞｽ 5=その他
					m_Entry,				// 仕入区分登録種別　ON:科目別・分類別に設定する。OFF:分類別のみ設定する
					m_BasisStock,			// 1=課税売上げに係る 2=非課税売上に係る 3=共通に係る
					m_SumSettleSw;			// 消費税申告書集計時の最終月の集計方法(★13'07.25)
											//					0:12ヶ月＋決算月で集計する
											// 　　　〃　　     1:12ヶ月のみで集計する
											// 　　　〃			2:決算月のみで集計	

	// テーブルハンドリング
	_TB_PAR			foreA;					// 消費税処理グループ区分単位 (naG)
	_TB_PAR			foreB;					// 課税仕入個別対応区分・簡易課税売上業種区分単位 (naG)
	_TB_PAR			foreC;					// 明細科目単位消費税レコード (naG)
	_TB_PAR			foreD;	

	_TB_PAR			KnTbl;

	////////////////
	// 変数エリア //
	////////////////

	// 仮受消費税／仮払消費税  残高格納
	struct	KARI	kr, kr3;

	// 仮払消費税集計エリア
	struct _SZREC	karibri_5;				// ５％
	struct _SZREC	karibri_3;				// ３％
	
	// 仮受消費税集計エリア
	struct _SZREC	kariuke_5;				// ５％
	struct _SZREC	kariuke_3;				// ３％

	// 棚卸調整額
	// 0:仮払5％＋３％
	// 1:課税／課税に係る5％ 　2:課税／課税に係る 3％ 
	// 3:　　／非課税に係る5％ 4:　　／非課税に係る3％ 
	// 5:　　／共通に係る3％   6:　　／共通に係る3％ 
	struct	_SZREC	stack_data[8];


	// 一括税抜時の変数
	short			m_Ikmon[20];
	char			m_Ikat, m_Ikdm;
	// 一括税抜時の仮払消費税額の科目残高集計エリア
	struct _SZREC	ikkrbri;				// ５％＋３％
	// 一括税抜時の仮受消費税額の科目残高集計エリア
	struct _SZREC	ikkruke;				// ５％＋３％
	// 精算の仮払消費税額の科目残高集計エリア
	struct _SZREC	sikrbri;				// ５％＋３％
	// 精算仮受消費税額の科目残高集計エリア
	struct _SZREC	sikruke;				// ５％＋３％

	// 個別対応時の仕入金額エリア
	struct	_SZREC	kobetu_5;
	struct	_SZREC	kobetu_3;


	int				INP_S;					// 開始年月 ｵﾌｾｯﾄ
	int				INP_E;					// 最終年月 ｵﾌｾｯﾄ
	int				Anbun,					// ﾎﾞﾘｭｰﾑﾗﾍﾞﾙ  1=個別対応  0=比例配分
					Srx,					// 課税売上割合  1= 95%以下  0= 95%以上
					Kani,					// 簡易課税選択時ＳＷ
					Skbn,					// 申告区分
					NfSw;
	char			s_tno, d_tno;			// 該当月   オフセット
	char			SYsw[14];				// 消費税集計ｻｲﾝ 
	int				K221sw;					// 規則２２条１項の適用（積み上げ方式）
	int				H301sw;					// 法第３０条第１項の適用（積み上げ方式）
	int				SimpSw;					// 簡易計算サイン
	int				RePrint;				// 再プリントサイン
	int				Gsw;					// 限界控除制度廃止期間チェックSW
	char			Kenei,					// 兼営事業選択ＳＷ
					toku;					// 特例計算適用ＳＷ
	char			N_ver, Ver;				// 消費税マスターバージョンＳＷ
	char			Kk_SG, Kk_DM;			// 期間短縮解除後の課税期間の開始日以降で申告書集計を行う。
	int				m_Middle;				// 前年実績による中間申告サイン [04'12.02]
	int				SetUpOfs;				// 設立時の開始オフセット

	char			ERMS[512], MoBf[512], CBF[128], KBF[128], MBF[128], DBF[10], BUF[128], asci[128], cnbuf[128];
	char			OPD, OPE, OPF, OPG, OPH, OPI, OPsign, OPdmm;

	char			SYMD[4];
	char			EYMD[4];
	char			CYMD[4];
	char			IYMD[4];
	DAYPAC			dtl[14];
	DATES			ymdtl[14];
//★//[11'12.26]///
//	short			daytl[14]; 
//	char			GEN1[4];				// ８年度改正年月日
//	char			GEN2[4];				// ９年度改正年月日＆限界控除廃止年月日
///////////////////
	int				Ssofs, Seofs, WSGN, Matu, VdSw, ItCt, ErNo, s_tbl, e_tbl, Kjct;

	char			WZURI[6];				// 変数設定
	char			Month26[14];			// 消費税申告書用月管理テーブル	[05'05.13]
	int				fMON,					//	限界控除適用日前月数
					rMON,					//	限界控除適用日後月数
					sMON;					//	平成９年４月以後の月数

	char			kensg, kani1, s_ymd[4], e_ymd[4];
	int				Kobet,					//	1 = 個別対応  0 = 比例配分
					Kazmon;					//	経過月数
	char			Kenuri[6];				//	事業別課税売上高合計
	char			Kenuri_n[6];			//	事業別課税売上高合計(新税率分)
	char			Svuriz[6];				//	課税標準消費税額 (積み上げ処理前)
	char			Svuriz_3[6];			//	課税標準消費税額 (積み上げ処理前)
	char			WORK0[6], WORK1[6], PW0[6], PW1[6], PW6[6], PW7[6], PW8[6];
	struct S_VAL	Asval;
	struct SUK_R	Bsval, SBval;
	struct _STUVAL	stval;
	struct GETBL	Gkai_tb[5];
	struct _STMVAL	Stm;

	// 消費税申告書　出力計算用
	struct	_F1		fh1;					// 付表１及び４用
	struct	_F2		fh2;					// 付表２及び２-(2)用
	struct	_F3		fh3;					// 付表３用
	struct	_F5		fh5;					// 付表５及び５-(2)用
	struct	_SHIN	shn;					// 申告書本表用

	// 棚卸中間バッファクリア 0,1,2は5％　3,4,5は3％
	char			ST_komi[6][6];	
	char			ST_komz[6][6];
	char			ST_nuki[6][6];
	char			ST_nukz[6][6];
	char			ST_nzei[6][6];

	unsigned short	UP1,					//	第１種事業　売上割合
					UP2,					//	第２種事業　売上割合
					UP3,					//	第３種事業　売上割合
					UP4,					//	第４種事業　売上割合
					UP5;					//	第５種事業　売上割合

	char			m_Except[20];			// 除外サイン
	char			m_Ikkatu[20];			// 一括税抜き月情報 アトラス変換後	[07'06.14]

	// 特定収入
	// 消費税集計テーブル
	struct TB_PAR	SpTbl;
	int				m_Specnt;				// 特定収入マスター登録数
	int				m_Class;				// クラスサイン
	int				m_SpcSg;				// 特定収入処理サイン

	// 課税売上割合に準ずる割合を有効サイン
	char			RATIO, RATIOX;

	// 課税売上割合の判定
	char			RatioAsc[6];
	int				RatioInt;
	int				m_TaxReforms;

	int				m_Basic;				// 個人

	char			m_ErroFlg;				// エラーフラグ						
	CString			ErrMessage;				// エラーメッセージ

	char			m_CheckFlg;				// チェックラグ						
	CString			CheckMessage;			// テックメッセージ

	// マスターオープンフラグ
	char			m_OpenFlg1,				// D0: 財務		ボリューム・ラベル
											// D1: 〃　		科目名称
											// D2: 〃		科目残高
											// D3: 〃		月情報
											// D7: 〃		システムデータベース
					m_OpenFlg2;				// D0: 消費税	管理情報
											// D1: 〃		申告書データ	
											// D2: 〃		残高（仕訳集計）
											// D3: 〃		特定収入残高
											// D4: 
	int				m_Stock;				// ON=比例配分及び簡易課税時の科目名称よりの仕入区分の取得
	char			m_Kani,					// 原則課税時の簡易課税計算（単一事業者の業種区分 1,2 - 5）
					m_Dmme;					// リザーブ
	// 消費税額計算用変数
	SUMVAL		HANS[30];					// 本表用
	SUMVAL		WANS[10];					// ワーク用
	SUMVAL		FANS[20];					// 付表用
	SUMDATA		SumData;					// 1表分

	SUMRATIO	SumRatio;					// 課税売上割合計算式パケット
	// 実額保存
	SUMINVENT	SumInvent;					// 棚卸調整(実額)
	SUMMONY		SumMony[4];					// その他の金額(実額)

	char		m_CnvVal[6];				// 課税売上高
	char		m_KzuVal[6];				// 課税売上高（按分格納）
//★//[12'08.01]///
	char		m_KzuValKm[6];				// 課税売上高（税込）
///////////////////

//★//[13'05.31]///
// 輸入仕税額保存
char	KazIp4[6];
char	KazIp3[6];
char	ComIp4[6];
char	ComIp3[6];
///////////////////

	long		m_symd, m_eymd;

public:

///////////////
// 共通関数  //
///////////////
	int ShinKoku_Convert( void );											// 消費税申告書データ変換
	int Shinkoku_Read( void );												// 消費税関係マスターの読み込み
	int	Shinkoku_Calcurate( SUMDATA* sumdata );								// 申告書集計
//★//[12'08.01]///
	int	Shinkoku_CalcurateEX( SUMDATA* sumdata );								// 申告書集計　免税点用拡張版
///////////////////
	void SumilateConvert( SUMDATA* sumdata );								// 課税方式比較表のパケットへデータを転送
	int TaxReformsCheck( void );											// ９５％ルール改正対応期間チェック

	void stm_new( char );													// 申告書マスター用　計算
	void fh1_set( char );													// 付表１ or 付表四内容セット
	void fh2_set( void );													// 付表２ or ２-(2)内容セット
	void fhbf_clr( char, char );											// 付表５用計算バッファクリア
	int fh5_set( char );													// 別表５の 内容計算
	int fh3_set( void );													// 付表３の 内容計算
	void stm_calq( void );													// 申告書本表　内容計算
	int stm_old( unsigned char );											// 申告書マスタ－用　計算

//★//[11'09.27]カット
//	int SyzShinYearsCmp( char * );											// 平成で決算期間チェック
//	int EntryGetKazei( void );												// 課税期間の取得
//	int EntrySkipKikan( void );												// 課税期間・中間期間の判定
//	int EntryCmpKazei( void );												// 課税期間での期首・期末比較
//	int EntryCmpTyukn( void );												// 中間期間での期首・期末比較	
//	int CheckKzeiUri( void );												// 課税売上の金額チェック
///////////////////			

#ifdef	VERSIONUP
	void SPassOffCalq( void );												// 特定収入に係る調整税額計算
	void SPassOnCalq( void );												// 特定収入に係る調整税額計算
#endif

	int YearConvert( char *, char * );										// 課税売上高５億円チェック	

//////////////////
// 改正チェック //
//////////////////
	int CheckDBShin( int, long, long, CDBZmSub* );							// 財務クラス
	int CheckNPShin( int, long, long, CDBNpSub* );							// 公益クラス
	CString& ErrMsgCheckShin( void );										// エラーメッセージの取得
	CString& GetMsgCheckShin( void );										// チェックメッセージの取得

	int CheckShinkoku( void );												// チェック開始

	int CheckShinkokuH26();													// 平成26年4月改正対応

//////////////////////
// 消費税申告書集計 //
//////////////////////
	int	CalculateDBShin( long, long, SUMINFO*, SUMLIST*, CDBZmSub* );		// 財務 消費税申告書集計
	int	CalculateNPShin( long, long, SUMINFO*, SUMLIST*, CDBNpSub* );		// 公益 消費税申告書集計
	CString& ErrMsgCalculateShin( void );									// エラーメッセージの取得

	int CalculateShinkoku( SUMINFO*, SUMLIST* );							// 消費税 集計開始

//////////////////
// その他の関数 //
//////////////////
public:

	void date_to_bcd( char, unsigned char *, long );						// 日付をＢＩＮからＢＣＤに変換 
	void asci_to_bcd( unsigned char *, CString *, int );					// 文字列をBCDにパック
	void val_to_bin( unsigned char *, CString );							// 金額変換（文字列よりバイナリーに変換）
	void cstring_to_char( unsigned char *, CString, int );					// 文字列転送（CString　から　Char へ）
	void date_to_bin( int, long *, unsigned char * );						// 日付をＢＣＤからＢＩＮに変換
	void val_to_asci( CString *, unsigned char * );							// 金額変換（バイナリーより文字列に変換）

	int shin_datecnv( unsigned char, unsigned char *, int );				// 平成～西暦　変換
	short shin_cmpymd( char *, char * );									// 年月日の比較
	short shin_Feb( short );												// 閏年計算用モジュール
	short shin_uday( char * );												// 閏年計算
	short shin_mday( char, char * );										// 期末月計算
	short shin_iniz( struct	_VOL1 *, struct _VOL2 * );						// 期間処理モジュール	初期設定
	short shin_mdchk( char, char, char *, char *, char * );					// 月日よりデータ期間チェック
	short shin_ofst( unsigned char *, int *, struct _VOL1 *, struct _VOL2 * );	// 月日よりオフセット返送	
	short shin_date( unsigned char *, unsigned char *, unsigned char *, struct _VOL1 *, struct _VOL2 * );	// 月日よりデータ期間 返送
	short shin_sign( char *, char *, struct _VOL1 *, struct _VOL2 * );			// 月日よりデータ生成 ＆ 月日チェック
	int	shin_dateset( unsigned char *, unsigned char *, unsigned char, DAYPAC * );	// 決算期間より各月データ期間計算

	int KnTablRead( void );													// 科目名称読み込み
	char StockSignGet( char *, char, char, char );							// 仕入区分対象科目チェック
	char SalesSignGet( char *, char, char, char );							// 売上区分対象科目チェック

	void ZaimuVolume_Cnv( void );											// 財務ボリュームラベル情報の変換
	void SyzVolume_Cnv( void );												// 消費税ボリュームラベルの変換
	void SyzShin_Cnv( void );												// 消費税申告書エリアデータ変換
	void SyzShin_Make( void );												// 消費税申告書ファイルの作成（登録）

	int toku75( char *, char *, char *  );									// 簡易課税　2種類以上の業種での特例計算チェック
	int formmake( int, int, struct KARI *, struct _SZREC *, struct _SZREC * );		// 財務マスタ－集計
	void formmake3( int, int, struct KARI *, struct _SZREC *, struct _SZREC * );	// 経過措置税率	仮払・仮受消費税　集計
	void StackMake( int, int, struct KARI *, struct _SZREC * );				// 棚卸調整税額を科目残より差引く
	int for_clear( struct TB_PAR * );										// 集計レコ－ドクリア－
	int key_set( struct TB_PAR *, unsigned char, unsigned char, unsigned char, unsigned char );	// キー設定
	int stm_ucalq( int, int, char *, char *, char *, char *, struct TB_PAR *, struct TB_PAR * );	// 通常集計
	int skb_calq( int, int, char *, char *, char *, char * );				// 個別対応税額計算（簡易計算のみ）
	int stm_scalq( int, int, char *, char *, char *, char *, struct TB_PAR *, struct TB_PAR * );	// 簡易計算
	int gen_chk( int );														// 限界控除制度　適用期間チェック
	void gencalq( char *, char * );											// 改正法　限界控除計算
	void so_read( void );													// 消費税申告書用  旧 WORK AREA 転送
	void so_close( void );													// 消費税申告書用  旧 WORK AREA WRITE
	void sn_read( void );													// 申告書用データ　ＲＥＡＤ
	void sn_close( void );													// 申告書用データ　ＷＲＩＴＥ

	int	SpRead( void );														// 特定収入集計テ－ブルの作成＆読み
	int IkTaxZanRead( void );												// 一括税抜時の仮受・仮払消費税額の取得
	int	ofs_get( void );													// 入力年月　開始・最終設定
	int	forcreat( struct TB_PAR *, struct TB_PAR *,  struct TB_PAR * );		// 集計テ－ブル作成
	int rui_read( struct TB_PAR *,  struct TB_PAR *,  struct TB_PAR * );	// 累積デ－タ リ－ド
	int	UriShiireGet( struct _SZREC	* );									// 科目属性より仮受消費税及び、仮払消費税の取得
	int suk_read( struct _H9SUM *, struct TB_PAR * );						// 課税仕入区分・売上区分集計エリアのリード
	void Stack_Clear( struct _SZREC * );									// 棚卸調整データの取得エリアのクリア
	int Stack_Stock( struct _SZREC *, struct _SZREC * );					// 棚卸調整データ集計
	int Import_Stock( struct _H9SUM *, struct _SZREC * );					// 輸入仕入データ集計
	void l_calq( char *, char *, char );									// 売上区分／仕入区分データ集計
	void l_6calq( char *, char *, char *, char * );							// ６倍長　集計　モジュ－ル
	void percent( char *, char *, int, char, char );						// 税率計算
	void p_calq( char *, char*, char );										// ６倍長　切り捨て　モジュ－ル
	int is5pertbl( void );													// 決算期間内 消費税５％改正オフセット
	int issymst( void );													// 消費税マスターチェック
	int iskanim( void );													// 簡易課税マスターチェック
	int iskobetm( void );													// 個別対応マスタ－チェック
	int isikkatm( void );													// 一括税抜きマスタ－チェック
	int ukbngt( char selsgn[] );											// 売上業種区分取得
	int is5perm( void );													// 決算期間内 消費税５％改正 チェック
	int Kubun_Stock( struct _SZREC *, struct TB_PAR * );					// 売上区分／仕入区分データ集計
	int skchek( int, int );													// 集計済みチェック
	void IsManagement(  struct _SZREC * );									// 税抜・税込・一括経理処理時の込み・抜きの生成			
	int IsTaxDivide( char, char );											// 税抜経理処理時の区分毎の税込み・税抜きの取得
	int isVerchk( char * );													// 消費税マスターバージョンチェック
	int Establish( void );													// 設立時の最終月のオフセットの取得

#ifdef	_CLOSE
	int OfficeRead( void );										// 事務所名の取得
	int MixSpaceCutLength( char *, int );						// 漢字文字数　検索
	int retsei( int );											// 西暦変換（登録・更新）
	int	nen( int );												// 閏年（登録・更新）
	int matu_chk_Entry( int, int, int );						// 末日チェック  登録
	void year_conv( char, unsigned char *, long * );			// 年度変換
	void date_to_bcd( char, unsigned char *, long );			// 日付をＢＩＮからＢＣＤに変換 
	void date_to_bin( int, long *, unsigned char * );			// 日付をＢＣＤからＢＩＮに変換
	void val_to_asci( CString *, unsigned char * );				// 金額変換（バイナリーより文字列に変換）
	void val_to_bin( unsigned char *, CString );				// 金額変換（文字列よりバイナリーに変換）
	void asci_to_bcd( unsigned char *, CString *, int );		// 文字列をBCDにパック
	void bcd_to_asci( CString *, unsigned char *, int );		// BCDを文字列に変換
	void cstring_to_char( unsigned char *, CString, int );		// 文字列転送（CString　から　Char へ）
	void char_to_cstring( CString *, unsigned char *, int );	// 文字列転送（Char　から　CString へ）
	int BasPerideTaxSold( int );								// 基準期間の課税売上高チェック
	int numlen( unsigned char *, int );							// カナ検索
	void val_to_asci( CString *, unsigned char * );				// 金額変換（バイナリーより文字列に変換）
	short shin_cmpymd( char *, char * );						// 年月日の比較
	short shin_Feb( short );									// 閏年計算用モジュール
	short shin_uday( char * );									// 閏年計算
	short shin_mday( char, char * );							// 期末月計算
	short shin_iniz( struct	_VOL1 *, struct _VOL2 * );			// 期間処理モジュール	初期設定
	short shin_mdchk( char, char, char *, char *, char * );		// 月日よりデータ期間チェック
	short shin_ofst( unsigned char *, int *, struct _VOL1 *, struct _VOL2 * );	// 月日よりオフセット返送	
	short shin_date( unsigned char *, unsigned char *, unsigned char *, struct _VOL1 *, struct _VOL2 * );	// 月日よりデータ期間 返送
	short shin_sign( char *, char *, struct _VOL1 *, struct _VOL2 * );			// 月日よりデータ生成 ＆ 月日チェック
	int	shin_dateset( unsigned char *, unsigned char *, unsigned char, DAYPAC * );	// 決算期間より各月データ期間計算
	void ZaimuVolume_Cnv( void );								// 財務ボリュームラベル情報の変換
	void SyzVolume_Cnv( void );									// 消費税ボリュームラベルの変換
	void SyzShin_Cnv( void );									// 消費税申告書エリアデータ変換
//	void SyzSkj_Cnv( void );									// 仕入控除明細データ変換
//	void SyzShin_Make( void );									// 消費税申告書ファイルの作成（登録）
	int read_tbl( int );										// 消費税申告書エリアデータ読み込み
	int skchek( int, int );										// 集計済みチェック
	int	ofs_get( void );										// 入力年月　開始・最終設定			
	int ofs_get_Next( void );									// 入力年月　開始・最終設定
	int chk_dsp( int );											// 消費税申告書　処理状況警告
//	int SyzShin_UpDate( void );									// 消費税申告書情報の書き込み	
//	int	CDBSyzShinView::AllUpData( void );						// 全データの更新 [09'04.01]

	int	forcreat( struct TB_PAR *, struct TB_PAR *, struct TB_PAR * );	// 集計テ－ブル作成
	int rui_read( struct TB_PAR *, struct TB_PAR *, struct TB_PAR * );	// 累積デ－タ リ－ド
	int suk_read( struct _H9SUM *, struct TB_PAR * );			// 課税仕入区分・売上区分集計エリアのリード
	void Stack_Clear( struct _SZREC * );						// 棚卸調整データの取得エリアのクリア
	int Stack_Stock( struct _SZREC *, struct _SZREC * );		// 棚卸調整データ集計
	int Import_Stock( struct _H9SUM *, struct _SZREC * );		// 輸入仕入データ集計
	int Kubun_Stock( struct _SZREC *, struct TB_PAR * );		// 売上区分／仕入区分データ集計
	void l_calq( char *, char *, char );						// ６倍長　切り捨て　モジュ－ル
	void l_6calq( char *, char *, char *, char * );				// ６倍長　集計　モジュ－ル
	void percent( char *, char *, int, char, char );			// 税率計算
	int	UriShiireGet( struct _SZREC	* );						// 科目属性より仮受消費税及び、仮払消費税の取得	SZsjsg
	void IsManagement( struct _SZREC * );						// 税抜・税込・一括経理処理時の込み・抜きの生成
	int isVerchk( char * );										// 消費税マスターバージョンチェック
	int issymst( void );										// 消費税マスターチェック
	int iskanim( void );										// 簡易課税マスターチェック
	int iskobetm( void );										// 個別対応マスタ－チェック
	int isikkatm( void );										// 一括税抜きマスタ－チェック
	int ukbngt( char selsgn[] );								// 売上業種区分取得
	int is5perm( void );										// 決算期間内 消費税５％改正 チェック
	int is5pertbl( void );										// 決算期間内 消費税５％改正オフセット

	int SyzShinEntry( void );									// 消費税申告書　処理準備
	void SyzShinEntryIniz( void );								// 登録初期画面
	void perinp( short, short, char *, short * );				// 整数（小数点含む）入力

	int toku75( char *, char *, char *  );						// 簡易課税　2種類以上の業種での特例計算チェック
	int formmake( int, int, struct KARI *, struct _SZREC *, struct _SZREC * );		// 財務マスタ－集計
	void formmake3( int, int, struct KARI *, struct _SZREC *, struct _SZREC * );	// 経過措置税率	仮払・仮受消費税　集計
	void StackMake( int, int, struct KARI *, struct _SZREC * );	// 棚卸調整税額を科目残より差引く
	int for_clear( struct TB_PAR * );							// 集計レコ－ドクリア－
	int key_set( struct TB_PAR *, unsigned char, unsigned char, unsigned char, unsigned char );	// キー設定
	int stm_ucalq( int, int, char *, char *, char *, char *, struct TB_PAR *, struct TB_PAR * );	// 通常集計
	int skb_calq( int, int, char *, char *, char *, char * );	// 個別対応税額計算（簡易計算のみ）
	int stm_scalq( int, int, char *, char *, char *, char *, struct TB_PAR *, struct TB_PAR * );	// 簡易計算
	int gen_chk( int );											// 限界控除制度　適用期間チェック
	void gencalq( char *, char * );								// 改正法　限界控除計算
	void so_read( void );										// 消費税申告書用  旧 WORK AREA 転送
	void so_close( void );										// 消費税申告書用  旧 WORK AREA WRITE
	void sn_read( void );										// 申告書用データ　ＲＥＡＤ
	void sn_close( void );										// 申告書用データ　ＷＲＩＴＥ

	void stm_new( char );										// 申告書マスター用　計算
	void fh1_set( char );										// 付表１ or 付表四内容セット
	void fh2_set( void );										// 付表２ or ２-(2)内容セット
	void fhbf_clr( char, char );								// 付表５用計算バッファクリア
	int fh5_set( char );										// 別表５の 内容計算
	int fh3_set( void );										// 付表３の 内容計算
	void stm_calq( void );										// 申告書本表　内容計算
	int stm_old( unsigned char );								// 申告書マスタ－用　計算

	int ShinCalq( void );										// 申告書集計

	int SyzShinYearsCmp( char * );								// 平成で決算期間チェック
	int EntryGetKazei( void );									// 課税期間の取得
	int EntrySkipKikan( void );									// 課税期間・中間期間の判定
	int EntryCmpKazei( void );									// 課税期間での期首・期末比較
	int EntryCmpTyukn( void );									// 中間期間での期首・期末比較	
	int EntryCheck( int );										// 初期画面　入力項目チェック
	int CheckKzeiUri( void );									// 課税売上の金額チェック
//	int SkjConnecteed( struct TB_PAR *, int, int );				// 仕入控除税額に関する明細書の財務連動
	int GetKnrecName( char *, char * );							// 科目名称の取得
	void AscToBcd( char*, char*, int );							// 変換 ( 文字列→BCD )

	// 税抜経理処理時の区分毎の税込み・税抜きの取得
	int IsTaxDivide( char, char );
	// カナを漢字に変換
	void AtoJis( char *, char *, int );

	// 一括税抜時の仮受・仮払消費税額の取得
	int IkTaxZanRead();

	// 特定収入に係る調整税額計算
	void SPassOffCalqEx( void );
	// 特定収入に係る調整税額計算
	void SPassOnCalqEx( void );
	// 漢字文字列検索 [08'12.01]
	int	MojiSearch(  char *, char *, int );

	// 設立時の最終月のオフセットの取得
	int Establish( void );

	// 中間納付税額の金額チェック
	int MiddleTaxCheck( void ) ;

	// 科目名称読み込み
	int KnTablRead( void );

	// 現在の選択マスターの過年度チェック 
	int IsKanendoCheck( void );
#endif

	char	m_Save1[512];

private:

	char	m_Save2[256];
};

#endif			// end of __ICS_SHINCALCULATEW__
