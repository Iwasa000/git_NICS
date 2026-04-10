#include "StdAfx.h"
#include "SpcBaseDlg.h"

CSpcBaseDlg::CSpcBaseDlg( UINT id, CWnd* pParent/*=NULL*/ )
	: ICSDialog(id, pParent)
	, m_pParent(pParent)
	, m_pSnHeadData(NULL)
	, m_pSpcListData(NULL)
	, m_pArith(NULL)
	, m_pZmSub(NULL)
	, m_svscll(0)
	, m_pagesiz(0)
	, m_sclmax(0)
	, m_initfg(0)
{
}

CSpcBaseDlg::~CSpcBaseDlg(void)
{
}

BOOL CSpcBaseDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	ICSDialog::OnInitDialogEX();

	// TODO:  ここに初期化を追加してください

	Make_StaticFont();

	m_InputLock.DefaultColor( FALSE );
	COLORREF color;
	color = PALETTERGB( 255, 0, 0 );
	m_InputLock.SetBackColor(color);
	color = PALETTERGB( 255, 255, 255 );
	m_InputLock.SetTextColor(color);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

HBRUSH CSpcBaseDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = ICSDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。

	// TODO:  既定値を使用したくない場合は別のブラシを返します。

	//if( GetDlgItem(IDC_STATIC_INPUTLOCK) != NULL ){
	//	if(pWnd->m_hWnd==GetDlgItem(IDC_STATIC_INPUTLOCK)->m_hWnd){
	//		pDC->SetTextColor( RGB(255, 255, 255) );
	//		return (HBRUSH)m_hBrushLock;
	//	}
	//}

	return hbr;
}

int CSpcBaseDlg::Make_StaticFont()
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

void CSpcBaseDlg::Static_Update( unsigned char Sn_Sign4 )
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

//-----------------------------------------------------------------------------
// 初期情報のセット
//-----------------------------------------------------------------------------
// 引数	pSpcListData	：	特定収入計算表データ
//		pSnHeadData		：	消費税ヘッダ情報
//		pZmSub			：	公益クラス
//		pArith			：	倍長演算
//-----------------------------------------------------------------------------
// 返送値	0			：	正常終了
//			-1			：	エラー
//-----------------------------------------------------------------------------
int CSpcBaseDlg::InitInfo( CH31SpcListData **pSpcListData, CSnHeadData **pSnHeadData, CDBNpSub *pZmSub, CArithEx *pArith )
{
	ASSERT( pSpcListData );
	if( pSpcListData == NULL ){
		return -1;
	}
	m_pSpcListData = pSpcListData;

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

	return 0;
}

//-----------------------------------------------------------------------------
// 入力項目　色変え
//-----------------------------------------------------------------------------
// 引数	id		：
//		index	：
//		sign	：
//-----------------------------------------------------------------------------
void CSpcBaseDlg::Color_Change( unsigned short id, short index, int sign )
{
	DIAGRAM_ATTRIBUTE	DA;

	diag_getattr( id, index, &DA, CIcsdiagctrl );
	switch( sign ){
		case 1 :		// 入力不可	（白色）
			DA.attr_bcolor = BC_WHITE;
			break;
		case 2 :		// ロック	（灰色）
			DA.attr_bcolor = BC_GRAY;
			break;
		case 3 : 		// 特殊色①	（青色）
			DA.attr_bcolor = BC_BLUE;	
			break;
		case 4 : 		// 特殊色②	（緑色）	
			DA.attr_bcolor = BC_GREEN;	
			break;
		case 5 : 		// 特殊色③	（薄緑色）	 
			DA.attr_bcolor = BC_GREEN_L;
			break;
		default: 		// 実額入力	（クリーム色） 
			DA.attr_bcolor = BC_CREAM;	
			break;
	}
	diag_setattr( id, index, &DA, FALSE, CIcsdiagctrl );
}

BOOL CSpcBaseDlg::PreTranslateMessage(MSG* pMsg)
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
		else if( pMsg->wParam == VK_F7 ){
			m_pParent->PostMessageA( WM_KEYDOWN, VK_F7 );
			return TRUE;
		}
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
	}


	return ICSDialog::PreTranslateMessage(pMsg);
}


//-----------------------------------------------------------------------------
// スクロール再描画
//-----------------------------------------------------------------------------
// 引数	mpos	：	動かす位置
//-----------------------------------------------------------------------------
void CSpcBaseDlg::SetRedrawScroll( int mpos )
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
void CSpcBaseDlg::SetRedrawScrollToIdx( int nowpos )
{
	RECT	rectA, rectB;
	GetWindowRect( &rectA );
	diag_getrect( IDC_ICSDIAG8CTRL1, nowpos, &rectB, CIcsdiagctrl );
	if( (rectA.top>rectB.top) || (rectA.bottom<rectB.bottom) ){
		int	cpos = GetScrollPos( SB_VERT );
		SetRedrawScroll( (cpos+rectB.top-rectA.top)-((rectA.bottom-rectA.top)/2) );
	}
}



BEGIN_MESSAGE_MAP(CSpcBaseDlg, ICSDialog)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


void CSpcBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	ICSDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INPUTLOCK, m_InputLock);
}

void CSpcBaseDlg::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	//	ICSDialog::OnOK();
}

void CSpcBaseDlg::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	//	ICSDialog::OnCancel();
}
