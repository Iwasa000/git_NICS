#pragma	once

//== 定数 ==
const	int	SH_MONTH_MAX		=	16;			// 月数

/*const	int	PER3_SYZ_YMD		=	19890401;	// 消費税：３％施行日
const	int	PER5_SYZ_YMD		=	19970401;	// 消費税：５％施行日
const	int	PER8_SYZ_YMD		=	20140401;	// 消費税：８％施行日
const	int	PER10_SYZ_YMD		=	20151001;	// 消費税：１０％施行日

const	int	H24_TAXREFORM_YMD	=	20120401;	// 税制改正施行日：95%ルール
*/
//static	char	AMENDK[] = { 0x23, 0x01, 0x01, 0 };	// （個人）平成23年より前の場合に許可
//static	char	AMENDH[] = { 0x23, 0x10, 0x02, 0 };	// （法人）平成23年10月2日より前の場合に許可
const	int	PERMIT_KJ_AMEND_YMD	=	20110101;	// 更正の申出書（個人）平成23年より前の場合に許可
const	int	PERMIT_HJ_AMEND_YMD	=	20111002;	// 更正の申出書（法人）平成23年10月2日より前の場合に許可

const	int BSINFO_VIEW_WND_IDX		=	0;	// 基本情報
const	int	H31_HONPYO_WND_IDX		=	1;	// 平成31年度版：本表・付表
const	int	H26_HONPYO_WND_IDX		=	2;	// 平成26年度版：本表・付表
const	int	H26_KZTORIHIKI_WND_IDX	=	3;	// 平成26年度版：課税取引・特定収入
const	int	H31_KZTORIHIKI_WND_IDX	=	4;	// 平成31年度版：課税取引・特定収入


static int	LASTDAY_EVMONTH[2][13] = {			// 各月の末日
			{ 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
			{ 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }	};

#define		REPLY_ERR		0x00000001
#define		REPLY_OK		0x00000002
#define		REPLY_NEW		0x00000004
#define		REPLY_NEXT		0x00010000
#define		REPLY_BACK		0x00020000
#define		REPLY_STAY		0x00040000
#define		REPLY_MOVE		0x00080000
#define		REPLY_DELT		0x80000000

#define		BC_WHITE		RGB(255,255,255)		// 入力不可	（白色）
#define		BC_CREAM		RGB(211,245,188)		// 入力可	（クリーム色）
#define		BC_GRAY			RGB(192,192,192)		// ロック	（灰色）
#define		BC_BLUE			RGB(160,255,255)		// 特殊①	（青）
#define		BC_GREEN		RGB(132,255,193)		// 特殊②	（緑）
#define		BC_GREEN_L		RGB(190,243,199)		// 特殊③	（薄緑）
#define		BC_CYAN			RGB(128,255,255)		// 特殊④	（水色）	// 修正No.168720 add

// 一括印刷の状態
typedef enum {
	ID_ORIGINALSTART_BPSTATE	=	0,	// 通常起動
	ID_ESTABLISH_BPSTATE,				// 詳細設定
	ID_NORMALPRINT_BPSTATE,				// 一括印刷
	ID_PDFPRINT_BPSTATE					// CDR出力
} EnumIdShBatchPrintState;

// 出力指定の状態
typedef enum {
	ID_SEP_PRINT_SEL_STATE		=	0,	// 絞り込み出力
	ID_ALL_PRINT_SEL_STATE				// 全表出力
} EnumIdPrintSelState;


//== 構造体 ==
/*
//struct	_PRMOVE	{
typedef struct	_tag_ICS_SH_PRMOVE_ {
	char	_PSign,		// 出力帳表　番号
			_PDmme;		// リザーブ
	short	_PIVad,		// 版下　縦方向調整量　1/100mm 
			_PIHad,		// 〃　　横方向調整量　〃 
			_PKVad,		// ＫＳＫ縦方向調整量　1/100mm 
			_PKHad;		// 〃　　横方向調整量　〃
	char	_PSave[6];
} SH_PRMOVE;

// 微調整用テーブル
//struct	_MvTbl	{
typedef struct _tag_ICS_SH_MvTbl_ {
	short	HyoCn,
			HyoTn;
	char	HyoMs[20];
} SH_MvTbl;
*/
/*
// 仕入控除税額に関する明細書の連動テーブル
//typedef struct	_SKJ_REC	{
typedef struct	_tag_ICS_SH_SKJ_REC_ {
			 char	SZan[6],	// 科目残高
					SKaz[6],	// 課税金額
					SKz5[6],	// 課税金額５％《抜き又は込み》簡易課税専用
					SKz3[6];	// 課税金額３％		〃				〃
//} SKJREC;
} SH_SKJ_REC;

//typedef	struct	_SKJ_CNV	{
typedef	struct	_tag_ICS_SH_SKJ_CNV_ {
	unsigned	char	SKJCod[10];		// 内部コード
				int		SKJSeq,			// シーケンス番号
						HJNCod,			// 法人用集計項目番号
						KJNCod;			// 個人用集計項目番号
//} SKJCNV;
} SH_SKJ_CNV;
*/

//== 印刷情報 ==
/*
typedef struct	_tag_ICS_SH_PRINT_CMINFO_ {
	char	PRtype;			// D0：申告書本表出力
							// D1：付表１又は４出力	
							// D2：付表２又は５出力
							// D3：付表２-(2)又は５-(2)出力
							// D4：付表３出力
							// D7：版下出力
	char	PRtable[50];	// 仕入控除税額に関する明細書（次葉）	←　本当？？？
	int		isColorPrn;		// モノクロ＆カラーサイン				←　m_isColorPrn
	int		PgMx;			// 出力枚数サイン（複写は含まない）
} SH_PRINT_CMINFO;
*/

// 一括印刷の情報
//typedef	struct	_tag_ICS_SH_BPPRINT_REC_ {
//	char	requestkey[128];	// 依頼キー
//} SH_BPPRINT_REC;


//== 文字列 ==
// 情報の保存
const	TCHAR	OWNTBL_SQL[]			=	_T("apl_name = '%s' and itm_name = '%s'");
const	TCHAR	SKJ_OWNTBL_SQL[]		=	_T("apl_name = '%s' and itm_name = '%s' and u_id=%d");
const	TCHAR	TKNEW_OWNTBL_SQL[]		=	_T("apl_name = '%s' and itm_name = '%s' and itm_seq=%d");	// 24/02/27wd_特定収入対応 add
const	TCHAR	SKJ_ITMNAME[]			=	_T("SKJ_SETUP");
const	TCHAR	SKJ52_ITMNAME[]			=	_T("SKJ52_SETUP");	// 新社会福祉対応
/*- '14.04.28 -*/
//const	TCHAR	PAST_PROGRAM_PATH[]		=	_T("%s:\\ICSWin\\CMDS\\GR1b\\JOBS30\\H25\\DBSyzShin.exe");
/*-------------*/
const	TCHAR	PAST_PROGRAM_PATH[]		=	_T("%s\\H25\\DBSyzShin.exe");
/*-------------*/

// 保存情報カラム名
const	TCHAR	DBSYZSHIN_APLNAME[]		=	_T("DBSYZSHIN");
const	TCHAR	DBSYZSHIN_ITMNAME[]		=	_T("DBSYZSHIN_SETUP");
const	TCHAR	DBSYZSHIN_ITMNAME_SUB[]	=	_T("DBSYZSHIN_SETUP_SUB");
const	TCHAR	DBSYZBASIC_APLNAME[]	=	_T("DBSYZBASIC");
const	TCHAR	DBSYZBASIC_ITMNAME[]	=	_T("DBSYZBASIC_SETUP");

//shimizu追加 還付明細書関係

#define LINE_OFS	420
#define	COLM_OFS	255
#define	PLIST_NLINE	60

//

//総合会社情報用、構造体作成
typedef struct _NOWCOINF{
	unsigned char	Sn_SEIRI[4],		// 整理番号　　　　　　［ＢＣＤ８桁］		szsnk->Sn_SEIRI
					Sn_ZMSYO[12],		// 税務署名　　　　　　［漢字６文字］		szsnk->Sn_ZMSYO
					Sn_TAXNO[5],		// 税務署コード
					Sn_ZIP[10],			// 郵便番号			　 ［ＡＳＣＩＩ８桁］	szsnk->Sn_ZIP
					Sn_NOZEI[40*2],		// 納税地　　　　　　　［漢字２０＊２文字］	szsnk->Sn_NOZEI
					Sn_TELNO1[6],		// 電話番号　　　　　　［ＡＳＣＩＩ１６桁］	szsnk->Sn_TELNO を分解
					Sn_TELNO2[4],		// 電話番号　　　　　　［ＡＳＣＩＩ１６桁］	szsnk->Sn_TELNO を分解
					Sn_TELNO3[4],		// 電話番号　　　　　　［ＡＳＣＩＩ１６桁］	szsnk->Sn_TELNO を分解
					Sn_DIHYO[40],		// 代表者氏名　　　　　［漢字２０文字］		szsnk->Sn_DIHYO
					Sn_DIFRI[80],		// 代表者氏名振り仮名　［漢字半角２０文字］	szsnk->Sn_DIFRI
					Sn_CONAM[40],		// 名称又は，屋号　　　［漢字２０文字］		szsnk->Sn_CONAM
					Sn_COFRI[80],		// 名称又は屋号振り仮名［漢字半角２０文字］	szsnk->Sn_COFRI
					Sn_KEIRI[40],		// 経理担当者氏名　　　［漢字２０文字］		szsnk->Sn_KEIRI
					Sn_YUBIN[16],		// 郵便局名　　　　　　［漢字６文字］			szsnk->Sn_YUBIN
					Sn_YUNUM1[5],		// 郵便貯金の記号番号１(5ｹﾀ)					szsnk->Sn_YUNUM1
					Sn_YUNUM2[8],		// 郵便貯金の記号番号２(8ｹﾀ)					szsnk->Sn_YUNUM2
					Sn_BANK[20],		// 銀行名称　　　　　　［漢字６文字］			szsnk->Sn_BANK
					Sn_SITEN[20],		// 支店名称　　　　　　［漢字６文字］			szsnk->Sn_SITEN
					Sn_YOKIN[8],		// 預金種別　　　　　　［漢字４文字］			szsnk->Sn_YOKIN
					Sn_KOZA[5],			// 口座番号　　　　　　［ＢＣＤ10桁］			szsnk->Sn_KOZA
					Sn_FINANCE,			// 0:無し 1:銀行 2:金庫 3:組合 4:農協 5:漁協	szsnk->Sn_FINANCE
					Sn_CHAIN,			// 0:無し 1:本店 2:支店 3:本社 4:支社			szsnk->Sn_CHAIN
					Sn_DIADR[40*2],		// 代表者住所　　　　　［漢字２０＊２文字］	szsnk->Sn_DIADR
					Sn_DIZIP[10],		// 代表者郵便番号　　　［ＡＳＣＩＩ８桁］	szsnk->Sn_ADRD4[5]
					Sn_DITELNO1[6],		// 代表者電話番号　　　［ＡＳＣＩＩ１６桁］	szsnk->Sn_ADRD4[6]を分解
					Sn_DITELNO2[4],		// 代表者電話番号　　　［ＡＳＣＩＩ１６桁］	szsnk->Sn_ADRD4[6]を分解
					Sn_DITELNO3[4];		// 代表者電話番号　　　［ＡＳＣＩＩ１６桁］	szsnk->Sn_ADRD4[6]を分解
}NOWCOINF;

// マイナンバー関連情報保存用
typedef struct	_ICS_SYZ_MYNO_SVINF_ {
	int				kubun;				// 法人・個人区分
	int				sn_seq;				// 履歴シーケンス
	unsigned char	apno,				// アプリケーションNo.
					dumy[15];			// 
} SYZ_MYNO_SVINF;

// 過年度マスター情報保存用
typedef struct	_ICS_SYZ_ZEN_MASTERINF_ {
	char	server[256];	// サーバー名
	int		isExist;		// 1:存在する,0:存在しない
	int		isCalqed;		// 1:計算済,0:計算未了
	int		isNotZero;		// 1:消費税額が０以外,0:消費税額が０
	int		nApno;			// アプリケーションNo.
	int		nCode;			// 会社コード
	int		nYmd;			// 期末年月日
	char	dumy[40];
} ZEN_MASTERINF;

static const int DBSUBSW_KAKUTEI = 0x200000;	// 所得税ｄｂ（確定申告書）
static const int DBSUBSW_KOKETSU = 0x400000;	// 所得税ｄｂ（個人決算書）