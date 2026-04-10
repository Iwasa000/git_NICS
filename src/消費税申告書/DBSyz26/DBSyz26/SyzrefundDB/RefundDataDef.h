#pragma once

//== 定数 ==
enum {
	ID_ICSSH_REF_HOJIN	=	1,	// 法人
	ID_ICSSH_REF_KOJIN			// 個人
};

enum {
	ID_ICSSH_REF_FORTUNE_TYPE	=	1,		// 2-(1) 主な課税資産の譲渡等
	ID_ICSSH_REF_EXPORT_TYPE,				// 2-(2) 主な輸出取引等の明細
	ID_ICSSH_REF_INVENT_TYPE,				// 3-(2) 主な棚卸資産・原材料等の取得
	ID_ICSSH_REF_FIXED_TYPE,				// 3-(3) 主な固定資産等の取得
	ID_ICSSH_REF_STOCK_TYPE					// 3-(1) 仕入金額等の明細
};

//== 文字列 ==
static	char	*FMT_MS_UPDATE	=	_T("この処理を行うと経理方式を除く全てのデータがクリアされます。\n処理を続行してもよろしいですか？");
static	char	*FMT_MS_UPBANK	=	_T("輸出取引等に利用する主な金融機関に、本表の還付先金融機関等情報を連動しますか？");
