// ShinSpecific21.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinSpecific21.h"
#include "TaxationListView.h"

//== 定数 ==
const	int	BottomIndex = 0;	// No3
const	int	StartIndex = 35;	// No2

// CShinSpecific21 ダイアログ

IMPLEMENT_DYNAMIC(CShinSpecific21, CSpcBaseDlg)

CShinSpecific21::CShinSpecific21(CWnd* pParent /*=NULL*/)
	: CSpcBaseDlg(CShinSpecific21::IDD, pParent)
	, m_Spage(0)
{
}

CShinSpecific21::~CShinSpecific21()
{
}

void CShinSpecific21::DoDataExchange(CDataExchange* pDX)
{
	CSpcBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL2, m_Specific21);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL3, m_Specific210);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Specific211);
	DDX_Control(pDX, IDC_ICSDISP8CTRL1, m_ICSDisp1);
}


BEGIN_MESSAGE_MAP(CShinSpecific21, CSpcBaseDlg)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON1, &CShinSpecific21::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CShinSpecific21::OnBnClickedButton2)
	ON_COMMAND(ID_BUTTON_F2, &CShinSpecific21::OnButtonF2)
	ON_COMMAND(ID_BUTTON_F3, &CShinSpecific21::OnButtonF3)
END_MESSAGE_MAP()


// CShinSpecific21 メッセージ ハンドラ

BOOL CShinSpecific21::OnInitDialog()
{
	CSpcBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	// 初期設定
	SpecificInit21();

	m_Specific211.EnableDelete(0);

	m_initfg = 1;

	CSpcBaseDlg::OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

BOOL CShinSpecific21::PreTranslateMessage(MSG* pMsg)
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
			//	m_pParent->PostMessageA( WM_CLOSE, 0 );
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

void CShinSpecific21::OnSize(UINT nType, int cx, int cy)
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

HBRUSH CShinSpecific21::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSpcBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
	if( nCtlColor == CTLCOLOR_DLG ){
		hbr = Ibrsh_ShinSpecific;
	}

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

void CShinSpecific21::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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
void CShinSpecific21::sykscroll( int mpos )
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
void CShinSpecific21::SetItemPosition( int IndexNo )
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

/*
//-----------------------------------------------------------------------------
// 入力項目　色変え
//-----------------------------------------------------------------------------
void CShinSpecific21::Color_Change( unsigned short id, short index, int sign )
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
void CShinSpecific21::SpecificInit21( void )
{
	// 倍長演算指定
	m_pArith->l_defn( 0x16 );
	// 最大ページ数
	m_Spage = 30;
	m_Specific211.SetScPageCnt( (short)m_Spage );

	SpecificDisp21();

	// 確定時は全入力不可
	if( ((*m_pSnHeadData)->Sn_Sign4&0x01) || !((*m_pSnHeadData)->Sn_Sign4&0x80) ){
		GetDlgItem( IDC_BUTTON1 )->EnableWindow( FALSE );
		GetDlgItem( IDC_BUTTON2 )->EnableWindow( FALSE );
	}

	SetItemPosition(0);
}

//-----------------------------------------------------------------------------
// 表示
//-----------------------------------------------------------------------------
void CShinSpecific21::SpecificDisp21( void )
{
	TSREC			TSrec;
	int				rno, max, index, indexEx, page;
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	max = 210;
	index = 1;
	indexEx = 35;
	page = 0;
	for( rno=0; rno!=max; rno++ ){
		char buf[128];
		sprintf_s( buf, sizeof( buf ), _T("%d"), rno+1 );
		DiagData.data_disp =  buf;
		diag_setdata( IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );

		if( rno < (*m_pSpcListData)->TsTbl.tp_rnum ){
			if( (*m_pSpcListData)->m_TblHdl.th_rget( &TSrec, &(*m_pSpcListData)->TsTbl, rno ) ){
				return;
			}
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+0, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+1, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+2, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+3, TRUE, CIcsdiagctrl );
			// 科目名称
			DiagData.data_edit =  TSrec.TsName;
			diag_setdata( IDC_ICSDIAG8CTRL1, indexEx, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			// 特定収入合計
			memmove( DiagData.data_val, TSrec.TsDtl[1].TsGsyu, 6 );
			diag_setdata( IDC_ICSDIAG8CTRL1, indexEx+1, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			// 課税仕入れに係る特定収入
			if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == TRUE ){
				// 特定収入（課税に係る＋非課税に係る＋共通に係る）★計算表の特殊合計用（個別対応専用）
				memmove( DiagData.data_val, TSrec.TsDtl[1].TsSsyu, 6 );
				diag_setdata( IDC_ICSDIAG8CTRL1, indexEx+2, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				// 個別対応時は課税仕入れに係る特定収入の入力をさせない。
				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+2, DIAG_MDFY_READONLY, CIcsdiagctrl );
				Color_Change( IDC_ICSDIAG8CTRL1, indexEx+2, 1 );
			}
			else{
				// 比例配分
				memmove( DiagData.data_val, TSrec.TsDtl[1].TsTsyu, 6 );
				diag_setdata( IDC_ICSDIAG8CTRL1, indexEx+2, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			}
			// 不特定収入
			memmove( DiagData.data_val, TSrec.TsDtl[1].TsFsyu, 6 );
			diag_setdata( IDC_ICSDIAG8CTRL1, indexEx+3, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		}
		else	{
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+0, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+1, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+2, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+3, TRUE, CIcsdiagctrl );
			if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == TRUE ){
				// 個別対応時は課税仕入れに係る特定収入の入力をさせない。
				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+2, DIAG_MDFY_READONLY, CIcsdiagctrl );
			}
		}

		// 確定時は全入力不可
		switch( (*m_pSnHeadData)->Sn_Sign4&0x81 ) {
			case 0x01 :	// ロック
			case 0x81 :	// 非連動でロック
				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+0, DIAG_MDFY_READONLY, CIcsdiagctrl );
				Color_Change( IDC_ICSDIAG8CTRL1, indexEx+0, 2 );
				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+1, DIAG_MDFY_READONLY, CIcsdiagctrl );
				Color_Change( IDC_ICSDIAG8CTRL1, indexEx+1, 2 );
				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+2, DIAG_MDFY_READONLY, CIcsdiagctrl );
				if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == TRUE ){
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+2, 1 );
				}
				else{
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+2, 2 );
				}
				break;
			case 0x00 :	// 連動
				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+0, DIAG_MDFY_READONLY, CIcsdiagctrl );
				Color_Change( IDC_ICSDIAG8CTRL1, indexEx+0, 1 );
				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+1, DIAG_MDFY_READONLY, CIcsdiagctrl );
				Color_Change( IDC_ICSDIAG8CTRL1, indexEx+1, 1 );
				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+2, DIAG_MDFY_READONLY, CIcsdiagctrl );
				Color_Change( IDC_ICSDIAG8CTRL1, indexEx+2, 1 );
				break;
			case 0x80 :	// 非連動
				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+0, DIAG_MDFY_EDIT, CIcsdiagctrl );
				Color_Change( IDC_ICSDIAG8CTRL1, indexEx+0, 0 );
				if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
					diag_modify( IDC_ICSDIAG8CTRL1, indexEx+1, DIAG_MDFY_EDIT, CIcsdiagctrl );
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+1, 0 );
					diag_modify( IDC_ICSDIAG8CTRL1, indexEx+2, DIAG_MDFY_EDIT, CIcsdiagctrl );
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+2, 0 );
				}
				else{
					diag_modify( IDC_ICSDIAG8CTRL1, indexEx+1, DIAG_MDFY_EDIT, CIcsdiagctrl );
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+1, 0 );
					diag_modify( IDC_ICSDIAG8CTRL1, indexEx+2, DIAG_MDFY_READONLY, CIcsdiagctrl );
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+2, 1 );
				}
				break;
			default   :
				break;
		}

		index += 5;
		indexEx += 4;
		if( rno && (((rno+1)%7)==0) ){
			page++;
			index = ( 63 * page ) + 1;
			indexEx = ( 63 * page ) + 35;
		}
	}
	
	// 再計算
	Specific21_Calq();
}

//-----------------------------------------------------------------------------
// 再計算
//-----------------------------------------------------------------------------
void CShinSpecific21::Specific21_Calq( void )
{
	TSREC			TSrec;
	int				rno;
	char			MONY1[6]={0}, MONY2[6]={0}, MONY3[6]={0};
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	rno = 0;
	while( 1 ){
		if( rno ==  (*m_pSpcListData)->TsTbl.tp_rnum ){
			break;
		}
		if( (*m_pSpcListData)->m_TblHdl.th_rget( &TSrec, &(*m_pSpcListData)->TsTbl, rno ) ){
			break;
		}
		// 特定収入合計
		m_pArith->l_add( MONY1, MONY1, TSrec.TsDtl[1].TsGsyu );
		// 課税仕入等にのみ係る特定収入
		if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == TRUE ){
			m_pArith->l_add( MONY2, MONY2, TSrec.TsDtl[1].TsSsyu );		// 個別対応
		}
		else{
			m_pArith->l_add( MONY2, MONY2, TSrec.TsDtl[1].TsTsyu );		// 比例配分
		}
		// 不特定収入
		m_pArith->l_add( MONY3, MONY3, TSrec.TsDtl[1].TsFsyu );
		rno++;
	}
	
	// 特定収入合計
	memmove( DiagData.data_val, MONY1, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL3, 5, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	// 課税仕入等にのみ係る特定収入
	memmove( DiagData.data_val, MONY2, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL3, 6, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	// 不特定収入
	memmove( DiagData.data_val, MONY3, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL3, 7, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 特定収入合計
	memset( (*m_pSpcListData)->stval.SpGsyu, '\0', sizeof((*m_pSpcListData)->stval.SpGsyu) );
	memmove( (*m_pSpcListData)->stval.SpGsyu, MONY1, sizeof((*m_pSpcListData)->stval.SpGsyu) );	
	if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
		// 課税仕入等にのみ係る特定収入
		memset( (*m_pSpcListData)->stval.SpTsyu, '\0', sizeof((*m_pSpcListData)->stval.SpTsyu) );
		memmove( (*m_pSpcListData)->stval.SpTsyu, MONY2, sizeof((*m_pSpcListData)->stval.SpTsyu) );	 
	}
	// 不特定収入
	memset( (*m_pSpcListData)->stval.SpFsyu, '\0', sizeof((*m_pSpcListData)->stval.SpFsyu) );
	memmove( (*m_pSpcListData)->stval.SpFsyu, MONY3, sizeof((*m_pSpcListData)->stval.SpFsyu) );

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
	
	char	buf[128]={0};
	sprintf_s( buf, _T("登録状況\n%3d/%3d"), (*m_pSpcListData)->TsTbl.tp_rnum, 210 );
	c_cls( IDC_ICSDISP8CTRL1, CIcsdisp8ctrl1 );
	puts( IDC_ICSDISP8CTRL1, buf, CIcsdisp8ctrl1 );

	m_Specific211.Refresh();
}

//-----------------------------------------------------------------------------
// テーブル番号の取得
//-----------------------------------------------------------------------------
int CShinSpecific21::GetTableNo( int index )
{
	int		i, j, pageno, tableno, indexno;

	// 該当ページ番号の取得
	pageno = (index / 63);
	// 該当ページ先頭テーブル番号
	tableno = (pageno * 7);
	// 該当ページ先頭
	indexno = (pageno * 63) + 35;

	i = index - indexno;
	j = ( i / 4 );
	tableno += j;

	return tableno;
}

//-----------------------------------------------------------------------------
// カラム番号の取得
//-----------------------------------------------------------------------------
int CShinSpecific21::GetColmNo( int index )
{
	int page = index / 63;
	int ofs = index - ( page * 63 + 35 );
	int colmn = ofs % 4;
	return colmn;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
int findindex(int line)
{
	int find_line = max(line - 1,0);
	int pgno = line / 7;
//-- '15.05.02 --
//	if(!(line % 7)){
//---------------
	if( line && !(line%7) ){
//---------------
		pgno--;
	}
	if( pgno < 0 ){
		return -1;
	}

	return ( find_line % 7 ) * 4  + ( pgno * 63 ) + 35;
}

BEGIN_EVENTSINK_MAP(CShinSpecific21, CSpcBaseDlg)
	ON_EVENT(CShinSpecific21, IDC_ICSDIAG8CTRL1, 1, CShinSpecific21::EditONIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinSpecific21, IDC_ICSDIAG8CTRL1, 2, CShinSpecific21::EditOFFIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinSpecific21, IDC_ICSDIAG8CTRL1, 3, CShinSpecific21::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinSpecific21::EditONIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( !m_initfg ){
		return;
	}

	int line = GetTableNo( index );
	if( line < (*m_pSpcListData)->TsTbl.tp_rnum ){
		return;
	}

	int find = findindex(line);
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

	if( !sw ){
		int find = findindex((*m_pSpcListData)->TsTbl.tp_rnum + 1);
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

void CShinSpecific21::EditOFFIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	TSREC				TSrec;
	int					TableNo, ColmNo, IndexAdd, PutData, NewLine;
	char				MONY[6]={0};
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	if( !m_initfg ){
		return;
	}

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
		case 1 :	// 特定収入
			PutData = m_pArith->l_cmp( DiagData.data_val, TSrec.TsDtl[1].TsGsyu );
			memmove( TSrec.TsDtl[1].TsGsyu, DiagData.data_val, sizeof(TSrec.TsDtl[1].TsGsyu) );
			IndexAdd = ( index + 2 );
			break;
		case 2 :	// 課税仕入れ等に係る特定収入
			if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == TRUE ){
				break;
			}
			PutData = m_pArith->l_cmp( DiagData.data_val, TSrec.TsDtl[1].TsTsyu );
			memmove( TSrec.TsDtl[1].TsTsyu, DiagData.data_val, sizeof(TSrec.TsDtl[1].TsTsyu) );
			IndexAdd = ( index + 1 );
			break;
		default:	// 
			break;
	}
	if( IndexAdd ){
		memset( MONY, '\0', sizeof(MONY) );
		if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == TRUE ){
			// 個別対応
			memmove( MONY, TSrec.TsDtl[1].TsGsyu, sizeof(MONY) );
			m_pArith->l_sub( MONY, MONY, TSrec.TsDtl[1].TsSsyu );
		}
		else{
			// 比例配分
			m_pArith->l_sub( MONY, TSrec.TsDtl[1].TsGsyu, TSrec.TsDtl[1].TsTsyu );
		}
		memmove( TSrec.TsDtl[1].TsFsyu, MONY, sizeof(TSrec.TsDtl[1].TsFsyu) );
		memmove( DiagData.data_val, TSrec.TsDtl[1].TsFsyu, sizeof(DiagData.data_val) );
		diag_setdata( IDC_ICSDIAG8CTRL1, IndexAdd, (struct IUnknown *)&DiagData, CIcsdiagctrl );
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
		AfxMessageBox("テーブルに書き込み中に障害が発生しました");
	}

	// 再計算
	Specific21_Calq();
}

void CShinSpecific21::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
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

	switch( nChar ){
		case VK_RETURN: case VK_RIGHT: case VK_DOWN:
			flg = REPLY_NEXT;
			break;
		case VK_LEFT: case VK_UP:
			flg = REPLY_BACK;
			break;
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
}

//-----------------------------------------------------------------------------
// テーブル番号の消去
//-----------------------------------------------------------------------------
int CShinSpecific21::LineDelete( int rwno )
{
	TSREC	TSrec;
	int		PutNo;

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
	j = ( i / 7 );
	k = (j * 63) + 35;
	m_Specific211.SetPosition(k);
	// 先頭のテーブル番号
	l = ( j * 7 );
	// 最終のテーブル番号
	m = i - l;
	// 最終の絡む番号
	n = ( m * 4 );
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
int CShinSpecific21::LineInsert( int rwno )
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
	j = ( i / 7 );
	k = (j * 63) + 35;
	m_Specific211.SetPosition(k);
	// 先頭のテーブル番号
	l = ( j * 7 );
	// 最終のテーブル番号
	m = i - l;
	// 最終の絡む番号
	n = ( m * 4 );
	// セットカラム
	CRect	rc;
	this->GetClientRect( &rc );
	this->InvalidateRect( &rc );
	m_Specific211.SetPosition(k+n);

	return( FALSE );
}

void CShinSpecific21::OnBnClickedButton1()
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

void CShinSpecific21::OnBnClickedButton2()
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

void CShinSpecific21::OnButtonF2()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	diag_setprev( IDC_ICSDIAG8CTRL1, CIcsdiagctrl );
}

void CShinSpecific21::OnButtonF3()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	diag_setnext( IDC_ICSDIAG8CTRL1, CIcsdiagctrl );
}

void CShinSpecific21::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
//	CSpcBaseDlg::OnOK();
}

void CShinSpecific21::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
//	CSpcBaseDlg::OnCancel();
}
