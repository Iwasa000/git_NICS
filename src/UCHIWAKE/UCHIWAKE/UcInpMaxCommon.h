/*****************************************************************
	UcInpMaxCommon.h

	内訳書の最大入力値ヘッダファイル
		定数、列挙型、構造体

	2006.11.20	New
******************************************************************/

#ifndef	__UCINPMAXCOMMON_H__
#define	__UCINPMAXCOMMON_H__

//①預貯金等の最大入力値
//////////////////////////////////////////////////////////////////
//	預貯金等の入力コントロールの最大桁数
#define MAX_KETA_011_KINYU		10		//	金融機関名
#define MAX_KETA_011_KINYUSUB	10		//	金融機関支店名
#define MAX_KETA_011_KAMOKU		12		//	種類
#define MAX_KETA_011_KOUZA		14		//	口座番号
#define MAX_KETA_011_TEKIYOU	40		//	摘要

//////////////////////////////////////////////////////////////////
//	預貯金等の内訳書_コントロールのの最大桁数
#define ID_ROWNORMAL_011		24		// １頁内の合計行を含む全体の行数

//////////////////////////////////////////////////////////////////
//	預貯金等の内訳書_金額欄の最大桁数
#define	ID_VAL_011_DATA			11		//	 11	データ行の金額欄最大桁数
#define	ID_VAL_011_TOTAL		12		//	 12	計行の金額欄最大桁数
// No.200903 add -->
#define	ID_VAL_011_K_DATA		13		//	 13	データ行の金額欄最大桁数
#define	ID_VAL_011_K_TOTAL		13		//	 13	計行の金額欄最大桁数
// No.200903 add <--

//②受取手形の最大入力値
//////////////////////////////////////////////////////////////////
//	受取手形の入力コントロールの最大桁数
#define MAX_KETA_021_KAMOKU		12		//	科目
#define MAX_KETA_021_ADNAME1	20		//	差出人１
#define MAX_KETA_021_SPDATE		6		//	振出年月日
#define MAX_KETA_021_PMDATE		6		//	支払期日
#define MAX_KETA_021_BKNAME1	10		//	支払銀行名
#define MAX_KETA_021_BKNAME2	10		//	支払銀行名（支店名）
#define MAX_KETA_021_BK2NAME1	10		//	割引銀行名
#define MAX_KETA_021_BK2NAME2	10		//	割引銀行名（支店名）
#define MAX_KETA_021_TEKI		32		//	摘要
// midori 156189,156190,156191 add -->
#define MAX_KETA_021_SYOKEI		26		//	小計名称（1行の桁数）
// midori 156189,156190,156191 add <--
#define MAX_KETA_021_INVONO		13		//	インボイス登録番号	// インボイス登録番号追加対応_23/09/11 add

//////////////////////////////////////////////////////////////////
//	「受取手形」の内訳書_コントロールの最大桁数
#define	ID_ROWNORMAL_021		24		// １頁内の合計行を含む全体の行数

//////////////////////////////////////////////////////////////////
//	「受取手形」の内訳書_金額欄の最大桁数
#define	ID_VAL_021_DATA			11		// 金額nのデータ行の最大桁数
#define	ID_VAL_021_TOTAL		12		// 金額の計行の最大桁数
// No.200903 add -->
#define	ID_VAL_021_K_DATA		13		// 金額nのデータ行の最大桁数
#define	ID_VAL_021_K_TOTAL		13		// 金額の計行の最大桁数
// No.200903 add <--

//③受取手形の最大入力値
//////////////////////////////////////////////////////////////////
//	売掛金の入力コントロールの最大桁数
#define MAX_KETA_031_KAMOKU		12		//	科目
#define MAX_KETA_031_ADNAME1	20		//	名称１
#define MAX_KETA_031_ADADD1		20		//	住所１
#define MAX_KETA_031_ADADD2		20		//	住所２
#define MAX_KETA_031_TEKI		26		//	摘要

//////////////////////////////////////////////////////////////////
//	「売掛金」の内訳書_コントロールの最大桁数
#define	ID_ROWNORMAL_031		27		// １頁内の合計行を含む全体の行数

//////////////////////////////////////////////////////////////////
//	「売掛金」の内訳書_金額欄の最大桁数
#define	ID_VAL_031_DATA			11		// 期末現在高nのデータ行の最大桁数
#define	ID_VAL_031_TOTAL		12		// 期末現在高の計行の最大桁数
// No.200903 add -->
#define	ID_VAL_031_K_DATA		13		// 期末現在高nのデータ行の最大桁数
#define	ID_VAL_031_K_TOTAL		13		// 期末現在高の計行の最大桁数
// No.200903 add <--

//④-1仮払金の最大入力値
//////////////////////////////////////////////////////////////////
//	入力コントロールの最大桁数
#define MAX_KETA_041_KAMOKU		12		//	科目
#define MAX_KETA_041_ADNAME1	20		//	名称１
#define MAX_KETA_041_ADADD1		20		//	住所１
#define MAX_KETA_041_ADADD2		20		//	住所２
#define MAX_KETA_041_KANKEI		6		//	法人・代表者との関係
#define MAX_KETA_041_NAIYOU		20		//	取引の内容

//////////////////////////////////////////////////////////////////
//	「仮払金」の内訳書_コントロールの最大桁数
#define	ID_ROWNORMAL_041		11		// 通常時：１頁内の合計行を含む全体の行数
#define	ID_ROWKOBETU_041		29		// 個別時：１頁内の合計行を含む全体の行数

//////////////////////////////////////////////////////////////////
//	「仮払金」の内訳書_金額欄の最大桁数
#define	ID_VAL_041_DATA			11		// 期末現在高nのデータ行の最大桁数
#define	ID_VAL_041_TOTAL		12		// 期末現在高の計行の最大桁数
// No.200903 add -->
#define	ID_VAL_041_K_DATA		12		// 期末現在高nのデータ行の最大桁数
// No.200903 add <--

//④-2貸付金の最大入力値
//////////////////////////////////////////////////////////////////
//	貸付金の入力コントロールの最大桁数
#define MAX_KETA_042_ADNAME1	20		//	貸付先１
#define MAX_KETA_042_KANKEI		6		//	法人・代表者との関係
#define MAX_KETA_042_ADADD1		20		//	所在地１
#define MAX_KETA_042_ADADD2		20		//	所在地２
#define MAX_KETA_042_RIYUU		24		//	貸付理由
#define MAX_KETA_042_NAIYOU		30		//	担保の内容

//////////////////////////////////////////////////////////////////
//	「貸付金」の内訳書_コントロールの最大桁数
#define	ID_ROWNORMAL_042		8		// 通常時：１頁内の合計行を含む全体の行数
#define	ID_ROWKOBETU_042		19		// 個別時：１頁内の合計行を含む全体の行数

//////////////////////////////////////////////////////////////////
//	「貸付金」の内訳書_金額欄の最大桁数
#define	ID_VAL_042_DATA			11		// 期末現在高nのデータ行の最大桁数
#define	ID_VAL_042_TOTAL		12		// 期末現在高の計行の最大桁数
// No.200903 del -->
//#define	ID_RISOKUL_042_DATA	9		// 期中の受取利息額nのデータ行の最大桁数
// No.200903 del <--
// No.200903 add -->
#define	ID_RISOKU_042_DATA		9		// 期中の受取利息額のデータ行の最大桁数
// No.200903 add <--
#define	ID_RISOKU_042_TOTAL		10		// 期中の受取利息額の計行の最大桁数
// No.200903 add -->
#define	ID_VAL_042_K_DATA		12		// 期末現在高のデータ行の最大桁数
#define	ID_RISOKU_042_K_DATA	10		// 期中の受取利息額のデータ行の最大桁数
// No.200903 add <--


//⑤棚卸資産の最大入力値
//////////////////////////////////////////////////////////////////
//	棚卸資産の入力コントロールの最大桁数
#define MAX_KETA_051_KAMOKU		12		//	科目
#define MAX_KETA_051_HINNMOKU	20		//	品目
#define MAX_KETA_051_TANNI		3		//	単位
#define MAX_KETA_051_TEKIYOU	30		//	摘要

//////////////////////////////////////////////////////////////////
//	「棚卸資産」の内訳書_コントロールの最大桁数
#define ID_ROWNORMAL_051		25		// １頁内の合計行を含む全体の行数

//////////////////////////////////////////////////////////////////
//	「棚卸資産」の内訳書_金額欄の最大桁数
#define	ID_VAL_051_DATA			12	//	 12	データ行の金額欄最大桁数
// No.200903 add -->
#define	ID_VAL_051_K_DATA		13	//	 13	データ行の金額欄最大桁数（※手入力で作成された一括金額データが計行と同じ桁数まで入力できてしまうので）
// No.200903 add <--
#define	ID_VAL_051_TOTAL		13	//	 13	計行の金額欄最大桁数

//////////////////////////////////////////////////////////////////
//	期末棚卸の方法(欄外)
#define	DT_METHOD_NOTHING		0	//	なし
#define	DT_METHOD_A				1	//	Ａ　実施棚卸
#define	DT_METHOD_B				2	//	Ｂ　帳簿棚卸
#define	DT_METHOD_C				3	//	Ｃ	ＡとＢの併用
#define	DT_METHOD_D				4	//	Ｄ　登録名称を印字


// ⑥有価証券の最大入力値
//////////////////////////////////////////////////////////////////
//	入力コントロールの最大桁数
#define MAX_KETA_061_KUBUN		12		//	区分
#define MAX_KETA_061_SYURUI		16		//	種類・銘柄
#define MAX_KETA_061_TANNI		3		//	単位１
#define MAX_KETA_061_SPDATE		6		//	異動年月日	
#define MAX_KETA_061_RIYUU		12		//	異動事由
#define MAX_KETA_061_TANNI2		3		//	単位２
#define MAX_KETA_061_ADNAME1	30		//	売却先の名称１
#define MAX_KETA_061_ADADD1		15		//	売却先の所在地１
#define MAX_KETA_061_ADADD2		15		//	売却先の所在地２
#define MAX_KETA_061_TEKI		30		//	摘要

// midori 190301 add -->
// 簡素化用最大桁数
#define MAX_KETA_061K_SYURUI		8		//	種類
#define MAX_KETA_061K_MEIGARA		8		//	銘柄
// midori 190301 add <--

//////////////////////////////////////////////////////////////////
//	「有価証券」の内訳書_コントロールの最大桁数
#define	ID_ROWNORMAL_061		16		// １頁内の合計行を含む全体の行数

//////////////////////////////////////////////////////////////////
//	「有価証券」の内訳書_金額欄の最大桁数
#define	ID_VAL_061_DATA	11	// 金額(1)nのデータ行の最大桁数
#define	ID_VAL_061_TOTAL	12	// 金額(1)の計行の最大桁数
#define	ID_VAL2_061_DATA	11	// 金額(2)nのデータ行の最大桁数
#define	ID_VAL2_061_TOTAL	12	// 金額(2)の計行の最大桁数
#define	ID_VAL3_061_DATA	11	// 金額nのデータ行の最大桁数
#define	ID_VAL3_061_TOTAL	12	// 金額の計行の最大桁数
// No.200903 add -->
#define	ID_VAL_061_K_DATA	12	// 金額(1)nのデータ行の最大桁数
#define	ID_VAL2_061_K_DATA	12	// 金額(2)nのデータ行の最大桁数
#define	ID_VAL3_061_K_DATA	12	// 金額nのデータ行の最大桁数
// No.200903 add <--

// ⑦有価証券の最大入力値
//////////////////////////////////////////////////////////////////
//	入力コントロールの最大桁数
#define MAX_KETA_071_SYURUI		14		//	種類・構造
#define MAX_KETA_071_YOUTO		10		//	用途
#define MAX_KETA_071_AD2ADD1	20		//	物件の所在地１
#define MAX_KETA_071_AD2ADD2	20		//	物件の所在地２
#define MAX_KETA_071_SPDATE		6		//	異動年月日
#define MAX_KETA_071_RIYUU		12		//	異動事由
#define MAX_KETA_071_ADNAME1	30		//	売却先の名称１
#define MAX_KETA_071_ADADD1		15		//	売却先の所在地１
#define MAX_KETA_071_ADADD2		15		//	売却先の所在地２
#define MAX_KETA_071_SPDATE2	6		//	売却物件の取得年月

//////////////////////////////////////////////////////////////////
//	「固定資産」の内訳書_コントロールの最大桁数
#define	ID_ROWNORMAL_071		10		// １頁内の合計行を含む全体の行数

//////////////////////////////////////////////////////////////////
//	「固定資産」の内訳書_金額欄の最大桁数
#define	ID_VAL_071_DATA			12		// 期末現在高nのデータ行の最大桁数
#define	ID_VAL_071_TOTAL		13		// 期末現在高の計行の最大桁数
#define	ID_MSVAL_071_DATA		11		// 所得(処分)価額nのデータ行の最大桁数
#define	ID_MSVAL_071_TOTAL		12		// 所得(処分)価額の計行の最大桁数
#define	ID_MSVAL2_071_DATA		11		// 異動直前の帳簿価額nのデータ行の最大桁数
#define	ID_MSVAL2_071_TOTAL		12		// 異動直前の帳簿価額の計行の最大桁数
// 修正No.157648 add -->
#define	ID_VAL_071_K_DATA		13		// 期末現在高nのデータ行の最大桁数
// 修正No.157648 add <--
// No.200903 add -->
#define	ID_MSVAL_071_K_DATA		12		// 所得(処分)価額nのデータ行の最大桁数
#define	ID_MSVAL2_071_K_DATA	12		// 異動直前の帳簿価額nのデータ行の最大桁数
// No.200903 add <--

// ⑧支払手形の最大入力値
//////////////////////////////////////////////////////////////////
//	入力コントロールの最大桁数
#define MAX_KETA_081_KAMOKU		12		//	科目					// 改良No.21-0086,21-0529 add
#define MAX_KETA_081_ADNAME1	20		//	支払先１
#define MAX_KETA_081_SPDATE		6		//	振出年月日
#define MAX_KETA_081_PMDATE		6		//	支払期日
#define MAX_KETA_081_BKNAME1	10		//	支払銀行名
#define MAX_KETA_081_BKNAME2	10		//	支払銀行名（支店名）
#define MAX_KETA_081_TEKI		40		//	摘要
#define MAX_KETA_081_SYOKEI		26		//	小計名称（1行の桁数）	// 改良No.21-0086,21-0529 add

//////////////////////////////////////////////////////////////////
//	「支払手形」の内訳書_コントロールの最大桁数
#define	ID_ROWNORMAL_081		24		// １頁内の合計行を含む全体の行数

//////////////////////////////////////////////////////////////////
//	「支払手形」の内訳書_金額欄の最大桁数
#define	ID_VAL_081_DATA			11		// 金額nのデータ行の最大桁数
#define	ID_VAL_081_TOTAL		12		// 金額の計行の最大桁数
// No.200903 add -->
#define	ID_VAL_081_K_DATA		13		// 金額nのデータ行の最大桁数
#define	ID_VAL_081_K_TOTAL		13		// 金額の計行の最大桁数
// No.200903 add <--

// ⑨買掛金の最大入力値
//////////////////////////////////////////////////////////////////
//	入力コントロールの最大桁数
#define MAX_KETA_091_KAMOKU		12		//	科目
#define MAX_KETA_091_ADNAME1	20		//	名称１
#define MAX_KETA_091_ADADD1		20		//	住所１
#define MAX_KETA_091_ADADD2		20		//	住所２
#define MAX_KETA_091_TEKI		20		//	摘要

//////////////////////////////////////////////////////////////////
//	「買掛金」の内訳書_コントロールの最大桁数
#define	ID_ROWNORMAL_091		24		// １頁内の合計行を含む全体の行数

//////////////////////////////////////////////////////////////////
//	「買掛金」の内訳書_金額欄の最大桁数
#define	ID_VAL_091_DATA			11		// 金額nのデータ行の最大桁数
#define	ID_VAL_091_TOTAL		12		// 金額の計行の最大桁数
// No.200903 add -->
#define	ID_VAL_091_K_DATA		13		// 金額nのデータ行の最大桁数
#define	ID_VAL_091_K_TOTAL		13		// 金額の計行の最大桁数
// No.200903 add <--

// ⑩-1仮受金の最大入力値
//////////////////////////////////////////////////////////////////
//	入力コントロールの最大桁数
#define MAX_KETA_101_KAMOKU		12		//	科目
#define MAX_KETA_101_ADNAME1	20		//	名称１
#define MAX_KETA_101_ADADD1		20		//	住所１
#define MAX_KETA_101_ADADD2		20		//	住所２
#define MAX_KETA_101_KANKEI		6		//	法人・代表者との関係
#define MAX_KETA_101_NAIYOU		20		//	取引の内容

//////////////////////////////////////////////////////////////////
//	「仮受金」の内訳書_コントロールの最大桁数
#define	ID_ROWNORMAL_101		12		// 通常時：１頁内の合計行を含む全体の行数
#define	ID_ROWKOBETU_101		24		// 個別時：１頁内の合計行を含む全体の行数

//////////////////////////////////////////////////////////////////
//	「仮受金」の内訳書_金額欄の最大桁数
#define	ID_VAL_101_DATA			11		// 金額nのデータ行の最大桁数
#define	ID_VAL_101_TOTAL		12		// 金額の計行の最大桁数
// No.200903 add -->
#define	ID_VAL_101_K_DATA		12		// 金額nのデータ行の最大桁数
// No.200903 add <--

// ⑩-2源泉所得税の最大入力値
//////////////////////////////////////////////////////////////////
//	入力コントロールの最大桁数
#define MAX_KETA_102_SYURUI		2		//	所得の種類

//////////////////////////////////////////////////////////////////
//	「源泉所得」の内訳書_コントロールの最大桁数
#define	ID_ROWNORMAL_102		14		// 通常時：１頁内の合計行を含む全体の行数
#define	ID_ROWKOBETU_102		52		// 個別時：１頁内の合計行を含む全体の行数

//////////////////////////////////////////////////////////////////
//	「源泉所得」の内訳書_金額欄の最大桁数
#define	ID_VAL_102_DATA			11		// 期末現在高nのデータ行の最大桁数
#define	ID_VAL_102_TOTAL		12		// 期末現在高の計行の最大桁数
// No.200903 add -->
#define	ID_VAL_102_K_DATA		13		// 期末現在高nのデータ行の最大桁数
#define	ID_VAL_102_K_TOTAL		13		// 期末現在高の計行の最大桁数
// No.200903 add <--


// ⑪借入金の最大入力値
//////////////////////////////////////////////////////////////////
//	入力コントロールの最大桁数
#define MAX_KETA_111_KAMOKU		12		//	科目
#define MAX_KETA_111_ADNAME1	20		//	借入先１
#define MAX_KETA_111_KANKEI		6		//	法人・代表者との関係
#define MAX_KETA_111_ADADD1		20		//	所在地１
#define MAX_KETA_111_ADADD2		20		//	所在地２
#define MAX_KETA_111_RIYUU		20		//	借入理由
#define MAX_KETA_111_NAIYOU		26		//	担保の内容
// midori 156189,156190,156191 add -->
#define MAX_KETA_111_SYOKEI		26		//	担保の内容
// midori 156189,156190,156191 add <--

//////////////////////////////////////////////////////////////////
//	「借入金」の内訳書_コントロールの最大桁数
#define	ID_ROWNORMAL_111		15		// １頁内の合計行を含む全体の行数

//////////////////////////////////////////////////////////////////
//	「借入金」の内訳書_金額欄の最大桁数
#define	ID_VAL_111_DATA			11		// 期末現在高nのデータ行の最大桁数
#define	ID_VAL_111_TOTAL		12		// 期末現在高の計行の最大桁数
#define	ID_RISOKU_111_DATA		9		// 期中の支払利息nのデータ行の最大桁数
#define	ID_RISOKU_111_TOTAL		10		// 期中の支払利息の計行の最大桁数
// No.200903 add -->
#define	ID_VAL_111_K_DATA		12		// 期末現在高nのデータ行の最大桁数
#define	ID_RISOKU_111_K_DATA	10		// 期中の支払利息nのデータ行の最大桁数
// No.200903 add <--

// ⑫土地の売上高等の最大入力値
//////////////////////////////////////////////////////////////////
//	入力コントロールの最大桁数
#define MAX_KETA_121_KUBUN		5		//	区分
#define MAX_KETA_121_AD2ADD1	15		//	商品の所在地１
#define MAX_KETA_121_AD2ADD2	15		//	商品の所在地２
#define MAX_KETA_121_HDNAME		8		//	地目
#define MAX_KETA_121_SPDATE		4		//	売上年月
#define MAX_KETA_121_ADNAME1	20		//	氏名１
#define MAX_KETA_121_ADADD1		20		//	住所１
#define MAX_KETA_121_ADADD2		20		//	住所２
#define MAX_KETA_121_SPDATE3	2		//	売上商品の取得年（年）

//////////////////////////////////////////////////////////////////
//	「土地売上」の内訳書_コントロールの最大桁数
#define	ID_ROWNORMAL_121		10		// １頁内の合計行を含む全体の行数

//////////////////////////////////////////////////////////////////
//	「土地売上」の内訳書_金額欄の最大桁数
#define	ID_VAL_121_DATA			11		// 売上金額nのデータ行の最大桁数
#define	ID_VAL_121_TOTAL		12		// 売上金額の計行の最大桁数
#define	ID_VAL2_121_DATA		11		// 売上金額nのデータ行の最大桁数
#define	ID_VAL2_121_TOTAL		12		// 売上金額の計行の最大桁数
// No.200903 add -->
#define	ID_VAL_121_K_DATA		13		// 売上金額nのデータ行の最大桁数
#define	ID_VAL_121_K_TOTAL		13		// 売上金額の計行の最大桁数
#define	ID_VAL2_121_K_DATA		13		// 売上金額nのデータ行の最大桁数
#define	ID_VAL2_121_K_TOTAL		13		// 売上金額の計行の最大桁数
// No.200903 add <--

// ⑬「売上高等の事業所別内訳書」の最大入力値
//////////////////////////////////////////////////////////////////
//	入力コントロールの最大桁数
#define MAX_KETA_131_ADNAME1	20		//	事業所の名称
#define MAX_KETA_131_ADADD1		20		//	所在地１
#define MAX_KETA_131_ADADD2		20		//	所在地２
#define MAX_KETA_131_HDNAME		16		//	責任者氏名
#define MAX_KETA_131_KANKEI		8		//	代表者との関係
#define MAX_KETA_131_NAIYOU		15		//	事業等の内容
#define MAX_KETA_131_HDNAME2	10		//	源泉所得税納付書
#define MAX_KETA_131_TEKI		24		//	摘要

//////////////////////////////////////////////////////////////////
//	「売上高」の内訳書_コントロールの最大桁数
#define	ID_ROWNORMAL_131		15		// １頁内の合計行を含む全体の行数

//////////////////////////////////////////////////////////////////
//	「売上高」の内訳書_金額欄の最大桁数
#define	ID_VAL_131_DATA			11		// 売上高nのデータ行の最大桁数
#define	ID_VAL_131_TOTAL		12		// 売上高の計行の最大桁数
#define	ID_VAL2_131_DATA		11		// 期末現在高nのデータ行の最大桁数
#define	ID_VAL2_131_TOTAL		12		// 期末現在高の計行の最大桁数
#define	ID_WORKER_131_DATA		6		// 期末従業員数のデータ行の最大桁数
#define	ID_WORKER_131_TOTAL		7		// 期末従業員数の計行の最大桁数
#define	ID_AREA_131_DATA		6		// 使用建物の延面積のデータ行の最大桁数
#define	ID_AREA_131_TOTAL		7		// 使用建物の延面積の計行の最大桁数
// No.200903 add -->
#define	ID_VAL_131_K_DATA		12		// 売上高nのデータ行の最大桁数
#define	ID_VAL2_131_K_DATA		12		// 期末現在高nのデータ行の最大桁数
// No.200903 add <--

// ⑭-1役員報酬の最大入力値
//////////////////////////////////////////////////////////////////
//	役員報酬の入力コントロールの最大桁数
#define MAX_KETA_141_NAMEYAKU	8		//	役職名
#define MAX_KETA_141_NAMETANT	8		//	担当業務
#define MAX_KETA_141_ADNAME1	24		//	名称１
#define MAX_KETA_141_KANKEI		6		//	代表者との関係
#define MAX_KETA_141_ADADD1		20		//	住所１
#define MAX_KETA_141_ADADD2		20		//	住所２

//////////////////////////////////////////////////////////////////
//	「役員報酬」の内訳書_コントロールの最大桁数
#define	ID_ROWNORMAL_141		11		// 通常時：１頁内の合計行を含む全体の行数
#define	ID_ROWKOBETU_141		15		// 個別時：１頁内の合計行を含む全体の行数

//////////////////////////////////////////////////////////////////
//	「役員報酬」の内訳書_金額欄の最大桁数
// 改良No.23-0961 del -->
//#define	ID_EMVAL_141_DATA	9	// 使用人職務分nのデータ行の最大桁数
//#define	ID_EMVAL_141_TOTAL	9	// 使用人職務分の計行の最大桁数
//#define	ID_PRVAL_141_DATA	9	// 定期同額給与nのデータ行の最大桁数
//#define	ID_PRVAL_141_TOTAL	9	// 定期同額給与の計行の最大桁数
//#define	ID_BFVAL_141_DATA	9	// 事前確定届出給与nのデータ行の最大桁数
//#define	ID_BFVAL_141_TOTAL	9	// 事前確定届出給与の計行の最大桁数
//#define	ID_PFVAL_141_DATA	9	// 利益連動給与nのデータ行の最大桁数
//#define	ID_PFVAL_141_TOTAL	9	// 利益連動給与の計行の最大桁数
//#define	ID_ETVAL_141_DATA	9	// その他nのデータ行の最大桁数
//#define	ID_ETVAL_141_TOTAL	9	// その他の計行の最大桁数
//#define	ID_RTVAL_141_DATA	9	// 退職給与nのデータ行の最大桁数
//#define	ID_RTVAL_141_TOTAL	9	// 退職給与の計行の最大桁数
//#define	ID_VAL_141_DATA		9	// 役員給与計nのデータ行の最大桁数
//#define	ID_VAL_141_TOTAL	9	// 役員給与計の計行の最大桁数
//// midori 190301 add -->
//#define	ID_VAL_141_DATA_K	10	// (新様式)役員給与計のデータ行の最大桁数
//#define	ID_VAL_141_TOTAL_K	10	// (新様式)役員給与計の計行の最大桁数
//// midori 190301 add <--
// 改良No.23-0961 del <--
// 改良No.23-0961 add -->
#define	ID_EMVAL_141_DATA		9	// 使用人職務分nのデータ行の最大桁数
#define	ID_EMVAL_141_TOTAL		9	// 使用人職務分の計行の最大桁数
#define	ID_PRVAL_141_DATA		9	// 定期同額給与nのデータ行の最大桁数
#define	ID_PRVAL_141_TOTAL		9	// 定期同額給与の計行の最大桁数
#define	ID_BFVAL_141_DATA		9	// 事前確定届出給与nのデータ行の最大桁数
#define	ID_BFVAL_141_TOTAL		9	// 事前確定届出給与の計行の最大桁数
#define	ID_PFVAL_141_DATA		9	// 利益連動給与nのデータ行の最大桁数
#define	ID_PFVAL_141_TOTAL		9	// 利益連動給与の計行の最大桁数
#define	ID_ETVAL_141_DATA		9	// その他nのデータ行の最大桁数
#define	ID_ETVAL_141_TOTAL		9	// その他の計行の最大桁数
#define	ID_RTVAL_141_DATA		9	// 退職給与nのデータ行の最大桁数
#define	ID_RTVAL_141_TOTAL		9	// 退職給与の計行の最大桁数
#define	ID_VAL_141_DATA			9	// 役員給与計nのデータ行の最大桁数
#define	ID_VAL_141_TOTAL		9	// 役員給与計の計行の最大桁数

#define	ID_EMVAL_141_DATA_K		10	// 使用人職務分nのデータ行の最大桁数
#define	ID_EMVAL_141_TOTAL_K	10	// 使用人職務分の計行の最大桁数
#define	ID_PRVAL_141_DATA_K		10	// 定期同額給与nのデータ行の最大桁数
#define	ID_PRVAL_141_TOTAL_K	10	// 定期同額給与の計行の最大桁数
#define	ID_BFVAL_141_DATA_K		10	// 事前確定届出給与nのデータ行の最大桁数
#define	ID_BFVAL_141_TOTAL_K	10	// 事前確定届出給与の計行の最大桁数
#define	ID_PFVAL_141_DATA_K		10	// 利益連動給与nのデータ行の最大桁数
#define	ID_PFVAL_141_TOTAL_K	10	// 利益連動給与の計行の最大桁数
#define	ID_ETVAL_141_DATA_K		10	// その他nのデータ行の最大桁数
#define	ID_ETVAL_141_TOTAL_K	10	// その他の計行の最大桁数
#define	ID_RTVAL_141_DATA_K		10	// 退職給与nのデータ行の最大桁数
#define	ID_RTVAL_141_TOTAL_K	10	// 退職給与の計行の最大桁数
#define	ID_VAL_141_DATA_K		11	// 役員給与計のデータ行の最大桁数
#define	ID_VAL_141_TOTAL_K		11	// 役員給与計の計行の最大桁数
// 改良No.23-0961 add <--


// ⑮-1地代家賃の最大入力値
//////////////////////////////////////////////////////////////////
//	入力コントロールの最大桁数
#define MAX_KETA_151_KUBUN		7		//	地代・家賃の区分
#define MAX_KETA_151_YOUTO		40		//	借地物件の用途
#define MAX_KETA_151_AD2ADD1	20		//	所在地１
#define MAX_KETA_151_AD2ADD2	20		//	所在地２
#define MAX_KETA_151_ADNAME1	20		//	貸主の名称１
#define MAX_KETA_151_ADADD1		20		//	貸主の所在地１
#define MAX_KETA_151_ADADD2		20		//	貸主の所在地２
#define MAX_KETA_151_SPDATE		4		//	支払対象期間１
#define MAX_KETA_151_SPDATE2	4		//	支払対象期間２
#define MAX_KETA_151_TEKI		42		//	摘要

//////////////////////////////////////////////////////////////////
//	「地代家賃」の内訳書_コントロールの最大桁数
#define	ID_ROWNORMAL_151		8		// 通常時：１頁内の合計行を含む全体の行数
#define	ID_ROWKOBETU_151		19		// 個別時：１頁内の合計行を含む全体の行数

//////////////////////////////////////////////////////////////////
//	「地代家賃」の内訳書_金額欄の最大桁数
#define	ID_VAL_151_DATA			11		// 支払貸借料nのデータ行の最大桁数
#define	ID_VAL_151_TOTAL		12		// 支払貸借料の計行の最大桁数
// No.200903 add -->
#define	ID_VAL_151_K_DATA		12		// 支払貸借料nのデータ行の最大桁数
// No.200903 add <--

// ⑮-2権利金の最大入力値
//////////////////////////////////////////////////////////////////
//	入力コントロールの最大桁数
#define MAX_KETA_152_ADNAME1	20		//	支払先の名称
#define MAX_KETA_152_ADADD1		20		//	支払先の所在地１
#define MAX_KETA_152_ADADD2		20		//	支払先の所在地２
#define MAX_KETA_152_SPDATE		6		//	支払年月日
#define MAX_KETA_152_NAIYOU		26		//	権利金等の内容
#define MAX_KETA_152_TEKI		33		//	摘要

//////////////////////////////////////////////////////////////////
//	「権利金」の内訳書_コントロールの最大桁数
#define	ID_ROWNORMAL_152		3		// 通常時：１頁内の合計行を含む全体の行数
#define	ID_ROWKOBETU_152		18		// 個別時：１頁内の合計行を含む全体の行数

//////////////////////////////////////////////////////////////////
//	「権利金」の内訳書_金額欄の最大桁数
#define	ID_VAL_152_DATA			11		// 支払金額nのデータ行の最大桁数
#define	ID_VAL_152_TOTAL		12		// 支払金額の計行の最大桁数
// No.200903 add -->
#define	ID_VAL_152_K_DATA		12		// 簡素化用　支払金額nのデータ行の最大桁数
// No.200903 add <--

// ⑮-3工業所有権の最大入力値
//////////////////////////////////////////////////////////////////
//	入力コントロールの最大桁数
#define MAX_KETA_153_HDNAME		10		//	名称
#define MAX_KETA_153_ADNAME1	20		//	支払先の名称
#define MAX_KETA_153_ADADD1		20		//	支払先の住所１
#define MAX_KETA_153_ADADD2		20		//	支払先の住所２
#define MAX_KETA_153_KYDATE		4		//	契約期間１
#define MAX_KETA_153_KYDATE2	4		//	契約期間２
#define MAX_KETA_153_SPDATE		4		//	支払対象期間１
#define MAX_KETA_153_SPDATE2	4		//	支払対象期間２
#define MAX_KETA_153_TEKI		30		//	摘要

//////////////////////////////////////////////////////////////////
//	「工業所有権」の内訳書_コントロールの最大桁数
#define	ID_ROWNORMAL_153		3		// 通常時：１頁内の合計行を含む全体の行数
#define	ID_ROWKOBETU_153		21		// 個別時：１頁内の合計行を含む全体の行数

//////////////////////////////////////////////////////////////////
//	「工業所有権」の内訳書_金額欄の最大桁数
#define	ID_VAL_153_DATA			11		// 支払金額nのデータ行の最大桁数
#define	ID_VAL_153_TOTAL		12		// 支払金額の計行の最大桁数
// No.200903 add -->
#define	ID_VAL_153_K_DATA		12		// 簡素化用　支払金額nのデータ行の最大桁数
// No.200903 add <--

// ⑯-1雑益の最大入力値
//////////////////////////////////////////////////////////////////
//	入力コントロールの最大桁数
#define MAX_KETA_161_KAMOKU		12		//	科目
#define MAX_KETA_161_NAIYOU		24		//	取引の内容
#define MAX_KETA_161_ADNAME1	10		//	相手先１
#define MAX_KETA_161_ADNAME2	10		//	相手先２
#define MAX_KETA_161_ADADD1		20		//	所在地１
#define MAX_KETA_161_ADADD2		20		//	所在地２

//////////////////////////////////////////////////////////////////
//	「雑益」の内訳書_コントロールの最大桁数
#define	ID_ROWNORMAL_161		10		// 通常時：１頁内の合計行を含む全体の行数
#define	ID_ROWKOBETU_161		20		// 個別時：１頁内の合計行を含む全体の行数

//////////////////////////////////////////////////////////////////
//	「雑益」の内訳書_金額欄の最大桁数
#define	ID_VAL_161_DATA			11		// 金額nのデータ行の最大桁数
#define	ID_VAL_161_TOTAL		12		// 金額の計行の最大桁数
// No.200903 add -->
#define	ID_VAL_161_K_DATA		12		// 金額nのデータ行の最大桁数
// No.200903 add <--

// ⑯-2雑損失の最大入力値
//////////////////////////////////////////////////////////////////
//	入力コントロールの最大桁数
#define MAX_KETA_162_KAMOKU		12		//	科目
#define MAX_KETA_162_NAIYOU		24		//	取引の内容
#define MAX_KETA_162_ADNAME1	10		//	相手先１
#define MAX_KETA_162_ADNAME2	10		//	相手先２
#define MAX_KETA_162_ADADD1		20		//	所在地１
#define MAX_KETA_162_ADADD2		20		//	所在地２

//////////////////////////////////////////////////////////////////
//	「雑損失」の内訳書_コントロールの最大桁数
#define	ID_ROWNORMAL_162		10		// 通常時：１頁内の合計行を含む全体の行数
#define	ID_ROWKOBETU_162		20		// 個別時：１頁内の合計行を含む全体の行数

//////////////////////////////////////////////////////////////////
//	「雑損失」の内訳書_金額欄の最大桁数
#define	ID_VAL_162_DATA			11		// 金額nのデータ行の最大桁数
#define	ID_VAL_162_TOTAL		12		// 金額の計行の最大桁数
// No.200903 add -->
#define	ID_VAL_162_K_DATA		12		// 金額nのデータ行の最大桁数
// No.200903 add <--

//////////////////////////////////////////////////////////////////
//	「その他科目」の内訳書①－１～５_コントロールの最大桁数
#define MAX_KETA_170_KAMOKU		8		//	科目
#define MAX_KETA_170_ADNAME1	20		//	名称１
#define MAX_KETA_170_ADADD1		20		//	住所１
#define MAX_KETA_170_TEKI		24		//	摘要
#define	ID_ROWNORMAL_170		27		// １頁内の合計行を含む全体の行数

// midori 170504 add -->
//	「その他科目」の内訳書①－１～２０_コントロールの最大桁数（インポート/エクスポート）
#define	ID_ROWNORMAL_171		27		//	通常時：１頁内の合計行を含む全体の行数
#define	ID_ROWKOBETU_171		27		//	個別時：１頁内の合計行を含む全体の行数
#define ID_ROWKEI_171			1		//	帳表固定の合計行数（1:合計あり，0:なし）
#define MAX_KETA_171_KAMOKU		12		//	科目
#define MAX_KETA_171_ADNAME1	20		//	名称１
#define MAX_KETA_171_ADADD1		20		//	住所１
#define MAX_KETA_171_ADADD2		20		//	住所２
#define MAX_KETA_171_TEKI		26		//	摘要
//////////////////////////////////////////////////////////////////
//	「その他科目の」の内訳書①－１～２０_金額欄の最大桁数
#define ID_VAL_171_DATA			11		// 金額のデータ行の最大桁数
// No.200903 add -->
#define	ID_VAL_171_K_DATA		13		// 金額のデータ行の最大桁数
// No.200903 add <--

//	「その他科目」の内訳書②－１～２０_コントロールの最大桁数（インポート/エクスポート）
#define ID_ROWNORMAL_181		27		// 通常時：１頁内の合計行を含む全体の行数
#define ID_ROWKOBETU_181		27		// 個別時：１頁内の合計行を含む全体の行数
#define ID_ROWKEI_181			1		// 帳表固定の合計行数（1:合計あり，0:なし）
#define MAX_KETA_181_KAMOKU		12		// 科目
//	「その他科目」の内訳書②－１～２０_コントロールの最大桁数
#define MAX_KETA_181_ITEM		40		//	項目
#define MAX_KETA_181_ITEMTITLE	8		//	項目タイトル
#define MAX_KETA_181_TEKI		40		//	摘要
//	「その他2」の内訳書_金額欄の最大桁数
#define	ID_VAL_181_DATA			11		// 金額nのデータ行の最大桁数
// midori 170504 add <--
// No.200903 add -->
#define	ID_VAL_181_K_DATA		13		// 金額nのデータ行の最大桁数
// No.200903 add <--

//////////////////////////////////////////////////////////////////
//	「その他科目」の内訳書②－１～５_コントロールの最大桁数
#define MAX_KETA_180_KAMOKU		8		//	科目
#define MAX_KETA_180_ADNAME1	20		//	項目
#define MAX_KETA_180_TEKI		24		//	摘要
#define	ID_ROWNORMAL_180		27		// １頁内の合計行を含む全体の行数

//////////////////////////////////////////////////////////////////
// インボイス登録番号桁数（様式共通）
#define MAX_KETA_INVOICENO		13		//	修正No.168363 add

//////////////////////////////////////////////////////////////////
//	科目名入力最大文字数
#define DR_INPUTLENGTH_NORMAL		12	//	入力文字数（通常）
#define DR_INPUTLENGTH_NORMAL_OLD	8	//	入力文字数（通常・従来の文字列長）
#define DR_INPUTLENGTH_INVOICE		6	//  インボイス登録番号							// インボイス登録番号追加対応_23/10/20 add
#define DR_INPUTLENGTH_GENSEN		2	//	入力文字数（⑩-2.源泉所得税預り金の内訳）
#define DR_INPUTLENGTH_TOTIURI		5	//	入力文字数（⑫.土地の売上高等の内訳書）
#define DR_INPUTLENGTH_TIDAI		7	//	入力文字数（⑮-1.地代家賃の内訳書）

#endif	//	__UCINPMAXCOMMON_H__