#pragma once

//課税取引個別指定　特定収入計算表個別指定　添付書面個別指定　のインデックス情報

#define	KAZ_MAXCOL     5
#define	TOK_MAXCOL    15
#define	ZEI_MAXCOL    11

// 24/05/17_個別タブから出力 cor -->
//#define	KAZ_MAXROW     2
//#define	TOK_MAXROW     4
//#define	ZEI_MAXROW     2
// ---------------------------------
#define	KAZ_MAXROW			3
#define	TOK_MAXROW			5
#define	ZEI_MAXROW			3
// 24/05/17_個別タブから出力 cor <--
//課税取引個別指定　個別指定　//////////////////////////
#define	TITLE_KA00			 0  // 『課税取引金額計算表』
#define	TITLE_KA01			 1	// 『印刷枚数』
#define	TITLE_KA02			12	// 『部数』			// 24/05/17_個別タブから出力 add
//チェックボックス
#define	CHEK_KA01			 2	// 課税取引金額計算表　事業所得用 
#define	CHEK_KA02			 4	// 課税取引金額計算表　農業所得用 
#define	CHEK_KA03			 6	// 課税取引金額計算表　不動産所得用 
#define	CHEK_KA04			 8	// 課税売上高計算表
#define	CHEK_KA05			10	// 課税仕入高計算表／事業別売上高
// 24/05/17_個別タブから出力 add -->
//部数
#define	CHEK_KA01_BUS		13	// 課税取引金額計算表　事業所得用 
#define	CHEK_KA02_BUS		14	// 課税取引金額計算表　農業所得用 
#define	CHEK_KA03_BUS		15	// 課税取引金額計算表　不動産所得用 
#define	CHEK_KA04_BUS		16	// 課税売上高計算表付表
#define	CHEK_KA05_BUS		17	// 課税仕入高計算表／事業別売上高
// 24/05/17_個別タブから出力 add <--
//枚数
#define	CHEK_KA01_NUM		 3	// 課税取引金額計算表　事業所得用 
#define	CHEK_KA02_NUM		 5	// 課税取引金額計算表　農業所得用 
#define	CHEK_KA03_NUM		 7	// 課税取引金額計算表　不動産所得用 
#define	CHEK_KA04_NUM		 9	// 課税売上高計算表付表
#define	CHEK_KA05_NUM		11	// 課税仕入高計算表／事業別売上高

//特定収入計算表個別指定　個別指定　//////////////////////////
#define	TITLE_TO00			 0  // 『課税取引金額計算表』
#define	TITLE_TO01			 1	// 『印刷枚数』
#define	TITLE_TO02			 2  // 『開始頁』
#define	TITLE_TO03			 3	// 『終了頁』
#define	TITLE_TO04			64	// 『部数』			// 24/05/17_個別タブから出力 add

//チェックボックス
#define	CHEK_TO01			 4	// 計算式１
#define	CHEK_TO02			 8 	// 計算式２（１）
#define	CHEK_TO03			12	// 計算式２（２）
#define	CHEK_TO04			16	// 計算式３，４
#define	CHEK_TO05			20	// 計算式５（１）
#define	CHEK_TO06			24	// 計算式５（２） 
#define	CHEK_TO07			28	// 計算式５（３）
#define	CHEK_TO08			32	// 計算式５－２（１） 
#define	CHEK_TO09			36	// 計算式５－２（２）
#define	CHEK_TO10			40	// 計算式５－２（３）－１
#define	CHEK_TO11			44	// 計算式５－２（３）－２ 
#define	CHEK_TO12			48	// 計算式５－２（３）－３ 
#define	CHEK_TO13			52	// 計算式５－２（４）－１ 
#define	CHEK_TO14			56	// 計算式５－２（４）－２ 
#define	CHEK_TO15			60	// 計算式５－２（４）－３ 

// 24/05/17_個別タブから出力 add -->
//部数
#define	CHEK_TO01_BUS		65	// 計算式１
#define	CHEK_TO02_BUS		66	// 計算式２（１）
#define	CHEK_TO03_BUS		67	// 計算式２（２）
#define	CHEK_TO04_BUS		68	// 計算式３，４
#define	CHEK_TO05_BUS		69	// 計算式５（１）
#define	CHEK_TO06_BUS		70	// 計算式５（２） 
#define	CHEK_TO07_BUS		71	// 計算式５（３）
#define	CHEK_TO08_BUS		72	// 計算式５－２（１） 
#define	CHEK_TO09_BUS		73	// 計算式５－２（２）
#define	CHEK_TO10_BUS		74	// 計算式５－２（３）－１
#define	CHEK_TO11_BUS		75	// 計算式５－２（３）－２ 
#define	CHEK_TO12_BUS		76	// 計算式５－２（３）－３ 
#define	CHEK_TO13_BUS		77	// 計算式５－２（４）－１ 
#define	CHEK_TO14_BUS		78	// 計算式５－２（４）－２ 
#define	CHEK_TO15_BUS		79	// 計算式５－２（４）－３ 
// 24/05/17_個別タブから出力 add <--

//印刷枚数
#define	CHEK_TO01_NUM		 5	//  計算式１
#define	CHEK_TO02_NUM		 9	//  計算式２（１）
#define	CHEK_TO03_NUM		13	//  計算式２（２）
#define	CHEK_TO04_NUM		17	//  計算式３，４
#define	CHEK_TO05_NUM		21	//  計算式５（１）
#define	CHEK_TO06_NUM		25	//  計算式５（２） 
#define	CHEK_TO07_NUM		29	//  計算式５（３）
#define	CHEK_TO08_NUM		33	//  計算式５－２（１） 
#define	CHEK_TO09_NUM		37	//  計算式５－２（２）
#define	CHEK_TO10_NUM		41	//  計算式５－２（３）－１
#define	CHEK_TO11_NUM		45	//  計算式５－２（３）－２ 
#define	CHEK_TO12_NUM		49	//  計算式５－２（３）－３ 
#define	CHEK_TO13_NUM		53	//  計算式５－２（４）－１ 
#define	CHEK_TO14_NUM		57	//  計算式５－２（４）－２ 
#define	CHEK_TO15_NUM		61	//  計算式５－２（４）－３ 

//開始頁
#define	CHEK_TO01_ST		 6	//  計算式１
#define	CHEK_TO02_ST		10	//  計算式２（１）
#define	CHEK_TO03_ST		14	//  計算式２（２）
#define	CHEK_TO04_ST		18	//  計算式３，４
#define	CHEK_TO05_ST		22	//  計算式５（１）
#define	CHEK_TO06_ST		26	//  計算式５（２） 
#define	CHEK_TO07_ST		30	//  計算式５（３）
#define	CHEK_TO08_ST		34	//  計算式５－２（１） 
#define	CHEK_TO09_ST		38	//  計算式５－２（２）
#define	CHEK_TO10_ST		42	//  計算式５－２（３）－１
#define	CHEK_TO11_ST		46	//  計算式５－２（３）－２ 
#define	CHEK_TO12_ST		50	//  計算式５－２（３）－３ 
#define	CHEK_TO13_ST		54	//  計算式５－２（４）－１ 
#define	CHEK_TO14_ST		58	//  計算式５－２（４）－２ 
#define	CHEK_TO15_ST		62	//  計算式５－２（４）－３ 

//終了頁
#define	CHEK_TO01_ED		 7	//  計算式１
#define	CHEK_TO02_ED		11	//  計算式２（１）
#define	CHEK_TO03_ED		15	//  計算式２（２）
#define	CHEK_TO04_ED		19	//  計算式３，４
#define	CHEK_TO05_ED		23	//  計算式５（１）
#define	CHEK_TO06_ED		27	//  計算式５（２） 
#define	CHEK_TO07_ED		31	//  計算式５（３）
#define	CHEK_TO08_ED		35	//  計算式５－２（１） 
#define	CHEK_TO09_ED		39	//  計算式５－２（２）
#define	CHEK_TO10_ED		43	//  計算式５－２（３）－１
#define	CHEK_TO11_ED		47	//  計算式５－２（３）－２ 
#define	CHEK_TO12_ED		51	//  計算式５－２（３）－３ 
#define	CHEK_TO13_ED		55	//  計算式５－２（４）－１ 
#define	CHEK_TO14_ED		59	//  計算式５－２（４）－２ 
#define	CHEK_TO15_ED		63	//  計算式５－２（４）－３ 


//税務代理書面　個別指定　//////////////////////////
#define	TITLE_ZE00			 0  // 『税務代理書面表』
#define	TITLE_ZE01			 1	// 『印刷枚数』
#define	TITLE_ZE02			24	// 『部数』			// 24/05/17_個別タブから出力 add
//チェックボックス
#define	CHEK_ZE01			 2	// 税務代理権限証書
#define	CHEK_ZE02			 4	// 税理士法第33条の2第1項に規定する添付書面（第1面）
#define	CHEK_ZE03			 6	// 税理士法第33条の2第1項に規定する添付書面（第2面）
#define	CHEK_ZE04			 8	// 税理士法第33条の2第1項に規定する添付書面（第3面）
#define	CHEK_ZE05			10	// 税理士法第33条の2第1項に規定する添付書面（第4面　1枚目）
#define	CHEK_ZE06			12	// 税理士法第33条の2第1項に規定する添付書面（第4面　2枚目）
#define	CHEK_ZE07			14	// 税理士法第33条の2第2項に規定する添付書面（第1面）
#define	CHEK_ZE08			16	// 税理士法第33条の2第2項に規定する添付書面（第2面）
#define	CHEK_ZE09			18	// 税理士法第33条の2第2項に規定する添付書面（第3面）
#define	CHEK_ZE10			20	// 税理士法第33条の2第2項に規定する添付書面（第4面　1枚目）
#define	CHEK_ZE11			22	// 税理士法第33条の2第2項に規定する添付書面（第4面　2枚目）

// 24/05/17_個別タブから出力 add -->
//部数
#define	CHEK_ZE01_BUS		25	// 税務代理権限証書
#define	CHEK_ZE02_BUS		26	// 税理士法第33条の2第1項に規定する添付書面（第1面）
#define	CHEK_ZE03_BUS		27	// 税理士法第33条の2第1項に規定する添付書面（第2面）
#define	CHEK_ZE04_BUS		28	// 税理士法第33条の2第1項に規定する添付書面（第3面）
#define	CHEK_ZE05_BUS		29	// 税理士法第33条の2第1項に規定する添付書面（第4面　1枚目）
#define	CHEK_ZE06_BUS		30	// 税理士法第33条の2第1項に規定する添付書面（第4面　2枚目）
#define	CHEK_ZE07_BUS		31	// 税理士法第33条の2第2項に規定する添付書面（第1面）
#define	CHEK_ZE08_BUS		32	// 税理士法第33条の2第2項に規定する添付書面（第2面）
#define	CHEK_ZE09_BUS		33	// 税理士法第33条の2第2項に規定する添付書面（第3面）
#define	CHEK_ZE10_BUS		34	// 税理士法第33条の2第2項に規定する添付書面（第4面　1枚目）
#define	CHEK_ZE11_BUS		35	// 税理士法第33条の2第2項に規定する添付書面（第4面　2枚目）
// 24/05/17_個別タブから出力 add <--

//印刷枚数
#define	CHEK_ZE01_NUM		 3	// 税務代理権限証書
#define	CHEK_ZE02_NUM		 5	// 税理士法第33条の2第1項に規定する添付書面（第1面）
#define	CHEK_ZE03_NUM		 7	// 税理士法第33条の2第1項に規定する添付書面（第2面）
#define	CHEK_ZE04_NUM		 9	// 税理士法第33条の2第1項に規定する添付書面（第3面）
#define	CHEK_ZE05_NUM		11	// 税理士法第33条の2第1項に規定する添付書面（第4面　1枚目）
#define	CHEK_ZE06_NUM		13	// 税理士法第33条の2第1項に規定する添付書面（第4面　2枚目）
#define	CHEK_ZE07_NUM		15	// 税理士法第33条の2第2項に規定する添付書面（第1面）
#define	CHEK_ZE08_NUM		17	// 税理士法第33条の2第2項に規定する添付書面（第2面）
#define	CHEK_ZE09_NUM		19	// 税理士法第33条の2第2項に規定する添付書面（第3面）
#define	CHEK_ZE10_NUM		21	// 税理士法第33条の2第2項に規定する添付書面（第4面　1枚目）
#define	CHEK_ZE11_NUM		23	// 税理士法第33条の2第2項に規定する添付書面（第4面　2枚目）



