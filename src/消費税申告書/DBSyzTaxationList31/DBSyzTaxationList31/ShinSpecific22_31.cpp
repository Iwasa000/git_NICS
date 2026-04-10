//yokono
// ShinSpecific22_31.cpp : 実装ファイル
//

#include "stdafx.h"
#include "ShinSpecific22_31.h"
#include "ShinSpecific22_31Idx.h"
#include "TaxationList31View.h"


// CShinSpecific22_31 ダイアログ

//== 定数 ==
const	int	BottomIndex = 0;	// No3
const	int	StartIndex = 56;	// No2

const	int	DIAG2_INP_STIDX = 56;	// 1ページの入力開始インデックス

const	int	NUM_PER_PAGE			=	105;	// 1ページ毎のデータ数
const	int	DIAG_INP_STIDX			=	56;		// 1ページの入力開始インデックス
const	int	TBL_REC_NUM_PER_PAGE	=	7;		// 1ページ毎のテーブルレコード数（CTblhdl上）
const	int	CLMNUM_PER_ROW			=	7;		// 1行毎の列数

IMPLEMENT_DYNAMIC(CShinSpecific22_31, CSpcBaseDlg)

CShinSpecific22_31::CShinSpecific22_31(CWnd* pParent /*=nullptr*/)
	: CSpcBaseDlg(IDD_DIALOG_K22_31, pParent)
{
	m_curwnd = -1;

	m_FcsUpEnd		= -1;
	m_FcsLeftEnd	= -1;
	m_FcsDownEnd	= -1;
	m_FcsRightEnd	= -1;
}

CShinSpecific22_31::~CShinSpecific22_31()
{
}

void CShinSpecific22_31::DoDataExchange(CDataExchange* pDX)
{
	CSpcBaseDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL2, m_Specific22);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL1, m_Specific221);
	DDX_Control(pDX, IDC_ICSDIAG8CTRL3, m_Specific220);
	DDX_Control(pDX, IDC_ICSDISP8CTRL1, m_ICSDisp1);
}


BEGIN_MESSAGE_MAP(CShinSpecific22_31, CSpcBaseDlg)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_BUTTON_F1, &CShinSpecific22_31::OnButtonF1)
	ON_COMMAND(ID_BUTTON_F2, &CShinSpecific22_31::OnButtonF2)
END_MESSAGE_MAP()


// CShinSpecific22_31 メッセージ ハンドラー


BOOL CShinSpecific22_31::OnInitDialog()
{
	CSpcBaseDlg::OnInitDialog();

	// TODO: ここに初期化を追加してください
	//初期設定
	SpecificInit22() ;

	m_Specific221.EnableDelete(0) ;

	m_initfg = 1 ;

	CSpcBaseDlg::OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CShinSpecific22_31::SpecificInit22()
{
	// 倍長演算指定
	m_pArith->l_defn( 0x16 );

	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
		m_Spage = 30;
		m_Specific221.SetScPageCnt( ( short )m_Spage );
	}

	IndexControl() ;

	SpecificDisp22();

	SetItemPosition() ;

}

//-----------------------------------------------------------------------------
// 表示
//-----------------------------------------------------------------------------
void CShinSpecific22_31::SpecificDisp22()
{
	TSREC			TSrec;
	int				rno, max, index, indexEx, page, lno;
	DIAGRAM_DATA   DiagData;
	DiagData.data_imgdata = NULL;

	m_TblIdxAry.RemoveAll();

	max = 210;
	index = 1;
	indexEx = DIAG_INP_STIDX;
	page = 0;
	for ( rno = 0, lno = 0; lno != max; ) {

		char buf[128] = { 0 };
		sprintf_s ( buf, sizeof ( buf ), _T( "%d" ), lno + 1 );
		DiagData.data_disp =  buf;
		diag_setdata ( IDC_ICSDIAG8CTRL1, index, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

		if ( rno < ( *m_pSpcListData )->TsTbl.tp_rnum ) {
			if ( ( *m_pSpcListData )->m_TblHdl.th_rget ( &TSrec, &( *m_pSpcListData )->TsTbl, rno ) ) {
				return;
			}

			if( IsNowZrtData( &TSrec ) == FALSE ){
				rno++;
				continue;
			}

			m_TblIdxAry.Add ( rno );

			// 科目名称
			DiagData.data_edit = TSrec.TsName;
			diag_setdata ( IDC_ICSDIAG8CTRL1, indexEx, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

			// 特定収入（課税に係る＋非課税に係る＋共通に係る）★計算表の特殊合計用（個別対応専用）
			memmove ( DiagData.data_val, TSrec.TsDtl[0].TsTsyu, sizeof ( DiagData.data_val ) );
			diag_setdata ( IDC_ICSDIAG8CTRL1, indexEx + 1, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

			// 課税売上にのみ係る
			memmove ( DiagData.data_val, TSrec.TsDtl[0].TsSsyu, sizeof ( DiagData.data_val ) );
			diag_setdata ( IDC_ICSDIAG8CTRL1, indexEx + 2, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

			// 共通に係る
			memmove ( DiagData.data_val, TSrec.TsDtl[0].TsKsyu, sizeof ( DiagData.data_val ) );
			diag_setdata ( IDC_ICSDIAG8CTRL1, indexEx + 3, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

			// 特定収入（課税に係る＋非課税に係る＋共通に係る）★計算表の特殊合計用（個別対応専用）
			memmove ( DiagData.data_val, TSrec.TsDtl[1].TsTsyu, sizeof ( DiagData.data_val ) );
			diag_setdata ( IDC_ICSDIAG8CTRL1, indexEx + 4, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

			// 課税売上にのみ係る
			memmove ( DiagData.data_val, TSrec.TsDtl[1].TsSsyu, sizeof ( DiagData.data_val ) );
			diag_setdata ( IDC_ICSDIAG8CTRL1, indexEx + 5, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

			// 共通に係る
			memmove ( DiagData.data_val, TSrec.TsDtl[1].TsKsyu, sizeof ( DiagData.data_val ) );
			diag_setdata ( IDC_ICSDIAG8CTRL1, indexEx + 6, ( struct IUnknown* )&DiagData, CIcsdiagctrl );
		}
		else{
			diag_clear ( IDC_ICSDIAG8CTRL1, indexEx + 0, TRUE, CIcsdiagctrl );
			diag_clear ( IDC_ICSDIAG8CTRL1, indexEx + 1, TRUE, CIcsdiagctrl );
			diag_clear ( IDC_ICSDIAG8CTRL1, indexEx + 2, TRUE, CIcsdiagctrl );
			diag_clear ( IDC_ICSDIAG8CTRL1, indexEx + 3, TRUE, CIcsdiagctrl );
			diag_clear ( IDC_ICSDIAG8CTRL1, indexEx + 4, TRUE, CIcsdiagctrl );
			diag_clear ( IDC_ICSDIAG8CTRL1, indexEx + 5, TRUE, CIcsdiagctrl );
			diag_clear ( IDC_ICSDIAG8CTRL1, indexEx + 6, TRUE, CIcsdiagctrl );
		}

		// 確定時は全入力不可
		if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01) || ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) == 0x00 ) ) {
			int	flg = 1;
			if ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) && ( ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) == 0x80 ) && ( lno < ( int )m_TblIdxAry.GetCount() ) ) {
				flg = 2;
			}

			diag_modify ( IDC_ICSDIAG8CTRL1, indexEx + 0, DIAG_MDFY_READONLY, CIcsdiagctrl );
			diag_modify ( IDC_ICSDIAG8CTRL1, indexEx + 1, DIAG_MDFY_READONLY, CIcsdiagctrl );
			diag_modify ( IDC_ICSDIAG8CTRL1, indexEx + 2, DIAG_MDFY_READONLY, CIcsdiagctrl );
			Color_Change ( IDC_ICSDIAG8CTRL1, indexEx + 2, flg );
			diag_modify ( IDC_ICSDIAG8CTRL1, indexEx + 3, DIAG_MDFY_READONLY, CIcsdiagctrl );
			Color_Change ( IDC_ICSDIAG8CTRL1, indexEx + 3, flg );
			diag_modify ( IDC_ICSDIAG8CTRL1, indexEx + 4, DIAG_MDFY_READONLY, CIcsdiagctrl );
			diag_modify ( IDC_ICSDIAG8CTRL1, indexEx + 5, DIAG_MDFY_READONLY, CIcsdiagctrl );
			Color_Change ( IDC_ICSDIAG8CTRL1, indexEx + 5, flg );
			diag_modify ( IDC_ICSDIAG8CTRL1, indexEx + 6, DIAG_MDFY_READONLY, CIcsdiagctrl );
			Color_Change ( IDC_ICSDIAG8CTRL1, indexEx + 6, flg );
		}
		//計算表２（１）で登録している分まで登録可とする。それ以降は入力不可
		else if ( lno >= ( int )m_TblIdxAry.GetCount() ) {
			int	flg = 1;
			diag_modify ( IDC_ICSDIAG8CTRL1, indexEx + 0, DIAG_MDFY_READONLY, CIcsdiagctrl );
			diag_modify ( IDC_ICSDIAG8CTRL1, indexEx + 1, DIAG_MDFY_READONLY, CIcsdiagctrl );
			diag_modify ( IDC_ICSDIAG8CTRL1, indexEx + 2, DIAG_MDFY_READONLY, CIcsdiagctrl );
			Color_Change ( IDC_ICSDIAG8CTRL1, indexEx + 2, flg );
			diag_modify ( IDC_ICSDIAG8CTRL1, indexEx + 3, DIAG_MDFY_READONLY, CIcsdiagctrl );
			Color_Change ( IDC_ICSDIAG8CTRL1, indexEx + 3, flg );
			diag_modify ( IDC_ICSDIAG8CTRL1, indexEx + 4, DIAG_MDFY_READONLY, CIcsdiagctrl );
			diag_modify ( IDC_ICSDIAG8CTRL1, indexEx + 5, DIAG_MDFY_READONLY, CIcsdiagctrl );
			Color_Change ( IDC_ICSDIAG8CTRL1, indexEx + 5, flg );
			diag_modify ( IDC_ICSDIAG8CTRL1, indexEx + 6, DIAG_MDFY_READONLY, CIcsdiagctrl );
			Color_Change ( IDC_ICSDIAG8CTRL1, indexEx + 6, flg );
		}
		else {
			Color_Change ( IDC_ICSDIAG8CTRL1, indexEx + 2, 0 );
			Color_Change ( IDC_ICSDIAG8CTRL1, indexEx + 3, 0 );
			Color_Change ( IDC_ICSDIAG8CTRL1, indexEx + 5, 0 );
			Color_Change ( IDC_ICSDIAG8CTRL1, indexEx + 6, 0 );
		}

		index += 8;
		indexEx += 7;
		if ( lno && ( ( ( lno + 1 ) % 7 ) == 0 ) ) {
			page++;
			index = ( NUM_PER_PAGE * page ) + 1;
			indexEx = ( NUM_PER_PAGE * page ) + DIAG_INP_STIDX;
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
void CShinSpecific22_31::Specific22_Calq( void )
{
	TSREC			TSrec = { 0 };
	int				rno = 0;
	char			MONY01[6] = { 0 }, MONY02[6] = { 0 }, MONY03[6] = { 0 }, 
					MONY04[6] = { 0 }, MONY05[6] = { 0 };
	char			MONY11[6] = { 0 }, MONY12[6] = { 0 }, MONY13[6] = { 0 },
					MONY14[6] = { 0 }, MONY15[6] = { 0 };

	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	rno = 0;
	while ( 1 ) {
		if ( rno == ( *m_pSpcListData )->TsTbl.tp_rnum ) {
			break;
		}

		if ( ( *m_pSpcListData )->m_TblHdl.th_rget ( &TSrec, &( *m_pSpcListData )->TsTbl, rno ) ) {
			break;
		}

		m_pArith->l_add ( MONY01, MONY01, TSrec.TsDtl[0].TsTsyu );
		m_pArith->l_add ( MONY02, MONY02, TSrec.TsDtl[0].TsSsyu );
		m_pArith->l_add ( MONY03, MONY03, TSrec.TsDtl[0].TsKsyu );
		m_pArith->l_add ( MONY04, MONY04, TSrec.TsDtl[0].TsFsyu );
		m_pArith->l_add ( MONY05, MONY05, TSrec.TsDtl[0].TsHsyu );
		m_pArith->l_add ( MONY11, MONY11, TSrec.TsDtl[1].TsTsyu );
		m_pArith->l_add ( MONY12, MONY12, TSrec.TsDtl[1].TsSsyu );
		m_pArith->l_add ( MONY13, MONY13, TSrec.TsDtl[1].TsKsyu );
		m_pArith->l_add ( MONY14, MONY14, TSrec.TsDtl[1].TsFsyu );
		m_pArith->l_add ( MONY15, MONY15, TSrec.TsDtl[1].TsHsyu );
		rno++;
	}	

	// 特定収入（課税に係る＋非課税に係る＋共通に係る）★計算表の特殊合計用（個別対応専用）
	memmove ( DiagData.data_val, MONY01, sizeof ( DiagData.data_val ) );
	diag_setdata ( IDC_ICSDIAG8CTRL3, 8, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	// 課税売上に係る特定収入
	memmove ( DiagData.data_val, MONY02, sizeof ( DiagData.data_val ) );
	diag_setdata ( IDC_ICSDIAG8CTRL3, 9, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	// 共通に係る特定収入
	memmove ( DiagData.data_val, MONY03, sizeof ( DiagData.data_val ) );
	diag_setdata ( IDC_ICSDIAG8CTRL3, 10, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	// 特定収入（課税に係る＋非課税に係る＋共通に係る）★計算表の特殊合計用（個別対応専用）
	memmove ( DiagData.data_val, MONY11, sizeof ( DiagData.data_val ) );
	diag_setdata ( IDC_ICSDIAG8CTRL3, 11, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	// 課税売上に係る特定収入
	memmove ( DiagData.data_val, MONY12, sizeof ( DiagData.data_val ) );
	diag_setdata ( IDC_ICSDIAG8CTRL3, 12, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	// 共通に係る特定収入
	memmove ( DiagData.data_val, MONY13, sizeof ( DiagData.data_val ) );
	diag_setdata ( IDC_ICSDIAG8CTRL3, 13, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	// 特定収入	 課税売上にのみに係る
	memmove ( ( *m_pSpcListData )->stval.SpSsyu4, MONY02, sizeof ( ( *m_pSpcListData )->stval.SpSsyu4 ) );

	// 特定収入	 非課税売上にのみに係る
	memmove ( ( *m_pSpcListData )->stval.SpHsyu4, MONY05, sizeof ( ( *m_pSpcListData )->stval.SpHsyu4 ) );

	// 不特定収入
	memmove ( ( *m_pSpcListData )->stval.SpFsyu4, MONY04, sizeof ( ( *m_pSpcListData )->stval.SpFsyu4 ) );

	// 特定収入	 共通に係る
	memmove ( ( *m_pSpcListData )->stval.SpKsyu4, MONY03, sizeof ( ( *m_pSpcListData )->stval.SpKsyu4 ) );

	// 特定収入合計
	m_pArith->l_add ( ( *m_pSpcListData )->stval.SpGsyu4, MONY01, MONY04 );

	// 特定収入	 課税売上にのみに係る
	memmove ( ( *m_pSpcListData )->stval.SpSsyu63, MONY12, sizeof ( ( *m_pSpcListData )->stval.SpSsyu63 ) );

	// 特定収入	 非課税売上にのみに係る
	memmove ( ( *m_pSpcListData )->stval.SpHsyu63, MONY15, sizeof ( ( *m_pSpcListData )->stval.SpHsyu63 ) );

	// 不特定収入
	memmove ( ( *m_pSpcListData )->stval.SpFsyu63, MONY14, sizeof ( ( *m_pSpcListData )->stval.SpFsyu63 ) );

	// 特定収入	 共通に係る
	memmove ( ( *m_pSpcListData )->stval.SpKsyu63, MONY13, sizeof ( ( *m_pSpcListData )->stval.SpKsyu63 ) );

	// 特定収入合計
	m_pArith->l_add ( ( *m_pSpcListData )->stval.SpGsyu63, MONY11, MONY14 );

	// 特定収入に係る課税仕入れ等の税額計算
	unsigned char	svSn_Sign2 = ( *m_pSnHeadData )->Sn_Sign2;
	( *m_pSpcListData )->SPassOffCalqMain ( *m_pSnHeadData );
	BOOL	IsChgDoSpc = FALSE;
	if ( ( svSn_Sign2 & 0x02 ) != ( ( *m_pSnHeadData )->Sn_Sign2 & 0x02 ) ) {
		IsChgDoSpc = TRUE;
	}

	( ( CTaxationList31View* )m_pParent )->LinkHonpyoData ( IsChgDoSpc );

	char	buf[128]={0};
	sprintf_s ( buf, _T( "登録状況\n%3d/%3d" ),  ( int )m_TblIdxAry.GetCount(), 210 );
	c_cls( IDC_ICSDISP8CTRL1, CIcsdisp8ctrl1 );
	puts( IDC_ICSDISP8CTRL1, buf, CIcsdisp8ctrl1 );

	m_Specific221.Refresh();

}

//-----------------------------------------------------------------------------
// テーブル番号の取得
//-----------------------------------------------------------------------------
int CShinSpecific22_31::GetTableNo ( int index )
{
	int	i= 0, j = 0;
	int pageno = 0, tableno = 0, indexno = 0;

	// 該当ページ番号の取得
	pageno = ( index / NUM_PER_PAGE );

	// 該当ページ先頭テーブル番号
	tableno = ( pageno * TBL_REC_NUM_PER_PAGE );

	// 該当ページ先頭
	indexno = ( pageno * NUM_PER_PAGE ) + DIAG_INP_STIDX;

	i = index - indexno;
	j = ( i / CLMNUM_PER_ROW );
	tableno += j;

	if ( tableno < ( int )m_TblIdxAry.GetCount() ) {
		return m_TblIdxAry.GetAt ( tableno );
	}
	else {
		return ERR;
	}

}

int CShinSpecific22_31::findindex (int line )
{
	int find_line = max ( line - 1 , 0 );
	int pgno = line / 7;

	if ( line && !( line % 7 ) ) {
		pgno--;
	}

	if ( pgno < 0 ) {
		return -1;
	}

	return ( find_line % 7 ) * 4  + ( pgno * 63 ) + 35;
}

//-----------------------------------------------------------------------------
// カラム番号の取得
//-----------------------------------------------------------------------------
int CShinSpecific22_31::GetColmNo ( int index )
{
	int page = index / NUM_PER_PAGE;
	int ofs = index - ( page * NUM_PER_PAGE + DIAG_INP_STIDX );
	int colmn = ofs % CLMNUM_PER_ROW;
	return colmn;
}

//---------------------------------------------------------------------------
// テーブル番号の取得(フォーカス移動移動用)
//-----------------------------------------------------------------------------
int CShinSpecific22_31::GetFocusNo ( int index )
{
	int	i = 0, j = 0;
	int pageno = 0, tableno = 0, indexno = 0;

	// 該当ページ番号の取得
	pageno = ( index / NUM_PER_PAGE );
	// 該当ページ先頭テーブル番号
	tableno = ( pageno * TBL_REC_NUM_PER_PAGE );
	// 該当ページ先頭
	indexno = ( pageno * NUM_PER_PAGE ) + DIAG_INP_STIDX;

	i = index - indexno;
	j = ( i / CLMNUM_PER_ROW );
	tableno += j;

	return tableno;
}

void CShinSpecific22_31::CellClear ( int index )
{
	if ( index < DIAG2_INP_STIDX ) {
		return;
	}

	diag_clear ( IDC_ICSDIAG8CTRL1, index, FALSE, CIcsdiagctrl );

	return;
}

BOOL CShinSpecific22_31::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

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
		else if ( pMsg->wParam == VK_PRIOR ) {
			m_Specific221.ScrollPos ( SB_PAGEUP, 0 );
		}
		else if ( pMsg->wParam == VK_NEXT ) {
			m_Specific221.ScrollPos ( SB_PAGEDOWN, 0 );
		}

	}

	return CSpcBaseDlg::PreTranslateMessage(pMsg);
}

BEGIN_EVENTSINK_MAP(CShinSpecific22_31, CSpcBaseDlg)
	ON_EVENT(CShinSpecific22_31, IDC_ICSDIAG8CTRL1, 1, CShinSpecific22_31::EditONIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinSpecific22_31, IDC_ICSDIAG8CTRL1, 2, CShinSpecific22_31::EditOFFIcsdiag8ctrl1, VTS_I2)
	ON_EVENT(CShinSpecific22_31, IDC_ICSDIAG8CTRL1, 3, CShinSpecific22_31::TerminationIcsdiag8ctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
END_EVENTSINK_MAP()

void CShinSpecific22_31::EditONIcsdiag8ctrl1(short index)
{

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	m_curwnd = IDC_ICSDIAG8CTRL1;

	if ( !m_initfg ) {
		return;
	}

	int line = GetTableNo( index );
	if ( line < ( *m_pSpcListData )->TsTbl.tp_rnum ) {
		return;
	}

	int find = findindex(line);
	if ( find < 0 ) {
		return;
	}

	DIAGRAM_DATA	DD;
	DD.data_imgdata = NULL;
	int				sw = 0;

	diag_getdata ( IDC_ICSDIAG8CTRL1, find, ( LPUNKNOWN )&DD, CIcsdiagctrl );
	if ( DD.data_edit.GetLength() ){
		sw |= 0x01;
	}

	diag_getdata ( IDC_ICSDIAG8CTRL1, find + 1, ( LPUNKNOWN )&DD, CIcsdiagctrl );
	if ( m_pArith->l_test ( DD.data_val ) ) {
		sw |= 0x02;
	}

	diag_getdata ( IDC_ICSDIAG8CTRL1, find + 2, ( LPUNKNOWN )&DD, CIcsdiagctrl );
	if ( m_pArith->l_test ( DD.data_val ) ) {
		sw |= 0x04;
	}

	diag_getdata ( IDC_ICSDIAG8CTRL1, find + 3, ( LPUNKNOWN )&DD, CIcsdiagctrl );
	if ( m_pArith->l_test ( DD.data_val ) ) {
		sw |= 0x08;
	}

	diag_getdata ( IDC_ICSDIAG8CTRL1, find + 4, ( LPUNKNOWN )&DD, CIcsdiagctrl );
	if ( m_pArith->l_test ( DD.data_val ) ) {
		sw |= 0x10;
	}

	diag_getdata ( IDC_ICSDIAG8CTRL1, find + 5, ( LPUNKNOWN )&DD, CIcsdiagctrl );
	if ( m_pArith->l_test ( DD.data_val ) ) {
		sw |= 0x20;
	}

	diag_getdata ( IDC_ICSDIAG8CTRL1, find + 6, ( LPUNKNOWN )&DD, CIcsdiagctrl );
	if ( m_pArith->l_test ( DD.data_val ) ) {
		sw |= 0x40;
	}

	if ( !sw ) {
		int find = findindex ( ( *m_pSpcListData )->TsTbl.tp_rnum + 1);
		if ( ( find < 0 ) || ( index == find ) ) {
			return;
		}

		diag_setposition ( IDC_ICSDIAG8CTRL1, find, CIcsdiagctrl );
		return;
	}

	if ( GetTableNo ( index ) == ERR ) {
		return;
	}
}

void CShinSpecific22_31::EditOFFIcsdiag8ctrl1(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	TSREC TSrec = { 0 };
	int	TableNo = 0, ColmNo = 0;
	int	IndexAdd = 0, PutData = 0, NewLine = 0;
	char MONY[6] = { 0 };
	DIAGRAM_DATA DiagData;
	DiagData.data_imgdata = NULL;
	BOOL isMonyChg = FALSE;

	if ( !m_initfg ) {
		return;
	}

	if ( IsWindowVisible() == FALSE ) {
		return;
	}

	ASSERT ( m_pSpcListData );
	if ( m_pSpcListData == NULL ) {
		return;
	}

	ASSERT ( *m_pSpcListData );
	if ( *m_pSpcListData == NULL ) {
		return;
	}

	diag_getdata ( IDC_ICSDIAG8CTRL1, index, ( struct IUnknown* )&DiagData, CIcsdiagctrl );

	if ( ( TableNo = GetTableNo ( index ) ) == ERR ) {
		return;
	}

	ColmNo = GetColmNo( index );

	// テーブル読み込み
	NewLine = 0;
	memset ( &TSrec, '\0', sizeof ( TSREC ) );
	if ( ( *m_pSpcListData )->m_TblHdl.th_rget ( &TSrec, &( *m_pSpcListData )->TsTbl, TableNo ) ) {
		if ( ( *m_pSpcListData )->TsTbl.tp_rnum + 1 > ( *m_pSpcListData )->TsTbl.tp_rmax ) {
			return;
		}

		// 次行の新規登録
		memset( &TSrec, '\0', sizeof ( TSREC ) );
		TSrec.TsLine = TableNo + 1;
		NewLine = 1;
	}

	IndexAdd = 0;
	PutData = 0;
	switch( ColmNo ) {
	case 0 :	// 科目名称
		sprintf_s ( TSrec.TsName, sizeof ( TSrec.TsName ), _T( "%s" ), DiagData.data_edit.GetBuffer() );
		PutData = DiagData.data_edit.GetLength(); 
		break;
	case 1 :	// (４％)特定収入
		if ( m_pArith->l_cmp ( TSrec.TsDtl[0].TsTsyu, DiagData.data_val ) ) {
			isMonyChg = TRUE;
		}
		memmove ( TSrec.TsDtl[0].TsTsyu, DiagData.data_val, sizeof ( TSrec.TsDtl[0].TsTsyu ) );
		PutData = m_pArith->l_test ( DiagData.data_val );
		break;
	case 2 :	// (４％)課税仕入れ等に係る特定収入
		if ( m_pArith->l_cmp ( TSrec.TsDtl[0].TsSsyu, DiagData.data_val ) ){
			isMonyChg = TRUE;
		}
		memmove ( TSrec.TsDtl[0].TsSsyu, DiagData.data_val, sizeof ( TSrec.TsDtl[0].TsSsyu ) );
		PutData = m_pArith->l_test ( DiagData.data_val );
		break;
	case 3 :	// (４％)共通に係る特定収入
		if ( m_pArith->l_cmp ( TSrec.TsDtl[0].TsKsyu, DiagData.data_val ) ){
			isMonyChg = TRUE;
		}
		memmove ( TSrec.TsDtl[0].TsKsyu, DiagData.data_val, sizeof ( TSrec.TsDtl[0].TsKsyu ) );
		PutData = m_pArith->l_test ( DiagData.data_val );
		break;
	case 4 :	// (６．３％)特定収入
		if( m_pArith->l_cmp ( TSrec.TsDtl[1].TsTsyu, DiagData.data_val ) ){
			isMonyChg = TRUE;
		}
		memmove ( TSrec.TsDtl[1].TsTsyu, DiagData.data_val, sizeof (TSrec.TsDtl[1].TsTsyu ) );
		PutData = m_pArith->l_test ( DiagData.data_val );
		break;
	case 5 :	// (６．３％)課税仕入れ等に係る特定収入
		if ( m_pArith->l_cmp ( TSrec.TsDtl[1].TsSsyu, DiagData.data_val ) ){
			isMonyChg = TRUE;
		}
		memmove ( TSrec.TsDtl[1].TsSsyu, DiagData.data_val, sizeof ( TSrec.TsDtl[1].TsSsyu ) );
		PutData = m_pArith->l_test ( DiagData.data_val );
		break;
	case 6 :	// (６．３％)共通に係る特定収入
		if ( m_pArith->l_cmp ( TSrec.TsDtl[1].TsKsyu, DiagData.data_val ) ){
			isMonyChg = TRUE;
		}
		memmove ( TSrec.TsDtl[1].TsKsyu, DiagData.data_val, sizeof ( TSrec.TsDtl[1].TsKsyu ) );
		PutData = m_pArith->l_test ( DiagData.data_val );
		break;
	default:	// 
		break;
	}

	// 非課税に係るを算出
	memset ( MONY, '\0', sizeof ( MONY ) );
	m_pArith->l_add ( MONY, TSrec.TsDtl[0].TsSsyu, TSrec.TsDtl[0].TsKsyu );
	m_pArith->l_sub(  TSrec.TsDtl[0].TsHsyu, TSrec.TsDtl[0].TsTsyu, MONY );

	memset ( MONY, '\0', sizeof ( MONY ) );
	m_pArith->l_add ( MONY, TSrec.TsDtl[1].TsSsyu, TSrec.TsDtl[1].TsKsyu );
	m_pArith->l_sub ( TSrec.TsDtl[1].TsHsyu, TSrec.TsDtl[1].TsTsyu, MONY );

	// 特定収入合計を計算
	memset ( MONY, '\0', sizeof ( MONY ) );
	m_pArith->l_add ( MONY, TSrec.TsDtl[0].TsTsyu, TSrec.TsDtl[0].TsFsyu );
	memmove ( TSrec.TsDtl[0].TsGsyu, MONY, sizeof ( TSrec.TsDtl[0].TsGsyu ) );

	memset ( MONY, '\0', sizeof ( MONY ) );
	m_pArith->l_add ( MONY, TSrec.TsDtl[1].TsTsyu, TSrec.TsDtl[1].TsFsyu );
	memmove( TSrec.TsDtl[1].TsGsyu, MONY, sizeof ( TSrec.TsDtl[1].TsGsyu ) );

	if ( ( PutData == 0 ) && NewLine ) {
		// 入力データ無し
		return;
	}

	if ( ( *m_pSpcListData )->m_TblHdl.th_acs ( thWritex, &TSrec, &( *m_pSpcListData )->TsTbl ) ) {
		ICSMessageBox ( "テーブルに書き込み中に障害が発生しました" );
		return;
	}

	// 再計算
	if ( isMonyChg ) {
		Specific22_Calq();
	}
}

void CShinSpecific22_31::TerminationIcsdiag8ctrl1(short index, short nChar, short length, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	char VK_FLG = '\0';
	short wTerm = 0;

	if ( nChar == 0 ) {
		return;
	}

	if ( !m_initfg ) {
		return;
	}

	// SHIFT+TABをTABに、TABをENTERに変換
	VK_FLG = 0;
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

	int	TableNo = 0, ColmNo = 0;

	//フォーカス用のポジション取得関数を追加。行の増減をGetTableNoでは操作している為。フォーカスポジションをうまく制御できない。
	if ( ( TableNo = GetFocusNo ( index ) ) == ERR ) {
		return;
	}

	m_TableNo = TableNo;
	ColmNo = GetColmNo ( index );
		
	UINT	flg = 0;

	switch ( wTerm ) {
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
			index = index + CLMNUM_PER_ROW;
			if ( m_TableNo && ( ( ( m_TableNo + 1 ) % TBL_REC_NUM_PER_PAGE ) == 0 ) ) {
				index += DIAG2_INP_STIDX;
			}	
			break;
		case VK_UP:
			index = index - CLMNUM_PER_ROW;
			if ( m_TableNo && ( ( m_TableNo % TBL_REC_NUM_PER_PAGE ) == 0 ) ) {
				index -= DIAG2_INP_STIDX;
			}
			break;
		case VK_DELETE :
			CellClear( index );
			break;
	}

	if ( flg & REPLY_NEXT ) {
		diag_setnext ( IDC_ICSDIAG8CTRL1, CIcsdiagctrl );
	}
	else if ( flg & REPLY_BACK ) {
		diag_setprev ( IDC_ICSDIAG8CTRL1, CIcsdiagctrl );
	}
	else {
		diag_setposition ( IDC_ICSDIAG8CTRL1, index, CIcsdiagctrl );
	}
}

void CShinSpecific22_31::IndexControl()
{
	int	Cnt = 0;
	while ( 1 ){
		if ( Specific22_31[Cnt].Index == 0 ) {
			break;
		}
		int	Flg = 0;
		// 確定時は全入力不可
		if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x01 ) {
			Flg = Specific22_31[Cnt].AllUnConnect;
		}
		else {
			if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
				Flg = Specific22_31[Cnt].UnConnect;
			}
			else {
				Flg = Specific22_31[Cnt].Connect;
			}
		}
		
		if ( Flg == 2 ) {
			if ( ( ( *m_pSnHeadData )->Sn_TUMIAG & 0x01 ) && ( m_pZmSub->zvol->s_sgn1 & 0xc0 ) ) {
				Flg = 0;
			}
		}

		if ( Flg == 3 ) {
			if ( ( ( *m_pSnHeadData )->Sn_TUMIAG & 0x01 ) && ( m_pZmSub->zvol->s_sgn1 & 0xc0 ) ) {
				Flg = 1;
			}
		}

		ATRIB_MOD ( IDC_ICSDIAG8CTRL1, Specific22_31[Cnt].Index, Flg, ( *m_pSnHeadData )->Sn_Sign4, Specific22_31[Cnt].Connect );
		Cnt++;
	}

	SetFocusRange();

	m_Specific221.EnableDelete ( 0 );//DELETEキー１回で削除モード設定
}

void CShinSpecific22_31::SetItemPosition ( BOOL IsInEnd/*=FALSE*/ )
{
	int index = 0;

	// 前回ポジションの復元
	if ( m_curwnd == IDC_ICSDIAG8CTRL1 ) {
		if ( ( index = m_Specific221.GetPosition() ) != -1 ) {
			if ( IsInEnd ) {

				// 画面上に完全に矩形領域がある時のみフォーカスを再セット
				RECT	rc221, rc22, rc220;
				m_Specific22.GetRect ( 1, ( LPUNKNOWN )&rc22 );
				m_Specific221.GetRect ( index, ( LPUNKNOWN )&rc221 );
				m_Specific220.GetRect ( 1, ( LPUNKNOWN )&rc220 );

				if ( ( ( rc22.bottom < rc221.top ) && ( rc221.bottom < rc220.top ) ) ) {
					m_Specific221.SetPosition(index);
				}

				m_Specific221.SetFocus();
			}
			else {
				m_Specific221.SetPosition ( index );
				m_Specific221.SetFocus();
			}
			return;
		}
	}

	this->SetFocus();

	m_Specific220.SetFocus();
	m_Specific221.SetFocus();

	if ( ( *m_pSnHeadData )->Sn_Sign4 & 0x80 ) {
		m_Specific221.SetPosition( ID202_1G );
	}

	SetRedrawScroll ( 0 );
}

void CShinSpecific22_31::SetFocusRange()
{

	DIAGRAM_ATTRIBUTE	DA;

	m_FcsUpEnd		= -1;
	m_FcsLeftEnd	= -1;
	m_FcsDownEnd	= -1;
	m_FcsRightEnd	= -1;

	for ( int idx = 0; idx < FCS_SPECIFIC22CNT; idx++ ) {
		m_Specific221.GetAttribute ( FCS_Specific22_31[idx].IDNo, ( LPUNKNOWN )&DA );
		
		if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
			m_FcsUpEnd = FCS_Specific22_31[idx].IDNo;
			break;
		}
	}

	for ( int idx = 0; idx < FCS_SPECIFIC22CNT; idx++ ) {
		m_Specific221.GetAttribute ( FCS_Specific22_31_LR[idx].IDNo, ( LPUNKNOWN )&DA );

		if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
			m_FcsLeftEnd = FCS_Specific22_31_LR[idx].IDNo;
			break;
		}
	}

	for ( int idx = FCS_SPECIFIC22CNT-1; idx > -1 ; idx-- ) {
		m_Specific221.GetAttribute ( FCS_Specific22_31[idx].IDNo, ( LPUNKNOWN )&DA) ;
		
		if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
			m_FcsDownEnd = FCS_Specific22_31[idx].IDNo;
			break;
		}
	}

	for ( int idx = FCS_SPECIFIC22CNT-1; idx > -1 ; idx-- ) {
		m_Specific221.GetAttribute ( FCS_Specific22_31_LR[idx].IDNo, ( LPUNKNOWN )&DA );
		
		if ( ( DA.attr_bcolor == BC_CREAM ) || ( DA.attr_bcolor == BC_BLUE ) ) {
			m_FcsRightEnd = FCS_Specific22_31_LR[idx].IDNo;
			break;
		}
	}
}

void CShinSpecific22_31::OnButtonF1()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	diag_setprev( IDC_ICSDIAG8CTRL1, CIcsdiagctrl );
}

void CShinSpecific22_31::OnButtonF2()
{
	// TODO: ここにコマンド ハンドラ コードを追加します。
	diag_setnext( IDC_ICSDIAG8CTRL1, CIcsdiagctrl );
}

void CShinSpecific22_31::OnSize(UINT nType, int cx, int cy)
{
	CSpcBaseDlg::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if ( m_initfg ) {
		RECT	a, b;

		GetWindowRect ( &a );

		m_Specific220.GetRect ( BottomIndex, ( LPUNKNOWN )&b );

	}
}

HBRUSH CShinSpecific22_31::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSpcBaseDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
	if ( nCtlColor == CTLCOLOR_DLG ) {
		hbr = Ibrsh_ShinSpecific;
	}

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}


//---------------------------------------------------------
// 該当税率データかどうか
//---------------------------------------------------------
// 引数		pTsrec
//---------------------------------------------------------
// 戻り値　	TRUE	：	該当税率データ
//			FALSE	：　該当税率データでない
//---------------------------------------------------------
BOOL CShinSpecific22_31::IsNowZrtData( TSREC *pTsrec )
{
	BOOL	rt = FALSE;

	ASSERT( pTsrec );
	if( pTsrec == NULL ){
		return rt;
	}

	// 旧税率または新税率のどちらかに金額があればTRUE
	for ( int i = 0; i < TS_ZR_MAX - 6; i++ ) {	// ５％、８％、８％軽減(6.24%)、１０％分
		if( (m_pArith->l_test( pTsrec->TsDtl[i].TsTsyu )) || (m_pArith->l_test( pTsrec->TsDtl[i].TsSsyu )) ||
			(m_pArith->l_test( pTsrec->TsDtl[i].TsHsyu )) || (m_pArith->l_test( pTsrec->TsDtl[i].TsKsyu ))){
			rt = TRUE;
			break;
		}
	}

	return rt;
}