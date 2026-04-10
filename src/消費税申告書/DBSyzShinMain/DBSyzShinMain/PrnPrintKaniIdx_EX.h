#pragma once

// 入力
#define	TITLE_01	0			// 『申告書・課税標準額等の内訳書』
#define	TITLE_02	1			// 『様式選択・個別部数指定』
#define	COMBO_TEI	2			// 『提出用カラー/モノクロ』コンボ
#define	COMBO_HIK	3			// 『控　用カラー/モノクロ』コンボ
#define	CHEK01_1	4			// 申告書本表
#define	CHEK01_2	5			// 課税標準額等の内訳書

// 様式選択・個別部数指定
#define	HP_TEI_CHK	6	// 申告書本表			提出用	チェック
#define	HP_TEI_BSU	7	// 申告書本表			提出用	部数
#define	HP_HIK_CHK	8	// 申告書本表			控　用	チェック
#define	HP_HIK_BSU	9	// 申告書本表			控　用	部数
#define	KZ_TEI_CHK	10	// 課税標準額等の内訳書	提出用	チェック
#define	KZ_TEI_BSU	11	// 課税標準額等の内訳書	提出用	部数
#define	KZ_HIK_CHK	12	// 課税標準額等の内訳書	控　用	チェック
#define	KZ_HIK_BSU	13	// 課税標準額等の内訳書	控　用	部数

#define	TITLE_07			14	// 『添付書類』
#define	CHEK04_1			15	// 付表４－１
#define	CHEK04_2			16	// 付表４－２
#define	CHEK04_3			17	// 付表４－３
#define	CHEK03_1			18	// 付表５－１
#define	CHEK03_2			19	// 付表５－１(２)
#define	CHEK05_1			20	// 付表５－２
#define	CHEK05_2			21	// 付表５－２(２)
#define	CHEK06_1			22	// 付表５－３
#define	CHEK06_2			23	// 付表５－３(２)
#define	CHEK21				24	// 付表６
#define	CHEK06_K			25	// 第２６号様式
#define	CHEK18_K  			26	// 第２６号様式Ａ４ヨコ控用
#define	CHEK09_K			27	// 消費税の還付申告に関する明細書（表面）
#define	CHEK10_K  			28	// 消費税の還付申告に関する明細書（裏面）
#define	CHEK11_K			29	// 更正の請求書
#define	CHEK22_K			33	// 特例１35-33
#define	CHEK12_K			30	// 課税取引金額計算表　個別指定用 
#define	CHEK13_K			31	// 特定収入計算表　個別指定用  
#define	CHEK14_K			32	// 税務代理書面　個別指定用  
//#define	CHEK15_K			33	// 課税売上高計算表
//#define	CHEK19				34	// 事業別売上高[13'09.19]

// 部数
#define	TITLE_08			34	// 『部数』
#define	CHEK04_1_BUSU		35	// 付表４－１
#define	CHEK04_2_BUSU		36	// 付表４－２
#define	CHEK04_3_BUSU		37	// 付表４－３
#define	CHEK03_1_BUSU		38	// 付表５－１
#define	CHEK03_2_BUSU		39	// 付表５－１(２)
#define	CHEK05_1_BUSU		40	// 付表５－２
#define	CHEK05_2_BUSU		41	// 付表５－２(２)
#define	CHEK06_1_BUSU		42	// 付表５－３
#define	CHEK06_2_BUSU		43	// 付表５－３(２)
#define	CHEK21_BUSU			44	// 付表６
#define	CHEK06_BUSU_K		45	// 第２６号様式
#define	CHEK18_BUSU_K		46	// 第２６号様式Ａ４ヨコ控用
#define	CHEK09_BUSU_K		47	// 消費税の還付申告に関する明細書（表面）
#define	CHEK10_BUSU_K		48	// 消費税の還付申告に関する明細書（裏面）
#define	CHEK11_BUSU_K		49	// 更正の請求書
#define	CHEK22_BUSU_K		50	// 特例１
#define	CHEK12_BUSU_K		51	// 課税取引金額計算表　個別指定用
#define	CHEK13_BUSU_K		52	// 特定収入　個別指定用 
#define	CHEK14_BUSU_K		53	// 税務代理書面　個別指定用 
//#define	CHEK15_BUSU_K		56	// 課税売上高計算表
//#define	CHEK19_BUSU			57	// 事業別売上高[13'09.19]

// 第２６号様式
static SH_ITEMINDEX	PrnTbl1_K[]	=	{
	{	COMBO_TEI,	0, 0, 0	},	// 提出用カラー/モノクロコンボ
	{	COMBO_HIK,	0, 0, 0	},	// 控　用カラー/モノクロコンボ
	{	CHEK01_1,	0, 0, 0	},	// 申告書本表
	{	CHEK01_2,	0, 0, 0	},	// 課税標準額等の内訳書	
	{	CHEK04_1,	0, 0, 0	},	// 付表４－１
	{	CHEK04_2,	0, 0, 0	},	// 付表４－２
	{	CHEK04_3,	0, 0, 0	},	// 付表４－３
	{	CHEK03_1,	0, 0, 0	},	// 付表５－１
	{	CHEK03_2,	0, 0, 0	},	// 付表５－１(２)
	{	CHEK06_1,	0, 0, 0	},	// 付表５－３
	{	CHEK06_2,	0, 0, 0	},	// 付表５－３(２)
	{	CHEK05_1,	0, 0, 0	},	// 付表５－２
	{	CHEK05_2,	0, 0, 0	},	// 付表５－２(２)
	{	CHEK21,	0, 0, 0		},	// 付表６
	{	CHEK06_K,	1, 1, 0	},	// 第２６号様式
	{	CHEK18_K,	1, 1, 0	},	// 第２６号様式Ａ４ヨコ控用
	{	CHEK09_K,	0, 0, 0	},	// 消費税の還付申告に関する明細書（表面）
	{	CHEK10_K,	0, 0, 0	},	// 消費税の還付申告に関する明細書（裏面）
	{	CHEK11_K,	0, 0, 0	},	// 更正の請求書
	{	CHEK22_K,	0, 0, 0	},	// 特例１
	{	CHEK12_K,	0, 0, 0	},	// 課税取引金額計算表　個別指定用 
	{	CHEK13_K,	0, 0, 0	},	// 特定収入計算表　個別指定用  
// 24/04/04_341 cor -->
	//{	CHEK14_K,	0, 0, 0	},	// 税務代理書面　個別指定用    
// --------------------
	{	CHEK14_K,	8, 8, 0	},	// 税務代理書面　個別指定用    
// 24/04/04_341 cor <--
//	{	CHEK15_K,	0, 0, 0	},	// 課税売上高計算表 
//	{	CHEK19,	0, 0, 0		},	// 事業別売上高[13'09.19]
	{	0,0,0,0				}
};

// ＯＣＲ印刷
static SH_ITEMINDEX	PrnTbl2_K[]	=	{
	{	COMBO_TEI,	0, 0, 0	},	// 提出用カラー/モノクロコンボ
	{	COMBO_HIK,	0, 0, 0	},	// 控　用カラー/モノクロコンボ
	{	CHEK01_1,	1, 1, 0	},	// 申告書本表
	{	CHEK01_2,	1, 1, 0	},	// 課税標準額等の内訳書	
	{	CHEK04_1,	0, 0, 0	},	// 付表４－１
	{	CHEK04_2,	0, 0, 0	},	// 付表４－２
	{	CHEK04_3,	0, 0, 0	},	// 付表４－３
	{	CHEK03_1,	0, 0, 0	},	// 付表５－１
	{	CHEK03_2,	0, 0, 0	},	// 付表５－１(２)
	{	CHEK06_1,	0, 0, 0	},	// 付表５－３
	{	CHEK06_2,	0, 0, 0	},	// 付表５－３(２)
	{	CHEK05_1,	0, 0, 0	},	// 付表５－２
	{	CHEK05_2,	0, 0, 0	},	// 付表５－２(２)
	{	CHEK21,	0, 0, 0		},	// 付表６
	{	CHEK06_K,	0, 0, 0	},	// 第２６号様式
	{	CHEK18_K,	0, 0, 0	},	// 第２６号様式Ａ４ヨコ控用
	{	CHEK09_K,	0, 0, 0	},	// 消費税の還付申告に関する明細書（表面）
	{	CHEK10_K,	0, 0, 0	},	// 消費税の還付申告に関する明細書（裏面）
	{	CHEK11_K,	0, 0, 0	},	// 更正の請求書
	{	CHEK22_K,	0, 0, 0	},	// 特例１
	{	CHEK12_K,	0, 0, 0	},	// 課税取引金額計算表　個別指定用 
	{	CHEK13_K,	0, 0, 0	},	// 特定収入計算表　個別指定用  
	{	CHEK14_K,	0, 0, 0	},	// 税務代理書面　個別指定用  
//	{	CHEK14_K,	0, 0, 0	},	// 税務代理書面　個別指定用  
//	{	CHEK15_K,	0, 0, 0	},	// 課税売上高計算表 
//	{	CHEK19,	0, 0, 0		},	// 事業別売上高[13'09.19]
	{	0,0,0,0				}
};

// イメージ印刷
static SH_ITEMINDEX	PrnTbl3_K[]	=	{
	{	COMBO_TEI,	1, 1, 0	},	// 提出用カラー/モノクロコンボ
	{	COMBO_HIK,	1, 1, 0	},	// 控　用カラー/モノクロコンボ
	{	CHEK01_1,	1, 1, 0	},	// 申告書本表
	{	CHEK01_2,	1, 1, 0	},	// 課税標準額等の内訳書	
	{	CHEK04_1,	1, 1, 0	},	// 付表４－１
	{	CHEK04_2,	0, 1, 0	},	// 付表４－２
	{	CHEK04_3,	1, 1, 0	},	// 付表４－３
	{	CHEK03_1,	1, 1, 0	},	// 付表５－１
	{	CHEK03_2,	1, 1, 0	},	// 付表５－１(２)
	{	CHEK06_1,	1, 1, 0	},	// 付表５－３
	{	CHEK06_2,	1, 1, 0	},	// 付表５－３(２)
	{	CHEK05_1,	0, 1, 0	},	// 付表５－２
	{	CHEK05_2,	0, 1, 0	},	// 付表５－２(２)
	{	CHEK21,13,13, 0		},	// 付表６
	{	CHEK06_K, 0, 0, 0	},	// 第２６号様式
	{	CHEK18_K, 0, 0, 0	},	// 第２６号様式Ａ４ヨコ控用
	{	CHEK09_K, 4, 4, 0	},	// 消費税の還付申告に関する明細書（表面）
	{	CHEK10_K, 4, 4, 0	},	// 消費税の還付申告に関する明細書（裏面）
	{	CHEK11_K, 5, 5, 0	},	// 更正の請求書
	{	CHEK22_K, 14, 14, 0	},	// 特例１
	{	CHEK12_K, 6, 6, 0	},	// 課税取引金額計算表　個別指定用 
	{	CHEK13_K, 7, 7, 0	},	// 特定収入計算表　個別指定用  
	{	CHEK14_K, 8, 8, 0	},	// 税務代理書面　個別指定用   
//	{	CHEK15_K, 9, 9, 0	},	// 課税売上高計算表  
//	{	CHEK19,12,12, 0		},	// 事業別売上高[13'09.19]
	{	0,0,0,0				}
};

// 部数関連
LST_BUSU	LstBusuTblK[] = {
	{CHEK01_1,HP_TEI_CHK,HP_TEI_BSU,HP_HIK_CHK,HP_HIK_BSU,-1},
	{CHEK01_2,KZ_TEI_CHK,KZ_TEI_BSU,KZ_HIK_CHK,KZ_HIK_BSU,-1},
	{CHEK04_1,CHEK04_1_BUSU,-1},
	{CHEK04_2,CHEK04_2_BUSU,-1},
	{CHEK04_3,CHEK04_3_BUSU,-1},
	{CHEK03_1,CHEK03_1_BUSU,-1},
	{CHEK03_2,CHEK03_2_BUSU,-1},
	{CHEK05_1,CHEK05_1_BUSU,-1},
	{CHEK05_2,CHEK05_2_BUSU,-1},
	{CHEK06_1,CHEK06_1_BUSU,-1},
	{CHEK06_2,CHEK06_2_BUSU,-1},

	{CHEK21,CHEK21_BUSU,-1},
	{CHEK06_K,CHEK06_BUSU_K,-1},
	{CHEK18_K,CHEK18_BUSU_K,-1},
	{CHEK09_K,CHEK09_BUSU_K,-1},
	{CHEK10_K,CHEK10_BUSU_K,-1},
	{CHEK11_K,CHEK11_BUSU_K,-1},
	{CHEK22_K,CHEK22_BUSU_K,-1},
	{CHEK12_K,CHEK12_BUSU_K,-1},
	{CHEK13_K,CHEK13_BUSU_K,-1},
	{CHEK14_K,CHEK14_BUSU_K,-1},
//	{CHEK15_K,CHEK15_BUSU_K,-1},
//	{CHEK19,CHEK19_BUSU,-1},
	{-1},
};

// 画面の番号、印刷の番号から部数が SH_PRINT_CMINFO::busu[50]
// の何番目に格納されているかを調べるためのテーブル
LST_BUSU_IDX	LstBusuIdxKan[] = {
{HP_TEI_BSU,0x01,-1},								// 申告書本表			提出用カラー
{HP_TEI_BSU,0x03,-1},								// 申告書本表			提出用モノクロ
{HP_HIK_BSU,0x02,-1},								// 申告書本表			控用カラー
{HP_HIK_BSU,0x04,-1},								// 申告書本表			控用モノクロ
{KZ_TEI_BSU,0x57,-1},								// 課税標準額等の内訳書	提出用カラー
{KZ_TEI_BSU,0x05,-1},								// 課税標準額等の内訳書	提出用モノクロ
{KZ_HIK_BSU,0x58,-1},								// 課税標準額等の内訳書	控用カラー
{KZ_HIK_BSU,0x06,-1},								// 課税標準額等の内訳書	控用モノクロ
{CHEK04_1_BUSU,0x70,-1},							// 付表４－１
{CHEK04_2_BUSU,0x71,-1},							// 付表４－２
{CHEK04_3_BUSU,0x17,-1},							// 付表４－３
{CHEK03_1_BUSU,0x72,-1},							// 付表５－１
{CHEK03_2_BUSU,0x73,-1},							// 付表５－１(２)
{CHEK06_1_BUSU,0x18,-1},							// 付表５－３
{CHEK06_2_BUSU,0x19,-1},							// 付表５－３(２)
{CHEK05_1_BUSU,0x74,-1},							// 付表５－２
{CHEK05_2_BUSU,0x75,-1},							// 付表５－２(２)
{CHEK21_BUSU,0x16,-1},								// 付表６
{CHEK06_BUSU_K,0x50,-1},							// 第２６号様式
{CHEK18_BUSU_K,0x51,-1},							// 第２６号様式Ａ４ヨコ控用
{CHEK09_BUSU_K,0x07,-1},							// 消費税の還付申告に関する明細書（表面）
{CHEK10_BUSU_K,0x08,-1},							// 消費税の還付申告に関する明細書（裏面）
{CHEK11_BUSU_K,0x09,-1},							// 更正の請求書
{CHEK22_BUSU_K,0x76,-1},							// 特例１
// 24/03/22_411 cor -->
//{CHEK12_BUSU_K,0x12,-1},							// 課税取引金額計算表　個別指定用 
//{CHEK13_BUSU_K,0x14,-1},							// 税務代理書面　個別指定用  
//{CHEK14_BUSU_K,0x13,-1},							// 特定収入　個別指定用  
// --------------------
// 課税取引金額計算表　個別指定用 
{CHEK12_BUSU_K,0x10,0x11,0x12,0x13,0x14,0x52,-1},
// 特定収入計算表　個別指定用  
{CHEK13_BUSU_K,0x40,0x41,0x42,0x56,0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,-1},
// 税務代理書面　個別指定用   
{CHEK14_BUSU_K,0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x90,-1},
// 24/03/22_411 cor <--
//{CHEK15_BUSU_K,0x10,-1},							// 課税売上高計算表  
//{CHEK19_BUSU,0x52,-1},							// 事業別売上高
{-1,-1},
};

short	CursorPosKan[30][10]=
{
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{CHEK01_1,HP_TEI_CHK,HP_TEI_BSU,HP_HIK_CHK,HP_HIK_BSU,-1},
	{CHEK01_2,KZ_TEI_CHK,KZ_TEI_BSU,KZ_HIK_CHK,KZ_HIK_BSU,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{CHEK04_1,CHEK04_1_BUSU,-1,-1,-1,-1,-1,-1,-1,-1},
	{CHEK04_2,CHEK04_2_BUSU,-1,-1,-1,-1,-1,-1,-1,-1},
	{CHEK04_3,CHEK04_3_BUSU,-1,-1,-1,-1,-1,-1,-1,-1},
	{CHEK03_1,CHEK03_1_BUSU,-1,-1,-1,-1,-1,-1,-1,-1},
	{CHEK03_2,CHEK03_2_BUSU,-1,-1,-1,-1,-1,-1,-1,-1},

	{CHEK05_1,CHEK05_1_BUSU,-1,-1,-1,-1,-1,-1,-1,-1},
	{CHEK05_2,CHEK05_2_BUSU,-1,-1,-1,-1,-1,-1,-1,-1},
	{CHEK06_1,CHEK06_1_BUSU,-1,-1,-1,-1,-1,-1,-1,-1},
	{CHEK06_2,CHEK06_2_BUSU,-1,-1,-1,-1,-1,-1,-1,-1},
	{CHEK21,CHEK21_BUSU,-1,-1,-1,-1,-1,-1,-1,-1},
	{CHEK06_K,CHEK06_BUSU_K,-1,-1,-1,-1,-1,-1,-1,-1},
	{CHEK18_K,CHEK18_BUSU_K,-1,-1,-1,-1,-1,-1,-1,-1},
	{CHEK09_K,CHEK09_BUSU_K,-1,-1,-1,-1,-1,-1,-1,-1},
	{CHEK10_K,CHEK10_BUSU_K,-1,-1,-1,-1,-1,-1,-1,-1},
	{CHEK11_K,CHEK11_BUSU_K,-1,-1,-1,-1,-1,-1,-1,-1},
	{CHEK22_K,CHEK22_BUSU_K,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{CHEK12_K,CHEK12_BUSU_K,-1,-1,-1,-1,-1,-1,-1,-1},
	{CHEK13_K,CHEK13_BUSU_K,-1,-1,-1,-1,-1,-1,-1,-1},
	{CHEK14_K,CHEK14_BUSU_K,-1,-1,-1,-1,-1,-1,-1,-1},
//	{CHEK15_K,CHEK15_BUSU_K,-1,-1,-1,-1,-1,-1,-1,-1},
//	{CHEK19,CHEK19_BUSU,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}
};



//前回保存


// 入力
#define	TITLE_01_OLD	0			// 『申告書・課税標準額等の内訳書』
#define	TITLE_02_OLD	1			// 『様式選択・個別部数指定』
#define	COMBO_TEI_OLD	2			// 『提出用カラー/モノクロ』コンボ
#define	COMBO_HIK_OLD	3			// 『控　用カラー/モノクロ』コンボ
#define	CHEK01_1_OLD	4			// 申告書本表
#define	CHEK01_2_OLD	5			// 課税標準額等の内訳書

// 様式選択・個別部数指定
#define	HP_TEI_CHK_OLD	6	// 申告書本表			提出用	チェック
#define	HP_TEI_BSU_OLD	7	// 申告書本表			提出用	部数
#define	HP_HIK_CHK_OLD	8	// 申告書本表			控　用	チェック
#define	HP_HIK_BSU_OLD	9	// 申告書本表			控　用	部数
#define	KZ_TEI_CHK_OLD	10	// 課税標準額等の内訳書	提出用	チェック
#define	KZ_TEI_BSU_OLD	11	// 課税標準額等の内訳書	提出用	部数
#define	KZ_HIK_CHK_OLD	12	// 課税標準額等の内訳書	控　用	チェック
#define	KZ_HIK_BSU_OLD	13	// 課税標準額等の内訳書	控　用	部数

#define	TITLE_07_OLD			14	// 『添付書類』
#define	CHEK04_1_OLD			15	// 付表４－１
#define	CHEK04_2_OLD			16	// 付表４－２
#define	CHEK04_3_OLD			17	// 付表４－３
#define	CHEK03_1_OLD			18	// 付表５－１
#define	CHEK03_2_OLD			19	// 付表５－１(２)
#define	CHEK05_1_OLD			20	// 付表５－２
#define	CHEK05_2_OLD			21	// 付表５－２(２)
#define	CHEK06_1_OLD			22	// 付表５－３
#define	CHEK06_2_OLD			23	// 付表５－３(２)
#define	CHEK21_OLD				24	// 付表６
#define	CHEK06_K_OLD			25	// 第２６号様式
#define	CHEK18_K_OLD  			26	// 第２６号様式Ａ４ヨコ控用
#define	CHEK09_K_OLD			27	// 消費税の還付申告に関する明細書（表面）
#define	CHEK10_K_OLD  			28	// 消費税の還付申告に関する明細書（裏面）
#define	CHEK11_K_OLD			29	// 更正の請求書
#define	CHEK22_K_OLD			35	// 特例１
#define	CHEK12_K_OLD			30	// 課税取引金額計算表　事業所得用 
#define	CHEK13_K_OLD			31	// 課税取引金額計算表　農業所得用 
#define	CHEK14_K_OLD			32	// 課税取引金額計算表　不動産所得用 
#define	CHEK15_K_OLD			33	// 課税売上高計算表
#define	CHEK19				34	// 事業別売上高[13'09.19]

// 部数
#define	TITLE_08_OLD			36	// 『部数』
#define	CHEK04_1_BUSU_OLD		37	// 付表４－１
#define	CHEK04_2_BUSU_OLD		38	// 付表４－２
#define	CHEK04_3_BUSU_OLD		39	// 付表４－３
#define	CHEK03_1_BUSU_OLD		40	// 付表５－１
#define	CHEK03_2_BUSU_OLD		41	// 付表５－１(２)
#define	CHEK05_1_BUSU_OLD		42	// 付表５－２
#define	CHEK05_2_BUSU_OLD		43	// 付表５－２(２)
#define	CHEK06_1_BUSU_OLD		44	// 付表５－３
#define	CHEK06_2_BUSU_OLD		45	// 付表５－３(２)
#define	CHEK21_BUSU_OLD			46	// 付表６
#define	CHEK06_BUSU_K_OLD		47	// 第２６号様式
#define	CHEK18_BUSU_K_OLD		48	// 第２６号様式Ａ４ヨコ控用
#define	CHEK09_BUSU_K_OLD		49	// 消費税の還付申告に関する明細書（表面）
#define	CHEK10_BUSU_K_OLD		50	// 消費税の還付申告に関する明細書（裏面）
#define	CHEK11_BUSU_K_OLD		51	// 更正の請求書
#define	CHEK22_BUSU_K_OLD		52	// 特例１
#define	CHEK12_BUSU_K_OLD		53	// 課税取引金額計算表　事業所得用 
#define	CHEK13_BUSU_K_OLD		54	// 課税取引金額計算表　農業所得用 
#define	CHEK14_BUSU_K_OLD		55	// 課税取引金額計算表　不動産所得用 
#define	CHEK15_BUSU_K_OLD		56	// 課税売上高計算表
#define	CHEK19_BUSU_OLD			57	// 事業別売上高[13'09.19]