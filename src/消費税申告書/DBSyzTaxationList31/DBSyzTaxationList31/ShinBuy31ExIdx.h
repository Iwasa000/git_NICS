#pragma once

#define BKLINCNTex		12		// 帳票の行数
#define BKYEARIDex		1
#define BKVMINex		95		//入力ボックスの最小のインデックス
#define BKVMAXex		126		//入力ボックスの最大のインデックス

#define BKZKBNIDex		2


#define	KBUYVAL01Aex	95		// 事業等課税仕入高　金額
#define KBUYVAL01Bex	96		// 事業等課税仕入高　うち軽減税率6.24%適用分
#define KBUYVAL01Cex	97		// 事業等課税仕入高　うち標準税率7.8%適用分

#define	KBUYVAL02Aex	98		// 農業課税仕入高　金額
#define KBUYVAL02Bex	99		// 農業課税仕入高　うち軽減税率6.24%適用分
#define KBUYVAL02Cex	100		// 農業課税仕入高　うち標準税率7.8%適用分

#define	KBUYVAL03Aex	101		// 課税仕入高　金額
#define KBUYVAL03Bex	102		// 課税仕入高　うち軽減税率6.24%適用分
#define KBUYVAL03Cex	103		// 課税仕入高　うち標準税率7.8%適用分

#define	KBUYVAL04Aex	104		// 損益計算書の仕入金額と経費の金額の合計額　金額
#define KBUYVAL04Bex	105		// 損益計算書の仕入金額と経費の金額の合計額　うち軽減税率6.24%適用分
#define KBUYVAL04Cex	106		// 損益計算書の仕入金額と経費の金額の合計額　うち標準税率7.8%適用分

#define	KBUYVAL05Aex	107		// 4のうち、課税仕入れにならないもの　金額
#define KBUYVAL05Bex	108		// 4のうち、課税仕入れにならないもの　うち軽減税率6.24%適用分
#define KBUYVAL05Cex	109		// 4のうち、課税仕入れにならないもの　うち標準税率7.8%適用分

#define	KBUYVAL06Aex	110		// 差引課税仕入高(4-5)　金額
#define KBUYVAL06Bex	111		// 差引課税仕入高(4-5)　うち軽減税率6.24%適用分
#define KBUYVAL06Cex	112		// 差引課税仕入高(4-5)　うち標準税率7.8%適用分

#define	KBUYVAL07Aex	113		// 業務用固定資産等の取得費　金額
#define KBUYVAL07Bex	114		// 業務用固定資産等の取得費　うち軽減税率6.24%適用分
#define KBUYVAL07Cex	115		// 業務用固定資産等の取得費　うち標準税率7.8%適用分

#define	KBUYVAL08Aex	116		// 7のうち、課税仕入れにならないもの　金額
#define KBUYVAL08Bex	117		// 7のうち、課税仕入れにならないもの　うち軽減税率6.24%適用分
#define KBUYVAL08Cex	118		// 7のうち、課税仕入れにならないもの　うち標準税率7.8%適用分

#define	KBUYVAL09Aex	119		// 差引課税仕入高(7-8)　金額
#define KBUYVAL09Bex	120		// 差引課税仕入高(7-8)　うち軽減税率6.24%適用分
#define KBUYVAL09Cex	121		// 差引課税仕入高(7-8)　うち標準税率7.8%適用分

#define	KBUYVAL10Aex	122		// 課税仕入高の合計額　金額
#define KBUYVAL10Bex	123		// 課税仕入高の合計額　うち軽減税率6.24%適用分
#define KBUYVAL10Cex	124		// 課税仕入高の合計額　うち標準税率7.8%適用分

#define	KBUYVAL11Kex	131		// 課税仕入に係る消費税額の計算	うち軽減税率6.24%適用分
#define KBUYVAL11Bex	86
#define KBUYVAL11ex		125

#define	KBUYVAL12Kex	134		// 課税仕入に係る消費税額の計算	うち標準税率7.8%適用分
#define KBUYVAL12Bex	92
#define KBUYVAL12ex		126

#define KBUYNAMEex		129		// (3) (   )所得に係る課税仕入高

// 22-0167,22-0361 cor -->
//#define KBADDTEXTSex	184		// 追加文言開始インデックス
//#define KBADDTEXTEex	187		// 追加文言終了インデックス
// -----------------------
#define KBUYChk1ex		137		// 自動計算チェック	金額
#define KBUYChk2ex		138		// 　　　 〃 　　　	うち軽減税率6.24%適用分
#define KBUYChk3ex		139		// 　　　 〃 　　　	うち標準税率7.8%適用分
// 22-0167,22-0361 cor <--

// 課税仕入高計算表
static SH_ITEMINDEX ShinBuy31Ex[] = {
	{  KBUYVAL01Aex, 0, 1, 0			},
	{  KBUYVAL01Bex, 0, 1, 0			},
	{  KBUYVAL01Cex, 0, 1, 0			},

	{  KBUYVAL02Aex, 0, 1, 0			},
	{  KBUYVAL02Bex, 0, 1, 0			},
	{  KBUYVAL02Cex, 0, 1, 0			},

	{  KBUYVAL03Aex, 0, 1, 0			},
	{  KBUYVAL03Bex, 0, 1, 0			},
	{  KBUYVAL03Cex, 0, 1, 0			},

	{  KBUYVAL04Aex, 1, 1, 0			},
	{  KBUYVAL04Bex, 1, 1, 0			},
	{  KBUYVAL04Cex, 1, 1, 0			},

	{  KBUYVAL05Aex, 1, 1, 0			},
	{  KBUYVAL05Bex, 1, 1, 0			},
	{  KBUYVAL05Cex, 1, 1, 0			},

	{  KBUYVAL06Aex, 0, 0, 0			},
	{  KBUYVAL06Bex, 0, 0, 0			},
	{  KBUYVAL06Cex, 0, 0, 0			},

	{  KBUYVAL07Aex, 0, 1, 0			},
	{  KBUYVAL07Bex, 0, 1, 0			},
	{  KBUYVAL07Cex, 0, 1, 0			},

	{  KBUYVAL08Aex, 0, 1, 0			},
	{  KBUYVAL08Bex, 0, 1, 0			},
	{  KBUYVAL08Cex, 0, 1, 0			},

	{  KBUYVAL09Aex, 0, 0, 0			},
	{  KBUYVAL09Bex, 0, 0, 0			},
	{  KBUYVAL09Cex, 0, 0, 0			},

	{  KBUYVAL10Aex, 0, 0, 0			},
	{  KBUYVAL10Bex, 0, 0, 0			},
	{  KBUYVAL10Cex, 0, 0, 0			},
	
	{  KBUYVAL11Kex, 0, 1, 0			},
	{  KBUYVAL11Bex, 0, 1, 0			},
	{  KBUYVAL11ex,  0, 1, 0			},

	{  KBUYVAL12Kex, 0, 1, 0			},
	{  KBUYVAL12Bex, 0, 1, 0			},
	{  KBUYVAL12ex,  0, 1, 0			},

	{  KBUYNAMEex,   1, 1, 0			},

	{ 0, 0, 0, 0 }
};

static FCSUPDOWN_INFO	FCS_ShinBuy31Ex[] = {
	{	KBUYNAMEex 		},
	{	KBUYVAL01Aex 	},
	{	KBUYVAL02Aex 	},
	{	KBUYVAL03Aex 	},
	{	KBUYVAL04Aex 	},
	{	KBUYVAL05Aex 	},
	{	KBUYVAL06Aex 	},
	{	KBUYVAL07Aex 	},
	{	KBUYVAL08Aex 	},
	{	KBUYVAL09Aex 	},
	{	KBUYVAL10Aex 	},
	{	KBUYVAL11Kex 	},
	{	KBUYVAL11Bex 	},
	{	KBUYVAL11ex 	},
	{	KBUYVAL12Kex 	},
	{	KBUYVAL12Bex 	},
	{	KBUYVAL12ex 	},
	{	KBUYVAL01Bex 	},
	{	KBUYVAL02Bex 	},
	{	KBUYVAL03Bex 	},
	{	KBUYVAL04Bex 	},
	{	KBUYVAL05Bex 	},
	{	KBUYVAL06Bex 	},
	{	KBUYVAL07Bex 	},
	{	KBUYVAL08Bex 	},
	{	KBUYVAL09Bex 	},
	{	KBUYVAL10Bex 	},
	{	KBUYVAL01Cex	},
	{	KBUYVAL02Cex 	},
	{	KBUYVAL03Cex 	},
	{	KBUYVAL04Cex 	},
	{	KBUYVAL05Cex 	},
	{	KBUYVAL06Cex 	},
	{	KBUYVAL07Cex 	},
	{	KBUYVAL08Cex 	},
	{	KBUYVAL09Cex 	},
	{	KBUYVAL10Cex 	},

};

static FCSUPDOWN_INFO	FCS_ShinBuy31Ex_LR[] = {
	{	KBUYVAL01Aex 	},
	{	KBUYVAL01Bex 	},
	{	KBUYVAL01Cex 	},
	{	KBUYVAL02Aex 	},
	{	KBUYVAL02Bex 	},
	{	KBUYVAL02Cex 	},
	{	KBUYVAL03Aex 	},
	{	KBUYVAL03Bex 	},
	{	KBUYVAL03Cex 	},
	{	KBUYNAMEex 		},
	{	KBUYVAL04Aex 	},
	{	KBUYVAL04Bex 	},
	{	KBUYVAL04Cex 	},
	{	KBUYVAL05Aex 	},
	{	KBUYVAL05Bex 	},
	{	KBUYVAL05Cex 	},
	{	KBUYVAL06Aex 	},
	{	KBUYVAL06Bex 	},
	{	KBUYVAL06Cex 	},
	{	KBUYVAL07Aex 	},
	{	KBUYVAL07Bex 	},
	{	KBUYVAL07Cex 	},
	{	KBUYVAL08Aex 	},
	{	KBUYVAL08Bex 	},
	{	KBUYVAL08Cex 	},
	{	KBUYVAL09Aex 	},
	{	KBUYVAL09Bex 	},
	{	KBUYVAL09Cex 	},
	{	KBUYVAL10Aex 	},
	{	KBUYVAL10Bex 	},
	{	KBUYVAL10Cex 	},
	{	KBUYVAL11Kex 	},
	{	KBUYVAL11Bex 	},
	{	KBUYVAL11ex 	},
	{	KBUYVAL12Kex 	},
	{	KBUYVAL12Bex 	},
	{	KBUYVAL12ex 	},
};

#define		FCS_SHINBUY31CNTex	37

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
