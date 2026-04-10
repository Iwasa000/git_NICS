
#pragma once


#define	CHUSEIRI		1		// 整理番号
#define	CHUZKJI			2		// 前課税期間　自
#define	CHUZKITA		3		// 前課税期間　至
#define	CHUSNMD			4		// 修正・更正・決定の年月日
#define	CHUZZEI			5		// 前課税期間の消費税額
#define	CHUTU1			6		// 月数換算　分子
#define	CHUTU2			7		// 月数換算　分母
#define	CHUNZEI			8		// 納付すべき消費税額
#define	CHUNTZEI		9		// 納付すべき地方消費税額
#define	CHUZEIGOU		10		// 消費税及び地方消費税の合計納付税額
#define	CHUNOU1			11		// 納税地　上段
#define	CHUNOU2			12		// 納税地　下段
#define	CHUTEL1			13		// 電話番号
#define	CHUTEL2			14		// 電話番号
#define	CHUTEL3			15		// 電話番号
#define	CHUMEIFRI		16		// フリガナ（名称部分）
#define	CHUMEI			17		// 名称又は屋号
#define	CHUKNO			18		// 個人番号
#define	CHUDAIFRI		19		// フリガナ（代表者氏名部分）
#define	CHUDAI			20		// 代表者氏名又は氏名
#define	CHUZEINEN		24		// 平成○○
#define	CHUZEISYO		25		// ○○税務署長殿
#define CHUZKJIGEN		78
#define CHUZKITAGEN		79


//cpp内で使用しているインデックスをdefine化。今後はインデックスが変わったらここを変えれば動く仕組みにしておく
#define	CHUZRJMEI		21		//税理士署名 (事務所名)
#define	CHUZRMEI		22		//税理士署名(税理士氏名)
#define	CHUZRTEL		23		//税理士署名(税理士 TEL)
#define	CHUSYONO		145		//税務署検索の下 (書番号)


#define	CHUSYUSZEI1		27		//修正申告　消費税　申告前の税額
#define	CHUSYUSZEI2		28		//修正申告　消費税　申告により増加する税額
#define	CHUSYUTZEI1		29		//修正申告　地方消費税　申告前の税額
#define	CHUSYUTZEI2		30		//修正申告　地方消費税　申告により増加する税額
#define	CHUSYUKEI		31		//修正申告　消費税及び地方消費税の合計納付税額



#define	CHUCHUKJI		32		// ○年○月○日　課税期間分の中間申告書（自）
#define	CHUCHUKITA		33		// ○年○月○日　課税期間分の中間申告書（至）
#define	CHUTAIJI		34		// 中間申告対象期間 自
#define	CHUTAIITA		35		// 中間申告対象期間 至
#define	CHUTAIJIGEN		73		// 中間申告対象期間 元号
#define	CHUTAIITAGEN	74		// 中間申告対象期間 元号





// インデックス管理
const	int	StartIndexEX	=	11;

// 消費税及び地方消費税の中間申告（第26号様式）
SH_ITEMINDEX	TyuukanEX[]	=	{
	{  CHUSEIRI, 1, 1, 0			},
	{  CHUZKJI, 1, 1, 0				},
	{  CHUZKITA, 1, 1, 0			},
	{  CHUSNMD, 1, 1, 0				},
	{  CHUZZEI, 0, 1, 0				},
	{  CHUTU1, 0, 1, 0				},
	{  CHUTU2, 0, 1, 0				},
	{  CHUNZEI, 0, 1, 0				},
	{  CHUNTZEI, 0, 1, 0			},
	{  CHUZEIGOU, 0, 1, 0			},
	{  CHUNOU1, 1, 1, 0				},
	{  CHUNOU2, 1, 1, 0				},
	{  CHUTEL1, 1, 1, 0				},
	{  CHUTEL2, 1, 1, 0				},
	{  CHUTEL3, 1, 1, 0				},
	{  CHUMEIFRI, 1, 1, 0			},
	{  CHUMEI, 1, 1, 0				},
	{  CHUDAIFRI, 1, 1, 0			},
	{  CHUDAI, 1, 1, 0				},
	{  CHUZEINEN, 1, 1, 0			},
	{  CHUZEISYO, 1, 1, 0			},
	{  -1, 0, 0, 0			}
};
