#pragma once

//== 定数 ==
// タブ種別
typedef	enum {
	ID_ICSSH_FHYO1_01_TAB = 1,		// 付表１－１
	ID_ICSSH_FHYO1_02_TAB,			// 付表１－２
	ID_ICSSH_FHYO2_01_TAB,			// 付表２－１
	ID_ICSSH_FHYO2_02_TAB,			// 付表２－２
	ID_ICSSH_FHYO4_01_TAB,			// 付表４－１
	ID_ICSSH_FHYO4_02_TAB,			// 付表４－２
	ID_ICSSH_FHYO5_01_TAB,			// 付表５－１
	ID_ICSSH_FHYO5_012_TAB,			// 付表５－１(2)
	ID_ICSSH_FHYO5_02_TAB,			// 付表５－２
	ID_ICSSH_FHYO5_022_TAB,			// 付表５－２(2)
	ID_ICSSH_FHYO6_TAB,				// 付表６
	ID_ICSSH_IPPAN01_TAB,			// 一般本表１
	ID_ICSSH_IPPAN02_TAB,			// 課税標準額等の内訳書
	ID_ICSSH_KANI01_TAB				// 簡易本表
} EnumIdIcsShTabtype;

//== 構造体 ==
// タブ管理用
typedef struct	_tag_ICS_SH_TABMNG_ {
	EnumIdIcsShTabtype	tabtype;	// タブ種別
	ICSDialog			*pWnd;		// ウィンドウ
	char				dmy[8];
} SH_TABMNG;

// 更新時の過年度情報
typedef struct _tag_ICS_SH_PASTINFO_ {
	int		SKKBN;		// 申告区分
	int		MCOUNT;		// 中間申告時中間回数
	int		MINASHI;	// みなし期間？
	char	dumy[52];
} SH_PASTINFO;