#ifndef _SYZREFUND_DEF_
#define _SYZREFUND_DEF_

#include "syzrefundrecdef.h"

// 年号
#define BKCOLORW BC_WHITE
#define BKCOLORG BC_GRAY
#define EDCOLOR  BC_CREAM
#define ZICOLOR  BC_BLUE
#define CLCOLOR  BC_GREEN
#define HPCOLOR  BC_CREAM	//	帳票色

#define ARRAYCOUNT( _array ) (sizeof(_array) / sizeof(_array[0]))

// 各項目の行数
#define HOJIN1_LINE_CNT 10	// 法人 課税
#define HOJIN2_LINE_CNT 10	// 法人 輸出
#define HOJIN3_LINE_CNT  5	// 法人 棚卸
#define HOJIN4_LINE_CNT 10	// 法人 固定
#define HOJIN_SHIRE_CNT 10	// 法人 仕入

#define KOJIN1_LINE_CNT  5	// 個人 課税
#define KOJIN2_LINE_CNT  5	// 個人 輸出
#define KOJIN3_LINE_CNT  5	// 個人 棚卸
#define KOJIN4_LINE_CNT  5	// 個人 固定
#define KOJIN_SHIRE_CNT 13	// 個人 仕入

#define NO_MOVE   -1
#define MOVE_PREV -2
#define MOVE_NEXT -3


// 各項目の桁数
#define KPSsLN 16  // 資産の種類等
#define KPSnLN 20  // 取引先住所
#define KPSaLN 30  // 取引先住所 

#define KPStLN 20  // 主な取引商品
#define KPSzLN 16  // 所轄税関名

#define KPEbnLN 20 // 銀行名
#define KPEstLN 20 // 支店名
#define KPEykLN 8  // 預金種別

#define KPEsnLN 40 // 主な通関業者　氏名
#define KPEaLN  40 // 主な通関業者　住所

#define KPSpLN 120 // 特殊事情

#define KPRrLN 40 // その他の理由


//----> 20230704
// CShinRefundHjn02EX用　ダイアグインデックス
#define HS_KOMI			59	// 仕入金額　決算額　税込ラジオボタン
#define HS_NUKI			60	// 仕入金額　決算額　税抜ラジオボタン
#define HS1_KSN			61	// 仕入金額　1行目　決算額
#define HS8_KGK			84	// 仕入金額　8行目　取得価額
#define HS8_NOSIIRE		85	// 仕入金額　8行目　イのうち課税仕入れにならないもの
#define HS10_KZSIIRE	90	// 仕入金額　10行目　課税仕入高

#define HT_CLMCNT		7	// 棚卸資産　カラム数
#define HT_TCHK			3	// 棚卸資産　取得金額　取引金額等チェック
#define HT1_TYPE		44	// 棚卸資産　1行目　資産の種類
#define HT1_NAME		49	// 棚卸資産　1行目　取引先氏名
#define HT1_ADDR		50	// 棚卸資産　1行目　取引先住所
#define HT2_TYPE		51	// 棚卸資産　2行目　資産の種類
#define HT4_ADDR		71	// 棚卸資産　4行目　取引先住所
#define HT5_TYPE		72	// 棚卸資産　5行目　資産の種類
#define HT5_YEAR		73	// 棚卸資産　5行目　取得年月日
#define HT5_YEARKZK		74	// 棚卸資産　5行目　取得年月日継続チェック
#define HT5_MNY			75	// 棚卸資産　5行目　取得金額
#define HT5_INVNO		76	// 棚卸資産　5行目　登録番号
#define HT5_NAME		77	// 棚卸資産　5行目　取引先氏名
#define HT5_ADDR		78	// 棚卸資産　5行目　取引先住所

#define HK_CLMCNT		6	// 固定資産　カラム数
#define HK_TCHK			3	// 固定資産　取得金額　取引金額等チェック
#define HK1_TYPE		79	// 固定資産　1行目　資産の種類
#define HK1_YEAR		80	// 固定資産　1行目　取得年月日
#define HK1_INVNO		82	// 固定資産　1行目　登録番号
#define HK1_NAME		83	// 固定資産　1行目　取引先氏名
#define HK1_ADDR		84	// 固定資産　1行目　取引先住所
#define HK2_TYPE		85	// 固定資産　2行目　資産の種類
#define HK9_ADDR		132	// 固定資産　9行目　取引先住所
#define HK10_TYPE		133	// 固定資産　10行目　資産の種類
#define HK10_YEAR		134	// 固定資産　10行目　取得年月日
#define HK10_MNY		135	// 固定資産　10行目　取得金額
#define HK10_INVNO		136	// 固定資産　10行目　登録番号
#define HK10_NAME		137	// 固定資産　10行目　取引先氏名
#define HK10_ADDR		138	// 固定資産　10行目　取引先住所

#define HT_INP1			1	// 特殊事情　20230704　いるのか？
#define HT_INP2			6	// 特殊事情　入力欄


// CShinRefundKjn02EX用　ダイアグインデックス
#define KS_KOMI			71	// 仕入金額　決算額　税込ラジオボタン
#define KS_NUKI			72	// 仕入金額　決算額　税抜ラジオボタン
#define KS_SYOTOKU		94	// 仕入金額　〇〇所得
#define KS10_KSN		101	// 仕入金額　10行目　決算額
#define KS10_NOSIIRE	102	// 仕入金額　10行目　左のうち課税仕入れにならないもの
#define KS13_KZSIIRE	108	// 仕入金額　13行目　課税仕入高

#define KT_CLMCNT		7	// 棚卸資産　カラム数
#define KT_TCHK			3	// 棚卸資産　取得金額　取引金額等チェック
#define KT1_TYPE		44	// 棚卸資産　1行目　資産の種類
#define KT1_NAME		49	// 棚卸資産　1行目　取引先氏名
#define KT1_ADDR		50	// 棚卸資産　1行目　取引先住所
#define KT2_TYPE		51	// 棚卸資産　2行目　資産の種類
#define KT4_ADDR		71	// 棚卸資産　4行目　取引先住所
#define KT5_TYPE		72	// 棚卸資産　5行目　資産の種類
#define KT5_YEAR		73	// 棚卸資産　5行目　取得年月日
#define KT5_YEARKZK		74	// 棚卸資産　5行目　取得年月日継続チェック
#define KT5_MNY			75	// 棚卸資産　5行目　取得金額
#define KT5_INVNO		76	// 棚卸資産　5行目　登録番号
#define KT5_NAME		77	// 棚卸資産　5行目　取引先氏名
#define KT5_ADDR		78	// 棚卸資産　5行目　取引先住所

#define KK_CLMCNT		6	// 固定資産　カラム数
#define KK_TCHK			3	// 固定資産　取得金額　取引金額等チェック
#define KK1_TYPE		44	// 固定資産　1行目　資産の種類
#define KK1_YEAR		45	// 固定資産　1行目　取得年月日
#define KK1_INVNO		47	// 固定資産　1行目　登録番号
#define KK1_NAME		48	// 固定資産　1行目　取引先氏名
#define KK1_ADDR		49	// 固定資産　1行目　取引先住所
#define KK2_TYPE		50	// 固定資産　2行目　資産の種類
#define KK4_ADDR		67	// 固定資産　4行目　取引先住所
#define KK5_TYPE		68	// 固定資産　5行目　資産の種類
#define KK5_YEAR		69	// 固定資産　5行目　取得年月日
#define KK5_MNY			70	// 固定資産　5行目　取得金額
#define KK5_INVNO		71	// 固定資産　5行目　登録番号
#define KK5_NAME		72	// 固定資産　5行目　取引先氏名
#define KK5_ADDR		73	// 固定資産　5行目　取引先住所

#define KT_INP1			1	// 特殊事情
#define KT_INP2			6	// 特殊事情　入力欄
//<---- 20230704

#endif