#pragma once

#define		SKLINCNTex	12		// 帳票の行数。この行数でデータを保存していってる
#define		SKVMINex	95		// 入力ボックスの最小のインデックス
#define		SKVMAXex	126		// 入力ボックスの最大のインデックス

#define		SKYEARIDex	1		// 令和～年分
#define		SKZKBNIDex	2		// 経理方式
#define		SKDISP1ex	29		// ?

#define		Msizex		512*20	// 印刷関連
#define		fno1_sex	3		// フィールド１の小さいフォントフィールド番号　いらんかも


// 入力項目のindex
#define	SEL31NAMEex	129		// (3)～所得に係る課税売上高

#define	SEL3101ex	95		// (1)事業所得に係る課税売上高　営業課税売上高　金額
#define	SEL3101Aex	96		// (1)事業所得に係る課税売上高　営業課税売上高　6.24%
#define	SEL3101Bex	97		// (1)事業所得に係る課税売上高　営業課税売上高　7.8%

#define	SEL3102ex	98		// (1)事業所得に係る課税売上高　農業課税売上高　金額
#define	SEL3102Aex	99		// (1)事業所得に係る課税売上高　農業課税売上高　6.24%
#define	SEL3102Bex	100		// (1)事業所得に係る課税売上高　農業課税売上高　7.8%

#define	SEL3103ex	101		// (2)不動産所得に係る課税売上高　課税売上高　金額
#define	SEL3103Aex	102		// (2)不動産所得に係る課税売上高　課税売上高　6.24%
#define	SEL3103Bex	103		// (2)不動産所得に係る課税売上高　課税売上高　7.8%

#define	SEL3104ex	104		// (3)～所得に係る課税売上高　損益計算書の収入金額　金額
#define	SEL3104Aex	105		// (3)～所得に係る課税売上高　損益計算書の収入金額　6.24%
#define	SEL3104Bex	106		// (3)～所得に係る課税売上高　損益計算書の収入金額　7.8%

#define	SEL3105ex	107		// (3)～所得に係る課税売上高　④のうち、課税売上にならないもの　金額
#define	SEL3105Aex	108		// (3)～所得に係る課税売上高　④のうち、課税売上にならないもの　6.24%
#define	SEL3105Bex	109		// (3)～所得に係る課税売上高　④のうち、課税売上にならないもの　7.8%

#define	SEL3106ex	110		// (3)～所得に係る課税売上高　差引課税売上高(4-5)　金額
#define	SEL3106Aex	111		// (3)～所得に係る課税売上高　差引課税売上高(4-5)　6.24%
#define	SEL3106Bex	112		// (3)～所得に係る課税売上高　差引課税売上高(4-5)　7.8%

#define	SEL3107ex	113		// (4)業務用資産の譲渡所得に係る課税売上高　業務用固定資産等の譲渡収入金額　金額
#define	SEL3107Aex	114		// (4)業務用資産の譲渡所得に係る課税売上高　業務用固定資産等の譲渡収入金額　6.24%
#define	SEL3107Bex	115		// (4)業務用資産の譲渡所得に係る課税売上高　業務用固定資産等の譲渡収入金額　7.8%

#define	SEL3108ex	116		// (4)業務用資産の譲渡所得に係る課税売上高　⑦のうち、課税売上高にならないもの　金額
#define	SEL3108Aex	117		// (4)業務用資産の譲渡所得に係る課税売上高　⑦のうち、課税売上高にならないもの　6.24%
#define	SEL3108Bex	118		// (4)業務用資産の譲渡所得に係る課税売上高　⑦のうち、課税売上高にならないもの　7.8%

#define	SEL3109ex	119		// (4)業務用資産の譲渡所得に係る課税売上高　差引課税売上高(7-8)　金額
#define	SEL3109Aex	120		// (4)業務用資産の譲渡所得に係る課税売上高　差引課税売上高(7-8)　6.24%
#define	SEL3109Bex	121		// (4)業務用資産の譲渡所得に係る課税売上高　差引課税売上高(7-8)　7.8%

#define	SEL3110ex	122		// (5)課税売上高の合計額(1+2+3+6+9)　金額
#define	SEL3110Aex	123		// (5)課税売上高の合計額(1+2+3+6+9)　6.24%
#define	SEL3110Bex	124		// (5)課税売上高の合計額(1+2+3+6+9)　7.8%

#define	SEL3111Aex	131		// (6)課税資産の譲渡等の対価の額の計算　6.24%　仮受消費税
#define	SEL3111Bex	86		// (6)課税資産の譲渡等の対価の額の計算　6.24%　仮受け消費税加算後
#define	SEL3111Cex	125		// (6)課税資産の譲渡等の対価の額の計算　6.24%　計算結果

#define	SEL3112Aex	134		// (6)課税資産の譲渡等の対価の額の計算　7.8%  仮受消費税
#define	SEL3112Bex	92		// (6)課税資産の譲渡等の対価の額の計算　7.8%　仮受け消費税加算後
#define	SEL3112Cex	126		// (6)課税資産の譲渡等の対価の額の計算　7.8%　計算結果

// 22-0167,22-0361 cor -->
//#define SADDTEXTSex	138		// 追加文言開始インデックス
//#define SADDTEXTEex	139		// 追加文言終了インデックス
// -----------------------
#define SELChk1ex	137		// 自動計算チェック	金額
#define SELChk2ex	138		// 　　　 〃 　　　	うち軽減税率6.24%適用分
#define SELChk3ex	139		// 　　　 〃 　　　	うち標準税率7.8%適用分
// 22-0167,22-0361 cor <--

// 入力不可・入力可の制御
static SH_ITEMINDEX	Sell31Ex[] = {
	{ SEL31NAMEex, 1, 1, 0 },

	{ SEL3101ex,  0, 1, 0 },
	{ SEL3101Aex, 0, 1, 0 },
	{ SEL3101Bex, 0, 1, 0 },

	{ SEL3102ex,  0, 1, 0 },
	{ SEL3102Aex, 0, 1, 0 },
	{ SEL3102Bex, 0, 1, 0 },

	{ SEL3103ex,  0, 1, 0 },
	{ SEL3103Aex, 0, 1, 0 },
	{ SEL3103Bex, 0, 1, 0 },

	{ SEL3104ex,  1, 1, 0 },
	{ SEL3104Aex, 1, 1, 0 },
	{ SEL3104Bex, 1, 1, 0 },

	{ SEL3105ex,  1, 1, 0 },
	{ SEL3105Aex, 1, 1, 0 },
	{ SEL3105Bex, 1, 1, 0 },

	{ SEL3106ex,  0, 0, 0 },
	{ SEL3106Aex, 0, 0, 0 },
	{ SEL3106Bex, 0, 0, 0 },

	{ SEL3107ex,  0, 1, 0 },
	{ SEL3107Aex, 0, 1, 0 },
	{ SEL3107Bex, 0, 1, 0 },

	{ SEL3108ex,  0, 1, 0 },
	{ SEL3108Aex, 0, 1, 0 },
	{ SEL3108Bex, 0, 1, 0 },

	{ SEL3109ex,  0, 0, 0 },
	{ SEL3109Aex, 0, 0, 0 },
	{ SEL3109Bex, 0, 0, 0 },

	{ SEL3110ex,  0, 0, 0 },
	{ SEL3110Aex, 0, 0, 0 },
	{ SEL3110Bex, 0, 0, 0 },

	{ SEL3111Aex, 0, 1, 0 },
	{ SEL3111Bex, 0, 1, 0 },
	{ SEL3111Cex, 0, 1, 0 },

	{ SEL3112Aex, 0, 1, 0 },
	{ SEL3112Bex, 0, 1, 0 },
	{ SEL3112Cex, 0, 1, 0 },

	{ 0, 0, 0, 0 }
};

// 上下フォーカス移動の制御
static	FCSUPDOWN_INFO	FCS_Sell31Ex[] = {
	{ SEL31NAMEex },
	{ SEL3101ex },
	{ SEL3102ex },
	{ SEL3103ex },
	{ SEL3104ex },
	{ SEL3105ex },
	{ SEL3106ex },
	{ SEL3107ex },
	{ SEL3108ex },
	{ SEL3109ex },
	{ SEL3110ex },
	{ SEL3111Aex },
	{ SEL3111Bex },
	{ SEL3111Cex },
	{ SEL3112Aex },
	{ SEL3112Bex },
	{ SEL3112Cex },

	{ SEL3101Aex },
	{ SEL3102Aex },
	{ SEL3103Aex },
	{ SEL3104Aex },
	{ SEL3105Aex },
	{ SEL3106Aex },
	{ SEL3107Aex },
	{ SEL3108Aex },
	{ SEL3109Aex },
	{ SEL3110Aex },
	/*{ SEL3111Aex },
	{ SEL3112Aex },
	{ SEL3113Aex },*/

	{ SEL3101Bex },
	{ SEL3102Bex },
	{ SEL3103Bex },
	{ SEL3104Bex },
	{ SEL3105Bex },
	{ SEL3106Bex },
	{ SEL3107Bex },
	{ SEL3108Bex },
	{ SEL3109Bex },
	{ SEL3110Bex },
	/*{ SEL3111Bex },
	{ SEL3112Bex },
	{ SEL3113Bex },*/

	/*{ SEL3111Bex },
	{ SEL3112Bex },
	{ SEL3113Bex },*/

	{ 0 }
};

// 左右フォーカス移動移動の制御
static	FCSUPDOWN_INFO	FCS_Sell31Ex_LR[] = {
	{ SEL3101ex },
	{ SEL3101Aex },
	{ SEL3101Bex },
	{ SEL3102ex },
	{ SEL3102Aex },
	{ SEL3102Bex },
	{ SEL3103ex },
	{ SEL3103Aex },
	{ SEL3103Bex },
	{ SEL31NAMEex },
	{ SEL3104ex },
	{ SEL3104Aex },
	{ SEL3104Bex },
	{ SEL3105ex },
	{ SEL3105Aex },
	{ SEL3105Bex },
	{ SEL3106ex },
	{ SEL3106Aex },
	{ SEL3106Bex },
	{ SEL3107ex },
	{ SEL3107Aex },
	{ SEL3107Bex },
	{ SEL3108ex },
	{ SEL3108Aex },
	{ SEL3108Bex },
	{ SEL3109ex },
	{ SEL3109Aex },
	{ SEL3109Bex },
	{ SEL3110ex },
	{ SEL3110Aex },
	{ SEL3110Bex },
	{ SEL3111Aex },
	{ SEL3111Bex },
	{ SEL3111Cex },
	{ SEL3112Aex },
	{ SEL3112Bex },
	{ SEL3112Cex },
	{ 0 }
};


// 入力可能項目数
#define	FCS_SEL31CNTex	38


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