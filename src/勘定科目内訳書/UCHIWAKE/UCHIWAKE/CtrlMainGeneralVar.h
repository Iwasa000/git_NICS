#pragma once
#include "dbUcinfMain.h"

class CCtrlMainGeneralVar
{
public:
	BOOL	bKanaDone;				// 科目名等にふりがなを振ったかどうか　立てたら立ちっぱなし
	BOOL	bHokanDataMente;		// データテーブル(dbUc○○○)と保管テーブルで同期がとれてなかった際のメンテナンスフラグ　一度立てたら折らない
	BOOL	bDispIkkatDlg;			// オプション設定::プレビュー、印刷押下時に一括印刷ダイアログを表示するか
	BOOL	bYoushikiSkip;			// 様式にデータ行がある様式に飛ぶ／全て空行でも飛ぶ (TRUE:データのある様式へ、FALSE:無条件に前後の様式へ)
	BOOL	bCopyOwnData;			// 内訳書をよく知らない時代にowntblにデータを持ってしまっていた。owntblのデータを内訳書テーブルにコピーしたかどうか　一度立ったら立ったまま
	// これ以降頭にDoneのつく変数は一度立てたら二度と折らないフラグ
	BOOL	bDoneUpdate1_0_2_1;		// 共通だった「金額０円データ及び空欄データを表示しない」を個別に持たせる為、コピーしたかどうか and ④－２、⑪、⑯－１、⑯－２の中でF9参照と一致するものはAdSeq,AdOrderを更新
	BOOL	bDispYoushikiSelect;	// オプション設定::起動時に様式選択画面を表示する
	BOOL	bReturnMoveDown;		// オプション設定::Enterキーでカーソルを下に移動する

	BOOL	bRegAutoRef;			// 参照ダイアログ::金融機関、取引先データを自動で登録する
// midori 150892 add -->
	BOOL	bMakeUpMethod;			// 財務連動ダイアログ→出力期間の指定→集計方法
// midori 150892 add <--

// midori 160612 add -->
	BOOL	bDataKakutei;			// 入力データを確定する
// midori 160612 add <--
// midori 161103 add -->
	BOOL	bZenFlg;				// 前年度からの更新サイン	0:更新無し	1:更新あり
// midori 161103 add <--
// midori 152135,152136 add -->
	BOOL	bBankSort;				// 参照ダイアログ::表示順（金融機関）	0:番号順	1:50音順
	BOOL	bAddressSort;			// 参照ダイアログ::表示順（取引先）		0:番号順	1:50音順
// midori 152135,152136 add <--
// midori 152745 add -->
	BOOL	bAddressSort2;			// 参照ダイアログ::表示順（取引先）得意先		0:番号順	1:50音順
// midori 152745 add <--
// midori UC_0004 add -->
	BOOL	bUpKakutei;				// 入力確定更新サインにする		0:更新する	1:更新しない
// midori UC_0004 add <--
// midori 190301 add -->
	BOOL	bUseStyle;				// 使用する様式				TRUE:新様式(簡素化)		FALSE:旧様式
// midori 190301 add <--
// midori 190504 add -->
	BOOL	bMstUpd;				// マスター更新サイン(改良依頼No.190505 データベース修正) 1:更新済み
// midori 190504 add <--
// midori 155314 del -->
	BOOL	bOmitUpd;				// 印刷設定 省略文字 修復サイン		1:修復済み
// midori 155314 del <--
// midori 191002 add -->
	BOOL	bKeiUpd;				// 小計名称更新サイン		1:更新済み
// midori 191002 add <--
// midori 157043 del -->
//// midori 156186 add -->
//	BOOL	bKmkRowChk;				// 画面表示「科目行」対応後、科目行処理方法変更メッセージ表示サイン　1:メッセージ表示済み　[勘定科目内訳書修正依頼20200611.xlsx] No.156186
//// midori 156186 add <--
// midori 157043 del <--
// midori 156188_3 add -->
	BOOL	bKmkRowCnv;				// 画面表示「科目行」対応後、科目行挿入済みサイン　　　　　　　　　　1:科目行挿入済み　　　[勘定科目内訳書修正依頼20200611.xlsx] No.156188
// midori 156188_3 add <--
// midori UC_0060 del -->
	BOOL	bOmitUpd2;				// 印刷設定 省略文字 修復サイン２		1:修復済み
// midori UC_0060 del <--
	BOOL	bUseStyle2;				// 使用する様式（登録番号版）	TRUE:新様式(登録番号追加版)	FALSE:旧様式(簡素化or旧)

	CCtrlMainGeneralVar(const CdbUcInfMain& rc);
	void Set(CdbUcInfMain& rc);
};
