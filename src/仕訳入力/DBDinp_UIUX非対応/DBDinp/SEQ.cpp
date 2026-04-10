// SEQ.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"

#include "SEQ.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <afxmt.h>
/////////////////////////////////////////////////////////////////////////////
// CSEQ

CSEQ::CSEQ()
{
//	m_pTbl = 0;
	m_nMax = m_Max = m_bMax = 0;
}

CSEQ::~CSEQ()
{
	m_DwArray.RemoveAll();

#ifdef DB_OLD_CUT
	if( m_pTbl )
		free( m_pTbl );
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CSEQ ハンドラ

//ＳＥＱテーブル作成
int CSEQ::mk_seqtbl( long max )
{
#ifdef _TEST_DEBUG
	max = 1;
#endif

	try {
		m_DwArray.SetSize( max );
		m_bMax = (max * sizeof(DWORD));
		m_Max = max;
		this->fill( );
	} 
	catch( CMemoryException* m ) {
		m->Delete();
		return -1;
	}
	return 0;

#ifdef DB_OLD_CUT
	if( max )
	{
		m_pTbl = (DWORD *)malloc( m_bMax = (max * sizeof(DWORD)) );
		if( m_pTbl )
		{
			m_Max = max;
			return( 0 );	//OK
		}
	}
	return( -1 );	//ERROR
#endif

}

//ＳＥＱテーブル削除
int CSEQ::del_seqtbl()
{
	m_nMax = m_Max = m_bMax = 0;
	m_DwArray.RemoveAll();
	return 0;

#ifdef DB_OLD_CUT
	if( m_pTbl )
	{
		delete m_pTbl;

		m_pTbl = 0;
		m_nMax = m_Max = m_bMax = 0;
		return 0;	//OK
	}
	return -1;	//ERROR
#endif
}

//ＳＥＱテーブルをイニシャライズ
void CSEQ::fill( void )
{
	memset( &m_DwArray[0], NULL, m_bMax );

#ifdef DB_OLD_CUT
	memset( m_pTbl, NULL, m_bMax );
	m_nMax = 0;
#endif
}

#ifdef DB_OLD_CUT
#ifdef MUTEX	// 05.23 /00
static CMutex ngetseq_criticalSection;
#else
static CCriticalSection ngetseq_criticalSection;
#endif
#endif

const int EXTPlus = 1000;

// n番目(0から始まる)のSEQ番号を返す
int CSEQ::ngetseq( long n )
{
	int seq;

	// 配列を拡張する
	int ext;
	if( n >= m_Max ) {
		if( (n - m_Max) < EXTPlus ) {
			ext = n + EXTPlus;
		}
		else	ext = (n+1);

		try {
			m_DwArray.SetSize( ext );
		}
		catch( CMemoryException* m ) {
			m->Delete();
			return -1;
		}
		memset( &m_DwArray[m_Max], 0, (ext-m_Max) * sizeof( DWORD ) );

		m_Max = ext;
		m_bMax = (ext * sizeof(DWORD));
	}

	if( n < 0 || (n > m_nMax) )
		seq = -1;
	else
		seq = m_DwArray[n];

#ifdef DB_OLD_CUT
	ngetseq_criticalSection.Lock();
		if( n >= m_Max || n < 0 || (n > m_nMax) )
			seq = -1;
		else
			seq = *(m_pTbl + n);
	ngetseq_criticalSection.Unlock();
#endif
	return seq;
}

#ifdef DB_OLD_CUT
#ifdef MUTEX	// 05.23 /00
static CMutex nsetseq_criticalSection;
#else
static CCriticalSection nsetseq_criticalSection;
#endif
#endif

// n番目(0から始まる)にＳＥＱ番号をセットする
int CSEQ::nsetseq( long seq, long n )
{
	int ret;

	// 配列を拡張する
	int ext;
	if( n >= m_Max ) {
		if( (n - m_Max) < EXTPlus ) {
			ext = n + EXTPlus;
		}
		else	ext = (n+1);

		try {
			m_DwArray.SetSize( ext );
		}
		catch( CMemoryException* m ) {
			m->Delete();
			return -1;
		}
		memset( &m_DwArray[m_Max], 0, (ext-m_Max) * sizeof( DWORD ) );

		m_Max = ext;
		m_bMax = (ext * sizeof(DWORD));
	}

	if( n < 0 )	return -1;
	else {
		m_DwArray[n] = (DWORD)seq;
		if( n > m_nMax )
			m_nMax = n;
		ret = 0;	//OK	
	}

#ifdef DB_OLD_CUT
	nsetseq_criticalSection.Lock();
		if( n >= m_Max || n < 0 )
			ret = -1;	//ERROR
		else
		{
			*(m_pTbl + n) = (DWORD)seq;
			if( n > m_nMax )
				m_nMax = n;
			ret = 0;	//OK
		}
	nsetseq_criticalSection.Unlock();
#endif

	return ret;
}
