// MainFrm.cpp : CMainFrame クラスの実装
//

#include "stdafx.h"
#include "UCHIWAKE.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// midori 160612 add -->
//== 定数 ==
const	int	ID_SH_LOCK_ID	=	1;
// midori 160612 add <--
// midori 152767 add -->
const	int	ID_SH_LOCK_ID2	=	2;
// midori 152767 add <--
// midori 153784 add -->
int			m_OtherSign;
// midori 153784 add <--

// 修正No.157633_元号日付 add -->
extern BOOL	bG_Kanso;
// 修正No.157633_元号日付 add <--
// インボイス登録番号追加対応_23/12/28 add -->
extern BOOL bG_InvNo;			// インボイス番号使用サイン（決算期末が令和6年3月1日を超えている）
// インボイス登録番号追加対応_23/12/28 add <--

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, ICSMainFrame)

BEGIN_MESSAGE_MAP(CMainFrame, ICSMainFrame)
	ON_WM_CREATE()
	ON_MESSAGE(ICS_MAINFRAME_MESSAGE_CALL,OnCall)
	ON_MESSAGE(ICS_MAINFRAME_MESSAGE_JOBSEL,OnFromForkChain)
	ON_MESSAGE(ICS_MAINFRAME_MESSAGE_COSEL,OnFromParentCosel)
	ON_MESSAGE(WM_CLOSE,OnMessageClose)
// midori 160612 add -->
	ON_MESSAGE(WM_USER_UPDATE_INDICATE, OnUserUpdateIndicate)
// midori 160612 add <--
	ON_WM_QUERYENDSESSION()
	ON_COMMAND( ID_BUTTON_SHINCYOKU,ProgTuuti_Dialog_Toolbar)
	ON_UPDATE_COMMAND_UI( ID_BUTTON_SHINCYOKU, &CMainFrame::OnUpdateButtonShincyoku )
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
{
	SetCommandBarsID( IDR_UCHIWAKETYPE );
	// TODO: メンバ初期化コードをここに追加してください。

	// PCOM_GetString()使用するための処理
#ifdef	_DEBUG
	m_pMon = new ICSMon( GetCurrentProcessId());
	_debug_SetDsn();
	PCOM_SetInt( _T("CompanySin"), 0);
	SET_MonitorVersion( 1 );			// 2010/08/XX 業務統合メニュー対応（統合会社情報固定）
#endif

	// DB財務クラス初期化
	((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub = NULL;
// midori 152710 del -->
//// midori 151403 add -->
//	m_Init=FALSE;
//// midori 151403 add <--
// midori 152710 del <--

// 修正No.158301 add -->
	// 確定のカーソル位置保存
	Kt_rowsv = -1;
	Kt_colmsv = -1;
// 修正No.158301 add <--
}

CMainFrame::~CMainFrame()
{
	// アプリケーションの終了処理(後始末)
	((CUCHIWAKEApp*)AfxGetApp())->EndApp();

	// PCOM_GetString()使用するための処理
#ifdef	_DEBUG
	_debug_ResetDsn();
	delete m_pMon;
#endif
}

int CMainFrame::BeginApp()
{
	// 2006.09.14：ICS様からの｢プログラム開始時に残像が表示される対応｣によるモジュール更新の対応
	int		intIndex = 0;
	BOOL	bFlag	 = FALSE;
// midori UC_0018 add -->
	CString cst = _T("");
	CUcFunctionCommon clsFunc;				// 共通関数クラス
// midori UC_0018 add <--

	CUCHIWAKEApp* pApp = (CUCHIWAKEApp*)AfxGetApp();


// midori 181002 add -->
#ifdef _DEBUG
	pApp->m_swUtwInfoRegister = 0;
// midori UC_0037 add -->
	pApp->m_swUtwTandoku = 0;
// midori UC_0037 add <--
#endif
// midori 181002 add <--

// midori 153784 add -->
	m_OtherSign = 0;
// midori 153784 add <--

	// ↓以下処理、DB財務クラスを取得した場合のみ実行(起動時以外は実行させない)
	// ※様式変更時、このBeginAppが走ってしまうための対応(様式変更時に初期処理(会社選択等)を実行してしまう)
	if ( pApp->m_pZmSub == NULL ){

// midori UC_0037 del -->
//// midori 5,UC_0018 add -->
//		cst = clsFunc.GetGrNo();
//		if(pApp->m_swUtwInfoRegister == 1 || cst.CompareNoCase( _T("1B") ) != 0)	{
//			ICSToolBar *pTool = GetToolBar(0);
//			CMenu menu;
//			menu.LoadMenu( IDR_UCHIWAKETYPE );
//			menu.RemoveMenu( 6,MF_BYPOSITION );
//			pTool->LoadMenu(&menu);
//		}
//// midori 5,UC_0018 add <--
// midori UC_0037 del <--

		if ( pApp->StartApp() <= 0 ){
			PostMessage( WM_CLOSE );
			return -1;
		}

		// 決算期間をウィンドウタイトルに表示します
		if ( pApp->m_pZmSub->VolumeOpen() == 0 ){

			SetKessanki(pApp->m_pZmSub->zvol->ki);
			pApp->m_pZmSub->VolumeClose();

			// ICSMainFrame::SetKessanki()でタイトルバーに決算期を追加した場合、
			// ICSMainFrame::BeginAppEX_()では非クライアント領域が更新されないため
			// タイトルバーに決算期の文言が反映されないまま表示されるので
			// CWnd::RedrawWindow()にて非クライアント領域に再描画を処理させます
			this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_FRAME);

		}else{
			// ボリュームがオープンエラー時は終了します
			pApp->m_clsFunc.ZmSubErrMsg(pApp->m_pZmSub);
			return -1;
		}

		// タブを非表示に設定
		HideMDITabs();

		//***********************************************************************************
		// 2009/09/03 NDS山下
		// TS接続時は、「データインポート」「データエクスポート」メニューを削除(非表示)
		// 　→09/02 ICS様に教えてもらった非表示処理で対応
		//
		//　※起動時のみ実行
		//	　→会社切替でBeginApp()が実行されると、削除済みのメニューをまた削除しようとするため(エラーになる)
		//***********************************************************************************

		// TS接続(顧問先ユーザ(1))で動作している？
		int nUtyp = GET_USERTYPE();
		if ( nUtyp == 1 ){

			ICSToolBar	*pTool = GetToolBar( 0 );			// 「0:メニュー」ツールバーを取得

			CXTPControl *pCtrl = pTool->GetControl( 0 );	// 「0:ファイル」メニューを取得

			CXTPCommandBar	*pCmd = pCtrl->GetCommandBar();
			CXTPControls	*pCtrls;
			pCtrls = pCmd->GetControls();					// 「0:ファイル」メニュ－内のメニュー情報を取得

			pCtrls->Remove( 1 );    // 「データエクスポート」メニューを削除
			pCtrls->Remove( 0 );    // 「データインポート」メニューを削除
									//  ※削除するとインデックス番号が詰まるので、
									//	　下のメニューから削除しないといけない
		}
	}

	GetActiveChild(&intIndex, &bFlag);		// 現在アクティブなタブのインデックスを取得
	SetActiveChild(intIndex);				// 

	// 2009.09.10 append
	//	TS接続時のツールバーからの起動業務数(２つまで)の対応
	//
	//	元々、ICSMainFrame::BeginApp()を実行して欲しいとの話だったが、
	//	内訳書では必ず「①預貯金」が表示されるため、ICS様にモジュールを対応してもらった
	ICSMainFrame::BeginAppEX_();

	//-----------------------------------------------------
	// 一括印刷 既存"一括印刷ダイアログ"表示
	//-----------------------------------------------------
	// 2010/07/XX 一括印刷対応
	// 一括印刷　全モード（設定、出力、CDR）にて実施
	// 　→当処理は、起動後一度だけ実行する。
	// 　　※CMainFrame::BeginApp()は以下のパターンで実行されるので、
	// 　　　2回目以降は実行禁止の制御が必要。
	// 　　　①通常アプリ起動
	// 　　　②会社切替 ← CMainFrame::OnFromParentCosel()からBeginApp()が呼ばれる。
	if (g_BatchPrt.IsBatchExec() == TRUE && m_fShowdlgPrintAllFlg != TRUE) {
		// 現在アクティブな帳表画面のハンドルを取得する
		CFrameWnd*			pFrame	= (CFrameWnd*)AfxGetMainWnd();
		CMDIChildWnd*		pChild	= (CMDIChildWnd *)pFrame->GetActiveFrame();
		CfrmUc000Common*	frmView	= (CfrmUc000Common*)pChild->GetActiveView();

		// 一括印刷ダイアログを表示する
		frmView->OnMenuPrintAll();

		// 表示済フラグ = TRUEとする
		m_fShowdlgPrintAllFlg = TRUE;
	}

	// オプション設定→起動時に様式選択画面を表示する（ただし一括印刷モードではOFF）
	if(g_BatchPrt.IsBatchExec() != TRUE){
		CFrameWnd*			pFrame	= (CFrameWnd*)AfxGetMainWnd();
		CMDIChildWnd*		pChild	= (CMDIChildWnd *)pFrame->GetActiveFrame();
		CfrmUc000Common*	frmView	= (CfrmUc000Common*)pChild->GetActiveView();
		if(frmView != NULL){
			if(frmView->GetDispYoushikiSelect()){
				//frmView->DoButtonInsert(FROM_RUN);
				frmView->PostMessage(WM_YOUSHIKI_SELECT, FROM_RUN);
			}
		}
	}

	return 0;
}

// midori 160612 add -->
//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
BOOL CMainFrame::UpdateLockIndicator( BOOL flg, BOOL flg2 )
{
	CXTPStatusBarPane* pane = m_wndStatusBar.FindPane( ID_SH_LOCK_ID );
	CXTPStatusBarPane* pane2 = m_wndStatusBar.FindPane( ID_SH_LOCK_ID2 );

	pane->SetVisible(flg2);

	// ②
	pane2->SetVisible(flg2);
	if(flg == TRUE)	{
		pane->SetText("『入力確定中』 解除する場合は【解除】をクリックしてください。");
		pane->SetTextColor(RGB(255,0,0));
		pane2->SetText("【解 除】");
		pane2->SetTextColor(RGB(255,255,255));
		pane2->SetBackgroundColor(RGB(255,0,0));
	}
	else	{
		pane->SetText("　　　　 入力を確定する場合は【確定】をクリックしてください。");
		pane->SetTextColor(RGB(0,0,255));
		pane2->SetText("【確 定】");
		pane2->SetTextColor(RGB(255,255,255));
		pane2->SetBackgroundColor(RGB(0,0,255));
	}

	return 0;
}
// midori 160612 add <--

// 2007.04.27 終了処理を内訳書特有→他アプリ共通に変更(他アプリ共通=OnMessageClose)
LRESULT	CMainFrame::OnMessageClose( WPARAM w, LPARAM l )
{
	TRACE(_T("***** CMainFrame::OnMessageClose\n"));

	// 2010/08/02 一括印刷 異常終了対応
	const CString	FN = _T("CMainFrame::OnMessageClose()");
	CString			strLog;
	strLog.Format( _T("START wParam=[0x%02x]"), w );
	DbgWriteLog( FN, strLog );

	// 2009.10.07 Windows7 対応 WPARAM変更（旧:正常終了=0／新:正常終了=1以外）
	// （OS共通：OSメッセージ→ICSモジュール[OS差異吸収]→本メッセージ関数 ）
	//エラー終了時
	if( ICS_MSG_WPARAM_EXIT_ERR == w ) {
		// Viewに対して終了処理を実行させる(2009.09.08 TS自動ログオフ対応)
		CloseView(WM_CLOSE, w);
	}
	//正常終了時
	else {
		// 2010/08/25 税務用年月日セット対応
		// DB財務クラスのポインタ取得
		CDBZmSub* pZmSub = ((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub;
		// 税務用年月日セット
		// 　一括印刷関連の処理（一括印刷アクセスモジュール使用）であるが、
		// 　アプリ終了時共通（一括印刷起動時のみではなく、通常起動時も）で実行する必要がある。
		// 　　→2010/08/25 ICS様より
		if (g_BatchPrt.SetPrtSchYmd( pZmSub ) != TRUE) {
			// エラー終了に倒す → メッセージ表示はCBatchPrt::SetPrtSchYmd()内で行う。
			//					（モジュールからエラーメッセージを取得するため）
			w = ICS_MSG_WPARAM_EXIT_ERR;
			CloseView( WM_CLOSE, w );
		} 
		else {
			// 2010/07/XX 一括印刷対応
			if ( LPARAM_BPRT_EXITAPP == l ) {
				// 一括印刷時の終了通知  CBatchPrt::ExitBatchMode() より送信
				// → 下記【他アプリとの相違点】を参照。
				// 　　よって、このタイミングでCloseView()を実行する。
				CloseView(WM_CLOSE, w);
			}
			else {
				CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
				CMDIChildWnd* pChild = (CMDIChildWnd *)pFrame->GetActiveFrame();

				CfrmUc000Common* frmCmn = (CfrmUc000Common*)pChild->GetActiveView();

				// アプリが起動していない場合は実行しない
				if ( frmCmn != NULL ){
					// 2009.10.08 Windows7 対応 
					// 印刷プレビュー中の場合 → プレビュー：終了／アプリ：終了しない。
					if ( TRUE == this->GetPreviewMode() ) {
						// プレビュー表示中の場合は現在表示されているプレビューのクラスポインタを取得します
						CfrmUc000Common* pView = (CfrmUc000Common*)this->m_pActivePreview;
						// プレビュー終了
						if (pView != NULL) {
							pView->m_prtJob->m_icsVprn.SetOptionMode( POF_PWCLOSE );
						}
						return TRUE;
					}

// インボイス登録番号追加対応_23/11/21 add -->
// 修正No.168537 del -->
					//if(frmCmn->pZmSel && frmCmn->pZmSel->IsDisplay() != FALSE)	{
					//	frmCmn->pZmSel->DialogOFF();
					//}
					//frmCmn->m_EndSyorityu = 1;
// 修正No.168537 del <--
// 修正No.168537 add -->
					frmCmn->m_EndSyorityu = 1;
					if(bG_InvNo == TRUE)	{
						if(frmCmn->pZmSel && frmCmn->pZmSel->IsDisplay() != FALSE)	{
							frmCmn->pZmSel->DialogOFF();
						}

						// 今カーソルがある場所が登録番号の場合、入力された登録番号をチェック
						// 編集中にEnd:業務終了を押下された時の対処
						if(frmCmn->InvNoClmChk() == TRUE)	{
							frmCmn->virCallChkInvoiceNum(3, frmCmn->m_uInfo.intCurPage, frmCmn->m_uInfo.intCurRow);
						}
					}
// 修正No.168537 add <--
// インボイス登録番号追加対応_23/11/21 add <--

					// 進捗記録　
					if( w != ICS_MSG_WPARAM_EXIT_IKKATU ){	//一括終了時はダイアログを表示させない
						ProgTuuti_Dialog_End( pZmSub );
					}

					// 終了処理
					frmCmn->SetEndViewFlag( TRUE );	// 終了フラグ ここで立てないとpDiag->ChangeFontSizeでデータ変更直後のEndキーでエラーが出るため
					if ( frmCmn->CmnEndView(WM_CLOSE, w) == FALSE ){
						frmCmn->SetEndViewFlag( FALSE );
// インボイス登録番号追加対応_23/11/21 add -->
						frmCmn->m_EndSyorityu = 0;
// インボイス登録番号追加対応_23/11/21 add <--
						return TRUE;
					}
				}
			}
		}
	}

	// 【他アプリとの相違点】
	//　 内訳書アプリでは、上記frmCmn->CmnEndView()内で、Viewに対し、
	//　 CloseDocument()を実行している。これにより、Viewはクローズする。
	// 　（Viewのデストラクタが実行される）
	// 　このため、以下のCloseView()を実行しても、Viewの終了処理は行われない。
	// 　よって、
	// 　　・エラー終了時
	// 　　・frmCmn->CmnEndView()内
	// 　で、Viewの終了処理を行うこととする。
	// 
	//// Viewに対して終了処理を実行させる(2009.09.08 TS自動ログオフ対応)
	//CloseView(WM_CLOSE, w);

	// 2010/08/02 一括印刷対応 異常終了対応
	DbgWriteLog( FN, _T("END") );
	return	ICSMainFrame::OnMessageClose( w, l );
}

int CMainFrame::TabLButtonDown()
{
	ICSMainFrame::TabLButtonDown();
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
		if( st ) {
// 修正No.168516 add -->
			CFrameWnd*			pFrame = (CFrameWnd*)AfxGetMainWnd();
			CMDIChildWnd*		pChild = (CMDIChildWnd *)pFrame->GetActiveFrame();
			CfrmUc000Common*	frmCmn = (CfrmUc000Common*)pChild->GetActiveView();

			if(frmCmn != NULL)	{
				// 登録番号ダイアログを更新
				frmCmn->UpdateZmSel();
			}
// 修正No.168516 add <--
		}
	}
	//自身がアクティブでボタン押下により、他のプロセスへ切り替えられようとしているとき
	else if( wParam == ICS_MAINFRAME_PARAM_FORKCHANGING ) {
// 要通知案件修正 21/07/09 add -->
		if(this->GetPreviewMode() == FALSE) {
			CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
			CMDIChildWnd* pChild = (CMDIChildWnd *)pFrame->GetActiveFrame();
			CfrmUc000Common* frmCmn = (CfrmUc000Common*)pChild->GetActiveView();
			if(frmCmn != NULL) {
				frmCmn->CmnEventStartProc(ICS_MAINFRAME_PARAM_FORKCHANGING, lParam);
			}
		}
// 要通知案件修正 21/07/09 add <--

		//他のプロセスで再リードが必要な場合は「1」を返す。
// midori 153784 add -->
		if(m_OtherSign == 1)	{
			st = 1;
			m_OtherSign = 0;
		}
// midori 153784 add <--
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
// midori 180104 del -->
	//CoSel icsCoSel;
// midori 180104 del <--
	BOOL  blnCmnFlg = FALSE;

	while(1){
// midori 152710 del -->
//// midori 151403 add -->
//		m_Init=TRUE;
//// midori 151403 add <--
// midori 152710 del <--
		// 他業務からの会社切替でICSDiag8がアクティブでないときに
		// SetComboFontSize()をコールすると不正終了するのでここでアクティブ化させる
		CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
		CMDIChildWnd* pChild = (CMDIChildWnd *)pFrame->GetActiveFrame();
		CfrmUc000Common* frmCmn = (CfrmUc000Common*)pChild->GetActiveView();

// 要通知案件修正 21/07/09 add -->
		// 実行前の準備処理
		frmCmn->CmnEventStartProc(ICS_MAINFRAME_MESSAGE_COSEL);
// 要通知案件修正 21/07/09 add <--

// インボイス登録番号追加対応_23/11/21 add -->
		if(bG_InvNo == TRUE)	{	// 修正No.168537 add
			if(frmCmn->pZmSel && frmCmn->pZmSel->IsDisplay() != FALSE)	{
				frmCmn->pZmSel->DialogOFF();
			}
		}
		frmCmn->m_EndSyorityu = 1;
// インボイス登録番号追加対応_23/11/21 add <--

		frmCmn->SetFocus();

		//	他のプロセスで会社選択ツールバーで会社が変更された
		if( wParam == 1){
// midori 180104 del -->
			//if( icsCoSel.co_ok() != IDOK){
// midori 180104 del <--
// midori 180104 add -->
			if(((CUCHIWAKEApp*)AfxGetApp())->co_ok_ex(1) != IDOK)	{
// midori 180104 add <--
// midori 152710 del -->
//// midori 151403 add -->
//				m_Init=FALSE;
//// midori 151403 add <--
// midori 152710 del --
				return 1;
			}
		}
		//	自分自身の会社選択ツールバーが押された
		else{
			// 進捗管理レコード書き込み
			ProgTuuti_Dialog_End( ((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub );

// midori 181002 del -->
//// midori 180104 del -->
//			//COPAK cop;
//			//cop.sgn0 = 0x10;
//			//if( icsCoSel.co_ok( cop) != IDOK){
//// midori 180104 del <--
//// midori 180104 add -->
//			if(((CUCHIWAKEApp*)AfxGetApp())->co_ok_ex(2) != IDOK)	{
//// midori 180104 add <--
// midori 181002 del <--
// midori 181002 add -->
			int nRet = ((CUCHIWAKEApp*)AfxGetApp())->co_ok_ex(2);
			// エラー
			if(nRet == -1) {
				// 帳表データ入力確定
				frmCmn->CmnEventStartProc(0);

				// カレント情報保存
				frmCmn->CmnUcInfSubSetCurrentInfo();

				// 空行削除
				if ( wParam != 1 ){
					// 空行データの削除処理
					if ( frmCmn->CmnTblCheckDeleteData() == TRUE ){
						frmCmn->CmnTblEndDeleteData();
					}
				}

				ICSMessageBox(_T("処理を終了します。"));
				AfxGetMainWnd()->PostMessage( WM_CLOSE, 1, 0 );
				return 1;
			}
			if(nRet != IDOK)	{
// midori 181002 add <--
				// 2008.11.24 Append ･･･ 売掛管理の修正依頼の対応
				//	会社切替で既に元の会社のDBを切断している場合、アプリを終了させる
				if (blnCmnFlg == TRUE) {
					AfxGetMainWnd()->PostMessage( WM_CLOSE, 1, 0 );
				}
// midori 152710 del -->
//// midori 151403 add -->
//				m_Init=FALSE;
//// midori 151403 add <--
// midori 152710 del <--

// インボイス登録番号追加対応_23/11/21 add -->
				frmCmn->m_EndSyorityu = 0;
// インボイス登録番号追加対応_23/11/21 add <--

// midori M-16090701 add -->
				// カーソル移動
				frmCmn->CmnDiagSetFocus(frmCmn->m_pDiag,frmCmn->m_uInfo.intCurRow,frmCmn->m_uInfo.intCurCol);
// midori M-16090701 add <--
				return 1;
			}
		}

		// 帳表終了時の設定
		if ( blnCmnFlg == FALSE ){
			// 帳表データ入力確定
			frmCmn->CmnEventStartProc(0);

			// カレント情報保存
			frmCmn->CmnUcInfSubSetCurrentInfo();

			// 空行削除
			if ( wParam != 1 ){
				// 空行データの削除処理
				if ( frmCmn->CmnTblCheckDeleteData() == TRUE ){
					frmCmn->CmnTblEndDeleteData();
				}
			}

//			// 子ウィンドウ閉じる
//			frmCmn->CmnCloseActiveDocument();
//			((CUCHIWAKEApp*)AfxGetApp())->CloseDocument();

			// DBハンドル開放
			delete ((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub;
			((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub = NULL;
			
			// フラグON
			blnCmnFlg = TRUE;
			frmCmn->SetEndViewFlag( TRUE );
		}

// midori 152710 del -->
//// midori 151403 add -->
//		m_Init=FALSE;
//// midori 151403 add <--
// midori 152710 del <--
// midori 153784 add -->
		m_OtherSign = 0;
// midori 153784 add <--
		// 業務選択ツールバーでの会社切り替え
		if ( wParam == 1 ){
			// 会社切り替え処理実行;
			int nRet = ((CUCHIWAKEApp*)AfxGetApp())->StartApp(2);
			if ( nRet == 0 ){
				// 親ウィンドウのエンド処理
				AfxGetMainWnd()->PostMessage( WM_CLOSE, 1, 0 );
				return 1;
			}
			else if( nRet == -1 ){
				AfxGetMainWnd()->PostMessage( WM_CLOSE, 1, 0 );
				return 1;
			}
		}
		// 自身の会社選択ツールバーからの会社切り替え
		else{
			// 会社切り替え処理実行
			int nRet = ((CUCHIWAKEApp*)AfxGetApp())->StartApp(1);
			if ( nRet == 0 ){
				continue;
			}
			else if( nRet == -1 ){
				AfxGetMainWnd()->PostMessage( WM_CLOSE, 1, 0 );
				return 1;
			}
		}

		// 決算期間をウィンドウタイトルに表示します
		if ( ((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub->VolumeOpen() == 0 ){
			SetKessanki(((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub->zvol->ki);
			((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub->VolumeClose();
		}else{
			// ボリュームがオープンエラー時は終了します
			((CUCHIWAKEApp*)AfxGetApp())->m_clsFunc.ZmSubErrMsg(((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub);
			return 1;
		}

		// 子ウィンドウ表示
		((CUCHIWAKEApp*)AfxGetApp())->BeginApp();

// インボイス登録番号追加対応_23/11/21 add -->
		frmCmn->m_EndSyorityu = 0;
// インボイス登録番号追加対応_23/11/21 add <--

		return	0;
	}
}


//***************************************************************************
//（2009.09.08 TS自動ログオフ対応）
//	Windowsログオフが実行された時の処理
//
//	IN		なし
//	RET		BOOL				TRUE=エラー，FALSE=正常
//***************************************************************************
BOOL CMainFrame::OnQueryEndSession()
{
	TRACE(_T("***** CMainFrame::OnQueryEndSession\n"));

	if (!ICSMainFrame::OnQueryEndSession())
		return FALSE;

	// Viewに対して終了処理を実行させる
	CloseView(WM_QUERYENDSESSION, NULL);

	return TRUE;
}

//***************************************************************************
//（2009.09.08 TS自動ログオフ対応）
//	Viewに対して終了処理を実行させる
//
//	IN		UINT				メッセージ
//			WPARAM				パラメータ
//	RET		BOOL				TRUE=エラー，FALSE=正常
//***************************************************************************
BOOL CMainFrame::CloseView(UINT message, WPARAM wParam)
{
	TRACE(_T("***** CMainFrame::CloseView\n"));

	// 2010/08/02 一括印刷対応 異常終了対応
	const CString	FN = _T("CMainFrame::CloseView()");
	CString			strLog;
	strLog.Format( _T("START message=[0x%02x], wParam=[0x%02x]"), message, wParam );
	DbgWriteLog( FN, strLog );

	//	2010/08/10 一括印刷 ログオフ時 UCOM I/F 返却なし対応
	g_BatchPrt.SetWmExitMsg( message, wParam );	

	// アプリポインタを取得する(ドキュメント管理情報取得のため)
	CUCHIWAKEApp*	pApp		= (CUCHIWAKEApp*)AfxGetApp();

	// すべての画面に対して行う
// midori 190301 del -->
//	for (int i = 0; i < UC_ID_FORMNO_MAX; i++) {
// midori 190301 del <--
// midori 190301 add -->
	//for(int i=0; i<UC_ID_FORMNO_MAX_KANSO; i++)	{	// インボイス登録番号追加対応_23/09/11 del
	for(int i=0; i<UC_ID_FORMNO_MAX_INVO; i++)	{		// インボイス登録番号追加対応_23/09/11 add
// midori 190301 add <--
		// Viewがオープンされていなければ無視
		if (pApp->m_uDocInfo[i].bOpenFlg != TRUE) {
			continue;
		}

		// Viewのポインタを取得する
		CfrmUc000Common* pView = (CfrmUc000Common*)((CUCHIWAKEApp*)AfxGetApp())->GetDocumentTemplate()->GetViewWnd(pApp->m_uDocInfo[i].nID);

		// Viewに終了メッセージを投げる
		if (pView != NULL) {
			pView->EndApp(message, wParam);
		}
	}

	// 2010/08/10 一括印刷 印刷時メッセージ非表示対応 条件変更
	// 2010/07/22 一括印刷対応
	// 2010/08/02 一括印刷 異常終了対応
	// 設定モードでの起動時
	if ( g_BatchPrt.IsBatchExec() ) {
		EndProcBprt();
	}

	// 2010/08/02 一括印刷対応 異常終了対応
	DbgWriteLog( FN, _T("END") );
	return TRUE;
}

//----------------------------------------------------------------
//	一括印刷時、終了処理（UCOM への情報返却）
//
//	IN		なし
//	RET		なし
//
// 2010/08/02 一括印刷 異常終了対応
//----------------------------------------------------------------
BOOL CMainFrame::EndProcBprt(void)
{
	// 2010/08/02 一括印刷対応
	const CString	FN = _T("CMainFrame::EndProcBprt()");
	DbgWriteLog( FN, _T("START") );

	// 2010/08/10 一括印刷 ログオフ時 UCOM I/F 返却なし対応
	if ( WM_CLOSE != g_BatchPrt.GetWmExitMsg() ) {
		DbgWriteLog( FN, _T("END - 終了通知 ≠ WM_CLOSE") );
		return TRUE;
	}

	// 2010/08/10 一括印刷 印刷時メッセージ非表示対応 条件変更
	BATCHPRTUCOMINF_RETPRT	udBatchPrtRet;

	// 2010/07/XX 一括印刷対応
	switch ( g_BatchPrt.GetBatchMode() ) { 
	//--------------------------------
	// 一括印刷 設定モードでの起動時
	//--------------------------------
	case ID_BATCHMODE_SET:		
		// 2010/08/02 一括印刷 異常終了対応
		// 正常終了時のみ UCOM I/F にセット
		//
		// 異常終了時、UCOM へのセットは行わない
		//
		// 2010/08/02 ICS中垣内様より
		// 一括印刷アプリの 判断（出力アプリ 正常終了／異常終了）
		//
		//	・一括印刷アプリが、起動時にUCOMにセットしない情報で、アプリから返却される情報 の 有無で 判断。
		//	　→「用紙サイズ」「用紙方向」「PDF出力ファイル名」等が UCOM にセットされていない場合、出力アプリ異常終了と判断。
		//
		if (( ICS_MSG_WPARAM_EXIT_ERR != g_BatchPrt.GetWmExitParam() ) &&
			( FALSE == g_BatchPrt.IsErrorExit() )) {

			// 2010/08/31 修正依頼No94 対応 「勘定科目内訳書」「附属明細表」出力年月日情報に「-1」を返却
			// zvolume より 期首期末情報取得処理削除

			// 設定 UCOM 返却情報セット
			BATCHPRTUCOMINF_RETSET udSetInf;
			udSetInf.nPrtSize		= ID_PRTSIZE_A4T;		// 用紙選択 A4縦固定
			// 勘定科目内訳書 不使用 情報
			udSetInf.nPrtOffSetS	= -1;					// 設定・出力月オフセット 開始
			udSetInf.nPrtOffSetE	= -1;					// 設定・出力月オフセット 終了
			// 2010/08/31 修正依頼No94 対応 「勘定科目内訳書」「附属明細表」出力年月日情報に「-1」を返却
			udSetInf.nPrtYmdS		= -1;					// 設定・出力年月日 開始 
			udSetInf.nPrtYmdE		= -1;					// 設定・出力年月日 終了
			udSetInf.nFlgYmd		= 0;					// 設定・出力年月日 日までの情報不要

			udSetInf.strPdfName		 = PRT_NAME_JOB;		// 設定・PDF出力ファイル名	2010/07/31 追加（仕様変更 7/30）
			// UCOM にセット
			g_BatchPrt.SetUcomInfSet( &udSetInf );
		}
		else {
			DbgWriteLog( FN, _T("異常終了時 → UCOM I/F セット無") );	
		}

		break;

	//--------------------------------
	// 一括印刷 出力モードでの起動時
	//--------------------------------
	case ID_BATCHMODE_PRT:
		// 2010/08/10 一括印刷 印刷時メッセージ非表示対応
		// WM_CLOSE受信後の 終了処理 EndProcBprt() で実施とする。CCmnView::OnButtonF5() から移動
		// 印刷結果格納
		if (( ICS_MSG_WPARAM_EXIT_ERR != g_BatchPrt.GetWmExitParam() ) &&
			( FALSE == g_BatchPrt.IsErrorExit() )) {
			udBatchPrtRet.nPrtRet = UCOMITM_BATCHPRT_OK;	// 印刷成功
		}
		else { 
			udBatchPrtRet.nPrtRet = UCOMITM_BATCHPRT_NG; 	// 印刷失敗
		}
		// 印刷結果 セット（UCOM）
		g_BatchPrt.SetUcomInfPrt( &udBatchPrtRet );
		break;

	//--------------------------------
	// 一括印刷 CDRモードでの起動時
	//--------------------------------
	case ID_BATCHMODE_CDR:
		// CDR出力時は UCOM へのセットは行わない
		break;

	default:
		break;
	}

	DbgWriteLog( FN, _T("END") );
	return TRUE;
}

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
	const CString	LOG_FILE = "C:\\NDS_DBG\\DBUchiwake.log";		

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

//----------------------------------------------------------------
//	終了処理時に進捗管理テーブルに通知する(ツールバー押下)
//
//	IN		なし
//	RET		なし
//
//----------------------------------------------------------------
#include "kmnctrl8.h"
#include "DBprogress_Mod8.h"
#include "DBProgTuutiGen8.h"
void CMainFrame::ProgTuuti_Dialog_Toolbar(/*CDBZmSub* pZmSub　OnCommandで呼ばれる関数は引数を持てない！*/)
{
	CDBZmSub* pZmSub = ((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub;
	ProgData data = GetInitProgData();
	wsprintf(data.pd_msid, "%s", "MS001002");

// インボイス登録番号追加対応_23/11/21 add -->
	CFrameWnd*			pFrame = (CFrameWnd*)AfxGetMainWnd();
	CMDIChildWnd*		pChild = (CMDIChildWnd *)pFrame->GetActiveFrame();
	CfrmUc000Common*	frmCmn = (CfrmUc000Common*)pChild->GetActiveView();
	if(frmCmn != NULL)	{
		if(bG_InvNo == TRUE)	{	// 修正No.168537 add
			if(frmCmn->pZmSel && frmCmn->pZmSel->IsDisplay() != FALSE)	{
				frmCmn->pZmSel->DialogOFF();
			}
		}
	}
// インボイス登録番号追加対応_23/11/21 add <--

	if ( pZmSub->VolumeOpen() == 0 ){
		data.pd_jigyo_symd = pZmSub->zvol->ss_ymd;
		data.pd_jigyo_eymd = pZmSub->zvol->ee_ymd;
		wsprintf( data.Komon_Nam, "%s", pZmSub->zvol->c_nam );
		pZmSub->VolumeClose();
	}
	else{
		return;
	}

	ProgTuutiGen( this, &data );

	return;
}

//----------------------------------------------------------------
//	終了処理時に進捗管理テーブルに通知する(終了時)
//
//	IN		なし
//	RET		なし
//
//----------------------------------------------------------------
int CMainFrame::ProgTuuti_Dialog_End(CDBZmSub* pZmSub)
{
	ProgData data = GetInitProgData();
	wsprintf(data.pd_msid, "%s", "MS001002");

	if ( pZmSub->VolumeOpen() == 0 ){
		data.pd_jigyo_symd = pZmSub->zvol->ss_ymd;
		data.pd_jigyo_eymd = pZmSub->zvol->ee_ymd;
		wsprintf( data.Komon_Nam, "%s", pZmSub->zvol->c_nam );
		pZmSub->VolumeClose();
	}
	else{
		return -1;
	}

	ProgEndTuuti( this, &data );

	return 0;
}

//　[：進捗記録]　ボタン監視
void CMainFrame::OnUpdateButtonShincyoku(CCmdUI* pCmdUI)
{
	BOOL ShincyokuButtonState = FALSE;

	// 通常起動時（一括印刷でない）でかつ進捗管理を行う
	if( g_BatchPrt.IsBatchExec() == FALSE &&
		GetShincyokuAdminSign( 1 ) != 0){
			ShincyokuButtonState = TRUE;
	}

	pCmdUI->Enable(ShincyokuButtonState);
}

// midori 160612 add -->
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
// midori 152767 del -->
	//CXTPStatusBarPane* pane = Bar.AddIndicator( id,1 );
	//CFont f;
	//f.CreateFont(
	//	12, 0, 0, 0,
	//	FW_BOLD, FALSE, FALSE, 0,
	//	SHIFTJIS_CHARSET,
	//	OUT_DEFAULT_PRECIS, 
	//	CLIP_DEFAULT_PRECIS, 
	//	DEFAULT_QUALITY, DEFAULT_PITCH, _T("ＭＳ ゴシック")
	//);
	//pane->SetTextFont(&f);
	//pane->SetText(text);
	//pane->SetWidth(360);
	//pane->SetTextColor( color );
	//pane->SetBackgroundColor(RGB(0xff,0xff,0xff));
	//pane->SetTextAlignment(1);
	//pane->SetVisible(FALSE);
// midori 152767 del <--

// midori 155049 add -->
	// 一番左のステータスバー(レディ)の幅を縮める
	if(id == ID_SH_LOCK_ID)	{
		CXTPStatusBarPane*	pane0;
		pane0 = Bar.GetPane(0);
		pane0->SetWidth(300);
	}
// midori 155049 add <--

// midori 152767 add -->
	CXTPStatusBarPane*	pane;

	if(id == ID_SH_LOCK_ID)	{
		pane = Bar.AddIndicator( id,1 );
	}
	else					{
		pane = Bar.AddIndicator( id,2 );
	}

	CFont f;
	f.CreateFont(
		12, 0, 0, 0,
		FW_NORMAL, FALSE, FALSE, 0,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, DEFAULT_PITCH, _T("ＭＳ ゴシック")
	);
	pane->SetTextFont(&f);
	pane->SetText(text);
	if(id == ID_SH_LOCK_ID)	pane->SetWidth(360);
	else					pane->SetWidth(51);
	pane->SetTextColor( color );
	pane->SetBackgroundColor(RGB(0xff,0xff,0xff));
	pane->SetTextAlignment(1);
	pane->SetVisible(FALSE);
// midori 152767 add <--
}

BOOL CMainFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle , const RECT& rect , CWnd* pParentWnd , LPCTSTR lpszMenuName , DWORD dwExStyle , CCreateContext* pContext)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	int rt = ICSMainFrame::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, lpszMenuName, dwExStyle, pContext);

	SetPane( m_wndStatusBar, ID_SH_LOCK_ID, _T(""), RGB(0,0,0) );
// midori 152767 add -->
	SetPane( m_wndStatusBar, ID_SH_LOCK_ID2, _T(""), RGB(0,0,0) );
// midori 152767 add <--

	return rt;
}

//-----------------------------------------------------------------------------
// メニュー更新
//-----------------------------------------------------------------------------
LRESULT CMainFrame::OnUserUpdateIndicate( WPARAM wParam, LPARAM lParam )
{
// 24/01/30_23-0845 add -->
	if(((CUCHIWAKEApp*)AfxGetApp())->m_CloudSansyoFlg == TRUE)	{
		return(0);
	}
// 24/01/30_23-0845 add <--
	CFrameWnd*			pFrame = (CFrameWnd*)AfxGetMainWnd();
	CMDIChildWnd*		pChild = (CMDIChildWnd *)pFrame->GetActiveFrame();
	CfrmUc000Common*	frmCmn = (CfrmUc000Common*)pChild->GetActiveView();
// midori 152766 del -->
	//BOOL				flg = FALSE;

	//if(frmCmn->m_DataKakutei)	flg = TRUE;

	//UpdateLockIndicator( flg );
// midori 152766 del <--
// midori 152766 add -->
	BOOL				flg = FALSE;
	BOOL				flg2 = FALSE;

	if(frmCmn->m_DataKakutei)	flg = TRUE;
	if(wParam == 1)				flg2 = TRUE;

	UpdateLockIndicator( flg, flg2 );
// midori 152766 add <--

	return 0;
}
// midori 160612 add <--

// midori 152718 add -->
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_LBUTTONDOWN)	{
		CXTPStatusBarPane*	ppane;
		CRect				rect;
		CPoint				point;
		int					xpos=0,ypos=0;
		int					sw=0;

		xpos = GET_X_LPARAM(pMsg->lParam); 
		ypos = GET_Y_LPARAM(pMsg->lParam); 

		point.SetPoint(xpos,ypos);
		ppane = m_wndStatusBar.HitTest(point);

		::ClientToScreen(pMsg->hwnd,&point);

		m_wndStatusBar.GetWindowRect(&rect);
		if(rect.PtInRect(point))	{
			if(ppane != NULL)	{
// midori 152767 del -->
//				if(ppane->GetID() == ID_SH_LOCK_ID)	{
// midori 152767 del <--
// midori 152767 add -->
				if(ppane->GetID() == ID_SH_LOCK_ID2)	{
// midori 152767 add <--
					OnKakuteiKaijyo();
					sw=1;
				}
			}
		}
		if(sw != 0)	{
			return(TRUE);
		}
	}
	return ICSMainFrame::PreTranslateMessage(pMsg);
}

void CMainFrame::OnKakuteiKaijyo()
{
	CFrameWnd*			pFrame = (CFrameWnd*)AfxGetMainWnd();
	CMDIChildWnd*		pChild = (CMDIChildWnd *)pFrame->GetActiveFrame();
	CfrmUc000Common*	frmCmn = (CfrmUc000Common*)pChild->GetActiveView();
	BOOL				flg = FALSE;
	BOOL				bkaku = FALSE;
	int					intIndex=0;
	CString				msg;
// midori UC_0053 add -->
	CUcFunctionCommon	clsFunc;				// 共通関数クラス
// midori UC_0053 add <--
// 修正No.157633_元号日付 add -->
// 修正No.158301 del -->
	//int		rowsv = -1, colmsv = -1;
// 修正No.158301 del <--
	int		colmset = -1;
	int		sign = 0;
// 修正No.157633_元号日付 add <--

// midori 154787 add -->
	// 入力データの確定を行う
	frmCmn->CmnEventStartProc(ID_USER_SETTING_MENU);
// midori 154787 add <--

// 修正No.158301 del -->
//// 修正No.157633_元号日付 add -->
//	rowsv = colmsv = -1;
//	sign = 0;
//	if(frmCmn->m_uInfo.intFormSeq == ID_FORMNO_021)	{
//		if(frmCmn->m_uInfo.intCurCol == ID_COL_021_SPDATE || frmCmn->m_uInfo.intCurCol == ID_COL_021_PMDATE)	{
//			colmset = ID_COL_021_KAMOKU;
//			sign = 1;
//		}
//	}
//	else if(frmCmn->m_uInfo.intFormSeq == ID_FORMNO_061)	{
//		if(bG_Kanso == TRUE && frmCmn->m_uInfo.intCurCol == ID_COL_061K_SPDATE)	{
//			colmset = ID_COL_061K_KUBUN;
//			sign = 1;
//		}
//		else if(bG_Kanso == FALSE && frmCmn->m_uInfo.intCurCol == ID_COL_061_SPDATE)	{
//			colmset = ID_COL_061_KUBUN;
//			sign = 1;
//		}
//	}
//	else if(frmCmn->m_uInfo.intFormSeq == ID_FORMNO_071)	{
//		if(frmCmn->m_uInfo.intCurCol == ID_COL_071_SPDATE || frmCmn->m_uInfo.intCurCol == ID_COL_071_SPDATE2)	{
//			colmset = ID_COL_071_SYURUI;
//			sign = 1;
//		}
//	}
//	else if(frmCmn->m_uInfo.intFormSeq == ID_FORMNO_081)	{
//		if(frmCmn->m_uInfo.intCurCol == ID_COL_081_SPDATE || frmCmn->m_uInfo.intCurCol == ID_COL_081_PMDATE)	{
//			colmset = ID_COL_081_ADNAME1;
//			sign = 1;
//		}
//	}
//	else if(frmCmn->m_uInfo.intFormSeq == ID_FORMNO_102)	{
//		if(frmCmn->m_uInfo.intCurCol == ID_COL_102_SPDATE)	{
//			colmset = ID_COL_102_KNORDER;
//			sign = 1;
//		}
//	}
//	else if(frmCmn->m_uInfo.intFormSeq == ID_FORMNO_121)	{
//		if(frmCmn->m_uInfo.intCurCol == ID_COL_121_SPDATE)	{
//			colmset = ID_COL_121_KUBUN;
//			sign = 1;
//		}
//	}
//	else if(frmCmn->m_uInfo.intFormSeq == ID_FORMNO_151)	{
//		if(frmCmn->m_uInfo.intCurCol == ID_COL_151_SPDATE || frmCmn->m_uInfo.intCurCol == ID_COL_151_SPDATE2)	{
//			colmset = ID_COL_151_KUBUN;
//			sign = 1;
//		}
//	}
//	else if(frmCmn->m_uInfo.intFormSeq == ID_FORMNO_152)	{
//		if(frmCmn->m_uInfo.intCurCol == ID_COL_152_SPDATE)	{
//			colmset = ID_COL_152_ADNAME1;
//			sign = 1;
//		}
//	}
//	else if(frmCmn->m_uInfo.intFormSeq == ID_FORMNO_153)	{
//		if( frmCmn->m_uInfo.intCurCol == ID_COL_153_KYDATE || frmCmn->m_uInfo.intCurCol == ID_COL_153_KYDATE2 ||
//			frmCmn->m_uInfo.intCurCol == ID_COL_153_SPDATE || frmCmn->m_uInfo.intCurCol == ID_COL_153_SPDATE2 )	{
//			colmset = ID_COL_153_HDNAME;
//			sign = 1;
//		}
//	}
//	if(sign == 1)	{
//		rowsv = frmCmn->m_uInfo.intCurRow;
//		colmsv = frmCmn->m_uInfo.intCurCol;
//		frmCmn->CmnDiagSetFocus(frmCmn->m_pDiag, frmCmn->m_uInfo.intCurRow, colmset);
//	}
//// 修正No.157633_元号日付 add <--
// 修正No.158301 del <--

	if(frmCmn->m_DataKakutei != 0)	bkaku = TRUE;
	else							bkaku = FALSE;

// 修正No.158301 add -->
	if(bkaku == FALSE) {
		Kt_rowsv = Kt_colmsv = -1;
		sign = 0;
// 修正No.168358,168521 del -->
		//if(frmCmn->m_uInfo.intFormSeq == ID_FORMNO_021) {
		//	if(frmCmn->m_uInfo.intCurCol == ID_COL_021_SPDATE || frmCmn->m_uInfo.intCurCol == ID_COL_021_PMDATE) {
		//		colmset = ID_COL_021_KAMOKU;
		//		sign = 1;
		//	}
		//}
		//else if(frmCmn->m_uInfo.intFormSeq == ID_FORMNO_061) {
		//	if(bG_Kanso == TRUE && frmCmn->m_uInfo.intCurCol == ID_COL_061K_SPDATE) {
		//		colmset = ID_COL_061K_KUBUN;
		//		sign = 1;
		//	}
		//	else if(bG_Kanso == FALSE && frmCmn->m_uInfo.intCurCol == ID_COL_061_SPDATE) {
		//		colmset = ID_COL_061_KUBUN;
		//		sign = 1;
		//	}
		//}
		//else if(frmCmn->m_uInfo.intFormSeq == ID_FORMNO_071) {
		//	if(frmCmn->m_uInfo.intCurCol == ID_COL_071_SPDATE || frmCmn->m_uInfo.intCurCol == ID_COL_071_SPDATE2) {
		//		colmset = ID_COL_071_SYURUI;
		//		sign = 1;
		//	}
		//}
		//else if(frmCmn->m_uInfo.intFormSeq == ID_FORMNO_081) {
		//	if(frmCmn->m_uInfo.intCurCol == ID_COL_081_SPDATE || frmCmn->m_uInfo.intCurCol == ID_COL_081_PMDATE) {
		//		colmset = ID_COL_081_ADNAME1;
		//		sign = 1;
		//	}
		//}
		//else if(frmCmn->m_uInfo.intFormSeq == ID_FORMNO_102) {
		//	if(frmCmn->m_uInfo.intCurCol == ID_COL_102_SPDATE) {
		//		colmset = ID_COL_102_KNORDER;
		//		sign = 1;
		//	}
		//}
		//else if(frmCmn->m_uInfo.intFormSeq == ID_FORMNO_121) {
		//	if(frmCmn->m_uInfo.intCurCol == ID_COL_121_SPDATE) {
		//		colmset = ID_COL_121_KUBUN;
		//		sign = 1;
		//	}
		//}
		//else if(frmCmn->m_uInfo.intFormSeq == ID_FORMNO_151) {
		//	if(frmCmn->m_uInfo.intCurCol == ID_COL_151_SPDATE || frmCmn->m_uInfo.intCurCol == ID_COL_151_SPDATE2) {
		//		colmset = ID_COL_151_KUBUN;
		//		sign = 1;
		//	}
		//}
		//else if(frmCmn->m_uInfo.intFormSeq == ID_FORMNO_152) {
		//	if(frmCmn->m_uInfo.intCurCol == ID_COL_152_SPDATE) {
		//		colmset = ID_COL_152_ADNAME1;
		//		sign = 1;
		//	}
		//}
		//else if(frmCmn->m_uInfo.intFormSeq == ID_FORMNO_153) {
		//	if(frmCmn->m_uInfo.intCurCol == ID_COL_153_KYDATE || frmCmn->m_uInfo.intCurCol == ID_COL_153_KYDATE2 ||
		//		frmCmn->m_uInfo.intCurCol == ID_COL_153_SPDATE || frmCmn->m_uInfo.intCurCol == ID_COL_153_SPDATE2) {
		//		colmset = ID_COL_153_HDNAME;
		//		sign = 1;
		//	}
		//}
// 修正No.168358,168521 del <--
// 修正No.168358,168521 add -->
		sign = frmCmn->CmnTblDataToFirstCol(&colmset);
// 修正No.168358,168521 add <--
		if(sign == 1) {
			Kt_rowsv = frmCmn->m_uInfo.intCurRow;
			Kt_colmsv = frmCmn->m_uInfo.intCurCol;
			frmCmn->CmnDiagSetFocus(frmCmn->m_pDiag, frmCmn->m_uInfo.intCurRow, colmset);
		}
	}
// 修正No.158301 add <--

	if(bkaku == TRUE)	{
		msg.Format( _T("入力確定を解除します。\nよろしいですか？"));
	}
	else	{
		msg.Format( _T("入力を確定します。\nよろしいですか？"));
	}
	
	if(ICSMessageBox(msg, (MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2)) != IDOK)	{
// 修正No.158301 del -->
//// 修正No.157633_元号日付 add -->
//		if(rowsv != -1 || colmsv != -1)	{
//			frmCmn->CmnDiagSetFocus(frmCmn->m_pDiag, rowsv, colmsv);
//		}
//// 修正No.157633_元号日付 add <--
// 修正No.158301 del <--
// 修正No.158301 add -->
		if(Kt_rowsv != -1 || Kt_colmsv != -1)	{
			frmCmn->CmnDiagSetFocus(frmCmn->m_pDiag, Kt_rowsv, Kt_colmsv);
			Kt_rowsv = -1;
			Kt_colmsv = -1;
		}
// 修正No.158301 add <--
		return;
	}

	if(bkaku == TRUE)	bkaku = FALSE;
	else				bkaku = TRUE;
	// 確定チェックが変更されたので、ｄｂに書き込んで画面再表示
	// ｄｂ更新
	CdbUcInfMain	rsMain(frmCmn->m_pDB);
	if(rsMain.Init() == DB_ERR_OK){
		rsMain.Edit();
		CCtrlMainGeneralVar cm(rsMain);
		cm.bDataKakutei = bkaku;

		cm.Set(rsMain);
		rsMain.Update();
	}
	rsMain.Fin();
// midori UC_0053 add -->
	// 勘定科目内訳書（非連動マスターの場合
	if(((CUCHIWAKEApp*)AfxGetApp())->m_swUtwTandoku == 1 ) {
		// zvolumeテーブルの確定サインを更新する
		if (((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub->VolumeOpen() != 0) {
			// エラー
			clsFunc.ZmSubErrMsg( ((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub );
			return;
		}
		((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub->zvol->Edit();
		if(bkaku == TRUE)	((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub->zvol->tl_cor &= ~0x10;
		else				((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub->zvol->tl_cor |= 0x10;
		((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub->zvol->Update();
		((CUCHIWAKEApp*)AfxGetApp())->m_pZmSub->VolumeClose();
	}
// midori UC_0053 add <--
	// 
	if(bkaku == TRUE)	frmCmn->m_DataKakutei = BIT_D2;
	else				frmCmn->m_DataKakutei = 0;
	// １ページ再表示
	frmCmn->virUpdateControlTblData();
	// ボタン操作
	frmCmn->CmnChangeButtonEnable(2);
	// フォーカスセット
	if(bkaku == FALSE)	{
// 修正No.158301 add -->
		if(Kt_rowsv != -1 || Kt_colmsv != -1)	{
			frmCmn->m_uInfo.intCurRow = Kt_rowsv;
			frmCmn->m_uInfo.intCurCol = Kt_colmsv;
			Kt_rowsv = -1;
			Kt_colmsv = -1;
		}
		if(frmCmn->m_uInfo.intCurCol < 0)	frmCmn->m_uInfo.intCurCol = 1;
// 修正No.158301 add <--

		intIndex = frmCmn->CmnGetControlIndex(frmCmn->m_uInfo.intCurRow,frmCmn->m_uInfo.intCurCol);
		frmCmn->m_pDiag->SetPosition(intIndex);
		frmCmn->m_uInfo.intCurFocus = 1;
	}
	else	{
// 修正No.158301 del -->
//// 修正No.157633_元号日付 add -->
//		if(rowsv != -1 || colmsv != -1)	{
//			frmCmn->m_uInfo.intCurRow = rowsv;
//			frmCmn->m_uInfo.intCurCol = colmsv;
//		}
//// 修正No.157633_元号日付 add <--
// 修正No.158301 del <--
		frmCmn->m_pPageDiag->SetPosition(1);
		frmCmn->m_uInfo.intCurFocus = 0;
	}
	//// 「入力確定中」画面表示/非表示
	//frmCmn->Kakutei_ShowHide(frmCmn->m_DataKakutei);
	// 画面右下に「入力確定」を表示する
// midori 152766 del -->
//	PostMessage( WM_USER_UPDATE_INDICATE, 0 );
// midori 152766 del <--
// midori 152766 add -->
	PostMessage( WM_USER_UPDATE_INDICATE, 1 );
// midori 152766 add <--

// 改良No.22-0811 add -->
	// メニューバー「財務連動」の有効無効切り替え
	frmCmn->CmnMenuEnableSwitch();
// 改良No.22-0811 add <--
}
// midori 152718 add <--
