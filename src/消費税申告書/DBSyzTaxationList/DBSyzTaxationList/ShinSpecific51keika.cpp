// ShinSpecific51.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinSpecific51keika.h"
#include "ShinSpecific51keikaIdx.h"
#include "TaxationListView.h"

//== 定数 ==
const	int	BottomIndex = 15;
const	int	StartIndex = 48;

// CShinSpecific51keika ダイアログ

IMPLEMENT_DYNAMIC(CShinSpecific51keika, CSpcBaseDlg)

CShinSpecific51keika::CShinSpecific51keika(CWnd* pParent /*=NULL*/)
	: CSpcBaseDlg(CShinSpecific51keika::IDD, pParent)
	, m_zeiType(ID_ICSSH_SPC_8PER)
	, m_wPos(0)
	, m_cmPos(0)
	, m_ScW(0)
{
}

CShinSpecific51keika::CShinSpecific51keika(int zeitype, CWnd* pParent /*=NULL*/)
	: CSpcBaseDlg(CShinSpecific51keika::IDD, pParent)
	, m_zeiType(zeitype)
	, m_wPos(0)
	, m_cmPos(0)
	, m_ScW(0)
{
}

CShinSpecific51keika::~CShinSpecific51keika()
{
}

void CShinSpecific51keika::DoDataExchange(CDataExchange* pDX)
{
	CSpcBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Specific51);
}


BEGIN_MESSAGE_MAP(CShinSpecific51keika, CSpcBaseDlg)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CShinSpecific51keika メッセージ ハンドラ

BOOL CShinSpecific51keika::OnInitDialog()
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

//	return TRUE;  // return TRUE unless you set the focus to a control
	return FALSE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

BOOL CShinSpecific51keika::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	if( pMsg->message == WM_KEYDOWN ){
		if( pMsg->wParam == VK_END ){
			if( m_pParent ){
				m_pParent->PostMessageA( WM_KEYDOWN, VK_END );
			}
			return TRUE;
		}
	}

	return CSpcBaseDlg::PreTranslateMessage(pMsg);

}

void CShinSpecific51keika::OnSize(UINT nType, int cx, int cy)
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

		GetWindowRect( &a );

		m_Specific51.GetRect( BottomIndex, (LPUNKNOWN)&b );
	}

	if( m_initfg ){
		RECT	a, b;
		SCROLLINFO ScrollInfo;
		GetWindowRect( &a );
		m_Specific51.GetRect( BottomIndex, (LPUNKNOWN)&b );
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

HBRUSH CShinSpecific51keika::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSpcBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
	if( nCtlColor == CTLCOLOR_DLG ){
		hbr = Ibrsh_ShinSpecific;
	}

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

void CShinSpecific51keika::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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
void CShinSpecific51keika::sykscroll( int mpos )
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
// 初期設定
//-----------------------------------------------------------------------------
void CShinSpecific51keika::SpecificInit51()
{
	// 倍長演算指定
	m_pArith->l_defn( 0x16 );

	//DIAGRAM_DATA		DiagData;
	//DiagData.data_imgdata = NULL;
	//if( m_zeiType == ID_ICSSH_SPC_8PER ){
	//	DiagData.data_disp = _T("６．３\n\n１０８");
	//}
	//else if( m_zeiType == ID_ICSSH_SPC_5PER ){
	//	DiagData.data_disp = _T("４\n\n１０５");
	//}
	//diag_setdata( IDC_ICSDIAG8CTRL1, 5, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	//if( m_zeiType == ID_ICSSH_SPC_8PER ){
	//	DiagData.data_disp = _T("　課税仕入れ等(税率6.3％)にのみ使途が特定されている特定収入\n　(「課税仕入れ等に係る特定収入」)　(計算表２(1)⑰Ｃ)");
	//}
	//else if( m_zeiType == ID_ICSSH_SPC_5PER ){
	//	DiagData.data_disp = _T("　課税仕入れ等(税率4％)にのみ使途が特定されている特定収入\n　(「課税仕入れ等に係る特定収入」)　(計算表２(1)⑰Ｂ)");
	//}
	//diag_setdata( IDC_ICSDIAG8CTRL1, 3, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	SpecificDisp51();

	IndexControl();

	SetItemPosition();
}

//-----------------------------------------------------------------------------
// 表示
//-----------------------------------------------------------------------------
void CShinSpecific51keika::SpecificDisp51( void )
{
	char			val[MONY_BUF_SIZE]={0};
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	// 1.調整前の課税仕入れ等の税額の合計					[SpKgzg]
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKgz4, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID510, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKgz63, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID519, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 2.課税仕入れ等のみに使途が特定されている特定収入		[SpTsyu + SpKsyu + SpHsyu]
	memset( val, '\0', sizeof(val) );
	memmove( val, (*m_pSpcListData)->stval.SpTsyu4, sizeof(val) );
	memmove( DiagData.data_val, val, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID511, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memset( val, '\0', sizeof(val) );
	memmove( val, (*m_pSpcListData)->stval.SpTsyu63, sizeof(val) );
	memmove( DiagData.data_val, val, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID520, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 3.②×６．３／１０８									[SpTszg + SpKszg]
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpGszg4, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID512, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpGszg63, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID521, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 4.①－③												[SpKgzg-(SpTszg + SpKszg)]
	m_pArith->l_sub( val, (*m_pSpcListData)->stval.SpKgz4, (*m_pSpcListData)->stval.SpGszg4 );
	memmove( DiagData.data_val, val, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID513, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	m_pArith->l_sub( val, (*m_pSpcListData)->stval.SpKgz63, (*m_pSpcListData)->stval.SpGszg63 );
	memmove( DiagData.data_val, val, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID522, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 5.調整割合 分子										[SpFsyu]
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpFsyu, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID514, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 5.調整割合 分母										[SpTuwb + SpFsyu]
	memset( val, '\0', sizeof(val) );
	m_pArith->l_add( val, (*m_pSpcListData)->stval.SpTuwb, (*m_pSpcListData)->stval.SpFsyu );
	memmove( DiagData.data_val, val, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID515, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 6.④×⑤												[SpTwag]
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTwag4, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID516, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( DiagData.data_val, (*m_pSpcListData	)->stval.SpTwag63, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID523, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 7.③＋⑥												[SpGgtg]
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpGgt4, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID517, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpGgt63, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID524, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 8.仕入れ控除税額										[SpSizg]
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpSiz4, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID518, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpSiz63, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID525, (struct IUnknown *)&DiagData, CIcsdiagctrl );

}

//-----------------------------------------------------------------------------
// 再計算
//-----------------------------------------------------------------------------
void CShinSpecific51keika::Specific51_Calq( void )
{
	// 特定収入に係る課税仕入れ等の税額計算
	unsigned char	svSn_Sign2 = (*m_pSnHeadData)->Sn_Sign2;
	(*m_pSpcListData)->SPassOffCalqMain( *m_pSnHeadData );
	BOOL	IsChgDoSpc = FALSE;
	if( (svSn_Sign2&0x02) != ((*m_pSnHeadData)->Sn_Sign2&0x02) ){
		IsChgDoSpc = TRUE;
	}
	((CTaxationListView*)m_pParent)->LinkHonpyoData(IsChgDoSpc);

	SpecificDisp51();
}

BEGIN_EVENTSINK_MAP(CShinSpecific51keika, CSpcBaseDlg)
	ON_EVENT(CShinSpecific51keika, IDC_ICSDIAG8CTRL1, 1, CShinSpecific51keika::EditONIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinSpecific51keika, IDC_ICSDIAG8CTRL1, 2, CShinSpecific51keika::EditOFFIcsdiag8ctrl1, VTS_I2)
END_EVENTSINK_MAP()

void CShinSpecific51keika::EditONIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

void CShinSpecific51keika::EditOFFIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	// 計算表5(1)で編集を無くしたのでカット
	return;

	ASSERT( m_pSpcListData );
	if( m_pSpcListData == NULL ){
		return;
	}
	ASSERT( *m_pSpcListData );
	if( *m_pSpcListData == NULL ){
		return;
	}
	if( IsWindowVisible() == FALSE ){
		return;
	}

	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	switch( index ){
		case ID510	:	
			diag_getdata( IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( (*m_pSpcListData)->stval.SpKgz4, DiagData.data_val, MONY_BUF_SIZE );
			// 合計
			m_pArith->l_add( (*m_pSpcListData)->stval.SpKgzg, (*m_pSpcListData)->stval.SpKgz4, (*m_pSpcListData)->stval.SpKgz63 );
			break;
		case ID519	:	
			diag_getdata( IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( (*m_pSpcListData)->stval.SpKgz63, DiagData.data_val, MONY_BUF_SIZE );
			// 合計
			m_pArith->l_add( (*m_pSpcListData)->stval.SpKgzg, (*m_pSpcListData)->stval.SpKgz4, (*m_pSpcListData)->stval.SpKgz63 );
			break;
		default     :
			break;
	}
	// 再計算
	Specific51_Calq();
}

void CShinSpecific51keika::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
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
			memmove( (*m_pSpcListData)->stval.SpKgz4, DiagData.data_val, MONY_BUF_SIZE );
			// 合計
			m_pArith->l_add( (*m_pSpcListData)->stval.SpKgzg, (*m_pSpcListData)->stval.SpKgz4, (*m_pSpcListData)->stval.SpKgz63 );
			// 再計算
			Specific51_Calq();
			break;
		case ID519 :
			diag_getdata( IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( (*m_pSpcListData)->stval.SpKgz63, DiagData.data_val, MONY_BUF_SIZE );
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
void CShinSpecific51keika::IndexControl( void )
{
	int	Flg = 0;
}

void CShinSpecific51keika::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
//	CSpcBaseDlg::OnOK();
}

void CShinSpecific51keika::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
//	CSpcBaseDlg::OnCancel();
}

//-----------------------------------------------------------------------------
// セットポジション
//-----------------------------------------------------------------------------
void CShinSpecific51keika::SetItemPosition( )
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
