// DBSubFnc.cpp : DLL の初期化ルーチンです。
//

#include "stdafx.h"
#include <afxdllx.h>
#ifdef _MANAGED
#error /clr と共にコンパイルするには DBSubFnc.cpp 内の操作手順を参照してください。
// /clr をプロジェクトに追加したい場合は、次の操作を行ってください:
//	1. 上記の afxdllx.h のインクルードを削除する
//	2. .cpp ファイルを、プリコンパイル済みヘッダーが無効にされていて次のテキストを含み、
//	   /clr がスローされていないプロジェクトに追加します:
//			#include <afxwin.h>
//			#include <afxdllx.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


static AFX_EXTENSION_MODULE DBSubFncDLL = { NULL, NULL };

#ifdef _MANAGED
#pragma managed(push, off)
#endif

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// lpReserved を使う場合はここを削除してください
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("DBSubFnc.DLL Initializing!\n");
		
		// 拡張 DLL を 1 回だけ初期化します。
		if (!AfxInitExtensionModule(DBSubFncDLL, hInstance))
			return 0;

		// この DLL をリソース チェーンへ挿入します。
		// メモ: 拡張 DLL が MFC アプリケーションではなく
		//  MFC 標準 DLL (ActiveX コントロールのような)
		//  に暗黙的にリンクされる場合、この行を DllMain
		//  から削除して、この拡張 DLL からエクスポート
		//  された別の関数内へ追加してください。
		//  この拡張 DLL を使用する標準 DLL はこの拡張 DLL
		//  を初期化するために明示的にその関数を呼び出します。
		//  それ以外の場合は、CDynLinkLibrary オブジェクトは
		//  標準 DLL のリソース チェーンへアタッチされず、
		//  その結果重大な問題となります。

		new CDynLinkLibrary(DBSubFncDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("DBSubFnc.DLL Terminating!\n");

		// デストラクタが呼び出される前にライブラリを終了します
		AfxTermExtensionModule(DBSubFncDLL);
	}
	return 1;   // OK
}

#ifdef _MANAGED
#pragma managed(pop)
#endif




// 日付入力文字列(ocxより)のチェック
extern "C" DBSUBFNC
int WINAPI DateInputCheck( char* check, BYTE* bcd )
{
	size_t	len	= strlen( check );

	short	daytbl[13]	=	{ 31, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	char	tmp[16];
	short	mnth, day, chk;

	memset( tmp, 0, sizeof tmp );

	if( len <= 0 ) {
		return	-1;
	}

	for( int i = 0; i < len; i++ ) {
		if( IsCharAlphaNumeric( check[i] ) && ! IsCharAlpha( check[i] ) );
		else 
			return -1;
	}

	switch( len ) {
	case 1:
		mnth = -1;
		tmp[0] = '0';
		tmp[1] = check[0];
		day = atoi( tmp );
		if( day == 0 )	mnth = 0;
		if( day > daytbl[0] )
			return -1;

		break;
	case 2:
		if( check[0] == '6' && check[1] == '1' ) {
			mnth = 61;
			day = 0;
		}
		else if( check[0] == '6' && check[1] == '2' ) {
			mnth = 62;
			day = 0;
		}
		else if( check[0] == '6' && check[1] == '3' ) {
			mnth = 63;
			day = 0;
		}
		else {
			mnth = -1;
			tmp[0] = check[0];
			tmp[1] = check[1];
			day = atoi( tmp );
			if( day == 0 )	mnth = 0;
			if( day > daytbl[0] )
				return -1;
		}	
		break;
	case 3:
		tmp[0] = '0';
		tmp[1] = check[0];
		mnth = atoi( tmp );
		if( ! (mnth >= 0 && mnth <= 12) )
			return -1;
		tmp[0] = check[1];
		tmp[1] = check[2];
		day = atoi( tmp );
		if( day > daytbl[mnth] )
			return -1;
		break;
	case 4:
		tmp[0] = check[0];
		tmp[1] = check[1];
		mnth = atoi( tmp );
		if( mnth < 0 || (mnth > 12 && !(mnth >= 61 && mnth <= 63) ) )
			return -1;
		tmp[0] = check[2];
		tmp[1] = check[3];
		day = atoi( tmp );
		chk = (mnth >= 61 && mnth <= 63) ? 0 : mnth;
		if( day > daytbl[chk] )
			return -1;
		break;
	default:
		ASSERT( FALSE );
		return -1;
		break;
	}

	::ZeroMemory( tmp, sizeof tmp );
	
	if( mnth == -1 ) {
		tmp[0] = '0';
		tmp[1] = '0';
	}
	else if( mnth == 0 ) {
		tmp[0] = '0';
		tmp[1] = '0';	
	}
	else if( mnth >= 61 && mnth <= 63 ) {
		sprintf_s( tmp, sizeof tmp, "%02d", mnth );
	}
	else {
		sprintf_s( &tmp[0], sizeof tmp, "%02d", mnth );
	}
	sprintf_s( &tmp[2], sizeof tmp - 2, "%02d", day );

	l_pack( bcd, tmp, 4 );
	if( day == 0 )	bcd[1] = (BYTE)'\xff';
//	bcd[2] = 0xAA;

	return	0;
}

// 科目文字列のチェック
extern "C" DBSUBFNC
int WINAPI KamokuStr_Chk( LPCTSTR string, struct _KamokuData* data, BOOL noteda/*=FALSE*/ )
{
	int		i, clen, elen, klen, st, edasw;
	size_t	length;
	char str[128] = {0};
	char kanabf[20] = {0}, edabf[20] = {0}, codebf[20] = {0};
	strcpy_s( str, sizeof str, string );
	length = strlen( (LPCTSTR)str );
	if( length > sizeof codebf )
		length = sizeof codebf - 1;
	
	st = edasw = 0;
	clen	=	elen	=	klen	= 0;
	for( i = 0; i < length; i++ ) {
		if( str[i] == 0x2e || str[i] == 0xa5 ) {// 枝番かどうかの 点(.)
			if( noteda )	break;
			edasw	=	TRUE;
		}
		else if( _ismbbkana( str[i] ) ) {
			if( clen || elen ) {
				st	=	-1;
				break;
			}
			kanabf[klen++]	=	str[i];
		}
		else if( IsCharAlphaNumeric( str[i] ) ) {
			if( klen ) {
				st	=	-1;
				break;
			}
			if( edasw == TRUE ) {
				if( !IsCharAlpha( str[i] ) )	edabf[elen++]	=	str[i];
				else	{
					st	=	-1;
					break;
				}
			}
			else				codebf[clen++]	=	str[i];
		}
		else {
			st	=	-1;
			break;
		}
	}
	if( st != -1 ) {
		::ZeroMemory( data, sizeof (struct _KamokuData) );
	
		if( klen ) {
			memcpy( data->kd_kana, kanabf, sizeof data->kd_kana );
			data->kd_sgn = -1;
		}
		else {
			memcpy( data->kd_code, codebf, sizeof data->kd_code );
			if( edabf[0] != '\0' )
				data->kd_eda = atoi( edabf );
			else
				data->kd_eda = -1;
			// ICSInput の chk_kmk.sgn と同じサインをセットする
			if( edasw ) {
				if( elen == 0 ) {
					if( clen == 0 )		 data->kd_sgn = 3;
					else if( clen >= 3 ) data->kd_sgn = 4;	
					else				 data->kd_sgn = 5;	
				}
				else {
					if( clen == 0 )			data->kd_sgn = 2;
					else if( clen >= 3 )	data->kd_sgn = 0;
					else					data->kd_sgn = 1;
				}
			}
			else {
				if( clen >= 3 )	data->kd_sgn = 0;
				else			data->kd_sgn = 1;
			}
		}
	}

	return st;
}



// 摘要 文字列のチェック
//	返送値
//	通常の文字列	= 0
//	番号のみ		= 1
//	ひらがな／カタカナのみ = 2
//	
extern "C" DBSUBFNC
int WINAPI TekiyoStr_Chk( LPCTSTR tkstr )
{
	int	i, clen, klen, st, c1, c2, end;
	size_t	tklen;
	char str[128] = {0};
	char kanabf[128] = {0}, codebf[128] = {0};
	tklen = strlen( tkstr );
	if( tklen > 128 )	tklen = 128;	// 128 byte 以上は処理しない

	clen = klen = end = 0;
	for( i = 0; i < tklen && end == 0; ) {
		st = IsKindOfLetter( tkstr, i );
		c1 = (BYTE)tkstr[i];
		
		switch( st ) {
		case 1:		// ASCII
			if( c1 >= 0x30 && c1 <= 0x39 ) {		// 数字
				if( klen )	end = 1;
				else		clen++;
			}
			else if( c1 >= 0xa6 && c1 <= 0xdd || c1 == 0x2d ) {	// ｦ ～ ﾝ	'-' 追加 [03.22 /12]
				if( clen )	end = 1;
				else		klen++;
			}
			else	end = 1;
			i++;
			break;
		case 2:		// MB文字1バイト目
			c2 = (BYTE)tkstr[i+1];

			if( c1 == 0x82 ) {
				if( c2 >= 0x4f && c2 <= 0x58 ) {	//全角数字
					if( klen )	end = 1;
					else		clen++;
				}
				else if( c2 >= 0x9f && c2 <= 0xf1 ) { // ひらがな
					if( clen )	end = 1;
					else		klen++;
				}
				else	end = 1;
			}
			else if( c1 == 0x83 ) {	
				if( (c2 >= 0x40 && c2 <= 0x96) && c2 != 0x7f) { // カタカナ
					if( clen )	end = 1;
					else		klen++;
				}
				else	end = 1;
			}
			else if( c1 == 0x81 ) {	// コーヒー などの 伸ばす記号　漏れ [11/12 /07]
				if( c2 == 0x5b || c2 == 0x7c ) { //全角ハイフン '－' 　漏れ [04/10 /13]
					if( clen )	end = 1;
					else		klen++;
				}
				else	end = 1;	// 修正漏れ [12.17 /07]
			}
			else	end = 1;
			i += 2;
			break;
		default:
			return -1;
		}
	}

	int rval;
	if( ! end )
		rval = clen ? 1 : 2;
	else
		rval = 0;

	return rval;
}


//	科目文字列チェック
//
extern "C" DBSUBFNC 
int WINAPI KamokuStr_ChkLength( LPCTSTR string, struct _KamokuData* data, int edcol )
{
	int		i, clen, elen, klen, st, edasw;
	size_t	length;
	char str[128] = {0};
	char kanabf[20] = {0}, edabf[20] = {0}, codebf[20] = {0};
	strcpy_s( str, sizeof str, string );
	length = strlen( (LPCTSTR)str );
	if( length > sizeof codebf )
		length = sizeof codebf - 1;
	
	st = edasw = 0;
	clen	=	elen	=	klen	= 0;
	for( i = 0; i < length; i++ ) {
		if( str[i] == 0x2e || str[i] == 0xa5 ) {// 枝番かどうかの 点(.)
			if( edcol <= 0 )	break;
			edasw	=	TRUE;
		}
		else if( _ismbbkana( str[i] ) ) {
			if( clen || elen ) {
				st	=	-1;
				break;
			}
			kanabf[klen++]	=	str[i];
		}
		else if( IsCharAlphaNumeric( str[i] ) ) {
			if( klen ) {
				st	=	-1;
				break;
			}
			if( edasw == TRUE ) {
				if( !IsCharAlpha( str[i] ) )	edabf[elen++]	=	str[i];
				else	{
					st	=	-1;
					break;
				}
			}
			else				codebf[clen++]	=	str[i];
		}
		else {
			st	=	-1;
			break;
		}
	}
	if( st != -1 ) {
		::ZeroMemory( data, sizeof (struct _KamokuData) );
	
		if( klen ) {
			memcpy( data->kd_kana, kanabf, sizeof data->kd_kana );
			data->kd_sgn = -1;
		}
		else {
			memcpy( data->kd_code, codebf, sizeof data->kd_code - 1 );
			if( edabf[0] != '\0' ) {
				if( elen > edcol ) {
					data->kd_eda = -1;
					::ZeroMemory( data->kd_code, sizeof data->kd_code );
					return -1;
				//	MessageBeep(0xffffffff);
				}
				else {
					data->kd_eda = atoi( edabf );
				}
			}
			else
				data->kd_eda = -1;
			// ICSInput の chk_kmk.sgn と同じサインをセットする
			if( edasw ) {
				if( elen == 0 ) {
					if( clen == 0 )		 data->kd_sgn = 3;
					else if( clen >= 3 ) data->kd_sgn = 4;	
					else				 data->kd_sgn = 5;	
				}
				else {
					if( clen == 0 )			data->kd_sgn = 2;
					else if( clen >= 3 )	data->kd_sgn = 0;
					else					data->kd_sgn = 1;
				}
			}
			else {
				if( clen >= 3 )	data->kd_sgn = 0;
				else			data->kd_sgn = 1;
			}
		}
	}

	return st;
}

