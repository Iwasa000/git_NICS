//yokono
// ShinSpecific53_31.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinSpecific53_31.h"
#include "ShinSpecific53_31Idx.h"
#include "TaxationList31View.h"
// 24/04/19特定収入追加 add -->
#include "SpeAddDlg.h"
// 24/04/19特定収入追加 add <--

//== 定数 ==
const	int	BottomIndex = 25;
const	int	StartIndex = 106;

const int ID503_11[] = { ID503_11A, ID503_11B, ID503_11C, ID503_11D, 0 };

// CShinSpecific53_31 ダイアログ

IMPLEMENT_DYNAMIC(CShinSpecific53_31, CSpcBaseDlg)

CShinSpecific53_31::CShinSpecific53_31(CWnd* pParent /*=nullptr*/)
	: CSpcBaseDlg(IDD_DIALOG_K53_31, pParent)
	,m_wPos ( 0 )
	,m_cmPos ( 0 )
	,m_ScW ( 0 )
	,m_SySgn ( FALSE )
{

}

CShinSpecific53_31::~CShinSpecific53_31()
{
}

void CShinSpecific53_31::DoDataExchange(CDataExchange* pDX)
{
	CSpcBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Specific53);
}


BEGIN_MESSAGE_MAP(CShinSpecific53_31, CSpcBaseDlg)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_VSCROLL()
// 24/04/19特定収入追加 add -->
	ON_BN_CLICKED(IDC_BUTTON1,&CShinSpecific53_31::OnBnClickedButton1)
// 24/04/19特定収入追加 add <--
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CShinSpecific53_31, CSyzBaseDlg)
	ON_EVENT(CShinSpecific53_31, IDC_ICSDIAG8CTRL1, 1, CShinSpecific53_31::EditONIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinSpecific53_31, IDC_ICSDIAG8CTRL1, 2, CShinSpecific53_31::EditOFFIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinSpecific53_31, IDC_ICSDIAG8CTRL1, 3, CShinSpecific53_31::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()


// CShinSpecific53_31 メッセージ ハンドラー


BOOL CShinSpecific53_31::OnInitDialog()
{
	CSpcBaseDlg::OnInitDialog();

	// TODO: ここに初期化を追加してください
	//初期設定
	SpecificInit53() ;

	m_initfg = 1 ;

	CSpcBaseDlg::OnInitDialogEX();

	//return TRUE;  // return TRUE unless you set the focus to a control
	return FALSE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//初期設定
void CShinSpecific53_31::SpecificInit53()
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
	Specific53_Calq();
// 24/04/19特定収入追加 add <--
	// 金額表示
	SpecificDisp53();

	//カーソルの位置をセット
	SetItemPosition() ;

}

BOOL CShinSpecific53_31::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
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

//カーソルの位置をセット
void CShinSpecific53_31::SetItemPosition()
{
	int index ;

	//前回のカーソル位置の復元
	if ( m_curwnd == IDC_ICSDIAG8CTRL1 ){
		if ( (index = m_Specific53.GetPosition()) != -1 ) {
			if ( ( ( *m_pSnHeadData )->SVmzsw == 0 ) && ( ( index == ID503_11A ) || ( index == ID503_11B ) ) ) {
				index = ID503_11C;
			}
			m_Specific53.SetPosition(index) ;
			m_Specific53.SetFocus() ;
			return ;
		}
	}

	//非連動のとき、106番（税率４％適用分の１）にセット
	this->SetFocus() ;
	if ( (*m_pSnHeadData)->Sn_Sign4 & 0x80 ){
		if ( ( *m_pSnHeadData )->Sn_UPERS & 0x10 ) {
			if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
				m_Specific53.SetPosition ( ID503_11A );
			}
			else {
				m_Specific53.SetPosition ( ID503_11C );
			}
		}
		else {
			m_Specific53.SetPosition(ID503_1A) ;
		}
	}
	else{
	}
	//スクロール再描画
	//SetRedrawScroll( 0 );
}

//==============================
// 金額表示
//==============================
void CShinSpecific53_31::SpecificDisp53()
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	char val[MONY_BUF_SIZE] = { 0 };

	// 1. 調整前の課税仕入れ等の税額の合計額
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpKgz4, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_1A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 4%
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpKgz63, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_1B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 6.3%
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpKgz624, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_1C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 6.24%
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpKgz78, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_1D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 7.8%

	// 2. 課税仕入れ等 ( 税率4% )にのみ使途が特定されている特定収入 (「課税仕入れ等に係る特定収入」 ) ( 計算表2 ( 1 ) 17B )
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpTsyu4, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_2A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	// 3. 2×4/105
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpZszg4, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_3A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	// 4. 課税仕入れ等 ( 税率6.3% )にのみ使途が特定されている特定収入 (「課税仕入れ等に係る特定収入」 ) ( 計算表2 ( 1 ) 17C )
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpTsyu63, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_4B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	// 5. 4×6.3/108
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpZszg63, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_5B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	// 6. 課税仕入れ等 ( 税率6.24% )にのみ使途が特定されている特定収入 (「課税仕入れ等に係る特定収入」 ) ( 計算表2 ( 1 ) 17D )
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpTsyu624, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_6C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	// 7. 6×6.24/108
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpZszg624, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_7C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	// 8. 課税仕入れ等 ( 税率7.8% )にのみ使途が特定されている特定収入 (「課税仕入れ等に係る特定収入」 ) ( 計算表2 ( 1 ) 17E )
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpTsyu78, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_8D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	// 9. 8×7.8/110
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpZszg78, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_9D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	if ( ( *m_pSnHeadData )->Sn_UPERS & 0x10 ) {
		diag_modify ( IDC_ICSDIAG8CTRL1, ID503_10_bunshi, DIAG_MDFY_TRANSPARENT, CIcsdiagctrl );
		diag_modify ( IDC_ICSDIAG8CTRL1, ID503_10_bunbo, DIAG_MDFY_TRANSPARENT, CIcsdiagctrl );
		diag_modify ( IDC_ICSDIAG8CTRL1, 53, DIAG_MDFY_TRANSPARENT, CIcsdiagctrl );

		char buf[64] = { 0 }, Uwri[64] = { 0 };
		memset ( buf, '\0', sizeof ( buf ) );
		memset ( Uwri, '\0', sizeof ( Uwri ) );
		m_pArith->l_print ( Uwri, ( *m_pSpcListData )->stval.SpUwri, _T( "SS9.99" ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "%s％" ), Uwri );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_10_wariai, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	}
	else {
		diag_modify ( IDC_ICSDIAG8CTRL1, ID503_10_wariai, DIAG_MDFY_TRANSPARENT, CIcsdiagctrl );
		
		diag_modify ( IDC_ICSDIAG8CTRL1, ID503_10_bunshi, DIAG_MDFY_OPAQUE, CIcsdiagctrl );
		diag_modify ( IDC_ICSDIAG8CTRL1, ID503_10_bunshi, DIAG_MDFY_READONLY, CIcsdiagctrl );
		diag_modify ( IDC_ICSDIAG8CTRL1, ID503_10_bunbo, DIAG_MDFY_OPAQUE, CIcsdiagctrl );
		diag_modify ( IDC_ICSDIAG8CTRL1, ID503_10_bunbo, DIAG_MDFY_READONLY, CIcsdiagctrl );
		diag_modify ( IDC_ICSDIAG8CTRL1, 53, DIAG_MDFY_OPAQUE, CIcsdiagctrl );

		// 10. 課税売上割合 分子
		memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpKuws, MONY_BUF_SIZE );
		diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_10_bunshi, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		// 10. 課税売上割合 分母
		memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpKuwb, MONY_BUF_SIZE );
		diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_10_bunbo, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	}

	// 11. 3×10、5×10、7×10、9×10 ( いずれも1円未満の端数切捨て )
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpTszg4, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_11A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 4%
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpTszg63, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_11B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 6.3%
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpTszg624, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_11C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 6.24%
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpTszg78, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_11D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 7.8%
	if ( ( ( *m_pSnHeadData )->Sn_UPERS & 0x10 ) && !( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID503_11A, ( ( *m_pSpcListData )->stval.SpUwSgn53[0] & 0x01 ) ? 3 : 0 );
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID503_11B, ( ( *m_pSpcListData )->stval.SpUwSgn53[0] & 0x02 ) ? 3 : 0 );
		}
		ChangeColor ( IDC_ICSDIAG8CTRL1, ID503_11C, ( ( *m_pSpcListData )->stval.SpUwSgn53[0] & 0x04 ) ? 3 : 0 );
		ChangeColor ( IDC_ICSDIAG8CTRL1, ID503_11D, ( ( *m_pSpcListData )->stval.SpUwSgn53[0] & 0x08 ) ? 3 : 0 );
	}

	// 12. 1-11
	memset ( val, '\0', MONY_BUF_SIZE );
	m_pArith->l_sub ( val, ( *m_pSpcListData )->stval.SpKgz4, ( *m_pSpcListData )->stval.SpTszg4 );
	memmove ( DiagData.data_val, val, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_12A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 4%
	memset ( val, '\0', MONY_BUF_SIZE );
	m_pArith->l_sub ( val, ( *m_pSpcListData )->stval.SpKgz63, ( *m_pSpcListData )->stval.SpTszg63 );
	memmove ( DiagData.data_val, val, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_12B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 6.3%
	memset ( val, '\0', MONY_BUF_SIZE );
	m_pArith->l_sub ( val, ( *m_pSpcListData )->stval.SpKgz624, ( *m_pSpcListData )->stval.SpTszg624 );
	memmove ( DiagData.data_val, val, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_12C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 6.24%
	memset ( val, '\0', MONY_BUF_SIZE );
	m_pArith->l_sub ( val, ( *m_pSpcListData )->stval.SpKgz78, ( *m_pSpcListData )->stval.SpTszg78 );
	memmove ( DiagData.data_val, val, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_12D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 7.8%

	// 13. 調整割合 ( 計算表4 ( 4 ) ) 	分子
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpFsyu, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_13_bunshi, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	// 調整割合 ( 計算表4 ( 4 ) ) 分母
	memset ( val, '\0', MONY_BUF_SIZE );
	m_pArith->l_add ( val, ( *m_pSpcListData )->stval.SpTuwb, ( *m_pSpcListData )->stval.SpFsyu );
	memmove ( DiagData.data_val, val, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_13_bunbo, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	// 14. 12×13 ( 1円未満の端数切捨て )
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpTwag4, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_14A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 4%
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpTwag63, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_14B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 6.3%
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpTwag624, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_14C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 6.24%
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpTwag78, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_14D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 7.8%

	// 15. 特定収入に係る課税仕入れ等の税額 ( 11+14 )
	memset(val, '\0', MONY_BUF_SIZE);
	m_pArith->l_add(val, (*m_pSpcListData)->stval.SpTszg4, (*m_pSpcListData)->stval.SpTwag4);
	memmove(DiagData.data_val, val, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID503_15A, (struct IUnknown *)&DiagData, CIcsdiagctrl);			// 4%
	memset(val, '\0', MONY_BUF_SIZE);
	m_pArith->l_add(val, (*m_pSpcListData)->stval.SpTszg63, (*m_pSpcListData)->stval.SpTwag63);
	memmove(DiagData.data_val, val, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID503_15B, (struct IUnknown *)&DiagData, CIcsdiagctrl);			// 6.3%
	memset(val, '\0', MONY_BUF_SIZE);
	m_pArith->l_add(val, (*m_pSpcListData)->stval.SpTszg624, (*m_pSpcListData)->stval.SpTwag624);
	memmove(DiagData.data_val, val, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID503_15C, (struct IUnknown *)&DiagData, CIcsdiagctrl);			// 6.24%
	memset(val, '\0', MONY_BUF_SIZE);
	m_pArith->l_add(val, (*m_pSpcListData)->stval.SpTszg78, (*m_pSpcListData)->stval.SpTwag78);
	memmove(DiagData.data_val, val, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID503_15D, (struct IUnknown *)&DiagData, CIcsdiagctrl);			// 7.8%

	//memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpTszg4, MONY_BUF_SIZE );
	//diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_15A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 4%
	//memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpTszg63, MONY_BUF_SIZE );
	//diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_15B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 6.3%
	//memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpTszg624, MONY_BUF_SIZE );
	//diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_15C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 6.24%
	//memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpTszg78, MONY_BUF_SIZE );
	//diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_15D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 7.8%

	// 16. 控除対象仕入税額
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpSiz4, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_16A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 4%
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpSiz63, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_16B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 6.3%
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpSiz624, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_16C, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 6.24%
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpSiz78, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_16D, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 7.8%
}


void CShinSpecific53_31::OnSize(UINT nType, int cx, int cy)
{
	if ( m_initfg ) {
		SetScrollPos ( SB_VERT, 0, TRUE );
		ScrollWindow ( 0, m_cmPos, NULL );
		m_cmPos = 0;
	}

	CSpcBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if ( m_initfg ) {
		RECT	a, b;
		SCROLLINFO ScrollInfo;
		GetWindowRect ( &a );
		m_Specific53.GetRect ( BottomIndex, ( LPUNKNOWN )&b );
		if ( GetScrollInfo( SB_VERT, &ScrollInfo ) == FALSE ) {
			ScrollInfo.nPos = 0;
		}

		ScrollInfo.nMin  = 0;
		ScrollInfo.nMax  = ( b.bottom + 30 ) - a.top + ScrollInfo.nPos - m_cmPos;
		ScrollInfo.nPage = a.bottom - a.top;
		SetScrollInfo ( SB_VERT, &ScrollInfo );
		m_wPos = 0;
		m_ScW = 15;
	}
}


HBRUSH CShinSpecific53_31::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSpcBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
	if ( nCtlColor == CTLCOLOR_DLG ) {
		hbr = Ibrsh_ShinSpecific;
	}

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

void CShinSpecific53_31::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	if ( pScrollBar ) {
		return;
	}

	SCROLLINFO SI;
	GetScrollInfo ( SB_VERT, &SI );
	int nowPos = m_wPos;

	switch ( nSBCode ) {
	case SB_LINEUP:
	case SB_PAGEUP:
		if ( nSBCode == SB_LINEUP ) {
			m_wPos -= m_ScW;
		}
		else {
			m_wPos -= SI.nPage;
		}
		
		if ( m_wPos <= 0 ) {
			m_wPos = 0;
		}
		break;
	case SB_LINEDOWN:
	case SB_PAGEDOWN:
		if ( nSBCode == SB_LINEDOWN ) {
			m_wPos += m_ScW;
		}
		else {
			m_wPos += SI.nPage;
		}
		
		if ( m_wPos >= SI.nMax-( int )SI.nPage ) {
			m_wPos = SI.nMax-SI.nPage;
		}
		break;
	case SB_THUMBTRACK:
		m_wPos = nPos;
		break;
	}
	
	if ( ( nSBCode == SB_LINEDOWN ) || ( nSBCode == SB_LINEUP ) || ( nSBCode == SB_PAGEDOWN ) ||
		 ( nSBCode == SB_PAGEUP ) || ( nSBCode == SB_THUMBTRACK ) ) {
		ScrollWindow ( 0, nowPos - m_wPos );
		SetScrollPos ( SB_VERT, m_wPos );
		m_cmPos = m_wPos;
	}

	CSpcBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

//======================================================
// ダイアグラム初期設定
//======================================================
void CShinSpecific53_31::InitDiagAttr()
{
	if ( ( ( *m_pSnHeadData )->Sn_UPERS & 0x10 ) && ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		int idx = 0;
		for ( int i = 0; i < 4; i++ ) {
			idx = ID503_11[i];
			if ( idx != 0 ) {
				ATRIB_MOD ( IDC_ICSDIAG8CTRL1, idx, 0, ( *m_pSnHeadData )->Sn_Sign4, 0 );
			}
		}
	}
	else {
		if ( ( *m_pSnHeadData )->Sn_UPERS & 0x10 ) {
			// 非連動の場合課税売上割合が小数点以下第2位までしか入力できないため、
			// 11欄を手入力できるように対応
			if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID503_11A, 0 );
				diag_modify( IDC_ICSDIAG8CTRL1, ID503_11A, DIAG_MDFY_EDIT, CIcsdiagctrl );

				ChangeColor ( IDC_ICSDIAG8CTRL1, ID503_11B, 0 );
				diag_modify( IDC_ICSDIAG8CTRL1, ID503_11B, DIAG_MDFY_EDIT, CIcsdiagctrl );
			}

			ChangeColor ( IDC_ICSDIAG8CTRL1, ID503_11C, 0 );
			diag_modify( IDC_ICSDIAG8CTRL1, ID503_11C, DIAG_MDFY_EDIT, CIcsdiagctrl );

			ChangeColor ( IDC_ICSDIAG8CTRL1, ID503_11D, 0 );
			diag_modify( IDC_ICSDIAG8CTRL1, ID503_11D, DIAG_MDFY_EDIT, CIcsdiagctrl );
		}
		else {
			if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
				ChangeColor ( IDC_ICSDIAG8CTRL1, ID503_11A, 1 );
				diag_modify( IDC_ICSDIAG8CTRL1, ID503_11A, DIAG_MDFY_READONLY, CIcsdiagctrl );

				ChangeColor ( IDC_ICSDIAG8CTRL1, ID503_11B, 1 );
				diag_modify( IDC_ICSDIAG8CTRL1, ID503_11B, DIAG_MDFY_READONLY, CIcsdiagctrl );
			}

			ChangeColor ( IDC_ICSDIAG8CTRL1, ID503_11C, 1 );
			diag_modify( IDC_ICSDIAG8CTRL1, ID503_11C, DIAG_MDFY_READONLY, CIcsdiagctrl );

			ChangeColor ( IDC_ICSDIAG8CTRL1, ID503_11D, 1 );
			diag_modify( IDC_ICSDIAG8CTRL1, ID503_11D, DIAG_MDFY_READONLY, CIcsdiagctrl );

			( *m_pSpcListData )->stval.SpUwSgn53[0] &= ~0x0f;
		}
	}

	// 連動時、入力ロック中はメッセージ非表示
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) || !( ( *m_pSnHeadData )->Sn_UPERS & 0x10 ) ) {
		m_Specific53.ModifyItem ( 156, DIAG_MDFY_TRANSPARENT );
	}
	else {
		m_Specific53.ModifyItem ( 156, DIAG_MDFY_OPAQUE );
	}

	m_Specific53.EnableDelete ( 0 );
}


//=========================================================
// 入力項目色変更
//---------------------------------------------------------
// 引数
//			id			:		ダイアグラムID
//			Index		:		ダイアグラム内インデックス
//			sign		:		変更サイン
//=========================================================
void CShinSpecific53_31::ChangeColor ( unsigned short id, short Index, int sign )
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

void CShinSpecific53_31::EditONIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_curwnd = IDC_ICSDIAG8CTRL1;

}

void CShinSpecific53_31::EditOFFIcsdiag8ctrl1(short index)
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
	case ID503_11A:
		if ( m_pArith->l_cmp ( ( *m_pSpcListData )->stval.SpTszg4, DiagData.data_val ) ) {
			memmove ( ( *m_pSpcListData )->stval.SpTszg4, DiagData.data_val, MONY_BUF_SIZE );
			( *m_pSpcListData )->stval.SpUwSgn53[0] |= 0x01;
		}
		break;
	case ID503_11B:
		if ( m_pArith->l_cmp ( ( *m_pSpcListData )->stval.SpTszg63, DiagData.data_val ) ) {
			memmove ( ( *m_pSpcListData )->stval.SpTszg63, DiagData.data_val, MONY_BUF_SIZE );
			( *m_pSpcListData )->stval.SpUwSgn53[0] |= 0x02;
		}
		break;
	case ID503_11C:
		if ( m_pArith->l_cmp ( ( *m_pSpcListData )->stval.SpTszg624, DiagData.data_val ) ) {
			memmove ( ( *m_pSpcListData )->stval.SpTszg624, DiagData.data_val, MONY_BUF_SIZE );
			( *m_pSpcListData )->stval.SpUwSgn53[0] |= 0x04;
		}
		break;
	case ID503_11D:
		if ( m_pArith->l_cmp ( ( *m_pSpcListData )->stval.SpTszg78, DiagData.data_val ) ) {
			memmove ( ( *m_pSpcListData )->stval.SpTszg78, DiagData.data_val, MONY_BUF_SIZE );
			( *m_pSpcListData )->stval.SpUwSgn53[0] |= 0x08;
		}
		break;
	default:
		break;
	}

	( *m_pSpcListData )->SPassOffCalqMain ( ( *m_pSnHeadData ) );
	SpecificDisp53();
	( ( CTaxationList31View* )m_pParent )->LinkHonpyoData ( TRUE );
}

void CShinSpecific53_31::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
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
		m_Specific53.SetNextPosition();
		m_SySgn = FALSE;
	}
	else if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 ) ) {
		m_SySgn = TRUE;
		m_Specific53.SetPrevPosition();
		m_SySgn = FALSE;
	}

	if ( nChar == VK_DELETE ) {
		switch ( index ) {
		case ID503_11A:
			( *m_pSpcListData )->stval.SpUwSgn53[0] &= ~0x01;
			break;
		case ID503_11B:
			( *m_pSpcListData )->stval.SpUwSgn53[0] &= ~0x02;
			break;
		case ID503_11C:
			( *m_pSpcListData )->stval.SpUwSgn53[0] &= ~0x04;
			break;
		case ID503_11D:
			( *m_pSpcListData )->stval.SpUwSgn53[0] &= ~0x08;
			break;
		default:
			break;
		}

		m_SySgn = TRUE;
		m_Specific53.SetPosition ( index );
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
void CShinSpecific53_31::OnBnClickedButton1()
{
	CSpeAddDlg SpeAdd(this);
	SpeAdd.InitInfo(*m_pSnHeadData,*m_pSpcListData,m_pArith,m_pZmSub);
	if(SpeAdd.DoModal() == 0)	{
		// 再計算
		Specific53_Calq();
		// 再表示
		SpecificDisp53();
	}
// 240501_168729 add -->
	GetDlgItem(IDC_BUTTON1)->SetFocus();
// 240501_168729 add <--
}

// 再計算
void CShinSpecific53_31::Specific53_Calq(void)
{
	BOOL			IsChgDoSpc = FALSE;
	unsigned char	svSn_Sign2 = (*m_pSnHeadData)->Sn_Sign2;

	(*m_pSpcListData)->SPassOffCalqMain(*m_pSnHeadData);

// 24/04/26特定収入追加 add -->
	// 5-2のデータがあればガイドを表示する
	if((*m_pSpcListData)->m_bIs52Data == TRUE)	{
		// 表示
		diag_modify(IDC_ICSDIAG8CTRL1,ID503_GUID,DIAG_MDFY_OPAQUE,CIcsdiagctrl);
	}
	else	{
		// 非表示
		diag_modify(IDC_ICSDIAG8CTRL1,ID503_GUID,DIAG_MDFY_TRANSPARENT,CIcsdiagctrl);
	}
// 24/04/26特定収入追加 add <--

	IsChgDoSpc = FALSE;
	if((svSn_Sign2 & 0x02) != ((*m_pSnHeadData)->Sn_Sign2 & 0x02)) {
		IsChgDoSpc = TRUE;
	}
	((CTaxationList31View*)m_pParent)->LinkHonpyoData(IsChgDoSpc);
}
// 24/04/19特定収入追加 add <--
