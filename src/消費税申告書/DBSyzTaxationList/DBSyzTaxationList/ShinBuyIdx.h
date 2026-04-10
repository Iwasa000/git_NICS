#pragma	once


//#define		VLIN	19
#define		LINCNT	17	//帳票の行数。この行数でデータを保存していってる
#define		VMIN	72	//入力ボックスの最小のインデックス
#define		VMAX	90	//入力ボックスの最大のインデックス



#define		YEARID	1

#define		ZKBNID	91
#define		DISP1	29



//'14.08.01
//定義変更。全てのIDX定義'14.08.11
#define		BUYKNAME	71
#define		BUYVAL01	72
#define		BUYVAL02	73
#define		BUYVAL03	74
#define		BUYVAL04	75
#define		BUYVAL05	76
#define		BUYVAL06	77
#define		BUYVAL07	78
#define		BUYVAL08	79
#define		BUYVAL09	80
#define		BUYVAL10	81
#define		BUYVAL11	82
#define		BUYVAL12	83
#define		BUYVAL13	84
#define		BUYVAL14	85
#define		BUYVAL15	86
#define		BUYVAL16	87
#define		BUYVAL17K	88
#define		BUYVAL17C	89
#define		BUYVAL17	90

// 課税仕入高計算表
static SH_ITEMINDEX	Buy[]	=	{
	{  BUYKNAME, 1, 1, 0			},
	{  BUYVAL01, 0, 1, 0			},
	{  BUYVAL02, 0, 1, 0			},
	{  BUYVAL04, 0, 1, 0			},
	{  BUYVAL05, 0, 1, 0			},
	{  BUYVAL07, 0, 1, 0			},
	{  BUYVAL08, 0, 1, 0			},
	{  BUYVAL10, 1, 1, 0			},
	{  BUYVAL11, 1, 1, 0			},
	{  BUYVAL13, 0, 1, 0			},
	{  BUYVAL14, 0, 1, 0			},
	{  BUYVAL17K, 0, 1, 0			},
	{  BUYVAL17C, 0, 1, 0			},
	{  BUYVAL17, 0, 1, 0			},
	{	0, 0, 0, 0					}
};

static FCSUPDOWN_INFO	FCS_ShinBuy[]	=	{
	{	BUYKNAME 		},
	{	BUYVAL01 		},
	{	BUYVAL02 		},
	{	BUYVAL03		},
	{	BUYVAL04		},
	{	BUYVAL05		},
	{	BUYVAL06		},
	{	BUYVAL07		},
	{	BUYVAL08		},
	{	BUYVAL09 		},
	{	BUYVAL10 		},
	{	BUYVAL11 		},
	{	BUYVAL12		},
	{	BUYVAL13		},
	{	BUYVAL14		},
	{	BUYVAL15		},
	{	BUYVAL16		},
	{	BUYVAL17K		},
	{	BUYVAL17C		},
	{	BUYVAL17		},
};

static FCSUPDOWN_INFO	FCS_ShinBuy_LR[]	=	{
	{	BUYVAL01 		},
	{	BUYVAL02 		},
	{	BUYVAL03		},
	{	BUYVAL04		},
	{	BUYVAL05		},
	{	BUYVAL06		},
	{	BUYVAL07		},
	{	BUYVAL08		},
	{	BUYVAL09 		},
	{	BUYKNAME 		},
	{	BUYVAL10 		},
	{	BUYVAL11 		},
	{	BUYVAL12		},
	{	BUYVAL13		},
	{	BUYVAL14		},
	{	BUYVAL15		},
	{	BUYVAL16		},
	{	BUYVAL17K		},
	{	BUYVAL17C		},
	{	BUYVAL17		},
};

#define		FCS_SHINBUYCNT	20


//'14.08.19
#define		BUYLINE1	0
#define		BUYLINE2	1
#define		BUYLINE3	2
#define		BUYLINE4	3
#define		BUYLINE5	4
#define		BUYLINE6	5
#define		BUYLINE7	6
#define		BUYLINE8	7
#define		BUYLINE9	8
#define		BUYLINE10	9
#define		BUYLINE11	10
#define		BUYLINE12	11
#define		BUYLINE13	12
#define		BUYLINE14	13
#define		BUYLINE15	14
#define		BUYLINE16	15
#define		BUYLINE17	16