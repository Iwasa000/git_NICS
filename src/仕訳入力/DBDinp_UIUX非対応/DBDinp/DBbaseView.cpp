// DBbaseView.cpp : 実装ファイル
//	選択画面に表示する部分を担当する
//


#include "stdafx.h"
#include "DBDinp.h"

#include "DBbaseView.h"
#include "icsdisp.h"

#include "input1.h"
#include "external.h"
#include "AutoInp.h"
#include "MstRw.h"

#include "mainfrm.h"
#include "BmnChk.h"

// 定型仕訳
extern CMKRDSub Mkrd;

// 入力設定項目選択
extern struct _AUTOSEL *pAUTOSEL;

extern CDBSyohi* pDBsy;

extern CBmnHaniCheck	BmChk;

extern
BOOL IsKaribarai( DWORD kncd );

// CDBbaseView
CSelectData	CDBbaseView::m_Seldata;
BOOL		CDBbaseView::m_bSelDataInit = FALSE;
CVPrint		CDBbaseView::m_Vprn;
NPRT_PAR	CDBbaseView::m_Pset;

int	CDBbaseView::m_ImgPrintFlg = PRINT_NONE;
int	CDBbaseView::m_End_PrintDlg = 0;


IMPLEMENT_DYNCREATE(CDBbaseView, ICSFormView)

CDBbaseView::CDBbaseView(UINT id)
	: ICSFormView(id), flgPen( FALSE )
{
	BRNTKY_sel = FALSE;		//枝番摘要を選択画面に表示
	bKamokuCode = FALSE;	//科目コードを画面に表示
	m_SelDispPN = -1;		//現在選択画面に表示しているモード
	m_pSyzInp = NULL;		//消費税画面
	m_pHusenInp = NULL;		//付箋入力画面
	m_pHjiyuInp = NULL;		//変動事由画面

	m_pSubDtInp = NULL;
	m_bNotNumber = FALSE;
	m_SET_SYZINP_PN = -1;

	m_vwColor = ::GetSysColor( COLOR_3DFACE ); 

	m_pSelWnd = NULL;	//選択画面

	m_ImgPrintFlg = PRINT_NONE;
}

CDBbaseView::~CDBbaseView()
{
	SyzInpDelete();
	HusenInpDelete();
	HjiyuInpDelete();
}

void CDBbaseView::DoDataExchange(CDataExchange* pDX)
{
	ICSFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ICSSELCTRL1, m_selfix8);
	DDX_Control(pDX, IDC_ICSSELCTRL2, m_selval32);
	DDX_Control(pDX, IDC_SELGUIDE, m_selGuid);
}

BEGIN_MESSAGE_MAP(CDBbaseView, ICSFormView)
END_MESSAGE_MAP()


// CDBbaseView 診断

#ifdef _DEBUG
void CDBbaseView::AssertValid() const
{
	ICSFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDBbaseView::Dump(CDumpContext& dc) const
{
	ICSFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDBbaseView メッセージ ハンドラ
void CDBbaseView::OnInitialUpdate()
{
	ICSFormView::OnInitialUpdate();

	// TODO: ここにOnInitialUpdate特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	GetWindowRect( &m_original_rect );	//オリジナルのサイズゲット
	GetDlgItem( IDC_ICSSELCTRL1 )->GetClientRect( &m_selfix8_rect );	//オリジナルのサイズゲット

	m_selGuid.ModifyStyle( 0, WS_CLIPSIBLINGS );
}

//
//	縦長画面のとき、選択部分を 少し広めにする
//
void CDBbaseView::SelectDispAdjust()
{
/*---------------------------*/
	int rsl = ResolutionRatio();

	if( rsl == RSL_LONGERHANWIDE /*psz->cy >= 1024*/ ) {
		int h_plus = 18;
		CRect rc, rect2;
		GetDlgItem( IDC_ICSSELCTRL1 )->GetWindowRect( rc );
		ScreenToClient( rc );
		GetDlgItem( IDC_ICSSELCTRL1 )->MoveWindow( rc.left, rc.top, rc.Width(), rc.Height()+h_plus );

		GetDlgItem( IDC_ICSSELCTRL2 )->GetWindowRect( rc );
		ScreenToClient( rc );
		GetDlgItem( IDC_ICSSELCTRL2 )->MoveWindow( rc.left, rc.top, rc.Width(), rc.Height()+h_plus );

		int h_mv = rc.top + rc.Height()+h_plus;
		int id;

		CWnd*	pwnd	=	GetWindow( GW_CHILD );

		while( pwnd ) {
			id = pwnd->GetDlgCtrlID();

			if( id != IDC_ICSSELCTRL1 && id != IDC_ICSSELCTRL2 ) {
				pwnd->GetWindowRect( rc );
				ScreenToClient( rc );
				rc.top += h_plus;
				rc.bottom += h_plus;
				pwnd->MoveWindow( rc.left, rc.top, rc.Width(), rc.Height() );
			}
			pwnd	=	pwnd->GetWindow( GW_HWNDNEXT );
		}
	}
/*---------------------------*/
}


// m_Seldata より呼ばれる関数
//	固定科目部分に内容をセットする
int CALLBACK CDBbaseView::SetSelectFix8( _SelTable* pSel, SelSetProcPar* pSetProc, int cnt, CWnd* lp )
{
	BOOL bNumber;
	CDBbaseView* pBvw;
	pBvw = (CDBbaseView*)lp;

	if( pBvw->m_Seldata.GetKmDispMode() == SL_KMK_CODE )
		bNumber = FALSE;
	else
		bNumber = TRUE;

	CICSSel*	psel8 = NULL;
	if( pAUTOSEL->BLKSEL_OPT ) {
		if( pBvw->m_pSelWnd != NULL ) {
			psel8 = &pBvw->m_pSelWnd->m_selfix8;
		}
		else {
			psel8 = &pBvw->m_selfix8;
		}
	}
	else {
		psel8 = &pBvw->m_selfix8;
	}

	if( psel8 == NULL )	return 0;

	if( psel8->GetNumber() != bNumber )
		psel8->SetNumber( bNumber );

	psel8->SetData( (long)pSel );

#ifdef OLD_CLOSE
	if( pBvw->m_selfix8.GetNumber() != bNumber )
		pBvw->m_selfix8.SetNumber( bNumber );

	pBvw->m_selfix8.SetData( (long)pSel );
#endif

	return 0;
}


// m_Seldata より呼ばれる関数
//	科目選択部分に内容をセットする
int CALLBACK CDBbaseView::SetSelectSel32( _SelTable* pSel, SelSetProcPar* pSetProc, int cnt, CWnd* lp )
{	
	BOOL bNumber;
	CDBbaseView* pBvw;
	pBvw = (CDBbaseView*)lp;

	switch( pSetProc->sp_nowpn ) {
	case SL_KAMOKU_PN:
	case SL_KMKANA_PN:
	case SL_KMINPCD_PN:
		if( pBvw->m_Seldata.GetKmDispMode() == SL_KMK_CODE )
			bNumber = FALSE;
		else
			bNumber = TRUE;
		break;
	case SL_BUMON_PN:
	case SL_BMNKANA_PN:
	case SL_KOJI_PN:
	case SL_KOJIKANA_PN:
		if( pAUTOSEL->BMNSEL_OPT == OPT_NONOPT )
			bNumber = FALSE;
		else
			bNumber = TRUE;
		break;

	default:
		bNumber = TRUE;
		if( pBvw->m_bNotNumber )
			bNumber = FALSE;
		break;
	}

	BOOL bSelWnd = FALSE;
	CICSSel*	psel32 = NULL;

	if( pAUTOSEL->BLKSEL_OPT ) {
		if( pBvw->m_pSelWnd != NULL ) {
			bSelWnd = TRUE;
			psel32 = &pBvw->m_pSelWnd->m_selval32;
		}
		else {
			psel32 = &pBvw->m_selval32;
		}
	}
	else {
		psel32 = &pBvw->m_selval32;
	}

	if( psel32->GetNumber() != bNumber )
		psel32->SetNumber( bNumber );

	if( pSetProc->sp_nowpn == SL_BRNTKY_PN ) {
		// 選択画面の色
		COLORREF txt = pBvw->m_Seldata.GetSel32TextColor();
		COLORREF back = pBvw->m_Seldata.GetSel32BackColor();

		psel32->SetForeColor( txt );	// 文字色
		psel32->SetBackColor( back );	// 背景色

		pBvw->BRNTKY_sel = 1;
	}
	else {
		if( pSetProc->sp_prevpn == SL_BRNTKY_PN ) {
			// 選択画面の色を元に戻す
			if( bSelWnd ) {
				pBvw->m_pSelWnd->enable_fixval( pBvw->fixvalcnd() );	// 元背景色強制描画のため
			}
			else {
				pBvw->enable_fixval( pBvw->fixvalcnd() );	// 元背景色強制描画のため
			}
		}
		else {
			if( psel32->GetBackColor() == BRTEK_BackCOL ) { //枝番の背景色が残る場合があった。
				if( bSelWnd ) {
					pBvw->m_pSelWnd->enable_fixval( pBvw->fixvalcnd() );
				}
				else {
					pBvw->enable_fixval( pBvw->fixvalcnd() );
				}
			}
		}
		pBvw->BRNTKY_sel = 0;
	}

	pBvw->m_SelDispPN = pSetProc->sp_nowpn;
	CString	stitle = pSetProc->seljob;

//	MyTrace(">> SetSelectSel32 pBvw->m_SelDispPN %d\n", pBvw->m_SelDispPN);

	// 03.16 /11
	if( pSetProc->sp_nowpn == SL_BUMON_PN || pSetProc->sp_nowpn == SL_KOJI_PN ||
		pSetProc->sp_nowpn == SL_BMNKANA_PN || pSetProc->sp_nowpn == SL_KOJIKANA_PN ) {
		if( (pAUTOSEL->BMNSEL_OPT == OPT_BMNSEL) )
			stitle += _T( "\r\n (画面番号優先) " );
		else {
			if( pSetProc->sp_nowpn == SL_BUMON_PN || pSetProc->sp_nowpn == SL_BMNKANA_PN )
				stitle += _T( "\r\n (部門番号優先) " );
			else
				stitle += _T( "\r\n (工事番号優先) " );
		}

		if( pSetProc->sp_nowpn == SL_KOJI_PN || pSetProc->sp_nowpn == SL_KOJIKANA_PN ) {
			stitle += _T( "\r\n [*]は完成工事 " );
		}
	}

	if( bSelWnd ) {
		pBvw->m_pSelWnd->set_valtitle( (char*)(LPCTSTR)stitle );
	}
	else {
		pBvw->set_valtitle( (char*)(LPCTSTR)stitle );
	}

	psel32->SetData( (long)pSel );

//	MyTrace( ">> SetSelectSel32 SetData %d, [%s]\n", pSel->number, pSel->Data );

//MyTrace( ">> SetSelectSel32 BRNTKY %d\n", pBvw->BRNTKY_sel );

	return 0;
}


// 固定科目選択画面状態ＳＷ
BOOL CDBbaseView::fix8cnd()
{
	return fix8sg;
}

void CDBbaseView::fix8cnd( BOOL sg )
{
	fix8sg = sg;
}

// 選択画面表示色状態
BOOL CDBbaseView::fixvalcnd()
{
	return fixvalsg;
}

void CDBbaseView::fixvalcnd( BOOL sg )
{
	fixvalsg = sg;
}


//
// 固定項目の表示
//	BOOL sg:	TRUE:固定科目の表示, FALSE:固定科目の表示しない(選択タイトルを表示)
//
void CDBbaseView::fix8disp( BOOL sg )
{
	if( sg )
	{
		// 固定科目選択を表示する
		enable_fix8( sg );
		// 選択タイトル≪部門コード≫・≪消費税≫・≪科目対応摘要≫など表示はしない
		valtitle( !sg );
	}
	else
	{
		// 選択タイトル≪部門コード≫・≪消費税≫・≪科目対応摘要≫などの表示
		valtitle( !sg );
		// 固定科目選択は非表示
		enable_fix8( sg );
	}

	fix8cnd( sg );

}

// 固定科目選択のディセーブル／イネーブル
void CDBbaseView::enable_fix8( BOOL sg )
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

		m_selfix8.ShowWindow(sg ? SW_SHOW : SW_HIDE);

	//	pfix8->SetWindowPos( 0, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE );
	//	pfix8->UpdateWindow();
	}

}

//
// 選択画面の表示色
void CDBbaseView::enable_fixval( BOOL sg )
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
void CDBbaseView::set_valtitle( char *msg )
{
	puts( IDC_VALTITLE, msg, CICSDisp );
}

// 選択タイトル表示のディセーブル／イネーブル
void CDBbaseView::valtitle( BOOL sg )
{
	CWnd *ttlbaseorg,*ttlorg;
	CICSSel *pfix8,*pSel32;
	RECT rect8,rect32,newrect,newrect2;
	LONG v_h, v_w;
	WINDOWPLACEMENT place;

	VERIFY(ttlbaseorg = (CICSDisp*)GetDlgItem( IDC_VALTITLEBASE ));
	VERIFY(ttlorg = (CICSDisp*)GetDlgItem( IDC_VALTITLE ));

/*
#ifdef	_ICSPEN_	// 01.20 /01
	//Penの時は表示無し
	if( IsPen() ) {
		ttlbaseorg->MoveWindow( 0,0,0,0 );
		ttlorg->MoveWindow( 0,0,0,0 );
		return;
	}
#endif	//end of _ICSPEN_
*/
	// New Size
	if( sg == TRUE )
	{
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
		newrect.top = 0;
		newrect.left = 0;
		newrect.bottom = 0;
		newrect.right = 0;

		ttlbaseorg->SetWindowPos( 0, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE );
		ttlorg->SetWindowPos( 0, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_NOZORDER|SWP_NOACTIVATE );
	}

//	ttlbaseorg->UpdateWindow();
//	ttlorg->UpdateWindow();
}


//----------------------------------------------------------
//   初期化処理(選択画面＆消費税画面)
//
//-----------------------------------------------------------
int CDBbaseView::InitWork( BOOL syuse ) 
{
	CWnd* org;

//#ifdef KEEP
	GetWindowRect( &m_original_rect );	//オリジナルのサイズゲット
	GetDlgItem( IDC_ICSSELCTRL1 )->GetClientRect( &m_selfix8_rect );	//オリジナルのサイズゲット
//#endif

	// 選択タイトル背景色、文字色
#ifdef ICSCOLOR_
	ICSColorInfo ci;
	((ICSWinApp*)AfxGetApp())->GetStanderdColor( ci );

	if( ci.m_swOnOff > 0 ) {
		((CICSDisp*)GetDlgItem( IDC_VALTITLEBASE ))->SetBackColor( ci.FixKmkBackColor() );
		((CICSDisp*)GetDlgItem( IDC_VALTITLEBASE ))->SetForeColor( ci.FixKmkTextColor() );

		((CICSDisp*)GetDlgItem( IDC_VALTITLE ))->SetBackColor( ci.FixKmkBackColor() );
		((CICSDisp*)GetDlgItem( IDC_VALTITLE ))->SetForeColor( ci.PageKmkTextColor() );
	}
	else {
		((CICSDisp*)GetDlgItem( IDC_VALTITLEBASE ))->SetBackColor( RGB_FIX8_BACK );
		((CICSDisp*)GetDlgItem( IDC_VALTITLEBASE ))->SetForeColor( RGB_BLACK );
		((CICSDisp*)GetDlgItem( IDC_VALTITLE ))->SetBackColor( RGB_FIX8_BACK );
		((CICSDisp*)GetDlgItem( IDC_VALTITLE ))->SetForeColor( RGB_VAL32_FORE );
	}
#endif

	// 選択タイトル用ＤＩＳＰコントロールのサイズをゼロ（見えなくする）に
	WINDOWPLACEMENT place;
	RECT rct;
	VERIFY(org = GetDlgItem( IDC_ICSSELCTRL1 ));
	org->GetWindowPlacement( &place );
	rct = place.rcNormalPosition;

	VERIFY(org = GetDlgItem( IDC_VALTITLEBASE));
	org->SetWindowPos( 0, rct.left, rct.top, 0, 0, SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOSIZE );

	VERIFY(org = GetDlgItem( IDC_VALTITLE));
	org->SetWindowPos( 0, rct.left, rct.top, 0, 0, SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOSIZE );

	ASSERT( pAUTOSEL != NULL );

	if( ! m_bSelDataInit ) {
		m_Seldata.Init( pDBzm, this, (SELECTDATAPROC)SetSelectFix8, (SELECTDATAPROC)SetSelectSel32 );

		if( pAUTOSEL->KMNO_OPT == OPT_KMNO ) {
			// 科目コード表示
			m_Seldata.SetKmDispMode( SL_KMK_CODE );
			bKamokuCode = TRUE;
		}
		else {
			m_Seldata.SetKmDispMode( SL_KMK_NUM );
			bKamokuCode = FALSE;
		}

		int	sw = 0;

		if( pAUTOSEL->BRTEK_OPT == OPT_DSP )	// 枝番名称表示
			sw |= BT_NOMALBRN_TEK;
		if( pAUTOSEL->SYGBRN_OPT == OPT_DSP )	// 諸口枝番表示
			sw |= BT_SYOGTBRN_TEK;

	//	sw |= SL_BRNTKY_ONLY;
		m_Seldata.SetBrnTkyMode( sw );

		// 仕訳摘要表示優先
		sw = 0;
		sw = (pAUTOSEL->SWKTEK_OPT == OPT_SWKTEK ) ? SL_TKY_SIWAKE : SL_TKY_RENSO;
		m_Seldata.SetTekiyoMode( sw );

		//カナ検索時のカナ順表示
		m_Seldata.SetKanaDspMode( (pAUTOSEL->KANADSP_OPT == OPT_KANAJUN) ? SL_KJUN_DSP : SL_NORMAL_MODE ); 
		//科目ページ保持
		m_Seldata.SetKamokuPageHold( pAUTOSEL->KMPG_OPT == OPT_KMPG ); 

		DWORD opt = 0;
		if( pAUTOSEL->SWTKYD_OPT == OPT_SWTKYD )	opt |= 0x01;
		if( pAUTOSEL->BMNTKBR_OPT == OPT_CHKON )	opt |= 0x02;
		if( pAUTOSEL->RTSWKKANA_OPT == OPT_KANAJUN )	opt |= 0x04;

//MyTrace( "baseview m_Seldata.SetSelOptMode = %04x\n", opt );

		m_Seldata.SetSelOptMode( opt );

		int mode = SL_NORMAL_MODE;
		if( pAUTOSEL->BRNKANA_OPT == OPT_KANAJUN ) {
			mode = SL_KJUN_DSP;
		}
		m_Seldata.SetBrnSelDispMode(mode);

		m_bSelDataInit = TRUE;
	}

	// 消費税画面作成
	if( syuse )	SyzInpMake();
	// 付箋入力画面作成
	HusenInpMake();
	// 変動事由入力画面作成
	HjiyuInpMake();

	fix8disp( TRUE );

	return 0;
}


//----------------------------------------------------------
//   終了処理(諸口枝番で使用の分析表レコードクローズ)
//
//-----------------------------------------------------------
void CDBbaseView::CloseWork() 
{
	m_Seldata.Close();
}


// 選択画面の設定をUpdate
//
void CDBbaseView::UpdateSelectPar() 
{

	if( pAUTOSEL->KMNO_OPT == OPT_KMNO ) {
		// 科目コード表示
		m_Seldata.SetKmDispMode( SL_KMK_CODE );
		bKamokuCode = TRUE;
	}
	else {
		m_Seldata.SetKmDispMode( SL_KMK_NUM );
		bKamokuCode = FALSE;
	}

	int	sw = 0;
	sw = m_Seldata.GetBrnTkyMode();
	sw &= ~(BT_NOMALBRN_TEK|BT_SYOGTBRN_TEK);

	if( pAUTOSEL->BRTEK_OPT == OPT_DSP )	// 枝番名称表示
		sw |= BT_NOMALBRN_TEK;
	if( pAUTOSEL->SYGBRN_OPT == OPT_DSP )	// 諸口枝番表示
		sw |= BT_SYOGTBRN_TEK;

	m_Seldata.SetBrnTkyMode( sw );

	// 仕訳摘要表示優先
	sw = 0;
	sw = (pAUTOSEL->SWKTEK_OPT == OPT_SWKTEK ) ? SL_TKY_SIWAKE : SL_TKY_RENSO;
	m_Seldata.SetTekiyoMode( sw );

	//カナ検索時のカナ順表示
	m_Seldata.SetKanaDspMode( (pAUTOSEL->KANADSP_OPT == OPT_KANAJUN) ? SL_KJUN_DSP : SL_NORMAL_MODE ); 
	//科目ページ保持
	m_Seldata.SetKamokuPageHold( pAUTOSEL->KMPG_OPT == OPT_KMPG ); 

	SelPar par;
	par.sel_pn = SL_KAMOKU_PN;
	m_Seldata.SelectJob( &par, -1 );

//	m_Seldata.SelectUpdate();
}

//----------------------------------------------------------
//	選択モジュールで扱う ウィンドウの変更
//-----------------------------------------------------------
void CDBbaseView::SetSelectWindow() 
{
	m_Seldata.SetWindow( this );
}



//----------------------------------------------------------
//	選択モジュールでの表示リセット
//-----------------------------------------------------------
void CDBbaseView::ResetSelect() 
{
	m_Seldata.KamokuRestrict( pDBzm->m_dwIndSgn );

	// 公益用
	m_Seldata.NpKamokuRestrict( pDBzm->m_Jgykcd, pDBzm->m_Knrkcd );

//	SelPar par;
//	par.sel_pn = SL_RESET;
//
//	m_Seldata.SelectJob( &par, -1 );
}

//----------------------------------------------------------
//	摘要五十音文字列チェック
//	return	TRUE ---- カナ検索
//			FALSE --- 数字文字列なので摘要番号指定を行う
//-----------------------------------------------------------
BOOL CDBbaseView::tky_50kana_chk( char *data )
{
	int n;
	unsigned char *p;
	char str[50], fmt[10];

	strcpy_s( str, sizeof str, (char *)data );
	p = (unsigned char *)str;

	if( !isdigitstr( (char*)p ) )	// 数字文字列でない
	{
		// ASCII to ANK
		for( ; *p ; ++p )
		{
			if( (unsigned char)*p == (unsigned char)'ｰ' )
				*p = '-';
			else if( (unsigned char)*p == (unsigned char)' ' )
				*p = ' ';
		}

		sprintf_s( fmt, sizeof fmt, "%%.%ds", sizeof( save_50kana ) - 1 );
		n = sprintf_s( m_tky_50kana, sizeof m_tky_50kana, fmt, str );

		if( strlen( m_tky_50kana ) > 4 ) {
			m_tky_50kana[4] = '\0';
		}

		return TRUE;
	}
	return FALSE;
}

#ifndef _SLIP_ACCOUNTS_
//----------------------------------------------------------
//	摘要選択
//	CDBipTKREC* ptkrec  摘要レコード
//	char *data ........ 摘要番号文字列
//	return	0 ---- 摘要レコード あり
//		   -1 ---- 摘要レコード なし
//-----------------------------------------------------------
int CDBbaseView::tky_select( CDBipTKREC* ptkrec, char *data )
{
	SelResult srt;
	srt.sel_pn = SL_TKY_PN;

	if( ! isdigitstr( data ) )	//数字文字列
		return -1;

	if( m_Seldata.GetSelResult( &srt, data ) ) {

		pDBzm->tkrec->MoveFirst();

		int sgn = 0;
		if( pDBzm->tkrec->st != -1 ) {
			do {
				if( srt.tky_code == pDBzm->tkrec->tkcod ) {
					ptkrec->m_tkcod = pDBzm->tkrec->tkcod;
					ptkrec->m_tkana = pDBzm->tkrec->tkana;
					ptkrec->m_tksgn.Copy( pDBzm->tkrec->tksgn );
					ptkrec->m_tkname = pDBzm->tkrec->tkname;
					// 後ろスペースをカット
					LPSTR	p = ptkrec->m_tkname.GetBuffer(256);
					int n = kjlen( p, TKYO_LEN );
					*(p + n) = '\0';
					ptkrec->m_tkname.ReleaseBuffer();

					ptkrec->m_tkari = pDBzm->tkrec->tkari;
					ptkrec->m_tkasi = pDBzm->tkrec->tkasi;
					sgn = 1;
					break;
				}
			} while( pDBzm->tkrec->MoveNext() == 0 );
		}
		return (sgn) ? 0 : -1;
	}

	return -1;

}
#else
//----------------------------------------------------------
//	摘要選択
//	CDBipTKREC* ptkrec  摘要レコード
//	char *data ........ 摘要番号文字列
//	return	0 ---- 摘要レコード あり
//		   -1 ---- 摘要レコード なし
//-----------------------------------------------------------
int CDBbaseView::tky_select( CDBipTKREC* ptkrec, char *data )
{
	SelResult srt;
	if( m_SelDispPN != SL_SLIP_TKY_PN ){
		srt.sel_pn = SL_TKY_PN;
	}
	else{
		srt.sel_pn = SL_SLIP_TKY_PN;
	}

	if( ! isdigitstr( data ) )	//数字文字列
		return -1;

	if( m_Seldata.GetSelResult( &srt, data ) ) {

		if( m_SelDispPN != SL_SLIP_TKY_PN ){

			pDBzm->tkrec->MoveFirst();

			int tkyo_len = TKYO_LEN;
			if( ! IsMasterType(MST_TKY20) )	tkyo_len = (TKYO_LEN/2);

			int sgn = 0;
			if( pDBzm->tkrec->st != -1 ) {
				do {
					if( srt.tky_code == pDBzm->tkrec->tkcod ) {
						ptkrec->m_tkcod = pDBzm->tkrec->tkcod;
						ptkrec->m_tkana = pDBzm->tkrec->tkana;
						ptkrec->m_tksgn.Copy( pDBzm->tkrec->tksgn );
						ptkrec->m_tkname = pDBzm->tkrec->tkname;
						// 後ろスペースをカット
						LPSTR	p = ptkrec->m_tkname.GetBuffer(256);
						int n = kjlen( p, tkyo_len );
						*(p + n) = '\0';
						ptkrec->m_tkname.ReleaseBuffer();

						ptkrec->m_tkari = pDBzm->tkrec->tkari;
						ptkrec->m_tkasi = pDBzm->tkrec->tkasi;
						if( pDBzm->tkrec->dsign.GetSize() == SIZE_DSIGN ) {
							for( int i = 0; i < SIZE_DSIGN; i++ ) {
							ptkrec->m_tkdsgn[i] = pDBzm->tkrec->dsign[i];
						}
						}
						CString tmpInvno = pDBzm->tkrec->invno;
						tmpInvno.Replace( _T( "T" ), _T( "" ));
						if( tmpInvno.GetLength() < 14 ) {
							sprintf_s( ptkrec->m_tkinvno, sizeof( ptkrec->m_tkinvno ), _T( "%s" ), tmpInvno );
						}

						sgn = 1;
						break;
					}
				} while( pDBzm->tkrec->MoveNext() == 0 );
			}
			return (sgn) ? 0 : -1;
		}
		else{
			ptkrec->m_tkcod = srt.tky_code;
			ptkrec->m_tkname = srt.tky;
			return 0;
		}
	}

	return -1;

}
#endif



//消費税入力画面作成＆表示
void CDBbaseView::SyzInpMake()
{
	if (m_pSyzInp == NULL)
	{
		if( !(m_pSyzInp = new CSyzInp()) )
		{
			ermset( ERROR_MEMORY, "消費税入力オブジェクトが作成できません。");
			return;
		}
	}

	// インボイス対応
	if( m_InpMode == INPMODE_DENP ) {
		m_pSyzInp->SetDenpMode();
	}

	if( m_pSyzInp->SetPara( pDBzm, pDBsy, this ) != 0 ) {
		ermset( ERROR_MEMORY, "消費税入力オブジェクトが作成できません。");
		return;
	}
}

//消費税入力画面 完全に 削除
void CDBbaseView::SyzInpDelete()
{
	if (m_pSyzInp ) {
		delete m_pSyzInp;
		m_pSyzInp = NULL;
	}
}


//消費税入力画面 を非表示
void CDBbaseView::Destroy_SyzInp()
{
	if (m_pSyzInp ) {
		if( m_pSyzInp->IsDisplay() ) {
			m_pSyzInp->DialogOFF();
			//ダイアログOFFに呼びたい関数
			Destroy_SyzInpSubFunc();
		}
	}
}

// 消費税名称を画面にセット
void CDBbaseView::DispSyzIchiran( WPARAM wParam, LPARAM lParam )
{
	DWORD i;
	CString *str = (CString*)lParam;

#define SYZSEL_MAX	8*4

static struct _SelTable SelVal32[SYZSEL_MAX];
static SelSetProcPar	spar;

	spar.sp_prevpn = m_SelDispPN;
	spar.sp_nowpn = BVW_SYZ_PN;
	WORD dspSign = HIWORD(wParam);
	if( dspSign & 0x01 ) {
		strcpy_s(spar.seljob, sizeof spar.seljob, "《登録番号》");
	}
	else {
		strcpy_s(spar.seljob, sizeof spar.seljob, "《消費税》");
	}
	int dspCnt = LOWORD(wParam);

	for( i = 0 ; i < SYZSEL_MAX ; ++i )
	{
		// 表示
		SelVal32[i].Data = i < dspCnt ? *(str+i) : "";
	//	SelVal32[i].number = i + (isEntrySys() ? 9 : 1);
		SelVal32[i].number = i + 1;
	}

	SetSelectSel32( SelVal32, &spar, SYZSEL_MAX, this );

}

//付箋入力画面作成＆表示
void CDBbaseView::HusenInpMake()
{
	if (m_pHusenInp == NULL)
	{
		if( !(m_pHusenInp = new CHusenInp()) )
		{
			ermset( ERROR_MEMORY, "付箋入力オブジェクトが作成できません。");
			return;
		}
	}

	if( m_pHusenInp->SetPara( pDBzm, this ) != 0 ) {
		ermset( ERROR_MEMORY, "付箋入力オブジェクトが作成できません。");
		return;
	}
}

//付箋入力画面 完全に 削除
void CDBbaseView::HusenInpDelete()
{
	if (m_pHusenInp ) {
		delete m_pHusenInp;
		m_pHusenInp = NULL;
	}
}


//消費税入力画面 を非表示
void CDBbaseView::Destroy_HusenInp()
{
	if (m_pHusenInp ) {
		if( m_pHusenInp->IsDisplay() ) {
			m_pHusenInp->DialogOFF();
		}
	}
}


//変動事由入力画面作成＆表示
void CDBbaseView::HjiyuInpMake()
{
	if (m_pHjiyuInp == NULL)
	{
		if( !(m_pHjiyuInp = new CHjiyuInp()) )
		{
			ermset( ERROR_MEMORY, "変動事由入力オブジェクトが作成できません。");
			return;
		}
	}

	if( m_pHjiyuInp->SetPara( pDBzm, this ) != 0 ) {
		ermset( ERROR_MEMORY, "変動事由入力オブジェクトが作成できません。");
		return;
	}
}

//変動事由入力画面 完全に 削除
void CDBbaseView::HjiyuInpDelete()
{
	if (m_pHjiyuInp ) {
		delete m_pHjiyuInp;
		m_pHjiyuInp = NULL;
	}
}


//変動事由入力画面 を非表示
void CDBbaseView::Destroy_HjiyuInp()
{
	if (m_pHjiyuInp ) {
		if( m_pHjiyuInp->IsDisplay() ) {
			m_pHjiyuInp->DialogOFF();
		}
	}
}


//	サブ入力画面を作成
void CDBbaseView::SubDataInpMake()
{
	if (m_pSubDtInp == NULL)
	{
		if( !(m_pSubDtInp = new CSubDataInp()) )
		{
			ermset( ERROR_MEMORY, "サブ入力オブジェクトが作成できません。");
			return;
		}
	}

	if( m_pSubDtInp->SetPara( pDBzm, this ) != 0 ) {
		ermset( ERROR_MEMORY, "サブ入力オブジェクトが作成できません。");
		return;
	}
}

//	サブ入力画面を 完全に削除
void CDBbaseView::SubDataInpDelete()
{
	if (m_pSubDtInp ) {
		delete m_pSubDtInp;
		m_pSubDtInp = NULL;
	}
}

//	サブ入力画面を 非表示
void CDBbaseView::Destroy_SubDataInp()
{
	if (m_pSubDtInp ) {
		if( m_pSubDtInp->IsDisplay() ) {
			m_pSubDtInp->DialogOFF();
		}
	}
}


//------------------------------------------------------------------
//    科目選択チェック
//
//  struct _KamokuData& ans ..... チェック結果格納
//  struct _KamokuData& check ... チェック用 
//  int _dcsw ......... 借／貸
//  BOOL bSelClick .... 選択欄をクリックで選んだか？
//	return TRUE ------> 該当科目があった。
//-------------------------------------------------------------------
BOOL CDBbaseView::InputItemCheck(struct _KamokuData& ans, struct _KamokuData& check, int _dcsw, BOOL bSelClick/*=FALSE*/ )
{

	return m_Seldata.InputItemCheck( ans, check, _dcsw, bSelClick );
}


//------------------------------------------------------------------
//    LastData をGet
//
//------------------------------------------------------------------
void CDBbaseView::LastDataGet()
{
}

//------------------------------------------------------------------
//    UCOMをセット
//
//------------------------------------------------------------------
void CDBbaseView::ForkUcomSet()
{
}

//------------------------------------------------------------------
//    画面再表示
//
//------------------------------------------------------------------
int CDBbaseView::ReloadView()
{

	return 0;
}

//------------------------------------------------------------------
//    タブで切り替えられたときの直前のビューの後処理の関数
//
//------------------------------------------------------------------
int CDBbaseView::TabClickChangedPrevView()
{

	return 0;
}

//------------------------------------------------------------------
//	選択科目表示
//------------------------------------------------------------------
long CDBbaseView::SelItemDsp( long nChar )
{
	SelPar par;

	switch( m_SelDispPN ) {
	case SL_KAMOKU_PN:
	case SL_KMKANA_PN:
	case SL_BRNTKY_PN:
	case SL_KMINPCD_PN:	// '09.03.11
		par.sel_pn = m_SelDispPN;
		m_Seldata.SelectJob( &par, nChar );

		switch( nChar ) {
		case VK_PRIOR: case VK_NEXT:
		case VK_HOME:
			nChar = 0;
			break;
		}
		break;
	}

	return nChar;
}


// 枝番摘要選択処理
int CDBbaseView::BRNTKY_select( int bmn, CString& code, char* kana/*=NULL*/, BOOL bDisp/*=FALSE*/ )
{
	SelPar	par;

	par.sel_pn = SL_BRNTKY_PN;
	par.sel_flg = bDisp ? 1 : 0;
	par.brn_bmn = bmn;
	par.tkybrn = code;
	par.brn_cd = -1;

	if( ! BmChk.BmnHaniCheck( bmn, code ) ) {
		par.brn_bmn = -1;
	}

	::ZeroMemory( par.brn_kana, sizeof par.brn_kana );
	if( kana != NULL )
		strcpy_s( par.brn_kana, sizeof par.brn_kana, kana );

	m_Seldata.SelectJob( &par, -1 );

	return BRNTKY_sel;
}


// 選択番号より部門コードを仕訳へ
//
BOOL CDBbaseView::bmncode_todata( CDBINPDataRec* data, int number, int dc_sw, int hchk/*=0*/, CString* bmnname/*NULL*/ )
{
	BOOL ret = FALSE;
	char buf[64];

	sprintf_s( buf, sizeof buf, "%d", number );
	SelResult srt;
	srt.sel_pn = SL_BUMON_PN;

	if( m_Seldata.GetSelResult( &srt, buf ) ) {

		if( hchk ) {	//範囲チェック
			switch( dc_sw ) {
			case 0:
				if( BmChk.BmnHaniCheck( srt.bmn, data->m_dbt ) ) {
					data->m_dbmn = srt.bmn;
					ret = TRUE;
				}
				break;
			case 1:
				if( BmChk.BmnHaniCheck( srt.bmn, data->m_cre ) ) {
					data->m_cbmn = srt.bmn;
					ret = TRUE;
				}
				break;
			default:
				if( BmChk.BmnHaniCheck( srt.bmn, data->m_dbt ) ) {
					data->m_dbmn = srt.bmn;
					ret = TRUE;
				}
				if( BmChk.BmnHaniCheck( srt.bmn, data->m_cre ) ) {
					data->m_cbmn = srt.bmn;
					ret = TRUE;
				}
				break;
			}
		}
		else {
			switch( dc_sw ) {
			case 0:
				data->m_dbmn = srt.bmn;
				break;
			case 1:
				data->m_cbmn = srt.bmn;
				break;
			default:
				data->m_dbmn = srt.bmn;
				data->m_cbmn = srt.bmn;
				break;
			}
			ret = TRUE;
		}

		if( ret ) {
			if( bmnname != NULL ) {
				*bmnname = srt.selname;
			}
		}
	}

	return ret;
}


// 選択番号より工事コードを仕訳へ
//
//
BOOL CDBbaseView::kojicode_todata( CDBINPDataRec* data, CString* kjnam, int number, int dc_sw )
{
	BOOL ret = FALSE;
	char buf[64];

	SelResult srt;
	srt.sel_pn = SL_KOJI_PN;

	sprintf_s( buf, sizeof buf, "%d", number );

	if( m_Seldata.GetSelResult( &srt, buf ) ) {
		srt.kjcode.TrimRight();
		switch( dc_sw ) {
		case 0:
			data->m_dkno = srt.kjcode;
			break;
		case 1:
			data->m_ckno = srt.kjcode;
			break;
		default:
			data->m_dkno = srt.kjcode;
			data->m_ckno = srt.kjcode;
			break;
		}
		// 工事名称
		if( kjnam != NULL ) {
			*kjnam = srt.selname;
		}

		ret = TRUE;
	}

	return ret;
}


//摘要表示欄ページめくり
long CDBbaseView::SelTkyDsp( long nChar )
{

	SelPar par;
	switch( m_SelDispPN ) {
	case SL_TKY_PN:
	case SL_TKYKANA_PN:
	case SL_TKYALL_PN:
#ifdef _SLIP_ACCOUNTS_
	case SL_SLIP_TKY_PN:
#endif
		par.sel_pn = m_SelDispPN;
		m_Seldata.SelectJob( &par, nChar );

		switch( nChar ) {
		case VK_PRIOR: case VK_NEXT:
		case VK_HOME:
			nChar = 0;
			break;
		}
		break;
	}

	return nChar;

}


//部門表示欄ページめくり
long CDBbaseView::SelBmonDsp( long nChar )
{
	SelPar par;
	switch( m_SelDispPN ) {
	case SL_BUMON_PN:
	case SL_BMNKANA_PN:
		par.sel_pn = m_SelDispPN;
		m_Seldata.SelectJob( &par, nChar );

		switch( nChar ) {
		case VK_PRIOR: case VK_NEXT:
		case VK_HOME:
			nChar = 0;
			break;
		}
		break;
	}

	return nChar;
}

// 工事名称 ページめくり
long CDBbaseView::SelKojiDsp( long nChar )
{
	SelPar par;
	switch( m_SelDispPN ) {
	case SL_KOJI_PN:
	case SL_KOJIKANA_PN:
		par.sel_pn = m_SelDispPN;
		m_Seldata.SelectJob( &par, nChar );

		switch( nChar ) {
		case VK_PRIOR: case VK_NEXT:
		case VK_HOME:
			nChar = 0;
			break;
		}
		break;
	}

	return nChar;
}


// 工事コードチェック
BOOL CDBbaseView::Kojicode_Ok( CString& kjcode )
{
	return m_Seldata.Kojicode_Ok( kjcode );
}

// 選択画面に選択
int CDBbaseView::BaseSelectJob( SelPar* par, long nChar/* = -1*/ )
{
	return m_Seldata.SelectJob( par, nChar );
}

// 仮想関数
void CDBbaseView::OnActivateJob(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
}


//------------------------------------------------------------
//	変動事由, 振替科目をセットできる仕訳かどうか？
//
//------------------------------------------------------------
BOOL CDBbaseView::IsEnableHjiyu( CDBINPDataRec *pcrec )
{
	if( pDBzm->zvol->apno < 0x50 ) {

		if( m_Seldata.GetHjiyuCount(0) < 0 ) {
			return FALSE;
		}

		int st = get_khkbntype( pcrec->m_dbt, pcrec->m_cre, pDBzm );
		if( st == -1 )
			return FALSE;

		// 変動事由コードが正しいか？
		CZmGen8		ZmGen;
		int ret = ZmGen.CheckByoinType( pDBzm );
		if( ret == ID_ZMG_H29REV_IRYO_TYPE ) {
			return IsIryoHjiyuCodeRight( pcrec->m_hjiyu, st );
		}
		else {
			return IsHenJiyuCodeRight( pcrec->m_hjiyu, st );
		}
	}
	else {
		if( IsSyafuKaisei( pDBzm->zvol ) ) {
			// 内部取引仕訳は、すべての仕訳にセット可能
			// m_Seldata.IsNaibuData( dbt, cre ) ではキー移動によるカーソルをセットするかの判定に使用
			return TRUE;
		}
		else {
			// 振替科目
			if( ! m_Seldata.IsNPfuridata( pcrec->m_dbt, pcrec->m_cre ) )
				return FALSE;

			return	m_Seldata.IsNPfuricode( pcrec->m_hjiyu );
		}
	}
}


// 変動事由か、振替科目[公益]かのチェック
BOOL CDBbaseView::IsHjiyuOrNPfuri( LPCTSTR dbt, LPCTSTR cre )
{
	BOOL ret = FALSE;

	if( pDBzm->zvol->apno < 0x50 ) {
		// 変動事由
		if( get_khkbntype( dbt, cre, pDBzm ) >= 0 ) {
			ret = TRUE;
		}
	}
	else {
		if( IsSyafuKaisei( pDBzm->zvol ) ) {	//社会福祉改正
			DWORD dwCode, dwGkcd;

			dwCode = _atoh( (LPSTR)dbt );
			dwGkcd = (dwCode&0xffffff00);
			hl_rev( &dwGkcd, 4 );

		//	DBGKREC* pGk = pDBzm->get_gksrc( dwGkcd );
			DBGKREC* pGk = pDBzm->PjisToGkrec( dwGkcd );

			if( pGk != NULL ) {
				if( (pGk->gksgn& 0x80) ) {
					ret = TRUE;
				}
			}

			if( ! ret) {
				dwCode = _atoh( (LPSTR)cre );
				dwGkcd = (dwCode&0xffffff00);
				hl_rev( &dwGkcd, 4 );
				
		//		pGk = pDBzm->get_gksrc( dwGkcd );
				pGk = pDBzm->PjisToGkrec( dwGkcd );

				if( pGk != NULL ) {
					if( (pGk->gksgn& 0x80) ) {
						ret = TRUE;
					}
				}
			}
		}
		else {
			// 振替区分
			if( m_Seldata.IsNPfuridata( dbt, cre ) ) {
				ret = TRUE;
			}
		}
	}

	return ret;
}


/******************************************
	工事番号未入力チェック
	dbcr_sg:	-1 のときは、出納帳より

		->	0 ... OK
			1 ... 修正
			-1 .. Error

*******************************************/
BOOL CDBbaseView::koji_noinpcheck( CString& kno, CString& code, int dbcr_sg )
{
	CString	msg;
	int st;

	if( ! M_KOJI )
		return FALSE;

	if( pAUTOSEL->KJIINP_OPT == OPT_KJIINP ) {
		if( m_Seldata.Kojicode_NoInp( kno, code ) ) {

			// 未入力メッセージ
			struct _DBKNREC* pKn;
			if( ( pKn = pDBzm->DB_PjisToKnrec( code ) ) == NULL )
				return -1;

			char* p;
			switch( dbcr_sg ) {
			case 0:		p = "借方";	break;
			case 1:		p = "貸方";	break;
			default:	p = "";	break;
			}

			msg.Format( "《工事番号未入力》\n\n%s科目「%.14s」を使用されていますが、工事番号が未入力です。\r\n\r\n修正しますか？",
							p, pKn->knnam );

			SoundBuzzer();

			st = myICSMessageBox( (LPCSTR)msg, MB_YESNO );

			if( st == IDYES )
				return 1;	// 修正
			else
				return 0;	// OK

		}
	}
	return 0;
}


/******************************************
	完成工事番号 チェック
	dbcr_sg:	-1 のときは、出納帳より

		->	0 ... OK
			1 ... 修正
			-1 .. Error
*******************************************/
BOOL CDBbaseView::koji_kanseicheck( CString& kno, CString& code, int dbcr_sg )
{
	CString	msg;
	int st;

	if( ! M_KOJI )
		return FALSE;

	if( pAUTOSEL->KANKJI_OPT == OPT_KANKJI ) {
		if( m_Seldata.Kojicode_KanChk( kno, code ) ) {

			// 未入力メッセージ
			struct _DBKNREC* pKn;
			if( ( pKn = pDBzm->DB_PjisToKnrec( code ) ) == NULL )
				return -1;

			char* p;
			switch( dbcr_sg ) {
			case 0:		p = "借方";	break;
			case 1:		p = "貸方";	break;
			default:	p = "";	break;
			}

			msg.Format( "《完成工事番号チェック》\n\n%s科目「%.14s」を使用されていますが、\r\n前期以前に完成した工事番号が入力されています。\r\n\r\n修正しますか？",
							p, pKn->knnam );

			SoundBuzzer();

			st = myICSMessageBox( (LPCSTR)msg, MB_YESNO );

			if( st == IDYES )
				return 1;	// 修正
			else
				return 0;	// OK

		}
	}
	return 0;
}



/******************************************
	部門未入力チェック
	dbcr_sg:	-1 のときは、出納帳より

		->	0 ... OK
			1 ... 修正
			-1 .. Error

*******************************************/
BOOL CDBbaseView::bmn_noinpcheck( int bmn, CString& code, int dbcr_sg, CString* pMsg/*=NULL*/)
{
	CString	msg, err;
	int st, cnt;

	if( ! BMON_MST )
		return FALSE;

	if( bmn >= 0 )
		return FALSE;

	DWORD dwCode;

	// 損益のみ部門
	dwCode = _atoh( (LPSTR)(LPCTSTR)code );
	if( (Voln1->bm_isw & 0x10) && dwCode < SONEKICODE )
		return FALSE;

	if( pAUTOSEL->KJIINP_OPT == OPT_KJIINP ) {

		cnt = bmnzan_count( pDBzm->m_database, code, err );
		if( cnt < 0 ) {
			myICSMessageBox(err);
			GetDBDinpMainFrame()->PostMessage( WM_CLOSE, 1 );
			return FALSE;
		}

		if( cnt > 0 ) {
			// 未入力メッセージ
			struct _DBKNREC* pKn;
			if( ( pKn = pDBzm->DB_PjisToKnrec( code ) ) == NULL )
				return -1;

			char* p;
			switch( dbcr_sg ) {
			case 0:		p = "借方";	break;
			case 1:		p = "貸方";	break;
			default:	p = "";	break;
			}

			if( pMsg == NULL ) {
				msg.Format( "《部門未入力》\n\n%s科目「%.14s」を使用されていますが、部門が未入力です。\r\n\r\n修正しますか？",
								p, pKn->knnam );

				SoundBuzzer();

				st = myICSMessageBox( (LPCSTR)msg, MB_YESNO,0,0,this );

				if( st == IDYES )
					return TRUE;	// 修正
				else
					return FALSE;	// OK
			}
			else {
				msg.Format( "%s科目「%.14s」を使用されていますが、部門が未入力です！", p, pKn->knnam );
				*pMsg = msg;
				return TRUE;
			}
		}
	}
	return FALSE;
}


// 選択欄の枝番表示を更新するか？
BOOL CDBbaseView::EdabanDispReload( CString& kmkcd, int bmncd/*=-2*/ )
{
	BOOL bRet = FALSE;

	if( pDBzm->IsEdaAdd() ) {
		pDBzm->IsEdaAdd() = FALSE;
		m_Seldata.BrnTekClear();
		return TRUE;
	}

	//現在処理中の 枝番名称の科目コード
	BRNTKY_PAR bpar;
	bpar = m_Seldata.GetBrnTkyPar();

	if( kmkcd.CompareNoCase( bpar.bt_code ) != 0 ) {
		bRet = TRUE;
	}
	//枝番 → 部門枝番に切り替わった際の情報の更新
	if( BMON_MST && bmncd != -2 ) {
		if( bpar.bt_bmn != bmncd ) {
			bRet = TRUE;
		}
	}
	return bRet;
}



// 内部取引消去 ページめくり
long CDBbaseView::SelNaibuDsp( long nChar )
{
	SelPar par;
	switch( m_SelDispPN ) {
	case SL_NAIBU_PN:
		par.sel_pn = m_SelDispPN;
		m_Seldata.SelectJob( &par, nChar );

		switch( nChar ) {
		case VK_PRIOR: case VK_NEXT:
		case VK_HOME:
			nChar = 0;
			break;
		}
		break;
	}

	return nChar;
}

#ifdef _SLIP_ACCOUNTS_
//----------------------------------------------------------
//	摘要選択
//	CDBipTKREC* ptkrec  摘要レコード
//	char *data ........ 摘要番号文字列
//	return	0 ---- 摘要レコード あり
//		   -1 ---- 摘要レコード なし
//-----------------------------------------------------------
int CDBbaseView::tky_select_in_img( CDBipTKREC* ptkrec, char *data )
{
	SelResult srt;
	srt.sel_pn = SL_SLIP_TKY_PN;

	if( !isdigitstr(data) ){	//数字文字列
		return -1;
	}

	if( m_Seldata.GetSelResult( &srt, data ) ) {

/*		pDBzm->tkrec->MoveFirst();

		int sgn = 0;
		if( pDBzm->tkrec->st != -1 ) {
			do {
				if( srt.tky_code == pDBzm->tkrec->tkcod ) {
					ptkrec->m_tkcod = pDBzm->tkrec->tkcod;
					ptkrec->m_tkana = pDBzm->tkrec->tkana;
					ptkrec->m_tksgn.Copy( pDBzm->tkrec->tksgn );
					ptkrec->m_tkname = pDBzm->tkrec->tkname;
					// 後ろスペースをカット
					LPSTR	p = ptkrec->m_tkname.GetBuffer(256);
					int n = kjlen( p, TKYO_LEN );
					*(p + n) = '\0';
					ptkrec->m_tkname.ReleaseBuffer();

					ptkrec->m_tkari = pDBzm->tkrec->tkari;
					ptkrec->m_tkasi = pDBzm->tkrec->tkasi;
					sgn = 1;
					break;
				}
			} while( pDBzm->tkrec->MoveNext() == 0 );
		}
		return (sgn) ? 0 : -1;*/

		ptkrec->m_tkcod = srt.tky_code;
		ptkrec->m_tkname = srt.tky;

		return 0;
	}

	return -1;

}
#endif


// 選択画面の作成
int CDBbaseView::MakeSelWnd( BOOL bIgnoreActive/* =FALSE */ )
{
	if (m_pSelWnd == NULL)
	{
		if( !(m_pSelWnd = new CSelWndDlg(this)) )
	//	if( !(m_pSelWnd = new CSelWndDlg( GetDBDinpMainFrame() )) )
		{
			ermset( ERROR_MEMORY, "選択ウィンドウオブジェクトのメモリ確保ができません。");
			return -1;
		}

		m_pSelWnd->m_bIgnoreActvate = bIgnoreActive;
	}
/*
	if( m_pSelWnd->GetSafeHwnd() == NULL )
	{
		if( m_pSelWnd->Create() == FALSE )
		{
			ermset( ERROR_MEMORY, "選択ウィンドウオブジェクトが作成できません。");
			return -1;
		}
	}
*/
	return 0;
}

// 選択画面の 完全に削除
void CDBbaseView::SelWndDelete()
{
	if ( m_pSelWnd ) {
		delete m_pSelWnd;
		m_pSelWnd = NULL;
	}
}


// 定型仕訳
void CDBbaseView::RdataTblIniz( void )
{
	m_RDTATBL.RD_lbnum = -1;
	m_RDTATBL.RD_cnt = 0;
	m_RDTATBL.RD_trn = 0;
	m_RDTATBL.RD_array.RemoveAll();
}

void CDBbaseView::EndRdata( void )
{
	m_RDTATBL.RD_cnt = 0;
	m_RDTATBL.RD_trn = 0;
	m_RDTATBL.RD_array.RemoveAll();

	// 定型仕訳 残
	RDRemainDispJOB();
}


int CDBbaseView::RemainRdata( void )
{
	// 定型仕訳 残
	RDRemainDispJOB();

	return m_RDTATBL.RD_cnt - m_RDTATBL.RD_trn;
}

//
//	定型仕訳の消費税チェック
//
//
#define BIT_MENZEI 0x20

int CDBbaseView::rd_reccheck( CDBINPDataRec *crec, CDBINPDataRec *pRDrec, DWORD* pSyCode )
{
	int syzCnt = 0;

	if( IsMasterType(MST_SYZ_INV) ) {
		if( (crec->m_ddate < SYZ_INV_YMD) && pRDrec->m_dsign[6] & BIT_MENZEI ) {
			*(pSyCode+syzCnt) = CD_SP_NO_EXEMPT;
			syzCnt++;
			pRDrec->m_dsign[6] &= ~BIT_MENZEI;
			//税額も変わるため初期化

			if( (pRDrec->m_dsign[0] & 0x40) && (pRDrec->m_dsign[2] & 0x0f) == 0x01 ) {
				//税率を再設定するため、一旦別の税率をセットしてから、定型仕訳の税率をセット
				switch( (pRDrec->m_dsign[1] & 0x0f)) {
				case 0: 
					*(pSyCode + syzCnt++) = CD_RITU10;
					*(pSyCode + syzCnt++) = CD_RITU5;	break;
				case 1:
					*(pSyCode + syzCnt++) = CD_RITU10;
					*(pSyCode + syzCnt++) = CD_RITU3;	break;
				case 2:
					*(pSyCode + syzCnt++) = CD_RITU10;
					*(pSyCode + syzCnt++) = CD_RITU0;	break;
				case 3:
					*(pSyCode + syzCnt++) = CD_RITU10;
					*(pSyCode + syzCnt++) = CD_RITU8;	break;
				case 4:
					*(pSyCode + syzCnt++) = CD_RITU10;
					*(pSyCode + syzCnt++) = CD_RD_RITU8;	break;
				case 5:
					*(pSyCode + syzCnt++) = CD_RITU8;
					*(pSyCode + syzCnt++) = CD_RITU10;	break;
				}
			}
		}
#ifdef CLOSE // 日付によるインボイスチェックを行わない
		if( crec->m_ddate < SYZ_INV_YMD ) {
			if( (pRDrec->m_invno[0]) ) {
				::ZeroMemory(pRDrec->m_invno, sizeof pRDrec->m_invno);
			}
		}
#endif
	}

	if( IsMasterType( MST_SYZ8_10 ) ) {
		if (RDsiwakeZeirituMode() == 0)
			return FALSE;

		if( crec->m_ddate < SYZ10_YMD ) {
			*(pSyCode + syzCnt) = CD_RITU8;
			//消費税対象＋10％(8軽)＋課税  → 8%へ
			if( (pRDrec->m_dsign[0]&0x40) && ((pRDrec->m_dsign[1]&0x0f) == 0x05) &&
				(pRDrec->m_dsign[2]&0x0f) == 0x01 ) {
				syzCnt++;
			}
			else {
				int dbt_sy, cre_sy;
				dbt_sy = pDBzm->km_syattr( pRDrec->m_dbt );
				cre_sy = pDBzm->km_syattr( pRDrec->m_cre );

				if( (dbt_sy & 0x0200) || (cre_sy & 0x0200) ) {
					if( (pRDrec->m_dsign[1]&0x0f) == 0x05 ) {
						syzCnt++;
					}
				}
			}
		}
		else {
			*(pSyCode + syzCnt) = CD_RITU10;
			//消費税対象＋８％＋課税 → 10%へ
			if( (pRDrec->m_dsign[0]&0x40) && (pRDrec->m_dsign[1]&0x0f) == 0x03 &&
				(pRDrec->m_dsign[2]&0x0f) == 0x01 ) {
				syzCnt++;
			}
			else {
				int dbt_sy, cre_sy;
				dbt_sy = pDBzm->km_syattr( pRDrec->m_dbt );
				cre_sy = pDBzm->km_syattr( pRDrec->m_cre );

				if( (dbt_sy & 0x0200) || (cre_sy & 0x0200) ) {

					if( (pRDrec->m_dsign[1]&0x0f) == 0x03 ) {
						syzCnt++;
					}
				}
			}
		}
	}

	if( IsMasterType( MST_SYZ5_8 ) ) {
		if( crec->m_ddate < SYZ8_YMD ) {
			*(pSyCode + syzCnt) = CD_RITU5;
			//消費税対象＋８％＋課税
			if( (pRDrec->m_dsign[0]&0x40) && (pRDrec->m_dsign[1]&0x0f) == 0x03 &&
				(pRDrec->m_dsign[2]&0x0f) == 0x01 ) {
				syzCnt++;
			}
			else {
				int dbt_sy, cre_sy;
				dbt_sy = pDBzm->km_syattr( pRDrec->m_dbt );
				cre_sy = pDBzm->km_syattr( pRDrec->m_cre );

				if( (dbt_sy & 0x0200) || (cre_sy & 0x0200) ) {

					if( (pRDrec->m_dsign[1]&0x0f) == 0x03 ) {
						syzCnt++;
					}
				}
			}
		}
		else {
			*(pSyCode + syzCnt) = CD_RITU8;
			//消費税対象＋５％＋課税
			if( (pRDrec->m_dsign[0]&0x40) && (pRDrec->m_dsign[1]&0x0f) == 0x00 &&
				(pRDrec->m_dsign[2]&0x0f) == 0x01 ) {
				syzCnt++;
			}
			else {
				int dbt_sy, cre_sy;
				dbt_sy = pDBzm->km_syattr( pRDrec->m_dbt );
				cre_sy = pDBzm->km_syattr( pRDrec->m_cre );

				if( (dbt_sy & 0x0200) || (cre_sy & 0x0200) ) {

					if( (pRDrec->m_dsign[1]&0x0f) == 0x00 ) {
						syzCnt++;
					}
				}
			}
		}
	}

	if( pDBsy->IsKaniMaster() ) {
		if( crec->m_ddate < KANI6_YMD ) {
			//簡易課税６種 対象外？
			if( (pRDrec->m_dsign[0]&0x40) && (pRDrec->m_dsign[2]&0x0f) == 0x01 &&
				(pRDrec->m_dsign[3]&0x0f) == 0x06 ) {

				BYTE defKbn = KaniDefaultKubun( Voln1 );
				//サインOFF
				pRDrec->m_dsign[3] &= ~0x0f;
				pRDrec->m_dsign[3] |=  defKbn;	//基本業種セット
			}
		}
	}
	//リバースチャージ
	if( crec->m_ddate < RVCHAR_YMD ) {
		//簡易課税６種 対象外？
		if( (pRDrec->m_dsign[0]&0x40) && (pRDrec->m_dsign[2]&0x0f) == 0x01 &&
			((pRDrec->m_dsign[4]) == 0x1d ||(pRDrec->m_dsign[4]) == 0x1e) ) {

			//サインOFF
			pRDrec->m_dsign[4] = 0;

			int dbt_kbn, cre_kbn;
			dbt_kbn = pDBzm->km_zeikbn( pRDrec->m_dbt );
			cre_kbn = pDBzm->km_zeikbn( pRDrec->m_cre );

			if( dbt_kbn < 0 && cre_kbn < 0 ) {
				::ZeroMemory( &pRDrec->m_dsign[0], SZ_SYZDSGN );
			}
			else {
				if( dbt_kbn > 0 || cre_kbn > 0 ) {
					*(pSyCode + syzCnt) = CD_SOTOZ;
				}
				else {
					*(pSyCode + syzCnt) = CD_UTIZ;
				}
				syzCnt++;
			}
		}
	}

	return syzCnt;
}



BOOL CDBbaseView::GetRdata( CDBINPDataRec* rdrec, CDBINPDataRec* pCrec, CDBINPDataRec* pSrec, BOOL* cnd )
{
	if( RemainRdata() > 0 )
	{
		int pos;
		pos = m_RDTATBL.RD_array[ m_RDTATBL.RD_trn++ ];

		if( Mkrd.data->lbnum != m_RDTATBL.RD_lbnum ) {
			mkrd_data_requery(m_RDTATBL.RD_lbnum);
		}
		Mkrd.data->SetAbsolutePosition( pos );

		rdrec->m_seq	= pCrec->m_seq;
		rdrec->m_ksign	= pSrec->m_ksign;
		rdrec->m_ddate	= pSrec->m_ddate;
		rdrec->m_mofs	= pSrec->m_mofs;
		rdrec->m_cno	= -1;
		rdrec->m_dbmn	= Mkrd.data->dbmn;
		rdrec->m_dkno	= Mkrd.data->dkno;
		LPSTR ptr = rdrec->m_dkno.GetBuffer(64);
		trim_zero(ptr, 64, strlen(ptr));
		rdrec->m_dkno.ReleaseBuffer();
		rdrec->m_dkno.TrimRight();

		rdrec->m_dbt	= Mkrd.data->dbt;
		rdrec->m_dbt.TrimRight();
		rdrec->m_dbr	= Mkrd.data->dbr;

		rdrec->m_cbmn	= Mkrd.data->cbmn;
		rdrec->m_ckno	= Mkrd.data->ckno;
		ptr = rdrec->m_ckno.GetBuffer(64);
		trim_zero(ptr, 64, strlen(ptr));
		rdrec->m_ckno.ReleaseBuffer();
		rdrec->m_ckno.TrimRight();

		rdrec->m_cre	= Mkrd.data->cre;
		rdrec->m_cre.TrimRight();
		rdrec->m_cbr	= Mkrd.data->cbr;

		// 09.16 /08
		int type =  IsKojinGyosyuMaster( pDBzm );
		if( type ) {
			struct _KamokuData  akd = {0}, ckd = {0};

			strcpy_s( ckd.kd_code, sizeof ckd.kd_code, rdrec->m_dbt );
			ckd.kd_sgn = 2;	// 科目コードチェック用に 枝番のみのサインセット
			if( ! m_Seldata.InputItemCheck( akd, ckd, 0 ) ) {
				rdrec->m_dbt.Empty();
				rdrec->m_dbr = -1;
			}

			::ZeroMemory( &akd, sizeof akd );
			::ZeroMemory( &ckd, sizeof ckd );
			strcpy_s( ckd.kd_code, sizeof ckd.kd_code, rdrec->m_cre );
			ckd.kd_sgn = 2;	// 科目コードチェック用に 枝番のみのサインセット
			if( ! m_Seldata.InputItemCheck( akd, ckd, 0 ) ) {
				rdrec->m_cre.Empty();
				rdrec->m_cbr = -1;
			}
		}
		//不使用チェック -- 04.13 /10
		DBKNREC* pKn;
		if( ! rdrec->m_dbt.IsEmpty() ) {
			rdrec->m_dbt.MakeLower();
			pKn = pDBzm->DB_PjisToKnrec( rdrec->m_dbt );
			if( pKn == NULL || (pKn->knvoid & 0x01) ) {
				rdrec->m_dbt.Empty();
				rdrec->m_dbr = -1;
			}
		}
		if( ! rdrec->m_cre.IsEmpty() ) {
			rdrec->m_cre.MakeLower();
			pKn = pDBzm->DB_PjisToKnrec( rdrec->m_cre );
			if( pKn == NULL || (pKn->knvoid & 0x01) ) {
				rdrec->m_cre.Empty();
				rdrec->m_cbr = -1;
			}
		}

		l_input( rdrec->m_val, (void*)(LPCTSTR)Mkrd.data->val );
		cnd[0] = Mkrd.data->valflag;
		l_input( rdrec->m_zei, (void*)(LPCTSTR)Mkrd.data->zei );
		cnd[1] = Mkrd.data->zeiflag;

		rdrec->m_dsign.Copy( Mkrd.data->dsign );
		rdrec->m_dsign[0] &= (BYTE)~0x80;	//イメージサインを落とす。
		rdrec->m_dsign[15] &= (BYTE)~0x01;	// 定型仕訳サイン

		rdrec->m_owner	= -1;
		rdrec->m_dtype	= 0 /*Mkrd.data->dtype*/;//定型仕訳は、通常入力のみなので ０

		rdrec->m_tekiyo	= Mkrd.data->tekiyo;
		rdrec->m_imgsq	= -1;
		rdrec->m_snumber	= "";
		rdrec->m_tdate	= -1;
		rdrec->m_edate	= -1;
		rdrec->m_udate	= -1;

		rdrec->m_icno	= -1;
		rdrec->m_skn		= "";
		rdrec->m_tag		= "";

		rdrec->m_hjiyu	= Mkrd.data->hjiyu;
		l_input( rdrec->m_taika, (void*)(LPCTSTR)Mkrd.data->taika );

		// インボイス対応
		sprintf_s( rdrec->m_invno, sizeof( rdrec->m_invno ), _T( "%s" ), Mkrd.data->invno );

		rdrec->m_rnumber = "";

		return TRUE;
	}
	else
	{
		// 定型仕訳取消
		EndRdata();
		return FALSE;
	}
}


void CDBbaseView::CalcMouseEvent()
{
	mouse_event( MOUSEEVENTF_LEFTDOWN, 0,0,0,0 );
	mouse_event( MOUSEEVENTF_LEFTUP, 0,0,0,0 );
}




// プリントパラメータセット
int CDBbaseView::SetNprtParam( CImgMng* pMng )
{
	int st = 0;

	memset( &m_Pset, '\0', sizeof NPRT_PAR );		// 印刷毎に初期化 ('15.06.18)

	PCOM_GetString( "SelectPrinter", m_Pset.psPrinterName );	// プリンタ名のみこちらでのセットが必要

	if (m_ImgPrintFlg == PRINT_VERIFY) {		// 一括検証
		st = pMng->ImgPrintInitVerify(&m_Pset);	// NPRT_PARをセット	
	}
	else if (m_ImgPrintFlg == PRINT_DOCIMG ) {	// 原票イメージ
		st = pMng->ImgPrintInitData(&m_Pset);	// NPRT_PARをセット

//MyTrace( "pMng->ImgPrintInitData st = %d\n", st );
	}

	if (st < 0) {	
		if (st == -2)	// ドットプリンタは処理不可
			ICSMessageBox("当業務では､ドットプリンターによる出力はできません！", MB_ICONSTOP);
		else if (st == -1)
			ICSMessageBox(pMng->GetErrorMsg(), MB_ICONSTOP);
		if (m_ImgPrintFlg == PRINT_VERIFY) {
			pMng->ImgPrintResetDlg();
		}
		else {
			pMng->ImgEndPrintData();
		}
		m_ImgPrintFlg = PRINT_NONE;
		return st;
	}
	return 0;
}


BOOL CDBbaseView::OnPreparePrinting(CPrintInfo* pInfo)
{
//MyTrace( "@CDBbaseView::OnPreparePrinting TOP m_bPreview = %d\n", pInfo->m_bPreview );
	return FALSE;
}

void CDBbaseView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo )
{
//MyTrace( "@CDBbaseView::OnBeginPrinting TOP \n" );
	m_Vprn.PrePrint(pDC, MM_LOMETRIC);

//MyTrace( "@CDBbaseView::OnBeginPrinting END \n" );
}

void CDBbaseView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
//MyTrace( "@CDBbaseView::OnPrepareDC TOP \n" );

	ICSFormView::OnPrepareDC(pDC, pInfo);
	if(pDC->IsPrinting() != 0)	m_Vprn.InitPage(pDC,0,0);

//MyTrace( "@CDBbaseView::OnPrepareDC END \n" );
}

void CDBbaseView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo )
{
//MyTrace( "@CDBbaseView::OnEndPrinting TOP m_bPreview = %d\n", pInfo->m_bPreview );
	m_Vprn.EndPrint();
	if( m_Vprn.IsPrintFromPreview() == FALSE ) {
		m_ImgPrintFlg = PRINT_NONE;
	}
#ifdef PRINT_LATER
	// 作成日・事務所名
	prsw1 = (BYTE)(m_Pset.psSigDate&0x0033);
	prsw2 = (BYTE)(m_Pset.psSigConame&0x0061);
#endif

//MyTrace( "@CDBbaseView::OnEndPrinting END\n" );
}

void CDBbaseView::OnPrint(CDC* pDC, CPrintInfo*)
{
}


void CDBbaseView::RDzeritsuModeWrite( int old_mode )
{
	int now_mode = RDsiwakeZeirituMode();

	if (now_mode != old_mode)
	{
		theApp.SetOptionInt( OPT_SECTION, OPT_RDZRITSU, now_mode );
	}
}


HWND CDBbaseView::GetKamokuCtrlHWND()
{
	return NULL;
}


//科目欄 複数文字でカナ検索
//	possgn:0	科目欄
//	possgn:1	登録番号欄
int CDBbaseView::FukuKanaSearch(MSG* pMsg, int edaonly, int possgn/* = 0*/)
{
	HIMC	hImc;
	char	buf[128] = { 0 };
	char	yomi[128] = { 0 };
	char	yomi_org[128] = { 0 };

	HWND	hWnd;
	if( !possgn ) {
		if( KanaSearchType() == 0 )
			return 0;
		//コントロールのHWND を取得する
		hWnd = GetKamokuCtrlHWND();
	}
	else {
		hWnd = GetKamokuCtrlHWND();
	}
	if( hWnd == NULL )
		return 0;

	if( pMsg->wParam == VK_RETURN || pMsg->wParam == VK_TAB ) {
		return 0;
	}


	if( pMsg->wParam == VK_NEXT || pMsg->wParam == VK_PRIOR ) {
		return 0;
	}

	hImc = ImmGetContext(hWnd);

	if( pMsg->wParam == VK_END ) {
		ImmNotifyIME(hImc, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
		ImmReleaseContext(hWnd, hImc);
		return 0;
	}

	ImmGetCompositionString(hImc, GCS_COMPSTR, buf, sizeof buf);
	ImmGetCompositionString(hImc, GCS_COMPREADSTR, yomi_org, sizeof yomi_org);

	//読みにアルファベットがあるか？
	CString stmp(yomi);
	kana_check(yomi, yomi_org);

//MyTrace("●CDBbaseView::FukuKanaSearch buf, = %s, yomi_org = %s, yomi = %s, pMsg->wParam = %d●\n", buf, yomi_org, yomi, pMsg->wParam );

	int orglen = strlen(yomi_org);
	int searchsgn = 1;
	if( orglen > 0 ) {
		int len = strlen(yomi);
		if( len < orglen ) {
			searchsgn = 0;
		}
	}

	int rval = 0;

	if( buf[0] && yomi[0] && searchsgn ) {
		SelPar sel;
		SelectWindowShow(TRUE);

		if( possgn == 0 ) {
			if( BRNTKY_sel ) {
				sel.sel_pn = SL_BRNTKY_PN;
				sel.sel_flg = 1;

				BRNTKY_PAR brp;
				brp = m_Seldata.GetBrnTkyPar();
				if( isSYOGT_CODE(brp.bt_code) ) {
					ImmReleaseContext(hWnd, hImc);
					return 0;
				}
				sel.tkybrn = brp.bt_code;
				sel.brn_bmn = brp.bt_bmn;
				// 絞込み
				sprintf_s(sel.brn_kana, sizeof sel.brn_kana, "%.8s", yomi);
			}
			else {
				//枝番のみの欄
				if( edaonly ) {
					ImmReleaseContext(hWnd, hImc);
					return 0;
				}
				sel.sel_pn = SL_KMKANA_PN;
				sprintf_s(sel.kmk_kana, sizeof sel.kmk_kana, "%.8s", yomi);
			}
		}
		else {
			//登録番号欄
			sel.sel_pn = SL_INVKANA_PN;
			sprintf_s(sel.tky_kana, sizeof sel.tky_kana, "%.8s", yomi);
		}
		m_Seldata.SelectJob(&sel, -1);
	}
	else if( (buf[0] == '\0') && (yomi[0] == '\0') ) {
		if( pMsg->wParam == VK_BACK || pMsg->wParam == VK_DELETE ) {
			SelPar par;
			if( possgn == 0 ) {
				if( BRNTKY_sel ) {
					BRNTKY_PAR brp;
					brp = m_Seldata.GetBrnTkyPar();
					if( brp.bt_kana[0] == 0 ) {
						par.sel_pn = SL_KAMOKU_PN;
						//枝番のみの欄
						if( edaonly ) {
							ImmReleaseContext(hWnd, hImc);
							return 0;
						}
					}
					else {
						par.sel_pn = SL_BRNTKY_PN;
						par.sel_flg = 1;
						par.tkybrn = brp.bt_code;
						par.brn_bmn = brp.bt_bmn;
						::ZeroMemory(par.brn_kana, sizeof par.brn_kana);
					}
				}
				else {
					par.sel_pn = SL_KAMOKU_PN;
				}
			}
			else {
				//登録番号欄
				par.sel_pn = SL_INVNO_PN;
				::ZeroMemory(par.tky_kana, sizeof par.tky_kana);
			}
			m_Seldata.SelectJob(&par, -1);
		}
		if( pMsg->wParam == VK_BACK ) {
			rval = 1;
		}
	}
	ImmReleaseContext(hWnd, hImc);

	return rval;
}


//hWnd: カナ検索のコントロールのHWND
int fncKanaCheckVK_BACK(MSG* pMsg, HWND hWnd)
{
	HIMC	hImc;
	char	buf[128] = { 0 };
	char	yomi[128] = { 0 };
	char	yomi_org[128] = { 0 };

	if( hWnd == NULL )
		return 0;

	if( pMsg->wParam != VK_BACK && pMsg->wParam != VK_F2 ) {
		return 0;
	}

	hImc = ImmGetContext(hWnd);

	if( pMsg->wParam == VK_F2 ) {
		ImmNotifyIME(hImc, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
		ImmReleaseContext(hWnd, hImc);
		return 0;
	}
	ImmGetCompositionString(hImc, GCS_COMPSTR, buf, sizeof buf);
	ImmGetCompositionString(hImc, GCS_COMPREADSTR, yomi_org, sizeof yomi_org);

	int rval = 0;

	if( (buf[0] == '\0') && (yomi[0] == '\0') ) {
		if( pMsg->wParam == VK_BACK ) {
			rval = 1;
		}
	}

	ImmReleaseContext(hWnd, hImc);

	return rval;
}

//科目欄 バックスペース対応
int CDBbaseView::KanaCheckVK_BACK(MSG* pMsg, int possgn/* =0*/ )
{
	char	buf[128] = { 0 };
	char	yomi[128] = { 0 };
	char	yomi_org[128] = { 0 };

	//コントロールのHWND を取得する
	HWND	hWnd;

	if( possgn == 0 ) {
		if( KanaSearchType() == 0 )
			return 0;
		hWnd = GetKamokuCtrlHWND();
	}
	else {
		//登録番号
		hWnd = GetKamokuCtrlHWND();
	}

	return fncKanaCheckVK_BACK(pMsg, hWnd);

#ifdef CLOSE
	if( hWnd == NULL )
		return 0;

	if( pMsg->wParam != VK_BACK && pMsg->wParam != VK_F2 ) {
		return 0;
	}


	hImc = ImmGetContext(hWnd);

	if( pMsg->wParam == VK_F2 ) {
		ImmNotifyIME(hImc, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
		ImmReleaseContext(hWnd, hImc);
		return 0;
	}
	ImmGetCompositionString(hImc, GCS_COMPSTR, buf, sizeof buf);
	ImmGetCompositionString(hImc, GCS_COMPREADSTR, yomi_org, sizeof yomi_org);

	int rval = 0;

	if( (buf[0] == '\0') && (yomi[0] == '\0') ) {
		if( pMsg->wParam == VK_BACK ) {
			rval = 1;
		}
	}

	ImmReleaseContext(hWnd, hImc);

	return rval;
#endif
}


//科目を打ち換えても、登録番号が残るのでクリアする
// 162291, 162293 で 仕訳区分のある仕訳の登録番号がクリアされるので
//	カット
//登録番号チェック
void CDBbaseView::InvnoCheck(CDBINPDataRec* prec)
{
#ifdef CLOSE
	if( prec->m_invno[0] != 0 ) {
		DBKNREC* pKn1, *pKn2;
		pKn1 = pDBzm->DB_PjisToKnrec(prec->m_dbt);
		pKn2 = pDBzm->DB_PjisToKnrec(prec->m_cre);
		int siire = 0;
		if( pKn1 && pDBzm->km_siire(pKn1) ) {
			siire++;
		}
		if( pKn2 && pDBzm->km_siire(pKn2) ) {
			siire++;
		}

		int tmp = 0;
		tmp = _atoh(( char* )( LPCTSTR )prec->m_dbt );
		if( IsKaribarai( tmp )) {
			siire++;
		}

		tmp = _atoh(( char* )( LPCTSTR )prec->m_cre );
		if( IsKaribarai( tmp )) {
			siire++;
		}

		// 仕入科目でない
		if( !siire ) {
			::ZeroMemory(prec->m_invno, sizeof prec->m_invno);
		}
	}
#endif
}


//------------------------------------------------------------------
//    他のプログラムを呼び出せる状態か？
//
//------------------------------------------------------------------
BOOL CDBbaseView::EnableCallPro()
{
	return TRUE;
}

