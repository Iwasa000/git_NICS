// ShinCalculate8.cpp : DLL の初期化ルーチンです。
//

#include "stdafx.h"
#include "DBZmSub8.h"
//#include "DBZmDef8.h"
#include "DBNpSub8.h"
#include "ICSGEN80.h"
#include "ICSGENL8.h"
#include "ICSARITH8.h"
#include "ICSTblsub8.h"
#include "VolDatedb8.h"
#include "Volume8.h"
// 消費税情報の変換後の定義ファイル
#include "DBSyzDef.h"
#include "DBSyzShinKoku.h"
//　消費税申告書　集計用変数
#include "DBShinDef.h"
#include "DBShinDef1.h"
#include "DBShinDef2.h"

#include "ShinSumilatePac8.h"
#include "ShinCalculateW.h"

#ifdef _MANAGED
#error /clr と共にコンパイルするには ShinCalculate8.cpp 内の操作手順を参照してください。
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


static AFX_EXTENSION_MODULE ShinCalculate8DLL = { NULL, NULL };

#ifdef _MANAGED
#pragma managed(push, off)
#endif

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// lpReserved を使う場合はここを削除してください
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("ShinCalculate8.DLL Initializing!\n");
		
		// 拡張 DLL を 1 回だけ初期化します。
		if (!AfxInitExtensionModule(ShinCalculate8DLL, hInstance))
			return 0;

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

		new CDynLinkLibrary(ShinCalculate8DLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("ShinCalculate8.DLL Terminating!\n");

		// デストラクタが呼び出される前にライブラリを終了します
		AfxTermExtensionModule(ShinCalculate8DLL);
	}
	return 1;   // OK
}

// 改正チェック関数	財務クラス
__declspec(dllexport) int DBShinReviseCheck( int type, long start, long end, CDBZmSub* zmsub, CString& message )
{
	CShinCalculate revise;
	int ret = revise.DBShinCheck( type, start, end, zmsub );
	if( ret && message ) {
		if( ret == -1 )	message = revise.ShinCheckErrMsg();
		else			message = revise.ShinCheckGetMsg();
	}
	return ret;
}
// 改正チェック関数 非営利クラス
__declspec(dllexport) int NPShinReviseCheck( int type, long start, long end, CDBNpSub* zmsub, CString& message )
{
	CShinCalculate revise;
	int ret = revise.NPShinCheck( type, start, end, zmsub );
	if( ret && message ) {
		if( ret == -1 )	message = revise.ShinCheckErrMsg();
		else			message = revise.ShinCheckGetMsg();
	}
	return ret;
}
// シミュレーション集計　財務クラス
__declspec(dllexport) int DBShinReviseCalcurate( long start, long end, SUMINFO* suminfo, SUMLIST* sumlist, CDBZmSub* zmsub, CString& message )
{
	CShinCalculate revise;
	int ret = revise.DBShinCalculate( start, end, suminfo, sumlist, zmsub );
	if( ret && message ) {
		message = revise.ShinCalculateErrMsg();
	}
	return ret;	
}
// シミュレーション集計　非営利クラス													 
__declspec(dllexport) int NPShinReviseCalcurate( long start, long end, SUMINFO* suminfo, SUMLIST* sumlist, CDBNpSub* zmsub, CString& message )
{
	CShinCalculate revise;
	int ret = revise.NPShinCalculate( start, end, suminfo, sumlist, zmsub );
	if( ret && message ) {
		message = revise.ShinCalculateErrMsg();
	}
	return ret;	
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

// CShinCalqlat8
CShinCalculate::CShinCalculate()
{
	m_pShincalculate = new CShinCalculateW();
}

CShinCalculate::~CShinCalculate()
{
	delete m_pShincalculate;
}

////////////////////////
// 課税売上高チェック //
////////////////////////
// 財務クラス
// type		:	0:各業務　試算表・消費税額試算表・消費税申告書・課税売上割合
//			:	1:翌期更新・前年度からの更新
// start:	開始年月日 西暦(long) 
//				[例]2011年 1月1日→20110101 
// end  :	最終年月日 西暦(long)
//				[例]2011年12月1日→20111231
// zmsub:	財務クラスのポインター
// 返送値
// -1: エラー
//	0: 平成２４年３月３１日以前の対象外
//	1: ５億円超え
//  2: ４億五千万以上５億円未満
//  3: ４億五千万未満
//  4: みなし仕入率の変更メッセージ
int CShinCalculate::DBShinCheck( int type, long start, long end, CDBZmSub* zmsub )
{
	return( m_pShincalculate->CheckDBShin( type, start, end, zmsub ) );
	m_pShincalculate->pShincalculate = this;
}

// 公益クラス
// type		:	0:各業務　試算表・消費税額試算表・消費税申告書・課税売上割合
//			:	1:翌期更新・前年度からの更新
// start:	開始年月日 西暦(long) 
//				[例]2011年 1月1日→20110101 
// end  :	最終年月日 西暦(long)
//				[例]2011年12月1日→20111231
// zmsub:	財務クラスのポインター
// 返送値
// -1: エラー
//	0: 平成２４年３月３１日以前の対象外
//	1: ５億円超え
//  2: ４億五千万以上５億円未満
//  3: ４億五千万未満
//  4: みなし仕入率の変更メッセージ
int CShinCalculate::NPShinCheck( int type, long start, long end, CDBNpSub* zmsub )
{
	return( m_pShincalculate->CheckNPShin( type, start, end, zmsub ) );
	m_pShincalculate->pShincalculate = this;
}

// エラーメッセージの取得
CString& CShinCalculate::ShinCheckErrMsg( void )
{
	return m_pShincalculate->ErrMsgCheckShin();
}

// チェックメッセージの取得
// DBShinCheck()及びNPShinCheck()の返送値が1,2の場合に有効
CString& CShinCalculate::ShinCheckGetMsg( void )
{
	return m_pShincalculate->GetMsgCheckShin();
}

////////////////////////
// 消費税申告書　集計 //
////////////////////////

// 財務クラス
// start	:	開始年月日 西暦(long) 
//					[例]2011年 1月1日→20110101 
// end		:	最終年月日 西暦(long)
//					[例]2011年12月1日→20111231
// suminfo	:	処理情報パケット
// sumlist	:	実額及び集計後の金額パケット
// zmsub	:	財務クラスのポインター
// 返送値
// -1: エラー
//	0: 申告書集計完了
int	CShinCalculate::DBShinCalculate( long start, long end, SUMINFO* suminfo, SUMLIST* sumlist, CDBZmSub* zmsub )
{
	return( m_pShincalculate->CalculateDBShin( start, end, suminfo, sumlist, zmsub ) );
	m_pShincalculate->pShincalculate = this;
}

// 公益クラス
// start	:	開始年月日 西暦(long) 
//					[例]2011年 1月1日→20110101 
// end		:	最終年月日 西暦(long)
//					[例]2011年12月1日→20111231
// suminfo	:	処理情報パケット
// sumlist	:	実額及び集計後の金額パケット
// zmsub	:	公益クラスのポインター
// 返送値
// -1: エラー
//	0: 申告書集計完了
int	CShinCalculate::NPShinCalculate( long start, long end, SUMINFO* suminfo, SUMLIST* sumlist, CDBNpSub* zmsub )
{
	return( m_pShincalculate->CalculateNPShin( start, end, suminfo, sumlist, zmsub ) );
	m_pShincalculate->pShincalculate = this;
}

// エラーメッセージの取得
CString& CShinCalculate::ShinCalculateErrMsg( void )
{
	return m_pShincalculate->ErrMsgCalculateShin();
}
