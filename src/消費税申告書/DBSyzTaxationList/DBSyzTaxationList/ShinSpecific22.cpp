// ShinSpecific22.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinSpecific22.h"
#include "ShinSpecific22Idx.h"//yoshida150315
#include "TaxationListView.h"

// CShinSpecific22 ダイアログ

//== 関数宣言 ==
extern int findindex( int line );

//== 定数 ==
const	int	BottomIndex = 0;	// No3
const	int	StartIndex = 35;	// No2

const	int	DIAG2_INP_STIDX = 35;	// 1ページの入力開始インデックス


IMPLEMENT_DYNAMIC(CShinSpecific22, CSpcBaseDlg)

CShinSpecific22::CShinSpecific22(CWnd* pParent /*=NULL*/)
	: CSpcBaseDlg(CShinSpecific22::IDD, pParent)
	, m_Spage(0)
	, m_zeiType(ID_ICSSH_SPC_8PER)
{

	//------>yoshida150316
	m_curwnd = -1;

	m_FcsUpEnd		= -1;
	m_FcsLeftEnd	= -1;
	m_FcsDownEnd	= -1;
	m_FcsRightEnd	= -1;
	//<-------------------

}


CShinSpecific22::CShinSpecific22(int zeitype, CWnd* pParent /*=NULL*/)
	: CSpcBaseDlg(CShinSpecific22::IDD, pParent)
	, m_Spage(0)
	, m_zeiType(zeitype)
{
	//------>yoshida150316
	m_curwnd = -1;

	m_FcsUpEnd		= -1;
	m_FcsLeftEnd	= -1;
	m_FcsDownEnd	= -1;
	m_FcsRightEnd	= -1;
	//<-------------------

}

CShinSpecific22::~CShinSpecific22()
{
}

void CShinSpecific22::DoDataExchange(CDataExchange* pDX)
{
	CSpcBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL2, m_Specific22);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL3, m_Specific220);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Specific221);
	DDX_Control(pDX, IDC_ICSDISP8CTRL1, m_ICSDisp1);
}


BEGIN_MESSAGE_MAP(CShinSpecific22, CSpcBaseDlg)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON1, &CShinSpecific22::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CShinSpecific22::OnBnClickedButton2)
	ON_COMMAND(ID_BUTTON_F2, &CShinSpecific22::OnButtonF2)
	ON_COMMAND(ID_BUTTON_F3, &CShinSpecific22::OnButtonF3)
END_MESSAGE_MAP()


// CShinSpecific22 メッセージ ハンドラ

BOOL CShinSpecific22::OnInitDialog()
{
	CSpcBaseDlg::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	// 初期設定
	SpecificInit22();

	m_Specific221.EnableDelete(0);

	m_initfg = 1;

	CSpcBaseDlg::OnInitDialogEX();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

BOOL CShinSpecific22::PreTranslateMessage(MSG* pMsg)
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

void CShinSpecific22::OnSize(UINT nType, int cx, int cy)
{
	CSpcBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( m_initfg ){
		RECT	a, b;

		GetWindowRect( &a );

		m_Specific220.GetRect( BottomIndex, (LPUNKNOWN)&b );

#ifdef	_CLOSE
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

HBRUSH CShinSpecific22::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSpcBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
	if( nCtlColor == CTLCOLOR_DLG ){
		hbr = Ibrsh_ShinSpecific;
	}

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

void CShinSpecific22::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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
void CShinSpecific22::sykscroll( int mpos )
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

//----->yoshida150316
/*
//-----------------------------------------------------------------------------
// セットポジション
//-----------------------------------------------------------------------------
void CShinSpecific22::SetItemPosition( int IndexNo )
{
	int	Index;
	if( IndexNo ){
		Index = IndexNo;
	}
	else{
		Index = StartIndex;
	}

	m_Specific221.SetFocus();
	// 確定時は全入力不可
	if( ((*m_pSnHeadData)->Sn_Sign4&0x01) ){
		return;
	}

	m_Specific221.SetPosition( Index );

	m_Specific221.Refresh();

	sykscroll( 0 );
}
*/
//<--------------------------

/*
//-----------------------------------------------------------------------------
// 入力項目　色変え
//-----------------------------------------------------------------------------
void CShinSpecific22::Color_Change( unsigned short id, short index, int sign )
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
void CShinSpecific22::SpecificInit22( void )
{
	// 倍長演算指定
	m_pArith->l_defn( 0x16 );

	// 最大ページ数
	m_Spage = 30;
	m_Specific221.SetScPageCnt( (short )m_Spage );


//2017.03.01 UPDATE START
	////------->yoshida150429
	//DIAGRAM_DATA		DiagData;
	//DiagData.data_imgdata = NULL;
	//if( m_zeiType == ID_ICSSH_SPC_8PER ){
	//	DiagData.data_disp = _T("　課税仕入れ等\n　(税率6.3％)\n　に係る特定収入\n　(計算表２(1)Ｃ）");
	//}
	//else if( m_zeiType == ID_ICSSH_SPC_5PER ){
	//	DiagData.data_disp = _T("　課税仕入れ等\n　(税率4％)に\n　係る特定収入\n　(計算表２(1)Ｂ）");
	//}
	//diag_setdata( IDC_ICSDIAG8CTRL2, 2, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	////<--------------------

	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;
	if( ((CTaxationListView*)m_pParent)->IsKeikaType_Keisansyo() == TRUE ){
		DiagData.data_disp = _T("　課税仕入れ等に係る\n特定収入\n　(計算表２(1)のＢ欄の\n金額）");
		diag_setdata( IDC_ICSDIAG8CTRL2, 2, (struct IUnknown *)&DiagData, CIcsdiagctrl );

		DiagData.data_disp = _T("　課税売上げにのみ要す\n　る課税仕入れ等にのみ\n　使途が特定されている\n　特定収入");
		diag_setdata( IDC_ICSDIAG8CTRL2, 4, (struct IUnknown *)&DiagData, CIcsdiagctrl );

		DiagData.data_disp = _T("　課税・非課税売上げに\n　共通して要する課税仕\n　入れ等にのみ使途が特\n　定されている特定収入");
		diag_setdata( IDC_ICSDIAG8CTRL2, 5, (struct IUnknown *)&DiagData, CIcsdiagctrl );

		DiagData.data_disp = _T("Ｄ");
		diag_setdata( IDC_ICSDIAG8CTRL2, 6, (struct IUnknown *)&DiagData, CIcsdiagctrl );

		diag_modify( IDC_ICSDIAG8CTRL2, 15, DIAG_MDFY_OPAQUE, CIcsdiagctrl );
	}else{
		if( m_zeiType == ID_ICSSH_SPC_8PER ){
			DiagData.data_disp = _T("　課税仕入れ等\n　(税率6.3％)\n　に係る特定収入\n　(計算表２(1)Ｃ）");
		}
		else if( m_zeiType == ID_ICSSH_SPC_5PER ){
			DiagData.data_disp = _T("　課税仕入れ等\n　(税率4％)に\n　係る特定収入\n　(計算表２(1)Ｂ）");
		}
		diag_setdata( IDC_ICSDIAG8CTRL2, 2, (struct IUnknown *)&DiagData, CIcsdiagctrl );

		DiagData.data_disp = _T("　うち課税売上げにのみ\n　要する課税仕入れ等に\n　のみ使途が特定されて\n　いる特定収入");
		diag_setdata( IDC_ICSDIAG8CTRL2, 4, (struct IUnknown *)&DiagData, CIcsdiagctrl );

		DiagData.data_disp = _T("　うち課税・非課税売上げ\n　に共通して要する課税\n　仕入れ等にのみ使途が\n　特定されている特定収入");
		diag_setdata( IDC_ICSDIAG8CTRL2, 5, (struct IUnknown *)&DiagData, CIcsdiagctrl );

		DiagData.data_disp = _T("　");
		diag_setdata( IDC_ICSDIAG8CTRL2, 6, (struct IUnknown *)&DiagData, CIcsdiagctrl );

		diag_modify( IDC_ICSDIAG8CTRL2, 15, DIAG_MDFY_TRANSPARENT, CIcsdiagctrl );
	}
//2017.03.01 UPDATE END

	IndexControl();

	SpecificDisp22();

	SetItemPosition();

	/*yoshida150316
	// 最大ページ数
	m_Spage = 30;
	m_Specific221.SetScPageCnt( (short )m_Spage );

	SpecificDisp22();

	// 確定時は全入力不可
	if( ((*m_pSnHeadData)->Sn_Sign4&0x01) || !((*m_pSnHeadData)->Sn_Sign4&0x80) ){
		GetDlgItem( IDC_BUTTON1 )->EnableWindow( FALSE );
		GetDlgItem( IDC_BUTTON2 )->EnableWindow( FALSE );
	}

	SetItemPosition(0);
	*/

//--> '15.03.21 INS START
	// 計算表２（１）で、二つの税率をまとめているので計算表２（２）では、行の制御は行わない！！
	GetDlgItem( IDC_BUTTON1 )->EnableWindow( FALSE );
	GetDlgItem( IDC_BUTTON1 )->ShowWindow( SW_HIDE );
	GetDlgItem( IDC_BUTTON2 )->EnableWindow( FALSE );
	GetDlgItem( IDC_BUTTON2 )->ShowWindow( SW_HIDE );
//<-- '15.03.21 INS END
}

//-----------------------------------------------------------------------------
// 該当税率データ？
//-----------------------------------------------------------------------------
// 引数		pTsrec	：	チェック対象
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	該当税率データ
//			FALSE	：	該当税率データでない
//-----------------------------------------------------------------------------
BOOL CShinSpecific22::IsNowZrtData( TSREC *pTsrec )
{
	BOOL	bRt = FALSE;

	ASSERT( pTsrec );
	if( pTsrec == NULL ){
		return bRt;
	}

	// 金額チェック
	int	zrIdx=0;
	if( m_zeiType == ID_ICSSH_SPC_5PER ){
		zrIdx = 0;
	}
	else if( m_zeiType == ID_ICSSH_SPC_8PER ){
		zrIdx = 1;
	}

	if( m_pArith->l_test(pTsrec->TsDtl[zrIdx].TsTsyu) || m_pArith->l_test(pTsrec->TsDtl[zrIdx].TsSsyu) || 
		m_pArith->l_test(pTsrec->TsDtl[zrIdx].TsHsyu) || m_pArith->l_test(pTsrec->TsDtl[zrIdx].TsKsyu) ){
		bRt = TRUE;
	}

	return bRt;
}

//-----------------------------------------------------------------------------
// 表示
//-----------------------------------------------------------------------------
void CShinSpecific22::SpecificDisp22( void )
{
	TSREC			TSrec;
	int				rno, max, index, indexEx, page, lno;
	DIAGRAM_DATA   DiagData;
	DiagData.data_imgdata = NULL;
	int				zrIdx=0;
	if( m_zeiType == ID_ICSSH_SPC_5PER ){
		zrIdx = 0;
	}
	else if( m_zeiType == ID_ICSSH_SPC_8PER ){
		zrIdx = 1;
	}
	else{
		ASSERT( !_T("不正な税率") );
		return;
	}

	m_TblIdxAry.RemoveAll();

	max = 210;
	index = 1;
	indexEx = 35;
	page = 0;
	for( rno=0, lno=0; lno!=max; ){
		char buf[128]={0};
		sprintf_s( buf, sizeof( buf ), _T("%d"), lno+1 );
		DiagData.data_disp =  buf;
		diag_setdata( IDC_ICSDIAG8CTRL1, index, (struct IUnknown *)&DiagData, CIcsdiagctrl );

		if( rno <  (*m_pSpcListData)->TsTbl.tp_rnum ){
			if( (*m_pSpcListData)->m_TblHdl.th_rget( &TSrec, &(*m_pSpcListData)->TsTbl, rno ) ){
				return;
			}

			if( IsNowZrtData(&TSrec) == FALSE ){
				rno++;
				continue;
			}
			m_TblIdxAry.Add( rno );

			// 科目名称
			DiagData.data_edit =  TSrec.TsName;
			diag_setdata( IDC_ICSDIAG8CTRL1, indexEx, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			// 特定収入（課税に係る＋非課税に係る＋共通に係る）★計算表の特殊合計用（個別対応専用）
//-- '15.04.25 --
//			memmove( DiagData.data_val, TSrec.TsDtl[zrIdx].TsSsyu, sizeof(DiagData.data_val) );
//---------------
			memmove( DiagData.data_val, TSrec.TsDtl[zrIdx].TsTsyu, sizeof(DiagData.data_val) );
//---------------
			diag_setdata( IDC_ICSDIAG8CTRL1, indexEx+1, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			// 課税売上にのみ係る
//-- '15.04.25 --
//			memmove( DiagData.data_val, TSrec.TsDtl[zrIdx].TsTsyu, sizeof(DiagData.data_val) );
//---------------
			memmove( DiagData.data_val, TSrec.TsDtl[zrIdx].TsSsyu, sizeof(DiagData.data_val) );
//---------------
			diag_setdata( IDC_ICSDIAG8CTRL1, indexEx+2, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			// 共通に係る
			memmove( DiagData.data_val, TSrec.TsDtl[zrIdx].TsKsyu, sizeof(DiagData.data_val) );
			diag_setdata( IDC_ICSDIAG8CTRL1, indexEx+3, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		}
		else{
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+0, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+1, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+2, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+3, TRUE, CIcsdiagctrl );
		}

		// 確定時は全入力不可
		if( ((*m_pSnHeadData)->Sn_Sign4&0x01) || (((*m_pSnHeadData)->Sn_Sign4&0x80)==0x00) ){
			int	flg = 1;
//-- '15.04.30 --
//			if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
//---------------
			if( ((*m_pSnHeadData)->Sn_Sign4&0x01) && (((*m_pSnHeadData)->Sn_Sign4&0x80)==0x80) && (lno<(int)m_TblIdxAry.GetCount()) ){
//---------------
				flg = 2;
			}
			diag_modify( IDC_ICSDIAG8CTRL1, indexEx+0, DIAG_MDFY_READONLY, CIcsdiagctrl );
//--> '15.03.17 CUT START
//			Color_Change( IDC_ICSDIAG8CTRL1, indexEx+0, flg );
//<-- '15.03.17 CUT END
			diag_modify( IDC_ICSDIAG8CTRL1, indexEx+1, DIAG_MDFY_READONLY, CIcsdiagctrl );
//--> '15.03.17 CUT START
//			Color_Change( IDC_ICSDIAG8CTRL1, indexEx+1, flg );
//<-- '15.03.17 CUT END
			diag_modify( IDC_ICSDIAG8CTRL1, indexEx+2, DIAG_MDFY_READONLY, CIcsdiagctrl );
			Color_Change( IDC_ICSDIAG8CTRL1, indexEx+2, flg );
			diag_modify( IDC_ICSDIAG8CTRL1, indexEx+3, DIAG_MDFY_READONLY, CIcsdiagctrl );
			Color_Change( IDC_ICSDIAG8CTRL1, indexEx+3, flg );
		}
		//------>yoshida150402
		//計算表２（１）で登録している分まで登録可とする。それ以降は入力不可
		else if(lno >= (int)m_TblIdxAry.GetCount()){
			int	flg = 1;
			diag_modify( IDC_ICSDIAG8CTRL1, indexEx+0, DIAG_MDFY_READONLY, CIcsdiagctrl );
			diag_modify( IDC_ICSDIAG8CTRL1, indexEx+1, DIAG_MDFY_READONLY, CIcsdiagctrl );
			diag_modify( IDC_ICSDIAG8CTRL1, indexEx+2, DIAG_MDFY_READONLY, CIcsdiagctrl );
			Color_Change( IDC_ICSDIAG8CTRL1, indexEx+2, flg );
			diag_modify( IDC_ICSDIAG8CTRL1, indexEx+3, DIAG_MDFY_READONLY, CIcsdiagctrl );
			Color_Change( IDC_ICSDIAG8CTRL1, indexEx+3, flg );

		}

		//<-------------------
		else{
//--> '15.03.17 CUT START
//			Color_Change( IDC_ICSDIAG8CTRL1, indexEx+0, 0 );
//			Color_Change( IDC_ICSDIAG8CTRL1, indexEx+1, 0 );
//<-- '15.03.17 CUT END
			Color_Change( IDC_ICSDIAG8CTRL1, indexEx+2, 0 );
			Color_Change( IDC_ICSDIAG8CTRL1, indexEx+3, 0 );
		}

		index += 5;
		indexEx += 4;
//-- '15.03.23 --
//		if( rno && (((rno+1)%7)==0) ){
//---------------
		if( lno && (((lno+1)%7)==0) ){
//---------------
			page++;
			index = ( 63 * page ) + 1;
			indexEx = ( 63 * page ) + 35;
		}
		lno++;
		rno++;
	}
	
	// 再計算
	Specific22_Calq();
}

//-----------------------------------------------------------------------------
// 再計算
//-----------------------------------------------------------------------------
void CShinSpecific22::Specific22_Calq( void )
{
	TSREC			TSrec;
	int				rno;
	char			MONY1[6]={0}, MONY2[6]={0}, MONY3[6]={0}, MONY4[6]={0}, MONY5[6]={0};
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	int				zrIdx=0;
	if( m_zeiType == ID_ICSSH_SPC_5PER ){
		zrIdx = 0;
	}
	else if( m_zeiType == ID_ICSSH_SPC_8PER ){
		zrIdx = 1;
	}
	else{
		ASSERT( !_T("不正な税率") );
		return;
	}

	rno = 0;
	while( 1 ){
		if( rno ==  (*m_pSpcListData)->TsTbl.tp_rnum ){
			break;
		}
		if( (*m_pSpcListData)->m_TblHdl.th_rget( &TSrec, &(*m_pSpcListData)->TsTbl, rno ) ){
			break;
		}
//-- '15.04.25 --
//		m_pArith->l_add( MONY1, MONY1, TSrec.TsDtl[zrIdx].TsSsyu );
//		m_pArith->l_add( MONY2, MONY2, TSrec.TsDtl[zrIdx].TsTsyu );
//---------------
		m_pArith->l_add( MONY1, MONY1, TSrec.TsDtl[zrIdx].TsTsyu );
		m_pArith->l_add( MONY2, MONY2, TSrec.TsDtl[zrIdx].TsSsyu );
//---------------
		m_pArith->l_add( MONY3, MONY3, TSrec.TsDtl[zrIdx].TsKsyu );
		m_pArith->l_add( MONY4, MONY4, TSrec.TsDtl[zrIdx].TsFsyu );
		m_pArith->l_add( MONY5, MONY5, TSrec.TsDtl[zrIdx].TsHsyu );
		rno++;
	}	
	// 特定収入（課税に係る＋非課税に係る＋共通に係る）★計算表の特殊合計用（個別対応専用）
	memmove( DiagData.data_val, MONY1, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL3, 5, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	// 課税売上に係る特定収入
	memmove( DiagData.data_val, MONY2, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL3, 6, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	// 共通に係る特定収入
	memmove( DiagData.data_val, MONY3, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL3, 7, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	if( m_zeiType == ID_ICSSH_SPC_5PER ){
//--> '15.04.25 CUT START
//		// 特定収入
//		memset( (*m_pSpcListData)->stval.SpTsyu4, '\0', sizeof((*m_pSpcListData)->stval.SpTsyu4) );
//		// 不特定収入
//		memset( (*m_pSpcListData)->stval.SpFsyu4, '\0', sizeof((*m_pSpcListData)->stval.SpFsyu4) );
//		// 特定収入合計	
//		memset( (*m_pSpcListData)->stval.SpGsyu4, '\0', sizeof((*m_pSpcListData)->stval.SpGsyu4) );
//
////-- '15.04.25 --
////		// 特定収入（課税に係る＋非課税に係る＋共通に係る）★計算表の特殊合計用（個別対応専用）						
////---------------
//		// 特定収入（課税仕入れ等にかかる）
////---------------
//		memmove( (*m_pSpcListData)->stval.SpTsyu4, MONY2, sizeof((*m_pSpcListData)->stval.SpTsyu4) );
//<-- '15.04.25 CUT END
//--> '15.04.25 INS START
		// 特定収入	 課税売上にのみに係る
		memmove( (*m_pSpcListData)->stval.SpSsyu4, MONY2, sizeof((*m_pSpcListData)->stval.SpSsyu4) );
//<-- '15.04.25 INS END
		// 特定収入	 非課税売上にのみに係る
		memmove( (*m_pSpcListData)->stval.SpHsyu4, MONY5, sizeof((*m_pSpcListData)->stval.SpHsyu4) );
		// 不特定収入
		memmove( (*m_pSpcListData)->stval.SpFsyu4, MONY4, sizeof((*m_pSpcListData)->stval.SpFsyu4) );
//--> '15.04.15 INS START
		// 特定収入	 共通に係る
		memmove( (*m_pSpcListData)->stval.SpKsyu4, MONY3, sizeof((*m_pSpcListData)->stval.SpKsyu4) );
//<-- '15.04.15 INS END
		// 特定収入合計
		m_pArith->l_add( (*m_pSpcListData)->stval.SpGsyu4, MONY1, MONY4 );
	}
	else if( m_zeiType == ID_ICSSH_SPC_8PER ){
//--> '15.04.25 CUT START
//		// 特定収入
//		memset( (*m_pSpcListData)->stval.SpTsyu63, '\0', sizeof((*m_pSpcListData)->stval.SpTsyu63) );
//		// 不特定収入
//		memset( (*m_pSpcListData)->stval.SpFsyu63, '\0', sizeof((*m_pSpcListData)->stval.SpFsyu63) );
//		// 特定収入合計	
//		memset( (*m_pSpcListData)->stval.SpGsyu63, '\0', sizeof((*m_pSpcListData)->stval.SpGsyu63) );
//
////-- '15.04.25 --
////		// 特定収入（課税に係る＋非課税に係る＋共通に係る）★計算表の特殊合計用（個別対応専用）						
////---------------
//		// 特定収入（課税仕入れ等にかかる）
////---------------
//		memmove( (*m_pSpcListData)->stval.SpTsyu63, MONY2, sizeof((*m_pSpcListData)->stval.SpTsyu63) );
//<-- '15.04.25 CUT END
//--> '15.04.25 INS START
		// 特定収入	 課税売上にのみに係る
		memmove( (*m_pSpcListData)->stval.SpSsyu63, MONY2, sizeof((*m_pSpcListData)->stval.SpSsyu63) );
//<-- '15.04.25 INS END
		// 特定収入	 非課税売上にのみに係る
		memmove( (*m_pSpcListData)->stval.SpHsyu63, MONY5, sizeof((*m_pSpcListData)->stval.SpHsyu63) );
		// 不特定収入
		memmove( (*m_pSpcListData)->stval.SpFsyu63, MONY4, sizeof((*m_pSpcListData)->stval.SpFsyu63) );
//--> '15.04.15 INS START
		// 特定収入	 共通に係る
		memmove( (*m_pSpcListData)->stval.SpKsyu63, MONY3, sizeof((*m_pSpcListData)->stval.SpKsyu63) );
//<-- '15.04.15 INS END
		// 特定収入合計
		m_pArith->l_add( (*m_pSpcListData)->stval.SpGsyu63, MONY1, MONY4 );
	}

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
//-- '15.03.23 --
//	sprintf_s( buf, _T("登録状況\n%3d/%3d"),  (*m_pSpcListData)->TsTbl.tp_rnum, 210 );
//---------------
	sprintf_s( buf, _T("登録状況\n%3d/%3d"),  (int)m_TblIdxAry.GetCount(), 210 );
//---------------
	c_cls( IDC_ICSDISP8CTRL1, CIcsdisp8ctrl1 );
	puts( IDC_ICSDISP8CTRL1, buf, CIcsdisp8ctrl1 );

	m_Specific221.Refresh();
}

//-----------------------------------------------------------------------------
// テーブル番号の取得
//-----------------------------------------------------------------------------
int CShinSpecific22::GetTableNo( int index )
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

//-- '15.03.17 --
	//yoshida150402
	//再度下の処理に戻した。２（１）で金額消した時に、入力したポジションとテーブルハンドリングのポジションがずれてしまう為yoshida150427
//	return tableno;
//---------------
	if( tableno < (int)m_TblIdxAry.GetCount() ){
		return m_TblIdxAry.GetAt(tableno);
	}
	else{
		return ERR;
	}
//---------------

}

//-----------------------------------------------------------------------------
// カラム番号の取得
//-----------------------------------------------------------------------------
int CShinSpecific22::GetColmNo( int index )
{
	int page = index / 63;
	int ofs = index - ( page * 63 + 35 );
	int colmn = ofs % 4;
	return colmn;
}

BEGIN_EVENTSINK_MAP(CShinSpecific22, CSpcBaseDlg)
	ON_EVENT(CShinSpecific22, IDC_ICSDIAG8CTRL1, 1, CShinSpecific22::EditONIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinSpecific22, IDC_ICSDIAG8CTRL1, 2, CShinSpecific22::EditOFFIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinSpecific22, IDC_ICSDIAG8CTRL1, 3, CShinSpecific22::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinSpecific22::EditONIcsdiag8ctrl1(short index)
{

	m_curwnd = IDC_ICSDIAG8CTRL1;//yoshida150316

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

	DIAGRAM_DATA	DD;
	DD.data_imgdata = NULL;
	int				sw = 0;
	diag_getdata( IDC_ICSDIAG8CTRL1, find, (LPUNKNOWN)&DD, CIcsdiagctrl );
	if( DD.data_edit.GetLength() ){
		sw |= 0x01;
	}
	diag_getdata( IDC_ICSDIAG8CTRL1, find+1, (LPUNKNOWN)&DD, CIcsdiagctrl );
	if( m_pArith->l_test(DD.data_val) ){
		sw |= 0x02;
	}
	diag_getdata( IDC_ICSDIAG8CTRL1, find+2, (LPUNKNOWN)&DD, CIcsdiagctrl );
	if( m_pArith->l_test(DD.data_val) ){
		sw |= 0x04;
	}
	diag_getdata( IDC_ICSDIAG8CTRL1, find+3, (LPUNKNOWN)&DD, CIcsdiagctrl );
	if( m_pArith->l_test(DD.data_val) ){
		sw |= 0x08;
	}

	if( !sw ){
		int find = findindex((*m_pSpcListData)->TsTbl.tp_rnum + 1);
		if( (find<0) || (index==find) ) {
			return;
		}
		diag_setposition( IDC_ICSDIAG8CTRL1, find, CIcsdiagctrl );
		return;
	}
	if( GetTableNo(index) == ERR ){
		return;
	}
}

void CShinSpecific22::EditOFFIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	TSREC			TSrec;
	int				TableNo, ColmNo, IndexAdd, PutData, NewLine;
	char			MONY[6]={0};
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;
//--> '15.05.01 INS START
	BOOL			isMonyChg = FALSE;
//<-- '15.05.01 INS END

	if( !m_initfg ){
		return;
	}
//--> '15.03.18 INS START
	if( IsWindowVisible() == FALSE ){
		return;
	}
//<-- '15.03.18 INS END

	int				zrIdx=0;
	if( m_zeiType == ID_ICSSH_SPC_5PER ){
		zrIdx = 0;
	}
	else if( m_zeiType == ID_ICSSH_SPC_8PER ){
		zrIdx = 1;
	}
	else{
		ASSERT( !_T("不正な税率") );
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

	if( ( TableNo = GetTableNo( index ) ) == ERR ){
		return;
	}
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
		TSrec.TsLine = TableNo + 1;
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
//--> '15.05.01 INS START
			if( m_pArith->l_cmp(TSrec.TsDtl[zrIdx].TsTsyu, DiagData.data_val) ){
				isMonyChg = TRUE;
			}
//<-- '15.05.01 INS END
//-- '15.04.25 --
//			memmove( TSrec.TsDtl[zrIdx].TsSsyu, DiagData.data_val, sizeof(TSrec.TsDtl[zrIdx].TsSsyu) );
//---------------
			memmove( TSrec.TsDtl[zrIdx].TsTsyu, DiagData.data_val, sizeof(TSrec.TsDtl[zrIdx].TsTsyu) );
//---------------
			PutData = m_pArith->l_test( DiagData.data_val );
			break;
		case 2 :	// 課税仕入れ等に係る特定収入
//--> '15.05.01 INS START
			if( m_pArith->l_cmp(TSrec.TsDtl[zrIdx].TsSsyu, DiagData.data_val) ){
				isMonyChg = TRUE;
			}
//<-- '15.05.01 INS END
//-- '15.04.25 --
//			memmove( TSrec.TsDtl[zrIdx].TsTsyu, DiagData.data_val, sizeof(TSrec.TsDtl[zrIdx].TsTsyu) );
//---------------
			memmove( TSrec.TsDtl[zrIdx].TsSsyu, DiagData.data_val, sizeof(TSrec.TsDtl[zrIdx].TsSsyu) );
//---------------
			PutData = m_pArith->l_test( DiagData.data_val );
			break;
		case 3 :	// 共通に係る特定収入
//--> '15.05.01 INS START
			if( m_pArith->l_cmp(TSrec.TsDtl[zrIdx].TsKsyu, DiagData.data_val) ){
				isMonyChg = TRUE;
			}
//<-- '15.05.01 INS END
			memmove( TSrec.TsDtl[zrIdx].TsKsyu, DiagData.data_val, sizeof(TSrec.TsDtl[zrIdx].TsKsyu) );
			PutData = m_pArith->l_test( DiagData.data_val );
			break;
		default:	// 
			break;
	}

	// 非課税に係るを算出
	memset( MONY, '\0', sizeof(MONY) );
//-- '15.04.25 --
//	m_pArith->l_add( MONY, TSrec.TsDtl[zrIdx].TsTsyu, TSrec.TsDtl[zrIdx].TsKsyu );
//	m_pArith->l_sub( TSrec.TsDtl[zrIdx].TsHsyu, TSrec.TsDtl[zrIdx].TsSsyu, MONY );
//---------------
	m_pArith->l_add( MONY, TSrec.TsDtl[zrIdx].TsSsyu, TSrec.TsDtl[zrIdx].TsKsyu );
	m_pArith->l_sub( TSrec.TsDtl[zrIdx].TsHsyu, TSrec.TsDtl[zrIdx].TsTsyu, MONY );
//---------------
	// 特定収入合計を計算
	memset( MONY, '\0', sizeof(MONY) );
//-- '15.04.25 --
//	m_pArith->l_add( MONY, TSrec.TsDtl[zrIdx].TsSsyu, TSrec.TsDtl[zrIdx].TsFsyu );
//---------------
	m_pArith->l_add( MONY, TSrec.TsDtl[zrIdx].TsTsyu, TSrec.TsDtl[zrIdx].TsFsyu );
	// 疑問？外特定収入は加算しなくていい？
//---------------

	memmove( TSrec.TsDtl[zrIdx].TsGsyu, MONY, sizeof(TSrec.TsDtl[zrIdx].TsGsyu) );
	if( (PutData==0) && NewLine ){
		return;		// 入力データ無し
	}

	if( (*m_pSpcListData)->m_TblHdl.th_acs( thWritex, &TSrec, &(*m_pSpcListData)->TsTbl ) ){
//-- '15.05.05 --
//		AfxMessageBox("テーブルに書き込み中に障害が発生しました");
//---------------
		ICSMessageBox("テーブルに書き込み中に障害が発生しました");
		return;
//---------------
	}
#ifdef	DEBUG_EX
	char	mny[64] = {0};
	char	buf[64] = {0};
	l_print( mny, TSrec.TsHsyu, "SSS,SSS,SSS,SSS" );
	sprintf_s( buf, "(%2d) 非課税に係る[%s]", index, mny );
	AfxGetMainWnd()->SetWindowText(buf);
#endif
	// 再計算
//-- '15.05.01 --
//	Specific22_Calq();
//---------------
	if( isMonyChg ){
		Specific22_Calq();
	}
//---------------
}

void CShinSpecific22::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
{

	//------->yoshida150311
	/*
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( nChar == 0 ){
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

	
	int pos = 0;
	if( wTerm==VK_UP ){		
		pos = FocusMoveCheck(index , 1);
		m_Specific221.SetPosition(pos);	
	}
	if( wTerm==VK_DOWN ){
		pos = FocusMoveCheck(index , 0);
		m_Specific221.SetPosition(pos);
	}

	if( (wTerm==VK_RETURN) || (wTerm==VK_F3) || (wTerm==VK_RIGHT) ){
		pos = FocusMoveCheck(index , 2);
		m_Specific221.SetPosition(pos);
	}
	else if( (wTerm==VK_TAB) || (wTerm==VK_LEFT) || (wTerm==VK_F2) ){
		pos = FocusMoveCheck(index , 3);
		m_Specific221.SetPosition(pos);
	}
	if( wTerm == VK_DELETE){
//		GetDiagData( index, length, &xdata, -1 );
	}
	
	// 再計算
	Specific22_Calq();

	SetRedrawScrollToIdx(m_Specific221.GetPosition());
	*/
	//<------------------------


	//---------->yoshida150402
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

	if( ( TableNo = GetFocusNo( index ) ) == ERR ){//フォーカス用のポジション取得関数を追加。行の増減をGetTableNoでは操作している為。フォーカスポジションをうまく制御できない。新たに追加したyoshida150427
//	if( ( TableNo = GetTableNo( index ) ) == ERR ){
		return;
	}
	m_TableNo = TableNo;//yoshida150402
	ColmNo = GetColmNo( index );
		
	UINT	flg = 0;

	switch( wTerm ){
		case VK_RETURN: case VK_RIGHT:
			flg = REPLY_NEXT;
			break;
		case VK_TAB://yoshida150402
		case VK_LEFT:
		case VK_F2://yoshida150402
			flg = REPLY_BACK;
			break;
		//------>yoshida150323
		//上下キー対応
		case VK_DOWN:
			index = index + 4;
			if( m_TableNo && ( ( ( m_TableNo + 1 ) % 7 ) == 0 ) ){
				index += DIAG2_INP_STIDX;
			}	
			break;

		case VK_UP:
			index = index - 4;
			if( m_TableNo && ( ( m_TableNo % 7 ) == 0 ) ){
				index -= DIAG2_INP_STIDX;
			}
			break;
		//<----------------------------
//2015.03.05 UPDATE START
		//case VK_DELETE :
		//	// 確定時は全入力不可
		////	if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
		////		break;
		////	}
		//	LineDelete( TableNo );
		//	break;
		//case VK_INSERT :
		//	// 確定時は全入力不可
		////	if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
		////		break;
		////	}
		//	LineInsert( TableNo );
		//	break;

		case VK_DELETE :
			CellClear( index );
			break;
//2015.03.05 UPDATE END
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
	//<---------------------
}

//-----------------------------------------------------------------------------
// テーブル番号の消去
//-----------------------------------------------------------------------------
int CShinSpecific22::LineDelete( int rwno )
{
	TSREC	TSrec;
	int		PutNo;

	if( (*m_pSpcListData)->TsTbl.tp_rnum ){
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
	SpecificDisp22();

	// 最終行へ移動 
	int	i, j, k, l, m, n;
	// 先頭のカラム番号
	i = rwno;
	j = ( i / 7 );
	k = (j * 63) + 35;
	m_Specific221.SetPosition(k);
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
	m_Specific221.SetPosition(k+n);

	return FALSE;
}

//-----------------------------------------------------------------------------
// テーブル番号の挿入
//-----------------------------------------------------------------------------
int CShinSpecific22::LineInsert( int rwno )
{
	TSREC	TSrec;

	int		PutNo;

	if( (*m_pSpcListData)->TsTbl.tp_rnum >= (*m_pSpcListData)->TsTbl.tp_rmax ){//２１０個登録されている状態で１行目でINSERTがきいてたのを修正yoshida150427
//	if( rwno+1 >= (*m_pSpcListData)->TsTbl.tp_rmax ){
		ICSMessageBox( "新規に登録できません。" );
		return( FALSE );
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
	SpecificDisp22();

	// 最終行へ移動 
	int	i, j, k, l, m, n;
	// 先頭のカラム番号
	i = rwno;
	j = ( i / 7 );
	k = (j * 63) + 35;
	m_Specific221.SetPosition(k);
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
	m_Specific221.SetPosition(k+n);

	return( FALSE );
}

void CShinSpecific22::OnBnClickedButton1()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int	TableNo;

	int	index = m_Specific221.GetPosition();
	if( ( TableNo = GetTableNo( index ) ) == ERR ){
		return;
	}
	m_TableNo = TableNo;
	LineDelete( TableNo );
}

void CShinSpecific22::OnBnClickedButton2()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int	TableNo;

	int	index = m_Specific221.GetPosition();
	if( ( TableNo = GetTableNo( index ) ) == ERR ){
		return;
	}
	m_TableNo = TableNo;
	LineInsert( TableNo );
}

void CShinSpecific22::OnButtonF2()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	diag_setprev( IDC_ICSDIAG8CTRL1, CIcsdiagctrl );
}

void CShinSpecific22::OnButtonF3()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	diag_setnext( IDC_ICSDIAG8CTRL1, CIcsdiagctrl );
}

void CShinSpecific22::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
//	CSpcBaseDlg::OnOK();
}

void CShinSpecific22::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
//	CSpcBaseDlg::OnCancel();
}



//------------->yoshida150311

void CShinSpecific22::IndexControl()
{
	int	Cnt = 0;
	while( 1 ){
		if( Specific22[Cnt].Index == 0 ){
			break;
		}
		int	Flg;
		// 確定時は全入力不可
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			Flg = Specific22[Cnt].AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 )	Flg = Specific22[Cnt].UnConnect;
			else								Flg = Specific22[Cnt].Connect;
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

		ATRIB_MOD( IDC_ICSDIAG8CTRL1, Specific22[Cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, Specific22[Cnt].Connect );
		Cnt++;
	}

	//--->yoshida150310
	SetFocusRange();

	m_Specific221.EnableDelete(0);//DELETEキー１回で削除モード設定
	//<---------------

	return;
}


//-----------------------------------------------------------------------------
// 次フォーカスの場所検索（上下左右動作）
//-----------------------------------------------------------------------------
//　戻り値　：　次のフォーカスインデックス
//-----------------------------------------------------------------------------
int CShinSpecific22::FocusMoveCheck(int Index , int Move)
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
		for(i = 0 ; i < FCS_SPECIFIC22CNT ; i++){
			if(Index == FCS_Specific22[i].IDNo){
				break;
			}
		}
		idx = FCS_Specific22[i].IDNo;//現在ポジションセット
	}else if(( Move == 2 )||( Move == 3 )){
		for(i = 0 ; i < FCS_SPECIFIC22CNT ; i++){
			if(Index == FCS_Specific22_LR[i].IDNo){
				break;
			}
		}
		idx = FCS_Specific22_LR[i].IDNo;//現在ポジションセット
	}

	//次ポジション検索
	if(Move == 0){//下方向
		for(j = i + 1 ; j < FCS_SPECIFIC22CNT ; j++){
			m_Specific221.GetAttribute(FCS_Specific22[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Specific22[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 1){//上方向
		for(j = i - 1 ; j > -1 ; j--){
			m_Specific221.GetAttribute(FCS_Specific22[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Specific22[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 2){//右方向
		for(j = i + 1 ; j < FCS_SPECIFIC22CNT ; j++){
			m_Specific221.GetAttribute(FCS_Specific22_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Specific22_LR[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 3){//左方向
		for(j = i - 1 ; j > -1 ; j--){
			m_Specific221.GetAttribute(FCS_Specific22_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Specific22_LR[j].IDNo;
				break;
			}
		}
	}
	return idx;
}


void CShinSpecific22::SetFocusRange()
{

	DIAGRAM_ATTRIBUTE	DA;

	m_FcsUpEnd		= -1;
	m_FcsLeftEnd	= -1;
	m_FcsDownEnd	= -1;
	m_FcsRightEnd	= -1;

	for( int idx = 0; idx < FCS_SPECIFIC22CNT; idx++ ){
		m_Specific221.GetAttribute(FCS_Specific22[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsUpEnd = FCS_Specific22[idx].IDNo;
			break;
		}
	}
	for( int idx = 0; idx < FCS_SPECIFIC22CNT; idx++ ){
		m_Specific221.GetAttribute(FCS_Specific22_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsLeftEnd = FCS_Specific22_LR[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SPECIFIC22CNT-1; idx > -1 ; idx-- ){
		m_Specific221.GetAttribute(FCS_Specific22[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsDownEnd = FCS_Specific22[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SPECIFIC22CNT-1; idx > -1 ; idx-- ){
		m_Specific221.GetAttribute(FCS_Specific22_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsRightEnd = FCS_Specific22_LR[idx].IDNo;
			break;
		}
	}
}



//-----------------------------------------------------------------------------
// 初期ポジションセット 
//-----------------------------------------------------------------------------
// 引数	IsInEnd	：	終了処理中？
//-----------------------------------------------------------------------------
void CShinSpecific22::SetItemPosition( BOOL IsInEnd/*=FALSE*/ )
{
	int index;

	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAG8CTRL1 ) {
		if( (index = m_Specific221.GetPosition()) != -1) {
//-- '15.05.02 --
//			m_Specific221.SetPosition(index);
//			m_Specific221.SetFocus();
//---------------
			if( IsInEnd ){
				// 画面上に完全に矩形領域がある時のみフォーカスを再セット
				RECT	rc221, rc22, rc220;
				m_Specific22.GetRect( 1, (LPUNKNOWN)&rc22 );
				m_Specific221.GetRect( index, (LPUNKNOWN)&rc221 );
				m_Specific220.GetRect( 1, (LPUNKNOWN)&rc220 );

				if( ((rc22.bottom<rc221.top)&&(rc221.bottom<rc220.top)) ){
					m_Specific221.SetPosition(index);
				}
				m_Specific221.SetFocus();
			}
			else{
				m_Specific221.SetPosition(index);
				m_Specific221.SetFocus();
			}
//---------------
			return;
		}
	}

	this->SetFocus();

//--> '15.05.01 INS START
	m_Specific220.SetFocus();
	m_Specific221.SetFocus();
//<-- '15.05.01 INS END

	if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
//-- '15.03.17 --
//		m_Specific221.SetPosition(ID220);
//---------------
		m_Specific221.SetPosition( ID222 );
//---------------
	}
	else{
	}
	SetRedrawScroll( 0 );

}


//<-------------------------------

//2015.03.05 INSERT START
void CShinSpecific22::CellClear( int index )
{
	if( index < DIAG2_INP_STIDX ){
		return;
	}

	diag_clear( IDC_ICSDIAG8CTRL1, index, FALSE, CIcsdiagctrl );

	return;
}
//2015.03.05 INSERT END


//----------->yoshida150427
//---------------------------------------------------------------------------
// テーブル番号の取得(フォーカス移動移動用)
//-----------------------------------------------------------------------------
int CShinSpecific22::GetFocusNo( int index )
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

//<--------------------------