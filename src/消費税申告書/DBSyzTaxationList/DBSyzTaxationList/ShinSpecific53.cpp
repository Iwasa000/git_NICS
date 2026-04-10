// ShinSpecific53.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinSpecific53.h"
#include "ShinSpecific53Idx.h"
#include "TaxationListView.h"

//== 定数 ==
const	int	BottomIndex = 13;
const	int	StartIndex = 37;

// CShinSpecific53 ダイアログ

IMPLEMENT_DYNAMIC(CShinSpecific53, CSpcBaseDlg)

CShinSpecific53::CShinSpecific53(CWnd* pParent /*=NULL*/)
	: CSpcBaseDlg(CShinSpecific53::IDD, pParent)
	,m_wPos(0)
	,m_cmPos(0)
	,m_ScW(0)
	,m_zeiType(ID_ICSSH_SPC_8PER)
{
}

CShinSpecific53::CShinSpecific53(int zeitype, CWnd* pParent /*=NULL*/)
	: CSpcBaseDlg(CShinSpecific53::IDD, pParent)
	,m_wPos(0)
	,m_cmPos(0)
	,m_ScW(0)
	,m_zeiType(zeitype)
{
}

CShinSpecific53::~CShinSpecific53()
{
}

void CShinSpecific53::DoDataExchange(CDataExchange* pDX)
{
	CSpcBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Specific53);
}


BEGIN_MESSAGE_MAP(CShinSpecific53, CSpcBaseDlg)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CShinSpecific53 メッセージ ハンドラ

BOOL CShinSpecific53::OnInitDialog()
{
	CSpcBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	// 初期設定
	SpecificInit53();

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
//		Color_Change( IDC_ICSDIAG8CTRL1, ID530, 0 );
//		m_Specific53.ModifyItem( ID530, DIAG_MDFY_EDIT );
//	}
//	else{
//		// 入力不可
//		Color_Change( IDC_ICSDIAG8CTRL1, ID530, 2 );
//		m_Specific53.ModifyItem( ID530, DIAG_MDFY_READONLY );
//	}
//<-- '15.03.26 CUT END

//	return TRUE;  // return TRUE unless you set the focus to a control
	return FALSE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

BOOL CShinSpecific53::PreTranslateMessage(MSG* pMsg)
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
//			GetAllData();
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
	}

	return CSpcBaseDlg::PreTranslateMessage(pMsg);
	*/
}

void CShinSpecific53::OnSize(UINT nType, int cx, int cy)
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
		m_Specific53.GetRect( BottomIndex, (LPUNKNOWN)&b );
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

HBRUSH CShinSpecific53::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSpcBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
	if( nCtlColor == CTLCOLOR_DLG ){
		hbr = Ibrsh_ShinSpecific;
	}

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

void CShinSpecific53::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	if( pScrollBar ){
		return;
	}
	SCROLLINFO SI;
	GetScrollInfo( SB_VERT, &SI );
	int nowPos = m_wPos;

	switch(nSBCode) {
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

	CSpcBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

//-----------------------------------------------------------------------------
// 再描画
//-----------------------------------------------------------------------------
void CShinSpecific53::sykscroll( int mpos )
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

/*---->yoshida150316
//-----------------------------------------------------------------------------
// セットポジション
//-----------------------------------------------------------------------------
void CShinSpecific53::SetItemPosition( int IndexNo )
{
	int	Index;
	if( IndexNo ){
		Index = IndexNo;
	}
	else{
		Index = StartIndex;
	}

	m_Specific53.SetFocus();
	// 確定時は全入力不可
	if( ((*m_pSnHeadData)->Sn_Sign4&0x01) ){
		return;
	}

	if( (*m_pSnHeadData)->SVmzsw == 0 ){
		m_Specific53.SetPosition( Index );
	}

	CRect	rc;
	this->GetClientRect( &rc );
	this->InvalidateRect( &rc );

	sykscroll( 0 );

}
//<---------------------

/*
//-----------------------------------------------------------------------------
// 入力項目　色変え
//-----------------------------------------------------------------------------
void CShinSpecific53::Color_Change( unsigned short id, short index, int sign )
{
	DIAGRAM_ATTRIBUTE	DA;

	diag_getattr( id, index, &DA, CIcsdiagctrl );
	switch( sign ){
		case 1 :
			DA.attr_bcolor = BC_WHITE;	
			break;		// 入力不可	（白色）
		case 2 : 
			DA.attr_bcolor = BC_GRAY;		
			break;		// ロック	（灰色）
		case 3 : 
			DA.attr_bcolor = BC_BLUE;		
			break;		// 特殊色①	（青色）
		case 4 : 
			DA.attr_bcolor = BC_GREEN;		
			break;		// 特殊色②	（緑色）	
		case 5 : 
			DA.attr_bcolor = BC_GREEN_L;	
			break;		// 特殊色③	（薄緑色）	 
		default: 	
			DA.attr_bcolor = BC_CREAM;		
			break;		// 実額入力	（クリーム色） 
	}
	diag_setattr( id, index, &DA, FALSE, CIcsdiagctrl );
}*/

//-----------------------------------------------------------------------------
// 初期設定
//-----------------------------------------------------------------------------
void CShinSpecific53::SpecificInit53( void )
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
//		DiagData.data_disp = _T("　課税仕入れ等(税率6.3％)にのみ使途が特定されている特定収入\n　(「課税仕入れ等に係る特定収入」)　(計算表２(１)⑰Ｃ)");//yoshida150429
////		DiagData.data_disp = _T("　課税仕入れ等にのみ使途が特定されている特定収入\n　（「課税仕入れ等に係る特定収入」　（計算表２（１）のＢ(8%)）");
//	}
//	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
//		DiagData.data_disp = _T("　課税仕入れ等(税率4％)にのみ使途が特定されている特定収入\n　(「課税仕入れ等に係る特定収入」)　(計算表２(１)⑰Ｂ)");//yoshida150429
////		DiagData.data_disp = _T("　課税仕入れ等にのみ使途が特定されている特定収入\n　（「課税仕入れ等に係る特定収入」　（計算表２（１）のＢ(5%)）");
//	}

	if( ((CTaxationListView*)m_pParent)->IsKeikaType_Keisansyo() == TRUE ){
		DiagData.data_disp = _T("　課税仕入れ等にのみ使途が特定されている特定収入\n　(「課税仕入れ等に係る特定収入」)　(計算表２(１)⑰のＢ)");
	}else{
		if( m_zeiType == ID_ICSSH_SPC_8PER ){
			DiagData.data_disp = _T("　課税仕入れ等(税率6.3％)にのみ使途が特定されている特定収入\n　(「課税仕入れ等に係る特定収入」)　(計算表２(１)⑰Ｃ)");
		}
		else if( m_zeiType == ID_ICSSH_SPC_5PER ){
			DiagData.data_disp = _T("　課税仕入れ等(税率4％)にのみ使途が特定されている特定収入\n　(「課税仕入れ等に係る特定収入」)　(計算表２(１)⑰Ｂ)");
		}
	}

//2017.03.01 UPDATE END
	diag_setdata( IDC_ICSDIAG8CTRL1, 3, (struct IUnknown *)&DiagData, CIcsdiagctrl );
//<-- '15.04.14 INS END

	SpecificDisp53();


	IndexControl();//yoshida150313


	SetItemPosition();//yoshida150316
//	SetItemPosition(0);
}

//-----------------------------------------------------------------------------
// 表示
//-----------------------------------------------------------------------------
void CShinSpecific53::SpecificDisp53( void )
{
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;
	char			val[MONY_BUF_SIZE]={0};

//--> '15.03.26 CUT START
//	// 経過措置チェック
//	if( (*m_pSnHeadData)->SVmzsw == 1 ){
//		Color_Change( IDC_ICSDIAG8CTRL1, ID530 , 0 );//色変更yoshida150313
//		diag_modify( IDC_ICSDIAG8CTRL1, ID530, DIAG_MDFY_EDIT , CIcsdiagctrl );//旧は入力可能だったのであけたyoshida150313
//		
////		Color_Change( IDC_ICSDIAG8CTRL1, ID530 , 1 );
////		diag_modify( IDC_ICSDIAG8CTRL1, ID530, DIAG_MDFY_READONLY, CIcsdiagctrl );
//	}
//	else{
//		// 確定時は全入力不可
//		if( !((*m_pSnHeadData)->Sn_Sign4&0x01) ){
//			Color_Change( IDC_ICSDIAG8CTRL1, ID530 , 0 );
//			diag_modify( IDC_ICSDIAG8CTRL1, ID530, DIAG_MDFY_EDIT, CIcsdiagctrl );
//		}
//	}
//<-- '15.03.26 CUT END

	// 1. 調整前の課税仕入れ等の税額の合計						[SpKgzg]
	if( m_zeiType == ID_ICSSH_SPC_8PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKgz63, sizeof(DiagData.data_val) );
	}
	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKgz4, sizeof(DiagData.data_val) );
	}
	diag_setdata( IDC_ICSDIAG8CTRL1, ID530, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 2. 課税仕入れ等のみに使途が特定されている特定収入		[SpTsyu]
	if( m_zeiType == ID_ICSSH_SPC_8PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTsyu63, sizeof(DiagData.data_val) );
	}
	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTsyu4, sizeof(DiagData.data_val) );
	}
	diag_setdata( IDC_ICSDIAG8CTRL1, ID531, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 3. ②×6.3/108, 4/105									[SpZszg] 
	if( m_zeiType == ID_ICSSH_SPC_8PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpZszg63, sizeof(DiagData.data_val) );
	}
	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpZszg4, sizeof(DiagData.data_val) );
	}
	diag_setdata( IDC_ICSDIAG8CTRL1, ID532, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 4. 課税売上割合	分子									[SpKuws] 
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKuws, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID533, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 4. 課税売上割合　分母									[SpKuwb] 
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKuwb, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID534, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 5. ③×④												[SpTszg] 				
	if( m_zeiType == ID_ICSSH_SPC_8PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTszg63, sizeof(DiagData.data_val) );
	}
	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTszg4, sizeof(DiagData.data_val) );
	}
	diag_setdata( IDC_ICSDIAG8CTRL1, ID535, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 6. ①－⑤												[SpKgzg - SpTszg]
	memset( val, '\0', sizeof(val) );
	if( m_zeiType == ID_ICSSH_SPC_8PER ){
		m_pArith->l_sub( val, (*m_pSpcListData)->stval.SpKgz63, (*m_pSpcListData)->stval.SpTszg63 );
	}
	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
		m_pArith->l_sub( val, (*m_pSpcListData)->stval.SpKgz4, (*m_pSpcListData)->stval.SpTszg4 );
	}
	memmove( DiagData.data_val, val, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID536, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 7. 調整割合		分子									[SpFsyu]
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpFsyu, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID537, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 7. 調整割合		分母									[SpTuwb + SpFsyu]
	memset( val, '\0', sizeof(val) );
	m_pArith->l_add( val, (*m_pSpcListData)->stval.SpTuwb, (*m_pSpcListData)->stval.SpFsyu );
	memmove( DiagData.data_val, val, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID538, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 8. ⑥×⑦												[SpTszg] -> [SpTwag]
	if( m_zeiType == ID_ICSSH_SPC_8PER ){
//		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTszg63, sizeof(DiagData.data_val) );
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTwag63, sizeof(DiagData.data_val) );
	}
	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
//		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTszg4, sizeof(DiagData.data_val) );
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTwag4, sizeof(DiagData.data_val) );
	}
	diag_setdata( IDC_ICSDIAG8CTRL1, ID539, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 9. 特定収入に係る課税仕入れ等の税額						[SpTszg + SpTwag]
	memset( val, '\0', sizeof(val) );
	if( m_zeiType == ID_ICSSH_SPC_8PER ){
		m_pArith->l_add( val, (*m_pSpcListData)->stval.SpTszg63, (*m_pSpcListData)->stval.SpTwag63 );
	}
	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
		m_pArith->l_add( val, (*m_pSpcListData)->stval.SpTszg4, (*m_pSpcListData)->stval.SpTwag4 );
	}
	memmove( DiagData.data_val, val, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID53A, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 10.控除対象仕入税額										[SpSizg]
	if( m_zeiType == ID_ICSSH_SPC_8PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpSiz63, sizeof(DiagData.data_val) );
	}
	else if( m_zeiType == ID_ICSSH_SPC_5PER ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpSiz4, sizeof(DiagData.data_val) );
	}
	diag_setdata( IDC_ICSDIAG8CTRL1, ID53B, (struct IUnknown *)&DiagData, CIcsdiagctrl );
}

//-----------------------------------------------------------------------------
// 再計算
//-----------------------------------------------------------------------------
void CShinSpecific53::Specific53_Calq( void )
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

	SpecificDisp53();
}

BEGIN_EVENTSINK_MAP(CShinSpecific53, CSpcBaseDlg)
	ON_EVENT(CShinSpecific53, IDC_ICSDIAG8CTRL1, 2, CShinSpecific53::EditOFFIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinSpecific53, IDC_ICSDIAG8CTRL1, 3, CShinSpecific53::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinSpecific53::EditOFFIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
//--> '15.04.29 INS START
	// 計算表5(3)で編集を無くしたのでカット
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
		case ID530	:	
			diag_getdata( IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			if( m_zeiType == ID_ICSSH_SPC_8PER ){
				memmove( (*m_pSpcListData)->stval.SpKgz63, DiagData.data_val, MONY_BUF_SIZE );
			}
			else if( m_zeiType == ID_ICSSH_SPC_5PER ){
				memmove( (*m_pSpcListData)->stval.SpKgz4, DiagData.data_val, MONY_BUF_SIZE );
			}
			// 合計
			m_pArith->l_add( (*m_pSpcListData)->stval.SpKgzg, (*m_pSpcListData)->stval.SpKgz63, (*m_pSpcListData)->stval.SpKgz4 );
			break;
		default     :
			break;
	}
	// 再計算
	Specific53_Calq();
}

void CShinSpecific53::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
//--> '15.04.29 INS START
	// 計算表5(3)で編集を無くしたのでカット
	return;
//<-- '15.04.29 INS END

	char	VK_FLG;
	short	wTerm;

	if( nChar == 0 ){
		return;
	}

	// SHIFT+TABをTABに、TABをENTERに変換
	VK_FLG = 0;
	VK_FLG = (char)::GetKeyState( VK_SHIFT );
	if( nChar == VK_TAB ){
		if( VK_FLG&0x80 )
			wTerm = VK_TAB;
		else
			wTerm = VK_RETURN;
	}
	else{
		wTerm = nChar;
	}

	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;
	switch( index ){
		case ID530 :
			diag_getdata( IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			if( m_zeiType == ID_ICSSH_SPC_8PER ){
				memmove( (*m_pSpcListData)->stval.SpKgz63, DiagData.data_val, MONY_BUF_SIZE );
			}
			else if( m_zeiType == ID_ICSSH_SPC_5PER ){
				memmove( (*m_pSpcListData)->stval.SpKgz4, DiagData.data_val, MONY_BUF_SIZE );
			}
			// 合計
			m_pArith->l_add( (*m_pSpcListData)->stval.SpKgzg, (*m_pSpcListData)->stval.SpKgz63, (*m_pSpcListData)->stval.SpKgz4 );
			// 再計算
			Specific53_Calq();
			break;
		default :
			break;
	}
}

void CShinSpecific53::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
//	CSpcBaseDlg::OnOK();
}

void CShinSpecific53::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
//	CSpcBaseDlg::OnCancel();
}


//---------------->yoshida150313

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void CShinSpecific53::IndexControl( void )
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
//	ATRIB_MOD( IDC_ICSDIAG8CTRL1, ID530, Flg, (*m_pSnHeadData)->Sn_Sign4, Flg );
//<-- '15.04.02 CUT END
}


//-----------------------------------------------------------------------------
// セットポジション
//-----------------------------------------------------------------------------
void CShinSpecific53::SetItemPosition( )
{


	int index;

	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAG8CTRL1 ) {
		if( (index = m_Specific53.GetPosition()) != -1) {
			m_Specific53.SetPosition(index);
			m_Specific53.SetFocus();
			return;
		}
	}

	this->SetFocus();
	if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
		m_Specific53.SetPosition(ID530);
	}
	else{
	}
	SetRedrawScroll( 0 );


}

//<------------------------------