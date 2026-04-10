// TkswkDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "resource.h"

#include "DBInpSubFunc.h"

#include "TkswkDlg.h"

#include "SwkSelDlg.h"
#include "TksrchDlg.h"
#include "SwlistSelDlg.h"

#include "TKswkCpyMsgDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTkswkDlg ダイアログ

// 仕訳リスト カラム
#define SWC_NO		0
#define SWC_DEBT	1
#define SWC_CRED	2

#define SWC_MAX		(SWC_CRED+1)
#define TKY_LMAX	(m_swkMax-m_swkCnt)

#define ONCOLOR		CL_BLACK
#define	OFFCOLOR	CL_TKBACK

extern
int num_chk( char* bf, int ln );

//------------------------------------
//	仕訳部分データの初期化
//
//
//------------------------------------
void init_swkdata( SWKCOD_DATA*	pdata )
{
	pdata->sc_dbt.Empty();
//	pdata->sc_dbtname.Empty();

	pdata->sc_cre.Empty();
//	pdata->sc_crename.Empty();
	
	pdata->sc_sgn = 0;
	pdata->sc_chg = 0;
}


//------------------------------------
//	仕訳摘要 filter 文字列取得
//
//
//------------------------------------
void filter_swkdata( CString* filter, SWKCOD_DATA* pdata )
{
	CString tmp;

	if( pdata->sc_dbt.IsEmpty() )	*filter = "stdbt IS NULL";
	else {
		filter->Format( "stdbt = '%s'", pdata->sc_dbt );
	}
	*filter += " AND ";

	if( pdata->sc_cre.IsEmpty() )	tmp = "stcre IS NULL";
	else {
		tmp.Format( "stcre = '%s'", pdata->sc_cre );
	}

	*filter += tmp;
}


// WM_USER_1, の WM_SETFOCUS の時のﾊﾟﾗﾒｰﾀ
// WM_RETROFOCUS でも使用
//
enum SF_OPDEF {

	SF_FIRSTPOS,
	SF_ONSIZE,
	SF_SWK_DEBT,
	SF_SWK_CRED,
	SF_DBEDT,
	SF_TKY_INP
};


CTkswkDlg::CTkswkDlg(CWnd* pParent /*=NULL*/)
	: CBaseDlg(CTkswkDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTkswkDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
	CBaseDlg::m_Xdl = 492;
	CBaseDlg::m_Ydl = 260;

	m_Didx = 0;
	m_CurDidx = 0;
	m_Dclm = -1;
	m_Tkyidx = -1;
	m_PosMarkIndex = 0;
	m_InsIdx = -1;

	m_bFocusSet = FALSE;
	m_bInitOK = FALSE;
	m_bSetCred = FALSE;

	m_bFirstShow = FALSE;
	m_bNoCurSwkCheck = FALSE;
//	m_pPrtIdx = NULL;

	m_bTkTopDisp = FALSE;
	m_swkMax = 10000;
	m_swkCnt = 0;

	init_swkdata( &m_CurData );
}


void CTkswkDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTkswkDlg)
	DDX_Control(pDX, IDC_ICSLINP8_SWK, m_listswk);
	DDX_Control(pDX, IDC_ICSLINP8_TKY, m_listtky);
	DDX_Control(pDX, IDC_BUTTON_COPY, m_Copy);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_ICSDBEDTCTRL1, m_DBEdt);
}


// 仮想関数
// ウィンド表示／非表示
void CTkswkDlg::WndShow(BOOL bShow)
{
	CBaseDlg::WndShow( bShow );
	// STATIC の位置調整
	if( bShow ) {

		if( !m_bFirstShow ) {
			// 初回 選択ダイアログ表示
			m_bFirstShow = TRUE;
			// 摘要辞書画面より表示ずみ
			if( ! CSWKselDlg::bSwkDlgShow ) {
				SWKselDisp( TRUE );
				CSWKselDlg::bSwkDlgShow = TRUE;
			}
			GetDlgItem(IDC_STATIC_ARROW)->Invalidate();
			GetDlgItem(IDC_STATIC_ARROW)->UpdateWindow();
			GetDlgItem(IDC_ST_REGCNT)->Invalidate();
			GetDlgItem(IDC_ST_REGCNT)->UpdateWindow();
		}
		else {
			if( m_Didx != -1 ) {
				m_InsIdx = -1;
				m_CurData = m_Swcd[m_Didx];
				TkyoInpSet( m_Didx );	// 摘要エリア表示更新
			}
		}
	}
}

// 仕訳選択画面の表示
void CTkswkDlg::SWKselDisp( BOOL bFirst/*=FALSE*/ )
{

	CSWKselDlg dlg( this, TRUE );
//	dlg.SetSize(-1,-1,m_Xrate, m_Yrate);
	dlg.SetZmClass( m_pZm );

	c_set( IDC_ICSLINP8_SWK );
//	m_listswk.RedrawControl();
	int max, addidx, i, pos;

	if( dlg.DoModal() == IDOK ) {

		SWKCOD_DATA sc, srec, tmp;
		sc.sc_dbt = dlg.m_Selected.debcode;
		sc.sc_cre = dlg.m_Selected.crecode;
		int datanum;

		CString filter;
		// チェック元の摘要コード
		filter_swkdata( &filter, &sc );

		m_pZm->strec->Requery( filter );
		pos = m_pZm->strec->GetRecordCount();

		// 追加
		if( pos <= 0 && m_swkCnt < m_swkMax ) {

			// 一致する配列まで検索
			max = m_Swcd.GetSize();
			datanum = max;
			pos = 0;
			for( i = 0; i < max; i++ ) {
				srec = m_Swcd[i];
				srec.sc_dbt.TrimRight();
				srec.sc_cre.TrimRight();

				if( srec.sc_dbt == sc.sc_dbt && srec.sc_cre == sc.sc_cre ) {
					pos = i+1;
					break;
				}
			}
			srec.sc_dbt = sc.sc_dbt;
			srec.sc_cre = sc.sc_cre;
			srec.sc_sgn = 0;
			srec.sc_chg = 0;

			if( !pos )	{
				addidx = -1;
				max = m_Swcd.GetSize();
				if( max > 0 ) {
					tmp = m_Swcd[max-1];
					if( tmp.sc_dbt.IsEmpty() && tmp.sc_cre.IsEmpty() ) {
						addidx = (max-1);
					}
				}
				int n;
				if( addidx != -1 ) {
					m_Swcd[addidx] = srec;
					n = addidx;
				}
				else {
					m_Swcd.Add( srec );
					n = m_Swcd.GetSize();
					if( n > 0 )	n--;
				}

				SiwakeInpSet( FALSE );
				m_CurData = srec;
				TkyoInpSet( n );
				m_Didx = n;
				m_Tkyidx = 0;
			}
			else {
				m_CurData = srec;
				TkyoInpSet( pos );
				m_Didx = pos;
				m_Tkyidx = 0;
			}
			// カレントレコードをすでにセットしているので、EditOn で更新しない
			m_bNoCurSwkCheck = TRUE;

			m_listswk.SetPosition( m_Didx );
			m_listswk.SetInputPositionEX( m_Didx , 1 );
			TorokDisp();
		}
		else {
			// 一致する配列まで検索
			int max = m_Swcd.GetSize();
			datanum = max;
			pos = 0;
			for( int i = 0; i < max; i++ ) {
				srec = m_Swcd[i];
				srec.sc_dbt.TrimRight();
				srec.sc_cre.TrimRight();

				if( srec.sc_dbt == sc.sc_dbt && srec.sc_cre == sc.sc_cre ) {
					pos = i+1;
					break;
				}
			}
			// 修正
			if( !pos )	{
				if( !datanum ) 
					pos = 1; // 最大まで登録されていて追加できない時 TOPに
				else
					pos = datanum;
			}
			// カレントレコードをセットしているので、EditOn で更新しない
			m_bNoCurSwkCheck = TRUE;

			m_listswk.SetPosition(pos-1);
			m_listswk.SetInputPositionEX( pos-1, SWC_DEBT );
			m_Didx = pos-1;
			m_Tkyidx = 0;
			m_CurData = m_Swcd[m_Didx];
			TkyoInpSet( m_Didx );
		}
	}
	else {
		// 初回表示呼び出しは 一番上にする
		if( bFirst ) {
			m_Didx = 0;
			m_Tkyidx = 0;
			m_CurData = m_Swcd[m_Didx];
			// カレントレコードをすでにセットしているので、EditOn で更新しない
			m_bNoCurSwkCheck = TRUE;

			m_listswk.SetPosition(0);
			m_listswk.SetInputPositionEX( 0, SWC_DEBT );
			TkyoInpSet( m_Didx );
		}
	}

	SetNowLineBkColor( m_Didx, TRUE );

}


// 仮想関数
// 摘要追加時のウィンド表示／非表示
void CTkswkDlg::WndShow_Add(BOOL bShow)
{
	CBaseDlg::WndShow( bShow );
	// STATIC の位置調整
	if( bShow ) {
		c_set( IDC_ICSLINP8_SWK );
//		m_listswk.RedrawControl();

//		m_listswk.SetPosition(0);
//		m_listswk.SetInputPositionEX( 0, SWC_DEBT );
		TkyoInpSet( m_Didx );	// 摘要エリア表示更新
	}
}


void CTkswkDlg::FocusMove( int direction ) 
{
	CWnd* pwnd;
	pwnd = GetFocus();

	if( pwnd ) {
		int id = pwnd->GetDlgCtrlID();

		if( id == IDC_ICSLINP8_SWK || m_listswk.IsChild( pwnd )) {
			if( direction > 0 ) { // 次項目
				if( m_listswk.GetColumn() == SWC_CRED )
					c_set( IDC_ICSLINP8_TKY );
				else
					m_listswk.SetInputPositionEX( m_Didx, SWC_CRED );
			}
			else {
				// 前項目
				if( m_listswk.GetColumn() == SWC_DEBT )
					c_set( IDC_ICSLINP8_TKY );
				else
					m_listswk.SetInputPositionEX( m_Didx, SWC_DEBT );
			}
		}
		else  { 
			c_set( IDC_ICSLINP8_SWK );
			if( direction < 0 ) 
				PostMessage( WM_USER_1, WM_SETFOCUS, SF_SWK_CRED ); // 貸方に
		//	short clm = SWC_DEBT;
		//	m_listswk.SetInputPositionEX( m_Didx, clm );
		}
	}
}

void CTkswkDlg::SetFirstPosition() 
{
	if( m_listtky.GetEnabled() == FALSE ) {
		c_set( IDC_ICSLINP8_SWK );
		m_listswk.RedrawControl();
		short clm = SWC_DEBT;
		m_listswk.SetInputPositionEX( m_Didx, clm );
	}
	else {
		c_set( IDC_ICSLINP8_TKY );
	}
	SetNowLineBkColor( m_Didx );

//TRACE( "swkdlg-> SetFirstPosition\n" );
}


// 仮想関数
// 会社確認後の初期処理
void CTkswkDlg::InitWork( double x, double y )
{
	CBaseDlg::InitWork( x, y );

ASSERT( m_pZm->strec != NULL );

	CString filter;
	filter = _T( "seq = 26" );
	m_pZm->dcntl->Requery( filter );
	m_swkMax = m_pZm->dcntl->mcnt;

	SiwakeInpSet( TRUE );
	m_CurData = m_Swcd[m_Didx];
	TkyoInpSet( m_Didx );

	TorokDisp();

	m_bFocusSet = TRUE;

	m_bInitOK = TRUE;
}

// 仮想関数
// ツールバー状態
BOOL CTkswkDlg::IsEnableToolBar( int nID )
{
	BOOL bEnable = FALSE;
	CWnd* pwnd = GetFocus();
	int id;

	switch( nID ) {
	case ID_TLBAR_F8:
		if( pwnd ) {
			id = pwnd->GetDlgCtrlID();
			if( id == IDC_ICSLINP8_TKY || m_listtky.IsChild( pwnd ) ) {
				bEnable = TRUE;
				break;
			}
		}
		bEnable = FALSE;
		break;
	case ID_TLBAR_F9:
		bEnable = TRUE;
		break;
	default:
		break;

	}

	return bEnable;
}



BEGIN_MESSAGE_MAP(CTkswkDlg, CBaseDlg)
	//{{AFX_MSG_MAP(CTkswkDlg)
	ON_BN_CLICKED(IDC_BUTTON_COPY, OnButtonCopy)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_USER_1, OnUser1_Msg)
	ON_MESSAGE( WM_RETROFOCUS, OnRetroFocus)
	ON_MESSAGE( WM_LIST_INSDEL, OnListInsDel)
END_MESSAGE_MAP() 

/////////////////////////////////////////////////////////////////////////////
// CTkswkDlg メッセージ ハンドラ
BOOL CTkswkDlg::OnInitDialog() 
{
	int len = GetTekiyoLength( m_pZm );
	if( len > 0 )	m_TKYO_LEN = len;

	CBaseDlg::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	m_listswk.EnableDelete(0);
	m_listtky.EnableDelete(0);
	m_DBEdt.EnableDelete(0);

	// リスト入力設定
	MakeTkrenFrm();
	MakeSiwakeFrm();
	
	m_DBEdt.SetVector( FALSE );
	m_DBEdt.SetNumeric( TRUE );
	m_DBEdt.SetKana( TRUE );
	m_DBEdt.SetZenkaku( FALSE );
	m_DBEdt.SetAscii( TRUE );
	m_DBEdt.SetAlphabet( TRUE );
	m_DBEdt.SetFormat( FALSE ); // カンマ編集
	m_DBEdt.SetMinval( "0" );
	m_DBEdt.SetMaxval( "0" );
	m_DBEdt.SetMaxLen( KMKCD_LN );
	m_DBEdt.SetInverse( TRUE );
	m_DBEdt.SetKanaTerm(TRUE, 1, ICSDBEDT_KTYPE_KANA );
	m_DBEdt.ImeEndMode( TRUE );
	m_DBEdt.SetCmove( TRUE );

	m_DBEdt.ShowWindow( SW_HIDE );

	ICSDialog::OnInitDialogEX();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}


//	仕訳登録部分
//	表形式入力　設定
void CTkswkDlg::MakeSiwakeFrm()
{

	LINP_COLUMN LC[4];
	LINP_VCOLORINFO	LVCOL[4];

	//　番号
	LC[0].clm_kind = LINP_KIND_STRING;
	LC[0].clm_max = 5;
	LC[0].clm_attr = LINP_ATTR_READONLY | LINP_ATTR_CENTER| LINP_ATTR_BOTTOM;
	LC[0].clm_width = 30;
	LC[0].clm_tbcolor = -1;
	LC[0].clm_ttcolor = -1;

	LC[1].clm_kind = /*LINP_KIND_KAMOKU*/LINP_KIND_STRING ;
	LC[1].clm_max = 14;
	LC[1].clm_attr = LINP_ATTR_CENTER | LINP_ATTR_BOTTOM | LINP_ATTR_ANK | LINP_ATTR_ONETERM;

	LC[1].clm_width = 74;
	LC[1].clm_title = "借方科目";
	LC[1].clm_tbcolor = -1;
	LC[1].clm_ttcolor = -1;

	LC[2].clm_kind = /*LINP_KIND_KAMOKU*/LINP_KIND_STRING;
	LC[2].clm_max = 14;
	LC[2].clm_attr = LINP_ATTR_CENTER | LINP_ATTR_BOTTOM | LINP_ATTR_ANK | LINP_ATTR_ONETERM;
	LC[2].clm_width = 74;
	LC[2].clm_title = "貸方科目";
	LC[2].clm_tbcolor = -1;
	LC[2].clm_ttcolor = -1;

	LC[0].clm_title = _T( "番号" );
	LC[1].clm_title = _T( "借　方" );
	LC[2].clm_title = _T( "貸　方" );

//	m_listswk.SetColumn( 3, (LPUNKNOWN)LC );
	linp_setclm( IDC_ICSLINP8_SWK, 3, LC, CICSListInp);

	// 表形式入力　グリッド線の設定	
//	m_listswk.SetHorzGrid( 5, CL_RED, LINP_GRID_SOLID );
	linp_horzgrid( IDC_ICSLINP8_SWK, 5, CL_RED, LINP_GRID_SOLID, CICSListInp );

	LVCOL[0].pos = 0;
	LVCOL[0].color = CL_GREEN;
	LVCOL[0].type = LINP_GRID_SOLID;

	LVCOL[1].pos = 1;
	LVCOL[1].color = CL_GREEN;
	LVCOL[1].type = LINP_GRID_SOLID;

//	m_listswk.SetVartGrid( 2, (LPUNKNOWN)LVCOL );
	linp_vartgrid( IDC_ICSLINP8_SWK, 2, (LPUNKNOWN)LVCOL, CICSListInp );


}

//
//	摘要 表形式入力　設定
void CTkswkDlg::MakeTkrenFrm()
{
	LINP_COLUMN LC[4];
	LINP_VCOLORINFO	LVCOL[4];

	// 摘要コード
	l_defn( 0x16 );
	
	LC[0].clm_kind = LINP_KIND_NUMERIC;
	LC[0].clm_max = 6;
	LC[0].clm_format = NUM_FMT;
	LC[0].clm_title = "摘要ｺｰﾄﾞ";
	LC[0].clm_attr =   LINP_ATTR_CENTER | LINP_ATTR_BOTTOM;
	LC[0].clm_width = 38;	//50;
	LC[0].clm_tbcolor = CL_DKBLUE;
	LC[0].clm_ttcolor = CL_WHITE;
	
	// 摘要名称
	LC[1].clm_kind = LINP_KIND_STRING;
	LC[1].clm_max = (m_TKYO_LEN/2);
	LC[1].clm_title = "摘 要 名 称";
	LC[1].clm_attr = LINP_ATTR_READONLY | LINP_ATTR_KANJI | LINP_ATTR_CENTER | LINP_ATTR_BOTTOM;
	LC[1].clm_width = 172;	//140;
	LC[1].clm_tbcolor = CL_DKBLUE;
	LC[1].clm_ttcolor = CL_WHITE;

//	m_listtky.SetColumn( 4, (LPUNKNOWN)LC );
	linp_setclm( IDC_ICSLINP8_TKY, 2, LC, CICSListInp);

	// 表形式入力　グリッド線の設定	
//	m_listtky.SetHorzGrid( 5, CL_RED, LINP_GRID_SOLID );
	linp_horzgrid( IDC_ICSLINP8_TKY, 5, CL_RED, LINP_GRID_SOLID, CICSListInp );

	LVCOL[0].pos = 0;
	LVCOL[0].color = CL_GREEN;
	LVCOL[0].type = LINP_GRID_SOLID;

	LVCOL[1].pos = 1;
	LVCOL[1].color = CL_GREEN;
	LVCOL[1].type = LINP_GRID_SOLID;

//	m_listtky.SetVartGrid( 3, (LPUNKNOWN)LVCOL );
	linp_vartgrid( IDC_ICSLINP8_TKY, 2, (LPUNKNOWN)LVCOL, CICSListInp );

}


// 登録数表示
void CTkswkDlg::TorokDisp()
{

	char bf[64];

	sprintf_s( bf, sizeof bf, "%5d", m_swkCnt );
	GetDlgItem(IDC_STDSP_RGCNT)->SetWindowText( bf );
	sprintf_s( bf, sizeof bf, "%5d", m_swkMax );		
	GetDlgItem(IDC_STDSP_RGMAX)->SetWindowText( bf );

}


// カレント仕訳摘要レコードが 借／貸両方に表示する科目か
BOOL CTkswkDlg::BothDispSwtkyo( CString& dbt, CString& cre )
{
	BOOL ret = FALSE;

	// 借方 あり 貸方 なし
	DBKNREC* pKn;
	if( ! dbt.IsEmpty() && cre.IsEmpty() ) {

		pKn = DB_PjisToKnrec( dbt, m_pZm );
		if( pKn && !(pKn->knatt&0x01) )
			ret = TRUE;
	}
	else if( dbt.IsEmpty() && ! cre.IsEmpty() ) {
		// 借方 なし 貸方 あり 

		pKn = DB_PjisToKnrec( cre, m_pZm );
		if( pKn &&  (pKn->knatt&0x01) )
			ret = TRUE;
	}

	return ret;
}


// bMastRead ... データベースより読み込む(TRUE 時)
//
int CTkswkDlg::SiwakeInpSet( BOOL bMastRead )
{
//	BOOL	bBoth;
	CString dbt, cre;
	int i;

//	m_listswk.RemoveAll();

	CSTREC* pst;
	pst = m_pZm->strec;

	pst->Requery("");
	pst->MoveFirst();
	m_swkCnt = pst->GetRecordCount();

	dbt.Empty();
	cre.Empty();

	SWKCOD_DATA scd;

	if( bMastRead ) {
		m_Swcd.RemoveAll();

		if( pst->st != -1 ) {
			i = 0;
			do {
				
				if( dbt != pst->stdbt || cre != pst->stcre ) {
					// リストインプットにセット
					dbt = pst->stdbt;
					cre = pst->stcre;

					scd.sc_dbt = dbt;
					scd.sc_cre = cre;
					scd.sc_sgn = 1;
					scd.sc_chg = 0;

					m_Swcd.Add( scd );

					i++;
				}
			} while( pst->MoveNext() == 0 );
		}

		SwkInpAllSetSub();
	}
	else {
		// 配列から
		int max = m_Swcd.GetSize();
		SwkInpAllSetSub();
	}

//#ifdef CLOSE
	// 新規追加ラインを入れる
//	if( IsEnableAddNewSwkLine() ) {
//		m_listswk.AddNewLine( TRUE );
//		SetNewSwkLine();
//	}
//#endif

	return 0;
}


//-------------------------------------------------------
//	仕訳リストにセット
//	前もって m_Swcd にデータベースより読込む必要あり
//
//-------------------------------------------------------
void CTkswkDlg::SwkInpAllSetSub()
{
	DBKNREC*	pKn;
	BOOL bBoth;

	LINP_DATA	LD[10];
	LPUNKNOWN	lp;
	lp = (LPUNKNOWN)&LD[0];

	CString	kmkcod;
	CString buf;

	int max, lcnt, idx;
	max = lcnt = 0;
	max = m_Swcd.GetSize();
	lcnt = max;

	m_listswk.RemoveAll();
	m_listswk.RedrawControl();

	for( idx = 0; idx < max; idx++ ) {

		SWKCOD_DATA& sd = m_Swcd[idx];
		bBoth = BothDispSwtkyo( sd.sc_dbt, sd.sc_cre );

		// 番号
		LD[SWC_NO].dat_string.Format( "%d", idx+1 );
		LD[SWC_NO].dat_attr = LINP_DISP_COLOR;
		LD[SWC_NO].dat_fc = ONCOLOR;
		LD[SWC_NO].dat_bc = (bBoth) ? CL_YELLOW : OFFCOLOR;
		LD[SWC_NO].dat_initflg = FALSE;

		for( int n = SWC_DEBT; n <= SWC_CRED; n++ ) {
			// 借・貸 科目
			LD[n].dat_string.Empty();
			LD[n].dat_attr = LINP_DISP_COLOR;
			LD[n].dat_fc = ONCOLOR;
			LD[n].dat_bc = OFFCOLOR;

			kmkcod = (n==SWC_DEBT) ? sd.sc_dbt : sd.sc_cre;

			if( !kmkcod )
				LD[n].dat_initflg = TRUE;
			else {
				LD[n].dat_initflg = FALSE;

				pKn = DB_PjisToKnrec( kmkcod, m_pZm );
				
			//	memset( LD[n].dat_kamoku.kamoku_name, 0, 14 );
				if( pKn ) {
					LD[n].dat_string.Format( "%.14s", pKn->knnam );
			//		wsprintf( (char *)LD[n].dat_kamoku.kamoku_name, "%.14s", pKn->knnam );
			//		LD[n].dat_kamoku.kamoku_code = pKn->kncod;
			//		LD[n].dat_kamoku.kamoku_edaban = 0xffff;
			//		LD[n].dat_kamoku.kamoku_num = 0;
				}
				else LD[n].dat_initflg = TRUE;
			}
		}

		m_listswk.SetData( idx, (LPUNKNOWN)LD, TRUE );

	}

	// 新規入力分
	if( IsEnableAddNewSwkLine() ) {
		// 番号
		m_listswk.AddNewLine( TRUE );
		SetNewSwkLine();

		// 配列に仕訳データを追加
		SWKCOD_DATA scdata;
		scdata.sc_sgn = 0;
		scdata.sc_chg = 0;

		m_Swcd.SetAtGrow( idx, scdata );
	}
}



// 新規仕訳行が追加可能か？
BOOL CTkswkDlg::IsEnableAddNewSwkLine()
{
	int cnt;
	cnt = m_Swcd.GetSize();
	SWKCOD_DATA sc;
	BOOL bAddOK = FALSE;

	if( m_swkCnt < m_swkMax ) {
		if( cnt > 0 ) {
			sc = m_Swcd[cnt-1];
			if( (! sc.sc_dbt.IsEmpty() || ! sc.sc_cre.IsEmpty() ) ) {
				bAddOK = TRUE;
			}
		}
		else {
			bAddOK = TRUE;
		}
	}
	
	return bAddOK;
}

//-------------------------------------------------
//	新規仕訳行のセット
//
//-------------------------------------------------
void CTkswkDlg::SetNewSwkLine()
{
	LINP_DATA	LD[4];

	int newidx = m_listswk.GetCount();
	newidx--;

	// 番号
	LD[SWC_NO].dat_string.Format( "%d", newidx+1 );
	LD[SWC_NO].dat_attr = LINP_DISP_COLOR;
	LD[SWC_NO].dat_fc = ONCOLOR;
	LD[SWC_NO].dat_bc = OFFCOLOR;
	LD[SWC_NO].dat_initflg = FALSE;

	for( int n = SWC_DEBT; n <= SWC_CRED; n++ ) {
		// 借・貸 科目 なし
	//	LD[n].dat_string.Empty();
		LD[n].dat_string = _T( "aiueo" );
		LD[n].dat_attr = LINP_DISP_COLOR;
		LD[n].dat_fc = ONCOLOR;
		LD[n].dat_bc = OFFCOLOR;
		LD[n].dat_initflg = TRUE;

	}
	m_listswk.SetData( newidx, (LPUNKNOWN)LD, TRUE );

}

// 摘要リストインプットセット
//		m_CurData にセットされている仕訳の 摘要リストを表示
//		この関数を呼ぶ前に、m_Didx と、m_CurData が一致させておく
//
int CTkswkDlg::TkyoInpSet( int idx )
{
	LINP_DATA	LD[4];
	LPCTSTR		p;
	int tkcod;
	CString Tknam, codbf;

	SWKCOD_DATA& sc = m_CurData;

#ifdef _DEBUG
	SWKCOD_DATA& sc_chk = m_Swcd[idx];
	ASSERT( sc.sc_dbt == sc_chk.sc_dbt && sc.sc_cre == sc_chk.sc_cre );
#endif

	if( sc.sc_sgn == 0 ) {
		// 新規ライン時
		// 次回は摘要登録欄を先頭より表示する
		m_bTkTopDisp = TRUE;
		m_listtky.RemoveAll();
		m_listtky.AddNewLine(TRUE);	// ダミーで追加 
//		m_listtky.RedrawControl();
		m_listtky.SetEnabled(FALSE);
		m_listtky.SetBackColor(CL_GRAY);
		return -1;
	}
	else {
		m_listtky.RemoveAll();
		m_listtky.SetBackColor(CL_WHITE);
		m_listtky.SetEnabled(TRUE);
	}

ASSERT( idx < m_swkMax );

	if( idx >= m_swkMax || idx < 0 )
		return -1;

	CSTREC* pst;
	pst = m_pZm->strec;

	int i = 0;

	CString filter;
	// チェック元の摘要コード
	filter_swkdata( &filter, &sc );

	if( pst->Requery( filter ) != 0 ) {
		SetNewTkyoInput( i );
		return -1;
	}

	pst->MoveFirst();

	do {
		Tknam = "";
		tkcod = pst->stcod;
		if( tkcod > 0 ) {
			if( tkyngetr( tkcod, m_pZm ) == 0 ) {
				Tknam.Format( "%-40.40s", m_pZm->tkrec->tkname );
			}
			else continue;
		}
		else continue;

		// 摘要コード
		codbf.Format( "%06d", tkcod );
		p = codbf;
		l_input( LD[0].dat_numeric, (void *)p );
		LD[0].dat_attr = LINP_DISP_COLOR;
		LD[0].dat_fc = ONCOLOR;
		LD[0].dat_bc = OFFCOLOR;

		if( tkcod )
			LD[0].dat_initflg = FALSE;
		else
			LD[0].dat_initflg = TRUE;

		// 摘要名称
		p = Tknam;
	//	kleng = kjlen( (void*)p, 20 );
		LD[1].dat_string = Tknam;
		LD[1].dat_attr = LINP_DISP_COLOR;
		LD[1].dat_fc = ONCOLOR;
		LD[1].dat_bc = OFFCOLOR;
		if( tkcod )
			LD[1].dat_initflg = FALSE;
		else
			LD[1].dat_initflg = TRUE;

		m_listtky.SetData( i, (LPUNKNOWN)LD, TRUE );

		i++;

	} while( pst->MoveNext() == 0 );

//TRACE( "tkyoinpset %d\n", i );

	SetNewTkyoInput( i );

	return 0;
}

// 新規摘要ライン追加
void CTkswkDlg::SetNewTkyoInput( int index )
{
	LINP_DATA	LD[4];

	// 摘要コード
	::ZeroMemory( LD[0].dat_numeric, sizeof LD[0].dat_numeric );
	LD[0].dat_attr = LINP_DISP_COLOR;
	LD[0].dat_fc = ONCOLOR;
	LD[0].dat_bc = OFFCOLOR;
	LD[0].dat_initflg = TRUE;
	// 摘要名称
	LD[1].dat_string = "";
	LD[1].dat_attr = LINP_DISP_COLOR;
	LD[1].dat_fc = ONCOLOR;
	LD[1].dat_bc = OFFCOLOR;
	LD[1].dat_initflg = TRUE;

	m_listtky.SetData( index, (LPUNKNOWN)LD, TRUE );
}


/*======================================================================
	現在ラインの色変更
	※仕訳リスト入力で SetInputPosition をした時は必ずこの関数を呼ぶ

	int setidx ... 現在行インデックス
	BOOL bDraw ... 強制再描画
  ======================================================================*/
void CTkswkDlg::SetNowLineBkColor( int setidx, BOOL bDraw /*=FALSE*/ )
{
	static int prev_idx = -1;
	int max;
	LINP_DATA	LD[4];

	if( !bDraw ) {
		// 現在と 同じラインか？
		if( setidx == prev_idx )
			return;
	}

	BOOL bBoth;
	SWKCOD_DATA sd;

	// 以前の行を元に戻す
	if( prev_idx >= 0 ) {
		max = m_Swcd.GetSize();
		if( prev_idx < max ) {
			sd = m_Swcd[prev_idx];
			bBoth = BothDispSwtkyo( sd.sc_dbt, sd.sc_cre );
		}
		else	bBoth = FALSE;

		m_listswk.GetData( prev_idx, (LPUNKNOWN)LD );

		for( int i = 0; i < SWC_MAX; i++ ) {
			LD[i].dat_attr = LINP_DISP_COLOR;
			LD[i].dat_fc = CL_BLACK;
			if( i == SWC_NO )
				LD[i].dat_bc = (bBoth) ? CL_YELLOW : CL_TKBACK;
			else
				LD[i].dat_bc = CL_TKBACK;
		}
		m_listswk.SetData( prev_idx, (LPUNKNOWN)LD, TRUE );
	}

	// 現在の行に 青ライン
	max = m_Swcd.GetSize();
	if( setidx < max ) {
		sd = m_Swcd[setidx];
		bBoth = BothDispSwtkyo( sd.sc_dbt, sd.sc_cre );
	}
	else	bBoth = FALSE;

	m_listswk.GetData( setidx, (LPUNKNOWN)LD );
	for( int i = 0; i < SWC_MAX; i++ ) {
		LD[i].dat_attr = LINP_DISP_COLOR;
		LD[i].dat_fc = CL_BLACK;
		if( i == SWC_NO )
			LD[i].dat_bc = (bBoth) ? CL_YELLOW : CL_WINE_BLUE;
		else
			LD[i].dat_bc = CL_WINE_BLUE;
	}
	m_listswk.SetData( setidx, (LPUNKNOWN)LD, TRUE );
	
	prev_idx = setidx;
}


// idx が 現在入力中の 仕訳摘要範囲内か？
BOOL CTkswkDlg::IsValidSwtkyoIdx( int idx )
{
	int cnt = m_Swcd.GetSize();

	if( idx < cnt ) {
		SWKCOD_DATA sc = m_Swcd[idx];

		// 新規行は 除く
		if( sc.sc_sgn != 0 )
			return TRUE;
	}

	return FALSE;
}


/*===========================================
	新規入力ラインを追加できるか

  ===========================================*/
void CTkswkDlg::SwtkNewlineCheck( short index, short clm  )
{
	LINP_DATA	LD[4];
	int cnt, ar_cnt;

	if( clm == SWC_CRED ) {
		cnt = m_listswk.GetCount();
		ar_cnt = m_Swcd.GetSize();

		// 最終ラインのときのみ
	//	if( m_Stsub.m_pHead->st_swmax > index+1 &&
	//		m_Stsub.m_pHead->st_swcnt == cnt &&	cnt == index+1 ) {
		if( m_swkCnt < m_swkMax && ar_cnt == cnt && cnt == index+1 ) {

			m_listswk.AddNewLine( TRUE );
			SetNewSwkLine();
		}
	}
}

//------------------------------------------------------
// 摘要コードエリアにデータセット
//	int index .......... インデックス
//	int code ........... 摘要ｺｰﾄﾞ
//	CString Kname ...... 摘要名称
//
//------------------------------------------------------
void CTkswkDlg::SetTkyListData( int index, int code, CString Kname )
{
	// データベースにセット
	int  seq;
	LINP_DATA LD[6];
	BOOL bNew = FALSE;
	char tmp[64] = {0};

	m_listtky.GetData( index, (LPUNKNOWN)LD );

	// 該当インデックスの摘要番号入力が修正か新規か？
	if( LD[0].dat_initflg )	bNew = TRUE;
	else {
		if( memcmp( tmp, LD[0].dat_numeric, 6 ) == 0 )
			bNew = TRUE;
	}

	char	sgn[10] = {0};

	SWKCOD_DATA& sc = m_CurData;

	if( bNew ) {
		int cnt = m_listtky.GetCount();

ASSERT( !(sc.sc_dbt.IsEmpty() && sc.sc_cre.IsEmpty()) );

		if( cnt <= index ) {
			seq = index + 1;
			m_pZm->strec->Append( (char*)(LPCTSTR)sc.sc_dbt, (char*)(LPCTSTR)sc.sc_cre, seq, code, 0, sgn );
			m_swkCnt++;
		}
		else {
			seq = index + 1;
			m_pZm->strec->Append( (char*)(LPCTSTR)sc.sc_dbt, (char*)(LPCTSTR)sc.sc_cre, seq, code, 0, sgn );
			m_swkCnt++;
		}
		// DB 登録サイン を更新
		if( sc.sc_sgn == 0 ) {
			sc.sc_sgn = 1;
			m_Swcd[m_Didx] = sc;
		}
		m_bDBupdate = TRUE;
		TorokDisp();
	}
	else if( code > 0 ) {
		// 修正
		seq = index + 1;
		m_pZm->strec->Correct( (char*)(LPCTSTR)sc.sc_dbt,  (char*)(LPCTSTR)sc.sc_cre, seq, seq, code, 0, sgn );
		m_bDBupdate = TRUE;
	}

	char pbuf[20];
	sprintf_s( pbuf, sizeof pbuf, "%d", code );

	LD[0].dat_initflg = FALSE;
	l_input( LD[0].dat_numeric, pbuf );
	LD[1].dat_initflg = FALSE;
	LD[1].dat_string = Kname;

	m_listtky.SetData( index, (LPUNKNOWN)LD, TRUE );

	if( m_InsIdx != -1 ) {
ASSERT( m_InsIdx == index );
		m_InsIdx = -1;
	}
}

// 借貸 登録科目は
// 番号エリアに 色をつける
void CTkswkDlg::BothKamokColorSet( short index )
{	
	LINP_DATA LD[6];

//	m_listswk.GetData( index, (LPUNKNOWN)LD );

	SWKCOD_DATA sd;
	int max = m_Swcd.GetSize();
	BOOL bBoth = FALSE;
	if( index < max ) {	
		bBoth = BothDispSwtkyo( sd.sc_dbt, sd.sc_cre );
		sd = m_Swcd[index];
	}

	if( index == m_Didx )
		LD[SWC_NO].dat_bc = (bBoth) ? CL_YELLOW : CL_WINE_BLUE;	// 現行の 青ライン
	else
		LD[SWC_NO].dat_bc = (bBoth) ? CL_YELLOW : CL_TKBACK;

//	m_listswk.SetData( index, (LPUNKNOWN)LD, TRUE );

}



/*===========================================
	仕訳コードのチェック

	return TRUE ..... チェックOK
		   FALSE .... すでに登録済み
					  仕訳ライン削除
					  エラー
  ===========================================*/
BOOL CTkswkDlg::CheckSwtkData( short idx, short colm, LPUNKNOWN data, BOOL bDelete/*= FALSE*/ )
{
	LINP_DATA	*LD = (LINP_DATA*)data;
	int		cnt, ar_cnt;
	BOOL	bSwkCheck = TRUE;
	BOOL	bNewLine = FALSE;

//	unsigned short code;
	CString code, str;
	DBKNREC*	pKn;
	char	debname[30] = {0}, crename[30] = {0};
	SWKCOD_DATA sc, swkdt, *pNowRec;

ASSERT( m_Didx == idx );

	cnt = m_listswk.GetCount();
	ar_cnt = m_Swcd.GetSize();

	// 最終行 で 新規ラインのとき
	if( cnt == idx+1 && cnt > ar_cnt ) {
		bNewLine = TRUE;
		pNowRec = &swkdt;
	
		bSwkCheck = FALSE; // チェックなし

		if( colm == SWC_DEBT );
		else {
			// データを取得
			LINP_DATA	ldata[4];
			m_listswk.GetData( idx, (LPUNKNOWN)ldata );

			if( !ldata[SWC_DEBT].dat_initflg ) {
				TV_kamokucodeConv( ldata[SWC_DEBT].dat_kamoku.kamoku_code, pNowRec->sc_dbt, CONV1_2 );
			//	pNowRec->sc_dbt = ldata[SWC_DEBT].dat_kamoku.kamoku_code;
			}
			else
				pNowRec->sc_dbt = "";

			if( !ldata[SWC_CRED].dat_initflg ) {
				TV_kamokucodeConv( ldata[SWC_CRED].dat_kamoku.kamoku_code, pNowRec->sc_cre, CONV1_2 );
			//	pNowRec->sc_cre = ldata[SWC_CRED].dat_kamoku.kamoku_code;
			}
			else
				pNowRec->sc_cre = "";
		}
	}
	else {
		swkdt = m_Swcd[idx];
		pNowRec = &swkdt;/*(m_Stsub.m_pRec + idx)*/;
	}

	// コードチェック
	sc.sc_dbt = pNowRec->sc_dbt;
	sc.sc_cre = pNowRec->sc_cre;

	// 入力データチェック
	if( !LD || LD->dat_kamoku.kamoku_code == 0xffff || LD->dat_kamoku.kamoku_code == 0 ) {
		code.Empty();
	}
	else {
		DWORD dwPjis;
		dwPjis = m_pZm->InpcToPjis( LD->dat_kamoku.kamoku_code );
		hl_rev( &dwPjis, 4 );
		code.Format( "%.08x", dwPjis );
		if( code.IsEmpty() )
			return FALSE;
	}

	// Delete 押下し 以外で 科目なしのときは 何もせず もどる
	// 科目ｺｰﾄﾞにない 数字＋Enter で １科目しかない時、削除されるのを防ぐため
	if( !bDelete && !code && !bNewLine ) {
		return FALSE;		
	}
	else {
		if( colm == SWC_DEBT )	sc.sc_dbt = code;
		else					sc.sc_cre = code;
	}
	
	// 仕訳削除
	if( sc.sc_dbt.IsEmpty() && sc.sc_cre.IsEmpty() ) {
		// 新規行は除く
		if( idx < ar_cnt ) {
			ICSMessageBox( "該当ラインの仕訳を削除します。", 0,0,0,this );

			DeleteSwtkData( idx );
		}
		// 最終新規行のときは、クリアデータをセット
//		if( m_listswk.GetCount() == (idx+1) && LD ) {
//			memset( LD->dat_kamoku.kamoku_name, 0, 14 );	
//			LD->dat_kamoku.kamoku_code = 0;
//			LD->dat_kamoku.kamoku_edaban = 0xffff;	
//			return TRUE;
//		}

		return FALSE;
	}

	// 同一仕訳チェック
	if( bSwkCheck ) {

		// 同一摘要チェック 09.17 /03
		SWKCOD_DATA tmp;
		tmp = *pNowRec;

		if( colm == SWC_DEBT )	tmp.sc_dbt = code;
		else					tmp.sc_cre = code;

		// 変更後の仕訳の摘要部分がすでにある レコードの摘要とかぶっていた場合は
		// 登録させない
		if( DoubleRegChkEX( pNowRec, &tmp ) ) {

			::ZeroMemory( debname, sizeof(debname));
			::ZeroMemory( crename, sizeof(crename));

			if( ! tmp.sc_dbt.IsEmpty() ) {
				pKn = DB_PjisToKnrec( tmp.sc_dbt, m_pZm );
				if( pKn ) memcpy( debname, pKn->knnam, sizeof(pKn->knnam ));
			}
			if( ! tmp.sc_cre.IsEmpty() ) {
				pKn = DB_PjisToKnrec( tmp.sc_cre, m_pZm );
				if( pKn ) memcpy( crename, pKn->knnam, sizeof(pKn->knnam ));			
			}
			str.Format( "　指定された仕訳\r\n\r\n　[借方]　%14.14s　[貸方]　%14.14s　\r\n\r\n\
　にはすでに同じ摘要が登録されている仕訳があります！\r\n　この仕訳には変更できません！", debname, crename );
			ICSMessageBox( str,0,0,0,this );
			return FALSE;
		}
	}

	if( LD ) {
		if( ! code.IsEmpty() ) {
			pKn = DB_PjisToKnrec( code, m_pZm );
			if( pKn == NULL )
				return FALSE;
		
			wsprintf( (char *)LD->dat_kamoku.kamoku_name, "%.14s", pKn->knnam );
		}
		else {
			memset( LD->dat_kamoku.kamoku_name, 0, 14 );	
		}

//		LD->dat_kamoku.kamoku_code = code;
		TV_kamokucodeConv( LD->dat_kamoku.kamoku_code, code, CONV2_1 );
		LD->dat_kamoku.kamoku_edaban = 0xffff;	
	}

	// 現在レコードにセット
	if( colm == SWC_DEBT )	pNowRec->sc_dbt = code;
	else					pNowRec->sc_cre = code;

	// 最終ライン(貸方)で まだデータが追加されていない
	if( cnt == idx+1 && cnt > m_swkCnt && colm == SWC_CRED ) {
		SWKCOD_DATA srec;

		srec = *pNowRec;
		TkyoInpSet( idx+1 );
		TorokDisp();
	}

	return TRUE;
}


// 仕訳データの変更 
//	int idx				変更するデータの 配列 ｲﾝﾃﾞｯｸｽ	
//  SWKCOD_DATA& sd		変更後のデータ
//
//	return TRUE ... 仕訳データを返送
//
BOOL CTkswkDlg::ChangeSwtkData( int idx, SWKCOD_DATA& sd )
{
	SWKCOD_DATA moto;
	int oldcnt = m_Swcd.GetSize();
	if( idx >= oldcnt )
		return FALSE;

	moto = m_Swcd[idx];

	moto.sc_dbt.TrimRight();
	sd.sc_dbt.TrimRight();
	moto.sc_cre.TrimRight();
	sd.sc_cre.TrimRight();

	CWaitCursor wt;

	CString dbt, cre;
	int seq;
	LINP_DATA ldata[6];
	DBKNREC* pKn;

	// 変更あり
	if( moto.sc_dbt != sd.sc_dbt || moto.sc_cre != sd.sc_cre ) {
		// まだ、データベースには登録されていない
		if( sd.sc_sgn == 0 )
			return TRUE;

		// すでに登録されている仕訳には変更させない
		if( DoubleReg_SwkChk( sd, idx ) ) {
			// 仕訳リストを元に戻す

			m_listswk.GetData( idx, (LPUNKNOWN)ldata );

			pKn = DB_PjisToKnrec( moto.sc_dbt, m_pZm );
			if( pKn ) {
				ldata[SWC_DEBT].dat_initflg = FALSE;
				ldata[SWC_DEBT].dat_string.Format( "%.14s", pKn->knnam );
			}
			else {
				ldata[SWC_DEBT].dat_initflg = TRUE;
			}

			pKn = DB_PjisToKnrec( moto.sc_cre, m_pZm );
			if( pKn ) {
				ldata[SWC_CRED].dat_initflg = FALSE;
				ldata[SWC_CRED].dat_string.Format( "%.14s", pKn->knnam );
			}
			else {
				ldata[SWC_CRED].dat_initflg = TRUE;
			}

			m_listswk.SetData( idx, (LPUNKNOWN)ldata, TRUE );

			return FALSE;
		}

		CString filter;
		// チェック元の摘要コード
		filter_swkdata( &filter, &moto );

		while(1) {
			if( m_pZm->strec->Requery( filter ) == 0 ) {
				m_pZm->strec->MoveFirst();
				dbt = m_pZm->strec->stdbt;
				cre = m_pZm->strec->stcre;
				seq = m_pZm->strec->stseq;
				m_pZm->strec->Delete( (char*)(LPCTSTR)dbt, (char*)(LPCTSTR)cre, seq );
				m_bDBupdate = TRUE;
			}
			else {
				break;
			}
		}

		// 新しい仕訳で追加
		int cnt = m_listtky.GetCount();
		dbt = sd.sc_dbt;
		cre = sd.sc_cre;

		LINP_DATA ldata[6];
		char buf[64];

		int i, seq, code;
		char sgn[10] = {0};

		for( i = seq = 0; i < cnt; i++ ) {
			m_listtky.GetData( i, (LPUNKNOWN)ldata );
			l_print( buf, ldata[0].dat_numeric, NUM_FMT );

			code = atoi( buf );
			if( code > 0 ) {
				m_pZm->strec->Append( (char*)(LPCTSTR)dbt, (char*)(LPCTSTR)cre, ++seq, code, 0, sgn );
				m_bDBupdate = TRUE;
			}
		}
		sd.sc_sgn = 1;	// DB に登録済み

		// 変更をセット
		m_Swcd[idx] = sd;
		return TRUE;
	}

	return FALSE;
}


// 仕訳データ行削除
void CTkswkDlg::DeleteSwtkData( int idx )
{
	int cnt, i, oldcnt;
	SWKCOD_DATA sc, scdata;
	LINP_DATA ldata[6];

	oldcnt = m_Swcd.GetSize();
	if( idx >= oldcnt )
		return;
	
	sc = m_Swcd[idx];
	if( (idx + 1) < oldcnt )
		m_Swcd.RemoveAt( idx );
	else {
		// 最終データの場合は空データをセット
		init_swkdata( &scdata );
		m_Swcd[idx] = scdata;
	}

	CString filter;
	// チェック元の摘要コード
	filter_swkdata( &filter, &sc );

	// DB データ削除
	if( m_pZm->strec->Requery( filter ) == 0 ) {
		int cnt = 0;
		int	seq = 0;
		cnt = m_pZm->strec->GetRecordCount();

		for( int i = 0; i < cnt; i++ ) {
			if( m_pZm->strec->st != -1 )
				seq = m_pZm->strec->stseq;

			m_pZm->strec->Delete( (char*)(LPCTSTR)sc.sc_dbt, (char*)(LPCTSTR)sc.sc_cre, seq );
			m_swkCnt--;
			m_pZm->strec->Requery( filter );
		}

		m_bDBupdate = TRUE;
	}
	
	m_listswk.RemoveData( idx );
	cnt = m_listswk.GetCount();

	if( idx+1 == oldcnt ) {
		// MAX で最終行のとき
		ldata[SWC_NO].dat_initflg = FALSE;
		ldata[SWC_NO].dat_string.Format( "%d", idx+1 );
		ldata[SWC_DEBT].dat_initflg = TRUE;
		ldata[SWC_CRED].dat_initflg = TRUE;

		m_listswk.SetData( idx, (LPUNKNOWN)ldata, TRUE );
	}
	else {
		// 番号更新
		for( i = idx; i < cnt; i++ ) {
			m_listswk.GetData( i, (LPUNKNOWN)ldata );
			ldata[SWC_NO].dat_initflg = FALSE;
			ldata[SWC_NO].dat_string.Format( "%d", i+1 );
			m_listswk.SetColumnData( i, SWC_NO, (LPUNKNOWN)ldata, TRUE );
		}

		// 最終に新規入力行がなければセット
		if( IsEnableAddNewSwkLine() ) {
			SetNewSwkLine();
		}
	}

ASSERT( m_Didx == idx );
	// 現ライン表示
	SetNowLineBkColor( m_Didx, TRUE );
	// 現摘要表示
	m_CurData = m_Swcd[m_Didx];
	TkyoInpSet( m_Didx );
	TorokDisp();
}



// 摘要部分の削除
//		return -1 ... 該当仕訳での摘要がなくなった。
//
int CTkswkDlg::DeleteTkyo( int index )
{
	SWKCOD_DATA sc;

	char	sgn[10] = {0};

	sc = m_Swcd[m_Didx];
	int seq = index + 1;

	CString filter;

	filter_swkdata( &filter, &sc );

//	tmp.Format( " AND stseq = %d", seq );
//	filter += tmp;

	m_pZm->strec->Requery( filter );

	// 11.06 /07	消せない摘要の対策
	m_pZm->strec->SetAbsolutePosition( seq );
	if( m_pZm->strec->st != -1 )
		seq = m_pZm->strec->stseq;

	m_pZm->strec->Delete( (char*)(LPCTSTR)sc.sc_dbt, (char*)(LPCTSTR)sc.sc_cre, seq );
	m_bDBupdate = TRUE;
	
	int ret;
	m_pZm->strec->Requery(filter);
	ret = (m_pZm->strec->st == -1) ? -1 : 0;

	m_listtky.RemoveData( index );
	m_swkCnt--;

	TorokDisp();
ASSERT( m_swkCnt >= 0 );

	return ret;
}


// 二重登録チェック
//	int code ... 摘要ｺｰﾄﾞ
//
//	既に登録されていた場合は TRUE を返送
//
BOOL CTkswkDlg::DoubleRegChk( int code, SWKCOD_DATA* pRec/*=NULL*/)
{
	BOOL	bDreg = FALSE;

	SWKCOD_DATA *pNowRec;

//	if( pRec == NULL )	pNowRec = &m_Swcd[ m_Didx ];
	if( pRec == NULL )	pNowRec = &m_CurData;
	else				pNowRec = pRec;

	CString filter, tmp;
	// チェック元の摘要コード
	filter_swkdata( &filter, pNowRec );

	tmp.Format( " AND stcod = %d", code );
	filter += tmp;

	if( m_pZm->strec->Requery( filter ) == 0 ) {
		bDreg = TRUE;
	}

	return bDreg;
}


// 仕訳の二重登録チェック
//	SWKCOD_DATA sd ... チェック仕訳データ
//	int curidx ....... チェック仕訳データ のｲﾝﾃﾞｯｸｽ
//
//	既に登録されていた場合は TRUE を返送
//
BOOL CTkswkDlg::DoubleReg_SwkChk( SWKCOD_DATA sd, int curidx/*=-1*/ )
{
	BOOL	bDreg = FALSE;

	int max;
	max = m_Swcd.GetSize();
	CString dbt, cre;

	for( int i = 0; i < max; i++ ) {
		// チェック仕訳データは除く
		if( i == curidx )	continue;

		dbt = m_Swcd[i].sc_dbt;
		cre = m_Swcd[i].sc_cre;

		dbt.TrimRight();
		cre.TrimRight();

		sd.sc_dbt.TrimRight();
		sd.sc_cre.TrimRight();
 
		if( dbt == sd.sc_dbt && cre == sd.sc_cre ) {
			bDreg = TRUE;
			break;
		}
	}

	return bDreg;
}



// 二重登録チェック 2 ----- 09.17 /03
//	SWKCOD_DATA *pChkRec ...... チェック摘要ｺｰﾄﾞが入ったレコード
//	SWKCOD_DATA *pRec ......... チェック科目ｺｰﾄﾞが入ったレコード
//								NULL のときは、現在のレコード
//
//	既に登録されていた場合は TRUE を返送
//
BOOL CTkswkDlg::DoubleRegChkEX( SWKCOD_DATA *pChkRec, SWKCOD_DATA *pRec /*=NULL*/ ) 
{
	BOOL	bDreg;
	
	bDreg = FALSE;
	
	SWKCOD_DATA *pNowRec;

	if( pRec == NULL ) {
		pNowRec = &m_Swcd[ m_Didx ];
	}
	else {
		pNowRec = pRec;
	}

	CDWordArray tkarray, tkchk;
	CString filter, tmp;
	// チェック元の摘要コード
	filter_swkdata( &filter, pNowRec );

	int cnt, chkcnt, i, n;

	m_pZm->strec->Requery( filter );

	if( (cnt = m_pZm->strec->GetRecordCount() > 0 ) ) {
		tkarray.SetSize( cnt );

		m_pZm->strec->MoveFirst();	
		i = 0;
		do {
			tkarray[i] = m_pZm->strec->stcod;
			i++;
		} while( m_pZm->strec->MoveNext() == 0 );
	}

	// チェック用摘要コード取得
	filter_swkdata( &filter, pChkRec );
	m_pZm->strec->Requery( filter );

	if( (chkcnt = m_pZm->strec->GetRecordCount() > 0 ) ) {
		tkchk.SetSize( chkcnt );

		m_pZm->strec->MoveFirst();
		i = 0;
		do {
			tkchk[i] = m_pZm->strec->stcod;
			i++;
		} while( m_pZm->strec->MoveNext() == 0 );
	}

	for( i = 0; (i < cnt && !bDreg); i++ ) {

		for( n = 0; n < chkcnt; n++ ) {
			if( tkchk[n] == tkarray[i] ) {
				bDreg = TRUE;
				break;
			}
		}
	}

	return bDreg;
}


//	表形式入力位置　マーク　表示／非表示
void CTkswkDlg::TkrenListPosMark( int mode )
{

//	if( SetInpPos_Sg ) {
		short index = m_listtky.GetTopIndex();
		index += m_listtky.GetIndex();

		if( index < 0 )	return;

		LINP_DATA	LD[2];
		COLORREF	ON_C, OFF_C;

		if( mode ) {
			m_PosMarkIndex = index;
			ON_C = CL_WHITE;
			OFF_C = CL_DKBLUE;
		}
		else	{
			index = m_PosMarkIndex;
			m_PosMarkIndex = 0;
			ON_C = CL_BLACK;
			OFF_C = CL_TKBACK;
		}

		m_listtky.GetColumnData( index, 0, (LPUNKNOWN)LD );

		if( (LD[0].dat_fc != ON_C) || (LD[0].dat_bc != OFF_C) ) {
			LD[0].dat_fc = ON_C;
			LD[0].dat_bc = OFF_C;
			m_listtky.SetColumnData( index, 0, (LPUNKNOWN)LD, TRUE );
		}
//	}
}


// ユーザーメッセージ１
LRESULT CTkswkDlg::OnUser1_Msg( WPARAM wParam, LPARAM lParam )
{
	switch( wParam ) {
	case WM_COMMAND:
		switch( lParam ) {
		// カナ検索ダイアログ表示
		case ID_TLBAR_F8:
			// 摘要リストマーク ON
			TkrenListPosMark( 1 );

			if( m_listtky.GetEnabled() ) {

				{	// ---- 初期化のためのカッコ
					CPoint pt;
					CRect rc;
					m_listtky.GetWindowRect( &rc );
					pt.x = rc.left;

					m_pParent->GetWindowRect( &rc );
					pt.y = rc.top + 5;

					CTksrchDlg dlg( m_pZm, pt, 0, this );
		//			dlg.SetSize(-1,-1, CBaseDlg::m_Xrate, CBaseDlg::m_Yrate);
					if( dlg.DoModal() == IDOK ) {
						CString tknam;
						tkyngetr( dlg.m_TKcod, m_pZm );
						tknam.Format( "%-40.40s", m_pZm->tkrec->tkname );

						if( DoubleRegChk( dlg.m_TKcod ) ) {
							ICSMessageBox("指定された摘要はすでに登録されています！",0,0,0,this);
						}
						else {
							SetTkyListData( m_Tkyidx, dlg.m_TKcod, tknam );
							int lcnt = m_listtky.GetCount();

							if( m_swkCnt < m_swkMax && (lcnt-1 == m_Tkyidx) ) {
								SetNewTkyoInput( m_Tkyidx+1 );
								m_listtky.SetInputPositionEX( m_Tkyidx+1, 0 );
							}
						}
					}
				}
				c_set( IDC_ICSLINP8_TKY );
			}
			break;

		// 仕訳選択ダイアログ表示
		case ID_TLBAR_F9:
			SWKselDisp();
			break;
		}
		break;

	// フォーカスセット
	case WM_SETFOCUS:
		switch( LOWORD( lParam ) ) {
		case SF_FIRSTPOS:		// 最初のﾌｫｰｶｽ位置にｾｯﾄ
			SetFirstPosition();
			break;
		case SF_ONSIZE:			// OnSize より入力ﾎﾞｯｸｽ再セット
			CWnd* pwnd;
			pwnd = GetFocus();
			int id;
			if( pwnd ) {
				id = pwnd->GetDlgCtrlID();

				if( ::IsWindow( m_listswk.m_hWnd ) && ::IsWindow( m_listtky.m_hWnd ) ) {
					if( id == IDC_ICSLINP8_SWK || m_listswk.IsChild( pwnd ) || pwnd == this ) {
//						m_listswk.RedrawControl();
//						short clm = m_listswk.GetColumn();
//						m_listswk.SetInputPositionEX(m_Didx, clm );
					}
					else if( id == IDC_ICSLINP8_TKY || m_listtky.IsChild( pwnd ) ) {
//						m_listtky.RedrawControl();
//						short tkidx = m_listtky.GetIndex();
//						m_listswk.SetInputPosition(tkidx, 1);
					}
				}
			}
			break;

		case SF_SWK_DEBT:	// 仕訳借方にセットする
			m_listswk.SetInputPositionEX(m_Didx, SWC_DEBT );
			break;

		case SF_SWK_CRED:	// 仕訳貸方にセットする
			m_listswk.SetInputPositionEX(m_Didx, SWC_CRED );
			break;
	
		case SF_DBEDT:
			c_set( IDC_ICSDBEDTCTRL1 );
			break;

		case SF_TKY_INP:
			m_listtky.SetFocus();
		//	c_set( IDC_ICSLINP8_TKY );
			m_listtky.SetInputPositionEX( HIWORD( lParam ), 0 );

		//	m_listtky.SetInputPositionEX( HIWORD( lParam ), SWC_CRED );
			break;
		}

		break;

	case WM_USER_1:	// 摘要コピー
		TekiCopySub( lParam );
		break;

	case 0:	// 仕訳摘要ファイルがすでに登録済み
		ICSMessageBox( "入力された仕訳は他で登録されています。",0,0,0,this );
		break;

	}

	return 1;
}

//---------------------------------------------
//	フォーカスセット用ユーザーメッセージ
//	WPARAM wParam	行インデックス
//	LPARAM lParam	アイテム
//---------------------------------------------
LRESULT CTkswkDlg::OnRetroFocus( WPARAM wParam, LPARAM lParam )
{
	int index;
	index = wParam;

	switch( lParam ) {
	case SF_SWK_DEBT:	// 仕訳借方にセットする
		m_listswk.SetFocus();
		m_listswk.SetInputPositionEX( index, SWC_DEBT );
		break;

	case SF_SWK_CRED:	// 仕訳貸方にセットする
		m_listswk.SetFocus();
		m_listswk.SetInputPositionEX( index, SWC_CRED );
		break;

	case SF_DBEDT:		// 実入力DBEDT にセット
		c_set( IDC_ICSDBEDTCTRL1 );
		break;

	case SF_TKY_INP:	// 摘要部分にフォーカスセット
		c_set( IDC_ICSLINP8_TKY );
		break;	
	}

	return 1;
}


BEGIN_EVENTSINK_MAP(CTkswkDlg, CBaseDlg)
    //{{AFX_EVENTSINK_MAP(CTkswkDlg)
	//}}AFX_EVENTSINK_MAP
	ON_EVENT(CTkswkDlg, IDC_ICSLINP8_SWK, 1, CTkswkDlg::EditONIcslinp8Swk, VTS_I2 VTS_I2)
	ON_EVENT(CTkswkDlg, IDC_ICSLINP8_SWK, 2, CTkswkDlg::TerminationIcslinp8Swk, VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CTkswkDlg, IDC_ICSLINP8_TKY, 1, CTkswkDlg::EditONIcslinp8Tky, VTS_I2 VTS_I2)
	ON_EVENT(CTkswkDlg, IDC_ICSLINP8_TKY, 2, CTkswkDlg::TerminationIcslinp8Tky, VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	ON_EVENT(CTkswkDlg, IDC_ICSLINP8_TKY, 9, CTkswkDlg::FocusedIcslinp8Tky, VTS_NONE)
	ON_EVENT(CTkswkDlg, IDC_ICSLINP8_TKY, 5, CTkswkDlg::EditOFFIcslinp8Tky, VTS_I2)
	ON_EVENT(CTkswkDlg, IDC_ICSDBEDTCTRL1, 5, CTkswkDlg::ImeEndCompositionIcsdbedtctrl1, VTS_I2 VTS_BSTR VTS_BSTR)
	ON_EVENT(CTkswkDlg, IDC_ICSDBEDTCTRL1, 4, CTkswkDlg::KanaTerminationIcsdbedtctrl1, VTS_BSTR)
	ON_EVENT(CTkswkDlg, IDC_ICSDBEDTCTRL1, 3, CTkswkDlg::SetFocusIcsdbedtctrl1, VTS_NONE)
	ON_EVENT(CTkswkDlg, IDC_ICSDBEDTCTRL1, 2, CTkswkDlg::KillFocusIcsdbedtctrl1, VTS_I4)
	ON_EVENT(CTkswkDlg, IDC_ICSDBEDTCTRL1, 1, CTkswkDlg::TerminationIcsdbedtctrl1, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CTkswkDlg, IDC_ICSLINP8_SWK, 4, CTkswkDlg::KeyInIcslinp8Swk, VTS_I2 VTS_I2 VTS_I2)
	ON_EVENT(CTkswkDlg, IDC_ICSLINP8_TKY, 4, CTkswkDlg::KeyInIcslinp8Tky, VTS_I2 VTS_I2 VTS_I2)
	ON_EVENT(CTkswkDlg, IDC_ICSLINP8_SWK, 9, CTkswkDlg::FocusedIcslinp8Swk, VTS_NONE)
	ON_EVENT(CTkswkDlg, IDC_ICSLINP8_SWK, 10, CTkswkDlg::SelChangeIcslinp8Swk, VTS_I2)
END_EVENTSINK_MAP()


// 仕訳リスト FOCUS ON
void CTkswkDlg::FocusedIcslinp8Swk()
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( !m_bInitOK )	return;
	int cnt = m_listswk.GetCount();
	int pos = m_listswk.GetIndex();
	int tidx = m_listswk.GetTopIndex();
//ASSERT( m_Didx < cnt );
//MyTrace( "FocusedIcslinp8Swk cnt = %d, pos = %d, tidx = %d\n" );

	if( m_Didx != (pos+tidx) ) {
		m_Didx = (pos+tidx);
		TkyoInpSet( m_Didx );
		SetNowLineBkColor( m_Didx );
	}

//	m_listswk.SetInputPositionEX( m_Didx, m_bSetCred ? SWC_CRED : SWC_DEBT );	
	m_bSetCred = FALSE;	

}


// 仕訳リスト エディットON
void CTkswkDlg::EditONIcslinp8Swk(short index, short column)
{
	if( ! m_bInitOK )	return;

	// DBEdit を表示する
	CRect rect;
	m_listswk.GetRect( index, column, (LPUNKNOWN)&rect );

	short topidx = m_listswk.GetTopIndex();

	int	real_index = index + topidx;

	// 現在処理 データインデックス
	if( real_index != m_Didx ) {
		// DBの更新（必要なら）
		if( ! m_bNoCurSwkCheck )
		{
			ChangeSwtkData(m_Didx, m_CurData);
			m_Swcd[m_Didx] = m_CurData;	// 現レコード更新分を配列にセット
		}
		// 新現在仕訳読み込み
		m_Didx = real_index;
		m_Tkyidx = 0;

		m_CurData = m_Swcd[m_Didx];

		m_InsIdx = -1;

		TkyoInpSet( m_Didx );
		SetNowLineBkColor( m_Didx );
	}
	m_bNoCurSwkCheck = FALSE;
	m_Dclm = column;

	HomeButtonCondition();

/*
	ScreenToClient( rect );
	m_DBEdt.MoveWindow( rect );

	// DBEdit にフォーカスセット
	PostMessage( WM_USER_1, WM_SETFOCUS, MAKELONG( SF_DBEDT, 0 ) );
*/

//TRACE( "@EditONSwk -- sc_dbt = %s, sc_cre %s, sc_sgn %d, sc_chg %d\n", 
//	  m_CurData.sc_dbt, m_CurData.sc_cre, m_CurData.sc_sgn, m_CurData.sc_chg );
}

// Home ボタン状態
void CTkswkDlg::HomeButtonCondition()
{

	BOOL sgn = m_listtky.GetEnabled();
	GetDlgItem( IDC_BUTTON_COPY )->EnableWindow( sgn );

/*	if( m_CurData.sc_dbt.IsEmpty() && m_CurData.sc_cre.IsEmpty() ) {
		BOOL sgn = m_listtky.GetEnabled();
		GetDlgItem( IDC_BUTTON_COPY )->EnableWindow( sgn );
	}
	else {
		GetDlgItem( IDC_BUTTON_COPY )->EnableWindow( TRUE );
	}
*/
}

// 仕訳リストターミネーション
void CTkswkDlg::TerminationIcslinp8Swk(short nChar, short len, short index, short clm, LPUNKNOWN data)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( !m_bInitOK )	return;
	long ret = 0;

	ret |= LINP_REPLY_OK;

	LINP_DATA* ldata;
	ldata = (LINP_DATA*)data;
	CString string;
	BOOL	bNewCheck = FALSE;
	BOOL	bDelJob = FALSE;
	BOOL	bTkyMove = FALSE;	// 摘要部分に移動
	SWKCOD_DATA	sc;
	LINP_DATA	*LD = (LINP_DATA *)data;
	LINP_DATA linpdata[6];
	int idx, para;
	idx = para = -1;

	switch( nChar ) {
	case	VK_LEFT:
					ret |= LINP_REPLY_BACK;
					break;

	case	VK_DOWN:
					ret |= LINP_REPLY_DOWN;
					break;

	case	VK_UP:
					ret |= LINP_REPLY_UP;
					break;

	case	VK_PRIOR:
//					m_listswk.PageMove( -1 );
					break;

	case	VK_NEXT:
//					m_listswk.PageMove( 1 );
					break;

	case	VK_RETURN:
		if( ldata != NULL && len > 0 ) {
			// 科目のチェック
			struct _KamokuData check = {0};
			BOOL bKana = FALSE;
			CString kmcd;
			DBKNREC* pKn = NULL;
			int		inpc, i;
			DWORD	pjis;

			string = ldata->dat_string;

			if( ! isdigitstr( (char*)(LPCTSTR)string ) ) {
				// カナ検索
				if( swk_kanasearch( index, clm, string, kmcd ) ) {
					pKn = DB_PjisToKnrec( kmcd, m_pZm );
					pjis = pKn->kncod;
					bKana = TRUE;
				}
			}

			SWKCOD_DATA sc;
			init_swkdata( &sc );
			BOOL bDataSet = TRUE;
			LINP_DATA	ld[6];

			if( ! bKana ) {
				if( KamokuStr_ChkLength( string, &check, m_pZm->zvol->edcol ) != 0 )
					ret = LINP_REPLY_ERR;
				else {
					inpc = atoi( check.kd_code );
					// 入力コードを内部コードに
					pjis = m_pZm->InpcToPjis( inpc );
					pKn = DB_PjisToKnrec( pjis, m_pZm );
				}
			}

			if( pKn ) {
				::ZeroMemory( &check, sizeof check );
				hl_rev( &pjis, 4 );
				sprintf_s( check.kd_code, sizeof check.kd_code, "%08x", pjis );
				sprintf_s( check.kd_name, sizeof check.kd_name, "%.14s", pKn->knnam );
				check.kd_eda = -1;

				m_listswk.GetData( index, (LPUNKNOWN)ld );
				ld[clm].dat_initflg = FALSE;
				ld[clm].dat_string = check.kd_name;
				m_listswk.SetColumnData( index, clm, (LPUNKNOWN)ld, TRUE );

				i = m_Swcd.GetSize();

				if( m_CurData.sc_sgn == 0 && m_Dclm == SWC_DEBT ) {
					// 新規追加で借方のときは登録チェックしない
				}
				else {
					sc = m_CurData;
					if( m_Dclm == SWC_DEBT )
						sc.sc_dbt = check.kd_code;
					else
						sc.sc_cre = check.kd_code;

					if( DoubleReg_SwkChk( sc ) ) {
						CString str;
						GetSwkRegMsg( str, sc );
						str += "\r\n　この仕訳には変更できません！";
						m_listswk.ReplyTermination( LINP_REPLY_ERR, data );

						ICSMessageBox( str,0,0,0,this );

						switch( clm ) {
						case SWC_DEBT:
							pKn = DB_PjisToKnrec( m_CurData.sc_dbt, m_pZm );
							break;
						case SWC_CRED:
							pKn = DB_PjisToKnrec( m_CurData.sc_cre, m_pZm );
							break;
						default:
							pKn = NULL;
						}
						m_listswk.GetData( index, (LPUNKNOWN)ld );
						if( pKn ) {
							ld[clm].dat_initflg = FALSE;
							ld[clm].dat_string.Format( "%.14s", pKn->knnam );
						}
						else {
							ld[clm].dat_initflg = TRUE;
						}
						m_listswk.SetColumnData( index, clm, (LPUNKNOWN)ld, TRUE );
						idx = index;
						para = clm;

						bDataSet = FALSE;
						ret = 0;
					}
				}

				if( bDataSet ) {
					// 新規追加
					m_CurData.sc_chg = 1;
					if( m_Didx >= i ) {
						m_CurData.sc_sgn = 0;
						if( m_Dclm == SWC_DEBT )
								m_CurData.sc_dbt = check.kd_code;
						else	m_CurData.sc_cre = check.kd_code;

						m_Swcd.Add( m_CurData );
					}
					else{
						// buffer 更新
						if( m_Dclm == SWC_DEBT )
								m_CurData.sc_dbt = check.kd_code;
						else	m_CurData.sc_cre = check.kd_code;
					}

				//	ret = LINP_REPLY_OK;
					
					if( m_Dclm == SWC_CRED ) {
						bTkyMove = TRUE; 
						ret |= LINP_REPLY_STAY;
					}
					else {
						ret |= LINP_REPLY_NEXT;
					}
				}
			}
			else {
				// 不正科目コード入力
				check.kd_eda = -1;
				int cnt = m_Swcd.GetSize();

				if( m_Didx < cnt ) {
					switch( m_Dclm ) {
					case SWC_DEBT:
						pKn = DB_PjisToKnrec( m_CurData.sc_dbt, m_pZm );
						break;
					case SWC_CRED:
						pKn = DB_PjisToKnrec( m_CurData.sc_cre, m_pZm );
						break;
					default:
						pKn = NULL;
					}
				}
				else {
					pKn = NULL;
				}

				if( pKn ) {
					pjis = pKn->kncod;
					hl_rev( &pjis, 4 );
					sprintf_s( check.kd_code, sizeof check.kd_code, "%08x", pjis );
					sprintf_s( check.kd_name, sizeof check.kd_name, "%.14s", pKn->knnam );
				}
				ret = LINP_REPLY_ERR;
			}
			break;
		}
		// else 以下は したへ
	case	VK_TAB:
	case	VK_RIGHT:
			if( clm == SWC_CRED ) {
				bTkyMove = TRUE; 
				ret |= LINP_REPLY_STAY;
			}
			else {
				ret |= LINP_REPLY_NEXT;
			}
			break;

	case VK_DELETE:
		idx = index;
		// 新規ラインは除く
		if( m_CurData.sc_sgn != 0 ) {
			CString title, s1;
			title = "摘要登録処理";
			s1 = _T( "　該当ラインの仕訳を削除しますか？\r\n" );

			int st = ICSExtBox( title, s1, 
								_T("仕訳削除(&S)"), 
								_T( "科目削除(&K)" ), 
								_T( "入力に戻る" ), MB_DEFBUTTON3 );

			if( st == IDYES ) {
				DeleteSwtkData( idx );
			}
			else if( st == IDINVALID_ICS ) {
				// 入力に戻る
				index = -1;
			}
			else {
				bDelJob = TRUE;
			}

			// ﾌｫｰｶｽを 仕訳欄に
			para  = clm == SWC_DEBT ? SF_SWK_DEBT : SF_SWK_CRED;
		}
		// 科目削除処理
		if( bDelJob ) {

			if( m_Dclm == SWC_DEBT ) {
				m_CurData.sc_dbt.Empty();
			}
			else {
				m_CurData.sc_cre.Empty();
			}

			// 仕訳削除
			if( m_CurData.sc_sgn != 0 && m_CurData.sc_dbt.IsEmpty() && m_CurData.sc_cre.IsEmpty() ) {
				ICSMessageBox( "該当ラインの仕訳を削除します。",0,0,0,this );
				DeleteSwtkData( index );
			}
			else {
				// 仕訳リスト表示更新
				m_listswk.GetData( index, (LPUNKNOWN)linpdata );
				linpdata[m_Dclm].dat_initflg = TRUE;
				m_listswk.SetColumnData( index, m_Dclm, (LPUNKNOWN)linpdata, TRUE );
			}
		}
		break;

	case VK_HOME:	// 摘要コピー
			if( m_listtky.GetEnabled() ) {
				if( m_Swcd.GetSize() > 1) {
					ret |= LINP_REPLY_DELETE;
					PostMessage(WM_USER_1, WM_USER_1, IDC_ICSLINP8_SWK );
				}
				else
					ret |= LINP_REPLY_STAY;
			}
			else	ret |= LINP_REPLY_STAY;
			break;

	case	VK_F8:
	case	VK_F9:
	case	VK_END:
				ret |= LINP_REPLY_STAY;
				break;

	case	0:
				ret |= LINP_REPLY_STAY;
				HomeButtonCondition();
				break;

	default	:	ret = LINP_REPLY_ERR;
				break;

	}

	if( ret )	m_listswk.ReplyTermination( ret, data );


	if( ! bTkyMove ) {
		if( idx != -1 )
			PostMessage( WM_RETROFOCUS, idx, para );
	}
	else {
		// 摘要コード入力欄へ
		sc = m_CurData;
//		sc = m_Swcd[index];

		if( sc.sc_sgn == 0 ) {
			// 仕訳のチェック
			para = (clm == SWC_DEBT) ? SF_SWK_DEBT : SF_SWK_CRED;
			if( sc.sc_dbt.IsEmpty() && sc.sc_cre.IsEmpty() ) {
				PostMessage( WM_USER_1, WM_SETFOCUS, MAKELONG( SF_SWK_DEBT, 0 ) );
				return;
			}
			else if( DoubleReg_SwkChk( sc, index ) ) {
				CString str;
				GetSwkRegMsg( str, sc );
				str += "\r\n　この仕訳を追加することはできません！";
				ICSMessageBox( str,0,0,0,this );

				m_listswk.GetData( m_Didx, (LPUNKNOWN)linpdata );
				linpdata[1].dat_initflg = TRUE;
				linpdata[2].dat_initflg = TRUE;
				m_listswk.SetData( m_Didx, (LPUNKNOWN)linpdata, TRUE );

				m_CurData = m_Swcd[index];

				PostMessage( WM_USER_1, WM_SETFOCUS, MAKELONG( SF_SWK_DEBT, 0 ) );
				return;
			}
			else {
				// 摘要リストに新規追加
				SetNewTkyoInput( 0 );
				m_listtky.SetEnabled(TRUE);
				m_listtky.SetBackColor(CL_WHITE);

				int cnt = m_Swcd.GetSize();
				BOOL bAdd = TRUE;
				if( cnt > 0 ) {
					SWKCOD_DATA* lastdata;
					if( index == (cnt-1) )
							lastdata = &m_CurData;
					else	lastdata = &m_Swcd[cnt-1];
					if( lastdata->sc_chg == 0 && lastdata->sc_dbt.IsEmpty() && lastdata->sc_cre.IsEmpty() ) {
						bAdd = FALSE;
					}
				}

				if( bAdd ) {
					init_swkdata( &sc );
					m_Swcd.Add( sc );

					// 仕訳リストに新規ライン用のデータを入れる
					m_listswk.AddNewLine(TRUE);
					SetNewSwkLine();
				}
			}
		}
		else {
			if( DoubleReg_SwkChk( sc, index ) ) {
				CString str;
				GetSwkRegMsg( str, sc );
				str += "\r\n　この仕訳を追加することはできません！";
				ICSMessageBox( str,0,0,0,this );

				PostMessage( WM_USER_1, WM_SETFOCUS, MAKELONG( SF_SWK_DEBT, 0 ) );
				return;
			}
			// 仕訳科目に変更があれば
			ChangeSwtkData( index, m_CurData );
		}

		PostMessage( WM_USER_1, WM_SETFOCUS, MAKELONG( SF_TKY_INP, 0 ) );

	}

//TRACE( "tkswk -- TerminationIcslinp8Swk nChar %d\n", nChar );
}

//
//	仕訳分 科目カナサーチ
//
BOOL CTkswkDlg::swk_kanasearch( int index, int clm, LPCTSTR kana, CString& newcode )
{
	KSEL_PAR kpar;
	CRect rc;
	GetDlgItem(IDC_ICSLINP8_SWK)->GetWindowRect( rc );

	m_pZm->knrec->Requery("", 0);

	BOOL ret = FALSE;;

	kpar.ksPsgn	= KSP_LEFTTOP;
	kpar.ksFsgn = 0;
	kpar.ksLnum = 10;

	::ZeroMemory( kpar.ksKana, sizeof kpar.ksKana );
	strncpy_s( kpar.ksKana, sizeof kpar.ksKana, kana, sizeof kpar.ksKana-1 );

	kpar.ksKrng = 0;
	kpar.ksPos.x	= rc.right;
	kpar.ksPos.y	= rc.top;
	kpar.ksCwnd = this;
	kpar.ksStat = 0;
	kpar.ksFhwd = m_Copy.m_hWnd;

	if( Kmk50onSelect(m_pZm, &kpar, this) ) {
		ret = TRUE;
		newcode = m_pZm->knrec->kncod;

	}

	return ret;
}


void CTkswkDlg::KeyInIcslinp8Swk(short nChar, short previndex, short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( !m_bInitOK )	return;
	
	if( nChar == VK_RETURN ) {
		short tidx = m_listswk.GetTopIndex();
		m_Didx = (index + tidx);
		m_CurData = m_Swcd[m_Didx];
		TkyoInpSet( m_Didx );
		SetNowLineBkColor( m_Didx );

		m_listswk.SetInputPositionEX( m_Didx, m_bSetCred ? SWC_CRED : SWC_DEBT );	
	}	
}


void CTkswkDlg::SelChangeIcslinp8Swk(short index)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
//	short tidx = m_listswk.GetTopIndex();
//	m_Didx = (index + tidx);
	m_Didx = index;
	m_CurData = m_Swcd[m_Didx];
	TkyoInpSet( m_Didx );
	SetNowLineBkColor( m_Didx );
}


/*
BOOL CTkswkDlg::TermDataCheck( LPCTSTR string, struct _KamokuData* ans  )
{

	struct _KamokuData check = {0};

	if( KamokuStr_Chk( string, &check ) != 0 )
		return FALSE;

	int inpc = atoi( check.kd_code );
	int i;
	// 入力コードを内部コードに
	DWORD pjis = m_pZm->InpcToPjis( inpc );
	DBKNREC* pKn;
	pKn = DB_PjisToKnrec( pjis, m_pZm );

	SWKCOD_DATA sc;
	init_swkdata( &sc );
	BOOL bDataSet = TRUE;

	LINP_DATA	ldata[6];

	if( pKn ) {
		::ZeroMemory( &check, sizeof check );
		hl_rev( &pjis, 4 );
		sprintf_s( check.kd_code, sizeof check.kd_code, "%08x", pjis );
		sprintf_s( check.kd_name, sizeof check.kd_name, "%.14s", pKn->knnam );
		check.kd_eda = -1;
		DBkamokudata_set( this, IDC_ICSDBEDTCTRL1, &check, m_pZm );

ASSERT( m_Dclm != -1 );
		m_listswk.GetData( m_Didx, (LPUNKNOWN)ldata );
		ldata[m_Dclm].dat_initflg = FALSE;
		ldata[m_Dclm].dat_string = check.kd_name;
		m_listswk.SetColumnData( m_Didx, m_Dclm, (LPUNKNOWN)ldata, TRUE );

		i = m_Swcd.GetSize();

		if( m_CurData.sc_sgn == 0 && m_Dclm == SWC_DEBT ) {
			// 新規追加で借方のときは登録チェックしない
		}
		else {
			sc = m_CurData;
			if( m_Dclm == SWC_DEBT )
				sc.sc_dbt = check.kd_code;
			else
				sc.sc_cre = check.kd_code;

			if( DoubleReg_SwkChk( sc ) ) {
				CString str;
				GetSwkRegMsg( str, sc );
				str += "\r\n　この仕訳には変更できません！";

				ICSMessageBox( str,0,0,0,this );
				bDataSet = FALSE;
			}
		}

		if( bDataSet ) {
			// 新規追加
			m_CurData.sc_chg = 1;
			if( m_Didx >= i ) {
				m_CurData.sc_sgn = 0;
				if( m_Dclm == SWC_DEBT )
						m_CurData.sc_dbt = check.kd_code;
				else	m_CurData.sc_cre = check.kd_code;

				m_Swcd.Add( m_CurData );
			}
			else{
				// buffer 更新
				if( m_Dclm == SWC_DEBT )
						m_CurData.sc_dbt = check.kd_code;
				else	m_CurData.sc_cre = check.kd_code;
			}
		}
	}
	else {
		check.kd_eda = -1;
		int cnt = m_Swcd.GetSize();

		if( m_Didx < cnt ) {
			switch( m_Dclm ) {
			case SWC_DEBT:
				pKn = DB_PjisToKnrec( m_CurData.sc_dbt, m_pZm );
				break;
			case SWC_CRED:
				pKn = DB_PjisToKnrec( m_CurData.sc_cre, m_pZm );
				break;
			default:
				pKn = NULL;
			}
		}
		else {
			pKn = NULL;
		}

		if( pKn ) {
			pjis = pKn->kncod;
			hl_rev( &pjis, 4 );
			sprintf_s( check.kd_code, sizeof check.kd_code, "%08x", pjis );
			sprintf_s( check.kd_name, sizeof check.kd_name, "%.14s", pKn->knnam );
		}
	}
}

*/


// DBEDIT 科目実入力部
void CTkswkDlg::SetFocusIcsdbedtctrl1()
{
	if( ! m_bInitOK )	return;

	// 科目データをセット
	struct _KamokuData check = {0};

	int max;
	max = m_Swcd.GetSize();

	if( m_Didx < 0 || m_Didx >= max )
		return;

	SWKCOD_DATA& scd = m_CurData;;

	DBKNREC* pKn;
	CString pjis;
	pjis = (m_Dclm == SWC_DEBT) ? scd.sc_dbt : scd.sc_cre;
	pKn = DB_PjisToKnrec( pjis, m_pZm );

	if( pKn ) {
		sprintf_s( check.kd_code, sizeof check.kd_code, "%s", (LPCTSTR)pjis );
		sprintf_s( check.kd_name, sizeof check.kd_name, "%.14s", pKn->knnam );
		check.kd_eda = -1;
		DBkamokudata_set( this, IDC_ICSDBEDTCTRL1, &check, m_pZm );
	}
	else {
		check.kd_eda = -1;
		DBkamokudata_set( this, IDC_ICSDBEDTCTRL1, &check, m_pZm );
	}

//TRACE( "tkswk -- SetFocusIcsdbedtctrl1\n" );
}

void CTkswkDlg::KillFocusIcsdbedtctrl1(long inplen)
{
	if( ! m_bInitOK )	return;

//TRACE( "tkswk -- KillFocusIcsdbedtctrl1\n" );

	m_DBEdt.MoveWindow( 0,0,0,0 );
}

void CTkswkDlg::ImeEndCompositionIcsdbedtctrl1(short nChar, LPCTSTR string, LPCTSTR ystring)
{
	if( ! m_bInitOK )	return;

	struct _KamokuData check = {0};

	KamokuStr_ChkLength( string, &check, m_pZm->zvol->edcol );

	int inpc = atoi( check.kd_code );
	int i;
	// 入力コードを内部コードに
	DWORD pjis = m_pZm->InpcToPjis( inpc );
	DBKNREC* pKn;
	pKn = DB_PjisToKnrec( pjis, m_pZm );

	SWKCOD_DATA sc;
	init_swkdata( &sc );
	BOOL bDataSet = TRUE;

	LINP_DATA	ldata[6];

	if( pKn ) {
		::ZeroMemory( &check, sizeof check );
		hl_rev( &pjis, 4 );
		sprintf_s( check.kd_code, sizeof check.kd_code, "%08x", pjis );
		sprintf_s( check.kd_name, sizeof check.kd_name, "%.14s", pKn->knnam );
		check.kd_eda = -1;
		DBkamokudata_set( this, IDC_ICSDBEDTCTRL1, &check, m_pZm );

ASSERT( m_Dclm != -1 );
		m_listswk.GetData( m_Didx, (LPUNKNOWN)ldata );
		ldata[m_Dclm].dat_initflg = FALSE;
		ldata[m_Dclm].dat_string = check.kd_name;
		m_listswk.SetColumnData( m_Didx, m_Dclm, (LPUNKNOWN)ldata, TRUE );

		i = m_Swcd.GetSize();

		if( m_CurData.sc_sgn == 0 && m_Dclm == SWC_DEBT ) {
			// 新規追加で借方のときは登録チェックしない
		}
		else {
			sc = m_CurData;
			if( m_Dclm == SWC_DEBT )
				sc.sc_dbt = check.kd_code;
			else
				sc.sc_cre = check.kd_code;

			if( DoubleReg_SwkChk( sc ) ) {
				CString str;
				GetSwkRegMsg( str, sc );
				str += "\r\n　この仕訳には変更できません！";

				ICSMessageBox( str,0,0,0,this );
				bDataSet = FALSE;
			}
		}

		if( bDataSet ) {
			// 新規追加
			m_CurData.sc_chg = 1;
			if( m_Didx >= i ) {
				m_CurData.sc_sgn = 0;
				if( m_Dclm == SWC_DEBT )
						m_CurData.sc_dbt = check.kd_code;
				else	m_CurData.sc_cre = check.kd_code;

				m_Swcd.Add( m_CurData );
			}
			else{
				// buffer 更新
				if( m_Dclm == SWC_DEBT )
						m_CurData.sc_dbt = check.kd_code;
				else	m_CurData.sc_cre = check.kd_code;
			}
		}
	}
	else {
		check.kd_eda = -1;
		int cnt = m_Swcd.GetSize();

		if( m_Didx < cnt ) {
			switch( m_Dclm ) {
			case SWC_DEBT:
				pKn = DB_PjisToKnrec( m_CurData.sc_dbt, m_pZm );
				break;
			case SWC_CRED:
				pKn = DB_PjisToKnrec( m_CurData.sc_cre, m_pZm );
				break;
			default:
				pKn = NULL;
			}
		}
		else {
			pKn = NULL;
		}

		if( pKn ) {
			pjis = pKn->kncod;
			hl_rev( &pjis, 4 );
			sprintf_s( check.kd_code, sizeof check.kd_code, "%08x", pjis );
			sprintf_s( check.kd_name, sizeof check.kd_name, "%.14s", pKn->knnam );
		}

		DBkamokudata_set( this, IDC_ICSDBEDTCTRL1, &check, m_pZm );
	}
}

//	DBEdit カナターミネーション処理
//
void CTkswkDlg::KanaTerminationIcsdbedtctrl1(LPCTSTR Kana)
{
	if( ! m_bInitOK )	return;

}


//	DBEdit キー処理
//
void CTkswkDlg::TerminationIcsdbedtctrl1(long nChar, long inplen, long kst)
{
	if( ! m_bInitOK )	return;

	BOOL	shift	=	FALSE;
	BOOL	bTkyMove = FALSE;
	SWKCOD_DATA	sc;

	if( kst & ICSDBEDT_KST_SHIFT ) {
		shift	=	TRUE;
	}

	LINP_DATA	ldata[6];
	int index, para, cnt;
	index = para = -1;

	if( nChar == VK_DOWN ) {
		index	=	m_Didx + 1;
		para = (m_Dclm == SWC_DEBT) ? SF_SWK_DEBT : SF_SWK_CRED;
	}
	else if( nChar == VK_UP ) {
		index	=	m_Didx - 1;
		para = (m_Dclm == SWC_DEBT) ? SF_SWK_DEBT : SF_SWK_CRED;
	}
	else if( nChar == VK_LEFT || nChar == VK_TAB && shift ) {
		if( m_Dclm == SWC_DEBT ) {
			if( m_Didx > 0 ) {
				index = (m_Didx - 1);
				para = SF_SWK_CRED;
			}
		}
		else {
			index = m_Didx;
			para = SF_SWK_DEBT;
		}
	}
	else if( nChar == VK_RIGHT || nChar == VK_RETURN || (nChar == VK_TAB && !shift) ) {

		if( m_Dclm == SWC_DEBT ) {
			index = m_Didx;
			para = SF_SWK_CRED;
		}
		else {
			index = m_Didx;
			para = SF_TKY_INP;
			bTkyMove = TRUE;
		}
	}
	else if( nChar == VK_DELETE ) {
		BOOL bDelJob = FALSE;
		index = m_Didx;
		// 新規ラインは除く
		if( IsValidSwtkyoIdx( index ) ) {
			CString title, s1;
			title = "摘要登録処理";
			s1 = _T( "　該当ラインの仕訳を削除しますか？\r\n" );

			int st = ICSExtBox( title, s1, 
								_T("仕訳削除(&S)"), 
								_T( "科目削除(&K)" ), 
								_T( "入力に戻る" ), MB_DEFBUTTON3 );

			if( st == IDYES ) {
				DeleteSwtkData( index );
			}
			else if( st == IDINVALID_ICS ) {
				// 入力に戻る
				index = -1;
			}
			else {
				bDelJob = TRUE;
			}

			// ﾌｫｰｶｽを 仕訳欄に
			index = m_Didx;
			para  = m_Dclm == SWC_DEBT ? SF_SWK_DEBT : SF_SWK_CRED;
		}
		// 科目削除処理
		if( bDelJob ) {

			if( m_Dclm == SWC_DEBT ) {
				m_CurData.sc_dbt.Empty();
			}
			else {
				m_CurData.sc_cre.Empty();
			}

			// 仕訳削除
			if( m_CurData.sc_sgn != 0 && m_CurData.sc_dbt.IsEmpty() && m_CurData.sc_cre.IsEmpty() ) {
				ICSMessageBox( "該当ラインの仕訳を削除します。",0,0,0,this );
				DeleteSwtkData( index );
			}
			else {
				// 仕訳リスト表示更新
				m_listswk.GetData( index, (LPUNKNOWN)ldata );
				ldata[m_Dclm].dat_initflg = TRUE;
				m_listswk.SetColumnData( index, m_Dclm, (LPUNKNOWN)ldata, TRUE );
			}
		}
	}

	int max;
	max = m_Swcd.GetSize();

	if( index >= max || para < 0 ) {
		index	=	m_Didx;
		para	=	m_Dclm == SWC_DEBT ? SF_SWK_DEBT : SF_SWK_CRED;
	}

	if( ! bTkyMove ) {
		PostMessage( WM_RETROFOCUS, index, para );
//		m_listswk.SetInputPositionEX( newidx, new );
	}
	else {
		// 摘要コード入力欄へ
		sc = m_CurData;
//		sc = m_Swcd[index];

		if( sc.sc_sgn == 0 ) {
			// 仕訳のチェック
			if( sc.sc_dbt.IsEmpty() && sc.sc_cre.IsEmpty() ) {
				m_listswk.SetInputPositionEX( m_Didx, m_Dclm );
				return;
			}
			else if( DoubleReg_SwkChk( sc, index ) ) {
				CString str;
				GetSwkRegMsg( str, sc );
				str += "\r\n　この仕訳を追加することはできません！";
				ICSMessageBox( str,0,0,0,this );

				m_CurData = m_Swcd[index];
				m_listswk.SetInputPositionEX( m_Didx, m_Dclm );
				return;
			}
			else {
				// 摘要リストに新規追加
				SetNewTkyoInput( 0 );
				m_listtky.SetEnabled(TRUE);
				m_listtky.SetBackColor(CL_WHITE);

				cnt = m_Swcd.GetSize();
				BOOL bAdd = TRUE;
				if( cnt > 0 ) {
					SWKCOD_DATA* lastdata;
					if( index == (cnt-1) )
							lastdata = &m_CurData;
					else	lastdata = &m_Swcd[cnt-1];
					if( lastdata->sc_chg == 0 && lastdata->sc_dbt.IsEmpty() && lastdata->sc_cre.IsEmpty() ) {
						bAdd = FALSE;
					}
				}

				if( bAdd ) {
					init_swkdata( &sc );
					m_Swcd.Add( sc );

					// 仕訳リストに新規ライン用のデータを入れる
					m_listswk.AddNewLine(TRUE);
					SetNewSwkLine();
				}
			}
		}
		else {
			if( DoubleReg_SwkChk( sc, index ) ) {
				CString str;
				GetSwkRegMsg( str, sc );
				str += "\r\n　この仕訳を追加することはできません！";
				ICSMessageBox( str,0,0,0,this );

				m_listswk.SetInputPositionEX( m_Didx, m_Dclm );
				return;
			}
			// 仕訳科目に変更があれば
			ChangeSwtkData( index, m_CurData );
		}

		PostMessage( WM_USER_1, WM_SETFOCUS, MAKELONG( SF_TKY_INP, 0 ) );

	}

}


// 摘要リスト フォーカスON
void CTkswkDlg::FocusedIcslinp8Tky()
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( !m_bInitOK )	return;

	short idx;
	if( m_bTkTopDisp ) {		// 必ず 0 番目 を表示
		idx = 0;
		m_bTkTopDisp = FALSE;
	}
	else {
		idx = m_listtky.GetIndex();
		idx += m_listtky.GetTopIndex();
		m_listtky.RedrawControl();
	}

	if( idx >= 0 ) {
		m_listtky.SetInputPositionEX( idx,0 );
	}		
}


// 摘要リスト エディットON
void CTkswkDlg::EditONIcslinp8Tky(short index, short column)
{
	TkrenListPosMark( 0 );

	if( m_Didx >= 0 ) {
		ChangeSwtkData( m_Didx, m_CurData );

		int tidx = m_listtky.GetTopIndex();
		int	real_index = index + tidx;

		m_Tkyidx = real_index;

		if( m_InsIdx >= 0 && real_index != m_InsIdx ) {
//			m_listtky.DeleteInput();
			m_listtky.RemoveData( m_InsIdx );
			m_InsIdx = -1;
//			m_listtky.SetInputPosition( index, column );
			PostMessage( WM_USER_1, WM_SETFOCUS, MAKELONG( SF_TKY_INP, real_index ) );
			return;
		}	
	}
	else {
		m_Tkyidx = -1;
	}
}

// 摘要リスト エディットOFF
void CTkswkDlg::EditOFFIcslinp8Tky(short nChar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( !m_bInitOK )	return;
	
	if( nChar == VK_LEFT ) {
		m_bSetCred = TRUE;	// 貸方サインON
	//	c_set( IDC_ICSLINP8_SWK );
		PostMessage( WM_RETROFOCUS, m_Didx, SF_SWK_DEBT );
	}
	else if( nChar == VK_TAB ) {
	//	c_set( IDC_ICSLINP8_SWK );
		PostMessage( WM_RETROFOCUS, m_Didx, SF_SWK_DEBT );
	}	
}

// 摘要リスト ターミネーション
void CTkswkDlg::TerminationIcslinp8Tky(short nChar, short len, short index, short clm, LPUNKNOWN data)
{
	if( !m_bInitOK )	return;

	long	ret;
	int		tkcnt, i, cnt;
	char	pbuf[64];
	int		scod = 0;
	CString		TKnam;

	LINP_DATA	ldata[10];
	LINP_DATA	*LD = ( LINP_DATA *) data;
	
	cnt = m_listtky.GetCount();
	tkcnt = get_tknum( m_pZm );

	ret = LINP_REPLY_OK;
	switch( nChar ) {

	case	VK_RIGHT:
				ret |= LINP_REPLY_NEXT;
				break;

	case	VK_LEFT:
				ret = LINP_REPLY_DELETE;
				break;

	case	VK_UP:
				ret |= LINP_REPLY_UP;
				break;

	case	VK_DOWN:
				ret |= LINP_REPLY_DOWN;
				break;

	case	VK_RETURN:

			// 摘要コード入力
			if( len ) {
				l_print( pbuf, LD->dat_numeric, NUM_FMT );

				if( num_chk( pbuf, 6 ) == 0 ) {
					pbuf[6] = 0;
					scod = atoi( pbuf );

					if( scod == 0 ) {
						ret = LINP_REPLY_ERR;
						break;
					}

					TKnam = "";
					if( tkyngetr( scod, m_pZm ) == 0 ) {
						if( DoubleRegChk( scod ) ) {
							ICSMessageBox("指定された摘要はすでに登録されています！",0,0,0,this);
							ret = LINP_REPLY_ERR;
						}
						else {
							TKnam.Format( "%-40.40s", m_pZm->tkrec->tkname );

							i = m_listtky.GetCount();
							if( i == (index+1) && (m_swkCnt < m_swkMax) ) 
								SetNewTkyoInput( index+1 );

							// 表示あり
							LD->dat_initflg = FALSE;
							ret = LINP_REPLY_NEW | LINP_REPLY_NEXT;
						}
						break;
					}
				}
				else {
					ret = LINP_REPLY_ERR;
					break;
				}
			}
			ret |= LINP_REPLY_NEXT;
			break;

	case	VK_DELETE:
			if( LD != NULL ) {
				if( l_test( LD->dat_numeric ) > 0 ) {
					scod = 0;
					TKnam = "";
					wsprintf( pbuf, "%06d", scod );
					l_input( LD->dat_numeric, pbuf );

					LD->dat_initflg = TRUE;  // 0　は、表示しないため
					ret |= LINP_REPLY_STAY;
					PostMessage(WM_LIST_INSDEL, 0, index );
				}
			}
			break;

	case	VK_INSERT:
			m_listtky.GetData( index, (LPUNKNOWN)ldata );

			if( m_InsIdx >= 0 ) {
				if( ldata[0].dat_initflg || ! l_test(  ldata[0].dat_numeric ) ) {
					m_listtky.RemoveData( index );	// 空行を元に戻す
					m_InsIdx = -1;
				}
			}
			else if( (index+1) < cnt ) {
				ldata[0].dat_initflg = TRUE;
				ldata[1].dat_initflg = TRUE;
				m_listtky.InsertData( index, (LPUNKNOWN)ldata, TRUE );
				m_InsIdx = index;
			}

			ret |= LINP_REPLY_STAY;
		//	ret = LINP_REPLY_STAY;
		//	PostMessage(WM_LIST_INSDEL, 1, index );

			break;

#ifdef CLOSE
	case	VK_HOME:
			if( m_listtky.GetTopIndex() != 0 ) {
				m_listtky.SetPosition( 0 );
				m_listtky.SetInputPosition( 0, 1 );
			}
			else {
				m_listtky.SetPosition( m_listtky.GetCount()-1 );
				m_listtky.SetInputPosition( m_listtky.GetCount()-1 , 1 );
			}
			break;
#endif

	case VK_HOME:	// 摘要コピー
			if( m_listtky.GetEnabled() ) {
				if( m_Swcd.GetSize() > 1) {
					ret |= LINP_REPLY_DELETE;
					PostMessage(WM_USER_1, WM_USER_1, IDC_ICSLINP8_TKY );
				}
				else
					ret |= LINP_REPLY_STAY;
			}
			else	ret |= LINP_REPLY_STAY;
			break;

	case	VK_PRIOR:
			m_listtky.PageMove( -1 );
			break;

	case	VK_NEXT:
			m_listtky.PageMove( 1 );
			break;

	case	VK_TAB:
			ret = LINP_REPLY_DELETE;
			break;

	case	VK_F8:
	case	VK_F9:
	case	VK_END:
			ret |= LINP_REPLY_STAY;
			break;

	case	0:
			ret |= LINP_REPLY_STAY;
			break;

	default	:
			ret = LINP_REPLY_ERR;
			break;
	}

	m_listtky.ReplyTermination( ret, data );

	if( ret & LINP_REPLY_NEW ) {
		SetTkyListData( index, scod, TKnam );
	}	

}



void CTkswkDlg::KeyInIcslinp8Tky(short nChar, short previndex, short index)
{
	if( nChar == VK_RETURN ) {
		m_listtky.SetInputPosition( index, 0 );
	}
}


// 仕訳がすでに登録されているときのメッセージ取得
void CTkswkDlg::GetSwkRegMsg( CString& msg, SWKCOD_DATA sc )
{
	char debt[36] = {0}, cred[36] = {0};
	DBKNREC* pKn;

	if( (pKn = DB_PjisToKnrec( sc.sc_dbt, m_pZm ) ) != NULL ) {
		sprintf_s( debt, sizeof debt, "%.14s", pKn->knnam );
	}
	if( (pKn = DB_PjisToKnrec( sc.sc_cre, m_pZm ) ) != NULL ) {
		sprintf_s( cred, sizeof cred, "%.14s", pKn->knnam );
	}

	msg.Format( "　指定された仕訳\r\n\r\n　[借方]　%14.14s　[貸方]　%14.14s　\r\n\r\n\
　はすでに登録されています！", debt, cred );

}


// 摘要ｴﾘｱのｺﾋﾟｰ
void CTkswkDlg::OnButtonCopy() 
{
	CWnd* pwnd;
	pwnd = GetFocus();
	int id, setID = 0;
	if( pwnd ) {
		id = pwnd->GetDlgCtrlID();

		if( id == IDC_ICSLINP8_SWK || m_listswk.IsChild( pwnd ) )
			setID = IDC_ICSLINP8_SWK;
		else if( id == IDC_ICSLINP8_TKY || m_listtky.IsChild( pwnd ) )
			setID = IDC_ICSLINP8_TKY;
	}

	PostMessage(WM_USER_1, WM_USER_1, setID );
}

// 摘要ｴﾘｱのｺﾋﾟｰ 実処理部
// LPARAM lParam	コピー後ﾌｫｰｶｽﾎﾟｼﾞｼｮﾝ
//
void CTkswkDlg::TekiCopySub( LPARAM lParam )
{
	CString title, s1;
	int setID;
	setID = IDC_ICSLINP8_TKY;	// 常に摘要欄

#ifndef OLD_CLOSE //科目設定が 出荷されてから
	int	add_sgn = 0;
	title = "摘要登録処理";
	s1 = _T( "　摘要登録部分を他の仕訳対応摘要から\r\n　コピーしますか？\r\n\r\n" );

#ifdef CLOSE
	setID = lParam;	// focus set postion
	if( !setID ) setID = IDC_ICSLINP8_TKY; // 摘要欄にフォーカスセット
#endif

	int st = ICSExtBox( title, s1, _T("一覧選択(&S)"), _T( "上段コピー(&U)" ), _T( "コピーしない" ), MB_DEFBUTTON3 );	
	int srcIdx = -1;

	if( st == IDYES ) {
		// 一覧選択
		CSwlistSelDlg dlg(this);
		dlg.SetZmClass( m_pZm, &m_Swcd );
		if( dlg.DoModal() == IDOK ) {
			srcIdx = dlg.m_SelIndex;
		}
	}
	else if( st == IDNO ) {
		// 上段コピー
		if( m_Didx > 0 ) {
			srcIdx = m_Didx-1;// 上段
		}
	}
#else
	CTKswkCpyMsgDlg	dlg(this);
	int srcIdx = -1;
	int	add_sgn;

	if( dlg.DoModal() == IDOK ) {
		if( dlg.m_Mode == 0 ) {
			// 一覧選択
			CSwlistSelDlg dlg(this);
			dlg.SetZmClass( m_pZm, &m_Swcd );
			if( dlg.DoModal() == IDOK ) {
				srcIdx = dlg.m_SelIndex;
			}
		}
		else {
			// 上段コピー
			if( m_Didx > 0 ) {
				srcIdx = m_Didx-1;// 上段
			}
		}

		add_sgn = dlg.m_Radio;
	}
#endif

	if( srcIdx != -1 ) {
		// コピー元の摘要が すでにある コピー先仕訳の摘要とかぶっていた場合は
		// コピーさせない
		TkyCopyDB( srcIdx, m_Didx, add_sgn );

		// DataBase 登録サイン ON
		if( m_CurData.sc_sgn == 0 ) {
			m_CurData.sc_sgn = 1;
			m_Swcd[m_Didx] = m_CurData;
		}

		TkyoInpSet( m_Didx );
	}

	if( setID ) {
		c_set( setID );
	}
}

// 摘要コピー処理
//		int add_sgn		0:上書き, 1:追加
//
int CTkswkDlg::TkyCopyDB( int srcIdx, int dtIdx, int add_sgn/* = 0*/ )
{
	CDWordArray da;
	int i;

	CSTREC* pst;
	pst = m_pZm->strec;
	SWKCOD_DATA sc;
	if( srcIdx < m_Swcd.GetSize() ) {
		sc = m_Swcd[srcIdx];
	}
	else	return -1;

	CString filter, tmp;
	// 転送元の摘要コード を配列化
	filter_swkdata( &filter, &sc );

	if( pst->Requery( filter ) == 0 ) {
		if( pst->MoveFirst() == 0 ) {
			do {
				da.Add( pst->stcod );
			} while( pst->MoveNext() == 0 );
		}
	}

#ifdef CLOSE
	if( dtIdx < m_Swcd.GetSize() ) {
		sc = m_Swcd[dtIdx];
	}
	else	return -1;
#endif
	sc	= m_CurData;

	// チェック元の摘要コード
	filter_swkdata( &filter, &sc );

	pst->Requery( filter );
	int cnt = pst->GetRecordCount();
	int newcnt = da.GetSize();

	// 修正
	int seq = 1;
	char sgn[10] = {0};
	if( newcnt > 0 ) {
		if( ! add_sgn ) {
			for( i = 0; i < cnt; i++ ) {
				if( i < newcnt ) {
					pst->Correct( (char*)(LPCTSTR)sc.sc_dbt, (char*)(LPCTSTR)sc.sc_cre, seq, seq, (int)da[i], 0, sgn );
					++seq;
				}
				else {
					// 既存の分は削除
					pst->Requery(filter);
					pst->Delete( (char*)(LPCTSTR)sc.sc_dbt, (char*)(LPCTSTR)sc.sc_cre, seq );
				//	seq++;
					m_swkCnt--;
				}
			}
			for( ; i < newcnt; i++ ) {
				// 新規追加
				if( i >= cnt ) {
					pst->Append( (char*)(LPCTSTR)sc.sc_dbt, (char*)(LPCTSTR)sc.sc_cre, seq, da[i], 0, sgn );
					m_swkCnt++;
				}
				else {
					// 既存の分は削除
					pst->Delete( (char*)(LPCTSTR)sc.sc_dbt, (char*)(LPCTSTR)sc.sc_cre, seq );
					m_swkCnt--;
				}
				++seq;
			}
		}
		else {
			//既存部分に追加
			seq = cnt+1;

			//重複データがあるか？
			pst->MoveFirst();
			if( pst->st != -1 ) {
				do {
					newcnt = da.GetCount();
					for( i = 0; i < newcnt; i++ ) {
						if( pst->stcod == da[i] ) {
							da.RemoveAt(i);
							break;
						}
					}
				} while( pst->MoveNext() == 0 );
			}

			newcnt = da.GetCount();

			for( i = 0; i < newcnt; i++ ) {
				// 新規追加
				pst->Append( (char*)(LPCTSTR)sc.sc_dbt, (char*)(LPCTSTR)sc.sc_cre, seq, da[i], 0, sgn );
				m_swkCnt++;
				++seq;
			}
		}

		m_bDBupdate = TRUE;
	}

	TorokDisp();

	return 0;
}
 

// 仮想関数
// 摘要辞書登録より摘要追加処理
//
void CTkswkDlg::AddTekiyoJob( LPARAM lParam )
{
	TK_ADD* pAdd = (TK_ADD*)lParam;

	SWKCOD_DATA sc, tmp;

	if( ! m_bFirstShow )	m_bFirstShow = TRUE;

	if( m_swkCnt >= m_swkMax )
		return;

#ifdef CLOSE
	for( int i = 0; i < 2; i++ ) {
		sc.sc_dbt = pAdd->ta_swk[i].debcd;
		sc.sc_cre = pAdd->ta_swk[i].crecd;
		
		if( sc.sc_deb.IsEmpty() && sc.sc_cre.IsEmpty() ) 
			break;
#else
		// 借貸 同時登録は 科目属性 に従って登録する
		sc.sc_dbt = pAdd->ta_swk[0].debcd;
		sc.sc_cre = pAdd->ta_swk[0].crecd;
#endif

		int pos, datanum;
		CString filter;
		// チェック元の摘要コード
		filter_swkdata( &filter, &sc );

		m_pZm->strec->Requery( filter );
		pos = m_pZm->strec->GetRecordCount();
		SWKCOD_DATA srec;

		// 新規追加
		if( pos <= 0 ) {
			// 一致する仕訳 配列まで検索
			int		max, n;
			max = m_Swcd.GetSize();
			datanum = max;
			pos = 0;
			for( int i = 0; i < max; i++ ) {
				srec = m_Swcd[i];
				srec.sc_dbt.TrimRight();
				srec.sc_cre.TrimRight();

				if( srec.sc_dbt == sc.sc_dbt && srec.sc_cre == sc.sc_cre ) {
					pos = i+1;
					break;
				}
			}

			if( !pos )	{
				// 仕訳が追加されていない
				srec.sc_dbt = sc.sc_dbt;
				srec.sc_cre = sc.sc_cre;
				srec.sc_sgn = 1;
				srec.sc_chg = 0;

				m_CurData = srec;

				int addidx = -1;
				int max = m_Swcd.GetSize();
				if( max > 0 ) {
					tmp = m_Swcd[max-1];
					if( tmp.sc_dbt.IsEmpty() && tmp.sc_cre.IsEmpty() ) {
						addidx = (max-1);
					}
				}
				if( addidx != -1 ) {
					m_Swcd[addidx] = srec;
					n = addidx;
				}
				else {
					m_Swcd.Add( srec );
					n = m_Swcd.GetSize();
					if( n > 0 )	n--;
				}
			}
			else {
				// 仕訳が追加されていた[テーブルにはレコードはない]
				n = pos-1;
				m_Swcd[m_Didx].sc_sgn = 1;		// 追加サインON
				m_CurData = m_Swcd[m_Didx];
			}

			SiwakeInpSet( FALSE );
			TkyoAddSub( &srec, pAdd->ta_cod );
			TkyoInpSet( n );
			m_Didx = n;

			m_Tkyidx = 0;
			TorokDisp();
			m_bNoCurSwkCheck = TRUE;
			// フォーカスをセット
			PostMessage( WM_USER_1, WM_SETFOCUS, SF_SWK_DEBT );
		}
		else {	// 追加(他に同じ仕訳で登録されている)
			// 一致する仕訳 配列まで検索
			int max = m_Swcd.GetSize();
			datanum = max;
			pos = 0;
			for( int i = 0; i < max; i++ ) {
				srec = m_Swcd[i];
				srec.sc_dbt.TrimRight();
				srec.sc_cre.TrimRight();

				if( srec.sc_dbt == sc.sc_dbt && srec.sc_cre == sc.sc_cre ) {
					pos = i+1;
					break;
				}
			}
			// 修正
			if( !pos )	{
				if( !datanum ) 
					pos = 1; // 最大まで登録されていて追加できない時 TOPに
				else
					pos = datanum;
			}
			
			if( DoubleRegChk( pAdd->ta_cod, &sc ) ) {
				ICSMessageBox("指定された摘要はすでに登録されています！",0,0,0,this);
				m_Didx = pos-1;
				m_CurData = m_Swcd[m_Didx];
				m_Tkyidx = 0;
				TkyoInpSet( m_Didx );
				TorokDisp();
			}
			else {
				SiwakeInpSet( FALSE );
				m_Didx = pos-1;
				m_CurData = m_Swcd[m_Didx];
				m_Tkyidx = 0;
				TkyoAddSub( &srec, pAdd->ta_cod );
				TkyoInpSet( m_Didx );

				TorokDisp();
			}
			m_bNoCurSwkCheck = TRUE;
			// フォーカスをセット
			PostMessage( WM_USER_1, WM_SETFOCUS, SF_SWK_DEBT );
		}
		
#ifdef CLOSE
	}	// for loop
#endif

	SetNowLineBkColor( m_Didx, TRUE );

}

//	仕訳摘要をデータベースに追加する
//
void CTkswkDlg::TkyoAddSub( SWKCOD_DATA* psc, int code )
{
	// データベースにセット
	int  seq;
	CString codbf, Tknam, filter;

	char	sgn[10] = {0};

	// チェック元の摘要コード
	filter_swkdata( &filter, psc );
	m_pZm->strec->Requery( filter );

	m_pZm->strec->MoveFirst();
	seq = 0;

	if( m_pZm->strec->st != -1 ) {
		seq = m_pZm->strec->stseq;
		do {
			if( seq < m_pZm->strec->stseq )
				seq = m_pZm->strec->stseq;
		} while( m_pZm->strec->MoveNext() == 0 );
	}
	// 追加SEQ番号
	if( seq > 0 )
		seq += 1;
	else
		seq = 1;

		// 追加
//TRACE( ">> TkswkDlg.cpp strec Append %s, %s, %d\n", psc->sc_dbt, psc->sc_cre, seq );

	m_pZm->strec->Append( (char*)(LPCTSTR)psc->sc_dbt,  (char*)(LPCTSTR)psc->sc_cre, seq, code, 0, sgn );
	m_swkCnt++;
	m_bDBupdate = TRUE;

	if( code > 0 ) {
		if( tkyngetr( code, m_pZm ) == 0 ) {
			Tknam.Format( "%-40.40s", m_pZm->tkrec->tkname );
		}
		// 摘要コード
		codbf.Format( "%6d", code );
	}
}


// 挿入 削除による行詰 処理
//		wParam .... 処理種別
//		lParam .... List のインデックス
//
LRESULT CTkswkDlg::OnListInsDel( WPARAM wParam, LPARAM lParam )
{
	m_listtky.DeleteInput();
	int tidx = m_listtky.GetTopIndex();
//	int realidx = tidx + lParam;
	int realidx = lParam;

	if( wParam == 0 ) {
		// 削除による行詰
		if( DeleteTkyo( realidx ) == -1 ) {
			DeleteSwtkData( m_Didx );
			PostMessage(WM_USER_1, WM_SETFOCUS, SF_SWK_DEBT );
			return 0;
		}
		m_listtky.RedrawControl();
	}
	else {
		// 挿入処理 --- 空行を挿入
		// その場所から移動して、新しく追加されていなかったら、その空行はなくす
		if( m_swkCnt < m_swkMax && m_InsIdx == -1 ) {
			m_InsIdx = lParam;

			LINP_DATA ld[SWC_MAX];

			for( int i = 0; i < SWC_MAX; i++ ) {
				ld[i].dat_fc = ONCOLOR;
				ld[i].dat_bc = OFFCOLOR;
				ld[i].dat_attr = LINP_DISP_COLOR;
				ld[i].dat_initflg = TRUE;
			}
			m_listtky.InsertData( (short)lParam, (LPUNKNOWN)ld, TRUE );
		}
	}

	m_listtky.SetInputPositionEX( realidx, 0 );

	return 0;
}



///////////////////////////////////////////////////////////////////////////////////////////

#ifdef CLOSE

void CTkswkDlg::OnEditONIcslistinpSwk(short index, short column) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( !m_bInitOK )	return;

//	short topidx = m_listswk.GetTopIndex();
	short topidx;

	// 現在処理 データインデックス
	if( (topidx + index) != m_Didx ) {
		m_Didx = topidx + index;
		TkyoInpSet( m_Didx );

		SetNowLineBkColor( m_Didx );
	}
//TRACE( "edit on index %d, ti %d, di %d\n", index, topidx, m_Didx );	
	
}

void CTkswkDlg::OnEditOFFIcslistinpSwk(short nChar) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( !m_bInitOK )	return;

	if( nChar == VK_RIGHT || nChar == VK_RETURN || nChar == VK_TAB ) {
		if( m_listtky.GetTopIndex() != 0 ) {
			m_listtky.SetPosition( 0 );
		}

		// EditOFF の時のみ、摘要リストの表示を最初からにする。
		m_bTkTopDisp = TRUE;

		c_set( IDC_ICSLINP8_TKY );
	}	
}


void CTkswkDlg::OnFocusedIcslistinpSwk() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( !m_bInitOK )	return;

	int cnt, pos, tidx;
//	int cnt = m_listswk.GetCount();
//	int pos = m_listswk.GetIndex();
//	int tidx = m_listswk.GetTopIndex();
//ASSERT( m_Didx < cnt );

	if( m_Didx != (pos+tidx) ) {
		m_Didx = (pos+tidx);
		TkyoInpSet( m_Didx );
		SetNowLineBkColor( m_Didx );
	}

//	m_listswk.SetInputPositionEX( m_Didx, m_bSetCred ? SWC_CRED : SWC_DEBT );	
	m_bSetCred = FALSE;	
}

void CTkswkDlg::OnKanaTermIcslistinpSwk(short index, short clm, LPCTSTR data) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	
}

void CTkswkDlg::OnTerminationIcslistinpSwk(short nChar, short len, short index, short clm, LPUNKNOWN data) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( !m_bInitOK )	return;

	long	ret;
	BOOL	bNewCheck = FALSE;
	LINP_DATA	*LD = (LINP_DATA *)data;

	ret = LINP_REPLY_OK;

	switch( nChar ) {

		case	VK_LEFT:
						ret |= LINP_REPLY_BACK;
						break;

		case	VK_TAB:
		case	VK_RIGHT:
						if( clm == SWC_CRED ) {
							if( IsValidSwtkyoIdx(index) ) ret |= LINP_REPLY_DELETE;
							else						  ret |= LINP_REPLY_STAY;	
						}
						else {
							ret |= LINP_REPLY_NEXT;
						}
						break;

		case	VK_DOWN:
						ret |= LINP_REPLY_DOWN;
						break;

		case	VK_UP:
						ret |= LINP_REPLY_UP;
						break;

		case	VK_PRIOR:
//						m_listswk.PageMove( -1 );
						break;

		case	VK_NEXT:
//						m_listswk.PageMove( 1 );
						break;

		case	VK_RETURN:

						if( !len ) {
							// 追加行で、貸方の時は CheckSwtkData() をとおす
							if( ! IsValidSwtkyoIdx(index) && clm == SWC_CRED );
							else {
							//	ret |= LINP_REPLY_NEXT;
								if( clm == SWC_CRED )	ret |= LINP_REPLY_DELETE;
								else					ret |= LINP_REPLY_NEXT;
								break;
							}
						}

						if( CheckSwtkData( index, clm, (LPUNKNOWN)LD ) ) {
							// ニューラインチェック
							bNewCheck = TRUE;

							if( LD ) {
								ret = LINP_REPLY_NEW;
								LD->dat_initflg = FALSE;
								// 貸方でデータありの時、再セット
								//(スクロールバーの出るタイミングで名称が消えていたため)
								if( clm == SWC_CRED ) {
									LINP_DATA ldt[10];
//									m_listswk.GetData( index, (LPUNKNOWN)ldt );
									ldt[ clm ].dat_initflg = FALSE;
									memcpy( &ldt[ clm ].dat_kamoku, &LD->dat_kamoku, sizeof(LD->dat_kamoku));
//									m_listswk.SetColumnData( index, clm, (LPUNKNOWN)ldt, TRUE );
								}
							}
							if( clm == SWC_CRED ) {
								if( IsValidSwtkyoIdx(index) ) { // 貸方かつ有効データのみ摘要入力へ
									ret |= LINP_REPLY_STAY;
									PostMessage(WM_USER_1, WM_SETFOCUS, SF_FIRSTPOS );
								}
								else
									ret |= LINP_REPLY_STAY;
							}
							else
								ret |= LINP_REPLY_NEXT;
						}
						else	{
							ret = LINP_REPLY_ERR;
						}
						break;

		case	VK_DELETE:
						// 新規ラインは除く
						if( IsValidSwtkyoIdx( index ) ) {
							CString title, s1;
							title.LoadString(AFX_IDS_APP_TITLE);
							s1 = _T( "　該当ラインの仕訳を削除しますか？\r\n" );

							int st = ICSExtBox( title, s1, 
												_T("仕訳削除(&S)"), 
												_T( "科目削除(&K)" ), 
												_T( "入力に戻る" ), MB_DEFBUTTON3 );

							if( st == IDYES ) {
								DeleteSwtkData( index );
								ret = LINP_REPLY_NEW|LINP_REPLY_OK;
								break;
							}
							else if( st == IDINVALID_ICS ) {
								LD = NULL;
							}

							// ﾌｫｰｶｽを 仕訳欄に
							PostMessage(WM_USER_1, WM_SETFOCUS, clm == SWC_DEBT ? SF_SWK_DEBT : SF_SWK_CRED );
						}

						if( LD ) {
							LD->dat_string.Empty();
							LD->dat_kamoku.kamoku_code = 0xffff;
							if( CheckSwtkData( index, clm, (LPUNKNOWN)LD, TRUE ) ) {
								ret = LINP_REPLY_NEW;
								LD->dat_initflg = FALSE;
							}
							else	{
								ret = LINP_REPLY_ERR;
							}
						}
						else {
							ret |= LINP_REPLY_STAY;
						}
						break;
#ifdef CLOSE
		case VK_HOME:	// 新規仕訳
						ret |= LINP_REPLY_DELETE;
						PostMessage(WM_COMMAND, MAKELONG(IDC_BUTTON_NEW, BN_CLICKED),
												(LPARAM)GetDlgItem(IDC_BUTTON_NEW)->m_hWnd );
						break;
#endif
		case VK_HOME: // 摘要のコピー
						if( IsValidSwtkyoIdx( index ) && m_Swcd.GetSize() > 1) {
							ret |= LINP_REPLY_DELETE;
							PostMessage(WM_USER_1, WM_USER_1, IDC_ICSLINP8_SWK );
						}
						else 
							ret |= LINP_REPLY_STAY;
						break;

		case	0:
						ret |= LINP_REPLY_STAY;
						break;

		default		:	ret = LINP_REPLY_ERR;
						break;
	}

//	m_listswk.ReplyTermination( ret, (LPUNKNOWN)LD );

	if( ret & LINP_REPLY_NEXT || bNewCheck ) SwtkNewlineCheck( index, clm );

	// 借貸 表示科目は 番号部分に色をつける
	BothKamokColorSet( index );

	
}


void CTkswkDlg::OnKeyInIcslistinpSwk(short nChar, short previndex, short index) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( !m_bInitOK )	return;
	
	if( nChar == VK_RETURN ) {
		m_Didx = index;
//		m_listswk.SetInputPositionEX( m_Didx, SWC_DEBT );
		TkyoInpSet( m_Didx );
		SetNowLineBkColor( m_Didx );
	}	
}

void CTkswkDlg::OnFocusedIcslistinpTeki() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( !m_bInitOK )	return;

	short idx;
	if( m_bTkTopDisp ) {		// 必ず 0 番目 を表示
		idx = 0;
		m_bTkTopDisp = FALSE;
	}
	else {
		idx = m_listtky.GetIndex();
		idx += m_listtky.GetTopIndex();
		m_listtky.RedrawControl();
	}

	if( idx >= 0 ) {
		m_listtky.SetInputPositionEX( idx,1 );
	}		
}

void CTkswkDlg::OnEditONIcslistinpTeki(short index, short column) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	TkrenListPosMark( 0 );		
}


void CTkswkDlg::OnEditOFFIcslistinpTeki(short nChar) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( !m_bInitOK )	return;
	
	if( nChar == VK_LEFT ) {
		m_bSetCred = TRUE;	// 貸方サインON
		c_set( IDC_ICSLINP8_SWK );
	}
	else if( nChar == VK_TAB ) {
		c_set( IDC_ICSLINP8_SWK );
	}	
}

void CTkswkDlg::OnTerminationIcslistinpTeki(short nChar, short len, short index, short clm, LPUNKNOWN data) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( !m_bInitOK )	return;

	long	ret;
	int		tkcnt;
	char	pbuf[32];
	unsigned short scod = 0;
	CString		TKnam;

	LINP_DATA	*LD = ( LINP_DATA *) data;
	
	tkcnt = get_tknum( m_pZm );

	ret = LINP_REPLY_OK;
	switch( nChar ) {

		case	VK_RIGHT:
					ret |= LINP_REPLY_NEXT;
					break;

		case	VK_LEFT:
					ret = LINP_REPLY_DELETE;
					break;

		case	VK_UP:
					ret |= LINP_REPLY_UP;
					break;

		case	VK_DOWN:
					ret |= LINP_REPLY_DOWN;
					break;

		case	VK_RETURN:

				// 摘要コード入力
				if( len ) {
					l_print( pbuf, LD->dat_numeric, NUM_FMT );

					if( num_chk( pbuf, 6 ) == 0 ) {
						pbuf[6] = 0;
						scod = atoi( pbuf );

						// 摘要辞書 MAX オーバー
						if( scod > tkcnt ) {
							MessageBeep( 0xffffffff );
							ret = LINP_REPLY_ERR;
							break;
						}
						else if( scod == 0 ) {
							ret = LINP_REPLY_ERR;
							break;
						}

						TKnam = "";
						if( tkyngetr( scod, m_pZm ) == 0 ) {
							if( DoubleRegChk( scod ) ) {
								ICSMessageBox("指定された摘要はすでに登録されています！",0,0,0,this);
								ret = LINP_REPLY_ERR;
							}
							else {
								TKnam.Format( "%-20.20s", m_pZm->tkrec->tkname );

								// 表示あり
								LD->dat_initflg = FALSE;
								ret = LINP_REPLY_NEW | LINP_REPLY_NEXT;
							}
							break;
						}
					}
					else {
						ret = LINP_REPLY_ERR;
						break;
					}
				}
				ret |= LINP_REPLY_NEXT;
				break;

		case	VK_DELETE:

				if( LD != NULL ) {

					scod = 0;
					TKnam = "";
					wsprintf( pbuf, "%06d", scod );
					l_input( LD->dat_numeric, pbuf );

					LD->dat_initflg = TRUE;  // 0　は、表示しないため
					ret = LINP_REPLY_NEW;
				}
				PostMessage(WM_LIST_INSDEL, 0, index );
				break;

		case	VK_INSERT:
				ret = LINP_REPLY_STAY;
				PostMessage(WM_LIST_INSDEL, 1, index );

				break;

#ifdef CLOSE
		case	VK_HOME:
				if( m_listtky.GetTopIndex() != 0 ) {
					m_listtky.SetPosition( 0 );
					m_listtky.SetInputPosition( 0, 1 );
				}
				else {
					m_listtky.SetPosition( m_listtky.GetCount()-1 );
					m_listtky.SetInputPosition( m_listtky.GetCount()-1 , 1 );
				}
				break;
#endif

		case VK_HOME:	// 摘要コピー
				if( m_Swcd.GetSize() > 1) {
					ret |= LINP_REPLY_DELETE;
					PostMessage(WM_USER_1, WM_USER_1, IDC_ICSLINP8_TKY );
				}
				else
					ret |= LINP_REPLY_STAY;

				break;

		case	VK_PRIOR:
				m_listtky.PageMove( -1 );
				break;

		case	VK_NEXT:
				m_listtky.PageMove( 1 );
				break;

		case	VK_TAB:
				ret = LINP_REPLY_DELETE;
				break;

		case	0:
				ret |= LINP_REPLY_STAY;
				break;

		default	:
				ret = LINP_REPLY_ERR;
				break;
	}

	m_listtky.ReplyTermination( ret, data );

	if( ret & LINP_REPLY_NEW ) {
		SetTkyListData( scod, TKnam );
	}	
}

// 
void CTkswkDlg::OnEditSetFocusIcsdblvTky(long line, long clm) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	
	DBLV_GETDATA dbgd;
	VARIANT var;
	var.pbVal = (BYTE*)&dbgd;

//	m_listtky.GetItem( line, clm, NULL, DBLV_DISPLAYINDEX, &var );
//	m_listtky.EditSetDisplayString( dbgd.text );

	int cnt = m_listtky.GetCount();

	if( m_InsIdx >= 0 && m_InsIdx != line ) {
		m_listtky.RemoveData( m_InsIdx );
		m_InsIdx = -1;
		PostMessage( WM_USER_1, WM_SETFOCUS, MAKELONG( SF_TKY_INP, line ) );
		return;
	}

	// 新規入力ラインを含む 場所より 大きいINDEX は 入力BOX を 元のインデックス に戻す。
	if( cnt < (line+1) ) {
		PostMessage( WM_USER_1, WM_SETFOCUS, MAKELONG( SF_TKY_INP, m_Tkyidx ) );
	}
	else	m_Tkyidx = line;

}


void CTkswkDlg::OnEditKillFocusIcsdblvTky(long line, long clm, long inplen) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください

	if( inplen > 0 ) {
	
	
	
	}
}

void CTkswkDlg::OnEditTerminationIcsdblvTky(long line, long clm, long nChar, long inplen, long kst) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	BOOL	shift	=	FALSE;
	BOOL	bTkyMove = FALSE;
	if( kst & ICSDBEDT_KST_SHIFT ) {
		shift	=	TRUE;
	}

	int	newclm, newline;
	int	maxline	=	m_listtky.GetCount();

//	VARIANT	v;
//	m_listtky.EditGetData( &v, ICSDBEDT_TYPE_STRING, 0 );

	newclm = 0;

	if( nChar == VK_DOWN ) {
		newline	=	line + 1;
	}
	else if( nChar == VK_UP ) {
		newline	=	line - 1;
	}
	else if( nChar == VK_RIGHT || nChar == VK_RETURN || (nChar == VK_TAB && shift == FALSE) ) {
		newline	=	line + 1;
	}
	else if( nChar == VK_LEFT || (nChar == VK_TAB && shift) ) {
		// 仕訳部分にフォーカスセット
		PostMessage( WM_USER_1, WM_SETFOCUS, MAKELONG( SF_SWK_DEBT, 0 ) );
		return;
	}
	else if( nChar == VK_DELETE ) {
		DeleteTkyo( line );
		newline = line;
		// 新しい摘要番号を 入力ボックスに表示させるため
		PostMessage( WM_USER_1, WM_SETFOCUS, MAKELONG( SF_TKY_INP, line ) );
		return;
	}
	else if( nChar == VK_INSERT ) {
		if( m_InsIdx == -1 ) {
//			m_listtky.InsertItem(line, 0);
			m_InsIdx = line;
			CString buf;
			buf.Empty();
//			m_listtky.SetItem(line, 0, DBLV_DISPLAYINDEX, buf );
//			m_listtky.SetItem(line, 1, DBLV_DISPLAYINDEX, buf );
		}
		else {
			m_listtky.RemoveData(line);
			m_InsIdx = -1;
		}
		PostMessage( WM_USER_1, WM_SETFOCUS, MAKELONG( SF_TKY_INP, line ) );
		return;
	}

	int scod = 0;
	if( inplen > 0 ) {
		VARIANT var;
//		m_listtky.EditGetData( &var, ICSDBEDT_TYPE_STRING, 0 );

		char pbuf[40];
		if( var.pbVal != NULL ) {
			strcpy_s( pbuf, sizeof pbuf, (char*)var.pbVal );
			if( num_chk( pbuf, strlen(pbuf) ) == 0 ) {
				scod = atoi( pbuf );
			}
		}

		CString TKnam;
		if( scod > 0 ) {
			TKnam = "";
			if( tkyngetr( scod, m_pZm ) == 0 ) {
				if( DoubleRegChk( scod ) ) {
					ICSMessageBox("指定された摘要はすでに登録されています！",0,0,0,this);
				}
				else {
					TKnam.Format( "%-20.20s", m_pZm->tkrec->tkname );
					// 表示あり
					// 新しい番号 ＆ 名称を コントロールにセット
					SetTkyListData( line, scod, TKnam );

					if( line == m_InsIdx ) {
						m_InsIdx = -1;
					}
				}
			}
		}	
	}

	maxline = m_listtky.GetCount();

	if( newline < 0 ) {
		newline	=	line;
	}
	else if( newline >= maxline ) {
		
		if( m_swkCnt < m_swkMax && scod > 0 ) {
			SetNewTkyoInput( line+1 );
		//	m_listtky.InsertItem(line+1, 0);
			newline = line + 1;
		}
		else {
			newline	= line;
		}
	}

	m_listtky.SetInputPositionEX( newline, newclm );
//	m_listtky.SetInput( newline, newclm, DBLV_DISPLAYINDEX );

}


void CTkswkDlg::OnEditSetFocusIcsdblvSwk(long line, long clm) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
TRACE( "OnEditSetFocusIcsdblvSwk %d\n", line );
	int sz = m_Swcd.GetSize();
	if( line >= sz ) {
		PostMessage( WM_USER_1, WM_SETFOCUS, clm == SWC_DEBT ? SF_SWK_DEBT : SF_SWK_CRED  );
		return;
	}

	if( clm == SWC_DEBT ) {
//		m_listswk.EditSetDisplayString( m_Swcd[line].sc_dbtname );
	}
	else if( clm == SWC_CRED ) {
//		m_listswk.EditSetDisplayString( m_Swcd[line].sc_crename );
	}

	// 現在処理 データインデックス
	if( line != m_Didx ) {
		m_Didx = line;
		m_Tkyidx = 0;

		TkyoInpSet( m_Didx );
		SetNowLineBkColor( m_Didx );
	}
}

void CTkswkDlg::OnEditKillFocusIcsdblvSwk(long line, long clm, long inplen) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
TRACE( "OnEditKillFocusIcsdblvSwk %d\n", line );
	
}

void CTkswkDlg::OnEditImeEndCompositionIcsdblvSwk(long line, long clm, long nChar, LPCTSTR string, LPCTSTR ystring) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
TRACE( "OnEditImeEndCompositionIcsdblvSwk\n" );

struct _KamokuData kd;
DBKNREC* pKn;
int max;
max = m_Swcd.GetSize();
SWKCOD_DATA sc;
DWORD dwPjis;
BOOL bAdd = FALSE;

	if( max <= line ) {
		// 新規追加
		bAdd = TRUE;
		sc.sc_sgn = 0;
	}
	else {
		sc = m_Swcd[line];
	}

	if( KamokuStr_Chk( string, &kd, TRUE ) != -1 ) {
		int inpcd;
		CString pjis;
		inpcd = atoi( kd.kd_code );
		if( inpcd > 0 ) {
			dwPjis = m_pZm->InpcToPjis( inpcd );
			hl_rev( &dwPjis, 4 );
			pjis.Format( "%.08x", dwPjis );

			if( ! pjis.IsEmpty() ) {
				if( clm == SWC_DEBT ) {
					if( ! bAdd && sc.sc_sgn != 0 ) {
						sc.sc_dbt = pjis;
						pKn = DB_PjisToKnrec( sc.sc_dbt, m_pZm );
						if( pKn ) sc.sc_dbtname = pKn->knnam;
						else	  sc.sc_dbtname.Empty();

						sc.sc_sgn = 2;
					}
					else {
						sc.sc_dbt = pjis;
						pKn = DB_PjisToKnrec( sc.sc_dbt, m_pZm );
						if( pKn ) sc.sc_dbtname.Format( "%.14s", pKn->knnam );
						else	  sc.sc_dbtname.Empty();
					}
//					m_listswk.SetItem(line, clm, DBLV_DISPLAYINDEX, m_Swcd[line].sc_dbtname );
				}
				else {
					if( ! bAdd  && sc.sc_sgn != 0 ) {
						sc.sc_cre = pjis;
						pKn = DB_PjisToKnrec( sc.sc_cre, m_pZm );
						if( pKn ) sc.sc_crename.Format( "%.14s", pKn->knnam );
						else	  sc.sc_crename.Empty();

						sc.sc_sgn = 2;
					}
					else {
						sc.sc_sgn = 0;
						sc.sc_cre = pjis;
						pKn = DB_PjisToKnrec( sc.sc_cre, m_pZm );
						if( pKn ) sc.sc_crename.Format( "%.14s", pKn->knnam );
						else	  sc.sc_crename.Empty();
					}
//					m_listswk.SetItem(line, clm, DBLV_DISPLAYINDEX, m_Swcd[line].sc_crename );				
				}

				BOOL bArraySet = FALSE;

				if( sc.sc_sgn == 0 && clm == SWC_DEBT ) {
					bArraySet = TRUE;
				}
				else {
					if( DoubleReg_SwkChk( sc ) ) {
						CString str;
						str.Format( "　指定された仕訳\r\n\r\n　[借方]　%14.14s　[貸方]　%14.14s　\r\n\r\n\
　にはすでに登録されています！\r\n　この仕訳には変更できません！", sc.sc_dbtname, sc.sc_crename );
						ICSMessageBox( str,0,0,0,this );
					}
					else {
						bArraySet = TRUE;
					}
				}
				if( bArraySet ) {
					if( bAdd )	m_Swcd.Add( sc );
					else if( sc.sc_sgn ) {
						// 科目の変更があった場合は、DataBase も 更新
						if( ChangeSwtkData( line, sc ) ) {
							m_Swcd[line] = sc;
						}
						else {
							return;
						}
					}
					else {
						m_Swcd[line] = sc;	
					}

			//		if( clm == SWC_DEBT )
			//			m_listswk.SetItem(line, clm, DBLV_DISPLAYINDEX, m_Swcd[line].sc_dbtname );
			//		else
			//			m_listswk.SetItem(line, clm, DBLV_DISPLAYINDEX, m_Swcd[line].sc_crename );

				}
			} // if( ! pjis.IsEmpty() )
		}
		OnEditTerminationIcsdblvSwk(line, clm, nChar, 0, 0);
	}
}


void CTkswkDlg::OnEditTerminationIcsdblvSwk(long line, long clm, long nChar, long inplen, long kst) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください	
	BOOL	shift	=	FALSE;
	BOOL	bTkyMove = FALSE;
	if( kst & ICSDBEDT_KST_SHIFT ) {
		shift	=	TRUE;
	}

	int	newclm, newline, cnt;
	int	maxline	=	m_listswk.GetCount();

	VARIANT	v;
	SWKCOD_DATA sc;
	CString buf;

//	m_listswk.EditGetData( &v, ICSDBEDT_TYPE_STRING, 0 );

	if( nChar == VK_DOWN ) {
		newline	=	line + 1;
		newclm	=	clm;
	}
	else if( nChar == VK_UP ) {
		newline	=	line - 1;
		newclm	=	clm;
	}
	else if( nChar == VK_RIGHT || nChar == VK_RETURN || (nChar == VK_TAB && shift == FALSE) ) {
		newline	=	line;
		if( clm < SWC_CRED ) {
			newclm	=	clm + 1;
		}
		else {
			newline	=	line + 1;
			// 摘要部分へ
			newclm	=	0;
			bTkyMove = TRUE;
		}
	}
	else if( nChar == VK_LEFT || (nChar == VK_TAB && shift) ) {
		newline	=	line;
		if( clm == SWC_CRED ) {
			newclm	=	clm - 1;
		}
		else {
			newclm	=	0;
		}
	}
	else if( nChar == VK_DELETE ) {
		BOOL bDelJob = FALSE;
		newline = line;
		// 新規ラインは除く
		if( IsValidSwtkyoIdx( line ) ) {
			CString title, s1;
			title = "摘要登録処理";
			s1 = _T( "　該当ラインの仕訳を削除しますか？\r\n" );

			int st = ICSExtBox( title, s1, 
								_T("仕訳削除(&S)"), 
								_T( "科目削除(&K)" ), 
								_T( "入力に戻る" ), MB_DEFBUTTON3 );

			if( st == IDYES ) {
				DeleteSwtkData( line );
			}
			else if( st == IDINVALID_ICS ) {
				// 入力に戻る
			}
			else {
				bDelJob = TRUE;
			}
			// ﾌｫｰｶｽを 仕訳欄に
		//	PostMessage(WM_USER_1, WM_SETFOCUS, clm == SWC_DEBT ? SF_SWK_DEBT : SF_SWK_CRED );
		}
		// 科目削除処理
		if( bDelJob ) {

			sc = m_Swcd[line];

			if( clm == SWC_DEBT ) {	
				sc.sc_dbt.Empty();
				sc.sc_dbtname.Empty();
			}
			else {
				sc.sc_cre.Empty();
				sc.sc_crename.Empty();
			}

			// 仕訳削除
			if( sc.sc_sgn != 0 && sc.sc_dbt.IsEmpty() && sc.sc_cre.IsEmpty() ) {
				ICSMessageBox( "該当ラインの仕訳を削除します。",0,0,0,this );
				DeleteSwtkData( line );
			}
			else {
				m_Swcd[line] = sc;
				buf.Empty();

			//	if( clm == SWC_DEBT )
			//		m_listswk.SetItem( line, SWC_DEBT, DBLV_DISPLAYINDEX, buf );
			//	else
			//		m_listswk.SetItem( line, SWC_CRED, DBLV_DISPLAYINDEX, buf );

			}
		}
	}

	if( newline >= maxline || newline < 0 ) {
		newline	=	line;
		newclm	=	clm;
	}

	if( ! bTkyMove ) {
//		m_listswk.SetInput( newline, newclm, DBLV_DISPLAYINDEX );
	}
	else {
		// 仕訳のチェック
		sc = m_Swcd[line];

		if( sc.sc_sgn == 0 ) {
			if( sc.sc_dbt.IsEmpty() && sc.sc_cre.IsEmpty() ) {
//				m_listswk.SetInput( line, clm, DBLV_DISPLAYINDEX );
				return;
			}
			else if( DoubleReg_SwkChk( sc, line ) ) {
				CString str;
				str.Format( "　指定された仕訳\r\n\r\n　[借方]　%14.14s　[貸方]　%14.14s　\r\n\r\n\
　にはすでに登録されています！\r\n　この仕訳を追加することはできません！", sc.sc_dbtname, sc.sc_crename );
				ICSMessageBox( str,0,0,0,this );

//				m_listswk.SetInput( line, clm, DBLV_DISPLAYINDEX );
				return;
			}
			else {
		//		m_listtky.SetLineCount(TKY_LMAX);
		//		m_listtky.InsertItem( 0, 0 );

				buf.Empty();
		//		m_listtky.SetItem( 0, 0, DBLV_DISPLAYINDEX, buf );
		//		m_listtky.SetItem( 0, 1, DBLV_DISPLAYINDEX, buf );

				cnt = m_Swcd.GetSize();
				if( cnt >= m_listswk.GetCount() ) {
					sc.sc_dbt.Empty();
					sc.sc_dbtname.Empty();
					sc.sc_cre.Empty();
					sc.sc_crename.Empty();
					m_Swcd.Add( sc );

		//			m_listswk.InsertItem( cnt, 0);
					buf.Format( "%d", cnt+1 );
		//			m_listswk.SetItem( cnt, SWC_NO,	DBLV_DISPLAYINDEX, buf );
					buf.Empty();
		//			m_listswk.SetItem( cnt, SWC_DEBT,	DBLV_DISPLAYINDEX, buf );
		//			m_listswk.SetItem( cnt, SWC_CRED,	DBLV_DISPLAYINDEX, buf );
				}
			}
		}

		PostMessage( WM_USER_1, WM_SETFOCUS, MAKELONG( SF_TKY_INP, 0 ) );
	}
}

void CTkswkDlg::OnEditKanaTerminationIcsdblvSwk(long line, long clm, LPCTSTR kana) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	
}


#endif
/////////////////////////////////////////////////////////////////////////////////////////




