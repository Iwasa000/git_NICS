#pragma once

#define		SKLINCNTex2	12		// 帳票の行数。この行数でデータを保存していってる
#define		SKVMINex2	95		// 入力ボックスの最小のインデックス
#define		SKVMAXex2	126		// 入力ボックスの最大のインデックス

#define		SKYEARIDex2	1		// 令和～年分
#define		SKZKBNIDex2	2		// 経理方式
#define		SKDISP1ex2	29		// ?

#define		Msizex2		512*20	// 印刷関連
#define		fno1_sex2	3		// フィールド１の小さいフォントフィールド番号　いらんかも


// 入力項目のindex
#define	SEL31NAMEex2	129		// (3)～所得に係る課税売上高

#define	SEL3101ex2	95		// (1)事業所得に係る課税売上高　営業課税売上高　金額
#define	SEL3101Aex2	96		// (1)事業所得に係る課税売上高　営業課税売上高　6.24%
#define	SEL3101Bex2	97		// (1)事業所得に係る課税売上高　営業課税売上高　7.8%

#define	SEL3102ex2	98		// (1)事業所得に係る課税売上高　農業課税売上高　金額
#define	SEL3102Aex2	99		// (1)事業所得に係る課税売上高　農業課税売上高　6.24%
#define	SEL3102Bex2	100		// (1)事業所得に係る課税売上高　農業課税売上高　7.8%

#define	SEL3103ex2	101		// (2)不動産所得に係る課税売上高　課税売上高　金額
#define	SEL3103Aex2	102		// (2)不動産所得に係る課税売上高　課税売上高　6.24%
#define	SEL3103Bex2	103		// (2)不動産所得に係る課税売上高　課税売上高　7.8%

#define	SEL3104ex2	104		// (3)～所得に係る課税売上高　損益計算書の収入金額　金額
#define	SEL3104Aex2	105		// (3)～所得に係る課税売上高　損益計算書の収入金額　6.24%
#define	SEL3104Bex2	106		// (3)～所得に係る課税売上高　損益計算書の収入金額　7.8%

#define	SEL3105ex2	107		// (3)～所得に係る課税売上高　④のうち、課税売上にならないもの　金額
#define	SEL3105Aex2	108		// (3)～所得に係る課税売上高　④のうち、課税売上にならないもの　6.24%
#define	SEL3105Bex2	109		// (3)～所得に係る課税売上高　④のうち、課税売上にならないもの　7.8%

#define	SEL3106ex2	110		// (3)～所得に係る課税売上高　差引課税売上高(4-5)　金額
#define	SEL3106Aex2	111		// (3)～所得に係る課税売上高　差引課税売上高(4-5)　6.24%
#define	SEL3106Bex2	112		// (3)～所得に係る課税売上高　差引課税売上高(4-5)　7.8%

#define	SEL3107ex2	113		// (4)業務用資産の譲渡所得に係る課税売上高　業務用固定資産等の譲渡収入金額　金額
#define	SEL3107Aex2	114		// (4)業務用資産の譲渡所得に係る課税売上高　業務用固定資産等の譲渡収入金額　6.24%
#define	SEL3107Bex2	115		// (4)業務用資産の譲渡所得に係る課税売上高　業務用固定資産等の譲渡収入金額　7.8%

#define	SEL3108ex2	116		// (4)業務用資産の譲渡所得に係る課税売上高　⑦のうち、課税売上高にならないもの　金額
#define	SEL3108Aex2	117		// (4)業務用資産の譲渡所得に係る課税売上高　⑦のうち、課税売上高にならないもの　6.24%
#define	SEL3108Bex2	118		// (4)業務用資産の譲渡所得に係る課税売上高　⑦のうち、課税売上高にならないもの　7.8%

#define	SEL3109ex2	119		// (4)業務用資産の譲渡所得に係る課税売上高　差引課税売上高(7-8)　金額
#define	SEL3109Aex2	120		// (4)業務用資産の譲渡所得に係る課税売上高　差引課税売上高(7-8)　6.24%
#define	SEL3109Bex2	121		// (4)業務用資産の譲渡所得に係る課税売上高　差引課税売上高(7-8)　7.8%

#define	SEL3110ex2	122		// (5)課税売上高の合計額(1+2+3+6+9)　金額
#define	SEL3110Aex2	123		// (5)課税売上高の合計額(1+2+3+6+9)　6.24%
#define	SEL3110Bex2	124		// (5)課税売上高の合計額(1+2+3+6+9)　7.8%

#define	SEL3111Aex2	131		// (6)課税資産の譲渡等の対価の額の計算　6.24%　仮受消費税
#define	SEL3111Bex2	86		// (6)課税資産の譲渡等の対価の額の計算　6.24%　仮受け消費税加算後
#define	SEL3111Cex2	125		// (6)課税資産の譲渡等の対価の額の計算　6.24%　計算結果

#define	SEL3112Aex2	134		// (6)課税資産の譲渡等の対価の額の計算　7.8%  仮受消費税
#define	SEL3112Bex2	92		// (6)課税資産の譲渡等の対価の額の計算　7.8%　仮受け消費税加算後
#define	SEL3112Cex2	126		// (6)課税資産の譲渡等の対価の額の計算　7.8%　計算結果

// 22-0167,22-0361 cor -->
//#define SADDTEXTSex	138		// 追加文言開始インデックス
//#define SADDTEXTEex	139		// 追加文言終了インデックス
// -----------------------
#define SELChk1ex2	137		// 自動計算チェック	金額
#define SELChk2ex2	138		// 　　　 〃 　　　	うち軽減税率6.24%適用分
#define SELChk3ex2	139		// 　　　 〃 　　　	うち標準税率7.8%適用分
// 22-0167,22-0361 cor <--

// 入力不可・入力可の制御
static SH_ITEMINDEX	Sell31Ex2[] = {
	{ SEL31NAMEex2, 1, 1, 0 },

	{ SEL3101ex2,  0, 1, 0 },
	{ SEL3101Aex2, 0, 1, 0 },
	{ SEL3101Bex2, 0, 1, 0 },

	{ SEL3102ex2,  0, 1, 0 },
	{ SEL3102Aex2, 0, 1, 0 },
	{ SEL3102Bex2, 0, 1, 0 },

	{ SEL3103ex2,  0, 1, 0 },
	{ SEL3103Aex2, 0, 1, 0 },
	{ SEL3103Bex2, 0, 1, 0 },

	{ SEL3104ex2,  1, 1, 0 },
	{ SEL3104Aex2, 1, 1, 0 },
	{ SEL3104Bex2, 1, 1, 0 },

	{ SEL3105ex2,  1, 1, 0 },
	{ SEL3105Aex2, 1, 1, 0 },
	{ SEL3105Bex2, 1, 1, 0 },

	{ SEL3106ex2,  0, 0, 0 },
	{ SEL3106Aex2, 0, 0, 0 },
	{ SEL3106Bex2, 0, 0, 0 },

	{ SEL3107ex2,  0, 1, 0 },
	{ SEL3107Aex2, 0, 1, 0 },
	{ SEL3107Bex2, 0, 1, 0 },

	{ SEL3108ex2,  0, 1, 0 },
	{ SEL3108Aex2, 0, 1, 0 },
	{ SEL3108Bex2, 0, 1, 0 },

	{ SEL3109ex2,  0, 0, 0 },
	{ SEL3109Aex2, 0, 0, 0 },
	{ SEL3109Bex2, 0, 0, 0 },

	{ SEL3110ex2,  0, 0, 0 },
	{ SEL3110Aex2, 0, 0, 0 },
	{ SEL3110Bex2, 0, 0, 0 },

	{ SEL3111Aex2, 0, 1, 0 },
	{ SEL3111Bex2, 0, 1, 0 },
	{ SEL3111Cex2, 0, 1, 0 },

	{ SEL3112Aex2, 0, 1, 0 },
	{ SEL3112Bex2, 0, 1, 0 },
	{ SEL3112Cex2, 0, 1, 0 },

	{ 0, 0, 0, 0 }
};

// 上下フォーカス移動の制御
static	FCSUPDOWN_INFO	FCS_Sell31Ex2[] = {
	{ SEL31NAMEex2 },
	{ SEL3101ex2 },
	{ SEL3102ex2 },
	{ SEL3103ex2 },
	{ SEL3104ex2 },
	{ SEL3105ex2 },
	{ SEL3106ex2 },
	{ SEL3107ex2 },
	{ SEL3108ex2 },
	{ SEL3109ex2 },
	{ SEL3110ex2 },
	{ SEL3111Aex2 },
	{ SEL3111Bex2 },
	{ SEL3111Cex2 },
	{ SEL3112Aex2 },
	{ SEL3112Bex2 },
	{ SEL3112Cex2 },

	{ SEL3101Aex2 },
	{ SEL3102Aex2 },
	{ SEL3103Aex2 },
	{ SEL3104Aex2 },
	{ SEL3105Aex2 },
	{ SEL3106Aex2 },
	{ SEL3107Aex2 },
	{ SEL3108Aex2 },
	{ SEL3109Aex2 },
	{ SEL3110Aex2 },
	/*{ SEL3111Aex },
	{ SEL3112Aex },
	{ SEL3113Aex },*/

	{ SEL3101Bex2 },
	{ SEL3102Bex2 },
	{ SEL3103Bex2 },
	{ SEL3104Bex2 },
	{ SEL3105Bex2 },
	{ SEL3106Bex2 },
	{ SEL3107Bex2 },
	{ SEL3108Bex2 },
	{ SEL3109Bex2 },
	{ SEL3110Bex2 },
	/*{ SEL3111Bex },
	{ SEL3112Bex },
	{ SEL3113Bex },*/

	/*{ SEL3111Bex },
	{ SEL3112Bex },
	{ SEL3113Bex },*/

	{ 0 }
};

// 左右フォーカス移動移動の制御
static	FCSUPDOWN_INFO	FCS_Sell31Ex2_LR[] = {
	{ SEL3101ex2 },
	{ SEL3101Aex2 },
	{ SEL3101Bex2 },
	{ SEL3102ex2 },
	{ SEL3102Aex2 },
	{ SEL3102Bex2 },
	{ SEL3103ex2 },
	{ SEL3103Aex2 },
	{ SEL3103Bex2 },
	{ SEL31NAMEex2 },
	{ SEL3104ex2 },
	{ SEL3104Aex2 },
	{ SEL3104Bex2 },
	{ SEL3105ex2 },
	{ SEL3105Aex2 },
	{ SEL3105Bex2 },
	{ SEL3106ex2 },
	{ SEL3106Aex2 },
	{ SEL3106Bex2 },
	{ SEL3107ex2 },
	{ SEL3107Aex2 },
	{ SEL3107Bex2 },
	{ SEL3108ex2 },
	{ SEL3108Aex2 },
	{ SEL3108Bex2 },
	{ SEL3109ex2 },
	{ SEL3109Aex2 },
	{ SEL3109Bex2 },
	{ SEL3110ex2 },
	{ SEL3110Aex2 },
	{ SEL3110Bex2 },
	{ SEL3111Aex2 },
	{ SEL3111Bex2 },
	{ SEL3111Cex2 },
	{ SEL3112Aex2 },
	{ SEL3112Bex2 },
	{ SEL3112Cex2 },
	{ 0 }
};


// 入力可能項目数
#define	FCS_SEL31CNTex2	38


//清水追加
//#define		KSELLINE1	0
//#define		KSELLINE2	1
//#define		KSELLINE3	2
//#define		KSELLINE4	6
//#define		KSELLINE5	7
//#define		KSELLINE6	8
//#define		KSELLINE7	9
//#define		KSELLINE8	10
//#define		KSELLINE9	11
//#define		KSELLINE10	12
//#define		KSELLINE11	13
//#define		KSELLINE12	14
//#define		KSELLINE13	15
////#define		KSELLINE14	17
////#define		KSELLINE15	16
//#define		KSELLINE14	16
//#define		KSELLINE15	17
//#define		KSELLINE16	18

#define		KSELLINE1	0
#define		KSELLINE2	1
#define		KSELLINE3	2
#define		KSELLINE4	3
#define		KSELLINE5	4
#define		KSELLINE6	5
#define		KSELLINE7	6
#define		KSELLINE8	7
#define		KSELLINE9	8
#define		KSELLINE10	9
#define		KSELLINE11	10
#define		KSELLINE12	11