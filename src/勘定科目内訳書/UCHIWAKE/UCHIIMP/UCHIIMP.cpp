// UCHIIMP.cpp : DLL の初期化ルーチンです。
//

#include "stdafx.h"
#include <afxdllx.h>

#include "DBUchiImp8.h"
#include "dlgImport.h"
#include "dlgImpOpt.h"
#include "impUc000Common.h"
#include "impUc011Yotyokin.h"
#include "impUc021Uketoritegata.h"
#include "impUc031Urikakekin.h"
#include "impUc041Karibarai.h"
#include "impUc042Kasituke.h"
#include "impUc051Tanaoroshi.h"
#include "impUc052Tanaoroshi2.h"
#include "impUc061Yuukasyouken.h"
#include "impUc071Koteisisan.h"
#include "impUc081Siharaitegata.h"
#include "impUc091Kaikakekin.h"
#include "impUc092Kaikakekin2.h"
#include "impUc101Kariukekin.h"
#include "impUc102Gensan.h"
#include "impUc111Kariirekin.h"
#include "impUc121Totiuriage.h"
#include "impUc131Uriagedaka.h"
#include "impUc141Yakuin.h"
#include "impUc142Jinkenhi.h"
#include "impUc151Tidaiyatin.h"
#include "impUc152Kenrikin.h"
#include "impUc153Kougyou.h"
#include "impUc161Zatueki.h"
#include "impUc162Zatuson.h"
// midori 170504 add -->
#include "impUc171Sonota1.h"
#include "impUc181Sonota2.h"
// midori 170504 add <--
#include "..\..\UCHIWAKE\UCHIWAKE\dbUcInfSub.h"
#include "..\..\UCHIWAKE\UCHIWAKE\dbUcInfFormType.h"
#include "..\..\UCHIWAKE\UCHIWAKE\dbUcLstBank.h"
#include "..\..\UCHIWAKE\UCHIWAKE\dbUcLstAddress.h"
#include "..\..\UCHIWAKE\UCHIWAKE\dbUcLstKamoku.h"
#include "..\..\UCHIWAKE\UCHIWAKE\UcFunctionCommon.h"
// midori 170504 del -->
//#include "..\UCHIWAKE\dbUc000Common.h"
// midori 170504 del <--
#include "..\..\UCHIWAKE\UCHIWAKE\dbUcInfMain.h"
// midori 161103 add -->
#include "..\..\UCHIWAKE\UCHIWAKE\dbIcsVtbl.h"		//	vtblテーブルクラス
// midori 161103 add <--
// midori 170504 add -->
#include "..\..\UCHIWAKE\UCHIWAKE\dbUcInfSonotaItem.h"
// midori 170504 add <--
// midori UC_0005 add -->
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc000Common.h"
// midori UC_0005 add <--
// midori 180503 add -->
#include "dlgYokukiMain.h"
#include "..\..\UCHIWAKE\UCHIWAKE\dbUcInfYokuki.h"
// midori 180503 add <--

// 24/02/09_ScoreLink add -->
#include <fstream>
// 24/02/09_ScoreLink add <--

#ifdef _MANAGED
#error /clr と共にコンパイルするには UCHIIMP.cpp 内の操作手順を参照してください。
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

// midori 170504 add -->
#define	AS_SPLIT_MAX	2048
// midori 170504 add <--

static AFX_EXTENSION_MODULE UCHIIMPDLL = { NULL, NULL };

HINSTANCE		g_hInstance;

// midori 161103 add -->
// -------------------------------------------------------------------------------------------------
// 変数定義
// -------------------------------------------------------------------------------------------------
char		sG_Svr[32]={0};			// SQLサーバー名
char		sG_Apno[32]={0};		// アプリケーション番号;
char		sG_Ccod[32]={0};		// 会社コード
char		sG_Date[32]={0};		// 期末年
CString		sG_Err=_T("");			// エラーメッセージ
ICSMCTL*	pG_Mctl=NULL;			// マスター管理クラス
CDBZmSub*	pG_Tzms=NULL;			// [当年度]財務クラス
CDBZmSub*	pG_Zzms=NULL;			// [過年度]財務クラス
// midori 190301 add -->
BOOL		bG_Kanso;
// midori 190301 add <--
BOOL		bG_InvNo;				// インボイス番号使用サイン（決算期末が令和6年3月1日を超えている） インボイス登録番号追加対応_23/11/06 add
// 24/02/19_ScoreLink add -->
BOOL		bG_Sl;					// ScoreLink連携からの起動
// 24/02/19_ScoreLink add <--
// 24/02/09_ScoreLink add -->
CString		SL_Err;
// 24/02/09_ScoreLink add <--

bool		m_Invoice;				// 改良No.21-0086,21-0529 add


// -------------------------------------------------------------------------------------------------
// 関数定義
// -------------------------------------------------------------------------------------------------
BOOL	ZenUpdateIniz( void );
// midori 153384 del -->
//BOOL	ZenUpdateMain( int pKakutei );
// midori 153384 del <--
// midori 153384 add -->
BOOL	ZenUpdateMain( int pKakutei, int pKakutei2 );
// midori 153384 add <--
// midori 152377 add -->
BOOL	ZenUpdateMain2( int pKakutei );
// midori 152377 add <--
void	ZenUpdateFinz( void );
int		ExecuteSQLWork(CDatabase* pdb,CString inStrSql);
// midori 161103 add <--
// midori UC_0005 add -->
BOOL GetShowKeiZero(CString strTemp);
void CmnKingakuAddFunc(CString *inTotalVal, CString inAddVal, int inAddFlag = 1);
CdbUc000Common* GetDataTableObject(CDBZmSub* pZmSub,int pFormSeq);
int CmnGetNextPageRow(int* pPage, int* pRow,int intRowMaxData);
void GetFormRow(int pFormSeq,int pintFormType,int intOutKei,int* pintRowMax,int* pintRowMaxData,int* pintRowKei);
int CmnTblInsertSpecialLine(CDBZmSub* pZmSub,int pFormSeq,int nPage, int nRow, int nType, CString strName);
int CmnTblInsertNullLine(CDBZmSub* pZmSub,int pFormSeq,int pintFormType,int pintOutKei,int nPage, int nRow, int nLine);
int CmnTblCountMaxRow(int pFormSeq,int pintFormType,int pintOutKei);
int CmnTblDeleteFgFunc(CDBZmSub* pZmSub,int inFgFuncKind,int pFormSeq);
int CmnTblUpdatePageNumRowNumReset(CDBZmSub* pZmSub,int inPage, int inRow,int pFormSeq,int pintFormType,int pintOutKei);
BOOL CmnUcInfFormTypeGetFormType(CDBZmSub* pZmSub,int intFormSeq,int* intFormType);
int CmnTblUpdateEndPageSetting(CDBZmSub* pZmSub,int pFormSeq);
int CmnTblInsertPageKeiRuiKei(CDBZmSub* pZmSub,int pFormSeq,int pintFormType,int pintOutKei,int NameKei);
int CmnTblCalcKei(CDBZmSub* pZmSub,int pFormSeq,int intOutKei,BOOL bPrintZeroData,BOOL bPrintNullData);
void CmnTblUpdatePageRowAllResetMain(CDBZmSub* pZmSub,CdbUcInfSub* pUcInfSub);
// midori UC_0005 add <--
// 157153 add -->
int CmnTblUpdatePageNumRowNumReset_Sub(int pFormSeq,CdbUc000Common* pData,CDBZmSub* pZmSub,int pSort1,int pOpOutNullLine,int pOpOutSyokei,int pintFormType,int pintOutKei);
int IkkatuInsertNull(CdbUc000Common* pData,CDBZmSub* pZmSub,CCtrlSubGeneralVar pcs,int pFormSeq,int pOutKei,int pSort1,int pOpOutNullLine,int pOpOutSyokei);
void CmnTblDeleteFirstNullLine(int pFormSeq,CdbUc000Common* pData,CDBZmSub* pZmSub,int pintFormType,int pintOutKei);
int IkkatuDeleteNull(CdbUc000Common* pData,CDBZmSub* pZmSub,CCtrlSubGeneralVar pcs,int pFormSeq,int pOutKei,int pOpOutNullLine,int pOpOutSyokei);
// 157153 add <--
// midori 180503 add -->
int CheckGeneralVar( CDBZmSub* pZmSub );
int ChkKakuteiUpd( CDBZmSub* pZmSub );
int UchiYokukiGetZvolumeYmd(CDBZmSub* pZmSub,long* lngSSymd, long* lngEEymd);
int CmnTblUpdateFgShowDataReturn(CDBZmSub* pZmSub, CdbUcInfSub* pUcInfSub, int nPage, int nRow, int nGroup);
int CmnTblIkkatuAutoUpdateProc(CDBZmSub* pZmSub, CdbUcInfSub* pUcInfSub,int nGroup, int* nPage, int* nRow, SORTKAMOKU_INFO uKamoku);
BOOL IsKanendoMaster( CDBZmSub* pZmSub );
int KakuteiSet( CDBZmSub* pZmSub );
int CopyHokanDataToHokanTable(CDBZmSub* pZmSub, int nFormSeq);
int KanendoKakuteiSet( CDBZmSub* pZmSub );
int UchiYokukiUpdateSubProc(CDBZmSub* pZmSub,int intFormSeq);
// midori 180503 add <--
// midori 156188 add -->
//int		ImpKamokuRowEnableSgn(CDatabase* pDB, int pSw);				// 改良No.21-0086,21-0529 del
int		ImpKamokuRowEnableSgn(CDatabase* pDB, int pSw, int nFormSeq);	// 改良No.21-0086,21-0529 add
// midori 156188 add <--

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
		TRACE0("UCHIIMP.DLL Initializing!\n");
		
		// 拡張 DLL を 1 回だけ初期化します。
		if (!AfxInitExtensionModule(UCHIIMPDLL, hInstance))
			return 0;

		g_hInstance = hInstance;	//	☆

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

		new CDynLinkLibrary(UCHIIMPDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("UCHIIMP.DLL Terminating!\n");

		// デストラクタが呼び出される前にライブラリを終了します
		AfxTermExtensionModule(UCHIIMPDLL);
	}
	return 1;   // OK
}


//--------------------------------------------------
//	コンストラクタ
//--------------------------------------------------
CDBUchiImp::CDBUchiImp(void)
{
}

//--------------------------------------------------
//	デストラクタ
//--------------------------------------------------
CDBUchiImp::~CDBUchiImp(void)
{
}


//**********************************************************************
// データインポート
//		・本関数の呼び出し方法により、動作が異なる。
//		　　(1) フォルダの指定、および実行するかどうか確認するダイアログが表示される。
//		　　(2) 引数で指定されたフォルダに対し、インポート処理を開始する。
//		・既存のデータは削除される（ただし、インポート対象のCSVファイルがない帳票に対しては何もしない）。
//		・インポート後、ページの再構成はされない（呼び出し元で実施すること）。
// 
//	引数
//			pZmSub					[IN] 財務データベースへのポインタ
//			strFolderPath			[IN] インポートデータがあるフォルダのパス
//			pParent					[IN] 親ウィンドウのポインタ
// 
//	戻値
//			エラーコード（DBUchiImpDEF.h参照）
//**********************************************************************
// (1) フォルダの指定、および実行するかどうか確認するダイアログが表示される。
int CDBUchiImp::Import(CDBZmSub* pZmSub, CWnd* pParent /*= NULL*/)
{
	HINSTANCE		m_hDefInstance;					//<-----20111007呼び出し側のリソースハンドル
	m_hDefInstance = AfxGetResourceHandle();		
	AfxSetResourceHandle( g_hInstance );			//DLL側のリソースハンドルセット

	CString		strFolderPath		= _T("");		// フォルダパス
	CdlgImport	dlg(pParent);						// データインポートダイアログ

	//--------------------------------------------------
	// INIファイルからの指定フォルダ取得
	//--------------------------------------------------
	strFolderPath = GetFolderPath(TRUE);

	// データインポートダイアログの表示
	// （データインポート元フォルダのパスを取得）
	if (dlg.ShowDialog(TRUE, &strFolderPath) != ID_DLG_OK) {
		//リソースハンドルをexeに戻す
		AfxSetResourceHandle( m_hDefInstance );

		// [キャンセル]ボタン押下
		return IMP_DIALOG_ERRCODE_CANCEL;
	}

	// 保存先のドライブ指定チェック									// [2009/09/04 Add]
	if ( strFolderPath.Find("\\\\") != 0 ) {							// ネットワークパス？
		if ( strFolderPath.Find(':') != 1 ) {							// ドライブ指定無し？
			strFolderPath = GetIcswinDrive() + "\\" + strFolderPath;	// ドライブ指定が無ければICSWinのあるドライブを指定
		}
	}
	// インポート処理
	int nRetValue = Import(pZmSub, strFolderPath, pParent);

	//リソースハンドルをexeに戻す
	AfxSetResourceHandle( m_hDefInstance );

	switch (nRetValue) {
// midori 160603 del -->
//	case	IMP_ERRCODE_FINDFOLDER:			// フォルダがない
// midori 160603 del <--
	case	IMP_ERRCODE_CREATEDFOLDER:		// ファイル作成失敗
	case	IMP_DIALOG_ERRCODE_CANCEL:
		// 実際の処理をしていないので CANCEL で返す
		return IMP_DIALOG_ERRCODE_CANCEL;
	}
	return IMP_DIALOG_ERRCODE_OK;
}

//**********************************************************************
// データインポート・電子申告用
//		・電子申告用にフォルダパスを新たに引数としてとる
// 
//	引数
//			pZmSub					[IN] 財務データベースへのポインタ
//			strFolderPath			[IN] インポートデータがあるフォルダのパス
//			pParent					[IN] 親ウィンドウのポインタ
// 
//	戻値
//			エラーコード（DBUchiImpDEF.h参照）
//**********************************************************************
int CDBUchiImp::ImportForDenshi(CDBZmSub* pZmSub, CString strFolderPath, CWnd* pParent)
{
	HINSTANCE		m_hDefInstance;					//<-----20111007呼び出し側のリソースハンドル
	m_hDefInstance = AfxGetResourceHandle();		
	AfxSetResourceHandle( g_hInstance );			//DLL側のリソースハンドルセット

	CdlgImport	dlg(pParent);						// データインポートダイアログ

	// データインポートダイアログの表示
	// （データインポート元フォルダのパスを取得）
	if (dlg.ShowDialog(TRUE, &strFolderPath) != ID_DLG_OK) {
		//リソースハンドルをexeに戻す
		AfxSetResourceHandle( m_hDefInstance );

		// [キャンセル]ボタン押下
		return IMP_DIALOG_ERRCODE_CANCEL;
	}

	// 保存先のドライブ指定チェック									// [2009/09/04 Add]
	if ( strFolderPath.Find("\\\\") != 0 ) {							// ネットワークパス？
		if ( strFolderPath.Find(':') != 1 ) {							// ドライブ指定無し？
			strFolderPath = GetIcswinDrive() + "\\" + strFolderPath;	// ドライブ指定が無ければICSWinのあるドライブを指定
		}
	}
	// インポート処理
	int nRetValue = Import(pZmSub, strFolderPath );

	//リソースハンドルをexeに戻す
	AfxSetResourceHandle( m_hDefInstance );

	switch (nRetValue) {
// midori 160603 del -->
//	case	IMP_ERRCODE_FINDFOLDER:			// フォルダがない
// midori 160603 del <--
	case	IMP_ERRCODE_CREATEDFOLDER:		// ファイル作成失敗
	case	IMP_DIALOG_ERRCODE_CANCEL:
		// 実際の処理をしていないので CANCEL で返す
		return IMP_DIALOG_ERRCODE_CANCEL;
	}
	return IMP_DIALOG_ERRCODE_OK;
}

// (2) 引数で指定されたフォルダに対し、インポート処理を開始する。
int CDBUchiImp::Import(CDBZmSub* pZmSub, CString strFolderPath, CWnd* pParent /*= NULL*/)
{
	int		nErrorCode			= IMP_ERRCODE_SUCCESS;	// エラーコード
// midori 153829 add -->
	int		len=0;
	int		ii=0;
	int		jj=0;
// midori 153829 add <--

	CString csCompanyName;								// 会社名用バッファ
	CString csSheetName;								// 帳票名用バッファ

	CCsv			csvUcFile;							// CSVファイルクラス
	CString			csCsvFilename;						// CSVファイル名(フルパス用)
	CStringArray	csaCsvLine;							// CSVファイル1行分のバッファ
	CStringArray	arrFileName;						//指定されたフォルダに存在するファイル名
	CStringArray	arrFileNo;							//上記ファイル名に対応する様式番号
	CdlgImpOpt		dlgOpt(pParent);					//データインポートオプションダイアログ<-----20111007
	CArray<ImpOpt, ImpOpt&> arrImpStct;					//<-----20111007
							arrImpStct.RemoveAll();
// midori 170504 add -->
	CString					cst=_T("");
	CdbUcInfSonotaItem		mfcRec( pZmSub->m_database );			//	uc_inf_sonota_itemテーブルクラス
// midori 170504 add <--

	int FormIndex = 1;									//  uc_inf_sub テーブル用の帳票番号（1～）

// midori 156935 add -->
	int				nFormSeq=0;
// 改良No.21-0086,21-0529 cor -->
	//BOOL			csvKamokuData[2];					// 様式②、⑪で科目行がインポートされたかどうかのサイン		[0]:様式② [1]:様式⑪
// ------------------------------
	BOOL			csvKamokuData[3];					// 様式②、⑧、⑪で科目行がインポートされたかどうかのサイン		[0]:様式② [1]:様式⑧ [2]:様式⑪
// 改良No.21-0086,21-0529 cor <--
// midori 156935 add <--


	// 親ウィンドウのポインタを保持
	m_pWnd		= pParent;

	//インポートオプションダイアログにインポート元にあるファイルを確認するため，呼び出し順の変更20111007----->
	// メッセージ処理用
	CimpUc000Common* imp = new CimpUc000Common();
	imp->m_uImpCsvInfo.fImport = TRUE;
	_tcscpy_s (imp->m_uImpCsvInfo.szFolderPath, sizeof(imp->m_uImpCsvInfo.szFolderPath), strFolderPath);

// midori 160603 del -->
	//// 指定フォルダ有無確認
	//if ( PathIsDirectory( strFolderPath ) == FALSE ){
	//	// 確認メッセージ表示
	//	(void)imp->ShowMessageBox(IMP_ERRCODE_FINDFOLDER, m_pWnd);
	//	// メッセージ用オブジェクトを解放する
	//	if ( imp != NULL ) { imp->Fin(); delete imp; }
	//	// エラーを返す
	//	return IMP_ERRCODE_FINDFOLDER;
	//}
// midori 160603 del <--

	// INIファイルへの指定フォルダ保存
	SaveFolderPath(TRUE, strFolderPath);

	// Zvolumeテーブルオープン
	if ( pZmSub->VolumeOpen() != 0 ) {
		nErrorCode = IMP_ERRCODE_DB;
	}
	if ( pZmSub->zvol == NULL ) {
		nErrorCode = IMP_ERRCODE_DB;
	}
	// エラー処理
	if ( nErrorCode != IMP_ERRCODE_SUCCESS ) {
		// 確認メッセージ表示する(DBエラーの場合、終了)
		int nRetValue = imp->ShowMessageBox(nErrorCode, m_pWnd);
		// メッセージ用オブジェクトを解放する
		if ( imp != NULL ) { imp->Fin(); delete imp; }
		// エラーを返す
		return IMP_ERRCODE_DB;
	}

	// 会社マスタから会社名取得
	CUcFunctionCommon* pUcFuncCmn;
	pUcFuncCmn = new CUcFunctionCommon();
	csCompanyName = pUcFuncCmn->DeleteRightSpace( pZmSub->zvol->c_nam );	// 会社名(末尾空白削除)
	delete pUcFuncCmn;

// midori 152109 add -->
	pZmSub->VolumeClose();
// midori 152109 add <--

	int bitSheet[SHEET_NUM];
	int base = 0x01;

// midori 150894 add -->
	// CSV情報を様式別のエラー情報として使用するため、ここで初期化をする
	ZeroMemory(&imp->m_uImpCsvInfo, sizeof(imp->m_uImpCsvInfo));
// midori 150894 add <--

// midori 153829 del ------------------>
//	for ( int i = 0; i < SHEET_NUM; i++ ) {
//		// 配列に2^nの値をセットする ex)D0→bitSheet[0] = 1 D1→bitSheet[1] = 2...
//// midori 170504 del -->
//		//if( i == 0 ){
//		//	bitSheet[i] = base;
//		//}
//		//else{
//		//	bitSheet[i] = base = base << 1;
//		//}
//// midori 170504 del <--
//// midori 170504 add -->
//		// i=22:様式⑰－１
//		if(i == 22 || i == 42)				base = 0x01;
//		if(i == 0 || i == 22 || i == 42)	bitSheet[i] = base;
//		else								bitSheet[i] = base = base << 1;
//// midori 170504 add <--
//
//// midori 150894 del -->
//		//// CSV情報の初期化をする
//		//ZeroMemory(&imp->m_uImpCsvInfo, sizeof(imp->m_uImpCsvInfo));
//		//imp->m_uImpCsvInfo.fImport = TRUE;
//// midori 150894 del <--
//
//		// 内訳書CSVオープン
//		csCsvFilename = strFolderPath + _T("\\") + UcCsvFile[i];
//		int nRet = csvUcFile.CsvOpen(csCsvFilename, CSVREAD);
//		// エラー処理
//		if ( nRet == FALSE ) {
//			continue;
//		}
//		//++++++++++++++++++++++++++++++++++++++++++++++++++
//		// 1行目：帳票名の取得＆チェック
//		//++++++++++++++++++++++++++++++++++++++++++++++++++
//		// エラーコードをクリア
//		nErrorCode = IMP_ERRCODE_SUCCESS;
//
//		// 帳票名がインポート可能かを監視
//		BOOL	fErrSheetName	= FALSE;
//
//// midori 150894 del -->
//		//// 現在行の情報を設定
//		//imp->m_uImpCsvInfo.nLine = 1;
//		//strcpy_s(imp->m_uImpCsvInfo.szId, sizeof(imp->m_uImpCsvInfo.szId), CSV_INF_ITEM_01);
//// midori 150894 del <--
//
//		// CSV項目数の設定
//		csaCsvLine.RemoveAll();
////		csaCsvLine.SetSize(CSV_INF_ITEM_CNT);					// [2009/07/10 Delete]
//
//		// CSVから1行分のデータを読み込む
//		nRet = csvUcFile.CsvRead(&csaCsvLine);
//
//		// STEP1: CSV読込エラー
//		if (nRet == FALSE) {
//			// 空行の場合もここを通る
//			nErrorCode		= IMP_ERRCODE_FIXDATA;				// 固定データが見つからない
//		} 
//		else {
//			// STEP2: 読み込み項目数のチェック
////			if (csaCsvLine.GetSize() != CSV_INF_ITEM_CNT) {
//			if (csaCsvLine.GetSize() < CSV_INF_ITEM_CNT) {		// [2009/07/10 Change for Excel
//				fErrSheetName	= TRUE;
//				nErrorCode		= IMP_ERRCODE_IMPORT;			// データインポート失敗
//			}
//			else {
//				// STEP3: 識別子チェック
//				if (csaCsvLine[0] != CSV_INF_ITEM_01) {
//					nErrorCode		= IMP_ERRCODE_FIXDATA;		// 固定データが見つからない
//				}
//				else {
//					// STEP4: 帳票名チェック
//					csSheetName	= pUcFuncCmn->DeleteRightSpace(csaCsvLine[1]);	// 帳票名（末尾スペース削除）
//					int nLength	= csSheetName.GetLength();						// 帳票名の文字数（バイト数）
//					// 文字数が有効範囲内か？（全角35文字）
//					if (nLength > (35*2)) {
//						fErrSheetName	= TRUE;
//						nErrorCode		= IMP_ERRCODE_IMPORT;	// データインポート失敗
//					}
//// midori 150894 del -->
//					//else{	//読み込んだ様式名称が正しい場合，その様式番号，様式名称，ファイル名をインポートオプションアレイに登録
//					//	ImpOpt	datImp;
//					//	datImp.SheetNo		= UcSheetNumber[i];		// 様式番号
//					//	datImp.SheetName	= csSheetName;			// 様式名称
//					//	datImp.FileName		= UcCsvFile[i];			// ファイル名
//					//	datImp.bitTarget	= bitSheet[i];			// D?ビット
//					//	arrImpStct.Add( datImp );
//					//}
//// midori 150894 del <--
//				}
//			}
//		}
//
//		//--------------------------------------------------
//		// エラーコード判断
//		// IMP_ERRCODE_FIXDATA、IMP_ERRCODE_IMPORT
//		//--------------------------------------------------
//// midori 150894 cor -->
//		//if ( nErrorCode != IMP_ERRCODE_SUCCESS ) {			// エラー処理
//
//		//	if (nErrorCode == IMP_ERRCODE_FIXDATA) {
//		//		// 確認メッセージを表示する
//		//		int nRetValue = imp->ShowMessageBox(nErrorCode, m_pWnd);
//		//		// CSV操作オブジェクトを閉じる
//		//		csvUcFile.Close();
//		//		// 確認メッセージの戻り値を判断する
//		//		if ( nRetValue == IDYES ) {		// [はい] → 次の帳票処理へ
//		//			continue;
//		//		} else {						// [いいえ] → 処理中断
//		//			// 各オブジェクトの解放
//		//			if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
//		//			// エラーを返す
//		//			return IMP_ERRCODE_FIXDATA;
//		//		}
//		//	}
//
//		//	if (nErrorCode == IMP_ERRCODE_IMPORT) {
//		//		// 確認メッセージを表示する
//		//		int nRetValue = imp->ShowMessageBox(IMP_ERRCODE_IMPORT, m_pWnd);
//		//		// 確認メッセージの戻り値を判断する
//		//		if ( nRetValue == IDYES ) {		// [はい] → 次の行処理へ
//		//		} else {						// [いいえ] → 処理中断
//		//			// CSV操作オブジェクトを閉じる
//		//			csvUcFile.Close();
//		//			// 各オブジェクトの解放
//		//			if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
//		//			// エラーを返す
//		//			return IMP_ERRCODE_IMPORT;
//		//		}
//		//	}
//		//}
//// -------------------------------
//		if ( nErrorCode != IMP_ERRCODE_SUCCESS ) {
//			// 読み込んだ様式名称がエラーの場合，CSV情報にフラグをセット
//			imp->m_uImpCsvInfo.szErr[i] = 1;
//		}
//		else	{
//			// 読み込んだ様式名称が正しい場合，その様式番号，様式名称，ファイル名を
//			// インポートオプションアレイに登録
//			ImpOpt	datImp;
//			datImp.SheetNo		= UcSheetNumber[i];		// 様式番号
//			datImp.SheetName	= csSheetName;			// 様式名称
//			datImp.FileName		= UcCsvFile[i];			// ファイル名
//			datImp.bitTarget	= bitSheet[i];			// D?ビット
//			arrImpStct.Add( datImp );
//		}
//// midori 150894 cor <--
//		// CSV操作オブジェクトを閉じる
//		csvUcFile.Close();
//	}
//
//// midori 150894 add -->
//	nErrorCode = IMP_ERRCODE_SUCCESS;
//	for ( int i = 0; i < SHEET_NUM; i++ ) {
//		if(imp->m_uImpCsvInfo.szErr[i] == 1)	{
//			nErrorCode = IMP_ERRCODE_IMPORT_TITLE;
//			break;
//		}
//	}
//	if(nErrorCode != IMP_ERRCODE_SUCCESS)	{
//		if((int)arrImpStct.GetCount() == 0)	{
//			imp->m_uImpCsvInfo.nAllErr = 1;
//		}
//		// 確認メッセージを表示する
//		int nRetValue = imp->ShowMessageBox(IMP_ERRCODE_IMPORT_TITLE,m_pWnd,pZmSub);
//		// 確認メッセージの戻り値を判断する
//		if(nRetValue != IDYES)	{	// [いいえ] → 処理中断
//			// 各オブジェクトの解放
//			if(imp != NULL)	{
//				imp->Fin();
//				delete imp;
//			}
//			// エラーを返す
//			return IMP_ERRCODE_IMPORT;
//		}
//	}
//// midori 150894 add <--
//
//	//<-----20111007
//
//	// インポートオプションダイアログの表示
//	if ( pParent && dlgOpt.ShowDialog( pZmSub, TRUE, arrImpStct ) != ID_DLG_OK ) {	//インポートオプションダイアログ表示<-----20111007
//		// [キャンセル]ボタン押下
//		if ( imp != NULL ) {
//			imp->Fin();
//			delete imp;       
//		}
//		return IMP_DIALOG_ERRCODE_CANCEL;
//	}
//
//	// 帳票ごとの情報テーブル(uc_inf_sub)の準備
//	CdbUcInfSub* pUcInfSub;
//	pUcInfSub = new CdbUcInfSub(pZmSub->m_database);
// midori 153829 del <------------------
// midori 153829 add ------------------>
	for ( int i = 0; i < SHEET_NUM; i++ ) {
		// 配列に2^nの値をセットする ex)D0→bitSheet[0] = 1 D1→bitSheet[1] = 2...
		// i=22:様式⑰－１
		if(i == 22 || i == 42)				base = 0x01;
		if(i == 0 || i == 22 || i == 42)	bitSheet[i] = base;
		else								bitSheet[i] = base = base << 1;

		// 内訳書CSVオープン
		csCsvFilename = strFolderPath + _T("\\") + UcCsvFile[i];
// 157782,157783 del -->
		//int nRet = csvUcFile.CsvOpen(csCsvFilename, CSVREAD);
// 157782,157783 del <--
// 157782,157783 add -->
		int nRet = csvUcFile.CsvOpen(csCsvFilename,CSVREAD,CFile::shareDenyNone);
// 157782,157783 add <--
		// エラー処理
		if ( nRet == FALSE ) {
			continue;
		}
		//++++++++++++++++++++++++++++++++++++++++++++++++++
		// 1行目：帳票名の取得＆チェック
		//++++++++++++++++++++++++++++++++++++++++++++++++++
		// エラーコードをクリア
		nErrorCode = IMP_ERRCODE_SUCCESS;

		// 帳票名がインポート可能かを監視
		BOOL	fErrSheetName	= FALSE;

		// CSV項目数の設定
		csaCsvLine.RemoveAll();

		// CSVから1行分のデータを読み込む
		nRet = csvUcFile.CsvRead(&csaCsvLine);

		// STEP1: CSV読込エラー
		if (nRet == FALSE) {
			// 空行の場合もここを通る
			nErrorCode		= IMP_ERRCODE_FIXDATA;				// 固定データが見つからない
		} 
		else {
			// STEP2: 読み込み項目数のチェック
			if (csaCsvLine.GetSize() < CSV_INF_ITEM_CNT) {		// [2009/07/10 Change for Excel
				fErrSheetName	= TRUE;
				nErrorCode		= IMP_ERRCODE_IMPORT;			// データインポート失敗
			}
			else {
				// STEP3: 識別子チェック
				if (csaCsvLine[0] != CSV_INF_ITEM_01) {
					nErrorCode		= IMP_ERRCODE_FIXDATA;		// 固定データが見つからない
				}
				else {
					// STEP4: 帳票名チェック
					csSheetName	= pUcFuncCmn->DeleteRightSpace(csaCsvLine[1]);	// 帳票名（末尾スペース削除）
					int nLength	= csSheetName.GetLength();						// 帳票名の文字数（バイト数）
					// 読み込んだ様式名称が正しい場合または文字数オーバーの場合、その様式番号，様式名称，ファイル名を
					// インポートオプションアレイに登録
					ImpOpt	datImp;
					datImp.SheetNo		= UcSheetNumber[i];		// 様式番号
					datImp.SheetName	= csSheetName;			// 様式名称
					datImp.FileName		= UcCsvFile[i];			// ファイル名
					datImp.bitTarget	= bitSheet[i];			// D?ビット
					arrImpStct.Add( datImp );
				}
			}
		}

		//--------------------------------------------------
		// エラーコード判断
		// IMP_ERRCODE_FIXDATA、IMP_ERRCODE_IMPORT
		//--------------------------------------------------
		if ( nErrorCode == IMP_ERRCODE_FIXDATA || nErrorCode == IMP_ERRCODE_IMPORT ) {
			// 読み込んだ様式名称がエラーの場合，CSV情報にフラグをセット
			imp->m_uImpCsvInfo.szErr[i] = 1;
		}
		// CSV操作オブジェクトを閉じる
		csvUcFile.Close();
	}

	nErrorCode = IMP_ERRCODE_SUCCESS;
	for ( int i = 0; i < SHEET_NUM; i++ ) {
		if(imp->m_uImpCsvInfo.szErr[i] == 1)	{
			nErrorCode = IMP_ERRCODE_IMPORT_TITLE;
			break;
		}
	}
	if(nErrorCode != IMP_ERRCODE_SUCCESS)	{
		if((int)arrImpStct.GetCount() == 0)	{
			imp->m_uImpCsvInfo.nAllErr = 1;
		}
		// 確認メッセージを表示する
		int nRetValue = imp->ShowMessageBox(IMP_ERRCODE_IMPORT_TITLE,m_pWnd,pZmSub);
		// 確認メッセージの戻り値を判断する
		if(nRetValue != IDYES)	{	// [いいえ] → 処理中断
			// 各オブジェクトの解放
			if(imp != NULL)	{
				imp->Fin();
				delete imp;
			}
			// エラーを返す
			return IMP_ERRCODE_IMPORT;
		}
	}

	// 帳票ごとの情報テーブル(uc_inf_sub)の準備
	CdbUcInfSub* pUcInfSub;
	pUcInfSub = new CdbUcInfSub(pZmSub->m_database);

	// 帳表タイトルが最大文字数を超えた場合のエラーチェック
	CString msg = _T("");
	cst.Empty();
	for (ii = 0;ii < arrImpStct.GetCount();ii++) {
		len	= arrImpStct.GetAt(ii).SheetName.GetLength();						// 帳票名の文字数（バイト数）
		// 文字数が有効範囲内か？（全角35文字）
		if (len > (35*2)) {
			for(jj=ii; jj<SHEET_NUM; jj++){
				if( arrImpStct.GetAt(ii).SheetNo.Compare( UcSheetNumber[jj] ) == 0 ){
					FormIndex = jj+1;
				}
			}
			if ( pUcInfSub->Init(FormIndex) != DB_ERR_OK ) {
				// エラーメッセージを表示
				(void)imp->ShowMessageBox(IMP_ERRCODE_DB, m_pWnd);
				// 各オブジェクトの解放
				if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
				if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
				// エラーを返す
				return IMP_ERRCODE_DB;
			}

			// 文字数オーバーのエラーメッセージ作成
			cst.Format(cst + _T("%s 【%s】\r\n"),arrImpStct.GetAt(ii).FileName,pUcInfSub->m_Title);
		}
	}
	if(cst.IsEmpty() == FALSE)	{
		msg = _T("帳票タイトルが最大文字数を超えています。（最大３５文字）\r\n\r\n");
		msg += cst + _T("\r\n");
		msg = msg + _T("帳票タイトルを最大文字数までインポートしますか？\r\n");
		msg = msg + _T("　「はい」　　　最大文字数までインポートする\r\n");
		msg = msg + _T("　「いいえ」　　上記の様式をスキップする\r\n");
		msg = msg + _T("　「キャンセル」処理を中断する\r\n");
		// メッセージボックスの表示
		int nRetValue = ICSMessageBox(msg, (MB_ICONWARNING | MB_YESNOCANCEL), 0, 0, m_pWnd);

		// 確認メッセージの戻り値を判断する
		if ( nRetValue == IDYES ) {
			// arrImpStctから最大文字数を超えている様式の文字数を35文字でカットする
			for (ii = 0;ii < arrImpStct.GetCount();ii++) {
				len	= arrImpStct.GetAt(ii).SheetName.GetLength();						// 帳票名の文字数（バイト数）
				// 文字数が有効範囲内か？（全角35文字）
				if (len > (35*2)) {
					arrImpStct.GetAt(ii).SheetName = arrImpStct.GetAt(ii).SheetName.Left(70);
				}
			}
		}
		else if( nRetValue == IDNO ){		// [いいえ] → スキップする
			// arrImpStctから最大文字数を超えている様式を削除する
			for (ii = (int)(arrImpStct.GetCount()-1);ii >= 0;ii--) {
				len	= arrImpStct.GetAt(ii).SheetName.GetLength();						// 帳票名の文字数（バイト数）
				// 文字数が有効範囲内か？（全角35文字）
				if (len > (35*2)) {
					arrImpStct.RemoveAt(ii);
				}
			}
			if((int)arrImpStct.GetCount() == 0)	{
				ICSMessageBox(_T("インポートできる様式はありませんでした。\r\n処理を中断します。"), MB_OK, 0, 0, m_pWnd);

				// 各オブジェクトの解放
				if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
				// エラーを返す
				return IMP_ERRCODE_IMPORT;
			}
		}
		else if( nRetValue == IDCANCEL ){		// [キャンセル] → 処理中断
			// 各オブジェクトの解放
			if(imp != NULL)	{
				imp->Fin();
				delete imp;
			}
			// エラーを返す
			return IMP_ERRCODE_IMPORT;
		}
	}

	// インポートオプションダイアログの表示
	if ( pParent && dlgOpt.ShowDialog( pZmSub, TRUE, arrImpStct ) != ID_DLG_OK ) {	//インポートオプションダイアログ表示<-----20111007
		// [キャンセル]ボタン押下
		if ( imp != NULL ) {
			imp->Fin();
			delete imp;       
		}
		return IMP_DIALOG_ERRCODE_CANCEL;
	}
// midori 153829 add <------------------

	// 帳表全体の情報テーブル(uc_inf_main)の準備
	BOOL isRegAutoRef = TRUE;	// 「取引先、金融機関を自動で登録する」　TRUEはこのフラグができる以前のデフォルト値
	{
	CdbUcInfMain* ucInfMain = new CdbUcInfMain(pZmSub->m_database);
	if(ucInfMain->Init() != DB_ERR_OK){
		// 各オブジェクトの解放
		if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
		if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
		ucInfMain->Fin();
		delete ucInfMain;
		ucInfMain = NULL;
		return IMP_ERRCODE_DB;
	}
	CCtrlMainGeneralVar cm(*ucInfMain);
	isRegAutoRef = cm.bRegAutoRef;
// midori 190301 0415 del -->
//// midori 190301 add -->
//	bG_Kanso = cm.bUseStyle;
//// midori 190301 add <--
// midori 190301 0415 del <--
// midori 190301 0415 add -->
	if(cm.bUseStyle)	bG_Kanso = TRUE;
	else				bG_Kanso = FALSE;
// midori 190301 0415 add <--
// 登録番号（法人番号）_23/11/21 add -->
	if(cm.bUseStyle2)	bG_InvNo = TRUE;
	else				bG_InvNo = FALSE;
// 登録番号（法人番号）_23/11/21 add <--

// 改良No.21-0086,21-0529 add -->
	CZmGen8			ZmGen;
	m_Invoice = ZmGen.IsInvoiceVupMaster(pZmSub);
// 改良No.21-0086,21-0529 add <--

	ucInfMain->Fin();
	delete ucInfMain;
	ucInfMain = NULL;
	}


	// プログレスバーの準備
	ICSProgressWnd* icsPrgBar;
	icsPrgBar = InitProgress( m_pWnd, 0, (int)arrImpStct.GetCount(), ID_IMPORT );
	(void)ShowProgress( icsPrgBar, 0 );

	// 会社名が異なるエラーが発生したかを監視
	BOOL	fErrCoName		= FALSE;

// midori 156935 add -->
	csvKamokuData[0] = FALSE;
	csvKamokuData[1] = FALSE;
	csvKamokuData[2] = FALSE;	// 改良No.21-0086,21-0529 add
// midori 156935 add <--
// midori 153829 add ------------------>
	FormIndex = 0;
// midori 153829 add <------------------
	for ( int i = 0; i < arrImpStct.GetCount(); i++ ) {		//インポート元フォルダにあるファイルの数しかループしない<-----20111007
		//出力チェックボックスがOFFの場合はスルー
		int retCheck = arrImpStct.GetAt(i).chkState;
		if( retCheck == FALSE){
			//FormIndex++;
			continue;
		}
		//ダイアログに表示されなかった様式のスキップ20111007----->
		for(int j=i; j<SHEET_NUM; j++){
			if( arrImpStct.GetAt(i).SheetNo.Compare( UcSheetNumber[j] ) == 0 ){
				FormIndex = j+1;
			}
		}
		//<-----20111007

		// エラーコードをクリア
		nErrorCode = IMP_ERRCODE_SUCCESS;

		// CSV情報の初期化をする
		ZeroMemory(&imp->m_uImpCsvInfo, sizeof(imp->m_uImpCsvInfo));
		imp->m_uImpCsvInfo.fImport = TRUE;
		
		// ファイル名/帳票名を設定する
		_tcscpy_s (imp->m_uImpCsvInfo.szFileName , sizeof(imp->m_uImpCsvInfo.szFileName) , arrImpStct.GetAt(i).FileName  );	//<-----20111007　インポートオプション構造体のファイル名，様式名称を登録
		_tcscpy_s (imp->m_uImpCsvInfo.szSheetName, sizeof(imp->m_uImpCsvInfo.szSheetName), arrImpStct.GetAt(i).SheetName);	// [2009/07/08 Add for Error Message]<-----20111007

		// 内訳書CSVオープン
		csCsvFilename = strFolderPath + _T("\\") + arrImpStct.GetAt(i).FileName;
// 157782,157783 del -->
		//int nRet = csvUcFile.CsvOpen(csCsvFilename, CSVREAD);
// 157782,157783 del <--
// 157782,157783 add -->
		int nRet = csvUcFile.CsvOpen(csCsvFilename,CSVREAD,CFile::shareDenyNone);
// 157782,157783 add <--
		// エラー処理
		if ( nRet == FALSE ) {	
			// プログレスバーを隠す
			icsPrgBar = HideProgress( icsPrgBar, m_pWnd, 0, SHEET_NUM, ID_IMPORT );
			// 確認メッセージを表示する
			int nRetValue = imp->ShowMessageBox(IMP_ERRCODE_CSVOPEN, m_pWnd);
			// 確認メッセージの戻り値を判断する
			if ( nRetValue == IDYES ) {		// [はい] → 次の帳票処理へ
				// プログレスバーの再表示
				(void)ShowProgress( icsPrgBar, i );
				FormIndex++;								// [2009/08/25 Add]
				continue;
			} else {						// [いいえ] → 処理中断
				// 各オブジェクトの解放
				if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
				if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
				if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
				// エラーを返す
				return IMP_ERRCODE_CSVOPEN;
			}
		}

		//###################
		//### Info. Block ###
		//###################

		//++++++++++++++++++++++++++++++++++++++++++++++++++
		// 1行目：帳票名の取得＆チェック
		//++++++++++++++++++++++++++++++++++++++++++++++++++
		// エラーコードをクリア
		nErrorCode = IMP_ERRCODE_SUCCESS;

		// 帳票名がインポート可能かを監視
		BOOL	fErrSheetName	= FALSE;

		// 現在行の情報を設定
		imp->m_uImpCsvInfo.nLine = 1;
		strcpy_s(imp->m_uImpCsvInfo.szId, sizeof(imp->m_uImpCsvInfo.szId), CSV_INF_ITEM_01);

		// CSV項目数の設定
		csaCsvLine.RemoveAll();
//		csaCsvLine.SetSize(CSV_INF_ITEM_CNT);					// [2009/07/10 Delete]

		// CSVから1行分のデータを読み込む
		nRet = csvUcFile.CsvRead(&csaCsvLine);

// midori 153829 del ------------------>
//		// STEP1: CSV読込エラー
//		if (nRet == FALSE) {
//			// 空行の場合もここを通る
//			nErrorCode		= IMP_ERRCODE_FIXDATA;				// 固定データが見つからない
//		} else {
//			// STEP2: 読み込み項目数のチェック
////			if (csaCsvLine.GetSize() != CSV_INF_ITEM_CNT) {
//			if (csaCsvLine.GetSize() < CSV_INF_ITEM_CNT) {		// [2009/07/10 Change for Excel
//				fErrSheetName	= TRUE;
//				nErrorCode		= IMP_ERRCODE_IMPORT;			// データインポート失敗
//			}
//			else {
//				// STEP3: 識別子チェック
//				if (csaCsvLine[0] != CSV_INF_ITEM_01) {
//					nErrorCode		= IMP_ERRCODE_FIXDATA;		// 固定データが見つからない
//				}
//				else {
//					// STEP4: 帳票名チェック
//					csSheetName	= pUcFuncCmn->DeleteRightSpace(csaCsvLine[1]);	// 帳票名（末尾スペース削除）
//					int nLength	= csSheetName.GetLength();						// 帳票名の文字数（バイト数）
//					// 文字数が有効範囲内か？（全角35文字）
//					if (nLength > (35*2)) {
//						fErrSheetName	= TRUE;
//						nErrorCode		= IMP_ERRCODE_IMPORT;	// データインポート失敗
//					}
//				}
//			}
//		}
//
//		//--------------------------------------------------
//		// エラーコード判断
//		// IMP_ERRCODE_FIXDATA、IMP_ERRCODE_IMPORT
//		//--------------------------------------------------
//		if ( nErrorCode != IMP_ERRCODE_SUCCESS ) {			// エラー処理
//
//			if (nErrorCode == IMP_ERRCODE_FIXDATA) {
//				// プログレスバーを隠す
//				icsPrgBar = HideProgress( icsPrgBar, m_pWnd, 0, SHEET_NUM, ID_IMPORT );
//				// 確認メッセージを表示する
//				int nRetValue = imp->ShowMessageBox(nErrorCode, m_pWnd);
//				// CSV操作オブジェクトを閉じる
//				csvUcFile.Close();
//				// 確認メッセージの戻り値を判断する
//				if ( nRetValue == IDYES ) {		// [はい] → 次の帳票処理へ
//					// プログレスバーの再表示
//					(void)ShowProgress( icsPrgBar, i );
//					continue;
//				} else {						// [いいえ] → 処理中断
//					// 各オブジェクトの解放
//					if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
//					if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
//					if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
//					// エラーを返す
//					return IMP_ERRCODE_FIXDATA;
//				}
//			}
//
//			if (nErrorCode == IMP_ERRCODE_IMPORT) {
//				// プログレスバーを隠す
//				icsPrgBar = HideProgress( icsPrgBar, m_pWnd, 0, SHEET_NUM, ID_IMPORT);
//				// 確認メッセージを表示する
//				int nRetValue = imp->ShowMessageBox(IMP_ERRCODE_IMPORT, m_pWnd);
//				// 確認メッセージの戻り値を判断する
//				if ( nRetValue == IDYES ) {		// [はい] → 次の行処理へ
//					// プログレスバーの再表示
//					(void)ShowProgress( icsPrgBar, i );
//				} else {						// [いいえ] → 処理中断
//					// CSV操作オブジェクトを閉じる
//					csvUcFile.Close();
//					// 各オブジェクトの解放
//					if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
//					if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
//					if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
//					// エラーを返す
//					return IMP_ERRCODE_IMPORT;
//				}
//			}
//		}
// midori 153829 del <------------------
// midori 153829 add ------------------>
		// 帳表名の取得
		csSheetName = arrImpStct.GetAt(i).SheetName;
// midori 153829 add <------------------

		//++++++++++++++++++++++++++++++++++++++++++++++++++
		// 2行目：会社名の取得＆チェック
		//++++++++++++++++++++++++++++++++++++++++++++++++++
		// エラーコードをクリア
		nErrorCode = IMP_ERRCODE_SUCCESS;

		// 現在行の情報を設定
		imp->m_uImpCsvInfo.nLine = 2;
		strcpy_s(imp->m_uImpCsvInfo.szId, sizeof(imp->m_uImpCsvInfo.szId), CSV_INF_ITEM_02);

		// CSV項目数の設定
		csaCsvLine.RemoveAll();
//		csaCsvLine.SetSize(CSV_INF_ITEM_CNT);					// [2009/07/10 Delete]

		// CSVから1行分のデータを読み込む
		nRet = csvUcFile.CsvRead(&csaCsvLine);

		// STEP1: CSV読込エラー
		if (nRet == FALSE) {
			// 空行の場合もここを通る
			nErrorCode = IMP_ERRCODE_FIXDATA;					// 固定データが見つからない
		}
		else {
			// STEP2: 読み込み項目数のチェック
//			if (csaCsvLine.GetSize() != CSV_INF_ITEM_CNT) {
			if (csaCsvLine.GetSize() < CSV_INF_ITEM_CNT) {		// [2009/07/23 Fix]
				fErrCoName = TRUE;
				nErrorCode = IMP_ERRCODE_CONAME;				// 会社名が異なる（ことにする）
			}
			else {
				// STEP3: 識別子チェック
				if (csaCsvLine[0] != CSV_INF_ITEM_02) {
					nErrorCode = IMP_ERRCODE_FIXDATA;			// 固定データが見つからない
				}
				else {
					// STEP4: 会社名チェック
					CString strCoName = pUcFuncCmn->DeleteRightSpace(csaCsvLine[1]);	// 会社名（末尾スペース削除）
					if (strCoName.Compare(csCompanyName) != 0) {
						// 会社名チェックエラーは1回のみ表示
						// 2回目以降は、会社名が異なっていもメッセージを出さずに処理を継続
						if (fErrCoName == TRUE) {
							nErrorCode	= IMP_ERRCODE_SUCCESS;	// 正常
						}
						else {
							fErrCoName	= TRUE;
							nErrorCode	= IMP_ERRCODE_CONAME;	// 会社名が異なる
						}

					}
				}
			}
		}

		//--------------------------------------------------
		// エラーコード判断
		// IMP_ERRCODE_FIXDATA、IMP_ERRCODE_CONAME
		//--------------------------------------------------
		if ( nErrorCode != IMP_ERRCODE_SUCCESS ) {			// エラー処理
			if (nErrorCode == IMP_ERRCODE_FIXDATA) {
				// プログレスバーを隠す
				icsPrgBar = HideProgress( icsPrgBar, m_pWnd, 0, SHEET_NUM, ID_IMPORT );
				// 確認メッセージを表示する
				int nRetValue = imp->ShowMessageBox(nErrorCode, m_pWnd);
				// CSV操作オブジェクトを閉じる
				csvUcFile.Close();
				// 確認メッセージの戻り値を判断する
				if ( nRetValue == IDYES ) {		// [はい] → 次の帳票処理へ
					// プログレスバーの再表示
					(void)ShowProgress( icsPrgBar, i );
					continue;
				} else {						// [いいえ] → 処理中断
					// 各オブジェクトの解放
					if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
					if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
					if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
					// エラーを返す
					return IMP_ERRCODE_FIXDATA;
				}
			}

			if (nErrorCode == IMP_ERRCODE_CONAME) {
				// プログレスバーを隠す
				icsPrgBar = HideProgress( icsPrgBar, m_pWnd, 0, SHEET_NUM, ID_IMPORT );
				// 確認メッセージを表示する
				int nRetValue = imp->ShowMessageBox(nErrorCode, m_pWnd);
				// 確認メッセージの戻り値を判断する
				if ( nRetValue == IDYES ) {		// [はい] → 現在の帳票の処理を継続
					nErrorCode = IMP_ERRCODE_SUCCESS;
					// プログレスバーの再表示
					(void)ShowProgress( icsPrgBar, i );
				} else {						// [いいえ] → 処理中断
					// 各オブジェクトの解放
					if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
					if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
					if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
					// エラーを返す
					return nErrorCode;
				}
			}
		}

// 157766 add -->
		//++++++++++++++++++++++++++++++++++++++++++++++++++
		// 3行目：項目数のチェック
		//++++++++++++++++++++++++++++++++++++++++++++++++++
		BOOL	fErrNumberOfItem = FALSE;
		nRet = pUcInfSub->Init(FormIndex++);
		if ( nRet != DB_ERR_OK ) {
			// プログレスバー終了
			icsPrgBar->EndProgress();
			// エラーメッセージ表示
			(void)imp->ShowMessageBox(IMP_ERRCODE_DB, m_pWnd);
			// CSV操作オブジェクトを閉じる
			csvUcFile.Close();
			// 各オブジェクトの解放
			if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放する
			if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
			if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
			// エラーを返す
			return IMP_ERRCODE_DB;
		}
		// 項目数のチェックでエラーになった場合、帳票名は取り込まないのでここでチェックを行う
		// CSVファイル用バッファを削除(Data Blockの準備)
		BOOL	bTitleRead=FALSE;
		csaCsvLine.RemoveAll();
		bTitleRead = csvUcFile.CsvRead(&csaCsvLine);
		if(bTitleRead != FALSE)	{
// 登録番号（法人番号）_23/11/21 cor -->
			//nErrorCode = imp->NumberOfItemCheck(bG_Kanso,pUcInfSub->m_FormSeq,&csaCsvLine);
// -------------------------------------
			nErrorCode = imp->NumberOfItemCheck(bG_InvNo,bG_Kanso,pUcInfSub->m_FormSeq,&csaCsvLine);
// 登録番号（法人番号）_23/11/21 cor <--
			if(nErrorCode == IMP_ERRCODE_NUMOFITEM)	{
// 157781 add -->
				fErrNumberOfItem = TRUE;
// 157781 add <--
				imp->m_uImpCsvInfo.nLine = 3;
				// プログレスバーを隠す
				icsPrgBar = HideProgress(icsPrgBar,m_pWnd,0,SHEET_NUM,ID_IMPORT);
				// 確認メッセージを表示する
				int nRetValue = imp->ShowMessageBox(nErrorCode,m_pWnd);
				// CSV操作オブジェクトを閉じる
				csvUcFile.Close();
				// 確認メッセージの戻り値を判断する
				if( nRetValue == IDYES)	{		// [はい] → 次の帳票処理へ
					// プログレスバーの再表示
					(void)ShowProgress(icsPrgBar,i);
// 157781 del -->
					//continue;
// 157781 del <--
				}
				else	{						// [いいえ] → 処理中断
					// 各オブジェクトの解放
					if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
					if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
					if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
					// エラーを返す
					return IMP_ERRCODE_NUMOFITEM;
				}
			}
		}
// 157766 add <--

		//++++++++++++++++++++++++++++++++++++++++++++++++++
		// 帳票名の登録
		//++++++++++++++++++++++++++++++++++++++++++++++++++
		// 対象帳票を指定（インデックスをインクリメント）

// midori 170504 del -->
		//// 帳票名にエラーがなければ登録
		//if (fErrSheetName != TRUE) {
		//	// 帳票名のテーブル登録
		//	nRet = pUcInfSub->Init(FormIndex++);
		//	if ( nRet != DB_ERR_OK ) {
		//		// プログレスバー終了
		//		icsPrgBar->EndProgress();
		//		// エラーメッセージ表示
		//		(void)imp->ShowMessageBox(IMP_ERRCODE_DB, m_pWnd);
		//		// CSV操作オブジェクトを閉じる
		//		csvUcFile.Close();
		//		// 各オブジェクトの解放
		//		if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放する
		//		if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
		//		if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
		//		// エラーを返す
		//		return IMP_ERRCODE_DB;
		//	}
// midori 170504 del <--
// midori 170504 add -->
// 157766 del -->
		//nRet = pUcInfSub->Init(FormIndex++);
		//if ( nRet != DB_ERR_OK ) {
		//	// プログレスバー終了
		//	icsPrgBar->EndProgress();
		//	// エラーメッセージ表示
		//	(void)imp->ShowMessageBox(IMP_ERRCODE_DB, m_pWnd);
		//	// CSV操作オブジェクトを閉じる
		//	csvUcFile.Close();
		//	// 各オブジェクトの解放
		//	if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放する
		//	if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
		//	if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
		//	// エラーを返す
		//	return IMP_ERRCODE_DB;
		//}
// 157766 del <--
// 157781 del -->
		//// 帳票名にエラーがなければ登録
		//if (fErrSheetName != TRUE) {
// 157781 del <--
// 157781 add -->
		// 帳票名、項目数にエラーがなければ登録
		if(fErrSheetName != TRUE && fErrNumberOfItem != TRUE) {
// 157781 add <--
			// 帳票名のテーブル登録
// midori 170504 add <--

			// 帳票名登録
			pUcInfSub->Edit();
			//既存処理が全ての様式をなめている為，アレイに登録されている様式までスキップ20111007----->
			int j;
			for(j=0; j<SHEET_NUM; j++){
				int retComp = arrImpStct.GetAt(i).FileName.Compare( UcCsvFile[j] );
				if( retComp == 0 )
					break;
			}
			//<-----20111007
			int FormNo = j + 1;
			//  uc_inf_sub の Title に登録
			if ( (FormNo != ID_FORMNO_142)			// 帳票⑭－２
			  && (FormNo != ID_FORMNO_152)			// 帳票⑮－２
			  && (FormNo != ID_FORMNO_162) ) {		// 帳票⑯－２以外は登録
				pUcInfSub->m_Title = csSheetName;
			}
			//  uc_inf_sub の TitleSaveTujo に登録（通常タイトル）
			if ( (FormNo == ID_FORMNO_041) || (FormNo == ID_FORMNO_042)		// 帳票④－１、④－２
			  || (FormNo == ID_FORMNO_101) || (FormNo == ID_FORMNO_102)		// 帳票⑩－１、⑩－２
			  || (FormNo == ID_FORMNO_141) || (FormNo == ID_FORMNO_151)		// 帳票⑭－１、⑮－１
			  || (FormNo == ID_FORMNO_153) || (FormNo == ID_FORMNO_161) ) {	// 帳票⑮－３、⑯－１に登録
				pUcInfSub->m_TitleSaveTujo = csSheetName;
			}
			//  uc_inf_sub の TitleSave に登録（個別タイトル）
			if ( (FormNo == ID_FORMNO_041) || (FormNo == ID_FORMNO_042)		// 帳票④－１、④－２
			  || (FormNo == ID_FORMNO_101) || (FormNo == ID_FORMNO_102)		// 帳票⑩－１、⑩－２
			  || (FormNo == ID_FORMNO_153) ) {								// 帳票⑮－３に登録
				pUcInfSub->m_TitleSave = csSheetName;
			}
			// テーブルの更新処理
			(void)pUcInfSub->Update();
		}

// 157766 del -->
		//// CSVファイル用バッファを削除(Data Blockの準備)
		//csaCsvLine.RemoveAll();
// 157766 del <--

		//##################
		//### Data Block ###
		//##################

// midori 170504 del -->
		////既存処理が全ての様式をなめている為，アレイに登録されている様式までスキップ20111007----->
		//int j;
		//for(j=0; j<SHEET_NUM; j++){
		//	int retComp = arrImpStct.GetAt(i).FileName.Compare( UcCsvFile[j] );
		//	if( retComp == 0 )
		//		break;
		//}
		////<-----20111007
// midori 170504 del <--

		// 内訳書テーブルの準備
		CimpUc000Common* pUcTbl;

// midori 170504 del -->
//		switch ( j + 1 ) {
// midori 170504 del <--
// midori 170504 add -->
		switch ( pUcInfSub->m_FormSeq ) {
// midori 170504 add <--
			case ID_FORMNO_011 :	// ①：預貯金等の内訳書
				pUcTbl		= (CimpUc000Common*)(new CimpUc011Yotyokin());
//				csaCsvLine.SetSize(CSV_011_ITEM_CNT);						// [2009/07/10 Delete]
				break;
			case ID_FORMNO_021 :	// ②：受取手形の内訳書
				pUcTbl		= (CimpUc000Common*)(new CimpUc021Uketoritegata());
//				csaCsvLine.SetSize(CSV_021_ITEM_CNT);						// [2009/07/10 Delete]
				break;
			case ID_FORMNO_031 :	// ③：売掛金（未収入金）の内訳書
				pUcTbl		= (CimpUc000Common*)(new CimpUc031Urikakekin());
//				csaCsvLine.SetSize(CSV_031_ITEM_CNT);						// [2009/07/10 Delete]
				break;
			case ID_FORMNO_041 :	// ④－１：仮払金（前渡金）の内訳書
				pUcTbl		= (CimpUc000Common*)(new CimpUc041Karibarai());
//				csaCsvLine.SetSize(CSV_041_ITEM_CNT);						// [2009/07/10 Delete]
				break;
			case ID_FORMNO_042 :	// ④－２：貸付金及び受取利息の内訳書
				pUcTbl		= (CimpUc000Common*)(new CimpUc042Kasituke());
//				csaCsvLine.SetSize(CSV_042_ITEM_CNT);						// [2009/07/10 Delete]
				break;
			case ID_FORMNO_051 :	// ⑤：棚卸資産の内訳書
				pUcTbl		= (CimpUc000Common*)(new CimpUc051Tanaoroshi());
//				csaCsvLine.SetSize(CSV_051_ITEM_CNT);						// [2009/07/10 Delete]
				break;
			case ID_FORMNO_061 :	// ⑥：有価証券の内訳書
				pUcTbl		= (CimpUc000Common*)(new CimpUc061Yuukasyouken());
//				csaCsvLine.SetSize(CSV_061_ITEM_CNT);						// [2009/07/10 Delete]
				break;
			case ID_FORMNO_071 :	// ⑦：固定資産の内訳書
				pUcTbl		= (CimpUc000Common*)(new CimpUc071Koteisisan());
//				csaCsvLine.SetSize(CSV_071_ITEM_CNT);						// [2009/07/10 Delete]
				break;
			case ID_FORMNO_081 :	// ⑧：支払手形の内訳書
				pUcTbl		= (CimpUc000Common*)(new CimpUc081Siharaitegata());
//				csaCsvLine.SetSize(CSV_081_ITEM_CNT);						// [2009/07/10 Delete]
				break;
			case ID_FORMNO_091 :	// ⑨：買掛金の内訳書
				pUcTbl		= (CimpUc000Common*)(new CimpUc091Kaikakekin());
//				csaCsvLine.SetSize(CSV_091_ITEM_CNT);						// [2009/07/10 Delete]
				break;
			case ID_FORMNO_101 :	// ⑩－１：仮受金の内訳書
				pUcTbl		= (CimpUc000Common*)(new CimpUc101Kariukekin());
//				csaCsvLine.SetSize(CSV_101_ITEM_CNT);						// [2009/07/10 Delete]
				break;
			case ID_FORMNO_102 :	// ⑩－２：源泉所得税預り金の内訳書
				pUcTbl		= (CimpUc000Common*)(new CimpUc102Gensan());
//				csaCsvLine.SetSize(CSV_102_ITEM_CNT);						// [2009/07/10 Delete]
				break;
			case ID_FORMNO_111 :	// ⑪：借入金及び支払利子の内訳書
				pUcTbl		= (CimpUc000Common*)(new CimpUc111Kariirekin());
//				csaCsvLine.SetSize(CSV_111_ITEM_CNT);						// [2009/07/10 Delete]
				break;
			case ID_FORMNO_121 :	// ⑫：土地の売上高等の内訳書
				pUcTbl		= (CimpUc000Common*)(new CimpUc121Totiuriage()); 
//				csaCsvLine.SetSize(CSV_121_ITEM_CNT);						// [2009/07/10 Delete]
				break;
			case ID_FORMNO_131 :	// ⑬：売上高等の事業所別内訳書
				pUcTbl		= (CimpUc000Common*)(new CimpUc131Uriagedaka());
//				csaCsvLine.SetSize(CSV_131_ITEM_CNT);						// [2009/07/10 Delete]
				break;
			case ID_FORMNO_141 :	// ⑭－１：役員報酬手当金等の内訳書
				pUcTbl		= (CimpUc000Common*)(new CimpUc141Yakuin());
//				csaCsvLine.SetSize(CSV_141_ITEM_CNT);						// [2009/07/10 Delete]
				break;
			case ID_FORMNO_142 :	// ⑭－２：人件費の内訳書
				pUcTbl		= (CimpUc000Common*)(new CimpUc142Jinkenhi());
//				csaCsvLine.SetSize(CSV_142_ITEM_CNT);						// [2009/07/10 Delete]
				break;
			case ID_FORMNO_151 :	// ⑮－１：地代家賃等の内訳書
				pUcTbl		= (CimpUc000Common*)(new CimpUc151Tidaiyatin());
//				csaCsvLine.SetSize(CSV_151_ITEM_CNT);						// [2009/07/10 Delete]
				break;
			case ID_FORMNO_152 :	// ⑮－２：権利金等の期中支払の内訳書
				pUcTbl		= (CimpUc000Common*)(new CimpUc152Kenrikin());
//				csaCsvLine.SetSize(CSV_152_ITEM_CNT);						// [2009/07/10 Delete]
				break;
			case ID_FORMNO_153 :	// ⑮－３：工業所所有権等の使用料の内訳書
				pUcTbl		= (CimpUc000Common*)(new CimpUc153Kougyou());
//				csaCsvLine.SetSize(CSV_153_ITEM_CNT);						// [2009/07/10 Delete]
				break;
			case ID_FORMNO_161 :	// ⑯－１：雑益等の内訳書
				pUcTbl		= (CimpUc000Common*)(new CimpUc161Zatueki());
//				csaCsvLine.SetSize(CSV_161_ITEM_CNT);						// [2009/07/10 Delete]
				break;
			case ID_FORMNO_162 :	// ⑯－２：雑損益の内訳書
				pUcTbl		= (CimpUc000Common*)(new CimpUc162Zatuson());
//				csaCsvLine.SetSize(CSV_162_ITEM_CNT);						// [2009/07/10 Delete]
				break;
// midori 170504 add -->
			case ID_FORMNO_171:			//	⑰－１：その他科目の内訳書①－１
			case ID_FORMNO_172:			//	⑰－２：その他科目の内訳書①－２
			case ID_FORMNO_173:			//	⑰－３：その他科目の内訳書①－３
			case ID_FORMNO_174:			//	⑰－４：その他科目の内訳書①－４
			case ID_FORMNO_175:			//	⑰－５：その他科目の内訳書①－５
			case ID_FORMNO_176:			//	⑰－６：その他科目の内訳書①－６
			case ID_FORMNO_177:			//	⑰－７：その他科目の内訳書①－７
			case ID_FORMNO_178:			//	⑰－８：その他科目の内訳書①－８
			case ID_FORMNO_179:			//	⑰－９：その他科目の内訳書①－９
			case ID_FORMNO_1710:		//	⑰－１０：その他科目の内訳書①－１０
			case ID_FORMNO_1711:		//	⑰－１１：その他科目の内訳書①－１１
			case ID_FORMNO_1712:		//	⑰－１２：その他科目の内訳書①－１２
			case ID_FORMNO_1713:		//	⑰－１３：その他科目の内訳書①－１３
			case ID_FORMNO_1714:		//	⑰－１４：その他科目の内訳書①－１４
			case ID_FORMNO_1715:		//	⑰－１５：その他科目の内訳書①－１５
			case ID_FORMNO_1716:		//	⑰－１６：その他科目の内訳書①－１６
			case ID_FORMNO_1717:		//	⑰－１７：その他科目の内訳書①－１７
			case ID_FORMNO_1718:		//	⑰－１８：その他科目の内訳書①－１８
			case ID_FORMNO_1719:		//	⑰－１９：その他科目の内訳書①－１９
			case ID_FORMNO_1720:		//	⑰－２０：その他科目の内訳書①－２０
				pUcTbl		= (CimpUc000Common*)(new CimpUc171Sonota1(pUcInfSub->m_FormSeq,pUcInfSub->m_FormOrder));
				break;
			case ID_FORMNO_181:			//	⑱－１：その他科目の内訳書②－１
			case ID_FORMNO_182:			//	⑱－２：その他科目の内訳書②－２
			case ID_FORMNO_183:			//	⑱－３：その他科目の内訳書②－３
			case ID_FORMNO_184:			//	⑱－４：その他科目の内訳書②－４
			case ID_FORMNO_185:			//	⑱－５：その他科目の内訳書②－５
			case ID_FORMNO_186:			//	⑱－６：その他科目の内訳書②－６
			case ID_FORMNO_187:			//	⑱－７：その他科目の内訳書②－７
			case ID_FORMNO_188:			//	⑱－８：その他科目の内訳書②－８
			case ID_FORMNO_189:			//	⑱－９：その他科目の内訳書②－９
			case ID_FORMNO_1810:		//	⑱－１０：その他科目の内訳書②－１０
			case ID_FORMNO_1811:		//	⑱－１１：その他科目の内訳書②－１１
			case ID_FORMNO_1812:		//	⑱－１２：その他科目の内訳書②－１２
			case ID_FORMNO_1813:		//	⑱－１３：その他科目の内訳書②－１３
			case ID_FORMNO_1814:		//	⑱－１４：その他科目の内訳書②－１４
			case ID_FORMNO_1815:		//	⑱－１５：その他科目の内訳書②－１５
			case ID_FORMNO_1816:		//	⑱－１６：その他科目の内訳書②－１６
			case ID_FORMNO_1817:		//	⑱－１７：その他科目の内訳書②－１７
			case ID_FORMNO_1818:		//	⑱－１８：その他科目の内訳書②－１８
			case ID_FORMNO_1819:		//	⑱－１９：その他科目の内訳書②－１９
			case ID_FORMNO_1820:		//	⑱－２０：その他科目の内訳書②－２０
				pUcTbl		= (CimpUc000Common*)(new CimpUc181Sonota2(pUcInfSub->m_FormSeq,pUcInfSub->m_FormOrder,_T(""),_T("")));
				break;
// midori 170504 add <--
			default :
				pUcTbl = 0;
				break;
		}

		// CSV情報の初期化をする
		ZeroMemory(&pUcTbl->m_uImpCsvInfo, sizeof(pUcTbl->m_uImpCsvInfo));
		pUcTbl->m_uImpCsvInfo.fImport = TRUE;
		
		// ファイル名/帳票名を設定する
		_tcscpy_s (pUcTbl->m_uImpCsvInfo.szFileName , sizeof(pUcTbl->m_uImpCsvInfo.szFileName ), arrImpStct.GetAt(i).FileName  );	//<-----20111007
		_tcscpy_s (pUcTbl->m_uImpCsvInfo.szSheetName, sizeof(pUcTbl->m_uImpCsvInfo.szSheetName), arrImpStct.GetAt(i).SheetName);	// [2009/07/08 Add for Error Message]<-----20111007

		// 「取引先、金融機関を自動で登録する」フラグセット
		pUcTbl->m_RegAutoRef = isRegAutoRef;

// 157766 add -->
		// 新旧ファイルの情報を親クラスから引き継ぐ
		pUcTbl->m_NewVer = imp->m_NewVer;
// 登録番号（法人番号）_23/11/21 add -->
		pUcTbl->m_NewVer2 = imp->m_NewVer2;
// 登録番号（法人番号）_23/11/21 add <--
		pUcTbl->m_KamokuVer = imp->m_KamokuVer;
		pUcTbl->m_KamokuVer2 = imp->m_KamokuVer2;			// 改良No.21-0086,21-0529 add
// 157766 add <--

		// 内訳書テーブルのオープン
// 210113_5301_1 add -->
		pUcTbl->m_IsExport = FALSE;
// 210113_5301_1 add <--
		nRet = pUcTbl->Init(pZmSub->m_database);
		if ( nRet != IMP_ERRCODE_SUCCESS ) {
			nErrorCode = IMP_ERRCODE_DB;
		}

		//「追加/上書き」処理の追加20111007----->
		// 内訳書テーブルの全レコード削除
		if( arrImpStct.GetAt(i).cmbTrans == TRANS_OVERWRITE ){
			nRet = pUcTbl->DeleteAllData();
			if ( nRet != IMP_ERRCODE_SUCCESS ) {
				nErrorCode = IMP_ERRCODE_DB;
			}
// midori 152713 del -->
			//// 全データ削除（保管データ削除）により保管テーブルのレコードも削除
			//CdbUcLstHokan mfcHokan(pZmSub->m_database);
			//mfcHokan.DeleteFormSeq(pUcTbl->GetFormSeq());
			//mfcHokan.Fin();
// midori 152713 del <--
		}
// 157781 del -->
		//else{
// 157781 del <--
// 157781 add -->
		// 項目数のエラーがなければ実行
		else if(fErrNumberOfItem != TRUE)	{
// 157781 add <--
			//改頁後の挿入
			//pUcTbl->ImpPageNew();

			//最大頁の最大行直後への挿入
			pUcTbl->ImpRowNew(pUcTbl->GetFormSeq(), pZmSub->m_database);
		}
		//「追加/上書き」処理の追加<-----20111007

// 157781 add -->
		// 項目数のエラーがある場合、データを削除したら次の帳票へスキップする
		if(fErrNumberOfItem == TRUE)	{
			continue;
		}
// 157781 add <--

		// エラー処理
		if ( nErrorCode != IMP_ERRCODE_SUCCESS ) {
			// プログレスバー終了
			icsPrgBar->EndProgress();
			// 確認メッセージを表示する
			(void)pUcTbl->ShowMessageBox(nErrorCode, m_pWnd);
			// CSV操作オブジェクトを閉じる
			csvUcFile.Close();
			// 各オブジェクトの解放
			if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
			if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
			if ( pUcTbl    != NULL ) { pUcTbl->Fin();    delete pUcTbl;    }	// テーブル操作用オブジェクトの解放
			if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
			// エラーを返す
			return IMP_ERRCODE_DB;
		}

		// CSV1行読込
		int nRowCount = 2;
		int regRowCount = 0;
		int regPageCount = 0;

		if ( pUcTbl->m_pUcCmn->GetNumPage() > 0 ) {
			regPageCount = pUcTbl->m_pUcCmn->GetNumPage() + 1;
		}
		else {
			regPageCount = 1;		// 帳表の初期表示でレコード無しなら１頁目挿入する対応
		}

// 157766 del -->
		//while(csvUcFile.CsvRead(&csaCsvLine) != FALSE)	{
// 157766 del <--
// 157766 add -->
		if(bTitleRead != FALSE)	{
			do	{
// 157766 add <--
// midori 150899 add -->
			// エラーマップ(１行分)の初期化
			pUcTbl->m_ErrMap.RemoveAll();
// midori 150899 add <--
			// エラーコードをクリア
			nErrorCode = IMP_ERRCODE_SUCCESS;
			// 行数を加算する
			nRowCount++;
			pUcTbl->m_uImpCsvInfo.nLine = nRowCount;
// midori 150966,150967,150969 del -->
//// midori 150898 add -->
//			strcpy_s(pUcTbl->m_uImpCsvInfo.szId, sizeof(pUcTbl->m_uImpCsvInfo.szId), csaCsvLine.GetAt(0));
//// midori 150898 add <--
// midori 150966,150967,150969 del <--
			// 項目行の読み飛ばし
			if(csaCsvLine.GetAt(0) == CSV_ID_ITEM)	{
				if(nRowCount == 3)	{		// ３行目なら読み飛ばし
// midori 170504 del -->
					//if( FormIndex - 1 == ID_FORMNO_021 ){
					//	if(( pUcTbl->virFormatedIsNoKamokuVer(&csaCsvLine) != FALSE )){
					//		pUcTbl->m_KamokuVer = TRUE;
					//	}
					//}
// midori 170504 del <--
// midori 170504 add -->
// 157766 del -->
//					if(pUcInfSub->m_FormSeq == ID_FORMNO_021)	{
//						if((pUcTbl->virFormatedIsNoKamokuVer(&csaCsvLine) != FALSE))	{
//							pUcTbl->m_KamokuVer = TRUE;
//						}
//					}
//// 210113_5301_1 add -->
//					else if(pUcInfSub->m_FormSeq == ID_FORMNO_042 || 
//							pUcInfSub->m_FormSeq == ID_FORMNO_111 || 
//							pUcInfSub->m_FormSeq == ID_FORMNO_131)	{
//						// 選択が簡素化様式の場合、新旧どちらのファイルもインポートできるようにする
//						if(bG_Kanso == TRUE)	{
//							pUcTbl->m_NewVer = pUcTbl->virFormatedIsNewVer(&csaCsvLine);
//						}
//						else	{
//							pUcTbl->m_NewVer = FALSE;
//						}
//					}
//// 210113_5301_1 add <--
					//else if(pUcInfSub->m_FormSeq >= ID_FORMNO_181 && pUcInfSub->m_FormSeq <= ID_FORMNO_1820)	{
// 157766 del <--
// 157766 add -->
					if(pUcInfSub->m_FormSeq >= ID_FORMNO_181 && pUcInfSub->m_FormSeq <= ID_FORMNO_1820)	{
// 157766 add <--
// midori 180404 del -->
						//// 項目タイトルから列番号を削除
						//cst = csaCsvLine.GetAt(2);
						//// 最後の文字が全角１の場合、全角１を取り除く
						//if(cst.Right(2).Compare(_T("１")) == 0)	cst.Delete(cst.GetLength()-2,2);
						//// 項目タイトルの文字数チェックを行う（項目タイトル＋列番号）
						//if (pUcTbl->CheckImportStr(cst, (MAX_KETA_181_ITEMTITLE)*2) != TRUE) {
						//	// プログレスバーを隠す
						//	icsPrgBar = HideProgress( icsPrgBar, m_pWnd, 0, SHEET_NUM, ID_IMPORT );
						//	// 確認メッセージを表示する
						//	int nRetValue = pUcTbl->ShowMessageBox(IMP_ERRCODE_IMPORT_ITEM, m_pWnd);
						//	// 確認メッセージの戻り値を判断する
						//	if ( nRetValue == IDYES ) {		// [はい] → 次の行へ
						//		// プログレスバーの再表示
						//		(void)ShowProgress( icsPrgBar, i );
						//		// csvの項目タイトル名を全角８文字まで項目タイトルエラー用変数にセット
						//		cst =_T("");
						//		GetSpritString(csaCsvLine.GetAt(2),&cst,(MAX_KETA_181_ITEMTITLE)*2);
						//		pUcTbl->m_CSV_181_ITEM_02 = cst;
						//		cst =_T("");
						//		GetSpritString(csaCsvLine.GetAt(3),&cst,(MAX_KETA_181_ITEMTITLE)*2);
						//		pUcTbl->m_CSV_181_ITEM_03 = cst ;
						//		continue;
						//	} else {						// [いいえ] → 処理中断
						//		// CSV操作オブジェクトを閉じる
						//		csvUcFile.Close();
						//		// 各オブジェクトの解放
						//		if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
						//		if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
						//		if ( pUcTbl    != NULL ) { pUcTbl->Fin();    delete pUcTbl;    }	// テーブル操作用オブジェクトの解放
						//		if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
						//		// エラーを返す
						//		return IMP_ERRCODE_IMPORT;
						//	}
						//}
						//// csvの項目タイトル名を項目タイトルエラー用変数にセット
						//pUcTbl->m_CSV_181_ITEM_02 = csaCsvLine.GetAt(2);
						//pUcTbl->m_CSV_181_ITEM_03 = csaCsvLine.GetAt(3);
						//// 項目タイトルを取り込む
						////	初期化成功？
						//if ( mfcRec.Init( pUcInfSub->m_FormSeq ) != DB_ERR_OK ){
						//	// プログレスバーを隠す
						//	icsPrgBar = HideProgress( icsPrgBar, m_pWnd, 0, SHEET_NUM, ID_IMPORT );
						//	// 確認メッセージを表示する
						//	int nRetValue = pUcTbl->ShowMessageBox(IMP_ERRCODE_IMPORT, m_pWnd);
						//	// 確認メッセージの戻り値を判断する
						//	if ( nRetValue == IDYES ) {		// [はい] → 次の行へ
						//		// プログレスバーの再表示
						//		(void)ShowProgress( icsPrgBar, i );
						//		continue;
						//	} else {						// [いいえ] → 処理中断
						//		// CSV操作オブジェクトを閉じる
						//		csvUcFile.Close();
						//		// 各オブジェクトの解放
						//		if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
						//		if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
						//		if ( pUcTbl    != NULL ) { pUcTbl->Fin();    delete pUcTbl;    }	// テーブル操作用オブジェクトの解放
						//		if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
						//		// エラーを返す
						//		return IMP_ERRCODE_IMPORT;
						//	}
						//}
// midori 180404 del <--
// midori 180404 add -->
						// 項目タイトルから列番号を削除
						cst = csaCsvLine.GetAt(2);
						// 最後の文字が全角１の場合、全角１を取り除く
						if(cst.Right(2).Compare(_T("１")) == 0)	cst.Delete(cst.GetLength()-2,2);

						// 項目タイトルの文字数チェックを行う（項目タイトル＋列番号）
						if (pUcTbl->CheckImportStr(cst, (MAX_KETA_181_ITEMTITLE)*2) != TRUE) {
							// プログレスバーを隠す
							icsPrgBar = HideProgress( icsPrgBar, m_pWnd, 0, SHEET_NUM, ID_IMPORT );
							// 確認メッセージを表示する
							// nRetValue:ICSMessageBoxの戻り値
							// ※項目エラーの場合、はい→IDNO、いいえ→IDCANCELを戻り値をして返す
							int nRetValue = pUcTbl->ShowMessageBox2(IMP_ERRCODE_IMPORT_ITEM, m_pWnd);
							// csvの項目タイトル名を全角８文字までデカット
							// 項目１
							cst =_T("");
							GetSpritString(csaCsvLine.GetAt(2),&cst,(MAX_KETA_181_ITEMTITLE)*2);
							csaCsvLine.SetAt(2,cst);
							// 項目２
							cst =_T("");
							GetSpritString(csaCsvLine.GetAt(3),&cst,(MAX_KETA_181_ITEMTITLE)*2);
							csaCsvLine.SetAt(3,cst);
							// 確認メッセージの戻り値を判断する
							if ( nRetValue == IDNO ) {				// [いいえ] → 次の帳票処理へ
								(void)ShowProgress( icsPrgBar, i );
								// csvの項目タイトル名を項目タイトルエラー用変数にセット
								pUcTbl->m_CSV_181_ITEM_02 = csaCsvLine.GetAt(2);
								pUcTbl->m_CSV_181_ITEM_03 = csaCsvLine.GetAt(3);
								continue;
							} else if( nRetValue == IDCANCEL ){		// [キャンセル] → 処理中断
								// CSV操作オブジェクトを閉じる
								csvUcFile.Close();
// midori UC_0011 add -->
								// 頁調整する
								CmnTblUpdatePageRowAllResetMain(pZmSub,pUcInfSub);
// midori UC_0011 add <--
								// 各オブジェクトの解放
								if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
								if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
								if ( pUcTbl    != NULL ) { pUcTbl->Fin();    delete pUcTbl;    }	// テーブル操作用オブジェクトの解放
								if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
								// エラーを返す
								return IMP_ERRCODE_IMPORT;
							}
// midori 153830 add ------------------>
							else	{								// [はい] → 文字数カット
								(void)ShowProgress( icsPrgBar, i );
							}
// midori 153830 add <------------------
							// 項目１のタイトル名を再度取得
							cst = csaCsvLine.GetAt(2);
						}
						// csvの項目タイトル名を項目タイトルエラー用変数にセット
						pUcTbl->m_CSV_181_ITEM_02 = csaCsvLine.GetAt(2);
						pUcTbl->m_CSV_181_ITEM_03 = csaCsvLine.GetAt(3);
						// 項目タイトルを取り込む
						//	初期化成功？
						if ( mfcRec.Init( pUcInfSub->m_FormSeq ) != DB_ERR_OK ){
							// プログレスバーを隠す
							icsPrgBar = HideProgress( icsPrgBar, m_pWnd, 0, SHEET_NUM, ID_IMPORT );
							// 確認メッセージを表示する
							pUcTbl->ShowMessageBox(IMP_ERRCODE_DB, m_pWnd);
							// CSV操作オブジェクトを閉じる
							csvUcFile.Close();
							// 各オブジェクトの解放
							if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
							if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
							if ( pUcTbl    != NULL ) { pUcTbl->Fin();    delete pUcTbl;    }	// テーブル操作用オブジェクトの解放
							if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
							// エラーを返す
							return IMP_ERRCODE_DB;
						}
// midori 180404 add <--
						//	レコードあり？
						if ( !mfcRec.IsEOF() ){
							//	先頭に移動
							mfcRec.MoveFirst();
							//	編集開始
							mfcRec.Edit();
							//	項目を設定
							mfcRec.m_ItemName = cst;
							//	更新
							mfcRec.Update(); 
						}
						//	閉じる
						mfcRec.Fin();
					}
// midori 170504 add <--
					continue;
				} else {					// ３行目以外ならデータエラー
					// プログレスバーを隠す
					icsPrgBar = HideProgress( icsPrgBar, m_pWnd, 0, SHEET_NUM, ID_IMPORT);
					// 確認メッセージを表示する
// midori 150898 add -->
					pUcTbl->m_uImpCsvInfo.nErr = 1;
// midori 150898 add <--
					int nRetValue = pUcTbl->ShowMessageBox(IMP_ERRCODE_IMPORT, m_pWnd);
					// 確認メッセージの戻り値を判断する
					if ( nRetValue == IDYES ) {		// [はい] → 次の帳票処理へ
						// プログレスバーの再表示
						(void)ShowProgress( icsPrgBar, i );
						continue;
					} else {						// [いいえ] → 処理中断
						// CSV操作オブジェクトを閉じる
						csvUcFile.Close();
// midori UC_0011 add -->
						// 頁調整する
						CmnTblUpdatePageRowAllResetMain(pZmSub,pUcInfSub);
// midori UC_0011 add <--
						// 各オブジェクトの解放
						if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
						if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
						if ( pUcTbl    != NULL ) { pUcTbl->Fin();    delete pUcTbl;    }	// テーブル操作用オブジェクトの解放
						if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
						// エラーを返す
						return IMP_ERRCODE_IMPORT;
					}
				}
			}
			// 欄外開始行の判断
			if ( csaCsvLine.GetAt(0) == CSV_ID_EXTRA ) { break; }

// 201224 del -->
//// midori 157044 add -->
//			// 小計行、または中計行のデータをクリアする
//			pUcTbl->CheckKeiStr(pZmSub->m_database,pUcInfSub->m_FormSeq,&csaCsvLine);
//// midori 157044 add <--
// 201224 del <--

			// データ登録を行う
			nRet = pUcTbl->SetCsvOneLineData(&csaCsvLine);

			if ( nRet != IMP_ERRCODE_SUCCESS ) {
				// プログレスバーを隠す
				icsPrgBar = HideProgress( icsPrgBar, m_pWnd, 0, SHEET_NUM, ID_IMPORT );
				// 確認メッセージを表示する
// midori 180404 del -->
//				int nRetValue = pUcTbl->ShowMessageBox(IMP_ERRCODE_IMPORT, m_pWnd);
//				// 確認メッセージの戻り値を判断する
//				if ( nRetValue == IDYES ) {		// [はい] → 次の帳票処理へ
//					// プログレスバーの再表示
//					(void)ShowProgress( icsPrgBar, i );
//					continue;
//				} else {						// [いいえ] → 処理中断
//					// CSV操作オブジェクトを閉じる
//					csvUcFile.Close();
//					// 各オブジェクトの解放
//					if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
//					if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
//					if ( pUcTbl    != NULL ) { pUcTbl->Fin();    delete pUcTbl;    }	// テーブル操作用オブジェクトの解放
//					if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
//					// エラーを返す
//					return IMP_ERRCODE_IMPORT;
//				}
// midori 180404 del <--
// midori 180404 add -->
				// nRetValue:ICSMessageBoxの戻り値
				// ※項目エラーの場合、はい→IDNO、いいえ→IDCANCELを戻り値をして返す
				int nRetValue = pUcTbl->ShowMessageBox2(IMP_ERRCODE_IMPORT, m_pWnd);
				// 確認メッセージの戻り値を判断する
				if ( nRetValue == IDNO ) {				// [いいえ] → 次の帳票処理へ
					// プログレスバーの再表示
					(void)ShowProgress( icsPrgBar, i );
					continue;
				} else if( nRetValue == IDCANCEL ){		// [キャンセル] → 処理中断
// midori UC_0011 add -->
					// 頁調整する
					CmnTblUpdatePageRowAllResetMain(pZmSub,pUcInfSub);
// midori UC_0011 add <--
					// CSV操作オブジェクトを閉じる
					csvUcFile.Close();
					// 各オブジェクトの解放
					if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
					if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
					if ( pUcTbl    != NULL ) { pUcTbl->Fin();    delete pUcTbl;    }	// テーブル操作用オブジェクトの解放
					if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
					// エラーを返す
					return IMP_ERRCODE_IMPORT;
				}
// midori 180404 add <--
// midori 153830 add ------------------>
				else	{								// [はい] → 文字数カット
					(void)ShowProgress( icsPrgBar, i );
				}
// midori 153830 add <------------------
			}
// midori 180404 add -->
			if(pUcTbl->ImpCsvOneLineData(&csaCsvLine) != IMP_ERRCODE_SUCCESS)	{
				ICSMessageBox(_T("データのインポートに失敗しました。"),MB_ICONSTOP);
				// CSV操作オブジェクトを閉じる
				csvUcFile.Close();
				// 各オブジェクトの解放
				if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
				if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
				if ( pUcTbl    != NULL ) { pUcTbl->Fin();    delete pUcTbl;    }	// テーブル操作用オブジェクトの解放
				if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
				// エラーを返す
				return IMP_ERRCODE_IMPORT;
			}
// midori 180404 add <--

			// [科目行]をインポートしたかどうかのサインを設定
// 改良No.21-0086,21-0529 cor -->
//// midori 156935 add -->
//			if(pUcInfSub->m_FormSeq == ID_FORMNO_021)	{	// ②：受取手形の内訳書
//				// 現在処理中の行属性が [科目] かどうか
//				if(pUcTbl->GetOneLineFgFunc() == ID_FGFUNC_KAMOKU)	{
//					csvKamokuData[0] = TRUE;
//				}
//			}
//			if(pUcInfSub->m_FormSeq == ID_FORMNO_111)	{	// ⑪：借入金及び支払利子の内訳書
//				// 現在処理中の行属性が [科目] かどうか
//				if(pUcTbl->GetOneLineFgFunc() == ID_FGFUNC_KAMOKU)	{
//					csvKamokuData[1] = TRUE;
//				}
//			}
//// midori 156935 add <--
// ------------------------------
			if(pUcInfSub->m_FormSeq == ID_FORMNO_021)	{	// ②：受取手形の内訳書
				// 現在処理中の行属性が [科目] かどうか
				if(pUcTbl->GetOneLineFgFunc() == ID_FGFUNC_KAMOKU)	{
					csvKamokuData[0] = TRUE;
				}
			}
			if(pUcInfSub->m_FormSeq == ID_FORMNO_081)	{	// ⑧：支払手形の内訳書
				if(bG_InvNo == TRUE)	{
					// 現在処理中の行属性が [科目] かどうか
					if(pUcTbl->GetOneLineFgFunc() == ID_FGFUNC_KAMOKU) {
						csvKamokuData[1] = TRUE;
					}
				}
			}
			if(pUcInfSub->m_FormSeq == ID_FORMNO_111)	{	// ⑪：借入金及び支払利子の内訳書
				// 現在処理中の行属性が [科目] かどうか
				if(pUcTbl->GetOneLineFgFunc() == ID_FGFUNC_KAMOKU)	{
					csvKamokuData[2] = TRUE;
				}
			}
// 改良No.21-0086,21-0529 cor <--

			//正常に登録されたら行数をインクリメント 20111007
			if (regRowCount > pUcTbl->GetRowMaxData()) {
				regPageCount = regPageCount + 1;
				regRowCount = 1;
			}
			else{
				regRowCount = regRowCount + 1;
			}
// 157766 del -->
		//}
// 157766 del <--
// 157766 add -->
			}while(csvUcFile.CsvRead(&csaCsvLine) != FALSE);
		}
// 157766 add <--
		regRowCount = regRowCount + 1;

// midori 170504 del -->
//// midori 150905 add -->
//		// ⑭－２：人件費の内訳書
//		// CSVでインポートしなかった行のデータをテーブルに登録する
//		if((j + 1) == ID_FORMNO_142)	{
//			pUcTbl->SetCsvNullLineData142();
//		}
//// midori 150905 add <--
// midori 170504 del <--
// midori 170504 add -->
		// ⑭－２：人件費の内訳書
		// CSVでインポートしなかった行のデータをテーブルに登録する
		if(pUcInfSub->m_FormSeq == ID_FORMNO_142)	{
			pUcTbl->SetCsvNullLineData142();
		}
// midori 170504 add <--
		
		//改頁処理の場合、データをインポートし終えたら、空行を挿入	20111007
		//if( arrImpStct.GetAt(i).cmbTrans == TRANS_ADD ){
				////一頁分の空行を挿入
				//pUcTbl->TableInsertNullLine( regPageCount, regRowCount, pUcTbl->GetRowMax() - regRowCount);
				//
				////計設定が「頁計と累計」の場合でかつ３ページを超える場合、インポートデータが一頁前の最終行にずれ込んでしまうため、
				////特殊行を空行にして成型する
				//if(( pUcTbl->m_pUcInfSub->m_OutKei == ID_OUTKEI_BOTH ) && ( regPageCount > 2 )){
				//	//BOOL ii = pUcTbl->m_pUcCmn->Requery();
				//	pUcTbl->m_pUcCmn->virSpecialLineToNull(regPageCount-1, pUcTbl->GetRowMaxData());
				//}

				////頁計が「なし」でかつ計設定が「累計」の場合、インポートデータが一頁前の最終行にずれ込んでしまうため、
				////特殊行を空行にして成型する
				//if(( pUcTbl->GetRowKei() == 0 ) && ( pUcTbl->m_pUcInfSub->m_OutKei == 2 )){
				//	pUcTbl->m_pUcCmn->virSpecialLineToNull(regPageCount-1, pUcTbl->GetRowMax());
				//}
			//pUcTbl->m_pUcCmn->DeleteFgFunc(ID_FGFUNC_NULL);					// 空行の削除
			//pUcTbl->m_pUcCmn->DeleteFgFunc(ID_FGFUNC_PAGEKEI);				// 頁計の削除
			//pUcTbl->m_pUcCmn->DeleteFgFunc(ID_FGFUNC_RUIKEI);				// 累計の削除
		//}
		
		// 内訳書テーブルの終了
		nRet = pUcTbl->Fin();
		if ( nErrorCode != IMP_ERRCODE_SUCCESS ) {
			// プログレスバー終了
			icsPrgBar->EndProgress();
			// 確認メッセージを表示する
			int nRetValue = imp->ShowMessageBox(IMP_ERRCODE_DB, m_pWnd);
			// CSV操作オブジェクトを閉じる
			csvUcFile.Close();
			// 各オブジェクトの解放
			if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
			if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
			if ( pUcTbl    != NULL ) { pUcTbl->Fin();    delete pUcTbl;    }	// テーブル操作用オブジェクトの解放
			if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
			// エラーを返す
			return IMP_ERRCODE_DB;
		}

		// テーブル操作用オブジェクトを解放する
		if ( pUcTbl != NULL ) { delete pUcTbl; }

		//##################
		//### Ext. Block ###
		//##################
		// 欄外登録の処理
		csaCsvLine.RemoveAll();
// midori 170504 del -->
//		int FormNo = j + 1;
// midori 170504 del <--
// midori 170504 add -->
		int FormNo = pUcInfSub->m_FormSeq;
// midori 170504 add <--
		if ( (FormNo == ID_FORMNO_051) || (FormNo == ID_FORMNO_091) ) {
			switch ( FormNo ) {
				case ID_FORMNO_051 :	// ⑤棚卸資産の内訳書[下欄]
					pUcTbl = (CimpUc000Common*)(new CimpUc052Tanaoroshi2());
//					csaCsvLine.SetSize(CSV_052_ITEM_CNT);						// [2009/07/10 Delete]
					break;
				case ID_FORMNO_091 :	// ⑨買掛金の内訳書[下欄]
					pUcTbl = (CimpUc000Common*)(new CimpUc092Kaikakekin2());
//					csaCsvLine.SetSize(CSV_092_ITEM_CNT);						// [2009/07/10 Delete]
					break;
			}

			// 内訳書テーブルのオープン
			nRet = pUcTbl->Init(pZmSub->m_database);
			if ( nRet != IMP_ERRCODE_SUCCESS ) {
				nErrorCode = IMP_ERRCODE_DB;
			}
			// 内訳書テーブルの全レコード削除	欄外登録の為保管レコードは削除しない
			nRet = pUcTbl->DeleteAllData();
			if ( nRet != IMP_ERRCODE_SUCCESS ) {
				nErrorCode = IMP_ERRCODE_DB;
			}

			// 確認メッセージを表示する
			_tcscpy_s (pUcTbl->m_uImpCsvInfo.szFileName , sizeof(pUcTbl->m_uImpCsvInfo.szFileName) , arrImpStct.GetAt(i).FileName );	//<-----20111007
			_tcscpy_s (pUcTbl->m_uImpCsvInfo.szSheetName, sizeof(pUcTbl->m_uImpCsvInfo.szSheetName), arrImpStct.GetAt(i).SheetName );	// [2009/07/08 Add for Error Messaage]  <-----20111007

			// エラー処理
			if ( nErrorCode != IMP_ERRCODE_SUCCESS ) {
				// プログレスバー終了
				icsPrgBar->EndProgress();
				// 確認メッセージを表示する
				(void)pUcTbl->ShowMessageBox(nErrorCode, m_pWnd);
				// CSV操作オブジェクトを閉じる
				csvUcFile.Close();
				// 各オブジェクトの解放
				if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
				if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
				if ( pUcTbl    != NULL ) { pUcTbl->Fin();    delete pUcTbl;    }	// テーブル操作用オブジェクトの解放
				if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
				// エラーを返す
				return IMP_ERRCODE_DB;
			}

			// CSVファイル1行読込とインポート処理
			while ( csvUcFile.CsvRead(&csaCsvLine) != FALSE ) {
// midori 150899 add -->
				// エラーマップ(１行分)の初期化
				pUcTbl->m_ErrMap.RemoveAll();
// midori 150899 add <--

				// エラーコードをクリア
				nErrorCode = IMP_ERRCODE_SUCCESS;

				// 行数を加算する
				nRowCount++;
				pUcTbl->m_uImpCsvInfo.nLine = nRowCount;

// midori 150966,150967,150969 del -->
//// midori 150898 add -->
//				strcpy_s(pUcTbl->m_uImpCsvInfo.szId, sizeof(pUcTbl->m_uImpCsvInfo.szId), csaCsvLine.GetAt(0));
//// midori 150898 add <--
// midori 150966,150967,150969 del <--

// midori 180404 del -->
				//// データ登録を行う
				//nRet = pUcTbl->SetCsvOneLineData(&csaCsvLine);
				//if ( nRet != IMP_ERRCODE_SUCCESS ) {
				//	// プログレスバーを隠す
				//	icsPrgBar = HideProgress( icsPrgBar, m_pWnd, 0, SHEET_NUM, ID_IMPORT);
				//	// 確認メッセージを表示する
				//	int nRetValue = pUcTbl->ShowMessageBox(IMP_ERRCODE_IMPORT, m_pWnd);
				//	// 確認メッセージの戻り値を判断する
				//	if ( nRetValue == IDYES ) {		// [はい] → 次の帳票処理へ
				//		// プログレスバーの再表示
				//		(void)ShowProgress( icsPrgBar, i );
				//		continue;
				//	} else {						// [いいえ] → 処理中断
				//		// CSV操作オブジェクトを閉じる
				//		csvUcFile.Close();
				//		// 各オブジェクトの解放
				//		if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
				//		if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
				//		if ( pUcTbl    != NULL ) { pUcTbl->Fin();    delete pUcTbl;    }	// テーブル操作用オブジェクトの解放
				//		if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
				//		// エラーを返す
				//		return IMP_ERRCODE_IMPORT;
				//	}
				//}
// midori 180404 del <--
// midori 180404 add -->
				// データ登録を行う
				nRet = pUcTbl->SetCsvOneLineData(&csaCsvLine);
				if ( nRet != IMP_ERRCODE_SUCCESS ) {
					// プログレスバーを隠す
					icsPrgBar = HideProgress( icsPrgBar, m_pWnd, 0, SHEET_NUM, ID_IMPORT);
					// 確認メッセージを表示する
					// nRetValue:ICSMessageBoxの戻り値
					// ※項目エラーの場合、はい→IDNO、いいえ→IDCANCELを戻り値をして返す
					int nRetValue = pUcTbl->ShowMessageBox2(IMP_ERRCODE_IMPORT, m_pWnd);
					// 確認メッセージの戻り値を判断する
					if ( nRetValue == IDNO ) {				// [いいえ] → 次の帳票処理へ
						// プログレスバーの再表示
						(void)ShowProgress( icsPrgBar, i );
						continue;
					} else if( nRetValue == IDCANCEL ){		// [キャンセル] → 処理中断
// midori UC_0011 add -->
						// 頁調整する
						CmnTblUpdatePageRowAllResetMain(pZmSub,pUcInfSub);
// midori UC_0011 add <--
						// CSV操作オブジェクトを閉じる
						csvUcFile.Close();
						// 各オブジェクトの解放
						if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
						if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
						if ( pUcTbl    != NULL ) { pUcTbl->Fin();    delete pUcTbl;    }	// テーブル操作用オブジェクトの解放
						if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
						// エラーを返す
						return IMP_ERRCODE_IMPORT;
					}
// midori 153830 add ------------------>
					else	{								// [はい] → 文字数カット
						(void)ShowProgress( icsPrgBar, i );
					}
// midori 153830 add <------------------
				}
				if(pUcTbl->ImpCsvOneLineData(&csaCsvLine) != IMP_ERRCODE_SUCCESS)	{
					ICSMessageBox(_T("データのインポートに失敗しました。"),MB_ICONSTOP);
					// CSV操作オブジェクトを閉じる
					csvUcFile.Close();
					// 各オブジェクトの解放
					if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
					if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
					if ( pUcTbl    != NULL ) { pUcTbl->Fin();    delete pUcTbl;    }	// テーブル操作用オブジェクトの解放
					if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
					// エラーを返す
					return IMP_ERRCODE_IMPORT;
				}
// midori 180404 add <--
			}

// midori 150954 add -->
// ⑨買掛金の内訳書（欄外）
			// CSVでインポートしなかった行のデータをテーブルに登録する
			if(FormNo == ID_FORMNO_091)	{
				if(pUcTbl->SetCsvNullLineData091() != DB_ERR_OK)	{
					// エラーを返す
					return IMP_ERRCODE_DB;
				}
			}
// midori 150954 add <--

			// 内訳書テーブルの終了
			nRet = pUcTbl->Fin();
			if ( nErrorCode != IMP_ERRCODE_SUCCESS ) {
				// プログレスバー終了
				icsPrgBar->EndProgress();
				// エラーメッセージ表示
				(void)imp->ShowMessageBox(IMP_ERRCODE_DB, m_pWnd);
				// CSV操作オブジェクトを閉じる
				csvUcFile.Close();
				// 各オブジェクトの解放
				if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
				if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
				if ( pUcTbl    != NULL ) { pUcTbl->Fin();    delete pUcTbl;    }	// テーブル操作用オブジェクトの解放
				if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
				// エラーを返す
				return IMP_ERRCODE_DB;
			}

			// テーブル操作用オブジェクトを解放する
			if ( pUcTbl != NULL ) { delete pUcTbl; }
		}

		// CSV操作オブジェクトを閉じる
		csvUcFile.Close();
		// プログレスバーの再表示
		(void)UpdateProgress( icsPrgBar, i );
// midori 170502 add -->
		// 頁調整する
		CmnTblUpdatePageRowAllResetMain(pZmSub,pUcInfSub);
// midori 170502 add <--
	}

// midori 156935 add -->
	// 科目行がインポートされた時、編集－ソートダイアログの「科目行を挿入する」のチェックをＯＮにする
// 改良No.21-0086,21-0529 cor -->
	//for(ii=0;ii<2;ii++) {
	//	if(csvKamokuData[ii] == TRUE) {
	//		// ②：受取手形の内訳書
	//		if(ii==0)			nFormSeq = ID_FORMNO_021;
	//		// ⑪：借入金及び支払利子の内訳書
	//		else				nFormSeq = ID_FORMNO_111;
// ------------------------------
	for(ii=0;ii<3;ii++) {
		if(csvKamokuData[ii] == TRUE) {
			// ②：受取手形の内訳書
			if(ii==0)			nFormSeq = ID_FORMNO_021;
			// ⑧：支払手形の内訳書
			else if(ii==1)		nFormSeq = ID_FORMNO_081;
			// ⑪：借入金及び支払利子の内訳書
			else				nFormSeq = ID_FORMNO_111;
// 改良No.21-0086,21-0529 cor <--

			int nRet = pUcInfSub->RequeryFormSeq(nFormSeq);
			if ( nRet == DB_ERR_OK ) {
				// サインを立てる
				pUcInfSub->Edit();
				CCtrlSubGeneralVar cs(*pUcInfSub);
// midori 157099,157119 del -->
//// midori 157042 del -->
//				//cs.bKamokuTitle = TRUE;
//// midori 157042 del <--
//// midori 157042 add -->
//				cs.bOutKamokuRow = TRUE;
//// midori 157042 add <--
// midori 157099,157119 del <--
// midori 157099,157119 add -->
				cs.bKamokuTitle = TRUE;
// midori 157099,157119 add <--
				cs.Set(*pUcInfSub);
				pUcInfSub->Update();
			}
		}
	}
// midori 156935 add <--

	// プログレスバー終了
	icsPrgBar->EndProgress();
	// 確認メッセージ表示
	int nRetValue = imp->ShowMessageBox(IMP_ERRCODE_SUCCESS, m_pWnd);
	// 各オブジェクトの解放
	if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトを解放する
	if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
	if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放

	return nErrorCode;
}

//**********************************************************************
// データエクスポート
//		・本関数の呼び出し方法により、動作が異なる。
//		　　(1) フォルダの指定、および実行するかどうか確認するダイアログが表示される。
//		　　(2) 引数で指定されたフォルダに対し、エクスポート処理を開始する。
//		・エクスポート先のフォルダがない場合は作成する。
//		・同名のCSVファイルがある場合は上書きする。
// 
//	引数
//			pZmSub					[IN] 財務データベースへのポインタ
//			strFolderPath			[IN] インポートデータがあるフォルダのパス
//			pParent					[IN] 親ウィンドウのポインタ
// 
//	戻値
//			エラーコード（DBUchiImpDEF.h参照）
//**********************************************************************
// (1) フォルダの指定、および実行するかどうか確認するダイアログが表示される。
int CDBUchiImp::Export(CDBZmSub* pZmSub, CWnd* pParent /*= NULL*/)
{
	HINSTANCE		m_hDefInstance;					//<-----20111007呼び出し側のリソースハンドル
	m_hDefInstance = AfxGetResourceHandle();		
	AfxSetResourceHandle( g_hInstance );			//DLL側のリソースハンドルセット

	CString		strFolderPath		= _T("");		// フォルダパス
	CdlgImport	dlg(pParent);						// データエクスポートダイアログ

// 24/01/16_ScoreLink add -->
	char	bf[MAX_PATH]={0};
	UCOM_GetString(_T("SCORELINK依頼ID"),bf);
	if(strlen(bf) > 0)	{
		memset(bf,0,sizeof(bf));
		PP_GetAppData(0x100,NULL,NULL,"BPR_PDFPTH",bf);
		strFolderPath.Format(_T("%s"),bf);
	}
	else	{
// 24/01/16_ScoreLink add <--
		//--------------------------------------------------
		// INIファイルからの指定フォルダ取得
		//--------------------------------------------------
		strFolderPath = GetFolderPath(FALSE);

		// データエクスポートダイアログの表示
		//（データエクスポート先フォルダのパスを取得）
		if (dlg.ShowDialog(FALSE, &strFolderPath) != ID_DLG_OK) {
			//リソースハンドルをexeに戻す
			AfxSetResourceHandle( m_hDefInstance );

			// [キャンセル]ボタン押下
			return IMP_DIALOG_ERRCODE_CANCEL;
		}

		// 保存先のドライブ指定チェック									// [2009/07/27 Add] [2009/09/04 Fix for Network path]
		if ( strFolderPath.Find("\\\\") != 0 ) {							// ネットワークパス？ [2009/09/04 Add]
			if ( strFolderPath.Find(':') != 1 ) {							// ドライブ指定無し？
				strFolderPath = GetIcswinDrive() + "\\" + strFolderPath;	// ドライブ指定が無ければICSWinのあるドライブを指定
			}
		}
// 24/01/16_ScoreLink add -->
	}
// 24/01/16_ScoreLink add <--

	// エクスポート処理
	int nRetValue = Export(pZmSub, strFolderPath, pParent);

	//リソースハンドルをexeに戻す
	AfxSetResourceHandle( m_hDefInstance );

	switch (nRetValue) {
// midori 160603 del -->
//	case	IMP_ERRCODE_FINDFOLDER:			// フォルダがない
// midori 160603 del <--
	case	IMP_ERRCODE_CREATEDFOLDER:		// ファイル作成失敗
	case	IMP_DIALOG_ERRCODE_CANCEL:
		// 再構成不要なので CANCEL を返す
		return IMP_DIALOG_ERRCODE_CANCEL;
	default :
		// 再構成が必要なので OK を返す
		return IMP_DIALOG_ERRCODE_OK;
	}
}
// (2) 引数で指定されたフォルダに対し、エクスポート処理を開始する。
int CDBUchiImp::Export(CDBZmSub* pZmSub, CString strFolderPath, CWnd* pParent /*= NULL*/)
{
	int			nErrorCode			= IMP_ERRCODE_SUCCESS;	// エラーコード

	CString					csCompanyName;					// 会社名用バッファ
	CString					csSheetName;					// 帳票名用バッファ
// midori 170504 add -->
	CString					csv_181_item_02=_T("");
	CString					csv_181_item_03=_T("");
// midori 170504 add <--
	
	CCsv					csvUcFile;						// CSVファイルクラス
	CString					csCsvFilename;					// CSVファイル名(フルパス用)
	CdlgImpOpt				dlgOpt(pParent);				//データインポートオプションダイアログ<-----20111007
	CArray<ImpOpt, ImpOpt&> arrImpStct;						//<-----20111007
							arrImpStct.RemoveAll();
// midori 170504 add -->
	CdbUcInfSonotaItem		mfcRec( pZmSub->m_database );	//	uc_inf_sonota_itemテーブルクラス
// midori 170504 add <--

// midori 190301 0415 add -->
	CdbUcInfMain* ucInfMain = new CdbUcInfMain(pZmSub->m_database);
	ucInfMain->Init();
	CCtrlMainGeneralVar	cm(*ucInfMain);
	if(cm.bUseStyle)	bG_Kanso = TRUE;
	else				bG_Kanso = FALSE;
// 登録番号（法人番号）_23/11/21 add -->
	if(cm.bUseStyle2)	bG_InvNo = TRUE;
	else				bG_InvNo = FALSE;
// 登録番号（法人番号）_23/11/21 add <--
	ucInfMain->Fin();
	delete ucInfMain;
	ucInfMain = NULL;
// midori 190301 0415 add <--

	// 親ウィンドウのポインタを保持
	m_pWnd		= pParent;

	// 帳票名取得の為に uc_inf_sub テーブル準備
	CdbUcInfSub* pUcInfSub;
	pUcInfSub = new CdbUcInfSub(pZmSub->m_database);
	int FormIndex = 1;

	int bitSheet[SHEET_NUM];
	int base = 0x01;

	for(int i=0; i<SHEET_NUM; i++){
		// 配列に2^nの値をセットする ex)D0→bitSheet[0] = 1 D1→bitSheet[1] = 2...
// midori 170504 del -->
		//if( i == 0 ){
		//	bitSheet[i] = base;
		//}
		//else{
		//	bitSheet[i] = base = base << 1;
		//}
// midori 170504 del <--
// midori 170504 add -->
		// i=22:様式⑰－１
		if(i == 22 || i == 42)				base = 0x01;
		if(i == 0 || i == 22 || i == 42)	bitSheet[i] = base;
		else								bitSheet[i] = base = base << 1;
// midori 170504 add <--

		int nRet = pUcInfSub->Init(i+1);
		if( nRet = 0 ){
			return IMP_DIALOG_ERRCODE_CANCEL;
		}

		ImpOpt	datImp;
		datImp.SheetNo		= UcSheetNumber[i];		// 様式番号
		datImp.SheetName	= pUcInfSub->m_Title;	// 様式名称
		datImp.FileName		= UcCsvFile[i];			// ファイル名
		datImp.bitTarget	= bitSheet[i];			// D?ビット
		arrImpStct.Add( datImp );
	}

	// エクスポートオプションダイアログの表示
	if ( dlgOpt.ShowDialog( pZmSub, FALSE, arrImpStct ) != ID_DLG_OK ) {	//エクスポートオプションダイアログ表示<-----20111007
		// [キャンセル]ボタン押下
		return IMP_DIALOG_ERRCODE_CANCEL;
	}

	// プログレスバーの準備
	ICSProgressWnd* icsPrgBar;
	icsPrgBar = InitProgress( m_pWnd, 0, SHEET_NUM, ID_EXPORT );

	// メッセージ処理用
	CimpUc000Common* imp = new CimpUc000Common();
	imp->m_uImpCsvInfo.fImport = FALSE;
	_tcscpy_s (imp->m_uImpCsvInfo.szFolderPath, sizeof(imp->m_uImpCsvInfo.szFolderPath), strFolderPath);

	// 指定フォルダ有無確認
	if ( PathIsDirectory(strFolderPath) == FALSE ){
		// フォルダが見つからない場合は、フォルダを作成する
		if (CreateDirectory(strFolderPath, NULL) != TRUE) {
			// 確認メッセージを表示する
			(void)imp->ShowMessageBox(IMP_ERRCODE_CREATEDFOLDER, m_pWnd);
			// メッセージ用オブジェクトを解放する
			if ( imp != NULL ) { imp->Fin(); delete imp; }
			// エラーを返す
			return IMP_ERRCODE_CREATEDFOLDER;
		}
	}
	// 指定フォルダ保存
// 24/01/16_ScoreLink cor -->
	//SaveFolderPath(FALSE, strFolderPath);
// --------------------------
	// ScoreLinkからの起動時は保存しない
	char	scid[MAX_PATH]={0};
	UCOM_GetString(_T("SCORELINK依頼ID"),scid);
// 24/02/19_ScoreLink add -->
	bG_Sl = FALSE;
// 24/02/19_ScoreLink add <--
	if(strlen(scid) == 0)	{
		SaveFolderPath(FALSE, strFolderPath);
	}
// 24/02/19_ScoreLink add -->
	else	{
		bG_Sl = TRUE;
	}
// 24/02/19_ScoreLink add <--
// 24/01/16_ScoreLink cor <--

	// 会社名の準備
	// Zvolumeテーブルオープン
	if ( pZmSub->VolumeOpen() != 0 ) {					
		nErrorCode = IMP_ERRCODE_DB;
	}
	if ( pZmSub->zvol == NULL ) {
		nErrorCode = IMP_ERRCODE_DB;
	}
	// エラー処理
	if ( nErrorCode != IMP_ERRCODE_SUCCESS ) {
		// 確認メッセージを表示する(DBエラーの場合、終了)
		(void)imp->ShowMessageBox(nErrorCode, m_pWnd);
		// メッセージ用オブジェクトを解放する
		if ( imp != NULL ) { imp->Fin(); delete imp; }
		// エラーを返す
		return IMP_ERRCODE_DB;
	}
	// 会社名取得
	CUcFunctionCommon* pUcFuncCmn;
	pUcFuncCmn = new CUcFunctionCommon();
	csCompanyName = pUcFuncCmn->DeleteRightSpace( pZmSub->zvol->c_nam );	// 会社名(末尾空白削除)
	delete pUcFuncCmn;

// midori 152109 add -->
	pZmSub->VolumeClose();
// midori 152109 add <--

	// 帳票名取得の為に uc_inf_sub テーブル準備
	//CdbUcInfSub* pUcInfSub;
	//pUcInfSub = new CdbUcInfSub(pZmSub->m_database);
	//int FormIndex = 1;						// uc_inf_sub テーブル用の帳票番号

	// エクスポート先のドライブ取得			// [2009/07/28 Add]
	int iPos = strFolderPath.Find(':');					// ドライブのパス
	if ( iPos == -1 ) {									// ネットワークのパス
		iPos = strFolderPath.Find("\\", 2);
		iPos = strFolderPath.Find("\\", iPos + 1);
	}
	CString csExpDrv = strFolderPath.Left(iPos + 1);

	// プログレスバーの表示開始
	(void)ShowProgress( icsPrgBar, 0 );

	for ( int i = 0; i < SHEET_NUM; i++ ) {
		// 出力状態にチェックがはいっていない場合はスルー
		if( arrImpStct.GetAt(i).chkState == 0 ){
			FormIndex++;
			continue;
		}

		// エラーコードをクリア
		nErrorCode = IMP_ERRCODE_SUCCESS;

		// CSV情報の初期化をする
		ZeroMemory(&imp->m_uImpCsvInfo, sizeof(imp->m_uImpCsvInfo));
		imp->m_uImpCsvInfo.fImport = FALSE;
		// ファイル名/帳票名を設定
		//_tcscpy_s (imp->m_uImpCsvInfo.szFileName , sizeof(imp->m_uImpCsvInfo.szFileName) , UcCsvFile[i]  );
		//_tcscpy_s (imp->m_uImpCsvInfo.szSheetName, sizeof(imp->m_uImpCsvInfo.szSheetName), UcSheetName[i]);		// [2009/07/08 Add for Error Message]
		_tcscpy_s (imp->m_uImpCsvInfo.szFileName , sizeof(imp->m_uImpCsvInfo.szFileName) , arrImpStct.GetAt(i).FileName  );
		_tcscpy_s (imp->m_uImpCsvInfo.szSheetName, sizeof(imp->m_uImpCsvInfo.szSheetName), arrImpStct.GetAt(i).SheetName );

		// エクスポート先ドライブの空き容量チェック			// [2009/07/28 Add for ICSWin Drive]
		ULARGE_INTEGER i64Total;	// 全体容量
		ULARGE_INTEGER i64Use;		// 使用容量
		ULARGE_INTEGER i64Free;		// 空き容量
		int nRet = TRUE;
		if ( GetDiskFreeSpaceEx( csExpDrv, &i64Use, &i64Total, &i64Free ) ) {
			if ( i64Free.QuadPart <= 200*1024 ) {	// 空き容量が200KB以下
				nRet = FALSE;
			}
		} else {									// 指定ドライブが無い
			nRet = FALSE;
		}
		// エラー処理
		if ( nRet == FALSE ) {
			// プログレスバーを隠す
			icsPrgBar = HideProgress( icsPrgBar, m_pWnd, 0, SHEET_NUM, ID_EXPORT );
			// エラーメッセージ表示
			(void)imp->ShowMessageBox(IMP_ERRCODE_DRIVE_EMPTY, m_pWnd);
			// 各オブジェクトの解放
			if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
			if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
			// エラーを返す
			return IMP_ERRCODE_DRIVE_EMPTY;
		}
		
		// 内訳書CSVオープン
		csCsvFilename = strFolderPath + _T("\\") + UcCsvFile[i];
		nRet = csvUcFile.CsvOpen(csCsvFilename, CSVCREATE );
		// エラー処理
		if ( nRet == FALSE ) {
			// プログレスバーを隠す
			icsPrgBar = HideProgress( icsPrgBar, m_pWnd, 0, SHEET_NUM, ID_EXPORT );
			// 確認メッセージ表示
			int nRetValue = imp->ShowMessageBox(IMP_ERRCODE_CSVOPEN, m_pWnd);
			// 確認メッセージの戻り値を判断する
			if ( nRetValue == IDYES ) {		// [はい] → 次の帳票処理へ
				// プログレスバーの再表示
				(void)ShowProgress( icsPrgBar, i );
				continue;
			} else {						// [いいえ] → 処理中断
				// 各オブジェクトの解放
				if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
				if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
				if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
				// エラーを返す
				return IMP_ERRCODE_CSVOPEN;
			}
		}

		//###################
		//### Info. Block ###
		//###################
		CStringArray csaInfo;		// CSV書込用バッファ

		//+++++++++++++++++++++
		// 1行目：帳票名の書込
		//+++++++++++++++++++++
		// 対象テーブルの初期化
		if ( pUcInfSub->Init(FormIndex++) != DB_ERR_OK ) {
			// プログレスバーを隠す
			icsPrgBar = HideProgress( icsPrgBar, m_pWnd, 0, SHEET_NUM, ID_EXPORT );
			// エラーメッセージを表示
			(void)imp->ShowMessageBox(IMP_ERRCODE_DB, m_pWnd);
			// CSV操作オブジェクトを閉じる
			csvUcFile.Close();
			// 各オブジェクトの解放
			if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
			if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
			if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
			// エラーを返す
			return IMP_ERRCODE_DB;
		}
		// 書込情報の準備
		csSheetName = pUcInfSub->m_Title;				// 帳票名取得
		csaInfo.SetSize(CSV_INF_ITEM_CNT);				// Info. Block項目数
		csaInfo.SetAt(0, CSV_INF_ITEM_01);				// 識別子
		csaInfo.SetAt(1, csSheetName);					// 帳票名
		// CSVファイルへの書込
		nRet = csvUcFile.CsvWrite(&csaInfo);
		// エラー処理
		if ( nRet == FALSE ) {
			// プログレスバーを隠す
			icsPrgBar = HideProgress( icsPrgBar, m_pWnd, 0, SHEET_NUM, ID_EXPORT );
			// 確認メッセージを表示する
			int nRetValue = imp->ShowMessageBox(IMP_ERRCODE_EXPORT, m_pWnd);
			// CSV操作オブジェクトを閉じる
			csvUcFile.Close();
			// 確認メッセージの戻り値を判断する
			if ( nRetValue == IDYES ) {		// [はい] → 次の帳票処理へ
				// プログレスバーの再表示
				(void)ShowProgress( icsPrgBar, i );
				continue;
			} else {						// [いいえ] → 処理中断
				// 各オブジェクトの解放
				if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
				if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
				if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
				// エラーを返す
				return IMP_ERRCODE_EXPORT;
			}
		}

		//+++++++++++++++++++++
		// 2行目：会社名の書込
		//+++++++++++++++++++++
		// 書込情報の準備
		csaInfo.SetSize(CSV_INF_ITEM_CNT);				// Info. Block項目数
		csaInfo.SetAt(0, CSV_INF_ITEM_02);				// 識別子
		csaInfo.SetAt(1, csCompanyName);				// 会社名
		// CSVファイルへの書込
		nRet = csvUcFile.CsvWrite(&csaInfo);
		// エラー処理
		if ( nRet == FALSE ) {
			// プログレスバーを隠す
			icsPrgBar = HideProgress( icsPrgBar, m_pWnd, 0, SHEET_NUM, ID_EXPORT );
			// 確認メッセージ表示
			int nRetValue = imp->ShowMessageBox(IMP_ERRCODE_EXPORT, m_pWnd);
			// CSV操作オブジェクトを閉じる
			csvUcFile.Close();
			// 確認メッセージの戻り値を判断する
			if ( nRetValue == IDYES ) {		// [はい] → 次の帳票処理へ
				// プログレスバーの再表示
				(void)ShowProgress( icsPrgBar, i );
				continue;
			} else {						// [いいえ] → 処理中断
				// 各オブジェクトの解放
				if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
				if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
				if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
				// エラーを返す
				return IMP_ERRCODE_EXPORT;
			}
		}

		//##################
		//### Data Block ###
		//##################
		// 内訳書テーブルと項目行の準備
		CimpUc000Common* pUcTbl;		// 内訳書テーブルへのポインタ
		CStringArray csaCsvData;		// CSVファイル1行分のバッファ
// midori 170504 del -->
//		switch ( i + 1 ) {
// midori 170504 del <--
// midori 170504 add -->
		switch ( pUcInfSub->m_FormSeq ) {
// midori 170504 add <--
			case ID_FORMNO_011 :	// ①：預貯金等の内訳書
				pUcTbl = (CimpUc000Common*)(new CimpUc011Yotyokin());
				csaCsvData.SetSize(CSV_011_ITEM_CNT);
				csaCsvData.SetAt( 1, CSV_011_ITEM_01);
				csaCsvData.SetAt( 2, CSV_011_ITEM_02);
				csaCsvData.SetAt( 3, CSV_011_ITEM_03);
				csaCsvData.SetAt( 4, CSV_011_ITEM_04);
				csaCsvData.SetAt( 5, CSV_011_ITEM_05);
				csaCsvData.SetAt( 6, CSV_011_ITEM_06);
				csaCsvData.SetAt( 7, CSV_011_ITEM_07);
				break;
			case ID_FORMNO_021 :	// ②：受取手形の内訳書
				pUcTbl = (CimpUc000Common*)(new CimpUc021Uketoritegata());
// 登録番号（法人番号）_23/11/21 cor -->
//				csaCsvData.SetSize(CSV_021_ITEM_CNT);
//				csaCsvData.SetAt( 1, CSV_021_ITEM_01);
//				csaCsvData.SetAt( 2, CSV_021_ITEM_02);
//				csaCsvData.SetAt( 3, CSV_021_ITEM_03);
//				csaCsvData.SetAt( 4, CSV_021_ITEM_04);
//				csaCsvData.SetAt( 5, CSV_021_ITEM_05);
//// 210113_5301_1 del -->
//				//csaCsvData.SetAt( 6, CSV_021_ITEM_06);
//				//csaCsvData.SetAt( 7, CSV_021_ITEM_07);
//// 210113_5301_1 del <--
//// 210113_5301_1 add -->
//				if(bG_Kanso == TRUE)	{
//					csaCsvData.SetAt( 6, CSV_021_ITEM_06_NEW);
//					csaCsvData.SetAt( 7, CSV_021_ITEM_07_NEW);
//				}
//				else	{
//					csaCsvData.SetAt( 6, CSV_021_ITEM_06);
//					csaCsvData.SetAt( 7, CSV_021_ITEM_07);
//				}
//// 210113_5301_1 add <--
//				csaCsvData.SetAt( 8, CSV_021_ITEM_08);
//// 210113_5301_1 del -->
//				//csaCsvData.SetAt( 9, CSV_021_ITEM_09);
//				//csaCsvData.SetAt(10, CSV_021_ITEM_10);
//// 210113_5301_1 del <--
//// 210113_5301_1 add -->
//				if(bG_Kanso == TRUE)	{
//					csaCsvData.SetAt( 9, CSV_021_ITEM_09_NEW);
//					csaCsvData.SetAt(10, CSV_021_ITEM_10_NEW);
//				}
//				else	{
//					csaCsvData.SetAt( 9, CSV_021_ITEM_09);
//					csaCsvData.SetAt(10, CSV_021_ITEM_10);
//				}
//// 210113_5301_1 add <--
//				csaCsvData.SetAt(11, CSV_021_ITEM_11);
//				csaCsvData.SetAt(12, CSV_021_ITEM_12);
// -------------------------------------
// 24/02/19_ScoreLink cor -->
				//if(bG_InvNo == TRUE)	{
// --------------------------
				if(bG_InvNo == TRUE || bG_Sl == TRUE)	{
// 24/02/19_ScoreLink cor <--
					csaCsvData.SetSize(CSV_021_ITEM_CNT+1);
					csaCsvData.SetAt( 1, CSV_021_ITEM_01);
					csaCsvData.SetAt( 2, CSV_021_ITEM_13);
					csaCsvData.SetAt( 3, CSV_021_ITEM_02);
					csaCsvData.SetAt( 4, CSV_021_ITEM_03);
					csaCsvData.SetAt( 5, CSV_021_ITEM_04);
					csaCsvData.SetAt( 6, CSV_021_ITEM_05);
					csaCsvData.SetAt( 7, CSV_021_ITEM_06_NEW);
					csaCsvData.SetAt( 8, CSV_021_ITEM_07_NEW);
					csaCsvData.SetAt( 9, CSV_021_ITEM_08);
					csaCsvData.SetAt(10, CSV_021_ITEM_09_NEW);
					csaCsvData.SetAt(11, CSV_021_ITEM_10_NEW);
					csaCsvData.SetAt(12, CSV_021_ITEM_11);
					csaCsvData.SetAt(13, CSV_021_ITEM_12);
				}
				else	{
					csaCsvData.SetSize(CSV_021_ITEM_CNT);
					csaCsvData.SetAt( 1, CSV_021_ITEM_01);
					csaCsvData.SetAt( 2, CSV_021_ITEM_02);
					csaCsvData.SetAt( 3, CSV_021_ITEM_03);
					csaCsvData.SetAt( 4, CSV_021_ITEM_04);
					csaCsvData.SetAt( 5, CSV_021_ITEM_05);
					if(bG_Kanso == TRUE)	{
						csaCsvData.SetAt( 6, CSV_021_ITEM_06_NEW);
						csaCsvData.SetAt( 7, CSV_021_ITEM_07_NEW);
					}
					else	{
						csaCsvData.SetAt( 6, CSV_021_ITEM_06);
						csaCsvData.SetAt( 7, CSV_021_ITEM_07);
					}
					csaCsvData.SetAt( 8, CSV_021_ITEM_08);
					if(bG_Kanso == TRUE)	{
						csaCsvData.SetAt( 9, CSV_021_ITEM_09_NEW);
						csaCsvData.SetAt(10, CSV_021_ITEM_10_NEW);
					}
					else	{
						csaCsvData.SetAt( 9, CSV_021_ITEM_09);
						csaCsvData.SetAt(10, CSV_021_ITEM_10);
					}
					csaCsvData.SetAt(11, CSV_021_ITEM_11);
					csaCsvData.SetAt(12, CSV_021_ITEM_12);
				}
// 登録番号（法人番号）_23/11/21 cor <--
				break;
			case ID_FORMNO_031 :	// ③：売掛金（未収入金）の内訳書
				pUcTbl = (CimpUc000Common*)(new CimpUc031Urikakekin());
// 登録番号（法人番号）_23/11/21 cor -->
				//csaCsvData.SetSize(CSV_031_ITEM_CNT);
				//csaCsvData.SetAt( 1, CSV_031_ITEM_01);
				//csaCsvData.SetAt( 2, CSV_031_ITEM_02);
				//csaCsvData.SetAt( 3, CSV_031_ITEM_03);
				//csaCsvData.SetAt( 4, CSV_031_ITEM_04);
				//csaCsvData.SetAt( 5, CSV_031_ITEM_05);
				//csaCsvData.SetAt( 6, CSV_031_ITEM_06);
				//csaCsvData.SetAt( 7, CSV_031_ITEM_07);
				//csaCsvData.SetAt( 8, CSV_031_ITEM_08);
// -------------------------------------
// 24/02/19_ScoreLink cor -->
				//if(bG_InvNo == TRUE)	{
// --------------------------
				if(bG_InvNo == TRUE || bG_Sl == TRUE)	{
// 24/02/19_ScoreLink cor <--
					csaCsvData.SetSize(CSV_031_ITEM_CNT+1);
					csaCsvData.SetAt( 1, CSV_031_ITEM_01);
					csaCsvData.SetAt( 2, CSV_031_ITEM_09);
					csaCsvData.SetAt( 3, CSV_031_ITEM_02);
					csaCsvData.SetAt( 4, CSV_031_ITEM_03);
					csaCsvData.SetAt( 5, CSV_031_ITEM_04);
					csaCsvData.SetAt( 6, CSV_031_ITEM_05);
					csaCsvData.SetAt( 7, CSV_031_ITEM_06);
					csaCsvData.SetAt( 8, CSV_031_ITEM_07);
					csaCsvData.SetAt( 9, CSV_031_ITEM_08);
				}
				else	{
					csaCsvData.SetSize(CSV_031_ITEM_CNT);
					csaCsvData.SetAt( 1, CSV_031_ITEM_01);
					csaCsvData.SetAt( 2, CSV_031_ITEM_02);
					csaCsvData.SetAt( 3, CSV_031_ITEM_03);
					csaCsvData.SetAt( 4, CSV_031_ITEM_04);
					csaCsvData.SetAt( 5, CSV_031_ITEM_05);
					csaCsvData.SetAt( 6, CSV_031_ITEM_06);
					csaCsvData.SetAt( 7, CSV_031_ITEM_07);
					csaCsvData.SetAt( 8, CSV_031_ITEM_08);
				}
// 登録番号（法人番号）_23/11/21 cor <--
				break;
			case ID_FORMNO_041 :	// ④－１：仮払金（前渡金）の内訳書
				pUcTbl = (CimpUc000Common*)(new CimpUc041Karibarai());
// 登録番号（法人番号）_23/11/21 cor -->
				//csaCsvData.SetSize(CSV_041_ITEM_CNT);
				//csaCsvData.SetAt( 1, CSV_041_ITEM_01);
				//csaCsvData.SetAt( 2, CSV_041_ITEM_02);
				//csaCsvData.SetAt( 3, CSV_041_ITEM_03);
				//csaCsvData.SetAt( 4, CSV_041_ITEM_04);
				//csaCsvData.SetAt( 5, CSV_041_ITEM_05);
				//csaCsvData.SetAt( 6, CSV_041_ITEM_06);
				//csaCsvData.SetAt( 7, CSV_041_ITEM_07);
				//if(bG_Kanso == TRUE)	{
				//	csaCsvData.SetAt( 8, CSV_041_ITEM_08_NEW);
				//	csaCsvData.SetAt( 9, CSV_041_ITEM_09_NEW);
				//}
				//else	{
				//	csaCsvData.SetAt( 8, CSV_041_ITEM_08);
				//	csaCsvData.SetAt( 9, CSV_041_ITEM_09);
				//}
// -------------------------------------
// 24/02/19_ScoreLink cor -->
				//if(bG_InvNo == TRUE)	{
// --------------------------
				if(bG_InvNo == TRUE || bG_Sl == TRUE)	{
// 24/02/19_ScoreLink cor <--
					csaCsvData.SetSize(CSV_041_ITEM_CNT+1);
					csaCsvData.SetAt( 1, CSV_041_ITEM_01);
					csaCsvData.SetAt( 2, CSV_041_ITEM_10);
					csaCsvData.SetAt( 3, CSV_041_ITEM_02);
					csaCsvData.SetAt( 4, CSV_041_ITEM_03);
					csaCsvData.SetAt( 5, CSV_041_ITEM_04);
					csaCsvData.SetAt( 6, CSV_041_ITEM_05);
					csaCsvData.SetAt( 7, CSV_041_ITEM_06);
					csaCsvData.SetAt( 8, CSV_041_ITEM_07);
					csaCsvData.SetAt( 9, CSV_041_ITEM_08_NEW);
					csaCsvData.SetAt(10, CSV_041_ITEM_09_NEW);
				}
				else	{
					csaCsvData.SetSize(CSV_041_ITEM_CNT);
					csaCsvData.SetAt( 1, CSV_041_ITEM_01);
					csaCsvData.SetAt( 2, CSV_041_ITEM_02);
					csaCsvData.SetAt( 3, CSV_041_ITEM_03);
					csaCsvData.SetAt( 4, CSV_041_ITEM_04);
					csaCsvData.SetAt( 5, CSV_041_ITEM_05);
					csaCsvData.SetAt( 6, CSV_041_ITEM_06);
					csaCsvData.SetAt( 7, CSV_041_ITEM_07);
					if(bG_Kanso == TRUE)	{
						csaCsvData.SetAt( 8, CSV_041_ITEM_08_NEW);
						csaCsvData.SetAt( 9, CSV_041_ITEM_09_NEW);
					}
					else	{
						csaCsvData.SetAt( 8, CSV_041_ITEM_08);
						csaCsvData.SetAt( 9, CSV_041_ITEM_09);
					}
				}
// 登録番号（法人番号）_23/11/21 cor <--
				break;
			case ID_FORMNO_042 :	// ④－２：貸付金及び受取利息の内訳書
				pUcTbl = (CimpUc000Common*)(new CimpUc042Kasituke());
// 登録番号（法人番号）_23/11/21 cor -->
				//if(bG_Kanso == TRUE)	{
				//	csaCsvData.SetSize(CSV_042_ITEM_CNT-2);
				//	csaCsvData.SetAt( 1, CSV_042_ITEM_01_NEW);
				//	csaCsvData.SetAt( 2, CSV_042_ITEM_02_NEW);
				//	csaCsvData.SetAt( 3, CSV_042_ITEM_03_NEW);
				//	csaCsvData.SetAt( 4, CSV_042_ITEM_04_NEW);
				//}
				//else	{
				//	csaCsvData.SetSize(CSV_042_ITEM_CNT);
				//	csaCsvData.SetAt( 1, CSV_042_ITEM_01);
				//	csaCsvData.SetAt( 2, CSV_042_ITEM_02);
				//	csaCsvData.SetAt( 3, CSV_042_ITEM_03);
				//	csaCsvData.SetAt( 4, CSV_042_ITEM_04);
				//}
				//csaCsvData.SetAt( 5, CSV_042_ITEM_05);
				//csaCsvData.SetAt( 6, CSV_042_ITEM_06);
				//csaCsvData.SetAt( 7, CSV_042_ITEM_07);
				//csaCsvData.SetAt( 8, CSV_042_ITEM_08);
				//if(bG_Kanso == TRUE)	{
				//	csaCsvData.SetAt( 9, CSV_042_ITEM_11);
				//	csaCsvData.SetAt(10, CSV_042_ITEM_12);
				//}
				//else	{
				//	csaCsvData.SetAt( 9, CSV_042_ITEM_09);
				//	csaCsvData.SetAt(10, CSV_042_ITEM_10);
				//	csaCsvData.SetAt(11, CSV_042_ITEM_11);
				//	csaCsvData.SetAt(12, CSV_042_ITEM_12);
				//}
// -------------------------------------
// 24/02/19_ScoreLink cor -->
				//if(bG_InvNo == TRUE)	{
// --------------------------
				if(bG_Sl == TRUE)	{
					// 登録番号の様式に簡素化前の２項目（貸付理由１、２）を追加するので+1
					csaCsvData.SetSize(CSV_042_ITEM_CNT+1);
					csaCsvData.SetAt( 1, CSV_042_ITEM_13);
					csaCsvData.SetAt( 2, CSV_042_ITEM_01_NEW);
					csaCsvData.SetAt( 3, CSV_042_ITEM_02_NEW);
					csaCsvData.SetAt( 4, CSV_042_ITEM_03_NEW);
					csaCsvData.SetAt( 5, CSV_042_ITEM_04_NEW);
					csaCsvData.SetAt( 6, CSV_042_ITEM_05);
					csaCsvData.SetAt( 7, CSV_042_ITEM_06);
					csaCsvData.SetAt( 8, CSV_042_ITEM_07);
					csaCsvData.SetAt( 9, CSV_042_ITEM_08);
					csaCsvData.SetAt(10, CSV_042_ITEM_09);
					csaCsvData.SetAt(11, CSV_042_ITEM_10);
					csaCsvData.SetAt(12, CSV_042_ITEM_11);
					csaCsvData.SetAt(13, CSV_042_ITEM_12);
				}
				else if(bG_InvNo == TRUE)	{
// 24/02/19_ScoreLink cor <--
					// CSV_042_ITEM_CNT が簡素化前の項目数になっているので
					// ここだけ -1 で合っている
					csaCsvData.SetSize(CSV_042_ITEM_CNT-1);
					csaCsvData.SetAt( 1, CSV_042_ITEM_13);
					csaCsvData.SetAt( 2, CSV_042_ITEM_01_NEW);
					csaCsvData.SetAt( 3, CSV_042_ITEM_02_NEW);
					csaCsvData.SetAt( 4, CSV_042_ITEM_03_NEW);
					csaCsvData.SetAt( 5, CSV_042_ITEM_04_NEW);
					csaCsvData.SetAt( 6, CSV_042_ITEM_05);
					csaCsvData.SetAt( 7, CSV_042_ITEM_06);
					csaCsvData.SetAt( 8, CSV_042_ITEM_07);
					csaCsvData.SetAt( 9, CSV_042_ITEM_08);
					csaCsvData.SetAt(10, CSV_042_ITEM_11);
					csaCsvData.SetAt(11, CSV_042_ITEM_12);
				}
				else	{
					if(bG_Kanso == TRUE)	{
						csaCsvData.SetSize(CSV_042_ITEM_CNT-2);
						csaCsvData.SetAt( 1, CSV_042_ITEM_01_NEW);
						csaCsvData.SetAt( 2, CSV_042_ITEM_02_NEW);
						csaCsvData.SetAt( 3, CSV_042_ITEM_03_NEW);
						csaCsvData.SetAt( 4, CSV_042_ITEM_04_NEW);
					}
					else	{
						csaCsvData.SetSize(CSV_042_ITEM_CNT);
						csaCsvData.SetAt( 1, CSV_042_ITEM_01);
						csaCsvData.SetAt( 2, CSV_042_ITEM_02);
						csaCsvData.SetAt( 3, CSV_042_ITEM_03);
						csaCsvData.SetAt( 4, CSV_042_ITEM_04);
					}
					csaCsvData.SetAt( 5, CSV_042_ITEM_05);
					csaCsvData.SetAt( 6, CSV_042_ITEM_06);
					csaCsvData.SetAt( 7, CSV_042_ITEM_07);
					csaCsvData.SetAt( 8, CSV_042_ITEM_08);
					if(bG_Kanso == TRUE)	{
						csaCsvData.SetAt( 9, CSV_042_ITEM_11);
						csaCsvData.SetAt(10, CSV_042_ITEM_12);
					}
					else	{
						csaCsvData.SetAt( 9, CSV_042_ITEM_09);
						csaCsvData.SetAt(10, CSV_042_ITEM_10);
						csaCsvData.SetAt(11, CSV_042_ITEM_11);
						csaCsvData.SetAt(12, CSV_042_ITEM_12);
					}
				}
// 登録番号（法人番号）_23/11/21 cor <--
				break;
			case ID_FORMNO_051 :	// ⑤：棚卸資産の内訳書
				pUcTbl = (CimpUc000Common*)(new CimpUc051Tanaoroshi());
				csaCsvData.SetSize(CSV_051_ITEM_CNT);
				csaCsvData.SetAt( 1, CSV_051_ITEM_01);
				csaCsvData.SetAt( 2, CSV_051_ITEM_02);
				csaCsvData.SetAt( 3, CSV_051_ITEM_03);
				csaCsvData.SetAt( 4, CSV_051_ITEM_04);
				csaCsvData.SetAt( 5, CSV_051_ITEM_05);
				csaCsvData.SetAt( 6, CSV_051_ITEM_06);
				csaCsvData.SetAt( 7, CSV_051_ITEM_07);
				csaCsvData.SetAt( 8, CSV_051_ITEM_08);
				csaCsvData.SetAt( 9, CSV_051_ITEM_09);
				break;
			case ID_FORMNO_061 :	// ⑥：有価証券の内訳書
				pUcTbl = (CimpUc000Common*)(new CimpUc061Yuukasyouken());
				csaCsvData.SetSize(CSV_061_ITEM_CNT);
				csaCsvData.SetAt( 1, CSV_061_ITEM_01);
// 210113_5301_1 del -->
				//csaCsvData.SetAt( 2, CSV_061_ITEM_02);
				//csaCsvData.SetAt( 3, CSV_061_ITEM_03);
// 210113_5301_1 del <--
// 210113_5301_1 add -->
// 24/02/19_ScoreLink cor -->
				//if(bG_Kanso == TRUE)	{
// --------------------------
				if(bG_Kanso == TRUE || bG_Sl == TRUE)	{
// 24/02/19_ScoreLink cor <--
					csaCsvData.SetAt( 2, CSV_061_ITEM_02_NEW);
					csaCsvData.SetAt( 3, CSV_061_ITEM_03_NEW);
				}
				else	{
					csaCsvData.SetAt( 2, CSV_061_ITEM_02);
					csaCsvData.SetAt( 3, CSV_061_ITEM_03);
				}
// 210113_5301_1 add <--
				csaCsvData.SetAt( 4, CSV_061_ITEM_04);
				csaCsvData.SetAt( 5, CSV_061_ITEM_05);
				csaCsvData.SetAt( 6, CSV_061_ITEM_06);
				csaCsvData.SetAt( 7, CSV_061_ITEM_07);
				csaCsvData.SetAt( 8, CSV_061_ITEM_08);
				csaCsvData.SetAt( 9, CSV_061_ITEM_09);
				csaCsvData.SetAt(10, CSV_061_ITEM_10);
				csaCsvData.SetAt(11, CSV_061_ITEM_11);
				csaCsvData.SetAt(12, CSV_061_ITEM_12);
				csaCsvData.SetAt(13, CSV_061_ITEM_13);
				csaCsvData.SetAt(14, CSV_061_ITEM_14);
				csaCsvData.SetAt(15, CSV_061_ITEM_15);
				csaCsvData.SetAt(16, CSV_061_ITEM_16);
				csaCsvData.SetAt(17, CSV_061_ITEM_17);
				csaCsvData.SetAt(18, CSV_061_ITEM_18);
				csaCsvData.SetAt(19, CSV_061_ITEM_19);
				csaCsvData.SetAt(20, CSV_061_ITEM_20);
				break;
			case ID_FORMNO_071 :	// ⑦：固定資産の内訳書
				pUcTbl = (CimpUc000Common*)(new CimpUc071Koteisisan());
// 登録番号（法人番号）_23/11/21 cor -->
				//csaCsvData.SetSize(CSV_071_ITEM_CNT);
				//csaCsvData.SetAt( 1, CSV_071_ITEM_01);
				//csaCsvData.SetAt( 2, CSV_071_ITEM_02);
				//csaCsvData.SetAt( 3, CSV_071_ITEM_03);
				//csaCsvData.SetAt( 4, CSV_071_ITEM_04);
				//csaCsvData.SetAt( 5, CSV_071_ITEM_05);
				//csaCsvData.SetAt( 6, CSV_071_ITEM_06);
				//csaCsvData.SetAt( 7, CSV_071_ITEM_07);
				//csaCsvData.SetAt( 8, CSV_071_ITEM_08);
				//csaCsvData.SetAt( 9, CSV_071_ITEM_09);
				//csaCsvData.SetAt(10, CSV_071_ITEM_10);
				//csaCsvData.SetAt(11, CSV_071_ITEM_11);
				//csaCsvData.SetAt(12, CSV_071_ITEM_12);
				//csaCsvData.SetAt(13, CSV_071_ITEM_13);
				//csaCsvData.SetAt(14, CSV_071_ITEM_14);
				//csaCsvData.SetAt(15, CSV_071_ITEM_15);
				//csaCsvData.SetAt(16, CSV_071_ITEM_16);
				//csaCsvData.SetAt(17, CSV_071_ITEM_17);
				//csaCsvData.SetAt(18, CSV_071_ITEM_18);
// -------------------------------------
// 24/02/19_ScoreLink cor -->
				//if(bG_InvNo == TRUE)	{
// --------------------------
				if(bG_InvNo == TRUE || bG_Sl == TRUE)	{
// 24/02/19_ScoreLink cor <--
					csaCsvData.SetSize(CSV_071_ITEM_CNT+1);
					csaCsvData.SetAt( 1, CSV_071_ITEM_01);
					csaCsvData.SetAt( 2, CSV_071_ITEM_02);
					csaCsvData.SetAt( 3, CSV_071_ITEM_03);
					csaCsvData.SetAt( 4, CSV_071_ITEM_04);
					csaCsvData.SetAt( 5, CSV_071_ITEM_05);
					csaCsvData.SetAt( 6, CSV_071_ITEM_06);
					csaCsvData.SetAt( 7, CSV_071_ITEM_07);
					csaCsvData.SetAt( 8, CSV_071_ITEM_08);
					csaCsvData.SetAt( 9, CSV_071_ITEM_09);
					csaCsvData.SetAt(10, CSV_071_ITEM_10);
					csaCsvData.SetAt(11, CSV_071_ITEM_11);
					csaCsvData.SetAt(12, CSV_071_ITEM_12);
					csaCsvData.SetAt(13, CSV_071_ITEM_13);
					csaCsvData.SetAt(14, CSV_071_ITEM_19);
					csaCsvData.SetAt(15, CSV_071_ITEM_14);
					csaCsvData.SetAt(16, CSV_071_ITEM_15);
					csaCsvData.SetAt(17, CSV_071_ITEM_16);
					csaCsvData.SetAt(18, CSV_071_ITEM_17);
					csaCsvData.SetAt(19, CSV_071_ITEM_18);
				}
				else	{
					csaCsvData.SetSize(CSV_071_ITEM_CNT);
					csaCsvData.SetAt( 1, CSV_071_ITEM_01);
					csaCsvData.SetAt( 2, CSV_071_ITEM_02);
					csaCsvData.SetAt( 3, CSV_071_ITEM_03);
					csaCsvData.SetAt( 4, CSV_071_ITEM_04);
					csaCsvData.SetAt( 5, CSV_071_ITEM_05);
					csaCsvData.SetAt( 6, CSV_071_ITEM_06);
					csaCsvData.SetAt( 7, CSV_071_ITEM_07);
					csaCsvData.SetAt( 8, CSV_071_ITEM_08);
					csaCsvData.SetAt( 9, CSV_071_ITEM_09);
					csaCsvData.SetAt(10, CSV_071_ITEM_10);
					csaCsvData.SetAt(11, CSV_071_ITEM_11);
					csaCsvData.SetAt(12, CSV_071_ITEM_12);
					csaCsvData.SetAt(13, CSV_071_ITEM_13);
					csaCsvData.SetAt(14, CSV_071_ITEM_14);
					csaCsvData.SetAt(15, CSV_071_ITEM_15);
					csaCsvData.SetAt(16, CSV_071_ITEM_16);
					csaCsvData.SetAt(17, CSV_071_ITEM_17);
					csaCsvData.SetAt(18, CSV_071_ITEM_18);
				}
// 登録番号（法人番号）_23/11/21 cor <--
				break;
			case ID_FORMNO_081 :	// ⑧：支払手形の内訳書
				pUcTbl = (CimpUc000Common*)(new CimpUc081Siharaitegata());
// 登録番号（法人番号）_23/11/21 cor -->
				//csaCsvData.SetSize(CSV_081_ITEM_CNT);
				//csaCsvData.SetAt( 1, CSV_081_ITEM_01);
				//csaCsvData.SetAt( 2, CSV_081_ITEM_02);
				//csaCsvData.SetAt( 3, CSV_081_ITEM_03);
				//csaCsvData.SetAt( 4, CSV_081_ITEM_04);
				//if(bG_Kanso == TRUE)	{
				//	csaCsvData.SetAt( 5, CSV_081_ITEM_05_NEW);
				//	csaCsvData.SetAt( 6, CSV_081_ITEM_06_NEW);
				//}
				//else	{
				//	csaCsvData.SetAt( 5, CSV_081_ITEM_05);
				//	csaCsvData.SetAt( 6, CSV_081_ITEM_06);
				//}
				//csaCsvData.SetAt( 7, CSV_081_ITEM_07);
				//csaCsvData.SetAt( 8, CSV_081_ITEM_08);
				//csaCsvData.SetAt( 9, CSV_081_ITEM_09);
// -------------------------------------
// 改良No.21-0086,21-0529 cor -->
//// 24/02/19_ScoreLink cor -->
//				//if(bG_InvNo == TRUE)	{
//// --------------------------
//// 24/05/31_ScoreLink cor -->
//				//if(bG_InvNo == TRUE || bG_Sl == TRUE)	{
//// --------------------------
//				if(bG_Sl == TRUE)	{
//					csaCsvData.SetSize(CSV_081_ITEM_CNT+2);
//					csaCsvData.SetAt( 1, CSV_081_ITEM_11);
//					csaCsvData.SetAt( 2, CSV_081_ITEM_10);
//					csaCsvData.SetAt( 3, CSV_081_ITEM_01);
//					csaCsvData.SetAt( 4, CSV_081_ITEM_02);
//					csaCsvData.SetAt( 5, CSV_081_ITEM_03);
//					csaCsvData.SetAt( 6, CSV_081_ITEM_04);
//					csaCsvData.SetAt( 7, CSV_081_ITEM_05_NEW);
//					csaCsvData.SetAt( 8, CSV_081_ITEM_06_NEW);
//					csaCsvData.SetAt( 9, CSV_081_ITEM_07);
//					csaCsvData.SetAt(10, CSV_081_ITEM_08);
//					csaCsvData.SetAt(11, CSV_081_ITEM_09);
//				}
//				else if(bG_InvNo == TRUE)	{
//// 24/05/31_ScoreLink cor <--
//// 24/02/19_ScoreLink cor <--
//					csaCsvData.SetSize(CSV_081_ITEM_CNT+1);
//					csaCsvData.SetAt( 1, CSV_081_ITEM_10);
//					csaCsvData.SetAt( 2, CSV_081_ITEM_01);
//					csaCsvData.SetAt( 3, CSV_081_ITEM_02);
//					csaCsvData.SetAt( 4, CSV_081_ITEM_03);
//					csaCsvData.SetAt( 5, CSV_081_ITEM_04);
//					csaCsvData.SetAt( 6, CSV_081_ITEM_05_NEW);
//					csaCsvData.SetAt( 7, CSV_081_ITEM_06_NEW);
//					csaCsvData.SetAt( 8, CSV_081_ITEM_07);
//					csaCsvData.SetAt( 9, CSV_081_ITEM_08);
//					csaCsvData.SetAt(10, CSV_081_ITEM_09);
//				}
// ------------------------------
				if(bG_InvNo == TRUE || bG_Sl == TRUE)	{
					csaCsvData.SetSize(CSV_081_ITEM_CNT_K+1);
					csaCsvData.SetAt( 1, CSV_081_ITEM_11);
					csaCsvData.SetAt( 2, CSV_081_ITEM_10);
					csaCsvData.SetAt( 3, CSV_081_ITEM_01);
					csaCsvData.SetAt( 4, CSV_081_ITEM_02);
					csaCsvData.SetAt( 5, CSV_081_ITEM_03);
					csaCsvData.SetAt( 6, CSV_081_ITEM_04);
					csaCsvData.SetAt( 7, CSV_081_ITEM_05_NEW);
					csaCsvData.SetAt( 8, CSV_081_ITEM_06_NEW);
					csaCsvData.SetAt( 9, CSV_081_ITEM_07);
					csaCsvData.SetAt(10, CSV_081_ITEM_08);
					csaCsvData.SetAt(11, CSV_081_ITEM_09);
				}
// 改良No.21-0086,21-0529 cor <--
				else	{
					csaCsvData.SetSize(CSV_081_ITEM_CNT);
					csaCsvData.SetAt( 1, CSV_081_ITEM_01);
					csaCsvData.SetAt( 2, CSV_081_ITEM_02);
					csaCsvData.SetAt( 3, CSV_081_ITEM_03);
					csaCsvData.SetAt( 4, CSV_081_ITEM_04);
					if(bG_Kanso == TRUE)	{
						csaCsvData.SetAt( 5, CSV_081_ITEM_05_NEW);
						csaCsvData.SetAt( 6, CSV_081_ITEM_06_NEW);
					}
					else	{
						csaCsvData.SetAt( 5, CSV_081_ITEM_05);
						csaCsvData.SetAt( 6, CSV_081_ITEM_06);
					}
					csaCsvData.SetAt( 7, CSV_081_ITEM_07);
					csaCsvData.SetAt( 8, CSV_081_ITEM_08);
					csaCsvData.SetAt( 9, CSV_081_ITEM_09);
				}
// 登録番号（法人番号）_23/11/21 cor <--
				break;
			case ID_FORMNO_091 :	// ⑨：買掛金の内訳書
				pUcTbl = (CimpUc000Common*)(new CimpUc091Kaikakekin());
// 登録番号（法人番号）_23/11/21 cor -->
				//csaCsvData.SetSize(CSV_091_ITEM_CNT);
				//csaCsvData.SetAt( 1, CSV_091_ITEM_01);
				//csaCsvData.SetAt( 2, CSV_091_ITEM_02);
				//csaCsvData.SetAt( 3, CSV_091_ITEM_03);
				//csaCsvData.SetAt( 4, CSV_091_ITEM_04);
				//csaCsvData.SetAt( 5, CSV_091_ITEM_05);
				//csaCsvData.SetAt( 6, CSV_091_ITEM_06);
				//csaCsvData.SetAt( 7, CSV_091_ITEM_07);
				//csaCsvData.SetAt( 8, CSV_091_ITEM_08);
// -------------------------------------
// 24/02/19_ScoreLink cor -->
				//if(bG_InvNo == TRUE)	{
// --------------------------
				if(bG_InvNo == TRUE || bG_Sl == TRUE)	{
// 24/02/19_ScoreLink cor <--
					csaCsvData.SetSize(CSV_091_ITEM_CNT+1);
					csaCsvData.SetAt( 1, CSV_091_ITEM_01);
					csaCsvData.SetAt( 2, CSV_091_ITEM_09);
					csaCsvData.SetAt( 3, CSV_091_ITEM_02);
					csaCsvData.SetAt( 4, CSV_091_ITEM_03);
					csaCsvData.SetAt( 5, CSV_091_ITEM_04);
					csaCsvData.SetAt( 6, CSV_091_ITEM_05);
					csaCsvData.SetAt( 7, CSV_091_ITEM_06);
					csaCsvData.SetAt( 8, CSV_091_ITEM_07);
					csaCsvData.SetAt( 9, CSV_091_ITEM_08);
				}
				else	{
					csaCsvData.SetSize(CSV_091_ITEM_CNT);
					csaCsvData.SetAt( 1, CSV_091_ITEM_01);
					csaCsvData.SetAt( 2, CSV_091_ITEM_02);
					csaCsvData.SetAt( 3, CSV_091_ITEM_03);
					csaCsvData.SetAt( 4, CSV_091_ITEM_04);
					csaCsvData.SetAt( 5, CSV_091_ITEM_05);
					csaCsvData.SetAt( 6, CSV_091_ITEM_06);
					csaCsvData.SetAt( 7, CSV_091_ITEM_07);
					csaCsvData.SetAt( 8, CSV_091_ITEM_08);
				}
// 登録番号（法人番号）_23/11/21 cor <--
				break;
			case ID_FORMNO_101 :	// ⑩－１：仮受金の内訳書
				pUcTbl = (CimpUc000Common*)(new CimpUc101Kariukekin());
// 登録番号（法人番号）_23/11/21 cor -->
				//csaCsvData.SetSize(CSV_101_ITEM_CNT);
				//csaCsvData.SetAt( 1, CSV_101_ITEM_01);
				//csaCsvData.SetAt( 2, CSV_101_ITEM_02);
				//csaCsvData.SetAt( 3, CSV_101_ITEM_03);
				//csaCsvData.SetAt( 4, CSV_101_ITEM_04);
				//csaCsvData.SetAt( 5, CSV_101_ITEM_05);
				//csaCsvData.SetAt( 6, CSV_101_ITEM_06);
				//csaCsvData.SetAt( 7, CSV_101_ITEM_07);
				//if(bG_Kanso == TRUE)	{
				//	csaCsvData.SetAt( 8, CSV_101_ITEM_08_NEW);
				//	csaCsvData.SetAt( 9, CSV_101_ITEM_09_NEW);
				//}
				//else	{
				//	csaCsvData.SetAt( 8, CSV_101_ITEM_08);
				//	csaCsvData.SetAt( 9, CSV_101_ITEM_09);
				//}
// -------------------------------------
// 24/02/19_ScoreLink cor -->
				//if(bG_InvNo == TRUE)	{
// --------------------------
				if(bG_InvNo == TRUE || bG_Sl == TRUE)	{
// 24/02/19_ScoreLink cor <--
					csaCsvData.SetSize(CSV_101_ITEM_CNT+1);
					csaCsvData.SetAt( 1, CSV_101_ITEM_01);
					csaCsvData.SetAt( 2, CSV_101_ITEM_10);
					csaCsvData.SetAt( 3, CSV_101_ITEM_02);
					csaCsvData.SetAt( 4, CSV_101_ITEM_03);
					csaCsvData.SetAt( 5, CSV_101_ITEM_04);
					csaCsvData.SetAt( 6, CSV_101_ITEM_05);
					csaCsvData.SetAt( 7, CSV_101_ITEM_06);
					csaCsvData.SetAt( 8, CSV_101_ITEM_07);
					csaCsvData.SetAt( 9, CSV_101_ITEM_08_NEW);
					csaCsvData.SetAt(10, CSV_101_ITEM_09_NEW);
				}
				else	{
					csaCsvData.SetSize(CSV_101_ITEM_CNT);
					csaCsvData.SetAt( 1, CSV_101_ITEM_01);
					csaCsvData.SetAt( 2, CSV_101_ITEM_02);
					csaCsvData.SetAt( 3, CSV_101_ITEM_03);
					csaCsvData.SetAt( 4, CSV_101_ITEM_04);
					csaCsvData.SetAt( 5, CSV_101_ITEM_05);
					csaCsvData.SetAt( 6, CSV_101_ITEM_06);
					csaCsvData.SetAt( 7, CSV_101_ITEM_07);
					if(bG_Kanso == TRUE)	{
						csaCsvData.SetAt( 8, CSV_101_ITEM_08_NEW);
						csaCsvData.SetAt( 9, CSV_101_ITEM_09_NEW);
					}
					else	{
						csaCsvData.SetAt( 8, CSV_101_ITEM_08);
						csaCsvData.SetAt( 9, CSV_101_ITEM_09);
					}
				}
// 登録番号（法人番号）_23/11/21 cor <--
				break;
			case ID_FORMNO_102 :	// ⑩－２：源泉所得税預り金の内訳書
				pUcTbl = (CimpUc000Common*)(new CimpUc102Gensan());
				csaCsvData.SetSize(CSV_102_ITEM_CNT);
				csaCsvData.SetAt( 1, CSV_102_ITEM_01);
				csaCsvData.SetAt( 2, CSV_102_ITEM_02);
				csaCsvData.SetAt( 3, CSV_102_ITEM_03);
				break;
			case ID_FORMNO_111 :	// ⑪：借入金及び支払利子の内訳書
				pUcTbl = (CimpUc000Common*)(new CimpUc111Kariirekin());
// 210113_5301_1 del -->
				//csaCsvData.SetSize(CSV_111_ITEM_CNT);
				//csaCsvData.SetAt( 1, CSV_111_ITEM_01);
				//csaCsvData.SetAt( 2, CSV_111_ITEM_02);
				//csaCsvData.SetAt( 3, CSV_111_ITEM_03);
				//csaCsvData.SetAt( 4, CSV_111_ITEM_04);
				//csaCsvData.SetAt( 5, CSV_111_ITEM_05);
// 210113_5301_1 del <--
// 210113_5301_1 add -->
// 24/02/19_ScoreLink cor -->
				//if(bG_Kanso == TRUE)	{
				//	csaCsvData.SetSize(CSV_111_ITEM_CNT-2);
// --------------------------
				if(bG_Kanso == TRUE || bG_Sl == TRUE)	{
					if(bG_Sl == TRUE)	{
						csaCsvData.SetSize(CSV_111_ITEM_CNT);
					}
					else	{
						csaCsvData.SetSize(CSV_111_ITEM_CNT-2);
					}
// 24/02/19_ScoreLink cor <--
					csaCsvData.SetAt( 1, CSV_111_ITEM_01);
					csaCsvData.SetAt( 2, CSV_111_ITEM_02_NEW);
					csaCsvData.SetAt( 3, CSV_111_ITEM_03_NEW);
					csaCsvData.SetAt( 4, CSV_111_ITEM_04_NEW);
					csaCsvData.SetAt( 5, CSV_111_ITEM_05_NEW);
				}
				else	{
					csaCsvData.SetSize(CSV_111_ITEM_CNT);
					csaCsvData.SetAt( 1, CSV_111_ITEM_01);
					csaCsvData.SetAt( 2, CSV_111_ITEM_02);
					csaCsvData.SetAt( 3, CSV_111_ITEM_03);
					csaCsvData.SetAt( 4, CSV_111_ITEM_04);
					csaCsvData.SetAt( 5, CSV_111_ITEM_05);
				}
// 210113_5301_1 add <--
				csaCsvData.SetAt( 6, CSV_111_ITEM_06);
				csaCsvData.SetAt( 7, CSV_111_ITEM_07);
				csaCsvData.SetAt( 8, CSV_111_ITEM_08);
				csaCsvData.SetAt( 9, CSV_111_ITEM_09);
// 210113_5301_1 del -->
				//csaCsvData.SetAt(10, CSV_111_ITEM_10);
				//csaCsvData.SetAt(11, CSV_111_ITEM_11);
				//csaCsvData.SetAt(12, CSV_111_ITEM_12);
				//csaCsvData.SetAt(13, CSV_111_ITEM_13);
// 210113_5301_1 del <--
// 210113_5301_1 add -->
// 24/02/19_ScoreLink cor -->
				//if(bG_Kanso == TRUE)	{
// --------------------------
				if(bG_Kanso == TRUE && bG_Sl == FALSE)	{
// 24/02/19_ScoreLink cor <--
					csaCsvData.SetAt(10, CSV_111_ITEM_12);
					csaCsvData.SetAt(11, CSV_111_ITEM_13);
				}
				else	{
					csaCsvData.SetAt(10, CSV_111_ITEM_10);
					csaCsvData.SetAt(11, CSV_111_ITEM_11);
					csaCsvData.SetAt(12, CSV_111_ITEM_12);
					csaCsvData.SetAt(13, CSV_111_ITEM_13);
				}
// 210113_5301_1 add <--
				break;
			case ID_FORMNO_121 :	// ⑫：土地の売上高等の内訳書
				pUcTbl = (CimpUc000Common*)(new CimpUc121Totiuriage()); 
// 登録番号（法人番号）_23/11/21 cor -->
				//csaCsvData.SetSize(CSV_121_ITEM_CNT);
				//csaCsvData.SetAt( 1, CSV_121_ITEM_01);
				//csaCsvData.SetAt( 2, CSV_121_ITEM_02);
				//csaCsvData.SetAt( 3, CSV_121_ITEM_03);
				//csaCsvData.SetAt( 4, CSV_121_ITEM_04);
				//csaCsvData.SetAt( 5, CSV_121_ITEM_05);
				//csaCsvData.SetAt( 6, CSV_121_ITEM_06);
				//csaCsvData.SetAt( 7, CSV_121_ITEM_07);
				//csaCsvData.SetAt( 8, CSV_121_ITEM_08);
				//csaCsvData.SetAt( 9, CSV_121_ITEM_09);
				//csaCsvData.SetAt(10, CSV_121_ITEM_10);
				//csaCsvData.SetAt(11, CSV_121_ITEM_11);
				//csaCsvData.SetAt(12, CSV_121_ITEM_12);
				//csaCsvData.SetAt(13, CSV_121_ITEM_13);
// -------------------------------------
// 24/02/19_ScoreLink cor -->
				//if(bG_InvNo == TRUE)	{
// --------------------------
				if(bG_InvNo == TRUE || bG_Sl == TRUE)	{
// 24/02/19_ScoreLink cor <--
					csaCsvData.SetSize(CSV_121_ITEM_CNT+1);
					csaCsvData.SetAt( 1, CSV_121_ITEM_01);
					csaCsvData.SetAt( 2, CSV_121_ITEM_02);
					csaCsvData.SetAt( 3, CSV_121_ITEM_03);
					csaCsvData.SetAt( 4, CSV_121_ITEM_04);
					csaCsvData.SetAt( 5, CSV_121_ITEM_05);
					csaCsvData.SetAt( 6, CSV_121_ITEM_06);
					csaCsvData.SetAt( 7, CSV_121_ITEM_14);
					csaCsvData.SetAt( 8, CSV_121_ITEM_07);
					csaCsvData.SetAt( 9, CSV_121_ITEM_08);
					csaCsvData.SetAt(10, CSV_121_ITEM_09);
					csaCsvData.SetAt(11, CSV_121_ITEM_10);
					csaCsvData.SetAt(12, CSV_121_ITEM_11);
					csaCsvData.SetAt(13, CSV_121_ITEM_12);
					csaCsvData.SetAt(14, CSV_121_ITEM_13);
				}
				else	{
					csaCsvData.SetSize(CSV_121_ITEM_CNT);
					csaCsvData.SetAt( 1, CSV_121_ITEM_01);
					csaCsvData.SetAt( 2, CSV_121_ITEM_02);
					csaCsvData.SetAt( 3, CSV_121_ITEM_03);
					csaCsvData.SetAt( 4, CSV_121_ITEM_04);
					csaCsvData.SetAt( 5, CSV_121_ITEM_05);
					csaCsvData.SetAt( 6, CSV_121_ITEM_06);
					csaCsvData.SetAt( 7, CSV_121_ITEM_07);
					csaCsvData.SetAt( 8, CSV_121_ITEM_08);
					csaCsvData.SetAt( 9, CSV_121_ITEM_09);
					csaCsvData.SetAt(10, CSV_121_ITEM_10);
					csaCsvData.SetAt(11, CSV_121_ITEM_11);
					csaCsvData.SetAt(12, CSV_121_ITEM_12);
					csaCsvData.SetAt(13, CSV_121_ITEM_13);
				}
// 登録番号（法人番号）_23/11/21 cor <--
				break;
			case ID_FORMNO_131 :	// ⑬：売上高等の事業所別内訳書
				pUcTbl = (CimpUc000Common*)(new CimpUc131Uriagedaka());
// 210113_5301_1 del -->
				//csaCsvData.SetSize(CSV_131_ITEM_CNT);
// 210113_5301_1 del <--
// 210113_5301_1 add -->
// 24/02/19_ScoreLink cor -->
				//if(bG_Kanso == TRUE)	{
// --------------------------
				if(bG_Kanso == TRUE && bG_Sl == FALSE)	{
// 24/02/19_ScoreLink cor <--
					csaCsvData.SetSize(CSV_131_ITEM_CNT-1);
				}
				else	{
					csaCsvData.SetSize(CSV_131_ITEM_CNT);
				}
// 210113_5301_1 add <--
				csaCsvData.SetAt( 1, CSV_131_ITEM_01);
				csaCsvData.SetAt( 2, CSV_131_ITEM_02);
				csaCsvData.SetAt( 3, CSV_131_ITEM_03);
				csaCsvData.SetAt( 4, CSV_131_ITEM_04);
				csaCsvData.SetAt( 5, CSV_131_ITEM_05);
				csaCsvData.SetAt( 6, CSV_131_ITEM_06);
				csaCsvData.SetAt( 7, CSV_131_ITEM_07);
				csaCsvData.SetAt( 8, CSV_131_ITEM_08);
				csaCsvData.SetAt( 9, CSV_131_ITEM_09);
				csaCsvData.SetAt(10, CSV_131_ITEM_10);
				csaCsvData.SetAt(11, CSV_131_ITEM_11);
				csaCsvData.SetAt(12, CSV_131_ITEM_12);
// 210113_5301_1 del -->
				//csaCsvData.SetAt(13, CSV_131_ITEM_13);
				//csaCsvData.SetAt(14, CSV_131_ITEM_14);
				//csaCsvData.SetAt(15, CSV_131_ITEM_15);
				//csaCsvData.SetAt(16, CSV_131_ITEM_16);
				//csaCsvData.SetAt(17, CSV_131_ITEM_17);
				//csaCsvData.SetAt(18, CSV_131_ITEM_18);
// 210113_5301_1 del <--
// 210113_5301_1 add -->
// 24/02/19_ScoreLink cor -->
				//if(bG_Kanso == TRUE)	{
// --------------------------
				if(bG_Kanso == TRUE && bG_Sl == FALSE)	{
// 24/02/19_ScoreLink cor <--
					csaCsvData.SetAt(13, CSV_131_ITEM_14);
					csaCsvData.SetAt(14, CSV_131_ITEM_15);
					csaCsvData.SetAt(15, CSV_131_ITEM_16);
					csaCsvData.SetAt(16, CSV_131_ITEM_17);
					csaCsvData.SetAt(17, CSV_131_ITEM_18);
				}
				else	{
					csaCsvData.SetAt(13, CSV_131_ITEM_13);
					csaCsvData.SetAt(14, CSV_131_ITEM_14);
					csaCsvData.SetAt(15, CSV_131_ITEM_15);
					csaCsvData.SetAt(16, CSV_131_ITEM_16);
					csaCsvData.SetAt(17, CSV_131_ITEM_17);
					csaCsvData.SetAt(18, CSV_131_ITEM_18);
				}
// 210113_5301_1 add <--
				break;
			case ID_FORMNO_141 :	// ⑭－１：役員報酬手当金等の内訳書
				pUcTbl = (CimpUc000Common*)(new CimpUc141Yakuin());
				csaCsvData.SetSize(CSV_141_ITEM_CNT);
				csaCsvData.SetAt( 1, CSV_141_ITEM_01);
				csaCsvData.SetAt( 2, CSV_141_ITEM_02);
				csaCsvData.SetAt( 3, CSV_141_ITEM_03);
				csaCsvData.SetAt( 4, CSV_141_ITEM_04);
				csaCsvData.SetAt( 5, CSV_141_ITEM_05);
				csaCsvData.SetAt( 6, CSV_141_ITEM_06);
				csaCsvData.SetAt( 7, CSV_141_ITEM_07);
				csaCsvData.SetAt( 8, CSV_141_ITEM_08);
				csaCsvData.SetAt( 9, CSV_141_ITEM_09);
				csaCsvData.SetAt(10, CSV_141_ITEM_10);
				csaCsvData.SetAt(11, CSV_141_ITEM_11);
// 210113_5301_1 del -->
				//csaCsvData.SetAt(12, CSV_141_ITEM_12);
// 210113_5301_1 del <--
// 210113_5301_1 add -->
// 24/02/19_ScoreLink cor -->
				//if(bG_Kanso == TRUE)	{
// --------------------------
				if(bG_Kanso == TRUE || bG_Sl == TRUE)	{
// 24/02/19_ScoreLink cor <--
					csaCsvData.SetAt(12, CSV_141_ITEM_12_NEW);
				}
				else	{
					csaCsvData.SetAt(12, CSV_141_ITEM_12);
				}
// 210113_5301_1 add <--
				csaCsvData.SetAt(13, CSV_141_ITEM_13);
				csaCsvData.SetAt(14, CSV_141_ITEM_14);
				csaCsvData.SetAt(15, CSV_141_ITEM_15);
				break;
			case ID_FORMNO_142 :	// ⑭－２：人件費の内訳書
				pUcTbl = (CimpUc000Common*)(new CimpUc142Jinkenhi());
				csaCsvData.SetSize(CSV_142_ITEM_CNT);
				csaCsvData.SetAt( 1, CSV_142_ITEM_01);
				csaCsvData.SetAt( 2, CSV_142_ITEM_02);
				csaCsvData.SetAt( 3, CSV_142_ITEM_03);
				break;
			case ID_FORMNO_151 :	// ⑮－１：地代家賃等の内訳書
				pUcTbl = (CimpUc000Common*)(new CimpUc151Tidaiyatin());
// 登録番号（法人番号）_23/11/21 cor -->
				//csaCsvData.SetSize(CSV_151_ITEM_CNT);
				//csaCsvData.SetAt( 1, CSV_151_ITEM_01);
				//csaCsvData.SetAt( 2, CSV_151_ITEM_02);
				//csaCsvData.SetAt( 3, CSV_151_ITEM_03);
				//csaCsvData.SetAt( 4, CSV_151_ITEM_04);
				//csaCsvData.SetAt( 5, CSV_151_ITEM_05);
				//csaCsvData.SetAt( 6, CSV_151_ITEM_06);
				//csaCsvData.SetAt( 7, CSV_151_ITEM_07);
				//csaCsvData.SetAt( 8, CSV_151_ITEM_08);
				//csaCsvData.SetAt( 9, CSV_151_ITEM_09);
				//csaCsvData.SetAt(10, CSV_151_ITEM_10);
				//csaCsvData.SetAt(11, CSV_151_ITEM_11);
				//csaCsvData.SetAt(12, CSV_151_ITEM_12);
				//csaCsvData.SetAt(13, CSV_151_ITEM_13);
				//csaCsvData.SetAt(14, CSV_151_ITEM_14);
// -------------------------------------
// 24/02/19_ScoreLink cor -->
				//if(bG_InvNo == TRUE)	{
// --------------------------
				if(bG_InvNo == TRUE || bG_Sl == TRUE)	{
// 24/02/19_ScoreLink cor <--
					csaCsvData.SetSize(CSV_151_ITEM_CNT+1);
					csaCsvData.SetAt( 1, CSV_151_ITEM_01);
					csaCsvData.SetAt( 2, CSV_151_ITEM_02);
					csaCsvData.SetAt( 3, CSV_151_ITEM_03);
					csaCsvData.SetAt( 4, CSV_151_ITEM_04);
					csaCsvData.SetAt( 5, CSV_151_ITEM_05);
					csaCsvData.SetAt( 6, CSV_151_ITEM_15);
					csaCsvData.SetAt( 7, CSV_151_ITEM_06);
					csaCsvData.SetAt( 8, CSV_151_ITEM_07);
					csaCsvData.SetAt( 9, CSV_151_ITEM_08);
					csaCsvData.SetAt(10, CSV_151_ITEM_09);
					csaCsvData.SetAt(11, CSV_151_ITEM_10);
					csaCsvData.SetAt(12, CSV_151_ITEM_11);
					csaCsvData.SetAt(13, CSV_151_ITEM_12);
					csaCsvData.SetAt(14, CSV_151_ITEM_13);
					csaCsvData.SetAt(15, CSV_151_ITEM_14);
				}
				else	{
					csaCsvData.SetSize(CSV_151_ITEM_CNT);
					csaCsvData.SetAt( 1, CSV_151_ITEM_01);
					csaCsvData.SetAt( 2, CSV_151_ITEM_02);
					csaCsvData.SetAt( 3, CSV_151_ITEM_03);
					csaCsvData.SetAt( 4, CSV_151_ITEM_04);
					csaCsvData.SetAt( 5, CSV_151_ITEM_05);
					csaCsvData.SetAt( 6, CSV_151_ITEM_06);
					csaCsvData.SetAt( 7, CSV_151_ITEM_07);
					csaCsvData.SetAt( 8, CSV_151_ITEM_08);
					csaCsvData.SetAt( 9, CSV_151_ITEM_09);
					csaCsvData.SetAt(10, CSV_151_ITEM_10);
					csaCsvData.SetAt(11, CSV_151_ITEM_11);
					csaCsvData.SetAt(12, CSV_151_ITEM_12);
					csaCsvData.SetAt(13, CSV_151_ITEM_13);
					csaCsvData.SetAt(14, CSV_151_ITEM_14);
				}
// 登録番号（法人番号）_23/11/21 cor <--
				break;
			case ID_FORMNO_152 :	// ⑮－２：権利金等の期中支払の内訳書
				pUcTbl = (CimpUc000Common*)(new CimpUc152Kenrikin());
// 登録番号（法人番号）_23/11/21 cor -->
				//csaCsvData.SetSize(CSV_152_ITEM_CNT);
				//csaCsvData.SetAt( 1, CSV_152_ITEM_01);
				//csaCsvData.SetAt( 2, CSV_152_ITEM_02);
				//csaCsvData.SetAt( 3, CSV_152_ITEM_03);
				//csaCsvData.SetAt( 4, CSV_152_ITEM_04);
				//csaCsvData.SetAt( 5, CSV_152_ITEM_05);
				//csaCsvData.SetAt( 6, CSV_152_ITEM_06);
				//csaCsvData.SetAt( 7, CSV_152_ITEM_07);
				//csaCsvData.SetAt( 8, CSV_152_ITEM_08);
				//csaCsvData.SetAt( 9, CSV_152_ITEM_09);
				//csaCsvData.SetAt(10, CSV_152_ITEM_10);
// -------------------------------------
// 24/02/19_ScoreLink cor -->
				//if(bG_InvNo == TRUE)	{
// --------------------------
				if(bG_InvNo == TRUE || bG_Sl == TRUE)	{
// 24/02/19_ScoreLink cor <--
					csaCsvData.SetSize(CSV_152_ITEM_CNT+1);
					csaCsvData.SetAt( 1, CSV_152_ITEM_11);
					csaCsvData.SetAt( 2, CSV_152_ITEM_01);
					csaCsvData.SetAt( 3, CSV_152_ITEM_02);
					csaCsvData.SetAt( 4, CSV_152_ITEM_03);
					csaCsvData.SetAt( 5, CSV_152_ITEM_04);
					csaCsvData.SetAt( 6, CSV_152_ITEM_05);
					csaCsvData.SetAt( 7, CSV_152_ITEM_06);
					csaCsvData.SetAt( 8, CSV_152_ITEM_07);
					csaCsvData.SetAt( 9, CSV_152_ITEM_08);
					csaCsvData.SetAt(10, CSV_152_ITEM_09);
					csaCsvData.SetAt(11, CSV_152_ITEM_10);
				}
				else	{
					csaCsvData.SetSize(CSV_152_ITEM_CNT);
					csaCsvData.SetAt( 1, CSV_152_ITEM_01);
					csaCsvData.SetAt( 2, CSV_152_ITEM_02);
					csaCsvData.SetAt( 3, CSV_152_ITEM_03);
					csaCsvData.SetAt( 4, CSV_152_ITEM_04);
					csaCsvData.SetAt( 5, CSV_152_ITEM_05);
					csaCsvData.SetAt( 6, CSV_152_ITEM_06);
					csaCsvData.SetAt( 7, CSV_152_ITEM_07);
					csaCsvData.SetAt( 8, CSV_152_ITEM_08);
					csaCsvData.SetAt( 9, CSV_152_ITEM_09);
					csaCsvData.SetAt(10, CSV_152_ITEM_10);
				}
// 登録番号（法人番号）_23/11/21 cor <--
				break;
			case ID_FORMNO_153 :	// ⑮－３：工業所所有権等の使用料の内訳書
				pUcTbl = (CimpUc000Common*)(new CimpUc153Kougyou());
// 登録番号（法人番号）_23/11/21 cor -->
				//csaCsvData.SetSize(CSV_153_ITEM_CNT);
				//csaCsvData.SetAt( 1, CSV_153_ITEM_01);
				//csaCsvData.SetAt( 2, CSV_153_ITEM_02);
				//csaCsvData.SetAt( 3, CSV_153_ITEM_03);
				//csaCsvData.SetAt( 4, CSV_153_ITEM_04);
				//csaCsvData.SetAt( 5, CSV_153_ITEM_05);
				//csaCsvData.SetAt( 6, CSV_153_ITEM_06);
				//csaCsvData.SetAt( 7, CSV_153_ITEM_07);
				//csaCsvData.SetAt( 8, CSV_153_ITEM_08);
				//csaCsvData.SetAt( 9, CSV_153_ITEM_09);
				//csaCsvData.SetAt(10, CSV_153_ITEM_10);
				//csaCsvData.SetAt(11, CSV_153_ITEM_11);
				//csaCsvData.SetAt(12, CSV_153_ITEM_12);
				//csaCsvData.SetAt(13, CSV_153_ITEM_13);
// -------------------------------------
// 24/02/19_ScoreLink cor -->
				//if(bG_InvNo == TRUE)	{
// --------------------------
				if(bG_InvNo == TRUE || bG_Sl == TRUE)	{
// 24/02/19_ScoreLink cor <--
					csaCsvData.SetSize(CSV_153_ITEM_CNT+1);
					csaCsvData.SetAt( 1, CSV_153_ITEM_01);
					csaCsvData.SetAt( 2, CSV_153_ITEM_02);
					csaCsvData.SetAt( 3, CSV_153_ITEM_14);
					csaCsvData.SetAt( 4, CSV_153_ITEM_03);
					csaCsvData.SetAt( 5, CSV_153_ITEM_04);
					csaCsvData.SetAt( 6, CSV_153_ITEM_05);
					csaCsvData.SetAt( 7, CSV_153_ITEM_06);
					csaCsvData.SetAt( 8, CSV_153_ITEM_07);
					csaCsvData.SetAt( 9, CSV_153_ITEM_08);
					csaCsvData.SetAt(10, CSV_153_ITEM_09);
					csaCsvData.SetAt(11, CSV_153_ITEM_10);
					csaCsvData.SetAt(12, CSV_153_ITEM_11);
					csaCsvData.SetAt(13, CSV_153_ITEM_12);
					csaCsvData.SetAt(14, CSV_153_ITEM_13);
				}
				else	{
					csaCsvData.SetSize(CSV_153_ITEM_CNT);
					csaCsvData.SetAt( 1, CSV_153_ITEM_01);
					csaCsvData.SetAt( 2, CSV_153_ITEM_02);
					csaCsvData.SetAt( 3, CSV_153_ITEM_03);
					csaCsvData.SetAt( 4, CSV_153_ITEM_04);
					csaCsvData.SetAt( 5, CSV_153_ITEM_05);
					csaCsvData.SetAt( 6, CSV_153_ITEM_06);
					csaCsvData.SetAt( 7, CSV_153_ITEM_07);
					csaCsvData.SetAt( 8, CSV_153_ITEM_08);
					csaCsvData.SetAt( 9, CSV_153_ITEM_09);
					csaCsvData.SetAt(10, CSV_153_ITEM_10);
					csaCsvData.SetAt(11, CSV_153_ITEM_11);
					csaCsvData.SetAt(12, CSV_153_ITEM_12);
					csaCsvData.SetAt(13, CSV_153_ITEM_13);
				}
// 登録番号（法人番号）_23/11/21 cor <--
				break;
			case ID_FORMNO_161 :	// ⑯－１：雑益等の内訳書
				pUcTbl = (CimpUc000Common*)(new CimpUc161Zatueki());
// 登録番号（法人番号）_23/11/21 cor -->
				//csaCsvData.SetSize(CSV_161_ITEM_CNT);
				//csaCsvData.SetAt( 1, CSV_161_ITEM_01);
				//csaCsvData.SetAt( 2, CSV_161_ITEM_02);
				//csaCsvData.SetAt( 3, CSV_161_ITEM_03);
				//if(bG_Kanso == TRUE)	{
				//	csaCsvData.SetAt( 4, CSV_161_ITEM_04_NEW);
				//	csaCsvData.SetAt( 5, CSV_161_ITEM_05_NEW);
				//	csaCsvData.SetAt( 6, CSV_161_ITEM_06_NEW);
				//	csaCsvData.SetAt( 7, CSV_161_ITEM_07_NEW);
				//}
				//else	{
				//	csaCsvData.SetAt( 4, CSV_161_ITEM_04);
				//	csaCsvData.SetAt( 5, CSV_161_ITEM_05);
				//	csaCsvData.SetAt( 6, CSV_161_ITEM_06);
				//	csaCsvData.SetAt( 7, CSV_161_ITEM_07);
				//}
				//csaCsvData.SetAt( 8, CSV_161_ITEM_08);
// -------------------------------------
// 24/02/19_ScoreLink cor -->
				//if(bG_InvNo == TRUE)	{
// --------------------------
				if(bG_InvNo == TRUE || bG_Sl == TRUE)	{
// 24/02/19_ScoreLink cor <--
					csaCsvData.SetSize(CSV_161_ITEM_CNT+1);
					csaCsvData.SetAt( 1, CSV_161_ITEM_01);
					csaCsvData.SetAt( 2, CSV_161_ITEM_02);
					csaCsvData.SetAt( 3, CSV_161_ITEM_03);
					csaCsvData.SetAt( 4, CSV_161_ITEM_09);
					csaCsvData.SetAt( 5, CSV_161_ITEM_04_NEW);
					csaCsvData.SetAt( 6, CSV_161_ITEM_05_NEW);
					csaCsvData.SetAt( 7, CSV_161_ITEM_06_NEW);
					csaCsvData.SetAt( 8, CSV_161_ITEM_07_NEW);
					csaCsvData.SetAt( 9, CSV_161_ITEM_08);
				}
				else	{
					csaCsvData.SetSize(CSV_161_ITEM_CNT);
					csaCsvData.SetAt( 1, CSV_161_ITEM_01);
					csaCsvData.SetAt( 2, CSV_161_ITEM_02);
					csaCsvData.SetAt( 3, CSV_161_ITEM_03);
					if(bG_Kanso == TRUE)	{
						csaCsvData.SetAt( 4, CSV_161_ITEM_04_NEW);
						csaCsvData.SetAt( 5, CSV_161_ITEM_05_NEW);
						csaCsvData.SetAt( 6, CSV_161_ITEM_06_NEW);
						csaCsvData.SetAt( 7, CSV_161_ITEM_07_NEW);
					}
					else	{
						csaCsvData.SetAt( 4, CSV_161_ITEM_04);
						csaCsvData.SetAt( 5, CSV_161_ITEM_05);
						csaCsvData.SetAt( 6, CSV_161_ITEM_06);
						csaCsvData.SetAt( 7, CSV_161_ITEM_07);
					}
					csaCsvData.SetAt( 8, CSV_161_ITEM_08);
				}
// 登録番号（法人番号）_23/11/21 cor <--
				break;
			case ID_FORMNO_162 :	// ⑯－２：雑損益の内訳書
				pUcTbl = (CimpUc000Common*)(new CimpUc162Zatuson());
// 登録番号（法人番号）_23/11/21 cor -->
				//csaCsvData.SetSize(CSV_162_ITEM_CNT);
				//csaCsvData.SetAt( 1, CSV_162_ITEM_01);
				//csaCsvData.SetAt( 2, CSV_162_ITEM_02);
				//csaCsvData.SetAt( 3, CSV_162_ITEM_03);
				//if(bG_Kanso == TRUE)	{
				//	csaCsvData.SetAt( 4, CSV_162_ITEM_04_NEW);
				//	csaCsvData.SetAt( 5, CSV_162_ITEM_05_NEW);
				//	csaCsvData.SetAt( 6, CSV_162_ITEM_06_NEW);
				//	csaCsvData.SetAt( 7, CSV_162_ITEM_07_NEW);
				//}
				//else	{
				//	csaCsvData.SetAt( 4, CSV_162_ITEM_04);
				//	csaCsvData.SetAt( 5, CSV_162_ITEM_05);
				//	csaCsvData.SetAt( 6, CSV_162_ITEM_06);
				//	csaCsvData.SetAt( 7, CSV_162_ITEM_07);
				//}
				//csaCsvData.SetAt( 8, CSV_162_ITEM_08);
// -------------------------------------
// 24/02/19_ScoreLink cor -->
				//if(bG_InvNo == TRUE)	{
// --------------------------
				if(bG_InvNo == TRUE || bG_Sl == TRUE)	{
// 24/02/19_ScoreLink cor <--
					csaCsvData.SetSize(CSV_162_ITEM_CNT+1);
					csaCsvData.SetAt( 1, CSV_162_ITEM_01);
					csaCsvData.SetAt( 2, CSV_162_ITEM_02);
					csaCsvData.SetAt( 3, CSV_162_ITEM_03);
					csaCsvData.SetAt( 4, CSV_162_ITEM_09);
					csaCsvData.SetAt( 5, CSV_162_ITEM_04_NEW);
					csaCsvData.SetAt( 6, CSV_162_ITEM_05_NEW);
					csaCsvData.SetAt( 7, CSV_162_ITEM_06_NEW);
					csaCsvData.SetAt( 8, CSV_162_ITEM_07_NEW);
					csaCsvData.SetAt( 9, CSV_162_ITEM_08);
				}
				else	{
					csaCsvData.SetSize(CSV_162_ITEM_CNT);
					csaCsvData.SetAt( 1, CSV_162_ITEM_01);
					csaCsvData.SetAt( 2, CSV_162_ITEM_02);
					csaCsvData.SetAt( 3, CSV_162_ITEM_03);
					if(bG_Kanso == TRUE)	{
						csaCsvData.SetAt( 4, CSV_162_ITEM_04_NEW);
						csaCsvData.SetAt( 5, CSV_162_ITEM_05_NEW);
						csaCsvData.SetAt( 6, CSV_162_ITEM_06_NEW);
						csaCsvData.SetAt( 7, CSV_162_ITEM_07_NEW);
					}
					else	{
						csaCsvData.SetAt( 4, CSV_162_ITEM_04);
						csaCsvData.SetAt( 5, CSV_162_ITEM_05);
						csaCsvData.SetAt( 6, CSV_162_ITEM_06);
						csaCsvData.SetAt( 7, CSV_162_ITEM_07);
					}
					csaCsvData.SetAt( 8, CSV_162_ITEM_08);
				}
// 登録番号（法人番号）_23/11/21 cor <--
				break;
// midori 170504 add -->
			case ID_FORMNO_171:		// ⑯－２：雑損益の内訳書
			case ID_FORMNO_172:
			case ID_FORMNO_173:
			case ID_FORMNO_174:
			case ID_FORMNO_175:
			case ID_FORMNO_176:
			case ID_FORMNO_177:
			case ID_FORMNO_178:
			case ID_FORMNO_179:
			case ID_FORMNO_1710:
			case ID_FORMNO_1711:
			case ID_FORMNO_1712:
			case ID_FORMNO_1713:
			case ID_FORMNO_1714:
			case ID_FORMNO_1715:
			case ID_FORMNO_1716:
			case ID_FORMNO_1717:
			case ID_FORMNO_1718:
			case ID_FORMNO_1719:
			case ID_FORMNO_1720:
				pUcTbl = (CimpUc000Common*)(new CimpUc171Sonota1(pUcInfSub->m_FormSeq,pUcInfSub->m_FormOrder));
// 登録番号（法人番号）_23/11/21 cor -->
				//csaCsvData.SetSize(CSV_171_ITEM_CNT);
				//csaCsvData.SetAt( 1, CSV_171_ITEM_01);
				//csaCsvData.SetAt( 2, CSV_171_ITEM_02);
				//csaCsvData.SetAt( 3, CSV_171_ITEM_03);
				//csaCsvData.SetAt( 4, CSV_171_ITEM_04);
				//csaCsvData.SetAt( 5, CSV_171_ITEM_05);
				//csaCsvData.SetAt( 6, CSV_171_ITEM_06);
				//csaCsvData.SetAt( 7, CSV_171_ITEM_07);
				//csaCsvData.SetAt( 8, CSV_171_ITEM_08);
// -------------------------------------
// 24/02/19_ScoreLink cor -->
				//if(bG_InvNo == TRUE)	{
// --------------------------
				if(bG_InvNo == TRUE || bG_Sl == TRUE)	{
// 24/02/19_ScoreLink cor <--
					csaCsvData.SetSize(CSV_171_ITEM_CNT+1);
					csaCsvData.SetAt( 1, CSV_171_ITEM_01);
					csaCsvData.SetAt( 2, CSV_171_ITEM_09);
					csaCsvData.SetAt( 3, CSV_171_ITEM_02);
					csaCsvData.SetAt( 4, CSV_171_ITEM_03);
					csaCsvData.SetAt( 5, CSV_171_ITEM_04);
					csaCsvData.SetAt( 6, CSV_171_ITEM_05);
					csaCsvData.SetAt( 7, CSV_171_ITEM_06);
					csaCsvData.SetAt( 8, CSV_171_ITEM_07);
					csaCsvData.SetAt( 9, CSV_171_ITEM_08);
				}
				else	{
					csaCsvData.SetSize(CSV_171_ITEM_CNT);
					csaCsvData.SetAt( 1, CSV_171_ITEM_01);
					csaCsvData.SetAt( 2, CSV_171_ITEM_02);
					csaCsvData.SetAt( 3, CSV_171_ITEM_03);
					csaCsvData.SetAt( 4, CSV_171_ITEM_04);
					csaCsvData.SetAt( 5, CSV_171_ITEM_05);
					csaCsvData.SetAt( 6, CSV_171_ITEM_06);
					csaCsvData.SetAt( 7, CSV_171_ITEM_07);
					csaCsvData.SetAt( 8, CSV_171_ITEM_08);
				}
// 登録番号（法人番号）_23/11/21 cor <--
				break;
			case ID_FORMNO_181:
			case ID_FORMNO_182:
			case ID_FORMNO_183:
			case ID_FORMNO_184:
			case ID_FORMNO_185:
			case ID_FORMNO_186:
			case ID_FORMNO_187:
			case ID_FORMNO_188:
			case ID_FORMNO_189:
			case ID_FORMNO_1810:
			case ID_FORMNO_1811:
			case ID_FORMNO_1812:
			case ID_FORMNO_1813:
			case ID_FORMNO_1814:
			case ID_FORMNO_1815:
			case ID_FORMNO_1816:
			case ID_FORMNO_1817:
			case ID_FORMNO_1818:
			case ID_FORMNO_1819:
			case ID_FORMNO_1820:
				csv_181_item_02.Empty();
				csv_181_item_03.Empty();
				if ( mfcRec.Init( pUcInfSub->m_FormSeq ) != DB_ERR_OK ){
					// プログレスバーを隠す
					icsPrgBar = HideProgress( icsPrgBar, m_pWnd, 0, SHEET_NUM, ID_EXPORT );
					// エラーメッセージを表示
					(void)imp->ShowMessageBox(IMP_ERRCODE_DB, m_pWnd);
					// CSV操作オブジェクトを閉じる
					csvUcFile.Close();
					// 各オブジェクトの解放
					if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
					if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
					if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
					// エラーを返す
					return IMP_ERRCODE_DB;
				}
				//	レコードあり？
				if ( !mfcRec.IsEOF() ){
					//	先頭に移動
					mfcRec.MoveFirst();
					csv_181_item_02 = mfcRec.m_ItemName + _T("１");
					csv_181_item_03 = mfcRec.m_ItemName + _T("２");
				}
				//	閉じる
				mfcRec.Fin();

				pUcTbl = (CimpUc000Common*)(new CimpUc181Sonota2(pUcInfSub->m_FormSeq,pUcInfSub->m_FormOrder,csv_181_item_02,csv_181_item_03));
				csaCsvData.SetSize(CSV_181_ITEM_CNT);
				csaCsvData.SetAt( 1, CSV_181_ITEM_01);
				csaCsvData.SetAt( 2, csv_181_item_02);
				csaCsvData.SetAt( 3, csv_181_item_03);
				csaCsvData.SetAt( 4, CSV_181_ITEM_04);
				csaCsvData.SetAt( 5, CSV_181_ITEM_05);
				csaCsvData.SetAt( 6, CSV_181_ITEM_06);
				break;
// midori 170504 add <--
			default :
				pUcTbl = 0;
				break;
		}
		csaCsvData.SetAt(0, CSV_ID_ITEM);	// 識別子のセット
		// CSV情報の初期化をする
		ZeroMemory(&pUcTbl->m_uImpCsvInfo, sizeof(pUcTbl->m_uImpCsvInfo));
		pUcTbl->m_uImpCsvInfo.fImport = FALSE;
		// ファイル名/帳票名を設定する
		//_tcscpy_s (pUcTbl->m_uImpCsvInfo.szFileName , sizeof(pUcTbl->m_uImpCsvInfo.szFileName ), UcCsvFile[i]  );		
		//_tcscpy_s (pUcTbl->m_uImpCsvInfo.szSheetName, sizeof(pUcTbl->m_uImpCsvInfo.szSheetName), UcSheetName[i]);		// [2009/07/08 Add for Error Message]
		_tcscpy_s (pUcTbl->m_uImpCsvInfo.szFileName , sizeof(pUcTbl->m_uImpCsvInfo.szFileName ), arrImpStct.GetAt(i).FileName );		//<-----20111007
		_tcscpy_s (pUcTbl->m_uImpCsvInfo.szSheetName, sizeof(pUcTbl->m_uImpCsvInfo.szSheetName), arrImpStct.GetAt(i).SheetName );		//<-----20111007

		// 内訳書テーブルのオープン
// 210113_5301_1 add -->
		pUcTbl->m_IsExport = TRUE;
// 210113_5301_1 add <--
		nErrorCode = pUcTbl->Init(pZmSub->m_database);
		if ( nErrorCode != IMP_ERRCODE_SUCCESS ) {
			// プログレスバー終了
			icsPrgBar->EndProgress();
			// 確認メッセージを表示する
			(void)pUcTbl->ShowMessageBox(nErrorCode, m_pWnd);
			// CSV操作オブジェクトを閉じる
			csvUcFile.Close();
			// 各オブジェクトの解放
			if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
			if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
			if ( pUcTbl    != NULL ) { pUcTbl->Fin();    delete pUcTbl;    }	// テーブル操作用オブジェクトの解放
			if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
			// エラーを返す
			return IMP_ERRCODE_DB;
		}

		// 一括集計金額データ準備
		// 一括集計金額データ取得
		nErrorCode = pUcTbl->GetIkkatuData();
		if ( nErrorCode != IMP_ERRCODE_SUCCESS ) {
			// プログレスバー終了
			icsPrgBar->EndProgress();
			// 確認メッセージを表示する
			(void)pUcTbl->ShowMessageBox(nErrorCode, m_pWnd);
			// CSV操作オブジェクトを閉じる
			csvUcFile.Close();
			// 各オブジェクトの解放
			if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
			if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
			if ( pUcTbl    != NULL ) { pUcTbl->Fin();    delete pUcTbl;    }	// テーブル操作用オブジェクトの解放
			if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
			// エラーを返す
			return IMP_ERRCODE_DB;
		}
		// 一括集計金額データ設定
		pUcTbl->SetIkkatuData();
		if ( nErrorCode != IMP_ERRCODE_SUCCESS ) {
			// プログレスバー終了
			icsPrgBar->EndProgress();
			// 確認メッセージを表示する
			(void)pUcTbl->ShowMessageBox(nErrorCode, m_pWnd);
			// CSV操作オブジェクトを閉じる
			csvUcFile.Close();
			// 各オブジェクトの解放
			if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
			if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
			if ( pUcTbl    != NULL ) { pUcTbl->Fin();    delete pUcTbl;    }	// テーブル操作用オブジェクトの解放
			if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
			// エラーを返す
			return IMP_ERRCODE_DB;
		}

		// 内訳書テーブルのデータ取得
		nErrorCode = pUcTbl->GetAllData();
		if ( nErrorCode != IMP_ERRCODE_SUCCESS ) {
			// プログレスバー終了
			icsPrgBar->EndProgress();
			// 確認メッセージを表示する
			(void)pUcTbl->ShowMessageBox(nErrorCode, m_pWnd);
			// CSV操作オブジェクトを閉じる
			csvUcFile.Close();
			// 各オブジェクトの解放
			if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
			if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
			if ( pUcTbl    != NULL ) { pUcTbl->Fin();    delete pUcTbl;    }	// テーブル操作用オブジェクトの解放
			if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
			// エラーを返す
			return IMP_ERRCODE_DB;
		}
		
		//+++++++++++++++++++++
		// 3行目：項目行の書込
		//+++++++++++++++++++++
		// CSVファイルへの書込
		nRet = csvUcFile.CsvWrite(&csaCsvData);
		if ( nRet == FALSE ) {
			// プログレスバーを隠す
			icsPrgBar = HideProgress( icsPrgBar, m_pWnd, 0, SHEET_NUM, ID_EXPORT );
			// 確認メッセージを表示する
			int nRetValue = pUcTbl->ShowMessageBox(IMP_ERRCODE_EXPORT, m_pWnd);
			// CSV操作オブジェクトを閉じる
			csvUcFile.Close();
			// 確認メッセージの戻り値を判断する
			if ( nRetValue == IDYES ) {		// [はい] → 次の帳票処理へ
				// プログレスバーの再表示
				(void)ShowProgress( icsPrgBar, i );
				continue;
			} else {						// [いいえ] → 処理中断
				// CSV操作オブジェクトを閉じる
				csvUcFile.Close();
				// 各オブジェクトの解放
				if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
				if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
				if ( pUcTbl    != NULL ) { pUcTbl->Fin();    delete pUcTbl;    }	// テーブル操作用オブジェクトの解放
				if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
				// エラーを返す
				return IMP_ERRCODE_EXPORT;
			}
		}

		//+++++++++++++++++++++++++++++
		// 4行目以降：明細データの書込
		//+++++++++++++++++++++++++++++
		while ( (nRet = pUcTbl->GetCsvOneLineData(&csaCsvData)) != IMP_REC_EOF ) {	// 1行分のデータ加工
			// エラーコードをクリア
			nErrorCode = IMP_ERRCODE_SUCCESS;
			// 一括集計金額の行の場合
			if( nRet == IMP_REC_IKKATU) {
				// 一括集計金額行を出力する
				int nOutRetValue = pUcTbl->OutIkkatuData(&csvUcFile);
				// エラーの場合
				if ( nOutRetValue != IMP_ERRCODE_SUCCESS) {
					// プログレスバーを隠す
					icsPrgBar = HideProgress( icsPrgBar, m_pWnd, 0, SHEET_NUM, ID_EXPORT );
					// 確認メッセージ表示
					int nRetValue = pUcTbl->ShowMessageBox(nRet, m_pWnd);
					// 確認メッセージの戻り値を判断する
					if ( nRetValue == IDYES ) {		// [はい] → 次行へ
						// プログレスバーの再表示
						(void)ShowProgress( icsPrgBar, i );
						continue;
					} else {						// [いいえ] → 処理中断
						// CSV操作オブジェクトを閉じる
						csvUcFile.Close();
						// 各オブジェクトの解放
						if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
						if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
						if ( pUcTbl    != NULL ) { pUcTbl->Fin();    delete pUcTbl;    }	// テーブル操作用オブジェクトの解放
						if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
						// エラーを返す
						return IMP_ERRCODE_EXPORT;
					}
				} else {	// 正常な場合
					continue;
				}
			} else
			// エクスポート対象外の行？
			if ( nRet == IMP_REC_NG || nRet == IMP_REC_HIDE) { 
				continue; 
			}

			// 1行分のCSVファイル書込
			nRet = csvUcFile.CsvWrite(&csaCsvData);
			// エラー処理
			if ( nRet == FALSE) {
				// プログレスバーを隠す
				icsPrgBar = HideProgress( icsPrgBar, m_pWnd, 0, SHEET_NUM, ID_EXPORT );
				// 確認メッセージを表示する
				int nRetValue = pUcTbl->ShowMessageBox(IMP_ERRCODE_EXPORT, m_pWnd);
				// 確認メッセージの戻り値を判断する
				if ( nRetValue == IDYES ) {		// [はい] → 次の帳票処理へ
					// プログレスバーの再表示
					(void)ShowProgress( icsPrgBar, i );
					continue;
				} else {						// [いいえ] → 処理中断
					// CSV操作オブジェクトを閉じる
					csvUcFile.Close();
					// 各オブジェクトの解放
					if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
					if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
					if ( pUcTbl    != NULL ) { pUcTbl->Fin();    delete pUcTbl;    }	// テーブル操作用オブジェクトの解放
					if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
					// エラーを返す
					return IMP_ERRCODE_EXPORT;
				}
			}
		}

		// 内訳書テーブルの終了
		nRet = pUcTbl->Fin();
		if ( nRet != IMP_ERRCODE_SUCCESS ) {
			// プログレスバー終了
			icsPrgBar->EndProgress();
			// エラーメッセージ表示
			int nRetValue = pUcTbl->ShowMessageBox(IMP_ERRCODE_DB, m_pWnd);
			// CSV操作オブジェクトを閉じる
			csvUcFile.Close();
			// 各オブジェクトの解放
			if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
			if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
			if ( pUcTbl    != NULL ) { pUcTbl->Fin();    delete pUcTbl;    }	// テーブル操作用オブジェクトの解放
			if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
			// エラーを返す
			return IMP_ERRCODE_DB;
		}

		// テーブル操作用オブジェクトを解放する
		if ( pUcTbl != NULL ) { delete pUcTbl; }


		//##################
		//### Ext. Block ###
		//##################
		// 欄外登録の処理
		int FormNo = i + 1;
// 210113_5301_1 del -->
		//if ( (FormNo == ID_FORMNO_051) || (FormNo == ID_FORMNO_091) ) {
// 210113_5301_1 del <--
// 210113_5301_1 add -->
// 24/02/21_ScoreLink cor -->
//// 24/02/19_ScoreLink cor -->
//		//if ( (FormNo == ID_FORMNO_051 && bG_Kanso != TRUE) || (FormNo == ID_FORMNO_091) ) {
//// --------------------------
//		if( (FormNo == ID_FORMNO_051 && (bG_Kanso != TRUE || bG_Sl == TRUE)) || 
//			(FormNo == ID_FORMNO_091) ) {
//// 24/02/19_ScoreLink cor <--
// --------------------------
		if ( (FormNo == ID_FORMNO_051 && bG_Kanso != TRUE) || (FormNo == ID_FORMNO_091) ) {
// 24/02/21_ScoreLink cor <--
// 210113_5301_1 add <--
			switch ( FormNo ) {
				case ID_FORMNO_051 :	// ⑤棚卸資産の内訳書[下欄]
					pUcTbl = (CimpUc000Common*)(new CimpUc052Tanaoroshi2());
					csaCsvData.SetSize(CSV_052_ITEM_CNT);
					csaCsvData.SetAt( 1, CSV_052_ITEM_01);
					csaCsvData.SetAt( 2, CSV_052_ITEM_02);
					csaCsvData.SetAt( 3, CSV_052_ITEM_03);
					break;
				case ID_FORMNO_091 :	// ⑨買掛金の内訳書[下欄]
					pUcTbl = (CimpUc000Common*)(new CimpUc092Kaikakekin2());
					csaCsvData.SetSize(CSV_092_ITEM_CNT);
					csaCsvData.SetAt( 1, CSV_092_ITEM_01);
					csaCsvData.SetAt( 2, CSV_092_ITEM_02);
					break;
			}
			csaCsvData.SetAt( 0, CSV_ID_EXTRA);

			// 欄外開始行のCSVファイル書込
			nRet = csvUcFile.CsvWrite(&csaCsvData);
			// エラー処理
			if ( nRet == FALSE) {
				// プログレスバーを隠す
				icsPrgBar = HideProgress( icsPrgBar, m_pWnd, 0, SHEET_NUM, ID_EXPORT );
				// 確認メッセージ表示
				int nRetValue = pUcTbl->ShowMessageBox(IMP_ERRCODE_EXPORT, m_pWnd);
				// 確認メッセージの戻り値を判断する
				if ( nRetValue == IDYES ) {			// [はい] → 次の帳票処理へ
					// プログレスバーの再表示
					(void)ShowProgress( icsPrgBar, i );
					continue;
				} else {							// [いいえ] → 処理中断
					// CSV操作オブジェクトを閉じる
					csvUcFile.Close();
					// 各オブジェクトの解放
					if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
					if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
					if ( pUcTbl    != NULL ) { pUcTbl->Fin();    delete pUcTbl;    }	// テーブル操作用オブジェクトの解放
					if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
					// エラーを返す
					return IMP_ERRCODE_EXPORT;
				}
			}

			// 内訳書テーブルのオープン
			nRet = pUcTbl->Init(pZmSub->m_database);
			if ( nRet != IMP_ERRCODE_SUCCESS ) {
				// プログレスバー終了
				icsPrgBar->EndProgress();
				// 確認メッセージを表示する
				int nRetValue = pUcTbl->ShowMessageBox(nErrorCode, m_pWnd);
				// CSV操作オブジェクトを閉じる
				csvUcFile.Close();
				// 各オブジェクトの解放
				if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
				if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
				if ( pUcTbl    != NULL ) { pUcTbl->Fin();    delete pUcTbl;    }	// テーブル操作用オブジェクトの解放
				if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
				// エラーを返す
				return IMP_ERRCODE_DB;
			}
			
			// 内訳書テーブルの準備
			nRet = pUcTbl->GetAllData();
			if ( nRet != IMP_ERRCODE_SUCCESS ) {
				// プログレスバー終了
				icsPrgBar->EndProgress();
				// 確認メッセージを表示する
				int nRetValue = pUcTbl->ShowMessageBox(nErrorCode, m_pWnd);
				// CSV操作オブジェクトを閉じる
				csvUcFile.Close();
				// 各オブジェクトの解放
				if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
				if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
				if ( pUcTbl    != NULL ) { pUcTbl->Fin();    delete pUcTbl;    }	// テーブル操作用オブジェクトの解放
				if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
				// エラーを返す
				return IMP_ERRCODE_DB;
			}

			// ファイル名/帳票名を設定する
			//_tcscpy_s (pUcTbl->m_uImpCsvInfo.szFileName , sizeof(pUcTbl->m_uImpCsvInfo.szFileName ), UcCsvFile[i]  );
			//_tcscpy_s (pUcTbl->m_uImpCsvInfo.szSheetName, sizeof(pUcTbl->m_uImpCsvInfo.szSheetName), UcSheetName[i]);		// [2009/07/08 Add for Error Message]
			_tcscpy_s (pUcTbl->m_uImpCsvInfo.szFileName , sizeof(pUcTbl->m_uImpCsvInfo.szFileName ), arrImpStct.GetAt(i).FileName  );		//<-----20111007
			_tcscpy_s (pUcTbl->m_uImpCsvInfo.szSheetName, sizeof(pUcTbl->m_uImpCsvInfo.szSheetName), arrImpStct.GetAt(i).SheetName );		//<-----20111007

			// 明細データのCSVファイル書込
			while ( (nRet = pUcTbl->GetCsvOneLineData(&csaCsvData)) != IMP_REC_EOF ) {	// 1行分のデータ加工
				// エクスポート対象外？
				if ( nRet != IMP_REC_OK ) { continue; }
				// CSVファイルへの書込
				nRet = csvUcFile.CsvWrite(&csaCsvData);
				// エラー処理
				if ( nRet == FALSE) {
					// プログレスバーを隠す
					icsPrgBar = HideProgress( icsPrgBar, m_pWnd, 0, SHEET_NUM, ID_EXPORT );
					// 確認メッセージ表示
					int nRetValue = pUcTbl->ShowMessageBox(IMP_ERRCODE_EXPORT, m_pWnd);
					// 確認メッセージの戻り値を判断する
					if ( nRetValue == IDYES ) {		// [はい] → 次の帳票処理へ
						// プログレスバーの再表示
						(void)ShowProgress( icsPrgBar, i );
						continue;
					} else {						// [いいえ] → 処理中断
						// CSV操作オブジェクトを閉じる
						csvUcFile.Close();
						// 各オブジェクトの解放
						if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
						if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
						if ( pUcTbl    != NULL ) { pUcTbl->Fin();    delete pUcTbl;    }	// テーブル操作用オブジェクトの解放
						if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
						// エラーを返す
						return IMP_ERRCODE_EXPORT;
					}
				}
			}

			// 内訳書テーブルの終了
			nRet = pUcTbl->Fin();
			if ( nErrorCode != IMP_ERRCODE_SUCCESS ) {
				// プログレスバー終了
				icsPrgBar->EndProgress();
				// エラーメッセージ表示
				(void)imp->ShowMessageBox(IMP_ERRCODE_DB, m_pWnd);
				// CSV操作オブジェクトを閉じる
				csvUcFile.Close();
				// 各オブジェクトの解放
				if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
				if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
				if ( pUcTbl    != NULL ) { pUcTbl->Fin();    delete pUcTbl;    }	// テーブル操作用オブジェクトの解放
				if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放
				// エラーを返す
				return IMP_ERRCODE_DB;
			}

			// テーブル操作用オブジェクトを解放する
			if ( pUcTbl != NULL ) { delete pUcTbl; }
		}

		// CSV操作オブジェクトを閉じる
		csvUcFile.Close();
		// プログレスバーの再表示
		(void)UpdateProgress( icsPrgBar, i );
	}

	// プログレスバー終了処理
	icsPrgBar->EndProgress();
	// 確認メッセージを表示する
// 24/01/16_ScoreLink cor -->
	//int nRetValue = imp->ShowMessageBox(IMP_ERRCODE_SUCCESS, m_pWnd);
// --------------------------
	if(strlen(scid) == 0)	{
		imp->ShowMessageBox(IMP_ERRCODE_SUCCESS,m_pWnd);
	}
// 24/01/16_ScoreLink cor <--
	// 各オブジェクトの解放
	if ( imp       != NULL ) { imp->Fin();       delete imp;       }	// メッセージ用オブジェクトの解放
	if ( pUcInfSub != NULL ) { pUcInfSub->Fin(); delete pUcInfSub; }	// 帳票情報テーブル操作用オブジェクトの解放
	if ( icsPrgBar != NULL ) {                   delete icsPrgBar; }	// プログレスバーオブジェクトの解放

	return nErrorCode;
}



//**********************************************************************
// フォルダ検索
// 
//	引数
//			strFolderPath			[IN] 検索対象のフォルダパス
// 
//	戻値
//			TRUE ：フォルダあり
//			FALSE：フォルダなし
//**********************************************************************
BOOL CDBUchiImp::SearchFolder(CString strFolderPath)
{
	DWORD		dwAttributes	= 0;			// フォルダの属性

	// 指定されたフォルダの属性を取得
	dwAttributes = ::GetFileAttributes(strFolderPath);

	// フォルダが存在しない
	if (dwAttributes == -1) {
		return FALSE;
	}

	// フォルダでない
	if ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
		return FALSE;
	}

	// フォルダあり
	return TRUE;
}

//**********************************************************************
// INIファイルからフォルダパスを取得
//		・C:\ICSWin\Cmds\GR1b\JOBS50 に格納されているINIファイルの情報を読み込む。
//		・INIファイルのデータ読み込みに失敗してもエラー処理しない。
// 
//	引数
//			fImport					[IN] 取得対象
//										TRUE ：インポート元フォルダ
//										FALSE：エクスポート先フォルダ
// 
//	戻値
//			フォルダパス
//**********************************************************************
CString CDBUchiImp::GetFolderPath(BOOL fImport)
{
	DWORD		dwRet			= 0;			// 戻り値（取得したデータのサイズ）
	CString		strKey			= _T("");		// INIキー
	CString		strIniPath		= _T("");		// INIファイルパス
	CString		strTmp			= _T("");		// INIファイル作成予備
	char		szFolderPath[MAX_PATH];			// フォルダパスバッファ

	// 初期化
	ZeroMemory(&szFolderPath, sizeof(szFolderPath));

	// INIキーを取得
	strKey = GetIniKey(fImport);

	// INIファイルパスの取得
//	strIniPath.Format(_T("%s\\%s"), IMP_INI_FOLDERPATH, IMP_INI_FILENAME);
	strTmp.Format( IMP_INI_FOLDERPATH, GetGrNo() );
	strIniPath.Format(_T("%s\\%s"), GetIcswinDrive() + strTmp, IMP_INI_FILENAME);		// [2009/07/27 Change for ICSWin Drive]

	// 文字列の取得　
	dwRet = GetPrivateProfileString(	IMP_INI_SECTION_SYSTEM,	// [IN] セクション
										strKey,					// [IN] キー
										NULL,					// [IN] デフォルトデータ
										szFolderPath,			// [OUT]データ
										sizeof(szFolderPath),	// [IN] データサイズ
										strIniPath);			// [IN] ファイル

	// デバッグ用
	TRACE(_T("GetPrivateProfileString=%d\n"), dwRet);

	return (CString)szFolderPath;
}

//**********************************************************************
// INIファイルにフォルダパスを保存
//		・INIファイルは、C:\ICSWin\Cmds\GR1b\JOBS50 に格納する。
//		・INIファイルへのデータ書き込みに失敗してもエラー処理しない。
// 
//	引数
//			fImport					[IN] 保存対象
//										TRUE ：インポート元フォルダ
//										FALSE：エクスポート先フォルダ
//			strFolderPath			[IN] フォルダパス
// 
//	戻値
//			なし
//**********************************************************************
void CDBUchiImp::SaveFolderPath(BOOL fImport, CString strFolderPath)
{
	BOOL		bRet			= FALSE;		// 戻り値（TRUE=成功, FALSE=失敗）
	CString		strIniKey		= _T("");		// INIキー
	CString		strIniPath		= _T("");		// INIファイルパス
	CString		strTmp			= _T("");		// INIファイル作成予備

	// IINIキーを取得
	strIniKey = GetIniKey(fImport);

	// INIファイルパスの取得
//	strIniPath.Format(_T("%s\\%s"), IMP_INI_FOLDERPATH, IMP_INI_FILENAME);
	strTmp.Format( IMP_INI_FOLDERPATH, GetGrNo() );
	strIniPath.Format(_T("%s\\%s"), GetIcswinDrive() + strTmp, IMP_INI_FILENAME);		// [2009/07/27 Change for ICSWin Drive]

	// 文字列の保存
	bRet = WritePrivateProfileString(	IMP_INI_SECTION_SYSTEM,	// [IN] セクション
										strIniKey,				// [IN] キー
										strFolderPath,			// [IN] データ
										strIniPath);			// [IN] ファイル

	// デバッグ用
	TRACE(_T("WritePrivateProfileString=%d\n"), bRet);
}

//**********************************************************************
//（サブ関数）
// INIファイルのキーを取得
// 
//	引数
//			fImport					[IN] 対象
//										TRUE ：インポート
//										FALSE：エクスポート
//			strFolderPath			[IN] フォルダパス
// 
//	戻値
//			INIキー
//**********************************************************************
CString CDBUchiImp::GetIniKey(BOOL fImport)
{
	CString		strKey			= _T("");		// INIキー

	// キー判定
	if (fImport == TRUE) {
		// インポート
		strKey = IMP_INI_KEY_IMPORT_FOLDER;
	}
	else {
		// エクスポート
		strKey = IMP_INI_KEY_EXPORT_FOLDER; 
	}

	return strKey;
}

//**********************************************************************
//（サブ関数）
// ICSWinフォルダのあるドライブ取得
// 
//	引数
//			 
//	戻値
//			ドライブレター
//				例) Cドライブ → "C:"
//**********************************************************************
CString CDBUchiImp::GetIcswinDrive(void)			// [2009/07/27 Add]
{
	char		cDrv[3];
	CString		retDrv;
#ifdef _DEBUG
	ICSMon *mon;
	mon = new ICSMon( GetCurrentProcessId() );
#endif

// midori 161113 del -->
//	(void)PCOM_GetString( "ProgramDevice", cDrv );
// midori 161113 del <--
// midori 161113 add -->
	(void)GCOM_GetString( "SystemDrive", cDrv );
// midori 161113 add <--
	retDrv = cDrv;
	retDrv = retDrv + ":";
	
#ifdef _DEBUG
	delete mon;
#endif

	return retDrv;
}

//**********************************************************************
//（サブ関数）
// 現在のGrNoを取得する
// 
//	引数	0:大文字／1:小文字（デフォルトで大文字）
//			 
//	戻値	"1B", "1f"...
//**********************************************************************
CString CDBUchiImp::GetGrNo(BOOL strType)
{
#ifdef _DEBUG
	ICSMon *mon;
	mon = new ICSMon( GetCurrentProcessId() );
#endif

	char			szGrNum[16];
	CString			strGrNum, strBuff;

	memset( szGrNum, '\0', sizeof(szGrNum) );		// バッファ初期化

	(void)PCOM_GetString( _T( "ExecGrJobs" ), szGrNum );
	strBuff.Format( "%s", szGrNum );

	if( strType == 1 ){
		strGrNum = strBuff.MakeLower();					// 大文字→小文字変換
	}
	else{
		strGrNum = strBuff;
	}

#ifdef _DEBUG
	delete mon;
#endif

	return strGrNum;
}

//**********************************************************************
//（サブ関数）
// プログレスバーの初期化
// 
//	引数
//			pParent					[IN] 親ウィンドウのポインタ
//			start					[IN] プログレスバーの範囲指定(開始)
//			end						[IN] プログレスバーの範囲指定(終了)
//			imp						[IN] インポート/エクスポートフラグ
//											TRUE ：インポート
//											FALSE：エクスポート
//	戻値
//			ICSProgressWndクラスのポインタ
//**********************************************************************
ICSProgressWnd* CDBUchiImp::InitProgress( CWnd* pParent, int start, int end, BOOL imp )
{
	CString		strTitle		= _T("");		// タイトル
	CString		strText			= _T("");		// テキスト

	ICSProgressWnd* pPrgWnd;
	pPrgWnd = new ICSProgressWnd(pParent);
	pPrgWnd->SetParent(pParent);					// 親ウィンド設定
	if ( imp == ID_IMPORT ) {						// 処理中のウィンドウタイトル＆テキスト設定
		strTitle	= IMP_DLGMSG_IMPORT_TITLE;
		strText.Format(_T("%s\r\n%s"), IMP_PRGMSG_IMPORT, IMP_PRGMSG_WAIT);
	} else {
		strTitle	= IMP_DLGMSG_EXPORT_TITLE;
		strText.Format(_T("%s\r\n%s"), IMP_PRGMSG_EXPORT, IMP_PRGMSG_WAIT);
	}
	pPrgWnd->SetTitle(strTitle);					// 処理のタイトル
	pPrgWnd->SetText(strText);						// 処理中のウィンドウ表示テキスト設定
	pPrgWnd->SetProgressRange(start, end);			// 処理中のプログレスバー範囲設定
	return pPrgWnd;
}

//**********************************************************************
//（サブ関数）
// プログレスバーを隠す
// 		・EndProgress()を実行すると、設定値が全て消えてしまい、
//		  そのままBeginProgress()を実行しても再表示ができない為、
//		  EndProgress()後に再設定を行う様にして対応
//		・さらに、再表示するには、ICSProgressWndクラスのコンストラクタの
//		  処理を実行する必要がある為、new を利用してコンストラクタが実行
//		  される様にしている
//
//	引数
//			pPrgWnd					[IN] ICSProgressWndのポインタ
//			pParent					[IN] 親ウィンドウのポインタ
//			start					[IN] プログレスバーの範囲指定(開始)
//			end						[IN] プログレスバーの範囲指定(終了)
//			imp						[IN] インポート/エクスポートフラグ
//											TRUE ：インポート
//											FALSE：エクスポート
//	戻値
//			INIキー
//**********************************************************************
ICSProgressWnd* CDBUchiImp::HideProgress( ICSProgressWnd* pPrgWnd, CWnd* pParent, int start, int end, BOOL imp )
{
	pPrgWnd->EndProgress();
	if ( pPrgWnd != NULL)  { delete pPrgWnd; }
	return InitProgress( pParent, start, end, imp );
}

//**********************************************************************
//（サブ関数）
// プログレスバーを指定位置まで進める
//		・プログレスバーの位置指定をして再表示する
//
//	引数
//			pPrgWnd					[IN] ICSProgressWndのポインタ
//			pos						[IN] プログレスバーの位置
//	戻値
//			BeginProgress()の戻り値
//**********************************************************************
void CDBUchiImp::UpdateProgress( ICSProgressWnd* pPrgWnd, int pos )
{
	pPrgWnd->SetProgressPos( pos );				// プログレスバーの位置設定
	pPrgWnd->Refresh();							// プログレスバーの表示更新
}

//**********************************************************************
//（サブ関数）
// プログレスバーを表示する
//		・指定した位置まで進んだプログレスバーを表示する
//
//	引数
//			pPrgWnd					[IN] ICSProgressWndのポインタ
//			pos						[IN] プログレスバーの位置
//	戻値
//			BeginProgress()の戻り値
//**********************************************************************
int CDBUchiImp::ShowProgress( ICSProgressWnd* pPrgWnd, int pos )
{
	int nRet = pPrgWnd->BeginProgress();		// プログレスバー表示
	UpdateProgress( pPrgWnd, pos );				// プログレスバーを進める
	
	return nRet;
}
#ifdef _MANAGED
#pragma managed(pop)
#endif

// midori 161103 add -->
//**********************************************************************
//（更新関数）
// 前年度からの更新処理を行う
//
//	引数
//			pWnd					[IN] ウインドウハンドル
// midori 153384 del -->
//// midori 152377 del -->
////			pKakutei				[IN] 「更新後前年度のマスターを確定とする」のサイン
//// midori 152377 del <--
//// midori 152377 add -->
////			pKakutei				[IN] D0 :「更新後前年度のマスターを確定とする」のサイン
////									     D1 : 内訳書データを更新するかどうかのサイン
//// midori 152377 add <--
// midori 153384 del <--
// midori 153384 add -->
//			pKakutei				[IN] D0 :「更新後前年度のマスターを確定とする」のサイン
//										 D1 : 内訳書データを更新するかどうかのサイン
//										 D2 : 表紙情報を更新するかどうかのサイン
// midori 153384 add <--
//	戻値
//			TRUE:正常終了	FALSE:エラー
//**********************************************************************
BOOL ZenUpdate( CWnd* pWnd, int pKakutei )
{
	BOOL		brv=FALSE;

	// エラーメッセージのクリア
	sG_Err = _T("");

	// 変数の初期化
	pG_Mctl = NULL;
	pG_Tzms = NULL;
	pG_Zzms = NULL;

	// 開始処理
	if(ZenUpdateIniz() == FALSE)	{
		return(FALSE);
	}

	// メイン
// midori 152377 del -->
//	brv = ZenUpdateMain(pKakutei);
// midori 152377 del <--
// midori 152377 add -->
	// 勘定科目内訳書を更新するがON
	if((pKakutei & 0x02) == 0)	{
// midori 153384 del -->
//		brv = ZenUpdateMain(pKakutei & 0x01);
// midori 153384 del <--
// midori 153384 add -->
		brv = ZenUpdateMain((pKakutei & 0x01), (pKakutei & 0x04));
// midori 153384 add <--
	}
	// 勘定科目内訳書を更新するがOFF
	else	{
		brv = ZenUpdateMain2(pKakutei & 0x01);
	}
// midori 152377 add <--

	// 終了処理
	ZenUpdateFinz();

	return(brv);
}

// -------------------------------------------------------------------------------------------------
// 開始処理
// -------------------------------------------------------------------------------------------------
BOOL ZenUpdateIniz( void )
{
	// 選択している過年度マスターの情報を取得
	PCOM_GetString(_T("SELECTCODSN"),sG_Svr,sG_Apno,sG_Ccod,sG_Date);

	// マスター管理クラス
	pG_Mctl = new ICSMCTL;
	if(pG_Mctl->SetDatabase(sG_Svr) < 0)	{
		sG_Err = _T("データベースの初期化に失敗しました。");
		return(FALSE);
	}

	// 財務クラス
	pG_Tzms = new CDBZmSub;
	pG_Zzms = new CDBZmSub;

	return(TRUE);
}

/**********************************************************************
	ExecuteSQLWork()
		SQL文を実行する 補助メソッド
		INSERT, UPDATE, DELETE などのSQL文を実行するための補助メソッド

	引数
			CDatabase	pdb			データベースハンドル
			CString		inStrSql	実行する SQL文
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int ExecuteSQLWork(CDatabase* pdb,CString inStrSql)
{
	int			retVal = DB_ERR;

	TRY
	{
		pdb->ExecuteSQL(inStrSql);
		retVal = DB_ERR_OK;						//成功
	}
	CATCH(CDBException, e)
	{
		// エラー コードは e->m_nRetCode に入ります。
		retVal = DB_ERR_EXESQL;					// ExecuteSQL() 失敗
	}

	END_CATCH

	return retVal;
}

// midori 153384 del -->
//// -------------------------------------------------------------------------------------------------
//// メイン
//// -------------------------------------------------------------------------------------------------
//BOOL ZenUpdateMain( int pKakutei )
// midori 153384 del <--
// midori 153384 add -->
// -------------------------------------------------------------------------------------------------
// メイン
// 【引数】	int		「更新後前年度のマスターを確定とする」のサイン
//			int		表紙情報を更新するかどうかのサイン
// -------------------------------------------------------------------------------------------------
BOOL ZenUpdateMain(int pKakutei, int pKakutei2)
// midori 153384 add <--
{
	int				rv=0;
	int				ii=0;
	int				ndate=0;
	int				zver=0;						// 勘定科目内訳書バージョン（前期）
// midori 156188_3 add -->
	int				tupsw=0;
	int				zupsw=0;
// midori 156188_3 add <--
	BYTE			tl_col=0;					// 確定（転送済）仕訳 修正許可 sw
	CString			strTblName=_T("");
	CString			strTempUtiwakeTbl=_T("");	// 一時テーブル名
	CString			strSQL=_T("");

	CString			wmsg=_T("");
	ZMGEN_CHKINFO	chkinfo={0};
	CO_CTL_DB		coctl;
	CO_CTL_Array	coary;
	CZmGen8			zg;
	CdbIcsVtbl*		vtbl = NULL;

	// --------------------------------------------------------------------------------------
	// マスター管理テーブルから当年度の情報を取得する
	// --------------------------------------------------------------------------------------
	// オープン
	if(pG_Mctl->vctl_open(MODE_READ,atoi(sG_Apno),0) < 0)	{
		sG_Err = _T("マスター管理テーブルのオープンに失敗しました。");
		return(FALSE);
	}

	// リード
	coctl.c_apno = atoi(sG_Apno);
	int_bcd(coctl.c_cod,atoi(sG_Ccod),4);
	coctl.c_gsno = 0xff;
	if(pG_Mctl->vctl_coinf(&coctl,&coary) < 0)	{
		sG_Err = _T("会社情報の取得に失敗しました。");
		return(FALSE);
	}

	// クローズ
	pG_Mctl->vctl_close();

	// --------------------------------------------------------------------------------------
	// 当年度マスターｄｂに接続
	// --------------------------------------------------------------------------------------
	// ｄｂに接続
	coctl = coary.GetAt(0);
	ndate = bcd_int(&coctl.c_kikan[3],3);
	ndate += (coctl.c_kikan[3] < 0x89) ? 20000000 : 19000000;
// midori UC_0036 del -->
//// midori 190302 del -->
//	//if(pG_Tzms->SetMasterDB(2,sG_Svr,(BYTE)coctl.c_apno,atoi(sG_Ccod),ndate) < 0)	{
//// midori 190302 del <--
//// midori 190302 add -->
//	if(pG_Tzms->SetMasterDB(3,sG_Svr,(BYTE)coctl.c_apno,atoi(sG_Ccod),ndate) < 0)	{
//// midori 190302 add <--
// midori UC_0036 del <--
// midori UC_0036 add -->
	if(pG_Tzms->SetMasterDB(DB_ICS_VERSION,sG_Svr,(BYTE)coctl.c_apno,atoi(sG_Ccod),ndate) < 0)	{
// midori UC_0036 add <--
		sG_Err = pG_Tzms->GetZmErrMsg();
		return(FALSE);
	}

// midori UC_0051 add -->
	chkinfo.verupsub = 0x800;
// midori UC_0051 add <--
	// マスタの状態をチェック
	rv = zg.CheckDBMasterState(pG_Tzms,wmsg,chkinfo);
	if((rv == -1) || (rv == -2))	{
		sG_Err = zg.GetErrorMsg();
		return(FALSE);
	}
	else if(rv == 1)	{
		// 注意喚起のメッセージ表示をして、処理継続
		ICSMessageBox(wmsg);
	}

	// --------------------------------------------------------------------------------------
	// 前年度マスターｄｂに接続
	// --------------------------------------------------------------------------------------
	// ｄｂに接続
// midori UC_0036 del -->
//// midori 190302 del -->
//	//if(pG_Zzms->SetMasterDB(2,sG_Svr,atoi(sG_Apno),atoi(sG_Ccod),atoi(sG_Date)) < 0)	{
//// midori 190302 del <--
//// midori 190302 add -->
//	if(pG_Zzms->SetMasterDB(3,sG_Svr,atoi(sG_Apno),atoi(sG_Ccod),atoi(sG_Date)) < 0)	{
//// midori 190302 add <--
// midori UC_0036 del <--
// midori UC_0036 add -->
	if(pG_Zzms->SetMasterDB(DB_ICS_VERSION,sG_Svr,atoi(sG_Apno),atoi(sG_Ccod),atoi(sG_Date)) < 0)	{
// midori UC_0036 add <--
		sG_Err = pG_Zzms->GetZmErrMsg();
		return(FALSE);
	}

	// マスタの状態チェック
	rv = zg.CheckDBMasterState(pG_Zzms,wmsg,chkinfo);
	if((rv == -1) || (rv == -2))	{
		sG_Err = zg.GetErrorMsg();
		return(FALSE);
	}
	else if(rv == 1)	{
		// 注意喚起のメッセージ表示をして、処理継続
		ICSMessageBox(wmsg);
	}
	if(pG_Zzms->VolumeOpen() != 0)	{
		sG_Err = pG_Zzms->GetZmErrMsg();
		return(FALSE);
	}
	tl_col = pG_Zzms->zvol->tl_cor;
	pG_Zzms->VolumeClose();

	// 前年度、勘定科目内訳書データベース作成チェック
	// vtbl に type = 1001 AND objname='DBUchiwake' のレコードが存在する場合、内訳書関連テーブルが存在するとみなす
	vtbl = new CdbIcsVtbl(pG_Zzms->m_database);

	zver = vtbl->VersionCheck(DB_UC_VTBL_TYPE, DB_UC_VTBL_OBJNAME);

	if (zver < 0) {	// DBエラー関連（Vtblアクセス時）
		if (zver == DB_ERR_EOF) {	// レコードが無い
			// レコードが無い場合、処理を行わない
			return(TRUE);
		}
		else	{
			// エラー
			sG_Err = pG_Zzms->GetZmErrMsg();
			return(FALSE);
		}
	}

	// 一時テーブル名作成
	strTempUtiwakeTbl.Format( _T("##temp_utiwake_tbl"));

	// 様式テーブルの更新
	for(ii=0;ii<39;ii++)	{
		switch(ii)	{
		case	0:
			strTblName = _T("uc_011_yotyokin");
			break;
		case	1:
			strTblName = _T("uc_021_uketoritegata");
			break;
		case	2:
			strTblName = _T("uc_031_urikakekin");
			break;
		case	3:
			strTblName = _T("uc_041_karibarai");
			break;
		case	4:
			strTblName = _T("uc_042_kasituke");
			break;
		case	5:
			strTblName = _T("uc_051_tanaoroshi");
			break;
		case	6:
			strTblName = _T("uc_052_tanaoroshi2");
			break;
		case	7:
			strTblName = _T("uc_061_yuukasyouken");
			break;
		case	8:
			strTblName = _T("uc_071_koteishisan");
			break;
		case	9:
			strTblName = _T("uc_081_siharaitegata");
			break;
		case	10:
			strTblName = _T("uc_091_kaikakekin");
			break;
		case	11:
			strTblName = _T("uc_092_kaikakekin2");
			break;
		case	12:
			strTblName = _T("uc_101_kariukekin");
			break;
		case	13:
			strTblName = _T("uc_102_gensen");
			break;
		case	14:
			strTblName = _T("uc_111_kariirekin");
			break;
		case	15:
			strTblName = _T("uc_121_totiuriage");
			break;
		case	16:
			strTblName = _T("uc_131_uriagedaka");
			break;
		case	17:
			strTblName = _T("uc_141_yakuin");
			break;
		case	18:
			strTblName = _T("uc_142_jinkenhi");
			break;
		case	19:
			strTblName = _T("uc_151_tidaiyatin");
			break;
		case	20:
			strTblName = _T("uc_152_kenrikin");
			break;
		case	21:
			strTblName = _T("uc_153_kougyou");
			break;
		case	22:
			strTblName = _T("uc_161_zatueki");
			break;
		case	23:
			strTblName = _T("uc_162_zatuson");
			break;
		case	24:
			strTblName = _T("uc_171_sonota1");
			break;
		case	25:
			strTblName = _T("uc_181_sonota2");
			break;
		case	26:
			// 金融機関名称テーブル(uc_lst_bank)
			strTblName = _T("uc_lst_bank");
			break;
		case	27:
			// 保管リストテーブル
			strTblName = _T("uc_lst_hokan");
			break;
		case	28:
			// 取引先名称テーブル(uc_lst_address)
			strTblName = _T("uc_lst_address");
			break;
		case	29:
			// 科目名称テーブル(uc_lst_kamoku)
			strTblName = _T("uc_lst_kamoku");
			break;
		case	30:
			// ソート項目情報リストテーブル(uc_lst_item_sort)
			strTblName = _T("uc_lst_item_sort");
			break;
		case	31:
			// 帳表の通常/個別の管理情報(uc_inf_form_type)
			strTblName = _T("uc_inf_form_type");
			break;
		case	32:
			// 帳票ごとの管理情報テーブル(uc_inf_sub)
			strTblName = _T("uc_inf_sub");
			break;
		case	33:
			// 帳票ごとの文字省略の管理情報テーブル(uc_inf_sub_omit)
			strTblName = _T("uc_inf_sub_omit");
			break;
		case	34:
			// その他科目の内訳書②用項目名称の管理テーブル(uc_inf_sonota_item)
			strTblName = _T("uc_inf_sonota_item");
			break;
		case	35:
			// バージョン管理(uc_inf_ver)
			strTblName = _T("uc_inf_ver");
			break;
		case	36:
			// 財務連動の管理情報テーブル(uc_ren_main)
			strTblName = _T("uc_ren_main");
			break;
		case	37:
			// 財務連動科目リストテーブル(uc_ren_kamoku)
			strTblName = _T("uc_ren_kamoku");
			break;
		case	38:
			// 翌期更新の設定情報(uc_inf_yokuki)
			strTblName = _T("uc_inf_yokuki");
			break;
		default:
			break;
		}
		rv  = pG_Zzms->object_check(2,strTblName);
		if(rv != 0)	{
			// 前期のデータを基に一時テーブルを作成
			strSQL.Format(_T("IF object_id('tempdb..%s', 'u') is not null drop table %s "),strTempUtiwakeTbl,strTempUtiwakeTbl);
			strSQL.Format( strSQL + _T("SELECT * INTO %s FROM %s"),strTempUtiwakeTbl,strTblName);
			if(ExecuteSQLWork(pG_Zzms->m_database,strSQL) != DB_ERR_OK)	{
				sG_Err.Format(_T("テーブル(%s)の更新に失敗しました。"),strTblName);
				return(FALSE);
			}
			// 当期のテーブルを削除し、一時テーブルからテーブルを作成
			strSQL.Format(_T("IF object_id('%s', 'u') is not null drop table %s"),strTblName,strTblName);
			if(ExecuteSQLWork(pG_Tzms->m_database,strSQL) != DB_ERR_OK)	{
				sG_Err.Format(_T("テーブル(%s)の削除に失敗しました。"),strTblName);
				return(FALSE);
			}
			strSQL.Format(_T("SELECT * INTO %s FROM %s"),strTblName,strTempUtiwakeTbl);
			if(ExecuteSQLWork(pG_Tzms->m_database,strSQL) != DB_ERR_OK)	{
				sG_Err.Format(_T("テーブル(%s)の作成に失敗しました。"),strTblName);
				return(FALSE);
			}
		}
	}

// midori 190301 add -->
	CdbUcInfMain	mfcMainT(pG_Tzms->m_database);
	CdbUcInfMain	mfcMainZ(pG_Zzms->m_database);
	BOOL			tsw=FALSE;
	BOOL			zsw=FALSE;
	BOOL			td1=FALSE;
	BOOL			td4=FALSE;
	BOOL			zd1=FALSE;
// midori 190301 add <--
// 修正No.168430 add -->
	BOOL			td11=FALSE;
	BOOL			td14=FALSE;
	BOOL			zd11=FALSE;
// 修正No.168430 add <--

	// アプリケーション全体の管理情報テーブル(uc_inf_main)
	rv  = pG_Zzms->object_check(2,_T("uc_inf_main"));
	if(rv != 0)	{
// midori 190301 add -->
		// 簡素化で追加した項目をセーブする
		if(mfcMainT.Init() == DB_ERR_OK)	{
			if(mfcMainT.m_GeneralVar[2] & BIT_D1)	td1=TRUE;
			if(mfcMainT.m_GeneralVar[2] & BIT_D4)	td4=TRUE;
// 修正No.168430 add -->
			if(mfcMainT.m_GeneralVar[4] & BIT_D5)	td11=TRUE;
			if(mfcMainT.m_GeneralVar[4] & BIT_D4)	td14=TRUE;
// 修正No.168430 add <--
			mfcMainT.Fin();
		}
		// 過年度の様式を取得する
		if(mfcMainZ.Init() == DB_ERR_OK)	{
			if(mfcMainZ.m_GeneralVar[2] & BIT_D1)	zd1=TRUE;
// 修正No.168430 add -->
			if(mfcMainZ.m_GeneralVar[4] & BIT_D5)	zd11=TRUE;
// 修正No.168430 add <--
			mfcMainZ.Fin();
		}
// midori 190301 add <--

		// 前期のデータを基に一時テーブルを作成
		strSQL.Format(_T("IF object_id('tempdb..%s', 'u') is not null drop table %s "),strTempUtiwakeTbl,strTempUtiwakeTbl);
		strSQL.Format( strSQL + _T("SELECT * INTO %s FROM uc_inf_main"),strTempUtiwakeTbl);
		if(ExecuteSQLWork(pG_Zzms->m_database,strSQL) != DB_ERR_OK)	{
			sG_Err.Format(_T("テーブル(uc_inf_main)の更新に失敗しました。"));
			return(FALSE);
		}

		// 当期のデータを基に一時テーブルのデータを更新
		strSQL.Format(_T("DECLARE @val binary SET @val = [dbo].[get_byte]((SELECT GeneralVar FROM uc_inf_main), 1) "));
		strSQL.Format(strSQL + _T("UPDATE %s SET %s.ssymd = uc_inf_main.ssymd,%s.eeymd = uc_inf_main.eeymd FROM %s,uc_inf_main  WHERE %s.Seq = uc_inf_main.Seq "),strTempUtiwakeTbl,strTempUtiwakeTbl,strTempUtiwakeTbl,strTempUtiwakeTbl,strTempUtiwakeTbl);
		strSQL.Format(strSQL + _T("IF((@val & 4) > 0) UPDATE %s SET GeneralVar = [dbo].[set_byte2](GeneralVar,1,4,1) "),strTempUtiwakeTbl);
		strSQL.Format(strSQL + _T("ELSE UPDATE %s SET GeneralVar = [dbo].[set_byte2](GeneralVar,1,4,0) "),strTempUtiwakeTbl);
		strSQL.Format(strSQL + _T("IF((@val & 8) > 0) UPDATE %s SET GeneralVar = [dbo].[set_byte2](GeneralVar,1,8,1) "),strTempUtiwakeTbl);
		strSQL.Format(strSQL + _T("ELSE UPDATE %s SET GeneralVar = [dbo].[set_byte2](GeneralVar,1,8,0)"),strTempUtiwakeTbl);
// midori 156188_3 add -->
		// 当期のデータを基に、一時テーブルの下記データを更新
		// 画面表示「科目行」対応後、科目行処理方法変更メッセージ表示サイン
		// 画面表示「科目行」対応後、科目行挿入済みサイン
		strSQL = strSQL + _T("SET @val = [dbo].[get_byte]((SELECT GeneralVar FROM uc_inf_main), 3) ");
		strSQL = strSQL + _T("IF((@val & 32) > 0) UPDATE ") + strTempUtiwakeTbl + _T(" SET GeneralVar = [dbo].[set_byte2](GeneralVar,3,32,1) ");
		strSQL = strSQL + _T("ELSE UPDATE ") + strTempUtiwakeTbl + _T(" SET GeneralVar = [dbo].[set_byte2](GeneralVar,3,32,0) ");
		strSQL = strSQL + _T("IF((@val & 64) > 0) UPDATE ") + strTempUtiwakeTbl + _T(" SET GeneralVar = [dbo].[set_byte2](GeneralVar,3,64,1) ");
		strSQL = strSQL + _T("ELSE UPDATE ") + strTempUtiwakeTbl + _T(" SET GeneralVar = [dbo].[set_byte2](GeneralVar,3,64,0) ");
// midori 156188_3 add <--
		if(ExecuteSQLWork(pG_Tzms->m_database,strSQL) != DB_ERR_OK)	{
			sG_Err.Format(_T("テーブル(uc_inf_main)の更新に失敗しました。"));
			return(FALSE);
		}

		// 当期のテーブルを削除し、一時テーブルからテーブルを作成
		strSQL.Format(_T("IF object_id('uc_inf_main', 'u') is not null drop table uc_inf_main"));
		if(ExecuteSQLWork(pG_Tzms->m_database,strSQL) != DB_ERR_OK)	{
			sG_Err.Format(_T("テーブル(uc_inf_main)の削除に失敗しました。"));
			return(FALSE);
		}

		strSQL.Format(_T("SELECT * INTO uc_inf_main FROM %s"),strTempUtiwakeTbl);
		if(ExecuteSQLWork(pG_Tzms->m_database,strSQL) != DB_ERR_OK)	{
			sG_Err.Format(_T("テーブル(uc_inf_main)の削除に失敗しました。"));
			return(FALSE);
		}

// midori 190301 add -->
		// 簡素化で追加した項目を元に戻す
		if(mfcMainT.Init() == DB_ERR_OK)	{
			mfcMainT.Edit();
			if(td1 == TRUE)	mfcMainT.m_GeneralVar[2] |= BIT_D1;
			else			mfcMainT.m_GeneralVar[2] &= ~BIT_D1;
			if(td4 == TRUE)	mfcMainT.m_GeneralVar[2] |= BIT_D4;
			else			mfcMainT.m_GeneralVar[2] &= ~BIT_D4;
// 修正No.168430 add -->
			if(td11 == TRUE)	mfcMainT.m_GeneralVar[4] |= BIT_D5;
			else				mfcMainT.m_GeneralVar[4] &= ~BIT_D5;
			if(td14 == TRUE)	mfcMainT.m_GeneralVar[4] |= BIT_D4;
			else				mfcMainT.m_GeneralVar[4] &= ~BIT_D4;
// 修正No.168430 add <--

			// 過年度と当年度の様式が異なる場合、内訳書起動時にデータの書き換えを行うためサインをたてる
			if(zd1 != td1)	mfcMainT.m_GeneralVar[2] |= BIT_D5;
			else			mfcMainT.m_GeneralVar[2] &= ~BIT_D5;
// 修正No.168430,168500 add -->
			//if(zd11 != td11)	mfcMainT.m_GeneralVar[2] |= BIT_D5;
			//else				mfcMainT.m_GeneralVar[2] &= ~BIT_D5;
			if(zd11 != td11)	mfcMainT.m_GeneralVar[4] |= BIT_D7;
			else				mfcMainT.m_GeneralVar[4] &= ~BIT_D7;
// 修正No.168430,168500 add <--
			mfcMainT.Update();
			mfcMainT.Fin();
		}
// midori 190301 add <--

	}

	// 財務の勘定科目内訳書バージョン情報を変更する。
	// 当期に前年度の内訳書バージョンレコードコピー
	strSQL.Format("UPDATE vtbl SET vers = %d WHERE type=%d AND objname='%s'", zver,DB_UC_VTBL_TYPE, DB_UC_VTBL_OBJNAME );
	if(ExecuteSQLWork(pG_Tzms->m_database,strSQL) != DB_ERR_OK)	{
		sG_Err = _T("テーブル(vtbl)の更新に失敗しました。");
		return(FALSE);
	}

	// アプリケーション全体の管理情報テーブル(uc_inf_main)
	rv  = pG_Zzms->object_check(2,_T("uc_inf_main"));
	if(rv != 0)	{
		if(((tl_col & 0x10) == 0) || (((tl_col & 0x10) == 0x10) && pKakutei == 1))	{
			// 「更新後、前年度のマスターを確定とする」と前年度マスターの「入力データを確定する」のチェックを連動
			if((tl_col & 0x10) == 0)	{
				// 前年度の決算確定、チェック有りの時。
				strSQL.Format(_T("UPDATE uc_inf_main SET GeneralVar = [dbo].[set_byte2](GeneralVar,1,4,1)"));
			}
			else	{
				// 前年度の決算確定、チェック無しの時。
				if(pKakutei == 1)	strSQL.Format(_T("UPDATE uc_inf_main SET GeneralVar = [dbo].[set_byte2](GeneralVar,1,4,1)"));
			}
			if(ExecuteSQLWork(pG_Zzms->m_database,strSQL) != DB_ERR_OK)	{
				sG_Err = _T("テーブル(uc_inf_main)の更新に失敗しました。");
				return(FALSE);
			}
		}
// midori 156188_3 add -->
		// 前年度が画面への科目行挿入対応年度でない（期首が 2019年4月1日より前）
		//if(ImpKamokuRowEnableSgn(pG_Zzms->m_database,0) == 0) {		// 改良No.21-0086,21-0529 del
		if(ImpKamokuRowEnableSgn(pG_Zzms->m_database,0,-1) == 0)	{	// 改良No.21-0086,21-0529 add
			zupsw=1;
		}
// midori 156188_3 add <--
	}

	// 当期の前年度からの更新サインをONにする
	rv  = pG_Tzms->object_check(2,_T("uc_inf_main"));
	if(rv != 0)	{
		strSQL.Format(_T("UPDATE uc_inf_main SET GeneralVar = [dbo].[set_byte2](GeneralVar,1,16,1)"));
		if(ExecuteSQLWork(pG_Tzms->m_database,strSQL) != DB_ERR_OK)	{
			sG_Err = _T("テーブル(uc_inf_main)の更新に失敗しました。");
			return(FALSE);
		}
// midori 156188_3 add -->
		// 当年度が画面への科目行挿入対応年度（期首が 2019年4月1日以降）
		//if(ImpKamokuRowEnableSgn(pG_Tzms->m_database,0) == 1) {		// 改良No.21-0086,21-0529 del
		if(ImpKamokuRowEnableSgn(pG_Tzms->m_database,0,-1) == 1)	{	// 改良No.21-0086,21-0529 add
			tupsw=1;
		}
// midori 156188_3 add <--
	}

// midori 156188_3 add -->
	// 下記条件の場合、マスターの科目行挿入処理を行うため、画面表示「科目行」対応後、科目行挿入済みサインをOFFにする
	// ・前年度の期首が2019年4月1日より前
	// ・当年度の期首が2019年4月1日以降
	if(zupsw == 1 && tupsw == 1) {
		// GeneralVar[3]のBIT_D5(0x20)をOFFにする
		strSQL.Format(_T("UPDATE uc_inf_main SET GeneralVar = [dbo].[set_byte2](GeneralVar,3,64,0)"));
		if(ExecuteSQLWork(pG_Tzms->m_database,strSQL) != DB_ERR_OK)	{
			sG_Err = _T("テーブル(uc_inf_main)の更新に失敗しました。");
			return(FALSE);
		}
	}
// midori 156188_3 add <--

// midori 153384 add -->
	// 表紙情報を更新する
	if(pKakutei2 != 0)	{
		// テーブル名作成
		strTblName.Format(_T("cpdata"));
		// 一時テーブル名作成
		strTempUtiwakeTbl.Format(_T("##temp_cpdata_tbl"));

		// 表紙用データ（会社名、住所上段、住所下段）を前期から当期のデータに上書きします。
		rv  = pG_Zzms->object_check(2,strTblName);
		if(rv != 0)	{
			// 前期のデータを基に一時テーブルを作成
			strSQL.Format(_T("IF object_id('tempdb..%s', 'u') is not null drop table %s "),strTempUtiwakeTbl,strTempUtiwakeTbl);
			strSQL.Format(strSQL + _T("SELECT * INTO %s FROM %s"),strTempUtiwakeTbl,strTblName);
			if(ExecuteSQLWork(pG_Zzms->m_database,strSQL) != DB_ERR_OK)	{
				sG_Err.Format(_T("テーブル(%s)の更新に失敗しました。"),strTblName);
				return(FALSE);
			}
			// 当期のテーブルに住所データを上書き
			strSQL.Format(_T("UPDATE [%s] SET "),strTblName);
			strSQL.Format(strSQL + _T("%s.cpyg=%s.cpyg,"),strTblName,strTempUtiwakeTbl);	// 会社名
			strSQL.Format(strSQL + _T("%s.cpua=%s.cpua,"),strTblName,strTempUtiwakeTbl);	// 住所上段
			strSQL.Format(strSQL + _T("%s.cpda=%s.cpda  "),strTblName,strTempUtiwakeTbl);	// 住所下段
			strSQL.Format(strSQL + _T("FROM %s "),strTblName);
			strSQL.Format(strSQL + _T("INNER JOIN %s "),strTempUtiwakeTbl);
			strSQL.Format(strSQL + _T("ON %s.cpkofs=%s.cpkofs "),strTblName,strTempUtiwakeTbl);
			// 当期の当期レコードに、前期の当期レコードを上書き
			strSQL.Format(strSQL + _T("WHERE %s.cpkofs=0"),strTempUtiwakeTbl);
			if(ExecuteSQLWork(pG_Tzms->m_database,strSQL) != DB_ERR_OK)	{
				sG_Err.Format(_T("テーブル(%s)の更新に失敗しました。"),strTblName);
				return(FALSE);
			}
		}
	}
// midori 153384 add <--

	return(TRUE);
}

// uc_inf_main

// midori 152377 add -->
// -------------------------------------------------------------------------------------------------
// メイン
// -------------------------------------------------------------------------------------------------
BOOL ZenUpdateMain2( int pKakutei )
{
	int				rv=0;
	int				zver=0;						// 勘定科目内訳書バージョン（前期）
// midori UC_0009 add -->
	int				sw=0;
	short			ind=0;
	CRecordset*		prs;
// midori UC_0009 add <--
	BYTE			tl_col=0;					// 確定（転送済）仕訳 修正許可 sw
	CString			strSQL=_T("");
	CString			wmsg=_T("");
// midori UC_0009 add -->
	CString			cs=_T("");
// midori UC_0009 add <--
	ZMGEN_CHKINFO	chkinfo={0};
	CZmGen8			zg;
	CdbIcsVtbl*		vtbl = NULL;

	// --------------------------------------------------------------------------------------
	// 前年度マスターｄｂに接続
	// --------------------------------------------------------------------------------------
	// ｄｂに接続
// midori UC_0036 del -->
//// midori 190302 del -->
//	//if(pG_Zzms->SetMasterDB(2,sG_Svr,atoi(sG_Apno),atoi(sG_Ccod),atoi(sG_Date)) < 0)	{
//// midori 190302 del <--
//// midori 190302 add -->
//	if(pG_Zzms->SetMasterDB(3,sG_Svr,atoi(sG_Apno),atoi(sG_Ccod),atoi(sG_Date)) < 0)	{
//// midori 190302 add <--
// midori UC_0036 del <--
// midori UC_0036 add -->
	if(pG_Zzms->SetMasterDB(DB_ICS_VERSION,sG_Svr,atoi(sG_Apno),atoi(sG_Ccod),atoi(sG_Date)) < 0)	{
// midori UC_0036 add <--
		sG_Err = pG_Zzms->GetZmErrMsg();
		return(FALSE);
	}

// midori UC_0051 add -->
	chkinfo.verupsub  =	0x800;
// midori UC_0051 add <--
	// マスタの状態チェック
	rv = zg.CheckDBMasterState(pG_Zzms,wmsg,chkinfo);
	if((rv == -1) || (rv == -2))	{
		sG_Err = zg.GetErrorMsg();
		return(FALSE);
	}
	else if(rv == 1)	{
		// 注意喚起のメッセージ表示をして、処理継続
		ICSMessageBox(wmsg);
	}
	if(pG_Zzms->VolumeOpen() != 0)	{
		sG_Err = pG_Zzms->GetZmErrMsg();
		return(FALSE);
	}
	tl_col = pG_Zzms->zvol->tl_cor;
	pG_Zzms->VolumeClose();

	// 前年度、勘定科目内訳書データベース作成チェック
	// vtbl に type = 1001 AND objname='DBUchiwake' のレコードが存在する場合、内訳書関連テーブルが存在するとみなす
	vtbl = new CdbIcsVtbl(pG_Zzms->m_database);

	zver = vtbl->VersionCheck(DB_UC_VTBL_TYPE, DB_UC_VTBL_OBJNAME);

	if (zver < 0) {	// DBエラー関連（Vtblアクセス時）
		if (zver == DB_ERR_EOF) {	// レコードが無い
			// レコードが無い場合、処理を行わない
			return(TRUE);
		}
		else	{
			// エラー
			sG_Err = pG_Zzms->GetZmErrMsg();
			return(FALSE);
		}
	}

	// アプリケーション全体の管理情報テーブル(uc_inf_main)
	rv  = pG_Zzms->object_check(2,_T("uc_inf_main"));
// midori UC_0009 del -->
	//if(rv != 0)	{
// midori UC_0009 del <--
// midori UC_0009 add -->
	// 過年度マスターの uc_inf_main に GeneralVar が作成されているかを調べる
	sw = 0;
	strSQL = _T("SELECT COLUMNPROPERTY(OBJECT_ID('uc_inf_main'),'GeneralVar','PRECISION')");
	prs = new CRecordset(pG_Zzms->m_database);
	prs->Open(CRecordset::forwardOnly,strSQL);
	prs->GetFieldValue(ind,cs);
	sw = _tstoi(cs);
	prs->Close();
	delete prs;
	prs=0;
	if(rv != 0 && sw != 0)	{
// midori UC_0009 add <--
		if(((tl_col & 0x10) == 0) || (((tl_col & 0x10) == 0x10) && pKakutei == 1))	{
			// 「更新後、前年度のマスターを確定とする」と前年度マスターの「入力データを確定する」のチェックを連動
			if((tl_col & 0x10) == 0)	{
				// 前年度の決算確定、チェック有りの時。
				strSQL.Format(_T("UPDATE uc_inf_main SET GeneralVar = [dbo].[set_byte2](GeneralVar,1,4,1)"));
			}
			else	{
				// 前年度の決算確定、チェック無しの時。
				if(pKakutei == 1)	strSQL.Format(_T("UPDATE uc_inf_main SET GeneralVar = [dbo].[set_byte2](GeneralVar,1,4,1)"));
			}
			if(ExecuteSQLWork(pG_Zzms->m_database,strSQL) != DB_ERR_OK)	{
				sG_Err = _T("テーブル(uc_inf_main)の更新に失敗しました。");
				return(FALSE);
			}
		}
	}

	return(TRUE);
}
// midori 152377 add <--

// -------------------------------------------------------------------------------------------------
// 終了処理
// -------------------------------------------------------------------------------------------------
void ZenUpdateFinz( void )
{
	// マスター管理クラス
	if(pG_Mctl != NULL)	{
		delete pG_Mctl;
		pG_Mctl = NULL;
	}

	// 財務クラス
	if(pG_Tzms != NULL)	{
		delete pG_Tzms;
		pG_Tzms = NULL;
	}

	if(pG_Zzms != NULL)	{
		delete pG_Zzms;
		pG_Zzms = NULL;
	}

}

// -------------------------------------------------------------------------------------------------
// ScZenUpdate関数で発生したエラーメッセージの取得
// (※呼出し元関数用)
// -------------------------------------------------------------------------------------------------
void ZenGetErrorMsg( CString& pErr )
{
	pErr = sG_Err;
}
// midori 161103 add <--

// midori 170502 add -->
// midori UC_0005 del -->
//void CDBUchiImp::CmnTblUpdatePageRowAllResetMain(CDBZmSub* pZmSub,CdbUcInfSub* pUcInfSub)
// midori UC_0005 del <--
// midori UC_0005 add -->
void CmnTblUpdatePageRowAllResetMain(CDBZmSub* pZmSub,CdbUcInfSub* pUcInfSub)
// midori UC_0005 add <--
{
	int					intOutKei=0;
	int					nameKei=0;
	int					formSeq=0;
	int					formtype=0;
	CCtrlSubGeneralVar	 cs(*pUcInfSub);

	formSeq = pUcInfSub->m_FormSeq;
	intOutKei = pUcInfSub->m_OutKei;
	nameKei = cs.bNameKei;

	formtype = ID_FORMTYPE_KOBETU;
	CmnUcInfFormTypeGetFormType(pZmSub,formSeq,&formtype);

	// 頁と行の調整処理
	CmnTblDeleteFgFunc(pZmSub,ID_FGFUNC_PAGEKEI,formSeq);						// 頁計の削除
	CmnTblDeleteFgFunc(pZmSub,ID_FGFUNC_RUIKEI,formSeq);						// 累計の削除
	CmnTblUpdatePageNumRowNumReset(pZmSub,1,1,formSeq,formtype,intOutKei);		// 先頭頁からページ再設定
	CmnTblUpdateEndPageSetting(pZmSub,formSeq);									// 最終頁を調整する
	CmnTblInsertPageKeiRuiKei(pZmSub,formSeq,formtype,intOutKei,nameKei);		// 頁計/累計挿入（最終ページの削除or空行追加）

	// 小計/中計/頁計/累計の再計算
	CmnTblCalcKei(pZmSub,formSeq,intOutKei,cs.bPrintZeroData,cs.bPrintNullData);
}

// 157153 add -->
int CmnTblUpdatePageNumRowNumReset_Sub(int pFormSeq,CdbUc000Common* pData,CDBZmSub* pZmSub,int pSort1,int pOpOutNullLine,int pOpOutSyokei,int pintFormType,int pintOutKei)
{
	int								sort=0;
	int								intNumPage=0;
	int								intNumRow=0;
	int								nBaseRow=0;
	int								retVal = FUNCTION_NG;
	int								numrow=0;
	int								numpage=0;
	int								fgfunc=0,fgfunc2=0;
	int								ii=0;
	int								sw=0;				//	1:小計行の次に空白を含める
	int								sw2=0;
	int								cnt2=0;				// 第１ソート項目ごとのデータ件数
	int								flg=0;				// 0:空白行を挿入	1:小計NULLを挿入
	BYTE							bytFgSykei = 0x00;
	CString							strItem[2]={_T("")};
	CString							strNewItem[2]={_T("")};
	CString							strfilter=_T("");
	CString							strFieldSykei[2]={_T("")};
	CString							strsort=_T("");
	CdbUcLstItemSort				rsLstIS( pZmSub->m_database );

	// 自動小計フラグ[FgSykei]がONの第１ソート項目が選択されている場合のみ処理を行う
	sort = pSort1;
	if(sort == 0)	{
		// 一度も編集～ソートを開いていない場合は、第１ソート項目の項目番号が書かれていない
		if(pFormSeq == 2)	sort=8;
		else				sort=1;
	}

	if(pFormSeq > 0 && sort > 0) {
		// クエリー実行
		if(rsLstIS.RequeryFormItem(pFormSeq, sort) == DB_ERR_OK) {
			if (!rsLstIS.IsEOF()) {
				// 必要情報をセット
				bytFgSykei = rsLstIS.m_FgSykei;					// 小計の対象である/ない
				strFieldSykei[0]	= rsLstIS.m_FieldSykei;		// 小計のグループ判定に使用するフィールド名
				strFieldSykei[1]	= rsLstIS.m_FieldSykei2;	// 小計のグループ判定に使用するフィールド名
			}
		}
	}


	if(pOpOutNullLine == 1 && bytFgSykei == 1)	{
		sw = 1;
	}
	if(pOpOutSyokei == 0 && bytFgSykei == 1)	{
		sw = 1;
	}
	if(sw == 0)	{
		return(0);
	}

	retVal = FUNCTION_OK;

	// 行番号を振り直すための基準となる行数を取得
	nBaseRow = CmnTblCountMaxRow(pFormSeq,pintFormType,pintOutKei);

	// 頁番号の昇順、行番号の昇順でソートされたレコードセットを取得する。
	// 全頁のレコードセットを取得する
	pData->RequeryFgShowSortPageRow(ID_FGSHOW_OFF);

	// 最終行-1行目までループを行う（最終行の場合は下に空白行を作成しないため）
	while (!pData->IsEOF()) {
		// 現在行のデータを取得
		numpage = pData->m_NumPage;
		numrow = pData->m_NumRow;
		fgfunc = pData->m_FgFunc;
		// 一行でも小計を挿入するにチェックが無い場合のみ
		if(pOpOutSyokei == 0) {
			// 第１ソート項目のデータを取得
			strItem[0] = pData->GetFieldValueString(strFieldSykei[0]);
			strItem[1] = pData->GetFieldValueString(strFieldSykei[1]);
		}
		// 次行のデータを取得
		pData->MoveNext();
		if(pData->IsEOF())	{
			// 次の行が無ければ終了
			break;
		}
		fgfunc2 = pData->m_FgFunc;
		// 一行でも小計を挿入するにチェックが無い場合のみ
		if(pOpOutSyokei == 0) {
			// 第１ソート項目のデータを取得
			strNewItem[0] = pData->GetFieldValueString(strFieldSykei[0]);
			strNewItem[1] = pData->GetFieldValueString(strFieldSykei[1]);
		}
		// 前の行に戻す
		pData->MovePrev();
		sw2 = 0;
		flg = 0;
		// 現在の行が小計で頁の最終行かつ次の行が空白行・中計以外の場合
		if (fgfunc == ID_FGFUNC_SYOKEI && numrow == nBaseRow &&
			fgfunc2 != ID_FGFUNC_NULL &&
			fgfunc2 != ID_FGFUNC_CHUKEI &&
			fgfunc2 != ID_FGFUNC_CHUKEINULL) {
			sw2 = 1;
		}
		if(bG_Kanso == TRUE) {
			// 現在の行が中計で次の行が空白行以外の場合
			if (fgfunc == ID_FGFUNC_CHUKEI && numrow == nBaseRow && fgfunc2 != ID_FGFUNC_NULL) {
				sw2 = 1;
			}
		}

		// 一行でも小計を挿入するにチェックが無い場合のみ
		if(pOpOutSyokei == 0) {
			// 現在の行がデータ行（明細行、一括金額(手動、自動)）かつ
			// 次の行がデータ行（明細行、一括金額(手動、自動)）
			if( (fgfunc == ID_FGFUNC_DATA || fgfunc == ID_FGFUNC_IKKATUMANUAL || fgfunc == ID_FGFUNC_IKKATUAUTO) && 
				fgfunc2 == ID_FGFUNC_DATA || fgfunc2 == ID_FGFUNC_IKKATUMANUAL || fgfunc2 == ID_FGFUNC_IKKATUAUTO) {
				// 現在の行と次の行の第一ソート項目のデータが異なる
				if((strItem[0] != strNewItem[0]) || (strItem[1] != strNewItem[1])) {
					// 現在の行が最終行
					if(numrow == nBaseRow) {
						// 現在の行以外にデータが無ければ
						if(cnt2 == 0) {
							sw2 = 1;
							// 小計NULLを挿入
							flg = 1;
						}
					}
					// データカウントを初期化
					cnt2 = 0;
				}
				// 現在の行と次の行の第一ソート項目のデータが同じ
				else {
					// データカウントを上げる
					cnt2++;
				}
			}
			// 現在の行または次の行がデータ行（明細行、一括金額(手動、自動)）でない
			else {
				// データカウントを初期化
				cnt2 = 0;
			}
		}

		// 空白行または小計NULLを挿入する
		if( sw2 == 1) {
			pData->CreateNewRecord2(numpage,numrow+1,flg);
		}
		pData->MoveNext();
	}

	pData->Fin();

	return(retVal);
}

int IkkatuInsertNull(CdbUc000Common* pData,CDBZmSub* pZmSub,CCtrlSubGeneralVar pcs,int pFormSeq,int pOutKei,int pSort1,int pOpOutNullLine,int pOpOutSyokei)
{
	int					formtype=0;

	formtype = ID_FORMTYPE_KOBETU;
	CmnUcInfFormTypeGetFormType(pZmSub,pFormSeq,&formtype);

	// 頁と行の調整処理
	CmnTblDeleteFgFunc(pZmSub,ID_FGFUNC_PAGEKEI,pFormSeq);
	CmnTblDeleteFgFunc(pZmSub,ID_FGFUNC_RUIKEI,pFormSeq);
	// 小計行下に空白行を挿入する、または小計NULLを挿入する
	CmnTblUpdatePageNumRowNumReset_Sub(pFormSeq,pData,pZmSub,pSort1,pOpOutNullLine,pOpOutSyokei,formtype,pOutKei);
	CmnTblUpdatePageNumRowNumReset(pZmSub,1,1,pFormSeq,formtype,pOutKei);			// 先頭頁からページ再設定
	CmnTblUpdateEndPageSetting(pZmSub,pFormSeq);										// 最終頁を調整する
	CmnTblInsertPageKeiRuiKei(pZmSub,pFormSeq,formtype,pOutKei,pcs.bNameKei);		// 頁計/累計挿入（最終ページの削除or空行追加）
	// 小計/中計/頁計/累計の再計算
	CmnTblCalcKei(pZmSub,pFormSeq,pOutKei,pcs.bPrintZeroData,pcs.bPrintNullData);

	return(FUNCTION_OK);
}

// --------------------------------------------------------------------------------------------
// 様式切替時、先頭行の空白または小計NULLを削除する
// --------------------------------------------------------------------------------------------
void CmnTblDeleteFirstNullLine(int pFormSeq,CdbUc000Common* pData,CDBZmSub* pZmSub,int pintFormType,int pintOutKei)
{
	int					numpage=0;
	int					numrow=0;
	int					delsw=0;
	int					nBaseRow=0;
	int					retVal = FUNCTION_NG;
	int					fgfunc = 0;
	CString				strfilter=_T("");
	CString				strsort=_T("");

	retVal = FUNCTION_OK;

	// 行番号を振り直すための基準となる行数を取得
	nBaseRow = CmnTblCountMaxRow(pFormSeq,pintFormType,pintOutKei);

	// 頁番号の昇順、行番号の昇順でソートされたレコードセットを取得する。
	numpage = 1;
	numrow = 1;
	pData->RequeryFgShowSortPageRow(ID_FGSHOW_OFF);

	while(!pData->IsEOF())	{
		delsw=0;
		// １行目が空白または小計NULLの場合、削除する
		if(numrow == 1) {
			if(pData->m_FgFunc == ID_FGFUNC_NULL || pData->m_FgFunc == ID_FGFUNC_SYOKEINULL )	{
				// 削除
				pData->Delete();
				delsw=1;
			}
		}
		pData->MoveNext();
		if(delsw == 0)	{
			numrow++;
			if(numrow > nBaseRow)	{
				numpage++;
				numrow = 1;
			}
		}
	}
	pData->Fin();
}

int IkkatuDeleteNull(CdbUc000Common* pData,CDBZmSub* pZmSub,CCtrlSubGeneralVar pcs,int pFormSeq,int pOutKei,int pOpOutNullLine,int pOpOutSyokei)
{
	int					formtype=0;

	formtype = ID_FORMTYPE_KOBETU;
	CmnUcInfFormTypeGetFormType(pZmSub,pFormSeq,&formtype);

	// 頁と行の調整処理
	CmnTblDeleteFgFunc(pZmSub,ID_FGFUNC_PAGEKEI,pFormSeq);
	CmnTblDeleteFgFunc(pZmSub,ID_FGFUNC_RUIKEI,pFormSeq);
	// 小計行下に空白行を挿入する、または小計NULLを挿入する
	CmnTblDeleteFirstNullLine(pFormSeq,pData,pZmSub,formtype,pOutKei);
	CmnTblUpdatePageNumRowNumReset(pZmSub,1,1,pFormSeq,formtype,pOutKei);			// 先頭頁からページ再設定
	CmnTblUpdateEndPageSetting(pZmSub,pFormSeq);										// 最終頁を調整する
	CmnTblInsertPageKeiRuiKei(pZmSub,pFormSeq,formtype,pOutKei,pcs.bNameKei);		// 頁計/累計挿入（最終ページの削除or空行追加）
	// 小計/中計/頁計/累計の再計算
	CmnTblCalcKei(pZmSub,pFormSeq,pOutKei,pcs.bPrintZeroData,pcs.bPrintNullData);

	return(FUNCTION_OK);
}
// 157153 add <--

// -------------------------------------------------------------------------------------------------
// 各様式のテーブルオブジェクト取得
// -------------------------------------------------------------------------------------------------
// midori UC_0005 del -->
//CdbUc000Common* CDBUchiImp::GetDataTableObject(CDBZmSub* pZmSub,int pFormSeq)
// midori UC_0005 del <--
// midori UC_0005 add -->
CdbUc000Common* GetDataTableObject(CDBZmSub* pZmSub,int pFormSeq)
// midori UC_0005 add <--
{
	CdbUc000Common *dbuccommon;

	dbuccommon = NULL;
	switch(pFormSeq)	{
	case ID_FORMNO_011:			//	①	
		// 預貯金テーブルのオブジェクトを生成し、ポインタを返す	
		dbuccommon = (CdbUc000Common*)(new CdbUc011Yotyokin(pZmSub->m_database));
		break;
	case ID_FORMNO_021:			//	②	
		// 「受取手形」テーブルのオブジェクトを生成し、ポインタを返す
		dbuccommon = (CdbUc000Common*)(new CdbUc021Uketoritegata(pZmSub->m_database));
		break;
	case ID_FORMNO_031:			//	③	
		// 「売掛金」テーブルのオブジェクトを生成し、ポインタを返す
		dbuccommon = (CdbUc000Common*)(new CdbUc031Urikakekin(pZmSub->m_database));
		break;
	case ID_FORMNO_041:			//	④-1
		dbuccommon = (CdbUc000Common*)(new CdbUc041Karibarai(pZmSub->m_database));
		break;
	case ID_FORMNO_042:			//	④-2
		dbuccommon = (CdbUc000Common*)(new CdbUc042Kasituke(pZmSub->m_database));
		break;
	case ID_FORMNO_051:			//	⑤
		dbuccommon = (CdbUc000Common*)(new CdbUc051Tanaoroshi(pZmSub->m_database));
		break;
	case ID_FORMNO_061:			//	⑥
		dbuccommon = (CdbUc000Common*)(new CdbUc061Yuukasyouken(pZmSub->m_database));
		break;
	case ID_FORMNO_071:			//	⑦
		dbuccommon = (CdbUc000Common*)(new CdbUc071Koteishisan(pZmSub->m_database));
		break;
	case ID_FORMNO_081:			//	⑧
		dbuccommon = (CdbUc000Common*)(new CdbUc081Siharaitegata(pZmSub->m_database));
		break;
	case ID_FORMNO_091:			//	⑨
		dbuccommon = (CdbUc000Common*)(new CdbUc091Kaikakekin(pZmSub->m_database));
		break;
	case ID_FORMNO_101:			//	⑩-1
		dbuccommon = (CdbUc000Common*)(new CdbUc101Kariukekin(pZmSub->m_database));
		break;
	case ID_FORMNO_102:			//	⑩-2
		dbuccommon = (CdbUc000Common*)(new CdbUc102Gensen(pZmSub->m_database));
		break;
	case ID_FORMNO_111:			//	⑪
		dbuccommon = (CdbUc000Common*)(new CdbUc111Kariirekin(pZmSub->m_database));
		break;
	case ID_FORMNO_121:			//	⑫
		dbuccommon = (CdbUc000Common*)(new CdbUc121Totiuriage(pZmSub->m_database));
		break;
	case ID_FORMNO_131:			//	⑬
		dbuccommon = (CdbUc000Common*)(new CdbUc131Uriagedaka(pZmSub->m_database));
		break;
	case ID_FORMNO_141:			//	⑭-1
		dbuccommon = (CdbUc000Common*)(new CdbUc141Yakuin(pZmSub->m_database));
		break;
	case ID_FORMNO_142:			//	⑭-2
		dbuccommon = (CdbUc000Common*)(new CdbUc142Jinkenhi(pZmSub->m_database));
		break;
	case ID_FORMNO_151:			//	⑮-1
		dbuccommon = (CdbUc000Common*)(new CdbUc151Tidaiyatin(pZmSub->m_database));
		break;
	case ID_FORMNO_152:			//	⑮-2
		dbuccommon = (CdbUc000Common*)(new CdbUc152Kenrikin(pZmSub->m_database));
		break;
	case ID_FORMNO_153:			//	⑮-3
		dbuccommon = (CdbUc000Common*)(new CdbUc153Kougyou(pZmSub->m_database));
		break;
	case ID_FORMNO_161:			//	⑯-1
		dbuccommon = (CdbUc000Common*)(new CdbUc161Zatueki(pZmSub->m_database));
		break;
	case ID_FORMNO_162:			//	⑯-2
		dbuccommon = (CdbUc000Common*)(new CdbUc162Zatuson(pZmSub->m_database));
		break;
	case ID_FORMNO_171:			//	⑰ 1
	case ID_FORMNO_172:			//	⑰ 2
	case ID_FORMNO_173:			//	⑰ 3
	case ID_FORMNO_174:			//	⑰ 4
	case ID_FORMNO_175:			//	⑰ 5
	case ID_FORMNO_176:			//	⑰ 6
	case ID_FORMNO_177:			//	⑰ 7
	case ID_FORMNO_178:			//	⑰ 8
	case ID_FORMNO_179:			//	⑰ 9
	case ID_FORMNO_1710:		//	⑰ 10
	case ID_FORMNO_1711:		//	⑰ 11
	case ID_FORMNO_1712:		//	⑰ 12
	case ID_FORMNO_1713:		//	⑰ 13
	case ID_FORMNO_1714:		//	⑰ 14
	case ID_FORMNO_1715:		//	⑰ 15
	case ID_FORMNO_1716:		//	⑰ 16
	case ID_FORMNO_1717:		//	⑰ 17
	case ID_FORMNO_1718:		//	⑰ 18
	case ID_FORMNO_1719:		//	⑰ 19
	case ID_FORMNO_1720:		//	⑰ 20
		dbuccommon = (CdbUc000Common*)(new CdbUc171Sonota1(pZmSub->m_database,pFormSeq));
		break;
	case ID_FORMNO_181:			//	⑱ 1
	case ID_FORMNO_182:			//	⑱ 2
	case ID_FORMNO_183:			//	⑱ 3
	case ID_FORMNO_184:			//	⑱ 4
	case ID_FORMNO_185:			//	⑱ 5
	case ID_FORMNO_186:			//	⑱ 6
	case ID_FORMNO_187:			//	⑱ 7
	case ID_FORMNO_188:			//	⑱ 8
	case ID_FORMNO_189:			//	⑱ 9
	case ID_FORMNO_1810:		//	⑱ 10
	case ID_FORMNO_1811:		//	⑱ 11
	case ID_FORMNO_1812:		//	⑱ 12
	case ID_FORMNO_1813:		//	⑱ 13
	case ID_FORMNO_1814:		//	⑱ 14
	case ID_FORMNO_1815:		//	⑱ 15
	case ID_FORMNO_1816:		//	⑱ 16
	case ID_FORMNO_1817:		//	⑱ 17
	case ID_FORMNO_1818:		//	⑱ 18
	case ID_FORMNO_1819:		//	⑱ 19
	case ID_FORMNO_1820:		//	⑱ 20	
		dbuccommon = (CdbUc000Common*)(new CdbUc181Sonota2(pZmSub->m_database, pFormSeq));
		break;
	default:
		break;
	}
	return(dbuccommon);
}

// -------------------------------------------------------------------------------------------------
//	<引数>
//		inFgFuncKind	:	削除したい特殊行種別
//	<戻値>
//		 0	:	成功
//		-1	:	失敗
//	<機能>
//		パラメータで指定された特殊行を削除する
// -------------------------------------------------------------------------------------------------
// midori UC_0005 del -->
//int CDBUchiImp::CmnTblDeleteFgFunc(CDBZmSub* pZmSub,int inFgFuncKind,int pFormSeq)
// midori UC_0005 del <--
// midori UC_0005 add -->
int CmnTblDeleteFgFunc(CDBZmSub* pZmSub,int inFgFuncKind,int pFormSeq)
// midori UC_0005 add <--
{
	CdbUc000Common*		rsData;
	int					retVal = FUNCTION_NG;

	retVal = FUNCTION_OK;

	rsData = GetDataTableObject(pZmSub,pFormSeq);

	// 特殊行削除メソッド
	rsData->DeleteFgFunc(inFgFuncKind);

	rsData->Fin();
	delete rsData;

	return retVal;	
}

// -------------------------------------------------------------------------------------------------
//	<引数>
//		inPage	:	頁番号
//		inRow	:	行番号
//	<戻値>
//		 0	:	成功
//		-1	:	失敗
//	<機能>
//		パラメータの頁番号(inPage)、行番号(inRow)以降の
//		頁番号と行番号を振り直す。
// -------------------------------------------------------------------------------------------------
// midori UC_0005 del -->
//int CDBUchiImp::CmnTblUpdatePageNumRowNumReset(CDBZmSub* pZmSub,int inPage, int inRow,int pFormSeq,int pintFormType,int pintOutKei)
// midori UC_0005 del <--
// midori UC_0005 add -->
int CmnTblUpdatePageNumRowNumReset(CDBZmSub* pZmSub,int inPage, int inRow,int pFormSeq,int pintFormType,int pintOutKei)
// midori UC_0005 add <--
{
	CdbUc000Common*		rsData;
	int					intNumPage;
	int					intNumRow;
	int					nBaseRow;
	int					retVal = FUNCTION_NG;

	retVal = FUNCTION_OK;

	rsData = GetDataTableObject(pZmSub,pFormSeq);

	// 行番号を振り直すための基準となる行数を取得
	nBaseRow = CmnTblCountMaxRow(pFormSeq,pintFormType,pintOutKei);

	// 頁番号の昇順、行番号の昇順でソートされたレコードセットを取得する。
	rsData->RequeryFgShowSortPageRow(ID_FGSHOW_OFF);

	intNumPage = inPage;
	intNumRow = inRow;

	// 頁番、行番振り直し
	while (!rsData->IsEOF()) {

		// パラメータの頁番号、行番号以上のレコードから番号を振り直す
		if ((rsData->m_NumPage > inPage) || ((rsData->m_NumPage == inPage) && (rsData->m_NumRow >= inRow))) {
			rsData->Edit();
			rsData->m_NumPage = intNumPage;		// 頁番号をセット
			rsData->m_NumRow = intNumRow;		// 行番号をセット
			rsData->Update();
		}

		// 頁番号と行番号をインクリメント
		if (intNumRow >= nBaseRow) {
			intNumPage++;
			intNumRow = 1;
		}
		else {
			intNumRow++;
		}

		// 次のレコードへ
		rsData->MoveNext();
	}

	rsData->Fin();
	delete rsData;

	return retVal;
}

// -------------------------------------------------------------------------------------------------
//	<引数>
//		なし
//	<戻値>
//		行数
//	<機能>
//		行数をカウントアップする際の、ベースになる１頁の行数を求める
// -------------------------------------------------------------------------------------------------
// midori UC_0005 del -->
//int CDBUchiImp::CmnTblCountMaxRow(int pFormSeq,int pintFormType,int pintOutKei)
// midori UC_0005 del <--
// midori UC_0005 add -->
int CmnTblCountMaxRow(int pFormSeq,int pintFormType,int pintOutKei)
// midori UC_0005 add <--
{
	int		nBaseRow=0;
	int		intRowKei=0;
	int		intRowMaxData=0;
	int		intRowMax=0;

	GetFormRow(pFormSeq,pintFormType,pintOutKei,&intRowMax,&intRowMaxData,&intRowKei);

	// デフォルトに「計あり」の行数をセットしておく
	nBaseRow = intRowMaxData;

	// ベースになる行数を決める
	// 「計なし」帳表で、計設定が「オフ」または「累計のみ」の場合は、「計なし」の場合の行数をセットする
	if ((intRowKei == 0) && ((pintOutKei == ID_OUTKEI_OFF) || (pintOutKei == ID_OUTKEI_RUIKEI))) {
		nBaseRow = intRowMax;
	}

	return nBaseRow;
}

// midori UC_0005 del -->
//void CDBUchiImp::GetFormRow(int pFormSeq,int pintFormType,int intOutKei,int* pintRowMax,int* pintRowMaxData,int* pintRowKei)
// midori UC_0005 del <--
// midori UC_0005 add -->
void GetFormRow(int pFormSeq,int pintFormType,int intOutKei,int* pintRowMax,int* pintRowMaxData,int* pintRowKei)
// midori UC_0005 add <--
{
	int		intRowKei=0;
	int		intRowKobetu=0;
	int		intRowNormal=0;
	int		intRowMaxData=0;
	int		intRowMax=0;
	//BOOL	bflg=FALSE;

	switch(pFormSeq)	{
	case ID_FORMNO_011:			//	①
		intRowKobetu = ID_ROWKOBETU_011;
		intRowNormal = ID_ROWNORMAL_011;
		intRowMax = ID_ROWKOBETU_011;
		intRowKei = ID_ROWKEI_011;
		break;
	case ID_FORMNO_021:			//	②
		intRowKobetu = ID_ROWKOBETU_021;
		intRowNormal = ID_ROWNORMAL_021;
		intRowMax = ID_ROWKOBETU_021;
		intRowKei = ID_ROWKEI_021;
		break;
	case ID_FORMNO_031:			//	③	
		intRowKobetu = ID_ROWKOBETU_031;
		intRowNormal = ID_ROWNORMAL_031;
		intRowMax = ID_ROWKOBETU_031;
		intRowKei = ID_ROWKEI_031;
		break;
	case ID_FORMNO_041:			//	④-1
		intRowKobetu = ID_ROWKOBETU_041;
		intRowNormal = ID_ROWNORMAL_041;
		intRowMax = ID_ROWKOBETU_041;
		intRowKei = ID_ROWKEI_041;
		//bflg = TRUE;
		break;
	case ID_FORMNO_042:			//	④-2
		intRowKobetu = ID_ROWKOBETU_042;
		intRowNormal = ID_ROWNORMAL_042;
		intRowMax = ID_ROWKOBETU_042;
		intRowKei = ID_ROWKEI_042;
		//bflg = TRUE;
		break;
	case ID_FORMNO_051:			//	⑤
		intRowKobetu = ID_ROWKOBETU_051;
		intRowNormal = ID_ROWNORMAL_051;
		intRowMax = ID_ROWKOBETU_051;
		intRowKei = ID_ROWKEI_051;
		break;
	case ID_FORMNO_061:			//	⑥
		intRowKobetu = ID_ROWKOBETU_061;
		intRowNormal = ID_ROWNORMAL_061;
		intRowMax = ID_ROWKOBETU_061;
		intRowKei = ID_ROWKEI_061;
		break;
	case ID_FORMNO_071:			//	⑦
		intRowKobetu = ID_ROWKOBETU_071;
		intRowNormal = ID_ROWNORMAL_071;
		intRowMax = ID_ROWKOBETU_071;
		intRowKei = ID_ROWKEI_071;
		break;
	case ID_FORMNO_081:			//	⑧
		intRowKobetu = ID_ROWKOBETU_081;
		intRowNormal = ID_ROWNORMAL_081;
		intRowMax = ID_ROWKOBETU_081;
		intRowKei = ID_ROWKEI_081;
		break;
	case ID_FORMNO_091:			//	⑨
		intRowKobetu = ID_ROWKOBETU_091;
		intRowNormal = ID_ROWNORMAL_091;
		intRowMax = ID_ROWKOBETU_091;
		intRowKei = ID_ROWKEI_091;
		break;
	case ID_FORMNO_101:			//	⑩-1
		intRowKobetu = ID_ROWKOBETU_101;
		intRowNormal = ID_ROWNORMAL_101;
		intRowMax = ID_ROWKOBETU_101;
		intRowKei = ID_ROWKEI_101;
		//bflg = TRUE;
		break;
	case ID_FORMNO_102:			//	⑩-2
		intRowKobetu = ID_ROWKOBETU_102;
		intRowNormal = ID_ROWNORMAL_102;
		intRowMax = ID_ROWKOBETU_102;
		intRowKei = ID_ROWKEI_102;
		//bflg = TRUE;
		break;
	case ID_FORMNO_111:			//	⑪
		intRowKobetu = ID_ROWKOBETU_111;
		intRowNormal = ID_ROWNORMAL_111;
		intRowMax = ID_ROWKOBETU_111;
		intRowKei = ID_ROWKEI_111;
		break;
	case ID_FORMNO_121:			//	⑫
		intRowKobetu = ID_ROWKOBETU_121;
		intRowNormal = ID_ROWNORMAL_121;
		intRowMax = ID_ROWKOBETU_121;
		intRowKei = ID_ROWKEI_121;
		break;
	case ID_FORMNO_131:			//	⑬
		intRowKobetu = ID_ROWKOBETU_131;
		intRowNormal = ID_ROWNORMAL_131;
		intRowMax = ID_ROWKOBETU_131;
		intRowKei = ID_ROWKEI_131;
		break;
	case ID_FORMNO_141:			//	⑭-1
		intRowKobetu = ID_ROWKOBETU_141;
		intRowNormal = ID_ROWNORMAL_141;
		intRowMax = ID_ROWKOBETU_141;
		intRowKei = ID_ROWKEI_141;
		//bflg = TRUE;
		break;
	case ID_FORMNO_142:			//	⑭-2
		intRowKobetu = ID_ROWKOBETU_142;
		intRowNormal = ID_ROWNORMAL_142;
		intRowMax = ID_ROWKOBETU_142;
		intRowKei = ID_ROWKEI_142;
		//bflg = TRUE;
		break;
	case ID_FORMNO_151:			//	⑮-1
		intRowKobetu = ID_ROWKOBETU_151;
		intRowNormal = ID_ROWNORMAL_151;
		intRowMax = ID_ROWKOBETU_151;
		intRowKei = ID_ROWKEI_151;
		//bflg = TRUE;
		break;
	case ID_FORMNO_152:			//	⑮-2
		intRowKobetu = ID_ROWKOBETU_152;
		intRowNormal = ID_ROWNORMAL_152;
		intRowMax = ID_ROWKOBETU_152;
		intRowKei = ID_ROWKEI_152;
		//bflg = TRUE;
		break;
	case ID_FORMNO_153:			//	⑮-3
		intRowKobetu = ID_ROWKOBETU_153;
		intRowNormal = ID_ROWNORMAL_153;
		intRowMax = ID_ROWKOBETU_153;
		intRowKei = ID_ROWKEI_153;
		//bflg = TRUE;
		break;
	case ID_FORMNO_161:			//	⑯-1
		intRowKobetu = ID_ROWKOBETU_161;
		intRowNormal = ID_ROWNORMAL_161;
		intRowMax = ID_ROWKOBETU_161;
		intRowKei = ID_ROWKEI_161;
		//bflg = TRUE;
		break;
	case ID_FORMNO_162:			//	⑯-2
		intRowKobetu = ID_ROWKOBETU_162;
		intRowNormal = ID_ROWNORMAL_162;
		intRowMax = ID_ROWKOBETU_162;
		intRowKei = ID_ROWKEI_162;
		//bflg = TRUE;
		break;
	case ID_FORMNO_171:			//	⑰ 1
	case ID_FORMNO_172:			//	⑰ 2
	case ID_FORMNO_173:			//	⑰ 3
	case ID_FORMNO_174:			//	⑰ 4
	case ID_FORMNO_175:			//	⑰ 5
	case ID_FORMNO_176:			//	⑰ 6
	case ID_FORMNO_177:			//	⑰ 7
	case ID_FORMNO_178:			//	⑰ 8
	case ID_FORMNO_179:			//	⑰ 9
	case ID_FORMNO_1710:		//	⑰ 10
	case ID_FORMNO_1711:		//	⑰ 11
	case ID_FORMNO_1712:		//	⑰ 12
	case ID_FORMNO_1713:		//	⑰ 13
	case ID_FORMNO_1714:		//	⑰ 14
	case ID_FORMNO_1715:		//	⑰ 15
	case ID_FORMNO_1716:		//	⑰ 16
	case ID_FORMNO_1717:		//	⑰ 17
	case ID_FORMNO_1718:		//	⑰ 18
	case ID_FORMNO_1719:		//	⑰ 19
	case ID_FORMNO_1720:		//	⑰ 20
		intRowKobetu = ID_ROWKOBETU_171;
		intRowNormal = ID_ROWNORMAL_171;
		intRowMax = ID_ROWKOBETU_171;
		intRowKei = ID_ROWKEI_171;
		break;
	case ID_FORMNO_181:			//	⑱ 1
	case ID_FORMNO_182:			//	⑱ 2
	case ID_FORMNO_183:			//	⑱ 3
	case ID_FORMNO_184:			//	⑱ 4
	case ID_FORMNO_185:			//	⑱ 5
	case ID_FORMNO_186:			//	⑱ 6
	case ID_FORMNO_187:			//	⑱ 7
	case ID_FORMNO_188:			//	⑱ 8
	case ID_FORMNO_189:			//	⑱ 9
	case ID_FORMNO_1810:		//	⑱ 10
	case ID_FORMNO_1811:		//	⑱ 11
	case ID_FORMNO_1812:		//	⑱ 12
	case ID_FORMNO_1813:		//	⑱ 13
	case ID_FORMNO_1814:		//	⑱ 14
	case ID_FORMNO_1815:		//	⑱ 15
	case ID_FORMNO_1816:		//	⑱ 16
	case ID_FORMNO_1817:		//	⑱ 17
	case ID_FORMNO_1818:		//	⑱ 18
	case ID_FORMNO_1819:		//	⑱ 19
	case ID_FORMNO_1820:		//	⑱ 20
		intRowKobetu = ID_ROWKOBETU_181;
		intRowNormal = ID_ROWNORMAL_181;
		intRowMax = ID_ROWKOBETU_181;
		intRowKei = ID_ROWKEI_181;
		break;
	default:
		break;
	}
	intRowMaxData = intRowMax - 1;

	//if(bflg == TRUE)	{
		// 出力形式から１頁の最大行数/データ行数と合計行の先頭インデックスを取得
		if (pintFormType == ID_FORMTYPE_KOBETU) {
			// 個別
			if (intRowKei == 1) {
				// 計あり帳表
				intRowMaxData = intRowKobetu - 1;
			}
			else if ((intOutKei == ID_OUTKEI_PAGEKEI) || (intOutKei == ID_OUTKEI_BOTH)) {
				// 頁計 or 頁計/累計
				intRowMaxData = intRowKobetu - 1;
			}
			else {
				// 最下段もデータとして使用する
				intRowMaxData = intRowKobetu;
			}
			intRowMax = intRowKobetu;				// １頁内の合計行を含む全体の行数
		}
		else {
			// 通常
			if (intRowKei == 1) {
				// 計あり帳表
				intRowMaxData = intRowNormal - 1;
			}
			else if ((intOutKei == ID_OUTKEI_PAGEKEI) || (intOutKei == ID_OUTKEI_BOTH)) {
				// 頁計 or 頁計/累計
				intRowMaxData = intRowNormal - 1;
			}
			else {
				// 最下段もデータとして使用する
				intRowMaxData = intRowNormal;
			}
			intRowMax = intRowNormal;				// １頁内の合計行を含む全体の行数
		}
	//}

	*pintRowMax = intRowMax;
	*pintRowMaxData = intRowMaxData;
	*pintRowKei = intRowKei;
}

//********************************************************************************
//	uc_inf_form_type：出力形式の取得
//		IN		なし
//		RET		BOOL		TRUE:正常終了
//********************************************************************************
// midori UC_0005 del -->
//BOOL CDBUchiImp::CmnUcInfFormTypeGetFormType(CDBZmSub* pZmSub,int intFormSeq,int* intFormType)
// midori UC_0005 del <--
// midori UC_0005 add -->
BOOL CmnUcInfFormTypeGetFormType(CDBZmSub* pZmSub,int intFormSeq,int* intFormType)
// midori UC_0005 add <--
{
	CdbUcInfFormType	rsFormType(pZmSub->m_database);
	int					intRet = ID_FORMTYPE_KOBETU;	// デフォルト：個別
	int					intFormNo=0;

	switch(intFormSeq)	{
	case ID_FORMNO_041:
	case ID_FORMNO_042:
		intFormNo = 4;
		break;
	case ID_FORMNO_101:
	case ID_FORMNO_102:
		intFormNo = 10;
		break;
	case ID_FORMNO_141:
	case ID_FORMNO_142:
		intFormNo = 14;
		break;
	case ID_FORMNO_151:
	case ID_FORMNO_152:
	case ID_FORMNO_153:
		intFormNo = 15;
		break;
	case ID_FORMNO_161:
	case ID_FORMNO_162:
		intFormNo = 16;
		break;
	default:
		break;
	}
	if(intFormNo != 0)	{
		// 出力形式を取得：切替不可の場合はレコード未登録
		if (rsFormType.Init(intFormNo) == DB_ERR_OK) {
			if (!rsFormType.IsEOF()) {
				// 設定値より取得
				intRet = rsFormType.m_FormType;
			}
			rsFormType.Fin();
			*intFormType = intRet;
		}
		else{
			return FALSE;
		}
	}

	return TRUE;
}

// -------------------------------------------------------------------------------------------------
//	<引数>
//		なし
//	<戻値>
//		 0	:	成功
//		-1	:	失敗
//	<機能>
//		最終頁を調整する。最終頁の全行が空行なら、頁まるごと削除する。
// -------------------------------------------------------------------------------------------------
// midori UC_0005 del -->
//int CDBUchiImp::CmnTblUpdateEndPageSetting(CDBZmSub* pZmSub,int pFormSeq)
// midori UC_0005 del <--
// midori UC_0005 add -->
int CmnTblUpdateEndPageSetting(CDBZmSub* pZmSub,int pFormSeq)
// midori UC_0005 add <--
{
	CdbUc000Common*		rsData;
	int					nBaseMaxPage, nMaxPage;	// 最大頁数
	int					nRowNum;				// 空行且つ表示可能な行の行数
	int					nNullRowNum;			// 空行の行数
	int					retVal = FUNCTION_NG;

	retVal = FUNCTION_OK;

	rsData = GetDataTableObject(pZmSub,pFormSeq);

	// 最大頁数を取得
	nBaseMaxPage = rsData->GetNumPage();
	nMaxPage = nBaseMaxPage;
	
	// 最終頁が全て空行なら頁まるごと削除する。1頁目は対象外とする。
	while (nMaxPage >= 2) {
		
		// 空行(FgFunc=0)、且つ表示行(FgShow=0)の行数を取得する。
		nRowNum = rsData->GetNumRowFgFuncFgShow(nMaxPage, ID_FGFUNC_NULL, ID_FGSHOW_OFF);

		// 表示行(FgShow=0)の行数を取得する。
		nNullRowNum = rsData->GetNumRowFgShow(nMaxPage, ID_FGSHOW_OFF);
		
		// 最終頁の削除有無をチェック
		if (nRowNum == nNullRowNum) {
			nMaxPage--;
		} else {
			break;	// ループから抜ける
		}

	}

	// 空行頁を削除する。
	if (nMaxPage < nBaseMaxPage) {
		rsData->DeletePage(nMaxPage + 1);
	}

	rsData->Fin();
	delete rsData;

	return retVal;
}

// -------------------------------------------------------------------------------------------------
//	<引数>
//		なし
//	<戻値>
//		 0	:	成功
//		-1	:	失敗
//	<機能>
//		頁計、累計行を挿入する。
// -------------------------------------------------------------------------------------------------
// midori UC_0005 del -->
//int CDBUchiImp::CmnTblInsertPageKeiRuiKei(CDBZmSub* pZmSub,int pFormSeq,int pintFormType,int pintOutKei,int NameKei)
// midori UC_0005 del <--
// midori UC_0005 add -->
int CmnTblInsertPageKeiRuiKei(CDBZmSub* pZmSub,int pFormSeq,int pintFormType,int pintOutKei,int NameKei)
// midori UC_0005 add <--
{
	CdbUc000Common*		rs;					// 全レコード取得用
	int			nSetNumPage;				// 頁番号保存変数
	int			nSetNumRow;					// 行番号保存変数
	int			retVal = FUNCTION_NG;
	int			intRowKei=0;
	int			intRowMaxData=0;
	int			intRowMax=0;
	CString		strNameKei;

	retVal = FUNCTION_OK;

	GetFormRow(pFormSeq,pintFormType,pintOutKei,&intRowMax,&intRowMaxData,&intRowKei);

	// 計の名称セット
	if(NameKei)		strNameKei = NAME_GOUKEI;
	else			strNameKei = NAME_KEI;

	rs = GetDataTableObject(pZmSub,pFormSeq);

	// 頁番号の昇順、行番号の昇順でソートされたレコードセットを取得する。
	rs->RequeryFgShowSortPageRow(ID_FGSHOW_OFF);

	// 1頁目の1行目からチェックする。
	nSetNumPage = 1;
	nSetNumRow = 1;

	// Step1：先頭頁の先頭行から順に参照して、頁計/累計行を挿入する
	switch (pintOutKei) {
	case ID_OUTKEI_OFF:		// なし
	case ID_OUTKEI_PAGEKEI:	// 頁計
	case ID_OUTKEI_RUIKEI:	// 累計
	case ID_OUTKEI_BOTH:	// 累計と累計

		// 計行、頁計行、累計行を挿入していく。
		while (!rs->IsEOF()) {
			// 頁計を挿入する条件式
			if (nSetNumRow > intRowMaxData) {
				if (intRowKei == 1) {
					// 計あり帳表
					switch (pintOutKei) {
					case ID_OUTKEI_OFF:		// なし
					case ID_OUTKEI_RUIKEI:	// 累計のみを挿入する
						// 頁計なしの場合、"計"or"合計"文字をセット
						//CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, NAME_KEI);
						CmnTblInsertSpecialLine(pZmSub,pFormSeq,nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, strNameKei);
						break;

					case ID_OUTKEI_PAGEKEI:	// 頁計のみを挿入する
					case ID_OUTKEI_BOTH:	// 累計と累計の両方を挿入する
						// 頁計をセット
						CmnTblInsertSpecialLine(pZmSub,pFormSeq,nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, NAME_PAGEKEI);
						break;
					}
				}
				else {
					// 計なし帳表
					switch (pintOutKei) {
					case ID_OUTKEI_PAGEKEI:	// 頁計のみを挿入する
					case ID_OUTKEI_BOTH:	// 累計と累計の両方を挿入する
						// 頁計をセット
						CmnTblInsertSpecialLine(pZmSub,pFormSeq,nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, NAME_PAGEKEI);
						break;
					}
				}
				nSetNumPage++;		// 頁番号をインクリメント
				nSetNumRow = 1;		// 行番号を先頭に戻す
			}
			else {
				rs->MoveNext();
				nSetNumRow++;		// 行番号をインクリメント
			}
		}
		break;
	}

	// Step2：最終頁の空行挿入＋頁計/累計作成
	switch (pintOutKei) {
	case ID_OUTKEI_OFF:		// 計なし
		// 計あり帳表：最終行は合計行とする
		// 計なし帳表：最終行まで空行で埋める
		while (nSetNumRow <= intRowMax) {
			if ((nSetNumRow == intRowMax) && (intRowKei == 1)) {
				// 最終行に合計行を挿入
				//CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, NAME_KEI);
				CmnTblInsertSpecialLine(pZmSub,pFormSeq,nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, strNameKei);
			}
			else {
				// 空行挿入
				CmnTblInsertNullLine(pZmSub,pFormSeq,pintFormType,pintOutKei,nSetNumPage, nSetNumRow, 1);
			}
			nSetNumRow++;		// 行番号をインクリメント
		}
		break;

	case ID_OUTKEI_PAGEKEI:	// 頁計
		// 計あり帳表，計なし帳表共に、最終行は合計行とする
		while (nSetNumRow <= intRowMax) {
			if (nSetNumRow == intRowMax) {
				// 最終行に合計行を挿入
				CmnTblInsertSpecialLine(pZmSub,pFormSeq,nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, NAME_PAGEKEI);
			}
			else {
				// 空行挿入
				CmnTblInsertNullLine(pZmSub,pFormSeq,pintFormType,pintOutKei,nSetNumPage, nSetNumRow, 1);
			}
			nSetNumRow++;		// 行番号をインクリメント
		}
		break;

	case ID_OUTKEI_RUIKEI:	// 累計
		// 計あり帳表：最終行は合計行とする
		// 計なし帳表：最終行は合計行とする。また最終行がデータ行なら次頁を空行で埋め最終行を累計とする
		if (intRowKei == 1) {
			// 計あり帳表
			while (nSetNumRow <= intRowMax) {
				if (nSetNumRow == intRowMax) {
					// 最終行に合計行を挿入
					//CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_RUIKEI, NAME_RUIKEI);
					CmnTblInsertSpecialLine(pZmSub,pFormSeq,nSetNumPage, nSetNumRow, ID_FGFUNC_RUIKEI, strNameKei);
				}
				else {
					// 空行挿入
					CmnTblInsertNullLine(pZmSub,pFormSeq,pintFormType,pintOutKei,nSetNumPage, nSetNumRow, 1);
				}
				nSetNumRow++;		// 行番号をインクリメント
			}
		}
		else {
			// 計なし帳表：最終データが空行なら削除する（最終行の条件を付けるべきかも）
			if (rs->IsBOF() == 0) {			// 空行削除では、前レコードが存在しない場合がある
				rs->MovePrev();
				if (!rs->IsEOF()) {
					if ((rs->m_FgFunc == 0) && (rs->m_FgShow == 0)) {
						rs->Delete();
						nSetNumRow--;
					}
				}
			}
			// すでに最終行がデータで埋まっている場合は次頁に累計行を入れる
			if (nSetNumRow > intRowMax) {
				nSetNumPage++;			// 頁番号をインクリメント
				nSetNumRow = 1;			// 行番号をクリア
			}
			// あとは空行で埋め、最終行に合計行を挿入
			if (nSetNumRow <= intRowMax) {
				while (nSetNumRow <= intRowMax) {
					if (nSetNumRow == intRowMax) {
						// 最終行に合計行を挿入
						//CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_RUIKEI, NAME_RUIKEI);
						CmnTblInsertSpecialLine(pZmSub,pFormSeq,nSetNumPage, nSetNumRow, ID_FGFUNC_RUIKEI, strNameKei);
					}
					else {
						// 空行挿入
						CmnTblInsertNullLine(pZmSub,pFormSeq,pintFormType,pintOutKei,nSetNumPage, nSetNumRow, 1);
					}
					nSetNumRow++;		// 行番号をインクリメント
				}
			}
		}
		break;

	case ID_OUTKEI_BOTH:	// 頁計と累計
		// 計あり帳表，計なし帳表共に、データの最後を頁計と累計とする。
		//	１頁しかない場合は、累計行のみ追加する
		//	合計行を１行しか挿入できない場合、該当頁は頁計として、次頁に頁計と累計行とする
		if (rs->IsBOF() == 0) {			// 空行削除では、前レコードが存在しない場合がある
			rs->MovePrev();
			if (!rs->IsEOF()) {
				if ((rs->m_FgFunc == 0) && (rs->m_FgShow == 0)) {
					rs->Delete();
					nSetNumRow--;
				}
			}
		}
		// 既に最終行－１がデータで埋まっている場合
		if (nSetNumRow >= intRowMax) {
			if (nSetNumPage == 1) {
				// 累計を挿入して終了
				//CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_RUIKEI, NAME_RUIKEI);
				CmnTblInsertSpecialLine(pZmSub,pFormSeq,nSetNumPage, nSetNumRow, ID_FGFUNC_RUIKEI, strNameKei);
				nSetNumRow++;			// 行番号をインクリメント
			}
			else {
				// 頁計を挿入して次頁に頁計と累計を出す
				CmnTblInsertSpecialLine(pZmSub,pFormSeq,nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, NAME_PAGEKEI);
				nSetNumPage++;			// 頁番号をインクリメント
				nSetNumRow = 1;			// 行番号をクリア
			}
		}
		// あとは空行で埋め、最終行－１と最終行に合計行を挿入
		while (nSetNumRow <= intRowMax) {
			if (nSetNumRow == intRowMax) {
				// 最終行に累計行を挿入
				//CmnTblInsertSpecialLine(nSetNumPage, nSetNumRow, ID_FGFUNC_RUIKEI, NAME_RUIKEI);
				CmnTblInsertSpecialLine(pZmSub,pFormSeq,nSetNumPage, nSetNumRow, ID_FGFUNC_RUIKEI, strNameKei);
			}
			else if ((nSetNumRow == intRowMax - 1) && (nSetNumPage >= 2)) {
				// 最終行－１に頁計行を挿入
				CmnTblInsertSpecialLine(pZmSub,pFormSeq,nSetNumPage, nSetNumRow, ID_FGFUNC_PAGEKEI, NAME_PAGEKEI);
			}
			else {
				// 空行挿入
				CmnTblInsertNullLine(pZmSub,pFormSeq,pintFormType,pintOutKei,nSetNumPage, nSetNumRow, 1);
			}
			nSetNumRow++;		// 行番号をインクリメント
		}
	}
	delete rs;

	return retVal;	
}

// -------------------------------------------------------------------------------------------------
//	空行挿入･･･複数行可能
//		IN		int		挿入位置（頁）
//				int		挿入位置（行）
//				int		挿入行数
//		RET		int		実行結果（FUNCTION_OK, FUNCTION_NG）
// -------------------------------------------------------------------------------------------------
// midori UC_0005 del -->
//int CDBUchiImp::CmnTblInsertNullLine(CDBZmSub* pZmSub,int pFormSeq,int pintFormType,int pintOutKei,int nPage, int nRow, int nLine)
// midori UC_0005 del <--
// midori UC_0005 add -->
int CmnTblInsertNullLine(CDBZmSub* pZmSub,int pFormSeq,int pintFormType,int pintOutKei,int nPage, int nRow, int nLine)
// midori UC_0005 add <--
{
	int					i = 0;
	int					nCountPage = nPage;
	int					nCountRow = nRow;
	int					intRowKei=0;
	int					intRowMaxData=0;
	int					intRowMax=0;

	// パラメータのチェック
	if (nPage <= 0 || nRow <= 0 || nLine <= 0) {
		return FUNCTION_NG;
	}

	GetFormRow(pFormSeq,pintFormType,pintOutKei,&intRowMax,&intRowMaxData,&intRowKei);

	// 帳表毎のテーブルクラスを"New"する
	CdbUc000Common*		rsData;
	rsData = GetDataTableObject(pZmSub,pFormSeq);

	// ｎ件分のレコード追加
	for (i = 0; i < nLine; i++){
		// 新規レコード追加
		rsData->CreateNewRecord();				// レコード初期化済み

		// 空行データとする
		rsData->Edit();
		rsData->m_NumPage = nCountPage;			// ページ番号
		rsData->m_NumRow = nCountRow;			// 行番号
		rsData->m_FgFunc = ID_FGFUNC_NULL;		// 特殊行フラグ
		rsData->m_FgShow = ID_FGSHOW_OFF;		// 表示フラグ

		// レコード更新
		rsData->Update();

		// 次の頁と行を取得
		CmnGetNextPageRow(&nCountPage, &nCountRow,intRowMaxData);
	}
	rsData->Fin();

	delete rsData;
	return FUNCTION_OK;
}

// -------------------------------------------------------------------------------------------------
//	頁＋行から次の行を取得する
//		IN:		int		頁番号 → 新しい頁番号へ更新
//				int		頁番号 → 新しい行番号へ更新
//		RET:	int		実行結果（FUNCTION_OK, FUNCTION_NG）
// -------------------------------------------------------------------------------------------------
// midori UC_0005 del -->
//int CDBUchiImp::CmnGetNextPageRow(int* pPage, int* pRow,int intRowMaxData)
// midori UC_0005 del <--
// midori UC_0005 add -->
int CmnGetNextPageRow(int* pPage, int* pRow,int intRowMaxData)
// midori UC_0005 add <--
{
	// 次の行は改行が必要？
	if (*pRow + 1 > intRowMaxData) {
		*pPage = *pPage + 1;
		*pRow = 1;
	}
	else {
		*pRow = *pRow + 1;
	}
	
	return	FUNCTION_OK;
}

// -------------------------------------------------------------------------------------------------
//	特殊行挿入･･･１行のみ可能
//		IN		int		挿入位置（頁）
//				int		挿入位置（行）
//				int		特殊行の種別
//				CString	特殊行の表示名称
//		RET		int		実行結果（FUNCTION_OK, FUNCTION_NG）
// -------------------------------------------------------------------------------------------------
// midori UC_0005 del -->
//int CDBUchiImp::CmnTblInsertSpecialLine(CDBZmSub* pZmSub,int pFormSeq,int nPage, int nRow, int nType, CString strName)
// midori UC_0005 del <--
// midori UC_0005 add -->
int CmnTblInsertSpecialLine(CDBZmSub* pZmSub,int pFormSeq,int nPage, int nRow, int nType, CString strName)
// midori UC_0005 add <--
{
	// パラメータのチェック
	if (nPage <= 0 || nRow <= 0) {
		return FUNCTION_NG;
	}
	switch ( nType ) {
	case ID_FGFUNC_SYOKEI:		// 小計行
	case ID_FGFUNC_SYOKEINULL:	// 小計行（空行）
	case ID_FGFUNC_CHUKEI:		// 中計行
	case ID_FGFUNC_CHUKEINULL:	// 中計行（空行）
	case ID_FGFUNC_RUIKEI:		// 累計行
	case ID_FGFUNC_PAGEKEI:		// 頁計行
		break;
	default:
		return FUNCTION_NG;
	}

	// 帳表毎のテーブルクラスを"New"する
	CdbUc000Common*		rsData;
	rsData = GetDataTableObject(pZmSub,pFormSeq);

	// 新規レコード追加
	rsData->CreateNewRecord();	// レコード初期化済み

	// 特殊行データ編集（共通データ）
	rsData->Edit();
	rsData->m_NumPage = nPage;				// ページ番号
	rsData->m_NumRow = nRow;				// 行番号
	rsData->m_FgFunc = nType;				// 特殊行フラグ
	rsData->m_FgShow = ID_FGSHOW_OFF;		// 表示フラグ

	rsData->m_KeiStr = strName;

	// レコード更新
	rsData->Update();
	rsData->Fin();

	delete rsData;
	return FUNCTION_OK;
}

// -------------------------------------------------------------------------------------------------
//	<引数>
//		なし
//	<戻値>
//		 0	:	成功
//		-1	:	失敗
//	<機能>
//		小計、中計、頁計、累計の合計を求める
// -------------------------------------------------------------------------------------------------
// midori UC_0005 del -->
//int CDBUchiImp::CmnTblCalcKei(CDBZmSub* pZmSub,int pFormSeq,int intOutKei,BOOL bPrintZeroData,BOOL bPrintNullData)
// midori UC_0005 del <--
// midori UC_0005 add -->
int CmnTblCalcKei(CDBZmSub* pZmSub,int pFormSeq,int intOutKei,BOOL bPrintZeroData,BOOL bPrintNullData)
// midori UC_0005 add <--
{
	CALCKEI_INFO	uCalcInfo;
	CString		strSyoKei[CALCKEI_MAXCOUNT];		// 小計計算用
	CString		strChuKei[CALCKEI_MAXCOUNT];		// 中計計算用
	CString		strPageKei[CALCKEI_MAXCOUNT];		// 頁計計算用
	CString		strRuiKei[CALCKEI_MAXCOUNT];		// 累計計算用
	BOOL		bSyoKei[CALCKEI_MAXCOUNT];			// 小計用０表示フラグ
	BOOL		bChuKei[CALCKEI_MAXCOUNT];			// 中計用０表示フラグ
	BOOL		bPageKei[CALCKEI_MAXCOUNT];			// 頁計用０表示フラグ
	BOOL		bRuiKei[CALCKEI_MAXCOUNT];			// 累計用０表示フラグ
	CString		strTemp = "";
	int			intMax;
	int			retVal = FUNCTION_OK;
	int			i = 0;
	int			OutZeroNull = 0;

	for (i = 0; i < CALCKEI_MAXCOUNT; i++) {
		uCalcInfo.strField[i] = "";			// 集計処理：集計フィールド名称クリア		･･･ 帳表固有
		uCalcInfo.strKei[i] = "";			// 集計処理：集計値クリア
		uCalcInfo.bExistNum[i] = FALSE;		// 集計処理：表示フラグクリア
	}
	// 集計処理の設定
	switch(pFormSeq)	{
	case ID_FORMNO_011:			//	①	
		uCalcInfo.intMaxCount = 1;				// 集計フィールド件数
		uCalcInfo.strField[0] = _T("Val");		// 集計フィールド名称１
		break;
	case ID_FORMNO_021:			//	②	
		uCalcInfo.intMaxCount = 1;				// 集計フィールド件数
		uCalcInfo.strField[0] = _T("Val");		// 集計フィールド名称1
		break;
	case ID_FORMNO_031:			//	③	
		uCalcInfo.intMaxCount = 1;				// 集計フィールド件数
		uCalcInfo.strField[0] = _T("Val");		// 集計フィールド名称1
		break;
	case ID_FORMNO_041:			//	④-1
		uCalcInfo.intMaxCount = 1;				// 集計フィールド件数
		uCalcInfo.strField[0] = _T("Val");		// 集計フィールド名称1
		break;
	case ID_FORMNO_042:			//	④-2
		uCalcInfo.intMaxCount = 2;				// 集計フィールド件数
		uCalcInfo.strField[0] = _T("Val");		// 集計フィールド名称1
		uCalcInfo.strField[1] = _T("Risoku");	// 集計フィールド名称2
		break;
	case ID_FORMNO_051:			//	⑤
		uCalcInfo.intMaxCount = 1;				// 集計フィールド件数
		uCalcInfo.strField[0] = _T("Val");		// 集計フィールド名称１
		break;
	case ID_FORMNO_061:			//	⑥
		uCalcInfo.intMaxCount = 2;				// 集計フィールド件数
		uCalcInfo.strField[0] = _T("Val2");		// 集計フィールド名称1
		uCalcInfo.strField[1] = _T("Val3");		// 集計フィールド名称2
		break;
	case ID_FORMNO_071:			//	⑦
		uCalcInfo.intMaxCount = 3;				// 集計フィールド件数
		uCalcInfo.strField[0] = _T("Val");		// 集計フィールド名称1
		uCalcInfo.strField[1] = _T("MsVal");	// 集計フィールド名称2
		uCalcInfo.strField[2] = _T("MsVal2");	// 集計フィールド名称3
		break;
	case ID_FORMNO_081:			//	⑧
		uCalcInfo.intMaxCount = 1;				// 集計フィールド件数
		uCalcInfo.strField[0] = _T("Val");		// 集計フィールド名称1
		break;
	case ID_FORMNO_091:			//	⑨
		uCalcInfo.intMaxCount = 1;				// 集計フィールド件数
		uCalcInfo.strField[0] = _T("Val");		// 集計フィールド名称1
		break;
	case ID_FORMNO_101:			//	⑩-1
		uCalcInfo.intMaxCount = 1;				// 集計フィールド件数
		uCalcInfo.strField[0] = _T("Val");		// 集計フィールド名称1
		break;
	case ID_FORMNO_102:			//	⑩-2
		uCalcInfo.intMaxCount = 1;				// 集計フィールド件数
		uCalcInfo.strField[0] = _T("Val");		// 集計フィールド名称1
		break;
	case ID_FORMNO_111:			//	⑪
		uCalcInfo.intMaxCount = 2;				// 集計フィールド件数
		uCalcInfo.strField[0] = _T("Val");		// 集計フィールド名称1
		uCalcInfo.strField[1] = _T("Risoku");	// 集計フィールド名称2
		break;
	case ID_FORMNO_121:			//	⑫
		uCalcInfo.intMaxCount = 2;				// 集計フィールド件数
		uCalcInfo.strField[0] = _T("Val");		// 集計フィールド名称1
		uCalcInfo.strField[1] = _T("Val2");		// 集計フィールド名称2
		break;
	case ID_FORMNO_131:			//	⑬
		uCalcInfo.intMaxCount = 4;				// 集計フィールド件数
		uCalcInfo.strField[0] = _T("Val");		// 集計フィールド名称1
		uCalcInfo.strField[1] = _T("Val2");		// 集計フィールド名称2
		uCalcInfo.strField[2] = _T("Worker");	// 集計フィールド名称3
		uCalcInfo.strField[3] = _T("Area");		// 集計フィールド名称4
		break;
	case ID_FORMNO_141:			//	⑭-1
		uCalcInfo.intMaxCount = 7;				// 集計フィールド件数
		uCalcInfo.strField[0] = _T("EmVal");	// 集計フィールド名称1
		uCalcInfo.strField[1] = _T("PrVal");	// 集計フィールド名称2
		uCalcInfo.strField[2] = _T("BfVal");	// 集計フィールド名称3
		uCalcInfo.strField[3] = _T("PfVal");	// 集計フィールド名称4
		uCalcInfo.strField[4] = _T("EtVal");	// 集計フィールド名称5
		uCalcInfo.strField[5] = _T("RtVal");	// 集計フィールド名称6
		uCalcInfo.strField[6] = _T("Val");		// 集計フィールド名称6
		break;
	case ID_FORMNO_142:			//	⑭-2
		uCalcInfo.intMaxCount = 2;				// 集計フィールド件数
		uCalcInfo.strField[0] = _T("Val");		// 集計フィールド名称1
		uCalcInfo.strField[1] = _T("Val2");		// 集計フィールド名称2
		break;
	case ID_FORMNO_151:			//	⑮-1
		uCalcInfo.intMaxCount = 1;				// 集計フィールド件数
		uCalcInfo.strField[0] = _T("Val");		// 集計フィールド名称1
		break;
	case ID_FORMNO_152:			//	⑮-2
		uCalcInfo.intMaxCount = 1;				// 集計フィールド件数
		uCalcInfo.strField[0] = _T("Val");		// 集計フィールド名称1
		break;
	case ID_FORMNO_153:			//	⑮-3
		uCalcInfo.intMaxCount = 1;				// 集計フィールド件数
		uCalcInfo.strField[0] = _T("Val");		// 集計フィールド名称1
		break;
	case ID_FORMNO_161:			//	⑯-1
		uCalcInfo.intMaxCount = 1;				// 集計フィールド件数
		uCalcInfo.strField[0] = _T("Val");		// 集計フィールド名称1
		break;
	case ID_FORMNO_162:			//	⑯-2
		uCalcInfo.intMaxCount = 1;				// 集計フィールド件数
		uCalcInfo.strField[0] = _T("Val");		// 集計フィールド名称1
		break;
	case ID_FORMNO_171:			//	⑰ 1
	case ID_FORMNO_172:			//	⑰ 2
	case ID_FORMNO_173:			//	⑰ 3
	case ID_FORMNO_174:			//	⑰ 4
	case ID_FORMNO_175:			//	⑰ 5
	case ID_FORMNO_176:			//	⑰ 6
	case ID_FORMNO_177:			//	⑰ 7
	case ID_FORMNO_178:			//	⑰ 8
	case ID_FORMNO_179:			//	⑰ 9
	case ID_FORMNO_1710:		//	⑰ 10
	case ID_FORMNO_1711:		//	⑰ 11
	case ID_FORMNO_1712:		//	⑰ 12
	case ID_FORMNO_1713:		//	⑰ 13
	case ID_FORMNO_1714:		//	⑰ 14
	case ID_FORMNO_1715:		//	⑰ 15
	case ID_FORMNO_1716:		//	⑰ 16
	case ID_FORMNO_1717:		//	⑰ 17
	case ID_FORMNO_1718:		//	⑰ 18
	case ID_FORMNO_1719:		//	⑰ 19
	case ID_FORMNO_1720:		//	⑰ 20
		uCalcInfo.intMaxCount = 1;				// 集計フィールド件数
		uCalcInfo.strField[0] = _T("Val");		// 集計フィールド名称1
		break;
	case ID_FORMNO_181:			//	⑱ 1
	case ID_FORMNO_182:			//	⑱ 2
	case ID_FORMNO_183:			//	⑱ 3
	case ID_FORMNO_184:			//	⑱ 4
	case ID_FORMNO_185:			//	⑱ 5
	case ID_FORMNO_186:			//	⑱ 6
	case ID_FORMNO_187:			//	⑱ 7
	case ID_FORMNO_188:			//	⑱ 8
	case ID_FORMNO_189:			//	⑱ 9
	case ID_FORMNO_1810:		//	⑱ 10
	case ID_FORMNO_1811:		//	⑱ 11
	case ID_FORMNO_1812:		//	⑱ 12
	case ID_FORMNO_1813:		//	⑱ 13
	case ID_FORMNO_1814:		//	⑱ 14
	case ID_FORMNO_1815:		//	⑱ 15
	case ID_FORMNO_1816:		//	⑱ 16
	case ID_FORMNO_1817:		//	⑱ 17
	case ID_FORMNO_1818:		//	⑱ 18
	case ID_FORMNO_1819:		//	⑱ 19
	case ID_FORMNO_1820:		//	⑱ 20
		uCalcInfo.intMaxCount = 1;				// 集計フィールド件数
		uCalcInfo.strField[0] = _T("Val");		// 集計フィールド名称1
		break;
	default:
		break;
	}

	intMax = uCalcInfo.intMaxCount;

	// ０円or空欄を出力しないサイン取得
	if(bPrintZeroData)	OutZeroNull |= BIT_D0;
	if(bPrintNullData)	OutZeroNull |= BIT_D1;

	// 帳表毎のテーブルクラスを"New"する
	CdbUc000Common*		rs;
	rs = GetDataTableObject(pZmSub,pFormSeq);				// 全レコード取得用
	CdbUc000Common*		rsData;
	rsData = GetDataTableObject(pZmSub,pFormSeq);			// 集計値更新用：Field名称可変で集計値を更新するため

	// 頁番号の昇順、行番号の昇順でソートされたレコードセットを取得する。
	rs->RequeryFgShowSortPageRow(ID_FGSHOW_OFF);

	BOOL isZeroNullData = 0;
	CString strTemp2[CALCKEI_MAXCOUNT] = {0};
	// 初期化
	for (int i = 0; i < CALCKEI_MAXCOUNT; i++) {
		uCalcInfo.strKei[i] = "";

		strSyoKei[i].Format("%d", 0);
		strChuKei[i].Format("%d", 0);
		strPageKei[i].Format("%d", 0);
		strRuiKei[i].Format("%d", 0);

		bSyoKei[i]	= FALSE;
		bChuKei[i]	= FALSE;
		bPageKei[i] = FALSE;
		bRuiKei[i]	= FALSE;
	}

	// 全レコードに対し１行目から計算していく
	while (!rs->IsEOF()) {
		switch (rs->m_FgFunc) {
		case ID_FGFUNC_SYOKEI:		// 小計値をセット
			// 金額欄へ登録＋小計値を初期化
			for (int i = 0; i < intMax; i++) {
				uCalcInfo.strKei[i] = strSyoKei[i].TrimLeft();		// 集計値で更新
				strSyoKei[i].Format("%d", 0);						// クリア

				uCalcInfo.bExistNum[i] = bSyoKei[i];				// フラグ更新
				bSyoKei[i] = FALSE;									// クリア
			}
// midori 156189,156190,156191 del -->
			//rsData->UpdateCalcKei(rs->m_Seq, uCalcInfo);			// テーブルへ登録
// midori 156189,156190,156191 del <--
// midori 156189,156190,156191 add -->
			if(rs->m_ShowKeiZero & BIT_D7) {
				rsData->UpdateCalcKei(rs->m_Seq, uCalcInfo,1);			// テーブルへ登録
			}
			else {
				rsData->UpdateCalcKei(rs->m_Seq, uCalcInfo,0);			// テーブルへ登録
			}
// midori 156189,156190,156191 add <--
			break;

		case ID_FGFUNC_SYOKEINULL:
			// 小計値を初期化
			for (int i = 0; i < intMax; i++) {
				strSyoKei[i].Format("%d", 0);						// クリア
				bSyoKei[i] = FALSE;									// クリア
			}
			break;

		case ID_FGFUNC_CHUKEI:		// 中計値をセット
			// 金額欄へ登録＋中計値を初期化
			for (int i = 0; i < intMax; i++) {
				uCalcInfo.strKei[i] = strChuKei[i].TrimLeft();		// 集計値で更新
				strChuKei[i].Format("%d", 0);						// クリア

				uCalcInfo.bExistNum[i] = bChuKei[i];				// フラグ更新
				bChuKei[i] = FALSE;									// クリア
// midori M-17061302 add -->
				// 小計をクリアする
				strSyoKei[i].Format("%d", 0);						// クリア
				bSyoKei[i] = FALSE;									// クリア
// midori M-17061302 add <--
			}
// midori 157046 del -->
			//rsData->UpdateCalcKei(rs->m_Seq, uCalcInfo);			// テーブルへ登録
// midori 157046 del <--
// midori 157046 add -->
			if(rs->m_ShowKeiZero & BIT_D7) {
				rsData->UpdateCalcKei(rs->m_Seq, uCalcInfo,1);			// テーブルへ登録
			}
			else {
				rsData->UpdateCalcKei(rs->m_Seq, uCalcInfo,0);			// テーブルへ登録
			}
// midori 157046 add <--
			break;

		case ID_FGFUNC_CHUKEINULL:
			// 中計値を初期化
			for (int i = 0; i < intMax; i++) {
				strChuKei[i].Format("%d", 0);						// クリア
				bChuKei[i] = FALSE;									// クリア
			}
			break;

		case ID_FGFUNC_PAGEKEI:		// 頁計値をセット
			// 金額欄へ登録＋頁計値を初期化
			for (int i = 0; i < intMax; i++) {
				switch (intOutKei) {
				case ID_OUTKEI_OFF:			// なし
				case ID_OUTKEI_RUIKEI:		// 累計のみを挿入する
					uCalcInfo.strKei[i] = _T("");
					uCalcInfo.bExistNum[i] = FALSE;
					break;
				case ID_OUTKEI_PAGEKEI:		// 頁計のみを挿入する
				case ID_OUTKEI_BOTH:		// 累計と累計の両方を挿入する
					uCalcInfo.strKei[i] = strPageKei[i].TrimLeft();;	// 集計値で更新
					uCalcInfo.bExistNum[i] = bPageKei[i];				// フラグ更新
					break;
				}
				strPageKei[i].Format("%d", 0);						// クリア
				bPageKei[i] = FALSE;								// クリア
			}
			rsData->UpdateCalcKei(rs->m_Seq, uCalcInfo);			// テーブルへ登録
			break;

		case ID_FGFUNC_RUIKEI:		// 累計値をセット
			// 金額欄へ登録＋クリアしない
			for (int i = 0; i < intMax; i++) {
				switch (intOutKei) {
				case ID_OUTKEI_OFF:			// なし
				case ID_OUTKEI_PAGEKEI:		// 頁計のみを挿入する
					uCalcInfo.strKei[i] = _T("");
					uCalcInfo.bExistNum[i] = FALSE;
					break;
				case ID_OUTKEI_RUIKEI:		// 累計のみを挿入する
				case ID_OUTKEI_BOTH:		// 累計と累計の両方を挿入する
					uCalcInfo.strKei[i] = strRuiKei[i].TrimLeft();;		// 集計値で更新
					uCalcInfo.bExistNum[i] = bRuiKei[i];				// フラグ更新
					break;
				}
			}
			rsData->UpdateCalcKei(rs->m_Seq, uCalcInfo);			// テーブルへ登録
			break;

		default:
			isZeroNullData = 0;

			// 小計、中計、頁計、累計を足しこむ
			for (int i = 0; i < intMax; i++) {
				// フィールド名称から値を取得
				strTemp = rs->GetFieldValueString(uCalcInfo.strField[i]);
				strTemp = strTemp.TrimLeft();
				strTemp2[i] = strTemp;	// 下のforループでGetFieldValueStringをコールすると記述子インデックスが正しくありませんと例外を吐くので仕方なく

				// 加算
				CmnKingakuAddFunc(&strSyoKei[i], strTemp);
				CmnKingakuAddFunc(&strChuKei[i], strTemp);
				CmnKingakuAddFunc(&strPageKei[i], strTemp);
				CmnKingakuAddFunc(&strRuiKei[i], strTemp);

				if((OutZeroNull & BIT_D0) && (OutZeroNull & BIT_D1)){
					if(!(strTemp.IsEmpty() || (strTemp.Compare("0") == 0)))	isZeroNullData++;
				}
				else if(OutZeroNull & BIT_D0){
					if(strTemp.Compare("0") != 0)			isZeroNullData++;
				}
				else if(OutZeroNull & BIT_D1){
					if(!(strTemp.IsEmpty()))				isZeroNullData++;
				}
				else 		isZeroNullData++;
			}

			if(isZeroNullData == 0)		break;

			for (int i = 0; i < intMax; i++) {
				// 集計対象に1つでも入力があれば、計０円表示フラグを立てる
				if( bSyoKei[i] == FALSE )	bSyoKei[i]	= GetShowKeiZero( strTemp2[i]);		// 一回でもTRUEになったら書き換えない
				if( bChuKei[i] == FALSE )	bChuKei[i]	= GetShowKeiZero( strTemp2[i]);
				if( bPageKei[i] == FALSE )	bPageKei[i] = GetShowKeiZero( strTemp2[i]);
				if( bRuiKei[i] == FALSE )	bRuiKei[i]	= GetShowKeiZero( strTemp2[i]);
			}
			break;
		}

		rs->MoveNext();
	}

	rs->Fin();
	rsData->Fin();

	delete rs;
	delete rsData;

	return retVal;
}

//********************************************************************************
//	金額を加算/減算して合計値を求める
//	【引数】	CString *inTotalVal	…	合計金額値格納用変数のポインタ
//				CString inAddVal	…	加算金額値格納用変数のポインタ
//				int		inAddFlag	…	1:加算（省略可能），-1:減算
//	【戻り値】	なし
//********************************************************************************
// midori UC_0005 del -->
//void CDBUchiImp::CmnKingakuAddFunc(CString *inTotalVal, CString inAddVal, int inAddFlag)
// midori UC_0005 del <--
// midori UC_0005 add -->
void CmnKingakuAddFunc(CString *inTotalVal, CString inAddVal, int inAddFlag)
// midori UC_0005 add <--
{
	char	prmTotalVal[32];		// 引数の合計金額格納用（CString型をchar型で格納）
	char	prmAddVal[32];			// 引数の加算金額格納用（CString型をchar型で格納）
	char	tmpTotalVal[32];		// 計算後の合計金額格納用（char型で格納）
	char	btAddVal[32];			// 計算後の合計金額格納用（倍長フィールド）
	char	buffAnser[32];			// 計算後の合計金額格を金額フォーマットで格納（char型で格納）

	//--------------------------------------------------------------
	// 初期化
	//--------------------------------------------------------------
	ZeroMemory(prmTotalVal, sizeof(prmTotalVal));
	ZeroMemory(prmAddVal, sizeof(prmAddVal));
	ZeroMemory(tmpTotalVal, sizeof(tmpTotalVal));
	ZeroMemory(btAddVal, sizeof(btAddVal));
	ZeroMemory(buffAnser, sizeof(buffAnser));
	
	//--------------------------------------------------------------
	// CString -> char* 変換
	//--------------------------------------------------------------
	lstrcpy(prmTotalVal, *inTotalVal);
	lstrcpy(prmAddVal, inAddVal);

	//--------------------------------------------------------------
	// 加算/減算
	//--------------------------------------------------------------
	__int64	i64;

	// char型を__int64に変換して加算/減算
	if (inAddFlag == 1) {
		i64 = _strtoi64(prmTotalVal, NULL, 10) + _strtoi64(prmAddVal, NULL, 10);
	}
	else {
		i64 = _strtoi64(prmTotalVal, NULL, 10) - _strtoi64(prmAddVal, NULL, 10);
	}

	// 上限、下限チェック
	if (i64 < DB_DECIMAL_MIN) {
		i64 = DB_DECIMAL_MIN;
	} else if (DB_DECIMAL_MAX < i64) {
		i64 = DB_DECIMAL_MAX;
	}

	// 合計値をchar型へ変換
	_i64toa_s(i64, tmpTotalVal, 32, 10);

	// CString型変数にchar型の合計金額値を格納
	*inTotalVal = tmpTotalVal;

	// 以下の処理は必要なし
	/*
	//--------------------------------------------------------------
	//	合計金額値を金額フォーマットへ変換
	//--------------------------------------------------------------
	int		intDefn;				// 倍長フィールド保存用

	// 現在の倍長フィールド取得
	intDefn = l_retn();

	// 倍長フィールド指定
	l_defn( 0x16 );

	// 倍長フィールドへ変換
	l_input(btAddVal, tmpTotalVal);

	// 合計金額値を金額フォーマットへ変換
	l_print(buffAnser, btAddVal, "SSSSSSSSSSSSS9");
	
	// CString型変数にchar型の合計金額値を格納
	*inTotalVal = buffAnser;

	// 倍長フィールドを元に戻す
	l_defn( intDefn );
	*/
}

//********************************************************************************
//	０円計の表示フラグをゲットする
//		【引数】	CString データ行の金額
//					
//		【戻値】	BOOL* ０円計表示フラグ
//********************************************************************************
// midori UC_0005 del -->
//BOOL CDBUchiImp::GetShowKeiZero(CString strTemp)
// midori UC_0005 del <--
// midori UC_0005 add -->
BOOL GetShowKeiZero(CString strTemp)
// midori UC_0005 add <--
{
	BOOL nRet = TRUE;
	if(strTemp.IsEmpty() != FALSE)	nRet = FALSE;

	return nRet;
}
// midori 170502 add <--

// midori 170504 add -->
//**************************************************
//	文字列の先頭から指定位置まで取得（２５６バイトまで対応）
//	【引数】	szSrc		…	文字列（元）
//				szDst		…	文字列（先）
//				nPos		…	分割位置までのバイト数
//	【戻値】	指定位置以降の文字列を返す
//**************************************************
CString CDBUchiImp::GetSpritString( CString szSrc, CString* szDst, int nPos )
{
	CString					szRet;					//	戻値
	CString					cs;						//	戻値
	char					szBuf[AS_SPLIT_MAX];	//	バッファ
	char					szBuf1[AS_SPLIT_MAX];	//	バッファ
	char					szBuf2[AS_SPLIT_MAX];	//	バッファ
	const unsigned char*	pBuf = 0;				//	バッファ
	int						nLen;					//	文字列長
	int						nSplitPos;				//	分割位置
	
	//	初期化
	ZeroMemory( szBuf, sizeof( szBuf ) );
	ZeroMemory( szBuf1, sizeof( szBuf1 ) );
	ZeroMemory( szBuf2, sizeof( szBuf2 ) );
	szDst->Empty();
	szRet.Empty(); 

	//	文字列（元）長取得
	nLen = szSrc.GetLength();
	
	//	文字列（元）長か分割位置が最大文字数を超えている？
	if (( nLen > AS_SPLIT_MAX ) ||
		( nPos > AS_SPLIT_MAX )){
			//	抜ける
			return( szRet );
	}

	//	文字列長が分割位置に満たない？
	if ( nLen <= nPos ){
		//	文字列（元）をそのまま取得
		*szDst = szSrc;
	}
	else{
		//	分割位置を取得
		nSplitPos = nPos;
		
		//	分割位置までの文字列を取得
		strcpy_s( szBuf, sizeof( szBuf ), szSrc );

		if(_mbsbtype((LPBYTE)szBuf,nSplitPos) == _MBC_TRAIL)	nSplitPos--;

		memcpy(szBuf1,szBuf,nSplitPos);
		if((int)strlen(szBuf) > nSplitPos)	{
			memcpy(szBuf2,&szBuf[nSplitPos],strlen(szBuf) - nSplitPos);
		}

		//	文字列の先頭から指定位置まで取得
		cs.Format(_T("%s"),szBuf1);
		*szDst = cs;
		//	指定位置以降の文字列を取得
		szRet.Format(_T("%s"),szBuf2);
	}

	//	戻値を返す
	return( szRet );
}
// midori 170504 add <--

// midori 152376 add -->
//**********************************************************************
// 勘定科目内訳書翌期更新サインの取得
//
//	戻値
//			0:更新不可 1:更新可能 -1:エラー
//**********************************************************************
// midori UC_0009 del -->
//int ZenGetSgn(void)
//{
//	int				ret=0;				// 戻り値
//	int				rv=0;				// 別関数からの戻り値
//	int				ndate=0;			// 年月日（西暦）
//	int				zver=0;				// 勘定科目内訳書バージョン（当期）
//	char			svr[32]={0};		// SQLサーバー名
//	char			apno[32]={0};		// アプリケーション番号;
//	char			ccod[32]={0};		// 会社コード
//	char			date[32]={0};		// 期末年
//	CString			wmsg=_T("");
//	ICSMCTL*		mctl=NULL;			// マスター管理クラス
//	CDBZmSub*		tzms=NULL;			// [当年度]財務クラス
//	CdbUcInfMain*	prsMain;			// uc_inf_mainテーブルクラス
//	CO_CTL_DB		coctl;
//	CO_CTL_Array	coary;
//	CZmGen8			zg;
//	ZMGEN_CHKINFO	chkinfo={0};
//	CdbIcsVtbl*		vtbl = NULL;
//
//	// --------------------------------------------------------------------------------------
//	// マスター管理テーブルから当年度の情報を取得する
//	// --------------------------------------------------------------------------------------
//	// マスター管理クラス
//	mctl = new ICSMCTL;
//	// 選択している過年度マスターの情報を取得
//	PCOM_GetString(_T("SELECTCODSN"),svr,apno,ccod,date);
//	if(mctl->SetDatabase(svr) < 0)	{
//		sG_Err = _T("データベースの初期化に失敗しました。");
//		return(-1);
//	}
//	// オープン
//	if(mctl->vctl_open(MODE_READ,atoi(apno),0) < 0)	{
//		sG_Err = _T("マスター管理テーブルのオープンに失敗しました。");
//		return(-1);
//	}
//
//	// リード
//	coctl.c_apno = atoi(apno);
//	int_bcd(coctl.c_cod,atoi(ccod),4);
//	coctl.c_gsno = 0xff;
//	if(mctl->vctl_coinf(&coctl,&coary) < 0)	{
//		sG_Err = _T("会社情報の取得に失敗しました。");
//		return(-1);
//	}
//
//	// クローズ
//	mctl->vctl_close();
//
//	// --------------------------------------------------------------------------------------
//	// 当年度マスターｄｂに接続
//	// --------------------------------------------------------------------------------------
//	// ｄｂに接続
//	coctl = coary.GetAt(0);
//	ndate = bcd_int(&coctl.c_kikan[3],3);
//	ndate += (coctl.c_kikan[3] < 0x89) ? 20000000 : 19000000;
//	tzms = new CDBZmSub;
//	if(tzms->SetMasterDB(2,svr,(BYTE)coctl.c_apno,atoi(ccod),ndate) < 0)	{
//		sG_Err = tzms->GetZmErrMsg();
//		return(-1);
//	}
//
//	// マスタの状態をチェック
//	rv = zg.CheckDBMasterState(tzms,wmsg,chkinfo);
//	if((rv == -1) || (rv == -2))	{
//		sG_Err = zg.GetErrorMsg();
//		return(-1);
//	}
//	else if(rv == 1)	{
//		// 注意喚起のメッセージ表示をして、処理継続
//		ICSMessageBox(wmsg);
//	}
//	// 「入力データを確定する」サインの取得
//	// 勘定科目内訳書（当年度）のデータベースが有る場合、サインを取得する
//	// 当年度、勘定科目内訳書データベース作成チェック
//	// vtbl に type = 1001 AND objname='DBUchiwake' のレコードが存在する場合、内訳書関連テーブルが存在するとみなす
//	vtbl = new CdbIcsVtbl(tzms->m_database);
//
//	zver = vtbl->VersionCheck(DB_UC_VTBL_TYPE, DB_UC_VTBL_OBJNAME);
//
//	if (zver < 0) {	// DBエラー関連（Vtblアクセス時）
//		if (zver == DB_ERR_EOF) {	// レコードが無い
//			// レコードが無い場合、前年度からの更新を不可とする
//			return(0);
//		}
//		else	{
//			// エラー
//			sG_Err = tzms->GetZmErrMsg();
//			return(-1);
//		}
//	}
//
//	// アプリケーション全体の管理情報テーブル(uc_inf_main)
//	rv  = tzms->object_check(2,_T("uc_inf_main"));
//	ret = 0;
//	if(rv != 0)	{
//		prsMain = new CdbUcInfMain(tzms->m_database);
//		if(prsMain->Init() == DB_ERR_OK){
//			// 当期入力データを確定するサインが無効の場合、前年度からの更新を可とする
//			if((prsMain->m_GeneralVar[1] & BIT_D2) == 0)	ret = 1;
//		}
//		prsMain->Fin();
//		delete prsMain;
//	}
//
//	// マスター管理クラス
//	if(mctl != NULL)	{
//		delete mctl;
//		mctl = NULL;
//	}
//
//	// 財務クラス
//	if(tzms != NULL)	{
//		delete tzms;
//		tzms = NULL;
//	}
//
//	return(ret);
//}
// midori UC_0009 del <--
// midori UC_0009 add -->
int ZenGetSgn(void)
{
	int				ret=0;				// 戻り値
	int				rv=0;				// 別関数からの戻り値
	int				ndate=0;			// 年月日（西暦）
	int				zver=0;				// 勘定科目内訳書バージョン（当期）
	int				tsw=0,zsw=0;		// 
	int				ii=0;				// 
	short			ind=0;				// 
	char			svr[32]={0};		// SQLサーバー名
	char			apno[32]={0};		// アプリケーション番号;
	char			ccod[32]={0};		// 会社コード
	char			date[32]={0};		// 期末年
	CRecordset*		prs;				// 
	CString			wmsg=_T("");		// 
	CString			strSQL=_T("");		// 
	CString			cs=_T("");			// 
	ICSMCTL*		mctl=NULL;			// マスター管理クラス
	CDBZmSub*		tzms=NULL;			// [当年度]財務クラス
	CDBZmSub*		zzms=NULL;			// [前年度]財務クラス
	CdbUcInfMain*	prsMain;			// uc_inf_mainテーブルクラス
	CO_CTL_DB		coctl;
	CO_CTL_Array	coary;
	CZmGen8			zg;
	ZMGEN_CHKINFO	chkinfo={0};
	CdbIcsVtbl*		vtbl = NULL;

	// --------------------------------------------------------------------------------------
	// マスター管理テーブルから当年度の情報を取得する
	// --------------------------------------------------------------------------------------
	// マスター管理クラス
	mctl = new ICSMCTL;
	// 選択している過年度マスターの情報を取得
	PCOM_GetString(_T("SELECTCODSN"),svr,apno,ccod,date);
	if(mctl->SetDatabase(svr) < 0)	{
		sG_Err = _T("データベースの初期化に失敗しました。");
		return(-1);
	}
	// オープン
	if(mctl->vctl_open(MODE_READ,atoi(apno),0) < 0)	{
		sG_Err = _T("マスター管理テーブルのオープンに失敗しました。");
		return(-1);
	}

	// リード
	coctl.c_apno = atoi(apno);
	int_bcd(coctl.c_cod,atoi(ccod),4);
	coctl.c_gsno = 0xff;
	if(mctl->vctl_coinf(&coctl,&coary) < 0)	{
		sG_Err = _T("会社情報の取得に失敗しました。");
		return(-1);
	}

	// クローズ
	mctl->vctl_close();

	// --------------------------------------------------------------------------------------
	// 当年度マスターｄｂに接続
	// --------------------------------------------------------------------------------------
	// ｄｂに接続
	coctl = coary.GetAt(0);
	ndate = bcd_int(&coctl.c_kikan[3],3);
	ndate += (coctl.c_kikan[3] < 0x89) ? 20000000 : 19000000;
	tzms = new CDBZmSub;
// midori UC_0036 del -->
//// midori 190302 del -->
//	//if(tzms->SetMasterDB(2,svr,(BYTE)coctl.c_apno,atoi(ccod),ndate) < 0)	{
//// midori 190302 del <--
//// midori 190302 add -->
//	if(tzms->SetMasterDB(3,svr,(BYTE)coctl.c_apno,atoi(ccod),ndate) < 0)	{
//// midori 190302 add <--
// midori UC_0036 del <--
// midori UC_0036 add -->
	if(tzms->SetMasterDB(DB_ICS_VERSION,svr,(BYTE)coctl.c_apno,atoi(ccod),ndate) < 0)	{
// midori UC_0036 add <--
		sG_Err = tzms->GetZmErrMsg();
		return(-1);
	}

// midori UC_0051 add -->
	chkinfo.verupsub = 0x800;
// midori UC_0051 add <--
	// マスタの状態をチェック
	rv = zg.CheckDBMasterState(tzms,wmsg,chkinfo);
	if((rv == -1) || (rv == -2))	{
		sG_Err = zg.GetErrorMsg();
		return(-1);
	}
	else if(rv == 1)	{
		// 注意喚起のメッセージ表示をして、処理継続
		ICSMessageBox(wmsg);
	}
// midori UC_0010 add -->
	rv = ChkUti(tzms);
	if(rv == 1)	{
		rv = VersionUp(tzms,svr,(int)coctl.c_apno,atoi(ccod),ndate,0);
		if(rv != 0)	{
			return(-1);
		}
	} else if(rv == -1)	{
		return(-1);
	}
// midori UC_0010 add <--

	// --------------------------------------------------------------------------------------
	// 前年度マスターｄｂに接続
	// --------------------------------------------------------------------------------------
	// ｄｂに接続
	zzms = new CDBZmSub;
// midori UC_0036 del -->
//// midori 190302 del -->
//	//if(zzms->SetMasterDB(2,svr,atoi(apno),atoi(ccod),atoi(date)) < 0)	{
//// midori 190302 del <--
//// midori 190302 add -->
//	if(zzms->SetMasterDB(3,svr,atoi(apno),atoi(ccod),atoi(date)) < 0)	{
//// midori 190302 add <--
// midori UC_0036 del <--
// midori UC_0036 add -->
	if(zzms->SetMasterDB(DB_ICS_VERSION,svr,atoi(apno),atoi(ccod),atoi(date)) < 0)	{
// midori UC_0036 add <--
		sG_Err = tzms->GetZmErrMsg();
		return(-1);
	}

	// マスタの状態をチェック
	rv = zg.CheckDBMasterState(zzms,wmsg,chkinfo);
	if((rv == -1) || (rv == -2))	{
		sG_Err = zg.GetErrorMsg();
		return(-1);
	}
	else if(rv == 1)	{
		// 注意喚起のメッセージ表示をして、処理継続
		ICSMessageBox(wmsg);
	}

// midori UC_0010 add -->
	rv = ChkUti(zzms);
	if(rv == 1)	{
		rv = VersionUp(zzms,svr,atoi(apno),atoi(ccod),atoi(date),0);
		if(rv != 0)	{
			return(-1);
		}
	} else if(rv == -1)	{
		return(-1);
	}
// midori UC_0010 add <--

// midori UC_0010 del -->
	//// 「入力データを確定する」サインの取得
	//// 勘定科目内訳書（当年度）のデータベースが有る場合、サインを取得する
	//// 当年度、勘定科目内訳書データベース作成チェック
	//// vtbl に type = 1001 AND objname='DBUchiwake' のレコードが存在する場合、内訳書関連テーブルが存在するとみなす
	//vtbl = new CdbIcsVtbl(tzms->m_database);

	//zver = vtbl->VersionCheck(DB_UC_VTBL_TYPE, DB_UC_VTBL_OBJNAME);

	//if (zver < 0) {	// DBエラー関連（Vtblアクセス時）
	//	if (zver == DB_ERR_EOF) {	// レコードが無い
	//		// レコードが無い場合、前年度からの更新を不可とする
	//		return(0);
	//	}
	//	else	{
	//		// エラー
	//		sG_Err = tzms->GetZmErrMsg();
	//		return(-1);
	//	}
	//}
// midori UC_0010 del <--

	// 当年度、前年度いずれかの uc_inf_main.GeneralVar が存在しない場合は更新を不可とする
	ret = 0;
	tsw = 0;
	zsw = 0;
	for(ii=0; ii<2; ii++)	{
		strSQL = _T("SELECT COLUMNPROPERTY(OBJECT_ID('uc_inf_main'),'GeneralVar','PRECISION')");
		if(ii == 0)	prs = new CRecordset(tzms->m_database);
		else		prs = new CRecordset(zzms->m_database);
		prs->Open(CRecordset::forwardOnly,strSQL);
		prs->GetFieldValue(ind,cs);
		if(ii == 0)	tsw = _tstoi(cs);
		else		zsw = _tstoi(cs);
		prs->Close();
		delete prs;
		prs=0;
	}
	if(tsw != 0 && zsw != 0)	{
		// アプリケーション全体の管理情報テーブル(uc_inf_main)
		rv  = tzms->object_check(2,_T("uc_inf_main"));
		if(rv != 0)	{
			prsMain = new CdbUcInfMain(tzms->m_database);
			if(prsMain->Init() == DB_ERR_OK){
				// 当期入力データを確定するサインが無効の場合、前年度からの更新を可とする
				if((prsMain->m_GeneralVar[1] & BIT_D2) == 0)	ret = 1;
			}
			prsMain->Fin();
			delete prsMain;
		}
	}

	// マスター管理クラス
	if(mctl != NULL)	{
		delete mctl;
		mctl = NULL;
	}

	// 財務クラス
	if(tzms != NULL)	{
		delete tzms;
		tzms = NULL;
	}
	if(zzms != NULL)	{
		delete zzms;
		zzms = NULL;
	}

	return(ret);
}
// midori UC_0009 add <--
// midori 152376 add <--

// midori UC_0005 add -->
//**********************************************************************
// 勘定科目内訳書マスターが作成されているかの確認関数
//
//	引数
//			pZmsub					[IN] 財務クラス
//	戻値
//			1:作成されている　0:作成されていない	-1:エラー
//**********************************************************************
int ChkUti( CDBZmSub* pZmsub )
{
	int				ver=0;				// 勘定科目内訳書バージョン
	int				ret=0;				// 返送値
	CdbIcsVtbl*		vtbl = NULL;
	
	// 勘定科目内訳書データベース作成チェック
	// vtbl に type = 1001 AND objname='DBUchiwake' のレコードが存在する場合、内訳書関連テーブルが存在するとみなす
	vtbl = new CdbIcsVtbl(pZmsub->m_database);

	ver = vtbl->VersionCheck(DB_UC_VTBL_TYPE, DB_UC_VTBL_OBJNAME);

	if (ver < 0) {	// DBエラー関連（Vtblアクセス時）
		if (ver == DB_ERR_EOF) {	// レコードが無い
			// レコードが無い場合、処理を行わない
			ret = 0;
		}
		else	{
			// エラー
// midori UC_0010 del -->
//			sG_Err = pG_Zzms->GetZmErrMsg();
// midori UC_0010 del <--
// midori UC_0010 add -->
			sG_Err = pZmsub->GetZmErrMsg();
// midori UC_0010 add <--
			ret = -1;
		}
	}
	else	{
		ret = 1;
	}
// midori UC_0012 add -->
	delete vtbl;
// midori UC_0012 add <--

	return(ret);
}

//**********************************************************************
//（更新関数）
// 翌期更新時、選択したマスターの確定サインを更新し
// 翌年度のマスターの確定サインをOFFにする
//
//	引数
//			pWnd					[IN] ウインドウハンドル
//			pKakutei				[IN] 「当期のマスターを確定とする」のサイン 0:チェック無し 1:チェック有り
//	戻値
//			0:正常終了	-1:エラー
//**********************************************************************
int YokUpdate( CWnd* pWnd, int pKakutei )
{
	int				rv=0;
	int				ndate=0;
// midori UC_0009 add -->
	int				sw=0;
	short			ind=0;
	CRecordset*		prs;
// midori UC_0009 add <--
// midori 180101 add -->
	int				symd=0,eymd=0;
// midori 180101 add <--
	ZMGEN_CHKINFO	chkinfo={0};
	CZmGen8			zg;
	CO_CTL_DB		coctl;
	CO_CTL_Array	coary;
	CString			wmsg=_T("");
	CString			strSQL=_T("");
// midori UC_0009 add -->
	CString			cs=_T("");
// midori UC_0009 add <--
// midori 180101 add -->
	CVolDateDB		icsVolDate;			// DB期間モジュール
// midori 180101 add <--

	// 選択しているマスターの情報を取得
	PCOM_GetString(_T("SELECTCODSN"),sG_Svr,sG_Apno,sG_Ccod,sG_Date);

	// マスター管理クラス
	pG_Mctl = new ICSMCTL;
	if(pG_Mctl->SetDatabase(sG_Svr) < 0)	{
		sG_Err = _T("データベースの初期化に失敗しました。");
		return(-1);
	}

	// 財務クラス
	pG_Tzms = new CDBZmSub;
	pG_Zzms = new CDBZmSub;

	// --------------------------------------------------------------------------------------
	// マスター管理テーブルから翌年度の情報を取得する
	// --------------------------------------------------------------------------------------
	// オープン
	if(pG_Mctl->vctl_open(MODE_READ,atoi(sG_Apno),0) < 0)	{
		sG_Err = _T("マスター管理テーブルのオープンに失敗しました。");
		return(-1);
	}

	// リード
	coctl.c_apno = atoi(sG_Apno);
	int_bcd(coctl.c_cod,atoi(sG_Ccod),4);
	coctl.c_gsno = 0xff;
	if(pG_Mctl->vctl_coinf(&coctl,&coary) < 0)	{
		sG_Err = _T("会社情報の取得に失敗しました。");
		return(-1);
	}

	// クローズ
	pG_Mctl->vctl_close();

	// --------------------------------------------------------------------------------------
	// 翌年度のマスターｄｂに接続
	// --------------------------------------------------------------------------------------
	// ｄｂに接続
	coctl = coary.GetAt(0);
	ndate = bcd_int(&coctl.c_kikan[3],3);
	ndate += (coctl.c_kikan[3] < 0x89) ? 20000000 : 19000000;
// midori UC_0036 del -->
//// midori 190302 del -->
//	//if(pG_Tzms->SetMasterDB(2,sG_Svr,(BYTE)coctl.c_apno,atoi(sG_Ccod),ndate) < 0)	{
//// midori 190302 del <--
//// midori 190302 add -->
//	if(pG_Tzms->SetMasterDB(3,sG_Svr,(BYTE)coctl.c_apno,atoi(sG_Ccod),ndate) < 0)	{
//// midori 190302 add <--
// midori UC_0036 del <--
// midori UC_0036 add -->
	if(pG_Tzms->SetMasterDB(DB_ICS_VERSION,sG_Svr,(BYTE)coctl.c_apno,atoi(sG_Ccod),ndate) < 0)	{
// midori UC_0036 add <--
		sG_Err = pG_Tzms->GetZmErrMsg();
		return(-1);
	}

// midori UC_0051 add -->
	chkinfo.verupsub = 0x800;
// midori UC_0051 add <--
	// マスタの状態をチェック
	rv = zg.CheckDBMasterState(pG_Tzms,wmsg,chkinfo);
	if((rv == -1) || (rv == -2))	{
		sG_Err = zg.GetErrorMsg();
		return(-1);
	}
	else if(rv == 1)	{
		// 注意喚起のメッセージ表示をして、処理継続
		ICSMessageBox(wmsg);
	}
// midori UC_0010 add -->
	rv = VersionUp(pG_Tzms,sG_Svr,(int)coctl.c_apno,atoi(sG_Ccod),ndate,0);
	if(rv != 0)	{
		return(-1);
	}
// midori UC_0010 add <--

	// --------------------------------------------------------------------------------------
	// 選択したマスターｄｂに接続
	// --------------------------------------------------------------------------------------
	// ｄｂに接続
// midori UC_0036 del -->
//// midori 190302 del -->
//	//if(pG_Zzms->SetMasterDB(2,sG_Svr,atoi(sG_Apno),atoi(sG_Ccod),atoi(sG_Date)) < 0)	{
//// midori 190302 del <--
//// midori 190302 add -->
//	if(pG_Zzms->SetMasterDB(3,sG_Svr,atoi(sG_Apno),atoi(sG_Ccod),atoi(sG_Date)) < 0)	{
//// midori 190302 add <--
// midori UC_0036 del <--
// midori UC_0036 add -->
	if(pG_Zzms->SetMasterDB(DB_ICS_VERSION,sG_Svr,atoi(sG_Apno),atoi(sG_Ccod),atoi(sG_Date)) < 0)	{
// midori UC_0036 add <--
		sG_Err = pG_Zzms->GetZmErrMsg();
		return(-1);
	}

	// マスタの状態チェック
	rv = zg.CheckDBMasterState(pG_Zzms,wmsg,chkinfo);
	if((rv == -1) || (rv == -2))	{
		sG_Err = zg.GetErrorMsg();
		return(-1);
	}
	else if(rv == 1)	{
		// 注意喚起のメッセージ表示をして、処理継続
		ICSMessageBox(wmsg);
	}
// midori UC_0010 add -->
	rv = VersionUp(pG_Zzms,sG_Svr,atoi(sG_Apno),atoi(sG_Ccod),atoi(sG_Date),0);
	if(rv != 0)	{
		return(-1);
	}
// midori UC_0010 add <--

// midori 180101 del -->
	//if(pG_Zzms->VolumeOpen() != 0)	{
	//	sG_Err = pG_Zzms->GetZmErrMsg();
	//	return(-1);
	//}
	//pG_Zzms->VolumeClose();
// midori 180101 del <--

	// 選択したマスターの入力確定サインを更新する
	// アプリケーション全体の管理情報テーブル(uc_inf_main)
// midori UC_0009 del -->
	//rv  = pG_Zzms->object_check(2,_T("uc_inf_main"));
	//if(rv != 0)	{
	//	// 入力確定サインを更新する
	//	strSQL.Empty();
	//	if(pKakutei == 1)	{
	//		strSQL = _T("UPDATE uc_inf_main SET GeneralVar = [dbo].[set_byte2](GeneralVar,1,4,1) ");
	//	}
	//	strSQL += _T("UPDATE uc_inf_main SET GeneralVar = [dbo].[set_byte2](GeneralVar,1,8,1) ");
	//	strSQL += _T("UPDATE uc_inf_main SET GeneralVar = [dbo].[set_byte2](GeneralVar,2,1,1) ");
	//	if(ExecuteSQLWork(pG_Zzms->m_database,strSQL) != DB_ERR_OK)	{
	//		sG_Err = _T("テーブル(uc_inf_main)の更新に失敗しました。");
	//		return(-1);
	//	}
	//}

	//// 翌年度の入力確定サインを更新する
	//// アプリケーション全体の管理情報テーブル(uc_inf_main)
	//rv  = pG_Tzms->object_check(2,_T("uc_inf_main"));
	//if(rv != 0)	{
	//	strSQL = _T("UPDATE uc_inf_main SET GeneralVar = [dbo].[set_byte2](GeneralVar,1,4,0) ");
	//	strSQL += _T("UPDATE uc_inf_main SET GeneralVar = [dbo].[set_byte2](GeneralVar,1,8,0) ");
	//	strSQL += _T("UPDATE uc_inf_main SET GeneralVar = [dbo].[set_byte2](GeneralVar,2,1,1) ");
	//	if(ExecuteSQLWork(pG_Tzms->m_database,strSQL) != DB_ERR_OK)	{
	//		sG_Err = _T("テーブル(uc_inf_main)の更新に失敗しました。");
	//		return(-1);
	//	}
	//}
// midori UC_0009 del <--
// midori UC_0009 add -->
	rv  = pG_Zzms->object_check(2,_T("uc_inf_main"));
	if(rv != 0)	{
		sw = 0;
		strSQL = _T("SELECT COLUMNPROPERTY(OBJECT_ID('uc_inf_main'),'GeneralVar','PRECISION')");
		prs = new CRecordset(pG_Zzms->m_database);
		prs->Open(CRecordset::forwardOnly,strSQL);
		prs->GetFieldValue(ind,cs);
		sw = _tstoi(cs);
		prs->Close();
		delete prs;
		prs=0;
		if(sw != 0)	{
			// 入力確定サインを更新する
			strSQL.Empty();
			if(pKakutei == 1)	{
				strSQL = _T("UPDATE uc_inf_main SET GeneralVar = [dbo].[set_byte2](GeneralVar,1,4,1) ");
			}
			strSQL += _T("UPDATE uc_inf_main SET GeneralVar = [dbo].[set_byte2](GeneralVar,1,8,1) ");
			strSQL += _T("UPDATE uc_inf_main SET GeneralVar = [dbo].[set_byte2](GeneralVar,2,1,1) ");
			if(ExecuteSQLWork(pG_Zzms->m_database,strSQL) != DB_ERR_OK)	{
				sG_Err = _T("テーブル(uc_inf_main)の更新に失敗しました。");
				return(-1);
			}
		}
	}

	// 翌年度の入力確定サインを更新する
	// アプリケーション全体の管理情報テーブル(uc_inf_main)
	rv  = pG_Tzms->object_check(2,_T("uc_inf_main"));
	if(rv != 0)	{
		sw = 0;
		strSQL = _T("SELECT COLUMNPROPERTY(OBJECT_ID('uc_inf_main'),'GeneralVar','PRECISION')");
		prs = new CRecordset(pG_Tzms->m_database);
		prs->Open(CRecordset::forwardOnly,strSQL);
		prs->GetFieldValue(ind,cs);
		sw = _tstoi(cs);
		prs->Close();
		delete prs;
		prs=0;
		if(sw != 0)	{
			strSQL = _T("UPDATE uc_inf_main SET GeneralVar = [dbo].[set_byte2](GeneralVar,1,4,0) ");
			strSQL += _T("UPDATE uc_inf_main SET GeneralVar = [dbo].[set_byte2](GeneralVar,1,8,0) ");
			strSQL += _T("UPDATE uc_inf_main SET GeneralVar = [dbo].[set_byte2](GeneralVar,2,1,1) ");
			if(ExecuteSQLWork(pG_Tzms->m_database,strSQL) != DB_ERR_OK)	{
				sG_Err = _T("テーブル(uc_inf_main)の更新に失敗しました。");
				return(-1);
			}
		}
// midori 154696 190425 add -->
// midori UC_0039 del -->
		//// 初回起動フラグをクリア
// midori UC_0039 del <--
// midori UC_0039 add -->
		// 簡素化様式対応プロで起動済みフラグをＯＦＦ
		// 翌期更新を行った後、一度も内訳書を起動していないフラグをＯＮ
// midori UC_0039 add <--
		CdbUcInfMain	mfcMain(pG_Tzms->m_database);
		if(mfcMain.Init() == DB_ERR_OK) {
// midori 155163 del -->
//			if((mfcMain.m_GeneralVar[2] & BIT_D4) != 0) {
//				mfcMain.Edit();
//				mfcMain.m_GeneralVar[2] &= ~BIT_D4;
//// midori UC_0039 add -->
//				mfcMain.m_GeneralVar[2] |= BIT_D6;
//// midori UC_0039 add <--
//				mfcMain.Update();
//			}
//			mfcMain.Fin();
// midori 155163 del <--
// midori 155163 add -->
			// 翌期更新を行った後、一度も内訳書を起動していないフラグが
			// 簡素化様式対応プロで起動済みフラグがＯＮのときしかＯＮになっていなかった
			mfcMain.Edit();
			if((mfcMain.m_GeneralVar[2] & BIT_D4) != 0) {
				mfcMain.m_GeneralVar[2] &= ~BIT_D4;
			}
// 修正No.168430 add -->
			// 登録番号追加版
			if((mfcMain.m_GeneralVar[4] & BIT_D4) != 0) {
				mfcMain.m_GeneralVar[4] &= ~BIT_D4;
			}
// 修正No.168430 add <--
			mfcMain.m_GeneralVar[2] |= BIT_D6;
			mfcMain.Update();
			mfcMain.Fin();
// midori 155163 add <--
		}
// midori 154696 190425 add <--
	}
// midori UC_0009 add <--

// midori 153382 del -->
//// midori 180101 add -->
//	// OwnTbテーブルをオープン
//	if ( pG_Tzms->OwnTblOpen() != 0 ) {
//		sG_Err = pG_Zzms->GetZmErrMsg();
//		return(-1);
//	}
//	try {
//		pG_Tzms->owntb->Requery(_T("apl_name='UCHIWAKE' and itm_name='HyoshiData' and itm_seq = 0"));
//		if(pG_Tzms->owntb->GetRecordCount() > 0)	{
//			coctl = coary.GetAt(0);
//			// 更新後の期首を取得
//			symd = bcd_int(&coctl.c_kikan[0],3);
//			symd += (coctl.c_kikan[0] < 0x89) ? 20000000 : 19000000;
//			// 更新後の期末を取得
//			eymd = bcd_int(&coctl.c_kikan[3],3);
//			eymd += (coctl.c_kikan[3] < 0x89) ? 20000000 : 19000000;
//
//			// 暦年表示設定が和暦の場合は和暦に変換する
//			if(pG_Tzms->owntb->vl[2] == 0)	{
//				icsVolDate.db_datecnv(symd,&symd,0,0);		// 0:西暦→平成へ変換，0:年月日より取得
//				icsVolDate.db_datecnv(eymd,&eymd,0,0);		// 0:西暦→平成へ変換，0:年月日より取得
//			}
//
//			// 決算期間　自、至を更新
//			pG_Tzms->owntb->Edit();
//			pG_Tzms->owntb->vl[0] = symd;	// 決算期間（自）
//			pG_Tzms->owntb->vl[1] = eymd;	// 決算期間（至）
//			pG_Tzms->owntb->Update();
//		}
//	}
//	catch (CDBException *e) {
//		sG_Err = _T("テーブル(owntb)の更新に失敗しました。");
//		e->Delete();
//		// OwnTbテーブルをクローズ
//		pG_Tzms->OwnTblClose();
//		return(-1);
//	}
//
//	// OwnTbテーブルをクローズ
//	pG_Tzms->OwnTblClose();
//// midori 180101 add <--
// midori 153382 del <--

// midori UC_0013 add -->
	// マスター管理クラス
	if(pG_Mctl != NULL)	{
		delete pG_Mctl;
		pG_Mctl = NULL;
	}

	// 財務クラス
	if(pG_Tzms != NULL)	{
		delete pG_Tzms;
		pG_Tzms = NULL;
	}

	if(pG_Zzms != NULL)	{
		delete pG_Zzms;
		pG_Zzms = NULL;
	}
// midori UC_0013 <--

	return(0);
}
// midori UC_0005 add <--

// midori UC_0010 add -->
/************************************************************************
	CheckAndMakeTableSub()
		SQLスクリプト(ファイル)を実行

		sqlcmdコマンドラインアプリを使用して実行する。	（ver1.0.1.12から　それ以前はosql）

	引数：	CString inFileName			ファイル名
			CString inSQLServer			SQLサーバ名
			CString	inDatabaseName		データベース名
			CString inSQLUse			ファイルの用途(例："作成/初期化用")
			int     mode				0:SQL作成 1:SQL更新
			int     cosel				0:会社起動時 or 更新時　1:会社選択ツールバーからの切り替え時　2:業務ツールバーからの会社切り替え時)

	戻値：	int							-1:ファイルオープンエラー -2:SQLファイル実行エラー 0:正常
*************************************************************************/
int CheckAndMakeTableSub	( CString inFileName
										, CString inSQLServer
										, CString inDatabaseName
										, CString inSQLUse
										, int	  mode
										, int	  cosel
										)
{
	CFileFind	FileFind;
	CString		errStr=_T("");

	// ファイル存在チェック
	if ( !FileFind.FindFile( inFileName ) ){
		
		// 業務ツールバーからの会社切り替え時はエラーメッセージ無し
		if ( cosel != 2 ){
			CString strMsg;
			strMsg = "勘定科目内訳書マスターの「" + inSQLUse + "ファイル」\r\nを検索出来ませんでした。";
			ICSMessageBox( strMsg );
		}
		return(-1);
	}

	if(ExecuteSqlBySMO(inSQLServer,inDatabaseName,inFileName,errStr) == -1)	{
		return(-2);
	}

	return(0);
}

/************************************************************************
	CheckAndMakeTable()

		内訳書で必要なテーブル、初期データを作成する

	引数：
			CString inSQLServer			SQLサーバ名
			CString	inDatabaseName		データベース名
			CString inFileName			ファイル名(フルパス)
			CString inSQLUse			ファイルの用途(例："作成/初期化用")
			int     mode				0:SQL作成 1:SQL更新
			int     cosel				0:会社起動時 or 更新時　1:会社選択ツールバーからの切り替え時　2:業務ツールバーからの会社切り替え時)
	戻値：	int							-1:ファイルオープンエラー -2:SQLファイル実行エラー 0:正常
*************************************************************************/
int CheckAndMakeTable( CString inSQLServer
									, CString inDatabaseName
									, CString inFileName
									, CString inSQLUse
									, int     mode
									, int	  cosel
									)
{
	int nRet;

	// 内訳書テーブル作成～初期値データ登録 SQL
	nRet = CheckAndMakeTableSub( inFileName, inSQLServer, inDatabaseName, inSQLUse, mode, cosel );

	return(nRet);
}

/************************************************************************
	CheckTable()
		テーブルの重複を修正

	引数：	なし

	戻値：	0:正常 1:エラー
*************************************************************************/
int CheckTable( CDBZmSub* pZms )
{
	int					ii=0;
	int					id=0;
	CdbUcInfSub			mfcRecSub(pZms->m_database);	// uc_inf_subテーブルクラス
	CString				strQuery=_T("");
	CString				strData[6];
	CRecordset			rs(pZms->m_database);

	// uc_lst_item_sort
	for(ii=0;ii<6;ii++)	{
		strData[ii].Empty();
	}
	try	{
		strQuery = _T("SELECT COUNT(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 2 and ItemSeq = 8 GROUP BY FormSeq,ItemSeq");
		rs.Open(CRecordset::forwardOnly, strQuery);
		rs.GetFieldValue((short)0,strData[0]);
		rs.Close();
	}
	catch(CException* e)	{
		e->Delete();
		return(-1);
	}

	if(_tstoi(strData[0]) > 1)	{
		// レコードを削除し、再作成
		pZms->m_database->ExecuteSQL(_T("DELETE FROM uc_lst_item_sort WHERE FormSeq = 2 and ItemSeq = 8"));

		strQuery = _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strQuery += _T("VALUES(2, 8, '科目', 1, 1, 1, 'KnOrder ASC', 'KnOrder', NULL, 'KnKana COLLATE Japanese_CS_AS_KS_WS ASC, KnOrder ASC')");
		pZms->m_database->ExecuteSQL(strQuery);
	}

	// uc_inf_sub_omit
	for(ii=0;ii<6;ii++)	{
		strData[ii].Empty();
	}
	try	{
		strQuery = _T("SELECT COUNT(FormSeq) FROM uc_inf_sub_omit WHERE FormSeq = 16 and ItemSeq = 4 GROUP BY FormSeq,ItemSeq");
		rs.Open(CRecordset::forwardOnly, strQuery);
		rs.GetFieldValue((short)0,strData[0]);
		rs.Close();
	}
	catch(CException* e)	{
		e->Delete();
		return(-1);
	}

	if(_tstoi(strData[0]) > 1)	{
		// データを取得
		strData[0].Empty();
		try	{
			strQuery = _T("SELECT FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen FROM uc_inf_sub_omit WHERE FormSeq = 16 and ItemSeq = 4");
			rs.Open(CRecordset::forwardOnly, strQuery);
			for(ii=0;ii<6;ii++)	{
				rs.GetFieldValue((short)ii,strData[ii]);
			}
			rs.Close();
		}
		catch(CException* e)	{
			e->Delete();
			return(-1);
		}
		// レコードを削除し、再作成
		pZms->m_database->ExecuteSQL(_T("DELETE FROM uc_inf_sub_omit WHERE FormSeq = 16 and ItemSeq = 4"));

		strQuery = _T("INSERT INTO uc_inf_sub_omit(FormSeq,ItemSeq,ItemName,Omit,OmitStr,OmitMaxLen) ");
		strQuery.Format(strQuery + _T("VALUES( %d, %d, '%s', %d, '%s', %d )"),_tstoi(strData[0]),_tstoi(strData[1]),strData[2],_tstoi(strData[3]),strData[4],_tstoi(strData[5]));
		pZms->m_database->ExecuteSQL(strQuery);
	}

	// -------------------------------------------------------------------------------------------
	// 科目行を出力するの有効サイン(uc_inf_sub.GeneralConstVal[15] & 0x02)がOFFであればONにする
	// -------------------------------------------------------------------------------------------
	for(ii=0; ii<2; ii++)	{
		// ②受取手形の内訳書
		if(ii == 0)	id = ID_FORMNO_021;
		// ⑪借入金及び支払利子の内訳書
		else		id = ID_FORMNO_111;
		if(mfcRecSub.RequeryFormSeq(id) == DB_ERR_OK)	{
			if(!(mfcRecSub.IsEOF()))	{
				if(!(mfcRecSub.m_GeneralConstVal[15] & BIT_D1))	{
					mfcRecSub.Edit();
					mfcRecSub.m_GeneralConstVal[15] |= BIT_D1;
					mfcRecSub.Update();
				}
			}
			mfcRecSub.Fin();
		}
	}

	return(0);
}

int VersionUp( CDBZmSub* pZms, CString pSqlServer, int pApNo, int pCoCode, int pKikan, int cosel )
{
	int				rv=0;
	char			szSQLServer[30]={0};	// SQLサーバ名
	char			szApno[20]={0};			// アプリケーション番号
	char			szCoCode[20]={0};		// 会社コード
	char			szKikan[20]={0};		// 期末年月日
	char			szDevice[4]={0};		// ドライブ文字
	char			szGrNum[8]={0};			// GR番号
	char			cCtrl=0;				// SHIFTキー状態
	CString			tmpKikan=_T("");		// CString work
	CString			m_DatabaseName=_T("");	// データベース名(Z??_C????????_Y????)
	CString			m_strPath=_T("");		// Sysフォルダのパス
	ZMGEN_CHKINFO	chkinfo={0};
	CZmGen8			zg;
	CString			wmsg=_T("");

	// -------------------------------------------------------------------
	// パラメータからローカル変数にセット
	// -------------------------------------------------------------------
	// ① SQLサーバ名
	memset(szSQLServer,0,30);
	sprintf_s(szSQLServer,30,_T("%s"),pSqlServer);
	// ② アプリケーション番号
	memset(szApno,0,20);
	sprintf_s(szApno,20,_T("%d"),pApNo);
	// ③ 会社コード
	memset(szCoCode,0,20);
	sprintf_s(szCoCode,20,_T("%08d"),pCoCode);
	// ④ 期末年月日
	memset(szKikan,0,20);
	sprintf_s(szKikan,20,_T("%08d"),pKikan);

	// -------------------------------------------------------------------
	// データベース名を作成する
	// -------------------------------------------------------------------
	tmpKikan.Format(_T("%s"),szKikan);
	m_DatabaseName.Format("Z%02x_C%08d_Y%04d",
				atoi(szApno),atoi(szCoCode),atoi(tmpKikan.Mid(2,4)));

	// Sysフォルダのパス名を作成する
	m_strPath.Empty();
	ZeroMemory(szDevice,sizeof(szDevice));
	ZeroMemory(szGrNum,sizeof(szGrNum));
	GCOM_GetString("SystemDrive",szDevice);
	PCOM_GetString("ExecGrJobs",szGrNum);
	m_strPath.Format("%s:\\ICSWin\\SYS\\GR%s\\",szDevice,szGrNum);

	//*****************************************************************
	// マスター移動業務のフィルタが内訳書テーブルは"uc_"で始まっている
	// としているので、今後テーブルを追加する際も、テーブル名は
	// "uc_○○"といった具合で追加すること
	//*****************************************************************
	///////////////////////////////////////////////////////////////////
	// 内訳書テーブルバージョンチェック
	//	(1) テーブルなし
	//			→ テーブルを作成、初期データ登録
	//	(2) アプリで扱えるバージョンより、テーブルバージョンが古い
	//			→ テーブルを更新する
	//	(3) アプリで扱えるバージョンより、テーブルバージョンが新しい
	//			→ アプリを更新する
	//	(4) アプリで扱えるバージョンと、テーブルバージョンが同じ
	//			→ OK!
	{
		CdbIcsVtbl	vtbl(pZms->m_database);
		int			version = DB_UC_TABLE_VERSION;
		int			retVer;

		retVer = vtbl.VersionCheck(DB_UC_VTBL_TYPE, DB_UC_VTBL_OBJNAME);

		if (retVer < version) {
			//	(2) アプリで扱えるバージョンより、テーブルバージョンが古い

// midori UC_0032 del -->
			//// テーブル作成、データ登録を実行する
			//int nRet = CheckAndMakeTable(szSQLServer, m_DatabaseName, (m_strPath + DB_STR_SQLFILE_UPDATE), DB_STR_SQLUSE_UPDATE, SQL_EXEC_UPDPATE, cosel);

			//if (nRet == -1) {
			//	if(cosel != 2)	{
			//		ICSMessageBox( _T("勘定科目内訳書マスター更新に失敗しました。") );
			//	}
			//	return(-1);
			//}
			//else if(nRet == -2)	{
			//	return(-2);
			//}
// midori UC_0032 del <--
// midori UC_0032 add -->
			CFileFind	FileFind;
			int			rv1=0,rv2=0;
			int			nRet=0;
			int			mver=0;

			rv1 = FileFind.FindFile(m_strPath + DB_STR_SQLFILE_UPDATE1);
			rv2 = FileFind.FindFile(m_strPath + DB_STR_SQLFILE_UPDATE2);

#if _MSC_VER < 1900
			mver = 0;
#else
			mver = 1;
#endif

			if(rv1 == 0 || rv2 == 0 || mver == 0)	{
				nRet = CheckAndMakeTable(szSQLServer,m_DatabaseName,(m_strPath + DB_STR_SQLFILE_UPDATE),DB_STR_SQLUSE_UPDATE,SQL_EXEC_UPDPATE,cosel);
			}
			else	{
				// テーブル作成、データ登録を実行する
				nRet = CheckAndMakeTable(szSQLServer,m_DatabaseName,(m_strPath + DB_STR_SQLFILE_UPDATE1),DB_STR_SQLUSE_UPDATE,SQL_EXEC_UPDPATE,cosel);
				if(nRet == 0)	{
					nRet = CheckAndMakeTable(szSQLServer,m_DatabaseName,(m_strPath + DB_STR_SQLFILE_UPDATE2),DB_STR_SQLUSE_UPDATE,SQL_EXEC_UPDPATE,cosel);
				}
			}
			if(nRet == -1)	{
				if(cosel != 2)	{
					ICSMessageBox(_T("勘定科目内訳書マスター更新に失敗しました。"));
				}
				return(-1);
			}
			else if(nRet == -2)	{
				return(-2);
			}
// midori UC_0032 add <--
		}
	}

	if(CheckTable(pZms) == -1)	{
		return(-1);
	}

	return(0);
}
// midori UC_0010 add <--

// midori 180503 add -->
// -----------------------------------------------------------------------------------------------------------------------------
// パラメータの頁番号(inPage)、行番号(inRow)以降の
// 頁番号と行番号を加減算する。
//
//		IN		inPage		頁番号
//				inRow		行番号
//				inAdd		加減数
//
//		戻り値	 0:成功
//				-1:失敗
// -----------------------------------------------------------------------------------------------------------------------------
int CmnTblUpdatePageNumRowNumAdd(CDBZmSub* pZmSub, int pintFormType,int pintOutKei, int nFormSeq, int inPage, int inRow, int inAdd)
{
	CdbUc000Common*		rsData;
	int					nBaseRow;
	int					retVal = FUNCTION_NG;

	retVal = FUNCTION_OK;

	// 様式テーブル取得
	rsData = GetDataTableObject(pZmSub,nFormSeq);

	// 行番号を振り直すための基準となる行数を取得
	nBaseRow = CmnTblCountMaxRow(nFormSeq,pintFormType,pintOutKei);

	// 頁番、行番振り直し
	rsData->UpdatePageNumRowNumAdd(inPage, inRow, inAdd, nBaseRow);

	rsData->Fin();
	delete rsData;

	return retVal;
}

// -----------------------------------------------------------------------------------------------------------------------------
// uc_inf_main に GeneralVar が存在するかのチェック
// -----------------------------------------------------------------------------------------------------------------------------
int	CheckGeneralVar( CDBZmSub* pZmSub )
{
	int				ret=0;
	short			ind=0;
	CString			strSQL=_T("");
	CString			cs=_T("");
	CRecordset*		prs;

	strSQL = _T("SELECT COLUMNPROPERTY(OBJECT_ID('uc_inf_main'),'GeneralVar','PRECISION')");
	prs = new CRecordset(pZmSub->m_database);
	prs->Open(CRecordset::forwardOnly,strSQL);
	prs->GetFieldValue(ind,cs);
	ret = _tstoi(cs);
	prs->Close();
	delete prs;
	prs=0;

	return(ret);
}

// -----------------------------------------------------------------------------------------------------------------------------
// 入力確定更新チェック
//
// 【戻り値】	0:入力確定の更新不要 1:入力確定の更新必要
// -----------------------------------------------------------------------------------------------------------------------------
int	ChkKakuteiUpd( CDBZmSub* pZmSub )
{
	int				ret=0;				// 戻り値
	CdbUcInfMain*	pfcRec;				// uc_inf_mainテーブルクラス

	// 一度でも過年度財務マスターから確定サインを転記しているかのフラグを取得
	pfcRec = new CdbUcInfMain(pZmSub->m_database);
	if(pfcRec->Init() == DB_ERR_OK)	{
		if(!(pfcRec->IsEOF()))	{
			pfcRec->MoveFirst();
			// 入力確定更新サインを取得	0:更新する	1:更新しない
			if(pfcRec->m_GeneralVar[2] & BIT_D0)	ret = 1;
		}
	}
	pfcRec->Fin();
	delete pfcRec;

	return(ret);
}

// -----------------------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------
// midori 181101 del -->
//void UchiCnvDbToBCD( long pYmd, UCHAR* pYear, UCHAR* pMon, UCHAR* pDay )
// midori 181101 del <--
// midori 181101 add -->
void UchiCnvDbToBCD( long pYmd, UCHAR* pMon, UCHAR* pDay )
// midori 181101 add <--
{
// midori 181101 del -->
//	int					yy;
// midori 181101 del <--
	int					mm;
	int					dd;
	CVolDateDB			vd;

	//	年
// midori 181101 del -->
//	vd.db_datecnv(( ( pYmd / 10000 ) % 100 ),&yy,0,2);
//	*pYear	= yy ;
// midori 181101 del <--
	//	月
	mm	= ( pYmd % 10000 ) / 100 ;
	int_bcd(pMon,mm,1);
	//	日
	dd	= ( pYmd % 10000 ) % 100 ;
	int_bcd(pDay,dd,1);
}


// -----------------------------------------------------------------------------------------------------------------------------
//	翌期更新：財務マスターの決算期間を取得
//		IN		CDBZmSub*		財務クラス
//				int*			決算期間（期首）
//				int*			決算期間（期末）
//		RET		int				FUNCTION_OK:正常終了
//								FUNCTION_NG:エラー
// -----------------------------------------------------------------------------------------------------------------------------
int UchiYokukiGetZvolumeYmd(CDBZmSub* pZmSub,long* lngSSymd, long* lngEEymd)
{

	// クリア
	*lngSSymd = 0;
	*lngEEymd = 0;

// midori 181002 del -->
	//// zvolumeテーブル参照
	//if ( pZmSub->VolumeOpen() == 0 ){
	//	
	//	*lngSSymd = pZmSub->zvol->ss_ymd;		// 決算期間(期首)
	//	*lngEEymd = pZmSub->zvol->ee_ymd;		// 決算期間(期末)

	//	// テーブルを閉じる
	//	pZmSub->VolumeClose();
	//}
	//else{
	//	return FUNCTION_NG;
	//}
// midori 181002 del <--
// midori 181002 add -->
	if(pZmSub->zvol == NULL) {
		// zvolumeテーブル参照
		if ( pZmSub->VolumeOpen() == 0 ){
			
			*lngSSymd = pZmSub->zvol->ss_ymd;		// 決算期間(期首)
			*lngEEymd = pZmSub->zvol->ee_ymd;		// 決算期間(期末)

			// テーブルを閉じる
			pZmSub->VolumeClose();
		}
		else{
			return FUNCTION_NG;
		}
	}
	else {
		*lngSSymd = pZmSub->zvol->ss_ymd;		// 決算期間(期首)
		*lngEEymd = pZmSub->zvol->ee_ymd;		// 決算期間(期末)
	}
// midori 181002 add <--

	return FUNCTION_OK;
}

// -----------------------------------------------------------------------------------------------------------------------------
//	一括金額/一時保管参照のデータを戻す処理
//		IN		EnumIdFgShow		ID_FGSHOW_IKKATU or ID_FGSHOW_HOKAN
//				int					行番号
//				int					列番号
//				int					グループ番号
//		RET		なし
// -----------------------------------------------------------------------------------------------------------------------------
//int CmnTblUpdateFgShowDataReturn(EnumIdFgShow nFgShow, int nPage, int nRow, int nGroup)
int CmnTblUpdateFgShowDataReturn(CDBZmSub* pZmSub, CdbUcInfSub* pUcInfSub, int nPage, int nRow, int nGroup)
{
	int			intFgShowTemp = 0;
	int			nBaseRow;
	int			intOutKei=0;
	int			formSeq=0;
	int			formtype=0;

	formSeq = pUcInfSub->m_FormSeq;
	intOutKei = pUcInfSub->m_OutKei;

	formtype = ID_FORMTYPE_KOBETU;
	CmnUcInfFormTypeGetFormType(pZmSub,formSeq,&formtype);

	// 行番号を振り直すための基準となる行数を取得
	nBaseRow = CmnTblCountMaxRow(formSeq,formtype,intOutKei);

	// パラメータのチェック
	if( nPage <= 0 || nRow <= 0 ){
		return FUNCTION_NG;
	}
//	switch( nFgShow ){
//	case ID_FGSHOW_IKKATU:	// 一括金額参照
//		intFgShowTemp = ID_FGSHOW_RET_IKKATU;
//		break;
//	case ID_FGSHOW_HOKAN:	// 一時保管参照
//		intFgShowTemp = ID_FGSHOW_RET_HOKAN;
//		break;
//	default:
//		return FUNCTION_NG;
//		break;
//	}
	intFgShowTemp = ID_FGSHOW_RET_IKKATU;

	CdbUc000Common*		rsData;
//	rsData = virGetDataTableObject();
	// 様式テーブル取得
	rsData = GetDataTableObject(pZmSub,formSeq);

	// 対象行数の取得
	int nNumRow = rsData->GetCountVal(intFgShowTemp, nGroup);

	// 対象レコードが、1つもないときは処理終了
	if( nNumRow <= 0 ){
		rsData->Fin();
		delete rsData;
		return(1);
	}

	// 頁番号行番号構築
	CmnTblUpdatePageNumRowNumAdd(pZmSub, formtype, intOutKei, formSeq, nPage, nRow, nNumRow );

	// 指定レコードを抽出
	if( rsData->RequeryFgShow( intFgShowTemp ) != DB_ERR_OK ){
		rsData->Fin();
		delete rsData;
		return FUNCTION_NG;
	}

//	if( nFgShow == ID_FGSHOW_HOKAN ){		// 一時保管
//
//		// 様式テーブルの実体に頁番号、行番号を設定
////		nRow--;	// ROW_NUMBER()が1で始まるため
////		rsData->SetPageRowNumToReturnData( nPage, nRow, nBaseRow );
//		SetPageRowNumToReturnDataByOrder( rsData, nPage, nRow, nBaseRow );
//
//		// 様式レコードをトップへ戻す
//		rsData->MoveFirst();
//
//		// 保管テーブルの整合性をとる
//		DoProcAfterReturnHokanTable( rsData );
//	}
//	else{	// 一括金額
		// 頁番号、行番号を設定
		rsData->MoveFirst();
		while( !rsData->IsEOF() ){	
			rsData->Edit();
			rsData->m_NumPage = nPage;			// 頁番号
			rsData->m_NumRow = nRow++;			// 行番号
			rsData->m_FgShow = ID_FGSHOW_OFF;	// 表示
			rsData->m_NumGroup = 0;				// グループ番号
			rsData->Update();
			rsData->MoveNext();	// 次のレコードへ

			// 次の行が。最終行＋1であれば、次頁へ
			if( nRow >= nBaseRow + 1 ){
				nPage++;
				nRow = 1;
			}
		}
//	}
	
	rsData->Fin();
	delete rsData;

	return FUNCTION_OK;
}

//********************************************************************************
//	自動一括集計金額行の追加/更新処理
//		IN		int				追加/更新区分（0:追加，1:更新，2:指定頁/行に追加）
//				int				追加対象のグループ番号
//				int*			頁番号				（追加時のみ使用）
//				int*			行番号				（追加時のみ使用）
//				SORTKAMOKU_INFO	ソート時の科目情報	（追加時のみ使用）
//		RET		int				実行結果（FUNCTION_OK, FUNCTION_NG）
//********************************************************************************
//int CfrmUc000Common::CmnTblIkkatuAutoUpdateProc(int nType, int nGroup, int* nPage, int* nRow, SORTKAMOKU_INFO uKamoku)
int CmnTblIkkatuAutoUpdateProc(CDBZmSub* pZmSub, CdbUcInfSub* pUcInfSub,int nGroup, int* nPage, int* nRow, SORTKAMOKU_INFO uKamoku)
{
	CString			strName;			// 名称
//	CString			strSum;				// 集計金額
	CALCKEI_INFO	uCalcKei;			// 集計金額
	int				nCount;				// 集計件数
	int				nTempPage = 0;
	int				nTempRow = 0;
	int				intOutKei=0;
	int				formSeq=0;
	int				formtype=0;

	formSeq = pUcInfSub->m_FormSeq;
	intOutKei = pUcInfSub->m_OutKei;

	formtype = ID_FORMTYPE_KOBETU;
	CmnUcInfFormTypeGetFormType(pZmSub,formSeq,&formtype);

	// 帳表毎のテーブルクラスを"New"する
	CdbUc000Common*		rsData;
//	rsData = virGetDataTableObject();
	rsData = GetDataTableObject(pZmSub,formSeq);

// 金額の集計は不要
//	// 金額と件数を集計
//	virTblGetIkkatuSumValue(rsData, ID_FGSHOW_IKKATU, nGroup, &uCalcKei);
	nCount = rsData->GetCountVal(ID_FGSHOW_IKKATU, nGroup);

//	// 一括金額レコードの名称
//	strName.Format("%s %d%s", NAME_IKKATU_1, nCount, NAME_IKKATU_2);
//
//	if ((nType == 0) || (nType == 2)) {
//		// 自動一括集計金額行の追加：１件以上あれば追加
//		if (nCount > 0) {
//			// 追加行の頁と行を取得
//			if (nType == 0) {
//				CmnGetNextPageRow(nPage, nRow);
//			}
//
//			// 新規レコード追加
//			rsData->CreateNewRecord();					// レコード初期化済み
//
//			// 特殊行データへ更新
//			rsData->Edit();
//			rsData->m_NumPage = *nPage;					// 頁番号
//			rsData->m_NumRow = *nRow;					// 行番号
//			rsData->m_FgFunc = ID_FGFUNC_IKKATUAUTO;	// 特殊行フラグ
//			rsData->m_FgShow = ID_FGSHOW_OFF;			// 表示フラグ
//			rsData->m_NumGroup = nGroup;				// グループ番号
//
//			virTblEditIkkatuLine(0, rsData, strName, uCalcKei, uKamoku);		// 帳表個別データ設定
//			rsData->Update();
//		}
//	}
//	else {
		// 自動一括集計金額行の更新：レコードがあれば更新（対象０件なら削除が必要）
		rsData->RequeryFgFuncGroup(ID_FGFUNC_IKKATUAUTO, nGroup);
		if (rsData->IsEOF() == 0) {
			if (nCount <= 0) {
				// 削除前に削除レコードの頁と行をバックアップ
				nTempPage = rsData->m_NumPage;
				nTempRow = rsData->m_NumRow;

				// 対象データが０件なので自動一括集計金額データの削除
				rsData->Delete();

				// データ削除した場合、頁番号/行番号振り直しが必要
				CmnTblUpdatePageNumRowNumAdd(pZmSub, formtype, intOutKei, formSeq, nTempPage, nTempRow, -1);
			}
//			else {
//				// 自動一括集計金額データの更新
//				rsData->Edit();
//				virTblEditIkkatuLine(1, rsData, strName, uCalcKei, uKamoku);	// 帳表個別データ設定
//				rsData->Update();
//			}
		}
//	}
	rsData->Fin();
	delete rsData;

	return	FUNCTION_OK;
}

// -----------------------------------------------------------------------------------------------------------------------------
// 過年度マスターが選択されているかのチェック
// -----------------------------------------------------------------------------------------------------------------------------
BOOL IsKanendoMaster( CDBZmSub* pZmSub )
{
	BOOL			brv=FALSE;
	char			szSQLServer[64];	// SQLサーバ名
	char			szApno[16];			// アプリケーション番号
	char			szCoCode[16];		// 会社コード
	char			szKikan[16];		// 期末年月日
	int				eymd=0;				// 過年度の終了年月日
	int				nen=0;
	int				kikan=0;
// midori 181101 del -->
//	UCHAR			yy,mm,dd;
// midori 181101 del <--
// midori 181101 add -->
	UCHAR			mm,dd;
// midori 181101 add <--
	UCHAR			ymd1[3]={0},ymd2[3]={0};
	BYTE			apno=0;
	CString			m_svr=_T("");
	CO_CTL_DB		coctl={0};
	CO_CTL_Array	coarray;
	ICSMCTL			m_mctl;

	// 選択した決算期間が、coarrayの[0]番目とより古ければ過年度マスター
	pZmSub->VolumeOpen();
	apno = pZmSub->zvol->apno;
	eymd = pZmSub->zvol->ee_ymd;
	pZmSub->VolumeClose();

	kikan = eymd;
	nen = (kikan / 10000);
	nen %= 100;
// midori 181101 del -->
//	UchiCnvDbToBCD(kikan,&yy,&mm,&dd);
// midori 181101 del <--
// midori 181101 add -->
	UchiCnvDbToBCD(kikan,&mm,&dd);
// midori 181101 add <--
	int_bcd(&ymd1[0],nen,1);
	ymd1[1] = mm;
	ymd1[2] = dd;

	PCOM_GetString(_T("SelectSqlServer"),m_svr.GetBuffer(256));
	m_svr.ReleaseBuffer();
	if(m_mctl.SetDatabase(m_svr) < 0)	{
		ICSExit(0,_T("マスター管理テーブルの読み込みに失敗しました。"));
		return(FUNCTION_NG);
	}
	if(m_mctl.vctl_open(MODE_READ,apno,0) == 0)	{
		PCOM_GetString("SELECTCODSN",szSQLServer,szApno,szCoCode,szKikan);
		coctl.c_apno = apno;
		int_bcd(coctl.c_cod,atoi(szCoCode),4);
		if(m_mctl.vctl_coinf(ICSMCTL::ZAIMU_SW,&coctl,&coarray) == 0)	{
			if((int)coarray.GetCount() > 1)	{
				memcpy(ymd2,&coarray[0].c_kikan[3],3);
				if(memcmp(ymd1,ymd2,3) < 0)	{
					brv = TRUE;
				}
			}
		}
		m_mctl.vctl_close();
	}

	return(brv);
}

// -----------------------------------------------------------------------------------------------------------------------------
// 過年度財務マスターの「決算確定」を、内訳書の「入力データを確定する」にセットする
// -----------------------------------------------------------------------------------------------------------------------------
int	KakuteiSet( CDBZmSub* pZmSub )
{
	char			szSQLServer[64];	// SQLサーバ名
	char			szApno[16];			// アプリケーション番号
	char			szCoCode[16];		// 会社コード
	char			szKikan[16];		// 期末年月日
	char			tmp1[16],tmp2[16],tmp3[16];
	int				kmst=0;
	int				max=0;
	int				nen=0;
	int				n_kikan=0,z_kikan=0;
	int				eymd=0;
	int				rv=0;
	int				ksw=0;
// midori 181101 del -->
//	UCHAR			yy,mm,dd;
// midori 181101 del <--
// midori 181101 add -->
	UCHAR			mm,dd;
// midori 181101 add <--
	UCHAR			ymd[3]={0},zymd[3]={0};
	BYTE			apno=0;
	CString			m_svr=_T("");
	CString			msg=_T("");
	ICSMCTL			m_mctl;
	CO_CTL_DB		coctl={0};
	CO_CTL_Array	coarray;
	CDBZmSub		*kzms;
	CZmGen8			zg;
	ZMGEN_CHKINFO	zc={0};

	pZmSub->VolumeOpen();
	apno = pZmSub->zvol->apno;
	eymd = pZmSub->zvol->ee_ymd1;
	pZmSub->VolumeClose();

	PCOM_GetString(_T("SelectSqlServer"),m_svr.GetBuffer(256));
	m_svr.ReleaseBuffer();
	if(m_mctl.SetDatabase(m_svr) < 0)	{
		ICSExit(0,_T("マスター管理テーブルの読み込みに失敗しました。"));
		return(FUNCTION_NG);
	}

	if(m_mctl.vctl_open(MODE_READ,apno,0) == 0)	{
		PCOM_GetString("SELECTCODSN",szSQLServer,szApno,szCoCode,szKikan);
		coctl.c_apno = apno;
		int_bcd(coctl.c_cod,atoi(szCoCode),4);
		if(m_mctl.vctl_coinf(ICSMCTL::ZAIMU_SW,&coctl,&coarray) == 0)	{
			n_kikan = atoi(szKikan);
			nen = (n_kikan / 10000);
			nen %= 100;
// midori 181101 del -->
//			UchiCnvDbToBCD(n_kikan,&yy,&mm,&dd);
// midori 181101 del <--
// midori 181101 add -->
			UchiCnvDbToBCD(n_kikan,&mm,&dd);
// midori 181101 add <--

			int_bcd(&ymd[0],nen,1);
			ymd[1] = mm;
			ymd[2] = dd;

			max = (int)coarray.GetCount();
			if(max > 1)	{
				if(memcmp(ymd,&coarray[0].c_kikan[3],sizeof(ymd)) == 0)	{
					memset(tmp1,0,16);
					memset(tmp2,0,16);
					memset(tmp3,0,16);
					sprintf_s(tmp1,16,"%08d",eymd);
					memcpy(tmp2,&tmp1[2],6);
					l_pack(tmp3,tmp2,6);
					if(memcmp(tmp3,&coarray[1].c_kikan[3],3) == 0)	{
						kmst = 1;
						memmove(zymd,&coarray[1].c_kikan[3],sizeof(zymd));
						z_kikan = bcd_int(zymd,3);
					}
				}
			}
		}
		m_mctl.vctl_close();
	}
	if(kmst)	{
		kzms = new CDBZmSub;
		kzms->ExitMode(1);
		if(kzms->SetMasterDB(DB_ICS_VERSION,szSQLServer,atoi(szApno),atoi(szCoCode),z_kikan) == -1)	{
			ICSMessageBox(kzms->GetZmErrMsg(),MB_ICONSTOP);
			return(FUNCTION_NG);
		}
// midori UC_0051 add -->
		zc.verupsub = 0x800;
// midori UC_0051 add <--
		rv = zg.CheckDBMasterState(kzms,msg,zc);
		if(rv == -1 || rv == -2)	{
			ICSMessageBox(zg.GetErrorMsg(),MB_ICONSTOP);
			return(FUNCTION_NG);
		}
		else if(rv == 1)	{
			ICSMessageBox(msg);
		}
		// 過年度の確定サインを更新する
		kzms->VolumeOpen();
		if(kzms->zvol->tl_cor & 0x10)	ksw=0;	// 財務決算確定ＯＦＦ
		else							ksw=1;	// 　　 〃 　　ＯＮ
		kzms->VolumeClose();

		if(CheckGeneralVar(kzms) != 0)	{
			CdbUcInfMain*	prsMain;
			prsMain = new CdbUcInfMain(kzms->m_database);
			if(prsMain->Init() == DB_ERR_OK){
				if((prsMain->m_GeneralVar[1] & BIT_D3) == 0)	{
					prsMain->Edit();
					if((prsMain->m_GeneralVar[1] & BIT_D2) != BIT_D2) {
						if(ksw)	prsMain->m_GeneralVar[1] |= BIT_D2;
					}
					prsMain->m_GeneralVar[1] |= BIT_D3;
					prsMain->Update();
				}
			}
			prsMain->Fin();
			delete prsMain;
		}

		delete kzms;

	}
	// 当期の確定サインを更新する
	CdbUcInfMain	rsMain2(pZmSub->m_database);
	if(rsMain2.Init() == DB_ERR_OK){
		rsMain2.Edit();
		CCtrlMainGeneralVar cm(rsMain2);
		cm.bDataKakutei = 0;
		cm.Set(rsMain2);
		rsMain2.m_GeneralVar[1] &= ~BIT_D3;
		rsMain2.m_GeneralVar[2] |= BIT_D0;
		rsMain2.Update();
	}
	rsMain2.Fin();

	return(FUNCTION_OK);
}

// -----------------------------------------------------------------------------------------------------------------------------
//	既存の保管データを保管テーブルにコピーする
//
//		【引数】	int			開始頁番号
//					int			開始行番号
//					int			頁を跨ぐ行数
//		【戻値】	BOOL 
// -----------------------------------------------------------------------------------------------------------------------------
int CopyHokanDataToHokanTable(CDBZmSub* pZmSub, int nFormSeq)
{
	int nOrder = 1;
	CdbUc000Common* rsData;
	CdbUcLstHokan	mfcRec( pZmSub->m_database );
	
	// virが使用できないためGetDataTableObjectで取得する
	// 様式テーブル取得
	rsData = GetDataTableObject(pZmSub,nFormSeq);

	// 保管レコードを抽出
// midori 180537 del -->
//	if( rsData->RequeryFgShow( ID_FGSHOW_HOKAN ) != DB_ERR_OK ){
// midori 180537 del <--
// midori 180537 add -->
	if( rsData->RequeryFgShow( ID_FGSHOW_HOKAN,1 ) != DB_ERR_OK ){
// midori 180537 add <--
		rsData->Fin();
		delete rsData;
		return FUNCTION_NG;
	}

	while( !rsData->IsEOF() ){
		// 保管レコード新規追加
		mfcRec.CreateNewRecord();

		mfcRec.Edit();
		mfcRec.m_FormSeq	= nFormSeq;
		mfcRec.m_Seq		= rsData->m_Seq;
		mfcRec.m_OrderNum	= nOrder;
		mfcRec.Update();

		// オーダーインクリメント
		nOrder++;

		// 次のレコードへ
		rsData->MoveNext();
	}

	rsData->Fin();
	delete rsData;

	return FUNCTION_OK;
}

// -----------------------------------------------------------------------------------------------------------------------------
// 処理１
// マスターによっては財務の翌期更新直後に過年度の確定サインが更新されない場合があるので、そのための修正
// -----------------------------------------------------------------------------------------------------------------------------
// 　入力データを確定する				uc_inf_main.GeneralVar[1] & 0x04
// 　過年度確定サインの更新フラグ		uc_inf_main.GeneralVar[1] & 0x08
// -----------------------------------------------------------------------------------------------------------------------------
// 　①一度翌期更新したマスターについて「財務マスターコピー」で過年度を当年度としてコピーする。
//					GeneralVar[1] & 0x04	GeneralVar[1] & 0x08	財務の確定フラグ
//		(過年度)			OFF						ON					OFF
//		(当年度)			OFF						OFF					OFF
//			… 過年度を当年度としてコピーする
//		(当年度)			OFF						ON	※				OFF
// 　②過年度を当年度としてコピーしたマスターを翌期更新(財務)する。
//		(過年度)			OFF						ON	※				ON
//		(当年度)			OFF						ON	※				OFF
// 
// 　※通常、当年度の (GeneralVar[1] & 0x08) がONになっていることはありえないが、過年度を当年度としてコピーするとONになります。
// 　　当年度の (GeneralVar[1] & 0x08) がONになっている場合は、当年度・過年度ともにOFFにする。
// 　　(過年度マスターを起動した場合も同じことをする)
// -----------------------------------------------------------------------------------------------------------------------------
// 処理２
// 当年度内訳書マスターが更新されていない状態で初めて過年度マスターを起動した場合、過年度財務マスターの確定サインを
// 過年度内訳書マスターの確定サインにセットする
// -----------------------------------------------------------------------------------------------------------------------------
// 　①過年度マスターを選択している
// 　②当年度内訳書マスターが更新されていない
// 　③当年度内訳書マスターが更新されていない状態で初めて過年度マスターを起動した
// 　→上記の場合、過年度財務マスターの確定サインを、過年度内訳書マスターの
// 　　確定サインにセットする
// -----------------------------------------------------------------------------------------------------------------------------
int	KanendoKakuteiSet( CDBZmSub* pZmSub )
{
	char			szSQLServer[64];	// SQLサーバ名
	char			szApno[16];			// アプリケーション番号
	char			szCoCode[16];		// 会社コード
	char			szKikan[16];		// 期末年月日
	int				eymd=0;				// 過年度の終了年月日
	int				eymd2=0;			// 当年度からみた、１期前の終了年月日
	int				kmst=0;
	int				max=0;
	int				nen=0;
	int				z_kikan=0,t_kikan=0;
	int				rv=0;
	int				zsymd=0,zeymd=0;	// 当期財務の決算期間
	int				zksgn=0;
	int				usymd=0,ueymd=0;	// 当期内訳書の決算期間
	int				uksgn=0;
	int				sw=0;
// midori 181101 del -->
//	UCHAR			yy,mm,dd;
// midori 181101 del <--
// midori 181101 add -->
	UCHAR			mm,dd;
// midori 181101 add <--
	UCHAR			ymd1[3]={0},ymd2[3]={0};
	UCHAR			tymd[3]={0};
	BYTE			apno=0;
	CString			m_svr=_T("");
	CString			msg=_T("");
	CO_CTL_DB		coctl={0};
	CO_CTL_Array	coarray;
	ICSMCTL			m_mctl;
	CDBZmSub		*tzms;
	CZmGen8			zg;
	ZMGEN_CHKINFO	zc={0};
	CdbUcInfMain*	pfcRec;				// uc_inf_mainテーブルクラス

	// ①coarrayの[1]番目と決算期間が同一なら過年度マスター
	pZmSub->VolumeOpen();
	apno = pZmSub->zvol->apno;
	eymd = pZmSub->zvol->ee_ymd;
	if(pZmSub->zvol->tl_cor & 0x10)	zksgn=0;	// 財務決算確定ＯＦＦ
	else								zksgn=1;	// 　　 〃 　　ＯＮ
	pZmSub->VolumeClose();

	PCOM_GetString(_T("SelectSqlServer"),m_svr.GetBuffer(256));
	m_svr.ReleaseBuffer();
	if(m_mctl.SetDatabase(m_svr) < 0)	{
		ICSExit(0,_T("マスター管理テーブルの読み込みに失敗しました。"));
		return(FUNCTION_NG);
	}
	if(m_mctl.vctl_open(MODE_READ,apno,0) == 0)	{
		PCOM_GetString("SELECTCODSN",szSQLServer,szApno,szCoCode,szKikan);
		coctl.c_apno = apno;
		int_bcd(coctl.c_cod,atoi(szCoCode),4);
		if(m_mctl.vctl_coinf(ICSMCTL::ZAIMU_SW,&coctl,&coarray) == 0)	{
			z_kikan = eymd;
			nen = (z_kikan / 10000);
			nen %= 100;
// midori 181101 del -->
//			UchiCnvDbToBCD(z_kikan,&yy,&mm,&dd);
// midori 181101 del <--
// midori 181101 add -->
			UchiCnvDbToBCD(z_kikan,&mm,&dd);
// midori 181101 add <--
			int_bcd(&ymd1[0],nen,1);
			ymd1[1] = mm;
			ymd1[2] = dd;
			max = (int)coarray.GetCount();
			if(max > 1)	{
				memcpy(ymd2,&coarray[1].c_kikan[3],3);
				if(memcmp(ymd1,ymd2,3) == 0)	{
					kmst = 1;
					memmove(tymd,&coarray[0].c_kikan[3],3);
					t_kikan = bcd_int(tymd,3);
				}
			}
			// 当年度マスター起動時(kmst!=1)に過年度マスターを参照するため
			// 期間を取得しておく
			z_kikan = 0;
			if(max > 1)	{
				memcpy(ymd2,&coarray[0].c_kikan[3],3);
				if(memcmp(ymd1,ymd2,3) == 0)	{
					memmove(tymd,&coarray[1].c_kikan[3],3);
					z_kikan = bcd_int(tymd,3);
				}
			}
		}
		m_mctl.vctl_close();
	}

	// ②当年度マスターの財務ｄｂに接続する
	if(kmst)	{
		// 一度でも過年度財務マスターから確定サインを転記しているかのフラグを取得
		pfcRec = new CdbUcInfMain(pZmSub->m_database);
		if(pfcRec->Init() == DB_ERR_OK)	{
			if(!(pfcRec->IsEOF()))	{
				pfcRec->MoveFirst();
				if(pfcRec->m_GeneralVar[1] & 0x08)	uksgn = 1;
				else								uksgn = 0;
			}
		}
		pfcRec->Fin();
		delete pfcRec;
		// 当期財務ｄｂに接続
		tzms = new CDBZmSub;
		tzms->ExitMode(1);
		if(tzms->SetMasterDB(DB_ICS_VERSION,szSQLServer,atoi(szApno),atoi(szCoCode),t_kikan) == -1)	{
			ICSMessageBox(tzms->GetZmErrMsg(),MB_ICONSTOP);
			return(FUNCTION_NG);
		}
// midori UC_0051 add -->
		zc.verupsub = 0x800;
// midori UC_0051 add <--
		rv = zg.CheckDBMasterState(tzms,msg,zc);
		if(rv == -1 || rv == -2)	{
			ICSMessageBox(zg.GetErrorMsg(),MB_ICONSTOP);
			return(FUNCTION_NG);
		}
		else if(rv == 1)	{
			ICSMessageBox(msg);
		}

		// 処理１ >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		int			retVer=0;
		CdbIcsVtbl*	vtbl;

		vtbl = new CdbIcsVtbl(tzms->m_database);
		retVer = vtbl->VersionCheck(DB_UC_VTBL_TYPE,DB_UC_VTBL_OBJNAME);
		delete vtbl;
		if(retVer > 0)	{
			// 当年度財務の過年度確定サインの更新フラグ(uc_inf_main.GeneralVar[1] & 0x08)を参照する
			if(CheckGeneralVar(tzms) != 0)	{
				pfcRec = new CdbUcInfMain(tzms->m_database);
				if(pfcRec->Init() == DB_ERR_OK)	{
					if(!(pfcRec->IsEOF()))	{
						pfcRec->MoveFirst();
						// フラグがONになっていれば当年度・過年度ともにOFFにする
						if(pfcRec->m_GeneralVar[1] & BIT_D3)	{
							// -------------------------------------
							// 当年度
							// -------------------------------------
							pfcRec->Edit();
							pfcRec->m_GeneralVar[1] &= ~BIT_D3;
							pfcRec->Update();
							// -------------------------------------
							// 過年度
							// -------------------------------------
							uksgn = 0;
						}
					}
				}
				pfcRec->Fin();
				delete pfcRec;
			}
		}

		// 一度でも過年度財務マスターから確定サインを転記していれば以下の処理は行わない
		if(uksgn != 1)	{
			// 内訳書過年度確定サインの更新フラグを参照してフラグがＯＦＦであれば
			// 過年度財務の確定サインをセットする
			pfcRec = new CdbUcInfMain(pZmSub->m_database);
			if(pfcRec->Init() == DB_ERR_OK){
				pfcRec->Edit();
				if((pfcRec->m_GeneralVar[1] & BIT_D2) != BIT_D2)	{
					if(zksgn)	pfcRec->m_GeneralVar[1] |= BIT_D2;
					else		pfcRec->m_GeneralVar[1] &= ~BIT_D2;
				}
				pfcRec->m_GeneralVar[1] |= BIT_D3;
				pfcRec->Update();
			}
			pfcRec->Fin();
			delete pfcRec;
		}
		delete tzms;
		// 入力確定更新サインをONにする	0:更新する	1:更新しない
		pfcRec = new CdbUcInfMain(pZmSub->m_database);
		if(pfcRec->Init() == DB_ERR_OK){
			pfcRec->Edit();
			pfcRec->m_GeneralVar[2] |= BIT_D0;
			pfcRec->Update();
		}
		pfcRec->Fin();
		delete pfcRec;
	}
	else	{
		// 処理１ >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		// 当年度財務の過年度確定サインの更新フラグ(uc_inf_main.GeneralVar[1] & 0x08)を参照する
		uksgn = 0;
		pfcRec = new CdbUcInfMain(pZmSub->m_database);
		if(pfcRec->Init() == DB_ERR_OK)	{
			if(!(pfcRec->IsEOF()))	{
				pfcRec->MoveFirst();
				// フラグがONになっていればOFFにする
				if(pfcRec->m_GeneralVar[1] & BIT_D3)	{
					// -------------------------------------
					// 当年度
					// -------------------------------------
					pfcRec->Edit();
					pfcRec->m_GeneralVar[1] &= ~BIT_D3;
					pfcRec->Update();
					uksgn = 1;
				}
			}
		}
		pfcRec->Fin();
		delete pfcRec;

		// フラグがONになっていれば過年度のフラグもOFFにする
		if(uksgn != 0 && z_kikan != 0)	{
			// 過年度マスターに接続
			tzms = new CDBZmSub;
			tzms->ExitMode(1);
			if(tzms->SetMasterDB(DB_ICS_VERSION,szSQLServer,atoi(szApno),atoi(szCoCode),z_kikan) == -1)	{
				ICSMessageBox(tzms->GetZmErrMsg(),MB_ICONSTOP);
				return(FUNCTION_NG);
			}
// midori UC_0051 add -->
			zc.verupsub = 0x800;
// midori UC_0051 add <--
			rv = zg.CheckDBMasterState(tzms,msg,zc);
			if(rv == -1 || rv == -2)	{
				ICSMessageBox(zg.GetErrorMsg(),MB_ICONSTOP);
				return(FUNCTION_NG);
			}
			else if(rv == 1)	{
				ICSMessageBox(msg);
			}
			if(CheckGeneralVar(tzms) != 0)	{
				pfcRec = new CdbUcInfMain(tzms->m_database);
				if(pfcRec->Init() == DB_ERR_OK)	{
					if(!(pfcRec->IsEOF()))	{
						pfcRec->MoveFirst();
						pfcRec->Edit();
						pfcRec->m_GeneralVar[1] &= ~BIT_D3;
						pfcRec->Update();
					}
				}
				delete tzms;
			}
		}
		// 処理１ <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	}

	return(FUNCTION_OK);
}

// -----------------------------------------------------------------------------------------------------------------------------
//	翌期更新：更新処理シーケンス
//		IN		int				FormSeq番号
//		RET		int				FUNCTION_OK:正常終了
//								FUNCTION_NG:エラー
// -----------------------------------------------------------------------------------------------------------------------------
int UchiYokukiUpdateSubProc(CDBZmSub* pZmSub,int intFormSeq)
{
	int					rv=0;
	int					nGroup;
//	CfrmUc000Common*	pView = NULL;			// FormViewオブジェクト
	CdbUc000Common*		pData1 = NULL;
	CdbUc000Common*		pData2 = NULL;
	CdbUcInfYokuki		rsYokuki(pZmSub->m_database);		// uc_inf_yokuki
	SORTKAMOKU_INFO		uKamoku;
	CUcFunctionCommon	pUcFuncCmn;
// 21-0292 add -->
	int					chk=0;
	CdbUcInfMain*		prsMain = new CdbUcInfMain(pZmSub->m_database);
	if(prsMain->Init() == DB_ERR_OK)	{
		chk = (prsMain->m_GeneralVar[4] & BIT_D2);
// 211022 add -->
		CCtrlMainGeneralVar	cm(*prsMain);
		if(cm.bUseStyle)	bG_Kanso = TRUE;
		else				bG_Kanso = FALSE;
// 211022 add <--
	}
	prsMain->Fin();
	delete prsMain;
// 21-0292 add <--

	//	帳票ごとの情報テーブル(uc_inf_sub)の準備
	CdbUcInfSub* pUcInfSub;
	pUcInfSub = new CdbUcInfSub(pZmSub->m_database);
// midori UC_0022 del -->
//	if ( pUcInfSub->Init(intFormSeq) != DB_ERR_OK ) {
// midori UC_0022 del <--
// midori UC_0022 add -->
	if ( pUcInfSub->RequeryFormSeq(intFormSeq) != DB_ERR_OK ) {
// midori UC_0022 add <--
		return FUNCTION_NG;
	}

	//----------------------------------------------------------------
	// 【 翌期更新 】
	//		①：一括金額行を元に戻す
	//		②：指定項目を０クリア
	//		③：現在のデータを全て一時保管へ移動
	//		④：①～③にて、頁/行、頁計/累計を再設定
	//----------------------------------------------------------------

	// uc_inf_yokuki参照
	rsYokuki.RequeryFormSeq(intFormSeq);

	if (!rsYokuki.IsEOF()) {
		// 翌期更新の対象帳表？
		if (rsYokuki.m_OpYokuki == 1) {
			// 各帳表のdbオブジェクト取得
			pData1 = GetDataTableObject(pZmSub,intFormSeq);
			if ( pUcFuncCmn.CheckTableOpen( (CdbBase*)pData1 ) != TRUE ){
				rsYokuki.Fin();
				return FUNCTION_NG;
			}

// 21-0292 add -->
			// ⑮－１地代家賃の内訳書
			if(intFormSeq == ID_FORMNO_151 && chk != 0) {
				int		date=0,yy=0,md=0,ii=0;
// 158557 add -->
				int			sw=0,neymd=0;
				int			sdate=0,edate=0;
				UCHAR		ucsymd[4]={0},uceymd[4]={0};
				CVolDateDB	vd;
// 158557 add <--
				pData1->RequeryFgFunc(ID_FGFUNC_DATA,0);	// データ行を昇順で抽出
				while(!pData1->IsEOF())	{
// 158557 add -->
					sw=0;
					sdate = ((CdbUc151Tidaiyatin *)pData1)->m_SpDate;
					edate = ((CdbUc151Tidaiyatin *)pData1)->m_SpDate2;
					if(sdate > 0 && edate > 0)	{
						// 開始から期間が１年間になる終了(A)を取得
						int_bcd(ucsymd,sdate,4);
						vd.vd_geteymd(uceymd,&ucsymd[1]);
						if(uceymd[0] && uceymd[1] && uceymd[2])	{
							neymd = bcd_int(uceymd,3);
							neymd += ((sdate/1000000)*1000000);	// +20000000
							// 終了が(A)と同じであれば＋１年の対象とする
							if(neymd == edate)	{
								sw=1;
							}
						}
					}
					if(sw != 0)	{
// 158557 add <--
						pData1->Edit();
						for(ii=0; ii<2; ii++)	{
							if(ii == 0)	date = ((CdbUc151Tidaiyatin *)pData1)->m_SpDate;
							else		date = ((CdbUc151Tidaiyatin *)pData1)->m_SpDate2;
							if(date > 0)	{
								date = date + 10000;
								yy = date / 10000;
								md = date % 10000;
								if(md >= 228)	{
									if((yy % 4 == 0) && (yy % 100 != 0) || (yy % 400 == 0))	{
										if(md == 228)	date = date + 1;
									}
									else	{
										if(md == 229)	date = date - 1;
									}
								}
								if(ii == 0)	((CdbUc151Tidaiyatin *)pData1)->m_SpDate  = date;
								else		((CdbUc151Tidaiyatin *)pData1)->m_SpDate2 = date;
							}
						}
						// 書き込み
						pData1->Update();
// 158557 add -->
					}
// 158557 add <--
					// 次データへ移動
					pData1->MoveNext();
				}
				pData1->Fin();			}
// 21-0292 add <--

			// ①：一括金額行を元に戻す（必ず実行）
			// 金額等を０クリア、データを保管へ移動のいずれかに
			// チェックを付けている場合は一括金額行を元に戻す
			if(rsYokuki.m_OpClear == 1 || rsYokuki.m_OpNullClear == 1 || rsYokuki.m_OpHokan == 1)	{
// 157153 add -->
				CCtrlSubGeneralVar pcs(*pUcInfSub);
				if(bG_Kanso == TRUE) {
// 211022 del -->
					//IkkatuInsertNull(pData1,pZmSub,pcs,intFormSeq,pUcInfSub->m_OutKei,pUcInfSub->m_Sort1,pUcInfSub->m_OpOutNullLine,pUcInfSub->m_OpOutSyokei);
// 211022 del <--
// 211022 add -->
					int	sw=1;
					if(pUcInfSub->m_OpSykeiAuto != 0 && pUcInfSub->m_OpOutSyokei == 0)	{
						sw=0;
					}
					IkkatuInsertNull(pData1,pZmSub,pcs,intFormSeq,pUcInfSub->m_OutKei,pUcInfSub->m_Sort1,pUcInfSub->m_OpOutNullLine,sw);
// 211022 add <--
				}
// 157153 add <--
				// 一括金額行を抽出
				pData1->RequeryFgFunc(ID_FGFUNC_IKKATUAUTO, 1);		// 一括金額行を降順で抽出

				while (!pData1->IsEOF()) {
					nGroup = (int)pData1->m_NumGroup;

					// 一括金額行の元データを抽出（現行：ダイアログ側で実施していた処理）
					pData2 = GetDataTableObject(pZmSub,intFormSeq);
					pData2->UpdateFgShowRetIkkatu(nGroup);
					delete pData2;

					// 一括金額行を戻す（現行：フォームView側で実施していた処理）
					rv = CmnTblUpdateFgShowDataReturn(pZmSub, pUcInfSub, pData1->m_NumPage, pData1->m_NumRow, nGroup);
					if(rv == -1)	{
						return(FUNCTION_NG);
					}
					if(rv == 0)		{
						// 元データの一括金額行の削除
						CmnTblIkkatuAutoUpdateProc(pZmSub, pUcInfSub, nGroup, 0, 0, uKamoku);
						// 頁調整する
						CmnTblUpdatePageRowAllResetMain(pZmSub,pUcInfSub);							// 頁と行の再設定
					}
					pUcInfSub->Fin();
					// 次データへ移動
					pData1->MoveNext();
				}
				pData1->Fin();
// 157153 add -->
				if(bG_Kanso == TRUE) {
// 211022 del -->
					//IkkatuDeleteNull(pData1,pZmSub,pcs,intFormSeq,pUcInfSub->m_OutKei,pUcInfSub->m_OpOutNullLine,pUcInfSub->m_OpOutSyokei);
// 211022 del <--
// 211022 add -->
					int	sw=1;
					if(pUcInfSub->m_OpSykeiAuto != 0 && pUcInfSub->m_OpOutSyokei == 0)	{
						sw=0;
					}
					IkkatuDeleteNull(pData1,pZmSub,pcs,intFormSeq,pUcInfSub->m_OutKei,pUcInfSub->m_OpOutNullLine,sw);
// 211022 add <--
				}
// 157153 add <--
			}

			// ②：指定項目を０クリア
			if (rsYokuki.m_OpClear == 1) {
				// 帳表データの０クリア
				pData1->UpdateYokukiZeroClear(0);

				// *******************************************************
				// OutRangaiを追加した時にバグで更新できなくなっている。
				// が、特にバグ報告もないので仕様ということにした
				// *******************************************************
				// 欄外データの０クリア
				//switch (intFormSeq) {
				//case ID_FORMNO_051:
				//	pData2 = (CdbUc000Common*)(new CdbUc052Tanaoroshi2(m_pDB));
				//	pData2->UpdateYokukiZeroClear();
				//	delete pData2;
				//	break;
				//case ID_FORMNO_091:
				//	pData2 = (CdbUc000Common*)(new CdbUc092Kaikakekin2(m_pDB));
				//	pData2->UpdateYokukiZeroClear();
				//	delete pData2;
				//	break;
				//}
			}

			// ②－２：指定項目を空白にする
			if(rsYokuki.m_OpNullClear == 1)	{
				pData1->UpdateYokukiZeroClear(1);
			}

			// ③：現在のデータを全て一時保管へ移動
			if (rsYokuki.m_OpHokan == 1) {
				// 現在の一時保管データを全て削除（その他不要データも削除）
				pData1->DeleteFgShow(ID_FGSHOW_HOKAN);			//	1	一時保管
				pData1->DeleteFgShow(ID_FGSHOW_IKKATU);			//	2	一括金額
				pData1->DeleteFgShow(ID_FGSHOW_RET_HOKAN);		//  101	一時保管からの戻し対象
				pData1->DeleteFgShow(ID_FGSHOW_RET_IKKATU);		//  102 一括金額からの戻し対象

				// 空行、小計、中計行等の不要データを削除
				pData1->DeleteFgFunc(ID_FGFUNC_NULL);			//	 0	空行	
				pData1->DeleteFgFunc(ID_FGFUNC_SYOKEI);			//	10	小計行
				pData1->DeleteFgFunc(ID_FGFUNC_SYOKEINULL);		//	11	小計行（空行）
				pData1->DeleteFgFunc(ID_FGFUNC_CHUKEI);			//	12	中計行
				pData1->DeleteFgFunc(ID_FGFUNC_CHUKEINULL);		//	13	中計行（空行）
				pData1->DeleteFgFunc(ID_FGFUNC_RUIKEI);			//	14	累計行
				pData1->DeleteFgFunc(ID_FGFUNC_PAGEKEI);		//	15	頁計行
				pData1->DeleteFgFunc(ID_FGFUNC_IKKATUAUTO);		//	22	一括集計金額行（自動）
// midori 190505 add -->
				pData1->DeleteFgFunc(ID_FGFUNC_KAMOKU);			//	 3	科目行（見出し）
// midori 190505 add <--

				// 保管テーブルの保管データを全て削除
				CdbUcLstHokan mfcRec(pZmSub->m_database);
				mfcRec.DeleteFormSeq(intFormSeq);

				// 残ったデータ行＋一括集計金額行（手動）を一時保管へ
				pData1->UpdateYokukiMoveToHokan();
				
				// 保管テーブルへコピー
				//pView->CopyHokanDataToHokanTable(intFormSeq);
				CopyHokanDataToHokanTable(pZmSub,intFormSeq);

				pData1->UpdateYokukiMoveToHokan2();

				// 欄外データの削除
				switch (intFormSeq) {
				case ID_FORMNO_051:
					pData2 = (CdbUc000Common*)(new CdbUc052Tanaoroshi2(pZmSub->m_database));
					pData2->DeleteAllRecord();
					delete pData2;
					break;
				case ID_FORMNO_091:
					pData2 = (CdbUc000Common*)(new CdbUc092Kaikakekin2(pZmSub->m_database));
					pData2->DeleteAllRecord();
					delete pData2;
					break;
				}
			}

			// ④：①～③にて、頁/行、頁計/累計を再設定
			if (pData1->GetNumPage() > 0) {
				// 頁調整する
				CmnTblUpdatePageRowAllResetMain(pZmSub,pUcInfSub);							// 頁と行の再設定
			}
		}

		if (pData1 != NULL) {
			delete pData1;
		}
	}

	pUcInfSub->Fin();

	return FUNCTION_OK;
}

// -----------------------------------------------------------------------------------------------------------------------------
//	翌期更新：メイン処理シーケンス
//		IN		CDBZmSub*		財務クラス（翌期）
//				long			内訳書の決算期間（期首）
//				long			内訳書の決算期間（期末）
//		RET		int				FUNCTION_OK:正常終了
//								FUNCTION_NG:エラー
// -----------------------------------------------------------------------------------------------------------------------------
// midori UC_0048 del -->
//// midori UC_0039 del -->
////int UchiYokukiUpdateMainProc( CWnd* pWnd, CDBZmSub* pZmSub, long lngUchiSSymd, long lngUchiEEymd )
//// midori UC_0039 del <--
//// midori UC_0039 add -->
//int UchiYokukiUpdateMainProc( CWnd* pWnd, CDBZmSub* pZmSub, long lngUchiSSymd, long lngUchiEEymd, int pHirendo )
//// midori UC_0039 add <--
// midori UC_0048 del <--
// midori UC_0048 add -->
int UchiYokukiUpdateMainProc( CWnd* pWnd, CDBZmSub* pZmSub, long lngUchiSSymd, long lngUchiEEymd )
// midori UC_0048 add <--
{
	HINSTANCE		m_hDefInstance;				// <-----20111007呼び出し側のリソースハンドル
	m_hDefInstance = AfxGetResourceHandle();
	AfxSetResourceHandle( g_hInstance );		// DLL側のリソースハンドルセット

	long				lngZvolSSymd = 0;		// 財務マスターの決算期間（期首）
	long				lngZvolEEymd = 0;		// 財務マスターの決算期間（期末）
	int					i;
	int					rv=0;
	int					flg=-1;					// ダイアログ表示サイン -1:表示しない 0:翌期更新ダイアログ表示 1:前年度からの更新ダイアログ表示
// midori UC_0039 add -->
	BOOL				updsw=FALSE;
// midori UC_0039 add <--
// midori 155163_2 add -->
	BOOL				mntsw=FALSE;
	CString				cs=_T("");
// midori 155163_2 add <--
	CdbUcInfMain		rsMain(pZmSub->m_database);
	CUcFunctionCommon	pUcFuncCmn;

	if(ChkKakuteiUpd(pZmSub) == 0)	{
		// -----------------------------------------------------------------------------
		// ①過年度マスターを選択している
		// ②当年度内訳書マスターが更新されていない
		// ③当年度内訳書マスターが更新されていない状態で初めて過年度マスターを起動した
		// →上記の場合、過年度財務マスターの確定サインを、過年度内訳書マスターの
		// 　確定サインにセットする
		// -----------------------------------------------------------------------------
		if(KanendoKakuteiSet(pZmSub) != FUNCTION_OK)	{
			AfxSetResourceHandle( m_hDefInstance );
			return(FUNCTION_NG);
		}
	}

	// 財務マスターの決算期間を取得
	if(UchiYokukiGetZvolumeYmd(pZmSub, &lngZvolSSymd, &lngZvolEEymd) == FUNCTION_NG)	{
		AfxSetResourceHandle( m_hDefInstance );
		return(FUNCTION_NG);
	}

	// 初めて内訳書を起動した場合は実行しない
	if ((lngUchiSSymd == 0) && (lngUchiEEymd == 0)) {
	}
	else {
		flg = -1;
		// 決算期間を比較する
		// -----------------------------------------------------------------
		// flg	-1 : 処理を行わない
		//		0  : 翌期更新処理を行う
		//		1  : 前年度からの更新処理を行う
		//		2  : 翌期更新処理を行うがダイアログは表示しない
		// -----------------------------------------------------------------
// midori UC_0039 del -->
		//if((lngUchiSSymd != lngZvolSSymd) || (lngUchiEEymd != lngZvolEEymd))	{
// midori UC_0039 del <--
// midori UC_0039 add -->
		// 非連動は表示しない
		// 翌期更新後でかつ、決算期間が違えば表示する
		if(rsMain.Init() == DB_ERR_OK){
			if(rsMain.m_GeneralVar[2] & BIT_D6) {
				//rsMain.Edit();
				updsw = TRUE;
				//rsMain.m_GeneralVar[2] &= ~BIT_D6;
				//rsMain.Update();
			}
// midori 155163_2 add -->
			if(rsMain.m_GeneralVar[2] & BIT_D7) {
				mntsw = TRUE;
			}
// midori 155163_2 add <--
			rsMain.Fin();
		}
// midori UC_0042 del -->
		//if(pHirendo == 0 && updsw == TRUE && ((lngUchiSSymd != lngZvolSSymd) || (lngUchiEEymd != lngZvolEEymd)))	{
// midori UC_0042 del <--
// midori UC_0042 add -->
		if(updsw == TRUE && ((lngUchiSSymd != lngZvolSSymd) || (lngUchiEEymd != lngZvolEEymd)))	{
// midori UC_0042 add <--
// midori UC_0039 add <--
			flg = 0;
			// 財務と決算期間が違っていても、過年度マスターが選択されている場合、翌期更新ダイアログを表示しない
			if(IsKanendoMaster(pZmSub) == TRUE)	{
				flg = 2;
			}
		}
// midori 155163_2 add -->
		else if(((lngUchiSSymd != lngZvolSSymd) || (lngUchiEEymd != lngZvolEEymd)) && (updsw == FALSE && mntsw == FALSE))	{
			cs = _T("財務マスターと勘定科目内訳書マスターで決算期間が異なっているため更新処理を行います。");
			cs += _T("\n勘定科目内訳書の更新処理の選択を行いますか？");
			cs += _T("\n\n");
			cs += _T("「はい」　・・・ 更新方法を選択したうえで更新する。\n");
			cs += _T("「いいえ」・・・ 金額のクリアなどを行わず、決算期間のみ更新する。");
			if(ICSMessageBox(cs,MB_YESNO) == IDYES)	{
				flg = 0;
			}
		}
// midori 155163_2 add <--

		// 翌期更新を行わない場合、前年度からの更新を行うか判断する
		if(flg == -1)	{
			// 前年度からの更新フラグを取得する
			if(rsMain.Init() == DB_ERR_OK){
				if((rsMain.m_GeneralVar[1] & BIT_D4) == 0x10)	flg = 1;
				rsMain.Fin();
			}
		}

		// 翌期更新、前年度からの更新を行わない場合、処理を抜ける
		if (flg == -1) {
// midori UC_0039 add -->
			// 翌期更新後であるかどうかのフラグをＯＦＦにする
			if(rsMain.Init() == DB_ERR_OK){
				rsMain.Edit();
				rsMain.m_GeneralVar[2] &= ~BIT_D6;
// midori 155163_3 add -->
				// 決算期間が異なるだけで更新ダイアログを表示したフラグをＯＮにする
				rsMain.m_GeneralVar[2] |= BIT_D7;
// midori 155163_3 add <--
				rsMain.Update();
				rsMain.Fin();
			}
// midori UC_0039 add <--
// midori 155163_3 add -->
			// 財務で決算期間を修正した場合、内訳書の決算期間も書き換える(電子申告のため)
			if((lngUchiSSymd != lngZvolSSymd) || (lngUchiEEymd != lngZvolEEymd))	{
				if(rsMain.Init() == DB_ERR_OK){
					rsMain.UpdateSSymdEEymd(lngZvolSSymd,lngZvolEEymd);
					rsMain.Fin();
				}
			}
// midori 155163_3 add <--
			//リソースハンドルをexeに戻す
			AfxSetResourceHandle( m_hDefInstance );
			return(FUNCTION_OK);
		}

		// 翌期更新テーブルがオープン可?
		CdbUcInfYokuki mfcYokuki( pZmSub->m_database );
		if ( pUcFuncCmn.CheckTableOpen( (CdbBase*)&mfcYokuki ) != TRUE ){
			AfxSetResourceHandle( m_hDefInstance );
			return(FUNCTION_NG);
		}

		// 翌期更新の設定画面→ キャンセル：抜ける
		CdlgYokukiMain		dlgYMain( pWnd );		// 翌期更新設定ダイアログ
		if(flg == 2)	{
			rv = 2;
		}
		else	{
			rv = dlgYMain.ShowDialog(pZmSub->m_database,flg);
		}
		// 対象の内訳書が１件もない場合はダイアログを表示せず
		// "rv" に "ID_DLG_MAX" をセットして戻ります
		if(rv == ID_DLG_CANCEL)	{
			// キャンセル
			AfxSetResourceHandle( m_hDefInstance );
			return(FUNCTION_NG);
		}
		else if(rv == ID_DLG_OK)	{
			// ＯＫ -> 翌期更新の更新処理
			for (i = ID_FORMNO_011; i <= ID_FORMNO_162; i++) {
				// ①.預貯金 ～ ⑯-2.雑損失
				if ( UchiYokukiUpdateSubProc(pZmSub,i) != FUNCTION_OK ){
					AfxSetResourceHandle( m_hDefInstance );
					return(FUNCTION_NG);
				}
			}
			for (i = 0; i < 20; i++) {
				// ⑰-x.その他１＋⑱-x.その他２
				UchiYokukiUpdateSubProc(pZmSub,ID_FORMNO_171 + i);
				UchiYokukiUpdateSubProc(pZmSub,ID_FORMNO_181 + i);
			}
		}

		// 翌期更新を行う場合
		if (flg == 0 || flg == 2) {
			if(ChkKakuteiUpd(pZmSub) == 0)	{
				// 過年度財務マスターの「決算確定」を、内訳書の「入力データを確定する」にセットする
				if(KakuteiSet(pZmSub) != FUNCTION_OK)	{
					AfxSetResourceHandle( m_hDefInstance );
					return(FUNCTION_NG);
				}
			}
		}
		// 前年度からの更新実行サインをOFFにする
		if(rsMain.Init() == DB_ERR_OK){
			rsMain.Edit();
			CCtrlMainGeneralVar cm(rsMain);
			cm.bZenFlg = FALSE;
			cm.Set(rsMain);
// midori UC_0039 add -->
			// 翌期更新後であるかどうかのフラグをＯＦＦにする
			rsMain.m_GeneralVar[2] &= ~BIT_D6;
// midori UC_0039 add <--
// midori 155163_2 add -->
			// 決算期間が異なるだけで更新ダイアログを表示したフラグをＯＮにする
			rsMain.m_GeneralVar[2] |= BIT_D7;
// midori 155163_2 add <--
			rsMain.Update();
			rsMain.Fin();
		}
// midori 190301 add -->

// midori 190301 add <--
	}

	// 内訳書の決算期間を更新（キャンセルを選択した場合は更新しない）
	if(rsMain.Init() == DB_ERR_OK){
		rsMain.UpdateSSymdEEymd(lngZvolSSymd, lngZvolEEymd);
		rsMain.Fin();
	}

	//リソースハンドルをexeに戻す
	AfxSetResourceHandle( m_hDefInstance );

	return(FUNCTION_OK);
}

// -----------------------------------------------------------------------------------------------------------------------------
//	翌期更新：メイン処理シーケンス（翌期更新ダイアログの表示）
//		IN		CWnd*			親のウインドウハンドル
//				CDBZmSub*		財務クラス（翌期）
// midori 181002 0221 del -->
////				int				複数社一括更新を行う	0:行なわない	1:行なう
// midori 181002 0221 del <--
//		RET		int				FUNCTION_OK:正常終了
//								FUNCTION_NG:エラー
// -----------------------------------------------------------------------------------------------------------------------------
// midori 181002 0221 del -->
//int UchiYokukiUpdateMainProc2( CWnd* pWnd, CDBZmSub* pZmSub,int exupd )
// midori 181002 0221 del <--
// midori 181002 0221 add -->
int UchiYokukiUpdateMainProc2( CWnd* pWnd, CDBZmSub* pZmSub )
// midori 181002 0221 add <--
{
	HINSTANCE		m_hDefInstance;
	m_hDefInstance = AfxGetResourceHandle();
	AfxSetResourceHandle( g_hInstance );		// DLL側のリソースハンドルセット

	int					rv=0;
	int					flg=0;
	CUcFunctionCommon	pUcFuncCmn;
	CdlgYokukiMain		dlgYMain( pWnd );		// 翌期更新設定ダイアログ

	// 翌期更新テーブルがオープン可?
	CdbUcInfYokuki mfcYokuki( pZmSub->m_database );
	if ( pUcFuncCmn.CheckTableOpen( (CdbBase*)&mfcYokuki ) != TRUE ){
		AfxSetResourceHandle( m_hDefInstance );
		return(FUNCTION_NG);
	}

	// m_flg 0:翌期更新を表示 1:前年度からの更新を表示 2:翌期更新ダイアログの表示を行なわない 
	//       3:内訳書単体マスター翌期更新を表示（通常） 4:内訳書単体マスター翌期更新を表示（一括）
// midori 181002 0221 del -->
	//if(exupd == 0)	flg = 3;
	//else			flg = 4;
// midori 181002 0221 del <--
// midori 181002 0221 add -->
	flg = 3;
// midori 181002 0221 add <--
	// 翌期更新の設定画面→ キャンセル：抜ける
	rv = dlgYMain.ShowDialog(pZmSub->m_database,flg);

	// 対象の内訳書が１件もない場合はダイアログを表示せず
	// "rv" に "ID_DLG_MAX" をセットして戻ります
	if(rv == ID_DLG_CANCEL)	{
		// キャンセル
		AfxSetResourceHandle( m_hDefInstance );
		return(ID_DLG_CANCEL);
	}

	//リソースハンドルをexeに戻す
	AfxSetResourceHandle( m_hDefInstance );

	return(FUNCTION_OK);
}

// 要通知修正_22_24/01/24 add -->
// -----------------------------------------------------------------------------------------------------------------------------
// 選択したマスターが内訳書単独マスターかどうかを調べる
// 【戻り値】	int … 0:財務ｄｂマスター　1:内訳書単独マスター　2:その他の単独マスター
// -----------------------------------------------------------------------------------------------------------------------------
int GetUcTandokuSw2(int pApno, int pCocod, int pEymd)
{
	int				ii = 0;
	int				ymd = 0;
	int				ymd2 = 0;
	int				rv = 0;
	INT_PTR			max = 0;
	CString			cst = _T("");
	CString			emsg = _T("");
	CO_CTL_DB		co;
	CO_CTL_DB		coctl;
	CO_CTL_Array	coary;
	ICSMCTL*		mctl;

	while(1) {
		// ICSMCTLテーブルのデータベースに接続
		cst.Empty();
		PCOM_GetString(_T("SelectSqlServer"), cst.GetBuffer(256));
		cst.ReleaseBuffer();
		mctl = new ICSMCTL;
		if(mctl->SetDatabase(cst) < 0) {
			rv = -1;
			emsg = _T("データベースの初期化に失敗しました。");
			break;
		}

		// ICSMCTLテーブルをオープン
		if(mctl->vctl_open(MODE_WRITE, pApno, TYPE_MS) < 0) {
			rv = -1;
			emsg = _T("マスター管理テーブルのオープンに失敗しました。");
			break;
		}

		// ICSMCTLテーブルから会社情報を取得
		coctl.c_apno = pApno;
		int_bcd(coctl.c_cod, pCocod, 4);
		coary.RemoveAll();
		if(mctl->vctl_coinf(&coctl, &coary) < 0) {
			rv = -1;
			emsg = _T("マスター管理情報の取得に失敗しました。");
			break;
		}
		max = coary.GetSize();
		for(ii = 0; ii < max; ii++) {
			co = coary.GetAt(ii);
			ymd = bcd_int(&co.c_kikan[3], 3);
			ymd += (co.c_kikan[3] < 0x89) ? 20000000 : 19000000;
			// 現在選択している事業期間の場合、財務マスターが作成されているかどうかを調べる
			if(ymd == pEymd) {
				rv = (co.c_exbin[128] & 0x01);
				if(rv != 0) {
					if((co.c_subsw & 0x100000) == 0x100000) {
						rv = 1;
					}
					else {
						rv = 2;
					}
				}
				break;
			}
		}
		break;
	}
	if(rv == -1) {
		ICSMessageBox(emsg, MB_ICONSTOP);
	}
	return(rv);
}

// ----------------------------------------------------------------------------------------------------------
//	インボイス登録番号カラム拡張
// ----------------------------------------------------------------------------------------------------------
int InvNoColumnConvert2(CDBZmSub* pZms)
{
	int					sw=0;
	int					ii=0;
	CString				strSQL;
	CString				sData;
	CdbUcInfMain		mfcMain(pZms->m_database);
	CRecordset			rs(pZms->m_database);
	CdbUc000Common*		rsData;

	//※要通知の件があったのでしばらくコメント
	// uc_inf_main::GeneralVar[4] & BIT_D6(0x40)がONになっていれば処理を行わない
	//if(mfcMain.Init() == DB_ERR_OK)	{
	//	if(mfcMain.m_GeneralVar[4] & BIT_D6)	sw = 1;
	//	mfcMain.Fin();
	//	if(sw != 0)	{
	//		return(0);
	//	}
	//}

	strSQL.Empty();
	// 各様式にインボイス登録番号カラムを追加
	// uc_021_uketoritegata (Table update)
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_021_uketoritegata]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_021_uketoritegata'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_021_uketoritegata add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");

	// uc_031_urikakekin (Table update)
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_031_urikakekin]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_031_urikakekin'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_031_urikakekin add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");

	// uc_041_karibarai (Table update)
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_041_karibarai]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_041_karibarai'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_041_karibarai add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");

	// uc_042_kasituke (Table update)
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_042_kasituke]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_042_kasituke'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_042_kasituke add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");

	// uc_071_koteishisan (Table update)
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_071_koteishisan]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_071_koteishisan'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_071_koteishisan add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");

	// uc_081_siharaitegata (Table update)
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_081_siharaitegata]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_081_siharaitegata'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_081_siharaitegata add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");

	// uc_091_kaikakekin (Table update)
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_091_kaikakekin]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_091_kaikakekin'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_091_kaikakekin add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");

	// uc_101_kariukekin (Table update)
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_101_kariukekin]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_101_kariukekin'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_101_kariukekin add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");

	// uc_121_totiuriage (Table update)
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_121_totiuriage]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_121_totiuriage'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_121_totiuriage add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");

	// uc_151_tidaiyatin (Table update)
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_151_tidaiyatin]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_151_tidaiyatin'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_151_tidaiyatin add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");

	// uc_152_kenrikin (Table update)
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_152_kenrikin]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_152_kenrikin'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_152_kenrikin add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");

	// uc_153_kougyou (Table update)
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_153_kougyou]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_153_kougyou'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_153_kougyou add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");

	// uc_161_zatueki (Table update)
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_161_zatueki]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_161_zatueki'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_161_zatueki add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");

	// uc_162_zatuson (Table update)
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_162_zatuson]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_162_zatuson'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_162_zatuson add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");

	// uc_171_sonota1 (Table update)
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_171_sonota1]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_171_sonota1'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_171_sonota1 add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");

	// uc_lst_addressにインボイス登録番号カラムを追加
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_lst_address]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_lst_address'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_lst_address add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");

	// uc_ren_darec_tempにインボイス登録番号カラムを追加
	strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_ren_darec_temp]') AND type in (N'U')) begin \r\n");
	strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_ren_darec_temp'),'Invno','PRECISION')) is null \r\n");
	strSQL += _T("ALTER TABLE uc_ren_darec_temp add Invno varchar(14) COLLATE Japanese_CI_AS NULL \r\n");
	strSQL += _T("end \r\n");

	try	{
		pZms->m_database->ExecuteSQL(strSQL);
	}
	catch(CDBException* dbe)	{
		ICSMessageBox(dbe->m_strError);
		dbe->Delete();
		return(-1);
	}

	// -------------------------

	// uc_lst_item_sort 各様式のソートに"登録番号"を追加
	strSQL.Format(_T("SELECT count(FormSeq) FROM uc_lst_item_sort WHERE FormSeq = 2 AND ItemSeq = 9"));
	try	{
		rs.Open(CRecordset::forwardOnly, strSQL);
		rs.GetFieldValue((short)0, sData);
		rs.Close();
	}

	catch(CDBException* dbe)	{
		ICSMessageBox(dbe->m_strError);
		dbe->Delete();
		return(-1);
	}
	
	if(_tstoi(sData) == 0)	{
		strSQL.Empty();

		// 様式２
		strSQL  = _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		//strSQL += _T("VALUES(2, 9, '登録番号(法人番号)', 1, 1, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");		// 修正No.168394 del
		strSQL += _T("VALUES(2, 9, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");		// 修正No.168394 add

		// 様式３
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		//strSQL += _T("VALUES(3, 5, '登録番号(法人番号)', 0, 1, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");		// 修正No.168394 del
		strSQL += _T("VALUES(3, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");		// 修正No.168394 add

		// 様式４－１
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		//strSQL += _T("VALUES(4, 5, '登録番号(法人番号)', 0, 1, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");		// 修正No.168394 del
		strSQL += _T("VALUES(4, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");		// 修正No.168394 add

		// 様式４－２
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		//strSQL += _T("VALUES(5, 4, '登録番号(法人番号)', 0, 1, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");		// 修正No.168394 del
		strSQL += _T("VALUES(5, 4, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");		// 修正No.168394 add

		// 様式７
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(8, 7, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");

		// 様式８
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		//strSQL += _T("VALUES(9, 8, '登録番号(法人番号)', 1, 1, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");		// 修正No.168394 del
		strSQL += _T("VALUES(9, 8, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");		// 修正No.168394 add

		// 様式９
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		//strSQL += _T("VALUES(10, 5, '登録番号(法人番号)', 0, 1, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");	// 修正No.168394 del
		strSQL += _T("VALUES(10, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");		// 修正No.168394 add

		// 様式１０－１
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		//strSQL += _T("VALUES(11, 5, '登録番号(法人番号)', 0, 1, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");	// 修正No.168394 del
		strSQL += _T("VALUES(11, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");		// 修正No.168394 add

		// 様式１２
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(14, 8, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");

		// 様式１５－１
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(18, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");

		// 様式１５－２
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(19, 6, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");

		// 様式１５－３
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(20, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");

		// 様式１６－１
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		//strSQL += _T("VALUES(21, 6, '登録番号(法人番号)', 0, 1, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");	// 修正No.168394 del
		strSQL += _T("VALUES(21, 6, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");		// 修正No.168394 add

		// 様式１６－２
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		//strSQL += _T("VALUES(22, 6, '登録番号(法人番号)', 0, 1, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");	// 修正No.168394 del
		strSQL += _T("VALUES(22, 6, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");		// 修正No.168394 add

		// 様式１７－１～２０
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(101, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(102, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(103, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(104, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(105, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(106, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(107, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(108, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(109, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(110, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(111, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(112, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(113, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(114, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(115, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(116, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(117, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(118, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(119, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");
		strSQL += _T("INSERT INTO uc_lst_item_sort(FormSeq,ItemSeq,ItemName,FgPage,FgSykei,FgItem,OrderStr,FieldSykei,FieldSykei2,OrderStr2) ");
		strSQL += _T("VALUES(120, 5, '登録番号(法人番号)', 0, 0, 0, 'IIF(Invno LIKE ''T%'' , 0, 1) ASC, IIF(Invno is NOT NULL, 2, 3) ASC, Invno ASC', NULL, NULL, NULL) \r\n");

		try	{
			pZms->m_database->ExecuteSQL(strSQL);
		}
		catch(CDBException* dbe)	{
			ICSMessageBox(dbe->m_strError);
			dbe->Delete();
			return(-1);
		}
	}

	// コンバート処理が終わったので、uc_inf_main::GeneralVar[4] & BIT_D6(0x40)をONにする
	if(mfcMain.Init() == DB_ERR_OK)	{
		mfcMain.Edit();
		mfcMain.m_GeneralVar[4] |= BIT_D6;
		mfcMain.Update();
		mfcMain.Fin();
	}

	return(0);
}
// 要通知修正_22_24/01/24 add <--

// 改良No.21-0086,21-0529 add -->
// ----------------------------------------------------------------------------------------------------------
//	様式⑧支払手形の内訳書の科目入力追加、「電子記録○○」等科目追加に関するカラム拡張
// ----------------------------------------------------------------------------------------------------------
int KamokuAddColumnConvert3(CDBZmSub* pZms)
{
	int						sw=0;
	CString					strSQL;
	CString					sData;
	CRecordset*				prs;

	// 様式⑧に必要なカラムが追加されているか確認
	strSQL.Empty();
	strSQL = _T("SELECT COLUMNPROPERTY(OBJECT_ID('uc_081_siharaitegata'),'KnSeq','PRECISION') ");
	prs = new CRecordset(pZms->m_database);
	prs->Open(CRecordset::forwardOnly, strSQL);
	prs->GetFieldValue((short)0, sData);
	sw = _tstoi(sData);
	prs->Close();
	if(sw == 0)	{	// カラムが存在しない
		// 様式⑧に科目に関するカラムを追加
		strSQL.Empty();

		// uc_081_siharaitegata (Table update)
		// KnSeq
		strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_081_siharaitegata]') AND type in (N'U')) begin \r\n");
		strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_081_siharaitegata'),'KnSeq','PRECISION')) is null \r\n");
		strSQL += _T("ALTER TABLE uc_081_siharaitegata add KnSeq int NULL \r\n");
		strSQL += _T("end \r\n");
		// KnOrder
		strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_081_siharaitegata]') AND type in (N'U')) begin \r\n");
		strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_081_siharaitegata'),'KnOrder','PRECISION')) is null \r\n");
		strSQL += _T("ALTER TABLE uc_081_siharaitegata add KnOrder int NULL \r\n");
		strSQL += _T("end \r\n");
		// KnName
		strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_081_siharaitegata]') AND type in (N'U')) begin \r\n");
		strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_081_siharaitegata'),'KnName','PRECISION')) is null \r\n");
		strSQL += _T("ALTER TABLE uc_081_siharaitegata add KnName varchar(30) COLLATE Japanese_CI_AS NULL \r\n");
		strSQL += _T("end \r\n");
		// KnKana
		strSQL += _T("IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[uc_081_siharaitegata]') AND type in (N'U')) begin \r\n");
		strSQL += _T("IF(SELECT COLUMNPROPERTY(OBJECT_ID('uc_081_siharaitegata'),'KnKana','PRECISION')) is null \r\n");
		strSQL += _T("ALTER TABLE uc_081_siharaitegata add KnKana varchar(6) COLLATE Japanese_CI_AS NULL \r\n");
		strSQL += _T("end \r\n");
		try {
			pZms->m_database->ExecuteSQL(strSQL);
		}
		catch(CDBException* dbe) {
			ICSMessageBox(dbe->m_strError);
			dbe->Delete();
			return(-1);
		}

		// -------------------------
		strSQL += _T("UPDATE uc_081_siharaitegata SET KnSeq=0, KnOrder=0 \r\n");
		try {
			pZms->m_database->ExecuteSQL(strSQL);
		}
		catch(CDBException* dbe) {
			ICSMessageBox(dbe->m_strError);
			dbe->Delete();
			return(-1);
		}
	}

	return(0);
}
// 改良No.21-0086,21-0529 add <--



// -----------------------------------------------------------------------------------------------------------------------------
//	翌期更新：メイン処理シーケンス（更新部分）
//		IN		CWnd*			親のウインドウハンドル
//				CDBZmSub*		財務クラス（翌期）
// midori 181002 0223 add -->
//				int				0:通常の翌期更新　1:過年度→当年度 OR 他システムからの呼び出し
// midori 181002 0223 add <--
//		RET		int				FUNCTION_OK:正常終了
//								FUNCTION_NG:エラー
// -----------------------------------------------------------------------------------------------------------------------------
// midori 181002 0223 del -->
//int UchiYokukiUpdateMainProc2_2( CWnd* pWnd, CDBZmSub* pZmSub )
// midori 181002 0223 del <--
// midori 181002 0223 add -->
int UchiYokukiUpdateMainProc2_2( CWnd* pWnd, CDBZmSub* pZmSub, int pSw )
// midori 181002 0223 add <--
{
	HINSTANCE		m_hDefInstance;
	m_hDefInstance = AfxGetResourceHandle();
	AfxSetResourceHandle( g_hInstance );		// DLL側のリソースハンドルセット

	long				lngZvolSSymd=0;			// 決算期間（期首）
	long				lngZvolEEymd=0;			// 決算期間（期末）
	int					ii=0;
// midori 181002 0223 add -->
	int					sub_sw=0;
// midori 181002 0223 add <--
// midori 181001 add -->
	int					opensw = 0;
// midori 181002_2 add -->
	int					nbuf = 0;
// midori 181002_2 add <--
	CString				strSQL=_T("");
// midori 181001 add <--
	CdbUcInfMain		rsMain(pZmSub->m_database);

// 要通知修正_22_24/01/24 add -->
	int			ret;
	char		szSQLServer[32]	={0};		// SQLサーバ名
	char		szApno[32]		={0};		// アプリケーション番号
	char		szCoCode[32]	={0};		// 会社コード
	char		szKikan[32]		={0};		// 期末年月日

	// C:\ICSWin\COMM\ログイン名\PCOM.INFの内容を読み込む
	ret = PCOM_GetString( "SELECTCODSN", szSQLServer, szApno, szCoCode, szKikan);
	if(ret < 4)	{
		AfxSetResourceHandle( m_hDefInstance );
		return(FUNCTION_NG);
	}
	// 起動しているマスターが単独（非連動）マスターかどうかをチェック
	if(GetUcTandokuSw2(atoi(szApno), atoi(szCoCode), atoi(szKikan)) == 1)	{
		// 非連動で翌期更新した時に、勘定科目内訳書を起動せず更新が可能になっているため、
		// ここで、インボイス登録番号用に必要なカラム等を追加する
		if(InvNoColumnConvert2(pZmSub) != 0) {
			AfxSetResourceHandle( m_hDefInstance );
			return(FUNCTION_NG);
		}

// 改良No.21-0086,21-0529 add -->
		// 支払手形の内訳書にて、追加になった科目情報のカラムを追加する
		if(KamokuAddColumnConvert3(pZmSub) != 0) {
			AfxSetResourceHandle( m_hDefInstance );
			return(FUNCTION_NG);
		}
// 改良No.21-0086,21-0529 add <--
	}
// 要通知修正_22_24/01/24 add <--

	// ＯＫ -> 翌期更新の更新処理
	for (ii = ID_FORMNO_011; ii <= ID_FORMNO_162; ii++) {
		// ①.預貯金 ～ ⑯-2.雑損失
		if ( UchiYokukiUpdateSubProc(pZmSub,ii) != FUNCTION_OK ){
			AfxSetResourceHandle( m_hDefInstance );
			return(FUNCTION_NG);
		}
	}
	for (ii = 0; ii < 20; ii++) {
		// ⑰-x.その他１＋⑱-x.その他２
		UchiYokukiUpdateSubProc(pZmSub,ID_FORMNO_171 + ii);
		UchiYokukiUpdateSubProc(pZmSub,ID_FORMNO_181 + ii);
	}

	// 内訳書の決算期間を更新（キャンセルを選択した場合は更新しない）
	if(rsMain.Init() == DB_ERR_OK){
		// 財務マスターの決算期間を取得
		if(UchiYokukiGetZvolumeYmd(pZmSub,&lngZvolSSymd,&lngZvolEEymd) == FUNCTION_NG)	{
			//リソースハンドルをexeに戻す
			AfxSetResourceHandle( m_hDefInstance );
			return(FUNCTION_NG);
		}

		rsMain.UpdateSSymdEEymd(lngZvolSSymd, lngZvolEEymd);
		rsMain.Fin();
	}

// midori 181001 add -->
// midori 181002 0223 del -->
	//// 当期zvolumeのsub_swの勘定科目内訳書(D20 0x100000)をOnにする
	//strSQL = _T("UPDATE zvolume SET sub_sw = sub_sw + 0x100000 ");
	//if(ExecuteSQLWork(pZmSub->m_database,strSQL) != 0)	{
	//	sG_Err = _T("テーブル(zvolume)の更新に失敗しました。");
	//	return(FUNCTION_NG);
	//}
// midori 181002 0223 del <--

	if(pZmSub->zvol == NULL) {
		if ( pZmSub->VolumeOpen() != 0 ) {
			sG_Err = pZmSub->GetZmErrMsg();
			return(FUNCTION_NG);
		}
		opensw = 1;
	}
	// 会社情報(corec)テーブルのsubswの勘定科目内訳書(D20 0x100000)をOnにする
	if(SetCoRec(pZmSub->zvol->apno,pZmSub->zvol->v_cod,pZmSub->zvol->ee_ymd,1,&sG_Err) == -1) {
		return(FUNCTION_NG);
	}
// midori 181002 0223 add -->
	sub_sw = pZmSub->zvol->sub_sw;
// midori 181002 0223 add <--
	if(opensw == 1) {
		pZmSub->VolumeClose();
	}
// midori 181001 add <--

// midori 181002 0223 add -->
	if(pSw != 0)	{
// midori 181002_2 del -->
		//if((sub_sw & 0x100000) != 0x100000)	{
		//	// 当期zvolumeのsub_swの勘定科目内訳書(D20 0x100000)をOnにする
		//	strSQL = _T("UPDATE zvolume SET sub_sw = sub_sw + 0x100000 ");
		//	if(ExecuteSQLWork(pZmSub->m_database,strSQL) != 0)	{
		//		return(FUNCTION_NG);
		//	}
		//}
// midori 181002_2 del <--
// midori 181002_2 add -->
		if((sub_sw & 0x100000) != 0x100000 || (sub_sw & 0x20) != 0x20)	{
			nbuf = 0;
			// 当期zvolumeのsub_swの勘定科目内訳書(D20 0x100000)をOnにする
			if((sub_sw & 0x100000) != 0x100000)	nbuf += 0x100000;
			// 当期zvolumeのsub_swの勘定科目内訳書(D5 0x20)をOnにする
			if((sub_sw & 0x20) != 0x20)			nbuf += 0x20;
			strSQL.Format(_T("UPDATE zvolume SET sub_sw = sub_sw + %d "),nbuf);
			if(ExecuteSQLWork(pZmSub->m_database,strSQL) != 0)	{
				return(FUNCTION_NG);
			}
		}
// midori 181002_2 add <--
	}
// midori 181002 0223 add <--

// midori UC_0042 add -->
		CdbUcInfMain	mfcMain(pZmSub->m_database);
		if(mfcMain.Init() == DB_ERR_OK) {
			mfcMain.Edit();
			mfcMain.m_GeneralVar[2] &= ~BIT_D6;
			mfcMain.Update();
			mfcMain.Fin();
		}
// midori UC_0042 add <--

	//リソースハンドルをexeに戻す
	AfxSetResourceHandle( m_hDefInstance );

	return(FUNCTION_OK);
}
// midori 180503 add <--

// midori 181001 add -->
// -----------------------------------------------------------------------------------------------------------------------------
//	翌期更新：メイン処理シーケンス（翌期更新ダイアログの表示）
//		IN		CWnd*			親のウインドウハンドル
//				CDBZmSub*		財務クラス（翌期）
//		RET		int				FUNCTION_OK:正常終了
//								FUNCTION_NG:エラー
// -----------------------------------------------------------------------------------------------------------------------------
int UchiYokukiUpdateMainProc3( CWnd* pWnd, CDBZmSub* pZmSub )
{
	HINSTANCE		m_hDefInstance;
	m_hDefInstance = AfxGetResourceHandle();
	AfxSetResourceHandle( g_hInstance );		// DLL側のリソースハンドルセット

	int					rv=0;
	int					flg=0;
	CUcFunctionCommon	pUcFuncCmn;
	CdlgYokukiMain		dlgYMain( pWnd );		// 翌期更新設定ダイアログ

	// 翌期更新テーブルがオープン可?
	CdbUcInfYokuki mfcYokuki( pZmSub->m_database );
	if ( pUcFuncCmn.CheckTableOpen( (CdbBase*)&mfcYokuki ) != TRUE ){
		AfxSetResourceHandle( m_hDefInstance );
		return(FUNCTION_NG);
	}

	// m_flg 0:翌期更新を表示 1:前年度からの更新を表示 2:翌期更新ダイアログの表示を行なわない 
	//       3:内訳書単体マスター翌期更新を表示（通常） 4:内訳書単体マスター翌期更新を表示（一括）
	//       5:内訳書単体マスター前年度からの更新を表示
	// 翌期更新の設定画面→ キャンセル：抜ける
	rv = dlgYMain.ShowDialog(pZmSub->m_database,5);

	// 対象の内訳書が１件もない場合はダイアログを表示せず
	// "rv" に "ID_DLG_MAX" をセットして戻ります
	if(rv == ID_DLG_CANCEL)	{
		// キャンセル
		AfxSetResourceHandle( m_hDefInstance );
		return(ID_DLG_CANCEL);
	}

	//リソースハンドルをexeに戻す
	AfxSetResourceHandle( m_hDefInstance );

	return(FUNCTION_OK);
}

// -----------------------------------------------------------------------------------------------------------------------------
//	翌期更新：メイン処理シーケンス（更新部分）
//		IN		CWnd*			親のウインドウハンドル
//				CDBZmSub*		財務クラス（翌期）
//		RET		int				FUNCTION_OK:正常終了
//								FUNCTION_NG:エラー
// -----------------------------------------------------------------------------------------------------------------------------
int UchiYokukiUpdateMainProc3_2( CWnd* pWnd, CDBZmSub* pZmSub )
{
	HINSTANCE		m_hDefInstance;
	m_hDefInstance = AfxGetResourceHandle();
	AfxSetResourceHandle( g_hInstance );		// DLL側のリソースハンドルセット

	long				lngZvolSSymd=0;			// 決算期間（期首）
	long				lngZvolEEymd=0;			// 決算期間（期末）
	int					ii=0;
	CdbUcInfMain		rsMain(pZmSub->m_database);

// 要通知修正_22_24/01/24 add -->
	int			ret;
	char		szSQLServer[32]	={0};		// SQLサーバ名
	char		szApno[32]		={0};		// アプリケーション番号
	char		szCoCode[32]	={0};		// 会社コード
	char		szKikan[32]		={0};		// 期末年月日

	// C:\ICSWin\COMM\ログイン名\PCOM.INFの内容を読み込む
	ret = PCOM_GetString( "SELECTCODSN", szSQLServer, szApno, szCoCode, szKikan);
	if(ret < 4)	{
		AfxSetResourceHandle( m_hDefInstance );
		return(FUNCTION_NG);
	}
	// 起動しているマスターが単独（非連動）マスターかどうかをチェック
	if(GetUcTandokuSw2(atoi(szApno), atoi(szCoCode), atoi(szKikan)) == 1)	{
		// 非連動で翌期更新した時に、勘定科目内訳書を起動せず更新が可能になっているため、
		// ここで、インボイス登録番号用に必要なカラム等を追加する
		if(InvNoColumnConvert2(pZmSub) != 0) {
			AfxSetResourceHandle( m_hDefInstance );
			return(FUNCTION_NG);
		}

// 改良No.21-0086,21-0529 add -->
		// 支払手形の内訳書にて、追加になった科目情報のカラムを追加する
		if(KamokuAddColumnConvert3(pZmSub) != 0) {
			AfxSetResourceHandle( m_hDefInstance );
			return(FUNCTION_NG);
		}
// 改良No.21-0086,21-0529 add <--
	}
// 要通知修正_22_24/01/24 add <--

	// ＯＫ -> 翌期更新の更新処理
	for (ii = ID_FORMNO_011; ii <= ID_FORMNO_162; ii++) {
		// ①.預貯金 ～ ⑯-2.雑損失
		if ( UchiYokukiUpdateSubProc(pZmSub,ii) != FUNCTION_OK ){
			AfxSetResourceHandle( m_hDefInstance );
			return(FUNCTION_NG);
		}
	}
	for (ii = 0; ii < 20; ii++) {
		// ⑰-x.その他１＋⑱-x.その他２
		UchiYokukiUpdateSubProc(pZmSub,ID_FORMNO_171 + ii);
		UchiYokukiUpdateSubProc(pZmSub,ID_FORMNO_181 + ii);
	}

	// 前年度からの更新実行サインをOFFにする
	if(rsMain.Init() == DB_ERR_OK){
		rsMain.Edit();
		CCtrlMainGeneralVar cm(rsMain);
		cm.bZenFlg = FALSE;
		cm.Set(rsMain);
		rsMain.Update();
		rsMain.Fin();
	}

	//リソースハンドルをexeに戻す
	AfxSetResourceHandle( m_hDefInstance );

	return(FUNCTION_OK);
}
// midori 181001 add <--

// midori 181002 add -->
// ----------------------------------------------------------------------------------------------------------
// 勘定科目内訳書のマスターが作成されていない場合、メッセージを表示する
// ----------------------------------------------------------------------------------------------------------
// 戻り値　0:マスター無し 1:マスターあり -1:エラー
// ----------------------------------------------------------------------------------------------------------
int MasterCheck(void)
{
	int			ret = 0;
	int			rv = 0;
	char		szSQLServer[32]	={0};		// SQLサーバ名
	char		szApno[32]		={0};		// アプリケーション番号
	char		szCoCode[32]	={0};		// 会社コード
	char		szKikan[32]		={0};		// 期末年月日
	CString		cst = _T("");

	// C:\ICSWin\COMM\ログイン名\PCOM.INFの内容を読み込む
	ret = PCOM_GetString( "SELECTCODSN", szSQLServer, szApno, szCoCode, szKikan);

	if( ret < 4) {
		// PCOM_GetString() でうまく取得できなかった場合
		// ここで 4 未満としているのは、
		//  -1 が失敗（多分 PCOM_GetString() 自体で何らかの失敗)
		//  正の値は、取得できたパラメータ数
		//    今回の場合、取得項目が4項目なので 4未満の条件ととした。
		//    # 第一引数に、存在しないセクション名を指定しても 0 が返ってくる
		//    # (-1は返ってこない)
		//ICSExit( 0x0205, "情報が不完全です。");
		ICSMessageBox( _T("コモン情報が、取得できません。(SELECTCODSN)") );
		return(-1);
	}

	rv = ChkCoRec(atoi(szApno),atoi(szCoCode),atoi(szKikan),&cst);
	if(rv == -1) {
		// エラー
		ICSMessageBox(cst);
	}
	// マスター無し
	if(rv == 0)	{
		cst = _T("選択した事業年度に勘定科目内訳書マスターが存在しません。\n");
// midori 8 del -->
		//cst += _T("処理を行う事業期間（期末）を正しく選択して下さい。\n");
// midori 8 del <--
// midori 8 add -->
		cst += _T("処理を行う事業期間（期末）を正しく選択してください。\n");
// midori 8 add <--
		cst += _T("\n");
		cst += _T("※当年度に更新する場合は、以下の手順で更新を行うことができます。\n");
// midori UC_0040 del -->
		//cst += _T("「勘定科目内訳書翌期更新」にて、\n");
		//cst += _T("会社選択の“過年度（Ｆ９）”から直前期を選択してください。\n");
// midori UC_0040 del <--
// midori UC_0040 add -->
		cst += _T("「翌期更新」にて、会社選択の“過年度（Ｆ９）”から直前期を選択してください。\n");
// midori UC_0040 add <--
		ICSMessageBox(cst);
	}

	return rv;
}

// ----------------------------------------------------------------------------------------------------------
//	内訳書マスター（内訳書関連の全てのテーブル）を削除し、
//	作成済みスイッチを落とす
// ----------------------------------------------------------------------------------------------------------
//	引数:		CDBZmSub*	zmsub	財務クラス
//				int			pSw		他業種サイン	1:他業種からの呼び出し
//
//	戻り値:	0:成功
//			-1:失敗
// ----------------------------------------------------------------------------------------------------------
int DeleteUtiTables(CDBZmSub* zmsub,int pSw,int kikan)
{
	int				apno			= 0;		// アプリケーション番号
	int				v_cod			= 0;		// 会社コード
	int				eymd			= 0;		// 事業期間（至）
	int				sub_sw			= 0;
// midori 181002_2 del -->
	int				nbuf			= 0;
// midori 181002_2 del <--
	CString			cs				=_T("");
	CString			strSQL			=_T("");
	CString			c_nam			= _T("");	// 会社名
	CDBPrtbl		pt;

	// テーブル名の頭3文字が"uc_"のテーブルを削除
	strSQL  = _T("DECLARE @name char(128) ");
	strSQL += _T("DECLARE cur CURSOR for SELECT name from sys.tables where name like 'uc_%' ");
	strSQL += _T("OPEN cur FETCH cur INTO @name WHILE @@fetch_status = 0 ");
	strSQL += _T("BEGIN ");
	strSQL += _T(	"exec('DROP TABLE ' + @name) ");
	strSQL += _T(	"FETCH cur INTO @name ");
	strSQL += _T("END CLOSE cur DEALLOCATE cur ");

	// zvolumeのsub_swのスイッチを落とす
	zmsub->VolumeOpen();
	sub_sw = zmsub->zvol->sub_sw;
	zmsub->VolumeClose();
	if(pSw == 0) {
		if((sub_sw & 0x20) == 0x20)	{
			strSQL += _T("UPDATE zvolume SET sub_sw = sub_sw - 0x20 ");
		}
	}
	else {
// midori 181002_2 del -->
		//if((sub_sw & 0x100000) == 0x100000)	{
		//	strSQL += _T("UPDATE zvolume SET sub_sw = sub_sw - 0x100000 ");
		//}
// midori 181002_2 del <--
// midori 181002_2 add -->
		if((sub_sw & 0x100000) == 0x100000 || (sub_sw & 0x20) == 0x20)	{
			nbuf = 0;
			if((sub_sw & 0x100000) == 0x100000)		nbuf += 0x100000;
			if((sub_sw & 0x20) == 0x20)				nbuf += 0x20;

			cs.Format(_T("UPDATE zvolume SET sub_sw = sub_sw - %d "),nbuf);
			strSQL += cs;
		}
// midori 181002_2 add <--
	}
	// vtblの内訳書レコードを削除
	cs.Format(_T(" DELETE FROM vtbl WHERE type = %d AND objname = '%s'"),DB_UC_VTBL_TYPE,DB_UC_VTBL_OBJNAME);
	strSQL += cs;

	// owntb の表紙情報を削除
	strSQL += _T(" DELETE FROM owntb WHERE apl_name = 'UCHIWAKE' AND itm_name = 'HyoshiData' AND itm_seq = 0");

	// owntbのインポート、エクスポート選択情報を削除
	strSQL += _T(" DELETE FROM owntb WHERE apl_name = 'DBUchiwake' AND itm_name = 'TRANS_TYPE'");

	// 保管参照、一括金額参照のリストの幅、位置情報を削除
	strSQL += _T(" DELETE FROM owntb WHERE apl_name = 'DBUCHIWAKE' AND itm_name = 'REPORTW'");
	strSQL += _T(" DELETE FROM owntb WHERE apl_name = 'DBUCHIWAKE' AND itm_name = 'REPORTV'");

	// トランザクション開始（ロック）
	zmsub->m_database->BeginTrans();

	try{
		// SQL実行
		zmsub->m_database->ExecuteSQL( strSQL );

		// トランザクションのコミット
		zmsub->m_database->CommitTrans();
	}
	catch(CDBException* e){
		sG_Err = e->m_strError;
		// 失敗したらロールバック
		zmsub->m_database->Rollback();
		e->Delete();
		return(-1);
	}

	if(pSw == 1)	{
		zmsub->VolumeOpen();
		// アプリケーション番号
		apno = zmsub->zvol->apno;
		// 会社コード
		v_cod = zmsub->zvol->v_cod;
		// 事業期間（至）
		if(kikan != 0)	eymd = kikan;
		else			eymd = zmsub->zvol->ee_ymd;
		// 会社名
		c_nam = zmsub->zvol->c_nam;
		zmsub->VolumeClose();

		// 決算期間情報・会社情報　初期化
		zmsub->m_database->BeginTrans();
		try{
			zmsub->m_database->ExecuteSQL( _T("delete from dbo.prfkin") );
		}
		catch( CDBException *e ){
			sG_Err = e->m_strError;
			zmsub->m_database->Rollback();
			e->Delete();
			return(-1);
		}
		zmsub->m_database->CommitTrans();

		pt.SetMasterDB( zmsub->m_database );
		if( pt.InitProof(zmsub) == -1 ){
			return(-1);
		}

		// 会社情報
		// SQLコマンド作成
		c_nam.Replace( _T("'"), _T("''") );	// 修正依頼No.4325
		strSQL.Format(_T("UPDATE [cpdata] SET cpyg='%s', cpua='', cpda='' "),c_nam);
		strSQL += _T("WHERE cpkofs = 0");
		// SQL文を実行
		zmsub->m_database->BeginTrans();
		try{
			zmsub->m_database->ExecuteSQL(strSQL);
		}
		catch( CDBException *e ){
			sG_Err = e->m_strError;
			zmsub->m_database->Rollback();
			e->Delete();
			return(-1);
		}
		zmsub->m_database->CommitTrans();

		// corecテーブルの勘定科目内訳書作成サインをOFFにする
		if(SetCoRec(apno,v_cod,eymd,0,&sG_Err) == -1) {
			return(-1);
		}
	}

	return 0;
}

// ----------------------------------------------------------------------------------------------------------
//	会社情報テーブル（corec)テーブルの勘定科目内訳書作成サインがＯＮであるかを調べる
// ----------------------------------------------------------------------------------------------------------
//	引数:		int			apno	アプリケーション番号
//				int			code	会社コード
//				int			eymd	期末年月
//				CString*	errmsg	エラーメッセージ
//
//	戻り値:		1:チェックＯＮ
//				0:チェックＯＦＦ
//				-1:失敗
// ----------------------------------------------------------------------------------------------------------
int ChkCoRec(int apno,int code,int eymd,CString* errmsg)
{
	int				ii			= 0;
	int				ymd			= 0;
	int				ymd2		= 0;
	int				ret			= 0;
	INT_PTR			max			= 0;
	CO_CTL_DB		co;
	CO_CTL_DB		coctl;
	CO_CTL_Array	coary;
	CString			cst			=_T("");
	ICSMCTL*		mctl;

	while( 1 ) {
		// ICSMCTLテーブルのデータベースに接続
		cst.Empty();
		PCOM_GetString( _T("SelectSqlServer"), cst.GetBuffer(256) );
		cst.ReleaseBuffer();
		mctl = new ICSMCTL;
		if(mctl->SetDatabase(cst) < 0)	{
			ret = -1;
			*errmsg = _T("データベースの初期化に失敗しました。");
			break;
		}

		// ICSMCTLテーブルをオープン
		if ( mctl->vctl_open( MODE_WRITE, apno, TYPE_MS ) < 0 ) {
			ret = -1;
			*errmsg = _T("マスター管理テーブルのオープンに失敗しました。");
			break;
		}

		// ICSMCTLテーブルから会社情報を取得
		coctl.c_apno = apno;
		int_bcd(coctl.c_cod,code,4);
		coary.RemoveAll();
		if ( mctl->vctl_coinf( &coctl, &coary ) < 0 ) {
			ret = -1;
			*errmsg = _T("マスター管理情報の取得に失敗しました。");
			break;
		}
		max = coary.GetSize();
		for(ii=0;ii<max;ii++) {
			co = coary.GetAt( ii );
			ymd = bcd_int( &co.c_kikan[3], 3 );
			ymd += (co.c_kikan[3] < 0x89) ? 20000000 : 19000000;
			// 現在選択している事業期間の場合
			if ( ymd == eymd ) {
				// 勘定科目内訳書マスター作成サイン(D20 0x100000)がONの場合、
				// 勘定科目内訳書マスター有り(1)を返す
				if((co.c_subsw & 0x100000) == 0x100000)		ret = 1;
				break;
			}
		}
		break;
	}
	mctl->vctl_close();
	// マスター管理クラス
	if(mctl != NULL)	{
		delete mctl;
		mctl = NULL;
	}

	return(ret);
}

// ----------------------------------------------------------------------------------------------------------
//	会社情報テーブル（corec)テーブルの勘定科目内訳書作成サインを変更する
// ----------------------------------------------------------------------------------------------------------
//	引数:		int			apno	アプリケーション番号
//				int			code	会社コード
//				int			eymd	事業期間（至）
//				int			pSw		0:作成サインOFF		1:作成サインON
//				CString*	errmsg	エラーメッセージ
// midori 190301 add -->
//				int			pHirendo	0:財務連動		1:非連動
// midori 190301 add <--
//
//	戻り値:		0:成功
//				-1:失敗
// ----------------------------------------------------------------------------------------------------------
int SetCoRec(int apno,int code,int eymd,int pSw,CString* errmsg)
{
	int				ii			= 0;
	int				ymd			= 0;
	int				ymd2		= 0;
	int				ret			= 0;
	INT_PTR			max			= 0;
	CO_CTL_DB		co;
	CO_CTL_DB		coctl;
	CO_CTL_Array	coary;
	CString			cst			=_T("");
	ICSMCTL*		mctl;

	while( 1 ) {
		// ICSMCTLテーブルのデータベースに接続
		cst.Empty();
		PCOM_GetString( _T("SelectSqlServer"), cst.GetBuffer(256) );
		cst.ReleaseBuffer();
		mctl = new ICSMCTL;
		if(mctl->SetDatabase(cst) < 0)	{
			ret = -1;
			*errmsg = _T("データベースの初期化に失敗しました。");
			break;
		}

		// ICSMCTLテーブルをオープン
		if ( mctl->vctl_open( MODE_WRITE, apno, TYPE_MS ) < 0 ) {
			ret = -1;
			*errmsg = _T("マスター管理テーブルのオープンに失敗しました。");
			break;
		}

		// ICSMCTLテーブルから会社情報を取得
		coctl.c_apno = apno;
		int_bcd(coctl.c_cod,code,4);
		coary.RemoveAll();
		if ( mctl->vctl_coinf( &coctl, &coary ) < 0 ) {
			ret = -1;
			*errmsg = _T("マスター管理情報の取得に失敗しました。");
			break;
		}
		max = coary.GetSize();
		for(ii=0;ii<max;ii++) {
			co = coary.GetAt( ii );
			ymd = bcd_int( &co.c_kikan[3], 3 );
			ymd += (co.c_kikan[3] < 0x89) ? 20000000 : 19000000;
			// 現在選択している事業期間の場合、勘定科目内訳書マスター作成サインを変更する
			if ( ymd == eymd ) {
				if( pSw == 0 && (co.c_subsw & 0x100000) == 0x100000 || 
					pSw == 1 && (co.c_subsw & 0x100000) == 0) {
// midori 181002_2 del -->
					//// 作成サインOFF
					//if(pSw == 0)	co.c_subsw &= ~0x100000;
					//// 作成サインON
					//else			co.c_subsw |= 0x100000;
// midori 181002_2 del <--
// midori 181002_2 add -->
					// 作成サインOFF
					if(pSw == 0)	{
						// 勘定科目内訳書サイン(0x20)
						co.c_subsw &= ~0x20;
						// 勘定科目内訳書（非連動）サイン(0x100000)
						co.c_subsw &= ~0x100000;
					}
					// 作成サインON
					else			{
						// 勘定科目内訳書サイン(0x20)
						co.c_subsw |= 0x20;
						// 勘定科目内訳書（非連動）サイン(0x100000)
						co.c_subsw |= 0x100000;
					}
// midori 181002_2 add <--
					if(mctl->vctl_comnt(co.seq,&co) < 0) {
						ret = -1;
						*errmsg = _T("マスター管理テーブルの更新に失敗しました。");
					}
				}
// midori UC_0035 add -->
				if(pSw == 0 && (co.c_subsw & 0x20) == 0x20)	{
					// 勘定科目内訳書サイン(0x20)OFF
					co.c_subsw &= ~0x20;
					if(mctl->vctl_comnt(co.seq,&co) < 0) {
						ret = -1;
						*errmsg = _T("マスター管理テーブルの更新に失敗しました。");
					}
				}
// midori UC_0035 add <--
				break;
			}
		}
		break;
	}
	mctl->vctl_close();
	// マスター管理クラス
	if(mctl != NULL)	{
		delete mctl;
		mctl = NULL;
	}

	return(ret);
}
// midori 181002 add <--

// midori UC_0035 add -->
int SetCoRec2(int apno,int code,int eymd,int pSw,CString* errmsg)
{
	int				ii			= 0;
	int				ymd			= 0;
	int				ymd2		= 0;
	int				ret			= 0;
	INT_PTR			max			= 0;
	CO_CTL_DB		co;
	CO_CTL_DB		coctl;
	CO_CTL_Array	coary;
	CString			cst			=_T("");
	ICSMCTL*		mctl;

	while( 1 ) {
		// ICSMCTLテーブルのデータベースに接続
		cst.Empty();
		PCOM_GetString( _T("SelectSqlServer"), cst.GetBuffer(256) );
		cst.ReleaseBuffer();
		mctl = new ICSMCTL;
		if(mctl->SetDatabase(cst) < 0)	{
			ret = -1;
			*errmsg = _T("データベースの初期化に失敗しました。");
			break;
		}

		// ICSMCTLテーブルをオープン
		if ( mctl->vctl_open( MODE_WRITE, apno, TYPE_MS ) < 0 ) {
			ret = -1;
			*errmsg = _T("マスター管理テーブルのオープンに失敗しました。");
			break;
		}

		// ICSMCTLテーブルから会社情報を取得
		coctl.c_apno = apno;
		int_bcd(coctl.c_cod,code,4);
		coary.RemoveAll();
		if ( mctl->vctl_coinf( &coctl, &coary ) < 0 ) {
			ret = -1;
			*errmsg = _T("マスター管理情報の取得に失敗しました。");
			break;
		}
		max = coary.GetSize();
		for(ii=0;ii<max;ii++) {
			co = coary.GetAt( ii );
			ymd = bcd_int( &co.c_kikan[3], 3 );
			ymd += (co.c_kikan[3] < 0x89) ? 20000000 : 19000000;
			// 現在選択している事業期間の場合、勘定科目内訳書マスター作成サインを変更する
			if ( ymd == eymd ) {
				// 作成サインOFF
				if(pSw == 0)	{
					// 勘定科目内訳書サイン(0x20)
					co.c_subsw &= ~0x20;
				}
				// 作成サインON
				else			{
					// 勘定科目内訳書サイン(0x20)
					co.c_subsw |= 0x20;
				}
				if(mctl->vctl_comnt(co.seq,&co) < 0) {
					ret = -1;
					*errmsg = _T("マスター管理テーブルの更新に失敗しました。");
				}
				break;
			}
		}
		break;
	}
	mctl->vctl_close();
	// マスター管理クラス
	if(mctl != NULL)	{
		delete mctl;
		mctl = NULL;
	}

	return(ret);
}
// midori UC_0035 add <-

// midori 156188 add -->
// ------------------------------------------------------------------------------
//	「科目行を挿入する」 有効/無効のサインを変更する
//	条件	①事業期間の期首が平成31年4月1日以降
//			②過年度確定で無い
//
//	引数	int		0:過年度確定サインのチェック無し
//					1:過年度確定サインのチェック有り
//			
// ------------------------------------------------------------------------------
//int ImpKamokuRowEnableSgn(CDatabase* pDB, int pSw)				// 改良No.21-0086,21-0529 del
int ImpKamokuRowEnableSgn(CDatabase* pDB, int pSw, int nFormSeq)	// 改良No.21-0086,21-0529 add
{
	CdbUcInfMain	mfcRecMain( pDB );	//	「uc_inf_main」テーブル用クラス
	int				symd=0,eymd=0;
	int				ret=0;
	BOOL			bG_KansoLock=FALSE;
	BOOL			bG_InvNoLock=FALSE;		// 改良No.21-0086,21-0529 add
	CRecordset		rs(pDB);
	CString			strSql = _T("");
	CString			strData[2] = {_T("")};

	try{
		rs.Open(CRecordset::forwardOnly,_T("select ss_ymd,ee_ymd from zvolume"));
		rs.GetFieldValue((short)0,strData[0]);
		rs.GetFieldValue((short)1,strData[1]);
		rs.Close();
	}catch(CDBException* e) {
		e->Delete();
		return(-1);
	}
	symd = _tstoi(strData[0]);
	eymd = _tstoi(strData[1]);

	// 1.期末が平成31年4月1日より前のマスターは現様式
	if(eymd < 20190401)	{
		bG_KansoLock = TRUE;
	}
	// 2.期首が平成31年4月1日以降のマスターは新様式
	else if(symd >= 20190401)	{
		bG_KansoLock = TRUE;
	}
	// 3.上記以外はプログラム内で様式を切り替えられるようにする
	else	{
		bG_KansoLock = FALSE;
	}

// 改良No.21-0086,21-0529 add -->
	// 1.期末が令和6年3月1日より前のマスター、またはインボイス改正マスターでないマスター　は簡素化様式
	if(eymd < 20240301 || m_Invoice == false)	{
		bG_InvNoLock = TRUE;
	}
	// 2.期首が令和6年3月1日以降且つインボイス改正マスターである マスターは登録番号追加版様式
	else if(symd >= 20240301 && m_Invoice == true)	{
		bG_InvNoLock = TRUE;
	}
	// 3.上記以外はプログラム内で様式を切り替えられるようにする
	else	{
		bG_InvNoLock = FALSE;
	}
// 改良No.21-0086,21-0529 add <--

	if(mfcRecMain.Init() == DB_ERR_OK)	{
		if(!(mfcRecMain.IsEOF()))	{
			mfcRecMain.MoveFirst();
			// (mfcRecMain.m_GeneralVar[2] & BIT_D1)	チェック有り：簡素化後の様式
			// bG_KansoLock == TRUE						簡素化前⇔簡素化後の変更不可
// 改良No.21-0086,21-0529 cor -->
			//if((mfcRecMain.m_GeneralVar[2] & BIT_D1) && bG_KansoLock == TRUE)	{
			//	ret = 1;
			//}
// ------------------------------
			if(nFormSeq == ID_FORMNO_081)	{	// nFormSeq が様式⑧(ID_FORMNO_081) 
				if((mfcRecMain.m_GeneralVar[2] & BIT_D1) && bG_KansoLock == TRUE && bG_InvNoLock == TRUE)	{
					ret = 1;
				}
			}
			else	{							// nFormSeq が様式⑧以外(NOT ID_FORMNO_081) または 指定なし(-1)
				if((mfcRecMain.m_GeneralVar[2] & BIT_D1) && bG_KansoLock == TRUE)	{
					ret = 1;
				}
			}
// 改良No.21-0086,21-0529 cor <--
			if(pSw == 1) {
				// 過年度確定
				if((mfcRecMain.m_GeneralVar[1] & BIT_D2)) {
					ret = 0;
				}
			}
		}
	}
	mfcRecMain.Fin();

	return(ret);
}
// midori 156188 add <--

// 24/02/09_ScoreLink add -->
// ----------------------------------------------------------------------------------------------------------
// ScoreLink用　ＣＳＶのヘッダ部のみ作成する
// ----------------------------------------------------------------------------------------------------------
//	引　数:		なし
//	戻り値:		TRUE:成功	FALSE:失敗
// ----------------------------------------------------------------------------------------------------------
BOOL SL_CsvExport( void )
{
	std::ofstream		file;
	CString				filepre=_T(""),filename=_T("");
	char				path[MAX_PATH] = {};
	CString				cs1=_T(""),cs2=_T(""),cs3=_T(""),cs4=_T("");

	// ＣＳＶファイルを作成
	PP_GetAppData(0x100, NULL, NULL, "BPR_PDFPTH", path);
    filepre = path;
    for(int ii=0; ii<SHEET_NUM; ii++)	{
		filename = filepre + '\\' + UcCsvFile[ii];
        file.open(filename, std::ios::out);
		if(!file)	{ 
			SL_Err = _T("ファイル作成エラー");
            return(FALSE);
		}
		cs1=_T("");
		cs2=_T("");
		cs3=_T("");
		cs4=_T("");
		switch(ii)	{
			case	0:
				cs1=_T("\"帳票名\",\"預貯金等の内訳書\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"金融機関名\",\"支店名\",\"種類\",\"口座番号\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	1:
				cs1=_T("\"帳票名\",\"受取手形の内訳書\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"登録番号（法人番号）\",\"振出人１\",\"振出人２\",\"振出年月日\",\"支払期日\",\"支払銀行名称\",\"支払銀行支店名\",\"金額\",\"割引銀行名及び支店名等１\",\"割引銀行名及び支店名等２\",\"摘要１\",\"摘要２\"");
				break;
			case	2:
				cs1=_T("\"帳票名\",\"売掛金（未収入金）の内訳書\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"登録番号（法人番号）\",\"相手先名称（氏名）１\",\"相手先名称（氏名）２\",\"相手先所在地（住所）１\",\"相手先所在地（住所）２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	3:
				cs1=_T("\"帳票名\",\"仮払金（前渡金）の内訳書\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"登録番号（法人番号）\",\"相手先名称（氏名）１\",\"相手先名称（氏名）２\",\"相手先所在地（住所）１\",\"相手先所在地（住所）２\",\"法人・代表者との関係\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	4:
				cs1=_T("\"帳票名\",\"貸付金及び受取利息の内訳書\"");
				cs2=_T("\"会社名\",\"\"");
// 24/02/19_ScoreLink cor -->
				//cs3=_T("\"[項目]\",\"登録番号（法人番号）\",\"貸付先名称（氏名）１\",\"貸付先名称（氏名）２\",\"貸付先所在地（住所）１\",\"貸付先所在地（住所）２\",\"法人・代表者との関係\",\"期末現在高\",\"期中の受取利息額\",\"利率\",\"担保の内容（物件の種類、数量、所在地等）１\",\"担保の内容（物件の種類、数量、所在地等）２\"");
// --------------------------
				cs3=_T("\"[項目]\",\"登録番号（法人番号）\",\"貸付先名称（氏名）１\",\"貸付先名称（氏名）２\",\"貸付先所在地（住所）１\",\"貸付先所在地（住所）２\",\"法人・代表者との関係\",\"期末現在高\",\"期中の受取利息額\",\"利率\",\"貸付理由１\",\"貸付理由２\",\"担保の内容（物件の種類、数量、所在地等）１\",\"担保の内容（物件の種類、数量、所在地等）２\"");
// 24/02/19_ScoreLink cor <--
				break;
			case	5:
				cs1=_T("\"帳票名\",\"棚卸資産（商品又は製品、半製品、仕掛品、原材料、貯蔵品）の内訳書\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"品目１\",\"品目２\",\"数量\",\"単位\",\"単価\",\"期末現在高\",\"摘要１\",\"摘要２\"");
// 24/02/19_ScoreLink add -->
				//cs4=_T("\"[欄外]\",\"期末棚卸の方法\",\"登録名称\",\"棚卸を行った時期\"");
// 24/02/19_ScoreLink add <--
				break;
			case	6:
				cs1=_T("\"帳票名\",\"有価証券の内訳書\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"区分\",\"種類\",\"銘柄\",\"期末数量\",\"（単位）\",\"期末現在高（時価評価前）\",\"期末現在高（時価評価後）\",\"異動年月日\",\"異動事由１\",\"異動事由２\",\"異動数量\",\"（単位）\",\"期末増（減）額\",\"売却（買入）先の名称（氏名）１\",\"売却（買入）先の名称（氏名）２\",\"売却（買入）先の所在地（住所）１\",\"売却（買入）先の所在地（住所）２\",\"摘要１\",\"摘要２\",\"摘要３\"");
				break;
			case	7:
				cs1=_T("\"帳票名\",\"固定資産（土地、土地の上に存する権利及び建物に限る。）の内訳書\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"種類・構造１\",\"種類・構造２\",\"用途１\",\"用途２\",\"面積\",\"物件の所在地１\",\"物件の所在地２\",\"期末現在高\",\"異動年月日\",\"異動事由１\",\"異動事由２\",\"取得（処分）価額\",\"異動直前の帳簿価額\",\"登録番号（法人番号）\",\"売却（購入）先の名称（氏名）１\",\"売却（購入）先の名称（氏名）２\",\"売却（購入）先の所在地（住所）１\",\"売却（購入）先の所在地（住所）２\",\"売却物件の取得年月\"");
				break;
			case	8:
				cs1=_T("\"帳票名\",\"支払手形の内訳書\"");
				cs2=_T("\"会社名\",\"\"");
// 24/05/31_ScoreLink cor -->
				//cs3=_T("\"[項目]\",\"登録番号（法人番号）\",\"支払先１\",\"支払先２\",\"振出年月日\",\"支払期日\",\"支払銀行名称\",\"支払銀行支店名\",\"金額\",\"摘要１\",\"摘要２\"");
// --------------------------
				cs3=_T("\"[項目]\",\"科目\",\"登録番号（法人番号）\",\"支払先１\",\"支払先２\",\"振出年月日\",\"支払期日\",\"支払銀行名称\",\"支払銀行支店名\",\"金額\",\"摘要１\",\"摘要２\"");
// 24/05/31_ScoreLink cor <--
				break;
			case	9:
				cs1=_T("\"帳票名\",\"買掛金（未払金・未払費用）の内訳書\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"登録番号（法人番号）\",\"相手先名称（氏名）１\",\"相手先名称（氏名）２\",\"相手先所在地（住所）１\",\"相手先所在地（住所）２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				cs4=_T("\"[欄外]\",\"支払確定年月日\",\"期末現在高\"");
				break;
			case	10:
				cs1=_T("\"帳票名\",\"仮受金（前受金・預り金）の内訳書\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"登録番号（法人番号）\",\"相手先名称（氏名）１\",\"相手先名称（氏名）２\",\"相手先所在地（住所）１\",\"相手先所在地（住所）２\",\"法人・代表者との関係\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	11:
				cs1=_T("\"帳票名\",\"源泉所得税預り金の内訳\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"支払年月\",\"所得の種類\",\"期末現在高\"");
				break;
			case	12:
				cs1=_T("\"帳票名\",\"借入金及び支払利子の内訳書\"");
				cs2=_T("\"会社名\",\"\"");
// 24/02/19_ScoreLink cor -->
				//cs3=_T("\"[項目]\",\"科目\",\"借入先名称（氏名）１\",\"借入先名称（氏名）２\",\"借入先所在地（住所）１\",\"借入先所在地（住所）２\",\"法人・代表者との関係\",\"期末現在高\",\"期中の支払利子額\",\"利率\",\"担保の内容（物件の種類、数量、所在地等）１\",\"担保の内容（物件の種類、数量、所在地等）２\"");
// --------------------------
				cs3=_T("\"[項目]\",\"科目\",\"借入先名称（氏名）１\",\"借入先名称（氏名）２\",\"借入先所在地（住所）１\",\"借入先所在地（住所）２\",\"法人・代表者との関係\",\"期末現在高\",\"期中の支払利子額\",\"利率\",\"借入理由１\",\"借入理由２\",\"担保の内容（物件の種類、数量、所在地等）１\",\"担保の内容（物件の種類、数量、所在地等）２\"");
// 24/02/19_ScoreLink cor <--
				break;
			case	13:
				cs1=_T("\"帳票名\",\"土地の売上高等の内訳書\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"区分\",\"商品の所在地１\",\"商品の所在地２\",\"地目\",\"総面積\",\"売上（仲介）年月\",\"登録番号（法人番号）\",\"売上（仲介）先名称（氏名）\",\"売上（仲介）先所在地（住所）１\",\"売上（仲介）先所在地（住所）２\",\"売上（仲介）面積\",\"売上金額\",\"仲介手数料\",\"売上商品の取得年\"");
				break;
			case	14:
				cs1=_T("\"帳票名\",\"売上高等の事業所別内訳書\"");
				cs2=_T("\"会社名\",\"\"");
// 24/02/19_ScoreLink cor -->
				//cs3=_T("\"[項目]\",\"事業所の名称\",\"所在地１\",\"所在地２\",\"責任者氏名１\",\"責任者氏名２\",\"代表者との関係\",\"事業等の内容１\",\"事業等の内容２\",\"事業等の内容３\",\"売上高\",\"期末棚卸高\",\"期末従事員数\",\"源泉所得税納付署１\",\"源泉所得税納付署２\",\"摘要１\",\"摘要２\",\"摘要３\"");
// --------------------------
				cs3=_T("\"[項目]\",\"事業所の名称\",\"所在地１\",\"所在地２\",\"責任者氏名１\",\"責任者氏名２\",\"代表者との関係\",\"事業等の内容１\",\"事業等の内容２\",\"事業等の内容３\",\"売上高\",\"期末棚卸高\",\"期末従事員数\",\"使用建物の延面積\",\"源泉所得税納付署１\",\"源泉所得税納付署２\",\"摘要１\",\"摘要２\",\"摘要３\"");
// 24/02/19_ScoreLink cor <--
				break;
			case	15:
				cs1=_T("\"帳票名\",\"役員給与等の内訳書\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"役職名\",\"担当業務\",\"氏名１\",\"氏名２\",\"住所１\",\"住所２\",\"代表者との関係\",\"常勤・非常勤の別\",\"使用人職務分\",\"定期同額給与\",\"事前確定届出給与\",\"業績連動給与\",\"その他\",\"退職給与\",\"役員給与計\"");
				break;
			case	16:
				cs1=_T("\"帳票名\",\"人件費の内訳書\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"区分\",\"総額\",\"総額のうち代表者及びその家族分\"");
				break;
			case	17:
				cs1=_T("\"帳票名\",\"地代家賃等の内訳書\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"地代・家賃の区分\",\"借地（借家）物件の用途１\",\"借地（借家）物件の用途２\",\"所在地１\",\"所在地２\",\"登録番号（法人番号）\",\"貸主の名称（氏名）\",\"貸主の所在地（住所）１\",\"貸主の所在地（住所）２\",\"支払対象期間（開始）\",\"支払対象期間（終了）\",\"支払賃借料\",\"摘要１\",\"摘要２\",\"摘要３\"");
				break;
			case	18:
				cs1=_T("\"帳票名\",\"権利金等の期中支払の内訳書\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"登録番号（法人番号）\",\"支払先の名称（氏名）\",\"支払先の所在地（住所）１\",\"支払先の所在地（住所）２\",\"支払年月日\",\"支払金額\",\"権利金等の内容１\",\"権利金等の内容２\",\"摘要１\",\"摘要２\",\"摘要３\"");
				break;
			case	19:
				cs1=_T("\"帳票名\",\"工業所有権等の使用料の内訳書\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"名称１\",\"名称２\",\"登録番号（法人番号）\",\"支払先の名称（氏名）\",\"支払先の所在地（住所）１\",\"支払先の所在地（住所）２\",\"契約期間（開始）\",\"契約期間（終了）\",\"支払対象期間（開始）\",\"支払対象期間（終了）\",\"支払金額\",\"摘要１\",\"摘要２\",\"摘要３\"");
				break;
			case	20:
				cs1=_T("\"帳票名\",\"雑益、雑損失等の内訳書\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"取引の内容１\",\"取引の内容２\",\"登録番号（法人番号）\",\"相手先名称（氏名）１\",\"相手先名称（氏名）２\",\"相手先所在地（住所）１\",\"相手先所在地（住所）２\",\"金額\"");
				break;
			case	21:
				cs1=_T("\"帳票名\",\"雑損失等の内訳書\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"取引の内容１\",\"取引の内容２\",\"登録番号（法人番号）\",\"相手先名称（氏名）１\",\"相手先名称（氏名）２\",\"相手先所在地（住所）１\",\"相手先所在地（住所）２\",\"金額\"");
				break;
			case	22:
				cs1=_T("\"帳票名\",\"その他科目の内訳書①－１\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"登録番号（法人番号）\",\"名称（氏名）１\",\"名称（氏名）２\",\"所在地（住所）１\",\"所在地（住所）２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	23:
				cs1=_T("\"帳票名\",\"その他科目の内訳書①－２\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"登録番号（法人番号）\",\"名称（氏名）１\",\"名称（氏名）２\",\"所在地（住所）１\",\"所在地（住所）２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	24:
				cs1=_T("\"帳票名\",\"その他科目の内訳書①－３\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"登録番号（法人番号）\",\"名称（氏名）１\",\"名称（氏名）２\",\"所在地（住所）１\",\"所在地（住所）２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	25:
				cs1=_T("\"帳票名\",\"その他科目の内訳書①－４\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"登録番号（法人番号）\",\"名称（氏名）１\",\"名称（氏名）２\",\"所在地（住所）１\",\"所在地（住所）２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	26:
				cs1=_T("\"帳票名\",\"その他科目の内訳書①－５\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"登録番号（法人番号）\",\"名称（氏名）１\",\"名称（氏名）２\",\"所在地（住所）１\",\"所在地（住所）２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	27:
				cs1=_T("\"帳票名\",\"その他科目の内訳書①－６\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"登録番号（法人番号）\",\"名称（氏名）１\",\"名称（氏名）２\",\"所在地（住所）１\",\"所在地（住所）２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	28:
				cs1=_T("\"帳票名\",\"その他科目の内訳書①－７\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"登録番号（法人番号）\",\"名称（氏名）１\",\"名称（氏名）２\",\"所在地（住所）１\",\"所在地（住所）２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	29:
				cs1=_T("\"帳票名\",\"その他科目の内訳書①－８\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"登録番号（法人番号）\",\"名称（氏名）１\",\"名称（氏名）２\",\"所在地（住所）１\",\"所在地（住所）２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	30:
				cs1=_T("\"帳票名\",\"その他科目の内訳書①－９\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"登録番号（法人番号）\",\"名称（氏名）１\",\"名称（氏名）２\",\"所在地（住所）１\",\"所在地（住所）２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	31:
				cs1=_T("\"帳票名\",\"その他科目の内訳書①－１０\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"登録番号（法人番号）\",\"名称（氏名）１\",\"名称（氏名）２\",\"所在地（住所）１\",\"所在地（住所）２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	32:
				cs1=_T("\"帳票名\",\"その他科目の内訳書①－１１\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"登録番号（法人番号）\",\"名称（氏名）１\",\"名称（氏名）２\",\"所在地（住所）１\",\"所在地（住所）２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	33:
				cs1=_T("\"帳票名\",\"その他科目の内訳書①－１２\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"登録番号（法人番号）\",\"名称（氏名）１\",\"名称（氏名）２\",\"所在地（住所）１\",\"所在地（住所）２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	34:
				cs1=_T("\"帳票名\",\"その他科目の内訳書①－１３\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"登録番号（法人番号）\",\"名称（氏名）１\",\"名称（氏名）２\",\"所在地（住所）１\",\"所在地（住所）２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	35:
				cs1=_T("\"帳票名\",\"その他科目の内訳書①－１４\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"登録番号（法人番号）\",\"名称（氏名）１\",\"名称（氏名）２\",\"所在地（住所）１\",\"所在地（住所）２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	36:
				cs1=_T("\"帳票名\",\"その他科目の内訳書①－１５\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"登録番号（法人番号）\",\"名称（氏名）１\",\"名称（氏名）２\",\"所在地（住所）１\",\"所在地（住所）２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	37:
				cs1=_T("\"帳票名\",\"その他科目の内訳書①－１６\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"登録番号（法人番号）\",\"名称（氏名）１\",\"名称（氏名）２\",\"所在地（住所）１\",\"所在地（住所）２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	38:
				cs1=_T("\"帳票名\",\"その他科目の内訳書①－１７\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"登録番号（法人番号）\",\"名称（氏名）１\",\"名称（氏名）２\",\"所在地（住所）１\",\"所在地（住所）２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	39:
				cs1=_T("\"帳票名\",\"その他科目の内訳書①－１８\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"登録番号（法人番号）\",\"名称（氏名）１\",\"名称（氏名）２\",\"所在地（住所）１\",\"所在地（住所）２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	40:
				cs1=_T("\"帳票名\",\"その他科目の内訳書①－１９\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"登録番号（法人番号）\",\"名称（氏名）１\",\"名称（氏名）２\",\"所在地（住所）１\",\"所在地（住所）２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	41:
				cs1=_T("\"帳票名\",\"その他科目の内訳書①－２０\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"登録番号（法人番号）\",\"名称（氏名）１\",\"名称（氏名）２\",\"所在地（住所）１\",\"所在地（住所）２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	42:
				cs1=_T("\"帳票名\",\"その他科目の内訳書②－１\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"項目１\",\"項目２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	43:
				cs1=_T("\"帳票名\",\"その他科目の内訳書②－２\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"項目１\",\"項目２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	44:
				cs1=_T("\"帳票名\",\"その他科目の内訳書②－３\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"項目１\",\"項目２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	45:
				cs1=_T("\"帳票名\",\"その他科目の内訳書②－４\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"項目１\",\"項目２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	46:
				cs1=_T("\"帳票名\",\"その他科目の内訳書②－５\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"項目１\",\"項目２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	47:
				cs1=_T("\"帳票名\",\"その他科目の内訳書②－６\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"項目１\",\"項目２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	48:
				cs1=_T("\"帳票名\",\"その他科目の内訳書②－７\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"項目１\",\"項目２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	49:
				cs1=_T("\"帳票名\",\"その他科目の内訳書②－８\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"項目１\",\"項目２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	50:
				cs1=_T("\"帳票名\",\"その他科目の内訳書②－９\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"項目１\",\"項目２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	51:
				cs1=_T("\"帳票名\",\"その他科目の内訳書②－１０\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"項目１\",\"項目２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	52:
				cs1=_T("\"帳票名\",\"その他科目の内訳書②－１１\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"項目１\",\"項目２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	53:
				cs1=_T("\"帳票名\",\"その他科目の内訳書②－１２\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"項目１\",\"項目２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	54:
				cs1=_T("\"帳票名\",\"その他科目の内訳書②－１３\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"項目１\",\"項目２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	55:
				cs1=_T("\"帳票名\",\"その他科目の内訳書②－１４\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"項目１\",\"項目２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	56:
				cs1=_T("\"帳票名\",\"その他科目の内訳書②－１５\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"項目１\",\"項目２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	57:
				cs1=_T("\"帳票名\",\"その他科目の内訳書②－１６\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"項目１\",\"項目２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	58:
				cs1=_T("\"帳票名\",\"その他科目の内訳書②－１７\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"項目１\",\"項目２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	59:
				cs1=_T("\"帳票名\",\"その他科目の内訳書②－１８\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"項目１\",\"項目２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	60:
				cs1=_T("\"帳票名\",\"その他科目の内訳書②－１９\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"項目１\",\"項目２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
			case	61:
				cs1=_T("\"帳票名\",\"その他科目の内訳書②－２０\"");
				cs2=_T("\"会社名\",\"\"");
				cs3=_T("\"[項目]\",\"科目\",\"項目１\",\"項目２\",\"期末現在高\",\"摘要１\",\"摘要２\"");
				break;
		}
		cs1=cs1+_T("\n");
		cs2=cs2+_T("\n");
		cs3=cs3+_T("\n");
		file.write(cs1,cs1.GetLength());
		file.write(cs2,cs2.GetLength());
		file.write(cs3,cs3.GetLength());
		if(cs4.IsEmpty() == FALSE)	{
			cs4=cs4+_T("\n");
			file.write(cs4,cs4.GetLength());
		}
        file.close();
	}

	return(TRUE);
}
void SL_GetErrorMsg( CString& pErr )
{
	pErr = SL_Err;
}
// 24/02/09_ScoreLink add <--
