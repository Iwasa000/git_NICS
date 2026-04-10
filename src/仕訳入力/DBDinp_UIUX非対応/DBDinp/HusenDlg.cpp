// HusenDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dbdinp.h"

#include "HusenDlg.h"

#include "DataListCtrl.h"
#include "DinpSub.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// フォーク中サイン
extern int IN_FORK;

/////////////////////////////////////////////////////////////////////////////
// CHusenDlg ダイアログ
#define HS_SEL_PN			0
#define HS_CMT_PN			1
#define HS_PN_MAX	HS_CMT_PN+1

static struct _InpCtl HUSEN_CTL[HS_PN_MAX] = {
	{IDC_HS_COMBO,		0,0,0,-1},	// 付箋選択
	{IDC_ICSDBEDTCTRL1,	0,0,0,-1}	// 付箋ｺﾒﾝﾄ
};



CHusenDlg::CHusenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHusenDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHusenDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
	m_nID = IDD;
	m_pParent = pParent;

	m_THIS_DIALOG_OK = FALSE;

	//アクセラレータテーブルロード
	m_hAccelTable = ::LoadAccelerators( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
}


void CHusenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHusenDlg)
	DDX_Control(pDX, IDC_ICSDBEDTCTRL1, m_DBedt);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_HS_COMBO, m_Hcbo );
}

//オリジナルダイアログサイズ
void CHusenDlg::get_originalsize( double& w, double& h )
{
	w = (double)(m_original_rect.right - m_original_rect.left);
	h = (double)(m_original_rect.bottom - m_original_rect.top);
}

BOOL CHusenDlg::Create()
{
	return CDialog::Create(m_nID, m_pParent);
}

void CHusenDlg::PostNcDestroy() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

	delete this;
//	CDialog::PostNcDestroy();
}


// IDC get
struct _InpCtl *CHusenDlg::getINP_CTL( int pn )
{
	if( pn >= 0 && pn < HS_PN_MAX )
		return( &HUSEN_CTL[pn] );
	else
		return( &HUSEN_CTL[0] );	//ERROR
}

void CHusenDlg::set_fastfocus( BOOL modefy_sg, int pn )
{
	m_MODEFY_OK = modefy_sg;
	m_THIS_DIALOG_OK = TRUE;

	if( ! m_MODEFY_OK ) {
		m_Hcbo.EnableWindow( FALSE );
		pn = HS_CMT_PN;
	}
	else {
		m_Hcbo.EnableWindow( TRUE );	
	}

	PostMessage( WM_RETROFOCUS, pn );
}

void CHusenDlg::set_nowfocus()
{
	set_focus( get_nowpn() );
}

// カーソルセット
void CHusenDlg::set_focus( int pn )
{
	if( pn >= 0 )
	{
		PostMessage(WM_RETROFOCUS, pn);
	}
}


LRESULT CHusenDlg::RetroFocus( WPARAM wParam, LPARAM lParam )
{
	switch( wParam ) {
	case HS_SEL_PN:
	case HS_CMT_PN:
		c_set( getINP_CTL( wParam )->IDC_X );
		break;
	}

	return 1;
}


// 現在ポジションセット
int CHusenDlg::set_nowpn( int pn )
{
	m_PREV_PN = m_NOW_PN;
	m_NOW_PN = pn;

	return( m_PREV_PN );	//直前のポジション
}

// 現在ポジションゲット
int CHusenDlg::get_nowpn( void )
{
	return( m_NOW_PN );
}

// 直前のポジションゲット
int CHusenDlg::get_prevpn()
{
	return( m_PREV_PN );
}

// 初期化処理
void CHusenDlg::HusenInp_Init( CDBipDataRec* crec, CDBipDataRec* srec, struct _DATA_LINE* dtline )
{
	m_pCREC = crec;
	m_pSREC = srec;
	m_Dtline = dtline;

	VARIANT var;
	char	buf[64];

	if( dtline->LINE_CND[HSEN_PN].INP_sg ) {
		if( dtline->_XXDTA.dd_husen < 0 )
			m_Hcbo.SetCurSel(-1);
		else
			m_Hcbo.SetCurSel( dtline->_XXDTA.dd_husen );
		strcpy_s( buf, sizeof buf, dtline->_XXDTA.dd_hscomment );
		var.pbVal = (BYTE*)buf;
		DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
	}
	else {
		int sgn = (srec->m_dsign[7]&0x0f);

		if( sgn < 0 )
			m_Hcbo.SetCurSel(-1);
		else
			m_Hcbo.SetCurSel( sgn );
	
		strcpy_s( buf, sizeof buf, srec->m_tag );
		var.pbVal = (BYTE*)buf;
		DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
	}

}


BEGIN_MESSAGE_MAP(CHusenDlg, CDialog)
	//{{AFX_MSG_MAP(CHusenDlg)
	ON_CBN_SELCHANGE(IDC_HS_COMBO, OnSelchangeHsCombo)
	ON_CBN_SETFOCUS(IDC_HS_COMBO, OnSetfocusHsCombo)
	ON_CBN_KILLFOCUS(IDC_HS_COMBO, OnKillfocusHsCombo)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_RETROFOCUS, RetroFocus)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CHusenDlg メッセージ ハンドラ

BOOL CHusenDlg::OnInitDialog() 
{
//	ControlInit( 82, 36, -1 );

	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	GetWindowRect( &m_original_rect );	//オリジナルのサイズゲット

	m_Hcbo.InitHusenCombo();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}


void CHusenDlg::OnSelchangeHsCombo() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
}

void CHusenDlg::OnSetfocusHsCombo() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( m_THIS_DIALOG_OK == FALSE || IN_FORK )
		return;

	set_nowpn( HS_SEL_PN );
}

void CHusenDlg::OnKillfocusHsCombo() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int sel = m_Hcbo.GetCurSel();
	
	if( sel != CB_ERR ) {
		m_pCREC->m_dsign[7] &= (~0x0f);
		m_pCREC->m_dsign[7] |= sel;

		m_Dtline->_XXDTA.dd_husen = sel;
		m_pParent->SendMessage( WM_HSENINP_MSG, 1 );
	}	
}

BEGIN_EVENTSINK_MAP(CHusenDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CHusenDlg)
	ON_EVENT(CHusenDlg, IDC_ICSDBEDTCTRL1, 3 /* SetFocus */, OnSetFocusIcsdbedtctrl1, VTS_NONE)
	ON_EVENT(CHusenDlg, IDC_ICSDBEDTCTRL1, 1 /* Termination */, OnTerminationIcsdbedtctrl1, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CHusenDlg, IDC_ICSDBEDTCTRL1, 2 /* KillFocus */, OnKillFocusIcsdbedtctrl1, VTS_I4)
	ON_EVENT(CHusenDlg, IDC_ICSDBEDTCTRL1, 5 /* ImeEndComposition */, OnImeEndCompositionIcsdbedtctrl1, VTS_I2 VTS_BSTR VTS_BSTR)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CHusenDlg::OnSetFocusIcsdbedtctrl1() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( m_THIS_DIALOG_OK == FALSE || IN_FORK )
		return;

	set_nowpn( HS_CMT_PN );	
}

void CHusenDlg::OnKillFocusIcsdbedtctrl1(long inplen) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( m_THIS_DIALOG_OK == FALSE || IN_FORK )
		return;
	
}


void CHusenDlg::OnImeEndCompositionIcsdbedtctrl1(short nChar, LPCTSTR string, LPCTSTR ystring) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	
}

void CHusenDlg::OnTerminationIcsdbedtctrl1(long nChar, long inplen, long kst) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( m_THIS_DIALOG_OK == FALSE || IN_FORK )
		return;

	TerminationHusenComment( nChar, inplen, kst );
}



void CHusenDlg::TerminationHusenComment(long nChar, long inplen, long kst)
{
	VARIANT var;
	char	buf[36] = {0};

	if( inplen ) {

		DBdata_get( this, getINP_CTL(HS_CMT_PN)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );

		if( m_MODEFY_OK ) {
			if( var.pbVal != NULL )	strncpy_s( buf, sizeof buf, (char*)var.pbVal, 10 );
			m_pCREC->m_tag = buf;
			m_Dtline->_XXDTA.dd_hscomment = buf;
		}
		else {
			//修正不可 元のデータに戻す
			strcpy_s( buf, sizeof buf, m_Dtline->_XXDTA.dd_hscomment );

			if( ! buf[0] )	var.pbVal = NULL;
			else			var.pbVal = (BYTE*)buf;

			DBdata_set( this, getINP_CTL(HS_CMT_PN)->IDC_X, &var, ICSDBEDT_TYPE_STRING, 0 );
		}
	}

	TermWork( HS_CMT_PN, nChar );

}


// ターミネーション処理
long CHusenDlg::TermWork( int pn, long nChar )
{
	long ret = nChar;

	if( nChar == VK_RIGHT || nChar == VK_LEFT ) {
		to_parent( nChar );
		ret = 0;
	}
	else if( nChar == VK_ESCAPE ) {
		ret = 0;
	}
	else {
		if( pn == HS_CMT_PN ) {
			switch( nChar ) {
			case VK_RETURN : case VK_TAB :
			case VK_DELETE : case VK_DOWN :
				to_parent( nChar );
				ret = 0;
				break;
			case VK_UP :
				set_focus( HS_SEL_PN );
				ret = 0;
				break;
			default:
				break;
			}
		}
		else {
			switch( nChar ) {
			case VK_RETURN : case VK_TAB :
				set_focus( HS_CMT_PN );
				ret = 0;
				break;
			case VK_DOWN :
				if( ! m_Hcbo.GetDroppedState() ) {
					m_Hcbo.ShowDropDown( TRUE );
					ret = 0;
				}
				break;
			default:
				break;
			}
		}
	}

	return ret;  
}


void CHusenDlg::to_parent( long nChar )
{
	m_pParent->SendMessage( WM_HSENINP_MSG, 0, nChar );
}



BOOL CHusenDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	if( (m_hAccelTable != NULL) && ::TranslateAccelerator( m_hWnd, m_hAccelTable, pMsg))
	{
		return TRUE;
	}

	if( pMsg->message == WM_KEYDOWN ) {
		if( pMsg->hwnd == m_Hcbo.m_hWnd ) {
			if( ! TermWork( HS_SEL_PN, pMsg->wParam ) )
				return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
