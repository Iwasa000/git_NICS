#pragma once

// 入力
#define	TITLE_01	0			// 『申告書・課税標準額等の内訳書』
#define	TITLE_02	1			// 『様式選択・個別部数指定』
#define	COMBO_TEI	2			// 『提出用カラー/モノクロ』コンボ
#define	COMBO_HIK	3			// 『控　用カラー/モノクロ』コンボ
#define	CHEK01_1	4			// 申告書本表
#define	CHEK01_2	5			// 特定課税仕入れがある場合の課税標準額等の内訳書

// 様式選択・個別部数指定
#define	HP_TEI_CHK	6			// 申告書本表			提出用	チェック
#define	HP_TEI_BSU	7			// 申告書本表			提出用	部数
#define	HP_HIK_CHK	8			// 申告書本表			控　用	チェック
#define	HP_HIK_BSU	9			// 申告書本表			控　用	部数
#define	KZ_TEI_CHK	10			// 課税標準額等の内訳書	提出用	チェック
#define	KZ_TEI_BSU	11			// 課税標準額等の内訳書	提出用	部数
#define	KZ_HIK_CHK	12			// 課税標準額等の内訳書	控　用	チェック
#define	KZ_HIK_BSU	13			// 課税標準額等の内訳書	控　用	部数

//印刷ダイアログ、付表２と付表１の並びを反転させた為変更
#define	TITLE_07	14			// 『添付書類』
#define	CHEK04_1	15			// 付表１－１
#define	CHEK04_2	16			// 付表１－２
#define	CHEK04_3	17			// 付表１－３
#define	CHEK03_1	18			// 付表２－１
#define	CHEK03_2	19			// 付表２－２
#define	CHEK03_3	20			// 付表２－３

#define	CHEK20		21			// 付表６
#define	CHEK06		22			// 第２６号様式
#define	CHEK18		23			// 第２６号様式Ａ４ヨコ控用
#define	CHEK09		24			// 消費税の還付申告に関する明細書（表面）
#define	CHEK10		25			// 消費税の還付申告に関する明細書（裏面）
#define	CHEK11		26			// 更正の請求書
#define	CHEK12		27			// 課税取引金額計算表　個別指定用 
#define	CHEK13		28			// 特定収入計算表　個別指定用  
#define	CHEK14		29			// 税務代理書面　個別指定用  
//#define	CHEK15		30			// 課税売上高計算表
//#define	CHEK16		31			// 課税仕入高計算表 
//#define	CHEK17		32			// 特定収入計算表
#define	CHEK22		30			// 特例１
#define	CHEK23		31			// 特例２
#define	CHEK24		32			// 特例３

// 部数
#define	TITLE_08	33			// 『部数』
#define	CHEK04_1_BUSU		34	// 付表１－１
#define	CHEK04_2_BUSU		35	// 付表１－２
#define	CHEK04_3_BUSU		36	// 付表１－３
#define	CHEK03_1_BUSU		37	// 付表２－１
#define	CHEK03_2_BUSU		38	// 付表２－２
#define	CHEK03_3_BUSU		39	// 付表２－３
#define	CHEK20_BUSU			40	// 付表６
#define	CHEK06_BUSU			41	// 第２６号様式
#define	CHEK18_BUSU			42	// 第２６号様式Ａ４ヨコ控用
#define	CHEK09_BUSU			43	// 消費税の還付申告に関する明細書（表面）
#define	CHEK10_BUSU			44	// 消費税の還付申告に関する明細書（裏面）
#define	CHEK11_BUSU			45	// 更正の請求書
#define	CHEK22_BUSU			46	// 特例１
#define	CHEK23_BUSU			47	// 特例２
#define	CHEK24_BUSU			48	// 特例３
#define	CHEK12_BUSU			49	// 課税取引金額計算表　個別指定用 
#define	CHEK13_BUSU			50	// 特定収入計算表　個別指定用  
#define	CHEK14_BUSU			51	// 税務代理書面　個別指定用   
//#define	CHEK15_BUSU			55	// 課税売上高計算表
//#define	CHEK16_BUSU			56	// 課税仕入高計算表 
//#define	CHEK17_BUSU			57	// 特定収入計算表

// 第２６号様式
static SH_ITEMINDEX	PrnTbl1[]	=	{
	{	COMBO_TEI,	0, 0, 0		},	// 提出用カラー/モノクロコンボ
	{	COMBO_HIK,	0, 0, 0		},	// 控　用カラー/モノクロコンボ
	{	CHEK01_1,	0, 0, 0		},	// 申告書本表
	{	CHEK01_2,	0, 0, 0		},	// 特定課税仕入れがある場合の課税標準額等の内訳書
	{	CHEK04_1,	0, 0, 0		},	// 付表１－１
	{	CHEK04_2,	0, 0, 0		},	// 付表１－２
	{	CHEK04_3,	0, 0, 0		},	// 付表１－３
	{	CHEK03_1,	0, 0, 0		},	// 付表２－１
	{	CHEK03_2,	0, 0, 0		},	// 付表２－２
	{	CHEK03_3,	0, 0, 0		},	// 付表２－３

	{	CHEK20,		0, 0, 0		},	// 付表６
	{	CHEK06,		1, 1, 0		},	// 第２６号様式
	{	CHEK18,		1, 1, 0		},	// 第２６号様式Ａ４ヨコ控用
	{	CHEK09,		0, 0, 0		},	// 消費税の還付申告に関する明細書（表面）
	{	CHEK10,		0, 0, 0		},	// 消費税の還付申告に関する明細書（裏面）
	{	CHEK11,		0, 0, 0		},	// 更正の請求書
	{	CHEK22,		0, 0, 0		},	// 特例１
	{	CHEK23,		0, 0, 0		},	// 特例２
	{	CHEK24,		0, 0, 0		},	// 特例３
	{	CHEK12,		0, 0, 0		},	// 課税取引金額計算表　個別指定用 
	{	CHEK13,		0, 0, 0		},	// 特定収入計算表　個別指定用
// 24/04/04_341 cor -->
	//{	CHEK14,		0, 0, 0		},	// 税務代理書面　個別指定用  
// --------------------
	{	CHEK14,		8, 8, 0		},	// 税務代理書面　個別指定用  
// 24/04/04_341 cor <--
//	{	CHEK15,		0, 0, 0		},	// 課税売上高計算表  
//	{	CHEK16,		0, 0, 0		},	// 課税仕入高計算表
//	{	CHEK17,		0, 0, 0		},	// 特定収入計算表
	// 更正の請求書の次に順番変更
//	{	CHEK22,		0, 0, 0		},	// 特例１
//	{	CHEK23,		0, 0, 0		},	// 特例２
//	{	CHEK24,		0, 0, 0		},	// 特例３

	{	0,0,0,0				}
};

// ＯＣＲ印刷
static SH_ITEMINDEX	PrnTbl2[]	=	{
	{	COMBO_TEI,	0, 0, 0		},	// 提出用カラー/モノクロコンボ
	{	COMBO_HIK,	0, 0, 0		},	// 控　用カラー/モノクロコンボ
	{	CHEK01_1,	1, 1, 0		},	// 申告書本表
	{	CHEK01_2,	13,13,0		},	// 特定課税仕入れがある場合の課税標準額等の内訳書
	{	CHEK04_1,	0, 0, 0		},	// 付表１－１
	{	CHEK04_2,	0, 0, 0		},	// 付表１－２
	{	CHEK04_3,	0, 0, 0		},	// 付表１－３
	{	CHEK03_1,	0, 0, 0		},	// 付表２－１
	{	CHEK03_2,	0, 0, 0		},	// 付表２－２
	{	CHEK03_3,	0, 0, 0		},	// 付表２－３

	{	CHEK20,		0, 0, 0		},	// 付表６
	{	CHEK06,		0, 0, 0		},	// 第２６号様式
	{	CHEK18,		0, 0, 0		},	// 第２６号様式Ａ４ヨコ控用
	{	CHEK09,		0, 0, 0		},	// 消費税の還付申告に関する明細書（表面）
	{	CHEK10,		0, 0, 0		},	// 消費税の還付申告に関する明細書（裏面）
	{	CHEK11,		0, 0, 0		},	// 更正の請求書
	{	CHEK22,		0, 0, 0		},	// 特例１
	{	CHEK23,		0, 0, 0		},	// 特例２
	{	CHEK24,		0, 0, 0		},	// 特例３
	{	CHEK12,		0, 0, 0		},	// 課税取引金額計算表　個別指定用 
	{	CHEK13,		0, 0, 0		},	// 特定収入計算表　個別指定用  
	{	CHEK14,		0, 0, 0		},	// 税務代理書面　個別指定用  
//	{	CHEK15,		0, 0, 0		},	// 課税売上高計算表  
//	{	CHEK16,		0, 0, 0		},	// 課税仕入高計算表 
//	{	CHEK17,		0, 0, 0		},	// 特定収入計算表
	//{	CHEK22,		0, 0, 0		},	// 特例１
	//{	CHEK23,		0, 0, 0		},	// 特例２
	//{	CHEK24,		0, 0, 0		},	// 特例３

	{	0,0,0,0				}
};

// イメージ印刷
static SH_ITEMINDEX	PrnTbl3[]	=	{
	{	COMBO_TEI,	1, 1, 0		},	// 提出用カラー/モノクロコンボ
	{	COMBO_HIK,	1, 1, 0		},	// 控　用カラー/モノクロコンボ
	{	CHEK01_1,	1, 1, 0		},	// 申告書本表
	{	CHEK01_2,	13,13,0		},	// 特定課税仕入れがある場合の課税標準額等の内訳書
	{	CHEK04_1,	1, 1, 0		},	// 付表１－１
	{	CHEK04_2,	0, 1, 0		},	// 付表１－２
	{	CHEK04_3,	1, 1, 0		},	// 付表１－３
	{	CHEK03_1,	1, 1, 0		},	// 付表２－１
	{	CHEK03_2,	0, 1, 0		},	// 付表２－２
	{	CHEK03_3,	1, 1, 0		},	// 付表２－３

	{	CHEK06,		0, 0, 0		},	// 第２６号様式
	{	CHEK18,		0, 0, 0		},	// 第２６号様式Ａ４ヨコ控用
	{	CHEK20,		14,14,0		},	// 付表６
	{	CHEK09,		4, 4, 0		},	// 消費税の還付申告に関する明細書（表面）
	{	CHEK10,		4, 4, 0		},	// 消費税の還付申告に関する明細書（裏面）
	{	CHEK11,		5, 5, 0		},	// 更正の請求書
	{	CHEK22,		15, 15, 0	},	// 特例１
	{	CHEK23,		16, 16, 0	},	// 特例２
	{	CHEK24,		17, 17, 0	},	// 特例３
	{	CHEK12,		6, 6, 0		},	// 課税取引金額計算表　個別指定用 
	{	CHEK13,		7, 7, 0		},	// 特定収入計算表　個別指定用  
	{	CHEK14,		8, 8, 0		},	// 税務代理書面　個別指定用  
//	{	CHEK15,		9, 9, 0		},	// 課税売上高計算表 
//	{	CHEK16,		10,10, 0	},	// 課税仕入高計算表
//	{	CHEK17,		11,11, 0	},	// 特定収入計算表
	{	0,0,0,0				}
};

// 部数関連
LST_BUSU	LstBusuTbl[] = {
	{CHEK01_1,HP_TEI_CHK,HP_TEI_BSU,HP_HIK_CHK,HP_HIK_BSU,-1},
	{CHEK01_2,KZ_TEI_CHK,KZ_TEI_BSU,KZ_HIK_CHK,KZ_HIK_BSU,-1},
	{CHEK04_1,CHEK04_1_BUSU,-1},
	{CHEK04_2,CHEK04_2_BUSU,-1},
	{CHEK04_3,CHEK04_3_BUSU,-1},
	{CHEK03_1,CHEK03_1_BUSU,-1},
	{CHEK03_2,CHEK03_2_BUSU,-1},
	{CHEK03_3,CHEK03_3_BUSU,-1},
	{CHEK20,CHEK20_BUSU,-1},
	{CHEK06,CHEK06_BUSU,-1},
	{CHEK18,CHEK18_BUSU,-1},
	{CHEK09,CHEK09_BUSU,-1},
	{CHEK10,CHEK10_BUSU,-1},
	{CHEK11,CHEK11_BUSU,-1},
	{CHEK22,CHEK22_BUSU,-1},
	{CHEK23,CHEK23_BUSU,-1},
	{CHEK24,CHEK24_BUSU,-1},
	{CHEK12,CHEK12_BUSU,-1},
	{CHEK13,CHEK13_BUSU,-1},
	{CHEK14,CHEK14_BUSU,-1},
//	{CHEK15,CHEK15_BUSU,-1},
//	{CHEK16,CHEK16_BUSU,-1},
//	{CHEK17,CHEK17_BUSU,-1},
	{-1},
};

// 画面の番号、印刷の番号から部数が SH_PRINT_CMINFO::busu[50]
// の何番目に格納されているかを調べるためのテーブル
LST_BUSU_IDX	LstBusuIdxGen[] = {
{HP_TEI_BSU,0x01,-1},								// 申告書本表			提出用カラー
{HP_TEI_BSU,0x03,-1},								// 申告書本表			提出用モノクロ
{HP_HIK_BSU,0x02,-1},								// 申告書本表			控用カラー
{HP_HIK_BSU,0x04,-1},								// 申告書本表			控用モノクロ
{KZ_TEI_BSU,0x57,-1},								// 課税標準額等の内訳書	提出用カラー
{KZ_TEI_BSU,0x05,-1},								// 課税標準額等の内訳書	提出用モノクロ
{KZ_HIK_BSU,0x58,-1},								// 課税標準額等の内訳書	控用カラー
{KZ_HIK_BSU,0x06,-1},								// 課税標準額等の内訳書	控用モノクロ
{CHEK04_1_BUSU,0x70,-1},							// 付表１－１
{CHEK04_2_BUSU,0x71,-1},							// 付表１－２
{CHEK04_3_BUSU,0x17,-1},							// 付表１－３
{CHEK03_1_BUSU,0x72,-1},							// 付表２－１
{CHEK03_2_BUSU,0x73,-1},							// 付表２－２
{CHEK03_3_BUSU,0x18,-1},							// 付表２－３
{CHEK20_BUSU,0x16,-1},								// 付表６
{CHEK06_BUSU,0x50,-1},								// 第２６号様式
{CHEK18_BUSU,0x51,-1},								// 第２６号様式Ａ４ヨコ控用
{CHEK09_BUSU,0x07,-1},								// 消費税の還付申告に関する明細書（表面）
{CHEK10_BUSU,0x08,-1},								// 消費税の還付申告に関する明細書（裏面）
{CHEK11_BUSU,0x09,-1},								// 更正の請求書
{CHEK22_BUSU,0x76,-1},								// 特例１
{CHEK23_BUSU,0x77,-1},								// 特例２
{CHEK24_BUSU,0x78,-1},								// 特例３
// 24/03/22_411 cor -->
//{CHEK12_BUSU,0x12,-1},								// 課税取引金額計算表　個別指定用 
//{CHEK13_BUSU,0x14,-1},								// 特定収入計算表　個別指定用  
//{CHEK14_BUSU,0x13,-1},								// 税務代理書面　個別指定用   
// --------------------
// 課税取引金額計算表　個別指定用 
{CHEK12_BUSU,0x10,0x11,0x12,0x13,0x14,0x52,-1},
// 特定収入計算表　個別指定用  
{CHEK13_BUSU,0x40,0x41,0x42,0x56,0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,-1},
// 税務代理書面　個別指定用   
{CHEK14_BUSU,0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x90,-1},
// 24/03/22_411 cor <--
//{CHEK15_BUSU,0x10,-1},								// 課税売上高計算表
//{CHEK16_BUSU,0x11,-1},								// 課税仕入高計算表 
// 24/02/26_特定収入対応 cor -->
//{CHEK17_BUSU,0x40,0x41,0x42,0x56,0x60,0x61,-1},		// 特定収入計算表
// -----------------------------
//{CHEK17_BUSU,0x40,0x41,0x42,0x56,0x60,0x61,
//0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,-1},		// 特定収入計算表
// 24/02/26_特定収入対応 cor <--
{-1,-1},
};

short	CursorPos[30][10]=
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
	{CHEK03_3,CHEK03_3_BUSU,-1,-1,-1,-1,-1,-1,-1,-1},
	{CHEK20,CHEK20_BUSU,-1,-1,-1,-1,-1,-1,-1,-1},
	{CHEK06,CHEK06_BUSU,-1,-1,-1,-1,-1,-1,-1,-1},
	{CHEK18,CHEK18_BUSU,-1,-1,-1,-1,-1,-1,-1,-1},
	{CHEK09,CHEK09_BUSU,-1,-1,-1,-1,-1,-1,-1,-1},
	{CHEK10,CHEK10_BUSU,-1,-1,-1,-1,-1,-1,-1,-1},
	{CHEK11,CHEK11_BUSU,-1,-1,-1,-1,-1,-1,-1,-1},
	{CHEK22,CHEK22_BUSU,-1,-1,-1,-1,-1,-1,-1,-1},
	{CHEK23,CHEK23_BUSU,-1,-1,-1,-1,-1,-1,-1,-1},
	{CHEK24,CHEK24_BUSU,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{CHEK12,CHEK12_BUSU,-1,-1,-1,-1,-1,-1,-1,-1},
	{CHEK13,CHEK13_BUSU,-1,-1,-1,-1,-1,-1,-1,-1},
	{CHEK14,CHEK14_BUSU,-1,-1,-1,-1,-1,-1,-1,-1},
//	{CHEK15,CHEK15_BUSU,-1,-1,-1,-1,-1,-1,-1,-1},
//	{CHEK16,CHEK16_BUSU,-1,-1,-1,-1,-1,-1,-1,-1},
//	{CHEK17,CHEK17_BUSU,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}
};




//旧保存


// 入力
#define	TITLE_01_OLD	0			// 『申告書・課税標準額等の内訳書』
#define	TITLE_02_OLD	1			// 『様式選択・個別部数指定』
#define	COMBO_TEI_OLD	2			// 『提出用カラー/モノクロ』コンボ
#define	COMBO_HIK_OLD	3			// 『控　用カラー/モノクロ』コンボ
#define	CHEK01_1_OLD	4			// 申告書本表
#define	CHEK01_2_OLD	5			// 特定課税仕入れがある場合の課税標準額等の内訳書

// 様式選択・個別部数指定
#define	HP_TEI_CHK_OLD	6			// 申告書本表			提出用	チェック
#define	HP_TEI_BSU_OLD	7			// 申告書本表			提出用	部数
#define	HP_HIK_CHK_OLD	8			// 申告書本表			控　用	チェック
#define	HP_HIK_BSU_OLD	9			// 申告書本表			控　用	部数
#define	KZ_TEI_CHK_OLD	10			// 課税標準額等の内訳書	提出用	チェック
#define	KZ_TEI_BSU_OLD	11			// 課税標準額等の内訳書	提出用	部数
#define	KZ_HIK_CHK_OLD	12			// 課税標準額等の内訳書	控　用	チェック
#define	KZ_HIK_BSU_OLD	13			// 課税標準額等の内訳書	控　用	部数

//印刷ダイアログ、付表２と付表１の並びを反転させた為変更
#define	TITLE_07_OLD	14			// 『添付書類』
#define	CHEK04_1_OLD	15			// 付表１－１
#define	CHEK04_2_OLD	16			// 付表１－２
#define	CHEK04_3_OLD	17			// 付表１－３
#define	CHEK03_1_OLD	18			// 付表２－１
#define	CHEK03_2_OLD	19			// 付表２－２
#define	CHEK03_3_OLD	20			// 付表２－３

#define	CHEK20_OLD		21			// 付表６
#define	CHEK06_OLD		22			// 第２６号様式
#define	CHEK18_OLD		23			// 第２６号様式Ａ４ヨコ控用
#define	CHEK09_OLD		24			// 消費税の還付申告に関する明細書（表面）
#define	CHEK10_OLD		25			// 消費税の還付申告に関する明細書（裏面）
#define	CHEK11_OLD		26			// 更正の請求書
#define	CHEK12_OLD		27			// 課税取引金額計算表　事業所得用 
#define	CHEK13_OLD		28			// 課税取引金額計算表　農業所得用 
#define	CHEK14_OLD		29			// 課税取引金額計算表　不動産所得用  
#define	CHEK15_OLD		30			// 課税売上高計算表
#define	CHEK16_OLD		31			// 課税仕入高計算表 
#define	CHEK17_OLD		32			// 特定収入計算表
#define	CHEK22_OLD		33			// 特例１
#define	CHEK23_OLD		34			// 特例２
#define	CHEK24_OLD		35			// 特例３

// 部数
#define	TITLE_08_OLD	36			// 『部数』
#define	CHEK04_1_BUSU_OLD		37	// 付表１－１
#define	CHEK04_2_BUSU_OLD		38	// 付表１－２
#define	CHEK04_3_BUSU_OLD		39	// 付表１－３
#define	CHEK03_1_BUSU_OLD		40	// 付表２－１
#define	CHEK03_2_BUSU_OLD		41	// 付表２－２
#define	CHEK03_3_BUSU_OLD		42	// 付表２－３
#define	CHEK20_BUSU_OLD			43	// 付表６
#define	CHEK06_BUSU_OLD			44	// 第２６号様式
#define	CHEK18_BUSU_OLD			45	// 第２６号様式Ａ４ヨコ控用
#define	CHEK09_BUSU_OLD			46	// 消費税の還付申告に関する明細書（表面）
#define	CHEK10_BUSU_OLD			47	// 消費税の還付申告に関する明細書（裏面）
#define	CHEK11_BUSU_OLD			48	// 更正の請求書
#define	CHEK22_BUSU_OLD			49	// 特例１
#define	CHEK23_BUSU_OLD		50	// 特例２
#define	CHEK24_BUSU_OLD			51	// 特例３
#define	CHEK12_BUSU_OLD			52	// 課税取引金額計算表　事業所得用 
#define	CHEK13_BUSU_OLD			53	// 課税取引金額計算表　農業所得用 
#define	CHEK14_BUSU_OLD			54	// 課税取引金額計算表　不動産所得用  
#define	CHEK15_BUSU_OLD			55	// 課税売上高計算表
#define	CHEK16_BUSU_OLD			56	// 課税仕入高計算表 
#define	CHEK17_BUSU_OLD			57	// 特定収入計算表