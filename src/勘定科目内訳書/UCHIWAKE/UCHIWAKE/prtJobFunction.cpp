//**************************************************************************************************
//
// ClassName	: CprtJobFunction
// FileName		:  prtJobFunction.cpp
//
// ■印刷関連 動作定義
//
//**************************************************************************************************

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "StdAfx.h"
#include ".\prtJobFunction.h"

#include "Uchiwake.h"
#include "UchiwakeDoc.h"
#include "UchiwakeView.h"

#include "CoinfCtl8.h"		// 2010/08/XX 業務統合メニュー対応
//#include "ICSMNSub8.h"		// 2015/06/03 マイナンバー対応

// midori 190301 add -->
#define	AS_SPLIT_MAX	256

extern BOOL	bG_Kanso;
extern BOOL	bG_InvNo;		// インボイス番号使用サイン（決算期末が令和6年3月1日を超えている）	インボイス登録番号追加対応_23/10/20 add
// 24/04/22_ScoreLink add -->
extern int	g_nLastPage;
// 24/04/22_ScoreLink add <--

int		Gothic;
// midori 190301 add <--

// midori 190505 add -->
// midori 157042 del -->
//extern int g_PrnHoho[2];
// midori 157042 del <--

extern int ChkIkkatuSort(CDatabase* pDB, CdbUcInfSub* pfcRec);
// 改良No.21-0086,21-0529 cor -->
//extern int KamokuRowEnableSgn(CDatabase* pDB, int pSw);
// ------------------------------
extern int KamokuRowEnableSgn(CDatabase* pDB, int pSw, int nFormSeq);
// 改良No.21-0086,21-0529 cor <--
// midori 190505 add <--

CprtJobFunction::CprtJobFunction(void)
: m_eJob(JobNop)
, m_index(1)	//
, m_lFncFlag(0)
, m_nPageSum(0)
, m_nPageAllSum(0)
, m_nCurPage(0)
// midori 180101 del -->
//, m_nOutGenzai(0)
// midori 180101 del <--
// midori 160608 add -->
, m_nOutZero(0)
// midori 160608 add <--
, m_bPrtAll(false)
, m_bSlash(false)
, m_CallBy(-1)
, m_isF5FromPrev(FALSE)
{
}

CprtJobFunction::~CprtJobFunction(void)
{
	// マップ情報クリア
	//DeletePageMapAll();
}

////////////////////////////////////////////////////////////////////////////////
// SelectJobOne()
//
// ■1帳表印刷を選択する。
// ※ビュークラスの、OnButtonF4()内にて、OnFilePrintPreview()の前に呼び出すこと。
// ※ビュークラスの、OnButtonF5()内にて、OnFilePrint()の前に呼び出すこと。
//
// ▼必ず、出力データを、出力リストの先頭（インデックス：0）に格納する
//
// midori 151730,151263 class del -->
//// 引数		：CDBZmSub*			pZmSub				DB財務クラスオブジェクト
// midori 151730,151263 class del <--
// midori 151730,151263 class add -->
// 引数		：CDBNpSub*			pZmSub				DB非営利クラスオブジェクト
// midori 151730,151263 class add <--
//			：CString			strPath				実行フォルダのパス
//			：int				nFormSeq			様式シーケンス番号
//			：intq				nNameKeiBitArray	計の名称のビット配列
//
// 戻り値	：int				エラーコード		FUNCTION_OK：成功
//													FUNCTION_NG：失敗
//
////////////////////////////////////////////////////////////////////////////////
// midori 190505 del -->
//// midori 151730,151263 class del -->
////int CprtJobFunction::SelectJobOne(CDBZmSub* pZmSub, CString strPath, int nFormSeq, int nNameKeiBitArray)
//// midori 151730,151263 class del <--
//// midori 151730,151263 class add -->
//int CprtJobFunction::SelectJobOne(CDBNpSub* pZmSub, CString strPath, int nFormSeq, int nNameKeiBitArray)
//// midori 151730,151263 class add <--
// midori 190505 del <--
// midori 190505 add -->
int CprtJobFunction::SelectJobOne(CDBNpSub* pZmSub, CString strPath, int nFormSeq, int nNameKeiBitArray)
// midori 190505 add <--
{
	m_pZmSub		= pZmSub;
	m_pDB			= pZmSub->m_database;
	m_strPath		= strPath;

	// 初期チェック
	if( CheckInit() == FUNCTION_NG ){

		return FUNCTION_NG;
	}

	// 一括印刷ではないのでOFF（既にOFFになっているはず）
	m_bPrtAll	= FALSE;

	// 初期化
	InitProp();
	m_eJob		= JobOne;	// 印刷処理の種類を設定
	m_lFncFlag	= 0x5;		// 丁合い許可、ページ指定許可

	// マップ情報クリア
	//DeletePageMapAll();

	// 様式シーケンス番号の設定
	m_PrtInfMainList[1].Lite.FormSeq	= nFormSeq;		// インデックス0は予約済み

	// uc_inf_sub
// midori 190505 del -->
//	GetDbDataTableUcInfSub( 1 );
// midori 190505 del <--
// midori 190505 add -->
	GetDbDataTableUcInfSub(1, 0);
// midori 190505 add <--

	// uc_inf_form_type
	GetDbDataTableUcInfFormType( 1 );

	// uc_inf_main
	GetDbDataTableUcInfMain();

	// 最大ページ数の取得
	int nRetPage = GetPageAll( 1, nNameKeiBitArray);

	if( nRetPage <= 0 ){

// midori 160913 cor -->
//		ICSMessageBox( PRT_ERR_NOTJOB );
// ---------------------
		ICSMessageBox( "出力データがありません。");
// midori 160913 cor <--

		return FUNCTION_NG;
	}

	// ページ情報の設定
	m_PrtInfMainList[1].Lite.PageMin	= 1;			// 「1」固定
	m_PrtInfMainList[1].PageAll			= nRetPage;
	m_PrtInfMainList[1].PageTotal		= nRetPage;
	m_PrtInfMainList[1].Lite.PageMax	= nRetPage;
	m_nPageAllSum						= nRetPage;

	// スケジュールリストの作成
	MakeListSch( 1 );

	// 初期情報の取得
	GetDataInitVal();

	// プリントモジュールが使用する出力情報定義パラメータの登録
	SetNprtParam();

	return FUNCTION_OK;
}

// midori 180101 del -->
//// midori 160608 cor -->
////////////////////////////////////////////////////////////////////////////////////
////// SelectJobAll()
//////
////// ■一括印刷を選択する。
////// ※ビュークラスの、OnPrintall1()内にて、OnFilePrint()の前に呼び出すこと。
//////
////// ▼必ず、出力データを出力リストの先頭（インデックス：0）から順に格納する
//////
////// 引数		：CDBZmSub*			pZmSub				DB財務クラスオブジェクト
//////			：CString			strPath				実行フォルダのパス
//////			：int				nOutGenzai			「現在」の出力（0以外なら出力）
//////			：PRT_INF_LITE*		pPrtInfLite			出力リスト
//////			：int				nNameKeiBitArray	計の名称のビット配列
//////			：int				call_by				呼び出し方法
//////
////// 戻り値	：int				エラーコード		FUNCTION_OK：成功
//////													FUNCTION_NG：失敗
//////
////////////////////////////////////////////////////////////////////////////////////
////int CprtJobFunction::SelectJobAll(CDBZmSub* pZmSub, CString strPath, int nOutGenzai, PRT_INF_LITE* pPrtInfLite, int nNameKeiBitArray, int call_by)
//// ---------------------
//////////////////////////////////////////////////////////////////////////////////
//// SelectJobAll()
////
//// ■一括印刷を選択する。
//// ※ビュークラスの、OnPrintall1()内にて、OnFilePrint()の前に呼び出すこと。
////
//// ▼必ず、出力データを出力リストの先頭（インデックス：0）から順に格納する
////
//// 引数		：CDBZmSub*			pZmSub				DB財務クラスオブジェクト
////			：CString			strPath				実行フォルダのパス
////			：int				nOutGenzai			「現在」の出力（0以外なら出力）
////			：int				nOutZero			帳票イメージのみ出力する（0:出力しない）
////			：PRT_INF_LITE*		pPrtInfLite			出力リスト
////			：int				nNameKeiBitArray	計の名称のビット配列
////			：int				call_by				呼び出し方法
////
//// 戻り値	：int				エラーコード		FUNCTION_OK：成功
////													FUNCTION_NG：失敗
////
//////////////////////////////////////////////////////////////////////////////////
//int CprtJobFunction::SelectJobAll(CDBZmSub* pZmSub, CString strPath, int nOutGenzai, PRT_INF_LITE* pPrtInfLite,int nOutZero, int nNameKeiBitArray, int call_by)
//// midori 160608 cor <--
// midori 180101 del <--
// midori 180101 add -->
////////////////////////////////////////////////////////////////////////////////
// SelectJobAll()
//
// ■一括印刷を選択する。
// ※ビュークラスの、OnPrintall1()内にて、OnFilePrint()の前に呼び出すこと。
//
// ▼必ず、出力データを出力リストの先頭（インデックス：0）から順に格納する
//
// midori 151730,151263 class del -->
//// 引数		：CDBZmSub*			pZmSub				DB財務クラスオブジェクト
// midori 151730,151263 class del <--
// midori 151730,151263 class add -->
// 引数		：CDBNpSub*			pZmSub				DB非営利クラスオブジェクト
// midori 151730,151263 class add <--
//			：CString			strPath				実行フォルダのパス
//			：int				nOutZero			帳票イメージのみ出力する（0:出力しない）
//			：PRT_INF_LITE*		pPrtInfLite			出力リスト
//			：int				nNameKeiBitArray	計の名称のビット配列
//			：int				call_by				呼び出し方法
//
// 戻り値	：int				エラーコード		FUNCTION_OK：成功
//													FUNCTION_NG：失敗
//
////////////////////////////////////////////////////////////////////////////////
// midori 151730,151263 class del -->
//int CprtJobFunction::SelectJobAll(CDBZmSub* pZmSub, CString strPath, PRT_INF_LITE* pPrtInfLite,int nOutZero, int nNameKeiBitArray, int call_by)
// midori 151730,151263 class del <--
// midori 151730,151263 class add -->
int CprtJobFunction::SelectJobAll(CDBNpSub* pZmSub, CString strPath, PRT_INF_LITE* pPrtInfLite,int nOutZero, int nNameKeiBitArray, int call_by)
// midori 151730,151263 class add <--
// midori 180101 add <--
{
	m_pZmSub	= pZmSub;
	m_pDB		= pZmSub->m_database;
	m_strPath	= strPath;
	m_CallBy	= call_by;

	// 初期チェック
	if( CheckInit() == FUNCTION_NG ){

		return FUNCTION_NG;
	}

	// 一括印刷の開始
	m_bPrtAll		= TRUE;

	// 初期化
	InitProp();
	m_eJob			= JobAll;	// 印刷処理の種類を設定
//	m_lFncFlag		= 0x11;		// 丁合い許可、印刷ダイアログでのプレビュー禁止
	m_lFncFlag		= 0x01;		// 丁合い許可、印刷ダイアログでのプレビュー可能
// midori 180101 del -->
//	m_nOutGenzai	= nOutGenzai;
// midori 180101 del <--
// midori 160608 add -->
	m_nOutZero		= nOutZero;	// 「帳票イメージのみ出力する」
// midori 160608 add <--

	// マップ情報クリア
	//DeletePageMapAll();

	// 出力リストに、データを格納
	for( int i = 1; i < PRT_ID_FORMNO_MAX; i++ ){	// インデックス0は予約済み
		m_PrtInfMainList[i].Lite = pPrtInfLite[i];
	}

	// 総ページ数の取得＆スケジュールリストの作成
	for( int j = 1; j < PRT_ID_FORMNO_MAX; j++ ){	// インデックス0は予約済み
		
		// uc_inf_sub
// midori 190505 del -->
		//GetDbDataTableUcInfSub( j );
// midori 190505 del <--
// midori 190505 add -->
		GetDbDataTableUcInfSub(j, 1);
// midori 190505 add <--

		// uc_inf_form_type
		GetDbDataTableUcInfFormType( j );

		// uc_inf_main
		GetDbDataTableUcInfMain();

		// 最大ページ数の取得
		int nRetPage = GetPageAll( j, nNameKeiBitArray);

// midori 160608 add -->
		// 帳票イメージのみ出力するにチェックが有る場合は終了頁を帳表の総ページ数とする
		if(nOutZero == 1)	nRetPage = m_PrtInfMainList[j].Lite.PageMax;
// midori 160608 add <--

		if( nRetPage <= 0 ){

			break;
		}

		// ページ情報の設定
		m_PrtInfMainList[j].PageAll		= nRetPage;
		m_PrtInfMainList[j].PageTotal	= m_PrtInfMainList[j].Lite.PageMax
										- m_PrtInfMainList[j].Lite.PageMin + 1;
		m_nPageAllSum += m_PrtInfMainList[j].PageTotal;

		// スケジュールリストの作成
		MakeListSch( j );
	}

	// 初期情報の取得
	GetDataInitVal();

	// プリントモジュールが使用する出力情報定義パラメータの登録
	SetNprtParam();

	return FUNCTION_OK;
}

////////////////////////////////////////////////////////////////////////////////
// JobPreparePrinting()
//
// ■必ずビュークラスの、OnPreparePrinting()内にて、呼び出すこと。
// 　それ以外では、呼び出してはならない！
//
////////////////////////////////////////////////////////////////////////////////
BOOL CprtJobFunction::JobPreparePrinting(CPrintInfo* pInfo, CView* pView)
{
// midori M-16090701 add -->
	CfrmUc000Common* frmCmn = NULL;
// midori M-16090701 add <--

	//////////////////////////////////////////////////
	// 印刷指定パケットの作成

//2007.03.19：Excel出力のためシーケンスを変更
//	// エラーが検出されなければ実行
//	if( m_icsVprn.m_ErrFlag == 0 ){
//		
//		// プリンタ名
//		PCOM_GetString( _T( "SelectPrinter" ), m_icsPset.psPrinterName );
//
//		// ドキュメント名
//		strcpy_s( (LPSTR)m_icsPset.psJobName, sizeof( m_icsPset.psJobName ), PRT_NAME_JOB );
//
//		// その他の設定
//		m_icsPset.psPaperSize	= DMPAPER_A4;			// 用紙サイズ
//		m_icsPset.psOrientation	= DMORIENT_PORTRAIT;	// 方向
//		m_icsPset.psPaperBin	|= 0x8000;				// ホッパ（給紙）
//		m_icsPset.psCopies		= 1;					// 印刷部数
//		m_icsPset.psPrevFlg		= 0x0100;				// プレビュー コントロール フラグ
//		m_icsPset.psFncFlg		= m_lFncFlag;			// ファンクション フラグ
//
//		// 印字位置調整
//		m_icsPset.psPrnFlg		= 0;																	// 印刷フラグ
//		strcpy_s( &m_icsPset.psPsecName[0], sizeof(m_icsPset.psPsecName), PRT_UCHIWAKE_PSECNAME );		// プログラム・セクション名
//		strcpy_s( &m_icsPset.psListName[0], sizeof(m_icsPset.psListName), PRT_UCHIWAKE_LISTNAME );		// 帳票名
//	}

	//////////////////////////////////////////////////
	// プリンタの初期化

	if( _Init_prn( 0x101, &m_icsVprn, pInfo, &m_icsPset, pView ) != 0 ){		// [2009/07/21 EMF対応 (第1引数を1->0x101)]

		if( m_icsVprn.m_ApErrCod == 2 ){

			// プログラムの終了
			ShowErrMsg( PRT_ERR_PRGEND, TRUE );
		}
		//-----------------------------------------------------------------------------
		//	2010/08/31 CVPrint::ErrorAbort() 実行後の一括印刷終了対応
		//  一括印刷時の CVPrint::ErrorAbort() 実行後の終了指示はこのタイミングで実施
		//  他のアプリは、このタイミングで ClrPrtMdlOnErr() を実施し、
		//  OnButtonF5() で実施している ExitBatchMode() で終了。
		//  内訳書アプリは、OnButtonF5() には実装していないため、このタイミングで行う。
		//-----------------------------------------------------------------------------
		switch ( g_BatchPrt.GetBatchMode()) {
		case ID_BATCHMODE_PRT:			// 一括印刷 出力モード
		case ID_BATCHMODE_CDR:			// 一括印刷 CDR出力モード
			// CVPrint::ErrorAbort() 実施？
			if ( g_BatchPrt.IsPrtMdlOnErr() ) {
				// 印刷モジュール異常終了処理中フラグ OFF（完了→WM_CLOSE送信可能）
				g_BatchPrt.ClrPrtMdlOnErr();		
				// 一括印刷終了指示
				g_BatchPrt.ExitBatchMode();
			}
			break;
		default:
			// 処理なし
			break;
		} 
		return FALSE;
	}

	// 2010/07/XX 一括印刷対応
	// 2010/07/31 仕様変更（7/30） エクセル出力以外は使用を許可する
	// 一括印刷アプリからの起動時、印刷ダイアログを表示しない
	switch ( g_BatchPrt.GetBatchMode() ) {
	case ID_BATCHMODE_SET:				// 一括印刷 設定モード
		// 下記設定により、印刷モジュール側で、印刷を実行せず、正常終了を返却する。
		// 本機能により、詳細設定モード時の 出力の禁止に対応。（7/30 From ICS）
		m_icsVprn.SetOptionMode( POF_PRCPRINT, 1 );
		break;
	
	case ID_BATCHMODE_PRT:				// 一括印刷 出力モード
	case ID_BATCHMODE_CDR:				// 一括印刷 CDR出力モード
		m_icsVprn.SetOptionMode( POF_PRCPRINT, 2 );
		break;

	case ID_BATCHMODE_NONE:				// 通常起動
	default:
		// 処理なし
		break;
	}

	//////////////////////////////////////////////////
	// 印刷ダイアログを起動

	if( !pView->DoPreparePrinting( pInfo ) ){

		int intRet = m_icsVprn.End_PrintDlg( pView );

		switch (intRet) {
		case -1:		// エラー
			// プログラムの終了
			ShowErrMsg( PRT_ERR_PRGEND, TRUE );

		case 0:			// Cancel
			// 印刷ダイアログで、エラー(-1)又はCancel(0)となった場合、
			// 一括印刷中なら終了のためフラグをOFFする
			// なお、印刷(1)の場合は、印刷終了時にOFFするのでフラグを更新しない
			if (m_bPrtAll == TRUE) {
				m_bPrtAll = FALSE;
				m_isF5FromPrev = FALSE;
			}
// midori M-16090701 add -->
			// カーソル移動
			frmCmn = (CfrmUc000Common*)pView;
// midori 155906 add -->
			frmCmn->ClearTempTbl();
// midori 155906 add <--
			frmCmn->CmnDiagSetFocus( frmCmn->m_pDiag, frmCmn->m_uInfo.intCurRow, frmCmn->m_uInfo.intCurCol );
// midori M-16090701 add <--
			break;
		}

		return FALSE;
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
// JobBeginPrinting()
//
// ■必ずビュークラスの、OnBeginPrinting()内にて、呼び出すこと。
// 　それ以外では、呼び出してはならない！
//
////////////////////////////////////////////////////////////////////////////////
void CprtJobFunction::JobBeginPrinting(CDC* pDC, CPrintInfo* pInfo /*= NULL*/, CView* pView /*= NULL*/)
{
	// オブジェクトの初期化、および出力に必要な描画オブジェクトの作成
	m_icsVprn.PrePrint( pDC );

	// 出力データスケジュールファイルの読み込み
// midori 190301 del -->
	//m_icsVprn.m_ErrFlag = psch_inz( 0x81, m_nSchList, 0x2, 
	//	(LPSTR)(LPCTSTR)(m_strPath +  PRT_FILE_SCH ) );
// midori 190301 del <--
// midori 190301 add -->
// インボイス登録番号追加対応_23/10/20 del -->
	//if(bG_Kanso == TRUE)	{
// インボイス登録番号追加対応_23/10/20 del <--
// インボイス登録番号追加対応_23/10/20 add -->
	if(bG_InvNo == TRUE)	{
		m_icsVprn.m_ErrFlag = psch_inz(0x81, m_nSchList, 0x2, (LPSTR)(LPCTSTR)(m_strPath + PRT_FILE_SCH_I));
		Gothic = m_icsVprn.RegisterFont(110, 0, 0, _T("ＭＳ ゴシック"));
	}
	else if(bG_Kanso == TRUE)	{
// インボイス登録番号追加対応_23/10/20 add <--
		m_icsVprn.m_ErrFlag = psch_inz( 0x81, m_nSchList, 0x2, (LPSTR)(LPCTSTR)(m_strPath + PRT_FILE_SCH_K ) );
		Gothic = m_icsVprn.RegisterFont(110,0,0,_T("ＭＳ ゴシック"));
	}
	else	{
		m_icsVprn.m_ErrFlag = psch_inz( 0x81, m_nSchList, 0x2, (LPSTR)(LPCTSTR)(m_strPath + PRT_FILE_SCH ) );
	}
// midori 190301 add <--
}

////////////////////////////////////////////////////////////////////////////////
// JobEndPrinting()
//
// ■必ずビュークラスの、OnEndPrinting()内にて、呼び出すこと。
// 　それ以外では、呼び出してはならない！
//
////////////////////////////////////////////////////////////////////////////////
void CprtJobFunction::JobEndPrinting(CDC* pDC /*= NULL*/, CPrintInfo* pInfo /*= NULL*/, CView* pView /*= NULL*/)
{
	// 一括印刷なら、一括印刷中フラグをOFFする
	// ただし、プレビューの場合は画面が印刷ダイアログに戻るため、
	// （まだ一括印刷が継続中なので）フラグを更新しない
	if (((m_bPrtAll == TRUE) && (pInfo->m_bPreview == FALSE)) || ((m_CallBy != ID_PRT_IKKATSU) && (m_CallBy != ID_PRT_F5))) {
	//if ((m_bPrtAll == TRUE) && (pInfo->m_bPreview == FALSE)) {
		m_bPrtAll = FALSE;
	}

// midori 190301 add -->
	//if(bG_Kanso == TRUE)	{						// 修正No.168497 del
	if(bG_InvNo == TRUE || bG_Kanso == TRUE)	{	// 修正No.168497 add
		m_icsVprn.DeleteRegFont(Gothic);
	}
// midori 190301 add <--

	// CVPrint::PrePrint()で作成した描画オブジェクトを削除
	m_icsVprn.EndPrint();

	//
	SetIsPrintFromPrev();

	// psch_inz()で確保したメモリ等の開放（新たにフォントを作成している場合は削除）
	psch_end();
}

////////////////////////////////////////////////////////////////////////////////
// JobPrepareDC()
//
// ■必ずビュークラスの、OnPrepareDC()内にて、呼び出すこと。
// 　それ以外では、呼び出してはならない！
//
////////////////////////////////////////////////////////////////////////////////
void CprtJobFunction::JobPrepareDC(CDC* pDC, CPrintInfo* pInfo /*= NULL*/, CView* pView /*= NULL*/)
{
	// デバイス コンテキストが、印刷に使われていなければ実行
	if( pDC->IsPrinting() ){

		// CVPrint::PrePrint()で指定された、マッピングモードを設定
		m_icsVprn.InitPage();
	}
}

////////////////////////////////////////////////////////////////////////////////
// JobPrint()
//
// ■必ずビュークラスの、OnPrint()内にて、呼び出すこと。
// 　それ以外では、呼び出してはならない！
//
////////////////////////////////////////////////////////////////////////////////
void CprtJobFunction::JobPrint(CDC* pDC, CPrintInfo* pInfo, CView* pView)
{
	// 直前のページの内容を再描画
	if( m_icsVprn.StartPageJob( pDC ) != 0 ){

		return;
	}

	// スケジュール ファイルの正当性を調べる
	if( CheckFileSch( m_icsVprn.m_ErrFlag ) == FUNCTION_NG ){
		
		return;
	}

	// 2010/08/XX 業務統合メニュー対応
	// 印刷文字列 取得  会社名称 ／ 顧問先名称
	//
	// 【補足】
	// 　会社名は、既存処理としてCprtJobFunction::GetDbDataTableZVolume()でzvolから取得している。
	// 　　→CprtJobFunction::GetDataInitVal()から、初期情報の取得の一処理として実行されている。
	// 　よって、"初期情報の取得"には手を加えたくないので、このタイミングで会社名だけ書き換える方針を取る。
	// 　　→印刷データを作りあげる直前。
	GetTgNameCd( m_PrtInfCommon.CoName );

	//////////////////////////////////////////////////
	// 1ページの出力

	// 現在のページ番号を取得
	int nCurPage = m_icsVprn.GetCurPageNo();

	// 1帳表印刷の場合は、取得した「現在のページ番号」を使用する
	// 1帳表なので、ページ番号をそのまま使用すると、部数印刷や印刷範囲指定も対応できる
	if( m_eJob == JobOne ){
		m_index    = 1;
		m_nCurPage = nCurPage;
	}

	// 一括印刷の場合は、印刷するページを取得する
	else if( m_eJob == JobAll ){
		// 印刷対象のページを取得
		GetPrtPageInfo( nCurPage );

		// 省略文字の設定
		GetDataOmit( m_index );
	}

	// 実行条件
	if( (nCurPage >= 1) && (nCurPage <= m_nPageAllSum ) ){
		// 印刷処理
		if( PrintDataPageOne( m_index, m_nCurPage , pDC ) != FUNCTION_OK ){

			ShowErrMsg( PRT_ERR_SCH_0 );

			return;
		}
	}

	// 次に印刷可能なページ番号を取得
	int nNextPage = m_icsVprn.GetNextPageNo();

	// 終了条件
	if( (nNextPage < 1) || (nNextPage > m_nPageAllSum) ){

		// 印刷の終了
		m_icsVprn.LastPage();
// 24/04/22_ScoreLink add -->
		g_nLastPage = 1;
// 24/04/22_ScoreLink add <--
	}
	//else{

	//	// 次の帳表を印刷
	//	if( nNextPage > m_nPageSum ){

	//		m_index++;
	//		m_nPageSum	+= m_PrtInfMainList[m_index].PageTotal;
	//		m_nCurPage	= m_PrtInfMainList[m_index].Lite.PageMin;
	//		GetDataOmit( m_index );
	//	}
	//	else{

	//		m_nCurPage++;
	//	}
	//}

	// 1ページ分の描画データを記憶
	m_icsVprn.EndPageJob( pDC );
}

////////////////////////////////////////////////////////////////////////////////
// GetPrtPageInfo()
//
// ■一括印刷時のページ管理情報を取得 ･･･ 部数印刷が正しく印刷できない対応
//
////////////////////////////////////////////////////////////////////////////////
void CprtJobFunction::GetPrtPageInfo(int nCurPage)
{
	int			nIndex = 1;					// 対象帳表
	int			nPage = 0;					// 対象ページ
	int			i;

	// プリントモジュールから指定されたページまで順に検索する
	for (i = 1; i <= nCurPage; i++) {
		// ページ＋１
		nPage = nPage + 1;

		// 次の帳表？
		if (nPage > m_PrtInfMainList[nIndex].PageTotal) {
			nIndex	= nIndex + 1;
			nPage	= 1;
		}
	}

	// ページ管理情報を取得
	m_index			= nIndex;
	// 2007.09.12 印刷開始ページが常に1になっていたため、一括印刷の
	//			　ページ範囲(開始)を反映するよう修正
	m_nCurPage		= ( m_PrtInfMainList[nIndex].Lite.PageMin - 1) + nPage;
}

////////////////////////////////////////////////////////////////////////////////
// JobFilePrintPreview()
//
// ■必ずビュークラスの、OnFilePrintPreview()内にて、呼び出すこと。
// 　それ以外では、呼び出してはならない！
//
////////////////////////////////////////////////////////////////////////////////
void CprtJobFunction::JobFilePrintPreview(CView* pView)
{
	m_icsVprn.DoPrintPreview( pView );	
}


////////////////////////////////////////////////////////////////////////////////
// CheckInit()
//
// ■印刷処理開始前に、実行環境がそろっているかチェックする
//
// 戻り値	：int				エラーコード		FUNCTION_OK：成功
//													FUNCTION_NG：失敗
//
////////////////////////////////////////////////////////////////////////////////
int CprtJobFunction::CheckInit(void)
{
// midori 190301 add -->
	int		sw=0;
// midori 190301 add <--
	// DBハンドルの有無
	if( m_pDB == NULL ){

		// 2010/08/30 実装クラス変更 → UCHIIMP ビルド対応
		// 2010/08/12 一括印刷 印刷時メッセージ非表示対応
		g_BatchPrt.ICSMessageBoxEx( PRT_ERR_NOTJOB );

		return FUNCTION_NG;
	}

	// ファイルの検索
	// 
	// 下位の処理で、同様にSCHファイル、およびイメージファイルのチェックを行っているが、
	// そこでエラーの処理を行うと動作的によろしくない。
	// （偶数回目の印刷処理実行時のみエラーメッセージが表示される）
	//
	// よって、印刷処理に入る前に、SCHファイルの有無をチェックする。
	CFileFind FileFind;

	// SCHファイルの検索
// midori 190301 del -->
	//if( !FileFind.FindFile( (m_strPath + PRT_FILE_SCH) ) ){
	//	CString strMsg;

	//	// 2010/08/30 実装クラス変更 → UCHIIMP ビルド対応
	//	// 2010/08/12 一括印刷 印刷時メッセージ非表示対応
	//	strMsg.Format( "%s\n%s", PRT_ERR_FILE_SCH, PRT_ERR_NOTJOB );
	//	g_BatchPrt.ICSMessageBoxEx( strMsg );

	//	return FUNCTION_NG;
	//}
// midori 190301 del <--
// midori 190301 add -->
	CString		strMsg;
// インボイス登録番号追加対応_23/10/20 del -->
	//if(bG_Kanso == TRUE)	{
// インボイス登録番号追加対応_23/10/20 del <--
// インボイス登録番号追加対応_23/10/20 add -->
	if(bG_InvNo == TRUE)	{
		if(!FileFind.FindFile((m_strPath + PRT_FILE_SCH_I)))	{
			strMsg.Format("%s\n%s", PRT_ERR_FILE_SCH, PRT_ERR_NOTJOB);
			g_BatchPrt.ICSMessageBoxEx(strMsg);
			return FUNCTION_NG;
		}
	}
	else if(bG_Kanso == TRUE)	{
// インボイス登録番号追加対応_23/10/20 add <--
		if( !FileFind.FindFile( (m_strPath + PRT_FILE_SCH_K) ) ){
			strMsg.Format( "%s\n%s", PRT_ERR_FILE_SCH, PRT_ERR_NOTJOB );
			g_BatchPrt.ICSMessageBoxEx( strMsg );
			return FUNCTION_NG;
		}
	}
	else	{
		if( !FileFind.FindFile( (m_strPath + PRT_FILE_SCH) ) )	{
			strMsg.Format( "%s\n%s", PRT_ERR_FILE_SCH, PRT_ERR_NOTJOB );
			g_BatchPrt.ICSMessageBoxEx( strMsg );
			return FUNCTION_NG;
		}
	}
// midori 190301 add <--

	// イメージファイルの検索		[2009/07/21 EMF対応 (TIF->IMG)]
// midori 190301 del -->
	//if(	!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_011) ) ||
	//	!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_021) ) ||
	//	!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_031) ) ||
	//	!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_040) ) ||
	//	!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_041) ) ||
	//	!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_042) ) ||
	//	!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_051) ) ||
	//	!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_061) ) ||
	//	!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_071) ) ||
	//	!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_081) ) ||
	//	!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_091) ) ||
	//	!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_100) ) ||
	//	!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_101) ) ||
	//	!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_102) ) ||
	//	!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_111) ) ||
	//	!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_121) ) ||
	//	!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_131) ) ||
	//	!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_140) ) ||
	//	!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_141) ) ||
	//	!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_142) ) ||
	//	!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_150) ) ||
	//	!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_151) ) ||
	//	!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_152) ) ||
	//	!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_153) ) ||
	//	!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_160) ) ||
	//	!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_161) ) ||
	//	!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_171) ) ||
	//	!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_181) ) ||
	//	!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_CVR) )
	//	//!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_CVR_S))
	//	){
// midori 190301 del <--
// midori 190301 add -->
	sw = 0;
// インボイス登録番号追加対応_23/10/23 del -->
	//if(bG_Kanso == TRUE)	{
// インボイス登録番号追加対応_23/10/23 del <--
// インボイス登録番号追加対応_23/10/23 add -->
	if(bG_InvNo == TRUE) {
		if( !FileFind.FindFile((m_strPath + PRT_FILE_IMG_011_K)) ||
			!FileFind.FindFile((m_strPath + PRT_FILE_IMG_021_I)) ||
			!FileFind.FindFile((m_strPath + PRT_FILE_IMG_031_I)) ||
			!FileFind.FindFile((m_strPath + PRT_FILE_IMG_040_I)) ||
			!FileFind.FindFile((m_strPath + PRT_FILE_IMG_041_I)) ||
			!FileFind.FindFile((m_strPath + PRT_FILE_IMG_042_I)) ||
			!FileFind.FindFile((m_strPath + PRT_FILE_IMG_051_K)) ||
			!FileFind.FindFile((m_strPath + PRT_FILE_IMG_061_K)) ||
			!FileFind.FindFile((m_strPath + PRT_FILE_IMG_071_I)) ||
			!FileFind.FindFile((m_strPath + PRT_FILE_IMG_081_I)) ||
			!FileFind.FindFile((m_strPath + PRT_FILE_IMG_091_I)) ||
			!FileFind.FindFile((m_strPath + PRT_FILE_IMG_100_I)) ||
			!FileFind.FindFile((m_strPath + PRT_FILE_IMG_101_I)) ||
			!FileFind.FindFile((m_strPath + PRT_FILE_IMG_102_I)) ||
			!FileFind.FindFile((m_strPath + PRT_FILE_IMG_111_K)) ||
			!FileFind.FindFile((m_strPath + PRT_FILE_IMG_121_I)) ||
			!FileFind.FindFile((m_strPath + PRT_FILE_IMG_131_K)) ||
			!FileFind.FindFile((m_strPath + PRT_FILE_IMG_140_K)) ||
			!FileFind.FindFile((m_strPath + PRT_FILE_IMG_141_K)) ||
			!FileFind.FindFile((m_strPath + PRT_FILE_IMG_142_K)) ||
			!FileFind.FindFile((m_strPath + PRT_FILE_IMG_150_I)) ||
			!FileFind.FindFile((m_strPath + PRT_FILE_IMG_151_I)) ||
			!FileFind.FindFile((m_strPath + PRT_FILE_IMG_152_I)) ||
			!FileFind.FindFile((m_strPath + PRT_FILE_IMG_153_I)) ||
			!FileFind.FindFile((m_strPath + PRT_FILE_IMG_160_I)) ||
			!FileFind.FindFile((m_strPath + PRT_FILE_IMG_161_I)) ||
			!FileFind.FindFile((m_strPath + PRT_FILE_IMG_162_I)) ||
			!FileFind.FindFile((m_strPath + PRT_FILE_IMG_171_I)) ||
			!FileFind.FindFile((m_strPath + PRT_FILE_IMG_181)) ||
			!FileFind.FindFile((m_strPath + PRT_FILE_IMG_CVR)) 
			) {
			sw = 1;
		}
	}
	else if(bG_Kanso == TRUE)	{
// インボイス登録番号追加対応_23/10/23 add <--
		if(	!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_011_K) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_021_K) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_031_K) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_040_K) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_041_K) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_042_K) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_051_K) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_061_K) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_071_K) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_081_K) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_091_K) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_100_K) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_101_K) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_102_K) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_111_K) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_121_K) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_131_K) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_140_K) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_141_K) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_142_K) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_150_K) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_151_K) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_152_K) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_153_K) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_160_K) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_161_K) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_162_K) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_171) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_181) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_CVR) )
			)	{
			sw = 1;
		}
	}
	else	{
		if(	!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_011) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_021) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_031) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_040) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_041) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_042) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_051) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_061) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_071) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_081) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_091) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_100) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_101) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_102) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_111) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_121) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_131) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_140) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_141) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_142) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_150) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_151) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_152) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_153) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_160) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_161) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_171) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_181) ) ||
			!FileFind.FindFile( (m_strPath + PRT_FILE_IMG_CVR) )
			)	{
			sw = 1;
		}
	}
	if(sw != 0)	{
// midori 190301 del <--

			CString strMsg;

			// 2010/08/30 実装クラス変更 → UCHIIMP ビルド対応
			// 2010/08/12 一括印刷 印刷時メッセージ非表示対応
			strMsg.Format( "%s\n%s", PRT_ERR_FILE_IMG, PRT_ERR_NOTJOB );		// [2009/07/21 EMF対応 (TIF->IMG)]
			g_BatchPrt.ICSMessageBoxEx( strMsg );
			
			return FUNCTION_NG;
	}

	return FUNCTION_OK;
}

////////////////////////////////////////////////////////////////////////////////
// InitProp()
//
// ■変数、配列、構造体の初期化
//
////////////////////////////////////////////////////////////////////////////////
void CprtJobFunction::InitProp(void)
{
	// メンバ変数
	m_eJob					= JobNop;
	m_index					= 1;
	m_lFncFlag				= 0;
	m_nPageSum				= 0;
	m_nPageAllSum			= 0;
	m_nCurPage				= 0;
// midori 180101 del -->
//	m_nOutGenzai			= 0;
// midori 180101 del <--
// midori 160608 add -->
	m_nOutZero				= 0;
// midori 160608 add <--

	// 印刷指定パケット
	ZeroMemory( &m_icsPset, sizeof( m_icsPset ) );

	// データ出力バッファ
	ZeroMemory( m_icsBufBox, sizeof( m_icsBufBox ) );

	// スケジュールリスト
	ZeroMemory( m_nSchList, sizeof( m_nSchList ) );
	m_nSchList[0] = SchSeqCmn;	// 共通（必須）

	// 出力リスト
	ZeroMemory( m_PrtInfMainList, sizeof(m_PrtInfMainList ) );

	// 帳表タイトルリスト
	for( int i = 0; i < UC_ID_FORMNO_MAX; i++ ){

		m_PrtInfTitleList[i].FormSeq		= NULL;
		m_PrtInfTitleList[i].Title.Empty();
		m_PrtInfTitleList[i].OpTitleKnFg	= NULL;
		m_PrtInfTitleList[i].OpTitleKn		= NULL;
	}

	// 省略文字リスト
	for( int j = 0; j < PRT_MAX_OMIT; j++ ){

		m_PrtInfOmitList1[j].ItemSeq	= NULL;
		m_PrtInfOmitList1[j].Omit		= NULL;
		m_PrtInfOmitList1[j].OmitStr.Empty();
		m_PrtInfOmitList2[j].ItemSeq	= NULL;
		m_PrtInfOmitList2[j].Omit		= NULL;
		m_PrtInfOmitList2[j].OmitStr.Empty();
		m_PrtInfOmitList3[j].ItemSeq	= NULL;
		m_PrtInfOmitList3[j].Omit		= NULL;
		m_PrtInfOmitList3[j].OmitStr.Empty();
	}

	// 共通情報
	m_PrtInfCommon.KikanStart		= NULL;
	m_PrtInfCommon.KikanEnd			= NULL;
// midori 153812 del ------------------>
//	m_PrtInfCommon.KikanEndPage		= NULL;
// midori 153812 del <------------------
	m_PrtInfCommon.Ki				= NULL;
	m_PrtInfCommon.OutKi			= NULL;
	m_PrtInfCommon.OutCoName		= NULL;
	m_PrtInfCommon.OutKikan			= NULL;
	m_PrtInfCommon.OutKingaku		= NULL;
	m_PrtInfCommon.OutPage			= NULL;
	m_PrtInfCommon.CoRekinen		= NULL;
	m_PrtInfCommon.CoName.Empty();
	m_PrtInfCommon.CoAddr1.Empty();
	m_PrtInfCommon.CoAddr2.Empty();
// midori 180101 add -->
	m_PrtInfCommon.OutGenzai		= NULL;
// midori 180101 add <--
// midori 181101 add -->
	m_PrtInfCommon.StartGen			= NULL;
	m_PrtInfCommon.EndGen			= NULL;
// midori 181101 add <--
//	m_PrtInfCommon.HjNo_zvol.Empty();
//	m_PrtInfCommon.HjNo_corec.Empty();
// midori 200101 add -->
	m_PrtInfCommon.OutGengo			= NULL;
// midori 200101 add <--

	// データボックス
// インボイス登録番号追加対応_23/10/23 del -->
	//for( int k = 0; k < PRT_MAX_COLUMN; k++ ){
// インボイス登録番号追加対応_23/10/23 del <--
// インボイス登録番号追加対応_23/10/23 add -->
	int		max = 0;
	if(bG_InvNo == TRUE)	max = PRT_MAX_COLUMN_I;
	else					max = PRT_MAX_COLUMN;
	for(int k=0; k<max; k++)	{
// インボイス登録番号追加対応_23/10/23 add <--
		StrArrayEmpty( &m_PrtDataBox[k].Data[0], sizeof( m_PrtDataBox[k].Data ) );
	}
}

////////////////////////////////////////////////////////////////////////////////
// PrintDataPageOne()
//
// ■1ページ分のデータを出力する
//
// 引数		：int				index				出力リストのインデックス
//			：int				nPage				ページ番号
//			：CDC*				pDC					デバイスコンテキスト
//
// 戻り値	：int				エラーコード		FUNCTION_OK：成功
//													FUNCTION_NG：失敗
//
////////////////////////////////////////////////////////////////////////////////
int CprtJobFunction::PrintDataPageOne(int index, int nPage, CDC* pDC)
{
	// データ出力バッファの初期化
	ZeroMemory( m_icsBufBox, sizeof( m_icsBufBox ) );

	// データボックスの初期化
// インボイス登録番号追加対応_23/10/23 del -->
//	for( int i = 0; i < PRT_MAX_COLUMN; i++ ){
// インボイス登録番号追加対応_23/10/23 del <--
// インボイス登録番号追加対応_23/10/23 add -->
	int		max = 0;
	if(bG_InvNo == TRUE)	max = PRT_MAX_COLUMN_I;
	else					max = PRT_MAX_COLUMN;
	for(int i=0; i<max; i++)	{
// インボイス登録番号追加対応_23/10/23 add <--
		StrArrayEmpty( &m_PrtDataBox[i].Data[0], sizeof( m_PrtDataBox[i].Data ) );
	}

	// 
	int nSchSeq = GetDataPageOne( index, nPage );

// 2008.03.04：PDF出力でタイトル出力できないため、版下→タイトルの順で出力する
//	if( nSchSeq != SchSeqCvr ){
//
//		// 1ページ分の共通データを出力
//		PrintDataCommon( index, nPage, pDC );
//	}

	// 版下、およびデータ出力開始座標のオフセットを設定
	_xy_offs( index, 0, 0, 0 );		// 版下の座標位置
	_xy_offs( index, 1, 0, 0 );		// データの座標位置

	// スケジュールファイルによる、1フィールドの出力
//	return _xprn( nSchSeq, 1, m_icsBufBox, 0, pDC, 0 );
	int nRet;
	try{
		nRet = _xprn( nSchSeq, 1, m_icsBufBox, 1, pDC, 0 );
	}
	catch( ... ){

		// MEMO：
		// SCHファイルに不備（必要なコンマが入力されていない）がある場合に、例外発生
		// 
		// try-catchにて、アプリエラーは抑えられるが、catch後の処理がされず、
		// そのままアプリが終了する。
		// 対策はあるか？

		return FUNCTION_NG;
	}
	
	// PDF出力対応にて、版下出力後に共通データを出力する
	if( nSchSeq != SchSeqCvr ){

		// 1ページ分の共通データを出力
		PrintDataCommon( index, nPage, pDC );
	}

	return nRet;
}

////////////////////////////////////////////////////////////////////////////////
// PrintDataCommon()
//
// ■1ページ分の共通データを出力する
//
// 引数		：int				index				出力リストのインデックス
//			：int				nPage				ページ番号
//			：CDC*				pDC					デバイスコンテキスト
//
// 戻り値	：int				エラーコード		FUNCTION_OK：成功
//													FUNCTION_NG：失敗
//
////////////////////////////////////////////////////////////////////////////////
int CprtJobFunction::PrintDataCommon(int index, int nPage, CDC* pDC)
{
	PBUF	icsBuf[FieldCmnMax];
	CString	strCoName	= _T( "" );
	CString	strKikan	= _T( "" );
	CString	strPage		= _T( "" );
	CString strHjNo		= _T( "" );
// midori 153812 add ------------------>
	CString cst1		= _T("");		// 年
	CString cst2		= _T("");		// 月
	CString cst3		= _T("");		// 日
// midori 153812 add <------------------
// midori 181101 add -->
	CString cst			= _T("");
	CString format		= _T("");
// midori 181101 add <--

	ZeroMemory( &icsBuf, sizeof( icsBuf ) );

	// 会社名
	if ( m_PrtInfCommon.OutCoName != 0 ){

		strCoName = m_clsFunc.DeleteRightSpace( m_PrtInfCommon.CoName );

		icsBuf[FieldCmnCoName].PITM[0] = (LPSTR)(LPCTSTR)strCoName;
	}

	// 法人番号
//	strHjNo = GetHjNo(m_icsPset.psSigCoprn, m_PrtInfCommon);	// strHjNoをかませないといけない！
//	icsBuf[FieldCmnHjNo].PITM[0] = (LPSTR)(LPCSTR)strHjNo;


	//int h_flg12 = m_icsPset.psSigCoprn & BIT_D12;
	//int h_flg13 = m_icsPset.psSigCoprn & BIT_D13;
	//int	h_flg = !(h_flg13) && h_flg12; //　法人番号出力有効h_flg12　+　出力をしないh_flg13　に注意
	//if( h_flg ){
	//	CICSMNSub8 mnsub8;
	//	if( m_icsPset.psSigCoprn & BIT_D1){	// 顧問から法人番号を取得
	//		strHjNo = mnsub8.MakePrintHojinNo((LPSTR)(LPCSTR)m_PrtInfCommon.HjNo_corec, BIT_D0);
	//		icsBuf[FieldCmnHjNo].PITM[0] = (LPSTR)(LPCSTR)strHjNo;
	//	}
	//	else{
	//		//char f_dmbuf[128] = {};
	//		//icsBuf[FieldCmnHjNo].PITM[0] = (LPSTR)(LPCSTR)mnsub8.MakePrintHojinNo((LPSTR)(LPCSTR)m_PrtInfCommon.HjNo_zvol, BIT_D0);
	//		//strcpy_s(icsBuf[FieldCmnHjNo].PITM[0], m_PrtInfCommon.HjNo_zvol.GetLength(), m_PrtInfCommon.HjNo_zvol);
	//		strHjNo = mnsub8.MakePrintHojinNo((LPSTR)(LPCSTR)m_PrtInfCommon.HjNo_zvol, BIT_D0);
	//		icsBuf[FieldCmnHjNo].PITM[0] = (LPSTR)(LPCSTR)strHjNo;
	//	}
	//}

	// 決算期間
	if( m_PrtInfCommon.OutKikan != 0 ){

// midori 153812 del ------------------>
//		CString format = PRT_FORMAT_DATE_F1;
//		if(m_PrtInfCommon.CoRekinen != 0)	format = PRT_FORMAT_DATE_F2;
//
//		strKikan = ConvertDate( m_PrtInfCommon.KikanEndPage, format, SchSeqCvr ) + PRT_FORMAT_GENZAI;
//		icsBuf[FieldCmnKikan].PITM[0] = (LPSTR)(LPCTSTR)strKikan;
// midori 153812 del <------------------
// midori 153812 add ------------------>
// midori 181101 del -->
//		CString format = _T("平成%2s年%2s月%2s日");
// midori 181101 del <--
// midori 181101 add -->
		if(IsValidKaigen() == FALSE) {
			format = _T("平成%2s年%2s月%2s日");
		}
		else {
			format = GetGengo(m_PrtInfCommon.EndGen) + _T("%2s年%2s月%2s日");
		}
// midori 181101 add <--
		if(m_PrtInfCommon.CoRekinen != 0)	format = _T("%4s年%2s月%2s日");

		// 年
		cst1.Empty();
		if((m_PrtInfCommon.KikanEnd / 10000) > 0)	{
// midori UC_0027 del -->
//			cst1.Format(_T("%d"),(m_PrtInfCommon.KikanEnd / 10000));
// midori UC_0027 del <--
// midori UC_0027 add -->
			if(m_PrtInfCommon.CoRekinen == 0 && (m_PrtInfCommon.KikanEnd / 10000) == 1) {
				// 和暦かつ年度が1年の場合、「元年」を出力する
				cst1 = _T("元");
			}
			else {
				cst1.Format(_T("%d"),(m_PrtInfCommon.KikanEnd / 10000));
			}
// midori UC_0027 add <--
		}
		// 月
		cst2.Empty();
		if(((m_PrtInfCommon.KikanEnd % 10000) / 100) > 0)	{
			cst2.Format(_T("%d"),((m_PrtInfCommon.KikanEnd % 10000) / 100));
		}
		// 日
		cst3.Empty();
		if((m_PrtInfCommon.KikanEnd % 100) > 0)	{
			cst3.Format(_T("%d"),(m_PrtInfCommon.KikanEnd % 100));
		}

		strKikan.Format(format + PRT_FORMAT_GENZAI,cst1,cst2,cst3);
		icsBuf[FieldCmnKikan].PITM[0] = (LPSTR)(LPCTSTR)strKikan;
// midori 153812 add <------------------
	}

	// ページ番号
	if( m_PrtInfCommon.OutPage != 0 ){

		strPage.Format( PRT_FORMAT_PAGE, nPage, m_PrtInfMainList[index].PageAll );
		//strPage.Format( PRT_FORMAT_PAGE, m_icsVprn.GetCurPageNo(), m_PrtInfMainList[index].PageAll );
		icsBuf[FieldCmnPageNum].PITM[0] = (LPSTR)(LPCTSTR)strPage;
	}
	
	// 帳表タイトル
	CString	strTitle = GetTitleOfKnName( index, nPage );
// midori 190301 del -->
	//icsBuf[FieldCmnTitle].PITM[0] = (LPSTR)(LPCTSTR)strTitle; 
// midori 190301 del <--
// midori 190301 add -->
	CString	strTitleDn	= _T( "" );	// 下段のタイトル
	int		nFormSeq	= m_PrtInfMainList[index].Lite.FormSeq;
	if(bG_Kanso == FALSE)	{
		icsBuf[FieldCmnTitle].PITM[0] = (LPSTR)(LPCTSTR)strTitle; 
	}
// midori 190301 add <--

	int	nFormType	= m_PrtInfMainList[index].FormType;

	// 出力形式「通常」の場合
	// 下段のタイトルを印字する
	if( nFormType == ID_FORMTYPE_NORMAL ){

// midori 190301 del -->
		//int		nFormSeq	= m_PrtInfMainList[index].Lite.FormSeq;
		//CString	strTitleDn	= _T( "" );	// 下段のタイトル
// midori 190301 del <--

		switch( nFormSeq ){

			case ID_FORMNO_041:
			case ID_FORMNO_042:

				strTitleDn = GetTitleFromList( ID_FORMNO_042 );
// midori 190301 del -->
				//icsBuf[FieldCmnDnTitle040].PITM[0] = (LPSTR)(LPCTSTR)strTitleDn;
// midori 190301 del <--
// midori 190301 add -->
				if(bG_Kanso == FALSE)	{
					icsBuf[FieldCmnDnTitle040].PITM[0] = (LPSTR)(LPCTSTR)strTitleDn;
				}
// midori 190301 add <--
				break;

			case ID_FORMNO_101:
			case ID_FORMNO_102:

				strTitleDn = GetTitleFromList( ID_FORMNO_102 );
// midori 190301 del -->
				//icsBuf[FieldCmnDnTitle100].PITM[0] = (LPSTR)(LPCTSTR)strTitleDn;
// midori 190301 del <--
// midori 190301 add -->
				if(bG_Kanso == FALSE)	{
					icsBuf[FieldCmnDnTitle100].PITM[0] = (LPSTR)(LPCTSTR)strTitleDn;
				}
// midori 190301 add <--
				break;

			case ID_FORMNO_151:
			case ID_FORMNO_152:
			case ID_FORMNO_153:

				strTitleDn = GetTitleFromList( ID_FORMNO_153 );
// midori 190301 del -->
				//icsBuf[FieldCmnDnTitle150].PITM[0] = (LPSTR)(LPCTSTR)strTitleDn;
// midori 190301 del <--
// midori 190301 add -->
				if(bG_Kanso == FALSE)	{
					icsBuf[FieldCmnDnTitle150].PITM[0] = (LPSTR)(LPCTSTR)strTitleDn;
				}
// midori 190301 add <--
				break;

			default:
				break;
		}
	}

	int nRet;
	try{

		nRet = _xprn( SchSeqCmn, 1, icsBuf, 1, pDC, 0 );
	}
	catch( ... ){

		// MEMO：
		// SCHファイルに不備（必要なコンマが入力されていない）がある場合に、例外発生
		// 
		// try-catchにて、アプリエラーは抑えられるが、catch後の処理がされず、
		// そのままアプリが終了する。
		// 対策はあるか？

		return FUNCTION_NG;
	}
// midori 190301 add -->
	// 簡素化様式の場合、タイトルだけはゴシック体で出力する
	if(bG_Kanso == TRUE)	{
		SF_PAR	sp;
		long	y=0;
// 158024 add -->
		int		reset=0;
// 158024 add <--
		if(strTitle.IsEmpty() == FALSE)	{
			ZeroMemory(&sp,sizeof(sp));
			sp.sf_fcsg = SFp_xl | SFp_yc;	// 左詰め、縦センターリング
			sp.sf_ksiz = Gothic;			// 全角10.8ポイント
			m_icsVprn.DrawString(0,1400,1800,strTitle,&sp);
// 158024 add -->
			reset=1;
// 158024 add <--
		}
		if(strTitleDn.IsEmpty() == FALSE)	{
			ZeroMemory(&sp,sizeof(sp));
			sp.sf_fcsg = SFp_xl | SFp_yc;	// 左詰め、縦センターリング
			sp.sf_ksiz = Gothic;			// 全角10.8ポイント
			switch(nFormSeq)	{
				case ID_FORMNO_041:
				case ID_FORMNO_042:
// 修正No.168367 del -->
					//y=14300;
// 修正No.168367 del <--
// 修正No.168367 add -->
					if(bG_InvNo == TRUE)	y=14500;
					else					y=14300;
// 修正No.168367 add <--
					break;
				case ID_FORMNO_101:
				case ID_FORMNO_102:
					y=18900;
					break;
				default:
// インボイス登録番号追加対応_23/11/06 del -->
					//y=21150;
// インボイス登録番号追加対応_23/11/06 del <--
// インボイス登録番号追加対応_23/11/06 add -->
					if(bG_InvNo == TRUE)	y=21350;
					else					y=21150;
// インボイス登録番号追加対応_23/11/06 add <--
					break;
			}
			m_icsVprn.DrawString(0,1400,y,strTitleDn,&sp);
// 158024 add -->
			reset=1;
// 158024 add <--
		}
// 158024 add -->
		if(reset!=0)	{
// 158024 add <--
		// スケジュールを使用して出力している項目の中にゴシックの設定を引き継ぐものがあったので
		// 下記２行で明朝に戻しています
		sp.sf_ksiz = FNT108z;
		m_icsVprn.DrawString(0,0,0," ",&sp);
// 158024 add -->
		}
// 158024 add <--
	}
// midori 190301 add <--
	
	return nRet;
}

////////////////////////////////////////////////////////////////////////////////
// CheckFileSch()
//
// ■スケジュール ファイルの正当性を調べる
//
// 引数		：int				nErrFlag			エラーフラグ
//
// 戻り値	：int				エラーコード		FUNCTION_OK：成功
//													FUNCTION_NG：失敗
//
/////////////////////////////////////////////////////////////////////////////////
int CprtJobFunction::CheckFileSch(int nErrFlag)
{
	// 正しくなければ、処理を終了
	switch( nErrFlag ){

	case 1:

		ShowErrMsg( PRT_ERR_SCH_1 );
		return FUNCTION_NG;

	case 2:

		ShowErrMsg( PRT_ERR_SCH_2 );
		return FUNCTION_NG;

	case 3:

		ShowErrMsg( PRT_ERR_SCH_3 );
		return FUNCTION_NG;

	case 4:

		ShowErrMsg( PRT_ERR_SCH_4 );
		return FUNCTION_NG;

	case 5:

		ShowErrMsg( PRT_ERR_SCH_5 );
		return FUNCTION_NG;

	case 6:

		ShowErrMsg( PRT_ERR_SCH_6 );
		return FUNCTION_NG;

	default:
		// 座標モードの設定
		// マッピングモードに設定
		if( m_icsVprn.SetCoMode( 1 ) != 0 ){

			ShowErrMsg( PRT_ERR_SCH_7 );

			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}

////////////////////////////////////////////////////////////////////////////////
// MakeListSch()
//
// ■出力リストに格納されている様式シーケンス番号を、
// 　先頭から順にスケジュールリストに格納する。
//
// ※以下の帳表に関しては、格納する値を変更する。
// 　　１．出力形式が選択可能な帳表に対して、出力形式が「通常」のとき
// 　　２．帳表⑯
// 　　３．帳表⑰
// 　　４．帳表⑱
//
// ※スケジュールファイルに格納される値は、様式シーケンス番号を基にしているが、
// 　スケジュールファイル内でのシーケンスであり、様式シーケンス番号とイコールでない
// 　ことに注意！
//
// 引数		：int				index				出力リストのインデックス
//
// 戻り値	：int				エラーコード		FUNCTION_OK：成功
//													FUNCTION_NG：失敗
//
/////////////////////////////////////////////////////////////////////////////////
int CprtJobFunction::MakeListSch(int index)
{
	int nSchSeq = m_PrtInfMainList[index].Lite.FormSeq;

	// 出力形式「通常」
	if( m_PrtInfMainList[index].FormType == 0 ){

		switch( nSchSeq ){

		case ID_FORMNO_041:
		case ID_FORMNO_042:
			
			nSchSeq = SchSeq040Nml;
			break;

		case ID_FORMNO_101:
		case ID_FORMNO_102:

			nSchSeq = SchSeq100Nml;
			break;

		case ID_FORMNO_141:
		case ID_FORMNO_142:

			nSchSeq = SchSeq140Nml;
			break;

		case ID_FORMNO_151:
		case ID_FORMNO_152:
		case ID_FORMNO_153:

			nSchSeq = SchSeq150Nml;
			break;

		case ID_FORMNO_161:
		case ID_FORMNO_162:

			nSchSeq = SchSeq160Nml;
			break;

		default:
			break;
		}
	}

	switch( nSchSeq ){

	case ID_FORMNO_161:
	case ID_FORMNO_162:
// midori 190301 del -->
		//nSchSeq = SchSeq160Cmn;
// midori 190301 del <--
// midori 190301 add -->
		if(bG_Kanso == TRUE)	{
			if(nSchSeq == ID_FORMNO_161)	{
				nSchSeq = SchSeq160Cmn;
			}
			else	{
				nSchSeq = SchSeq161Cmn;
			}
		}
		else	{
			nSchSeq = SchSeq160Cmn;
		}
// midori 190301 add <--
		break;

	case ID_FORMNO_171:
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

		nSchSeq = SchSeq170Cmn;
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

		nSchSeq = SchSeq180Cmn;
		break;

	default:
		break;
	}

	// スケジュールリストに様式シーケンス番号を格納
	m_nSchList[index] = nSchSeq;

	return FUNCTION_OK;
}

////////////////////////////////////////////////////////////////////////////////
// ShowErrMsg()
//
// ■エラーメッセージの表示
//
// 引数		：CString			strMsg				メッセージ文字列
//			：BOOL				bFgProgEnd			TRUE ：プログラムの終了
//													FALSE：印刷処理の終了
//
////////////////////////////////////////////////////////////////////////////////
void CprtJobFunction::ShowErrMsg(CString strMsg, BOOL bFgProgEnd /*= FALSE*/)
{
	if( bFgProgEnd ){

		// プログラムの終了
		ICSExit( 0, (LPSTR)(LPCTSTR)strMsg );
	}
	else{

		// MEMO：
		// 1回目の実行時に、エラーメッセージが表示されないようだが？

		// 印刷中のアプリケーション エラーによる出力中断（メッセージボックスの表示）
		//	2010/08/31 CVPrint::ErrorAbort() 実行後の一括印刷終了対応
		//m_icsVprn.ErrorAbort( 1, strMsg );
		CVPrintErrorAbortPrc( &m_icsVprn, 1, strMsg );
	}
}

////////////////////////////////////////////////////////////////////////////////
// SetDataBuff()
//
// ■文字列データのポインタをデータ出力バッファに格納する。
//
// 引数		：int				nColumn				列数
//			：int				nRow				行数
//
////////////////////////////////////////////////////////////////////////////////
void CprtJobFunction::SetDataBuffCover(int nColumn, int nRow)
{
	for( int i = 0; i < nColumn; i++ ){

		for( int j = 0; j < nRow; j++ ){

			m_icsBufBox[i].PITM[j] = (LPSTR)(LPCTSTR)m_PrtDataBox[i].Data[j];
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// SetDataBuff()
//
// ■文字列データのポインタをデータ出力バッファに格納する。
//
// 引数		：CprtJobFunction*  prtObject			子クラスのポインタ
//			：int				index				出力インデックス
//
////////////////////////////////////////////////////////////////////////////////
void CprtJobFunction::SetDataBuff(CprtJobFunction* prtObject, int index)
{
	int nColumn = prtObject->m_nColumn;
	int nRow	= prtObject->m_nRowMax;

// インボイス登録番号追加対応_23/10/23 del -->
	//for( int i = 0; i < PRT_MAX_COLUMN; i++ ){
// インボイス登録番号追加対応_23/10/23 del <--
// インボイス登録番号追加対応_23/10/23 add -->
	int		max = 0;

	if(bG_InvNo == TRUE)	max = PRT_MAX_COLUMN_I;
	else					max = PRT_MAX_COLUMN;
	for(int i=0; i<max; i++)	{
// インボイス登録番号追加対応_23/10/23 add <--
		for(int j = 0;j<PRT_MAX_ROW;j++)
			m_PrtDataBox[i].Data[j] = prtObject->m_PrtDataBox[i].Data[j];
	}

	for( int i = 0; i < nColumn; i++ ){
		for( int j = 0; j < nRow; j++ ){

			m_icsBufBox[i].PITM[j] = (LPSTR)(LPCSTR)m_PrtDataBox[i].Data[j];
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// GetDataInitVal()
//
// ■初期情報の取得
//
// 戻り値	：int				エラーコード		FUNCTION_OK：成功
//													FUNCTION_NG：失敗
//
////////////////////////////////////////////////////////////////////////////////
int CprtJobFunction::GetDataInitVal(void)
{
	// ページ設定
	m_nPageSum	= m_PrtInfMainList[1].PageTotal;
	m_nCurPage	= m_PrtInfMainList[1].Lite.PageMin;

// midori 180101 del -->
//// midori 152109 cor -->
//	//// 2011/12/22 表紙の会社名、住所、決算期間を決算書の既定値に変更のため廃止
//	//// zvolume
//	////GetDbDataTableZVolume();
//// ---------------------
//	// zvolume
//	GetDbDataTableZVolume();
//// midori 152109 cor <--
// midori 180101 del <--

	// cpdata
	GetDbDataTableCpdata();

	// prfkin
	GetDbDataTablePrfkin();

	// uc_inf_main
	//GetDbDataTableUcInfMain();

	// 全帳表タイトルの取得
	GetDbDataTitleAll();

	// 省略文字情報の取得
	GetDataOmit( 1 );

	return FUNCTION_OK;
}

////////////////////////////////////////////////////////////////////////////////
// GetDataOmit()
//
// ■省略文字情報の取得
//
// 引数		：int				index				出力リストのインデックス
//
// 戻り値	：int				エラーコード		FUNCTION_OK：成功
//													FUNCTION_NG：失敗
//
////////////////////////////////////////////////////////////////////////////////
int CprtJobFunction::GetDataOmit(int index)
{
	int nFormSeq = m_PrtInfMainList[index].Lite.FormSeq;

	// その他科目の内訳書
	switch( nFormSeq ){

		case ID_FORMNO_171:
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
			
			GetDbDataTableUcInfSubOmit( nFormSeq, m_PrtInfOmitList1 );
			return FUNCTION_OK;

		default:
			break;
	}

	switch( m_PrtInfMainList[index].FormTag ){

	// 帳表○-1
	case TagFirst:

		GetDbDataTableUcInfSubOmit( nFormSeq, m_PrtInfOmitList1 );

		// 出力形式「通常」
		switch( nFormSeq ){

		case ID_FORMNO_041:

			GetDbDataTableUcInfSubOmit( ID_FORMNO_042, m_PrtInfOmitList2 );
			break;

		case ID_FORMNO_101:

			GetDbDataTableUcInfSubOmit( ID_FORMNO_102, m_PrtInfOmitList2 );
			break;

		case ID_FORMNO_141:

			GetDbDataTableUcInfSubOmit( ID_FORMNO_142, m_PrtInfOmitList2 );
			break;

		case ID_FORMNO_151:

			GetDbDataTableUcInfSubOmit( ID_FORMNO_152, m_PrtInfOmitList2 );
			GetDbDataTableUcInfSubOmit( ID_FORMNO_153, m_PrtInfOmitList3 );
			break;

		case ID_FORMNO_161:

			GetDbDataTableUcInfSubOmit( ID_FORMNO_162, m_PrtInfOmitList2 );
			break;

		default:
			break;
		}
		break;

	// 帳表○-2
	case TagSecond:

		GetDbDataTableUcInfSubOmit( nFormSeq, m_PrtInfOmitList2 );

		// 出力形式「通常」
		switch( nFormSeq ){

		case ID_FORMNO_042:
			GetDbDataTableUcInfSubOmit( ID_FORMNO_041, m_PrtInfOmitList1 );
			break;

		case ID_FORMNO_102:
			GetDbDataTableUcInfSubOmit( ID_FORMNO_101, m_PrtInfOmitList1 );
			break;

		case ID_FORMNO_142:
			GetDbDataTableUcInfSubOmit( ID_FORMNO_141, m_PrtInfOmitList1 );
			break;

		case ID_FORMNO_152:
			GetDbDataTableUcInfSubOmit( ID_FORMNO_151, m_PrtInfOmitList1 );
			GetDbDataTableUcInfSubOmit( ID_FORMNO_153, m_PrtInfOmitList3 );
			break;

		case ID_FORMNO_162:
			GetDbDataTableUcInfSubOmit( ID_FORMNO_161, m_PrtInfOmitList1 );
			break;

		default:
			break;
		}
		break;

	// 帳表○-3
	case TagThird:

		GetDbDataTableUcInfSubOmit( nFormSeq, m_PrtInfOmitList3 );

		// 出力形式「通常」
		switch( nFormSeq ){

		case ID_FORMNO_153:
			GetDbDataTableUcInfSubOmit( ID_FORMNO_151, m_PrtInfOmitList1 );
			GetDbDataTableUcInfSubOmit( ID_FORMNO_152, m_PrtInfOmitList2 );
			break;

		default:
			break;
		}
		break;

	default:
		break;
	}

	return FUNCTION_OK;
}

////////////////////////////////////////////////////////////////////////////////
// GetDataPageOne()
//
// ■帳表1ページ分のデータを取得し、データボックスに格納する。
//
// 引数		：int				index				出力リストのインデックス
//			：int				nPage				ページ番号
//
// 戻り値	：int				スケジュールシーケンス番号
//
////////////////////////////////////////////////////////////////////////////////
int CprtJobFunction::GetDataPageOne(int index, int nPage)
{
	int nSchList	= m_nSchList[index];
	int nFormSeq	= m_PrtInfMainList[index].Lite.FormSeq;
	CprtJobFunction* prtJob = NULL;

	// データボックスの初期化
// インボイス登録番号追加対応_23/10/23 del -->
	//for( int i = 0; i < PRT_MAX_COLUMN; i++ ){
// インボイス登録番号追加対応_23/10/23 del <--
// インボイス登録番号追加対応_23/10/23 add -->
	int		max = 0;

	if(bG_InvNo == TRUE)	max = PRT_MAX_COLUMN_I;
	else					max = PRT_MAX_COLUMN;
	for(int i=0; i<max; i++)	{
// インボイス登録番号追加対応_23/10/23 add <--
		StrArrayEmpty( &m_PrtDataBox[i].Data[0], sizeof( m_PrtDataBox[i].Data ) );
	}

	// 表紙
	if( nSchList == SchSeqCvr ){
		// 
		//SetNprtParTgInf(&m_icsPset);
		// 
		GetDataCover();
	}
	// 帳表
	else{
// midori 160608 cor -->
		//// 様式別の印刷オブジェクト取得
		//prtJob = virGetPrintObject( nSchList, nFormSeq );

		//// 様式別の印刷データ取得
		//prtJob->GetDbDataSheet( nPage, this, m_PrtInfMainList[index].Option, nFormSeq);

		//// 印刷データをバッファにセット
		//SetDataBuff( prtJob, index );

		//// 印刷オブジェクト破棄
		//delete prtJob;
// ---------------------
		if(m_nOutZero == 0)	{
			// 様式別の印刷オブジェクト取得
			prtJob = virGetPrintObject( nSchList, nFormSeq );

			// 様式別の印刷データ取得
			prtJob->GetDbDataSheet( nPage, this, m_PrtInfMainList[index].Option, nFormSeq);

			// 印刷データをバッファにセット
			SetDataBuff( prtJob, index );

			// 印刷オブジェクト破棄
			delete prtJob;
		}
// midori 160608 cor <--
	}

	return nSchList;
}

////////////////////////////////////////////////////////////////////////////////
// GetDataCover()
//
// ■表紙のデータを取得し、データボックスに格納する。
//
// 戻り値	：int				エラーコード		FUNCTION_OK：成功
//													FUNCTION_NG：失敗
//
////////////////////////////////////////////////////////////////////////////////
int CprtJobFunction::GetDataCover(void)
{
	CString			strCoAddr = _T("");
	CStringArray	strArray;

	//// 決算期間（自）
	//m_PrtDataBox[FieldCvrKikan1].Data[0] =
	//	ConvertDate( m_PrtInfCommon.KikanStart, PRT_FORMAT_DATE_C1, SchSeqCvr );
	//

	//// 決算期間（至）
	//m_PrtDataBox[FieldCvrKikan2].Data[0] =
	//	ConvertDate( m_PrtInfCommon.KikanEnd, PRT_FORMAT_DATE_C1, SchSeqCvr );

	// 元号(位置、幅等決算期間の繰り返しに含められない為別インデックス)
// midori 153812 del ------------------>
//// midori 150957 del -->
//	//if(m_PrtInfCommon.CoRekinen == 0){
//	//	for(int i=0; i<2; i++){
//	//		m_PrtDataBox[FieldCvrGengo].Data[i] = GetGengo(m_PrtInfCommon.KikanStart);
//	//	}
//	//}
//	//// 決算期間（自）
//	//for(int i=0; i<3; i++){
//	//	m_PrtDataBox[FieldCvrKikan1].Data[i] = ConvertDateCover( m_PrtInfCommon.KikanStart, i );
//	//}
//	//// 決算期間（至）
//	//for(int i=0; i<3; i++){
//	//	m_PrtDataBox[FieldCvrKikan2].Data[i] = ConvertDateCover( m_PrtInfCommon.KikanEnd, i );
//	//}
//// midori 150957 del <--
//// midori 150957 add -->
//	// 年号（自）
//	if(m_PrtInfCommon.CoRekinen == 0){
//		m_PrtDataBox[FieldCvrGengo].Data[0] = GetGengo(m_PrtInfCommon.KikanStart);
//	}
//	// 決算期間（自）
//	for(int i=0; i<3; i++){
//		m_PrtDataBox[FieldCvrKikan1].Data[i] = ConvertDateCover( m_PrtInfCommon.KikanStart, i );
//	}
//	// 年号（至）
//	if(m_PrtInfCommon.CoRekinen == 0){
//		m_PrtDataBox[FieldCvrGengo2].Data[0] = GetGengo(m_PrtInfCommon.KikanStart);
//	}
//	// 決算期間（至）
//	for(int i=0; i<3; i++){
//		m_PrtDataBox[FieldCvrKikan2].Data[i] = ConvertDateCover( m_PrtInfCommon.KikanEnd, i );
//	}
//// midori 150957 add <--
// midori 153812 del <------------------
// midori 153812 add ------------------>
	// 年号（自）
	if(m_PrtInfCommon.CoRekinen == 0){
// midori 181101 del -->
//		m_PrtDataBox[FieldCvrGengo].Data[0] = GetGengo(m_PrtInfCommon.KikanStart);
// midori 181101 del <--
// midori 181101 add -->
		m_PrtDataBox[FieldCvrGengo].Data[0] = GetGengo(m_PrtInfCommon.StartGen);
// midori 181101 add <--
	}

	// 決算期間（自）
	// 年
	m_PrtDataBox[FieldCvrKikan1].Data[0].Empty();
	if((m_PrtInfCommon.KikanStart / 10000) > 0)	{
// midori UC_0027 del -->
//		m_PrtDataBox[FieldCvrKikan1].Data[0].Format(_T("%d"),(m_PrtInfCommon.KikanStart / 10000));
// midori UC_0027 del <--
// midori UC_0027 add -->
		if((m_PrtInfCommon.KikanStart / 10000) == 1) {
			// 年度が1年の場合、「元年」を出力する
			m_PrtDataBox[FieldCvrKikan1].Data[0] = _T("元");
		}
		else {
			m_PrtDataBox[FieldCvrKikan1].Data[0].Format(_T("%d"),(m_PrtInfCommon.KikanStart / 10000));
		}
// midori UC_0027 add <--
	}
	// 月
	m_PrtDataBox[FieldCvrKikan1].Data[1].Empty();
	if(((m_PrtInfCommon.KikanStart % 10000) / 100) > 0)	{
		m_PrtDataBox[FieldCvrKikan1].Data[1].Format(_T("%d"),((m_PrtInfCommon.KikanStart % 10000) / 100));
	}
	// 日
	m_PrtDataBox[FieldCvrKikan1].Data[2].Empty();
	if((m_PrtInfCommon.KikanStart % 100) > 0)	{
		m_PrtDataBox[FieldCvrKikan1].Data[2].Format(_T("%d"),(m_PrtInfCommon.KikanStart % 100));
	}

	// 年号（至）
	if(m_PrtInfCommon.CoRekinen == 0){
// midori 181101 del -->
//		m_PrtDataBox[FieldCvrGengo2].Data[0] = GetGengo(m_PrtInfCommon.KikanStart);
// midori 181101 del <--
// midori 181101 add -->
		m_PrtDataBox[FieldCvrGengo2].Data[0] = GetGengo(m_PrtInfCommon.EndGen);
// midori 181101 add <--
	}

	// 決算期間（至）
	// 年
	m_PrtDataBox[FieldCvrKikan2].Data[0].Empty();
	if((m_PrtInfCommon.KikanEnd / 10000) > 0)	{
// midori UC_0027 del -->
//		m_PrtDataBox[FieldCvrKikan2].Data[0].Format(_T("%d"),(m_PrtInfCommon.KikanEnd / 10000));
// midori UC_0027 del <--
// midori UC_0027 add -->
		if((m_PrtInfCommon.KikanEnd / 10000) == 1) {
			// 年度が1年の場合、「元年」を出力する
			m_PrtDataBox[FieldCvrKikan2].Data[0] = _T("元");
		}
		else {
			m_PrtDataBox[FieldCvrKikan2].Data[0].Format(_T("%d"),(m_PrtInfCommon.KikanEnd / 10000));
		}
// midori UC_0027 add <--
	}
	// 月
	m_PrtDataBox[FieldCvrKikan2].Data[1].Empty();
	if(((m_PrtInfCommon.KikanEnd % 10000) / 100) > 0)	{
		m_PrtDataBox[FieldCvrKikan2].Data[1].Format(_T("%d"),((m_PrtInfCommon.KikanEnd % 10000) / 100));
	}
	// 日
	m_PrtDataBox[FieldCvrKikan2].Data[2].Empty();
	if((m_PrtInfCommon.KikanEnd % 100) > 0)	{
		m_PrtDataBox[FieldCvrKikan2].Data[2].Format(_T("%d"),(m_PrtInfCommon.KikanEnd % 100));
	}
// midori 153812 add <------------------

	// 「現在」
// midori 180101 del -->
//	if( m_nOutGenzai != 0 ){
//		m_PrtDataBox[FieldCvrGenzai].Data[0] = PRT_FORMAT_GENZAI;
//	}
// midori 180101 del <--
// midori 180101 add -->
	if( m_PrtInfCommon.OutGenzai != 0 ){
		m_PrtDataBox[FieldCvrGenzai].Data[0] = PRT_FORMAT_GENZAI;
	}
// midori 180101 add <--

	// 期
	if( m_PrtInfCommon.OutKi != 0 ){
		if( m_PrtInfCommon.Ki != 0 ){
			m_PrtDataBox[FieldCvrKi].Data[0].Format( PRT_FORMAT_DATE_H1, m_PrtInfCommon.Ki );
		}
		else{
			//第0期の場合は0を出力しない
			m_PrtDataBox[FieldCvrKi].Data[0] = PRT_FORMAT_DATE_H2;
		}
	}

	// 法人番号
	//m_PrtDataBox[FieldCvrHjNo].Data[0] = GetHjNo(m_icsPset.psSigCoprn, m_PrtInfCommon);


	//int h_flg12 = m_icsPset.psSigCoprn & BIT_D12;
	//int h_flg13 = m_icsPset.psSigCoprn & BIT_D13;
	//int	h_flg = !(h_flg13) && h_flg12; //　法人番号出力有効h_flg12　+　出力をしないh_flg13　に注意
	//if( h_flg ){
	//	CICSMNSub8 mnsub8;
	//	if( m_icsPset.psSigCoprn & BIT_D1){	// 顧問から法人番号を取得
	//		m_PrtDataBox[FieldCvrHjNo].Data[0] = mnsub8.MakePrintHojinNo((LPSTR)(LPCSTR)m_PrtInfCommon.HjNo_corec, BIT_D0);
	//	}
	//	else{
	//		//char f_dmbuf[128] = {};
	//		//strncpy_s(f_dmbuf, sizeof(f_dmbuf), m_PrtInfCommon.HjNo_zvol, m_PrtInfCommon.HjNo_zvol.GetLength());
	//		//m_PrtDataBox[FieldCvrHjNo].Data[0] = mnsub8.MakePrintHojinNo(f_dmbuf, 0x01);
	//		m_PrtDataBox[FieldCvrHjNo].Data[0] = mnsub8.MakePrintHojinNo((LPSTR)(LPCSTR)m_PrtInfCommon.HjNo_zvol, BIT_D0);
	//	}
	//}

	// 決算書の設定値を使用する
	// 会社名
	if(!(m_icsPset.psSigCoprn & BIT_D1)){
		CString	strCoName = m_clsFunc.DeleteRightSpace( m_PrtInfCommon.CoName );
		m_PrtDataBox[FieldCvrCoName].Data[0] = strCoName.TrimRight();
	}
	else{
		GetTgNameCd( m_PrtDataBox[FieldCvrCoName].Data[0] );
		//m_PrtDataBox[FieldCvrCoName].Data[0] = 
	}

	// MEMO：2006/10/23 住所出力仕様確定

	// 住所
	m_PrtInfCommon.CoAddr1 = m_clsFunc.DeleteRightSpace( m_PrtInfCommon.CoAddr1 );
	m_PrtInfCommon.CoAddr2 = m_clsFunc.DeleteRightSpace( m_PrtInfCommon.CoAddr2 );
	int nLen1 = m_PrtInfCommon.CoAddr1.GetLength();
	int nLen2 = m_PrtInfCommon.CoAddr2.GetLength();
	// 住所上段・下段の両方に入力がある
	if(( nLen1 != 0 ) && ( nLen2 != 0 )){
		// 文字数が異なる
		if( nLen1 != nLen2 ){
			BOOL flag = FALSE;		// 下段住所のほうが長い場合、TRUE
			CString  strLong, strShort;
			
			if( nLen1 > nLen2 ){
				strLong = m_PrtInfCommon.CoAddr1;
				strShort = m_PrtInfCommon.CoAddr2;
			}
			else{
				strLong = m_PrtInfCommon.CoAddr2;
				strShort = m_PrtInfCommon.CoAddr1;
				flag = TRUE;
			}

			// 短い住所の後ろに半角スペースを長い住所と文字数が同じになるまで挿入する
			while( strLong.GetLength() - 1 >= strShort.GetLength() ){
				strShort = strShort + " ";
			}

			if( flag != FALSE ){
				m_PrtInfCommon.CoAddr1 = strShort;
			}
			else{
				m_PrtInfCommon.CoAddr2 = strShort;
			}
		}
	}
	strCoAddr = m_PrtInfCommon.CoAddr1 + PRT_FORMAT_CHCODE + m_PrtInfCommon.CoAddr2;
	StrDivision( strCoAddr , &strArray , 2 );

	// 決算書の設定値を使用する
	m_PrtDataBox[FieldCvrCoAddr1].Data[0] = strArray.GetAt(0);
	m_PrtDataBox[FieldCvrCoAddr2].Data[0] = strArray.GetAt(1);

	// データをデータ出力バッファに格納
	SetDataBuffCover( FieldCvrMax, 3/*1*/ );

	return FUNCTION_OK;
}

// midori 190505 del -->
//////////////////////////////////////////////////////////////////////////////////
//// GetDbDataPageAll()
////
//// ■1帳表の総ページ数（最大ページ番号）を取得
////
//// 引数		：int				nFormSeq			様式シーケンス番号
////			：CDatabase*		pDb					DBハンドル
////			：int				nNameKeiBitArray	計の名称のビット配列
////
//// 戻り値	：int				総ページ数			取得できなかった場合は、
////													FUNCTION_NG（-1）を返す
////
//////////////////////////////////////////////////////////////////////////////////
//int CprtJobFunction::GetDbDataPageAll(int nFormSeq, CDatabase* pDb, int OutZero, int nNameKeiBitArray, int nFormType)
// midori 190505 del <--
// midori 190505 add -->
////////////////////////////////////////////////////////////////////////////////
// GetDbDataPageAll()
//
// ■1帳表の総ページ数（最大ページ番号）を取得
//
// 引数		：int				nFormSeq			様式シーケンス番号
//			：CDatabase*		pDb					DBハンドル
//			：int				OutZero				金額０円(空欄)出力設定
//			：int				nNameKeiBitArray	計の名称のビット配列
//			：int				nFormType			
//			：int				nPrnHoho			出力方法
//
// 戻り値	：int				総ページ数			取得できなかった場合は、
//													FUNCTION_NG（-1）を返す
//
////////////////////////////////////////////////////////////////////////////////
int CprtJobFunction::GetDbDataPageAll(int nFormSeq, CDatabase* pDb, int OutZero, int nNameKeiBitArray, int nFormType, int nPrnHoho)
// midori 190505 add <--
{
	CdbUc000Common*	prc;

	int		nRetPage=0;
// midori 160610 add -->
	int		ks=0;
// midori 160610 add <--
// midori 190505 add -->
	int		nOutzero=0;
// midori 157042 add -->
	int		kmkrowsw = 0;		// 0:入力データに科目行がない or ソートダイアログ「科目行を挿入する」非表示(画面に科目行挿入を可能にするの対応前）
								// 1:入力データに科目行がある
								// 2:科目指定から呼ばれた（科目行に対する処理は不要）
	CString strMoto = _T("");
// midori 157042 add <--

// midori 157042 del -->
	//if(nFormSeq == ID_FORMNO_021)		g_PrnHoho[0] = 0;
	//else if(nFormSeq == ID_FORMNO_111)	g_PrnHoho[1] = 0;
// midori 157042 del <--
// midori 190505 add <--

	if( OutZero == -1 ){
		return FUNCTION_NG;
	}

	try{
		switch( nFormSeq ){

		case SchSeqCvr:

			// 表紙のページ数は必ず「1」
			return 1;

		case ID_FORMNO_011:

			prc = (CdbUc000Common*)( new CdbUc011Yotyokin( pDb ) );
			break;

		case ID_FORMNO_021:

			prc = (CdbUc000Common*)( new CdbUc021Uketoritegata( pDb ) );
			break;

		case ID_FORMNO_031:

			prc = (CdbUc000Common*)( new CdbUc031Urikakekin( pDb ) );
			break;

		case ID_FORMNO_041:

			prc = (CdbUc000Common*)( new CdbUc041Karibarai( pDb ) );
			break;

		case ID_FORMNO_042:

			prc = (CdbUc000Common*)( new CdbUc042Kasituke( pDb ) );
			break;

		case ID_FORMNO_051:

			prc = (CdbUc000Common*)( new CdbUc051Tanaoroshi( pDb ) );
// midori 190505 del -->
//// midori 154982 del -->
//			//// 欄外登録の一時テーブル作成
//			//prc->CreateRangaiTempTable();
//// midori 154982 del <--
//// midori 154982 add -->
//			if(bG_Kanso != TRUE)	{
//				// 欄外登録の一時テーブル作成
//// midori 155525_2 del -->
//				//prc->CreateRangaiTempTable();
//// midori 155525_2 del <--
//// midori 155525_2 add -->
//				prc->CreateRangaiTempTable(OutZero);
//// midori 155525_2 add <--
//			}
//// midori 154982 add <--
// midori 190505 del <--
			break;

		case ID_FORMNO_061:

			prc = (CdbUc000Common*)( new CdbUc061Yuukasyouken( pDb ) );
			break;

		case ID_FORMNO_071:

			prc = (CdbUc000Common*)( new CdbUc071Koteishisan( pDb ) );
			break;

		case ID_FORMNO_081:

			prc = (CdbUc000Common*)( new CdbUc081Siharaitegata( pDb ) );
			break;

		case ID_FORMNO_091:

			prc = (CdbUc000Common*)( new CdbUc091Kaikakekin( pDb ) );
// midori 190505 del -->
//			// 欄外登録の一時テーブル作成
//// midori 155525_2 del -->
//			//prc->CreateRangaiTempTable();
//// midori 155525_2 del <--
//// midori 155525_2 add -->
//			prc->CreateRangaiTempTable(OutZero);
//// midori 155525_2 add <--
// midori 190505 del <--
			break;

		case ID_FORMNO_101:

			prc = (CdbUc000Common*)( new CdbUc101Kariukekin( pDb ) );
			break;

		case ID_FORMNO_102:

			prc = (CdbUc000Common*)( new CdbUc102Gensen( pDb ) );
			break;

		case ID_FORMNO_111:

			prc = (CdbUc000Common*)( new CdbUc111Kariirekin( pDb ) );
			break;

		case ID_FORMNO_121:

			prc = (CdbUc000Common*)( new CdbUc121Totiuriage( pDb ) );
			break;

		case ID_FORMNO_131:

			prc = (CdbUc000Common*)( new CdbUc131Uriagedaka( pDb ) );
			break;

		case ID_FORMNO_141:

			prc = (CdbUc000Common*)( new CdbUc141Yakuin( pDb ) );
			break;

		case ID_FORMNO_142:

			prc = (CdbUc000Common*)( new CdbUc142Jinkenhi( pDb ) );
			break;

		case ID_FORMNO_151:

			prc = (CdbUc000Common*)( new CdbUc151Tidaiyatin( pDb ) );
			break;

		case ID_FORMNO_152:

			prc = (CdbUc000Common*)( new CdbUc152Kenrikin( pDb ) );
			break;

		case ID_FORMNO_153:

			prc = (CdbUc000Common*)( new CdbUc153Kougyou( pDb ) );
			break;

		case ID_FORMNO_161:

			prc = (CdbUc000Common*)( new CdbUc161Zatueki( pDb ) );
			break;

		case ID_FORMNO_162:

			prc = (CdbUc000Common*)( new CdbUc162Zatuson( pDb ) );
			break;

		case ID_FORMNO_171:
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

			prc = (CdbUc000Common*)( new CdbUc171Sonota1( pDb, nFormSeq ) );
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

			prc = (CdbUc000Common*)( new CdbUc181Sonota2( pDb, nFormSeq ) );
			break;

		default:
			return FUNCTION_NG;
		}

// midori 190505 add -->
// midori 157001 del -->
		//if(OutZero != 0)	{
		//	// 「０円(空欄)データ出力しない」チェックON でも0円(空欄)のデータがなかったら nOutzero に0をセット
		//	int ck1,ck2;
		//	ck1 = ck2 = 0;
		//	if(OutZero & BIT_D0)	ck1 = 1;
		//	if(OutZero & BIT_D1)	ck2 = 1;
		//	if(prc->GetCountDataRecord2(nFormSeq, ck1, ck2) == 2)	{
		//		OutZero = 0;
		//	}
		//}
// midori 157001 del <--

		// 欄外登録の一時テーブル作成
		if(nFormSeq == ID_FORMNO_051)	{
			if(bG_Kanso != TRUE)	{
				prc->CreateRangaiTempTable(OutZero);
			}
		}
		else if(nFormSeq == ID_FORMNO_091)	{
			prc->CreateRangaiTempTable(OutZero);
		}
// midori 190505 add <--

// midori 160610 cor -->
		//// ページ数取得
		//if( OutZero == 0 ){	// 「０円データ出力しない」チェックOFF
		//	if( nFormSeq == ID_FORMNO_021 || nFormSeq == ID_FORMNO_111 ){
		//		// 受取手形の内訳書と借入金の内訳書は印刷設定の「科目行を出力する」で分岐
		//		CdbUcInfSub		mfcRecSub(pDb);
		//		if( mfcRecSub.RequeryFormSeq( nFormSeq ) == DB_ERR_OK ){
		//			if( !(mfcRecSub.IsEOF()) ){
		//				// inf_sub汎用変数コントロール
		//				CCtrlSubGeneralVar cs( mfcRecSub );
		//				if( mfcRecSub.m_GeneralConstVal[15] & BIT_D1 ){
		//					if( cs.bOutKamokuRow ){
		//						// 科目行追加
		//						nRetPage = prc->GetNumPageFromTempTable( nFormSeq, nNameKeiBitArray, OutZero );
		//					}
		//					else{
		//						// 科目行追加なし
		//						nRetPage = prc->GetNumPage();
		//					}
		//				}
		//			}
		//			mfcRecSub.Fin();
		//		}
		//	}
		//	else{	// 科目行追加なし
		//		nRetPage = prc->GetNumPage();
		//	}
		//}
		//else{	// 「０円データ出力しない」チェックON
		//	nRetPage = prc->GetNumPageFromTempTable( nFormSeq, nNameKeiBitArray, OutZero );
		//}
// ---------------------
		// ----------------------------------------
		// ページ数取得
		// ----------------------------------------
		CdbUcInfSub		mfcRecSub(pDb);
		if( mfcRecSub.RequeryFormSeq( nFormSeq ) == DB_ERR_OK ){
			if( !(mfcRecSub.IsEOF()) ){
				if(mfcRecSub.m_GeneralVar[1] & BIT_D3)	{
					// 科目指定を行う
// midori M-16113008 del -->
					//if(mfcRecSub.m_GeneralVar[1] & BIT_D4)		{
					//	// 科目でソートを行う
					//	ks=2;
					//}
					//else	{
					//	ks=1;
					//}
// midori M-16113008 del <--
// midori M-16113008 add -->
					ks=1;
// midori M-16113008 add <--
				}
				else	{
					// 科目指定を行わない
					ks=0;
				}

				// 「０円データ出力しない」チェックOFF
				if(OutZero == 0)	{
// 改良No.21-0086,21-0529 cor -->
					//// 受取手形の内訳書と借入金の内訳書は印刷設定の「科目行を出力する」で分岐
					//if( nFormSeq == ID_FORMNO_021 || nFormSeq == ID_FORMNO_111 ){
// ------------------------------
					// 受取手形の内訳書、支払手形の内訳表、借入金の内訳書は印刷設定の「科目行を出力する」で分岐
					if(nFormSeq == ID_FORMNO_021 || (bG_InvNo == TRUE && nFormSeq == ID_FORMNO_081) || nFormSeq == ID_FORMNO_111)	{
// 改良No.21-0086,21-0529 cor <--
						// inf_sub汎用変数コントロール
						CCtrlSubGeneralVar cs( mfcRecSub );
						if(mfcRecSub.m_GeneralConstVal[15] & BIT_D1)	{
// midori 157099,157119_2 del -->
//// midori 157098 del -->
////// midori 157042_2 del -->
////							//// 科目行を出力する
////							//if(cs.bOutKamokuRow)	{
////							//	nRetPage = prc->GetNumPageFromTempTable(nFormSeq,nNameKeiBitArray,OutZero,1,ks,GetPhoho(nFormSeq,nPrnHoho,&g_BatchPrt,m_pZmSub));
////							//}
////// midori 157042_2 del <--
////// midori 157042_2 add -->
////							kmkrowsw = 0;
////							// 期首が2019年4月以降
////							if(KamokuRowEnableSgn(pDb,0) == 1) {
////								if (ks == 0) {
////									kmkrowsw = prc->GetKmkRowSw();
////								}
////								else {
////									kmkrowsw = 2;
////								}
////							}
////							// 科目行を出力する
////							if(cs.bOutKamokuRow && kmkrowsw == 0)	{
////								nRetPage = prc->GetNumPageFromTempTable(nFormSeq,nNameKeiBitArray,OutZero,1,ks,GetPhoho(nFormSeq,nPrnHoho,&g_BatchPrt,m_pZmSub));
////							}
////// midori 157042_2 add <--
////							// 科目行を出力しない
////							else	{
////								// 科目指定を行う
////								if(ks)	{
////// midori 190505 del -->
////									//nRetPage = prc->GetNumPageFromTempTable(nFormSeq,nNameKeiBitArray,0,0,ks);
////// midori 190505 del <--
////// midori 190505 add -->
////									nRetPage = prc->GetNumPageFromTempTable(nFormSeq,nNameKeiBitArray,0,0,ks,GetPhoho(nFormSeq,nPrnHoho,&g_BatchPrt,m_pZmSub));
////// midori 190505 add <--
////								}
////								// 科目指定を行わない
////								else	{
////									nRetPage = prc->GetNumPage();
////									if(prc->GetCountDataRecord(1) == 0)	{
////										nRetPage = 0;
////									}
////								}
////							}
//// midori 157098 del <--
//// midori 157098 add -->
//							// 期首が2019年4月以降
//							if(KamokuRowEnableSgn(pDb,0) == 1) {
//								// 科目指定を行う
//								if(ks)	{
//									nRetPage = prc->GetNumPageFromTempTable(nFormSeq,nNameKeiBitArray,0,0,ks,GetPhoho(nFormSeq,nPrnHoho,&g_BatchPrt,m_pZmSub));
//								}
//								// 科目指定を行わない
//								else {
//									kmkrowsw = prc->GetKmkRowSw();
//									// 科目行を出力するにチェックあり
//									// データに科目行が無い
//									if(cs.bOutKamokuRow && kmkrowsw == 0)	{
//										nRetPage = prc->GetNumPageFromTempTable(nFormSeq,nNameKeiBitArray,0,1,ks,GetPhoho(nFormSeq,nPrnHoho,&g_BatchPrt,m_pZmSub));
//									}
//									// 科目行を出力するにチェック無し
//									// データに科目行が有る
//									else if(cs.bOutKamokuRow == FALSE && kmkrowsw == 1)	{
//										nRetPage = prc->GetNumPageFromTempTable(nFormSeq,nNameKeiBitArray,0,0,ks,GetPhoho(nFormSeq,nPrnHoho,&g_BatchPrt,m_pZmSub));
//									}
//									else {
//										nRetPage = prc->GetNumPage();
//										if(prc->GetCountDataRecord(1) == 0)	{
//											nRetPage = 0;
//										}
//									}
//								}
//							}
//							else {
//								// 科目行を出力する
//								if(cs.bOutKamokuRow)	{
//									nRetPage = prc->GetNumPageFromTempTable(nFormSeq,nNameKeiBitArray,OutZero,1,ks,GetPhoho(nFormSeq,nPrnHoho,&g_BatchPrt,m_pZmSub));
//								}
//								// 科目行を出力しない
//								else	{
//									// 科目指定を行う
//									if(ks)	{
//										nRetPage = prc->GetNumPageFromTempTable(nFormSeq,nNameKeiBitArray,0,0,ks,GetPhoho(nFormSeq,nPrnHoho,&g_BatchPrt,m_pZmSub));
//									}
//									// 科目指定を行わない
//									else	{
//										nRetPage = prc->GetNumPage();
//										if(prc->GetCountDataRecord(1) == 0)	{
//											nRetPage = 0;
//										}
//									}
//								}
//							}
//// midori 157098 add <--
// midori 157099,157119_2 del <--
// midori 157099,157119_2 add -->
							kmkrowsw = 0;
							// 期首が2019年4月以降
// 改良No.21-0086,21-0529 cor -->
							//if(KamokuRowEnableSgn(pDb,0) == 1) {
// ------------------------------
							if(KamokuRowEnableSgn(pDb, 0, nFormSeq) == 1)	{
// 改良No.21-0086,21-0529 cor <--
								if (ks == 0) {
									kmkrowsw = prc->GetKmkRowSw();
								}
								else {
									kmkrowsw = 2;
								}
							}
							// 科目行を出力する
							if(cs.bOutKamokuRow && kmkrowsw == 0)	{
								nRetPage = prc->GetNumPageFromTempTable(nFormSeq,nNameKeiBitArray,OutZero,1,ks,GetPhoho(nFormSeq,nPrnHoho,&g_BatchPrt,m_pZmSub));
							}
							// 科目行を出力しない
							else	{
								// 科目指定を行う
								if(ks)	{
									nRetPage = prc->GetNumPageFromTempTable(nFormSeq,nNameKeiBitArray,0,0,ks,GetPhoho(nFormSeq,nPrnHoho,&g_BatchPrt,m_pZmSub));
								}
								// 科目指定を行わない
								else	{
									nRetPage = prc->GetNumPage();
									if(prc->GetCountDataRecord(1) == 0)	{
										nRetPage = 0;
									}
								}
							}
// midori 157099,157119_2 add <--
						}
					}
					// 受取手形の内訳書と借入金の内訳書以外
					else	{
						// 科目指定を行う
						if(ks)	{
// midori 190505 del -->
							//nRetPage = prc->GetNumPageFromTempTable(nFormSeq,nNameKeiBitArray,0,0,ks);
// midori 190505 del <--
// midori 190505 add -->
							nRetPage = prc->GetNumPageFromTempTable(nFormSeq,nNameKeiBitArray,0,0,ks,GetPhoho(nFormSeq,nPrnHoho,&g_BatchPrt,m_pZmSub));
// midori 190505 add <--
						}
						// 科目指定を行わない
						else	{
							nRetPage = prc->GetNumPage();
							if(prc->GetCountDataRecord(1) == 0)	{
								nRetPage = 0;
							}
						}
					}
				}
				// 「０円データ出力しない」チェックON
				else	{
// midori 190505 del -->
					//nRetPage = prc->GetNumPageFromTempTable(nFormSeq,nNameKeiBitArray,OutZero,0,ks);
// midori 190505 del <--
// midori 190505 add -->
					nRetPage = prc->GetNumPageFromTempTable(nFormSeq,nNameKeiBitArray,OutZero,0,ks,GetPhoho(nFormSeq,nPrnHoho,&g_BatchPrt,m_pZmSub));
// midori 190505 add <--
				}
				mfcRecSub.Fin();
			}
		}
// midori 160610 cor <--

		delete prc;
	}
	catch( ... ){

		TRACE( "\n# ERROR: CJobFunction::GetDbDataPageAll()\n" );

		if( prc->IsOpen() ){

			prc->Fin();
			delete prc;
			return FUNCTION_NG;
		}
	}

	return nRetPage;
}

////////////////////////////////////////////////////////////////////////////////
// GetDbDataKnNameFirstRow()
//
// ■指定ページの最初に出現する科目の名称を取得
//
// 引数		：int				nFormSeq			様式シーケンス番号
//			：int				nPage				ページ番号
//			：CStirng*			pstrKnName			科目名
//			：int				OutZero				金額０円を出力するか？
//
// 戻り値	：int				エラーコード		FUNCTION_OK：成功
//													FUNCTION_NG：失敗
//
////////////////////////////////////////////////////////////////////////////////
int CprtJobFunction::GetDbDataKnNameFirstRow(int nFormSeq, int nPage, CString* pstrKnName, int OutZero)
{
// midori 157001 del -->
//// midori 190505 add -->
//	int					ck1 = 0;
//	int					ck2 = 0;
//	int					rt = 0;
//// midori 190505 add <--
// midori 157001 del <--
// midori 157042_2 add -->
	int					sw = 0;
// midori 157042_2 add <--
// midori 157098 add -->
	int					sw2 = 0;
// midori 157098 add <--
	CdbUc000Common*		prc;

	try{
		// 科目名の取得
		switch( nFormSeq ){
		case ID_FORMNO_021:
			prc = (CdbUc000Common*)( new CdbUc021Uketoritegata( m_pDB ) );
			break;

		case ID_FORMNO_031:
			prc = (CdbUc000Common*)( new CdbUc031Urikakekin( m_pDB ) );
			break;

		case ID_FORMNO_041:
			prc = (CdbUc000Common*)( new CdbUc041Karibarai( m_pDB ) );
			break;

		case ID_FORMNO_051:
			prc = (CdbUc000Common*)( new CdbUc051Tanaoroshi( m_pDB ) );
			break;

// 改良No.21-0086,21-0529 add -->
		case ID_FORMNO_081:
			prc = (CdbUc000Common*)( new CdbUc081Siharaitegata( m_pDB ) );
			break;
// 改良No.21-0086,21-0529 add <--

		case ID_FORMNO_091:
			prc = (CdbUc000Common*)( new CdbUc091Kaikakekin( m_pDB ) );
			break;

		case ID_FORMNO_101:
			prc = (CdbUc000Common*)( new CdbUc101Kariukekin( m_pDB ) );
			break;

		case ID_FORMNO_111:
			prc = (CdbUc000Common*)( new CdbUc111Kariirekin( m_pDB ) );
			break;

		case ID_FORMNO_161:
			prc = (CdbUc000Common*)( new CdbUc161Zatueki( m_pDB ) );
			break;

		case ID_FORMNO_162:
			prc = (CdbUc000Common*)( new CdbUc162Zatuson( m_pDB ) );
			break;

		// 科目別タイトル使用不可の帳表
		default:
			return FUNCTION_NG;
		}

		CString strTable;
// midori 160610 cor -->
		//if( OutZero != 0 ){
		//	strTable.Format( _T("#temp_utiwake_tbl_%d"), nFormSeq );
		//}
		//else{
		//	if( nFormSeq == ID_FORMNO_021 || nFormSeq == ID_FORMNO_111 ){
		//		// 受取手形の内訳書と借入金の内訳書は印刷設定の「科目行を出力する」で分岐
		//		CdbUcInfSub		mfcRecSub(m_pDB);
		//		if( mfcRecSub.RequeryFormSeq( nFormSeq ) == DB_ERR_OK ){
		//			if( !(mfcRecSub.IsEOF()) ){
		//				// inf_sub汎用変数コントロール
		//				CCtrlSubGeneralVar cs( mfcRecSub );
		//				if( mfcRecSub.m_GeneralConstVal[15] & BIT_D1 ){
		//					if( cs.bOutKamokuRow ){
		//						// 科目行追加
		//						strTable.Format( _T("#temp_utiwake_tbl_%d"), nFormSeq );
		//					}
		//				}
		//			}
		//			mfcRecSub.Fin();
		//		}
		//	}
		//}
// ---------------------
// midori 157001 del -->
//// midori 190505 del -->
//		//if( OutZero != 0 ){
//// midori 190505 del <--
//// midori 190505 add -->
//		// 金額0円(空欄)を出力しないにチェックが付いている状態でも、
//		// データに0円(空欄)が含まれていない場合、一時テーブルを作成せず、画面のデータをそのまま出力する
//		ck1 = ck2 = 0;
//		if(OutZero & BIT_D0)	ck1 = 1;
//		if(OutZero & BIT_D1)	ck2 = 1;
//		rt = prc->GetCountDataRecord2(nFormSeq, ck1, ck2);
//
//		if(OutZero != 0 && rt != 2)	{
//// midori 190505 add <--
// midori 157001 del <--
// midori 157001 add -->
		if( OutZero != 0 ){
// midori 157001 add <--
#ifdef _DEBUG
			strTable.Format( _T("##temp_utiwake_tbl_%d"), nFormSeq );
#else
			strTable.Format( _T("#temp_utiwake_tbl_%d"), nFormSeq );
#endif
		}
		else{
			CdbUcInfSub		mfcRecSub(m_pDB);
			if( mfcRecSub.RequeryFormSeq( nFormSeq ) == DB_ERR_OK ){
				if( !(mfcRecSub.IsEOF()) ){
// midori 157042_2 del -->
//// midori 190505 del -->
////					if( nFormSeq == ID_FORMNO_021 || nFormSeq == ID_FORMNO_111 ){
////						// 受取手形の内訳書と借入金の内訳書は印刷設定の「科目行を出力する」で分岐
////						// inf_sub汎用変数コントロール
////						CCtrlSubGeneralVar cs( mfcRecSub );
////						if( mfcRecSub.m_GeneralConstVal[15] & BIT_D1 ){
////							if( cs.bOutKamokuRow ){
////								// 科目行追加
////#ifdef _DEBUG
////								strTable.Format( _T("##temp_utiwake_tbl_%d"), nFormSeq );
////#else
////								strTable.Format( _T("#temp_utiwake_tbl_%d"), nFormSeq );
////#endif
////							}
////							else	{
////								if(mfcRecSub.m_GeneralVar[1] & BIT_D3)	{
////									// 科目指定を行う
////#ifdef _DEBUG
////									strTable.Format( _T("##temp_utiwake_tbl_%d"), nFormSeq );
////#else
////									strTable.Format( _T("#temp_utiwake_tbl_%d"), nFormSeq );
////#endif
////								}
////							}
////						}
////					}
////					else	{
////						if(mfcRecSub.m_GeneralVar[1] & BIT_D3)	{
////							// 科目指定を行う
////#ifdef _DEBUG
////							strTable.Format( _T("##temp_utiwake_tbl_%d"), nFormSeq );
////#else
////							strTable.Format( _T("#temp_utiwake_tbl_%d"), nFormSeq );
////#endif
////						}
////					}
//// midori 190505 del <--
//// midori 190505 add -->
//					if(mfcRecSub.m_GeneralVar[1] & BIT_D3)	{
//						// 科目指定を行う
//#ifdef _DEBUG
//						strTable.Format( _T("##temp_utiwake_tbl_%d"), nFormSeq );
//#else
//						strTable.Format( _T("#temp_utiwake_tbl_%d"), nFormSeq );
//#endif
//					}
//// midori 190505 add <--
// midori 157042_2 del <--
// midori 157042_2 add -->
// 改良No.21-0086,21-0529 cor -->
					//if( nFormSeq == ID_FORMNO_021 || nFormSeq == ID_FORMNO_111 ){
// ------------------------------
					if(nFormSeq == ID_FORMNO_021 || (bG_InvNo == TRUE && nFormSeq == ID_FORMNO_081) || nFormSeq == ID_FORMNO_111)	{
// 改良No.21-0086,21-0529 cor <--
// midori 157099,157119_2 del -->
//// midori 157098 del -->
////						sw = 0;
////						if(KamokuRowEnableSgn(m_pDB,0) > 0) {
////							sw = prc->GetKmkRowSw();
////						}
////						// 受取手形の内訳書と借入金の内訳書は印刷設定の「科目行を出力する」で分岐
////						// inf_sub汎用変数コントロール
////						CCtrlSubGeneralVar cs( mfcRecSub );
////						if( mfcRecSub.m_GeneralConstVal[15] & BIT_D1 ){
////							if( cs.bOutKamokuRow && sw == 0 ){
////								// 科目行追加
////#ifdef _DEBUG
////								strTable.Format( _T("##temp_utiwake_tbl_%d"), nFormSeq );
////#else
////								strTable.Format( _T("#temp_utiwake_tbl_%d"), nFormSeq );
////#endif
////							}
////							else	{
////								if(mfcRecSub.m_GeneralVar[1] & BIT_D3)	{
////									// 科目指定を行う
////#ifdef _DEBUG
////									strTable.Format( _T("##temp_utiwake_tbl_%d"), nFormSeq );
////#else
////									strTable.Format( _T("#temp_utiwake_tbl_%d"), nFormSeq );
////#endif
////								}
////							}
////						}
//// midori 157098 del <--
//// midori 157098 add -->
//						// 受取手形の内訳書と借入金の内訳書は印刷設定の「科目行を出力する」で分岐
//						// inf_sub汎用変数コントロール
//						CCtrlSubGeneralVar cs( mfcRecSub );
//						if( mfcRecSub.m_GeneralConstVal[15] & BIT_D1 ){
//							sw = KamokuRowEnableSgn(m_pDB,0);
//							if(sw == 1) {
//								sw2 = 0;
//								if(sw > 0) {
//									sw2 = prc->GetKmkRowSw();
//								}
//								// 科目行を出力するにチェックあり
//								// データに科目行なし
//								if( cs.bOutKamokuRow && sw2 == 0 ){
//									// 科目行追加
//#ifdef _DEBUG
//									strTable.Format( _T("##temp_utiwake_tbl_%d"), nFormSeq );
//#else
//									strTable.Format( _T("#temp_utiwake_tbl_%d"), nFormSeq );
//#endif
//								}
//								// 科目行を出力するにチェックなし
//								// データに科目行あり
//								else if( cs.bOutKamokuRow == FALSE && sw2 == 1 ){
//#ifdef _DEBUG
//									strTable.Format( _T("##temp_utiwake_tbl_%d"), nFormSeq );
//#else
//									strTable.Format( _T("#temp_utiwake_tbl_%d"), nFormSeq );
//#endif
//								}
//								else	{
//									if(mfcRecSub.m_GeneralVar[1] & BIT_D3)	{
//										// 科目指定を行う
//#ifdef _DEBUG
//										strTable.Format( _T("##temp_utiwake_tbl_%d"), nFormSeq );
//#else
//										strTable.Format( _T("#temp_utiwake_tbl_%d"), nFormSeq );
//#endif
//									}
//								}
//
//							}
//							else {
//								if( cs.bOutKamokuRow ){
//									// 科目行追加
//#ifdef _DEBUG
//									strTable.Format( _T("##temp_utiwake_tbl_%d"), nFormSeq );
//#else
//									strTable.Format( _T("#temp_utiwake_tbl_%d"), nFormSeq );
//#endif
//								}
//								else	{
//									if(mfcRecSub.m_GeneralVar[1] & BIT_D3)	{
//										// 科目指定を行う
//#ifdef _DEBUG
//										strTable.Format( _T("##temp_utiwake_tbl_%d"), nFormSeq );
//#else
//										strTable.Format( _T("#temp_utiwake_tbl_%d"), nFormSeq );
//#endif
//									}
//								}
//							}
//						}
//// midori 157098 add <--
// midori 157099,157119_2 del <--
// midori 157099,157119_2 add -->
						sw = 0;
// 改良No.21-0086,21-0529 cor -->
						//if(KamokuRowEnableSgn(m_pDB,0) > 0) {
// ------------------------------
						if(KamokuRowEnableSgn(m_pDB, 0, nFormSeq) > 0)	{
// 改良No.21-0086,21-0529 cor <--
							sw = prc->GetKmkRowSw();
						}
						// 受取手形の内訳書、支払手形の内訳書、借入金の内訳書は印刷設定の「科目行を出力する」で分岐
						// inf_sub汎用変数コントロール
						CCtrlSubGeneralVar cs( mfcRecSub );
						if( mfcRecSub.m_GeneralConstVal[15] & BIT_D1 ){
							if( cs.bOutKamokuRow && sw == 0 ){
								// 科目行追加
#ifdef _DEBUG
								strTable.Format( _T("##temp_utiwake_tbl_%d"), nFormSeq );
#else
								strTable.Format( _T("#temp_utiwake_tbl_%d"), nFormSeq );
#endif
							}
							else	{
								if(mfcRecSub.m_GeneralVar[1] & BIT_D3)	{
									// 科目指定を行う
#ifdef _DEBUG
									strTable.Format( _T("##temp_utiwake_tbl_%d"), nFormSeq );
#else
									strTable.Format( _T("#temp_utiwake_tbl_%d"), nFormSeq );
#endif
								}
							}
						}
// midori 157099,157119_2 add <--
					}
					else	{
						if(mfcRecSub.m_GeneralVar[1] & BIT_D3)	{
							// 科目指定を行う
#ifdef _DEBUG
							strTable.Format( _T("##temp_utiwake_tbl_%d"), nFormSeq );
#else
							strTable.Format( _T("#temp_utiwake_tbl_%d"), nFormSeq );
#endif
						}
					}
// midori 157042_2 add <--
				}
				mfcRecSub.Fin();
			}
		}
// midori 160610 cor <--

		prc->GetKnNameFirstRow( nPage, pstrKnName, strTable );
		delete prc;

		// 科目名を取得できなければ、タイトルをそのまま返す
		if( pstrKnName->IsEmpty() ){

			return FUNCTION_NG;
		}
	}
	catch( ... ){

		TRACE( "\n# ERROR: CJobFunction::StrMakeTitle()\n" );

		if( prc->IsOpen() ){

			prc->Fin();
			delete prc;
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}

////////////////////////////////////////////////////////////////////////////////
// GetDbDataTitleAll()
//
// ■全帳表タイトルの取得
//
// 戻り値	：int				エラーコード		FUNCTION_OK：成功
//													FUNCTION_NG：失敗
//
////////////////////////////////////////////////////////////////////////////////
int CprtJobFunction::GetDbDataTitleAll(void)
{
	CdbUcInfSub	rc( m_pDB );

	try{

		for( int i = 0; i <= UC_ID_FORMNO_MAX; i++ ){

			rc.Init( i );	// FormOrderから検索

			if( !rc.IsEOF() ){

				m_PrtInfTitleList[i].FormSeq	= rc.m_FormSeq;
				m_PrtInfTitleList[i].Title		= rc.m_Title;
				m_PrtInfTitleList[i].OpTitleKnFg	= rc.m_OpTitleKnFg;
				m_PrtInfTitleList[i].OpTitleKn		= rc.m_OpTitleKn;
			}

			rc.Fin();
		}
	}
	catch( ... ){

		TRACE( "\n# ERROR: CJobFunction::GetDbDataTitleAll()\n" );

		if( rc.IsOpen() ){

			rc.Fin();
			return FUNCTION_NG;
		}
	}
	return FUNCTION_OK;
}

// midori 180101 del -->
// midori 152109 cor -->
////////////////////////////////////////////////////////////////////////////////////
////// GetDbDataTableZVolume()
//////
////// ■DB（テーブル：zvolume）からデータを取得する。
//////
////// 戻り値	：int				エラーコード		FUNCTION_OK：成功
//////													FUNCTION_NG：失敗
//////************
//////2011/12/22廃止
//////******************
////////////////////////////////////////////////////////////////////////////////////
////int CprtJobFunction::GetDbDataTableZVolume(void)
////{
////	// 2010/08/XX 業務統合メニュー対応
////	// 【補足】
////	// 　会社名は当関数内で取得しているが、CprtJobFunction::JobPrint()内で実行している
////	// 　CprtJobFunction::GetTgNameCd()にて書き換えている。
////	// 　　→設定により、顧問先名に書き換える必要があるため。
////	// 　　→当関数の既存既存実行タイミングを変更したくないため。
////	// 　　　（"初期情報の取得"の一処理として実行されている）
////
////	// zvolumeテーブル参照
////	if ( m_pZmSub->VolumeOpen() == 0 ){
////		m_PrtInfCommon.CoName = m_pZmSub->zvol->c_nam;		// 会社名
//////		m_PrtInfCommon.CoName.TrimRight();
////		m_PrtInfCommon.KikanStart = m_pZmSub->zvol->ss_ymd;	// 決算期間(期首)
////		m_PrtInfCommon.KikanEnd = m_pZmSub->zvol->ee_ymd;	// 決算期間(期末)
////
////		// テーブルを閉じる
////		m_pZmSub->VolumeClose();
////	}
////	else{
////		// 2007.03 DBエラー対応
////		m_clsFunc.ZmSubErrMsg( m_pZmSub );
////		return FUNCTION_NG;
////	}
////
////	return FUNCTION_OK;
////
////}
//// ---------------------
//////////////////////////////////////////////////////////////////////////////////
//// GetDbDataTableZVolume()
////
//// ■DB（テーブル：zvolume）からデータを取得する。
////
//// 戻り値	：int				エラーコード		FUNCTION_OK：成功
////													FUNCTION_NG：失敗
//////////////////////////////////////////////////////////////////////////////////
//int CprtJobFunction::GetDbDataTableZVolume(void)
//{
//	// zvolumeテーブル参照
//	if ( m_pZmSub->VolumeOpen() == 0 ){
//		m_PrtInfCommon.Ki = m_pZmSub->zvol->ki;
//
//		// テーブルを閉じる
//		m_pZmSub->VolumeClose();
//	}
//	else{
//		// 2007.03 DBエラー対応
//		m_clsFunc.ZmSubErrMsg( m_pZmSub );
//		return(FUNCTION_NG);
//	}
//
//	return(FUNCTION_OK);
//}
//// midori 152109 cor <--
// midori 180101 del <--

////////////////////////////////////////////////////////////////////////////////
// GetDbDataTableCpdata()
//
// ■DB（テーブル：cpdata）からデータを取得する。
//
// 戻り値	：int				エラーコード		FUNCTION_OK：成功
//													FUNCTION_NG：失敗
//
////////////////////////////////////////////////////////////////////////////////
int CprtJobFunction::GetDbDataTableCpdata(void)
{
	// MEMO:

	CdbBase rc( m_pDB );

	try{
		// 2007.12.12：翌期更新をすると｢当期の住所｣が印字されない対応
		CString strSqlAdd = _T( "SELECT cpyg , cpua , cpda FROM [cpdata] WHERE cpkofs = 0" );
		if ( !rc.OpenEx( NULL , strSqlAdd , "cpdata" ) ){
			return FUNCTION_NG;
		}

		if( !rc.IsEOF() ){

			CDBVariant		var;
			CString			strDate;
			CdateConvert	dateCvt;

			// 会社名
			rc.GetFieldValue( _T( "cpyg" ), var );

			if( var.m_dwType != DBVT_NULL ){
				m_PrtInfCommon.CoName = *var.m_pstring;
				m_PrtInfCommon.CoName.TrimRight();

				// 印刷ダイアログで「顧問先の会社名を使用する」の切替に対応する為
				// ここでマスター側の会社名を保存しておく（m_PrtInfCommon.CoNameでは顧問先名で潰されてしまう）
				m_strCoNameCpdata = m_PrtInfCommon.CoName;	
			}

			// 住所(上段)
			rc.GetFieldValue( _T( "cpua" ), var );

			if( var.m_dwType != DBVT_NULL ){
				m_PrtInfCommon.CoAddr1 = *var.m_pstring;
				m_PrtInfCommon.CoAddr1.TrimRight();
			}

			// 住所(下段)
			rc.GetFieldValue( _T( "cpda" ), var );

			if( var.m_dwType != DBVT_NULL ){
				m_PrtInfCommon.CoAddr2 = *var.m_pstring;
				m_PrtInfCommon.CoAddr2.TrimRight();
			}

		}
		rc.Close();
	}
	catch( ... ){

		TRACE( "\n# ERROR: CJobFunction::GetDbDataTableCpdata()\n" );

		if( rc.IsOpen() ){

			rc.Close();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}

////////////////////////////////////////////////////////////////////////////////
// GetDbDataTablePrfkin()
//
// ■prfkinテーブルからデータを取得する。
//
// 戻り値	：int				エラーコード		FUNCTION_OK：成功
//													FUNCTION_NG：失敗
//
////////////////////////////////////////////////////////////////////////////////
int	CprtJobFunction::GetDbDataTablePrfkin(void)
{
// midori 180101 del -->
//	CDBPrtbl pt;
//	pt.SetMasterDB( m_pDB );
//	int st = pt.PrfkinOpen( "pkofs = 0" );	// 当期のみ参照
//	if( st != 0 )	return FUNCTION_NG;
//	
//	m_PrtInfCommon.KikanStart	= pt.prfkin->pksdy;		// 決算表示期間（自）	表紙
//	m_PrtInfCommon.KikanEnd		= pt.prfkin->pkedy;		// 決算表示期間（至）	表紙
//// midori 152109 cor -->
////	m_PrtInfCommon.Ki			= pt.prfkin->pkdkin;	// 期
//// ---------------------
//	if(m_PrtInfCommon.Ki == 0)	{
//		m_PrtInfCommon.Ki			= pt.prfkin->pkdkin;	// 期
//	}
//// midori 152109 cor <--
//	// 様式日付　決算書業務のF6登録画面の「各ページ表示期間」のチェック状態
//	if(pt.prfkin->pkmsw & BIT_D0)				m_PrtInfCommon.KikanEndPage = pt.prfkin->pkmdy;
//	else										m_PrtInfCommon.KikanEndPage = pt.prfkin->pkedy;
//	// 暦年表示サイン D2ビットが立っている→西暦 立っていない→和暦
//	if( (pt.prfkin->pkmsw & BIT_D2) != 0 )		m_PrtInfCommon.CoRekinen = 1;
//	else										m_PrtInfCommon.CoRekinen = 0;
//	// 期出力サイン
//	if( (pt.prfkin->pkmsw & BIT_D1) != 0 )		m_PrtInfCommon.OutKi = 1;
//	else										m_PrtInfCommon.OutKi = 0;
//		
//	pt.PrfkinClose();
// midori 180101 del <--
// midori 180101 add -->
	int				st=0;
	int				symd=0,eymd=0;
// midori 191201 add -->
	int				nRekinen=0;
	int				nver=0;
// midori 151730,151263 del -->
	//int				nchkreki=0;
// midori 151730,151263 del <--
// midori 191201 add <--
// midori 153813 add ------------------>
	BYTE			apno=0x00;
// midori 153813 add <------------------
	CString			strQuery="";
	CDBPrtbl		pt;
	CVolDateDB		icsVolDate;			// DB期間モジュール

// midori 153382 del -->
	//// 決算書データ側から取得（一旦、内訳書で必要なすべての項目を取得）
	//pt.SetMasterDB( m_pDB );
	//st = pt.PrfkinOpen( "pkofs = 0" );	// 当期のみ参照
	//if( st != 0 )	return FUNCTION_NG;
	//
	//m_PrtInfCommon.Ki			= pt.prfkin->pkdkin;	// 期
	//// 様式日付　決算書業務のF6登録画面の「各ページ表示期間」のチェック状態
	//if(pt.prfkin->pkmsw & BIT_D0)			m_PrtInfCommon.KikanEndPage = pt.prfkin->pkmdy;
	//else									m_PrtInfCommon.KikanEndPage = pt.prfkin->pkedy;
	//// 期出力サイン
	//if( (pt.prfkin->pkmsw & BIT_D1) != 0 )	m_PrtInfCommon.OutKi = 1;
	//else									m_PrtInfCommon.OutKi = 0;
	//
	//pt.PrfkinClose();

	//// 会社情報から取得する
	//m_pZmSub->VolumeOpen();
	//icsVolDate.db_datecnv(m_pZmSub->zvol->ss_ymd,&symd,0,0);		// 0:西暦→平成へ変換，0:年月日より取得
	//icsVolDate.db_datecnv(m_pZmSub->zvol->ee_ymd,&eymd,0,0);		// 0:西暦→平成へ変換，0:年月日より取得
	//m_PrtInfCommon.KikanStart	= symd;		// 決算期間（自）
	//m_PrtInfCommon.KikanEnd		= eymd;		// 決算表示期間（至）
	//m_PrtInfCommon.CoRekinen	= 0;		// 暦年表示サイン
	//m_pZmSub->VolumeClose();

	//// Owntbテーブルから取得
	//m_pZmSub->OwnTblOpen();
	//strQuery.Format(_T("apl_name='%s' and itm_name='%s' and itm_seq = 0"), "UCHIWAKE", "HyoshiData");
	//m_pZmSub->owntb->Requery(strQuery);
	//if(m_pZmSub->owntb->GetRecordCount() > 0)	{
	//	// Owntbテーブルからデータを取得
	//	m_PrtInfCommon.KikanStart		= m_pZmSub->owntb->vl[0];			// 決算表示期間（自）	表紙
	//	m_PrtInfCommon.KikanEnd			= m_pZmSub->owntb->vl[1];			// 決算表示期間（至）	表紙
	//	m_PrtInfCommon.CoRekinen		= m_pZmSub->owntb->vl[2];			// 暦年表示サイン D2ビットが立っている→西暦 立っていない→和暦
	//}
	//// Owntbテーブルをクローズ
	//m_pZmSub->OwnTblClose();
// midori 153382 del <--
// midori 153382 153383 add -->
	// 会社情報から取得する
	m_pZmSub->VolumeOpen();
// midori 153812 del ------------------>
//	icsVolDate.db_datecnv(m_pZmSub->zvol->ss_ymd,&symd,0,0);		// 0:西暦→平成へ変換，0:年月日より取得
//	icsVolDate.db_datecnv(m_pZmSub->zvol->ee_ymd,&eymd,0,0);		// 0:西暦→平成へ変換，0:年月日より取得
//	m_PrtInfCommon.KikanStart	= symd;		// 決算期間（自）
//	m_PrtInfCommon.KikanEnd		= eymd;		// 決算表示期間（至）
//	m_PrtInfCommon.CoRekinen	= 0;		// 暦年表示サイン
// midori 153812 del <------------------
// midori 153813 add ------------------>
	apno = m_pZmSub->zvol->apno;
// midori 153813 add <------------------
// midori 191201 add -->
	nver = m_pZmSub->zvol->m_ver;
// midori 191201 add <--
	m_pZmSub->VolumeClose();

	// 決算書データテーブルのオープン
	pt.SetMasterDB( m_pDB );
	st = pt.PrfkinOpen( "pkofs = 0" );	// 当期のみ参照
	if( st != 0 )	return FUNCTION_NG;

	// Owntbテーブルのオープン
	m_pZmSub->OwnTblOpen();
	strQuery.Format(_T("apl_name='%s' and itm_name='%s' and itm_seq = 0"), "UCHIWAKE", "HyoshiData");
	m_pZmSub->owntb->Requery(strQuery);

// midori 155882 add -->
	m_clsFunc.YmdChg(m_pZmSub,&pt);
// midori 155882 add <--

	// -------------------------------------------------------------------------------------------------
	// 「指定期間と連動させる」ONの時
// midori 151730,151263 del -->
//// midori 191201 del -->
//	//if((pt.prfkin->pkmsw & BIT_D3) != 0)	{
//// midori 191201 del <--
//// midori 191201 add -->
//	nchkreki = m_clsFunc.GetRekinen(m_pZmSub->m_database,apno,nver,&nRekinen);
//	if((pt.prfkin->pkmsw & BIT_D3) != 0  && nchkreki == 0) {
//// midori 191201 add <--
// midori 151730,151263 del <--
// midori 151730,151263 add -->
	if((pt.prfkin->pkmsw & BIT_D3) != 0) {
// midori 151730,151263 add <--
		// 期
		m_PrtInfCommon.Ki			= pt.prfkin->pkdkin;

		if((m_pZmSub->owntb->vl[0] == 0 || m_pZmSub->owntb->vl[1] == 0)
// midori 181101 del -->
//		|| (m_pZmSub->owntb->vl[2] != pt.prfkin->pksdy || m_pZmSub->owntb->vl[3] != pt.prfkin->pkedy))	{
// midori 181101 del <--
// midori 181101 add -->
		 || (m_pZmSub->owntb->vl[6] != pt.prfkin->pkHyGen1 || m_pZmSub->owntb->vl[2] != pt.prfkin->pksdy
		 || m_pZmSub->owntb->vl[7] != pt.prfkin->pkHyGen2 || m_pZmSub->owntb->vl[3] != pt.prfkin->pkedy))	{
// midori 181101 add <--
			// 決算表示期間（自）	表紙
// midori 181101 add -->
			m_PrtInfCommon.StartGen			= pt.prfkin->pkHyGen1;
// midori 181101 add <--
			m_PrtInfCommon.KikanStart		= pt.prfkin->pksdy;
			// 決算表示期間（至）	表紙
// midori 181101 add -->
			m_PrtInfCommon.EndGen			= pt.prfkin->pkHyGen2;
// midori 181101 add <--
			m_PrtInfCommon.KikanEnd			= pt.prfkin->pkedy;
			// 暦年表示サイン D2ビットが立っている→西暦 立っていない→和暦
// midori 151730,151263 del -->
//// midori 153813 del ------------------>
////			if(pt.prfkin->pkmsw&0x04)		m_PrtInfCommon.CoRekinen = 1;
////			else							m_PrtInfCommon.CoRekinen = 0;
//// midori 153813 del <------------------
//// midori 153813 add ------------------>
//			// 公益:0x50、公益Ⅲ:0x51、社会福祉法人:0x52、学校:0x58、宗教法人:0x59の場合は和暦固定にする
//			if(apno == 0x50 || apno == 0x51 || apno == 0x52 || apno == 0x58 || apno == 0x59)	{
//				m_PrtInfCommon.CoRekinen = 0;
//			}
//			else	{
//				if(pt.prfkin->pkmsw&0x04)		m_PrtInfCommon.CoRekinen = 1;
//				else							m_PrtInfCommon.CoRekinen = 0;
//			}
//// midori 153813 add <------------------
//// midori 153812 del ------------------>
////			// 様式日付　決算書業務のF6登録画面の「各ページ表示期間」のチェック状態
////			if(pt.prfkin->pkmsw & BIT_D0)	m_PrtInfCommon.KikanEndPage = pt.prfkin->pkmdy;
////			else							m_PrtInfCommon.KikanEndPage = pt.prfkin->pkedy;
//// midori 153812 del <------------------
// midori 151730,151263 del <--
// midori 151730,151263 add -->
			if(pt.prfkin->pkmsw&0x04)		m_PrtInfCommon.CoRekinen = 1;
			else							m_PrtInfCommon.CoRekinen = 0;
// midori 151730,151263 add <--
		}
		else	{
			// 決算表示期間（自）	表紙
// midori 181101 add -->
			m_PrtInfCommon.StartGen			= m_pZmSub->owntb->vl[4];
// midori 181101 add <--
			m_PrtInfCommon.KikanStart		= m_pZmSub->owntb->vl[0];
			// 決算表示期間（至）	表紙
// midori 181101 add -->
			m_PrtInfCommon.EndGen			= m_pZmSub->owntb->vl[5];
// midori 181101 add <--
			m_PrtInfCommon.KikanEnd			= m_pZmSub->owntb->vl[1];
			// 暦年表示サイン D2ビットが立っている→西暦 立っていない→和暦
// midori 151730,151263 del -->
//// midori 153813 del ------------------>
////			if(pt.prfkin->pkmsw&0x04)		m_PrtInfCommon.CoRekinen = 1;
////			else							m_PrtInfCommon.CoRekinen = 0;
//// midori 153813 del <------------------
//// midori 153813 add ------------------>
//			// 公益:0x50、公益Ⅲ:0x51、社会福祉法人:0x52、学校:0x58、宗教法人:0x59の場合は和暦固定にする
//			if(apno == 0x50 || apno == 0x51 || apno == 0x52 || apno == 0x58 || apno == 0x59)	{
//				m_PrtInfCommon.CoRekinen = 0;
//			}
//			else	{
//				if(pt.prfkin->pkmsw&0x04)		m_PrtInfCommon.CoRekinen = 1;
//				else							m_PrtInfCommon.CoRekinen = 0;
//			}
//// midori 153813 add <------------------
//// midori 153812 del ------------------>
////			// 様式日付　決算書業務のF6登録画面の「各ページ表示期間」のチェック状態
////			if(pt.prfkin->pkmsw & BIT_D0)	m_PrtInfCommon.KikanEndPage = pt.prfkin->pkmdy;
////			else							m_PrtInfCommon.KikanEndPage = m_pZmSub->owntb->vl[1];
//// midori 153812 del <------------------
// midori 151730,151263 del <--
// midori 151730,151263 add -->
			if(pt.prfkin->pkmsw&0x04)		m_PrtInfCommon.CoRekinen = 1;
			else							m_PrtInfCommon.CoRekinen = 0;
// midori 151730,151263 add <--
		}
		// 期出力サイン
		if( (pt.prfkin->pkmsw & BIT_D1) != 0 )	m_PrtInfCommon.OutKi = 1;
		else									m_PrtInfCommon.OutKi = 0;
	}
	// 「指定期間と連動させる」OFFの時
	else	{
		// 期
		m_PrtInfCommon.Ki						= pt.prfkin->pkdkin;
		// 決算表示期間（自）	表紙
// midori 181101 add -->
		m_PrtInfCommon.StartGen					= pt.prfkin->pkHyGen1;
// midori 181101 add <--
		m_PrtInfCommon.KikanStart				= pt.prfkin->pksdy;
		// 決算表示期間（至）	表紙
// midori 181101 add -->
		m_PrtInfCommon.EndGen					= pt.prfkin->pkHyGen2;
// midori 181101 add <--
		m_PrtInfCommon.KikanEnd					= pt.prfkin->pkedy;
		// 暦年表示サイン D2ビットが立っている→西暦 立っていない→和暦
// midori 153813 del ------------------>
//		if(pt.prfkin->pkmsw&0x04)				m_PrtInfCommon.CoRekinen = 1;
//		else									m_PrtInfCommon.CoRekinen = 0;
// midori 153813 del <------------------
// midori 153813 add ------------------>
// midori 151730,151263 del -->
//// midori 191201 del -->
//		//// 公益:0x50、公益Ⅲ:0x51、社会福祉法人:0x52、学校:0x58、宗教法人:0x59の場合は和暦固定にする
//		//if(apno == 0x50 || apno == 0x51 || apno == 0x52 || apno == 0x58 || apno == 0x59)	{
//		//	m_PrtInfCommon.CoRekinen = 0;
//// midori 191201 del <--
//// midori 191201 add -->
//		if(apno == 0x50 || apno == 0x51 || apno == 0x52 || apno == 0x58 || apno == 0x59)	{
//			// 下記の場合は和暦固定にする
//			// ・公益または社会福祉法人の旧会計基準
//			// ・暦年表示未対応
//			if(nchkreki == 1) {
//				m_PrtInfCommon.CoRekinen = nRekinen;
//			}
//			else {
//				m_PrtInfCommon.CoRekinen = 0;
//			}
//// midori 191201 add <--
//		}
//		else	{
//			if(pt.prfkin->pkmsw&0x04)				m_PrtInfCommon.CoRekinen = 1;
//			else									m_PrtInfCommon.CoRekinen = 0;
//		}
// midori 151730,151263 del <--
// midori 151730,151263 add -->
		if(isHIEIRI(apno))	{
			// 下記の場合は和暦固定にする
			// ・公益
			// ・学校法人の旧会計基準
			// ・社会福祉法人の旧会計基準
			if(isKYUKAIKEI(apno,nver)) {
				m_PrtInfCommon.CoRekinen = 0;
			}
			else {
				m_PrtInfCommon.CoRekinen = m_clsFunc.GetRekinen(m_pZmSub);
			}
		}
		else	{
			if(pt.prfkin->pkmsw&0x04)				m_PrtInfCommon.CoRekinen = 1;
			else									m_PrtInfCommon.CoRekinen = 0;
		}
// midori 151730,151263 add <--
// midori 153813 add <------------------
// midori 153812 del ------------------>
//		// 様式日付　決算書業務のF6登録画面の「各ページ表示期間」のチェック状態
//		if(pt.prfkin->pkmsw & BIT_D0)			m_PrtInfCommon.KikanEndPage = pt.prfkin->pkmdy;
//		else									m_PrtInfCommon.KikanEndPage = pt.prfkin->pkedy;
// midori 153812 del <------------------
		// 期出力サイン
		if((pt.prfkin->pkmsw & BIT_D1) != 0)	m_PrtInfCommon.OutKi = 1;
		else									m_PrtInfCommon.OutKi = 0;
	}

	// -------------------------------------------------------------------------------------------------

	// Owntbテーブルをクローズ
	m_pZmSub->OwnTblClose();

	// 決算書テーブルのクローズ
	pt.PrfkinClose();
// midori 153382 153383 add <--

// midori 180101 add <--

	return FUNCTION_OK;
}

///////////////////////////////////////////////////////////////////////////////
// GetDbDataTableUcInfMain()
//
// ■DB（テーブル：uc_inf_main）からデータを取得する。
//
// 戻り値	：int				エラーコード		FUNCTION_OK：成功
//													FUNCTION_NG：失敗
//
////////////////////////////////////////////////////////////////////////////////
int CprtJobFunction::GetDbDataTableUcInfMain(void)
{
	CdbUcInfMain	rc( m_pDB );

	try{

		rc.Init();
		if( !rc.IsEOF() ){

			m_PrtInfCommon.OutCoName	= rc.m_OutCoName;
			m_PrtInfCommon.OutKikan		= rc.m_OutKikan;
			//m_PrtInfCommon.OutKingaku	= rc.m_OutKingaku;
			m_PrtInfCommon.OutKingaku	= ( rc.m_OutKingaku & BIT_D0 );
			m_PrtInfCommon.OutPage		= rc.m_OutPage;

// midori 180101 add -->
			if(rc.m_GeneralVar[2] & BIT_D2)	m_PrtInfCommon.OutGenzai = 1;
			else							m_PrtInfCommon.OutGenzai = 0;
// midori 180101 add <--

// midori 200101 add -->
			//if( (rc.m_GeneralVar[3] & BIT_D3) == BIT_D3 )	{
// midori 155871 del -->
			//if(rc.m_GeneralVar[3] & BIT_D3)	m_PrtInfCommon.OutGengo = 1;
// midori 155871 del <--
// midori 155871 add -->
			if((rc.m_GeneralVar[3] & BIT_D3) == 0)	m_PrtInfCommon.OutGengo = 1;
// midori 155871 add <--
			else									m_PrtInfCommon.OutGengo = 0;
// midori 200101 add <--
		}

		rc.Fin();
	}
	catch( ... ){

		TRACE( "\n# ERROR: CJobFunction::GetDbDataTableUcInfMain()\n" );

		if( rc.IsOpen() ){

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}

// midori 190505 del -->
//////////////////////////////////////////////////////////////////////////////////
//// GetDbDataTableUcInfSub()
////
//// ■DB（テーブル：uc_inf_sub）からデータを取得する。
////
//// 引数		：int				index				出力リストのインデックス
////
//// 戻り値	：int				エラーコード		FUNCTION_OK：成功
////													FUNCTION_NG：失敗
////
//////////////////////////////////////////////////////////////////////////////////
//int CprtJobFunction::GetDbDataTableUcInfSub(int index)
// midori 190505 del <--
// midori 190505 add -->
////////////////////////////////////////////////////////////////////////////////
// GetDbDataTableUcInfSub()
//
// ■DB（テーブル：uc_inf_sub）からデータを取得する。
//
// 引数		：int				index				出力リストのインデックス
//			：int				pflg				0:単独出力、1:一括出力
//
// 戻り値	：int				エラーコード		FUNCTION_OK：成功
//													FUNCTION_NG：失敗
//
////////////////////////////////////////////////////////////////////////////////
int CprtJobFunction::GetDbDataTableUcInfSub(int index, int pflg)
// midori 190505 add <--
{
	CdbUcInfSub	rc( m_pDB );

	try{
		rc.RequeryFormSeq( m_PrtInfMainList[index].Lite.FormSeq );
	
		if( !rc.IsEOF() ){
			CCtrlSubGeneralVar cs( rc );

			m_PrtInfMainList[index].FormNo		= rc.m_FormNo;
			m_PrtInfMainList[index].FormTag		= (PRT_ENUM_FORM_TAG)rc.m_FormNo2;
			m_PrtInfMainList[index].Option.KinyuLine	= cs.bKinyuLine;
// midori 190505 del -->
			//m_PrtInfMainList[index].Option.KamokuRow	= cs.bOutKamokuRow;
// midori 190505 del <--
// midori 156188_2 add -->
			m_PrtInfMainList[index].Option.KamokuRow	= cs.bOutKamokuRow;
// midori 156188_2 add <--
// midori 160610 add -->
			if(rc.m_GeneralVar[1] & BIT_D3)	m_PrtInfMainList[index].Option.KamokuSitei[0] = 1;
			else							m_PrtInfMainList[index].Option.KamokuSitei[0] = 0;
// midori 160610 add <--
			if(cs.bPrintZeroData)	m_PrtInfMainList[index].Option.OutZeroNull[0] |= BIT_D0;
			if(cs.bPrintNullData)	m_PrtInfMainList[index].Option.OutZeroNull[0] |= BIT_D1;

// midori 190505 add -->
			//// 出力方法
			//if(pflg == 0)	{
			//	if(cs.bPrnHoho1)	m_PrtInfMainList[index].Lite.PrnHoho = 0;
			//	if(cs.bPrnHoho2)	m_PrtInfMainList[index].Lite.PrnHoho = 1;
			//	if(cs.bPrnHoho3)	m_PrtInfMainList[index].Lite.PrnHoho = 2;
			//}
// midori 190505 add <--

			m_PrtInfMainList[index].Option.HideZero[0]	= cs.bPrintHideZero;
// midori 154119 add -->
			if(rc.m_GeneralVar[0] & BIT_D6)	m_PrtInfMainList[index].Option.NameKei[0] = 1;
			else							m_PrtInfMainList[index].Option.NameKei[0] = 0;
// midori 154119 add <--
		}

		rc.Fin();
	}
	catch( ... ){

		TRACE( "\n# ERROR: CJobFunction::GetDbDataTableUcInfSub()\n" );

		if( rc.IsOpen() ){

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}

////////////////////////////////////////////////////////////////////////////////
// GetDbDataTableUcInfFormType()
//
// ■DBから、出力形式を取得する。
//
// 引数		：int				index				出力リストのインデックス
//
// 戻り値	：int				エラーコード		FUNCTION_OK：成功
//													FUNCTION_NG：失敗
//
////////////////////////////////////////////////////////////////////////////////
int CprtJobFunction::GetDbDataTableUcInfFormType(int index, CDatabase* pDB)
{
	CDatabase*			tmp_pDB = NULL;

	if( pDB != NULL ){
		tmp_pDB = pDB;
	}
	else{
		tmp_pDB = m_pDB;
	}

	CdbUcInfFormType	rc( tmp_pDB );
	

	try{

		rc.Init( m_PrtInfMainList[index].FormNo );

		if( !rc.IsEOF() ){

			m_PrtInfMainList[index].FormType	= rc.m_FormType;
		}

		rc.Fin();
	}
	catch( ... ){

		TRACE( "\n# ERROR: CJobFunction::GetDbDataTableUcInfFormType()\n" );

		if( rc.IsOpen() ){

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}

////////////////////////////////////////////////////////////////////////////////
// GetDbDataTableUcInfSubOmit()
//
// ■DB（テーブル：uc_inf_sub_omit）からデータを取得する。
//
// 引数		：int				nFormSeq			様式シーケンス番号
//			：PRT_ENUM_FORM_TAG	tag					様式番号の枝番
//
// 戻り値	：int				エラーコード		FUNCTION_OK：成功
//													FUNCTION_NG：失敗
//
////////////////////////////////////////////////////////////////////////////////
int CprtJobFunction::GetDbDataTableUcInfSubOmit(int nFormSeq, PRT_INF_OMIT* pPrtInfOmit)
{
	PRT_INF_OMIT*	pPrtInfOmitWork;

	pPrtInfOmitWork = pPrtInfOmit;	// ポインタをコピー

	// 省略文字リストの初期化
	for( int i = 0; i < PRT_MAX_OMIT; i++ ){

		pPrtInfOmitWork->ItemSeq	= NULL;
		pPrtInfOmitWork->Omit		= NULL;
		pPrtInfOmitWork->OmitStr.Empty();
		pPrtInfOmitWork++;
	}

	CdbUcInfSubOmit	rc( m_pDB );

	try{

		rc.Init( nFormSeq );

		// データ取得
		while( !rc.IsEOF() ){

			pPrtInfOmit->ItemSeq	= rc.m_ItemSeq;
			pPrtInfOmit->Omit		= rc.m_Omit;
			pPrtInfOmit->OmitStr	= rc.m_OmitStr;

			// 次のレコードへ移動
			rc.MoveNext();
			pPrtInfOmit++;
		}

		rc.Fin();
	}
	catch( ... ){

		TRACE( "\n# ERROR: CJobFunction::GetDbDataTableUcInfSubOmit()\n" );

		if( rc.IsOpen() ){

			rc.Fin();
			return FUNCTION_NG;
		}
	}

	return FUNCTION_OK;
}


////////////////////////////////////////////////////////////////////////////////
// CheckDbDataFgFuncNull()
//
// ■空行かどうかを判定する。
//
// 引数		：CdbUc000Common*	prc					DBアクセスクラスポインタ
//
// 戻り値	：BOOL				TRUE ：空行
//								FALSE：空行でない
//
////////////////////////////////////////////////////////////////////////////////
BOOL CprtJobFunction::CheckDbDataFgFuncNull(CdbUc000Common* prc)
{
	if( (prc->m_FgFunc == ID_FGFUNC_NULL) ||
		(prc->m_FgFunc == ID_FGFUNC_SYOKEINULL) ||
		(prc->m_FgFunc == ID_FGFUNC_CHUKEINULL) ){

		// 次のレコードへ移動
		prc->MoveNext();

		return TRUE;
	}

	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
// CheckDbDataFgFuncKei()
//
// ■計行かどうかを判定する。
//
// 引数		：CdbUc000Common*	prc					DBアクセスクラスポインタ
//
// 戻り値	：BOOL				TRUE ：計行
//								FALSE：計行でない
//
////////////////////////////////////////////////////////////////////////////////
BOOL CprtJobFunction::CheckDbDataFgFuncKei(CdbUc000Common* prc)
{
	if(	(prc->m_FgFunc == ID_FGFUNC_SYOKEI) ||
		(prc->m_FgFunc == ID_FGFUNC_CHUKEI) ||
		(prc->m_FgFunc == ID_FGFUNC_RUIKEI) ||
		(prc->m_FgFunc == ID_FGFUNC_PAGEKEI) ){
			
		return TRUE;
	}

	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
// CheckDbDataFgFuncData()
//
// ■データ行かどうかを判定する。
// ０円判定行の場合はTRUEを返す。
//
// 引数		：CdbUc000Common*	prc					DBアクセスクラスポインタ
//
// 戻り値	：BOOL				TRUE ：合計行
//								FALSE：合計行でない
//
////////////////////////////////////////////////////////////////////////////////
BOOL CprtJobFunction::CheckDbDataFgFuncData(CdbUc000Common* prc)
{
	if(	prc->m_FgFunc != ID_FGFUNC_DATA ){	
		return TRUE;
	}

	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
// CheckOmit()
//
// ■省略文字の使用可否判定
//
// 引数		：CString			strTarget			文字列
//			：CString			strOld				前行の文字列
//			：int				index				省略文字リストのインデックス
//			：PRT_INF_OMIT*		pPrtInfOmit			省略文字リストへのポインタ
//
// 戻り値	：BOOL				使用可否			TRUE ：使用する
//													FALSE：使用しない
//
////////////////////////////////////////////////////////////////////////////////
BOOL CprtJobFunction::CheckOmit(CString strTarget, CString strOld, int index, PRT_INF_OMIT* pPrtInfOmit)
{
	// 省略文字が使用不可の場合
	if( pPrtInfOmit[index].Omit == 0 ){

		return FALSE;
	}

	// 前行の文字列と一致しなかった場合
	if( strTarget.Compare( strOld ) != 0 ){

		return FALSE;
	}

	// 文字列が改行コード、スペース（全角・半角）のみ、まはたNULLの場合
	strTarget.Replace( _T( "　" ), _T( "" ) );	// 全角スペースは、Trimで削除されない
	if( strTarget.Trim().IsEmpty() ){			// 改行コード、半角半角スペースを削除

		return FALSE;
	}

	// MEMO：上記の条件分岐により、前行の文字列チェックはカバーされている
//	// 前行の文字列が、NULL、改行コード、スペース（全角・半角）のみの場合
//	strOld.Replace( _T( "　" ), _T( "" ) );
//	if( strOld.Trim().IsEmpty() ){
//
//		return FALSE;
//	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
// ShowFieldOfVal()
//
// ■金額欄の表示／非表示を設定する
//
// 引数		：CString			strTarget			文字列
//
// 戻り値	：CString			変換後の文字列		変換前の文字列／NULL文字
//
////////////////////////////////////////////////////////////////////////////////
CString CprtJobFunction::ShowFieldOfVal(CString strTarget, CdbUc000Common* prc, CString strFormat /*= _T( "" )*/, int nBit)
{
	// 金額欄出力 OFF
	if( m_PrtInfCommon.OutKingaku == 0 ){
		strTarget.Empty();
		return strTarget;
	}

	// 計行でデータ行がNULLの0→非表示
	//				　 合計の0→表示(ex.100 + (-100) = 0
	if( nBit != 0 ){
		if(	CheckDbDataFgFuncKei( prc ) ){
			// 0円の場合、非表示
			if ( strTarget.Trim() == "0"){
				// 指定のビットからサインを取得
				if( !( prc->m_ShowKeiZero & nBit ) ){
					strTarget.Empty();
					return strTarget;
				}
			}
		}
	}

	//*****************廃止*****************
	// 計行は0円時、常に非表示
	// 計行か？
	//if(	CheckDbDataFgFuncKei( prc ) ){
	//	// 0円の場合、非表示
	//	if ( strTarget.Trim() == "0"){
	//		strTarget.Empty();
	//		return strTarget;
	//	}
	//}
	//*****************廃止*****************


	return ConvertNum( strTarget, strFormat );	
}

////////////////////////////////////////////////////////////////////////////////
// ConvertNum()
//
// ■文字列を、フォーマット文字列に従い、変換する
// 　※半角数字以外を引数に指定した場合は、「0」を返す
//
// 引数		：CString			strTarget			文字列（数字）
//								strFormat			フォーマット文字列
//
// 戻り値	：CString			変換後の文字列
//
////////////////////////////////////////////////////////////////////////////////
CString CprtJobFunction::ConvertNum(CString strTarget, CString strFormat)
{
	// 空欄、およびフォーマット文字列が空の場合は、そのまま返す
	if( strTarget.IsEmpty() ||  strFormat.IsEmpty() ){

		return strTarget;
	}

	char	szTarget[32];		// 変換前文字列用バッファ
	char	szTargetNew[32];	// 変換後文字列用バッファ
	char*	pszTarget	= NULL;
	char*	pszFormat	= NULL;
	
	ZeroMemory( szTarget, sizeof( szTarget ) );
	ZeroMemory( szTargetNew, sizeof( szTargetNew ) );

	pszTarget = (LPSTR)(LPCTSTR)strTarget;
	pszFormat = (LPSTR)(LPCTSTR)strFormat;
	int	nRet = l_retn();

	l_defn( 0x16 );

	l_input( szTarget, pszTarget );

	l_print( szTargetNew, szTarget, pszFormat );

	l_defn( nRet );

	// 空文字を削除
	strTarget = szTargetNew;
	strTarget.TrimLeft();

	// 小数点以下の0の調整
//	if( 0/*サインに変更？*/ ){
//		strTarget = FixUnderDotZero( strTarget );
//	}

	// マイナスを"△"に変更
	//strTarget = ChangeMinusToTriangle( strTarget );

	return strTarget;
}

// No.200903 del -->
//////////////////////////////////////////////////////////////////////////////////
//// MakeFormatOfNum()
////
//// ■フォーマット文字列を作成する
////
//// 　※"sss,sss,ss9.99"の形にする	"s"：データがなければスペースを返す
////									"9"：データがなければ「0」を返す
////
//// 引数		：int				nIntDigit			整数の桁数
////			：int				nDecDigit			小数点以下の桁数（0）
////			：BOOL				bFgComma			TRUE :カンマ有
////													FALSE:カンマ無
////
//// 戻り値	：CString			フォーマット文字列
////
//////////////////////////////////////////////////////////////////////////////////
//CString CprtJobFunction::MakeFormatOfNum(int nIntDigit, int nDecDigit /*= 0*/, BOOL bFgComma /*= TRUE*/)
// No.200903 del <--
// No.200903 add -->
////////////////////////////////////////////////////////////////////////////////
// MakeFormatOfNum()
//
// ■フォーマット文字列を作成する
//
// 　※"sss,sss,ss9.99"の形にする	"s"：データがなければスペースを返す
//									"9"：データがなければ「0」を返す
//
// 引数		：int				nIntDigit			整数の桁数
//			：int				nDecDigit			小数点以下の桁数（0）
//			：BOOL				bFgComma			TRUE :カンマ有
//													FALSE:カンマ無
//			：BOOL				bComma13			TRUE :13桁目のカンマ有	S,SSS,SSS,SSS,SS9
//													FALSE:13桁目のカンマ無	SSSS,SSS,SSS,SS9
//
// 戻り値	：CString			フォーマット文字列
//
////////////////////////////////////////////////////////////////////////////////
CString CprtJobFunction::MakeFormatOfNum(int nIntDigit, int nDecDigit /*= 0*/, BOOL bFgComma /*= TRUE*/, BOOL bComma13 /*= TRUE*/)
// No.200903 add <--
{
	CString strFormat = _T( "" );

	// 小数点以下
	if( nDecDigit > 0 ){

		for( int i = 0; i < nDecDigit; i++ ){

			strFormat = "9" + strFormat;
		}

		// 小数点の追加
		strFormat = "." + strFormat;
	}

	// 整数
	strFormat = "9" + strFormat;	// 9.99

	if( nIntDigit > 0 ){
		for( int i = 2; i <= nIntDigit; i++ ){

			// 4, 7, 10･･･桁目の場合にカンマ付きでセットする
// No.200903 del -->
			//if( i != 1 && i % 3 == 1 && bFgComma ){
// No.200903 del <--
// No.200903 add -->
			if(i != 1 && i % 3 == 1 && bFgComma && (bComma13 == TRUE || i < 13))	{
// No.200903 add <--
				strFormat = "s," + strFormat;		// s,ss9
			}
			else{

				strFormat = "s" + strFormat;		// ss9
			}
		}
	}
	return strFormat;
}

////////////////////////////////////////////////////////////////////////////////
// StrDivision()
//
// ■文字列を分割する
//
// ▼使用例
// 　（1）2行分割を指定し、1行のみ（改行コードなし）入力した場合
//			｜----------｜
//			｜　　　　　｜	・下詰めとなる
// 			｜----------｜
// 			｜  １行目  ｜
// 			｜----------｜
// 
// 　（2）2行分割を指定し、2行入力した場合
// 			｜----------｜
// 			｜  １行目  ｜
// 			｜----------｜
// 			｜  ２行目  ｜
// 			｜----------｜
// 
// 　（3）2行分割を指定し、3行入力した場合
// 			｜----------｜
// 			｜  ２行目  ｜	・1行目が破棄され、2行目、3行目が出力される
// 			｜----------｜
// 			｜  ３行目  ｜
// 			｜----------｜
// 
// 引数		：CString			strTarget			文字列
//			：CStringArray*		pstrArray			分割後の文字列を格納
//			：int				nSize				格納先のサイズ
//
////////////////////////////////////////////////////////////////////////////////
void CprtJobFunction::StrDivision(CString strTarget, CStringArray* pstrArray, int nSize)
{
	// 不当なサイズなら何もしない
	if( nSize <= 0 ){

		return;
	}

	int	nChCode	= 0;							// 改行コードの位置
	int nLen	= 0;							// 文字列の長さ
	int	iLast	= (nSize - 1);					// 格納先の最終インデックス
	CString	strWork;							// 作業用

	// 格納先の初期化
	pstrArray->RemoveAll();
	pstrArray->SetSize( nSize );

	while( 1 ){

		// 配列内の要素の移動
		for( int i = 0; i < iLast; i++ )
		{
			strWork = pstrArray->GetAt( i + 1 );
			pstrArray->SetAt( i, strWork );
			pstrArray->SetAt( (i + 1), NULL );
		}

		// 改行コードの検索
		nChCode = strTarget.Find( PRT_FORMAT_CHCODE );

		// 改行コードが見つからない場合は、文字列をそのまま格納
		if( nChCode < 0 ){

			pstrArray->SetAt( iLast, strTarget );
			break;
		}

		// 改行コードより前の文字列を格納
		pstrArray->SetAt( iLast, strTarget.Left( nChCode ) );

		// 次のターゲット文字列を作成
		nLen = strTarget.GetLength();
//		strTarget = strTarget.Right( nLen - (nChCode + 1) );
		strTarget = strTarget.Right( nLen - (nChCode + 2) );	// 「2」は、改行コードの文字数
																// 　文字数を直接指定することは好ましくないが...

		// NULL文字列なら終了
		if( strTarget.IsEmpty() ){

			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// StrArrayEmpty()
//
// ■文字列配列の初期化
//
// 引数		：CString*			pstrDevision		対象の文字列
//			：int				size				配列のサイズ
//
////////////////////////////////////////////////////////////////////////////////
void CprtJobFunction::StrArrayEmpty(CString* pstrTarget, int nSize)
{
	int nElement = nSize / 4;	// 要素数

	for( int i = 0; i < nElement; i++ ){

		pstrTarget->Empty();
		pstrTarget++;
	}
}

////////////////////////////////////////////////////////////////////////////////
// GetTitleOfKnName()
//
// ■科目別タイトルを取得する
// 　※科目名を取得しない、または取得できない場合は、標準のタイトルを取得する。
//
// 引数		：int				index				出力リストのインデックス
//			：int				nPage				ページ番号
//
// 戻り値	：CString			帳表タイトル
//
////////////////////////////////////////////////////////////////////////////////
CString CprtJobFunction::GetTitleOfKnName(int index, int nPage)
{
	int		nFormSeq		= m_PrtInfMainList[index].Lite.FormSeq;
	int		nFormType		= m_PrtInfMainList[index].FormType;
// midori 160608 cor -->
//	int		nOutZeroNull	= m_PrtInfMainList[index].Option.OutZeroNull[0];
// ---------------------
	int		nOutZeroNull	= 0;

	if(m_nOutZero == 0)		nOutZeroNull = m_PrtInfMainList[index].Option.OutZeroNull[0];
	else					nOutZeroNull = 0;
// midori 160608 cor <--

	// 出力形式「通常」の場合
	if(  nFormType == ID_FORMTYPE_NORMAL ){

		switch( nFormSeq ){

			case ID_FORMNO_042:

				nFormSeq = ID_FORMNO_041;
				break;

			case ID_FORMNO_102:

				nFormSeq = ID_FORMNO_101;
				break;

			case ID_FORMNO_142:

				nFormSeq = ID_FORMNO_141;
				break;

			case ID_FORMNO_152:
			case ID_FORMNO_153:

				nFormSeq = ID_FORMNO_151;
				break;

			case ID_FORMNO_162:

				nFormSeq = ID_FORMNO_161;
				break;

			default:
				break;
		}
	}

	CString	strTitle	= GetTitleFromList( nFormSeq );	// リストからタイトルを取得

	// 特別対応：帳表⑯
	// 出力形式「通常」では、科目別タイトルを使用しない
	if( (nFormSeq == ID_FORMNO_161) && (nFormType == ID_FORMTYPE_NORMAL) ){
		return strTitle;
	}

	// 科目名を使用するかチェックする
	if( CheckUseTitleOfKnName( nFormSeq ) == FUNCTION_NG ){

		return strTitle;
	}

	CString	strKnName	= _T( "" );	// 科目名

	// 科目名を取得
	if( GetDbDataKnNameFirstRow( nFormSeq, nPage, &strKnName, nOutZeroNull ) == FUNCTION_NG ){
	
		return strTitle;
	}

	// 科目別タイトルの作成
	switch( nFormSeq ){

		case ID_FORMNO_051:

			strTitle.Format( PRT_FORMAT_TITLE_051, strKnName);
			break;

		case ID_FORMNO_111:

			strTitle.Format( PRT_FORMAT_TITLE_111, strKnName);
			break;

		case ID_FORMNO_161:
				
			strTitle.Format( PRT_FORMAT_TITLE_161, strKnName);
			break;

		case ID_FORMNO_162:
			
			strTitle.Format( PRT_FORMAT_TITLE_162, strKnName);
			break;

		case ID_FORMNO_021:
		case ID_FORMNO_031:
		case ID_FORMNO_041:
		case ID_FORMNO_091:
		case ID_FORMNO_101:

			strTitle = strKnName;
			break;

// 改良No.21-0086,21-0529 add -->
		case ID_FORMNO_081:
			if(bG_InvNo == FALSE) {
				return strTitle;
			}
			else {
				strTitle = strKnName;
			}
			break;
// 改良No.21-0086,21-0529 add <--

		default:

			return strTitle;
	}

	strTitle += PRT_FORMAT_TITLE_TAIL;
	return strTitle;
}

////////////////////////////////////////////////////////////////////////////////
// GetTitleFromList()
//
// ■リストからタイトルを取得する
//
// 引数		：int				nFormSeq			様式シーケンス番号
//
// 戻り値	：CString			帳表タイトル
//
////////////////////////////////////////////////////////////////////////////////
CString CprtJobFunction::GetTitleFromList(int nFormSeq)
{
	for( int i = 0; i <= UC_ID_FORMNO_MAX; i++ ){

		if( m_PrtInfTitleList[i].FormSeq == nFormSeq ){

			return m_PrtInfTitleList[i].Title;
		}
	}
	
	return CString();
}

////////////////////////////////////////////////////////////////////////////////
// CheckUseTitleOfKnName()
//
// ■リストから科目別タイトル使用可否を取得する
//
// 引数		：int				nFormSeq			様式シーケンス番号
//
// 戻り値	：BOOL				使用可否			TRUE ：使用する
//													FALSE：使用しない
//
////////////////////////////////////////////////////////////////////////////////
BOOL CprtJobFunction::CheckUseTitleOfKnName(int nFormSeq)
{
	for( int i = 0; i <= UC_ID_FORMNO_MAX; i++ ){

		if( m_PrtInfTitleList[i].FormSeq == nFormSeq ){
			
			if( m_PrtInfTitleList[i].OpTitleKnFg == 1 &&
// midori 160608 cor -->
//				m_PrtInfTitleList[i].OpTitleKn == 1 ){
// ---------------------
				m_PrtInfTitleList[i].OpTitleKn == 1 &&
				m_nOutZero == 0){
// midori 160608 cor <--
					
					return FUNCTION_OK;
			}
		}
	}

	return FUNCTION_NG;
}

////////////////////////////////////////////////////////////////////////////////
// SetTitleToList()
//
// ■リストにタイトルをセットする
//
// 引数		：int				nFormSeq			様式シーケンス番号
//			：CString			strTitle			新しいタイトル
//
////////////////////////////////////////////////////////////////////////////////
void CprtJobFunction::SetTitleToList(int nFormSeq, CString strTitle)
{
	for( int i = 1; i <= UC_ID_FORMNO_MAX; i++ ){

		if( m_PrtInfTitleList[i].FormSeq == nFormSeq ){

			m_PrtInfTitleList[i].Title = strTitle;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// InsertSlashInHead()
//
// ■第2文字列の先頭に「／」を挿入する。
// ※「／」が挿入されるのは、第1、第2文字列が、共にNULLでない場合
//
// ▼「支店名」の印字時に使用する。
//
//
// 引数		：CString			str1				第1文字列
//			：CString			str2				第2文字列
//
// 戻り値	：CString			編集後のstr2
//
////////////////////////////////////////////////////////////////////////////////
CString CprtJobFunction::InsertSlashInHead(CString str1, CString str2)
{
	// フラグが立っていれば、処理を実行
	if( m_bSlash ){

		if( !str1.IsEmpty() && !str2.IsEmpty() ){

			return (PRT_FORMAT_SLASH + str2);
		}
	}

	return str2;
}

////////////////////////////////////////////////////////////////////////////////
// ConvertNullIntoSpace()
//
// ■NULL文字列の場合、指定されたサイズの空白（半角スペース）文字列を返す。
// ※変換前に、文字悦の前後のスペースは削除されます。
//
// 引数		：CString			strTarget			変換対象の文字列
//
// 戻り値	：CString			変換後の文字列
//
////////////////////////////////////////////////////////////////////////////////
CString CprtJobFunction::ConvertNullIntoSpace(CString strTarget, int nSize)
{
	// 文字列の先頭と末尾にある空白文字を取り除く
//	strTarget.Trim();

	// 
	if( strTarget.IsEmpty() ){

		// 空白文字列の作成
		for( int i = 0; i < nSize; i++ ){

			strTarget += " ";	// 半角スペース
		}
	}

	return strTarget;
}

////////////////////////////////////////////////////////////////////////////////
// ConvertDate()
//
// ■日付データを、フォーマット文字列に従い、変換する。
//
// 引数		：int				nDate				元となる日付データ
//			：CString			strFormat			フォーマット文字列
// midori 200101 add -->
//			：int				swSub				編集サイン（元号1文字or2文字 等）
// midori 200101 add <--
//
// 戻り値	：CString			変換後の日付
//
////////////////////////////////////////////////////////////////////////////////
// midori 200101 del -->
//CString CprtJobFunction::ConvertDate(int nDate, CString strFormat, int swKes /*= 0*/)
// midori 200101 del <--
// midori 200101 add -->
// ※swKes に数字を渡している箇所はコメントになっていたのでこちらもカット
// 代わりに編集用のサインに使用。
CString CprtJobFunction::ConvertDate(int nDate, CString strFormat, int swSub /*=0*/)
// midori 200101 add <--
{
	CdateConvert	dateCvt;
	CString			strCvt	= _T( "" );
	
// midori 181101 del -->
	//if( swKes == SchSeqCvr ){	// 表紙		
	//	dateCvt.Convert( nDate, m_PrtInfCommon.CoRekinen );
	//}
	//else{
	//	dateCvt.Convert( nDate );
	//}
// midori 181101 del <--
// midori 181101 add -->
	dateCvt.Convert( nDate );
// midori 181101 add <--

	if( dateCvt.m_nGengou != GT_REIGAI ){

		if( strFormat.Compare( PRT_FORMAT_DATE_A1 ) == 0 ){

			strCvt.Format( strFormat, dateCvt.m_nWareki, dateCvt.m_nMonth );
		}
// midori 200101 add -->
		else if( strFormat.Compare( PRT_FORMAT_DATE_A2 ) == 0 ){

			strCvt.Format( strFormat, dateCvt.m_nWareki );
		}
// midori 200101 add <--
		else if( strFormat.Compare( PRT_FORMAT_DATE_B1 ) == 0 ){

			strCvt.Format( strFormat, dateCvt.m_nWareki, dateCvt.m_nMonth, dateCvt.m_nDay );
		}
		else if( strFormat.Compare( PRT_FORMAT_DATE_C1 ) == 0 ){

			strCvt.Format( strFormat, dateCvt.m_nWareki, dateCvt.m_nMonth, dateCvt.m_nDay );
		}
		else if( strFormat.Compare( PRT_FORMAT_DATE_C2 ) == 0 ){

			strCvt.Format( strFormat, dateCvt.m_nWareki, dateCvt.m_nMonth, dateCvt.m_nDay );
		}
		else if( strFormat.Compare( PRT_FORMAT_DATE_D1 ) == 0 ){
// midori 200101 del -->
			//strCvt.Format( strFormat, dateCvt.m_szGengouHead, dateCvt.m_nWareki );
// midori 200101 del <--
// midori 200101 add -->
			if(swSub == 1)	strCvt.Format( strFormat, dateCvt.m_szGengou, dateCvt.m_nWareki );
			else			strCvt.Format( strFormat, dateCvt.m_szGengouHead, dateCvt.m_nWareki );
// midori 200101 add <--
		}
		else if( strFormat.Compare( PRT_FORMAT_DATE_E1 ) == 0 ){

			strCvt.Format( strFormat, dateCvt.m_szGengouHead, dateCvt.m_nWareki, dateCvt.m_nMonth );
		}
// midori 200101 add -->
		else if( strFormat.Compare( PRT_FORMAT_DATE_E2 ) == 0 ){

			strCvt.Format( strFormat, dateCvt.m_szGengouHead, dateCvt.m_nWareki, dateCvt.m_nMonth, dateCvt.m_nDay );
		}
// midori 200101 add <--
// midori 181101 del -->
//		else if( strFormat.Compare( PRT_FORMAT_DATE_F1 ) == 0 ){
//
//			strCvt.Format( strFormat, dateCvt.m_nWareki, dateCvt.m_nMonth, dateCvt.m_nDay );
//		}
// midori 181101 del <--
		else if( strFormat.Compare( PRT_FORMAT_DATE_G1 ) == 0 ){
// midori 200101 del -->
			//strCvt.Format( strFormat, dateCvt.m_szGengouHead );
// midori 200101 del <--
// midori 200101 add -->
			if(swSub == 1)	strCvt.Format( strFormat, dateCvt.m_szGengou );
			else			strCvt.Format( strFormat, dateCvt.m_szGengouHead );
// midori 200101 add <--
		}
		else if( strFormat.Compare( PRT_FORMAT_DATE_F2 ) == 0 ){

			strCvt.Format( strFormat, dateCvt.m_nSeireki, dateCvt.m_nMonth, dateCvt.m_nDay );
		}
	}
	else{
// midori 200101 del -->
//		// 決算書業務の項目登録において、「各ページの年月日を表紙と異なる表示期間にする」がONにも関わらず
//		// 空欄の場合は平成  年  月  月のように表示する
//		if( swKes == SchSeqCvr ){	
//// midori 181101 del -->
////			if( strFormat.Compare( PRT_FORMAT_DATE_F1 ) == 0 )			strCvt = PRT_FORMAT_DATE_F3;
////			else if( strFormat.Compare( PRT_FORMAT_DATE_F2 ) == 0 )		strCvt = PRT_FORMAT_DATE_F4;
//// midori 181101 del <--
//// midori 181101 add -->
//			if( strFormat.Compare( PRT_FORMAT_DATE_F2 ) == 0 )		strCvt = PRT_FORMAT_DATE_F4;
//// midori 181101 add <--
//		}
// midori 200101 del <--
	}

	return strCvt;
}

// midori 181101 del -->
//////////////////////////////////////////////////////////////////////////////////
//// ConvertDateCover()
////
//// ■表紙用の日付変換を行う
////
//// 引数		：int				nDate				元となる日付データ
////			：int				nType				0:年 1:月 2:日
////
//// 戻り値	：CString			変換後の文字列
////
//////////////////////////////////////////////////////////////////////////////////
//CString CprtJobFunction::ConvertDateCover(int nDate, int nType)
//{
//	CString			strRet;
//	CdateConvert	dateCvt;
//
//	// 日付変換（西暦の場合、和暦に変換）
//	dateCvt.Convert( nDate, m_PrtInfCommon.CoRekinen );
//
//	switch( nType ){
//		case FieldCvrDateYear:
//			if(m_PrtInfCommon.CoRekinen != 0)	strRet.Format( "%d", dateCvt.m_nSeireki );
//			else								strRet.Format( "%d", dateCvt.m_nWareki );
//			break;
//
//		case FieldCvrDateMonth:
//			strRet.Format( "%d", dateCvt.m_nMonth );
//			break;
//
//		case FieldCvrDateDay:
//			strRet.Format( "%d", dateCvt.m_nDay );
//			break;
//
//		default:
//			strRet.Empty();
//			break;
//	}
//
//	return strRet;
//}
// midori 181101 del <--

////////////////////////////////////////////////////////////////////////////////
// GetPageAll()
//
// ■1帳表の総ページ数を取得する。
// 　※出力形式が「通常」の場合は、2つ（または3つ）の帳表の中で、
// 　　1番大きなページ数を用いる。
//
// 引数		：int				index				出力リストのインデックス
//			：int				nNameKeiBitArray	計の名称のビット配列
//
// 戻り値	：int			ページ数
//
////////////////////////////////////////////////////////////////////////////////
int CprtJobFunction::GetPageAll(int index, int nNameKeiBitArray)
{
	int nFormSeq		= m_PrtInfMainList[index].Lite.FormSeq;
// midori 160608 cor -->
//	int	OutZero			= m_PrtInfMainList[index].Option.OutZeroNull[0];
// ---------------------
	int	OutZero			= 0;
// midori 160608 cor <--
	int nRetPage		= 0;

// midori 160608 add -->
	if(m_nOutZero == 0)	OutZero = m_PrtInfMainList[index].Option.OutZeroNull[0];
	else				OutZero = 0;
// midori 160608 add <--
	// 「個別」
	if( m_PrtInfMainList[index].FormType == ID_FORMTYPE_KOBETU ){
// midori 190505 del -->
		//nRetPage = GetDbDataPageAll( nFormSeq, m_pDB, OutZero, nNameKeiBitArray, ID_FORMTYPE_KOBETU );
// midori 190505 del <--
// midori 190505 add -->
		nRetPage = GetDbDataPageAll( nFormSeq, m_pDB, OutZero, nNameKeiBitArray, ID_FORMTYPE_KOBETU, -1);
// midori 190505 add <--
		// 
		if( nRetPage == FUNCTION_NG ){

			return FUNCTION_NG;
		}
	}
	// 「通常」
	else{
		int nRetPage2 = 0;
		int nRetPage3 = 0;
// midori 190505 add -->
		int hoho = 0;
// midori 190505 add <--

		switch( nFormSeq ){

			case ID_FORMNO_041:
			case ID_FORMNO_042:
// midori 190505 del -->
				//nRetPage	= GetDbDataPageAllSub(index, ID_FORMNO_041, m_pDB, nNameKeiBitArray, 1);
				//nRetPage2	= GetDbDataPageAllSub(index, ID_FORMNO_042, m_pDB, nNameKeiBitArray, 2);
// midori 190505 del <--
// midori 190505 add -->
				nRetPage	= GetDbDataPageAllSub(index, ID_FORMNO_041, m_pDB, nNameKeiBitArray, 1);
				nRetPage2	= GetDbDataPageAllSub(index, ID_FORMNO_042, m_pDB, nNameKeiBitArray, 2);
// midori 190505 add <--
				m_PrtInfMainList[index].Option.nPageNum[0]	= nRetPage;
				m_PrtInfMainList[index].Option.nPageNum[1]	= nRetPage2;
				break;

			case ID_FORMNO_101:
			case ID_FORMNO_102:
// midori 190505 del -->
				//nRetPage	= GetDbDataPageAllSub(index, ID_FORMNO_101, m_pDB, nNameKeiBitArray, 1);
				//nRetPage2	= GetDbDataPageAllSub(index, ID_FORMNO_102, m_pDB, nNameKeiBitArray, 2);
				////nRetPage	= GetDbDataPageAll( ID_FORMNO_101, m_pDB, OutZero, nNameKeiBitArray );
				////nRetPage2	= GetDbDataPageAll( ID_FORMNO_102, m_pDB, OutZero, nNameKeiBitArray );
// midori 190505 del <--
// midori 190505 add -->
				nRetPage	= GetDbDataPageAllSub(index, ID_FORMNO_101, m_pDB, nNameKeiBitArray, 1);
				nRetPage2	= GetDbDataPageAllSub(index, ID_FORMNO_102, m_pDB, nNameKeiBitArray, 2);
// midori 190505 add <--
				m_PrtInfMainList[index].Option.nPageNum[0]	= nRetPage;
				m_PrtInfMainList[index].Option.nPageNum[1]	= nRetPage2;
				break;

			case ID_FORMNO_141:
			case ID_FORMNO_142:
// midori 190505 del -->
				//nRetPage	= GetDbDataPageAllSub(index, ID_FORMNO_141, m_pDB, nNameKeiBitArray, 1);
				//nRetPage2	= GetDbDataPageAllSub(index, ID_FORMNO_142, m_pDB, nNameKeiBitArray, 2);
				////nRetPage	= GetDbDataPageAll( ID_FORMNO_141, m_pDB, OutZero, nNameKeiBitArray );
				////nRetPage2	= GetDbDataPageAll( ID_FORMNO_142, m_pDB, OutZero, nNameKeiBitArray );
// midori 190505 del <--
// midori 190505 add -->
				nRetPage	= GetDbDataPageAllSub(index, ID_FORMNO_141, m_pDB, nNameKeiBitArray, 1);
				nRetPage2	= GetDbDataPageAllSub(index, ID_FORMNO_142, m_pDB, nNameKeiBitArray, 2);
// midori 190505 add <--
				m_PrtInfMainList[index].Option.nPageNum[0]	= nRetPage;
				m_PrtInfMainList[index].Option.nPageNum[1]	= nRetPage2;
				break;

			case ID_FORMNO_151:
			case ID_FORMNO_152:
			case ID_FORMNO_153:
// midori 190505 del -->
				//nRetPage	= GetDbDataPageAllSub(index, ID_FORMNO_151, m_pDB, nNameKeiBitArray, 1);
				//nRetPage2	= GetDbDataPageAllSub(index, ID_FORMNO_152, m_pDB, nNameKeiBitArray, 2);
				//nRetPage3	= GetDbDataPageAllSub(index, ID_FORMNO_153, m_pDB, nNameKeiBitArray, 3);
				////nRetPage	= GetDbDataPageAll( ID_FORMNO_151, m_pDB, OutZero, nNameKeiBitArray );
				////nRetPage2	= GetDbDataPageAll( ID_FORMNO_152, m_pDB, OutZero, nNameKeiBitArray );
				////nRetPage3	= GetDbDataPageAll( ID_FORMNO_153, m_pDB, OutZero, nNameKeiBitArray );
// midori 190505 del <--
// midori 190505 add -->
				nRetPage	= GetDbDataPageAllSub(index, ID_FORMNO_151, m_pDB, nNameKeiBitArray, 1);
				nRetPage2	= GetDbDataPageAllSub(index, ID_FORMNO_152, m_pDB, nNameKeiBitArray, 2);
				nRetPage3	= GetDbDataPageAllSub(index, ID_FORMNO_153, m_pDB, nNameKeiBitArray, 3);
// midori 190505 add <--
				m_PrtInfMainList[index].Option.nPageNum[0]	= nRetPage;
				m_PrtInfMainList[index].Option.nPageNum[1]	= nRetPage2;
				m_PrtInfMainList[index].Option.nPageNum[2]	= nRetPage3;
				break;

			case ID_FORMNO_161:
			case ID_FORMNO_162:
// midori 190505 del -->
				//nRetPage	= GetDbDataPageAllSub(index, ID_FORMNO_161, m_pDB, nNameKeiBitArray, 1);
				//nRetPage2	= GetDbDataPageAllSub(index, ID_FORMNO_162, m_pDB, nNameKeiBitArray, 2);
				////nRetPage	= GetDbDataPageAll( ID_FORMNO_161, m_pDB, OutZero, nNameKeiBitArray );
				////nRetPage2	= GetDbDataPageAll( ID_FORMNO_162, m_pDB, OutZero, nNameKeiBitArray );
// midori 190505 del <--
// midori 190505 add -->
				nRetPage	= GetDbDataPageAllSub(index, ID_FORMNO_161, m_pDB, nNameKeiBitArray, 1);
				nRetPage2	= GetDbDataPageAllSub(index, ID_FORMNO_162, m_pDB, nNameKeiBitArray, 2);
// midori 190505 add <--
				m_PrtInfMainList[index].Option.nPageNum[0]	= nRetPage;
				m_PrtInfMainList[index].Option.nPageNum[1]	= nRetPage2;
				break;

			default:
// midori 190505 del -->
				//nRetPage = GetDbDataPageAll( nFormSeq, m_pDB, OutZero, nNameKeiBitArray );
// midori 190505 del <--
// midori 190505 add -->
				nRetPage = GetDbDataPageAll( nFormSeq, m_pDB, OutZero, nNameKeiBitArray, 0, -1);
// midori 190505 add <--
				// 欄外登録の複数ページ対応により、欄外登録のページ数も考慮する
// midori 154982 del -->
				//if(nFormSeq == ID_FORMNO_051){
// midori 154982 del <--
// midori 154982 add -->
				if(nFormSeq == ID_FORMNO_051 && bG_Kanso != TRUE)	{
// midori 154982 add <--
					CdbUc052Tanaoroshi2 mfcRec(m_pDB);

// midori 155525 del -->
					//int nPageMaxRangai = mfcRec.GetPrintPageNum();
// midori 155525 del <--
// midori 155525 add -->
					int nPageMaxRangai = mfcRec.GetPrintPageNum(OutZero);
// midori 155525 add <--
					if(nPageMaxRangai > nRetPage)	nRetPage = nPageMaxRangai;
				}
				else if(nFormSeq == ID_FORMNO_091){
					CdbUc092Kaikakekin2 mfcRec(m_pDB);

// midori 155525 del -->
					//int nPageMaxRangai = mfcRec.GetPrintPageNum();
// midori 155525 del <--
// midori 155525 add -->
					int nPageMaxRangai = mfcRec.GetPrintPageNum(OutZero);
// midori 155525 add <--
					if(nPageMaxRangai > nRetPage)	nRetPage = nPageMaxRangai;
				}
				break;
		}

		// 
		if( nRetPage == FUNCTION_NG || nRetPage2 == FUNCTION_NG || nRetPage3 == FUNCTION_NG ){

			return FUNCTION_NG;
		}

		// 1番大きな値を選択する
		if( nRetPage < nRetPage2 ){
			
			nRetPage = nRetPage2;
		}
		if( nRetPage < nRetPage3 ){
			
			nRetPage = nRetPage3;
		}
	}

	return nRetPage;
}

////////////////////////////////////////////////////////////////////////////////
// SetNprtParam()
//
// ■プリントモジュールが使用する出力情報定義パラメータ登録
//
// 引数		：なし
//
// 戻り値	：なし
//
////////////////////////////////////////////////////////////////////////////////
void CprtJobFunction::SetNprtParam()
{
	const CString	FN = _T("CprtJobFunction::SetNprtParam()");
	CString			strLog;
	DbgWriteLog( FN, _T("START") );

	// 2010/08/XX 業務統合メニュー対応
	// 初期化 → 常に、顧問先情報、zvolume の設定内容をダイアログに表示する
	m_icsPset.psSigCoprn = 0x00;

	// プリンタ名
	// 2010/07/XX 一括印刷対応
	//PCOM_GetString( _T( "SelectPrinter" ), m_icsPset.psPrinterName );
	// 2010/07/31 仕様変更（7/30） 対応 
	// 一括印刷アプリからの起動→出力？
	switch ( g_BatchPrt.GetBatchMode() ) {
	case ID_BATCHMODE_PRT:
	case ID_BATCHMODE_CDR:
		// 一括印刷用 プリンタ名取得（UCOM情報）
		memset( m_icsPset.psPrinterName, 0x00, sizeof(m_icsPset.psPrinterName) );		// プリンタ名
		strcpy_s( (char*)m_icsPset.psPrinterName, sizeof(m_icsPset.psPrinterName), g_BatchPrt.GetPrtName());
		break;

	default:
		// 通常起動、一括印刷設定時 は　通常のプリンタを使用
		// ・一括印刷：設定時、UCOM　I/F に プリンタ名の情報なし → プレビュー表示を行うため、通常用を使用する
		PCOM_GetString( _T( "SelectPrinter" ), m_icsPset.psPrinterName );
	}
	// ログ出力
	strLog.Format( _T("プリンタ名 NPRT_PAR psPrinterName [%s]"), m_icsPset.psPrinterName  );
	DbgWriteLog( FN, strLog );

	// ドキュメント名
// 24/01/16_ScoreLink cor -->
	//strcpy_s( (LPSTR)m_icsPset.psJobName, sizeof( m_icsPset.psJobName ), PRT_NAME_JOB );
// --------------------------
	if(g_BatchPrt.IsScoreLinkExec() == TRUE)	{
		strcpy_s( (LPSTR)m_icsPset.psJobName, sizeof( m_icsPset.psJobName ), g_BatchPrt.GetPdfName() );
	}
	else	{
		strcpy_s( (LPSTR)m_icsPset.psJobName, sizeof( m_icsPset.psJobName ), PRT_NAME_JOB );
	}
// 24/01/16_ScoreLink cor <--

	// その他の設定
	m_icsPset.psPaperSize	= DMPAPER_A4;			// 用紙サイズ
	m_icsPset.psOrientation	= DMORIENT_PORTRAIT;	// 方向
//2007.07.25：「給紙方法＝自動選択」固定のため修正
//	m_icsPset.psPaperBin	|= 0x8000;				// ホッパ（給紙）

	// 2010/07/XX 一括印刷対応
	// 一括印刷アプリからの起動？
	int	nPrtCnt = 1;
	if ( TRUE == g_BatchPrt.IsBatchExec()) {
		// 一括印刷用 部数指定（UCOM情報）
		nPrtCnt = g_BatchPrt.GetPrtCnt();
	}
	//m_icsPset.psCopies		= 1;					// 印刷部数
	m_icsPset.psCopies		= nPrtCnt;					// 印刷部数
	// ログ出力
	strLog.Format( _T("印刷部数 NPRT_PAR psCopies [%d]"), m_icsPset.psCopies  );
	DbgWriteLog( FN, strLog );

	m_icsPset.psPrevFlg		= 0x0100;				// プレビュー コントロール フラグ
	m_icsPset.psFncFlg		= m_lFncFlag;			// ファンクション フラグ

	// 印字位置調整
	m_icsPset.psPrnFlg		= 0;																	// 印刷フラグ
	strcpy_s( &m_icsPset.psPsecName[0], sizeof(m_icsPset.psPsecName), PRT_UCHIWAKE_PSECNAME );		// プログラム・セクション名
	strcpy_s( &m_icsPset.psListName[0], sizeof(m_icsPset.psListName), PRT_UCHIWAKE_LISTNAME );		// 帳票名

	// 2010/08/XX 業務統合メニュー対応
	// 顧問先名称、顧問先コード／会社名、会社コード 出力切替 パラメータセット
	SetNprtParTgInf( &m_icsPset );

	DbgWriteLog( FN, _T("END") );
}

// midori 181101 del -->
//////////////////////////////////////////////////////////////////////////////////
//// JobFilePrintToCsv()
////
//// ■Excel出力を実現するためのデータ登録処理
////
//// 引数		：UCHIWAKE_INFO				共通データ
////
//// 戻り値	：なし
////
//////////////////////////////////////////////////////////////////////////////////
//void CprtJobFunction::JobFilePrintToCsv(UCHIWAKE_INFO uInfo)
//{
//	WORD wHcel = 0;	// 帳表別の列の最大セル数
//	WORD wVcel = 0;	// 帳表別の行の最大セル数
//
//	// プリントモジュールが使用する出力情報定義パラメータの登録
//	SetNprtParam();
//
//	// CSV出力定義パラメータの初期化
//	ZeroMemory(&m_icsPcsv, sizeof(PCSV_PAR));
//
//	// 列＆行の最大セル数取得
//	GetCsvHcelVcel(uInfo.intFormSeq, uInfo.intFormType, &wHcel, &wVcel);
//
//    m_icsPcsv.cvVprn = &m_icsVprn;			// 印刷ジュール・オブジェクト
//    m_icsPcsv.cvFlag = 0;
//    m_icsPcsv.cvHcel = wHcel;				// 列の最大セル数
//    m_icsPcsv.cvVcel = wVcel;				// 行の最大セル数
//    m_icsPcsv.cvNpar = &m_icsPset;			// 出力定義パラメータ
//
//	// 帳表名を取得
//	CString		strTitle = GetStringByteSize(uInfo.strTitleName, sizeof(m_icsPcsv.cvLnam) - 1);
//	strTitle.Trim();
//
//	strcpy_s(m_icsPcsv.cvLnam, sizeof(m_icsPcsv.cvLnam), strTitle);									// 帳表名
//	strcpy_s(m_icsPcsv.cvCnam, sizeof(m_icsPcsv.cvCnam), m_PrtInfCommon.CoName);					// 会社名（後ろの空白は必須）
//	ConvertEEymdToEtrm(m_icsPcsv.cvEtrm, sizeof(m_icsPcsv.cvEtrm), m_PrtInfCommon.KikanEnd);		// 期末年月（BCD／年：和暦）
//}
// midori 181101 del <--

////////////////////////////////////////////////////////////////////////////////
// GetStringByteSize()
//
// ■文字列の先頭から指定バイト数分取り出す（全角/半角を考慮する）
//
// 引数		：CString					指定文字列
//			　int						バイト数
//
// 戻り値	：CString					先頭から指定バイト数分の文字列
//
////////////////////////////////////////////////////////////////////////////////
CString CprtJobFunction::GetStringByteSize(CString strData, int intByteSize)
{
	CString		strBuff		= _T("");	// バッファ
	char		chrBuff[200];			// 文字化け防止のためのバッファ
	int			intTrueMax	= 0;		// 全角/半角を考慮した分割位置数

	if ((strData != "") && (intByteSize > 0)) {
		// 指定バイト数以下ならそのまま返す
		int x = strData.GetLength();
		if (strData.GetLength() <= intByteSize) {
			return strData;
		}

		// CString→char
		ZeroMemory(chrBuff, sizeof(chrBuff));
		strcpy_s(chrBuff, sizeof(chrBuff), (const char *)strData);

		// 文字列判断(全角?半角?)
		if ((_ismbblead((unsigned int)chrBuff[intByteSize - 1]) == 1) && 
			(_ismbslead((const unsigned char*)&chrBuff[0], (const unsigned char*)&chrBuff[intByteSize - 1]) == -1)) {
			// 分割位置の文字列が全角2バイト文字の1バイト目なら1バイト前の文字で分割
			intTrueMax = intByteSize - 1;
		}
		else {
			intTrueMax = intByteSize;
		}

		// 指定文字数分取得
		strBuff = strData.Left(intTrueMax);
	}
	return strBuff;
}

// midori 181101 del -->
//////////////////////////////////////////////////////////////////////////////////
//// ConvertEEymdToEtrm()
////
//// ■Excel出力の期末年月を設定（期末年月日→BCD和暦年月）
////
//// 引数		：BYTE*						期末年月（BCD／年：和暦）
////			　long						サイズ
////			　long						期末年月日
////
//// 戻り値	：なし
////
//////////////////////////////////////////////////////////////////////////////////
//void CprtJobFunction::ConvertEEymdToEtrm(BYTE* pBcdBuf, long lngBcdSize, long lngEEymd)
//{
//	CVolDateDB		icsVolDate;						// DB期間モジュール
//	int				yymm = 0;
//
//	// 出力バッファの初期化
//	ZeroMemory(pBcdBuf, lngBcdSize);
//
//	// 現在の倍長フィールド取得
//	int	intDefn = l_retn();
//
//	// 倍長フィールド指定
//	l_defn(0x16);
//
//	// 和暦変換
//	icsVolDate.db_datecnv(lngEEymd, &yymm, 0, 0);	// 0:西暦→平成へ変換，0:年月日より取得
//	yymm	= (yymm / 100);							// yymmdd→yymm
//
//	// 年月をBCDに変換
//	int_bcd(pBcdBuf, yymm, 2);						// Arith使用:先頭2Byteのみ設定
//
//	// 倍長フィールドを元に戻す
//	l_defn(intDefn);
//}
// midori 181101 del <--

////////////////////////////////////////////////////////////////////////////////
// GetCsvHcelVcel()
//
// ■Excel出力時のパラメータ(列の最大セル数、行の最大セル数)を様式別に取得
//
// 引数		：int				nFormSeq			様式シーケンス番号
//			：int				nFormType			出力形式
//		　　：WORD				wHcel				列の最大セル数
//			：WORD				wVcel				行の最大セル数
//
// 戻り値	：なし
//
////////////////////////////////////////////////////////////////////////////////
void CprtJobFunction::GetCsvHcelVcel(int nFormSeq , int nFormType , WORD* wHcel , WORD* wVcel )
{
	// パラメータのFormSeq番号に対応する配列番号を返す
	switch (nFormSeq) {
	// ①預貯金
	case ID_FORMNO_011:
		*wHcel = 5;
		*wVcel = 66;				// Data=215mm(24*2行) → A4=296mm(66行)
		break;
	// ②受取手形
	case ID_FORMNO_021:
		*wHcel = 6;
		*wVcel = 103;				// Data=206mm(24*3行) → A4=296mm(103行)
		break;
	// ③売掛金
	case ID_FORMNO_031:
		*wHcel = 5;
		*wVcel = 74;				// Data=216mm(27*2行) → A4=296mm(74行)
		break;
	// ④-1仮払金
	case ID_FORMNO_041:
		*wHcel = 6;
		// 「通常」の場合
		if ( nFormType == ID_FORMTYPE_NORMAL ){
			*wVcel = 92;			// Data=81+92mm(11*2+8*4行) → A4=296mm(92行)
		}
		// 「個別」の場合
		else{
			*wVcel = 80;			// Data=213mm(29*2行) → A4=296mm(80行)
		}
		break;
	// ④-2貸付金
	case ID_FORMNO_042:
		*wHcel = 6;
		// 「通常」の場合
		if ( nFormType == ID_FORMTYPE_NORMAL ){
			*wVcel = 92;			// Data=81+92mm(11*2+8*4行) → A4=296mm(92行)
		}
		// 「個別」の場合
		else{
			*wVcel = 103;			// Data=218mm(19*4行) → A4=296mm(103行)
		}
		break;
	// ⑤棚卸資産
	case ID_FORMNO_051:
		*wHcel = 6;
		*wVcel = 76;				// Data=195mm(25*2行) → A4=296mm(76行)
		break;
	// ⑥有価証券
	case ID_FORMNO_061:
		*wHcel = 7;
		*wVcel = 92;				// Data=205mm(16*4行) → A4=296mm(92行)
		break;
	// ⑦固定資産
	case ID_FORMNO_071:
		*wHcel = 8;
		*wVcel = 89;				// Data=199mm(10*6行) → A4=296mm(89行)
		break;
	// ⑧支払手形
	case ID_FORMNO_081:
		*wHcel = 5;
		*wVcel = 96;				// Data=221mm(24*3行) → A4=296mm(96行)
		break;
	// ⑨買掛金
	case ID_FORMNO_091:
		*wHcel = 5;
		*wVcel = 77;				// Data=185mm(24*2行) → A4=296mm(77行)
		break;
	// ⑩-1仮受金
	case ID_FORMNO_101:
		*wHcel = 6;
		// 「通常」の場合
		if ( nFormType == ID_FORMTYPE_NORMAL ){
			// 出力されない行があるため、⑩-2を無視して行を指定する
//			*wVcel = 85;			// Data=109+62mm(12*3+7*2行) → A4=296mm(85行)
			*wVcel = 98;			// Data=109mm(12*3行) → A4=296mm(98行)
		}
		// 「個別」の場合
		else{
			*wVcel = 98;			// Data=218mm(24*3行) → A4=296mm(98行)
		}
		break;
	// ⑩-2源泉所得税
	case ID_FORMNO_102:
		*wHcel = 6;
		// 「通常」の場合
		if ( nFormType == ID_FORMTYPE_NORMAL ){
			// 出力されない行があるため、⑩-2を無視して行を指定する
//			*wVcel = 85;			// Data=109+62mm(12*3+7*2行) → A4=296mm(85行)
			*wVcel = 98;			// Data=109mm(12*3行) → A4=296mm(98行)
		}
		// 「個別」の場合
		else{
			*wVcel = 67;			// Data=230mm(26*2行) → A4=296mm(67行)
		}
		break;
	// ⑪借入金
	case ID_FORMNO_111:
		*wHcel = 6;
		*wVcel = 93;				// Data=191mm(15*4行) → A4=296mm(93行)
		break;
	// ⑫土地の売上高
	case ID_FORMNO_121:
		*wHcel = 8;
		*wVcel = 58;				// Data=204mm(10*4行) → A4=296mm(58行)
		break;
	// ⑬売上高等の事業所別
	case ID_FORMNO_131:
		*wHcel = 8;
		*wVcel = 102;				// Data=218mm(15*5行) → A4=296mm(102行)
		break;
	// ⑭-1役員報酬
	case ID_FORMNO_141:
		*wHcel = 11;
		// 「通常」の場合
		if ( nFormType == ID_FORMTYPE_NORMAL ){
			*wVcel = 94;			// Data=131+33mm(11*4+4*2行) → A4=296mm(94行)
		}
		// 「個別」の場合
		else{
			*wVcel = 99;			// Data=179mm(15*4行) → A4=296mm(99行)
		}
		break;
	// ⑭-2人件費
	case ID_FORMNO_142:
		// 「通常」の場合
		if ( nFormType == ID_FORMTYPE_NORMAL ){
			*wHcel = 11;
			*wVcel = 94;			// Data=131+33mm(11*4+4*2行) → A4=296mm(94行)
		}
		// 「個別」の場合
		else{
			*wHcel = 4;
			*wVcel = 59;			// Data=40mm(4*2行) → A4=296mm(59行)
		}
		break;
	// ⑮-1地代家賃
	case ID_FORMNO_151:
		// 「通常」の場合
		if ( nFormType == ID_FORMTYPE_NORMAL ){
			*wHcel = 6;
			*wVcel = 108;			// Data=88+33+33mm(8*4+3*4+3*4行) → A4=296mm(108行)
		}
		// 「個別」の場合
		else{
			*wHcel = 5;
			*wVcel = 103;			// Data=219mm(19*4行) → A4=296mm(103行)
		}
		break;
	// ⑮-2権利金等
	case ID_FORMNO_152:
		// 「通常」の場合
		if ( nFormType == ID_FORMTYPE_NORMAL ){
			*wHcel = 6;
			*wVcel = 108;			// Data=88+33+33mm(8*4+3*4+3*4行) → A4=296mm(108行)
		}
		// 「個別」の場合
		else{
			*wHcel = 5;
			*wVcel = 101;			// Data=212mm(18*4行) → A4=296mm(101行)
		}
		break;
	// ⑮-3工業所有権
	case ID_FORMNO_153:
		*wHcel = 6;
		// 「通常」の場合
		if ( nFormType == ID_FORMTYPE_NORMAL ){
			*wVcel = 108;			// Data=88+33+33mm(8*4+3*4+3*4行) → A4=296mm(108行)
		}
		// 「個別」の場合
		else{
			*wVcel = 110;			// Data=227mm(21*4行) → A4=296mm(110行)
		}
		break;
	// ⑯-1雑益
	case ID_FORMNO_161:
	// ⑯-2雑損失
	case ID_FORMNO_162:
		*wHcel = 5;
		*wVcel = 80;				// Data=216mm(20*3行) → A4=296mm(80行)
		break;
	// ⑰その他①
	case ID_FORMNO_171:
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
		*wHcel = 5;
		*wVcel = 74;				// Data=216mm(27*2行) → A4=296mm(74行)
		break;
	// ⑱その他②
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
		*wHcel = 4;
		*wVcel = 74;				// Data=216mm(27*2行) → A4=296mm(74行)
		break;
	default:
		break;
	}

}

//----------------------------------------------------------------------------------
// 顧問先名称、顧問先コード／会社名、会社コード 法人番号　出力切替
//
//	IN		NPRT_PAR*			出力条件パラメータ
//	RET		BOOL				TRUE=エラー，FALSE=正常
//
// 業務等当メニュー対応
// 全体の流れ
//	① 総合会社メニュー設定を参照し、印刷ダイアログに初期値をセット（顧問先固定の時に有効）
//     社マスタ作成時の設定を参照し、印刷ダイアログに初期値をセット
//	② 印刷ダイアログ表示、オペレータによるチェック ON/OFFを選択可能 → 印刷実行で選択確定 ③へ
//	③ 設定結果を出力前に再度参照し、振る舞いを決める
//
//	本関数は 下記 ① を実行
//	※）詳細 CprtJobFunction::GetTgNameCd()  関数コメント参照
//
// (※:P?) ⇒ 新総合会社情報_100420.doc ページ番号
// 2010/08/XX 業務統合メニュー対応
//----------------------------------------------------------------------------------
BOOL	CprtJobFunction::SetNprtParTgInf( NPRT_PAR* pPrtPar )
{
	const CString	FN = _T("CprtJobFunction::SetNprtParTgInf()");
	CString			strLog;
	DbgWriteLog( FN, _T("START") );

	if ( !pPrtPar ) {
		DbgWriteLog( FN, _T("END - パラメータ異常") );
		return FALSE;
	}

	// zvolumeテーブル参照
	if (m_pZmSub->VolumeOpen() != 0) {
		// エラー
		m_clsFunc.ZmSubErrMsg( m_pZmSub );
		return FALSE;
	}

	CCoinfCtl		clsCoInfCtl;			// 会社情報管理クラス
	CINF_REC		CoRec;					// 会社情報レコード格納バッファ
	CTBL_PAR		CtPar;					// 会社情報アクセスパケット
	TgPackData		tpdata;
	unsigned char	cCoCode[4];

	int_bcd( cCoCode, m_pZmSub->zvol->v_cod, sizeof( cCoCode ) );	// 会社コード変換

	//-------------------------------------------------------------------------------------
	// 顧問s会情報登録業務で、「会社名を顧問先情報で出力する 」が指定されている場合、
	// 印刷ダイアログの 該当チェックを ON にする情報を NPRT_PAR にセット
	//-------------------------------------------------------------------------------------
	// マスターか顧問先情報か
	//	0：固定していない／1：固定している（20100615_コモン情報グローバル関数.doc P3）
	int nRet = GET_TGCoCode( &tpdata );
	strLog.Format( _T("GET_TGCoCode() ret = [%d]"), nRet );
	DbgWriteLog( FN, strLog );
	// 顧問先が固定されている
	if ( 1 == nRet ) {		

		DbgWriteLog( FN, _T("GET_TGCoCode() Ret 顧問先固定状態") );
		strLog.Format( _T("tpdata.TgName=[%s], tpdata.TgCode=[%d]"), tpdata.TgName, tpdata.TgCode );
		DbgWriteLog( FN, strLog );

		//---------------------------------------------------------------------
		// 会社情報ファイル Open (※:P9)
		// 以降の CCoinfCtl クラス関数を実行するため先頭でOpenを行う
		//---------------------------------------------------------------------
		char		sdev[16]	={0}, tmpdev[16]	={0};
		PCOM_GetString( _T("MasterDevice"), tmpdev );
		wsprintf( sdev, _T("%s:"), tmpdev );
		// open 第一引数( 1:Read Mode / 3:Update Mode )
		if ( FALSE == clsCoInfCtl.OpenFile(1, sdev)) {
			ICSExitEx( clsCoInfCtl.GetErrorMessage() );

			// テーブルを閉じる
			m_pZmSub->VolumeClose();

			return FALSE;
		}

		//---------------------------------------------------
		// FindRecFromMcod() パラメータセット
		//---------------------------------------------------
		memset( &CoRec, '\0', sizeof(CINF_REC) );
		memset( &CtPar, '\0', sizeof(CTBL_PAR) );

		CtPar.tpDtyp = 1;						// 旧／ＤＢ（0:旧／1:DB）
		CtPar.tpMtyp = 0;						// マスタータイプ（MS=0 MA=2 MB=3）
		CtPar.tpApno = m_pZmSub->zvol->apno;	// アプリケーションNo
		memmove( CtPar.tpCcod, cCoCode, sizeof( cCoCode ) );			// マスター(会社)コード

		//---------------------------------------------------
		// マスター(会社)コードから総合会社情報レコードを取り出す
		// 新総合会社情報_100420.doc P12 (第一引数 未使用)
		//---------------------------------------------------
		nRet = clsCoInfCtl.FindRecFromMcod( NULL, &CoRec, &CtPar );
		strLog.Format( _T("CCoinfCtl::FindRecFromMcod() ret = [%d]"), nRet );
		DbgWriteLog( FN, strLog );

		//---------------------------------------------------
		// “会社名を顧問先名で出力”設定
		// CINF_REC.ciCopsw　総合会社情報設定値
		//---------------------------------------------------
		pPrtPar->psSigCoprn |= BIT_D0;			// 使用可能とする								(D0)
		if ( CoRec.ciCopsw & BIT_D0 ) {			// 顧問先コード、名称を出力する 設定の場合		(D0)
			pPrtPar->psSigCoprn  |=  BIT_D1;	// 会社名を顧問先名で出力する チェックON指定	(D1)
		}
		else {
			pPrtPar->psSigCoprn  &=  ~BIT_D1;	// 会社名を顧問先名で出力する チェックOFF指定	(D1)
		}

		//---------------------------------------------------
		// 法人番号取得
		//---------------------------------------------------
//		m_PrtInfCommon.HjNo_corec = CoRec.ciHjnNo1;

		strLog.Format( _T("CINF_REC.ciCopsw = [0x%08x]"), CoRec.ciCopsw );
		DbgWriteLog( FN, strLog );

		//---------------------------------------------------
		// 会社情報ファイル Close
		//---------------------------------------------------
		clsCoInfCtl.CloseFile();
	}
	else {
		DbgWriteLog( FN, _T("GET_TGCoCode() 固定されていない") );
	}

	//-------------------------------------------------------------------------------------
	// 会社コードを出力する 帳表の場合、以下のテーブルを参照し、出力の振る舞いを決める
	//
	//	zvolume.g_hsw　D0 (ON：会社コードを出力しない, OFF：会社コードを出力する)
	//																ICS中垣内様より
	//	→ 会社登録業務にて 出力する／しない を設定
	//	→ 出力アプリの 会社コード出力切替 選択機能は削除し、上記設定を参照する
	//	  （※ 元々コードを出力していないアプリは、使用禁止＆チェックをOFFにする）	
	// マイナンバー対応により法人番号は出力する（2015/06/03）
	//-------------------------------------------------------------------------------------
	// 【要注意！】
	// 　・勘定科目内訳書はコード出力なし固定（既存仕様）→印刷ダイアログでのコード出力指定は無視（非表示）する。
	// 　・よって、印刷ダイアログの設定として“会社コードを出力する”も非表示とする。
	//pPrtPar->psSigCoprn |= BIT_D8;				// “会社コードを出力する”指定を使用可能とする	(D8)
	//if ( !(pZmSub->zvol->g_hsw & BIT_D0) ) {	// 出力する指定の場合							(D0)
	//	pPrtPar->psSigCoprn &= ~BIT_D9;			// 会社コード出力 チェックON指定				(D9) OFF
	//}
	//else {
	//	pPrtPar->psSigCoprn |= BIT_D9;			// 会社コード出力 チェックOFF指定				(D9) ON
	//}
	pPrtPar->psSigCoprn &= ~BIT_D8;				// “会社コードを出力する”指定を使用不可能とする	(D8)
	pPrtPar->psSigCoprn &= ~BIT_D9;				// 会社コード出力 チェックOFF指定					(D9)

	//法人番号　追記 （法人業種のみ）
//	if((m_pZmSub->zvol->ind_type == 0x00) && (m_pZmSub->zvol->apno != 0x10) ){ 
//		pPrtPar->psSigCoprn |= BIT_D12;			// “法人番号を出力する”(D12)
//		CICSMNSub8 mnsub8;
//		mnsub8.SetPrintHojinNoFlag(&pPrtPar->psSigCoprn, m_pZmSub->zvol->g_hsw, 0);
//		// 出力する指定の場合(D3)
//		//if ( !(m_pZmSub->zvol->g_hsw & BIT_D3) ){
//		//	pPrtPar->psSigCoprn |= BIT_D13;	// 法人番号出力 チェックOFF指定(D13) ON
//		//}
//		//else{
//		//	pPrtPar->psSigCoprn &= ~BIT_D13;
//		//}
//	}
	
//	m_PrtInfCommon.HjNo_zvol = m_pZmSub->zvol->hojin_no;
	
	strLog.Format( _T("zvol->g_hsw = [%d], NPRT_PAR.psSigCoprn = [0x%08x]"), m_pZmSub->zvol->g_hsw, pPrtPar->psSigCoprn );
	DbgWriteLog( FN, strLog );


	// テーブルを閉じる
	m_pZmSub->VolumeClose();

	DbgWriteLog( FN, _T("END") );

	return TRUE;

}//CprtJobFunction::SetTgMnueCoCd

//----------------------------------------------------------------------------------
// 印刷文字列 取得  会社名称(コード) ／ 顧問先名称(コード) 
//
//	IN		CString*			出力結果返却
//	RET		BOOL				TRUE=顧問先固定状態，FALSE=顧問先解除状態 名称取得不可能
//
// 業務等当メニュー対応
// 全体の流れ
//	① 総合会社メニュー設定を参照し、印刷ダイアログに初期値をセット（顧問先固定の時に有効）
//     社マスタ作成時の設定を参照し、印刷ダイアログに初期値をセット
//	② 印刷ダイアログ表示、オペレータによるチェック ON/OFFを選択可能 → 印刷実行で選択確定 ③へ
//	③ 設定結果を出力前に再度参照し、振る舞いを決める
//
//	本関数は上記 ③ を実行
//	上記②の選択結果は 印刷ダイアログがm_icsPset メモリを直接更新。よってアプリは参照のみ。
//
//	１）名称、コードは 顧問先名 と 会社名 の出力切替をサポート
//		--------------------------------------------------------------
//		顧問先名で出力する		ON				OFF
//		--------------------------------------------------------------
//		出力名称				顧問先名		会社名
//		コード					顧問先コード	会社コード
//		--------------------------------------------------------------
//
//	２）コード出力する／しない の設定は、マスタ作成業務にて行う
//		各出力アプリでの設定 GUI は 削除する
//		→ 元々コードを出力していないアプリは、常に出力なしとする
//		   印刷ダイアログの コード出力選択チェックを 使用禁止状態で実行すること。
//
// 2010/08/XX 業務統合メニュー対応
//----------------------------------------------------------------------------------
BOOL	CprtJobFunction::GetTgNameCd( CString& strName )
{
	const CString	FN = _T("CprtJobFunction::GetTgNameCd()");
	CString			strLog;

	strLog.Format( _T("START") );
	DbgWriteLog( FN, strLog );

	TgPackData  tpdata;

	//------------------------------------------
	// 顧問先情報 or 会社情報 出力？
	//------------------------------------------
	strLog.Format( _T("m_icsPcsv.psSigCoprn = [%d]"), m_icsPset.psSigCoprn );
	DbgWriteLog( FN, strLog );
	// 顧問先情報？
	// D0：使用可能, D1：チェックON
	if (( BIT_D0 | BIT_D1 ) == ( m_icsPset.psSigCoprn & ( BIT_D0 | BIT_D1 ))) {
		// 顧問先情報取得
		//	顧問先状態　　0：固定していない／1：固定している（20100615_コモン情報グローバル関数.doc P3）
		int	nRet = GET_TGCoCode( &tpdata );
		strLog.Format( _T("GET_TGCoCode() ret = [%d]"), nRet );
		DbgWriteLog( FN, strLog );
		if ( 1 == nRet ) {
			strName = tpdata.TgName;
		}
	}
	// 会社情報？
	else {
		//*************************************************
		// 会社情報の場合、決算書の規定値を使用するので廃止
		//*************************************************
		// zvolumeテーブル参照
		//if (m_pZmSub->VolumeOpen() == 0) {
		//	// 会社名を参照
		//	strName = m_pZmSub->zvol->c_nam;

		//	// テーブルを閉じる
		//	m_pZmSub->VolumeClose();
		//}		
		strName = m_strCoNameCpdata;
	}

	DbgWriteLog( FN, _T("END") );

	return TRUE;
}//CprtJobFunction::GetTgNameCd

//----------------------------------------------------------------------------------
//	CVPrint::ErrorAbort() 実行関数
//
//	IN		CVPrint*			プリントモジュール
//			int					CVPrint::ErrorAbort() にセットするエラーコード
//			LPCTSTR				CVPrint::ErrorAbort() にセットするエラーメッセージ
//
//	RET		なし
//
//	MEMO	一括印刷を行うアプリは、CVPrint::ErrorAbort()は本関数より実行してください
//
//	2010/08/31 CVPrint::ErrorAbort() 実行後の一括印刷終了対応
//----------------------------------------------------------------------------------
void CprtJobFunction::CVPrintErrorAbortPrc( CVPrint* pPrtMdl, int ApErrCod /*= 0*/, LPCTSTR pErrMsg /*= NULL*/ )
{
	const CString	FN = _T("CprtJobFunction::CVPrintErrorAbortPrc()");
	CString			strLog;

	strLog.Format( _T("START   pPrtMdl=[0x%08x], ApErrCod=[%d], pErrMsg=[%s]"), pPrtMdl, ApErrCod, pErrMsg );
	DbgWriteLog( FN, strLog );

	// パラメータチェック
	if ( !pPrtMdl ) {
		DbgWriteLog( FN, _T("END - パラメータ異常") );
		return;
	}

	// 印刷中断実施情報セット
	switch ( g_BatchPrt.GetBatchMode() ) {
	case ID_BATCHMODE_PRT:				// 一括印刷 出力
	case ID_BATCHMODE_CDR:				// 一括印刷 CDR出力
		g_BatchPrt.SetPrtMdlOnErr();	// 印刷モジュール異常終了処理中フラグ ON
		g_BatchPrt.SetErrorExit();		// 一括印刷 異常終了フラグセット
		break;
	default:
		// 処理無
		break;
	}//switch

	// 印刷エラーセット
	pPrtMdl->ErrorAbort( ApErrCod, pErrMsg );

	DbgWriteLog( FN, _T("END") );

}//CprtJobFunction::CVPrintErrorAbortPrc

///////////////////////////////////////////////////////////////////////////////
// GetOutZero()
//
// ■金額０円フラグを取得する
//
// 戻り値	：int			金額０円フラグ	
//							エラーコード		FUNCTION_NG：失敗											
//
////////////////////////////////////////////////////////////////////////////////
int CprtJobFunction::GetOutZero(const CdbUcInfSub& mfcSub)
{
	int ret = 0;
	CCtrlSubGeneralVar cs(mfcSub);

	if(cs.bPrintZeroData)	ret |= BIT_D0;
	if(cs.bPrintNullData)	ret |= BIT_D1;

	return ret;
}

///////////////////////////////////////////////////////////////////////////////
// GetHideZero()
//
// ■金額０円を印字するフラグを取得する
//
// 戻り値	：int			金額０円を印字するフラグ											
//
////////////////////////////////////////////////////////////////////////////////
int CprtJobFunction::GetHideZero(const CdbUcInfSub& mfcSub)
{
	int ret = FALSE;
	CCtrlSubGeneralVar cs(mfcSub);

	if(cs.bPrintHideZero)	ret = TRUE;

	return ret;
}


///////////////////////////////////////////////////////////////////////////////
// DeletePageMapAll()
//
// ■マップ情報をクリアする
//
// 戻り値	：なし										
//
////////////////////////////////////////////////////////////////////////////////
void CprtJobFunction::DeletePageMapAll()
{
	//if( m_mapPrintAll.GetSize() ){
	//	POSITION pos = m_mapPrintAll.GetStartPosition();

	//	while(pos != NULL){
	//		int id; CPageMap* map;
	//		m_mapPrintAll.GetNextAssoc(pos,id,map);
	//		delete map;
	//	}
	//}

	//m_mapPrintAll.RemoveAll();
}

////////////////////////////////////////////////////////////////////////////////
// 未使用
// ※GetCountValを有効化すること！！
// GetIkkatuStringValue(CString strIkkatu, CdbUc000Common* rc)
//
// ■自動一括金のその他○件から０円行数を引いた文字列を返す
//
// 引数		：CString					”その他○件”（０円行含む）
//			　CdbUc000Common*			DBアクセスクラスポインタ
//
// 戻り値	：CString					”その他○件”（０円行含まず）
//
////////////////////////////////////////////////////////////////////////////////
CString CprtJobFunction::GetIkkatuStringValue(CString strIkkatu, CdbUc000Common* rc)
{
	int		nLen			= 0;
	int		nNumLen			= 0;
	int		nIkkatu			= 0;
	int		nIkkatuOutZero	= 0;
	CString strRet;


	// ０円行を含まない一括金額された行数取得
	// 本関数使用時は次の関数を有効化すること！！
	//nIkkatuOutZero = rc->GetCountVal( ID_FGSHOW_IKKATU, rc->m_NumGroup, m_PrtInfCommon.OutZero );

	// 文字列全体の長さから数字のバイト長を取得
	nLen	= strIkkatu.GetLength();
	nNumLen = nLen - 9;	// 9 == "その他 " + "件"の9バイト

	if( nNumLen < 0 ){
		return strRet;
	}
	
	// "その他 ○件"から○をint型でを抽出
	nIkkatu = _ttoi( strIkkatu.Mid( 7/*"その他 "*/, nNumLen ) );
	
	nIkkatuOutZero = nIkkatu - nIkkatuOutZero;

	// "その他 ○件"文言作成
	strRet.Format("%s %d%s", NAME_IKKATU_1, nIkkatuOutZero, NAME_IKKATU_2);

	return strRet;
}

//********************************************************************************
//	金額を加算/減算して合計値を求める
//	【引数】	CString *inTotalVal	…	合計金額値格納用変数のポインタ
//				CString inAddVal	…	加算金額値格納用変数のポインタ
//				int		inAddFlag	…	1:加算（省略可能），-1:減算
//	【戻り値】	なし
//********************************************************************************
void CprtJobFunction::KingakuAddFunc(CString *inTotalVal, CString inAddVal, int inAddFlag)
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
}

// virtual関数：各帳表で実装する処理
CprtJobFunction* CprtJobFunction::virGetPrintObject(int nSchList, int nFormSeq)
{
	switch( nSchList ){
		case ID_FORMNO_011:
			return new CprtUc011Yotyokin( m_pDB );

		case ID_FORMNO_021:
			return new CprtUc021Uketoritegata( m_pDB );

		case ID_FORMNO_031:
			return new CprtUc031Urikakekin( m_pDB );

		case ID_FORMNO_041:
			return new CprtUc041Karibarai( m_pDB );

		case ID_FORMNO_042:
			return new CprtUc042Kasituke( m_pDB );

		case ID_FORMNO_051:
			return new CprtUc051Tanaorosi( m_pDB );

		case ID_FORMNO_061:
			return new CprtUc061Yuukasyouken( m_pDB );

		case ID_FORMNO_071:
			return new CprtUc071Koteisisan( m_pDB );

		case ID_FORMNO_081:
			return new CprtUc081Siharaitegata( m_pDB );

		case ID_FORMNO_091:
			return new CprtUc091Kaikakekin( m_pDB );

		case ID_FORMNO_101:
			return new CprtUc101Kariukekin( m_pDB );

		case ID_FORMNO_102:
			return new CprtUc102Gensen( m_pDB );

		case ID_FORMNO_111:
			return new CprtUc111Kariirekin( m_pDB );

		case ID_FORMNO_121:
			return new CprtUc121Totiuriage( m_pDB );

		case ID_FORMNO_131:
			return new CprtUc131Uriagedaka( m_pDB );

		case ID_FORMNO_141:
			return new CprtUc141Yakuin( m_pDB );

		case ID_FORMNO_142:
			return new CprtUc142Jinkenhi( m_pDB );

		case ID_FORMNO_151:
			return new CprtUc151Tidaiyatin( m_pDB );

		case ID_FORMNO_152:
			return new CprtUc152Kenrikin( m_pDB );

		case ID_FORMNO_153:			
			return new CprtUc153Kougyou( m_pDB );

		case SchSeq160Cmn:
			switch( nFormSeq ){
				case ID_FORMNO_161:
					return new CprtUc161Zatueki( m_pDB );

				case ID_FORMNO_162:
					return new CprtUc162Zatuson( m_pDB );

				default:
					return NULL;
			}

// midori 190301 add -->
		case SchSeq161Cmn:
			return new CprtUc162Zatuson( m_pDB );
// midori 190301 add <--

		case SchSeq170Cmn:
			return new CprtUc171Sonota1( m_pDB );

		case SchSeq180Cmn:
			return new CprtUc181Sonota2( m_pDB );

		case SchSeq040Nml:
			return new CprtUc040KaribaraiKasituke( m_pDB );

		case SchSeq100Nml:
			return new CprtUc100KariukekinGensen( m_pDB );

		case SchSeq140Nml:
			return new CprtUc140YakuinJinkenhi( m_pDB );

		case SchSeq150Nml:
			return new CprtUc150TidaiyatinKenrikinKougyou( m_pDB );

		case SchSeq160Nml:
			return new CprtUc160ZatuekiZatuson( m_pDB );

		default:
			return NULL;
	}
			
	//return (CprtJobFunction*)NULL;
}

// virtual関数：各帳表で実装する処理
int CprtJobFunction::GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq)
{
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// GetPrintInfo(CprtJobFunction* parent)
//
// ■マップ、m_PrtInfCommon、省略文字情報を取得する
//
// 引数		：CprtJobFunction*		親クラスのポインタ
//
// 戻り値	：int					0
//
////////////////////////////////////////////////////////////////////////////////
int CprtJobFunction::GetPrintInfo(CprtJobFunction* parent)
{
	m_PrtInfCommon	= parent->m_PrtInfCommon;

	m_bSlash		= parent->m_bSlash;

	for(int i=0; i<PRT_MAX_OMIT; i++){
		m_PrtInfOmitList1[i] = parent->m_PrtInfOmitList1[i];
		m_PrtInfOmitList2[i] = parent->m_PrtInfOmitList2[i];
		m_PrtInfOmitList3[i] = parent->m_PrtInfOmitList3[i];
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// HideZeroMoney(CString strVal)
//
// ■strValが"0"のとき、NULLを返す
//
// 引数		：CString				「金額０円を印字する」の対象の項目
//
// 戻り値	：CString				引数が"0"のとき：NULL／それ以外:引数
//
////////////////////////////////////////////////////////////////////////////////
CString CprtJobFunction::HideZeroMoney(CString strVal)
{
	if( strVal.Compare("0") == 0 ){
		strVal.Empty();
	}

	return strVal;
}

////////////////////////////////////////////////////////////////////////////////
// FixUnderDotZero(CString strVal)
//
// ■小数点以下の値が0のみの場合、それをカットする
// 現状様式⑤と⑥の数量で使用
//
// 引数		：CString				小数点以下の値をカットする文字
//
// 戻り値	：CString				0のみの場合：カットした後の文字／それ以外：引数
//
////////////////////////////////////////////////////////////////////////////////
CString CprtJobFunction::FixUnderDotZero(CString strVal)
{
	int		nDotPoint	= strVal.Find( "." );
	int		nDotLen		= 0;
	CString strDec;

	// 整数はそのままリターン
	if( nDotPoint == -1 ){
		return strVal;
	}
	
	// 小数点を含む少数点以下の文字列長
	nDotLen = strVal.GetLength() - nDotPoint;

	// 小数点以下の値
	strDec = strVal.Right( nDotLen - 1 );	// -1は「.」分

	// 小数点以下の値が.00 または.000の場合、それらを削除
	if(( strDec.Compare( "00" ) == 0) || ( strDec.Compare( "000" ) == 0 )){
		strVal = strVal.Left( strVal.GetLength() - nDotLen );
	}

	return strVal;
}

////////////////////////////////////////////////////////////////////////////////
// ChangeMinusToTriangle(Cstring strVal)
//
// ■マイナス記号を△にする。現在は未使用(2012/07/20)
//
// 引数		：CString				金額
//
// 戻り値	：CString				先頭文字がマイナスなら、△に変更後の金額
//
////////////////////////////////////////////////////////////////////////////////
CString CprtJobFunction::ChangeMinusToTriangle(CString strVal)
{
	CString strSign = strVal.Left( 1 );

	if( strSign.Compare( "-" ) == 0 ){		// 金額の先頭が-(マイナス)

		// マイナスを△に置き換えた金額
		strVal = "△" + strVal.Right( strVal.GetLength() - 1 );
	}

	return strVal;
}

////////////////////////////////////////////////////////////////////////////////
// RemoveComma(Cstring strVal)
//
// ■整数部が9桁かつ小数部が3桁の場合、カンマを全て削除する
//
// 引数		：CString				金額
//
// 戻り値	：CString				カンマを削除した金額
//
////////////////////////////////////////////////////////////////////////////////
CString CprtJobFunction::RemoveComma(CString strVal)
{
	int nLenDot = 0;
	CString	strDecimal = 0;
	CString strRemvPoint;

	// 小数点の位置取得
	nLenDot		= strVal.Find( "." );

	// 小数部の値取得
	strDecimal = strVal.Right( 3 );
	
	if(( nLenDot < 11 ) && ( strDecimal.Find(".") == -1 )){
		// 整数部が9桁未満かつ、小数部が3桁未満ものはそのままリターン
		return strVal;
	}

	while ( strVal.Find( "," ) > 0 ){
		nLenDot = strVal.Find( "," );	// カンマの位置
		strVal.Delete( nLenDot, 1 );	// カンマ削除
	}

	return strVal;
}

////////////////////////////////////////////////////////////////////////////////
// FillSpace(Cstring strVal)
//
// ■指定されたバイト数まで半角スペースで埋める
//   入力文字列がバイト数以上の場合はなにもしない
//
// 引数		：CString				金額
//
// 戻り値	：CString				カンマを削除した金額
//
////////////////////////////////////////////////////////////////////////////////
CString CprtJobFunction::FillSpace(CString strBase, int nByte)
{
	while( strBase.GetLength() < nByte ){
		strBase += _T(" ");
	}

	return strBase;
}

////////////////////////////////////////////////////////////////////////////////
// ■出力形式が通常の様式の場合、「金額０円データを出力しない」「金額空欄データを出力しない」「金額０円を印字する」
// を個別に判断する必要があるため、当関数にてそのフラグをセットする
// 当関数は出力形式が「通常」のものからのみコールすること
//
// 引数		：int					インデックス
//			：int					様式シーケンス
//			：CDatabase*			データベースポインタ
//			：int					計名称
//			：int					uc_inf_sub.Form2
//
// 戻り値	：int					ページ数
//
////////////////////////////////////////////////////////////////////////////////
int CprtJobFunction::GetDbDataPageAllSub(int index, int nFormSeq, CDatabase* pDB, int nNameKeiBitArray, int FormNo2)
{
// midori 160608 add -->
	int OutZero = 0;
	int HideZero = 0;
// midori 160608 add <--

	CdbUcInfSub mfcSub(pDB);

	mfcSub.RequeryFormSeq(nFormSeq);

	// 個別情報にセット
	int tujo = 0;
	if(FormNo2 == 1)		tujo = 0;
	else if(FormNo2 == 2)	tujo = 1;
	else if(FormNo2	== 3)	tujo = 2;
	else					tujo = 0;

// midori 160608 cor -->
//	int OutZero = GetOutZero(mfcSub);
//	int HideZero = GetHideZero(mfcSub);
//
//	m_PrtInfMainList[index].Option.OutZeroNull[tujo] = OutZero;
//	m_PrtInfMainList[index].Option.HideZero[tujo] = HideZero;
// ---------------------
	HideZero = GetHideZero(mfcSub);

	// 帳票イメージのみ出力するが有効の場合
	//「金額０円データを出力しない」「金額空欄データを出力しない」のフラグを０にする
	if(m_nOutZero == 1)	{
		OutZero = 0;
	}
	else	{
		OutZero = GetOutZero(mfcSub);
	}

	m_PrtInfMainList[index].Option.OutZeroNull[tujo] = GetOutZero(mfcSub);
	m_PrtInfMainList[index].Option.HideZero[tujo] = HideZero;
	if(mfcSub.m_GeneralVar[1] & BIT_D3)	{
		m_PrtInfMainList[index].Option.KamokuSitei[tujo] = 1;
	}
	else	{
		m_PrtInfMainList[index].Option.KamokuSitei[tujo] = 0;
	}
// midori 160608 cor <--
// midori 154119 add -->
	if(mfcSub.m_GeneralVar[0] & BIT_D6)	m_PrtInfMainList[index].Option.NameKei[tujo] = 1;
	else								m_PrtInfMainList[index].Option.NameKei[tujo] = 0;
// midori 154119 add <--

// midori 190505 del -->
//	int numPage	= GetDbDataPageAll( nFormSeq, pDB, OutZero, nNameKeiBitArray);
// midori 190505 del <--
// midori 190505 add -->
	int numPage	= GetDbDataPageAll( nFormSeq, pDB, OutZero, nNameKeiBitArray, 0, -1);
// midori 190505 add <--

	mfcSub.Fin();
	return numPage;
}

////////////////////////////////////////////////////////////////////////////////
// ■F4プレビュー画面からのF5かどうかをチェックする
//   EndPrint()の後でコールすること
//
// 引数		なし
//
// 戻り値	BOOL			FALSE:通常入力からのF5から　TRUE:F4プレビューのF5押下から
//
////////////////////////////////////////////////////////////////////////////////
void CprtJobFunction::SetIsPrintFromPrev()
{
	m_isF5FromPrev = m_icsVprn.IsPrintFromPreview();
}

////////////////////////////////////////////////////////////////////////////////
// GetHjNo(long psSigCoprn, const PRT_INF_COMMON& inf_common)
//
// ■印刷ダイアログのチェック状態によって、顧問先法人番号かマスター法人番号を返す
//
// 引数		：long					印刷ダイアログのチェック状態変数
//
// 戻り値	：CString				顧問先法人番号orマスター法人番号
//
////////////////////////////////////////////////////////////////////////////////
CString CprtJobFunction::GetHjNo(long psSigCoprn, const PRT_INF_COMMON& inf_common)
{
	//int h_flg12 = psSigCoprn & BIT_D12;
	//int h_flg13 = psSigCoprn & BIT_D13;
	//int	h_flg = !(h_flg13) && h_flg12; //　法人番号出力有効h_flg12　+　出力をしないh_flg13　に注意
	//if( h_flg ){
	//	CICSMNSub8 mnsub8;
	//	if( psSigCoprn & BIT_D1)	return mnsub8.MakePrintHojinNo((LPSTR)(LPCSTR)inf_common.HjNo_corec, BIT_D0);	// 顧問先から法人番号を取得
	//	else						return mnsub8.MakePrintHojinNo((LPSTR)(LPCSTR)inf_common.HjNo_zvol, BIT_D0);	// マスターから法人番号を取得
	//}

	return _T("");
}

// 元号取得
// 元号が追加される度、ここで分岐していくこと（ホントは汎用関数が欲しい）
// 引数は和暦をセットしてください。
CString CprtJobFunction::GetGengo(long nDateWareki)
{
// midori 181101 del -->
//	return DC_GENGOU_HEISEI;
// midori 181101 del <--
// midori 181101 add -->
	CString cst = _T("");
// midori UC_0026 del -->
//	int		no = 0;
// midori UC_0026 del <--

	if(IsValidKaigen() == FALSE) {
		cst = DC_GENGOU_HEISEI;
		//m_icsVprn
	}
	else {
// midori UC_0026 del -->
		//no = ((CUCHIWAKEApp*)AfxGetApp())->m_APNO;
		//// 公益:0x50、公益Ⅲ:0x51、社会福祉法人:0x52、学校:0x58、宗教法人:0x59の場合は
		//if(no == 0x50 || no == 0x51 || no == 0x52 ||
		//	no == 0x58 || no == 0x59)	{
		//	db_vd_NoToStrGen(nDateWareki+GENGO_OFFSET,cst);
		//}
		//else {
		//	if(nDateWareki == 0)	{
		//		cst = _T("平成");
		//	}
		//	else {
		//		db_vd_NoToStrGen(nDateWareki,cst);
		//	}
		//}
// midori UC_0026 del <--
// midori UC_0026 add -->
		if(nDateWareki == 0)	{
			cst = _T("平成");
		}
		else {
			db_vd_NoToStrGen(nDateWareki,cst);
		}
// midori UC_0026 add <--
	}
	return(cst);
// midori 181101 add <--
}

// midori 190505 add -->
int CprtJobFunction::GetPhoho( int nFormSeq, int nPHoho, CBatchPrt* pBp, CDBNpSub* pZmSub )
{
	int		rv=0;
// midori 20/10/08u add -->
	int		nSortChk;
// midori 20/10/08u add <--

// midori 20/10/08_2 add -->
	// 確認ダイアログを表示しない場合、処理不要
	if(prnConfimationSw == FALSE) {
		return(0);
	}
// midori 20/10/08_2 add <--

	// 確認ダイアログから呼ばれたときは、まだｄｂに書き込んでいないため渡された引数を使用する
	if(nPHoho != -1)	{
		rv = nPHoho;
	}
	else	{
		// 確認ダイアログで選択する出力方法(改頁･空行を詰める・改頁･空行を保持する・第１ソート項目で改頁する)を取得する
		CdbUcInfSub		uis(pZmSub->m_database);	// uc_inf_subテーブルクラス

		if(uis.RequeryFormSeq(nFormSeq) == DB_ERR_OK)	{
			if(!(uis.IsEOF()))	{
				uis.MoveFirst();
			}
			// 共通処理メニューからの一括印刷から起動時
			if(pBp->IsBatchExec() == TRUE)	{
				COwntbData*		pod;	// owntbクラス
				int				val=0;

				pZmSub->OwnTblOpen();
				pod = new COwntbData(pZmSub,pBp);
				pod->LoadAllData();

				val = pod->m_uInfo.uBPrtCmn.nOPPrnHoho[uis.m_FormOrder - 1];
// midori 20/10/08u del -->
				//if(val == 0 || val == 1 || val == 2)	{
// midori 20/10/08u del <--
// midori 20/10/08u add -->
				if(val == 0 || val == 1)	{
// midori 20/10/08u add <--
					rv = val;

				}
				pZmSub->OwnTblClose();

				delete pod;
			}
			// 通常起動時
			else	{
				CCtrlSubGeneralVar	cs(uis);
// midori 20/10/08u del -->
				//if(cs.bPrnHoho1 == FALSE && cs.bPrnHoho2 == FALSE && cs.bPrnHoho3 == FALSE)	{
				//	// 新規起動時
				//	// ソートダイアログの「改頁を行う」チェックがONの時
				//	if(uis.m_OpKaiPage == 1)	rv = 2;
// midori 20/10/08u del <--
// midori 20/10/08u add -->
				if(cs.bPrnHoho1 == FALSE && cs.bPrnHoho2 == FALSE)	{
					// 新規起動時
					// ソートダイアログの「改頁を行う」チェックがONの時
					if(uis.m_OpKaiPage == 1)	rv = 1;
// midori 20/10/08u add <--
					else						rv = 0;
				}
				else	{
					if(cs.bPrnHoho1)		rv = 0;
					else if(cs.bPrnHoho2)	rv = 1;
// midori 20/10/08u del -->
					//else if(cs.bPrnHoho3)	rv = 2;
// midori 20/10/08u del <--
// midori 20/10/08_2 add -->
					nSortChk = ChkIkkatuSort(pZmSub->m_database, &uis);
					if(nSortChk == 0 && rv != 0) {
						rv = 0;
					}
// midori 20/10/08_2 add <--
				}
			}
			uis.Fin();
		}
	}
	return(rv);
}
// midori 190505 add <--
