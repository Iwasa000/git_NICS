// SELRNG.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#ifdef REGDATA
//#include "Dinp.h"
#include "SELRNG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSELRNG

CSELRNG::CSELRNG()
{
	this->Inz();
}

CSELRNG::~CSELRNG()
{
}


/*
BEGIN_MESSAGE_MAP(CSELRNG, CWnd)
	//{{AFX_MSG_MAP(CSELRNG)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
*/

/////////////////////////////////////////////////////////////////////////////
// CSELRNG メッセージ ハンドラ
#define StartPos	0
#define EndPos		1
//
// クラスイニシャライズ
void CSELRNG::Inz( void )
{
	// 範囲データ イニシャライズ
	m_SELRNG.m_NowPos = 0;												// 開始ポジションから
	m_SELRNG.m_bInpSg[StartPos] = m_SELRNG.m_bInpSg[EndPos] = FALSE;	// 開始/終了 未設定
	m_SELRNG.m_Num[StartPos] = m_SELRNG.m_Num[EndPos] = 0;
}

//
// 番号セット
void CSELRNG::SetNum( int Pos, int Num )
{
	m_SELRNG.m_Num[Pos] = Num;
	m_SELRNG.m_bInpSg[Pos] = TRUE;
}

//
// 番号消去
void CSELRNG::DelNum( int Pos )
{
	m_SELRNG.m_Num[Pos] = 0;
	m_SELRNG.m_bInpSg[Pos] = FALSE;
}

//
// 現在ポジション
int CSELRNG::GetNowPos( void )
{
	return m_SELRNG.m_NowPos;
}

//
// ポジション設定
void CSELRNG::SetPos( int Pos )
{
	m_SELRNG.m_NowPos = Pos;
}

//
// 次のポジション読み出し
// -> 次のポジション(-1：終了)
int CSELRNG::GetNxtPos( void )
{
	switch( m_SELRNG.m_NowPos )		// 現在入力ポジション
	{
	case StartPos: return EndPos;
	default: return -1;
	}
}

//
// 開始番号読み出し
// -> 入力有無サイン
BOOL CSELRNG::GetStartNum( int& Num )
{
	Num = m_SELRNG.m_Num[StartPos];
	return GetStartNum();
}

//
// 終了番号読み出し
// -> 入力有無サイン
BOOL CSELRNG::GetEndNum( int& Num )
{
	Num = m_SELRNG.m_Num[EndPos];
	return GetEndNum();
}

//
// 開始番号状態読み出し
// -> 入力有無サイン
BOOL CSELRNG::GetStartNum(  )
{
	return m_SELRNG.m_bInpSg[StartPos];
}

//
// 終了番号状態読み出し
// -> 入力有無サイン
BOOL CSELRNG::GetEndNum(  )
{
	return m_SELRNG.m_bInpSg[EndPos];
}

//
// 順序チェック
// ->	0 ... 同じ、または片方のみ、又は両方未入力
//		1 ... 正順：小→大
//		-1 .. 逆順：大→小
int CSELRNG::order( )
{
	unsigned int start_num = m_SELRNG.m_Num[StartPos];
	unsigned int end_num = m_SELRNG.m_Num[EndPos];
	BOOL start_sg = m_SELRNG.m_bInpSg[StartPos];
	BOOL end_sg = m_SELRNG.m_bInpSg[EndPos];
	
	if( start_sg && end_sg )
	{
		if( start_num < end_num )
			return 1;	// 正順
		else if( start_num > end_num )
			return -1;	// 逆順
	}
	return 0;
}

//
// 順序反転
// ->反転結果が	0 ... 同じ、または片方のみ、又は両方未入力
//				1 ... 正順：小→大
//				-1 .. 逆順：大→小
int CSELRNG::rev( )
{
	unsigned int	*start_num	=	&m_SELRNG.m_Num[StartPos];
	unsigned int	*end_num	=	&m_SELRNG.m_Num[EndPos];
	BOOL			*start_sg	=	&m_SELRNG.m_bInpSg[StartPos];
	BOOL			*end_sg		=	&m_SELRNG.m_bInpSg[EndPos];
	int sv;
	
	// 値
	sv = *start_num;
	*start_num = *end_num;
	*end_num = sv;

	// サイン
	sv = *start_sg;
	*start_sg = *end_sg;
	*end_sg = sv;

	return this->order();	// 結果並び
}

#endif
