// Forksel.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "resource.h"
#include "Forksel.h"

//#include "MstRw.h"
//#include "zinpsub.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CForksel ダイアログ


CForksel::CForksel(CWnd* pParent /*=NULL*/)
	: ICSDialog(CForksel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CForksel)
	m_radio = -1;
	//}}AFX_DATA_INIT

	// 09.03 /03
	m_GuidColor = ::CreateSolidBrush( RGB_WHITE );
}


void CForksel::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CForksel)
	DDX_Radio(pDX, IDC_RADIO1, m_radio);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	//}}AFX_DATA_MAP
}

void CForksel::OnInitial( struct _JOBSEL_PAC *sel, int nChar )
{
	alt_key = nChar;
	p_sel = sel;
}

//int CForksel::get_selno()
//{
//	return m_radio;
//}

/*****************************************************
	現在のビュー拡大比率に従って表示サイズを変える
******************************************************/
//void CForksel::kakudai(CWnd* wnd)
//{
//	RECT	newrect;
//	double rate_w, rate_h;
//
//	get_viewrate( rate_w, rate_h );	//ビューサイズ比率をゲット
//	wnd->GetWindowRect( &newrect );
//	newrect.bottom = newrect.top + ((newrect.bottom - newrect.top ) * rate_h);
//	newrect.right = newrect.left + ((newrect.right - newrect.left ) * rate_w);
//
//	wnd->SetWindowPos( 0, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_NOZORDER|SWP_NOMOVE );
//}

BEGIN_MESSAGE_MAP(CForksel, ICSDialog)
	//{{AFX_MSG_MAP(CForksel)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CForksel メッセージ ハンドラ

#define CASE_ID(x, s_id, r_id)	case x : s_id = IDC_STATIC##x;\
										 r_id = IDC_RADIO##x;	break

BOOL CForksel::OnInitDialog() 
{
	CWnd *pwnd, *pwnd_static;
	int i;
	int idc, idc_static;
	int idx;
	int cnt;
	int set_radio = -1;

	// 09.03 /03
	BOOL bDisp = FALSE;
	if( p_sel ) {
		if( !p_sel->guid_msg.IsEmpty() ) {
			if( p_sel->sel_max < _JOBSEL_MAX ) {

				int st_id, rd_id;
				st_id = rd_id = -1;

				switch( p_sel->sel_max+1 ) {
				CASE_ID( 1, st_id, rd_id );
				CASE_ID( 2, st_id, rd_id );
				CASE_ID( 3, st_id, rd_id );
				CASE_ID( 4, st_id, rd_id );
				CASE_ID( 5, st_id, rd_id );
				CASE_ID( 6, st_id, rd_id );
				}

				if( st_id != -1 && rd_id != -1 ) {
					CRect st_rect, rd_rect;

					GetDlgItem(st_id)->GetWindowRect( &st_rect );
					GetDlgItem(rd_id)->GetWindowRect( &rd_rect );

					ScreenToClient(&st_rect);
					ScreenToClient(&rd_rect);

					CWnd* pwnd;
					pwnd = GetDlgItem(IDC_STATIC_GUID);
					CRect rc;
					pwnd->GetWindowRect( &rc );
					pwnd->MoveWindow( rd_rect.left, st_rect.top,
									  st_rect.Width() + (st_rect.left-rd_rect.left), rc.Height(), TRUE );
					pwnd->ShowWindow(SW_SHOW);
					pwnd->SetWindowText(p_sel->guid_msg);
				}
			}
		}
	}

//	ControlInit( 287, 166, -1 );

	ICSDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください

	if( p_sel )
	{
		this->SetWindowText( p_sel->title );	//ダイアログタイトル
		GetDlgItem( IDC_STATIC0 )->SetWindowText( p_sel->itemmsg );
		cnt = 0;
		for( i = 0 ; i < _JOBSEL_MAX ; ++i )
		{
			switch( i )
			{
			case 0: idc = IDC_RADIO1; idc_static = IDC_STATIC1; break;
			case 1: idc = IDC_RADIO2; idc_static = IDC_STATIC2; break;
			case 2: idc = IDC_RADIO3; idc_static = IDC_STATIC3; break;
			case 3: idc = IDC_RADIO4; idc_static = IDC_STATIC4; break;
			case 4: idc = IDC_RADIO5; idc_static = IDC_STATIC5; break;
			case 5: idc = IDC_RADIO6; idc_static = IDC_STATIC6; break;
//			case 6: idc = IDC_RADIO7; idc_static = IDC_STATIC7; break;
//			case 7: idc = IDC_RADIO8; idc_static = IDC_STATIC8; break;
			default:
				continue;
			}

			pwnd = GetDlgItem( idc );
			pwnd_static = GetDlgItem( idc_static );

			if( i < p_sel->sel_max )
			{
				pwnd->SetWindowText( p_sel->sel_msg[i] );
				pwnd_static->SetWindowText( p_sel->sel_guide[i] );
				if( p_sel->sel_attr[i] )
				{
					pwnd->EnableWindow( TRUE );
					pwnd_static->EnableWindow( TRUE );

					if( set_radio == -1 )
					{
						((CButton*)pwnd)->SetCheck( 1 );
						set_radio = i;
					}
					if( p_sel->sel_no == i )
					{
//						if( idx = GetCheckedRadioButton( IDC_RADIO1, IDC_RADIO8 ) )
						if( idx = GetCheckedRadioButton( IDC_RADIO1, IDC_RADIO6 ) )
						{
							// 既に立っているチェックをOFFにする。
							((CButton*)GetDlgItem( idx ))->SetCheck( 0 );	// チェックOFF
						}
						((CButton*)pwnd)->SetCheck( 1 );	// チェックON
						m_radio = i;
					}

					++cnt;
				}
				else
				{
					pwnd->EnableWindow(  FALSE );
					pwnd_static->EnableWindow( FALSE );
				}
			}
			else
			{
				pwnd->ShowWindow( SW_HIDE );
				pwnd_static->ShowWindow( SW_HIDE );
			}
		}

		if( !cnt )
			OnCancel();
		else if( cnt == 1 )
			GotoDlgCtrl( GetDlgItem( IDOK ) );
		if( m_radio < 0 )
			m_radio = set_radio;

		this->UpdateWindow();

	}

	ICSDialog::OnInitDialogEX();

	return FALSE;	//TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

BOOL CForksel::PreTranslateMessage(MSG* pMsg) 
{
int idc;

	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

	if( pMsg->message == WM_KEYDOWN )
	{
		if(  pMsg->wParam == alt_key )
		{
			GetDlgItem( IDCANCEL )->PostMessage( WM_LBUTTONDOWN, 0, 0 );
			GetDlgItem( IDCANCEL )->PostMessage( WM_LBUTTONUP, 0, 0 );
			return 1;
		}
		else if( GetKeyState( VK_SHIFT ) < 0 )
			;
		// ラジオボタンエリア
		else if( (GetDlgItem( IDCANCEL )->m_hWnd != pMsg->hwnd) && (GetDlgItem( IDOK )->m_hWnd != pMsg->hwnd) )
		{
			if( pMsg->wParam == VK_END || pMsg->wParam == VK_RETURN ) {
				GotoDlgCtrl( GetDlgItem( IDOK ) );
					return 1;
			}
/*
			switch( pMsg->wParam )
			{
			case VK_RETURN:
			case VK_F3:
			case VK_RIGHT:
				GotoDlgCtrl( GetDlgItem( IDOK ) );
				return 1;
			case VK_LEFT:
				return 1;
			}
*/
		}
		// 確認エリア
		else if( (GetDlgItem( IDCANCEL )->m_hWnd == pMsg->hwnd) || (GetDlgItem( IDOK )->m_hWnd == pMsg->hwnd) )
		{
			switch( pMsg->wParam )
			{
/*			case VK_F3:
				if( GetDlgItem( IDOK )->m_hWnd == pMsg->hwnd )
				{
					GotoDlgCtrl( GetDlgItem( IDCANCEL ) );
					return 1;
				}
				break;
*/
			case VK_F2:
				if( GetDlgItem( IDCANCEL )->m_hWnd == pMsg->hwnd )
				{
					GotoDlgCtrl( GetDlgItem( IDOK ) );
					return 1;
				}
			case VK_LEFT:
			case VK_UP:
				if(GetDlgItem( IDOK )->m_hWnd != pMsg->hwnd)
					break;
				idc = GetCheckedRadioButton( IDC_RADIO1, IDC_RADIO6);
//				idc = GetCheckedRadioButton( IDC_RADIO1, IDC_RADIO8);
//
//				idc = -1;
//				switch( m_radio )
//				{
//				case 0: idc = IDC_RADIO1; break;
//				case 1: idc = IDC_RADIO2; break;
//				case 2: idc = IDC_RADIO3; break;
//				case 3: idc = IDC_RADIO4; break;
//				case 4: idc = IDC_RADIO5; break;
//				}
//				if( idc != -1 )
				if( idc )
				{	
					GotoDlgCtrl( GetDlgItem( idc ) );
					return 1;
				}
			}
		}
	}
	return ICSDialog::PreTranslateMessage(pMsg);
}

void CForksel::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	ICSDialog::OnOK();
}


void CForksel::OnCancel() 
{
	// TODO: この位置に特別な後処理を追加してください。
//#ifdef _ICSPEN_	// 03.27 /01
//	flgPenDlg = TRUE;
//#endif

	ICSDialog::OnCancel();
}



HBRUSH CForksel::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = ICSDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: この位置で DC のアトリビュートを変更してください

	if( nCtlColor == CTLCOLOR_STATIC ) {
		// 09.03 /03
		if( pWnd ) {
			if( pWnd->GetDlgCtrlID() == IDC_STATIC_GUID ) {

				pDC->SetBkColor( RGB_WHITE );
				hbr = m_GuidColor;		
			}
		}
	}
	
	// TODO: デフォルトのブラシが望みのものでない場合には、違うブラシを返してください
	return hbr;
}
