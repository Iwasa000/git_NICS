// ShinSpecific1.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinSpecific1.h"
#include "ShinSpecific1Idx.h"
#include "TaxationListView.h"

//== 定数 ==
const	int	BottomIndex = 9;
const	int	StartIndex = 19;

// CShinSpecific1 ダイアログ

IMPLEMENT_DYNAMIC(CShinSpecific1, CSpcBaseDlg)

CShinSpecific1::CShinSpecific1(CWnd* pParent /*=NULL*/)
	: CSpcBaseDlg(CShinSpecific1::IDD, pParent)
{

	//------>yoshida150310
	m_curwnd = -1;

	m_FcsUpEnd		= -1;
	m_FcsLeftEnd	= -1;
	m_FcsDownEnd	= -1;
	m_FcsRightEnd	= -1;
	//<-------------------

}

CShinSpecific1::~CShinSpecific1()
{
}

void CShinSpecific1::DoDataExchange(CDataExchange* pDX)
{
	CSpcBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Specific1);
}


BEGIN_MESSAGE_MAP(CShinSpecific1, CSpcBaseDlg)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CShinSpecific1 メッセージ ハンドラ

BOOL CShinSpecific1::OnInitDialog()
{
	CSpcBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	// 初期設定
	SpecificInit1();

	m_initfg = 1;

	CSpcBaseDlg::OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

BOOL CShinSpecific1::PreTranslateMessage(MSG* pMsg)
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

		if( pMsg->wParam == VK_F2 ){
			DIAGRAM_DATA   dd;
			if( m_curwnd != -1) {
				short result = ((CIcsdiagctrl*)GetDlgItem( m_curwnd ))->GetPosition();
				diag_getdata( m_curwnd, result, &dd, CIcsdiagctrl );
				if( m_curwnd == IDC_ICSDIAG8CTRL1 ) {
					TerminationIcsdiag8ctrl1( result, (short)pMsg->wParam, 0, (LPUNKNOWN)&dd );
				}
				return TRUE;
			}
		}

	}

	return CSpcBaseDlg::PreTranslateMessage(pMsg);
	//<-------------------


	/*yoshida150310

	if( pMsg->message == WM_KEYDOWN ){

		if( GetCombineKeyDown(VK_SHIFT) && (VK_F1<=pMsg->wParam) && (pMsg->wParam<=VK_F12) ){
			return TRUE;
		}

		if ( pMsg->wParam == VK_NEXT || pMsg->wParam == VK_PRIOR )	{
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

void CShinSpecific1::OnSize(UINT nType, int cx, int cy)
{
	CSpcBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( m_initfg ){
		RECT	a, b;

		GetWindowRect( &a );

		m_Specific1.GetRect( BottomIndex, (LPUNKNOWN)&b );

#ifdef	_CLOSE
		SCROLLINFO ScrollInfo;

		if ( GetScrollInfo( SB_VERT, &ScrollInfo ) == FALSE )
			ScrollInfo.nPos = 0;
		ScrollInfo.nMin  = 0;
		ScrollInfo.nMax  = (b.bottom /*+ 30*/) - a.top + ScrollInfo.nPos;
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

HBRUSH CShinSpecific1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSpcBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
	if( nCtlColor == CTLCOLOR_DLG ){
		hbr = Ibrsh_ShinSpecific;
	}

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

void CShinSpecific1::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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
void CShinSpecific1::sykscroll( int mpos )
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


//---->yoshida150310
/*
//-----------------------------------------------------------------------------
// セットポジション
//-----------------------------------------------------------------------------
void CShinSpecific1::SetItemPosition( int IndexNo )
{
	int	Index;
	if( IndexNo ){
		Index = IndexNo;
	}
	else{
		Index = StartIndex;
		if( ((*m_pSnHeadData)->Sn_TUMIAG&0x01) && (m_pZmSub->zvol->s_sgn1&0xc0) ){
			Index++;
		}
	}

	m_Specific1.SetFocus();
	// 確定時は全入力不可
	if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
		return;
	}
	m_Specific1.SetPosition( Index );

	CRect	rc;
	this->GetClientRect( &rc );
	this->InvalidateRect( &rc );

	sykscroll( 0 );
}
*/
//<----------------------

/*
//-----------------------------------------------------------------------------
// 入力項目　色変え
//-----------------------------------------------------------------------------
void CShinSpecific1::Color_Change( unsigned short id, short index, int sign )
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
}*/

//-----------------------------------------------------------------------------
// 初期設定
//-----------------------------------------------------------------------------
void CShinSpecific1::SpecificInit1( void )
{
	// 倍長演算指定
	m_pArith->l_defn( 0x16 );

	SpecificDisp1();

	IndexControl();

	SetItemPosition();//yoshida150310
//	SetItemPosition(0);
}

//-----------------------------------------------------------------------------
// 表示
//-----------------------------------------------------------------------------
void CShinSpecific1::SpecificDisp1( void )
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	if( ((*m_pSnHeadData)->Sn_TUMIAG&0x01) && (m_pZmSub->zvol->s_sgn1&0xc0) ){
//--> '15.04.15 CUT START
//		Color_Change( IDC_ICSDIAG8CTRL1, ID10 , 1 );
//		diag_modify( IDC_ICSDIAG8CTRL1, ID10, DIAG_MDFY_READONLY, CIcsdiagctrl );
//		// 課税売上
//		Color_Change( IDC_ICSDIAG8CTRL1, ID11 , 0 );
//		diag_modify( IDC_ICSDIAG8CTRL1, ID11, DIAG_MDFY_EDIT, CIcsdiagctrl );
//<-- '15.04.15 CUT END
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKzur, sizeof(DiagData.data_val) );
		diag_setdata( IDC_ICSDIAG8CTRL1, ID11, (struct IUnknown *)&DiagData, CIcsdiagctrl );
//--> '15.04.15 CUT START
//		// 確定時は全入力不可
//		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
//			Color_Change( IDC_ICSDIAG8CTRL1, ID11 , 1 );
//			diag_modify( IDC_ICSDIAG8CTRL1, ID11, DIAG_MDFY_READONLY, CIcsdiagctrl );
//		}
//<-- '15.04.15 CUT END

//--> '15.04.30 INS START
		diag_clear( IDC_ICSDIAG8CTRL1, ID10, TRUE, CIcsdiagctrl );
//<-- '15.04.30 INS END
	}
	else{
		// 課税売上
//--> '15.04.15 CUT START
//		Color_Change( IDC_ICSDIAG8CTRL1, ID10 , 0 );
//		diag_modify( IDC_ICSDIAG8CTRL1, ID10, DIAG_MDFY_EDIT, CIcsdiagctrl );
//<-- '15.04.15 CUT END
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKzur, sizeof(DiagData.data_val) );
		diag_setdata( IDC_ICSDIAG8CTRL1, ID10, (struct IUnknown *)&DiagData, CIcsdiagctrl );
//--> '15.04.15 CUT START
//		Color_Change( IDC_ICSDIAG8CTRL1, ID11 , 1 );
//		diag_modify( IDC_ICSDIAG8CTRL1, ID11, DIAG_MDFY_READONLY, CIcsdiagctrl );
//		// 確定時は全入力不可
//		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
//			Color_Change( IDC_ICSDIAG8CTRL1, ID10 , 1 );
//			diag_modify( IDC_ICSDIAG8CTRL1, ID10, DIAG_MDFY_READONLY, CIcsdiagctrl );
//		}
//<-- '15.04.15 CUT END

//--> '15.04.30 INS START
		diag_clear( IDC_ICSDIAG8CTRL1, ID11, TRUE, CIcsdiagctrl );
//<-- '15.04.30 INS END
	}

	// 免税売上げ
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpMzur, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID12, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 非課税売上げ
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpHkur, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID13, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 資産の譲渡等の対価の額の合計
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTuwb, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID14, (struct IUnknown *)&DiagData, CIcsdiagctrl );

}

#ifdef _20150413_EXTEND_
//-----------------------------------------------------------------------------
// メインウィンドウにメッセージ送信
//-----------------------------------------------------------------------------
void CShinSpecific1::PostTabInitMes()
{
	CWnd* wnd = GetParent();
	if( wnd && wnd->GetParent() ){
		wnd->GetParent()->PostMessage( WM_USER_SHINTAB_INIT );
	}
}
#else
//-----------------------------------------------------------------------------
// メインウィンドウにメッセージ送信
//-----------------------------------------------------------------------------
void CShinSpecific1::PostTabInitMes()
{
	if( m_pParent ){
		((CTaxationListView*)m_pParent)->WriteSpcData( FALSE );
		m_pParent->PostMessage( WM_USER_SHINTAB_INIT );
	}
}
#endif

//-----------------------------------------------------------------------------
// 再計算
//-----------------------------------------------------------------------------
void CShinSpecific1::Specific1_Calq( void )
{
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;
//--> '15.05.18 CUT START
//	char			WORK0[MONY_BUF_SIZE]={0}, WORK1[MONY_BUF_SIZE]={0}, WORK2[MONY_BUF_SIZE]={0};
//	memmove( WORK0, (*m_pSpcListData)->stval.SpKuws, sizeof(WORK0) );
//	memmove( WORK1, (*m_pSpcListData)->stval.SpKuwb, sizeof(WORK1) );
//	memmove( WORK2, (*m_pSpcListData)->stval.SpUwri, sizeof(WORK2) );
//<-- '15.05.18 CUT END
	memset( (*m_pSpcListData)->stval.SpTuwb, '\0', sizeof((*m_pSpcListData)->stval.SpTuwb) );	// 分母
	m_pArith->l_add( (*m_pSpcListData)->stval.SpTuwb, (*m_pSpcListData)->stval.SpTuwb, (*m_pSpcListData)->stval.SpKzur );
	m_pArith->l_add( (*m_pSpcListData)->stval.SpTuwb, (*m_pSpcListData)->stval.SpTuwb, (*m_pSpcListData)->stval.SpMzur );
	m_pArith->l_add( (*m_pSpcListData)->stval.SpTuwb, (*m_pSpcListData)->stval.SpTuwb, (*m_pSpcListData)->stval.SpHkur );
	// 資産の譲渡等の対価の額の合計
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTuwb, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID14, (struct IUnknown *)&DiagData, CIcsdiagctrl );
//--> '15.05.18 CUT START
//	// 売上割合計算
//	// 分子
//	memmove( (*m_pSpcListData)->stval.SpKuws, (*m_pSpcListData)->stval.SpKzur, sizeof((*m_pSpcListData)->stval.SpKuws) );
//	// 分母
//	memmove( (*m_pSpcListData)->stval.SpKuwb, (*m_pSpcListData)->stval.SpTuwb, sizeof((*m_pSpcListData)->stval.SpKuwb) );
//	// 割合
//	m_pArith->l_par100( (unsigned char *)(*m_pSpcListData)->stval.SpUwri, (unsigned char *)(*m_pSpcListData)->stval.SpKuws, (unsigned char *)(*m_pSpcListData)->stval.SpKuwb, 2, 0 );
//<-- '15.05.18 CUT END

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

	SpecificDisp1();

//-- '15.05.18 --
// 課税売上高割合は変動なし！
//	char	PW[MONY_BUF_SIZE]={0};
//	char	moji[64]={0};
//	wsprintf( moji, _T("%d"), (*m_pSnHeadData)->m_UriRatioBorder );
//	m_pArith->l_input( PW, moji );
//	if( (m_pArith->l_cmp((*m_pSpcListData)->stval.SpUwri, PW)<0) && (m_pArith->l_cmp(WORK2, PW)>=0) ){
//		ICSMessageBox( "課税売上割合が９５％未満になりましたので計算表を初期化します。" );
//		PostTabInitMes();
//	}
//	
//	if( (m_pArith->l_cmp((*m_pSpcListData)->stval.SpUwri, PW)>=0) && (m_pArith->l_cmp(WORK2, PW)<0) ){
//		ICSMessageBox( "課税売上割合が９５％を超えましたので計算表を初期化します。" );
//		PostTabInitMes();
//	}
//---------------
}

BEGIN_EVENTSINK_MAP(CShinSpecific1, CSpcBaseDlg)
	ON_EVENT(CShinSpecific1, IDC_ICSDIAG8CTRL1, 1, CShinSpecific1::EditONIcsdiag8ctrl1, VTS_I2)//yoshida150310
	ON_EVENT(CShinSpecific1, IDC_ICSDIAG8CTRL1, 2, CShinSpecific1::EditOFFIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinSpecific1, IDC_ICSDIAG8CTRL1, 3, CShinSpecific1::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinSpecific1::EditOFFIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
//-- '15.05.18 復活
//--> '15.04.29 INS START
//	// 計算表１で編集を無くしたのでカット
//	return;
//<-- '15.04.29 INS END
//-----------------

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
		case ID10	:	
			diag_getdata( IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( (*m_pSpcListData)->stval.SpKzur, DiagData.data_val, MONY_BUF_SIZE );
			break;
		case ID11	:	
			diag_getdata( IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( (*m_pSpcListData)->stval.SpKzur, DiagData.data_val, MONY_BUF_SIZE );
			break;
		case ID12	:	
			diag_getdata( IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( (*m_pSpcListData)->stval.SpMzur, DiagData.data_val, MONY_BUF_SIZE );
			break;
		case ID13	:	
			diag_getdata( IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( (*m_pSpcListData)->stval.SpHkur, DiagData.data_val, MONY_BUF_SIZE );
			break;
		default     :	
			break;
	}
	// 再計算
	Specific1_Calq();
}

void CShinSpecific1::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
//-- '15.05.18 復活
//--> '15.04.29 INS START
//	// 計算表１で編集を無くしたのでカット
//	return;
//<-- '15.04.29 INS END
//---------------

	//------->yoshida150310
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( nChar == 0 ){
		return;
	}

	DIAGRAM_DATA	*idata;
	DIAGRAM_DATA	xdata;

	UINT	flg = 0;

	idata = (DIAGRAM_DATA *)data;
	m_Util.DiagOcxIniz( &xdata );
	m_Util.MoveDiagData( &xdata, idata );

	//フォーカス移動処理対応
	// Shift+TABをTABに、TABをENTERに返還
	short	wTerm;
	char	VK_FLG = 0x00;
	VK_FLG = (char )::GetKeyState( VK_SHIFT );
	if( nChar == VK_TAB ){
		if( VK_FLG & 0x80 ){
			wTerm = VK_TAB;
		}
		else{
			wTerm = VK_RETURN;
		}
	}
	else{
		wTerm = nChar;
	}

	
	int pos = 0;
	if( wTerm==VK_UP ){		
		pos = FocusMoveCheck(index , 1);
		m_Specific1.SetPosition(pos);	
	}
	if( wTerm==VK_DOWN ){
		pos = FocusMoveCheck(index , 0);
		m_Specific1.SetPosition(pos);
	}

	if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) ){
		pos = FocusMoveCheck(index , 2);
		m_Specific1.SetPosition(pos);
	}
	else if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
		pos = FocusMoveCheck(index , 3);
		m_Specific1.SetPosition(pos);
	}
	if( wTerm == VK_DELETE){
//		GetDiagData( index, length, &xdata, -1 );
		//--> '15.05.19 INS START  //yoshida150519
		m_Specific1.DataClear( index, TRUE );
//		m_Specific1.SetData( index, (struct IUnknown *)&xdata );
		m_Specific1.SetPosition(index);
		//<-- '15.05.19 INS END
	}
	
	// 再計算
	Specific1_Calq();

	//--> '15.05.19 INS START  //yoshida150519
	//フォーカスがキー移動したら消えてたのを修正
	m_Specific1.SetPosition(index);
	//<-- '15.05.19 INS END

	SetRedrawScrollToIdx(m_Specific1.GetPosition());

	//<------------------------

	/*yoshida150310

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
		case ID10 :
			// 次項目へ
			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) ){
				diag_setposition( IDC_ICSDIAG8CTRL1, ID12, CIcsdiagctrl );
			}
			break;
		case ID11 :
			// 次項目へ
			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) ){
				diag_setposition( IDC_ICSDIAG8CTRL1, ID12, CIcsdiagctrl );
			}
			break;
		case ID12 :
			// 次項目へ
			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) ){
				diag_setposition( IDC_ICSDIAG8CTRL1, ID13, CIcsdiagctrl );
			}
			// 前項目へ
			if( (wTerm==VK_TAB) || (wTerm==VK_F2) ){
				if( ((*m_pSnHeadData)->Sn_TUMIAG&0x01) && (m_pZmSub->zvol->s_sgn1&0xc0) )
						diag_setposition( IDC_ICSDIAG8CTRL1, ID11, CIcsdiagctrl );
				else	diag_setposition( IDC_ICSDIAG8CTRL1, ID10, CIcsdiagctrl );
			}
			break;
		case ID13 :
			diag_getdata( IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( (*m_pSpcListData)->stval.SpHkur, DiagData.data_val, MONY_BUF_SIZE );
			// 前項目へ
			if( (wTerm==VK_TAB) || (wTerm==VK_F2) ){
				diag_setposition( IDC_ICSDIAG8CTRL1, ID12, CIcsdiagctrl );
			}
			break;
		default :
			break;
	}
	// 再計算
	Specific1_Calq();

	*/

}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void CShinSpecific1::IndexControl( void )
{
	int	Cnt = 0;
	while( 1 ){
		if( Specific1[Cnt].Index == 0 ){
			break;
		}
		int	Flg;
		// 確定時は全入力不可
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			Flg = Specific1[Cnt].AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
				Flg = Specific1[Cnt].UnConnect;
			}
			else{
				Flg = Specific1[Cnt].Connect;
			}
		}
		if( Flg == 2 ){
			if( ((*m_pSnHeadData)->Sn_TUMIAG&0x01) && (m_pZmSub->zvol->s_sgn1&0xc0) ){
				Flg = 0;
			}
		}
		if( Flg == 3 ){
			if( ((*m_pSnHeadData)->Sn_TUMIAG&0x01) && (m_pZmSub->zvol->s_sgn1&0xc0) ){
				Flg = 1;
			}
		}
		ATRIB_MOD( IDC_ICSDIAG8CTRL1, Specific1[Cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, Specific1[Cnt].Connect );
		Cnt++;
	}

	//--->yoshida150310
	SetFocusRange();

	m_Specific1.EnableDelete(0);//DELETEキー１回で削除モード設定
	//<---------------

}

void CShinSpecific1::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
//	CSpcBaseDlg::OnOK();
}

void CShinSpecific1::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
//	CSpcBaseDlg::OnCancel();
}


//------------->yoshida150310

void CShinSpecific1::EditONIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	
/*
	m_Specific1.OffControl( FALSE );

	m_Idx = index;
	if( (m_Idx>=VMIN) && (m_Idx<=VMAX) ){
//		if( m_Idx == VMAX ){
//			m_Lin = m_Idx-VMIN-1;
//		}
		if(( m_Idx == BUYVAL17K )||( m_Idx == BUYVAL17C )||( m_Idx == BUYVAL17 )){
			m_Lin = BUYLINE17;
		}
		else{
			m_Lin = m_Idx-VMIN;
		}
	}
	else{
		m_Lin = -1;
	}
	*/

	m_curwnd = IDC_ICSDIAG8CTRL1;//'14.08.01

}




//-----------------------------------------------------------------------------
// 次フォーカスの場所検索（上下左右動作）
//-----------------------------------------------------------------------------
//　戻り値　：　次のフォーカスインデックス
//-----------------------------------------------------------------------------
int CShinSpecific1::FocusMoveCheck(int Index , int Move)
{
	int idx = 0;
	int i = 0 , j = 0;
	BOOL end_flg = FALSE;

	DIAGRAM_ATTRIBUTE	DA;

	//ポジションの端到達判定
	if(Move == 0){//下方向
		if( Index == m_FcsDownEnd ){
			idx = m_FcsUpEnd;
			end_flg = TRUE;
		}
	}
	else if(Move == 1){//上方向
		if( Index == m_FcsUpEnd ){
			idx = m_FcsDownEnd;
			end_flg = TRUE;
		}
	}
	else if(Move == 2){//右方向
		if( Index == m_FcsRightEnd ){
			idx = m_FcsLeftEnd;
			end_flg = TRUE;
		}
	}
	else if(Move == 3){//左方向
		if( Index == m_FcsLeftEnd ){
			idx = m_FcsRightEnd;
			end_flg = TRUE;
		}
	}

	if( end_flg == TRUE ){
		return idx;
	}

	//現在ポジション検索
	if(( Move == 0 )||( Move == 1 )){
		for(i = 0 ; i < FCS_SPECIFIC1CNT ; i++){
			if(Index == FCS_Specific1[i].IDNo){
				break;
			}
		}
		idx = FCS_Specific1[i].IDNo;//現在ポジションセット
	}else if(( Move == 2 )||( Move == 3 )){
		for(i = 0 ; i < FCS_SPECIFIC1CNT ; i++){
			if(Index == FCS_Specific1_LR[i].IDNo){
				break;
			}
		}
		idx = FCS_Specific1_LR[i].IDNo;//現在ポジションセット
	}

	//次ポジション検索
	if(Move == 0){//下方向
		for(j = i + 1 ; j < FCS_SPECIFIC1CNT ; j++){
			m_Specific1.GetAttribute(FCS_Specific1[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Specific1[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 1){//上方向
		for(j = i - 1 ; j > -1 ; j--){
			m_Specific1.GetAttribute(FCS_Specific1[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Specific1[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 2){//右方向
		for(j = i + 1 ; j < FCS_SPECIFIC1CNT ; j++){
			m_Specific1.GetAttribute(FCS_Specific1_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Specific1_LR[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 3){//左方向
		for(j = i - 1 ; j > -1 ; j--){
			m_Specific1.GetAttribute(FCS_Specific1_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Specific1_LR[j].IDNo;
				break;
			}
		}
	}
	return idx;
}


void CShinSpecific1::SetFocusRange()
{

	DIAGRAM_ATTRIBUTE	DA;

	m_FcsUpEnd		= -1;
	m_FcsLeftEnd	= -1;
	m_FcsDownEnd	= -1;
	m_FcsRightEnd	= -1;

	for( int idx = 0; idx < FCS_SPECIFIC1CNT; idx++ ){
		m_Specific1.GetAttribute(FCS_Specific1[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsUpEnd = FCS_Specific1[idx].IDNo;
			break;
		}
	}
	for( int idx = 0; idx < FCS_SPECIFIC1CNT; idx++ ){
		m_Specific1.GetAttribute(FCS_Specific1_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsLeftEnd = FCS_Specific1_LR[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SPECIFIC1CNT-1; idx > -1 ; idx-- ){
		m_Specific1.GetAttribute(FCS_Specific1[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsDownEnd = FCS_Specific1[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SPECIFIC1CNT-1; idx > -1 ; idx-- ){
		m_Specific1.GetAttribute(FCS_Specific1_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsRightEnd = FCS_Specific1_LR[idx].IDNo;
			break;
		}
	}
}


//-----------------------------------------------------------------------------
// 初期ポジションセット
//-----------------------------------------------------------------------------
//2017.03.01 UPDATE START
//void CShinSpecific1::SetItemPosition()
void CShinSpecific1::SetItemPosition( BOOL IsInEnd )
//2017.03.01 UPDATE END
{
	int index;

	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAG8CTRL1 ) {
		if( (index = m_Specific1.GetPosition()) != -1) {
//2017.03.01 UPDATE START
//			m_Specific1.SetPosition(index);
//			m_Specific1.SetFocus();

			//if( IsInEnd ){
				RECT rc,rc2;
				::ZeroMemory(&rc,sizeof(RECT));
				::ZeroMemory(&rc2,sizeof(RECT));
				m_Specific1.GetRect(index,(LPUNKNOWN)&rc);
				this->ClientToScreen( &rc2 );
				if( (rc2.top<rc.top)&&(rc.bottom<rc2.bottom)){
					m_Specific1.SetPosition(index);
				}
				m_Specific1.SetFocus();
			//}else{
			//	m_Specific1.SetPosition(index);
			//	m_Specific1.SetFocus();
			//}
//2017.03.01 UPDATE END
			return;
		}
	}

	this->SetFocus();
	if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
		m_Specific1.SetPosition(ID13);
	}
	else{
	}
	SetRedrawScroll( 0 );

}



//<-----------------------------------------