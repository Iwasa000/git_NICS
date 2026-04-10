// TtlDsp.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DBDinp.h"
#include "TtlDsp.h"

#include "input1.h"
#include "zanask.h"
#include "mstrw.h"

#include "SubFnc.h"
#include "TaiData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern char FMT18[];

extern class CMINFO*	Minfo;

//表示バッファポインタ
static struct _TTLDSP_TBL *pTTLDSP = NULL;

//行項目定義
static LINP_COLUMN	TTLDSP_LINE_DEF[ 6 ];	//項目定義
#define ColmN  (sizeof(TTLDSP_LINE_DEF)/sizeof(LINP_COLUMN))

//１行分データ
static LINP_DATA	TTLDSP_DATA[ ColmN ];

static LINP_VCOLORINFO vcolinfo[5] = {
	{ 0, RGB(0,0,0),LINP_GRID_SOLID},
	{ 1, RGB(0,0,0),LINP_GRID_SOLID},
	{ 2, RGB(0,0,0),LINP_GRID_SOLID},
	{ 3, RGB(0,0,0),LINP_GRID_SOLID},
	{ 4, RGB(0,0,0),LINP_GRID_SOLID}
};

/////////////////////////////////////////////////////////////////////////////
// CTtlDsp ダイアログ


CTtlDsp::CTtlDsp(CWnd* pParent /*=NULL*/)
	: ICSDialog(CTtlDsp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTtlDsp)
	//}}AFX_DATA_INIT

	m_IDALT = 0;
	m_pAltTxt = 0;

}


void CTtlDsp::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTtlDsp)
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDC_ALT, m_Btn1);
	DDX_Control(pDX, IDC_ICSLISTINPCTRL1, m_linp);
	//}}AFX_DATA_MAP
}

////////////////////////////////////////////////////////
//		外部よりのオペレーション関係
////////////////////////////////////////////////////////
int CTtlDsp::ttl_dsp( int mode )
{
	pTTLDSP = ttldsp( mode );

	if( !pTTLDSP )
		return -1;

	return 0;
}

//行定義作成
void CTtlDsp::mk_linedef( int colm_max, LINP_COLUMN *defn, struct _TTLDSP_TBL *tbl  )
{
	int i;
//	char *fmt18 = "ss,sss,sss,sss,ss9"; 05.20 /00
	LINP_COLUMN *def;
	
	
	for( i = 0 ; i < colm_max ; ++i )
	{
		def = defn + i;

		//color
		def->clm_tbcolor = RGB(   0,   0, 255);
		def->clm_ttcolor = RGB( 255, 255, 255);

		switch( i )
		{
		case 0:	//月分
				def->clm_kind = LINP_KIND_STRING;
				def->clm_max = 4;
				def->clm_attr = LINP_ATTR_BOTTOM|LINP_ATTR_CENTER|LINP_ATTR_ANK|LINP_ATTR_READONLY;
				def->clm_format = "";
				def->clm_width = 20;
				def->clm_lf = NULL;
				def->clm_title = tbl->TITLE[i];
				break;

		case 1:	//通常
				def->clm_kind = LINP_KIND_KINGAKU;
				def->clm_max = 12;
				def->clm_attr = LINP_ATTR_BOTTOM|LINP_ATTR_RIGHT|LINP_ATTR_CONMA|LINP_ATTR_MINUS|LINP_ATTR_READONLY;
				def->clm_format = FMT18;	// fmt18 05.20 /00;
				def->clm_width = 68;
				def->clm_lf = NULL;
				def->clm_title = tbl->TITLE[i];
				break;

		case 2:	//諸口
				def->clm_kind = LINP_KIND_KINGAKU;
				def->clm_max = 12;
				def->clm_attr = LINP_ATTR_BOTTOM|LINP_ATTR_RIGHT|LINP_ATTR_CONMA|LINP_ATTR_MINUS|LINP_ATTR_READONLY;
				def->clm_format = FMT18;	// fmt18 05.20 /00;
				def->clm_width = 68;
				def->clm_lf = NULL;
				def->clm_title = tbl->TITLE[i];
				break;

		case 3:	//諸口
				def->clm_kind = LINP_KIND_KINGAKU;
				def->clm_max = 12;
				def->clm_attr = LINP_ATTR_BOTTOM|LINP_ATTR_RIGHT|LINP_ATTR_CONMA|LINP_ATTR_MINUS|LINP_ATTR_READONLY;
				def->clm_format = FMT18;	// fmt18 05.20 /00;
				def->clm_width = 68;
				def->clm_lf = NULL;
				def->clm_title = tbl->TITLE[i];
				break;

		case 4:	//貸借 -> 諸口差額へ
				def->clm_kind = LINP_KIND_KINGAKU;
				def->clm_max = 12;
				def->clm_attr = LINP_ATTR_BOTTOM|LINP_ATTR_RIGHT|LINP_ATTR_CONMA|LINP_ATTR_MINUS|LINP_ATTR_READONLY;
				def->clm_format = FMT18;	// fmt18 05.20 /00;
				def->clm_width = 68;
				def->clm_lf = NULL;
				def->clm_title = tbl->TITLE[i];
				break;

		case 5:	//入力件数
				def->clm_kind = LINP_KIND_KINGAKU;
				def->clm_max = 6;
				def->clm_attr = LINP_ATTR_BOTTOM|LINP_ATTR_RIGHT|LINP_ATTR_CONMA|LINP_ATTR_MINUS|LINP_ATTR_READONLY;
				def->clm_format = "SSS,SS9";
				def->clm_width = 37;
				def->clm_lf = NULL;
				def->clm_title = tbl->TITLE[i];
				break;
		}

	}
}

//合計データ表示
BOOL CTtlDsp::dsp_ttldsp( int colm_max, LINP_DATA *data, struct _TTLDSP_TBL *tbl )
{
BOOL last_line = FALSE;
int i,j;
LINP_DATA *dat;
BOOL syoguti_err = FALSE;

	l_defn( _BIN );

	for( j = 0 ; j < tbl->TTL_CNT ; ++j )
	{
		if( j == (tbl->TTL_CNT-1) )
			last_line = TRUE;

		for( i = 0 ; i < colm_max ; ++i )
		{
			dat = data + i;

			switch( i )
			{
			case 0:	//月分
				dat->dat_initflg = FALSE;
				dat->dat_string = tbl->DSP_REC[j].GATUBUN;
				break;

			case 1:	//通常
				dat->dat_initflg = FALSE;
				memmove( dat->dat_val, tbl->DSP_REC[j].TUJYO, 6);
				break;

			case 2:	//諸口
				dat->dat_initflg = FALSE;
				memmove( dat->dat_val, tbl->DSP_REC[j].SYOG_DEB, 6);
				break;
			case 3:	//諸口
				dat->dat_initflg = FALSE;
				memmove( dat->dat_val, tbl->DSP_REC[j].SYOG_CRE, 6);
				break;

			case 4:	//貸借 -> 諸口差額へ
				dat->dat_initflg = FALSE;
				l_sub( dat->dat_val, tbl->DSP_REC[j].SYOG_DEB, tbl->DSP_REC[j].SYOG_CRE );
			//	memmove( dat->dat_val, tbl->DSP_REC[j].TAISYAKU, 6);
				break;

			case 5:	//入力件数
				dat->dat_initflg = FALSE;
				memmove( dat->dat_val, tbl->DSP_REC[j].INPLINE, 6);
				break;
			}
			if( last_line )
			{
				dat->dat_fc = RGB(   0,   0,   0 );
				dat->dat_bc = SELECT_BACK_COL;	//RGB(   0,   0, 255 );
				dat->dat_attr = LINP_DISP_COLOR;
			}
			else if( tbl->DSP_REC[j].GATUBUN_ATTR )
			{
				dat->dat_fc = RGB(   0,   0,   0 );
				dat->dat_bc = RGB( 255, 255,   0 );
				dat->dat_attr = LINP_DISP_COLOR;

				syoguti_err = TRUE;
			}
			else
				dat->dat_attr = 0;

#ifdef _TEST_CODE_
			if( i != 5 )
				l_input( dat->dat_val, "123,456,789,000" );
			else
				l_input( dat->dat_val, "123,456" );
#endif
		}
		m_linp.SetData( j, (LPUNKNOWN)data, TRUE ); 
	}
	return syoguti_err;
}


BEGIN_MESSAGE_MAP(CTtlDsp, ICSDialog)
	//{{AFX_MSG_MAP(CTtlDsp)
	ON_WM_SETFOCUS()
	ON_BN_CLICKED(IDC_ALT, OnAlt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTtlDsp メッセージ ハンドラ
void CTtlDsp::set_alt( int alt_msg, char *alt_txt)
{
	m_IDALT = alt_msg;
	m_pAltTxt = alt_txt;
}

BOOL CTtlDsp::OnInitDialog() 
{
BOOL syoguti_err = FALSE;
CWnd	*pALT_BUTTON = GetDlgItem( IDC_ALT );

	// TODO: この位置に初期化の補足処理を追加してください
//	ControlInit( 342, 204, -1 );

	ICSDialog::OnInitDialog();
	
	if( pTTLDSP )
	{
		if( pTTLDSP->m_Mode == MODE_NORMAL )
			GetDlgItem( IDCANCEL )->ShowWindow( SW_HIDE );
		else
		{
			GetDlgItem( IDOK )->SetWindowText( "終了" );
			if( pTTLDSP->m_Mode == MODE_END ) {
				GetDlgItem( IDCANCEL )->SetWindowText( "戻る" );
				GetDlgItem( IDCANCEL )->ShowWindow( SW_SHOW );
			}
			else {
				GetDlgItem( IDCANCEL )->ShowWindow( SW_HIDE );
			}
		}

		this->SetWindowText( pTTLDSP->TITLE_BAR );	//タイトルバー

		m_linp.SetHorzGrid( 1, RGB( 0, 0, 0), LINP_GRID_SOLID );	//水平線指定
		m_linp.SetVartGrid( 4, (LPUNKNOWN)vcolinfo );				//垂直線指定
		m_linp.SetBackColor( RGB_PAPER_WHITE );	//背景色

		//表示項目登録
		mk_linedef( ColmN, TTLDSP_LINE_DEF, pTTLDSP );	//表示項目設定
		m_linp.SetColumn( ColmN, (LPUNKNOWN)TTLDSP_LINE_DEF );	//登録 
		//全表示データイニシャライズ
		m_linp.RemoveAll();

		//合計データ表示
		syoguti_err = dsp_ttldsp( ColmN, TTLDSP_DATA, pTTLDSP );
	}

	if( m_pAltTxt )
	{
		pALT_BUTTON->SetWindowText( m_pAltTxt );
		pALT_BUTTON->ShowWindow( SW_SHOW );
	}
	else
	{
		pALT_BUTTON->ShowWindow( SW_HIDE );
	}

	if( syoguti_err == FALSE )
	{
		GetDlgItem( IDC_STATIC1 )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_ICSDISPCTRL1)->ShowWindow( SW_HIDE );
	}
	else if( pTTLDSP->m_Mode == MODE_END || pTTLDSP->m_Mode == MODE_COSEL )
		Buzzer();				

	ICSDialog::OnInitDialogEX();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/*****************************************************
	現在のビュー拡大比率に従って表示サイズを変える
******************************************************/
//void CTtlDsp::kakudai(CWnd* wnd)
//{
//	RECT	newrect;
//	double rate_w, rate_h;
//
//	get_viewrate( rate_w, rate_h );	//ビューサイズ比率をゲット
//	wnd->GetWindowRect( &newrect );
//	newrect.bottom = newrect.top + ((newrect.bottom - newrect.top ) * rate_h);
//	newrect.right = newrect.left + ((newrect.right - newrect.left ) * rate_w);
//
//	wnd->SetWindowPos( 0, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_NOMOVE|SWP_NOZORDER );
//}

void CTtlDsp::OnSetFocus(CWnd* pOldWnd) 
{
	ICSDialog::OnSetFocus(pOldWnd);
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	GotoDlgCtrl( GetDlgItem( IDOK ) );
}


void CTtlDsp::OnAlt() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	EndDialog( m_IDALT );

}

BOOL CTtlDsp::PreTranslateMessage(struct tagMSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F9 )
	{
		GetDlgItem( IDC_ALT )->PostMessage( WM_LBUTTONDOWN, 0, 0 );
		GetDlgItem( IDC_ALT )->PostMessage( WM_LBUTTONUP, 0, 0 );
		return 1;
	}
	return ICSDialog::PreTranslateMessage(pMsg);
}
