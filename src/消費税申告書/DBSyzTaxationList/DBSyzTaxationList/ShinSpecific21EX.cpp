// ShinSpecific21.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinSpecific21EX.h"
#include "ShinSpecific21EXIdx.h"//yoshida150311
#include "TaxationListView.h"

//== 定数 ==
const	int	BottomIndex				=	0;	// No3
const	int	StartIndex				=	35;	// No2

const	int	NUM_PER_PAGE			=	91;	// 1ページ毎のデータ数
const	int	DIAG_INP_STIDX			=	49;	// 1ページの入力開始インデックス
const	int	TBL_REC_NUM_PER_PAGE	=	7;	// 1ページ毎のテーブルレコード数（CTblhdl上）
const	int	CLMNUM_PER_ROW			=	6;	// 1行毎の列数

// CShinSpecific21EX ダイアログ

IMPLEMENT_DYNAMIC(CShinSpecific21EX, CSpcBaseDlg)

CShinSpecific21EX::CShinSpecific21EX(CWnd* pParent /*=NULL*/)
	: CSpcBaseDlg(CShinSpecific21EX::IDD, pParent)
	, m_Spage(0)
{

	//------>yoshida150310
	m_curwnd = -1;

	m_FcsUpEnd		= -1;
	m_FcsLeftEnd	= -1;
	m_FcsDownEnd	= -1;
	m_FcsRightEnd	= -1;
	//<-------------------

}

CShinSpecific21EX::~CShinSpecific21EX()
{
}

void CShinSpecific21EX::DoDataExchange(CDataExchange* pDX)
{
	CSpcBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL2, m_Specific21);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL3, m_Specific210);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Specific211);
	DDX_Control(pDX, IDC_ICSDISP8CTRL1, m_ICSDisp1);
}


BEGIN_MESSAGE_MAP(CShinSpecific21EX, CSpcBaseDlg)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON1, &CShinSpecific21EX::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CShinSpecific21EX::OnBnClickedButton2)
	ON_COMMAND(ID_BUTTON_F2, &CShinSpecific21EX::OnButtonF2)
	ON_COMMAND(ID_BUTTON_F3, &CShinSpecific21EX::OnButtonF3)
END_MESSAGE_MAP()


// CShinSpecific21EX メッセージ ハンドラ

BOOL CShinSpecific21EX::OnInitDialog()
{
	CSpcBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	// 初期設定
	SpecificInit21();

	m_Specific211.EnableDelete(0);

	m_initfg = 1;

	CSpcBaseDlg::OnInitDialogEX();

//	m_Specific211.EnableDelete(0);//yoshida150311

	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

BOOL CShinSpecific21EX::PreTranslateMessage(MSG* pMsg)
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
	if ( pMsg->message == WM_KEYDOWN ){

		if( GetCombineKeyDown(VK_SHIFT) && (VK_F1<=pMsg->wParam) && (pMsg->wParam<=VK_F12) ){
			return TRUE;
		}

		if( (pMsg->wParam==VK_NEXT) || (pMsg->wParam==VK_PRIOR) ){
			SCROLLINFO ScrollInfo;
			int	c = GetScrollPos( SB_VERT );
			if( c == 0 ){
				return 1;
			}
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

		if( pMsg->wParam == VK_F2 ){
			OnButtonF2();
			return 1;
		}
		if( pMsg->wParam == VK_F3 ){
			OnButtonF3();
			return 1;
		}
	}

	return CSpcBaseDlg::PreTranslateMessage(pMsg);
	*/

}

void CShinSpecific21EX::OnSize(UINT nType, int cx, int cy)
{
	CSpcBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if ( m_initfg )	{
#ifdef	_CLOSE
		RECT	a, b;

		GetWindowRect( &a );

		m_Specific210.GetRect( BottomIndex, (LPUNKNOWN)&b );
		SCROLLINFO ScrollInfo;
		if ( GetScrollInfo( SB_VERT, &ScrollInfo ) == FALSE )
			ScrollInfo.nPos = 0;
		ScrollInfo.nMin  = 0;
		ScrollInfo.nMax  = (b.bottom + 20) - a.top + ScrollInfo.nPos;
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

HBRUSH CShinSpecific21EX::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSpcBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
	if( nCtlColor == CTLCOLOR_DLG ){
		hbr = Ibrsh_ShinSpecific;
	}

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

void CShinSpecific21EX::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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
void CShinSpecific21EX::sykscroll( int mpos )
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

//----->yoshida150311
/*
//-----------------------------------------------------------------------------
// セットポジション
//-----------------------------------------------------------------------------
void CShinSpecific21EX::SetItemPosition( int IndexNo )
{
	int	Index;
	if( IndexNo ){
		Index = IndexNo;
	}
	else{
		Index = StartIndex;
	}

	m_Specific211.SetFocus();
	// 確定時は全入力不可
	if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
		return;
	}

	m_Specific211.SetPosition( Index );

	m_Specific211.Refresh();
	sykscroll( 0 );
}
*/
//<--------------------------

/*
//-----------------------------------------------------------------------------
// 入力項目　色変え
//-----------------------------------------------------------------------------
void CShinSpecific21EX::Color_Change( unsigned short id, short index, int sign )
{
	DIAGRAM_ATTRIBUTE	DA;

	diag_getattr( id, index, &DA, CIcsdiagctrl );
	switch( sign ) {
		case 1 :	DA.attr_bcolor = BC_WHITE;		break;		// 入力不可	（白色）
		case 2 : 	DA.attr_bcolor = BC_GRAY;		break;		// ロック	（灰色）
		case 3 : 	DA.attr_bcolor = BC_BLUE;		break;		// 特殊色①	（青色）
		case 4 : 	DA.attr_bcolor = BC_GREEN;		break;		// 特殊色②	（緑色）	
		case 5 : 	DA.attr_bcolor = BC_GREEN_L;	break;		// 特殊色③	（薄緑色）	 
		default: 	DA.attr_bcolor = BC_CREAM;		break;		// 実額入力	（クリーム色） 
	}
	diag_setattr( id, index, &DA, FALSE, CIcsdiagctrl );
}*/

//-----------------------------------------------------------------------------
// 初期設定
//-----------------------------------------------------------------------------
void CShinSpecific21EX::SpecificInit21( void )
{
	// 倍長演算指定
	m_pArith->l_defn( 0x16 );
	// 最大ページ数
	m_Spage = 30;
	m_Specific211.SetScPageCnt( (short)m_Spage );


//	IndexControl();//yoshida150323

	SpecificDisp21();

//2017.03.01 INSERT START
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;
	if( ((CTaxationListView*)m_pParent)->IsKeikaType_Keisansyo() == TRUE ){
		DiagData.data_disp.Format("計算表5(1)④,(3)④へ");
	}else{
		DiagData.data_disp.Format("計算表5(1)②,(3)②へ");
	}
	diag_setdata( IDC_ICSDIAG8CTRL3, 5, (struct IUnknown *)&DiagData, CIcsdiagctrl );
//2017.03.01 INSERT END

	//yoshida150311
	// 確定時は全入力不可
	if( ((*m_pSnHeadData)->Sn_Sign4&0x01) || !((*m_pSnHeadData)->Sn_Sign4&0x80) ){
		GetDlgItem( IDC_BUTTON1 )->EnableWindow( FALSE );
		GetDlgItem( IDC_BUTTON2 )->EnableWindow( FALSE );
	}

	SetItemPosition();//yoshida150311
//	SetItemPosition(0);
}

//-----------------------------------------------------------------------------
// 表示
//-----------------------------------------------------------------------------
void CShinSpecific21EX::SpecificDisp21( void )
{
	TSREC			TSrec;
	int				rno, max, index, indexEx, page;
	char			sum[MONY_BUF_SIZE]={0};
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	max = 210;
	index = 1;
	indexEx = DIAG_INP_STIDX;
	page = 0;
	for( rno=0; rno!=max; rno++ ){
		char buf[128];
		sprintf_s( buf, sizeof( buf ), _T("%d"), rno+1 );
		DiagData.data_disp =  buf;
		diag_setdata( IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );

		//---->yoshida150325
		DIAGRAM_ATTRIBUTE	DA;
		m_Specific211.GetAttribute( indexEx, (LPUNKNOWN)&DA );
		DA.attr_editattr |= DIAG_EATTR_ZENHAN;
		m_Specific211.SetAttribute( indexEx, (LPUNKNOWN)&DA, TRUE );
		//<-----------------

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
			// 科目名称
			DiagData.data_edit =  TSrec.TsName;
			diag_setdata( IDC_ICSDIAG8CTRL1, indexEx, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			// 資産の譲渡等の対価以外の収入
//			m_pArith->l_clear( DiagData.data_val );
			memmove( DiagData.data_val, TSrec.TsJsyu, sizeof(DiagData.data_val) );
			diag_setdata( IDC_ICSDIAG8CTRL1, indexEx+1, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			// 特定収入合計
			m_pArith->l_add( sum, TSrec.TsDtl[0].TsGsyu, TSrec.TsDtl[1].TsGsyu );
			memmove( DiagData.data_val, sum, sizeof(DiagData.data_val) );
			diag_setdata( IDC_ICSDIAG8CTRL1, indexEx+2, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			// 課税仕入れに係る特定収入
			if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == TRUE ){
//-- '15.04.25 --
//				// 特定収入（課税に係る＋非課税に係る＋共通に係る）★計算表の特殊合計用（個別対応専用）
//				memmove( DiagData.data_val, TSrec.TsDtl[0].TsSsyu, 6 );
//				diag_setdata( IDC_ICSDIAG8CTRL1, indexEx+3, (struct IUnknown *)&DiagData, CIcsdiagctrl );
//				// 個別対応時は課税仕入れに係る特定収入の入力をさせない。
////				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+3, DIAG_MDFY_READONLY, CIcsdiagctrl );
////				Color_Change( IDC_ICSDIAG8CTRL1, indexEx+3, 1 );
//
//				// 特定収入（課税に係る＋非課税に係る＋共通に係る）★計算表の特殊合計用（個別対応専用）
//				memmove( DiagData.data_val, TSrec.TsDtl[1].TsSsyu, 6 );
//				diag_setdata( IDC_ICSDIAG8CTRL1, indexEx+4, (struct IUnknown *)&DiagData, CIcsdiagctrl );
//				// 個別対応時は課税仕入れに係る特定収入の入力をさせない。
////				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+4, DIAG_MDFY_READONLY, CIcsdiagctrl );
////				Color_Change( IDC_ICSDIAG8CTRL1, indexEx+4, 1 );
//---------------
				// 特定収入（5%)
				memmove( DiagData.data_val, TSrec.TsDtl[0].TsTsyu, 6 );
				diag_setdata( IDC_ICSDIAG8CTRL1, indexEx+3, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				// 特定収入（8%)
				memmove( DiagData.data_val, TSrec.TsDtl[1].TsTsyu, 6 );
				diag_setdata( IDC_ICSDIAG8CTRL1, indexEx+4, (struct IUnknown *)&DiagData, CIcsdiagctrl );
//---------------
			}
			else{
				// 比例配分
				memmove( DiagData.data_val, TSrec.TsDtl[0].TsTsyu, 6 );
				diag_setdata( IDC_ICSDIAG8CTRL1, indexEx+3, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				// 比例配分
				memmove( DiagData.data_val, TSrec.TsDtl[1].TsTsyu, 6 );
				diag_setdata( IDC_ICSDIAG8CTRL1, indexEx+4, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			}
			// 不特定収入
			m_pArith->l_add( sum, TSrec.TsDtl[0].TsFsyu, TSrec.TsDtl[1].TsFsyu );
			memmove( DiagData.data_val, sum, sizeof(DiagData.data_val) );
			diag_setdata( IDC_ICSDIAG8CTRL1, indexEx+5, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		}
		else{
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+0, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+1, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+2, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+3, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+4, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+5, TRUE, CIcsdiagctrl );
			if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == TRUE ){
				// 個別対応時は課税仕入れに係る特定収入の入力をさせない。
//				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+2, DIAG_MDFY_READONLY, CIcsdiagctrl );
			}
		}

		// 確定時は全入力不可
		switch( (*m_pSnHeadData)->Sn_Sign4&0x81 ){
			case 0x01 :	// ロック
			case 0x81 :	// 非連動でロック
				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+0, DIAG_MDFY_READONLY, CIcsdiagctrl );
//-- '15.04.30 --
//				Color_Change( IDC_ICSDIAG8CTRL1, indexEx+0, 2 );
//---------------
				if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+0, 2 );
				}
				else{
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+0, 1 );
				}
//---------------
				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+1, DIAG_MDFY_READONLY, CIcsdiagctrl );
//-- '15.04.30 --
//				Color_Change( IDC_ICSDIAG8CTRL1, indexEx+1, 2 );
//---------------
				if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+1, 2 );
				}
				else{
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+1, 1 );
				}
//---------------
				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+2, DIAG_MDFY_READONLY, CIcsdiagctrl );
//-- '15.03.23 --
//				if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == TRUE ){
//					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+2, 1 );
//				}
//				else{
//					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+2, 2 );
//				}
//---------------
//-- '15.04.30 --
//				Color_Change( IDC_ICSDIAG8CTRL1, indexEx+2, 2 );
//---------------
				if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+2, 2 );
				}
				else{
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+2, 1 );
				}
//---------------
//-- '15.04.30 --
//				if( (*m_pSnHeadData)->SVmzsw == 1 ){
//					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+3, 2 );
//					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+4, 2 );
//				}
//				else{
//					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+3, 1 );
//					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+4, 2 );
//				}
//---------------
				if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
					if( (*m_pSnHeadData)->SVmzsw == 1 ){
						Color_Change( IDC_ICSDIAG8CTRL1, indexEx+3, 2 );
						Color_Change( IDC_ICSDIAG8CTRL1, indexEx+4, 2 );
					}
					else{
						Color_Change( IDC_ICSDIAG8CTRL1, indexEx+3, 1 );
						Color_Change( IDC_ICSDIAG8CTRL1, indexEx+4, 2 );
					}
				}
				else{
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+3, 1 );
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+4, 1 );
				}
//---------------
//--> '15.04.30 INS START
				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+3, DIAG_MDFY_READONLY, CIcsdiagctrl );
				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+4, DIAG_MDFY_READONLY, CIcsdiagctrl );
//<-- '15.04.30 INS END
				Color_Change( IDC_ICSDIAG8CTRL1, indexEx+5, 1 );
//---------------
				break;
			case 0x00 :	// 連動
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
				break;
			case 0x80 :	// 非連動
				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+0, DIAG_MDFY_EDIT, CIcsdiagctrl );
				Color_Change( IDC_ICSDIAG8CTRL1, indexEx+0, 0 );
				if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
					diag_modify( IDC_ICSDIAG8CTRL1, indexEx+1, DIAG_MDFY_EDIT, CIcsdiagctrl );
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+1, 0 );
					diag_modify( IDC_ICSDIAG8CTRL1, indexEx+2, DIAG_MDFY_EDIT, CIcsdiagctrl );
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+2, 0 );

//-- '15.04.17 --
//					//yoshida150323
//					diag_modify( IDC_ICSDIAG8CTRL1, indexEx+3, DIAG_MDFY_EDIT, CIcsdiagctrl );
//					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+3, 0 );
//
//					//yoshida150311
//					if( (*m_pSnHeadData)->SVmzsw == 1 ){//経過措置チェック有り(入力可能にする)
//						diag_modify( IDC_ICSDIAG8CTRL1, indexEx+4, DIAG_MDFY_EDIT, CIcsdiagctrl );
//						Color_Change( IDC_ICSDIAG8CTRL1, indexEx+4, 0 );
//					}
//					else{//経過措置チェック有り(入力不可にする)
//						diag_modify( IDC_ICSDIAG8CTRL1, indexEx+4, DIAG_MDFY_READONLY, CIcsdiagctrl );
//						Color_Change( IDC_ICSDIAG8CTRL1, indexEx+4, 1 );
//					}
//---------------
					if( (*m_pSnHeadData)->SVmzsw == 1 ){	//経過措置チェック有り(入力可能にする)
						diag_modify( IDC_ICSDIAG8CTRL1, indexEx+3, DIAG_MDFY_EDIT, CIcsdiagctrl );
						Color_Change( IDC_ICSDIAG8CTRL1, indexEx+3, 0 );
					}
					else{									//経過措置チェック有り(入力不可にする)
						diag_modify( IDC_ICSDIAG8CTRL1, indexEx+3, DIAG_MDFY_READONLY, CIcsdiagctrl );
						Color_Change( IDC_ICSDIAG8CTRL1, indexEx+3, 1 );
					}
					diag_modify( IDC_ICSDIAG8CTRL1, indexEx+4, DIAG_MDFY_EDIT, CIcsdiagctrl );
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+4, 0 );
//---------------

				}
				else{
					diag_modify( IDC_ICSDIAG8CTRL1, indexEx+1, DIAG_MDFY_EDIT, CIcsdiagctrl );
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+1, 0 );
					diag_modify( IDC_ICSDIAG8CTRL1, indexEx+2, DIAG_MDFY_EDIT, CIcsdiagctrl );
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+2, 0 );
//-- '15.04.17 --
//					diag_modify( IDC_ICSDIAG8CTRL1, indexEx+3, DIAG_MDFY_EDIT, CIcsdiagctrl );
//					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+3, 0 );
//
//					//yoshida150311
//					if( (*m_pSnHeadData)->SVmzsw == 1 ){//経過措置チェック有り(入力可能にする)
//						diag_modify( IDC_ICSDIAG8CTRL1, indexEx+4, DIAG_MDFY_EDIT, CIcsdiagctrl );
//						Color_Change( IDC_ICSDIAG8CTRL1, indexEx+4, 0 );
//					}
//					else{//経過措置チェック有り(入力不可にする)
//						diag_modify( IDC_ICSDIAG8CTRL1, indexEx+4, DIAG_MDFY_READONLY, CIcsdiagctrl );
//						Color_Change( IDC_ICSDIAG8CTRL1, indexEx+4, 1 );
//					}				
//					diag_modify( IDC_ICSDIAG8CTRL1, indexEx+5, DIAG_MDFY_READONLY, CIcsdiagctrl );
//					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+5, 1 );
//---------------
					if( (*m_pSnHeadData)->SVmzsw == 1 ){	// 経過措置チェック有り(入力可能にする)
						diag_modify( IDC_ICSDIAG8CTRL1, indexEx+3, DIAG_MDFY_EDIT, CIcsdiagctrl );
						Color_Change( IDC_ICSDIAG8CTRL1, indexEx+3, 0 );
					}
					else{									// 経過措置チェック有り(入力不可にする)
						diag_modify( IDC_ICSDIAG8CTRL1, indexEx+3, DIAG_MDFY_READONLY, CIcsdiagctrl );
						Color_Change( IDC_ICSDIAG8CTRL1, indexEx+3, 1 );
					}
					diag_modify( IDC_ICSDIAG8CTRL1, indexEx+4, DIAG_MDFY_EDIT, CIcsdiagctrl );
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+4, 0 );
//---------------
					diag_modify( IDC_ICSDIAG8CTRL1, indexEx+5, DIAG_MDFY_READONLY, CIcsdiagctrl );
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+5, 1 );
				}
				break;
			default   :
				break;
		}

		index += 7;
		indexEx += 6;
		if( rno && (((rno+1)%7)==0) ){
			page++;
			index = ( NUM_PER_PAGE * page ) + 1;
			indexEx = ( NUM_PER_PAGE * page ) + DIAG_INP_STIDX;
		}
	}
	
	// 再計算
	Specific21_Calq();
}

//-----------------------------------------------------------------------------
// 再計算
//-----------------------------------------------------------------------------
void CShinSpecific21EX::Specific21_Calq( void )
{
	TSREC			TSrec;
	int				rno;
	char			MONY1_8per[MONY_BUF_SIZE]={0}, MONY2_8per[MONY_BUF_SIZE]={0}, MONY3_8per[MONY_BUF_SIZE]={0};
	char			MONY1_5per[MONY_BUF_SIZE]={0}, MONY2_5per[MONY_BUF_SIZE]={0}, MONY3_5per[MONY_BUF_SIZE]={0};
	char			MONY0_sum[MONY_BUF_SIZE]={0}, MONY1_sum[MONY_BUF_SIZE]={0}, MONY2_sum[MONY_BUF_SIZE]={0}, MONY3_sum[MONY_BUF_SIZE]={0};
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	rno = 0;
	while( 1 ){
		if( rno == (*m_pSpcListData)->TsTbl.tp_rnum ){
			break;
		}
		if( (*m_pSpcListData)->m_TblHdl.th_rget( &TSrec, &(*m_pSpcListData)->TsTbl, rno ) ){
			break;
		}
		// 資産の譲渡等の対価以外の収入合計
		m_pArith->l_add( MONY0_sum, MONY0_sum, TSrec.TsJsyu );
		// 特定収入合計
		m_pArith->l_add( MONY1_8per, MONY1_8per, TSrec.TsDtl[1].TsGsyu );
		m_pArith->l_add( MONY1_5per, MONY1_5per, TSrec.TsDtl[0].TsGsyu );

		// 課税仕入等にのみ係る特定収入
		if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == TRUE ){					// 個別対応
//-- '15.04.25 --
//			m_pArith->l_add( MONY2_8per, MONY2_8per, TSrec.TsDtl[1].TsSsyu );
//			m_pArith->l_add( MONY2_5per, MONY2_5per, TSrec.TsDtl[0].TsSsyu );
//---------------
			m_pArith->l_add( MONY2_8per, MONY2_8per, TSrec.TsDtl[1].TsTsyu );
			m_pArith->l_add( MONY2_5per, MONY2_5per, TSrec.TsDtl[0].TsTsyu );
//---------------
		}
		else{																	// 比例配分
			m_pArith->l_add( MONY2_8per, MONY2_8per, TSrec.TsDtl[1].TsTsyu );
			m_pArith->l_add( MONY2_5per, MONY2_5per, TSrec.TsDtl[0].TsTsyu );
		}

		// 不特定収入
		m_pArith->l_add( MONY3_8per, MONY3_8per, TSrec.TsDtl[1].TsFsyu );
		m_pArith->l_add( MONY3_5per, MONY3_5per, TSrec.TsDtl[0].TsFsyu );
		rno++;
	}

	// 資産の譲渡等
	memmove( DiagData.data_val, MONY0_sum, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL3, 7, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	
	// 特定収入合計
	m_pArith->l_add( MONY1_sum, MONY1_8per, MONY1_5per );
	memmove( DiagData.data_val, MONY1_sum, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL3, 8, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 課税仕入等にのみ係る特定収入(5%)
	memmove( DiagData.data_val, MONY2_5per, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL3, 9, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	// 課税仕入等にのみ係る特定収入(8%)
	memmove( DiagData.data_val, MONY2_8per, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL3, 10, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 不特定収入
	m_pArith->l_add( MONY3_sum, MONY3_8per, MONY3_5per );
	memmove( DiagData.data_val, MONY3_sum, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL3, 11, (struct IUnknown *)&DiagData, CIcsdiagctrl );


	// 特定収入合計
	memset( (*m_pSpcListData)->stval.SpGsyu, '\0', sizeof((*m_pSpcListData)->stval.SpGsyu) );
	memmove( (*m_pSpcListData)->stval.SpGsyu, MONY1_sum, sizeof((*m_pSpcListData)->stval.SpGsyu) );	
//-- '15.04.25 --
//	if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
//		// 課税仕入等にのみ係る特定収入
//		memset( (*m_pSpcListData)->stval.SpTsyu, '\0', sizeof((*m_pSpcListData)->stval.SpTsyu) );
//		memmove( (*m_pSpcListData)->stval.SpTsyu, MONY2_sum, sizeof((*m_pSpcListData)->stval.SpTsyu) );	
//
//		memset( (*m_pSpcListData)->stval.SpTsyu4, '\0', sizeof((*m_pSpcListData)->stval.SpTsyu4) );
//		memmove( (*m_pSpcListData)->stval.SpTsyu4, MONY2_5per, sizeof((*m_pSpcListData)->stval.SpTsyu4) );
//		memset( (*m_pSpcListData)->stval.SpTsyu63, '\0', sizeof((*m_pSpcListData)->stval.SpTsyu63) );
//		memmove( (*m_pSpcListData)->stval.SpTsyu63, MONY2_8per, sizeof((*m_pSpcListData)->stval.SpTsyu63) );	
//	}
//---------------
	// 課税仕入等にのみ係る特定収入
	memset( (*m_pSpcListData)->stval.SpTsyu, '\0', sizeof((*m_pSpcListData)->stval.SpTsyu) );
	memmove( (*m_pSpcListData)->stval.SpTsyu, MONY2_sum, sizeof((*m_pSpcListData)->stval.SpTsyu) );	

	memset( (*m_pSpcListData)->stval.SpTsyu4, '\0', sizeof((*m_pSpcListData)->stval.SpTsyu4) );
	memmove( (*m_pSpcListData)->stval.SpTsyu4, MONY2_5per, sizeof((*m_pSpcListData)->stval.SpTsyu4) );
	memset( (*m_pSpcListData)->stval.SpTsyu63, '\0', sizeof((*m_pSpcListData)->stval.SpTsyu63) );
	memmove( (*m_pSpcListData)->stval.SpTsyu63, MONY2_8per, sizeof((*m_pSpcListData)->stval.SpTsyu63) );	
//---------------

	// 不特定収入
	memset( (*m_pSpcListData)->stval.SpFsyu, '\0', sizeof((*m_pSpcListData)->stval.SpFsyu) );
	memmove( (*m_pSpcListData)->stval.SpFsyu, MONY3_sum, sizeof((*m_pSpcListData)->stval.SpFsyu) );

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

//-- '15.05.05 --
//	char	buf[128]={0};
//	sprintf_s( buf, _T("登録状況\n%3d/%3d"), (*m_pSpcListData)->TsTbl.tp_rnum, 210 );
//	c_cls( IDC_ICSDISP8CTRL1, CIcsdisp8ctrl1 );
//	puts( IDC_ICSDISP8CTRL1, buf, CIcsdisp8ctrl1 );
//---------------
	DispRegCount();
//---------------

	m_Specific211.Refresh();
}

//-----------------------------------------------------------------------------
// 登録数表示('15.05.05)
//-----------------------------------------------------------------------------
void CShinSpecific21EX::DispRegCount()
{
	char	buf[128]={0};
	sprintf_s( buf, _T("登録状況\n%3d/%3d"), (*m_pSpcListData)->TsTbl.tp_rnum, 210 );
	c_cls( IDC_ICSDISP8CTRL1, CIcsdisp8ctrl1 );
	puts( IDC_ICSDISP8CTRL1, buf, CIcsdisp8ctrl1 );
}

//-----------------------------------------------------------------------------
// Tblhdl 上のレコード位置を取得
//-----------------------------------------------------------------------------
// 引数	nowIdx	：	ダイアグラム上のインデックス
//-----------------------------------------------------------------------------
// 返送値		：	Tblhdl 上のレコード位置
//-----------------------------------------------------------------------------
int CShinSpecific21EX::GetTableNo( int nowIdx )
{
//	int		j, pageno, tableno;

	// 該当ページ番号の取得
	int pageno = (nowIdx / NUM_PER_PAGE);
	// 該当ページ先頭テーブル番号
	int stIdxInTbl = (pageno * TBL_REC_NUM_PER_PAGE);
	// 該当ページ先頭(ダイアグラム上)
	int stInNowPg = (pageno * NUM_PER_PAGE) + DIAG_INP_STIDX;

	int dif = nowIdx - stInNowPg;
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
int CShinSpecific21EX::GetColmNo( int nowIdx )
{
	int page = nowIdx / NUM_PER_PAGE;
	int dif = nowIdx - ((page*NUM_PER_PAGE)+DIAG_INP_STIDX);
	int colmn = dif % CLMNUM_PER_ROW;
	return colmn;
}

//-----------------------------------------------------------------------------
// 該当行の内容列のダイアグラムインデックスを取得
//-----------------------------------------------------------------------------
// 引数	line	：	行番号（CTblhdl上）
//-----------------------------------------------------------------------------
// 返送値		：	ダイアグラムインデックス
//-----------------------------------------------------------------------------
int findindexEX( int line )
{
	int find_line = max(line-1, 0);
	int pgno = line / TBL_REC_NUM_PER_PAGE;
//-- '15.05.02 --
//	if( !(line%TBL_REC_NUM_PER_PAGE) ){
//---------------
	if( line && !(line%TBL_REC_NUM_PER_PAGE) ){
//---------------
		pgno--;
	}
	if( pgno < 0 ){
		return -1;
	}

	return ((find_line%TBL_REC_NUM_PER_PAGE)*CLMNUM_PER_ROW) + (pgno*NUM_PER_PAGE) + DIAG_INP_STIDX;
}

BEGIN_EVENTSINK_MAP(CShinSpecific21EX, CSpcBaseDlg)
	ON_EVENT(CShinSpecific21EX, IDC_ICSDIAG8CTRL1, 1, CShinSpecific21EX::EditONIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinSpecific21EX, IDC_ICSDIAG8CTRL1, 2, CShinSpecific21EX::EditOFFIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinSpecific21EX, IDC_ICSDIAG8CTRL1, 3, CShinSpecific21EX::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinSpecific21EX::EditONIcsdiag8ctrl1(short index)
{

	m_curwnd = IDC_ICSDIAG8CTRL1;//yoshida150311

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( !m_initfg ){
		return;
	}

	int line = GetTableNo( index );
	if( line < (*m_pSpcListData)->TsTbl.tp_rnum ){
		return;
	}

	int find = findindexEX(line);
//	int find = findindexEX(line+1);
	if( find < 0 ){
		return;
	}

//	CDiagData DD;
	DIAGRAM_DATA		DD;
	DD.data_imgdata = NULL;
	int sw = 0;
	diag_getdata( IDC_ICSDIAG8CTRL1, find, (struct IUnknown *)&DD, CIcsdiagctrl );
	if( DD.data_edit.GetLength() ){
		sw |= 0x01;
	}
	diag_getdata( IDC_ICSDIAG8CTRL1, find+1, (struct IUnknown *)&DD, CIcsdiagctrl );
	if( m_pArith->l_test(DD.data_val) ){
		sw |= 0x02;
	}
	diag_getdata( IDC_ICSDIAG8CTRL1, find+2, (struct IUnknown *)&DD, CIcsdiagctrl );
	if( m_pArith->l_test(DD.data_val ) ){
		sw |= 0x04;
	}
	diag_getdata( IDC_ICSDIAG8CTRL1, find+3, (struct IUnknown *)&DD, CIcsdiagctrl );
	if( m_pArith->l_test(DD.data_val ) ){
		sw |= 0x08;
	}
	diag_getdata( IDC_ICSDIAG8CTRL1, find+4, (struct IUnknown *)&DD, CIcsdiagctrl );
	if( m_pArith->l_test(DD.data_val ) ){
		sw |= 0x10;
	}

	if( !sw ){
		int find = findindexEX((*m_pSpcListData)->TsTbl.tp_rnum + 1);
		if( (find<0) || (find==index) ){
			return;
		}
		diag_setposition( IDC_ICSDIAG8CTRL1, find, CIcsdiagctrl );
		return;
	}

	if( GetTableNo(index) == ERR ){
		return;
	}



}

void CShinSpecific21EX::EditOFFIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	TSREC				TSrec;
	int					TableNo, ColmNo, IndexAdd, PutData, NewLine;
	char				MONY[6]={0};
//--> '15.04.30 INS START
	BOOL				isMonyChg = FALSE;
//<-- '15.04.30 INS END
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	if( !m_initfg ){
		return;
	}
//--> '15.03.18 INS START
	if( IsWindowVisible() == FALSE ){
		return;
	}
//<-- '15.03.18 INS END

	ASSERT( m_pSpcListData );
	if( m_pSpcListData == NULL ){
		return;
	}
	ASSERT( *m_pSpcListData );
	if( *m_pSpcListData == NULL ){
		return;
	}
	
	diag_getdata( IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	if( (TableNo = GetTableNo(index)) == ERR ){
		return;
	}
	m_TableNo = TableNo;
	ColmNo = GetColmNo( index );
	// テーブル読み込み
	NewLine = 0;
	memset( &TSrec, '\0', sizeof( TSREC ) );
	if( (*m_pSpcListData)->m_TblHdl.th_rget( &TSrec, &(*m_pSpcListData)->TsTbl, TableNo ) ){
		if( (*m_pSpcListData)->TsTbl.tp_rnum+1 > (*m_pSpcListData)->TsTbl.tp_rmax ){
			return;
		}
		// 次行の新規登録
		memset( &TSrec, '\0', sizeof( TSREC ) );
		TSrec.TsLine = TableNo+1;
		NewLine = 1;
	}
	IndexAdd = 0;
	PutData = 0;
	switch( ColmNo ){
		case 0 :	// 科目名称
			sprintf_s( TSrec.TsName, sizeof( TSrec.TsName ), _T("%s"), DiagData.data_edit.GetBuffer());
			PutData = DiagData.data_edit.GetLength(); 
			break;
		case 1 :	// 資産の譲渡等
			//---->yoshida150309
			PutData = m_pArith->l_cmp( DiagData.data_val, TSrec.TsJsyu );
			memmove( TSrec.TsJsyu, DiagData.data_val, sizeof(TSrec.TsJsyu) );
	//		IndexAdd = ( index + 2 );
			//<-----------------
//--> '15.04.30 INS START
			if( PutData ){
				isMonyChg = TRUE;
			}
//<-- '15.04.30 INS END
			
			break;
		case 2 :	// 特定収入
//-- '15.03.18 --
//			PutData = m_pArith->l_cmp( DiagData.data_val, TSrec.TsDtl[1].TsGsyu );
//			memmove( TSrec.TsDtl[1].TsGsyu, DiagData.data_val, sizeof(TSrec.TsDtl[1].TsGsyu) );
//---------------
			if( (*m_pSnHeadData)->SVmzsw == 1 ){	// 経過税率あり
				char	sum[MONY_BUF_SIZE]={0};
				char	dif[MONY_BUF_SIZE]={0};
				m_pArith->l_add( sum, TSrec.TsDtl[0].TsGsyu, TSrec.TsDtl[1].TsGsyu );

				m_pArith->l_sub( dif, DiagData.data_val, sum );
				PutData = m_pArith->l_cmp( DiagData.data_val, sum );
				
				m_pArith->l_add( TSrec.TsDtl[1].TsGsyu, TSrec.TsDtl[1].TsGsyu, dif );
			}
			else{									// 経過税率なし
				PutData = m_pArith->l_cmp( DiagData.data_val, TSrec.TsDtl[1].TsGsyu );
				memmove( TSrec.TsDtl[1].TsGsyu, DiagData.data_val, sizeof(TSrec.TsDtl[1].TsGsyu) );
			}
//---------------
			IndexAdd = ( index + 3 );
//--> '15.04.30 INS START
			if( PutData ){
				isMonyChg = TRUE;
			}
//<-- '15.04.30 INS END
			break;
		case 3 :	// 課税仕入れ等に係る特定収入(5%)
			if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == TRUE ){
//-- '15.04.25 --
//				PutData = m_pArith->l_cmp( DiagData.data_val, TSrec.TsDtl[0].TsSsyu );
//				memmove( TSrec.TsDtl[0].TsSsyu, DiagData.data_val, sizeof(TSrec.TsDtl[0].TsSsyu) );
//---------------
				PutData = m_pArith->l_cmp( DiagData.data_val, TSrec.TsDtl[0].TsTsyu );
				memmove( TSrec.TsDtl[0].TsTsyu, DiagData.data_val, sizeof(TSrec.TsDtl[0].TsTsyu) );
//---------------

				//---->yoshida150505
				// 非課税に係るを算出
				memset( MONY, '\0', sizeof(MONY) );
				//非課税の計算を行う
				m_pArith->l_add( MONY, TSrec.TsDtl[0].TsSsyu, TSrec.TsDtl[0].TsKsyu );
				m_pArith->l_sub( TSrec.TsDtl[0].TsHsyu, TSrec.TsDtl[0].TsTsyu, MONY );
				//<------------------
			}
			else{
				PutData = m_pArith->l_cmp( DiagData.data_val, TSrec.TsDtl[0].TsTsyu );
				memmove( TSrec.TsDtl[0].TsTsyu, DiagData.data_val, sizeof(TSrec.TsDtl[0].TsTsyu) );

				//---->yoshida150505
				// 非課税に係るを算出
				memset( MONY, '\0', sizeof(MONY) );
				//非課税の計算を行う
				m_pArith->l_add( MONY, TSrec.TsDtl[0].TsSsyu, TSrec.TsDtl[0].TsKsyu );
				m_pArith->l_sub( TSrec.TsDtl[0].TsHsyu, TSrec.TsDtl[0].TsTsyu, MONY );
				//<------------------
			}
			IndexAdd = ( index + 2 );
//--> '15.04.30 INS START
			if( PutData ){
				isMonyChg = TRUE;
			}
//<-- '15.04.30 INS END
			break;
		case 4 :	// 課税仕入れ等に係る特定収入(8%)
			if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == TRUE ){
//-- '15.04.25 --
//				PutData = m_pArith->l_cmp( DiagData.data_val, TSrec.TsDtl[1].TsSsyu );
//				memmove( TSrec.TsDtl[1].TsSsyu, DiagData.data_val, sizeof(TSrec.TsDtl[1].TsSsyu) );
//---------------
				PutData = m_pArith->l_cmp( DiagData.data_val, TSrec.TsDtl[1].TsTsyu );
				memmove( TSrec.TsDtl[1].TsTsyu, DiagData.data_val, sizeof(TSrec.TsDtl[1].TsTsyu) );

				//---->yoshida150428
				// 非課税に係るを算出
				memset( MONY, '\0', sizeof(MONY) );
				//非課税の計算を行う
				m_pArith->l_add( MONY, TSrec.TsDtl[1].TsSsyu, TSrec.TsDtl[1].TsKsyu );
				m_pArith->l_sub( TSrec.TsDtl[1].TsHsyu, TSrec.TsDtl[1].TsTsyu, MONY );
				//<------------------
				
//---------------
			}
			else{
				PutData = m_pArith->l_cmp( DiagData.data_val, TSrec.TsDtl[1].TsTsyu );
				memmove( TSrec.TsDtl[1].TsTsyu, DiagData.data_val, sizeof(TSrec.TsDtl[1].TsTsyu) );

				//---->yoshida150428
				// 非課税に係るを算出
				memset( MONY, '\0', sizeof(MONY) );
				//非課税の計算を行う
				m_pArith->l_add( MONY, TSrec.TsDtl[1].TsSsyu, TSrec.TsDtl[1].TsKsyu );
				m_pArith->l_sub( TSrec.TsDtl[1].TsHsyu, TSrec.TsDtl[1].TsTsyu, MONY );
				//<------------------

			}
			IndexAdd = ( index + 1 );
//--> '15.04.30 INS START
			if( PutData ){
				isMonyChg = TRUE;
			}
//<-- '15.04.30 INS END
			break;
		default:	// 
			break;
	}

	if( IndexAdd ){
		memset( MONY, '\0', sizeof(MONY) );
//-- '15.03.18 --
//		if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == TRUE ){
//			// 個別対応
//			memmove( MONY, TSrec.TsDtl[1].TsGsyu, sizeof(MONY) );
//			m_pArith->l_sub( MONY, MONY, TSrec.TsDtl[1].TsSsyu );
//		}
//		else{
//			// 比例配分
//			m_pArith->l_sub( MONY, TSrec.TsDtl[1].TsGsyu, TSrec.TsDtl[1].TsTsyu );
//		}
//		memmove( TSrec.TsDtl[1].TsFsyu, MONY, sizeof(TSrec.TsDtl[1].TsFsyu) );
//		memmove( DiagData.data_val, TSrec.TsDtl[1].TsFsyu, sizeof(DiagData.data_val) );
//		diag_setdata( IDC_ICSDIAG8CTRL1, IndexAdd, (struct IUnknown *)&DiagData, CIcsdiagctrl );
//---------------
		if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == TRUE ){
			if( (ColmNo==2) || (ColmNo==4) ){
//-- '15.04.25 --
//				m_pArith->l_sub( MONY, TSrec.TsDtl[1].TsGsyu, TSrec.TsDtl[1].TsSsyu );
//				memmove( TSrec.TsDtl[1].TsFsyu, MONY, sizeof(TSrec.TsDtl[1].TsFsyu) );
//---------------
				m_pArith->l_sub( MONY, TSrec.TsDtl[1].TsGsyu, TSrec.TsDtl[1].TsTsyu );
				memmove( TSrec.TsDtl[1].TsFsyu, MONY, sizeof(TSrec.TsDtl[1].TsFsyu) );
//---------------
			}
			if( (ColmNo==2) || (ColmNo==3) ){
//-- '15.04.25 --
//				m_pArith->l_sub( MONY, TSrec.TsDtl[0].TsGsyu, TSrec.TsDtl[0].TsSsyu );
//				memmove( TSrec.TsDtl[0].TsFsyu, MONY, sizeof(TSrec.TsDtl[0].TsFsyu) );
//---------------
				m_pArith->l_sub( MONY, TSrec.TsDtl[0].TsGsyu, TSrec.TsDtl[0].TsTsyu );
				memmove( TSrec.TsDtl[0].TsFsyu, MONY, sizeof(TSrec.TsDtl[0].TsFsyu) );
//---------------
			}
		}
		else{
			if( (ColmNo==2) || (ColmNo==4) ){
				m_pArith->l_sub( MONY, TSrec.TsDtl[1].TsGsyu, TSrec.TsDtl[1].TsTsyu );
				memmove( TSrec.TsDtl[1].TsFsyu, MONY, sizeof(TSrec.TsDtl[1].TsFsyu) );
			}
			if( (ColmNo==2) || (ColmNo==3) ){
				m_pArith->l_sub( MONY, TSrec.TsDtl[0].TsGsyu, TSrec.TsDtl[0].TsTsyu );
				memmove( TSrec.TsDtl[0].TsFsyu, MONY, sizeof(TSrec.TsDtl[0].TsFsyu) );
			}
		}

		char	sum[MONY_BUF_SIZE]={0};
		m_pArith->l_add( sum, TSrec.TsDtl[0].TsFsyu, TSrec.TsDtl[1].TsFsyu );
		memmove( DiagData.data_val, sum, sizeof(DiagData.data_val) );

		diag_setdata( IDC_ICSDIAG8CTRL1, IndexAdd, (struct IUnknown *)&DiagData, CIcsdiagctrl );
//---------------
	}
#ifdef	_DEBUG_EX
	char	buf[128] = {0};
	char	mny1[20] = {0};
	char	mny2[20] = {0};
	char	mny3[20] = {0};
	l_print( mny1, TSrec.TsGsyu, "SSS,SSS,SSS,SSS" );
	l_print( mny2, TSrec.TsTsyu, "SSS,SSS,SSS,SSS" );
	l_print( mny3, TSrec.TsFsyu, "SSS,SSS,SSS,SSS" );
	sprintf_s( buf, "( %d, %d ) [%s]-[%s]=[%s]", m_TableNo, ColmNo, mny1, mny2, mny3 );	
	AfxGetMainWnd()->SetWindowText( buf );
#endif
	if( (PutData==0) && NewLine ){
		return;		// 入力データ無し
	}

	if( (*m_pSpcListData)->m_TblHdl.th_acs( thWritex, &TSrec, &(*m_pSpcListData)->TsTbl ) )	{
//-- '15.05.05 --
//		AfxMessageBox("テーブルに書き込み中に障害が発生しました");
//---------------
		ICSMessageBox("テーブルに書き込み中に障害が発生しました");
		return;
//---------------
	}

	// 再計算
//-- '15.04.30 --
//	Specific21_Calq();
//---------------
	if( isMonyChg ){
		Specific21_Calq();
	}
	else{
		DispRegCount();
	}
//---------------
}

void CShinSpecific21EX::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
{

	//------->yoshida150311
	// TODO: ここにメッセージ ハンドラ コードを追加します。
/*	if( nChar == 0 ){
		return;
	}

	if( !m_initfg ){
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

//--> '15.03.21 INS START
	int		TableNo;
	int		ColmNo;
	if( (TableNo=GetTableNo(index)) == ERR ){
		return;
	}
	m_TableNo = TableNo;
	ColmNo = GetColmNo( index );
//<-- '15.03.21 INS END
	
	int pos = 0;
	if( wTerm==VK_UP ){		
		pos = FocusMoveCheck(index , 1);
		m_Specific211.SetPosition(pos);	
	}
	if( wTerm==VK_DOWN ){
		pos = FocusMoveCheck(index , 0);
		m_Specific211.SetPosition(pos);
	}

	if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) ){
		pos = FocusMoveCheck(index , 2);
		m_Specific211.SetPosition(pos);
	}
	else if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
		pos = FocusMoveCheck(index , 3);
		m_Specific211.SetPosition(pos);
	}
//-- '15.03.21 --
//	if( wTerm == VK_DELETE ){
////		GetDiagData( index, length, &xdata, -1 );
//	}
//---------------
	else if( (wTerm==VK_DELETE) || (wTerm==VK_INSERT) ){
		if( wTerm == VK_DELETE ){
			LineDelete( TableNo );
		}
		else if( wTerm == VK_INSERT ){
			LineInsert( TableNo );
		}
	}
//---------------
	
	// 再計算
	Specific21_Calq();

	SetRedrawScrollToIdx(m_Specific211.GetPosition());
*/
	//<------------------------


	//----->yoshida150311
	

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	char	VK_FLG;
	short	wTerm;

	if( nChar == 0 ){
		return;
	}
	if( !m_initfg ){
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

	int		TableNo;
	int		ColmNo;
	if( ( TableNo = GetTableNo( index ) ) == ERR ){
		return;
	}
	m_TableNo = TableNo;
	ColmNo = GetColmNo( index );
		
	UINT	flg = 0;

	

	switch( wTerm ){
		case VK_RETURN: case VK_RIGHT://yoshida150323
//		case VK_RETURN: case VK_RIGHT: case VK_DOWN:
			flg = REPLY_NEXT;
			break;
		case VK_TAB://yoshida150402
		case VK_LEFT://yoshida150323
		case VK_F2://yoshida150402
//		case VK_LEFT: case VK_UP:
			flg = REPLY_BACK;
			break;

		//------>yoshida150323
		//上下キー対応
		case VK_DOWN:
			index = index + 6;
			if( m_TableNo && (((m_TableNo+1)%7)==0) ){
				index += DIAG_INP_STIDX;
			}	
			break;

		case VK_UP:
			index = index - 6;
			if( m_TableNo && ( ( m_TableNo % 7 ) == 0 ) ){
				index -= DIAG_INP_STIDX;
			}
			break;
		//<----------------------------

		case VK_DELETE :
			LineDelete( TableNo );
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
	
	//<----------------------------------
}

//-----------------------------------------------------------------------------
// テーブル番号の消去
//-----------------------------------------------------------------------------
int CShinSpecific21EX::LineDelete( int rwno )
{
	TSREC	TSrec;
	int		PutNo;

//--> '15.03.21 INS START
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
//<-- '15.03.21 INS END

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
	// 最終の絡む番号
	n = ( m * CLMNUM_PER_ROW );
	// セットカラム
	CRect	rc;
	this->GetClientRect( &rc );
	this->InvalidateRect( &rc );
	m_Specific211.SetPosition(k+n);

	return( FALSE );
}

//-----------------------------------------------------------------------------
// テーブル番号の挿入
//-----------------------------------------------------------------------------
int CShinSpecific21EX::LineInsert( int rwno )
{
	TSREC	TSrec;

	int		PutNo;

	if( (*m_pSpcListData)->TsTbl.tp_rnum >= (*m_pSpcListData)->TsTbl.tp_rmax ){//２１０個登録されている状態で１行目でINSERTがきいてたのを修正yoshida150427
//	if( rwno+1 >= (*m_pSpcListData)->TsTbl.tp_rmax ){
		ICSMessageBox( "新規に登録できません。" );
		return FALSE;
	}

	PutNo = (*m_pSpcListData)->TsTbl.tp_rnum;
	(*m_pSpcListData)->TsTbl.tp_rnum++;
	for( ; rwno != PutNo ; PutNo-- ){
		if( (*m_pSpcListData)->m_TblHdl.th_rget( &TSrec, &(*m_pSpcListData)->TsTbl, PutNo-1 ) ){
			return ERR;
		}
		TSrec.TsLine += 1;
		if( (*m_pSpcListData)->m_TblHdl.th_rput( &TSrec, &(*m_pSpcListData)->TsTbl, PutNo ) ){
			return ERR;
		}
	}
	memset( &TSrec, '\0', sizeof(TSREC) );
	TSrec.TsLine = rwno+1;
	if( (*m_pSpcListData)->m_TblHdl.th_rput( &TSrec, &(*m_pSpcListData)->TsTbl, rwno ) ){
		return ERR;
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

	return( FALSE );
}

void CShinSpecific21EX::OnBnClickedButton1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int	TableNo;

	int	index = m_Specific211.GetPosition();
	if( ( TableNo = GetTableNo( index ) ) == ERR ){
		return;
	}
	m_TableNo = TableNo;
	LineDelete( TableNo );
}

void CShinSpecific21EX::OnBnClickedButton2()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int	TableNo;

	int	index = m_Specific211.GetPosition();
	if( ( TableNo = GetTableNo( index ) ) == ERR ){
		return;
	}
	m_TableNo = TableNo;
	LineInsert( TableNo );
}

void CShinSpecific21EX::OnButtonF2()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	diag_setprev( IDC_ICSDIAG8CTRL1, CIcsdiagctrl );
}

void CShinSpecific21EX::OnButtonF3()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	diag_setnext( IDC_ICSDIAG8CTRL1, CIcsdiagctrl );
}

void CShinSpecific21EX::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
//	CSpcBaseDlg::OnOK();
}

void CShinSpecific21EX::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
//	CSpcBaseDlg::OnCancel();
}



//------------->yoshida150311

void CShinSpecific21EX::IndexControl()
{
	int	Cnt = 0;
	while( 1 ){
		if( Specific21[Cnt].Index == 0 ){
			break;
		}
		int	Flg;
		// 確定時は全入力不可
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			Flg = Specific21[Cnt].AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 )	Flg = Specific21[Cnt].UnConnect;
			else								Flg = Specific21[Cnt].Connect;
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

		ATRIB_MOD( IDC_ICSDIAG8CTRL1, Specific21[Cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, Specific21[Cnt].Connect );
		Cnt++;
	}

	//--->yoshida150310
	SetFocusRange();

	m_Specific211.EnableDelete(0);//DELETEキー１回で削除モード設定
	//<---------------

	return;
}


//-----------------------------------------------------------------------------
// 次フォーカスの場所検索（上下左右動作）
//-----------------------------------------------------------------------------
//　戻り値　：　次のフォーカスインデックス
//-----------------------------------------------------------------------------
int CShinSpecific21EX::FocusMoveCheck(int Index , int Move)
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
		for(i = 0 ; i < FCS_SPECIFIC21CNT ; i++){
			if(Index == FCS_Specific21[i].IDNo){
				break;
			}
		}
		idx = FCS_Specific21[i].IDNo;//現在ポジションセット
	}else if(( Move == 2 )||( Move == 3 )){
		for(i = 0 ; i < FCS_SPECIFIC21CNT ; i++){
			if(Index == FCS_Specific21_LR[i].IDNo){
				break;
			}
		}
		idx = FCS_Specific21_LR[i].IDNo;//現在ポジションセット
	}

	//次ポジション検索
	if(Move == 0){//下方向
		for(j = i + 1 ; j < FCS_SPECIFIC21CNT ; j++){
			m_Specific211.GetAttribute(FCS_Specific21[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Specific21[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 1){//上方向
		for(j = i - 1 ; j > -1 ; j--){
			m_Specific211.GetAttribute(FCS_Specific21[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Specific21[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 2){//右方向
		for(j = i + 1 ; j < FCS_SPECIFIC21CNT ; j++){
			m_Specific211.GetAttribute(FCS_Specific21_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Specific21_LR[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 3){//左方向
		for(j = i - 1 ; j > -1 ; j--){
			m_Specific211.GetAttribute(FCS_Specific21_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Specific21_LR[j].IDNo;
				break;
			}
		}
	}
	return idx;
}


void CShinSpecific21EX::SetFocusRange()
{

	DIAGRAM_ATTRIBUTE	DA;

	m_FcsUpEnd		= -1;
	m_FcsLeftEnd	= -1;
	m_FcsDownEnd	= -1;
	m_FcsRightEnd	= -1;

	for( int idx = 0; idx < FCS_SPECIFIC21CNT; idx++ ){
		m_Specific211.GetAttribute(FCS_Specific21[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsUpEnd = FCS_Specific21[idx].IDNo;
			break;
		}
	}
	for( int idx = 0; idx < FCS_SPECIFIC21CNT; idx++ ){
		m_Specific211.GetAttribute(FCS_Specific21_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsLeftEnd = FCS_Specific21_LR[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SPECIFIC21CNT-1; idx > -1 ; idx-- ){
		m_Specific211.GetAttribute(FCS_Specific21[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsDownEnd = FCS_Specific21[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SPECIFIC21CNT-1; idx > -1 ; idx-- ){
		m_Specific211.GetAttribute(FCS_Specific21_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsRightEnd = FCS_Specific21_LR[idx].IDNo;
			break;
		}
	}
}



//-----------------------------------------------------------------------------
// 初期ポジションセット 
//-----------------------------------------------------------------------------
// 引数	IsInEnd	：	終了処理中？
//-----------------------------------------------------------------------------
void CShinSpecific21EX::SetItemPosition( BOOL IsInEnd/*=FALSE*/ )
{
	int index;

	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAG8CTRL1 ) {
		if( (index = m_Specific211.GetPosition()) != -1) {

//-- '15.05.02 --
//			m_Specific211.SetPosition(index);
//			m_Specific211.SetFocus();
//---------------
			if( IsInEnd ){
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
				m_Specific211.SetFocus();
			}
//---------------
			return;
		}
	}

	this->SetFocus();

//--> '15.05.01 INS START
	m_Specific210.SetFocus();
	m_Specific211.SetFocus();
//<-- '15.05.01 INS END

	if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
		m_Specific211.SetPosition(ID210);
	}
	else{
	}
	SetRedrawScroll( 0 );

}


//<-------------------------------