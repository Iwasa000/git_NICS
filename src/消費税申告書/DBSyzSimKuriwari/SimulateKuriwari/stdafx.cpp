// stdafx.cpp : 標準インクルードのみを含むソース ファイル
// SimulateKuriwari.pch はプリコンパイル済みヘッダーになります
// stdafx.obj にはプリコンパイル済み型情報が含まれます。

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

