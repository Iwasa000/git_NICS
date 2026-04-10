#pragma once

//=============================================================================
// 列挙子
//=============================================================================

// SQL文字列変換ルール
typedef enum {
	ID_ICSSYZ_NUMBER_SQLTYPE	=	0,	// 数値
	ID_ICSSYZ_STRING_SQLTYPE,			// 文字列
	ID_ICSSYZ_NUMSTR_SQLTYPE,			// 数値文字
	ID_ICSSYZ_DATE_SQLTYPE,				// 日付（BCD->INT）
	ID_ICSSYZ_NULL_SQLTYPE				// NULL データ
} EnumIdIcsSyzCnvSqlType;

// 状態チェック時の呼び出しタイプ
typedef enum {
	ID_ICSSYZ_ZAIMU_CHK_CALLTYPE	=	0,	// 財務からの呼び出し
	ID_ICSSYZ_ZEIMU_CHK_CALLTYPE			// 税務からの呼び出し
} EnumIdIcsSyzChkCallType;

// 変換タイプ
typedef enum {
	IC_ICSSYZ_START_JCYTYPE	=	0,	// 開始
	IC_ICSSYZ_END_JCYTYPE			// 終了
} EnumIdJcyCnvType;

// 仕訳変換タイプ（税率変換）
typedef enum {
	IC_ICSSYZ_5_TO_8_CNV	=	0,	// 5%->8%
	IC_ICSSYZ_8_TO_10_CNV			// 8%->10%
} EnumIdDataCnvType;

// 税率対応
typedef enum {
	ID_ICSSYZ_8PER_TYPE	=	0,		// 8%
	ID_ICSSYZ_10PER_TYPE,			// 10%
	ID_ICSSYZ_RD8PER_TYPE			// 軽減8%
} EmumIdZeirituType;

//=============================================================================
// 定数
//=============================================================================

const	int		ALL_TS_YMD			=	20000101;			// 全マスター対象年月日（開始）
const	int		NCM_YMD				=	20060501;			// 新会社法施行年月日
const	int		KH_TS_YMD			=	20060401;			// 公益法人対象年月日（開始）
const	int		KH3_TS_YMD			=	20081201;			// 公益法人Ⅲ対象年月日（開始）
const	int		SFH_TS_YMD			=	20000401;			// 社会福祉法人対象年月日（開始）
const	int		SFH_SKTS_YMD		=	20120401;			// 社会福祉法人新会計基準対象年月日（開始）
const	int		PER8_SYZ_YMD		=	20140401;			// 消費税：８％施行日
const	int		BF_PER10_LST_YMD	=	20190930;			// 消費税：１０％施行日前 最終日
const	int		PER10_SYZ_YMD		=	20191001;			// 消費税：１０％施行日
const	int		BF_INVOICE_LST_YMD	=	20230930;			// 消費税：インボイス施行日前 最終日
const	int		INVOICE_SYZ_YMD		=	20231001;			// 消費税：インボイス施行日

const	int		HOJIN_APNO			=	0x00;				// 法人
const	int		UNSO_APNO			=	0x01;				// 運送
const	int		KOJIN_APNO			=	0x10;				// 個人
const	int		KENSETU_APNO		=	0x20;				// 建設
const	int		BYOIN_APNO			=	0x30;				// 病院
const	int		KOEKI_APNO			=	0x50;				// 公益
const	int		KOEKI3_APNO			=	0x51;				// 公益Ⅲ
const	int		SYAKAI_APNO			=	0x52;				// 社会福祉
const	int		GAKKO_APNO			=	0x58;				// 学校
const	int		SYUKYO_APNO			=	0x59;				// 宗教
const	int		ZAIMUEND_APNO		=	0x5F;				// 最終財務領域
const	int		ZEIMUST_APNO		=	0x60;				// 開始税務領域

const	int 	CLMMAX_ZVOLUME		=	81;					// zvolume のカラム最大数
const	int 	CLMMAX_BVOLUME		=	48;					// bmvolume のカラム最大数
const	int 	CLMMAX_MINFO		=	26;					// month_info のカラム最大数
const	int		CLMMAX_ZMINFO		=	11;					// zmonth_info のカラム最大数

const int		MONTHCNT			=	12;					// 通常月
const int		ACCOUNTS			=	4;					// 決算修正月
const int		TSCOUNT				=	MONTHCNT+ACCOUNTS;	// 当期月数（１６ヶ月）
const int		TNSCOUNT			=	3;					// 翌期月数
const int		MONTHALL			=	TSCOUNT+TNSCOUNT;	// 当期＋翌期（１９ヶ月）

const	int		MVER_11				=	0x11;				// 初期バージョン
const	int		MVER_12				=	0x11;				// 青色申告決算書対応バージョン
const	int		NOW_MVER			=	MVER_12;			// 現在のバージョン

// zvol.s_ver にリンクしているバージョン
const	int		PER5_SYZ_VER		=	16;					// 消費税バージョン（5%）
const	int		PER8_SYZ_VER		=	17;					// 消費税バージョン（8%）
const	int		PER10_SYZ_VER		=	18;					// 消費税バージョン（10%）
//const	int		VALID_SYZ_VER		=	PER8_SYZ_VER;		// 消費税バージョン（有効バージョン）
const	int		VALID_SYZ_VER		=	PER10_SYZ_VER;		// 消費税バージョン（有効バージョン）

//const	int		VALID_MKDLL_VER		=	PER5_SYZ_VER;		// 消費税バージョン（DLL作成有効バージョン）
// '13.12.10 消費税有効バージョンを『PER8_SYZ_VER』にアップ
//const	int		VALID_MKDLL_VER		=	PER8_SYZ_VER;		// 消費税バージョン（DLL作成有効バージョン）
// '19.06.03 消費税有効バージョンを『PER10_SYZ_VER』にアップ
const	int		VALID_MKDLL_VER		=	PER10_SYZ_VER;		// 消費税バージョン（DLL作成有効バージョン）

const	BYTE	DEFAULT_SYZ_KNSGN	=	0x00;				// デフォルト税率サイン	knrec->knsgn[3]の上位4ビットに連動
const	BYTE	PER3_SYZ_KNSGN		=	0x10;				// 3%
const	BYTE	PER0_SYZ_KNSGN		=	0x20;				// 0%
const	BYTE	PER5_SYZ_KNSGN		=	0x30;				// 5%
const	BYTE	PER8_SYZ_KNSGN		=	0x40;				// 8%
const	BYTE	PER_RD8_SYZ_KNSGN	=	0x50;				// 8%（軽減税率）
const	BYTE	PER10_SYZ_KNSGN		=	0x60;				// 10%

const	int 	AM1_OFFSET			=	3;					// 第１四半期
const	int 	IAM_OFFSET			=	7;					// 中間決算
const	int 	AM3_OFFSET			=	11;					// 第３四半期
const	int 	TAM_OFFSET			=	15;					// 最終決算
const	int 	TAM2_OFFSET			=	14;					// 最終決算（決算修正無し）

//== 消費税ボリュームラベルバージョン(szvol) ==
const	int		PER8_SYZ_SZVOL_VER	=	18;					// 消費税率８％対応
const	int		KANI6_SYZ_SZVOL_VER	=	19;					// 簡易課税第６種対応('14.12.12)
const	int		PER10_SYZ_SZVOL_VER	=	20;					// 消費税率10％対応('19.02.22)

//=============================================================================
// 文字列
//=============================================================================
static char*	INSERT_TARGET_TABLE[] = {
	_T("knrec"),
	_T("gkrec"),
	_T("kzrec"),
	_T("kzysrec"),
	_T("tkrec"),
	_T("rtrec"),
	_T("month_info"),
	_T("skzan"),
	_T("zvolume"),
	_T("zmonth_info"),
	_T("bmvol"),
	_T("bmkname"),
	_T("szvol"),
	_T("dcntl"),
	_T("hyjrec"),
	_T("kshctl"),
	_T("kshnam"),
	_T("kshrec"),
	_T("vtbl"),
	_T("anaclq"),
	_T("anahyo"),
	_T("anahyoitm"),
	_T("anaitm"),
	_T("anasyk"),
	_T("kprf")
//	"kshctl","kshnam","kshrec","vtbl","anaclq","anahyo","anahyoitm","anaitm","anasyk"
};
const int INSERT_TABLE_MAX = (sizeof(INSERT_TARGET_TABLE)/sizeof(char*));

static char*	INSERT_TARGET_TABLE_NP[] = {
	_T("knrec"),
	_T("gkrec"),
	_T("kzrec"),
	_T("tkrec"),
	_T("rtrec"),
	_T("month_info"),
	_T("zvolume"),
	_T("zmonth_info"),
	_T("bmvol"),
	_T("bmkname"),
	_T("szvol"),
	_T("dcntl"),
	_T("vtbl"),
	_T("gsrec"),
	_T("ksrec"),
	_T("kszrec"),
	_T("npknrec"),
	_T("npsszrc"),
	_T("npvol")
//	,"npysn"
};
const int INSERT_TABLE_MAX_NP = (sizeof(INSERT_TARGET_TABLE_NP)/sizeof(char*));

static char*	INSERT_TARGET_TABLE_NP3[] = {
	_T("knrec"),
	_T("gkrec"),
	_T("kzrec"),
	_T("tkrec"),
	_T("rtrec"),
	_T("month_info"),
	_T("zvolume"),
	_T("zmonth_info"),
	_T("bmvol"),
	_T("bmkname"),
	_T("szvol"),
	_T("dcntl"),
	_T("vtbl"),
	_T("kszrec"),
	_T("npknrec"),
	_T("npsszrc"),
	_T("npvol")
//	,"npysn"
};
const int INSERT_TABLE_MAX_NP3 = (sizeof(INSERT_TARGET_TABLE_NP3)/sizeof(char*));

static char*	INSERT_TARGET_TABLE_SFKS[] = {
	_T("knrec"),
	_T("gkrec"),
	_T("kzrec"),
	_T("tkrec"),
	_T("rtrec"),
	_T("month_info"),
	_T("zvolume"),
	_T("zmonth_info"),
	_T("bmvol"),
	_T("bmkname"),
	_T("szvol"),
	_T("dcntl"),
	_T("vtbl"),
	_T("kszrec"),
	_T("npknrec"),
	_T("npsszrc"),
	_T("npvol"),
	_T("npaut"),
	_T("fuzokukamoku")
//	,"npysn"
};
const int INSERT_TABLE_MAX_SFKS = (sizeof(INSERT_TARGET_TABLE_SFKS)/sizeof(char*));

static char*	INSERT_TARGET_TABLE_SFKS2[] = {
	_T("knrec"),
	_T("gkrec"),
	_T("kzrec"),
	_T("tkrec"),
	_T("rtrec"),
	_T("month_info"),
	_T("zvolume"),
	_T("zmonth_info"),
	_T("bmvol"),
	_T("bmkname"),
	_T("szvol"),
	_T("dcntl"),
	_T("vtbl"),
	_T("gsrec"),
	_T("ksrec"),
	_T("kszrec"),
	_T("npknrec"),
	_T("npsszrc"),
	_T("npvol"),
	_T("npaut"),
	_T("fuzokukamoku")
//	,"npysn"
};
static char*	INSERT_TARGET_TABLE_SFKS2_SYS[] = {
	_T("knrec2"),
	_T("gkrec2"),
	_T("kzrec"),
	_T("tkrec"),
	_T("rtrec"),
	_T("month_info"),
	_T("zvolume"),
	_T("zmonth_info"),
	_T("bmvol"),
	_T("bmkname"),
	_T("szvol2"),
	_T("dcntl"),
	_T("vtbl"),
	_T("gsrec2"),
	_T("ksrec2"),
	_T("kszrec"),
	_T("npknrec2"),
	_T("npsszrc"),
	_T("npvol2"),
	_T("npaut2"),
	_T("fuzokukamoku")
//	,"npysn"
};
const int INSERT_TABLE_MAX_SFKS2 = (sizeof(INSERT_TARGET_TABLE_SFKS2)/sizeof(char*));

static char* INSERT_TARGET_TABLE_SKYO[] = {
	_T("knrec"),
	_T("gkrec"),
	_T("kzrec"),
	_T("tkrec"),
	_T("rtrec"),
	_T("month_info"),
	_T("zvolume"),
	_T("zmonth_info"),
	_T("bmvol"),
	_T("bmkname"),
	_T("szvol"),
	_T("dcntl"),
	_T("vtbl"),
	_T("kszrec"),
	_T("npknrec"),
	_T("npsszrc"),
	_T("npvol"),
	_T("npaut"),
	_T("fuzokukamoku")
//	,"npysn"
};
const int INSERT_TABLE_MAX_SKYO = (sizeof(INSERT_TARGET_TABLE_SKYO)/sizeof(char*));

static char* INSERT_TARGET_TABLE_GAKKO[] = {
	_T("knrec"),
	_T("gkrec"),
	_T("kzrec"),
	_T("tkrec"),
	_T("rtrec"),
	_T("month_info"),
	_T("zvolume"),
	_T("zmonth_info"),
	_T("bmvol"),
	_T("bmkname"),
	_T("szvol"),
	_T("dcntl"),
	_T("vtbl"),
	_T("gsrec"),
	_T("ksrec"),
	_T("kszrec"),
	_T("npknrec"),
	_T("npsszrc"),
	_T("npvol")
};
const int INSERT_TABLE_MAX_GAKKO = (sizeof(INSERT_TARGET_TABLE_GAKKO)/sizeof(char*));

// 税務単体マスター用のボリューム情報('14.11.05)
static char* INSERT_TARGET_TABLE_ONLYTAX[] = {
	_T("zvolume"),
	_T("dcntl"),
	_T("vtbl")
};
const int INSERT_TABLE_MAX_ONLYTAX = (sizeof(INSERT_TARGET_TABLE_ONLYTAX)/sizeof(char*));

//=============================================================================
// 構造体
//=============================================================================
typedef struct _tag_ICSSYZ_COACS_ {
	int		apno;		// アプリケーションNo
	long	cocode;		// 会社コード
	long	user_id;	// ユーザID
	long	user_att;	// ユーザ属性
	BYTE	owner;		// 
	BYTE	access;		// 
	BYTE	option1;	// 
	long	option2;	// 
	long	acsok;		// 
} COACS;

// 和暦年月情報
typedef struct _tag_ICSSYZ_JCYMD_ {
	int		gengo;		// 元号
	UCHAR	ymd[3];		// 年月日（BCD）
	char	dmy[25];
} JCYMD;

//=============================================================================
// SQL
//=============================================================================

// 消費税バージョンの取得
static	char*	GET_SVER_FROM_ZVOL_SQL			=	_T("SELECT s_ver from zvolume");
// 8% の仕訳取得(取消仕訳を除く)
/*- '13.10.16 -*/
// 下のフィルタにすると、受取利息などので非課税で８％のサインが立ってしまうものが正しく集計されないので！
static	char*	GET_ZEI8COUNT_FROM_DATA_SQL		=	_T("SELECT COUNT(*) FROM datarec WHERE ((cast(cast(dsign as binary(2)) as int) & 0x0f) = 0x03) and ((cast(cast(dsign as binary(1)) as int) & 0x01) = 0x00)");
/*-------------*/
//static	char*	GET_ZEI8COUNT_FROM_DATA_SQL		=	_T("SELECT COUNT(*) FROM datarec WHERE ( (( cast (SUBSTRING( dsign, 1, 1 ) as int) & 0x40 = 0x40 AND cast (SUBSTRING( dsign, 3, 1 ) as int) & 0x0f = 0x01) OR dbt = '01071101' OR cre = '01071101' OR dbt = '04051601' OR cre = '04051601') AND cast (SUBSTRING( dsign, 2, 1 ) as int) & 0x0f = 0x03 ) AND ((cast(cast(dsign as binary(1)) as int) & 0x01) = 0x00)");
/*-------------*/
static	char*	GET_ZEI10COUNT_FROM_DATA_SQL		=	_T("SELECT COUNT(*) FROM datarec WHERE ((cast(cast(dsign as binary(2)) as int) & 0x0f) = 0x05) and ((cast(cast(dsign as binary(1)) as int) & 0x01) = 0x00)");
static	char*	GET_ZEI8RDCOUNT_FROM_DATA_SQL		=	_T("SELECT COUNT(*) FROM datarec WHERE ((cast(cast(dsign as binary(2)) as int) & 0x0f) = 0x04) and ((cast(cast(dsign as binary(1)) as int) & 0x01) = 0x00)");
// 指定日以降の仕訳取得
static	char*	GET_DATA_COUNT_FROM_DATA_SQL	=	_T("SELECT COUNT(*) FROM datarec WHERE ddate >= %d");
// 指定日以降の仕訳取得（取消仕訳を除く）
static	char*	GET_DATA_COUNT_FROM_DATA_REM_DEL_SQL	=	_T("SELECT COUNT(*) FROM datarec WHERE (ddate>=%d) and ((cast(cast(dsign as binary(1)) as int) & 0x01) = 0x00)");
// 消費税ボリュームのバージョンアップ (平成26年改正)
static	char*	VUP_SZVOL_FOR_H26SYZ_SQL		=	_T("UPDATE szvol set SVvers=18, SVzper3=80, SVrmax=4, SVrnum=4");
// 消費税ボリュームのバージョンアップ (平成31年改正)
static	char*	VUP_SZVOL_FOR_H31SYZ_SQL		=	_T("UPDATE szvol set SVvers=19, SVzper4=100, SVrmax=5, SVrnum=5");
// 財務ボリュームラベルのバージョンアップ (平成26年改正)
static	char*	VUP_ZVOL_FOR_H26SYZ_SQL			=	_T("UPDATE zvolume set s_ver=%d");
// 財務ボリュームラベルのバージョンアップ (平成31年改正)
static	char*	VUP_ZVOL_FOR_H31SYZ_SQL			=	VUP_ZVOL_FOR_H26SYZ_SQL;
// 消費税バージョンと期末年月日の取得
static	char*	GET_SVER_EYMD_FROM_ZVOL_SQL		=	_T("SELECT s_ver, ee_ymd from zvolume");

// 8% の仕訳取得(取消仕訳を除く：日付指定有)
static	char*	GET_ZEI8COUNT_BY_DATE_FROM_DATA_SQL	=	_T("SELECT COUNT(*) FROM datarec WHERE ((cast(cast(dsign as binary(2)) as int) & 0x0f) = 0x03) and ((cast(cast(dsign as binary(1)) as int) & 0x01) = 0x00) and (ddate>=%d) and (ddate<=%d)");
// 10% の仕訳取得(取消仕訳を除く：日付指定有)
static	char*	GET_ZEI10COUNT_BY_DATE_FROM_DATA_SQL	=	_T("SELECT COUNT(*) FROM datarec WHERE ((cast(cast(dsign as binary(2)) as int) & 0x0f) = 0x05) and ((cast(cast(dsign as binary(1)) as int) & 0x01) = 0x00) and (ddate>=%d) and (ddate<=%d)");
// 8%（軽減） の仕訳取得(取消仕訳を除く：日付指定有)
static	char*	GET_ZEI8RDCOUNT_BY_DATE_FROM_DATA_SQL	=	_T("SELECT COUNT(*) FROM datarec WHERE ((cast(cast(dsign as binary(2)) as int) & 0x0f) = 0x04) and ((cast(cast(dsign as binary(1)) as int) & 0x01) = 0x00) and (ddate>=%d) and (ddate<=%d)");

// 輸入仕入仕訳の仕入区分のセット
static	char*	UPDATE_DATAREC_IMPORT_KARI_SQL	=	_T("UPDATE datarec set datarec.dsign = dbo.set_byte2(datarec.dsign, 3, 0x%02x, 1) where (dbt='%s' or cre='%s') and ((cast(cast(dsign as binary(5)) as int)&0xff)=0x04) and ((cast(cast(dsign as binary(4)) as int)&0x30)=0x00)");
// 消費税集計済みサインをOFF
static	char*	UPDATE_MINFO_OFF_SYKSIGN_SQL	=	_T("UPDATE month_info set c_ksw = 0 where insw = 1");

// 消費税バージョンの取得
static	char*	GET_SVER_MTYPE_FROM_ZVOL_SQL	=	_T("SELECT s_ver, m_type from zvolume");
// 消費税管理情報の取得
static	char*	GET_SVVER_MTYPE_FROM_SZVOL_SQL	=	_T("SELECT SVvers from szvol");
// 消費税管理情報の更新(平成27年改正：簡易６種)
static	char*	MENT_SZVOL_FOR_KANI6_SQL		=	_T("IF NOT EXISTS (SELECT name FROM sys.columns WHERE OBJECT_ID IN (SELECT OBJECT_ID('szvol')) and name = 'SVsper6') BEGIN \n"
													   "ALTER TABLE szvol add SVsper6 smallint null \n"
													   "END \n");
// 消費税管理情報の更新(平成27年改正：簡易６種)
static	char*	VUP_SZVOL_FOR_KANI6_SQL			=	_T("IF EXISTS (SELECT name FROM sys.columns WHERE OBJECT_ID IN (SELECT OBJECT_ID('szvol')) and name = 'SVsper6') BEGIN \n"
													   "UPDATE szvol set SVsper6=600 \n"
													   "UPDATE szvol set SVvers=%d \n"
													   "END\n");

// 簡易第六種の仕訳取得(取消仕訳を除く)
static	char*	GET_KANI6COUNT_FROM_DATA_SQL	=	_T("SELECT COUNT(*) FROM datarec WHERE ( (( cast (SUBSTRING( dsign, 1, 1 ) as int) & 0x40 = 0x40 AND cast (SUBSTRING( dsign, 3, 1 ) as int) & 0x0f = 0x01) OR dbt = '01071101' OR cre = '01071101' OR dbt = '04051601' OR cre = '04051601') AND cast (SUBSTRING( dsign, 4, 1 ) as int) & 0x0f = 0x06 ) AND ((cast(cast(dsign as binary(1)) as int) & 0x01) = 0x00)");
// 簡易第六種の仕訳取得(取消仕訳を除く：日付指定有)
static	char*	GET_KANI6COUNT_BY_DATE_FROM_DATA_SQL	=	_T("SELECT COUNT(*) FROM datarec WHERE ( (( cast (SUBSTRING( dsign, 1, 1 ) as int) & 0x40 = 0x40 AND cast (SUBSTRING( dsign, 3, 1 ) as int) & 0x0f = 0x01) OR dbt = '01071101' OR cre = '01071101' OR dbt = '04051601' OR cre = '04051601') AND cast (SUBSTRING( dsign, 4, 1 ) as int) & 0x0f = 0x06 ) AND ((cast(cast(dsign as binary(1)) as int) & 0x01) = 0x00) AND (ddate>=%d) AND (ddate<=%d)");
// 消費税情報の取得
static	char*	GET_SZINFO_FROM_ZVOL_SQL		=	_T("SELECT s_sgn2 from zvolume");
// 特定収入８％の仕訳取得(取消仕訳を除く：日付指定有)
static	char*	GET_SPC8COUNT_BY_DATE_FROM_DATA_SQL	=	_T("SELECT COUNT(*) FROM datarec WHERE ( (( cast (SUBSTRING(dsign, 1, 1) as int) & 0x40 = 0x40 AND cast (SUBSTRING( dsign, 3, 1 ) as int) & 0x0f = 0x02) ) AND ((cast (SUBSTRING( dsign, 6, 1 ) as int)&0x0f=0x06) OR (cast (SUBSTRING( dsign, 6, 1 ) as int)&0x0f=0x07)) ) AND ((cast(cast(dsign as binary(1)) as int) & 0x01) = 0x00) AND (ddate>=%d) AND (ddate<=%d)");

// 建設マスター：未成工事支出金の情報取得
static	char*	GET_MISEIKOUJI_KJVOL_SQL			=	_T("SELECT VOL_MTYP, VOL_DMSW09 FROM KJVOL");
// 建設マスター：未成工事支出金関連の仕訳取得（貸借科目に未成工事支出金、付箋を使用、対象外）
static	char*	GET_MISEIKOUJI_DATA_SQL				=	_T("(dbt=\'01060201\' or cre =\'01060201\') and ((cast(cast(dsign as binary(8)) as int) & 0x0f)>=0x03) and ((cast(cast(dsign as binary(1)) as int) & 0x40)=0x00)");

// 国税・地方税分割入力の輸入仕訳取得
static	char*	GET_EXTIMPORT_BY_DATE_FROM_DATA_SQL	=	_T("SELECT COUNT(*) FROM datarec WHERE ((cast(cast(dsign as binary(1)) as int) & 0x01) = 0x00) AND (((cast(SUBSTRING(dsign,5,1) as int)&0xFF=0x20)) OR ((cast(SUBSTRING(dsign,5,1) as int)&0xFF=0x21)) OR ((cast(SUBSTRING(dsign,5,1) as int)&0xFF=0x22))) AND (ddate>=%d) AND (ddate<=%d)");
// 完成時振替の仕訳取得
static	char*	GET_KOJI_FURIKAE_BY_DATE_FROM_DATA_SQL	=	_T("SELECT COUNT(*) FROM datarec WHERE ((cast(cast(dsign as binary(1)) as int) & 0x01) = 0x00) AND ((cast(SUBSTRING(dsign,5,1) as int)&0xFF=0x1F)) AND ((cast(SUBSTRING(dsign,7,1) as int)&0x10=0x10)) AND (ddate>=%d) AND (ddate<=%d)");

// szsyk：PKを作成する為のテーブル制約の変更
static	char*	ALTER_SZSYK_CONSTRAINT_SQL	=	_T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[PK_szsyk]') AND type in (N'PK')) \n"
													"BEGIN \n"
													"ALTER TABLE szsyk drop CONSTRAINT[PK_szsyk] \n"
													"END \n"
													"UPDATE szsyk SET SZzchg = 0 WHERE SZzchg is NULL \n"
													"ALTER TABLE szsyk ALTER COLUMN[SZzchg] tinyint NOT NULL \n");
// szsyk：PKに列（SZzchg）を追加
static	char*	ALTER_SZSYK_PK_SQL	=	_T("ALTER TABLE szsyk ADD CONSTRAINT[PK_szsyk] PRIMARY KEY CLUSTERED \n"
											"( \n"
											"[SZkcod] ASC, \n"
											"[SZgrp1] ASC, \n"
											"[SZgrp2] ASC, \n"
											"[SZzkbn] ASC, \n"
											"[SZitm] ASC, \n"
											"[SZsjsg] ASC, \n"
											"[SZzchg] ASC \n"
											") WITH (IGNORE_DUP_KEY = OFF) ON [PRIMARY] \n");

// 輸入仕訳取得
static	char*	GET_IMPORT_BY_DATE_FROM_DATA_SQL	=	_T("SELECT COUNT(*) FROM datarec WHERE ((cast(cast(dsign as binary(1)) as int) & 0x01) = 0x00) AND ( ((cast(SUBSTRING(dsign,2,1) as int)&0x0F=0x00)) OR ((cast(SUBSTRING(dsign,2,1) as int)&0x0F=0x01)) ) AND (((cast(SUBSTRING(dsign,5,1) as int)&0xFF=0x04)) OR ((cast(SUBSTRING(dsign,5,1) as int)&0xFF=0x20)) OR ((cast(SUBSTRING(dsign,5,1) as int)&0xFF=0x21)) OR ((cast(SUBSTRING(dsign,5,1) as int)&0xFF=0x22))) AND (ddate>=%d) AND (ddate<=%d)");

// 免税事業者からの仕入仕訳取得
static	char*	GET_STOCK_FROM_MENZEI_FROM_DATA_SQL	=	_T("SELECT COUNT(*) FROM datarec WHERE ((cast(cast(dsign as binary(7)) as int) & 0x20) = 0x20) AND ((cast(cast(dsign as binary(1)) as int) & 0x01) = 0x00) AND (ddate>=%d) AND (ddate<=%d)");

// 免税事業者からの仕入の仕訳取得(取消仕訳を除く：日付指定有)
static	char*	GET_MENZEICOUNT_BY_DATE_FROM_DATA_SQL	=	_T("SELECT COUNT(*) FROM datarec WHERE ( (( cast (SUBSTRING( dsign, 1, 1 ) as int) & 0x40 = 0x40 AND cast (SUBSTRING( dsign, 3, 1 ) as int) & 0x0f = 0x01) OR dbt = '01071101' OR cre = '01071101' OR dbt = '04051601' OR cre = '04051601') AND cast (SUBSTRING( dsign, 7, 1 ) as int) & 0x20 = 0x20 ) AND ((cast(cast(dsign as binary(1)) as int) & 0x01) = 0x00) AND (ddate>=%d) AND (ddate<=%d)");

// 月別情報を更新して再集計を促す
static	char*	UPDATE_CKSW_IN_MINFO_SQL			=	_T("update month_info set c_ksw=0, intime=current_timestamp where insw=1");
