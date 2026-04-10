// SwlistSelDlg.cpp : インプリメンテーション ファイル
//	仕訳摘要コピー時の 仕訳選択ダイアログ
//

#include "stdafx.h"
#include "resource.h"

#include "DBInpSubFunc.h"

#include "SwlistSelDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSwlistSelDlg ダイアログ


CSwlistSelDlg::CSwlistSelDlg(CWnd* pParent /*=NULL*/)
	: ICSDialog(CSwlistSelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSwlistSelDlg)
	//}}AFX_DATA_INIT

	m_pZm = NULL;
	m_bInit = FALSE;

	m_SelIndex = 0;
}


void CSwlistSelDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSwlistSelDlg)
	DDX_Control(pDX, IDC_ICSLISTINPCTRL1, m_listswk);
	DDX_Control(pDX, IDOK, m_Ok);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	//}}AFX_DATA_MAP
}


void CSwlistSelDlg::SetZmClass(CDBZmSub* pZm, CSwcdArray* pSwary )
{
	m_pZm = pZm;
	m_pSwk = pSwary;
}


BEGIN_MESSAGE_MAP(CSwlistSelDlg, ICSDialog)
	//{{AFX_MSG_MAP(CSwlistSelDlg)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_USER_1, OnUser1_Msg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSwlistSelDlg メッセージ ハンドラ

BOOL CSwlistSelDlg::OnInitDialog() 
{
	// TODO: この位置に初期化の補足処理を追加してください
	ICSDialog::OnInitDialog();

	MakeSiwakeFrm();

	ICSDialog::OnInitDialogEX();

	return FALSE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}


//	仕訳選択部分
//	表形式入力　設定
void CSwlistSelDlg::MakeSiwakeFrm()
{
	LINP_COLUMN LC[4];
	LINP_VCOLORINFO	LVCOL[4];

	//　番号
	LC[0].clm_kind = LINP_KIND_STRING;
	LC[0].clm_max = 2;
	LC[0].clm_attr = LINP_ATTR_CENTER| LINP_ATTR_BOTTOM;
	LC[0].clm_width = 20;
	LC[0].clm_title = _T( "番号" );
	LC[0].clm_tbcolor = -1;
	LC[0].clm_ttcolor = -1;

	LC[1].clm_kind = LINP_KIND_KAMOKU;
	LC[1].clm_max = 4;
	LC[1].clm_attr = LINP_ATTR_CENTER | LINP_ATTR_BOTTOM;
	LC[1].clm_width = 75;
	LC[1].clm_title = _T( "借　方" );
	LC[1].clm_tbcolor = -1;
	LC[1].clm_ttcolor = -1;

	LC[2].clm_kind = LINP_KIND_KAMOKU;
	LC[2].clm_max = 4;
	LC[2].clm_attr = LINP_ATTR_CENTER | LINP_ATTR_BOTTOM;
	LC[2].clm_width = 75;
	LC[2].clm_title = _T( "貸　方" );
	LC[2].clm_tbcolor = -1;
	LC[2].clm_ttcolor = -1;

//	m_listswk.SetColumn( 3, (LPUNKNOWN)LC );
	linp_setclm( IDC_ICSLISTINPCTRL1, 3, LC, CICSListInp);

	// 表形式入力　グリッド線の設定	
//	m_listswk.SetHorzGrid( 5, CL_RED, LINP_GRID_SOLID );
	linp_horzgrid( IDC_ICSLISTINPCTRL1, 5, CL_RED, LINP_GRID_SOLID, CICSListInp );

	LVCOL[0].pos = 0;
	LVCOL[0].color = CL_GREEN;
	LVCOL[0].type = LINP_GRID_SOLID;

	LVCOL[1].pos = 1;
	LVCOL[1].color = CL_GREEN;
	LVCOL[1].type = LINP_GRID_SOLID;

//	m_listswk.SetVartGrid( 2, (LPUNKNOWN)LVCOL );
	linp_vartgrid( IDC_ICSLISTINPCTRL1, 2, (LPUNKNOWN)LVCOL, CICSListInp );
}


int CSwlistSelDlg::SiwakeInpSet()
{
	LINP_DATA	LD[4];
	COLORREF	ONCOLOR, OFFCOLOR;
	DBKNREC*	pKn;
	SWKCOD_DATA	sc;

	CString	kmkcod;
	m_listswk.RemoveAll();

	ONCOLOR = CL_BLACK;
	OFFCOLOR = CL_TKBACK;

	int max;
	max = m_pSwk->GetSize();

	for( int i=0; i < max; i++ ) {
		sc = m_pSwk->GetAt( i );
		if( (i+1) == max ) {
			if( ! sc.sc_sgn ) continue;
		}
		// 科目がない
		if( sc.sc_dbt.IsEmpty() && sc.sc_cre.IsEmpty() )
			continue;

		// 番号
		LD[0].dat_string.Format( "%d", i+1 );
		LD[0].dat_attr = LINP_DISP_COLOR;
		LD[0].dat_fc = ONCOLOR;
		LD[0].dat_bc = OFFCOLOR;
		LD[0].dat_initflg = FALSE;

		for( int n = 1; n <= 2; n++ ) {
			// 借・貸 科目
			LD[n].dat_string.Empty();
			LD[n].dat_attr = LINP_DISP_COLOR;
			LD[n].dat_fc = ONCOLOR;
			LD[n].dat_bc = OFFCOLOR;

			kmkcod = (n==1) ? sc.sc_dbt : sc.sc_cre;

			if( kmkcod.IsEmpty() )
				LD[n].dat_initflg = TRUE;
			else {
				LD[n].dat_initflg = FALSE;

				pKn = DB_PjisToKnrec( kmkcod, m_pZm );
				
				memset( LD[n].dat_kamoku.kamoku_name, 0, 14 );
				if( pKn ) {
					wsprintf( (char *)LD[n].dat_kamoku.kamoku_name, "%.14s", pKn->knnam );
					CString tmp;
					tmp.Format( "%08x", pKn->kncod );
					TV_kamokucodeConv(LD[n].dat_kamoku.kamoku_code, tmp, CONV2_1 );
				//	LD[n].dat_kamoku.kamoku_code = pKn->KNcod;
					LD[n].dat_kamoku.kamoku_edaban = 0xffff;
					LD[n].dat_kamoku.kamoku_num = 0;
				}
			}
		}

		m_listswk.SetData( i, (LPUNKNOWN)LD, TRUE );
	}

	return 0;
}


LRESULT CSwlistSelDlg::OnUser1_Msg(WPARAM wParam, LPARAM lParam)
{
	if( wParam == 0 ) {
		// ダイアログ表示のときに摘要セット
		SiwakeInpSet();
	//	c_set( IDC_ICSLISTINPCTRL1 );
		m_listswk.SetFocus();
	}

	return 0;
}


void CSwlistSelDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	ICSDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	if( bShow && !m_bInit ) {
		m_bInit = TRUE;
		PostMessage( WM_USER_1, 0, 0 );
	}	
}

void CSwlistSelDlg::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください

	int top = m_listswk.GetTopIndex();
	int idx = m_listswk.GetIndex();

	m_SelIndex = top + idx;
	
	ICSDialog::OnOK();
}

BEGIN_EVENTSINK_MAP(CSwlistSelDlg, ICSDialog)
    //{{AFX_EVENTSINK_MAP(CSwlistSelDlg)
	ON_EVENT(CSwlistSelDlg, IDC_ICSLISTINPCTRL1, 4 /* KeyIn */, OnKeyInIcslistinpctrl1, VTS_I2 VTS_I2 VTS_I2)
	ON_EVENT(CSwlistSelDlg, IDC_ICSLISTINPCTRL1, 7 /* LButtonDblClick */, OnLButtonDblClickIcslistinpctrl1, VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CSwlistSelDlg::OnKeyInIcslistinpctrl1(short nChar, short previndex, short index) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	
	if( nChar == VK_RETURN ) {
		PostMessage(WM_COMMAND, MAKELONG(IDOK, BN_CLICKED),
								(LPARAM)GetDlgItem(IDOK)->m_hWnd );
	}
	else if( nChar == VK_ESCAPE ) {
		PostMessage(WM_COMMAND, MAKELONG(IDCANCEL, BN_CLICKED),
								(LPARAM)GetDlgItem(IDCANCEL)->m_hWnd );
	}
}


void CSwlistSelDlg::OnLButtonDblClickIcslistinpctrl1(short index) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	
	PostMessage(WM_COMMAND, MAKELONG(IDOK, BN_CLICKED),
							(LPARAM)GetDlgItem(IDOK)->m_hWnd );

}
