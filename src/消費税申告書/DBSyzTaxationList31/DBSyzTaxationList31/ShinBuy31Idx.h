#pragma once

#define BKLINCNT	13 // 仮
#define BKYEARID	1
#define BKVMIN		127	//入力ボックスの最小のインデックス
#define BKVMAX		169 //入力ボックスの最大のインデックス

#define BKZKBNID	2


#define	KBUYVAL01A	127		// 事業等課税仕入高　金額
#define KBUYVAL01B	128		// 事業等課税仕入高　うち旧税率6.3%適用分
#define KBUYVAL01C	129		// 事業等課税仕入高　うち軽減税率6.24%適用分
#define KBUYVAL01D	130		// 事業等課税仕入高　うち標準税率7.8%適用分

#define	KBUYVAL02A	131		// 農業課税仕入高　金額
#define KBUYVAL02B	132		// 農業課税仕入高　うち旧税率6.3%適用分
#define KBUYVAL02C	133		// 農業課税仕入高　うち軽減税率6.24%適用分
#define KBUYVAL02D	134		// 農業課税仕入高　うち標準税率7.8%適用分

#define	KBUYVAL03A	135		// 課税仕入高　金額
#define KBUYVAL03B	136		// 課税仕入高　うち旧税率6.3%適用分
#define KBUYVAL03C	137		// 課税仕入高　うち軽減税率6.24%適用分
#define KBUYVAL03D	138		// 課税仕入高　うち標準税率7.8%適用分

#define	KBUYVAL04A	139		// 損益計算書の仕入金額と経費の金額の合計額　金額
#define KBUYVAL04B	140		// 損益計算書の仕入金額と経費の金額の合計額　うち旧税率6.3%適用分
#define KBUYVAL04C	141		// 損益計算書の仕入金額と経費の金額の合計額　うち軽減税率6.24%適用分
#define KBUYVAL04D	142		// 損益計算書の仕入金額と経費の金額の合計額　うち標準税率7.8%適用分

#define	KBUYVAL05A	143		// 4のうち、課税仕入れにならないもの　金額
#define KBUYVAL05B	144		// 4のうち、課税仕入れにならないもの　うち旧税率6.3%適用分
#define KBUYVAL05C	145		// 4のうち、課税仕入れにならないもの　うち軽減税率6.24%適用分
#define KBUYVAL05D	146		// 4のうち、課税仕入れにならないもの　うち標準税率7.8%適用分

#define	KBUYVAL06A	147		// 差引課税仕入高(4-5)　金額
#define KBUYVAL06B	148		// 差引課税仕入高(4-5)　うち旧税率6.3%適用分
#define KBUYVAL06C	149		// 差引課税仕入高(4-5)　うち軽減税率6.24%適用分
#define KBUYVAL06D	150		// 差引課税仕入高(4-5)　うち標準税率7.8%適用分

#define	KBUYVAL07A	151		// 業務用固定資産等の取得費　金額
#define KBUYVAL07B	152		// 業務用固定資産等の取得費　うち旧税率6.3%適用分
#define KBUYVAL07C	153		// 業務用固定資産等の取得費　うち軽減税率6.24%適用分
#define KBUYVAL07D	154		// 業務用固定資産等の取得費　うち標準税率7.8%適用分

#define	KBUYVAL08A	155		// 7のうち、課税仕入れにならないもの　金額
#define KBUYVAL08B	156		// 7のうち、課税仕入れにならないもの　うち旧税率6.3%適用分
#define KBUYVAL08C	157		// 7のうち、課税仕入れにならないもの　うち軽減税率6.24%適用分
#define KBUYVAL08D	158		// 7のうち、課税仕入れにならないもの　うち標準税率7.8%適用分

#define	KBUYVAL09A	159		// 差引課税仕入高(7-8)　金額
#define KBUYVAL09B	160		// 差引課税仕入高(7-8)　うち旧税率6.3%適用分
#define KBUYVAL09C	161		// 差引課税仕入高(7-8)　うち軽減税率6.24%適用分
#define KBUYVAL09D	162		// 差引課税仕入高(7-8)　うち標準税率7.8%適用分

#define	KBUYVAL10A	163		// 課税仕入高の合計額　金額
#define KBUYVAL10B	164		// 課税仕入高の合計額　うち旧税率6.3%適用分
#define KBUYVAL10C	165		// 課税仕入高の合計額　うち軽減税率6.24%適用分
#define KBUYVAL10D	166		// 課税仕入高の合計額　うち標準税率7.8%適用分

#define	KBUYVAL11K	175		// 課税仕入に係る消費税額の計算	うち旧税率6.3%適用分
#define KBUYVAL11C  111
#define KBUYVAL11	167

#define	KBUYVAL12K	178		// 課税仕入に係る消費税額の計算	うち軽減税率6.24%適用分
#define KBUYVAL12C	118
#define KBUYVAL12	168

#define	KBUYVAL13K	181		// 課税仕入に係る消費税額の計算	うち標準税率7.8%適用分
#define KBUYVAL13C	124
#define KBUYVAL13	169

#define KBUYNAME	173		// (3) (   )所得に係る課税仕入高

#define KBADDTEXTS	184		// 追加文言開始インデックス
#define KBADDTEXTE	187		// 追加文言終了インデックス


// 課税仕入高計算表
static SH_ITEMINDEX ShinBuy31[] = {
	{  KBUYVAL01A, 0, 1, 0			},
	{  KBUYVAL01B, 0, 2, 0			},
	{  KBUYVAL01C, 0, 1, 0			},
	{  KBUYVAL01D, 0, 1, 0			},

	{  KBUYVAL02A, 0, 1, 0			},
	{  KBUYVAL02B, 0, 2, 0			},
	{  KBUYVAL02C, 0, 1, 0			},
	{  KBUYVAL02D, 0, 1, 0			},

	{  KBUYVAL03A, 0, 1, 0			},
	{  KBUYVAL03B, 0, 2, 0			},
	{  KBUYVAL03C, 0, 1, 0			},
	{  KBUYVAL03D, 0, 1, 0			},

	{  KBUYVAL04A, 1, 1, 0			},
	{  KBUYVAL04B, 2, 2, 0			},
	{  KBUYVAL04C, 1, 1, 0			},
	{  KBUYVAL04D, 1, 1, 0			},

	{  KBUYVAL05A, 1, 1, 0			},
	{  KBUYVAL05B, 2, 2, 0			},
	{  KBUYVAL05C, 1, 1, 0			},
	{  KBUYVAL05D, 1, 1, 0			},

	{  KBUYVAL06A, 0, 0, 0			},
	{  KBUYVAL06B, 0, 0, 0			},
	{  KBUYVAL06C, 0, 0, 0			},
	{  KBUYVAL06D, 0, 0, 0			},

	{  KBUYVAL07A, 0, 1, 0			},
	{  KBUYVAL07B, 0, 2, 0			},
	{  KBUYVAL07C, 0, 1, 0			},
	{  KBUYVAL07D, 0, 1, 0			},

	{  KBUYVAL08A, 0, 1, 0			},
	{  KBUYVAL08B, 0, 2, 0			},
	{  KBUYVAL08C, 0, 1, 0			},
	{  KBUYVAL08D, 0, 1, 0			},

	{  KBUYVAL09A, 0, 0, 0			},
	{  KBUYVAL09B, 0, 0, 0			},
	{  KBUYVAL09C, 0, 0, 0			},
	{  KBUYVAL09D, 0, 0, 0			},

	{  KBUYVAL10A, 0, 0, 0			},
	{  KBUYVAL10B, 0, 0, 0			},
	{  KBUYVAL10C, 0, 0, 0			},
	{  KBUYVAL10D, 0, 0, 0			},

	{  KBUYVAL11K, 0, 2, 0			},
	{  KBUYVAL11C, 0, 2, 0			},
	{  KBUYVAL11,  0, 2, 0			},
	
	{  KBUYVAL12K, 0, 1, 0			},
	{  KBUYVAL12C, 0, 1, 0			},
	{  KBUYVAL12,  0, 1, 0			},

	{  KBUYVAL13K, 0, 1, 0			},
	{  KBUYVAL13C, 0, 1, 0			},
	{  KBUYVAL13,  0, 1, 0			},

	{  KBUYNAME,   1, 1, 0			},

	{ 0, 0, 0, 0 }
};

static FCSUPDOWN_INFO	FCS_ShinBuy31[] = {
	{	KBUYNAME 		},
	{	KBUYVAL01A 		},
	{	KBUYVAL02A 		},
	{	KBUYVAL03A 		},
	{	KBUYVAL04A 		},
	{	KBUYVAL05A 		},
	{	KBUYVAL06A 		},
	{	KBUYVAL07A 		},
	{	KBUYVAL08A 		},
	{	KBUYVAL09A 		},
	{	KBUYVAL10A 		},
	{	KBUYVAL11K 		},
	{	KBUYVAL11C 		},
	{	KBUYVAL11 		},
	{	KBUYVAL12K 		},
	{	KBUYVAL12C 		},
	{	KBUYVAL12 		},
	{	KBUYVAL13K 		},
	{	KBUYVAL13C 		},
	{	KBUYVAL13 		},
	{	KBUYVAL01B 		},
	{	KBUYVAL02B 		},
	{	KBUYVAL03B 		},
	{	KBUYVAL04B 		},
	{	KBUYVAL05B 		},
	{	KBUYVAL06B 		},
	{	KBUYVAL07B 		},
	{	KBUYVAL08B 		},
	{	KBUYVAL09B 		},
	{	KBUYVAL10B 		},
	{	KBUYVAL01C 		},
	{	KBUYVAL02C 		},
	{	KBUYVAL03C 		},
	{	KBUYVAL04C 		},
	{	KBUYVAL05C 		},
	{	KBUYVAL06C 		},
	{	KBUYVAL07C 		},
	{	KBUYVAL08C 		},
	{	KBUYVAL09C 		},
	{	KBUYVAL10C 		},
	{	KBUYVAL01D		},
	{	KBUYVAL02D 		},
	{	KBUYVAL03D 		},
	{	KBUYVAL04D 		},
	{	KBUYVAL05D 		},
	{	KBUYVAL06D 		},
	{	KBUYVAL07D 		},
	{	KBUYVAL08D 		},
	{	KBUYVAL09D 		},
	{	KBUYVAL10D 		},

};

static FCSUPDOWN_INFO	FCS_ShinBuy31_LR[] = {
	{	KBUYVAL01A 		},
	{	KBUYVAL01B 		},
	{	KBUYVAL01C 		},
	{	KBUYVAL01D 		},
	{	KBUYVAL02A 		},
	{	KBUYVAL02B 		},
	{	KBUYVAL02C 		},
	{	KBUYVAL02D 		},
	{	KBUYVAL03A 		},
	{	KBUYVAL03B 		},
	{	KBUYVAL03C 		},
	{	KBUYVAL03D 		},
	{	KBUYNAME 		},
	{	KBUYVAL04A 		},
	{	KBUYVAL04B 		},
	{	KBUYVAL04C 		},
	{	KBUYVAL04D 		},
	{	KBUYVAL05A 		},
	{	KBUYVAL05B 		},
	{	KBUYVAL05C 		},
	{	KBUYVAL05D 		},
	{	KBUYVAL06A 		},
	{	KBUYVAL06B 		},
	{	KBUYVAL06C 		},
	{	KBUYVAL06D 		},
	{	KBUYVAL07A 		},
	{	KBUYVAL07B 		},
	{	KBUYVAL07C 		},
	{	KBUYVAL07D 		},
	{	KBUYVAL08A 		},
	{	KBUYVAL08B 		},
	{	KBUYVAL08C 		},
	{	KBUYVAL08D 		},
	{	KBUYVAL09A 		},
	{	KBUYVAL09B 		},
	{	KBUYVAL09C 		},
	{	KBUYVAL09D 		},
	{	KBUYVAL10A 		},
	{	KBUYVAL10B 		},
	{	KBUYVAL10C 		},
	{	KBUYVAL10D 		},
	{	KBUYVAL11K 		},
	{	KBUYVAL11C 		},
	{	KBUYVAL11 		},
	{	KBUYVAL12K 		},
	{	KBUYVAL12C 		},
	{	KBUYVAL12 		},
	{	KBUYVAL13K 		},
	{	KBUYVAL13C 		},
	{	KBUYVAL13 		},
};

#define		FCS_SHINBUY31CNT	50

//清水追加
//#define		KBUYLINE1	0
//#define		KBUYLINE2	1
//#define		KBUYLINE3	2
//#define		KBUYLINE4	6
//#define		KBUYLINE5	7
//#define		KBUYLINE6	8
//#define		KBUYLINE7	9
//#define		KBUYLINE8	10
//#define		KBUYLINE9	11
//#define		KBUYLINE10	12
//#define		KBUYLINE11	13
//#define		KBUYLINE12	14
//#define		KBUYLINE13	15
////#define		KBUYLINE14	17
////#define		KBUYLINE15	16
//#define		KBUYLINE14	16
//#define		KBUYLINE15	17
//#define		KBUYLINE16	18

#define		KBUYLINE1	0
#define		KBUYLINE2	1
#define		KBUYLINE3	2
#define		KBUYLINE4	3
#define		KBUYLINE5	4
#define		KBUYLINE6	5
#define		KBUYLINE7	6
#define		KBUYLINE8	7
#define		KBUYLINE9	8
#define		KBUYLINE10	9
#define		KBUYLINE11	10
#define		KBUYLINE12	11
#define		KBUYLINE13	12
#define		KBUYLINE14	13
#define		KBUYLINE15	14
#define		KBUYLINE16	15
