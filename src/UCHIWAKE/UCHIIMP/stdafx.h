// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Windows ヘッダーから使用されていない部分を除外します。
#endif

// 下で指定された定義の前に対象プラットフォームを指定しなければならない場合、以下の定義を変更してください。
// 異なるプラットフォームに対応する値に関する最新情報については、MSDN を参照してください。
#ifndef WINVER				// Windows XP 以降のバージョンに固有の機能の使用を許可します。
#define WINVER 0x0501		// これを Windows の他のバージョン向けに適切な値に変更してください。
#endif

#ifndef _WIN32_WINNT		// Windows XP 以降のバージョンに固有の機能の使用を許可します。                   
#define _WIN32_WINNT 0x0501	// これを Windows の他のバージョン向けに適切な値に変更してください。
#endif						

#ifndef _WIN32_WINDOWS		// Windows 98 以降のバージョンに固有の機能の使用を許可します。
#define _WIN32_WINDOWS 0x0410 // これを Windows Me またはそれ以降のバージョン向けに適切な値に変更してください。
#endif

#ifndef _WIN32_IE			// IE 6.0 以降のバージョンに固有の機能の使用を許可します。
#define _WIN32_IE 0x0600	// これを IE. の他のバージョン向けに適切な値に変更してください。
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 一部の CString コンストラクタは明示的です。

#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
#include <afxext.h>         // MFC の拡張部分

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE クラス
#include <afxodlgs.h>       // MFC OLE ダイアログ クラス
#include <afxdisp.h>        // MFC オートメーション クラス
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC データベース クラス
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO データベース クラス
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC の Internet Explorer 4 コモン コントロール サポート
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC の Windows コモン コントロール サポート
#endif // _AFX_NO_AFXCMN_SUPPORT


#include <ICSGENL8.h>
#include <ICSGEN80.h>
// midori 161103 add -->
#include <dbzmsub8.h>
// midori 161103 add <--

#include "..\..\UCHIWAKE\UCHIWAKE\UcCommon.h"		// 内訳書 共通宣言
#include "..\..\UCHIWAKE\UCHIWAKE\UcRenCommon.h"	// 内訳書財務連動 共通宣言

// midori 161103 add -->
#include "ZmGen8.h"			// 消費税8%対応(2013/9/10)
// midori 161103 add <--

// midori 150894 cor -->
//#include "DBUchiImpDEF.h"
//#include "UcCsvCommon.h"
// -------------------------------
#include "UcCsvCommon.h"
#include "DBUchiImpDEF.h"
// midori 150894 cor <--

// midori 181101 add -->
#include <KaigenInfo8.h>
// midori 181101 add <--

// midori 190505 add -->
#include <DBNpSub8.h>
// midori 190505 add <--

// 登録番号（法人番号）_23/11/21 add -->
#include <SyzInvoice.h>
// 登録番号（法人番号）_23/11/21 add <--

// ユーザ定義メッセージ
#define WM_END_MAIN	(WM_USER + 100)
#define WM_END_VIEW	(WM_USER + 200)

// midori 20/10/08_2 add -->
#define	prnConfimationSw	FALSE
// midori 20/10/08_2 add <--

//#ifdef _UNICODE
//#if defined _M_IX86
//#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
//#elif defined _M_IA64
//#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
//#elif defined _M_X64
//#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
//#else
//#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
//#endif
//#endif
//
void DbgViewTrace( LPCTSTR lpszFormat, ...);