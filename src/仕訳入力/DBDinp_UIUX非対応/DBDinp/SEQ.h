

// SEQ.h : ヘッダー ファイル
//
#ifndef _INCLUDE_SEQ_H_
#define	_INCLUDE_SEQ_H_

/*
#ifndef ON
# define ON 1
#endif
#ifndef OFF
# define OFF 0
#endif
*/
/////////////////////////////////////////////////////////////////////////////
//

class CSEQ
{
// コンストラクション
public:
	CSEQ();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CSEQ)
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	long m_Max;	//テーブルマックス
	long m_bMax;	//バイトマックス
	long m_nMax;	//セットポジション最大値
	CDWordArray	m_DwArray;

#ifdef DB_OLD_CUT
	WORD *m_pTbl;			//ＳＥＱテーブル
#endif

public:
	virtual ~CSEQ();

	//ＳＥＱテーブル作成
	int mk_seqtbl( long max );

	//ＳＥＱテーブル削除
	int del_seqtbl();

	//ＳＥＱテーブルをイニシャライズ
	void fill( void );

	// n番目(0から始まる)のSEQ番号を返す
	int ngetseq( long n );

	// n番目(0から始まる)にＳＥＱ番号をセットする
	int nsetseq( long seq, long n );

protected:
};


#endif // _INCLUDE_SEQ_H_