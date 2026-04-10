#pragma once


#define ID_KSURI2_TITLE1	 85		//	事業の区分ごとの計算　1
#define ID_KSURI2_TITLE2	 86		//	事業の区分ごとの計算　2

#define ID_KSURI2_1_1		 87		//	１.課税仕入れに係る支払い対価の額（税込み）　1
#define ID_KSURI2_1_2		 88		//	１.課税仕入れに係る支払い対価の額（税込み）　2

#define ID_KSURI2_2_1		 89		//	２.特定課税仕入れに係る支払対価の額　1
#define ID_KSURI2_2_2		 90		//	２.特定課税仕入れに係る支払対価の額　2

#define ID_KSURI2_3_1		 91		//	３.保税地域から引き取った課税貨物に係る税込引取価額　1
#define ID_KSURI2_3_2		 92		//	３.保税地域から引き取った課税貨物に係る税込引取価額　2

#define ID_KSURI2_4_1		 93		//	４.課税仕入れに係る支払対価の額等の合計額　1
#define ID_KSURI2_4_2		 94		//	４.課税仕入れに係る支払対価の額等の合計額　2

#define ID_KSURI2_5_1		 95		//	５.4のうち、軽減対象資産の譲渡等（税率6.24%適用分）にのみ要するものの金額（税込み）　1
#define ID_KSURI2_5_2		 96		//	５.4のうち、軽減対象資産の譲渡等（税率6.24%適用分）にのみ要するものの金額（税込み）　2

#define ID_KSURI2_6_1		 97		//	６.小売等軽減仕入割合　1
#define ID_KSURI2_6_2		 98		//	６.小売等軽減仕入割合　2

#define ID_KSURI2_6CHECK_1  111     //  ６．５０％適用チェックボックス
#define ID_KSURI2_6CHECK_2  112     //  ６．５０％適用チェックボックス

#define ID_KSURI2_7_1		 99		//	７.課税資産の譲渡等の税込価額の合計額　1
#define ID_KSURI2_7_2		100		//	７.課税資産の譲渡等の税込価額の合計額　2

#define ID_KSURI2_8_1		101		//	８.軽減対象資産の譲渡等（税率6.24%適用分）の対価の額の合計額（税抜き）　1
#define ID_KSURI2_8_2		102		//	８.軽減対象資産の譲渡等（税率6.24%適用分）の対価の額の合計額（税抜き）　2
#define ID_KSURI2_8_KEI		103		//	８.軽減対象資産の譲渡等（税率6.24%適用分）の対価の額の合計額（税抜き）　3

#define ID_KSURI2_9_1		104		//	９.軽減対象資産の譲渡等以外の課税資産の譲渡等（税率7.8%適用分）の対価の額の合計額（税抜き）　1
#define ID_KSURI2_9_2		105		//	９.軽減対象資産の譲渡等以外の課税資産の譲渡等（税率7.8%適用分）の対価の額の合計額（税抜き）　2
#define ID_KSURI2_9_KEI		106		//	９.軽減対象資産の譲渡等以外の課税資産の譲渡等（税率7.8%適用分）の対価の額の合計額（税抜き）　3

#define ID_KSURI2_10		107		//	１０.軽減対象資産の譲渡等（税率6.24%適用分）の対価の額の合計額（税抜き）
#define ID_KSURI2_11		108		//	１１.軽減対象資産の譲渡等以外の課税資産の譲渡等（税率7.8%適用分）の対価の額の合計額（税抜き）

#define ID_KSURI2_12		109		//	１２.軽減対象資産の譲渡等（税率6.24%適用分）の対価の額の合計額（税抜き）
#define ID_KSURI2_13		110		//	１３.軽減対象資産の譲渡等以外の課税資産の譲渡等（税率7.8%適用分）の対価の額の合計額（税抜き）

#define	ID_KSURI2_NO1		116		//	列番号１
#define	ID_KSURI2_NO2		117		//	列番号２



static TOKUREI_ITEMINDEX Ksisan_Uri2[] = {

	{ ID_KSURI2_TITLE1,   1, 1, 1, 0 },
	{ ID_KSURI2_TITLE2,   0, 1, 1, 0 },

	{ ID_KSURI2_1_1,      0, 1, 1, 0 },
	{ ID_KSURI2_1_2,      0, 1, 1, 0 },

	{ ID_KSURI2_2_1,      0, 1, 1, 0 },
	{ ID_KSURI2_2_2,      0, 1, 1, 0 },

	{ ID_KSURI2_3_1,      0, 1, 1, 0 },
	{ ID_KSURI2_3_2,      0, 1, 1, 0 },

	{ ID_KSURI2_4_1,      0, 0, 0, 0 },
	{ ID_KSURI2_4_2,      0, 0, 0, 0 },

	{ ID_KSURI2_5_1,      0, 1, 1, 0 },
	{ ID_KSURI2_5_2,      0, 1, 1, 0 },

	{ ID_KSURI2_6CHECK_1, 1, 1, 1, 0 },
	//{ ID_KSURI2_6CHECK_2, 1, 1, 1, 0 },
	{ ID_KSURI2_6CHECK_2, 0, 1, 1, 0 },

	{ ID_KSURI2_6_1,      0, 0, 0, 0 },
	{ ID_KSURI2_6_2,      0, 0, 0, 0 },

	{ ID_KSURI2_7_1,      0, 1, 1, 0 },
	{ ID_KSURI2_7_2,      0, 1, 1, 0 },

	{ ID_KSURI2_8_1,      0, 1, 1, 0 },
	{ ID_KSURI2_8_2,      0, 1, 1, 0 },
	{ ID_KSURI2_8_KEI,    0, 0, 0, 0 },

	{ ID_KSURI2_9_1,      0, 1, 1, 0 },
	{ ID_KSURI2_9_2,      0, 1, 1, 0 },
	{ ID_KSURI2_9_KEI,    0, 0, 0, 0 },

	{ ID_KSURI2_10,       0, 1, 1, 0 },
	{ ID_KSURI2_11,       0, 1, 1, 0 },

	{ ID_KSURI2_12,       0, 0, 0, 0 },
	{ ID_KSURI2_13,       0, 0, 0, 0 },

	{            0,       0, 0, 0, 0 }
};

static FCSUPDOWN_INFO FCS_Ksisan_Uri2[] = {

	{ ID_KSURI2_TITLE1 },
	{ ID_KSURI2_1_1 },
	{ ID_KSURI2_2_1 },
	{ ID_KSURI2_3_1 },
	{ ID_KSURI2_4_1 },
	{ ID_KSURI2_5_1 },
	{ ID_KSURI2_6CHECK_1 },
	{ ID_KSURI2_6_1 },
	{ ID_KSURI2_7_1 },
	{ ID_KSURI2_8_1 },
	{ ID_KSURI2_9_1 },

	{ ID_KSURI2_TITLE2 },
	{ ID_KSURI2_1_2 },
	{ ID_KSURI2_2_2 },
	{ ID_KSURI2_3_2 },
	{ ID_KSURI2_4_2 },
	{ ID_KSURI2_5_2 },
	{ ID_KSURI2_6CHECK_2 },
	{ ID_KSURI2_6_2 },
	{ ID_KSURI2_7_2 },
	{ ID_KSURI2_8_2 },
	{ ID_KSURI2_9_2 },

	{ ID_KSURI2_8_KEI },
	{ ID_KSURI2_9_KEI },

	{ ID_KSURI2_10 },
	{ ID_KSURI2_11 },
	{ ID_KSURI2_12 },
	{ ID_KSURI2_13 }
};

enum {
	MOVE_DOWN,
	MOVE_UP
};

#define KSISANURI2_DIAGCTRLID_NAME       5
#define KSISANURI2_DIAGCTRLID_KAZEI_S    6
#define KSISANURI2_DIAGCTRLID_KAZEI_E    7
#define KSISANURI2_DIAGCTRLID_TEKIYO_S   8
#define KSISANURI2_DIAGCTRLID_TEKIYO_E   9
#define KSISANURI2_DIAGCTRLID_MAX      110

#define KSISANURI2_PRT_PAGE_MAX          5
#define JIGYO_PER_PAGE_URI2              2

int idx_ksuri2_dummy[] = { -1 };
int idx_ksuri2_title[] = { ID_KSURI2_TITLE1, ID_KSURI2_TITLE2 };
int idx_ksuri2_1[] = { ID_KSURI2_1_1, ID_KSURI2_1_2 };
int idx_ksuri2_2[] = { ID_KSURI2_2_1, ID_KSURI2_2_2 };
int idx_ksuri2_3[] = { ID_KSURI2_3_1, ID_KSURI2_3_2 };
int idx_ksuri2_4[] = { ID_KSURI2_4_1, ID_KSURI2_4_2 };
int idx_ksuri2_5[] = { ID_KSURI2_5_1, ID_KSURI2_5_2 };
int idx_ksuri2_6_check[] = { ID_KSURI2_6CHECK_1, ID_KSURI2_6CHECK_2 };
int idx_ksuri2_6[] = { ID_KSURI2_6_1, ID_KSURI2_6_2 };
int idx_ksuri2_7[] = { ID_KSURI2_7_1, ID_KSURI2_7_2 };
int idx_ksuri2_8[] = { ID_KSURI2_8_1, ID_KSURI2_8_2 };
int idx_ksuri2_9[] = { ID_KSURI2_9_1, ID_KSURI2_9_2 };

int idx_ksuri2_8_kei[] = { ID_KSURI2_8_KEI };
int idx_ksuri2_9_kei[] = { ID_KSURI2_9_KEI };
int idx_ksuri2_10[] = { ID_KSURI2_10 };
int idx_ksuri2_11[] = { ID_KSURI2_11 };
int idx_ksuri2_12[] = { ID_KSURI2_12 };
int idx_ksuri2_13[] = { ID_KSURI2_13 };

int idx_ksuri2_row1[] = { ID_KSURI2_TITLE1,
						  ID_KSURI2_1_1,
						  ID_KSURI2_2_1,
						  ID_KSURI2_3_1,
						  ID_KSURI2_4_1,
						  ID_KSURI2_5_1,
						  ID_KSURI2_6CHECK_1,
						  ID_KSURI2_6_1,
						  ID_KSURI2_7_1,
						  ID_KSURI2_8_1,
						  ID_KSURI2_9_1 };

int idx_ksuri2_row2[] = { ID_KSURI2_TITLE2,
						  ID_KSURI2_1_2,
						  ID_KSURI2_2_2,
						  ID_KSURI2_3_2,
						  ID_KSURI2_4_2,
						  ID_KSURI2_5_2,
						  ID_KSURI2_6CHECK_2,
						  ID_KSURI2_6_2,
						  ID_KSURI2_7_2,
						  ID_KSURI2_8_2,
						  ID_KSURI2_9_2 };

int idx_ksuri2Rend_next[] = { ID_KSURI2_1_1, 
							  ID_KSURI2_2_1,
							  ID_KSURI2_3_1,
							  ID_KSURI2_4_1,
							  ID_KSURI2_5_1,
							  ID_KSURI2_6CHECK_1,
							  ID_KSURI2_6_1,
							  ID_KSURI2_7_1,
							  ID_KSURI2_8_1,
							  ID_KSURI2_9_1,
							  ID_KSURI2_10 };

int idx_ksuri2Lend_next[] = { ID_KSURI2_13,
							  ID_KSURI2_TITLE1,
							  ID_KSURI2_1_2,
							  ID_KSURI2_2_2,
							  ID_KSURI2_3_2,
							  ID_KSURI2_4_2,
							  ID_KSURI2_5_2,
							  ID_KSURI2_6CHECK_2,
							  ID_KSURI2_6_2,
							  ID_KSURI2_7_2,
							  ID_KSURI2_8_KEI };