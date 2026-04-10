// Bit.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"

#include "Bit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static unsigned char __msk[8] = {
	0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01
};
//char 1 バイト中のbitn番目(0から始まる)の ON マスクchar値を返す
#define onmask( bitn ) __msk[bitn]
//bitn番目(0から始まる)のchar ポインタをリード
//#define adr( bitn ) (m_pTbl + bitn / 8)
#define adr( bitn ) ((&m_ByArray[0]) + bitn / 8)
//bitn番目(0から始まる)のchar 1バイト中のビット番号(0から始まる)を返す
#define rem( bitn ) (bitn % 8)

#include <afxmt.h>
/////////////////////////////////////////////////////////////////////////////
// CBit

CBit::CBit()
{
//	m_pTbl = 0;
	m_Max = m_bMax = 0;
}

CBit::~CBit()
{
//	if( m_pTbl )
//		free( m_pTbl );
	m_ByArray.RemoveAll();
}

/////////////////////////////////////////////////////////////////////////////
// CBit ハンドラ

//ビットテーブル作成
int CBit::mk_bitbl( unsigned long max, BOOL fill_sg )
{
	if( max )
	{
	//	m_pTbl = (unsigned char *)malloc( m_bMax = (max + 7) / 8 );
	//	if( m_pTbl )
	//	{
	//		m_Max = max;
	//		this->fill( fill_sg );
	//		return( 0 );	//OK
	//	}

		try {
			m_bMax = (max + 7) / 8;
			m_Max = max;
			m_ByArray.SetSize( m_bMax );
			this->fill( fill_sg );
			return 0;
		}
		catch( CMemoryException* me )
		{
			me->Delete();
			return -1;
		}
	}
	return( -1 );	//ERROR
}

//ビットテーブル削除
int CBit::del_bitbl()
{
	m_ByArray.RemoveAll();
	m_Max = m_bMax = 0;
	return 0;

#ifdef DB_OLD_CLOSE
	if( m_pTbl )
	{
		delete m_pTbl;

		m_pTbl = 0;
		m_Max = m_bMax = 0;
		return 0;	//OK
	}
	return -1;	//ERROR
#endif
}

//ビットテーブルを ON または OFF でイニシャライズ
void CBit::fill( int sw )
{
//	memset( m_pTbl, (sw ? '\xff' : '\0'), m_bMax );
	memset( &m_ByArray[0], (sw ? '\xff' : '\0'), m_bMax );
}

#ifdef MUTEX	// 05.23 /00
static CMutex bitncnd_criticalSection;
#else
static CCriticalSection bitncnd_criticalSection;
#endif
// bitn番目(0から始まる)のビット状態を返す
int CBit::bitncnd( unsigned long bitn )
{
	unsigned char *p;
	int ret;

	bitncnd_criticalSection.Lock();

		if( bitn >= m_Max ) {
			if( extend_bitbl( bitn ) != 0 ) {
				ret = -1;
				goto RET_;
			}
		}

		if( bitn >= m_Max || bitn < 0 ) {
			ret = -1;
		}
		else
		{
			p = adr( bitn );
			ret = ( *p & onmask( rem( bitn)) ? ON : OFF );
		}
RET_:		
	bitncnd_criticalSection.Unlock();

	return ret;
}

#ifdef MUTEX	// 05.23 /00
static CMutex onbitn_criticalSection;
#else
static CCriticalSection onbitn_criticalSection;
#endif
// bitn番目(0から始まる)のビットを ON とする
int CBit::onbitn( unsigned long bitn )
{
	unsigned char *p;
	int ret;

	onbitn_criticalSection.Lock();
		if( bitn >= m_Max ) {
			if( extend_bitbl( bitn ) != 0 ) {
				ret = -1;
				goto RET_;
			}
		}

		if( bitn >= m_Max || bitn < 0 )
			ret = -1;	//ERROR
		else
		{
			p = adr( bitn );
			*p |= ( onmask( rem( bitn)) );

			ret = 0;	//OK
		}
RET_:
	onbitn_criticalSection.Unlock();

	return ret;
}

#ifdef MUTEX	// 05.23 /00
static CMutex offbitn_criticalSection;
#else
static CCriticalSection offbitn_criticalSection;
#endif
// bitn番目(0から始まる)のビットを OFF とする
int CBit::offbitn( unsigned long bitn )
{
	unsigned char *p;
	int ret;

	offbitn_criticalSection.Lock();
		if( bitn >= m_Max ) {
			if( extend_bitbl( bitn ) != 0 ) {
				ret = -1;
				goto RET_;
			}
		}
		if( bitn >= m_Max || bitn < 0 )
			ret = -1;	//ERROR
		else
		{
			p = adr( bitn );
			*p &= ( ~onmask( rem( bitn)) );

			ret = 0;	//OK
		}
RET_:
	offbitn_criticalSection.Unlock();

	return ret;
}

#ifdef MUTEX	// 05.23 /00
static CMutex geton_bitn_criticalSection;
#else
static CCriticalSection geton_bitn_criticalSection;
#endif
// n 番目( 1 から始まる)の ON ビットのビット番号( 0 から始まる)を返す
int CBit::geton_bitn( unsigned long n )
{
//	unsigned char *p = m_pTbl;
	unsigned char *p = &m_ByArray[0];
	unsigned long cnt;
	unsigned long i, j;
	int ret;

	geton_bitn_criticalSection.Lock();
		i = 0;
		for( cnt = 0 ; cnt < m_bMax ; ++cnt, ++p )
		{
			j = onbit_n( *p );	//1char中のONビット数を計算
			if( i + j < n )
			{
				i += j;
			}
			else
			{
				ret = ( (cnt * 8 + onbit_no( *p, n - i )) - 1);
				goto RET_;
			}
		}
		ret = -1;	//ERROR
RET_:
	geton_bitn_criticalSection.Unlock();

	return ret;
}

#ifdef MUTEX	// 05.23 /00
static CMutex geton_bitno_criticalSection;
#else
static CCriticalSection geton_bitno_criticalSection;
#endif
// n ビット番目(０から始まる)位置の ON ビット番目(１から始まる)を返す
int CBit::geton_bitno( unsigned long n )
{
//	unsigned char *p = m_pTbl;
	unsigned char *p = &m_ByArray[0];
	unsigned long cnt;
	unsigned long i, j;
	unsigned long k;
	unsigned char mask = 0x80;
	int ret;

	geton_bitno_criticalSection.Lock();
		i = n / 8;
		k = 0;
		for( cnt = 0 ; cnt < m_bMax ; ++cnt, ++p )
		{
			if( cnt < i )
				k += onbit_n( *p );	//1char中のONビット数を計算
			else
			{
				for( j = 0 ; j < (unsigned long)((long)n - ((long)i * 8 - 1)) ; ++j )
				{
					if( *p & mask )
					{
						++k;
					}
					mask >>= 1;
				}
				break;
			}
		}
		ret = k;
	geton_bitno_criticalSection.Unlock();

	return ret;
}

// 1 char の ON ビットを数える。
//private:
int CBit::onbit_n( unsigned char c )
{
	int i;
	int n = 0;

	if( c )
		for( i = 8 ; i ; --i )
		{
			if( c & 0x01 )
				++n;
			c >>= 1;
	}

	return( n );
}

//1char中の n ON ビット番目( 1 から始まる) のビット番号(１から始まる)を返す
//private:
int CBit::onbit_no( unsigned char c, unsigned long n )
{
	unsigned long i,j;
	unsigned char mask = 0x80;
	
	i = 0;
	for( j = 0 ; (i < n) && (j < 8) ; ++j )
	{
		if( c & mask )
			++i;
		mask>>=1;
	}
	return( !i ? 0 : j );
}

// ビットテーブルの拡張
int CBit::extend_bitbl( int max ) 
{
const int ext_cnt = 4096;

	int ext;

	if( max < m_Max )
		return 0;

	// max チェック( 1000万BIT分のみ確保--バグによる大量確保を防ぐ)
	if( max > 10000000 )
		return -1;

	try {
		if( ((max - m_Max) / 8) < ext_cnt )
			ext = ext_cnt;
		else
			ext = ((max - m_Max) / 8) + 1;
	
		m_ByArray.SetSize( m_bMax + ext );
		memset( &m_ByArray[m_bMax], 0, ext );	// 02.15 /08
		m_Max += (ext*8);
		m_bMax += ext;
	}
	catch( CMemoryException* me )
	{
		me->Delete();
		return -1;
	}
	return 0;
}
