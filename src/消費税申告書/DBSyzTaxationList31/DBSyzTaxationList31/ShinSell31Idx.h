#pragma once

#define		SKLINCNT	13//19	// 帳票の行数。この行数でデータを保存していってる
#define		SKVMIN		127		// 入力ボックスの最小のインデックス
#define		SKVMAX		169		// 入力ボックスの最大のインデックス

#define		SKYEARID	1		// 令和～年分
#define		SKZKBNID	2		// 経理方式
#define		SKDISP1		29		// ?

#define		Msiz		512*20	// 印刷関連
#define		fno1_s		3		// フィールド１の小さいフォントフィールド番号　いらんかも


// 入力項目のindex
#define	SEL31NAME	173		// (3)～所得に係る課税売上高

#define	SEL3101		127		// (1)事業所得に係る課税売上高　営業課税売上高　金額
#define	SEL3101A	128		// (1)事業所得に係る課税売上高　営業課税売上高　6.3%
#define	SEL3101B	129		// (1)事業所得に係る課税売上高　営業課税売上高　6.24%
#define	SEL3101C	130		// (1)事業所得に係る課税売上高　営業課税売上高　7.8%

#define	SEL3102		131		// (1)事業所得に係る課税売上高　農業課税売上高　金額
#define	SEL3102A	132		// (1)事業所得に係る課税売上高　農業課税売上高　6.3%
#define	SEL3102B	133		// (1)事業所得に係る課税売上高　農業課税売上高　6.24%
#define	SEL3102C	134		// (1)事業所得に係る課税売上高　農業課税売上高　7.8%

#define	SEL3103		135		// (2)不動産所得に係る課税売上高　課税売上高　金額
#define	SEL3103A	136		// (2)不動産所得に係る課税売上高　課税売上高　6.3%
#define	SEL3103B	137		// (2)不動産所得に係る課税売上高　課税売上高　6.24%
#define	SEL3103C	138		// (2)不動産所得に係る課税売上高　課税売上高　7.8%

#define	SEL3104		139		// (3)～所得に係る課税売上高　損益計算書の収入金額　金額
#define	SEL3104A	140		// (3)～所得に係る課税売上高　損益計算書の収入金額　6.3%
#define	SEL3104B	141		// (3)～所得に係る課税売上高　損益計算書の収入金額　6.24%
#define	SEL3104C	142		// (3)～所得に係る課税売上高　損益計算書の収入金額　7.8%

#define	SEL3105		143		// (3)～所得に係る課税売上高　④のうち、課税売上にならないもの　金額
#define	SEL3105A	144		// (3)～所得に係る課税売上高　④のうち、課税売上にならないもの　6.3%
#define	SEL3105B	145		// (3)～所得に係る課税売上高　④のうち、課税売上にならないもの　6.24%
#define	SEL3105C	146		// (3)～所得に係る課税売上高　④のうち、課税売上にならないもの　7.8%

#define	SEL3106		147		// (3)～所得に係る課税売上高　差引課税売上高(4-5)　金額
#define	SEL3106A	148		// (3)～所得に係る課税売上高　差引課税売上高(4-5)　6.3%
#define	SEL3106B	149		// (3)～所得に係る課税売上高　差引課税売上高(4-5)　6.24%
#define	SEL3106C	150		// (3)～所得に係る課税売上高　差引課税売上高(4-5)　7.8%

#define	SEL3107		151		// (4)業務用資産の譲渡所得に係る課税売上高　業務用固定資産等の譲渡収入金額　金額
#define	SEL3107A	152		// (4)業務用資産の譲渡所得に係る課税売上高　業務用固定資産等の譲渡収入金額　6.3%
#define	SEL3107B	153		// (4)業務用資産の譲渡所得に係る課税売上高　業務用固定資産等の譲渡収入金額　6.24%
#define	SEL3107C	154		// (4)業務用資産の譲渡所得に係る課税売上高　業務用固定資産等の譲渡収入金額　7.8%

#define	SEL3108		155		// (4)業務用資産の譲渡所得に係る課税売上高　⑦のうち、課税売上高にならないもの　金額
#define	SEL3108A	156		// (4)業務用資産の譲渡所得に係る課税売上高　⑦のうち、課税売上高にならないもの　6.3%
#define	SEL3108B	157		// (4)業務用資産の譲渡所得に係る課税売上高　⑦のうち、課税売上高にならないもの　6.24%
#define	SEL3108C	158		// (4)業務用資産の譲渡所得に係る課税売上高　⑦のうち、課税売上高にならないもの　7.8%

#define	SEL3109		159		// (4)業務用資産の譲渡所得に係る課税売上高　差引課税売上高(7-8)　金額
#define	SEL3109A	160		// (4)業務用資産の譲渡所得に係る課税売上高　差引課税売上高(7-8)　6.3%
#define	SEL3109B	161		// (4)業務用資産の譲渡所得に係る課税売上高　差引課税売上高(7-8)　6.24%
#define	SEL3109C	162		// (4)業務用資産の譲渡所得に係る課税売上高　差引課税売上高(7-8)　7.8%

#define	SEL3110		163		// (5)課税売上高の合計額(1+2+3+6+9)　金額
#define	SEL3110A	164		// (5)課税売上高の合計額(1+2+3+6+9)　6.3%
#define	SEL3110B	165		// (5)課税売上高の合計額(1+2+3+6+9)　6.24%
#define	SEL3110C	166		// (5)課税売上高の合計額(1+2+3+6+9)　7.8%

#define	SEL3111A	175		// (6)課税資産の譲渡等の対価の額の計算　6.3%　仮受消費税
#define	SEL3111B	111		// (6)課税資産の譲渡等の対価の額の計算　6.3%　仮受け消費税加算後
#define SEL3111C	167		// (6)課税資産の譲渡等の対価の額の計算　6.3%　計算結果

#define	SEL3112A	178		// (6)課税資産の譲渡等の対価の額の計算　6.24% 仮受消費税
#define	SEL3112B	118		// (6)課税資産の譲渡等の対価の額の計算　6.24%　仮受け消費税加算後
#define SEL3112C	168		// (6)課税資産の譲渡等の対価の額の計算　6.24%　計算結果

#define	SEL3113A	181		// (6)課税資産の譲渡等の対価の額の計算　7.8%　仮受消費税
#define	SEL3113B	124		// (6)課税資産の譲渡等の対価の額の計算　7.8%　仮受け消費税加算後
#define SEL3113C	169		// (6)課税資産の譲渡等の対価の額の計算　7.8%　計算結果


#define SADDTEXTS	184		// 追加文言開始インデックス
#define SADDTEXTE	185		// 追加文言終了インデックス

// 入力不可・入力可の制御
static SH_ITEMINDEX	Sell31[] = {
	{ SEL31NAME, 1, 1, 0 },

	{ SEL3101,	0, 1, 0 },
	{ SEL3101A, 0, 2, 0 },
	{ SEL3101B, 0, 1, 0 },
	{ SEL3101C, 0, 1, 0 },

	{ SEL3102,	0, 1, 0 },
	{ SEL3102A, 0, 2, 0 },
	{ SEL3102B, 0, 1, 0 },
	{ SEL3102C, 0, 1, 0 },

	{ SEL3103,  0, 1, 0 },
	{ SEL3103A,	0, 2, 0 },
	{ SEL3103B, 0, 1, 0 },
	{ SEL3103C, 0, 1, 0 },

	{ SEL3104,  1, 1, 0 },
	{ SEL3104A,	2, 2, 0 },
	{ SEL3104B, 1, 1, 0 },
	{ SEL3104C, 1, 1, 0 },

	{ SEL3105,  1, 1, 0 },
	{ SEL3105A,	2, 2, 0 },
	{ SEL3105B, 1, 1, 0 },
	{ SEL3105C, 1, 1, 0 },

	{ SEL3106,  0, 0, 0 },
	{ SEL3106A,	0, 0, 0 },
	{ SEL3106B, 0, 0, 0 },
	{ SEL3106C, 0, 0, 0 },

	{ SEL3107,  0, 1, 0 },
	{ SEL3107A,	0, 2, 0 },
	{ SEL3107B, 0, 1, 0 },
	{ SEL3107C, 0, 1, 0 },

	{ SEL3108,  0, 1, 0 },
	{ SEL3108A,	0, 2, 0 },
	{ SEL3108B, 0, 1, 0 },
	{ SEL3108C, 0, 1, 0 },

	{ SEL3109,  0, 0, 0 },
	{ SEL3109A,	0, 0, 0 },
	{ SEL3109B, 0, 0, 0 },
	{ SEL3109C, 0, 0, 0 },

	{ SEL3110,  0, 0, 0 },
	{ SEL3110A,	0, 0, 0 },
	{ SEL3110B, 0, 0, 0 },
	{ SEL3110C, 0, 0, 0 },

	{ SEL3111A, 0, 2, 0 },
	{ SEL3111B,	0, 2, 0 },
	{ SEL3111C,	0, 2, 0 },

	{ SEL3112A, 0, 1, 0 },
	{ SEL3112B, 0, 1, 0 },
	{ SEL3112C, 0, 1, 0 },

	{ SEL3113A, 0, 1, 0 },
	{ SEL3113B,	0, 1, 0 },
	{ SEL3113C,	0, 1, 0 },

	{ 0, 0, 0, 0 }
};

// 上下フォーカス移動の制御
static	FCSUPDOWN_INFO	FCS_Sell31[] = {
	{ SEL31NAME },
	{ SEL3101 },
	{ SEL3102 },
	{ SEL3103 },
	{ SEL3104 },
	{ SEL3105 },
	{ SEL3106 },
	{ SEL3107 },
	{ SEL3108 },
	{ SEL3109 },
	{ SEL3110 },
	{ SEL3111A },
	{ SEL3111B },
	{ SEL3111C },
	{ SEL3112A },
	{ SEL3112B },
	{ SEL3112C },
	{ SEL3113A },
	{ SEL3113B },
	{ SEL3113C },

	{ SEL3101A },
	{ SEL3102A },
	{ SEL3103A },
	{ SEL3104A },
	{ SEL3105A },
	{ SEL3106A },
	{ SEL3107A },
	{ SEL3108A },
	{ SEL3109A },
	{ SEL3110A },
	/*{ SEL3111A },
	{ SEL3112A },
	{ SEL3113A },*/

	{ SEL3101B },
	{ SEL3102B },
	{ SEL3103B },
	{ SEL3104B },
	{ SEL3105B },
	{ SEL3106B },
	{ SEL3107B },
	{ SEL3108B },
	{ SEL3109B },
	{ SEL3110B },
	/*{ SEL3111B },
	{ SEL3112B },
	{ SEL3113B },*/

	{ SEL3101C },
	{ SEL3102C },
	{ SEL3103C },
	{ SEL3104C },
	{ SEL3105C },
	{ SEL3106C },
	{ SEL3107C },
	{ SEL3108C },
	{ SEL3109C },
	{ SEL3110C },
	/*{ SEL3111B },
	{ SEL3112B },
	{ SEL3113B },*/

	{ 0 }
};

// 左右フォーカス移動移動の制御
static	FCSUPDOWN_INFO	FCS_Sell31_LR[] = {
	{ SEL3101 },
	{ SEL3101A },
	{ SEL3101B },
	{ SEL3101C },
	{ SEL3102 },
	{ SEL3102A },
	{ SEL3102B },
	{ SEL3102C },
	{ SEL3103 },
	{ SEL3103A },
	{ SEL3103B },
	{ SEL3103C },
	{ SEL31NAME },
	{ SEL3104 },
	{ SEL3104A },
	{ SEL3104B },
	{ SEL3104C },
	{ SEL3105 },
	{ SEL3105A },
	{ SEL3105B },
	{ SEL3105C },
	{ SEL3106 },
	{ SEL3106A },
	{ SEL3106B },
	{ SEL3106C },
	{ SEL3107 },
	{ SEL3107A },
	{ SEL3107B },
	{ SEL3107C },
	{ SEL3108 },
	{ SEL3108A },
	{ SEL3108B },
	{ SEL3108C },
	{ SEL3109 },
	{ SEL3109A },
	{ SEL3109B },
	{ SEL3109C },
	{ SEL3110 },
	{ SEL3110A },
	{ SEL3110B },
	{ SEL3110C },
	{ SEL3111A },
	{ SEL3111B },
	{ SEL3111C },
	{ SEL3112A },
	{ SEL3112B },
	{ SEL3112C },
	{ SEL3113A },
	{ SEL3113B },
	{ SEL3113C },
	{ 0 }
};


// 入力可能項目数
#define	FCS_SEL31CNT	50


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
#define		KSELLINE13	12
#define		KSELLINE14	13
#define		KSELLINE15	14
#define		KSELLINE16	15