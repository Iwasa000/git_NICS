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
#define	DLG1EDG	18		// 課税期間　至 元号

#define	DLG1SDT	35		// 課税期間　自
#define	DLG1EDT	36		// 課税期間　至
#define	DLG1TDT	37		// 提出年月日
#define	DLG1KDT	38		// 更正決定通知書日付
#define	DLG1KSW	39		// 申告、更正、決定
#define	DLG1REA	40		// 更正の請求をする理由等
#define	DLG1JDT	41		// 更正決定通知書受理年月日

#define	DLG2VKS	22		// 正当とする額　最初
#define	DLG2SYZ	23		// 2  消費税額
#define	DLG2KJT	24		// 3  控除過大調整税額
#define	DLG2KJS	25		// 4  控除対象仕入税額
#define	DLG2HTZ	26		// 5  返還等対価に係る税額
#define	DLG2KSE	27		// 6  貸倒れに係る税額(更正の請求金額)
#define	DLG2MDE	31		// 10 中間納付税額(更正の請求金額)
#define DLG2KZ1	34		// 13 この請求前の既確定税額
#define DLG2KJK	35		// 14 控除不足還付税額
#define DLG2SHZ	36		// 15 差引税額
#define	DLG2LKE	37		// 16 地方消費税　譲渡割額　還付額(前)'14.07.22
#define	DLG2LNE	38		// 17 地方消費税　譲渡割額　納税額(前)'14.07.22
#define DLG2MDJ 39		// 18 中間納付譲渡割額
#define DLG2KZ2	42		// 21 この請求前の既確定譲渡割額
#define	DLG2VKE	42		// 正当とする額　最後
#define	DLG2LIN	21		// 金額　行数

#define	DLG3BSW	16		// 金融機関
#define	DLG3BNM	17		// 銀行等名称
#define	DLG3BKN 18		// 銀行等区分
#define	DLG3SNM 19		// 本支店名称
#define	DLG3SKN 20		// 本支店区分
#define	DLG3DNM 21		// 預金
#define	DLG3BAC 22		// 口座番号
#define	DLG3KAC 23		// 公金受取口座を利用する
#define	DLG3AC1 24		// 口座番号１
#define	DLG3AC2 25		// 口座番号２
#define	DLG3PNM 26		// 郵便局名称
#define	DLG3DOC	27		// 添付書類
#define	DLG3ZNM	28		// 税理士署名
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

#define	DLG1_KNO	52	// 個人番号

//リソース毎処理カット。現在は必要ない為
//#define	DLG0CMB	6		// 請求or申出


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
	{  DLG2VKS, 0, 1, 0			},
	{  DLG2SYZ, 0, 1, 0			},
	{  DLG2KJT, 0, 1, 0			},
	{  DLG2KJS, 0, 1, 0			},
	{  DLG2HTZ, 0, 1, 0			},
	{  DLG2KSE, 0, 1, 0			},
	{  DLG2MDE, 0, 1, 0			},
	{  DLG2KZ1, 1, 1, 0			},
	{  DLG2KJK, 0, 1, 0			},	// 31から実額項目へ
	{  DLG2SHZ, 0, 1, 0			},	// 31から実額項目へ
	{  DLG2LKE, 0, 1, 0			},
	{  DLG2LNE, 0, 1, 0			},
	{  DLG2MDJ, 0, 1, 0			},
	{  DLG2VKE, 1, 1, 0			},
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
	{  DLG3KAC, 1, 1, 0			},
	{  DLG3AC1, 1, 1, 0			},
	{  DLG3AC2, 1, 1, 0			},
	{  DLG3PNM, 1, 1, 0			},
	{  DLG3DOC, 1, 1, 0			},
	{	0, 0, 0, 0			}
};
static SH_ITEMINDEX	KjnKsk5[]	=	{
//	{   DLG0CMB, 1, 1, 0			},
	{	0, 0, 0, 0			}
};
