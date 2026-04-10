//**************************************************************************************************
//
// FileName		:  prtConstData.h
//
// ■印刷関連 定数定義
//
//**************************************************************************************************

#ifndef	__UCHIWAKE__PRTCONSTDATA_H__
#define	__UCHIWAKE__PRTCONSTDATA_H__

using namespace std;

//**************************************************************************************************
// 定数群
//**************************************************************************************************

//	様式最大数
#define	PRT_ID_FORMNO_MAX		(UC_ID_FORMNO_MAX + 2)			// 帳表数＋表紙（1）＋共通情報（1）
	// 印刷時には、表紙もカウントされるため、様式最大数＋1とした。
	// また、「配列のインデックス」＝「帳表の印刷順」 とするため、さらに＋1とした。
	// ※インデックス0は、印刷時の共通データ格納用として使用する

// 最大項目数
#define	PRT_MAX_COLUMN			(Field151Max + Field152Max + Field153Max)
#define PRT_MAX_COLUMN_I		(Field151IMax + Field152IMax + Field153IMax) // インボイス登録番号追加対応_23/11/02 add
										// データボックスの最大列数
#define	PRT_MAX_ROW				40		// データボックスの最大行数
	// PBUF.PITM[40]のサイズに合わせた
#define	PRT_MAX_OMIT			7		// 1帳表に対する省略文字の最大項目数
	// 「配列のインデックス」＝「項目数」 とするため、最大項目数＋1とした

// 行数
	// 以下で定義している行数は、1帳表の中で、行が2列にまたがって存在する場合の1列分の行数
#define PRT_ROW_092				2		// 帳表⑨（下欄）
#define PRT_ROW_102_KOBETU		26		// 帳表⑩-2「個別」
#define PRT_ROW_102_NORMAL		7		// 帳表⑩-2「通常」
	// 以下で定義している行数は、「小計･中計」の文言と、
	// 最終行の「計」の文言の印字位置（セル内配置）が異なる帳表の行数
#define PRT_ROW_042_KOBETU		19		// 帳表④-2「個別」
#define PRT_ROW_042_NORMAL		8		// 帳表④-2「通常」
#define PRT_ROW_111				15		// 帳表⑪
#define PRT_ROW_131				15		// 帳表⑬

// 計行に表示される項目数
#define NUM_KEI_FIELD042		2
#define NUM_KEI_FIELD061		2
#define NUM_KEI_FIELD071		3
#define NUM_KEI_FIELD111		2
#define NUM_KEI_FIELD121		2
#define NUM_KEI_FIELD131		4
#define NUM_KEI_FIELD141		7

// 名称
#define	PRT_NAME_JOB			_T( "勘定科目内訳書" )			// ドキュメント名

// 出力スケジュールファイル
#define	PRT_FILE_SCH			_T( "DBUchiwakeList.sch" )
#define	PRT_FILE_SCH_K			_T( "DBUchiwakeListK.sch" )		// midori 190301 add
#define	PRT_FILE_SCH_I			_T( "DBUchiwakeListI.sch" )		// インボイス登録番号追加対応_23/10/20 add

// イメージファイル		[2009/07/21 EMF対応 (TIF->IMG, .tif->.emf)]
#define	PRT_FILE_IMG_011		_T( "uc_011.emf" )
#define	PRT_FILE_IMG_021		_T( "uc_021.emf" )
#define	PRT_FILE_IMG_031		_T( "uc_031.emf" )
#define	PRT_FILE_IMG_040		_T( "uc_040.emf" )
#define	PRT_FILE_IMG_041		_T( "uc_041.emf" )
#define	PRT_FILE_IMG_042		_T( "uc_042.emf" )
#define	PRT_FILE_IMG_051		_T( "uc_051.emf" )
#define	PRT_FILE_IMG_061		_T( "uc_061.emf" )
#define	PRT_FILE_IMG_071		_T( "uc_071.emf" )
#define	PRT_FILE_IMG_081		_T( "uc_081.emf" )
#define	PRT_FILE_IMG_091		_T( "uc_091.emf" )
#define	PRT_FILE_IMG_100		_T( "uc_100.emf" )
#define	PRT_FILE_IMG_101		_T( "uc_101.emf" )
#define	PRT_FILE_IMG_102		_T( "uc_102.emf" )
#define	PRT_FILE_IMG_111		_T( "uc_111.emf" )
#define	PRT_FILE_IMG_121		_T( "uc_121.emf" )
#define	PRT_FILE_IMG_131		_T( "uc_131.emf" )
#define	PRT_FILE_IMG_140		_T( "uc_140.emf" )
#define	PRT_FILE_IMG_141		_T( "uc_141.emf" )
#define	PRT_FILE_IMG_142		_T( "uc_142.emf" )
#define	PRT_FILE_IMG_150		_T( "uc_150.emf" )
#define	PRT_FILE_IMG_151		_T( "uc_151.emf" )
#define	PRT_FILE_IMG_152		_T( "uc_152.emf" )
#define	PRT_FILE_IMG_153		_T( "uc_153.emf" )
#define	PRT_FILE_IMG_160		_T( "uc_160.emf" )
#define	PRT_FILE_IMG_161		_T( "uc_161.emf" )
#define	PRT_FILE_IMG_171		_T( "uc_171.emf" )
#define	PRT_FILE_IMG_181		_T( "uc_181.emf" )
#define	PRT_FILE_IMG_CVR		_T( "uc_cover.emf" )
//#define	PRT_FILE_IMG_CVR_S		_T( "uc_cover_s.emf" )
// midori 190301 add -->
#define	PRT_FILE_IMG_011_K		_T( "uc_011_3104.emf" )
#define	PRT_FILE_IMG_021_K		_T( "uc_021_3104.emf" )
#define	PRT_FILE_IMG_031_K		_T( "uc_031_3104.emf" )
#define	PRT_FILE_IMG_040_K		_T( "uc_040_3104.emf" )
#define	PRT_FILE_IMG_041_K		_T( "uc_041_3104.emf" )
#define	PRT_FILE_IMG_042_K		_T( "uc_042_3104.emf" )
#define	PRT_FILE_IMG_051_K		_T( "uc_051_3104.emf" )
#define	PRT_FILE_IMG_061_K		_T( "uc_061_3104.emf" )
#define	PRT_FILE_IMG_071_K		_T( "uc_071_3104.emf" )
#define	PRT_FILE_IMG_081_K		_T( "uc_081_3104.emf" )
#define	PRT_FILE_IMG_091_K		_T( "uc_091_3104.emf" )
#define	PRT_FILE_IMG_100_K		_T( "uc_100_3104.emf" )
#define	PRT_FILE_IMG_101_K		_T( "uc_101_3104.emf" )
#define	PRT_FILE_IMG_102_K		_T( "uc_102_3104.emf" )
#define	PRT_FILE_IMG_111_K		_T( "uc_111_3104.emf" )
#define	PRT_FILE_IMG_121_K		_T( "uc_121_3104.emf" )
#define	PRT_FILE_IMG_131_K		_T( "uc_131_3104.emf" )
#define	PRT_FILE_IMG_140_K		_T( "uc_140_3104.emf" )
#define	PRT_FILE_IMG_141_K		_T( "uc_141_3104.emf" )
#define	PRT_FILE_IMG_142_K		_T( "uc_142_3104.emf" )
#define	PRT_FILE_IMG_150_K		_T( "uc_150_3104.emf" )
#define	PRT_FILE_IMG_151_K		_T( "uc_151_3104.emf" )
#define	PRT_FILE_IMG_152_K		_T( "uc_152_3104.emf" )
#define	PRT_FILE_IMG_153_K		_T( "uc_153_3104.emf" )
#define	PRT_FILE_IMG_160_K		_T( "uc_160_3104.emf" )
#define	PRT_FILE_IMG_161_K		_T( "uc_161_3104.emf" )
#define	PRT_FILE_IMG_162_K		_T( "uc_162_3104.emf" )
// midori 190301 add <--

// インボイス登録番号追加対応_23/10/27 add -->
#define PRT_FILE_IMG_021_I		_T("uc_021_202404.emf")
#define PRT_FILE_IMG_031_I		_T("uc_031_202404.emf")
#define PRT_FILE_IMG_040_I		_T("uc_040_202404.emf")
#define PRT_FILE_IMG_041_I		_T("uc_041_202404.emf")
#define PRT_FILE_IMG_042_I		_T("uc_042_202404.emf")
#define PRT_FILE_IMG_071_I		_T("uc_071_202404.emf")
#define PRT_FILE_IMG_081_I		_T("uc_081_202404.emf")
#define PRT_FILE_IMG_091_I		_T("uc_091_202404.emf")
#define PRT_FILE_IMG_100_I		_T("uc_100_202404.emf")
#define PRT_FILE_IMG_101_I		_T("uc_101_202404.emf")
#define PRT_FILE_IMG_102_I		_T("uc_102_202404.emf")
#define PRT_FILE_IMG_121_I		_T("uc_121_202404.emf")
#define PRT_FILE_IMG_150_I		_T("uc_150_202404.emf")
#define PRT_FILE_IMG_151_I		_T("uc_151_202404.emf")
#define PRT_FILE_IMG_152_I		_T("uc_152_202404.emf")
#define PRT_FILE_IMG_153_I		_T("uc_153_202404.emf")
#define PRT_FILE_IMG_160_I		_T("uc_160_202404.emf")
#define PRT_FILE_IMG_161_I		_T("uc_161_202404.emf")
#define PRT_FILE_IMG_162_I		_T("uc_162_202404.emf")
#define PRT_FILE_IMG_171_I		_T("uc_171_202404.emf")
// インボイス登録番号追加対応_23/10/27 add <--

// 文字列補正
#define	PRT_FORMAT_GENZAI		_T( " 現在" )
#define	PRT_FORMAT_SLASH		_T( "／" )
#define	PRT_FORMAT_CHECK		_T( "○" )
#define	PRT_FORMAT_WAVE			_T( "～" )
#define	PRT_FORMAT_CHCODE		_T( "\r\n" )
	// ICSDiagの1セル内で、2行以上入力できる場合、その改行コードが上記コードとなっている。

#define	PRT_FORMAT_PAGE			_T( "%d/%d" )					// ページ番号
#define	PRT_FORMAT_DATE_A1		_T( "%2d.%2d" )
// midori 200101 add -->
#define	PRT_FORMAT_DATE_A2		_T( "%2d" )						// 年
// midori 200101 add <--
#define	PRT_FORMAT_DATE_B1		_T( "%2d.%2d.%2d" )
//#define	PRT_FORMAT_DATE_C1		_T( "%2d     %2d     %2d" )		// 表紙
#define	PRT_FORMAT_DATE_C1		_T( "%2d    %2d    %2d" )		// 表紙
#define	PRT_FORMAT_DATE_C2		_T( "%2d      %2d     %2d" )	// 帳表⑤-1（下欄）
#define	PRT_FORMAT_DATE_D1		_T( "%s%2d" )					// 帳表⑫（取得年）
#define	PRT_FORMAT_DATE_E1		_T( "%s%2d.%2d" )				// 帳表⑮（契約期間）
// midori 200101 add -->
#define	PRT_FORMAT_DATE_E2		_T( "%s%2d.%2d.%2d" )			// 帳表（元号和暦年.月.日）
// midori 200101 add <--
// midori 181101 del -->
//#define	PRT_FORMAT_DATE_F1		_T( "平成%2d年%2d月%2d日" )		// 決算期間（和暦）
// midori 181101 del <--
#define	PRT_FORMAT_DATE_F2		_T( "%4d年%2d月%2d日" )			// 決算期間（西暦）
// midori 181101 del -->
//#define	PRT_FORMAT_DATE_F3		_T( "平成  年  月  日" )		// 決算期間空白（和暦）
// midori 181101 del <--
#define	PRT_FORMAT_DATE_F4		_T( "    年  月  日" )			// 決算期間空白（西暦）
#define	PRT_FORMAT_DATE_G1		_T( "%s" )						// 帳表⑦
#define	PRT_FORMAT_DATE_H1		_T( "第　%d　期" )				// 表紙・期
#define	PRT_FORMAT_DATE_H2		_T( "第　  　期" )				// 表紙・期(第0期の場合、0を出力させない)
#define	PRT_FORMAT_TITLE_TAIL	_T( "の内訳書" )
#define	PRT_FORMAT_TITLE_021	_T( "受取手形（%s）" )
#define	PRT_FORMAT_TITLE_051	_T( "棚卸資産（%s）" )
#define	PRT_FORMAT_TITLE_111	_T( "%s及び支払利子" )
#define	PRT_FORMAT_TITLE_161	_T( "雑益等（%s）" )
#define	PRT_FORMAT_TITLE_162	_T( "雑損失等（%s）" )

// エラーメッセージ
#define	PRT_ERR_PRGEND			_T( "出力処理エラーのため、プログラムを終了します。" )
#define	PRT_ERR_SCH_0			_T( "エラー" )
#define	PRT_ERR_SCH_1			_T( "出力スケジュール ファイルがオープン出来ません。" )
#define	PRT_ERR_SCH_2			_T( "出力スケジュール ファイルの読み込みができません。" )
#define	PRT_ERR_SCH_3			_T( "イメージ ファイルが検索出来ません。" )
#define	PRT_ERR_SCH_4			_T( "メモリ領域が十分ではありません" )
#define	PRT_ERR_SCH_5			_T( "出力スケジュール ファイルの記述に不都合があります。" )
#define	PRT_ERR_SCH_6			_T( "フォントの作成に失敗しました。" )
#define	PRT_ERR_SCH_7			_T( "座標モードの設定に失敗しました。" )
// 自前エラー
#define	PRT_ERR_FILE_SCH		_T( "出力スケジュール ファイルを検索出来ませんでした。" )
#define	PRT_ERR_FILE_IMG		_T( "イメージ ファイルを検索出来ませんでした。" )		// [2009/07/21 EMF対応 (TIF->IMG)]
#define	PRT_ERR_NOTJOB			_T( "印刷処理を実行できません！" )

// 印字位置調整
#define	PRT_UCHIWAKE_PSECNAME	_T( "DBUchiwake" )			// プログラム・セクション名
#define	PRT_UCHIWAKE_LISTNAME	_T( "勘定科目内訳書" )		// 帳票名（内訳書は全帳表共通の設定とする）

// midori 181101 add -->
#define GENGO_OFFSET      4
#define GENGO_HEISEI      4
#define GENGO_NEXT_HEISEI 5
// midori 181101 add <--

//**************************************************************************************************
// 列挙型群
//**************************************************************************************************

// 印刷処理
typedef enum tagPrtEnumSelectJob{
	JobNop = 0,				// 選択なし
	JobOne,					// 1帳表
	JobAll					// 一括
} PRT_ENUM_SELECT_JOB;

// スケジュールシーケンス番号
	// ここで指定していない帳表については、様式シーケンス番号と同じ値を用いる
// midori 190301 del -->
typedef enum tagPrtEnumSchSeq{
	SchSeq160Cmn = 21,		// 帳表⑯ （共通）
	SchSeq170Cmn,			// 帳表⑰ （共通）
	SchSeq180Cmn,			// 帳表⑱ （共通）
	SchSeq040Nml = 51,		// 帳表④ 「通常」
	SchSeq100Nml,			// 帳表⑩ 「通常」
	SchSeq140Nml,			// 帳表⑭ 「通常」
	SchSeq150Nml,			// 帳表⑮ 「通常」
	SchSeq160Nml,			// 帳表⑯ 「通常」
	SchSeqCvr = 90,			// 表紙
// midori 190301 del -->
	//SchSeqCmn = 99			// 共通	←必ず帳表リストに入れること！
// midori 190301 del <--
// midori 190301 add -->
	SchSeqCmn = 99,			// 共通	←必ず帳表リストに入れること！
	SchSeq161Cmn = 122		// 帳表⑯-2 （共通）
// midori 190301 add <--
} PRT_ENUM_SCH_SEQ;
// midori 190301 del <--

// 様式番号の枝番
typedef enum tagPrtEnumFormTag{
	TagFirst = 1,			// ○-1
	TagSecond,				// ○-2
	TagThird				// ○-3
} PRT_ENUM_FORM_TAG;


//////////////////////////////////////////////////
// 帳表ごとの印字フィールドを番号付け

// ①．預貯金等
typedef enum tagPrtEnumField011{
	Field011BkName1 = 0,	// 金融機関 銀行名
	Field011BkName2,		// 金融機関 支店名
	Field011BkName1_1Slash,	// 金融機関 銀行名（一段表示用）1行目
	Field011BkName1_2Slash,	// 金融機関 銀行名（一段表示用）2行目
	Field011BkName2_1Slash,	// 金融機関 支店名（一段表示用）1行目
	Field011BkName2_2Slash,	// 金融機関 支店名（一段表示用）2行目
	Field011BkName1Slash,	// 金融機関 銀行名（一段表示用）5文字以下
	Field011BkName2Slash,	// 金融機関 支店名（一段表示用）5文字以下
	Field011KnName1,			// 種類（9文字以上）
	Field011KnName2,			// 種類
	Field011AcNum,			// 口座番号
	Field011Val,			// 期末現在高
	Field011Teki1,			// 摘要
	Field011Teki2,			// 
	// （計）
	Field011KeiStr,			// 
	// （省略文字）
	Field011BkNameOm,		// 金融機関名
	Field011KnNameOm,		// 種類
	Field011Slash,			// "/"
// midori 190301 add -->
	Field011Teki3,			// (簡素化用)摘要３行目
	Field011KeiStr2,		// 最終ページの頁計
	Field011StNameOm,		// 金融機関名
// midori 190301 add <--
	// 最大数
	Field011Max				// 
} PRT_ENUM_FIELD_011;

// ②．受取手形
typedef enum tagPrtEnumField021{
	Field021KnName = 0,		// 科目（振出人上段を使用）
	Field021KnName1,		// 科目（振出人上段を使用）（9文字以上）
	Field021KnName2,		// 科目（振出人上段を使用）（9文字以上）
	Field021AdName1,		// 振出人（名称：上段）
	Field021AdName2,		// 振出人（名称：下段）
	Field021SpDate,			// 振出年月日
	Field021PmDate,			// 支払期日
	Field021BkName1,		// 支払銀行名（銀行名）
	Field021BkName2,		// 支払銀行名（支店名）
	Field021BkName1_1Slash,	// 支払銀行名（銀行名）（一段表示用）1行目
	Field021BkName1_2Slash,	// 支払銀行名（銀行名）（一段表示用）2行目
	Field021BkName2_1Slash,	// 支払銀行名（支店名）（一段表示用）1行目
	Field021BkName2_2Slash,	// 支払銀行名（支店名）（一段表示用）2行目
	Field021BkName1Slash,	// 支払銀行名（銀行名）（一段表示用）5文字以下
	Field021BkName2Slash,	// 支払銀行名（支店名）（一段表示用）5文字以下
	Field021Val,			// 金額
	Field021Bk2Name1,		// 割引銀行名等（銀行名）
	Field021Bk2Name2,		// 割引銀行名等（支店名）
	Field021Bk2Name1_1Slash,// 割引銀行名等（銀行名）（一段表示用）1行目
	Field021Bk2Name1_2Slash,// 割引銀行名等（銀行名）（一段表示用）2行目
	Field021Bk2Name2_1Slash,// 割引銀行名等（支店名）（一段表示用）1行目
	Field021Bk2Name2_2Slash,// 割引銀行名等（支店名）（一段表示用）2行目
	Field021Bk2Name1Slash,	// 割引銀行名等（銀行名）（一段表示用）5文字以下
	Field021Bk2Name2Slash,	// 割引銀行名等（銀行名）（一段表示用）5文字以下
	Field021Teki1,			// 摘要
	Field021Teki2,			// 
	// （計）
	Field021KeiStr,			// 
// midori 155826 del -->
//// midori 150917 add -->
//	Field021KeiStr1,		// 10文字を超えたときの "計" 上段
//	Field021KeiStr2,		// 10文字を超えたときの "計" 下段
//// midori 150917 add <--
//// midori 191002 add-- >
//	Field021KeiStr3,		// 
//	Field021KeiStr4,		// 
//	Field021KeiStr5,		// 
//// midori 155541,155542 add -->
//	Field021KeiStr6,		// 
//// midori 155541,155542 add <--
//// midori 191002 add < --
//	// （省略文字）
//	Field021AdNameOm,		// 振出人
//	Field021BkNameOm,		// 支払銀行名
//	Field021Bk2NameOm,		// 割引銀行名
//	Field021Slash,			// 支払銀行名"/"
//	Field021Slash2,			// 割引銀行名"/"
//// midori 190301 add -->
//	Field021Teki3,			// （簡素化用）摘要3行目
//	Field021StNameOm,		// 支払銀行名
//// midori 190301 add <--
// midori 155826 del <--
// midori 155826 add -->
	Field021KeiStr1,		// 10文字を超えたときの "計" 上段
	Field021KeiStr2,		// 10文字を超えたときの "計" 下段
	// （省略文字）
	Field021AdNameOm,		// 振出人
	Field021BkNameOm,		// 支払銀行名
	Field021Bk2NameOm,		// 割引銀行名
	Field021Slash,			// 支払銀行名"/"
	Field021Slash2,			// 割引銀行名"/"

	Field021Teki3,			// （簡素化用）摘要3行目
	Field021StNameOm,		// 支払銀行名

	Field021KeiStr3,		// 
	Field021KeiStr4,		// 
	Field021KeiStr5,		// 
	Field021KeiStr6,		// 
// midori 155826 add <--
// midori 155870 add -->
	Field021SpDate2,		// 振出年月日（元号XX年XX月XX日）
	Field021PmDate2,		// 支払期日（元号XX年XX月XX日）
// midori 155870 add <--
// midori 156189 add -->
	Field021KeiStr7,		// 
	Field021KeiStr8,		// 
	Field021KeiStr9,		// 
	Field021KeiStr10,		// 
// midori 156189 add <--
	// 最大数
	Field021Max				// 
} PRT_ENUM_FIELD_021;

// インボイス登録番号追加対応_23/10/23 add -->
// ②．受取手形（インボイス登録番号追加版）
typedef enum tagPrtEnumField021I {
	Field021IInvoNo = 0,		// インボイス登録番号
	Field021IKnName,			// 科目（振出人上段を使用）
	Field021IKnName1,			// 科目（振出人上段を使用）（9文字以上）
	Field021IKnName2,			// 科目（振出人上段を使用）（9文字以上）
	Field021IAdName1,			// 振出人（名称：上段）
	Field021IAdName2,			// 振出人（名称：下段）
	Field021ISpDate,			// 振出年月日
	Field021IPmDate,			// 支払期日
	Field021IBkName1,			// 支払銀行名（銀行名）
	Field021IBkName2,			// 支払銀行名（支店名）
	Field021IBkName1_1Slash,	// 支払銀行名（銀行名）（一段表示用）1行目
	Field021IBkName1_2Slash,	// 支払銀行名（銀行名）（一段表示用）2行目
	Field021IBkName2_1Slash,	// 支払銀行名（支店名）（一段表示用）1行目
	Field021IBkName2_2Slash,	// 支払銀行名（支店名）（一段表示用）2行目
	Field021IBkName1Slash,		// 支払銀行名（銀行名）（一段表示用）5文字以下
	Field021IBkName2Slash,		// 支払銀行名（支店名）（一段表示用）5文字以下
	Field021IVal,				// 金額
	Field021IBk2Name1,			// 割引銀行名等（銀行名）
	Field021IBk2Name2,			// 割引銀行名等（支店名）
	Field021IBk2Name1_1Slash,	// 割引銀行名等（銀行名）（一段表示用）1行目
	Field021IBk2Name1_2Slash,	// 割引銀行名等（銀行名）（一段表示用）2行目
	Field021IBk2Name2_1Slash,	// 割引銀行名等（支店名）（一段表示用）1行目
	Field021IBk2Name2_2Slash,	// 割引銀行名等（支店名）（一段表示用）2行目
	Field021IBk2Name1Slash,		// 割引銀行名等（銀行名）（一段表示用）5文字以下
	Field021IBk2Name2Slash,		// 割引銀行名等（銀行名）（一段表示用）5文字以下
	Field021ITeki1,				// 摘要
	Field021ITeki2,				// 
	// （計）
	Field021IKeiStr,			// 
	Field021IKeiStr1,			// 10文字を超えたときの "計" 上段
	Field021IKeiStr2,			// 10文字を超えたときの "計" 下段
	// （省略文字）
	Field021IAdNameOm,			// 振出人
	Field021IBkNameOm,			// 支払銀行名
	Field021IBk2NameOm,			// 割引銀行名
	Field021ISlash,				// 支払銀行名"/"
	Field021ISlash2,			// 割引銀行名"/"

	Field021ITeki3,				// （簡素化用）摘要3行目
	Field021IStNameOm,			// 支払銀行名

	Field021IKeiStr3,			// 
	Field021IKeiStr4,			// 
	Field021IKeiStr5,			// 
	Field021IKeiStr6,			// 
	Field021ISpDate2,			// 振出年月日（元号XX年XX月XX日）
	Field021IPmDate2,			// 支払期日（元号XX年XX月XX日）
	Field021IKeiStr7,			// 
	Field021IKeiStr8,			// 
	Field021IKeiStr9,			// 
	Field021IKeiStr10,			// 
	Field021IKeiStr11,			// 累計、頁計			// 修正No.168370 add
	// 最大数
	Field021IMax				// 
} PRT_ENUM_FIELD_021I;
// インボイス登録番号追加対応_23/10/23 add <--

// ③．売掛金
typedef enum tagPrtEnumField031{
	Field031KnName1 = 0,	// 科目（名称）（9文字以上）
	Field031KnName2,		// 科目（名称）
	Field031AdName1,		// 相手先（名称：上段）
	Field031AdName2,		// 相手先（名称：下段）
	Field031AdAdd1,			// 相手先（所在地：上段）
	Field031AdAdd2,			// 相手先（所在地：下段）
	Field031Val,			// 期末現在高
	Field031Teki1,			// 摘要
	Field031Teki2,			// 
	// （計）
	Field031KeiStr,			// 
	// （省略文字）
	Field031KnNameOm,		// 科目
	Field031AdNameOm,		// 相手先（名称）　
	Field031AdAddOm,		// 相手先（所在地）　
	// 最大数
	Field031Max				// 
} PRT_ENUM_FIELD_031;

// インボイス登録番号追加対応_23/10/23 add -->
// ③．売掛金（インボイス登録番号追加版）
typedef enum tagPrtEnumField031I{
	Field031IKnName1 = 0,	// 科目（名称）（9文字以上）
	Field031IKnName2,		// 科目（名称）
	Field031IInvoNo,		// インボイス登録番号
	Field031IAdName1,		// 相手先（名称：上段）
	Field031IAdName2,		// 相手先（名称：下段）
	Field031IAdAdd1,		// 相手先（所在地：上段）
	Field031IAdAdd2,		// 相手先（所在地：下段）
	Field031IVal,			// 期末現在高
	Field031ITeki1,			// 摘要
	Field031ITeki2,			// 
	// （計）
	Field031IKeiStr,		// 
	// （省略文字）
	Field031IKnNameOm,		// 科目
	Field031IAdNameOm,		// 相手先（名称）　
	Field031IAdAddOm,		// 相手先（所在地）　
	// 最大数
	Field031IMax			// 
} PRT_ENUM_FIELD_031I;
// インボイス登録番号追加対応_23/10/23 add <--

// ④-1．仮払金
typedef enum tagPrtEnumField041{
	Field041KnName1 = 0,	// 科目（名称）（9文字以上）
	Field041KnName2,		// 科目（名称）
	Field041AdName1,		// 相手先（名称１）
	Field041AdName2,		// 取引先（名称２）
	Field041AdAdd1,			// 相手先（所在地：上段）
	Field041AdAdd2,			// 相手先（所在地：下段）
	Field041Kankei,			// 法人・代表者との関係
	Field041Val,			// 期末現在高
	Field041Naiyou1,		// 取引の内容
	Field041Naiyou2,		// 
	// （計）
	Field041KeiStr,			// 
	// （省略文字）
	Field041KnNameOm,		// 科目
	Field041AdNameOm,		// 相手先（名称）　
	Field041AdAddOm,		// 相手先（所在地）　
	Field041KankeiOm,		// 相手先（関係）
	// 最大数
	Field041Max				// 
} PRT_ENUM_FIELD_041;

// インボイス登録番号追加対応_23/10/23 add -->
// ④-1．仮払金（インボイス登録番号追加版）
typedef enum tagPrtEnumField041I {
	Field041IKnName1 = 0,	// 科目（名称）（9文字以上）
	Field041IKnName2,		// 科目（名称）
	Field041IInvoNo,		// インボイス登録番号
	Field041IAdName1,		// 相手先（名称１）
	Field041IAdName2,		// 取引先（名称２）
	Field041IAdAdd1,		// 相手先（所在地：上段）
	Field041IAdAdd2,		// 相手先（所在地：下段）
	Field041IKankei,		// 法人・代表者との関係
	Field041IVal,			// 期末現在高
	Field041INaiyou1,		// 取引の内容
	Field041INaiyou2,		// 
	// （計）
	Field041IKeiStr,		// 
	// （省略文字）
	Field041IKnNameOm,		// 科目
	Field041IAdNameOm,		// 相手先（名称）　
	Field041IAdAddOm,		// 相手先（所在地）　
	Field041IKankeiOm,		// 相手先（関係）
	// 最大数
	Field041IMax				// 
} PRT_ENUM_FIELD_041I;
// インボイス登録番号追加対応_23/10/23 add <--

// ④-2．貸付金及び受取利息
typedef enum tagPrtEnumField042{
	Field042AdName1 = 0,	// 貸付先（名称：上段）
	Field042AdName2,		// 貸付先（名称：下段）
	Field042AdAdd1,			// 貸付先（所在地：上段）
	Field042AdAdd2,			// 貸付先（所在地：下段）
	Field042Kankei,			// 貸付先（代表者との関係）
	Field042Val,			// 期末現在高
	Field042Risoku,			// 期中の受取利息額
	Field042Rate,			// 利率
	Field042Riyuu1,			// 貸付理由
	Field042Riyuu2,			// 
	Field042Naiyou1,		// 担保の内容
	Field042Naiyou2,		// 
	// （計）
	Field042KeiStr,			// 
	Field042DnKeiStr,		// 最下行の計の文言
	// （省略文字）
	Field042AdNameOm,		// 貸付先
	Field042AdAddOm,		// 貸付先（所在地）
	Field042KankeiOm,		// 貸付先（関係）
// midori 190301 add -->
	Field042Kankei2,		// 貸付先（代表者との関係）※簡素化用
	Field042AdAdd1J,		// 貸付先（所在地：上段の上段）
	Field042AdAdd2J,		// 貸付先（所在地：下段の上段）
// midori 190301 add <--
	// 最大数
	Field042Max				// 
} PRT_ENUM_FIELD_;

// インボイス登録番号追加対応_23/10/23 add -->
// ④-2．貸付金及び受取利息（インボイス登録番号追加版）
typedef enum tagPrtEnumField042I {
	Field042IInvoNo = 0,	// インボイス登録番号
	Field042IAdName1,		// 貸付先（名称：上段）
	Field042IAdName2,		// 貸付先（名称：下段）
	Field042IAdAdd1,		// 貸付先（所在地：上段）
	Field042IAdAdd2,		// 貸付先（所在地：下段）
	Field042IKankei,		// 貸付先（代表者との関係）
	Field042IVal,			// 期末現在高
	Field042IRisoku,		// 期中の受取利息額
	Field042IRate,			// 利率
	Field042IRiyuu1,		// 貸付理由
	Field042IRiyuu2,		// 
	Field042INaiyou1,		// 担保の内容
	Field042INaiyou2,		// 
	// （計）
	Field042IKeiStr,		// 
	Field042IDnKeiStr,		// 最下行の計の文言
	// （省略文字）
	Field042IAdNameOm,		// 貸付先
	Field042IAdAddOm,		// 貸付先（所在地）
	Field042IKankeiOm,		// 貸付先（関係）
	Field042IKankei2,		// 貸付先（代表者との関係）※簡素化用
	Field042IAdAdd1J,		// 貸付先（所在地：上段の上段）
	Field042IAdAdd2J,		// 貸付先（所在地：下段の上段）
	// 最大数
	Field042IMax			// 
} PRT_ENUM_FIELD_I;
// インボイス登録番号追加対応_23/10/23 add <--

// ⑤．棚卸資産
typedef enum tagPrtEnumField051{
	Field051KnName1 = 0,	// 科目（名称）（9文字以上）
	Field051KnName2,		// 科目（名称）
	Field051HdName1,		// 品目
	Field051HdName2,		// 
	Field051Suuryou,		// 数量
	Field051Tanni,			// 単位
	Field051Price,			// 単価
	Field051Val,			// 期末現在高
	Field051Teki1,			// 摘要
	Field051Teki2,			// 
	// （下欄）
	Field052Method1,		// 期末棚卸の方法
	Field052Method2,		// 
	Field052Method3,		// 
	Field052HdName,			// 登録名称
	Field052SpDate,			// 棚卸を行った時期
	// （計）
	Field051KeiStr,			// 
	// （省略文字）
	Field051KnNameOm,		// 科目
	Field051HdNameOm,		// 品目
	// 最大数
	Field051Max				// 
} PRT_ENUM_FIELD_051;

// ⑥．有価証券
typedef enum tagPrtEnumField061{
//	Field061KubunName = 0,	// 区分の文言
	Field061KnName1 = 0,	// 区分の文言（9文字以上）
	Field061KnName2,		// 区分の文言
	Field061Syurui1,		// 種類・銘柄
	Field061Syurui2,		// 
	Field061Suuryou,		// 期末現在高（数量）
	Field061Tanni,			// 期末現在高（単位）
	Field061Val,			// 期末現在高（金額上段）
	Field061Val2,			// 期末現在高（金額下段）
	Field061SpDate,			// 期中増の明細（異動年月日）
	Field061Riyuu1,			// 期中増の明細（異動事由）
	Field061Riyuu2,			// 
	Field061Suuryou2,		// 期中増の明細（数量）
	Field061Tanni2,			// 期中増の明細（単位）
	Field061Val3,			// 期中増の明細（金額）
	Field061AdName1,		// 売却先（名称：上段）
	Field061AdName2,		// 売却先（名称：下段）
	Field061AdAdd1,			// 売却先（所在地：上段）
	Field061AdAdd2,			// 売却先（所在地：下段）
	Field061Teki1,			// 摘要
	Field061Teki2,			// 
	Field061Teki3,			// 
	// （計）
	Field061KeiStr,			// 
	// （省略文字）
//	Field061KubunNameOm,	// 区分
	Field061KnNameOm,		// 区分
	Field061SyuruiOm,		// 種類
	Field061RiyuuOm,		// 異動事由
	Field061AdNameOm,		// 売却先(名称）
	Field061AdAddOm,		// 売却先（所在地）
// midori 190301 add -->
	Field061KeiStr2,		// 小計、中計
	Field061KeiStr3,		// 最終ページの頁計
	Field061MeigaraOm,		// 銘柄
// midori 190301 add <--
	// 最大数
	Field061Max				// 
} PRT_ENUM_FIELD_061;

// ⑦．固定資産
typedef enum tagPrtEnumField071{
	Field071Syurui1 = 0,		// 種類・構造
	Field071Syurui2,			// 
	Field071Youto1,			// 用途
	Field071Youto2,			// 
	Field071Area,			// 面積
	Field071Ad2Add1,		// 物件の所在地
	Field071Ad2Add2,		// 
	Field071Val,			// 期末現在高
	Field071SpDate,			// 異動年月日
	Field071Riyuu1,			// 異動事由
	Field071Riyuu2,			// 
	Field071MsVal,			// 取得価額
	Field071MsVal2,			// 異動直前の帳簿価額
	Field071AdName1,		// 売却先（名称：上段）
	Field071AdName2,		// 売却先（名称：下段）
	Field071AdAdd1,			// 売却先（所在地：上段）
	Field071AdAdd2,			// 売却先（所在地：下段）
	Field071SpDate21,		// 売却物件の取得年月
	Field071SpDate22,		// 
	// （計）
	Field071KeiStr,			// 
	// （省略文字）
	Field071SyuruiOm,			// 種類・構造
	Field071Ad2AddOm,		// 物件の所在地
	Field071Riyuu1Om,		// 異動事由
	Field071AdNameOm,		// 売却先(名称）
	Field071AdAddOm,		// 売却先（所在地）
// midori 200101 add -->
	Field071SpGengo,		// 異動年月日の元号
// midori 200101 add <--
	// 最大数
	Field071Max				// 
} PRT_ENUM_FIELD_071;

//インボイス登録番号追加対応_23/10/23 add -->
// ⑦．固定資産（インボイス登録番号追加版）
typedef enum tagPrtEnumField071I {
	Field071ISyurui1 = 0,	// 種類・構造
	Field071ISyurui2,		// 
	Field071IYouto1,		// 用途
	Field071IYouto2,		// 
	Field071IArea,			// 面積
	Field071IAd2Add1,		// 物件の所在地
	Field071IAd2Add2,		// 
	Field071IVal,			// 期末現在高
	Field071ISpDate,		// 異動年月日
	Field071IRiyuu1,		// 異動事由
	Field071IRiyuu2,		// 
	Field071IMsVal,			// 取得価額
	Field071IMsVal2,		// 異動直前の帳簿価額
	Field071IInvoNo,		// インボイス登録番号
	Field071IAdName1,		// 売却先（名称：上段）
	Field071IAdName2,		// 売却先（名称：下段）
	Field071IAdAdd1,		// 売却先（所在地：上段）
	Field071IAdAdd2,		// 売却先（所在地：下段）
	Field071ISpDate21,		// 売却物件の取得年月
	Field071ISpDate22,		// 
	// （計）
	Field071IKeiStr,		// 
	// （省略文字）
	Field071ISyuruiOm,		// 種類・構造
	Field071IAd2AddOm,		// 物件の所在地
	Field071IRiyuu1Om,		// 異動事由
	Field071IAdNameOm,		// 売却先(名称）
	Field071IAdAddOm,		// 売却先（所在地）
	Field071ISpGengo,		// 異動年月日の元号
	// 最大数
	Field071IMax				// 
} PRT_ENUM_FIELD_071I;
//インボイス登録番号追加対応_23/10/23 add <--

// ⑧．支払手形
typedef enum tagPrtEnumField081{
	Field081AdName1 = 0,	// 支払先（名称：上段）
	Field081AdName2,		// 支払先（名称：下段）
	Field081SpDate,			// 振出年月日
	Field081PmDate,			// 支払期日
	Field081BkName1,		// 支払銀行名（銀行名）
	Field081BkName2,		// 支払銀行名（支店名）
	Field081BkName1_1Slash,	// 支払銀行名（銀行名）（一段表示用）1行目
	Field081BkName1_2Slash,	// 支払銀行名（銀行名）（一段表示用）2行目
	Field081BkName2_1Slash,	// 支払銀行名（支店名）（一段表示用）1行目
	Field081BkName2_2Slash,	// 支払銀行名（支店名）（一段表示用）2行目
	Field081BkName1Slash,	// 支払銀行名（銀行名）（一段表示用）5文字以下
	Field081BkName2Slash,	// 支払銀行名（支店名）（一段表示用）5文字以下
	Field081Val,			// 金額
	Field081Teki1,			// 摘要
	Field081Teki2,			// 
	// （計）
	Field081KeiStr,			// 
	// （省略文字）
	Field081AdNameOm,		// 支払先
	Field081BkNameOm,		// 支払銀行名
	Field081Slash,			// "/"
// No.200903 del -->
//// midori 190301 add -->
//	Field081Teki3,			// 摘要３行目（簡素化用）
//	Field081StNameOm,		// 支払銀行名
//// midori 190301 add <--
// No.200903 del <--
// No.200903 add -->
// midori 190301 add -->
	Field081StNameOm,		// 支払銀行名
	//
	Field081Teki3,			// 摘要３行目（簡素化用）
// midori 190301 add <--
// No.200903 add <--
// midori 155870 add -->
	Field081SpDate2,		// 振出年月日（元号XX年XX月XX日）
	Field081PmDate2,		// 支払期日（元号XX年XX月XX日）
// midori 155870 add <--
	// 最大数
	Field081Max				// 
} PRT_ENUM_FIELD_081;

// インボイス登録番号追加対応_23/10/23 add -->
// ⑧．支払手形（インボイス登録番号追加版）
typedef enum tagPrtEnumField081I {
	Field081IInvoNo = 0,		// インボイス登録番号
	Field081IAdName1,			// 支払先（名称：上段）
	Field081IAdName2,			// 支払先（名称：下段）
	Field081ISpDate,			// 振出年月日
	Field081IPmDate,			// 支払期日
	Field081IBkName1,			// 支払銀行名（銀行名）
	Field081IBkName2,			// 支払銀行名（支店名）
	Field081IBkName1_1Slash,	// 支払銀行名（銀行名）（一段表示用）1行目
	Field081IBkName1_2Slash,	// 支払銀行名（銀行名）（一段表示用）2行目
	Field081IBkName2_1Slash,	// 支払銀行名（支店名）（一段表示用）1行目
	Field081IBkName2_2Slash,	// 支払銀行名（支店名）（一段表示用）2行目
	Field081IBkName1Slash,		// 支払銀行名（銀行名）（一段表示用）5文字以下
	Field081IBkName2Slash,		// 支払銀行名（支店名）（一段表示用）5文字以下
	Field081IVal,				// 金額
	Field081ITeki1,				// 摘要
	Field081ITeki2,				// 
	// （計）
	Field081IKeiStr,			// 
	// （省略文字）
	Field081IAdNameOm,			// 支払先
	Field081IBkNameOm,			// 支払銀行名
	Field081ISlash,				// "/"
	Field081IStNameOm,			// 支払銀行名
	//
	Field081ITeki3,				// 摘要３行目（簡素化用）
	Field081ISpDate2,			// 振出年月日（元号XX年XX月XX日）
	Field081IPmDate2,			// 支払期日（元号XX年XX月XX日）
// 改良No.21-0086,21-0529 cor -->
	//Field081IKeiStr2,			// 頁計、累計		// 修正No.168370 add
// ------------------------------
	Field081IKnName,			// 科目（支払先上段を使用）
	Field081IKnName1,			// 科目（支払先上段を使用）（9文字以上）
	Field081IKnName2,			// 科目（支払先下段を使用）（9文字以上）
	// （計）
	Field081IKeiStr1,			// 10文字を超えたときの "計" 上段
	Field081IKeiStr2,			// 10文字を超えたときの "計" 下段
	Field081IKeiStr3,			// 
	Field081IKeiStr4,			// 
	Field081IKeiStr5,			// 
	Field081IKeiStr6,			// 
	Field081IKeiStr7,			// 
	Field081IKeiStr8,			// 
	Field081IKeiStr9,			// 
	Field081IKeiStr10,			// 
	Field081IKeiStr11,			// 頁計、累計
// 改良No.21-0086,21-0529 add <--
	// 最大数
	Field081IMax				// 
} PRT_ENUM_FIELD_081I;
// インボイス登録番号追加対応_23/10/23 add <--

// ⑨．買掛金
typedef enum tagPrtEnumField091{
	Field091KnName1 = 0,	// 科目（名称）（9文字以上）
	Field091KnName2,		// 科目（名称）
	Field091AdName1,		// 相手先（名称：上段）
	Field091AdName2,		// 相手先（名称：下段）
	Field091AdAdd1,			// 相手先（所在地：上段）
	Field091AdAdd2,			// 相手先（所在地：下段）
	Field091Val,			// 期末現在高
	Field091Teki1,			// 摘要
	Field091Teki2,			// 
	// （下欄）
	Field092SpDateL,		// 支払確定年月日（配当金）
	Field092ValL,			// 期末現在高（配当金）
	Field092SpDateR,		// 支払確定年月日（役員賞与）
	Field092ValR,			// 期末現在高（役員賞与）
	// （計）
	Field091KeiStr,			// 
	// （省略文字）
	Field091KnNameOm,		// 科目名
	Field091AdNameOm,		// 相手先（名称）　
	Field091AdAddOm,		// 相手先（所在地）　
	// 最大数
	Field091Max				// 
} PRT_ENUM_FIELD_091;

// インボイス登録番号追加対応_23/10/23 add -->
// ⑨．買掛金（インボイス登録番号追加版）
typedef enum tagPrtEnumField091I {
	Field091IKnName1 = 0,	// 科目（名称）（9文字以上）
	Field091IKnName2,		// 科目（名称）
	Field091IInvoNo,		// インボイス登録番号
	Field091IAdName1,		// 相手先（名称：上段）
	Field091IAdName2,		// 相手先（名称：下段）
	Field091IAdAdd1,		// 相手先（所在地：上段）
	Field091IAdAdd2,		// 相手先（所在地：下段）
	Field091IVal,			// 期末現在高
	Field091ITeki1,			// 摘要
	Field091ITeki2,			// 
	// （下欄）
	Field092ISpDateL,		// 支払確定年月日（配当金）
	Field092IValL,			// 期末現在高（配当金）
	Field092ISpDateR,		// 支払確定年月日（役員賞与）
	Field092IValR,			// 期末現在高（役員賞与）
	// （計）
	Field091IKeiStr,		// 
	// （省略文字）
	Field091IKnNameOm,		// 科目名
	Field091IAdNameOm,		// 相手先（名称）　
	Field091IAdAddOm,		// 相手先（所在地）　
	// 最大数
	Field091IMax			// 
} PRT_ENUM_FIELD_091I;
// インボイス登録番号追加対応_23/10/23 add <--

// ⑩-1．仮受金
typedef enum tagPrtEnumField101{
	Field101KnName1 = 0,	// 科目（名称）（9文字以上）
	Field101KnName2,		// 科目（名称）
	Field101AdName1,		// 相手先（名称：上段）
	Field101AdName2,		// 相手先（名称：下段）
	Field101AdAdd1,			// 相手先（所在地：上段）
	Field101AdAdd2,			// 相手先（所在地：下段）
	Field101Kankei,			// 法人・代表者との関係
	Field101Val,			// 期末現在高
	Field101Naiyou1,		// 取引の内容
	Field101Naiyou2,		// 
	// （計）
	Field101KeiStr,			// 
	// （省略文字）
	Field101KnNameOm,		// 科目名
	Field101AdNameOm,		// 相手先（名称）　
	Field101AdAddOm,		// 相手先（所在地）　
	Field101KankeiOm,		// 相手先（関係）
	// 最大数
	Field101Max				// 
} PRT_ENUM_FIELD_101;

// インボイス登録番号追加対応_23/10/23 add -->
// ⑩-1．仮受金（インボイス登録番号追加版）
typedef enum tagPrtEnumField101I {
	Field101IKnName1 = 0,	// 科目（名称）（9文字以上）
	Field101IKnName2,		// 科目（名称）
	Field101IInvoNo,		// インボイス登録番号
	Field101IAdName1,		// 相手先（名称：上段）
	Field101IAdName2,		// 相手先（名称：下段）
	Field101IAdAdd1,		// 相手先（所在地：上段）
	Field101IAdAdd2,		// 相手先（所在地：下段）
	Field101IKankei,		// 法人・代表者との関係
	Field101IVal,			// 期末現在高
	Field101INaiyou1,		// 取引の内容
	Field101INaiyou2,		// 
	// （計）
	Field101IKeiStr,		// 
	// （省略文字）
	Field101IKnNameOm,		// 科目名
	Field101IAdNameOm,		// 相手先（名称）　
	Field101IAdAddOm,		// 相手先（所在地）　
	Field101IKankeiOm,		// 相手先（関係）
	// 最大数
	Field101IMax			// 
} PRT_ENUM_FIELD_101I;
// インボイス登録番号追加対応_23/10/23 add <--

// ⑩-2．源泉所得税預り金
typedef enum tagPrtEnumField102{
	Field102SpDateL = 0,	// 年月日（左列）
	Field102KnNameL,		// 所得の種類（左列）
	Field102ValL,			// 期末現在高（左列）
	Field102SpDateR,		// 年月日（右列）
	Field102KnNameR,		// 所得の種類（右列）
	Field102ValR,			// 期末現在高（右列）
	// （計）
	Field102KeiL,			// （左列）
	Field102KeiR,			// （右列）
	// （省略文字）
	Field102KnNameLOm,		// 取得の種類（左列）
	Field102KnNameROm,		// 取得の種類（右列）
	// 最大数
	Field102Max				// 
} PRT_ENUM_FIELD_102;

// ⑪．借入金
typedef enum tagPrtEnumField111{
	Field111KnName = 0,		// 科目（借入先名称１を使用）
	Field111KnName1,		// 科目（借入先名称１を使用）（9文字以上）
	Field111KnName2,		// 科目（借入先名称１を使用）（9文字以上）
	Field111AdName1,		// 借入先（名称１）
	Field111AdName2,		// 借入先（名称２）
	Field111AdAdd1,			// 借入先（所在地１）
	Field111AdAdd2,			// 借入先（所在地２）
	Field111Kankei,			// 法人・代表者との関係
	Field111Val,			// 期末現在高
	Field111Risoku,			// 期中の支払利子額
	Field111Rate,			// 利率
	Field111Riyuu1,			// 借入理由
	Field111Riyuu2,			// 
	Field111Naiyou1,		// 担保の内容
	Field111Naiyou2,		// 
	// （計）
	Field111KeiStr,			// 
	Field111DnKeiStr,		// 最下行の計の文言
	// （省略文字）
	Field111AdNameOm,		// 借入先
	Field111AdAddOm,		// 借入先（所在地）
	Field111KankeiOm,		// 借入先（関係）
// midori 190301 add -->
	Field111Kankei2,		// 法人・代表者との関係（下段）	※簡素化用
	Field111AdAdd1J,		// 貸付先（所在地：上段の上段）
	Field111AdAdd2J,		// 貸付先（所在地：下段の上段）
	Field111Naiyou0,		// 担保の内容
	Field111KeiStr2,		// 
// midori 190301 add <--
// midori 191002 add -->
	Field111KeiStr3,		// 
	Field111KeiStr4,		// 
	Field111KeiStr5,		// 
// midori 191002 add <--
// midori 155541,155542 add -->
	Field111KeiStr6,		// 
	Field111KeiStr7,		// 
// midori 155541,155542 add <--
// midori 156189 add -->
	Field111KeiStr8,		// 
	Field111KeiStr9,		// 
// midori 156189 add <--
	// 最大数
	Field111Max				// 
} PRT_ENUM_FIELD_111;

// ⑫．土地の売上高等
typedef enum tagPrtEnumField121{
//	Field121Kubun = 0,		// 区分
	Field121KnName,			// 区分
	Field121Ad2Add1,		// 商品の所在地
	Field121Ad2Add2,		// 
	Field121HdName,			// 地目
	Field121Area,			// 総面積
	Field121SpDate,			// 売上年月
	Field121AdName1,		// 売上先（名称１）
	Field121AdName2,		// 売上先（名称２）
	Field121AdAdd1,			// 売上先（所在地：上段）
	Field121AdAdd2,			// 売上先（所在地：下段）
	Field121Area2,			// 売上面積
	Field121Val,			// 売上金額（上段）
	Field121Val2,			// 売上金額（下段）
	Field121SpDate2,		// 売上商品の取得年
	// （計）
	Field121KeiStr,			// 
	// （省略文字）
//	Field121KubunOm,		// 区分
	Field121KnNameOm,		// 区分
	Field121Ad2AddOm,		// 商品の所在地
	Field121HdNameOm,		// 地目
	Field121AdNameOm,		// 売上先（名称）
	Field121AdAddOm,		// 売上先（所在地）
// midori 200101 add -->
	Field121SpGengo2,		// 売上商品の取得年の元号
	Field121SpGengo,		// 売上年月の元号
// midori 200101 add <--
	// 最大数
	Field121Max				// 
} PRT_ENUM_FIELD_121;

// インボイス登録番号追加対応_23/10/23 add -->
// ⑫．土地の売上高等（インボイス登録番号追加版）
typedef enum tagPrtEnumField121I {
	Field121IKnName = 0,	// 区分
	Field121IAd2Add1,		// 商品の所在地
	Field121IAd2Add2,		// 
	Field121IHdName,		// 地目
	Field121IArea,			// 総面積
	Field121ISpDate,		// 売上年月
	Field121IInvoNo,		// インボイス登録番号
	Field121IAdName1,		// 売上先（名称１）
	Field121IAdName2,		// 売上先（名称２）
	Field121IAdAdd1,		// 売上先（所在地：上段）
	Field121IAdAdd2,		// 売上先（所在地：下段）
	Field121IArea2,			// 売上面積
	Field121IVal,			// 売上金額（上段）
	Field121IVal2,			// 売上金額（下段）
	Field121ISpDate2,		// 売上商品の取得年
	// （計）
	Field121IKeiStr,		// 
	// （省略文字）
	Field121IKnNameOm,		// 区分
	Field121IAd2AddOm,		// 商品の所在地
	Field121IHdNameOm,		// 地目
	Field121IAdNameOm,		// 売上先（名称）
	Field121IAdAddOm,		// 売上先（所在地）
	Field121ISpGengo2,		// 売上商品の取得年の元号
	Field121ISpGengo,		// 売上年月の元号
	// 最大数
	Field121IMax			// 
} PRT_ENUM_FIELD_121I;
// インボイス登録番号追加対応_23/10/23 add <--

// ⑬．売上高等の事業所別
typedef enum tagPrtEnumField131{
	Field131AdName1 = 0,	// 事業所の名称（名称１）
	Field131AdName2,		// 事業所の名称（名称２）
	Field131AdAdd1,			// 所在地（所在地：上段）
	Field131AdAdd2,			// 所在地（所在地：下段）
	Field131HdName1,		// 責任者氏名
	Field131HdName2,		// 
	Field131Kankei,			// 代表者との関係
	Field131Naiyou1,		// 事業等の内容
	Field131Naiyou2,		// 
	Field131Naiyou3,		// 
	Field131Val,			// 売上高
	Field131Val2,			// 期末棚卸高
	Field131Worker,			// 期末従業員数
	Field131Area,			// 使用建物の延面積
	Field131HdName21,		// 源泉取得納税付署
	Field131HdName22,		// 
	Field131Teki1,			// 摘要
	Field131Teki2,			// 
	Field131Teki3,			// 
	// （計）
	Field131KeiStr,			// 
	Field131DnKeiStr,		// 最下行の計の文言
	// （省略文字）
	Field131AdNameOm,		// 事業所の名称
	Field131AdAddOm,		// 事業所（所在地）
	Field131HdName1Om,		// 責任者氏名
	Field131KankeiOm,		// 代表者との関係
	// 最大数
	Field131Max				// 
} PRT_ENUM_FIELD_131;

// ⑭-1．役員報酬手当等
typedef enum tagPrtEnumField141{
	Field141NameYaku = 0,	// 役職名
	Field141NameTant,		// 担当業務
	Field141AdName1,		// 氏名（名称：上段）
	Field141AdName2,		// 氏名（名称：下段）
	Field141AdAdd1,			// 住所（所在地：上段）
	Field141AdAdd2,			// 住所（所在地：下段）
	Field141Kankei,			// 代表者との関係
	Field141FpTime1,		// 常勤・非常勤の別
	Field141FpTime2,		// 
// 2006.10.23 Update start：新会社法対応
	Field141Val,			// 役員給与計
	Field141EmVal,			// 使用人職務分
	Field141PrVal,			// 定期同額給与
	Field141BfVal,			// 事前確定届出給与
	Field141PfVal,			// 利益連動給与
	Field141EtVal,			// その他
	Field141RtVal,			// 退職給与
// 2006.10.23 Update end：新会社法対応
	// （計）
	Field141KeiStr,			// 
	// （省略文字）
	Field141NameYakuOm,		// 役職名
	Field141NameTantOm,		// 担当業務
	Field141KankeiOm,		// 代表者との関係
	Field141AdAddOm,		// 住所
// midori 190301 add -->
	Field141KeiStr2,		// 最終ページの頁計、小計、中計
	Field141NameYaku2,		// 役職名　(15byte以上)
	Field141NameTant2,		// 担当業務(15byte以上)
	Field141NameYaku3,		// 役職名　(16byte以上)
	Field141NameTant3,		// 担当業務(16byte以上)
// midori 190301 add <--
// 調査依頼20240712_24/09/19 del -->
//// 改良No.23-0961 add -->
//	Field141Val2,			// 役員給与計（10桁入力用）
//	Field141EmVal2,			// 使用人職務分（10桁入力用）
//	Field141PrVal2,			// 定期同額給与（10桁入力用）
//	Field141BfVal2,			// 事前確定届出給与（10桁入力用）
//	Field141PfVal2,			// 利益連動給与（10桁入力用）
//	Field141EtVal2,			// その他（10桁入力用）
//	Field141RtVal2,			// 退職給与（10桁入力用）
//// 改良No.23-0961 add <--
// 調査依頼20240712_24/09/19 del <--
	// 最大数
	Field141Max				// 
} PRT_ENUM_FIELD_141;

// 調査依頼20240712_24/09/19 add -->
// ⑭-1．役員報酬手当等（簡素化以降）
typedef enum tagPrtEnumField141K{
	Field141KNameYaku = 0,	// 役職名
	Field141KNameTant,		// 担当業務
	Field141KAdName1,		// 氏名（名称：上段）
	Field141KAdName2,		// 氏名（名称：下段）
	Field141KAdAdd1,		// 住所（所在地：上段）
	Field141KAdAdd2,		// 住所（所在地：下段）
	Field141KKankei,		// 代表者との関係
	Field141KFpTime1,		// 常勤・非常勤の別
	Field141KFpTime2,		// 
	Field141KVal,			// 役員給与計
	Field141KEmVal,			// 使用人職務分
	Field141KPrVal,			// 定期同額給与
	Field141KBfVal,			// 事前確定届出給与
	Field141KPfVal,			// 利益連動給与
	Field141KEtVal,			// その他
	Field141KRtVal,			// 退職給与
	// （計）
	Field141KKeiStr,		// 
	// （省略文字）
	Field141KNameYakuOm,	// 役職名
	Field141KNameTantOm,	// 担当業務
	Field141KKankeiOm,		// 代表者との関係
	Field141KAdAddOm,		// 住所
	Field141KKeiStr2,		// 最終ページの頁計、小計、中計
	Field141KNameYaku2,		// 役職名　(15byte以上)
	Field141KNameTant2,		// 担当業務(15byte以上)
	Field141KNameYaku3,		// 役職名　(16byte以上)
	Field141KNameTant3,		// 担当業務(16byte以上)
	//
	Field141KVal2,			// 役員給与計（10桁入力用）
	Field141KEmVal2,		// 使用人職務分（10桁入力用）
	Field141KPrVal2,		// 定期同額給与（10桁入力用）
	Field141KBfVal2,		// 事前確定届出給与（10桁入力用）
	Field141KPfVal2,		// 利益連動給与（10桁入力用）
	Field141KEtVal2,		// その他（10桁入力用）
	Field141KRtVal2,		// 退職給与（10桁入力用）
	// 最大数
	Field141KMax			// 
} PRT_ENUM_FIELD_141K;
// 調査依頼20240712_24/09/19 add <--

// ⑭-2．人件費
typedef enum tagPrtEnumField142{
	Field142Val = 0,		// 総額
	Field142Val2,			// 総額のうち代表者及びその家族分
	// （計）
	Field142KeiStr,			// 
	// （省略文字）
	Field142DummyOm,		// 
	// 最大数
	Field142Max				// 
} PRT_ENUM_FIELD_142;

// ⑮-1．地代家賃
typedef enum tagPrtEnumField151{
//	Field151Kubun = 0,		// 地代・家賃の区分
	Field151KnName = 0,		// 地代・家賃の区分
	Field151Youto1,			// 借地物件の用途
	Field151Youto2,			// 
	Field151Ad2Add1,		// 所在地（上段）
	Field151Ad2Add2,		// 所在地（下段）
	Field151AdName1,		// 貸主（名称：上段）
	Field151AdName2,		// 貸主（名称：下段）
	Field151AdAdd1,			// 貸主（所在地：上段）
	Field151AdAdd2,			// 貸主（所在地：下段）
// midori 155870 del -->
//	Field151SpDate,			// 支払対象期間(自 ～)
// midori 155870 del <--
// midori 155870 add -->
	Field151SpDate,			// 支払対象期間(自、至 どちらかのみ)+元号有り
// midori 155870 add <--
	Field151Val,			// 支払貸借料
	Field151Teki1,			// 摘要
	Field151Teki2,			// 
	Field151Teki3,			// 
	// （計）
	Field151KeiStr,			// 
	// （省略文字）
//	Field151KubunOm,		// 地代家賃の区分
	Field151KnNameOm,		// 地代家賃の区分
	Field151YoutoOm,		// 借地物件の用途
	Field151Ad2AddOm,		// 所在地
	Field151AdNameOm,		// 貸主の名称
	Field151AdAddOm,		// 貸主の所在地
// midori 155870 del -->
//// midori 200101 add -->
//	Field151SpDate2,		// 支払対象期間(至)
//	Field151SpDate3,		// 支払対象期間(自、至 どちらかのみ/簡素化以前)
//// midori 200101 add <--
// midori 155870 del <--
// midori 155870 add -->
	Field151SpDate2,		// 支払対象期間(自 ～)	+元号有り
	Field151SpDate3,		// 支払対象期間(至)		+元号有り
	Field151SpDate4,		// 支払対象期間(自、至 どちらかのみ)+元号無し
	Field151SpDate5,		// 支払対象期間(自 ～)	+元号無し
	Field151SpDate6,		// 支払対象期間(至)		+元号無し
// midori 155870 add <--
	// 最大数
	Field151Max				// 
} PRT_ENUM_FIELD_151;

//インボイス登録番号追加対応_23/10/23 add -->
// ⑮-1．地代家賃（インボイス登録番号追加版）
typedef enum tagPrtEnumField151I {
	Field151IKnName = 0,	// 地代・家賃の区分
	Field151IYouto1,		// 借地物件の用途
	Field151IYouto2,		// 
	Field151IAd2Add1,		// 所在地（上段）
	Field151IAd2Add2,		// 所在地（下段）
	Field151IInvoNo,		// インボイス登録番号
	Field151IAdName1,		// 貸主（名称：上段）
	Field151IAdName2,		// 貸主（名称：下段）
	Field151IAdAdd1,		// 貸主（所在地：上段）
	Field151IAdAdd2,		// 貸主（所在地：下段）
	Field151ISpDate,		// 支払対象期間(自、至 どちらかのみ)+元号有り
	Field151IVal,			// 支払貸借料
	Field151ITeki1,			// 摘要
	Field151ITeki2,			// 
	Field151ITeki3,			// 
	// （計）
	Field151IKeiStr,		// 
	// （省略文字）
	Field151IKnNameOm,		// 地代家賃の区分
	Field151IYoutoOm,		// 借地物件の用途
	Field151IAd2AddOm,		// 所在地
	Field151IAdNameOm,		// 貸主の名称
	Field151IAdAddOm,		// 貸主の所在地
	Field151ISpDate2,		// 支払対象期間(自 ～)	+元号有り
	Field151ISpDate3,		// 支払対象期間(至)		+元号有り
	Field151ISpDate4,		// 支払対象期間(自、至 どちらかのみ)+元号無し
	Field151ISpDate5,		// 支払対象期間(自 ～)	+元号無し
	Field151ISpDate6,		// 支払対象期間(至)		+元号無し
	// 最大数
	Field151IMax			// 
} PRT_ENUM_FIELD_151I;
//インボイス登録番号追加対応_23/10/23 add <--

// ⑮-2．権利金等の期中支払
typedef enum tagPrtEnumField152{
	Field152AdName1 = 0,	// 支払先（名称１）
	Field152AdName2,		// 支払先（名称２）
	Field152AdAdd1,			// 支払先（所在地：上段）
	Field152AdAdd2,			// 支払先（所在地：下段）
	Field152SpDate,			// 支払年月日
	Field152Val,			// 支払金額
	Field152Naiyou1,		// 権利金等の内容
	Field152Naiyou2,		// 
	Field152Teki1,			// 摘要
	Field152Teki2,			// 
	Field152Teki3,			// 
	// （計）
	Field152KeiStr,			// 
	// （省略文字）
	Field152AdNameOm,		// 支払先の名称
	Field152AdAddOm,		// 支払先の所在地
	Field152NaiyouOm,		// 権利金等の内容
	// 最大数
	Field152Max				// 
} PRT_ENUM_FIELD_152;

// インボイス登録番号追加対応_23/10/23 add -->
// ⑮-2．権利金等の期中支払（インボイス登録番号追加版）
typedef enum tagPrtEnumField152I {
	Field152IInvoNo = 0,	// インボイス登録番号
	Field152IAdName1,		// 支払先（名称１）
	Field152IAdName2,		// 支払先（名称２）
	Field152IAdAdd1,		// 支払先（所在地：上段）
	Field152IAdAdd2,		// 支払先（所在地：下段）
	Field152ISpDate,		// 支払年月日
	Field152IVal,			// 支払金額
	Field152INaiyou1,		// 権利金等の内容
	Field152INaiyou2,		// 
	Field152ITeki1,			// 摘要
	Field152ITeki2,			// 
	Field152ITeki3,			// 
	// （計）
	Field152IKeiStr,		// 
	// （省略文字）
	Field152IAdNameOm,		// 支払先の名称
	Field152IAdAddOm,		// 支払先の所在地
	Field152INaiyouOm,		// 権利金等の内容
	// 最大数
	Field152IMax			// 
} PRT_ENUM_FIELD_152I;
// インボイス登録番号追加対応_23/10/23 add <--

// ⑮-3．工業所有権の使用料
typedef enum tagPrtEnumField153{
	Field153HdName1 = 0,	// 名称
	Field153HdName2,		// 
	Field153AdName1,		// 支払先（名称１）
	Field153AdName2,		// 支払先（名称２）
	Field153AdAdd1,			// 支払先（所在地：上段）
	Field153AdAdd2,			// 支払先（所在地：下段）
	Field153KyDate,			// 契約期間（開始）
	Field153KyDate2,		// 契約期間（終了）
	Field153KyDate0,		// 契約期間（片方のみ）
	Field153SpDate,			// 支払対象期間（開始）
	Field153SpDate2,		// 支払対象期間（終了）
	Field153SpDate0,		// 支払対象期間（片方のみ）
	Field153Val,			// 支払金額
	Field153Teki1,			// 摘要
	Field153Teki2,			// 
	Field153Teki3,			// 
	// （計）
	Field153KeiStr,			// 
	// （省略文字）
	Field153HdNameOm,		// 名称
	Field153AdNameOm,		// 支払先の名称
	Field153AdAddOm,		// 支払先の所在地
	// 最大数
	Field153Max				// 
} PRT_ENUM_FIELD_153;

//インボイス登録番号追加対応_23/10/23 add -->
//⑮-3．工業所有権の使用料（インボイス登録番号追加版）
typedef enum tagPrtEnumField153I {
	Field153IHdName1 = 0,	// 名称
	Field153IHdName2,		// 
	Field153IInvoNo,		// インボイス登録番号
	Field153IAdName1,		// 支払先（名称１）
	Field153IAdName2,		// 支払先（名称２）
	Field153IAdAdd1,		// 支払先（所在地：上段）
	Field153IAdAdd2,		// 支払先（所在地：下段）
	Field153IKyDate,		// 契約期間（開始）
	Field153IKyDate2,		// 契約期間（終了）
	Field153IKyDate0,		// 契約期間（片方のみ）
	Field153ISpDate,		// 支払対象期間（開始）
	Field153ISpDate2,		// 支払対象期間（終了）
	Field153ISpDate0,		// 支払対象期間（片方のみ）
	Field153IVal,			// 支払金額
	Field153ITeki1,			// 摘要
	Field153ITeki2,			// 
	Field153ITeki3,			// 
	// （計）
	Field153IKeiStr,		// 
	// （省略文字）
	Field153IHdNameOm,		// 名称
	Field153IAdNameOm,		// 支払先の名称
	Field153IAdAddOm,		// 支払先の所在地
	// 最大数
	Field153IMax			// 
} PRT_ENUM_FIELD_153I;
//インボイス登録番号追加対応_23/10/23 add <--

// ⑯-1．雑益等
typedef enum tagPrtEnumField161{
	Field161KnName1 = 0,	// 科目（名称）（9文字以上）
	Field161KnName2,		// 科目（名称）
	Field161Naiyou1,		// 取引の内容
	Field161Naiyou2,		// 
	Field161AdName1,		// 相手先（名称：上段）
	Field161AdName2,		// 相手先（名称：下段）
	Field161AdAdd1,			// 相手先（所在地：上段）
	Field161AdAdd2,			// 相手先（所在地：下段）
	Field161Val,			// 金額
	// （計）
	Field161KeiStr,			// 
	// （省略文字）
	Field161KnNameOm,		// 科目
	Field161NaiyouOm,		// 取引の内容
	Field161AdNameOm,		// 相手先
	Field161AdAddOm,		// 所在地
	// 最大数
	Field161Max				// 
} PRT_ENUM_FIELD_161;

// インボイス登録番号追加対応_23/10/23 add -->
// ⑯-1．雑益等（インボイス登録番号追加版）
typedef enum tagPrtEnumField161I {
	Field161IKnName1 = 0,	// 科目（名称）（9文字以上）
	Field161IKnName2,		// 科目（名称）
	Field161INaiyou1,		// 取引の内容
	Field161INaiyou2,		// 
	Field161IInvoNo,		// インボイス登録番号
	Field161IAdName1,		// 相手先（名称：上段）
	Field161IAdName2,		// 相手先（名称：下段）
	Field161IAdAdd1,		// 相手先（所在地：上段）
	Field161IAdAdd2,		// 相手先（所在地：下段）
	Field161IVal,			// 金額
	// （計）
	Field161IKeiStr,		// 
	// （省略文字）
	Field161IKnNameOm,		// 科目
	Field161INaiyouOm,		// 取引の内容
	Field161IAdNameOm,		// 相手先
	Field161IAdAddOm,		// 所在地
	// 最大数
	Field161IMax			// 
} PRT_ENUM_FIELD_161I;
// インボイス登録番号追加対応_23/10/23 add <--

// ⑯-2．雑損失等
typedef enum tagPrtEnumField162{
	Field162KnName1 = 0,	// 科目（名称）（9文字以上）
	Field162KnName2,		// 科目（名称）
	Field162Naiyou1,		// 取引の内容
	Field162Naiyou2,		// 
	Field162AdName1,		// 相手先（名称：上段）
	Field162AdName2,		// 相手先（名称：下段）
	Field162AdAdd1,			// 相手先（所在地：上段）
	Field162AdAdd2,			// 相手先（所在地：下段）
	Field162Val,			// 金額
	// （計）
	Field162KeiStr,			// 
	// （省略文字）
	Field162KnNameOm,		// 科目
	Field162NaiyouOm,		// 取引の内容
	Field162AdNameOm,		// 相手先
	Field162AdAddOm,		// 所在地
	// 最大数
	Field162Max				// 
} PRT_ENUM_FIELD_162;

// インボイス登録番号追加対応_23/10/23 add -->
// ⑯-2．雑損失等（インボイス登録番号追加版）
typedef enum tagPrtEnumField162I {
	Field162IKnName1 = 0,	// 科目（名称）（9文字以上）
	Field162IKnName2,		// 科目（名称）
	Field162INaiyou1,		// 取引の内容
	Field162INaiyou2,		// 
	Field162IInvoNo,		// インボイス登録番号
	Field162IAdName1,		// 相手先（名称：上段）
	Field162IAdName2,		// 相手先（名称：下段）
	Field162IAdAdd1,		// 相手先（所在地：上段）
	Field162IAdAdd2,		// 相手先（所在地：下段）
	Field162IVal,			// 金額
	// （計）
	Field162IKeiStr,		// 
	// （省略文字）
	Field162IKnNameOm,		// 科目
	Field162INaiyouOm,		// 取引の内容
	Field162IAdNameOm,		// 相手先
	Field162IAdAddOm,		// 所在地
	// 最大数
	Field162IMax			// 
} PRT_ENUM_FIELD_162I;
// インボイス登録番号追加対応_23/10/23 add <--

// ⑰．その他１
typedef enum tagPrtEnumField171{
	Field171KnName1 = 0,	// 科目（名称）（9文字以上）
	Field171KnName2,		// 科目（名称）
	Field171AdName1,		// 相手先（名称：上段）
	Field171AdName2,		// 相手先（名称：下段）
	Field171AdAdd1,			// 相手先（所在地：上段）
	Field171AdAdd2,			// 相手先（所在地：下段）
	Field171Val,			// 期末現在高
	Field171Teki1,			// 摘要
	Field171Teki2,			// 
	// （計）
	Field171KeiStr,			// 
	// （省略文字）
	Field171KnNameOm,		// 科目
	Field171AdNameOm,		// 相手先
	Field171AdAddOm,		// 所在地
	// 最大数
	Field171Max				// 
} PRT_ENUM_FIELD_171;

// インボイス登録番号追加対応_23/10/23 add -->
// ⑰．その他１（インボイス登録番号追加版）
typedef enum tagPrtEnumField171I {
	Field171IKnName1 = 0,	// 科目（名称）（9文字以上）
	Field171IKnName2,		// 科目（名称）
	Field171IInvoNo,		// インボイス登録番号
	Field171IAdName1,		// 相手先（名称：上段）
	Field171IAdName2,		// 相手先（名称：下段）
	Field171IAdAdd1,		// 相手先（所在地：上段）
	Field171IAdAdd2,		// 相手先（所在地：下段）
	Field171IVal,			// 期末現在高
	Field171ITeki1,			// 摘要
	Field171ITeki2,			// 
	// （計）
	Field171IKeiStr,		// 
	// （省略文字）
	Field171IKnNameOm,		// 科目
	Field171IAdNameOm,		// 相手先
	Field171IAdAddOm,		// 所在地
	// 最大数
	Field171IMax			// 
} PRT_ENUM_FIELD_171I;
// インボイス登録番号追加対応_23/10/23 add <--

// ⑱．その他２
typedef enum tagPrtEnumField181{
	Field181KnName1 = 0,	// 科目（名称）（9文字以上）
	Field181KnName2,		// 科目（名称）
	Field181Item1,			// 項目
	Field181Item2,			// 
	Field181Val,			// 期末現在高
	Field181Teki1,			// 摘要
	Field181Teki2,			// 
	// （計）
	Field181KeiStr,			// 
	// （省略文字）
	Field181KnNameOm,		// 科目
	// （項目タイトル）
	Field181ItemName,		// 項目名
	// 最大数
	Field181Max				// 
} PRT_ENUM_FIELD_181;

// 表紙
typedef enum tagPrtEnumFieldCvr{
	FieldCvrGengo = 0,
	FieldCvrKikan1,			// 決算期間（自）
// midori 150957 add -->
	FieldCvrGengo2,			// 元号（至）
// midori 150957 add <--
	FieldCvrKikan2,			// 決算期間（至）
	FieldCvrGenzai,			// 「現在」
	FieldCvrCoName,			// 会社名
	FieldCvrCoAddr1,		// 住所
	FieldCvrCoAddr2,		// 
	FieldCvrKi,
//	FieldCvrHjNo,			// 法人番号
	// 最大数
	FieldCvrMax				// 
} PRT_ENUM_FIELD_CVR;

typedef enum tagPrtEnumFieldCvrDate{
	FieldCvrDateYear = 0,	// 年（和暦）
	FieldCvrDateMonth,		// 月
	FieldCvrDateDay			// 日
} PRT_ENUM_FIELD_CVR_DATE;

// 共通
typedef enum tagPrtEnumFieldCmn{
	FieldCmnCoName = 0,		// 会社名
	FieldCmnKikan,			// 決算期間
	FieldCmnPageNum,		// ページ番号
	FieldCmnTitle,			// 帳表タイトル 
	FieldCmnDnTitle040,		// 下段タイトル（帳表④）
	FieldCmnDnTitle100,		// 下段タイトル（帳表⑩）
	FieldCmnDnTitle150,		// 下段タイトル（帳表⑮）
//	FieldCmnHjNo,			// 法人番号
	// 最大数
	FieldCmnMax			// 
} PRT_ENUM_FIELD_CMN;

//**************************************************************************************************
// 構造体群
//**************************************************************************************************

// 印刷時の最小限の情報
typedef struct tagPrtInfLite{
	int					FormSeq;			// 様式シーケンス番号
	int					PageMin;			// 印刷対象の最小ページ番号
	int					PageMax;			// 印刷対象の最大ページ番号
} PRT_INF_LITE;

typedef struct tagPrtOption{
	tagPrtOption(){
		KinyuLine = 0;
// midori 190505 del -->
		//KamokuRow = 0;
// midori 190505 del <--
		//OutZeroNull = 0;
		//HideZero = 0;
		::ZeroMemory(OutZeroNull, sizeof(OutZeroNull));
		::ZeroMemory(HideZero, sizeof(HideZero));
		::ZeroMemory(nPageNum, sizeof(nPageNum));
// midori 160610 add -->
		::ZeroMemory(KamokuSitei, sizeof(KamokuSitei));
// midori 160610 add <--
// midori 154119 add -->
		::ZeroMemory(NameKei, sizeof(NameKei));
// midori 154119 add <--
		//mapPageNum.clear();
	}
	int					KinyuLine;			// 金融機関を一段表示する
// midori 190505 del -->
	//int					KamokuRow;			// 科目行を出力する
// midori 190505 del <--
// midori 156188_2 add -->
	int					KamokuRow;			// 科目行を出力する
// midori 156188_2 add <--
	int					OutZeroNull[3];		// D0:金額０円を出力しない　D1:金額空欄を出力しない
	int					HideZero[3];		// 金額０印字 0：する／1：しない
	int					nPageNum[3];		// 出力形式が通常の様式において、片方が０ページの場合でも一時テーブルを参照しに行くバグがあった為、ページ数を保持しておく
// midori 160610 add -->
	int					KamokuSitei[3];		// 科目指定を行う
// midori 160610 add <--
// midori 154119 add -->
	int					NameKei[3];			// 計名称
// midori 154119 add <--
	//int					nPageNum_2;			// 〃
	//int					nPageNum_3;			// 〃
	//map<int, int>		mapPageNum;			// mapで持ちたかったが、例外のせいで使用することができなかった為、やむなく廃止
} PRT_OPTION;

// 印刷時の情報
typedef struct tagPrtInfMain{
	PRT_INF_LITE		Lite;				// 
	int					PageTotal;			// 印刷対象のページ数
	int					PageAll;			// 帳表の総ページ数
	int					FormNo;				// 様式番号
	PRT_ENUM_FORM_TAG	FormTag;			// （DB上でのフィールド名は「FormNo2」）
	int					FormType;			// 出力形式
	PRT_OPTION			Option;
} PRT_INF_MAIN;

// 省略文字の情報
typedef struct tagPrtInfOmit{
	int					ItemSeq;			// 項目番号
	int					Omit;				// 省略 0：しない／1：する
	CString				OmitStr;			// 省略文字	

	tagPrtInfOmit(){
		ItemSeq = 0;
		Omit = 0;
	}

	tagPrtInfOmit(const tagPrtInfOmit& prtinf){
		ItemSeq = prtinf.ItemSeq;
		Omit = prtinf.Omit;
		OmitStr = prtinf.OmitStr;
	}
} PRT_INF_OMIT;

// 内訳書の共通情報
typedef struct tagPrtInfCommon{
	long				KikanStart;			// 決算期間（自）
	long				KikanEnd;			// 決算期間（至）
// midori 153812 del ------------------>
//	long				KikanEndPage;		// 決算期間（各ページ用）
// midori 153812 del <------------------
	int					Ki;					// 期
	int					OutCoName;			// 会社名出力 0：しない／1：する
	int					OutKikan;			// 決算期間出力 0：しない／1：する
	int					OutKingaku;			// 金額欄出力 0：しない／1：する
	int					OutPage;			// ページ番号出力 0：しない／1：する
	int					OutKi;				// 期出力 0：しない／1：する
	int					CoRekinen;			// 暦年表示 0：西暦／1：和暦
	CString				CoName;				// 会社名
	CString				CoAddr1;			// 住所(上段)
	CString				CoAddr2;			// 住所(下段)
// midori 180101 add -->
	int					OutGenzai;			// 「現在」を出力 0:しない／1:する
// midori 180101 add <--
// midori 181101 add -->
	int					StartGen;			// 元号（自）
	int					EndGen;				// 元号（至）
// midori 181101 add <--
//	CString				HjNo_zvol;			// 法人番号(マスター）
//	CString				HjNo_corec;			// 法人番号(顧問先）
// midori 200101 add -->
	int					OutGengo;			// 日付に元号を出力する 0:しない/1:する
// midori 200101 add <--
} PRT_INF_COMMON;

// 帳表タイトル
typedef struct tagPrtInfTitle{
	int					FormSeq;			// 様式シーケンス番号
	CString				Title;				// タイトル
	int					OpTitleKnFg;		// 科目別タイトル使用 0：不可／1：可
	int					OpTitleKn;			// 科目別タイトル使用 0：しない／1：する
} PRT_INF_TITLE;

// 帳表の1列分のデータを格納
typedef struct tagPrtDataList{
	CString				Data[PRT_MAX_ROW];	// 印字文字列
} PRT_DATA_LIST;

// ①預貯金、②受取手形、⑧支払手形の内訳書において、金融機関を1段で表示するための区切り文字数
#define ONE_LINE_CHAR_NUM	5/*10byte*/

#endif	// __UCHIWAKE__PRTCONSTDATA_H__
