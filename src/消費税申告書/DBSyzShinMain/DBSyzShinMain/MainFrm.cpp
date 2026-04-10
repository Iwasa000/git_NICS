// MainFrm.cpp : CMainFrame クラスの実装
//

#include "stdafx.h"
#include "DBSyzShinMain.h"

#include "MainFrm.h"
#include "DBSyzShinMainDoc.h"
#include "DBSyzShinMainView.h"

// 改良依頼No.18-0686 add -->
#include "DlgJobEnd.h"
#include "DlgJobEnd2.h"
// 改良依頼No.18-0686 add <--

//----> 納付書・相続人対応
#include "SozokuSel.h"
//<---- 納付書・相続人対応

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//== 定数 ==
const	int	ID_SH_LOCK_ID	=	1;
const	int	ID_SH_RIREKI_ID	=	2;
//#define lock_id 1
//#define rireki_id 2

// 24/02/19wd_特定収入対応 add -->
extern PRNEXSUB_INF									m_PrnEXSub;
extern CMap<int, int, PRNEXSUB_INF, PRNEXSUB_INF>	m_pMapPrnEXSub;
// 24/02/19wd_特定収入対応 add <--
//20240307
extern PRNEXSUB_INF									m_PrnEXSub_KAZ;
extern CMap<int, int, PRNEXSUB_INF, PRNEXSUB_INF>	m_pMapPrnEXSub_KAZ;

extern PRNEXSUB_INF									m_PrnEXSub_ZEI;
extern CMap<int, int, PRNEXSUB_INF, PRNEXSUB_INF>	m_pMapPrnEXSub_ZEI;
//20240307

// CMainFrame

//#define	DBGLOG_OUT 1
// ------------------------------------------------------------------------------------------
// 文字列に時刻を付加してログファイルに出力する（デバッグ用）
// ------------------------------------------------------------------------------------------
// [PARAM]
// CString			クラス関数名
// CString			ログ文字列
// ------------------------------------------------------------------------------------------
// [RETURN]
//					TRUE	=成功
//					FALSE	=失敗
// ------------------------------------------------------------------------------------------
// ★TODO：リリース時、下記 DBGLOG_OUT 定義を コメントにすること
//#define		DBGLOG_OUT
void DbgWriteLog( CString strFN, LPCSTR strLog)
{
#ifndef DBGLOG_OUT
	UNREFERENCED_PARAMETER ( strFN );
	UNREFERENCED_PARAMETER ( strLog );
#else
	const CString	LOG_FILE = "C:\\SyzMain.log";		

	SYSTEMTIME	uSTime;
	CString		strLogString,
				strTmp;		
	static		BOOL	fErrW = FALSE;		// 書き込みエラー発生 = TRUE
	static		BOOL	fOnce = FALSE;		// 初回=FALSE 

	// 書き込み失敗発生している場合は、終了
	if ( TRUE == fErrW ) { return; }

	strTmp = strLog;

	// 末尾の改行削除
	if ( strTmp.Right( 1 ) == "\n" ) {
		strTmp = strTmp.Left( strTmp.GetLength() -1 );
	}

	//	時刻を取得する
	::GetLocalTime(&uSTime);

	//	ログ文字列を作成する
	CString strTmp2;
	strTmp2.Format( "%02d:%02d:%02d.%03d   [%s]",	uSTime.wHour, 
													uSTime.wMinute, 
													uSTime.wSecond, 
													uSTime.wMilliseconds,
													strFN);

	strLogString.Format( _T( "%-64s  %s\n"), strTmp2, strTmp );

	// ログ削除
	if ( FALSE == fOnce ) {
		DeleteFile( LOG_FILE );
	}

	//	ログファイルに文字列を追加する
	FILE*	pLogFile;
	//	ログファイルを開く
	if( fopen_s( &pLogFile, LOG_FILE, "a") != 0){
		// ファイルopen失敗時は、以降継続しない
		fErrW = TRUE;
		return;
	}

	if ( FALSE == fOnce ) {
		fputs( "LOG START =====================================================================================================\n", pLogFile);
		fOnce = TRUE;
	}

	//	ログを書き込む
	fputs( strLogString, pLogFile);
	
	//	ログファイルを閉じる
	if( fclose( pLogFile) != 0){
		fErrW = TRUE;
		return;
	}
#endif

	return;
}//DbgWriteLog


IMPLEMENT_DYNAMIC(CMainFrame, ICSMainFrame)

BEGIN_MESSAGE_MAP(CMainFrame, ICSMainFrame)
	ON_MESSAGE(WM_USER_SETFOCUS, OnUserSetFocus)
	ON_MESSAGE(WM_USER_UPDATE_INDICATE, OnUserUpdateIndicate)
	ON_MESSAGE(WM_USER_READ_ZEIRISHI, OnUserReadZeirishi)
	ON_MESSAGE(WM_USER_RECALQ, OnUserRecalq)
	ON_MESSAGE(WM_USER_DISP_SPECPASS, OnUserDispSpecpass)
	ON_MESSAGE(WM_USER_SPCPOST_RECALQ, OnUserSpcpostRecalq)	// '15.03.09
	ON_MESSAGE(WM_USER_SKJPOST_RECALQ, OnUserSkjpostRecalq)	// '15.04.03
	ON_MESSAGE(WM_USER_SET_ENDSTART, OnUserEndStart)		// '15.07.31
	ON_MESSAGE(WM_USER_RESET_TKKZ, OnUserResetTkkz)			// '15.09.09
	ON_MESSAGE(WM_USER_REFOCUS_PRINT, OnUserRefocusPrint)	// '16.01.07
	ON_MESSAGE(WM_USER_CHG_LOCK_DISP, OnUserChgLockDisp)	// '16.07.22
	ON_MESSAGE(WM_USER_RESET_REFUND, OnUserResetRefund)		// '19.09.09
	ON_MESSAGE(WM_ENDPROCESS, OnEndProcess)
	ON_WM_CREATE()
	ON_MESSAGE(ICS_MAINFRAME_MESSAGE_CALL,OnCall)
	ON_MESSAGE(ICS_MAINFRAME_MESSAGE_JOBSEL,OnFromForkChain)
	ON_MESSAGE(ICS_MAINFRAME_MESSAGE_COSEL,OnFromParentCosel)
	ON_MESSAGE(WM_CLOSE,OnMessageClose)
	ON_COMMAND(ID_TAXTION_LIST, &CMainFrame::OnTaxtionList)
	ON_COMMAND(ID_SHIN_PAYMENT, &CMainFrame::OnShinPayment)
	ON_COMMAND(ID_BUTTON_F0, &CMainFrame::OnButtonF0)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_F0, &CMainFrame::OnUpdateButtonF0)
	ON_COMMAND(ID_KOMONSAKI_INF, &CMainFrame::OnKomonsakiInf)
	ON_UPDATE_COMMAND_UI(ID_TAXTION_LIST, &CMainFrame::OnUpdateTaxtionList)
	ON_COMMAND(ID_HISTORY_REV, &CMainFrame::OnHistoryRev)
	ON_UPDATE_COMMAND_UI(ID_HISTORY_REV, &CMainFrame::OnUpdateHistoryRev)
	ON_UPDATE_COMMAND_UI(ID_SHIN_PAYMENT, &CMainFrame::OnUpdateShinPayment)
	ON_COMMAND(ID_ZENNEN_TAX, &CMainFrame::OnZennenTax)
	ON_UPDATE_COMMAND_UI(ID_ZENNEN_TAX, &CMainFrame::OnUpdateZennenTax)
// 24/03/18_税務代理書面 add -->
	ON_COMMAND(ID_SHIN_ZEIMUD, &CMainFrame::OnShinZeimud)
	ON_UPDATE_COMMAND_UI(ID_SHIN_ZEIMUD, &CMainFrame::OnUpdateShinZeimud)
// 24/03/18_税務代理書面 add <--
	ON_WM_ACTIVATE()
	ON_WM_MOVE()
//2016.06.22 INSERT START
	ON_COMMAND(ID_KOKETSU_MEISYO, &CMainFrame::OnKoketsuMeisyo)
	ON_UPDATE_COMMAND_UI(ID_KOKETSU_MEISYO, &CMainFrame::OnUpdateKoketsuMeisyo)
//2016.06.22 INSERT END
	ON_COMMAND( ID_KOKETSU_KINGAKU, &CMainFrame::OnKoketsuKingaku )
	ON_UPDATE_COMMAND_UI( ID_KOKETSU_KINGAKU, &CMainFrame::OnUpdateKoketsuKingaku )


//ON_UPDATE_COMMAND_UI(ID_KANI_KOUJOSELECT, &CMainFrame::OnUpdateKoujoSelect)
//ON_COMMAND(ID_KANI_KOUJOSELECT, &CMainFrame::OnKoujoSelect)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ステータス ライン インジケータ
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame コンストラクション/デストラクション

CMainFrame::CMainFrame()
	: m_pMainView(NULL)
	, m_pZmSub(NULL)
	, m_sgDotPrn(0)
	, m_isNonCorp(FALSE)
	, m_sgIkat(0x00)
	, m_sgBasic(0x00)
	, m_sgSpc(0)
	, m_Specnt(0)
	, m_SplistCnt(0)
	, m_isKndMster(FALSE)
	, m_pCoinfCtl(NULL)
	, m_isNewMake(FALSE)
	, m_isOpenEselect(FALSE)
	, m_isAfterH24Reform(FALSE)
	, m_isExistOffice(FALSE)
	, m_isAbleProgress(FALSE)
	, m_isPrintEtaxFinish(FALSE)
	, m_swFork(0)
	, m_swBeForked(0)
	, m_SnSeq(-1)
	, m_pSzzanList(NULL)
	, m_pSyzSyukei(NULL)
	, m_nowTabIdx(0)
	, m_isEndStart(FALSE)
	, m_Over500MillionSw(0)
	, m_Progress(0)
	, m_FinishSw(0)
	, m_Finish(0)
	, m_old_Sn_SKKBN(0)
	, m_old_Sn_KZTNSK(0)
	, m_old_Sn_MCOUNT(0)
	, m_swForkErr(0)
	, m_isCompChange(FALSE)
	, m_MainInstance(NULL)
	, m_isInitEnd(FALSE)
	, m_isCompReInit(FALSE)
	, m_isKeyoff(FALSE)
	, m_svSVmzsw(-1)
	, m_svRealSvmzsw(0)
	, m_symd(0)					// 150107
	, m_isShownHonpyo(FALSE)	// '15.02.03
	, m_nowTabmax(0)			// '15.03.18
	, m_eymd(0)					// 特定課税仕入対応150713
	, m_isNeedChkTkkz(TRUE)		// '15.11.17
	, m_isHirendoCall(false)	// '15.12.23
	, m_rtIsRegistMcode(-999)	// '18.12.17
	, m_bAlterIndex(false)		// '18.12.17
	, m_JcContinuePrint(-1)
	, m_nowTabver(0)			// '19.07.17
	, m_ActViewVer(-1)
	, m_ActedViewFlg(0)
{
#ifdef _DEBUG
	pMon = new ICSMon(GetCurrentProcessId());
	PCOM_SetInt( _T("CompanySin" ),  0 ); 
	SET_MonitorVersion( 1 );
#endif //_DEBUG
	SetCommandBarsID( IDR_DBSYZSHINMAINTYPE );
	// TODO: メンバ初期化コードをここに追加してください。

	memset( &m_SkjCnvtbl, '\0', sizeof(_TB_PAR) );
	m_SkjCnvtbl.tp_tno = -1;
	m_Arith.l_defn( 0x16 );
	memset( &m_SvPayment, '\0', sizeof(m_SvPayment) );
	memset( &m_Month26, '\0', sizeof(m_Month26) );
	memset( &m_SYsw, '\0', sizeof(m_SYsw) );
	memset( &m_dtl, '\0', sizeof(m_dtl) );
	memset( &m_ShinInfo, '\0', sizeof(SYC_SHININFO) );
	memset( &m_Eselect, '\0', sizeof(ESELECT) );
	memset( &m_O_HEAD, '\0', sizeof(OFC_HEAD) );
	memset( &m_O_DATA, '\0', sizeof(OFC_DATA) );
	memset( &m_Z_DATA, '\0', sizeof(ZEI_DATA) );
	memset( &m_C_DATA, '\0', sizeof(COP_DATA) );
	memset( &m_Progdata, '\0', sizeof(struct ProgDataEx) );
	memset( m_FinishPrn, '\0', sizeof(m_FinishPrn) );
	memset( &NowCoInf, '\0', sizeof(NowCoInf));//
	memset( &m_ZeiriInfo, '\0', sizeof(m_ZeiriInfo) );	// '15.02.25
//--> '16.01.30 INS START
	memset( &m_TgPDat, '\0', sizeof(TgPackData) );
	m_TgPDat.TgCode = -1;
//<-- '16.01.30 INS END

	m_isKoketsuKingaku = false;
//----> 20240116
	m_isScoreLinkCall = -1;
	m_ScoreLinkPdfNam.Empty();
//----> 20240116
}

CMainFrame::~CMainFrame()
{
#ifdef _DEBUG
	delete pMon;
#endif //_DEBUG
}

#include <l_dotnet.h>

int CMainFrame::BeginApp()
{
#ifdef	_DEBUG
	PCOM_SetStringAll( _T("AppsNumBer"), "00,59" );
#endif
	// フォーク情報の取得
	GetForkInfo();

	// 会社選択
#ifdef	_DEBUG
	// 非連動ｄｂ会社確認
	COPAK	cop;
	cop.pksw = 0x1;
	cop.sgn4 = 0x20;
	if( m_cosel.co_ok(cop) != IDOK ){
#else
	if( m_cosel.co_ok() != IDOK ){
#endif
		ICSExit( 0, _T("処理を終了します。") );
		return 0;
	}

	// アプリケーションの初期化
	BeginWaitCursor();
	if( InitApl(TRUE) ){
		EndWaitCursor();
//		ICSExit( 0, _T("初期化失敗！！") );
		return 0;
	}
	EndWaitCursor();

	m_MainInstance = AfxGetResourceHandle();

	// 基本情報
	if( ICSMainFrame::BeginApp() ){
		ErrorExit( _T("基本情報の設定に失敗しました") );
		return 0;
	}

/*	char	buf1[10]={0}, buf2[10]={0}, buf3[10]={0};
	char	tmpbuf[10]={0};
	CArith	ar(0x16);
	ar.l_input( buf1, _T("6300") );
	ar.l_input( buf2, _T("17") );
	ar.l_input( buf3, _T("63") );

	l_pardotnet( tmpbuf, buf1, buf2, buf3 );

	char	moji[64]={0};
	ar.l_print( moji, tmpbuf, _T("sss,sss,sss,sss") );*/


	// 初期化
	CDBSyzShinMainView	*pView = (CDBSyzShinMainView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(BSINFO_VIEW_WND_IDX));
	if( pView != NULL ){
		m_pMainView = pView;

		memset( &m_ShinInfo, '\0', sizeof(SYC_SHININFO) );
		m_ShinInfo.pBaseView = m_pMainView;
		m_ShinInfo.pParent = this;
		m_ShinInfo.pEselect = &m_Eselect;
		m_ShinInfo.pIsOpenEselect = &m_isOpenEselect;
//-- '15.04.04 --
//		m_ShinInfo.sgSpc = m_sgSpc;
//---------------
		m_ShinInfo.pSgSpc = &m_sgSpc;
//---------------
		m_ShinInfo.isExistOffice = m_isExistOffice;
		m_ShinInfo.pO_DATA = &m_O_DATA;
		m_ShinInfo.pZ_DATA = &m_Z_DATA;
		m_ShinInfo.pOver500MillionSw = &m_Over500MillionSw;
		m_ShinInfo.pToolBar = GetToolBar( 0 );
		m_ShinInfo.pProgress = &m_Progress;
		m_ShinInfo.pFinish = &m_Finish;
		m_ShinInfo.pFinishSw = &m_FinishSw;
		m_ShinInfo.pFinishPrn = m_FinishPrn;
		m_ShinInfo.isDotPrn = m_sgDotPrn;
		if( pView->Init(m_pZmSub, this, &m_SnHeadData, &m_ShinInfo, &m_Tblhdl, &m_SkjCnvtbl) ){
			ErrorExit( _T("基本情報画面の作成に失敗しました") );
			return 0;
		}
//--> '15.03.18 INS START
		m_nowTabmax++;
//<-- '15.03.18 INS END
	}
	else{
		m_pMainView = NULL;
	}

	//------>150107
//-- '15.05.01 --
//	if( m_symd >= ICS_SH_SIXKIND_REFORM_DAY ){
//		m_pMainView->m_SixKindFlg = TRUE;
//	}
//	else{
//		m_pMainView->m_SixKindFlg = FALSE;
//	}
//---------------
	if( m_pMainView ){
		m_pMainView->m_SixKindFlg = IsSixKindKani( m_symd );
		//---->特定課税仕入対応150713
		m_pMainView->SetTkkzSign( m_eymd );
		//<-----------------------------
	}
//---------------
	//<------------------

	// 事務所情報を読込み
//-- '16.08.08 --
//	if( ReadOfficeInfo(TRUE) == 1){
//		m_ShinInfo.isExistOffice = TRUE;
//	}
//	else{
//		m_ShinInfo.isExistOffice = FALSE;
//	}
//---------------
	if( IsCompanyPackageInSyz() ){
		m_ShinInfo.isExistOffice = FALSE;
	}
	else{
		if( ReadOfficeInfo(TRUE) == 1){
			m_ShinInfo.isExistOffice = TRUE;
		}
		else{
			m_ShinInfo.isExistOffice = FALSE;
		}
	}
//---------------

	// 本表・付表
#ifndef SHOWTABTEST
	AfxSetResourceHandle(GetH31HyoInstance());
	if (((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->Open(1)) {
		ErrorExit(_T("本表・付表(31)画面の作成に失敗しました"));
		return 0;
	}
	m_nowTabmax++;
	if (GetActViewVer() == ID_VER_SYZ31) {
		SetOldData();

		// 他のビューの初期化
		if (InitOtherView(3, m_pZmSub)) {
			return 0;
		}
//		m_nowTabver = ID_VER_SYZ31;
		SetKaiseiVer(ID_VER_SYZ31);
	}
	AfxSetResourceHandle(GetH26HyoInstance());
	if (((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->Open(2)) {
		ErrorExit(_T("本表・付表画面の作成に失敗しました"));
		return 0;
	}
	//--> '15.03.18 INS START
	m_nowTabmax++;
	//<-- '15.03.18 INS END
	if (GetActViewVer() == ID_VER_SYZ26) {
		SetOldData();

		// 他のビューの初期化
		if (InitOtherView(0, m_pZmSub)) {
			return 0;
		}
//		m_nowTabver = ID_VER_SYZ26;
		SetKaiseiVer(ID_VER_SYZ26);
	}
#else
	if (m_SnHeadData.Kaisei_VER == ID_VER_SYZ31) {
		AfxSetResourceHandle(GetH31HyoInstance());
		if (((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->Open(1)) {
			ErrorExit(_T("本表・付表(31)画面の作成に失敗しました"));
			return 0;
		}
		m_nowTabmax++;

		// 他のビューの初期化
		if (InitOtherView(3, m_pZmSub)) {
			return 0;
		}
	}
	else {
		AfxSetResourceHandle(GetH26HyoInstance());
		if (((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->Open(2)) {
			ErrorExit(_T("本表・付表画面の作成に失敗しました"));
			return 0;
		}
		//--> '15.03.18 INS START
		m_nowTabmax++;
		//<-- '15.03.18 INS END

		SetOldData();

		// 他のビューの初期化
		if (InitOtherView(0, m_pZmSub)) {
			return 0;
		}
	}
#endif
	AfxSetResourceHandle( GetTaxationListInstance() );
	if(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->Open(3)){
		ErrorExit( _T("課税取引計算書画面の作成に失敗しました") );
		return 0;
	}
	AfxSetResourceHandle( GetTaxationList31Instance() );
	if(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->Open(4)){
		ErrorExit( _T("課税取引計算書画面の作成に失敗しました") );
		return 0;
	}
	//課税取引金額計算表
	if (m_Util.IsBasicMst(m_pZmSub->zvol)) {
		if (GetActViewVer() == ID_VER_SYZ26) {
			// 他のビューの初期化
			m_nowTabmax++;
			if (InitOtherView(1, m_pZmSub)) {
				return 0;
			}
		}
		if (GetActViewVer() == ID_VER_SYZ31) {
			m_nowTabmax++;
			if (InitOtherView(4, m_pZmSub)) {
				return 0;
			}
		}
	}
	// 特定収入計算表
	else if( m_Util.IsSpcMaster(m_pZmSub) ){	
		// タブ表示
		//AfxSetResourceHandle( GetTaxationListInstance() );
		//if(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->Open(3)){
		//	ErrorExit( _T("特定収入計算表画面の作成に失敗しました") );
		//	return 0;
		//}
//--> '15.03.18 INS START
		m_nowTabmax++;
//<-- '15.03.18 INS END

		// 他のビューの初期化
		//if( InitOtherView(2, m_pZmSub) ){
		//	return 0;
		//}
//<-- '15.02.06 INS END

		if ( GetActViewVer() == ID_VER_SYZ26 ) {
			// 他のビューの初期化
			m_nowTabmax++;
			if (InitOtherView ( 2, m_pZmSub ) ) {
				return 0;
			}
		}
		if ( GetActViewVer() == ID_VER_SYZ31 ) {
			m_nowTabmax++;
			if ( InitOtherView ( 5, m_pZmSub ) ) {
				return 0;
			}
		}
	}

	// 元に戻す
	AfxSetResourceHandle( m_MainInstance );

	SetTabTitle();

	// 進捗可能チェック
	InitProgressInfo();

//-- '18.12.17 --
// 上位関数に吸収
//	// 過年度マスターチェック[S]
//	m_KndFlg = SetKanendoData();
//---------------

	// 履歴情報の初期化
//-- '14.09.10 --
//	InitHistoryReport();
//---------------
	if( InitHistoryReport() ){
		return 0;
	}
//---------------

	if( pView ){
		pView->SaveTabMakeInfo();
	}

	m_isInitEnd = TRUE;

	// 2024/03/14
	// 課税取引金額計算表、特定収入計算表、税務代理書面の設定値を取得する
	pView->GetPrnExSubInfo();

	if( m_swBeForked ){
	//yoshida190824
	//ここで一括印刷の１０％改正年度の課税期間の場合に処理止める
#ifdef _20190830_SYUKA_ 
		if (pView->IsSpc31OutKikan(pView->curset)) {
			ErrorExit(_T("令和１年10月改正の消費税申告書は、現在未対応です。\n対応までしばらくお待ちください。"));
			return 0;
		}
#endif

		//--->yoshida190905
		// 特定収入計算表未対応。期間を見て対象期間の場合処理制御
//		if (m_Util.IsSpcMaster(m_pZmSub)) {
//			if (pView->IsSpc31OutKikan(pView->curset)) {
//				ErrorExit(_T("令和１年10月改正の特定収入計算表申告書は、現在未対応です。\n対応までしばらくお待ちください。"));
//				return 0;
//			}
//		}
		//<------


		if( pView->ForkJob()){
			return 0;
		}
	}

	PostMessage( WM_USER_SETFOCUS, 0 );	// 「入力」へ harino ('14.03.09)

	return 0;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
BOOL CMainFrame::UpdateLockIndicator( int flg )
{
	CXTPStatusBarPane* pane = m_wndStatusBar.FindPane( ID_SH_LOCK_ID );
	if( flg != pane->IsVisible() ){
		pane->SetVisible(flg);
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
BOOL CMainFrame::UpdateRirekiIndicator( int flg )
{
	CXTPStatusBarPane* pane = m_wndStatusBar.FindPane( ID_SH_RIREKI_ID );
	if( flg != pane->IsVisible() ){
		pane->SetVisible( flg );
	}

	return 0;
}

//-----------------------------------------------------------------------------
// タブタイトルのセット
//-----------------------------------------------------------------------------
// 引数	change	：	1:切り替え時
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CMainFrame::SetTabTitle( int change )
{
#ifndef SHOWTABTEST
	if (!change) {
		SetTabLabel(0, CString("基本情報"));
		//if (m_SnHeadData.Kaisei_VER == ID_VER_SYZ31)
		SetTabLabel(1, CString("本表・付表"));
		//else
			//SetTabLabel( 1, CString("本表・付表") );
		SetTabLabel(2, CString("本表・付表"));
		SetTabLabel(3, CString("課税取引金額計算表"));
		SetTabLabel(4, CString("課税取引金額計算表"));
	}
	/*else {
		SetTabLabel( 0, CString("基本情報") );
		if (m_SnHeadData.Kaisei_VER == ID_VER_SYZ31)
			SetTabLabel( 1, CString("本表・付表31") );
		else
			SetTabLabel( 1, CString("本表・付表") );
	}*/
#else
	SetTabLabel( 0, CString("基本情報") );
	/*if (m_SnHeadData.Kaisei_VER == ID_VER_SYZ31)
		SetTabLabel( 1, CString("本表・付表31") );
	else
		SetTabLabel( 1, CString("本表・付表") );*/
#endif
//-- '15.02.06 --
//	if( m_Util.IsBasicMst(m_pZmSub->zvol) ){
//		SetTabLabel( 2, CString("課税取引金額計算表") );
//		ShowTabOrgIndex( 2, TRUE, FALSE );//法人個人切替で課税取引のタブがきりかわらなかったのを修正
//	}
//	else{
//		ShowTabOrgIndex( 2, FALSE, FALSE );//法人個人切替で課税取引のタブがきりかわらなかったのを修正
//	}
//---------------
	//ShowTabOrgIndex( 1, FALSE, FALSE );
#ifndef SHOWTABTEST
	if (GetActViewVer() == ID_VER_SYZ31) {
		ShowTabOrgIndex(1, TRUE, FALSE);
		ShowTabOrgIndex(2, FALSE, FALSE);
		if(!change) m_nowTabmax--;	// 起動時は両方のタブがカウントされているため
	}
	else if (GetActViewVer() == ID_VER_SYZ26) {
		ShowTabOrgIndex(1, FALSE, FALSE);
		ShowTabOrgIndex(2, TRUE, FALSE);
		if(!change) m_nowTabmax--;	// 起動時は両方のタブがカウントされているため
	}
#endif

#ifdef AAA//　調査中
	if( m_Util.IsBasicMst(m_pZmSub->zvol) ){
		SetTabLabel( 2, CString("課税取引金額計算表") );
		HINSTANCE hInst = AfxGetResourceHandle();
		if (m_SnHeadData.Kaisei_VER == ID_VER_SYZ31) {
			AfxSetResourceHandle( GetTaxationList31Instance() );
			ShowTabOrgIndex( 3, TRUE, FALSE );
		}
		else{
			AfxSetResourceHandle( GetTaxationListInstance() );
			ShowTabOrgIndex( 3, TRUE, FALSE );
		}
		AfxSetResourceHandle( hInst );
	}
	else if( m_Util.IsSpcMaster(m_pZmSub) ){
		SetTabLabel( 3, CString("特定収入計算表") );
		HINSTANCE hInst = AfxGetResourceHandle();
		AfxSetResourceHandle( GetTaxationListInstance() );
		ShowTabOrgIndex( 3, TRUE, FALSE );
		AfxSetResourceHandle( hInst );
	}
#else
	if( m_Util.IsBasicMst(m_pZmSub->zvol) ){
		SetTabLabel( 2, CString("課税取引金額計算表") );
		SetTabLabel( 3, CString("課税取引金額計算表") );
		HINSTANCE hInst = AfxGetResourceHandle();
		if (GetActViewVer() == ID_VER_SYZ31) {
			AfxSetResourceHandle( GetTaxationListInstance() );
			ShowTabOrgIndex( 3, FALSE, FALSE );
			AfxSetResourceHandle( GetTaxationList31Instance() );
			ShowTabOrgIndex( 4, TRUE, FALSE );		}
		else{
			AfxSetResourceHandle( GetTaxationListInstance() );
			ShowTabOrgIndex( 3, TRUE, FALSE );
			AfxSetResourceHandle( GetTaxationList31Instance() );
			ShowTabOrgIndex( 4, FALSE, FALSE );
		}
//		if(!change) m_nowTabmax--;	// 起動時は両方のタブがカウントされているため //yoshida190725
		AfxSetResourceHandle( hInst );
	}
	else if( m_Util.IsSpcMaster(m_pZmSub) ){
		SetTabLabel( 2, CString("特定収入計算表") );
		SetTabLabel( 3, CString("特定収入計算表") );
		HINSTANCE hInst = AfxGetResourceHandle();
		if (GetActViewVer() == ID_VER_SYZ31) {
			AfxSetResourceHandle( GetTaxationListInstance() );
			ShowTabOrgIndex( 3, FALSE, FALSE );
			AfxSetResourceHandle( GetTaxationList31Instance() );
			ShowTabOrgIndex( 4, TRUE, FALSE );
#ifdef _20190930_SYUKA
			ShowTabOrgIndex( 4, FALSE, FALSE );
			*m_ShinInfo.pSgSpc = 0;
#endif
			// 2割特例を適用する場合は簡易と同じ扱いになるため、特定収入タブを非表示にする
			if( m_SnHeadData.m_DispTabSgn & 0x2000 ){
				ShowTabOrgIndex( 4, FALSE, FALSE );
			}
			else{
				ShowTabOrgIndex( 4, TRUE, FALSE );
			}
		}
		else{
			AfxSetResourceHandle( GetTaxationListInstance() );
			ShowTabOrgIndex( 3, TRUE, FALSE );
			AfxSetResourceHandle( GetTaxationList31Instance() );
			ShowTabOrgIndex( 4, FALSE, FALSE );
		}

		AfxSetResourceHandle(hInst);//yoshida190729

//		if(!change) m_nowTabmax--;	// 起動時は両方のタブがカウントされているため //yoshida190725
	}
#endif
	else{
//		ToggleMDITabs();
//		ShowTabOrgIndex( 2, FALSE, FALSE );
//		ToggleMDITabs();
		HINSTANCE hInst = AfxGetResourceHandle();
		AfxSetResourceHandle( GetTaxationListInstance() );
		ShowTabOrgIndex( 3, FALSE, FALSE );
		AfxSetResourceHandle( GetTaxationList31Instance() );
		ShowTabOrgIndex( 4, FALSE, FALSE );
		AfxSetResourceHandle( hInst );
		m_nowTabmax--;
	}

/*		ToggleMDITabs();
		HideMDITabs();
		ShowTabOrgIndex( 0, TRUE, FALSE );
		ShowTabOrgIndex( 1, TRUE, FALSE );
		ToggleMDITabs();*/
//---------------

	return 0;
}

//-----------------------------------------------------------------------------
// 各付表の初期化
//-----------------------------------------------------------------------------
// 引数	type	：	対象タブの種類（0:本表・付表, 1:課税取引計算書, 2:特定収入計算表）
//		pZmSub	：	公益クラスのポインタ
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::InitOtherView( int type, CDBNpSub *pZmSub )
{
#ifdef _DBG_VIEW_START_
	CString	hoge;
	hoge.Format("□□□InitOtherView_START:type:%d\n", type);
	DbgViewTrace(hoge);
#endif

	if( type == 0 ){
		// 本表・付表ビュー
		CH26HyoView	*pH26View = (CH26HyoView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(H26_HONPYO_WND_IDX));
		if( pH26View != NULL ){
			m_ShinInfo.pToolBar = GetToolBar( 1 );
			pH26View->SetSnSeq( m_SnSeq );

			//-------->特定課税仕入対応150713
			pH26View->SetShRevType( m_pMainView->m_ShRevType );
			//<----------------------------------

			if( pH26View->InitView(pZmSub, m_pSyzSyukei, m_pSyzSubSyukei, &m_ShinInfo, &m_SnHeadData, &m_Tblhdl, &m_SkjCnvtbl , m_pMainView->m_SixKindFlg) ){//150107
//			if( pH26View->InitView(pZmSub, m_pSyzSyukei, m_pSyzSubSyukei, &m_ShinInfo, &m_SnHeadData, &m_Tblhdl, &m_SkjCnvtbl) ){
				ICSMessageBox( pH26View->GetErrMsg(), MB_ICONSTOP );
				PostMessage( WM_CLOSE, 0x2000 );
				return -1;
			}
		}
	}
//-- '15.02.06 --
//	else if( type == 1 ){
//		// 課税取引計算書
//		CTaxationListView *pTaxationListView = (CTaxationListView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd( 3 ));
//		if( pTaxationListView != NULL ){
//			m_ShinInfo.pToolBar = GetToolBar( 2 );
//			pTaxationListView->SetSnSeq( m_SnSeq );
//			if( pTaxationListView->InitView(pZmSub, &m_ShinInfo, &m_SnHeadData, m_pSyzSyukei) ){
////			if( pTaxationListView->InitView(pZmSub, &m_ShinInfo, &m_SnHeadData, m_SnSeq) ){
//				ICSMessageBox( pTaxationListView->GetErrMsg(), MB_ICONSTOP );
//				PostMessage( WM_CLOSE, 0x2000 );
//				return -1;
//			}
//		}
//	}
//---------------
	else if( (type==1) || (type==2) ){
		// 課税取引計算書
		CTaxationListView *pTaxationListView = (CTaxationListView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(H26_KZTORIHIKI_WND_IDX));
		if( pTaxationListView != NULL ){
			m_ShinInfo.pToolBar = GetToolBar( 2 );
			pTaxationListView->SetSnSeq( m_SnSeq );
//2016.10.20 UPDATE START
//			if( pTaxationListView->InitView(pZmSub, &m_ShinInfo, &m_SnHeadData, m_pSyzSyukei) ){
			if( pTaxationListView->InitView(pZmSub, &m_ShinInfo, &m_SnHeadData, m_pSyzSyukei , m_pMainView->m_SixKindFlg) ){
//2016.10.20 UPDATE END
				ICSMessageBox( pTaxationListView->GetErrMsg(), MB_ICONSTOP );
				PostMessage( WM_CLOSE, 0x2000 );
				return -1;
			}
		}
	}
//---------------
	else if( type == 3 ){
		// 本表・付表ビュー31
		CH31HyoView	*pH31View = (CH31HyoView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(H31_HONPYO_WND_IDX));
		if( pH31View != NULL ){
			m_ShinInfo.pToolBar = GetToolBar( 2 );
			pH31View->SetSnSeq( m_SnSeq );

			// 改正様式をセット
			pH31View->SetShRevType ( m_pMainView->m_ShRevType );

			//---->yoshida190627
			
			if (pH31View->InitView(pZmSub, m_pSyzSyukei, &m_SnHeadData, &m_ShinInfo, &m_Tblhdl, &m_SkjCnvtbl)) {
				PostMessage(WM_CLOSE, 0x2000);
				return -1;
			}
			//-----
//			if( pH31View->InitView(pZmSub , m_pSyzSyukei, &m_SnHeadData , &m_ShinInfo) ){
//				//ICSMessageBox( pH26View->GetErrMsg(), MB_ICONSTOP );
//				PostMessage( WM_CLOSE, 0x2000 );
//				return -1;
//			}
			//<---------
		}
	}
	//else if( type == 4 ){
	else if ( ( type == 4 ) || ( type == 5 ) ) {
		// 課税取引計算書31
		CTaxationList31View *pTaxationList31View = (CTaxationList31View *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(H31_KZTORIHIKI_WND_IDX));
		if( pTaxationList31View != NULL ){
			m_ShinInfo.pToolBar = GetToolBar( 2 );
			pTaxationList31View->SetSnSeq( m_SnSeq );
//2016.10.20 UPDATE START
//			if( pTaxationListView->InitView(pZmSub, &m_ShinInfo, &m_SnHeadData, m_pSyzSyukei) ){
			if( pTaxationList31View->InitView(pZmSub, &m_ShinInfo, &m_SnHeadData, m_pSyzSyukei /*, m_pMainView->m_SixKindFlg*/) ){
//2016.10.20 UPDATE END
				ICSMessageBox( pTaxationList31View->GetErrMsg(), MB_ICONSTOP );
				PostMessage( WM_CLOSE, 0x2000 );
				return -1;
			}
		}
	}

#ifdef _DBG_VIEW_START_
	hoge.Format("□□□InitOtherView_END:type:%d\n", type);
	DbgViewTrace(hoge);
#endif

	return 0;
}

//-----------------------------------------------------------------------------
// 各付表の終了処理
//-----------------------------------------------------------------------------
// 引数	wflg	：	1：マスターとの接続を破棄する時に呼ばれている
//					0：マスターとの接続を破棄する時以外に呼ばれている
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::EndOtherView( int wflg/*=1*/ )
{

	//----->'19.10.24
	//電子申告で予定26号「納付すべき地方消費税額」が申告書の金額とずれがでていた。「Sn_TEDNOF」の金額が３１と２６共に書き込まれていたのが原因。順番を変更
	//場所移動
	// 本表・付表ビュー
	/*
	CBaseHyoView *pBaseView = GetHonHyoView();
	if( pBaseView != NULL ){
//--> '15.01.24 INS START
		if( m_nowTabIdx > 0 ){
			// 計算元になる帳票で終了した時に、関連データの更新が行われなかった為
			pBaseView->UpdateAllSheet();
		}
//--> '15.01.24 INS END
		pBaseView->EndJob(wflg);

		// 初期化完了フラグを落とす
		// 会社切り替えを行ったときに初期化が行われないことがあったため
		pBaseView->SetInitedFlg ( false );
	}
	*/
	//<----------------

	// 他方の本表・付表ビューの終了処理
	if( wflg ){
		if( (m_ActedViewFlg&0x03) == 0x03 ){
			CBaseHyoView *pOtherBaseView = NULL;

			// m_ActViewVerではなくKaisei_VERにより取得するビューを判断
			//int act = GetActViewVer();
			int act = GetKaiseiVer();
			if( act == ID_VER_SYZ26 ){
				pOtherBaseView = GetHonHyoView(ID_VER_SYZ31);
				m_ActedViewFlg &= ~0x02;
			}
			else if( act == ID_VER_SYZ31 ){
				pOtherBaseView = GetHonHyoView(ID_VER_SYZ26);
				m_ActedViewFlg &= ~0x01;
			}
			if( pOtherBaseView ){
				// フォーカス移動
				pOtherBaseView->EndJobFocus();

				pOtherBaseView->EndJob(wflg);

				// 初期化完了フラグを落とす
				// 会社切り替えを行ったときに初期化が行われないことがあったため
				pOtherBaseView->SetInitedFlg ( false );
			}
		}
	}

	//----->'19.10.24
	//電子申告で予定26号「納付すべき地方消費税額」が申告書の金額とずれがでていた。「Sn_TEDNOF」の金額が３１と２６共に書き込まれていたのが原因。順番を変更
	//場所移動後
	// 本表・付表ビュー
	CBaseHyoView *pBaseView = GetHonHyoView();
	if( pBaseView != NULL ){
//--> '15.01.24 INS START
		if( m_nowTabIdx > 0 ){
			// 計算元になる帳票で終了した時に、関連データの更新が行われなかった為
			pBaseView->UpdateAllSheet();
		}
//--> '15.01.24 INS END

		//-->'24.03.07 ADD h.wada
		//修正依頼168613で税額情報/当課税期間の課税売上高の金額をcursetの2割特例フラグを見て計算するように対応したが
		//税額情報ダイアログで登録ボタンを押すとDBに金額が書き込まれてしまう
		//本表・付表タブに遷移せずに処理終了する場合にHeadDataのフラグで再集計を行い、金額を書き戻す
		
		//2割特例チェックの状態は履歴選択でも破棄されるので上記の現象が発生する可能性はある
		//2割特例を変更して履歴表示に遷移できるのは、修正確定/更新の請求→確定(履歴)のみ
		//修正確定/更新の請求で2割特例の適用を切り替えるのは通常行わないので、この場合については対応していない
		
		if (m_pMainView->IsInvoiceMaster()) {
			//原則課税のとき、2割特例を適用するかで集計額が変わる
			if (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) {
				CSnset	curset = m_pMainView->GetSnset();
				//2割特例の適用状態に差異がある	
				if ((curset.m_DispTabSgn & 0x2000) ^ (m_SnHeadData.m_DispTabSgn & 0x2000)) {
					//void CDBSyzShinMainView::GetKazeiUriagedaka( char *pArith )の仕様により、
					//下記の条件では税額情報画面で当課税期間の課税売上高の集計を行わない
					bool skipCalq = (m_pMainView->InLockInput()) || (m_pMainView->IsInputKazeiUriage());
					if (!skipCalq) {
						long symd = 0, eymd = 0;
						char buf[MONY_BUF_SIZE] = { 0 }, money[MONY_BUF_SIZE] = { 0 };

						// 免税期間集計
						m_pMainView->GetSelectKazeiKikan(symd, eymd);
						((CH31HyoView*)pBaseView)->MenzeiKikanSyukei(symd, eymd, money);
						m_Arith.l_add(buf, buf, money);

						// 課税期間集計
						memset(money, '\0', MONY_BUF_SIZE);
						((CH31HyoView*)pBaseView)->KazeiUriageSyukei(money, m_SnHeadData.m_DispTabSgn & 0x2000);//明示的にHeadDataで判定する
						m_Arith.l_add(buf, buf, money);

						CString tag = m_pMainView->GetKazeiUriageCode();
						CString chtag = _T("");
						m_pMainView->SetMony(tag, chtag, buf, m_ActViewVer);
					}
				}
			}
// 24/06/13_税務代理書面 add -->
			CallWriteTaxAgentDB(m_pZmSub,&m_SnHeadData,this);
// 24/06/13_税務代理書面 add <--
		}
		//<<--'24.03.07 ADD h.wada

		pBaseView->EndJob(wflg);

		// 起動したサインが残っていることでビューの破棄が行われるときにビューが作成されていた
		int kVer = GetKaiseiVer();
		if ( kVer == ID_VER_SYZ31 ) {
			m_ActedViewFlg &= ~0x02;
		}
		else if ( kVer == ID_VER_SYZ26 ) {
			m_ActedViewFlg &= ~0x01;
		}

		// 初期化完了フラグを落とす
		// 会社切り替えを行ったときに初期化が行われないことがあったため
		pBaseView->SetInitedFlg ( false );
	}
	//<----------------


	// 課税取引計算書ビュー
	//CTaxationListView *pTaxationListView = (CTaxationListView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd( 3 ));
	//if( pTaxationListView != NULL ){
	//	pTaxationListView->EndJob();
	//}
	// 課税取引計算書ビュー
	CBaseTaxHyoView *pBaseTaxView = GetTaxListView();
	if( pBaseTaxView != NULL ){
//--> '15.01.24 INS START
		if( m_nowTabIdx > 0 ){
			// 計算元になる帳票で終了した時に、関連データの更新が行われなかった為
			//pBaseView->UpdateAllSheet();	// taxにはない関数なので閉じる
		}
//--> '15.01.24 INS END
		pBaseTaxView->EndJob( wflg );

		// 起動したサインが残っていることでビューの破棄が行われるときにビューが作成されていた
		int kVer = GetKaiseiVer();
		if ( kVer == ID_VER_SYZ31 ) {
			m_ActedViewFlg &= ~0x02;
		}
		else if ( kVer == ID_VER_SYZ26 ) {
			m_ActedViewFlg &= ~0x01;
		}
	}

	return 0;
}

#ifdef _20150727_EXTEND_
//-----------------------------------------------------------------------------
// 各付表の終了処理　フォーカスセット
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::EndViewFocus(){

	CH26HyoView	*pH26View = (CH26HyoView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd( 1 ));
	if( pH26View != NULL ){
		pH26View->EndJobFocus();
	}
	return 0;
}
#else
//-----------------------------------------------------------------------------
// 各付表の終了処理　フォーカスセット
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::EndViewFocus(){

	if( m_nowTabIdx > 0 ){
		CBaseHyoView *pBaseView = GetHonHyoView();
		if( pBaseView != NULL ){
			pBaseView->EndJobFocus();
		}
	}
	else if( m_nowTabIdx == 0 ){
		if( m_pMainView != NULL ){
			m_pMainView->EndJobFocus();
		}
	}
	return 0;
}
#endif

//-----------------------------------------------------------------------------
// エラー終了
//-----------------------------------------------------------------------------
// 引数	ErrStr	：	エラー内容
//-----------------------------------------------------------------------------
void CMainFrame::ErrorExit( const char *pErrStr )
{
	ICSMessageBox( pErrStr, MB_ICONSTOP );
	PostMessage( WM_CLOSE, 0x2000 );
}

//-----------------------------------------------------------------------------
// フォーク情報の取得
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CMainFrame::GetForkInfo()
{
	char	temp[5][64] = {0};
	memset( &m_BpPrintInfo, '\0', sizeof(m_BpPrintInfo) );

	m_swBeForked = 0;
	if( PCOM_GetString(_T("ProgramType"), temp[0]) != -1 ){
		if( !strcmp(temp[0], _T("Fork")) ){
			// フォーク
			if( UCOM_GetString(_T("呼出プログラム"), temp[1]) != -1 ) {
				if( !strcmp(temp[1], _T("一括印刷")) ) {
					m_swBeForked = 1;
				}
				else if( !strcmp(temp[1], _T("非連動")) ) {
					m_isHirendoCall = true;
				}
			}
		}
	}

//#ifdef _DEBUG
//m_swBeForked = 1;
//
//// ※依頼キーの参考→ 財務：DBSyzShin1b0001、公益Ⅲ：DBSyzShin1f0001、学校：DBSyzShin280001
//
////UCOM_SetString(_T("呼出プログラム"),_T("一括印刷"));
////UCOM_SetString(_T("依頼プログラム"),_T("消費税申告書"),_T("51"));
////UCOM_SetString(_T("依頼項目"),_T("詳細設定"));
////UCOM_SetString(_T("依頼キー"),_T("DBSyzShin1f0001"));
////UCOM_SetString(_T("設定年月日"),_T("-1"),_T("-1"),_T("-1"),_T("-1"));
//
//UCOM_SetString(_T("呼出プログラム"),_T("一括印刷"));
//UCOM_SetString(_T("依頼プログラム"),_T("消費税申告書"),_T("0"));
//UCOM_SetString(_T("依頼項目"),_T("印刷"));
//UCOM_SetString(_T("依頼キー"),_T(""));
//UCOM_SetString(_T("依頼詳細"),_T("1"),_T("Antenna House PDF Driver 7.5"));
//UCOM_SetString(_T("出力年月日"),_T("-1"),_T("-1"),_T("-1"),_T("-1"));
//UCOM_SetString(_T("印刷状態"),_T(""));
//UCOM_SetString(_T("出力詳細"),_T(""));
//#endif

	if( m_swBeForked ){
		// 会社選択ボタンを無効に
		EnableCoselButton( FALSE );
		// 詳細設定
		if( UCOM_GetString(_T("依頼項目"), temp[0]) != -1 ){
			if( strcmp(temp[0], _T("CDR出力")) == 0 ){
				m_swBeForked = 3;
			}
			else if( strcmp(temp[0], _T("印刷")) == 0 ){
				m_swBeForked = 2;
			}
			else{
				m_swBeForked = 1;	// 詳細設定
			}
		}
		// 共通
		if( UCOM_GetString(_T("依頼キー"), temp[0]) != -1 ){
			sprintf_s( m_BpPrintInfo.ForkKey, sizeof(m_BpPrintInfo.ForkKey), _T("%s"), temp[0] );
		}
		//	詳細設定・印刷・CDR
		if( m_swBeForked == 1 ){	//	詳細設定
			if( UCOM_GetString(_T("設定年月日"), temp[0], temp[1], temp[2], temp[3]) != -1 ){
				m_BpPrintInfo.date_start = atoi( temp[0] );
				m_BpPrintInfo.date_end = atoi( temp[1] );
				m_BpPrintInfo.ofs_start	= atoi( temp[2] );
				m_BpPrintInfo.ofs_end = atoi( temp[3] );
			}
		}
		else if( (m_swBeForked==2) || (m_swBeForked==3) ){	// 印刷及びＣＤＲ印刷
			if( UCOM_GetString(_T("依頼詳細"), temp[0], temp[1], temp[2]) != -1 ) {
				sprintf_s( m_BpPrintInfo.PrinterName, sizeof(m_BpPrintInfo.PrinterName), _T("%s"), temp[1] );	// プリンタ名
				m_BpPrintInfo.copynum = atoi( temp[0] );	// 部数
				if( m_BpPrintInfo.copynum <= 0 ){
					m_BpPrintInfo.copynum = 1;
				}
			}
			if( UCOM_GetString(_T("出力年月日"), temp[0], temp[1], temp[2], temp[3]) != -1 ){
				m_BpPrintInfo.date_start	= atoi( temp[0] );
				m_BpPrintInfo.date_end	= atoi( temp[1] );
				m_BpPrintInfo.ofs_start	= atoi( temp[2] );
				m_BpPrintInfo.ofs_end		= atoi( temp[3] );
			}
			//----> 20240116
			if( UCOM_GetString( _T("SCORELINK依頼ID"), temp[0] ) != -1 ){
				// scorelinkからの呼び出し：0
				// 一括印刷からの呼び出し：-1
				m_isScoreLinkCall = atoi( temp[0] );
			}
			if( m_isScoreLinkCall != -1 ){
				if( UCOM_GetString( _T("SCORELINK出力PDF名称"), temp[0] ) != -1 ){
					// scorelinkからの呼び出し：消費税申告書
					// 一括印刷からの呼び出し：-1
					m_ScoreLinkPdfNam.Format( "%s", temp[0] );
				}
			}
			//<---- 20240116
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 年度に合わせたプログラムの呼び出し
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::CallSyzProgram( BOOL isFirst )
{
	char	dev[20] = {0};
	char	cmline[128] = {0};

	PCOM_GetString( _T("ProgramDevice"), dev );

	if( m_pZmSub->VolumeOpen() ){
		ErrorExit( m_pZmSub->GetZmErrMsg() );
		return -1;
	}

	// 年度チェック
	if( IsBeforeH25Master(1) ){

#ifdef _DEBUG
		ErrorExit( _T("過年度プログラム呼出") );
		return -1;
#else
		if( isFirst ){

			CallSyzAllYearProgram();

/*			char	path[_MAX_PATH];
			ToggleMDITabs();

			// 平成26年4月改正対応前まで
			wsprintf( path, _T("%s:\\ICSWin\\CMDS\\GR1b\\JOBS30\\H25\\DBSyzShin.exe"), dev );

			ICSFork8_Ex( AfxGetMainWnd()->m_hWnd,
						path, 
						cmline,
						SW_SHOWMAXIMIZED,
						SW_HIDE,//SW_MAXIMIZE 
						FALSE,//TRUE
						0x0004000
						);

			m_swFork = 1;

			// 旧プロ呼出
			CH26HyoView	*pH26View = (CH26HyoView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd( 1 ));
			if( pH26View != NULL ){
				pH26View->SetDispedFlg( FALSE );
			}*/
		}
		else{
			ICSMessageBox( _T("期末日が平成26年3月31日以前の会社に切り換えることは出来ません。\n処理を終了します。"), MB_ICONSTOP );
			return -1;
		}
#endif

		return 1;
	}
	else{

#ifdef _20140314_SYUKA_
		if( m_swBeForked ){
			ErrorExit( _T("平成26年4月改正の消費税申告書は、現在未対応です。\n対応までしばらくお待ちください。") );
			return -1;
		}
#endif
		// バージョンチェック
		if( m_pZmSub->zvol->s_ver < 17 ){
			ErrorExit( _T("期末日が平成26年4月1日以降ですが『消費税改正バージョンアップ』が未了の為、処理できません。\n『導入・更新』の『平成２６年４月消費税改正』業務でバージョンアップを行ってください。") );
			return -1;
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 履歴に合わせたプログラムの呼び出し
//-----------------------------------------------------------------------------
// 引数	seq		：	履歴番号
//-----------------------------------------------------------------------------
// 返送値	1	：	正常終了
//-----------------------------------------------------------------------------
int CMainFrame::CallSyzHistoryProgram( int seq )
{
	char	path[_MAX_PATH];
	char	dev[20] = {0};
	char	cmline[128] = {0};

//--> '14.04.11 INS START
	if( !(m_SnHeadData.Sn_ZeiHou&0x80) ){
		m_SnHeadData.Sn_ZeiHou |= 0x80;
		m_SnHeadData.SetData( m_pZmSub, seq );
	}
//<-- '14.04.11 INS END

//--> '14.04.11 INS START
	m_svSVmzsw = -1;
	if( seq != 0 ){
		if( m_pZmSub ){
			if( m_pZmSub->szvol == NULL ){
				m_pZmSub->SyohizeiOpen();
			}
			m_svRealSvmzsw = m_pZmSub->szvol->SVmzsw;;
			m_svSVmzsw = m_pZmSub->szvol->SVmzsw;
		}
	}
//<-- '14.04.11 INS START

	CString	seqStr;
	seqStr.Format( _T("%d"), seq );

	PCOM_GetString( _T("ProgramDevice"), dev );

	UCOM_SetString( _T("呼出プログラム"), _T("履歴") );
	UCOM_SetString( _T("履歴番号"), seqStr.GetBuffer() );

	ToggleMDITabs();

	// 平成26年4月改正対応前まで
/*- '14.04.28 -*/
//	wsprintf( path, PAST_PROGRAM_PATH, dev );
/*-------------*/
	wsprintf( path, PAST_PROGRAM_PATH, m_PrgPath );
/*-------------*/

/*- '14.03.15 -*/
//	ICSFork8_Ex( AfxGetMainWnd()->m_hWnd,
//				path, 
//				cmline,
//				SW_SHOWMAXIMIZED,
//				SW_HIDE,//SW_MAXIMIZE 
//				FALSE,//TRUE
//				0x0004000
//				);
/*-------------*/
	ICSFork8( AfxGetMainWnd()->m_hWnd,
				path, 
				cmline,
				SW_SHOWMAXIMIZED,
				SW_HIDE,//SW_MAXIMIZE 
				FALSE//TRUE
				);
/*-------------*/

	m_swFork = 2;
	m_isKeyoff = TRUE;
	// 旧プロ呼出
	CH26HyoView	*pH26View = (CH26HyoView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(H26_HONPYO_WND_IDX));
	if( pH26View != NULL ){
		pH26View->SetDispedFlg( FALSE );
	}

	return 1;
}

#ifdef _20150219_EXTEND_
//-----------------------------------------------------------------------------
// 過年度プログラム呼出マスタ？
//-----------------------------------------------------------------------------
// 引数	chktype		：	元情報タイプ（0:コモン情報, 1:ボリュームラベル）
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	過年度プロ呼出マスター
//			FALSE	：	過年度プロ呼出マスターでない
//-----------------------------------------------------------------------------
BOOL CMainFrame::IsBeforeH25Master( int chktype )
{
	BOOL	bRt = FALSE;

	int		n_apno, n_cocode, n_kikan;

	if( chktype == 0 ){
		char	sqlsvr[128]={0}, apno[32]={0}, cocode[32]={0}, kikan[32]={0};
		PCOM_GetString( "SELECTCODSN", sqlsvr, apno, cocode, kikan );
		n_apno = atoi( apno );
		n_cocode = atoi( cocode );
		n_kikan = atoi( kikan );
	}
	else if( chktype == 1 ){
		n_kikan = m_pZmSub->zvol->ee_ymd;
	}
	else{
		// 当年度のプログラム呼出
		n_kikan = 99999999;
	}

	if( n_kikan < ICS_SH_8PER_REFORM_DAY ){
		bRt = TRUE;
	}
	//----->150107
	else{
		m_eymd = n_kikan;
	}
	//<-----------

	if( m_swBeForked ){
		m_pZmSub->SzsnkOpen();
		switch(m_pZmSub->szsnk->Sn_SKKBN&0xff ){
			case 1 : case 3 :	// 確定・確定修正
				n_kikan = m_pZmSub->szsnk->Sn_KDAYE;
				break;
			default:			//中間・中間修正
				n_kikan = m_pZmSub->szsnk->Sn_MDAYE;
				break;
		}
		m_pZmSub->SzsnkClose();
		// 当年度のプログラム呼出
		if( n_kikan < ICS_SH_8PER_REFORM_DAY ){
			bRt = TRUE;
		}
	}

//--> '14.03.27 INS START
	if( (chktype==0) && bRt ){

		char	sqlsvr[128]={0}, apno[32]={0}, cocode[32]={0}, kikan[32]={0};
		PCOM_GetString( "SELECTCODSN", sqlsvr, apno, cocode, kikan );
		n_apno = atoi( apno );
		n_cocode = atoi( cocode );
		n_kikan = atoi( kikan );
	
		CDBNpSub	*ptmpZmSub = NULL;

		// 公益クラス
		if( (ptmpZmSub = new CDBNpSub) == NULL ){
			;
		}
		ptmpZmSub->ExitMode( 1 );

		if( ptmpZmSub->SetMasterDB(3, sqlsvr, n_apno, n_cocode, n_kikan) == -1 ){
			;
		}
		ptmpZmSub->VolumeOpen();

		ChkSzvolVersion( ptmpZmSub );

		ptmpZmSub->VolumeClose();

		if( ptmpZmSub ){
			delete ptmpZmSub;
			ptmpZmSub = NULL;
		}
	}
//<-- '14.03.27 INS END

	return bRt;
}
#else
//-----------------------------------------------------------------------------
// 過年度プログラム呼出マスタ？
//-----------------------------------------------------------------------------
// 引数	chktype		：	元情報タイプ（0:コモン情報, 1:ボリュームラベル）
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	過年度プロ呼出マスター
//			FALSE	：	過年度プロ呼出マスターでない
//-----------------------------------------------------------------------------
BOOL CMainFrame::IsBeforeH25Master( int chktype )
{
	BOOL	bRt = FALSE;

	//----------->特定課税仕入対応150713
	BOOL openFlg = FALSE;
	//<----------------------------------


	int		n_apno, n_cocode, n_kikan;

	if( chktype == 0 ){
		char	sqlsvr[128]={0}, apno[32]={0}, cocode[32]={0}, kikan[32]={0};
		PCOM_GetString( "SELECTCODSN", sqlsvr, apno, cocode, kikan );
		n_apno = atoi( apno );
		n_cocode = atoi( cocode );
		n_kikan = atoi( kikan );
	}
	else if( chktype == 1 ){
		n_kikan = m_pZmSub->zvol->ee_ymd;
	}
	else{
		// 当年度のプログラム呼出
		n_kikan = 99999999;
	}

	if( n_kikan < ICS_SH_8PER_REFORM_DAY ){
		bRt = TRUE;
	}
	//----->150107
//	else{
//		m_symd = n_kikan;
//	}
//	//<-----------

	if( m_swBeForked ){
		m_pZmSub->SzsnkOpen();
		//---->yoshida190905
		if (m_pZmSub->szsnk->st != -1) {//テーブル存在する場合
			switch (m_pZmSub->szsnk->Sn_SKKBN & 0xff) {
				case 1: case 3:	// 確定・確定修正
					n_kikan = m_pZmSub->szsnk->Sn_KDAYE;
					m_symd = m_pZmSub->szsnk->Sn_KDAYS;
					break;
				default:			//中間・中間修正
					n_kikan = m_pZmSub->szsnk->Sn_MDAYE;
					m_symd = m_pZmSub->szsnk->Sn_MDAYS;
					break;
			}

		}
		else {
			m_symd = m_pZmSub->zvol->ss_ymd;
			n_kikan = m_pZmSub->zvol->ee_ymd;
		}

		//-------------------
//		switch(m_pZmSub->szsnk->Sn_SKKBN&0xff ){
//			case 1 : case 3 :	// 確定・確定修正
//				n_kikan = m_pZmSub->szsnk->Sn_KDAYE;
//				break;
//			default:			//中間・中間修正
//				n_kikan = m_pZmSub->szsnk->Sn_MDAYE;
//				break;
//		}
////--> '16.07.21 INS START
//		if( m_pZmSub->szsnk->st != -1 ){
//			m_symd = m_pZmSub->szsnk->Sn_KDAYS;
//		}
//		else{
//			m_symd = m_pZmSub->zvol->ss_ymd;
//		}
////<-- '16.07.21 INS END
		//<-----------------

		m_pZmSub->SzsnkClose();
		// 当年度のプログラム呼出
		if( n_kikan < ICS_SH_8PER_REFORM_DAY ){
			bRt = TRUE;
		}
	}
//--> '15.02.19 INS START
	else{
		if( (chktype==1) && (m_Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_KANNI) && (n_kikan>=ICS_SH_SIXKIND_REFORM_DAY) ){
			//----------->特定課税仕入対応150713
			//コメント化。場所変更
//			BOOL openFlg = FALSE;
			//<------------------------------------

			if( m_pZmSub->szsnk == NULL ){
				m_pZmSub->SzsnkOpen();
				openFlg = TRUE;
			}
			else{
				CString	filter;
				filter.Format( _T("Sn_SEQ=%d"), m_SnSeq );
				m_pZmSub->szsnk->Requery( filter );
			}

			if( m_pZmSub->szsnk->st != -1 ){
//-- '15.04.28 --
//				switch(m_pZmSub->szsnk->Sn_SKKBN&0xff ){
//					case 1 : case 3 :	// 確定・確定修正
//						m_symd = m_pZmSub->szsnk->Sn_KDAYS;
//						break;
//					default:			//中間・中間修正
//						m_symd = m_pZmSub->szsnk->Sn_MDAYS;
//						break;
//				}
//---------------
				m_symd = m_pZmSub->szsnk->Sn_KDAYS;
//---------------
			}
			else{
				m_symd = m_pZmSub->zvol->ss_ymd;
			}


			//----------->特定課税仕入対応150713
			//コメント化。閉じる場所変更
			/*
			if( openFlg ){
				m_pZmSub->SzsnkClose();
			}
			*/
			//<------------------------------------
			
		}
	}
//<-- '15.02.19 INS END


	//----------->特定課税仕入対応150713
	if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){//原則課税の場合
		if(openFlg == FALSE){
			if( m_pZmSub->szsnk == NULL ){
				m_pZmSub->SzsnkOpen();
				openFlg = TRUE;
			}
			else{
				CString	filter;
				filter.Format( _T("Sn_SEQ=%d"), m_SnSeq );
				m_pZmSub->szsnk->Requery( filter );
			}
		}

		if( m_pZmSub->szsnk->st != -1 ){
//-- '15.11.12 --
//			m_eymd = m_pZmSub->szsnk->Sn_KDAYE;
//---------------
			if( (m_pZmSub->szsnk->Sn_SEQ==0) && ((m_SnHeadData.SVsasw&0x01)==0) ){
				m_eymd = m_pZmSub->zvol->ee_ymd;
			}
			else{
				m_eymd = m_pZmSub->szsnk->Sn_KDAYE;
			}
//---------------
		}
		else{
			m_eymd = m_pZmSub->zvol->ee_ymd;
		}
	}

	//閉じる場所変更後
	if( openFlg ){
		m_pZmSub->SzsnkClose();
	}
	//<------------------------------------




//--> '14.03.27 INS START
	if( (chktype==0) && bRt ){

		char	sqlsvr[128]={0}, apno[32]={0}, cocode[32]={0}, kikan[32]={0};
		PCOM_GetString( "SELECTCODSN", sqlsvr, apno, cocode, kikan );
		n_apno = atoi( apno );
		n_cocode = atoi( cocode );
		n_kikan = atoi( kikan );
	
		CDBNpSub	*ptmpZmSub = NULL;

		// 公益クラス
		if( (ptmpZmSub = new CDBNpSub) == NULL ){
			;
		}
		ptmpZmSub->ExitMode( 1 );

		if( ptmpZmSub->SetMasterDB(3, sqlsvr, n_apno, n_cocode, n_kikan) == -1 ){
			;
		}
		ptmpZmSub->VolumeOpen();

		ChkSzvolVersion( ptmpZmSub );

		ptmpZmSub->VolumeClose();

		if( ptmpZmSub ){
			delete ptmpZmSub;
			ptmpZmSub = NULL;
		}
	}
//<-- '14.03.27 INS END

	return bRt;
}
#endif

//-----------------------------------------------------------------------------
// 通年のプログラム呼出
//-----------------------------------------------------------------------------
// 返送値	1	：	正常終了
//-----------------------------------------------------------------------------
int CMainFrame::CallSyzAllYearProgram()
{
	char	dev[20] = {0};
	char	cmline[128] = {0};
	PCOM_GetString( _T("ProgramDevice"), dev );

	if( m_swBeForked ){
		UCOM_SetString( _T("呼出プログラム"), _T("一括印刷") );
		UCOM_SetString( _T("依頼キー"), m_BpPrintInfo.ForkKey );
	}
	else{
		UCOM_SetString( _T("呼出プログラム"), _T("通年") );
	}
	char	path[_MAX_PATH];
	ToggleMDITabs();

	// 平成26年4月改正対応前まで
/*- '14.04.28 -*/
//	wsprintf( path, PAST_PROGRAM_PATH, dev );
/*-------------*/
	wsprintf( path, PAST_PROGRAM_PATH, m_PrgPath );
/*-------------*/

/*- '14.03.14 -*/
//	ICSFork8_Ex( AfxGetMainWnd()->m_hWnd,
//				path, 
//				cmline,
//				SW_SHOWMAXIMIZED,
//				SW_HIDE,//SW_MAXIMIZE 
//				FALSE,//TRUE
//				0x0004000
//				);
/*-------------*/
	ICSFork8( AfxGetMainWnd()->m_hWnd,
				path, 
				cmline,
				SW_SHOWMAXIMIZED,
				SW_HIDE,//SW_MAXIMIZE 
				FALSE//TRUE
				);
/*-------------*/

	m_swFork = 1;
	m_isKeyoff = TRUE;

	// 旧プロ呼出
	CH26HyoView	*pH26View = (CH26HyoView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(H26_HONPYO_WND_IDX));
	if( pH26View != NULL ){
		pH26View->SetDispedFlg( FALSE );
	}

	return 1;
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 引数	isFirst	：	初期呼出？
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::InitApl( BOOL isFirst )
{
/*	unsigned char Sn_DIFRI[80]={0};

	CINF_REC	CoRec = {0};
	char	WKbuf[128], Ascii[128], kana[128], kanj[128];
	memset( kana,'\0', sizeof( kana ) );
	memset( kanj,'\0', sizeof( kanj ) );

	CoRec.ciKana[0] = 0xd5;
	CoRec.ciKana[1] = 0xb3;
	CoRec.ciKana[2] = 0xb9;
	CoRec.ciKana[3] = 0xb3;

	memmove( kana, CoRec.ciKana, 34 );
	m_Util.AtoJis( kanj, kana, 34 );
	memset( Sn_DIFRI, '\0', 68 );
	memmove( Sn_DIFRI, kanj, strlen( kanj ) );*/

	// DB接続
	if( ConnectDB() ){
		return -1;
	}

//--> '14.04.28 MOVE START
	// 環境情報の取得
	if( GetEnvInfo() ){
		return -1;
	}
//<-- '14.04.28 MOVE END

	// 年度に合わせたプログラムの呼出
	if( CallSyzProgram(isFirst) ){
		return -1;
	}

//--> '14.04.28 MOVE START
//	// 環境情報の取得
//	if( GetEnvInfo() ){
//		return -1;
//	}
//<-- '14.04.28 MOVE END

	// テーブルオープン 
	if( OpenZmtbl() ){
		return -1;
	}

	// 集計クラス初期化
	if( InitSyzSyukei() ){
		return -1;
	}

	// 個人決算書金額の取得可能？
	IsGetKoketsuKingaku();

	return 0;
}

//-----------------------------------------------------------------------------
// 集計クラス初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::InitSyzSyukei()
{
/*- CUT -*/
	// 基礎データ
	if( m_pSzzanList ){
		delete m_pSzzanList;
		m_pSzzanList = NULL;
	}
	if( (m_pSzzanList = new CSzzanList()) == NULL ){
		return -1;
	}
	if( m_pSzzanList->initialize(m_pZmSub) ){
		return -1;
	}

	// 外部集計
	if( m_pSyzSyukei ){
		delete m_pSyzSyukei;
		m_pSyzSyukei = NULL;
	}

	if( (m_pSyzSyukei = new CSyzSyukei()) == NULL ){
		return -1;
	}
	if( m_pSyzSyukei->initialize(m_pSzzanList) ){
		return -1;
	}
/*-------*/
/*	if( m_pSyzSyukei ){
		delete m_pSyzSyukei;
		m_pSyzSyukei = NULL;
	}

	if( (m_pSyzSyukei = new CSyzSyukei()) == NULL ){
		return -1;
	}
	if( m_pSyzSyukei->initialize(m_pZmSub) ){
		return -1;
	}*/
/*-------*/
	// 月別情報を全体に開きなおす...
	m_pZmSub->minfo->Requery( _T("") );

	char	uwriai[MONY_BUF_SIZE]={0};
	if( m_SnHeadData.IsReadCalqedData() ){

		GetMofs();//

		if( StartSyzSyukei(uwriai) ){
			return -1;
		}

		// 月別情報を全体に開きなおす...
		m_pZmSub->minfo->Requery( _T("") );
	}
	else { // 非連動（集計モジュール初期化のための仮対応）
		GetMofs();//

		/*if (StartSyzSyukei(uwriai)) {
			return -1;
		}*/
		CSyzScopeOption	scopeOption;

		int		tmpMmdd = 0, sofs = -1, eofs = -1;
		char	buf[16] = { 0 };
		int		endYymmdd = 0;
		sprintf_s(buf, sizeof(buf), _T("%02x%02x"), m_Vol2.D_TBL[m_ShinInfo.s_tno]._EMD[0], m_Vol2.D_TBL[m_ShinInfo.s_tno]._EMD[1]);
		int	sMmdd = atoi(buf);
		sprintf_s(buf, sizeof(buf), _T("%02x%02x"), m_Vol2.D_TBL[m_ShinInfo.d_tno]._EMD[0], m_Vol2.D_TBL[m_ShinInfo.d_tno]._EMD[1]);
		int	eMmdd = atoi(buf);
		for (int i = 0;; i++) {
			if (i == 0) {
				if ((m_pZmSub->minfo->MoveFirst() == -1) || (m_pZmSub->minfo->st == -1)) {
					break;
				}
			}
			else {
				if ((m_pZmSub->minfo->MoveNext() == -1) || (m_pZmSub->minfo->st == -1)) {
					break;
				}
			}
			tmpMmdd = m_pZmSub->minfo->emd % 10000;
			if (sofs == -1) {
				if (tmpMmdd == sMmdd) {
					sofs = m_pZmSub->minfo->ofset;
				}
			}
			if (tmpMmdd == eMmdd) {
				eofs = m_pZmSub->minfo->ofset;
				//--> '14.03.10 INS START
				endYymmdd = m_pZmSub->minfo->emd;
				//<-- '14.03.10 INS END
			}
			else if (eofs != -1) {
				break;
			}
		}
		scopeOption.sofs = sofs;
		scopeOption.eofs = eofs;

		if (scopeOption.sofs < 0) {
			scopeOption.sofs = 0;
		}
		if (scopeOption.eofs > 15) {
			scopeOption.eofs = 15;
		}

			// 一次集計
		if (m_SnHeadData.Sn_SPECIAL & 0x01) {
			CSyzCalqOption	clqOption;
			clqOption.SimpleSw = TRUE;
			clqOption.MakeShisan = 1;
			clqOption.shisan_rawdata = 1;
			if( m_SnHeadData.m_DispTabSgn & 0x2000 ){
				// 第二種固定で計算する為にセット
				clqOption.SimComp = TRUE;		// TRUEにしておかないと集計モジュール側で以下2つのフラグを参照しないためTRUEにする
				clqOption.SimKazeiType = kani;  // 課税方式
				clqOption.SimCategorySw = 2;    // 簡易課税の基本業種(1-6を指定)
			}
			if (m_pSyzSyukei->stmonlycalq(&scopeOption, &clqOption)) {
				return -1;
			}
		}
		else {
			CSyzCalqOption	clqOption;
			if (m_SnHeadData.Sn_TUMIAG & 0x01) {
				clqOption.K221 = TRUE;
			}
			if (m_SnHeadData.Sn_TUMIAG & 0x02) {
				clqOption.H301 = TRUE;
			}
			clqOption.MakeShisan = 1;
			clqOption.shisan_rawdata = 1;
			if( m_SnHeadData.m_DispTabSgn & 0x2000 ){
				// 第二種固定で計算する為にセット
				clqOption.SimComp = TRUE;		// TRUEにしておかないと集計モジュール側で以下2つのフラグを参照しないためTRUEにする
				clqOption.SimKazeiType = kani;  // 課税方式
				clqOption.SimCategorySw = 2;    // 簡易課税の基本業種(1-6を指定)
			}

			if (m_pSyzSyukei->stmonlycalq(&scopeOption, &clqOption)) {
				return -1;
			}
		}

		// 実データ集計

		//先に１で呼んでおかないとリバースチャージの判定ができない為、コール
		m_pSyzSyukei->MakeData(0x01);
		
		if (m_pSyzSyukei->IsReverseCharge() == false) {
			m_pSyzSyukei->MakeData(-1);
		}
		
		

		// 月別情報を全体に開きなおす...
		m_pZmSub->minfo->Requery(_T(""));
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 環境情報の取得
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::GetEnvInfo()
{
	// 標準色作成
/*	((ICSWinApp*)AfxGetApp())->GetStanderdColor( ColorInfo );
	if( ColorInfo.m_swOnOff == 1 ) {
		// バックカラーの取得
		COLORREF Pre = ColorInfo.ViewColor();
		for( int i = 0; ItemNo[i]; i++ ) {
			cl_dotoff( ItemNo[i], Pre, CIcsdisp8ctrl1 );
		}
	}*/

	// パスの作成
	m_PrgPath.Empty();
	m_SysPath.Empty();
	m_SysGrPath.Empty();
	m_GrPath.Empty();
	m_PrgMdev.Empty();

#ifdef _DEBUG
	char	SecBf1[20]={0};
	if( PCOM_GetString(_T("ProgramDevice"), SecBf1) == -1 ){
		m_PrgMdev = _T("D");
	}
	else{
		m_PrgMdev = SecBf1;
	}
	m_PrgPath.Format( _T("%s:\\ICSWIN\\CMDS\\GR1B\\JOBS30"), m_PrgMdev );
	m_SysPath.Format( _T("%s:\\ICSWIN\\SYS"), m_PrgMdev );
	m_SysGrPath.Format( _T("%s:\\ICSWIN\\SYS\\GR1B"), m_PrgMdev );
	m_GrPath = _T("1B");
#else
	char	SecBf1[20]={0}, SecBf2[20]={0}, SecBf3[20]={0}, SecBf4[20]={0};
	// プログラムデバイス
	if( PCOM_GetString(_T("ProgramDevice"), SecBf1) == -1 ){
		ErrorExit( _T("プログラムデバイスが取得できませんでした。" ) );
		return -1;
	}
	m_PrgMdev = SecBf1;
	// グループ番号
	if( PCOM_GetString(_T("ExecGrJobs"), SecBf2, SecBf3 ) == -1 ){
		ErrorExit( _T("プログラム管理番号が取得できませんでした。" ) );
		return -1;
	}
	m_GrPath = SecBf2;
	// 共用デバイス
	if( GCOM_GetString(_T("SystemDrive"), SecBf4) == -1 )	{
		ErrorExit( _T("共用デバイスが取得できませんでした。" ) );
		return -1;
	}
	m_PrgPath.Format( _T("%s:\\ICSWIN\\CMDS\\GR%s\\JOBS%s"), SecBf1, SecBf2, SecBf3 );
	m_SysPath.Format( _T("%s:\\ICSWIN\\SYS"), SecBf4 );
	m_SysGrPath.Format( _T("%s\\GR%s"), m_SysPath, SecBf2 );
#endif

	// ドット・プリンターチェック
	m_sgDotPrn = 0;
	char	PrinterName[128] ={0};
	if( PCOM_GetString(_T("SelectPrinter"), PrinterName) != -1 ){
		m_sgDotPrn = IsDotPrinter( PrinterName );
	}
	m_ShinInfo.isDotPrn = m_sgDotPrn;

//-- '18.12.17 --	
//	// ユーザ情報の取得
//	ICSMCTL		icsmctl;
//	if( icsmctl.SetDatabase() ){
//		ErrorExit( _T("マスターコントロールテーブルのセットに失敗しました。") );
//		return -1;
//	}
//	icsmctl.get_usertbl( m_usertbl );
//---------------

	return 0;
}

//-----------------------------------------------------------------------------
// DB接続
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::ConnectDB()
{
	// 情報の取得
	char	sqlsvr[128]={0}, apno[32]={0}, cocode[32]={0}, kikan[32]={0};
	int		n_apno, n_cocode, n_kikan;
	PCOM_GetString( "SELECTCODSN", sqlsvr, apno, cocode, kikan );
	n_apno = atoi( apno );
	n_cocode = atoi( cocode );
	n_kikan = atoi( kikan );
	
	// 非営利法人？
	m_isNonCorp = FALSE;
	if( (n_apno&0xf0) > 0x30 ){
		m_isNonCorp = TRUE;
	}

	if( m_pZmSub ){
		// 公益クラスの破棄
		CloseZmtbl();
	}

	// 公益クラス
	if( (m_pZmSub = new CDBNpSub) == NULL ){
		ErrorExit( _T("財務クラスの初期化に失敗しました") );
		return -1;
	}
	m_pZmSub->ExitMode( 1 );

	if( m_pZmSub->SetMasterDB(3, sqlsvr, n_apno, n_cocode, n_kikan) == -1 ){
		ErrorExit( m_pZmSub->GetZmErrMsg() );
		return -1;
	}

#ifdef _DEBUG
	if( m_pZmSub ){
		// 公益クラスの破棄
		CloseZmtbl();
	}

	// 公益クラス
	if( (m_pZmSub = new CDBNpSub) == NULL ){
		ErrorExit( _T("財務クラスの初期化に失敗しました") );
		return -1;
	}
	m_pZmSub->ExitMode( 1 );

	if( m_pZmSub->SetMasterDB(3, sqlsvr, n_apno, n_cocode, n_kikan) == -1 ){
		ErrorExit( m_pZmSub->GetZmErrMsg() );
		return -1;
	}
#endif

	// マスタの状態チェック
	CZmGen8			ZmGen;
	CString			str;
	ZMGEN_CHKINFO	chkinfo = { 0 };

	//消費税非連動対応。内訳書共存マスターも可能に。7月出荷分なので処理は一旦コメント化yoshida200420
	//消費税非連動対応。内訳書共存マスターも可能に。８月出荷となり新表対応と同時期にアップ。コメントはずした
	chkinfo.verupsub |= 0x800;

	int rt = ZmGen.CheckDBMasterState( m_pZmSub, str, chkinfo );
	if( (rt==-1) || (rt==-2) ){		// エラー
		// エラー処理
		ErrorExit( ZmGen.GetErrorMsg() );
		return -1;
	}else if( rt == 1 ){	// メッセージを表示する必要がある正常終了
		// 注意喚起のメッセージ表示をして、処理継続
		ICSMessageBox( str );
	}




//------>'23.02.03
//インボイス出力出荷（9月予定分）前までは起動時チェックで止める。出力対応後はこの処理をカット予定
//CSyzMakeSub SyzMkSub;
//if (SyzMkSub.CheckWrongData(m_pZmSub)) {
//	ErrorExit(SyzMkSub.GetErrorMsg());
//	return -1;
//}
//<--------


//'19.08.28
//処理カット
/*
//---->処理を有効化させる。
	// 消費税情報のチェック
	CSyzMakeSub SyzMkSub;
	if (SyzMkSub.CheckWrongData(m_pZmSub)) {
		ErrorExit(SyzMkSub.GetErrorMsg());
		return -1;
	}

//-----

*/

#ifndef _DEBUG	// 正式出荷まで、テストのために判定カット

#ifdef _CUT//リリーステストする為に処理カットyoshida
	// 消費税情報のチェック
	CSyzMakeSub SyzMkSub;
	if( SyzMkSub.CheckWrongData(m_pZmSub) ) {
		ErrorExit( SyzMkSub.GetErrorMsg() );
		return -1;
	}
#endif

#endif
//<------

	char	syssvr[128]={0};
	GCOM_GetString( _T("SystemDataBase"), syssvr );
	if( m_pZmSub->SysCtlDB(syssvr) == -1 ){
		ErrorExit( m_pZmSub->GetZmErrMsg() );
		return -1;
	}

//--> '14.03.26 INS START
	if( ChkSzvolVersion(m_pZmSub) ){
		ErrorExit( _T("消費税申告書情報の更新に失敗しました") );
		return -1;
	}
//<-- '14.03.26 INS END

//--> yoshida200925
	if (Chkzvolsubsw(m_pZmSub)) {
		ErrorExit(_T("消費税申告書非連動サイン情報の更新に失敗しました"));
		return -1;
	}
//<-- 

	if ( ChkTkreiTable( m_pZmSub ) ) {
		return -1;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// テーブルオープン
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::OpenZmtbl()
{
/*	// 期間モジュール
	if( pVolDate == NULL )	pVolDate = new CVolDateDB;
	if( pVolDate == NULL ) {
		ErMs = _T("ＤＢ期間モジュールクラス");
		ErrorExit( ErMs );
		return;
	}
	// テーブルハンドリングモジュール
	if( pTblHdl == NULL )	pTblHdl = new CTblhdl;
	if( pTblHdl == NULL ) {
		ErMs = _T("ＤＢテーブルハンドリングモジュールクラス");
		ErrorExit( ErMs );
		return;
	}
	// プリントモジュール
	if( pVPrint == NULL )	pVPrint = new CVPrint;
	if( pVPrint == NULL ) {
		ErMs = _T("ＤＢプリントモジュールクラス");
		ErrorExit( ErMs );
		return;
	}
	// 倍長演算
	if( pArithEx == NULL )	pArithEx = new CArithEx;
	if( pArithEx == NULL ) {
		ErMs = _T("ＤＢ倍長演算");
		ErrorExit( ErMs );
		return;
	}*/

	// ボリュームラベル
//-- '18.12.17 --
//	if( m_pZmSub->VolumeOpen() ){
//		ErrorExit( m_pZmSub->GetZmErrMsg() );
//		return -1;
//	}
//---------------
	if( m_Util.IsUnConnectMst(m_pZmSub->zvol) ){
		ChangeToolbar();
	}
	else{
		// 消費税仕訳集計
		BeginWaitCursor();
		int st = 0;
		if( m_isNonCorp ){
			st = NPSySkZanCalq( m_pZmSub );
		}
		else{
			st = DBSySkZanCalq( m_pZmSub );
		}
		if( st ){
			EndWaitCursor();
			ErrorExit( GetSsErrMsg() );
			return -1;
		}
		EndWaitCursor();
	}
	SetKessanki( m_pZmSub->zvol->ki );

	// 消費税情報のチェック
	if( CheckSyzInfo() ){
		return -1;
	}

	// 科目名称
	if( m_pZmSub->KamokuMeisyoOpen() == ERR ){
		ErrorExit( m_pZmSub->GetZmErrMsg() );
		return -1;
	}
	// 科目名称読み込み
/*	if( KnTablRead() == ERR ) {
		ErMs = _T("科目名称を読み込み中に障害が発生しました。");
		ErrorExit( ErMs );
		return;
	}*/
	// 科目残高
	if( m_pZmSub->KamokuZanOpen() == ERR ){
		ErrorExit( m_pZmSub->GetZmErrMsg() );
		return -1;
	}
	//----> 20230704 zmselで必要なためオープン
	// 科目名称ファイルリード
	if( m_pZmSub->KamokuMeisyoRead() == -1 ){
		ErrorExit( m_pZmSub->GetZmErrMsg() );
		return -1;
	}
	// 枝番名称ファイルリード
	if( m_pZmSub->EdabanOpen() == -1 ){
		ErrorExit( m_pZmSub->GetZmErrMsg() );
		return -1;
	}
	// 部門名称
	if( m_pZmSub ->BmnameOpen() == -1 ){
		ErrorExit( m_pZmSub->GetZmErrMsg() );
		return -1;
	}
	// 連想摘要
	if( m_pZmSub->RensoTekiyoOpen() == -1 ){
		ErrorExit( m_pZmSub->GetZmErrMsg() );
		return -1;
	}
	// 摘要登録
	if( m_pZmSub->KanaTekiyoOpen() == -1 ){
		ErrorExit( m_pZmSub->GetZmErrMsg() );
		return -1;
	}
	// 仕訳対応摘要
	if( m_pZmSub->SiwakeTaioTekiyoOpen() == -1 ){
		ErrorExit( m_pZmSub->GetZmErrMsg() );
		return -1;
	}
	//<---- 20230704

	// 消費税ボリュームラベル
	if( m_pZmSub->SyohizeiOpen() ){
		ErrorExit( m_pZmSub->GetZmErrMsg() );
		return -1;
	}

	// 消費税残高集計ファイル
	if( m_pZmSub->SzsykOpen("") == ERR ){
		ErrorExit( m_pZmSub->GetZmErrMsg() );
		return -1;
	}

//2016.06.22 INSERT START
	//個人決算書
	if( m_Util.IsBasicMst(m_pZmSub->zvol) ){
		CString filter;
		if( m_pZmSub->zvol->ind_type & 0x10 ){	// 白
			filter.Format( "prf_bw = 2" );
		}
		else{	// 青
			filter.Format( "prf_bw = 1" );
		}
		if( m_pZmSub->KojinKessansyoOpen( filter ) == ERR ){
			ErrorExit( m_pZmSub->GetZmErrMsg() );
			return -1;
		}
	}
//2016.06.22 INSERT END

	// 消費税残高集計：不正仕訳のチェック
	CSyzMakeSub SyzMkSub;
	int st = 0;
	if( (st=SyzMkSub.ChkSzitmInSzsyk(m_pZmSub->m_database, (m_pZmSub->zvol->s_sgn2&0x0f))) == -1 ){
		ErrorExit( SyzMkSub.GetErrorMsg() );
		return -1;
	}
	else if( st > 1 ){	// 疑問：何故　１個はOK???
		ErrorExit( _T("消費税区分の不正な仕訳があります。\n会社登録及び仕訳の消費税区分を確認してください。") );
		return -1;
	}

	// 特定収入残高読込み
	if( (m_pZmSub->zvol->apno&0xf0) == 0x50 ){			// 学校・公益・福祉・宗教
		if( (m_pZmSub->zvol->s_sgn2&0x0f) == 0x00 ){	// 原則課税
			// 特定収入残高読み込み
// 未実装！！
//			if( SpRead() == ERR ){
//				ErrorExit( _T("特定収入マスターの読み込み中に障害が発生しました。") );
//				return -1;
//			}
		}
//--> '15.04.09 INS START
		if( (m_pZmSub->zvol->s_sgn2&0x0f) != 0x00 ){
			// この条件の時には、splist テーブルの拡張が行われない為、拡張用に下記関数を通す
			if( m_pZmSub->NpTokuteiSyunyuKeisanOpen() ){
				ErrorExit( _T("特定収入マスターの読み込み中に障害が発生しました。") );
				return -1;
			}
			m_pZmSub->NpTokuteiSyunyuKeisanClose();
		}
//<-- '15.04.09 INS END
	}

	// 月情報
	if( m_pZmSub->minfo == NULL ){
		if( m_pZmSub->MinfoOpen() == ERR ){
			ErrorExit( m_pZmSub->GetZmErrMsg() );
			return -1;
		}
	}
	else{
		m_pZmSub->minfo->Requery( _T("") );
	}
	// 各月集計サインチェック
	memset( m_sgMnthExcept, '\0', sizeof(m_sgMnthExcept) );
	memset( m_sgSyzsyk, '\0', sizeof(m_sgSyzsyk) );
	for( int i=0; i<SH_MONTH_MAX; i++ ){
		if( i==0 ){
			if( m_pZmSub->minfo->MoveFirst() == ERR ){
				ErrorExit( m_pZmSub->GetZmErrMsg() );
				return -1;
			}
		}
		else{
			if( m_pZmSub->minfo->MoveNext() == ERR ){
				ErrorExit( m_pZmSub->GetZmErrMsg() );
				return -1;
			}
		}
		if( m_pZmSub->minfo->st == -1 ){
			break;
		}

		// 決算除外サイン
		m_sgMnthExcept[i] = m_pZmSub->minfo->kes_syk_cut;
		// 消費税集計サイン
		if( m_pZmSub->minfo->c_ksw&0x40 ){
			m_sgSyzsyk[i] = 0x01;
		}
		else{
			m_sgSyzsyk[i] = 0;
		}
	}

/*	この箇所は変換せずに、素のままで扱えるように！！
	// 会社番号の変換
	memset( asci, '\0', sizeof( asci ) );
	memset( CoNo, '\0', sizeof( CoNo ) );
	sprintf_s( asci, sizeof( asci ), "%08d", m_pZmSub->zvol->v_cod );
	l_pack( CoNo, asci, 8 );
	////////////////////
	// 初回　表の計算 //
	////////////////////
	// データベース会社情報をアトラス会社情報へコンバージョン
	ZaimuVolume_Cnv();
	// 合併マスターチェック
	if( Vol1.M_TYPE & 0x01 )	GpSw = 1;
	// 消費税ボリュームラベル変換
	SyzVolume_Cnv();
	memmove( &SVhead, &TBhead, sizeof( struct _SVHED ) );
*/
	// '14.02.09：月別情報を断念・・・
	CnvZvolMinfo();

	// 消費税バージョンチェック
	CString	msg;
	if( CheckSyzVersion(msg) == 0 ){
		ErrorExit( msg );
		return -1;
	}
	// バージョン保存？？
//	Ver = (N_ver & 0xff);

	// 現在の選択マスターは過年度マスタ？
//-- '18.12.17 --
//	if( ChkKanendoMaster() == -1 ){
//		ErrorExit( _T("過年度チェック中に障害が発生しました。") );
//		return -1;
//	}
//---------------
	if( ChkKndMasterInfo() == -1 ){
		ErrorExit( _T("過年度チェック中に障害が発生しました。") );
		return -1;
	}
//---------------

//--> '15.09.14 INS START
	if( SaveRelCoInfo() ){
		if( IsCompanyPackageInSyz() ){
			ErrorExit( _T("事業所情報の取得中に障害が発生しました。") );
		}
		else{
			ErrorExit( _T("顧問先情報の取得中に障害が発生しました。") );
		}
		return -1;
	}
//<-- '15.09.14 INS END

//--> '16.09.05 INS START
	// 本来は、この戻り値によってマイナンバーにアクセスしないという制御が必要になりますが、この関数をコールしておけば
	// NoTblMod8.dll 内部で自動判定して頂けるので、消費税申告書内でアクセスする前に呼び出すように修正。
	NTBMOD8_IsAbleToMyNumber( m_pZmSub->zvol->apno, 0 );
//<-- '16.09.05 INS END

	// 消費税申告書
	if( m_pZmSub->szvol->SVsasw&0x01 ){

		// ここから下は申告書データがある前提！
		// サインが立っていても申告書データが無い場合があったので、その時のエラー処理を入れておいた方が無難。。。

		// 消費税申告書（基本情報）
		CString	errmsg;
		if( OpenSzsnk(0, errmsg) == -1 ){
			ErrorExit( errmsg );
			return -1;
		}

//--> '15.07.11 INS START
		// マイナンバー取込
		m_SnHeadData.TakeInMynumber( m_pZmSub, &m_TgPDat, errmsg, NTBMOD_GYFLG_SYOHI );
//<-- '15.07.11 INS END

		// データ読込み
		m_SnHeadData.GetData( m_pZmSub, m_SnSeq );
		((CDBSyzShinMainApp*)AfxGetApp())->SetKaiseiVer( SetActViewVer(&m_SnHeadData) );

//- 未使用なのでカット -
//		if( CheckHistoryVersion() ){
//			return -1;
//		}
//----------------------

		// 総合会社情報
		if( InitCoinfo(errmsg) == 0 ){
			// 書込み？
			// 顧問先情報取り込み後、全ての保存情報に書き込む
			if( OpenSzsnk(0, errmsg) ){
				ErrorExit( errmsg );
				return -1;
			}
			
			//選択された顧問先情報を取り込む為に処理追加
			m_SnHeadData.GetData( m_pZmSub, m_SnSeq );
		}

		// 履歴対応へのバージョンアップ
//		VerUpExec();

		// 法人・個人事業者のチェック
		int Sgn1 = 0;
		if( m_Util.IsBasicMst(m_pZmSub->zvol) ){
			Sgn1 = 1;
		}
		int Sgn2 = 0;
/*- '13.12.30 -*/
//		if( Snh.Sn_KOJIN ){
//			Sgn2 = 1;
//		}
/*-------------*/
		if( m_SnHeadData.Sn_KOJIN ){
			Sgn2 = 1;
		}
/*-------------*/
		if( Sgn1 != Sgn2 ){
			m_SnHeadData.Sn_KOJIN = 0x00;
			if( ((m_pZmSub->zvol->apno&0xff)==0x10) || (m_pZmSub->zvol->ind_type) ){
				m_SnHeadData.Sn_KOJIN = 0x01;
			}
			if( CloseSzsnk(0, errmsg, FALSE) == -1 ){
				ErrorExit( errmsg );
				return 1;
			}
		}

//--> '15.03.11 INS START
		if( m_SnHeadData.Sn_Sign4&0x80 ){
			if( m_SnHeadData.IsKobetuSiireAnbun() == FALSE ){
				// 課税売上割合に準ずる割合のサインを落とす
				m_SnHeadData.Sn_UPERS &= 0xfd;
			}
		}
//<-- '15.03.11 INS END

		if ( ( m_SnHeadData.Sn_GenericSgn & 0x80 ) == 0x00 ) {
			m_SnHeadData.Sn_GenericSgn |= 0x80;
			// 2021/9リリースプロ起動サイン
			if ( m_Util.GetKazeihoushiki ( m_pZmSub->zvol ) == ID_ICSSH_KANNI ) {
				m_SnHeadData.Sn_GenericSgn |= 0x40;
			}
		}
	}
	else{	// 新規登録

		// 消費税申告書データの新規登録
		CString	errmsg;
		if( AddNewSzsnkData(errmsg) ){
			ErrorExit( errmsg );
			return -1;
		}
		m_isNewMake = TRUE;

		((CDBSyzShinMainApp*)AfxGetApp())->SetKaiseiVer( SetActViewVer(&m_SnHeadData) );

		if( ((m_pZmSub->zvol->apno&0xff)==0x10) || m_pZmSub->zvol->ind_type ){
			m_SnHeadData.Sn_KOJIN |= 0x01;
		}

		if( GetCoInfo(1) == 1 ){
			// 顧問先情報取り込み後、全ての保存情報に書き込む
			if( AdviseInfoRenew( 0, errmsg ) ){
				ErrorExit( errmsg );
				return -1;
			}
		}

		// バージョンセット
		// 10%改正対応
		//m_SnHeadData.SetSzvolHeadVersion( m_pZmSub, ICS_SH_H26_SZVOL_VER );
		CZmGen8 ZmGen;
		int ver = ZmGen.GetZgValiableVersion ( m_pZmSub->m_database );
		if ( ver == 2 ) {
			m_SnHeadData.SetSzvolHeadVersion( m_pZmSub, ICS_SH_H31_SZVOL_VER );
		}
		else {
			m_SnHeadData.SetSzvolHeadVersion( m_pZmSub, ICS_SH_H26_SZVOL_VER );
		}

		UpdateSyzShin();
	}


//--> '15.07.27 INS START
	if( GetCoInfo(3) == -1 ){
		if( IsCompanyPackageInSyz() ){
			ErrorExit( _T("事業所情報の取得に失敗しました") );
		}
		else{
			ErrorExit( _T("顧問先情報の取得に失敗しました") );
		}
		return -1;
	}
//<-- '15.07.27 INS END

	//総合会社情報の為の起動時の保存
	Copy_NowCoInfo();

//	m_UPsign = 1;
	// 税務署コードの取得
	m_isOpenEselect = FALSE;
	if( Esel_EtaxMasterOpen(m_SysPath) == 0 ){
		m_isOpenEselect = TRUE;
		//エラーになるので一旦コメント化。あとで調べるyoshida
		//下記処理コメント化したままだったので有効化190725。必要な処理。現行出荷版でも有効。エラー落ちした場合には調べるyoshida190725
		SetTxationOffice( 1 );
	}

	// 起動時の95％ルール改正対応期間チェック
	if( (m_pZmSub->zvol->s_sgn2&0x0f) < 2 ){
		Chk_H24Reform_InInit();
	}

// 不要？？？
//	// 還付申告時の付表切り替えサイン
//	if( Snd.Sn_ZeiHou&0x80 )	m_TaxRefund = 1;
//	else						m_TaxRefund = 0;

	// 更正の申出書の変換
	if( ((m_pZmSub->zvol->apno&0xff)==0x10) || (m_pZmSub->szsnk->Sn_KOJIN&0x01) ){
		// 平成23年より前の場合に許可
		if( m_pZmSub->szsnk->Sn_KDAYE >= PERMIT_KJ_AMEND_YMD ){
			m_SnHeadData.Sn_Syuusei &= 0xfb;
		}
	}
	else{
		// 平成23年10月2日より前の場合に許可
		if( m_pZmSub->szsnk->Sn_KDAYE >= PERMIT_HJ_AMEND_YMD ){
			m_SnHeadData.Sn_Syuusei &= 0xfb;
		}
	}

// 不要？？？
//	// 合併マスターチェック
//	if( Vol1.M_TYPE & 0x01 )	GpSw = 1;

/*	// 各サイン初期設定
	K221sw = 0;		// 規則２２条１項の適用積み上げ方式サイン
	H301sw = 0;		// 法第３０条第１項の適用積み上げ方式サイン
	SimpSw = 0;		// 簡易計算サイン
	GoodsSw= 0;		// 貨物税額計算サイン
	if( Vol1.S_SGN4 & 0x80 )	Anbun = 1;		// 個別対応
	else						Anbun = 0;		// 比例配分
	if( Vol1.S_SGN2 & 0x80 )	Kenei = 0x01;	// 兼業事業者
	else						Kenei = 0x00;	// 単一事業者
	Kani = (int)(Vol1.S_SGN2 & 0x0f);			// 業種区分
*/

	// 一括税抜時の仮受・仮払消費税額の取得
/*	if( IkTaxZanRead() ){
		ErrorExit( _T("仮受・仮払消費税の残高が取得できません！") );
		return -1;
	}*/

#ifdef _DEBUG
	m_Progress = 1;
	m_Finish = 1;
	sprintf_s( m_FinishPrn, sizeof( m_FinishPrn ), "[電子申告(受付)日時:2012/04/05 11:12:30 受付番号:12345678901234567890]" );
#else
	char	MDev[4]={0}, MDdm[4]={0};
	// マスターデバイス
	if( PCOM_GetString(_T("MasterDevice"), MDev, MDdm) == -1 ){
		ErrorExit( _T("マスターデバイスが取得できません。" ) );
		return -1;
	}
	m_Progress = 0;

	// 進捗処理可能チェック
	if( GetShincyokuSign( (MDev[0]&0xff) ) == 1 ){
		m_Progress = 1;
		if( m_SnHeadData.Sn_Sign2 & 0x80 ){
			m_FinishSw = 1;
		}
		else{
			m_FinishSw = 0;
		}
	}
#endif

	// 仕入控除税額に関する明細書の変換テーブルの取得
	if( GetSkjCnvtbl() ){
		return -1;
	}

	// 仕入控除税額に関する明細書(個人用)　財務連動
	if( m_Util.IsUnConnectMst(m_pZmSub->zvol) == FALSE ){
		if( (m_Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_GENSOKU) && (m_Util.IsBasicMst(m_pZmSub->zvol)) ){
			if( SkjPersonalConvert() ){
				return -1;
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// テーブル再オープン
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::ReOpenZmtbl()
{
	if( m_pZmSub->zvol ){
		m_pZmSub->VolumeClose();
	}
	if( m_pZmSub->knrec ){
		m_pZmSub->KamokuMeisyoClose();
	}
	if( m_pZmSub->kzrec ){
		m_pZmSub->KamokuZanClose();
	}
//----> 20230704
	if( m_pZmSub->pKnrec ){
		m_pZmSub->KamokuMeisyoFree();
	}
	if( m_pZmSub->ezrec ){
		m_pZmSub->EdabanClose();
	}
	if( m_pZmSub->bmname ){
		m_pZmSub->BmnameClose();
	}
	if( m_pZmSub->rtrec ){
		m_pZmSub->RensoTekiyoClose();
	}
	if( m_pZmSub->tkrec ){
		m_pZmSub->KanaTekiyoClose();
	}
	if( m_pZmSub->strec ){
		m_pZmSub->SiwakeTaioTekiyoClose();
	}
//<---- 20230704
	if( m_pZmSub->szvol ){
		m_pZmSub->SyohizeiClose();
	}
	if( m_pZmSub->szsyk ){
		m_pZmSub->SzsykClose();
	}
	if( m_pZmSub->minfo ){
		m_pZmSub->MinfoClose();
	}
//2016.06.22 INSERT START
	if( m_pZmSub->kprf ){
		m_pZmSub->KojinKessansyoClose();
	}
//2016.06.22 INSERT END

	if( m_pZmSub->VolumeOpen() ){
		ErrorExit( m_pZmSub->GetZmErrMsg() );
		return -1;
	}
	if( m_Util.IsUnConnectMst(m_pZmSub->zvol) ){
		ChangeToolbar();
	}
	else{
		// 消費税仕訳集計
		BeginWaitCursor();
		int st = 0;
		if( m_isNonCorp ){
			st = NPSySkZanCalq( m_pZmSub );
		}
		else{
			st = DBSySkZanCalq( m_pZmSub );
		}
		if( st ){
			EndWaitCursor();
			ErrorExit( GetSsErrMsg() );
			return -1;
		}
		EndWaitCursor();
	}
	if( m_pZmSub->VolumeOpen() ){
		ErrorExit( m_pZmSub->GetZmErrMsg() );
		return -1;
	}
	SetKessanki( m_pZmSub->zvol->ki );

	// 消費税情報のチェック
	if( CheckSyzInfo() ){
		return -1;
	}

	// 科目名称
	if( m_pZmSub->KamokuMeisyoOpen() == ERR ){
		ErrorExit( m_pZmSub->GetZmErrMsg() );
		return -1;
	}
	// 科目名称読み込み
/*	if( KnTablRead() == ERR ) {
		ErMs = _T("科目名称を読み込み中に障害が発生しました。");
		ErrorExit( ErMs );
		return;
	}*/
	// 科目残高
	if( m_pZmSub->KamokuZanOpen() == ERR ){
		ErrorExit( m_pZmSub->GetZmErrMsg() );
		return -1;
	}
	//----> 20230704 zmselで必要なためオープン
	// 科目名称ファイルリード
	if( m_pZmSub->KamokuMeisyoRead() == -1 ){
		ErrorExit( m_pZmSub->GetZmErrMsg() );
		return -1;
	}
	// 枝番名称ファイルリード
	if( m_pZmSub->EdabanOpen() == -1 ){
		ErrorExit( m_pZmSub->GetZmErrMsg() );
		return -1;
	}
	// 部門名称
	if( m_pZmSub ->BmnameOpen() == -1 ){
		ErrorExit( m_pZmSub->GetZmErrMsg() );
		return -1;
	}
	// 連想摘要
	if( m_pZmSub->RensoTekiyoOpen() == -1 ){
		ErrorExit( m_pZmSub->GetZmErrMsg() );
		return -1;
	}
	// 摘要登録
	if( m_pZmSub->KanaTekiyoOpen() == -1 ){
		ErrorExit( m_pZmSub->GetZmErrMsg() );
		return -1;
	}
	// 仕訳対応摘要
	if( m_pZmSub->SiwakeTaioTekiyoOpen() == -1 ){
		ErrorExit( m_pZmSub->GetZmErrMsg() );
		return -1;
	}
	//<---- 20230704

	// 消費税ボリュームラベル
	if( m_pZmSub->SyohizeiOpen() ){
		ErrorExit( m_pZmSub->GetZmErrMsg() );
		return -1;
	}

	// 消費税残高集計ファイル
	if( m_pZmSub->SzsykOpen("") == ERR ){
		ErrorExit( m_pZmSub->GetZmErrMsg() );
		return -1;
	}

//2016.06.22 INSERT START
	//個人決算書
	CString filter;
	if( m_pZmSub->zvol->ind_type & 0x10 ){	// 白
		filter.Format( "prf_bw = 2" );
	}
	else{	// 青
		filter.Format( "prf_bw = 1" );
	}
	if( m_pZmSub->KojinKessansyoOpen( filter ) == ERR ){
		ErrorExit( m_pZmSub->GetZmErrMsg() );
		return -1;
	}
//2016.06.22 INSERT END

	// 消費税残高集計：不正仕訳のチェック
	CSyzMakeSub SyzMkSub;
	int st = 0;
	if( (st=SyzMkSub.ChkSzitmInSzsyk(m_pZmSub->m_database, (m_pZmSub->zvol->s_sgn2&0x0f))) == -1 ){
		ErrorExit( SyzMkSub.GetErrorMsg() );
		return -1;
	}
	else if( st > 1 ){	// 疑問：何故　１個はOK???
		ErrorExit( _T("消費税区分の不正な仕訳があります。\n会社登録及び仕訳の消費税区分を確認してください。") );
		return -1;
	}

	// 特定収入残高読込み
	if( (m_pZmSub->zvol->apno&0xf0) == 0x50 ){			// 学校・公益・福祉・宗教
		if( (m_pZmSub->zvol->s_sgn2&0x0f) == 0x00 ){	// 原則課税
			// 特定収入残高読み込み
// 未実装！！
//			if( SpRead() == ERR ){
//				ErrorExit( _T("特定収入マスターの読み込み中に障害が発生しました。") );
//				return -1;
//			}
		}
	}

	// 月情報
	if( m_pZmSub->minfo == NULL ){
		if( m_pZmSub->MinfoOpen() == ERR ){
			ErrorExit( m_pZmSub->GetZmErrMsg() );
			return -1;
		}
	}
	else{
		m_pZmSub->minfo->Requery( _T("") );
	}
	// 各月集計サインチェック
	memset( m_sgMnthExcept, '\0', sizeof(m_sgMnthExcept) );
	memset( m_sgSyzsyk, '\0', sizeof(m_sgSyzsyk) );
	for( int i=0; i<SH_MONTH_MAX; i++ ){
		if( i==0 ){
			if( m_pZmSub->minfo->MoveFirst() == ERR ){
				ErrorExit( m_pZmSub->GetZmErrMsg() );
				return -1;
			}
		}
		else{
			if( m_pZmSub->minfo->MoveNext() == ERR ){
				ErrorExit( m_pZmSub->GetZmErrMsg() );
				return -1;
			}
		}
		if( m_pZmSub->minfo->st == -1 ){
			break;
		}

		// 決算除外サイン
		m_sgMnthExcept[i] = m_pZmSub->minfo->kes_syk_cut;
		// 消費税集計サイン
		if( m_pZmSub->minfo->c_ksw&0x40 ){
			m_sgSyzsyk[i] = 0x01;
		}
		else{
			m_sgSyzsyk[i] = 0;
		}
	}

/*	この箇所は変換せずに、素のままで扱えるように！！
	// 会社番号の変換
	memset( asci, '\0', sizeof( asci ) );
	memset( CoNo, '\0', sizeof( CoNo ) );
	sprintf_s( asci, sizeof( asci ), "%08d", m_pZmSub->zvol->v_cod );
	l_pack( CoNo, asci, 8 );
	////////////////////
	// 初回　表の計算 //
	////////////////////
	// データベース会社情報をアトラス会社情報へコンバージョン
	ZaimuVolume_Cnv();
	// 合併マスターチェック
	if( Vol1.M_TYPE & 0x01 )	GpSw = 1;
	// 消費税ボリュームラベル変換
	SyzVolume_Cnv();
	memmove( &SVhead, &TBhead, sizeof( struct _SVHED ) );
*/
	// '14.02.09：月別情報を断念・・・
	CnvZvolMinfo();

	// 集計クラス再初期化
	if( m_pSzzanList ){
		if( m_pSzzanList->initialize(m_pZmSub) ){
			return -1;
		}
	}

	// 外部集計
	if( m_pSyzSyukei ){
		if( m_pSyzSyukei->initialize(m_pSzzanList) ){
			return -1;
		}
	}
	// 月別情報を全体に開きなおす...
	m_pZmSub->minfo->Requery( _T("") );

	return 0;
}

//-----------------------------------------------------------------------------
// 消費税情報のチェック
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::CheckSyzInfo()
{
	// 課税方式チェック
	if( (m_pZmSub->zvol->s_sgn2&0x0f) == 0x01 ){
		ErrorExit( _T("課税方式が免税の場合は処理できません") );
		return -1;
	}

	// 一括税抜きマスターチェック
	m_sgIkat = 0x00;
	m_sgIkat = (m_pZmSub->zvol->s_sgn1&0xc0);

	// 課税取引金額計算表 個人決算書対応マスターチェック
	m_sgBasic = 0x00;
	if( ((m_pZmSub->zvol->apno&0xff)==0x10) || m_pZmSub->zvol->ind_type ){
		m_sgBasic = 0x01;
	}

	// 特定収入マスターチェック
//-- '15.02.11 --
/*	m_sgSpc = 0;
	//shimizu
	if( (m_pZmSub->zvol->apno&0xf0) == 0x50 ){			// 学校・公益・福祉・宗教
		if( (m_pZmSub->zvol->s_sgn2&0x0f) == 0x00 ){	// 原則課税
			// 特定収入
			if( m_pZmSub->NpTokuteiSyunyuOpen(_T("")) == ERR ){
				ErrorExit( m_pZmSub->GetZmErrMsg() );
				return -1;
			}
			m_Specnt = m_pZmSub->m_pSpsyk->GetRecordCountEx();
			m_sgSpc = 1;

			// 特定収入計算表
			if( m_pZmSub->NpTokuteiSyunyuKeisanOpen(_T("spseq = 0")) == ERR ){
				ErrorExit( m_pZmSub->GetZmErrMsg() );
				return -1;
			}
			m_SplistCnt = m_pZmSub->m_pSplist->GetRecordCountEx();
		}
	}
	m_sgSpc = 0;
*/
//---------------
	m_sgSpc = 0;
	if( m_Util.IsSpcMaster(m_pZmSub) ){
/*		// 特定収入
		if( m_pZmSub->NpTokuteiSyunyuOpen(_T("")) == ERR ){
			ErrorExit( m_pZmSub->GetZmErrMsg() );
			return -1;
		}
		m_Specnt = m_pZmSub->m_pSpsyk->GetRecordCountEx();

		// 特定収入計算表
		if( m_pZmSub->NpTokuteiSyunyuKeisanOpen(_T("spseq = 0")) == ERR ){
			ErrorExit( m_pZmSub->GetZmErrMsg() );
			return -1;
		}
		m_SplistCnt = m_pZmSub->m_pSplist->GetRecordCountEx();*/

		m_sgSpc = 1;
	}
//---------------

	return 0;
}

//-----------------------------------------------------------------------------
// 消費税関連後処理
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CMainFrame::SyzEndJob()
{
// 24/06/17_ScoreLink追加対応 cor -->
	//// 最終データ書込み
	//m_SnHeadData.SetData( m_pZmSub, m_SnSeq );
// ----------------------------------
	// 最終データ書込み
	if(m_isScoreLinkCall == -1)	{
		m_SnHeadData.SetData( m_pZmSub, m_SnSeq );
	}
// 24/06/17_ScoreLink追加対応 cor <--

	// 税務署コードの取得の後始末
	if( m_isOpenEselect ){
		Esel_EtaxMasterClose();
		m_isOpenEselect = FALSE;
	}

	// 仕入控除用テーブル
	if( m_SkjCnvtbl.tp_tno != -1 ){
		m_Tblhdl.th_close( m_SkjCnvtbl.tp_tno );
		memset( &m_SkjCnvtbl, '\0', sizeof(_TB_PAR) );
		m_SkjCnvtbl.tp_tno = -1;
	}

	// 集計後始末
	if( m_pSyzSyukei ){
		m_pSyzSyukei->finalize();
		delete m_pSyzSyukei;
		m_pSyzSyukei = NULL;
	}
	if( m_pSzzanList ){
		m_pSzzanList->finalize();
		delete m_pSzzanList;
		m_pSzzanList = NULL;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// テーブルクローズ
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::CloseZmtbl()
{
	if( m_pZmSub == NULL ){
		return 0;
	}

//--> '15.03.18 INS START
	if( m_pZmSub->zvol ){
		if( (m_pZmSub->zvol->apno&0xf0) > 0x50 ){
			if( m_pZmSub->m_pSpsyk ){
				m_pZmSub->NpTokuteiSyunyuClose();
			}
			//if( m_pZmSub->m_pSpsyk ){
			if( m_pZmSub->m_pSplist ){
				m_pZmSub->NpTokuteiSyunyuKeisanClose();
			}
		}
	}
//<-- '15.03.18 INS END

	if( m_pZmSub->zvol ){
		m_pZmSub->VolumeClose();
	}

	if( m_pZmSub->knrec ){
		m_pZmSub->KamokuMeisyoClose();
	}

	if( m_pZmSub->kzrec ){
		m_pZmSub->KamokuZanClose();
	}

//----> 20230704
	if( m_pZmSub->pKnrec ){
		m_pZmSub->KamokuMeisyoFree();
	}

	if( m_pZmSub->ezrec ){
		m_pZmSub->EdabanClose();
	}

	if( m_pZmSub->bmname ){
		m_pZmSub->BmnameClose();
	}

	if( m_pZmSub->rtrec ){
		m_pZmSub->RensoTekiyoClose();
	}

	if( m_pZmSub->tkrec ){
		m_pZmSub->KanaTekiyoClose();
	}

	if( m_pZmSub->strec ){
		m_pZmSub->SiwakeTaioTekiyoClose();
	}
//<---- 20230704

	if( m_pZmSub->szsyk ){
		m_pZmSub->SzsykClose();
	}

	if( m_pZmSub->szvol ){
		m_pZmSub->SyohizeiClose();
	}

	if( m_pZmSub->szvol ){
		m_pZmSub->SyohizeiClose();
	}

	if( m_pZmSub->minfo ){
		m_pZmSub->MinfoClose();
	}

//2016.06.22 INSERT START
	if( m_pZmSub->kprf ){
		m_pZmSub->KojinKessansyoClose();
	}
//2016.06.22 INSERT END
	
	if( m_pZmSub->szsnk ){
		m_pZmSub->SzsnkClose();
	}

	if( m_pZmSub ){
		delete m_pZmSub;
		m_pZmSub = NULL;
	}

	return 0;
}


LRESULT	CMainFrame::OnMessageClose( WPARAM w, LPARAM l )
{
	MoveKanpuFocus();

// 改良依頼No.18-0686 add -->
	CDlgJobEnd dlgjobend(this);
	CDlgJobEnd2 dlgjobend2(this);
	int locksw = 0;
// 改良依頼No.18-0686 add <--

	//WPARAMが何れの値であっても、クローズ処理などは必要です。
	//下記分岐が必要なのは、正常終了メッセージや、エラーメッセージを表示する等です。
	//エラー終了時に特段の処理が無い場合は、０とそれ以外という分岐で構いません。
	if( GetPreviewMode() ){
		if( m_pMainView ){
			m_pMainView->SetOptionMode();
		}
		return 1;
	}

//2018.03.13 INSERT START
	if( Message_MyNumber_error() == -1 ){
		SetEndStartFlg( FALSE );
		return 1;
	}
	if( Message_MyNumber_overlap() == -1 ){
		SetEndStartFlg( FALSE );
		return 1;
	}
	EditOff_Fuhyo6();
	if( Message_SozokuWariai() == -1 ){
		SetEndStartFlg( FALSE );
		return 1;
	}
	if( Message_NofuKanpuGokei() == IDYES ){
		SetEndStartFlg( FALSE );
		return 1;
	}
//2018.03.13 INSERT END

	CheckInvno( 3 );

//	m_isEndStart = TRUE;
	SetEndStartFlg( TRUE );

	if( (w==100) && (m_swFork==2) ){
//		m_isEndStart = FALSE;	// 終了処理キャンセル
		SetEndStartFlg( FALSE );
		return 1;
	}
	else if( ((w==0)||(w==100)) && (m_swFork!=1) ){
		// 正常終了
		// 終了ボタンやENDキー等、通常のWM_CLOSE

//--> '16.01.07 INS START
		bool isYotei = false;
		if( m_SnHeadData.IsMiddleProvisional() ){
			isYotei = true;
		}
//<-- '16.01.07 INS END

//--> '15.09.09 INS START
		// 終了可能？
		if( m_isNeedChkTkkz ){
			CBaseHyoView *pBaseView = GetHonHyoView();
			if( pBaseView != NULL ){
				if( pBaseView->ChkTkkzAndKzwari(m_nowTabIdx, 3) ){
//					m_isEndStart = FALSE;	// 終了処理キャンセル
					SetEndStartFlg( FALSE );
					return 1;
				}
			}
		}
//<-- '15.09.09 INS END

//--> '16.12.29 CUT START
//		if( m_swBeForked < ID_NORMALPRINT_BPSTATE ){
//			// 会社情報へ書き込み
//			SetCoInformation();
//		}
//<-- '16.12.29 CUT END

//--> '15.01.27 INS START
		if( m_nowTabIdx == 1 ){
			// 終了用のフォーカス移動
			CBaseHyoView *pBaseView = GetHonHyoView();
			if( pBaseView != NULL ){
				pBaseView->MoveFocusInEnd();
			}
		}
//---> '15.09.07 INS START
		else if( m_nowTabIdx == 0 ){
			if( (m_SnSeq>0) && ((m_SnHeadData.Sn_Sign4&0x01)==0x00) ){
				if( m_pMainView ){
					m_pMainView->EndJobFocus();
				}
			}
		}
//<--- '15.09.07 INS END


//2016.06.22 INSERT START
		//還付を受けようとする金融機関等
		int erst=0;
		if( Message_BlankKinyu(erst) == IDYES ){
			// 還付金融機関の入力へ
			GuideKanpuKinyu(erst);
//			m_isEndStart = FALSE;	// 終了処理キャンセル
			SetEndStartFlg( FALSE );
			return 1;
		}
//2016.06.22 INSERT END

		if( (m_nowTabIdx!=0) && (m_SnSeq==0) && ((m_SnHeadData.Sn_Sign4&0x01)==0x00) ){
			if( (m_SnHeadData.Sn_TAXNO[0]=='F') || (m_SnHeadData.Sn_TAXNO[0]=='\0') ){
				CString	msg;
				msg = _T("提出先税務署名が未入力か、または署番号が決定されていません。");
				msg += _T("\n電子申告には、署番号の入力が必須です。");
				msg += _T("\n税務署を入力しますか？");
				if( ICSMessageBox(msg, MB_YESNO, 0) == IDYES ){
					// 提出先税務署名の入力へ
					GuideTaxOfficeInput();
//					m_isEndStart = FALSE;	// 終了処理キャンセル
					SetEndStartFlg( FALSE );
//--> '15.11.17 INS START
					m_isNeedChkTkkz = TRUE;
//<-- '15.11.17 INS END
					return 1;
				}
			}
		}
//<-- '15.01.27 INS END

//2017.12.04 INSERT START
		if( Message_BlankKijunKazei() == IDYES ){
//			m_isEndStart = FALSE;
			SetEndStartFlg( FALSE );
			return 1;
		}

		if( Message_BlankSoneki() == IDYES ){
//			m_isEndStart = FALSE;
			SetEndStartFlg( FALSE );
			return 1;
		}
//2017.12.04 INSERT END

		if ( Message_H31ManualInputSign() == -1 ) {
			SetEndStartFlg ( FALSE );
			return 1;
		}

//--> '16.01.27 INS START
		// 終了確認
		char	tmpBuf[128]={0};
		memmove( tmpBuf, m_SnHeadData.Sn_DIADR, sizeof(m_SnHeadData.Sn_DIADR) );
		if( m_SnHeadData.IsSoleProprietor() == FALSE ){
			if( m_Util.MixSpaceCutLength(tmpBuf, 80) == 0 ){
				CString	msg;
				msg = _T("代表者住所が入力されていません。");
				msg += _T("\n電子申告には、代表者住所の入力が必須です。");
				msg += _T("\n基本情報タブの電子申告情報で代表者住所を入力してください。");
				if( ICSMessageBox(msg, MB_YESNO, 0) == IDYES ){
//					m_isEndStart = FALSE;	// 終了処理キャンセル
					SetEndStartFlg( FALSE );
					m_isNeedChkTkkz = TRUE;
					return 1;
				}
			}
		}
//<-- '16.01.27 INS END

//--> '16.12.29 INS START
		if( m_swBeForked < ID_NORMALPRINT_BPSTATE ){
			// 会社情報へ書き込み
			SetCoInformation();
//-- '18.12.17 --
			if( m_pCoinfCtl ){
				m_pCoinfCtl->CloseFile();
				delete m_pCoinfCtl;
				m_pCoinfCtl = NULL;
			}
//---------------
		}
//<-- '16.12.29 INS END

		if( !m_swBeForked ){
			if( m_Progress && (w==0) ){
				ProgressEnd();
			}

			// 改良依頼No.18-0686 add -->
			CString		Title, AppTitle, str;
			int			status;
			AppTitle.LoadString( AFX_IDS_APP_TITLE );
			CBaseHyoView *pBaseView = GetHonHyoView();

			locksw = 0;
			// 入力ロックON or ビューが消費税10％改正前 or 新規作成
			if( m_SnHeadData.Sn_Sign4&0x01 || GetActViewVer() == ID_VER_SYZ26 || (m_pZmSub->szvol->SVsasw&0x01) == 0x00){
				// 改良依頼No.18-0686 add <--
				if( m_Util.IsUnConnectMst(m_pZmSub->zvol) ){
					// 改良依頼No.18-0686 del -->
					//CString		Title, AppTitle, str;
					//int			status;
					// 改良依頼No.18-0686 del <--
					Title  = _T("\n 入力処理を終了します。");
					Title += _T("\n 他の会社の処理を行いますか？");
					Title += _T("\n （［いいえ］を押すと、業務を終了します。）");
					AppTitle.LoadString( AFX_IDS_APP_TITLE );
					status = ICSExtBox( AppTitle, Title, _T("はい(&Y)"), _T("いいえ(&N)"),
						_T(" 入力に戻る(&R) "), MB_ICONQUESTION | MB_DEFBUTTON2 );
					if( status == IDINVALID_ICS ){
						//						m_isEndStart = FALSE;	// 終了処理キャンセル ('14.03.13)
						SetEndStartFlg( FALSE );
						//--> '15.11.17 INS START
						m_isNeedChkTkkz = TRUE;
						//<-- '15.11.17 INS END
						return 1;
					}
					// 入力情報更新（後始末）
					if( status == IDYES ){
						UCOM_SetString_BC( _T("終了プログラム"), _T("会社選択") );
					}
					else{
						UCOM_SetString_BC( _T("終了プログラム"), _T("処理終了") );
					}
				}
				else{
					if( ICSJobEndBox( _T("消費税申告書を終了してもよろしいですか？") ) != IDYES ){
						//--> '15.04.13 INS START
						SetFocusToInputPos();
						//<-- '15.04.13 INS END
						//						m_isEndStart = FALSE;	// 終了処理キャンセル ('14.03.13)
						SetEndStartFlg( FALSE );
						//--> '15.11.17 INS START
						m_isNeedChkTkkz = TRUE;
						//<-- '15.11.17 INS END
						return 1;
					}
				}
				// 改良依頼No.18-0686 add -->
			}
			// 入力ロックOFF
			else {
				if( m_Util.IsUnConnectMst(m_pZmSub->zvol) ){
					Title  = _T("入力処理を終了します。");
					Title += _T("\n他の会社の処理を行いますか？");
					Title += _T("\n（［いいえ］を押すと、業務を終了します。）");
					// AppTitle,Title以外の引数は現在未使用
					status = dlgjobend2.ShowDialog(AppTitle,Title,_T("入力ロック(&Y)"),_T("いいえ(&N)") ,_T("入力に戻る(&R)"),_T("はい(&E)"), MB_ICONQUESTION | MB_DEFBUTTON2,0,0,this);
					switch(status){
					case IDYES:						// 入力ロックを行い、処理を終了する
						locksw = 1;
						UCOM_SetString_BC( _T("終了プログラム"), _T("処理終了") );
						break;
					case IDNO:						// 処理を終了する
						UCOM_SetString_BC( _T("終了プログラム"), _T("処理終了") );
						break;
					case 101:						// 会社入力画面に戻る
						UCOM_SetString_BC( _T("終了プログラム"), _T("会社選択") );
						break;
					default:						// 終了処理をキャンセル
						SetFocusToInputPos();
						SetEndStartFlg( FALSE );
						m_isNeedChkTkkz = TRUE;
						return 1;
					}
				}
				else {
					Title  = _T("消費税申告書を終了してもよろしいですか？");
					// AppTitle,Title以外の引数は現在未使用
					status = dlgjobend.ShowDialog(AppTitle,Title,_T("入力ロック(&Y)"),_T("処理終了(&E)"),_T("処理に戻る(&N)"), MB_ICONQUESTION | MB_DEFBUTTON2,0,0,this);
					switch(status){
					case IDYES:						// 入力ロックを行い、処理を終了する
						locksw = 1;
						break;
					case IDNO:						// 処理を終了する
						break;
					default:						// 終了処理をキャンセル
						SetFocusToInputPos();
						SetEndStartFlg( FALSE );
						m_isNeedChkTkkz = TRUE;
						return 1;
					}
				}
			}
			// 改良依頼No.18-0686 add <--
		}
		else{
			if( m_swBeForked == ID_ESTABLISH_BPSTATE ){
				CString	str[6];
				// 依頼キー
				UCOM_SetString_BC( _T("依頼キー"), m_BpPrintInfo.ForkKey );
				// 用紙サイズ
				str[0] = _T( "PC_PAPERA4" );
				// 用紙方向
				str[1] = _T( "PORTRAIT" );
				// 用紙サイズ
				UCOM_SetString_BC( _T("用紙サイズ"), str[0].GetBuffer(), str[1].GetBuffer() );
				str[0].ReleaseBuffer();
				str[1].ReleaseBuffer();
				// 設定年月日
				str[0] = _T("-1");
				str[1] = _T("-1");
				str[2] = _T("-1");
				str[3] = _T("-1");
				UCOM_SetString_BC( _T("設定年月日"), str[0].GetBuffer(), str[1].GetBuffer(), str[2].GetBuffer(), str[3].GetBuffer(), _T("0") );
				str[0].ReleaseBuffer();
				str[1].ReleaseBuffer();
				str[2].ReleaseBuffer();
				str[3].ReleaseBuffer();
				// 詳細設定
				if( m_SnHeadData.IsMiddleProvisional() ){
					str[0] = _T("消費税の中間申告(26号様式)");
				}
				else{
					str[0] = _T("消費税申告書");
				}
				UCOM_SetString_BC( _T("設定詳細"), str[0].GetBuffer() );
				str[0].ReleaseBuffer();
			}
			else if( (m_swBeForked==ID_NORMALPRINT_BPSTATE) || (m_swBeForked==ID_PDFPRINT_BPSTATE) ){	// 印刷
				// 依頼キー
				UCOM_SetString_BC( _T("依頼キー"), m_BpPrintInfo.ForkKey );
				// 印刷状態
				UCOM_SetString_BC( _T("印刷状態"), ( m_swForkErr ? _T("-1"): _T("0")) );
			}
		}

//--> '15.12.23 INS START
		if( m_isHirendoCall ){
			m_isKeyoff = TRUE;
		}
//<-- '15.12.23 INS END

		// 表示データの取得
		GetDispHyoData();

		// 全データの更新
//		((CDBSyzShinView *)HomeView)->AllUpData();
		if( m_pMainView ){
//			m_pMainView->EndJob(m_swFork, m_swBeForked, m_usertbl, m_GrPath );
			m_pMainView->EndJob(m_swBeForked, m_swForkErr, m_usertbl, m_GrPath );
		}

		// ビュー内部からフォーカスを外す！
//		m_MainTab.SetFocus();
//--' 15.07.25 --
//		EndViewFocus();
//---------------
		if( m_nowTabIdx > 0 ){
			EndViewFocus();
		}
//---------------

		EndOtherView();

// 改良依頼No.18-0686 add -->
		if(locksw == 1) {
			// 入力ロックのサインをONにする
			m_SnHeadData.Sn_Sign4 |= 0x01;
			if( m_pMainView ){
				m_pMainView->SendMessage(WM_USER_UNLOCK, (WPARAM)0);
			}
		}
// 改良依頼No.18-0686 add <--

		SyzEndJob();

		CloseZmtbl();

//--> '16.01.05 INS START
		if( m_pMainView ){
			m_pMainView->FinalizeMyno( isYotei );
		}
//<-- '16.01.05 INS START
	}
	else if( w == 1 ) {
		//エラー終了
		//ICSExit()等の強制終了関数で、エラーとしてのWM_CLOSE

//--> '15.03.18 INS START
/*		if( m_pMainView ){
			m_pMainView->EndJob(m_swBeForked, m_swForkErr, m_usertbl, m_GrPath );
		}
		// ビュー内部からフォーカスを外す！
		EndViewFocus();

		EndOtherView();

		SyzEndJob();*/

		CloseZmtbl();
//<-- '15.03.18 INS END

//--> '16.01.05 INS START
		if( m_pMainView ){
			m_pMainView->FinalizeMyno( false );
		}
//<-- '16.01.05 INS START
	}
	else if( w == 0x2000 ){
		CloseZmtbl();
//--> '16.01.05 INS START
		if( m_pMainView ){
			m_pMainView->FinalizeMyno( false );
		}
//<-- '16.01.05 INS START
	}
	else {
		//それ以外
		//モジュールからの一括終了等、特殊用途のWM_CLOSE
		//100:一括終了
	}

	//m_isEndStart = TRUE;
	
	return	ICSMainFrame::OnMessageClose( w, l );

}

int CMainFrame::TabLButtonDown()
{
	ICSMainFrame::TabLButtonDown();

	int no = m_wndMDITabWindow.IndexLClick();
	if( m_wndMDITabWindow.IndexLClick() ){
		int		index;
		BOOL	min;
		CWnd	*pWnd = GetActiveChild( &index, &min );

		if( (m_SnHeadData.Sn_SKKBN==0x02) && (m_SnHeadData.Sn_Syuusei&0x08) && (m_wndMDITabWindow.IndexLClick()==2) ){
			ICSMessageBox( _T("予定（26号）選択時は処理できません。") );
			SetActiveChild( index );
			return TRUE;
		}
////2018.03.13 INSERT START
		if( no != 1 ){
			if( Message_MyNumber_error() == -1 ){
				SetActiveChild( 1 );
				return TRUE;
			}
			if( Message_MyNumber_overlap() == -1 ){
				SetActiveChild( 1 );
				return TRUE;
			}
			EditOff_Fuhyo6();
			if( Message_SozokuWariai() == -1 ){
				SetActiveChild( 1 );
				return TRUE;
			}
		}
////2018.03.13 INSERT END
		if( m_pMainView != NULL ){
//2016.02.23 INSERT START
			m_pMainView->Static_Update( m_SnHeadData.Sn_Sign4 );
//2016.02.23 INSERT END
		//	if(( index != 1 ) && ( index != 2 )){ // 基本タブからの移動のみ
			if( index == 0 ){ // 基本タブからの移動のみ
				if( m_wndMDITabWindow.IndexLClick() != index ){
//					unsigned char sv_ver = m_nowTabver;
					int	sv_Kver = GetKaiseiVer();
					if( !m_pMainView->SyzEdit() ){
						m_pMainView->RedrawGroupText();
						SetActiveChild( 0 );
						return TRUE;
					}
					else{
						if( m_wndMDITabWindow.IndexLClick() != 0 ){
							if( m_pMainView->ChkCallProgramType() == 0 ){
								// 過年度プロ
	#ifdef _DEBUG
								AfxMessageBox( _T("過年度プロ呼出") );
	#else
								if( m_pMainView->IsHistoryMode() ){
									CallSyzHistoryProgram( m_pMainView->GetHistoryCurSel()+1 );
								}
								else{
									CallSyzHistoryProgram( 0 );
								}
	#endif
								SetActiveChild( 0 );
								return TRUE;

							}
						}

//						if( sv_ver != m_nowTabver ) {
						if ( sv_Kver != GetKaiseiVer() ) {
							// タブが再作成されているので自前でセット
							SetActiveChild(1);
							return TRUE;
						}
					}
				}
			}
		}
	}

////2018.03.13 INSERT START
	//基本情報タブをクリック
	if( no != 1 ){
		if( Message_MyNumber_error() == -1 ){
			SetActiveChild( 1 );
			return TRUE;
		}
		if( Message_MyNumber_overlap() == -1 ){
			SetActiveChild( 1 );
			return TRUE;
		}
		EditOff_Fuhyo6();
		if( Message_SozokuWariai() == -1 ){
			SetActiveChild( 1 );
			return TRUE;
		}
		//if( Message_NofuKanpuGokei() == IDYES ){
		//	SetActiveChild( index );
		//	return TRUE;
		//}

		CheckInvno( 2 );
	}
////2018.03.13 INSERT END
	return	0;
}

int CMainFrame::TabRButtonDown()
{
	ICSMainFrame::TabRButtonDown();
	return	0;
}

void CMainFrame::ChildFrameActive( CWnd* pChild )
{
	ICSMainFrame::ChildFrameActive( pChild );

	if( m_isEndStart ){
		return;
	}
	if( m_pMainView == NULL ){
		return;
	}
	if( m_swFork ){
		return;
	}
	if( m_isInitEnd == FALSE ){
		return;
	}

	int		index;
	BOOL	min;
	CWnd	*pWnd = GetActiveChild( &index, &min );

	if( index == m_nowTabIdx ){
		return;
	}

	ICSToolBar* pBar = GetToolBar( 1 );
	if( index == 0 ){
		pBar = GetToolBar( 0 );
	}
	else if( index == 1 ){
		pBar = GetToolBar( 1 );
	}
	else if( index == 2 ){
		pBar = GetToolBar( 2 );
	}

	// 他のビュー
	CBaseHyoView *pBaseView = GetHonHyoView();

	//  課税取引金額計算表'14.08.05
	//CTaxationListView *pTaxationListView = (CTaxationListView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd( 3 ));
//	CTaxationListView *pTaxationListView = NULL;	// 31テスト
	CBaseTaxHyoView *pBaseTaxView = GetTaxListView();


	switch( index ){
		case 0 :	// 基本画面
			if( m_nowTabIdx > 0 ){
				if( pBaseView ){
//--> '15.04.30 INS START
					// 10%改正対応
					//if ( GetKaiseiVer() == ID_VER_SYZ31 ) {
					if ( GetActViewVer() == ID_VER_SYZ31 ) {
						AfxSetResourceHandle( GetH31HyoInstance() );
					}
					else {
						AfxSetResourceHandle( GetH26HyoInstance() );
					}
//<-- '15.04.30 INS END
					pBaseView->UpdateAllSheet();
					pBaseView->WriteData( FALSE, TRUE );
//--> '15.04.30 INS START
					AfxSetResourceHandle( m_MainInstance );
//<-- '15.04.30 INS END
					m_pMainView->curset.SetSnh( m_SnHeadData );//shimizu
				}

				//課税取引金額計算表
//-- '15.03.20 --
//				if( pTaxationListView ){
//					pTaxationListView->WriteData( FALSE );
//				}
//---------------
				if( m_nowTabmax > 2 ){
//					if( pTaxationListView ){
//						pTaxationListView->WriteData( FALSE );
//					}
					if( pBaseTaxView ){
											// 10%改正対応
						//if ( GetKaiseiVer() == ID_VER_SYZ31 ) {
						if ( GetActViewVer() == ID_VER_SYZ31 ) {
							AfxSetResourceHandle( GetTaxationList31Instance() );
						}
						else {
							AfxSetResourceHandle( GetTaxationListInstance() );
						}
					//	AfxSetResourceHandle( GetTaxationListInstance() );
						//pBaseTaxView->UpdateAllSheet();
						//pBaseTaxView->WriteData( FALSE, TRUE );
						pBaseTaxView->WriteData( FALSE );
						AfxSetResourceHandle( m_MainInstance );
						//m_pMainView->curset.SetSnh( m_SnHeadData );//shimizu
					}
				}
//---------------
			}

			AfxSetResourceHandle( m_MainInstance );

			if( m_pMainView ){
				m_pMainView->InitDisp();
			}

//--> '15.07.25 INS START
			if( m_nowTabIdx > 0 ){
				if( m_pMainView ){
					m_pMainView->InitMynumberControl();
				}
			}
//<-- '15.07.25 INS END

			UpdateLockIndicator( FALSE );

			m_nowTabIdx = index;

			PostMessage( WM_USER_SETFOCUS, 0 );	// 「入力」へ harino ('14.03.09)
			break;

		case 1 :	// 本表 入力

//--> '15.02.03 INS START
			m_isShownHonpyo = TRUE;
//--> '15.02.03 INS END

			UpdateLockIndicator( m_SnHeadData.Sn_Sign4&0x01 );

			// リソースの切替でうまく事が運びそうだが保留
//			AfxSetResourceHandle( GetH26HyoInstance() );

			//課税取引金額計算表
//-- '15.03.20 --
//			if( pTaxationListView ){
//				pTaxationListView->WriteData( FALSE );
//
//				pTaxationListView->SetDispedFlg( TRUE );
//				pTaxationListView->ReInitDisp( m_isCompChange );
//			}
//---------------
			if( m_nowTabmax > 2 ){
				if( pBaseTaxView ){
//--> '15.04.30 INS START
					// 10%改正対応
					//if ( GetKaiseiVer() == ID_VER_SYZ31 ) {
					if ( GetActViewVer() == ID_VER_SYZ31 ) {
						AfxSetResourceHandle( GetTaxationList31Instance() );
					}
					else {
						AfxSetResourceHandle( GetTaxationListInstance() );
					}
				//	AfxSetResourceHandle( GetTaxationListInstance() );
//<-- '15.04.30 INS END
					//pBaseTaxView->WriteData( FALSE , TRUE );
					pBaseTaxView->WriteData( FALSE );

					pBaseTaxView->SetDispedFlg( TRUE );
//2016.10.20 UPDATE START
//					pTaxationListView->ReInitDisp( m_isCompChange );
					pBaseTaxView->ReInitDisp( m_isCompChange, m_pMainView->m_SixKindFlg );
//2016.10.20 UPDATE END
//--> '15.04.30 INS START
					AfxSetResourceHandle( m_MainInstance );
//<-- '15.04.30 INS END
				}
			}
//---------------
			if( pBaseView ){
//--> '15.04.30 INS START
				// 10%改正対応
				//if ( GetKaiseiVer() == ID_VER_SYZ31 ) {
				if ( GetActViewVer() == ID_VER_SYZ31 ) {
					AfxSetResourceHandle( GetH31HyoInstance() );
				}
				else {
					AfxSetResourceHandle( GetH26HyoInstance() );
				}
//<-- '15.04.30 INS END
				if( m_SnHeadData.Sn_EXP100&0x01 ){
					pBaseView->UpdateUntilSheet();
				}
				pBaseView->SetDispedFlg( TRUE );

				//-------->特定収入対応150713
				pBaseView->ReInitDisp( m_isCompChange , m_pMainView->m_SixKindFlg , m_pMainView->m_ShRevType );//150106
				//----------------------------------
//				pH26View->ReInitDisp( m_isCompChange , m_pMainView->m_SixKindFlg);//150106
				//<---------------------------------
				
//				pH26View->ReInitDisp( m_isCompChange );

//--> '15.04.30 INS START
				AfxSetResourceHandle( m_MainInstance );
//<-- '15.04.30 INS END
			}

			m_isCompChange = FALSE;//'14.10.08

//--> '14.05.16 INS START
			// この時点の情報を保存
			if( m_pMainView ){
				m_pMainView->SaveTabMakeInfo();
			}
//<-- '14.05.16 INS END

			m_nowTabIdx = index;
			break;

		case 2 :	// 課税取引金額計算表＆特定収入計算表

			//---------->'14.08.05

			UpdateLockIndicator( m_SnHeadData.Sn_Sign4&0x01 );

			//------>'14.08.07
			if( pBaseView ){	
//--> '15.04.30 INS START
				//AfxSetResourceHandle( GetH26HyoInstance() );
//<-- '15.04.30 INS END

				if ( GetActViewVer() == ID_VER_SYZ31 ) {
					AfxSetResourceHandle( GetH31HyoInstance() );
				}
				else {
					AfxSetResourceHandle( GetH26HyoInstance() );
				}
				if(m_nowTabIdx != 0){//基本情報画面以外から切り替えられた時。データの更新
					pBaseView->UpdateAllSheet();
					pBaseView->WriteData( FALSE, TRUE );
				}
				if(m_nowTabIdx == 0){//基本情報画面から切替られた時。タブ再作成
					pBaseView->SetDispedFlg( TRUE );

					//-------->特定収入対応150713
					pBaseView->ReInitDisp( m_isCompChange , m_pMainView->m_SixKindFlg , m_pMainView->m_ShRevType );//150106
					//----------------------------------
//					pH26View->ReInitDisp( m_isCompChange , m_pMainView->m_SixKindFlg);//150106
					//<---------------------------------
					
//					pH26View->ReInitDisp( m_isCompChange );
				}
//--> '15.04.30 INS START
				AfxSetResourceHandle( m_MainInstance );
//<-- '15.04.30 INS END
			}
			//<--------------

			if( pBaseTaxView ){
//--> '15.04.30 INS START
				// 10%改正対応
				if ( GetActViewVer() == ID_VER_SYZ31 ) {
					AfxSetResourceHandle( GetTaxationList31Instance() );
				}
				else {
					AfxSetResourceHandle( GetTaxationListInstance() );
				}
				//	AfxSetResourceHandle( GetTaxationListInstance() );
//<-- '15.04.30 INS END
				pBaseTaxView->SetDispedFlg( TRUE );
//2016.10.20 UPDATE START
//				pTaxationListView->ReInitDisp( m_isCompChange );
				pBaseTaxView->ReInitDisp( m_isCompChange , m_pMainView->m_SixKindFlg );
//2016.10.20 UPDATE END
//--> '15.04.30 INS START
				AfxSetResourceHandle( m_MainInstance );
//<-- '15.04.30 INS END		
			}

			m_isCompChange = FALSE;//'14.08.07

			// この時点の情報を保存
			if( m_pMainView ){
				m_pMainView->SaveTabMakeInfo();
			}
			//<-------------------

			m_nowTabIdx = index;
			break;

		default:
			AfxSetResourceHandle( m_MainInstance );
			break;
	}

}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (ICSMainFrame::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !ICSMainFrame::PreCreateWindow(cs) )
		return FALSE;

	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return TRUE;
}


// CMainFrame 診断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	ICSMainFrame::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	ICSMainFrame::Dump(dc);
}

#endif //_DEBUG


// CMainFrame メッセージ ハンドラ

LRESULT CMainFrame::OnCall( WPARAM wParam, LPARAM lParam )
{
//	return	ICSMainFrame::OnCall( wParam, lParam );

	//プロセス呼び出し開始
	if( wParam == ICS_MAINFRAME_PARAM_CALLSTART ) {
		char*	pStr	=	(char*)lParam;
		//呼び出しを許可しない場合０以外を返す
		//return 1L;
	}
	//プロセス呼び出しに成功
	else if( wParam == ICS_MAINFRAME_PARAM_CALLOK ) {
		//CHAINの場合で自分自身を終了させる場合は０以外を返す
		//return 1L;
	}
	//プロセス呼び出しに失敗
	else if( wParam == ICS_MAINFRAME_PARAM_CALLERROR ) {
		//CHAINの場合で自分自身を終了させる場合は０以外を返す
		//return 1L;
	}
	return	0L;
}

LRESULT CMainFrame::OnFromForkChain( WPARAM wParam, LPARAM lParam )
{
	LRESULT	st	=	ICSMainFrame::OnFromForkChain( wParam, lParam );

	//プロセスが終了
	if( wParam == ICS_MAINFRAME_PARAM_FORKEND ) {
	}
	//自身がFORK済みでアクティブになったとき
	else if( wParam == ICS_MAINFRAME_PARAM_FORKCHANGEACTIVE ) {
		//他のプロセスでマスターに変更が加えられた為、再リードが必要
		if( st ){
			if( (m_swFork==0) && m_pMainView && (m_pMainView->IsPrinting()==FALSE) ){
				if( m_pMainView->IsHistoryMode() == 0 ){
					m_pMainView->SetOptionMode();

					if( m_nowTabIdx != 0 ){
						SetActiveChild( 0 );
					}
					if( ReOpenZmtbl() ){
						return 1;
					}
//--> '14.04.17 INS START
					// 再集計サインをON
					m_isCompReInit = TRUE;
//<-- '14.04.17 INS END
					return 0;
				}
				else{
					// 再集計サインをON
					m_isCompReInit = TRUE;
					return 0;
				}
			}
			else if( (m_swFork==2) && m_pMainView && (m_pMainView->IsPrinting()==FALSE) ){
				// 再集計サインをON
				m_isCompReInit = TRUE;
				return 0;
			}
		}
	}
	//自身がアクティブでボタン押下により、他のプロセスへ切り替えられようとしているとき
	else if( wParam == ICS_MAINFRAME_PARAM_FORKCHANGING ) {
		//他のプロセスで再リードが必要な場合は「1」を返す。
	}
	//自身が初めてプロセス起動されたとき
	else if( wParam == ICS_MAINFRAME_PARAM_FORKCALLEDSTART ) {
	}
	//起動されている自身以外のプロセスがアクティブに切り替えられた時
	else if( wParam == ICS_MAINFRAME_PARAM_FORKCHANGE ) {
	}
	
	return	st;
}

LRESULT CMainFrame::OnFromParentCosel( WPARAM wParam, LPARAM lParam )
{
	if( m_nowTabIdx ){
		ICSMessageBox( _T("基本情報登録画面に戻って、会社を選択してください。") );
		return 1;
	}

//--> '15.11.12 INS START
	// 終了可能？
	CBaseHyoView *pBaseView = GetHonHyoView();
	if( pBaseView != NULL ){
		if( pBaseView->ChkTkkzAndKzwari(m_nowTabIdx, 3) ){
			return 1;
		}
	}
//<-- '15.11.12 INS END

	m_pMainView->HistoryModeOff();

	if( m_Progress ){
		ProgressEnd();
	}

	int sw = 0;
	int st = 0;

//--> '15.03.19 MOVE START
	m_isInitEnd = FALSE;
//<-- '15.03.19 MOVE END

	//他のプロセスで会社選択ツールバーで会社が変更された
	if( wParam == 1 ){
		if( (st = m_cosel.co_ok()) == IDCANCEL ){
//--> '15.03.19 INS START
			m_isInitEnd = TRUE;
//<-- '15.03.19 INS END
			PostMessageA( WM_CLOSE, 1 );
			return 1;
		}
		else if( st != IDOK ){
//--> '15.03.19 INS START
			m_isInitEnd = TRUE;
//<-- '15.03.19 INS END
			return 1;
		}
	}
	//自分自身の会社選択ツールバーが押された
	else {
		COPAK	cop;
		cop.sgn0 = 0x10;
		if( (st=m_cosel.co_ok(cop)) != IDOK ){
//--> '15.03.19 INS START
			m_isInitEnd = TRUE;
//<-- '15.03.19 INS END
			return 1;
		}
	}

//--> '15.02.03 INS START
	m_isShownHonpyo = FALSE;
//--> '15.02.03 INS END

	// 過年度プログラム？
	if( IsBeforeH25Master(0) ){
//--> '14.04.30 INS START
		// マスター情報の保存
		SaveNowSyzInfo();
//<-- '14.04.30 INS END
		// プログラム呼出
		CallSyzAllYearProgram();
//--> '15.03.19 INS START
		m_isInitEnd = TRUE;
//<-- '15.03.19 INS END
		return 1;
	}

//--> '15.03.19 CUT START
//	m_isInitEnd = FALSE;
//<-- '15.03.19 CUT END

	// 印刷中はマスターの再読み込みはしない。
	if( m_pMainView->IsPrinting() == FALSE ){
		//　再読み込み前に印刷ダイアログ、及び、印刷プレビューウィンドウのクローズ
		if( ReConstructMaster() ){
//--> '15.03.19 INS START
			m_isInitEnd = TRUE;
//<-- '15.03.19 INS END
			PostMessageA( WM_CLOSE, 0x2000 );
			return 1;
		}
	}

	// 履歴一覧初期設定
	if( m_pMainView != NULL ){		// メイン画面が出ていない場合はは画面切り替えしない
		// デバッグ用に、SHIFTキーを押しながら起動した際、テーブル消去
		if( InitHistoryReport() ){
//--> '15.03.19 INS START
			m_isInitEnd = TRUE;
//<-- '15.03.19 INS END
			return 1;
		}
// 24/05/08_168730 add -->
		// 課税取引金額計算表、特定収入計算表、税務代理書面の設定値を取得する
		m_pMainView->GetPrnExSubInfo();
// 24/05/08_168730 add <--
	}

	SetActiveChild( 0 );

	// 会社切り替え時にも保存
	m_pMainView->SaveTabMakeInfo();

	m_isInitEnd = TRUE;

	PostMessage( WM_USER_SETFOCUS, 0 );	// 「入力」へ harino ('14.03.09)

	return	0;
}

LRESULT	CMainFrame::OnEndProcess( WPARAM wParam, LPARAM lParam )
{
	if( m_swFork == 1 ){
//--> '14.04.24 INS START
		m_SnHeadData.SetEselVersion( m_pZmSub, 0 );
//<-- '14.04.24 INS END

		PostMessageA( WM_CLOSE, 0x2000 );
//--> '14.03.27 CUT
//		m_swFork = 0;
//<-- '14.03.27
		m_isKeyoff = FALSE;
		return 0;
	}

	// タブが消えるので...
	ToggleMDITabs();
	//--->yoshida190820
	//タブが５％の画面から８％の画面に行くと、全てのタブが表示された。モジュール側にて初期化関数（InitTabOrgIndex）を新たに作成していただいたのでコールする
	int tabmode = 0x11;
	InitTabOrgIndex(tabmode);
	//<----
	SetTabTitle();

	this->SetForegroundWindow();

	CDBSyzShinMainView	*pView = (CDBSyzShinMainView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(BSINFO_VIEW_WND_IDX));
	if( pView != NULL ){
		pView->EndProcess( wParam, lParam );
	}

	//== 注意 ==
	// フォークで呼び出しを行うのは、８％から５％のみなので、戻りとしては８％のみしか考えない！

	// 再データ読込み
	CBaseHyoView *pBaseView = GetHonHyoView(ID_VER_SYZ26);
	if( pBaseView != NULL ){
		pBaseView->GetPastData();
	}

	//---->'15.01.09
	// 課税取引計算書
	//CTaxationListView *pTaxationListView = (CTaxationListView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd( 3 ));
	//if( pTaxationListView != NULL ){
	//	pTaxationListView->GetPastData();
	//}
	CBaseTaxHyoView *pBaseTaxView = GetTaxListView(ID_VER_SYZ26);
	if( pBaseTaxView != NULL ){
		pBaseTaxView->GetPastData();
	}
	//<-----------------


	if( m_pMainView ){
		m_pMainView->SetPastData();
	}
	GetPastSkjData();//20140324G

//--> '14.04.19 INS START
	// 総合会社情報の為の起動時の保存
	Copy_NowCoInfo();
//<-- '14.04.19 INS END

//--> '14.04.22 INS START
	// 印刷情報の再取得
	if( pView != NULL ){
		pView->InitPrintInfo();
	}
//<-- '14.04.22 INS END

//--> '14,05.15 INS START
	if( m_svSVmzsw != -1 ){
		if( m_pZmSub ){
			if( m_pZmSub->szvol ){
				m_pZmSub->SyohizeiClose();
			}
			m_pZmSub->SyohizeiOpen();

			m_pZmSub->szvol->Edit();
			m_pZmSub->szvol->SVmzsw = m_svRealSvmzsw;
			m_pZmSub->szvol->Update();
		}
	}
	m_svRealSvmzsw = 0;
	m_svSVmzsw = -1;
//<-- '14,05.15 INS END

	m_swFork = 0;
	m_isKeyoff = FALSE;

	return 0;
}

//-----------------------------------------------------------------------------
// 消費税マスターバージョンチェック
//-----------------------------------------------------------------------------
// 引数		msg	：	エラーメッセージ取得用
//-----------------------------------------------------------------------------
// 返送値	1	：	処理可能
//			0	：	処理不可
//-----------------------------------------------------------------------------
int CMainFrame::CheckSyzVersion( CString &msg )
{
	if( m_pZmSub->szvol->SVvers < 0x10 ){		// 旧バージョン
		if( 19940401 > m_pZmSub->zvol->ee_ymd ){
			msg = _T("平成６年３月３１日以前の期間のマスターは扱えません。");
			return 0;
		}
		else{
			return 1;
		}
	}
	else if( m_pZmSub->szvol->SVvers < 0x20 ){	// 新バージョン
		int zritu = ChkDefaultZritu();
		if( (zritu&0xff) == 0x01 ){
			msg.Format( _T("この消費税マスター(V%02x：標準税率３％)は扱えません。"), m_pZmSub->szvol->SVvers );
			return 0;
		}
		else{
			return 1;
		}
	}
	else{
		msg.Format( _T("この消費税マスター(V%02x)は扱えません。"), m_pZmSub->szvol->SVvers );
		return 0;
	}
}

//-----------------------------------------------------------------------------
// 決算期間内の税率チェック
//-----------------------------------------------------------------------------
// 返送値	：	有効税率	□□□□ □□□□ □□□□ □□□□ □□□□ □□□□ □□□□ ■■■■
//				  D0：３％
//				  D1：５％
//				  D2：８％
//				  D3：１０％
//-----------------------------------------------------------------------------
int CMainFrame::ChkDefaultZritu()
{
	int	rtZritu = 0;

	if( ICS_SH_8PER_REFORM_DAY <= m_pZmSub->zvol->ee_ymd ){
		if( ICS_SH_8PER_REFORM_DAY <= m_pZmSub->zvol->ss_ymd ){
			rtZritu |= 0x04;
		}
		else{
			rtZritu |= (0x04|0x02);
		}
	}
	else if( ICS_SH_5PER_REFORM_DAY <= m_pZmSub->zvol->ee_ymd ){
		if( ICS_SH_5PER_REFORM_DAY <= m_pZmSub->zvol->ss_ymd ){
			rtZritu |= 0x02;
		}
		else{
			rtZritu |= (0x02|0x01);
		}
	}
	else if( ICS_SH_3PER_REFORM_DAY <= m_pZmSub->zvol->ee_ymd ){
			rtZritu |= 0x01;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 決算期間内 消費税５％改正 チェック
//-----------------------------------------------------------------------------
// 返送値	1	：	決算期間内で３→５の変更あり
//			0	：	一定の税率
//-----------------------------------------------------------------------------
int CMainFrame::Chk_5per_ReviseDayIn()
{
	if( (m_pZmSub->zvol->ss_ymd<ICS_SH_5PER_REFORM_DAY) && (ICS_SH_5PER_REFORM_DAY<=m_pZmSub->zvol->ee_ymd) ){
		return 1;
	}
	else{
		return 0;
	}
}

//-----------------------------------------------------------------------------
// 決算期間内 消費税８％改正 チェック
//-----------------------------------------------------------------------------
int CMainFrame::Chk_8per_ReviseDayIn()
{
	return 0;
}

//-----------------------------------------------------------------------------
// 過年度チェック
//-----------------------------------------------------------------------------
// 調べるべき指針１：自分自身が過年度マスターかを確認
// 調べるべき指針２：自分の前年マスターの情報取得
//-----------------------------------------------------------------------------
int CMainFrame::ChkKndMasterInfo()
{
	// VLCTL から自分自身と過年度の情報を確認

	// 接続中のマスター情報
	char	sqlsvr[128]={0}, apno[32]={0}, cocode[32]={0}, kikan[32]={0};
	int		n_apno, n_cocode, n_kikan;
	PCOM_GetString( "SELECTCODSN", sqlsvr, apno, cocode, kikan );
	n_apno = atoi( apno );
	n_cocode = atoi( cocode );
	n_kikan = atoi( kikan );

	ICSMCTL		icsmctl;
	if( icsmctl.SetDatabase(sqlsvr) ){
		return -1;
	}
	if( icsmctl.vctl_open(0x00400001, n_apno, 0) ){
		return -1;
	}
	icsmctl.get_usertbl( m_usertbl );

	// 現在のマスターが過年度マスター？
	CO_CTL_DB		coctl;
	CO_CTL_Array	coarray;
	memset( &coctl, '\0', sizeof(CO_CTL_DB) );
	coctl.c_apno = n_apno;
	int_bcd( coctl.c_cod, n_cocode, 4 );

	if( icsmctl.vctl_coinf(&coctl, &coarray) ){
		icsmctl.vctl_close();
		return -1;
	}
	int max = (int)coarray.GetCount();
	if( max > 1 ){
		// 現行の決算期間取得
		UCHAR	Mon, Day;
		int		n_Nen;

		n_Nen = n_kikan / 10000;
		n_Nen %= 100;

		long mmdd;
		mmdd = n_kikan % 10000;
		int mm, dd;
		// 月
		mm = mmdd / 100;
		int_bcd( &Mon, mm, 1 );
		// 日
		dd = mmdd % 100;
		int_bcd( &Day, dd, 1 );

		UCHAR	ymd[3]={0}, zymd[3]={0};
		int_bcd( &ymd[0], n_Nen, 1 );
		ymd[1] = Mon;
		ymd[2] = Day;

		for( int i=1; i<max; i++ ){
			if( ymd[0] == coarray[i].c_kikan[3] ){
				m_isKndMster = TRUE;
				break;
			}
		}
	}

	// 前年マスターの年度チェック
	memset( &m_ZMasterinfo, '\0', sizeof(m_ZMasterinfo) );

	bool	hasKMst = false;
	int		z_kikan=0;

	memset( &coctl, '\0', sizeof(CO_CTL_DB) );
	coctl.c_apno = n_apno;
	int_bcd( coctl.c_cod, n_cocode, 4 );
	coarray.RemoveAll();

	long	cksubsw = 0;
	if( m_pZmSub->zvol->sub_sw&0x20000 ){
		cksubsw = (ICSMCTL::ZAIMU_SW|0x20000);
	}
	else{
		cksubsw = ICSMCTL::ZAIMU_SW;
	}

	if( icsmctl.vctl_coinf( cksubsw, &coctl, &coarray ) == 0 ){
		ChkKanendoByVctl( coarray, z_kikan, hasKMst );
	}
	else{
		// 前期：非連動 → 当期：連動と言う形で、変化した場合の備え
		if( m_pZmSub->zvol->sub_sw&0x20000 ){
			cksubsw = ICSMCTL::ZAIMU_SW;
		}
		else{
			cksubsw = (ICSMCTL::ZAIMU_SW|0x20000);
		}
		if( icsmctl.vctl_coinf( cksubsw, &coctl, &coarray ) == 0 ){
			ChkKanendoByVctl( coarray, z_kikan, hasKMst );
		}
	}

	// 接続対象
	if( hasKMst ){
		m_ZMasterinfo.isExist = 1;
		memmove( m_ZMasterinfo.server, sqlsvr, sizeof(sqlsvr) );
		m_ZMasterinfo.nApno = n_apno;
		m_ZMasterinfo.nCode = n_cocode;
		m_ZMasterinfo.nYmd = z_kikan;
		m_KndFlg = 1;
	}
	else{
		m_KndFlg = 0;
	}

	icsmctl.vctl_close();

	return 0;
}

//-----------------------------------------------------------------------------
// 前年の消費税額を取得
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
// ------------------------->2019.12.10.yokono
//          1   ：前年度のマスターの設定が免税
//<-------------------------------------------
//-----------------------------------------------------------------------------
int CMainFrame::GetLastYearSyz()
{
	if( m_ZMasterinfo.isExist == 0 ){
		memset( m_ZenkTax, '\0', sizeof(m_ZenkTax) );
		memset( m_ZenkSymd, '\0', sizeof(m_ZenkSymd) );
		memset( m_ZenkEymd, '\0', sizeof(m_ZenkEymd) );
		return 0;
	}
	if( m_ZMasterinfo.isCalqed ){
		return 0;
	}

	memset( m_ZenkTax, '\0', sizeof(m_ZenkTax) );
	memset( m_ZenkSymd, '\0', sizeof(m_ZenkSymd) );
	memset( m_ZenkEymd, '\0', sizeof(m_ZenkEymd) );

	// データベース財務クラスの作成
	CDBNpSub	*pZmSub=NULL;
	if( (m_pZmSub->zvol->apno&0xf0) > 0x30 ){
		pZmSub = (CDBNpSub*)new CDBNpSub;
	}
	else{
		pZmSub = (CDBNpSub*)new CDBZmSub;
	}
	if( pZmSub == NULL ){
		return ERR;
	}

	// 注意!! 過年度マスターからの取得時はチェックマスターDBを経由しない(SetMasterDBを直呼び)
	// 以降の処理は消費税申告書マスターのレコードのみ依存するため、財務マスターのバージョンの影響を受けない
	// 将来消費税マスターのレイアウトを変更するときは、これ以降の処理に影響があるか考慮して行うこと。

	if( pZmSub->SetMasterDB( 3, m_ZMasterinfo.server, m_ZMasterinfo.nApno, m_ZMasterinfo.nCode, m_ZMasterinfo.nYmd) == ERR ){
		if( pZmSub != NULL ) {
			delete( pZmSub );
			pZmSub = NULL;
		}
		return ERR;
	}

	// 必須テーブルオープン
	//== ボリューム・ラベル ==
	if( pZmSub->VolumeOpen() == ERR ){
		if( pZmSub != NULL ){
			delete( pZmSub );
			pZmSub = NULL;
		}
		return ERR;
	}
	//-------------->2019.12.10.yokono
    //前年度のマスターが免税なら、返送値1を返す。
    //（ツールバーのオプションから「前課税期間の消費税額を過年度マスタより取得する」を行ったとき、前年が免税の場合専用のエラーメッセージを表示するため）
	if ((pZmSub->zvol->s_sgn2 & 0x0f) == 0x01) {
		// ボリューム・ラベル
		pZmSub->VolumeClose();
		if (pZmSub != NULL) {
			delete(pZmSub);
			pZmSub = NULL;
		}
		return 1;
	}
	//<-------------
	// 消費税管理
	if( pZmSub->SyohizeiOpen() == ERR ){
		// ボリューム・ラベル
		pZmSub->VolumeClose();
		if( pZmSub != NULL ){
			delete( pZmSub );
			pZmSub = NULL;
		}
		return ERR;
	}
	// 消費税申告書
	if( pZmSub->SzsnkOpen("Sn_SEQ = 0") == ERR ){
		// ボリューム・ラベル
		pZmSub->VolumeClose();
		// 消費税管理
		pZmSub->SyohizeiClose();
		if( pZmSub != NULL ){
			delete( pZmSub );
			pZmSub = NULL;
		}
		return ERR;
	}
	if( pZmSub->szsnk->st == ERR ){
		// ボリューム・ラベル
		pZmSub->VolumeClose();
		// 消費税管理
		pZmSub->SyohizeiClose();
		if( pZmSub != NULL ){
			delete( pZmSub );
			pZmSub = NULL;
		}
		return ERR;
	}

	if( pZmSub->szsnk->Sn_VER >= ICS_SH_H26_ETAX_VER ){
		GetLastYearTax( pZmSub, m_ZenkTax );
	}
	else{
		unsigned char		m_1F14C[6]={0}, m_SYTX[6]={0}, m_KJADJZ[6]={0}, m_SIREZ[6]={0}, m_HKANZ[6]={0}, m_KSITZ[6]={0};

		// 合計差引税額
		m_Util.val_to_bin( m_1F14C,	pZmSub->szsnk->Sn_1F14C );
		// 消費税額（その他）
		m_Util.val_to_bin( m_SYTX,	pZmSub->szsnk->Sn_SYTX );
		// 貸倒回収に係る税額（控除過大調整税額）
		m_Util.val_to_bin( m_KJADJZ,	pZmSub->szsnk->Sn_KJADJZ );
		// 控除対象仕入税額
		m_Util.val_to_bin( m_SIREZ,	pZmSub->szsnk->Sn_SIREZ );
		// 返還等対価に係る金額
		m_Util.val_to_bin( m_HKANZ,	pZmSub->szsnk->Sn_HKANZ );
		// 貸倒に係る税額
		m_Util.val_to_bin( m_KSITZ,	pZmSub->szsnk->Sn_KSITZ );

		if( pZmSub->szvol->SVmzsw ){							// 経過措置有り
			if( m_Arith.l_test( m_1F14C ) > 0 ){
				memmove( m_ZenkTax, m_1F14C, 6 );
				// １００以下の切り捨て
				m_Util.l_calq( m_ZenkTax, m_ZenkTax, 0x10 );
			}
		}
		else{													// 経過措置無し
			m_Arith.l_add( m_ZenkTax, m_ZenkTax, m_SYTX );		// ＋消費税額②
			m_Arith.l_add( m_ZenkTax, m_ZenkTax, m_KJADJZ );	// ＋控除過大調整税額③
			m_Arith.l_sub( m_ZenkTax, m_ZenkTax, m_SIREZ );		// －控除対象仕入税額④
			m_Arith.l_sub( m_ZenkTax, m_ZenkTax, m_HKANZ );		// －返還等対価に係る税額⑤
			m_Arith.l_sub( m_ZenkTax, m_ZenkTax, m_KSITZ );		// －貸倒に係る税額⑥
			// 限界控除前の税額がマイナスの時
			if( m_Arith.l_test(m_ZenkTax) > 0 ){
				// １００以下の切り捨て
				m_Util.l_calq( m_ZenkTax, m_ZenkTax, 0x10 );
			}
		}
	}

	m_Util.date_to_bcd( 0x04, (unsigned char *)m_ZenkSymd, pZmSub->szsnk->Sn_KDAYS );	// 課税期間　自
	m_Util.date_to_bcd( 0x04, (unsigned char *)m_ZenkEymd, pZmSub->szsnk->Sn_KDAYE );	// 課税期間　至

	// ボリューム・ラベル
	pZmSub->VolumeClose();
	// 消費税管理
	pZmSub->SyohizeiClose();
	// 消費税申告書
	pZmSub->SzsnkClose();
	if( pZmSub != NULL ) {
		delete( pZmSub );
		pZmSub = NULL;
	}
	if( m_Arith.l_test(m_ZenkTax) ){
		m_ZMasterinfo.isNotZero = 1;
	}
	m_ZMasterinfo.isCalqed = 1;

	return 0;
}

//-----------------------------------------------------------------------------
// 現在の選択マスターは過年度マスタ？
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::ChkKanendoMaster()
{
	m_isKndMster = FALSE;

	char	sqlsvr[128]={0}, apno[32]={0}, cocode[32]={0}, kikan[32]={0};
	int		n_apno, n_cocode, n_kikan;
	PCOM_GetString( "SELECTCODSN", sqlsvr, apno, cocode, kikan );
	n_apno = atoi( apno );			// アプリケーションNo.
	n_cocode = atoi( cocode );		// 会社コード
	n_kikan = atoi( kikan );		// 期間

	ICSMCTL		icsmctl;
	if( icsmctl.SetDatabase(sqlsvr) ){
		return -1;
	}
	if( icsmctl.vctl_open(0x00400001, n_apno, 0) ){
		return -1;
	}
	CO_CTL_DB		coctl;
	CO_CTL_Array	coarray;
	memset( &coctl, '\0', sizeof(CO_CTL_DB) );
	coctl.c_apno = n_apno;
	int_bcd( coctl.c_cod, n_cocode, 4 );

	if( icsmctl.vctl_coinf(&coctl, &coarray) ){
		icsmctl.vctl_close();
		return -1;
	}
	int max = (int)coarray.GetCount();
	if( max <= 1 ){
		icsmctl.vctl_close();
		return 0;
	}

	// 現行の決算期間取得
	UCHAR	Mon, Day;
	int		n_Nen;

	n_Nen = n_kikan / 10000;
	n_Nen %= 100;

	long mmdd;
	mmdd = n_kikan % 10000;
	int mm, dd;
	// 月
	mm = mmdd / 100;
	int_bcd( &Mon, mm, 1 );
	// 日
	dd = mmdd % 100;
	int_bcd( &Day, dd, 1 );

	UCHAR	ymd[3]={0}, zymd[3]={0};
	int_bcd( &ymd[0], n_Nen, 1 );
	ymd[1] = Mon;
	ymd[2] = Day;

	for( int i=1; i<max; i++ ){
//		if( memcmp(ymd, &coarray[i].c_kikan[3], sizeof(ymd)) == 0 ){
		if( ymd[0] == coarray[i].c_kikan[3] ){
			m_isKndMster = TRUE;
			break;
		}
	}
	icsmctl.vctl_close();

	return 0;
}

// 改良依頼No.20-0560 add -->
//-----------------------------------------------------------------------------
// 前年の消費税額を取得(予定申告での金額チェック用)
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::GetZenSyz(char *pArith)
{
	char zenarith[6] = {0};

	memset(pArith,'\0',6);

	// 前年度マスターが存在しない
	if(m_ZMasterinfo.isExist == 0) {
		return(0);
	}

	// データベース財務クラスの作成
	CDBNpSub	*pZmSub=NULL;
	if( (m_pZmSub->zvol->apno&0xf0) > 0x30 ){
		pZmSub = (CDBNpSub*)new CDBNpSub;
	}
	else{
		pZmSub = (CDBNpSub*)new CDBZmSub;
	}
	if( pZmSub == NULL ){
		return ERR;
	}

	// 注意!! 過年度マスターからの取得時はチェックマスターDBを経由しない(SetMasterDBを直呼び)
	// 以降の処理は消費税申告書マスターのレコードのみ依存するため、財務マスターのバージョンの影響を受けない
	// 将来消費税マスターのレイアウトを変更するときは、これ以降の処理に影響があるか考慮して行うこと。

	if( pZmSub->SetMasterDB( 3, m_ZMasterinfo.server, m_ZMasterinfo.nApno, m_ZMasterinfo.nCode, m_ZMasterinfo.nYmd) == ERR ){
		if( pZmSub != NULL ) {
			delete( pZmSub );
			pZmSub = NULL;
		}
		return ERR;
	}

	// 必須テーブルオープン
	//== ボリューム・ラベル ==
	if( pZmSub->VolumeOpen() == ERR ){
		if( pZmSub != NULL ){
			delete( pZmSub );
			pZmSub = NULL;
		}
		return ERR;
	}
	//前年度のマスターが免税か
	if ((pZmSub->zvol->s_sgn2 & 0x0f) == 0x01) {
		// ボリューム・ラベル
		pZmSub->VolumeClose();
		if (pZmSub != NULL) {
			delete(pZmSub);
			pZmSub = NULL;
		}
		return 0;
	}
	// 消費税管理
	if( pZmSub->SyohizeiOpen() == ERR ){
		// ボリューム・ラベル
		pZmSub->VolumeClose();
		if( pZmSub != NULL ){
			delete( pZmSub );
			pZmSub = NULL;
		}
		return ERR;
	}
	// 消費税申告書
	if( pZmSub->SzsnkOpen("Sn_SEQ = 0") == ERR ){
		// ボリューム・ラベル
		pZmSub->VolumeClose();
		// 消費税管理
		pZmSub->SyohizeiClose();
		if( pZmSub != NULL ){
			delete( pZmSub );
			pZmSub = NULL;
		}
		return ERR;
	}
	if( pZmSub->szsnk->st == ERR ){
		// ボリューム・ラベル
		pZmSub->VolumeClose();
		// 消費税管理
		pZmSub->SyohizeiClose();
		if( pZmSub != NULL ){
			delete( pZmSub );
			pZmSub = NULL;
		}
		return ERR;
	}

	GetLastYearTax( pZmSub, zenarith );

	// ボリューム・ラベル
	pZmSub->VolumeClose();
	// 消費税管理
	pZmSub->SyohizeiClose();
	// 消費税申告書
	pZmSub->SzsnkClose();
	if( pZmSub != NULL ) {
		delete( pZmSub );
		pZmSub = NULL;
	}

	memmove ( pArith, zenarith, sizeof ( zenarith ) );

	return 0;
}
// 改良依頼No.20-0560 add <--

#include "ShinMinfoCheck.h"

//--> '15.01.07 INS START
//-----------------------------------------------------------------------------
// 履歴があるかどうかのチェック
//-----------------------------------------------------------------------------
// 返送値	1	：	
//			0	：	
//-----------------------------------------------------------------------------
int CMainFrame::IsExistHistoryFromDB()
{
	int	rt = 0;

	if( m_pZmSub->szsnk == NULL ){
		m_pZmSub->SzsnkOpen();
	}
	else{
		CString	filter;
		m_pZmSub->szsnk->Requery( filter );
	}

	for( int i=0;; i++ ){
		if( i==0 ){
			if( (m_pZmSub->szsnk->MoveFirst()==-1) || (m_pZmSub->szsnk->st==-1) ){
				break;
			}
		}
		else{
			if( (m_pZmSub->szsnk->MoveNext()==-1) || (m_pZmSub->szsnk->st==-1) ){
				break;
			}
		}
		if( m_pZmSub->szsnk->Sn_SEQ > 0 ){
			rt = 1;
			break;
		}
	}

	return rt;
}
//<-- '15.01.07 INS END

//-----------------------------------------------------------------------------
// 履歴情報の初期化
//-----------------------------------------------------------------------------
// 返送値	0	：
//			-1	：
//-----------------------------------------------------------------------------
int CMainFrame::InitHistoryReport()
{
	ASSERT( m_pMainView );

//--> '15.01.13 INS START
	BOOL	isExistHistory = FALSE;
//<-- '15.01.13 INS END

	// デバッグ用に、CTRLキーを押しながら起動した際、テーブル消去
	int execdrop = (GetKeyState(VK_CONTROL)&0x8000) != 0;
	if( m_pMainView->CheckShinMinfo() ){

//-- '15.01.13 --
//		CShinMinfoCheck check;
//		if( check.DoModal() != IDOK ){
//			ICSMessageBox( _T("処理を中止します。") );
//			PostMessageA( WM_CLOSE, 0x2000 );
//			return 1;
//		}
//---------------
		if( IsExistHistoryFromDB() ){
			// DB 上に履歴データがある時のみ質問！
			CShinMinfoCheck check;
			if( check.DoModal() != IDOK ){
				ICSMessageBox( _T("処理を中止します。") );
				PostMessageA( WM_CLOSE, 0x2000 );
				return 1;
			}
			isExistHistory = TRUE;
		}
//---------------
		execdrop = TRUE;
	}
	if( execdrop ){
//-- '14.09.10 --
//		if( m_pMainView->DropHistoryAll() ){
//			ICSMessageBox( _T("処理を中止します。") );
//			PostMessageA( WM_CLOSE, 0x2000 );
//			return 1;
//		}
//---------------
		CString	msg;
		int		st = 0;
//--- '15.01.13 ---
//		if( (st=m_pMainView->DropHistoryAll(msg)) == 1 ){
//-----------------
		if( (st=m_pMainView->DropHistoryAll(msg, isExistHistory)) == 1 ){
//-----------------
			ICSMessageBox( _T("処理を中止します。") );
			PostMessageA( WM_CLOSE, 0x2000 );
			return 1;
		}
		else if( st == -1 ){
			ICSMessageBox( msg );
			PostMessageA( WM_CLOSE, 0x2000 );
			return 1;
		}
//---------------
	}
	m_pMainView->HistoryReportInit();
	m_pMainView->SetOldData();

	return 0;
}

//-----------------------------------------------------------------------------
// 期間の取得
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CMainFrame::GetKzkikanOfs()
{
	return 0;
}

//-----------------------------------------------------------------------------
// 集計済み金額の取得	('14.06.30)
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::GetCalqedMony()
{
	CBaseHyoView *pBaseView = GetHonHyoView();
	if (pBaseView != NULL) {
		return pBaseView->GetCalqedMony();
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 集計
//-----------------------------------------------------------------------------
// 引数	pUwriai	：	課税売上割合の格納先
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::StartSyzSyukei( char *pUwriai )
{
	ASSERT( m_pSyzSyukei );
	if( m_pSyzSyukei == NULL ){
		return -1;
	}

	CSyzScopeOption	scopeOption;

	int		tmpMmdd=0, sofs=-1, eofs=-1;
	char	buf[16]={0};
	int		endYymmdd=0;
	sprintf_s( buf, sizeof(buf), _T("%02x%02x"), m_Vol2.D_TBL[m_ShinInfo.s_tno]._EMD[0], m_Vol2.D_TBL[m_ShinInfo.s_tno]._EMD[1] );
	int	sMmdd = atoi( buf );
	sprintf_s( buf, sizeof(buf), _T("%02x%02x"), m_Vol2.D_TBL[m_ShinInfo.d_tno]._EMD[0], m_Vol2.D_TBL[m_ShinInfo.d_tno]._EMD[1] );
	int	eMmdd = atoi( buf );
	for( int i=0;; i++ ){
		if( i==0 ){
			if( (m_pZmSub->minfo->MoveFirst()==-1) || (m_pZmSub->minfo->st==-1) ){
				break;
			}
		}
		else{
			if( (m_pZmSub->minfo->MoveNext()==-1) || (m_pZmSub->minfo->st==-1) ){
				break;
			}
		}
		tmpMmdd = m_pZmSub->minfo->emd % 10000;
		if( sofs == -1 ){
			if( tmpMmdd == sMmdd ){
				sofs = m_pZmSub->minfo->ofset;
			}
		}
		if( tmpMmdd == eMmdd ){
			eofs = m_pZmSub->minfo->ofset;
//--> '14.03.10 INS START
			endYymmdd = m_pZmSub->minfo->emd;
//<-- '14.03.10 INS END
		}
		else if( eofs != -1 ){
			break;
		}
	}
	scopeOption.sofs = sofs;
	scopeOption.eofs = eofs;

	if( scopeOption.sofs < 0 ){
		scopeOption.sofs = 0;
	}
	if( scopeOption.eofs > 15 ){
		scopeOption.eofs = 15;
	}

//-- '15.09.29 --
//	if( m_SnHeadData.Sn_SPECIAL&0x01 ){
//		CSyzCalqOption	clqOption;
//		clqOption.SimpleSw = TRUE;
//		clqOption.MakeShisan = 1;
//		clqOption.shisan_rawdata = 1;
//		if( m_pSyzSyukei->stmcalq(&scopeOption, &clqOption) ){
//			return -1;
//		}
//	}
//	else{
//		CSyzCalqOption	clqOption;
//		if( m_SnHeadData.Sn_TUMIAG&0x01 ){
//			clqOption.K221 = TRUE;
//		}
//		if( m_SnHeadData.Sn_TUMIAG&0x02 ){
//			clqOption.H301 = TRUE;
//		}
//		clqOption.MakeShisan = 1;
//		clqOption.shisan_rawdata = 1;
//
//		if( m_pSyzSyukei->stmcalq(&scopeOption, &clqOption) ){
//			return -1;
//		}
//	}
//---------------
	bool isNiwari = false;

	// 一次集計
	if( m_SnHeadData.Sn_SPECIAL&0x01 ){
		CSyzCalqOption	clqOption;
		clqOption.SimpleSw = TRUE;
		clqOption.MakeShisan = 1;
		clqOption.shisan_rawdata = 1;

		if( m_pMainView ) {
			if( m_pMainView->curset.m_DispTabSgn & 0x2000 ){
				isNiwari = true;
			}
		}
		else {
			if( m_SnHeadData.m_DispTabSgn & 0x2000 ){
				isNiwari = true;
			}
		}

		if( isNiwari ) {
			// 第二種固定で計算する為にセット
			clqOption.SimComp = TRUE;		// TRUEにしておかないと集計モジュール側で以下2つのフラグを参照しないためTRUEにする
			clqOption.SimKazeiType = kani;  // 課税方式
			clqOption.SimCategorySw = 2;    // 簡易課税の基本業種(1-6を指定)
		}
		if( m_pSyzSyukei->stmonlycalq(&scopeOption, &clqOption) ){
			return -1;
		}
	}
	else{
		CSyzCalqOption	clqOption;
		if( m_SnHeadData.Sn_TUMIAG&0x01 ){
			clqOption.K221 = TRUE;
		}
		if( m_SnHeadData.Sn_TUMIAG&0x02 ){
			clqOption.H301 = TRUE;
		}
		clqOption.MakeShisan = 1;
		clqOption.shisan_rawdata = 1;

		if( m_pMainView ) {
			if( m_pMainView->curset.m_DispTabSgn & 0x2000 ){
				isNiwari = true;
			}
		}
		else {
			if( m_SnHeadData.m_DispTabSgn & 0x2000 ){
				isNiwari = true;
			}
		}

		if( isNiwari ){
			// 第二種固定で計算する為にセット
			clqOption.SimComp = TRUE;		// TRUEにしておかないと集計モジュール側で以下2つのフラグを参照しないためTRUEにする
			clqOption.SimKazeiType = kani;  // 課税方式
			clqOption.SimCategorySw = 2;    // 簡易課税の基本業種(1-6を指定)
		}

		if( m_pSyzSyukei->stmonlycalq(&scopeOption, &clqOption) ){
			return -1;
		}
	}

	// 実データ集計
	m_pSyzSyukei->MakeData( 0x01 );
	m_SvRevMoneyArray = m_pSyzSyukei->GetShisanData( PR_SHIIRE );

//---> '15.10.01 MOVE START
	int svDispTabSgn = m_SnHeadData.m_DispTabSgn;
	if( (m_pSyzSyukei->IsReverseCharge()==true) && (m_pSyzSyukei->ReverseChargeHassei()==true) ){
		m_SnHeadData.m_DispTabSgn |= 0x01;
		if( m_pMainView && (m_SnSeq==0) ){
			m_pMainView->curset.m_DispTabSgn |= 0x01;
		}
	}
	else{
		m_SnHeadData.m_DispTabSgn &= ~0x01;
		if( m_pMainView && (m_SnSeq==0) ){
			m_pMainView->curset.m_DispTabSgn &= ~0x01;
		}
	}
//<--- '15.10.01 MOVE END

//2015.11.04 UPDATE START
//	if( m_pSyzSyukei->ReverseChargeHassei() == false ){
	if( m_pSyzSyukei->IsReverseCharge()==false){
//2015.11.04 UPDATE END
		m_pSyzSyukei->MakeData( -1 );
	}
//---------------

	// 経過税率あり？
	unsigned char	svSVmzsw = m_SnHeadData.SVmzsw;
	int sign = m_pSyzSyukei->GetHasseiSign();
	int ursign = m_pSyzSyukei->GetHasseiSign( 0 );
	EnumIdIcsShKazeihoushiki kzhousiki = m_Util.GetKazeihoushiki( m_pZmSub->zvol );

/*- '14.03.10 -*/
//	if( ((sign&0xff)==0x04) || !(sign&0xff) ){
//		// 8%のみか発生無しの時のみ！
//		m_SnHeadData.SVmzsw &= ~0x01;
//	}
//	else{
//		m_SnHeadData.SVmzsw |= 0x01;
////		m_SnHeadData.SVmzsw &= ~0x01;
//	}
/*-------------*/
	if (GetActViewVer() == ID_VER_SYZ31) {	// 仮対応
		if( kzhousiki == ID_ICSSH_GENSOKU ){
			//if( (sign&0x18) || !(sign&0xff) ){
			//if ( !( sign & 0x07 ) || !( sign & 0xff ) ){
			if( (!( sign & 0x07 ) && !( sign & 0x700 ) && !( sign & 0x70000 )) || !( sign & 0xfffff ) ){	// 旧税率で免税サインONのもの（80%控除/50%控除）もチェック
				// 10%、8%軽減のみか発生無しの時のみ！
				m_SnHeadData.SVmzsw &= ~0x01;
			}
			else{
				m_SnHeadData.SVmzsw |= 0x01;
			}

			if ( m_sgSpc ) {
				if ( !( m_SnHeadData.SVmzsw & 0x01 ) && ( Chk5PerSpc ( scopeOption.sofs, scopeOption.eofs, endYymmdd ) == 1 ) ) {
					m_SnHeadData.SVmzsw |= 0x01;
				}
			}
		}
		else if( kzhousiki == ID_ICSSH_KANNI ){
			//if( (ursign&0x18) || !(ursign&0xff) ){
			if ( !( ursign & 0x07 ) || !( ursign & 0xff ) ){
				// 10%、8%軽減のみか発生無しの時のみ！
				m_SnHeadData.SVmzsw &= ~0x01;
			}
			else{
				m_SnHeadData.SVmzsw |= 0x01;
			}
		}
	}
	else if( endYymmdd < ICS_SH_8PER_REFORM_DAY ){
		if( kzhousiki == ID_ICSSH_GENSOKU ){
			if( ((sign&0xff)==0x02) || !(sign&0xff) ){
				// 5%のみか発生無しの時のみ！
				m_SnHeadData.SVmzsw &= ~0x01;
			}
			else{
				m_SnHeadData.SVmzsw |= 0x01;
			}
		}
		else if( kzhousiki == ID_ICSSH_KANNI ){
			if( ((ursign&0xff)==0x02) || !(ursign&0xff) ){
				// 5%のみか発生無しの時のみ！
				m_SnHeadData.SVmzsw &= ~0x01;
			}
			else{
				m_SnHeadData.SVmzsw |= 0x01;
			}
		}
	}
	else{
		if( kzhousiki == ID_ICSSH_GENSOKU ){
			if( ((sign&0xff)==0x04) || !(sign&0xff) ){
				// 8%のみか発生無しの時のみ！
				m_SnHeadData.SVmzsw &= ~0x01;
			}
			else{
				m_SnHeadData.SVmzsw |= 0x01;
			}
//--> '15.04.27 INS START
			if( m_sgSpc ){
				if( !(m_SnHeadData.SVmzsw&0x01) && (Chk5PerSpc(scopeOption.sofs, scopeOption.eofs, endYymmdd)==1) ){
					m_SnHeadData.SVmzsw |= 0x01;
				}
			}
//<-- '15.04.27 INS END

		}
		else if( kzhousiki == ID_ICSSH_KANNI ){
			if( ((ursign&0xff)==0x04) || !(ursign&0xff) ){
				// 8%のみか発生無しの時のみ！
				m_SnHeadData.SVmzsw &= ~0x01;
			}
			else{
				m_SnHeadData.SVmzsw |= 0x01;
			}
		}
	}
/*-------------*/

//---> '15.10.01 MOVE START
////--> '15.08.26 INS START
//	int svDispTabSgn = m_SnHeadData.m_DispTabSgn;
//	if( m_pSyzSyukei->IsReverseCharge() ){
//		m_SnHeadData.m_DispTabSgn |= 0x01;
//	}
//	else{
//		m_SnHeadData.m_DispTabSgn &= ~0x01;
//	}
////<-- '15.08.26 INS END
//<--- '15.10.01 MOVE END

//-- '15.08.26 --
//	if( svSVmzsw != m_SnHeadData.SVmzsw ){
//---------------
	if( (svSVmzsw!=m_SnHeadData.SVmzsw) || (svDispTabSgn!=m_SnHeadData.m_DispTabSgn) ){
//---------------
		m_SnHeadData.SetData( m_pZmSub, m_SnSeq );
	}

	// 課税売上割合取得
	MoneyBasejagArray money;

	// 10%改正対応
	if ( GetActViewVer() == ID_VER_SYZ31 ) {
		money = m_pSyzSyukei->GetShinkokuData( _T("323300") );
	}
	else {
		money = m_pSyzSyukei->GetShinkokuData( _T("120000") );
	}

	int cnt = (int)money.GetCount();
	if( cnt && pUwriai ){
		memmove( pUwriai, money[0][0].arith, MONY_BUF_SIZE );
	}

//--- '15.02.24 ---
////--> '14.04.22 INS START
//	// 還付申告が必要？
//	if( (m_SnHeadData.Sn_SKKBN&0xff) == 0x01 ){
//		m_SnHeadData.Sn_Sign1 &= 0xf9;
//		if( kzhousiki == ID_ICSSH_GENSOKU ){
//			money = m_pSyzSyukei->GetShinkokuData( _T("100800") );
//		}
//		else{
//			money = m_pSyzSyukei->GetShinkokuData( _T("130800") );
//		}
//		cnt = (int)money.GetCount();
//		if( cnt ){
//			if( m_Arith.l_test(money[0][0].arith) ){
//				m_SnHeadData.Sn_Sign1 |= 0x06;
//			}
//		}
//	}
////<-- '14.04.22 INS END
//-----------------
	if( kzhousiki == ID_ICSSH_GENSOKU ){
		if( (m_SnHeadData.Sn_SKKBN&0xff) == 0x01 ){
			m_SnHeadData.Sn_Sign1 &= 0xf9;
			if( kzhousiki == ID_ICSSH_GENSOKU ){
				// 10%改正対応yoshida190613
				if (GetActViewVer() == ID_VER_SYZ31) {
					money = m_pSyzSyukei->GetShinkokuData(_T("300800"));
				}
				else {
					money = m_pSyzSyukei->GetShinkokuData(_T("100800"));
				}
			}
//			else{
//				money = m_pSyzSyukei->GetShinkokuData( _T("130800") );
//			}
			cnt = (int)money.GetCount();
			if( cnt ){
				if( m_Arith.l_test(money[0][0].arith) ){
					m_SnHeadData.Sn_Sign1 |= 0x06;
				}
			}
			if( m_pMainView ){
				if( m_SnHeadData.Sn_Sign1&0x06 ){
	
				m_pMainView->ChangeRefundOut( 1 );
				}
				else{
					m_pMainView->ChangeRefundOut( 0 );
				}
			}
		}
		else{
			if( m_pMainView ){
				m_pMainView->ChangeRefundOut( 0 );
			}
		}
	}
	else{
		if( m_pMainView ){
			m_pMainView->ChangeRefundOut( 0 );
		}
	}
//-----------------

	return 0;
}

//-----------------------------------------------------------------------------
// 仮受・仮払消費税チェック
//-----------------------------------------------------------------------------
// 返送値	0	：	正当な仮受・仮払
//			-1	：	不正な仮受・仮払
//-----------------------------------------------------------------------------
int CMainFrame::CheckKariukeKaribarai()
{
	if( !(m_SnHeadData.Sn_SPECIAL&0x01) ){
		CKariCheck	KariCheck(m_pSyzSyukei->GetStmclq());
		if( KariCheck.CheckKariukeKaribarai() ){
			return -1;
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 再集計
//-----------------------------------------------------------------------------
// 引数	bMiddleCalq	：	中間申告の再転記を行う？
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::ReStartSyzSyukei( BOOL bMiddleCalq )
{
	if( ( ((m_SnHeadData.Sn_SKKBN)==0x01) || (((m_SnHeadData.Sn_SKKBN)==0x02)&&(m_SnHeadData.IsMiddleProvisional()==FALSE)) || ((m_SnHeadData.Sn_SKKBN)==0x03) || ((m_SnHeadData.Sn_SKKBN)==0x04) ) && ((m_SnHeadData.Sn_Sign4&0x81)==0x00) ){
/*- '14.03.03 -*/
//		char	uwriai[MONY_BUF_SIZE]={0};
//		if( StartSyzSyukei(uwriai) ){
//			return -1;
//		}
/*-------------*/
		if( m_pMainView ){
			m_pMainView->ShinCalq( bMiddleCalq );
		}
/*-------------*/

		// 各帳票のデータ更新
		if( RegetCalqedMony() ){
			return -1;
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 金額再取得
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::RegetCalqedMony()
{
	// 各帳票のデータ更新
	// 本表・付表ビュー
	CBaseHyoView *pBaseView = GetHonHyoView();
	if( pBaseView != NULL ){
		pBaseView->ReGetCalqedMony();
	}
	// 課税取引計算書 or 特定収入計算書
	if(	m_Util.IsBasicMst(m_pZmSub->zvol) || m_Util.IsSpcMaster(m_pZmSub) ){
		CBaseTaxHyoView *pBaseTaxView = GetTaxListView();
		if( pBaseTaxView != NULL ){
			if( m_nowTabIdx == 1 ){
				pBaseTaxView->ReGetCalqedMony( FALSE );
			}
			else{
				pBaseTaxView->ReGetCalqedMony( TRUE );
			}
		}
	}

//	PostMessage( WM_USER_SETFOCUS, 0 );	// 「入力」へ harino ('14.04.28)

	return 0;
}

//-----------------------------------------------------------------------------
// 現申告データを消去して、データの再読込み
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::ReReadData()
{
	int rt = -1;

	CBaseHyoView *pBaseView = GetHonHyoView();
	if( pBaseView != NULL ){
		rt = pBaseView->ReReadData();
	}

//	if( rt == -1 ){
//		return rt;
//	}

	CBaseTaxHyoView *pBaseTaxView = GetTaxListView();

	if( pBaseTaxView != NULL ){
		rt = pBaseTaxView->ReReadData();
	}

	return rt;
}

//-----------------------------------------------------------------------------
// 手入力サインのクリア
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::ClearManualInputSign( BOOL isChgKzritu )
{
	int	rt = -1;

	CBaseHyoView *pBaseView = GetHonHyoView();
	if( pBaseView != NULL ){
		rt = pBaseView->ClearManualInputSign( isChgKzritu );
	}

	CBaseTaxHyoView *pBaseTaxView = GetTaxListView();
	if( pBaseTaxView != NULL ){
		rt = pBaseTaxView->ClearManualInputSign();
	}

	return rt;
}

//-----------------------------------------------------------------------------
// 不要になった帳票のデータをクリア
//-----------------------------------------------------------------------------
void CMainFrame::ClearHyoData()
{
	CH31HyoView	*pH31View = (CH31HyoView *)GetHonHyoView(ID_VER_SYZ31);
	if (pH31View != NULL) {
		pH31View->ClearHyoData();
	}
}

void CMainFrame::OnTaxtionList()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。

	int		index;
	BOOL	minmax;
	CWnd *pWnd = GetActiveChild( &index, &minmax );

	if( index == 2 ){	// 課税取引金額計算表
		CBaseTaxHyoView *pBaseTaxView = GetTaxListView();
		if( pBaseTaxView != NULL ){
			int	KHstax = 0;
			if( pBaseTaxView->ChgTaxationSystem(&KHstax) == IDOK ){
				// 履歴情報の更新
				m_pMainView->curset.Sn_Sign2 &= 0xf7;
				if( KHstax ){
//					KHstax |= 0x08;
					m_pMainView->curset.Sn_Sign2 |= 0x08;
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
// 引数	Bar		：	
//		id		：	
//		text	：	
//		color	：	
//-----------------------------------------------------------------------------
void SetPane( CXTPStatusBar& Bar, int id, CString text, COLORREF color )
{
	CXTPStatusBarPane* pane = Bar.AddIndicator( id,1 );
	CFont f;
	f.CreateFont(
		12, 0, 0, 0,
		FW_BOLD, FALSE, FALSE, 0,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, DEFAULT_PITCH, _T("ＭＳ ゴシック")
	);
	pane->SetTextFont(&f);
	pane->SetText(text);
	pane->SetWidth(75);
	pane->SetTextColor( color );
	pane->SetBackgroundColor(RGB(0xff,0xff,0xff));
	pane->SetTextAlignment(1);
	pane->SetVisible(FALSE);
}

BOOL CMainFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle , const RECT& rect , CWnd* pParentWnd , LPCTSTR lpszMenuName , DWORD dwExStyle , CCreateContext* pContext)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	int rt = ICSMainFrame::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, lpszMenuName, dwExStyle, pContext);

	SetPane( m_wndStatusBar, ID_SH_LOCK_ID, _T("入力ロック中"), RGB(255,0,0) );
	SetPane( m_wndStatusBar, ID_SH_RIREKI_ID, _T("履歴選択中"), RGB(0,0,255) );

	return rt;
}

//-----------------------------------------------------------------------------
// 履歴番号の切替（他表）
//-----------------------------------------------------------------------------
// 引数	snseq	：	セットする履歴シーケンス
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::ChangeViewSnSeq( int snseq )
{
	int	st = -1;

	CBaseHyoView *pBaseView = GetHonHyoView();
	if( pBaseView != NULL ){
		st = pBaseView->ChangeSnSeq( snseq );
	}

	//履歴対応。ここでTax版のChangeSnSeqを作成する'14.09.01
//-- '15.07.21 --
//	CTaxationListView *pTaxationListView = (CTaxationListView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd( 3 ));
//	if( pTaxationListView != NULL ){
//		st = pTaxationListView->ChangeSnSeq( snseq );
//	}
//---------------
	if( m_Util.IsBasicMst(m_pZmSub->zvol) || m_Util.IsSpcMaster(m_pZmSub) ){
		CBaseTaxHyoView *pBaseTaxView = GetTaxListView();
		if( pBaseTaxView != NULL ){
			st = pBaseTaxView->ChangeSnSeq( snseq );
		}
	}
//---------------

	// ヘッダ情報読込み
	if( m_SnSeq != snseq ){
		m_SnHeadData.SetData( m_pZmSub, m_SnSeq );
		m_SnHeadData.GetData( m_pZmSub, snseq );
		m_SnSeq = snseq;
	}

	return st;
}

//=============================================================================
// 履歴番号の切替（他表）
//-----------------------------------------------------------------------------
// 引数
//			snseq	：	セットする履歴シーケンス
//			actVer	：	ビューのバージョン
//-----------------------------------------------------------------------------
// 返送値
//			0	：	正常終了
//			-1	：	エラー
//=============================================================================
int CMainFrame::ChangeViewSnSeq( int snseq, int actVer )
{
	int	st = -1;

	CBaseHyoView *pBaseView = GetHonHyoView ( actVer );
	if ( pBaseView != NULL ){
		st = pBaseView->ChangeSnSeq ( snseq );
	}

	if ( m_Util.IsBasicMst ( m_pZmSub->zvol ) || m_Util.IsSpcMaster ( m_pZmSub ) ) {
		CBaseTaxHyoView *pBaseTaxView = GetTaxListView ( actVer );
		if ( pBaseTaxView != NULL ) {
			st = pBaseTaxView->ChangeSnSeq ( snseq );
		}
	}

	if ( m_SnSeq != snseq ) {
		m_SnHeadData.SetData ( m_pZmSub, m_SnSeq );
		m_SnHeadData.GetData ( m_pZmSub, snseq );
		m_SnSeq = snseq;
	}

	return st;
}

//-----------------------------------------------------------------------------
// 前年の消費税額取得（過年度マスターから）
//-----------------------------------------------------------------------------
// 引数	pBfNpSub	：	過年度マスターに接続済の非営利クラス
//		pArith		：	消費税額の保存先
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::GetLastYearTax( CDBNpSub *pBfNpSub, char *pArith )
{
	CBaseHyoView *pBaseView = GetHonHyoView();
	if( pBaseView != NULL ){
		return pBaseView->GetLastYearTax( pBfNpSub, pArith );
	}

	return -1;
}

//-----------------------------------------------------------------------------
// 金額取得
//-----------------------------------------------------------------------------
// 引数	tag		：	メインタグ
//		chtag	：	明細タグ
//		pArith	：	金額の取得先
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::GetMony( CString &tag, CString &chtag, char *pArith )
{
	CBaseHyoView *pBaseView = GetHonHyoView();
	if( pBaseView != NULL ){
		return pBaseView->GetMony( tag, chtag, pArith );
	}

	return -1;
}

//-----------------------------------------------------------------------------
// 金額取得
//-----------------------------------------------------------------------------
// 引数	tag		：	メインタグ
//		chtag	：	明細タグ
//		pArith	：	金額の取得先
//		kver	：	取得先ビューのバージョン
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::GetMony(CString &tag, CString &chtag, char *pArith, int kver)
{
	CBaseHyoView *pBaseView = GetHonHyoView(kver);
	if( pBaseView != NULL ){
		return pBaseView->GetMony(tag, chtag, pArith);
	}

	return -1;
}

//-----------------------------------------------------------------------------
// 金額セット
//-----------------------------------------------------------------------------
// 引数	tag		：	メインタグ
//		chtag	：	明細タグ
//		pArith	：	金額の取得先
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::SetMony( CString &tag, CString &chtag, char *pArith )
{
	CBaseHyoView *pBaseView = GetHonHyoView();
	if( pBaseView != NULL ){
		return pBaseView->SetMony( tag, chtag, pArith );
	}

	return -1;
}

//-----------------------------------------------------------------------------
// 金額セット
//-----------------------------------------------------------------------------
// 引数	tag		：	メインタグ
//		chtag	：	明細タグ
//		pArith	：	セットする金額
//		kver	：	セットビューのバージョン
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::SetMony(CString &tag, CString &chtag, char *pArith, int kver)
{
	CBaseHyoView *pBaseView = GetHonHyoView(kver);
	if( pBaseView != NULL ){
		return pBaseView->SetMony(tag, chtag, pArith);
	}

	return -1;
}

//-----------------------------------------------------------------------------
// 本表の計算(1)～(26)迄
// 前回が確定で今回が修正確定の場合納付税額を既確定へ転送する準備
//-----------------------------------------------------------------------------
void CMainFrame::AlreadyDecisionCalc( int type )
{
	CBaseHyoView *pBaseView = GetHonHyoView();
	if( pBaseView != NULL ){
		pBaseView->AlreadyDecisionCalc( type );
	}
}

//-----------------------------------------------------------------------------
// 部分データ書込み
//-----------------------------------------------------------------------------
// 引数	flg	：	書込み対象
//-----------------------------------------------------------------------------
int CMainFrame::WriteSepData( int flg )
{
//	CBaseHyoView *pBaseView = GetHonHyoView();
	CBaseHyoView *pBaseView = GetHonHyoView( GetActViewVer() );
	if( pBaseView != NULL ){
		return pBaseView->WriteSepData( flg );
	}

	return -1;
}

//-----------------------------------------------------------------------------
// 部分データ読込み
//-----------------------------------------------------------------------------
// 引数	flg	：	書込み対象
//-----------------------------------------------------------------------------
int CMainFrame::ReadSepData( int flg )
{
//	CBaseHyoView *pBaseView = GetHonHyoView();
	CBaseHyoView *pBaseView = GetHonHyoView( GetActViewVer() );
	if( pBaseView != NULL ){
		return pBaseView->ReadSepData( flg );
	}

	return -1;
}

//-----------------------------------------------------------------------------
// ツールバーの変更
//-----------------------------------------------------------------------------
void CMainFrame::ChangeToolbar()
{
	ICSToolBar	*pToolbar = GetToolBar(2);
	if( pToolbar ){
		pToolbar->SetButtonVisible( 0, FALSE );
	}
}

//-----------------------------------------------------------------------------
// 一括印刷情報の取得
//-----------------------------------------------------------------------------
// 引数	pBpState	：	一括印刷の状態
//		pBpPrintrec	：	一括印刷の情報
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//			-2		：	一括印刷でない
//-----------------------------------------------------------------------------
int CMainFrame::GetBatchPrintInfo( EnumIdShBatchPrintState *pBpState, SH_BPPRINT_INFO *pBpPrintInfo )
{
	if( m_swBeForked == ID_ORIGINALSTART_BPSTATE ){
		return -2;
	}
	ASSERT( pBpState );
	ASSERT( pBpPrintInfo );

	*pBpState = (EnumIdShBatchPrintState)m_swBeForked;
	memmove( pBpPrintInfo, &m_BpPrintInfo, sizeof(SH_BPPRINT_INFO) );

	return 0;
}

//-----------------------------------------------------------------------------
// ScoreLink情報の取得 20240116
//-----------------------------------------------------------------------------
// 引数		isScoreLinkCall		:	ScoreLinkからの呼び出しか
//			ScoreLinkPdfNam		:	ScoreLinkで使用するpdf名
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
void CMainFrame::GetScoreLinkInfo( int& isScoreLinkCall, CString& ScoreLinkPdfNam )
{
	isScoreLinkCall = m_isScoreLinkCall;
	ScoreLinkPdfNam = m_ScoreLinkPdfNam;
}

//-----------------------------------------------------------------------------
// 一括印刷をエラー状態にセット
//-----------------------------------------------------------------------------
void CMainFrame::SetBpPrintErr()
{
	if( m_swBeForked ){
		m_swForkErr = 1;
	}
}


//-----------------------------------------------------------------------------
// 国税納付書
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CMainFrame::AccessShinPayment( CString	&errmes )
{
	CString filter;
	filter.Format( _T("NFSequ = %d"), m_SnSeq );

//	int		PaymentCnt = 0;
//	int		Lng = 0;

	// 申告書集計チェック
	if( ((m_SnHeadData.Sn_SKKBN==0x02)&&(m_SnHeadData.Sn_Syuusei&0x08)) || (m_SnHeadData.Sn_Sign4&0x81) ){
		;
	}
	else{
		if( m_SnHeadData.IsMiddleProvisional() == FALSE ){
			if( !(m_SnHeadData.SVsacq&0x01) ){
//				if( chk_dsp(4) ){
//					return 0;
//				}
			}
		}
	}

//	m_Kojin = (Snh.Sn_KOJIN&0x01);					// 個人を選択時は選択可
//	if( (Vol1.APNO&0xf0) == 0x10 )	m_Kojin = 1;	// 個人業種マスターは選択可

	CDBSyzAccess	*pDBSyzAcc = NULL;
	pDBSyzAcc = new CDBSyzAccess;
	if( pDBSyzAcc == NULL )	{
		errmes = _T("消費税拡張マスターが取得出来ませんでした。");
		return ERR;
	}
	// 管理部オープン
	if( pDBSyzAcc->ShinPayMentOpen(m_pZmSub->m_database, filter) == ERR ){
		errmes =  pDBSyzAcc->GetShinPayMentErrMsg();
		if( pDBSyzAcc ){
			delete pDBSyzAcc;
			pDBSyzAcc = NULL;
		}
		return ERR;
	}

	int SozokuFlg = 0;			// 相続人用の納付書画面を開くか
	int	sel_max = 0;			// 選択された相続人数
	CArray<int> SozokuNoAry;	// 相続人番号（syz_sozoku.Sn_NO）

	NOHFUSHO_INFO	TEMP_INFO[40];	// 相続人最大登録数40
	NOHFUSHO_DATA	TEMP_DATA[40];
	for( int i = 0; i < 40; i++ ){
		memset( &TEMP_INFO[i], '\0', sizeof(TEMP_INFO[i]) );
		memset( &TEMP_DATA[i], '\0', sizeof(TEMP_DATA[i]) );
	}

	// 個人・準確定か
	if( m_SnHeadData.IsSoleProprietor() && m_SnHeadData.Sn_Syuusei&0x10 ){

		// 相続人情報の取得
		if( GetActViewVer() == ID_VER_SYZ31 ){

			SozokuFlg = 1;

			CH31HyoView	*pH31View = NULL;
			pH31View = (CH31HyoView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd( 1 ));
			int sozoku_cnt = pH31View->GetSozokuInfo( &TEMP_INFO[0], &TEMP_DATA[0], &SozokuNoAry );
			if( sozoku_cnt == -1 ){
				// 付表6の入力データチェックで過不足が見つかった
				if( pDBSyzAcc ){
					delete pDBSyzAcc;
					pDBSyzAcc = NULL;
				}
				return 0;
			}
			if( sozoku_cnt == 0 ){
				long symd = 0, eymd = 0;
				pH31View->GetKazeiKikan( symd, eymd );
				if( eymd >= ICS_SH_INVOICE_DAY ){
					ICSMessageBox( "付表7に相続人情報が登録されていません。" );
				}
				else{
					ICSMessageBox( "付表6に相続人情報が登録されていません。" );
				}
				if( pDBSyzAcc ){
					delete pDBSyzAcc;
					pDBSyzAcc = NULL;
				}
				return 0;
			}
			else{
				// 相続人選択画面
				CSozokuSel	dlg;
				dlg.InitInfo( &TEMP_INFO[0], &TEMP_DATA[0], SozokuNoAry, sozoku_cnt );
				if( dlg.DoModal() == IDOK ){
					// 選択された相続人情報を再セット
					memset( TEMP_INFO, '\0', sizeof(TEMP_INFO) );
					memset( TEMP_DATA, '\0', sizeof(TEMP_DATA) );
					SozokuNoAry.RemoveAll();
					memmove( TEMP_INFO, dlg.SELECT_INFO, sizeof(TEMP_INFO) );
					memmove( TEMP_DATA, dlg.SELECT_DATA, sizeof(TEMP_DATA) );
					SozokuNoAry.Copy( dlg.m_SelectNoAry );
					sel_max = SozokuNoAry.GetCount();
				}
				else{
					if( pDBSyzAcc ){
						delete pDBSyzAcc;
						pDBSyzAcc = NULL;
					}
					return 0;
				}
			}
		}
	}

	// 相続人用の納付書画面を開く
	if( SozokuFlg ){

		NOHFUSHO_INFO	NF_INFO_SOZOKU[40];	// 相続人最大登録数40
		NOHFUSHO_DATA	NF_DATA_SOZOKU[40];
		for( int i = 0; i < 40; i++ ){
			memset( &NF_INFO_SOZOKU[i], '\0', sizeof(NF_INFO_SOZOKU[i]) );
			memset( &NF_DATA_SOZOKU[i], '\0', sizeof(NF_DATA_SOZOKU[i]) );
		}

		for( int idx = 0; idx < sel_max; idx++ ){

			// 相続人情報セット
			SetSozokuInfo( NF_INFO_SOZOKU[idx], NF_DATA_SOZOKU[idx], TEMP_INFO[idx], TEMP_DATA[idx] );

			// 納付書情報セット
			SetNOHFUSHO_INFO( NF_INFO_SOZOKU[idx], 0 );

			// 納付書データセット
			int sozoku_no = SozokuNoAry.GetAt( idx );
			SetNOHFUSHO_DATA( NF_DATA_SOZOKU[idx], pDBSyzAcc, sozoku_no+2 );	// NFLineの初期値1と分けたいため＋2する
		}

		// 納付書画面（複数ページ用）
		if( PayMent_DispEx( 11, &NF_INFO_SOZOKU[0], &NF_DATA_SOZOKU[0], this, sel_max ) == 0 ){	// 第一引数 11 →消費税申告書で相続人用画面を表示する

			int updateFlg = 0;
			int exsistFlg = 0;
			int sozoku_no = 0;
			int PaymentCnt = pDBSyzAcc->m_pShinPayment->GetRecordCount();

			for( int idx = 0; idx < sel_max; idx++ ){

				sozoku_no = SozokuNoAry.GetAt( idx ) + 2;	// NFLineの初期値1と分けたいため＋2する

				for( int rec = 0; rec < PaymentCnt; rec++ ){
					if( rec == 0 ){
						if( (pDBSyzAcc->m_pShinPayment->MoveFirst() == -1) || (pDBSyzAcc->m_pShinPayment->st == -1) ){
							break;
						}
					}
					else{
						if( (pDBSyzAcc->m_pShinPayment->MoveNext() == -1) || (pDBSyzAcc->m_pShinPayment->st == -1) ){
							break;
						}
					}
					// 更新
					if( pDBSyzAcc->m_pShinPayment->NFLine == sozoku_no ){
						UpdateShinPayment( pDBSyzAcc, NF_DATA_SOZOKU[idx], 0, sozoku_no );
						updateFlg = 1;
						break;
					}
					// NFLine=1のレコードも探す
					if( pDBSyzAcc->m_pShinPayment->NFLine == 1 ){
						exsistFlg = 1;
					}
				}
				// 新規追加
				if( !updateFlg ){
					UpdateShinPayment( pDBSyzAcc, NF_DATA_SOZOKU[idx], 1, sozoku_no );
				}
				updateFlg = 0;

				// NFLine=1(通常の納付書用レコード)の追加。古いプロで動かす場合のために追加しておく。
				// レコードの追加順に関してはOpen時にソートがかかると思うため考慮しない。
				if( !exsistFlg ){
					NOHFUSHO_DATA	NF_DATA;
					memset( &NF_DATA, '\0', sizeof(NOHFUSHO_DATA) );
					UpdateShinPayment( pDBSyzAcc, NF_DATA, 1, -1 );
					exsistFlg = 1;
				}
			}
		}
	}
	// 通常の納付書画面を開く
	else{
		NOHFUSHO_INFO	NF_INFO;
		NOHFUSHO_DATA	NF_DATA;
		memset( &NF_INFO, '\0', sizeof(NOHFUSHO_INFO) );
		memset( &NF_DATA, '\0', sizeof(NOHFUSHO_DATA) );

		// 納付書情報セット
		SetNOHFUSHO_INFO( NF_INFO, 1 );

		// 納付書データセット
		SetNOHFUSHO_DATA( NF_DATA, pDBSyzAcc );

		// 納付書画面
		if( PayMent_Disp( 3, &NF_INFO, &NF_DATA, this ) == 0 ){

			int updateFlg = 0;
			int PaymentCnt = pDBSyzAcc->m_pShinPayment->GetRecordCount();

			for( int rec = 0; rec < PaymentCnt; rec++ ){
				if( rec == 0 ){
					if( (pDBSyzAcc->m_pShinPayment->MoveFirst() == -1) || (pDBSyzAcc->m_pShinPayment->st == -1) ){
						break;
					}
				}
				else{
					if( (pDBSyzAcc->m_pShinPayment->MoveNext() == -1) || (pDBSyzAcc->m_pShinPayment->st == -1) ){
						break;
					}
				}
				// 更新
				if( pDBSyzAcc->m_pShinPayment->NFLine == 1 ){
					UpdateShinPayment( pDBSyzAcc, NF_DATA, 0 );
					updateFlg = 1;
					break;
				}
			}
			// 新規追加
			if( !updateFlg ){
				UpdateShinPayment( pDBSyzAcc, NF_DATA, 1 );
			}
		}
	}

	pDBSyzAcc->ShinPayMentClose();

	if( pDBSyzAcc ){
		delete pDBSyzAcc;
		pDBSyzAcc = NULL;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 国税納付書　NF_INFOへのデータセット
//-----------------------------------------------------------------------------
// 引数		nf_info	：	納付書情報
//			setflg	：	住所/氏名/電話番号をセットするか　0:しない / 1:する
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
void CMainFrame::SetNOHFUSHO_INFO( NOHFUSHO_INFO &nf_info, int setflg )
{
	char	buf[64] = {0};
	char	CoNo[4] = {0};
	sprintf_s( buf, sizeof(buf), "%08d", m_pZmSub->zvol->v_cod );
	m_Arith.l_pack( CoNo, buf, 8 );
	memmove( nf_info.NINFCOCODE, CoNo, 4 );		// 会社コード

	switch( m_SnHeadData.Sn_SKKBN ){			// 申告区分
		case 0x01 :	
			if( m_SnHeadData.Sn_Syuusei&0x02 ){	// 更正の請求
				nf_info.NINF_SKBN = 6;
			}
			else{								// 確定申告
				nf_info.NINF_SKBN = 4;
			}
			break;
		case 0x02 :								// 中間申告
			nf_info.NINF_SKBN = 3;
			break;
		case 0x03 :								// 修正確定
			nf_info.NINF_SKBN = 5;
			break;
		case 0x04 :								// 修正中間
			nf_info.NINF_SKBN = 5;	
			break;
		default   :								// その他
			nf_info.NINF_SKBN = 0;	
			break;
	}
	if( m_SnHeadData.Sn_TAXNO[0] != 'F' ){
		memmove( nf_info.ZEIMUSHO_CODE, m_SnHeadData.Sn_TAXNO, sizeof(m_SnHeadData.Sn_TAXNO) );		// 税務署番号
		memmove( nf_info.ZEIMUSHO_NAME, m_SnHeadData.Sn_ZMSYO, sizeof(m_SnHeadData.Sn_ZMSYO) );		// 税務署名
	}
	if( (m_SnHeadData.Sn_SEIRI[0]&0xff) != 0xff ){
		m_Arith.l_unpac( buf, m_SnHeadData.Sn_SEIRI, 8 );

		int len = (int)strlen(buf);
		while( len > 0 ) {
			if (buf[--len] != 0x20) {
				break;
			}
		}
		++len;
		char buf2[20] = {0};
		memmove( buf2, buf, len );
		memmove( nf_info.SEIRI_CODE, buf2, 8 );				// 整理番号
	}
//	nf_info.NOHKI_SYMD[0] = 0x04;							// 元号
//	m_SnHeadData.GetYmdData( ID_ICSSH_KAZEIKIKAN_FROM, &nf_info.NOHKI_SYMD[1], &nf_info.NOHKI_SYMD[2], &nf_info.NOHKI_SYMD[3] );
//	memmove( &nf_info.NOHKI_SYMD[1], Snh.Sn_KDAYS, 3 );		// 課税期間　自
	int g = 0;
	if( IsJcContinuePrint(&m_SnHeadData) ){
		nf_info.NOHKI_SYMD[0] = 0x04;						// 元号
		m_SnHeadData.GetYmdData( ID_ICSSH_KAZEIKIKAN_FROM, &nf_info.NOHKI_SYMD[1], &nf_info.NOHKI_SYMD[2], &nf_info.NOHKI_SYMD[3] );
	}
	else{
		g = 0;
		m_SnHeadData.GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &nf_info.NOHKI_SYMD[1], &nf_info.NOHKI_SYMD[2], &nf_info.NOHKI_SYMD[3], &g);
		m_Arith.int_bcd(&nf_info.NOHKI_SYMD[0], g, 1);
	}

//	nf_info.NOHKI_EYMD[0] = 0x04;							// 元号
//	m_SnHeadData.GetYmdData( ID_ICSSH_KAZEIKIKAN_TO, &nf_info.NOHKI_EYMD[1], &nf_info.NOHKI_EYMD[2], &nf_info.NOHKI_EYMD[3] );
//	memmove( &nf_info.NOHKI_EYMD[1], Snh.Sn_KDAYE, 3 );		// 課税期間　至
	if( IsJcContinuePrint(&m_SnHeadData) ){
		nf_info.NOHKI_EYMD[0] = 0x04;						// 元号
		m_SnHeadData.GetYmdData( ID_ICSSH_KAZEIKIKAN_TO, &nf_info.NOHKI_EYMD[1], &nf_info.NOHKI_EYMD[2], &nf_info.NOHKI_EYMD[3] );
	}
	else{
		g = 0;
		m_SnHeadData.GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &nf_info.NOHKI_EYMD[1], &nf_info.NOHKI_EYMD[2], &nf_info.NOHKI_EYMD[3], &g);
		m_Arith.int_bcd(&nf_info.NOHKI_EYMD[0], g, 1);
	}


	if( setflg ){
		int Lng = 0;
		memmove( nf_info.NINFZIPNO, m_SnHeadData.Sn_ZIP, 8 );			// 郵便番号
		Lng = m_Util.MixSpaceCutLength( (char *)m_SnHeadData.Sn_NOZEI, 40 );
		memmove( nf_info.NINFADDR1, m_SnHeadData.Sn_NOZEI, Lng );		// 納税地（上段）
		Lng = m_Util.MixSpaceCutLength( (char *)&m_SnHeadData.Sn_NOZEI[40], 40 );
		memmove( nf_info.NINFADDR2, &m_SnHeadData.Sn_NOZEI[40], Lng );	// 納税地（下段段）
		// 電話番号
		char	TelNo1[10] = {0};
		char	TelNo2[10] = {0};
		char	TelNo3[10] = {0};
		char	stel[64] = {0};
		char	ptel[64] = {0};
		int		len1, len2, len3;
		memmove( TelNo1, m_SnHeadData.Sn_TELNO1, 6 );
		memmove( TelNo2, m_SnHeadData.Sn_TELNO2, 4 );
		memmove( TelNo3, m_SnHeadData.Sn_TELNO3, 4 );
		m_Util.AnkSpaceCut( TelNo1, 6 );
		m_Util.AnkSpaceCut( TelNo2, 4 );
		m_Util.AnkSpaceCut( TelNo3, 4 );
		len1 = (int)strlen( TelNo1 );
		len2 = (int)strlen( TelNo2 );
		len3 = (int)strlen( TelNo3 );
		int i = 0;
		if( len1 || len2 || len3 ){
			if( !len1 ){
				strcat_s( ptel, sizeof(ptel), _T("    -") );
			}
			else{
				strcat_s( ptel, sizeof(ptel), TelNo1 );
				i = (int)strlen( ptel );
				ptel[i] = '-';
				ptel[i+1] = 0x00;
			}
			if( !len2 ){
				strcat_s( ptel, sizeof( ptel ), _T("    -") );
			}
			else{
				strcat_s( ptel, sizeof(ptel), TelNo2 );
				i = i = (int)strlen( ptel );
				ptel[i] = '-';
				ptel[i+1] = 0x00;
			}
			if( !len3 ){
				strcat_s( ptel, sizeof( ptel ), _T("    ") );
			}
			else{
				strcat_s( ptel, sizeof( ptel ), TelNo3 );
			}
			memmove( nf_info.NINFTELNO, ptel, strlen( ptel ) );
		}
		// 氏名（法人名）　及びフリガナ
		char	WKbuf1[128] = {0};
		char	WKbuf2[128] = {0};
		char	WKbuf3[128] = {0};
		if( m_SnHeadData.IsSoleProprietor() ){
			// 個人
			memmove( WKbuf1, m_SnHeadData.Sn_DIHYO, 40 );		// 代表者氏名
			memmove( WKbuf2, m_SnHeadData.Sn_DIFRI, 68 );		// 代表者氏名フリガナ
		}
		else{
			// 法人
			memmove( WKbuf1, m_SnHeadData.Sn_CONAM, 40 );		// 氏名又は屋号
			memmove( WKbuf2, m_SnHeadData.Sn_COFRI, 68 );		// 氏名又は屋号フリガナ
		}
		memmove( nf_info.NINFNAME, WKbuf1, 40 );				// 氏名（法人名）
		// 漢字文字を（ひらがな・カタカナ）をカタカナに変換
		m_Util.LetterAlter( WKbuf2, WKbuf3, 34 );
		Lng = m_Util.MixSpaceCutLength( (char *)WKbuf3, 34 );
		memmove( nf_info.NINFKANA, WKbuf3, Lng );				// 氏名（法人名）フリガナ
	}


//--> '15.06.27 INS START
	// 会社コード出力
	if( m_pZmSub->zvol->g_hsw&0x01 ){
		nf_info.INP_MST_FLG = 2;
	}
	else{
		nf_info.INP_MST_FLG = 1;
	}

	EnumIdIcsShRevType	tmpShRevType=ID_ICSSH_NO_REVTYPE;
	CDBSyzShinMainView	*pView = (CDBSyzShinMainView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(BSINFO_VIEW_WND_IDX));
	if( pView ){
		tmpShRevType = pView->GetShRevType();
	}

	// 法人番号出力
	nf_info.INP_NO_FLG = 0;
	if( tmpShRevType >= ID_ICSSH_MYNUMBER_REVTYPE ){
		if( (m_pZmSub->zvol->ind_type==0x00) && (m_pZmSub->zvol->apno!=0x10) ){
			if( m_pZmSub->zvol->g_hsw&0x08 ){
				nf_info.INP_NO_FLG = 1;
			}
			else{
				nf_info.INP_NO_FLG = 2;
			}
		}
	}
	// 顧問先固定されている時の法人番号は？
	// 法人番号
	memmove( nf_info.HO_NO_MAIN, m_pZmSub->zvol->hojin_no.GetBuffer(), m_pZmSub->zvol->hojin_no.GetLength() );
//<-- '15.06.27 INS END

}

//-----------------------------------------------------------------------------
// 国税納付書　NF_DATAへのデータセット
//-----------------------------------------------------------------------------
// 引数		nf_data		：	納付書データ
//			pDBSyzAcc	：	テーブルアクセスクラス
//			sozoku_no	：	相続人番号（syz_sozoku.Sn_NOに+2したもの）
//						：	-1の場合は相続人データをセットしない
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CMainFrame::SetNOHFUSHO_DATA( NOHFUSHO_DATA &nf_data, CDBSyzAccess *pDBSyzAcc, int sozoku_no/*=-1*/ )
{
	CString	tag, chtag;
	SH_ANS	ans[30] = {0};

	if( m_SnHeadData.IsMiddleProvisional() ){
		// 予定２６号様式
		char	mony[MONY_BUF_SIZE]={0};
		tag = _T("Sn_KTNFZ");
		chtag.Empty();
		GetMony( tag, chtag, mony );
		if( m_Arith.l_test(mony) > 0 ){
			memmove( nf_data.HONZEI, mony, 6 );
		}
		else{
			memset( nf_data.HONZEI, '\0', 6 );
		}
	}
	else{
		// 消費税及び地方消費税の合計計算
		char	Chk = 0x00;
		char	Dmm = 0x00;
		char	Xymd[4] = { 0x09, 0x04, 0x01, 0x00 };
		// 消費税集計
		int		N_ver = 0x02;
		if( Dmm = (N_ver&0xff) ) {		// バージョンチェック
			switch( m_SnHeadData.Sn_SKKBN&0xff ) {
				case 1 :
				case 3 :		// 確定・確定修正
//					if( m_Arith.l_jsgn( Snh.Sn_KDAYE, Xymd, 3 ) < 0 ){
//						Dmm = 0; // H9.4ﾌｸﾑ
//					}
					break;
				default:				//中間・中間修正
//					if( m_Arith.l_jsgn( Snh.Sn_MDAYE, Xymd, 3 ) < 0 ){
//						Dmm = 0; // H9.4ﾌｸﾑ
//					}
					break;
			}
		}
		switch( Dmm ){
			case 1 :
				if( !m_SnHeadData.SVmzsw ){
					Dmm = 2;
				}
				break;
			case 2 :
				if( m_SnHeadData.SVmzsw ){
					Dmm = 1;
				}
				break;
			default:
				break;
		}
//		if( stm_pcalq( Dmm, N_ver ) ){
//			errmes = _T("消費税集計中に障害が発生し集計できませんでした。");
//			pDBSyzAcc->ShinPayMentClose();
//			delete( pDBSyzAcc );
//			return( ERR );
//		}

		// 金額の取得
		// 相続人の納付書を出す場合はSetSozokuInfoでセットするため、通常の納付書のときのみ通す
		if( sozoku_no == -1 ){
			if( GetActViewVer() == ID_VER_SYZ31 ){
				tag = _T("AAK00130");
			}
			else{
				tag = _T("AAC00130");
			}
			chtag.Empty();
			GetMony( tag, chtag, (char*)ans[25].val );

			// 本税
			if( m_Arith.l_test(ans[25].val) > 0 ){
				memmove( nf_data.HONZEI, ans[25].val, 6 );
			}
			else{
				memset( nf_data.HONZEI, '\0', 6 );
			}
		}
	}

#ifdef	_DEBUG
	char mny[20]= {0};
	m_Arith.l_print( mny, nf_data.HONZEI, "SSS,SSS,SSS,SSS" );
	TRACE( "消費税及び地方消費税の合計[ %s ]", mny );
#endif
	
	int PaymentCnt = pDBSyzAcc->m_pShinPayment->GetRecordCount();
	if( PaymentCnt ){

		int find = 0;

		for( int i = 0; i < PaymentCnt; i++ ){
			if( i == 0 ){
				if( (pDBSyzAcc->m_pShinPayment->MoveFirst() == -1) || (pDBSyzAcc->m_pShinPayment->st == -1) ){
					break;
				}
			}
			else{
				if( (pDBSyzAcc->m_pShinPayment->MoveNext() == -1) || (pDBSyzAcc->m_pShinPayment->st == -1) ){
					break;
				}
			}

			if( sozoku_no != -1 ){
				// 相続人情報で納付書を開く場合は相続人Noと一致するものを探す
				if( pDBSyzAcc->m_pShinPayment->NFLine == sozoku_no ){
					find = 1;
				}
			}
			else{
				// 通常のデータで納付書を開く場合は1で探す
				if( pDBSyzAcc->m_pShinPayment->NFLine == 1 ){
					find = 1;
				}
			}

			if( find ){
				// 保存データをセット
				nf_data.KASNZEI_KBN = pDBSyzAcc->m_pShinPayment->NFKskb;											// 加算税区分
				if( m_SnHeadData.IsSoleProprietor() ){
					nf_data.SKBN_NOFLG = -1;																		//「申告区分を「該当なし」にする」チェックボックスの増加
				}
				memmove( nf_data.SINTAKU_NAME, pDBSyzAcc->m_pShinPayment->NFStnm, pDBSyzAcc->m_pShinPayment->NFStnm.GetLength() );
				nf_data.JG_FLG = pDBSyzAcc->m_pShinPayment->NFJflg;													// 実額フラグ
				m_Util.val_to_bin( (unsigned char *)nf_data.JUKASANZEI,	pDBSyzAcc->m_pShinPayment->NFJval );		// 重加算税
				m_Util.val_to_bin( (unsigned char *)nf_data.KASANZEI, pDBSyzAcc->m_pShinPayment->NFKval );			// 加算税
				m_Util.val_to_bin( (unsigned char *)nf_data.RISHIZEI, pDBSyzAcc->m_pShinPayment->NFRval );			// 利子税
				m_Util.val_to_bin( (unsigned char *)nf_data.ENTAIZEI, pDBSyzAcc->m_pShinPayment->NFEval );			// 延滞税
				m_Util.val_to_bin( (unsigned char *)nf_data.GOHKEI,	pDBSyzAcc->m_pShinPayment->NFGval );			// 合計
				m_Util.val_to_bin( (unsigned char *)nf_data.HONZEI_JG, pDBSyzAcc->m_pShinPayment->NFHjvl );			// 本税実額

				nf_data.NINFNEND[0] = pDBSyzAcc->m_pShinPayment->NFSign[0];											// 元号
				nf_data.NINFNEND[1] = pDBSyzAcc->m_pShinPayment->NFSign[1];											// 年度
				nf_data.KANRI_NO[0] = pDBSyzAcc->m_pShinPayment->NFSign[2];											// 国税管理番号
				nf_data.KANRI_NO[1] = pDBSyzAcc->m_pShinPayment->NFSign[3];											// 〃
				nf_data.KANRI_NO[2] = pDBSyzAcc->m_pShinPayment->NFSign[4];											// 〃
				nf_data.KANRI_NO[3] = pDBSyzAcc->m_pShinPayment->NFSign[5];											// 〃
				nf_data.KANRI_NO[4] = pDBSyzAcc->m_pShinPayment->NFSign[6];											// 〃
				nf_data.KANRI_NO[5] = pDBSyzAcc->m_pShinPayment->NFSign[7];											// 〃
				nf_data.KANRI_NO[6] = pDBSyzAcc->m_pShinPayment->NFSign[8];											// 〃
				nf_data.KANRI_NO[7] = pDBSyzAcc->m_pShinPayment->NFSign[9];											// 〃
				nf_data.KANRI_NO[8] = pDBSyzAcc->m_pShinPayment->NFSign[10];										// 〃
				nf_data.KANRI_NO[9] = pDBSyzAcc->m_pShinPayment->NFSign[11];										// 〃
				nf_data.yousi_flg		= pDBSyzAcc->m_pShinPayment->NFSign[12];									//用紙　0---KSK 1---imfカラー　2---imfモノクロ　3---控
				nf_data.mst_nam_flg		= pDBSyzAcc->m_pShinPayment->NFSign[13];									//以下	0---出力する　１---出力しない
				nf_data.nendo_flg		= pDBSyzAcc->m_pShinPayment->NFSign[14];	  								//年度
				nf_data.zeimoku_no_flg	= pDBSyzAcc->m_pShinPayment->NFSign[15];									//税目番号
				nf_data.zeimu_nam_flg	= pDBSyzAcc->m_pShinPayment->NFSign[16];									//税務署名
				nf_data.zeimu_no_flg	= pDBSyzAcc->m_pShinPayment->NFSign[17];									//税務署番号
				nf_data.seiri_no_flg	= pDBSyzAcc->m_pShinPayment->NFSign[18];									//整理番号
				nf_data.zeimoku_nam_flg	= pDBSyzAcc->m_pShinPayment->NFSign[19];									//税目
				nf_data.sintaku_nam_flg	= pDBSyzAcc->m_pShinPayment->NFSign[20];									//信託の名称
				nf_data.tel_flg			= pDBSyzAcc->m_pShinPayment->NFSign[21];									//電話番号
				nf_data.add_flg			= pDBSyzAcc->m_pShinPayment->NFSign[22];									//住所
				nf_data.nam_flg			= pDBSyzAcc->m_pShinPayment->NFSign[23];									//氏名
				nf_data.honzei_flg		= pDBSyzAcc->m_pShinPayment->NFSign[24];									//本税
				nf_data.jukasanzei_flg	= pDBSyzAcc->m_pShinPayment->NFSign[25];									//重加算税
				nf_data.gaitou_no_flg	= pDBSyzAcc->m_pShinPayment->NFSign[26];									//該当項目
				nf_data.kasanzei_flg	= pDBSyzAcc->m_pShinPayment->NFSign[27];									//加算税
				nf_data.risizei_flg		= pDBSyzAcc->m_pShinPayment->NFSign[28];									//利子税
				nf_data.entaizei_flg	= pDBSyzAcc->m_pShinPayment->NFSign[29];									//延滞税
				nf_data.goukei_flg		= pDBSyzAcc->m_pShinPayment->NFSign[30];									//合計額
				nf_data.nouki_flg		= pDBSyzAcc->m_pShinPayment->NFSign[31];									//納期等の区分(年月日)
				nf_data.kbn_no_flg		= pDBSyzAcc->m_pShinPayment->NFSign[32];									//申告区分
				nf_data.kanri_no_flg	= pDBSyzAcc->m_pShinPayment->NFSign[33];									//日銀管理番号//10.11 '12

				memset( &m_SvPayment, '\0', sizeof(SH_PAYMENT) );
				m_SvPayment.PM_SKKBN    = pDBSyzAcc->m_pShinPayment->NFSign[34];									// 申告区分
				m_SvPayment.PM_KZTNSK   = pDBSyzAcc->m_pShinPayment->NFSign[35];									// 課税期間短縮
				m_SvPayment.PM_MCOUNT   = pDBSyzAcc->m_pShinPayment->NFSign[36];									// 中間申告回数
				m_SvPayment.PM_Syuusei  = pDBSyzAcc->m_pShinPayment->NFSign[37];									// 更正の請求
				m_SvPayment.PM_KDAYS[0] = pDBSyzAcc->m_pShinPayment->NFSign[38];									// 課税期間 自　年
				m_SvPayment.PM_KDAYS[1] = pDBSyzAcc->m_pShinPayment->NFSign[39];									// 〃　　　　　 月
				m_SvPayment.PM_KDAYS[2] = pDBSyzAcc->m_pShinPayment->NFSign[40];									// 〃           日
				m_SvPayment.PM_KDAYE[0] = pDBSyzAcc->m_pShinPayment->NFSign[41];									// 〃　　　 至  年　
				m_SvPayment.PM_KDAYE[1] = pDBSyzAcc->m_pShinPayment->NFSign[42];									// 〃　　　　　 月
				m_SvPayment.PM_KDAYE[2] = pDBSyzAcc->m_pShinPayment->NFSign[43];									// 〃　　　　　 日
				m_SvPayment.PM_MDAYS[0] = pDBSyzAcc->m_pShinPayment->NFSign[44];									// 中間期間 自　年
				m_SvPayment.PM_MDAYS[1] = pDBSyzAcc->m_pShinPayment->NFSign[45];									// 〃	   　　 月
				m_SvPayment.PM_MDAYS[2] = pDBSyzAcc->m_pShinPayment->NFSign[46];									// 〃	        日
				m_SvPayment.PM_MDAYE[0] = pDBSyzAcc->m_pShinPayment->NFSign[47];									// 〃　　　 至  年　
				m_SvPayment.PM_MDAYE[1] = pDBSyzAcc->m_pShinPayment->NFSign[48];									// 〃　　　　　 月
				m_SvPayment.PM_MDAYE[2] = pDBSyzAcc->m_pShinPayment->NFSign[49];									// 〃　　　　　 日
				break;
			}
		}
	}

	// 年度変換
	int		Flg = 0;
	char	EYMD[3] = {0};
	int		EG = 0;

	if( !(m_SnHeadData.Sn_SKKBN%2) ){
		// 今回中間
		if( m_SvPayment.PM_SKKBN != m_SnHeadData.Sn_SKKBN ){
			Flg = 1;							// 申告区分が違う
		}
		if( m_SvPayment.PM_MCOUNT != m_SnHeadData.Sn_MCOUNT ){
			Flg = 1;							// 中間申告回数が違う
		}
//		if( l_jsgn( Snh.Sn_MDAYE, m_SvPayment.PM_MDAYE, 3 ) != 0 )			Flg = 1;						// 中間期間の至が違う
//		memmove( EYMD, Snh.Sn_MDAYE, 3 );																	// 今回 中間申告至
//		m_SnHeadData.GetYmdData( ID_ICSSH_CHUUKAN_TO, &EYMD[0], &EYMD[1], &EYMD[2] );
		if( IsJcContinuePrint(&m_SnHeadData) ){
			EG = 4;
			m_SnHeadData.GetYmdData( ID_ICSSH_CHUUKAN_TO, &EYMD[0], &EYMD[1], &EYMD[2] );
		}
		else{
			m_SnHeadData.GetYmdDataGen(ID_ICSSH_CHUUKAN_TO, &EYMD[0], &EYMD[1], &EYMD[2], &EG);
		}
	}
	else{
		// 今回確定
		if( m_SvPayment.PM_SKKBN != m_SnHeadData.Sn_SKKBN ){
			Flg = 1;							// 申告区分が違う
		}
		if( m_SvPayment.PM_KZTNSK != m_SnHeadData.Sn_KZTNSK ){
			Flg = 1;							// 期間短縮回数等が違う
		}
//		if( l_jsgn( Snh.Sn_KDAYE, m_SvPayment.PM_KDAYE, 3 ) != 0 ){
//			Flg = 1;							// 課税期間の至が違う
//		}
		if( (m_SvPayment.PM_Syuusei&0x02) != (m_SnHeadData.Sn_Syuusei&0x02) ){
			Flg = 1;							// 更正の請求
		}
//		memmove( EYMD, Snh.Sn_KDAYE, 3 );																	// 今回 課税期間至 
//		m_SnHeadData.GetYmdData( ID_ICSSH_KAZEIKIKAN_TO, &EYMD[0], &EYMD[1], &EYMD[2] );
		if( IsJcContinuePrint(&m_SnHeadData) ){
			EG = 4;
			m_SnHeadData.GetYmdData( ID_ICSSH_KAZEIKIKAN_TO, &EYMD[0], &EYMD[1], &EYMD[2] );
		}
		else{
			m_SnHeadData.GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &EYMD[0], &EYMD[1], &EYMD[2], &EG);
		}
	}
	if( !(nf_data.NINFNEND[0]&0xff) && !(nf_data.NINFNEND[1]&0xff) ){
		Flg = 1;							// 年度が初期
	}

	int		Add = 2;
	char	snkdays[3]={0}, snmdays[3]={0};
	m_SnHeadData.GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, &snkdays[0], &snkdays[1], &snkdays[2] );
	m_SnHeadData.GetYmdDataGen( ID_ICSSH_CHUUKAN_FROM, &snmdays[0], &snmdays[1], &snmdays[2] );
	if( !(m_SnHeadData.Sn_SKKBN%2) && (m_SnHeadData.Sn_MCOUNT==11) && (m_Arith.l_jsgn(snkdays, snmdays, 3)==0) ){
		if( m_SnHeadData.IsSoleProprietor() ){
			Add = 4;
		}
		else{
			Add = 3;
		}
	}
	m_Arith.int_bcd( &nf_data.NINFNEND[0], EG, 1 );															// 年度	[0]:元号
	nf_data.NINFNEND[1] = GetNohusyoNendo( Add, (unsigned char *) &EYMD[0] );								//		[1]：年

	if( Flg ){
		//int		Add = 2;
		//char	snkdays[3]={0}, snmdays[3]={0};
		//m_SnHeadData.GetYmdData( ID_ICSSH_KAZEIKIKAN_FROM, &snkdays[0], &snkdays[1], &snkdays[2] );
		//m_SnHeadData.GetYmdData( ID_ICSSH_CHUUKAN_FROM, &snmdays[0], &snmdays[1], &snmdays[2] );
		//if( !(m_SnHeadData.Sn_SKKBN%2) && (m_SnHeadData.Sn_MCOUNT==11) && (m_Arith.l_jsgn(snkdays, snmdays, 3)==0) ){
		//	if( m_SnHeadData.IsSoleProprietor() ){
		//		Add = 4;
		//	}
		//	else{
		//		Add = 3;
		//	}
		//}
		//nf_data.NINFNEND[0] = 4;																	// 年度	[0]:元号
		//nf_data.NINFNEND[1] = GetNohusyoNendo( Add, (unsigned char *) &EYMD[0] );					//		[1]：年

		// 初期設定
		nf_data.KASNZEI_KBN = 0;																	// 加算税区分
		if( m_SnHeadData.IsSoleProprietor() ){
			nf_data.SKBN_NOFLG = -1;																//「申告区分を「該当なし」にする」チェックボックスの増加
		}
		nf_data.JG_FLG = 0;																			// 実額フラグ
		memset( nf_data.JUKASANZEI, '\0', 6 );														// 重加算税
		memset( nf_data.KASANZEI, '\0', 6 );														// 加算税
		memset( nf_data.RISHIZEI, '\0', 6 );														// 利子税
		memset( nf_data.ENTAIZEI, '\0', 6 );														// 延滞税
		memset( nf_data.GOHKEI, '\0', 6 );															// 合計
		memset( nf_data.HONZEI_JG, '\0', 6 );														// 本税実額
	}
}

//-----------------------------------------------------------------------------
// nf_info / nf_data に相続人情報をセット
//-----------------------------------------------------------------------------
// 引数		nf_info		:	納付書情報
//			nf_data		:	納付書データ
//			sozokuinfo	:	相続人情報
//			sozokudata	:	相続人データ
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
void CMainFrame::SetSozokuInfo( NOHFUSHO_INFO &nf_info, NOHFUSHO_DATA &nf_data, NOHFUSHO_INFO &sozokuinfo, NOHFUSHO_DATA &sozokudata )
{
	/** 相続人情報 **/
	// 住所
	memmove( nf_info.NINFADDR2_1, sozokuinfo.NINFADDR2_1, sizeof(nf_info.NINFADDR2_1) );
	memmove( nf_info.NINFADDR2_2, sozokuinfo.NINFADDR2_2, sizeof(nf_info.NINFADDR2_2) );
	
	// 電話番号
	memmove( nf_info.NINFTELNO, sozokuinfo.NINFTELNO, sizeof(nf_info.NINFTELNO) );

	// 氏名
	memmove( nf_info.NINFNAME2, sozokuinfo.NINFNAME2, sizeof(nf_info.NINFNAME2) );
	memmove( nf_info.NINFNAME_TITLE, sozokuinfo.NINFNAME_TITLE, sizeof(nf_info.NINFNAME_TITLE) );

	// カナ
	memmove( nf_info.NINFKANA, sozokuinfo.NINFKANA, sizeof(nf_info.NINFKANA) );

	// 本税
	memmove( nf_data.HONZEI, sozokudata.HONZEI, sizeof(nf_data.HONZEI) );


	/** 被相続人情報 **/
	// 住所
	memmove( nf_info.NINFADDR1, sozokuinfo.NINFADDR1, sizeof(nf_info.NINFADDR1) );
	memmove( nf_info.NINFADDR2, sozokuinfo.NINFADDR2, sizeof(nf_info.NINFADDR2) );

	// 氏名
	memmove( nf_info.NINFNAME, sozokuinfo.NINFNAME, sizeof(nf_info.NINFNAME) );
}

//-----------------------------------------------------------------------------
// 国税納付書　ShinPaymentテーブルの更新
//-----------------------------------------------------------------------------
// 引数		pDBSyzAcc	:	テーブルアクセスクラス
//			nf_data		:	納付書データ
//			update_type	:	0：更新 / 1：新規追加
//			sozoku_no	:	相続人番号（syz_sozoku.Sn_NOに+2したもの）
//						:	-1の場合は相続人番号のセットはしない
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
void CMainFrame::UpdateShinPayment( CDBSyzAccess *pDBSyzAcc, NOHFUSHO_DATA &nf_data, int update_type, int sozoku_no/*=-1*/ )
{
	if( update_type ){
		pDBSyzAcc->m_pShinPayment->AddNew();
	}
	else{
		pDBSyzAcc->m_pShinPayment->Edit();
	}
	
	pDBSyzAcc->m_pShinPayment->NFItem = 1;																// アイテム番号 1=納付書
	if( sozoku_no == -1 ){
		pDBSyzAcc->m_pShinPayment->NFLine = 1;															// 行番号
	}
	else{
		pDBSyzAcc->m_pShinPayment->NFLine = sozoku_no;													// 相続人番号（syz_sozoku.Sn_NOに+2したもの）
	}
	pDBSyzAcc->m_pShinPayment->NFSequ = m_SnSeq;														// 保存番号
	pDBSyzAcc->m_pShinPayment->NFKskb = nf_data.KASNZEI_KBN;											// 加算税区分
	if( m_SnHeadData.IsSoleProprietor() ){
		pDBSyzAcc->m_pShinPayment->NFKsdm = -1;															//「申告区分を「該当なし」にする」チェックボックスの増加
	}
	pDBSyzAcc->m_pShinPayment->NFStnm = nf_data.SINTAKU_NAME;											// 信託の名称
	pDBSyzAcc->m_pShinPayment->NFJflg = nf_data.JG_FLG;													// 実額フラグ
	m_Util.val_to_asci( &pDBSyzAcc->m_pShinPayment->NFJval, (unsigned char *)nf_data.JUKASANZEI );		// 重加算税
	m_Util.val_to_asci( &pDBSyzAcc->m_pShinPayment->NFKval, (unsigned char *)nf_data.KASANZEI );		// 加算税
	m_Util.val_to_asci( &pDBSyzAcc->m_pShinPayment->NFRval, (unsigned char *)nf_data.RISHIZEI );		// 利子税
	m_Util.val_to_asci( &pDBSyzAcc->m_pShinPayment->NFEval, (unsigned char *)nf_data.ENTAIZEI );		// 延滞税
	m_Util.val_to_asci( &pDBSyzAcc->m_pShinPayment->NFGval, (unsigned char *)nf_data.GOHKEI );			// 合計
	m_Util.val_to_asci( &pDBSyzAcc->m_pShinPayment->NFHjvl, (unsigned char *)nf_data.HONZEI_JG );		// 本税実額
	pDBSyzAcc->m_pShinPayment->NFSign[0] = nf_data.NINFNEND[0];											// 元号
	pDBSyzAcc->m_pShinPayment->NFSign[1] = nf_data.NINFNEND[1];											// 年度
	pDBSyzAcc->m_pShinPayment->NFSign[2] = nf_data.KANRI_NO[0];											// 国税管理番号
	pDBSyzAcc->m_pShinPayment->NFSign[3] = nf_data.KANRI_NO[1];											// 〃
	pDBSyzAcc->m_pShinPayment->NFSign[4] = nf_data.KANRI_NO[2];											// 〃
	pDBSyzAcc->m_pShinPayment->NFSign[5] = nf_data.KANRI_NO[3];											// 〃
	pDBSyzAcc->m_pShinPayment->NFSign[6] = nf_data.KANRI_NO[4];											// 〃
	pDBSyzAcc->m_pShinPayment->NFSign[7] = nf_data.KANRI_NO[5];											// 〃
	pDBSyzAcc->m_pShinPayment->NFSign[8] = nf_data.KANRI_NO[6];											// 〃
	pDBSyzAcc->m_pShinPayment->NFSign[9] = nf_data.KANRI_NO[7];											// 〃
	pDBSyzAcc->m_pShinPayment->NFSign[10] = nf_data.KANRI_NO[8];										// 〃
	pDBSyzAcc->m_pShinPayment->NFSign[11] = nf_data.KANRI_NO[9];										// 〃
	pDBSyzAcc->m_pShinPayment->NFSign[12] = nf_data.yousi_flg;											// 用紙　0---KSK 1---imfカラー　2---imfモノクロ　3---控 
	pDBSyzAcc->m_pShinPayment->NFSign[13] = nf_data.mst_nam_flg;										// 以下	0---出力する　１---出力しない
	pDBSyzAcc->m_pShinPayment->NFSign[14] = nf_data.nendo_flg;	  										// 年度
	pDBSyzAcc->m_pShinPayment->NFSign[15] = nf_data.zeimoku_no_flg;										// 税目番号
	pDBSyzAcc->m_pShinPayment->NFSign[16] = nf_data.zeimu_nam_flg;										// 税務署名
	pDBSyzAcc->m_pShinPayment->NFSign[17] = nf_data.zeimu_no_flg;										// 税務署番号
	pDBSyzAcc->m_pShinPayment->NFSign[18] = nf_data.seiri_no_flg;										// 整理番号
	pDBSyzAcc->m_pShinPayment->NFSign[19] = nf_data.zeimoku_nam_flg;									// 税目
	pDBSyzAcc->m_pShinPayment->NFSign[20] = nf_data.sintaku_nam_flg;									// 信託の名称
	pDBSyzAcc->m_pShinPayment->NFSign[21] = nf_data.tel_flg;											// 電話番号
	pDBSyzAcc->m_pShinPayment->NFSign[22] = nf_data.add_flg;											// 住所
	pDBSyzAcc->m_pShinPayment->NFSign[23] = nf_data.nam_flg;											// 氏名
	pDBSyzAcc->m_pShinPayment->NFSign[24] = nf_data.honzei_flg;											// 本税
	pDBSyzAcc->m_pShinPayment->NFSign[25] = nf_data.jukasanzei_flg;										// 重加算税
	pDBSyzAcc->m_pShinPayment->NFSign[26] = nf_data.gaitou_no_flg;										// 該当項目
	pDBSyzAcc->m_pShinPayment->NFSign[27] = nf_data.kasanzei_flg;										// 加算税
	pDBSyzAcc->m_pShinPayment->NFSign[28] = nf_data.risizei_flg;										// 利子税
	pDBSyzAcc->m_pShinPayment->NFSign[29] = nf_data.entaizei_flg;										// 延滞税
	pDBSyzAcc->m_pShinPayment->NFSign[30] = nf_data.goukei_flg;											// 合計額
	pDBSyzAcc->m_pShinPayment->NFSign[31] = nf_data.nouki_flg;											// 納期等の区分(年月日)
	pDBSyzAcc->m_pShinPayment->NFSign[32] = nf_data.kbn_no_flg;											// 申告区分
	pDBSyzAcc->m_pShinPayment->NFSign[33] = nf_data.kanri_no_flg;										// 日銀管理番号//10.11 '12					  						
	pDBSyzAcc->m_pShinPayment->NFSign[34] = m_SnHeadData.Sn_SKKBN;										// 申告区分
	pDBSyzAcc->m_pShinPayment->NFSign[35] = m_SnHeadData.Sn_KZTNSK;										// 課税期間短縮
	pDBSyzAcc->m_pShinPayment->NFSign[36] = m_SnHeadData.Sn_MCOUNT;										// 中間申告回数
	pDBSyzAcc->m_pShinPayment->NFSign[37] = m_SnHeadData.Sn_Syuusei;									// 更正の請求

	// 課税期間 自
	m_SnHeadData.GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, (char*)&pDBSyzAcc->m_pShinPayment->NFSign[38], (char*)&pDBSyzAcc->m_pShinPayment->NFSign[39], (char*)&pDBSyzAcc->m_pShinPayment->NFSign[40] );
	// 課税期間 至
	m_SnHeadData.GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, (char*)&pDBSyzAcc->m_pShinPayment->NFSign[41], (char*)&pDBSyzAcc->m_pShinPayment->NFSign[42], (char*)&pDBSyzAcc->m_pShinPayment->NFSign[43] );
	// 中間期間 自
	m_SnHeadData.GetYmdDataGen( ID_ICSSH_CHUUKAN_FROM, (char*)&pDBSyzAcc->m_pShinPayment->NFSign[44], (char*)&pDBSyzAcc->m_pShinPayment->NFSign[45], (char*)&pDBSyzAcc->m_pShinPayment->NFSign[46] );
	// 中間期間 至
	m_SnHeadData.GetYmdDataGen( ID_ICSSH_CHUUKAN_TO, (char*)&pDBSyzAcc->m_pShinPayment->NFSign[47], (char*)&pDBSyzAcc->m_pShinPayment->NFSign[48], (char*)&pDBSyzAcc->m_pShinPayment->NFSign[49] );


	pDBSyzAcc->m_pShinPayment->Update();
}

//-----------------------------------------------------------------------------
// 納付書年度判定関数
//-----------------------------------------------------------------------------
//
//
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
unsigned char CMainFrame::GetNohusyoNendo( int ADD, unsigned char EYMD[] )
{
	char Nen[4] = {0};
	char buf[12] = {0};
	int  year = 0, month = 0;

	Nen[0] = EYMD[0];
	Nen[1] = EYMD[1];

	wsprintf( buf, _T("%x"), Nen[0]&0xff );
	year = atoi( buf );
	wsprintf( buf, _T("%x"), Nen[1]&0xff );
	month = atoi( buf );

	month += ADD;
	if( month > 12 ){	// 11月、12月は年をまたぐ。
		month -= 12;
		year++;
	}

	if( month < 4 ){
		year--;
	}

	wsprintf( buf, _T("%d"), year);
	Nen[0] = _atoh( buf );

	return Nen[0];
}

void CMainFrame::OnShinPayment()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	CString	ErMes;
	ErMes.Empty();
//	if( m_TabView == 1 && pSheetView )	((CSyzShinSheet*)pSheetView)->SheetUpDateEx();		
//	int seq = ((CDBSyzShinView *)HomeView)->IsHistoryMode() ?  ((CDBSyzShinView *)HomeView)->GetHistorySeq() : 0;

	if( m_nowTabIdx == 1 ){
		CBaseHyoView *pBaseView = GetHonHyoView();
		if( pBaseView != NULL ){
			pBaseView->CheckInvno( 2 );
			pBaseView->UpdateSheetFromOuter();
		}
	}

	if( AccessShinPayment(ErMes) == ERR ){
		ICSMessageBox( ErMes );
	}
}

//-----------------------------------------------------------------------------
// 再集計
//-----------------------------------------------------------------------------
LRESULT CMainFrame::OnUserRecalq( WPARAM wParam, LPARAM lParam ) 
{
	BOOL bMiddleCalq = FALSE;
	if( wParam == 1 ){
		bMiddleCalq = TRUE;
	}

	ReStartSyzSyukei( bMiddleCalq );

	return 0;
}

//-----------------------------------------------------------------------------
// メニュー更新
//-----------------------------------------------------------------------------
LRESULT CMainFrame::OnUserUpdateIndicate( WPARAM wParam, LPARAM lParam )
{
	UpdateLockIndicator( m_SnHeadData.Sn_Sign4&0x01 );

	return 0;
}

//-----------------------------------------------------------------------------
// フォーカスの初期値セット harino ('14.03.09)
//-----------------------------------------------------------------------------
LRESULT CMainFrame::OnUserSetFocus( WPARAM wParam, LPARAM lParam )
{
	if( m_pMainView ){
		m_pMainView->SetDefFocus();
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 仕入控除税額との連動
//-----------------------------------------------------------------------------
// 引数	isUpdDisp	：	画面の更新をするかどうか
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
int CMainFrame::ConnectedSkj( bool isUpdDisp )
{
	CBaseHyoView *pBaseView = GetHonHyoView();
	if( pBaseView != NULL ){
//-- '15.09.30 --
//		pH26View->ConnectedSkj();
//---------------
		pBaseView->ConnectedSkj( m_SvRevMoneyArray, isUpdDisp );
//---------------
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 仕入控除税額のみとの連動
//-----------------------------------------------------------------------------
// 引数	isUpdDisp	：	画面の更新をするかどうか
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
int CMainFrame::ConnectedSkjZeigaku( bool isUpdDisp )
{
	CBaseHyoView *pBaseView = GetHonHyoView();
	if( pBaseView != NULL ){
		pBaseView->ConnectedSkjZeigaku(isUpdDisp);
	}

	return 0;
}

//2016.02.23 INSERT START
//-----------------------------------------------------------------------------
// 仕入金額等の明細のクリア
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CMainFrame::ClearShiire()
{
	CBaseHyoView *pBaseView = GetHonHyoView();
	if( pBaseView != NULL ){
		pBaseView->ClearShiire();
	}

	return 0;
}
//2016.02.23 INSERT END
//-----------------------------------------------------------------------------
// 課税取引金額計算書との連動（'14.08.08）
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CMainFrame::ConnectedTaxList()
{
	CBaseTaxHyoView *pBaseTaxView = GetTaxListView();
	if( pBaseTaxView != NULL ){
		pBaseTaxView->ConnectedTaxList();
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 特定収入計算書との連動（'15.02.13）
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CMainFrame::ConnectedSpcList()
{
	CBaseTaxHyoView *pBaseTaxView = GetTaxListView();
	if( pBaseTaxView != NULL ){
		pBaseTaxView->ConnectedSpcList();
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 棚卸調整額の連動
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CMainFrame::CalqStatckSyz()
{
	CBaseHyoView *pBaseView = GetHonHyoView();
	if( pBaseView != NULL ){
		pBaseView->CalqStatckSyz();
	}
	return 0;
}

//-----------------------------------------------------------------------------
// 更正の請求用　申告書金額計算
//-----------------------------------------------------------------------------
// 引数	mode	：	
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
//void CDBSyzShinView::ShzBillValSet( int idx, int mode )
int CMainFrame::CalqSyzForKsk( int mode )
{
	CBaseHyoView *pBaseView = GetHonHyoView();
	if( pBaseView != NULL ){
		pBaseView->CalqSyzForKsk( mode );
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 中間及び期間短縮の更新
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
//int CMainFrame::SyzShinRenew()
int CMainFrame::RenewSyzShin( SH_PASTINFO *pPastInfo )
{
	CBaseHyoView *pBaseView = GetHonHyoView();
	if( pBaseView != NULL ){
		return pBaseView->RenewSyzShin( pPastInfo );
	}

	return -1;
}

//-----------------------------------------------------------------------------
// 印刷可能？
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	印刷可能
//			FALSE	：	印刷不可
//-----------------------------------------------------------------------------
BOOL CMainFrame::IsAblePrint()
{
	if( m_sgDotPrn || m_swBeForked || (m_nowTabIdx==0) ){
		return FALSE;
	}
	else{
		return TRUE;
	}
}

//-----------------------------------------------------------------------------
// 共通情報のセット
//-----------------------------------------------------------------------------


void CMainFrame::OnKomonsakiInf()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。

	if( m_nowTabIdx == 0 ){
		GetCoInfo( 2 );
//--> '15.07.27 INS START
		if( m_pMainView ){
			m_pMainView->SetMNData();
		}
//<-- '15.07.27 INS END
	}
	else if( m_nowTabIdx == 1 ){
//--> '15.01.14 INS START
		// 本表データのフォーカス位置の情報が更新されないことの対策
		CBaseHyoView *pBaseView = GetHonHyoView();
		if( pBaseView != NULL ){
			pBaseView->SaveInputPosition();
		}
//<-- '15.01.14 INS END

		if( GetCoInfo(2) ){
			CString	Mes;
			if( AdviseInfoRenew(0, Mes) ){
				if( IsCompanyPackageInSyz() ){
					ICSMessageBox( _T("事業所情報を他の保存マスターへ書き込みが出来ませんでした。") );
				}
				else{
					ICSMessageBox( _T("顧問先情報を他の保存マスターへ書き込みが出来ませんでした。") );
				}
				return;
			}
		}

//--> '15.07.27 INS START
		if( m_pMainView ){
			m_pMainView->SetMNData();
		}
//<-- '15.07.27 INS END

//--> '15.01.14 CUT START
//		CH26HyoView	*pH26View = (CH26HyoView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd( 2 ));
//<-- '15.01.14 CUT END
		if( pBaseView != NULL ){

//2016.10.20 UPDATE START
		//	//-------->特定課税仕入対応150713
		//	pH26View->ReInitDisp( FALSE, m_pMainView->m_SixKindFlg, m_pMainView->m_ShRevType );
		//	//----------------------------------
		////	pH26View->ReInitDisp( FALSE , m_pMainView->m_SixKindFlg );//150106
		//	//<---------------------------------

			pBaseView->ReDispHonpyoUpper();

//2016.10.20 UPDATE END

//---> '15.03.02 INS START
			pBaseView->RestoreInputPosition();
//<--- '15.03.02 INS END
		}
	}
}

//-----------------------------------------------------------------------------
// 表示帳票の最終データ取得
//-----------------------------------------------------------------------------
void CMainFrame::GetDispHyoData()
{
	if( m_nowTabIdx == 1 ){
		// 表示中のデータ保存
		CBaseHyoView *pBaseView = GetHonHyoView();
		if( pBaseView != NULL ){
			pBaseView->GetDispHyoData();
		}
	}
}

void CMainFrame::OnUpdateTaxtionList(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。

	//------>'14.08.05
	//課税取引金額計算表の切替えオプションを有効にした。
	BOOL bEnable = FALSE;

/*	if( (m_pZmSub->zvol->apno&0xf0) == 0x50 ){			// 学校・公益・福祉・宗教
		//150310
		pCmdUI->Enable( FALSE );
	}
	else{
		if( m_nowTabIdx == 2 ){

			// 10%様式の場合、非連動マスターのみ選択可能
			//if( m_SnHeadData.Sn_Sign4&0x01 ){//'14.08.07
			if (  ( GetActViewVer() == ID_VER_SYZ31 ) && ( m_isHirendoCall == false ) ) {
				pCmdUI->Enable( FALSE );
			}
			else{
				pCmdUI->Enable( TRUE );
			}
		}
		else{
			pCmdUI->Enable( FALSE );
		}
	}
	//<--------------*/

	// 財務クラスがメモリ確保される前、zvolumeテーブルがオープンされる前だと不正終了するため修正
	if( m_pZmSub && ( m_pZmSub->zvol ) && (( m_pZmSub->zvol->apno & 0xf0 ) == 0x50 )) {
		bEnable = FALSE;
	}
	else {
		if( m_nowTabIdx == 2 ) {
			// 10%様式の場合、非連動マスターのみ選択可能
			if(( GetActViewVer() == ID_VER_SYZ31 ) && ( m_isHirendoCall == false )) {
				bEnable = FALSE;
			}
			else {
				if( IsChangeKeiriHoushiki() == true ) {
					bEnable = TRUE;
				}
				else {
					// 令和5年1月1日以降開始の課税期間では、経理方式の切り替えは不可
					bEnable = FALSE;
				}
			}
		}
		else{
			bEnable = FALSE;
		}
	}

	pCmdUI->Enable( bEnable );
}

//-----------------------------------------------------------------------------
// 会社選択ボタンの切替
//-----------------------------------------------------------------------------
void CMainFrame::ChangeCoselButton( BOOL onof )
{
	EnableCoselButton( onof );
}

void CMainFrame::OnHistoryRev()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	if( m_pMainView && (m_nowTabIdx==0) ){
		m_pMainView->DropHistory();
	}
}

void CMainFrame::OnUpdateHistoryRev(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	if( m_pMainView && (m_nowTabIdx==0) ){
		BOOL	bOn = FALSE;
		if( m_pMainView->IsExistsHistory() ){
			if( m_pMainView->IsHistoryMode() == FALSE ){
				bOn = TRUE;
			}
		}
		pCmdUI->Enable( bOn );
	}
	else{
		pCmdUI->Enable( FALSE );
	}
}

void CMainFrame::OnUpdateShinPayment(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	int	Flg = 0;
	if( m_nowTabIdx != 2 ){
		if( m_pMainView ){
			//if( (m_pMainView->IsHistoryMode()==FALSE) && !(m_SnHeadData.Sn_Syuusei&0x02) ){
			if( !(m_SnHeadData.Sn_Syuusei&0x02) ){	// 17-0503　履歴保存でも国税納付書が選択できるように対応
				if( m_sgDotPrn==0 ){
					Flg = 1;	// 履歴以外
				}
			}
		}
	}
	if( Flg ){
		pCmdUI->Enable( TRUE );
	}
	else{
		pCmdUI->Enable( FALSE );
	}
}

// 24/04/02_186 add -->
int GetDivideLength( const CString& source, int length )
{
	int char_num = length*2;
	unsigned char name[128] = {0};
	int max_size = (sizeof(name) / sizeof(name[0])) - 1/*NULL文字を入れる為の-1*/;
	if(char_num > max_size)	return(-1);
	int strBaseLen = source.GetLength();
	if(strBaseLen < max_size)	max_size = strBaseLen;	// 小さいほうのサイズを採用
	memmove(name,(LPCSTR)source,max_size);
	if(_ismbstrail(name, &name[length*2]))	{
		char_num--;
	}
	return(char_num);
}

int Divide( CString source, CString& result1, CString& result2, int length )
{
	result1.Empty();
	result2.Empty();
	int	divide_len = GetDivideLength(source,length);
	if(divide_len == -1)	return(-1);
	result1 = source.Left(divide_len);
	CString	dmy_source = source;
	dmy_source.Delete(0,divide_len);
	result2 = dmy_source;
	return(0);
}
// 24/04/02_186 add <--

// 24/03/18_税務代理書面 add -->
// 税務代理権限証書
void CMainFrame::OnShinZeimud()
{
	int		yflg=0,loksgn=0;
	int		index=0;
	BOOL	min=FALSE;
	CWnd	*pw = GetActiveChild(&index,&min);

	if(m_isHirendoCall == false)	yflg=0;		// 消費税申告書
	else							yflg=1;		// 消費税申告書（非連動）

	if(m_SnHeadData.Sn_Sign4&0x01)	loksgn=1;	// 入力ロック中
	else							loksgn=0;

	TAXAGENT_INFO	ti;
	int				rv=0;

	memset(&ti,0,sizeof(TAXAGENT_INFO));
	SetTaxagInfoPar(&ti,&m_SnHeadData);
	// 
	rv=ShowZtenpInput(m_pZmSub->m_database,yflg,1,0,0,loksgn,&ti,pw,m_pZmSub);
	if(rv == 100)	{
		//// すべての帳票が削除されていれば内部のチェックをすべてONにする
		//int									sw=0,ii=0;
		//int									yflg=0,yno=0;
		//int									flg=0;
		//CArray<TAXAGENT_STFF,TAXAGENT_STFF>	car;
		//// 呼び出し元業務
		//if(m_Util.IsUnConnectMst(m_pZmSub->zvol))	yflg=1;	// 消費税申告書（非連動）
		//else										yflg=0;	// 消費税申告書
		//// 
		//for(ii=0; ii<11; ii++)	{
		//	switch(ii)	{
		//		// 税務代理権限証書
		//		case	0:	yno = 0x01;		break;
		//		// 税理士法第33条の2第1項に規定する添付書面（第1面
		//		case	1:	yno = 0x02;		break;
		//		// 税理士法第33条の2第1項に規定する添付書面（第2面）
		//		case	2:	yno = 0x04;		break;
		//		// 税理士法第33条の2第1項に規定する添付書面（第3面）
		//		case	3:	yno = 0x08;		break;
		//		// 計税理士法第33条の2第1項に規定する添付書面（第4面　1枚目）
		//		case	4:	yno = 0x10;		break;
		//		// 税理士法第33条の2第1項に規定する添付書面（第4面　2枚目）
		//		case	5:	yno = 0x20;		break;
		//		// 税理士法第33条の2第2項に規定する添付書面（第1面）
		//		case	6:	yno = 0x40;		break;
		//		// 税理士法第33条の2第2項に規定する添付書面（第2面）
		//		case	7:	yno = 0x80;		break;
		//		// 税理士法第33条の2第2項に規定する添付書面（第3面）
		//		case	8:	yno = 0x100;	break;
		//		// 税理士法第33条の2第2項に規定する添付書面（第4面　1枚目）
		//		case	9:	yno = 0x200;	break;
		//		// 税理士法第33条の2第2項に規定する添付書面（第4面　2枚目）
		//		case	10:	yno = 0x400;	break;
		//	}
		//	if(ZtenpGetPrintPage(m_pZmSub->m_database,yflg,yno,1,1,&car,this) != 0)	{
		//		sw=1;
		//		break;
		//	}
		//}
		//if(sw == 0)	{
			for(int ii=0; ii<11; ii++)	{
				::ZeroMemory(&m_PrnEXSub_ZEI, sizeof(PRNEXSUB_INF));
				m_pMapPrnEXSub_ZEI.Lookup(ii,m_PrnEXSub_ZEI);
				m_PrnEXSub_ZEI.check = 1;
				m_PrnEXSub_ZEI.busu = 1;		// 24/07/10_個別タブから出力 add
				m_pMapPrnEXSub_ZEI.SetAt(ii,m_PrnEXSub_ZEI);
			}
		//}
	}
// 24/04/03_checkon2 cor <--
// 24/03/26_税務代理書面 cor <--
}

void CMainFrame::OnUpdateShinZeimud(CCmdUI *pCmdUI)
{
// 24/03/26_税務代理書面 cor -->
	//long eymd=0;
	//if(!(m_SnHeadData.Sn_SKKBN % 2))	{
	//	eymd = m_SnHeadData.Sn_MDAYE;
	//}
	//else	{
	//	eymd = m_SnHeadData.Sn_KDAYE;
	//}
	//if(eymd >= R06_ZEIMUDAIRI)	pCmdUI->Enable(TRUE);
	//else						pCmdUI->Enable(FALSE);
// -----------------------------
	int		index=0;
	BOOL	minmax=FALSE;
	BOOL	brv=FALSE;
	CWnd *pWnd = GetActiveChild(&index,&minmax);
	if(index != 0)	{
		brv=IsUsedZeimud();
	}
	pCmdUI->Enable(brv);
// 24/03/26_税務代理書面 cor <--
}

// 税務代理権限証書に必要な情報を渡す
void CMainFrame::SetTaxagInfoPar( struct _TAXAGENT_INFO* pTi, CSnHeadData* pSh )
{
	char			bf[128]={0};
	char			bf1[128]={0},bf2[128]={0};
	CString			cs=_T(""),cs1=_T(""),cs2=_T("");
	int				symd=0,eymd=0;

	// ------------ 33-①、33-2画面上部情報 --------------------------------------------
	// ＿税
	memset(bf,0,sizeof(bf));
	strcpy_s(bf,sizeof(bf),_T("消費"));
	memcpy(pTi->ZEIKND,bf,strlen(bf));
	// ＿申告書
	memset(bf,0,sizeof(bf));
	if(pSh->IsSoleProprietor() && pSh->Sn_Syuusei&0x10)	{
		strcpy_s(bf,sizeof(bf),_T("準確定"));
	}
	else	{
		switch(pSh->Sn_SKKBN)	{
			case	0x02:	strcpy_s(bf,sizeof(bf),_T("中間"));		break;
			case	0x03:	strcpy_s(bf,sizeof(bf),_T("修正確定"));	break;
			case	0x04:	strcpy_s(bf,sizeof(bf),_T("修正中間"));	break;
			default:		strcpy_s(bf,sizeof(bf),_T("確定"));		break;
		}
	}
	memcpy(pTi->SINKOK,bf,strlen(bf));
	// 確定申告
	symd = pSh->Sn_KDAYS;
	eymd = pSh->Sn_KDAYE;
	// ＿年分　元号
	// ＿年分　年
	// 提出年月日
	if( pSh->Sn_Syuusei & 0x02)	{
		// 更正の請求
		CH31HyoView	*pH31View = (CH31HyoView *)GetHonHyoView(ID_VER_SYZ31);
		if(pH31View != NULL)	{
			pTi->Sn_PRDAY = pH31View->Get_KouseiTymd();
		}
	}
	else	{
		pTi->Sn_PRDAY = pSh->Sn_PRDAY;
	}
	memset(bf,0,sizeof(bf));
	sprintf_s(bf,sizeof(bf),_T("%08d"),pTi->Sn_PRDAY);
// 24/07/30 月日のいずれかが入力されていて、年が未入力の場合は提出年月日をクリアする
//	if(strncmp(bf,_T("0000"),4) == 0 || strncmp(&bf[4],_T("00"),2) == 0 || strncmp(&bf[6],_T("00"),2) == 0)	{
	if(strncmp(bf,_T("0000"),4) == 0 && (strncmp(&bf[4],_T("00"),2) != 0 || strncmp(&bf[6],_T("00"),2) != 0))	{
		pTi->Sn_PRDAY = 0;
	}
	// 事業期間　自
	pTi->Sn_KDAYS = symd;
	// 事業期間　至
	pTi->Sn_KDAYE = eymd;
	// 提出先名
	memset(bf,0,sizeof(bf));
	memcpy(bf,pSh->Sn_ZMSYO,sizeof(pSh->Sn_ZMSYO));
	cs.Format(_T("%s"),bf);
	cs.TrimRight();
	cs += _T("　税務署長");
	memset(bf,0,sizeof(bf));
	strcpy_s(bf,sizeof(bf),cs);
	memcpy(pTi->TEIDAY,bf,strlen(bf));
	// 税務署コード
	memcpy(pTi->Zi_ZCOD,pSh->Sn_TAXNO,sizeof(pSh->Sn_TAXNO));
	// ------------ 依頼者情報 ---------------------------------------------------------
	// 氏名又は名称
	memset(bf,0,sizeof(bf));
	if((pSh->Sn_Syuusei & 0x02) && m_Util.IsBasicMst(m_pZmSub->zvol) != FALSE)	{
		// 更正の請求 or 個人
		// 代表者氏名(Sn_DIHYO)
		memset(bf2,0,sizeof(bf2));
		memcpy(bf2,pSh->Sn_DIHYO,sizeof(pSh->Sn_DIHYO));
		strcpy_s(bf,sizeof(bf),bf2);
	}
	else	{
		// 名称又は屋号(Sn_CONAM) + 代表者氏名(Sn_DIHYO)
		memset(bf1,0,sizeof(bf1));
		memcpy(bf1,pSh->Sn_CONAM,sizeof(pSh->Sn_CONAM));
		memset(bf2,0,sizeof(bf2));
		memcpy(bf2,pSh->Sn_DIHYO,sizeof(pSh->Sn_DIHYO));
		cs.Format(_T("%s　%s"),bf1,bf2);
		Divide(cs,cs1,cs2,40);
		strcpy_s(bf,sizeof(bf),cs1);
	}
	memcpy(pTi->Dlg_Cl_MEINAM,bf,strlen(bf));
	// 住所又は事務所の所在地
	memset(bf,0,sizeof(bf));
	memset(bf1,0,sizeof(bf1));
	memcpy(bf1,pSh->Sn_NOZEI,40);
	memcpy(bf2,&pSh->Sn_NOZEI[40],40);
	sprintf_s(bf,_T("%s\r\n%s"),bf1,bf2);
	memcpy(pTi->Dlg_Cl_Adrs,bf,strlen(bf));
	// 電話番号１
	memset(bf,0,sizeof(bf));
	memcpy(bf,pSh->Sn_TELNO1,sizeof(pSh->Sn_TELNO1));
	cs.Format(_T("%s"),bf);
	cs.TrimRight();
	memset(bf,0,sizeof(bf));
	strcpy_s(bf,sizeof(bf),cs);
	if(strlen(bf) > 0)	memcpy(pTi->Sn_TELNO1,bf,strlen(bf));
	// 電話番号２
	memset(bf,0,sizeof(bf));
	memcpy(bf,pSh->Sn_TELNO2,sizeof(pSh->Sn_TELNO2));
	cs.Format(_T("%s"),bf);
	cs.TrimRight();
	memset(bf,0,sizeof(bf));
	strcpy_s(bf,sizeof(bf),cs);
	if(strlen(bf) > 0)	memcpy(pTi->Sn_TELNO2,bf,strlen(bf));
	// 電話番号３
	memset(bf,0,sizeof(bf));
	memcpy(bf,pSh->Sn_TELNO3,sizeof(pSh->Sn_TELNO3));
	cs.Format(_T("%s"),bf);
	cs.TrimRight();
	memset(bf,0,sizeof(bf));
	strcpy_s(bf,sizeof(bf),cs);
	if(strlen(bf) > 0)	memcpy(pTi->Sn_TELNO3,bf,strlen(bf));
	// ------------ 税理士情報 ---------------------------------------------------------
	// 担当税理士番号
	pTi->SVzisq = pSh->SVzisq;
	// 税理士情報
	pTi->SelectTaxInfo = pSh->SubInf;
	// 税理士情報
	// ------------ １　提示を受けた帳簿書類に関する事項 -------------------------------
	// 税目
	pTi->SEL_ZEIMOKU = 3;	// 3:消費税及び地方消費税（譲渡割）
	// 年分等 自 西暦 年月日
	pTi->SEL_ZEIMOKU_SYMD = symd;
	// 年分等 至 西暦 年月日
	pTi->SEL_ZEIMOKU_EYMD = eymd;
	// 追加税目自由入力部 税目
	// 追加税目自由入力部 年分
	if(pSh->m_ZroutSgn & 0x01)	pTi->opt |= 0x01;
	if(pSh->m_ZroutSgn & 0x02)	pTi->opt |= 0x02;
}
// 24/03/18_税務代理書面 add <--

// 24/03/26_税務代理書面 add -->
BOOL CMainFrame::IsUsedZeimud( void )
{
	// 上手くんシリーズ
	if(GET_EntrySign() != 0)			return(FALSE);
	if(GET_USERTYPE() != 0)				return(FALSE);
	// 税理士添付書面のライセンス無
#ifndef _DEBUG
	if(IsPakageCd("421") != 0)			return(FALSE);
#endif
	// 企業版
	if(IsCompanyPackageInSyz() == TRUE)	return(FALSE);
	// インボイス期間
	long	eymd=0;
	if(!(m_SnHeadData.Sn_SKKBN % 2))	eymd = m_SnHeadData.Sn_MDAYE;
	else								eymd = m_SnHeadData.Sn_KDAYE;
	if(eymd < R06_ZEIMUDAIRI)			return(FALSE);

	return(TRUE);
}
// 24/03/26_税務代理書面 add <--

//[S]
void CMainFrame::OnZennenTax()
{
	if( m_pMainView ){
		// TODO: ここにコマンド ハンドラ コードを追加します。

		//------------------->2019.12.10.yokono
		switch ( GetLastYearSyz() ) {
		case -1 :
			//エラー
			ICSMessageBox(_T("前年の消費税額の取得に失敗しました。"));
			return;
		case 1 :
			//前年度のマスターが免税
			ICSMessageBox(_T("前年は免税のため消費税額を取得できません。"));
			return;
		default :
			//正常終了　このswitch文を抜けて下へ
			break;
		}
		//<-------------------

		char	mval[64];
		CString	Mes;
		memset( mval, '\0', sizeof( mval ) );
		m_Arith.l_print( mval, m_ZenkTax, "SSS,SSS,SSS,SSS" );
	//★//[13'07.01]///
	//	Mes.Format( "前課税期間の消費税額 %s 円を取得します。\nよろしいですか？", mval );
	//////////////////
		Mes.Format( "前課税期間 自%2x年%2x月%2x日 至%2x年%2x月%02x日と消費税額 %s 円を取得します。\nよろしいですか？", 
				m_ZenkSymd[0], m_ZenkSymd[1],m_ZenkSymd[2],
				m_ZenkEymd[0], m_ZenkEymd[1],m_ZenkEymd[2], mval );
	//////////////////
		if( ICSMessageBox( Mes, MB_YESNO, 0 ) == IDYES )	{

			CBaseHyoView *pBaseView = GetHonHyoView();
			if( pBaseView != NULL ){
				//pBaseView->ZenkiDate_Chsk( m_ZenkSymd , m_ZenkEymd );
				m_ZenkSg = 0;
				m_ZenkEg = 0;
				pBaseView->ZenkiDate_Chsk( m_ZenkSymd , m_ZenkEymd , m_ZenkSg , m_ZenkEg );
			}
			CString	tag, chtag;
			tag = _T("AAF00030");
			chtag = _T("1");
			m_pMainView->SetMony( tag, chtag, (char*)m_ZenkTax );
			if( m_pMainView->WriteNofu() ){
	//			return -1;
			}
			if( pBaseView != NULL ){
				pBaseView->IndicateSheet();
			}
		}
	}
}

//[S]
void CMainFrame::OnUpdateZennenTax(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->Enable( FALSE );

	
	if( (m_nowTabIdx==1) && m_SnHeadData.IsMiddleProvisional() && !(m_SnHeadData.Sn_Sign4&0x01) ){	// 前年実績による中間申告（第２６号様式）
//-- '18.12.17 --
//		if( m_KndFlg )	pCmdUI->Enable( TRUE );
//---------------
		if( m_ZMasterinfo.isExist ){
			if( m_ZMasterinfo.isCalqed == 0 ){
				pCmdUI->Enable( TRUE );
			}
			else if( m_ZMasterinfo.isCalqed && m_ZMasterinfo.isNotZero ){
				pCmdUI->Enable( TRUE );
			}
		}
//---------------
	}
}

// 会社情報へ書き込み
int CMainFrame::SetCoInformation( void )
{
	int St, Ck, Ct, Ln;
	int BankGt = -1;	// 銀行情報サイン
	int	PostGt = -1;	// 郵便情報サイン
	char	WKbuf[128], Ascii[128], TelNo1[10], TelNo2[10], TelNo3[10];

	char	asci[512] = {0};
	char	CoNo[4] = {0};


	char	MDev[4]={0};
	char	MDdm[4]={0};
	if( PCOM_GetString( _T("MasterDevice"), MDev, MDdm ) == -1 ) {
//		errmsg =  _T("マスターデバイスが取得できません。" );
		return -1;
	}

	// 現在の選択マスターは過年度マスターかどうか。過年度の場合処理しない
	if(m_isKndMster)	return( 0 );

//-- '18.12.17 --
//	m_pCoinfCtl = NULL;
//	// クラスの構築
//	if( m_pCoinfCtl == NULL )	m_pCoinfCtl = new CCoinfCtl;
//	if( m_pCoinfCtl == NULL ) return( ERR );
//	sprintf_s( MDdm, sizeof( MDdm ), "%s:", MDev );
//	if( m_pCoinfCtl->OpenFile( 3, MDdm ) == FALSE )	{
//		delete( m_pCoinfCtl );
//		m_pCoinfCtl = NULL;	// ('14.03.13)
//		return( 0 );
//	}
//---------------
	if( m_pCoinfCtl == NULL ){
		if( (m_pCoinfCtl = new CCoinfCtl) == NULL ){
			return( ERR );
		}
	}
	else{
		m_pCoinfCtl->CloseFile();
	}

	sprintf_s( MDdm, sizeof( MDdm ), "%s:", MDev );
// 書き込み時には、情報の縮小は行わないこと！
//	m_pCoinfCtl->TgtLimit_Kmn( LM_MAININFO|LM_KINYU|LM_TANTOZEIRI|LM_UPDATE );
	if( m_pCoinfCtl->OpenFile( 3, MDdm ) == FALSE )	{
		delete( m_pCoinfCtl );
		m_pCoinfCtl = NULL;	// ('14.03.13)
		return( 0 );
	}
//---------------

	memset( asci, '\0', sizeof( asci ) );
	memset( CoNo, '\0', sizeof( CoNo ) );
	sprintf_s( asci, sizeof( asci ), "%08d", m_pZmSub->zvol->v_cod );
	l_pack( CoNo, asci, 8 );

//--'18.12.17 --
//	St = m_pCoinfCtl->IsRegistMcode( 1, (m_pZmSub->zvol->apno&0xff), 0, (LPBYTE)CoNo, NULL );
//--------------
	St = m_rtIsRegistMcode;
//--------------
	if( St != 1 ) {
		m_pCoinfCtl->CloseFile();
		delete( m_pCoinfCtl );
		m_pCoinfCtl = NULL;	// ('14.03.13)
		return( 0 );
	}
	else{
		m_Finish = 1;
	}

	// 金融機関の為の会社情報の取込
	CINF_REC	CoRecGt = {0};
	CTBL_PAR	CtParGt	= {0};
	RFIN_REC	RfinRec = {0};
	char		KeyBuf[10] = {0};
	char		AscBuf[10] = {0};

	CtParGt.tpDtyp = 1;
	CtParGt.tpMtyp = 0;
	CtParGt.tpApno = (m_pZmSub->zvol->apno&0xff);
	memmove( CtParGt.tpCcod, CoNo, 4 );
	if( m_pCoinfCtl->FindRecFromMcod( 0, &CoRecGt, &CtParGt ) != 1 )	{
		m_pCoinfCtl->CloseFile();
		delete( m_pCoinfCtl );
		m_pCoinfCtl = NULL;	// ('14.03.13)
		return( 0 );
	}

	if( m_SnHeadData.Sn_BYsw == 1 )	{	// 金融機関はゆうちょ銀行
		PostGt = 0;
		RfinRec.rfKubn = 1;
		// 郵便局名
		memmove( RfinRec.rfFnam, m_SnHeadData.Sn_YUBIN, 16 );
		// 郵便貯金の記号番号１
		memmove( RfinRec.rfKzno, m_SnHeadData.Sn_YUNUM1, 5 );
		// 郵便貯金の記号番号２
		memmove( &RfinRec.rfKzno[6], m_SnHeadData.Sn_YUNUM2, 8 );
	}
	else	{								// 金融機関は銀行
		BankGt = 0;
		RfinRec.rfKubn = 2;
		// 銀行名称
		memmove( RfinRec.rfFnam, m_SnHeadData.Sn_BANK, 20 );
		// 支店名称
		memmove( RfinRec.rfSnam, m_SnHeadData.Sn_SITEN, 20 );
		// 預金種別
		memset( WKbuf, '\0', sizeof( WKbuf ) );
		memmove( WKbuf, m_SnHeadData.Sn_YOKIN, 8 );
		if( !CompareStringSJIS( WKbuf,_T("普通　　") ) )	RfinRec.rfDepo = 1;
		if( !CompareStringSJIS( WKbuf,_T("当座　　") ) )	RfinRec.rfDepo = 2;
		if( !CompareStringSJIS( WKbuf,_T("納税準備") ) )	RfinRec.rfDepo = 3;
		if( !CompareStringSJIS( WKbuf,_T("貯蓄　　") ) )	RfinRec.rfDepo = 4;
		if( !RfinRec.rfDepo ) {
			memmove( RfinRec.rfDnam, m_SnHeadData.Sn_YOKIN, 8 );
		}
		// 口座番号
		l_unpac( RfinRec.rfKzno, m_SnHeadData.Sn_KOZA, 10 );
		// 銀行・金庫等
		switch(m_SnHeadData.Sn_FINANCE)	{
			case 0x00 :	RfinRec.rfKubn = 2;	break;	// 銀行
			case 0x01 :	RfinRec.rfKubn = 3;	break;	// 金庫
			case 0x02 :	RfinRec.rfKubn = 4;	break;	// 組合
			case 0x03 :	RfinRec.rfKubn = 5;	break;	// 農協 
			case 0x04 :	RfinRec.rfKubn = 6;	break;	// 漁協
			default   : RfinRec.rfKubn = 0;	break;	// 無し
		}
		// 本店・支店等
		switch(m_SnHeadData.Sn_CHAIN)	{
			case 0x00 :	RfinRec.rfSkbn = 1;	break;	// 本店
			case 0x01 :	RfinRec.rfSkbn = 2;	break;	// 支店
			case 0x02 :	RfinRec.rfSkbn = 3;	break;	// 本所
			case 0x03 :	RfinRec.rfSkbn = 4;	break;	// 支所
			case 0x05 :	RfinRec.rfSkbn = 5;	break;	// 出張所
			case 0x06 : RfinRec.rfSkbn = 6;	break;	// 上記以外 ★[13'11.27]
			default   : RfinRec.rfSkbn = 0;	break;	// 無し
		}
	}

	// 会社情報へ書き込み
	NCOF_PAR	NCpar = {0};
	CINF_REC	CoRec = {0};
	
	NCpar.cpPare = this;
	NCpar.cpDtyp = 1;
	NCpar.cpMtyp = 0;
	NCpar.cpApno = (m_pZmSub->zvol->apno&0xff);
	memmove( NCpar.cpCode, CoNo, 4 );

	long& item_flg0 = NCpar.cpEflg[0];
	long& item_flg1 = NCpar.cpEflg[1];
	
	// 整理番号
	if( m_SnHeadData.Sn_SEIRI[0] != NowCoInf.Sn_SEIRI[0] || m_SnHeadData.Sn_SEIRI[1] != NowCoInf.Sn_SEIRI[1] ||
		m_SnHeadData.Sn_SEIRI[2] != NowCoInf.Sn_SEIRI[2] || m_SnHeadData.Sn_SEIRI[3] != NowCoInf.Sn_SEIRI[3] ) {
		item_flg1 |= 0x00000080;
		memset( WKbuf, '\0', sizeof( WKbuf ) );
		l_unpac( WKbuf, m_SnHeadData.Sn_SEIRI,  8 );
		memmove(  CoRec.ciRegNo, WKbuf, 8 );
	}

	// 税務署名
	if( m_Util.StringCompare( m_SnHeadData.Sn_ZMSYO, NowCoInf.Sn_ZMSYO, 12 ) )	{
		item_flg1 |= 0x00000040;
		Ln = m_Util.MixSpaceCutLength( (char *)m_SnHeadData.Sn_ZMSYO, 12 );
		memmove( CoRec.ciTaxName, m_SnHeadData.Sn_ZMSYO, Ln );
	}
	if( m_Util.StringCompare( m_SnHeadData.Sn_TAXNO, NowCoInf.Sn_TAXNO, 5 ) )	{
		item_flg1 |= 0x00000040;
		if( m_SnHeadData.Sn_TAXNO[0] == 'F' )	memset( CoRec.ciTaxCode, 0xff, 5 );
		else							memmove( (BYTE*)CoRec.ciTaxCode, m_SnHeadData.Sn_TAXNO, 5 );
	}

	
	// 納税地　郵便番号
	if( m_Util.StringCompare( m_SnHeadData.Sn_ZIP, NowCoInf.Sn_ZIP, 8 ) )	{
		char	ZipNo[10] = {0}; 	
		if( PostcodeConvert( ZipNo, m_SnHeadData.Sn_ZIP ) ) {
			item_flg1 |= 0x00000400;
			memmove( CoRec.ciTppPno, ZipNo, 8 );
			if( CoRecGt.ciNzsw1 == 1 )	{
				item_flg0 |= 0x00000800;
				memmove( CoRec.ciPostNo, ZipNo, 8 );
			}
			if( CoRecGt.ciNzsw1 == 2 )	{
				item_flg1 |= 0x00020000;
				memmove( CoRec.ciUtpPno, ZipNo, 8 );
			}
		}
	}

	// 納税地上段及び下段
	if( m_Util.StringCompare( m_SnHeadData.Sn_NOZEI, NowCoInf.Sn_NOZEI, 40 ) ||
		m_Util.StringCompare( &m_SnHeadData.Sn_NOZEI[40], &NowCoInf.Sn_NOZEI[40], 40 ) ) {
		// 納税地（上段）
		item_flg1 |= 0x00000800;
		Ln = m_Util.MixSpaceCutLength( (char *)m_SnHeadData.Sn_NOZEI, 40 );
		memmove( CoRec.ciTppAd1, m_SnHeadData.Sn_NOZEI, Ln );
		if( CoRecGt.ciNzsw1 == 1 )	{
			item_flg0 |= 0x00001000;
			memmove( CoRec.ciAdrs1, m_SnHeadData.Sn_NOZEI, Ln );
		}
		if( CoRecGt.ciNzsw1 == 2 )	{
			item_flg1 |= 0x00040000;
			memmove( CoRec.ciUtpAd1, m_SnHeadData.Sn_NOZEI, Ln );
		}
		// 納税地（下段）
		item_flg1 |= 0x00002000;
		Ln = m_Util.MixSpaceCutLength( (char *)&m_SnHeadData.Sn_NOZEI[40], 40 );
		memmove( CoRec.ciTppAd2, &m_SnHeadData.Sn_NOZEI[40], Ln );
		if( CoRecGt.ciNzsw1 == 1 )	{
			item_flg0 |= 0x00004000;
			memmove( CoRec.ciAdrs2, &m_SnHeadData.Sn_NOZEI[40], Ln );
		}
		if( CoRecGt.ciNzsw1 == 2 )	{
			item_flg1 |= 0x00100000;
			memmove( CoRec.ciUtpAd2, &m_SnHeadData.Sn_NOZEI[40], Ln );
		}
	}

	// 納税地 電話番号
	if( m_Util.StringCompare( m_SnHeadData.Sn_TELNO1, NowCoInf.Sn_TELNO1, 6 ) ||
		m_Util.StringCompare( m_SnHeadData.Sn_TELNO2, NowCoInf.Sn_TELNO2, 4 ) ||
		m_Util.StringCompare( m_SnHeadData.Sn_TELNO3, NowCoInf.Sn_TELNO3, 4 ) )	{
		item_flg1 |= 0x00008000;
		memset( WKbuf, '\0', sizeof( WKbuf ) );
		memset( TelNo1, '\0', sizeof( TelNo1 ) );
		memset( TelNo2, '\0', sizeof( TelNo2 ) );
		memset( TelNo3, '\0', sizeof( TelNo3 ) );
		memmove( TelNo1, m_SnHeadData.Sn_TELNO1, 6 );
		memmove( TelNo2, m_SnHeadData.Sn_TELNO2, 4 );
		memmove( TelNo3, m_SnHeadData.Sn_TELNO3, 4 );
		m_Util.AnkSpaceCut( TelNo1, 6 );
		m_Util.AnkSpaceCut( TelNo2, 4 );
		m_Util.AnkSpaceCut( TelNo3, 4 );
		sprintf_s( WKbuf, sizeof( WKbuf ), "%s-%s-%s", TelNo1, TelNo2, TelNo3 );
		memmove( CoRec.ciTppTel, WKbuf, 16 );
		if( CoRecGt.ciNzsw1 == 1 )	{
			item_flg0 |= 0x00010000;
			memmove( CoRec.ciTeln2, WKbuf, 16 );
		}
		if( CoRecGt.ciNzsw1 == 2 )	{
			item_flg1 |= 0x00010000;
			memmove( CoRec.ciUtpTel, WKbuf, 16 );
		}
	}
	
	// 代表者氏名及び氏名又は屋号 ●セットする時は顧問先の法人及び個人に従う●
	if( CoRecGt.ciHKkbn == 1 ) {

		// 個人は会社名称を代表者氏名に転送
		// 代表者氏名
		if( m_Util.StringCompare( m_SnHeadData.Sn_DIHYO, NowCoInf.Sn_DIHYO, 40 ) )	{
			item_flg0 |= 0x00000004;
			Ln = m_Util.MixSpaceCutLength( (char *)m_SnHeadData.Sn_DIHYO, 40 );
			memmove( CoRec.ciName, m_SnHeadData.Sn_DIHYO, Ln );
		}
		// 代表者氏名（ふりがな）漢字変換はCompareStringSJIS()関数がしてくれる
		if( m_Util.StringCompare( m_SnHeadData.Sn_DIFRI, NowCoInf.Sn_DIFRI, 34*2 ) )	{
			item_flg0 |= 0x00000008;
			memset( WKbuf, '\0', sizeof( WKbuf ) );
			memmove( WKbuf, m_SnHeadData.Sn_DIFRI, 80 );
			memset( Ascii, '\0', sizeof( Ascii ) );
			// 漢字文字を（ひらがな・カタカナ）をカタカナに変換
			m_Util.LetterAlter( WKbuf, Ascii, 34 );
			Ln =m_Util. MixSpaceCutLength( (char *)Ascii, 34 );
			memmove( CoRec.ciKana, Ascii, Ln );
		}
		// 屋号又は氏名
		if( m_Util.StringCompare( m_SnHeadData.Sn_CONAM, NowCoInf.Sn_CONAM, 36 ) )	{
			item_flg0 |= 0x00000080;
			Ln = m_Util.MixSpaceCutLength( (char *)m_SnHeadData.Sn_CONAM, 36 );
			memmove( CoRec.ciYagou, m_SnHeadData.Sn_CONAM, Ln );
		}
		// 屋号又は氏名（ふりがな）漢字変換が必要
		if( m_Util.StringCompare( m_SnHeadData.Sn_COFRI, NowCoInf.Sn_COFRI, 34*2 ) )	{
			item_flg0 |= 0x00000100;
			memset( WKbuf, '\0', sizeof( WKbuf ) );
			memmove( WKbuf, m_SnHeadData.Sn_COFRI, 80 );
			memset( Ascii, '\0', sizeof( Ascii ) );
			// 漢字文字を（ひらがな・カタカナ）をカタカナに変換
			m_Util.LetterAlter( WKbuf, Ascii, 30 );
			Ln = m_Util.MixSpaceCutLength( (char *)Ascii, 30 );
			memmove( CoRec.ciYgKna, Ascii, Ln );
		}
	}

	else	{
		// 法人は会社名称を屋号又は氏名に転送
		// 名称又は屋号
		if(  m_Util.StringCompare( m_SnHeadData.Sn_CONAM, NowCoInf.Sn_CONAM, 40 ) )	{
			item_flg0 |= 0x00000004;
			Ln =  m_Util.MixSpaceCutLength( (char *)m_SnHeadData.Sn_CONAM, 40 );
			memmove( CoRec.ciName, m_SnHeadData.Sn_CONAM, Ln );
		}
		// 名称又は屋号（ふりがな）漢字変換が必要
		if(  m_Util.StringCompare( m_SnHeadData.Sn_COFRI, NowCoInf.Sn_COFRI, 34*2 ) )	{
			item_flg0 |= 0x00000008;
			memset( WKbuf, '\0', sizeof( WKbuf ) );
			memmove( WKbuf, m_SnHeadData.Sn_COFRI, 80 );
			memset( Ascii, '\0', sizeof( Ascii ) );
			// 漢字文字を（ひらがな・カタカナ）をカタカナに変換
			 m_Util.LetterAlter( WKbuf, Ascii, 34 );
			Ln =  m_Util.MixSpaceCutLength( (char *)Ascii, 34 );
			memmove( CoRec.ciKana, Ascii, Ln );
		}
		// 代表者名
		if(  m_Util.StringCompare(  m_SnHeadData.Sn_DIHYO, NowCoInf.Sn_DIHYO, 40 ) )	{
			item_flg0 |= 0x00800000;
			Ln =  m_Util.MixSpaceCutLength( (char *)m_SnHeadData.Sn_DIHYO, 24 );
			memmove( CoRec.ciRepName, m_SnHeadData.Sn_DIHYO, Ln );
		}
		// 代表社名（ふりがな）
		if(  m_Util.StringCompare( m_SnHeadData.Sn_DIFRI, NowCoInf.Sn_DIFRI, 34*2 ) )	{
			item_flg0 |= 0x01000000;
			memset( WKbuf, '\0', sizeof( WKbuf ) );
			memmove( WKbuf, m_SnHeadData.Sn_DIFRI, 80 );
			memset( Ascii, '\0', sizeof( Ascii ) );
			// 漢字文字を（ひらがな・カタカナ）をカタカナに変換
			 m_Util.LetterAlter( WKbuf, Ascii, 24 );
			Ln =  m_Util.MixSpaceCutLength( (char *)Ascii, 24 );
			memmove( CoRec.ciRepKana, Ascii, Ln );
		}
//--> '15.12.15 INS START
		// 代表者　郵便番号
		if( m_Util.StringCompare( m_SnHeadData.Sn_DIZIP, NowCoInf.Sn_DIZIP, 8 ) )	{
			char	ZipNo[10] = {0}; 	
			if( PostcodeConvert( ZipNo, m_SnHeadData.Sn_DIZIP ) ) {
				item_flg0 |= 0x02000000;
				memmove( CoRec.ciRepPno, ZipNo, 8 );
			}
		}

		// 代表者住所上段及び下段
		if( m_Util.StringCompare( m_SnHeadData.Sn_DIADR, NowCoInf.Sn_DIADR, 40 ) ||
			m_Util.StringCompare( &m_SnHeadData.Sn_DIADR[40], &NowCoInf.Sn_DIADR[40], 40 ) ) {
			// 代表者住所（上段）
			item_flg0 |= 0x04000000;
			Ln = m_Util.MixSpaceCutLength( (char *)m_SnHeadData.Sn_DIADR, 40 );
			memmove( CoRec.ciRepAd1, m_SnHeadData.Sn_DIADR, Ln );
			// 代表者住所（下段）
			item_flg0 |= 0x08000000;
			Ln = m_Util.MixSpaceCutLength( (char *)&m_SnHeadData.Sn_DIADR[40], 40 );
			memmove( CoRec.ciRepAd2, &m_SnHeadData.Sn_DIADR[40], Ln );
		}
//<-- '15.12.15 INS END

//--> '15.12.23 INS START
		if( m_Util.StringCompare( m_SnHeadData.Sn_DITELNO1, NowCoInf.Sn_DITELNO1, 6 ) ||
			m_Util.StringCompare( m_SnHeadData.Sn_DITELNO2, NowCoInf.Sn_DITELNO2, 4 ) ||
			m_Util.StringCompare( m_SnHeadData.Sn_DITELNO3, NowCoInf.Sn_DITELNO3, 4 ) )	{
			item_flg0 |= 0x10000000;
			memset( WKbuf, '\0', sizeof( WKbuf ) );
			memset( TelNo1, '\0', sizeof( TelNo1 ) );
			memset( TelNo2, '\0', sizeof( TelNo2 ) );
			memset( TelNo3, '\0', sizeof( TelNo3 ) );
			memmove( TelNo1, m_SnHeadData.Sn_DITELNO1, 6 );
			memmove( TelNo2, m_SnHeadData.Sn_DITELNO2, 4 );
			memmove( TelNo3, m_SnHeadData.Sn_DITELNO3, 4 );
			m_Util.AnkSpaceCut( TelNo1, 6 );
			m_Util.AnkSpaceCut( TelNo2, 4 );
			m_Util.AnkSpaceCut( TelNo3, 4 );
			sprintf_s( WKbuf, sizeof( WKbuf ), "%s-%s-%s", TelNo1, TelNo2, TelNo3 );
			memmove( CoRec.ciRepTel, WKbuf, 16 );
		}
//<-- '15.12.23 INS END
	}

	// 経理担当社名
	if( m_Util.StringCompare( m_SnHeadData.Sn_KEIRI, NowCoInf.Sn_KEIRI, 40 ) )	{
		item_flg1 |= 0x00000002;
		memmove( CoRec.ciAcName, m_SnHeadData.Sn_KEIRI, 24 );
	}

	// 郵便＆銀行情報の取得
	int Bank = -1;	// 銀行情報サイン
	int	Post = -1;	// 郵便情報サイン
	if( m_SnHeadData.Sn_BYsw == 1 )	{	// 金融機関はゆうちょ銀行
		// 郵便局名
		if( m_Util.StringCompare( m_SnHeadData.Sn_YUBIN, NowCoInf.Sn_YUBIN, 16 ) )					Post = 0;
		// 郵便貯金の記号番号１
		if( m_Util.StringCompare( m_SnHeadData.Sn_YUNUM1, NowCoInf.Sn_YUNUM1, 5 ) )					Post = 0;
		// 郵便貯金の記号番号２
		if( m_Util.StringCompare( m_SnHeadData.Sn_YUNUM2, NowCoInf.Sn_YUNUM2, 8 ) )					Post = 0;
		if( Post != -1 )	CoRec.ciFinTbl[Post].rfKubn = 1;
	}
	else	{								// 金融機関は銀行
		// 銀行名称
		if( m_Util.StringCompare( m_SnHeadData.Sn_BANK, NowCoInf.Sn_BANK, 20 ) ){
			Bank = 0;
		}
		// 支店名称
		if( m_Util.StringCompare( m_SnHeadData.Sn_SITEN, NowCoInf.Sn_SITEN, 20 ) ){
			Bank = 0;
		}
		// 預金種別
		if( m_Util.StringCompare( m_SnHeadData.Sn_YOKIN, NowCoInf.Sn_YOKIN, 8 ) ){
			Bank = 0;
		}
		// 口座番号
		if( strncmp( (char *)m_SnHeadData.Sn_KOZA, (char *)NowCoInf.Sn_KOZA, 5 )	){
			Bank = 0;
		}
		// 銀行・金庫等
		if( m_SnHeadData.Sn_FINANCE != NowCoInf.Sn_FINANCE ){
			Bank = 0;
		}
		// 本店・支店等
		if( m_SnHeadData.Sn_CHAIN != NowCoInf.Sn_CHAIN ){
			Bank = 0;
		}
		if( Bank != -1 ){
			CoRec.ciFinTbl[Bank].rfKubn = 2;
		}
	}

	// 金融テーブルの比較
	if( Post != -1 || Bank != -1 ) {
		for( Ct = 0; Ct != 5; Ct++ ) {
			if( CoRecGt.ciFinTbl[Ct].rfKubn == RfinRec.rfKubn ) {
				Ck = 0;
				if( m_SnHeadData.Sn_BYsw == 1 ) {
					// 郵便局名
					if( !m_Util.StringCompare( (BYTE*)CoRecGt.ciFinTbl[Ct].rfFnam, (BYTE*)RfinRec.rfFnam, 16 ) )			Ck++;
					// 郵便貯金の記号番号１・２
					if( !m_Util.StringCompare( (BYTE*)CoRecGt.ciFinTbl[Ct].rfKzno, (BYTE*)RfinRec.rfKzno, 14 ) )			Ck++;
					if( Ck == 2 )	{
						CoRec.ciFinTbl[Post].rfKubn = 0;
						Post = -1;
					}
				}
				else	{
					// 銀行名称
					if( m_Util.StringCompare( (BYTE*)CoRecGt.ciFinTbl[Ct].rfFnam, (BYTE*)RfinRec.rfFnam, 20 ) )			Ck++;
					// 支店名称										
					if( m_Util.StringCompare( (BYTE*)CoRecGt.ciFinTbl[Ct].rfSnam, (BYTE*)RfinRec.rfSnam, 20 ) )			Ck++;
					// 預金種別
					if( CoRecGt.ciFinTbl[Ct].rfDepo != RfinRec.rfDepo )												Ck++;
					if( m_Util.StringCompare( (BYTE*)CoRecGt.ciFinTbl[Ct].rfDnam, (BYTE*)RfinRec.rfDnam, 8 ) )				Ck++;
					// 口座番号										
					if( m_Util.StringCompare( (BYTE*)CoRecGt.ciFinTbl[Ct].rfKzno, (BYTE*)RfinRec.rfKzno, 10 )	)			Ck++;
					// 銀行・金庫等
					if( CoRecGt.ciFinTbl[Ct].rfKubn != RfinRec.rfKubn )												Ck++;
					// 本店・支店等
					if( CoRecGt.ciFinTbl[Ct].rfSkbn != RfinRec.rfSkbn )												Ck++;
					if( Ck == 7 )	{
						CoRec.ciFinTbl[Ct].rfKubn = 0;
						Bank = -1;
					}

				}
			}
		}

		// 銀行情報
		if( Bank != -1 )	{
			// ゆうちょ銀行＆銀行情報
			item_flg1 |= 0x00000010;

			// 銀行名称
			memmove( CoRec.ciFinTbl[Bank].rfFnam, m_SnHeadData.Sn_BANK, 20 );
			// 支店名称
			memmove( CoRec.ciFinTbl[Bank].rfSnam, m_SnHeadData.Sn_SITEN, 20 );
			// 預金種別
			memset( WKbuf, '\0', sizeof( WKbuf ) );
			memmove( WKbuf, m_SnHeadData.Sn_YOKIN, 8 );
			if( !CompareStringSJIS( WKbuf,_T("普通　　") ) )	CoRec.ciFinTbl[Bank].rfDepo = 1;
			if( !CompareStringSJIS( WKbuf,_T("当座　　") ) )	CoRec.ciFinTbl[Bank].rfDepo = 2;
			if( !CompareStringSJIS( WKbuf,_T("納税準備") ) )	CoRec.ciFinTbl[Bank].rfDepo = 3;
			if( !CompareStringSJIS( WKbuf,_T("貯蓄　　") ) )	CoRec.ciFinTbl[Bank].rfDepo = 4;
			if( !CoRec.ciFinTbl[Bank].rfDepo ) {
				memmove( CoRec.ciFinTbl[Bank].rfDnam, m_SnHeadData.Sn_YOKIN, 8 );
			}
			// 口座番号
			memset( WKbuf, '\0', sizeof( WKbuf ) );
			l_unpac( WKbuf, m_SnHeadData.Sn_KOZA, 10 );
			memset( Ascii, '\0', sizeof( Ascii ) );
			m_Util.AllAnkSpaceCut( Ascii, WKbuf, 10 );
			memmove( CoRec.ciFinTbl[Bank].rfKzno, Ascii, 10 );
			// 銀行・金庫等
			switch(m_SnHeadData.Sn_FINANCE)	{
				case 0x00 :	CoRec.ciFinTbl[Bank].rfKubn = 2;	break;	// 銀行
				case 0x01 :	CoRec.ciFinTbl[Bank].rfKubn = 3;	break;	// 金庫
				case 0x02 :	CoRec.ciFinTbl[Bank].rfKubn = 4;	break;	// 組合
				case 0x03 :	CoRec.ciFinTbl[Bank].rfKubn = 5;	break;	// 農協 
				case 0x04 :	CoRec.ciFinTbl[Bank].rfKubn = 6;	break;	// 漁協
				default   :  item_flg1 &= 0xffffffef;	break;	// 無し
			}
			// 本店・支店等
			switch(m_SnHeadData.Sn_CHAIN)	{
				case 0x00 :	CoRec.ciFinTbl[Bank].rfSkbn = 1;	break;	// 本店
				case 0x01 :	CoRec.ciFinTbl[Bank].rfSkbn = 2;	break;	// 支店
				case 0x02 :	CoRec.ciFinTbl[Bank].rfSkbn = 3;	break;	// 本所
				case 0x03 :	CoRec.ciFinTbl[Bank].rfSkbn = 4;	break;	// 支所
				case 0x05 :	CoRec.ciFinTbl[Bank].rfSkbn = 5;	break;	// 出張所
				case 0x06 :	CoRec.ciFinTbl[Bank].rfSkbn = 6;	break;	// 上記以外 ★[13'11.27]
				default   : item_flg1 &= 0xffffffef;	break;	// 無し
			}
			
			if( !(item_flg1 & 0x00000010) ) {
				char		ErrMsg[256];
				memset( ErrMsg, '\0', sizeof( ErrMsg ) );
				strcpy_s( ErrMsg, sizeof( ErrMsg ), _T("還付先金融機関の種別（銀行・金庫及び本店・支店）が選択されていない為、還付金融機関の項目は転記されません。" ) );
				NCpar.cpMsgp = ErrMsg;
				NCpar.cpFunc |= 0x40;			// メッセージを表示
			}
		}
		// 郵便局情報
		if( Post != -1 )	{
			// ゆうちょ銀行＆銀行情報
			item_flg1 |= 0x00000010;

			// 郵便局名
			memmove( CoRec.ciFinTbl[Post].rfFnam, m_SnHeadData.Sn_YUBIN, 16 );

			memset( TelNo1, '\0', sizeof( TelNo1 ) );
			m_Util.AllAnkSpaceCut( TelNo1, (char*)m_SnHeadData.Sn_YUNUM1, 5 );

			memset( TelNo2, '\0', sizeof( TelNo2 ) );
			m_Util.AllAnkSpaceCut( TelNo2, (char*)m_SnHeadData.Sn_YUNUM2, 8 );

			memset( Ascii, '\0', sizeof( Ascii ) );
			sprintf_s( Ascii, sizeof( Ascii ), "%s-%s", TelNo1, TelNo2 );
			Ln = m_Util.MixSpaceCutLength( (char *)Ascii, 14 );
			memmove( CoRec.ciFinTbl[Post].rfKzno, Ascii, Ln );
		}	
	}


	CSnset	curset = m_pMainView->GetSnset();

	if( !( curset.IsKakuteiShinkoku() && !curset.IsKikanTansyuku() ) ){
	
		if( curset.IsTyukanShinkoku() ){ 
			// 中間申告 処理単位の設定
			if( m_SnHeadData.Sn_MCOUNT != m_old_Sn_MCOUNT ){
				item_flg1 |= 0x00000020;
				CoRec.ciSyDcTimes = m_SnHeadData.Sn_MCOUNT;
			}
			
			// 課税期間短縮 処理単位のクリア
			if( (m_SnHeadData.Sn_KZTNSK&0x0f) != (m_old_Sn_KZTNSK&0x0f) ){
				item_flg1 |= 0x00000200;
				CoRec.ciSyTShort = 0;
			}

			if( m_old_tansyuku != CTime() )
				item_flg1 |= 0x00010000;
		}

		if( curset.IsKakuteiShinkoku() && curset.IsKikanTansyuku() ) {
			// 課税期間短縮 処理単位の設定
			if( !curset.IsMinashi() ) {
				if( (m_SnHeadData.Sn_KZTNSK&0x0f) != (m_old_Sn_KZTNSK&0x0f) )	{
					item_flg1 |= 0x00000200;
					if(m_SnHeadData.Sn_KZTNSK&0x0f) // 顧問先情報の仕様により反転
						CoRec.ciSyTShort = (m_SnHeadData.Sn_KZTNSK&0x0f) == 0x01 ? 0x02 : 0x01;
					else
						CoRec.ciSyTShort = 0;
				}

				// 中間申告 処理単位のクリア
				if( m_SnHeadData.Sn_MCOUNT != m_old_Sn_MCOUNT ){
					item_flg1 |= 0x00000020;
					CoRec.ciSyDcTimes = 0;
				}

				// 課税期間短縮 開始年月日の設定orクリア
				CTime tansyuku = m_pMainView->FindTansyukuKaishi();
				if( tansyuku != CTime() ){
//					CoRec.ciSyTSDay[0] = 0x04;
//					CTimeControl::SetYMD(&CoRec.ciSyTSDay[1],tansyuku);
					CTimeControl::SetYMD(CoRec.ciSyTSDay, tansyuku);
				}

				if( tansyuku != m_old_tansyuku ){
					item_flg1 |= 0x00010000;
				}
			}
		}
	}

	
	memmove( &NCpar.cpCflg, &NCpar.cpEflg, sizeof( NCpar.cpEflg ) );

	if( item_flg0 == 0x00000000 && item_flg1 == 0x00000000 )	{
		if( m_pCoinfCtl->CloseFile() == FALSE )	{
			delete( m_pCoinfCtl );
			m_pCoinfCtl = NULL;	// ('14.03.13)
			return( 0 );
		}
		delete( m_pCoinfCtl );
		m_pCoinfCtl = NULL;	// ('14.03.13)
		return( 0 );
	}

//--> 20230324 顧問先情報の取込書込
	m_pCoinfCtl->SetMstDat( &CoRec, KMNCTL_GKIND_SYZ );
//<-- 20230324

	St = 0;
//-- '15.12.22 --
//	St = m_pCoinfCtl->PutCoinfEX( 0x40, &CoRec, &NCpar );
//---------------
	St = m_pCoinfCtl->PutCoinfEX( 0x50, &CoRec, &NCpar );
//---------------
	if( m_pCoinfCtl->CloseFile() == FALSE )	{
		delete( m_pCoinfCtl );
		m_pCoinfCtl = NULL;	// ('14.03.13)
		return( 0 );
	}
	else{
		m_Finish = 1;
	}
	delete( m_pCoinfCtl );
	m_pCoinfCtl = NULL;	// ('14.03.13)

	return( St );

}


// 顧問先情報へ郵便番号の変換
int	CMainFrame::PostcodeConvert( char *SetCode, unsigned char *GetCode )
{
	int		i, j, pos;
	int		Pln;
	char	Pnum[20], Pnum1[10], Pnum2[10];

	memset( Pnum, '\0', sizeof( Pnum ) );
	memset( Pnum1, '\0', sizeof( Pnum1 ) );
	memset( Pnum2, '\0', sizeof( Pnum2 ) );
	Pln = m_Util.MixSpaceCutLength( (char *)GetCode, 8 );
	memmove( Pnum, GetCode, Pln );

	// 郵便番号
	// ハイフン検索
//★//[12'08.21]///
//	for( i = j = pos = 0; i != Pln; ++i ) {
///////////////////
	for( i = j = pos = 0; i <= Pln; ++i ) {
///////////////////
		if( j < 0 ) {
			if( (Pnum[i]&0xff) != '-' )	continue;
		}
		if( (Pnum[i]&0xff) == '-' ) {
			++i;	++pos;	j = 0;
		}
		if( (Pnum[i]&0xff) == ' ' )	continue;
		switch( pos ) {
			case 0 :	//上3桁
				Pnum1[j] = (Pnum[i]&0xff);
				if( ++j == 3 )	j = -1;
				break;
			case 1 :	//下4桁
				Pnum2[j] = (Pnum[i]&0xff);
				if( ++j == 4 )	j = -1;
				break;
			default:	break;
		}
	}
	if( !pos ) {
		// スペース検索
		for( i = j = pos = 0; i != Pln; ++i ) {
			if( j < 0 ) {
				if( (Pnum[i]&0xff) != ' ' )	continue;
			}
			if( (Pnum[i]&0xff) == ' ' ) {
				++i;	++pos;	j = 0;
			}
			switch( pos ) {
				case 0 :	//上3桁
					Pnum1[j] = (Pnum[i]&0xff);
					if( ++j == 3 )	j = -1;
					break;
				case 1 :	//下4桁
					Pnum2[j] = (Pnum[i]&0xff);
					if( ++j == 4 )	j = -1;
					break;
				default:	break;
			}
		}
		if( !pos && Pln > 3 ) {
			// 連続
			memmove( Pnum1, Pnum, 3 );
			memmove( Pnum2, &Pnum[3], 4 );
			pos++;
		}
	}
	if( pos )	{
		memset( Pnum, '\0', sizeof( Pnum ) );
		sprintf_s( Pnum, sizeof( Pnum ), "%s-%s", Pnum1, Pnum2 );
		Pln = (int)strlen( Pnum );
		memmove( SetCode, Pnum, Pln );
	}
	return( pos );
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	ICSMainFrame::OnActivate(nState, pWndOther, bMinimized);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
//--> '15.12.18 INS START
	if( nState == WA_INACTIVE ){
		return;
	}
//<-- '15.12.18 INS END

	if( m_isEndStart ) {	// 終了処理中は通さない harino ('14.03.13)
		return;
	}

	if( m_pMainView != NULL ) {
		if( m_pMainView->m_Printsw == 1 )	// プレビュー、印刷中は通さない harino ('14.03.13)
			return;
	}

	if( nState == WA_ACTIVE ) {
//		switch( m_nowTabIdx ) {
//			case 1 :	// 本表 入力
		if( m_nowTabIdx == 1 ){
				CBaseHyoView *pBaseView = GetHonHyoView();
				if( pBaseView != NULL ){
					pBaseView->SetNowPosition();
				}
//				break;
//			case 2 :	// 課税取引金額計算表
		}else if ( m_nowTabIdx == 2 ){
				CBaseTaxHyoView *pBaseTaxView = GetTaxListView();
				if( pBaseTaxView != NULL ){
//-- '15.05.02 --
//					pTaxlistView->SetNowPosition();
//---------------
					if( m_isEndStart == FALSE ){
						pBaseTaxView->SetNowPosition();
					}
//---------------
				}
//				break;
		}
	}

}

//-----------------------------------------------------------------------------
// 次回申告データ作成に必要になるデータの読込み
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::ReadDataForNextShinkoku()
{
	int st = -1;

	CBaseHyoView *pBaseView = GetHonHyoView();
	if( pBaseView != NULL ){
		st = pBaseView->ReadDataForNextShinkoku();
	}

	return st;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( pMsg->message == WM_KEYDOWN ){
		if( m_isKeyoff ){
			return TRUE;	// KeyDown制御
		}
	}

	return ICSMainFrame::PreTranslateMessage(pMsg);
}

//-----------------------------------------------------------------------------
// szvol バージョンチェック
//-----------------------------------------------------------------------------
// 引数	pZmsub	：	財務クラス
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::ChkSzvolVersion( CDBNpSub *pZmsub )
{
	ASSERT( pZmsub );
	ASSERT( pZmsub->m_database );
	if( (pZmsub==NULL) || (pZmsub->m_database==NULL) ){
		return -1;
	}

	CString	sql;
	sql.Format( _T("select SVvers, SVzper3 from szvol") );

	BOOL	findFlg = FALSE;
	CString	strSVvers, strSVzper3, strCount;

	CRecordset dbrec( pZmsub->m_database );
	try{
		dbrec.Open( CRecordset::forwardOnly, sql, CRecordset::readOnly );

		if( dbrec.IsEOF() == FALSE ){
			dbrec.GetFieldValue( _T("SVvers"), strSVvers );
			dbrec.GetFieldValue( _T("SVzper3"), strSVzper3 );
			findFlg = TRUE;
		}
	}
	catch( CDBException* dbe ){
		dbrec.Close();
		dbe->Delete();
		return -1;
	}
	dbrec.Close();

	if( findFlg ){
		if( atoi(strSVvers) == 0 ){
			// 不正バージョンなので更新
			if( atoi(strSVzper3) == 80 ){
				// 18 へ更新！
				sql.Format( _T("UPDATE szvol set SVvers=18") );
			}
			else{
				// 履歴対応版かどうかを判定
				sql.Empty();
				sql = _T("IF OBJECTPROPERTY (object_id(\'shinminfo\'),\'ISTABLE\') IS NOT NULL BEGIN \n");
				sql += _T("UPDATE szvol set SVvers=17 \n");
				sql += _T("END \n");
				sql += _T("ELSE \n");
				sql += _T("BEGIN \n");
				sql += _T("UPDATE szvol set SVvers=16 \n");
				sql += _T("END");
			}

			// バージョンアップ
			pZmsub->m_database->BeginTrans();
			try{
				pZmsub->m_database->ExecuteSQL( sql );
			}
			catch( CDBException* dbe ){
				pZmsub->m_database->Rollback();
				dbe->Delete();
				return -1;
			}
			pZmsub->m_database->CommitTrans();

			ICSMessageBox( _T("消費税申告書情報が不正な為、修復を行いました。\n消費税申告書の内容及び税額情報を確認してください。"), 0, 0, 0, this );
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// ボリュームコントロールによるマスターチェック
//-----------------------------------------------------------------------------
// 引数	coarray		：	取得した会社情報
//		rtZ_kikan	：	前年期間
//		rtHasKMst	：	過年度マスターが存在する？
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
int CMainFrame::ChkKanendoByVctl( CO_CTL_Array& coarray, int& rtZ_kikan, bool& rtHasKMst )
{
	// 現行の決算期間取得
	UCHAR	ymd[3]={0}, zymd[3]={0};

	// 年
	int n_Nen = m_pZmSub->zvol->ee_ymd / 10000;
	n_Nen %= 100;
	int_bcd( &ymd[0], n_Nen, 1 );
	// 月
	long mmdd = m_pZmSub->zvol->ee_ymd % 10000;
	int mm = mmdd / 100;
	int_bcd( &ymd[1], mm, 1 );
	// 日
	int dd = mmdd % 100;
	int_bcd( &ymd[2], dd, 1 );

	int max = (int)coarray.GetCount();
	for( int i=0; i<max; i++ ){
		if( memcmp( ymd, &coarray[i].c_kikan[3], sizeof(ymd) ) == 0 ){
			if( (i+1) < max ){
				memmove( zymd, &coarray[i+1].c_kikan[3], sizeof(zymd) );
				rtZ_kikan = bcd_int( zymd, 3 );
				rtHasKMst = true;
			}
			break;
		}
	}
	return 0;
}

#ifdef _20181212_EXTEND_
//[S]
// 過年度マスター存在チェック＆読み込み
int CMainFrame::SetKanendoData( void )
{
	char				m_Server[256]={0};			// サーバー名
	int					m_Cod, m_Apn, m_Ymd;
	unsigned char		m_1F14C[6]={0}, m_SYTX[6]={0}, m_KJADJZ[6]={0}, m_SIREZ[6]={0}, m_HKANZ[6]={0}, m_KSITZ[6]={0};

	CDBNpSub	*pZmSub=NULL;

	// 前年課税期間の消費税額
	memset( m_ZenkTax, '\0', sizeof(m_ZenkTax) );
	memset( m_ZenkSymd, '\0', sizeof(m_ZenkSymd) );
	memset( m_ZenkEymd, '\0', sizeof(m_ZenkEymd) );
	memset( m_1F14C, '\0', sizeof(m_1F14C) );
	memset( m_SYTX, '\0', sizeof(m_SYTX) );
	memset( m_KJADJZ, '\0', sizeof(m_KJADJZ) );
	memset( m_SIREZ, '\0', sizeof(m_SIREZ) );
	memset( m_HKANZ, '\0', sizeof(m_HKANZ) );
	memset( m_KSITZ, '\0', sizeof(m_KSITZ) );

	m_ZenkSg = 0;
	m_ZenkEg = 0;
	

	// UCOM情報からサーバー名を取得
	memset( m_Server, '\0', sizeof(m_Server) );
/*- '13.12.18 -*/
//#ifdef	_DEBUG
//	strcpy_s( m_Server, SERVER_NAME );
//#else
//	// マスターデバイスからサーバー名を取得
//	strcpy_s( m_Server, sizeof( m_Server ), GET_DBNAME() );
//#endif
/*-------------*/
	// マスターデバイスからサーバー名を取得
	strcpy_s( m_Server, sizeof(m_Server), GET_DBNAME() );
/*-------------*/
	// 会社コード
	m_Cod = m_pZmSub->zvol->v_cod;
	// アプリケーション番号
	m_Apn = (int)m_pZmSub->zvol->apno;
	// 過年度の期末年月日（この取り方だと期間短縮でNG）
	m_Ymd = (m_pZmSub->zvol->ee_ymd - 10000 );


//--> '15.12.22 INS START
	bool	hasKMst = false;
	int		z_kikan=0;
	ICSMCTL	icsmctl;
	if( icsmctl.SetDatabase() ){
		return ERR;
	}
	if( icsmctl.vctl_open( 0x00400001, m_Apn, 0 ) == 0 ){
		CO_CTL_DB		coctl;
		CO_CTL_Array	coarray;
		memset( &coctl, '\0', sizeof(CO_CTL_DB) );
		coctl.c_apno = m_Apn;
		int_bcd( coctl.c_cod, m_Cod, 4 );

		long	cksubsw = 0;
		if( m_pZmSub->zvol->sub_sw&0x20000 ){
			cksubsw = (ICSMCTL::ZAIMU_SW|0x20000);
		}
		else{
			cksubsw = ICSMCTL::ZAIMU_SW;
		}

		if( icsmctl.vctl_coinf( cksubsw, &coctl, &coarray ) == 0 ){
			ChkKanendoByVctl( coarray, z_kikan, hasKMst );
			icsmctl.vctl_close();
		}
		else{
			// 前期：非連動 → 当期：連動と言う形で、変化した場合の備え
			if( m_pZmSub->zvol->sub_sw&0x20000 ){
				cksubsw = ICSMCTL::ZAIMU_SW;
			}
			else{
				cksubsw = (ICSMCTL::ZAIMU_SW|0x20000);
			}
			if( icsmctl.vctl_coinf( cksubsw, &coctl, &coarray ) == 0 ){
				ChkKanendoByVctl( coarray, z_kikan, hasKMst );
				icsmctl.vctl_close();
			}
		}
	}
//<-- '15.12.22 INS END

	if( hasKMst == false ){
		return 0;
	}
	else{
		m_Ymd = z_kikan;
	}

	// 初期設定
	pZmSub = NULL;
	
	// データベース財務クラスの作成
	if( (m_pZmSub->zvol->apno&0xf0) > 0x30 ){
		pZmSub = (CDBNpSub*)new CDBNpSub;
	}
	else{
		pZmSub = (CDBNpSub*)new CDBZmSub;
	}
	if( pZmSub == NULL ){
		return ERR;
	}

	// 注意!! 過年度マスターからの取得時はチェックマスターDBを経由しない(SetMasterDBを直呼び)
	// 以降の処理は消費税申告書マスターのレコードのみ依存するため、財務マスターのバージョンの影響を受けない
	// 将来消費税マスターのレイアウトを変更するときは、これ以降の処理に影響があるか考慮して行うこと。

	if( pZmSub->SetMasterDB( 3, m_Server, m_Apn, m_Cod, m_Ymd) == ERR ){
		if( pZmSub != NULL ) {
			delete( pZmSub );
			pZmSub = NULL;
		}
		return 0;
	}

	// ボリューム・ラベル
	if( pZmSub->VolumeOpen() == ERR ){
		if( pZmSub != NULL ){
			delete( pZmSub );
			pZmSub = NULL;
		}
		return 0;
	}
	// 消費税管理
	if( pZmSub->SyohizeiOpen() == ERR ){
		// ボリューム・ラベル
		pZmSub->VolumeClose();
		if( pZmSub != NULL ){
			delete( pZmSub );
			pZmSub = NULL;
		}
		return 0;
	}
	// 消費税申告書
	if( pZmSub->SzsnkOpen("Sn_SEQ = 0") == ERR ){
		// ボリューム・ラベル
		pZmSub->VolumeClose();
		// 消費税管理
		pZmSub->SyohizeiClose();
		if( pZmSub != NULL ){
			delete( pZmSub );
			pZmSub = NULL;
		}
		return 0;
	}
	if( pZmSub->szsnk->st == ERR ){
		// ボリューム・ラベル
		pZmSub->VolumeClose();
		// 消費税管理
		pZmSub->SyohizeiClose();
		if( pZmSub != NULL ){
			delete( pZmSub );
			pZmSub = NULL;
		}
		return 0;
	}

	// 計算金額の取得
	if( pZmSub->szsnk->Sn_VER >= ICS_SH_H26_ETAX_VER ){
		GetLastYearTax( pZmSub, m_ZenkTax );
	}
	else if( pZmSub->szsnk->Sn_VER == ICS_SH_H25_ETAX_VER ){
		// 合計差引税額
		m_Util.val_to_bin( m_1F14C,	pZmSub->szsnk->Sn_1F14C );
		// 消費税額（その他）
		m_Util.val_to_bin( m_SYTX,	pZmSub->szsnk->Sn_SYTX );
		// 貸倒回収に係る税額（控除過大調整税額）
		m_Util.val_to_bin( m_KJADJZ,	pZmSub->szsnk->Sn_KJADJZ );
		// 控除対象仕入税額
		m_Util.val_to_bin( m_SIREZ,	pZmSub->szsnk->Sn_SIREZ );
		// 返還等対価に係る金額
		m_Util.val_to_bin( m_HKANZ,	pZmSub->szsnk->Sn_HKANZ );
		// 貸倒に係る税額
		m_Util.val_to_bin( m_KSITZ,	pZmSub->szsnk->Sn_KSITZ );

		if( pZmSub->szvol->SVmzsw ){							// 経過措置有り
			if( m_Arith.l_test( m_1F14C ) > 0 ){
				memmove( m_ZenkTax, m_1F14C, 6 );
				// １００以下の切り捨て
				m_Util.l_calq( m_ZenkTax, m_ZenkTax, 0x10 );
			}
		}
		else{													// 経過措置無し
			m_Arith.l_add( m_ZenkTax, m_ZenkTax, m_SYTX );		// ＋消費税額②
			m_Arith.l_add( m_ZenkTax, m_ZenkTax, m_KJADJZ );	// ＋控除過大調整税額③
			m_Arith.l_sub( m_ZenkTax, m_ZenkTax, m_SIREZ );		// －控除対象仕入税額④
			m_Arith.l_sub( m_ZenkTax, m_ZenkTax, m_HKANZ );		// －返還等対価に係る税額⑤
			m_Arith.l_sub( m_ZenkTax, m_ZenkTax, m_KSITZ );		// －貸倒に係る税額⑥
			// 限界控除前の税額がマイナスの時
			if( m_Arith.l_test(m_ZenkTax) > 0 ){
				// １００以下の切り捨て
				m_Util.l_calq( m_ZenkTax, m_ZenkTax, 0x10 );
			}
		}
	}

////★//[13'07.01]///
	m_Util.date_to_bcd( 0x04, (unsigned char *)m_ZenkSymd, pZmSub->szsnk->Sn_KDAYS );	// 課税期間　自
	m_Util.date_to_bcd( 0x04, (unsigned char *)m_ZenkEymd, pZmSub->szsnk->Sn_KDAYE );	// 課税期間　至
/////////////////////

	// ボリューム・ラベル
	pZmSub->VolumeClose();
	// 消費税管理
	pZmSub->SyohizeiClose();
	// 消費税申告書
	pZmSub->SzsnkClose();
	if( pZmSub != NULL ) {
		delete( pZmSub );
		pZmSub = NULL;
	}
	// 消費税額がない場合は何もしない。
	if( m_Arith.l_test(m_ZenkTax) == 0 ){
		return 0;
	}

	return 1;
}
#else
//[S]
// 過年度マスター存在チェック＆読み込み
int CMainFrame::SetKanendoData( void )
{
	// 処理を凍結。
	// 関数の呼び出しを全てクローズしましたが、この関数自体にも問題があるので処理をクローズ。
	return 1;


	char				m_Server[256]={0};			// サーバー名
	int					m_Cod, m_Apn, m_Ymd;
	unsigned char		m_1F14C[6]={0}, m_SYTX[6]={0}, m_KJADJZ[6]={0}, m_SIREZ[6]={0}, m_HKANZ[6]={0}, m_KSITZ[6]={0};

	CDBNpSub	*pZmSub=NULL;

	// 前年課税期間の消費税額
	memset( m_ZenkTax, '\0', sizeof(m_ZenkTax) );
	memset( m_ZenkSymd, '\0', sizeof(m_ZenkSymd) );
	memset( m_ZenkEymd, '\0', sizeof(m_ZenkEymd) );
	memset( m_1F14C, '\0', sizeof(m_1F14C) );
	memset( m_SYTX, '\0', sizeof(m_SYTX) );
	memset( m_KJADJZ, '\0', sizeof(m_KJADJZ) );
	memset( m_SIREZ, '\0', sizeof(m_SIREZ) );
	memset( m_HKANZ, '\0', sizeof(m_HKANZ) );
	memset( m_KSITZ, '\0', sizeof(m_KSITZ) );

	// UCOM情報からサーバー名を取得
	memset( m_Server, '\0', sizeof(m_Server) );
/*- '13.12.18 -*/
//#ifdef	_DEBUG
//	strcpy_s( m_Server, SERVER_NAME );
//#else
//	// マスターデバイスからサーバー名を取得
//	strcpy_s( m_Server, sizeof( m_Server ), GET_DBNAME() );
//#endif
/*-------------*/
	// マスターデバイスからサーバー名を取得
	strcpy_s( m_Server, sizeof(m_Server), GET_DBNAME() );
/*-------------*/
	// 会社コード
	m_Cod = m_pZmSub->zvol->v_cod;
	// アプリケーション番号
	m_Apn = (int)m_pZmSub->zvol->apno;
	// 過年度の期末年月日（この取り方だと期間短縮でNG）
	m_Ymd = (m_pZmSub->zvol->ee_ymd - 10000 );


//--> '15.12.22 INS START
	bool	hasKMst = false;
	int		z_kikan=0;
	ICSMCTL	icsmctl;
	if( icsmctl.SetDatabase() ){
		return ERR;
	}
	if( icsmctl.vctl_open( 0x00400001, m_Apn, 0 ) == 0 ){
		CO_CTL_DB		coctl;
		CO_CTL_Array	coarray;
		memset( &coctl, '\0', sizeof(CO_CTL_DB) );
		coctl.c_apno = m_Apn;
		int_bcd( coctl.c_cod, m_Cod, 4 );

		long	cksubsw = 0;
		if( m_pZmSub->zvol->sub_sw&0x20000 ){
			cksubsw = (ICSMCTL::ZAIMU_SW|0x20000);
		}
		else{
			cksubsw = ICSMCTL::ZAIMU_SW;
		}

		if( icsmctl.vctl_coinf( cksubsw, &coctl, &coarray ) == 0 ){
			ChkKanendoByVctl( coarray, z_kikan, hasKMst );
			icsmctl.vctl_close();
		}
		else{
			// 前期：非連動 → 当期：連動と言う形で、変化した場合の備え
			if( m_pZmSub->zvol->sub_sw&0x20000 ){
				cksubsw = ICSMCTL::ZAIMU_SW;
			}
			else{
				cksubsw = (ICSMCTL::ZAIMU_SW|0x20000);
			}
			if( icsmctl.vctl_coinf( cksubsw, &coctl, &coarray ) == 0 ){
				ChkKanendoByVctl( coarray, z_kikan, hasKMst );
				icsmctl.vctl_close();
			}
		}
	}
//<-- '15.12.22 INS END

	if( hasKMst == false ){
		return 0;
	}
	else{
		m_Ymd = z_kikan;
	}

	// 初期設定
	pZmSub = NULL;
	
	// データベース財務クラスの作成
	if( (m_pZmSub->zvol->apno&0xf0) > 0x30 ){
		pZmSub = (CDBNpSub*)new CDBNpSub;
	}
	else{
		pZmSub = (CDBNpSub*)new CDBZmSub;
	}
	if( pZmSub == NULL ){
		return ERR;
	}

	// 注意!! 過年度マスターからの取得時はチェックマスターDBを経由しない(SetMasterDBを直呼び)
	// 以降の処理は消費税申告書マスターのレコードのみ依存するため、財務マスターのバージョンの影響を受けない
	// 将来消費税マスターのレイアウトを変更するときは、これ以降の処理に影響があるか考慮して行うこと。

	if( pZmSub->SetMasterDB( 3, m_Server, m_Apn, m_Cod, m_Ymd) == ERR ){
		if( pZmSub != NULL ) {
			delete( pZmSub );
			pZmSub = NULL;
		}
		return 0;
	}

	if( pZmSub->szsnk->Sn_VER >= ICS_SH_H26_ETAX_VER ){
		// 必須テーブルオープン
		//== ボリューム・ラベル ==
		if( pZmSub->VolumeOpen() == ERR ){
			if( pZmSub != NULL ){
				delete( pZmSub );
				pZmSub = NULL;
			}
			return 0;
		}
		// 消費税管理
		if( pZmSub->SyohizeiOpen() == ERR ){
			// ボリューム・ラベル
			pZmSub->VolumeClose();
			if( pZmSub != NULL ){
				delete( pZmSub );
				pZmSub = NULL;
			}
			return 0;
		}
		// 消費税申告書
		if( pZmSub->SzsnkOpen("Sn_SEQ = 0") == ERR ){
			// ボリューム・ラベル
			pZmSub->VolumeClose();
			// 消費税管理
			pZmSub->SyohizeiClose();
			if( pZmSub != NULL ){
				delete( pZmSub );
				pZmSub = NULL;
			}
			return 0;
		}
		if( pZmSub->szsnk->st == ERR ){
			// ボリューム・ラベル
			pZmSub->VolumeClose();
			// 消費税管理
			pZmSub->SyohizeiClose();
			if( pZmSub != NULL ){
				delete( pZmSub );
				pZmSub = NULL;
			}
			return 0;
		}

		GetLastYearTax( pZmSub, m_ZenkTax );
	}
	else{
		// ボリューム・ラベル
		if( pZmSub->VolumeOpen() == ERR ){
			if( pZmSub != NULL ){
				delete( pZmSub );
				pZmSub = NULL;
			}
			return 0;
		}
		// 消費税管理
		if( pZmSub->SyohizeiOpen() == ERR ){
			// ボリューム・ラベル
			pZmSub->VolumeClose();
			if( pZmSub != NULL ){
				delete( pZmSub );
				pZmSub = NULL;
			}
			return 0;
		}
		// 消費税申告書
		if( pZmSub->SzsnkOpen("Sn_SEQ = 0") == ERR ){
			// ボリューム・ラベル
			pZmSub->VolumeClose();
			// 消費税管理
			pZmSub->SyohizeiClose();
			if( pZmSub != NULL ){
				delete( pZmSub );
				pZmSub = NULL;
			}
			return 0;
		}
		if( pZmSub->szsnk->st == ERR ){
			// ボリューム・ラベル
			pZmSub->VolumeClose();
			// 消費税管理
			pZmSub->SyohizeiClose();
			if( pZmSub != NULL ){
				delete( pZmSub );
				pZmSub = NULL;
			}
			return 0;
		}

		// 合計差引税額
		m_Util.val_to_bin( m_1F14C,	pZmSub->szsnk->Sn_1F14C );
		// 消費税額（その他）
		m_Util.val_to_bin( m_SYTX,	pZmSub->szsnk->Sn_SYTX );
		// 貸倒回収に係る税額（控除過大調整税額）
		m_Util.val_to_bin( m_KJADJZ,	pZmSub->szsnk->Sn_KJADJZ );
		// 控除対象仕入税額
		m_Util.val_to_bin( m_SIREZ,	pZmSub->szsnk->Sn_SIREZ );
		// 返還等対価に係る金額
		m_Util.val_to_bin( m_HKANZ,	pZmSub->szsnk->Sn_HKANZ );
		// 貸倒に係る税額
		m_Util.val_to_bin( m_KSITZ,	pZmSub->szsnk->Sn_KSITZ );

		if( pZmSub->szvol->SVmzsw ){							// 経過措置有り
			if( m_Arith.l_test( m_1F14C ) > 0 ){
				memmove( m_ZenkTax, m_1F14C, 6 );
				// １００以下の切り捨て
				m_Util.l_calq( m_ZenkTax, m_ZenkTax, 0x10 );
			}
		}
		else{													// 経過措置無し
			m_Arith.l_add( m_ZenkTax, m_ZenkTax, m_SYTX );		// ＋消費税額②
			m_Arith.l_add( m_ZenkTax, m_ZenkTax, m_KJADJZ );	// ＋控除過大調整税額③
			m_Arith.l_sub( m_ZenkTax, m_ZenkTax, m_SIREZ );		// －控除対象仕入税額④
			m_Arith.l_sub( m_ZenkTax, m_ZenkTax, m_HKANZ );		// －返還等対価に係る税額⑤
			m_Arith.l_sub( m_ZenkTax, m_ZenkTax, m_KSITZ );		// －貸倒に係る税額⑥
			// 限界控除前の税額がマイナスの時
			if( m_Arith.l_test(m_ZenkTax) > 0 ){
				// １００以下の切り捨て
				m_Util.l_calq( m_ZenkTax, m_ZenkTax, 0x10 );
			}
		}
	}

	m_Util.date_to_bcd( 0x04, (unsigned char *)m_ZenkSymd, pZmSub->szsnk->Sn_KDAYS );	// 課税期間　自
	m_Util.date_to_bcd( 0x04, (unsigned char *)m_ZenkEymd, pZmSub->szsnk->Sn_KDAYE );	// 課税期間　至

	// ボリューム・ラベル
	pZmSub->VolumeClose();
	// 消費税管理
	pZmSub->SyohizeiClose();
	// 消費税申告書
	pZmSub->SzsnkClose();
	if( pZmSub != NULL ) {
		delete( pZmSub );
		pZmSub = NULL;
	}
	// 消費税額がない場合は何もしない。
	if( m_Arith.l_test(m_ZenkTax) == 0 ){
		return 0;
	}

	return 1;
}
#endif

//-----------------------------------------------------------------------------
// 既確定税額のクリア('15.01.28)
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::ClearAlreadyDecision()
{
	CBaseHyoView *pBaseView = GetHonHyoView();
	if( pBaseView != NULL ){
		return pBaseView->ClearAlreadyDecision();
	}
	return 0;
}

//-----------------------------------------------------------------------------
// 税務署入力にフォーカス誘導('15.02.03)
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::GuideTaxOfficeInput()
{
	if( m_nowTabIdx == 1 ){
		CBaseHyoView *pBaseView = GetHonHyoView();
		if( pBaseView != NULL ){
			return pBaseView->GuideTaxOfficeInput();
		}
	}
	else{
		SetActiveChild( 1 );
//2016.06.22 INSERT START
		m_nowTabIdx = 1;
//2016.06.22 INSERT END
		CBaseHyoView *pBaseView = GetHonHyoView();
		if( pBaseView != NULL ){
			return pBaseView->GuideTaxOfficeInput();
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 消費税簡易計算表　ダイアログ表示
//-----------------------------------------------------------------------------
LRESULT CMainFrame::OnUserDispSpecpass( WPARAM wParam, LPARAM lParam ) 
{
	CBaseTaxHyoView *pBaseTaxView = GetTaxListView();

	if( pBaseTaxView ){
//2015.03.05 UPDATE START
		//if( wParam == 0 ){
		//	pTaxationListView->SpecPassOffDlg();
		//}
		//else if( wParam == 1 ){
		//	pTaxationListView->SpecPassOnDlg();
		//}

		pBaseTaxView->SpecPassOffExDlg();
//2015.03.05 UPDATE END
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 簡易課税第六種フラグをセット('15.02.20)
//-----------------------------------------------------------------------------
// 引数	SixKindFlg	：	簡易課税第六種フラグ
//-----------------------------------------------------------------------------
void CMainFrame::SetSixKindFlg( BOOL SixKindFlg )
{
	CBaseHyoView *pBaseView = GetHonHyoView();
	if( pBaseView != NULL ){
		pBaseView->SetSixKindFlg( SixKindFlg );
	}
}

//-----------------------------------------------------------------------------
// 還付申告？('15.02.24)
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	還付申告
//			FALSE	：	還付申告でない
//-----------------------------------------------------------------------------
BOOL CMainFrame::IsKanpuShinkoku()
{
	BOOL	bRt = FALSE;

	CBaseHyoView *pBaseView = GetHonHyoView();
	if( pBaseView != NULL ){
		bRt = pBaseView->IsKanpuShinkoku();
	}

	return bRt;
}
//2016.06.22 INSERT START
//-----------------------------------------------------------------------------
// 還付申告？(中間納付税額、中間納付譲渡割額加味)
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	還付申告
//			FALSE	：	還付申告でない
//-----------------------------------------------------------------------------
BOOL CMainFrame::IsKanpuShinkoku2()
{
	BOOL	bRt = FALSE;

	CBaseHyoView *pBaseView = GetHonHyoView();
	if( pBaseView != NULL ){
		bRt = pBaseView->IsKanpuShinkoku2();
		//if( pH26View->IsKanpuShinkoku() || pH26View->IsKanpuShinkoku2() ){
		//	bRt = TRUE;
		//}
	}

	return bRt;
}
//2016.06.22 INSERT END
//-----------------------------------------------------------------------------
// 特定収入計算表　金額連動
//-----------------------------------------------------------------------------
LRESULT CMainFrame::OnUserSpcpostRecalq( WPARAM wParam, LPARAM lParam ) 
{
	SPCPOST_DATA	*pPostData = (SPCPOST_DATA*)wParam;
	ASSERT( pPostData );
	if( pPostData == NULL ){
		return 0;
	}

	if( !(pPostData->Sn_Sign2&0x02) ){
		char	mony[MONY_BUF_SIZE]={0};
		CString	tag, chtag;

		if ( GetActViewVer() == ID_VER_SYZ26 ) {
			tag = _T("AEG00020");
			SetMony( tag, chtag, (char*)pPostData->Sn_2F21A );

			tag = _T("AEG00030");
			SetMony( tag, chtag, (char*)pPostData->Sn_2F21B );

			tag = _T("AEG00040");
			if( m_SnHeadData.SVmzsw ){
				chtag.Format( _T("%d"), 0x111 );
			}
			else{
				chtag.Format( _T("%d"), 0x100 );
			}
			SetMony( tag, chtag, (char*)pPostData->Sn_2F21C );
		}
		else if ( GetActViewVer() == ID_VER_SYZ31 ) {
			// 4%
			if ( m_Arith.l_test ( pPostData->Sn_202_23B ) < 0 ) {
				// 付表2-2 24B欄へ
				tag = _T( "AEG00070" );
				m_Arith.l_neg ( pPostData->Sn_202_23B );
				SetMony ( tag, chtag, ( char* )pPostData->Sn_202_23B );
			}
			else {
				// 付表2-2 23B欄へ
				tag = _T( "AEG00030" );
				SetMony ( tag, chtag, ( char* )pPostData->Sn_202_23B );

				// 付表1-2 4B欄へ
				tag = _T( "BAE00030" );
				SetMony ( tag, chtag, ( char* )pPostData->Sn_202_23B );
			}

			// 6.3%
			if ( m_Arith.l_test ( pPostData->Sn_202_23C ) < 0 ) {
				// 付表2-2 24C欄へ
				tag = _T( "AEG00075" );
				m_Arith.l_neg ( pPostData->Sn_202_23C );
				SetMony ( tag, chtag, ( char* )pPostData->Sn_202_23C );
			}
			else {
				// 付表2-2 23C欄へ
				tag = _T( "AEG00035" );
				SetMony ( tag, chtag, ( char* )pPostData->Sn_202_23C );

				// 付表1-2 4C欄へ
				tag = _T( "BAE00040" );
				SetMony ( tag, chtag, ( char* )pPostData->Sn_202_23C );
			}

			// 6.24%
			if ( m_Arith.l_test ( pPostData->Sn_201_23D ) < 0 ) {
				// 付表2-1 24D欄へ
				tag = _T( "CRI00080" );
				m_Arith.l_neg ( pPostData->Sn_201_23D );
				SetMony ( tag, chtag, ( char* )pPostData->Sn_201_23D );
			}
			else {
				// 付表2-1 23D欄へ
				tag = _T( "CRI00030" );
				SetMony ( tag, chtag, ( char* )pPostData->Sn_201_23D );

				// 付表1-1 4D欄へ
				tag = _T( "CQF00030" );
				SetMony ( tag, chtag, ( char* )pPostData->Sn_201_23D );
			}

			// 7.8%
			if ( m_Arith.l_test ( pPostData->Sn_201_23E ) < 0 ) {
				// 付表2-1 24E欄へ
				tag = _T( "CRI00090" );
				m_Arith.l_neg ( pPostData->Sn_201_23E );
				SetMony ( tag, chtag, ( char* )pPostData->Sn_201_23E );
			}
			else {
				// 付表2-1 23E欄へ
				tag = _T( "CRI00040" );
				SetMony ( tag, chtag, ( char* )pPostData->Sn_201_23E );

				// 付表1-1 4E欄へ
				tag = _T( "CQF00040" );
				SetMony ( tag, chtag, ( char* )pPostData->Sn_201_23E );
			}
		}
	}
	else{
		char	mony[MONY_BUF_SIZE]={0};
		CString	tag, chtag;
		tag = _T("AEG00040");
		if( m_SnHeadData.SVmzsw ){
			chtag.Format( _T("%d"), 0x000 );
		}
		else{
			chtag.Format( _T("%d"), 0x000 );
		}
		SetMony( tag, chtag, (char*)pPostData->Sn_2F21C );
	}

	if( m_nowTabIdx == 2 ){
		CBaseHyoView *pBaseView = GetHonHyoView();
		if( pBaseView && m_pMainView ){
//2017.03.01 UPDATE START
			pBaseView->UpdateAllSheet(1);
//2017.03.01 UPDATE END
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 仕入額変更による特定収入の再計算
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
LRESULT CMainFrame::OnUserSkjpostRecalq( WPARAM wParam, LPARAM lParam )
{
	// 調整前 控除対象仕入税額の取得
	SYZTOSPC_DATA	*pSyzToSpc = (SYZTOSPC_DATA*)wParam;

	// 特定収入の再計算
	CBaseTaxHyoView *pBaseTaxView = GetTaxListView();
	if( pBaseTaxView != NULL ){
		pBaseTaxView->RecalqSpc( pSyzToSpc );
	}

	return 0;
}

//-----------------------------------------------------------------------------
// フォーカスを入力画面に戻す('15.04.13)
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::SetFocusToInputPos()
{
	if( m_nowTabIdx == 1 ){
		CBaseHyoView *pBaseView = GetHonHyoView();
		if( pBaseView != NULL ){
			pBaseView->SetNowPosition();
		}
	}
	else if( m_nowTabIdx == 2 ){
		CBaseTaxHyoView *pBaseTaxView = GetTaxListView();
		if( pBaseTaxView != NULL ){
			pBaseTaxView->SetNowPosition(TRUE);
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------
// ８％の特定収入もチェックする('19.11.27)
// ５％の特定収入アリ？('15.04.27)
//-----------------------------------------------------------------------------
// 引数	sofs		：	開始月オフセット
//		eofs		：	終了月オフセット
//		endYymmdd	：	指定期間の末日
//-----------------------------------------------------------------------------
// 返送値	1		：	レコード有り
//			0		：	レコード無し
//-----------------------------------------------------------------------------
int CMainFrame::Chk5PerSpc( int sofs, int eofs, int endYymmdd )
{
	int rt = 0;

	EnumIdIcsShKazeihoushiki kzhousiki = m_Util.GetKazeihoushiki( m_pZmSub->zvol );
/*	if( endYymmdd >= ICS_SH_8PER_REFORM_DAY ){
		if( kzhousiki == ID_ICSSH_GENSOKU ){
			// この時のみチェック
			CRecordset	rs(m_pZmSub->m_database);
			CString	sql;
			sql = _T("select top 1 SPzkbn from dbo.spsyk where SPzkbn = 0");
			try{
				rs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly);
				if( rs.IsEOF() == FALSE ){
					// 5%のレコードあり
					rt = 1;
				}
			}
			catch( CDBException *pde ){
				pde->Delete();
				return -1;
			}
			rs.Close();
		}
		if( rt ){
			if( Chk5PerSpcDetail(sofs, eofs) != 1 ){
				rt = 0;
			}
		}
	}
*/

	if ( kzhousiki == ID_ICSSH_GENSOKU ) {
		CRecordset rs ( m_pZmSub->m_database );
		CString sql;

		if ( endYymmdd >= ICS_SH_10PER_REFORM_DAY ) {
			sql = _T( "select top 1 SPzkbn from dbo.spsyk where SPzkbn = 0 or SPzkbn = 3" );
		}
		else if ( endYymmdd >= ICS_SH_8PER_REFORM_DAY ){
			sql = _T( "select top 1 SPzkbn from dbo.spsyk where SPzkbn = 0" );
		}
		else {
			return 0;
		}

		try{
			rs.Open ( CRecordset::forwardOnly, sql, CRecordset::readOnly );
			if ( rs.IsEOF() == FALSE ) {
				rt = 1;
			}
		}
		catch ( CDBException *pde ) {
			if ( rs.IsOpen() ) {
				rs.Close();
			}
			pde->Delete();
			return -1;
		}
		rs.Close();

		if ( rt ) {
			if ( Chk5PerSpcDetail ( sofs, eofs, endYymmdd ) != 1 ) {
				rt = 0;
			}
		}
	}

	return rt;
}

//-----------------------------------------------------------------------------
// ８％の特定収入の詳細もチェック('19.11.27)
// ５％の特定収入の詳細チェック('15.04.27)
//-----------------------------------------------------------------------------
// 引数	sofs		：	開始月オフセット
//		eofs		：	終了月オフセット
//		endyymmdd	：	指定期間の末日('19.11.27)
//-----------------------------------------------------------------------------
// 返送値	1		：	レコード有り
//			0		：	レコード無し
//-----------------------------------------------------------------------------
//int CMainFrame::Chk5PerSpcDetail( int sofs, int eofs )
int CMainFrame::Chk5PerSpcDetail( int sofs, int eofs, int endYymmdd )
{
	int rt = 0;

	BOOL	openFlg = FALSE;
	CString	filter;

	if ( endYymmdd >= ICS_SH_10PER_REFORM_DAY ) {
		filter.Format( _T("SPzkbn = 0 or SPzkbn = 3") );
	}
	else if ( endYymmdd >= ICS_SH_8PER_REFORM_DAY ){
		filter.Format( _T("SPzkbn = 0") );
	}
	else {
		return 0;
	}

	if( m_pZmSub->m_pSpsyk == NULL ){
		if( m_pZmSub->NpTokuteiSyunyuOpen(filter) == -1 ){
			return -1;
		}
		openFlg = TRUE;
	}
	else{
		m_pZmSub->m_pSpsyk->RequeryEx(filter);
	}

	for( int i=0;; i++ ){
		if( i==0 ){
			if( m_pZmSub->m_pSpsyk->MoveFirstEx() ){
				break;
			}
		}
		else{
			if( m_pZmSub->m_pSpsyk->MoveNextEx() ){
				break;
			}
		}

		for( int j=sofs; j<=eofs; j++ ){
			if( ((m_pZmSub->m_pSpsyk->m_SPtkzn[j].GetLength())&&(m_pZmSub->m_pSpsyk->m_SPtkzn[j]!=_T("0"))) ||
				((m_pZmSub->m_pSpsyk->m_SPthzn[j].GetLength())&&(m_pZmSub->m_pSpsyk->m_SPthzn[j]!=_T("0"))) ||
				((m_pZmSub->m_pSpsyk->m_SPtczn[j].GetLength())&&(m_pZmSub->m_pSpsyk->m_SPtczn[j]!=_T("0"))) ||
				((m_pZmSub->m_pSpsyk->m_SPtszn[j].GetLength())&&(m_pZmSub->m_pSpsyk->m_SPtszn[j]!=_T("0"))) ){

				rt = 1;
				break;
			}
		}
		if( rt ){
			break;
		}
	}

	if( openFlg ){
		m_pZmSub->NpTokuteiSyunyuClose();
	}

	return rt;
}

//-----------------------------------------------------------------------------
// 簡易課税第六種？('15.05.01)
//-----------------------------------------------------------------------------
// 引数		symd	：	課税期間の開始日
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	簡易課税第六種
//			FALSE	：	簡易課税第六種でない
//-----------------------------------------------------------------------------
BOOL CMainFrame::IsSixKindKani( long symd )
{
	BOOL	bRt = FALSE;

	ASSERT(m_pZmSub);
	ASSERT(m_pZmSub->zvol);
	if( (m_Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_KANNI) && (symd>=ICS_SH_SIXKIND_REFORM_DAY) ){
		bRt = TRUE;
	}

	return bRt;
}

//-----------------------------------------------------------------------------
// 終了処理開始フラグをONにする
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
LRESULT CMainFrame::OnUserEndStart( WPARAM wParam, LPARAM lParam )
{
//	m_isEndStart = TRUE;
	SetEndStartFlg( FALSE );

//--> '15.11.17 INS START
	if( wParam == 1 ){
		m_isNeedChkTkkz = FALSE;
	}
	else{
		m_isNeedChkTkkz = TRUE;
	}
//<-- '15.11.17 INS ENE

	return 0;
}

//-----------------------------------------------------------------------------
// 削除履歴を登録
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CMainFrame::RegistDelHistory()
{
	NOHISTTBL	nhtbl;		// マイナンバー履歴データテーブル

	NTBMOD8_BeginProc( 0, NULL );
	NTBMOD8_InitNoHistoryTbl( 0, &nhtbl );
	SetNoHistory( nhtbl );
	SetCoNoHistory( nhtbl );

	nhtbl.user_play |= (NTBMOD_USER_PLAY_DELETE|NTBMOD_USER_PLAY_APPLI);

	CNoHistoryTblArray nhtblary;
	nhtblary.Add( nhtbl );

//--> '15.09.02 INS START
	if( MynoAccessPerm() == FALSE ){
		return 0;
	}
//<-- '15.09.02 INS END

	// 履歴テーブルにセット
	NTBMOD8_SetNoHistoryTbl( 0, &nhtblary );
	NTBMOD8_EndProc();

	return 0;
}

//-----------------------------------------------------------------------------
// 個人番号履歴情報セット
//-----------------------------------------------------------------------------
// 引数	nhtbl	：	履歴情報のセット先
//-----------------------------------------------------------------------------
void CMainFrame::SetNoHistory( NOHISTTBL &nhtbl )
{
	nhtbl.mst_code = m_pZmSub->zvol->v_cod;
	nhtbl.mst_apno = m_pZmSub->zvol->apno;
//2018.08.21 UPDATE START
	//int len =  m_Util.MixSpaceCutLength( (char *)m_SnHeadData.Sn_DIHYO, sizeof(m_SnHeadData.Sn_DIHYO) );
	//if( len ){
	//	memmove( nhtbl.mst_nam, m_SnHeadData.Sn_DIHYO, sizeof(m_SnHeadData.Sn_DIHYO) );
	//}
	//else{
	//	memmove( nhtbl.mst_nam, m_pZmSub->zvol->c_nam, min(sizeof(nhtbl.mst_nam), m_pZmSub->zvol->c_nam.GetLength()) );
	//}

		memmove( nhtbl.mst_nam, m_pZmSub->zvol->c_nam, min(sizeof(nhtbl.mst_nam), m_pZmSub->zvol->c_nam.GetLength()) );
	memmove( nhtbl.tgt_char, m_SnHeadData.Sn_DIHYO, sizeof(m_SnHeadData.Sn_DIHYO) );

//2018.08.21 UPDATE END

//2018.08.21 INSERT START
	CDBSyzShinMainView	*pView = (CDBSyzShinMainView *)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(BSINFO_VIEW_WND_IDX));

	if( pView != NULL ){
		CString mst_nam_str,kessan_str;
		
		mst_nam_str = nhtbl.mst_nam;
		mst_nam_str = mst_nam_str.TrimRight();
		m_SnHeadData.GetString_KessanKikan( &kessan_str );
		strcpy_s( nhtbl.mst_nam, sizeof(nhtbl.mst_nam), mst_nam_str + kessan_str );
	}
//2018.08.21 INSERT END

	strcpy_s( nhtbl.gy_nam, sizeof(nhtbl.gy_nam), _T("消費税申告書") );
}

//-----------------------------------------------------------------------------
// 個人番号履歴情報セット（顧問先情報）
//-----------------------------------------------------------------------------
// 引数	nhtbl	：	履歴情報のセット先
//-----------------------------------------------------------------------------
void CMainFrame::SetCoNoHistory( NOHISTTBL &nhtbl )
{
//-- '16.01.30 --
//	if( m_TgPDat.TgCode == 0 ){
//---------------
	if( m_TgPDat.TgCode == -1 ){
//---------------
		return;
	}

	nhtbl.kmn_code = m_TgPDat.TgCode;
	nhtbl.kmn_hkkbn = m_TgPDat.HKkbn;
	memmove( nhtbl.kmn_nam, m_TgPDat.TgName, sizeof(m_TgPDat.TgName) );
}

//-------------------->特定課税仕入対応150713
//-----------------------------------------------------------------------------
// 特定課税仕入（チェックボックス出すかどうか）
//-----------------------------------------------------------------------------
// 引数		symd	：	課税期間の期末日
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	チェックボックス表示
//			FALSE	：	チェックボックス非表示
//-----------------------------------------------------------------------------
BOOL CMainFrame::IsTokutei( long eymd )
{
	BOOL	bRt = FALSE;

	ASSERT(m_pZmSub);
	ASSERT(m_pZmSub->zvol);
	if( (m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU) && (eymd >= ICS_SH_TOKUTEI_REFORM_DAY) ){
		bRt = TRUE;
	}

	return bRt;
}

//-----------------------------------------------------------------------------
// 改正様式をセット('15.08.27)
//-----------------------------------------------------------------------------
void CMainFrame::SetShRevType( EnumIdIcsShRevType shrevtype )
{
	CBaseHyoView *pBaseView = GetHonHyoView();
	if( pBaseView != NULL ){
		pBaseView->SetShRevType( shrevtype );
	}
}
//<-----------------------------------------------------

//-----------------------------------------------------------------------------
// 特定課税仕入を『有』から『無し』へ
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
LRESULT CMainFrame::OnUserResetTkkz( WPARAM wParam, LPARAM lParam )
{
	// 第一画面へ
	SetActiveChild( 0 );

	// 特定課税仕入をOFFに
	if( m_pMainView ){
		m_pMainView->SetTkkzChk( 0 );
	}

	// データ書き込み
	CBaseHyoView *pBaseView = GetHonHyoView();
	if( pBaseView != NULL ){
		pBaseView->WriteData( FALSE, FALSE );
	}

	return 0;
}

//-----------------------------------------------------------------------------
// フォーカスの再セット
//-----------------------------------------------------------------------------
LRESULT CMainFrame::OnUserRefocusPrint( WPARAM wParam, LPARAM lParam )
{
	if( m_nowTabIdx == 1 ){
		CBaseHyoView *pBaseView = GetHonHyoView();
		if( pBaseView != NULL ){
			pBaseView->SetNowPosition();
		}
	}else if ( m_nowTabIdx == 2 ){
		CBaseTaxHyoView *pBaseTaxView = GetTaxListView();
		if( pBaseTaxView != NULL ){
			if( m_isEndStart == FALSE ){
				pBaseTaxView->SetNowPosition();
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 入力ロック切替時の再表示
//-----------------------------------------------------------------------------
LRESULT CMainFrame::OnUserChgLockDisp( WPARAM wParam, LPARAM lParam )
{
	CBaseTaxHyoView *pBaseTaxView = GetTaxListView();
	if( pBaseTaxView != NULL ){
		pBaseTaxView->ChgLockDisp();
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 還付申告のデータ再セット
//-----------------------------------------------------------------------------
// 引数	wParam	：	1:画面の更新をする, 0:画面の更新をしない
//		lParam	：	どこまでの情報を更新するのか
//					　0:全データ, 1:税額データのみ
//-----------------------------------------------------------------------------
LRESULT CMainFrame::OnUserResetRefund( WPARAM wParam, LPARAM lParam )
{
	bool isUpdDisp = false;
	if( wParam == 1 ){
		isUpdDisp = true;
	}

	if( lParam == 1 ){
		ConnectedSkjZeigaku(isUpdDisp);
	}
	else{
		ConnectedSkj(isUpdDisp);
	}

	return 0;
}

void CMainFrame::OnMove(int x, int y)
{
	ICSMainFrame::OnMove(x, y);

	// TODO: ここにメッセージ ハンドラ コードを追加します。

	if( m_pMainView ){
		m_pMainView->RedrawGroupText();
	}
}

//2016.06.22 INSERT START
void CMainFrame::OnKoketsuMeisyo()
{

	int		index;
	BOOL	minmax;
	CWnd *pWnd = GetActiveChild( &index, &minmax );

	if( index == 2 ){	// 課税取引金額計算書
		CBaseTaxHyoView *pBaseTaxView = GetTaxListView();
		if( pBaseTaxView != NULL ){
			int Koketsu = 0;
			if( pBaseTaxView->ChgKoketsuMeisyoGet(&Koketsu) == IDOK ){

			}
		}
	}
}

void CMainFrame::OnUpdateKoketsuMeisyo(CCmdUI *pCmdUI)
{
	if( m_Util.IsUnConnectMst(m_pZmSub->zvol) ){ //非連動ｄｂ
		pCmdUI->Enable( FALSE );
	}else
	if( (m_pZmSub->zvol->apno&0xf0) == 0x50 ){ // 学校・公益・福祉・宗教
		pCmdUI->Enable( FALSE );
	}
	else{
		if( m_nowTabIdx == 2 ){

			if( m_SnHeadData.Sn_Sign4&0x01 ){ //入力ロック
				pCmdUI->Enable( FALSE );
			}
			else{
				pCmdUI->Enable( TRUE );
			}
		}
		else{
			pCmdUI->Enable( FALSE );
		}
	}
}

void CMainFrame::OnKoketsuKingaku()
{
	int index = 0;
	BOOL minmax = FALSE;

	CWnd* pWNd = GetActiveChild( &index, &minmax );
	if( index == 2 ) {
		CTaxationList31View *pTaxView = ( CTaxationList31View* )GetTaxListView( ID_VER_SYZ31 );
		if( pTaxView ) {
			pTaxView->GetKoketsuKingaku();
		}
	}
}

void CMainFrame::OnUpdateKoketsuKingaku( CCmdUI *pCmdUI )
{
	BOOL bEnable = FALSE;

	if( m_isKoketsuKingaku && ( m_nowTabIdx == 2 )) {
		// 非連動処理を行うにチェックが入っている
		// 入力ロックを行っていない
		if(( m_SnHeadData.Sn_Sign4 & 0x80 ) && !( m_SnHeadData.Sn_Sign4 & 0x01 )) {
			bEnable = TRUE;
		}
	}
	else {
		bEnable = FALSE;
	}

	pCmdUI->Enable( bEnable );
}

/* // ICSP連携専用の機能とするため、一時的にカット
   // ICSP連携リリース時に復活
//====================================
// 付表5・控除対象仕入税額の選択
//====================================
void CMainFrame::OnUpdateKoujoSelect(CCmdUI *pCmdUI)
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	BOOL	bSw = FALSE;
	int		check = 0;

	if ( GetActViewVer() == ID_VER_SYZ31 ) {
		if ( ( m_Util.GetKazeihoushiki ( m_pZmSub->zvol ) == ID_ICSSH_KANNI ) && ( m_nowTabIdx == 1 ) ) {
			CH31HyoView *pH31HyoView = NULL;
			pH31HyoView = ( CH31HyoView* )( ( ( CDBSyzShinMainApp* )AfxGetApp() )->GetDocumentTemplate()->GetViewWnd ( H31_HONPYO_WND_IDX ) );
			if ( pH31HyoView != NULL ) {
				if ( pH31HyoView->IsFhyo5() ) {
					bSw = TRUE;
				}
			}

			if ( m_SnHeadData.Sn_GenericSgn & 0x08 ) {
				check = 1;
			}
			else {
				check = 0;
			}
		}
	}

	pCmdUI->Enable ( bSw );
	pCmdUI->SetCheck ( check );
}

void CMainFrame::OnKoujoSelect()
{
	// TODO: ここにコマンド更新 UI ハンドラ コードを追加します。
	if ( GetActViewVer() == ID_VER_SYZ31 ) {
		if ( ( m_Util.GetKazeihoushiki ( m_pZmSub->zvol ) == ID_ICSSH_KANNI ) && ( m_nowTabIdx == 1 ) ) {
			CH31HyoView *pH31HyoView = NULL;
			pH31HyoView = ( CH31HyoView* )( ( ( CDBSyzShinMainApp* )AfxGetApp() )->GetDocumentTemplate()->GetViewWnd ( H31_HONPYO_WND_IDX ) );
			if ( pH31HyoView != NULL ) {
				pH31HyoView->SelectFhyo5();
			}
		}
	}
}
*/
int	CMainFrame::Message_BlankKinyu( int &erst )
{
	if( (m_nowTabIdx!=0) && (m_SnSeq==0) && ((m_SnHeadData.Sn_Sign4&0x01)==0x00) ){
		if( IsKanpuShinkoku2() == TRUE ){

//--> '16.12.26 INS START
			if( m_nowTabIdx == 1 ){
				// データ確定用のフォーカス移動
				CBaseHyoView *pBaseView = GetHonHyoView();
				if( pBaseView != NULL ){
					pBaseView->MoveFocusInEnd();
				}
			}
//<-- '16.12.26 INS END

			int ret = 0;

			if( m_pMainView ){
				ret = m_pMainView->Message_BlankKinyu_Honpyo();
				if( ret == IDYES ){
					erst = ID_SYZ_MOVE_TO_HONPYO_BANK;
					return ret;
				}
			}

			/*CBaseHyoView *pBaseView = GetHonHyoView();
			if( pBaseView != NULL ){
//				ret = pH26View->Message_BlankKinyu_Kanpu();
//				if( ret == IDYES ){
//					erst = ID_SYZ_MOVE_TO_KANNPU_BANK;
//					return ret;
//				}
				ret =  pBaseView->Message_BlankKinyu_Kousei();
				if( ret == IDYES ){
					erst = ID_SYZ_MOVE_TO_KOUSEI_BANK;
					return ret;
				}
			}*/
		}

		// 更正の請求は常に還付となるため還付かどうかのチェックは行わない
		CBaseHyoView *pBaseView = GetHonHyoView();
		if( pBaseView != NULL ){
//				ret = pH26View->Message_BlankKinyu_Kanpu();
//				if( ret == IDYES ){
//					erst = ID_SYZ_MOVE_TO_KANNPU_BANK;
//					return ret;
//				}
			int ret = pBaseView->Message_BlankKinyu_Kousei();
			if( ret == IDYES ){
				erst = ID_SYZ_MOVE_TO_KOUSEI_BANK;
				return ret;
			}
		}
	}

	return 0;
}
//2016.06.22 INSERT END
//2017.12.04 INSERT START
int CMainFrame::Message_BlankKijunKazei()
{
	int ret = 0;

	// 本表のデータ取得
	char	zzuri[MONY_BUF_SIZE]={0};
	CString	tag, chtag;

	// 10%改正対応
	if ( GetActViewVer() == ID_VER_SYZ26 ) {
		tag = _T("AAF00000");
	}
	else {
		tag = _T("AAM00030");
	}
	GetMony( tag, chtag, zzuri );

	if( (m_nowTabIdx!=0) && (m_SnSeq==0) && ((m_SnHeadData.Sn_Sign4&0x01)==0x00)&&( m_Arith.l_test(zzuri)==0 ) ){
		CString	msg;
		msg = _T("基準期間の課税売上高が0円（未入力）です。");
// 改良依頼 18-0439 修正 del -->
		//msg += _T("\n「税額情報」で確認してください。");
// 改良依頼 18-0439 修正 del <--
// 改良依頼 18-0439 修正 add -->
		msg += _T("\n「基本情報」の「税額情報」で確認してください");
// 改良依頼 18-0439 修正 add <--
		ret = ICSMessageBox(msg, MB_YESNO, 0);
	}

	return ret;
}

int CMainFrame::Message_BlankSoneki()
{
	int ret = 0;

	if( (m_nowTabIdx!=0) && (m_SnSeq==0) && ((m_SnHeadData.Sn_Sign4&0x01)==0x00) ){
		if(( m_Util.IsUnConnectMst(m_pZmSub->zvol) == FALSE )&&!( m_SnHeadData.Sn_Sign4&0x80 )){
			CBaseTaxHyoView *pBaseTaxView = GetTaxListView();
			if( pBaseTaxView != NULL ){
				if(( pBaseTaxView->IsEmpty_SonekiSyunyu() == FALSE )||( pBaseTaxView->IsEmpty_SonekiShiireKeihi() == FALSE )){
					CString	msg;
					msg = _T("課税取引金額計算表の「損益計算書の収入(仕入)金額」が入力されています。");
					msg += _T("\n非連動処理に切り替えてF8申告書転記を行ってください。");
					ret = ICSMessageBox(msg, MB_YESNO, 0);
				}
			}
		}
	}

	return ret;
}
//2017.12.04 INSERT END

//2018.03.13 INSERT START
int CMainFrame::Message_MyNumber_error()
{
	int ret = 0;
	if( (m_SnSeq==0) && ((m_SnHeadData.Sn_Sign4&0x01)==0x00) ){
		if(( m_SnHeadData.IsSoleProprietor())&&( m_SnHeadData.Sn_Syuusei&0x10 )){
			CBaseHyoView *pBaseView = GetHonHyoView();
			if( pBaseView ){
				ret = pBaseView->Message_MyNumber_error();
			}
			//if( ret == -1 ){
			//	ICSMessageBox( "付表６　相続人等に関する事項\n個人番号が重複しています。", MB_ICONSTOP );
			//}			
		}
	}
	return ret;
}

int CMainFrame::Message_MyNumber_overlap()
{
	int ret = 0;
	if( (m_SnSeq==0) && ((m_SnHeadData.Sn_Sign4&0x01)==0x00) ){
		if(( m_SnHeadData.IsSoleProprietor())&&( m_SnHeadData.Sn_Syuusei&0x10 )){
			CBaseHyoView *pBaseView = GetHonHyoView();
			if( pBaseView ){
				ret = pBaseView->Message_MyNumber_overlap();
			}
			//if( ret == -1 ){
			//	ICSMessageBox( "付表６　相続人等に関する事項\n個人番号が重複しています。", MB_ICONSTOP );
			//}			
		}
	}
	return ret;
}

int CMainFrame::Message_SozokuWariai()
{
	int ret = 0;
	if( (m_SnSeq==0) && ((m_SnHeadData.Sn_Sign4&0x01)==0x00) ){
		if(( m_SnHeadData.IsSoleProprietor())&&( m_SnHeadData.Sn_Syuusei&0x10 )){
			CBaseHyoView *pBaseView = GetHonHyoView();
			if( pBaseView ){
				ret = pBaseView->Message_SozokuWariai();
			}
			//if( ret == -1 ){
			//	ICSMessageBox( "付表６　相続人等に関する事項\n承継割合の合計が『 1 』ではありません。", MB_ICONSTOP );
			//}
		}
	}
	return ret;
}

int CMainFrame::Message_NofuKanpuGokei()
{
	CString msg;
	int ret = 0;
//	if( (m_SnSeq==0) && ((m_SnHeadData.Sn_Sign4&0x01)==0x00) ){
	if( m_SnSeq==0 ){
		if(( m_SnHeadData.IsSoleProprietor())&&( m_SnHeadData.Sn_Syuusei&0x10 )){
			CBaseHyoView *pBaseView = GetHonHyoView();

			//ret = pH26View->NofuKanpuGokeiCheck();
			if( pBaseView ){
				return pBaseView->Message_NofuKanpuGokei();
			}
			//if( ret ){
			//	if( ret&0x01 ){
			//		msg += "⑨の各人の納付税額(消費税)の合計金額と②の納める消費税の合計額\n";
			//	}
			//	if( ret&0x02 ){
			//		msg += "⑩の各人の納付税額(地方消費税)の合計金額と③の納める地方消費税の合計額\n";
			//	}
			//	if( ret&0x04 ){
			//		msg += "⑫の各人の還付税額(消費税)の合計金額と⑤の還付される消費税の合計額\n";
			//	}
			//	if( ret&0x08 ){
			//		msg += "⑬の各人の還付税額(地方消費税)の合計金額と⑥の還付される地方消費税の合計額\n";
			//	}
			//	msg += "が一致していません。\n金額を確認してください";

			//	if( msg.IsEmpty() == FALSE ){
			//		return ICSMessageBox(msg, MB_YESNO, 0);
			//	}
			//}
		}
	}
	return ret;
}

void CMainFrame::EditOff_Fuhyo6()
{
	if( (m_SnSeq==0) && ((m_SnHeadData.Sn_Sign4&0x01)==0x00) ){
		if(( m_SnHeadData.IsSoleProprietor())&&( m_SnHeadData.Sn_Syuusei&0x10 )){
			CBaseHyoView *pBaseView = GetHonHyoView();
			if( pBaseView ){
				pBaseView->EditOff_Fuhyo6();
			}
		}
	}
}
//2018.03.13 INSERT END

//========================================================
// 10%様式で、実額入力項目が存在する場合メッセージを表示
//--------------------------------------------------------
// 返送値
//		0		:		処理継続
//		-1		:		入力に戻る
//========================================================
int CMainFrame::Message_H31ManualInputSign()
{
	int ret = 0;
	if ( ( m_nowTabIdx != 0 ) && ( GetActViewVer() == ID_VER_SYZ31 ) && ( ( m_SnHeadData.Sn_Sign4 & 0x01 ) == 0x00 ) ) {
		BOOL InputSign    = FALSE,
			 taxInputSign = FALSE,
			 taxInputSign2= FALSE;

		CH31HyoView	*pH31View = NULL;
		CTaxationList31View *pTax31View = NULL;

		pH31View = ( CH31HyoView* )GetHonHyoView ( ID_VER_SYZ31 );
		if ( pH31View != NULL ) {
			InputSign = pH31View->IsManualInputSign();
		}

		if ( m_Util.IsSpcMaster ( m_pZmSub ) ) {
			pTax31View = ( CTaxationList31View* )GetTaxListView ( ID_VER_SYZ31 );
			if ( pTax31View != NULL ) {
				taxInputSign = pTax31View->IsManualInputSign();

				taxInputSign2 = pTax31View->IsManualInputSign52();	// 24/02/29wd_特定収入対応 add
			}
		}

		//if ( pH31View->IsManualInputSign() == TRUE ) {
		//if ( ( InputSign == TRUE ) || ( taxInputSign == TRUE ) ) {						// 24/02/29wd_特定収入対応 del
		if((InputSign == TRUE) || (taxInputSign == TRUE) || (taxInputSign2 == TRUE))	{	// 24/02/29wd_特定収入対応 add
			CString msg;
			msg.Format ( _T( "実額入力されている項目（背景色が水色）が存在します。\r\n処理を継続しますか？" ) );
			if ( ICSMessageBox ( msg, ( MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 ) ) == IDNO ) {
				// 現在の入力項目へフォーカスセット
				if ( m_nowTabIdx == 1 ) {
					pH31View->SetNowPosition();
				}
				ret = -1;
			}
		}
	}

	return ret;
}

//----> 2022/12/27
//-----------------------------------------------------------------------------
// 公金受取口座にチェックがついているが個人番号がない場合のメッセージ
// (チェックを付けた後に個人番号を削除した場合など)
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::Message_KoukinUketori()
{
	int ret = 0;

//	if( (m_SnSeq==0) && ((m_SnHeadData.Sn_Sign4&0x01)==0x00) ){	// 履歴や入力ロックに限らず常にチェックする（所得税も同様の動き）
		if( (m_SnHeadData.IsSoleProprietor()) && !(m_SnHeadData.Sn_Syuusei&0x10) ){

			long eymd = 0;
			if( !(m_SnHeadData.Sn_SKKBN % 2) ){
				eymd = m_SnHeadData.Sn_MDAYE;
			}else{
				eymd = m_SnHeadData.Sn_KDAYE;
			}
			if( eymd >= R04REFORM ){
				CString str_myno;
				if( (m_SnHeadData.GetMyNumber(str_myno)==1) && ((m_SnHeadData.Sn_GenericSgn&0x400) || (m_SnHeadData.Sn_GenericSgn&0x1000)) ){
					ICSMessageBox( "公金受取口座を利用する際に必要な以下の項目が未登録のため、処理を継続できません。\n\n「個人番号」" );
					ret = -1;
				}
			}
		}
//	}

	return ret;
}
//<---- 2022/12/27

//-----------------------------------------------------------------------------
// 還付付表2　インボイス登録番号のチェック
//-----------------------------------------------------------------------------
// 引数		sgn		：	0　科目選択をマウスクリックした場合
//					：　1　diagに入力した場合
//					：	2　F4プレビューやタブ移動など
//					：	3　処理終了
//-----------------------------------------------------------------------------
// 返送値	true	：	問題なし
//			false	：	不正な登録番号あり
//-----------------------------------------------------------------------------
bool CMainFrame::CheckInvno( int sgn )
{
	bool ret = 0;
	if( (m_SnSeq==0) && ((m_SnHeadData.Sn_Sign4&0x01)==0x00) ){
		CBaseHyoView *pBaseView = GetHonHyoView();
		if( pBaseView ){
			ret = pBaseView->CheckInvno( sgn );
		}
	}
	return ret;
}

//-----------------------------------------------------------------------------
// 還付金融機関入力にフォーカス誘導('16.12.26)
//-----------------------------------------------------------------------------
// 引数	dsInput	：	移動先
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::GuideKanpuKinyu( int dsInput )
{
	if( m_nowTabIdx == 1 ){
		CBaseHyoView *pBaseView = GetHonHyoView();
		if( pBaseView != NULL ){
			return pBaseView->GuideKanpuKinyu( dsInput );
		}
	}
	else{
		SetActiveChild( 1 );
		m_nowTabIdx = 1;
		CBaseHyoView *pBaseView = GetHonHyoView();
		if( pBaseView != NULL ){
			return pBaseView->GuideKanpuKinyu( dsInput );
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 予定申告の月数クリア('17.02.09)
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::ClearYoteiMonth()
{
	CBaseHyoView *pBaseView = GetHonHyoView();
	if( pBaseView != NULL ){
		return pBaseView->ClearYoteiMonth();
	}
	return 0;
}

//-----------------------------------------------------------------------------
// 終了処理フラグの切り替え('18.06.22)
//-----------------------------------------------------------------------------
// 引数	flg	：	セットするフラグ
//-----------------------------------------------------------------------------
void CMainFrame::SetEndStartFlg( BOOL flg )
{
	m_isEndStart = flg;

	CBaseHyoView *pBaseView = GetHonHyoView();
	if( pBaseView != NULL ){
		pBaseView->SetEndStartFlg( flg );
	}
}



//-----------------------------------------------------------------------------
// 企業環境？
//-----------------------------------------------------------------------------
BOOL CMainFrame::IsCompanyPackageInSyz()
{
	static	int	nCheckd = -1;
	BOOL	bRt = FALSE;

	if( nCheckd == 0 ){
		bRt = FALSE;
	}
	else if( nCheckd == 1 ){
		bRt = TRUE;
	}
	else{
		if( IsCompanyPackage() ){
			nCheckd = 1;
			bRt = TRUE;
		}
		else{
			nCheckd = 0;
		}
	}

	return bRt;
}

//-----------------------------------------------------------------------------
// 平成継続出力
//-----------------------------------------------------------------------------
// 引数	pSnHeadData	：	消費税ヘッダ情報
//-----------------------------------------------------------------------------
// 返送値	true	：	平成継続出力
//			false	：	令和切替出力
//-----------------------------------------------------------------------------
bool CMainFrame::IsJcContinuePrint(CSnHeadData *pSnHeadData)
{
	if( m_JcContinuePrint == -1 ){
		m_JcContinuePrint = 0;
		if( m_Util.IsJcContinuePrint(pSnHeadData) ){
			m_JcContinuePrint = 1;
		}
	}

	if( m_JcContinuePrint == 1 ){
		return true;
	}
	else{
		return false;
	}
}


void CMainFrame::KikanChange( unsigned char newver )
{
	// データが確定したタイミングでコール

#ifdef _DBG_VIEW_START_
	CString	hoge;
	hoge.Format("○○○KikanChange_START\n");
	DbgViewTrace(hoge);

	hoge.Format("○○○KikanChange_Now:%d, New:%d\n", m_SnHeadData.Kaisei_VER, newver );
	DbgViewTrace(hoge);
#endif

	EndOtherView(0);

//	m_SnHeadData.Kaisei_VER = newver;
	((CDBSyzShinMainApp*)AfxGetApp())->SetKaiseiVer( SetActViewVer(newver) );

	HINSTANCE hInst = AfxGetResourceHandle();

	// 他のビューの初期化
	if (GetActViewVer() == ID_VER_SYZ31) {
		m_SnHeadData.SetData ( m_pZmSub, m_SnSeq );
		AfxSetResourceHandle(GetH31HyoInstance());
		if (InitOtherView(3, m_pZmSub)) {
			return;
		}

		// 課税取引
		if ( m_Util.IsBasicMst ( m_pZmSub->zvol ) ) {
			AfxSetResourceHandle ( GetTaxationList31Instance() );
			if (InitOtherView ( 4, m_pZmSub ) ) {
				return;
			}
		}
		// 特定収入計算表
		if ( m_Util.IsSpcMaster ( m_pZmSub ) ) {
			AfxSetResourceHandle ( GetTaxationList31Instance() );
			if (InitOtherView ( 5, m_pZmSub ) ) {
				return;
			}
		}

//		m_nowTabver = ID_VER_SYZ31;
		SetKaiseiVer(ID_VER_SYZ31);
	}
	else {
		AfxSetResourceHandle(GetH26HyoInstance());
		if (InitOtherView(0, m_pZmSub)) {
			return;
		}

		// 課税取引
		if ( m_Util.IsBasicMst ( m_pZmSub->zvol ) ) {
			AfxSetResourceHandle ( GetTaxationListInstance() );
			if (InitOtherView ( 1, m_pZmSub ) ) {
				return;
			}
		}
		// 特定収入計算表
		if ( m_Util.IsSpcMaster ( m_pZmSub ) ) {
			AfxSetResourceHandle ( GetTaxationListInstance() );
			if (InitOtherView ( 2, m_pZmSub ) ) {
				return;
			}
		}

//		m_nowTabver = ID_VER_SYZ26;
		SetKaiseiVer(ID_VER_SYZ26);
	}

	// 元に戻す
	AfxSetResourceHandle( hInst );

	SetTabTitle(1);

#ifdef _DBG_VIEW_START_
	hoge.Format("○○○KikanChange_END\n");
	DbgViewTrace(hoge);
#endif
}

//-----------------------------------------------------------------------------
// データの引き継ぎ
//-----------------------------------------------------------------------------
// 引数	now_Kaisei_VER	：	現在の改正バージョン
//		dst_Kaisei_VER	：	変更後の改正バージョン
//-----------------------------------------------------------------------------
// 返送値	0			：	正常終了
//			-1			：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::TakeOverData( int now_Kaisei_VER, int dst_Kaisei_VER )
{
#ifdef _DBG_VIEW_START_
	CString	hoge;
	hoge.Format("TakeOverData:%d -> %d\n", now_Kaisei_VER, dst_Kaisei_VER );
	DbgViewTrace(hoge);
#endif

	int rt = 0;
	if( (now_Kaisei_VER==ID_VER_SYZ31) && (dst_Kaisei_VER==ID_VER_SYZ26) ){
		// １０％ → ８％
		// ８％ 側で１０％のデータを書き込み
		CH26HyoView *pH26View = (CH26HyoView*)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(H26_HONPYO_WND_IDX));
		if( pH26View ){
			ASSERT(m_pMainView);
			rt = pH26View->TakeOverData(now_Kaisei_VER, m_pMainView->m_ShRevType, m_pMainView->m_SixKindFlg, m_SnSeq, m_pZmSub);
		}
	}
	else if( (now_Kaisei_VER==ID_VER_SYZ26) && (dst_Kaisei_VER==ID_VER_SYZ31) ){
		// ８％ → １０％
		// １０％ 側で８％のデータを書き込み
		CH31HyoView *pH31View = (CH31HyoView*)(((CDBSyzShinMainApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(H31_HONPYO_WND_IDX));
		if( pH31View ){
			rt = pH31View->TakeOverData(now_Kaisei_VER, m_SnSeq, m_pZmSub);
		}
	}
	return rt;
}

//-----------------------------------------------------------------------------
// 課税取引ビューの取得
//-----------------------------------------------------------------------------
// 引数		vtype		：	呼び出しビュータイプ（-1：Kaisei_VERによる自動判定、それ以外：呼び出すビューのタイプ）
//-----------------------------------------------------------------------------
// 返送値	NULL以外	：	現時点での課税取引ビューのポインタ
//			NULL		：	エラー
//-----------------------------------------------------------------------------
CBaseTaxHyoView* CMainFrame::GetTaxListView(int vtype/*=-1*/)
{
	CBaseTaxHyoView		*pBaseTaxView = NULL;
	CDBSyzShinMainApp	*pMainApp = (CDBSyzShinMainApp*)AfxGetApp();
	if( pMainApp ){
		if( vtype == -1 ){
			pBaseTaxView = (CBaseTaxHyoView *)pMainApp->GetTaxListView();
//-- 初期化判定は行わない --
// 必要になった時点で追加する
//			if( pBaseTaxView ){
//				if( pBaseTaxView->IsInitedView() == false ){
//					int Kaisei_VER = pMainApp->GetKaiseiVer();
//					if( Kaisei_VER == ID_VER_SYZ31 ){
//						if( InitOtherView(3, m_pZmSub) ){
//							return NULL;
//						}
//					}
//					else if( Kaisei_VER == ID_VER_SYZ26 ){
//						if( InitOtherView(0, m_pZmSub) ){
//							return NULL;
//						}
//					}
//				}
//			}
//----------------
		}
		else{
			pBaseTaxView = (CBaseTaxHyoView *)pMainApp->GetTaxListView(vtype);
//-- 初期化判定は行わない --
// 必要になった時点で追加する
//			if( pBaseView ){
//				if( pBaseView->IsInitedView() == false ){
//					if( vtype == ID_VER_SYZ31 ){
//						if( InitOtherView(3, m_pZmSub) ){
//							return NULL;
//						}
//					}
//					else if( vtype == ID_VER_SYZ26 ){
//						if( InitOtherView(0, m_pZmSub) ){
//							return NULL;
//						}
//					}
//				}
//			}
//----------------
		}
	}

	return pBaseTaxView;
}

//-----------------------------------------------------------------------------
// 本表ビューの取得
//-----------------------------------------------------------------------------
// 引数		vtype		：	呼び出しビュータイプ（-1：Kaisei_VERによる自動判定、それ以外：呼び出すビューのタイプ）
//-----------------------------------------------------------------------------
// 返送値	NULL以外	：	現時点での本表・付表ビューのポインタ
//			NULL		：	エラー
//-----------------------------------------------------------------------------
CBaseHyoView* CMainFrame::GetHonHyoView( int vtype/*=-1*/ )
{
	CBaseHyoView		*pBaseView = NULL;
	CDBSyzShinMainApp	*pMainApp = (CDBSyzShinMainApp*)AfxGetApp();
	if( pMainApp ){
		if( vtype == -1 ){
			pBaseView = (CBaseHyoView *)pMainApp->GetHonHyoView();
			if( pBaseView ){
				if( pBaseView->IsInitedView() == false ){
					int Kaisei_VER = pMainApp->GetKaiseiVer();
					if( Kaisei_VER == ID_VER_SYZ31 ){
						if( InitOtherView(3, m_pZmSub) ){
							return NULL;
						}
					}
					else if( Kaisei_VER == ID_VER_SYZ26 ){
						if( InitOtherView(0, m_pZmSub) ){
							return NULL;
						}
					}
				}
			}
		}
		else{
			pBaseView = (CBaseHyoView *)pMainApp->GetHonHyoView( vtype );
			if( pBaseView ){
				if( pBaseView->IsInitedView() == false ){
					if( vtype == ID_VER_SYZ31 ){
						if( InitOtherView(3, m_pZmSub) ){
							return NULL;
						}
					}
					else if( vtype == ID_VER_SYZ26 ){
						if( InitOtherView(0, m_pZmSub) ){
							return NULL;
						}
					}
				}
			}
		}
	}

	return pBaseView;
}


// '19.08.22：取り敢えずは簡単なゲット・セットですが、ここで種々の判定は追加する予定！

//-----------------------------------------------------------------------------
// 起動中のビューのバージョンを設定（ヘッドデータを使用）
//-----------------------------------------------------------------------------
// 引数	pSnHeadData	：	取得元のヘッドデータ
//-----------------------------------------------------------------------------
// 返送値			：	セット後のバージョン
//-----------------------------------------------------------------------------
int CMainFrame::SetActViewVer( CSnHeadData *pSnHeadData )
{
#ifdef _DBG_VIEW_START_
	CString	hoge;
	hoge.Format("※※※※※  CMainFrame::SetActViewVer_Head_ST：m_ActViewVer：%d\n", m_ActViewVer);
	DbgViewTrace(hoge);
#endif

	ASSERT(pSnHeadData);
	
	long matsubi = 0;
	if( pSnHeadData->IsTyukanShinkoku() ){
		matsubi = pSnHeadData->Sn_MDAYE;
	}
	else{
		matsubi = pSnHeadData->Sn_KDAYE;
	}

	if( matsubi >= ICS_SH_10PER_REFORM_DAY ){
		m_ActViewVer = ID_VER_SYZ31;
		m_ActedViewFlg |= 0x02;

#ifdef _20190930_SYUKA
		if ( m_Util.IsSpcMaster ( m_pZmSub ) ) {
			if ( m_ShinInfo.pSgSpc ) {
				ShowTabOrgIndex ( 3, FALSE, FALSE );
				*m_ShinInfo.pSgSpc = 0;
			}
		}
#endif
		// 2割特例を適用する場合は簡易と同じ扱いになるため、特定収入タブを非表示にする
		if( m_SnHeadData.m_DispTabSgn & 0x2000 ){
			if( m_Util.IsSpcMaster( m_pZmSub ) ){
				if( m_ShinInfo.pSgSpc ){
					ShowTabOrgIndex( 3, FALSE, FALSE );
				}
			}
		}
	}
	else{
		m_ActViewVer = ID_VER_SYZ26;
		m_ActedViewFlg |= 0x01;

#ifdef _20190930_SYUKA
		if ( m_Util.IsSpcMaster ( m_pZmSub ) ) {
			if ( m_ShinInfo.pSgSpc ) {
				ShowTabOrgIndex ( 3, TRUE, FALSE );
				*m_ShinInfo.pSgSpc = 1;
			}
		}
#endif
	}

#ifdef _DBG_VIEW_START_
	hoge.Format("※※※※※  CMainFrame::SetActViewVer_Head_END：m_ActViewVer：%d\n", m_ActViewVer);
	DbgViewTrace(hoge);
#endif

	return m_ActViewVer;
}

//-----------------------------------------------------------------------------
// 起動中のビューのバージョンを設定（期間の末日）
//-----------------------------------------------------------------------------
// 引数	eymd	：	期間の末日
//-----------------------------------------------------------------------------
// 返送値		：	セット後のバージョン
//-----------------------------------------------------------------------------
int CMainFrame::SetActViewVerByDay( long eymd )
{
#ifdef _DBG_VIEW_START_
	CString	hoge;
	hoge.Format("※※※※※  CMainFrame::SetActViewVerByDay_ST：m_ActViewVer：%d\n", m_ActViewVer);
	DbgViewTrace(hoge);
#endif

	if( eymd >= ICS_SH_10PER_REFORM_DAY ){
		m_ActViewVer = ID_VER_SYZ31;
		m_ActedViewFlg |= 0x02;

#ifdef _20190930_SYUKA
		if ( m_Util.IsSpcMaster ( m_pZmSub ) ) {
			ShowTabOrgIndex ( 3, FALSE, FALSE );
			*m_ShinInfo.pSgSpc = 0;
		}
#endif
		// 2割特例を適用する場合は簡易と同じ扱いになるため、特定収入タブを非表示にする
		if( m_SnHeadData.m_DispTabSgn & 0x2000 ){
			if ( m_Util.IsSpcMaster ( m_pZmSub ) ) {
				ShowTabOrgIndex ( 3, FALSE, FALSE );
			}
		}
	}
	else{
		m_ActViewVer = ID_VER_SYZ26;
		m_ActedViewFlg |= 0x01;

#ifdef _20190930_SYUKA
		if ( m_Util.IsSpcMaster ( m_pZmSub ) ) {
			ShowTabOrgIndex ( 3, TRUE, FALSE );
			*m_ShinInfo.pSgSpc = 1;
		}
#endif
	}

#ifdef _DBG_VIEW_START_
	hoge.Format("※※※※※  CMainFrame::SetActViewVerByDay_ED：m_ActViewVer：%d, eymd:%d\n", m_ActViewVer, eymd);
	DbgViewTrace(hoge);
#endif

	return m_ActViewVer;
}

//-----------------------------------------------------------------------------
// 起動中のビューのバージョンを設定（ビュー情報を使用）
//-----------------------------------------------------------------------------
// 引数	pSnset	：	取得元のビュー情報
//-----------------------------------------------------------------------------
// 返送値		：	セット後のバージョン
//-----------------------------------------------------------------------------
int CMainFrame::SetActViewVer( CSnset *pSnset )
{
#ifdef _DBG_VIEW_START_
	CString	hoge;
	hoge.Format("※※※※※  CMainFrame::SetActViewVer_Snset_ST：m_ActViewVer：%d\n", m_ActViewVer);
	DbgViewTrace(hoge);
#endif

	ASSERT(pSnset);

	CString	ymdStr;
	long	matsubi = 0;
	if( pSnset->IsTyukanShinkoku() ){
		TimePair tyukankikan = pSnset->GetSetTyukanKikan();
		ymdStr = tyukankikan.second.Format("%Y%m%d");
		matsubi = atoi( ymdStr );
	}
	else{
		TimePair kessankikan = pSnset->GetSetKessanKikan();
		ymdStr = kessankikan.second.Format("%Y%m%d");
		matsubi = atoi(ymdStr);
	}

	if( matsubi >= ICS_SH_10PER_REFORM_DAY ){
		m_ActViewVer = ID_VER_SYZ31;
		m_ActedViewFlg |= 0x02;
	}
	else{
		m_ActViewVer = ID_VER_SYZ26;
		m_ActedViewFlg |= 0x01;
	}

#ifdef _DBG_VIEW_START_
	hoge.Format("※※※※※  CMainFrame::SetActViewVer_Snset_ED：m_ActViewVer：%d, matsubi:%d\n", m_ActViewVer, matsubi);
	DbgViewTrace(hoge);
#endif

	return m_ActViewVer;
}

//-----------------------------------------------------------------------------
// 起動中のビューのバージョンを設定
//-----------------------------------------------------------------------------
// 引数	ver	：	セットするバージョン
//-----------------------------------------------------------------------------
int CMainFrame::SetActViewVer(int ver)
{
#ifdef _DBG_VIEW_START_
	CString	hoge;
	hoge.Format("※※※※※  CMainFrame::SetActViewVer：m_ActViewVer：%d, ver：%d\n", m_ActViewVer,ver);
	DbgViewTrace(hoge);
#endif

	return m_ActViewVer = ver;
}

//-----------------------------------------------------------------------------
// 起動中のビューのバージョンの取得
//-----------------------------------------------------------------------------
int CMainFrame::GetActViewVer()
{
	return m_ActViewVer;
}

// 前提条件
//   8%対応分（ZIM15_Syz26.dll）と、10%対応分（ZIM15_Syz31.dll）に、それぞれの改正対応分のビューを作成して、そのビューをMDIタブの中に作成する。
//   起動時には、両方のMDIタブが作成されていて、その表示・非表示を切り替える事で、改正ごとの申告書を表示する事に対応しています。

// メインフレーム(CMainFrame)で管理しているビューのバージョンとは？
//   有効タブまでは変更していない可能性があるが、課税期間の切り替えなどで、データの取得先として対象にすべきビューの年度

// メインアプリ(CDBSyzShinMainApp)で管理している改正バージョンとは？
//   全年度のタブが作成されているのですが、そのタブの内、実際に有効になっている年度を管理している

//-----------------------------------------------------------------------------
// メインアプリで管理している改正バージョンの取得
//-----------------------------------------------------------------------------
// 返送値	：	改正バージョン
//-----------------------------------------------------------------------------
int CMainFrame::GetKaiseiVer()
{
	CDBSyzShinMainApp	*pMainApp = (CDBSyzShinMainApp*)AfxGetApp();
	if( pMainApp ){
		return pMainApp->GetKaiseiVer();
	}
	
	return 0;
}

//-----------------------------------------------------------------------------
// メインアプリで管理している改正バージョンの設定
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::SetKaiseiVer(int ver)
{
	int rt = -1;
	CDBSyzShinMainApp	*pMainApp = (CDBSyzShinMainApp*)AfxGetApp();
	if( pMainApp ){
		pMainApp->SetKaiseiVer( ver );
		rt = 0;
	}

	return rt;
}


//---->yoshida200925
//-----------------------------------------------------------------------------
// zvol subswチェック＆修復
//-----------------------------------------------------------------------------
// 引数	pZmsub	：	財務クラス
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CMainFrame::Chkzvolsubsw(CDBNpSub *pZmsub)
{
	ASSERT(pZmsub);
	ASSERT(pZmsub->m_database);
	if ((pZmsub == NULL) || (pZmsub->m_database == NULL)) {
		return -1;
	}

	CString	sql;
	sql.Format(_T("select sub_sw from zvolume"));

	BOOL	findFlg = FALSE;
	CString	str_subsw;

	CRecordset dbrec(pZmsub->m_database);
	try {
		dbrec.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly);

		if (dbrec.IsEOF() == FALSE) {
			dbrec.GetFieldValue(_T("sub_sw"), str_subsw);
			findFlg = TRUE;
		}
	}
	catch (CDBException* dbe) {
		dbrec.Close();
		dbe->Delete();
		return -1;
	}
	dbrec.Close();


	int subsw = atoi(str_subsw);

	if (findFlg) {
		if (subsw & 0x20000) {//消費税非連動サイン立ってる場合

			char	sqlsvr[128] = { 0 }, apno[32] = { 0 }, cocode[32] = { 0 }, kikan[32] = { 0 };
			int		n_apno, n_cocode, n_kikan;
			PCOM_GetString("SELECTCODSN", sqlsvr, apno, cocode, kikan);
			n_apno = atoi(apno);			// アプリケーションNo.
			n_cocode = atoi(cocode);		// 会社コード
			n_kikan = atoi(kikan);		// 期間


			ICSMCTL		icsmctl;
			if (icsmctl.SetDatabase(sqlsvr)) {
				return -1;
			}
			if (icsmctl.vctl_open(0x00400001, n_apno, 0)) {
				return -1;
			}
			CO_CTL_DB		coctl;
			CO_CTL_DB		tmpCoctl;
			CO_CTL_Array	coarray;

			memset(&coctl, '\0', sizeof(CO_CTL_DB));
			memset(&tmpCoctl, '\0', sizeof(CO_CTL_DB));
			
			coctl.c_apno = n_apno;
			int_bcd(coctl.c_cod, n_cocode, 4);

			int eeymd = atoi(kikan);
			unsigned char	Eymd[4] = { 0 };
			int_bcd(Eymd, eeymd, 4);

			if (icsmctl.vctl_coinf(&coctl, &coarray)) {
				icsmctl.vctl_close();
				return -1;
			}

			int max = (int)coarray.GetCount();
			for (int i = 0; i < max; i++) {
				tmpCoctl = coarray.GetAt(i);
				if (memcmp(&tmpCoctl.c_kikan[3], &Eymd[1], 3) == 0) {
					coctl = tmpCoctl;
					break;
				}
			}

			//のみマスターフラグが立っていない時
			if (coctl.c_exbin[128] == 0) {
				//ここに入ることは、のみマスターフラグ立ってて、非連動サインが立っていることになる。
				//不正状態であるので、sub_swの消費税非連動サインを落とすようにする。
				sql.Empty();
				sql.Format(_T("update zvolume set sub_sw = sub_sw - 0x20000 where (sub_sw&0x20000) = 0x20000"));

				// 修復処理開始
				pZmsub->m_database->BeginTrans();
				try {
					pZmsub->m_database->ExecuteSQL(sql);
				}
				catch (CDBException* dbe) {
					pZmsub->m_database->Rollback();
					dbe->Delete();
					icsmctl.vctl_close();
					return -1;
				}
				pZmsub->m_database->CommitTrans();

				//CORECのSUB_SWも立ってる場合があるので、これも落とす処理をながしておく
				coctl.c_subsw &= ~0x20000;

				if ( icsmctl.vctl_comnt(coctl.seq, &coctl) == -1 ) {
					icsmctl.vctl_close();
					return -1;
				}
			}

			icsmctl.vctl_close();
		}
	}

	return 0;
}
//<-----

//===========================================================
// 特例テーブルチェック&修復
//-----------------------------------------------------------
// 引数
//		pZmsub		:		公益クラス
//-----------------------------------------------------------
// 返送値
//		0			;		正常終了
//		-1			:		エラー
//===========================================================
int CMainFrame::ChkTkreiTable ( CDBNpSub *pZmsub )
{
	ASSERT ( pZmsub );
	ASSERT ( pZmsub->m_database );
	if ( ( pZmsub == NULL ) || ( pZmsub->m_database == NULL ) ) {
		return -1;
	}

	int symd = 0, st = 0;
	CString sql, table, tblData;
	CDatabase *db = pZmsub->m_database;
	CRecordset rs ( db );

	sql.Format ( _T( "SELECT ss_ymd FROM zvolume" ) );
	try {
		tblData.Empty();
		rs.Open( CRecordset::forwardOnly, sql );
		if ( !rs.IsEOF() ) {
			rs.GetFieldValue ( ( short )0, tblData );
		}
		rs.Close();
	}
	catch ( CDBException *dbe ) {
		rs.Close();
		dbe->Delete();
		return -1;
	}

	symd = atoi ( tblData );
	if ( ( symd < TOKUREI_KIKAN_S ) || ( symd > TOKUREI_KIKAN_E ) ) {
		// 特例計算期間外のため、チェックも修復も必要なし
		return 0;
	}

	sql.Format ( _T( "SELECT COUNT ( * ) FROM sys.tables WHERE name = 'simku_eigyobi'" ) );
	try {
		tblData.Empty();
		rs.Open( CRecordset::forwardOnly, sql );
		if ( !rs.IsEOF() ) {
			rs.GetFieldValue ( ( short )0, tblData );
		}
		rs.Close();
	}
	catch ( CDBException *dbe ) {
		rs.Close();
		dbe->Delete();
		return -1;
	}

	if ( atoi ( tblData ) == 0 ) {
		// 10日間営業日テーブルが存在しないため、チェックも修復も必要なし
		return 0;
	}

	st = ChkEigyobiTable ( symd, db, &rs );
	if ( st == -1 ) {
		ErrorExit ( _T( "特例帳票　10日間営業日の更新に失敗しました" ) );
		return -1;
	}
	else if ( st == 1 ) {
		// 修復された場合には10日間営業日テーブルをクリアされているため、
		// 休日設定テーブルのチェック＆修復は必要ない
		// 10日間営業日テーブルの修復がされなかった場合のみ、休日設定テーブルのチェック＆修復を行う
		if ( ChkHolidayTable ( db, &rs ) == -1 ) {
			ErrorExit ( _T( "特例帳票　休日設定の更新に失敗しました" ) );
			return -1;
		}

		// 上記休日設定テーブルの修復時に休日設定テーブルの内容をを全て反映させてしまっていたためテーブルの再修復を行う
		if ( RepairEigyobi ( db, &rs ) == -1 ) {
			ErrorExit ( _T( "特例帳票　休日設定の更新に失敗しました" ) );
			return -1;
		}
	}

	return 0;
}

//===========================================================
// 10日間営業日テーブルチェック&修復
//-----------------------------------------------------------
// 引数
//		symd		:		決算期間期首
//		pdb			:		CDatabaseのポインタ
//		prs			:		CRecordsetのポインタ
//-----------------------------------------------------------
// 返送値
//		1			:		修復なし
//		0			;		正常終了
//		-1			:		エラー
//===========================================================
int CMainFrame::ChkEigyobiTable ( int symd, CDatabase *pdb, CRecordset *prs )
{
	ASSERT ( pdb );
	ASSERT ( prs );
	if ( ( pdb == NULL ) || ( prs == NULL ) ) {
		return -1;
	}

	int tkSymd = 0, ret = 0;
	CString sql, table, tblData;

	table = _T( "simku_eigyobi" );
	try {
		sql.Empty();
		tblData.Empty();
		sql.Format ( _T( "SELECT TOP 1 ddate FROM %s ORDER BY ddate ASC" ), table );
		prs->Open ( CRecordset::forwardOnly, sql );
		if ( !prs->IsEOF() ) {
			prs->GetFieldValue ( ( short )0, tblData );
		}
		prs->Close();

		tkSymd = atoi ( tblData );
		if ( tkSymd != symd ) {
			pdb->BeginTrans();
			// 不正データ
			for ( int i = 0; i < 4; i++ ) {
				sql.Empty();
				table.Empty();
				switch ( i ) {
				case 0:
					table = _T( "simku_eigyobi" );
					break;
				case 1:
					table = _T( "simku_kyujitsu" );
					break;
				case 2:
					table = _T( "simku_syukeitenki" );
					break;
				case 3:
					table = _T( "simku_syukujitsu" );
					break;
				default:
					// simku_settingholidayはクリアしない
					continue;
				}
				sql.Format ( _T( "IF OBJECTPROPERTY ( object_id ( '%s' ), 'ISTABLE' ) IS NOT NULL DROP TABLE %s" ), table, table );
				pdb->ExecuteSQL ( sql );
			}
			pdb->CommitTrans();
			ret = 0;
		}
		else {
			ret = 1;
		}
	}
	catch ( CDBException *dbe ) {
		pdb->Rollback();
		prs->Close();
		dbe->Delete();
		return -1;
	}


	return ret;
}

//===========================================================
// 休日設定テーブルチェック&修復
//-----------------------------------------------------------
// 引数
//		pdb			:		CDatabaseのポインタ
//		prs			:		CRecordsetのポインタ
//-----------------------------------------------------------
// 返送値
//		0			;		正常終了
//		-1			:		エラー
//===========================================================
int CMainFrame::ChkHolidayTable ( CDatabase *pdb, CRecordset *prs )
{
	ASSERT ( pdb );
	ASSERT ( prs );
	if ( ( pdb == NULL ) || ( prs == NULL ) ) {
		return -1;
	}

	int cnt = 0, day = 0;
	CString sql,subSql, tblData;
	CArray<int> dayArray;
	sql.Format ( _T( "SELECT COUNT ( * ) FROM sys.tables WHERE name = 'simku_settingholiday'" ) );
	try {
		prs->Open ( CRecordset::forwardOnly, sql );
		tblData.Empty();
		if ( !prs->IsEOF() ) {
			prs->GetFieldValue ( ( short )0, tblData );
		}
		prs->Close();
	}
	catch ( CDBException *dbe ) {
		prs->Close();
		dbe->Delete();
		return -1;
	}

	if ( atoi ( tblData ) == 0 ) {
		// 休日設定テーブルなし
		return 0;
	}

	// 祝日設定もしくは集計除外日設定を行っていると、simku_settingholiday::yobi=0, simku_eigyobi::hieigyobi=1となるため修正
	//sql.Format ( _T( "SELECT COUNT ( * ) FROM simku_eigyobi AS eigyo, simku_settingholiday AS holiday WHERE eigyo.yobi = holiday.yobi AND eigyo.hieigyobi != holiday.yflg" ) );
	sql.Format ( _T( "SELECT COUNT ( * ) FROM simku_eigyobi AS eigyo, simku_settingholiday AS holiday WHERE eigyo.yobi = holiday.yobi AND ( eigyo.hieigyobi = 0 AND holiday.yflg = 1 )" ) );
	try {
		prs->Open ( CRecordset::forwardOnly, sql );
		tblData.Empty();
		if ( !prs->IsEOF() ) {
			prs->GetFieldValue ( ( short )0, tblData );
		}
		prs->Close();
	}
	catch ( CDBException *dbe ) {
		prs->Close();
		dbe->Delete();
		return -1;
	}

	if ( atoi ( tblData ) > 0 ) {
		// 不正データ
		pdb->BeginTrans();
		try {
			sql.Format ( _T( "SELECT yobi FROM simku_settingholiday WHERE yflg = 1 ORDER BY yobi ASC" ) );
			prs->Open ( CRecordset::forwardOnly, sql );
			for ( ; !prs->IsEOF(); prs->MoveNext() ) {
				prs->GetFieldValue ( ( short )0, tblData );
				dayArray.Add ( atoi ( tblData ) );
			}
			prs->Close();

			sql.Empty();
			cnt = ( int )dayArray.GetCount();
			for ( int i = 0; i < cnt; i++ ) {
				subSql.Empty();
				day = dayArray.GetAt ( i );
				subSql.Format ( _T( "UPDATE simku_eigyobi SET hieigyobi = 1 WHERE yobi = %d " ), day );
				sql += subSql;
			}
			pdb->ExecuteSQL ( sql );
		}
		catch ( CDBException *dbe ) {
			pdb->Rollback();
			prs->Close();
			dbe->Delete();
			return -1;
		}
		pdb->CommitTrans();
	}

	return 0;
}

//===========================================================
// 10日間営業日テーブル修復
//-----------------------------------------------------------
// 引数
//		pdb			:		CDatabaseのポインタ
//		prs			:		CRecordsetのポインタ
//-----------------------------------------------------------
// 返送値
//		0			;		正常終了
//		-1			:		エラー
//===========================================================
int CMainFrame::RepairEigyobi ( CDatabase *pdb, CRecordset *prs )
{
	ASSERT ( pdb );
	ASSERT ( prs );
	if ( ( pdb == NULL ) || ( prs == NULL ) ) {
		return -1;
	}

	typedef struct _SETDAY {
		int date;
		int flg;
	} SETDAY;

	SETDAY sday = { 0 };
	CString sql, subSql, tblData, table;
	CArray<SETDAY> dayArray;
	int cnt = 0;
	BOOL tFlg = FALSE, wFlg = FALSE;


	for ( int i = 0; i < 2; i++ ) {
		switch ( i ) {
		case 0:
			table = _T( "simku_kyujitsu" );
			break;
		case 1:
			table = _T( "simku_syukujitsu" );
			break;
		default:
			continue;
		}
		sql.Format ( _T( "SELECT COUNT ( * ) FROM sys.tables WHERE name = '%s'" ), table );
		try {
			prs->Open ( CRecordset::forwardOnly, sql );
			tblData.Empty();
			if ( !prs->IsEOF() ) {
				prs->GetFieldValue ( ( short )0, tblData );
			}
			prs->Close();
		}
		catch ( CDBException *dbe ) {
			prs->Close();
			dbe->Delete();
			return -1;
		}

		if ( atoi ( tblData ) == 0 ) {
			tFlg = TRUE;
			break;
		}
	}

	if ( tFlg ) {
		// テーブルがないため修復なし
		return 0;
	}

	for ( int i = 0; i < 2; i++ ) {
		sql.Empty();
		switch ( i ) {
		case 0:
			sql.Format ( _T( "SELECT COUNT ( * ) FROM simku_eigyobi AS eigyo, simku_kyujitsu AS kyujitsu WHERE ( ( eigyo.ddate = kyujitsu.ddate ) AND ( eigyo.hflg = 0 OR eigyo.hieigyobi = 0 ) )" ) );
			break;
		case 1:
			sql.Format ( _T( "SELECT COUNT ( * ) FROM simku_eigyobi AS eigyo, simku_syukujitsu AS syukujitsu WHERE ( ( eigyo.ddate = syukujitsu.ddate ) AND ( eigyo.sflg = 0 OR eigyo.hieigyobi = 0 ) )" ) );
			break;
		default:
			continue;
		}

		try {
			prs->Open ( CRecordset::forwardOnly, sql );
			tblData.Empty();
			if ( !prs->IsEOF() ) {
				prs->GetFieldValue ( ( short )0, tblData );
			}
			prs->Close();
		}
		catch ( CDBException *dbe ) {
			prs->Close();
			dbe->Delete();
			return -1;
		}

		if ( atoi ( tblData ) > 0 ) {
			// 不正データ
			wFlg = TRUE;
			break;
		}
	}

	if ( wFlg ) {
		// 不正データ
		pdb->BeginTrans();
		try {
			sql.Empty();
			sql.Format ( _T( "SELECT ddate FROM simku_kyujitsu" ) );
			prs->Open ( CRecordset::forwardOnly, sql );
			for ( ; !prs->IsEOF(); prs->MoveNext() ) {
				memset ( &sday, '\0', sizeof ( SETDAY ) );
				tblData.Empty();

				prs->GetFieldValue ( ( short )0, tblData );
				sday.date = atoi ( tblData );
				sday.flg = 0;
				dayArray.Add ( sday );
			}
			prs->Close();

			sql.Empty();
			sql.Format ( _T( "SELECT ddate FROM simku_syukujitsu" ) );
			prs->Open ( CRecordset::forwardOnly, sql );
			for ( ; !prs->IsEOF(); prs->MoveNext() ) {
				memset ( &sday, '\0', sizeof ( SETDAY ) );
				tblData.Empty();

				prs->GetFieldValue ( ( short )0, tblData );
				sday.date = atoi ( tblData );
				sday.flg = 1;
				dayArray.Add ( sday );
			}
			prs->Close();


			cnt = ( int )dayArray.GetCount();
			sql.Empty();
			for ( int i = 0; i < cnt; i++ ) {
				subSql.Empty();
				sday = dayArray.GetAt ( i );
				if ( sday.flg == 0 ) {
					subSql.Format (_T( "UPDATE simku_eigyobi SET hflg = 1, hieigyobi = 1 WHERE ddate = %d " ), sday.date );
				}
				else {
					subSql.Format (_T( "UPDATE simku_eigyobi SET sflg = 1, hieigyobi = 1 WHERE ddate = %d " ), sday.date );
				}
				sql += subSql;
			}
			pdb->ExecuteSQL ( sql );
		}
		catch ( CDBException *dbe ) {
			pdb->Rollback();
			prs->Close();
			dbe->Delete();
			return -1;
		}
		pdb->CommitTrans();
	}

	return 0;
}

//======================================================
// 課税期間の取得
//------------------------------------------------------
// 引数
//		symd		:		課税期間（自）
//		eymd		:		課税期間（至）
//======================================================
void CMainFrame::GetKazeiKikan( long &symd, long &eymd )
{
	long tmpSymd = 0, tmpEymd = 0;

	// 初期化
	symd = 0;
	eymd = 0;

	if( !( m_SnHeadData.Sn_SKKBN % 2 )) {
		// 中間申告
		tmpSymd = m_SnHeadData.Sn_MDAYS;
		tmpEymd = m_SnHeadData.Sn_MDAYE;
	}
	else {
		// 確定申告
		tmpSymd = m_SnHeadData.Sn_KDAYS;
		tmpEymd = m_SnHeadData.Sn_KDAYE;
	}

	symd = tmpSymd;
	eymd = tmpEymd;
}

//======================================================
// 課税取引金額計算表　経理方式の選択可能？
//------------------------------------------------------
// 返送値
//		true		:		可能
//		false		:		不可能
//======================================================
bool CMainFrame::IsChangeKeiriHoushiki()
{
	bool bRet = false;
	long symd = 0, eymd = 0;
	GetKazeiKikan( symd, eymd );

	// 令和5年1月1日より前の課税期間であれば、課税取引金額計算表の経理方式の切り替えが可能
	if( symd < R05_SDAY ) {
		bRet = true;
	}

	return bRet;
}

//======================================================
// 集計クラス再初期化
//------------------------------------------------------
// 返送値
//		0			:		正常終了
//		-1			:		エラー
//======================================================
int CMainFrame::ReSyzSyukei()
{
	// 集計クラス解放
	m_pSyzSyukei->finalize();

	// 集計クラス初期化
	if( m_pSyzSyukei->initialize( m_pSzzanList )) {
		return -1;
	}

	// 再集計
	//--> 20240226 add
	//修正依頼168613 
	//m_ShinInfo.d_tnoがMainFrame::BeginApp()でInitSyzSyukei()による集計モジュールの初期化後にリセットされているため
	//一度集計を行うまで月オフセット0分の金額のみが集計されていた。画面描画後初の集計にはGetMofs()が必要。
	GetMofs();
	//<-- 20240226 add
	char uWari[MONY_BUF_SIZE] = { 0 };
	if( StartSyzSyukei( uWari ) == -1 ) {
		return -1;
	}

	return 0;
}

//================================================================
// 還付付表２　フォーカス移動
//----------------------------------------------------------------
// 備考
//		令和5年10月1日以降の課税期間用の還付付表２には登録番号が
//		追加されている。
//		ZmSelを使用して摘要情報からの連動を行っているが、
//		ダイアログ表示・非表示の切り替えの制御が難しい。
//		そのため、当関数で強制的にフォーカスを移動させ
//		ZmSelを非表示にする。
//================================================================
void CMainFrame::MoveKanpuFocus()
{
	if( GetActViewVer() == ID_VER_SYZ31 ) {
		CH31HyoView *pView = ( CH31HyoView* )GetHonHyoView( ID_VER_SYZ31 );
		pView->MoveKanpuFocus();
	}
}

//======================================================
// 個人決算書金額の取得可能
//======================================================
void CMainFrame::IsGetKoketsuKingaku()
{
	// 所得税ｄｂと同居マスター
	// 決算期間の末日が令和5年10月1日以降
	// Koketsu.iniファイルが存在している場合に個人決算書金額の取得可能

	m_isKoketsuKingaku = false;
	//if( m_isHirendoCall ) {
		if(( m_pZmSub->zvol->sub_sw & DBSUBSW_KAKUTEI ) || ( m_pZmSub->zvol->sub_sw & DBSUBSW_KOKETSU)) {
			if( m_pZmSub->zvol->ee_ymd >= ICS_SH_INVOICE_DAY ) {
				CString	filePath;
				char dev[8] = { 0 };
				char gr[8] = { 0 }, jobs[8] = { 0 };

				memset( gr, '\0', sizeof( gr ));
				memset( jobs, '\0', sizeof( jobs ));

				// 実行パスを取得
				PCOM_GetString( _T( "ProgramDevice" ), dev );
				PCOM_GetString ( _T( "ExecGrJobs" ), gr, jobs );

				filePath.Format( _T( "%s:\\ICSWin\\Cmds\\GR%s\\JOBS%s\\Koketsu.txt"), dev, gr, jobs );
				if( PathFileExists( filePath )) {
					m_isKoketsuKingaku = true;
				}
			}
		}
	//}
}

// 24/06/13_税務代理書面 add -->
// -------------------------------------------------------------------------
// 自動入力モードの場合に自動で取得する項目の情報を保存する関数を呼び出す
// -------------------------------------------------------------------------
void CMainFrame::CallWriteTaxAgentDB( CDBNpSub* pZms, CSnHeadData* pSh, CWnd* pWnd )
{
	int				yflg=0,loksgn=0;
	TAXAGENT_INFO	ti;

	if(m_isHirendoCall == false)	yflg=0;		// 消費税申告書
	else														yflg=1;		// 消費税申告書（非連動）

	if(pSh->Sn_Sign4&0x01)	loksgn=1;	// 入力ロック中
	else					loksgn=0;

	memset(&ti,0,sizeof(TAXAGENT_INFO));
	SetTaxagInfoPar(&ti,pSh);

	WriteTaxAgentDB(pZms->m_database,yflg,1,0,0,loksgn,&ti,pWnd,pZms);
}
// 24/06/13_税務代理書面 add <--
