// TkyVer2Dlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "resource.h"

#include "DBInpSubFunc.h"

#include "icsdisp.h"
#include "TkyVer2Dlg.h"

#include "TkyEdaDlg.h"
#include "TkySel.h"


//#define TKYDIC_MAX		TKYO_LEN

extern
int get_strcnt(char* str, int byte);

extern
void filter_swkdata( CString* filter, SWKCOD_DATA* pdata );


extern
int NewEdabanSearch( CDBZmSub* pzm, LPCTSTR kcod, int eda, int ketachk = 0 );

// CTkyVer2Dlg ダイアログ

IMPLEMENT_DYNAMIC(CTkyVer2Dlg, ICSDialog)

CTkyVer2Dlg::CTkyVer2Dlg(CWnd* pParent /*=NULL*/)
	: ICSDialog(CTkyVer2Dlg::IDD, pParent)
{
	m_pZm = NULL;
	m_pSy = NULL;

	m_bDBupdate	= FALSE;
	m_bReg		= FALSE;

	m_pDeb	= m_pCre = NULL;

	m_bEZquery = FALSE;

	m_debBrn = m_creBrn = -1;

	::ZeroMemory( m_debZan, sizeof m_debZan );
	::ZeroMemory( m_creZan, sizeof m_creZan );

	m_svTky.Empty();
	m_svKana.Empty();

	m_bEdaReg = FALSE;

	m_SvMode = FALSE;

	m_addTKcod = 0;

	m_TKYO_LEN = TKYO_LEN;

	m_bInvMaster = false;
	m_bInvUse = false;
	m_bInvDisp = false;

	::ZeroMemory(m_TkyInvno, sizeof m_TkyInvno);
	m_bIgnoreFocus = FALSE;
	m_Menchk = 0;
}

CTkyVer2Dlg::~CTkyVer2Dlg()
{
}

void CTkyVer2Dlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ICSDBEDT8_KANA, m_DBEkana);
	DDX_Control(pDX, IDC_ICSDBEDT8_DZBRN, m_DZbrn);
	DDX_Control(pDX, IDC_ICSDBEDT8_CZBRN, m_CZbrn);
	DDX_Control(pDX, IDC_ICSDBEDT_INV , m_DBEinv);
	DDX_Control(pDX, IDC_COMBO_MEN, m_ComboMen);
}


void CTkyVer2Dlg::InitDlg( CDBZmSub* pZm, CDBSyohi* pSy )
{
	m_pZm = pZm;
	m_pSy = pSy;

	m_bDBupdate	= FALSE;
	m_bEdaMast = FALSE;

	int len = GetTekiyoLength( m_pZm );
	if( len > 0 )	m_TKYO_LEN = len;
}


BEGIN_MESSAGE_MAP(CTkyVer2Dlg, ICSDialog)
	ON_BN_CLICKED(IDC_BTN_REG, &CTkyVer2Dlg::OnBnClickedBtnReg)
	ON_MESSAGE( WM_RETROFOCUS, &CTkyVer2Dlg::OnRetroFocus)
	ON_MESSAGE(WM_MYMESSAGE, OnMyMessage)
	ON_BN_CLICKED(IDC_CHK_TKYONLY, &CTkyVer2Dlg::OnBnClickedChkTkyonly)
	ON_BN_CLICKED(IDC_BRNCRE_BTN, &CTkyVer2Dlg::OnBnClickedBrncreBtn)
	ON_BN_CLICKED(IDC_BRNDEB_BTN, &CTkyVer2Dlg::OnBnClickedBrndebBtn)
	ON_BN_CLICKED(IDC_BTN_TKYSEL, &CTkyVer2Dlg::OnBnClickedBtnTkysel)
END_MESSAGE_MAP()


// CTkyVer2Dlg メッセージ ハンドラ

BOOL CTkyVer2Dlg::OnInitDialog()
{
	// インボイス対応
	CZmGen8 zmGen;

	// バージョンアップマスター判定
	m_bInvMaster = zmGen.IsInvoiceVupMaster(m_pZm);
	if( m_bInvMaster ) {
		// インボイス期間か
		m_bInvUse = zmGen.IsAbleToUseInvoice(m_pZm);
		// 登録番号を表示するか
		m_bInvDisp = zmGen.IsDispInvno(m_pZm);
	}
	else {
		m_bInvUse = false;
		m_bInvDisp = false;
	}

	syzInvoice.SetDBZmSub(m_pZm);

	ICSDialog::OnInitDialog();

	m_DBEkana.EnableDelete(0);
	m_DBEkana.ImeEndMode( FALSE );
	m_DBEinv.EnableDelete(0);

	((CICSDBEDT*)GetDlgItem(IDC_ICSDBEDT8_TKY))->SetMaxLen( m_TKYO_LEN );

	CString strMsg;
	strMsg.Format( "(最大全角%d文字)", m_TKYO_LEN/2 );
	GetDlgItem(IDC_STATIC15)->SetWindowText( strMsg );

	TKY_VER2*	ptv = get_tkyver2();
	// 摘要文字列セット
	WORD sp, ep;
	int st;
	char	buf[128] = {0};
	char	kanabf[32] = {0};
	VARIANT var;

	sp = LOWORD( ptv->t2_sel );
	ep = HIWORD( ptv->t2_sel );

	if( (m_pZm->zvol->br_sw & 0x01) )	m_bEdaMast = TRUE;

	if( m_SvMode ) {
		strcpy_s( buf, sizeof buf, m_Save.tky );

		var.pbVal = (BYTE*)buf;
		DBdata_set( this, IDC_ICSDBEDT8_TKY, &var, ICSDBEDT_TYPE_STRING, 0 );

		strcpy_s( kanabf, sizeof kanabf, m_Save.kana );

		m_debBrn	=	m_Save.debBrn;
		m_creBrn	=	m_Save.creBrn;

		memcpy( m_debZan, m_Save.dZan, 6 );
		memcpy( m_creZan, m_Save.cZan, 6 );

		m_svTky		=	m_Save.sv_tky;
		m_svKana	=	m_Save.sv_kana;

		memcpy(m_TkyInvno, m_Save.sv_invno, sizeof m_TkyInvno);
		m_Menchk = m_Save.sv_menchk;
	}
	else {
		// 先頭から摘要をセット
		if( ptv->t2_tky[0] ) {
			if( sp == (WORD)0xffff || ep == (WORD)0xffff || (ep-sp) <= 0 ) {
				st = get_strcnt( ptv->t2_tky, m_TKYO_LEN );
				memcpy( buf, ptv->t2_tky, st );

				var.pbVal = (BYTE*)buf;

				DBdata_set( this, IDC_ICSDBEDT8_TKY, &var, ICSDBEDT_TYPE_STRING, 0 );
			}
			else {
				// 選択された部分から
				if( (ep-sp) > m_TKYO_LEN ) {
					st = get_strcnt( &ptv->t2_tky[sp], m_TKYO_LEN );
					memcpy( buf, &ptv->t2_tky[sp], st );
				}
				else {
					memcpy( buf, &ptv->t2_tky[sp], (ep-sp) );
				}

				var.pbVal = (BYTE*)buf;
				DBdata_set( this, IDC_ICSDBEDT8_TKY, &var, ICSDBEDT_TYPE_STRING, 0 );
			}

			CRevConv rv;
			DWORD opt;
			opt = RVF_KATAKANA | RVF_HANKAKU;
			CString kana;

			if( rv.GetFurigana( opt, CString(buf), kana ) == 0 ) {
				kana_chk( &kana, kana.GetLength() );
				furikana_chk( kanabf, sizeof kanabf, kana );
			}
		}
		memcpy(m_TkyInvno, ptv->t2_invno, sizeof m_TkyInvno);
		m_Menchk = ptv->t2_menchk;
	}

	if( kanabf[0] )	var.pbVal = (BYTE*)kanabf;
	else			var.pbVal = (BYTE*)NULL;
	DBdata_set( this, IDC_ICSDBEDT8_KANA, &var, ICSDBEDT_TYPE_STRING, 0 );

	// チェックボックス文字列
	CString str;
	m_pDeb = DB_PjisToKnrec( ptv->t2_debt, m_pZm );
	m_pCre = DB_PjisToKnrec( ptv->t2_cred, m_pZm );

	var.pbVal = (BYTE*)m_debZan;
	DBdata_set( this, IDC_ICSDBEDT8_DZBRN, &var, ICSDBEDT_TYPE_ARITH, 0 );
	var.pbVal = (BYTE*)m_creZan;
	DBdata_set( this, IDC_ICSDBEDT8_CZBRN, &var, ICSDBEDT_TYPE_ARITH, 0 );

	//枝番残高
	BOOL bDebEda, bCreEda;

	if( m_bEdaMast ) {
		if( m_pDeb && ! isSYOGT_CODE( ptv->t2_debt ) )
			bDebEda = TRUE;
		else
			bDebEda = FALSE;

		if( m_pCre && ! isSYOGT_CODE( ptv->t2_cred ) )
			bCreEda = TRUE;
		else
			bCreEda = FALSE;
	}
	else {
		bDebEda = bCreEda = FALSE;
	}

	if( bDebEda ) {
		str.Format( "借方『%-14.14s』の枝番をこの摘要で登録する", m_pDeb->knnam );
	}
	else  {
		str = "－－－－－－－－－－－－－－－－－－－－";
	}
	GetDlgItem( IDC_CHK_BRNDEB )->SetWindowText( str );
	if( ! bDebEda ) {
		GetDlgItem( IDC_CHK_BRNDEB )->EnableWindow( FALSE );
		GetDlgItem( IDC_STATIC_G7 )->EnableWindow( FALSE );
		GetDlgItem( IDC_BRNDEB_BTN )->EnableWindow( FALSE );
		GetDlgItem( IDC_STATIC16 )->EnableWindow( FALSE );
		m_DZbrn.EnableWindow( FALSE );
	}

	if( bCreEda ) {
		str.Format( "貸方『%-14.14s』の枝番をこの摘要で登録する", m_pCre->knnam );
	}
	else {
		str = "－－－－－－－－－－－－－－－－－－－－";
	}
	GetDlgItem( IDC_CHK_BRNCRE )->SetWindowText( str );
	if( ! bCreEda ) {
		GetDlgItem( IDC_CHK_BRNCRE )->EnableWindow( FALSE );
		GetDlgItem( IDC_STATIC_G8 )->EnableWindow( FALSE );
		GetDlgItem( IDC_BRNCRE_BTN )->EnableWindow( FALSE );
		GetDlgItem( IDC_STATIC17 )->EnableWindow( FALSE );
		m_CZbrn.EnableWindow( FALSE );
	}


	if( m_pDeb ) {
		str.Format( "借方『%-14.14s』の科目対応摘要 に登録する", m_pDeb->knnam );
	}
	else  {
		str = "－－－－－－－－－－－－－－－－－－－－";
	}
	GetDlgItem( IDC_CHK_KMKDEB )->SetWindowText( str );
	if( m_pDeb == NULL || m_pDeb->knrtb == 0 ) {
		GetDlgItem( IDC_CHK_KMKDEB )->EnableWindow( FALSE );
		GetDlgItem( IDC_STATIC_G1 )->EnableWindow( FALSE );
	}

	if( m_pCre ) {
		str.Format( "貸方『%-14.14s』の科目対応摘要 に登録する", m_pCre->knnam );
	}
	else {
		str = "－－－－－－－－－－－－－－－－－－－－";
	}
	GetDlgItem( IDC_CHK_KMKCRE )->SetWindowText( str );
	if( m_pCre == NULL || m_pCre->knrtb == 0 ) {
		GetDlgItem( IDC_CHK_KMKCRE )->EnableWindow( FALSE );
		GetDlgItem( IDC_STATIC_G2 )->EnableWindow( FALSE );
	}

	BOOL bEnable;
	if( m_pDeb && m_pCre ) {
		str.Format( "(仕訳) %-14.14s ／ %-14.14s　の仕訳摘要 に登録する", m_pDeb->knnam, m_pCre->knnam );
		bEnable = TRUE;
	}
	else {
		str = "－－－－－－－－－－－－－－－－－－－－";
		bEnable = FALSE;
	}
	GetDlgItem( IDC_CHK_SWKTKY )->SetWindowText( str );
	GetDlgItem( IDC_CHK_SWKTKY )->EnableWindow( bEnable );
	GetDlgItem( IDC_STATIC_G3 )->EnableWindow( bEnable );

	if( m_pDeb ) {
		str.Format( "(単独) %-14.14s　の摘要 に登録する", m_pDeb->knnam );
		bEnable = TRUE;
	}
	else {
		str = "－－－－－－－－－－－－－－－－－－－－";
		bEnable = FALSE;
	}
	GetDlgItem( IDC_CHK_TANDEB )->SetWindowText( str );
	GetDlgItem( IDC_CHK_TANDEB )->EnableWindow( bEnable );
	GetDlgItem( IDC_STATIC_G4 )->EnableWindow( bEnable );

	if( m_pCre ) {
		str.Format( "(単独) %-14.14s　の摘要 に登録する", m_pCre->knnam );
		bEnable = TRUE;
	}
	else {
		str = "－－－－－－－－－－－－－－－－－－－－";
		bEnable = FALSE;
	}
	GetDlgItem( IDC_CHK_TANCRE )->SetWindowText( str );
	GetDlgItem( IDC_CHK_TANCRE )->EnableWindow( bEnable );
	GetDlgItem( IDC_STATIC_G5 )->EnableWindow( bEnable );

	if( ! m_bEdaReg ) {
		EdaCtrlWork();
	}

	ICSDialog::OnInitDialogEX();

	c_disp( IDC_ICSDISPCTRL1, 0, 1, "＊入力時の表示", CICSDisp );
	c_disp( IDC_ICSDISPCTRL1, 2, 1, 
			"・仕訳摘要として登録した場合、その仕訳に対応した摘要として", CICSDisp );
	c_disp( IDC_ICSDISPCTRL1, 3, 1, "　表示します。", CICSDisp );
	c_disp( IDC_ICSDISPCTRL1, 4, 1, 
			"・単独科目摘要として登録した場合、借方・貸方の発生に関係なく", CICSDisp );
	c_disp( IDC_ICSDISPCTRL1, 5, 1, "　摘要を表示します。", CICSDisp );

	if( m_SvMode )	SetCheckFromSave();

	//摘要選択ボタン
	if( CTkySel::m_orgTky.GetLength() <= 0 ) {
		GetDlgItem(IDC_BTN_TKYSEL)->EnableWindow(FALSE);
	}

	//摘要番号
	tkdb_init( m_pZm );

	m_pZm->tkrec->MoveLast();
	if( m_pZm->tkrec->st == -1 )	m_addTKcod = 1;
	else							m_addTKcod = m_pZm->tkrec->tkcod+1;
	str.Format( "%6d", m_addTKcod );
	GetDlgItem( IDC_STATIC_TKNUM )->SetWindowText( str );

	if( m_bInvMaster ) {
		m_ComboMen.SetCurSel(m_Menchk);
	}
	//インボイス登録番号
	if( m_bInvDisp ) {
		CString strWork;
		str.Empty();
		if( m_TkyInvno[0] ) {
			if( m_TkyInvno[0] == 'T' ) {
				str = m_TkyInvno;
				strWork = m_TkyInvno;

				strWork.Replace("T", "");
				var.pbVal = (BYTE*)strWork.GetBuffer(128);
				strWork.ReleaseBuffer(-1);
			}
			else {
				str.Format(_T("T%s"), m_TkyInvno);
				var.pbVal = (BYTE*)m_TkyInvno;
			}
		}
		DBdata_set(this, IDC_ICSDBEDT_INV, &var, ICSDBEDT_TYPE_STRING, 0);
		DBSetDispString(this, IDC_ICSDBEDT_INV, str);
	}
	else {
		InvCtrlWork();
	}

	if( ! m_bEdaReg ) {
		EdaCtrlWork(1);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

//枝番部分コントロールID
static
int brn_id[] = {
	IDC_STATIC_FRM4, IDC_CHK_BRNDEB, IDC_STATIC_G7,IDC_BRNDEB_BTN,IDC_STATIC16, IDC_ICSDBEDT8_DZBRN,
					 IDC_CHK_BRNCRE, IDC_STATIC_G8,IDC_BRNCRE_BTN,IDC_STATIC17, IDC_ICSDBEDT8_CZBRN, 0
};

//科目対応摘要部分コントロールID
static
int kmk_id[] = {
	IDC_STATIC_FRM,IDC_STATIC_G1,IDC_CHK_KMKDEB,IDC_STATIC_G2,IDC_CHK_KMKCRE,0
};
//仕訳対応摘要以下の部分コントロールID
static
int mv_id[] = {
	IDC_STATIC_FRM2,IDC_STATIC_G3,IDC_STATIC_G4,IDC_STATIC_G5,
	IDC_CHK_SWKTKY,IDC_CHK_TANDEB,IDC_CHK_TANCRE,IDC_ICSDISPCTRL1,
	IDC_BTN_REG,IDOK,IDCANCEL,0
};

// int sign ... 0 = コントロール初期移動時
//			... 1 = コントロール移動
//
void CTkyVer2Dlg::EdaCtrlWork(int sign)
{
	CRect rect, rc;
	GetDlgItem( IDC_STATIC_FRM3 )->GetWindowRect( rect );
	ScreenToClient( &rect );

	if( sign == 0 ) {
		for( int i = 0; brn_id[i] != 0; i++ ) {
			SetResize( brn_id[i] );
		}
	}
	//disable
	GetDlgItem( IDC_STATIC_FRM4 )->EnableWindow( FALSE );
	GetDlgItem( IDC_CHK_BRNDEB )->EnableWindow( FALSE );
	GetDlgItem( IDC_STATIC_G7 )->EnableWindow( FALSE );
	GetDlgItem( IDC_BRNDEB_BTN )->EnableWindow( FALSE );
	GetDlgItem( IDC_STATIC16 )->EnableWindow( FALSE );
	m_DZbrn.EnableWindow( FALSE );

	GetDlgItem( IDC_CHK_BRNCRE )->EnableWindow( FALSE );
	GetDlgItem( IDC_STATIC_G8 )->EnableWindow( FALSE );
	GetDlgItem( IDC_BRNCRE_BTN )->EnableWindow( FALSE );
	GetDlgItem( IDC_STATIC17 )->EnableWindow( FALSE );
	m_CZbrn.EnableWindow( FALSE );

	//不可視
	GetDlgItem( IDC_CHK_BRNDEB )->ShowWindow( SW_HIDE );
	GetDlgItem( IDC_STATIC_G7 )->ShowWindow( SW_HIDE );
	GetDlgItem( IDC_BRNDEB_BTN )->ShowWindow( SW_HIDE );
	GetDlgItem( IDC_STATIC16 )->ShowWindow( SW_HIDE );
	m_DZbrn.ShowWindow( SW_HIDE );

	GetDlgItem( IDC_CHK_BRNCRE )->ShowWindow( SW_HIDE );
	GetDlgItem( IDC_STATIC_G8 )->ShowWindow( SW_HIDE );
	GetDlgItem( IDC_BRNCRE_BTN )->ShowWindow( SW_HIDE );
	GetDlgItem( IDC_STATIC17 )->ShowWindow( SW_HIDE );
	m_CZbrn.ShowWindow( SW_HIDE );
	GetDlgItem( IDC_STATIC_FRM4 )->ShowWindow( SW_HIDE );

	int i;
	int ht = rect.Height();

	int start_ypos = rect.bottom + 20;
	int idou = 0;
	int ctrl_ht;

	for( i = 0; kmk_id[i] != 0; i++ ) {
		GetDlgItem( kmk_id[i] )->GetWindowRect( rc );
		ScreenToClient( &rc );
		ctrl_ht = rc.Height();

		if( i == 0 ) {
			idou = rc.top - start_ypos;
		}

		rect.top = rc.top - idou;
		rect.bottom = rect.top + ctrl_ht;

		rect.left = rc.left;
		rect.right = rc.right;
		GetDlgItem( kmk_id[i] )->MoveWindow( rect );

		if( sign == 0 ) {
			SetResize( kmk_id[i] );
		}
	}

	for( i = 0; mv_id[i] != 0; i++ ) {
		GetDlgItem( mv_id[i] )->GetWindowRect( rc );
		ScreenToClient( &rc );
		ctrl_ht = rc.Height();

		rect.top = rc.top - idou;
		rect.bottom = rect.top + ctrl_ht;
		rect.left = rc.left;
		rect.right = rc.right;

		GetDlgItem( mv_id[i] )->MoveWindow( rect );
		if( sign == 0 ) {
			SetResize( mv_id[i] );
		}
	}

	//ダイアログ大きさ調整
	GetDlgItem( IDC_BTN_REG )->GetWindowRect( rc );

	GetWindowRect( rect );
	rect.bottom = rc.bottom + rc.Height();

	MoveWindow( rect );
}


//登録番号を表示しないとき。
void CTkyVer2Dlg::InvCtrlWork()
{
	CRect rect, rc;

	// 免税のみ表示
	GetDlgItem(IDC_STATIC18)->ShowWindow(SW_HIDE);
	m_DBEinv.ShowWindow(SW_HIDE);
	m_DBEinv.EnableWindow(FALSE);

	if( m_bInvMaster && m_bInvUse) {
		GetDlgItem(IDC_STATIC18)->GetWindowRect(rect);
		GetDlgItem(IDC_STATIC20)->GetWindowRect(rc);
		int ctrlwd = rc.Width();
		rc.right = rect.right;
		rc.left = rc.right - ctrlwd;
		ScreenToClient(&rc);
		GetDlgItem(IDC_STATIC20)->MoveWindow(rc);

		m_DBEkana.GetWindowRect(rect);
		m_ComboMen.GetWindowRect(rc);
		ctrlwd = rc.Width();
		rc.left = rect.left;
		rc.right = rc.left + ctrlwd;
		ScreenToClient(&rc);
		m_ComboMen.MoveWindow(rc);
		return;
	}
	else {
		GetDlgItem(IDC_STATIC20)->ShowWindow(SW_HIDE);
		m_ComboMen.ShowWindow(SW_HIDE);
		m_ComboMen.EnableWindow(FALSE);
	}

	//摘要カナ　フレームの調整
	GetDlgItem(IDC_STATIC_FRM3)->GetWindowRect(rect);
	ScreenToClient(&rect);
	m_DBEinv.GetWindowRect(rc);
	int mv_ht = rc.Height();
	rect.bottom -= mv_ht;

	GetDlgItem(IDC_STATIC_FRM3)->MoveWindow(rect);

	int ctrl_id[] = {
		IDC_STATIC_G6, IDC_CHK_TKYONLY, IDC_STATIC_GNUM, IDC_STATIC_TKNUM, 0
	};

	int ctrl_ht;

	for( int i = 0; ctrl_id[i] != 0; i++ ) {
		GetDlgItem(ctrl_id[i])->GetWindowRect(rc);
		ScreenToClient(&rc);

		ctrl_ht = rc.Height();

		rect.top = rc.top - mv_ht;
		rect.bottom = rect.top + ctrl_ht;

		rect.left = rc.left;
		rect.right = rc.right;
		GetDlgItem(ctrl_id[i])->MoveWindow(rect);
	}
	//枝番表示しない場合は、EdaCtrlWork
	if( !m_bEdaReg )	return;

	//枝番以下のコントロールを上に上げる
	int* idptr;

	for( int n = 0; n < 3; n++ ) {
		switch( n ) {
		case 0:	idptr = brn_id;	break;
		case 1:	idptr = kmk_id;	break;
		case 2:	idptr = mv_id;	break;
		default: 
			idptr = NULL;
			break;
		}
		if( idptr == NULL )
			break;

		for( int i = 0; idptr[i] != 0; i++ ) {
			GetDlgItem(idptr[i])->GetWindowRect(rc);
			ScreenToClient(&rc);

			ctrl_ht = rc.Height();

			rect.top = rc.top - mv_ht;
			rect.bottom = rect.top + ctrl_ht;

			rect.left = rc.left;
			rect.right = rc.right;
			GetDlgItem(idptr[i])->MoveWindow(rect);
		}
	}
	//ダイアログ大きさ調整
	GetDlgItem(IDC_BTN_REG)->GetWindowRect(rc);

	GetWindowRect(rect);
	rect.bottom = rc.bottom + rc.Height();

	MoveWindow(rect);
}


int CTkyVer2Dlg::ButttonClickByMouseEvent(UINT id)
{
	CPoint nowPt;
	::GetCursorPos(&nowPt);
	CRect rect;
	CWnd* pwnd;
	pwnd = GetDlgItem(id);

//	GetDlgItem(id)->GetWindowRect(&rect);
	int x = (rect.left + rect.Width() / 2);
	int y = (rect.top + rect.Height() / 2);
	//	'実際にマウスを移動させる為の補正値を求める
	//	'画面の解像度を取得
	int smx, smy, posx, posy;
	//	'画面解像度の取得
	smx = GetSystemMetrics(SM_CXSCREEN);
	smy = GetSystemMetrics(SM_CYSCREEN);
	//	'マウスの移動量を計算(絶対位置)
	posx = x * (65535 / smx);
	posy = y * (65535 / smy);
	mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, posx, posy, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	//元の位置に戻す
	posx = nowPt.x * (65535 / smx);
	posy = nowPt.y * (65535 / smy);
	mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, posx, posy, 0, 0);
	return 1;
}



BOOL CTkyVer2Dlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( pMsg->message == WM_KEYDOWN ) {
		int id, vect, term = 1;
		long nChar;

		switch( pMsg->wParam ) {
		case VK_F4:
		case VK_F5:	
		case VK_F6:
		case VK_F7:
		case VK_F8:
		case VK_F9:
		case VK_INSERT: case VK_HOME:
			CheckJob( pMsg->wParam );
			break;
		case VK_F3:
		//	ButttonClickByMouseEvent(IDC_BTN_REG);
			OnBnClickedBtnReg();
			return TRUE;
			break;
		case VK_F12:
			OnOK();
			return TRUE;
			break;

		case VK_RETURN:
		case VK_TAB:
		case VK_DOWN:
		case VK_RIGHT:
			vect = 1;
			if( pMsg->wParam == VK_TAB ) {
				if( GetKeyState( VK_SHIFT ) < 0 ) {
					vect = -1;
				}
			}
			id = ::GetDlgCtrlID( pMsg->hwnd );

			nChar = pMsg->wParam;

			nChar = ComboBoxJob(id, pMsg->wParam);

			if( nChar ) {
				if( CheckTerminationID(id, pMsg->wParam) ) {
					if( TerminationJob(id, vect) )
						return TRUE;
				}
			}
			break;
		case VK_F2:
		case VK_LEFT:
		case VK_UP:
			id = ::GetDlgCtrlID( pMsg->hwnd );
			nChar = ComboBoxJob(id, pMsg->wParam);
			if( nChar ) {
				if( CheckTerminationID(id, pMsg->wParam) ) {
					if( TerminationJob(id, -1) )
						return TRUE;
				}
			}
			break;
		case VK_ESCAPE:
			m_bIgnoreFocus = TRUE;
			EndDialog( IDCANCEL );
			return TRUE;
		case VK_F11:
			PostMessage(WM_COMMAND, MAKELONG(IDC_BTN_TKYSEL, BN_CLICKED),
									(LPARAM)GetDlgItem(IDC_BTN_TKYSEL)->m_hWnd );
			break;
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

// wp はキーコード
long CTkyVer2Dlg::ComboBoxJob(UINT id, WPARAM wp)
{
	long ret = wp;

	if( id == IDC_COMBO_MEN ) {
		CComboBox* pCmb = &m_ComboMen;

		switch( wp ) {
		case VK_DOWN:
			if( !pCmb->GetDroppedState() ) {
				pCmb->ShowDropDown(TRUE);
			}
			ret = 0;
			break;

		case VK_UP:
			ret = 0;
			break;
		case VK_RETURN:
		case VK_RIGHT:
		case VK_F2:
		case VK_LEFT:
			if( pCmb->GetDroppedState() )
				pCmb->ShowDropDown(FALSE);
			break;
		}
	}

	return ret;
}


void CTkyVer2Dlg::CheckJob( WPARAM vkey )
{
	int id = 0;

	switch( vkey ) {
	case VK_F4: id = IDC_CHK_TKYONLY;	break;
	case VK_F5:	id = IDC_CHK_KMKDEB;	break;
	case VK_F6:	id = IDC_CHK_KMKCRE;	break;
	case VK_F7:	id = IDC_CHK_SWKTKY;	break;
	case VK_F8:	id = IDC_CHK_TANDEB;	break;
	case VK_F9:	id = IDC_CHK_TANCRE;	break;

	case VK_INSERT:	id = IDC_CHK_BRNDEB;	break;
	case VK_HOME:	id = IDC_CHK_BRNCRE;	break;
	}

	if( id ) {
		CButton* pb;

		if( ! GetDlgItem( id )->IsWindowEnabled() )
			return;

		pb = (CButton*)GetDlgItem( id );

		int st = pb->GetCheck();
		int chk;

		if( st == BST_CHECKED )	chk = BST_UNCHECKED;
		else					chk = BST_CHECKED;

		pb->SetCheck( chk );

		if( id == IDC_CHK_TKYONLY ) {
			OnBnClickedChkTkyonly();
		}
	}
}



//同じ摘要のチェック
// type = 0: 摘要登録、1 = 枝番時のチェック
int CTkyVer2Dlg::SameTekiyoCheck(char* teki, char* kanabf, char* invno, BYTE menchk, int& tklast, int type )
{
	// 摘要のチェック
	CStringArray	strArray, kanaArray, invArray;
	CByteArray	baDsign4;
	CDWordArray	tkcdArray;

	int	tkcnt, tkcod = 0;

	tklast = 0;
	tkdb_init(m_pZm);
	tkcnt = get_tknum(m_pZm);

	CTKREC* ptk;
	ptk = m_pZm->tkrec;
	ptk->MoveFirst();

	BOOL bTkyOnly;
	CButton* pb = (CButton*)GetDlgItem(IDC_CHK_TKYONLY);
	int st = pb->GetCheck();
	bTkyOnly = (st == BST_CHECKED) ? TRUE : FALSE;

	if( ptk->st != -1 ) {
		do {
			CString strName;
			strName.Format("%.40s", ptk->tkname);

			if( tklast < ptk->tkcod )
				tklast = ptk->tkcod;

			// 摘要名称文字列の完全なマッチングをとる
			if( strcmp(teki, strName) ) continue;

			strArray.Add(strName);
			kanaArray.Add(ptk->tkana);
			tkcdArray.Add(ptk->tkcod);

			if( m_bInvDisp ) {
				invArray.Add(ptk->invno);
			}
			if( m_bInvMaster ) {
				// 免税事業者からの課税仕入れ
				if( ptk->dsign.GetSize() != SIZE_DSIGN ) {
					ptk->dsign.SetSize(SIZE_DSIGN);
					baDsign4.Add(0);
				}
				else {
					baDsign4.Add(ptk->dsign[4]);
				}
			}
		} while( ptk->MoveNext() == 0 );
	}

	if( tkcdArray.GetCount() != 0 ) {
		int kana_inv = 0;
		if( kanaArray[0].Compare(kanabf) != 0 ) {
			kana_inv |= 0x01;
		}
		if( m_bInvDisp ) {
			if( invArray[0].Compare(invno) != 0 ) {
				kana_inv |= 0x02;
			}
		}
		if( m_bInvMaster ) {
			if( menchk ) {
				if( !(baDsign4[0] & 0x10) ) {
					kana_inv |= 0x04;
				}
			}
			else {
				if( (baDsign4[0] & 0x10) ) {
					kana_inv |= 0x04;
				}
			}
		}

		CString title, pbuf, tmp, swork;
		pbuf = _T("");

		tmp.Empty();
		if( (kana_inv & 0x01) == 0x01 ) {
			swork = _T("カナ");
			tmp += swork;
		}
		if( (kana_inv & 0x02) == 0x02 ) {
			if( !tmp.IsEmpty() )
				tmp += "・";
			swork = _T("登録番号");
			tmp += swork;
		}
		if( (kana_inv & 0x04) == 0x04 ) {
			if( !tmp.IsEmpty() )
				tmp += "・";
			swork = _T("免税");
			tmp += swork;
		}

		if( kana_inv ) {
			pbuf.Format(_T("同じ摘要辞書『(%6d) %.40s』が既に登録されていますが、\r\n%sが異なります。\r\n\r\n"), tkcdArray[0], teki, tmp);
			if( type == 0 ) {
				title = _T("摘要辞書登録");
				pbuf += _T("新しく摘要辞書に登録しますか？");
				st = ICSExtBox(title, pbuf, _T("新 規 登 録(&N)"), _T("既存摘要で登録(&K)"), _T("ｷｬﾝｾﾙ(&E)"), MB_DEFBUTTON1, 0, 0, this);
			}
			else {
				title = _T("枝番設定");
				pbuf += _T("設定する摘要コードを選んでください。");
				st = ICSExtBox(title, pbuf, _T("新規摘要コード"), _T("既存摘要コード"), _T("ｷｬﾝｾﾙ"), MB_DEFBUTTON1, 0, 0, this);
			}

			if( st == IDYES )		tkcod = 0;
			else if( st == IDNO ) {
				//既存摘要コード
				tkcod = tkcdArray[0];
				if( type == 0 ) {
					CString filter;
					filter.Format("tkcod = %d", tkcod);

					m_pZm->tkrec->Requery(filter, 0);
					if( m_pZm->tkrec->tkcod == tkcod ) {
						m_pZm->tkrec->Edit();
						m_pZm->tkrec->tkana = kanabf;
						if( m_bInvDisp ) {
							m_pZm->tkrec->invno = invno;
						}
						if( m_bInvMaster ) {
							if( m_pZm->tkrec->dsign.GetSize() != SIZE_DSIGN ) {
								m_pZm->tkrec->dsign.SetSize(SIZE_DSIGN);
							}
							if( menchk ) {
								m_pZm->tkrec->dsign[4] |= 0x10;
							}
							else {
								m_pZm->tkrec->dsign[4] &= ~0x10;
							}
						}
						m_pZm->tkrec->Update();
						m_bDBupdate = TRUE;
						tkquery_flag() = TRUE;
						tkdb_init(m_pZm);
					}
				}
			}
			else	return -1;
		}
		else {
			if( type == 0 ) {
				if( bTkyOnly ) {
					CString msg;
					msg.Format(_T("同じ摘要辞書『%.40s』が既に登録されています。"), teki);
					ICSMessageBox(msg, 0, 0, 0, this);
				}
			}
			else {
				CString msg;
				msg.Format(_T("同じ摘要辞書『(%6d) %.40s』が既に登録されています。\r\n既存摘要コードを使用します。"), tkcdArray[0], teki);
				ICSMessageBox(msg, 0, 0, 0, this);
			}
			tkcod = tkcdArray[0];
		}
	}

	return tkcod;
}




void CTkyVer2Dlg::OnOK()
{
	// 摘要取得
	VARIANT	var;
	char teki[60] = {0};
	char kanabf[24] = {0};

	DBdata_get(this, IDC_ICSDBEDT8_TKY, &var, ICSDBEDT_TYPE_STRING, 0 );
	strcpy_s( teki, sizeof teki, (const char*)var.pbVal );

	if( teki[0] == '\0' ) {
		CString msg;
		msg.Format( _T( "摘要が未入力です！"), teki );
		ICSMessageBox( msg, 0,0,0,this);
	//	EndDialog(IDCANCEL);
		return;
	}

	//インボイス番号チェック
	if( !CheckInvoiceNo() )
		return;


	char pbuf[128];
	CString checkInvno;
	int ncnt;
	if( m_bInvDisp ) {
		ncnt = GetInvnoString(pbuf, sizeof pbuf);
		if( ncnt == 13 ) {
			checkInvno.Format(_T("T%s"), pbuf);
		}
	}

	CString org, kana;
	DBdata_get(this, IDC_ICSDBEDT8_KANA, &var, ICSDBEDT_TYPE_STRING, 0 );
	kana = var.pbVal;
	org = kana;

	kana_chk( &kana, kana.GetLength() );
	var.pbVal = (BYTE*)kana.GetBuffer(16);
	kana.ReleaseBuffer();

	if( org != kana ) {
		DBdata_set(this, IDC_ICSDBEDT8_KANA, &var, ICSDBEDT_TYPE_STRING, 0 );
	}

	strcpy_s( kanabf, sizeof kanabf, (char*)(LPCTSTR)kana );

	// チェックボックスの状態チェック
	int chk[3], st;
	CButton* pb;

	chk[0] = 0;
	pb = (CButton*)GetDlgItem(IDC_CHK_KMKDEB);
	st = pb->GetCheck();
	if( st == BST_CHECKED )	chk[0] |= 0x01;

	pb = (CButton*)GetDlgItem(IDC_CHK_KMKCRE);
	st = pb->GetCheck();
	if( st == BST_CHECKED )	chk[0] |= 0x02;

	chk[1] = 0;
	pb = (CButton*)GetDlgItem(IDC_CHK_SWKTKY);
	st = pb->GetCheck();
	if( st == BST_CHECKED )	chk[1] |= 0x01;

	pb = (CButton*)GetDlgItem(IDC_CHK_TANDEB);
	st = pb->GetCheck();
	if( st == BST_CHECKED )	chk[1] |= 0x02;

	pb = (CButton*)GetDlgItem(IDC_CHK_TANCRE);
	st = pb->GetCheck();
	if( st == BST_CHECKED )	chk[1] |= 0x04;

	//借方枝番
	chk[2] = 0;
	pb = (CButton*)GetDlgItem(IDC_CHK_BRNDEB);
	st = pb->GetCheck();
	if( st == BST_CHECKED )	chk[2] |= 0x01;

	DBdata_get(this, IDC_ICSDBEDT8_DZBRN, &var, ICSDBEDT_TYPE_ARITH, 0 );
	memcpy( m_debZan, var.pbVal, 6 );

	//貸方枝番
	pb = (CButton*)GetDlgItem(IDC_CHK_BRNCRE);
	st = pb->GetCheck();
	if( st == BST_CHECKED )	chk[2] |= 0x02;

	DBdata_get(this, IDC_ICSDBEDT8_CZBRN, &var, ICSDBEDT_TYPE_ARITH, 0 );
	memcpy( m_creZan, var.pbVal, 6 );

	BOOL bTkyOnly;
	pb = (CButton*)GetDlgItem(IDC_CHK_TKYONLY);
	st = pb->GetCheck();
	bTkyOnly = (st == BST_CHECKED) ? TRUE : FALSE;

	// どれかにチェックが必要
	if( ! bTkyOnly && chk[0] == 0 && chk[1] == 0 && chk[2] == 0 ) {
		ICSMessageBox( "登録する摘要項目が選択されていません。", 0,0,0,this);
		return;
	}

	//枝番時
	if( chk[2] ) {
		if( m_debBrn != -1 || m_creBrn != -1 ) {
			if( m_svTky.Compare( teki ) != 0 ) {
				st = ICSMessageBox( "枝番設定後に、摘要の文字が変更されています！\r\n枝番設定の確認をしますか？", MB_YESNO,0,0,this);
				if( st == IDYES )
					return;
			}
		}
	}

	// 摘要のチェック
	int tklast = 0;
	BYTE menchk = 0;
	if( m_bInvMaster ) {
		menchk = m_ComboMen.GetCurSel();
	}
	int tkcod = SameTekiyoCheck(teki, kanabf, (LPSTR)(LPCTSTR)checkInvno, menchk, tklast, 0);
	if( tkcod == -1 )
		return;

#ifdef CLOSE
	CStringArray	strArray, kanaArray;
	CDWordArray	tkcdArray;

	int	tkcnt, tkcod = 0;
	int tklast = 0;
	tkdb_init( m_pZm );
	tkcnt = get_tknum( m_pZm );

	CTKREC* ptk;
	ptk = m_pZm->tkrec;
	ptk->MoveFirst();

	if( ptk->st != -1 ) {
		do {

			CString strName;
			strName.Format( "%.40s", ptk->tkname );

			if( tklast < ptk->tkcod )
				tklast = ptk->tkcod;

			// 摘要名称文字列の完全なマッチングをとる
			if( strcmp( teki, strName )) continue;

			strArray.Add( strName );
			kanaArray.Add( ptk->tkana );

			tkcdArray.Add( ptk->tkcod );

		} while( ptk->MoveNext() == 0 );
	}

	if( tkcdArray.GetCount() != 0 ) {
		if( kanaArray[0].Compare( kanabf ) != 0 ) {

			CString title, pbuf;
			title = _T( "摘要辞書登録" );
			pbuf = _T( "" );
			pbuf.Format( _T( "同じ摘要辞書『%.40s』が既に登録されていますが、\r\nカナが異なります。\r\n\r\n\
新しく摘要辞書に登録しますか？"), teki );

			int st = ICSExtBox( title, pbuf, _T( "新 規 登 録(&N)" ), _T( "既存摘要で登録(&K)" ), _T( "ｷｬﾝｾﾙ(&E)" ), MB_DEFBUTTON1,0,0,this );

			if( st == IDYES )		tkcod = 0;
			else if( st == IDNO ) {
				tkcod = tkcdArray[0];

				CString filter;
				filter.Format( "tkcod = %d", tkcod );

				m_pZm->tkrec->Requery( filter, 0 );
				if( m_pZm->tkrec->tkcod == tkcod ) {
					m_pZm->tkrec->Edit();
					m_pZm->tkrec->tkana = kanabf;
					m_pZm->tkrec->Update();
					m_bDBupdate = TRUE;
					tkquery_flag() = TRUE;
					tkdb_init( m_pZm );
				}
			}
			else	return;

		}
		else {
			if( bTkyOnly ) {
				CString msg;
				msg.Format( _T( "同じ摘要辞書『%.40s』が既に登録されています。"), teki );
				ICSMessageBox( msg, 0,0,0,this);
			}
			tkcod = tkcdArray[0];
		}
	}
#endif

	if( tkcod == 0 ) {
		// 新規に登録
	//	tkcod	=	tkcnt+1;
		tkcod	=	tklast + 1;

		CString filter;
		filter.Format( "tkcod = %d", tkcod );

		m_pZm->tkrec->Requery( filter, 0 );
		if( m_pZm->tkrec->tkcod == tkcod ) {
			tkdb_init( m_pZm );
			m_pZm->tkrec->MoveLast();
			if( m_pZm->tkrec->st == -1 )	tkcod = 1;
			else							tkcod = m_pZm->tkrec->tkcod + 1;
		}

		if( tkcod > 999999 ) {	// 6桁まで入力
			m_pZm->tkrec->MoveLast();
			int tknum = m_pZm->tkrec->tkcod;

			m_pZm->tkrec->MovePrev();
			if( m_pZm->tkrec->st != -1 ) {
				do {
					if( (tknum - m_pZm->tkrec->tkcod) > 1 ) {
						tkcod = (m_pZm->tkrec->tkcod + 1);
						break;
					}
					tknum = m_pZm->tkrec->tkcod;
				} while( m_pZm->tkrec->MovePrev() == 0 );
			}
		}

		m_pZm->tkrec->AddNew();
		m_pZm->tkrec->tkcod = tkcod;
		m_pZm->tkrec->tkname = teki;
		m_pZm->tkrec->tkana = kanabf;
		if( m_bInvDisp ) {
			m_pZm->tkrec->invno = checkInvno;
		}
		if( m_bInvMaster ) {
			if( menchk ) {
				m_pZm->tkrec->dsign.SetSize(SIZE_DSIGN);
				for( int idx = 0; idx < SIZE_DSIGN; idx++ ) {
					m_pZm->tkrec->dsign.SetAt(idx, 0);
				}
				m_pZm->tkrec->dsign[4] = 0x10;
			}
		}
		m_pZm->tkrec->Update();
		m_bDBupdate = TRUE;

		if( tkcod != m_addTKcod ) {
			//他で登録されている番号
			CString msg;
			msg.Format( _T( "摘要『%s』\r\n\r\n番号[%6d]が他で登録されたため、番号[%6d]で登録しました。"), teki, m_addTKcod, tkcod );
			ICSMessageBox( msg, 0,0,0,this);
		}

		m_pZm->tkrec->Requery( "", 0 );
	}

	if( ! bTkyOnly ) {
		// 科目対応摘要に登録
		if( AddKmkTekiyoFunc( tkcod, teki, chk[0] ) != 0 ) {
			EndDialog(-1);
			return;
		}

		// 仕訳対応摘要に登録
		if( AddSiwakeTekiyoFunc( tkcod, teki, chk[1] ) != 0 ) {
			EndDialog(-1);
			return;
		}

		//枝番残高に追加
		if( AddKmkEdabanFunc( tkcod, teki, chk[2] ) != 0 ) {
			EndDialog(-1);
			return;
		}
	}

	ICSDialog::OnOK();
}

// 科目対応摘要 を追加する
//		int	tkcod ..... 摘要コード( 反転はしていない )
//		 char* teki ... 摘要名称
//		int btnchk .... チェックボタンの 状態
//						D0=借方科目, D1=貸方科目
//	--------------------------------------------------------
//		return 0 = 追加成功, -1 = エラー
//
int CTkyVer2Dlg::AddKmkTekiyoFunc( int tkcod, char* teki, int btnchk )
{
	DBKNREC* pKn;
	int ret = 0;
	int bit, st;
	CString msg;

	for( int i = 0; i < 2; i++ ) {

		if( i==0) {
			pKn = m_pDeb;
			bit = 0x01;
		}
		else {
			pKn = m_pCre;
			bit = 0x02;
		}

		if( ! (btnchk & bit) ) continue;

		if( pKn->knrtb > 0 ) {

			if( DoubleRegChk( tkcod, pKn->knrtb ) ) {
				char	dcbf[16];
				strcpy_s( dcbf, sizeof dcbf, (i==0) ? _T("借方") : _T("貸方") );

				msg.Format( "%s 『%-14.14s』の科目対応摘要に\r\n\
指定された摘要 [%6d] %.40s はすでに登録されています！", dcbf, pKn->knnam, tkcod, teki );

				ICSMessageBox( msg, 0,0,0,this);
			}
			else {
				// DoubleRegChk 内で Requery()
				CRTREC* rt;
				rt = m_pZm->rtrec;

#ifdef CLOSE
				rt->MoveLast();
				int seq;
				if( rt->st != -1 )
					seq = rt->rtseq + 1;
				else 
					seq = 1;
#endif
				int seq = 1;
				// 01.17 /11 ---- SEQ 再設定
				rt->MoveFirst();
				if( rt->st != -1 ) {
					do {
						if( rt->rtpgn == pKn->knrtb ) {
							seq = rt->rtseq + 1;
						}
					} while( rt->MoveNext() == 0 );
				}

				st = rt->Append(  pKn->knrtb, seq, tkcod );
				if( st == -1 ) {
					ret = -1 ;
					break;
				}

				rt->Requery( "" );
				m_bDBupdate = TRUE;
			}
		}
	}

	return ret;
}

// 科目対応摘要 二重登録チェック
//	int code ... 摘要ｺｰﾄﾞ
//
//	既に登録されていた場合は TRUE を返送
//
BOOL CTkyVer2Dlg::DoubleRegChk( int tkcod, int rpgno )
{
	BOOL ret = FALSE;

	CString filter;
	filter.Format( "rtpgn = %d", rpgno );

	CRTREC* rt;
	rt = m_pZm->rtrec;

#ifdef CLOSE
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
#endif

	rt->MoveFirst();
	if( rt->st != -1 ) {
		do {
			if( rt->rtpgn == rpgno && rt->rtcod == tkcod ) {
				ret = TRUE;
				break;
			}
		} while( rt->MoveNext() == 0 );
	}

	return ret;
}


//
//	仕訳対応摘要追加処理
//
int CTkyVer2Dlg::AddSiwakeTekiyoFunc( int tkcod, char* teki, int btnchk )
{
	int ret = 0;
	int bit;
	CString str, msg;
	SWKCOD_DATA	sd;
	TKY_VER2*	ptv = get_tkyver2();

	for( int i = 0; i < 3; i++ ) {

		switch( i ) {
		case 0:
			sd.sc_dbt = ptv->t2_debt;
			sd.sc_cre = ptv->t2_cred;
			sd.sc_sgn = sd.sc_chg = 0;
			bit = 0x01;
			break;
		case 1:
			if( m_pDeb && !(m_pDeb->knatt&0x01) ) {
				sd.sc_dbt = ptv->t2_debt;
				sd.sc_cre.Empty();
			}
			else {
				sd.sc_dbt.Empty();
				sd.sc_cre = ptv->t2_debt;
			}
			bit = 0x02;
			break;
		default:
			if( m_pCre && !(m_pCre->knatt&0x01) ) {
				sd.sc_dbt = ptv->t2_cred;
				sd.sc_cre.Empty();
			}
			else {
				sd.sc_dbt.Empty();
				sd.sc_cre = ptv->t2_cred;
			}
			bit = 0x04;
			break;
		}

		if( ! (btnchk & bit) ) continue;

		if( SwkTkyo_DoubleRegChk( tkcod, &sd ) ) {

			switch( i ) {
			case 0:
				msg.Format( "(仕訳) %-14.14s ／ %-14.14s　の仕訳摘要 に\r\n", m_pDeb->knnam, m_pCre->knnam );
				break;
			case 1:
				msg.Format( "(単独) %-14.14s　の摘要 に\r\n", m_pDeb->knnam );
				break;
			default:
				msg.Format( "(単独) %-14.14s　の摘要 に\r\n", m_pCre->knnam );
				break;
			}
			str.Format( "指定された摘要 [%6d] %.40s はすでに登録されています！", tkcod, teki );

			msg += str;

			ICSMessageBox( msg, 0,0,0,this);

		}
		else {
			ret = SwkTkyoAddSub( &sd, tkcod );
			if( ret != 0 )
				break;
		}
	}

	return ret;
}


//
//	仕訳摘要をデータベースに追加する
//
int CTkyVer2Dlg::SwkTkyoAddSub( SWKCOD_DATA* psc, int code )
{
	// データベースにセット
	int  seq, st;
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

	st = m_pZm->strec->Append( (char*)(LPCTSTR)psc->sc_dbt,  (char*)(LPCTSTR)psc->sc_cre, seq, code, 0, sgn );
	m_bDBupdate = TRUE;

	return st;
}


// 仕訳対応摘要 二重登録チェック
//	int code ... 摘要ｺｰﾄﾞ
//
//	既に登録されていた場合は TRUE を返送
//
BOOL CTkyVer2Dlg::SwkTkyo_DoubleRegChk( int code, SWKCOD_DATA* pRec)
{
	BOOL	bDreg = FALSE;
#ifdef CLOSE
	SWKCOD_DATA *pNowRec;

	if( pRec == NULL )	pNowRec = &m_CurData;
	else				pNowRec = pRec;
#endif

	CString filter, tmp;
	// チェック元の摘要コード
	filter_swkdata( &filter, pRec );

	tmp.Format( " AND stcod = %d", code );
	filter += tmp;

	if( m_pZm->strec->Requery( filter ) == 0 ) {
		bDreg = TRUE;
	}

	return bDreg;
}



// 詳細設定で、今までの摘要ダイアログを表示する
void CTkyVer2Dlg::OnBnClickedBtnReg()
{
	if( !CheckInvoiceNo() ) {
		return;
	}
	m_bReg = TRUE;

	TKY_REGPAR* par = get_tkyregpar();

	VARIANT	var;
	char teki[128] = {0};

	DBdata_get(this, IDC_ICSDBEDT8_TKY, &var, ICSDBEDT_TYPE_STRING, 0 );
//	strcpy_s( teki, sizeof teki, (const char*)var.pbVal );
	strcpy_s( teki, sizeof teki, CTkySel::m_orgTky );
	strcpy_s( par->tr_tky, sizeof par->tr_tky, teki );

	if( strlen( teki ) < par->tr_pos )
		par->tr_pos = 0;

	CString org, kana;
	DBdata_get(this, IDC_ICSDBEDT8_KANA, &var, ICSDBEDT_TYPE_STRING, 0 );
	kana = var.pbVal;
	org = kana;

	kana_chk( &kana, kana.GetLength() );
	::ZeroMemory( m_kanabf, sizeof m_kanabf );
	strcpy_s( m_kanabf, sizeof m_kanabf, (char*)(LPCTSTR)kana );

	char pbuf[128] = { 0 };
	int ncnt = GetInvnoString(pbuf, sizeof pbuf);
	CString checkInvno;
	checkInvno.Format(_T("T%s"), pbuf);
	::ZeroMemory(m_TkyInvno, sizeof m_TkyInvno);
	if( ncnt > 0 ) {
		memcpy(m_TkyInvno, (LPSTR)(LPCTSTR)checkInvno, checkInvno.GetLength());
	}

	if( m_bInvMaster ) {
		m_Menchk = m_ComboMen.GetCurSel();
	}
	// 表示する画面を設定
	if( par->tr_tky[0] ) 
		par->tr_jobtyp = TP_DIC;
	else {
		BOOL bDeb, bCre;

		bDeb = GetDlgItem( IDC_CHK_KMKDEB )->IsWindowEnabled();
		bCre = GetDlgItem( IDC_CHK_KMKCRE )->IsWindowEnabled();

		if( bDeb || bCre ) {
			par->tr_jobtyp = TP_KAMOK;
			par->tr_dcsw = (bDeb) ? 0 : 1;
		}
		else {
			par->tr_jobtyp = TP_SIWAKE;
		}
	}

	SaveTkyData( &m_Save );

	EndDialog( IDOK );
}


BEGIN_EVENTSINK_MAP(CTkyVer2Dlg, ICSDialog)
	ON_EVENT(CTkyVer2Dlg, IDC_ICSDBEDT8_TKY, 1, CTkyVer2Dlg::TerminationIcsdbedt8Tky, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CTkyVer2Dlg, IDC_ICSDBEDT8_KANA, 1, CTkyVer2Dlg::TerminationIcsdbedt8Kana, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CTkyVer2Dlg, IDC_ICSDBEDT8_DZBRN, 1, CTkyVer2Dlg::TerminationIcsdbedt8Dzbrn, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CTkyVer2Dlg, IDC_ICSDBEDT8_CZBRN, 1, CTkyVer2Dlg::TerminationIcsdbedt8Czbrn, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CTkyVer2Dlg, IDC_ICSDBEDT_INV, 1, CTkyVer2Dlg::TerminationIcsdbedt8Invno, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CTkyVer2Dlg, IDC_ICSDBEDT_INV, 2 /* KillFocus */, CTkyVer2Dlg::OnKillFocusIcsdbedt8Invno, VTS_I4)
END_EVENTSINK_MAP()

void CTkyVer2Dlg::TerminationIcsdbedt8Tky(long nChar, long inplen, long kst)
{
	VARIANT var;

	if( nChar == VK_DELETE ) {
		var.pbVal = NULL;
		DBdata_set(this, IDC_ICSDBEDT8_TKY, &var, ICSDBEDT_TYPE_STRING, 0 );
	}
	else if( nChar == VK_TAB ) {
		if( ICSDBEDT_KST_SHIFT & kst ) {
			TerminationJob( IDC_ICSDBEDT8_TKY, -1 );
		}
		else {
			TerminationJob( IDC_ICSDBEDT8_TKY, 1 );
		}
	}
	else {
		switch( nChar ) {
		case VK_RETURN :
		case VK_DOWN :
		case VK_RIGHT:
			TerminationJob( IDC_ICSDBEDT8_TKY, 1 );
			break;
		case VK_LEFT :
		case VK_F2 :
			TerminationJob( IDC_ICSDBEDT8_TKY, -1 );
			break;
		}
	}
}


void CTkyVer2Dlg::TerminationIcsdbedt8Kana(long nChar, long inplen, long kst)
{
	VARIANT var;

	if( nChar == VK_DELETE ) {
		var.pbVal = NULL;
		DBdata_set(this, IDC_ICSDBEDT8_KANA, &var, ICSDBEDT_TYPE_STRING, 0 );
	}
	else if( nChar == VK_TAB ) {
		if( ICSDBEDT_KST_SHIFT & kst ) {
			TerminationJob( IDC_ICSDBEDT8_KANA, -1 );
		}
		else {
			TerminationJob( IDC_ICSDBEDT8_KANA, 1 );
		}
	}
	else {
		switch( nChar ) {
		case VK_RETURN :
		case VK_DOWN :
		case VK_RIGHT:
			TerminationJob( IDC_ICSDBEDT8_KANA, 1 );
			break;
		case VK_UP:
		case VK_LEFT :
		case VK_F2 :
			TerminationJob( IDC_ICSDBEDT8_KANA, -1 );
			break;
		}
	}
}

//借方 枝番 開始残
void CTkyVer2Dlg::TerminationIcsdbedt8Dzbrn(long nChar, long inplen, long kst)
{
	VARIANT var;

	if( nChar == VK_DELETE ) {
		::ZeroMemory( m_debZan, sizeof m_debZan );
		var.pbVal = (BYTE*)m_debZan;
		DBdata_set(this, IDC_ICSDBEDT8_DZBRN, &var, ICSDBEDT_TYPE_ARITH, 0 );
	}
	else if( nChar == VK_TAB ) {
		if( ICSDBEDT_KST_SHIFT & kst ) {
			TerminationJob( IDC_ICSDBEDT8_DZBRN, -1 );
		}
		else {
			TerminationJob( IDC_ICSDBEDT8_DZBRN, 1 );
		}
	}
	else {
		switch( nChar ) {
		case VK_RETURN :
		case VK_DOWN :
		case VK_RIGHT:
			TerminationJob( IDC_ICSDBEDT8_DZBRN, 1 );
			break;
		case VK_UP:
		case VK_LEFT :
		case VK_F2 :
			TerminationJob( IDC_ICSDBEDT8_DZBRN, -1 );
			break;
		}
	}
}

//貸方 枝番 開始残
void CTkyVer2Dlg::TerminationIcsdbedt8Czbrn(long nChar, long inplen, long kst)
{
	VARIANT var;

	if( nChar == VK_DELETE ) {
		::ZeroMemory( m_creZan, sizeof m_creZan );
		var.pbVal = (BYTE*)m_creZan;
		DBdata_set(this, IDC_ICSDBEDT8_CZBRN, &var, ICSDBEDT_TYPE_ARITH, 0 );
	}
	else if( nChar == VK_TAB ) {
		if( ICSDBEDT_KST_SHIFT & kst ) {
			TerminationJob( IDC_ICSDBEDT8_CZBRN, -1 );
		}
		else {
			TerminationJob( IDC_ICSDBEDT8_CZBRN, 1 );
		}
	}
	else {
		switch( nChar ) {
		case VK_RETURN :
		case VK_DOWN :
		case VK_RIGHT:
			TerminationJob( IDC_ICSDBEDT8_CZBRN, 1 );
			break;
		case VK_UP:
		case VK_LEFT :
		case VK_F2 :
			TerminationJob( IDC_ICSDBEDT8_CZBRN, -1 );
			break;
		}
	}
}


// フォーカスセット
void CTkyVer2Dlg::set_focus( int id )
{
	PostMessage( WM_RETROFOCUS, id );
}

// フォーカスセット
LRESULT CTkyVer2Dlg::OnRetroFocus(WPARAM wParam, LPARAM lParam)
{
	c_set( wParam );

	return 1;
}

// 標準のコントロールのIDか
int CTkyVer2Dlg::CheckTerminationID( int id, long nChar )
{
	int ret = 0;

	switch( id ) {
	case IDC_ICSDBEDT8_TKY:
	case IDC_ICSDBEDT8_KANA:
	case IDC_ICSDBEDT_INV:
	case 0:
		break;
	default:
		ret = 1;
		break;
	}
	//
	if( nChar == VK_RETURN ) {
		switch(id) {
		case IDC_BTN_REG:
		case IDOK:
		case IDCANCEL:
		case IDC_BRNDEB_BTN:
		case IDC_BRNCRE_BTN:
			ret = 0;
			break;
		}
	}

	return ret;
}

//	キー移動処理
//		int nID		移動元のコントロール
//		int vect	> 0 -> Next, < 0 -> Prev 
//
int CTkyVer2Dlg::TerminationJob( int nID, int vect )
{
	int idtbl[] = {
		IDC_ICSDBEDT8_TKY, IDC_ICSDBEDT8_KANA,IDC_ICSDBEDT_INV,IDC_COMBO_MEN,
		IDC_CHK_TKYONLY,
		IDC_CHK_BRNDEB, IDC_ICSDBEDT8_DZBRN, IDC_BRNDEB_BTN,
		IDC_CHK_BRNCRE, IDC_ICSDBEDT8_CZBRN, IDC_BRNCRE_BTN,
		IDC_CHK_KMKDEB, IDC_CHK_KMKCRE, 
		IDC_CHK_SWKTKY, IDC_CHK_TANDEB, IDC_CHK_TANCRE, 
		/*IDC_BTN_REG,*/ IDOK, IDCANCEL
	};

	int	idsz = sizeof idtbl / sizeof idtbl[0];
	int	move_id = 0;
	int loop = 1;
	int	repeat = 0;

	do {

		for( int i = 0; i < idsz; i++ ) {
			if( nID == idtbl[i] ) {
				if( vect < 0 ) {
					if( i > 0 )	move_id = idtbl[(i-1)];
				}
				else {
					if( (i+1) < idsz )	move_id = idtbl[(i+1)];
					else {
						if( repeat == 0 ) {
							repeat++;
							move_id = IDC_ICSDBEDT8_TKY;
						}
					}
				}
				break;
			}
		}

		if( move_id ) {
			if( ! GetDlgItem( move_id )->IsWindowEnabled() ) {
				nID = move_id;
				move_id = 0;
			}
			else {
				loop = 0;
			}
		}
		else {
			loop = 0;
		}

	} while(loop);

	int ret = 0;

	if( move_id ) {
		set_focus( move_id );
		ret = 1;
	}

	return ret;
}

//摘要のみ登録の場合、他のチェックボックスの処理
void CTkyVer2Dlg::OnBnClickedChkTkyonly()
{
	BOOL bEnable;
	CButton* pb;

	pb = (CButton*)GetDlgItem(IDC_CHK_TKYONLY);
	int st = pb->GetCheck();
	if( st == BST_CHECKED )	bEnable = FALSE;
	else					bEnable = TRUE;

	BOOL bDeb, bCre;
	bDeb = bCre = bEnable;

	if( m_pDeb == NULL || m_pDeb->knrtb == 0 ) {
		bDeb = FALSE;
	}
	if( m_pCre == NULL || m_pCre->knrtb == 0 ) {
		bCre = FALSE;
	}

	if( m_bEdaMast) {
		GetDlgItem( IDC_CHK_BRNDEB )->EnableWindow( bDeb );
		GetDlgItem( IDC_STATIC_G7 )->EnableWindow( bDeb );
		GetDlgItem( IDC_BRNDEB_BTN )->EnableWindow( bDeb );
		GetDlgItem( IDC_STATIC16 )->EnableWindow( bDeb );
		m_DZbrn.EnableWindow( bDeb );

		GetDlgItem( IDC_CHK_BRNCRE )->EnableWindow( bCre );
		GetDlgItem( IDC_STATIC_G8 )->EnableWindow( bCre );
		GetDlgItem( IDC_BRNCRE_BTN )->EnableWindow( bCre );
		GetDlgItem( IDC_STATIC17 )->EnableWindow( bCre );
		m_CZbrn.EnableWindow( bCre );
	}
	else {
		GetDlgItem( IDC_CHK_BRNDEB )->EnableWindow( FALSE );
		GetDlgItem( IDC_STATIC_G7 )->EnableWindow( FALSE );
		GetDlgItem( IDC_BRNDEB_BTN )->EnableWindow( FALSE );
		GetDlgItem( IDC_STATIC16 )->EnableWindow( FALSE );
		m_DZbrn.EnableWindow( FALSE );

		GetDlgItem( IDC_CHK_BRNCRE )->EnableWindow( FALSE );
		GetDlgItem( IDC_STATIC_G8 )->EnableWindow( FALSE );
		GetDlgItem( IDC_BRNCRE_BTN )->EnableWindow( FALSE );
		GetDlgItem( IDC_STATIC17 )->EnableWindow( FALSE );
		m_CZbrn.EnableWindow( FALSE );
	}

	GetDlgItem( IDC_CHK_KMKDEB )->EnableWindow( bDeb );
	GetDlgItem( IDC_STATIC_G1 )->EnableWindow( bDeb );

	GetDlgItem( IDC_CHK_KMKCRE )->EnableWindow( bCre );
	GetDlgItem( IDC_STATIC_G2 )->EnableWindow( bCre );

	if( m_pDeb == NULL && m_pCre == NULL ) {
		GetDlgItem( IDC_CHK_SWKTKY )->EnableWindow( FALSE );
		GetDlgItem( IDC_STATIC_G3 )->EnableWindow( FALSE );
	}
	else {
		GetDlgItem( IDC_CHK_SWKTKY )->EnableWindow( bEnable );
		GetDlgItem( IDC_STATIC_G3 )->EnableWindow( bEnable );
	}

	GetDlgItem( IDC_CHK_TANDEB )->EnableWindow( m_pDeb == NULL ? FALSE : bEnable );
	GetDlgItem( IDC_STATIC_G4 )->EnableWindow( m_pDeb == NULL ? FALSE : bEnable );

	GetDlgItem( IDC_CHK_TANCRE )->EnableWindow( m_pCre == NULL ? FALSE :  bEnable );
	GetDlgItem( IDC_STATIC_G5 )->EnableWindow( m_pCre == NULL ? FALSE : bEnable );

	GetDlgItem( IDC_STATIC_FRM )->EnableWindow( bEnable );
	GetDlgItem( IDC_STATIC_FRM2 )->EnableWindow( bEnable );

//	if( bEnable == FALSE ) {
		CWnd* pwnd = GetFocus();
		int id = 0;
		if( pwnd )	id = pwnd->GetDlgCtrlID();
		if( id != IDC_CHK_TKYONLY ) {
			set_focus( IDC_CHK_TKYONLY );
		}
//	}
}


// 科目枝番 を追加する
//		int	tkcod ..... 摘要コード( 反転はしていない )
//		char* teki .... 摘要名称
//		int btnchk .... チェックボタンの 状態
//						D0=借方科目, D1=貸方科目
//	--------------------------------------------------------
//		return 0 = 追加成功, -1 = エラー
//
int CTkyVer2Dlg::AddKmkEdabanFunc( int tkcod, char* teki, int btnchk )
{
	DBKNREC* pKn;
	int ret = 0;
	int bit, brn, st;
	CString msg;
	char edabf[64] = {0};

//	m_debBrn = m_creBrn = -1;

	for( int i = 0; i < 2; i++ ) {

		if( i==0) {
			pKn = m_pDeb;
			bit = 0x01;
		}
		else {
			pKn = m_pCre;
			bit = 0x02;
		}

		if( ! (btnchk & bit) ) continue;

		st = EdabanDoubleRegChk( i, tkcod, brn, pKn, teki );

		if( st > 0 ) {
			//DB更新
			m_bDBupdate = TRUE;
		}
		else if( st < 0 ) {
			//設定可能な枝番なし。(桁数分登録済み)
			brn = -1;
		}
		else {
			//枝番登録済み
			char	dcbf[16];
			strcpy_s( dcbf, sizeof dcbf, (i==0) ? _T("借方") : _T("貸方") );

			set_codestr( edabf, sizeof edabf, brn, m_pZm->zvol->edcol );

			msg.Format( "%s 『%-14.14s』の枝番[%s]に\r\n\
指定された摘要 【[%6d] %.40s】 はすでに登録されています。\r\nそのため、枝番開始残高の更新は行いません。", dcbf, pKn->knnam, edabf, tkcod, teki, edabf );

			ICSMessageBox( msg, 0,0,0,this);
		}

		//セットする枝番
		if( i==0) {
			m_debBrn = brn;
		}
		else {
			m_creBrn = brn;
		}
	}

	return ret;
}

//
// 枝番名称入力と一致するものを検索
//
int CTkyVer2Dlg::EdaNameInpSearch( LPCTSTR kcod, char* teki )
{
	CEZREC* pez;
	pez = m_pZm->ezrec;

	int eda = -1;
	int j = 0;
	int cnt = pez->Find( kcod );

	if( cnt > 0 ) {
		do
		{
			if( pez->ezname.Compare( teki ) == 0 ) {
				eda = pez->ezecd;
				break;
			}
		} while( ++j < cnt && pez->MoveNext() == 0 );
	}

	return eda;
}

//	枝番の登録チェック
//	return	0 = 指定科目の指定摘要コードの枝番がある。
//			1 = 指定摘要コードの枝番があるため、新枝番を作成
//			2 = 枝番コードが、指定摘要コード
//			-----------------------------------------
//			設定ボタンを押した
//			3 = 枝番がすでに登録済み--摘要コードを更新
//			4 = 枝番を新規登録
//
int CTkyVer2Dlg::EdabanDoubleRegChk( int type, int tkcod, int& brn, DBKNREC* pKn, char* teki )
{
	//摘要コードで、枝番を検索する。
	if( ! m_bEZquery ) {
		m_bEZquery = TRUE;
		m_pZm->ezrec->Requery( "", 1 );
	}
	int ret;

	CEZREC* pez;
	pez = m_pZm->ezrec;

	CString kcod;
	DWORD tmp;
	char	bf[64];

	tmp = pKn->kncod;
	hl_rev( &tmp, 4 );
	kcod.Format( "%08x", tmp );

	int brnNo;
	BOOL bAuto = FALSE;
	CString msg;
	char	dcbf[32], edabf[32];

	if( type == 0 ) {
		if( m_debBrn != -1 ) {	//設定あり
			brnNo = m_debBrn;
		}
		else {
			//自動
			bAuto = TRUE;
		}
	}
	else {
		if( m_creBrn != -1 ) {	//設定あり
			brnNo = m_creBrn;
		}
		else {
			//自動
			bAuto = TRUE;
		}
	}

	if( bAuto ) {
		// 指定科目の指定摘要コードで登録済み
		if( pez->Find2( kcod, tkcod ) > 0 ) {
			//自動で、既存枝番のときは、開始残高を登録しない。
			brn = pez->ezecd;
			ret = 0;
		}
		else {
			//枝番名称 手入力の場合
			if( (brn = EdaNameInpSearch( kcod, teki )) > 0 ) {
				set_codestr( edabf, sizeof edabf, brn, m_pZm->zvol->edcol );
				strcpy_s( dcbf, sizeof dcbf, (type==0) ? _T("借方") : _T("貸方") );
				msg.Format( "%s 『%-14.14s』の枝番[%s]に\r\n枝番名称【%s】が設定されています。\
この枝番を使用します。", dcbf, pKn->knnam, edabf, teki );
				ICSMessageBox( msg, 0,0,0,this );

				//摘要番号・残高を更新する
				pez->Edit();

				pez->eztcd	= tkcod;
				l_print( bf, ( type == 0 ) ? m_debZan : m_creZan, FMT12Z );
				pez->ezzan	= bf;

				pez->Update();
				pez->Requery("", 1);
				ret = 5;
				return ret;
			}

			// 枝番がすでに登録済み
			if( pez->Find( kcod, tkcod ) > 0 ) {
				//次の新枝番をサーチ
				brn = NewEdabanSearch( m_pZm, kcod, tkcod );
				if( brn == -1 ) {
					strcpy_s( dcbf, sizeof dcbf, (type==0) ? _T("借方") : _T("貸方") );
					msg.Format( "%s 『%-14.14s』に\r\n設定可能な枝番がありませんでした。", dcbf, pKn->knnam );

					ICSMessageBox( msg, 0,0,0,this );
					ret = -1;
				}
				else {
					ret = 1;
				}
			}
			else {
				//桁数チェックを行う
				brn = NewEdabanSearch( m_pZm, kcod, tkcod, 1 );
				if( brn == -1 ) {
					strcpy_s( dcbf, sizeof dcbf, (type==0) ? _T("借方") : _T("貸方") );
					msg.Format( "%s 『%-14.14s』に\r\n設定可能な枝番がありませんでした。", dcbf, pKn->knnam );
					ICSMessageBox( msg, 0,0,0,this );
					ret = -1;
				}
				else {
					if( tkcod != brn )	ret = 1;
					else				ret = 2;
				}
			}

			// DBに追加
			if( ret > 0 ) {
				pez->AddNew();

				pez->ezkcd	= kcod;
				pez->ezecd	= brn;
				pez->eztcd	= tkcod;
				pez->ezatt	= pKn->knatt;

				l_print( bf, ( type == 0 ) ? m_debZan : m_creZan, FMT12Z );
				pez->ezzan	= bf;

				pez->Update();
				pez->Requery("", 1);
			}
		}
	}
	else {
		brn = brnNo;

		// 枝番がすでに登録済み
		if( pez->Find( kcod, brn ) > 0 ) {
			//摘要番号・残高を更新する
			pez->Edit();

			pez->eztcd	= tkcod;
			l_print( bf, ( type == 0 ) ? m_debZan : m_creZan, FMT12Z );
			pez->ezzan	= bf;

			pez->Update();
			pez->Requery("", 1);
			ret = 3;
		}
		else {
			//新規追加
			pez->AddNew();

			pez->ezkcd	= kcod;
			pez->ezecd	= brn;
			pez->eztcd	= tkcod;
			pez->ezatt	= pKn->knatt;
			l_print( bf, ( type == 0 ) ? m_debZan : m_creZan, FMT12Z );
			pez->ezzan	= bf;

			pez->Update();
			pez->Requery("", 1);
			ret = 4;
		}
	}

	return ret;
}



//借方枝番設定
void CTkyVer2Dlg::OnBnClickedBrndebBtn()
{
	int tkcod;

	if( eda_seteicheck( 0, tkcod) == 0 )
		return;

	CTkyEdaDlg	dlg(this);
	dlg.Set_Class( m_pZm, m_pSy, m_pDeb, tkcod );
	dlg.Set_Info( m_svTky, m_svKana, m_debBrn, m_debZan );

	if( dlg.DoModal() == IDOK ) {
		CButton* pb;
		pb = (CButton*)GetDlgItem(IDC_CHK_BRNDEB);
		pb->SetCheck(BST_CHECKED);

		//残高部分更新
		dlg.GetTkyEdaData(m_debBrn, m_debZan);

		VARIANT var;
		var.pbVal = (BYTE*)m_debZan;
		DBdata_set(this, IDC_ICSDBEDT8_DZBRN, &var, ICSDBEDT_TYPE_ARITH, 0 );
	}
}

//貸方枝番設定
void CTkyVer2Dlg::OnBnClickedBrncreBtn()
{
	int tkcod;

	if( eda_seteicheck( 1, tkcod) == 0 )
		return;

	CTkyEdaDlg	dlg(this);
	dlg.Set_Class( m_pZm, m_pSy, m_pCre, tkcod );
	dlg.Set_Info( m_svTky, m_svKana, m_creBrn, m_creZan );

	if( dlg.DoModal() == IDOK ) {
		CButton* pb;
		pb = (CButton*)GetDlgItem(IDC_CHK_BRNCRE);
		pb->SetCheck(BST_CHECKED);

		//残高部分更新
		dlg.GetTkyEdaData(m_creBrn, m_creZan);
		VARIANT var;

		var.pbVal = (BYTE*)m_creZan;
		DBdata_set(this, IDC_ICSDBEDT8_CZBRN, &var, ICSDBEDT_TYPE_ARITH, 0 );
	}
}


//
//	枝番設定ボタンが押されたときのチェック
//	type	0 = 借方科目
//			1 = 貸方科目
//
//	return 0 --- キャンセル等 処理しない。
//
int CTkyVer2Dlg::eda_seteicheck( int type, int& tkcod )
{
	CButton* pb;
	int st;

	pb = (CButton*)GetDlgItem(IDC_CHK_TKYONLY);
	st = pb->GetCheck();
	if( st == BST_CHECKED )
		return 0;

	// 摘要取得
	VARIANT	var;
	char teki[60] = {0};
	char kanabf[24] = {0};

	DBdata_get(this, IDC_ICSDBEDT8_TKY, &var, ICSDBEDT_TYPE_STRING, 0 );
	strcpy_s( teki, sizeof teki, (const char*)var.pbVal );

	if( teki[0] == '\0' ) {
		CString msg;
		msg.Format( _T( "摘要が未入力です！"), teki );
		ICSMessageBox( msg, 0,0,0,this);
		return 0;
	}

	CString org, kana;
	DBdata_get(this, IDC_ICSDBEDT8_KANA, &var, ICSDBEDT_TYPE_STRING, 0 );
	kana = var.pbVal;
	org = kana;

	kana_chk( &kana, kana.GetLength() );
	var.pbVal = (BYTE*)kana.GetBuffer(16);
	kana.ReleaseBuffer();

	if( org != kana ) {
		DBdata_set(this, IDC_ICSDBEDT8_KANA, &var, ICSDBEDT_TYPE_STRING, 0 );
	}

	strcpy_s( kanabf, sizeof kanabf, (char*)(LPCTSTR)kana );

	// チェックボックスにチェックをつける
	//借方枝番
	if( type == 0) {
		DBdata_get(this, IDC_ICSDBEDT8_DZBRN, &var, ICSDBEDT_TYPE_ARITH, 0 );
		memcpy( m_debZan, var.pbVal, 6 );
	}
	else {
		//貸方枝番
		DBdata_get(this, IDC_ICSDBEDT8_CZBRN, &var, ICSDBEDT_TYPE_ARITH, 0 );
		memcpy( m_creZan, var.pbVal, 6 );
	}

	// 摘要のチェック
	char pbuf[128];
	int ncnt;
	ncnt = GetInvnoString(pbuf, sizeof pbuf);
	CString checkInvno;
	if( ncnt == 13 ) {
		checkInvno.Format(_T("T%s"), pbuf);
	}

	BYTE menchk = 0;
	if( m_bInvMaster ) {
		menchk = m_ComboMen.GetCurSel();
	}
	int tklast = 0;
	tkcod = SameTekiyoCheck(teki, kanabf, (LPSTR)(LPCTSTR)checkInvno, menchk, tklast, 1);
	if( tkcod == -1 ) {
		return 0;
	}


#ifdef CLOSE
	CStringArray	strArray, kanaArray;
	CDWordArray	tkcdArray;

	int	tkcnt;
	tkdb_init( m_pZm );
	tkcnt = get_tknum( m_pZm );

	CTKREC* ptk;
	ptk = m_pZm->tkrec;
	ptk->MoveFirst();

	if( ptk->st != -1 ) {
		do {

			CString strName;
			strName.Format( "%.40s", ptk->tkname );

			if( tklast < ptk->tkcod )
				tklast = ptk->tkcod;

			// 摘要名称文字列の完全なマッチングをとる
			if( strcmp( teki, strName )) continue;

			strArray.Add( strName );
			kanaArray.Add( ptk->tkana );

			tkcdArray.Add( ptk->tkcod );

		} while( ptk->MoveNext() == 0 );
	}

	if( tkcdArray.GetCount() != 0 ) {
		if( kanaArray[0].Compare( kanabf ) != 0 ) {

			CString title, pbuf;
			title = _T( "枝番設定" );
			pbuf = _T( "" );
			pbuf.Format( _T( "同じ摘要辞書『(%6d) %.40s』が既に登録されていますが、\r\nカナが異なります。\r\n\r\n\
設定する摘要コードを選んでください。"), tkcdArray[0], teki );

			int st = ICSExtBox( title, pbuf, _T( "新規摘要コード" ), _T( "既存摘要コード" ), _T( "ｷｬﾝｾﾙ" ), MB_DEFBUTTON1,0,0,this );

			if( st == IDYES )		tkcod = 0;
			else if( st == IDNO )	tkcod = tkcdArray[0];
			else	return 0;

		}
		else {
			CString msg;
			msg.Format( _T( "同じ摘要辞書『(%6d) %.40s』が既に登録されています。\r\n既存摘要コードを使用します。"), tkcdArray[0], teki );
			ICSMessageBox( msg, 0,0,0,this);

			tkcod = tkcdArray[0];
		}
	}
	else {
		//新規登録
		tkcod = 0;
	}
#endif

	//新規に摘要を追加
	if( tkcod == 0 ) {
		tkcod = tklast + 1;
		if( tkcod > 999999 ) {	// 6桁まで入力
			m_pZm->tkrec->MoveLast();
			int tknum = m_pZm->tkrec->tkcod;

			m_pZm->tkrec->MovePrev();
			if( m_pZm->tkrec->st != -1 ) {
				do {

					if( (tknum - m_pZm->tkrec->tkcod) > 1 ) {
						tkcod = (m_pZm->tkrec->tkcod + 1);
						break;
					}
					tknum = m_pZm->tkrec->tkcod;

				} while( m_pZm->tkrec->MovePrev() == 0 );
			}
		}
	}

	//摘要が変更された場合のチェックのために、摘要とカナを保存する。
	m_svTky = teki;
	m_svKana = kanabf;

	return 1;
}

//
//	枝番取得
//
void CTkyVer2Dlg::GetRegEdaban( int& dbr, int& cbr )
{
	dbr = m_debBrn;
	cbr = m_creBrn;
}


// 登録内容の保存
void CTkyVer2Dlg::SaveTkyData( TKYV2_SAVE* psv )
{
	clear_TKYV2_SAVE(psv);

	// 摘要取得
	VARIANT	var;
	char teki[60] = {0};
	char kanabf[24] = {0};

	DBdata_get(this, IDC_ICSDBEDT8_TKY, &var, ICSDBEDT_TYPE_STRING, 0 );
	strcpy_s( psv->tky, sizeof  psv->tky, (const char*)var.pbVal );

	CString org, kana;
	DBdata_get(this, IDC_ICSDBEDT8_KANA, &var, ICSDBEDT_TYPE_STRING, 0 );
	kana = var.pbVal;
	org = kana;

	kana_chk( &kana, kana.GetLength() );
	strcpy_s( psv->kana, sizeof psv->kana, (char*)(LPCTSTR)kana );

	//借方枝番
	DBdata_get(this, IDC_ICSDBEDT8_DZBRN, &var, ICSDBEDT_TYPE_ARITH, 0 );
	memcpy( m_debZan, var.pbVal, 6 );

	//貸方枝番
	DBdata_get(this, IDC_ICSDBEDT8_CZBRN, &var, ICSDBEDT_TYPE_ARITH, 0 );
	memcpy( m_creZan, var.pbVal, 6 );


	psv->debBrn = m_debBrn;
	psv->creBrn = m_creBrn;
	memcpy( psv->dZan, m_debZan, 6 );
	memcpy( psv->cZan, m_creZan, 6 );

	//チェックボックス状態
	int st;
	CButton* pb;

	//摘要のみ登録
	psv->chkTky = 0;
	pb = (CButton*)GetDlgItem(IDC_CHK_TKYONLY);
	st = pb->GetCheck();
	if( st == BST_CHECKED )	psv->chkTky = 1;

	psv->chk[0] = 0;
	pb = (CButton*)GetDlgItem(IDC_CHK_KMKDEB);
	st = pb->GetCheck();
	if( st == BST_CHECKED )	psv->chk[0] |= 0x01;

	pb = (CButton*)GetDlgItem(IDC_CHK_KMKCRE);
	st = pb->GetCheck();
	if( st == BST_CHECKED )	psv->chk[0] |= 0x02;

	psv->chk[1] = 0;
	pb = (CButton*)GetDlgItem(IDC_CHK_SWKTKY);
	st = pb->GetCheck();
	if( st == BST_CHECKED )	psv->chk[1] |= 0x01;

	pb = (CButton*)GetDlgItem(IDC_CHK_TANDEB);
	st = pb->GetCheck();
	if( st == BST_CHECKED )	psv->chk[1] |= 0x02;

	pb = (CButton*)GetDlgItem(IDC_CHK_TANCRE);
	st = pb->GetCheck();
	if( st == BST_CHECKED )	psv->chk[1] |= 0x04;

	//借方枝番
	psv->chk[2] = 0;
	pb = (CButton*)GetDlgItem(IDC_CHK_BRNDEB);
	st = pb->GetCheck();
	if( st == BST_CHECKED )	psv->chk[2] |= 0x01;

	//貸方枝番
	pb = (CButton*)GetDlgItem(IDC_CHK_BRNCRE);
	st = pb->GetCheck();
	if( st == BST_CHECKED )	psv->chk[2] |= 0x02;

	strcpy_s( psv->sv_tky, sizeof psv->sv_tky, m_svTky );
	strcpy_s( psv->sv_kana, sizeof psv->sv_kana, m_svKana );

	strcpy_s( psv->sv_seltky, sizeof psv->sv_seltky, m_svSelTky );
	memcpy( psv->sv_invno, m_TkyInvno, sizeof psv->sv_invno);
	psv->sv_menchk = m_Menchk;
}

//前回 登録内容を保存
void CTkyVer2Dlg::GetTkyData( TKYV2_SAVE* psv )
{
	memcpy( psv, &m_Save, sizeof TKYV2_SAVE );

}

// 前回 登録内容を保存
void CTkyVer2Dlg::SetSaveTkyData( TKYV2_SAVE* psv )
{
	m_SvMode = TRUE;
	memcpy( &m_Save, psv, sizeof TKYV2_SAVE );
	m_svSelTky = psv->sv_seltky;
}

//
void CTkyVer2Dlg::SetCheckFromSave()
{
	CButton* pb;

	//摘要のみ登録
	pb = (CButton*)GetDlgItem(IDC_CHK_TKYONLY);
	if( m_Save.chkTky )		pb->SetCheck( BST_CHECKED );

	//科目対応摘要
	pb = (CButton*)GetDlgItem(IDC_CHK_KMKDEB);
	if( m_Save.chk[0] & 0x01 )		pb->SetCheck( BST_CHECKED );

	pb = (CButton*)GetDlgItem(IDC_CHK_KMKCRE);
	if( m_Save.chk[0] & 0x02 )		pb->SetCheck( BST_CHECKED );

	//仕訳対応摘要
	pb = (CButton*)GetDlgItem(IDC_CHK_SWKTKY);
	if( m_Save.chk[1] & 0x01 )		pb->SetCheck( BST_CHECKED );

	pb = (CButton*)GetDlgItem(IDC_CHK_TANDEB);
	if( m_Save.chk[1] & 0x02 )		pb->SetCheck( BST_CHECKED );

	pb = (CButton*)GetDlgItem(IDC_CHK_TANCRE);
	if( m_Save.chk[1] & 0x04 )		pb->SetCheck( BST_CHECKED );

	//借方枝番
	pb = (CButton*)GetDlgItem(IDC_CHK_BRNDEB);
	if( m_Save.chk[2] & 0x01 )		pb->SetCheck( BST_CHECKED );

	//貸方枝番
	pb = (CButton*)GetDlgItem(IDC_CHK_BRNCRE);
	if( m_Save.chk[2] & 0x02 )		pb->SetCheck( BST_CHECKED );

	if( m_Save.chkTky ) {
		OnBnClickedChkTkyonly();
	}
}


// 摘要選択
void CTkyVer2Dlg::OnBnClickedBtnTkysel()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	if (CTkySel::m_orgTky.GetLength() <= 0) {
		return;
	}
	CTkySel	dlg(this);
	dlg.m_TKYO_LEN = m_TKYO_LEN;

	if( !CheckInvoiceNo() ) {
		return;
	}

	if( dlg.DoModal() == IDOK ) {

		VARIANT var;
		var.pbVal = (BYTE*)dlg.m_Teki;
		m_svSelTky = dlg.m_Teki;

		DBdata_set( this, IDC_ICSDBEDT8_TKY, &var, ICSDBEDT_TYPE_STRING, 0 );

		CRevConv rv;
		DWORD opt;
		opt = RVF_KATAKANA | RVF_HANKAKU;
		CString kana;

		if( rv.GetFurigana( opt, CString(dlg.m_Teki), kana ) == 0 ) {
			kana_chk( &kana, kana.GetLength() );
			furikana_chk( m_kanabf, sizeof m_kanabf, kana );
			var.pbVal = (BYTE*)m_kanabf;
			DBdata_set(this, IDC_ICSDBEDT8_KANA, &var, ICSDBEDT_TYPE_STRING, 0 );
		}
	}
}


//インボイス登録番号取得
int CTkyVer2Dlg::GetInvnoString(char* pbuf, int szpbuf)
{
	int ncnt = 0;
	VARIANT var;

	pbuf[0] = '\0';
	DBdata_get(this, IDC_ICSDBEDT_INV, &var, ICSDBEDT_TYPE_STRING, 0);
	sprintf_s(pbuf, szpbuf, _T("%s"), (char*)var.pbVal);
	ncnt = 0;
	for( int i = 0; pbuf[i] != '\0'; i++ ) {
		if( isdigit(pbuf[i]) ) {
			ncnt++;
			continue;
		}
		else {
			pbuf[i] = '\0';
			break;
		}
	}

	return ncnt;
}


//インボイス番号
void CTkyVer2Dlg::TerminationIcsdbedt8Invno(long nChar, long inplen, long kst)
{
	VARIANT var;
	CString strWork, edtInvno;
	char pbuf[128] = {0};
	int ncnt;

	switch( nChar ) {
	case NULL:
	case VK_RIGHT:
	case VK_RETURN:
	case VK_TAB:
	case VK_DOWN:
	case VK_F2:
	case VK_UP:
	case VK_LEFT:
	case VK_INSERT:
	case 0x16:	//貼り付け
		if( inplen == 0 ) { // 貼り付けの場合はinplenが0になる
			strWork.Format("%s", m_TkyInvno);
			strWork.Replace("T", "");
			DBdata_get(this, IDC_ICSDBEDT_INV, &var, ICSDBEDT_TYPE_STRING, 0);

			edtInvno = var.pbVal;
			if( strWork.CompareNoCase(edtInvno) ) {
				inplen = 1;
			}
		}

		if( inplen > 0 ) {
			ncnt = GetInvnoString(pbuf, sizeof pbuf);
			CString checkInvno;
			checkInvno.Format(_T("T%s"), pbuf);

			if( CheckInvoiceNo() ) {
				int len = checkInvno.GetLength();
				::ZeroMemory(m_TkyInvno, sizeof m_TkyInvno);
				if( len > sizeof m_TkyInvno ) len = sizeof m_TkyInvno;
				memcpy(m_TkyInvno, checkInvno, len);

				CString stmp;
				stmp = checkInvno;
				stmp.Replace(_T("T"), _T(""));
				VARIANT var;
				var.pbVal = (BYTE*)(LPCTSTR)stmp;
				DBdata_set(this, IDC_ICSDBEDT_INV, &var, ICSDBEDT_TYPE_STRING, 0);
				DBSetDispString(this, IDC_ICSDBEDT_INV, checkInvno);
			}
		}
		else {
			if( inplen < 0 ) {
				// Delete, Back Spaceで削除すると-1となる
			//	CString dmyInvno;
			//	syzInvoice.CheckInvoiceNumber(dmyInvno);
			//	m_bMsgDsp = TRUE;
			//	ICSMessageBox(syzInvoice.GetErrorMessage(), MB_ICONSTOP); //必ずエラーになる

				::ZeroMemory(m_TkyInvno, sizeof m_TkyInvno);
				var.pbVal = NULL;
				DBdata_set(this, IDC_ICSDBEDT_INV, &var, ICSDBEDT_TYPE_STRING, 0);
				DBSetDispString(this, IDC_ICSDBEDT_INV, NULL);
			//	set_focus();
				nChar = 0;
			//	m_bMsgDsp = FALSE;
			}
		}
		break;

	case VK_DELETE:
		var.pbVal = NULL;
		::ZeroMemory(m_TkyInvno, sizeof m_TkyInvno);
		DBdata_set(this, IDC_ICSDBEDT_INV, &var, ICSDBEDT_TYPE_STRING, 0);
		DBSetDispString(this, IDC_ICSDBEDT_INV, NULL);
		break;
	}

	if( nChar == VK_TAB ) {
		if( ICSDBEDT_KST_SHIFT & kst ) {
			TerminationJob(IDC_ICSDBEDT_INV, -1);
		}
		else {
			TerminationJob(IDC_ICSDBEDT_INV, 1);
		}
	}
	else {
		switch( nChar ) {
		case VK_RETURN:
		case VK_DOWN:
		case VK_RIGHT:
			TerminationJob(IDC_ICSDBEDT_INV, 1);
			break;
		case VK_UP:
		case VK_LEFT:
		case VK_F2:
			TerminationJob(IDC_ICSDBEDT_INV, -1);
			break;
		}
	}
}


afx_msg void CTkyVer2Dlg::OnKillFocusIcsdbedt8Invno(long inplen)
{
	if( !m_bIgnoreFocus ) {
		TerminationIcsdbedt8Invno(0, inplen, 0);
	}
	else {
		m_bIgnoreFocus = FALSE;
	}
}


BOOL CTkyVer2Dlg::CheckInvoiceNo()
{
	char pbuf[128];
	VARIANT var;
	int ncnt;

	if( !m_bInvDisp )
		return TRUE;

	ncnt = GetInvnoString(pbuf, sizeof pbuf);

	if( ncnt == 0 )
		return TRUE;

	CString checkInvno;
	checkInvno.Format(_T("T%s"), pbuf);
	if( syzInvoice.CheckInvoiceNumber(checkInvno) == -1 ) {
		m_bIgnoreFocus = TRUE;
		m_ErrMsg = syzInvoice.GetErrorMessage();
		PostMessage(WM_MYMESSAGE, INV_ERRMSG);
	//	ICSMessageBox(syzInvoice.GetErrorMessage(), MB_ICONSTOP);
		var.pbVal = (BYTE*)pbuf;
		DBdata_set(this, IDC_ICSDBEDT_INV, &var, ICSDBEDT_TYPE_STRING, 0);
		DBSetDispString(this, IDC_ICSDBEDT_INV, "");
	//	set_focus(IDC_ICSDBEDT_INV);
	//	m_bIgnoreFocus = FALSE;
		return FALSE;
	}

	return TRUE;
}


//インボイス番号より
LRESULT CTkyVer2Dlg::OnMyMessage(WPARAM wp, LPARAM lp)
{
	 if( wp == INV_ERRMSG ) {
		if( IsWindowVisible() ) {
			ICSMessageBox(m_ErrMsg, MB_ICONSTOP, 0, 0, this);
			set_focus(IDC_ICSDBEDT_INV);
		}
		m_bIgnoreFocus = FALSE;
		m_ErrMsg.Empty();
	}

	return 1;
}


/*

void CTkyVer2Dlg::GetReverseConversion( CString& strText )
{
	HKL hKL = GetKeyboardLayout(0);

//	CANDIDATELIST	CandList = {0};
	CANDIDATELIST*	pCandList;

	HIMC hIMC = ImmGetContext(this->m_hWnd);
	int BufLen;
	BufLen =  ImmGetConversionList(hKL, hIMC, strText, NULL, 0, GCL_REVERSE_LENGTH)

	char pMem = new char[BufLen];
	if( pMem == NULL )
		return;

    ImmGetConversionList(hKL, hIMC, strText, pMem, BufLen, GCL_REVERSECONVERSION)

	ImmReleaseContext( this->m_hWnd, hImc );

	if( pCandList->dwCount > 0 ) {


	}

	delete [] pMem;


	
	Dim List() As String
	If CandList.dwCount > 0 Then
		List = Encoding.GetEncoding("shift-jis").GetString(Buffer, CandList.dwOffset, BufLen - CandList.dwOffset).Split(New Char() {ControlChars.NullChar})
		ReDim Preserve List(CandList.dwCount - 1)
	Else
		List = New String() {"*候補なし*"}




}



    Public Function GetReverseConversion(ByVal strText As String) As String()
        Try
            Dim hKL As IntPtr = GetKeyboardLayout(0)
            Dim CandList As New CANDIDATELIST
            Dim hIMC As IntPtr = ImmGetContext(Me.Handle)
            Dim BufLen As Integer = ImmGetConversionList(hKL, hIMC, strText, IntPtr.Zero, 0, GCL_REVERSE_LENGTH)
            Dim Buffer(BufLen - 1) As Byte
            Dim hGC As GCHandle = GCHandle.Alloc(Buffer, GCHandleType.Pinned)
            Dim hBuffer As IntPtr = hGC.AddrOfPinnedObject()
            ImmGetConversionList(hKL, hIMC, strText, hBuffer, BufLen, GCL_REVERSECONVERSION)
            Marshal.PtrToStructure(hBuffer, CandList)
            hGC.Free()
            ImmReleaseContext(Me.Handle, hIMC)
            Dim List() As String
            If CandList.dwCount > 0 Then
                List = Encoding.GetEncoding("shift-jis").GetString(Buffer, CandList.dwOffset, BufLen - CandList.dwOffset).Split(New Char() {ControlChars.NullChar})
                ReDim Preserve List(CandList.dwCount - 1)
            Else
                List = New String() {"*候補なし*"}
            End If
            Return List
        Catch ex As Exception
            Dim List() As String = New String() {"*エラー発生*"}
            Return List
        End Try
    End Function


*/
