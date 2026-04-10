// DnpInpDialog.cpp : 実装ファイル
//

#include "stdafx.h"

#include "resource.h"

#include "DInputSub.h"
#include "DnpBaseClass.h"

#include "DnpInpDialog.h"


// CDnpInpDialog ダイアログ


// Because of the above definitions of TRY...CATCH it is necessary to
//  explicitly delete exception objects at the catch site.

#define DELETE_EXCEPTION(e) do { if(e) { e->Delete(); } } while (0)
#define NO_CPP_EXCEPTION(expr)


static HHOOK	mouseHook = NULL;

extern
CDnpInpDialog*	gDnpdlg;

/****************************************************************
  WH_MOUSE hook procedure
 ****************************************************************/
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	MOUSEHOOKSTRUCT *pmh; pmh = (MOUSEHOOKSTRUCT *)lParam;

	if( nCode < 0 )  // do not process the message 
		return CallNextHookEx(mouseHook, nCode, wParam, lParam);

	if( gDnpdlg != NULL ) {
		if( wParam == WM_RBUTTONDOWN ) {
			if( pmh->wHitTestCode == HTCLIENT ) {
				if( gDnpdlg->MouseHookRbuttonJob(pmh) ) {
					return TRUE;
				}
			}
		}
		else if (wParam == WM_LBUTTONDOWN) {
			if (gDnpdlg->show_flg == 0) {
				return TRUE;
			}

		}
	}

	return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}

// マウスフック
int DnpInSetMouseHook()
{
	if( mouseHook == NULL ) {
		mouseHook = SetWindowsHookEx(WH_MOUSE, MouseProc, NULL, GetCurrentThreadId());
		if( mouseHook == NULL ) {
			DWORD err = GetLastError();
			return -1;
		}
	}

	return 0;
}

// マウスフック解除
int DnpInMouseUnhook()
{
	if( mouseHook != NULL ) {
		UnhookWindowsHookEx(mouseHook);
		mouseHook = NULL;
	}

	return 0;
}




IMPLEMENT_DYNAMIC(CDnpInpDialog, ICSDialog)

CDnpInpDialog::CDnpInpDialog(CWnd* pParent /*=NULL*/)
	: ICSDialog(CDnpInpDialog::IDD, pParent)
{
	m_base = NULL;
	modelessDomodal = 0;
	show_flg = 0;
}

CDnpInpDialog::~CDnpInpDialog()
{
	if( m_base != NULL ) {
		m_base->EndWork();
		delete m_base;
	}
}

void CDnpInpDialog::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSSELCTRL1, m_selfix8);
	DDX_Control(pDX, IDC_ICSSELCTRL2, m_selval32);
	DDX_Control(pDX, IDC_SELGUIDE, m_selGuid);
}


//モードレスダイアログとして作成
BOOL CDnpInpDialog::CreateDlg()
{
	if ( m_pInpPar == NULL )
		return FALSE;

	if (m_pInpPar->modeless == 0)
		return FALSE;

	if (m_base == NULL) {
		m_base = new CDnpBaseClass();
		m_base->m_pCallParent = m_pInpPar->parent;
		m_base->m_pWinApp = m_pInpPar->winApp;
		m_base->m_pZm = m_pInpPar->pZmsub;
		m_base->m_pSy = m_pInpPar->pSyohi;
		m_base->m_DataSeq = m_pInpPar->seq;
		m_base->show_flg = 0;//20230210
		m_base->m_pDENPPAR = m_pInpPar;
	}
	//作成時にOnInitDialog 呼ばれる
	return ICSDialog::Create(CDnpInpDialog::IDD, m_base->m_pCallParent);
}

void CDnpInpDialog::SetInputPar(DENP_INPUTPAR* para)
{
	m_pInpPar = para;

	m_base->m_pCallParent = para->parent;
	m_base->m_pWinApp = para->winApp;
	m_base->m_pZm = para->pZmsub;
	m_base->m_pSy = para->pSyohi;
	m_base->m_DataSeq = para->seq;

	m_base->m_pDENPPAR = m_pInpPar;
}



int CDnpInpDialog::ModelessDoModal()
{
	HWND hWndParent = m_base->m_pCallParent->m_hWnd;
	
//	AfxUnhookWindowCreate();
	BOOL bEnableParent = FALSE;
	CWnd* pMainWnd = NULL;
	BOOL bEnableMainWnd = FALSE;

	m_nModalResult = 0;
	m_nFlags |= WF_CONTINUEMODAL;
	
	if (hWndParent && hWndParent != ::GetDesktopWindow() && ::IsWindowEnabled(hWndParent))
	{
		::EnableWindow(hWndParent, FALSE);
		bEnableParent = TRUE;

		pMainWnd = m_base->m_pWinApp->GetMainWnd();
		if (pMainWnd && pMainWnd->IsFrameWnd() && pMainWnd->IsWindowEnabled())
		{
			//
			// We are hosted by non-MFC container
			// 
			pMainWnd->EnableWindow(FALSE);
			bEnableMainWnd = TRUE;
		}
	}
	this->EnableWindow(TRUE);

	//OnInitDialogで確保したメモリ関連を初回のみここで開放する(次のInitWorkでまた確保するため）
	if (modelessDomodal == 0) {
		//DB 関連の終了処理
		m_base->CloseWork();
	}

	int err_sgn = -1;
	if (m_base->InitWork(FALSE) == 0) {
		if (m_base->ChangeDnpyoInput() == 0) {
			::BlockInput(TRUE);
			ShowWindow(SW_SHOW);
			MoveDenpDialog();
			err_sgn = 0;
			::BlockInput(FALSE);
		}
	}
	if (err_sgn) {
		m_nFlags &= ~WF_CONTINUEMODAL;
	}

	TRY
	{
		// create modeless dialog
		if (m_nFlags & WF_CONTINUEMODAL)
		{
			// enter modal loop
			DWORD dwFlags = MLF_SHOWONIDLE;
			if (GetStyle() & DS_NOIDLEMSG)
				dwFlags |= MLF_NOIDLEMSG;

			DnpInSetMouseHook();
			SetTimer(1, 1500, NULL);//20230307

			VERIFY(RunModalLoop(dwFlags) == m_nModalResult);
		}
#if _MSC_VER >= 1900
		m_bClosedByEndDialog = FALSE;
#endif
	}
	CATCH_ALL(e)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed.\n");
		DELETE_EXCEPTION(e);
		m_nModalResult = -1;
	}
	END_CATCH_ALL
	modelessDomodal++;
	//モードレス起動時の終了処理
	m_base->ModelessCloseJob();

	DnpInMouseUnhook();

	this->EnableWindow(FALSE);
	ShowWindow(SW_HIDE);

	if (bEnableMainWnd)
		pMainWnd->EnableWindow(TRUE);
	if (bEnableParent)
		::EnableWindow(hWndParent, TRUE);
	if (hWndParent != NULL && ::GetActiveWindow() == m_hWnd)
		::SetActiveWindow(hWndParent);

	if (err_sgn)	m_nModalResult = -1;

	return m_nModalResult;
}


//20230307
void CDnpInpDialog::Set_show_flg(int setF)
{
	show_flg = setF;//20230210
	m_base->show_flg = setF;
}

void CDnpInpDialog::EndDialogFunc(int nResult)
{
	ASSERT(::IsWindow(m_hWnd));

#if _MSC_VER >= 1900
	m_bClosedByEndDialog = TRUE;
#endif

	if (m_nFlags & (WF_MODALLOOP | WF_CONTINUEMODAL))
		EndModalLoop(nResult);
	//モードレスで起動時はダイアログの破棄をしないため。
	if (m_pInpPar->modeless == 0) {
		::EndDialog(m_hWnd, nResult);
	}
}


void CDnpInpDialog::MoveDenpDialog()
{
	CRect   rcDlg;
	m_base->GetNowDnpDialog()->GetWindowRect(rcDlg);
	ScreenToClient(rcDlg);

	CRect   rcClient;
	rcClient.SetRect(0, 0, defWidth, defHeight);
		// ツールバーの高さの取得			defHeight =
	CSize   sizeToolBar;
	sizeToolBar = m_wndToolBar.CalcFixedLayout(TRUE, TRUE);
	int ht = rcClient.Height();
	rcClient.bottom = rcDlg.bottom + (sizeToolBar.cy * 2);

	CRect	rcParent;
	m_pInpPar->parent->GetWindowRect(rcParent);

	int scrX = GetSystemMetrics(SM_CXFULLSCREEN);
	int left = rcParent.left;
	int wd = rcClient.Width();
	int scrY = GetSystemMetrics(SM_CYFULLSCREEN);
	ht = rcClient.Height();

	if (rcParent.right <= scrX || (left <= scrX && rcParent.right > scrX)) {
		if ((left + wd) > scrX) {
			int ntmp = (left + wd) - scrX;
			left -= ntmp;
		}
	}
	CRect rcFrm;
	m_pInpPar->winApp->m_pMainWnd->GetWindowRect(rcFrm);

	int nTop = rcParent.top;
	int dispBottom = nTop + ht;

	if (dispBottom > rcFrm.bottom) {
		dispBottom = rcFrm.bottom;
		nTop = (dispBottom - ht);
	}

	MoveWindow(left, nTop, defWidth,defHeight);
}


BEGIN_MESSAGE_MAP(CDnpInpDialog, ICSDialog)
	ON_MESSAGE( WM_RETROFOCUS, OnRetroFocus )
	ON_MESSAGE( WM_RETROFOCUS2, OnRetroFocus2 )
	ON_MESSAGE( WM_SKBN_INEND, OnSkbnEnd )
	ON_MESSAGE( WM_SKBN_ICHIRN, DspSkbnIchiran)
	ON_MESSAGE( WM_SKBN_LINEDISP, SetSkbnLineDisp )

	ON_MESSAGE( WM_HJIYU_INEND, OnHjiyuInEnd )
	ON_MESSAGE( WM_HJIYU_SEL, OnHjiyuSel )
	ON_MESSAGE( WM_HSINP_INEND, OnHusenInpMsg )
	ON_MESSAGE( WM_SELGUIDEMSG, OnSelGuideMsg)
	ON_MESSAGE( WM_KICKIDLE, OnKickIdle)

	ON_COMMAND_RANGE( ID_TLBAR_F1, ID_TLBAR_ESC, OnCommandTlbar )
	ON_COMMAND(ID_BUTTON_END, OnButtonEnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()




// ツールバー情報の構造体
struct	stToolBarInfo
{
    int     btnId;      // ボタンのID
    int     imgIdx;     // ボタンにつけるイメージのインデックス
    int     strId;      // ボタンに表示する文字列ID
};


#define DNP_TBAR_BUTTON_NUM             2

const stToolBarInfo cstTBarInfo[DNP_TBAR_BUTTON_NUM] = {
	{ID_TLBAR_ESC,0,ID_TLBAR_ESC},
	{ID_TLBAR_END,0,ID_TLBAR_END},
};



// CDnpInpDialog メッセージ ハンドラ
BOOL CDnpInpDialog::OnInitDialog()
{
	ICSDialog::OnInitDialog();
	// TODO:  ここに初期化を追加してください

	ICSDialog::OnInitDialogEX();

	CRect   rcClient;
	GetClientRect(rcClient);
	defWidth = rcClient.Width();
	defHeight = rcClient.Height();

	// ツールバーの作成
	BOOL bRet = m_wndToolBar.Create(this/*,WS_CHILD | WS_VISIBLE | CBRS_BORDER_ANY, IDR_TOOLBAR1*/);
	if(bRet == FALSE)
		return -1;
	bRet = m_wndToolBar.LoadToolBar(IDR_TOOLBAR1);

#ifdef CLOSE
	// テキスト
	// ボタンの設定(後でSetButtonInfo()で設定するので数だけ設定)
	m_wndToolBar.SetButtons(NULL,DNP_TBAR_BUTTON_NUM);
	// ボタンのスタイル
	// 各ボタンの設定
	for(int i=0;i<DNP_TBAR_BUTTON_NUM;i++){
		// ボタンのイメージのセット
		m_wndToolBar.SetButtonInfo(i,cstTBarInfo[i].btnId,TBBS_BUTTON,cstTBarInfo[i].imgIdx);
		// ボタン文字列の設定
	}
#endif
	//DoModal で作成
	if (m_pInpPar->modeless == 0) {
		m_base = new CDnpBaseClass();

		m_base->m_pCallParent = m_pInpPar->parent;
		m_base->m_pWinApp = m_pInpPar->winApp;
		m_base->m_pZm = m_pInpPar->pZmsub;
		m_base->m_pSy = m_pInpPar->pSyohi;
		m_base->m_DataSeq = m_pInpPar->seq;

		m_base->m_pDENPPAR = m_pInpPar;
	}
	m_base->m_pDnpParent = this;
	m_base->m_pselFix8 = &m_selfix8;
	m_base->m_pselSel32 = &m_selval32;

	if( m_base->InitWork( TRUE ) != 0 ) {

	}

	// WINDOWS VISTA では、これをしないと、取消線の描画が、できなかった。
	ModifyStyle(WS_CLIPCHILDREN, 0);

	if (m_pInpPar->modeless == 0) {
		if (m_base->ChangeDnpyoInput() != 0) {
			EndDialog(IDCANCEL);
			return TRUE;
		}
	}
	else {
		m_base->ChangeKind(KIND_FURIKAE);
	}

	CRect   rcDlg;
	m_base->GetNowDnpDialog()->GetWindowRect(rcDlg);
	ScreenToClient(rcDlg);

	 // ツールバーのリサイズ処理
	// ダイアログのrect
	GetClientRect(rcClient);
	// ツールバーの高さの取得
	CSize   sizeToolBar;
	sizeToolBar = m_wndToolBar.CalcFixedLayout(TRUE,TRUE);
	// 作成時は大きさが0なので設定する
	m_wndToolBar.MoveWindow(0, rcDlg.bottom, rcClient.Width(), sizeToolBar.cy);
	defHeight += sizeToolBar.cy;

	MoveDenpDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}



LRESULT CDnpInpDialog::OnRetroFocus( WPARAM wParam, LPARAM lParam )
{
//FILETrace( "@CDnpInView::OnRetroFocus %d,%d\n", wParam, lParam );
	m_base->GetNowDnpDialog()->set_focus( wParam, lParam );

	return 1;
}

// 指定IDのウィンドウにフォーカスセット
LRESULT CDnpInpDialog::OnRetroFocus2( WPARAM wParam, LPARAM lParam )
{
	CWnd* pwnd;
	pwnd = m_base->GetNowDnpDialog()->GetDlgItem( wParam );
	
	m_base->GetNowDnpDialog()->GotoDlgCtrl( pwnd );
	return 1;
}


//　消費税区分選択番号
LRESULT CDnpInpDialog::SetSkbnLineDisp( WPARAM wParam, LPARAM lParam ) 
{
TRACE( "selno = %d\n", wParam );
	m_base->SetSkbnLineDisp();

	return 1;
}



//キーで終わった場合 CSkbnIn からのメッセージ
LRESULT CDnpInpDialog::OnSkbnEnd( WPARAM wParam, LPARAM lParam ) 
{
	m_base->TerminationSyz( wParam );

	return 1;
}


// 消費税区分一覧表示
LRESULT CDnpInpDialog::DspSkbnIchiran( WPARAM wParam, LPARAM lParam )
{
	m_base->DispSyzIchiran( wParam, lParam );
	return TRUE;
}



LRESULT CDnpInpDialog::OnHjiyuInEnd( WPARAM w, LPARAM l )
{

	m_base->OnTermHjiyu( m_base->get_nowln(), w );

	return 1;
}


LRESULT CDnpInpDialog::OnHjiyuSel( WPARAM w, LPARAM l )
{
	m_base->OnHjiyuSel( w, l );

	return 1;
}


// 付箋入力ダイアログからのメッセージ処理
LRESULT CDnpInpDialog::OnHusenInpMsg( WPARAM w, LPARAM l )
{
	m_base->OnHusenInpMsg( w, l );

	return 1;
}


LRESULT CDnpInpDialog::OnSelGuideMsg(WPARAM w, LPARAM l )
{
	if( w == VK_NEXT || w == VK_PRIOR ) {
		keybd_event( w, 0, 0, 0 );
		keybd_event( w, 0, KEYEVENTF_KEYUP, 0 );
	}

	return 1;
}


void CDnpInpDialog::OnCommandTlbar( UINT id )
{
	int st;

	if( id == ID_TLBAR_END ) {
		if( m_base != NULL ) {
			st = m_base->EndScan();
		}
		int endsw = IDOK;
		if( st == -2 ) {
			endsw = st;
		}
		else if( st == -1 ) //メッセージで修正する。
			return;

		EndDialog(endsw);
	}
	else if( id == ID_TLBAR_ESC ) {
		EndDialog(IDCANCEL);
	}
	else if( id == ID_TLBAR_F3 ) {
		if( m_base != NULL ) {
			if( m_base->ToolF3Job() == 0 ) {
				EndDialog(IDOK);
			}
		}
	}
	else if( id == ID_TLBAR_F4 ) {
		if( m_base != NULL ) {
			m_base->ToolF4Job();
		}
	}
	else if( id == ID_TLBAR_F12 ) {
		if( m_base != NULL ) {
			m_base->ToolF12Job();
		}
	}
	else if( id == ID_TLBAR_INS ) {
		if( m_base != NULL ) {
			m_base->ToolINSJob();
		}
	}
	else if( id == ID_TLBAR_DEL ) {
		if( m_base != NULL ) {
			m_base->ToolDELJob();
		}
	}
	else if( id == ID_TLBAR_F7 ) {
		if( m_base != NULL ) {
			m_base->ToolF7Job();
		}
	}
}


BEGIN_EVENTSINK_MAP(CDnpInpDialog, ICSDialog)
	ON_EVENT(CDnpInpDialog, IDC_ICSSELCTRL1, 1, CDnpInpDialog::SelectedIcsselctrl1, VTS_I2 VTS_I4)
	ON_EVENT(CDnpInpDialog, IDC_ICSSELCTRL2, 1, CDnpInpDialog::SelectedIcsselctrl2, VTS_I2 VTS_I4)
END_EVENTSINK_MAP()

void CDnpInpDialog::SelectedIcsselctrl1(short position, long Number)
{
	m_base->SelectedIcsselctrl( position, Number, 0 );
}

void CDnpInpDialog::SelectedIcsselctrl2(short position, long Number)
{
	m_base->SelectedIcsselctrl( position, Number, 1 );
}


LRESULT CDnpInpDialog::OnKickIdle(WPARAM wParam, LPARAM lParam)
{
	ToolBarUpdate();
	return FALSE;
}


void CDnpInpDialog::ToolBarUpdate()
{
	if( m_base == NULL )
		return;

	CToolBarCtrl& tctrl = TBarCtrl();

	BOOL bEnable;

	int nID[] = {
		ID_TLBAR_F3, ID_TLBAR_F4, ID_TLBAR_F12, ID_TLBAR_INS, ID_TLBAR_DEL, ID_TLBAR_F7, 0
	};

	for( int i = 0; nID[i] != 0; i++ ) {
		bEnable = m_base->IsEnableToolBar( nID[i] );
		tctrl.EnableButton(nID[i], bEnable);
	}
}



BOOL CDnpInpDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( m_base != NULL ) {
		if( m_base->m_PRO_OK ) {
			m_base->Lineattr_dsp();

			if( pMsg->message == WM_LBUTTONDOWN ) {
				m_base->CancelInvNoIME();
			}
		}
	}

	if( pMsg->message == WM_KEYDOWN ) {
		switch( pMsg->wParam ) {
		case VK_F3:
			OnCommandTlbar( ID_TLBAR_F3 );
			break;
		case VK_F4:
			OnCommandTlbar( ID_TLBAR_F4 );
			break;
		case VK_F7:
			OnCommandTlbar( ID_TLBAR_F7 );
			break;
		case VK_F12:
			OnCommandTlbar( ID_TLBAR_F12 );
			break;
		case VK_END:
			OnCommandTlbar( ID_TLBAR_END );
			break;
		}
	}

	//20230307
	if ( pMsg->message == WM_LBUTTONDOWN) {
		if (show_flg == 0) {
			return TRUE;
		}
	}

	//if (pMsg->message == WM_TIMER) {	//20230307
	//	Set_show_flg(1);
	//	KillTimer(1);
	//	return TRUE;
	//}

	return ICSDialog::PreTranslateMessage(pMsg);
}


void CDnpInpDialog::OnButtonEnd()
{
	OnCommandTlbar(ID_TLBAR_END);
}


BOOL CDnpInpDialog::MouseHookRbuttonJob(MOUSEHOOKSTRUCT *pmh)
{
	return m_base->MouseHookRbuttonJob(pmh);
}


void CDnpInpDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	if( nIDEvent == 1 )
	{
		Set_show_flg(1);
		KillTimer(1);
	}

	CWnd::OnTimer(nIDEvent);
}
