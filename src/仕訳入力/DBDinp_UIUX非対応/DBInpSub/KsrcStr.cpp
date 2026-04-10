// KsrcStr.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "resource.h"
#include "KsrcStr.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKsrcStr ダイアログ

#define TKNO_LEN	6


CKsrcStr::CKsrcStr(CWnd* pParent /*=NULL*/)
	: ICSDialog(CKsrcStr::IDD, pParent)
//	: ICSDialog()

{
	//{{AFX_DATA_INIT(CKsrcStr)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT

#ifdef OLD_CLOSE
	if( CDtmp.Load( MAKEINTRESOURCE( IDD_KSRCSTR )) == TRUE ) {
		InitModalIndirect( CDtmp.m_hTemplate, NULL );
		DlgFontSize = ICSDlgFont( pParent, &CDtmp, _T("ＭＳ ゴシック" ) );
	}
#endif

	Maxflg = 0;
	bOther = FALSE;

	m_RecNo = -1;
}


void CKsrcStr::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKsrcStr)
	DDX_Control(pDX, IDC_LISTTKSRCH, m_ListTkstr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKsrcStr, ICSDialog)
	//{{AFX_MSG_MAP(CKsrcStr)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REP, OnRep)
	ON_BN_CLICKED(IDC_TOROK, OnTorok)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKsrcStr メッセージ ハンドラ

BOOL CKsrcStr::OnInitDialog() 
{
	// 他の対応摘要にいく場合は、ボタンの文字列レイアウトの変更をする
	if( bOther ) {
		CRect dlgrect, btnrect, list;

		GetWindowRect( dlgrect ); 

		GetDlgItem( IDC_ADD )->GetWindowRect( btnrect );
		GetDlgItem( IDC_LISTTKSRCH )->GetWindowRect( list );

		ScreenToClient( btnrect );
		ScreenToClient( list );

		int ht = btnrect.Height();
		int wd = list.Width();
		int btnHeight, sps;
		btnHeight = (ht/2);
		btnHeight += (ht/4);
		sps = (btnHeight /3);	//ボタン表示間隔

		int x, y;
		x = list.left;
		y = list.bottom;

		y = (y+sps);
		GetDlgItem( IDC_ADD )->MoveWindow( x, y, wd, btnHeight );
		y = (y+btnHeight);
		y += sps;
		GetDlgItem( IDC_REP )->MoveWindow( x, y, wd, btnHeight );
		y = (y+btnHeight);
		y += sps;
		GetDlgItem( IDC_TOROK )->MoveWindow( x, y, wd, btnHeight );
		y = (y+btnHeight);
		y += sps;
		GetDlgItem( IDCANCEL )->MoveWindow( x, y, wd, btnHeight );

		// ｷｬﾝｾﾙのスクリーン座標より、ダイアログの大きさをセットする
		GetDlgItem( IDCANCEL )->GetWindowRect( btnrect );

		int dlght;
		dlght = (btnrect.bottom + sps) - dlgrect.top;

		MoveWindow( dlgrect.left, dlgrect.top, dlgrect.Width(), dlght );

		GetDlgItem( IDC_ADD )->SetWindowText("摘要辞書に追加して、対応摘要に登録する");
		GetDlgItem( IDC_REP )->SetWindowText("摘要辞書を置換して、対応摘要に登録する");
		GetDlgItem( IDC_TOROK )->SetWindowText("類似摘要を対応摘要に登録する");
	}

	ICSDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください

	m_ListTkstr.ResetContent();
	int cnt = strArray.GetSize();

	if( Maxflg ) GetDlgItem( IDC_ADD )->EnableWindow( FALSE );

	// 他登録ボタン
	GetDlgItem( IDC_TOROK )->EnableWindow( bOther );
	if( ! bOther ) {
		GetDlgItem( IDC_TOROK )->ShowWindow( SW_HIDE );
	}

	for( int i=0; i < cnt; i++ ) {
		m_ListTkstr.AddString( strArray[i] );
	}

	m_ListTkstr.SetCurSel( 0 );

	ICSDialog::OnInitDialogEX();

	RECT	rc;
	GetWindowRect( &rc );

	if( (posx != -1) && (posy != -1) ) {
		rc.bottom = posy + (rc.bottom-rc.top);
		rc.right = posx + ( rc.right - rc.left );
		rc.top = posy;
		rc.left = posx;

		MoveWindow( &rc );
	}

	GotoDlgCtrl( GetDlgItem( IDCANCEL ) );
	return FALSE;

//	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

// 追加
void CKsrcStr::OnAdd() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	ICSDialog::EndDialog( EV_ADD );
}

// 置換
void CKsrcStr::OnRep() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	int sel = m_ListTkstr.GetCurSel();
	m_RecNo = -1;	

	if( sel != LB_ERR ) {
		CString str = strArray[sel];
		CString no = str.Left(TKNO_LEN);
		m_RecNo = atoi( (LPCTSTR)no );
	}

	ICSDialog::EndDialog( EV_REP );
}

// 他の登録へ
void CKsrcStr::OnTorok() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int sel = m_ListTkstr.GetCurSel();
	m_RecNo = -1;	

	if( sel != LB_ERR ) {
		CString str = strArray[sel];
		CString no = str.Left(TKNO_LEN);
		m_RecNo = atoi( (LPCTSTR)no );
	}

	ICSDialog::EndDialog( EV_OTHER );
}


// キャンセル
void CKsrcStr::OnCancel() 
{
	// TODO: この位置に特別な後処理を追加してください。

	ICSDialog::EndDialog( EV_CANCEL );
}



BOOL CKsrcStr::PreTranslateMessage(MSG* pMsg) 
{

	if( pMsg->message == WM_KEYDOWN ) {
		if( pMsg->hwnd == GetDlgItem( IDC_LISTTKSRCH )->m_hWnd ) {
			if( pMsg->wParam == VK_RETURN ) {
				NextDlgCtrl();
				return TRUE;
			}
			else if( pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN ) {
				return ICSDialog::PreTranslateMessage(pMsg);
			}
		}

		if( ! bOther ) {
			switch( pMsg->wParam ) {
			case	VK_UP:
			case	VK_DOWN:
			case	VK_NEXT:
			case	VK_PRIOR:
		
				m_ListTkstr.SendMessage( pMsg->message, pMsg->wParam );
				return TRUE;
			}
		}
		else {
			switch( pMsg->wParam ) {
			case	VK_UP:
				PrevDlgCtrl();
				return TRUE;
			case	VK_DOWN:
				NextDlgCtrl();
				return TRUE;
			case	VK_NEXT:
			case	VK_PRIOR:
				m_ListTkstr.SendMessage( pMsg->message, pMsg->wParam );
				return TRUE;
			}
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

