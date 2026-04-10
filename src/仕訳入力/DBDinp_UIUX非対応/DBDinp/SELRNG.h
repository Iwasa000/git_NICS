#ifdef REGDATA
# ifndef _SELRNG_H_
# define _SELRNG_H_

// SELRNG.h : ヘッダー ファイル
//

// 範囲データ
struct _tagSELRNG {
	int				m_NowPos;		// 現在入力ポジション				0 ... 開始		1 ... 終了
	BOOL			m_bInpSg[2];	// 開始/終了 番号 入力済みサイン	FALSE ... 未	TRUE ... 済
	unsigned int	m_Num[2];		// 開始/終了 番号
};

/////////////////////////////////////////////////////////////////////////////
// CSELRNG ウィンドウ

class CSELRNG //: public CWnd
{
// コンストラクション
public:
	int rev( void );
	int order( void );
	int GetNxtPos( void );
	BOOL GetEndNum();
	BOOL GetStartNum();
	BOOL GetEndNum( int& Num );
	BOOL GetStartNum( int& Num );
	void SetPos( int Pos );
	int GetNowPos( void );
	void DelNum( int Pos );
	void SetNum( int Pos, int Num );
	void Inz( void );
	CSELRNG();

// アトリビュート
public:
protected:
	struct _tagSELRNG m_SELRNG;		// 選択データ

// オペレーション
public:

/*
// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CSELRNG)
	//}}AFX_VIRTUAL
*/

// インプリメンテーション
public:
	virtual ~CSELRNG();

/*
	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CSELRNG)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
*/
};

/////////////////////////////////////////////////////////////////////////////
# endif	// end of _SELRNG_H_
#endif // end of REGDATA