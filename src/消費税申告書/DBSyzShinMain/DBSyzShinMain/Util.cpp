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
// CString　から　Char へ
//-----------------------------------------------------------------------------
// 引数	d_asci	：	変換先
//		s_asci	：	変換元
//		len		：	変換文字列長
//-----------------------------------------------------------------------------
void CUtil::cstring_to_char( unsigned char *d_asci, CString s_asci, int len )
{
	char	asci[128] = {0};
	int		cnt;

	memset( d_asci, '\0', len );
	memset( asci, '\0', sizeof(asci) );
	strcpy_s( asci, (LPCTSTR)s_asci );
	if( (cnt = (int)strlen(asci)) ){
		if( len < cnt ){
			cnt = len;
		}
		memmove( d_asci, asci, cnt );
	}
}

//-----------------------------------------------------------------------------
// 文字列をBCDにパック
//-----------------------------------------------------------------------------
// 引数	bcdbf	：	BCD
//		ascbf	：	文字列
//		len		：	文字数
//-----------------------------------------------------------------------------
void CUtil::asci_to_bcd( unsigned char *bcdbf, CString *ascbf, int len )
{
	char	asci[20]={0}, bcde[10]={0};
	int		length = ascbf->GetLength();
	memmove( asci, *ascbf, length );
	m_Arith.l_pack( bcde, asci, len );
	memmove( bcdbf, bcde, len );
}


