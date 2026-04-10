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


#include "icsgen80.h"

#include "icsocx.h"
#include "DBZmSub8.h"
#include "dbzmdef8.h"
#include "DBSyohi8.h"
#include "DBsycode8.h"
#include <_ICSSfam.h>
#include "DBSubFnc8.h"
#include "DBInpSub8.h"
#include "icsgenl8.h"
#include "VolDateDB8.h"
#include "ICSArith8.h"
#include <afxdisp.h>
#include <vctldb8.h>
#include "DinpCol.h"

#include <DBKmksel8.h>


#include <DBNPSUB8.H>

#include <ZmGen8.h>


// CSkbnLiteDlg ダイアログ
typedef struct _INVOICE_INFO
{
	int		inv_tkNo;		// 摘要番号
	char	inv_tkNam[64];	// 摘要名称
	char	inv_tkKana[8];
	char	inv_invNo[16];	// 登録番号
} INVOICE_INFO;

//インボイス番号情報の管理クラス
class CInvnoMng
{
public:
	CInvnoMng();
	~CInvnoMng();

	void SetInvArray(CArray<INVOICE_INFO> *ary);
	int  FukuKanaSearch(MSG* pMsg);
	int  KanaCheckVK_BACK(MSG* pMsg);
	int  InvNoSearch(LPSTR kana);

	void InvDispPage(int sgn);
	int  CancelIME();

public:
	CArray<INVOICE_INFO> *m_pDefInv;
	CArray<INVOICE_INFO> *m_pInvAry;
	CArray<INVOICE_INFO> m_AryInv;

	int  m_invPG;		//現在ページ数
	int  m_invPGCnt;	//ページ当たりデータ数

	//カナ検索を行うDBEDIT
	int		m_nID;
	HWND	m_hWnd;
	//ダイアログのポインタ
	CWnd*	m_pDlg;

	char dmy[256];
};

#include "BrnTkyDef.h"
#include "DinpINI.h"
#include "BrnTky.h"
#include "SyzInp.h"
#include "HusenInp.h"
#include "HjiyuInp.h"

#include "DBDinpRec.h"
#include "icssel.h"
#include "icsdisp.h"
#include "DataZeiCalq.h"
#include "CSWKDataSub.h"
#include "CSkbnLiteDlg.h"

#include "errormsg.h"
#include <KaigenInfo8.h>

#include <set>
#include <map>
#include <queue>
#include <deque>
#include <vector>
#include <algorithm>
#include <afxwin.h>
#include <ZIM_TkjourUtil.h>

#include <AnalyzeAJ.h>
#include "SelectData.h"
#include <afxwin.h>

using namespace std;



void MyTrace( LPCTSTR lpszFormat, ...);
void MyDebugFileOut( LPCTSTR lpszFormat, ...);

//#define FILETrace	MyDebugFileOut
//#define	FILETrace	MyTrace
#define	FILETrace	ATLTRACE

#define SRCTrace		MyTrace( "src = %s [%d]\n", __FILE__, __LINE__ )
//#define DBGFUNCTION		MyTrace("@%s, LINE(%d)\n", __FUNCTION__, __LINE__ )
#define DBGFUNCTION		__noop

#define DBGPTR(ptr)		MyTrace("%s:%08x\n", #ptr, ptr )

//#define DSIGN_TRACE(dsign)	MyTrace("%s:dsign[0-6] %02x%02x%02x%02x%02x%02x%02x\n", __FUNCTION__, dsign[0], dsign[1], dsign[2], dsign[3], dsign[4], dsign[5], dsign[6] );
//#define DSIGN_TRACE2(dsign)	MyTrace("%s:dsign[7-15] %02x%02x%02x%02x%02x%02x%02x%02x%02x\n", __FUNCTION__, dsign[7], dsign[8], dsign[9], dsign[10], dsign[11], dsign[12], dsign[13], dsign[14], dsign[15] );
#define DSIGN_TRACE(dsign)	__noop;
#define DSIGN_TRACE2(dsign)	__noop;



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

#define is_SHIFT()	( (GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0 )
#define Buzzer()	MessageBeep( 0xffffffff )

#define	WM_MYTABCHANGE		(WM_APP+100)
#define WM_USER_1			(WM_APP+101)
#define WM_TEKIADDJOB		(WM_APP+102)
#define WM_LIST_INSDEL		(WM_APP+103)
#define	WM_RETROFOCUS		(WM_APP+104)
#define WM_TKDICMESSAGE		(WM_APP+105)
#define WM_SELGUIDEMSG		(WM_APP+106)
#define	MESSAGE_IMGST_KEY	(WM_APP+107)
#define	MESSAGE_IMGST_FOCUS	(WM_APP+108)
#define WM_RETROFOCUS2		(WM_APP+109)
#define WM_FONT_SIZE		(WM_APP+110)
#define WM_MYMESSAGE		(WM_APP+111)
#define WM_MENZEI_MESSAGE	( WM_APP + 112 )
#define WM_KUBUN_MESSAGE	( WM_APP + 113 )
#define WM_INVNO_MESSAGE	( WM_APP + 114 )

// 消費税 インボイス登録番号用 WM_MYMESSAGE の wParam
	#define INV_KBNMAKE		0
	#define INV_ONTERM		1
	#define INV_ERRMSG		2

// 仕訳入力用 message
#define WM_DBDINPMSG	(WM_APP+110)
	#define DWP_LINECOUNT		0	// OnSize での 仕訳行数処理
	#define DWP_COPYMENU		1	// 右クリックコピー処理のメニュー表示
	#define DWP_DISPLINE		2	// 仕訳行の再表示

	#define DWP_SELECTED1		3	// 固定選択欄でのクリック選択
	#define DWP_SELECTED2		4	// 32科目選択欄でのクリック選択

	#define DWP_SETFOCUS		5	// フォーカスセット
	#define DWP_DENPDIALOG		6	// 伝票修正ダイアログ

	#define DWP_TKYSWKDLG		7

#define	DIS_ENDDIALOG		1

#define CL_BLACK		RGB(0, 0, 0)
#define CL_WHITE		RGB(255,255,255)

#define CL_TKBACK		RGB(255,255,240)
#define CL_RED			RGB(255,0,0)
#define CL_GRAY			RGB(192,192,192)
#define	CL_GREEN		RGB(0,128,128)
#define	CL_DKBLUE		RGB(0, 0, 128)

#define CL_WINE_BLUE	RGB(166,202,240)	//穏やかな水色

#define CL_YELLOW		RGB(255,255,128)	// 黄色


#define KMNLEN		14
#define KMKCD_LN	8

#define TKYO_LEN	40

#define	NUM_FMT		"SSSSS9"

#define	NUM_FMT4	"SSS9"
#define	NUM_FMT5	"SSSS9"

#define	NUM_FMTZ	"999999"

#define	NUM_FMTZ2	"99"
#define	NUM_FMTZ3	"999"
#define	NUM_FMTZ4	"9999"
#define	NUM_FMTZ5	"99999"


#define FMT12C_Z	"SSS,SSS,SSS,SS9"
#define FMT12Z		"SSSSSSSSSSS9"
#define FMT15		"sss,sss,sss,ss9"

// SetMasterDB vers
#define	SMDB_VERS		3


//簡易登録保存用
typedef struct _TKYV2_SAVE
{
	//チェックボックス状態
	int		chkTky;
	int		chk[3];

	//摘要情報
	char	tky[60],
			kana[24];

	//枝番情報
	int		debBrn,
			creBrn;

	char	dZan[6],
			cZan[6];

	char	sv_tky[60],
			sv_kana[24];
	//選択摘要
	char	sv_seltky[60];
	//インボイス登録番号
	char	sv_invno[16];

	BYTE	sv_menchk;
	char	sv_rsv[3];

} TKYV2_SAVE;


extern const char* INI_AppName;
extern const char* INI_ItmName;

#define SZ_DSGN			16
#define SZ_SYZDSGN		7

#define	SZ_INVNO		16

// マスターフラグ
#define	MST_INSERT		0x00000001
#define MST_TKY20		0x00000002
#define MST_SYZ5_8		0x00000004
#define MST_SYZ8		0x00000008
#define MST_SYZ8_10		0x00000010
#define MST_SYZ10		0x00000020
#define MST_SYZ_INV		0x00000040

#define SYZ8_YMD	20140401
#define KANI6_YMD	20150401
#define RVCHAR_YMD	20151001
#define SYZ10_YMD	20191001
#define SYZ_INV_YMD	20231001

#define SONEKICODE		0x08000000

//免税事業者
#define SYZMEN1_YMD	20231001
#define SYZMEN2_YMD	20260930
#define SYZMEN3_YMD	20261001
#define SYZMEN4_YMD	20290930


//摘要削除時の科目情報
typedef struct _KMK_REC {
	int		ezbmn;
	CString kncod;
	CString knnam;
	long	knicod;
	int		ezecd;
	int		type;
}KMK_REC;


#define		SIZE_DSIGN	6

#define		INVKANA_MAX	16
