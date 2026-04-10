//CRevConvSub 

#include "stdafx.h"

#include "RevConvSub.h"

#include "comutil.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////
// 実際に 他から呼ばれるクラス
//
CRevConv::CRevConv()
{
	pSub = new CRevConvSub;
}


CRevConv::~CRevConv()
{
	if( pSub != NULL )
		delete pSub;
}


int CRevConv::GetFurigana( DWORD dwOpt, CString& str, CString& kana )
{
	return pSub->_GetFurigana( dwOpt, str, kana );
}


//////////////////////////////////////////////////
CRevConvSub::CRevConvSub()
{
	m_bInit = FALSE;
	pIFE = NULL;

	// COM ライブラリを初期化。
	HRESULT hr;
	hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	int i;

	if( SUCCEEDED(hr) ) {
		m_bInit = TRUE;

		try {
			CLSID clsid;
			char msime[32];
			
			strcpy_s( msime, sizeof(msime), "MSIME.Japan" );

			CComBSTR bstr;
			bstr.Empty();
			bstr.Append( msime );

			// 文字列の CLSID から CLSID へのポインタを取得する
			hr = CLSIDFromString( bstr, &clsid);

			if( FAILED( hr )) {
				for( i = imeVersionMax; i >= imeVersionMin; i-- ) {
					sprintf_s( msime, sizeof msime, "MSIME.Japan.%d", i );
					bstr.Empty();
					bstr.Append( msime );

					{
						hr = CLSIDFromString( bstr, &clsid);
						if( SUCCEEDED(hr) ) {
							break;
						}
					}
				}
				if( i < imeVersionMin )
					return;
			}
			hr	=	::CoCreateInstance( clsid,
										NULL,
										CLSCTX_ALL,
										IID_IFELanguage,
										(void**)&pIFE );
			if( FAILED(hr) ) {
				throw hr;
			}

			hr = pIFE->Open();

			if( FAILED(hr) ) {
				throw hr;
			}
		}
		catch( HRESULT hResult ) 
		{
			TRACE( "CRevConvSub HRESULT = %08x\n", hResult );
		}
	}
}


CRevConvSub::~CRevConvSub()
{
	if( pIFE != NULL ) {
		pIFE->Close();
		pIFE->Release();
	}

	// COM ライブラリを解放する。
	if( m_bInit ) {
		CoUninitialize();
	}
}


int CRevConvSub::_GetFurigana( DWORD dwOpt, CString& strText, CString& kana )
{
	CComBSTR bstr;
//	strText = "明日";
	bstr.Append( (LPCTSTR)strText );

	int wLen = bstr.Length();;
	long length = wLen;
	HRESULT hr;

	if( pIFE == NULL )
		return -1;

#ifdef CLOSE
	int k, byte, st;
	byte = strText.GetLength();
	long length = byte;

	for( k = 0; k < byte; ) {

		st = IsKindOfLetter( (LPCTSTR)strText, k );
		switch( st ) {
		case 1:		// ASCII
			length++;
			k++;	
			break;
		case 2:		// MB文字1バイト目
			length++;
			k += 2;
			break;
		default:
			break;
		}
	}
#endif

	if( length != 0 ) {
		BSTR ret;
		hr = pIFE->GetPhonetic( (BSTR)bstr, 1, length, &ret );

		if( hr == S_OK ) {
			CString chkStr, strTmp;
			_bstr_t	bstrTmp;
			bstrTmp = ret;

			chkStr.Format( "%s", (LPCTSTR)bstrTmp );
			int len = chkStr.GetLength();
				
			if( dwOpt & (RVF_HANKAKU|RVF_KATAKANA) ) {
				int bfsiz = (len + 32);
				LPSTR lpStr = strTmp.GetBuffer( bfsiz );
				::ZeroMemory( lpStr, bfsiz );
				
				DWORD    dwMapFlags = 0;

				if( dwOpt & RVF_KATAKANA )	dwMapFlags |= LCMAP_KATAKANA;
				if( dwOpt & RVF_HANKAKU )	dwMapFlags |= LCMAP_HALFWIDTH;

				LCMapString( GetUserDefaultLCID(),
							 dwMapFlags, chkStr, len, lpStr, bfsiz );

				strTmp.ReleaseBuffer();
			}
			else {
				strTmp = chkStr;
			}

			kana = strTmp;
			SysFreeString( ret );
		}
		else {
			return -1;
		}
	}

	return 0;
}


#ifdef _SAMPLE_BSTR_HENKAN_


int main(char* argv[])
{
	//char を BSTR に変換-----------------------------------------------

	char*buf="いなばabcd";
	LPSTR lstr=buf;

	BSTR bstr;

	//BSTRに必要なバッファサイズを求める
	int bstrlen=(int)MultiByteToWideChar(
		CP_ACP,			// コードページ ANSI コードページ
		0,			// 文字の種類を指定するフラグ
		lstr,			// マップ元文字列のアドレス
		strlen(lstr),		// マップ元文字列のバイト数
		NULL,			// マップ先ワイド文字列を入れるバッファのアドレス
		0			// バッファのサイズ
		);

	//バッファを確保する
	bstr=SysAllocStringLen(NULL,bstrlen);

	//BSTRに複製
	MultiByteToWideChar(
		CP_ACP,
		0,
		lstr,
		strlen(lstr),
		bstr,
		bstrlen
		);


	//BSTR を char に変換----------------------------------------------

	char buf2[64]="";

	WideCharToMultiByte(
		CP_ACP,			// コードページ ANSI コードページ
		0,			// 処理速度とマッピング方法を決定するフラグ
		(OLECHAR*)bstr,		// ワイド文字列のアドレス
		-1,			// ワイド文字列の文字数
		buf2,			// 新しい文字列を受け取るバッファのアドレス
		sizeof(buf2) - 1,		// 新しい文字列を受け取るバッファのサイズ
		NULL,			// マップできない文字の既定値のアドレス
		NULL			// 既定の文字を使ったときにセットするフラグのアドレス
	);

	printf(buf2);
	return 0;
}



#endif