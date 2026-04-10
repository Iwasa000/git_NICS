// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Windows ヘッダーから使用されていない部分を除外します。
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 一部の CString コンストラクターは明示的です。

#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
#include <afxext.h>         // MFC の拡張部分

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE クラス
#include <afxodlgs.h>       // MFC OLE ダイアログ クラス
#include <afxdisp.h>        // MFC オートメーション クラス
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC データベース クラス
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO データベース クラス
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC の Internet Explorer 4 コモン コントロール サポート
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC の Windows コモン コントロール サポート
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "Resource.h"

#include <vector>
#include <algorithm>

#include <icspackdef.h>
#include <dbzmsub8.h>
#include <ICSGEN80.h>
#include <VolDateDB8.h>
#include "ICSArith8.h"
#include <SnComDef.h>
#include <DBSyzShinCom8.h>

#include "SimKuriwariW.h"
#include <afxcontrolbars.h>
#include <afxdtctl.h>
#include <afxdtctl.h>
#include <afxwin.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>

const COLORREF WHITE_COLOR  = RGB(0xff, 0xff, 0xff); 
const COLORREF BLACK_COLOR  = RGB(0x00, 0x00, 0x00);
const COLORREF RED_COLOR    = RGB(0xff, 0x00, 0x00);
const COLORREF BLUE_COLOR = RGB(0x00, 0x00, 0xff);
const COLORREF YELLOW_COLOR = RGB(0xff, 0xff, 0x00);
const COLORREF GRAY_COLOR = RGB(0xc0, 0xc0, 0xc0);
const COLORREF BLUEGREEN_COLOR = RGB(0x00, 0x80, 0x80);
const COLORREF GREY25_COLOR = RGB(0x00, 0x80, 0x80);

static char* KaribaraiSyohizei = _T("01071101");
static char* KariukeSyohizei = _T("04051601");
const int INVOICE_SYZ_YMD = 20231001;			// 消費税：インボイス施行日


void DbgViewTrace(LPCTSTR lpszFormat, ...);