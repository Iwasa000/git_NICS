///////////////////////////
//
//		"scansydef.h"
//
//	スキャン用消費税関係
//
///////////////////////////

// 消費税仕訳区分サイン
#define	KH_URI		1
#define KASISON		2
#define KASISYU		3
#define S_IMPORT	4
#define H_EXPORT	5
#define	HAB_SYUN	6
#define HAB_HIYO	7
#define SHIWARI		8
#define URIWARI		9
#define TANA_PL		10
#define TANA_MN		11
#define	TOKT_SYU	12
#define	MEN_EXPORT	13
#define	SI_SYUTOK	14
#define	SI_JOTO		15
//#define	KSI_SYUJO	16
#define	URI_HEN		17
#define	EGAI_SYUEKI	18
#define	TOK_RIEKI	19
#define	UK_RISOK	20
#define	SIIRE		21
#define	SIIRE_HEN	22
#define	KEIHI		23
#define	EGAI_HIYO	24
#define	TOK_SON		25
//#define	YUKA_JOTO	26
//#define	TO_JOTO		27
#define SEISAN_SWK	28

#define RVCHARGE	29
#define RVCHAR_HEN	30
//完成時振替
#define KOJI_KANFURI 31
//輸入消費税
#define YUNYU_KOKU	32
#define YUNYU_CHIHO	33
#define YUNYU_GASAN	34

// 税率 サイン
#define	ZR_5		1
#define	ZR_3		2
#define ZR_0		3
#define ZR_8		4
#define ZR_8K		5
#define ZR_10		6


// 消費税区分サイン
#define	SK_SOTO		1
#define	SK_UCHI		2	
#define	SK_BETU		3
#define	SK_HIKZ		4
#define	SK_KAZI		5
#define	SK_HUKA		6

// 売上業種区分サイン
#define	GY_OROSI	1
#define	GY_KOURI	2
#define	GY_SEIZO	3
#define	GY_SRVCE	5
#define	GY_ETC		4
#define	GY_FDOSN	6

// 仕入区分サイン
#define	URI_KAZEI	1
#define	URI_HIKAZEI	2
#define	URI_KYOTU	3
#define URI_KAMOKU	4	//科目設定に従う

#define URI_KIHON	5	//基本設定
#define URI_TAISYO	6	//仕入区分対象仕訳

// 特定収入区分サイン
#define TK_SYNYU	1	// 特定収入
#define TK_KATOK	2	// 課税特定収入
#define TK_HKATK	3	// 非課税特定収入
#define TK_KYOTK	4	// 共通特定収入
#define TK_FUTOK	5	// 不特定収入
#define TK_GAITK	6	// 外特定収入

#define TK_SYNYU5	7
#define TK_KATOK5	8	// 課税特定収入
#define TK_HKATK5	9	// 非課税特定収入
#define TK_KYOTK5	10	// 共通特定収入
#define TK_FUTOK5	11

#define TK_SYNYU8	12
#define TK_KATOK8	13	// 課税特定収入
#define TK_HKATK8	14	// 非課税特定収入
#define TK_KYOTK8	15	// 共通特定収入
#define TK_FUTOK8	16

#define TK_SYNYU10	17
#define TK_KATOK10	18
#define TK_HKATK10	19
#define TK_KYOTK10	20
#define TK_FUTOK10	21

#define TK_SYNYU8K	22
#define TK_KATOK8K	23
#define TK_HKATK8K	24
#define TK_KYOTK8K	25
#define TK_FUTOK8K	26

#define TK_GAI		50	// 対象外


#define KTS_SYUTOK	1	//資産の取得
#define KTS_JOTO	2	//資産の譲渡

//インボイス免税事業
#define INV_MEN80	1
#define INV_MEN50	2


// 未選択サイン
#define NON_SELECT	0

// 消費税 仕訳サイン取消
#define SWKSW_DEL	100


struct	_ZMSG1 {
	char		*zeitem;
	long		zeifnc;
};

static struct	_ZMSG1	_Z_SCAN[] = {
	{ "（なし）",		NON_SELECT },
	{ "売上",			KH_URI	},
	{ "貸倒損失",		KASISON	},
	{ "貸倒回収",		KASISYU	},
	{ "輸入仕入",		S_IMPORT},
	{ "非課税輸出売上",	H_EXPORT },
	{ "販売奨励金収入",	HAB_SYUN},
	{ "販売奨励金費用",	HAB_HIYO},
	{ "仕入割引",		SHIWARI	 },
	{ "売上割引",		URIWARI	},
	{ "棚卸調整加算額",	TANA_PL	 },
	{ "棚卸調整減算額",	TANA_MN	 },
	{ "輸出売上",		MEN_EXPORT	},
	{ "資産取得",		SI_SYUTOK	},
	{ "資産譲渡",		SI_JOTO		},
//*	{ "固定資産の取得／譲渡",	KSI_SYUJO	},

	{ "売上返還",	URI_HEN	}, // 売上値引・戻り → 売上返還
//	{ "営業外収益",	EGAI_SYUEKI	},
//	{ "特別利益",	TOK_RIEKI	},
	{ "受取利息",	UK_RISOK	},
	{ "仕入",		SIIRE		},
	{ "仕入返還",	SIIRE_HEN	}, // 仕入返品 → 仕入返還
	{ "経費",		KEIHI		},
//	{ "営業外費用",	EGAI_HIYO	},
//	{ "特別損失",	TOK_SON		},
//*	{ "有価証券譲渡",	YUKA_JOTO	},
//*	{ "土地譲渡",		TO_JOTO	},
	{ "消費税精算",	SEISAN_SWK	},
	{ "特定課税仕入",		RVCHARGE },
	{ "特定課税仕入返還",	RVCHAR_HEN },

	{ "輸入消費税(国税)",		YUNYU_KOKU },
	{ "輸入消費税(地方税)",	YUNYU_CHIHO },
	{ "輸入消費税(国地合算)",	YUNYU_GASAN },
	{ "完成時振替",	KOJI_KANFURI }
};

// 税率
static struct	_ZMSG1	_Z_SCAN2[] = {
	{ "（なし）",	NON_SELECT },
	{ "10％",	 	ZR_10 },
	{ "８％軽減",	ZR_8K },
	{ "８％",	 	ZR_8 },
	{ "５％",	 	ZR_5 },
	{ "３％",		ZR_3 },
	{ "０％",		ZR_0 },
};

// 税区分
static struct	_ZMSG1	_Z_SCAN3[] = {
	{ "（なし）",	NON_SELECT },
	{ "内税",		SK_UCHI	 },
	{ "外税",	 	SK_SOTO	 },
	{ "別記",		SK_BETU	 },
	{ "非課税",	 	SK_HIKZ	 },
	{ "不課税",		SK_HUKA  },
};	


static struct _ZMSG1 _Z_SCAN4[] = {
	{ "（なし）",	NON_SELECT },
	{ "卸売業",		GY_OROSI },
	{ "小売業",		GY_KOURI },
	{ "製造業",		GY_SEIZO },
	{ "その他",		GY_ETC },
	{ "サービス業",	GY_SRVCE },
	{ "不動産業",	GY_FDOSN }
};

static struct _ZMSG1	_Z_SCAN5[] = {
	{ "（なし）",	NON_SELECT },
	{ "課税売上",	URI_KAZEI },
	{ "非課税売上",	URI_HIKAZEI },
	{ "共通売上",	URI_KYOTU }
};

#ifdef _OLD_TOKTEI_	// 05.10 /01
// 特定収入（個別対応時）
static struct _ZMSG1	_Z_SCAN6[] = {
	{ "（なし）",		NON_SELECT },
# ifndef CLOSE	// 05.29 /01
	{ "特定収入",		TK_SYNYU },
# endif
	{ "課税特定収入",	TK_KATOK },
	{ "非課税特定収入",	TK_HKATK },
	{ "共通特定収入",	TK_KYOTK },
	{ "不特定収入",		TK_FUTOK },
	{ "外特定収入",		TK_GAITK },
	{ "対象外",			TK_GAI   }
};

// 特定収入（比例配分時）
static struct _ZMSG1	_Z_SCAN7[] = {
	{ "（なし）",	NON_SELECT },
	{ "特定収入",	TK_SYNYU },
	{ "不特定収入",	TK_FUTOK },
	{ "外特定収入",	TK_GAITK },
	{ "対象外",		TK_GAI   }

};
#endif



// 特定収入（個別対応時）
static struct _ZMSG1	_Z_SCAN6[] = {
	{ "（なし）",		NON_SELECT },

	{ "特定収入",		TK_SYNYU },
	{ "課税特定収入",	TK_KATOK },
	{ "非課税特定収入",	TK_HKATK },
	{ "共通特定収入",	TK_KYOTK },

	{ "不特定収入",		TK_FUTOK },
	{ "外特定収入",		TK_GAITK },
	{ "特定収入(5%)",	TK_SYNYU5 },
	{ "課税特定収入(5%)",	TK_KATOK5 },
	{ "非課税特定収入(5%)",	TK_HKATK5 },
	{ "共通特定収入(5%)",	TK_KYOTK5 },
	{ "不特定収入(5%)",	TK_FUTOK5 },

	{ "特定収入(8%)",	TK_SYNYU8 },
	{ "課税特定収入(8%)",	TK_KATOK8 },
	{ "非課税特定収入(8%)",	TK_HKATK8 },
	{ "共通特定収入(8%)",	TK_KYOTK8 },
	{ "不特定収入(8%)",	TK_FUTOK8 },

	{ "特定収入(8%軽減)",		TK_SYNYU8K },
	{ "課税特定収入(8%軽減)",	TK_KATOK8K },
	{ "非課税特定収入(8%軽減)",TK_HKATK8K },
	{ "共通特定収入(8%軽減)",	TK_KYOTK8K },
	{ "不特定収入(8%軽減)",	TK_FUTOK8K },

	{ "特定収入(10%)",		TK_SYNYU10 },
	{ "課税特定収入(10%)",	TK_KATOK10 },
	{ "非課税特定収入(10%)",TK_HKATK10 },
	{ "共通特定収入(10%)",	TK_KYOTK10 },
	{ "不特定収入(10%)",	TK_FUTOK10 },

	{ "対象外",		TK_GAI },
};


// 特定収入
static struct _ZMSG1	_Z_SCAN7[] = {
	{ "（なし）",	NON_SELECT },
	{ "特定収入",	TK_SYNYU },
	{ "不特定収入",	TK_FUTOK },
	{ "外特定収入",	TK_GAITK },
	{ "特定収入(5%)",	TK_SYNYU5 },
	{ "不特定収入(5%)",	TK_FUTOK5 },
	{ "特定収入(8%)",	TK_SYNYU8 },
	{ "不特定収入(8%)",	TK_FUTOK8 },
	{ "特定収入(8%軽減)",		TK_SYNYU8K },
	{ "不特定収入(8%軽減)",	TK_FUTOK8K },
	{ "特定収入(10%)",	TK_SYNYU10 },
	{ "不特定収入(10%)",TK_FUTOK10 },
{ "対象外",		TK_GAI },
};



struct _ZMSGBIND {
	char	_kbn_sgn,
			n_zmsg;
	struct _ZMSG1 *zmsg;
};


#define SWKKB_PG	0
#define ZRITU_PG	1
#define SKBN_PG		2
#define GYOSYU_PG	3
#define SIRE_PG		4
#define KBTOKUS_PG	5
#define TOKUS_PG	6

static struct _ZMSGBIND ZMSGBND[TOKUS_PG+1] = {
	{ SWKKB_PG,	sizeof( _Z_SCAN ) / sizeof( _Z_SCAN[0] ),	_Z_SCAN },
	{ ZRITU_PG,	sizeof( _Z_SCAN2 ) / sizeof( _Z_SCAN2[0] ),	_Z_SCAN2 },
	{ SKBN_PG,	sizeof( _Z_SCAN3 ) / sizeof( _Z_SCAN3[0] ),	_Z_SCAN3 },
	{ GYOSYU_PG,sizeof( _Z_SCAN4 ) / sizeof( _Z_SCAN4[0] ),	_Z_SCAN4 },
	{ SIRE_PG,	sizeof( _Z_SCAN5 ) / sizeof( _Z_SCAN5[0] ), _Z_SCAN5 },
	{ KBTOKUS_PG,	sizeof( _Z_SCAN6 ) / sizeof( _Z_SCAN6[0] ), _Z_SCAN6 },
	{ TOKUS_PG,		sizeof( _Z_SCAN7 ) / sizeof( _Z_SCAN7[0] ), _Z_SCAN7 },
};


#define USE_SKBN_PG		0
#define USE_URISIRE_PG	1

static struct _ZMSGBIND *NOW_SYSEL_PG[USE_URISIRE_PG+1] = {0};
