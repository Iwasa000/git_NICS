#pragma	once


#define		JSWIDECNT	4	//カラムの横の数
#define		JSLINCNT	9	//帳票の行数。この行数でデータを保存していってる
#define		JSVMIN		38	//入力ボックスの最小のインデックス
#define		JSVMAX		73	//入力ボックスの最大のインデックス


//#define		JSYEARID	1

#define		JSZKBNID	1
#define		JSDISP1		29

//全てのIDX定義
#define		JSVAL01A	38
#define		JSVAL01B	39
#define		JSVAL01C	40
#define		JSVAL01D	41
#define		JSVAL02A	42
#define		JSVAL02B	43
#define		JSVAL02C	44
#define		JSVAL02D	45
#define		JSVAL03UA	46
#define		JSVAL03UB	47
#define		JSVAL03UC	48
#define		JSVAL03UD	49
#define		JSVAL03DA	50
#define		JSVAL03DB	51
#define		JSVAL03DC	52
#define		JSVAL03DD	53
#define		JSVAL04UA	54
#define		JSVAL04UB	55
#define		JSVAL04UC	56
#define		JSVAL04UD	57
#define		JSVAL04DA	58
#define		JSVAL04DB	59
#define		JSVAL04DC	60
#define		JSVAL04DD	61
#define		JSVAL05UA	62
#define		JSVAL05UB	63
#define		JSVAL05UC	64
#define		JSVAL05UD	65
#define		JSVAL05DA	66
#define		JSVAL05DB	67
#define		JSVAL05DC	68
#define		JSVAL05DD	69
#define		JSVAL06A	70
#define		JSVAL06B	71
#define		JSVAL06C	72
#define		JSVAL06D	73

// 課税仕入高計算表
static SH_ITEMINDEX	ShinKniinp[]	=	{
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
	{  JSVAL05UA, 0, 1, 0			},
	{  JSVAL05UB, 0, 1, 0			},
	{  JSVAL05UD, 0, 1, 0			},
	{  JSVAL05DA, 0, 1, 0			},
	{  JSVAL05DB, 0, 1, 0			},
	{  JSVAL05DD, 0, 1, 0			},
	{	0, 0, 0, 0					}
};

static FCSUPDOWN_INFO	FCS_ShinKniinp[]	=	{
	{	JSVAL01A 		},
	{	JSVAL02A 		},
	{	JSVAL03UA 		},
	{	JSVAL03DA 		},
	{	JSVAL04UA 		},
	{	JSVAL04DA 		},
	{	JSVAL05UA 		},
	{	JSVAL05DA 		},
	{	JSVAL06A 		},
	{	JSVAL01B 		},
	{	JSVAL02B 		},
	{	JSVAL03UB 		},
	{	JSVAL03DB 		},
	{	JSVAL04UB 		},
	{	JSVAL04DB 		},
	{	JSVAL05UB 		},
	{	JSVAL05DB 		},
	{	JSVAL06B 		},
	{	JSVAL01C 		},
	{	JSVAL02C 		},
	{	JSVAL03UC 		},
	{	JSVAL03DC 		},
	{	JSVAL04UC 		},
	{	JSVAL04DC 		},
	{	JSVAL05UC 		},
	{	JSVAL05DC 		},
	{	JSVAL06C 		},
	{	JSVAL01D 		},
	{	JSVAL02D 		},
	{	JSVAL03UD 		},
	{	JSVAL03DD 		},
	{	JSVAL04UD 		},
//	{	JSVAL04DD 		},
	{	JSVAL05UD 		},
	{	JSVAL05DD 		},
	{	JSVAL06D 		},
};

static FCSUPDOWN_INFO	FCS_ShinKniinp_LR[]	=	{
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
	{	JSVAL05UA 		},
	{	JSVAL05UB 		},
	{	JSVAL05UC 		},
	{	JSVAL05UD 		},
	{	JSVAL05DA 		},
	{	JSVAL05DB 		},
	{	JSVAL05DC 		},
	{	JSVAL05DD 		},
	{	JSVAL06A 		},
	{	JSVAL06B 		},
	{	JSVAL06C 		},
	{	JSVAL06D 		},
};

//#define		FCS_SHINKNIINPCNT	36
#define		FCS_SHINKNIINPCNT	35


#define		KNIINPLINE1		0
#define		KNIINPLINE2		1
#define		KNIINPLINE3U	2
#define		KNIINPLINE3D	3
#define		KNIINPLINE4U	4
#define		KNIINPLINE4D	8
#define		KNIINPLINE5U	5
#define		KNIINPLINE5D	6
#define		KNIINPLINE6		7
