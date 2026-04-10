// SELDLG.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "resource.h"

#include "SELDLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// SELDLG ダイアログ


SELDLG::SELDLG(CWnd* pParent /*=NULL*/)
	: ICSDialog(SELDLG::IDD, pParent)
//	:ICSDialog()
{
	//{{AFX_DATA_INIT(SELDLG)
	//}}AFX_DATA_INIT

	TRACE("SELDLG ctor\n");

	Create( SELDLG::IDD, pParent );

	m_pParent = pParent;

#ifdef OLD_CLOSE
	// モードレスダイアログのフォントサイズ変更　99.11.01
	if( CDtmp.Load( MAKEINTRESOURCE( IDD_SELDLG1 )) == TRUE ) {
		DlgFontSize = ICSDlgFont( pParent, &CDtmp, _T("ＭＳ ゴシック" ) );
		CDtmp.SetFont( _T("ＭＳ ゴシック" ), DlgFontSize );

		CreateIndirect( CDtmp.m_hTemplate, m_pParent );
		m_Fnt.CreatePointFont( DlgFontSize*10, _T("ＭＳ ゴシック" ) );

//		InitModalIndirect( CDtmp.m_hTemplate, NULL );
//		DlgFontSize = ICSDlgFont( pParent, &CDtmp, _T("ＭＳ ゴシック" ) );
//		m_Fnt.CreatePointFont( DlgFontSize*10, _T("ＭＳ ゴシック" ) );
	}
#endif

	Lmode = 1;

	m_columnFlg = 0;
}


void SELDLG::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SELDLG)
	DDX_Control(pDX, IDC_SELLIST, m_sylist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SELDLG, ICSDialog)
	//{{AFX_MSG_MAP(SELDLG)
	ON_LBN_DBLCLK(IDC_SELLIST, OnDblclkSellist)
	ON_LBN_KILLFOCUS(IDC_SELLIST, OnKillfocusSellist)
	ON_WM_VKEYTOITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SELDLG メッセージ ハンドラ


BEGIN_EVENTSINK_MAP(SELDLG, ICSDialog)
    //{{AFX_EVENTSINK_MAP(SELDLG)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


void SELDLG::OnDblclkSellist() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	SelTerm = VK_RETURN;
	OnOK();

}


void SELDLG::PostNcDestroy() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

	delete this;	
	ICSDialog::PostNcDestroy();
}

#define _KANIADD 1

void SELDLG::SetDlgData()
{

	RECT	rc;

	SetWindowText( SelItem );

	m_sylist.ClickMode( Lmode );
	m_sylist.ResetContent();

	CString msg;

	CDC *pd = GetDC();
	CSize	sz( 0, 0 ), msz( 0, 0 );

	CFont *pfont = m_sylist.GetFont();
	LOGFONT	lf;
	pfont->GetLogFont(&lf);
	m_Fnt.CreateFontIndirect(&lf);
	CFont *pOldfnt = pd->SelectObject( &m_Fnt );

#ifdef _KANIADD	//簡易課税第６業種対応2014.12.01
	int increase_size = 25;//下に伸ばすサイズ
#endif

	int i = 0;
	while( 1 ) {
		msg = SelMsg[i];
		if( msg.GetLength() == 0 ) break;
		m_sylist.AddString( msg );

		sz = pd->GetTextExtent( msg );
		if( sz.cx > msz.cx ) msz.cx = sz.cx;
		msz.cy += sz.cy;
		i++;
	}
#ifdef _KANIADD	//摘要辞書登録で消費税選択に『標準－10％』を追加	2020.01.16
	//選択項目数が９を超えると、超えた項目数分だけ下に伸ばす
	if( i > 8 ) {
		increase_size += 21 * (i - 8);
	}
#endif

//	pd->LPtoDP( &msz );
	if( msz.cx < 120 ) msz.cx = 120;
	else msz.cx += 5;

	pd->SelectObject(&pOldfnt);
	ReleaseDC( pd );
	m_Fnt.DeleteObject();

	GetWindowRect( &rc );

	rc.right = rc.left +msz.cx;
//	rc.bottom = rc.top + msz.cy+ (sz.cy/2);
#ifdef _KANIADD	//簡易課税第６業種対応2014.12.01
	int ywidth = rc.bottom - rc.top + increase_size;
#else
	int ywidth = rc.bottom - rc.top;
#endif
	int xwidth = rc.right - rc.left;

	int cyCaption = GetSystemMetrics(SM_CYCAPTION);     // タイトルバーの高さ
	ywidth += cyCaption;

	sx_pos += 15;
	rc.top = sy_pos;
	rc.left = sx_pos;
	rc.bottom = sy_pos + ywidth;
	rc.right = sx_pos + xwidth;

	RECT	prc;
	GetParentFrame()->GetWindowRect( &prc );

	if( rc.bottom > prc.bottom ) {
//		rc.top = sy_pos + ny_width - ywidth;
		rc.top = sy_pos - ny_width  - ywidth;
//		rc.bottom = sy_pos + ny_width;
		rc.bottom = sy_pos - ny_width;
	}

	rc.right += 5;
	m_sylist.SetCurSel( Selno );
	MoveWindow( &rc, TRUE );
//MyTrace("SetDlgData MoveWindow (%d,%d, %d,%d)\n", rc.left, rc.top, rc.right, rc.bottom);

	RECT syRect;
	m_sylist.GetWindowRect( &syRect);
	syRect.right = syRect.left + msz.cx;
//	syRect.bottom = syRect.top + msz.cy + (sz.cy/4);
#ifdef _KANIADD	//簡易課税第６業種対応2014.12.01
	syRect.bottom += increase_size;
#endif
	ScreenToClient( &syRect);
	m_sylist.MoveWindow( &syRect, TRUE );

	SelTerm = 0;
	ShowWindow( SW_SHOW );

}

void SELDLG::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください

//	ICSDialog::OnOK();

	// インボイス対応
	UINT msg = 0;
	TRACE("SELDLG::OnOK()\n ");

	Selno = m_sylist.GetCurSel();
	switch( SelTerm ) {

		case	0:
				SelTerm = VK_RETURN;

		case	VK_RETURN:
		case	VK_RIGHT:

				//m_pParent->SendMessage( WM_TKDICMESSAGE, 0, 0L );
			// インボイス対応
			msg = GetMsg();
			m_pParent->SendMessage( msg, 0, 0L );

				break;

		default	:	OnCancel();
					break;
	}
}

void SELDLG::OnCancel() 
{
	// TODO: この位置に特別な後処理を追加してください。
	TRACE("SELDLG::OnCancel() %08x, SelTerm (%d)\n", this->m_hWnd, SelTerm );

	WPARAM wParam;

	if( SelTerm ) wParam = 1;
	else		  wParam = 2;
	//if( SelTerm )

	// インボイス対応
	//m_pParent->SendMessage( WM_TKDICMESSAGE, wParam, 0L );
	UINT msg = GetMsg();
	m_pParent->SendMessage( msg, wParam, 0L );

	DestroyWindow();

	//	ICSDialog::OnCancel();
}


void SELDLG::DeleteSelDlg()
{
	TRACE("SELDLG::DeleteSelDlg()\n  ");

	DestroyWindow();
}

// リストボックスの フォーカスがなくなったら ダイアログを 消去する
void SELDLG::OnKillfocusSellist() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CWnd *wp = GetActiveWindow();
	   
	if( wp != this ) {
		if( SelTerm == 0 ) {
			TRACE("SELDLG::OnKillfocusSellist() %08x\n", this->m_hWnd );
			PostMessage(WM_CLOSE); //OnCancel が動く
		}
	}
}


int SELDLG::OnVKeyToItem(UINT nKey, CListBox* pListBox, UINT nIndex) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	
	if( (nKey == VK_LEFT) || (nKey == VK_RIGHT) ) {
		SelTerm = nKey;
		OnOK();
		return -2;
	}
	return ICSDialog::OnVKeyToItem(nKey, pListBox, nIndex);
}



void SELDLG::OnSetFont(CFont* pFont) 
{
#ifdef OLD_CLOSE
	ICSDialog::OnSetFont( &m_Fnt );
#endif
	ICSDialog::OnSetFont(pFont);
}

BOOL SELDLG::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	ICSDialog::OnInitDialogEX();

	TRACE("SELDLG::OnInitDialog() %08x\n ", this->m_hWnd);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

BOOL SELDLG::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( pMsg->message == WM_KEYDOWN ) {
		if( pMsg->wParam == VK_RETURN ) {
			OnOK();
			return TRUE;
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

//==================================================================
// メッセージ取得
//------------------------------------------------------------------
// 返送値
//		取得したメッセージ
//==================================================================
UINT SELDLG::GetMsg()
{
	TRACE("SELDLG::GetMsg() \n ");

	UINT msg = 0;

	if( m_columnFlg == SEL_SYOHI ) {
		msg = WM_TKDICMESSAGE;
	}
	else if( m_columnFlg == SEL_MENZEI ) {
		msg = WM_MENZEI_MESSAGE;
	}
	else {
		msg = WM_KUBUN_MESSAGE;
	}

	return msg;
}


//==================================================================
// カラムセット
//------------------------------------------------------------------
// 引数
//		clm				:				カラム
//==================================================================
void SELDLG::SetColumn( int clm )
{
	TRACE("SELDLG::SetColumn() clm = %d, %08x\n ", clm, this->m_hWnd);

	if( clm < 0 ) {
		return;
	}

	m_columnFlg = clm;
}