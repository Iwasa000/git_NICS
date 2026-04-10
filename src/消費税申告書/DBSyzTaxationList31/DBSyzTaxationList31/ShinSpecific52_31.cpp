//yokono
// ShinSpecific52_31.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinSpecific52_31.h"
#include "ShinSpecific52_31Idx.h"
#include "TaxationList31View.h"
// 24/04/19特定収入追加 add -->
#include "SpeAddDlg.h"
// 24/04/19特定収入追加 add <--

// 定義
const int	BottomIndex = 42;	// 最下行のダイアグインデックス
const int	ID502_19[] = { ID502_19A, ID502_19B, ID502_19C, ID502_19D, 0 };

// CShinSpecific52_31 ダイアログ

IMPLEMENT_DYNAMIC(CShinSpecific52_31, CSpcBaseDlg)

CShinSpecific52_31::CShinSpecific52_31(CWnd* pParent /*=nullptr*/)
	: CSpcBaseDlg(IDD_DIALOG_K52_31, pParent)
	, m_wPos(0)
	, m_ScW(0)
	, m_cmPos(0)
	, m_SySgn ( FALSE )
{
	m_curwnd = -1;
}

CShinSpecific52_31::~CShinSpecific52_31()
{
}

void CShinSpecific52_31::DoDataExchange(CDataExchange* pDX)
{
	CSpcBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Specific52);
}


BEGIN_MESSAGE_MAP(CShinSpecific52_31, CSpcBaseDlg)
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
// 24/04/19特定収入追加 add -->
	ON_BN_CLICKED(IDC_BUTTON1,&CShinSpecific52_31::OnBnClickedButton1)
// 24/04/19特定収入追加 add <--
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CShinSpecific52_31, CSyzBaseDlg)
	ON_EVENT(CShinSpecific52_31, IDC_ICSDIAG8CTRL1, 1, CShinSpecific52_31::EditONIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinSpecific52_31, IDC_ICSDIAG8CTRL1, 2, CShinSpecific52_31::EditOFFIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinSpecific52_31, IDC_ICSDIAG8CTRL1, 3, CShinSpecific52_31::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

// CShinSpecific52_31 メッセージ ハンドラー


BOOL CShinSpecific52_31::OnInitDialog()
{
	CSpcBaseDlg::OnInitDialog();

	// TODO: ここに初期化を追加してください
	//初期設定
	SpecificInit52() ;

	m_initfg = 1 ;

	CSpcBaseDlg::OnInitDialogEX();

	//return TRUE;  // return TRUE unless you set the focus to a control
	return FALSE ;
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//初期設定
void CShinSpecific52_31::SpecificInit52()
{
	// 倍長演算指定
	m_pArith->l_defn( 0x16 );

	InitDiagAttr();

// 24/04/19特定収入追加 add -->
	// 取戻し対象特定収入ボタンはインボイス後のみ表示する
	long eymd = 0;
	if(!((*m_pSnHeadData)->Sn_SKKBN % 2))	{
		eymd = (*m_pSnHeadData)->Sn_MDAYE;
	}
	else	{
		eymd = (*m_pSnHeadData)->Sn_KDAYE;
	}
	if(eymd >= KIKAN_START_INV80)	{
		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_SHOW);
	}
	else	{
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);
	}

	// 再計算
	Specific52_Calq();
// 24/04/19特定収入追加 add <--

	// 画面表示
	SpecificDisp52();

	//カーソルの位置をセット
	SetItemPosition() ;

}

void CShinSpecific52_31::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SCROLLINFO	SI;
	GetScrollInfo( SB_VERT, &SI );
	int nowpos = m_wPos;

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
			if( m_wPos >= SI.nMax - (int)SI.nPage ){
				m_wPos = SI.nMax - SI.nPage;
			}
			break;
		case SB_THUMBTRACK:
			m_wPos = nPos;
			break;
		default:
			break;
	}
	if( (nSBCode == SB_LINEUP) || (nSBCode == SB_PAGEUP) || (nSBCode == SB_LINEDOWN) || (nSBCode == SB_PAGEDOWN) || (nSBCode == SB_THUMBTRACK) ){
		ScrollWindow( 0, nowpos - m_wPos );
		SetScrollPos( SB_VERT, m_wPos );
		m_cmPos = m_wPos;
	}

	CSpcBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CShinSpecific52_31::OnSize(UINT nType, int cx, int cy)
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
		m_Specific52.GetRect( BottomIndex, (LPUNKNOWN)&b );
		if ( GetScrollInfo( SB_VERT, &ScrollInfo ) == FALSE )
			ScrollInfo.nPos = 0;
		ScrollInfo.nMin  = 0;
		ScrollInfo.nMax  = (b.bottom + 30) - a.top + ScrollInfo.nPos - m_cmPos;
		ScrollInfo.nPage = a.bottom - a.top;
		SetScrollInfo( SB_VERT, &ScrollInfo );
		m_wPos = 0;
		m_ScW = 15;
	}

	CSpcBaseDlg::OnSize(nType, cx, cy);
}

HBRUSH CShinSpecific52_31::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSpcBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
	if( nCtlColor == CTLCOLOR_DLG ){
		hbr = Ibrsh_ShinSpecific;
	}

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

BOOL CShinSpecific52_31::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	if ( pMsg->message == WM_KEYDOWN ) {
		if ( pMsg->wParam == VK_END ) {
			if ( m_pParent ) {
				m_pParent->PostMessageA ( WM_KEYDOWN , VK_END ) ;
			}
			return TRUE ;
		}
	}

	return CSpcBaseDlg::PreTranslateMessage(pMsg);
}

void CShinSpecific52_31::SetItemPosition()
{
	int index ;

	//前回のカーソルの位置を復元
	if ( m_curwnd == IDC_ICSDIAG8CTRL1 ) {
		if ( ( index = m_Specific52.GetPosition() ) != -1 ) {
			if ( ( ( *m_pSnHeadData )->SVmzsw == 0 ) && ( ( index == ID502_19A ) || ( index == ID502_19B ) ) ) {
				index = ID502_19C;
			}
			m_Specific52.SetPosition (index) ;
			m_Specific52.SetFocus() ;
			return ;
		}
	}

	//非連動のとき、168番（税率４％適用分の１）にセット
	this->SetFocus() ;
	if ( (*m_pSnHeadData)->Sn_Sign4 & 0x80 ) {
		if ( ( *m_pSnHeadData )->Sn_UPERS & 0x10 ) {
			if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
				m_Specific52.SetPosition ( ID502_19A );
			}
			else {
				m_Specific52.SetPosition ( ID502_19C );
			}
		}
		else {
			m_Specific52.SetPosition( ID502_1A ) ;
		}
	}
	else{
	}

	//スクロール再描画
	//SetRedrawScroll( 0 ) ;
}

//-----------------------------------------
//	画面表示
//-----------------------------------------
void CShinSpecific52_31::SpecificDisp52()
{
	char			val[MONY_BUF_SIZE] = {0};
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	// １．調整前の課税仕入れ等の税額の合計額
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKgz4, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_1A, (LPUNKNOWN)&DiagData, CIcsdiagctrl);
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKgz63, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_1B, (LPUNKNOWN)&DiagData, CIcsdiagctrl);
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKgz624, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_1C, (LPUNKNOWN)&DiagData, CIcsdiagctrl);
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKgz78, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_1D, (LPUNKNOWN)&DiagData, CIcsdiagctrl);

	// ２．課税売上にのみ要する課税仕入れ等(税率４%)にのみ使途が特定されている特定収入（計算表2(2)17G）
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpSsyu4, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_2A, (LPUNKNOWN)&DiagData, CIcsdiagctrl);

	// ３．2×4/105（1円未満の端数切捨て）
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTszg4, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_3A, (LPUNKNOWN)&DiagData, CIcsdiagctrl);

	// ４．課税・非課税売上げに共通して要する課税仕入れ等(税率4％)にのみ使途が特定されている特定収入（計算表2(2)17H）
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKsyu4, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_4A, (LPUNKNOWN)&DiagData, CIcsdiagctrl);

	// ５．4×4/105（1円未満の端数切捨て）
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpZszg4, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_5A, (LPUNKNOWN)&DiagData, CIcsdiagctrl);

	// ６．課税売上高にのみ要する課税仕入れ等(税率6.3％)にのみ使途が特定されいてる特定収入（計算表2(2)17I）
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpSsyu63, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_6B, (LPUNKNOWN)&DiagData, CIcsdiagctrl);

	// ７．6×6.3/108（1円未満の端数切捨て）
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTszg63, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_7B, (LPUNKNOWN)&DiagData, CIcsdiagctrl);

	// ８．課税・非課税売上げに共通して要する課税仕入れ等(税率6.3％)にのみ使途が特定されている特定収入（計算表2(2)17J）
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKsyu63, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_8B, (LPUNKNOWN)&DiagData, CIcsdiagctrl);

	// ９．8×6.3/108（1円未満の端数切捨て）
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpZszg63, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_9B, (LPUNKNOWN)&DiagData, CIcsdiagctrl);

	// １０．課税売上げにのみ要する課税仕入れ(税率6.24%)にのみ使途が特定されいている特定収入（計算表2(2)17K）
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpSsyu624, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_10C, (LPUNKNOWN)&DiagData, CIcsdiagctrl);

	// １１．10×6.24/108（1円未満の端数切捨て）
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTszg624, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_11C, (LPUNKNOWN)&DiagData, CIcsdiagctrl);

	// １２．課税・非課税売上げに共通して要する課税仕入れ等(税率6.24%)にのみ使途が特定されている特定収入（計算表2(2)17L）
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKsyu624, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_12C, (LPUNKNOWN)&DiagData, CIcsdiagctrl);

	// １３．12×6.24/108（1円未満の端数切捨て）
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpZszg624, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_13C, (LPUNKNOWN)&DiagData, CIcsdiagctrl);

	// １４．課税売上げにのみ要する課税仕入れ等(税率7.8%)にのみ使途が特定されている特定収入（計算表2(2)17M）
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpSsyu78, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_14D, (LPUNKNOWN)&DiagData, CIcsdiagctrl);

	// １５．14×7.8/110（1円未満の端数切捨て）
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTszg78, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_15D, (LPUNKNOWN)&DiagData, CIcsdiagctrl);

	// １６．課税・非課税売上げに共通して要する課税仕入れ等(税率7.8%)にのみ使途が特定されている特定収入（計算表2(2)17N）
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKsyu78, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_16D, (LPUNKNOWN)&DiagData, CIcsdiagctrl);

	// １７．16×7.8/110（1円未満の端数切捨て）
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpZszg78, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_17D, (LPUNKNOWN)&DiagData, CIcsdiagctrl);

	// １８．課税売上割合（準ずる割合の承認を受けている場合はその割合）
	// 課税売上割合を変更している場合は、割合のみを出力
	if ( ( ( *m_pSnHeadData )->IsUseSemiUriRatio() ) || ( ( *m_pSnHeadData )->Sn_UPERS & 0x10 ) ) {
		diag_modify ( IDC_ICSDIAG8CTRL1, ID502_18_wariai, DIAG_MDFY_OPAQUE, CIcsdiagctrl );

		diag_modify ( IDC_ICSDIAG8CTRL1, ID502_18_bunshi, DIAG_MDFY_TRANSPARENT, CIcsdiagctrl );
		diag_modify ( IDC_ICSDIAG8CTRL1, ID502_18_bunbo, DIAG_MDFY_TRANSPARENT, CIcsdiagctrl );
		diag_modify ( IDC_ICSDIAG8CTRL1, 87, DIAG_MDFY_TRANSPARENT, CIcsdiagctrl );

		char buf[64] = { 0 }, Uwri[64] = { 0 };
		memset ( buf, '\0', sizeof ( buf ) );
		memset ( Uwri, '\0', sizeof ( Uwri ) );
		m_pArith->l_print ( Uwri, ( *m_pSpcListData )->stval.SpUwri, _T( "SS9.99" ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "%s％" ), Uwri );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL1, ID502_18_wariai, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	}
	else {
		diag_modify ( IDC_ICSDIAG8CTRL1, ID502_18_wariai, DIAG_MDFY_TRANSPARENT, CIcsdiagctrl );

		diag_modify ( IDC_ICSDIAG8CTRL1, ID502_18_bunshi, DIAG_MDFY_OPAQUE, CIcsdiagctrl );
		diag_modify ( IDC_ICSDIAG8CTRL1, ID502_18_bunshi, DIAG_MDFY_READONLY, CIcsdiagctrl );
		diag_modify ( IDC_ICSDIAG8CTRL1, ID502_18_bunbo, DIAG_MDFY_OPAQUE, CIcsdiagctrl );
		diag_modify ( IDC_ICSDIAG8CTRL1, ID502_18_bunbo, DIAG_MDFY_READONLY, CIcsdiagctrl );
		diag_modify ( IDC_ICSDIAG8CTRL1, 87, DIAG_MDFY_OPAQUE, CIcsdiagctrl );

		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKuws, MONY_BUF_SIZE );
		diag_setdata(IDC_ICSDIAG8CTRL1, ID502_18_bunshi, (LPUNKNOWN)&DiagData, CIcsdiagctrl);
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKuwb, MONY_BUF_SIZE );
		diag_setdata(IDC_ICSDIAG8CTRL1, ID502_18_bunbo, (LPUNKNOWN)&DiagData, CIcsdiagctrl);
	}

	// １９．5×18、9×18、13×18、17×18（いずれも1円未満の端数切捨て）
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKszg4, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_19A, (LPUNKNOWN)&DiagData, CIcsdiagctrl);
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKszg63, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_19B, (LPUNKNOWN)&DiagData, CIcsdiagctrl);
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKszg624, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_19C, (LPUNKNOWN)&DiagData, CIcsdiagctrl);
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKszg78, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_19D, (LPUNKNOWN)&DiagData, CIcsdiagctrl);
	
	if ( ( ( *m_pSnHeadData )->Sn_UPERS & 0x10 ) && !( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID502_19A, ( ( *m_pSpcListData )->stval.SpUwSgn52[0] & 0x01 ) ? 3 : 0 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID502_19B, ( ( *m_pSpcListData )->stval.SpUwSgn52[0] & 0x02 ) ? 3 : 0 );
		}
		ChangeColor ( IDC_ICSDIAG8CTRL1, ID502_19C, ( ( *m_pSpcListData )->stval.SpUwSgn52[0] & 0x04 ) ? 3 : 0 );
		ChangeColor ( IDC_ICSDIAG8CTRL1, ID502_19D, ( ( *m_pSpcListData )->stval.SpUwSgn52[0] & 0x08 ) ? 3 : 0 );
	}

	// ２０．3＋19、7＋19、11＋19、15＋19
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpGszg4, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_20A, (LPUNKNOWN)&DiagData, CIcsdiagctrl);
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpGszg63, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_20B, (LPUNKNOWN)&DiagData, CIcsdiagctrl);
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpGszg624, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_20C, (LPUNKNOWN)&DiagData, CIcsdiagctrl);
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpGszg78, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_20D, (LPUNKNOWN)&DiagData, CIcsdiagctrl);

	// ２１．1-20
	memset(val, '\0', MONY_BUF_SIZE);
	m_pArith->l_sub(val, (*m_pSpcListData)->stval.SpKgz4, (*m_pSpcListData)->stval.SpGszg4);
	memmove(DiagData.data_val, val, sizeof(DiagData.data_val));
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_21A, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	memset(val, '\0', MONY_BUF_SIZE);
	m_pArith->l_sub(val, (*m_pSpcListData)->stval.SpKgz63, (*m_pSpcListData)->stval.SpGszg63);
	memmove(DiagData.data_val, val, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_21B, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	memset(val, '\0', MONY_BUF_SIZE);
	m_pArith->l_sub(val, (*m_pSpcListData)->stval.SpKgz624, (*m_pSpcListData)->stval.SpGszg624);
	memmove(DiagData.data_val, val, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_21C, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	memset(val, '\0', MONY_BUF_SIZE);
	m_pArith->l_sub(val, (*m_pSpcListData)->stval.SpKgz78, (*m_pSpcListData)->stval.SpGszg78);
	memmove(DiagData.data_val, val, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_21D, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	//memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKgz4, MONY_BUF_SIZE );
	//diag_setdata(IDC_ICSDIAG8CTRL1, ID502_21A, (LPUNKNOWN)&DiagData, CIcsdiagctrl);
	//memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKgz63, MONY_BUF_SIZE );
	//diag_setdata(IDC_ICSDIAG8CTRL1, ID502_21B, (LPUNKNOWN)&DiagData, CIcsdiagctrl);
	//memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKgz624, MONY_BUF_SIZE );
	//diag_setdata(IDC_ICSDIAG8CTRL1, ID502_21C, (LPUNKNOWN)&DiagData, CIcsdiagctrl);
	//memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKgz78, MONY_BUF_SIZE );
	//diag_setdata(IDC_ICSDIAG8CTRL1, ID502_21D, (LPUNKNOWN)&DiagData, CIcsdiagctrl);

	// ２２．調整割合（計算表44）
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpFsyu, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_22_bunshi, (LPUNKNOWN)&DiagData, CIcsdiagctrl);

	memset(val, '\0', MONY_BUF_SIZE);
	m_pArith->l_add(val, (*m_pSpcListData)->stval.SpTuwb, (*m_pSpcListData)->stval.SpFsyu);
	memmove(DiagData.data_val, val, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_22_bunbo, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	//memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTuwb, MONY_BUF_SIZE );
	//diag_setdata(IDC_ICSDIAG8CTRL1, ID502_22_bunbo, (LPUNKNOWN)&DiagData, CIcsdiagctrl);

	// ２３．21×22（1円未満の端数切捨て）
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTwag4, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_23A, (LPUNKNOWN)&DiagData, CIcsdiagctrl);
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTwag63, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_23B, (LPUNKNOWN)&DiagData, CIcsdiagctrl);
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTwag624, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_23C, (LPUNKNOWN)&DiagData, CIcsdiagctrl);
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTwag78, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_23D, (LPUNKNOWN)&DiagData, CIcsdiagctrl);

	// ２４．特定収入に係る課税仕入れなどの税額（20＋23）
	memset(val, '\0', MONY_BUF_SIZE);
	m_pArith->l_add(val, (*m_pSpcListData)->stval.SpGszg4, (*m_pSpcListData)->stval.SpTwag4);
	memmove(DiagData.data_val, val, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_24A, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	memset(val, '\0', MONY_BUF_SIZE);
	m_pArith->l_add(val, (*m_pSpcListData)->stval.SpGszg63, (*m_pSpcListData)->stval.SpTwag63);
	memmove(DiagData.data_val, val, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_24B, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	memset(val, '\0', MONY_BUF_SIZE);
	m_pArith->l_add(val, (*m_pSpcListData)->stval.SpGszg624, (*m_pSpcListData)->stval.SpTwag624);
	memmove(DiagData.data_val, val, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_24C, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	memset(val, '\0', MONY_BUF_SIZE);
	m_pArith->l_add(val, (*m_pSpcListData)->stval.SpGszg78, (*m_pSpcListData)->stval.SpTwag78);
	memmove(DiagData.data_val, val, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_24D, (struct IUnknown *)&DiagData, CIcsdiagctrl);

	//memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpGszg4, MONY_BUF_SIZE );
	//diag_setdata(IDC_ICSDIAG8CTRL1, ID502_24A, (LPUNKNOWN)&DiagData, CIcsdiagctrl);
	//memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpGszg63, MONY_BUF_SIZE );
	//diag_setdata(IDC_ICSDIAG8CTRL1, ID502_24B, (LPUNKNOWN)&DiagData, CIcsdiagctrl);
	//memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpGszg624, MONY_BUF_SIZE );
	//diag_setdata(IDC_ICSDIAG8CTRL1, ID502_24C, (LPUNKNOWN)&DiagData, CIcsdiagctrl);
	//memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpGszg78, MONY_BUF_SIZE );
	//diag_setdata(IDC_ICSDIAG8CTRL1, ID502_24D, (LPUNKNOWN)&DiagData, CIcsdiagctrl);

	// ２５．控除対象仕入税額（1-24）
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpSiz4, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_25A, (LPUNKNOWN)&DiagData, CIcsdiagctrl);
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpSiz63, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_25B, (LPUNKNOWN)&DiagData, CIcsdiagctrl);
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpSiz624, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_25C, (LPUNKNOWN)&DiagData, CIcsdiagctrl);
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpSiz78, MONY_BUF_SIZE );
	diag_setdata(IDC_ICSDIAG8CTRL1, ID502_25D, (LPUNKNOWN)&DiagData, CIcsdiagctrl);
}



//======================================================
// ダイアグラム初期設定
//======================================================
void CShinSpecific52_31::InitDiagAttr()
{
	if ( ( ( *m_pSnHeadData )->Sn_UPERS & 0x10 ) && ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		int idx = 0;
		for ( int i = 0; i <= 4; i++ ) {
			idx = ID502_19[i];
			if ( idx != 0 ) {
				ATRIB_MOD ( IDC_ICSDIAG8CTRL1, idx, 0, ( *m_pSnHeadData )->Sn_Sign4, 0 );
			}
		}
	}
	else {
		if ( ( *m_pSnHeadData )->Sn_UPERS & 0x10 ) {
			// 非連動の場合課税売上割合が小数点以下第2位までしか入力できないため、
			// 19欄を手入力できるように対応
			if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID502_19A, 0 );
				diag_modify( IDC_ICSDIAG8CTRL1, ID502_19A, DIAG_MDFY_EDIT, CIcsdiagctrl );

				ChangeColor ( IDC_ICSDIAG8CTRL1, ID502_19B, 0 );
				diag_modify( IDC_ICSDIAG8CTRL1, ID502_19B, DIAG_MDFY_EDIT, CIcsdiagctrl );
			}

			ChangeColor ( IDC_ICSDIAG8CTRL1, ID502_19C, 0 );
			diag_modify( IDC_ICSDIAG8CTRL1, ID502_19C, DIAG_MDFY_EDIT, CIcsdiagctrl );

			ChangeColor ( IDC_ICSDIAG8CTRL1, ID502_19D, 0 );
			diag_modify( IDC_ICSDIAG8CTRL1, ID502_19D, DIAG_MDFY_EDIT, CIcsdiagctrl );

			diag_modify ( IDC_ICSDIAG8CTRL1, 87, DIAG_MDFY_TRANSPARENT, CIcsdiagctrl );
		}
		else {
			if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID502_19A, 1 );
				diag_modify( IDC_ICSDIAG8CTRL1, ID502_19A, DIAG_MDFY_READONLY, CIcsdiagctrl );

				ChangeColor ( IDC_ICSDIAG8CTRL1, ID502_19B, 1 );
				diag_modify( IDC_ICSDIAG8CTRL1, ID502_19B, DIAG_MDFY_READONLY, CIcsdiagctrl );
			}

			ChangeColor ( IDC_ICSDIAG8CTRL1, ID502_19C, 1 );
			diag_modify( IDC_ICSDIAG8CTRL1, ID502_19C, DIAG_MDFY_READONLY, CIcsdiagctrl );

			ChangeColor ( IDC_ICSDIAG8CTRL1, ID502_19D, 1 );
			diag_modify( IDC_ICSDIAG8CTRL1, ID502_19D, DIAG_MDFY_READONLY, CIcsdiagctrl );

			diag_modify ( IDC_ICSDIAG8CTRL1, 87, DIAG_MDFY_OPAQUE, CIcsdiagctrl );


			( *m_pSpcListData )->stval.SpUwSgn52[0] &= ~0x0f;
		}
	}

	// 連動時、入力ロック中はメッセージ非表示
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) || !( ( *m_pSnHeadData )->Sn_UPERS & 0x10 ) ) {
		m_Specific52.ModifyItem ( 242, DIAG_MDFY_TRANSPARENT );
	}
	else {
		m_Specific52.ModifyItem ( 242, DIAG_MDFY_OPAQUE );
	}

	m_Specific52.EnableDelete ( 0 );
}


//=========================================================
// 入力項目色変更
//---------------------------------------------------------
// 引数
//			id			:		ダイアグラムID
//			Index		:		ダイアグラム内インデックス
//			sign		:		変更サイン
//=========================================================
void CShinSpecific52_31::ChangeColor ( unsigned short id, short Index, int sign )
{
	DIAGRAM_ATTRIBUTE	DA;

	diag_getattr ( id, Index, &DA, CIcsdiagctrl );
	switch ( sign ) {
	case 1 :	// 入力不可	（白色）
		DA.attr_bcolor = BC_WHITE;
		break;
	case 2 :	// ロック	（灰色）
		DA.attr_bcolor = BC_GRAY;
		break;
	case 3 :	// 特殊色①	（青色）
		DA.attr_bcolor = BC_BLUE;
		break;
	case 4 :	// 特殊色②	（緑色）	
		DA.attr_bcolor = BC_GREEN;
		break;
	case 5 :	// 特殊色③	（薄緑色）	 
		DA.attr_bcolor = BC_GREEN_L;
		break;
	default:	// 実額入力	（クリーム色） 
		DA.attr_bcolor = BC_CREAM;
		break;
	}
	diag_setattr ( id, Index, &DA, FALSE, CIcsdiagctrl );

}

void CShinSpecific52_31::EditONIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_curwnd = IDC_ICSDIAG8CTRL1;

}

void CShinSpecific52_31::EditOFFIcsdiag8ctrl1(short index)
{
	if ( !( ( *m_pSnHeadData )->Sn_UPERS & 0x10 ) ) {
		return;
	}

	if ( !m_SySgn ) {
		return;
	}

	DIAGRAM_DATA DiagData ;
	DiagData.data_imgdata = NULL ;
	diag_getdata ( IDC_ICSDIAG8CTRL1, index , ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	switch ( index) {
	case ID502_19A:
		if ( m_pArith->l_cmp ( ( *m_pSpcListData )->stval.SpKszg4, DiagData.data_val ) ) {
			memmove ( ( *m_pSpcListData )->stval.SpKszg4, DiagData.data_val, MONY_BUF_SIZE );
			( *m_pSpcListData )->stval.SpUwSgn52[0] |= 0x01;
		}
		break;
	case ID502_19B:
		if ( m_pArith->l_cmp ( ( *m_pSpcListData )->stval.SpKszg63, DiagData.data_val ) ) {
			memmove ( ( *m_pSpcListData )->stval.SpKszg63, DiagData.data_val, MONY_BUF_SIZE );
			( *m_pSpcListData )->stval.SpUwSgn52[0] |= 0x02;
		}
		break;
	case ID502_19C:
		if ( m_pArith->l_cmp ( ( *m_pSpcListData )->stval.SpKszg624, DiagData.data_val ) ) {
			memmove ( ( *m_pSpcListData )->stval.SpKszg624, DiagData.data_val, MONY_BUF_SIZE );
			( *m_pSpcListData )->stval.SpUwSgn52[0] |= 0x04;
		}
		break;
	case ID502_19D:
		if ( m_pArith->l_cmp ( ( *m_pSpcListData )->stval.SpKszg78, DiagData.data_val ) ) {
			memmove ( ( *m_pSpcListData )->stval.SpKszg78, DiagData.data_val, MONY_BUF_SIZE );
			( *m_pSpcListData )->stval.SpUwSgn52[0] |= 0x08;
		}
		break;
	default:
		break;
	}

	( *m_pSpcListData )->SPassOffCalqMain ( ( *m_pSnHeadData ) );
	SpecificDisp52();
	( ( CTaxationList31View* )m_pParent )->LinkHonpyoData ( TRUE );
}

void CShinSpecific52_31::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	if ( nChar == 0 ) {
		return;
	}

	if ( !( ( *m_pSnHeadData )->Sn_UPERS & 0x10 ) ) {
		return;
	}

	// Shift+TABをTABに、TABをENTERに返還
	short wTerm = 0;
	char VK_FLG = 0x00;
	VK_FLG = ( char )::GetKeyState ( VK_SHIFT );
	if ( nChar == VK_TAB ) {
		if ( VK_FLG & 0x80 ) {
			wTerm = VK_TAB;
		}
		else {
			wTerm = VK_RETURN;
		}
	}
	else {
		wTerm = nChar;
	}

	int pos = 0;

	if ( ( wTerm == VK_RETURN ) || ( wTerm == VK_F3 ) || ( wTerm == VK_RIGHT ) ) {
		m_SySgn = TRUE;
		m_Specific52.SetNextPosition();
		m_SySgn = FALSE;
	}
	else if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 ) ) {
		m_SySgn = TRUE;
		m_Specific52.SetPrevPosition();
		m_SySgn = FALSE;
	}

	if ( nChar == VK_DELETE ) {
		switch ( index ) {
		case ID502_19A:
			( *m_pSpcListData )->stval.SpUwSgn52[0] &= ~0x01;
			break;
		case ID502_19B:
			( *m_pSpcListData )->stval.SpUwSgn52[0] &= ~0x02;
			break;
		case ID502_19C:
			( *m_pSpcListData )->stval.SpUwSgn52[0] &= ~0x04;
			break;
		case ID502_19D:
			( *m_pSpcListData )->stval.SpUwSgn52[0] &= ~0x08;
			break;
		default:
			break;
		}

		m_SySgn = TRUE;
		m_Specific52.SetPosition ( index );
		m_SySgn = FALSE;
	}


	RECT rectA, rectB;
	GetWindowRect ( &rectA );
	diag_getrect ( IDC_ICSDIAG8CTRL1, index, &rectB, CIcsdiagctrl );
	if ( ( rectA.top > rectB.top ) || ( rectA.bottom < rectB.bottom ) ) {
		int	cpos = GetScrollPos ( SB_VERT );
		SetRedrawScroll ( ( cpos + rectB.top - rectA.top ) - ( ( rectA.bottom - rectA.top ) / 2 ) );
	}
}

// 24/04/19特定収入追加 add -->
// 取戻し対象特定収入ボタン押下
void CShinSpecific52_31::OnBnClickedButton1()
{
	CSpeAddDlg SpeAdd(this);
	SpeAdd.InitInfo(*m_pSnHeadData,*m_pSpcListData,m_pArith,m_pZmSub);
	if(SpeAdd.DoModal() == 0)	{
		// 再計算
		Specific52_Calq();
		// 再表示
		SpecificDisp52();
	}
// 240501_168729 add -->
	GetDlgItem(IDC_BUTTON1)->SetFocus();
// 240501_168729 add <--
}

// 再計算
void CShinSpecific52_31::Specific52_Calq(void)
{
	BOOL			IsChgDoSpc = FALSE;
	unsigned char	svSn_Sign2 = (*m_pSnHeadData)->Sn_Sign2;

	(*m_pSpcListData)->SPassOffCalqMain(*m_pSnHeadData);

// 24/04/26特定収入追加 add -->
	// 5-2のデータがあればガイドを表示する
	if((*m_pSpcListData)->m_bIs52Data == TRUE)	{
		// 表示
		diag_modify(IDC_ICSDIAG8CTRL1,ID502_GUID,DIAG_MDFY_OPAQUE,CIcsdiagctrl);
	}
	else	{
		// 非表示
		diag_modify(IDC_ICSDIAG8CTRL1,ID502_GUID,DIAG_MDFY_TRANSPARENT,CIcsdiagctrl);
	}
// 24/04/26特定収入追加 add <--

	IsChgDoSpc = FALSE;
	if((svSn_Sign2 & 0x02) != ((*m_pSnHeadData)->Sn_Sign2 & 0x02)) {
		IsChgDoSpc = TRUE;
	}
	((CTaxationList31View*)m_pParent)->LinkHonpyoData(IsChgDoSpc);
}
// 24/04/19特定収入追加 add <--
