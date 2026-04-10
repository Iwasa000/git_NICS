#pragma once

#define		VLIN	19

#define		VMIN	72
#define		VMAX	90


#define		YEARID	1

#define		ZKBNID	92
#define		DISP1	29



//---->'14.08.04
//定義変更。全てのIDX定義'14.08.11
#define		SELKNAME	71
#define		SELVAL01	72
#define		SELVAL02	73
#define		SELVAL03	74
#define		SELVAL04	75
#define		SELVAL05	76
#define		SELVAL06	77
#define		SELVAL07	78
#define		SELVAL08	79
#define		SELVAL09	80
#define		SELVAL10	81
#define		SELVAL11	82
#define		SELVAL12	83
#define		SELVAL13	84
#define		SELVAL14	85
#define		SELVAL15	86
#define		SELVAL16	87
#define		SELVAL17K	88
#define		SELVAL17C	89
#define		SELVAL17	90

//<-------------

// 課税売上高計算表
static SH_ITEMINDEX	Sell[]	=	{
	{  SELKNAME, 1, 1, 0			},
	{  SELVAL01, 0, 1, 0			},
	{  SELVAL02, 0, 1, 0			},
	{  SELVAL04, 0, 1, 0			},
	{  SELVAL05, 0, 1, 0			},
	{  SELVAL07, 0, 1, 0			},
	{  SELVAL08, 0, 1, 0			},
	{  SELVAL10, 1, 1, 0			},
	{  SELVAL11, 1, 1, 0			},
	{  SELVAL13, 0, 1, 0			},
	{  SELVAL14, 0, 1, 0			},
	{  SELVAL17K, 0, 1, 0			},
	{  SELVAL17C, 0, 1, 0			},
	{  SELVAL17, 0, 1, 0			},
	{	0, 0, 0, 0			}
};


//'14.08.04
static FCSUPDOWN_INFO	FCS_ShinSell[]	=	{
	{	SELKNAME 		},
	{	SELVAL01 		},
	{	SELVAL02 		},
	{	SELVAL03		},
	{	SELVAL04		},
	{	SELVAL05		},
	{	SELVAL06		},
	{	SELVAL07		},
	{	SELVAL08		},
	{	SELVAL09 		},
	{	SELVAL10 		},
	{	SELVAL11 		},
	{	SELVAL12		},
	{	SELVAL13		},
	{	SELVAL14		},
	{	SELVAL15		},
	{	SELVAL16		},
	{	SELVAL17K		},
	{	SELVAL17C		},
	{	SELVAL17		},
};

static FCSUPDOWN_INFO	FCS_ShinSell_LR[]	=	{
	{	SELVAL01 		},
	{	SELVAL02 		},
	{	SELVAL03		},
	{	SELVAL04		},
	{	SELVAL05		},
	{	SELVAL06		},
	{	SELVAL07		},
	{	SELVAL08		},
	{	SELVAL09 		},
	{	SELKNAME 		},
	{	SELVAL10 		},
	{	SELVAL11 		},
	{	SELVAL12		},
	{	SELVAL13		},
	{	SELVAL14		},
	{	SELVAL15		},
	{	SELVAL16		},
	{	SELVAL17K		},
	{	SELVAL17C		},
	{	SELVAL17		},
};

#define		FCS_SHINSELLCNT	20

#define		SELLINE1	0
#define		SELLINE2	1
#define		SELLINE3	2
#define		SELLINE4	3
#define		SELLINE5	4
#define		SELLINE6	5
#define		SELLINE7	6
#define		SELLINE8	7
#define		SELLINE9	8
#define		SELLINE10	9
#define		SELLINE11	10
#define		SELLINE12	11
#define		SELLINE13	12
#define		SELLINE14	13
#define		SELLINE15	14
#define		SELLINE16	15
#define		SELLINE17	16