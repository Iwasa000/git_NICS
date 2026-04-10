#include "StdAfx.h"
#include "SyzBaseDlg.h"

CSyzBaseDlg::CSyzBaseDlg( UINT id, CWnd* pParent/*=NULL*/ )
	: ICSDialog(id, pParent)
	, m_wPos(0)
	, m_cmPos(0)
	, m_ScW(0)
	, m_bInitFlg(FALSE)
	, m_pBottomDiag(NULL)
	, m_BottomIdx(0)
	, m_pParent(pParent)
	, m_IsStartEnd(FALSE)
{
//2016.02.23 INSERT START
	//LOGBRUSH	lb;
	//lb.lbStyle = BS_SOLID;
	//lb.lbColor = PALETTERGB( 255, 0, 0 );
	//m_hBrushLock.CreateBrushIndirect( &lb );
//2016.02.23 INSERT END
//2016.06.22 INSERT START
	memset( FZERO, '\0', sizeof(FZERO) );
//2016.06.22 INSERT END

}

CSyzBaseDlg::~CSyzBaseDlg(void)
{
}

void CSyzBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INPUTLOCK, m_InputLock);	// 31テスト削除

}

BOOL CSyzBaseDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	ICSDialog::OnInitDialogEX();

	m_bInitFlg = TRUE;

//2016.02.23 INSERT START
	Make_StaticFont();

	SetInputLockColor();
//2016.02.23 INSERT END

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

BOOL CSyzBaseDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( pMsg->message == WM_KEYDOWN ){
		if( (pMsg->wParam==VK_NEXT) || (pMsg->wParam==VK_PRIOR) ){
			SCROLLINFO ScrollInfo;
			int	c = GetScrollPos( SB_VERT );
			GetScrollInfo( SB_VERT, &ScrollInfo );
			if( pMsg->wParam == VK_NEXT ){
				SetRedrawScroll( c+ScrollInfo.nPage );
			}
			if( pMsg->wParam == VK_PRIOR ){
				SetRedrawScroll( c-ScrollInfo.nPage );
			}
			return TRUE;
		}
		else if( pMsg->wParam == VK_F4 ){
			m_pParent->PostMessageA( WM_KEYDOWN, VK_F4 );
			return TRUE;
		}
		else if( pMsg->wParam == VK_F5 ){
			m_pParent->PostMessageA( WM_KEYDOWN, VK_F5 );
			return TRUE;
		}

		else if( pMsg->wParam == VK_F6 ){
			m_pParent->PostMessageA( WM_KEYDOWN, VK_F6 );
			return TRUE;
		}

//		else if( pMsg->wParam == VK_F6 ){
//			m_pParent->PostMessageA( WM_KEYDOWN, VK_F4 );
//			return TRUE;
//		}
		else if( pMsg->wParam == VK_F9 ){
			m_pParent->PostMessageA( WM_KEYDOWN, VK_F9 );
			return TRUE;
		}
		else if( pMsg->wParam == VK_F11 ){
			m_pParent->PostMessageA( WM_KEYDOWN, VK_F11 );
			return TRUE;
		}
		else if( pMsg->wParam == VK_F12 ){
			m_pParent->PostMessageA( WM_KEYDOWN, VK_F12 );
			return TRUE;
		}

	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

//-----------------------------------------------------------------------------
// スクロール再描画
//-----------------------------------------------------------------------------
// 引数	mpos	：	動かす位置
//-----------------------------------------------------------------------------
void CSyzBaseDlg::SetRedrawScroll( int mpos )
{
	int smax = GetScrollLimit( SB_VERT );
	if( mpos < 0 ){
		mpos = 0;
	}
	if( mpos > smax ){
		mpos = smax;
	}
	WPARAM	wParam;
	wParam = mpos << 16;
	wParam |= (int)SB_THUMBTRACK;
	PostMessage( WM_VSCROLL, wParam );
}


BEGIN_MESSAGE_MAP(CSyzBaseDlg, ICSDialog)
	ON_WM_CTLCOLOR()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
END_MESSAGE_MAP()

HBRUSH CSyzBaseDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = ICSDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。

	// TODO:  既定値を使用したくない場合は別のブラシを返します。

	if( nCtlColor == CTLCOLOR_DLG ){
		hbr = m_ShinCom.GetBkClrBrsh();
	}

//2016.02.23 INSERT START
	//if( GetDlgItem(IDC_STATIC_INPUTLOCK) != NULL ){
	//	if(pWnd->m_hWnd==GetDlgItem(IDC_STATIC_INPUTLOCK)->m_hWnd){
	//		pDC->SetTextColor( RGB(255, 255, 255) );
	//		return (HBRUSH)m_hBrushLock;
	//	}
	//}
//2016.02.23 INSERT END

	return hbr;
}

void CSyzBaseDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	if( pScrollBar ){
		return;
	}
	SCROLLINFO SI;
	if( GetScrollInfo( SB_VERT, &SI ) == FALSE ){
		return;	// スクロールがない場合 harino ('25.03.11)
	}
	int nowPos = m_wPos;

	switch( nSBCode ){
	case SB_LINEUP:
	case SB_PAGEUP:
		if( nSBCode == SB_LINEUP ){
			m_wPos -= m_ScW;
		}
		else{
			m_wPos -= SI.nPage;
		}
		if( m_wPos <= 0 ){
			m_wPos = 0;
		}
		break;
	case SB_LINEDOWN:
	case SB_PAGEDOWN:
		if( nSBCode == SB_LINEDOWN ){
			m_wPos += m_ScW;
		}
		else{
			m_wPos += SI.nPage;
		}
		if( m_wPos >= SI.nMax-(int)SI.nPage ){
			m_wPos = SI.nMax-SI.nPage;
		}
		break;
	case SB_THUMBTRACK:
		m_wPos = nPos;
		break;
	}
	if( (nSBCode==SB_LINEDOWN) || (nSBCode==SB_LINEUP) || (nSBCode==SB_PAGEDOWN) ||
			(nSBCode==SB_PAGEUP) || (nSBCode==SB_THUMBTRACK) ){
		ScrollWindow( 0, nowPos-m_wPos );
		SetScrollPos( SB_VERT, m_wPos );
		UpdateWindow();
		m_cmPos = m_wPos;
	}

	ICSDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CSyzBaseDlg::OnSize(UINT nType, int cx, int cy)
{
	if( m_pBottomDiag ){
		SetScrollPos( SB_VERT, 0, TRUE );
		ScrollWindow( 0, m_cmPos, NULL );
		m_cmPos = 0;
	}

	ICSDialog::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( m_pBottomDiag ){
		RECT	a, b;
		SCROLLINFO ScrollInfo;
		GetWindowRect( &a );
		m_pBottomDiag->GetRect( m_BottomIdx, (LPUNKNOWN)&b );
		if( GetScrollInfo( SB_VERT, &ScrollInfo ) == FALSE ){
			ScrollInfo.nPos = 0;
		}
		ScrollInfo.nMin  = 0;
		ScrollInfo.nMax  = (b.bottom + 80) - a.top + ScrollInfo.nPos - m_cmPos;
		ScrollInfo.nPage = a.bottom - a.top;
		SetScrollInfo( SB_VERT, &ScrollInfo );
		m_wPos = 0;
		m_ScW = 15;
	}
}

void CSyzBaseDlg::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

//	ICSDialog::OnOK();
}

void CSyzBaseDlg::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

//	ICSDialog::OnCancel();
}

//2016.02.23 INSERT START
int CSyzBaseDlg::Make_StaticFont()
{
	// 31テスト削除
	CWnd *item;
	item = GetDlgItem(IDC_STATIC_INPUTLOCK);

	if( item == NULL ){
		return -1;
	}

	LOGFONT log_font;
	item->GetFont()->GetLogFont(&log_font);
	log_font.lfWeight = FW_BOLD;

	m_font.DeleteObject();
	m_font.CreateFontIndirectA( &log_font );

	item->SetFont( &m_font, FALSE );

	return 0;
}

void CSyzBaseDlg::Static_Update( unsigned char Sn_Sign4 )
{
	// 31テスト削除
	CWnd *item;
	item = GetDlgItem(IDC_STATIC_INPUTLOCK);
	
	if( item == NULL ){
		return;
	}

	if( Sn_Sign4&0x01){
		GetDlgItem(IDC_STATIC_INPUTLOCK)->ShowWindow(TRUE);
	}else{
		GetDlgItem(IDC_STATIC_INPUTLOCK)->ShowWindow(FALSE);
	}
}
//2016.02.23 INSERT END

//-----------------------------------------------------------------------------
// 再描画（入力ロック）
//-----------------------------------------------------------------------------
void CSyzBaseDlg::RedrawInputLock()
{
	// 各表に入力ロックコントロール（IDC_STATIC_INPUTLOCK）が必要
	// 31テスト削除
	SetInputLockColor();
	m_InputLock.UpdateWindow();
	
}

//-----------------------------------------------------------------------------
// 入力ロックコントロール色設定
//-----------------------------------------------------------------------------
void CSyzBaseDlg::SetInputLockColor()
{
	if( m_InputLock.m_hWnd ){
		m_InputLock.DefaultColor( FALSE );
		m_InputLock.SetBackColor( SYZBASE_RED );
		m_InputLock.SetTextColor( SYZBASE_WHITE );
	}
}

