// *****************************************************************************************
// BatchPrt.cpp
// 一括印刷処理 実装クラス
//
//
//
// *****************************************************************************************
#include "StdAfx.h"
#include "BatchPrt.h"

#include "PrintSch8.h"	// 2010/08/25 税務用年月日セット対応

#ifdef _DEBUG
 #if 0
  #define		_UCOM_DBG		// ★TODO：デバッグ用 削除
 #endif
#endif//_DEBUG
//--------------------------------------------------------
// コンストラクタ
//
//--------------------------------------------------------
CBatchPrt::CBatchPrt( void )
{
	// コモン情報(UCOM) 初期化
	InitBatchPrgInf();

	m_fErrorEnd			= FALSE;	// TRUE=異常終了／FALSE正常終了  2010/08/02 一括印刷 異常終了対応 
	m_uiWmExitMsg		= 0x00;		// 終了通知メッセージ					2010/08/10 一括印刷 ログオフ時 UCOM I/F 返却なし対応
	m_wpWmExitParam		= 0x00;		// 終了通知メッセージ 受信時 パラメータ	2010/08/10 一括印刷 ログオフ時 UCOM I/F 返却なし対応
	m_fPrtMdlErr		= FALSE;	// 印刷モジュール異常終了処理中フラグ   2010/08/31 CVPrint::ErrorAbort() 実行後の一括印刷終了対応

	// 2011/02/08 修正依頼No.N-040 一括印刷情報マスタ単位昇格（運用中マスタ対応）
	//m_fDoSetMode		= FALSE;	// 詳細設定モード実行誘導フラグ  TRUE=誘導が必要(一括印刷 出力不可) // 2011/01/11 修正依頼No.N-037 一括印刷ユーザ情報共通化
	m_nDoSetMode		= OWNTB_CHKREC_NG_DOSET01;	// 詳細設定モード実行誘導フラグ  0以外=誘導が必要(一括印刷 出力不可) 2011/02/08
}


CBatchPrt::~CBatchPrt(void)
{
	// コモン情報(UCOM) 初期化
	InitBatchPrgInf();
}

//--------------------------------------------------------
// コモン情報(UCOM) 初期化
//	IN:		なし
//	RET:	なし
//--------------------------------------------------------
void	CBatchPrt::InitBatchPrgInf(void)
{
	m_nBatchMode = ID_BATCHMODE_NONE;		// 一括印刷起動モード 初期化
	
	// UCOM 情報 初期化
	m_udUcomInf.strCallPrg		= "";	// 呼出プログラム 名 
	m_udUcomInf.strBatchPrg		= "";	// 依頼プログラム 名
	m_udUcomInf.strBatchMode	= "";	// 起動モード（ 印刷、設定、CDR出力 ）
	m_udUcomInf.strBatchKey		= "";	// 依頼キー 

	m_udUcomInf.strPrtCnt		= "";	// 依頼詳細 印刷部数
	m_udUcomInf.strPrtName		= "";	// 依頼詳細 プリンタ名
	m_udUcomInf.strPrtYmdS		= "";	// 設定・出力年月日 開始（NDS担当アプリ不要）
	m_udUcomInf.strPrtYmdE		= "";	// 設定・出力年月日 終了（NDS担当アプリ不要）
	m_udUcomInf.strPrtOffSetS	= "";	// 設定・出力月オフセット 開始
	m_udUcomInf.strPrtOffSetE	= "";	// 設定・出力月オフセット 終了
// 24/01/16_ScoreLink add -->
	m_udUcomInf.strSlIraiId		= "";	// ScoreLink 依頼ID
	m_udUcomInf.strSlPdfName	= "";	// ScoreLink 出力PDFファイル名
// 24/01/16_ScoreLink add <--

	//m_udUcomInf.strCdrPath		= "";	// 出力パス	  PDF出力先 パス文字列	2010/07/31 削除（仕様変更 7/30）
}


//--------------------------------------------------------
// 一括印刷 コモン情報(UCOM) 印刷・設定共通情報 取得 
//	IN:		なし
//	RET:	TRUE 正常終了
//--------------------------------------------------------
BOOL	CBatchPrt::GetUcomInf( void )
{
	const CString	FN = _T("CBatchPrt::GetUcomInf()");
	char	szBuf[256];

	// コモン情報(UCOM) 初期化
	InitBatchPrgInf();

	//------------------------------
	// “呼出プログラム”
	//------------------------------

// 24/01/16_ScoreLink add -->
//#ifdef _DEBUG
//UCOM_SetString(UCOMKEY_CALLPRG,_T("一括印刷"));
//UCOM_SetString(UCOMKEY_BATCHPRG,_T("勘定科目内訳書"),_T("0"));
////UCOM_SetString(UCOMKEY_BATCHMODE,_T("詳細設定"));
//UCOM_SetString(UCOMKEY_BATCHMODE,_T("印刷"));
////UCOM_SetString(UCOMKEY_BATCHKEY,_T("DBUchiwake1b0001SL"));
//UCOM_SetString(UCOMKEY_BATCHKEY,_T("DBUchiwake1b0001"));
//UCOM_SetString(UCOMKEY_PRTINFO,_T("1"),_T("Antenna house PDF Driver 7.5"));
//UCOM_SetString(UCOMKEY_PRTYMD,_T("-1"),_T("-1"),_T("-1"),_T("-1"));
////UCOM_SetString(_T("SCORELINK依頼ID"),_T("0"));
////UCOM_SetString(_T("SCORELINK出力PDF名称"),_T("勘定科目内訳書"));
////UCOM_SetString(_T("SCORELINK接続マスター決算期間"),_T("20240101"),_T("20241231"));
//#endif
	memset(szBuf,0,sizeof(szBuf));
	UCOM_GetString(_T("SCORELINK依頼ID"),szBuf);
	m_udUcomInf.strSlIraiId = szBuf;
	memset(szBuf,0,sizeof(szBuf));
	UCOM_GetString(_T("SCORELINK出力PDF名称"),szBuf);
	m_udUcomInf.strSlPdfName = szBuf;
// 24/01/16_ScoreLink add <--

	memset( szBuf, 0x00, sizeof(szBuf));
	UCOM_GetString( UCOMKEY_CALLPRG, szBuf );
	m_udUcomInf.strCallPrg = szBuf;
#ifdef _UCOM_DBG
	m_udUcomInf.strCallPrg = _T("UCOM パラメータデバッグ用");
	m_udUcomInf.strCallPrg = _T("一括印刷");
#endif
	// ログ
	DbgWriteLog( FN, _T( "UCOM I/F <GET> 呼出プログラム [") + m_udUcomInf.strCallPrg + _T("]") );

	//------------------------------
	// 一括印刷 アプリからの起動？		
	//------------------------------
	if ( UCOMITM_BATCHMODE == m_udUcomInf.strCallPrg ) {
		// 処理継続
		DbgWriteLog( FN, _T( "UCOM I/F 起動モード：一括印刷アプリからの起動") );

	} else {
		// 通常起動（業務メニュー or 一括印刷以外のフォーク起動）
		m_nBatchMode = ID_BATCHMODE_NONE;
		m_udUcomInf.strCallPrg = "";

		DbgWriteLog( FN, _T( "UCOM I/F 起動モード：通常起動（業務メニュー or 一括印刷以外のフォーク起動") );
		// 終了
		return TRUE;
	}

	// 一括印刷 アプリからの起動時 UCOM 情報取得		
	//------------------------------
	// “依頼プログラム”
	//------------------------------
	memset( szBuf, 0x00, sizeof(szBuf));
	UCOM_GetString( UCOMKEY_BATCHPRG, szBuf );
	m_udUcomInf.strBatchPrg = szBuf;
#ifdef _UCOM_DBG
	m_udUcomInf.strBatchPrg = _T("勘定科目内訳書");
#endif
	// ログ
	DbgWriteLog( FN, _T( "UCOM I/F <GET> 依頼プログラム  [") + m_udUcomInf.strBatchPrg + _T("]") );


	//------------------------------
	// “依頼項目”
	//------------------------------
	memset( szBuf, 0x00, sizeof(szBuf));
	UCOM_GetString( UCOMKEY_BATCHMODE, szBuf );
	m_udUcomInf.strBatchMode = szBuf;

#ifdef _UCOM_DBG
	switch ( AfxMessageBox( _T("一括印刷デバッグ用起動：\n\n  印刷モード＝「はい」／設定モード＝「いいえ」／CDRモード＝「キャンセル」"), MB_YESNOCANCEL)) {
	case IDYES:	m_udUcomInf.strBatchMode = _T("印刷");		break;
	case IDNO:	m_udUcomInf.strBatchMode = _T("詳細設定");	break;
	default:	m_udUcomInf.strBatchMode = _T("CDR出力");	break;
	}	
#endif

	// 内部管理 一括印刷モードセット
	if (	  UCOMITM_BATCHPRT == m_udUcomInf.strBatchMode )	{ m_nBatchMode = ID_BATCHMODE_PRT; }	// 印刷
	else if ( UCOMITM_BATCHSET == m_udUcomInf.strBatchMode )	{ m_nBatchMode = ID_BATCHMODE_SET; }	// 設定
	else if ( UCOMITM_BATCHCDR == m_udUcomInf.strBatchMode )	{ m_nBatchMode = ID_BATCHMODE_CDR; }	// CDR出力
	else {
		// あり得ない パターン
		DbgWriteLog( FN, _T( "UCOM I/F  <GET> 依頼項目 不正 [") + m_udUcomInf.strBatchMode + _T("]") );
		m_nBatchMode = ID_BATCHMODE_NONE;
		return FALSE;
	}
	// ログ
	DbgWriteLog( FN, _T( "UCOM I/F <GET> 依頼項目	       [") + m_udUcomInf.strBatchMode + _T("]") );

	//------------------------------
	// “依頼キー”
	//------------------------------
	memset( szBuf, 0x00, sizeof(szBuf));
	UCOM_GetString( UCOMKEY_BATCHKEY, szBuf );
	m_udUcomInf.strBatchKey = szBuf;

#ifdef _UCOM_DBG
	m_udUcomInf.strBatchKey = _T("DBUchiwake1b0001");
#endif

	// UCOM 情報 デバッグ出力
	DbgWriteLog( FN, _T( "UCOM I/F <GET> 依頼キー        [") + m_udUcomInf.strBatchKey + _T("]") );

	//------------------------------
	// パラメータチェック（依頼キー）
	//------------------------------
	if ( "" == m_udUcomInf.strBatchKey ) {
		DbgWriteLog( FN, _T( "UCOM I/F 依頼キー 不正  → エラー終了") );
		return FALSE;
	}

	//---------------------------------------------
	// 各 起動モード（依頼項目）により情報を取得
	//---------------------------------------------
	switch ( m_nBatchMode ) {
	case ID_BATCHMODE_PRT:		// 印刷
		GetUcomInfPrt();
		break;
	case ID_BATCHMODE_SET:		// 設定
		GetUcomInfSet();
		break;
	case ID_BATCHMODE_CDR:		// CDR出力
		GetUcomInfCDR();
		break;
	}

#ifdef _UCOM_DBG
	m_udUcomInf.strPrtCnt		= _T("1");
	m_udUcomInf.strPrtName		= _T("FinePrint");
	m_udUcomInf.strPrtYmdS		= _T("20100401");
	m_udUcomInf.strPrtYmdE		= _T("20100430");
	m_udUcomInf.strPrtOffSetS	= _T("0");
	m_udUcomInf.strPrtOffSetE	= _T("0");
#endif

	// ログ
	DbgWriteLog( FN, _T( "UCOM I/F <GET> 依頼詳細 印刷部数    [") + m_udUcomInf.strPrtCnt + _T("]") );
	DbgWriteLog( FN, _T( "UCOM I/F <GET> 依頼詳細 プリンタ名  [") + m_udUcomInf.strPrtName + _T("]") );

	DbgWriteLog( FN, _T( "UCOM I/F <GET> 設定・出力年月日 開始  [") + m_udUcomInf.strPrtYmdS + _T("]") );
	DbgWriteLog( FN, _T( "UCOM I/F <GET> 設定・出力年月日 終了  [") + m_udUcomInf.strPrtYmdE + _T("]") );
	DbgWriteLog( FN, _T( "UCOM I/F <GET> 設定・出力月オフセット 開始  [") + m_udUcomInf.strPrtOffSetS + _T("]") );
	DbgWriteLog( FN, _T( "UCOM I/F <GET> 設定・出力月オフセット 終了  [") + m_udUcomInf.strPrtOffSetE + _T("]") );

	return TRUE;
}// GetUcomInfCom

//--------------------------------------------------------
// 設定モード  起動時 コモン情報(UCOM) 取得 
//	IN:		なし
//	RET:	TRUE 正常終了
//--------------------------------------------------------
BOOL	CBatchPrt::GetUcomInfSet( void )
{
	const CString	FN = _T("CBatchPrt::GetUcomInfSet()");

	//------------------------------
	// “設定年月日”
	//------------------------------
	BOOL fRet = GetUcomInfYMD( UCOMKEY_SETYMD );
	return fRet;
}

//--------------------------------------------------------
// 出力モード  起動時 コモン情報(UCOM) 取得 
//	IN:		なし
//	RET:	TRUE 正常終了
//--------------------------------------------------------
BOOL	CBatchPrt::GetUcomInfPrt( void )
{
	char	szBuf[2][256];

	//------------------------------
	// 出力年月日 取得
	//------------------------------
	GetUcomInfYMD( UCOMKEY_PRTYMD );

	//------------------------------
	// “依頼詳細”
	//------------------------------
	memset( szBuf, 0x00, sizeof(szBuf));
	UCOM_GetString( UCOMKEY_PRTINFO, szBuf[0], szBuf[1] );
	m_udUcomInf.strPrtCnt		= szBuf[0];		// 依頼詳細 印刷部数
	m_udUcomInf.strPrtName		= szBuf[1];		// 依頼詳細 プリンタ名

	return TRUE;
}

//--------------------------------------------------------
// CDR出力モード  起動時 コモン情報(UCOM) 取得 
//	IN:		なし
//	RET:	TRUE 正常終了
//--------------------------------------------------------
BOOL	CBatchPrt::GetUcomInfCDR( void )
{
	// 2010/08/15 修正依頼対応 N-001
	return GetUcomInfPrt();
}

//--------------------------------------------------------
// 一括印刷 コモン情報(UCOM) 情報 “設定年月日”or “出力年月日” 取得 
//	IN:		なし
//	RET:	TRUE 正常終了
//--------------------------------------------------------
BOOL	CBatchPrt::GetUcomInfYMD( CString strKey )
{
	char	szBuf[4][256];

	//------------------------------
	// “設定年月日”or “出力年月日”
	//------------------------------
	memset( szBuf, 0x00, sizeof(szBuf));
	UCOM_GetString( strKey, szBuf[0], szBuf[1], szBuf[2], szBuf[3] );
	m_udUcomInf.strPrtYmdS		= szBuf[0];		// 設定・出力年月日 開始年月日（NDS担当アプリ不要）
	m_udUcomInf.strPrtYmdE		= szBuf[1];		// 設定・出力年月日 終了年月日（NDS担当アプリ不要）
	m_udUcomInf.strPrtOffSetS	= szBuf[2];		// 設定・出力年月日 開始月オフセット
	m_udUcomInf.strPrtOffSetE	= szBuf[3];		// 設定・出力年月日 終了月オフセット

	return TRUE;
}

//--------------------------------------------------------
// 設定結果 返却  コモン情報(UCOM)
//	IN:		BATCHPRTUCOMINF_RETSET		設定結果返却情報
//	RET:	TRUE 正常終了
//--------------------------------------------------------
BOOL	CBatchPrt::SetUcomInfSet( BATCHPRTUCOMINF_RETSET *pRetSetInf )
{
	char	szBuf[5][256];
	CString	strKey = "";
	const CString	FN = _T("CBatchPrt::SetUcomInfSet()");

	// パラメータチェック
	if ( !pRetSetInf ) {
		DbgWriteLog( FN, _T("パラメータ異常 END") );
		return FALSE;
	}

	// UCOM 情報 デバッグ出力
	CString		strLog;
	//DbgWriteLog( FN, _T( "UCOM I/F 依頼キー             [") + pRetSetInf->strBatchKey + _T("]") );
	//DbgWriteLog( FN, _T( "UCOM I/F 用紙サイズ  　　　　 [") + pRetSetInf->strPrtSize + _T("]") );
	strLog.Format( _T( "UCOM I/F <param> 用紙サイズ(AP管理ID)         [%d]"), pRetSetInf->nPrtSize );
	DbgWriteLog( FN, strLog );
	strLog.Format( _T( "UCOM I/F <param> 設定・出力年月日 開始        [%d]"), pRetSetInf->nPrtYmdS );
	DbgWriteLog( FN, strLog );
	strLog.Format( _T( "UCOM I/F <param> 設定・出力年月日 終了        [%d]"), pRetSetInf->nPrtYmdE );
	DbgWriteLog( FN, strLog );
	strLog.Format( _T( "UCOM I/F <param> 設定・出力月オフセット 開始  [%d]"), pRetSetInf->nPrtOffSetS );
	DbgWriteLog( FN, strLog );
	strLog.Format( _T( "UCOM I/F <param> 設定・出力月オフセット 終了  [%d]"), pRetSetInf->nPrtOffSetE );
	DbgWriteLog( FN, strLog );
	strLog.Format( _T( "UCOM I/F <param> 出力年月日情報の日まで必要?  [%d]"), pRetSetInf->nFlgYmd );
	// 2010/07/31 追加 仕様変更（7/30）
	DbgWriteLog( FN, strLog );
	strLog.Format( _T( "UCOM I/F <param> PDFファイル名                [%s]"), pRetSetInf->strPdfName );

	//------------------------------
	// “用紙サイズ”
	//------------------------------
	sprintf_s( szBuf[0], _T("%s"), CnvPrtSize_Ap2Ucom( pRetSetInf->nPrtSize ));	// 用紙サイズ変換
	sprintf_s( szBuf[1], _T("%s"), CnvPrtVH_Ap2Ucom( pRetSetInf->nPrtSize ));	// 用紙方向変換

	strLog.Format( _T("UCOM I/F <SET> 用紙サイズ[%s], 印刷方向[%s] (UCOM管理ID)"), szBuf[0], szBuf[1] );
	DbgWriteLog( FN, strLog );

	// 書き込み
	UCOM_SetString_BC( UCOMKEY_PRTSIZE, szBuf[0], szBuf[1] );

	//------------------------------
	// “依頼キー”
	//------------------------------
	// 依頼キー取得
	strKey = GetBatchKey();
	memset( szBuf, 0x00, sizeof(szBuf));
	strcpy_s( szBuf[0], strKey);

	strLog.Format( _T( "UCOM I/F <SET> 依頼キー             [%s]"), szBuf[0] );
	DbgWriteLog( FN, strLog );

	// 書き込み
	UCOM_SetString_BC( UCOMKEY_BATCHKEY, szBuf[0] );

	//------------------------------
	// “設定年月日”
	//------------------------------
	memset( szBuf, 0x00, sizeof(szBuf));
	if ( 0 == pRetSetInf->nPrtYmdS )		{ pRetSetInf->nPrtYmdS = -1; }		// 年月日 は不要をセット
	if ( 0 == pRetSetInf->nPrtYmdE )		{ pRetSetInf->nPrtYmdE = -1; }		// 年月日 は不要をセット
	sprintf_s( szBuf[0], _T("%d"), pRetSetInf->nPrtYmdS );			// 設定・出力年月日 開始
	sprintf_s( szBuf[1], _T("%d"), pRetSetInf->nPrtYmdE );			// 設定・出力年月日 終了
	sprintf_s( szBuf[2], _T("%d"), pRetSetInf->nPrtOffSetS );		// 設定・出力月オフセット 開始
	sprintf_s( szBuf[3], _T("%d"), pRetSetInf->nPrtOffSetE );		// 設定・出力月オフセット 終了
	sprintf_s( szBuf[4], _T("%d"), pRetSetInf->nFlgYmd );			// 設定・出力年月日情報に 日 まで必要か？（1=必要／0=不要））

	// 書き込み
	UCOM_SetString_BC( UCOMKEY_SETYMD, szBuf[0], szBuf[1], szBuf[2], szBuf[3], szBuf[4] );

	strLog.Format( _T( "UCOM I/F <SET> 設定・出力年月日 開始        [%s]"), szBuf[0] );
	DbgWriteLog( FN, strLog );
	strLog.Format( _T( "UCOM I/F <SET> 設定・出力年月日 終了        [%s]"), szBuf[1] );
	DbgWriteLog( FN, strLog );
	strLog.Format( _T( "UCOM I/F <SET> 設定・出力月オフセット 開始  [%s]"), szBuf[2] );
	DbgWriteLog( FN, strLog );
	strLog.Format( _T( "UCOM I/F <SET> 設定・出力月オフセット 終了  [%s]"), szBuf[3] );
	DbgWriteLog( FN, strLog );
	strLog.Format( _T( "UCOM I/F <SET> 出力年月日情報の日まで必要?  [%s]"), szBuf[4] );
	DbgWriteLog( FN, strLog );

	// 2010/07/31 追加 仕様変更（7/30）
	//------------------------------
	// “詳細設定”
	//------------------------------
	strcpy_s( szBuf[0], sizeof( szBuf[0] ), pRetSetInf->strPdfName ); // PDFファイル名

	// 書き込み
	UCOM_SetString_BC( UCOMKEY_SETDETAIL, szBuf[0] );

	strLog.Format( _T( "UCOM I/F <SET> PDFファイル名                [%s]"), szBuf[0] );
	DbgWriteLog( FN, strLog );

	return TRUE;
}

//--------------------------------------------------------
// UCOM用 用紙サイズ 変換
//
// アプリ内部管理 用紙サイズNo（コンボ選択結果）から、一括印UCOM情報に変換
//
//	IN:		int			用紙サイズNo（コンボ選択結果）
//	RET:	CString		一括印UCOM返却用文字列 用紙サイズ
//--------------------------------------------------------
CString	CBatchPrt::CnvPrtSize_Ap2Ucom( int nPinf )
{
	const CString	FN = _T("CBatchPrt::CnvPrtSize_Ap2Ucom()");

	// 用紙サイズコンボ の選択結果から、UCOM情報に変換
	CString		strRet;
	CString		strLog;

	switch ( nPinf ) {
	case	ID_PRTSIZE_A4T:	strRet = UCOMITM_BATCHPRT_PAPERA4;	break;		// 0:A4縦

	default:
		strLog.Format( _T("%d"), nPinf);
		DbgWriteLog( FN, _T( "UCOM I/F AP管理用紙サイズ指定値 異常 [") + strLog + _T("] → A4縦(初期値)として処理継続") );

		strRet = UCOMITM_BATCHPRT_PAPERA4;		// 0:A4縦
		break;
	}

	return strRet;
}

//--------------------------------------------------------
// UCOM用 用紙方向 変換
//
// アプリ内部管理 用紙サイズNo（コンボ選択結果）から、一括印UCOM情報に変換
//
//	IN:		int			用紙サイズNo（コンボ選択結果）
//	RET:	CString		一括印UCOM返却用文字列 用紙方向
//--------------------------------------------------------
CString	CBatchPrt::CnvPrtVH_Ap2Ucom( int nPinf )
{
	const CString	FN = _T("CBatchPrt::CnvPrtVH_Ap2Ucom()");

	// TODO：実装するアプリ用に、用紙選択コンボ の選択肢により、変換処理を実装してください。

	// 用紙サイズコンボ の選択結果から、UCOM情報に変換
	CString		strRet;
	CString		strLog;

	switch ( nPinf ) {
	case	ID_PRTSIZE_A4T:					// 0:A4縦
		strRet = UCOMITM_BATCHPRT_V_STR;	// 縦をセット
		break;

	default:
		strLog.Format( _T("%d"), nPinf);
		DbgWriteLog( FN, _T( "UCOM I/F AP管理用紙サイズ指定値 異常 [") + strLog + _T("] → A4縦(初期値)として処理継続") );

		strRet = UCOMITM_BATCHPRT_V_STR;	// 縦をセット
		break;
	}

	return strRet;
}

//--------------------------------------------------------
// 出力結果返却  コモン情報(UCOM)
//	IN:		BATCHPRTUCOMINF_RETPRT		印刷結果返却情報
//	RET:	TRUE 正常終了
//--------------------------------------------------------
BOOL	CBatchPrt::SetUcomInfPrt( BATCHPRTUCOMINF_RETPRT *pRetPrtInf )
{
	const CString	FN = _T("CBatchPrt::SetUcomInfPrt()");
	CString	strKey = "";
	char	szBuf[256];

	// パラメータチェック
	if ( !pRetPrtInf ) {
		DbgWriteLog( FN, _T("パラメータ異常 END") );
		return FALSE;
	}

	// 依頼キー取得
	strKey = GetBatchKey();

	// UCOM 情報 デバッグ出力
	CString		strLog;
	DbgWriteLog( FN, _T( "UCOM I/F <SET> 依頼キー             [") + strKey + _T("]") );
	strLog.Format( _T("%d"), pRetPrtInf->nPrtRet ); 
	DbgWriteLog( FN, _T( "UCOM I/F <SET> 印刷結果             [") + strLog + _T("]") );

	//------------------------------
	// “依頼キー”
	//------------------------------
	memset( szBuf, 0x00, sizeof(szBuf));
	strcpy_s( szBuf, strKey );
	UCOM_SetString_BC( UCOMKEY_BATCHKEY, szBuf );

	//------------------------------
	// “印刷結果”
	//------------------------------
	memset( szBuf, 0x00, sizeof(szBuf));
	sprintf_s( szBuf, _T("%d"),  pRetPrtInf->nPrtRet);
	UCOM_SetString_BC( UCOMKEY_PRTRET, szBuf );

	return TRUE;
}


//--------------------------------------------------------
// 一括印刷アプリからの起動  確認
//	IN:		なし
//	RET:	TRUE 出力モードで起動／FALSE 以外
//	MEMO:	GetUcomInfCom() 実行後に有効な関数
//--------------------------------------------------------
BOOL	CBatchPrt::IsBatchExec( void )	
{  
	// 一括印刷アプリからの起動 の場合 TRUE 返却
	if ( ID_BATCHMODE_NONE != m_nBatchMode ) { return TRUE; }
	return FALSE; 
}

// 24/01/16_ScoreLink add -->
//--------------------------------------------------------
// ScloreLinkからの起動  確認
//	IN:		なし
//	RET:	TRUE ScoreLinkからの起動／FALSE 以外
//--------------------------------------------------------
BOOL	CBatchPrt::IsScoreLinkExec( void )	
{  
	if(strlen(m_udUcomInf.strSlIraiId) > 0) { return TRUE; }
	return FALSE; 
}

//--------------------------------------------------------
// PDFファイル名 取得
//
//	IN:		なし
//	RET:	起動時に UCOMから受け取った PDFファイル名
//	MEMO:	GetUcomInfCom() 実行後に有効な関数
//--------------------------------------------------------
CString	CBatchPrt::GetPdfName( void )
{
	return m_udUcomInf.strSlPdfName;
}
// 24/01/16_ScoreLink add <--

//--------------------------------------------------------
// 一括印刷 出力モードでの起動 確認
//	IN:		なし
//	RET:	TRUE 出力モードで起動／FALSE 以外
//	MEMO:	GetUcomInfCom() 実行後に有効な関数
//--------------------------------------------------------
BOOL	CBatchPrt::IsBatchModePrt( void )	
{  
	// 一括印刷 出力モード の場合 TRUE 返却
	if ( ID_BATCHMODE_PRT == m_nBatchMode ) { return TRUE; }
	return FALSE; 
}

//--------------------------------------------------------
// 一括印刷 CDR出力モードでの起動 確認
//	IN:		なし
//	RET:	TRUE CDR出力モードで起動／FALSE 以外
//	MEMO:	GetUcomInfCom() 実行後に有効な関数
//--------------------------------------------------------
BOOL	CBatchPrt::IsBatchModeCDR( void )	
{  
	// 一括印刷 出力モード の場合 TRUE 返却
	if ( ID_BATCHMODE_CDR == m_nBatchMode ) { return TRUE; }
	return FALSE; 
}

//--------------------------------------------------------
// 一括印刷 設定モードでの起動 確認
//	IN:		なし
//	RET:	TRUE 一括印刷設定モードで起動／FALSE 以外
//	MEMO:	GetUcomInfCom() 実行後に有効な関数
//--------------------------------------------------------
BOOL	CBatchPrt::IsBatchModeSet( void )	
{  
	// 一括印刷 設定モード の場合 TRUE 返却
	if ( ID_BATCHMODE_SET == m_nBatchMode ) { return TRUE; }
	return FALSE; 
}

//--------------------------------------------------------
// 一括印刷モード取得
//
//	IN:		なし
//	RET:	EnumIdBatchMode 参照
//	MEMO:	GetUcomInfCom() 実行後に有効な関数
//--------------------------------------------------------
int		CBatchPrt::GetBatchMode( void )
{
	return m_nBatchMode;
}

//--------------------------------------------------------
// 依頼キー 取得
//
//	IN:		なし
//	RET:	起動時に UCOMから受け取った 依頼キー
//	MEMO:	GetUcomInfCom() 実行後に有効な関数
//--------------------------------------------------------
CString	CBatchPrt::GetBatchKey( void )
{
	return m_udUcomInf.strBatchKey;
}

//--------------------------------------------------------
// 出力プリンタ名 取得
//
//	IN:		なし
//	RET:	起動時に UCOMから受け取った プリンタ名
//	MEMO:	GetUcomInfCom() 実行後に有効な関数
//--------------------------------------------------------
CString	CBatchPrt::GetPrtName( void )
{
	return m_udUcomInf.strPrtName;
}

//--------------------------------------------------------
// 出力部数 取得
//
//	IN:		なし
//	RET:	起動時に UCOMから受け取った 印刷部数取得
//	MEMO:	GetUcomInfCom() 実行後に有効な関数
//--------------------------------------------------------
int	CBatchPrt::GetPrtCnt( void )
{
	return atoi( m_udUcomInf.strPrtCnt );
}

//--------------------------------------------------------
// 開始月 オフセット 取得
//
//	IN:		なし
//	RET:	起動時に UCOMから受け取った 開始月オフセット値取得
//	MEMO:	GetUcomInfCom() 実行後に有効な関数
//--------------------------------------------------------
int	CBatchPrt::GetPrtOfsetS( void )
{
	return atoi( m_udUcomInf.strPrtOffSetS );
}
//--------------------------------------------------------
// 終了月 オフセット 取得
//
//	IN:		なし
//	RET:	起動時に UCOMから受け取った 終了月オフセット値取得
//	MEMO:	GetUcomInfCom() 実行後に有効な関数
//--------------------------------------------------------
int	CBatchPrt::GetPrtOfsetE( void )
{
	return atoi( m_udUcomInf.strPrtOffSetE );
}
//--------------------------------------------------------
// 異常終了 状態セット
//
//	IN:		なし
//	RET:	なし
//	MEMO:	エラー終了通知受信時に呼び出し
//			CMainFrame::OnMessageClose() (WPARAM=1)
//
//	2010/08/02 一括印刷 異常終了対応
//--------------------------------------------------------
void	CBatchPrt::SetErrorExit(void )
{
	const CString	FN = _T("CBatchPrt::SetErrorExit()");
	DbgWriteLog( FN, _T("START - END  異常発生による 終了を セット"));
	m_fErrorEnd = TRUE;
}
//--------------------------------------------------------
// 異常終了 状態取得
//
//	IN:		なし
//	RET:	TRUE=異常終了時／FALSE正常終了時
//
//	2010/08/02 一括印刷 異常終了対応
//--------------------------------------------------------
BOOL	CBatchPrt::IsErrorExit(void )
{
	return	m_fErrorEnd;
}

//--------------------------------------------------------
// 終了通知メッセージセット
//
//	IN:		UINT	通知メッセージ
//			UINT	終了通知 パラメータ
//	RET:	なし
//	MEMO:	WM_CLOSE, WM_QUERYENDSESSION 受信時にセット
//
//	2010/08/10 一括印刷 ログオフ時 UCOM I/F 返却なし対応
//--------------------------------------------------------
void	CBatchPrt::SetWmExitMsg( UINT message, WPARAM param )
{
	const CString	FN = _T("CBatchPrt::SetWmExitMsg()");
	CString			strLog;
	strLog.Format( _T("START  message=[0x%04x], param=[0x%04x]"), message, param );
	DbgWriteLog( FN, strLog );
	
	m_uiWmExitMsg	= message;	// メッセージをセット
	m_wpWmExitParam	= param;	// パラメータをセット	

}
//--------------------------------------------------------
// 終了通知メッセージ取得
//
//	IN:		なし
//	RET:	終了通知メッセージ WM_CLOSE, WM_QUERYENDSESSION 等
//
//	2010/08/10 一括印刷 ログオフ時 UCOM I/F 返却なし対応
//--------------------------------------------------------
UINT	CBatchPrt::GetWmExitMsg( void )
{
	return	m_uiWmExitMsg;
}
//--------------------------------------------------------
// 終了通知 パラメータ取得
//
//	IN:		なし
//	RET:	終了通知メッセージ WM_CLOSE, WM_QUERYENDSESSION 等
//
//	2010/08/10 一括印刷 ログオフ時 UCOM I/F 返却なし対応
//--------------------------------------------------------
WPARAM	CBatchPrt::GetWmExitParam( void )
{
	return	m_wpWmExitParam;
}

//--------------------------------------------------------
// 一括印刷 出力 指示
//	IN:		なし
//	RET:	TRUE 正常終了
//--------------------------------------------------------
BOOL	CBatchPrt::DoBatchPrint( void )
{
	const CString	FN = _T("CBatchPrt::DoBatchPrint()");
	DbgWriteLog( FN, _T("START   送信   WM_KEYDOWN   VK_F5"));

	// 印刷ボタン押下 メッセージ送信
	AfxGetMainWnd()->PostMessage( WM_KEYDOWN, VK_F5, 0xFF );

	DbgWriteLog( FN, _T("END"));
	return TRUE;
}

//--------------------------------------------------------
// 一括印刷 モードでの起動終了 指示（アプリ終了）
//	IN:		なし
//	RET:	TRUE 正常終了
//--------------------------------------------------------
BOOL	CBatchPrt::ExitBatchMode( void )
{
	const CString	FN = _T("CBatchPrt::ExitBatchMode()");
	DbgWriteLog( FN, _T("START"));

	//	2010/08/31 CVPrint::ErrorAbort() 実行後の一括印刷終了対応
	// 印刷モジュール異常終了処理中フラグ OFF（完了→WM_CLOSE送信可能）
	if ( IsPrtMdlOnErr() ) {
		DbgWriteLog( FN, _T("END - 印刷モジュール異常終了後処理中 WN_CLOSE 送信迂回"));
		return TRUE;
	}

	// 終了メッセージ送信
	// CMainFrame::OnMessageClose() wparam=1 は エラー終了 → 0をセット
	// 一括印刷時は 終了確認メッセージを表示せず終了する仕様。
	// lparam に 9999 をセット
	AfxGetMainWnd()->PostMessage( WM_CLOSE, 0, LPARAM_BPRT_EXITAPP );

	DbgWriteLog( FN, _T("END - 送信   WM_CLOSE"));
	return TRUE;
}

//--------------------------------------------------------
// 一括印刷 エラーメッセージ表示/非表示 状態取得
//	IN:		なし
//	RET:	TRUE 表示 / FALSE 非表示
//
// 2010/08/12 一括印刷 印刷時メッセージ非表示対応
//--------------------------------------------------------
BOOL	CBatchPrt::GetMsgShowFlg( void )
{
	const CString	FN = _T("CBatchPrt::GetMsgShowFlg()");
	DbgWriteLog( FN, _T("START"));

	// メッセージ表示フラグ
	BOOL fErrMsgShowFlg = FALSE;

	// 一括印刷 印刷時メッセージ非表示対応
	// 　→一括印刷 出力モード全般（出力、CDR出力）にて、
	// 　　出力実行中のメッセージ表示を制限する。

	// 一括印刷 起動モード取得
	switch ( GetBatchMode() ) {
	case ID_BATCHMODE_PRT:			// 出力モード
	case ID_BATCHMODE_CDR:			// CDR出力モード
		fErrMsgShowFlg = FALSE;		// メッセージ 非表示
		break;
	case ID_BATCHMODE_NONE:			// 通常起動
	case ID_BATCHMODE_SET:			// 設定モード
	default:
		fErrMsgShowFlg = TRUE;		// メッセージ 表示
		break;
	}

	DbgWriteLog( FN, _T("END"));
	return fErrMsgShowFlg;
}

//--------------------------------------------------------
// 税務用年月日セット
//	IN:		CDBZmSub*	DB財務クラスのポインタ
//	RET:	TRUE：正常 / FALSE：エラー
//
// 【補足】
// 　一括印刷関連の処理（一括印刷アクセスモジュール使用）であるが、
// 　アプリ終了時共通（一括印刷起動時のみではなく、通常起動時も）で実行する必要がある。
// 　　→2010/08/25 ICS様より
//
// 2010/08/25 税務用年月日セット対応
//--------------------------------------------------------
BOOL CBatchPrt::SetPrtSchYmd( CDBZmSub* pZmSub )
{
	int nRet = FALSE;		// 戻り値（エラー=FALSEをセット）

	const CString	FN = _T("CBatchPrt::SetPrtSchYmd()");
	CString			strLog;
	DbgWriteLog( FN, _T("START"));

	//---------------------------------------------------
	// グループNOを取得する
	//---------------------------------------------------
	char			szGrNum[16];
	CString			strGrNum, strBuff;

	memset( szGrNum, '\0', sizeof(szGrNum) );		// バッファ初期化

	PCOM_GetString( _T( "ExecGrJobs" ), szGrNum );
	strBuff.Format( "%s", szGrNum );

	strGrNum = strBuff.MakeLower();					// 大文字→小文字変換

	//---------------------------------------------------
	// 期首月・期末月を取得する
	//---------------------------------------------------
	int nSs_ymd = 0, nEe_ymd = 0;

	try {
		// zvolumeテーブルオープン
		pZmSub->VolumeOpen();

		nSs_ymd = pZmSub->zvol->ss_ymd;	// 期首年月日
		nEe_ymd = pZmSub->zvol->ee_ymd;	// 期末年月日
	
		// zvolumeテーブルクローズ
		pZmSub->VolumeClose();
	}
	catch (CDBException *e) {
		// テーブルオープンに失敗時 → エラー
		e->Delete();

		// エラー
		ICSMessageBox( pZmSub->GetZmErrMsg(), MB_ICONSTOP );
		return nRet;
	}

	CPrintSch		PrintSch;		// 一括印刷アクセスモジュール
	PRT_TAXSETREC	taxrec;			// 税務年月日セット情報	
	memset( &taxrec, '\0', sizeof(PRT_TAXSETREC) );		// バッファ初期化

	// データセット
	//wsprintf( taxrec.grno, _T("1b") );					// グループNo.
	wsprintf( taxrec.grno, (LPTSTR)(LPCTSTR)strGrNum );		// グループNo.	非営利からも呼ばれるので、"1b"固定から変更
	wsprintf( taxrec.proname, _T("DBUchiwake.exe") );		// プログラム名
	taxrec.symd = nSs_ymd;									// 期首年月日
	taxrec.eymd = nEe_ymd;									// 期末年月日

	// 税務年月日をセットする
	if (PrintSch.SetPrtschYmd( pZmSub, &taxrec ) == -1) {
		// ログ
		DbgWriteLog( FN, _T("税務年月日 登録エラー") );

		// エラー → エラー文言はモジュールから取得する（2010/08/25 ICS様より）
		ICSMessageBox( PrintSch.GetErrMsg(), MB_ICONSTOP );
	} else {
		// ログ
		strLog.Format( _T("グループNo. = [%s] / プログラム名 = [%s]"), taxrec.grno, taxrec.proname );
		DbgWriteLog( FN, strLog );
		strLog.Format( _T("期首年月日 = [%d] / 期末年月日 = [%d]"), taxrec.symd, taxrec.eymd );
		DbgWriteLog( FN, strLog );

		// 正常
		nRet = TRUE;
	}

	DbgWriteLog( FN, _T("END"));
	return nRet;
}

//****************************************************************
//	一括印刷 印刷時メッセージ表示/非表示
//	【引数】	CString			メッセージ文言
//	【戻値】	なし
//
// 2010/08/12 一括印刷 印刷時メッセージ非表示対応
// 2010/08/30 実装クラス変更 → UCHIIMP ビルド対応
//*****************************************************************
void CBatchPrt::ICSMessageBoxEx( CString strMsg )
{
	const CString	FN = _T("CBatchPrt::ICSMessageBoxEx()");
	CString			strLog;
	strLog.Format( _T("START  strMsg=[%s]"), strMsg );
	DbgWriteLog( FN, strLog);

	// 一括印刷 メッセージ表示/非表示フラグを取得する
	switch( GetMsgShowFlg() )
	{
	case FALSE:
		// メッセージを表示しない。
		break;
	default:
		// メッセージを表示する。
		ICSMessageBox( strMsg );
		break;
	}

	DbgWriteLog( FN, _T("END"));
}//ICSMessageBoxEx

//--------------------------------------------------------
// 印刷モジュールエラー発生時にセット
//
//	Memo：CVPrint::ErrorAbort() 実行時に必ず実行してください
//
//	IN:		なし
//	RET:	なし
//
//	2010/08/31 CVPrint::ErrorAbort() 実行後の一括印刷終了対応
//--------------------------------------------------------
void	CBatchPrt::SetPrtMdlOnErr( void )
{
	const CString	FN = _T("CBatchPrt::SetPrtMdlOnErr()");
	DbgWriteLog( FN, _T("START - END") );
	m_fPrtMdlErr = TRUE;
}
//--------------------------------------------------------
// 印刷モジュールエラー発生状態解除
//
//	Memo：本関数は、下記④のタイミングで実行してください
//		  
//	 CVPrint::ErrorAbort() 実行時に以下の順で処理される
//			一括印刷仕様_100831.doc（P49）より
//	　① CVPrint.ErrorAbort() 実行 
//	　② CxxView::OnEndPrinting() が呼び出される
//	　③ 最終的には OnButtonF5 が呼び出され、 CxxView::OnPreparePrinting() が再度実行される
//		 ( ErrorAbort() 実行により、OnButtonF5 をコールしている by ICS中垣内様 )
//	　④ CxxView::OnPreparePrinting() の CVPrint.InitPrint() が０以外の値を返して、エラー処理に流れる
//	
//		上記④が完了するまで、WM_CLOSE を送信してはいけない。
//		→ 印刷モジュールの後処理が完了できなくなり異常終了等につながる。
//		   ④の前で送信したWM_CLOSEが、④でメッセージ表示を行うタイミング、または
//         他の処理でダイアログ表示を行うタイミングで処理される様子。
//        （試算表の場合貸借バランスNG→印刷確認メッセージ表示）
//
//		   結果として、意図していないタイミングで アプリの終了処理が実行されるので、異常終了につながる。
//		  （ ICSExitEx() の2回以上実行で異常終了発生と同様。）
//		
//	IN:		なし
//	RET:	なし
//
//	2010/08/31 CVPrint::ErrorAbort() 実行後の一括印刷終了対応
//--------------------------------------------------------
void	CBatchPrt::ClrPrtMdlOnErr( void )
{
	const CString	FN = _T("CBatchPrt::ClrPrtMdlOnErr()");
	DbgWriteLog( FN, _T("START - END") );
	m_fPrtMdlErr = FALSE;
}

//--------------------------------------------------------
// 印刷モジュールエラー発生状態確認
//
//	IN:		なし
//	RET:	BOOL	TRUE ：WM_CLOSE送信禁止 = エラー発生中
//					FALSE：WM_CLOSE送信可能 = エラー発生無 or エラー後処理完了
//
//	2010/08/31 CVPrint::ErrorAbort() 実行後の一括印刷終了対応
//--------------------------------------------------------
BOOL	CBatchPrt::IsPrtMdlOnErr( void )
{
	const CString	FN = _T("CBatchPrt::ClrPrtMdlOnErr()");
	CString			strLog;
	strLog.Format( _T("START - END  return  m_fPrtMdlErr=[%d]"), m_fPrtMdlErr );
	DbgWriteLog( FN, strLog );
	return 	m_fPrtMdlErr;
}

//--------------------------------------------------------
// 詳細設定 実行誘導 フラグ ON
//
//	IN:		なし
//	RET:	なし
//
// 2011/01/11 修正依頼No.N-037 一括印刷ユーザ情報共通化
// 2011/02/08 修正依頼No.N-040 一括印刷情報マスタ単位昇格（運用中マスタ対応）
//--------------------------------------------------------
//void	CBatchPrt::SetDoSetMode( void )
void	CBatchPrt::SetDoSetMode( int nDoSetMode )
{
	//m_fDoSetMode = TRUE;
	m_nDoSetMode = nDoSetMode;
}
//--------------------------------------------------------
// 詳細設定 実行誘導 フラグ 取得
//
//	IN:		なし
//	RET:	BOOL	TRUE	：印刷/CDR 中断し、詳細設定実行誘導メッセージの表示が必要
//					FASLE	：処理継続可能
//	
// 2011/01/11 修正依頼No.N-037 一括印刷ユーザ情報共通化
// 2011/02/08 修正依頼No.N-040 一括印刷情報マスタ単位昇格（運用中マスタ対応）
//--------------------------------------------------------
//BOOL	CBatchPrt::IsDoSetMode( void )
int	CBatchPrt::IsDoSetMode( void )
{
	//return m_fDoSetMode;
	return m_nDoSetMode;
}
//--------------------------------------------------------
// 詳細設定 実行誘導 フラグ クリア
//
//	IN:		なし
//	RET:	なし
//
// 2011/01/11 修正依頼No.N-037 一括印刷ユーザ情報共通化
// 2011/02/08 修正依頼No.N-040 一括印刷情報マスタ単位昇格（運用中マスタ対応）
//--------------------------------------------------------
void	CBatchPrt::ClrDoSetMode( void )
{
	//m_fDoSetMode = FALSE;
	m_nDoSetMode = OWNTB_CHKREC_OK_EXIST;
}
