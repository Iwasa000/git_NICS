//yokono
// ShinSpecific53_31.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinSpecific53_31Ex.h"
#include "ShinSpecific53_31_exIdx.h"
#include "TaxationList31View.h"

const	int	BottomIndex = 17;
const int ID503_7[] = { ID503_7A, ID503_7B, 0 };

// CShinSpecific53_31Ex ダイアログ

IMPLEMENT_DYNAMIC(CShinSpecific53_31Ex, CSpcBaseDlg)

CShinSpecific53_31Ex::CShinSpecific53_31Ex(CWnd* pParent /*=nullptr*/)
	: CSpcBaseDlg(IDD_DIALOG_K53_31_ex, pParent)
	, m_wPos(0)
	, m_cmPos(0)
	, m_ScW(0)
	, m_SySgn ( FALSE )
{
	m_curwnd = -1;

}

CShinSpecific53_31Ex::~CShinSpecific53_31Ex()
{
}

void CShinSpecific53_31Ex::DoDataExchange(CDataExchange* pDX)
{
	CSpcBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Specific53Ex);
}

BEGIN_MESSAGE_MAP(CShinSpecific53_31Ex, CSpcBaseDlg)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CShinSpecific53_31Ex メッセージ ハンドラー
BOOL CShinSpecific53_31Ex::PreTranslateMessage(MSG* pMsg)
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

BOOL CShinSpecific53_31Ex::OnInitDialog()
{
	CSpcBaseDlg::OnInitDialog();

	// TODO: ここに初期化を追加してください
	SpecificInit53Ex();
	m_initfg = 1;
	CSpcBaseDlg::OnInitDialogEX();

	return TRUE;
}

void CShinSpecific53_31Ex::SpecificInit53Ex()
{
	// 倍長演算指定
	m_pArith->l_defn( 0x16 );

	InitDiagAttr();

	// 金額表示
	SpecificDisp53Ex();

	//カーソルの位置をセット
	SetItemPosition();
}

void CShinSpecific53_31Ex::SetItemPosition()
{
	int index ;

	//前回のカーソル位置の復元
	if ( m_curwnd == IDC_ICSDIAG8CTRL1 ){
		if ( (index = m_Specific53Ex.GetPosition()) != -1 ) {
			if ( ( index == ID503_7A ) || ( index == ID503_7B ) ) {
				index = ID503_7A;
			}
			m_Specific53Ex.SetPosition(index) ;
			m_Specific53Ex.SetFocus() ;
			return ;
		}
	}

	this->SetFocus() ;
	if ( (*m_pSnHeadData)->Sn_Sign4 & 0x80 ){
		if ( ( *m_pSnHeadData )->Sn_UPERS & 0x10 ) {
			m_Specific53Ex.SetPosition ( ID503_7A );
		}
		else {
			m_Specific53Ex.SetPosition(ID503_1A) ;
		}
	}
	else{
	}
}

//==============================
// 金額表示
//==============================
void CShinSpecific53_31Ex::SpecificDisp53Ex()
{
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;

	char val[MONY_BUF_SIZE] = { 0 };

	// 1. 調整前の課税仕入れ等の税額の合計額
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpKgz624, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_1A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 6.24%
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpKgz78, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_1B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 7.8%

	// 2. 課税仕入れ等 ( 税率6.24% )にのみ使途が特定されている特定収入 (「課税仕入れ等に係る特定収入」 ) ( 計算表2 ( 1 ) 17B )
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpTsyu624, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_2A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	// 3 6×6.24/108
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpZszg624, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_3A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	// 4. 課税仕入れ等 ( 税率7.8% )にのみ使途が特定されている特定収入 (「課税仕入れ等に係る特定収入」 ) ( 計算表2 ( 1 ) 17C )
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpTsyu78, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_4B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	// 5. 8×7.8/110
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpZszg78, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_5B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	if ( ( *m_pSnHeadData )->Sn_UPERS & 0x10 ) {
		diag_modify ( IDC_ICSDIAG8CTRL1, ID503_6_wariai, DIAG_MDFY_OPAQUE, CIcsdiagctrl );
		
		diag_modify ( IDC_ICSDIAG8CTRL1, ID503_6_bunshi, DIAG_MDFY_TRANSPARENT, CIcsdiagctrl );
		diag_modify ( IDC_ICSDIAG8CTRL1, ID503_6_bunbo, DIAG_MDFY_TRANSPARENT, CIcsdiagctrl );
		diag_modify ( IDC_ICSDIAG8CTRL1, 31, DIAG_MDFY_TRANSPARENT, CIcsdiagctrl );

		char buf[64] = { 0 }, Uwri[64] = { 0 };
		memset ( buf, '\0', sizeof ( buf ) );
		memset ( Uwri, '\0', sizeof ( Uwri ) );
		m_pArith->l_print ( Uwri, ( *m_pSpcListData )->stval.SpUwri, _T( "SS9.99" ) );
		sprintf_s ( buf, sizeof ( buf ), _T( "%s％" ), Uwri );
		DiagData.data_disp = ( char* )buf;
		diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_6_wariai, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	}
	else {
		diag_modify ( IDC_ICSDIAG8CTRL1, ID503_6_wariai, DIAG_MDFY_TRANSPARENT, CIcsdiagctrl );

		diag_modify ( IDC_ICSDIAG8CTRL1, ID503_6_bunshi, DIAG_MDFY_OPAQUE, CIcsdiagctrl );
		diag_modify ( IDC_ICSDIAG8CTRL1, ID503_6_bunshi, DIAG_MDFY_READONLY, CIcsdiagctrl);
		diag_modify ( IDC_ICSDIAG8CTRL1, ID503_6_bunbo, DIAG_MDFY_OPAQUE, CIcsdiagctrl );
		diag_modify ( IDC_ICSDIAG8CTRL1, ID503_6_bunbo, DIAG_MDFY_READONLY, CIcsdiagctrl);
		diag_modify ( IDC_ICSDIAG8CTRL1, 31, DIAG_MDFY_OPAQUE, CIcsdiagctrl );

		// 6. 課税売上割合 分子
		memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpKuws, MONY_BUF_SIZE );
		diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_6_bunshi, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		// 6. 課税売上割合 分母
		memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpKuwb, MONY_BUF_SIZE );
		diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_6_bunbo, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
	}

	// 7. 3×6、5×6 ( いずれも1円未満の端数切捨て )
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpTszg624, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_7A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 6.24%
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpTszg78, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_7B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 7.8%
	if ( ( ( *m_pSnHeadData )->Sn_UPERS & 0x10 ) && !( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		ChangeColor ( IDC_ICSDIAG8CTRL1, ID503_7A, ( ( *m_pSpcListData )->stval.SpUwSgn53[0] & 0x04 ) ? 3 : 0 );
		ChangeColor ( IDC_ICSDIAG8CTRL1, ID503_7B, ( ( *m_pSpcListData )->stval.SpUwSgn53[0] & 0x08 ) ? 3 : 0 );
	}

	// 8. 1-7
	memset ( val, '\0', MONY_BUF_SIZE );
	m_pArith->l_sub ( val, ( *m_pSpcListData )->stval.SpKgz624, ( *m_pSpcListData )->stval.SpTszg624 );
	memmove ( DiagData.data_val, val, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_8A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 6.24%
	memset ( val, '\0', MONY_BUF_SIZE );
	m_pArith->l_sub ( val, ( *m_pSpcListData )->stval.SpKgz78, ( *m_pSpcListData )->stval.SpTszg78 );
	memmove ( DiagData.data_val, val, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_8B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 7.8%

	// 9. 調整割合 ( 計算表4 ( 4 ) ) 	分子
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpFsyu, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_9_bunshi, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	// 調整割合 ( 計算表4 ( 4 ) ) 分母
	memset ( val, '\0', MONY_BUF_SIZE );
	m_pArith->l_add ( val, ( *m_pSpcListData )->stval.SpTuwb, ( *m_pSpcListData )->stval.SpFsyu );
	memmove ( DiagData.data_val, val, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_9_bunbo, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	// 10 8×9 ( 1円未満の端数切捨て )
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpTwag624, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_10A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 6.24%
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpTwag78, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_10B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 7.8%

	// 11. 特定収入に係る課税仕入れ等の税額 ( 11+14 )
	memset(val, '\0', MONY_BUF_SIZE);
	m_pArith->l_add(val, (*m_pSpcListData)->stval.SpTszg624, (*m_pSpcListData)->stval.SpTwag624);
	memmove(DiagData.data_val, val, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID503_11A, (struct IUnknown *)&DiagData, CIcsdiagctrl);			// 6.24%
	memset(val, '\0', MONY_BUF_SIZE);
	m_pArith->l_add(val, (*m_pSpcListData)->stval.SpTszg78, (*m_pSpcListData)->stval.SpTwag78);
	memmove(DiagData.data_val, val, MONY_BUF_SIZE);
	diag_setdata(IDC_ICSDIAG8CTRL1, ID503_11B, (struct IUnknown *)&DiagData, CIcsdiagctrl);			// 7.8%

	// 12. 控除対象仕入税額
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpSiz624, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_12A, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 6.24%
	memmove ( DiagData.data_val, ( *m_pSpcListData )->stval.SpSiz78, MONY_BUF_SIZE );
	diag_setdata ( IDC_ICSDIAG8CTRL1, ID503_12B, ( struct IUnknown* )&DiagData, CIcsdiagctrl );	// 7.8%
}

//======================================================
// ダイアグラム初期設定
//======================================================
void CShinSpecific53_31Ex::InitDiagAttr()
{
	if ( ( ( *m_pSnHeadData )->Sn_UPERS & 0x10 ) && ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) ) {
		int idx = 0;
		for ( int i = 0; i < 2; i++ ) {
			idx = ID503_7[i];
			if ( idx != 0 ) {
				ATRIB_MOD ( IDC_ICSDIAG8CTRL1, idx, 0, ( *m_pSnHeadData )->Sn_Sign4, 0 );
			}
		}
	}
	else {
		if ( ( *m_pSnHeadData )->Sn_UPERS & 0x10 ) {
			// 非連動の場合課税売上割合が小数点以下第2位までしか入力できないため、
			// 7欄を手入力できるように対応

			ChangeColor ( IDC_ICSDIAG8CTRL1, ID503_7A, 0 );
			diag_modify( IDC_ICSDIAG8CTRL1, ID503_7A, DIAG_MDFY_EDIT, CIcsdiagctrl );

			ChangeColor ( IDC_ICSDIAG8CTRL1, ID503_7B, 0 );
			diag_modify( IDC_ICSDIAG8CTRL1, ID503_7B, DIAG_MDFY_EDIT, CIcsdiagctrl );
		}
		else {
			ChangeColor ( IDC_ICSDIAG8CTRL1, ID503_7A, 1 );
			diag_modify( IDC_ICSDIAG8CTRL1, ID503_7A, DIAG_MDFY_READONLY, CIcsdiagctrl );

			ChangeColor ( IDC_ICSDIAG8CTRL1, ID503_7B, 1 );
			diag_modify( IDC_ICSDIAG8CTRL1, ID503_7B, DIAG_MDFY_READONLY, CIcsdiagctrl );

			( *m_pSpcListData )->stval.SpUwSgn53[0] &= ~0x0f;
		}
	}

	// 連動時、入力ロック中はメッセージ非表示
	if ( !( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) || ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) || !( ( *m_pSnHeadData )->Sn_UPERS & 0x10 ) ) {
		m_Specific53Ex.ModifyItem ( 80, DIAG_MDFY_TRANSPARENT );
	}
	else {
		m_Specific53Ex.ModifyItem ( 80, DIAG_MDFY_OPAQUE );
	}

	m_Specific53Ex.EnableDelete ( 0 );
}


//=========================================================
// 入力項目色変更
//---------------------------------------------------------
// 引数
//			id			:		ダイアグラムID
//			Index		:		ダイアグラム内インデックス
//			sign		:		変更サイン
//=========================================================
void CShinSpecific53_31Ex::ChangeColor ( unsigned short id, short Index, int sign )
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

void CShinSpecific53_31Ex::OnSize(UINT nType, int cx, int cy)
{
	if (m_initfg) {
		SetScrollPos(SB_VERT, 0, TRUE);
		ScrollWindow(0, m_cmPos, NULL);
		m_cmPos = 0;
	}

	CSpcBaseDlg::OnSize(nType, cx, cy);
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if (m_initfg) {
		RECT	a, b;
		SCROLLINFO ScrollInfo;
		GetWindowRect(&a);
		m_Specific53Ex.GetRect(BottomIndex, (LPUNKNOWN)&b);
		if (GetScrollInfo(SB_VERT, &ScrollInfo) == FALSE) {
			ScrollInfo.nPos = 0;
		}

		ScrollInfo.nMin = 0;
		ScrollInfo.nMax = (b.bottom + 30) - a.top + ScrollInfo.nPos - m_cmPos;
		ScrollInfo.nPage = a.bottom - a.top;
		SetScrollInfo(SB_VERT, &ScrollInfo);
		m_wPos = 0;
		m_ScW = 30;
	}
}


HBRUSH CShinSpecific53_31Ex::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSpcBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
	if ( nCtlColor == CTLCOLOR_DLG ) {
		hbr = Ibrsh_ShinSpecific;
	}

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

void CShinSpecific53_31Ex::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	if (pScrollBar) {
		return;
	}

	SCROLLINFO SI;
	GetScrollInfo(SB_VERT, &SI);
	int nowPos = m_wPos;

	switch (nSBCode) {
	case SB_LINEUP:
	case SB_PAGEUP:
		if (nSBCode == SB_LINEUP) {
			m_wPos -= m_ScW;
		}
		else {
			m_wPos -= SI.nPage;
		}

		if (m_wPos <= 0) {
			m_wPos = 0;
		}
		break;
	case SB_LINEDOWN:
	case SB_PAGEDOWN:
		if (nSBCode == SB_LINEDOWN) {
			m_wPos += m_ScW;
		}
		else {
			m_wPos += SI.nPage;
		}

		if (m_wPos >= SI.nMax - (int)SI.nPage) {
			m_wPos = SI.nMax - SI.nPage;
		}
		break;
	case SB_THUMBTRACK:
		m_wPos = nPos;
		break;
	}

	if ((nSBCode == SB_LINEDOWN) || (nSBCode == SB_LINEUP) || (nSBCode == SB_PAGEDOWN) ||
		(nSBCode == SB_PAGEUP) || (nSBCode == SB_THUMBTRACK)) {
		ScrollWindow(0, nowPos - m_wPos);
		SetScrollPos(SB_VERT, m_wPos);
		m_cmPos = m_wPos;
	}
	CSpcBaseDlg::OnVScroll(nSBCode, nPos, pScrollBar);
}

BEGIN_EVENTSINK_MAP(CShinSpecific53_31Ex, CSyzBaseDlg)
	ON_EVENT(CShinSpecific53_31Ex, IDC_ICSDIAG8CTRL1, 1, CShinSpecific53_31Ex::EditONIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinSpecific53_31Ex, IDC_ICSDIAG8CTRL1, 2, CShinSpecific53_31Ex::EditOFFIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinSpecific53_31Ex, IDC_ICSDIAG8CTRL1, 3, CShinSpecific53_31Ex::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinSpecific53_31Ex::EditONIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_curwnd = IDC_ICSDIAG8CTRL1;

}

void CShinSpecific53_31Ex::EditOFFIcsdiag8ctrl1(short index)
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
	case ID503_7A:
		if ( m_pArith->l_cmp ( ( *m_pSpcListData )->stval.SpTszg624, DiagData.data_val ) ) {
			memmove ( ( *m_pSpcListData )->stval.SpTszg624, DiagData.data_val, MONY_BUF_SIZE );
			( *m_pSpcListData )->stval.SpUwSgn53[0] |= 0x04;
		}
		break;
	case ID503_7B:
		if ( m_pArith->l_cmp ( ( *m_pSpcListData )->stval.SpTszg78, DiagData.data_val ) ) {
			memmove ( ( *m_pSpcListData )->stval.SpTszg78, DiagData.data_val, MONY_BUF_SIZE );
			( *m_pSpcListData )->stval.SpUwSgn53[0] |= 0x08;
		}
		break;
	default:
		break;
	}

	( *m_pSpcListData )->SPassOffCalqMain ( ( *m_pSnHeadData ) );
	SpecificDisp53Ex();
	( ( CTaxationList31View* )m_pParent )->LinkHonpyoData ( TRUE );
}

void CShinSpecific53_31Ex::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
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
		m_Specific53Ex.SetNextPosition();
		m_SySgn = FALSE;
	}
	else if ( ( wTerm == VK_TAB ) || ( wTerm == VK_LEFT ) || ( wTerm == VK_F2 ) ) {
		m_SySgn = TRUE;
		m_Specific53Ex.SetPrevPosition();
		m_SySgn = FALSE;
	}

	if ( nChar == VK_DELETE ) {
		switch ( index ) {
			( *m_pSpcListData )->stval.SpUwSgn53[0] &= ~0x02;
			break;
		case ID503_7A:
			( *m_pSpcListData )->stval.SpUwSgn53[0] &= ~0x04;
			break;
		case ID503_7B:
			( *m_pSpcListData )->stval.SpUwSgn53[0] &= ~0x08;
			break;
		default:
			break;
		}

		m_SySgn = TRUE;
		m_Specific53Ex.SetPosition ( index );
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