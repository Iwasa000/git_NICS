// ShinSpecific51.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinSpecific51.h"
#include "ShinSpecific51Idx.h"
#include "TaxationListView.h"

//== 定数 ==
const	int	BottomIndex = 11;
const	int	StartIndex = 30;

// CShinSpecific51 ダイアログ

IMPLEMENT_DYNAMIC(CShinSpecific51, CSpcBaseDlg)

CShinSpecific51::CShinSpecific51(CWnd* pParent /*=NULL*/)
	: CSpcBaseDlg(CShinSpecific51::IDD, pParent)
	, m_zeiType(ID_ICSSH_SPC_8PER)
{
}

CShinSpecific51::CShinSpecific51(int zeitype, CWnd* pParent /*=NULL*/)
	: CSpcBaseDlg(CShinSpecific51::IDD, pParent)
	, m_zeiType(zeitype)
{
}

CShinSpecific51::~CShinSpecific51()
{
}

void CShinSpecific51::DoDataExchange(CDataExchange* pDX)
{
	CSpcBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Specific51);
}


BEGIN_MESSAGE_MAP(CShinSpecific51, CSpcBaseDlg)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CShinSpecific51 メッセージ ハンドラ

BOOL CShinSpecific51::OnInitDialog()
{
	CSpcBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	// 初期設定
	SpecificInit51();

	CSpcBaseDlg::OnInitDialogEX();

	m_initfg = 1;

	// 確定時は全入力不可
	int Flg = 0;
	if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
		Flg = 0;
	}
	else{
		if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
			Flg = 1;
		}
	}

//--> '15.03.26 CUT START
//	if( Flg ){
//		// 実額入力
//		Color_Change( IDC_ICSDIAG8CTRL1, ID510, 0 );
//		m_Specific51.ModifyItem( ID510, DIAG_MDFY_EDIT );
//	}
//	else{
//		// 入力不可
//		Color_Change( IDC_ICSDIAG8CTRL1, ID510, 1 );
//		m_Specific51.ModifyItem( ID510, DIAG_MDFY_READONLY );
//	}
//<-- '15.03.26 CUT END

//	return TRUE;  // return TRUE unless you set the focus to a control
	return FALSE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

BOOL CShinSpecific51::PreTranslateMessage(MSG* pMsg)
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

void CShinSpecific51::OnSize(UINT nType, int cx, int cy)
{
	CSpcBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( m_initfg ){
		RECT	a, b;

		GetWindowRect( &a );

		m_Specific51.GetRect( BottomIndex, (LPUNKNOWN)&b );

#ifdef	_CLOSE
		SCROLLINFO ScrollInfo;
		if ( GetScrollInfo( SB_VERT, &ScrollInfo ) == FALSE )
			ScrollInfo.nPos = 0;
		ScrollInfo.nMin  = 0;
		ScrollInfo.nMax  = (b.bottom + 30) - a.top + ScrollInfo.nPos;
		ScrollInfo.nPage = a.bottom - a.top;
		SetScrollInfo( SB_VERT, &ScrollInfo );

		if ( m_svscll )		m_svscll = m_sclmax;
		m_sclmax  = ScrollInfo.nMax - ScrollInfo.nPage + 1;
		m_pagesiz = ScrollInfo.nPage;
		if ( !m_svscll )	m_svscll = m_sclmax;

		int	cpos = ScrollInfo.nPos;
		if ( cpos < 0 ) cpos = ScrollInfo.nMin;
		scpos = (float)cpos;
		smax = (float)m_sclmax;
		svmax = (float)m_svscll;
		scpos *= smax / svmax;

		SetScrollPos( SB_VERT, 0, TRUE );
		ScrollWindow( 0, cpos, NULL );
		sykscroll((int)scpos);
#endif
	}
}

HBRUSH CShinSpecific51::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSpcBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
	if( nCtlColor == CTLCOLOR_DLG ){
		hbr = Ibrsh_ShinSpecific;
	}

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

void CShinSpecific51::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
#ifdef	_CLOSE
	int	cpos = GetScrollPos( SB_VERT );
	int	mpos;
	
	if ( nSBCode == SB_LINEDOWN )			mpos = cpos+15;
	else if ( nSBCode == SB_LINEUP )		mpos = cpos-15;
	else if ( nSBCode == SB_PAGEDOWN )		mpos = cpos+m_pagesiz;
	else if ( nSBCode == SB_PAGEUP )		mpos = cpos-m_pagesiz;
	else if ( nSBCode == SB_THUMBTRACK )	mpos = nPos;

	if ( nSBCode == SB_LINEDOWN || nSBCode == SB_LINEUP || nSBCode == SB_PAGEDOWN ||
			nSBCode == SB_PAGEUP || nSBCode == SB_THUMBTRACK ) {
		if ( mpos < 0 )		mpos = 0;
		if ( mpos > m_sclmax )	mpos = m_sclmax;
		SetScrollPos( SB_VERT, mpos, TRUE );
		ScrollWindow( 0, cpos-mpos, NULL );
	}

	CSpcBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
#endif
}

//-----------------------------------------------------------------------------
// 再描画
//-----------------------------------------------------------------------------
void CShinSpecific51::sykscroll( int mpos )
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

/*
//----->yoshida150316
//-----------------------------------------------------------------------------
// セットポジション
//-----------------------------------------------------------------------------
void CShinSpecific51::SetItemPosition( int IndexNo )
{
	int	Index;
	if( IndexNo ){
		Index = IndexNo;
	}
	else{
		Index = StartIndex;
	}

	m_Specific51.SetFocus();
	// 確定時は全入力不可
	if( ((*m_pSnHeadData)->Sn_Sign4&0x01) ){
		return;
	}

	if( (*m_pSnHeadData)->SVmzsw == 0 ){
		m_Specific51.SetPosition( Index );
	}

	CRect	rc;
	this->GetClientRect( &rc );
	this->InvalidateRect( &rc );

	sykscroll( 0 );

}
*/
//<--------------------

/*
//-----------------------------------------------------------------------------
// 入力項目　色変え
//-----------------------------------------------------------------------------
void CShinSpecific51::Color_Change( unsigned short id, short index, int sign )
{
	DIAGRAM_ATTRIBUTE	DA;

	diag_getattr( id, index, &DA, CIcsdiagctrl );
	switch( sign ){
		case 1 :		// 入力不可	（白色）
			DA.attr_bcolor = BC_WHITE;
			break;
		case 2 : 		// ロック	（灰色）
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
}*/

//-----------------------------------------------------------------------------
// 初期設定
//-----------------------------------------------------------------------------
void CShinSpecific51::SpecificInit51()
{
	// 倍長演算指定
	m_pArith->l_defn( 0x16 );

	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;
	if( m_zeiType == ID_ICSSH_SPC_8PER ){
		DiagData.data_disp = _T("６．３\n\n１０８");
	}
	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
		DiagData.data_disp = _T("４\n\n１０５");
	}
	diag_setdata( IDC_ICSDIAG8CTRL1, 5, (struct IUnknown *)&DiagData, CIcsdiagctrl );

//--> '15.04.14 INS START
//2017.03.01 UPDATE START
//	if( m_zeiType == ID_ICSSH_SPC_8PER ){
//		DiagData.data_disp = _T("　課税仕入れ等(税率6.3％)にのみ使途が特定されている特定収入\n　(「課税仕入れ等に係る特定収入」)　(計算表２(1)⑰Ｃ)");//yoshida150429
////		DiagData.data_disp = _T("　課税売上げ等にのみ使途が特定されている特定収入（「課税仕入れ\n　等に係る特定収入」）　（計算表２(1)⑰のＢ（8%））");
//	}
//	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
//		DiagData.data_disp = _T("　課税仕入れ等(税率4％)にのみ使途が特定されている特定収入\n　(「課税仕入れ等に係る特定収入」)　(計算表２(1)⑰Ｂ)");//yoshida150429
////		DiagData.data_disp = _T("　課税売上げ等にのみ使途が特定されている特定収入（「課税仕入れ\n　等に係る特定収入」）　（計算表２(1)⑰のＢ（5%））");
//	}

	if( ((CTaxationListView*)m_pParent)->IsKeikaType_Keisansyo() == TRUE ){
		DiagData.data_disp = _T("　課税仕入れ等にのみ使途が特定されている特定収入\n　(「課税仕入れ等に係る特定収入」)　(計算表２(1)⑰のＢ)");
	}else{
		if( m_zeiType == ID_ICSSH_SPC_8PER ){
			DiagData.data_disp = _T("　課税仕入れ等(税率6.3％)にのみ使途が特定されている特定収入\n　(「課税仕入れ等に係る特定収入」)　(計算表２(1)⑰Ｃ)");
		}
		else if( m_zeiType == ID_ICSSH_SPC_5PER ){
			DiagData.data_disp = _T("　課税仕入れ等(税率4％)にのみ使途が特定されている特定収入\n　(「課税仕入れ等に係る特定収入」)　(計算表２(1)⑰Ｂ)");
		}
	}
//2017.03.01 UPDATE END
	diag_setdata( IDC_ICSDIAG8CTRL1, 3, (struct IUnknown *)&DiagData, CIcsdiagctrl );
//<-- '15.04.14 INS END

	SpecificDisp51();

	IndexControl();

	SetItemPosition();//yoshida150316
//	SetItemPosition(0);
}

//-----------------------------------------------------------------------------
// 表示
//-----------------------------------------------------------------------------
void CShinSpecific51::SpecificDisp51( void )
{
	char			val[MONY_BUF_SIZE]={0};
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

//--> '15.03.26 CUT START
//	// 経過措置チェック
//	if( (*m_pSnHeadData)->SVmzsw == 1 ){
//
//		Color_Change( IDC_ICSDIAG8CTRL1, ID510 , 0 );//色変更yoshida150313
//		diag_modify( IDC_ICSDIAG8CTRL1, ID510, DIAG_MDFY_EDIT , CIcsdiagctrl );//旧は入力可能だったのであけたyoshida150313
//
////		Color_Change( IDC_ICSDIAG8CTRL1, ID510 , 1 );
////		diag_modify( IDC_ICSDIAG8CTRL1, ID510, DIAG_MDFY_READONLY, CIcsdiagctrl );
//
//	}
//	else{
//		// 確定時は全入力不可
//		if( !((*m_pSnHeadData)->Sn_Sign4&0x01) ){
//			Color_Change( IDC_ICSDIAG8CTRL1, ID510 , 0 );
//			diag_modify( IDC_ICSDIAG8CTRL1, ID510, DIAG_MDFY_EDIT, CIcsdiagctrl );
//		}
//	}
//<-- '15.03.26 CUT END

	// 1.調整前の課税仕入れ等の税額の合計					[SpKgzg]
	if( m_zeiType == ID_ICSSH_SPC_8PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKgz63, sizeof(DiagData.data_val) );
	}
	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKgz4, sizeof(DiagData.data_val) );
	}
	else{
		ASSERT( !_T("不正税率") );
		memset( DiagData.data_val, '\0', sizeof(DiagData.data_val) );
	}
	diag_setdata( IDC_ICSDIAG8CTRL1, ID510, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 2.課税仕入れ等のみに使途が特定されている特定収入		[SpTsyu + SpKsyu + SpHsyu]
	memset( val, '\0', sizeof(val) );
	if( m_zeiType == ID_ICSSH_SPC_8PER ){
//-- '15.04.25 --
//		m_pArith->l_add( val, (*m_pSpcListData)->stval.SpTsyu63, (*m_pSpcListData)->stval.SpKsyu63 );
//		m_pArith->l_add( val, val, (*m_pSpcListData)->stval.SpHsyu63 );
//---------------
		memmove( val, (*m_pSpcListData)->stval.SpTsyu63, sizeof(val) );
//---------------
	}
	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
//-- '15.04.25 --
//		m_pArith->l_add( val, (*m_pSpcListData)->stval.SpTsyu4, (*m_pSpcListData)->stval.SpKsyu4 );
//		m_pArith->l_add( val, val, (*m_pSpcListData)->stval.SpHsyu4 );
//---------------
		memmove( val, (*m_pSpcListData)->stval.SpTsyu4, sizeof(val) );
//---------------
	}
	memmove( DiagData.data_val, val, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID511, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 3.②×６．３／１０８									[SpTszg + SpKszg]
	if( m_zeiType == ID_ICSSH_SPC_8PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpGszg63, sizeof(DiagData.data_val) );
	}
	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpGszg4, sizeof(DiagData.data_val) );
	}
	diag_setdata( IDC_ICSDIAG8CTRL1, ID512, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 4.①－③												[SpKgzg-(SpTszg + SpKszg)]
	if( m_zeiType == ID_ICSSH_SPC_8PER ){
		m_pArith->l_sub( val, (*m_pSpcListData)->stval.SpKgz63, (*m_pSpcListData)->stval.SpGszg63 );
	}
	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
		m_pArith->l_sub( val, (*m_pSpcListData)->stval.SpKgz4, (*m_pSpcListData)->stval.SpGszg4 );
	}
	memmove( DiagData.data_val, val, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID513, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 5.調整割合 分子										[SpFsyu]
//-- '15.04.25 --
//	if( m_zeiType == ID_ICSSH_SPC_8PER ){
//		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpFsyu63, sizeof(DiagData.data_val) );
//	}
//	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
//		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpFsyu4, sizeof(DiagData.data_val) );
//	}
//---------------
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpFsyu, sizeof(DiagData.data_val) );
//---------------
	diag_setdata( IDC_ICSDIAG8CTRL1, ID514, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 5.調整割合 分母										[SpTuwb + SpFsyu]
	memset( val, '\0', sizeof(val) );
	m_pArith->l_add( val, (*m_pSpcListData)->stval.SpTuwb, (*m_pSpcListData)->stval.SpFsyu );
	memmove( DiagData.data_val, val, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID515, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 6.④×⑤												[SpTwag]
	if( m_zeiType == ID_ICSSH_SPC_8PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTwag63, sizeof(DiagData.data_val) );
	}
	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTwag4, sizeof(DiagData.data_val) );
	}
	diag_setdata( IDC_ICSDIAG8CTRL1, ID516, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 7.③＋⑥												[SpGgtg]
	if( m_zeiType == ID_ICSSH_SPC_8PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpGgt63, sizeof(DiagData.data_val) );
	}
	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpGgt4, sizeof(DiagData.data_val) );
	}
	diag_setdata( IDC_ICSDIAG8CTRL1, ID517, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 8.仕入れ控除税額										[SpSizg]
	if( m_zeiType == ID_ICSSH_SPC_8PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpSiz63, sizeof(DiagData.data_val) );
	}
	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpSiz4, sizeof(DiagData.data_val) );
	}
	diag_setdata( IDC_ICSDIAG8CTRL1, ID518, (struct IUnknown *)&DiagData, CIcsdiagctrl );

}

//-----------------------------------------------------------------------------
// 再計算
//-----------------------------------------------------------------------------
void CShinSpecific51::Specific51_Calq( void )
{
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

	SpecificDisp51();
}

BEGIN_EVENTSINK_MAP(CShinSpecific51, CSpcBaseDlg)
	ON_EVENT(CShinSpecific51, IDC_ICSDIAG8CTRL1, 1, CShinSpecific51::EditONIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinSpecific51, IDC_ICSDIAG8CTRL1, 2, CShinSpecific51::EditOFFIcsdiag8ctrl1, VTS_I2)
END_EVENTSINK_MAP()

void CShinSpecific51::EditONIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

void CShinSpecific51::EditOFFIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
//--> '15.04.29 INS START
	// 計算表5(1)で編集を無くしたのでカット
	return;
//<-- '15.04.29 INS END

	ASSERT( m_pSpcListData );
	if( m_pSpcListData == NULL ){
		return;
	}
	ASSERT( *m_pSpcListData );
	if( *m_pSpcListData == NULL ){
		return;
	}
//--> '15.03.18 INS START
	if( IsWindowVisible() == FALSE ){
		return;
	}
//<-- '15.03.18 INS END

	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	switch( index ){
		case ID510	:	
			diag_getdata( IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			if( m_zeiType == ID_ICSSH_SPC_8PER ){
				memmove( (*m_pSpcListData)->stval.SpKgz63, DiagData.data_val, MONY_BUF_SIZE );
			}
			else if( m_zeiType == ID_ICSSH_SPC_5PER ){
				memmove( (*m_pSpcListData)->stval.SpKgz4, DiagData.data_val, MONY_BUF_SIZE );
			}
			// 合計
			m_pArith->l_add( (*m_pSpcListData)->stval.SpKgzg, (*m_pSpcListData)->stval.SpKgz4, (*m_pSpcListData)->stval.SpKgz63 );
			break;
		default     :
			break;
	}
	// 再計算
	Specific51_Calq();
}

void CShinSpecific51::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	char	VK_FLG;
	short	wTerm;

	if( nChar == 0 ){
		return;
	}

	// SHIFT+TABをTABに、TABをENTERに変換
	VK_FLG = 0;
	VK_FLG = (char)::GetKeyState( VK_SHIFT );
	if( nChar == VK_TAB ){
		if( VK_FLG&0x80 ){
			wTerm = VK_TAB;
		}
		else{
			wTerm = VK_RETURN;
		}
	}
	else{
		wTerm = nChar;
	}

	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;
	switch( index ){
		case ID510 :
			diag_getdata( IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			if( m_zeiType == ID_ICSSH_SPC_8PER ){
				memmove( (*m_pSpcListData)->stval.SpKgz63, DiagData.data_val, MONY_BUF_SIZE );
			}
			else if( m_zeiType == ID_ICSSH_SPC_5PER ){
				memmove( (*m_pSpcListData)->stval.SpKgz4, DiagData.data_val, MONY_BUF_SIZE );
			}
			// 合計
			m_pArith->l_add( (*m_pSpcListData)->stval.SpKgzg, (*m_pSpcListData)->stval.SpKgz4, (*m_pSpcListData)->stval.SpKgz63 );
			// 再計算
			Specific51_Calq();
			break;
		default :
			break;
	}
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void CShinSpecific51::IndexControl( void )
{
	int	Flg = 0;
//--> '15.03.26 CUT START
//	// 確定時は全入力不可
//	if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
//		Flg = 0;
//	}
//	else{
//		if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
//			Flg = 1;
//		}
//		else{
//			Flg = 0;
//		}
//	}
//<-- '15.03.26 CUT END
//--> '15.04.02 CUT START
//	ATRIB_MOD( IDC_ICSDIAG8CTRL1, ID510, Flg, (*m_pSnHeadData)->Sn_Sign4, Flg );
//<-- '15.04.02 CUT END
}

void CShinSpecific51::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
//	CSpcBaseDlg::OnOK();
}

void CShinSpecific51::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
//	CSpcBaseDlg::OnCancel();
}




//----->yoshida150316
//-----------------------------------------------------------------------------
// セットポジション
//-----------------------------------------------------------------------------
void CShinSpecific51::SetItemPosition( )
{


	int index;

	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAG8CTRL1 ) {
		if( (index = m_Specific51.GetPosition()) != -1) {
			m_Specific51.SetPosition(index);
			m_Specific51.SetFocus();
			return;
		}
	}

	this->SetFocus();
	if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
		m_Specific51.SetPosition(ID510);
	}
	else{
	}
	SetRedrawScroll( 0 );


}
//<-------------------------