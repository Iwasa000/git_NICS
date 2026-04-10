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

#define _SYZ_H26_HYO_EXPORT_CHANGE_		1

#include <icspackdef.h>
#include <ICSGenl8.h>
#include <ICSGEN80.h>

//#include <ICSWindows8.h>
#include <ICSTblsub8.h>
#include <ICSArith8.h>
#include <icsocx.h>
#include "resource.h"

#include <dbzmsub8.h>
#include <dbzmdef8.h>
#include <DBNpSub8.h>
#include <icsprn8.h>
#include <Subtool8.h>
#include <DBSyzAccess8.h>
#include <icszipad8.h>
#include <xysprn8.h>

//#include "Util.h"
#include "Syz26ComDef.h"

// 共通定義
#include <SyzShinComDef.h>
#include "SyzBaseDlg.h"
#include <DBSyzShinCom8.h>
#include "DBSyzSkjoRec.h"

// 集計関連
#include <SyzSyukei8.h>

// 各帳票データ
#include <DBSyz26Data8.h>
//#include "H26SnHonpyoData.h"
//#include "H26SnFhyo10Data.h"
//#include "H26SnFhyo20Data.h"
//#include "H26SnTanaData.h"
//#include "H26SnFhyo50Data.h"
//#include "H26SnChskData.h"
#include "H26SnKskData.h"

#include "OrgResource.h"

// 還付
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include ".\SyzrefundDB\syzrefunddef.h"
#include ".\SyzrefundDB\DiagControlBase.h"
#include ".\SyzrefundDB\H26SnRefundData.h"
#include "RefundBaseDialog.h"
#include <voldatedb8.h>

//2015.10.20 INSERT START
#include "NoTblMod8.h"
//2015.10.20 INSERT END
//2015.12.03 INSERT START
#include <ICSMNSub8.h>
//2015.12.03 INSERT END
//2015.12.08 INSERT START
#include <ICSMNEdit8.h>
//2015.12.08 INSERT END

#include <KaigenInfo8.h>

#define FitView(view) \
		CRect parentrect;\
		GetClientRect(parentrect);\
		CRect childrect;\
		view.GetClientRect(childrect);\
		childrect.bottom += ( parentrect.bottom - childrect.bottom );\
		view.MoveWindow(childrect);\
		SetScrollPos(SB_HORZ,0,1);

//#define	_DBG_VIEW_START_	1	// DbgViewTrace を有効にする時には、このフラグを有効にしてください。

#ifdef _DBG_VIEW_START_
void DbgViewTrace( LPCTSTR lpszFormat, ...);
#endif
