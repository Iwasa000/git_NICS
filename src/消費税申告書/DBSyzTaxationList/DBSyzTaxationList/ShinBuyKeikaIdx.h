#pragma	once

#define		BKLINCNT	19	//帳票の行数。この行数でデータを保存していってる
//#define		BKLINCNT	16	//帳票の行数。この行数でデータを保存していってる
#define		BKVMIN		104	//入力ボックスの最小のインデックス
#define		BKVMAX		149	//入力ボックスの最大のインデックス


#define		BKYEARID	1

#define		BKZKBNID	150
#define		BKDISP1		29


//'14.08.01

//定義変更。全てのIDX定義'14.08.11
#define		KBUYKNAME	103
#define		KBUYVAL01A	104
#define		KBUYVAL01B	105
#define		KBUYVAL01C	106
#define		KBUYVAL02A	107
#define		KBUYVAL02B	108
#define		KBUYVAL02C	109
#define		KBUYVAL03A	110
#define		KBUYVAL03B	111
#define		KBUYVAL03C	112
#define		KBUYVAL04A	113
#define		KBUYVAL04B	114
#define		KBUYVAL04C	115
#define		KBUYVAL05A	116
#define		KBUYVAL05B	117
#define		KBUYVAL05C	118
#define		KBUYVAL06A	119
#define		KBUYVAL06B	120
#define		KBUYVAL06C	121
#define		KBUYVAL07A	122
#define		KBUYVAL07B	123
#define		KBUYVAL07C	124
#define		KBUYVAL08A	125
#define		KBUYVAL08B	126
#define		KBUYVAL08C	127
#define		KBUYVAL09A	128
#define		KBUYVAL09B	129
#define		KBUYVAL09C	130
#define		KBUYVAL10A	131
#define		KBUYVAL10B	132
#define		KBUYVAL10C	133
#define		KBUYVAL11A	134
#define		KBUYVAL11B	135
#define		KBUYVAL11C	136
#define		KBUYVAL12A	137
#define		KBUYVAL12B	138
#define		KBUYVAL12C	139
#define		KBUYVAL13A	140
#define		KBUYVAL13B	141
#define		KBUYVAL13C	142
#define		KBUYVAL14K	143
#define		KBUYVAL14C	144
#define		KBUYVAL14	145
#define		KBUYVAL15K	146
#define		KBUYVAL15C	147
#define		KBUYVAL15	148
#define		KBUYVAL16	149

/*

#define		KZNAME	60
#define		VAL01	61
#define		VAL02	62
#define		VAL03	63
#define		VAL04	64
#define		VAL05	65
#define		VAL06	66
#define		VAL07	67
#define		VAL08	68
#define		VAL09	69
#define		VAL10	70
#define		VAL11	71
#define		VAL12	72
#define		VAL13	73
#define		VAL14K	74
#define		VAL14C	75
#define		VAL14	76
#define		VAL15K	82
#define		VAL15C	84
#define		VAL15	94
#define		VAL16	98

*/


// 課税仕入高計算表
static SH_ITEMINDEX	BuyKeika[]	=	{
	{  KBUYKNAME, 1, 1, 0			},
	{  KBUYVAL01A, 0, 1, 0			},
	{  KBUYVAL02A, 0, 1, 0			},
	{  KBUYVAL03B, 0, 1, 0			},
	{  KBUYVAL03C, 0, 1, 0			},
	{  KBUYVAL04A, 0, 1, 0			},
	{  KBUYVAL05A, 0, 1, 0			},
	{  KBUYVAL06B, 0, 1, 0			},
	{  KBUYVAL06C, 0, 1, 0			},
	{  KBUYVAL07A, 1, 1, 0			},
	{  KBUYVAL08A, 1, 1, 0			},
	{  KBUYVAL09B, 1, 1, 0			},
	{  KBUYVAL09C, 1, 1, 0			},
	{  KBUYVAL10A, 0, 1, 0			},
	{  KBUYVAL11A, 0, 1, 0			},
	{  KBUYVAL12B, 0, 1, 0			},
	{  KBUYVAL12C, 0, 1, 0			},
	{  KBUYVAL14K, 0, 1, 0			},
	{  KBUYVAL14C, 0, 1, 0			},
	{  KBUYVAL14, 0, 1, 0			},
	{  KBUYVAL15K, 0, 1, 0			},
	{  KBUYVAL15C, 0, 1, 0			},
	{  KBUYVAL15, 0, 1, 0			},
//	{  KBUYVAL16, 0, 1, 0			},
	{	0, 0, 0, 0					}
};


//'14.08.01
static FCSUPDOWN_INFO	FCS_ShinBuyKeika[]	=	{
	{	KBUYKNAME 		},
	{	KBUYVAL01A 		},
	{	KBUYVAL02A 		},
	{	KBUYVAL03A		},
	{	KBUYVAL04A		},
	{	KBUYVAL05A		},
	{	KBUYVAL06A		},
	{	KBUYVAL07A		},
	{	KBUYVAL08A		},
	{	KBUYVAL09A 		},
	{	KBUYVAL10A 		},
	{	KBUYVAL11A 		},
	{	KBUYVAL12A		},
	{	KBUYVAL13A		},
	{	KBUYVAL14K		},
	{	KBUYVAL14C		},
	{	KBUYVAL14		},
	{	KBUYVAL15K		},
	{	KBUYVAL15C		},
	{	KBUYVAL15		},
	{	KBUYVAL16		},
	{	KBUYVAL01B 		},
	{	KBUYVAL02B 		},
	{	KBUYVAL03B		},
	{	KBUYVAL04B		},
	{	KBUYVAL05B		},
	{	KBUYVAL06B		},
	{	KBUYVAL07B		},
	{	KBUYVAL08B		},
	{	KBUYVAL09B 		},
	{	KBUYVAL10B 		},
	{	KBUYVAL11B 		},
	{	KBUYVAL12B		},
	{	KBUYVAL13B		},
	{	KBUYVAL01C 		},
	{	KBUYVAL02C 		},
	{	KBUYVAL03C		},
	{	KBUYVAL04C		},
	{	KBUYVAL05C		},
	{	KBUYVAL06C		},
	{	KBUYVAL07C		},
	{	KBUYVAL08C		},
	{	KBUYVAL09C 		},
	{	KBUYVAL10C 		},
	{	KBUYVAL11C 		},
	{	KBUYVAL12C		},
	{	KBUYVAL13C		},

};

static FCSUPDOWN_INFO	FCS_ShinBuyKeika_LR[]	=	{
	{	KBUYVAL01A 		},
	{	KBUYVAL01B 		},
	{	KBUYVAL01C 		},
	{	KBUYVAL02A 		},
	{	KBUYVAL02B 		},
	{	KBUYVAL02C 		},
	{	KBUYVAL03A		},
	{	KBUYVAL03B		},
	{	KBUYVAL03C		},
	{	KBUYVAL04A		},
	{	KBUYVAL04B		},
	{	KBUYVAL04C		},
	{	KBUYVAL05A		},
	{	KBUYVAL05B		},
	{	KBUYVAL05C		},
	{	KBUYVAL06A		},
	{	KBUYVAL06B		},
	{	KBUYVAL06C		},
	{	KBUYKNAME 		},
	{	KBUYVAL07A		},
	{	KBUYVAL07B		},
	{	KBUYVAL07C		},
	{	KBUYVAL08A		},
	{	KBUYVAL08B		},
	{	KBUYVAL08C		},
	{	KBUYVAL09A		},
	{	KBUYVAL09B		},
	{	KBUYVAL09C		},
	{	KBUYVAL10A		},
	{	KBUYVAL10B		},
	{	KBUYVAL10C		},
	{	KBUYVAL11A		},
	{	KBUYVAL11B		},
	{	KBUYVAL11C		},
	{	KBUYVAL12A		},
	{	KBUYVAL12B		},
	{	KBUYVAL12C		},
	{	KBUYVAL13A		},
	{	KBUYVAL13B		},
	{	KBUYVAL13C		},
	{	KBUYVAL14K		},
	{	KBUYVAL14C		},
	{	KBUYVAL14		},
	{	KBUYVAL15K		},
	{	KBUYVAL15C		},
	{	KBUYVAL15		},
	{	KBUYVAL16		},
};

#define		FCS_SHINBUYKEIKACNT	47


//'14.08.20

#define		KBUYLINE1	0
#define		KBUYLINE2	1
#define		KBUYLINE3	2
#define		KBUYLINE4	6
#define		KBUYLINE5	7
#define		KBUYLINE6	8
#define		KBUYLINE7	9
#define		KBUYLINE8	10
#define		KBUYLINE9	11
#define		KBUYLINE10	12
#define		KBUYLINE11	13
#define		KBUYLINE12	14
#define		KBUYLINE13	15
//#define		KBUYLINE14	17
//#define		KBUYLINE15	16
#define		KBUYLINE14	16
#define		KBUYLINE15	17
#define		KBUYLINE16	18


/*
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
#define		KBUYLINE13	12
#define		KBUYLINE14	13
#define		KBUYLINE15	14
#define		KBUYLINE16	15
*/