#pragma once

//== 定数 ==
// タブ種別
typedef	enum {
	ID_ICSTAX_GENERAL_LIST_TAB = 1,			// 一般
	ID_ICSTAX_REALESTATE_LIST_TAB,			// 不動産
	ID_ICSTAX_AGRICULTUTRE_LIST_TAB,			// 農業
	ID_ICSTAX_SALES_LIST_TAB,				// 課税売上高
	ID_ICSTAX_STOCK_LIST_TAB,				// 課税仕入高
	ID_ICSTAX_BUSINESS_SALES_LIST_TAB,		// 事業別売上高
	ID_ICSTAX_KEI_GENERAL_LIST_TAB,			// 一般（経過措置用）
	ID_ICSTAX_KEI_REALESTATE_LIST_TAB,		// 不動産（経過措置用）
	ID_ICSTAX_KEI_AGRICULTUTRE_LIST_TAB,		// 農業（経過措置用）
	ID_ICSTAX_KEI_SALES_LIST_TAB,			// 課税売上高（経過措置用）
	ID_ICSTAX_KEI_STOCK_LIST_TAB,			// 課税仕入高（経過措置用）
	ID_ICSTAX_KEI_BUSINESS_SALES_LIST_TAB,	// 事業別売上高（経過措置用）
	ID_ICSTAX_6SYU_BUSINESS_SALES_LIST_TAB,		// 事業別売上高　６種
	ID_ICSTAX_KEI_6SYU_BUSINESS_SALES_LIST_TAB,	// 事業別売上高　６種（経過措置用）
	//31年度
	ID_ICSTAX_31_GENERAL_LIST_TAB,			// 一般
	ID_ICSTAX_31_REALESTATE_LIST_TAB,		// 不動産
	ID_ICSTAX_31_AGRICULTUTRE_LIST_TAB,		// 農業
	ID_ICSTAX_31_SALES_LIST_TAB,			// 課税売上高
	ID_ICSTAX_31_STOCK_LIST_TAB,			// 課税仕入高
	ID_ICSTAX_31_BUSINESS_SALES_LIST_TAB,	// 事業別売上高
} EnumIdIcsTaxTabtype;

//== 構造体 ==
// タブ管理用
typedef struct	_tag_ICS_TAX_TABMNG_ {
	EnumIdIcsTaxTabtype	tabtype;	// タブ種別
	ICSDialog			*pWnd;		// ウィンドウ
	char				dmy[8];
} TAX_TABMNG;

// 更新時の過年度情報
typedef struct _tag_ICS_TAX_PASTINFO_ {
	int		SKKBN;		// 申告区分
	int		MCOUNT;		// 中間申告時中間回数
	int		MINASHI;	// みなし期間？
	char	dumy[52];
} TAX_PASTINFO;

