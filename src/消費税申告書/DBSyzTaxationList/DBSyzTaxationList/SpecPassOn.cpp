// SpecPassOn.cpp : 実装ファイル
//

#include "stdafx.h"
#include "SpecPassOn.h"

//== 定数 ==
const int	IdTop		=	95;
const int	BottomIndex	=	144;

// CSpecPassOn ダイアログ

IMPLEMENT_DYNAMIC(CSpecPassOn, ICSDialog)

CSpecPassOn::CSpecPassOn(CWnd* pParent /*=NULL*/)
	: ICSDialog(CSpecPassOn::IDD, pParent)
	, m_pParent(pParent)
	, m_pSnHeadData(NULL)
	, m_pSpcListData(NULL)
	, m_pArith(NULL)
	, m_initfg(0)
	, m_wPos(0)
	, m_cmPos(0)
	, m_ScW(0)
{
}

CSpecPassOn::~CSpecPassOn()
{
}

void CSpecPassOn::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSpecPassOn, ICSDialog)
	ON_WM_VSCROLL()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CSpecPassOn メッセージ ハンドラ

BOOL CSpecPassOn::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	InitDisp();

	m_initfg = 1;
	c_set( IDOK );

	ICSDialog::OnInitDialogEX();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//-----------------------------------------------------------------------------
// 初期表示
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSpecPassOn::InitDisp()
{
	char			CBF[80]={0}, BUF[80]={0}, KBF[80]={0}, MBF[80]={0},
					WORK0[6]={0}, WORK1[6]={0}, WORK2[6]={0};
	DIAGRAM_DATA	DiagData;
	DiagData.data_imgdata = NULL;

	// ①.課税売上割合
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKuws, _T("SSS,SSS,SSS,SS9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKuwb, _T("SSS,SSS,SSS,SS9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+1, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpUwri, _T("SSS9.99") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+2, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// ②.調整前の仕入控除税額
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKgz3, _T("SSS,SSS,SSS,SS9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+3, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKgz4, _T("SSS,SSS,SSS,SS9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+4, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKgzg, _T("SSS,SSS,SSS,SS9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+5, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	// ③.特定割合	
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGsyu, _T("SSS,SSS,SSS,SS9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+6, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	memset( WORK0, '\0', 6 );
	m_pArith->l_add( WORK0, (*m_pSpcListData)->stval.SpGsyu, (*m_pSpcListData)->stval.SpTuwb );
	m_pArith->l_print( MBF, WORK0, _T("SSS,SSS,SSS,SS9") );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+7, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	// [04'07.29]
	m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTswr, "SSS9.9" );
	DiagData.data_disp = MBF;
	diag_setdata( IDC_ICSDIAGCTRL1, IdTop+8, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	memset( WORK0, '\0', sizeof(WORK0) );
	m_pArith->l_input( WORK0, _T("50") );

	if( m_pArith->l_cmp( (*m_pSpcListData)->stval.SpTswr, WORK0 ) > 0 ){	// 特例収入割合５％以上	
		// ④.特定収入に係る税額
		memset( WORK0, '\0', sizeof(WORK0) );
		m_pArith->l_input( WORK0, &(*m_pSnHeadData)->m_UriRatioBorder );
		if( m_pArith->l_cmp( (*m_pSpcListData)->stval.SpUwri, WORK0 ) >= 0 ){	// 課税売上割合９５％以上
			memset( WORK0, '\0', sizeof(WORK0) );
			m_pArith->l_add( WORK0, (*m_pSpcListData)->stval.SpTsyu, (*m_pSpcListData)->stval.SpKsyu );
			m_pArith->l_add( WORK0, WORK0, (*m_pSpcListData)->stval.SpHsyu );
			m_pArith->l_print( MBF, WORK0, _T("SSS,SSS,SSS,SS9") );
			DiagData.data_disp = MBF;
			diag_setdata( IDC_ICSDIAGCTRL1, IdTop+9, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGszg, _T("SSS,SSS,SSS,SS9") );
			DiagData.data_disp = MBF;
			diag_setdata( IDC_ICSDIAGCTRL1, IdTop+10, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		}
		else{
			if( (*m_pSnHeadData)->IsKobetuSiireAnbun() == FALSE ){
				// 比例配分
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTsyu, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+11, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpUwri, _T("SSS9.99") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+12, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTszg, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+13, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			}
			else{
				// 個別対応
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTsyu, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+14, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTszg, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+15, (struct IUnknown *)&DiagData, CIcsdiagctrl );

				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKsyu, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+16, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpUwri, _T("SSS9.99") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+17, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKszg, _T("SSS,SSS,SSS,SS9") );
				DiagData.data_disp = MBF;
				diag_setdata( IDC_ICSDIAGCTRL1, IdTop+18, (struct IUnknown *)&DiagData, CIcsdiagctrl );
			}
		}
		// ⑤.調整割合
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpFsyu, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+19, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		memset( WORK0, '\0', 6 );
		m_pArith->l_add( WORK0, (*m_pSpcListData)->stval.SpFsyu, (*m_pSpcListData)->stval.SpTuwb );
		m_pArith->l_print( MBF, WORK0, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+20, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTwar, _T("SSS9.99") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+21, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		// ⑥.調整額
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKgzg, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+22, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGszg, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+23, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTwar, _T("SSS9.99") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+24, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTwag, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+25, (struct IUnknown *)&DiagData, CIcsdiagctrl );

		// ⑦．新税率適用分
		// ⑦-1．配賦割合
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKsi4, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+26, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKsig, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+27, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTsyu, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+28, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTsyu, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+29, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKsb4, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+30, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKsbg, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+31, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKshf, _T("SSS9.99") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+32, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		// ⑦-2．特定収入に係る課税仕入れ等の税額
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGszg, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+33, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTwag, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+34, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKshf, _T("SSS9.99") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+35, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGgt4, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+36, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		// ⑦-3．控除対象仕入税額
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKgz4, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+37, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGgt4, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+38, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpSiz4, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+39, (struct IUnknown *)&DiagData, CIcsdiagctrl );

		// ⑧．旧税率適用分
		// ⑧-1．特定収入に係る課税仕入れ等の税額
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGszg, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+40, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpTwag, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+41, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpGgt4, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+42, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKsh3, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+43, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		// ⑧-2．控除対象仕入税額
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKgz3, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+44, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpKsh3, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+45, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpSiz3, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+46, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		// ⑨．調整後の仕入れ控除税額
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpSiz3, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+47, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpSiz4, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+48, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		m_pArith->l_print( MBF, (*m_pSpcListData)->stval.SpSizg, _T("SSS,SSS,SSS,SS9") );
		DiagData.data_disp = MBF;
		diag_setdata( IDC_ICSDIAGCTRL1, IdTop+49, (struct IUnknown *)&DiagData, CIcsdiagctrl );

	}

	return 0;
}

BOOL CSpecPassOn::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( pMsg->message == WM_KEYDOWN ){

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
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

//-----------------------------------------------------------------------------
// スクロール再描画
//-----------------------------------------------------------------------------
// 引数	mpos	：	移動位置
//-----------------------------------------------------------------------------
void CSpecPassOn::sykscroll( int mpos )
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

void CSpecPassOn::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	if( pScrollBar ){
		return;
	}
	SCROLLINFO SI;
	GetScrollInfo( SB_VERT, &SI );
	int nowPos = m_wPos;

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
			if( m_wPos >= SI.nMax-(int)SI.nPage ){
				m_wPos = SI.nMax-SI.nPage;
			}
			break;
		case SB_THUMBTRACK:
			m_wPos = nPos;
			break;
	}
	if( (nSBCode==SB_LINEDOWN) || (nSBCode==SB_LINEUP) ||
		(nSBCode==SB_PAGEDOWN) || (nSBCode==SB_PAGEUP) || (nSBCode==SB_THUMBTRACK) ){
		ScrollWindow( 0, nowPos-m_wPos );
		SetScrollPos( SB_VERT, m_wPos );
		m_cmPos = m_wPos;
	}

	ICSDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CSpecPassOn::OnSize(UINT nType, int cx, int cy)
{
	if( m_initfg ){
		SetScrollPos( SB_VERT, 0, TRUE );
		ScrollWindow( 0, m_cmPos, NULL );
		m_cmPos = 0;
	}

	ICSDialog::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( m_initfg ){
		RECT	a;
		SCROLLINFO ScrollInfo;
		GetWindowRect( &a );
		if( GetScrollInfo(SB_VERT, &ScrollInfo) == FALSE ){
			ScrollInfo.nPos = 0;
		}
		ScrollInfo.nMin  = 0;
		ScrollInfo.nMax  = a.bottom - a.top;
		ScrollInfo.nPage = a.bottom - a.top;
		SetScrollInfo( SB_VERT, &ScrollInfo );
		m_wPos = 0;
		m_ScW = 15;
	}
}

//-----------------------------------------------------------------------------
// 初期値セット
//-----------------------------------------------------------------------------
// 引数	pSnHeadData		：	ヘッダ情報
//		pSpcListData	：	特定収入計算表データ
//		pArith			：	倍長演算
//-----------------------------------------------------------------------------
// 返送値	0			：	正常終了
//			-1			：	エラー
//-----------------------------------------------------------------------------
int CSpecPassOn::Init( CSnHeadData **pSnHeadData, CH26SpcListData **pSpcListData, CArithEx *pArith )
{
	ASSERT( pSnHeadData );
	if( pSnHeadData == NULL ){
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	ASSERT( pSpcListData );
	if( pSpcListData == NULL ){
		return -1;
	}
	m_pSpcListData = pSpcListData;

	ASSERT( pArith );
	if( pArith == NULL ){
		return -1;
	}
	m_pArith = pArith;

	return 0;
}
