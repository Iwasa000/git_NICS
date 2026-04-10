// ShinSpecific6.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinSpecific6.h"
#include "ShinSpecific6Idx.h"
#include "TaxationListView.h"

//== 定数 ==
const	int	BottomIndex = 19;
const	int	StartIndex = 54;

// CShinSpecific6 ダイアログ

IMPLEMENT_DYNAMIC(CShinSpecific6, CSpcBaseDlg)

CShinSpecific6::CShinSpecific6(CWnd* pParent /*=NULL*/)
	: CSpcBaseDlg(CShinSpecific6::IDD, pParent)
	,m_wPos(0)
	,m_cmPos(0)
	,m_ScW(0)
{
}

CShinSpecific6::~CShinSpecific6()
{
}

void CShinSpecific6::DoDataExchange(CDataExchange* pDX)
{
	CSpcBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Specific6);
}


BEGIN_MESSAGE_MAP(CShinSpecific6, CSpcBaseDlg)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CShinSpecific6 メッセージ ハンドラ

BOOL CShinSpecific6::OnInitDialog()
{
	CSpcBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	// 初期設定
	SpecificInit6();

	m_initfg = 1;

	CSpcBaseDlg::OnInitDialogEX();

//	return TRUE;  // return TRUE unless you set the focus to a control
	return FALSE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

BOOL CShinSpecific6::PreTranslateMessage(MSG* pMsg)
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

void CShinSpecific6::OnSize(UINT nType, int cx, int cy)
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
		m_Specific6.GetRect( BottomIndex, (LPUNKNOWN)&b );
		if ( GetScrollInfo( SB_VERT, &ScrollInfo ) == FALSE ){
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

HBRUSH CShinSpecific6::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSpcBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
	if( nCtlColor == CTLCOLOR_DLG ){
		hbr = Ibrsh_ShinSpecific;
	}

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

void CShinSpecific6::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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
void CShinSpecific6::sykscroll( int mpos )
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
void CShinSpecific6::SetItemPosition( int IndexNo )
{
	int	Index;
	if( IndexNo ){
		Index = IndexNo;
	}
	else{
		Index = StartIndex;
	}

	m_Specific6.SetFocus();
	// 確定時は全入力不可
	if( ((*m_pSnHeadData)->Sn_Sign4&0x01) ){
		return;
	}

	m_Specific6.SetPosition( Index );

	CRect	rc;
	this->GetClientRect( &rc );
	this->InvalidateRect( &rc );
	sykscroll( 0 );

}

/*
//-----------------------------------------------------------------------------
// 入力項目　色変え
//-----------------------------------------------------------------------------
void CShinSpecific6::Color_Change( unsigned short id, short index, int sign )
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
void CShinSpecific6::SpecificInit6( void )
{
	// 倍長演算指定
	m_pArith->l_defn( 0x16 );

	SpecificDisp6();

	IndexControl();

	SetItemPosition(0);
}

//-----------------------------------------------------------------------------
// 表示
//-----------------------------------------------------------------------------
void CShinSpecific6::SpecificDisp6( void )
{
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	// 1. 調整前の新税率適用分の課税仕入れ等の税額			[SpKgz4]
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKgz4, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID60, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 2. 調整前の旧税率適用分の課税仕入れ等の税額			[SpKgz3]
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKgz3, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID61, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 3. ①＋②											[SpKgzg]
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKgzg, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID62, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 4. 特定収入に係る税額								[SpGszg]
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpGszg, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID63, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 5. 調整額											[SpTwag]
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTwag, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID64, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 6. 課税仕入れ等の税額								[SpKsig]
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKsig, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID65, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 7. 課税仕入れにのみ使途が特定されている特定収入		[SpTsyu]
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTsyu, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID66, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 8. 課税仕入れ等に係る対価の返還等の金額				[SpKsbg]
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKsbg, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID67, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 9. 新税率適用分の課税仕入れ等に係る金額				[SpKsi4]
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKsi4, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID68, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 10.新税率適用分の課税仕入れ等に係る返還等の金額		[SpKsb4]
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKsb4, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID69, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 11.配賦割合	分子									[SpHfgs]
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpHfgs, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID6A, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 11.配賦割合　分母									[SpHfgb]
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpHfgb, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID6B, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 12.新税率適用分における配賦後の調整額				[SpGgt4]
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpGgt4, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID6C, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 13.新税率適用分の控除対象仕入税額					[SpSiz4]	
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpSiz4, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID6D, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 14.旧税率適用分における配賦後の調整額				[SpGgt3]
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpGgt3, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID6E, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 15.旧税率適用分の控除対象仕入税額					[SpSiz3]
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpSiz3, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID6F, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 16.調整後の控除対象仕入税額の合計					[SpSizg]
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpSizg, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID6G, (struct IUnknown *)&DiagData, CIcsdiagctrl );
}

// 再計算
void CShinSpecific6::Specific6_Calq( void )
{
	// 調整前の課税仕入れ等の税額の合計
	m_pArith->l_add( (*m_pSpcListData)->stval.SpKgzg, (*m_pSpcListData)->stval.SpKgz4, (*m_pSpcListData)->stval.SpKgz3 );
	// 配賦割合	分子
	memset( (*m_pSpcListData)->stval.SpHfgs, '\0', sizeof((*m_pSpcListData)->stval.SpHfgs) );
	// 配賦割合	分母
	memset( (*m_pSpcListData)->stval.SpHfgb, '\0', sizeof((*m_pSpcListData)->stval.SpHfgb) );
	// 特定収入に係る課税仕入れ等の税額計算
//-- '15.03.07 --
//	(*m_pSpcListData)->SPassOnCalqEx( (*m_pSnHeadData) );		// 経過措置有り
//---------------
	unsigned char	svSn_Sign2 = (*m_pSnHeadData)->Sn_Sign2;
	(*m_pSpcListData)->SPassOffCalqMain( *m_pSnHeadData );		// 経過措置有り
	BOOL	IsChgDoSpc = FALSE;
	if( (svSn_Sign2&0x02) != ((*m_pSnHeadData)->Sn_Sign2&0x02) ){
		IsChgDoSpc = TRUE;
	}
	((CTaxationListView*)m_pParent)->LinkHonpyoData(IsChgDoSpc);
//---------------

	SpecificDisp6();
}

BEGIN_EVENTSINK_MAP(CShinSpecific6, CSpcBaseDlg)
	ON_EVENT(CShinSpecific6, IDC_ICSDIAG8CTRL1, 2, CShinSpecific6::EditOFFIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinSpecific6, IDC_ICSDIAG8CTRL1, 3, CShinSpecific6::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinSpecific6::EditOFFIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
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
		case ID60	:	
			diag_getdata( IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( (*m_pSpcListData)->stval.SpKgz4, DiagData.data_val, MONY_BUF_SIZE );
			break;
		case ID61	:	
			diag_getdata( IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( (*m_pSpcListData)->stval.SpKgz3, DiagData.data_val, MONY_BUF_SIZE );
			break;
		case ID65	:	
			diag_getdata( IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( (*m_pSpcListData)->stval.SpKsig, DiagData.data_val, MONY_BUF_SIZE );
			break;
		case ID67	:	
			diag_getdata( IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( (*m_pSpcListData)->stval.SpKsbg, DiagData.data_val, MONY_BUF_SIZE );
			break;
		case ID68	:	
			diag_getdata( IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( (*m_pSpcListData)->stval.SpKsi4, DiagData.data_val, MONY_BUF_SIZE );
			break;
		case ID69	:	
			diag_getdata( IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( (*m_pSpcListData)->stval.SpKsb4, DiagData.data_val, MONY_BUF_SIZE );
			break;
		default     :	
			break;
	}
	// 再計算
	Specific6_Calq();
}

void CShinSpecific6::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
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
		case ID60 :
			// 次項目へ
			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) ){
				diag_setposition( IDC_ICSDIAG8CTRL1, ID61, CIcsdiagctrl );
			}
			break;
		case ID61 :
			// 次項目へ
			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) ){
				diag_setposition( IDC_ICSDIAG8CTRL1, ID60, CIcsdiagctrl );
			}
			// 前項目へ
			if( (wTerm==VK_TAB) || (wTerm==VK_F2) ){
				diag_setposition( IDC_ICSDIAG8CTRL1, ID65, CIcsdiagctrl );
			}
			break;
		case ID65 :
			// 次項目へ
			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) ){
				diag_setposition( IDC_ICSDIAG8CTRL1, ID67, CIcsdiagctrl );
			}
			// 前項目へ
			if( (wTerm==VK_TAB) || (wTerm==VK_F2) ){
				diag_setposition( IDC_ICSDIAG8CTRL1, ID61, CIcsdiagctrl );
			}
			break;
		case ID67 :
			// 次項目へ
			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) ){
				diag_setposition( IDC_ICSDIAG8CTRL1, ID68, CIcsdiagctrl );
			}
			// 前項目へ
			if( (wTerm==VK_TAB) || (wTerm==VK_F2) ){
				diag_setposition( IDC_ICSDIAG8CTRL1, ID65, CIcsdiagctrl );
			}
			break;
		case ID68 :
			// 次項目へ
			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) ){
				diag_setposition( IDC_ICSDIAG8CTRL1, ID69, CIcsdiagctrl );
			}
			// 前項目へ
			if( (wTerm==VK_TAB) || (wTerm==VK_F2) ){
				diag_setposition( IDC_ICSDIAG8CTRL1, ID67, CIcsdiagctrl );
			}
			break;
		case ID69 :
			// 次項目へ
			if( (wTerm==VK_RETURN) || (wTerm==VK_F3) ){
				diag_getdata( IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				memmove( (*m_pSpcListData)->stval.SpKsb4, DiagData.data_val, MONY_BUF_SIZE );
				// 再計算
				Specific6_Calq();
			}
			// 前項目へ
			if( (wTerm==VK_TAB) || (wTerm==VK_F2) ){
				diag_setposition( IDC_ICSDIAG8CTRL1, ID68, CIcsdiagctrl );
			}
			break;
		default :
			break;
	}
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void CShinSpecific6::IndexControl( void )
{
	int	Cnt = 0;
	while( 1 ) {
		if( Specific6[Cnt].Index == 0 ){
			break;
		}
		int	Flg;
		// 確定時は全入力不可
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			Flg = Specific6[Cnt].AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
				Flg = Specific6[Cnt].UnConnect;
			}
			else{
				Flg = Specific6[Cnt].Connect;
			}
		}
		ATRIB_MOD( IDC_ICSDIAG8CTRL1, Specific6[Cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, Specific6[Cnt].Connect );
		Cnt++;
	}
}

void CShinSpecific6::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
//	CSpcBaseDlg::OnOK();
}

void CShinSpecific6::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
//	CSpcBaseDlg::OnCancel();
}
