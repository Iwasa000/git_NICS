// stdafx.cpp : 標準インクルード SyzMakeSub8.pch のみを
// 含むソース ファイルは、プリコンパイル済みヘッダーになります。
// stdafx.obj にはプリコンパイル済み型情報が含まれます。

#include "stdafx.h"

void DbgViewTrace(LPCTSTR lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);

	int nBuf;
	TCHAR szBuffer[1024];

	nBuf = vsprintf_s(szBuffer, sizeof szBuffer, lpszFormat, args);
	ASSERT(nBuf < sizeof(szBuffer));

	OutputDebugString(szBuffer);

	va_end(args);
}


