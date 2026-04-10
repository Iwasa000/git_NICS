// NSDenpDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBDinp.h"

#include "DnpInView.h"
#include "NSDenpDlg.h"



extern CDnpInView* p_DnpView;

// 入金・出金伝票 項目ポジション

static DNP_POS NSHeadPos[] = {
	// ヘッド部分
	{ 0, 0, DP_DENP_PN },
	{ 0, 1, DP_MM_PN },
	{ 0, 2, DP_DD_PN },
	{ 0, 13,DP_SYONUM_PN },
	{ 0, 3, DP_BMN_PN },
	{ 0, 4, DP_KOJI_PN },
	{ 0, 5, DP_KESYU_PN },

	{ -1, -1, -1 }
};

// 入金伝票
static DNP_POS NDataPos[] = {
	// 仕訳部分
	{ 1, 0, -1/*FD_CBMN_PN*/,	0},
	{ 1, 1, -1/*FD_CKOJI_PN*/,	0},
	{ 1, 2, FD_CRED_PN,		1},
	{ 1, 3, FD_TKY_PN,		1},
	{ 1, 5, FD_CRVAL_PN,	1},
	{ 1, 6, FD_CRZEI_PN,	1},
	{ 1, 4, FD_CSKBN_PN,	1},

	{ 1, 7, FD_TGNUM_PN	,	1},
	{ 1, 8, FD_TGDATE_PN,	1},
	{ 1, 9, FD_HJIYU_PN	,	1},
	{ 1,10, FD_HUSEN_PN	,	1},

	{ -1,-1, -1,	1},
};

static DNP_POS NKeiPos[] = {
	//合計部分
	{ 1, 66, FD_CRVAL_PN,	0},
	{ 1, 67, FD_CRZEI_PN,	0},

	{ -1,-1, -1,	1},
};

// 出金伝票
static DNP_POS SDataPos[] = {
	// 仕訳部分
	{ 1, 0, -1/*FD_DBMN_PN*/,	0},
	{ 1, 1, -1/*FD_DKOJI_PN*/,	0},
	{ 1, 2, FD_DEBT_PN,		1},
	{ 1, 3, FD_TKY_PN,		1},
	{ 1, 5, FD_DBVAL_PN,	1},
	{ 1, 6, FD_DBZEI_PN,	1},
	{ 1, 4, FD_DSKBN_PN,	1},

	{ 1, 7, FD_TGNUM_PN	,	1},
	{ 1, 8, FD_TGDATE_PN,	1},
	{ 1, 9, FD_HJIYU_PN	,	1},
	{ 1,10, FD_HUSEN_PN	,	1},

	{ -1,-1, -1,	1},
};

static DNP_POS SKeiPos[] = {
	//合計部分
	{ 1, 66, FD_DBVAL_PN,	0},
	{ 1, 67, FD_DBZEI_PN,	0},

	{ -1,-1, -1,	1},
};


// CNSDenpDlg ダイアログ

IMPLEMENT_DYNAMIC(CNSDenpDlg, CDnpBaseDlg)

CNSDenpDlg::CNSDenpDlg(CWnd* pParent /*=NULL*/)
	: CDnpBaseDlg(CNSDenpDlg::IDD, pParent)
{
	SetDataLineCount(6);
	InitDnpBase( NSHeadPos, NDataPos, NKeiPos );
	m_Type = KIND_NYUKIN;

	m_pParent = pParent;
}

CNSDenpDlg::~CNSDenpDlg()
{
}

void CNSDenpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDnpBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_diag[0]);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL2, m_diag[1]);
	DDX_Control(pDX, IDC_ICSDISPJOB, m_Djob );
	DDX_Control(pDX, IDC_ICSDISPBKMK, m_Bkmk );
	
	DDX_Control(pDX, IDC_IMAGE1, m_image1);
	DDX_Control(pDX, IDC_IMAGE2, m_image2);
	DDX_Control(pDX, IDC_IMAGE3, m_image3);
	DDX_Control(pDX, IDC_IMAGE4, m_image4);
	DDX_Control(pDX, IDC_IMAGE5, m_image5);
	DDX_Control(pDX, IDC_IMAGE6, m_image6);
	DDX_Control(pDX, IDC_IMAGE7, m_image7);
	DDX_Control(pDX, IDC_IMAGE8, m_image8);

	DDX_Control(pDX, IDC_HUSEN1, m_Husen1);
	DDX_Control(pDX, IDC_HUSEN2, m_Husen2);
	DDX_Control(pDX, IDC_HUSEN3, m_Husen3);
	DDX_Control(pDX, IDC_HUSEN4, m_Husen4);
	DDX_Control(pDX, IDC_HUSEN5, m_Husen5);
	DDX_Control(pDX, IDC_HUSEN6, m_Husen6);
	DDX_Control(pDX, IDC_HUSEN7, m_Husen7);

	DDX_Control(pDX, IDC_SCROLLBAR1, m_scBar);
}


BEGIN_MESSAGE_MAP(CNSDenpDlg, CDnpBaseDlg)
ON_WM_VSCROLL()
ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CNSDenpDlg メッセージ ハンドラ

BOOL CNSDenpDlg::OnInitDialog()
{
	CDnpBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	InitBaseDiag( m_diag, sizeof m_diag / sizeof m_diag[0] );

	// 未使用
	m_image7.ShowWindow(SW_HIDE);
	m_Husen7.ShowWindow(SW_HIDE);


	ICSDialog::OnInitDialogEX();

	set_scrollbar( &m_scBar, -1, DP_SCROLL_NL, DP_SCROLL_NL_N );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//--------------------------------------------------
// 入出金の切り替え
//		int type	KIND_NYUKIN(入金)
//					KIND_SYUKIN(出金)
//
//--------------------------------------------------
void CNSDenpDlg::ChangeDenpType( int type )
{
	DIAGRAM_ATTRIBUTE	da;
	DIAGRAM_DATA		dd;

	int idx1[] = {6,7,8,9,10,11,12};
	int idx2[] = {74,75,76,77,78,79,80,81};
	int i, max;
	COLORREF	cl;

	cl = (type == KIND_NYUKIN) ? RGB_RED : RGB_BLUE;

	max = sizeof idx1 / sizeof idx1[0];
	for( i = 0; i < max; i++ ) {
		m_diag[0].GetAttribute( idx1[i], (LPUNKNOWN)&da );
		da.attr_fcolor = cl;
		m_diag[0].SetAttribute( idx1[i], (LPUNKNOWN)&da, FALSE );
	}

	if( type == KIND_NYUKIN )
			dd.data_disp = "入 金 伝 票";
	else	dd.data_disp = "出 金 伝 票";

	m_diag[0].SetData( 6, (LPUNKNOWN)&dd );
	m_diag[0].Refresh();

	//項目名色変更
	max = sizeof idx2 / sizeof idx2[0];
	int n;
	for( i = 0; i < max; i++ ) {
		m_diag[1].GetAttribute( idx2[i], (LPUNKNOWN)&da );
		da.attr_fcolor = cl;
		m_diag[1].SetAttribute( idx2[i], (LPUNKNOWN)&da, FALSE );

		n = idx2[i];
	}
	n++;
	//線の色変更
	for( i = n; i <= (n+12); i++ ) {
		m_diag[1].GetAttribute( i, (LPUNKNOWN)&da );
		da.attr_lfcolor = cl;
		m_diag[1].SetAttribute( i, (LPUNKNOWN)&da, FALSE );
	}
	m_diag[1].Refresh();

	if( type == KIND_NYUKIN ) {
		InitDnpBase( NSHeadPos, NDataPos, NKeiPos );
	}
	else {
		InitDnpBase( NSHeadPos, SDataPos, SKeiPos );
	}

	m_Type = type;
}



BEGIN_EVENTSINK_MAP(CNSDenpDlg, CDnpBaseDlg)
	ON_EVENT(CNSDenpDlg, IDC_ICSDIAG8CTRL1, 1, CNSDenpDlg::EditONIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CNSDenpDlg, IDC_ICSDIAG8CTRL1, 2, CNSDenpDlg::EditOFFIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CNSDenpDlg, IDC_ICSDIAG8CTRL1, 3, CNSDenpDlg::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CNSDenpDlg, IDC_ICSDIAG8CTRL2, 1, CNSDenpDlg::EditONIcsdiag8ctrl2, VTS_I2)
	ON_EVENT(CNSDenpDlg, IDC_ICSDIAG8CTRL2, 2, CNSDenpDlg::EditOFFIcsdiag8ctrl2, VTS_I2)
	ON_EVENT(CNSDenpDlg, IDC_ICSDIAG8CTRL2, 3, CNSDenpDlg::TerminationIcsdiag8ctrl2, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CNSDenpDlg, IDC_ICSDIAG8CTRL2, 7, CNSDenpDlg::KanaTermIcsdiag8ctrl2, VTS_I2 VTS_BSTR)

	ON_EVENT_RANGE(CNSDenpDlg, IDC_ICSDBEDT_KASITKY, IDC_ICSDBEDT_KMINP, 5, CNSDenpDlg::ImeEndCompositionIcsdbedt8ctrl, VTS_I4 VTS_I2 VTS_BSTR VTS_BSTR)
	ON_EVENT_RANGE(CNSDenpDlg, IDC_ICSDBEDT_KASITKY, IDC_ICSDBEDT_KMINP, 1, CNSDenpDlg::TerminationIcsdbedt8ctrl, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT_RANGE(CNSDenpDlg, IDC_ICSDBEDT_KASITKY, IDC_ICSDBEDT_KMINP, 3, CNSDenpDlg::SetFocusIcsdbedt8ctrl, VTS_I4 )
	ON_EVENT_RANGE(CNSDenpDlg, IDC_ICSDBEDT_KASITKY, IDC_ICSDBEDT_KMINP, 2, CNSDenpDlg::KillFocusIcsdbedt8ctrl, VTS_I4 VTS_I4)
	ON_EVENT_RANGE(CNSDenpDlg, IDC_ICSDBEDT_KASITKY, IDC_ICSDBEDT_KMINP, 4, CNSDenpDlg::KanaTerminationIcsdbedtctrl, VTS_I4 VTS_BSTR)
END_EVENTSINK_MAP()


void CNSDenpDlg::EditONIcsdiag8ctrl1(short index)
{
	CDnpBaseDlg::DiagEditON( IDC_ICSDIAG8CTRL1, index );
}

void CNSDenpDlg::EditOFFIcsdiag8ctrl1(short index)
{
	DIAGRAM_DATA		dd;
	diag_getdata( IDC_ICSDIAG8CTRL1, index, &dd, CIcsdiag8ctrl );

	CDnpBaseDlg::DiagTermination(IDC_ICSDIAG8CTRL1, index, 0, 0, (LPUNKNOWN)&dd);
}

void CNSDenpDlg::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	CDnpBaseDlg::DiagTermination(IDC_ICSDIAG8CTRL1, index, nChar, length, data);
}

void CNSDenpDlg::EditONIcsdiag8ctrl2(short index)
{
	CDnpBaseDlg::DiagEditON( IDC_ICSDIAG8CTRL2, index );
}

void CNSDenpDlg::EditOFFIcsdiag8ctrl2(short index)
{
	DIAGRAM_DATA		dd;
	diag_getdata( IDC_ICSDIAG8CTRL2, index, &dd, CIcsdiag8ctrl );

	CDnpBaseDlg::DiagTermination(IDC_ICSDIAG8CTRL2, index, 0, 0, (LPUNKNOWN)&dd);
}

void CNSDenpDlg::TerminationIcsdiag8ctrl2(short index, short nChar, short length, LPUNKNOWN data)
{
	CDnpBaseDlg::DiagTermination(IDC_ICSDIAG8CTRL2, index, nChar, length, data);
}


void CNSDenpDlg::KanaTermIcsdiag8ctrl2(short index, LPCTSTR kana)
{
	CDnpBaseDlg::DiagKanaTermination(IDC_ICSDIAG8CTRL2, index, kana);
}


void CNSDenpDlg::SetFocusIcsdbedt8ctrl( UINT ID )
{
	CDnpBaseDlg::SetFocusDBEDT( ID );
}

void CNSDenpDlg::KillFocusIcsdbedt8ctrl( UINT ID, long inplen)
{
	CDnpBaseDlg::KillFocusDBEDT( ID, inplen );
}

void CNSDenpDlg::TerminationIcsdbedt8ctrl(UINT ID, long nChar, long inplen, long kst)
{
	CDnpBaseDlg::TerminationDBEDT( ID, nChar, inplen, kst );
}

void CNSDenpDlg::ImeEndCompositionIcsdbedt8ctrl(UINT ID, short nChar, LPCTSTR string, LPCTSTR ystring)
{
	CDnpBaseDlg::ImeEndDBEDT( ID, nChar, string, ystring );
}

void CNSDenpDlg::KanaTerminationIcsdbedtctrl(UINT ID, LPCTSTR Kana)
{
	CDnpBaseDlg::KanaTermDBEDT(ID, Kana);
}


void CNSDenpDlg::PostNcDestroy()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	delete this;

//	CDnpBaseDlg::PostNcDestroy();
}


//--------------------------------------------------
//	ダイアグの設定
//	int bmon	部門ありか？
//	int koji	工事ありか？
//--------------------------------------------------
void CNSDenpDlg::diag_inz()
{
	// ヘッド部の部門・工事 状態セット
	DIAGRAM_ATTRIBUTE da;

	if( m_BmonLNG ) {
		m_diag[0].ModifyItem( 10, DIAG_MDFY_OPAQUE );
		m_diag[0].ModifyItem( 3, DIAG_MDFY_EDIT );

		m_diag[0].GetAttribute( 3, (LPUNKNOWN)&da );
		da.attr_inpmax = m_BmonLNG;
		m_diag[0].SetAttribute( 3, (LPUNKNOWN)&da, TRUE );
	}
	else {
		m_diag[0].ModifyItem( 10, DIAG_MDFY_TRANSPARENT );
		m_diag[0].ModifyItem( 3, DIAG_MDFY_READONLY );
	}

	if( m_KojiLNG ) {
		m_diag[0].ModifyItem( 11, DIAG_MDFY_OPAQUE );
		m_diag[0].ModifyItem( 4, DIAG_MDFY_EDIT );

		m_diag[0].GetAttribute( 3, (LPUNKNOWN)&da );
		da.attr_inpmax = m_KojiLNG;
		m_diag[0].SetAttribute( 3, (LPUNKNOWN)&da, TRUE );
	}
	else {
		m_diag[0].ModifyItem( 11, DIAG_MDFY_TRANSPARENT );
		m_diag[0].ModifyItem( 4, DIAG_MDFY_READONLY );
	}
	if (m_SyoNumLNG) {
		m_diag[0].ModifyItem(12, DIAG_MDFY_OPAQUE);
		m_diag[0].ModifyItem(13, DIAG_MDFY_EDIT);

		m_diag[0].GetAttribute(13, (LPUNKNOWN)&da);
		da.attr_inpmax = m_SyoNumLNG;
		m_diag[0].SetAttribute(13, (LPUNKNOWN)&da, TRUE);
	}
	else {
		m_diag[0].ModifyItem(12, DIAG_MDFY_TRANSPARENT);
		m_diag[0].ModifyItem(13, DIAG_MDFY_READONLY);
	}
	// 変動事由・振替科目
	DIAGRAM_DATA		dd;
	dd.data_imgdata  = NULL;

	dd.data_disp.Format( "%s  付箋", p_DnpView->get_hjnptitle() );
	m_diag[1].SetData( 77, (LPUNKNOWN)&dd );

	CDnpBaseDlg::diag_inz();
}

//----------------------------------------------------
//	伝票モード変更による表示
//----------------------------------------------------
void CNSDenpDlg::ChangeJob( int job )
{
	if( job == JOB_INPUT )
		m_Djob.ShowWindow(SW_HIDE);
	else {
		DispScanTitle( &m_Djob );
		m_Djob.ShowWindow(SW_SHOW);
	}
}


//----------------------------------------------------
//	取消表示用の赤ラインインデックスを返す
//		取り消しラインは2本だが、各行の先頭を返す
//----------------------------------------------------
int	 CNSDenpDlg::del_idx( int ln ) 
{ 
	const int top_del = 88;
	int add = 0;
	
	add = (ln-1) * 2;

	return top_del + add; 
};


//----------------------------------------------------
//	取消表示用の矩形を取得 スクリーン座標
//----------------------------------------------------
void CNSDenpDlg::GetDeleteRect( int ln, CRect& delrect )
{
	CRect rect, rc, tmp;
	int pn;

	if( m_Type == KIND_NYUKIN )
			pn = FD_CRED_PN;
	else	pn = FD_DEBT_PN;

	GetRect( ln, pn, rc );
	rect.left = rc.left;
	int nTmp;
	nTmp = rc.bottom - rc.top;
	nTmp -= 3;
	nTmp /= 2;

	rect.top = rc.top + nTmp;
	rect.bottom = rect.top + 3;

	if( m_Type == KIND_NYUKIN )
			pn = FD_CSKBN_PN;
	else	pn = FD_DSKBN_PN;
	GetRect( ln, pn, tmp );
	rect.right = tmp.right;

	delrect = rect;
}


//----------------------------------------------------
//	1行の矩形を取得 スクリーン座標
//----------------------------------------------------
void CNSDenpDlg::GetLineRect( int ln, CRect& rect )
{
	CRect rc;
	int pn;

	if( m_Type == KIND_NYUKIN )
			pn = FD_CRED_PN;
	else	pn = FD_DEBT_PN;

	GetRect( ln, pn, rc );
	rect.top	= rc.top;
	rect.left	= rc.left;
	rect.bottom	= rc.bottom;

	if( m_Type == KIND_NYUKIN )
			pn = FD_CSKBN_PN;
	else	pn = FD_DSKBN_PN;

	GetRect( ln, pn, rc );
	rect.right = rc.right;
}


//-------------------------------------------------------
//	自動仕訳状態を 摘要タイトル部分の色変更で表現
//
//-------------------------------------------------------
void CNSDenpDlg::AutoSiwake( int mode ) 
{	
	// 色の変更
	DIAGRAM_ATTRIBUTE	da;
	short index = 75;

	m_diag[1].GetAttribute( index, (LPUNKNOWN)&da );

	da.attr_bcolor = (mode == MODE_AUTO) ? RGB_WINE_GREEN : RGB_WHITE;

	m_diag[1].SetAttribute( index, (LPUNKNOWN)&da, TRUE );
}


//-------------------------------------------------------
//	仕訳ラインの色を変更する
//
//-------------------------------------------------------
void CNSDenpDlg::SetDataLineColor( int ln, COLORREF back, COLORREF txt, char* syz_chg )
{
	CDnpBaseDlg::SetDataLineColor( ln, back,txt,syz_chg );

	if( ln > 0 ) {
		// 付箋横の空白部分の色
		int sidx = 68;
		int index = sidx + (ln-1);

		DIAGRAM_ATTRIBUTE	da;

		m_diag[1].GetAttribute( index, (LPUNKNOWN)&da );

		da.attr_bcolor	= back;
		da.attr_frcolor	= back;

		m_diag[1].SetAttribute( index, (LPUNKNOWN)&da, TRUE );
	}
}



void CNSDenpDlg::DispBaseKmk( BOOL bDisp, CString& str )
{
	if( ! bDisp ) {
		m_Bkmk.ShowWindow( SW_HIDE );
	}
	else {
		m_Bkmk.SetCaption( str );
		m_Bkmk.ShowWindow( SW_SHOW );

		CRect rect, move;

		m_Djob.GetWindowRect( rect );
		ScreenToClient( rect );

		m_Bkmk.GetWindowRect( move );
		ScreenToClient( move );
		int wd = move.Width();

		if( p_DnpView->IsJob() == JOB_INPUT ) {
			move.left = rect.left;
			move.right = move.left + wd;
		}
		else {
			move.left = rect.right;
			move.right = move.left + wd;
		}

		m_Bkmk.MoveWindow( move );
	}
}


void CNSDenpDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	if( pScrollBar && ::IsWindow(m_scBar.m_hWnd) ) {
		int curPos = m_scBar.GetScrollPos();
TRACE("\nnPos = %d, curPos = %d\n", nPos, curPos );
		switch( nSBCode ) {
		case SB_LINEDOWN:
			p_DnpView->Vs_Linemove( 1 );	TRACE( "SB_LINEDOWN\n" );	
			break;
		case SB_LINEUP:
			p_DnpView->Vs_Linemove( -1 );	TRACE( "SB_LINEUP\n" );	
			break;
		case SB_PAGEDOWN:
			p_DnpView->Vs_Linemove( DP_SCROLL_NL_N );	TRACE( "SB_PAGEDOWN\n" );
			break;
		case SB_PAGEUP:
			p_DnpView->Vs_Linemove( -DP_SCROLL_NL_N );	TRACE( "SB_PAGEUP\n");
			break;
		case SB_THUMBTRACK:
			p_DnpView->Vs_Linemove( nPos - curPos );	TRACE( "SB_THUMBTRACK n-cur %d\n", nPos-curPos );	
			break;
		case SB_THUMBPOSITION:
			TRACE( "SB_THUMBPOSITION\n" );	break;
		case SB_BOTTOM:
			TRACE( "SB_BOTTOM\n" );	break;
		case SB_ENDSCROLL:
			TRACE( "SB_ENDSCROLL\n" );	break;
		case SB_TOP:
			TRACE( "SB_TOP\n" );	break;		
		}
	}


	CDnpBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}


//スクロールバーメッセージ
int CNSDenpDlg::ScrollbarMsgFunc( UINT Msg, WPARAM wParam, LPARAM lParam )
{
	PostMessage(Msg, wParam, (LPARAM)m_scBar.m_hWnd);
	return 0;
}


BOOL CNSDenpDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	if( zDelta > 0 ) {
		ScrollbarMsgFunc( WM_VSCROLL, MAKELONG(SB_LINEUP, 0), 0 );
	}
	else {
		ScrollbarMsgFunc( WM_VSCROLL, MAKELONG(SB_LINEDOWN, 0), 0 );
	}

	return CDnpBaseDlg::OnMouseWheel(nFlags, zDelta, pt);
}


//
void CNSDenpDlg::DispLineInfo( int nowln )
{
	DIAGRAM_DATA		dd;
	dd.data_imgdata  = NULL;
	dd.data_disp.Format( "最大%3d行 [%3d/%3d]", DP_SCROLL_NL, nowln, DP_SCROLL_NL );
	m_diag[1].SetData( 95, (LPUNKNOWN)&dd );
}
