// TkregDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "resource.h"

#include "DBInpSubFunc.h"
#include "TkregDlg.h"
#include "swkseldlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTkregDlg ダイアログ


CTkregDlg::CTkregDlg(CWnd* pParent /*=NULL*/)
	: CICSTbarDlg(CTkregDlg::IDD, /*IDR_TOOLBAR1*/0, 1, pParent)
{
	//{{AFX_DATA_INIT(CTkregDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT

	m_NOWIDX = -1;
	m_bFirst = FALSE;
	
	::ZeroMemory( m_kanabf, sizeof m_kanabf );
	::ZeroMemory(m_svInvno, sizeof m_svInvno);
	m_svMenchk = 0;

	clear_TKYV2_SAVE(&m_Save);
}

void CTkregDlg::InitDlg( CDBZmSub* pZm, CDBSyohi* pSy )
{
	m_pZm = pZm;
	m_pSy = pSy;
}

void CTkregDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTkregDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}


BEGIN_MESSAGE_MAP(CTkregDlg, CICSTbarDlg)
	//{{AFX_MSG_MAP(CTkregDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP

	ON_MESSAGE(WM_MYTABCHANGE, OnMyTabChange)
	ON_MESSAGE(WM_TEKIADDJOB, OnTekiAddJob)
	ON_COMMAND_RANGE( ID_TLBAR_F1, ID_TLBAR_END, OnCommandTlbar )

//	ON_MESSAGE( WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
//	ON_UPDATE_COMMAND_UI_RANGE( ID_TLBAR_F1, ID_TLBAR_END, OnUpdateCommandTlbar )
//	ON_MESSAGE( WM_KICKIDLE, OnKickIdle )

	ON_BN_CLICKED(IDC_BTN_F8, &CTkregDlg::OnBnClickedBtnF8)
	ON_BN_CLICKED(IDC_BTN_F9, &CTkregDlg::OnBnClickedBtnF9)
	ON_BN_CLICKED(IDC_BTN_END, &CTkregDlg::OnBnClickedBtnEnd)
	ON_BN_CLICKED(IDC_BTN_F3, &CTkregDlg::OnBnClickedBtnF3)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTkregDlg メッセージ ハンドラ

void CTkregDlg::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
#ifdef _CTABCTRL
	int sel = m_Tab.GetCurSel();
#else
	int sel = m_Tab.GetCurSel();
#endif
	TabChg( sel );

	*pResult = 0;
}

// タブ切り替え
//	int index ..... 表示画面インデックス
//	BOOL bAdd ..... 追加時の 切り替えか？
//
void CTkregDlg::TabChg( int index, BOOL bAdd /*=FALSE*/ )
{

	int prev_idx;

	prev_idx = m_NOWIDX;

	if( index == prev_idx )		return;

	if( index == 2 ) {
		GetDlgItem( IDC_BTN_F9 )->EnableWindow(TRUE);
	}
	else {
		GetDlgItem( IDC_BTN_F9 )->EnableWindow(FALSE);
	}

	CBaseDlg* pBase;

	if( prev_idx != -1 ) {
		pBase = m_TabArray.GetAt( prev_idx );
		pBase->WndShow( FALSE );
	}

//	m_Tab.SetSelectedItem( index );
	pBase = m_TabArray.GetAt( index );

	if( !bAdd ) {
		pBase->WndShow( TRUE );
		pBase->SetFirstPosition();
	}
	else
		pBase->WndShow_Add( TRUE );

	m_NOWIDX = index;

}


// タブのダイアログ 位置調整
void CTkregDlg::TabDlgMove()
{
	CBaseDlg* pBase;
#ifdef _CTABCTRL
	int index = m_Tab.GetCurSel();
#else
	int index = m_Tab.GetCurSel();
#endif

ASSERT(index >= 0);
	pBase = m_TabArray.GetAt( index );

	double x, y;
	x = y = 1.0;
//	GetRate( x, y );
	pBase->DlgWndMove( x, y );
}


BOOL CTkregDlg::OnInitDialog() 
{
//	ControlInit( 495, 280, -1 );

	IsKaniJob() = FALSE;

	CICSTbarDlg::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	TC_ITEM tc = {0};
	char buf[100];
	short set_tab = 0;
	int max = 3;
//	int max = 1;

	CSWKselDlg::bSwkDlgShow = FALSE;

	// タブの中身作成
	try {
		m_pDicdlg = new CTkdicDlg(&m_Tab);
		m_pKmkdlg = new CTkkmkDlg(&m_Tab);
		m_TabArray.Add( (CBaseDlg*)m_pDicdlg );
		m_TabArray.Add( (CBaseDlg*)m_pKmkdlg );
		m_pSwkdlg = new CTkswkDlg(&m_Tab);
		m_TabArray.Add( (CBaseDlg*)m_pSwkdlg );
	}
	catch( CMemoryException* e )
	{
		e->Delete();
		EndDialog(-1);
		return TRUE;
	}

	m_pDicdlg->Set_Class( m_pZm, m_pSy );
	memcpy( m_pDicdlg->m_Kana, m_kanabf, sizeof m_pDicdlg->m_Kana );
	m_pDicdlg->m_svTky = m_svTky;
	memcpy( m_pDicdlg->m_TkyInvno, m_svInvno, sizeof m_pDicdlg->m_TkyInvno);
	m_pDicdlg->m_Menchk = m_svMenchk;
	m_pDicdlg->CreateDlg();

	m_pKmkdlg->Set_Class( m_pZm, m_pSy  );
	m_pKmkdlg->CreateDlg();

	m_pSwkdlg->Set_Class( m_pZm, m_pSy  );
	m_pSwkdlg->CreateDlg();


#ifdef _CTABCLRL
	for( int n = 0; n < max; n++ ) {
		tc.mask = TCIF_TEXT;
		tc.pszText = buf;
		tc.cchTextMax = sizeof buf;
		tc.iImage = -1;
		tc.lParam = 0;

		switch( n ) {
		case 0:	strcpy_s(buf, sizeof buf, " 摘要辞書登録 " );
				break;
		case 1:	strcpy_s(buf, sizeof buf, " 科目対応摘要登録 " );
				break;
		case 2:	strcpy_s(buf, sizeof buf, " 仕訳対応摘要登録 " );
				break;
		}

		m_Tab.InsertItem(n, &tc);
	}
#else
	HWND hwnd;
	for( int n = 0; n < max; n++ ) {
		switch( n ) {
		case 0:	strcpy_s(buf, sizeof buf, " 摘要辞書登録 " );
				hwnd = m_pDicdlg->m_hWnd;
				break;
		case 1:	strcpy_s(buf, sizeof buf, " 科目対応摘要登録 " );
				hwnd = m_pKmkdlg->m_hWnd;
				break;
		case 2:	strcpy_s(buf, sizeof buf, " 仕訳対応摘要登録 " );
				hwnd = m_pSwkdlg->m_hWnd;
				break;
		}

		m_Tab.InsertItem(n, buf, hwnd);
	}

	m_Tab.SetAppearance( ICS_TABCTRL_VSTUDIO2005 );
	m_Tab.SetOneNoteColors( TRUE );
	m_Tab.SetColorSet( ICS_TABCTRL_CS_WINXP );
	m_Tab.BoldSelected( TRUE );

#endif


	// 各ダイアログの 財務クラスオープン後の初期化処理
	int num = m_TabArray.GetSize();
	CBaseDlg* pBase;
	double x, y;
	x = y = 1.0;
//	GetRate( x, y );
	for( int i = 0; i < num; i++ ) {
		pBase = m_TabArray.GetAt( i );
		pBase->InitWork( x, y );
	}

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}




// ユーザーメッセージ
LRESULT CTkregDlg::OnMyTabChange(WPARAM wParam, LPARAM lParam)
{

#ifdef _CTABCTRL
	if( wParam == 0 )		// タブ 表示変更
		TabChg( m_Tab.GetCurSel() );
	else if( wParam == 1 )
		TabDlgMove();		// タブ 内部ダイアログ移動
	else if( wParam == 2 ) {
#else
	if( wParam == 2 ) {
#endif
		// タブの最初のコントロールにフォーカスセット
		CBaseDlg* pBase;
ASSERT(m_NOWIDX >= 0);
		pBase = m_TabArray.GetAt( m_NOWIDX );
		pBase->SetFirstPosition();
	}

	return 0;
}

void CTkregDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CICSTbarDlg::OnShowWindow(bShow, nStatus);
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	if( bShow == TRUE && ! m_bFirst ) {
		m_bFirst = TRUE;

		TKY_REGPAR* par;
		par = get_tkyregpar();

#ifdef _CTABCTRL
		m_Tab.SetCurSel( par->tr_jobtyp );
#else
		m_Tab.SetSelectedItem( par->tr_jobtyp );
#endif
		PostMessage( WM_MYTABCHANGE, 0, 0 );
	}	
}


// 摘要を他の対応摘要に追加する処理
LRESULT CTkregDlg::OnTekiAddJob(WPARAM wParam, LPARAM lParam)	
{
	TK_ADD* pAdd = (TK_ADD*)lParam;

#ifdef _CTABCTRL
	m_Tab.SetCurSel( pAdd->ta_typ );
	// 追加用 画面切り替え
	TabChg( m_Tab.GetCurSel(), TRUE );
#else
	m_Tab.SetSelectedItem( pAdd->ta_typ );
#endif

	CBaseDlg* pBase;
ASSERT(m_NOWIDX >= 0);
	pBase = m_TabArray.GetAt( m_NOWIDX );

	// 各画面での追加 登録処理
	pBase->AddTekiyoJob( lParam );

	return 0;
}


// ツールバーより
void CTkregDlg::OnCommandTlbar( UINT id )
{
	switch( id ) {
	case ID_TLBAR_F1:
	case ID_TLBAR_F2:
	case ID_TLBAR_F3:
	case ID_TLBAR_F4:
	case ID_TLBAR_F5:
	case ID_TLBAR_F6:
	case ID_TLBAR_F7:
	case ID_TLBAR_F8:
	case ID_TLBAR_F9:
	case ID_TLBAR_F11:
	case ID_TLBAR_F12:
		if( m_NOWIDX != -1 ) {
			CBaseDlg* pBase;
			pBase = m_TabArray.GetAt( m_NOWIDX );
			pBase->SendMessage( WM_USER_1, WM_COMMAND, id );
			return;
		}
		break;
	case ID_TLBAR_END:
		EndDialog( IDOK );
		break;
	}
}



LRESULT CTkregDlg::OnKickIdle( WPARAM wParam, LPARAM lParam )
{

//TRACE( "******************* CTkregDlg::OnKickIdle\n" );
#ifdef CLOSE

	CBaseDlg* pBase;

	if( m_NOWIDX >= 0 ) {
		pBase = m_TabArray.GetAt( m_NOWIDX );
		BOOL bEnable;

		bEnable = pBase->IsEnableToolBar( ID_TLBAR_F8 );

		GetDlgItem( IDC_BTN_F8 )->EnableWindow( bEnable );
	}
	else {
		GetDlgItem( IDC_BTN_F8 )->EnableWindow( FALSE );
	}

#endif

	return 1;
}

#ifdef CLOSE

void CTkregDlg::OnUpdateCommandTlbar( CCmdUI* pCmdUI )
{
TRACE( "OnUpdateCommandTlbar\n" );
}

LRESULT CTkregDlg::OnIdleUpdateCmdUI( WPARAM wParam, LPARAM lParam )
{
	return 1;
}

#endif



int CTkregDlg::ButttonClickByMouseEvent(UINT id)
{
	CPoint nowPt;
	::GetCursorPos(&nowPt);
	CRect rect;
	CWnd* pwnd;
	pwnd = GetDlgItem(id);

	//	GetDlgItem(id)->GetWindowRect(&rect);
	int x = (rect.left + rect.Width() / 2);
	int y = (rect.top + rect.Height() / 2);
	//	'実際にマウスを移動させる為の補正値を求める
	//	'画面の解像度を取得
	int smx, smy, posx, posy;
	//	'画面解像度の取得
	smx = GetSystemMetrics(SM_CXSCREEN);
	smy = GetSystemMetrics(SM_CYSCREEN);
	//	'マウスの移動量を計算(絶対位置)
	posx = x * (65535 / smx);
	posy = y * (65535 / smy);
	mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, posx, posy, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	//元の位置に戻す
	posx = nowPt.x * (65535 / smx);
	posy = nowPt.y * (65535 / smy);
	mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, posx, posy, 0, 0);
	return 1;
}


BOOL CTkregDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

	if( pMsg->message == WM_KEYDOWN ) {

		switch( pMsg->wParam ) {
		case VK_F8:
			if( GetDlgItem( IDC_BTN_F8 )->IsWindowEnabled() ) {
				PostMessage( WM_COMMAND, MAKELONG( IDC_BTN_F8, BN_CLICKED ),
								(LPARAM)GetDlgItem( IDC_BTN_F8 )->m_hWnd );
			}
			break;
		case VK_F9:
			if( GetDlgItem( IDC_BTN_F9 )->IsWindowEnabled() ) {
				PostMessage( WM_COMMAND, MAKELONG( IDC_BTN_F9, BN_CLICKED ),
								(LPARAM)GetDlgItem( IDC_BTN_F9 )->m_hWnd );
			}
			break;
		case VK_F3:
			if( GetDlgItem( IDC_BTN_F3 )->IsWindowEnabled() ) {
				PostMessage( WM_COMMAND, MAKELONG( IDC_BTN_F3, BN_CLICKED ),
								(LPARAM)GetDlgItem( IDC_BTN_F3 )->m_hWnd );
			//	ButttonClickByMouseEvent(IDC_BTN_F3);
			}
			break;

		case VK_RETURN:
			if( m_NOWIDX != -1 ) {
				CBaseDlg* pBase;
				pBase = m_TabArray.GetAt( m_NOWIDX );
				pBase->SetFirstPosition();
				return 1;
			}
			break;
		case VK_END:
			PostMessage( WM_COMMAND, MAKELONG( IDC_BTN_END, BN_CLICKED ),
							(LPARAM)GetDlgItem( IDC_BTN_END )->m_hWnd );
			break;
		}
	}

#ifdef CLOSE
	if( pMsg->message == WM_COMMAND ) {
		switch( pMsg->wParam ) {
		case ID_TLBAR_F2:  
		case ID_TLBAR_F1:
		case ID_TLBAR_F4:    
		case ID_TLBAR_F6:    
		case ID_TLBAR_F7:    
		case ID_TLBAR_F8:    
		case ID_TLBAR_F9:    
		case ID_TLBAR_F11:    
		case ID_TLBAR_F12:   
		//case ID_TLBAR_END:   
		case ID_TLBAR_F3:   
		case ID_TLBAR_F5:    
			if( m_NOWIDX != -1 ) {
				CBaseDlg* pBase;
				pBase = m_TabArray.GetAt( m_NOWIDX );
				pBase->SendMessage( WM_USER_1, WM_COMMAND, pMsg->wParam );
				return 1;
			}
			break;
		default:
			break;
		}
	}
#endif

	return CICSTbarDlg::PreTranslateMessage(pMsg);
}


void CTkregDlg::OnBnClickedBtnF8()
{
	if( m_NOWIDX != -1 ) {
		if( m_NOWIDX == 0 ) {
			if( m_pDicdlg ) {
				if( !m_pDicdlg->CheckInvoiceNo() )
					return;
			}
		}
		CBaseDlg* pBase;
		pBase = m_TabArray.GetAt( m_NOWIDX );
		pBase->SendMessage( WM_USER_1, WM_COMMAND, ID_TLBAR_F8 );
	}
}

void CTkregDlg::OnBnClickedBtnF9()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if( m_NOWIDX != -1 ) {
		CBaseDlg* pBase;
		pBase = m_TabArray.GetAt( m_NOWIDX );
		pBase->SendMessage( WM_USER_1, WM_COMMAND, ID_TLBAR_F9 );
	}
}


void CTkregDlg::OnBnClickedBtnEnd()
{
	// 仕訳摘要選択 ダイアログ出ないように
	CSWKselDlg::bSwkDlgShow = TRUE;

	GetDBupdateSgn();
	strec_searchjob();

	EndDialog( IDOK );
}


void CTkregDlg::OnCancel()
{
	// 仕訳摘要選択 ダイアログ出ないように
	CSWKselDlg::bSwkDlgShow = TRUE;

	GetDBupdateSgn();

	CICSTbarDlg::OnCancel();
}


// データベースが更新されたか？
void CTkregDlg::GetDBupdateSgn()
{
	CBaseDlg* pBase;
	m_bUpdate = FALSE;

	for( int i = 0; i < m_TabArray.GetCount(); i++ ) {
		pBase = m_TabArray.GetAt(i);
		pBase->LastDataGet();

		if( pBase->IsDataBaseUpdated() ) {
			m_bUpdate = TRUE;
			break;
		}
	}
}



void CTkregDlg::OnBnClickedBtnF3()
{
	if( m_pDicdlg ) {
		if( !m_pDicdlg->CheckInvoiceNo() )
			return;
	}
	IsKaniJob() = TRUE;
	strec_searchjob();
	//摘要辞書の情報取得
	SaveTkyData(&m_Save, TRUE);

	EndDialog(IDOK);
}


// 仕訳摘要画面作成で、strec がクエリーされているのを戻す。
void CTkregDlg::strec_searchjob()
{
	TKY_REGPAR par = {0};
	memcpy( &par, get_tkyregpar(), sizeof TKY_REGPAR );

	CString dbt, cre;
	if( par.tr_debt[0] )	dbt = par.tr_debt;
	if( par.tr_cred[0] )	cre = par.tr_cred;
	swktky_search( 0, dbt, cre, m_pZm );
}



// 登録内容の保存
void CTkregDlg::SaveTkyData(TKYV2_SAVE* psv, BOOL bClear/*=FALSE*/)
{
	if( bClear ) {
		clear_TKYV2_SAVE(psv);
	}

	if( m_pDicdlg ) {
		m_pDicdlg->SaveTkyData(psv, bClear);
	}
}
