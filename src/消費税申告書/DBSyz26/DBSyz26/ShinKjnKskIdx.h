#pragma once

#define	DLG1TAX	26		// 税務署長
#define	DLG1ZIP	27		// 納税地〒
#define	DLG1AD1	28		// 納税地１
#define	DLG1AD2	29		// 納税地２
#define	DLG1TEL	30		// 電話番号
#define	DLG1DKN	31		// 氏名フリガナ
#define	DLG1DNM	32		// 氏名
#define	DLG1SDT	33		// 課税期間　自
#define	DLG1EDT	34		// 課税期間　至
#define	DLG1TDT	35		// 提出年月日
#define	DLG1KDT	36		// 更正決定通知書日付
#define	DLG1KSW	37		// 申告、更正、決定
#define	DLG1REA	38		// 更正の請求をする理由等
#define	DLG1JDT	39		// 更正決定通知書受理年月日

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

//★//[12'11.14]///
#define	DLG1_TAX	41	// 税務署長
#define	DLG1_ZIP	42	// 納税地〒
#define	DLG1_TL1	43	// 電話番号
#define	DLG1_TL2	44	// 電話番号
#define	DLG1_TL3	45	// 電話番号
#define	DLG1_AD1	46	// 納税地１
#define	DLG1_AD2	47	// 納税地２
#define	DLG1_DKN	48	// 氏名フリガナ
#define	DLG1_DNM	49	// 氏名
////////////////////

//★//[11'12.13]///
#define	DLG0CMB	6		// 請求or申出
///////////////////

// 更正の請求書（個人）

static SH_ITEMINDEX	KjnKsk1[]	=	{
	{  35, 1, 1, 0			},
	{  36, 1, 1, 0			},
	{  37, 1, 1, 0			},
	{  38, 1, 1, 0			},
	{  39, 1, 1, 0			},
	{  41, 1, 1, 0			},
	{  42, 1, 1, 0			},
	{  43, 1, 1, 0			},
	{  44, 1, 1, 0			},
	{  45, 1, 1, 0			},
	{  46, 1, 1, 0			},
	{  47, 1, 1, 0			},
	{  48, 1, 1, 0			},
	{  49, 1, 1, 0			},
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
	{  73, 0, 1, 0			},//'14.07.22
	{  74, 0, 1, 0			},//'14.07.22
	{  75, 0, 1, 0			},
	{	0, 0, 0, 0			}
};
static SH_ITEMINDEX	KjnKsk3[]	=	{
	{  16, 1, 1, 0			},
	{  17, 1, 1, 0			},
	{  18, 1, 1, 0			},
	{  19, 1, 1, 0			},
	{  20, 1, 1, 0			},
	{  21, 1, 1, 0			},
	{  22, 1, 1, 0			},
	{  23, 1, 1, 0			},
	{  24, 1, 1, 0			},
	{  25, 1, 1, 0			},
	{  26, 1, 1, 0			},
	{	0, 0, 0, 0			}
};
static SH_ITEMINDEX	KjnKsk5[]	=	{
	{   6, 1, 1, 0			},
	{	0, 0, 0, 0			}
};
