// DBSyzShinCom.cpp : DLL の初期化ルーチンです。
//

#include "stdafx.h"
#include <afxdllx.h>
#ifdef _MANAGED
#error /clr と共にコンパイルするには DBSyzShinCom.cpp 内の操作手順を参照してください。
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


static AFX_EXTENSION_MODULE DBSyzShinComDLL = { NULL, NULL };

#ifdef _MANAGED
#pragma managed(push, off)
#endif

HINSTANCE shComInstance = 0;

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// lpReserved を使う場合はここを削除してください
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("DBSyzShinCom.DLL Initializing!\n");
		
		// 拡張 DLL を 1 回だけ初期化します。
		if (!AfxInitExtensionModule(DBSyzShinComDLL, hInstance))
			return 0;

		shComInstance = hInstance;

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

		new CDynLinkLibrary(DBSyzShinComDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("DBSyzShinCom.DLL Terminating!\n");

		// デストラクタが呼び出される前にライブラリを終了します
		AfxTermExtensionModule(DBSyzShinComDLL);
	}
	return 1;   // OK
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

