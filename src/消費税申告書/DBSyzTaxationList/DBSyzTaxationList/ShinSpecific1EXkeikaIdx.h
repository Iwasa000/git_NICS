#pragma	once

//-----------------------------------------------------------------------------
// 計算表１：インデックスファイル
//-----------------------------------------------------------------------------

#define	ID20 36		// 1A.課税売上（免税・非課税売上・値引を含まない）４％		[SpKzur]
#define	ID21 37		// 2A.課税売上（免税・非課税売上・値引を含まない）４％		[SpKzur]
#define	ID30 38		// 1B.課税売上（免税・非課税売上・値引を含まない）６．３％	[SpKzur]
#define	ID31 39		// 2B.課税売上（免税・非課税売上・値引を含まない）６．３％	[SpKzur]
#define	ID10 40		// 1C.課税売上（免税・非課税売上・値引を含まない）合計		[SpKzur]
#define	ID11 41		// 2C.課税売上（免税・非課税売上・値引を含まない）合計		[SpKzur]
#define	ID12 42		// 3C.免税売上												[SpMzur]
#define	ID13 43		// 4C.非課税売上（有価証券を全額で含む）					[SpHkur]
#define	ID14 44		// 5C.国外における資産の譲渡等の対価の額					[SpKgjt]
#define	ID15 45		// 6C.資産の譲渡等の対価の合計額							[SpTuwb]


SH_ITEMINDEX	Specific1ExKeika[]	=	{
	{	ID20,	0, 0, 0		},
	{	ID21,	0, 0, 0		},
	{	ID30,	0, 0, 0		},
	{	ID31,	0, 0, 0		},
	{	ID10,	0, 0, 0		},
	{	ID11,	0, 0, 0		},
	{	ID12,	0, 0, 0		},
	{	ID13,	0, 1, 0		},
	{	ID14,	1, 1, 0		},
	{	   0,	0, 0, 0		}
};


static FCSUPDOWN_INFO	FCS_Specific1ExKeika[]	=	{
	{	ID20 		},
	{	ID21 		},
	{	ID30 		},
	{	ID31 		},
	{	ID10 		},
	{	ID11 		},
	{	ID12 		},
	{	ID13 		},
	{	ID14 		},
	{	ID15 		}
};

static FCSUPDOWN_INFO	FCS_Specific1ExKeika_LR[]	=	{
	{	ID20 		},
	{	ID30 		},
	{	ID10 		},
	{	ID21 		},
	{	ID31 		},
	{	ID11 		},
	{	ID12 		},
	{	ID13 		},
	{	ID14 		},
	{	ID15 		}	
};

#define		FCS_SPECIFIC1CNT	10