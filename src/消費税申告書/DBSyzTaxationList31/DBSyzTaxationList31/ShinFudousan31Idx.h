#pragma	once

//'19.07.02

#define		FKWIDECNT	6		//カラムの横の数
#define		FKLINCNT	16		//帳票の行数。この行数でデータを保存していってる
#define		FKVMIN		142		//入力ボックスの最小のインデックス
#define		FKVMAX		237		//入力ボックスの最大のインデックス

#define		FKVLST		127		//？？未使用？

#define		FKYEARID	1

//#define		FKZKBNID	143
#define		FKZKBNID	246		//経理方式：税込みの位置


//全てのIDX定義
#define		KFUDVAL01A	142
#define		KFUDVAL01B	143
#define		KFUDVAL01C	144
#define		KFUDVAL01D	145
#define		KFUDVAL01E	146
#define		KFUDVAL01F	147
#define		KFUDVAL02A	148
#define		KFUDVAL02B	149
#define		KFUDVAL02C	150
#define		KFUDVAL02D	151
#define		KFUDVAL02E	152
#define		KFUDVAL02F	153
#define		KFUDVAL03A	154
#define		KFUDVAL03B	155
#define		KFUDVAL03C	156
#define		KFUDVAL03D	157
#define		KFUDVAL03E	158
#define		KFUDVAL03F	159
#define		KFUDVAL04A	160
#define		KFUDVAL04B	161
#define		KFUDVAL04C	162
#define		KFUDVAL04D	163
#define		KFUDVAL04E	164
#define		KFUDVAL04F	165
#define		KFUDVAL04TA	166
#define		KFUDVAL04TB	167
#define		KFUDVAL04TC	168
#define		KFUDVAL04TD	169
#define		KFUDVAL04TE	170
#define		KFUDVAL04TF	171
#define		KFUDVAL05A	172
#define		KFUDVAL05B	173
#define		KFUDVAL05C	174
#define		KFUDVAL05D	175
#define		KFUDVAL05E	176
#define		KFUDVAL05F	177
#define		KFUDVAL06A	178
#define		KFUDVAL06B	179
#define		KFUDVAL06C	180
#define		KFUDVAL06D	181
#define		KFUDVAL06E	182
#define		KFUDVAL06F	183
#define		KFUDVAL07A	184
#define		KFUDVAL07B	185
#define		KFUDVAL07C	186
#define		KFUDVAL07D	187
#define		KFUDVAL07E	188
#define		KFUDVAL07F	189
#define		KFUDVAL08A	190
#define		KFUDVAL08B	191
#define		KFUDVAL08C	192
#define		KFUDVAL08D	193
#define		KFUDVAL08E	194
#define		KFUDVAL08F	195
#define		KFUDVAL09A	196
#define		KFUDVAL09B	197
#define		KFUDVAL09C	198
#define		KFUDVAL09D	199
#define		KFUDVAL09E	200
#define		KFUDVAL09F	201
#define		KFUDVAL10A	202
#define		KFUDVAL10B	203
#define		KFUDVAL10C	204
#define		KFUDVAL10D	205
#define		KFUDVAL10E	206
#define		KFUDVAL10F	207
#define		KFUDVAL11A	208
#define		KFUDVAL11B	209
#define		KFUDVAL11C	210
#define		KFUDVAL11D	211
#define		KFUDVAL11E	212
#define		KFUDVAL11F	213
#define		KFUDVAL12A	214
#define		KFUDVAL12B	215
#define		KFUDVAL12C	216
#define		KFUDVAL12D	217
#define		KFUDVAL12E	218
#define		KFUDVAL12F	219
#define		KFUDVAL13A	220
#define		KFUDVAL13B	221
#define		KFUDVAL13C	222
#define		KFUDVAL13D	223
#define		KFUDVAL13E	224
#define		KFUDVAL13F	225
#define		KFUDVAL14A	226
#define		KFUDVAL14B	227
#define		KFUDVAL14C	228
#define		KFUDVAL14D	229
#define		KFUDVAL14E	230
#define		KFUDVAL14F	231
#define		KFUDVAL15A	232
#define		KFUDVAL15B	233
#define		KFUDVAL15C	234
#define		KFUDVAL15D	235
#define		KFUDVAL15E	236
#define		KFUDVAL15F	237
#define		KFUDKNAME03	238
#define		KFUDKNAME12	239


#define		FDIAGONALLINE_S	273	// 追加斜線部分開始インデックス
#define		FDIAGONALLINE_E	278	// 追加斜線部分終了インデックス


// 課税取引金額計算表（不動産所得用）
static SH_ITEMINDEX	Fudousan31[]	=	{
	{  KFUDVAL01A, 0, 1, 0			},
	{  KFUDVAL01B, 0, 1, 0			},
	{  KFUDVAL01D, 0, 1, 0			},
	{  KFUDVAL01E, 0, 1, 0			},
	{  KFUDVAL01F, 0, 1, 0			},
	{  KFUDVAL02A, 0, 1, 0			},
	{  KFUDVAL02B, 0, 1, 0			},
	{  KFUDVAL02D, 0, 1, 0			},
	{  KFUDVAL02E, 0, 1, 0			},
	{  KFUDVAL02F, 0, 1, 0			},
	{  KFUDVAL03A, 0, 1, 0			},
	{  KFUDVAL03B, 0, 1, 0			},
	{  KFUDVAL03D, 0, 1, 0			},
	{  KFUDVAL03E, 0, 1, 0			},
	{  KFUDVAL03F, 0, 1, 0			},
	{  KFUDVAL04A, 0, 0, 0			},
	{  KFUDVAL04B, 0, 0, 0			},
	{  KFUDVAL04D, 0, 0, 0			},
	{  KFUDVAL04E, 0, 0, 0			},
	{  KFUDVAL04F, 0, 0, 0			},	
	{  KFUDVAL04TA, 0, 1, 0			},
	{  KFUDVAL04TB, 0, 1, 0			},
	{  KFUDVAL04TD, 0, 1, 0			},
	{  KFUDVAL04TE, 0, 1, 0			},
	{  KFUDVAL04TF, 0, 1, 0			},
	{  KFUDVAL05A, 0, 1, 0			},
	{  KFUDVAL05B, 0, 1, 0			},
	{  KFUDVAL05D, 0, 1, 0			},
	{  KFUDVAL05E, 0, 1, 0			},
	{  KFUDVAL05F, 0, 1, 0			},
	{  KFUDVAL06A, 0, 1, 0			},
	{  KFUDVAL07A, 0, 1, 0			},
	{  KFUDVAL07D, 0, 1, 0			},
	{  KFUDVAL07E, 0, 1, 0			},
	{  KFUDVAL07F, 0, 1, 0			},
	{  KFUDVAL08A, 0, 1, 0			},
	{  KFUDVAL09A, 0, 1, 0			},
	{  KFUDVAL10A, 0, 1, 0			},
	{  KFUDVAL10B, 0, 1, 0			},
	{  KFUDVAL10D, 0, 1, 0			},
	{  KFUDVAL10E, 0, 1, 0			},
	{  KFUDVAL10F, 0, 1, 0			},
	{  KFUDVAL11A, 0, 1, 0			},
	{  KFUDVAL11B, 0, 1, 0			},
	{  KFUDVAL11D, 0, 1, 0			},
	{  KFUDVAL11E, 0, 1, 0			},
	{  KFUDVAL11F, 0, 1, 0			},
	{  KFUDVAL12A, 0, 1, 0			},
	{  KFUDVAL12B, 0, 1, 0			},
	{  KFUDVAL12D, 0, 1, 0			},
	{  KFUDVAL12E, 0, 1, 0			},
	{  KFUDVAL12F, 0, 1, 0			},
	{  KFUDVAL13A, 0, 1, 0			},
	{  KFUDVAL13B, 0, 1, 0			},
	{  KFUDVAL13D, 0, 1, 0			},
	{  KFUDVAL13E, 0, 1, 0			},
	{  KFUDVAL13F, 0, 1, 0			},
	{  KFUDKNAME03, 1, 1, 0			},
	{  KFUDKNAME12, 1, 1, 0			},
	{  0, 0, 0, 0			}
};


//'14.08.18
static FCSUPDOWN_INFO	FCS_ShinFudousan31[]	=	{
	{	KFUDKNAME03 		},
	{	KFUDKNAME12 		},
	{	KFUDVAL01A	 		},
	{	KFUDVAL02A			},
	{	KFUDVAL03A			},
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
	{	KFUDVAL01B 			},
	{	KFUDVAL02B			},
	{	KFUDVAL03B			},
	{	KFUDVAL04TB			},
	{	KFUDVAL05B			},
	{	KFUDVAL10B			},
	{	KFUDVAL11B			},
	{	KFUDVAL12B 			},
	{	KFUDVAL13B			},
	{	KFUDVAL01D 			},
	{	KFUDVAL02D			},
	{	KFUDVAL03D			},
	{	KFUDVAL04TD			},
	{	KFUDVAL05D			},
	{	KFUDVAL07D			},	
	{	KFUDVAL10D			},
	{	KFUDVAL11D			},
	{	KFUDVAL12D 			},
	{	KFUDVAL13D			},
	{	KFUDVAL01E 			},
	{	KFUDVAL02E			},
	{	KFUDVAL03E			},
	{	KFUDVAL04TE			},
	{	KFUDVAL05E			},
	{	KFUDVAL07E			},	
	{	KFUDVAL10E			},
	{	KFUDVAL11E			},
	{	KFUDVAL12E 			},
	{	KFUDVAL13E			},
	{	KFUDVAL01F 			},
	{	KFUDVAL02F			},
	{	KFUDVAL03F			},
	{	KFUDVAL04TF			},
	{	KFUDVAL05F			},
	{	KFUDVAL07F			},
	{	KFUDVAL10F			},
	{	KFUDVAL11F			},
	{	KFUDVAL12F 			},
	{	KFUDVAL13F			},
	{	0			},	// ダミー
	{	0			},
	{	0			},
};


static FCSUPDOWN_INFO	FCS_ShinFudousan31_LR[]	=	{
	{	KFUDVAL01A 		},
	{	KFUDVAL01B 		},
	{	KFUDVAL01D 		},
	{	KFUDVAL01E 		},
	{	KFUDVAL01F 		},
	{	KFUDVAL02A 		},
	{	KFUDVAL02B 		},
	{	KFUDVAL02D 		},
	{	KFUDVAL02E 		},
	{	KFUDVAL02F 		},
	{	KFUDKNAME03		},
	{	KFUDVAL03A 		},
	{	KFUDVAL03B 		},
	{	KFUDVAL03D 		},
	{	KFUDVAL03E 		},
	{	KFUDVAL03F 		},
	{	KFUDVAL04TA		},
	{	KFUDVAL04TB		},
	{	KFUDVAL04TD		},
	{	KFUDVAL04TE		},
	{	KFUDVAL04TF		},
	{	KFUDVAL05A 		},
	{	KFUDVAL05B 		},
	{	KFUDVAL05D 		},
	{	KFUDVAL05E 		},
	{	KFUDVAL05F 		},
	{	KFUDVAL06A 		},
	{	KFUDVAL07A 		},
	{	KFUDVAL07D 		},
	{	KFUDVAL07E 		},
	{	KFUDVAL07F 		},
	{	KFUDVAL08A 		},
	{	KFUDVAL09A 		},
	{	KFUDVAL10A 		},
	{	KFUDVAL10B 		},
	{	KFUDVAL10D 		},
	{	KFUDVAL10E 		},
	{	KFUDVAL10F 		},
	{	KFUDVAL11A 		},
	{	KFUDVAL11B 		},
	{	KFUDVAL11D 		},
	{	KFUDVAL11E 		},
	{	KFUDVAL11F		},
	{	KFUDKNAME12		},
	{	KFUDVAL12A 		},
	{	KFUDVAL12B 		},
	{	KFUDVAL12D 		},
	{	KFUDVAL12E 		},
	{	KFUDVAL12F 		},
	{	KFUDVAL13A 		},
	{	KFUDVAL13B 		},
	{	KFUDVAL13D 		},
	{	KFUDVAL13E 		},
	{	KFUDVAL13F 		},
	{	0 		},	// ダミー
	{	0 		},
	{	0 		},
};

#define		FCS_SHINFUDOUSAN31CNT	54



//'19.07.02
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





// 令和2年4月1日改正用
static SH_ITEMINDEX	R02_Fudousan31[]	=	{
	{  KFUDVAL01A, 0, 1, 0			},
	{  KFUDVAL01B, 0, 1, 0			},
	{  KFUDVAL01D, 0, 1, 0			},
	{  KFUDVAL01E, 0, 0, 0			},
	{  KFUDVAL01F, 0, 1, 0			},
	{  KFUDVAL02A, 0, 1, 0			},
	{  KFUDVAL02B, 0, 1, 0			},
	{  KFUDVAL02D, 0, 1, 0			},
	{  KFUDVAL02E, 0, 0, 0			},
	{  KFUDVAL02F, 0, 1, 0			},
	{  KFUDVAL03A, 0, 1, 0			},
	{  KFUDVAL03B, 0, 1, 0			},
	{  KFUDVAL03D, 0, 1, 0			},
	{  KFUDVAL03E, 0, 1, 0			},
	{  KFUDVAL03F, 0, 1, 0			},
	{  KFUDVAL04A, 0, 0, 0			},
	{  KFUDVAL04B, 0, 0, 0			},
	{  KFUDVAL04D, 0, 0, 0			},
	{  KFUDVAL04E, 0, 0, 0			},
	{  KFUDVAL04F, 0, 0, 0			},	
	{  KFUDVAL04TA, 0, 1, 0			},
	{  KFUDVAL04TB, 0, 1, 0			},
	{  KFUDVAL04TD, 0, 1, 0			},
	{  KFUDVAL04TE, 0, 1, 0			},
	{  KFUDVAL04TF, 0, 1, 0			},
	{  KFUDVAL05A, 0, 1, 0			},
	{  KFUDVAL05B, 0, 1, 0			},
	{  KFUDVAL05D, 0, 1, 0			},
	{  KFUDVAL05E, 0, 0, 0			},
	{  KFUDVAL05F, 0, 1, 0			},
	{  KFUDVAL06A, 0, 1, 0			},
	{  KFUDVAL07A, 0, 1, 0			},
	{  KFUDVAL07D, 0, 1, 0			},
	{  KFUDVAL07E, 0, 0, 0			},
	{  KFUDVAL07F, 0, 1, 0			},
	{  KFUDVAL08A, 0, 1, 0			},
	{  KFUDVAL09A, 0, 1, 0			},
	{  KFUDVAL10A, 0, 1, 0			},
	{  KFUDVAL10B, 0, 1, 0			},
	{  KFUDVAL10D, 0, 1, 0			},
	{  KFUDVAL10E, 0, 0, 0			},
	{  KFUDVAL10F, 0, 1, 0			},
	{  KFUDVAL11A, 0, 1, 0			},
	{  KFUDVAL11B, 0, 1, 0			},
	{  KFUDVAL11D, 0, 1, 0			},
	{  KFUDVAL11E, 0, 0, 0			},
	{  KFUDVAL11F, 0, 1, 0			},
	{  KFUDVAL12A, 0, 1, 0			},
	{  KFUDVAL12B, 0, 1, 0			},
	{  KFUDVAL12D, 0, 1, 0			},
	{  KFUDVAL12E, 0, 1, 0			},
	{  KFUDVAL12F, 0, 1, 0			},
	{  KFUDVAL13A, 0, 1, 0			},
	{  KFUDVAL13B, 0, 1, 0			},
	{  KFUDVAL13D, 0, 1, 0			},
	{  KFUDVAL13E, 0, 1, 0			},
	{  KFUDVAL13F, 0, 1, 0			},
	{  KFUDKNAME03, 1, 1, 0			},
	{  KFUDKNAME12, 1, 1, 0			},
	{  0, 0, 0, 0			}
};


static FCSUPDOWN_INFO	R02_FCS_ShinFudousan31[]	=	{
	{	KFUDKNAME03 		},
	{	KFUDKNAME12 		},
	{	KFUDVAL01A	 		},
	{	KFUDVAL02A			},
	{	KFUDVAL03A			},
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
	{	KFUDVAL01B 			},
	{	KFUDVAL02B			},
	{	KFUDVAL03B			},
	{	KFUDVAL04TB			},
	{	KFUDVAL05B			},
	{	KFUDVAL10B			},
	{	KFUDVAL11B			},
	{	KFUDVAL12B 			},
	{	KFUDVAL13B			},
	{	KFUDVAL01D 			},
	{	KFUDVAL02D			},
	{	KFUDVAL03D			},
	{	KFUDVAL04TD			},
	{	KFUDVAL05D			},
	{	KFUDVAL07D			},	
	{	KFUDVAL10D			},
	{	KFUDVAL11D			},
	{	KFUDVAL12D 			},
	{	KFUDVAL13D			},
	{	KFUDVAL03E			},
	{	KFUDVAL04TE			},
	{	KFUDVAL12E 			},
	{	KFUDVAL13E			},
	{	KFUDVAL01F 			},
	{	KFUDVAL02F			},
	{	KFUDVAL03F			},
	{	KFUDVAL04TF			},
	{	KFUDVAL05F			},
	{	KFUDVAL07F			},
	{	KFUDVAL10F			},
	{	KFUDVAL11F			},
	{	KFUDVAL12F 			},
	{	KFUDVAL13F			},
	{		0				},	// ダミー
	{		0				},
	{		0				},
};


static FCSUPDOWN_INFO	R02_FCS_ShinFudousan31_LR[]	=	{
	{	KFUDVAL01A 		},
	{	KFUDVAL01B 		},
	{	KFUDVAL01D 		},
	{	KFUDVAL01F 		},
	{	KFUDVAL02A 		},
	{	KFUDVAL02B 		},
	{	KFUDVAL02D 		},
	{	KFUDVAL02F 		},
	{	KFUDKNAME03		},
	{	KFUDVAL03A 		},
	{	KFUDVAL03B 		},
	{	KFUDVAL03D 		},
	{	KFUDVAL03E 		},
	{	KFUDVAL03F 		},
	{	KFUDVAL04TA		},
	{	KFUDVAL04TB		},
	{	KFUDVAL04TD		},
	{	KFUDVAL04TE		},
	{	KFUDVAL04TF		},
	{	KFUDVAL05A 		},
	{	KFUDVAL05B 		},
	{	KFUDVAL05D 		},
	{	KFUDVAL05F 		},
	{	KFUDVAL06A 		},
	{	KFUDVAL07A 		},
	{	KFUDVAL07D 		},
	{	KFUDVAL07F 		},
	{	KFUDVAL08A 		},
	{	KFUDVAL08C 		},
	{	KFUDVAL09A 		},
	{	KFUDVAL10A 		},
	{	KFUDVAL10B 		},
	{	KFUDVAL10D 		},
	{	KFUDVAL10F 		},
	{	KFUDVAL11A 		},
	{	KFUDVAL11B 		},
	{	KFUDVAL11D 		},
	{	KFUDVAL11F		},
	{	KFUDKNAME12		},
	{	KFUDVAL12A 		},
	{	KFUDVAL12B 		},
	{	KFUDVAL12D 		},
	{	KFUDVAL12E 		},
	{	KFUDVAL12F 		},
	{	KFUDVAL13A 		},
	{	KFUDVAL13B 		},
	{	KFUDVAL13D 		},
	{	KFUDVAL13E 		},
	{	KFUDVAL13F 		},
	{		0			},	// ダミー
	{		0	 		},
	{		0			},
};

#define		R02_FCS_SHINFUDOUSAN31CNT	49