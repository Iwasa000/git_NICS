// TkkmkDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "resource.h"

#include "TkkmkDlg.h"

#include "DBInpSubFunc.h"

#include "TksrchDlg.h"
#include "SwkTkyDef.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTkkmkDlg ダイアログ
#define		LINE_OFS	425
#define		COLM_OFS	255
#define		PLIST_NLINE	60

#define		RNPG_MAX	999

int
num_chk( char* bf, int ln )
{

	CString tmp;
	tmp = bf;
	tmp.TrimLeft();

	BOOL b = isdigitstr( (char*)(LPCTSTR)tmp );

	return b ? 0 : 1;
}



CTkkmkDlg::CTkkmkDlg(CWnd* pParent /*=NULL*/)
	: CBaseDlg(CTkkmkDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTkkmkDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT

	CBaseDlg::m_Xdl = 492;
	CBaseDlg::m_Ydl = 260;

	m_RenDispPage = 0;
	m_PosMarkIndex = 0;
	m_InsIndex = -1;

	m_bFocusSet = FALSE;
	m_bNOkmk	= FALSE;
	m_bCHGmode	= FALSE;
	m_bListTermOK = TRUE;

	m_rnMax = 10000;
//	m_rnMax = 230;
	m_rnCnt = 0;
}


void CTkkmkDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTkkmkDlg)
	DDX_Control(pDX, IDC_LIST_KMNAM, m_RenSubList);
	DDX_Control(pDX, IDC_SPIN1, m_PageCtrl);
	DDX_Control(pDX, IDC_ICSLISTINPCTRL1, m_listtkren);
	DDX_Control(pDX, IDC_ICSDBEDTPAGE, m_DBedtRenpg);
	DDX_Control(pDX, IDC_BUTTON_CHG, m_BTchg);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT, m_EditPg);
}

// 仮想関数
// ウィンド表示／非表示
void CTkkmkDlg::WndShow(BOOL bShow)
{
	CBaseDlg::WndShow( bShow );
	// STATIC の位置調整
	if( bShow ) {
		MoveTitleCtrl();
		SetRensoList( m_RenDispPage );	// 摘要表示更新

		GetDlgItem(IDC_STATIC_KMNAM)->Invalidate();
		GetDlgItem(IDC_STATIC_KMNAM)->UpdateWindow();
		GetDlgItem(IDC_ST_REGCNT)->Invalidate();
		GetDlgItem(IDC_ST_REGCNT)->UpdateWindow();
	}
}

// 仮想関数
// 摘要追加時のウィンド表示／非表示
void CTkkmkDlg::WndShow_Add(BOOL bShow)
{
	WndShow( bShow );
}

// 仮想関数
// フォーカス移動
void CTkkmkDlg::FocusMove( int direction )
{
	CWnd* pwnd;
	pwnd = GetFocus();
	int id = -1;

	if( pwnd ) {
		id = pwnd->GetDlgCtrlID();
	}

	if( direction < 0 ) {
		if( id == IDC_RADIO_DEBT || id == IDC_RADIO_DEBT ) {
			GotoDlgCtrl( &m_listtkren );
		}
		else {
			if( pwnd ) {
			//	if( pwnd->IsKindOf( RUNTIME_CLASS( CICSListInp ) ) ) {
				if( m_listtkren.IsChild( pwnd ) ) {
					m_listtkren.DeleteInput();
				}				
			}
			PrevDlgCtrl();
		}
	}
	else {
		NextDlgCtrl();
	}
}

// 仮想関数
// フォーカス初期位置
void CTkkmkDlg::SetFirstPosition()
{
	CButton *pdeb, *pcre;
	pdeb = (CButton*)GetDlgItem(IDC_RADIO_DEBT);
	pcre = (CButton*)GetDlgItem(IDC_RADIO_CRED);
	
	int id;
	if( !m_bNOkmk ) id = GetCheckedRadioButton(IDC_RADIO_DEBT, IDC_RADIO_CRED);
	else			id = IDC_ICSDBEDTPAGE;
	
	if( id ) 	c_set( id );

}

// 仮想関数
// 初期化処理
void CTkkmkDlg::InitWork( double x, double y )
{
	CBaseDlg::InitWork( x, y );

	m_KanaIndex = -1;

	CString filter;
	filter = _T( "seq = 15" );
	m_pZm->dcntl->Requery( filter );
	m_rnMax = m_pZm->dcntl->mcnt;

	TKY_REGPAR par;
	memcpy( &par, get_tkyregpar(), sizeof TKY_REGPAR );

	CWnd *pdeb, *pcre;
	CString str;
	char tmp[64];
	DBKNREC *pKndeb, *pKncre;
	BOOL	bDebt, bCred;
	pdeb = GetDlgItem(IDC_RADIO_DEBT);
	pcre = GetDlgItem(IDC_RADIO_CRED);
	int pageno = 0;
	int dc_sw = par.tr_dcsw;

	bDebt = bCred = TRUE;
	// 借・貸 どちらの科目もない時は 全ページの 修正のみ可
	if( (! par.tr_debt[0] ) && (! par.tr_cred[0] ) ) {
		m_bNOkmk = TRUE;
		GetDlgItem(IDC_STATIC_FRM)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CHG)->ShowWindow(SW_HIDE);
		bDebt = bCred = FALSE;
	}

	// 単一科目対応摘要修正時
	//	(借方) 有効な科目コード, (貸方)	0
	//		を指定したとき
	if( par.tr_debt[0] && ! par.tr_cred[0] ) {
		pKndeb = DB_PjisToKnrec( par.tr_debt, m_pZm );
		if( !pKndeb ) {
			GetParent()->PostMessage( WM_USER_1, DIS_ENDDIALOG, 0 );
//			ICSExit(0, "科目コードが正しくありません！" );
			return;
		}
		str.Format( "『%.14s』の科目対応摘要", pKndeb->knnam );
		pdeb->SetWindowText( str );
		pcre->ShowWindow(SW_HIDE);
		bCred = FALSE;
	}
	else {
		// 借方
		if( ! par.tr_debt[0] ) {
			bDebt = FALSE;
			dc_sw = 1;
		}
		else {
			pKndeb = DB_PjisToKnrec( par.tr_debt, m_pZm );
			if( !pKndeb ) {
				GetParent()->PostMessage( WM_USER_1, DIS_ENDDIALOG, 0 );
//				ICSExit(0, "借方コードが正しくありません！" );
				return;
			}
			str.Format( "借方『%.14s』の科目対応摘要", pKndeb->knnam );
			pdeb->SetWindowText( str );
		}
		// 貸方
		if( ! par.tr_cred[0] ) {
			bCred = FALSE;
			dc_sw = 0;
		}
		else {
			pKncre = DB_PjisToKnrec( par.tr_cred, m_pZm );
			if( !pKncre ) {
				GetParent()->PostMessage( WM_USER_1, DIS_ENDDIALOG, 0 );
//				ICSExit(0, "貸方コードが正しくありません！" );
				return;
			}
			str.Format( "貸方『%.14s』の科目対応摘要", pKncre->knnam );
			pcre->SetWindowText( str );	
		}
	}

	// 科目対応摘要リストセット
	if( m_bNOkmk ) pageno = 1;
	else		   pageno = (!dc_sw) ? pKndeb->knrtb : pKncre->knrtb;
	SetRensoList( pageno );

	m_PageCtrl.SetBase( 10 );

	m_pZm->rtrec->Requery("");
	m_rnCnt = m_pZm->rtrec->GetRecordCount();
#ifdef CLOSE ////////////////////////////////////
	int cnt;
	cnt = m_rnCnt;
	cnt += (m_rnCnt/3);

	m_pZm->dcntl->MoveFirst();
	do {
		if( m_pZm->dcntl->itm == "rtrec" ) {
			cnt = m_pZm->dcntl->cnt;
			cnt += (cnt/3);
			break;
		}
	} while( m_pZm->dcntl->MoveNext() == 0 );
#endif //////////////////////////////////////////
	m_PageCtrl.SetRange( 1, RNPG_MAX/*cnt*/ );

	UDACCEL uda;
	uda.nSec = 0; uda.nInc = 1;
	m_PageCtrl.SetAccel( 1, &uda );

	m_RenDispPage = pageno;
	m_PageCtrl.SetPos( m_RenDispPage );

#ifndef DBEDIT_VER
	::ZeroMemory( tmp, sizeof tmp );
	if( pageno ) {	
		sprintf_s( tmp, sizeof tmp, "%d", pageno );
	}
	m_EditPg.SetWindowTextA( tmp );
#else
	if( pageno ) {	
		sprintf_s( tmp, sizeof tmp, "%d", pageno );
		var.pbVal = (BYTE*)tmp;
		DBdata_set(this, IDC_ICSDBEDTPAGE, &var, ICSDBEDT_TYPE_STRING, 0 );
	}
	else {
		var.pbVal = NULL;
		DBdata_set(this, IDC_ICSDBEDTPAGE, &var, ICSDBEDT_TYPE_STRING, 0 );
	}
#endif

	if( !m_bNOkmk ) {
#ifndef DBEDIT_VER
		m_EditPg.EnableWindow( FALSE );
#else
		m_DBedtRenpg.SetEnabled( FALSE );
#endif
		if( bDebt || bCred ) {
			CheckRadioButton( IDC_RADIO_DEBT,
							  IDC_RADIO_CRED,
							  (!dc_sw) ? IDC_RADIO_DEBT : IDC_RADIO_CRED
							);
		}
	}
	pdeb->EnableWindow( bDebt );
	pcre->EnableWindow( bCred );

	TorokDisp();
}

// 仮想関数
// ツールバー状態
BOOL CTkkmkDlg::IsEnableToolBar( int nID )
{
	BOOL bEnable = FALSE;
	int id;
	CWnd* pwnd;
	pwnd = GetFocus();

	switch( nID ) {
	case ID_TLBAR_F8:
		if( pwnd ) {
			id = pwnd->GetDlgCtrlID();
			if( id == IDC_ICSLISTINPCTRL1 || m_listtkren.IsChild( pwnd ) ) 
				bEnable = TRUE;
		}
		break;
	case ID_TLBAR_F6:
		break;
	default:
		break;

	}

	return bEnable;
}


// 登録数表示
void CTkkmkDlg::TorokDisp()
{
	char bf[64];

	sprintf_s( bf, sizeof bf, "%5d", m_rnCnt );
	GetDlgItem(IDC_STDSP_RGCNT)->SetWindowText( bf );
	sprintf_s( bf, sizeof bf, "%5d", m_rnMax );
	GetDlgItem(IDC_STDSP_RGMAX)->SetWindowText( bf );
}


BEGIN_MESSAGE_MAP(CTkkmkDlg, CBaseDlg)
	//{{AFX_MSG_MAP(CTkkmkDlg)
	ON_BN_CLICKED(IDC_RADIO_DEBT, OnRadioDebt)
	ON_BN_CLICKED(IDC_RADIO_CRED, OnRadioCred)
	ON_BN_CLICKED(IDC_BUTTON_CHG, &CTkkmkDlg::OnBnClickedButtonChg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_USER_1, OnUser1_Msg)
	ON_MESSAGE( WM_LIST_INSDEL, OnListInsDel)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CTkkmkDlg::OnDeltaposSpin1)
	ON_EN_KILLFOCUS(IDC_EDIT, &CTkkmkDlg::OnEnKillfocusEdit)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTkkmkDlg メッセージ ハンドラ
void CTkkmkDlg::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDlg::OnSize(nType, cx, cy);
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	
}


BOOL CTkkmkDlg::OnInitDialog() 
{
	int len = GetTekiyoLength( m_pZm );
	if( len > 0 )	m_TKYO_LEN = len;

	MakeTkrenFrm();

	CBaseDlg::OnInitDialog();
	
	// 連想摘要表示ページ番号　と　SpinCtrl の関連付け
#ifndef DBEDIT_VER
	m_PageCtrl.SetBuddy( &m_EditPg );
//	m_PageCtrl.EnableWindow( FALSE );
	m_EditPg.SetLimitText(3);
	m_DBedtRenpg.SetEnabled( FALSE );
	m_DBedtRenpg.ShowWindow( SW_HIDE );
#else
	m_PageCtrl.SetBuddy( &m_DBedtRenpg /*GetDlgItem( IDC_ICSDBEDTPAGE )*/);
	m_DBedtRenpg.EnableDelete(0);
# ifndef _DBEDT_FRAME_
//	m_DBedtRenpg.SetFrame( -1, RGB_DARK_GRAY );
# endif
#endif
	m_listtkren.EnableDelete(0);

	ICSDialog::OnInitDialogEX();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}


//
//	表形式入力　設定
void CTkkmkDlg::MakeTkrenFrm()
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
	LC[0].clm_width = 38;	//50
	LC[0].clm_tbcolor = CL_DKBLUE;
	LC[0].clm_ttcolor = CL_WHITE;
	
	// 摘要名称
	LC[1].clm_kind = LINP_KIND_STRING;
	LC[1].clm_max = (m_TKYO_LEN/2);
	LC[1].clm_title = "摘要名称";
	LC[1].clm_attr = LINP_ATTR_READONLY | LINP_ATTR_KANJI | LINP_ATTR_CENTER | LINP_ATTR_BOTTOM;
	LC[1].clm_width = 172;	//140
	LC[1].clm_tbcolor = CL_DKBLUE;
	LC[1].clm_ttcolor = CL_WHITE;

//	m_listtkren.SetColumn( 4, (LPUNKNOWN)LC );
	linp_setclm( IDC_ICSLISTINPCTRL1, 2, LC, CICSListInp);

	// 表形式入力　グリッド線の設定	
//	m_listtkren.SetHorzGrid( 5, CL_RED, LINP_GRID_SOLID );
	linp_horzgrid( IDC_ICSLISTINPCTRL1, 5, CL_RED, LINP_GRID_SOLID, CICSListInp );

	LVCOL[0].pos = 0;
	LVCOL[0].color = CL_GREEN;
	LVCOL[0].type = LINP_GRID_SOLID;

	LVCOL[1].pos = 1;
	LVCOL[1].color = CL_GREEN;
	LVCOL[1].type = LINP_GRID_SOLID;

//	m_listtkren.SetVartGrid( 3, (LPUNKNOWN)LVCOL );
	linp_vartgrid( IDC_ICSLISTINPCTRL1, 2, (LPUNKNOWN)LVCOL, CICSListInp );

}


// static タイトル 位置調整
//
void CTkkmkDlg::MoveTitleCtrl()
{
#ifdef DB_CLOSE
	if( m_listtkren ) {
		m_listtkren.RedrawControl();
		RECT	rc1, rc2;

		m_listtkren.GetRect( 0, 0, (LPUNKNOWN)&rc1 );
		m_Static1.GetWindowRect( &rc2 );
		rc2.left = rc1.left;
		rc2.right = rc1.right;
		ScreenToClient( &rc2 );
		m_Static1.MoveWindow( &rc2, TRUE );

		m_listtkren.GetRect( 0, 1, (LPUNKNOWN)&rc1 );
		m_Static2.GetWindowRect( &rc2 );
		rc2.left = rc1.left;
		m_listtkren.GetRect( 0, 2, (LPUNKNOWN)&rc1 );
		rc2.right = rc1.right;
		ScreenToClient( &rc2 );
		m_Static2.MoveWindow( &rc2, TRUE );

		m_listtkren.GetRect( 0, 3, (LPUNKNOWN)&rc1 );
		m_Static3.GetWindowRect( &rc2 );
		rc2.left = rc1.left;
		rc2.right = rc1.right;
		ScreenToClient( &rc2 );
		m_Static3.MoveWindow( &rc2, TRUE );
	}
#endif
}


//
//	連想摘要ページ作成
//
void CTkkmkDlg::SetRensoList( int pageno, BOOL bRequery/*=TRUE*/ )
{
//	struct _RTREC renrec;
//	struct _TKREC tk;
	CString	Tknam, codbf, filter;
	int	tkcod;
	LINP_DATA	LD[4];
	COLORREF	ONCOLOR, OFFCOLOR;
	LPCTSTR		p;
	int		kleng;

	m_InsIndex = -1;	// 09.02 /13

	if( ! IsWindow( m_listtkren.m_hWnd ) ) return;

	if( pageno <= 0 ) {
		Tklist_Clear();
		m_listtkren.SetEnabled(FALSE);
		m_listtkren.SetBackColor( CL_GRAY );
		m_listtkren.RedrawControl();
		// 該当科目リストボックスクリア
		KamokuRenPage( 0 );
		return;
	}
	else {
		m_listtkren.RemoveAll();
		m_listtkren.SetEnabled(TRUE);
		m_listtkren.SetBackColor( CL_TKBACK );
	}

	// 他で該当ページをクエリー済み
	if( bRequery ) {
		filter.Format( "rtpgn = %d", pageno );
		m_pZm->rtrec->Requery( filter );
	}
	
	ONCOLOR = CL_BLACK;
	OFFCOLOR = CL_TKBACK;
	int i = 0;

	if( m_pZm->rtrec->st != -1 ) {
		do {
			Tknam = "";

			tkcod = m_pZm->rtrec->rtcod;
			if( tkyngetr( tkcod, m_pZm ) == 0 ) {

				Tknam.Format( "%-40.40s", m_pZm->tkrec->tkname );

				// 摘要コード
				codbf.Format( "%6d", tkcod );
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
				kleng = kjlen( (void*)p, 20 );
				LD[1].dat_string = Tknam;
				LD[1].dat_attr = LINP_DISP_COLOR;
				LD[1].dat_fc = ONCOLOR;
				LD[1].dat_bc = OFFCOLOR;
				if( tkcod )
					LD[1].dat_initflg = FALSE;
				else
					LD[1].dat_initflg = TRUE;

				m_listtkren.SetData( i, (LPUNKNOWN)LD, TRUE );
				
				i++;
			}
		} while( m_pZm->rtrec->MoveNext() == 0 );
	}

	m_pZm->rtrec->Requery( "" );

//	RenPageCount( (void *)(m_pZm->pRnTeki+pageno) );

	if( m_rnCnt < m_rnMax ) {// 新規ライン
		AddEmptyData();
	}

	// 該当科目表示
	KamokuRenPage( pageno );

}


// 空データを追加
void CTkkmkDlg::AddEmptyData()
{
	LINP_DATA	LD[4];
	COLORREF	ONCOLOR, OFFCOLOR;
	ONCOLOR = CL_BLACK;
	OFFCOLOR = CL_TKBACK;

	if( m_rnCnt < m_rnMax ) {
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

		m_listtkren.AddData( (LPUNKNOWN)LD, TRUE);
	}
}


// 摘要リストをクリア
// 背景は灰色
void CTkkmkDlg::Tklist_Clear() 
{
	LINP_DATA	LD[4];
	
	for( int i=0; i < /*RNTKYO_MAX*/60; i++ ) {
		// 番号
		LD[0].dat_attr = LINP_DISP_COLOR;
		LD[0].dat_fc = CL_BLACK;
		LD[0].dat_bc = CL_GRAY;
		LD[0].dat_initflg = TRUE;
		// 摘要コード
		LD[1].dat_attr = LINP_DISP_COLOR;
		LD[1].dat_fc = CL_BLACK;
		LD[1].dat_bc = CL_GRAY;
		LD[1].dat_initflg = TRUE;
		// ﾀﾞﾐｰ		
		LD[2].dat_attr = LINP_DISP_COLOR;
		LD[2].dat_fc = CL_BLACK;
		LD[2].dat_bc = CL_GRAY;
		LD[2].dat_initflg = TRUE;
		// 摘要名称
		LD[3].dat_attr = LINP_DISP_COLOR;
		LD[3].dat_fc = CL_BLACK;
		LD[3].dat_bc = CL_GRAY;
		LD[3].dat_initflg = TRUE;

		m_listtkren.SetData( i, (LPUNKNOWN)LD, TRUE );
	}
}

// 該当科目名称のサブリスト
//
int	CTkkmkDlg::KamokuRenPage( int page )
{
	struct 	_DBKNREC	*kp;
	int		i, j;
	CString	work;

	m_RenSubList.ResetContent();

	for( i=j=0; i < m_pZm->knm_info->reccount; i++ ) {
		kp = (m_pZm->pKnrec +i);
		if( kp->knrtb == page && page != 0 ) {

			if( CheckKamokuRestrict( m_pZm, kp ) ) {
				work.Format( "%-14.14s", kp->knnam );
				m_RenSubList.InsertString( j, work );
				j++;
			}
		}
	}
	
	return( j );
}


void CTkkmkDlg::OnRadioDebt() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	PostMessage( WM_USER_1, WM_SETTEXT, 0 );
	PostMessage( WM_USER_1, WM_SETFOCUS, IDC_RADIO_DEBT );		
}


void CTkkmkDlg::OnRadioCred() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	PostMessage( WM_USER_1, WM_SETTEXT, 1 );
	PostMessage( WM_USER_1, WM_SETFOCUS, IDC_RADIO_CRED );	
}


// 連想摘要エリア切り替え
void CTkkmkDlg::ChangeRensoList( int dc_sw )
{
	// 科目設定なし
	if( m_bNOkmk )	return;

	TKY_REGPAR par;
	memcpy( &par, get_tkyregpar(), sizeof TKY_REGPAR );

	DBKNREC *pKn;
	CString	code;
	int pageno = 0;

	code = !dc_sw ? par.tr_debt : par.tr_cred;

	// 変更中のとき、元に戻す
	if( m_bCHGmode ) {
		m_bCHGmode = FALSE;
		m_CHGcode.Empty();

#ifndef DBEDIT_VER
		m_EditPg.EnableWindow(FALSE);
#else
		m_DBedtRenpg.SetEnabled( FALSE );
#endif
		m_PageCtrl.EnableWindow( FALSE );
		GetDlgItem(IDC_BUTTON_CHG)->SetWindowText("ﾍﾟｰｼﾞ変更[Home]");
	}

	if( code.IsEmpty() );
	else {
		pKn = DB_PjisToKnrec( code, m_pZm );
		if( pKn ) {
			// 科目対応摘要リストセット
			pageno = pKn->knrtb;
			SetRensoList( pageno );

			m_RenDispPage = pageno;
			m_PageCtrl.SetPos( m_RenDispPage );
	
#ifndef DBEDIT_VER
			char bf[30] = {0};
			if( pageno ) {
				sprintf_s( bf, sizeof bf, "%d", pageno );
			}
			m_EditPg.SetWindowTextA( bf );
#else
			VARIANT var;
			if( pageno ) {
				char bf[10];
				sprintf_s( bf, sizeof bf, "%d", pageno );
				var.pbVal = (BYTE*)bf;
				DBdata_set( this, IDC_ICSDBEDTPAGE, &var, ICSDBEDT_TYPE_STRING, 0 );
			}
			else {
				var.pbVal = NULL;
				DBdata_set( this, IDC_ICSDBEDTPAGE, &var, ICSDBEDT_TYPE_STRING, 0 );
			}
#endif
		}
	}	
}



BEGIN_EVENTSINK_MAP(CTkkmkDlg, CBaseDlg)
    //{{AFX_EVENTSINK_MAP(CTkkmkDlg)
	ON_EVENT(CTkkmkDlg, IDC_ICSLISTINPCTRL1, 1 /* EditON */, OnEditONIcslistinpctrl1, VTS_I2 VTS_I2)
	ON_EVENT(CTkkmkDlg, IDC_ICSLISTINPCTRL1, 4 /* KeyIn */, OnKeyInIcslistinpctrl1, VTS_I2 VTS_I2 VTS_I2)
	ON_EVENT(CTkkmkDlg, IDC_ICSLISTINPCTRL1, 2 /* Termination */, OnTerminationIcslistinpctrl1, VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_UNKNOWN)
	//}}AFX_EVENTSINK_MAP
	ON_EVENT(CTkkmkDlg, IDC_ICSDBEDTPAGE, 2, CTkkmkDlg::KillFocusIcsdbedtpage, VTS_I4)
	ON_EVENT(CTkkmkDlg, IDC_ICSDBEDTPAGE, 1, CTkkmkDlg::TerminationIcsdbedtpage, VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()


void CTkkmkDlg::OnEditONIcslistinpctrl1(short index, short column) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int tidx = m_listtkren.GetTopIndex();
	int	real_index = index + tidx;

	if( m_KanaIndex == -1 ) 
		m_KanaIndex	= real_index;

	if( m_InsIndex >= 0 && real_index != m_InsIndex ) {
		m_listtkren.DeleteInput();
		m_listtkren.RemoveData( m_InsIndex );
		m_InsIndex = -1;
		m_listtkren.SetInputPosition( index, column );
		return;
	}	
}

void CTkkmkDlg::OnKeyInIcslistinpctrl1(short nChar, short previndex, short index) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( nChar == VK_RETURN ) {
		m_listtkren.SetInputPosition( index, 0 );	
	}	
}

void CTkkmkDlg::OnTerminationIcslistinpctrl1(short nChar, short len, short index, short clm, LPUNKNOWN data) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	long	ret;
	char	pbuf[64];
	int		cnt, scod = 0;
	CString		TKnam, str;
	LINP_DATA	ldata[10];

	LINP_DATA	*LD = ( LINP_DATA *) data;	
	cnt = m_listtkren.GetCount();

	ret = LINP_REPLY_OK;
	switch( nChar ) {

		case	VK_RIGHT:
					ret |= LINP_REPLY_NEXT;
					break;

		case	VK_LEFT:
					ret |= LINP_REPLY_DELETE;
					break;

		case	VK_UP:
					ret |= LINP_REPLY_UP;
					break;

		case	VK_DOWN:
					ret |= LINP_REPLY_DOWN;
					break;

		case	VK_RETURN:
				// 摘要コード入力
				if( len && m_bListTermOK ) {
					l_print( pbuf, LD->dat_numeric, NUM_FMT );

					if( num_chk( pbuf, 6 ) == 0 ) {
						pbuf[6] = 0;
						scod = atoi( pbuf );

						// 摘要辞書 MAX オーバー
						if( scod == 0 ) {
							ret = LINP_REPLY_ERR;
							break;
						}

						if( tkyngetr( scod, m_pZm ) != 0 ) {
							MessageBeep( 0xffffffff );
							ret = LINP_REPLY_ERR;
							break;
						}
						else {
							TKnam = "";
							if( DoubleRegChk( scod, m_RenDispPage ) ) {
								ICSMessageBox("指定された摘要はすでに登録されています！", 0, 0,0,this);
								ret = LINP_REPLY_ERR;
							}
							else {
								TKnam.Format( "%-40.40s", m_pZm->tkrec->tkname );
								// 表示あり
								LD->dat_initflg = FALSE;
								ret = LINP_REPLY_NEW;
								SetTkrenListData( index, scod, TKnam );

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

					l_print( pbuf, LD[0].dat_numeric, NUM_FMT );
					pbuf[6] = 0;
					scod = atoi( pbuf );
					str.Format( "摘要[%6d] を削除します。", scod );

					m_bListTermOK = FALSE;
					if( ICSMessageBox( str, MB_YESNO, 0,0,this ) == IDYES ) {
						m_rnCnt--;
						DeleteRtrec( index );
						m_listtkren.RemoveData( index );
						int cnt = m_listtkren.GetCount();
						if( (cnt-1) == index )	index--;
						ret |= LINP_REPLY_STAY;
						TorokDisp();
					//	m_listtkren.SetInputPositionEX( index, clm );
						PostMessage(WM_LIST_INSDEL, 0, index );
					}
					m_bListTermOK = TRUE;

				//	LD->dat_initflg = TRUE;  // 0　は、表示しないため
				//	ret = LINP_REPLY_NEW;					
				}
				break;

		case	VK_INSERT:
				m_listtkren.GetData( index, (LPUNKNOWN)ldata );

				if( m_InsIndex >= 0 ) {
					if( ldata[0].dat_initflg || ! l_test( ldata[0].dat_numeric ) ) {
						m_listtkren.RemoveData( index );	// 空行を元に戻す
						m_InsIndex = -1;
					}
				}
				else if( (index+1) < cnt ) {
					ldata[0].dat_initflg = TRUE;
					ldata[1].dat_initflg = TRUE;
					m_listtkren.InsertData( index, (LPUNKNOWN)ldata, TRUE );
					m_InsIndex = index;
				}

				ret |= LINP_REPLY_STAY;

				break;
#ifdef CLOSE
		case	VK_HOME:
				if( m_listtkren.GetTopIndex() != 0 ) {
					m_listtkren.SetPosition( 0 );
					m_listtkren.SetInputPosition( 0, 1 );
				}
				else {
					m_listtkren.SetPosition( m_listtkren.GetCount()-1 );
					m_listtkren.SetInputPosition( m_listtkren.GetCount()-1 , 1 );
				}
				break;
#endif
		case	VK_PRIOR:
				m_listtkren.PageMove( -1 );
				break;

		case	VK_NEXT:
				m_listtkren.PageMove( 1 );
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

	m_listtkren.ReplyTermination( ret, data );

//	if( ret & LINP_REPLY_NEW ) {
//		SetTkrenListData( index, scod, TKnam );
//	}

}



// 二重登録チェック
//	int code ... 摘要ｺｰﾄﾞ
//
//	既に登録されていた場合は TRUE を返送
//
BOOL CTkkmkDlg::DoubleRegChk( int tkcod, int rpgno )
{
	BOOL ret = FALSE;

	CString filter;
	filter.Format( "rtpgn = %d", rpgno );

	CRTREC* rt;
	rt = m_pZm->rtrec;
	rt->Requery( filter );

	if( rt->st != -1 ) {

		rt->MoveFirst();
		do {
			if( rt->rtcod == tkcod ) {
				ret = TRUE;
				break;
			}
		} while( rt->MoveNext() == 0 );
	}

	return ret;
}


// 科目対応摘要レコードの書き換え
void CTkkmkDlg::SetTkrenListData( int index, int code, CString Kname )
{
	LINP_DATA LD[6];
	char	pbuf[16];

	m_listtkren.DeleteInput();

	m_listtkren.GetData( index, (LPUNKNOWN)LD );

	wsprintf( pbuf, "%06d", code );
	l_input( LD[0].dat_numeric, pbuf );
	LD[1].dat_string.Format( "%s", Kname );

	if( code ) {
		LD[0].dat_initflg = FALSE;
		LD[1].dat_initflg = FALSE;
	}
	else	{
		LD[0].dat_initflg = TRUE;
		LD[1].dat_initflg = TRUE;
	}

	m_listtkren.SetData( index, (LPUNKNOWN)LD, TRUE );
	SetTkrenRecord( index );

}

// マスター側のバッファにセット
void CTkkmkDlg::SetTkrenRecord( int index )
{
	LINP_DATA	LD[6];
	char	pbuf[64];
	int		code;

	m_listtkren.GetData( index, (LPUNKNOWN)LD );

	if( LD[0].dat_initflg ) code = 0;
	else {
		l_print( pbuf, LD[0].dat_numeric, NUM_FMT );
		pbuf[6] = 0;
		code = atoi( pbuf );
	}

	if( code ) {
		BOOL bAdd = FALSE;

		CRTREC* rt;
		rt = m_pZm->rtrec;
		CString filter;
		filter.Format( "rtpgn = %d", m_RenDispPage );
		rt->Requery( filter );
		int n = rt->GetRecordCount();

		if( m_rnCnt < m_rnMax && (m_InsIndex == index || index >= n) ) {
			bAdd = TRUE;
			m_InsIndex = -1;
		}
		int seq = index+1;

		if( bAdd ) {
			rt->Append( m_RenDispPage, seq, code );
			rt->Requery( "" );
			m_rnCnt++;
			AddEmptyData();
			TorokDisp();
			if( m_rnCnt < m_rnMax )	index++;
		}
		else {
			rt->Correct( m_RenDispPage, seq, seq, code );
			rt->Requery( "" );
		}
		m_bDBupdate = TRUE;

		m_listtkren.SetInputPositionEX( index, 0 );

		// 科目対応摘要登録件数　集計
	//	RenPageCount( (void *)(m_pZm->pRnTeki + m_RenDispPage -1) );
	}
}


void CTkkmkDlg::DeleteRtrec( int index )
{

	m_pZm->rtrec->Delete( m_RenDispPage, (index+1) );

	CString filter;
	filter.Format( "rtpgn = %d", m_RenDispPage );
	m_pZm->rtrec->Requery( filter );
//	m_pZm->rtrec->MoveFirst();
	m_bDBupdate = TRUE;

}


LRESULT CTkkmkDlg::OnUser1_Msg( WPARAM wParam, LPARAM lParam )
{
	switch( wParam ) {
	case WM_COMMAND:
		switch( lParam ) {
		// カナ検索ダイアログ表示
		case ID_TLBAR_F8:
			if( m_listtkren.GetEnabled() ) {
				{
					int index;
					if( m_KanaIndex == -1 ) {
						// カナ検索初回は、最後に追加する。
						if( m_rnCnt < m_rnMax ) {
							index = m_listtkren.GetCount() - 1;	// 空データ分 -1
						}
						else {
							index = m_listtkren.GetIndex() + m_listtkren.GetTopIndex();
						}
						m_KanaIndex = index;
					}
					else {
						index = m_listtkren.GetIndex() + m_listtkren.GetTopIndex();
					}
					// 摘要リストマーク ON
					TkrenListPosMark( 1, index );

					CPoint pt;
					CRect rc;
					m_listtkren.GetWindowRect( &rc );
					pt.x = rc.left;
					pt.y = rc.top;

					CTksrchDlg dlg( m_pZm, pt, 0, this );
	//				dlg.SetSize(-1,-1, CBaseDlg::m_Xrate, CBaseDlg::m_Yrate);
					if( dlg.DoModal() == IDOK ) {
						CString tknam;
						tkyngetr( dlg.m_TKcod, m_pZm );
						tknam.Format( "%-40.40s", m_pZm->tkrec->tkname );
						if( DoubleRegChk( dlg.m_TKcod, m_RenDispPage ) ) {
							ICSMessageBox("指定された摘要はすでに登録されています！", 0,0,0,this);
						}
						else {
							SetTkrenListData( index, dlg.m_TKcod, tknam );
						}
					}
					// カレントマーク消去
					TkrenListPosMark( 0 );
				}
				c_set( IDC_ICSLISTINPCTRL1 );
			}
			break;
		}
		break;

	// フォーカスセット
	case WM_SETFOCUS:
//TRACE( "user setfocus @tkkmk %d\n", lParam );
		if( !lParam ) SetFirstPosition();
		else		  c_set( lParam );
		break;
	// 摘要リストに データセット(WM_SETTEXT はその意味に使っている)
	case WM_SETTEXT:
		ChangeRensoList( lParam );
		break;
	}

	return 1;
}


// 挿入or削除時の処理
LRESULT CTkkmkDlg::OnListInsDel( WPARAM wParam, LPARAM lParam )
{
	if( wParam == 0	) { // 削除時
		m_listtkren.RedrawControl();
		m_listtkren.SetInputPositionEX( (short)lParam, 0 );
	}

	return 1;
}


// 摘要追加処理
void CTkkmkDlg::AddTekiyoJob( LPARAM lParam )
{
	TK_ADD* pAdd = (TK_ADD*)lParam;
	TKY_REGPAR par;
	memcpy( &par, get_tkyregpar(), sizeof TKY_REGPAR );

	int dc_sw = -1;	// ﾗｼﾞｵﾎﾞﾀﾝの位置

	switch( pAdd->ta_kmktyp ) {

	case 0:	// 借方科目対応摘要に追加
		if( AddTekiyoFunc( par.tr_debt, pAdd->ta_cod, 0 ) == 0 )
			dc_sw = 0;

		break;

	case 1:	// 貸方科目対応摘要に追加
		if( AddTekiyoFunc( par.tr_cred, pAdd->ta_cod, 1 ) == 0 ) 
			dc_sw = 1;

		break;
	case 2:	// 貸借科目対応摘要に追加
		if( AddTekiyoFunc( par.tr_debt, pAdd->ta_cod, 0 ) == 0 )
			dc_sw = 0;
		if( AddTekiyoFunc( par.tr_cred, pAdd->ta_cod, 1 ) == 0 ) {
			if( dc_sw < 0 ) dc_sw = 1;
		}

		break;

	default: break;

	}
	RTRECcount();
	TorokDisp();

	if( dc_sw >= 0 ) {
		int id = (!dc_sw) ? IDC_RADIO_DEBT : IDC_RADIO_CRED;
		CheckRadioButton( IDC_RADIO_DEBT,
						  IDC_RADIO_CRED,
						  id
						);

		PostMessage(WM_COMMAND, MAKELONG( id, BN_CLICKED ),
								(LPARAM)GetDlgItem( id )->m_hWnd );
	}
	else {
		SetFirstPosition();	
	}
}

// 摘要 を追加する
//		char* kmkcod .... 科目コード
//		int	tkcod ..... 摘要コード( 反転はしていない )
//		int dc_sw ..... 借方(0)、貸方(1)
//	--------------------------------------------------------
//		return 0 = 追加成功, -1 = 追加できなかった
//
int CTkkmkDlg::AddTekiyoFunc( char* kmkcod, int tkcod, int dc_sw )
{
	DBKNREC* pKn;
	int ret = 0;

	if( kmkcod[0] != 0  ) {
		pKn = DB_PjisToKnrec( kmkcod, m_pZm );
			
		if( pKn->knrtb > 0 ) {

			if( DoubleRegChk( tkcod, pKn->knrtb ) ) {
				ICSMessageBox("指定された摘要はすでに登録されています！", 0,0,0,this);
				ret = -1;
			}
			else {
				// DoubleRegChk 内で Requery()
				CRTREC* rt;
				rt = m_pZm->rtrec;

				rt->MoveLast();
				int seq;
				if( rt->st != -1 )
					seq = rt->rtseq + 1;
				else 
					seq = 1;
				
				rt->Append(  pKn->knrtb, seq, tkcod );
				rt->Requery( "" );
				m_bDBupdate = TRUE;
			}
		}
		else {
			ret = -1;
		}
	}
	else {
		ret = -1;
	}

	return ret;
}


BOOL CTkkmkDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( pMsg->message == WM_KEYDOWN ) {
		if( pMsg->wParam == VK_RETURN ) {
			HWND deb, cre;
			deb = GetDlgItem(IDC_RADIO_DEBT)->m_hWnd;
			cre = GetDlgItem(IDC_RADIO_CRED)->m_hWnd;

			if( pMsg->hwnd == deb || pMsg->hwnd == cre ) {
				NextDlgCtrl();
				return TRUE;
			}
		}
		else if( pMsg->wParam == VK_HOME ) {
			PostMessage(WM_COMMAND, MAKELONG(IDC_BUTTON_CHG, BN_CLICKED),
									(LPARAM)GetDlgItem(IDC_BUTTON_CHG)->m_hWnd );
			return TRUE;
		}

		CWnd* pwnd = CWnd::FromHandle( pMsg->hwnd );
		if( pwnd ) {
			int id = pwnd->GetDlgCtrlID();
			if( id == IDC_BUTTON_CHG ) {
				switch( pMsg->wParam ) {
				case VK_LEFT:
#ifndef DBEDIT_VER
					if( m_EditPg.IsWindowEnabled() )
						GotoDlgCtrl( &m_EditPg );
					else
						SetFirstPosition();
#else
					if( m_DBedtRenpg.GetEnabled() != FALSE )
						GotoDlgCtrl( &m_DBedtRenpg );
					else
						SetFirstPosition();
#endif
					return TRUE;
				case VK_UP:
					SetFirstPosition();
					return TRUE;
				case VK_RIGHT:
					PrevDlgCtrl();
					return TRUE;
				case VK_DOWN:
					PrevDlgCtrl();
					return TRUE;
				}			
			}
			else if( id == IDC_LIST_KMNAM ) {
				switch( pMsg->wParam ) {
				case VK_RETURN:
				case VK_RIGHT:
					NextDlgCtrl();
					return TRUE;
				case VK_LEFT:
					PrevDlgCtrl();
					return TRUE;
				}
			}
			else if( id == IDC_EDIT ) {
				if( TermEditRenPage( pMsg->wParam, 0, 0 ) == 0 )
					return TRUE;
			}
		}
	}

	return CBaseDlg::PreTranslateMessage(pMsg);
}


// 連想摘要ページ変更
void CTkkmkDlg::OnBnClickedButtonChg()
{
	int id = GetCheckedRadioButton(IDC_RADIO_DEBT, IDC_RADIO_CRED);

	if( !id || m_bNOkmk )	return;

	TKY_REGPAR par;
	memcpy( &par, get_tkyregpar(), sizeof TKY_REGPAR );

	CString code;
	DBKNREC *pKn;

	// 変更決定
	if( m_bCHGmode ) {
		pKn = DB_PjisToKnrec( m_CHGcode, m_pZm );
		if( !pKn ) {
			ICSExit(0, "科目コードが正しくありません！" );
			return;
		}

		// マスターエリアにセット
		CString filter;
		filter.Format( "kncod = '%s'", m_CHGcode );

		m_pZm->knrec->Requery( filter, 0 );

		if( m_pZm->knrec->st != -1 ) {
			m_pZm->knrec->Edit();
			m_pZm->knrec->knrtb = m_RenDispPage;
			m_pZm->knrec->Update();
			m_bDBupdate = TRUE;

			pKn->knrtb = m_RenDispPage;
		}
		m_bCHGmode = FALSE;
		m_CHGcode.Empty();

#ifndef DBEDIT_VER
		m_EditPg.EnableWindow( FALSE );
#else
		m_DBedtRenpg.SetEnabled( FALSE );
#endif
		m_PageCtrl.EnableWindow( FALSE );
		GetDlgItem(IDC_BUTTON_CHG)->SetWindowText("ﾍﾟｰｼﾞ変更[Home]");
		// 該当科目表示
		KamokuRenPage( m_RenDispPage );

		if( m_listtkren.GetEnabled() ) {
			m_listtkren.SetFocus();
			m_listtkren.SetInputPositionEX( 0, 1 );
		}
		else {
			int id = GetCheckedRadioButton(IDC_RADIO_DEBT, IDC_RADIO_CRED);
			if( id ) 	c_set( id );		
		}
		return;
	}

	// ページ変更
	code = (id == IDC_RADIO_DEBT) ? par.tr_debt : par.tr_cred;
	CString str;

	if( code.IsEmpty() );
	else {
		pKn = DB_PjisToKnrec( code, m_pZm );
		if( !pKn ) {
			ICSExit(0, "科目コードが正しくありません！" );
			return;
		}
		// 単一科目対応摘要修正時
		if( par.tr_debt[0] &&  par.tr_cred[0] == 0 ) {
			str.Format( "　『%.14s』の科目対応摘要ページを\r\n\r\n　変更しますか？\r\n", pKn->knnam );
		}
		else {
			str.Format( "　%s『%.14s』の科目対応摘要ページを\r\n\r\n　変更しますか？\r\n", 
						(id == IDC_RADIO_DEBT) ? "借 方" : "貸 方", pKn->knnam );
		}
	}

	if( ICSMessageBox( str, MB_YESNO|MB_DEFBUTTON2, 0,0,this ) == IDYES ) {
		m_PageCtrl.EnableWindow( TRUE );
		m_PageCtrl.Invalidate();
		m_PageCtrl.UpdateWindow();
#ifndef DBEDIT_VER
		m_EditPg.EnableWindow( TRUE );
		c_set( IDC_EDIT );
#else
		m_DBedtRenpg.SetEnabled( TRUE );
		c_set( IDC_ICSDBEDTPAGE );
#endif
		GetDlgItem(IDC_BUTTON_CHG)->SetWindowText("変更決定[Home]");
		// 変更中サインセット
		m_bCHGmode = TRUE;
		m_CHGcode = code;
	}
}

void CTkkmkDlg::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	int Pofs = pNMUpDown->iPos;
	*pResult = TRUE;

	if( (Pofs & 0xffff0000) == 0x00000000 ) {
		if( (Pofs + pNMUpDown->iDelta) > RNPG_MAX )
			;	
		else
			Pofs = Pofs + pNMUpDown->iDelta; // ポジションの増減

		CString filter;
		filter.Format( "rtpgn = %d", Pofs );
		m_pZm->rtrec->Requery( filter );

		if( (Pofs < 1) /*|| m_pZm->rtrec->st == -1*/ ) {
			Pofs = pNMUpDown->iPos;
			filter.Format( "rtpgn = %d", Pofs );
			m_pZm->rtrec->Requery( filter );
		}

		if( m_RenDispPage != Pofs ) {
			m_RenDispPage = Pofs;
			SetRensoList( Pofs, FALSE );
			*pResult = FALSE;
		}

		char buf[48];

#ifndef DBEDIT_VER
		sprintf_s( buf, sizeof buf, "%d", m_RenDispPage );
		m_EditPg.SetWindowText( buf );
#else
		VARIANT var;
		sprintf_s( buf, sizeof buf, "%d", m_RenDispPage );
		var.pbVal = (BYTE*)buf;

		DBdata_set(this, IDC_ICSDBEDTPAGE, &var, ICSDBEDT_TYPE_STRING, 0 );
		((CDialog *)this)->GotoDlgCtrl( GetDlgItem( IDC_ICSDBEDTPAGE) );
#endif
	}
}


//
//	連想摘要ページ KillFocus
//
void CTkkmkDlg::KillFocusIcsdbedtpage(long inplen)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	TermEditRenPage( 0, inplen, 0 );
}

//
//	連想摘要ページ Termination
//
void CTkkmkDlg::TerminationIcsdbedtpage(long nChar, long inplen, long kst)
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	TermEditRenPage( nChar, inplen, kst );
}


long CTkkmkDlg::TermEditRenPage(long nChar, long inplen, long kst)
{
	CWnd	*pwnd;
#ifndef DBEDIT_VER
	UINT nID = IDC_EDIT;
#else
	UINT nID = IDC_ICSDBEDTPAGE;
#endif

	if( (pwnd = GetDlgItem( nID )) ) {
		if( ! ::IsWindow( pwnd->m_hWnd ) )
			return 0;
	}

	char	buf[256];
	int		pg;
//	VARIANT	var;

	if( nChar == VK_RETURN ) {
#ifndef DBEDIT_VER
		m_EditPg.GetWindowText( buf, sizeof buf );
		pg = atoi( buf );
#else
		DBdata_get( this, IDC_ICSDBEDTPAGE, &var, ICSDBEDT_TYPE_STRING, 0 );
		if( var.pbVal == NULL )		pg = 0;
		else {
			pg = atoi( (const char*)var.pbVal );
		}
#endif
		if( pg == 0) {
			m_RenDispPage = 0;
			SetRensoList( m_RenDispPage );
			c_set( IDC_BUTTON_CHG );
		}
		else {
#ifdef REGPAGE_CHECK // 登録済みのページのみ
			CString filter;
			filter.Format( "rtpgn = %d", pg );
			m_pZm->rtrec->Requery( filter );

			if(m_pZm->rtrec->st == -1 ) {
				filter.Format( "rtpgn = %d", m_RenDispPage );
				m_pZm->rtrec->Requery( filter );

#ifndef DBEDIT_VER
				if( m_RenDispPage ) {
					sprintf_s( buf, sizeof buf, "%d", m_RenDispPage );
				}
				else {
					::ZeroMemory( buf, sizeof buf );
				}
				m_EditPg.SetWindowText( buf );
#else
				if( m_RenDispPage ) {
					sprintf_s( buf, sizeof buf, "%d", m_RenDispPage );
					var.pbVal = (BYTE*)buf;
				}
				else {
					var.pbVal = NULL;
				}
				DBdata_set( this, IDC_ICSDBEDTPAGE, &var, ICSDBEDT_TYPE_STRING, 0 );
#endif
				// 対応ページなし
				if( ! m_bNOkmk ) {
					m_PageCtrl.SetPos ( m_RenDispPage );
					SetRensoList( m_RenDispPage );
					c_set( IDC_BUTTON_CHG );
				}
				else {
					// 借貸 科目なし
				}
			}
			else {
#endif // REGPAGE_CHECK
				// 他の科目の登録がなくても、登録させる
				m_RenDispPage = pg;
				m_PageCtrl.SetPos ( m_RenDispPage );
				SetRensoList( m_RenDispPage );
				c_set( m_bNOkmk ? IDC_ICSLISTINPCTRL1 : IDC_BUTTON_CHG);
//			}
		}
		nChar = 0;
	}
	else if( nChar == VK_DELETE ) {
		if( !m_bNOkmk ) {
			m_RenDispPage = 0;
			m_PageCtrl.SetPos ( m_RenDispPage );
			SetRensoList( m_RenDispPage );
			m_EditPg.SetWindowText( "" );

			c_set( IDC_BUTTON_CHG );
		}
		nChar = 0;
	}
	else if( nChar == VK_UP || nChar == VK_DOWN ) {
//		m_PageCtrl.PostMessage( WM_KEYDOWN, nChar );
//		PostMessage( WM_NOTIFY, );
//		UDN_DELTAPOS
	}
	else if( nChar == 0 ) {
		if( m_RenDispPage ) {
#ifndef DBEDIT_VER
			sprintf_s( buf, sizeof buf, "%d", m_RenDispPage );
			m_EditPg.SetWindowTextA( buf );
#else
			sprintf_s( buf, sizeof buf, "%d", m_RenDispPage );
			var.pbVal = (BYTE*)buf;
			DBdata_get( this, IDC_ICSDBEDTPAGE, &var, ICSDBEDT_TYPE_STRING, 0 );
#endif
		}
	}

	return nChar;
}

void CTkkmkDlg::OnEnKillfocusEdit()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	TermEditRenPage( 0, 0, 0 );
}



//	表形式入力位置　マーク　表示／非表示
void CTkkmkDlg::TkrenListPosMark( int mode, int set_idx/*=-1*/ )
{
	short index;

	if( set_idx == -1 ) {
		index = m_listtkren.GetTopIndex();
		index += m_listtkren.GetIndex();
	}
	else {
		index = set_idx;
	}

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

	m_listtkren.GetColumnData( index, 0, (LPUNKNOWN)LD );

	if( (LD[0].dat_fc != ON_C) || (LD[0].dat_bc != OFF_C) ) {
		LD[0].dat_fc = ON_C;
		LD[0].dat_bc = OFF_C;
		m_listtkren.SetColumnData( index, 0, (LPUNKNOWN)LD, TRUE );
	}
}

#include "CStRecordSet.h"

BOOL CTkkmkDlg::RTRECcount()
{
	CStRecordSet st_rec(m_pZm->m_database);
	CString strError;

	TRY{
		st_rec.m_sql.Format("select count(*) as st from rtrec");
		if( !st_rec.Open() ) {
			ICSMessageBox(_T("摘要の使用チェックが行えません！"));
			return FALSE;
		}

		if( st_rec.m_st > 0 ) {
			m_rnCnt = st_rec.m_st;
		}
		st_rec.Close();
	}
	CATCH(CMemoryException, me) {
		me->GetErrorMessage(strError.GetBuffer(_MAX_PATH), _MAX_PATH);
		strError.ReleaseBuffer();
		ICSMessageBox(strError);
		return FALSE;
	}
	CATCH(CDBException, de) {
		strError = de->m_strError;
		ICSMessageBox(strError);
		return FALSE;
	}
	END_CATCH

	return TRUE;
}

