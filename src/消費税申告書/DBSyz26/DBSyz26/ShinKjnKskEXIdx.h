#pragma once


/*
#define	DLG1TAX	26		// 税務署長
#define	DLG1ZIP	27		// 納税地〒
#define	DLG1AD1	28		// 納税地１
#define	DLG1AD2	29		// 納税地２
#define	DLG1TEL	30		// 電話番号
#define	DLG1DKN	31		// 氏名フリガナ
#define	DLG1DNM	32		// 氏名
*/

#define	DLG1SDG	17		// 課税期間　自 元号
#define	DLG1SDT	35		// 課税期間　自
#define	DLG1EDG	18		// 課税期間　至 元号
#define	DLG1EDT	36		// 課税期間　至
#define	DLG1TDG	13		// 提出年月日 元号
#define	DLG1TDT	37		// 提出年月日
#define	DLG1KDG	20		// 更正決定通知書日付 元号
#define	DLG1KDT	38		// 更正決定通知書日付
#define	DLG1KSW	39		// 申告、更正、決定
#define	DLG1REA	40		// 更正の請求をする理由等
#define	DLG1JDG	26		// 更正決定通知書受理年月日 元号
#define	DLG1JDT	41		// 更正決定通知書受理年月日

#define	DLG2BUF	0		// 確定額(　　額)
#define	DLG2VSS	40		// 確定額　最初
#define	DLG2KSS	45		//[Y] 6  貸倒れに係る税額(この請求前の金額)
#define	DLG2MDS	49		//[Y] 10 中間納付税額(この請求前の金額)
#define	DLG2LKS	54		// 地方消費税　譲渡割額　還付額(前)'14.07.22
#define	DLG2LNS	55		// 地方消費税　譲渡割額　納税額(前)'14.07.22
#define	DLG2VSE	56		// 確定額　最後
#define	DLG2VKS	59		// 正当とする額　最初
#define	DLG2KSE	64		//[Y] 6  貸倒れに係る税額(更正の請求金額)
#define	DLG2MDE	68		//[Y] 10 中間納付税額(更正の請求金額)
#define	DLG2LKE	73		// 地方消費税　譲渡割額　還付額'14.07.22
#define	DLG2LNE	74		// 地方消費税　譲渡割額　納税額'14.07.22
#define	DLG2VKE	75		// 正当とする額　最後
#define	DLG2LIN	19		// 金額　行数

#define	DLG3BSW	16		// 金融機関
#define	DLG3BNM	17		// 銀行等名称
#define	DLG3BKN 18		// 銀行等区分
#define	DLG3SNM 19		// 本支店名称
#define	DLG3SKN 20		// 本支店区分
#define	DLG3DNM 21		// 預金
#define	DLG3BAC 22		// 口座番号
#define	DLG3AC1 23		// 口座番号１
#define	DLG3AC2 24		// 口座番号２
#define	DLG3PNM 25		// 郵便局名称
#define	DLG3DOC	26		// 添付書類
#define	DLG3ZNM	27		// 税理士署名
#define	DLG3ZJM 14		// 事務所名


#define	DLG1_TAX	43	// 税務署長
#define	DLG1_ZIP	44	// 納税地〒
#define	DLG1_TL1	45	// 電話番号
#define	DLG1_TL2	46	// 電話番号
#define	DLG1_TL3	47	// 電話番号
#define	DLG1_AD1	48	// 納税地１
#define	DLG1_AD2	49	// 納税地２
#define	DLG1_DKN	50	// 氏名フリガナ
#define	DLG1_DNM	51	// 氏名

//2015.10.20 UPDATE START
//#define	DLG1_KNO	11	// 個人番号
#define	DLG1_KNO	52	// 個人番号
//2015.10.20 UPDATE END

#define	DLG0CMB	6		// 請求or申出


// 更正の請求書（個人）

static SH_ITEMINDEX	KjnKsk1[]	=	{
	{  DLG1TDT, 1, 1, 0			},
	{  DLG1KDT, 1, 1, 0			},
	{  DLG1KSW, 1, 1, 0			},
	{  DLG1REA, 1, 1, 0			},
	{  DLG1JDT, 1, 1, 0			},
	{  DLG1_TAX, 1, 1, 0		},
	{  DLG1_ZIP, 1, 1, 0		},
	{  DLG1_TL1, 1, 1, 0		},
	{  DLG1_TL2, 1, 1, 0		},
	{  DLG1_TL3, 1, 1, 0		},
	{  DLG1_AD1, 1, 1, 0		},
	{  DLG1_AD2, 1, 1, 0		},
	{  DLG1_DKN, 1, 1, 0		},
	{  DLG1_DNM, 1, 1, 0		},
	{	0, 0, 0, 0			}
};
static SH_ITEMINDEX	KjnKsk2[]	=	{
	{  40, 0, 1, 0			},
	{  41, 0, 1, 0			},
	{  42, 0, 1, 0			},
	{  43, 0, 1, 0			},
	{  44, 0, 1, 0			},
	{  45, 0, 1, 0			},
	{  49, 0, 1, 0			},
	{  52, 0, 1, 0			},//'14.09.09
	{  53, 0, 1, 0			},//'14.09.09
	{  54, 0, 1, 0			},//'14.07.22
	{  55, 0, 1, 0			},//'14.07.22
	{  56, 0, 1, 0			},
	{  59, 0, 1, 0			},
	{  60, 0, 1, 0			},
	{  61, 0, 1, 0			},
	{  62, 0, 1, 0			},
	{  63, 0, 1, 0			},
	{  64, 0, 1, 0			},
	{  68, 0, 1, 0			},
	{  71, 0, 1, 0			},
	{  72, 0, 1, 0			},
	{  73, 0, 1, 0			},//'14.07.22
	{  74, 0, 1, 0			},//'14.07.22
	{  75, 0, 1, 0			},
	{	0, 0, 0, 0			}
};
static SH_ITEMINDEX	KjnKsk3[]	=	{
	{  DLG3BSW, 1, 1, 0			},
	{  DLG3BNM, 1, 1, 0			},
	{  DLG3BKN, 1, 1, 0			},
	{  DLG3SNM, 1, 1, 0			},
	{  DLG3SKN, 1, 1, 0			},
	{  DLG3DNM, 1, 1, 0			},
	{  DLG3BAC, 1, 1, 0			},
	{  DLG3AC1, 1, 1, 0			},
	{  DLG3AC2, 1, 1, 0			},
	{  DLG3PNM, 1, 1, 0			},
	{  DLG3DOC, 1, 1, 0			},
	{	0, 0, 0, 0			}
};
static SH_ITEMINDEX	KjnKsk5[]	=	{
	{   DLG0CMB, 1, 1, 0			},
	{	0, 0, 0, 0			}
};
