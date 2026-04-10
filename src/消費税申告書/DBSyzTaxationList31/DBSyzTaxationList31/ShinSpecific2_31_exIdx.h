#pragma once

#define	ID201_1ITEM 49	// 1行目　内容
#define	ID201_1KEI	50	// 1行目　資産の譲渡等の対価以外の収入
#define	ID201_1A	51	// 1行目　左のうち特定収入　A
#define	ID201_1B	52	// 1行目　課税仕入れ等に係る特定収入　B　６．２４％
#define	ID201_1C	53	// 1行目　課税仕入れ等に係る特定収入　C　７．８％
#define	ID201_1D	54	// 1行目　A－（B＋C)の金額　D

#define	ID201_2ITEM 55	// 2行目　内容
#define	ID201_2KEI	56	// 2行目　資産の譲渡等の対価以外の収入
#define	ID201_2A	57	// 2行目　左のうち特定収入　A
#define	ID201_2B	58	// 2行目　課税仕入れ等に係る特定収入　B　６．２４％
#define	ID201_2C	59	// 2行目　課税仕入れ等に係る特定収入　C　７．８％
#define	ID201_2D	60	// 2行目　A－（B＋C)の金額　D

#define	ID201_3ITEM 61	// 3行目　内容
#define	ID201_3KEI	62	// 3行目　資産の譲渡等の対価以外の収入
#define	ID201_3A	63	// 3行目　左のうち特定収入　A
#define	ID201_3B	64	// 3行目　課税仕入れ等に係る特定収入　B　６．２４％
#define	ID201_3C	65	// 3行目　課税仕入れ等に係る特定収入　C　７．８％
#define	ID201_3D	66	// 3行目　A－（B＋C)の金額　D

#define	ID201_4ITEM 67	// 4行目　内容
#define	ID201_4KEI	68	// 4行目　資産の譲渡等の対価以外の収入
#define	ID201_4A	69	// 4行目　左のうち特定収入　A
#define	ID201_4B	70	// 4行目　課税仕入れ等に係る特定収入　B　６．２４％
#define	ID201_4C	71	// 4行目　課税仕入れ等に係る特定収入　C　７．８％
#define	ID201_4D	72	// 4行目　A－（B＋C)の金額　D

#define	ID201_5ITEM 73	// 5行目　内容
#define	ID201_5KEI	74	// 5行目　資産の譲渡等の対価以外の収入
#define	ID201_5A	75	// 5行目　左のうち特定収入　A
#define	ID201_5B	76	// 5行目　課税仕入れ等に係る特定収入　B　６．２４％
#define	ID201_5C	77	// 5行目　課税仕入れ等に係る特定収入　C　７．８％
#define	ID201_5D	78	// 5行目　A－（B＋C)の金額　D

#define	ID201_6ITEM 79	// 6行目　内容
#define	ID201_6KEI	80	// 6行目　資産の譲渡等の対価以外の収入
#define	ID201_6A	81	// 6行目　左のうち特定収入　A
#define	ID201_6B	82	// 6行目　課税仕入れ等に係る特定収入　B　６．２４％
#define	ID201_6C	83	// 6行目　課税仕入れ等に係る特定収入　C　７．８％
#define	ID201_6D	84	// 6行目　A－（B＋C)の金額　D

#define	ID201_7ITEM 85	// 7行目　内容
#define	ID201_7KEI	86	// 7行目　資産の譲渡等の対価以外の収入
#define	ID201_7A	87	// 7行目　左のうち特定収入　A
#define	ID201_7B	88	// 7行目　課税仕入れ等に係る特定収入　B　６．２４％
#define	ID201_7C	89	// 7行目　課税仕入れ等に係る特定収入　C　７．８％
#define	ID201_7D	90	// 7行目　A－（B＋C)の金額　D


#define	ID201_17KEI	7		// 資産の譲渡等の対価以外の収入 合計
#define	ID201_17A	8		// 左のうち特定収入　A
#define	ID201_17B	9		// 課税仕入れ等に係る特定収入　B　６．２４％
#define	ID201_17C	10		// 課税仕入れ等に係る特定収入　C　７．８％
#define	ID201_17D	11      // A－（B＋C)の金額　D


//{インデックス、連動、非連動、確定｝　
SH_ITEMINDEX	Specific21_31Ex[] = {
{	ID201_1ITEM,	0, 1, 0		},
{	ID201_1KEI,	0, 1, 0		},
{	ID201_1A,	0, 1, 0		},
{	ID201_1B,	0, 1, 0		},
{	ID201_1C,	0, 1, 0		},
{	ID201_1D,	0, 0, 0		},

{	ID201_2ITEM,	0, 1, 0		},
{	ID201_2KEI,	0, 1, 0		},
{	ID201_2A,	0, 1, 0		},
{	ID201_2B,	0, 1, 0		},
{	ID201_2C,	0, 1, 0		},
{	ID201_2D,	0, 0, 0		},

{	ID201_3ITEM,	0, 1, 0		},
{	ID201_3KEI,	0, 1, 0		},
{	ID201_3A,	0, 1, 0		},
{	ID201_3B,	0, 1, 0		},
{	ID201_3C,	0, 1, 0		},
{	ID201_3D,	0, 0, 0		},

{	ID201_4ITEM,	0, 1, 0		},
{	ID201_4KEI,	0, 1, 0		},
{	ID201_4A,	0, 1, 0		},
{	ID201_4B,	0, 1, 0		},
{	ID201_4C,	0, 1, 0		},
{	ID201_4D,	0, 0, 0		},

{	ID201_5ITEM,	0, 1, 0		},
{	ID201_5KEI,	0, 1, 0		},
{	ID201_5A,	0, 1, 0		},
{	ID201_5B,	0, 1, 0		},
{	ID201_5C,	0, 1, 0		},
{	ID201_5D,	0, 0, 0		},

{	ID201_6ITEM,	0, 1, 0		},
{	ID201_6KEI,	0, 1, 0		},
{	ID201_6A,	0, 1, 0		},
{	ID201_6B,	0, 1, 0		},
{	ID201_6C,	0, 1, 0		},
{	ID201_6D,	0, 0, 0		},

{	ID201_7ITEM,	0, 1, 0		},
{	ID201_7KEI,	0, 1, 0		},
{	ID201_7A,	0, 1, 0		},
{	ID201_7B,	0, 1, 0		},
{	ID201_7C,	0, 1, 0		},
{	ID201_7D,	0, 0, 0		},

{	   0,	0, 0, 0		}
};

static FCSUPDOWN_INFO	FCS_Specific21_31Ex[] = {
	{	ID201_1ITEM 		},
{	ID201_2ITEM 		},
{	ID201_3ITEM 		},
{	ID201_4ITEM 		},
{	ID201_5ITEM 		},
{	ID201_6ITEM 		},
{	ID201_7ITEM 		},

{	ID201_1KEI 		},
{	ID201_2KEI 		},
{	ID201_3KEI		},
{	ID201_4KEI		},
{	ID201_5KEI		},
{	ID201_6KEI		},
{	ID201_7KEI		},

{	ID201_1A 		},
{	ID201_2A 		},
{	ID201_3A 		},
{	ID201_4A 		},
{	ID201_5A 		},
{	ID201_6A 		},
{	ID201_7A 		},

{	ID201_1B 		},
{	ID201_2B 		},
{	ID201_3B 		},
{	ID201_4B 		},
{	ID201_5B 		},
{	ID201_6B 		},
{	ID201_7B 		},

{	ID201_1C 		},
{	ID201_2C 		},
{	ID201_3C 		},
{	ID201_4C 		},
{	ID201_5C    	},
{	ID201_6C 		},
{	ID201_7C 		},

{	ID201_1D 		},
{	ID201_2D 		},
{	ID201_3D 		},
{	ID201_4D 		},
{	ID201_5D 		},
{	ID201_6D 		},
{	ID201_7D 		},

};

static FCSUPDOWN_INFO	FCS_Specific21_31Ex_LR[] = {
	{	ID201_1ITEM 		},
{	ID201_1KEI 		},
{	ID201_1A 		},
{	ID201_1B 		},
{	ID201_1C 		},
{	ID201_1D 		},

{	ID201_2ITEM 	},
{	ID201_2KEI  	},
{	ID201_2A    	},
{	ID201_2B		},
{	ID201_2C 		},
{	ID201_2D 		},

{	ID201_3ITEM 	},
{	ID201_3KEI		},
{	ID201_3A 		},
{	ID201_3B 		},
{	ID201_3C 		},
{	ID201_3D 		},

{	ID201_4ITEM 	},
{	ID201_4KEI		},
{	ID201_4A 		},
{	ID201_4B 		},
{	ID201_4C 		},
{	ID201_4D 		},

{	ID201_5ITEM 	},
{	ID201_5KEI		},
{	ID201_5A 		},
{	ID201_5B 		},
{	ID201_5C 		},
{	ID201_5D 		},

{	ID201_6ITEM 	},
{	ID201_6KEI		},
{	ID201_6A 		},
{	ID201_6B 		},
{	ID201_6C 		},
{	ID201_6D 		},

{	ID201_7ITEM 		},
{	ID201_7KEI		},
{	ID201_7A 		},
{	ID201_7B 		},
{	ID201_7C 		},
{	ID201_7D 		},
};

#define		FCS_SPECIFIC21CNT	46