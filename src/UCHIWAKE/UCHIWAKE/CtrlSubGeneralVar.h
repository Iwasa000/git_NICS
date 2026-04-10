#pragma once
#include "dbUcInfSub.h"

// uc_inf_subの汎用変数のコントロールクラス
class CCtrlSubGeneralVar		
{
public:
	BOOL bKinyuLine;			// 印刷設定ダイアログ→「金融機関を一段で表示する」
	BOOL bDfltRendoKikan;		// 財務連動ダイアログ→「金額のみ再連動する」 0 :デフォルト状態　／ 1 : 非デフォルト状態
// midori 150892 del -->
	//BOOL bMakeUpMethod;			// 財務連動ダイアログ→出力期間の指定→集計方法
// midori 150892 del <--
	BOOL bAddHokan;				// 既存の保管データを保管テーブルにコピーした？　→問題があったため、リリース後潰す詳細はMaintenanceHokanData内にて
// midori 157099,157119 del -->
//// midori 190505 del -->
////	BOOL bOutKamokuRow;			// 印刷設定ダイアログ→「科目行を出力する」
//// midori 190505 del <--
//// midori 190505 add -->
//	BOOL bOutKamokuRow;			// ※今後未使用（最初にメッセージを出すためのチェックに使用して今後参照しません）印刷設定ダイアログ→「科目行を出力する」
//// midori 190505 add <--
// midori 157099,157119 del <--
// midori 157099,157119 add -->
	BOOL bOutKamokuRow;			// 印刷設定ダイアログ→「科目行を出力する」
// midori 157099,157119 add <--
	BOOL bIkkatuOutZero;		// ソートダイアログ→「一括金額に０円を含まない」」
	BOOL bNameKei;				// 合計設定ダイアログ→「計名称」
	// これ以降頭にDoneのつく変数は一度立てたら二度と折らないフラグ
	BOOL bPrintZeroData;		// 印刷設定ダイアログ→「金額０円データを表示しない」
	BOOL bPrintNullData;		// 印刷設定ダイアログ→「金額空欄データを表示しない」
	BOOL bPrintHideZero;		// 印刷設定ダイアログ→「金額０円を印字する」
	BOOL bUseAdvSetting;		// ソートダイアログ→「詳細金額を使用する」
// midori 152135,152136 add -->
	BOOL bKamokuSort;			// 参照ダイアログ→「表示順（科目）」
// midori 152135,152136 add <--
// midori 190505 add -->
// midori 157042 del -->
//	BOOL bKamokuTitle;			// ソートダイアログ→「科目行を出力する」
// midori 157042 del <--
// midori 157099,157119 add -->
	BOOL bKamokuTitle;			// ソートダイアログ→「科目行を挿入する」
// midori 157099,157119 add <--

	BOOL bPrnHoho1;				// 確認ダイアログ→「出力方法」1=金額０円(空欄)データを削除した分、詰めて出力
	BOOL bPrnHoho2;				// 確認ダイアログ→「出力方法」1=金額０円(空欄)データを削除するが、画面上で行った改頁を保って出力
// midori 20/10/08u del -->
	//BOOL bPrnHoho3;				// 確認ダイアログ→「出力方法」1=編集－ソート設定の第１ソート項目でソートを行い、改頁して出力
// midori 20/10/08u del <--
// midori 190505 add <--
// No.200602 add -->
	BOOL bKanryakuSw;			// 保管参照→「全項目を１画面に表示する」
// No.200602 add <--

	CCtrlSubGeneralVar(const CdbUcInfSub& rc);
	void Set(CdbUcInfSub& rc);
};
