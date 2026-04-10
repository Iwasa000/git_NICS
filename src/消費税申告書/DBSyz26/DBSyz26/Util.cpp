#include "stdafx.h"
#include "Util.h"

CUtil::CUtil()
{
	m_Arith.l_defn( 0x16 );
}

CUtil::~CUtil()
{
}

//-----------------------------------------------------------------------------
// 文字列を６倍長に変換
//-----------------------------------------------------------------------------
// 引数	d_val	：	金額（６倍長　バイナリ）
//		s_val	：	金額（文字列）
//-----------------------------------------------------------------------------
void CUtil::val_to_bin( unsigned char *d_val, CString s_val )
{
	char	asci[20] = {0};

	memset( d_val, '\0', 6 );
	strcpy_s( asci, (LPCTSTR)s_val );
	m_Arith.l_input( d_val, asci );
}

//-----------------------------------------------------------------------------
// ６倍長を文字列に変換
//-----------------------------------------------------------------------------
// 引数	d_val	：	金額（文字列）
//		s_val	：	金額（６倍長　バイナリ）
//-----------------------------------------------------------------------------
void CUtil::val_to_asci( CString *d_val, unsigned char *s_val )
{
	char	asci[20];

	d_val->Empty();
	memset( asci, '\0', sizeof( asci ) );
	l_print( asci, s_val, "SSSSSSSSSSSSS9" );
	*d_val += asci;
	d_val->TrimLeft();
}

//-----------------------------------------------------------------------------
// ６倍長　切り捨て
//-----------------------------------------------------------------------------
// 引数	des	：	
//		sou	：	
//		sgn	：	サイン
//					0x0:千円未満 0x?0:切り捨て
//					0x1:百円未満 0x?1:切り上げ
//					0x2:十円未満 0x?2:四捨五入
//-----------------------------------------------------------------------------
void CUtil::l_calq( char *des, char *sou, char sgn )
{
	char 	buf1[10]={0}, buf2[10]={0}, val100[10]={0}, val10[10]={0}, val4[10]={0};
	int		msign;
		
	memset( buf1,   '\0', sizeof( buf1 ) );
	memset( val100, '\0', sizeof( val100 ) );
	memset( val10,  '\0', sizeof( val10 ) );
	memmove( buf1, sou, 6 );
	
	// 計算
	switch( sgn&0xf0 ){
		case 0x00 :
			m_Arith.l_input( val100, _T("1000") );
			m_Arith.l_input( val10, _T("100") );
			break;
		case 0x10 :
			m_Arith.l_input( val100, "100" );
			m_Arith.l_input( val10, "10" );
			break;
		default   :
			m_Arith.l_input( val100, "10" );
			m_Arith.l_input( val10, "0" );
			break;
	}
	
	// 計算
	if( (sgn &= 0x03) ){
		m_Arith.l_input( val4,  "4" );
		
		if( (msign = m_Arith.l_test(buf1)) < 0 ){
			m_Arith.l_neg( buf1 );
		}
		m_Arith.l_mod( buf2, buf1, val100 );
		m_Arith.l_div( buf2, buf2, val10 );
		
		if( sgn == 1 ){			// 切り上げ
			if( m_Arith.l_test(buf2) ){
				m_Arith.l_input( buf2, "1" );
			}
		}
		else if( sgn == 2 ){	// 四捨五入
			m_Arith.l_sub( buf2, buf2, val4 );
			if( m_Arith.l_test( buf2 ) > 0 ){
				m_Arith.l_input( buf2, "1" );
			}
			else{
				m_Arith.l_clear( buf2 );
			}
		}
		m_Arith.l_div( buf1, buf1, val100 );
		m_Arith.l_add( buf1, buf1, buf2 );
		m_Arith.l_mul( buf1, buf1, val100 );
		if( msign < 0 ){
			m_Arith.l_neg( buf1 );
		}
	}
	else {						// 切り捨て
		m_Arith.l_div( buf1, buf1, val100 );
		m_Arith.l_mul( buf1, buf1, val100 );
	}
	
	memmove( des, buf1, 6 );
}

