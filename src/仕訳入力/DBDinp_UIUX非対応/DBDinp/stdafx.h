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

#include <afxdb.h>

#include <set>
#include <map>
#include <queue>
#include <deque>
#include <vector>
#include <algorithm>
#include <atlimage.h>
#include <memory>

using namespace std;

#include <ICSGEN80.h>
#include <docsub8.h>
#include <ImgMng8.h>	// _SLIP_ACCOUNTS_

#include <BatchVerify8.h>
#include <DocPrintRec8.h>
#include <DocPrint8.h>
#include <ICSPrn8.h>

#include "DBSubFnc8.h"
#include "ocxdef.h"
#include "DBZmSub8.h"
#include "dbzmdef8.h"
#include "DBSyCode8.h"
#include "DBSyohi8.h"

#include "DBDinpRec.h" //CDBipDataRecは使用しない(DBDinp.EXEでは)
#include "../DBInpSub/DBInpSub8.h"

#include "DBNpSub8.h"
#include "DipZmSub.h"
#include "DBNpCal8.h"

#include "ICSWindows8.h"
#include "icsgenl8.h"
#include "VolDateDB8.h"
#include "ICSArith8.h"
#include "DBKmksel8.h"
#include "ZmselDB.h"

#include "MkrdSub.h"

#include "../DBInpSub/BrnTkyDef.h"
#include "../DBInpSub/DinpINI.h"
#include "../DBInpSub/BrnTky.h"
#include "../DBInpSub/SyzInp.h"
#include "../DBInpSub/HusenInp.h"
#include "../DBInpSub/HjiyuInp.h"

#include "../DBInpSub/SelectData.h"
#include "../DBInpSub/DataZeiCalq.h"

#include "MessageDef.h"

#include "ocrdef.h"

#include "errblk.h"
#include "errormsg.h"
#include "zinpsub.h"

#include <vctldb8.h>
#include <DinpCol.h>

#include "SubDataInp.h"

#include "subfnc.h"

#include <afxext.h>
#include <afxdisp.h>
#include <afxdisp.h>
#include <afxdb.h>

#define _BLK_DATA_
#define _BLKINS_1
#define _BLKINS_2

#include "BlkIns.h"

#ifdef _SLIP_ACCOUNTS_
#include "IconOwnerButton.h"
#endif
#include <jz2cloudopt.h>
//#include <jzcheck.h>
//------------------------------------------
//   製品導入済みチェック（Atomの場合は、atom_enable()を呼び出す）
// 返送値
//	  0　: 購入済み
// 　-1　: 環境外
// 　-2　: 期限切れ（ATOMの場合）
// 　-3　: 未購入
//------------------------------------------
AFX_EXT_API int WINAPI IsPakageCd( CString pcode );


#include "SelGuideCtrl.h"

#include <Subtool8.h>
#include <CoinfCtl8.h>

#include <kmnctrl8.h>
#include <DBprogress_Mod8.h>
#include "DBProgTuutiGen8.h"

#include <ZmGen8.h>

#include "resource.h"
#include "RDSel.h"

#include <ZIM_TkjourUtil_Rec.h>
#include <ZIM_TkjourUtil.h>
#include <ReadOCRCnv8.h>

#include <DBana8.h>
#include <VintaPDFviewer.h>
#include <CreditOcrUtil.h>
#include <afxwin.h>
#include <CAddInvData.h>

/////////////////////////////////////////////////////////////////////////////


// 仕訳データサイン バイト数
#define SZ_DSGN			16
#define SZ_SYZDSGN		7
#define DDATA_LNMAX		40		//40行まで表示

#define TKYO_LEN		40

#define SYGET_TYPE		0x07

#define SONEKICODE		0x08000000

#define	DINP_BLKMAX		5

#define	BLK_SELECT		1
#define	BLK_DTLIST		2
#define	BLK_INPTYPE		3
#define	BLK_DTNEW		4
#define	BLK_GNKIN		5


#define	RDADD_MAX		100

//インボイス登録番号 バイト数
#define SZ_INVNO		16


// マスターフラグ
#define	MST_INSERT		0x00000001
#define MST_TKY20		0x00000002
#define MST_SYZ5_8		0x00000004
#define MST_SYZ8		0x00000008
#define MST_SYZ8_10		0x00000010
#define MST_SYZ10		0x00000020
#define MST_SYZ_INV		0x00000040

// SetMasterDB vers
#define	SMDB_VERS		3


void MyTrace( LPCTSTR lpszFormat, ...);
void MyDebugFileOut( LPCTSTR lpszFormat, ...);


//#define FILETrace			MyDebugFileOut
//#define FILETrace			MyTrace
#define FILETrace			ATLTRACE

#define SRCTrace			MyTrace( "src = %s [%d]\n", __FILE__, __LINE__ )

#define SRCMsgBox		{ CString msg;	msg.Format("src = %s [%d]\n", __FILE__, __LINE__ ); AfxMessageBox(msg); }

//#define DSIGN_TRACE(dsign)	MyTrace("%s(L=%d):dsign[6] %02x%02x%02x%02x%02x%02x%02x\n", __FUNCTION__, __LINE__, dsign[0], dsign[1], dsign[2], dsign[3], dsign[4], dsign[5], dsign[6] );
#define DSIGN_TRACE(dsign)	__noop;

//#define DBGFUNCTION(s)	MyTrace("@%s, LINE(%d)--%s\n", __FUNCTION__, __LINE__, s)
#define DBGFUNCTION			__noop;

#ifdef _DEBUG
#define L_PRINT( s, v )	{ char a[36];	l_print( a, v, "sss,sss,sss,ss9" );\
							FILETrace( "%s = %s", s, a );	}
#else
#define L_PRINT( s, v )	{}
#endif


#ifndef _PERFORMANCE_CHK

#define	BEGIN_PERFORMANCE		memset(&nFreq,   0x00, sizeof nFreq);\
								memset(&nBefore, 0x00, sizeof nBefore);\
								memset(&nAfter,  0x00, sizeof nAfter);\
								dwTime = 0;\
								QueryPerformanceFrequency(&nFreq);\
								QueryPerformanceCounter(&nBefore);\

#define END_PERFORMANCE(x)		QueryPerformanceCounter(&nAfter);\
								dwTime = (DWORD)((nAfter.QuadPart - nBefore.QuadPart) * 1000 / nFreq.QuadPart);\
								MyDebugFileOut("%s: %d ミリ秒\n", x, dwTime);\


#endif


#define	CHECK_UCSTR1	_T("依頼チェック1")
#define	CHECK_UCSTR2	_T("依頼チェック2")
#define	CHECK_UCSTR3	_T("依頼チェック3")
#define	CHECK_UCSTR4	_T("依頼チェック4")

// 一括終了のための メッセージボックス
int myICSMessageBox( LPCTSTR text, UINT type = MB_OK, UINT help = 0, short point = 0, CWnd* pParent = NULL );
int myICSExtBox( LPCTSTR title, LPCTSTR text, LPCTSTR btn1text, LPCTSTR btn2text, LPCTSTR btn3text, UINT type, UINT help = 0, short point = 0, CWnd* pParent = NULL );

#define	PRO_NAME	"DBDinp"
#define SETUP_NAME	"DBDINP_SETUP2"



#define SYZ8_YMD	20140401
#define KANI6_YMD	20150401
#define RVCHAR_YMD	20151001
#define SYZ10_YMD	20191001
#define SYZ_INV_YMD	20231001

#define SYZMEN1_YMD	20231001
#define SYZMEN2_YMD	20260930
#define SYZMEN3_YMD	20261001
#define SYZMEN4_YMD	20291001

#define DOCSHT_NONSUP	-1

#define JZCLOUD_SEC		"Ledscn8"
#define JZCLOUD_KEY		"CloudFlg"

enum DTCLM_TYPE {
	DTCLM_LINE1 = 1,
	DTCLM_LINE2,
	DTCLM_SNOL1,
	DTCLM_SNOL2,
};


#define OPT_SECTION		"DBDINP"
#define OPT_KEYTKYREG	"TkyReg"		// 摘要登録
#define OPT_KEYVALCPY	"ValCopy"		// 金額複写
#define OPT_KEYZMOVE	"Zmove"			// 残高問合せ移行
#define OPT_KEYTKTAB	"Tktab"			// 摘要欄Tabキーの動作
#define OPT_KEYDENPTYPE	"DenpType"		// 伝票形式タイプを表示する

#define OPT_RDZRITSU	"RDZRituAuto"	// 定型仕訳 日付で税率自動判定

#define OPT_KANASEARCH	"KanaStype"		// 科目カナ検索タイプ
#define		SIZE_DSIGN	6

#define OPT_SYZSTOP		"SyzStop"		// 消費税欄 毎回確認

#define	INVKANA_MAX	16

//重複仕訳ビットサイン
#define DS_DENP_BIT		0x01
#define DS_BMN_BIT		0x02
#define DS_KOJI_BIT		0x04
#define DS_TKY_BIT		0x08










