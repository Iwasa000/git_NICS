// yokono
// ShinSpecific2_31.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinSpecific2_31.h"
#include "ShinSpecific2_31Idx.h"
#include "TaxationList31View.h"

// 定数
const	int	ButtonIndex				= 0;
const	int	StartIndex				= 63;

const	int	NUM_PER_PAGE			= 119;	// 1ページ毎のデータ数（index0～118なんで119）
const	int	DIAG_INP_STIDX			= 63;	// 1ページの入力開始インデックス
const	int	TBL_REC_NUM_PER_PAGE	= 7;	// 1ページ毎のテーブルレコード数（CTblhdl上）
const	int	CLMNUM_PER_ROW			= 8;	// 1行毎の列数



// CShinSpecific2_31 ダイアログ

IMPLEMENT_DYNAMIC(CShinSpecific2_31, CSpcBaseDlg)

CShinSpecific2_31::CShinSpecific2_31(CWnd* pParent /*=nullptr*/)
	: CSpcBaseDlg(IDD_DIALOG_K21_31, pParent)
{
	m_curwnd = -1 ; //現在のウィンドウハンドル

	m_FcsUpEnd = -1 ;//入力可能な範囲を示す変数の初期化
	m_FcsDownEnd = -1 ;
	m_FcsRightEnd = -1 ;
	m_FcsLeftEnd = -1 ;

}

CShinSpecific2_31::~CShinSpecific2_31()
{
}

void CShinSpecific2_31::DoDataExchange(CDataExchange* pDX)
{
	CSpcBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL2, m_Specific21);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL3, m_Specific210);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Specific211);
	DDX_Control(pDX, IDC_ICSDISP8CTRL1, m_ICSDisp1);
}


BEGIN_MESSAGE_MAP(CShinSpecific2_31, CSpcBaseDlg)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON1, &CShinSpecific2_31::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CShinSpecific2_31::OnBnClickedButton2)
	ON_COMMAND(ID_BUTTON_F2, &CShinSpecific2_31::OnButtonF2)
	ON_COMMAND(ID_BUTTON_F3, &CShinSpecific2_31::OnButtonF3)
END_MESSAGE_MAP()


// CShinSpecific2_31 メッセージ ハンドラー


BOOL CShinSpecific2_31::OnInitDialog()
{
	CSpcBaseDlg::OnInitDialog();

	// TODO: ここに初期化を追加してください
	//初期設定
	SpecificInit21() ;

	m_Specific211.EnableDelete(0) ;

	m_initfg = 1 ;

	CSpcBaseDlg::OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CShinSpecific2_31::OnSize(UINT nType, int cx, int cy)
{
	CSpcBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if ( m_initfg )	{

	}
}

HBRUSH CShinSpecific2_31::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSpcBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
	if( nCtlColor == CTLCOLOR_DLG ){
		hbr = Ibrsh_ShinSpecific;
	}

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

void CShinSpecific2_31::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
}

BOOL CShinSpecific2_31::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	if ( pMsg->message == WM_KEYDOWN ) {
		if ( pMsg->wParam == VK_END ){
			if ( m_pParent ) {
				m_pParent->PostMessageA( WM_KEYDOWN, VK_END );
			}
			return TRUE ;
		}

		if ( pMsg->wParam ==VK_F2 ) {
			DIAGRAM_DATA dd; 
			if ( m_curwnd != -1 ) {
				short result = ((CIcsdiagctrl*)GetDlgItem( m_curwnd ))->GetPosition();
				diag_getdata( m_curwnd, result, &dd, CIcsdiagctrl );
				if ( m_curwnd = IDC_ICSDIAG8CTRL1 ) {
					TerminationIcsdiag8ctrl1( result, (short)pMsg->wParam, 0, (LPUNKNOWN)&dd );
				}
				return TRUE ;
			}
		}
		else if ( pMsg->wParam == VK_PRIOR ) {
			m_Specific211.ScrollPos ( SB_PAGEUP, 0 );
		}
		else if ( pMsg->wParam == VK_NEXT ) {
			m_Specific211.ScrollPos ( SB_PAGEDOWN, 0 );
		}
	}

	return CSpcBaseDlg::PreTranslateMessage(pMsg);

}

void CShinSpecific2_31::SpecificInit21()
{
	// 倍長演算指定
	m_pArith->l_defn( 0x16 );

	//確定時は入力不可
	if( ((*m_pSnHeadData)->Sn_Sign4&0x01) || !((*m_pSnHeadData)->Sn_Sign4&0x80) ){
		GetDlgItem( IDC_BUTTON1 )->EnableWindow( FALSE );
		GetDlgItem( IDC_BUTTON2 )->EnableWindow( FALSE );
	}

	SpecificDisp21();

	SetItemPosition() ;

	//IndexControl() ;
}

BEGIN_EVENTSINK_MAP(CShinSpecific2_31, CSpcBaseDlg)
	ON_EVENT(CShinSpecific2_31, IDC_ICSDIAG8CTRL1, 3, CShinSpecific2_31::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CShinSpecific2_31, IDC_ICSDIAG8CTRL1, 2, CShinSpecific2_31::EditOFFIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinSpecific2_31, IDC_ICSDIAG8CTRL1, 1, CShinSpecific2_31::EditONIcsdiag8ctrl1, VTS_I2)
END_EVENTSINK_MAP()


void CShinSpecific2_31::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	char	VK_FLG;
	short	wTerm;

	if( nChar == 0 ){
		return;
	}
	if( !m_initfg ){
		return;
	}

	// Shift+TabはTab、TabはEnterに変換
	VK_FLG = 0;
	VK_FLG = (char)GetKeyState( VK_SHIFT );
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
	

	int TableNo ;
	int ColmNo ;
	//このインデックスのTblhdl位置を取得し、TableNoに代入
	if( ( TableNo = GetTableNo( index ) ) == ERR ){
		return;
	}
	m_TableNo = TableNo;
	ColmNo = GetColmNo( index );

	UINT	flg = 0;


	switch( wTerm ){
		case VK_RETURN:
		case VK_RIGHT:
		case VK_F3:			// 修正No.168704 add
			flg = REPLY_NEXT;
			break;
		case VK_TAB:
		case VK_LEFT:
		case VK_F2:
			flg = REPLY_BACK;
			break;
		//上下キー対応
		case VK_DOWN:
			index = index + 8;
			if( m_TableNo && (((m_TableNo+1)%7)==0) ){
				index += DIAG_INP_STIDX;
			}	
			break;
		case VK_UP:
			index = index - 8;
			if( m_TableNo && ( ( m_TableNo % 7 ) == 0 ) ){
				index -= DIAG_INP_STIDX;
			}
			break;
		case VK_DELETE :
			if ( ColmNo == 0 ) {
				LineDelete( TableNo );
			}
			else {
				diag_clear ( IDC_ICSDIAG8CTRL1, index, TRUE, CIcsdiagctrl );
			}
			break;
		case VK_INSERT :
			LineInsert( TableNo );
			break;
	}
	if( flg & REPLY_NEXT ){
		diag_setnext( IDC_ICSDIAG8CTRL1, CIcsdiagctrl );
	}
	else if( flg & REPLY_BACK ){
		diag_setprev( IDC_ICSDIAG8CTRL1, CIcsdiagctrl );
	}
	else{
		diag_setposition( IDC_ICSDIAG8CTRL1, index, CIcsdiagctrl );
	}
}


//カーソルの位置をセット
//IsInEnd：処理終了中かどうか
void CShinSpecific2_31::SetItemPosition(BOOL IsInEnd/*FALSE*/)
{
	int index ;

	//前回のカーソルの位置を復元
	if ( m_curwnd == IDC_ICSDIAG8CTRL1 ) {
		if ( (index = m_Specific211.GetPosition() ) != -1 ) {
			if ( IsInEnd ) {
				// 画面上に完全に矩形領域がある時のみフォーカスを再セット
				RECT	rc211, rc21, rc210;
				m_Specific21.GetRect( 1, (LPUNKNOWN)&rc21 );
				m_Specific211.GetRect( index, (LPUNKNOWN)&rc211 );
				m_Specific210.GetRect( 1, (LPUNKNOWN)&rc210 );

				if( ((rc21.bottom<rc211.top)&&(rc211.bottom<rc210.top)) ){
					m_Specific211.SetPosition(index);
				}
				m_Specific211.SetFocus();
			}
			else{
				m_Specific211.SetPosition(index);
				m_Specific211.SetFocus() ;
			}

			return ;
		}
	}

	this->SetFocus() ;

	m_Specific210.SetFocus() ;
	m_Specific211.SetFocus() ;

	//非連動のとき、63番（内容の1行目）にセット
	if ( (*m_pSnHeadData)->Sn_Sign4 & 0x80 ) {
		m_Specific211.SetPosition( ID201_1ITEM ) ;
	}
	else{
	}

	//スクロール再描画
	SetRedrawScroll(0) ;
}

//入力の可不可
void CShinSpecific2_31::IndexControl()
{
	int Cnt = 0 ;

	while ( 1 ) {
		if ( Specific21_31[Cnt].Index == 0 ) {
			break ;
		}

		int Flg = 0 ;
		//入力ロック中のとき
		if ( (*m_pSnHeadData)->Sn_Sign4 & 0x01 ) {
			Flg = Specific21_31[Cnt].AllUnConnect ;
		}
		else{
			//入力ロック中でない　かつ　非連動
			if ((*m_pSnHeadData)->Sn_Sign4 & 0x80 ){
				Flg = Specific21_31[Cnt].UnConnect ;
			}
			//入力ロック中でない　かつ　連動
			else{
				Flg = Specific21_31[Cnt].Connect ;
			}
		}
		//入力の可不可をセット
		ATRIB_MOD( IDC_ICSDIAG8CTRL1, Specific21_31[Cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, Specific21_31[Cnt].Connect );
		Cnt ++ ;
	}

	SetFocusRange() ;
	m_Specific211.EnableDelete(0) ;

	return ;
}

//入力可能な範囲を調べる
void CShinSpecific2_31::SetFocusRange()
{
	DIAGRAM_ATTRIBUTE DA ;

	//上端を調べる
	for ( int idx = 0 ; idx < FCS_SPECIFIC21CNT ; idx ++ ) {
		m_Specific211.GetAttribute ( FCS_Specific21_31[idx].IDNo , (LPUNKNOWN)&DA ) ;
		if ( DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE ) {
			m_FcsUpEnd = FCS_Specific21_31[idx].IDNo ;
			break ;
		}
	}
	//左端を調べる
	for ( int idx = 0 ; idx < FCS_SPECIFIC21CNT ; idx ++ ) {
		m_Specific211.GetAttribute ( FCS_Specific21_31_LR[idx].IDNo , (LPUNKNOWN)&DA ) ;
		if ( DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE ) {
			m_FcsLeftEnd = FCS_Specific21_31_LR[idx].IDNo ;
			break ;
		}
	}
	//下端を調べる
	for ( int idx = FCS_SPECIFIC21CNT - 1 ; idx > -1 ; idx -- ) {
		m_Specific211.GetAttribute ( FCS_Specific21_31[idx].IDNo , (LPUNKNOWN)&DA ) ;
		if ( DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE ) {
			m_FcsDownEnd = FCS_Specific21_31[idx].IDNo ;
			break ;
		}
	}
	//右端を調べる
	for ( int idx = FCS_SPECIFIC21CNT -1 ; idx > -1 ; idx -- ) {
		m_Specific211.GetAttribute ( FCS_Specific21_31_LR[idx].IDNo , (LPUNKNOWN)&DA ) ;
		if ( DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE ) {
			m_FcsRightEnd = FCS_Specific21_31_LR[idx].IDNo ;
			break ;
		}
	}
}


//-----------------------------------------------------------------------------
// Tblhdl 上のレコード位置を取得
//-----------------------------------------------------------------------------
// 引数	nowIdx	：	ダイアグラム上のインデックス
//-----------------------------------------------------------------------------
// 返送値		：	Tblhdl 上のレコード位置
//-----------------------------------------------------------------------------
int CShinSpecific2_31::GetTableNo( int nowindex ) 
{

	// 該当ページ番号の取得
	int pageno = (nowindex / NUM_PER_PAGE);
	// 該当ページ先頭テーブル番号
	int stIdxInTbl = (pageno * TBL_REC_NUM_PER_PAGE);
	// 該当ページ先頭(ダイアグラム上)
	int stInNowPg = (pageno * NUM_PER_PAGE) + DIAG_INP_STIDX;

	int dif = nowindex - stInNowPg;
	int addedIdx = ( dif / CLMNUM_PER_ROW );


	return stIdxInTbl + addedIdx;
}

//-----------------------------------------------------------------------------
// カラム番号の取得
//-----------------------------------------------------------------------------
// 引数	nowIdx	：	ダイアグラム上のインデックス
//-----------------------------------------------------------------------------
// 返送値		：	カラムインデックス
//-----------------------------------------------------------------------------
int CShinSpecific2_31::GetColmNo( int nowIdx ) 
{
	int page = nowIdx / NUM_PER_PAGE;
	int dif  = nowIdx - ((page*NUM_PER_PAGE) + DIAG_INP_STIDX);
	int clm  = dif % CLMNUM_PER_ROW;

	return clm;
}

//----------------------------------------------
// 画面表示
//----------------------------------------------
void CShinSpecific2_31::SpecificDisp21()
{
	TSREC			TSrec;
	int				max = 0, rno = 0, index = 0, indexEx = 0, page = 0;
	char			sum[MONY_BUF_SIZE] = {0};
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	max = 210;
	index = 1;
	indexEx = DIAG_INP_STIDX;
	page = 0;

	for( rno = 0; rno != max; rno++ ){

		
		char	buf[128];
		sprintf_s( buf, sizeof( buf ), _T("%d"), rno+1 );
		DiagData.data_disp = buf;
		diag_setdata( IDC_ICSDIAG8CTRL1, index, (struct IUnknown*)&DiagData, CIcsdiagctrl );
		

		DIAGRAM_ATTRIBUTE	DA;
		diag_getattr( IDC_ICSDIAG8CTRL1, indexEx, (struct IUnknown*)&DA, CIcsdiagctrl );
		DA.attr_editattr |= DIAG_EATTR_ZENHAN;
		diag_setattr( IDC_ICSDIAG8CTRL1, indexEx, (struct IUnknown*)&DA, TRUE, CIcsdiagctrl );

		if( rno < (*m_pSpcListData)->TsTbl.tp_rnum ){
			if( (*m_pSpcListData)->m_TblHdl.th_rget( &TSrec, &(*m_pSpcListData)->TsTbl, rno ) ){
				return;
			}
			
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+0, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+1, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+2, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+3, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+4, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+5, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+6, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+7, TRUE, CIcsdiagctrl );

			// 内容
			DiagData.data_edit = TSrec.TsName;
			diag_setdata( IDC_ICSDIAG8CTRL1, indexEx, (struct IUnknown*)&DiagData, CIcsdiagctrl );

			// 資産の譲渡等の対価以外の収入
			memmove( DiagData.data_val, TSrec.TsJsyu, sizeof(DiagData.data_val) );
			diag_setdata( IDC_ICSDIAG8CTRL1, indexEx+1, (struct IUnknown*)&DiagData, CIcsdiagctrl );

			// 左のうち特定収入
			m_pArith->l_add( sum, TSrec.TsDtl[0].TsGsyu, TSrec.TsDtl[1].TsGsyu );
			m_pArith->l_add( sum, sum, TSrec.TsDtl[2].TsGsyu );
			m_pArith->l_add( sum, sum, TSrec.TsDtl[3].TsGsyu );
			memmove( DiagData.data_val, sum, sizeof(DiagData.data_val) );
			diag_setdata( IDC_ICSDIAG8CTRL1, indexEx+2, (struct IUnknown*)&DiagData, CIcsdiagctrl );

			// 課税仕入れ等に係る特定収入
			memmove( DiagData.data_val, TSrec.TsDtl[0].TsTsyu, sizeof(DiagData.data_val) );			// うち税率4%
			diag_setdata( IDC_ICSDIAG8CTRL1, indexEx+3, (struct IUnknown*)&DiagData, CIcsdiagctrl );

			memmove( DiagData.data_val, TSrec.TsDtl[1].TsTsyu, sizeof(DiagData.data_val) );			// うち税率6.3%
			diag_setdata( IDC_ICSDIAG8CTRL1, indexEx+4, (struct IUnknown*)&DiagData, CIcsdiagctrl );

			memmove( DiagData.data_val, TSrec.TsDtl[2].TsTsyu, sizeof(DiagData.data_val) );			// うち税率6.24%
			diag_setdata( IDC_ICSDIAG8CTRL1, indexEx+5, (struct IUnknown*)&DiagData, CIcsdiagctrl );

			memmove( DiagData.data_val, TSrec.TsDtl[3].TsTsyu, sizeof(DiagData.data_val) );			// うち税率7.8%
			diag_setdata( IDC_ICSDIAG8CTRL1, indexEx+6, (struct IUnknown*)&DiagData, CIcsdiagctrl );


			// 不特定収入
			memset( sum, '\0', sizeof(sum) );
			m_pArith->l_add( sum, TSrec.TsDtl[0].TsFsyu, TSrec.TsDtl[1].TsFsyu );
			m_pArith->l_add( sum, sum, TSrec.TsDtl[2].TsFsyu );
			m_pArith->l_add( sum, sum, TSrec.TsDtl[3].TsFsyu );
			memmove( DiagData.data_val, sum, sizeof(DiagData.data_val) );
			diag_setdata( IDC_ICSDIAG8CTRL1, indexEx+7, (struct IUnknown*)&DiagData, CIcsdiagctrl );
		}
		else
		{
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+0, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+1, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+2, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+3, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+4, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+5, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+6, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+7, TRUE, CIcsdiagctrl );
		}


		// 確定時は全入力不可
		switch( (*m_pSnHeadData)->Sn_Sign4 & 0x81 ){
			case 0x01:	// ロック
			case 0x81:	// 非連動でロック
				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+0, DIAG_MDFY_READONLY, CIcsdiagctrl );
				if( (*m_pSnHeadData)->Sn_Sign4 & 0x80 ){
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+0, 2 );
				}
				else{
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+0, 1 );
				}

				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+1, DIAG_MDFY_READONLY, CIcsdiagctrl );
				if( (*m_pSnHeadData)->Sn_Sign4 & 0x80 ){
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+1, 2 );
				}
				else{
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+1, 1 );
				}

				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+2, DIAG_MDFY_READONLY, CIcsdiagctrl );
				if( (*m_pSnHeadData)->Sn_Sign4 & 0x80 ){
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+2, 2 );
				}
				else{
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+2, 1 );
				}

				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+3, DIAG_MDFY_READONLY, CIcsdiagctrl );
				if( (*m_pSnHeadData)->Sn_Sign4 & 0x80 ){
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+3, 2 );
				}
				else{
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+3, 1 );
				}

				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+4, DIAG_MDFY_READONLY, CIcsdiagctrl );
				if( (*m_pSnHeadData)->Sn_Sign4 & 0x80 ){
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+4, 2 );
				}
				else{
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+4, 1 );
				}

				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+5, DIAG_MDFY_READONLY, CIcsdiagctrl );
				if( (*m_pSnHeadData)->Sn_Sign4 & 0x80 ){
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+5, 2 );
				}
				else{
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+5, 1 );
				}

				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+6, DIAG_MDFY_READONLY, CIcsdiagctrl );
				if( (*m_pSnHeadData)->Sn_Sign4 & 0x80 ){
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+6, 2 );
				}
				else{
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+6, 1 );
				}

				Color_Change( IDC_ICSDIAG8CTRL1, indexEx+7, 1 );
				break;
			case 0x00:	// 連動
				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+0, DIAG_MDFY_READONLY, CIcsdiagctrl );
				Color_Change( IDC_ICSDIAG8CTRL1, indexEx+0, 1 );

				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+1, DIAG_MDFY_READONLY, CIcsdiagctrl );
				Color_Change( IDC_ICSDIAG8CTRL1, indexEx+1, 1 );

				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+2, DIAG_MDFY_READONLY, CIcsdiagctrl );
				Color_Change( IDC_ICSDIAG8CTRL1, indexEx+2, 1 );

				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+3, DIAG_MDFY_READONLY, CIcsdiagctrl );
				Color_Change( IDC_ICSDIAG8CTRL1, indexEx+3, 1 );

				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+4, DIAG_MDFY_READONLY, CIcsdiagctrl );
				Color_Change( IDC_ICSDIAG8CTRL1, indexEx+4, 1 );

				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+5, DIAG_MDFY_READONLY, CIcsdiagctrl );
				Color_Change( IDC_ICSDIAG8CTRL1, indexEx+5, 1 );

				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+6, DIAG_MDFY_READONLY, CIcsdiagctrl );
				Color_Change( IDC_ICSDIAG8CTRL1, indexEx+6, 1 );

				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+7, DIAG_MDFY_READONLY, CIcsdiagctrl );
				Color_Change( IDC_ICSDIAG8CTRL1, indexEx+7, 1 );
				break;
			case 0x80:	// 非連動
				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+0, DIAG_MDFY_EDIT, CIcsdiagctrl);
				Color_Change( IDC_ICSDIAG8CTRL1, indexEx+0, 0 );

				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+1, DIAG_MDFY_EDIT, CIcsdiagctrl );
				Color_Change( IDC_ICSDIAG8CTRL1, indexEx+1, 0 );

				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+2, DIAG_MDFY_EDIT, CIcsdiagctrl );
				Color_Change( IDC_ICSDIAG8CTRL1, indexEx+2, 0 );
				
				if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
					diag_modify( IDC_ICSDIAG8CTRL1, indexEx+3, DIAG_MDFY_EDIT, CIcsdiagctrl );
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+3, 0 );

					diag_modify( IDC_ICSDIAG8CTRL1, indexEx+4, DIAG_MDFY_EDIT, CIcsdiagctrl );
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+4, 0 );
				}
				else {
					diag_modify( IDC_ICSDIAG8CTRL1, indexEx+3, DIAG_MDFY_READONLY, CIcsdiagctrl );
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+3, 1 );

					diag_modify( IDC_ICSDIAG8CTRL1, indexEx+4, DIAG_MDFY_READONLY, CIcsdiagctrl );
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+4, 1 );
				}

				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+5, DIAG_MDFY_EDIT, CIcsdiagctrl );
				Color_Change( IDC_ICSDIAG8CTRL1, indexEx+5, 0 );

				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+6, DIAG_MDFY_EDIT, CIcsdiagctrl );
				Color_Change( IDC_ICSDIAG8CTRL1, indexEx+6, 0 );
				break;
			default:
				break;
		}

		// index計算
		index += 9;
		indexEx += 8;
		if( rno && (((rno+1)%7)==0) ){
			page++;
			index = ( NUM_PER_PAGE * page ) + 1;
			indexEx = ( NUM_PER_PAGE * page ) + DIAG_INP_STIDX;
		}
	}


	// 再計算
	Specific21_Calq();
}

//--------------------------------------------
// 計算処理
//--------------------------------------------
void CShinSpecific2_31::Specific21_Calq()
{
	TSREC			TSrec;
	int				rno = 0;
	char			MONY0[MONY_BUF_SIZE] = {0}, MONY1_sum[MONY_BUF_SIZE] = {0}, MONY3_sum[MONY_BUF_SIZE] = {0};
	char			MONY1_4per[MONY_BUF_SIZE] = {0}, MONY1_63per[MONY_BUF_SIZE] = {0}, MONY1_624per[MONY_BUF_SIZE] = {0}, MONY1_78per[MONY_BUF_SIZE] = {0};
	char			MONY2_4per[MONY_BUF_SIZE] = {0}, MONY2_63per[MONY_BUF_SIZE] = {0}, MONY2_624per[MONY_BUF_SIZE] = {0}, MONY2_78per[MONY_BUF_SIZE] = {0};
	char			MONY3_4per[MONY_BUF_SIZE] = {0}, MONY3_63per[MONY_BUF_SIZE] = {0}, MONY3_624per[MONY_BUF_SIZE] = {0}, MONY3_78per[MONY_BUF_SIZE] = {0};
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;



	// 金額の取得
	rno = 0;
	while( 1 ){
		if( rno == (*m_pSpcListData)->TsTbl.tp_rnum ){
			break;
		}
		if( (*m_pSpcListData)->m_TblHdl.th_rget( &TSrec, &(*m_pSpcListData)->TsTbl, rno ) ){
			break;
		}

		// 資産の譲渡
		m_pArith->l_add( MONY0, MONY0, TSrec.TsJsyu );

		// 左のうち特定収入　各税率分の金額を取得
		m_pArith->l_add( MONY1_4per, MONY1_4per, TSrec.TsDtl[0].TsGsyu );
		m_pArith->l_add( MONY1_63per, MONY1_63per, TSrec.TsDtl[1].TsGsyu );
		m_pArith->l_add( MONY1_624per, MONY1_624per, TSrec.TsDtl[2].TsGsyu );
		m_pArith->l_add( MONY1_78per, MONY1_78per, TSrec.TsDtl[3].TsGsyu );

		// 課税仕入れ等に係る特定収入
		m_pArith->l_add( MONY2_4per, MONY2_4per, TSrec.TsDtl[0].TsTsyu );		// 4%
		m_pArith->l_add( MONY2_63per, MONY2_63per, TSrec.TsDtl[1].TsTsyu );		// 6.3%
		m_pArith->l_add( MONY2_624per, MONY2_624per, TSrec.TsDtl[2].TsTsyu );		// 6.24%
		m_pArith->l_add( MONY2_78per, MONY2_78per, TSrec.TsDtl[3].TsTsyu );			// 7.8%

		// 不特定収入　各税率分の金額を取得
		m_pArith->l_add( MONY3_4per, MONY3_4per, TSrec.TsDtl[0].TsFsyu );
		m_pArith->l_add( MONY3_63per, MONY3_63per, TSrec.TsDtl[1].TsFsyu );
		m_pArith->l_add( MONY3_624per, MONY3_624per, TSrec.TsDtl[2].TsFsyu );
		m_pArith->l_add( MONY3_78per, MONY3_78per, TSrec.TsDtl[3].TsFsyu );

		rno++;
	}




	// 画面表示

	// 資産の譲渡
	memmove( DiagData.data_val, MONY0, MONY_BUF_SIZE );
	diag_setdata( IDC_ICSDIAG8CTRL3, ID201_17KEI, (LPUNKNOWN)&DiagData, CIcsdiagctrl );

	// 左のうち特定収入　各税率分の金額を足す
	m_pArith->l_add( MONY1_sum, MONY1_4per, MONY1_63per );
	m_pArith->l_add( MONY1_sum, MONY1_sum, MONY1_624per );
	m_pArith->l_add( MONY1_sum, MONY1_sum, MONY1_78per );
	memmove( DiagData.data_val, MONY1_sum, MONY_BUF_SIZE );
	diag_setdata( IDC_ICSDIAG8CTRL3, ID201_17A, (LPUNKNOWN)&DiagData, CIcsdiagctrl );

	// 課税仕入れ等に係る特定収入
	memmove( DiagData.data_val, MONY2_4per, MONY_BUF_SIZE );	// 4%
	diag_setdata( IDC_ICSDIAG8CTRL3, ID201_17B, (LPUNKNOWN)&DiagData, CIcsdiagctrl );

	memmove( DiagData.data_val, MONY2_63per, MONY_BUF_SIZE );	// 6.3%
	diag_setdata( IDC_ICSDIAG8CTRL3, ID201_17C, (LPUNKNOWN)&DiagData, CIcsdiagctrl );

	memmove( DiagData.data_val, MONY2_624per, MONY_BUF_SIZE );	// 6.24%
	diag_setdata( IDC_ICSDIAG8CTRL3, ID201_17D, (LPUNKNOWN)&DiagData, CIcsdiagctrl );

	memmove( DiagData.data_val, MONY2_78per, MONY_BUF_SIZE );	// 7.8%
	diag_setdata( IDC_ICSDIAG8CTRL3, ID201_17E, (LPUNKNOWN)&DiagData, CIcsdiagctrl );

	// 不特定収入　各税率分の金額を足す
	m_pArith->l_add( MONY3_sum, MONY3_4per, MONY3_63per );
	m_pArith->l_add( MONY3_sum, MONY3_sum, MONY3_624per );
	m_pArith->l_add( MONY3_sum, MONY3_sum, MONY3_78per );
	memmove( DiagData.data_val, MONY3_sum, MONY_BUF_SIZE );
	diag_setdata( IDC_ICSDIAG8CTRL3, ID201_17F, (LPUNKNOWN)&DiagData, CIcsdiagctrl );




	// データクラスにセット

	// 資産の譲渡
	memset( (*m_pSpcListData)->stval.SpTsyu, '\0', sizeof(*m_pSpcListData)->stval.SpTsyu );
	memmove( (*m_pSpcListData)->stval.SpTsyu, MONY0, sizeof(*m_pSpcListData)->stval.SpTsyu );

	// 左のうち特定収入
	memset( (*m_pSpcListData)->stval.SpGsyu, '\0', sizeof(*m_pSpcListData)->stval.SpGsyu );
	memmove( (*m_pSpcListData)->stval.SpGsyu, MONY1_sum, sizeof(*m_pSpcListData)->stval.SpGsyu );

	// 課税仕入れ等に係る特定収入
	memset( (*m_pSpcListData)->stval.SpTsyu4, '\0', sizeof(*m_pSpcListData)->stval.SpTsyu4 );
	memmove( (*m_pSpcListData)->stval.SpTsyu4, MONY2_4per, sizeof(*m_pSpcListData)->stval.SpTsyu4 );		// 4%
	memset( (*m_pSpcListData)->stval.SpTsyu63, '\0', sizeof(*m_pSpcListData)->stval.SpTsyu63 );
	memmove( (*m_pSpcListData)->stval.SpTsyu63, MONY2_63per, sizeof(*m_pSpcListData)->stval.SpTsyu63 );		// 6.3%
	memset( (*m_pSpcListData)->stval.SpTsyu624, '\0', sizeof(*m_pSpcListData)->stval.SpTsyu624 );
	memmove( (*m_pSpcListData)->stval.SpTsyu624, MONY2_624per, sizeof(*m_pSpcListData)->stval.SpTsyu624 );		// 6.24%
	memset( (*m_pSpcListData)->stval.SpTsyu78, '\0', sizeof(*m_pSpcListData)->stval.SpTsyu78 );
	memmove( (*m_pSpcListData)->stval.SpTsyu78, MONY2_78per, sizeof(*m_pSpcListData)->stval.SpTsyu78 );			// 7.8%

	// 不特定収入
	memset( (*m_pSpcListData)->stval.SpFsyu, '\0', sizeof(*m_pSpcListData)->stval.SpFsyu );
	memmove( (*m_pSpcListData)->stval.SpFsyu, MONY3_sum, sizeof(*m_pSpcListData)->stval.SpFsyu );



	// 特定収入に係る課税仕入れ等の税額計算
	unsigned char	svSn_Sign2 = (*m_pSnHeadData)->Sn_Sign2;
	(*m_pSpcListData)->SPassOffCalqMain( *m_pSnHeadData );
	BOOL	IsChgDoSpc = FALSE;
	if( (svSn_Sign2&0x02) != ((*m_pSnHeadData)->Sn_Sign2&0x02) ){
		IsChgDoSpc = TRUE;
	}
	((CTaxationList31View*)m_pParent)->LinkHonpyoData(IsChgDoSpc);
	DispRegCount();


	m_Specific211.Refresh();
}

//----------------------------------------------
// 登録数表示
//----------------------------------------------
void CShinSpecific2_31::DispRegCount()
{
	char buf[128] = {0};
	sprintf_s( buf, _T("登録状況\n%3d/%3d"), (*m_pSpcListData)->TsTbl.tp_rnum, 210 );
	c_cls( IDC_ICSDISP8CTRL1, CIcsdisp8ctrl1 );
	puts( IDC_ICSDISP8CTRL1, buf, CIcsdisp8ctrl1 );
}

void CShinSpecific2_31::EditOFFIcsdiag8ctrl1(short index)
{
	TSREC				TSrec;
	int					TableNo, ColmNo, IndexAdd, PutData, NewLine;
	char				MONY[6]={0};
	BOOL				isMonyChg = FALSE;
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	if( !m_initfg ){
		return;
	}

	if( IsWindowVisible() == FALSE ){
		return;
	}

	ASSERT( m_pSpcListData );
	if( m_pSpcListData == NULL ){
		return;
	}

	diag_getdata( IDC_ICSDIAG8CTRL1, index, (struct IUnknown*)&DiagData, CIcsdiagctrl );

	if( ( TableNo = GetTableNo( index ) ) == ERR ){
		return;
	}

	m_TableNo = TableNo;
	ColmNo = GetColmNo( index );

	NewLine = 0;
	memset( &TSrec, '\0', sizeof(TSREC) );
	if( (*m_pSpcListData)->m_TblHdl.th_rget( &TSrec, &(*m_pSpcListData)->TsTbl, TableNo ) ){
		if( (*m_pSpcListData)->TsTbl.tp_rnum+1 > (*m_pSpcListData)->TsTbl.tp_rmax ){
			return;
		}
		memset( &TSrec, '\0', sizeof(TSREC) );
		TSrec.TsLine = TableNo+1;
		NewLine = 1;
	}

	IndexAdd = 0;
	PutData = 0;

	switch( ColmNo ){
		case 0:	// 名称
			sprintf_s( TSrec.TsName, sizeof(TSrec.TsName), _T("%s"), DiagData.data_edit.GetBuffer() );
			PutData = DiagData.data_edit.GetLength();
			break;
		case 1:	// 資産の譲渡
			PutData = m_pArith->l_cmp( DiagData.data_val, TSrec.TsJsyu );
			memmove( TSrec.TsJsyu, DiagData.data_val, sizeof(TSrec.TsJsyu) );
			if( PutData ){
				isMonyChg = TRUE;
			}
			break;
		case 2:	// 左のうち特定収入
			if( (*m_pSnHeadData)->SVmzsw == 1 ){	// 経過措置あり
				char	sum[MONY_BUF_SIZE];
				char	dif[MONY_BUF_SIZE];

				m_pArith->l_add( sum, TSrec.TsDtl[0].TsGsyu, TSrec.TsDtl[1].TsGsyu );
				m_pArith->l_add( sum, sum, TSrec.TsDtl[2].TsGsyu );
				m_pArith->l_add( sum, sum, TSrec.TsDtl[3].TsGsyu );

				m_pArith->l_sub( dif, DiagData.data_val, sum );
				PutData = m_pArith->l_cmp( DiagData.data_val, sum );
				m_pArith->l_add( TSrec.TsDtl[3].TsGsyu, TSrec.TsDtl[3].TsGsyu, dif );

			}
			else{	// 経過措置なし
				//PutData = m_pArith->l_cmp( DiagData.data_val, TSrec.TsDtl[0].TsGsyu );
				//memmove( TSrec.TsDtl[0].TsGsyu, DiagData.data_val, sizeof(TSrec.TsDtl[0].TsGsyu) );

				char	sum[MONY_BUF_SIZE];
				char	dif[MONY_BUF_SIZE];

				m_pArith->l_add( sum, TSrec.TsDtl[2].TsGsyu, TSrec.TsDtl[3].TsGsyu );
				m_pArith->l_sub( dif, DiagData.data_val, sum );
				PutData = m_pArith->l_cmp( DiagData.data_val, sum );
				m_pArith->l_add( TSrec.TsDtl[3].TsGsyu, TSrec.TsDtl[3].TsGsyu, dif );
			}
			IndexAdd = index + 5;
			if( PutData ){
				isMonyChg = TRUE;
			}
			break;
		case 3:	// 課税仕入れ等に係る特定収入　4%
			PutData = m_pArith->l_cmp( DiagData.data_val, TSrec.TsDtl[0].TsTsyu );
			memmove( TSrec.TsDtl[0].TsTsyu, DiagData.data_val, sizeof(TSrec.TsDtl[0].TsTsyu) );

			// 非課税に係るを算出
			memset( MONY, '\0', sizeof(MONY) );
			//非課税の計算を行う
			m_pArith->l_add( MONY, TSrec.TsDtl[0].TsSsyu, TSrec.TsDtl[0].TsKsyu );
			m_pArith->l_sub( TSrec.TsDtl[0].TsHsyu, TSrec.TsDtl[0].TsTsyu, MONY );

			IndexAdd = index + 4;
			if( PutData ){
				isMonyChg = TRUE;
			}
			break;
		case 4:	// 課税仕入れ等に係る特定収入　6.3%
			PutData = m_pArith->l_cmp( DiagData.data_val, TSrec.TsDtl[1].TsTsyu );
			memmove( TSrec.TsDtl[1].TsTsyu, DiagData.data_val, sizeof(TSrec.TsDtl[1].TsTsyu) );

			// 非課税に係るを算出
			memset( MONY, '\0', sizeof(MONY) );
			//非課税の計算を行う
			m_pArith->l_add( MONY, TSrec.TsDtl[1].TsSsyu, TSrec.TsDtl[1].TsKsyu );
			m_pArith->l_sub( TSrec.TsDtl[1].TsHsyu, TSrec.TsDtl[1].TsTsyu, MONY );

			IndexAdd = index + 3;
			if( PutData ){
				isMonyChg = TRUE;
			}
			break;
		case 5:	// 課税仕入れ等に係る特定収入　6.24%
			PutData = m_pArith->l_cmp( DiagData.data_val, TSrec.TsDtl[2].TsTsyu );
			memmove( TSrec.TsDtl[2].TsTsyu, DiagData.data_val, sizeof(TSrec.TsDtl[2].TsTsyu) );

			// 非課税に係るを算出
			memset( MONY, '\0', sizeof(MONY) );
			//非課税の計算を行う
			m_pArith->l_add( MONY, TSrec.TsDtl[2].TsSsyu, TSrec.TsDtl[2].TsKsyu );
			m_pArith->l_sub( TSrec.TsDtl[2].TsHsyu, TSrec.TsDtl[2].TsTsyu, MONY );

			IndexAdd = index + 2;
			if( PutData ){
				isMonyChg = TRUE;
			}
			break;
		case 6:	// 課税仕入れ等に係る特定収入　7.8%
			PutData = m_pArith->l_cmp( DiagData.data_val, TSrec.TsDtl[3].TsTsyu );
			memmove( TSrec.TsDtl[3].TsTsyu, DiagData.data_val, sizeof(TSrec.TsDtl[3].TsTsyu) );

			// 非課税に係るを算出
			memset( MONY, '\0', sizeof(MONY) );
			//非課税の計算を行う
			m_pArith->l_add( MONY, TSrec.TsDtl[3].TsSsyu, TSrec.TsDtl[3].TsKsyu );
			m_pArith->l_sub( TSrec.TsDtl[3].TsHsyu, TSrec.TsDtl[3].TsTsyu, MONY );

			IndexAdd = index + 1;
			if( PutData ){
				isMonyChg = TRUE;
			}
			break;
		default:
			break;
	}

	if( IndexAdd ){
		memset( MONY, '\0', sizeof(MONY) );
		if( (ColmNo == 2) || (ColmNo == 3) ){
			m_pArith->l_sub( MONY, TSrec.TsDtl[0].TsGsyu, TSrec.TsDtl[0].TsTsyu );
			memmove( TSrec.TsDtl[0].TsFsyu, MONY, sizeof(TSrec.TsDtl[0].TsFsyu) );
		}
		if( (ColmNo == 2) || (ColmNo == 4) ){
			m_pArith->l_sub( MONY, TSrec.TsDtl[1].TsGsyu, TSrec.TsDtl[1].TsTsyu );
			memmove( TSrec.TsDtl[1].TsFsyu, MONY, sizeof(TSrec.TsDtl[1].TsFsyu) );
		}
		if( (ColmNo == 2) || (ColmNo == 5) ){
			m_pArith->l_sub( MONY, TSrec.TsDtl[2].TsGsyu, TSrec.TsDtl[2].TsTsyu );
			memmove( TSrec.TsDtl[2].TsFsyu, MONY, sizeof(TSrec.TsDtl[2].TsFsyu) );
		}
		if( (ColmNo == 2) || (ColmNo == 6) ){
			m_pArith->l_sub( MONY, TSrec.TsDtl[3].TsGsyu, TSrec.TsDtl[3].TsTsyu );
			memmove( TSrec.TsDtl[3].TsFsyu, MONY, sizeof(TSrec.TsDtl[3].TsFsyu) );
		}

		char	sum[MONY_BUF_SIZE]={0};
		m_pArith->l_add( sum, TSrec.TsDtl[0].TsFsyu, TSrec.TsDtl[1].TsFsyu );
		m_pArith->l_add( sum, sum, TSrec.TsDtl[2].TsFsyu );
		m_pArith->l_add( sum, sum, TSrec.TsDtl[3].TsFsyu );
		memmove( DiagData.data_val, sum, sizeof(DiagData.data_val) );

		diag_setdata( IDC_ICSDIAG8CTRL1, IndexAdd, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	}

	if( (PutData == 0) && NewLine ){
		return;	// 入力データなし
	}

	if( (*m_pSpcListData)->m_TblHdl.th_acs( thWritex, &TSrec, &(*m_pSpcListData)->TsTbl ) ){
		ICSMessageBox( "テーブルに書き込み中に障害が発生しました。" );
		return;
	}

	if( isMonyChg ){
		Specific21_Calq();
	}
	else{
		DispRegCount();
	}

}


void CShinSpecific2_31::EditONIcsdiag8ctrl1(short index)
{
	m_curwnd = IDC_ICSDIAG8CTRL1;

	if( !m_initfg ){
		return;
	}

	int ClmNo = GetColmNo ( index );
	if ( ClmNo == 0 ) {
		GetDlgItem ( IDC_BUTTON1 )->SetWindowText ( _T( "行削除\n(Delete)" ) );
	}
	else {
		GetDlgItem ( IDC_BUTTON1 )->SetWindowText ( _T( "項目削除\n(Delete)" ) );
	}

	int line = GetTableNo( index );
	if( line < (*m_pSpcListData)->TsTbl.tp_rnum ){
		return;
	}
	int find = findindexEX2( line );
	if( find < 0 ){
		return;
	}

	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;
	int sw = 0;
	diag_getdata( IDC_ICSDIAG8CTRL1, find+0, (struct IUnknown*)&DiagData, CIcsdiagctrl );
	if( DiagData.data_edit.GetLength() ){
		sw |= 0x01;
	}
	diag_getdata( IDC_ICSDIAG8CTRL1, find+1, (struct IUnknown*)&DiagData, CIcsdiagctrl );
	if( m_pArith->l_test( DiagData.data_val ) ){
		sw |= 0x02;
	}
	diag_getdata( IDC_ICSDIAG8CTRL1, find+2, (struct IUnknown*)&DiagData, CIcsdiagctrl );
	if( m_pArith->l_test( DiagData.data_val ) ){
		sw |= 0x04;
	}
	diag_getdata( IDC_ICSDIAG8CTRL1, find+3, (struct IUnknown*)&DiagData, CIcsdiagctrl );
	if( m_pArith->l_test( DiagData.data_val ) ){
		sw |= 0x08;
	}
	diag_getdata( IDC_ICSDIAG8CTRL1, find+4, (struct IUnknown*)&DiagData, CIcsdiagctrl );
	if( m_pArith->l_test( DiagData.data_val ) ){
		sw |= 0x10;
	}
	diag_getdata( IDC_ICSDIAG8CTRL1, find+5, (struct IUnknown*)&DiagData, CIcsdiagctrl );
	if( m_pArith->l_test( DiagData.data_val ) ){
		sw |= 0x20;
	}
	diag_getdata( IDC_ICSDIAG8CTRL1, find+6, (struct IUnknown*)&DiagData, CIcsdiagctrl );
	if( m_pArith->l_test( DiagData.data_val ) ){
		sw |= 0x30;
	}
	diag_getdata( IDC_ICSDIAG8CTRL1, find+7, (struct IUnknown*)&DiagData, CIcsdiagctrl );
	if( m_pArith->l_test( DiagData.data_val ) ){
		sw |= 0x40;
	}



	if( !sw ){
		find = findindexEX2( (*m_pSpcListData)->TsTbl.tp_rnum + 1 );
		if( (find < 0) || (find == index) ){
			return;
		}
		diag_setposition( IDC_ICSDIAG8CTRL1, find, CIcsdiagctrl );
		return;
	}

	if( GetTableNo( index ) ==  ERR ){
		return;
	}
}

//-------------------------------------------------------
// テーブル登録削除
//-------------------------------------------------------
int CShinSpecific2_31::LineDelete( int rwno )
{
	TSREC	TSrec;
	int		PutNo;

	// 削除確認
	if( (*m_pSpcListData)->TsTbl.tp_rnum <= rwno ){
		return 0;
	}
	(*m_pSpcListData)->m_TblHdl.th_rget( &TSrec, &(*m_pSpcListData)->TsTbl, rwno );
	CString	msg;
	msg.Format( _T("内容「%s」を削除します。よろしいですか？"), TSrec.TsName );
	if( ICSMessageBox( msg, (MB_YESNO|MB_DEFBUTTON2), 0, 0, this ) == IDNO ){
		return 0;
	}

	if( (*m_pSpcListData)->TsTbl.tp_rnum ) {
		for( PutNo = rwno; PutNo+1 != (*m_pSpcListData)->TsTbl.tp_rnum; PutNo++ )	{
			if( (*m_pSpcListData)->m_TblHdl.th_rget( &TSrec, &(*m_pSpcListData)->TsTbl, PutNo+1 ) ){
				return ERR;
			}
			TSrec.TsLine = PutNo+1;
			if( (*m_pSpcListData)->m_TblHdl.th_rput( &TSrec, &(*m_pSpcListData)->TsTbl, PutNo ) ){
				return ERR;
			}
		}
		(*m_pSpcListData)->TsTbl.tp_rnum--;
	}

	// 再表示
	SpecificDisp21();

	// 最終行へ移動 
	int	i, j, k, l, m, n;
	// 先頭のカラム番号
	i = rwno;
	j = ( i / TBL_REC_NUM_PER_PAGE );
	k = (j * NUM_PER_PAGE) + DIAG_INP_STIDX;
	m_Specific211.SetPosition(k);
	// 先頭のテーブル番号
	l = ( j * TBL_REC_NUM_PER_PAGE );
	// 最終のテーブル番号
	m = i - l;
	// 最終のカラム番号
	n = ( m * CLMNUM_PER_ROW );
	// セットカラム
	CRect	rc;
	this->GetClientRect( &rc );
	this->InvalidateRect( &rc );
	m_Specific211.SetPosition(k+n);

	return 0;
}

//-------------------------------------------------------
// テーブル番号挿入
//-------------------------------------------------------
int CShinSpecific2_31::LineInsert( int rwno )
{
	TSREC		TSrec;
	int			PutNo;

	if( (*m_pSpcListData)->TsTbl.tp_rnum >= (*m_pSpcListData)->TsTbl.tp_rmax ){
		ICSMessageBox( "新規に登録できません。" );
		return FALSE;
	}

	PutNo = (*m_pSpcListData)->TsTbl.tp_rnum;
	(*m_pSpcListData)->TsTbl.tp_rnum++;
	for( ; rwno != PutNo; PutNo-- ){
		if( (*m_pSpcListData)->m_TblHdl.th_rget( &TSrec, &(*m_pSpcListData)->TsTbl, PutNo-1 ) ){
			return ERR;
		}
		TSrec.TsLine += 1;
		if( (*m_pSpcListData)->m_TblHdl.th_rput( &TSrec, &(*m_pSpcListData)->TsTbl, PutNo ) ){
			return ERR;
		}
	}

	memset( &TSrec, '\0', sizeof(TSREC) );
	TSrec.TsLine = rwno + 1;
	if( (*m_pSpcListData)->m_TblHdl.th_rput( &TSrec, &(*m_pSpcListData)->TsTbl, rwno ) ){
		return ERR;
	}

	SpecificDisp21();


	// 最終行へ移動 
	int	i, j, k, l, m, n;
	// 先頭のカラム番号
	i = rwno;
	j = ( i / TBL_REC_NUM_PER_PAGE );
	k = (j * NUM_PER_PAGE) + DIAG_INP_STIDX;
	m_Specific211.SetPosition(k);
	// 先頭のテーブル番号
	l = ( j * TBL_REC_NUM_PER_PAGE );
	// 最終のテーブル番号
	m = i - l;
	// 最終のカラム番号
	n = ( m * CLMNUM_PER_ROW );
	// セットカラム
	CRect	rc;
	this->GetClientRect( &rc );
	this->InvalidateRect( &rc );
	m_Specific211.SetPosition(k+n);


	return 0;
}


void CShinSpecific2_31::OnBnClickedButton1()
{
	int TableNo = 0;
	int ClmNo = 0;
	int index = m_Specific211.GetPosition();
	ClmNo = GetColmNo ( index );
	if ( ClmNo == 0 ) {
		if( ( TableNo = GetTableNo( index ) ) == ERR ){
			return;
		}

		m_TableNo = TableNo;
		LineDelete( TableNo );
	}
	else {
		diag_clear ( IDC_ICSDIAG8CTRL1, index, TRUE, CIcsdiagctrl );
		diag_setposition ( IDC_ICSDIAG8CTRL1, index, CIcsdiagctrl );
	}
}


void CShinSpecific2_31::OnBnClickedButton2()
{
	int TableNo = 0;
	int index = m_Specific211.GetPosition();
	if( ( TableNo = GetTableNo( index ) ) == ERR ){
		return;
	}

	m_TableNo = TableNo;
	LineInsert( TableNo );
}

//-----------------------------------------------------------------------------
// 該当行の内容列のダイアグラムインデックスを取得
//-----------------------------------------------------------------------------
// 引数	line	：	行番号（CTblhdl上）
//-----------------------------------------------------------------------------
// 返送値		：	ダイアグラムインデックス
//-----------------------------------------------------------------------------
int CShinSpecific2_31::findindexEX2( int line )
{
	int find_line = max(line-1, 0);
	int pgno = line / TBL_REC_NUM_PER_PAGE;
	if( line && !(line%TBL_REC_NUM_PER_PAGE) ){
		pgno--;
	}
	if( pgno < 0 ){
		return -1;
	}

	return ((find_line%TBL_REC_NUM_PER_PAGE)*CLMNUM_PER_ROW) + (pgno*NUM_PER_PAGE) + DIAG_INP_STIDX;
}

void CShinSpecific2_31::OnButtonF2()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	diag_setprev( IDC_ICSDIAG8CTRL1, CIcsdiagctrl );
}

void CShinSpecific2_31::OnButtonF3()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	diag_setnext( IDC_ICSDIAG8CTRL1, CIcsdiagctrl );
}

void CShinSpecific2_31::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
//	CSpcBaseDlg::OnOK();
}

void CShinSpecific2_31::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
//	CSpcBaseDlg::OnCancel();
}

