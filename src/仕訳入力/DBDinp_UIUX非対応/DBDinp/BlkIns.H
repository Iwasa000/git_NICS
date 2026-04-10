
#ifdef _BLK_DATA_
# ifndef _BLKINS_H_
# define _BLKINS_H_

#define _BLKINS_1	// 07.29 /08
#ifdef _BLKINS_1	// _BLKINS_1 を設定すること無しに _BLKINS_2 を設定してはならない
# define _BLKINS_2	// 07.31 /08
#endif

#ifdef _BLKINS_2	// 08.05 /08
#include "CRC.h"
#endif

#pragma once

//////////////////////////////////////////////////
//	ＢＬＫ ＩＮＳＥＲＴ を使用した一括データ追加
//		2008年7月4日
//////////////////////////////////////////////////

/*	<< コンパイルの注意 >>
	プリプロセッサに _BLK_DATA_ を定義してください。


	<< 使用例 >>
	仕訳追加部分において －－－

	CBlkIns Blk;										// クラスの作成

	Blk.Iniz( CDBZmSub*, "ワークディレクトリパス" );	// 初期化：財務クラスとワークディレクトリパスのポインタを渡す。


	for( データ追加 ループ ) {
			・
			・
			・
		Blk.PutBlkData( CDBDATA* );		// 財務クラスの仕訳Append の替わりにこの関数を発行して、
										//  BLKINSERT 用ファイルを"ワークディレクトリパス"内に作成してゆきます。
										//
										// ・(注)CDBDATA には追加データをセットしておいてください。
										// ・データのＳＥＱ番号は当関数でセットします。
										// ・また、データの書き込みが終わるまで dcntl の仕訳数はカウント
										// 　されませんので、開始時以外の仕訳数は実仕訳数としては使えません。
			・
			・
			・
	}	// データ追加 ループ の終端

	Blk.WrtBlkData();					// BLKINSERT 用ファイルを使いデータの一括追加を行います。
										// ・dcntl の実仕訳数もセットします。
			・
			・
	－－－ 以上でデータの追加が終わります。

	・なお、残高への集計及び枝番登録は行われていませんので、db_recalc_ex( ) を使用した再集計が必要です。
	・また、財務クラス->dbdata->BeginTransLock(),財務クラス->dbdata->CommitTransUnLock() のロック制御は
	　エラーとなりますので、はずしてください。

	・当クラス関数の返送値がエラー( -1 )の場合、クラスメンバ m_ErrMsg にエラーメッセージがセットされています。
*/

# ifdef _BLK_DATA2_	// 07.04 /08
#define DATA_WRITE_CNT	5000
# endif

//#define BLK_BUF_SIZE	(1024*300)

// CBlkIns コマンド ターゲット
class CBlkIns //: public CDatabase
{
//	DECLARE_DYNAMIC(CBlkIns)

public:
	CBlkIns();
	virtual ~CBlkIns();


	// エラーメッセージ
	CString m_ErrMsg;

	// 初期登録関数
	int Iniz(CDBZmSub* zm, LPCTSTR wpath );
	// バルクインサート用データ1仕訳分作成
	int PutBlkData( CDBDATA* drec );
	// 仕訳バルクインサート
	int WrtBlkData( void );

protected:

	// 登録数
	long RecCnt( int tblofs );
	// データコントロールデータのゲット
	CDCNTL* gdcntl( int tblofs );


	// クローズ
	void EndClose(void);
	// バッファクリア
	void BufClr( void );
	// 仕訳データバルクインサート用1データ変換
	CString MkBlkData( CDBDATA* drec );
#ifdef _BLK_DATA2_	// 07.04 /08
	// 仕訳データバルクインサート
	int BlkInsData( void );
#endif

	// ワークディレクトリパス
	CString m_wpath;
#ifdef _BLKINS_1	// 07.29 /08
	// ワークファイル名
	CString m_wfname;
#endif
	// ワークＦＵＬＬパス
	CString m_wfpath;
	// 財務クラスポインタ
	CDBZmSub* m_Zmp;
	// データベースポインタ
	CDatabase* m_db;
	// 親ウィンドウ
	CWnd* m_pParent;

	// seq カウンター
	DWORD m_seq;

	// バルクインサート用ファイルハンドル
	int m_fd;
	//// フラッシュバッファ
	//BYTE* m_wrtbf;
	//DWORD m_wbfpn;
# ifdef _BLK_DATA2_	// 07.04 /08
	long m_wrtpn;
# endif

#ifdef _BLKINS_1	// 07.29 /08
# ifndef _BLKINS_2	// 07.31 /08
	BOOL m_share;						// 共有済みサイン
	char m_uni_globalfolda[MAX_PATH];	// 削除対象の共有名
# endif
# ifdef _BLKINS_2	// 08.04 /08
	// CRC算出		// 08.05 /08
	CCRC m_CRC; 
	// データベース側マシン名
	CString m_machine;
	CString m_sqlsvr;
	CString m_apno;
	CString m_cocode;
	CString m_kikan;
# endif
#endif

// 10.08 /08
	// 内部伝票番号
	long m_icno;
	long m_set_icno;

public:
	static CString pcom_sqlsvr;
	static CString pcom_apno;
	static CString pcom_cocode;
	static CString pcom_kikan;

};
# endif	// End of _BLKINS_H_
#endif	// End of_BLK_DATA_
