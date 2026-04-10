#pragma once

#define	ID11A	128	//  1.A ‰ÛÅ•W€Šz
#define	ID11B	129	//  1.B ‰ÛÅ•W€Šz
#define	ID11C	130	//  1.C ‰ÛÅ•W€Šz
#define	ID11D	131	//  1.D ‰ÛÅ•W€Šz

#define	ID12A	132	//  2.A Á”ïÅ
#define	ID12B	133	//  2.B Á”ïÅ
#define	ID12C	134	//  2.C Á”ïÅ
#define	ID12D	135	//  2.D Á”ïÅ

#define	ID13A	136	//  3.A Tœ‰ß‘å’²®Šz
#define	ID13B	137	//  3.B Tœ‰ß‘å’²®Šz
#define	ID13C	138	//  3.C Tœ‰ß‘å’²®Šz
#define	ID13D	139	//  3.D Tœ‰ß‘å’²®Šz

#define	ID14A	140	//  4.A Tœ‘ÎÛd“üÅŠz
#define	ID14B	141	//  4.B Tœ‘ÎÛd“üÅŠz
#define	ID14C	142	//  4.C Tœ‘ÎÛd“üÅŠz
#define	ID14D	143	//  4.D Tœ‘ÎÛd“üÅŠz

#define	ID15A	144	//  5.A •ÔŠÒ“™‘Î‰¿‚ÉŒW‚éÅŠz
#define	ID15B	145	//  5.B •ÔŠÒ“™‘Î‰¿‚ÉŒW‚éÅŠz
#define	ID15C	146	//  5.C •ÔŠÒ“™‘Î‰¿‚ÉŒW‚éÅŠz
#define	ID15D	147	//  5.D •ÔŠÒ“™‘Î‰¿‚ÉŒW‚éÅŠz

#define	ID16A	148	//  6.A ‘İ“|‚ÉŒW‚éÅŠz
#define	ID16B	149	//  6.B ‘İ“|‚ÉŒW‚éÅŠz
#define	ID16C	150	//  6.C ‘İ“|‚ÉŒW‚éÅŠz
#define	ID16D	151	//  6.D ‘İ“|‚ÉŒW‚éÅŠz

#define	ID17A	152	//  7.A TœÅŠz¬Œv
#define	ID17B	153	//  7.B TœÅŠz¬Œv
#define	ID17C	154	//  7.C TœÅŠz¬Œv
#define	ID17D	155	//  7.D TœÅŠz¬Œv

#define	ID18A	156	//  8.A Tœ•s‘«ŠÒ•tÅŠz
#define	ID18B	157	//  8.B Tœ•s‘«ŠÒ•tÅŠz
#define	ID18C	158	//  8.C Tœ•s‘«ŠÒ•tÅŠz
#define	ID18D	159	//  8.D Tœ•s‘«ŠÒ•tÅŠz

#define	ID19A	160	//  9.A ·ˆøÅŠz
#define	ID19B	161	//  9.B ·ˆøÅŠz
#define	ID19C	162	//  9.C ·ˆøÅŠz
#define	ID19D	163	//  9.D ·ˆøÅŠz

#define	ID110D	164	// 10.D ‡Œv·ˆøÅŠz

#define	ID111B	165	// 11.B Tœ•s‘«ŠÒ•tÅŠz
#define	ID111C	166	// 11.C Tœ•s‘«ŠÒ•tÅŠz
#define	ID111D	167	// 11.D Tœ•s‘«ŠÒ•tÅŠz

#define	ID112B	168	// 12.B ·ˆøÅŠz
#define	ID112C	169	// 12.C ·ˆøÅŠz
#define	ID112D	170	// 12.D ·ˆøÅŠz

#define	ID113D	171	// 13.D ‡Œv·ˆøÅŠz

#define	ID114B	172	// 14.B ŠÒ•tŠz
#define	ID114C	173	// 14.C ŠÒ•tŠz
#define	ID114D	174	// 14.D ŠÒ•tŠz

#define	ID115B	175	// 15.B ”[ÅŠz
#define	ID115C	176	// 15.C ”[ÅŠz
#define	ID115D	177	// 15.D ”[ÅŠz

#define	ID116D	178	// 16.D ‡Œv·ˆøÅŠz

// •t•\‚P
static SH_ITEMINDEX	Fhyo1[]	=	{
	{	ID11A, 0, 1, 0		},
	{	ID11B, 0, 1, 0		},
	{	ID11C, 0, 1, 0		},

	{	ID12A, 0, 1, 0		},
	{	ID12B, 0, 1, 0		},
	{	ID12C, 0, 1, 0		},

	{	ID15A, 0, 1, 0		},
	{	ID15B, 0, 1, 0		},
	{	ID15C, 0, 1, 0		},

	{	ID16A, 0, 1, 0		},
	{	ID16B, 0, 1, 0		},
	{	ID16C, 0, 1, 0		},

	{	0,0,0,0				}
};

static FCSUPDOWN_INFO	FCS_Fhyo10[]	=	{
	{	ID11A 		},
	{	ID12A 		},
	{	ID13A 		},
	{	ID14A		},
	{	ID15A		},
	{	ID16A		},
	{	ID17A		},
	{	ID18A		},
	{	ID19A		},

	{	ID11B 		},
	{	ID12B 		},
	{	ID13B 		},
	{	ID14B		},
	{	ID15B		},
	{	ID16B		},
	{	ID17B		},
	{	ID18B		},
	{	ID19B		},
	{	ID111B		},
	{	ID112B		},
	{	ID114B		},
	{	ID115B		},
	
	{	ID11C 		},
	{	ID12C 		},
	{	ID13C 		},
	{	ID14C		},
	{	ID15C		},
	{	ID16C		},
	{	ID17C		},
	{	ID18C		},
	{	ID19C		},
	{	ID111C		},
	{	ID112C		},
	{	ID114C		},
	{	ID115C		},

	{	ID11D 		},
	{	ID12D 		},
	{	ID13D 		},
	{	ID14D		},
	{	ID15D		},
	{	ID16D		},
	{	ID17D		},
	{	ID18D		},
	{	ID19D		},
	{	ID110D		},
	{	ID111D		},
	{	ID112D		},
	{	ID113D		},
	{	ID114D		},
	{	ID115D		},
	{	ID116D		}

};

#define		FCS_FHYO10CNT	51
