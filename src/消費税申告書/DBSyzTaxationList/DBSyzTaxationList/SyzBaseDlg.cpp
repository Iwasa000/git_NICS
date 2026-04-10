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
	, m_pTaxListData(NULL)
	, m_pSnHeadData(NULL)
	, m_pArith(NULL)
	, m_pParent(pParent)
	, m_JcContinuePrint(-1)
{
//2016.02.23 INSERT START
	//LOGBRUSH	lb;
	//lb.lbStyle = BS_SOLID;
	//lb.lbColor = PALETTERGB( 255, 0, 0 );
	//m_hBrushLock.CreateBrushIndirect( &lb );
//2016.02.23 INSERT END
}

CSyzBaseDlg::~CSyzBaseDlg(void)
{
}

BOOL CSyzBaseDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	ICSDialog::OnInitDialogEX();

	m_bInitFlg = TRUE;

//2016.02.23 INSERT START
	Make_StaticFont();

	m_InputLock.DefaultColor( FALSE );
	COLORREF color;
	color = PALETTERGB( 255, 0, 0 );
	m_InputLock.SetBackColor(color);
	color = PALETTERGB( 255, 255, 255 );
	m_InputLock.SetTextColor(color);
//2016.02.23 INSERT END

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

BOOL CSyzBaseDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	//'14.08.05
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

		//'14.08.06
		else if( pMsg->wParam == VK_F7 ){
			m_pParent->PostMessageA( WM_KEYDOWN, VK_F7 );
			return TRUE;
		}

		//'14.10.08
		else if( pMsg->wParam == VK_F8 ){
			m_pParent->PostMessageA( WM_KEYDOWN, VK_F8 );
			return TRUE;
		}


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
		//<-----------------------------------

	}


	return ICSDialog::PreTranslateMessage(pMsg);
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
	GetScrollInfo( SB_VERT, &SI );
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
//		ScrollInfo.nMax  = (b.bottom + 80) - a.top + ScrollInfo.nPos - m_cmPos;
		ScrollInfo.nMax  = (b.bottom + 30) - a.top + ScrollInfo.nPos - m_cmPos;
		ScrollInfo.nPage = a.bottom - a.top;
		SetScrollInfo( SB_VERT, &ScrollInfo );
		m_wPos = 0;
		m_ScW = 15;
	}
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

//-----------------------------------------------------------------------------
// スクロール再描画
//-----------------------------------------------------------------------------
// 引数	mpos	：	動かす位置
//-----------------------------------------------------------------------------
void CSyzBaseDlg::SetRedrawScrollToIdx( int nowpos )
{
	RECT	rectA, rectB;
	GetWindowRect( &rectA );
	diag_getrect( IDC_ICSDIAGCTRL1, nowpos, &rectB, CIcsdiagctrl );
	if( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
		int	cpos = GetScrollPos( SB_VERT );
		SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	}
}

//-----------------------------------------------------------------------------
// スクロール再描画
//-----------------------------------------------------------------------------
// 引数	mpos	：	動かす位置
//-----------------------------------------------------------------------------
void CSyzBaseDlg::SetRedrawScrollToIdx8( int nowpos )
{
	RECT	rectA, rectB;
	GetWindowRect( &rectA );
	diag_getrect( IDC_ICSDIAG8CTRL1, nowpos, &rectB, CIcsdiagctrl );
	if( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
		int	cpos = GetScrollPos( SB_VERT );
		SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	}
}

//-----------------------------------------------------------------------------
// 初期情報のセット
//-----------------------------------------------------------------------------
// 引数	pTaxListData	：	課税取引金額計算表データ
//		pSnHeadData		：	消費税ヘッダ情報
//		pZmSub			：	公益クラス
//		pArith			：	倍長演算
//-----------------------------------------------------------------------------
// 返送値	0			：	正常終了
//			-1			：	エラー
//-----------------------------------------------------------------------------
int CSyzBaseDlg::InitInfo( CH26TaxListData **pTaxListData, CSnHeadData **pSnHeadData, CDBNpSub *pZmSub, CArithEx *pArith , CShzCalc **Scalc )// 集計クラス追加'14.08.06
{
	ASSERT( pTaxListData );
	if( pTaxListData == NULL ){
		return -1;
	}
	m_pTaxListData = pTaxListData;

	ASSERT( pSnHeadData );
	if( pSnHeadData == NULL ){
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	ASSERT( pZmSub );
	if( pZmSub == NULL ){
		return -1;
	}
	m_pZmSub = pZmSub;

	ASSERT( pArith );
	if( pArith == NULL ){
		return -1;
	}
	m_pArith = pArith;

	//------->集計クラス追加'14.08.06
	ASSERT( Scalc );
	if( Scalc == NULL ){
		return -1;
	}
	m_Scalc = Scalc;
	//<------------------------------


	return 0;
}

//-----------------------------------------------------------------------------
// 画面表示
//-----------------------------------------------------------------------------
// 引数	mode	：	
//-----------------------------------------------------------------------------
void CSyzBaseDlg::DispList( int mode )
{
	// 基底クラスでは何もしない
}

//------>14.08.05
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
//<-----------------


//-----------------------------------------------------------------------------
// フォーカスセット
//-----------------------------------------------------------------------------
// 引数	selno	：	
//-----------------------------------------------------------------------------
void CSyzBaseDlg::SetItemPosition( )
{
	// 基底クラスでは何もしない
}

//2016.02.23 INSERT START
int CSyzBaseDlg::Make_StaticFont()
{
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
	CWnd *item;
	item = GetDlgItem(IDC_STATIC_INPUTLOCK);

	if( item == NULL ){
		return;
	}

	if( Sn_Sign4&0x01){
		GetDlgItem(IDC_STATIC_INPUTLOCK)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_INPUTLOCK)->ShowWindow(TRUE);
	}else{
		GetDlgItem(IDC_STATIC_INPUTLOCK)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_INPUTLOCK)->EnableWindow(FALSE);
	}
}

void CSyzBaseDlg::Switch_KamokuEdit( unsigned short id, short index )
{

	if((*m_pSnHeadData)->Sn_Sign4&0x01){ //入力ロック
		return;
	}

	DIAGRAM_ATTRIBUTE	DA;
	diag_getattr( id, index, &DA, CIcsdiagctrl );
	
	if( (*m_pTaxListData)->Head.KHkoke == 0 ){
		diag_modify( id, index, DIAG_MDFY_EDIT, CIcsdiagctrl );
		DA.attr_bcolor = BC_CREAM;
	}else{
		diag_modify( id, index, DIAG_MDFY_READONLY, CIcsdiagctrl );
		DA.attr_bcolor = BC_GRAY;
	}

	diag_setattr( id, index, &DA, FALSE, CIcsdiagctrl );
}
//2016.02.23 INSERT END
void CSyzBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INPUTLOCK, m_InputLock);
}

#ifdef _20190628_EXTEND_
//-----------------------------------------------------------------------------
// 平成継続出力？
//-----------------------------------------------------------------------------
// 返送値	true	：	平成継続出力
//			false	：	令和切替出力
//-----------------------------------------------------------------------------
bool CSyzBaseDlg::IsJcContinuePrint()
{
	if( m_JcContinuePrint == -1 ){
		CSyzUtil	util;
		m_JcContinuePrint = 0;
		if( util.IsJcContinuePrint(*m_pSnHeadData) ){
			m_JcContinuePrint = 1;
		}
	}

	if( m_JcContinuePrint == 1 ){
		return true;
	}
	else{
		return false;
	}
}
#else
// この対応で、m_JcContinuePrintは不要になりましたが、変数としては残しています。
//-----------------------------------------------------------------------------
// 平成継続出力？
//-----------------------------------------------------------------------------
// 返送値	true	：	平成継続出力
//			false	：	令和切替出力
//-----------------------------------------------------------------------------
bool CSyzBaseDlg::IsJcContinuePrint()
{
	CSyzUtil	util;
	if( util.IsJcContinuePrint(*m_pSnHeadData) ){
		return true;
	}
	return false;
}
#endif

//-----------------------------------------------------------------------------
// 30年プラス印字？
//-----------------------------------------------------------------------------
// 返送値	true	：	平成継続なので令和１年を平成３１年に変更する為に、プラス３０年をする
//			false	：	上記以外
//-----------------------------------------------------------------------------
bool CSyzBaseDlg::Is30PlusPrint()
{
	if( IsJcContinuePrint() && (*m_pTaxListData)->YEAR && ((*m_pTaxListData)->Y_GENGO==0x05) ){
		return true;
	}
	else{
		return false;
	}
}
