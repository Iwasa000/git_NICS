/**************************************************************
	FileName : dbErrCode.h

	データベースアクセスクラス向け
	エラーコード定数
***************************************************************/
#ifndef		_DB_ERR_CODE_H_
#define		_DB_ERR_CODE_H_

// midori 190505 del -->
//#define		DB_ERR_OK		0			// 成功
//
//#define		DB_ERR_OPEN		-1			// Open()に失敗
//#define		DB_ERR_REQU		-2			// Requery()に失敗
//
//#define		DB_ERR_EOF		-100		// 該当レコードなし
//#define		DB_ERR_NULL		-101		// 該当値(フィールド値)がNULL
//#define		BD_ERR_ALL_NULL	-102		// テーブルは存在するが、レコードは存在しない
//
//#define		DB_ERR_VAL		-110		// 取得した値が正しくない
//											// 新規レコード作成時は、主キーの値が不正
//
//#define		DB_ERR_EXESQL	-200		// ExecuteSQL に失敗
//
//										// DBバージョンチェックエラー
//#define		DB_ERR_VER_OLD	-500		// バージョンが古い
//#define		DB_ERR_VER_NEW	-501		// バージョンが新しい
//
//#define		DB_ERR			-999		// 何らかのエラーが発生
//											//(通常することはない)
// midori 190505 del <--
// midori 190505 add -->
// 西暦対応で "DBNpSub8.h" をインクルードするようにしたことに伴う対応
// 重複している宣言はカットする
#define		DB_ERR_NULL		-1001		// 該当値(フィールド値)がNULL
#define		BD_ERR_ALL_NULL	-1002		// テーブルは存在するが、レコードは存在しない
#define		DB_ERR_VAL		-1003		// 取得した値が正しくない
										// 新規レコード作成時は、主キーの値が不正

#define		DB_ERR_EXESQL	-200		// ExecuteSQL に失敗
// midori 190505 add <--

#endif		//_DB_ERR_CODE_H_
