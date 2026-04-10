#pragma once


#define	ID_KSURI_TITLE1		81		// 事業の区分ごとの計算　項目名１
#define	ID_KSURI_TITLE2		82		// 事業の区分ごとの計算　項目名２
#define	ID_KSURI_TITLE3		83		// 事業の区分ごとの計算　項目名３

#define	ID_KSURI_1_1		84		// １. 課税資産の譲渡等の税込価格の合計額　１
#define	ID_KSURI_1_2		85		// １. 課税資産の譲渡等の税込価格の合計額　２
#define	ID_KSURI_1_3		86		// １. 課税資産の譲渡等の税込価格の合計額　３

#define	ID_KSURI_2_1_S		87		// ２. 通常の事業を行う連続する１０営業日（自）　１
#define	ID_KSURI_2_1_E		90		// ２. 通常の事業を行う連続する１０営業日（至）　１
#define	ID_KSURI_2_2_S		88		// ２. 通常の事業を行う連続する１０営業日（自）　２
#define	ID_KSURI_2_2_E		91		// ２. 通常の事業を行う連続する１０営業日（至）　２
#define	ID_KSURI_2_3_S		89		// ２. 通常の事業を行う連続する１０営業日（自）　３
#define	ID_KSURI_2_3_E		92		// ２. 通常の事業を行う連続する１０営業日（至）　３

#define	ID_KSURI_3_1		93		// ３. ２の期間中に行った課税資産の譲渡等の税込価格の合計額　１
#define	ID_KSURI_3_2		94		// ３. ２の期間中に行った課税資産の譲渡等の税込価格の合計額　２
#define	ID_KSURI_3_3		95		// ３. ２の期間中に行った課税資産の譲渡等の税込価格の合計額　３

#define	ID_KSURI_4_1		96		// ４. のうち、軽減対象資産の譲渡等（税込み6.24%適用分）に係る部分の金額（税込み）　１
#define	ID_KSURI_4_2		97		// ４. のうち、軽減対象資産の譲渡等（税込み6.24%適用分）に係る部分の金額（税込み）　２
#define	ID_KSURI_4_3		98		// ４. のうち、軽減対象資産の譲渡等（税込み6.24%適用分）に係る部分の金額（税込み）　３

#define ID_KSURI_5CHECK_1   120     // ５. ５０%適用チェックボックス １
#define ID_KSURI_5CHECK_2   121     // ５. ５０%適用チェックボックス ２
#define ID_KSURI_5CHECK_3   122     // ５. ５０%適用チェックボックス ３

#define	ID_KSURI_5_1		99		// ５. 軽減売上割合　１
#define	ID_KSURI_5_2		100		// ５. 軽減売上割合　２
#define	ID_KSURI_5_3		101		// ５. 軽減売上割合　３

#define	ID_KSURI_6_1		102		// ６. 軽減対象資産の譲渡等（6.24%適用分）の対価の額の合計額（税抜き）　１
#define	ID_KSURI_6_2		103		// ６. 軽減対象資産の譲渡等（6.24%適用分）の対価の額の合計額（税抜き）　２
#define	ID_KSURI_6_3		104		// ６. 軽減対象資産の譲渡等（6.24%適用分）の対価の額の合計額（税抜き）　３
#define	ID_KSURI_6_KEI		105		// ６. 軽減対象資産の譲渡等（6.24%適用分）の対価の額の合計額（税抜き）　合計額

#define	ID_KSURI_7_1		106		// ７. 軽減対象資産の譲渡等以外の課税資産の譲渡等（税率7.8%適用分）の対価の額の合計額（税抜き）　１
#define	ID_KSURI_7_2		107		// ７. 軽減対象資産の譲渡等以外の課税資産の譲渡等（税率7.8%適用分）の対価の額の合計額（税抜き）　２
#define	ID_KSURI_7_3		108		// ７. 軽減対象資産の譲渡等以外の課税資産の譲渡等（税率7.8%適用分）の対価の額の合計額（税抜き）　３
#define	ID_KSURI_7_KEI		109		// ７. 軽減対象資産の譲渡等以外の課税資産の譲渡等（税率7.8%適用分）の対価の額の合計額（税抜き）　合計額

#define	ID_KSURI_8			110		// ８. 軽減対象資産の譲渡等（税率6.24%適用分）の対価の額の合計額（税抜き）

#define	ID_KSURI_9			111		// ９. 軽減対象資産の譲渡等以外の課税資産の譲渡等（税率7.8%適用分）の対価の額の合計額（税抜き）

#define	ID_KSURI_10			112		// １０. 軽減対象資産の譲渡等（税率6.24%適用分）の対価の額の合計額（税抜き）

#define	ID_KSURI_11			113		// １１. 軽減対象資産の譲渡等以外の課税資産の譲渡等（税率7.8%適用分）の対価の額の合計額（税抜き）

#define ID_KSURI_NO1		126		// 列番号１
#define ID_KSURI_NO2		127		// 列番号２
#define ID_KSURI_NO3		128		// 列番号３


static TOKUREI_ITEMINDEX KSisanUri[]	=	{
	{  ID_KSURI_TITLE1,   1, 1, 1, 0 },
	{  ID_KSURI_TITLE2,   0, 1, 1, 0 },
	{  ID_KSURI_TITLE3,   0, 1, 1, 0 },
	{  ID_KSURI_1_1,      0, 1, 1, 0 },
	{  ID_KSURI_1_2,      0, 1, 1, 0 },
	{  ID_KSURI_1_3,      0, 1, 1, 0 },
	{  ID_KSURI_2_1_S,    0, 1, 1, 0 },
	{  ID_KSURI_2_1_E,    0, 1, 1, 0 },
	{  ID_KSURI_2_2_S,    0, 1, 1, 0 },
	{  ID_KSURI_2_2_E,    0, 1, 1, 0 },
	{  ID_KSURI_2_3_S,    0, 1, 1, 0 },
	{  ID_KSURI_2_3_E,    0, 1, 1, 0 },
	{  ID_KSURI_3_1,      0, 1, 1, 0 },
	{  ID_KSURI_3_2,      0, 1, 1, 0 },
	{  ID_KSURI_3_3,      0, 1, 1, 0 },
	{  ID_KSURI_4_1,      0, 1, 1, 0 },
	{  ID_KSURI_4_2,      0, 1, 1, 0 },
	{  ID_KSURI_4_3,      0, 1, 1, 0 },
	{  ID_KSURI_5CHECK_1, 1, 1, 1, 0 },
//	{  ID_KSURI_5CHECK_2, 1, 1, 1, 0 },
//	{  ID_KSURI_5CHECK_3, 1, 1, 1, 0 },
	{  ID_KSURI_5CHECK_2, 0, 1, 1, 0 },
	{  ID_KSURI_5CHECK_3, 0, 1, 1, 0 },
	{  ID_KSURI_5_1,      0, 0, 0, 0 },
	{  ID_KSURI_5_2,      0, 0, 0, 0 },
	{  ID_KSURI_5_3,      0, 0, 0, 0 },
	{  ID_KSURI_6_1,      0, 1, 1, 0 },
	{  ID_KSURI_6_2,      0, 1, 1, 0 },
	{  ID_KSURI_6_3,      0, 1, 1, 0 },
	{  ID_KSURI_6_KEI,    0, 0, 0, 0 },
	{  ID_KSURI_7_1,      0, 1, 1, 0 },
	{  ID_KSURI_7_2,      0, 1, 1, 0 },
	{  ID_KSURI_7_3,      0, 1, 1, 0 },
	{  ID_KSURI_7_KEI,    0, 0, 0, 0 },
	{  ID_KSURI_8,        0, 1, 1, 0 },
	{  ID_KSURI_9,        0, 1, 1, 0 },
	{  ID_KSURI_10,       0, 0, 0, 0 },
	{  ID_KSURI_11,       0, 0, 0, 0 },
	{	         0,       0, 0, 0, 0 }
};

static FCSUPDOWN_INFO FCS_KSisanUri[] = {
	{ ID_KSURI_TITLE1	},
	{ ID_KSURI_1_1		},
	{ ID_KSURI_2_1_S	},
	{ ID_KSURI_2_1_E	},
	{ ID_KSURI_3_1		},
	{ ID_KSURI_4_1		},
	{ ID_KSURI_5CHECK_1	},
	{ ID_KSURI_5_1		},
	{ ID_KSURI_6_1		},
	{ ID_KSURI_7_1		},

	{ ID_KSURI_TITLE2	},
	{ ID_KSURI_1_2		},
	{ ID_KSURI_2_2_S	},
	{ ID_KSURI_2_2_E	},
	{ ID_KSURI_3_2		},
	{ ID_KSURI_4_2		},
	{ ID_KSURI_5CHECK_2	},
	{ ID_KSURI_5_2		},
	{ ID_KSURI_6_2		},
	{ ID_KSURI_7_2		},

	{ ID_KSURI_TITLE3	},
	{ ID_KSURI_1_3		},
	{ ID_KSURI_2_3_S	},
	{ ID_KSURI_2_3_E	},
	{ ID_KSURI_3_3		},
	{ ID_KSURI_4_3		},
	{ ID_KSURI_5CHECK_3	},
	{ ID_KSURI_5_3		},
	{ ID_KSURI_6_3		},
	{ ID_KSURI_7_3		},

	{ ID_KSURI_6_KEI	},
	{ ID_KSURI_7_KEI	},

	{ ID_KSURI_8		},
	{ ID_KSURI_9		},
	{ ID_KSURI_10		},
	{ ID_KSURI_11		},

};

enum {
	MOVE_DOWN,
    MOVE_UP
};

#define KSISANURI_DIAGCTRLID_NAME       5
#define KSISANURI_DIAGCTRLID_KAZEI_S    6
#define KSISANURI_DIAGCTRLID_KAZEI_E    7
#define KSISANURI_DIAGCTRLID_TEKIYO_S   8
#define KSISANURI_DIAGCTRLID_TEKIYO_E   9
#define KSISANURI_DIAGCTRLID_MAX      113

#define KSISANURI_PRT_PAGE_MAX          5
#define JIGYO_PER_PAGE_URI    3

char ksuri_kbntitle[JIGYOKUBUN_MAX][64] = { "区分なし", "卸売業", "小売業等", "製造業等", "その他", "サービス業等", "不動産業" };

int idx_ksuri_dummy[] = {-1};
int idx_ksuri_title[] = { ID_KSURI_TITLE1, ID_KSURI_TITLE2, ID_KSURI_TITLE3 };
int idx_ksuri_1[] = { ID_KSURI_1_1, ID_KSURI_1_2, ID_KSURI_1_3 };
int idx_ksuri_2_s[] = { ID_KSURI_2_1_S, ID_KSURI_2_2_S, ID_KSURI_2_3_S };
int idx_ksuri_2_e[] = { ID_KSURI_2_1_E,ID_KSURI_2_2_E,ID_KSURI_2_3_E };
int idx_ksuri_3[] = { ID_KSURI_3_1, ID_KSURI_3_2, ID_KSURI_3_3 };
int idx_ksuri_4[] = { ID_KSURI_4_1, ID_KSURI_4_2, ID_KSURI_4_3 };
int idx_ksuri_5_check[] = { ID_KSURI_5CHECK_1, ID_KSURI_5CHECK_2, ID_KSURI_5CHECK_3 };
int idx_ksuri_5[] = { ID_KSURI_5_1, ID_KSURI_5_2, ID_KSURI_5_3 };
int idx_ksuri_6[] = { ID_KSURI_6_1, ID_KSURI_6_2, ID_KSURI_6_3 };
int idx_ksuri_7[] = { ID_KSURI_7_1, ID_KSURI_7_2, ID_KSURI_7_3 };

int idx_ksuri_6_kei[] = { ID_KSURI_6_KEI };
int idx_ksuri_7_kei[] = { ID_KSURI_7_KEI };
int idx_ksuri_8[] = { ID_KSURI_8 };
int idx_ksuri_9[] = { ID_KSURI_9 };
int idx_ksuri_10[] = { ID_KSURI_10 };
int idx_ksuri_11[] = { ID_KSURI_11 };

int idx_ksuri_row1[] = { ID_KSURI_TITLE1,
						ID_KSURI_1_1,
						ID_KSURI_2_1_S,
						ID_KSURI_2_1_E,
						ID_KSURI_3_1,
						ID_KSURI_4_1,
						ID_KSURI_5CHECK_1,
						ID_KSURI_5_1,
						ID_KSURI_6_1,
						ID_KSURI_7_1 };

int idx_ksuri_row2[] = { ID_KSURI_TITLE2,
						ID_KSURI_1_2,
						ID_KSURI_2_2_S,
						ID_KSURI_2_2_E,
						ID_KSURI_3_2,
						ID_KSURI_4_2,
						ID_KSURI_5CHECK_2,
						ID_KSURI_5_2,
						ID_KSURI_6_2,
						ID_KSURI_7_2 };

int idx_ksuri_row3[] = { ID_KSURI_TITLE3,
						ID_KSURI_1_3,
						ID_KSURI_2_3_S,
						ID_KSURI_2_3_E,
						ID_KSURI_3_3,
						ID_KSURI_4_3,
						ID_KSURI_5CHECK_3,
						ID_KSURI_5_3,
                        ID_KSURI_6_3,
	                    ID_KSURI_7_3 };


int idx_ksuriRend_next[] = { ID_KSURI_1_1,
						ID_KSURI_2_1_S,
						ID_KSURI_2_1_E,
						ID_KSURI_3_1,
						ID_KSURI_4_1,
						ID_KSURI_5CHECK_1,
						ID_KSURI_5_1,
						ID_KSURI_6_1,
						ID_KSURI_7_1,
						ID_KSURI_8 };
						

int idx_ksuriLend_next[] = { ID_KSURI_11,
						ID_KSURI_TITLE3,
						ID_KSURI_1_3,
						ID_KSURI_2_3_S,
						ID_KSURI_2_3_E,
						ID_KSURI_3_3,
						ID_KSURI_4_3,
						ID_KSURI_5CHECK_3,
						ID_KSURI_5_3,
						ID_KSURI_6_KEI };


