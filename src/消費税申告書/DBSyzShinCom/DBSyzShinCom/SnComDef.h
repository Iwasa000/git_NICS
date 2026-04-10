
//#pragma	once
#ifndef  __DBSYZSHINCOM_ICS_SNCOMDEF__
#define  __DBSYZSHINCOM_ICS_SNCOMDEF__


//== 文字列 ==
static	char	*ICS_SN_IPPAN_HONPYO_HYOID	=	_T("SHA010");	// 一般本表
static	char	*ICS_SN_KANNI_HONPYO_HYOID	=	_T("SHA020");	// 一般簡易
static	char	*ICS_SN_FHYO_1_HYOID		=	_T("SHB010");	// 付表１
static	char	*ICS_SN_FHYO_2_HYOID		=	_T("SHB020");	// 付表２
static	char	*ICS_SN_FHYO_2_2_HYOID		=	_T("SHB030");	// 付表２－２
static	char	*ICS_SN_FHYO_4_HYOID		=	_T("SHB040");	// 付表４
static	char	*ICS_SN_FHYO_5_HYOID		=	_T("SHB050");	// 付表５
static	char	*ICS_SN_FHYO_5_2_HYOID		=	_T("SHB060");	// 付表５－２
static	char	*ICS_SN_TANA_HIREI_HYOID	=	_T("SHX010");	// 棚卸調整（比例配分）
static	char	*ICS_SN_TANA_KOBETU_HYOID	=	_T("SHX020");	// 棚卸調整（個別対応）

// 消費税10%対応
static	char	*ICS_SN_IPPAN_HONPYO1_HYOID	=	_T("SHA010-1");	// 一般本表
static	char	*ICS_SN_IPPAN_HONPYO2_HYOID	=	_T("SHA010-2");	// 一般本表2
static	char	*ICS_SN_KANNI_HONPYO1_HYOID	=	_T("SHA020-1");	// 簡易本表
static	char	*ICS_SN_KANNI_HONPYO2_HYOID	=	_T("SHA020-2");	// 簡易本表2
static char		*ICS_SN_FHYO_1_01_HYOID		=	_T("SHB013");	// 付表１－１
static char		*ICS_SN_FHYO_1_02_HYOID		=	_T("SHB015");	// 付表１－２
static char		*ICS_SN_FHYO_2_01_HYOID		=	_T("SHB025");	// 付表２－１
static char		*ICS_SN_FHYO_2_02_HYOID		=	_T("SHB030");	// 付表２－２
static char		*ICS_SN_FHYO_4_01_HYOID		=	_T("SHB043");	// 付表４－１
static char		*ICS_SN_FHYO_4_02_HYOID		=	_T("SHB045");	// 付表４－２
static char		*ICS_SN_FHYO_5_01_HYOID		=	_T("SHB055-1");	// 付表５－１（１）
static char		*ICS_SN_FHYO_5_012_HYOID	=	_T("SHB055-2");	// 付表５－１（２）
static char		*ICS_SN_FHYO_5_02_HYOID		=	_T("SHB065-1");	// 付表５－２（１）
static char		*ICS_SN_FHYO_5_022_HYOID	=	_T("SHB065-2");	// 付表５－２（２）
static char		*ICS_SN_FHYO_6_01_HYOID		=	_T("SHB070");	// 付表６
static char     *ICS_SN_TOKUREI_1_HYOID     =   _T("SHB100");   // 特例帳票１
static char     *ICS_SN_TOKUREI_2_HYOID     =   _T("SHB110");   // 特例帳票２
static char     *ICS_SN_TOKUREI_3_HYOID     =   _T("SHB120");   // 特例帳票２

//== 定数 ==
// 申告区分
typedef enum {	
	ID_ICSSH_KAKUTEI = 1,		// 確定
	ID_ICSSH_TYUUKAN,			// 中間
	ID_ICSSH_YOTEI,				// 予定(26号)
	ID_ICSSH_KAKUTEI_SYUUSEI,	// 修正確定
	ID_ICSSH_TYUUKAN_SYUUSEI,	// 修正中間
	ID_ICSSH_KOUSEI,			// 更正
//2018.03.13 INSERT START
	ID_ICSSH_JUNKAKUTEI,
//2018.03.13 INSERT END
} EnumIdIcsShShinkokuKubun;

// 課税方式
typedef enum {
	ID_ICSSH_GENSOKU = 1,		// 原則課税
	ID_ICSSH_KANNI,				// 簡易課税
	ID_ICSSH_MENZEI,			// 免税
} EnumIdIcsShKazeihoushiki;

// タブ種別	('15.02.11)
typedef enum {
	ID_ICSSH_NOLIST_TYPE	=	-1,	// 選択無し
	ID_ICSSH_TAXLIST_TYPE	=	1,	// 課税取引
	ID_ICSSH_SPCLIST_TYPE			// 特定収入
} EnumIdIcsSelTaxtype;

// 改正様式	('15.08.27)
typedef enum {
	ID_ICSSH_NO_REVTYPE			=	-1,	// 該当無し
	ID_ICSSH_8PER_REVTYPE		=	1,	// 消費税率8%
	ID_ICSSH_SIXKIND_REVTYPE,			// 簡易課税第六種
	ID_ICSSH_TKKZSIIRE_REVTYPE,			// 特定課税仕入
	ID_ICSSH_MYNUMBER_REVTYPE,			// マイナンバー
	ID_ICSSH_10PER_REVTYPE,				// 消費税率10%
	ID_ICSSH_KYOJU_REVTYPR,				// 居住用賃貸建物を課税賃貸用に供した
} EnumIdIcsShRevType;

// 元号
typedef enum {
	ID_ICSSH_MEIJI_GENGO	=	1,		// 明治
	ID_ICSSH_TAISHO_GENGO,				// 大正
	ID_ICSSH_SHOWA_GENGO,				// 昭和
	ID_ICSSH_HEISEI_GENGO,				// 平成
	ID_ICSSH_REIWA_GENGO				// 令和
} EnumIdIcsSyzGengo;

//保存データと表示データ関連付け
typedef struct _TBLDATAINFO {
	CString itemId;
	int kind;
	char* data;
	int size;
	int* diagidx;
} TBLDATAINFO;

typedef enum {
	DATAKIND_VAL,
	DATAKIND_RATE,
	DATAKIND_STR,
	DATAKIND_DATE,
	DATAKIND_CHECK,
	DATAKIND_SIGN
} EnumDataKind;

typedef enum {
	TOKUREIMODE_HIRENDO,  //非連動
    TOKUREIMODE_HANRENDO, //半連動(特例のみ非連動)
	TOKUREIMODE_RENDO,    //連動
} EnumTokureiMode;

//特例計算期間
#define TOKUREI_KIKAN_S 20191001
#define TOKUREI_KIKAN_E 20230930

//特例テーブルバージョン
#define TOKUREI_TBL_VERSION 3
#define TOKUREI_TBL_OLD_VERSION 2

//微調整用テーブル番号
#define	AD_IPPAN		0		// 申告書本表（原則）
#define	AD_KANI			4		// 申告書本表（簡易）
#define	AD_KANPU_K		8		// 還付申告明細書（個人）
#define	AD_KANPU_H		9		// 還付申告明細書（法人）
#define	AD_KOSEI_K		10		// 更正の請求書（個人）
#define	AD_KOSEI_H		11		// 更正の請求書（法人）
#define	AD_KAZEI		12		// 課税取引金額計算表
#define	AD_IPPAN2		14		// 課税標準額等内訳書
#define	AD_FHYO6		15		// 付表６
#define	AD_FHYO6_2wari	23		// 付表６（税率別消費税額計算表）	('23.05.01 Add)
//-->yoshida190816
//付表１、２、４、５は旧と共有の場所使うよう変更。微調整引継ぎの為
#define	AD_FHYO1		1		// 付表１
#define	AD_FHYO2		3		// 付表２
#define	AD_FHYO4		5		// 付表４
#define	AD_FHYO5		7		// 付表５
//----
//#define	AD_FHYO1		16		// 付表１
//#define	AD_FHYO2		17		// 付表２
//#define	AD_FHYO4		18		// 付表４
//#define	AD_FHYO5		19		// 付表５
//<-----
#define	AD_TOKUREI1		20		// 第５－（１）号
#define	AD_TOKUREI2		21		// 第５－（２）号
#define	AD_TOKUREI3		22		// 第５－（３）号


#endif	// __DBSYZSHINCOM_ICS_SNCOMDEF__