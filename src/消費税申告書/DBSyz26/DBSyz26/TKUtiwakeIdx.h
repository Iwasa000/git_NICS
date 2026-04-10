#pragma once

// 基本情報
#define ID1020		31	// 郵便番号
#define ID1030		32	// 納税地　上段
#define ID1031		33	// 〃　　　下段
#define ID1040		34	// 電話番号　市外
#define ID1041		35	// 〃　　　　市内
#define ID1042		36	// 〃		 局番
#define ID1050		37	// 名称又は屋号		（フリガナ）
#define ID1051		38	// 名称又は屋号
#define ID1052		39	// 被合併名
#define ID1060		40	// 代書者又は氏名	（フリガナ）
#define ID1061		41	// 代書者又は氏名
#define ID1071		56	// 整理番号

#define	ID1080		42	// 課税期間　自年
#define	ID1081		43	// 〃		 自月
#define	ID1082		44	// 〃		 自日
#define	ID1090		45	// 課税期間　至年
#define	ID1091		46	// 〃		 至月
#define	ID1092		47	// 〃		 至日

#define	ID1100		48	// 確定等

#define	ID1110		49	// 中間期間　自年
#define	ID1111		50	// 〃		 自月
#define	ID1112		51	// 〃		 自日
#define	ID1120		52	// 中間期間　至年
#define	ID1121		53	// 〃		 至月
#define	ID1122		54	// 〃		 至日

// 基本情報元号
#define	ID1083		11	// 課税期間　自元号
#define	ID1093		15	// 課税期間　至元号
#define	ID1113		22	// 中間期間　自元号
#define	ID1123		26	// 中間期間　至元号

// 金額項目
#define	ID2XM1		40	// 1.課税標準額
#define	ID2XM2		41	// 2.課税資産の譲渡等の対価の額
#define ID2XM3		42	// 3.特定課税仕入に係る支払対価の額
#define	ID2XM4		43	// 4.返還等対価に係る税額
#define	ID2XM5		44	// 5.売上げの返還等対価に係る税額
#define	ID2XM6		45	// 6.特定課税仕入の返還等対価に係る税額

						// 2及び3の内訳
#define	ID2UTI11	46	// 課税資産の譲渡等の対価の額(3%)
#define	ID2UTI12	47	// 課税資産の譲渡等の対価の額(4%)
#define	ID2UTI13	48	// 課税資産の譲渡等の対価の額(6.3%)
#define	ID2UTI23	50	// 特定課税仕入に係る支払対価の額(6.3%)
#define	ID2UTI31	52	// 合計(3%)
#define	ID2UTI32	53	// 合計(4%)
#define	ID2UTI33	54	// 合計(6.3%)

// 第27-(3)号様式①
static SH_ITEMINDEX	TkUtiwakeIdx1[]	=	{
	{	ID1020,  1, 1, 0	},
	{	ID1030,  1, 1, 0	},
	{	ID1031,  1, 1, 0	},
	{	ID1040,  1, 1, 0	},
	{	ID1041,  1, 1, 0	},
	{	ID1042,  1, 1, 0	},
	{	ID1050,  1, 1, 0	},
	{	ID1051,  1, 1, 0	},
	{	ID1052,  1, 1, 0	},
	{	ID1060,  1, 1, 0	},
	{	ID1061,  1, 1, 0	},
	{	ID1071,  1, 1, 0	},
	{	0,0,0,0				}
};

// 第27-(3)号様式②
static SH_ITEMINDEX	TkUtiwakeIdx2[]	=	{
	{	ID2XM1,		0, 0, 0	},
	{	ID2XM2,		0, 2, 0	},
	{	ID2XM3,		0, 2, 0	},
	{	ID2XM4,		0, 0, 0	},
	{	ID2XM5,		0, 2, 0	},
	{	ID2XM6,		0, 2, 0	},
//	{	ID2UTI11,	0, 0, 0	},
//	{	ID2UTI12,	0, 0, 0	},
//	{	ID2UTI13,	0, 0, 0	},
//	{	ID2UTI23,	0, 0, 0	},
//	{	ID2UTI31,	0, 0, 0	},
//	{	ID2UTI32,	0, 0, 0	},
//	{	ID2UTI33,	0, 0, 0	},
	{	0,0,0,0				}
};
