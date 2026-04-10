//
//　検索データ
//
#ifndef _SCANDATA_H_
#define _SCANDATA_H_

struct _dbscan_data_ {
	int rno,
		max,
		vect,
		n;						// 検出総数
	int		seq;

	int		start_no;	// 範囲開始番号（０～）
	int		end_no;		// 範囲終了番号（０～）

	BOOL dsp_sw;				// 表示ＳＷ. （表示が行われれば表示側でＴＲＵＥ）
	BOOL inz;					// 初回送付サイン（受け手でＯＦＦにする）
	int end_sg;					// スレッド終了サイン　１…正常終了　－1…abort
	BOOL msg_send;				// メッセージ送付サイン

	int	found_seq;	// 検索目標ＳＥＱ番号
	BOOL continue_sw;			// 継続検索ＳＷ
	BOOL continue_msg_send;		// 継続検索でのメッセージ送付サイン
	BOOL found_count_use;		// 表示最大数カウント許可サイン
	int	found_max_count;		// 現在表示最大数
	int found_n;				// 検出データ読み出し数

	int message;		// 送付メッセージ
	int id_no;
	CWnd* pCWnd;		// 送付先
};

//
// データレコード番号ﾌｧｲﾙ／メモリーポインタ
struct _MFP {
	int fp;				// ワークﾌｧｲﾙポインタ
	int *precn;			// ワークメモリーポインタ
};

struct _dbscan2_data {
	int end_sg;		// スレッド終了サイン　１･･･正常終了　－１･･･abort
	int n;
	struct _MFP fp;
//	int fp;			// file no
//	CFile *fp;		// fileNo
};

#endif	// end of _SCANDATA_H_