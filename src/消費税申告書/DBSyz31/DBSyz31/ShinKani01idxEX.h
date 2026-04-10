#pragma once

// 基本情報
#define ID2000	70	// 提出年
#define ID2001	71	// 提出月
#define ID2002	72	// 提出日
#define ID2010	73	// 税務署名
#define ID2011	74	// 税務署番号
#define ID2020	75	// 郵便番号
#define ID2030	76	// 納税地 上段
#define ID2031	77	// 納税地 下段
#define ID2040	78	// 電話番号 市外
#define ID2041	79	// 電話番号 市内
#define ID2042	80	// 電話番号 局番
#define ID2050	81	// 名称又は屋号 フリガナ
#define ID2051	82	// 名称又は屋号
#define ID2052	83	// 被合併名
#define ID2060	85	// 代表者又は氏名 フリガナ
#define ID2061	86	// 代表者又は氏名
#define ID2062	87	//　（個人の方）振替継続希望
#define ID2081	88	// 所管
#define ID2082	89	// 整理番号

#define ID2100	90	// 課税期間 自年
#define ID2101	91	// 課税期間 自月
#define ID2102	92	// 課税期間 自日
#define ID2110	93	// 課税期間 至年
#define ID2111	94	// 課税期間 至月
#define ID2112	95	// 課税期間 至日

#define ID2120	96	// 確定等

#define ID2130	97	// 中間期間 自年
#define ID2131	98	// 中間期間 自月
#define ID2132	99	// 中間期間 自日
#define ID2140	100	// 中間期間 至年
#define ID2141	101	// 中間期間 至月
#define ID2142	102	// 中間期間 至日

// この申告書による消費税の税額計算
#define ID3XM1	59	// 1.  課税標準額
#define ID3XM2	60	// 2.  消費税額
#define ID3XM3	61	// 3.  貸倒回収に係る消費税額
#define ID3XM4	62	// 4.  控除対象仕入税額
#define ID3XM5	63	// 5.  返還等対価に係る税額
#define ID3XM6	64	// 6.  貸倒れに係る税額
#define ID3XM7	65	// 7.  控除税額小計
#define ID3XM8	66	// 8.  控除不足還付税額
#define ID3XM9	67	// 9.  差引税額
#define ID3XM10	68	// 10. 中間納付税額
#define ID3XM11 69	// 11. 納付税額
#define ID3XM12	70	// 12. 中間納付還付税額
#define ID3XM13	71	// 13. 既確定税額
#define ID3XM14	72	// 14. 差引納付税額
#define ID3XM15	73	// 15. この課税期間の課税売上高
#define ID3XM16	74	// 16. 基準期間の課税売上高
#define ID3XM17	75	// 17. 控除不足還付税額
#define ID3XM18	76	// 18. 差引税額
#define ID3XM19	77	// 19. 還付額
#define ID3XM20	78	// 20. 納税額
#define ID3XM21	79	// 21. 中間納付譲渡割額
#define ID3XM22	80	// 22. 納付譲渡割額
#define ID3XM23	81	// 23. 中間納付還付譲渡割額
#define ID3XM24	82	// 24. 既確定譲渡割額
#define ID3XM25	83	// 25. 差引納付譲渡割額
#define ID3XM26	84	// 26. 消費税及び地方消費税の合計(納付又は還付)税額

// 付記・参考事項等
#define ID4000	47	// 割賦基準の適用 有
#define ID4001	48	// 割賦基準の適用 無
#define ID4010	49	// 延払基準の適用 有
#define ID4011	50	// 延払基準の適用 無
#define ID4020	51	// 工事進行基準の適用 有
#define ID4021	52	// 工事進行基準の適用 無
#define ID4030	53	// 現金主義会計の適用 有
#define ID4031	54	// 現金主義会計の適用 無
#define ID4040	55	// 課税標準額に対する消費税額の計算の特例適用 有
#define ID4041	56	// 課税標準額に対する消費税額の計算の特例適用 無

#define ID4100	57	// 第1種 課税売上高
#define ID4101	58	// 第1種 売上割合
#define ID4110	59	// 第2種 課税売上高
#define ID4111	60	// 第2種 売上割合
#define ID4120	61	// 第3種 課税売上高
#define ID4121	62	// 第3種 売上割合
#define ID4130	63	// 第4種 課税売上高
#define ID4131	64	// 第4種 売上割合
#define ID4140	65	// 第5種 課税売上高
#define ID4141	66	// 第5種 売上割合
#define ID4150	67	// 第6種 課税売上高
#define ID4151	68	// 第6種 売上割合
#define ID4160	69	// 特例計算適用 有
#define ID4161	70	// 特例計算適用 無

#define ID4210	71	// 金融機関 銀行名
#define ID4211	73	// 金融機関 支店名
#define ID4212	72	// 金融機関 銀行・金庫等
#define ID4213	74	// 金融機関 本社・支社等
#define ID4214	75	// 金融機関 預金種類
#define ID4215	76	// 金融機関 口座番号

#define ID4310	77	// 郵便局 貯金記号
#define ID4311	78	// 郵便局 貯金番号
#define ID4312	79	// 郵便局 郵便局名

#define ID4410	80	// 還付金融機関選択
#define ID4420	81	// （個人の方）公金受取口座の利用

#define ID4RNO	82	// 税務署整理欄

#define ID4ZI0	43	// 税理士署名押印 事務所名
#define ID4ZI1	44	// 税理士署名押印 氏名
#define ID4ZI2	46	// 税理士署名押印 電話番号
#define ID4ZI3	83	// 税理士法第30条
#define ID4ZI4	84	// 税理士法第33条の2

static SH_ITEMINDEX Kani1[] = {
	{ ID2000, 1, 1, 0 },
	{ ID2001, 1, 1, 0 },
	{ ID2002, 1, 1, 0 },
	{ ID2010, 1, 1, 0 },
	{ ID2020, 1, 1, 0 },
	{ ID2030, 1, 1, 0 },
	{ ID2031, 1, 1, 0 },
	{ ID2040, 1, 1, 0 },
	{ ID2041, 1, 1, 0 },
	{ ID2042, 1, 1, 0 },
	{ ID2050, 1, 1, 0 },
	{ ID2051, 1, 1, 0 },
	{ ID2052, 1, 1, 0 },
	{ ID2060, 1, 1, 0 },
	{ ID2061, 1, 1, 0 },
	{ ID2062, 1, 1, 0 },
	{ ID2081, 1, 1, 0 },
	{ ID2082, 1, 1, 0 },
	{ 0, 0, 0, 0 }
};

static SH_ITEMINDEX Kani2[] = {
	{ ID3XM1, 0, 0, 0 },
	{ ID3XM2, 0, 0, 0 },
	{ ID3XM3, 0, 0, 0 },
	{ ID3XM4, 0, 0, 0 },
	{ ID3XM5, 0, 0, 0 },
	{ ID3XM6, 0, 0, 0 },
	{ ID3XM7, 0, 0, 0 },
	{ ID3XM8, 0, 0, 0 },
	{ ID3XM9, 0, 0, 0 },
	{ ID3XM10, 1, 1, 0 },
	{ ID3XM11, 0, 0, 0 },
	{ ID3XM12, 0, 0, 0 },
	{ ID3XM13, 1, 1, 0 },
	{ ID3XM14, 0, 0, 0 },
	{ ID3XM15, 0, 1, 0 },
	{ ID3XM16, 1, 1, 0 },
	{ ID3XM17, 0, 0, 0 },
	{ ID3XM18, 0, 0, 0 },
	{ ID3XM19, 0, 0, 0 },
	{ ID3XM20, 0, 0, 0 },
	{ ID3XM21, 1, 1, 0 },
	{ ID3XM22, 0, 0, 0 },
	{ ID3XM23, 0, 0, 0 },
	{ ID3XM24, 1, 1, 0 },
	{ ID3XM25, 0, 0, 0 },
	{ ID3XM26, 0, 0, 0 },
	{ 0, 0, 0, 0 }
};

static SH_ITEMINDEX Kani3[] = {
	{ ID4000, 1, 1, 0 },
	{ ID4001, 1, 1, 0 },
	{ ID4010, 1, 1, 0 },
	{ ID4011, 1, 1, 0 },
	{ ID4020, 1, 1, 0 },
	{ ID4021, 1, 1, 0 },
	{ ID4030, 1, 1, 0 },
	{ ID4031, 1, 1, 0 },
	{ ID4040, 0, 0, 0 },
	{ ID4041, 0, 0, 0 },
	{ ID4160, 0, 0, 0 },
	{ ID4161, 0, 0, 0 },
	{ ID4210, 1, 1, 0 },
	{ ID4211, 1, 1, 0 },
	{ ID4212, 1, 1, 0 },
	{ ID4213, 1, 1, 0 },
	{ ID4214, 1, 1, 0 },
	{ ID4215, 1, 1, 0 },
	{ ID4310, 1, 1, 0 },
	{ ID4311, 1, 1, 0 },
	{ ID4312, 1, 1, 0 },
	{ ID4410, 1, 1, 0 },
	{ ID4420, 1, 1, 0 },
	{ ID4RNO, 1, 1, 0 },
	{ ID4ZI1, 0, 0, 0 },
	{ ID4ZI2, 0, 0, 0 },
	{ ID4ZI3, 1, 1, 0 },
	{ ID4ZI4, 1, 1, 0 },
	{ 0, 0, 0, 0 }
};