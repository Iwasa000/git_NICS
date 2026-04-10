// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Windows ヘッダーから使用されていない部分を除外します。
#endif

// 下で指定された定義の前に対象プラットフォームを指定しなければならない場合、以下の定義を変更してください。
// 異なるプラットフォームに対応する値に関する最新情報については、MSDN を参照してください。
#ifndef WINVER				// Windows 95 および Windows NT 4 以降のバージョンに固有の機能の使用を許可します。
#define WINVER 0x0501		// これを Windows 98 および Windows 2000 またはそれ以降のバージョン向けに適切な値に変更してください。
#endif

#ifndef _WIN32_WINNT		// Windows NT 4 以降のバージョンに固有の機能の使用を許可します。
#define _WIN32_WINNT 0x0501		// これを Windows 98 および Windows 2000 またはそれ以降のバージョン向けに適切な値に変更してください。
#endif						

#ifndef _WIN32_WINDOWS		// Windows 98 以降のバージョンに固有の機能の使用を許可します。
#define _WIN32_WINDOWS 0x0501 // これを Windows Me またはそれ以降のバージョン向けに適切な値に変更してください。
#endif

#ifndef _WIN32_IE			// IE 4.0 以降のバージョンに固有の機能の使用を許可します。
#define _WIN32_IE 0x0501	// これを IE 5.0  またはそれ以降のバージョン向けに適切な値に変更してください。
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 一部の CString コンストラクタは明示的です。

// 一般的で無視しても安全な MFC の警告メッセージの一部の非表示を解除します。
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
#include <afxext.h>         // MFC の拡張部分
#include <afxdisp.h>        // MFC オートメーション クラス

#include <afxdtctl.h>		// MFC の Internet Explorer 4 コモン コントロール サポート
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC の Windows コモン コントロール サポート
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <icspackdef.h>
#include <ICSGENL8.h>
#include <ICSGEN80.h>
#include <dbzmsub8.h>
#include <DBPrtbl8.h>
#include <VolDateDB8.H>
#include <afxdb.h>	        // CRecordset 使用のため追加
#include <afxtempl.h>		// CMap 使用のため追加
#include "UcCommon.h"		// 内訳書 共通宣言
#include "UcRenCommon.h"	// 内訳書財務連動 共通宣言
#include "UcInpMaxCommon.h"	// 内訳書 最大入力値 共通宣言

#include <CoSel8.h>			// 会社選択				2010/07/XX 一括印刷対応
#include "OwntbData.h"		// owntbデータクラス	2010/07/XX 一括印刷対応
//#include "CtrlSubGeneralVar.h"
#include "CtrlMainGeneralVar.h"
#include "ZmGen8.h"			// 消費税8%対応(2013/9/10)
#include <vector>
#include <map>
#include <algorithm>
#include "CharController.h"
// midori 180101 del -->
#include "ICSZipAd8.h"			//	住所検索用ヘッダ
// midori 180101 del <--

// midori 181101 add -->
#include <KaigenInfo8.h>
// midori 181101 add <--

// midori 151730,151263 add -->
// midori 190505 del -->
//#include "DBNpSub8.h"
// midori 190505 del <--
// midori 190505 add -->
#include <DBNpSub8.h>
// midori 190505 add <--

// 24/01/30_23-0845 add -->
#include "jz2cloudopt.h"
// 24/01/30_23-0845 add <--

// 改良No.22-0404,22-0812 add -->
#include	"colorbtn.h"
//#include <afxcontrolbars.h>
//#include <afxcontrolbars.h>
//#include <afxext.h>
//#include <afxext.h>
//#include <afxext.h>
//#include <afxext.h>
//#include <afxext.h>
//#include <afxext.h>
//#include <afxext.h>
//#include <afxext.h>
//#include <afxext.h>
//#include <afxext.h>
//#include <afxext.h>
//#include <afxext.h>
//#include <afxext.h>
//#include <afxext.h>
//#include <afxext.h>
// 改良No.22-0404,22-0812 add <--

// インボイス登録番号追加対応_23/11/06 add -->
#include	<SyzInvoice.h>	// 修正No.168516 add
#include	<HJAccess.h>
#include	<imgmng8.h>
#include	<ZmselDB.h>
// インボイス登録番号追加対応_23/11/06 add <--

# define isHIEIRI(ap) ((ap & 0xf0) == 0x50)
# define isKYUKAIKEI( ap,ver ) (ap == 0x50 || (ap == 0x52 && ver < 0x11) || (ap == 0x58 && ver < 0x11))
// midori 151730,151263 add <--

// midori 20/10/08_2 add -->
#define	prnConfimationSw	FALSE
// midori 20/10/08_2 add <--

// ユーザ定義メッセージ
#define WM_END_MAIN				(WM_USER + 100)
#define WM_END_VIEW				(WM_USER + 200)
#define WM_YOUSHIKI_SELECT		WM_APP + 0x01	// WM_USERでもいいかも？
#define WM_SETCURSORBUTEPOS		(WM_USER +101)
// midori 160612 add -->
#define	WM_USER_UPDATE_INDICATE	(WM_USER +102)
// midori 160612 add <--
// midori M-16090701 add -->
#define	WM_USER_FOCUSSET		(WM_USER +103)
// midori M-16090701 add <--
// midori 152133 add -->
#define	WM_USER_RCCLICK			(WM_USER +104)
// midori 152133 add <--
// midori 155879 add -->
#define	WM_USER_CONTROLPAGE		(WM_USER +105)
// midori 155879 add <--
// 修正No.157633_元号日付 add -->
#define	WM_USER_FOCUSSET2		(WM_USER +106)
// 修正No.157633_元号日付 add <--
// No.158072 add -->
#define	WM_USER_REPORT_SORT		(WM_USER +107)
// No.158072 add <--
#define	WM_USER_1				(WM_USER +108)	// 改良No.22-0404,22-0812 add
#define	WM_USER_SELECTCHECK		(WM_USER +109)	// 修正No.162394 add
#define	WM_USER_FOCUSSET3		(WM_USER +110)	// 修正_23/04/21 add
#define	WM_USER_REDRAWVIEW		(WM_USER +111)	// 修正No.162446 add
#define	WM_USER_MESSAGESET		(WM_USER +112)	// インボイス登録番号追加対応_23/11/06 add
#define	WM_ZMSEL_MESSAGE		(WM_USER +113)	// インボイス登録番号追加対応_23/11/06 add
#define	WM_USER_FOCUSSET4		(WM_USER +114)	// 修正No.168393 add

// midori 151393 add -->
typedef struct FORM_TYPE_DATA_{
	FORM_TYPE_DATA_(){
		int FormSeq = 0;
		int FormType = 0;
		int num = 2;
	}
	int FormSeq;
	int FormType;
	int num;
}FORM_TYPE_DATA;
// midori 151393 add <--

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif



void DbgViewTrace( LPCTSTR lpszFormat, ...);

#pragma warning (disable:4018 4267)