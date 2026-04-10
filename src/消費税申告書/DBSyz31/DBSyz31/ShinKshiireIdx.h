#pragma once


#define ID_KSHIIRE_TITLE1	116		//	事業の区分ごとの計算　1
#define ID_KSHIIRE_TITLE2	117		//	事業の区分ごとの計算　2

#define ID_KSHIIRE_1_1		118		//	１.課税資産の譲渡等（免税取引及び旧税率(6.3%等)が適用される取引は除く。）の税込み価格の合計額　1
#define ID_KSHIIRE_1_2		119		//	１.課税資産の譲渡等（免税取引及び旧税率(6.3%等)が適用される取引は除く。）の税込み価格の合計額　2

#define ID_KSHIIRE_2_1		120		//	２.軽減対象資産の譲渡等(6.24%適用分)の税率価額の合計額　1
#define ID_KSHIIRE_2_2		121		//	２.軽減対象資産の譲渡等(6.24%適用分)の税率価額の合計額　2

#define ID_KSHIIRE_3_1		122		//	３.小売等軽減売上割合（2/1）　1
#define ID_KSHIIRE_3_2		123		//	３.小売等軽減売上割合（2/1）　2

#define ID_KSHIIRE_4_1		124		//	４.課税仕入れに係る支払対価の額（税込み）　1
#define ID_KSHIIRE_4_2		125		//	４.課税仕入れに係る支払対価の額（税込み）　2

#define ID_KSHIIRE_5_1		126		//	５.保税地域から引き取った課税貨物に係る税込引取価額　1
#define ID_KSHIIRE_5_2		127		//	５.保税地域から引き取った課税貨物に係る税込引取価額　2

#define ID_KSHIIRE_6_1		128		//	６.課税仕入れに係る支払対価の額等の合計額　1
#define ID_KSHIIRE_6_2		129		//	６.課税仕入れに係る支払対価の額等の合計額　2

#define ID_KSHIIRE_7_1		130		//	７.軽減対象資産に係る課税仕入れ等(税率6.24%適用分)の税額　1
#define ID_KSHIIRE_7_2		131		//	７.軽減対象資産に係る課税仕入れ等(税率6.24%適用分)の税額　2
#define ID_KSHIIRE_7_KEI	132		//	７.軽減対象資産に係る課税仕入れ等(税率6.24%適用分)の税額　3

#define ID_KSHIIRE_8_1		133		//	８.軽減対象資産に係る課税仕入れ等以外の課税仕入れ等(税率7.8%適用分)の税額　1
#define ID_KSHIIRE_8_2		134		//	８.軽減対象資産に係る課税仕入れ等以外の課税仕入れ等(税率7.8%適用分)の税額　2
#define ID_KSHIIRE_8_KEI	135		//	８.軽減対象資産に係る課税仕入れ等以外の課税仕入れ等(税率7.8%適用分)の税額　3

#define ID_KSHIIRE_9		136		//	９.納税義務の免税を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額　税率6.24%適用分

#define ID_KSHIIRE_10		137		//	１０.納税義務の免税を受けない(受ける)こととなった場合における消費税額の調整(加算又は減算)額　税率7.8%適用分

#define ID_KSHIIRE_11_1		138		//	１１.課税仕入れに係る支払対価の額（税込み）　税率6.24%適用分
#define ID_KSHIIRE_11_2		139		//	１１.課税仕入れに係る支払対価の額（税込み）　税率7.8%適用分

#define ID_KSHIIRE_12_1		140		//	１２.課税仕入れに係る消費税額　税率6.24%適用分
#define ID_KSHIIRE_12_2		141		//	１２.課税仕入れに係る消費税額　税率7.8%適用分

#define ID_KSHIIRE_13_2		142		//	１３.特定課税仕入れに係る支払対価の額　税率7.8%適用分

#define ID_KSHIIRE_14_2		143		//	１４.特定課税仕入れに係る消費税額　税率7.8%適用分

#define ID_KSHIIRE_15_1		144		//	１５.課税貨物に係る消費税額　税率6.24%適用分
#define ID_KSHIIRE_15_2		145		//	１５.課税貨物に係る消費税額　税率7.8%適用分

#define ID_KSHIIRE_16_1		146		//	１６.納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整（加算又は減算）額　税率6.24%適用分
#define ID_KSHIIRE_16_2		147		//	１６.納税義務の免除を受けない(受ける)こととなった場合における消費税額の調整（加算又は減算）額　税率7.8%適用分

#define ID_KSHIIRE_17		148		//	１７.課税仕入れ等の税額の合計額　税率6.24%適用分

#define ID_KSHIIRE_18		149		//	１８.課税仕入れ等の税額の合計額　税率7.8%適用分

#define ID_KSHIIRE_19		150		//	１９.軽減対象資産に係る課税仕入れ等(税率6.24%適用分)の税額の合計額

#define ID_KSHIIRE_20		151		//	２０.軽減対象資産に係る課税仕入れ等以外の課税仕入れ等(税率7.8%適用分)の税額の合計額

#define ID_KSHIIRE_NO1		155		//	列番号１
#define ID_KSHIIRE_NO2		156		//	列番号２


static TOKUREI_ITEMINDEX Kshiire[] = {

	{ ID_KSHIIRE_TITLE1,   1, 1, 1, 0 },
	{ ID_KSHIIRE_TITLE2,   0, 1, 1, 0 },

	{ ID_KSHIIRE_1_1,      0, 1, 1, 0 },
	{ ID_KSHIIRE_1_2,      0, 1, 1, 0 },

	{ ID_KSHIIRE_2_1,      0, 1, 1, 0 },
	{ ID_KSHIIRE_2_2,      0, 1, 1, 0 },

	{ ID_KSHIIRE_3_1,      0, 0, 0, 0 },
	{ ID_KSHIIRE_3_2,      0, 0, 0, 0 },

	{ ID_KSHIIRE_4_1,      0, 1, 1, 0 },
	{ ID_KSHIIRE_4_2,      0, 1, 1, 0 },

	{ ID_KSHIIRE_5_1,      0, 1, 1, 0 },
	{ ID_KSHIIRE_5_2,      0, 1, 1, 0 },

	{ ID_KSHIIRE_6_1,      0, 0, 0, 0 },
	{ ID_KSHIIRE_6_2,      0, 0, 0, 0 },

	{ ID_KSHIIRE_7_1,      0, 1, 1, 0 },
	{ ID_KSHIIRE_7_2,      0, 1, 1, 0 },
	{ ID_KSHIIRE_7_KEI,    0, 0, 0, 0 },

	{ ID_KSHIIRE_8_1,      0, 1, 1, 0 },
	{ ID_KSHIIRE_8_2,      0, 1, 1, 0 },
	{ ID_KSHIIRE_8_KEI,    0, 0, 0, 0 },

	{ ID_KSHIIRE_9,        0, 1, 1, 0 },
	{ ID_KSHIIRE_10,       0, 1, 1, 0 },

	{ ID_KSHIIRE_11_1,     0, 1, 1, 0 },
	{ ID_KSHIIRE_11_2,     0, 1, 1, 0 },

	{ ID_KSHIIRE_12_1,     0, 1, 1, 0 },
	{ ID_KSHIIRE_12_2,     0, 1, 1, 0 },

	{ ID_KSHIIRE_13_2,     0, 1, 1, 0 },

	{ ID_KSHIIRE_14_2,     0, 1, 1, 0 },

	{ ID_KSHIIRE_15_1,     0, 1, 1, 0 },
	{ ID_KSHIIRE_15_2,     0, 1, 1, 0 },

	{ ID_KSHIIRE_16_1,     0, 1, 1, 0 },
	{ ID_KSHIIRE_16_2,     0, 1, 1, 0 },

	{ ID_KSHIIRE_17,       0, 0, 0, 0 },
	{ ID_KSHIIRE_18,       0, 0, 0, 0 },

	{ ID_KSHIIRE_19,       0, 0, 0, 0 },

	{ ID_KSHIIRE_20,       0, 0, 0, 0 },

	{             0,       0, 0, 0, 0 }
};

static FCSUPDOWN_INFO FCS_Kshiire[] = {

	{ ID_KSHIIRE_TITLE1 },
	{ ID_KSHIIRE_1_1 },
	{ ID_KSHIIRE_2_1 },
	{ ID_KSHIIRE_3_1 },
	{ ID_KSHIIRE_4_1 },
	{ ID_KSHIIRE_5_1 },
	{ ID_KSHIIRE_6_1 },
	{ ID_KSHIIRE_7_1 },
	{ ID_KSHIIRE_8_1 },

	{ ID_KSHIIRE_TITLE2 },
	{ ID_KSHIIRE_1_2 },
	{ ID_KSHIIRE_2_2 },
	{ ID_KSHIIRE_3_2 },
	{ ID_KSHIIRE_4_2 },
	{ ID_KSHIIRE_5_2 },
	{ ID_KSHIIRE_6_2 },
	{ ID_KSHIIRE_7_2 },
	{ ID_KSHIIRE_8_2 },

	{ ID_KSHIIRE_7_KEI },
	{ ID_KSHIIRE_8_KEI },

	{ ID_KSHIIRE_9 },
	{ ID_KSHIIRE_10 },

	{ ID_KSHIIRE_11_1 },
	{ ID_KSHIIRE_12_1 },
	{ ID_KSHIIRE_15_1 },
	{ ID_KSHIIRE_16_1 },
	{ ID_KSHIIRE_17 },

	{ ID_KSHIIRE_11_2 },
	{ ID_KSHIIRE_12_2 },
	{ ID_KSHIIRE_13_2 },
	{ ID_KSHIIRE_14_2 },
	{ ID_KSHIIRE_15_2 },
	{ ID_KSHIIRE_16_2 },
	{ ID_KSHIIRE_18 },

	{ ID_KSHIIRE_19 },
	{ ID_KSHIIRE_20 }

};

enum {
	MOVE_DOWN,
	MOVE_UP
};

#define KSHIIRE_DIAGCTRLID_NAME       5
#define KSHIIRE_DIAGCTRLID_KAZEI_S    6
#define KSHIIRE_DIAGCTRLID_KAZEI_E    7
#define KSHIIRE_DIAGCTRLID_TEKIYO_S   8
#define KSHIIRE_DIAGCTRLID_TEKIYO_E   9
#define KSHIIRE_DIAGCTRLID_MAX      151

#define KSHIIRE_PRTPAGE_MAX           5
#define JIGYO_PER_PAGE_SHIIRE 2

int idx_kshiire_title[] = { ID_KSHIIRE_TITLE1, ID_KSHIIRE_TITLE2 };
int idx_kshiire_1[] = { ID_KSHIIRE_1_1, ID_KSHIIRE_1_2 };
int idx_kshiire_2[] = { ID_KSHIIRE_2_1, ID_KSHIIRE_2_2 };
int idx_kshiire_3[] = { ID_KSHIIRE_3_1, ID_KSHIIRE_3_2 };
int idx_kshiire_4[] = { ID_KSHIIRE_4_1, ID_KSHIIRE_4_2 };
int idx_kshiire_5[] = { ID_KSHIIRE_5_1, ID_KSHIIRE_5_2 };
int idx_kshiire_6[] = { ID_KSHIIRE_6_1, ID_KSHIIRE_6_2 };
int idx_kshiire_7[] = { ID_KSHIIRE_7_1, ID_KSHIIRE_7_2 };
int idx_kshiire_8[] = { ID_KSHIIRE_8_1, ID_KSHIIRE_8_2 };

int idx_kshiire_7_kei[] = { ID_KSHIIRE_7_KEI };
int idx_kshiire_8_kei[] = { ID_KSHIIRE_8_KEI };
int idx_kshiire_9[] = { ID_KSHIIRE_9 };
int idx_kshiire_10[] = { ID_KSHIIRE_10 };
int idx_kshiire_11_1[] = { ID_KSHIIRE_11_1 };
int idx_kshiire_11_2[] = { ID_KSHIIRE_11_2 };
int idx_kshiire_12_1[] = { ID_KSHIIRE_12_1 };
int idx_kshiire_12_2[] = { ID_KSHIIRE_12_2 };
int idx_kshiire_13_2[] = { ID_KSHIIRE_13_2 };
int idx_kshiire_14_2[] = { ID_KSHIIRE_14_2 };
int idx_kshiire_15_1[] = { ID_KSHIIRE_15_1 };
int idx_kshiire_15_2[] = { ID_KSHIIRE_15_2 };
int idx_kshiire_16_1[] = { ID_KSHIIRE_16_1 };
int idx_kshiire_16_2[] = { ID_KSHIIRE_16_2 };
int idx_kshiire_17[] = { ID_KSHIIRE_17 };
int idx_kshiire_18[] = { ID_KSHIIRE_18 };
int idx_kshiire_19[] = { ID_KSHIIRE_19 };
int idx_kshiire_20[] = { ID_KSHIIRE_20 };
int idx_kshiire_dummy[] = { -1 };

int idx_kshiire_row1[] = { ID_KSHIIRE_TITLE1,
						   ID_KSHIIRE_1_1,
						   ID_KSHIIRE_2_1,
						   ID_KSHIIRE_3_1,
						   ID_KSHIIRE_4_1,
						   ID_KSHIIRE_5_1,
						   ID_KSHIIRE_6_1,
						   ID_KSHIIRE_7_1,
						   ID_KSHIIRE_8_1,
                           ID_KSHIIRE_9 };

int idx_kshiire_row2[] = { ID_KSHIIRE_TITLE2,
						   ID_KSHIIRE_1_2,
						   ID_KSHIIRE_2_2,
						   ID_KSHIIRE_3_2,
						   ID_KSHIIRE_4_2,
						   ID_KSHIIRE_5_2,
						   ID_KSHIIRE_6_2,
						   ID_KSHIIRE_7_2,
						   ID_KSHIIRE_8_2 };

int idx_kshiireRend_next[] = { ID_KSHIIRE_1_1,
							  ID_KSHIIRE_2_1,
							  ID_KSHIIRE_3_1,
							  ID_KSHIIRE_4_1,
							  ID_KSHIIRE_5_1,
							  ID_KSHIIRE_6_1,
							  ID_KSHIIRE_7_1,
							  ID_KSHIIRE_8_1,
							  ID_KSHIIRE_9 };

int idx_kshiireLend_next[] = { ID_KSHIIRE_20,
							  ID_KSHIIRE_TITLE1,
							  ID_KSHIIRE_1_2,
							  ID_KSHIIRE_2_2,
							  ID_KSHIIRE_3_2,
							  ID_KSHIIRE_4_2,
							  ID_KSHIIRE_5_2,
							  ID_KSHIIRE_6_2,
							  ID_KSHIIRE_7_KEI,
							  ID_KSHIIRE_8_KEI };
