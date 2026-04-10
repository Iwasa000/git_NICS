#pragma	once

#define		SKLINCNT	19		//帳票の行数。この行数でデータを保存していってる
//#define		SKLINCNT	16		//帳票の行数。この行数でデータを保存していってる
#define		SKVMIN		104		//入力ボックスの最小のインデックス
#define		SKVMAX		149		//入力ボックスの最大のインデックス


#define		SKYEARID	1

#define		SKZKBNID	150
#define		SKDISP1		29



//---->'14.08.04
//定義変更。全てのIDX定義'14.08.11
#define		KSELKNAME	103
#define		KSELVAL01A	104
#define		KSELVAL01B	105
#define		KSELVAL01C	106
#define		KSELVAL02A	107
#define		KSELVAL02B	108
#define		KSELVAL02C	109
#define		KSELVAL03A	110
#define		KSELVAL03B	111
#define		KSELVAL03C	112
#define		KSELVAL04A	113
#define		KSELVAL04B	114
#define		KSELVAL04C	115
#define		KSELVAL05A	116
#define		KSELVAL05B	117
#define		KSELVAL05C	118
#define		KSELVAL06A	119
#define		KSELVAL06B	120
#define		KSELVAL06C	121
#define		KSELVAL07A	122
#define		KSELVAL07B	123
#define		KSELVAL07C	124
#define		KSELVAL08A	125
#define		KSELVAL08B	126
#define		KSELVAL08C	127
#define		KSELVAL09A	128
#define		KSELVAL09B	129
#define		KSELVAL09C	130
#define		KSELVAL10A	131
#define		KSELVAL10B	132
#define		KSELVAL10C	133
#define		KSELVAL11A	134
#define		KSELVAL11B	135
#define		KSELVAL11C	136
#define		KSELVAL12A	137
#define		KSELVAL12B	138
#define		KSELVAL12C	139
#define		KSELVAL13A	140
#define		KSELVAL13B	141
#define		KSELVAL13C	142
#define		KSELVAL14K	143
#define		KSELVAL14C	144
#define		KSELVAL14	145
#define		KSELVAL15K	146
#define		KSELVAL15C	147
#define		KSELVAL15	148
#define		KSELVAL16	149

// 課税売上高計算表
static SH_ITEMINDEX	SellKeika[]	=	{
	{  KSELKNAME, 1, 1, 0			},
	{  KSELVAL01A, 0, 1, 0			},
	{  KSELVAL02A, 0, 1, 0			},
	{  KSELVAL03B, 0, 1, 0			},
	{  KSELVAL03C, 0, 1, 0			},
	{  KSELVAL04A, 0, 1, 0			},
	{  KSELVAL05A, 0, 1, 0			},
	{  KSELVAL06B, 0, 1, 0			},
	{  KSELVAL06C, 0, 1, 0			},
	{  KSELVAL07A, 1, 1, 0			},
	{  KSELVAL08A, 1, 1, 0			},
	{  KSELVAL09B, 1, 1, 0			},
	{  KSELVAL09C, 1, 1, 0			},
	{  KSELVAL10A, 0, 1, 0			},
	{  KSELVAL11A, 0, 1, 0			},
	{  KSELVAL12B, 0, 1, 0			},
	{  KSELVAL12C, 0, 1, 0			},
	{  KSELVAL14K, 0, 1, 0			},
	{  KSELVAL14C, 0, 1, 0			},
	{  KSELVAL14, 0, 1, 0			},
	{  KSELVAL15K, 0, 1, 0			},
	{  KSELVAL15C, 0, 1, 0			},
	{  KSELVAL15, 0, 1, 0			},
//	{  KSELVAL16, 0, 1, 0			},
	{	0, 0, 0, 0					}
};

//'14.08.01
static FCSUPDOWN_INFO	FCS_ShinSellKeika[]	=	{
	{	KSELKNAME 		},
	{	KSELVAL01A 		},
	{	KSELVAL02A 		},
	{	KSELVAL03A		},
	{	KSELVAL04A		},
	{	KSELVAL05A		},
	{	KSELVAL06A		},
	{	KSELVAL07A		},
	{	KSELVAL08A		},
	{	KSELVAL09A 		},
	{	KSELVAL10A 		},
	{	KSELVAL11A 		},
	{	KSELVAL12A		},
	{	KSELVAL13A		},
	{	KSELVAL14K		},
	{	KSELVAL14C		},
	{	KSELVAL14		},
	{	KSELVAL15K		},
	{	KSELVAL15C		},
	{	KSELVAL15		},
	{	KSELVAL16		},
	{	KSELVAL01B 		},
	{	KSELVAL02B 		},
	{	KSELVAL03B		},
	{	KSELVAL04B		},
	{	KSELVAL05B		},
	{	KSELVAL06B		},
	{	KSELVAL07B		},
	{	KSELVAL08B		},
	{	KSELVAL09B 		},
	{	KSELVAL10B 		},
	{	KSELVAL11B 		},
	{	KSELVAL12B		},
	{	KSELVAL13B		},
	{	KSELVAL01C 		},
	{	KSELVAL02C 		},
	{	KSELVAL03C		},
	{	KSELVAL04C		},
	{	KSELVAL05C		},
	{	KSELVAL06C		},
	{	KSELVAL07C		},
	{	KSELVAL08C		},
	{	KSELVAL09C 		},
	{	KSELVAL10C 		},
	{	KSELVAL11C 		},
	{	KSELVAL12C		},
	{	KSELVAL13C		},

};

static FCSUPDOWN_INFO	FCS_ShinSellKeika_LR[]	=	{
	{	KSELVAL01A 		},
	{	KSELVAL01B 		},
	{	KSELVAL01C 		},
	{	KSELVAL02A 		},
	{	KSELVAL02B 		},
	{	KSELVAL02C 		},
	{	KSELVAL03A		},
	{	KSELVAL03B		},
	{	KSELVAL03C		},
	{	KSELVAL04A		},
	{	KSELVAL04B		},
	{	KSELVAL04C		},
	{	KSELVAL05A		},
	{	KSELVAL05B		},
	{	KSELVAL05C		},
	{	KSELVAL06A		},
	{	KSELVAL06B		},
	{	KSELVAL06C		},
	{	KSELKNAME 		},
	{	KSELVAL07A		},
	{	KSELVAL07B		},
	{	KSELVAL07C		},
	{	KSELVAL08A		},
	{	KSELVAL08B		},
	{	KSELVAL08C		},
	{	KSELVAL09A		},
	{	KSELVAL09B		},
	{	KSELVAL09C		},
	{	KSELVAL10A		},
	{	KSELVAL10B		},
	{	KSELVAL10C		},
	{	KSELVAL11A		},
	{	KSELVAL11B		},
	{	KSELVAL11C		},
	{	KSELVAL12A		},
	{	KSELVAL12B		},
	{	KSELVAL12C		},
	{	KSELVAL13A		},
	{	KSELVAL13B		},
	{	KSELVAL13C		},
	{	KSELVAL14K		},
	{	KSELVAL14C		},
	{	KSELVAL14		},
	{	KSELVAL15K		},
	{	KSELVAL15C		},
	{	KSELVAL15		},
	{	KSELVAL16		},

};

#define		FCS_SHINSELLKEIKACNT	47

//'14.08.20

#define		KSELLINE1	0
#define		KSELLINE2	1
#define		KSELLINE3	2
#define		KSELLINE4	6
#define		KSELLINE5	7
#define		KSELLINE6	8
#define		KSELLINE7	9
#define		KSELLINE8	10
#define		KSELLINE9	11
#define		KSELLINE10	12
#define		KSELLINE11	13
#define		KSELLINE12	14
#define		KSELLINE13	15
//#define		KSELLINE14	17
//#define		KSELLINE15	16
#define		KSELLINE14	16
#define		KSELLINE15	17
#define		KSELLINE16	18


/*
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
*/