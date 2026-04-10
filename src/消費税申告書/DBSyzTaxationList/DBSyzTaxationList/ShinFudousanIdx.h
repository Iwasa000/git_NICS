#pragma once

#define		VCNT	3
#define		VLIN	16

#define		VMIN	87
#define		VMAX	134
#define		VLST	127

#define		YEARID	1

#define		ZKBNID	143


//定義変更。全てのIDX定義'14.08.11
#define		FUDVAL01A	87
#define		FUDVAL01B	88
#define		FUDVAL01C	89
#define		FUDVAL02A	90
#define		FUDVAL02B	91
#define		FUDVAL02C	92
#define		FUDVAL03A	93
#define		FUDVAL03B	94
#define		FUDVAL03C	95
#define		FUDVAL04A	96
#define		FUDVAL04B	97
#define		FUDVAL04C	98
#define		FUDVAL04TA	99
#define		FUDVAL04TB	100
#define		FUDVAL04TC	101
#define		FUDVAL05A	102
#define		FUDVAL05B	103
#define		FUDVAL05C	104
#define		FUDVAL06A	105
#define		FUDVAL06B	106
#define		FUDVAL06C	107
#define		FUDVAL07A	108
#define		FUDVAL07B	109
#define		FUDVAL07C	110
#define		FUDVAL08A	111
#define		FUDVAL08B	112
#define		FUDVAL08C	113
#define		FUDVAL09A	114
#define		FUDVAL09B	115
#define		FUDVAL09C	116
#define		FUDVAL10A	117
#define		FUDVAL10B	118
#define		FUDVAL10C	119
#define		FUDVAL11A	120
#define		FUDVAL11B	121
#define		FUDVAL11C	122
#define		FUDVAL12A	123
#define		FUDVAL12B	124
#define		FUDVAL12C	125
#define		FUDVAL13A	126
#define		FUDVAL13B	127
#define		FUDVAL13C	128
#define		FUDVAL14A	129
#define		FUDVAL14B	130
#define		FUDVAL14C	131
#define		FUDVAL15A	132
#define		FUDVAL15B	133
#define		FUDVAL15C	134
#define		FUDKNAME03	135
#define		FUDKNAME12	136


// 課税取引金額計算表（不動産所得用）
static SH_ITEMINDEX	Fudousan[]	=	{
	{  FUDVAL01A, 0, 1, 0			},
	{  FUDVAL01B, 0, 1, 0			},
	{  FUDVAL02A, 0, 1, 0			},
	{  FUDVAL02B, 0, 1, 0			},
	{  FUDVAL03A, 0, 1, 0			},
	{  FUDVAL03B, 0, 1, 0			},
	{  FUDVAL04TA, 0, 1, 0			},
	{  FUDVAL04TB, 0, 1, 0			},
	{  FUDVAL05A, 0, 1, 0			},
	{  FUDVAL05B, 0, 1, 0			},
	{  FUDVAL06A, 0, 1, 0			},
	{  FUDVAL07A, 0, 1, 0			},
	{  FUDVAL08A, 0, 1, 0			},
	{  FUDVAL09A, 0, 1, 0			},
	{  FUDVAL10A, 0, 1, 0			},
	{  FUDVAL10B, 0, 1, 0			},
	{  FUDVAL11A, 0, 1, 0			},
	{  FUDVAL11B, 0, 1, 0			},
	{  FUDVAL12A, 0, 1, 0			},
	{  FUDVAL12B, 0, 1, 0			},
	{  FUDVAL13A, 0, 1, 0			},
	{  FUDVAL13B, 0, 1, 0			},
	{  FUDKNAME03, 1, 1, 0			},
	{  FUDKNAME12, 1, 1, 0			},
	{  0, 0, 0, 0			}
};


//'14.08.18
static FCSUPDOWN_INFO	FCS_ShinFudousan[]	=	{
	{	FUDKNAME03 		},
	{	FUDKNAME12 		},
	{	FUDVAL01A 		},
	{	FUDVAL02A		},
	{	FUDVAL03A		},
	{	FUDVAL04A 		},
	{	FUDVAL04TA		},
	{	FUDVAL05A		},
	{	FUDVAL06A 		},	
	{	FUDVAL07A		},	
	{	FUDVAL08A		},
	{	FUDVAL09A 		},
	{	FUDVAL10A		},
	{	FUDVAL11A		},
	{	FUDVAL12A 		},
	{	FUDVAL13A		},
	{	FUDVAL14A		},
	{	FUDVAL15A		},
	{	FUDVAL01B 		},
	{	FUDVAL02B		},
	{	FUDVAL03B		},
	{	FUDVAL04B 		},
	{	FUDVAL04TB		},
	{	FUDVAL05B		},
	{	FUDVAL06B 		},	
	{	FUDVAL07B		},	
	{	FUDVAL08B		},
	{	FUDVAL09B 		},
	{	FUDVAL10B		},
	{	FUDVAL11B		},
	{	FUDVAL12B 		},
	{	FUDVAL13B		},
	{	FUDVAL14B		},
	{	FUDVAL15B		},
	{	FUDVAL01C 		},
	{	FUDVAL02C		},
	{	FUDVAL03C		},
	{	FUDVAL04C 		},
	{	FUDVAL04TC		},
	{	FUDVAL05C		},
	{	FUDVAL06C 		},	
	{	FUDVAL07C		},	
	{	FUDVAL08C		},
	{	FUDVAL09C 		},
	{	FUDVAL10C		},
	{	FUDVAL11C		},
	{	FUDVAL12C 		},
	{	FUDVAL13C		},
	{	FUDVAL14C		},
	{	FUDVAL15C		},
};

static FCSUPDOWN_INFO	FCS_ShinFudousan_LR[]	=	{
	{	FUDVAL01A 		},
	{	FUDVAL01B 		},
	{	FUDVAL01C 		},
	{	FUDVAL02A 		},
	{	FUDVAL02B 		},
	{	FUDVAL02C 		},
	{	FUDKNAME03 		},
	{	FUDVAL03A 		},
	{	FUDVAL03B 		},
	{	FUDVAL03C 		},
	{	FUDVAL04A 		},
	{	FUDVAL04B 		},
	{	FUDVAL04C 		},
	{	FUDVAL04TA		},
	{	FUDVAL04TB		},
	{	FUDVAL04TC		},
	{	FUDVAL05A 		},
	{	FUDVAL05B 		},
	{	FUDVAL05C 		},
	{	FUDVAL06A 		},
	{	FUDVAL06B 		},
	{	FUDVAL06C 		},
	{	FUDVAL07A 		},
	{	FUDVAL07B 		},
	{	FUDVAL07C 		},
	{	FUDVAL08A 		},
	{	FUDVAL08B 		},
	{	FUDVAL08C 		},
	{	FUDVAL09A 		},
	{	FUDVAL09B 		},
	{	FUDVAL09C 		},
	{	FUDVAL10A 		},
	{	FUDVAL10B 		},
	{	FUDVAL10C 		},
	{	FUDVAL11A 		},
	{	FUDVAL11B 		},
	{	FUDVAL11C 		},
	{	FUDKNAME12 		},
	{	FUDVAL12A 		},
	{	FUDVAL12B 		},
	{	FUDVAL12C 		},
	{	FUDVAL13A 		},
	{	FUDVAL13B 		},
	{	FUDVAL13C 		},
	{	FUDVAL14A 		},
	{	FUDVAL14B 		},
	{	FUDVAL14C 		},
	{	FUDVAL15A 		},
	{	FUDVAL15B 		},
	{	FUDVAL15C 		},
};

#define		FCS_SHINFUDOUSANCNT	50

#define		KFUDLINE1	0
#define		KFUDLINE2	1
#define		KFUDLINE3	2
#define		KFUDLINE4	3	//計
#define		KFUDLINE4TA	15	//返還等対価
#define		KFUDLINE5	4
#define		KFUDLINE6	5
#define		KFUDLINE7	6
#define		KFUDLINE8	7
#define		KFUDLINE9	8
#define		KFUDLINE10	9
#define		KFUDLINE11	10
#define		KFUDLINE12	11
#define		KFUDLINE13	12
#define		KFUDLINE14	13
#define		KFUDLINE15	14
