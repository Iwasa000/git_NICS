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
#define _WIN32_WINDOWS 0x0410 // これを Windows Me またはそれ以降のバージョン向けに適切な値に変更してください。
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


//#ifndef	_DEBUG
//#define	_20140314_SYUKA_	1
//#endif


//#ifndef _DEBUG
//#define	_20190830_SYUKA_	1
//#endif 

// 消費税10%改正
// 特定収入計算表未対応部分
//#define _20190930_SYUKA	1

//特例適用期間
#define SIIRETOKUREI_EDAY 20200930
#define URIAGETOKUREI_EDAY 20230930


// 新型コロナウイルスによる申告・納付期限延長申請
#define H31_SDAY 20190101


// 令和3年1月1日以降の課税期間
// 付表６旧様式出力期限
#define R03_SDAY 20210101

// 特定収入計算表　様式改正
#define R03YEAR 20211231	// 令和3年

// kasai add -->
// 課税取引（旧税率なし）開始日
// Mainでも同様の定義してるため、変更する場合はそっちも変更
#define TAX_R04_SDAY 20211231
// kasai add <--

// 2023/07/13 h.wada add --> 
// DBSyzTaxationList31でも同様の定義をしている
#define TAX_R05_EDAY 20231231
// h.wada <--

// 220412 add -->
// 令和4年4月1日以降の課税期間であれば、特例計算（5-3）のチェック欄を非表示
#define TOK53MODIFY 20220401
// 220412 add <--

// 令和4年12月31日以後の課税期間であれば、更正の請求の「この請求前の金額」列をカット
// 「この請求前の期確定税額」項目を追加
#define KSKMODIFY 20221231

// 令和5年1月1日以降の課税期間の場合には、課税取引金額計算表の経理方式の切り替えを不可
#define R05_SDAY 20230101

// 令和4年4月1日以降の課税期間の場合、振替継続希望欄と公金受取口座の利用項目が本表に追加
#define R04REFORM 20220401

// 2割特例適用期間　令和５年10月１日から令和８年９月30日までの日の属する各課税期間
#define INVOICE_2WARI_EDAY 20260930

// 24/03/18_税務代理書面 add -->
// 税務代理権限証書の新様式（令和6年4月1日）
//#define R06_ZEIMUDAIRI 20240401
#define R06_ZEIMUDAIRI 20231001	// とりあえず電子の仕様が確定するまでインボイスと同じにする
// 24/03/18_税務代理書面 add <--

#include <icspackdef.h>
#include <ICSGENL8.h>
#include <ICSGEN80.h>

#include <ICSWindows8.h>

#include <volume8.h>
#include <dbzmsub8.h>
#include <dbzmdef8.h>
#include <DBNpSub8.h>
#include <icsprn8.h>
#include <ZmGen8.h>
#include <SyzMakeSub8.h>
#include <DBAddUp8.h>
#include <voldatedb8.h>
#include <DBKmkSel8.h>

//#include <RegOffdef.h>//必要ないので削除した'14.08.05
#include <Office8.h>
#include <OfficeInfo8.h>
#include <COfces8.h>

//== 印刷関連 ==
#include <xysprn8.h>
#include <pforminf8.h>
#include <PrintSch8.h>
#include <Subtool8.h>

#include <CoinfCtl8.h>
#include <Kmnctrl8.h>
#include <DBprogress_Mod8.h>

#include <Eselect8.h>

#include <SyzShinComDef.h>
#include "GroupStatic.h"
#include "TransStatic.h"
#include "SyzShinMainDef.h"
#include "SyzShinRec.h"
#include "Util.h"
#include <DBSyzShinCom8.h>

//== 課税売上高５億円チェック ==
#include <ShinSumilatePac8.h>
#include <ShinRevise8.h>


//== 集計関連 ==
#include <SyzSyukei8.h>
#include <KariCheck.h>

//== 履歴関連 ==
#include <vector>
#include <map>
#include "TimeControl.h"
#include "Snset.h"
#include "HistoryReport.h"

//== 納付書関連 ==
#include <Payment8.h>

//== マイナンバー関連 ==
#include <ICSMNSub8.h>
#include <ICSMNEdit8.h>
#include <NoTblMod8.h>
#include <HJAccess.h>

#include <DBSyzAccess8.h>
#include "MiddlePymentRec.h"
#include <icsocx.h>
#include ".\SyzrefundDB\syzrefunddef.h"
#include ".\SyzrefundDB\DiagControlBase.h"

#include <set>
#include <algorithm>

#include <H26HyoView.h>
#include <H31HyoView.h>
//-- '15.02.23 --
//#include "..\..\DBSyz26\DBSyz26\H26HyoDoc.h"
//#include "..\..\DBSyzTaxationList\DBSyzTaxationList\TaxationListView.h"
//---------------
#include <H26HyoDoc.h>
#include <H31HyoDoc.h>
#include <TaxationListView.h>
#include <TaxationList31View.h>
#include <TaxListDoc.h>
#include <TaxList31Doc.h>
//---------------
//2015.12.15 INSERT START
#include <icszipad8.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxwin.h>
#include <afxwin.h>
#include <afxwin.h>
//2015.12.15 INSERT END

// 24/03/18_税務代理書面 add -->
#include <TaxAgent.h>
// 24/03/18_税務代理書面 add <--
// 24/03/26_税務代理書面 add -->
#include <jzcheck.h>
// 24/03/26_税務代理書面 add <--

//#define	_DBG_VIEW_START_	1	// DbgViewTrace を有効にする時には、このフラグを有効にしてください。


#define WM_ACTIVE_SETFORCUS		(WM_USER+101)
#define WM_MY_COLORCHANGE		(WM_USER+102)	// 修正No.160356,168678 add

#ifdef _DBG_VIEW_START_
void DbgViewTrace(LPCTSTR lpszFormat, ...);
#endif

// 部数関連
typedef struct LST_BUSU_	{
	int	idx[10];
}LST_BUSU;

typedef struct LST_BUSU_IDX_	{
	int	dsp_no;
// 24/02/26_特定収入対応 cor -->
	//int	prt_no[10];
// -----------------------------
	int	prt_no[20];	// 特定収入計算表 5-2 追加のため拡張
// 24/02/26_特定収入対応 cor <--
}LST_BUSU_IDX;

// 24/02/19wd_特定収入対応 add -->
// 特定収入計算表 個別出力設定//　240307課税取引金額計算表　+　税務代理帳票　でも使用
typedef struct PRNEXSUB_INF_	{
	int	check;
	int	busu;
	int	stpage;
	int	edpage;
}PRNEXSUB_INF;
// 24/02/19wd_特定収入対応 add <--


