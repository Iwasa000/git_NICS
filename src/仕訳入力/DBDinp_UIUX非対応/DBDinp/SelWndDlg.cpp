// SelWndDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBDinp.h"
#include "SelWndDlg.h"


// CSelWndDlg ダイアログ

IMPLEMENT_DYNAMIC(CSelWndDlg, ICSDialog)

CSelWndDlg::CSelWndDlg(CWnd* pParent /*=NULL*/)
	: ICSDialog(CSelWndDlg::IDD, pParent), m_selGuid( FALSE )
{
	m_nID = IDD;
	m_pParent = pParent;
	flgPen = FALSE;

	m_bIgnoreActvate = FALSE;

	Create( CSelWndDlg::IDD, pParent );
}

CSelWndDlg::~CSelWndDlg()
{
}

void CSelWndDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ICSSELCTRL1, m_selfix8);
	DDX_Control(pDX, IDC_ICSSELCTRL2, m_selval32);

	DDX_Control(pDX, IDC_SELGUIDE, m_selGuid);
}


BEGIN_MESSAGE_MAP(CSelWndDlg, ICSDialog)
	ON_MESSAGE( WM_SELGUIDEMSG, OnSelGuideMsg)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CSelWndDlg メッセージ ハンドラ

BOOL CSelWndDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	fix8disp( TRUE );
	enable_fixval( TRUE );

	ICSDialog::OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}



//BOOL CSelWndDlg::Create()
//{
//	return ICSDialog::Create(m_nID, m_pParent);
//}


void CSelWndDlg::PostNcDestroy() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	delete this;
	
//	ICSDialog::PostNcDestroy();
}


// 固定科目選択画面状態ＳＷ
BOOL CSelWndDlg::fix8cnd()
{
	return fix8sg;
}

void CSelWndDlg::fix8cnd( BOOL sg )
{
	fix8sg = sg;
}

// 選択画面表示色状態
BOOL CSelWndDlg::fixvalcnd()
{
	return fixvalsg;
}

void CSelWndDlg::fixvalcnd( BOOL sg )
{
	fixvalsg = sg;
}


//
// 固定項目の表示
void CSelWndDlg::fix8disp( BOOL sg )
{
	if( sg )
	{
		// 固定科目選択のディセーブル／イネーブル
		enable_fix8( sg );

		// 選択タイトル表示のディセーブル／イネーブル
		valtitle( !sg );
	}
	else
	{
		// 選択タイトル表示のディセーブル／イネーブル
		valtitle( !sg );

		// 固定科目選択のディセーブル／イネーブル
		enable_fix8( sg );
	}

	fix8cnd( sg );

}

// 固定科目選択のディセーブル／イネーブル
void CSelWndDlg::enable_fix8( BOOL sg )
{
	if( flgPen )
	{
		RECT rect1,rect2;
		CWnd *pfix8,*pSel32;

		VERIFY(pfix8 = GetDlgItem( IDC_ICSSELCTRL1 ));
		VERIFY(pSel32 = GetDlgItem( IDC_ICSSELCTRL2 ));
		pfix8->GetWindowRect( &rect1 );
		ScreenToClient( &rect1 );
		pSel32->GetWindowRect( &rect2 );
		ScreenToClient( &rect2 );

		// New Size
		if( sg == TRUE )
			rect2.top = rect1.bottom/* + 1*/;
		else
			rect2.top = rect1.top;

		pSel32->SetWindowPos( 0, rect2.left, rect2.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER );

	}
	else
	{
#ifdef OLD_CLOSE
		RECT newrect;
		double rate_w, rate_h;

	//	get_viewrate( rate_w, rate_h );	//ビューサイズ比率をゲット
	//	GetRate( rate_w, rate_h );

		rate_w = rate_h = 1.0;

		CWnd *pfix8;
		VERIFY(pfix8 = GetDlgItem( IDC_ICSSELCTRL1 ));

		// New Size
		if( sg == TRUE )
		{
			newrect.top = 0;
			newrect.left = 0;
			newrect.bottom = m_selfix8_rect.bottom * rate_h;
			newrect.right = m_selfix8_rect.right * rate_w;
		}
		else
		{
			newrect.top = 0;
			newrect.left = 0;
			newrect.bottom = 0;
			newrect.right = 0;
		}
#endif
		m_selfix8.ShowWindow(sg ? SW_SHOW : SW_HIDE);

	//	pfix8->SetWindowPos( 0, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE );
	//	pfix8->UpdateWindow();
	}

}

//
// 選択画面の表示色
void CSelWndDlg::enable_fixval( BOOL sg )
{
	CICSSel *pfix8,*pSel32;
	CICSDisp *ttlbaseorg,*ttlorg;

	VERIFY(pfix8 = (CICSSel*)GetDlgItem( IDC_ICSSELCTRL1 ));
	VERIFY(pSel32 = (CICSSel*)GetDlgItem( IDC_ICSSELCTRL2 ));

	VERIFY(ttlbaseorg = (CICSDisp*)GetDlgItem( IDC_VALTITLEBASE ));
	VERIFY(ttlorg = (CICSDisp*)GetDlgItem( IDC_VALTITLE ));

#ifndef ICSCOLOR_
	ICSColorInfo ci;
	((ICSWinApp*)AfxGetApp())->GetStanderdColor( ci );

	if( ci.m_swOnOff > 0 ) {
//	if( 0 ) {
		// 科目選択画面背景色の設定
		pfix8->SetBackColor( ci.FixKmkBackColor() );
		pSel32->SetBackColor( ci.PageKmkBackColor() );

		// 科目選択画面文字色の設定
		pfix8->SetForeColor( ci.FixKmkTextColor() );
		pSel32->SetForeColor( ci.PageKmkTextColor() );

		// 科目選択画面枠の色
		pfix8->SetLineColor( RGB_GRAY );
		pSel32->SetLineColor( RGB_GRAY );

		// 選択タイトル背景色、文字色
		ttlbaseorg->SetBackColor( ci.PageKmkBackColor() );
		ttlorg->SetBackColor( ci.PageKmkBackColor() );
		ttlorg->SetForeColor( ci.PageKmkTextColor() );

		m_vwColor = ci.ViewColor();
	}
	else {
		// 科目選択画面背景色の設定
		pfix8->SetBackColor( RGB_FIX8_BACK );
		pSel32->SetBackColor( RGB_VAL32_BACK );

		// 科目選択画面文字色の設定
		pfix8->SetForeColor( RGB_FIX8_FORE );
		pSel32->SetForeColor( RGB_VAL32_FORE );
		// 科目選択画面枠の色
		pfix8->SetLineColor( RGB_GRAY );
		pSel32->SetLineColor( RGB_GRAY );

		// 選択タイトル背景色、文字色
		ttlbaseorg->SetBackColor( RGB_VAL32_BACK );
		ttlorg->SetBackColor( RGB_VAL32_BACK );
		ttlorg->SetForeColor( RGB_VAL32_FORE );
	}
#else

	// New Size
	if( sg )
	{
		// 科目選択画面背景色の設定
		pfix8->SetBackColor( RGB_FIX8_BACK );
		pSel32->SetBackColor( RGB_VAL32_BACK );

		// 科目選択画面文字色の設定
		pfix8->SetForeColor( RGB_FIX8_FORE );
		pSel32->SetForeColor( RGB_VAL32_FORE );
		// 科目選択画面枠の色
		pfix8->SetLineColor( RGB_GRAY );
		pSel32->SetLineColor( RGB_GRAY );

		// 選択タイトル背景色、文字色
		ttlbaseorg->SetBackColor( RGB_VAL32_BACK );
		ttlorg->SetBackColor( RGB_VAL32_BACK );
		ttlorg->SetForeColor( RGB_VAL32_FORE );
	}
	else
	{
		// 科目選択画面背景色の設定
		pfix8->SetBackColor( RGB_FIX8_BACK_UNSEL );
		pSel32->SetBackColor( RGB_VAL32_BACK_UNSEL );
		// 科目選択画面文字色の設定
		pfix8->SetForeColor( RGB_FIX8_FORE_UNSEL );
		pSel32->SetForeColor( RGB_VAL32_FORE_UNSEL );
		// 科目選択画面枠の色
		pfix8->SetLineColor( RGB_GRAY );
		pSel32->SetLineColor( RGB_GRAY );

		// 選択タイトル背景色、文字色
		ttlbaseorg->SetBackColor( RGB_VAL32_BACK_UNSEL );
		ttlorg->SetBackColor( RGB_VAL32_BACK_UNSEL );
		ttlorg->SetForeColor( RGB_VAL32_FORE_UNSEL );
	}
#endif

	fixvalcnd( sg );
}

// 選択タイトルテキストセット
void CSelWndDlg::set_valtitle( char *msg )
{
	puts( IDC_VALTITLE, msg, CICSDisp );
}

// 選択タイトル表示のディセーブル／イネーブル
void CSelWndDlg::valtitle( BOOL sg )
{
	CWnd *ttlbaseorg,*ttlorg;
	CICSSel *pfix8,*pSel32;
	RECT rect8,rect32,newrect,newrect2;
	LONG v_h, v_w;
	WINDOWPLACEMENT place;

	VERIFY(ttlbaseorg = (CICSDisp*)GetDlgItem( IDC_VALTITLEBASE ));
	VERIFY(ttlorg = (CICSDisp*)GetDlgItem( IDC_VALTITLE ));

	// New Size
	if( sg == TRUE )
	{
		ttlbaseorg->ShowWindow(SW_SHOW);
		ttlorg->ShowWindow(SW_SHOW);

		VERIFY(pfix8 = (CICSSel*)GetDlgItem( IDC_ICSSELCTRL1));
		VERIFY(pSel32 = (CICSSel*)GetDlgItem( IDC_ICSSELCTRL2));
		pfix8->GetWindowRect( &rect8 );
		pSel32->GetWindowRect( &rect32 );

		newrect.top = rect8.top;
		newrect.left = rect8.left;
		newrect.bottom = rect8.bottom;	// m_selfix8_rect.bottom * rate_h;
		newrect.right = rect8.right;	// m_selfix8_rect.right * rate_w;

	//	ttlbaseorg->SetWindowPos( 0, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE );
		ScreenToClient( &newrect );
		ttlbaseorg->MoveWindow( newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top );

		v_h = (newrect.bottom - newrect.top) / 3;
		v_w = ((newrect.right - newrect.left) / 12);
		ttlbaseorg->GetWindowPlacement( &place );
		newrect2.top = place.rcNormalPosition.top + v_h;
		newrect2.left = place.rcNormalPosition.left;
		newrect2.bottom = newrect2.top + v_h;
		newrect2.right = place.rcNormalPosition.right;

		ttlorg->SetWindowPos( 0, newrect2.left, newrect2.top, newrect2.right - newrect2.left, newrect2.bottom - newrect2.top, SWP_NOZORDER|SWP_NOACTIVATE );
	}
	else
	{
		ttlbaseorg->ShowWindow(SW_HIDE);
		ttlorg->ShowWindow(SW_HIDE);
	}
}

BEGIN_EVENTSINK_MAP(CSelWndDlg, ICSDialog)
	ON_EVENT(CSelWndDlg, IDC_ICSSELCTRL1, 1, CSelWndDlg::SelectedIcsselctrl1, VTS_I2 VTS_I4)
	ON_EVENT(CSelWndDlg, IDC_ICSSELCTRL2, 1, CSelWndDlg::SelectedIcsselctrl2, VTS_I2 VTS_I4)
END_EVENTSINK_MAP()



void CSelWndDlg::SelectedIcsselctrl1(short position, long Number)
{
	LPARAM lp;
	lp = MAKELPARAM( position, Number );

	m_pParent->SendMessage( WM_DBDINPMSG, DWP_SELECTED1, lp );
}

void CSelWndDlg::SelectedIcsselctrl2(short position, long Number)
{
	LPARAM lp;
	lp = MAKELPARAM( position, Number );

	m_pParent->SendMessage( WM_DBDINPMSG, DWP_SELECTED2, lp );
}


// 選択ガイドのクリック
LRESULT CSelWndDlg::OnSelGuideMsg( WPARAM wp, LPARAM lp )
{
	m_pParent->SendMessage( WM_SELGUIDEMSG, wp, 0 );

	return 1;
}

void CSelWndDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
//MyTrace( "CSelWndDlg::OnActivate nState = %d, pWndOther = %08x, bMinimized = %d\n", nState, pWndOther, bMinimized );

	ICSDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
}
