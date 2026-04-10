#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 消費税属性区分
typedef enum {
	ID_ICSSYZ_NO_KBN	=	0,		// 無し

	ID_ICSSYZ_SSN_SYUTOKU_KBN,		// 資産取得
	ID_ICSSYZ_SSN_JYOUTO_KBN,		// 資産譲渡
	ID_ICSSYZ_SSN_STK_JYT_KBN,		// 資産取得／譲渡
	ID_ICSSYZ_YUUKA_JYT_KBN,		// 有価証券譲渡
	ID_ICSSYZ_TOTI_JYT_KBN,			// 土地譲渡

	ID_ICSSYZ_URIAGE_KBN,			// 売上
	ID_ICSSYZ_URI_HENPIN_KBN,		// 売上返品
	ID_ICSSYZ_EIGYOGAI_EKI_KBN,		// 営業外収益
	ID_ICSSYZ_TOKUBETU_EKI_KBN,		// 特別利益
	ID_ICSSYZ_UKETORIRISOKU_KBN,	// 受取利息
	ID_ICSSYZ_KASIKAISYU_KBN,		// 貸倒回収
	ID_ICSSYZ_HANSYO_HIYOU_KBN,		// 販売奨励金費用
	ID_ICSSYZ_URIAGEWARIBIKI_KBN,	// 売上割引
	ID_ICSSYZ_HIKZ_YUSYUTUURI_KBN,	// 非課税輸出売上
	ID_ICSSYZ_YUSYUTU_MENZ_URI_KBN,	// 輸出免税売上
	ID_ICSSYZ_KARIUKE_SEISAN_KBN,	// 仮受消費税清算
	ID_ICSSYZ_SYK_YUUKA_JYT_KBN,	// 有価証券譲渡（社会福祉・宗教）
	ID_ICSSYZ_SYK_TOTI_JYT_KBN,		// 土地譲渡（社会福祉・宗教）

	ID_ICSSYZ_SIIRE_KBN,			// 仕入
	ID_ICSSYZ_SIR_HENPIN_KBN,		// 仕入返品
	ID_ICSSYZ_HANSYO_SYUNYU_KBN,	// 販売奨励金収入
	ID_ICSSYZ_SIIREWARIBIKI_KBN,	// 仕入割引
	ID_ICSSYZ_YUNYUSIIRE_KBN,		// 輸入仕入
	ID_ICSSYZ_TANAKASAN_KBN,		// 棚卸加算額
	ID_ICSSYZ_TANAGENSAN_KBN,		// 棚卸減算額
	ID_ICSSYZ_KARIBARAI_SEISAN_KBN,	// 仮払消費税清算
	ID_ICSSYZ_SYK_YUUKA_STK_KBN,	// 有価証券取得（社会福祉・宗教）
	ID_ICSSYZ_SYK_TOTI_STK_KBN,		// 土地取得（社会福祉・宗教）

	ID_ICSSYZ_KEIHI_KBN,			// 経費
	ID_ICSSYZ_EIGYOGAI_HIYOU_KBN,	// 営業外費用
	ID_ICSSYZ_TOKUBETU_SON_KBN,		// 特別損失
	ID_ICSSYZ_KASISON_KBN			// 貸倒損失

} EnumIdIcsSyzAtrKbn;

// 消費税率
typedef enum {
	ID_ICSSYZ_NO_ZRITU		=	-1,	// 不定
	ID_ICSSYZ_0_PER_ZRITU	=	0,	// 0 パーセント
	ID_ICSSYZ_3_PER_ZRITU,			// 3 パーセント
	ID_ICSSYZ_5_PER_ZRITU,			// 5 パーセント
	ID_ICSSYZ_8_PER_ZRITU			// 8 パーセント
} EnumIdIcsSyzZritu;


// 消費税属性（処理グループ区分）
const unsigned char	ICSSYZ_SISAN_GRKBN	=	0x01;	// 資産
const unsigned char	ICSSYZ_URIAGE_GRKBN	=	0x02;	// 売上
const unsigned char	ICSSYZ_SIIRE_GRKBN	=	0x04;	// 仕入
const unsigned char	ICSSYZ_KEIHI_GRKBN	=	0x08;	// 経費
const unsigned char	ICSSYZ_TAIKA_GRKBN	=	0x10;	// 対価


//-----------------------------------------------------------------------------
// 消費税属性区分の取得
//-----------------------------------------------------------------------------
// 返送値	：	消費税属性区分
//-----------------------------------------------------------------------------
int CSyzMakeSubW::GetSyzAtrKbn()
{
	EnumIdIcsSyzAtrKbn	syzkbn = ID_ICSSYZ_NO_KBN;

	unsigned char	grkbn=0, mskbn=0;

	//== 資産 ==
	if( (grkbn==ICSSYZ_SISAN_GRKBN) || (grkbn==(ICSSYZ_SISAN_GRKBN|ICSSYZ_TAIKA_GRKBN)) ){
		if( (grkbn==ICSSYZ_SISAN_GRKBN) && (mskbn==0x00) ){
			syzkbn = ID_ICSSYZ_SSN_SYUTOKU_KBN;
		}
		else if( (grkbn==ICSSYZ_SISAN_GRKBN) && (mskbn==0x01) ){
			syzkbn = ID_ICSSYZ_SSN_JYOUTO_KBN;
		}
		else if( (grkbn==(ICSSYZ_SISAN_GRKBN|ICSSYZ_TAIKA_GRKBN)) && (mskbn==0x02) ){
			syzkbn = ID_ICSSYZ_SSN_STK_JYT_KBN;
		}
		else if( (grkbn==(ICSSYZ_SISAN_GRKBN|ICSSYZ_TAIKA_GRKBN)) && (mskbn==0x03) ){
			syzkbn = ID_ICSSYZ_YUUKA_JYT_KBN;
		}
		else if( (grkbn==(ICSSYZ_SISAN_GRKBN|ICSSYZ_TAIKA_GRKBN)) && (mskbn==0x04) ){
			syzkbn = ID_ICSSYZ_TOTI_JYT_KBN;
		}
	}
	//== 売上 ==
	else if( grkbn == ICSSYZ_URIAGE_GRKBN ){
		if( mskbn == 0x00 ){
			syzkbn = ID_ICSSYZ_URIAGE_KBN;
		}
		else if( mskbn == 0x01 ){
			syzkbn = ID_ICSSYZ_URI_HENPIN_KBN;
		}
		else if( mskbn == 0x02 ){
			syzkbn = ID_ICSSYZ_EIGYOGAI_EKI_KBN;
		}
		else if( mskbn == 0x03 ){
			syzkbn = ID_ICSSYZ_TOKUBETU_EKI_KBN;
		}
		else if( mskbn == 0x04 ){
			syzkbn = ID_ICSSYZ_UKETORIRISOKU_KBN;
		}
		else if( mskbn == 0x05 ){
			syzkbn = ID_ICSSYZ_KASIKAISYU_KBN;
		}
		else if( mskbn == 0x06 ){
			syzkbn = ID_ICSSYZ_HANSYO_HIYOU_KBN;
		}
		else if( mskbn == 0x07 ){
			syzkbn = ID_ICSSYZ_URIAGEWARIBIKI_KBN;
		}
		else if( mskbn == 0x08 ){
			syzkbn = ID_ICSSYZ_HIKZ_YUSYUTUURI_KBN;
		}
		else if( mskbn == 0x09 ){
			syzkbn = ID_ICSSYZ_YUSYUTU_MENZ_URI_KBN;
		}
		else if( mskbn == 0x0A ){
			syzkbn = ID_ICSSYZ_KARIUKE_SEISAN_KBN;
		}
		else if( mskbn == 0x0B ){
			syzkbn = ID_ICSSYZ_SYK_YUUKA_JYT_KBN;
		}
		else if( mskbn == 0x0C ){
			syzkbn = ID_ICSSYZ_SYK_TOTI_JYT_KBN;
		}
	}
	//== 仕入 ==
	else if( grkbn == ICSSYZ_SIIRE_GRKBN ){
		if( mskbn == 0x00 ){
			syzkbn = ID_ICSSYZ_SIIRE_KBN;
		}
		else if( mskbn == 0x01 ){
			syzkbn = ID_ICSSYZ_SIR_HENPIN_KBN;
		}
		else if( mskbn == 0x02 ){
			syzkbn = ID_ICSSYZ_HANSYO_SYUNYU_KBN;
		}
		else if( mskbn == 0x03 ){
			syzkbn = ID_ICSSYZ_SIIREWARIBIKI_KBN;
		}
		else if( mskbn == 0x04 ){
			syzkbn = ID_ICSSYZ_YUNYUSIIRE_KBN;
		}
		else if( mskbn == 0x05 ){
			syzkbn = ID_ICSSYZ_TANAKASAN_KBN;
		}
		else if( mskbn == 0x06 ){
			syzkbn = ID_ICSSYZ_TANAGENSAN_KBN;
		}
		else if( mskbn == 0x07 ){
			syzkbn = ID_ICSSYZ_KARIBARAI_SEISAN_KBN;
		}
		else if( mskbn == 0x08 ){
			syzkbn = ID_ICSSYZ_SYK_YUUKA_STK_KBN;
		}
		else if( mskbn == 0x09 ){
			syzkbn = ID_ICSSYZ_SYK_TOTI_STK_KBN;
		}
	}
	//== 経費 ==
	else if( grkbn == ICSSYZ_KEIHI_GRKBN ){
		if( mskbn == 0x00 ){
			syzkbn = ID_ICSSYZ_KEIHI_KBN;
		}
		else if( mskbn == 0x01 ){
			syzkbn = ID_ICSSYZ_EIGYOGAI_HIYOU_KBN;
		}
		else if( mskbn == 0x02 ){
			syzkbn = ID_ICSSYZ_TOKUBETU_SON_KBN;
		}
		else if( mskbn == 0x03 ){
			syzkbn = ID_ICSSYZ_KASISON_KBN;
		}
	}

	return syzkbn;
}

//-----------------------------------------------------------------------------
// 消費税率の取得
//-----------------------------------------------------------------------------
// 返送値	：	消費税率
//-----------------------------------------------------------------------------
int CSyzMakeSubW::GetSyzZritu()
{
	int	zritu = ID_ICSSYZ_NO_ZRITU;

	unsigned char	zrsgn = 0;

	if( zrsgn == 0x00 ){
		zritu = ID_ICSSYZ_5_PER_ZRITU;
	}
	else if( zrsgn == 0x01 ){
		zritu = ID_ICSSYZ_3_PER_ZRITU;
	}
	else if( zrsgn == 0x02 ){
		zritu = ID_ICSSYZ_0_PER_ZRITU;
	}
	else if( zrsgn == 0x03 ){
		zritu = ID_ICSSYZ_8_PER_ZRITU;
	}

	return zritu;
}
