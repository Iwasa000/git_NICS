#pragma	once


#define		JSWIDECNT	4	//カラムの横の数
#define		JSLINCNT	9	//帳票の行数。この行数でデータを保存していってる
#define		JSVMIN		42	//入力ボックスの最小のインデックス
#define		JSVMAX		77	//入力ボックスの最大のインデックス


//#define		JSYEARID	1

#define		JSZKBNID	1
#define		JSDISP1		29

//全てのIDX定義
#define		JSVAL01A	42
#define		JSVAL01B	43
#define		JSVAL01C	44
#define		JSVAL01D	45
#define		JSVAL02A	46
#define		JSVAL02B	47
#define		JSVAL02C	48
#define		JSVAL02D	49
#define		JSVAL03UA	50
#define		JSVAL03UB	51
#define		JSVAL03UC	52
#define		JSVAL03UD	53
#define		JSVAL03DA	54
#define		JSVAL03DB	55
#define		JSVAL03DC	56
#define		JSVAL03DD	57
#define		JSVAL04UA	58
#define		JSVAL04UB	59
#define		JSVAL04UC	60
#define		JSVAL04UD	61
#define		JSVAL04DA	62
#define		JSVAL04DB	63
#define		JSVAL04DC	64
#define		JSVAL04DD	65
#define		JSVAL05A	66
#define		JSVAL05B	67
#define		JSVAL05C	68
#define		JSVAL05D	69
#define		JSVAL06A	70
#define		JSVAL06B	71
#define		JSVAL06C	72
#define		JSVAL06D	73
#define		JSVAL07A	74
#define		JSVAL07B	75
#define		JSVAL07C	76
#define		JSVAL07D	77

// 課税仕入高計算表
static SH_ITEMINDEX	ShinKniinp6syu[]	=	{
	{  JSVAL01A, 0, 1, 0			},
	{  JSVAL01B, 0, 1, 0			},
	{  JSVAL01D, 0, 1, 0			},
	{  JSVAL02A, 0, 1, 0			},
	{  JSVAL02B, 0, 1, 0			},
	{  JSVAL02D, 0, 1, 0			},
	{  JSVAL03UA, 0, 1, 0			},
	{  JSVAL03UB, 0, 1, 0			},
	{  JSVAL03UD, 0, 1, 0			},
	{  JSVAL03DA, 0, 1, 0			},
	{  JSVAL03DB, 0, 1, 0			},
	{  JSVAL03DD, 0, 1, 0			},
	{  JSVAL04UA, 0, 1, 0			},
	{  JSVAL04UB, 0, 1, 0			},
	{  JSVAL04UD, 0, 1, 0			},
	{  JSVAL04DA, 0, 1, 0			},
	{  JSVAL04DB, 0, 1, 0			},
//	{  JSVAL04DD, 0, 1, 0			},
	{  JSVAL05A, 0, 1, 0			},
	{  JSVAL05B, 0, 1, 0			},
	{  JSVAL05D, 0, 1, 0			},
	{  JSVAL06A, 0, 1, 0			},
	{  JSVAL06B, 0, 1, 0			},
	{  JSVAL06D, 0, 1, 0			},
	{	0, 0, 0, 0					}
};

static FCSUPDOWN_INFO	FCS_ShinKniinp6syu[]	=	{
	{	JSVAL01A 		},
	{	JSVAL02A 		},
	{	JSVAL03UA 		},
	{	JSVAL03DA 		},
	{	JSVAL04UA 		},
	{	JSVAL04DA 		},
	{	JSVAL05A 		},
	{	JSVAL06A 		},
	{	JSVAL07A 		},
	{	JSVAL01B 		},
	{	JSVAL02B 		},
	{	JSVAL03UB 		},
	{	JSVAL03DB 		},
	{	JSVAL04UB 		},
	{	JSVAL04DB 		},
	{	JSVAL05B 		},
	{	JSVAL06B 		},
	{	JSVAL07B 		},
	{	JSVAL01C 		},
	{	JSVAL02C 		},
	{	JSVAL03UC 		},
	{	JSVAL03DC 		},
	{	JSVAL04UC 		},
	{	JSVAL04DC 		},
	{	JSVAL05C 		},
	{	JSVAL06C 		},
	{	JSVAL07C 		},
	{	JSVAL01D 		},
	{	JSVAL02D 		},
	{	JSVAL03UD 		},
	{	JSVAL03DD 		},
	{	JSVAL04UD 		},
//	{	JSVAL04DD 		},
	{	JSVAL05D 		},
	{	JSVAL06D 		},
	{	JSVAL07D 		},
};

static FCSUPDOWN_INFO	FCS_ShinKniinp6syu_LR[]	=	{
	{	JSVAL01A 		},
	{	JSVAL01B 		},
	{	JSVAL01C 		},
	{	JSVAL01D 		},
	{	JSVAL02A 		},
	{	JSVAL02B 		},
	{	JSVAL02C 		},
	{	JSVAL02D 		},
	{	JSVAL03UA 		},
	{	JSVAL03UB 		},
	{	JSVAL03UC 		},
	{	JSVAL03UD 		},
	{	JSVAL03DA 		},
	{	JSVAL03DB 		},
	{	JSVAL03DC 		},
	{	JSVAL03DD 		},
	{	JSVAL04UA 		},
	{	JSVAL04UB 		},
	{	JSVAL04UC 		},
	{	JSVAL04UD 		},
	{	JSVAL04DA 		},
	{	JSVAL04DB 		},
	{	JSVAL04DC 		},
//	{	JSVAL04DD 		},
	{	JSVAL05A 		},
	{	JSVAL05B 		},
	{	JSVAL05C 		},
	{	JSVAL05D 		},
	{	JSVAL06A 		},
	{	JSVAL06B 		},
	{	JSVAL06C 		},
	{	JSVAL06D 		},
	{	JSVAL07A 		},
	{	JSVAL07B 		},
	{	JSVAL07C 		},
	{	JSVAL07D 		},
};

//#define		FCS_SHINKNIINPCNT	36
#define		FCS_SHINKNIINPCNT	35


#define		KNIINPLINE1		0
#define		KNIINPLINE2		1
#define		KNIINPLINE3U	2
#define		KNIINPLINE3D	3
#define		KNIINPLINE4U	4
#define		KNIINPLINE4D	8
#define		KNIINPLINE5		5
#define		KNIINPLINE6		6
#define		KNIINPLINE7		7
