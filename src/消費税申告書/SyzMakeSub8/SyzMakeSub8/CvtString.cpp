// CvtString.cpp : インプリメンテーション ファイル
//


//#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
//#include <afxext.h>         // MFC の拡張部分

#include "stdafx.h"

#include "CvtString.h"

#include <mbstring.h>


/////////////////////////////////////////////////////////////////////////////


CCvtString::CCvtString(int nType)
{
	m_nType = nType;
}

int CCvtString::IsMBCharLeadByte(const char* s, int n)
{
	int k;
	if ( !_ismbblead(s[n]) )
		return 0;
	k = n;
	while ( --k >= 0 && _ismbblead( s[k]) );
	return (n - k) & 1;
}

int CCvtString::GetMBCharType(const char* s, int n)
{
	if ( n > 0 && IsMBCharLeadByte( s, n - 1 ) ) {
		return _ismbbtrail( s[n] ) ? MBTYPE_TRAIL : MBTYPE_ERR;
	}
	else if ( _ismbblead( s[n]) ) {
		return MBTYPE_LEAD;
	}
	else {
		return MBTYPE_SB;
	}
}


/////////////////////////////////////////////////////////////////////////////
// CCvtString

//--------------------------------------------------
// 改行コードタイプの取得
//		int		nType		0 : \n
//							1 : \r\n
//--------------------------------------------------
void CCvtString::ChangeCRLFType(int nType)
{
	m_nType = nType;
}

//===================================================
// 文字の種別を判定
//		LPCTSTR		string		判定文字列
//		short		offset		文字列位置
//	----------------------------------------------
//		返送値		1 : ASCII
//					2 : lead
//					3 : trail
//					4 : extend
//					5 : crlf
//					0 : else
//===================================================
short CCvtString::IsKindOfLetter(LPCTSTR string, short offset)
{
	int	len	= lstrlen( string );
	unsigned char *pStr	= (unsigned char*)string;
	unsigned char ch, ch1;

	for ( int i = 0; i < len; ) {
		ch = *(pStr + i);
		//lead of MBCS
		if ( (ch >= 0x81 && ch <= 0x9f) || (ch >= 0xe0 && ch <= 0xfc) ) {
			if ( i + 1 < len ) {
				ch1	=	*(pStr + i + 1);
				//trail of MBCS
//				if ( (ch >= 0x40 && ch <= 0x7e) || (ch >= 0x80 && ch <= 0xec) ) {
				if ( (ch1 >= 0x40 && ch1 <= 0x7e) || (ch1 >= 0x80 && ch1 <= 0xfc) ) {
					if ( i == offset ) {
						if ( ch >= 0xf0 && ch <= 0xf9 )
							return 4;
						return 2;
					}
					if ( i + 1 == offset )
						return 3;
					i	+=	2;
				}
				else {
					if ( i == offset )
						return 0;
					i++;
				}
			}
			else {
				if ( i == offset )
					return 0;
				i++;
			}
		}
		//ASCII
		else if ( (ch >= 0x20 && ch <= 0x7e) || (ch >= 0xa1 && ch <= 0xdf) ) {
			if ( i == offset )
				return 1;
			i++;
		}
		else {
			if ( (ch == 0x0a) || (ch == 0x0d) ) {		// 改行 (03/08/12)
				if ( i == offset )
					return 5;
				i++;
			}
			else {
				if ( i == offset )
					return 0;
				i++;
			}
		}
	}
	
	return 0;
}

//===================================================
// 半角→全角
//		LPCTSTR		ank			半角文字列
//		void*		sjis		全角文字列
//		int			mode		1 : カタカナ
//								2 : ひらがな
//	----------------------------------------------
//		返送値		0	: ok
//					100	: extend
//===================================================
int CCvtString::ANKtoSJIS(LPCTSTR ank, void* sjis, int mode)
{
	int	len	= lstrlen( ank );
	int	i, j, nlen, csw;
	int	st;
	short kind;
	char ch[4], zch[4];
	unsigned short han, zen;
	unsigned char *kanji = (unsigned char*)sjis;

	st = 0;
	for ( i = nlen = 0; i < len; ) {
		//1->ASCII 2->lead 3->trail 0->else 4->extend
		kind = IsKindOfLetter( ank, i );

		if ( kind == 0 ) {
			i++;
		}
		else if ( kind == 5 ) {			// 改行はそのまま追加 (03/08/12)
			memmove( kanji + nlen, ank + i, 1 );
			nlen++;
			i++;
		}
		else if ( kind == 4 ) {
			memmove( kanji + nlen, "　", 2 );
			nlen +=	2;
			i += 2;
			st = 100;
		}
		else if ( kind == 2 ) {
			memmove( kanji + nlen, ank + i, 2 );
			nlen +=	2;
			i += 2;
		}
		else if ( kind == 1 ) {
			ch[0] =	(unsigned char)ank[i];
			if ( i + 1 < len ) {
				ch[1] =	(unsigned char)ank[i+1];
				ch[2] =	NULL;

				for ( j = csw = 0; ; j++ ) {
					if ( KANATABLE2[j].ank == NULL )
						break;
					if ( !lstrcmp( ch, KANATABLE2[j].ank ) ) {
						csw	= 1;
						break;
					}
				}
				if ( csw ) {
					if ( mode == 2 ) {
						memmove( kanji + nlen, KANATABLE2[j].hira, lstrlen( KANATABLE2[j].hira ) );
						nlen +=	lstrlen( KANATABLE2[j].hira );
					}
					else {
						memmove( kanji + nlen, KANATABLE2[j].kana, lstrlen( KANATABLE2[j].kana ) );
						nlen += lstrlen( KANATABLE2[j].kana );
					}
					i += 2;
					continue;
				}
			}
			ch[1] =	NULL;
			for ( j = csw = 0; ; j++ ) {
				if ( KANATABLE1[j].ank == NULL )
					break;
				if ( !lstrcmp( ch, KANATABLE1[j].ank ) ) {
					csw	=	1;
					break;
				}
			}
			if ( csw ) {
				if ( mode == 2 ) {
					memmove( kanji + nlen, KANATABLE1[j].hira, lstrlen( KANATABLE1[j].hira ) );
					nlen +=	lstrlen( KANATABLE1[j].hira );
				}
				else {
					memmove( kanji + nlen, KANATABLE1[j].kana, lstrlen( KANATABLE1[j].kana ) );
					nlen +=	lstrlen( KANATABLE1[j].kana );
				}
				i++;
			}
			else {
				han	= (ch[0] & 0x00ff);
				zen	= _mbbtombc( han );
				if ( zen == han ) {
					memmove( kanji + nlen, "　", 2 );
					nlen +=	2;
					i++;
				}
				else {
					memmove( &zch[1], &zen, sizeof(unsigned short) );
					zch[0] = (unsigned char)(zen >> 8);
					zch[1] = (unsigned char)zen;
					zch[2] = NULL;
					memmove( kanji + nlen, zch, sizeof(unsigned short) );
					nlen +=	2;
					i++;
				}
			}
		}
	}
	*(kanji + nlen)	= NULL;

	return st;
}

//===================================================
// 半角→全角
//		LPCTSTR		sjis		全角文字列
//		void*		ank			半角文字列
//	----------------------------------------------
//		返送値		0	: ok
//					100	: extend
//===================================================
int	CCvtString::SJIStoANK(LPCTSTR sjis, void* ank)
{
	int	len	= lstrlen( sjis );
	int	i, j, nlen, csw;
	int	st;
	short kind;
	char ch[4];
	unsigned short han, zen;
	unsigned char *ankd	= (unsigned char*)ank;

	st = 0;
	for ( i = nlen = 0; i < len; ) {
		//1->ASCII 2->lead 3->trail 0->else 4->extend
		kind = IsKindOfLetter( sjis, i );

		if ( kind == 0 ) {
			i++;
		}
		else if ( kind == 5 ) {			// 改行 (03/08/12)
			memmove( ankd + nlen, sjis + i, 1 );
			nlen +=	1;
			i += 1;
		}
		else if ( kind == 4 ) {
			memmove( ankd + nlen, " ", 1 );
			nlen +=	1;
			i += 2;
			st = 100;
		}
		else if ( kind == 1 ) {
			memmove( ankd + nlen, sjis + i, 1 );
			nlen +=	1;
			i += 1;
		}
		else if ( kind == 2 ) {
			ch[0] =	(unsigned char)sjis[i];

			if ( i + 1 < len ) {
				ch[1] =	(unsigned char)sjis[i+1];
				ch[2] =	NULL;

				for ( j = csw = 0; ; j++ ) {
					if ( KANATABLE2[j].kana == NULL )
						break;
					if ( !lstrcmp( ch, KANATABLE2[j].kana ) || !lstrcmp( ch, KANATABLE2[j].hira )) {
						csw	= 1;
						break;
					}
				}
				if ( csw ) {
					memmove( ankd + nlen, KANATABLE2[j].ank, lstrlen( KANATABLE2[j].ank ) );
					nlen +=	lstrlen( KANATABLE2[j].ank );
					i += 2;
					continue;
				}

				for ( j = csw = 0; ; j++ ) {
					if ( KANATABLE1[j].kana == NULL )
						break;
					if ( !lstrcmp( ch, KANATABLE1[j].kana ) || !lstrcmp( ch, KANATABLE1[j].hira ) ) {
						csw	= 1;
						break;
					}
				}
				if ( csw ) {
					memmove( ankd + nlen, KANATABLE1[j].ank, lstrlen( KANATABLE1[j].ank ) );
					nlen +=	lstrlen( KANATABLE1[j].ank );
					i += 2;
				}
				else {
					zen	= (ch[0] & 0x00ff);
					zen	*= 256;
					zen	|= (ch[1] & 0x00ff);

					han	= _mbctombb( zen );
					if ( zen == han ) {
						i += 2;
					}
					else {
						ch[0] =	(han & 0x00ff);
						memmove( ankd + nlen, ch, 1 );
						nlen +=	1;
						i += 2;
					}
				}
			}
		}
	}
	*(ankd + nlen) = NULL;

	return st;
}

//====================================================
// 半角→全角（全角→全角）
//		LPCTSTR		ank		半角文字列
//		LPSTR		kanji	全角文字列
//		int			mode	1 : カタカナ
//							2 : ひらがな
//							3 : ひらがな→カタカナ
//							4 : カタカナ→ひらがな
//====================================================
int	CCvtString::ConvertMultibyte(LPCTSTR ank, LPSTR kanji, int mode)
{
	if ( (mode == 1) || (mode == 2) ) {
		return ANKtoSJIS( ank, kanji, mode );
	}
	else if ( (mode == 3) || (mode == 4) ) {
		char buf[512] = { 0 };
		ConvertSinglebyte( ank, buf );
		return ANKtoSJIS( buf, kanji, (mode/2) );
	}

	return 0;
}

//====================================================
// 全角→半角
//		LPSTR		kanji	全角文字列
//		LPCTSTR		ank		半角文字列
//====================================================
int	CCvtString::ConvertSinglebyte(LPCTSTR kanji, LPSTR ank)
{
	return SJIStoANK( kanji, ank );
}

//==================================================================
// 改行コードの追加
//		LPTSTR	pString		改行追加文字列
//		int		nSize		文字列バッファサイズ
//		int		nLmax		一行の最大入力数（バイト数）
//	-----------------------------------------------------------
//		返送値		0		ＯＫ
//					-1		ＥＲＲＯＲ
//==================================================================
int CCvtString::AddCRLF(LPTSTR pString, int nSize, int nLmax)
{
	int i = 0, j = 0, k = 0;
	int len = 0, bflen = 0, klen = 0, kcnt = 0;
	int ret = -1;
	char* pstr = NULL, *pbuf = NULL, *crlf = NULL;

	// 改行タイプ
	if ( !m_nType ) {
		klen = 1;	crlf = _T("\n");
	}
	else if ( m_nType == 1 ) {
		klen = 2;	crlf = _T("\r\n");
	}

	if ( (nLmax < 1) || (nSize < 1) )		// 入力数orバッファサイズなし
		goto ADD_CRLF_END;

	pstr = new char[nSize+1];
	if ( !pstr )
		goto ADD_CRLF_END;
	::ZeroMemory( pstr, nSize+1 );
	memmove( pstr, pString, nSize );

	// 改行削除
	ret = DeleteCRLF( pstr, nSize );
	if ( ret < 1 )
		goto ADD_CRLF_END;
	else 
		ret = -1;
	len = (int)strlen( pstr );

	// 領域の確保
	pbuf = new char[(len*3)+1];
	if ( !pbuf )
		goto ADD_CRLF_END;
	::ZeroMemory( pbuf, (len*3)+1 );

	// 改行追加
	for ( i = 0, j = 0, k = 0; i < len; i++ ) {
		if ( nLmax == 1 ) {
			int type = GetMBCharType( pstr, i );
			if ( type != MBTYPE_SB )		// マルチバイト文字orＥＲＲ
				goto ADD_CRLF_END;
			pbuf[j] = pstr[i];
			j++;
			strcat_s( pbuf, (len*3)+1, crlf );
			j += klen;
		}
		else {
			int type = GetMBCharType( pstr, i );
			if ( k == (nLmax-1) ) {		// 改行チェック
				if ( type == MBTYPE_LEAD ) {		// マルチバイト文字（１バイト目）
					strcat_s( pbuf, (len*3)+1, crlf );
					j += klen;
					pbuf[j] = pstr[i];
					j++;
					k = 1;
				}
				else {		// シングルバイト文字orマルチバイト文字（２バイト目）
					pbuf[j] = pstr[i];
					j++;
					if ( i != (len-1) ) {
						strcat_s( pbuf, (len*3)+1, crlf );
						j += klen;
					}
					k = 0;
				}
				kcnt++;
			}
			else {
				pbuf[j] = pstr[i];
				j++;
				k++;
			}
		}
	}

	bflen = (int)strlen( pbuf );

	if ( nSize < bflen )
		goto ADD_CRLF_END;

	::ZeroMemory( pString, nSize );
	memmove( pString, pbuf, bflen );
	ret = 0;

ADD_CRLF_END:
	if ( pstr )
		delete[] pstr;
	if ( pbuf )
		delete[] pbuf;

	return ret;
}

//==================================================================
// 改行コードの追加
//		CString&	rString		改行追加文字列
//		int			nLmax		一行の最大入力数（バイト数）
//	-----------------------------------------------------------
//		返送値		0		ＯＫ
//					-1		ＥＲＲＯＲ
//==================================================================
int CCvtString::AddCRLF(CString& rString, int nLmax)
{
	int len = rString.GetLength();
	int cnt = (((len/nLmax)+1)*2);
	return AddCRLF( rString.GetBufferSetLength(len+cnt), len+cnt+1, nLmax );
}

//==========================================================
// 改行コードの削除
//		LPTSTR		pString		改行削除文字列
//		int			nSize		文字列サイズ
//	-----------------------------------------------------
//		返送値		1 ～	: 文字列あり
//					0		: 文字列なし
//					-1		: エラー
//==========================================================
int CCvtString::DeleteCRLF(LPTSTR pString, int nSize)
{
	int len = 0, i = 0, j = 0;
	char* pbuf = NULL;

	// 領域の確保
	pbuf = new char[nSize+1];
	if ( !pbuf )
		return -1;
	::ZeroMemory( pbuf, nSize+1 );

	memmove( pbuf, pString, nSize );
	::ZeroMemory( pString, nSize );

	len = (int)strlen( pbuf );

	// 改行の削除
	for ( i = 0, j = 0; i < len; i++ ) {
		if ( pbuf[i] == '\0' ) {
			break;
		}
		else if ( pbuf[i] == '\r' || pbuf[i] == '\n' ) {
			continue;
		}
		else {
			pString[j] = pbuf[i];
			j++;
		}
	}

	delete[] pbuf;

	return j;
}

//===================================================
// 改行コードの削除
//		CString&	rStr		改行削除文字列
//	--------------------------------------------
//		返送値		1 ～	: 文字列あり
//					0		: 文字列なし
//					-1		: エラー
//===================================================
int CCvtString::DeleteCRLF(CString& rString)
{
	int len = rString.GetLength();
	return DeleteCRLF( rString.GetBufferSetLength(len), len+1 );
}

//===================================================
// 改行コード数の取得
//		LPTSTR		pString		取得対象文字列
//		int			nSize		文字列サイズ
//	--------------------------------------------
//		返送値		1 ～	: 改行コードあり
//					0		: 改行コードなし
//					-1		: エラー
//===================================================
int CCvtString::GetCRLFCount(LPCTSTR pString, int nSize)
{
	int len = 0, cnt = 0;
	char* pbuf = NULL;

	// 領域の確保
	pbuf = new char[nSize+1];
	if ( !pbuf )
		return -1;
	::ZeroMemory( pbuf, nSize+1 );

	memmove( pbuf, pString, nSize );
	len = (int)strlen( pbuf );

	for ( int i = 0; i < len; i++ ) {
		if ( pbuf[i] == '\0' ) {
			break;
		}
		else if ( pbuf[i] == '\n' ) {
			cnt++;
		}
	}

	delete[] pbuf;

	return cnt;
}

//===================================================
// 改行コード数の取得
//		CString		rStr		取得対象文字列
//	--------------------------------------------
//		返送値		1 ～	: 改行コードあり
//					0		: 改行コードなし
//					-1		: エラー
//===================================================
int CCvtString::GetCRLFCount(CString String)
{
	int len = String.GetLength();
	return GetCRLFCount( String, len+1 );
}

//----------------------------------------------------
// 全角スペースのカット
//		char*		pStr		文字列
//		int			nSize		文字列サイズ
//----------------------------------------------------
void CCvtString::SpaceCut(char* pStr, int nSize)
{
	char buf[512] = { 0 };
	for ( int i = 0, j = 0; i < nSize; i += 2 ) {
		if ( !memcmp( (pStr+i), "　", 2 ) )
			continue;
		memmove( &buf[j], (pStr+i), 2 );
		j += 2;
	}

	memmove( pStr, buf, nSize );
}

//----------------------------------------------------
// 半角スペースのカット
//		char*		pStr		文字列
//		int			nSize		文字列サイズ
//----------------------------------------------------
void CCvtString::HalfSpaceCut(char* pStr, int nSize)
{
	char buf[512] = { 0 };
	for ( int i = 0, j = 0; i < nSize; i++ ) {
		if ( !memcmp( (pStr+i), " ", 1 ) )
			continue;
		memmove( &buf[j], (pStr+i), 1 );
		j += 1;
	}

	memmove( pStr, buf, nSize );
}

//----------------------------------------------------
// 左側スペースカット
//		char*		pStr		文字列
//		int			nSize		文字列サイズ
//----------------------------------------------------
void CCvtString::TrimLeft(char* pStr, int nSize)
{
	char buf[512] = { 0 };

	for ( int i = 0; i < nSize; ) {
		// 半角スペースチェック
		if ( !memcmp( (pStr+i), " ", 1 ) ) {
			i++;	continue;
		}
		// 全角スペースチェック
		if ( (i+1) < nSize ) {
			if ( !memcmp( (pStr+i), "　", 2 ) ) {
				i += 2;		continue;
			}
		}
		memmove( buf, (pStr+i), (nSize-i) );
		break;
	}

	::ZeroMemory( pStr, nSize );
	memmove( pStr, buf, strlen( buf ) );
}

//----------------------------------------------------
// 右側スペースカット
//		char*		pStr		文字列
//		int			nSize		文字列サイズ
//----------------------------------------------------
void CCvtString::TrimRight(char* pStr, int nSize)
{
	char buf[512] = { 0 };

	for ( int i = (nSize-1); i >= 0; ) {
		// 半角スペースチェック
		if ( !memcmp( (pStr+i), " ", 1 ) ) {
			i--;	continue;
		}
		// 全角スペースチェック
		if ( (i-1) > 0 ) {
			if ( !memcmp( (pStr+(i-1)), "　", 2 ) ) {
				i -= 2;		continue;
			}
		}
		if ( i >= 0 )
			memmove( buf, pStr, (i+1) );
		break;
	}

	::ZeroMemory( pStr, nSize );
	memmove( pStr, buf, strlen( buf ) );
}

//----------------------------------------------------
// 左側・右側スペースカット
//		char*		pStr		文字列
//		int			nSize		文字列サイズ
//----------------------------------------------------
void CCvtString::TrimLeftRight(char* pStr, int nSize)
{
	char buf[1024] = { 0 };

	memmove( buf, pStr, min( nSize, 1024) );
	::ZeroMemory( pStr, nSize );

	TrimLeft( buf, (int)strlen( buf ) );
	TrimRight( buf, (int)strlen( buf ) );

	memmove( pStr, buf, min( nSize, (int)strlen( buf ) ) );
}

//----------------------------------------------------
// スペースカット
//		char*		pStr		文字列
//		int			nSize		文字列サイズ
//----------------------------------------------------
void CCvtString::TrimAll(char* pStr, int nSize)
{
	char buf[512] = { 0 };

	for ( int i = 0, j = 0; i < nSize; ) {
		// 半角スペースチェック
		if ( !memcmp( (pStr+i), " ", 1 ) ) {
			i++;	continue;
		}
		// 全角スペースチェック
		if ( (i+1) < nSize ) {
			if ( !memcmp( (pStr+i), "　", 2 ) ) {
				i += 2;		continue;
			}
		}
		buf[j] = *(pStr+i);
		i++;
		j++;
	}

	::ZeroMemory( pStr, nSize );
	memmove( pStr, buf, strlen( buf ) );
}

//=================================================================================
// 文字列の結合
//		LPTSTR		pString		作成された文字列格納
//		int			nSize		バッファサイズ（-1は全ての文字列を格納）
//		LPCTSTR		pSvStr		元の文字列
//		LPCTSTR		pAddStr		追加文字列
//		LONG		lPos		結合位置
//		int			nLmax		一行の最大文字数
//	----------------------------------------------------------------------------
//		返送値		0 < 	ＯＫ
//					-1		ＥＲＲＯＲ
//=================================================================================
int CCvtString::UnionString(LPTSTR pString, int nSize, LPCTSTR pSvStr, LPCTSTR pAddStr, LONG lPos, int nLmax)
{
	if ( (nLmax < 1) || (lPos < 0) || (nSize == 0) )		// 行数エラーorポジションエラーorサイズエラー
		return -1;

	char* pbuf = NULL;
	CString str1 = pSvStr;
	CString str2 = pAddStr;
	DeleteCRLF( str1 );
	DeleteCRLF( str2 );
	int len1 = str1.GetLength();
	int len2 = str2.GetLength();

	if ( len1 < lPos )
		lPos = len1;

	int bsize = (((len1+len2)*3)+1);
	pbuf = new char[bsize];
	if ( !pbuf )
		return -1;
	::ZeroMemory( pbuf, bsize );

	int i = 0; 
	if ( lPos > 0 ) {		// 途中に結合
		if ( GetMBCharType( str1, (lPos-1) ) == MBTYPE_LEAD )
			// 挿入ポジションの前文字がマルチバイト（１文字目）の場合はこの文字のあとで結合
			lPos++;
		memmove( pbuf, str1, lPos );
		strcat_s( pbuf, bsize, str2 );
		int len = (int)strlen( pbuf );
		LPTSTR p = str1.GetBufferSetLength( len1 );
		memmove( &pbuf[len], (p+lPos), (len1-lPos) );
	}
	else if ( lPos == len1 ) {		// 最後に結合
		strcat_s( pbuf, bsize, str1 );
		strcat_s( pbuf, bsize, str2 );
	}
	else {		// 先頭に結合
		strcat_s( pbuf, bsize, str2 );
		strcat_s( pbuf, bsize, str1 );
	}

	AddCRLF( pbuf, bsize, nLmax );

	int len = (int)strlen( pbuf );
	if ( nSize > 0 )
		::ZeroMemory( pString, nSize );
	else if ( nSize == -1 )
		nSize = len;

	memmove( pString, pbuf, min( len, nSize ) );

	delete[] pbuf;

	return min( len, nSize );

}

//=================================================================================
// 文字列の結合
//		CString&	rString		作成された文字列格納
//		LPCTSTR		pSvStr		元の文字列
//		LPCTSTR		pAddStr		追加文字列
//		LONG		lPos		結合位置
//		int			nLmax		一行の最大文字数
//	----------------------------------------------------------------------------
//		返送値		0 <		ＯＫ
//					-1		ＥＲＲＯＲ
//=================================================================================
int CCvtString::UnionString(CString& rString, LPCTSTR pSvStr, LPCTSTR pAddStr, LONG lPos, int nLmax)
{
	int ret = -1;
	int len1 = (int)strlen( pSvStr );
	int len2 = (int)strlen( pAddStr );
	LPTSTR pstr = rString.GetBufferSetLength( ((len1+len2)*3+1) );
	if ( pstr ) {
		::ZeroMemory( pstr, ((len1+len2)*3+1) );
		ret = UnionString( pstr, -1, pSvStr, pAddStr, lPos, nLmax );
	}
	return ret;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// 05/09/14 追加

int CCvtString::IsMultiByteOnly(LPCTSTR pStr, int nSize)
{
	char ch[4] = { 0 };

	for ( int i = 0; i < nSize; i++ ) {
		int rsw = 0;
		short ret = IsKindOfLetter( pStr, i );
		if ( ret == 1 ) {		// ASCII
			return -1;
		}
		if ( (ret == 4) || (ret == 0) ) {
			return -3;
		}
	}
	return 0;
}

int CCvtString::IsMultiKanaOnly(LPCTSTR pStr, int nSize)
{
	int i = 0, j = 0;
	char ch[4] = { 0 };

	for ( i = 0; i < nSize; i++ ) {
		int rsw = 0;
		short ret = IsKindOfLetter( pStr, i );
		if ( ret == 1 ) {		// ASCII
			return -1;
		}
		else if ( ret == 2 ) {
			ch[0] =	*(pStr+i);
			if ( (i + 1) < nSize ) {
				ch[1] =	*(pStr+i+1);
			}
			for ( j = 0; !rsw; j++ ) {
				if ( KANATABLE2[j].kana == NULL )
					break;
				if ( !lstrcmp( ch, KANATABLE2[j].kana ) ) {
					rsw = 1;
					break;
				}
			}
			for ( j = 0; !rsw; j++ ) {
				if ( KANATABLE1[j].kana == NULL )
					break;
				if ( !lstrcmp( ch, KANATABLE1[j].kana ) ) {
					rsw = 1;
					break;
				}
			}
			if ( rsw )
				continue;

			return -2;
		}
		if ( (ret == 4) || (ret == 0) ) {
			return -3;
		}
	}
	return 0;
}

int CCvtString::IsNumericOnly(LPCTSTR pStr, int nSize)
{
	for ( int i = 0; i < nSize; i++ ) {
		short ret = IsKindOfLetter( pStr, i );
		if ( ret == 1 ) {		// ASCII
			if ( (*(pStr+i) >= '0') && (*(pStr+i) <= '9') )
				continue;
			return -1;
		}
		else if ( ret == 2 ) {
			return -2;
		}
		if ( (ret == 4) || (ret == 0) ) {
			return -3;
		}
	}
	return 0;
}

int CCvtString::IsPassuseOnly(LPCTSTR pStr, int nSize)
{
	for ( int i = 0; i < nSize; i++ ) {
		short ret = IsKindOfLetter( pStr, i );
		if ( ret == 1 ) {		// ASCII
			if ( (*(pStr+i) >= '0') && (*(pStr+i) <= '9') )
				continue;
			if ( (*(pStr+i) >= 'a') && (*(pStr+i) <= 'z') )
				continue;
			if ( (*(pStr+i) >= 'A') && (*(pStr+i) <= 'Z') )
				continue;
			if ( (*(pStr+i) == '@') || (*(pStr+i) == '.') || (*(pStr+i) == '-') || (*(pStr+i) == '_') )
				continue;
			return -1;
		}
		else if ( ret == 2 ) {
			return -2;
		}
		if ( (ret == 4) || (ret == 0) ) {
			return -3;
		}
	}
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
