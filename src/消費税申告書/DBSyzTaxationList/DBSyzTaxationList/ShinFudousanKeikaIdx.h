#pragma	once

//'14.08.18

#define		FKWIDECNT	5		//カラムの横の数
#define		FKLINCNT	16		//帳票の行数。この行数でデータを保存していってる
#define		FKVMIN		124		//入力ボックスの最小のインデックス
#define		FKVMAX		203		//入力ボックスの最大のインデックス

#define		FKVLST	127

#define		FKYEARID	1

//#define		FKZKBNID	143
#define		FKZKBNID	212


//全てのIDX定義
#define		KFUDVAL01A	124
#define		KFUDVAL01B	125
#define		KFUDVAL01C	126
#define		KFUDVAL01D	127
#define		KFUDVAL01E	128
#define		KFUDVAL02A	129
#define		KFUDVAL02B	130
#define		KFUDVAL02C	131
#define		KFUDVAL02D	132
#define		KFUDVAL02E	133
#define		KFUDVAL03A	134
#define		KFUDVAL03B	135
#define		KFUDVAL03C	136
#define		KFUDVAL03D	137
#define		KFUDVAL03E	138
#define		KFUDVAL04A	139
#define		KFUDVAL04B	140
#define		KFUDVAL04C	141
#define		KFUDVAL04D	142
#define		KFUDVAL04E	143
#define		KFUDVAL04TA	144
#define		KFUDVAL04TB	145
#define		KFUDVAL04TC	146
#define		KFUDVAL04TD	147
#define		KFUDVAL04TE	148
#define		KFUDVAL05A	149
#define		KFUDVAL05B	150
#define		KFUDVAL05C	151
#define		KFUDVAL05D	152
#define		KFUDVAL05E	153
#define		KFUDVAL06A	154
#define		KFUDVAL06B	155
#define		KFUDVAL06C	156
#define		KFUDVAL06D	157
#define		KFUDVAL06E	158
#define		KFUDVAL07A	159
#define		KFUDVAL07B	160
#define		KFUDVAL07C	161
#define		KFUDVAL07D	162
#define		KFUDVAL07E	163
#define		KFUDVAL08A	164
#define		KFUDVAL08B	165
#define		KFUDVAL08C	166
#define		KFUDVAL08D	167
#define		KFUDVAL08E	168
#define		KFUDVAL09A	169
#define		KFUDVAL09B	170
#define		KFUDVAL09C	171
#define		KFUDVAL09D	172
#define		KFUDVAL09E	173
#define		KFUDVAL10A	174
#define		KFUDVAL10B	175
#define		KFUDVAL10C	176
#define		KFUDVAL10D	177
#define		KFUDVAL10E	178
#define		KFUDVAL11A	179
#define		KFUDVAL11B	180
#define		KFUDVAL11C	181
#define		KFUDVAL11D	182
#define		KFUDVAL11E	183
#define		KFUDVAL12A	184
#define		KFUDVAL12B	185
#define		KFUDVAL12C	186
#define		KFUDVAL12D	187
#define		KFUDVAL12E	188
#define		KFUDVAL13A	189
#define		KFUDVAL13B	190
#define		KFUDVAL13C	191
#define		KFUDVAL13D	192
#define		KFUDVAL13E	193
#define		KFUDVAL14A	194
#define		KFUDVAL14B	195
#define		KFUDVAL14C	196
#define		KFUDVAL14D	197
#define		KFUDVAL14E	198
#define		KFUDVAL15A	199
#define		KFUDVAL15B	200
#define		KFUDVAL15C	201
#define		KFUDVAL15D	202
#define		KFUDVAL15E	203
#define		KFUDKNAME03	204
#define		KFUDKNAME12	205


// 課税取引金額計算表（不動産所得用）
static SH_ITEMINDEX	FudousanKeika[]	=	{
	{  KFUDVAL01A, 0, 1, 0			},
	{  KFUDVAL01B, 0, 1, 0			},
	{  KFUDVAL01D, 0, 1, 0			},
	{  KFUDVAL01E, 0, 1, 0			},
	{  KFUDVAL02A, 0, 1, 0			},
	{  KFUDVAL02B, 0, 1, 0			},
	{  KFUDVAL02D, 0, 1, 0			},
	{  KFUDVAL02E, 0, 1, 0			},
	{  KFUDVAL03A, 0, 1, 0			},
	{  KFUDVAL03B, 0, 1, 0			},
	{  KFUDVAL03D, 0, 1, 0			},
	{  KFUDVAL03E, 0, 1, 0			},
	{  KFUDVAL04TA, 0, 1, 0			},
	{  KFUDVAL04TB, 0, 1, 0			},
	{  KFUDVAL04TD, 0, 1, 0			},
	{  KFUDVAL04TE, 0, 1, 0			},
	{  KFUDVAL05A, 0, 1, 0			},
	{  KFUDVAL05B, 0, 1, 0			},
	{  KFUDVAL05D, 0, 1, 0			},
	{  KFUDVAL05E, 0, 1, 0			},
	{  KFUDVAL06A, 0, 1, 0			},
	{  KFUDVAL07A, 0, 1, 0			},
	{  KFUDVAL07D, 0, 1, 0			},
	{  KFUDVAL07E, 0, 1, 0			},
	{  KFUDVAL08A, 0, 1, 0			},
	{  KFUDVAL09A, 0, 1, 0			},
	{  KFUDVAL10A, 0, 1, 0			},
	{  KFUDVAL10B, 0, 1, 0			},
	{  KFUDVAL10D, 0, 1, 0			},
	{  KFUDVAL10E, 0, 1, 0			},
	{  KFUDVAL11A, 0, 1, 0			},
	{  KFUDVAL11B, 0, 1, 0			},
	{  KFUDVAL11D, 0, 1, 0			},
	{  KFUDVAL11E, 0, 1, 0			},
	{  KFUDVAL12A, 0, 1, 0			},
	{  KFUDVAL12B, 0, 1, 0			},
	{  KFUDVAL12D, 0, 1, 0			},
	{  KFUDVAL12E, 0, 1, 0			},
	{  KFUDVAL13A, 0, 1, 0			},
	{  KFUDVAL13B, 0, 1, 0			},
	{  KFUDVAL13D, 0, 1, 0			},
	{  KFUDVAL13E, 0, 1, 0			},
	{  KFUDKNAME03, 1, 1, 0			},
	{  KFUDKNAME12, 1, 1, 0			},
	{  0, 0, 0, 0			}
};


//'14.08.18
static FCSUPDOWN_INFO	FCS_ShinFudousanKeika[]	=	{
	{	KFUDKNAME03 		},
	{	KFUDKNAME12 		},
	{	KFUDVAL01A	 		},
	{	KFUDVAL02A			},
	{	KFUDVAL03A			},
	{	KFUDVAL04A 			},
	{	KFUDVAL04TA			},
	{	KFUDVAL05A			},
	{	KFUDVAL06A 			},	
	{	KFUDVAL07A			},	
	{	KFUDVAL08A			},
	{	KFUDVAL09A 			},
	{	KFUDVAL10A			},
	{	KFUDVAL11A			},
	{	KFUDVAL12A 			},
	{	KFUDVAL13A			},
	{	KFUDVAL14A			},
	{	KFUDVAL15A			},
	{	KFUDVAL01B 			},
	{	KFUDVAL02B			},
	{	KFUDVAL03B			},
	{	KFUDVAL04B 			},
	{	KFUDVAL04TB			},
	{	KFUDVAL05B			},
	{	KFUDVAL06B 			},	
	{	KFUDVAL07B			},	
	{	KFUDVAL08B			},
	{	KFUDVAL09B 			},
	{	KFUDVAL10B			},
	{	KFUDVAL11B			},
	{	KFUDVAL12B 			},
	{	KFUDVAL13B			},
	{	KFUDVAL14B			},
	{	KFUDVAL15B			},
	{	KFUDVAL01C 			},
	{	KFUDVAL02C			},
	{	KFUDVAL03C			},
	{	KFUDVAL04C 			},
	{	KFUDVAL04TC			},
	{	KFUDVAL05C			},
	{	KFUDVAL06C 			},	
	{	KFUDVAL07C			},	
	{	KFUDVAL08C			},
	{	KFUDVAL09C 			},
	{	KFUDVAL10C			},
	{	KFUDVAL11C			},
	{	KFUDVAL12C 			},
	{	KFUDVAL13C			},
	{	KFUDVAL14C			},
	{	KFUDVAL15C			},
	{	KFUDVAL01D 			},
	{	KFUDVAL02D			},
	{	KFUDVAL03D			},
	{	KFUDVAL04D 			},
	{	KFUDVAL04TD			},
	{	KFUDVAL05D			},
	{	KFUDVAL06D 			},	
	{	KFUDVAL07D			},	
	{	KFUDVAL08D			},
	{	KFUDVAL09D 			},
	{	KFUDVAL10D			},
	{	KFUDVAL11D			},
	{	KFUDVAL12D 			},
	{	KFUDVAL13D			},
	{	KFUDVAL14D			},
	{	KFUDVAL15D			},
	{	KFUDVAL01E 			},
	{	KFUDVAL02E			},
	{	KFUDVAL03E			},
	{	KFUDVAL04E 			},
	{	KFUDVAL04TE			},
	{	KFUDVAL05E			},
	{	KFUDVAL06E 			},	
	{	KFUDVAL07E			},	
	{	KFUDVAL08E			},
	{	KFUDVAL09E 			},
	{	KFUDVAL10E			},
	{	KFUDVAL11E			},
	{	KFUDVAL12E 			},
	{	KFUDVAL13E			},
	{	KFUDVAL14E			},
	{	KFUDVAL15E			},
};


static FCSUPDOWN_INFO	FCS_ShinFudousanKeika_LR[]	=	{
	{	KFUDVAL01A 		},
	{	KFUDVAL01B 		},
	{	KFUDVAL01C 		},
	{	KFUDVAL01D 		},
	{	KFUDVAL01E 		},
	{	KFUDVAL02A 		},
	{	KFUDVAL02B 		},
	{	KFUDVAL02C 		},
	{	KFUDVAL02D 		},
	{	KFUDVAL02E 		},
	{	KFUDKNAME03 		},
	{	KFUDVAL03A 		},
	{	KFUDVAL03B 		},
	{	KFUDVAL03C 		},
	{	KFUDVAL03D 		},
	{	KFUDVAL03E 		},
	{	KFUDVAL04A 		},
	{	KFUDVAL04B 		},
	{	KFUDVAL04C 		},
	{	KFUDVAL04D 		},
	{	KFUDVAL04E 		},
	{	KFUDVAL04TA		},
	{	KFUDVAL04TB		},
	{	KFUDVAL04TC		},
	{	KFUDVAL04TD		},
	{	KFUDVAL04TE		},
	{	KFUDVAL05A 		},
	{	KFUDVAL05B 		},
	{	KFUDVAL05C 		},
	{	KFUDVAL05D 		},
	{	KFUDVAL05E 		},
	{	KFUDVAL06A 		},
	{	KFUDVAL06B 		},
	{	KFUDVAL06C 		},
	{	KFUDVAL06D 		},
	{	KFUDVAL06E 		},
	{	KFUDVAL07A 		},
	{	KFUDVAL07B 		},
	{	KFUDVAL07C 		},
	{	KFUDVAL07D 		},
	{	KFUDVAL07E 		},
	{	KFUDVAL08A 		},
	{	KFUDVAL08B 		},
	{	KFUDVAL08C 		},
	{	KFUDVAL08D 		},
	{	KFUDVAL08E 		},
	{	KFUDVAL09A 		},
	{	KFUDVAL09B 		},
	{	KFUDVAL09C 		},
	{	KFUDVAL09D 		},
	{	KFUDVAL09E 		},
	{	KFUDVAL10A 		},
	{	KFUDVAL10B 		},
	{	KFUDVAL10C 		},
	{	KFUDVAL10D 		},
	{	KFUDVAL10E 		},
	{	KFUDVAL11A 		},
	{	KFUDVAL11B 		},
	{	KFUDVAL11C 		},
	{	KFUDVAL11D 		},
	{	KFUDVAL11E 		},
	{	KFUDKNAME12		},
	{	KFUDVAL12A 		},
	{	KFUDVAL12B 		},
	{	KFUDVAL12C 		},
	{	KFUDVAL12D 		},
	{	KFUDVAL12E 		},
	{	KFUDVAL13A 		},
	{	KFUDVAL13B 		},
	{	KFUDVAL13C 		},
	{	KFUDVAL13D 		},
	{	KFUDVAL13E 		},
	{	KFUDVAL14A 		},
	{	KFUDVAL14B 		},
	{	KFUDVAL14C 		},
	{	KFUDVAL14D 		},
	{	KFUDVAL14E 		},
	{	KFUDVAL15A 		},
	{	KFUDVAL15B 		},
	{	KFUDVAL15C 		},
	{	KFUDVAL15D 		},
	{	KFUDVAL15E 		},
};

#define		FCS_SHINFUDOUSANKEIKACNT	82



//'14.08.21
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






