// HjiyuDlg.cpp : 実装ファイル
//

#include "stdafx.h"

#include "resource.h"

#include "HjiyuInp.h"
#include "HjiyuDlg.h"


// CHjiyuDlg ダイアログ

IMPLEMENT_DYNAMIC(CHjiyuDlg, ICSDialog)

CHjiyuDlg::CHjiyuDlg(CWnd* pParent /*=NULL*/)
	: ICSDialog(CHjiyuDlg::IDD, pParent)
{
	m_nID = IDD;
	m_pParent = pParent;
	m_Dispflg = FALSE;
	m_DlgRect.SetRectEmpty();
	m_THIS_DIALOG_OK = FALSE;
	m_mode = 0;

	Create( CHjiyuDlg::IDD, pParent );
	//アクセラレータテーブルロード
	m_hAccelTable = ::LoadAccelerators( AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
}

CHjiyuDlg::~CHjiyuDlg()
{
}

void CHjiyuDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHjiyuDlg, ICSDialog)
	ON_EN_SETFOCUS(IDC_EDIT1, &CHjiyuDlg::OnEnSetfocusEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CHjiyuDlg::OnEnKillfocusEdit1)
	ON_MESSAGE( WM_RETROFOCUS, RetroFocus )
END_MESSAGE_MAP()


// カーソルセット
void CHjiyuDlg::set_focus()
{
	PostMessage(WM_RETROFOCUS);
}

void CHjiyuDlg::set_fastfocus( BOOL modefy_sg )
{
	m_MODEFY_OK = modefy_sg;
	m_THIS_DIALOG_OK = TRUE;

	GetDlgItem( IDC_EDIT1 )->SetWindowText( m_Hdata.hjy_name );
	((CEdit*)GetDlgItem( IDC_EDIT1 ))->SetSel( 0, -1);

	set_focus();
}

LRESULT CHjiyuDlg::RetroFocus( WPARAM wParam, LPARAM lParam )
{
	c_set( IDC_EDIT1 );

	return 1;
}


// CHjiyuDlg メッセージ ハンドラ

void CHjiyuDlg::PostNcDestroy()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	delete this;

//	ICSDialog::PostNcDestroy();
}

BOOL CHjiyuDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	// 入力桁数
	((CEdit*)GetDlgItem( IDC_EDIT1 ))->LimitText(2);
	mode_string();

	ICSDialog::OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CHjiyuDlg::OnEnSetfocusEdit1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
}

void CHjiyuDlg::OnEnKillfocusEdit1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	TerminationHjiyuedit(0);
}


//親の現在のフォーカスポジションへ
void CHjiyuDlg::to_parent( CWnd *parent, short nChar )
{
	parent->SendMessage( WM_HJIYU_INEND, nChar );
}

int CHjiyuDlg::TerminationHjiyuedit(short nChar) 
{
	int i;
	char pbuf[50], tmp[64] = {0};

//TRACE( "@CHjiyuDlg::TerminationHjiyuedit %d\n", nChar );

	if( m_THIS_DIALOG_OK == FALSE )
		return 1;

	// SPACE キーは入力させない
	if( nChar == VK_SPACE )
		return 0;

	switch( nChar )
	{
	case NULL:
	case VK_RIGHT:
	case VK_RETURN:
	case VK_TAB:
//	case VK_F3:
	case VK_F2:
	case VK_LEFT:
	case VK_UP:
	case VK_DOWN:
		pbuf[0] = '\0';
		//セットした変動事由と同じ場合
		GetDlgItem( IDC_EDIT1 )->GetWindowText( tmp, sizeof tmp );
		if( strcmp( m_Hdata.hjy_name, tmp ) == 0 ){
			break;
		}

		GetDlgItem( IDC_EDIT1 )->GetWindowText( pbuf, 3 );
		for( i = 0 ; pbuf[i] ; ++i )
		{
			if( isdigit( (int)(BYTE)pbuf[i] ) )
				continue;
			else
			{
				pbuf[0] = '\0';
				break;
			}
		}

		// Return キーで未入力はブザー
		if( nChar == VK_RETURN && pbuf[0] == '\0' ) {
			if( m_Hdata.hjy_code == 0 ) {
				Buzzer();
				break;
			}
		}
		if( pbuf[0] == '\0' )	break;	//空うちは処理しない

		if( ! m_MODEFY_OK )	// 仕訳修正不可
			break;

		//親に選択番号を通知する
		//(通知後は即座に親より返答で SetHjiyuData()を呼んでもらう)
		m_pParent->SendMessage( WM_HJIYU_SEL, atoi( pbuf ) );
		break;

	case VK_BACK:		//変動事由削除
	case VK_DELETE:
		m_pParent->SendMessage( WM_HJIYU_SEL, 0 );
		break;
	}

	if( nChar == VK_TAB && is_SHIFT() )
		return 1;

	switch( nChar )
	{
	case VK_RIGHT:
	case VK_LEFT:
	case VK_RETURN:
	case VK_UP:
	case VK_DOWN:
//	case VK_F3:
	case VK_F2:
	case VK_ESCAPE:
	case VK_TAB:
	case VK_PRIOR:
	case VK_NEXT:
		to_parent( m_pParent, nChar );
		return 0;
		break;

//	case VK_TAB:
//		update_editstr();	// 選択文字を更新
//		return 1;
//		break;
//	case VK_UP:
//		to_parent( m_pParent, nChar );
//		break;

	default:
		return 1;
		break;
	}
}


// 変動事由セット
int CHjiyuDlg::SetHjiyuData( HJIYU_DATA* data, BOOL bUpdate )
{
	memcpy( &m_Hdata, data, sizeof HJIYU_DATA );

	if( bUpdate ) {
		GetDlgItem( IDC_EDIT1 )->SetWindowText( data->hjy_name );
		((CEdit*)GetDlgItem( IDC_EDIT1 ))->SetSel( 0, -1);

		c_set( IDC_EDIT1 );
	}

	return 0;
}

// モードをセット
void CHjiyuDlg::set_mode( int mode )
{
	m_mode = mode;
	mode_string();
}

// モードタイトルをセット
void CHjiyuDlg::mode_string()
{
	CString str;

	switch(	 m_mode ) {
	case 0: str = "変動事由";	break;
	case 1: str = "振替科目";	break;
	case 2: str = "内部取引";	break;
	case 4: str = "資金調整";	break;
	}

	GetDlgItem( IDC_STATIC0 )->SetWindowText(str);
}


BOOL CHjiyuDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( (m_hAccelTable != NULL) && ::TranslateAccelerator( m_hWnd, m_hAccelTable, pMsg))
	{
		return TRUE;
	}

	if( ((CEdit*)GetDlgItem( IDC_EDIT1 ))->m_hWnd == pMsg->hwnd )
	{
		if( pMsg->message == WM_KEYDOWN )
		{
			if( pMsg->wParam == VK_TAB && is_SHIFT() )
			{
				pMsg->wParam = VK_LEFT;
			}
			if( TerminationHjiyuedit(pMsg->wParam) == 0 )
			{
				return 1;
			}
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}
