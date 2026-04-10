// SyzMakeSub8.cpp : DLL の初期化ルーチンです。
//

#include "stdafx.h"
#include <afxdllx.h>
#ifdef _MANAGED
#error /clr と共にコンパイルするには SyzMakeSub8.cpp 内の操作手順を参照してください。
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

HINSTANCE	syzMakeInstance = 0;	// 該当DLL のインスタンスハンドル

static AFX_EXTENSION_MODULE SyzMakeSub8DLL = { NULL, NULL };

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
		TRACE0("SyzMakeSub8.DLL Initializing!\n");
		
		// 拡張 DLL を 1 回だけ初期化します。
		if (!AfxInitExtensionModule(SyzMakeSub8DLL, hInstance))
			return 0;

		syzMakeInstance = hInstance;

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

		new CDynLinkLibrary(SyzMakeSub8DLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("SyzMakeSub8.DLL Terminating!\n");

		// デストラクタが呼び出される前にライブラリを終了します
		AfxTermExtensionModule(SyzMakeSub8DLL);
	}
	return 1;   // OK
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

//=============================================================================
// 消費税非連動：マスター管理モジュール （外部クラス）
//=============================================================================

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CSyzMakeSub::CSyzMakeSub()
	: m_pSyzMakeSubW(NULL)
	, pdummy1(NULL)
{
	m_pSyzMakeSubW = new CSyzMakeSubW();
	ASSERT( m_pSyzMakeSubW );

	memset( rsv1, '\0', sizeof(rsv1) );
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CSyzMakeSub::~CSyzMakeSub()
{
	if( pdummy1 ){
		delete [] pdummy1;
		pdummy1 = NULL;
	}

	if( m_pSyzMakeSubW ){
		delete m_pSyzMakeSubW;
		m_pSyzMakeSubW = NULL;
	}
}

//-----------------------------------------------------------------------------
// エラーメッセージ取得
//-----------------------------------------------------------------------------
// 返送値	：	エラーメッセージ
//-----------------------------------------------------------------------------
CString& CSyzMakeSub::GetErrorMsg()
{
	m_ErrMsg.Empty();

	ASSERT( m_pSyzMakeSubW );
	if( m_pSyzMakeSubW ){
		m_ErrMsg = m_pSyzMakeSubW->GetErrorMsg();
	}

	return m_ErrMsg;
}

//-----------------------------------------------------------------------------
// データベースマスターの作成
//-----------------------------------------------------------------------------
// 引数	mkcoinfo	：	作成するマスター情報
//		pParent		：	親ウィンドウ
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//			-2		：	例外
//-----------------------------------------------------------------------------
int CSyzMakeSub::MakeDBMaster( SYZMAKE_MKCOINFO &mkcoinfo, CWnd *pParent )
{
	HINSTANCE svhInstResource = AfxGetResourceHandle();
	AfxSetResourceHandle( syzMakeInstance );

	int	rt = 0;
	ASSERT( m_pSyzMakeSubW );
	if( m_pSyzMakeSubW ){
		rt = m_pSyzMakeSubW->MakeDBMaster( mkcoinfo, pParent );
	}
	else{
		rt = -1;
	}

	AfxSetResourceHandle( svhInstResource );

	return rt;
}

//-----------------------------------------------------------------------------
// 消費税の状態チェック
//-----------------------------------------------------------------------------
// 引数	pZmsub	：	マスターに接続済の財務クラスのポインタ
//		pMsg	：	表示するメッセージの保存先
//		ChkInfo	：	状態チェック付加情報
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			1	：	メッセージを表示して処理を継続する	
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSub::CheckSyzState( CDBZmSub *pZmsub, CString &Msg, SYZ_CHKINFO &ChkInfo )
{
	int	rt = 0;

	ASSERT( m_pSyzMakeSubW );
	if( m_pSyzMakeSubW ){
		rt = m_pSyzMakeSubW->CheckSyzState( pZmsub, &Msg, ChkInfo );
	}
	else{
		rt = -1;
	}

	return rt;
}

//-----------------------------------------------------------------------------
// データベースマスターの削除
//-----------------------------------------------------------------------------
// 引数	mkcoinfo	：	削除するマスター情報
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//			-2		：	例外
//-----------------------------------------------------------------------------
int CSyzMakeSub::DeleteDBMaster( SYZMAKE_MKCOINFO &mkcoinfo )
{
	int	rt = 0;

	ASSERT( m_pSyzMakeSubW );
	if( m_pSyzMakeSubW ){
		rt = m_pSyzMakeSubW->DeleteDBMaster( mkcoinfo );
	}
	else{
		rt = -1;
	}

	return rt;
}

//-----------------------------------------------------------------------------
// 消費税不正仕訳のチェック
//-----------------------------------------------------------------------------
// 引数	pZmsub	：	マスターに接続済の財務クラスのポインタ
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSub::CheckWrongData( CDBZmSub *pZmsub )
{
	int	rt = 0;

	ASSERT( m_pSyzMakeSubW );
	if( m_pSyzMakeSubW ){
		rt = m_pSyzMakeSubW->CheckWrongData( pZmsub );
	}
	else{
		rt = -1;
	}

	return rt;
}

//-----------------------------------------------------------------------------
// 誘導メッセージ取得
//-----------------------------------------------------------------------------
// 返送値	：	誘導メッセージ
//-----------------------------------------------------------------------------
CString& CSyzMakeSub::GetGuideMsg()
{
	static	CString	GuideMsg;
	GuideMsg.Empty();

	ASSERT( m_pSyzMakeSubW );
	if( m_pSyzMakeSubW ){
		GuideMsg = m_pSyzMakeSubW->GetGuideMsg();
	}

	return GuideMsg;
}

//-----------------------------------------------------------------------------
// 平成26年改正 消費税コンバージョン
//-----------------------------------------------------------------------------
// 引数	pZmsub		：	マスターに接続済の財務クラスのポインタ
//		CnvInfo		：	コンバージョンオプション
//		rtMstst		：	マスターの状態（返送用）
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSub::ConversionToZei8( CDBZmSub *pZmsub, SYZ_CNVINFO &CnvInfo, long &rtMstst )
{
	int	rt = 0;

	ASSERT( m_pSyzMakeSubW );
	if( m_pSyzMakeSubW ){
		rt = m_pSyzMakeSubW->ConversionToZei8( pZmsub, CnvInfo, rtMstst );
	}
	else{
		rt = -1;
	}

	return rt;

}

//-----------------------------------------------------------------------------
// ５％を明記すべきマスター？
//-----------------------------------------------------------------------------
// 引数		pZmsub	：	マスターに接続済の財務クラスのポインタ
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	明記すべきマスター
//			FALSE	：	明記の必要が無いマスター
//-----------------------------------------------------------------------------
BOOL CSyzMakeSub::IsNeed5PerNote( CDBZmSub *pZmsub )
{
	BOOL	bRt = FALSE;

	ASSERT( m_pSyzMakeSubW );
	if( m_pSyzMakeSubW ){
		bRt = m_pSyzMakeSubW->IsNeed5PerNote( pZmsub );
	}

	return bRt;
}

//-----------------------------------------------------------------------------
// 消費税集計テーブル内の不正仕訳チェック
//-----------------------------------------------------------------------------
// 引数	pDatabase	：	チェック対象のマスターDB
//		isKani		：	簡易課税？
//-----------------------------------------------------------------------------
// 返送値	-1以外	：	不正仕訳の個数
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSub::ChkSzitmInSzsyk( CDatabase *pDatabase, BOOL isKani )
{
	int	rt = 0;
	ASSERT( m_pSyzMakeSubW );
	if( m_pSyzMakeSubW ){
		rt = m_pSyzMakeSubW->ChkSzitmInSzsyk( pDatabase, isKani );
	}

	return rt;
}

//-----------------------------------------------------------------------------
// 消費税集計テーブル内の不正仕訳チェック
//-----------------------------------------------------------------------------
// 引数	pDatabase	：	チェック対象のマスターDB
//		StYymmdd	：	指定期間の始まり
//		EdYymmdd	：	指定期間の終了
//-----------------------------------------------------------------------------
// 返送値		0		：	正常（不正仕訳無し）
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSub::CheckWrongData( CDBZmSub *pZmsub, long StYymmdd, long EdYymmdd )
{
	int	rt = 0;
	ASSERT( m_pSyzMakeSubW );
	if( m_pSyzMakeSubW ){
		rt = m_pSyzMakeSubW->CheckWrongData( pZmsub, StYymmdd, EdYymmdd );
	}

	return rt;
}

//-----------------------------------------------------------------------------
// 簡易税不正仕訳のチェック('15.02.23)
//-----------------------------------------------------------------------------
// 引数	pZmsub		：	チェック対象の財務クラス
//		StYymmdd	：	開始年月日
//		EdYymmdd	：	終了年月日
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSub::CheckWrongKaniData( CDBZmSub *pZmsub, long StYymmdd, long EdYymmdd )
{
	int rt = 0;

	ASSERT( m_pSyzMakeSubW );
	if( m_pSyzMakeSubW ){
		rt = m_pSyzMakeSubW->CheckWrongKaniData( pZmsub, StYymmdd, EdYymmdd );
	}

	return rt;
}

//-----------------------------------------------------------------------------
// 特定収入不正仕訳のチェック('15.03.11)
//-----------------------------------------------------------------------------
// 引数	pZmsub		：	チェック対象の財務クラス
//		StYymmdd	：	開始年月日
//		EdYymmdd	：	終了年月日
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSub::CheckWrongSpcData( CDBZmSub *pZmsub, long StYymmdd, long EdYymmdd )
{
	int rt = 0;

	ASSERT( m_pSyzMakeSubW );
	if( m_pSyzMakeSubW ){
		rt = m_pSyzMakeSubW->CheckWrongSpcData( pZmsub, StYymmdd, EdYymmdd );
	}

	return rt;
}

//-----------------------------------------------------------------------------
// 前期以前の不正月別情報の修正('16.12.19)
//-----------------------------------------------------------------------------
// 引数	pZmsub		：	修正対象の財務クラス
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSub::RepairZmonthInfo( CDBZmSub *pZmsub )
{
	int rt = 0;

	ASSERT( m_pSyzMakeSubW );
	if( m_pSyzMakeSubW ){
		rt = m_pSyzMakeSubW->RepairZmonthInfo( pZmsub );
	}

	return rt;
}

//-----------------------------------------------------------------------------
// zvolumeの再作成('17.05.01)
//-----------------------------------------------------------------------------
// 引数	mkcoinfo	：	作成情報
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSub::ReMakeZvolume( SYZMAKE_MKCOINFO &mkcoinfo )
{
	int rt = 0;

	ASSERT( m_pSyzMakeSubW );
	if( m_pSyzMakeSubW ){
		rt = m_pSyzMakeSubW->ReMakeZvolume( mkcoinfo );
	}

	return rt;
}

//-----------------------------------------------------------------------------
// マスターを作成する時の必須項目をチェック('18.08.21)
//-----------------------------------------------------------------------------
// 引数	mkcoinfo	：	作成情報
//-----------------------------------------------------------------------------
// 返送値	0		：	作成可能
//			-1		：	作成不可
//-----------------------------------------------------------------------------
int CSyzMakeSub::CheckEssentialItem( SYZMAKE_MKCOINFO &mkcoinfo )
{
	int rt = 0;

	ASSERT( m_pSyzMakeSubW );
	if( m_pSyzMakeSubW ){
		rt = m_pSyzMakeSubW->CheckEssentialItem( mkcoinfo );
	}

	return rt;
}

//-----------------------------------------------------------------------------
// 平成31年改正 消費税コンバージョン
//-----------------------------------------------------------------------------
// 引数	pZmsub		：	マスターに接続済の財務クラスのポインタ
//		CnvInfo		：	コンバージョンオプション
//		rtMstst		：	マスターの状態（返送用）
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSub::ConversionToZei10(CDBZmSub *pZmsub, SYZ_CNVINFO &CnvInfo, long &rtMstst)
{
	int	rt = 0;

	ASSERT(m_pSyzMakeSubW);
	if( m_pSyzMakeSubW ){
		rt = m_pSyzMakeSubW->ConversionToZei10(pZmsub, CnvInfo, rtMstst);
	}
	else{
		rt = -1;
	}

	return rt;

}

//-----------------------------------------------------------------------------
// 平成31年改正 自動コンバージョンを行える？
//-----------------------------------------------------------------------------
// 返送値	true	：	行える
//			false	：	行えない
//-----------------------------------------------------------------------------
bool CSyzMakeSub::IsAbleToAutoVup()
{
	ASSERT(m_pSyzMakeSubW);
	if( m_pSyzMakeSubW ){
		return m_pSyzMakeSubW->IsAbleToAutoVup();
	}
	else{
		return false;
	}
}

//-----------------------------------------------------------------------------
// 3%か5%の輸入仕入仕訳のチェック
//-----------------------------------------------------------------------------
// 引数	pZmsub		：	チェック対象の財務クラス
//		StYymmdd	：	開始年月日
//		EdYymmdd	：	終了年月日
//-----------------------------------------------------------------------------
// 返送値	0		：	正常（不正仕訳無し）
//			1		：	正常（不正仕訳あり）
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSub::Check3or5ImportData(CDBZmSub *pZmsub, long StYymmdd, long EdYymmdd)
{
	ASSERT(m_pSyzMakeSubW);
	if( m_pSyzMakeSubW ){
		return m_pSyzMakeSubW->ChkInput3or5ImportData(pZmsub, StYymmdd, EdYymmdd);
	}
	else{
		return false;
	}
}

//-----------------------------------------------------------------------------
// 別記仕訳のチェック
//-----------------------------------------------------------------------------
// 返送値	0		：	正常（不正仕訳無し）
//			1		：	正常（不正仕訳あり）
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSub::CheckSepParaData(CDBZmSub *pZmsub, long StYymmdd, long EdYymmdd)
{
	ASSERT(m_pSyzMakeSubW);
	if( m_pSyzMakeSubW ){
		return m_pSyzMakeSubW->CheckSepParaData(pZmsub, StYymmdd, EdYymmdd);
	}
	else{
		return false;
	}
}

//-----------------------------------------------------------------------------
// 消費税申告書に関連するデータの削除 ('20.02.20)
//-----------------------------------------------------------------------------
// 引数	mkcoinfo	：	削除するマスター情報
// 引数	optionflg	：	削除オプション（ビット管理）
//					：　□□□□ □□□□ □□□□ □□□□ □□□□ □□□□ □□□□ □□□■
//					：　// D0　ON:MCTL＆zvolume情報更新(sub_swのOFF処理)
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSub::DeleteSyzData(SYZMAKE_MKCOINFO &mkcoinfo, int optionflg)
{

	int rt = 0;

	ASSERT(m_pSyzMakeSubW);
	if (m_pSyzMakeSubW) {

		rt = m_pSyzMakeSubW->DeleteSyzData(mkcoinfo, optionflg);
	}

	return rt;
}

//-----------------------------------------------------------------------------
// 当期の不正月別情報の修正('20.08.28)
//-----------------------------------------------------------------------------
// 引数	pZmsub		：	修正対象の財務クラス
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSub::RepairMonthInfo(CDBZmSub *pZmsub)
{
	int rt = 0;

	ASSERT(m_pSyzMakeSubW);
	if( m_pSyzMakeSubW ){
		rt = m_pSyzMakeSubW->RepairMonthInfo(pZmsub);
	}

	return rt;
}


//-----------------------------------------------------------------------------
// //消費税申告書のデータクリア処理（会社登録課税区分切り替え時に呼び出し） '22.11.29
//-----------------------------------------------------------------------------
// 引数	pZmsub		：	財務クラス
// 引数	optionflg	：	データクリアオプション（現在未使用。今後の為用に作成）
//					：　□□□□ □□□□ □□□□ □□□□ □□□□ □□□□ □□□□ □□□□
//					：　// D0　
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSub::ClearSyzShinData(CDBZmSub *pZmsub, int optionflg)
{

	int rt = 0;

	ASSERT(m_pSyzMakeSubW);
	if (m_pSyzMakeSubW) {

		rt = m_pSyzMakeSubW->ClearSyzShinData(pZmsub, optionflg);
	}

	return rt;
}

//-----------------------------------------------------------------------------
// 不正な免税事業者からの課税仕入 有無チェック('23.03.29)
//-----------------------------------------------------------------------------
// 引数	pZmsub		：	チェック対象の財務クラス
//-----------------------------------------------------------------------------
// 返送値		0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSub::CheckWrongMzSrData(CDBZmSub *pZmsub)
{
	int rt = 0;

	ASSERT(m_pSyzMakeSubW);
	if( m_pSyzMakeSubW ){
		rt = m_pSyzMakeSubW->CheckWrongMzSrData(pZmsub);
	}

	return rt;
}

//-----------------------------------------------------------------------------
// 免税事業者からの課税仕入 有無チェック ('23.03.29)
//-----------------------------------------------------------------------------
// 引数	pZmsub		：	チェック対象の財務クラス
//		StYymmdd	：	開始年月日
//		EdYymmdd	：	終了年月日
//-----------------------------------------------------------------------------
// 返送値		0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSub::CheckMzSrData(CDBZmSub *pZmsub, long StYymmdd, long EdYymmdd)
{
	int rt = 0;

	ASSERT(m_pSyzMakeSubW);
	if( m_pSyzMakeSubW ){
		rt = m_pSyzMakeSubW->CheckMzSrData(pZmsub, StYymmdd, EdYymmdd);
	}

	return rt;
}

//-----------------------------------------------------------------------------
// 消費税の集計済サインを落とす('23.08.28)
//-----------------------------------------------------------------------------
// 引数	pZmsub		：	対象の財務クラス
//-----------------------------------------------------------------------------
// 返送値		0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSub::TrunOffSyzCksw(CDBZmSub *pZmsub)
{
	int rt = 0;

	ASSERT(m_pSyzMakeSubW);
	if( m_pSyzMakeSubW ){
		rt = m_pSyzMakeSubW->TrunOffSyzCksw(pZmsub);
	}

	return rt;
}

//-----------------------------------------------------------------------------
// 指定期間内の仕訳に打たれているインボイス登録番号の正当性をチェックする.
// Web-APIの判定時間に時間がかかるためチェック中待ちダイアログを表示する.
//-----------------------------------------------------------------------------
// 引数	pZmsub		     ：	マスターに接続済の財務クラスのポインタ
//		StYymmdd	     ：	開始年月日
//		EdYymmdd	     ：	終了年月日
//      WrongInvoiceDataList： 不正データ詳細
//      pParent           :  親ウィンドウ
//-----------------------------------------------------------------------------
// 返送値	0		：	正常（インボイス登録番号データ不正なし）
//			1		：	正常（インボイス登録番号データ不正あり）
//		   -1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSub::CheckWrongInvoiceData(CDBZmSub *pZmSub, long StYymmdd, long EdYymmdd, SYZ_WrongInvoiceDataList& WrongInvoiceDataList, CWnd* pParent)
{
    int rt = 0;
	ASSERT(m_pSyzMakeSubW);
	if (m_pSyzMakeSubW) {
		rt = m_pSyzMakeSubW->CheckWrongInvoiceData(pZmSub, StYymmdd, EdYymmdd, WrongInvoiceDataList, pParent);
	}

	//DbgViewTrace(_T("【DEBUG】%d %s() rt:%d\n"), __LINE__, __FUNCTION__, rt);
 //   if( rt < 0 ) {
	//	DbgViewTrace(_T("【DEBUG】%d %s() errormsg:%s\n"), __LINE__, __FUNCTION__, m_pSyzMakeSubW->GetErrorMsg());
	//}

	return rt;
}

