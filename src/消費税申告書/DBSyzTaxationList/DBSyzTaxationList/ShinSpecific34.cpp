// ShinSpecific34.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinSpecific34.h"
#include "ShinSpecific34Idx.h"
#include "TaxationListView.h"

//== 定数 ==
const	int	BottomIndex = 20;
const	int	StartIndex = 33;

// CShinSpecific34 ダイアログ

IMPLEMENT_DYNAMIC(CShinSpecific34, CSpcBaseDlg)

CShinSpecific34::CShinSpecific34(CWnd* pParent /*=NULL*/)
	: CSpcBaseDlg(CShinSpecific34::IDD, pParent)
	, m_wPos(0)
	, m_cmPos(0)
	, m_ScW(0)
{
}

CShinSpecific34::~CShinSpecific34()
{
}

void CShinSpecific34::DoDataExchange(CDataExchange* pDX)
{
	CSpcBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Specific34);
}


BEGIN_MESSAGE_MAP(CShinSpecific34, CSpcBaseDlg)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CShinSpecific34 メッセージ ハンドラ

BOOL CShinSpecific34::OnInitDialog()
{
	CSpcBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	// 初期設定
	SpecificInit34();

	m_initfg = 1;

	CSpcBaseDlg::OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

BOOL CShinSpecific34::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	//----->yoshida150310
	if( pMsg->message == WM_KEYDOWN ){
		if( pMsg->wParam == VK_END ){
			if( m_pParent ){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_END );
			}
			return TRUE;
		}
	}

	return CSpcBaseDlg::PreTranslateMessage(pMsg);
	//<-------------------

	/*yoshida150310
	if( pMsg->message == WM_KEYDOWN ){

		if( GetCombineKeyDown(VK_SHIFT) && (VK_F1<=pMsg->wParam) && (pMsg->wParam<=VK_F12) ){
			return TRUE;
		}

		if( (pMsg->wParam==VK_NEXT) || (pMsg->wParam==VK_PRIOR) ){
			SCROLLINFO ScrollInfo;
			int	c = GetScrollPos( SB_VERT );
			GetScrollInfo( SB_VERT, &ScrollInfo );
			if( pMsg->wParam == VK_NEXT ){
				sykscroll( c+ScrollInfo.nPage );
			}
			if( pMsg->wParam == VK_PRIOR ){
				sykscroll( c-ScrollInfo.nPage );
			}
			return 1;
		}
		if( pMsg->wParam == VK_END ){
			// 更新
			if( m_pParent ){
				//yoshida150310
				m_pParent->PostMessageA( WM_KEYDOWN, VK_END );
//				m_pParent->PostMessageA( WM_CLOSE, 0 );
			}
			return 1;
		}
		if( pMsg->wParam == VK_ESCAPE ){
			return 1;
		}
		if( pMsg->wParam == VK_F5 ){
			if( m_pParent ){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_F5 );
			}
			return 1;
		}
		if( pMsg->wParam == VK_F4 ){
			if( m_pParent ){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_F4 );
			}
			return 1;
		}
		if( pMsg->wParam == VK_F9 ){
			if( m_pParent ){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_F9 );
			}
			return 1;
		}
		if( pMsg->wParam == VK_F11 ){
			if( m_pParent ){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_F11 );
			}
			return 1;
		}
	}

	return CSpcBaseDlg::PreTranslateMessage(pMsg);
	*/
}

void CShinSpecific34::OnSize(UINT nType, int cx, int cy)
{
	if( m_initfg ){
		SetScrollPos( SB_VERT, 0, TRUE );
		ScrollWindow( 0, m_cmPos, NULL );
		m_cmPos = 0;
	}

	CSpcBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( m_initfg ){
		RECT	a, b;
		SCROLLINFO ScrollInfo;
		GetWindowRect( &a );
		m_Specific34.GetRect( BottomIndex, (LPUNKNOWN)&b );
		if( GetScrollInfo( SB_VERT, &ScrollInfo ) == FALSE ){
			ScrollInfo.nPos = 0;
		}
		ScrollInfo.nMin  = 0;
		ScrollInfo.nMax  = (b.bottom + 30) - a.top + ScrollInfo.nPos - m_cmPos;
		ScrollInfo.nPage = a.bottom - a.top;
		SetScrollInfo( SB_VERT, &ScrollInfo );
		m_wPos = 0;
		m_ScW = 15;
	}
}

HBRUSH CShinSpecific34::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSpcBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
	if( nCtlColor == CTLCOLOR_DLG ){
		hbr = Ibrsh_ShinSpecific;
	}

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

void CShinSpecific34::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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
			if( m_wPos >= (SI.nMax-(int)SI.nPage) ){
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

	CSpcBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

//-----------------------------------------------------------------------------
// 再描画
//-----------------------------------------------------------------------------
void CShinSpecific34::sykscroll( int mpos )
{
	int		smax;

	smax = GetScrollLimit( SB_VERT );
	if( mpos < 0 ){
		mpos = 0;
	}
	if( mpos > smax ){
		mpos = smax;
	}
	WPARAM wParam;
	wParam = mpos << 16;
	wParam |= (int)SB_THUMBTRACK;
	PostMessage( WM_VSCROLL, wParam );
}

//-----------------------------------------------------------------------------
// セットポジション
//-----------------------------------------------------------------------------
void CShinSpecific34::SetItemPosition( int IndexNo )
{
//--> '15.05.01 INS START
	this->SetFocus();
//<-- '15.05.01 INS END
	m_Specific34.SetFocus();
	// 確定時は全入力不可
	if( ((*m_pSnHeadData)->Sn_Sign4&0x01) ){
		return;
	}

	CRect	rc;
	this->GetClientRect( &rc );
	this->InvalidateRect( &rc );

	sykscroll( 0 );

}

/*
//-----------------------------------------------------------------------------
// 入力項目　色変え
//-----------------------------------------------------------------------------
void CShinSpecific34::Color_Change( unsigned short id, short index, int sign )
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
		case 3 :		// 特殊色①	（青色）
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
}*/

//-----------------------------------------------------------------------------
// 初期設定
//-----------------------------------------------------------------------------
void CShinSpecific34::SpecificInit34( void )
{
	// 倍長演算指定
	m_pArith->l_defn( 0x16 );

//2017.03.01 INSERT START
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;
	if( ((CTaxationListView*)m_pParent)->IsKeikaType_Keisansyo() == TRUE ){
		if( (*m_pSnHeadData)->SVmzsw == 1 ){
			DiagData.data_disp = _T("　資産の譲渡等の対価の額の合計額（計算表１⑥Ｃ）");
		}else{
			DiagData.data_disp = _T("　資産の譲渡等の対価の額の合計額（計算表１⑥）");
		}
	}else{
		DiagData.data_disp = _T("　資産の譲渡等の対価の額の合計額（計算表１⑤）");
	}
	diag_setdata( IDC_ICSDIAG8CTRL1, 2, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	if( ((CTaxationListView*)m_pParent)->IsKeikaType_Keisansyo() == TRUE ){
		if( (*m_pSnHeadData)->SVmzsw == 1 ){
			DiagData.data_disp = _T("　特定収入の合計額（計算表２(1)⑰Ａ）");
		}else{
			DiagData.data_disp = _T("　特定収入の合計額（計算表２(1)⑰のＡ）");
		}
	}else{
		DiagData.data_disp = _T("　特定収入の合計額（計算表２(1)⑰Ａ）");
	}
	diag_setdata( IDC_ICSDIAG8CTRL1, 3, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	if( ((CTaxationListView*)m_pParent)->IsKeikaType_Keisansyo() == TRUE ){
		if( (*m_pSnHeadData)->SVmzsw == 1 ){
			DiagData.data_disp = _T("　資産の譲渡等の対価の額の合計額（計算表１⑥Ｃ）");
		}else{
			DiagData.data_disp = _T("　資産の譲渡等の対価の額の合計額（計算表１⑥）");
		}
	}else{
		DiagData.data_disp = _T("　資産の譲渡等の対価の額の合計額（計算表１⑤）");
	}
	diag_setdata( IDC_ICSDIAG8CTRL1, 17, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	if( ((CTaxationListView*)m_pParent)->IsKeikaType_Keisansyo() == TRUE ){
		if( (*m_pSnHeadData)->SVmzsw == 1 ){
			DiagData.data_disp = _T("　課税仕入れ等に係る特定収入以外の特定収入\n　（計算表２(1)⑰Ｄ）");
		}else{
			DiagData.data_disp = _T("　課税仕入れ等に係る特定収入以外の特定収入\n　（計算表２(1)⑰のＣ）");
		}
	}else{
		DiagData.data_disp = _T("　課税仕入れ等に係る特定収入以外の特定収入\n　（計算表２(1)⑰Ｄ）");
	}
	diag_setdata( IDC_ICSDIAG8CTRL1, 18, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	if( ((CTaxationListView*)m_pParent)->IsKeikaType_Keisansyo() == TRUE ){
		if( (*m_pSnHeadData)->SVmzsw == 1 ){
			DiagData.data_disp = _T("計算表５(1)⑦、(2)⑭、(3)⑨へ");
		}else{
			DiagData.data_disp = _T("");
		}
	}else{
		DiagData.data_disp = _T("");
	}
	diag_setdata( IDC_ICSDIAG8CTRL1, 31, (struct IUnknown *)&DiagData, CIcsdiagctrl );
//2017.03.01 INSERT END

	SpecificDisp34();

	SetItemPosition(0);

}

//-----------------------------------------------------------------------------
// 表示
//-----------------------------------------------------------------------------
void CShinSpecific34::SpecificDisp34( void )
{
	char			val[MONY_BUF_SIZE]={0};
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	// 資産の譲渡等の対価の額の合計
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTuwb, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID340, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 特定収入の合計額
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpGsyu, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID341, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 分母の額
	memset( val, '\0', sizeof(val) );
	m_pArith->l_add( val, (*m_pSpcListData)->stval.SpTuwb, (*m_pSpcListData)->stval.SpGsyu );
	memmove( DiagData.data_val, val, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID342, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 特定収入割合
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTswr, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID343, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 資産の譲渡等の対価の額の合計
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTuwb, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID344, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 課税仕入れに係る特定収入以外の特定収入
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpFsyu, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID345, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 分母の額
	memset( val, '\0', sizeof(val) );
	m_pArith->l_add( val, (*m_pSpcListData)->stval.SpTuwb, (*m_pSpcListData)->stval.SpFsyu );
	memmove( DiagData.data_val, val, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID346, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 調整割合・分子	
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpFsyu, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID347, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 調整割合・分母	
	memset( val, '\0', sizeof(val) );
	m_pArith->l_add( val, (*m_pSpcListData)->stval.SpTuwb, (*m_pSpcListData)->stval.SpFsyu );
	memmove( DiagData.data_val, val, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID348, (struct IUnknown *)&DiagData, CIcsdiagctrl );
}

//-----------------------------------------------------------------------------
// 再計算
//-----------------------------------------------------------------------------
void CShinSpecific34::Specific34_Calq( void )
{
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	// 特定収入割合
	m_pArith->l_par100( (unsigned char*)(*m_pSpcListData)->stval.SpTswr, (unsigned char*)(*m_pSpcListData)->stval.SpGsyu,
												(unsigned char*)(*m_pSpcListData)->stval.SpTswb, 1, 9 );
	// 特定収入割合
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTswr, 6 );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID343, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 特定収入に係る課税仕入れ等の税額計算
//-- '15.03.07 --
//	if( (*m_pSnHeadData)->SVmzsw == 1 ){
//		(*m_pSpcListData)->SPassOnCalqEx( (*m_pSnHeadData) );
//	}
//	else{
//		(*m_pSpcListData)->SPassOffCalqEx( (*m_pSnHeadData) );
//	}
//---------------
	unsigned char	svSn_Sign2 = (*m_pSnHeadData)->Sn_Sign2;
	(*m_pSpcListData)->SPassOffCalqMain( *m_pSnHeadData );
	BOOL	IsChgDoSpc = FALSE;
	if( (svSn_Sign2&0x02) != ((*m_pSnHeadData)->Sn_Sign2&0x02) ){
		IsChgDoSpc = TRUE;
	}
	((CTaxationListView*)m_pParent)->LinkHonpyoData(IsChgDoSpc);
//---------------

	SpecificDisp34();

}

void CShinSpecific34::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
//	CSpcBaseDlg::OnOK();
}

void CShinSpecific34::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
//	CSpcBaseDlg::OnCancel();
}
