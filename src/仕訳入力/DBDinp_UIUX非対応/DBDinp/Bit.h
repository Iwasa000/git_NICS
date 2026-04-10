// Bit.h : ヘッダー ファイル
//

#ifndef ON
# define ON 1
#endif
#ifndef OFF
# define OFF 0
#endif

/////////////////////////////////////////////////////////////////////////////
//

class CBit
{
// コンストラクション
public:
	CBit();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CBit)
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	unsigned long m_Max;	//ビットマックス
	unsigned long m_bMax;	//バイトマックス
//	unsigned char *m_pTbl;	//ビットテーブル
	CByteArray	  m_ByArray;

public:
	virtual ~CBit();

	//ビットテーブル作成
	int mk_bitbl( unsigned long max, BOOL fill_sg );

	//ビットテーブル削除
	int del_bitbl();

	//ビットテーブルを ON または OFF でイニシャライズ
	void fill( int sw );

	// bitn番目(0から始まる)のビット状態を返す
	int bitncnd( unsigned long bitn );

	// bitn番目(0から始まる)のビットを ON とする
	int onbitn( unsigned long bitn );

	// bitn番目(0から始まる)のビットを OFF とする
	int offbitn( unsigned long bitn );

	// n 番目( 1 から始まる)の ON ビットのビット番号( 0 から始まる)を返す
	int geton_bitn( unsigned long n );

	// n ビット番目(０から始まる)位置の ON ビット番目(１から始まる)を返す
	int geton_bitno( unsigned long n );

protected:
	// 1 char の ON ビットを数える。
	int onbit_n( unsigned char c );

	//1char中の n ON ビット番目( 1 から始まる) のビット番号を返す
	int onbit_no( unsigned char c, unsigned long n );

	//ビットテーブル拡張
	int extend_bitbl( int max );

};

/////////////////////////////////////////////////////////////////////////////
