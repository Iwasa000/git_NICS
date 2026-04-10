// stdafx.cpp : 標準インクルードを含むソース ファイルです。
// SyzAcsTest.pch は、プリコンパイル済みヘッダーになります。
// stdafx.obj にはプリコンパイル済み方情報が含まれます。

#include "stdafx.h"

void DbgViewTrace(LPCTSTR lpszFormat, ...)
{
#ifdef _DEBUG
	va_list args;
	va_start(args, lpszFormat);

	int nBuf;
	TCHAR szBuffer[1024];

	nBuf = vsprintf_s(szBuffer, sizeof szBuffer, lpszFormat, args);
	ASSERT(nBuf < sizeof(szBuffer));

	OutputDebugString(szBuffer);

	va_end(args);
#endif
}

