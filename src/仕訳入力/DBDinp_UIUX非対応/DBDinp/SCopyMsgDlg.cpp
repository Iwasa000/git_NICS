// SCopyMsgDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBDinp.h"
#include "SCopyMsgDlg.h"


// CSCopyMsgDlg ダイアログ

IMPLEMENT_DYNAMIC(CSCopyMsgDlg, ICSDialog)

CSCopyMsgDlg::CSCopyMsgDlg(LPCTSTR ermsg, CWnd* pParent /*=NULL*/)
	: ICSDialog(CSCopyMsgDlg::IDD, pParent)
{
	m_bCancel=FALSE;
	m_bParentDisabled = FALSE;

	m_strOK = _T( "OK" );
	m_strCANCEL = _T( "ｷｬﾝｾﾙ" );
	m_btnSW[0] = m_btnSW[1] = SW_SHOW;

	m_bMsgMode = TRUE;
	m_strMsg = ermsg;
}



CSCopyMsgDlg::CSCopyMsgDlg()
{
	m_bCancel=FALSE;
	m_bParentDisabled = FALSE;

	m_strOK = _T( "OK" );
	m_strCANCEL = _T( "ｷｬﾝｾﾙ" );
	m_btnSW[0] = m_btnSW[1] = SW_SHOW;

	m_bMsgMode = FALSE;
}


CSCopyMsgDlg::~CSCopyMsgDlg()
{
	if(m_hWnd!=NULL)
		DestroyWindow();
}


BOOL CSCopyMsgDlg::DestroyWindow()
{
	ReEnableParent();
	return CDialog::DestroyWindow();
}


void CSCopyMsgDlg::ReEnableParent()
{
	if(m_bParentDisabled && (m_pParentWnd!=NULL))
		m_pParentWnd->EnableWindow(TRUE);
	m_bParentDisabled=FALSE;
}

BOOL CSCopyMsgDlg::Create(CWnd *pParent)
{
	// ダイアログの実際の親ウィンドウを取得します。
	m_pParentWnd = CWnd::GetSafeOwner(pParent);

	// m_bParentDisabled は、このダイアログが破棄された時に、親ウィンドウを
	// 再び有効にするために使用します。従って、この時点で親ウィンドウがすでに
	// 有効な場合のみ、この変数に TRUE を設定します。

	if((m_pParentWnd!=NULL) && m_pParentWnd->IsWindowEnabled())
	{
		m_pParentWnd->EnableWindow(FALSE);	// 親窓 無効
		m_bParentDisabled = TRUE;
	}

	if(!ICSDialog::Create(CSCopyMsgDlg::IDD,pParent))
	{
		ReEnableParent();
		return FALSE;
	}

	return TRUE;
}


void CSCopyMsgDlg::OnCancel()
{
	m_bCancel=TRUE;

	if( m_bMsgMode ) {
		EndDialog( IDCANCEL );
	}
}

void CSCopyMsgDlg::OnOK()
{
	if( m_bMsgMode ) {
		EndDialog( IDOK );
	}
}


void CSCopyMsgDlg::PumpMessages()
{
	// ダイアログを使用する前に Create() を呼んでください。
	ASSERT(m_hWnd!=NULL);

	MSG msg;
	// ダイアログ メッセージの処理
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if(!IsDialogMessage(&msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}


BOOL CSCopyMsgDlg::CheckCancelButton()
{
	// ペンディング中のメッセージの処理
	PumpMessages();

	// m_bCancel を FALSE に戻すことにより
	// ユーザがキャンセル ボタンを押すまで、CheckCancelButton は
	// FALSE を返します。このため、CheckCancelButton を呼び出した
	// 後にオペレーションを継続することが可能になります。
	// もし m_bCancel が TRUE に設定されたままだと、次回以降の
	// CheckCancelButton の呼び出しは常に TRUE を返してしまいます。

	BOOL bResult = m_bCancel;
	m_bCancel = FALSE;

	return bResult;
}



void CSCopyMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_RECCNT, m_PrgRcnt);
}



BEGIN_MESSAGE_MAP(CSCopyMsgDlg, ICSDialog)
END_MESSAGE_MAP()


// CSCopyMsgDlg メッセージ ハンドラ

BOOL CSCopyMsgDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	GetDlgItem(IDOK)->SetWindowText( m_strOK );
	GetDlgItem(IDCANCEL)->SetWindowText( m_strCANCEL );
	GetDlgItem(IDC_STATIC_MESSAGE_1)->SetWindowText( m_strMsg );

	ICSDialog::OnInitDialogEX();

	if( m_btnSW[0] == SW_HIDE ) {
		GetDlgItem(IDOK)->ShowWindow( SW_HIDE );
		MoveCenterWindow( IDCANCEL, FALSE );
	}
	if( m_btnSW[1] == SW_HIDE ) {
		GetDlgItem(IDCANCEL)->ShowWindow( SW_HIDE );
		MoveCenterWindow( IDOK, FALSE );
	}

	if( m_bMsgMode ) {
		GetDlgItem(IDC_PROGRESS_RECCNT)->ShowWindow( SW_HIDE );
	}

	CenterWindow();
	ShowWindow( SW_SHOW );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CSCopyMsgDlg::MoveCenterWindow( int nID, BOOL bRepaint)
{
	CRect	nDrct;
	CRect	nCrct;

	CWnd* pWnd = GetDlgItem( nID );

	GetWindowRect(&nDrct);
	pWnd->GetWindowRect(&nCrct);
	ScreenToClient(&nDrct);
	ScreenToClient(&nCrct);

	CRect	nTrct(&nCrct);
	LPRECT(nTrct)->left = LPRECT(nDrct)->left + ((nDrct.Width() - nCrct.Width()) / 2);
	LPRECT(nTrct)->right = LPRECT(nTrct)->left + nCrct.Width();

	// pWnd->ShowWindow(SW_HIDE);
	pWnd->MoveWindow(&nTrct, bRepaint);
	if (bRepaint == TRUE)
		pWnd->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
}


void CSCopyMsgDlg::SetMessageText(LPCTSTR setmsg)
{
	m_strMsg = setmsg;
	GetDlgItem(IDC_STATIC_MESSAGE_1)->SetWindowText( m_strMsg );
}


// コピーできなかった仕訳の表示
void CSCopyMsgDlg::SetCopyErrorData(LPCTSTR errmsg)
{
	m_strMsg = errmsg;
	GetDlgItem(IDC_STATIC_MESSAGE_1)->SetWindowText( m_strMsg );

	m_strCANCEL = _T( "ＯＫ" );
	GetDlgItem(IDCANCEL)->SetWindowText( m_strCANCEL );

	if (m_PrgRcnt.IsWindowEnabled() != 0)  {
		m_PrgRcnt.EnableWindow(FALSE);
		m_PrgRcnt.ShowWindow(SW_HIDE);
	}
}
