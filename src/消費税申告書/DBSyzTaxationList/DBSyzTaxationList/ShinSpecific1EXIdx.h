#pragma	once

//-----------------------------------------------------------------------------
// 計算表１：インデックスファイル
//-----------------------------------------------------------------------------

#define	ID10 22		// 1.課税売上（免税・非課税売上・値引を含まない）	[SpKzur]
#define	ID11 23		// 2.課税売上（免税・非課税売上・値引を含まない）	[SpKzur]
#define	ID12 24		// 3.免税売上										[SpMzur]
#define	ID13 25		// 4.非課税売上（有価証券を全額で含む）				[SpHkur]
#define	ID14 26		// 5.国外における資産の譲渡等の対価の額				[SpKgjt]
#define	ID15 27		// 6.資産の譲渡等の対価の合計額						[SpTuwb]


SH_ITEMINDEX	Specific1Ex[]	=	{
	{	ID10,	0, 0, 0		},
	{	ID11,	0, 0, 0		},
	{	ID12,	0, 0, 0		},
	{	ID13,	0, 1, 0		},
	{	ID14,	1, 1, 0		},
	{	   0,	0, 0, 0		}
};


static FCSUPDOWN_INFO	FCS_Specific1Ex[]	=	{
	{	ID10 		},
	{	ID11 		},
	{	ID12 		},
	{	ID13 		},
	{	ID14 		},
	{	ID15 		}
};

static FCSUPDOWN_INFO	FCS_Specific1Ex_LR[]	=	{
	{	ID10 		},
	{	ID11 		},
	{	ID12 		},
	{	ID13 		},
	{	ID14 		},
	{	ID15 		}
	
};

#define		FCS_SPECIFIC1CNT	6