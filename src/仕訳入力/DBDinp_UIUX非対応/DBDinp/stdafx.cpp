// stdafx.cpp : 標準インクルードを含むソース ファイルです。
// DBDinp.pch は、プリコンパイル済みヘッダーになります。
// stdafx.obj にはプリコンパイル済み方情報が含まれます。

#include "stdafx.h"


//#ifdef CLOSE
//////////////////////////////////////////
// PERFORMANCE
LARGE_INTEGER nFreq, nBefore, nAfter;
DWORD dwTime;
//////////////////////////////////////////
//#endif


void MyTrace( LPCTSTR lpszFormat, ...)
{
	return;		//ユーザリリース版はデバッグ文字列出さない
	//**********//

	va_list args;
	va_start(args, lpszFormat);

	int nBuf;
	TCHAR szBuffer[1024];

	nBuf = vsprintf_s(szBuffer, sizeof szBuffer, lpszFormat, args);
	ASSERT(nBuf < sizeof(szBuffer));

	OutputDebugString( szBuffer );

	va_end(args);
}

void MyDebugFileOut( LPCTSTR lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);

	TCHAR filename[_MAX_PATH] = {0};
	GetModuleFileName(NULL, filename, sizeof filename );

	CString fopn_path;
	if( filename[0] ) {
		CFileFind	f;
//		fopn_path.Format( "%c:\\ICSWin\\Log", filename[0] );
		char	wkPath[256];
		COM_GetWorkDirectory( wkPath );
		fopn_path = wkPath;

//MyTrace( "wkPath = %s\n", wkPath );

		if( f.FindFile( fopn_path ) ) {
			f.FindNextFileA();

			if( ! f.IsDirectory() ) {
				f.Close();
				return;
			}
			
			f.Close();
		}
		char* p = strrchr( filename, '\\' );
		if( NULL == p )	return;

		for( int i = 1; *(p+i) != '\0'; i++ ) {
			if( *(p+i) == '.' ) {
				*(p+i) = '\0';
				break;
			}
		}

		fopn_path.Format( "%s\\%s.log", wkPath, p+1 );
//		fopn_path.Format( "%c:\\ICSWin\\Log\\%s.log", filename[0], p+1 );
	}
	else {
		return;
	}	
	
	FILE* f; 
	errno_t err;
	err = fopen_s( &f, fopn_path, "a+t" );

#ifdef CLOSE
	HANDLE	hFile;

	hFile = CreateFile( fopn_path,
						GENERIC_READ|GENERIC_WRITE,
						0,
						0,
						OPEN_ALWAYS,
						FILE_ATTRIBUTE_NORMAL,
						NULL );

	if( hFile == INVALID_HANDLE_VALUE )
		return;


	OVERLAPPED ova;
	ova.hEvent = 0;
	ova.Offset = ova.OffsetHigh = 0; // 初期値設定

　	if( ! LockFileEx(hAddFile, LOCKFILE_EXCLUSIVE_LOCK, 0, 0, GetFileSize(hAddFile, NULL), &ova) )
		return;
#endif

	if( err == 0 ) {
		SYSTEMTIME	stm;
		GetLocalTime( &stm );

		CString tstr;
		tstr.Format( "%5d/%2d/%2d %2d:%02d:%02d.%03d", stm.wYear, stm.wMonth, stm.wDay, 
													stm.wHour, stm.wMinute, stm.wSecond, stm.wMilliseconds );
		CString pid;
		pid.Format( "%x=", GetCurrentProcessId() );

		int nBuf;
		TCHAR szBuffer[1024];
		sprintf_s( szBuffer, sizeof szBuffer, "%s%s ", (LPCTSTR)pid, (LPCTSTR)tstr );

		fprintf_s( f, "%s%s ", (LPCTSTR)pid, (LPCTSTR)tstr );

		nBuf = vsprintf_s(szBuffer, sizeof szBuffer, lpszFormat, args);
		ASSERT(nBuf < sizeof(szBuffer));

		fprintf_s( f, "%s", szBuffer );
		fclose( f );

	}

	va_end(args);
}

