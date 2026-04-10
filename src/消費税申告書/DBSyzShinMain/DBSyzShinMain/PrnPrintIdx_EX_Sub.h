// 24/02/19wd_特定収入対応 add -->
#pragma once

// 特定収入計画表 個別出力設定
#define	TITLE_SUB_01		0			// 『添付書類』
#define	CHECK_SUB_01		1			// 計画表１
#define	CHECK_SUB_02		2			// 計画表２(１)
#define	CHECK_SUB_03		3			// 計画表２(２)
#define	CHECK_SUB_04		4			// 計画表３、４
#define	CHECK_SUB_05		5			// 計画表５(１)
#define	CHECK_SUB_06		6			// 計画表５(２)
#define	CHECK_SUB_07		7			// 計画表５(３)
#define	CHECK_SUB_08		8			// 計画表５－２(１)
#define	CHECK_SUB_09		9			// 計画表５－２(２)
#define	CHECK_SUB_10		10			// 計画表５－２(３)－１
#define	CHECK_SUB_11		11			// 計画表５－２(３)－２
#define	CHECK_SUB_12		12			// 計画表５－２(３)－３
#define	CHECK_SUB_13		13			// 計画表５－２(４)－１
#define	CHECK_SUB_14		14			// 計画表５－２(４)－２
#define	CHECK_SUB_15		15			// 計画表５－２(４)－３

#define	TITLE_SUB_02		16			// 『部数』
#define	BUSU_SUB_01			17			// 計画表１
#define	BUSU_SUB_02			18			// 計画表２(１)
#define	BUSU_SUB_03			19			// 計画表２(２)
#define	BUSU_SUB_04			20			// 計画表３、４
#define	BUSU_SUB_05			21			// 計画表５(１)
#define	BUSU_SUB_06			22			// 計画表５(２)
#define	BUSU_SUB_07			23			// 計画表５(３)
#define	BUSU_SUB_08			24			// 計画表５－２(１)
#define	BUSU_SUB_09			25			// 計画表５－２(２)
#define	BUSU_SUB_10			26			// 計画表５－２(３)－１
#define	BUSU_SUB_11			27			// 計画表５－２(３)－２
#define	BUSU_SUB_12			28			// 計画表５－２(３)－３
#define	BUSU_SUB_13			29			// 計画表５－２(４)－１
#define	BUSU_SUB_14			30			// 計画表５－２(４)－２
#define	BUSU_SUB_15			31			// 計画表５－２(４)－３

#define	TITLE_SUB_03		32			// 『開始頁』
#define	STPG_SUB_01			33			// 計画表１
#define	STPG_SUB_02			34			// 計画表２(１)
#define	STPG_SUB_03			35			// 計画表２(２)
#define	STPG_SUB_04			36			// 計画表３、４
#define	STPG_SUB_05			37			// 計画表５(１)
#define	STPG_SUB_06			38			// 計画表５(２)
#define	STPG_SUB_07			39			// 計画表５(３)
#define	STPG_SUB_08			40			// 計画表５－２(１)
#define	STPG_SUB_09			41			// 計画表５－２(２)
#define	STPG_SUB_10			42			// 計画表５－２(３)－１
#define	STPG_SUB_11			43			// 計画表５－２(３)－２
#define	STPG_SUB_12			44			// 計画表５－２(３)－３
#define	STPG_SUB_13			45			// 計画表５－２(４)－１
#define	STPG_SUB_14			46			// 計画表５－２(４)－２
#define	STPG_SUB_15			47			// 計画表５－２(４)－３

#define	TITLE_SUB_04		48			// 『終了頁』
#define	EDPG_SUB_01			49			// 計画表１
#define	EDPG_SUB_02			50			// 計画表２(１)
#define	EDPG_SUB_03			51			// 計画表２(２)
#define	EDPG_SUB_04			52			// 計画表３、４
#define	EDPG_SUB_05			53			// 計画表５(１)
#define	EDPG_SUB_06			54			// 計画表５(２)
#define	EDPG_SUB_07			55			// 計画表５(３)
#define	EDPG_SUB_08			56			// 計画表５－２(１)
#define	EDPG_SUB_09			57			// 計画表５－２(２)
#define	EDPG_SUB_10			58			// 計画表５－２(３)－１
#define	EDPG_SUB_11			59			// 計画表５－２(３)－２
#define	EDPG_SUB_12			60			// 計画表５－２(３)－３
#define	EDPG_SUB_13			61			// 計画表５－２(４)－１
#define	EDPG_SUB_14			62			// 計画表５－２(４)－２
#define	EDPG_SUB_15			63			// 計画表５－２(４)－３

short	CursorPos[16][5]=
{
	//{CHECK_SUB_01, BUSU_SUB_01, STPG_SUB_01, EDPG_SUB_01, -1},
	//{CHECK_SUB_02, BUSU_SUB_02, STPG_SUB_02, EDPG_SUB_02, -1},
	//{CHECK_SUB_03, BUSU_SUB_03, STPG_SUB_03, EDPG_SUB_03, -1},
	//{CHECK_SUB_04, BUSU_SUB_04, STPG_SUB_04, EDPG_SUB_04, -1},
	//{CHECK_SUB_05, BUSU_SUB_05, STPG_SUB_05, EDPG_SUB_05, -1},
	//{CHECK_SUB_06, BUSU_SUB_06, STPG_SUB_06, EDPG_SUB_06, -1},
	//{CHECK_SUB_07, BUSU_SUB_07, STPG_SUB_07, EDPG_SUB_07, -1},
	{CHECK_SUB_01, BUSU_SUB_01, -1, -1, -1},
	{CHECK_SUB_02, BUSU_SUB_02, -1, -1, -1},
	{CHECK_SUB_03, BUSU_SUB_03, -1, -1, -1},
	{CHECK_SUB_04, BUSU_SUB_04, -1, -1, -1},
	{CHECK_SUB_05, BUSU_SUB_05, -1, -1, -1},
	{CHECK_SUB_06, BUSU_SUB_06, -1, -1, -1},
	{CHECK_SUB_07, BUSU_SUB_07, -1, -1, -1},
	{CHECK_SUB_08, BUSU_SUB_08, STPG_SUB_08, EDPG_SUB_08, -1},
	{CHECK_SUB_09, BUSU_SUB_09, STPG_SUB_09, EDPG_SUB_09, -1},
	{CHECK_SUB_10, BUSU_SUB_10, STPG_SUB_10, EDPG_SUB_10, -1},
	{CHECK_SUB_11, BUSU_SUB_11, STPG_SUB_11, EDPG_SUB_11, -1},
	{CHECK_SUB_12, BUSU_SUB_12, STPG_SUB_12, EDPG_SUB_12, -1},
	{CHECK_SUB_13, BUSU_SUB_13, STPG_SUB_13, EDPG_SUB_13, -1},
	{CHECK_SUB_14, BUSU_SUB_14, STPG_SUB_14, EDPG_SUB_14, -1},
	{CHECK_SUB_15, BUSU_SUB_15, STPG_SUB_15, EDPG_SUB_15, -1},
	{-1,-1,-1,-1,-1}
};

// 24/02/19wd_特定収入対応 add <--
