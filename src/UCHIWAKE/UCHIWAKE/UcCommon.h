/*****************************************************************
	UcCommon.h
	
	共通事項ヘッダファイル
		定数、列挙型、構造体

	2006.01.21	New
	2006.01.25	統合 UchiwakeConst.h
******************************************************************/

#ifndef	__UCCOMMON_H__
#define	__UCCOMMON_H__

#include	"dbErrCode.h"

// ICSライブラリ郡
#include	"icsocx.h"
#include	"icsdiag.h"
//#include	"ICSDiag.h"
#include	"ICSArith8.h"
#include	"ICSGENL8.h"
#include	"ICSStatic.h"


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//	アプリケーション全体に関連する事項
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//////////////////////////////////////////////////////////////////
//	ドキュメント管理情報
typedef	struct	tag_DocumentInfo {
	UINT	nID;				// ドキュメントテンプレート管理ID（-1:未登録）
	BOOL	bOpenFlg;			// ドキュメントオープン状態　　　（TRUE:オープン済み，FALSE:未オープン）
}	DOCUMENT_INFO;


//////////////////////////////////////////////////////////////////
//	関数の戻り値（成功/失敗）
#define	FUNCTION_OK		0		// 成功
#define	FUNCTION_NG		-1		// 失敗

#define	FUNCTION_UPDATE	1		// 更新あり
#define FUNCTION_DISPUPDATE 2	// 画面表示の更新のみ(データ更新無し)

//////////////////////////////////////////////////////////////////
//	ダイアログのボタンイベント返却ID
//////////////////////////////////////////////////////////////////
typedef	enum {
	ID_DLG_OK		= 0,	//	 0	OKボタン
	ID_DLG_CANCEL,			//	 1	キャンセル
	ID_DLG_BACK,			//	 2	戻す
	ID_DLG_BUTTON1,			//	 3　ボタン１
	ID_DLG_BUTTON2,			//	 4　ボタン２
	ID_DLG_BUTTON3,			//	 5　ボタン３
	ID_DLG_MAX				//		最大値用
} EnumIdDlg;


//////////////////////////////////////////////////////////////////
//	参照ダイアログ種別
typedef enum {
	ID_DLGTYPE_NONE = -1,			//	-1	対象外
	ID_DLGTYPE_BANK = 0,			//	 0	金融機関
	ID_DLGTYPE_BANK_REFERENCE,		//	 1	金融機関（参照のみ＝自動登録しない）	// すべての様式で自動登録化で未使用に　改良依頼No15-479　2016/01/27
	ID_DLGTYPE_ADDRESS,				//	 2	取引先
	ID_DLGTYPE_ADDRESS_REFERENCE,	//	 3	取引先（参照のみ＝自動登録しない）		// すべての様式で自動登録化で未使用に　改良依頼No15-479　2016/01/27
	ID_DLGTYPE_KAMOKU,				//	 4	科目
	ID_DLGTYPE_ICSADDSEARCH,		//	 5	ICS様住所検索
	ID_DLGTYPE_YAKU_ZOKU,			//	 6　役員報酬手当等の内訳書(⑭-1)「役職名」、「代表者との関係」
	ID_DLGTYPE_INVOICENO			//	 7　インボイス登録番号（法人番号）			// インボイス登録番号追加対応_23/11/17 add
} EnumIdDlgType;

//////////////////////////////////////////////////////////////////
//	参照ダイアログモード
typedef enum {
	ID_DLGREF_APPEND = 0,			//	 0	追加
	ID_DLGREF_INSERT,				//	 1	挿入
	ID_DLGREF_UPDATE				//	 2	更新
} EnumIdDlgRefMode;

//////////////////////////////////////////////////////////////////
//	取引先グループ番号（取引先参照などで使用）
#define	ID_ADDRESSGR_NONE	0		//	 0	その他
#define	ID_ADDRESSGR_URI	3		//	 3	売掛金
#define	ID_ADDRESSGR_KAI	10		//	 10	買掛金


//////////////////////////////////////////////////////////////////
//	一時保管・一括表示種別
typedef enum {
	ID_FGSHOW_OFF = 0,			//	0	なし
	ID_FGSHOW_HOKAN,			//	1	一時保管
	ID_FGSHOW_IKKATU,			//	2	一括金額
	ID_FGSHOW_RET_HOKAN = 101,	//  101	一時保管からの戻し対象
	ID_FGSHOW_RET_IKKATU,		//  102 一括金額からの戻し対象
	ID_FGSHOW_TANAOROSHI = 201,	//	201	⑤棚卸資産下欄の印字する／しないの判別に使用
	ID_FGSHOW_KAIKAKEKIN		//	202	⑨買掛金欄の印字する／しないの判別に使用
} EnumIdFgShow;


//////////////////////////////////////////////////////////////////
//	頁計・累計種別
typedef enum {
	ID_OUTKEI_OFF = 0,		//	 0	なし
	ID_OUTKEI_PAGEKEI,		//	 1	頁計
	ID_OUTKEI_RUIKEI,		//	 2	累計
	ID_OUTKEI_BOTH,			//	 3	頁計と累計
} EnumIdOutKei;


//////////////////////////////////////////////////////////////////
//	特殊行属性種別
typedef enum {
	ID_FGFUNC_NULL = 0,				//	 0	空行
	ID_FGFUNC_DATA,					//	 1	データ行
// midori 190505 del -->
//	ID_FGFUNC_KAMOKU = 3,			//	 3　②受取手形、⑪借入金の内訳書の科目行（科目出力での一時テーブルで使用）
// midori 190505 del <--
// midori 190505 add -->
	ID_FGFUNC_KAMOKU = 3,			//	 3　②受取手形、⑪借入金の内訳書の科目行
// midori 190505 add <--
	ID_FGFUNC_DATA_TEMP,			//　 4
	ID_FGFUNC_IKKATUMANUAL_ZERO,	//	 5　一括集計金額行（手動）（０円出力での一時テーブルで使用）
	ID_FGFUNC_IKKATUAUTO_ZERO,		//	 6　一括集計金額行（自動）（０円出力での一時テーブルで使用）
	ID_FGFUNC_SYOKEI = 10,			//	10	小計行
	ID_FGFUNC_SYOKEINULL,			//	11	小計行（空行）
	ID_FGFUNC_CHUKEI,				//	12	中計行
	ID_FGFUNC_CHUKEINULL,			//	13	中計行（空行）
	ID_FGFUNC_RUIKEI,				//	14	累計行
	ID_FGFUNC_PAGEKEI,				//	15	頁計行
	ID_FGFUNC_IKKATUMANUAL = 21,	//	21	一括集計金額行（手動）
	ID_FGFUNC_IKKATUAUTO,			//	22	一括集計金額行（自動）
	ID_FGFUNC_TANAOROSHI = 201,		//	201	⑤棚卸資産下欄で利用
	ID_FGFUNC_KAIKAKEKIN			//	202	⑨買掛金欄外で利用
} EnumIdFgFunc;


//////////////////////////////////////////////////////////////////
//	出力形式種別
typedef enum {
	ID_FORMTYPE_NORMAL = 0,	//	 0	通常
	ID_FORMTYPE_KOBETU 		//	 1	個別
} EnumIdFormType;


//////////////////////////////////////////////////////////////////
//	項目の種類種別
typedef enum {
	ID_FGITEM_NONE = 0,		//	 0	特に指定なし
	ID_FGITEM_KAMOKU 		//	 1	科目
} EnumIdFgItem;


//////////////////////////////////////////////////////////////////
//	帳表全体	様式番号
typedef enum {
	ID_FORMNO_011 = 1,		//	①	
	ID_FORMNO_021,			//	②	
	ID_FORMNO_031,			//	③	
	ID_FORMNO_041,			//	④-1
	ID_FORMNO_042,			//	④-2
	ID_FORMNO_051,			//	⑤
	ID_FORMNO_061,			//	⑥
	ID_FORMNO_071,			//	⑦
	ID_FORMNO_081,			//	⑧
	ID_FORMNO_091,			//	⑨
	ID_FORMNO_101,			//	⑩-1
	ID_FORMNO_102,			//	⑩-2
	ID_FORMNO_111,			//	⑪
	ID_FORMNO_121,			//	⑫
	ID_FORMNO_131,			//	⑬
	ID_FORMNO_141,			//	⑭-1
	ID_FORMNO_142,			//	⑭-2
	ID_FORMNO_151,			//	⑮-1
	ID_FORMNO_152,			//	⑮-2
	ID_FORMNO_153,			//	⑮-3
	ID_FORMNO_161,			//	⑯-1
	ID_FORMNO_162,			//	⑯-2
	ID_FORMNO_171 = 101,	//	⑰ 1
	ID_FORMNO_172,			//	⑰ 2
	ID_FORMNO_173,			//	⑰ 3
	ID_FORMNO_174,			//	⑰ 4
	ID_FORMNO_175,			//	⑰ 5
	ID_FORMNO_176,			//	⑰ 6
	ID_FORMNO_177,			//	⑰ 7
	ID_FORMNO_178,			//	⑰ 8
	ID_FORMNO_179,			//	⑰ 9
	ID_FORMNO_1710,			//	⑰ 10
	ID_FORMNO_1711,			//	⑰ 11
	ID_FORMNO_1712,			//	⑰ 12
	ID_FORMNO_1713,			//	⑰ 13
	ID_FORMNO_1714,			//	⑰ 14
	ID_FORMNO_1715,			//	⑰ 15
	ID_FORMNO_1716,			//	⑰ 16
	ID_FORMNO_1717,			//	⑰ 17
	ID_FORMNO_1718,			//	⑰ 18
	ID_FORMNO_1719,			//	⑰ 19
	ID_FORMNO_1720,			//	⑰ 20
	ID_FORMNO_181 = 201,	//	⑱ 1
	ID_FORMNO_182,			//	⑱ 2
	ID_FORMNO_183,			//	⑱ 3
	ID_FORMNO_184,			//	⑱ 4
	ID_FORMNO_185,			//	⑱ 5
	ID_FORMNO_186,			//	⑱ 6
	ID_FORMNO_187,			//	⑱ 7
	ID_FORMNO_188,			//	⑱ 8
	ID_FORMNO_189,			//	⑱ 9
	ID_FORMNO_1810,			//	⑱ 10
	ID_FORMNO_1811,			//	⑱ 11
	ID_FORMNO_1812,			//	⑱ 12
	ID_FORMNO_1813,			//	⑱ 13
	ID_FORMNO_1814,			//	⑱ 14
	ID_FORMNO_1815,			//	⑱ 15
	ID_FORMNO_1816,			//	⑱ 16
	ID_FORMNO_1817,			//	⑱ 17
	ID_FORMNO_1818,			//	⑱ 18
	ID_FORMNO_1819,			//	⑱ 19
	ID_FORMNO_1820,			//	⑱ 20
	//ID_FORMNO_MAX			//	最大値（使用中止）
	ID_FORMNO_DUMMY			//	ダミー（使用不可）
} EnumIdFormNo;

//////////////////////////////////////////////////////////////////
//	帳表全体	様式番号
typedef enum {
	ID_FORMNO_04 = 4,
	ID_FORMNO_10 = 10,
	ID_FORMNO_14 = 14,
	ID_FORMNO_15 = 15,
	ID_FORMNO_16 = 16
} EnumIdFormNoForFormType;


//#define	UC_ID_FORMNO_MAX	(ID_FORMNO_MAX - 1)		//	様式最大数
#define	UC_ID_FORMNO_MAX		62						//	様式最大数
#define UC_ID_FORMNO_MAX_TBL7	32	// テーブルバージョン7以前の様式最大数
// midori 190301 add -->
#define	UC_ID_FORMNO_MAX_KANSO	84	// 簡素化様式を含めた最大数
// midori 190301 add <--
// インボイス登録番号追加対応_23/09/11 add -->
#define	UC_ID_FORMNO_MAX_INVO	186	// 登録番号付き様式を含めた最大数
// インボイス登録番号追加対応_23/09/11 add <--


//////////////////////////////////////////////////////////////////
//	テーブル名 定義
#define	STR_TBL_NAME_011	_T("[uc_011_yotyokin]")			//	①	
#define	STR_TBL_NAME_021	_T("[uc_021_uketoritegata]")	//	②	
#define	STR_TBL_NAME_031	_T("[uc_031_urikakekin]")		//	③	
#define	STR_TBL_NAME_041	_T("[uc_041_karibarai]")		//	④-1
#define	STR_TBL_NAME_042	_T("[uc_042_kasituke]")			//	④-2
#define	STR_TBL_NAME_051	_T("[uc_051_tanaoroshi]")		//	⑤
#define	STR_TBL_NAME_052	_T("[uc_052_tanaoroshi2]")		//	⑤(sub)
#define STR_TBL_NAME_052_TMP	_T("#row_id_tbl_rangai_052")		//	⑤(一時テーブル)
#define	STR_TBL_NAME_061	_T("[uc_061_yuukasyouken]")		//	⑥
#define	STR_TBL_NAME_071	_T("[uc_071_koteishisan]")		//	⑦
#define	STR_TBL_NAME_081	_T("[uc_081_siharaitegata]")	//	⑧
#define	STR_TBL_NAME_091	_T("[uc_091_kaikakekin]")		//	⑨
#define	STR_TBL_NAME_092	_T("[uc_092_kaikakekin2]")		//	⑨(sub)
// midori 155525_2 del -->
//#define STR_TBL_NAME_092_TMP	_T("#row_id_tbl_rangai_092")		//	⑤(一時テーブル)
// midori 155525_2 del <--
// midori 155525_2 add -->
#ifdef _DEBUG
#define STR_TBL_NAME_092_TMP	_T("##row_id_tbl_rangai_092")		//	⑤(一時テーブル)
#else
#define STR_TBL_NAME_092_TMP	_T("#row_id_tbl_rangai_092")		//	⑤(一時テーブル)
#endif
// midori 155525_2 add <--
#define	STR_TBL_NAME_101	_T("[uc_101_kariukekin]")		//	⑩-1
#define	STR_TBL_NAME_102	_T("[uc_102_gensen]")			//	⑩-2
#define	STR_TBL_NAME_111	_T("[uc_111_kariirekin]")		//	⑪
#define	STR_TBL_NAME_121	_T("[uc_121_totiuriage]")		//	⑫
#define	STR_TBL_NAME_131	_T("[uc_131_uriagedaka]")		//	⑬
#define	STR_TBL_NAME_141	_T("[uc_141_yakuin]")			//	⑭-1
#define	STR_TBL_NAME_142	_T("[uc_142_jinkenhi]")			//	⑭-2
#define	STR_TBL_NAME_151	_T("[uc_151_tidaiyatin]")		//	⑮-1
#define	STR_TBL_NAME_152	_T("[uc_152_kenrikin]")			//	⑮-2
#define	STR_TBL_NAME_153	_T("[uc_153_kougyou]")			//	⑮-3
#define	STR_TBL_NAME_161	_T("[uc_161_zatueki]")			//	⑯-1
#define	STR_TBL_NAME_162	_T("[uc_162_zatuson]")			//	⑯-2
#define	STR_TBL_NAME_171	_T("[uc_171_sonota1]")			//	⑰ 1
#define	STR_TBL_NAME_181	_T("[uc_181_sonota2]")			//	⑱ 1

#define	STR_TBL_NAME_INF_FORM_TYPE		_T("[uc_inf_form_type]")
#define	STR_TBL_NAME_INF_MAIN			_T("[uc_inf_main]")
#define	STR_TBL_NAME_INF_SONOTA_ITEM	_T("[uc_inf_sonota_item]")
#define	STR_TBL_NAME_INF_SUB			_T("[uc_inf_sub]")
#define	STR_TBL_NAME_INF_SUB_OMIT		_T("[uc_inf_sub_omit]")
#define	STR_TBL_NAME_INF_VER			_T("[uc_inf_ver]")
#define	STR_TBL_NAME_INF_YOKUKI			_T("[uc_inf_yokuki]")
#define	STR_TBL_NAME_LST_ADDRESS		_T("[uc_lst_address]")
#define	STR_TBL_NAME_LST_BANK			_T("[uc_lst_bank]")
#define	STR_TBL_NAME_LST_ITEM_SORT		_T("[uc_lst_item_sort]")
#define	STR_TBL_NAME_LST_KAMOKU			_T("[uc_lst_kamoku]")
#define	STR_TBL_NAME_LST_HOKAN			_T("[uc_lst_hokan]")
#define	STR_TBL_NAME_REN_MAIN			_T("[uc_ren_main]")
#define	STR_TBL_NAME_REN_KAMOKU			_T("[uc_ren_kamoku]")
#define	STR_TBL_NAME_REN_DAREC_TEMP		_T("[uc_ren_darec_temp]")

//////////////////////////////////////////////////////////////////
//	データテーブルにて「一括金額行(自動)」→「データ行」に戻す際の一時的な頁番号
#define	ID_DATA_PAGE_IKKATU_TEMP		10000		// 頁番号の最大値(9999) + 1

//////////////////////////////////////////////////////////////////
//	リストテーブルのユーザー登録分のシーケンス番号
#define	ID_LST_USER_START				1000000				// 開始番号

//////////////////////////////////////////////////////////////////
//	テーブルへ追加するカラム名
#define STR_CLM_NAME_SHOWKEIZERO		_T("ShowKeiZero")
#define STR_CLM_NAME_KAMOKU				_T("KnName")
#define	STR_CLM_NAME_OUT_RANGAI			_T("OutRangai")

//////////////////////////////////////////////////////////////////
//	一括金額の名称
#define NAME_IKKATU_1	_T("その他")
#define NAME_IKKATU_2	_T("件")

//	頁計/累計の名称
#define NAME_KEI		_T("計")
#define NAME_PAGEKEI	_T("頁計")
#define NAME_RUIKEI		_T("計")
#define NAME_GOUKEI		_T("合計")

//　計名称シーケンス
#define ID_NAME_KEI		0
#define ID_NAME_GOUKEI	1

//////////////////////////////////////////////////////////////////
//	自動計算種別
typedef enum {
	ID_AUTOCALC_AUTO = 0,	//	0	自動計算
	ID_AUTOCALC_INPUT		//	1	手入力
} EnumIdAutoCalc;

//////////////////////////////////////////////////////////////////
//	特殊行の括弧

//	括弧用列挙型
typedef enum {
	ID_BRACKET_NOTHING = 0,		//	なし
	ID_BRACKET_01,				//	【】	全角
	ID_BRACKET_02,				//	〔〕	全角
	ID_BRACKET_03,				//	《》	全角
	ID_BRACKET_04,				//	（）	全角
	ID_BRACKET_05,				//	［］	全角
	ID_BRACKET_06,				//	｛｝	全角
	ID_BRACKET_07,				//	〈〉	全角
	ID_BRACKET_08,				//	＜＞	全角
	ID_BRACKET_09,				//	≪≫	全角
	ID_BRACKET_10,				//	()	半角
	ID_BRACKET_11,				//	<>	半角
	ID_BRACKET_12,				//	[]	半角
	ID_BRACKET_13,				//	{}	半角
	ID_BRACKET_MAX				//	最大値
} EnumIdBracket;

//	括弧用構造体
struct tag_BracketInfo{
	CString		szLeft;		//	括弧（左）
	CString		szRight;	//	括弧（右）
};

//	括弧なし
#define NAME_BRACKET_NOTHING	_T("なし")

//	括弧用変換テーブル
// **************Attention!!**************
// DeleteKnNameBehindSyokei()内で括弧の絶対位置を使用！！
static struct tag_BracketInfo	BRACKET_KIND[] = {
	{ "",	""},		//	0	：なし
	{ "【",	"】"},		//	1	：【】	全角
	{ "〔",	"〕"},		//	2	：〔〕	全角
	{ "《",	"》"},		//	3	：《》	全角
	{ "（",	"）"},		//	4	：（）	全角
	{ "［",	"］"},		//	5	：［］	全角
	{ "｛",	"｝"},		//	6	：｛｝	全角
	{ "〈",	"〉"},		//	7	：〈〉	全角
	{ "＜",	"＞"},		//	8	：＜＞	全角
	{ "≪",	"≫"},		//	9	：≪≫	全角
	{ "(",	")"},		//	10	：()	半角
	{ "<",	">"},		//	11	：<>	半角
	{ "[",	"]"},		//	12	：[]	半角
	{ "{",	"}"}		//	13	：{}	半角
};

//////////////////////////////////////////////////////////////////
//	入力画面のタイトル入力文字数･･･全て共通
#define ID_TITLE_INPMAX			35

//////////////////////////////////////////////////////////////////
//	数値をテーブルに登録する際の限界値（DECIMAL14桁）
#define	DB_DECIMAL_MIN	(-99999999999999)
#define	DB_DECIMAL_MAX	(99999999999999)

//////////////////////////////////////////////////////////////////
//	入力画面の背景色定義
#define COLOR_DATA				RGB( 255, 255, 255 )		// データ・空行
#define COLOR_SYOKEI			RGB( 255, 240, 255 )		// 小計行
#define COLOR_CHUKEI			RGB( 255, 240, 220 )		// 中計行
#define COLOR_IKKATUMANUAL		RGB( 240, 240, 255 )		// 一括集計金額行（手動）
#define COLOR_IKKATUAUTO		RGB( 225, 225, 255 )		// 一括集計金額行（自動）
//#define COLOR_AUTOCALC_INPUT	RGB( 220, 255, 255 )		// 行内自動計算
#define COLOR_AUTOCALC_INPUT	RGB( 132, 255, 193 )		// 行内自動計算（緑：法人税申告書と同色）
#define COLOR_INVERSE			RGB( 128, 128, 128 )		// インバース行
// midori 190505 add -->
#define COLOR_KAMOKU			RGB( 220, 240, 255 )		// 科目行（見出し）
// midori 190505 add <--

// 入力画面の文字色定義
#define COLOR_CHAR_NOINVERSE	RGB( 0, 0, 0 )				// インバース行以外の行
#define COLOR_CHAR_INVERSE		RGB( 255, 255, 255 )		// インバース行
// midori 160612 add -->
#define BC_DISABLE				RGB(0xe0,0xe0,0xe0)			// 灰
// midori 160612 add <--
// No.200601 add -->
#define COLOR_AUTOCHAR_OUT		RGB(   0, 122, 204 )		// 電子申告の文字数を超えた色
// No.200601 add <--

//////////////////////////////////////////////////////////////////
//	各帳表の集計処理データ構造
#define CALCKEI_MAXCOUNT	7
typedef	struct	tag_CalcKeiInfo {
	int		intMaxCount;					// 集計フィールド件数
	BOOL	bExistNum[CALCKEI_MAXCOUNT];	// 計の０円表示/非表示フラグ TRUE:金額が入力されているので０を表示/FALSE:金額がNULLなので０を非表示
	CString	strField[CALCKEI_MAXCOUNT];		// 集計フィールド名称
	CString	strKei[CALCKEI_MAXCOUNT];		// 集計値
}	CALCKEI_INFO;

//////////////////////////////////////////////////////////////////
//	ソートで使用するソート項目情報
typedef	struct	tag_ItemSortInfo {
	BYTE	bytFgSykei;						// 小計の対象である/ない
	BYTE	bytFgItem;						// 項目の種類を示す番号
	CString	strOrderStr;					// 並び替え用SQL文字列
	CString	strFieldSykei;					// 小計のグループ判定に使用するフィールド名
	CString	strFieldSykei2;					// 小計のグループ判定に使用するフィールド名
// midori 152137 add -->
	CString	strOrderStr2;					// 50音並び替え用SQL文字列
// midori 152137 add <--
// 20-0450 add -->
	int		intItemSeq;
// 20-0450 add <--
}	ITEMSORT_INFO;

//////////////////////////////////////////////////////////////////
//	ソートで使用する科目情報
typedef	struct	tag_SortKamokuInfo {
	int		intKnSeq;						// 科目シーケンス番号
	int		intKnOrder;						// 科目並び順
	CString	strKnName;						// 科目名称
// midori 152137 add -->
	CString	strKnKana;						// 科目カナ
// midori 152137 add <--
}	SORTKAMOKU_INFO;

#define SORT_IKKATU_FIELD		"Val"		// 自動一括集計金額の対象フィールドは"Val"固定


//////////////////////////////////////////////////////////////////
//	共通モジュールからカレント情報を取得する項目情報
typedef	struct	tag_CurrentInfo {
	int		intFgFunc;						// カレント行の特殊行フラグ
	int		intNumGroup;					// カレント行のグループ番号
	int		intCopyFlag;					// 行コピー済みフラグ
	int		intFgInput;						// 自動計算フラグ
}	CURRENT_INFO;


//////////////////////////////////////////////////////////////////
//	共通データのデータ構造
typedef	struct	tag_UchiwakeInfo {
	// カレント情報
	int		intMaxPage;				// 最大頁数
	int		intCurPage;				// カレント頁番号
	int		intCurRow;				// カレント行番号
	int		intCurCol;				// カレント列番号
	int		intCurCtlIndex;			// カレントコントロールのインデックス番号
	int		intCurFocus;			// カレントフォーカス情報（1:データ入力欄にフォーカスあり，0:なし）

	// ページ構成情報
	int		intOutKei;				// 計設定内容（uc_inf_sub）
	int		intFormType;			// 出力形式設定内容（uc_inf_form_type）
	int		intOutKeiDialog;		// 計設定ダイアログのボタン指定
	int		intRowMax;				// １頁内の合計行を含む全体の行数
	int		intRowMaxData;			// １頁内のデータ行数（合計行は含まず）
	int		intColMax;				// １行内のコントロール個数	（預貯金=13）
	int		intColMaxData;			// １行内の列数				（預貯金=6）
	int		intCtlStartIndex;		// 入力コントロールの先頭インデックス（預貯金=7）
	int		intT1StartIndex;		// 合計行①の先頭インデックス（預貯金=299）
	int		intT2StartIndex;		// 合計行②の先頭インデックス（預貯金=312）
	int		intCursolDefPos;		// カーソル移動時の初期位置（タイトルから↓，合計から↑で使用）
	int		intCursolKeiPos;		// 合計行のカーソル移動位置（カーソル移動で使用）  2012/3/30計の別ダイアログ化により実質不使用

	// 
	int		OldPage;				// 行移動確認の頁番号
	int		OldRow;					// 行移動確認の行番号
	CString	OldstrDiag;				// データ入力の更新チェック用文字列
	int		OldCombo;				// データ入力の更新チェック用ComboBox値
// midori 200101 del -->
	//int		OldDateDiag;			// 日付データ入力更新チェック用の値
// midori 200101 del <--
	int		OldCol;					// 列移動確認の列番号
// 修正No.157633_元号日付 add -->
	int		OldDate[5];				// 日付データ入力更新チェック用の値
	int		OldGengo[5];			// 日付(元号番号)データ入力更新チェック用の値
	int		GenListSw;				// 
	int		EditSign;				// EditON⇔EditOFFがペアで通ってるか確認するためのサイン
// 修正No.157633_元号日付 add <--

	// 帳表情報
	int		intFormSeq;				// 様式シーケンス番号
	int		intFormNo;				// 様式番号
	int		intFormNo2;				// 様式番号枝番
	BOOL	bRenFlg;				// 財務連動実行可/不可フラグ

	// 帳表タイトル
	CString	strTitleName;			// 帳表タイトル名称
	CString strTitleNumber;			// 帳表タイトル番号（様式＋枝番）

	// 読み取り専用データ：#defineの値を設定
	int		intRowNormal;			// 通常時：１頁内の合計行を含む全体の行数
	int		intRowKobetu;			// 個別時：１頁内の合計行を含む全体の行数
	int		intT1StartNormal;		// 通常時：合計行①の先頭インデックス
	int		intT1StartKobetu;		// 個別時：合計行①の先頭インデックス
	int		intT2StartNormal;		// 通常時：合計行②の先頭インデックス
	int		intT2StartKobetu;		// 個別時：合計行②の先頭インデックス
	int		intRowKei;				// 帳表固定の合計行数（1:合計あり，0:なし）
	int		intKei1StartIndex;		// 計ダイアグラム1行目の先頭インデックス
	int		intKei2StartIndex;		// 計ダイアグラム2行目の先頭インデックス

	// その他情報
	DIAGRAM_ATTRIBUTE	uComboAttr;	// ComboBox情報のバックアップ（特殊行でCombo→Edit変換した場合に使用する）
	DIAGRAM_ATTRIBUTE	uEditAttr;	// EditBox情報のバックアップ（特殊行で日付→半/全角文字に変換する場合などに使用する)
	CALCKEI_INFO		uCalcInfo;	// 集計処理の必要情報
}	UCHIWAKE_INFO;

//////////////////////////////////////////////////////////////////
//	参照のみ＝自動登録しない場合の文字列検索用構造体
typedef	struct	tag_ReferencehInfo{
	CString	szName1;				//	銀行名／名称（上段）
	CString	szName2;				//	支店名／名称（下段）
	CString	szAdd1;					//	未使用／所在地（上段）
	CString	szAdd2;					//	未使用／所在地（下段）
}	REFERENCE_INFO;

//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//	一括で一時保管へ移動 で使用する対象帳表＆設定値
typedef struct tagHokanData{
	int					intFormSeq;			// 様式シーケンス番号
	int					blnZeroFg;			// 金額０円行を一時保管へ移動する
	int					blnNullFg;			// 金額空欄行を一時保管へ移動する
} HOKAN_DATA;

// midori 160606 add -->
//////////////////////////////////////////////////////////////////
//	金額等を数値０クリア で使用する対象帳表＆設定値
typedef struct tagClearData{
	int					intFormSeq;			// 様式シーケンス番号
	int					blnZeroFg;			// 金額０円行を一時保管へ移動する
	int					blnNullFg;			// 金額空欄行を一時保管へ移動する
} CLEAR_DATA;
// midori 160606 add <--

//////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// 内訳書データベース関連の定数定義
//
//#define	DB_STR_SQLFILE_TABLE	_T( "UcTables.sql" )			//SQLファイル名
//#define	DB_STR_SQLFILE_DATA		_T( "UcTables_InitData.sql" )	//SQLファイル名
#define	DB_STR_SQLFILE_TABLE			_T( "DBUchiwakeTables.sql" )				//作成/初期化用SQLファイル名
// midori UC_0044 add -->
#define	DB_STR_SQLFILE_TABLE1			_T( "DBUchiwakeTables1.sql" )				//作成/初期化用SQLファイル名
#define	DB_STR_SQLFILE_TABLE2			_T( "DBUchiwakeTables2.sql" )				//作成/初期化用SQLファイル名
// midori UC_0044 add <--
#define DB_STR_SQLFILE_UPDATE			_T( "DBUchiwakeUpdate.sql" )				//更新用SQLファイル名
// midori UC_0032 add -->
#define DB_STR_SQLFILE_UPDATE1			_T( "DBUchiwakeUpdate1.sql" )				//更新用SQLファイル名
#define DB_STR_SQLFILE_UPDATE2			_T( "DBUchiwakeUpdate2.sql" )				//更新用SQLファイル名
// midori UC_0032 add <--
#define DB_STR_SQLFILE_UPDATE_CONVERT	_T( "DBUchiwakeUpdateAfterConvert.sql" )	//更新用SQLファイル名
#define DB_STR_SQLUSE_TABLE				_T( "作成／初期化用" )						//作成/初期化用SQLファイルの用途
#define DB_STR_SQLUSE_UPDATE			_T( "更新用" )								//更新用SQLファイルの用途
#define	SQL_EXEC_CREATE					0
#define	SQL_EXEC_UPDPATE				1

#define	DB_STR_UNAME			_T( "ICSsa" )					//SQLサーバ：ユーザ名
//#define	DB_STR_PNAME			_T( "TbGh6398B4523qXz" )		//SQLサーバ：パスワード

// midori UC_0036 del -->
//// midori 190302 del -->
////#define DB_ICS_VERSION			2					//ICSマスターバージョン
//// midori 190302 del <--
//// midori 190302 add -->
//#define DB_ICS_VERSION			3					//ICSマスターバージョン
//// midori 190302 add <--
// midori UC_0036 del <--
// midori UC_0036 add -->
#define DB_ICS_VERSION			3					//ICSマスターバージョン
// midori UC_0036 add <--
													//(SetMasterDB()呼び出し時に使用)

// midori 161110 del -->
//// midori 160610 cor -->
////#define DB_UC_TABLE_VERSION		8				//内訳書テーブルバージョン
////												//(本アプリで使用するバージョン)
////----------------------
//#define DB_UC_TABLE_VERSION		9				//内訳書テーブルバージョン
//													//(本アプリで使用するバージョン)
//// midori 160610 cor <--
// midori 161110 del <--
// midori 161110 add -->
#define DB_UC_TABLE_VERSION		10					//内訳書テーブルバージョン
													//(本アプリで使用するバージョン)
// midori 161110 add <--

#define DB_UC_VTBL_TYPE			1001				//内訳書テーブルバージョンチェック用識別番号
#define DB_UC_VTBL_OBJNAME		_T( "DBUchiwake" )	//内訳書テーブルバージョンチェック用識別番号
#define	UCHIWAKE_FILE_PATH		_T( "\\JOBS50\\" )	// 
#define DB_UC_TABLE_CREATE_ON	0x20				//内訳書テーブル作成サイン
// midori 181002 add -->
#define DB_UC_TABLE_CREATE_ON_T	0x100000			//内訳書テーブル作成サイン（非連動）
// midori 181002 add <--
//
////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------
// ICSアプリ メッセージ パラメータ定義
//----------------------------------------------------------------
#define				ICS_MSG_WPARAM_EXIT_ERR			(1)					// 異常終了通知 CMainFrame::OnMessageClose() WPARAM の値
#define				ICS_MSG_WPARAM_EXIT_IKKATU		100					// 一括終了通知 CMainFrame::OnMessageClose() WPARAM の値

//----------------------------------------------------------------
//	ICS財務アプリケーション全体に関連する事項
//----------------------------------------------------------------
// アプリ共通：BIT定義
#define	BIT_D0				0x01		// D0 :□□□□□□□□□□□□□□□■
#define	BIT_D1				0x02		// D1 :□□□□□□□□□□□□□□■□
#define	BIT_D2				0x04		// D2 :□□□□□□□□□□□□□■□□
#define	BIT_D3				0x08		// D3 :□□□□□□□□□□□□■□□□
#define	BIT_D4				0x10		// D4 :□□□□□□□□□□□■□□□□
#define	BIT_D5				0x20		// D5 :□□□□□□□□□□■□□□□□
#define	BIT_D6				0x40		// D6 :□□□□□□□□□■□□□□□□
#define	BIT_D7				0x80		// D7 :□□□□□□□□■□□□□□□□
#define	BIT_D8				0x0100		// D8 :□□□□□□□■□□□□□□□□
#define	BIT_D9				0x0200		// D9 :□□□□□□■□□□□□□□□□
#define	BIT_D10				0x0400		// D10:□□□□□■□□□□□□□□□□
#define	BIT_D11				0x0800		// D11:□□□□■□□□□□□□□□□□
#define	BIT_D12				0x1000		// D12:□□□■□□□□□□□□□□□□
#define	BIT_D13				0x2000		// D13:□□■□□□□□□□□□□□□□
#define	BIT_D14				0x4000		// D14:□■□□□□□□□□□□□□□□
#define	BIT_D15				0x8000		// D15:■□□□□□□□□□□□□□□□
#define BIT_D16				0x01000000
#define BIT_D17				0x02000000
#define BIT_D18				0x04000000
#define BIT_D19				0x08000000
#define BIT_D20				0x10000000
#define BIT_D21				0x20000000
#define BIT_D22				0x40000000
#define BIT_D23				0x80000000

//////////////////////////////////////////////////////////////////
//	「共通設定」アプリの「設定した色を業務に使用」チェックの値
#define ID_ICS_STANDARDCOLOR_ON			1

//////////////////////////////////////////////////////////////////
// 業種No
#define STR_GYOUSHU_ZAIMU		_T( "1B" )
#define STR_GYOUSHU_KOUEKI		_T( "1E" )
#define STR_GYOUSHU_KOUEKI3		_T( "1F" )
#define STR_GYOUSHU_SHAHUKU		_T( "2E" )
#define STR_GYOUSHU_SHUKYO		_T( "29" )

//////////////////////////////////////////////////////////////////
// 文字のバイト判定（先行バイト or 後続バイト）
#define ID_CHARACTER_LEAD		0
#define ID_CHARACTER_TRAIL		-1

//////////////////////////////////////////////////////////////////
//	他マスターからコピーする設定項目情報(共通)
typedef	struct	tag_CopySettingInfoCom {
	BOOL	OutKikan;						// 印刷設定→会社名を出力する
	BOOL	OutKingaku;						// 印刷設定→金額欄を空欄で出力する
	BOOL	OutZero;						// 印刷設定→金額０円データ及び空欄データを出力しない
	BOOL	HideZero;						// 印刷設定→金額０円を印字する
}COPY_SETTING_INFO_COM;

//////////////////////////////////////////////////////////////////
//	他マスターからコピーする設定項目情報(個別)
typedef	struct	tag_CopySettingInfoInd {
	CString	Title;							// 表示するタイトル
	CString	TitleSave;						// 個別様式の保存タイトル
	CString TitleSaveTujo;					// 通常様式の保存タイトル
	BOOL	OpTitleKn;						// 印刷設定→科目名入りタイトルで表示する
	BOOL	KinyuLine;						// 印刷設定→金融機関を一段で表示する
}COPY_SETTING_INFO_IND;

//////////////////////////////////////////////////////////////////
//	様式別データ全削除
typedef struct tag_DeleteYoushiki{
	BOOL	tsujo;
	BOOL	hokan;
}DELETE_YOUSHIKI;

//////////////////////////////////////////////////////////////////
// 区分選択
#define		ID_KUBUN_SELECT_YAKUSYOKU	0
#define		ID_KUBUN_SELECT_KANKEI		1

//////////////////////////////////////////////////////////////////
// 科目同期モード
#define		ID_REFERENCE_MODE_F9		0
#define		ID_REFERENCE_MODE_COPY		1

//	役職名変換テーブル
static char* NAME_YAKU[] = {
	{ "代表取締役" },	//
	{ "常務取締役" },	//
	{ "専務取締役" },	//
	{ "取締役" },		//
	{ "監査役" },		//
	{ "有限責任社員" },	//
	{ "無限責任社員" },	//
	{ "代表社員" },		//
	{ "社員" },			//
	{ "理事長" },		//
	{ "副理事長" },		//
	{ "常務理事" },		//
	{ "専務理事" },		//
	{ "常任理事" },		//
	{ "理事" },			//
	{ "顧問" },			//
	{ "監事" },			//
	{ "その他役員" }	//
};

//	代表者との関係変換テーブル
static char* NAME_KANKEI[] = {
	{ "本人" },		//
	{ "配偶者" },	//
	{ "父" },		//
	{ "母" },		//
	{ "長男" },		//
	{ "次男" },		//
	{ "三男" },		//
	{ "長女" },		//
	{ "次女" },		//
	{ "三女" },		//
	{ "子" },		//
	{ "孫" },		//
	{ "祖父" },		//
	{ "祖母" },		//
	{ "兄弟" },		//
	{ "姉妹" },		//
	{ "婿" },		//
	{ "嫁" },		//
	{ "伯父" },		//
	{ "叔父" },		//
	{ "伯母" },		//
	{ "叔母" },		//
	{ "従兄弟" },	//
	{ "従姉妹" },	//
	{ "その他" }	//
};

// midori UC_0045 add -->
//	代表者との関係変換テーブル（簡素化用）
static char* NAME_KANKEI_K[] = {
	{ "本人" },			//
	{ "配偶者" },		//
	{ "父" },			//
	{ "母" },			//
	{ "長男" },			//
	{ "次男" },			//
	{ "三男" },			//
	{ "長女" },			//
	{ "次女" },			//
	{ "三女" },			//
	{ "子" },			//
	{ "孫" },			//
	{ "祖父" },			//
	{ "祖母" },			//
	{ "兄弟" },			//
	{ "姉妹" },			//
	{ "子の配偶者" },	//
	{ "伯父" },			//
	{ "叔父" },			//
	{ "伯母" },			//
	{ "叔母" },			//
	{ "従兄弟" },		//
	{ "従姉妹" },		//
	{ "甥" },			//
	{ "姪" },			//
	{ "その他" }		//
};
// midori UC_0045 add <--

typedef	enum {
	ID_PRT_IKKATSU		= -1,
	ID_PRT_F5			= 0
}EnumIdPrintCallBy;

// 金額フォーマット
static char	FMT12[]	= "sss,sss,sss,ss9";
// No.200903 add -->
static char	FMT13[]	= "s,sss,sss,sss,ss9";
// No.200903 add <--

// オプション設定情報
typedef	struct	tag_OptionSettingInfo {
	BOOL YoushikiSkip;				// データのある様式へ移動する
	BOOL DispIkkatDlg;				// プレビュー、印刷時に一括印刷画面を表示する
	BOOL DispYoushikiSelect;		// 起動時に様式選択画面を表示する
	BOOL EnterMoveDown;				// Enterキーでカーソルを下に移動する
// midori 160612 add -->
	BOOL DataKakutei;				// 入力データを確定する
// midori 160612 add <--
// midori 190301 add -->
	BOOL UseStyle;					// 使用する様式
// midori 190301 add <--
	BOOL UseStyle2;					// 使用する様式(登録番号)	 インボイス登録番号追加対応_23/11/06 add
// 24/01/30_23-0845 add -->
	BOOL CloudSansyoFlg;
// 24/01/30_23-0845 add <--

	tag_OptionSettingInfo()
	{
		YoushikiSkip = FALSE;
		DispIkkatDlg = FALSE;
		DispYoushikiSelect = FALSE;
		EnterMoveDown = FALSE;
// midori 160612 add -->
		DataKakutei = FALSE;
// midori 160612 add <--
// midori 190301 add -->
		UseStyle = FALSE;
// midori 190301 add <--
		UseStyle2 = FALSE;	// インボイス登録番号追加対応_23/11/06 add
// 24/01/30_23-0845 add -->
		CloudSansyoFlg = FALSE;
// 24/01/30_23-0845 add <--
	}
// midori 190301 del -->
//// midori 160612 cor -->
//	//tag_OptionSettingInfo(BOOL argYoushikiSkip, BOOL argDispIkkatDlg, BOOL argDispYoushikiSelect, BOOL argReturnMoveDown)
//	//{
//	//	YoushikiSkip = argYoushikiSkip;
//	//	DispIkkatDlg = argDispIkkatDlg;
//	//	DispYoushikiSelect = argDispYoushikiSelect;
//	//	EnterMoveDown = argReturnMoveDown;
//	//}
//// ---------------------
//	tag_OptionSettingInfo(BOOL argYoushikiSkip, BOOL argDispIkkatDlg, BOOL argDispYoushikiSelect, BOOL argReturnMoveDown, BOOL argDataKakutei)
//	{
//		YoushikiSkip = argYoushikiSkip;
//		DispIkkatDlg = argDispIkkatDlg;
//		DispYoushikiSelect = argDispYoushikiSelect;
//		EnterMoveDown = argReturnMoveDown;
//		DataKakutei = argDataKakutei;
//	}
//// midori 160612 cor <--
// midori 190301 del <--
// midori 190301 add -->
// インボイス登録番号追加対応_23/11/06 del -->
//	tag_OptionSettingInfo(BOOL argYoushikiSkip, BOOL argDispIkkatDlg, BOOL argDispYoushikiSelect, BOOL argReturnMoveDown, BOOL argDataKakutei, BOOL argUseStyle)
// インボイス登録番号追加対応_23/11/06 del <--
// インボイス登録番号追加対応_23/11/06 add -->
// 24/01/30_23-0845 cor -->
	//tag_OptionSettingInfo(BOOL argYoushikiSkip, BOOL argDispIkkatDlg, BOOL argDispYoushikiSelect, BOOL argReturnMoveDown, BOOL argDataKakutei,
	//					  BOOL argUseStyle, BOOL argUseStyle2)
// ------------------------
	tag_OptionSettingInfo(BOOL argYoushikiSkip, BOOL argDispIkkatDlg, BOOL argDispYoushikiSelect, BOOL argReturnMoveDown, BOOL argDataKakutei,
						  BOOL argUseStyle, BOOL argUseStyle2, BOOL argCloudSansyoFlg)
// 24/01/30_23-0845 cor <--
// インボイス登録番号追加対応_23/11/06 add <--
	{
		YoushikiSkip = argYoushikiSkip;
		DispIkkatDlg = argDispIkkatDlg;
		DispYoushikiSelect = argDispYoushikiSelect;
		EnterMoveDown = argReturnMoveDown;
		DataKakutei = argDataKakutei;
		UseStyle = argUseStyle;
		UseStyle2 = argUseStyle2;	// インボイス登録番号追加対応_23/11/06 add
// 24/01/30_23-0845 add -->
		CloudSansyoFlg = argCloudSansyoFlg;
// 24/01/30_23-0845 add <--
	}
// midori 190301 add <--

	tag_OptionSettingInfo(const tag_OptionSettingInfo &obj){}
}USER_OPTION_INFO;

typedef enum {
	FROM_INSERT_KEY = 0,	// 0:Insertキーからコール
	FROM_RUN				// 1:起動時に自動的にコール
} EnumIdYoushikiSelectMode;

// No.200601 add -->
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 電子帳票、文字列比較関係
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------

//	電子申告の文字数を超えたかどうか
typedef enum {
	ID_AUTOCHAR_SAFE = 0,	//	0	超えていない
	ID_AUTOCHAR_OUT			//	1	超えている
} EnumIdAutoChar;

// 電子帳票の桁数(文字数)
#define UC_BANK_LEN			11
#define UC_KAMOKU_LEN		10
// 様式①
#define UC_011_TEKI_LEN		30
// 様式②
#define UC_021_TEKI_LEN		50
// 様式③
#define UC_031_TEKI_LEN		50
// 様式④-1
#define UC_041_KANKEI_LEN	10
// 様式④-2
#define UC_042_KANKEI_LEN	10
#define UC_042_NAIYOU_LEN	40
// 様式⑤
#define UC_051_HINMOKU_LEN	20
#define UC_051_TEKI_LEN		30
// 様式⑥
#define UC_061_SYURUI_LEN	10
#define UC_061_MEIGARA_LEN	10
#define UC_061_RIYUU_LEN	10
#define UC_061_TEKI_LEN		50
// 様式⑦
#define UC_071_RIYUU_LEN	10
#define UC_071_YOUTO_LEN	10
// 様式⑧
#define UC_081_TEKI_LEN		50
// 様式⑩-1
#define UC_101_KANKEI_LEN	10
// 様式⑪
#define UC_111_KANKEI_LEN	10
#define UC_111_NAIYOU_LEN	20
// 様式⑫
#define UC_121_HDNAME_LEN	10
// 様式⑬
#define UC_131_KANKEI_LEN	10
#define UC_131_HDNAME2_LEN	15
#define UC_131_TEKI_LEN		30
// 様式⑭-1
#define UC_141_NAMETANT_LEN	10
// 様式⑮-1
#define UC_151_KUBUN_LEN	10
#define UC_151_YOUTO_LEN	10
#define UC_151_TEKI_LEN		40
// 様式⑮-2
#define UC_152_NAIYOU_LEN	30
#define UC_152_TEKI_LEN		40
// 様式⑮-3
#define UC_153_HDNAME_LEN	10
#define UC_153_TEKI_LEN		40
// 様式⑯-1
#define UC_161_NAIYOU_LEN	30
// 様式⑯-2
#define UC_162_NAIYOU_LEN	30
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------
// No.200601 add <--

// 改良No.21-0086,21-0529 add -->
// --- 電子記録債権科目追加用
// 電子記録債権
#define ADDKMK_0211_UCHINAME	_T("電子記録債権")			// 内訳書科目名称
#define ADDKMK_0211_UCHIKANA	_T("ﾃﾞﾝｼｷﾛ")				// 内訳書科目名カナ
#define ADDKMK_0211_KNAME1		_T("電子記録　債権")		// 財務小分類
#define ADDKMK_0211_KNAME2		_T("電子記録　債権")		// 財務明細
#define ADDKMK_0211_KCODE		_T("01040500")				// 財務内部コード
// 割引電子記録債権
#define ADDKMK_0212_UCHINAME	_T("割引電子記録債権")		// 内訳書科目名称
#define ADDKMK_0212_UCHIKANA	_T("ﾜﾘﾋﾞｷﾃ")				// 内訳書科目名カナ
#define ADDKMK_0212_KNAME1		_T("割引電子記録債権")		// 財務小分類
#define ADDKMK_0212_KNAME2		_T("割引　電子債権")		// 財務明細
#define ADDKMK_0212_KCODE		_T("04051a00")				// 財務内部コード
// 営業外電子記録債権
#define ADDKMK_0213_UCHINAME	_T("営業外電子記録債権")	// 内訳書科目名称
#define ADDKMK_0213_UCHIKANA	_T("ｴｲｷﾞｮｳ")				// 内訳書科目名カナ
#define ADDKMK_0213_KNAME1		_T("営業外電子記録債権")	// 財務小分類
#define ADDKMK_0213_KNAME2		_T("営業外電子債権")		// 財務明細
#define ADDKMK_0213_KCODE		_T("01071400")				// 財務内部コード
// 医業外電子記録債権
#define ADDKMK_0214_UCHINAME	_T("医業外電子記録債権")	// 内訳書科目名称
#define ADDKMK_0214_UCHIKANA	_T("ｲｷﾞｮｳｶ")				// 内訳書科目名カナ
#define ADDKMK_0214_KNAME1		_T("医業外電子記録債権")	// 財務小分類
#define ADDKMK_0214_KNAME2		_T("医業外電子債権")		// 財務明細
#define ADDKMK_0214_KCODE		_T("01071400")				// 財務内部コード
// 事業外電子記録債権
#define ADDKMK_0215_UCHINAME	_T("事業外電子記録債権")	// 内訳書科目名称
#define ADDKMK_0215_UCHIKANA	_T("ｼﾞｷﾞｮｳ")				// 内訳書科目名カナ
#define ADDKMK_0215_KNAME1		_T("事業外電子記録債権")	// 財務小分類
#define ADDKMK_0215_KNAME2		_T("事業外電子債権")		// 財務明細
#define ADDKMK_0215_KCODE		_T("01071400")				// 財務内部コード
// 電子記録債務
#define ADDKMK_0811_UCHINAME	_T("電子記録債務")			// 内訳書科目名称
#define ADDKMK_0811_UCHIKANA	_T("ﾃﾞﾝｼｷﾛ")				// 内訳書科目名カナ
#define ADDKMK_0811_KNAME1		_T("電子記録　債務")		// 財務小分類
#define ADDKMK_0811_KNAME2		_T("電子記録　債務")		// 財務明細
#define ADDKMK_0811_KCODE		_T("04010400")				// 財務内部コード
// 営業外電子記録債務
#define ADDKMK_0812_UCHINAME	_T("営業外電子記録債務")	// 内訳書科目名称
#define ADDKMK_0812_UCHIKANA	_T("ｴｲｷﾞｮｳ")				// 内訳書科目名カナ
#define ADDKMK_0812_KNAME1		_T("営業外電子記録債務")	// 財務小分類
#define ADDKMK_0812_KNAME2		_T("営業外電子債務")		// 財務明細
#define ADDKMK_0812_KCODE		_T("04051c00")				// 財務内部コード
// 医業外電子記録債務
#define ADDKMK_0813_UCHINAME	_T("医業外電子記録債務")	// 内訳書科目名称
#define ADDKMK_0813_UCHIKANA	_T("ｲｷﾞｮｳｶ")				// 内訳書科目名カナ
#define ADDKMK_0813_KNAME1		_T("医業外電子記録債務")	// 財務小分類
#define ADDKMK_0813_KNAME2		_T("医業外電子債務")		// 財務明細
#define ADDKMK_0813_KCODE		_T("04051c00")				// 財務内部コード
// 事業外電子記録債務
#define ADDKMK_0814_UCHINAME	_T("事業外電子記録債務")	// 内訳書科目名称
#define ADDKMK_0814_UCHIKANA	_T("ｼﾞｷﾞｮｳ")				// 内訳書科目名カナ
#define ADDKMK_0814_KNAME1		_T("事業外電子記録債務")	// 財務小分類
#define ADDKMK_0814_KNAME2		_T("事業外電子債務")		// 財務明細
#define ADDKMK_0814_KCODE		_T("04051c00")				// 財務内部コード
// 電子記録債権売却損
#define ADDKMK_1621_UCHINAME	_T("電子記録債権売却損")	// 内訳書科目名称
#define ADDKMK_1621_UCHIKANA	_T("ﾃﾞﾝｼｷﾛ")				// 内訳書科目名カナ
#define ADDKMK_1621_KNAME1		_T("電子記録債権売却損")	// 財務小分類
#define ADDKMK_1621_KNAME2		_T("電子債権売却損")		// 財務明細
#define ADDKMK_1621_KCODE		_T("0b020a00")				// 財務内部コード
// 改良No.21-0086,21-0529 add <--

#endif	//	__UCCOMMON_H__