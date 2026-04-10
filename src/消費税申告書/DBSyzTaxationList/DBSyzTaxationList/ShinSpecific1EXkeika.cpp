// ShinSpecific1.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinSpecific1EXkeika.h"
#include "ShinSpecific1EXkeikaIdx.h"
#include "TaxationListView.h"

//== 定数 ==
const	int	BottomIndex = 9;
const	int	StartIndex = 19;

// CShinSpecific1EXkeika ダイアログ

IMPLEMENT_DYNAMIC(CShinSpecific1EXkeika, CSpcBaseDlg)

CShinSpecific1EXkeika::CShinSpecific1EXkeika(CWnd* pParent /*=NULL*/)
	: CSpcBaseDlg(CShinSpecific1EXkeika::IDD, pParent)
{

	m_curwnd = -1;

	m_FcsUpEnd		= -1;
	m_FcsLeftEnd	= -1;
	m_FcsDownEnd	= -1;
	m_FcsRightEnd	= -1;

}

CShinSpecific1EXkeika::~CShinSpecific1EXkeika()
{
}

void CShinSpecific1EXkeika::DoDataExchange(CDataExchange* pDX)
{
	CSpcBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Specific1);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL2, m_diag_dmy1);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL3, m_diag_dmy2);
}


BEGIN_MESSAGE_MAP(CShinSpecific1EXkeika, CSpcBaseDlg)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CShinSpecific1EXkeika メッセージ ハンドラ

BOOL CShinSpecific1EXkeika::OnInitDialog()
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

BOOL CShinSpecific1EXkeika::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。


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

}

void CShinSpecific1EXkeika::OnSize(UINT nType, int cx, int cy)
{
	CSpcBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( m_initfg ){
		RECT	a, b;

		GetWindowRect( &a );

		m_Specific1.GetRect( BottomIndex, (LPUNKNOWN)&b );
	}
}

HBRUSH CShinSpecific1EXkeika::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSpcBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
	if( nCtlColor == CTLCOLOR_DLG ){
		hbr = Ibrsh_ShinSpecific;
	}

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

void CShinSpecific1EXkeika::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
}

//-----------------------------------------------------------------------------
// 再描画
//-----------------------------------------------------------------------------
void CShinSpecific1EXkeika::sykscroll( int mpos )
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
void CShinSpecific1EXkeika::SpecificInit1( void )
{
	// 倍長演算指定
	m_pArith->l_defn( 0x16 );

	SpecificDisp1();

	IndexControl();

	SetItemPosition();
}

//-----------------------------------------------------------------------------
// 表示
//-----------------------------------------------------------------------------
void CShinSpecific1EXkeika::SpecificDisp1( void )
{
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;



	if( ((*m_pSnHeadData)->Sn_TUMIAG&0x01) && (m_pZmSub->zvol->s_sgn1&0xc0) ){
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKzur, sizeof(DiagData.data_val) );
		diag_setdata( IDC_ICSDIAG8CTRL1, ID11, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		diag_clear( IDC_ICSDIAG8CTRL1, ID10, TRUE, CIcsdiagctrl );

		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKzur4, sizeof(DiagData.data_val) );
		diag_setdata( IDC_ICSDIAG8CTRL1, ID21, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		diag_clear( IDC_ICSDIAG8CTRL1, ID20, TRUE, CIcsdiagctrl );

		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKzur63, sizeof(DiagData.data_val) );
		diag_setdata( IDC_ICSDIAG8CTRL1, ID31, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		diag_clear( IDC_ICSDIAG8CTRL1, ID30, TRUE, CIcsdiagctrl );
	}
	else{
		// 課税売上
		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKzur, sizeof(DiagData.data_val) );
		diag_setdata( IDC_ICSDIAG8CTRL1, ID10, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		diag_clear( IDC_ICSDIAG8CTRL1, ID11, TRUE, CIcsdiagctrl );

		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKzur4, sizeof(DiagData.data_val) );
		diag_setdata( IDC_ICSDIAG8CTRL1, ID20, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		diag_clear( IDC_ICSDIAG8CTRL1, ID21, TRUE, CIcsdiagctrl );

		memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKzur63, sizeof(DiagData.data_val) );
		diag_setdata( IDC_ICSDIAG8CTRL1, ID30, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		diag_clear( IDC_ICSDIAG8CTRL1, ID31, TRUE, CIcsdiagctrl );
	}

	// 免税売上げ
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpMzur, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID12, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 非課税売上げ
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpHkur, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID13, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 国外における資産の譲渡等の対価の額
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpKgjt, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID14, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 資産の譲渡等の対価の額の合計
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTuwb, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID15, (struct IUnknown *)&DiagData, CIcsdiagctrl );

}

//-----------------------------------------------------------------------------
// 再計算
//-----------------------------------------------------------------------------
void CShinSpecific1EXkeika::Specific1_Calq( void )
{
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;
	memset( (*m_pSpcListData)->stval.SpTuwb, '\0', sizeof((*m_pSpcListData)->stval.SpTuwb) );	// 分母
	m_pArith->l_add( (*m_pSpcListData)->stval.SpTuwb, (*m_pSpcListData)->stval.SpTuwb, (*m_pSpcListData)->stval.SpKzur );
	m_pArith->l_add( (*m_pSpcListData)->stval.SpTuwb, (*m_pSpcListData)->stval.SpTuwb, (*m_pSpcListData)->stval.SpMzur );
	m_pArith->l_add( (*m_pSpcListData)->stval.SpTuwb, (*m_pSpcListData)->stval.SpTuwb, (*m_pSpcListData)->stval.SpHkur );
	m_pArith->l_add( (*m_pSpcListData)->stval.SpTuwb, (*m_pSpcListData)->stval.SpTuwb, (*m_pSpcListData)->stval.SpKgjt );
	// 資産の譲渡等の対価の額の合計
	memmove( DiagData.data_val, (*m_pSpcListData)->stval.SpTuwb, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL1, ID15, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	unsigned char	svSn_Sign2 = (*m_pSnHeadData)->Sn_Sign2;
	(*m_pSpcListData)->SPassOffCalqMain( *m_pSnHeadData );
	BOOL	IsChgDoSpc = FALSE;
	if( (svSn_Sign2&0x02) != ((*m_pSnHeadData)->Sn_Sign2&0x02) ){
		IsChgDoSpc = TRUE;
	}
	((CTaxationListView*)m_pParent)->LinkHonpyoData(IsChgDoSpc);

	SpecificDisp1();

}

BEGIN_EVENTSINK_MAP(CShinSpecific1EXkeika, CSpcBaseDlg)
	ON_EVENT(CShinSpecific1EXkeika, IDC_ICSDIAG8CTRL1, 1, CShinSpecific1EXkeika::EditONIcsdiag8ctrl1, VTS_I2)//yoshida150310
	ON_EVENT(CShinSpecific1EXkeika, IDC_ICSDIAG8CTRL1, 2, CShinSpecific1EXkeika::EditOFFIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinSpecific1EXkeika, IDC_ICSDIAG8CTRL1, 3, CShinSpecific1EXkeika::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinSpecific1EXkeika::EditOFFIcsdiag8ctrl1(short index)
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
	if( IsWindowVisible() == FALSE ){
		return;
	}

	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	switch( index ){
		case ID20	:	
			diag_getdata( IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( (*m_pSpcListData)->stval.SpKzur4, DiagData.data_val, MONY_BUF_SIZE );
			break;
		case ID21	:	
			diag_getdata( IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( (*m_pSpcListData)->stval.SpKzur4, DiagData.data_val, MONY_BUF_SIZE );
			break;
		case ID30	:	
			diag_getdata( IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( (*m_pSpcListData)->stval.SpKzur63, DiagData.data_val, MONY_BUF_SIZE );
			break;
		case ID31	:	
			diag_getdata( IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( (*m_pSpcListData)->stval.SpKzur63, DiagData.data_val, MONY_BUF_SIZE );
			break;
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
		case ID14	:	
			diag_getdata( IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			memmove( (*m_pSpcListData)->stval.SpKgjt, DiagData.data_val, MONY_BUF_SIZE );
			break;
		default     :	
			break;
	}
	// 再計算
	Specific1_Calq();
}

void CShinSpecific1EXkeika::GetDiagData()
{

	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	//diag_getdata( IDC_ICSDIAG8CTRL1, ID20, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	//memmove( (*m_pSpcListData)->stval.SpKzur4, DiagData.data_val, MONY_BUF_SIZE );
	//diag_getdata( IDC_ICSDIAG8CTRL1, ID21, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	//memmove( (*m_pSpcListData)->stval.SpKzur4, DiagData.data_val, MONY_BUF_SIZE );
	//diag_getdata( IDC_ICSDIAG8CTRL1, ID30, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	//memmove( (*m_pSpcListData)->stval.SpKzur63, DiagData.data_val, MONY_BUF_SIZE );
	//diag_getdata( IDC_ICSDIAG8CTRL1, ID31, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	//memmove( (*m_pSpcListData)->stval.SpKzur63, DiagData.data_val, MONY_BUF_SIZE );
	//diag_getdata( IDC_ICSDIAG8CTRL1, ID10, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	//memmove( (*m_pSpcListData)->stval.SpKzur, DiagData.data_val, MONY_BUF_SIZE );
	//diag_getdata( IDC_ICSDIAG8CTRL1, ID11, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	//memmove( (*m_pSpcListData)->stval.SpKzur, DiagData.data_val, MONY_BUF_SIZE );
	//diag_getdata( IDC_ICSDIAG8CTRL1, ID12, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	//memmove( (*m_pSpcListData)->stval.SpMzur, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL1, ID13, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( (*m_pSpcListData)->stval.SpHkur, DiagData.data_val, MONY_BUF_SIZE );
	diag_getdata( IDC_ICSDIAG8CTRL1, ID14, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memmove( (*m_pSpcListData)->stval.SpKgjt, DiagData.data_val, MONY_BUF_SIZE );
}

void CShinSpecific1EXkeika::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
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
		m_Specific1.DataClear( index, TRUE );
		m_Specific1.SetPosition(index);
	}
	
	// 再計算
	Specific1_Calq();

	//フォーカスがキー移動したら消えてたのを修正
	if( pos == 0 ){
		m_Specific1.SetPosition(index);
	}else{
		m_Specific1.SetPosition(pos);
	}

	SetRedrawScrollToIdx(m_Specific1.GetPosition());

}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void CShinSpecific1EXkeika::IndexControl( void )
{
	int	Cnt = 0;
	while( 1 ){
		if( Specific1ExKeika[Cnt].Index == 0 ){
			break;
		}
		int	Flg;
		// 確定時は全入力不可
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			Flg = Specific1ExKeika[Cnt].AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
				Flg = Specific1ExKeika[Cnt].UnConnect;
			}
			else{
				Flg = Specific1ExKeika[Cnt].Connect;
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
		ATRIB_MOD( IDC_ICSDIAG8CTRL1, Specific1ExKeika[Cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, Specific1ExKeika[Cnt].Connect );
		Cnt++;
	}

	SetFocusRange();

	m_Specific1.EnableDelete(0);//DELETEキー１回で削除モード設定

}

void CShinSpecific1EXkeika::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
//	CSpcBaseDlg::OnOK();
}

void CShinSpecific1EXkeika::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
//	CSpcBaseDlg::OnCancel();
}

void CShinSpecific1EXkeika::EditONIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	
	m_curwnd = IDC_ICSDIAG8CTRL1;//'14.08.01

}




//-----------------------------------------------------------------------------
// 次フォーカスの場所検索（上下左右動作）
//-----------------------------------------------------------------------------
//　戻り値　：　次のフォーカスインデックス
//-----------------------------------------------------------------------------
int CShinSpecific1EXkeika::FocusMoveCheck(int Index , int Move)
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
		for(i = 0 ; i < FCS_SPECIFIC1CNT; i++){
			if(Index == FCS_Specific1ExKeika[i].IDNo){
				break;
			}
		}
		idx = FCS_Specific1ExKeika[i].IDNo;//現在ポジションセット
	}else if(( Move == 2 )||( Move == 3 )){
		for(i = 0 ; i < FCS_SPECIFIC1CNT; i++){
			if(Index == FCS_Specific1ExKeika_LR[i].IDNo){
				break;
			}
		}
		idx = FCS_Specific1ExKeika_LR[i].IDNo;//現在ポジションセット
	}

	//次ポジション検索
	if(Move == 0){//下方向
		for(j = i + 1 ; j < FCS_SPECIFIC1CNT ; j++){
			m_Specific1.GetAttribute(FCS_Specific1ExKeika[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Specific1ExKeika[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 1){//上方向
		for(j = i - 1 ; j > -1 ; j--){
			m_Specific1.GetAttribute(FCS_Specific1ExKeika[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Specific1ExKeika[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 2){//右方向
		for(j = i + 1 ; j < FCS_SPECIFIC1CNT ; j++){
			m_Specific1.GetAttribute(FCS_Specific1ExKeika_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Specific1ExKeika_LR[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 3){//左方向
		for(j = i - 1 ; j > -1 ; j--){
			m_Specific1.GetAttribute(FCS_Specific1ExKeika_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Specific1ExKeika_LR[j].IDNo;
				break;
			}
		}
	}
	return idx;
}


void CShinSpecific1EXkeika::SetFocusRange()
{

	DIAGRAM_ATTRIBUTE	DA;

	m_FcsUpEnd		= -1;
	m_FcsLeftEnd	= -1;
	m_FcsDownEnd	= -1;
	m_FcsRightEnd	= -1;

	for( int idx = 0; idx < FCS_SPECIFIC1CNT; idx++ ){
		m_Specific1.GetAttribute(FCS_Specific1ExKeika[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsUpEnd = FCS_Specific1ExKeika[idx].IDNo;
			break;
		}
	}
	for( int idx = 0; idx < FCS_SPECIFIC1CNT; idx++ ){
		m_Specific1.GetAttribute(FCS_Specific1ExKeika_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsLeftEnd = FCS_Specific1ExKeika_LR[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SPECIFIC1CNT-1; idx > -1 ; idx-- ){
		m_Specific1.GetAttribute(FCS_Specific1ExKeika[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsDownEnd = FCS_Specific1ExKeika[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SPECIFIC1CNT-1; idx > -1 ; idx-- ){
		m_Specific1.GetAttribute(FCS_Specific1ExKeika_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsRightEnd = FCS_Specific1ExKeika_LR[idx].IDNo;
			break;
		}
	}
}


//-----------------------------------------------------------------------------
// 初期ポジションセット
//-----------------------------------------------------------------------------
void CShinSpecific1EXkeika::SetItemPosition( BOOL IsInEnd )
{
	int index;

	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAG8CTRL1 ) {
		if( (index = m_Specific1.GetPosition()) != -1) {

			//if( IsInEnd ){
				RECT rc,rc_dmy1,rc_dmy2;
				::ZeroMemory(&rc,sizeof(RECT));
				::ZeroMemory(&rc_dmy1,sizeof(RECT));
				::ZeroMemory(&rc_dmy2,sizeof(RECT));

				m_Specific1.GetRect(index,(LPUNKNOWN)&rc);
				m_diag_dmy1.GetRect(0,(LPUNKNOWN)&rc_dmy1);
				m_diag_dmy2.GetRect(0,(LPUNKNOWN)&rc_dmy2);

				if( (rc_dmy1.top<rc.top)&&(rc.bottom<rc_dmy2.bottom)){
					m_Specific1.SetPosition(index);
				}			
				m_Specific1.SetFocus();
			//}else{
			//	m_Specific1.SetPosition(index);
			//	m_Specific1.SetFocus();	
			//}

			return;
		}
	}

	this->SetFocus();
	if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
		m_Specific1.SetPosition(ID13);
	}
	else{
		m_Specific1.SetPosition(ID14);
	}
	SetRedrawScroll( 0 );

}



//<-----------------------------------------