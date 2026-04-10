// ShinSpecific21.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinSpecific21EX2.h"
#include "ShinSpecific21EX2Idx.h"
#include "TaxationListView.h"

//== 定数 ==
const	int	BottomIndex				=	0;	// No3
const	int	StartIndex				=	42;	// No2

const	int	NUM_PER_PAGE			=	77;	// 1ページ毎のデータ数
const	int	DIAG_INP_STIDX			=	42;	// 1ページの入力開始インデックス
const	int	TBL_REC_NUM_PER_PAGE	=	7;	// 1ページ毎のテーブルレコード数（CTblhdl上）
const	int	CLMNUM_PER_ROW			=	5;	// 1行毎の列数

// CShinSpecific21EX2 ダイアログ

IMPLEMENT_DYNAMIC(CShinSpecific21EX2, CSpcBaseDlg)

CShinSpecific21EX2::CShinSpecific21EX2(CWnd* pParent /*=NULL*/)
	: CSpcBaseDlg(CShinSpecific21EX2::IDD, pParent)
	, m_Spage(0)
{

	m_curwnd = -1;

	m_FcsUpEnd		= -1;
	m_FcsLeftEnd	= -1;
	m_FcsDownEnd	= -1;
	m_FcsRightEnd	= -1;

}

CShinSpecific21EX2::~CShinSpecific21EX2()
{
}

void CShinSpecific21EX2::DoDataExchange(CDataExchange* pDX)
{
	CSpcBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL2, m_Specific21);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL3, m_Specific210);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Specific211);
	DDX_Control(pDX, IDC_ICSDISP8CTRL1, m_ICSDisp1);
}


BEGIN_MESSAGE_MAP(CShinSpecific21EX2, CSpcBaseDlg)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON1, &CShinSpecific21EX2::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CShinSpecific21EX2::OnBnClickedButton2)
	ON_COMMAND(ID_BUTTON_F2, &CShinSpecific21EX2::OnButtonF2)
	ON_COMMAND(ID_BUTTON_F3, &CShinSpecific21EX2::OnButtonF3)
END_MESSAGE_MAP()


// CShinSpecific21EX2 メッセージ ハンドラ

BOOL CShinSpecific21EX2::OnInitDialog()
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

BOOL CShinSpecific21EX2::PreTranslateMessage(MSG* pMsg)
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

void CShinSpecific21EX2::OnSize(UINT nType, int cx, int cy)
{
	CSpcBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if ( m_initfg )	{

	}
}

HBRUSH CShinSpecific21EX2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSpcBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
	if( nCtlColor == CTLCOLOR_DLG ){
		hbr = Ibrsh_ShinSpecific;
	}

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

void CShinSpecific21EX2::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
}

//-----------------------------------------------------------------------------
// 再描画
//-----------------------------------------------------------------------------
void CShinSpecific21EX2::sykscroll( int mpos )
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
void CShinSpecific21EX2::SpecificInit21( void )
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

	SetItemPosition();
}

//-----------------------------------------------------------------------------
// 表示
//-----------------------------------------------------------------------------
void CShinSpecific21EX2::SpecificDisp21( void )
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

		DIAGRAM_ATTRIBUTE	DA;
		m_Specific211.GetAttribute( indexEx, (LPUNKNOWN)&DA );
		DA.attr_editattr |= DIAG_EATTR_ZENHAN;
		m_Specific211.SetAttribute( indexEx, (LPUNKNOWN)&DA, TRUE );

		if( rno < (*m_pSpcListData)->TsTbl.tp_rnum ){
			if( (*m_pSpcListData)->m_TblHdl.th_rget( &TSrec, &(*m_pSpcListData)->TsTbl, rno ) ){
				return;
			}

			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+0, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+1, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+2, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+3, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+4, TRUE, CIcsdiagctrl );
			// 科目名称
			DiagData.data_edit =  TSrec.TsName;
			diag_setdata( IDC_ICSDIAG8CTRL1, indexEx, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			// 資産の譲渡等の対価以外の収入
			memmove( DiagData.data_val, TSrec.TsJsyu, sizeof(DiagData.data_val) );
			diag_setdata( IDC_ICSDIAG8CTRL1, indexEx+1, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			// 特定収入合計
			//m_pArith->l_add( sum, TSrec.TsDtl[0].TsGsyu, TSrec.TsDtl[1].TsGsyu );
			//memmove( DiagData.data_val, sum, sizeof(DiagData.data_val) );
			memmove( DiagData.data_val, TSrec.TsDtl[1].TsGsyu, sizeof(DiagData.data_val) );
			diag_setdata( IDC_ICSDIAG8CTRL1, indexEx+2, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			// 課税仕入れに係る特定収入
			if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == TRUE ){
				// 特定収入（8%)
				memmove( DiagData.data_val, TSrec.TsDtl[1].TsTsyu, 6 );
				diag_setdata( IDC_ICSDIAG8CTRL1, indexEx+3, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			}
			else{
				// 比例配分
				memmove( DiagData.data_val, TSrec.TsDtl[1].TsTsyu, 6 );
				diag_setdata( IDC_ICSDIAG8CTRL1, indexEx+3, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			}
			// 不特定収入
			//m_pArith->l_add( sum, TSrec.TsDtl[0].TsFsyu, TSrec.TsDtl[1].TsFsyu );
			//memmove( DiagData.data_val, sum, sizeof(DiagData.data_val) );
			memmove( DiagData.data_val, TSrec.TsDtl[1].TsFsyu, sizeof(DiagData.data_val) );
			diag_setdata( IDC_ICSDIAG8CTRL1, indexEx+4, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		}
		else{
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+0, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+1, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+2, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+3, TRUE, CIcsdiagctrl );
			diag_clear( IDC_ICSDIAG8CTRL1, indexEx+4, TRUE, CIcsdiagctrl );
			if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == TRUE ){
			}
		}

		// 確定時は全入力不可
		switch( (*m_pSnHeadData)->Sn_Sign4&0x81 ){
			case 0x01 :	// ロック
			case 0x81 :	// 非連動でロック
				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+0, DIAG_MDFY_READONLY, CIcsdiagctrl );
				if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+0, 2 );
				}
				else{
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+0, 1 );
				}
				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+1, DIAG_MDFY_READONLY, CIcsdiagctrl );
				if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+1, 2 );
				}
				else{
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+1, 1 );
				}
				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+2, DIAG_MDFY_READONLY, CIcsdiagctrl );
				if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+2, 2 );
				}
				else{
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+2, 1 );
				}
				if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+3, 2 );
				}
				else{
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+3, 1 );
				}
				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+3, DIAG_MDFY_READONLY, CIcsdiagctrl );
				Color_Change( IDC_ICSDIAG8CTRL1, indexEx+4, 1 );
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
				break;
			case 0x80 :	// 非連動
				diag_modify( IDC_ICSDIAG8CTRL1, indexEx+0, DIAG_MDFY_EDIT, CIcsdiagctrl );
				Color_Change( IDC_ICSDIAG8CTRL1, indexEx+0, 0 );
				if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
					diag_modify( IDC_ICSDIAG8CTRL1, indexEx+1, DIAG_MDFY_EDIT, CIcsdiagctrl );
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+1, 0 );
					diag_modify( IDC_ICSDIAG8CTRL1, indexEx+2, DIAG_MDFY_EDIT, CIcsdiagctrl );
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+2, 0 );
					diag_modify( IDC_ICSDIAG8CTRL1, indexEx+3, DIAG_MDFY_EDIT, CIcsdiagctrl );
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+3, 0 );
				}
				else{
					diag_modify( IDC_ICSDIAG8CTRL1, indexEx+1, DIAG_MDFY_EDIT, CIcsdiagctrl );
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+1, 0 );
					diag_modify( IDC_ICSDIAG8CTRL1, indexEx+2, DIAG_MDFY_EDIT, CIcsdiagctrl );
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+2, 0 );
					diag_modify( IDC_ICSDIAG8CTRL1, indexEx+3, DIAG_MDFY_EDIT, CIcsdiagctrl );
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+3, 0 );
					diag_modify( IDC_ICSDIAG8CTRL1, indexEx+4, DIAG_MDFY_READONLY, CIcsdiagctrl );
					Color_Change( IDC_ICSDIAG8CTRL1, indexEx+4, 1 );
				}
				break;
			default   :
				break;
		}

		index += 6;
		indexEx += 5;
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
void CShinSpecific21EX2::Specific21_Calq( void )
{
	TSREC			TSrec;
	int				rno;
	char			MONY1_8per[MONY_BUF_SIZE]={0}, MONY2_8per[MONY_BUF_SIZE]={0}, MONY3_8per[MONY_BUF_SIZE]={0};
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

		// 課税仕入等にのみ係る特定収入
		if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == TRUE ){					// 個別対応
			m_pArith->l_add( MONY2_8per, MONY2_8per, TSrec.TsDtl[1].TsTsyu );
		}
		else{																	// 比例配分
			m_pArith->l_add( MONY2_8per, MONY2_8per, TSrec.TsDtl[1].TsTsyu );
		}

		// 不特定収入
		m_pArith->l_add( MONY3_8per, MONY3_8per, TSrec.TsDtl[1].TsFsyu );
		rno++;
	}

	// 資産の譲渡等
	memmove( DiagData.data_val, MONY0_sum, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL3, 6, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	
	// 特定収入合計
	memmove( DiagData.data_val, MONY1_8per, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL3, 7, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 課税仕入等にのみ係る特定収入(8%)
	memmove( DiagData.data_val, MONY2_8per, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL3, 8, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// 不特定収入
	memmove( DiagData.data_val, MONY3_8per, sizeof(DiagData.data_val) );
	diag_setdata( IDC_ICSDIAG8CTRL3, 9, (struct IUnknown *)&DiagData, CIcsdiagctrl );


	// 特定収入合計
	memset( (*m_pSpcListData)->stval.SpGsyu, '\0', sizeof((*m_pSpcListData)->stval.SpGsyu) );
	memmove( (*m_pSpcListData)->stval.SpGsyu, MONY1_8per, sizeof((*m_pSpcListData)->stval.SpGsyu) );	
	// 課税仕入等にのみ係る特定収入
	memset( (*m_pSpcListData)->stval.SpTsyu, '\0', sizeof((*m_pSpcListData)->stval.SpTsyu) );
	memmove( (*m_pSpcListData)->stval.SpTsyu, MONY2_sum, sizeof((*m_pSpcListData)->stval.SpTsyu) );	

	memset( (*m_pSpcListData)->stval.SpTsyu63, '\0', sizeof((*m_pSpcListData)->stval.SpTsyu63) );
	memmove( (*m_pSpcListData)->stval.SpTsyu63, MONY2_8per, sizeof((*m_pSpcListData)->stval.SpTsyu63) );	

	// 不特定収入
	memset( (*m_pSpcListData)->stval.SpFsyu, '\0', sizeof((*m_pSpcListData)->stval.SpFsyu) );
	memmove( (*m_pSpcListData)->stval.SpFsyu, MONY3_8per, sizeof((*m_pSpcListData)->stval.SpFsyu) );

	// 特定収入に係る課税仕入れ等の税額計算
	unsigned char	svSn_Sign2 = (*m_pSnHeadData)->Sn_Sign2;
	(*m_pSpcListData)->SPassOffCalqMain( *m_pSnHeadData );
	BOOL	IsChgDoSpc = FALSE;
	if( (svSn_Sign2&0x02) != ((*m_pSnHeadData)->Sn_Sign2&0x02) ){
		IsChgDoSpc = TRUE;
	}
	((CTaxationListView*)m_pParent)->LinkHonpyoData(IsChgDoSpc);	
	DispRegCount();

	m_Specific211.Refresh();
}

//-----------------------------------------------------------------------------
// 登録数表示('15.05.05)
//-----------------------------------------------------------------------------
void CShinSpecific21EX2::DispRegCount()
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
int CShinSpecific21EX2::GetTableNo( int nowIdx )
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
int CShinSpecific21EX2::GetColmNo( int nowIdx )
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
int findindexEX2( int line )
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

BEGIN_EVENTSINK_MAP(CShinSpecific21EX2, CSpcBaseDlg)
	ON_EVENT(CShinSpecific21EX2, IDC_ICSDIAG8CTRL1, 1, CShinSpecific21EX2::EditONIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinSpecific21EX2, IDC_ICSDIAG8CTRL1, 2, CShinSpecific21EX2::EditOFFIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinSpecific21EX2, IDC_ICSDIAG8CTRL1, 3, CShinSpecific21EX2::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinSpecific21EX2::EditONIcsdiag8ctrl1(short index)
{

	m_curwnd = IDC_ICSDIAG8CTRL1;

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( !m_initfg ){
		return;
	}

	int line = GetTableNo( index );
	if( line < (*m_pSpcListData)->TsTbl.tp_rnum ){
		return;
	}

	int find = findindexEX2(line);
	if( find < 0 ){
		return;
	}

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

	if( !sw ){
		int find = findindexEX2((*m_pSpcListData)->TsTbl.tp_rnum + 1);
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

void CShinSpecific21EX2::EditOFFIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
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
			PutData = m_pArith->l_cmp( DiagData.data_val, TSrec.TsJsyu );
			memmove( TSrec.TsJsyu, DiagData.data_val, sizeof(TSrec.TsJsyu) );
			if( PutData ){
				isMonyChg = TRUE;
			}
			
			break;
		case 2 :	// 特定収入
			if( (*m_pSnHeadData)->SVmzsw == 1 ){	// 経過税率あり
				char	sum[MONY_BUF_SIZE]={0};
				char	dif[MONY_BUF_SIZE]={0};
				//m_pArith->l_add( sum, TSrec.TsDtl[0].TsGsyu, TSrec.TsDtl[1].TsGsyu );

				//m_pArith->l_sub( dif, DiagData.data_val, sum );
				m_pArith->l_sub( dif, DiagData.data_val, TSrec.TsDtl[1].TsGsyu );
				//PutData = m_pArith->l_cmp( DiagData.data_val, sum );
				PutData = m_pArith->l_cmp( DiagData.data_val, TSrec.TsDtl[1].TsGsyu );
				
				m_pArith->l_add( TSrec.TsDtl[1].TsGsyu, TSrec.TsDtl[1].TsGsyu, dif );
			}
			else{									// 経過税率なし
				PutData = m_pArith->l_cmp( DiagData.data_val, TSrec.TsDtl[1].TsGsyu );
				memmove( TSrec.TsDtl[1].TsGsyu, DiagData.data_val, sizeof(TSrec.TsDtl[1].TsGsyu) );
			}
			IndexAdd = ( index + 2 );
			if( PutData ){
				isMonyChg = TRUE;
			}
			break;
		case 3 :	// 課税仕入れ等に係る特定収入(8%)
			if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == TRUE ){
				PutData = m_pArith->l_cmp( DiagData.data_val, TSrec.TsDtl[1].TsTsyu );
				memmove( TSrec.TsDtl[1].TsTsyu, DiagData.data_val, sizeof(TSrec.TsDtl[1].TsTsyu) );

				// 非課税に係るを算出
				memset( MONY, '\0', sizeof(MONY) );
				//非課税の計算を行う
				m_pArith->l_add( MONY, TSrec.TsDtl[1].TsSsyu, TSrec.TsDtl[1].TsKsyu );
				m_pArith->l_sub( TSrec.TsDtl[1].TsHsyu, TSrec.TsDtl[1].TsTsyu, MONY );
			}
			else{
				PutData = m_pArith->l_cmp( DiagData.data_val, TSrec.TsDtl[1].TsTsyu );
				memmove( TSrec.TsDtl[1].TsTsyu, DiagData.data_val, sizeof(TSrec.TsDtl[1].TsTsyu) );

				// 非課税に係るを算出
				memset( MONY, '\0', sizeof(MONY) );
				//非課税の計算を行う
				m_pArith->l_add( MONY, TSrec.TsDtl[1].TsSsyu, TSrec.TsDtl[1].TsKsyu );
				m_pArith->l_sub( TSrec.TsDtl[1].TsHsyu, TSrec.TsDtl[1].TsTsyu, MONY );
			}
			IndexAdd = ( index + 1 );
			if( PutData ){
				isMonyChg = TRUE;
			}
			break;
		default:	// 
			break;
	}

	if( IndexAdd ){
		memset( MONY, '\0', sizeof(MONY) );
		if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == TRUE ){
			if( (ColmNo==2) || (ColmNo==3) ){
				m_pArith->l_sub( MONY, TSrec.TsDtl[1].TsGsyu, TSrec.TsDtl[1].TsTsyu );
				memmove( TSrec.TsDtl[1].TsFsyu, MONY, sizeof(TSrec.TsDtl[1].TsFsyu) );
			}
		}
		else{
			if( (ColmNo==2) || (ColmNo==3) ){
				m_pArith->l_sub( MONY, TSrec.TsDtl[1].TsGsyu, TSrec.TsDtl[1].TsTsyu );
				memmove( TSrec.TsDtl[1].TsFsyu, MONY, sizeof(TSrec.TsDtl[1].TsFsyu) );
			}
		}

		char	sum[MONY_BUF_SIZE]={0};
		memmove( DiagData.data_val, TSrec.TsDtl[1].TsFsyu, sizeof(DiagData.data_val) );

		diag_setdata( IDC_ICSDIAG8CTRL1, IndexAdd, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	}
	if( (PutData==0) && NewLine ){
		return;		// 入力データ無し
	}

	if( (*m_pSpcListData)->m_TblHdl.th_acs( thWritex, &TSrec, &(*m_pSpcListData)->TsTbl ) )	{
		ICSMessageBox("テーブルに書き込み中に障害が発生しました");
		return;
	}

	// 再計算
	if( isMonyChg ){
		Specific21_Calq();
	}
	else{
		DispRegCount();
	}
}

void CShinSpecific21EX2::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
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

	

	switch( wTerm ){
		case VK_RETURN: case VK_RIGHT:
			flg = REPLY_NEXT;
			break;
		case VK_TAB:
		case VK_LEFT:
		case VK_F2:
			flg = REPLY_BACK;
			break;

		//上下キー対応
		case VK_DOWN:
			index = index + 5;
			if( m_TableNo && (((m_TableNo+1)%7)==0) ){
				index += DIAG_INP_STIDX;
			}	
			break;

		case VK_UP:
			index = index - 5;
			if( m_TableNo && ( ( m_TableNo % 7 ) == 0 ) ){
				index -= DIAG_INP_STIDX;
			}
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
	
	//<----------------------------------
}

//-----------------------------------------------------------------------------
// テーブル番号の消去
//-----------------------------------------------------------------------------
int CShinSpecific21EX2::LineDelete( int rwno )
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
int CShinSpecific21EX2::LineInsert( int rwno )
{
	TSREC	TSrec;

	int		PutNo;

	if( (*m_pSpcListData)->TsTbl.tp_rnum >= (*m_pSpcListData)->TsTbl.tp_rmax ){//２１０個登録されている状態で１行目でINSERTがきいてたのを修正
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

void CShinSpecific21EX2::OnBnClickedButton1()
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

void CShinSpecific21EX2::OnBnClickedButton2()
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

void CShinSpecific21EX2::OnButtonF2()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	diag_setprev( IDC_ICSDIAG8CTRL1, CIcsdiagctrl );
}

void CShinSpecific21EX2::OnButtonF3()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	diag_setnext( IDC_ICSDIAG8CTRL1, CIcsdiagctrl );
}

void CShinSpecific21EX2::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
//	CSpcBaseDlg::OnOK();
}

void CShinSpecific21EX2::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
//	CSpcBaseDlg::OnCancel();
}

void CShinSpecific21EX2::IndexControl()
{
	int	Cnt = 0;
	while( 1 ){
		if( Specific21Ex2[Cnt].Index == 0 ){
			break;
		}
		int	Flg;
		// 確定時は全入力不可
		if( (*m_pSnHeadData)->Sn_Sign4&0x01 ){
			Flg = Specific21Ex2[Cnt].AllUnConnect;
		}
		else{
			if( (*m_pSnHeadData)->Sn_Sign4&0x80 )	Flg = Specific21Ex2[Cnt].UnConnect;
			else								Flg = Specific21Ex2[Cnt].Connect;
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

		ATRIB_MOD( IDC_ICSDIAG8CTRL1, Specific21Ex2[Cnt].Index, Flg, (*m_pSnHeadData)->Sn_Sign4, Specific21Ex2[Cnt].Connect );
		Cnt++;
	}

	SetFocusRange();

	m_Specific211.EnableDelete(0);//DELETEキー１回で削除モード設定

	return;
}


//-----------------------------------------------------------------------------
// 次フォーカスの場所検索（上下左右動作）
//-----------------------------------------------------------------------------
//　戻り値　：　次のフォーカスインデックス
//-----------------------------------------------------------------------------
int CShinSpecific21EX2::FocusMoveCheck(int Index , int Move)
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
			if(Index == FCS_Specific21Ex2[i].IDNo){
				break;
			}
		}
		idx = FCS_Specific21Ex2[i].IDNo;//現在ポジションセット
	}else if(( Move == 2 )||( Move == 3 )){
		for(i = 0 ; i < FCS_SPECIFIC21CNT ; i++){
			if(Index == FCS_Specific21Ex2_LR[i].IDNo){
				break;
			}
		}
		idx = FCS_Specific21Ex2_LR[i].IDNo;//現在ポジションセット
	}

	//次ポジション検索
	if(Move == 0){//下方向
		for(j = i + 1 ; j < FCS_SPECIFIC21CNT ; j++){
			m_Specific211.GetAttribute(FCS_Specific21Ex2[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Specific21Ex2[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 1){//上方向
		for(j = i - 1 ; j > -1 ; j--){
			m_Specific211.GetAttribute(FCS_Specific21Ex2[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Specific21Ex2[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 2){//右方向
		for(j = i + 1 ; j < FCS_SPECIFIC21CNT ; j++){
			m_Specific211.GetAttribute(FCS_Specific21Ex2_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Specific21Ex2_LR[j].IDNo;
				break;
			}
		}
	}
	else if(Move == 3){//左方向
		for(j = i - 1 ; j > -1 ; j--){
			m_Specific211.GetAttribute(FCS_Specific21Ex2_LR[j].IDNo ,  (LPUNKNOWN)&DA);
			if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
				idx = FCS_Specific21Ex2_LR[j].IDNo;
				break;
			}
		}
	}
	return idx;
}


void CShinSpecific21EX2::SetFocusRange()
{

	DIAGRAM_ATTRIBUTE	DA;

	m_FcsUpEnd		= -1;
	m_FcsLeftEnd	= -1;
	m_FcsDownEnd	= -1;
	m_FcsRightEnd	= -1;

	for( int idx = 0; idx < FCS_SPECIFIC21CNT; idx++ ){
		m_Specific211.GetAttribute(FCS_Specific21Ex2[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsUpEnd = FCS_Specific21Ex2[idx].IDNo;
			break;
		}
	}
	for( int idx = 0; idx < FCS_SPECIFIC21CNT; idx++ ){
		m_Specific211.GetAttribute(FCS_Specific21Ex2_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsLeftEnd = FCS_Specific21Ex2_LR[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SPECIFIC21CNT-1; idx > -1 ; idx-- ){
		m_Specific211.GetAttribute(FCS_Specific21Ex2[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsDownEnd = FCS_Specific21Ex2[idx].IDNo;
			break;
		}
	}
	for( int idx = FCS_SPECIFIC21CNT-1; idx > -1 ; idx-- ){
		m_Specific211.GetAttribute(FCS_Specific21Ex2_LR[idx].IDNo ,  (LPUNKNOWN)&DA);
		if(DA.attr_bcolor == BC_CREAM || DA.attr_bcolor == BC_BLUE){
			m_FcsRightEnd = FCS_Specific21Ex2_LR[idx].IDNo;
			break;
		}
	}
}



//-----------------------------------------------------------------------------
// 初期ポジションセット 
//-----------------------------------------------------------------------------
// 引数	IsInEnd	：	終了処理中？
//-----------------------------------------------------------------------------
void CShinSpecific21EX2::SetItemPosition( BOOL IsInEnd/*=FALSE*/ )
{
	int index;

	// 前回ポジションの復元
	if( m_curwnd == IDC_ICSDIAG8CTRL1 ) {
		if( (index = m_Specific211.GetPosition()) != -1) {

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
			return;
		}
	}

	this->SetFocus();

	m_Specific210.SetFocus();
	m_Specific211.SetFocus();

	if( (*m_pSnHeadData)->Sn_Sign4&0x80 ){
		m_Specific211.SetPosition(ID210);
	}
	else{
	}
	SetRedrawScroll( 0 );

}
