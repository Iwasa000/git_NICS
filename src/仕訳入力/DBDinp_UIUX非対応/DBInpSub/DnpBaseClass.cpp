#include "stdafx.h"

#include "resource.h"

#include "DBInpSub8.h"
#include "DBInpSubFunc.h"
#include "DInputSub.h"

#include "DnpBaseClass.h"
#include "icsdbedt.h"
#include "font.h"

#include "FuriDlg.h"
#include "NSDenpDlg.h"

#include "DnpInpDialog.h"
#include "CStRecordSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define OPT_SYZSTOP		"SyzStop"		// 消費税欄 毎回確認


#define myICSMessageBox		ICSMessageBox


extern
int get_strcnt(char* str, int byte);


//-----------------------------------------------------------
// 初回入力枝番タイプ
//	return =0 ... 枝番未入力, =99 枝番入力
//
//-----------------------------------------------------------
int brn_dfltinptype( int brn )
{
	return (brn == -1 ? 0 : 99);
}


BOOL	IsDebtPosition( int pn )
{
	BOOL bRet = FALSE;
	switch( pn ) {	
	case FD_DBVAL_PN:
	case FD_DBZEI_PN:
	case FD_DBMN_PN:
	case FD_DKOJI_PN:
	case FD_DEBT_PN:
	case FD_DSYOG_PN:
	case FD_DSKBN_PN:
		bRet = TRUE;
		break;
	}

	return bRet;
}


BOOL	IsCredPosition( int pn )
{
	BOOL bRet = FALSE;
	switch( pn ) {	
	case FD_CRVAL_PN:
	case FD_CRZEI_PN:
	case FD_CBMN_PN	:
	case FD_CKOJI_PN:
	case FD_CRED_PN	:
	case FD_CSYOG_PN:
	case FD_CSKBN_PN:
		bRet = TRUE;
		break;
	}

	return bRet;
}





CDnpBaseClass::CDnpBaseClass()
{
	m_pDnpParent = NULL;

	BRNTKY_sel = FALSE;		//枝番摘要を選択画面に表示
	bKamokuCode = FALSE;	//科目コードを画面に表示
	m_SelDispPN = -1;		//現在選択画面に表示しているモード
	m_pSyzInp = NULL;		//消費税画面
	m_pHusenInp = NULL;		//付箋入力画面
	m_pHjiyuInp = NULL;		//変動事由画面

	m_bNotNumber = FALSE;
	m_SET_SYZINP_PN = -1;

	m_DataSeq = -1;

	m_DENP_PN = -1;
	m_DPDATA_LN = -1;
	m_pFuri		= NULL;
	m_pNSdlg	= NULL;

	// 輸入仕入れサインを クリア
	m_bSyzYunyu[0] = m_bSyzYunyu[1] = FALSE;
	// 伝票持越し用 内部伝票番号を保存
	m_InsideCno = -1;
	// データ位置
	m_TopIdx = 0;

	bImgStaticUpdate = FALSE;	//イメージスタティックセットフラグ
	m_bDispLno = TRUE;
	m_bLineCopy = m_bLinePaste = TRUE;
	m_nCopyLine = -1;

	m_bDLineMMinpIgnore = FALSE;
	m_bDLineChanging = FALSE;
	m_PRO_OK = FALSE;

	SyogSiwakeMode() = TRUE;
	show_flg = 0;

}


CDnpBaseClass::~CDnpBaseClass()
{


}


//---------------------------------------------------------------
//	摘要欄でのコード＋タブキー動作フラグを取得 [02.07 /13]
//
//	0 = 数字として入力
//	1 = 摘要枝番のみセットする。(文字としての数字は入力しない)
//---------------------------------------------------------------
int& CDnpBaseClass::TKtabMode()
{
	return tktab_sysflg;
}


//--------------------------------------------
//	摘要番号 ターミネーションフラグを取得
//
//--------------------------------------------
int& CDnpBaseClass::TekiyoKeyMode()
{
	return tky_sysflg;
}


//--------------------------------------------
//	金額 Enter 複写 フラグを取得 [09.08 /08]
//
//--------------------------------------------
int& CDnpBaseClass::ValEnterCopyMode()
{
	return val_sysflg;
}

//--------------------------------------------
//	金額 Enter 複写 フラグを取得 [09.08 /08]
//
//--------------------------------------------
int& CDnpBaseClass::SyzStopMode()
{
	return SyzStop_sysflg;
}

// m_Seldata より呼ばれる関数
//	固定科目部分に内容をセットする
int CALLBACK CDnpBaseClass::SetSelectFix8( _SelTable* pSel, SelSetProcPar* pSetProc, int cnt, CWnd* lp )
{
	BOOL bNumber;
	CDnpInpDialog* pDlg;
	pDlg = (CDnpInpDialog*)lp;
	CDnpBaseClass* pBase = pDlg->m_base;

	if( pBase->m_Seldata.GetKmDispMode() == SL_KMK_CODE )
		bNumber = FALSE;
	else
		bNumber = TRUE;

	CICSSel*	psel8 = NULL;
	psel8 = pBase->m_pselFix8;

	if( psel8 == NULL )	return 0;

	if( psel8->GetNumber() != bNumber )
		psel8->SetNumber( bNumber );

	psel8->SetData( (long)pSel );

	return 0;
}


// m_Seldata より呼ばれる関数
//	科目選択部分に内容をセットする
int CALLBACK CDnpBaseClass::SetSelectSel32( _SelTable* pSel, SelSetProcPar* pSetProc, int cnt, CWnd* lp )
{	
	BOOL bNumber;
	CDnpInpDialog* pDlg;
	pDlg = (CDnpInpDialog*)lp;
	CDnpBaseClass* pBase = pDlg->m_base;

	switch( pSetProc->sp_nowpn ) {
	case SL_KAMOKU_PN:
	case SL_KMKANA_PN:
		if( pBase->m_Seldata.GetKmDispMode() == SL_KMK_CODE )
			bNumber = FALSE;
		else
			bNumber = TRUE;
		break;
	case SL_BUMON_PN:
	case SL_BMNKANA_PN:
	case SL_KOJI_PN:
	case SL_KOJIKANA_PN:
		if( pBase->m_pAsel->BMNSEL_OPT == OPT_NONOPT )
			bNumber = FALSE;
		else
			bNumber = TRUE;
		break;

	default:
		bNumber = TRUE;
		if( pBase->m_bNotNumber )
			bNumber = FALSE;
		break;
	}

	CICSSel*	psel32 = NULL;
	psel32 = pBase->m_pselSel32;

	if( psel32->GetNumber() != bNumber )
		psel32->SetNumber( bNumber );

	if( pSetProc->sp_nowpn == SL_BRNTKY_PN ) {
		// 選択画面の色
		COLORREF txt = pBase->m_Seldata.GetSel32TextColor();
		COLORREF back = pBase->m_Seldata.GetSel32BackColor();

		psel32->SetForeColor( txt );	// 文字色
		psel32->SetBackColor( back );	// 背景色

		pBase->BRNTKY_sel = 1;
	}
	else {
		if( pSetProc->sp_prevpn == SL_BRNTKY_PN ) {
			// 選択画面の色を元に戻す
			pBase->enable_fixval( TRUE );	// 元背景色強制描画のため
		}
		else {
			if( psel32->GetBackColor() == BRTEK_BackCOL ) { //枝番の背景色が残る場合があった。
				pBase->enable_fixval( TRUE );
			}
		}
		pBase->BRNTKY_sel = 0;
	}

	pBase->m_SelDispPN = pSetProc->sp_nowpn;
	CString	stitle = pSetProc->seljob;

	// 03.16 /11
	if( pSetProc->sp_nowpn == SL_BUMON_PN || pSetProc->sp_nowpn == SL_KOJI_PN ||
		pSetProc->sp_nowpn == SL_BMNKANA_PN || pSetProc->sp_nowpn == SL_KOJIKANA_PN ) {
		if( (pBase->m_pAsel->BMNSEL_OPT == OPT_BMNSEL) )
			stitle += _T( "\r\n (画面番号優先) " );
		else {
			if( pSetProc->sp_nowpn == SL_BUMON_PN || pSetProc->sp_nowpn == SL_BMNKANA_PN )
				stitle += _T( "\r\n (部門番号優先) " );
			else
				stitle += _T( "\r\n (工事番号優先) " );
		}

		if( pSetProc->sp_nowpn == SL_KOJI_PN || pSetProc->sp_nowpn == SL_KOJIKANA_PN ) {
			stitle += _T( "\r\n [*]は完成工事 " );
		}
	}

	pBase->set_valtitle( (char*)(LPCTSTR)stitle );
	psel32->SetData( (long)pSel );

//FILETrace( ">> SetSelectSel32 BRNTKY %d\n", pBase->BRNTKY_sel );

	return 0;
}



int CDnpBaseClass::bmnzan_count(CDatabase* database, CString& code, CString& errmsg)
{

	CStRecordSet	bminp(database);
	int	cnt = 0;

	TRY {
		bminp.m_sql.Format( "select kzbmn from bmkzrec where kzkcd = '%s'", code );

		if( ! bminp.Open() ) {
			errmsg = _T( "部門未入力レコードセットをオープンできません！" );
			return -1;
		}

		if( bminp.IsBOF() ) {
			//レコードなし
			bminp.Close();
		}
		else {
			do {
				// 部門コード 範囲チェック
				if( m_BmChk.BmnHaniCheck( bminp.m_st, code ) ) {
					cnt = 1;
					break;
				}

				bminp.MoveNext();
			} while( ! bminp.IsEOF() );

			bminp.Close();
		}

	}
	CATCH( CMemoryException, me ) {
		me->GetErrorMessage( errmsg.GetBuffer(_MAX_PATH), _MAX_PATH );
		errmsg.ReleaseBuffer();
		return -1;	
	}
	CATCH( CDBException, de ) {
		errmsg = de->m_strError;
		return -1;
	}
	END_CATCH

	return cnt;
}


// 固定項目の表示
void CDnpBaseClass::fix8disp( BOOL sg )
{
	if( sg )
	{
		// 固定科目選択のディセーブル／イネーブル
		enable_fix8( sg );
		// 選択タイトル表示のディセーブル／イネーブル
		valtitle( !sg );
	}
	else
	{
		// 選択タイトル表示のディセーブル／イネーブル
		valtitle( !sg );
		// 固定科目選択のディセーブル／イネーブル
		enable_fix8( sg );
	}
}



// 固定科目選択のディセーブル／イネーブル
void CDnpBaseClass::enable_fix8( BOOL sg )
{
	m_pselFix8->ShowWindow(sg ? SW_SHOW : SW_HIDE);
}

//
// 選択画面の表示色
void CDnpBaseClass::enable_fixval( BOOL sg )
{
	CICSDisp *ttlbaseorg,*ttlorg;

	VERIFY(ttlbaseorg = (CICSDisp*)m_pDnpParent->GetDlgItem( IDC_VALTITLEBASE ));
	VERIFY(ttlorg = (CICSDisp*)m_pDnpParent->GetDlgItem( IDC_VALTITLE ));

#ifndef ICSCOLOR_
	ICSColorInfo ci;
	((ICSWinApp*)AfxGetApp())->GetStanderdColor( ci );

	// 選択画面の色
	COLORREF txt32, back32;

	if( BRNTKY_sel ) {
		txt32 = m_Seldata.GetSel32TextColor();
		back32 = m_Seldata.GetSel32BackColor();
	}
	else {
		txt32 = ci.m_swOnOff > 0 ? ci.PageKmkTextColor() : RGB_VAL32_FORE;
		back32 = ci.m_swOnOff > 0 ? ci.PageKmkBackColor() : RGB_VAL32_BACK;
	}

	if( ci.m_swOnOff > 0 ) {
//	if( 0 ) {
		// 科目選択画面背景色の設定
		m_pselFix8->SetBackColor( ci.FixKmkBackColor() );
		m_pselSel32->SetBackColor( back32 );

		// 科目選択画面文字色の設定
		m_pselFix8->SetForeColor( ci.FixKmkTextColor() );
		m_pselSel32->SetForeColor( txt32 );

		// 科目選択画面枠の色
		m_pselFix8->SetLineColor( RGB_GRAY );
		m_pselSel32->SetLineColor( RGB_GRAY );

		// 選択タイトル背景色、文字色
		ttlbaseorg->SetBackColor( ci.PageKmkBackColor() );
		ttlorg->SetBackColor( ci.PageKmkBackColor() );
		ttlorg->SetForeColor( ci.PageKmkTextColor() );

		m_vwColor = ci.ViewColor();
	}
	else {
		// 科目選択画面背景色の設定
		m_pselFix8->SetBackColor( RGB_FIX8_BACK );
		m_pselSel32->SetBackColor(back32);

		// 科目選択画面文字色の設定
		m_pselFix8->SetForeColor( RGB_FIX8_FORE );
		m_pselSel32->SetForeColor(txt32);
		// 科目選択画面枠の色
		m_pselFix8->SetLineColor( RGB_GRAY );
		m_pselSel32->SetLineColor( RGB_GRAY );

		// 選択タイトル背景色、文字色
		ttlbaseorg->SetBackColor( RGB_VAL32_BACK );
		ttlorg->SetBackColor( RGB_VAL32_BACK );
		ttlorg->SetForeColor( RGB_VAL32_FORE );
	}
#else

	// New Size
	if( sg )
	{
		// 科目選択画面背景色の設定
		m_pselFix8->SetBackColor( RGB_FIX8_BACK );
		m_pselSel32->SetBackColor( RGB_VAL32_BACK );

		// 科目選択画面文字色の設定
		m_pselFix8->SetForeColor( RGB_FIX8_FORE );
		m_pselSel32->SetForeColor( RGB_VAL32_FORE );
		// 科目選択画面枠の色
		m_pselFix8->SetLineColor( RGB_GRAY );
		m_pselSel32->SetLineColor( RGB_GRAY );

		// 選択タイトル背景色、文字色
		ttlbaseorg->SetBackColor( RGB_VAL32_BACK );
		ttlorg->SetBackColor( RGB_VAL32_BACK );
		ttlorg->SetForeColor( RGB_VAL32_FORE );
	}
	else
	{
		// 科目選択画面背景色の設定
		m_pselFix8->SetBackColor( RGB_FIX8_BACK_UNSEL );
		m_pselSel32->SetBackColor( RGB_VAL32_BACK_UNSEL );
		// 科目選択画面文字色の設定
		m_pselFix8->SetForeColor( RGB_FIX8_FORE_UNSEL );
		m_pselSel32->SetForeColor( RGB_VAL32_FORE_UNSEL );
		// 科目選択画面枠の色
		m_pselFix8->SetLineColor( RGB_GRAY );
		m_pselSel32->SetLineColor( RGB_GRAY );

		// 選択タイトル背景色、文字色
		ttlbaseorg->SetBackColor( RGB_VAL32_BACK_UNSEL );
		ttlorg->SetBackColor( RGB_VAL32_BACK_UNSEL );
		ttlorg->SetForeColor( RGB_VAL32_FORE_UNSEL );
	}
#endif
}

// 選択タイトルテキストセット
void CDnpBaseClass::set_valtitle( char *msg )
{
	((CICSDisp*)m_pDnpParent->GetDlgItem( IDC_VALTITLE ))->SetCaption( msg );
	((CICSDisp*)m_pDnpParent->GetDlgItem( IDC_VALTITLE ))->CheckData();
}

// 選択タイトル表示のディセーブル／イネーブル
void CDnpBaseClass::valtitle( BOOL sg )
{
	CWnd *ttlbaseorg,*ttlorg;
	RECT rect8,rect32,newrect,newrect2;
	LONG v_h, v_w;
	WINDOWPLACEMENT place;

	VERIFY(ttlbaseorg = (CICSDisp*)m_pDnpParent->GetDlgItem( IDC_VALTITLEBASE ));
	VERIFY(ttlorg = (CICSDisp*)m_pDnpParent->GetDlgItem( IDC_VALTITLE ));

	// New Size
	if( sg == TRUE )
	{
		m_pselFix8->GetWindowRect( &rect8 );
		m_pselSel32->GetWindowRect( &rect32 );

		newrect.top = rect8.top;
		newrect.left = rect8.left;
		newrect.bottom = rect8.bottom;	// m_selfix8_rect.bottom * rate_h;
		newrect.right = rect8.right;	// m_selfix8_rect.right * rate_w;

	//	ttlbaseorg->SetWindowPos( 0, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE );
		m_pDnpParent->ScreenToClient( &newrect );
		ttlbaseorg->MoveWindow( newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top );

		v_h = (newrect.bottom - newrect.top) / 3;
		v_w = ((newrect.right - newrect.left) / 12);
		ttlbaseorg->GetWindowPlacement( &place );
		newrect2.top = place.rcNormalPosition.top + v_h;
		newrect2.left = place.rcNormalPosition.left;
		newrect2.bottom = newrect2.top + v_h;
		newrect2.right = place.rcNormalPosition.right;

		ttlorg->SetWindowPos( 0, newrect2.left, newrect2.top, newrect2.right - newrect2.left, newrect2.bottom - newrect2.top, SWP_NOZORDER|SWP_NOACTIVATE );
	}
	else
	{
		newrect.top = 0;
		newrect.left = 0;
		newrect.bottom = 0;
		newrect.right = 0;

		ttlbaseorg->SetWindowPos( 0, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE );
		ttlorg->SetWindowPos( 0, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_NOZORDER|SWP_NOACTIVATE );
	}

//	ttlbaseorg->UpdateWindow();
//	ttlorg->UpdateWindow();
}



// 部門コードチェック 部門範囲より
int CDnpBaseClass::CheckBumonInput( int bmn )
{
	if( IsJob() == JOB_INPUT ) {
		return 0;
	}
	
	if( bmn == -1 )
		return 0;

	CString code;

	int ln, pn, kmpn, kmpn2, max, line, dt_ln;
	int oksgn = 0;

	ln = get_nowln();
	pn = get_nowpn();
	dt_ln = get_dataline(ln);

	BOOL bDataBmon = FALSE;

	if( IsKind() == KIND_FURIKAE ) {
		if( m_FuriMode ==  FURI_DATABMON ) {
			kmpn = (pn == FD_DBMN_PN) ? FD_DEBT_PN : FD_CRED_PN;
			bDataBmon = TRUE;
		}
	}

	if( bDataBmon ) {
		if( _getCND( dt_ln, kmpn )->INP_sg ) {
			code = _getDATA( dt_ln, kmpn )->DP_CdNum;
		}

		if( ! m_BmChk.BmnHaniCheck( bmn, code ) )
			return -1;
	}
	else {
		switch( IsKind() ) {
		case KIND_FURIKAE:
		//	max = DP_SCROLL_NL;
			kmpn	= FD_DEBT_PN;
			kmpn2	= FD_CRED_PN;
			break;
		case KIND_NYUKIN:
		//	max = DP_SCROLL_NL_N;
			kmpn	= FD_CRED_PN;
			kmpn2	= -1;
			break;
		case KIND_SYUKKIN:
		//	max = DP_SCROLL_NL_S;
			kmpn	= FD_DEBT_PN;
			kmpn2 = -1;
			break;
		}
		max = get_MaxLine();

		// 伝票全体で、部門入力がひとつのときは、
		// すべての 入力科目で、部門範囲チェックをする。
		for( line = 1; line <= max; line++ ) {

			if( kmpn != -1 ) {
				if( _getCND( line, kmpn )->INP_sg ) {
					code	= _getDATA( line, kmpn )->DP_CdNum;
				}

				if( m_BmChk.BmnHaniCheck( bmn, code ) ) {
					oksgn++;
					break;
				}
			}
			if( kmpn2 != -1 ) {
				if( _getCND( line, kmpn2 )->INP_sg ) {
					code	= _getDATA( line, kmpn2 )->DP_CdNum;
				}

				if( m_BmChk.BmnHaniCheck( bmn, code ) ) {
					oksgn++;
					break;
				}
			}
		}

		if( ! oksgn )	return -1;

	}

	return 0;
}


void CDnpBaseClass::EndWork()
{
	DBGFUNCTION;
	if (m_pDENPPAR->modeless == 0) {
		CloseWork();
	}
	SyzInpDelete();
	HusenInpDelete();
	HjiyuInpDelete();
}

//----------------------------------------------------------
//  初期化処理(選択画面＆消費税画面)
//	BOOL oninitdlg (OnInitDialog からの呼び出し)
//-----------------------------------------------------------
int CDnpBaseClass::InitWork( BOOL oninitdlg ) 
{
	CWnd* org;

	DMY_SEQ = 0;
	// 輸入仕入れサインを クリア
	m_bSyzYunyu[0] = m_bSyzYunyu[1] = FALSE;

	IsKind()		=	KIND_FURIKAE;
	IsType()		=	TYPE_TANDOKU;
	IsMode()		=	MODE_NORMAL;
	IsJob()			=	JOB_INPUT;

	if (oninitdlg) {
		m_pDnpParent->GetWindowRect(&m_original_rect);	//オリジナルのサイズゲット
		m_pDnpParent->GetDlgItem(IDC_ICSSELCTRL1)->GetClientRect(&m_selfix8_rect);	//オリジナルのサイズゲット
	}

	// 選択タイトル背景色、文字色
#ifdef ICSCOLOR_
	ICSColorInfo ci;
	((ICSWinApp*)AfxGetApp())->GetStanderdColor( ci );

	CWnd *ttlbaseorg,*ttlorg;
	VERIFY(ttlbaseorg = (CICSDisp*)m_pDnpParent->GetDlgItem( IDC_VALTITLEBASE ));
	VERIFY(ttlorg = (CICSDisp*)m_pDnpParent->GetDlgItem( IDC_VALTITLE ));

	if( ci.m_swOnOff > 0 ) {
		((CICSDisp*)ttlbaseorg)->SetBackColor( ci.FixKmkBackColor() );
		((CICSDisp*)ttlbaseorg)->SetForeColor( ci.FixKmkTextColor() );

		((CICSDisp*)ttlorg)->SetBackColor( ci.FixKmkBackColor() );
		((CICSDisp*)ttlorg)->SetForeColor( ci.PageKmkTextColor() );
	}
	else {
		((CICSDisp*)ttlbaseorg)->SetBackColor( RGB_FIX8_BACK );
		((CICSDisp*)ttlbaseorg)->SetForeColor( RGB_BLACK );
		((CICSDisp*)ttlorg)->SetBackColor( RGB_FIX8_BACK );
		((CICSDisp*)ttlorg)->SetForeColor( RGB_VAL32_FORE );
	}
#endif

	// 選択タイトル用ＤＩＳＰコントロールのサイズをゼロ（見えなくする）に
	WINDOWPLACEMENT place;
	RECT rct;
	VERIFY(org = m_pDnpParent->GetDlgItem( IDC_ICSSELCTRL1 ));
	org->GetWindowPlacement( &place );
	rct = place.rcNormalPosition;

	VERIFY(org = m_pDnpParent->GetDlgItem( IDC_VALTITLEBASE));
	org->SetWindowPos( 0, rct.left, rct.top, 0, 0, SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOSIZE );

	VERIFY(org = m_pDnpParent->GetDlgItem( IDC_VALTITLE));
	org->SetWindowPos( 0, rct.left, rct.top, 0, 0, SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOSIZE );

	m_DinpSW.Init( m_pZm );
	m_pAsel = m_DinpSW.GetDinpSW();

	ASSERT( m_pAsel != NULL );

	if( m_DI.InitSub( m_pZm, m_pSy, m_pAsel, m_pDnpParent) != 0 ) {
		return -1;
	}

	m_Seldata.Init( m_pZm, m_pDnpParent, (SELECTDATAPROC)SetSelectFix8, (SELECTDATAPROC)SetSelectSel32 );

	if( m_pAsel->KMNO_OPT == OPT_KMNO ) {
		// 科目コード表示
		m_Seldata.SetKmDispMode( SL_KMK_CODE );
		bKamokuCode = TRUE;
	}
	else {
		m_Seldata.SetKmDispMode( SL_KMK_NUM );
		bKamokuCode = FALSE;
	}

	// 部門チェック
	if( m_BmChk.SetDBZmSub( m_pZm ) != 0 ) {
		m_ErrMsg = _T("部門チェッククラスの作成ができません！");
		return -1;
	}
	if( m_BmChk.BmnCheckInit() != 0 ) {
		m_ErrMsg = _T("部門チェッククラスの初期化ができません！");
		return -1;
	}

	// 工事番号 チェック
	if( m_KjChk.SetDBZmSub( m_pZm ) != 0 ) {
		m_ErrMsg = _T("工事番号チェッククラスの作成ができません！");
		return -1;
	}
	if( m_KjChk.KojiCheckInit() != 0 ) {
		m_ErrMsg = _T("工事番号チェッククラスの初期化ができません！");
		return -1;
	}

	int	sw = 0;

	if( m_pAsel->BRTEK_OPT == OPT_DSP )	// 枝番名称表示
		sw |= BT_NOMALBRN_TEK;
	if( m_pAsel->SYGBRN_OPT == OPT_DSP )	// 諸口枝番表示
		sw |= BT_SYOGTBRN_TEK;

//	sw |= SL_BRNTKY_ONLY;
	m_Seldata.SetBrnTkyMode( sw );

	// 仕訳摘要表示優先
	sw = 0;
	sw = (m_pAsel->SWKTEK_OPT == OPT_SWKTEK ) ? SL_TKY_SIWAKE : SL_TKY_RENSO;
	m_Seldata.SetTekiyoMode( sw );

	//カナ検索時のカナ順表示
	m_Seldata.SetKanaDspMode( (m_pAsel->KANADSP_OPT == OPT_KANAJUN) ? SL_KJUN_DSP : SL_NORMAL_MODE ); 
	//科目ページ保持
	m_Seldata.SetKamokuPageHold( m_pAsel->KMPG_OPT == OPT_KMPG ); 

	DWORD opt = 0;
	if( m_pAsel->SWTKYD_OPT == OPT_SWTKYD )	opt |= 0x01;
	if( m_pAsel->BMNTKBR_OPT == OPT_CHKON )	opt |= 0x02;
	if( m_pAsel->RTSWKKANA_OPT == OPT_KANAJUN )	opt |= 0x04;

//MyTrace( "baseview m_Seldata.SetSelOptMode = %04x\n", opt );

	m_Seldata.SetSelOptMode( opt );

	int mode = SL_NORMAL_MODE;
	if( m_pAsel->BRNKANA_OPT == OPT_KANAJUN ) {
		mode = SL_KJUN_DSP;
	}
	m_Seldata.SetBrnSelDispMode(mode);

	//モードレスダイアログ作成はOnInitDialogで
	if (oninitdlg) {
		// 消費税画面作成
		SyzInpMake();
		// 付箋入力画面作成
		HusenInpMake();
		// 変動事由入力画面作成
		HjiyuInpMake();
	}
	else {
		if( m_pSyzInp != NULL )
		{
			if( m_pSyzInp->SetPara(m_pZm, m_pSy, m_pDnpParent) != 0 ) {
				ErrMsgSet(ERROR_MEMORY, "消費税入力オブジェクトが作成できません。");
				return -1;
			}
		}
	}
	fix8disp( TRUE );

	SelPar par;
	par.sel_pn = SL_KAMOKU_PN;
	m_Seldata.SelectJob( &par, -1 );

	m_InsideCno		= -1;
	m_bIgnoreFocus	=	FALSE;

	memset( tblImage, 0, sizeof tblImage );			//イメージ摘要サイン
	memset( tblImageOfst, 0, sizeof tblImageOfst );	//イメージ摘要オフセット
	memset( tblDel, 0, sizeof tblDel );

	//伝票画面を作成する
	int bmln, kjln, edaln, syonum;
	bmln = kjln = edaln = syonum = 0;

	if( m_DI.IsBumon() )		bmln = m_pZm->zvol->bmcol;
	if( m_DI.IsKoji() )			kjln = m_pZm->zvol->kjcol;

	if (m_DI.IsEdaban())		edaln = m_pZm->zvol->edcol;
	if( m_DI.IsSyonum() )		syonum = m_pZm->zvol->snocol;

	CDnpBaseDlg::SetMasterPar( bmln, kjln, edaln, syonum );
	CDnpBaseDlg::SetScanTitle( m_DI.IsConFirmMaster() );

	if (oninitdlg) {
		if (m_pFuri == NULL) {
			m_pFuri = new CFuriDlg(m_pDnpParent);
			m_pFuri->m_pBaseCls = this;
			if (!m_pFuri->Create(IDD_DLG_FURI, m_pDnpParent)) {
				ErrMsgSet(ERROR_MEMORY, "振替伝票画面を作成できません！");
			}
			//	m_pFuri->GetClientRect( &m_FuriRect );
		}

		if (m_pNSdlg == NULL) {
			m_pNSdlg = new CNSDenpDlg(m_pDnpParent);
			m_pNSdlg->m_pBaseCls = this;
			if (!m_pNSdlg->Create(IDD_DLG_NSKIN, m_pDnpParent)) {
				ErrMsgSet(ERROR_MEMORY, "入出金伝票画面を作成できません！");
			}
			//	m_pFuri->GetClientRect( &m_NSRect );
		}
	}

	sheet_clr();

	//設定読み出し
	ProfileSet();

#ifdef LATER_CLOSE
	CRDdnpBaseDlg::SetMasterPar( bmln, kjln );
	// 日付セット
	m_selDate.SetMode( ICS_DATESELECT_MODE_ONESELF );
	m_selDate.Iniz( m_pZm, ICS_DATESELECT_RANGE_PERIOD|ICS_DATESELECT_EDIT_OFF|ICS_DATESELECT_INPOK_ONLY, 0, 0 );

	m_FuriMode = ((CMainFrame*)GetDBDinpMainFrame())->FuriMode();
#endif

	FuriModeChange( m_FuriMode );

	m_pFuri->ChangeJob( IsJob() );
	m_pNSdlg->ChangeJob( IsJob() );

#ifdef LATER_CLOSE
	int nAswk = ((CMainFrame*)GetDBDinpMainFrame())->AutoSwkType();
	if( nAswk & 0x02 )	IsMode() = MODE_AUTO;
	else				IsMode() = MODE_NORMAL;
#endif
	IsMode() = MODE_NORMAL;

	m_pFuri->AutoSiwake( IsMode() );
	m_pNSdlg->AutoSiwake( IsMode() );

	//伝票スキャンイニシャライズ
	m_DPscan.DenpScanInit( &m_DI, m_pDENPPAR );

	inpctl_inz();
	// 伝票番号、日付などの初期データを設定
	BYTE pack[16] = {0};
	m_DI.GetInpDate(pack);

	getDATA( DP_MM_PN )->DP_BCDDAY[0] = pack[0];
	getCND( DP_MM_PN )->INP_sg = TRUE;
	getDATA( DP_DD_PN )->DP_BCDDAY[0] = pack[1];
	getCND( DP_DD_PN )->INP_sg = TRUE;

	getDATA( DP_DENP_PN )->DP_DENP = -1;
	getDATA( DP_BMN_PN )->DP_BMN = -1;
	::ZeroMemory( getDATA( DP_KOJI_PN )->DP_KOJI, sizeof getDATA( DP_KOJI_PN )->DP_KOJI );
	::ZeroMemory(getDATA(DP_SYONUM_PN)->DP_SNUM, sizeof getDATA(DP_KOJI_PN)->DP_SNUM);

//	sheet_clr();

	// 摘要 入力モードGet
	CString section, key;
	section = "DBDINP";

	int rval = m_pWinApp->GetTekiyoKeyMode();
	TekiyoKeyMode() = rval;
	// 金額 Enter 複写
	key = "ValCopy";
	rval = m_pWinApp->GetOptionInt( section, key, 0 );
	ValEnterCopyMode() = rval;
	// タブキーの動作
	key = "Tktab";
	rval = m_pWinApp->GetOptionInt( section, key, 0 );
	TKtabMode() = rval;

	kana_sysflg = GetKanaSearchType();

	// 消費税欄毎回停止
//	rval = m_pWinApp->GetOptionInt(section, OPT_SYZSTOP, 0);
	rval = GetSyzStopMode();
//	MyTrace("GetSyzStopMode = %d\n", rval );

	if( m_DI.IsKani() || m_DI.IsMenzei() ) {
		SyzStopMode() = 0;
	}
	else {
		SyzStopMode() = rval;
	}

	m_PRO_OK = TRUE;

	return 0;
}


//----------------------------------------------------------
//   終了処理(諸口枝番で使用の分析表レコードクローズ)
//
//-----------------------------------------------------------
void CDnpBaseClass::CloseWork() 
{
	m_Seldata.Close();
	m_DI.CloseSub();
}


void CDnpBaseClass::ModelessCloseJob()
{
	DBGFUNCTION;
	m_PRO_OK = FALSE;
	CloseWork();
	sheet_clr(SHEET_ALL, 1);
	m_DPscan.Close();
}


// 選択画面の設定をUpdate
//
void CDnpBaseClass::UpdateSelectPar() 
{

	if( m_pAsel->KMNO_OPT == OPT_KMNO ) {
		// 科目コード表示
		m_Seldata.SetKmDispMode( SL_KMK_CODE );
		bKamokuCode = TRUE;
	}
	else {
		m_Seldata.SetKmDispMode( SL_KMK_NUM );
		bKamokuCode = FALSE;
	}

	int	sw = 0;
	sw = m_Seldata.GetBrnTkyMode();
	sw &= ~(BT_NOMALBRN_TEK|BT_SYOGTBRN_TEK);

	if( m_pAsel->BRTEK_OPT == OPT_DSP )	// 枝番名称表示
		sw |= BT_NOMALBRN_TEK;
	if( m_pAsel->SYGBRN_OPT == OPT_DSP )	// 諸口枝番表示
		sw |= BT_SYOGTBRN_TEK;

	m_Seldata.SetBrnTkyMode( sw );

	// 仕訳摘要表示優先
	sw = 0;
	sw = (m_pAsel->SWKTEK_OPT == OPT_SWKTEK ) ? SL_TKY_SIWAKE : SL_TKY_RENSO;
	m_Seldata.SetTekiyoMode( sw );

	//カナ検索時のカナ順表示
	m_Seldata.SetKanaDspMode( (m_pAsel->KANADSP_OPT == OPT_KANAJUN) ? SL_KJUN_DSP : SL_NORMAL_MODE ); 
	//科目ページ保持
	m_Seldata.SetKamokuPageHold( m_pAsel->KMPG_OPT == OPT_KMPG ); 

	SelPar par;
	par.sel_pn = SL_KAMOKU_PN;
	m_Seldata.SelectJob( &par, -1 );

//	m_Seldata.SelectUpdate();
}

//----------------------------------------------------------
//	選択モジュールで扱う ウィンドウの変更
//-----------------------------------------------------------
void CDnpBaseClass::SetSelectWindow() 
{
	m_Seldata.SetWindow( m_pDnpParent );
}



//----------------------------------------------------------
//	選択モジュールでの表示リセット
//-----------------------------------------------------------
void CDnpBaseClass::ResetSelect() 
{
	m_Seldata.KamokuRestrict( m_pAsel->INDSGN_OPT );

	// 公益用
	m_Seldata.NpKamokuRestrict( m_pAsel->NPJGY_OPT, m_pAsel->NPKNR_OPT );

//	SelPar par;
//	par.sel_pn = SL_RESET;
//
//	m_Seldata.SelectJob( &par, -1 );
}

//----------------------------------------------------------
//	摘要五十音文字列チェック
//	return	TRUE ---- カナ検索
//			FALSE --- 数字文字列なので摘要番号指定を行う
//-----------------------------------------------------------
BOOL CDnpBaseClass::tky_50kana_chk( char *data )
{
	int n;
	unsigned char *p;
	char str[50];

	strcpy_s( str, sizeof str, (char *)data );
	p = (unsigned char *)str;

	if( !isdigitstr( (char*)p ) )	// 数字文字列でない
	{
		// ASCII to ANK
		for( ; *p ; ++p )
		{
			if( (unsigned char)*p == (unsigned char)'ｰ' )
				*p = '-';
			else if( (unsigned char)*p == (unsigned char)' ' )
				*p = ' ';
		}

		n = sprintf_s( m_tky_50kana, sizeof m_tky_50kana, "%.10s", str );

		if( strlen( m_tky_50kana ) > 4 ) {
			m_tky_50kana[4] = '\0';
		}

		return TRUE;
	}
	return FALSE;
}

//----------------------------------------------------------
//	摘要選択
//	CDBipTKREC* ptkrec  摘要レコード
//	char *data ........ 摘要番号文字列
//	return	0 ---- 摘要レコード あり
//		   -1 ---- 摘要レコード なし
//-----------------------------------------------------------
int CDnpBaseClass::tky_select( CDBipTKREC* ptkrec, char *data )
{
	SelResult srt;
	if( m_SelDispPN != SL_SLIP_TKY_PN ){
		srt.sel_pn = SL_TKY_PN;
	}
	else{
		srt.sel_pn = SL_SLIP_TKY_PN;
	}

	if( ! isdigitstr( data ) )	//数字文字列
		return -1;

	if( m_Seldata.GetSelResult( &srt, data ) ) {

		if( m_SelDispPN != SL_SLIP_TKY_PN ){

			m_pZm->tkrec->MoveFirst();

			int tkyo_len = TKYO_LEN;
			if( ! m_DI.IsMasterType(MST_TKY20) )	tkyo_len = (TKYO_LEN/2);

			int sgn = 0;
			if( m_pZm->tkrec->st != -1 ) {
				do {
					if( srt.tky_code == m_pZm->tkrec->tkcod ) {
						ptkrec->m_tkcod = m_pZm->tkrec->tkcod;
						ptkrec->m_tkana = m_pZm->tkrec->tkana;
						ptkrec->m_tksgn.Copy( m_pZm->tkrec->tksgn );
						ptkrec->m_tkname = m_pZm->tkrec->tkname;
						// 後ろスペースをカット
						LPSTR	p = ptkrec->m_tkname.GetBuffer(256);
						int n = kjlen( p, tkyo_len );
						*(p + n) = '\0';
						ptkrec->m_tkname.ReleaseBuffer();

						ptkrec->m_tkari = m_pZm->tkrec->tkari;
						ptkrec->m_tkasi = m_pZm->tkrec->tkasi;
						sgn = 1;
						break;
					}
				} while( m_pZm->tkrec->MoveNext() == 0 );
			}
			return (sgn) ? 0 : -1;
		}
		else{
			ptkrec->m_tkcod = srt.tky_code;
			ptkrec->m_tkname = srt.tky;
			return 0;
		}
	}

	return -1;

}


//消費税入力画面作成＆表示
void CDnpBaseClass::SyzInpMake()
{
	if (m_pSyzInp == NULL)
	{
		if( !(m_pSyzInp = new CSyzInp()) )
		{
			ErrMsgSet( ERROR_MEMORY, "消費税入力オブジェクトが作成できません。");
			return;
		}
	}

	// インボイス対応
//	m_pSyzInp->SetDenpMode();

	if( m_pSyzInp->SetPara( m_pZm, m_pSy, m_pDnpParent ) != 0 ) {
		ErrMsgSet( ERROR_MEMORY, "消費税入力オブジェクトが作成できません。");
		return;
	}
}

//消費税入力画面 完全に 削除
void CDnpBaseClass::SyzInpDelete()
{
	if (m_pSyzInp ) {
		delete m_pSyzInp;
		m_pSyzInp = NULL;
	}
}


//消費税入力画面 を非表示
void CDnpBaseClass::Destroy_SyzInp()
{
	if (m_pSyzInp ) {
		if( m_pSyzInp->IsDisplay() ) {
			m_pSyzInp->DialogOFF();
		}
	}
}

// 消費税名称を画面にセット
void CDnpBaseClass::DispSyzIchiran( WPARAM wParam, LPARAM lParam )
{
	DWORD i;
	CString *str = (CString*)lParam;

#define SYZSEL_MAX	8*4

static struct _SelTable SelVal32[SYZSEL_MAX];
static SelSetProcPar	spar;

	spar.sp_prevpn = m_SelDispPN;
	spar.sp_nowpn = BVW_SYZ_PN;
	WORD dspSign = HIWORD(wParam);
	if( dspSign & 0x01 ) {
		strcpy_s(spar.seljob, sizeof spar.seljob, "《登録番号》");
	}
	else {
		strcpy_s(spar.seljob, sizeof spar.seljob, "《消費税》");
	}
	int dspCnt = LOWORD(wParam);

	for( i = 0 ; i < SYZSEL_MAX ; ++i )
	{
		// 表示
		SelVal32[i].Data = i < dspCnt ? *(str+i) : "";
	//	SelVal32[i].number = i + (isEntrySys() ? 9 : 1);
		SelVal32[i].number = i + 1;
	}

	SetSelectSel32( SelVal32, &spar, SYZSEL_MAX, m_pDnpParent );

}

//付箋入力画面作成＆表示
void CDnpBaseClass::HusenInpMake()
{
	if (m_pHusenInp == NULL)
	{
		if( !(m_pHusenInp = new CHusenInp()) )
		{
			ErrMsgSet( ERROR_MEMORY, "付箋入力オブジェクトが作成できません。");
			return;
		}
	}

	if( m_pHusenInp->SetPara( m_pZm, m_pDnpParent ) != 0 ) {
		ErrMsgSet( ERROR_MEMORY, "付箋入力オブジェクトが作成できません。");
		return;
	}
}

//付箋入力画面 完全に 削除
void CDnpBaseClass::HusenInpDelete()
{
	if (m_pHusenInp ) {
		delete m_pHusenInp;
		m_pHusenInp = NULL;
	}
}


//消費税入力画面 を非表示
void CDnpBaseClass::Destroy_HusenInp()
{
	if (m_pHusenInp ) {
		if( m_pHusenInp->IsDisplay() ) {
			m_pHusenInp->DialogOFF();
		}
	}
}


//変動事由入力画面作成＆表示
void CDnpBaseClass::HjiyuInpMake()
{
	if (m_pHjiyuInp == NULL)
	{
		if( !(m_pHjiyuInp = new CHjiyuInp()) )
		{
			ErrMsgSet( ERROR_MEMORY, "変動事由入力オブジェクトが作成できません。");
			return;
		}
	}

	if( m_pHjiyuInp->SetPara( m_pZm, m_pDnpParent ) != 0 ) {
		ErrMsgSet( ERROR_MEMORY, "変動事由入力オブジェクトが作成できません。");
		return;
	}
}

//変動事由入力画面 完全に 削除
void CDnpBaseClass::HjiyuInpDelete()
{
	if (m_pHjiyuInp ) {
		delete m_pHjiyuInp;
		m_pHjiyuInp = NULL;
	}
}


//変動事由入力画面 を非表示
void CDnpBaseClass::Destroy_HjiyuInp()
{
	if (m_pHjiyuInp ) {
		if( m_pHjiyuInp->IsDisplay() ) {
			m_pHjiyuInp->DialogOFF();
		}
	}
}


//------------------------------------------------------------------
//    科目選択チェック
//
//  struct _KamokuData& ans ..... チェック結果格納
//  struct _KamokuData& check ... チェック用 
//  int _dcsw ......... 借／貸
//  BOOL bSelClick .... 選択欄をクリックで選んだか？
//	return TRUE ------> 該当科目があった。
//-------------------------------------------------------------------
BOOL CDnpBaseClass::InputItemCheck(struct _KamokuData& ans, struct _KamokuData& check, int _dcsw, BOOL bSelClick/*=FALSE*/ )
{

	return m_Seldata.InputItemCheck( ans, check, _dcsw, bSelClick );
}



long CDnpBaseClass::ErrMsgSet( int ercd, char* msg )
{
	m_Errcd	= ercd;
	m_ErrMsg = msg;

	return 0;
}


void CDnpBaseClass::get_hjiyustr( int h_code, CString& str )
{
	m_pZm->kshnam->MoveFirst();

	do {
		if( m_pZm->kshnam->kofs > 0 )
			break;

		if( m_pZm->kshnam->hnjcod == h_code && (m_pZm->kshnam->hnpflg & 0x01) ) {
			str = m_pZm->kshnam->hnname;
			break;
		}
	} while( m_pZm->kshnam->MoveNext() == 0 );
}



//--------------------------------------------------------------
//	変動事由コードエリアより 名称を取得
//	[マスターによって 変動事由名称か、振替科目区分か]
//
//--------------------------------------------------------------
BOOL CDnpBaseClass::_get_hjiyustr( int hjcd, int nbcd, CString& str )
{
	BOOL ret = FALSE;

	if( m_pZm->zvol->apno < 0x50 ) {
		str.Empty();
		get_hjiyustr( hjcd, str );
		if( ! str.IsEmpty() )	ret = TRUE;
	}
	else {
		if( ! IsSyafuKaisei( m_pZm->zvol ) ) {
			if( m_Seldata.GetNPfuriStr( hjcd, str ) ) {
				ret = TRUE;
			}
		}
		else {
			if( m_Seldata.GetNaibuString( hjcd, nbcd, str ) ) {
				ret = TRUE;
			}
		}
	}

	return ret;
}



//------------------------------------------------------------------
//	選択科目表示
//------------------------------------------------------------------
long CDnpBaseClass::SelItemDsp( long nChar )
{
	SelPar par;

	switch( m_SelDispPN ) {
	case SL_KAMOKU_PN:
	case SL_KMKANA_PN:
	case SL_BRNTKY_PN:
	case SL_KMINPCD_PN:	// '09.03.11
		par.sel_pn = m_SelDispPN;
		m_Seldata.SelectJob( &par, nChar );

		switch( nChar ) {
		case VK_PRIOR: case VK_NEXT:
		case VK_HOME:
			nChar = 0;
			break;
		}
		break;
	}

	return nChar;
}


// 枝番摘要選択処理
int CDnpBaseClass::BRNTKY_select( int bmn, CString& code, char* kana/*=NULL*/, BOOL bDisp/*=FALSE*/ )
{
	SelPar	par;

	par.sel_pn = SL_BRNTKY_PN;
	par.sel_flg = bDisp ? 1 : 0;
	par.brn_bmn = bmn;
	par.tkybrn = code;
	par.brn_cd = -1;

	::ZeroMemory( par.brn_kana, sizeof par.brn_kana );
	if( kana != NULL )
		strcpy_s( par.brn_kana, sizeof par.brn_kana, kana );

	m_Seldata.SelectJob( &par, -1 );

	return BRNTKY_sel;
}


// 変動事由か、振替科目[公益]かのチェック
BOOL CDnpBaseClass::IsHjiyuOrNPfuri( LPCTSTR dbt, LPCTSTR cre )
{
	BOOL ret = FALSE;

	if( m_pZm->zvol->apno < 0x50 ) {
		// 変動事由
		if( get_khkbntype( dbt, cre, m_pZm ) >= 0 ) {
			ret = TRUE;
		}
	}
	else {
		if( IsSyafuKaisei( m_pZm->zvol ) ) {	//社会福祉改正
			DWORD dwCode, dwGkcd;

			dwCode = _atoh( (LPSTR)dbt );
			dwGkcd = (dwCode&0xffffff00);
			hl_rev( &dwGkcd, 4 );

		//	DBGKREC* pGk = m_pZm->get_gksrc( dwGkcd );
			DBGKREC* pGk = m_pZm->PjisToGkrec( dwGkcd );

			if( pGk != NULL ) {
				if( (pGk->gksgn& 0x80) ) {
					ret = TRUE;
				}
			}

			if( ! ret) {
				dwCode = _atoh( (LPSTR)cre );
				dwGkcd = (dwCode&0xffffff00);
				hl_rev( &dwGkcd, 4 );
				
		//		pGk = m_pZm->get_gksrc( dwGkcd );
				pGk = m_pZm->PjisToGkrec( dwGkcd );

				if( pGk != NULL ) {
					if( (pGk->gksgn& 0x80) ) {
						ret = TRUE;
					}
				}
			}
		}
		else {
			// 振替区分
			if( m_Seldata.IsNPfuridata( dbt, cre ) ) {
				ret = TRUE;
			}
		}
	}

	return ret;
}


// 内部取引消去 ページめくり
long CDnpBaseClass::SelNaibuDsp( long nChar )
{
	SelPar par;
	switch( m_SelDispPN ) {
	case SL_NAIBU_PN:
		par.sel_pn = m_SelDispPN;
		m_Seldata.SelectJob( &par, nChar );

		switch( nChar ) {
		case VK_PRIOR: case VK_NEXT:
		case VK_HOME:
			nChar = 0;
			break;
		}
		break;
	}

	return nChar;
}



// 選択番号より部門コードを仕訳へ
//
BOOL CDnpBaseClass::bmncode_todata( CDBINPDataRec* data, int number, int dc_sw, int hchk/*=0*/, CString* bmnname/*NULL*/ )
{
	BOOL ret = FALSE;
	char buf[64];

	sprintf_s( buf, sizeof buf, "%d", number );
	SelResult srt;
	srt.sel_pn = SL_BUMON_PN;

	if( m_Seldata.GetSelResult( &srt, buf ) ) {

		if( hchk ) {	//範囲チェック
			switch( dc_sw ) {
			case 0:
				if( m_BmChk.BmnHaniCheck( srt.bmn, data->m_dbt ) ) {
					data->m_dbmn = srt.bmn;
					ret = TRUE;
				}
				break;
			case 1:
				if( m_BmChk.BmnHaniCheck( srt.bmn, data->m_cre ) ) {
					data->m_cbmn = srt.bmn;
					ret = TRUE;
				}
				break;
			default:
				if( m_BmChk.BmnHaniCheck( srt.bmn, data->m_dbt ) ) {
					data->m_dbmn = srt.bmn;
					ret = TRUE;
				}
				if( m_BmChk.BmnHaniCheck( srt.bmn, data->m_cre ) ) {
					data->m_cbmn = srt.bmn;
					ret = TRUE;
				}
				break;
			}
		}
		else {
			switch( dc_sw ) {
			case 0:
				data->m_dbmn = srt.bmn;
				break;
			case 1:
				data->m_cbmn = srt.bmn;
				break;
			default:
				data->m_dbmn = srt.bmn;
				data->m_cbmn = srt.bmn;
				break;
			}
			ret = TRUE;
		}

		if( ret ) {
			if( bmnname != NULL ) {
				*bmnname = srt.selname;
			}
		}
	}

	return ret;
}


//部門表示欄ページめくり
long CDnpBaseClass::SelBmonDsp( long nChar )
{
	SelPar par;
	switch( m_SelDispPN ) {
	case SL_BUMON_PN:
	case SL_BMNKANA_PN:
		par.sel_pn = m_SelDispPN;
		m_Seldata.SelectJob( &par, nChar );

		switch( nChar ) {
		case VK_PRIOR: case VK_NEXT:
		case VK_HOME:
			nChar = 0;
			break;
		}
		break;
	}

	return nChar;
}



// 選択番号より工事コードを仕訳へ
//
//
BOOL CDnpBaseClass::kojicode_todata( CDBINPDataRec* data, CString* kjnam, int number, int dc_sw )
{
	BOOL ret = FALSE;
	char buf[64];

	SelResult srt;
	srt.sel_pn = SL_KOJI_PN;

	sprintf_s( buf, sizeof buf, "%d", number );

	if( m_Seldata.GetSelResult( &srt, buf ) ) {
		srt.kjcode.TrimRight();
		switch( dc_sw ) {
		case 0:
			data->m_dkno = srt.kjcode;
			break;
		case 1:
			data->m_ckno = srt.kjcode;
			break;
		default:
			data->m_dkno = srt.kjcode;
			data->m_ckno = srt.kjcode;
			break;
		}
		// 工事名称
		if( kjnam != NULL ) {
			*kjnam = srt.selname;
		}

		ret = TRUE;
	}

	return ret;
}


//摘要表示欄ページめくり
long CDnpBaseClass::SelTkyDsp( long nChar )
{

	SelPar par;
	switch( m_SelDispPN ) {
	case SL_TKY_PN:
	case SL_TKYKANA_PN:
	case SL_TKYALL_PN:
#ifdef _SLIP_ACCOUNTS_
	case SL_SLIP_TKY_PN:
#endif
		par.sel_pn = m_SelDispPN;
		m_Seldata.SelectJob( &par, nChar );

		switch( nChar ) {
		case VK_PRIOR: case VK_NEXT:
		case VK_HOME:
			nChar = 0;
			break;
		}
		break;
	}

	return nChar;

}


//------------------------------------------------------------
//	変動事由, 振替科目をセットできる仕訳かどうか？
//
//------------------------------------------------------------
BOOL CDnpBaseClass::IsEnableHjiyu( CDBINPDataRec *pcrec )
{
	if( m_pZm->zvol->apno < 0x50 ) {

		if( m_Seldata.GetHjiyuCount(0) < 0 ) {
			return FALSE;
		}

		int st = get_khkbntype( pcrec->m_dbt, pcrec->m_cre, m_pZm );
		if( st == -1 )
			return FALSE;

		// 変動事由コードが正しいか？
		CZmGen8		ZmGen;
		int ret = ZmGen.CheckByoinType( m_pZm );
		if( ret == ID_ZMG_H29REV_IRYO_TYPE ) {
			return IsIryoHjiyuCodeRight( pcrec->m_hjiyu, st );
		}
		else {
			return IsHenJiyuCodeRight( pcrec->m_hjiyu, st );
		}
	}
	else {
		if( IsSyafuKaisei( m_pZm->zvol ) ) {
			// 内部取引仕訳は、すべての仕訳にセット可能
			// m_Seldata.IsNaibuData( dbt, cre ) ではキー移動によるカーソルをセットするかの判定に使用
			return TRUE;
		}
		else {
			// 振替科目
			if( ! m_Seldata.IsNPfuridata( pcrec->m_dbt, pcrec->m_cre ) )
				return FALSE;

			return	m_Seldata.IsNPfuricode( pcrec->m_hjiyu );
		}
	}
}

// 選択欄の枝番表示を更新するか？
BOOL CDnpBaseClass::EdabanDispReload( CString& kmkcd )
{
	BOOL bRet = FALSE;

	if( m_DI.IsEdaAdd() ) {
		m_DI.IsEdaAdd() = FALSE;
		m_Seldata.BrnTekClear();
		return TRUE;
	}

	//現在処理中の 枝番名称の科目コード
	BRNTKY_PAR bpar;
	bpar = m_Seldata.GetBrnTkyPar();

	if( kmkcd.CompareNoCase( bpar.bt_code ) != 0 ) {
		bRet = TRUE;
	}

	return bRet;
}


// 工事名称 ページめくり
long CDnpBaseClass::SelKojiDsp( long nChar )
{
	SelPar par;
	switch( m_SelDispPN ) {
	case SL_KOJI_PN:
	case SL_KOJIKANA_PN:
		par.sel_pn = m_SelDispPN;
		m_Seldata.SelectJob( &par, nChar );

		switch( nChar ) {
		case VK_PRIOR: case VK_NEXT:
		case VK_HOME:
			nChar = 0;
			break;
		}
		break;
	}

	return nChar;
}


/******************************************
	完成工事番号 チェック
	dbcr_sg:	-1 のときは、出納帳より

		->	0 ... OK
			1 ... 修正
			-1 .. Error
*******************************************/
BOOL CDnpBaseClass::koji_kanseicheck( CString& kno, CString& code, int dbcr_sg )
{
	CString	msg;
	int st;

	if( ! m_DI.IsKoji() )
		return FALSE;

	if( m_pAsel->KANKJI_OPT == OPT_KANKJI ) {
		if( m_Seldata.Kojicode_KanChk( kno, code ) ) {

			// 未入力メッセージ
			struct _DBKNREC* pKn;
			if( ( pKn = DB_PjisToKnrec( code, m_pZm ) ) == NULL )
				return -1;

			char* p;
			switch( dbcr_sg ) {
			case 0:		p = "借方";	break;
			case 1:		p = "貸方";	break;
			default:	p = "";	break;
			}

			msg.Format( "《完成工事番号チェック》\n\n%s科目「%.14s」を使用されていますが、\r\n前期以前に完成した工事番号が入力されています。\r\n\r\n修正しますか？",
							p, pKn->knnam );

			Buzzer();

			st = myICSMessageBox( (LPCSTR)msg, MB_YESNO );

			if( st == IDYES )
				return 1;	// 修正
			else
				return 0;	// OK

		}
	}
	return 0;
}



/******************************************
	部門未入力チェック
	dbcr_sg:	-1 のときは、出納帳より

		->	0 ... OK
			1 ... 修正
			-1 .. Error

*******************************************/
BOOL CDnpBaseClass::bmn_noinpcheck( int bmn, CString& code, int dbcr_sg, CString* pMsg/*=NULL*/ )
{
	CString	msg, err;
	int st, cnt;

	if( ! m_DI.IsBumon() )
		return FALSE;

	if( bmn >= 0 )
		return FALSE;

	DWORD dwCode;

	// 損益のみ部門
	dwCode = _atoh( (LPSTR)(LPCTSTR)code );
	if( (m_pZm->zvol->bm_isw & 0x10) && dwCode < SONEKICODE )
		return FALSE;

	if( m_pAsel->KJIINP_OPT == OPT_KJIINP ) {

		cnt = bmnzan_count( m_pZm->m_database, code, err );
		if( cnt < 0 ) {
			myICSMessageBox(err);
		//	GetDBDinpMainFrame()->PostMessage( WM_CLOSE, 1 );
			return FALSE;
		}

		if( cnt > 0 ) {
			// 未入力メッセージ
			struct _DBKNREC* pKn;
			if( ( pKn = DB_PjisToKnrec( code, m_pZm ) ) == NULL )
				return -1;

			char* p;
			switch( dbcr_sg ) {
			case 0:		p = "借方";	break;
			case 1:		p = "貸方";	break;
			default:	p = "";	break;
			}

			if( pMsg == NULL ) {
				msg.Format( "《部門未入力》\n\n%s科目「%.14s」を使用されていますが、部門が未入力です。\r\n\r\n修正しますか？",
								p, pKn->knnam );

				Buzzer();

				st = myICSMessageBox( (LPCSTR)msg, MB_YESNO,0,0, m_pDnpParent );

				if( st == IDYES )
					return TRUE;	// 修正
				else
					return FALSE;	// OK
			}
			else {
				msg.Format( "%s科目「%.14s」を使用されていますが、部門が未入力です！", p, pKn->knnam );
				*pMsg = msg;
				return TRUE;
			}
		}
	}
	return FALSE;
}




// 工事コードチェック
BOOL CDnpBaseClass::Kojicode_Ok( CString& kjcode )
{
	return m_Seldata.Kojicode_Ok( kjcode );
}


/******************************************
	工事番号未入力チェック
	dbcr_sg:	-1 のときは、出納帳より

		->	0 ... OK
			1 ... 修正
			-1 .. Error

*******************************************/
BOOL CDnpBaseClass::koji_noinpcheck( CString& kno, CString& code, int dbcr_sg )
{
	CString	msg;
	int st;

	if( ! m_DI.IsKoji() )
		return FALSE;

	if( m_pAsel->KJIINP_OPT == OPT_KJIINP ) {
		if( m_Seldata.Kojicode_NoInp( kno, code ) ) {

			// 未入力メッセージ
			struct _DBKNREC* pKn;
			if( ( pKn = DB_PjisToKnrec( code, m_pZm ) ) == NULL )
				return -1;

			char* p;
			switch( dbcr_sg ) {
			case 0:		p = "借方";	break;
			case 1:		p = "貸方";	break;
			default:	p = "";	break;
			}

			msg.Format( "《工事番号未入力》\n\n%s科目「%.14s」を使用されていますが、工事番号が未入力です。\r\n\r\n修正しますか？",
							p, pKn->knnam );

			Buzzer();

			st = myICSMessageBox( (LPCSTR)msg, MB_YESNO );

			if( st == IDYES )
				return 1;	// 修正
			else
				return 0;	// OK

		}
	}
	return 0;
}


#ifdef CLOSE

// 選択画面に選択
int CDnpBaseClass::BaseSelectJob( SelPar* par, long nChar/* = -1*/ )
{
	return m_Seldata.SelectJob( par, nChar );
}

// 仮想関数
void CDnpBaseClass::OnActivateJob(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
}


#ifdef _SLIP_ACCOUNTS_
//----------------------------------------------------------
//	摘要選択
//	CDBipTKREC* ptkrec  摘要レコード
//	char *data ........ 摘要番号文字列
//	return	0 ---- 摘要レコード あり
//		   -1 ---- 摘要レコード なし
//-----------------------------------------------------------
int CDnpBaseClass::tky_select_in_img( CDBipTKREC* ptkrec, char *data )
{
	SelResult srt;
	srt.sel_pn = SL_SLIP_TKY_PN;

	if( !isdigitstr(data) ){	//数字文字列
		return -1;
	}

	if( m_Seldata.GetSelResult( &srt, data ) ) {

/*		m_pZm->tkrec->MoveFirst();

		int sgn = 0;
		if( m_pZm->tkrec->st != -1 ) {
			do {
				if( srt.tky_code == m_pZm->tkrec->tkcod ) {
					ptkrec->m_tkcod = m_pZm->tkrec->tkcod;
					ptkrec->m_tkana = m_pZm->tkrec->tkana;
					ptkrec->m_tksgn.Copy( m_pZm->tkrec->tksgn );
					ptkrec->m_tkname = m_pZm->tkrec->tkname;
					// 後ろスペースをカット
					LPSTR	p = ptkrec->m_tkname.GetBuffer(256);
					int n = kjlen( p, TKYO_LEN );
					*(p + n) = '\0';
					ptkrec->m_tkname.ReleaseBuffer();

					ptkrec->m_tkari = m_pZm->tkrec->tkari;
					ptkrec->m_tkasi = m_pZm->tkrec->tkasi;
					sgn = 1;
					break;
				}
			} while( m_pZm->tkrec->MoveNext() == 0 );
		}
		return (sgn) ? 0 : -1;*/

		ptkrec->m_tkcod = srt.tky_code;
		ptkrec->m_tkname = srt.tky;

		return 0;
	}

	return -1;

}
#endif


// 選択画面の作成
int CDnpBaseClass::MakeSelWnd( BOOL bIgnoreActive/* =FALSE */ )
{
	if (m_pSelWnd == NULL)
	{
		if( !(m_pSelWnd = new CSelWndDlg(m_pDnpParent)) )
	//	if( !(m_pSelWnd = new CSelWndDlg( GetDBDinpMainFrame() )) )
		{
			ErrMsgSet( ERROR_MEMORY, "選択ウィンドウオブジェクトのメモリ確保ができません。");
			return -1;
		}

		m_pSelWnd->m_bIgnoreActvate = bIgnoreActive;
	}
/*
	if( m_pSelWnd->GetSafeHwnd() == NULL )
	{
		if( m_pSelWnd->Create() == FALSE )
		{
			ErrMsgSet( ERROR_MEMORY, "選択ウィンドウオブジェクトが作成できません。");
			return -1;
		}
	}
*/
	return 0;
}

// 選択画面の 完全に削除
void CDnpBaseClass::SelWndDelete()
{
	if ( m_pSelWnd ) {
		delete m_pSelWnd;
		m_pSelWnd = NULL;
	}
}


// 定型仕訳
void CDnpBaseClass::RdataTblIniz( void )
{
	m_RDTATBL.RD_lbnum = -1;
	m_RDTATBL.RD_cnt = 0;
	m_RDTATBL.RD_trn = 0;
	m_RDTATBL.RD_array.RemoveAll();
}

void CDnpBaseClass::EndRdata( void )
{
	m_RDTATBL.RD_cnt = 0;
	m_RDTATBL.RD_trn = 0;
	m_RDTATBL.RD_array.RemoveAll();

	// 定型仕訳 残
	RDRemainDispJOB();
}


int CDnpBaseClass::RemainRdata( void )
{
	// 定型仕訳 残
	RDRemainDispJOB();

	return m_RDTATBL.RD_cnt - m_RDTATBL.RD_trn;
}

//
//	定型仕訳の消費税チェック
//
//

BOOL CDnpBaseClass::rd_reccheck( CDBINPDataRec *crec, CDBINPDataRec *pRDrec, DWORD& syCode )
{
	BOOL bSyzSET = FALSE;

	if( IsMasterType( MST_SYZ5_8 ) ) {
		if( crec->m_ddate < SYZ8_YMD ) {
			syCode = CD_RITU5;
			//消費税対象＋８％＋課税
			if( (pRDrec->m_dsign[0]&0x40) && (pRDrec->m_dsign[1]&0x0f) == 0x03 &&
				(pRDrec->m_dsign[2]&0x0f) == 0x01 ) {
				bSyzSET = TRUE;
			}
			else {
				int dbt_sy, cre_sy;
				dbt_sy = km_syattr( pRDrec->m_dbt, m_pZm );
				cre_sy = km_syattr( pRDrec->m_cre, m_pZm );

				if( (dbt_sy & 0x0200) || (cre_sy & 0x0200) ) {

					if( (pRDrec->m_dsign[1]&0x0f) == 0x03 ) {
						bSyzSET = TRUE;
					}
				}
			}
		}
		else {
			syCode = CD_RITU8;
			//消費税対象＋５％＋課税
			if( (pRDrec->m_dsign[0]&0x40) && (pRDrec->m_dsign[1]&0x0f) == 0x00 &&
				(pRDrec->m_dsign[2]&0x0f) == 0x01 ) {
				bSyzSET = TRUE;
			}
			else {
				int dbt_sy, cre_sy;
				dbt_sy = km_syattr( pRDrec->m_dbt, m_pZm );
				cre_sy = km_syattr( pRDrec->m_cre, m_pZm );

				if( (dbt_sy & 0x0200) || (cre_sy & 0x0200) ) {

					if( (pRDrec->m_dsign[1]&0x0f) == 0x00 ) {
						bSyzSET = TRUE;
					}
				}
			}
		}
	}

	if( m_pSy->IsKaniMaster() ) {
		if( crec->m_ddate < KANI6_YMD ) {
			//簡易課税６種 対象外？
			if( (pRDrec->m_dsign[0]&0x40) && (pRDrec->m_dsign[2]&0x0f) == 0x01 &&
				(pRDrec->m_dsign[3]&0x0f) == 0x06 ) {

				BYTE defKbn = KaniDefaultKubun( m_pZm->zvol );
				//サインOFF
				pRDrec->m_dsign[3] &= ~0x0f;
				pRDrec->m_dsign[3] |=  defKbn;	//基本業種セット
			}
		}
	}
	//リバースチャージ
	if( crec->m_ddate < RVCHAR_YMD ) {
		//簡易課税６種 対象外？
		if( (pRDrec->m_dsign[0]&0x40) && (pRDrec->m_dsign[2]&0x0f) == 0x01 &&
			((pRDrec->m_dsign[4]) == 0x1d ||(pRDrec->m_dsign[4]) == 0x1e) ) {

			//サインOFF
			pRDrec->m_dsign[4] = 0;

			int dbt_kbn, cre_kbn;
			dbt_kbn = m_pZm->km_zeikbn( pRDrec->m_dbt );
			cre_kbn = m_pZm->km_zeikbn( pRDrec->m_cre );

			if( dbt_kbn < 0 && cre_kbn < 0 ) {
				::ZeroMemory( &pRDrec->m_dsign[0], SZ_SYZDSGN );
			}
			else {
				if( dbt_kbn > 0 || cre_kbn > 0 ) {
					syCode = CD_SOTOZ;
				}
				else {
					syCode = CD_UTIZ;
				}
				bSyzSET = TRUE;
			}
		}
	}

	return bSyzSET;
}



BOOL CDnpBaseClass::GetRdata( CDBINPDataRec* rdrec, CDBINPDataRec* pCrec, CDBINPDataRec* pSrec, BOOL* cnd )
{
	if( RemainRdata() > 0 )
	{
		int pos;
		pos = m_RDTATBL.RD_array[ m_RDTATBL.RD_trn++ ];
		Mkrd.data->SetAbsolutePosition( pos );

		rdrec->m_seq	= pCrec->m_seq;
		rdrec->m_ksign	= pSrec->m_ksign;
		rdrec->m_ddate	= pSrec->m_ddate;
		rdrec->m_mofs	= pSrec->m_mofs;
		rdrec->m_cno	= -1;
		rdrec->m_dbmn	= Mkrd.data->dbmn;
		rdrec->m_dkno	= Mkrd.data->dkno;
		rdrec->m_dkno.TrimRight();
		rdrec->m_dbt	= Mkrd.data->dbt;
		rdrec->m_dbt.TrimRight();
		rdrec->m_dbr	= Mkrd.data->dbr;

		rdrec->m_cbmn	= Mkrd.data->cbmn;
		rdrec->m_ckno	= Mkrd.data->ckno;
		rdrec->m_ckno.TrimRight();
		rdrec->m_cre	= Mkrd.data->cre;
		rdrec->m_cre.TrimRight();
		rdrec->m_cbr	= Mkrd.data->cbr;

		// 09.16 /08
		int type =  IsKojinGyosyuMaster( m_pZm );
		if( type ) {
			struct _KamokuData  akd = {0}, ckd = {0};

			strcpy_s( ckd.kd_code, sizeof ckd.kd_code, rdrec->m_dbt );
			ckd.kd_sgn = 2;	// 科目コードチェック用に 枝番のみのサインセット
			if( ! m_Seldata.InputItemCheck( akd, ckd, 0 ) ) {
				rdrec->m_dbt.Empty();
				rdrec->m_dbr = -1;
			}

			::ZeroMemory( &akd, sizeof akd );
			::ZeroMemory( &ckd, sizeof ckd );
			strcpy_s( ckd.kd_code, sizeof ckd.kd_code, rdrec->m_cre );
			ckd.kd_sgn = 2;	// 科目コードチェック用に 枝番のみのサインセット
			if( ! m_Seldata.InputItemCheck( akd, ckd, 0 ) ) {
				rdrec->m_cre.Empty();
				rdrec->m_cbr = -1;
			}
		}
		//不使用チェック -- 04.13 /10
		DBKNREC* pKn;
		if( ! rdrec->m_dbt.IsEmpty() ) {
			pKn = DB_PjisToKnrec( rdrec->m_dbt, m_pZm );
			if( pKn == NULL || (pKn->knvoid & 0x01) ) {
				rdrec->m_dbt.Empty();
				rdrec->m_dbr = -1;
			}
		}
		if( ! rdrec->m_cre.IsEmpty() ) {
			pKn = DB_PjisToKnrec( rdrec->m_cre, m_pZm );
			if( pKn == NULL || (pKn->knvoid & 0x01) ) {
				rdrec->m_cre.Empty();
				rdrec->m_cbr = -1;
			}
		}

		l_input( rdrec->m_val, (void*)(LPCTSTR)Mkrd.data->val );
		cnd[0] = Mkrd.data->valflag;
		l_input( rdrec->m_zei, (void*)(LPCTSTR)Mkrd.data->zei );
		cnd[1] = Mkrd.data->zeiflag;

		rdrec->m_dsign.Copy( Mkrd.data->dsign );
		rdrec->m_dsign[0] &= (BYTE)~0x80;	//イメージサインを落とす。
		rdrec->m_dsign[15] &= (BYTE)~0x01;	// 定型仕訳サイン

		rdrec->m_owner	= -1;
		rdrec->m_dtype	= 0 /*Mkrd.data->dtype*/;//定型仕訳は、通常入力のみなので ０

		rdrec->m_tekiyo	= Mkrd.data->tekiyo;
		rdrec->m_imgsq	= -1;
		rdrec->m_rnumber	= "";
		rdrec->m_tdate	= -1;
		rdrec->m_edate	= -1;
		rdrec->m_udate	= -1;

		rdrec->m_icno	= -1;
		rdrec->m_skn		= "";
		rdrec->m_tag		= "";

		rdrec->m_hjiyu	= Mkrd.data->hjiyu;
		l_input( rdrec->m_taika, (void*)(LPCTSTR)Mkrd.data->taika );

		return TRUE;
	}
	else
	{
		// 定型仕訳取消
		EndRdata();
		return FALSE;
	}
}


void CDnpBaseClass::CalcMouseEvent()
{
	mouse_event( MOUSEEVENTF_LEFTDOWN, 0,0,0,0 );
	mouse_event( MOUSEEVENTF_LEFTUP, 0,0,0,0 );
}


#endif


// get line data adr.
struct _DP_DATA_LINE *CDnpBaseClass::getLINE_DATA( int line )
{
	return( &DNPDT_LINE[ line-1 ] );
}


// 上段データ検索
//	int ln ... データ上の　行番号(1～100)
//
struct _DenpData *CDnpBaseClass::_getPRDATA( int ln, int pn )
{
	// 直前の行のみ複写に変更 [From管理課] <01.31 /07>
	ln--;
	if( ln >= 1 ) {
		if( _getCND( ln, pn )->INP_sg )
		{
			// found
			return _getDATA( ln, pn );
		}
	}
	return NULL;
}

//データゲット
struct _DenpData *CDnpBaseClass::getDATA( int pn )
{
	return _getDATA( /*get_nowln()*/get_dataline(), pn );
}

//データゲット ライン指定
struct _DenpData *CDnpBaseClass::_getDATA( int ln, int pn )
{
	switch( pn ) {
		case DP_DENP_PN:	return &DENP_DATE_LINE.DENP_DATA;
		case DP_MM_PN:		return &DENP_DATE_LINE.MM_DATA;
		case DP_DD_PN:		return &DENP_DATE_LINE.DD_DATA;
		case DP_SYONUM_PN:	return &DENP_DATE_LINE.SYONUM_DATA;
		case DP_BMN_PN:		return &DENP_DATE_LINE.BMN_DATA;
		case DP_KOJI_PN:	return &DENP_DATE_LINE.KOJI_DATA;
	}
	return _getDATAs( getLINE_DATA( ln ), pn);
}

//データゲット ライン指定（サブ）
struct _DenpData *CDnpBaseClass::_getDATAs( struct _DP_DATA_LINE *pd, int pn )
{
	switch( pn ) {
		case FD_DBVAL_PN:	return &pd->DBVAL_DATA;
		case FD_DBZEI_PN:	return &pd->DBZEI_DATA;
		case FD_DBMN_PN:	return &pd->DBMN_DATA;
		case FD_DKOJI_PN:	return &pd->DKOJI_DATA;
		case FD_DEBT_PN:	return &pd->DEBT_DATA;
		case FD_CRVAL_PN:	return &pd->CRVAL_DATA;
		case FD_CRZEI_PN:	return &pd->CRZEI_DATA;
		case FD_CBMN_PN:	return &pd->CBMN_DATA;
		case FD_CKOJI_PN:	return &pd->CKOJI_DATA;
		case FD_CRED_PN:	return &pd->CRED_DATA;
		case FD_TKY_PN:		return &pd->TKY_DATA;
		case FD_DSKBN_PN:	return &pd->DSKBN_DATA;
		case FD_CSKBN_PN:	return &pd->CSKBN_DATA;
		case FD_TGNUM_PN:	return &pd->TGNUM_DATA;
		case FD_TGDATE_PN:	return &pd->TGDATE_DATA;
		case FD_HJIYU_PN:	return &pd->DPHJYU_DATA;
		case FD_HUSEN_PN:	return &pd->DPHSEN_DATA;

		case FD_DSYOG_PN:	return &pd->DSYOG_DATA;
		case FD_CSYOG_PN:	return &pd->CSYOG_DATA;
		default:		break;
	}
	return NULL;
}

//データコンディションサインゲット
struct _DNPLINE_CND *CDnpBaseClass::getCND( int pn )
{
	return _getCND( /*get_nowln()*/ get_dataline(), pn );
}

//データコンディションサインゲット ライン指定
struct _DNPLINE_CND *CDnpBaseClass::_getCND( int ln, int pn )
{
	if( pn >= FD_DBVAL_PN )
		return( &DNPDT_LINE[ ln-1 ].LINE_CND[pn-FD_DBVAL_PN] );
	else
		return( &DENP_DATE_LINE.LINE_CND[ pn ] );
}

//データコンディションサインゲット ライン指定
struct _DNPLINE_CND *CDnpBaseClass::_getCNDs( struct _DP_DATA_LINE *pd, int pn )
{
	if( pn >= FD_DBVAL_PN )
		return( &pd->LINE_CND[pn-FD_DBVAL_PN] );
	else
		return( &DENP_DATE_LINE.LINE_CND[ pn ] );
}


/////////////////////////////////
//
//	データマップアクセス関係
//
/////////////////////////////////
//
// 仕訳データマップイニシャライズ
void CDnpBaseClass::inzMAP()
{
int i;
int	max = get_MaxLine();

	for( i = 0 ; i < max ; ++i )
		DATA_MAP[i][0] = DATA_MAP[i][1] = -1;
}

//
// 仕訳データマップ借貸サイン変換
int CDnpBaseClass::cvtMAPdb( int pn )
{
	switch( pn )
	{
	case FD_DEBT_PN: return 0;
	case FD_CRED_PN: return 1;
	}
	return -1;
}

// 
// 仕訳データマップサインセット
void CDnpBaseClass::setMAP( int pn )
{
	_setMAP( get_dataline(),/*get_nowln()*/ pn );
}

void CDnpBaseClass::_setMAP( int line, int pn )
{
int db_sgn;
int i, ln;
short now_sgn;

	ln = line-1;

	if( (db_sgn = cvtMAPdb( pn )) == -1 )	// 借貸決定
		return;

	int	max = get_MaxLine();

	now_sgn = DATA_MAP[ln][db_sgn];
	for( i = ln ; i < max ; ++i )
	{
		if( DATA_MAP[i][db_sgn] == now_sgn )
			DATA_MAP[i][db_sgn] = line;
		else
			break;
		if( i + 1 == now_sgn )
			break;

		//次のサインをチェック
		if( (i+1) < max ) {
			if( DATA_MAP[i + 1][db_sgn] > line ) {
				break;
			}
		}
	}

	if( now_sgn >= line || now_sgn == -1 )
		for( i = ln - 1 ; i >= 0 ; --i )
		{
			if( DATA_MAP[i][db_sgn] == now_sgn )
				DATA_MAP[i][db_sgn] = line;
			else
				break;
		}
}

//
// 仕訳データマップサインゲット
int CDnpBaseClass::getMAP( int pn )
{
	return _getMAP( get_dataline()/*get_nowln()*/, pn );
}

int CDnpBaseClass::_getMAP( int ln, int pn )
{
int db_sgn;

	if( (db_sgn = cvtMAPdb( pn )) != -1 )
		return DATA_MAP[ln-1][db_sgn];
	else
		return -1;
}

//
// 仕訳データマップサイン取消
void CDnpBaseClass::delMAP( int pn )
{
	_delMAP( get_dataline()/*get_nowln()*/, pn );
}

void CDnpBaseClass::_delMAP( int line, int pn )
{
int i,j;
short now_sgn, set_sgn;
int db_sgn, ln;

	ln = line-1;

	if( (db_sgn = cvtMAPdb( pn )) == -1 )
		return;

	now_sgn = DATA_MAP[ln][db_sgn];
	if( line != now_sgn )
		return;

int	max = get_MaxLine();

	set_sgn = ln > 0 ? DATA_MAP[ln-1][db_sgn] : now_sgn;
	for( i = ln ; i < max ; ++i )
	{
		if( DATA_MAP[i][db_sgn] == now_sgn )
			DATA_MAP[i][db_sgn] = set_sgn;
		else
			break;
	}

	set_sgn = i < max ? DATA_MAP[i][db_sgn] : now_sgn;
	for( j = i - 1 ; j >= 0 ; --j )
	{
		if( DATA_MAP[j][db_sgn] == now_sgn )
			DATA_MAP[j][db_sgn] = set_sgn;
		else
			break;
	}

	//すべてが同じサインになった場合は､何も無しとする。
	if( (i >= max) && (j < 0) )
	{
		for( i = 0 ; i < max ; ++i )
			DATA_MAP[i][db_sgn] = -1;
	}
}


// データマップ経由の借貸データゲット
struct _DenpData *CDnpBaseClass::getMAPtoDATA( int pn )
{
	return _getMAPtoDATA( get_dataline()/*get_nowln()*/, pn );
}

// getMAPtoDATA() のサブ
struct _DenpData *CDnpBaseClass::_getMAPtoDATA( int ln, int pn )
{
int lnx;

	lnx = _getMAP( ln, pn );	

	if( lnx != -1 )
		return _getDATA( lnx, pn );
	else
		return 0;
}


//
// 仕訳データマップサイン挿入
//		データバッファに挿入する前に呼び出す。
//
void CDnpBaseClass::_insMAP( int line )
{
int i;
short dbt_now_sgn, dbt_set_sgn, dbt_nxt_sgn, dbt_kminp;
short cre_now_sgn, cre_set_sgn, cre_nxt_sgn, cre_kminp;

int lidx, kmk_sgn;

	lidx = line-1;

	// 借方・貸方 同時に行う
	dbt_now_sgn = DATA_MAP[lidx][0];
	cre_now_sgn = DATA_MAP[lidx][1];

	// 挿入位置には、前行サイン
	dbt_set_sgn = lidx > 0 ? DATA_MAP[lidx-1][0] : dbt_now_sgn;
	cre_set_sgn = lidx > 0 ? DATA_MAP[lidx-1][1] : cre_now_sgn;

	dbt_kminp = cre_kminp = 0;

	// 挿入行の借方科目入力のチェック
	kmk_sgn = _getCND( line, FD_DEBT_PN )->INP_sg;
	// 挿入行に科目入力がなければ 挿入行の次の行は、挿入前行のサイン
	// 科目が入力されていたら、挿入行の次の行は、挿入次行のサインに変更(+1)

	if( dbt_now_sgn > line ) {	//挿入行の後に、科目入力ラインがある場合。
		dbt_kminp = 1;
	}
	if( ! kmk_sgn ) {
		if( ! dbt_kminp ) {
			dbt_nxt_sgn = dbt_now_sgn;
		}
		else {
			dbt_nxt_sgn = dbt_now_sgn + 1 ;
		}
	}
	else	dbt_nxt_sgn = dbt_now_sgn+1;

	//貸方
	kmk_sgn = _getCND( line, FD_CRED_PN )->INP_sg;

	if( cre_now_sgn > line ) {	//挿入行の後に、科目入力ラインがある場合。
		cre_kminp = 1;
	}
	if( ! kmk_sgn )	{
		if( ! cre_kminp ) {
			cre_nxt_sgn = cre_now_sgn;
		}
		else {
			cre_nxt_sgn = cre_now_sgn + 1;
		}
	}
	else {
		cre_nxt_sgn = cre_now_sgn+1;
	}

int	max = get_MaxLine();

	for( i = (max-1); i >= lidx; i-- ) 
	{
		if( i == lidx ) {	//挿入行
			DATA_MAP[i][0] = dbt_set_sgn;
			DATA_MAP[i][1] = cre_set_sgn;
		}
		else if( i == (lidx+1) ) {	//挿入行＋１の位置
			DATA_MAP[i][0] = dbt_nxt_sgn;
			DATA_MAP[i][1] = cre_nxt_sgn;
		}
		else {
			int map_sgn = DATA_MAP[i-1][0];
			// 借方
			if( map_sgn == -1 ) {
				DATA_MAP[i][0] = -1;
			}
			else {
				if( map_sgn != dbt_set_sgn || dbt_kminp ) {
					DATA_MAP[i][0] = map_sgn+1;
				}
				else {
					DATA_MAP[i][0] = dbt_nxt_sgn;
				}
			}
			// 貸方
			map_sgn = DATA_MAP[i-1][1];
			if( map_sgn == -1 ) {
				DATA_MAP[i][1] = -1;
			}
			else {
				if( map_sgn != cre_set_sgn || cre_kminp ) {
					DATA_MAP[i][1] = map_sgn+1;
				}
				else {
					DATA_MAP[i][1] = cre_nxt_sgn;
				}
			}
		}
	}

	// 挿入行の後に科目入力ラインの時は、挿入分プラスする。
	if( lidx >= 0 && (dbt_kminp || cre_kminp) ) {
		for( i = lidx; i >= 0; i-- ) 
		{
			int map_sgn = DATA_MAP[i][0];
			// 借方
			if( map_sgn > line ) {
				DATA_MAP[i][0] = map_sgn+1;
			}
			// 貸方
			map_sgn = DATA_MAP[i][1];
			if( map_sgn > line ) {
				DATA_MAP[i][1] = map_sgn+1;
			}
		}
	}
}


//
// 仕訳データマップサイン削除 行単位
//		空白行を削除する場合に呼び出す。
//
void CDnpBaseClass::_delLineMAP( int line )
{
int i;
short dbt_now_sgn, dbt_set_sgn;
short cre_now_sgn, cre_set_sgn;

int lidx;
int	max = get_MaxLine();

	lidx = line-1;

	// 借方・貸方 同時に行う
	dbt_now_sgn = DATA_MAP[lidx][0];
	cre_now_sgn = DATA_MAP[lidx][1];

	// 削除位置には、次行サイン
	dbt_set_sgn = lidx < (max-1) ? DATA_MAP[lidx+1][0] : dbt_now_sgn;
	cre_set_sgn = lidx < (max-1) ? DATA_MAP[lidx+1][1] : cre_now_sgn;

	for( i = lidx; i < max; i++ ) {
		int map_sgn;
		if( i == (max-1) )	map_sgn = DATA_MAP[i][0];		//最終
		else				map_sgn = DATA_MAP[i+1][0];

		// 借方
		if( map_sgn == -1 ) {
			DATA_MAP[i][0] = -1;
		}
		else {
		//	if( map_sgn != dbt_now_sgn ) {
			if( map_sgn > line ) {
				DATA_MAP[i][0] = map_sgn-1;
			}
			else {
				DATA_MAP[i][0] = dbt_now_sgn;
			}
		}

		map_sgn = DATA_MAP[i+1][1];
		// 貸方
		if( map_sgn == -1 ) {
			DATA_MAP[i][1] = -1;
		}
		else {
		//	if( map_sgn != cre_now_sgn ) {
			if( map_sgn > line ) {
				DATA_MAP[i][1] = map_sgn-1;
			}
			else {
				DATA_MAP[i][1] = cre_now_sgn;
			}
		}
	}
}



//----------------------------------------------------
//	1 伝票あたりの最大データ行数を取得
//
//----------------------------------------------------
int	CDnpBaseClass::get_MaxLine()
{
	int max = DP_SCROLL_NL;

	if( IsKind() ) {
		if( IsJob() == JOB_SCAN ) {
			if( SyogSiwakeMode() )	max = DP_SCROLL_MAX;
		}
	}

	return max;
}

//----------------------------------------------------
//	伝票ごとのデータ行数を取得 [１画面あたりの ]
//
//----------------------------------------------------
int	CDnpBaseClass::getSCROLL_NL()
{
	switch( IsKind() ) {
		case	KIND_NYUKIN:	return	DP_SCROLL_NL_N;	break;
		case	KIND_SYUKKIN:	return	DP_SCROLL_NL_S;	break;
		default:				return	DP_SCROLL_NL_F;	break;
	}
}


//--------------------------------------------------
//	現在行 の科目コードを取得
//	現在行に入力されていない場合は、支配科目
//
//	int ln ... 画面伝票上の　行番号(１～最大７)
//
//--------------------------------------------------
void CDnpBaseClass::get_linekamokucode( int ln, CString& dbt, CString& cre )
{
	int DEBT_ln, CRED_ln;

	get_linekamokucodeEX( ln, dbt, cre, DEBT_ln, CRED_ln );
}

//--------------------------------------------------
//	int ln ... 画面伝票上の　行番号(１～最大７)
//
//	int& dbtln	
//	int& creln	科目の記入があるデータ行番号(1～100)
//--------------------------------------------------
void CDnpBaseClass::get_linekamokucodeEX( int ln, CString& dbt, CString& cre, int& dbtln, int& creln )
{
	int DEBT_ln, CRED_ln;
	int dt_ln = get_dataline( ln );

	DEBT_ln = _getMAP( dt_ln, FD_DEBT_PN ),
	CRED_ln = _getMAP( dt_ln, FD_CRED_PN );

	dbtln = creln = -1;

	if( _getCND( dt_ln, FD_DEBT_PN )->INP_sg ) {
		dbt = _getDATA( dt_ln, FD_DEBT_PN )->DP_CdNum;
		dbtln = ln;
	}
	else if( DEBT_ln != -1 ) {
		dbt = _getDATA( DEBT_ln, FD_DEBT_PN )->DP_CdNum;
		dbtln = DEBT_ln;
	}
	else	dbt.Empty();

	if( _getCND( dt_ln, FD_CRED_PN )->INP_sg ) {
		cre = _getDATA( dt_ln, FD_CRED_PN )->DP_CdNum;
		creln = ln;
	}
	else if( CRED_ln != -1 ) {
		cre = _getDATA( CRED_ln, FD_CRED_PN )->DP_CdNum;
		creln = CRED_ln;
	}
	else	cre.Empty();

}


//------------------------------------------------
//	現在データ行位置 取得(1 から １００)
//
//------------------------------------------------
int CDnpBaseClass::get_dataline()
{
	return get_nowln() + m_TopIdx;
}

//-----------------------------------------------------
//	現在データ行位置 取得(1 から １００)
//	int ln ... 伝票上の行数(１から７行[入出金は６])
//
//-----------------------------------------------------
int CDnpBaseClass::get_dataline( int ln )
{
	if( ln <= 0 )	return -1;

	return m_TopIdx + ln;
}


//------------------------------------------------
//	現在行 取得 
//	伝票上の行数まで(0=ヘッド部)
//					(１から７行[入出金は６])
//
//------------------------------------------------
int CDnpBaseClass::get_nowln()
{
	return m_DPDATA_LN;
}

//------------------------------------------------
//	現在行 セット(0からはじまる)
//	伝票上の行数まで(0=ヘッド部)
//					(１から７行[入出金は６])
//
//------------------------------------------------
void CDnpBaseClass::set_nowln( int ln )
{
//TRACE( "set_nowln = %d\n", ln );

	m_DPDATA_LN = ln;
}

//---------------------------------
//	ポジションサイン取得
//
//---------------------------------
int CDnpBaseClass::get_nowpn()
{
	return m_NOW_PN;
}

//---------------------------------
//	ポジションサインセット
//
//---------------------------------
void CDnpBaseClass::set_nowpn( int pn )
{
	m_NOW_PN = pn;
}


//---------------------------------------------------
//	入力位置へフォーカスセット
//		int ln		表示行(1より)
//		int pn		ポジションサイン
//		int vect	行の移動方向( 1 = 下, -1 = 上)
//
//	return	 0 = 新しくフォーカスセットできた。
//			-1 = 該当入力位置にはセットできない
//---------------------------------------------------
int CDnpBaseClass::set_position( int ln, int pn, int vect )
{
	DNP_POS* pos;
	pos = GetDNP_POS( 1, pn );
	int dt_ln;
	int org_ln = ln;
	int tenso_cnt = 0;

	if( (ln >= 1 && ln <= getSCROLL_NL()) && pos->dp_enable )
	{
		dt_ln = get_dataline( ln );

		if( IsJob() == JOB_SCAN ) {
			//転送済み仕訳
			while( 1 ) {
				if( _getCND( dt_ln, pn )->EDT_sg <= 1 )	break;
				else {
					tenso_cnt++;
				}
				if( vect > 0 )	ln++;
				else			ln--;

				if( ln > getSCROLL_NL() ) {
					if( tenso_cnt == (ln - org_ln) ) {
						ln = org_ln;
						dt_ln = get_dataline( ln );
						break;
					}
					return	-1;
				}
				if( ln <= 0 ) {
					if( tenso_cnt == org_ln ) {
						ln = org_ln;
						dt_ln = get_dataline( ln );
						break;
					}
					return	-1;
				}
				dt_ln = get_dataline( ln );
			}
			//取消ライン
			while( 1 ) {
				if( ! m_DPscan.DeleteLine( dt_ln ) )	break;
				else {
					if( IsKind() == KIND_FURIKAE ) {
						if( pn == FD_DBVAL_PN )	break;
					}
					else if( IsKind() == KIND_NYUKIN ) {
						if( pn == FD_CRED_PN )	break;
					}
					else if( IsKind() == KIND_SYUKKIN ) {
						if( pn == FD_DEBT_PN )	break;
					}
				}
				if( vect > 0 )	ln++;
				else			ln--;
				if( ln > getSCROLL_NL() )	return	-1;
				if( ln <= 0 )	return	-1;

				dt_ln = get_dataline( ln );
			}
		}

		if( pn == FD_TKY_PN && IsTekiyoImage( ln ) ) {
			SetFocusToImage( ln, FALSE );
		}
		else {
			if( IsKind() == KIND_FURIKAE ) {
				// 単一部門入力
				if( pn == FD_DBMN_PN || pn == FD_CBMN_PN ) {
					if( m_FuriMode == FURI_TANBMON )
						return -1;
				}
			}

			_set_focus( ln, pn );
		}

		return 0;
	}
	else
	{
		return -1;
	}
}



//
//	データスクロール時のイメージデータセット
//
int	CDnpBaseClass::ImgTekiyoSet( CDnpBaseDlg* pDlg, int dt_ln, int ln )
{
	int idx = dt_ln - 1;
	IMREC	*imp;
	int ret = 0;

	if( IsKind() == KIND_FURIKAE ) {

		if( tblImage[idx][0] ) {
			if( tblImageOfst[idx][0] == -1 ) {
				pDlg->SetImage( 0, ln, NULL );
			}
			else {
				imp = NULL;

				if ( !m_pZm->image->SetAbsolutePosition(  tblImageOfst[idx][0] ) )
					imp	= m_pZm->image->DecodeImage();
				pDlg->SetImage( 0, ln, imp );
			}

			ret++;;
		}

		if( tblImage[idx][1] ) {
			if( tblImageOfst[idx][1] == -1 ) {
				pDlg->SetImage( 1, ln, NULL );
			}
			else {
				imp = NULL;

				if ( !m_pZm->image->SetAbsolutePosition(  tblImageOfst[idx][1] ) )
					imp	= m_pZm->image->DecodeImage();
				pDlg->SetImage( 1, ln, imp );
			}

			ret++;;
		}
	}
	else {
		if( tblImage[idx][0] ) {
			if( tblImageOfst[idx][0] == -1 ) {
				pDlg->SetImage( 0, ln, NULL );
			}
			else {
				imp = NULL;

				if ( !m_pZm->image->SetAbsolutePosition(  tblImageOfst[idx][0] ) )
					imp	= m_pZm->image->DecodeImage();
				pDlg->SetImage( 0, ln, imp );
			}

			ret++;;
		}
	}

	return ret;
}


void CDnpBaseClass::KamokuString( DBKNREC* pKn, CString& string )
{
	KamokuString( pKn, string.GetBuffer(64), 64 );
	string.ReleaseBuffer();
}

void CDnpBaseClass::KamokuString( DBKNREC* pKn, char* buf, int bufsize )
{
	if( m_pAsel->SCNCD_OPT == OPT_SCNCD ) {
		kmkcode_string( 2, pKn, buf, bufsize );
	}
	else {
		sprintf_s( buf, bufsize, "%.14s", pKn->knnam );
	}
}



//--------------------------------------------------
// 伝票ダイアログより フォーカス時の仕事のくくり
//	int displn		今回のライン
//	int pn			今回のポジション
//	CRect			今回のポジション矩形(スクリーン座標)
//--------------------------------------------------
int CDnpBaseClass::ToViewFocusJob( int displn, int pn, CRect rect )
{
	int line;

//MyTrace( "@ToViewFocusJob top displn %d, pn = %d m_bIgnoreFocus %d\n", displn, pn, m_bIgnoreFocus );

	if( m_bIgnoreFocus ) {
		m_bIgnoreFocus = FALSE;
		return 0;
	}

#ifndef NEWDATE_CLOSE
	// 日付チェック＆アクション
	if( pn != DP_DENP_PN && pn != DP_MM_PN && pn != DP_DD_PN ) {
		if( DateCheck_and_action() == -1 )
			return 0;
	}
#endif

	// 選択画面
	SelectDisp( displn, pn );

	line = (displn == -1) ? get_nowln() : displn;

	OnFocusJob( line, pn, rect );

	if( get_nowln() != line ) {
		LineChgJob( m_CREC, line, pn );
		bImgStaticUpdate = TRUE;	//イメージスタティックセットフラグ
	}
	else {
		// 消費税Init
		int cnt = mkSY_CREC( m_CREC, line );
		if( cnt > 0 )
			DnpSyzInit( m_CREC, cnt, pn, get_nowpn() );

		if( pn != get_nowpn() ) {
			bImgStaticUpdate = TRUE;
		}
	}

	set_nowpn( pn );
	set_nowln( line );

	int linfo;
	if( displn == -1 ) {
		if( m_bDispLno ) {
			linfo = get_dataline( 1 );
			GetNowDnpDialog()->DispLineInfo( linfo );
		}
		else {
			m_bDispLno = TRUE;
		}
	}
	else {
		linfo = get_dataline( line );
		GetNowDnpDialog()->DispLineInfo( linfo );
	}

#ifdef NEWDATE_CLOSE
	// 日付チェック＆アクション
	if( pn != DP_MM_PN && pn != DP_DD_PN ) {
		if( DateCheck_and_action() == -1 )
			return 0;
	}
#endif

	if( pn == FD_DSKBN_PN || pn == FD_CSKBN_PN ) {
		if( SkbnInDisp( line, pn, rect ) != 0 ) {
			// 空の消費税区分欄表示
			m_pDnpParent->PostMessage( WM_SKBN_ICHIRN, (WPARAM)0, (LPARAM)0 );
			Destroy_SyzInp();
		}
		Destroy_HjiyuInp();
		Destroy_HusenInp();
	}
	else {
		// 消費税区分入力非表示
		Destroy_SyzInp();
		Destroy_HjiyuInp();
		Destroy_HusenInp();

		if( pn == FD_HJIYU_PN ) {
			FocusHjiyu( line, rect );
		}
		else if( pn == FD_HUSEN_PN ) {
			FocusHusen( line, rect );
		}
		else if( pn == FD_TKY_PN ) {
			// イメージ行のときは、イメージにフォーカスをあわせる
			if( IsJob() == JOB_SCAN && IsTekiyoImage( displn ) ) {
				SetFocusToImage( displn, 0 );
			}
		}
	}
	disp_change();

	int dt_ln = get_dataline( displn );

	// 枝番名称を表示時は、科目名称＋枝番に戻す
	if( pn == FD_DEBT_PN || pn == FD_CRED_PN ) {
		SETMEISYO sm = {0};
		char edabf[30];

		if( _getCND( dt_ln, pn )->INP_sg ) {
			set_codestr( edabf, sizeof edabf, _getDATA( dt_ln, pn )->DP_BRN, m_pZm->zvol->edcol );
		//	m_pZm->EdabanToStr( edabf, sizeof edabf, _getDATA( dt_ln, pn )->DP_BRN );
			sprintf_s( sm.sm_name, sizeof sm.sm_name, "%.14s%s", _getDATA( dt_ln, pn )->DP_KNAM, edabf );

			m_bIgnoreFocus = TRUE;	// この関数がもう一度呼ばれるのを防ぐ
			GetNowDnpDialog()->SetKnam( displn, pn, &sm, TRUE );
		}
	}
	else if( pn == FD_DBMN_PN || pn == FD_CBMN_PN || pn == DP_BMN_PN ) {
#ifdef OLD_BMON_CLOSE
		if( _getCND( dt_ln, pn )->INP_sg ) {
			if( _getDATA( dt_ln, pn )->DP_BMN != -1 ) {
				m_bIgnoreFocus = TRUE;	// この関数がもう一度呼ばれるのを防ぐ
			}
			GetNowDnpDialog()->SetBmon( displn, pn, _getDATA( dt_ln, pn )->DP_BMN, NULL, TRUE );
		}
#endif
	}
#ifdef OLD_KOJI_CLOSE
	else if( pn == FD_DKOJI_PN || pn == FD_CKOJI_PN || pn == DP_KOJI_PN ) {
		if( _getCND( dt_ln, pn )->INP_sg ) {
			if( _getDATA( dt_ln, pn )->DP_KOJI[0] != '\0' ) {
				m_bIgnoreFocus = TRUE;	// この関数がもう一度呼ばれるのを防ぐ
			}
			CString kno;
			kno = _getDATA( dt_ln, pn )->DP_KOJI;
			GetNowDnpDialog()->SetKoji( displn, pn, kno, NULL, TRUE );
		}
	}
#endif
	return 0;
}

//------------------------------------------
// 月　入力＆オフフォーカス
//------------------------------------------
long CDnpBaseClass::MM_Inp( int pn, long nChar, BYTE mnth ) 
{
	// オフフォーカス
	if( !nChar )
	{
		// データＳＥＴ
		MM_Set( pn, mnth );
		kesyu_chkdsp();

		return(0);
	}
	else
	{
		if( nChar == VK_TAB && is_SHIFT() )
			return 0;
	}

	// 入力
	if( nChar == VK_RETURN || nChar == VK_TAB )
	{
	}

	return( nChar );
}

// 月　データＳＥＴ
//
void CDnpBaseClass::MM_Set( int pn, BYTE mnth ) 
{
	struct _DenpData *keybuf = getDATA( pn );

	if( pn < 0 )	return;

	CDBINPDataRec	tmpRec;
	setDATE_CREC( &tmpRec );

	if( mnth )
	{
		getCND( pn )->INP_sg = TRUE;
		keybuf->DP_BCDDAY[0] = mnth;
	}
	else {
		keybuf->DP_BCDDAY[0] = 0;
		getCND( pn )->INP_sg = FALSE;
	}
	// 伝票ダイアログへデータをセット
	GetNowDnpDialog()->SetMM( DP_MM_PN, mnth );

	//データセット
//	Data_Set( getINP_CTL( pn )->pIDC->IDC_X, keybuf );

	// 消費税チェック
	date_syzcheck( &tmpRec );
}


// 日付変更時の 消費税チェック
void CDnpBaseClass::date_syzcheck( CDBINPDataRec* prec )
{
	int ln, dt_ln, disp_ln;
	int lntype;

//	ln = get_nowln();

	// DLine_Chg では、ライン数表示更新のため、月にフォーカスをセットするが、
	// 日付変更がないため、処理しない。
	if( m_bDLineMMinpIgnore ) {
		m_bDLineMMinpIgnore = FALSE;
		return;
	}

	ln = 0;
	lntype = 1;
	disp_ln = 1 + m_TopIdx;	//画面伝票上の先頭データ番号

#ifdef CLOSE
	// 消費税関係再セット（現在行の次から始めて最終に現在行になるように全行チェック）
	for( ln = get_nowln() + 1, i = 0 ; i < getSCROLL_NL(); ++i )
#endif
	for( dt_ln = 1; dt_ln <= get_MaxLine(); dt_ln++ )
	{
#ifdef SCROL_CLOSE
		ln = ln <= getSCROLL_NL() ? ln : 1;
#endif

		if( disp_ln == dt_ln ) {	// 画面伝票上のデータ１行目と同じ
			lntype = 0;
			ln = 1;
		}
		else {
			if( ! lntype ) {
				ln++;

				if( ln > getSCROLL_NL() ) {
					lntype = 1;
					ln = 0;
				}
			}
		}

		int cnt, n, syzpn;
		struct _SY_MSG_PACK2 smpk;
		cnt = mkSY_CREC( m_CREC, lntype ? dt_ln : ln, lntype );

		if( cnt != -1 )	// 消費税データ作成
		{
			struct _DenpData* pd;
			for( n = 0; n < cnt; n++ ) {
				CDBINPDataRec	trec;

				trec = m_CREC[n];

				switch( IsKind() ) {
				case KIND_NYUKIN:
					syzpn = FD_CSKBN_PN;	break;
				case KIND_SYUKKIN:
					syzpn = FD_DSKBN_PN;	break;
				default:
					if( cnt == 2 ) {
						syzpn = (n==0) ? FD_DSKBN_PN : FD_CSKBN_PN;
					}
					else {
						if( isSYOGT_CODE( trec.m_dbt ) || ! m_DI.km_syattr( trec.m_dbt ) )
								syzpn = FD_CSKBN_PN;
						else	syzpn = FD_DSKBN_PN;
					}
					break;
				}

/*DNP_SYZ8*/
#ifdef CLOSE
				int zeipn;
				if( syzpn == FD_DSKBN_PN )	zeipn = FD_DBZEI_PN;
				else						zeipn = FD_CRZEI_PN;

				::ZeroMemory( &trec.m_dsign[0], SZ_DSGN );

				// 消費税区分をクリア
				::ZeroMemory( &_getDATA( dt_ln, syzpn )->SYDATA, sizeof _getDATA( dt_ln, syzpn )->SYDATA );
				// 税額手入力サインをクリア
				_getCND( dt_ln, zeipn )->SET_sg = FALSE;
#endif
/*DNP_SYZ8*/

				pd = _getDATA( dt_ln, syzpn );

#ifdef CLOSE
				//*DNP_SYZ8*/ --------------------------
				if( 1 ) {
					CDBINPDataRec dmy;	// 税額再計算のため、ダミー呼び出し。
					dmy.Reset();
					dmy = m_CREC[n];
					dmy
					dmy.m_dbt = SYOGT;
					dmy.m_cre = SYOGT;
					m_DI.sy_chk_txt( &dmy, NULL );
				}
				//*DNP_SYZ8*/ ----------------------------------
#endif

				trec.m_ddate = prec->m_ddate;
				trec.m_ksign = prec->m_ksign;

				m_DI.DB_SyIniz( &trec );

				trec.m_ddate = m_CREC[n].m_ddate;
				trec.m_ksign = m_CREC[n].m_ksign;

				if( m_pSy->IsKaniMaster() ) {
					if( pd->SYDATA.uri_sw == CD_6FUDOSAN ) {
						if( trec.m_ddate < KANI6_YMD ) {
							DWORD	cdUri;
							cdUri = (KaniDefaultKubun( m_pZm->zvol ) << 8);
							pd->SYDATA.uri_sw = cdUri;
						}
					}
				}
				//リバースチャージ
				if( pd->SYDATA.swk_sw == 0x1d || pd->SYDATA.swk_sw == 0x1e ) {
					if( trec.m_ddate < RVCHAR_YMD ) {
						pd->SYDATA.uri_sw = 0;
					}
				}

				if( m_pSy->SyMsg( &smpk ) == 0) {

					if( pd->SYDATA.swk_sw > 0 )		m_pSy->SyFnc( pd->SYDATA.swk_sw );
					if( pd->SYDATA.ritu_sw > 0 )	m_pSy->SyFnc( pd->SYDATA.ritu_sw );
					if( pd->SYDATA.zeikbn_sw > 0 )	m_pSy->SyFnc( pd->SYDATA.zeikbn_sw );
					if( pd->SYDATA.uri_sw > 0 )		m_pSy->SyFnc( pd->SYDATA.uri_sw );
					if( pd->SYDATA.toku_sw > 0 )	m_pSy->SyFnc( pd->SYDATA.toku_sw );
					if( pd->SYDATA.kts_sw > 0 )		m_pSy->SyFnc( pd->SYDATA.kts_sw );

#ifdef KEEP_CLOSE
					int swk, ritu, zei, uri, tai;
					swk = ritu = zei = uri = tai = 0;

					swk		= smpk.SY_MSG_SWKBN->sel_no;
					ritu	= smpk.SY_MSG_RITU->sel_no;
					zei		= smpk.SY_MSG_ZEI->sel_no;
					uri		= smpk.SY_MSG_URISIRE->sel_no;
					tai		= smpk.SY_MSG_TAIKAKBN->sel_no;

					if( swk )	pd->SYDATA.swk_def = smpk.SY_MSG_SWKBN->msg_tbl[swk-1]->code;
					else		pd->SYDATA.swk_def = 0;

					if( ritu )	pd->SYDATA.ritu_def = smpk.SY_MSG_RITU->msg_tbl[ritu-1]->code;
					else		pd->SYDATA.ritu_def = 0;
	
					if( zei )	pd->SYDATA.zeikbn_def = smpk.SY_MSG_ZEI->msg_tbl[zei-1]->code;
					else		pd->SYDATA.zeikbn_def = 0;

					if( uri )	pd->SYDATA.uri_def = smpk.SY_MSG_URISIRE->msg_tbl[uri-1]->code;
					else		pd->SYDATA.uri_def = 0;

					if( tai )	pd->SYDATA.kts_def = smpk.SY_MSG_TAIKAKBN->msg_tbl[tai-1]->code;
					else		pd->SYDATA.kts_def = 0;
#endif
				}

				// 消費税サインをデータテーブルに
				setSY_CREC( &m_CREC[n], (syzpn == FD_DSKBN_PN) ? 0 : 1, lntype ? dt_ln : ln, lntype );

			}
		}
#ifdef SCROL_CLOSE
		++ln;
#endif
	}
}

//
// 日　入力＆オフフォーカス
long CDnpBaseClass::DD_Inp( int pn, long nChar, BYTE dd ) 
{

	// オフフォーカス
	if( !nChar )
	{
		// データセット
		DD_Set( pn, dd );

		return(0);
	}
	else
	{
		if( nChar == VK_TAB && is_SHIFT() )
			return 0;
	}
	// 入力
	switch( nChar ) {
	case VK_RETURN: 
	case VK_TAB:
	case VK_LEFT:  case VK_UP: 
	case VK_RIGHT: case VK_DOWN:
		DD_Set( pn, dd );
		break;
	}
	return( nChar );
}

// 日　データセット
void CDnpBaseClass::DD_Set( int pn, BYTE dd ) 
{
	struct _DenpData check = {0}, *keybuf = getDATA( pn );

	if( pn < 0 )	return;

	CDBINPDataRec	tmpRec;
	setDATE_CREC( &tmpRec );

	//データリード
//	Data_Get( getINP_CTL( pn )->pIDC->IDC_X, &check );
	if( dd )
	{
		getCND( pn )->INP_sg = TRUE;

		keybuf->DP_BCDDAY[0] = dd;
	}
	else {
		getCND( pn )->INP_sg = FALSE;
	}
	// 伝票ダイアログへデータをセット
	GetNowDnpDialog()->SetDD( DP_DD_PN, dd );

	//データセット
//	Data_Set( getINP_CTL( pn )->pIDC->IDC_X, keybuf );

	// 消費税チェック
	date_syzcheck( &tmpRec );

}


//------------------------------------------------------------------
//	月チェック
//	int	pn			ポジション
//	int mm			入力された月(61,62,63,00 は決修)
//					月の数字でない場合は、現在のデータを取得する
//	BYTE* pBcd		BCD変換した月
//
//	return TRUE = 入力された月に変更した, FALSE = 不正月
//------------------------------------------------------------------
BOOL CDnpBaseClass::MM_Check( int pn, int mm, BYTE* pBcd )
{
	BYTE	month[4] = {0};
	BYTE	check[4] = {0};
	char	buf[48];
	DAPACGEN	da;
	int sgn = 0;
	BOOL ret = FALSE;

	if( (mm >= 0 && mm <= 12) || mm == 61 || mm == 62 || mm == 63 ) {
		BOOL bChk = FALSE;
		if( !db_vd_dateGen(1, mm, &da, m_pZm) && !db_vd_check(1, mm, m_pZm) ) {
			bChk = TRUE;
		}
		else {
			int mmdd;
				
			sprintf_s(buf, sizeof buf, "%02x", getDATA(DP_DD_PN)->DP_BCDDAY[0]);
			mmdd = atoi(buf);
			mmdd += (mm * 100);

			if( !db_vd_dateGen(0, mmdd, &da, m_pZm) && !db_vd_check(0, mmdd, m_pZm) ) {
				bChk = TRUE;
			}
		}

		if( bChk ) {

			switch( mm ) {
			case 0:		sgn = 4;	break;
			case 61:	
			case 62:	
			case 63:	
				sgn = mm - 60;
				break;
			default:
				sgn = 0;
				break;
			}
			if( mm == 0 || mm == 61 || mm == 62 || mm == 63 ) {
				mm = (da.Eymd % 10000) / 100;
			}

			sprintf_s( buf, sizeof buf, "%02d", mm );
			l_pack( check, buf, 2 );
			check[1] = getDATA( DP_DD_PN )->DP_BCDDAY[0];

#ifdef NEWDATE_CLOSE
			if( m_pZm->check_datelimit( (BYTE*)check ) ) {
#endif
			//	int mm = (da.Eymd / 100) % 100;
				sprintf_s( buf, sizeof buf, "%02d", mm );
				l_pack( month, buf, 2 );

				if( sgn ) {
					KESY_SG_SET( month[0], sgn );
				}
				*pBcd = month[0];

				ret = TRUE;

#ifdef NEWDATE_CLOSE
			}
#endif
		}
	}

	if( ! ret ) {
		// 前のデータに戻す
		_DenpData* pData = getDATA( DP_MM_PN );
		*pBcd = pData->DP_BCDDAY[0];
	}

	return ret;
}


//------------------------------------------------------------------
//	日チェック
//	int	pn			ポジション
//	BYTE dd			日(BCD)
//					日の数字でない場合は、現在のデータを取得する
//	BYTE* pBcd		BCD変換した月
//
//	return TRUE = 入力された月に変更した, FALSE = 不正月
//------------------------------------------------------------------
BOOL CDnpBaseClass::DD_Check( int pn, BYTE dd, BYTE* pBcd )
{
struct _DenpData rdsp = {0};
struct _DenpData *mmbuf, *keybuf;
BOOL	ret = FALSE;
int		ksgn, date, ddtmp;
char	buf[48];
BYTE	check[4] = {0};

	//データゲット
	mmbuf	= getDATA( DP_MM_PN );
	keybuf	= getDATA( pn );
ASSERT( mmbuf != NULL );

	if( dd > 0x31 ) {
		*pBcd = keybuf->DP_BCDDAY[0];
	}
	else {
		ksgn = KESY_SG_GET( mmbuf->DP_BCDDAY[0] );

		switch( ksgn ) {
		case 1:	date = 6100;	break;
		case 2:	date = 6200;	break;
		case 3: date = 6300;	break;
		case 4:	date = 0;		break;
		default:
			sprintf_s( buf, sizeof buf, "%02x", (mmbuf->DP_BCDDAY[0]&0x1f) );
			date = atoi( buf );
			date *= 100;
			break;
		}
		// 月日を作成して日付チェック
		sprintf_s( buf, sizeof buf, "%02x", dd );
		ddtmp = atoi( buf );
		date += ddtmp;

#ifdef NEWDATE_CLOSE
		if( ! db_vd_date( 0, date, &da, m_pZm ) && ! db_vd_check( 0, date, m_pZm) ) {
			switch( ksgn ) {
			case 1:	check[0] = 0x61;	break;
			case 2:	check[0] = 0x62;	break;
			case 3: check[0] = 0x63;	break;
			case 4:	check[0] = 0x00;	break;
			default:
				check[0] = mmbuf->DP_BCDDAY[0];
				break;
			}
			check[1] = dd;

			if( m_pZm->check_datelimit( (BYTE*)check ) ) {
				*pBcd = dd;
				ret = TRUE;
			}
			else {
				*pBcd = keybuf->DP_BCDDAY[0];
			}
		}
		else {
			*pBcd = keybuf->DP_BCDDAY[0];
		}
#else
		*pBcd = dd;
#endif
	}

	return ret;
}


//-------------------------------------------
// 日付チェック＆アクション
//-------------------------------------------
int CDnpBaseClass::DateCheck_and_action( BOOL bMsg/*=TRUE*/ )
{
char *pmsg;
int sg;

	switch( sg = DateCheck() )
	{
	case -1:	// 月未入力
		pmsg = "「日付」の「月」を入力してください。";
		break;
	case -2:	// 日未入力
		pmsg = "「日付」の「日」を入力してください。";
		break;
	case -3:	// 月日未入力
		pmsg = "「日付」を入力してください。";
		break;
	case -4:	// エラー
		pmsg = "「日付」がまちがっています。";
		break;
#ifndef NEWDATE_CLOSE
	case -5:	// 範囲指定外
		pmsg = "「日付」が月選択の範囲指定外です。";
		break;
#endif
	default:
		return 0;
	}

	if( bMsg ) {
		m_PRO_OK = FALSE;
		myICSMessageBox( pmsg );

		m_PRO_OK = TRUE;
#ifdef NEWDATE_CLOSE
	//	set_focus( get_nowpn() );
		set_focus( DP_DD_PN );
#else
		set_focus( DP_MM_PN );
#endif
	}

	return -1;
}

//-------------------------------------------
// 日付チェック
//	->	0 ... OK
//		-1 .. 月未入力
//		-2 .. 日未入力
//		-3 .. 月日未入力
//		-4 .. エラー
//-------------------------------------------
int CDnpBaseClass::DateCheck() 
{
struct _DenpData rdsp = {0};
BYTE check[4] = {0};
BYTE vdchk[4] = {0};
BYTE symd[4] = {0}, eymd[4] = {0};
int sgn;

	//月セット
	if( getCND( DP_MM_PN )->INP_sg )
		check[0] = getDATA( DP_MM_PN )->DP_BCDDAY[0];	//入力の月
	else
		check[0] = 0;	//未入力サイン

	//日セット
	if( getCND( DP_DD_PN )->INP_sg )
		check[1] = getDATA( DP_DD_PN )->DP_BCDDAY[0];	//入力の日
	else
		check[1] = 0;	//未入力サイン

	if( !check[0] && check[1] )
		return -1;	// 月未入力
	else if( check[0] && !check[1] )
		return -2;	// 日未入力
	else if( !check[0] && !check[1] )
		return -3;	// 月日未入力

	sgn = KESY_SG_GET( check[0] );

	switch( sgn ) {
	case 1:	vdchk[0] = 0x61;	break;
	case 2:	vdchk[0] = 0x62;	break;
	case 3:	vdchk[0] = 0x63;	break;
	case 4:	vdchk[0] = 0x00;	break;
	default:
		vdchk[0] = check[0];
		break;
	}
	vdchk[1] = check[1];
	int s_gengo, e_gengo;

	if( ! _db_vd_dateGen( vdchk, &s_gengo, symd, &e_gengo, eymd, m_pZm ) && ! _db_vd_check( vdchk, m_pZm) ) {
		getDATA( DP_MM_PN )->DP_BCDDAY[0] = check[0];
		getDATA( DP_DD_PN )->DP_BCDDAY[0] = check[1];

		// 諸口貸借表示
		syogdsp();

#ifndef NEWDATE_CLOSE
//		if( ! m_pZm->check_datelimit( (BYTE*)vdchk ) ) {
//			return -5;
//		}
#endif

		return 0;	// OK
	}

	return -4;	// 日付エラー
}




//----------------------------------------------------
//	選択画面に科目名等を表示する
//	int dnp_ln	今回のライン
//	int dnp_pn	今回のポジション
//
//----------------------------------------------------
void CDnpBaseClass::SelectDisp( int dnp_ln, int dnp_pn )
{
	SelPar par;
	int	ln, pn;
	int	nondisp = 0;

	NotSelNumber() = FALSE;

	switch( dnp_pn ) {
	case DP_BMN_PN:
	case FD_DBMN_PN:
	case FD_CBMN_PN:
		par.sel_pn = SL_BUMON_PN;
		break;
	case DP_KOJI_PN:
	case FD_DKOJI_PN:
	case FD_CKOJI_PN:
		par.sel_pn = SL_KOJI_PN;
		break;
	case FD_TKY_PN:
		ln = dnp_ln;
	//	ln = get_nowln();	[07.11 /08]

		if( IsAutoSiwake( ln )) {
			par.sel_pn = SL_TKYALL_PN;
			par.dbt.Empty();
			par.cre.Empty();
		}
		else {
			par.sel_pn = SL_TKY_PN;
			::ZeroMemory( par.tky_kana, sizeof par.tky_kana );

			get_linekamokucode( ln, par.dbt, par.cre );

			if( IsKind() == KIND_NYUKIN ) {
				par.dbt = GNKINCode(m_pZm->zvol->apno);
			}
			else if( IsKind() == KIND_SYUKKIN ) {
				par.cre = GNKINCode(m_pZm->zvol->apno);
			}
			//行変わりは、再表示
			if( get_nowln() != dnp_ln ) {
				m_SelDispPN = -1;
			}
		}
		break;
	case FD_DSKBN_PN:
	case FD_CSKBN_PN:
	case FD_HJIYU_PN:
		nondisp = 1;
		break;

	case FD_DSYOG_PN:
	case FD_CSYOG_PN:

		NotSelNumber() = TRUE;

		pn = get_nowpn();
		if( pn == FD_DSYOG_PN || pn == FD_CSYOG_PN ) {
			if( m_SelDispPN == SL_BRNTKY_PN )
				nondisp = 1;
		}

		if( ! nondisp ) {
			if( BRNTKY_select( -1, CString(SYOGT), NULL, TRUE ) ) {
				nondisp = 1;
			}
		}
		break;

	case FD_DEBT_PN:
	case FD_CRED_PN:
		ln = get_nowln();	//前回ポジション
		pn = get_nowpn();

		if( ln != dnp_ln || pn != dnp_pn ) {
			par.sel_pn = SL_KAMOKU_PN;
		}
		else {
			if( m_SelDispPN == SL_KAMOKU_PN || m_SelDispPN == SL_KMKANA_PN ||
				m_SelDispPN == SL_BRNTKY_PN ) {
				return;
			}
		}
	default:
		par.sel_pn = SL_KAMOKU_PN;
		break;
	}

	// 消費税の選択欄表示は、メッセージより
	// 変動事由はあとで
	if( ! nondisp ) {
		if( m_SelDispPN != par.sel_pn ) {
			m_Seldata.SelectJob( &par, -1 );
		}
	}
}


// 不正な科目コード入力時など、科目名称部分をを再表示
//	int ln ... 画面伝票上の　行番号(１～最大７)
//
void CDnpBaseClass::KnamRedisp( int ln, int pn )
{
	//DBEditに変更したため 再表示の必要がなくなった
	return;

	// 科目名称＋枝番に
	if( pn == FD_DEBT_PN || pn == FD_CRED_PN ) {
		SETMEISYO sm = {0};
		char edabf[30];
		int dt_ln = get_dataline( ln );

		set_codestr( edabf, sizeof edabf, _getDATA( dt_ln, pn )->DP_BRN, m_pZm->zvol->edcol );
		sprintf_s( sm.sm_name, sizeof sm.sm_name, "%.14s%s", _getDATA( dt_ln, pn )->DP_KNAM, edabf );

		if( sm.sm_name[0] ) {
			GetNowDnpDialog()->SetKnam( ln, pn, &sm, TRUE );
			m_bIgnoreFocus = TRUE;	// DIAG EDIT ON が呼ばれるので
		}
		else {
			GetNowDnpDialog()->SetKnam( ln, pn, NULL, TRUE );
		}
	}
}

// 取消仕訳のフォーカス処理
// 行・ポジションの位置をセット
//
void CDnpBaseClass::ToViewDelFocusJob( int ln, int pn )
{
	// 選択画面
	SelectDisp( ln, pn );
	int line, pos;

	line = (ln == -1) ? get_nowln() : ln;
	pos = ( pn == -1 ) ? get_nowpn() : pn;

	set_nowpn( pn );
	set_nowln( line );
}


//--------------------------------------------------
// オンフォーカス時の仕事のくくり
//	displn	= 今回のライン
//	pn		= 今回のポジション
//--------------------------------------------------
int CDnpBaseClass::OnFocusJob( int displn, int pn, CRect& rect )
{

	if( pn < 0 )	return	0;

#ifdef OLD_CLOSE
	// データ背景色
	line_backcol( ln, pn );

	//自動仕訳モードの解除
	if( ln != get_nowln() ) {
		if( IsJob() == JOB_SCAN && IsKind() == KIND_FURIKAE && IsScanLine( ln ) ) {
			if( IsMode() == MODE_AUTO ) {
				IsMode()	=	MODE_NORMAL;
				if( GetNowDnpDialog )	GetNowDnpDialog->ChangeMode( IsMode() );
			}
		}
	}
#endif
	int	nowline;
	nowline = get_nowln();

	GetNowDnpDialog()->FocusTkyColor( nowline, FALSE );

	if( IsJob() == JOB_SCAN ) {
		if( displn != nowline || pn != FD_TKY_PN ) {
			//貸方摘要の消去
			KasiTekiyoDisplay( FALSE );
			//貸方イメージの消去
			KasiImageDisplay( displn, FALSE );
			//借方イメージの背景色
			ImageBackColor( nowline, FALSE );
			//イメージ再表示有効
			ImagePaintSw( 1, getSCROLL_NL(), 0 );
		}
	}

	// 消費税欄 リセット
/*
	if( get_nowpn() == getSKBN_PN() ) {
		_set_color(_getINP_CTL( get_nowln(), getSKBN_PN() )->pIDC, clNormalBack, FALSE );
		_set_color(_getINP_CTL( get_nowln(), getSKBN_PN() )->pIDC, clNormalFore, TRUE );
	}
*/
	// 摘要欄の伸縮
	if( nowline != displn ) {
		// DBEditを戻す
		tkyexpan( nowline, NULL, FALSE );			// 元の位置
		kjedit_expan( nowline, pn, NULL, FALSE );	// 元の位置
		tgnum_expan( nowline, NULL, FALSE );
		bmnedit_expan( nowline, pn, NULL, FALSE );
		syobrnedit_expan(nowline, pn, NULL, FALSE);
		syonum_expan(nowline, NULL, FALSE);
		kamoku_expan(nowline, pn, NULL, FALSE);

		//フォーカスカウントOFF
		FocusCountClear(nowline);
	}
	else {
		if( (pn != FD_TKY_PN && get_nowpn() == FD_TKY_PN) ) {
			tkyexpan( get_nowln(), NULL, FALSE );	// 元の位置
		}
		if( (pn != FD_DKOJI_PN && get_nowpn() == FD_DKOJI_PN) ||
			(pn != FD_CKOJI_PN && get_nowpn() == FD_CKOJI_PN) ||
			(pn != DP_KOJI_PN  && get_nowpn() == DP_KOJI_PN) ){
			kjedit_expan( get_nowln(), pn, NULL, FALSE );	// 元の位置
		}
		if( (pn != FD_TGNUM_PN && get_nowpn() == FD_TGNUM_PN) ) {
			tgnum_expan( nowline, NULL, FALSE );
		}

		if( (pn != FD_DBMN_PN && get_nowpn() == FD_DBMN_PN) ||
			(pn != FD_CBMN_PN && get_nowpn() == FD_CBMN_PN) ||
			(pn != DP_BMN_PN  && get_nowpn() == DP_BMN_PN) ){
			bmnedit_expan( nowline, pn, NULL, FALSE );
		}
		if ((pn != FD_DSYOG_PN && get_nowpn() == FD_DSYOG_PN) ||
			(pn != FD_CSYOG_PN && get_nowpn() == FD_CSYOG_PN)) {
			syobrnedit_expan(nowline, pn, NULL, FALSE);
		}
		if( (pn != DP_SYONUM_PN && get_nowpn() == DP_SYONUM_PN) ) {
			syonum_expan(nowline, NULL, FALSE);
		}
		if( (pn != FD_DEBT_PN && get_nowpn() == FD_DEBT_PN) ||
			(pn != FD_CRED_PN && get_nowpn() == FD_CRED_PN) ) {
			kamoku_expan(nowline, pn, NULL, FALSE);
		}
	}

	if( pn == FD_TKY_PN ) {
		// イメージ摘要は、あとでフォーカスセットする
		if( ! ( IsJob() == JOB_SCAN && IsTekiyoImage( displn ) ) ) {
			tkyexpan( displn, &rect, TRUE );		// 伸ばし
		}
	}
	else if( pn == FD_DKOJI_PN || pn == FD_CKOJI_PN || pn == DP_KOJI_PN ) {
		kjedit_expan( displn, pn, &rect, TRUE );
	}
	else if( pn == FD_TGNUM_PN ) {
		tgnum_expan( displn, &rect, TRUE );
	}
	else if( pn == FD_DBMN_PN || pn == FD_CBMN_PN || pn == DP_BMN_PN ) {
		bmnedit_expan( displn, pn, &rect, TRUE );
	}
	else if (pn == FD_DSYOG_PN || pn == FD_CSYOG_PN) {
		syobrnedit_expan(displn, pn, &rect, TRUE);
	}
	else if( pn == DP_SYONUM_PN ) {
		syonum_expan(displn, &rect, TRUE);
	}
	else if( pn == FD_DEBT_PN || pn == FD_CRED_PN ) {
		kamoku_expan(displn, pn, &rect, TRUE);
	}

#ifdef LATER_CLOSE
	if( pn != FD_TKY_PN ) {
		((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar( 4, 20 );
	}
	else {
		((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar( 4, 21 );
	}
#endif

	int p_pn;
	p_pn = get_nowpn();

	// 消費税区分入力非表示
	if( p_pn != FD_DSKBN_PN && p_pn != FD_CSKBN_PN ) {
		Destroy_SyzInp();
	}

//	GetNowDnpDialog()->FocusLine( ln, TRUE );

	return 0;
}


//-----------------------------------------------------------
// 行変更時のくくり
//	CDBINPDataRec *data	最低2レコードのエリアを渡す
//	int	ln				今回のライン [画面伝票上(１～最大７)]
//	int pn				今回のポジション
//-----------------------------------------------------------
int CDnpBaseClass::LineChgJob( CDBINPDataRec *data, int ln, int pn )
{
	int	i;

TRACE( "LineChgJob TOP\n" );

	if( ln <= 0	)	return 0;

	int dt_ln = get_dataline( ln );

	//修正時元仕訳のタイプに合わせる --- 単独仕訳のときは、片方の金額修正をもう片方に反映
	if( IsJob() == JOB_SCAN ) {
		if( IsKind() == KIND_FURIKAE ) {
			if( IsScanLine( dt_ln ) ) {
				if( m_DPscan.IsFukugoSiwake( dt_ln ) ) {
//					if( IsType() != TYPE_FUKUGOU )	ChangeType( TYPE_FUKUGOU );
					ChangeType( TYPE_FUKUGOU, TRUE );
				}
				else {
//					if( IsType() != TYPE_TANDOKU )	ChangeType( TYPE_TANDOKU );
					ChangeType( TYPE_TANDOKU, TRUE );
				}
			}
			else {
				ChangeType( IsDefaultType()/*IsType()*/ );
			}
#ifndef	CUT040202
#else
			else {
				for( i = 0; i < getSCROLL_NL(); i++ ) {
					if( !IsScanLine( i ) ) {
						if( _getCND( i, FD_DBVAL_PN )->INP_sg != FALSE ||
							_getCND( i, FD_DEBT_PN )->INP_sg != FALSE ||
							_getCND( i, FD_CRED_PN )->INP_sg != FALSE ||
							_getCND( i, FD_CRVAL_PN )->INP_sg != FALSE ||
							_getCND( i, FD_TKY_PN )->INP_sg != FALSE ) {
							break;
						}
					}
				}
				if( i >= getSCROLL_NL() ) {
					if( IsType() != IsDefaultType() )	ChangeType( IsDefaultType() );
				}
			}
#endif
		}
	}
#ifdef OLD_CLOSE	// 諸口入力させない
	else {
		//諸口入力時は単一仕訳
		struct _DenpData *debt, *cred;
		debt	=	_getMAPtoDATA( ln, FD_DEBT_PN );
		cred	=	_getMAPtoDATA( ln, FD_CRED_PN );
		BOOL	sw	=	FALSE;

		if( _getCND( ln, FD_DBVAL_PN )->INP_sg != FALSE &&
			_getCND( ln, FD_CRVAL_PN )->INP_sg != FALSE ) {
			struct _DenpData *dval, *cval;
			dval	=	_getDATA( ln, FD_DBVAL_PN );
			cval	=	_getDATA( ln, FD_CRVAL_PN );
			if( !l_cmp( dval->DP_VAL, cval->DP_VAL ) ) {
//				if( _getCND( ln, FD_DEBT_PN )->INP_sg != FALSE ) {
				if( debt ) {
					if( isSYOGT_CODE( debt->DP_CdNum ) )	sw	=	TRUE;
				}
//				if( _getCND( ln, FD_CRED_PN )->INP_sg != FALSE ) {
				if( cred ) {
					if( isSYOGT_CODE( cred->DP_CdNum ) )	sw	=	TRUE;
				}

				if( sw ) {
					if( IsType() != TYPE_TANDOKU )	ChangeType( TYPE_TANDOKU );
				}
			}
		}
	}
#endif

//char buf[20] = {0};
//char sv[8];
struct _DenpData *keybuf;
DBKNREC* pKn;

	int kind = IsKind();
	int cnt, dcpn, dc_sw, idx;

	// データバッファクリア
	for( i = 0; i < 2; i++ )
		(data+i)->Reset();

	// 輸入仕入れ サインクリア
	m_bSyzYunyu[0] = m_bSyzYunyu[1] = FALSE;

	// 入力データより仕訳レコード作成
	if( ( cnt = mkSY_CREC( data, ln ) ) == -1 ) {
		// データがないときは、ダミーでイニシャライズ
		data->m_seq = 1;
		setDATE_CREC( data );
		m_DI.DB_SyIniz( &data[0] );
		return 0;
	}

	if( kind == KIND_FURIKAE ) {

		dc_sw = _get_dcsgn( pn );
		dcpn = dc_sw ? FD_CSKBN_PN : FD_DSKBN_PN;

		for( i = 0; i < cnt; i++ ) {
			if( cnt == 1 ) {
				m_DI.DB_SyIniz( &data[0] );
				pKn = DB_PjisToKnrec( data[0].m_dbt, m_pZm );
				if( pKn ) {
					if( m_DI.km_syattr( pKn ) )
						dcpn = FD_DSKBN_PN;
					else
						dcpn = FD_CSKBN_PN;
				}
				else {
					if( (pKn = DB_PjisToKnrec( data[0].m_cre, m_pZm )) ) {
						if( m_DI.km_syattr( pKn ) )
							dcpn = FD_CSKBN_PN;
						else
							dcpn = FD_DSKBN_PN;
					}
				}

				keybuf = _getDATA( dt_ln, dcpn );

				if( keybuf->SYDATA.swk_sw > 0 )	m_pSy->SyFnc( keybuf->SYDATA.swk_sw );
				if( keybuf->SYDATA.ritu_sw > 0 )	m_pSy->SyFnc( keybuf->SYDATA.ritu_sw );
				if( keybuf->SYDATA.zeikbn_sw > 0 )	m_pSy->SyFnc( keybuf->SYDATA.zeikbn_sw );
				if( keybuf->SYDATA.uri_sw > 0 )		m_pSy->SyFnc( keybuf->SYDATA.uri_sw );
				if( keybuf->SYDATA.toku_sw > 0 )	m_pSy->SyFnc( keybuf->SYDATA.toku_sw );	//特定収入
				if( keybuf->SYDATA.kts_sw > 0 )	m_pSy->SyFnc( keybuf->SYDATA.kts_sw );
			}
			else {
				if( dc_sw == 1 ) {
					//現ポジションが貸方の場合は、借方→貸方の順で処理
					dcpn	= (i == 0) ? FD_DSKBN_PN : FD_CSKBN_PN;
					idx		= (i == 0) ? 0 : 1;
				}
				else {
					//現ポジションが借方の場合は、貸方→借方の順で処理
					dcpn	= (i == 0) ? FD_CSKBN_PN : FD_DSKBN_PN;
					idx		= (i == 0) ? 1 : 0;
				}
				m_DI.DB_SyIniz( &data[idx] );
				keybuf = _getDATA( dt_ln, dcpn );

				if( keybuf->SYDATA.swk_sw > 0 )		m_pSy->SyFnc( keybuf->SYDATA.swk_sw );
				if( keybuf->SYDATA.ritu_sw > 0 )	m_pSy->SyFnc( keybuf->SYDATA.ritu_sw );
				if( keybuf->SYDATA.zeikbn_sw > 0 )	m_pSy->SyFnc( keybuf->SYDATA.zeikbn_sw );
				if( keybuf->SYDATA.uri_sw > 0 )		m_pSy->SyFnc( keybuf->SYDATA.uri_sw );
				if( keybuf->SYDATA.toku_sw > 0 )	m_pSy->SyFnc( keybuf->SYDATA.toku_sw );	//特定収入
				if( keybuf->SYDATA.kts_sw > 0 )	m_pSy->SyFnc( keybuf->SYDATA.kts_sw );
			}
		}
	}
	else if( kind == KIND_NYUKIN ) {
		m_DI.DB_SyIniz( &data[0] );
		dcpn = FD_CSKBN_PN;
		keybuf = _getDATA( dt_ln, dcpn );

		if( keybuf->SYDATA.swk_sw > 0 )		m_pSy->SyFnc( keybuf->SYDATA.swk_sw );
		if( keybuf->SYDATA.ritu_sw > 0 )	m_pSy->SyFnc( keybuf->SYDATA.ritu_sw );
		if( keybuf->SYDATA.zeikbn_sw > 0 )	m_pSy->SyFnc( keybuf->SYDATA.zeikbn_sw );
		if( keybuf->SYDATA.uri_sw > 0 )		m_pSy->SyFnc( keybuf->SYDATA.uri_sw );
		if( keybuf->SYDATA.toku_sw > 0 )	m_pSy->SyFnc( keybuf->SYDATA.toku_sw );	//特定収入
		if( keybuf->SYDATA.kts_sw > 0 )	m_pSy->SyFnc( keybuf->SYDATA.kts_sw );
	}
	else {
		m_DI.DB_SyIniz( &data[0] );
		dcpn = FD_DSKBN_PN;
		keybuf = _getDATA( dt_ln, dcpn );

		if( keybuf->SYDATA.swk_sw > 0 )		m_pSy->SyFnc( keybuf->SYDATA.swk_sw );
		if( keybuf->SYDATA.ritu_sw > 0 )	m_pSy->SyFnc( keybuf->SYDATA.ritu_sw );
		if( keybuf->SYDATA.zeikbn_sw > 0 )	m_pSy->SyFnc( keybuf->SYDATA.zeikbn_sw );
		if( keybuf->SYDATA.uri_sw > 0 )		m_pSy->SyFnc( keybuf->SYDATA.uri_sw );
		if( keybuf->SYDATA.toku_sw > 0 )	m_pSy->SyFnc( keybuf->SYDATA.toku_sw );	//特定収入
		if( keybuf->SYDATA.kts_sw > 0 )	m_pSy->SyFnc( keybuf->SYDATA.kts_sw );
	}

	// 貸借データ表示
	total_dsp();

TRACE( "LineChgJob END\n" );

	return 0;
}


// TODO 外税の件で、検討の余地あり
// 貸借合計表示
void CDnpBaseClass::total_dsp()
{
	// 貸借計算
	if( IsJob() == JOB_INPUT )	total_clq();
	else						ScanAllTotalClc();

	GetNowDnpDialog()->total_dsp();
}


// 貸借合計計算
void CDnpBaseClass::total_clq()
{
int ln, pn, n, syzpn, zeipn;
int		rv_pn, rv_zei, rv_val;
struct _DenpData *pdata, *syzbuf;
struct _DNPLINE_CND *psg;
struct _DenpData *pzdata;
struct _DNPLINE_CND *pzsg;

	l_defn( 0x16 );

	if( IsKind() == KIND_FURIKAE ) {

		for( n = 0 ; n < 2 ; ++n ) {

			if( !n ) {
				pn		= FD_DBVAL_PN;
				zeipn	= FD_DBZEI_PN;
				syzpn	= FD_DSKBN_PN;
				rv_pn	= FD_CRED_PN;
				rv_val	= FD_CRVAL_PN;
				rv_zei	= FD_CRZEI_PN;

				pdata = &DEBT_TOTAL;
				psg = &DEBT_TOTAL_CND;
				// 持ち越し
				pzdata = &ZDEBT_TOTAL;
				pzsg = &ZDEBT_TOTAL_CND;
			}
			else {
				pn		= FD_CRVAL_PN;
				zeipn	= FD_CRZEI_PN;
				syzpn	= FD_CSKBN_PN;
				rv_pn	= FD_DEBT_PN;
				rv_val	= FD_DBVAL_PN;
				rv_zei	= FD_DBZEI_PN;

				pdata = &CRED_TOTAL;
				psg = &CRED_TOTAL_CND;
				// 持ち越し
				pzdata = &ZCRED_TOTAL;
				pzsg = &ZCRED_TOTAL_CND;
			}

			memmove( pdata, pzdata, sizeof( struct _DenpData ));
			memmove( psg, pzsg, sizeof( struct _DNPLINE_CND ));

			for( ln = 1 ; ln <= get_MaxLine(); ++ln ) {
				if( _getCND( ln, pn )->INP_sg ) {
					l_add( pdata->DP_VAL, pdata->DP_VAL, _getDATA( ln, pn )->DP_VAL);
					psg->INP_sg = TRUE;

					syzbuf = _getDATA( ln, syzpn );

					if( _sotozei_data( syzbuf->SYDATA._DSGN ) ) {
						l_add( pdata->DP_VAL, pdata->DP_VAL, _getDATA( ln, zeipn )->DP_VAL );
					}
				}
			}
		}
	}
	else {
		if( IsKind() == KIND_NYUKIN ) {
			pn		=	FD_CRVAL_PN;
			zeipn	=	FD_CRZEI_PN;
			syzpn	=	FD_CSKBN_PN;
			pdata	=	&CRED_TOTAL;
			psg		=	&CRED_TOTAL_CND;
			pzdata	=	&ZCRED_TOTAL;
			pzsg	=	&ZCRED_TOTAL_CND;
		}
		else if( IsKind() == KIND_SYUKKIN ) {
			pn		=	FD_DBVAL_PN;
			zeipn	=	FD_DBZEI_PN;
			syzpn	=	FD_DSKBN_PN;
			pdata	=	&DEBT_TOTAL;
			psg		=	&DEBT_TOTAL_CND;
			pzdata	=	&ZDEBT_TOTAL;
			pzsg	=	&ZDEBT_TOTAL_CND;
		}
		memmove( pdata, pzdata, sizeof( struct _DenpData ));
		memmove( psg, pzsg, sizeof( struct _DNPLINE_CND ));
		for( ln = 1 ; ln <= get_MaxLine(); ++ln ) {
			if( _getCND( ln, pn )->INP_sg ) {
				l_add( pdata->DP_VAL, pdata->DP_VAL, _getDATA( ln, pn )->DP_VAL);
				psg->INP_sg	=	TRUE;

				syzbuf = _getDATA( ln, syzpn );

				if( _sotozei_data( syzbuf->SYDATA._DSGN ) ) {
					l_add( pdata->DP_VAL, pdata->DP_VAL, _getDATA( ln, zeipn )->DP_VAL );
				}
			}
		}
	}
}

//貸借チェック
//	->	0 ... 貸借ＯＫ
//		>0 .. 借方金額大
//		<0 .. 貸方金額大
int CDnpBaseClass::total_chk()
{
	if( IsKind() ==KIND_FURIKAE )
		return	l_cmp( DEBT_TOTAL.DP_VAL, CRED_TOTAL.DP_VAL );
	else
		return	0;
}




//---------------------------------------------------------------------------
// インプットバッファから消費税用データ作成
//	借方・貸方の消費税属性をサポートするため、借方・貸方が両方とも
//	消費税科目の場合は、２仕訳作成する
//
//	CDBINPDataRec *pdata		仕訳格納レコード(最低２レコードのエリアが必要)
//	int ln					仕訳行番号
//	int lntype				0=画面上の仕訳行, 1=データ上の仕訳行 [int ln の種別]
//	int mkauto				1:自動仕訳時の呼び出し
//	return	作成仕訳データ数( 2 or 1 )
//			-1 = 科目未入力など仕訳作成せず
//----------------------------------------------------------------------------
int CDnpBaseClass::mkSY_CREC( CDBINPDataRec *pdata, int ln, int lntype/*=0*/, int mkauto/*=0*/)
{
CString sv;
struct _DBKNREC *pKn;
int pn, i, dbsg = 0, n = -1;
CDBINPDataRec	tmp;

int dt_ln, displn;

	if( ! lntype ) {
		dt_ln = get_dataline( ln );
		displn = ln;
	}
	else {
		dt_ln = ln;
		displn = 1;
	}

int DEBT_ln = _getMAP( dt_ln, FD_DEBT_PN ),
	CRED_ln = _getMAP( dt_ln, FD_CRED_PN );

int prev_dbtln, prev_creln;
	prev_dbtln = prev_creln = -1;

	m_SYdataCnt = -1;

	for( i = 0; i < 2; i++ ) {
		(pdata+i)->Reset();

		if( i == 0 )	(pdata+i)->m_seq = SEQ_DATA[ dt_ln-1 ];
		else			(pdata+i)->m_seq = pdata->m_seq;
		//日付を作成
		if( i == 0 ) {
			setDATE_CREC( (pdata+i) );
		}
		(pdata+i)->m_dbt	= SYOGT;
		(pdata+i)->m_cre	= SYOGT;
	}
	// 日付をコピー
	pdata[1].m_ddate	= pdata[0].m_ddate;
	pdata[1].m_mofs		= pdata[0].m_mofs;	
	pdata[1].m_ksign	= pdata[0].m_ksign;

	int dbmn, cbmn;
	dbmn = cbmn = -1;

	if( IsKind() == KIND_FURIKAE ) {
		if( ! GetDNP_POS( displn, FD_DSKBN_PN )->dp_enable || 
			! GetDNP_POS( displn, FD_CSKBN_PN )->dp_enable )
			return -1;

		if (m_DI.IsBumon()) {
			if (m_FuriMode == FURI_DATABMON) {
				dbmn = _getDATA(dt_ln, FD_DBMN_PN)->DP_BMN;
				cbmn = _getDATA(dt_ln, FD_CBMN_PN)->DP_BMN;;
			}
			else {
				dbmn = getDATA(DP_BMN_PN)->DP_BMN;
				cbmn = dbmn;
			}
		}
	}
	else {
		pn = (IsKind() == KIND_NYUKIN) ? FD_CSKBN_PN : FD_DSKBN_PN;
	
		if( ! GetDNP_POS( displn, pn )->dp_enable )
			return -1;

		if (m_DI.IsBumon()) {
			dbmn = getDATA(DP_BMN_PN)->DP_BMN;
			cbmn = dbmn;
		}
	}

	if( DEBT_ln == -1 && CRED_ln == -1 )
		return -1;

	tmp.Reset();

	tmp.m_seq = SEQ_DATA[ dt_ln-1 ];
	//日付を作成
	tmp.m_ddate	= pdata[0].m_ddate;
	tmp.m_mofs	= pdata[0].m_mofs;	
	tmp.m_ksign	= pdata[0].m_ksign;

	tmp.m_dbt	= SYOGT;
	tmp.m_cre	= SYOGT;
	tmp.m_dbmn = dbmn;
	tmp.m_cbmn = cbmn;

	dbsg |= _getCND( dt_ln, FD_DEBT_PN )->INP_sg ? 0x01 : 0;
	dbsg |= _getCND( dt_ln, FD_CRED_PN )->INP_sg ? 0x02 : 0;

#ifdef OLD_CLOSE
	if( !dbsg )
		return -1;
#endif

	if( IsKind() == KIND_NYUKIN ) {
		if( dbsg == 0x01 ) {
			if( _getCND( dt_ln, FD_CRVAL_PN )->INP_sg == FALSE )	return	-1;
		}
	}
	else if( IsKind() == KIND_SYUKKIN ) {
		if( dbsg == 0x02 ) {
			if( _getCND( dt_ln, FD_DBVAL_PN )->INP_sg == FALSE )	return	-1;
		}
	}

	int divsgn = 0;

	// 借方
	if( dbsg & 0x01 )
	{
		tmp.m_dbt = _getDATA( DEBT_ln, FD_DEBT_PN )->DP_CdNum;
		tmp.m_dbr = _getDATA(DEBT_ln, FD_DEBT_PN)->DP_BRN;
		// 消費税科目の場合サインON
		pKn = DB_PjisToKnrec( tmp.m_dbt, m_pZm );
		if( pKn ) {
			if( m_DI.km_syattr( pKn ) )
				divsgn |= 0x01;
		}
	}
	else if( DEBT_ln != -1 && !mkauto )
	{
		// 無い場合は支配科目が消費税科目かどうか調べる

		if( ! FuriSyogPosition( dt_ln, FD_DEBT_PN ) ) {
			sv = pdata->m_dbt;
			tmp.m_dbt = _getDATA( DEBT_ln, FD_DEBT_PN )->DP_CdNum;
			tmp.m_dbr = _getDATA(DEBT_ln, FD_DEBT_PN)->DP_BRN;
			pKn = DB_PjisToKnrec( tmp.m_dbt, m_pZm );
			if( pKn ) {
				if( ! m_DI.km_syattr( pKn ) )
						tmp.m_dbt = sv;	// 戻し
				else {
					divsgn |= 0x01;
					prev_dbtln = DEBT_ln;
				}
			}
		}
	}

	// 貸方
	if( dbsg & 0x02 )
	{
		tmp.m_cre = _getDATA( CRED_ln, FD_CRED_PN )->DP_CdNum;
		tmp.m_cbr = _getDATA(CRED_ln, FD_CRED_PN)->DP_BRN;
		pKn = DB_PjisToKnrec( tmp.m_cre, m_pZm );
		if( pKn ) {
			if( m_DI.km_syattr( pKn ) )
				divsgn |= 0x02;
		}
	}
	else if( CRED_ln != -1 && !mkauto )
	{
		// 無い場合は支配科目が消費税科目かどうか調べる
		if( ! FuriSyogPosition( dt_ln, FD_CRED_PN ) ) {
			sv = tmp.m_cre;
			tmp.m_cre = _getDATA( CRED_ln, FD_CRED_PN )->DP_CdNum ;
			tmp.m_cbr = _getDATA(CRED_ln, FD_CRED_PN)->DP_BRN;
			pKn = DB_PjisToKnrec( tmp.m_cre, m_pZm );
			if( pKn ) {
				if( ! m_DI.km_syattr( pKn ) )
						tmp.m_cre = sv;	// 戻し
				else {
					divsgn |= 0x02;
					prev_creln = CRED_ln;
				}
			}
		}
	}
//	MyTrace("mkSY_CREC inv(1) = %s\n", tmp.m_invno);

	//金額,税額
	if( IsKind() == KIND_NYUKIN ) {
		memmove( &tmp.m_val, &_getDATA( dt_ln, FD_CRVAL_PN )->DP_VAL, 6 );
		memmove( &tmp.m_zei, &_getDATA( dt_ln, FD_CRZEI_PN )->DP_VAL, 6 );
		memmove( &tmp.m_dsign[0], &_getDATA( dt_ln, FD_CSKBN_PN )->SYDATA._DSGN, SZ_DSGN );	// 消費税区分
		memmove( &tmp.m_taika, &_getDATA( dt_ln, FD_CSKBN_PN )->SYDATA.TKsyohi, 6 );
		memmove( &tmp.m_invno, &_getDATA(dt_ln, FD_CSKBN_PN)->SYDATA.TKinvno, SZ_INVNO);
	}
	else if( IsKind() == KIND_SYUKKIN ) {
		memmove( &tmp.m_val, &_getDATA( dt_ln, FD_DBVAL_PN )->DP_VAL, 6 );
		memmove( &tmp.m_zei, &_getDATA( dt_ln, FD_DBZEI_PN )->DP_VAL, 6 );
		memmove( &tmp.m_dsign[0], &_getDATA( dt_ln, FD_DSKBN_PN )->SYDATA._DSGN, SZ_DSGN );
		memmove( &tmp.m_taika, &_getDATA( dt_ln, FD_DSKBN_PN )->SYDATA.TKsyohi, 6 );
		memmove( &tmp.m_invno, &_getDATA(dt_ln, FD_DSKBN_PN)->SYDATA.TKinvno, SZ_INVNO);
	}

//	MyTrace("mkSY_CREC inv(2) = %s\n", tmp.m_invno);

	BOOL bDivSgn = FALSE;
	BOOL bTandok = FALSE;
	int inpline = 1;
	int syog_mode_tandok_dbt = 0;

	if( IsJob() == JOB_SCAN ) {
		if( IsScanLine( dt_ln ) ) {
			if( m_DPscan.IsFukugoSiwake(dt_ln) )
				bDivSgn = TRUE;
			else
				bTandok = TRUE;

			//諸口詰めない場合の借方でも貸方の消費税属性を下で取得していたため
			if (bTandok && SyogSiwakeMode()) {
				if (dbsg & 0x01) {
					syog_mode_tandok_dbt = 1;
				}
			}

			inpline = 0;
		}
	}

	// 入力ライン
	if( inpline ) {
		if( IsType() == TYPE_TANDOKU )
			bTandok = TRUE;

		// 借貸
		if( ( (divsgn & 0x03) == 0x03 || IsType() == TYPE_FUKUGOU ) ) {
			bDivSgn = TRUE;
		}
	}

	if( IsKind() == KIND_FURIKAE && bTandok && ! bDivSgn ) {
		// 単独仕訳なので、借方／貸方同じ金額
		if( (divsgn & 0x03) == 0x01 || syog_mode_tandok_dbt ) {
			memmove( &tmp.m_val, &_getDATA( dt_ln, FD_DBVAL_PN )->DP_VAL, 6 );
			memmove( &tmp.m_zei, &_getDATA( dt_ln, FD_DBZEI_PN )->DP_VAL, 6 );
			memmove( &tmp.m_dsign[0], &_getDATA( dt_ln, FD_DSKBN_PN )->SYDATA._DSGN, SZ_DSGN );
			memmove( &tmp.m_taika, &_getDATA( dt_ln, FD_DSKBN_PN )->SYDATA.TKsyohi, 6 );
			memmove( &tmp.m_invno, &_getDATA( dt_ln, FD_DSKBN_PN)->SYDATA.TKinvno, SZ_INVNO);
		}
		else {
			memmove( &tmp.m_val, &_getDATA( dt_ln, FD_CRVAL_PN )->DP_VAL, 6 );
			memmove( &tmp.m_zei, &_getDATA( dt_ln, FD_CRZEI_PN )->DP_VAL, 6 );
			memmove( &tmp.m_dsign[0], &_getDATA( dt_ln, FD_CSKBN_PN )->SYDATA._DSGN, SZ_DSGN );
			memmove( &tmp.m_taika, &_getDATA( dt_ln, FD_CSKBN_PN )->SYDATA.TKsyohi, 6 );
			memmove( &tmp.m_invno, &_getDATA( dt_ln, FD_CSKBN_PN)->SYDATA.TKinvno, SZ_INVNO);
		}
	}
	// 借方／貸方どちらも消費税科目
	if( IsKind() == KIND_FURIKAE && bDivSgn ) {
		*pdata = tmp;
		pdata->m_cre = SYOGT;
		pdata->m_cbr = -1;
		memmove( pdata->m_val, &_getDATA( dt_ln, FD_DBVAL_PN )->DP_VAL, 6 );
		memmove( pdata->m_zei, &_getDATA( dt_ln, FD_DBZEI_PN )->DP_VAL, 6 );

		int linenum = dt_ln;
		if( prev_dbtln != -1 )	linenum	= prev_dbtln;
		memmove( &pdata->m_dsign[0], &_getDATA( linenum, FD_DSKBN_PN )->SYDATA._DSGN, SZ_DSGN );
		memmove( pdata->m_taika, &_getDATA( dt_ln, FD_DSKBN_PN )->SYDATA.TKsyohi, 6 );
		memmove( pdata->m_invno, &_getDATA(dt_ln, FD_DSKBN_PN)->SYDATA.TKinvno, SZ_INVNO);

		pdata++;
		*pdata = tmp;
		pdata->m_seq++;
		pdata->m_dbt = SYOGT;
		pdata->m_dbr = -1;
		memmove( pdata->m_val, &_getDATA( dt_ln, FD_CRVAL_PN )->DP_VAL, 6 );
		memmove( pdata->m_zei, &_getDATA( dt_ln, FD_CRZEI_PN )->DP_VAL, 6 );

		linenum = dt_ln;
		if( prev_creln != -1 )	linenum	= prev_creln;
		memmove( &pdata->m_dsign[0], &_getDATA( linenum, FD_CSKBN_PN )->SYDATA._DSGN, SZ_DSGN );
		memmove( pdata->m_taika, &_getDATA( dt_ln, FD_CSKBN_PN )->SYDATA.TKsyohi, 6 );
		memmove( pdata->m_invno, &_getDATA(dt_ln, FD_CSKBN_PN)->SYDATA.TKinvno, SZ_INVNO);
		n = 2;
	}
	else {
		*pdata = tmp;
		n = 1;
	}

#ifdef OLD_KEEP
	// 消費税関係
	memmove( &pdata->m_dsign, &_getDATA( ln, SKBN_PN )->SYDATA._DSGN, SZ_SYZDSGN );	// 消費税区分
//	memmove( &pdata->_TKS[m_pZm->zvol->TK_LN * 2], _getDATA( ln, SKBN_PN )->SYDATA.TKsyohi, 8 );	// 摘要消費税区分
#endif 
	// 作成データ数を保存 (振伝時、setSY_CREC を呼び出すときに使う)
	m_SYdataCnt = n;

	return n;

}


//------------------------------------------------------------------------------
// 入力日付データからレコードへ日付をセット
//
//	CDBINPDataRec *pdata	仕訳データ
//	return -1 = Error
//------------------------------------------------------------------------------
int CDnpBaseClass::setDATE_CREC( CDBINPDataRec *pdata )
{
	//日付を作成
	int ksgn, date, tmpymd, ymd;
	VDPAC vdp;
	char buf[36];
	ksgn = KESY_SG_GET( getDATA( DP_MM_PN )->DP_BCDDAY[0] );

	switch( ksgn ) {
	case 1 :	date = 6100;	break;
	case 2 :	date = 6200;	break;
	case 3 :	date = 6300;	break;
	case 4 :	date = 0;		break;
	default:
		sprintf_s( buf, sizeof buf, "%02x", (getDATA( DP_MM_PN )->DP_BCDDAY[0] & 0x1f) );
		date = atoi( buf );
		date *= 100;
		break;
	}
	sprintf_s( buf, sizeof buf, "%02x", getDATA( DP_DD_PN )->DP_BCDDAY[0] );
	tmpymd = atoi( buf );
	date += tmpymd;

	int gengo;
	if( db_vd_dpacGen( 1, 0, date, &vdp, m_pZm ) != 0 )
		return -1;
	// 平成から西暦へ
	db_datecnvGen( vdp.Dgengo, vdp.Dymd, &gengo, &ymd, 1, 0 );

	pdata->m_ddate	= ymd;
	pdata->m_mofs	= vdp.Doffset;
	pdata->m_ksign	= ksgn ? 1 : 0;

	return 0;
}


//------------------------------------------------------------------------------
// 消費税用データからインプットバッファに消費税関係セット
//
//	CDBINPDataRec *pdata	仕訳データ
//	int		dcsw		借方・貸方サイン(振替伝票のとき使用)
//	int		ln			仕訳行番号
//	int		lntype		0:伝票上の表示行, 1:データ行 [int ln の種別]
//
//	この関数を呼ぶ前にSyIniz()しておく必要あり
//------------------------------------------------------------------------------
int CDnpBaseClass::setSY_CREC( CDBINPDataRec *pdata, int dcsw, int ln, int lntype/* = 0*/ )
{
//	MyTrace("setSY_CREC(ln=%d), seq = %d, dbt=%s, cre=%s, invno = %s\n", ln, pdata->m_seq, pdata->m_dbt, pdata->m_cre, pdata->m_invno);

	struct _DenpData *keybuf;
	struct _DNPLINE_CND* pcnd;
	char	buf[64];
	int pn, zeipn, valpn, clear, kmkpn, dbtsgn;
	int	furi_tan = 0;
	clear = dbtsgn = 0;

	int dt_ln;
	int displn;
	
	if( ! lntype ) {
		dt_ln	= get_dataline( ln );
		displn	= ln;
	}
	else {
		dt_ln	= ln;
		displn	= 1;	//参考用に１行目
	}

//FILETrace( "CDnpBaseClass::setSY_CREC ln = %d, m_TopIdx = %d, lntype = %d, dt_ln = %d, displn = %d\n", ln, m_TopIdx, lntype, dt_ln, displn );

	switch( IsKind() ) {
	case KIND_FURIKAE:
		if( IsScanLine( dt_ln )) {
			if( ! m_DPscan.IsFukugoSiwake( dt_ln ) ) {
				furi_tan = 1;	//単一仕訳
			}
		}
		if( ! dcsw ) {
			// 借方の消費税処理
			pn = FD_DSKBN_PN;
			kmkpn = FD_DEBT_PN;
			valpn = FD_DBVAL_PN;
			zeipn = FD_DBZEI_PN;

			if( isSYOGT_CODE( pdata->m_dbt ) || ! m_DI.km_syattr( pdata->m_dbt ) )
				clear = 1;
			// 借方／貸方 どちらも消費税科目で、単一仕訳の場合、借方には、消費税を表示しない
			if( m_DI.km_syattr( pdata->m_dbt ) && m_DI.km_syattr( pdata->m_cre ) ) {
				if( m_SYdataCnt == 1 ) {
					clear = 1;
				}
			}
		}
		else {
			// 貸方の消費税処理
			pn = FD_CSKBN_PN;
			kmkpn = FD_CRED_PN;
			valpn = FD_CRVAL_PN;
			zeipn = FD_CRZEI_PN;
			if( isSYOGT_CODE( pdata->m_cre ) || ! m_DI.km_syattr( pdata->m_cre ) ) {
				clear = 1;
				// 伝票修正時、単一仕訳で、借方の消費税が表示されていない[借方は、消費税あり]
				if( IsScanLine( dt_ln )) {
					if( m_SYdataCnt == 1 ) {
						if( m_DI.km_syattr( pdata->m_dbt ) ) {
							if( ! _getCND( dt_ln, FD_DSKBN_PN )->INP_sg )
								dbtsgn = 1;
						}
					}
				}
			}
			else {
				// 伝票修正時、単一仕訳で、借方の消費税が表示されている
				// 貸方にのみ表示する
				if( IsScanLine( dt_ln )) {
					if( m_SYdataCnt == 1 ) {
						if( _getCND( dt_ln, FD_DSKBN_PN )->INP_sg )
							dbtsgn = 1;
					}
				}
			}
		}

		break;
	case KIND_NYUKIN:
		pn = FD_CSKBN_PN;
		kmkpn = FD_CRED_PN;
		valpn = FD_CRVAL_PN;
		zeipn = FD_CRZEI_PN;
		break;
	case KIND_SYUKKIN:
		pn = FD_DSKBN_PN;
		kmkpn = FD_DEBT_PN;
		valpn = FD_DBVAL_PN;
		zeipn = FD_DBZEI_PN;
		break;
	default:
		return -1;
		break;
	}

	// 金額・科目が未入力なら、消費税をクリア
	if( ! _getCND( dt_ln, valpn )->INP_sg && ! _getCND( dt_ln, kmkpn )->INP_sg )
		clear = 1;

	keybuf	= _getDATA( dt_ln, pn );
	pcnd	= _getCND( dt_ln, pn );

	struct _SY_MSG_PACK2 smpk;
	int ritu_sw, zeikbn_sw, uri_sw, zei_sw, swk_sw, tai_sw, toku_sw;

	ritu_sw = zeikbn_sw = uri_sw = zei_sw = swk_sw = tai_sw = toku_sw= 0;

	_getCND( dt_ln, pn )->INP_sg = FALSE;

	if( IsKind() == KIND_FURIKAE ) {
		if( ! GetDNP_POS( displn, FD_DSKBN_PN )->dp_enable || 
			! GetDNP_POS( displn, FD_CSKBN_PN )->dp_enable )
			return -1;
	}
	else {
		if( ! GetDNP_POS( displn, pn )->dp_enable )
			return -1;
	}

	// 消費税表示テキスト
	LPCTSTR systr;

	if( ! clear ) {
		pcnd->INP_sg = TRUE;

///		memmove( &pdata->m_dsign[0], keybuf->SYDATA._DSGN, SZ_DSGN );
		systr = m_DI.sy_chk_txt( pdata, &smpk );
//		m_pZm->GetCDBData( pdata );

		if( keybuf->SYDATA.swk_sw > 0 )		m_pSy->SyFnc( keybuf->SYDATA.swk_sw );
		if( keybuf->SYDATA.ritu_sw > 0 )	m_pSy->SyFnc( keybuf->SYDATA.ritu_sw );
		if( keybuf->SYDATA.zeikbn_sw > 0 )	m_pSy->SyFnc( keybuf->SYDATA.zeikbn_sw );
		if( keybuf->SYDATA.uri_sw > 0 )		m_pSy->SyFnc( keybuf->SYDATA.uri_sw );
		if( keybuf->SYDATA.toku_sw > 0 )	m_pSy->SyFnc( keybuf->SYDATA.toku_sw );
		if( keybuf->SYDATA.kts_sw > 0 )	m_pSy->SyFnc( keybuf->SYDATA.kts_sw );
		//免税事業者からの
		_DSGN_MenWariFnc(keybuf);

		m_DI.GetCDBData( pdata ); //上より移動

		systr = m_DI.sy_chk_txt( pdata, &smpk );

		m_DI.GetCDBData( pdata );

		::ZeroMemory( keybuf->SYDATA.SKBNtxt, sizeof keybuf->SYDATA.SKBNtxt );
		strcpy_s( keybuf->SYDATA.SKBNtxt, sizeof keybuf->SYDATA.SKBNtxt, systr );

		swk_sw	= smpk.SY_MSG_SWKBN->sel_no;
		ritu_sw	= smpk.SY_MSG_RITU->sel_no;
		zeikbn_sw	= smpk.SY_MSG_ZEI->sel_no;
		uri_sw		= smpk.SY_MSG_URISIRE->sel_no;
		toku_sw		= smpk.SY_MSG_TOKTEI->sel_no;
		tai_sw		= smpk.SY_MSG_TAIKAKBN->sel_no;

		memcpy(pdata->m_invno, keybuf->SYDATA.TKinvno, SZ_INVNO);
		m_pZm->dbdata->invno = pdata->m_invno;
	}
	else {
		pcnd->INP_sg = FALSE;

		::ZeroMemory( keybuf->SYDATA.SKBNtxt, sizeof keybuf->SYDATA.SKBNtxt );

		if( IsScanLine( dt_ln )) {
			if( IsKind() == KIND_FURIKAE ) {
				int re_sgn = 0;
				if( ! dcsw ) {
					// 借方の消費税処理
					if( ! m_DI.km_syattr( pdata->m_dbt ) && isSYOGT_CODE( pdata->m_cre ) )
						re_sgn = 1;
				}
				else {
					// 貸方の消費税処理
					if( isSYOGT_CODE( pdata->m_dbt ) && ! m_DI.km_syattr( pdata->m_cre ) )
						re_sgn = 1;

					if( furi_tan ) {
						if( m_DI.km_syattr( pdata->m_dbt ) && !m_DI.km_syattr( pdata->m_cre ) ) {
							dbtsgn = 1;
						}
					}
				}
				if( ! m_DI.km_syattr( pdata->m_dbt ) && ! m_DI.km_syattr( pdata->m_cre ) )
					re_sgn = 1;

				if( re_sgn ) {
					m_DI.GetCDBData( pdata ); //上より移動
					systr = m_DI.sy_chk_txt( pdata, &smpk );
					m_DI.GetCDBData( pdata );
				}
			}
		}
	}

	if( swk_sw ) {
		if( keybuf->SYDATA.swk_def == 0 )
			keybuf->SYDATA.swk_def = smpk.SY_MSG_SWKBN->msg_tbl[swk_sw-1]->code;
	}
	else {
		keybuf->SYDATA.swk_sw = 0;
		keybuf->SYDATA.swk_def = 0;
	}

	if( ritu_sw ) {
		if( keybuf->SYDATA.ritu_def == 0 )
			keybuf->SYDATA.ritu_def = smpk.SY_MSG_RITU->msg_tbl[ritu_sw-1]->code;
	}
	else {
		keybuf->SYDATA.ritu_sw = 0;
		keybuf->SYDATA.ritu_def = 0;
	}

	if( zeikbn_sw ) {
		if( keybuf->SYDATA.zeikbn_def == 0 )
			keybuf->SYDATA.zeikbn_def = smpk.SY_MSG_ZEI->msg_tbl[zeikbn_sw-1]->code;
	}
	else {
		keybuf->SYDATA.zeikbn_sw = 0;
		keybuf->SYDATA.zeikbn_def = 0;
	}

	if( uri_sw ) {
		if( keybuf->SYDATA.uri_def == 0 )
			keybuf->SYDATA.uri_def = smpk.SY_MSG_URISIRE->msg_tbl[uri_sw-1]->code;
	}
	else {
		keybuf->SYDATA.uri_sw = 0;
		keybuf->SYDATA.uri_def = 0;
	}
	// 特定収入
	if( toku_sw ) {
		if( keybuf->SYDATA.toku_def == 0 )
			keybuf->SYDATA.toku_def = smpk.SY_MSG_TOKTEI->msg_tbl[toku_sw-1]->code;
	}
	else {
		keybuf->SYDATA.toku_sw = 0;
		keybuf->SYDATA.toku_def = 0;
	}

	if( tai_sw ) {
		if( keybuf->SYDATA.kts_def == 0 )
			keybuf->SYDATA.kts_def = smpk.SY_MSG_TAIKAKBN->msg_tbl[tai_sw-1]->code;
	}
	else {
		keybuf->SYDATA.kts_sw = 0;
		keybuf->SYDATA.kts_def = 0;
	}

	if( swk_sw || ritu_sw || zeikbn_sw	|| uri_sw || tai_sw || toku_sw || tai_sw )
			_getCND( dt_ln, pn )->INP_sg = TRUE;
	else	_getCND( dt_ln, pn )->INP_sg = FALSE;

	// 税額 再表示
	char zei[6] = {0};
	int sgn = 0;
	int sotomen = 0;
	if( ! clear ) {
		// 税額 手入力 サインON
		if( _getCND( dt_ln, zeipn )->SET_sg ) {
			l_print( buf, _getDATA( dt_ln, zeipn )->DP_VAL, FMT15 );
			if( m_pSy->SyZei( buf ) == 0 ) {
				m_pSy->SyMsg( &smpk );
			}
		}
		if( m_pSy->IsTaikaData( m_pZm->dbdata ) == 2 )	sgn = 1;
		if (!m_DI.is_syohizeisiwake( m_pZm->dbdata ))	sgn = 2;
		else {
			COLORREF back;
			if (m_DI.isnot_defzeigaku(m_pZm->dbdata, back)) {
				sgn = 3;
				if (back != RGB_ZEI_MDFY) {
					sgn++;
				}
			}
			// 輸出仕訳
			if( m_DI.is_yusyutsusiwake(m_pZm->dbdata) )	  sgn = 5;

			//外税 免税事業者等からの
			if( m_DI.is_sotomensiwake(m_pZm->dbdata) ) {
				sotomen = 1;
			}
		}

		l_input( zei, (char*)(LPCTSTR)smpk.SY_MSG_SYZEI );	// 税額をセット
		memmove( pdata->m_zei, zei, 6 );
	}
	else {
		if( ! furi_tan ) {
			::ZeroMemory( pdata->m_zei, sizeof pdata->m_zei );
		}
	}
	memmove( _getDATA( dt_ln, zeipn )->DP_VAL, zei, 6 );
	BOOL bVal = _getCND( dt_ln, valpn )->INP_sg;
	_getCND( dt_ln, zeipn )->INP_sg = bVal;
	_getCND( dt_ln, zeipn )->INP_type = sgn;
	set_sotomen_sign(dt_ln, zeipn, sotomen);

	if( ! lntype ) {
		GetNowDnpDialog()->SetVal( ln, zeipn, zei, sgn );
	}

	int other_pn = 0;
	struct _DenpData *dbt_data = NULL;
	struct _DenpData *cre_data = NULL;
	int	put_sg[2] = {0};

	if( furi_tan ) {
		other_pn = (pn == FD_CSKBN_PN) ? FD_DSKBN_PN : FD_CSKBN_PN;

		if( pn == FD_DSKBN_PN ) {
			dbt_data = keybuf;
			cre_data = _getDATA( dt_ln, other_pn );

			if( ! FuriSyogPosition( dt_ln, pn ) )		put_sg[0] = 1;
			if( ! FuriSyogPosition( dt_ln, other_pn ) )	put_sg[1] = 1;
		}
		else {
			dbt_data = _getDATA( dt_ln, other_pn );
			cre_data = keybuf;

			if( ! FuriSyogPosition( dt_ln, other_pn ) )	put_sg[0] = 1;
			if( ! FuriSyogPosition( dt_ln, pn ) )		put_sg[1] = 1;
		}
	}

	BYTE dsign0, dsign6;
	dsign0 = dsign6 = 0;

	if( IsScanLine( dt_ln )) {
		CDBINPDataRec	dtemp;
		int dc_sw;
		dc_sw = (pn == FD_CSKBN_PN) ? 1 : 0;

		dtemp	=	m_DPscan.GetOriginData( dt_ln, dc_sw );
		dsign0 = dtemp.m_dsign[0];
		dsign6 = dtemp.m_dsign[6];
		MyTrace("★ dtemp.m_seq = %d, dsign6 = %02x", dtemp.m_seq, dsign6 );

		if( m_DI.isSyzdiff_data( pdata, &dtemp ) ) {
			SRCTrace;

			if( dtemp.m_seq > 0 ) {
			//	SYZMDFY_BIT.onbitn( dtemp.m_seq - 1);
				if( furi_tan ) {
					SRCTrace;

					//単一仕訳には 借方／貸方 両方に変更サインをセットする
					dbt_data->SYDATA.sy_chg = 1;
					cre_data->SYDATA.sy_chg = 1;
				}
				else {
					SRCTrace;


					keybuf->SYDATA.sy_chg = 1;
				}
			}
		}
		// 表示
		if( ! lntype ) {
			if( furi_tan ) {
				char skbn_txt[20] = {0};
				if( (m_DI.km_syattr( pdata->m_dbt ) && m_DI.km_syattr( pdata->m_cre )) || 
					( ! m_DI.km_syattr( pdata->m_dbt ) &&   m_DI.km_syattr( pdata->m_cre )) ||
					( ! m_DI.km_syattr( pdata->m_dbt ) && ! m_DI.km_syattr( pdata->m_cre )) ) {

					SRCTrace;

					PutsSyz( ln, FD_DSKBN_PN, skbn_txt, put_sg[0] ? 0 : -1 );
					PutsSyz( ln, FD_CSKBN_PN, cre_data->SYDATA.SKBNtxt, put_sg[1] ? cre_data->SYDATA.sy_chg : -1 );
				}
				else {
					SRCTrace;
					PutsSyz( ln, FD_DSKBN_PN, dbt_data->SYDATA.SKBNtxt, put_sg[0] ? dbt_data->SYDATA.sy_chg : -1 );
					PutsSyz( ln, FD_CSKBN_PN, skbn_txt, put_sg[1] ? 0 : -1 );
				}
			}
			else {
				if( ! FuriSyogPosition( dt_ln, pn )) {
					char sy_chg = keybuf->SYDATA.sy_chg;
					if( clear )	sy_chg = 0;
					PutsSyz( ln, pn, keybuf->SYDATA.SKBNtxt, sy_chg );
				}
			}
		}
	}
	else {
		// 表示
		if( ! lntype ) {
			if( ! FuriSyogPosition( dt_ln, pn )) {
				SRCTrace;
				PutsSyz( ln, pn, keybuf->SYDATA.SKBNtxt, 0 );
			}
		}
	}

	memmove( keybuf->SYDATA._DSGN, &pdata->m_dsign[0], SZ_DSGN );
	if (dsign0 & (BYTE)0xA0) {
		keybuf->SYDATA._DSGN[0] |= (dsign0 & (BYTE)0xA0);
	}
	if (dsign6 & 0x01) {
		keybuf->SYDATA._DSGN[6] |= 0x01;
	}

//MyTrace( "(2)keybuf->SYDATA.sy_chg = %d\n", keybuf->SYDATA.sy_chg );
//BYTE a[16];
//memcpy( a, keybuf->SYDATA._DSGN, 16 );
//MyTrace( "setSY_CREC [dbt=(%s),cre=(%s)]dsign = %02x%02x%02x%02x%02x\n", pdata->m_dbt, pdata->m_cre, a[0], a[1], a[2], a[3], a[4] );

	//
	if( dbtsgn ) {
		setSY_CREC( pdata, 0, ln, lntype );
	}


	return 0;
}


//---------------------------------------------
//	現ポジションが借方・貸方のどちらか？
//	return 0 = 借方, 1 = 貸方
//---------------------------------------------
int CDnpBaseClass::get_dcsgn()
{
	return _get_dcsgn( get_nowpn() );
}

int CDnpBaseClass::_get_dcsgn( int pn )
{
	int dc_sw = -1;

	switch( pn ) {
	case FD_DBVAL_PN:
	case FD_DBZEI_PN:
	case FD_DBMN_PN:
	case FD_DKOJI_PN:
	case FD_DEBT_PN:
	case FD_DSKBN_PN:
		dc_sw = 0;
		break;
	case FD_CRVAL_PN:
	case FD_CRZEI_PN:
	case FD_CBMN_PN:
	case FD_CKOJI_PN:
	case FD_CRED_PN:
	case FD_CSKBN_PN:
		dc_sw = 1;
	}

	return dc_sw;
}



//--------------------------------------------------------------
//	消費税文字列をセット
//
//
//--------------------------------------------------------------
void CDnpBaseClass::PutsSyz( int ln, int pn, char* syztxt, char sy_chg )
{
	DBGFUNCTION;
	MyTrace("sy_chg = %d\n", sy_chg);

	COLORREF back = GetLineBackColor( get_dataline( ln ) );

	if( m_pDENPPAR->nosyzmdf ) {
		if( sy_chg != -1 )	sy_chg = 0;
	}

	GetNowDnpDialog()->PutsSyz( ln, pn, syztxt, sy_chg, back );
}


//---------------------------------------------------------------------
//	借方・貸方の消費税をサポートするため、借方⇔貸方に
//	項目が切り替わったら SyIniz()を呼び出す
//
//	CDBINPDataRec *data	最低2レコードのエリアを渡す(mkSY_CRECで作成)
//	int	dcnt			今回のライン
//	int nowpn			現在ポジション
//	int prevpn			以前のポジション
//---------------------------------------------------------------------
void CDnpBaseClass::DnpSyzInit( CDBINPDataRec *data, int dcnt, int nowpn, int prevpn )
{
	int dc_sw = -1;

	if( IsKind() == KIND_FURIKAE ) {
		if( dcnt >= 1 ) {
			// 借方→貸方へ変わったとき
			if( IsDebtPosition( prevpn ) ) {
				if( IsCredPosition( nowpn ) ) {
					dc_sw = 1;
				}
			}
			else if( IsCredPosition( prevpn ) ) { 
				// 貸方→借方へ変わったとき
				if( IsDebtPosition( nowpn ) ) {
					dc_sw = 0;
				}
			}
			else {
				if( IsCredPosition( nowpn ) ) {
					dc_sw = 1;
				}
				if( IsDebtPosition( nowpn ) ) {
					dc_sw = 0;
				}
			}

			if( m_SYdataCnt == 2 ) {
				if( dc_sw == 0 ) {
					m_DI.DB_SyIniz( data );
				}
				else if( dc_sw == 1 ) {
					m_DI.DB_SyIniz( &data[1] );
				}
			}
			else {
				m_DI.DB_SyIniz( data );
			}
		}
	}
	// 
//	if( dc_sw == -1 )
//		m_DI.DB_SyIniz( data );
}


//--------------------------------------------------------
//	前行 の摘要をコピーする
//	int ln ... 画面伝票上の　行番号(１～最大７)
//--------------------------------------------------------
BOOL CDnpBaseClass::TekiyoCopy( int ln, CString& tekiyo )
{
	BOOL bCopy = FALSE;
	BOOL bRet = FALSE;
	struct _DenpData* prdata;
	int pos = FD_TKY_PN;
	BOOL debt, cred;
	debt = cred = FALSE;

	int dt_ln = get_dataline( ln );
	int	len =	kjlen( _getDATA( dt_ln, pos )->DP_TKY, sizeof _getDATA( dt_ln, pos )->DP_TKY );

	// 伝票修正時は、コピーしない
	// 伝票修正時も、コピーする [01.21 /10]
	if( (IsJob() == JOB_INPUT &&  !_getCND( dt_ln, pos )->INP_sg) ||
		(IsJob() == JOB_SCAN && ! len ) ) {
		// 科目が未入力行も摘要をコピーしない
		if( IsKind() == KIND_FURIKAE ) {
			debt = _getCND( dt_ln, FD_DEBT_PN )->INP_sg;
			cred = _getCND( dt_ln, FD_CRED_PN )->INP_sg;

			if( debt != FALSE || cred != FALSE )
				bCopy = TRUE;
		}
		else if( IsKind() == KIND_NYUKIN ) {
			cred = _getCND( dt_ln, FD_CRED_PN )->INP_sg;
			if( cred )	bCopy = TRUE;
		}
		else {
			debt = _getCND( dt_ln, FD_DEBT_PN )->INP_sg;
			if( debt )	bCopy = TRUE;
		}
	}

	if( bCopy && (prdata = _getPRDATA( dt_ln, pos )) ) {

		::ZeroMemory( _getDATA( dt_ln, pos )->DP_TKY, sizeof _getDATA( dt_ln, pos )->DP_TKY );
		if( prdata->DP_TKY[0] ) {
			tekiyo = prdata->DP_TKY;
			bRet = TRUE;

			//摘要の消費税属性で科目の消費税を変更する。[10.28 /11]
			if( m_pSy->IsSyohizeiMaster() && IsJob() == JOB_INPUT ) {

				struct _DenpData* pr_dsykb, *pr_csykb;

				pr_dsykb = _getPRDATA(dt_ln, FD_DSKBN_PN );
				pr_csykb = _getPRDATA(dt_ln, FD_CSKBN_PN );

				struct _DenpData* dsybuf, *csybuf, *keybuf;
				dsybuf = _getDATA( dt_ln, FD_DSKBN_PN );
				csybuf = _getDATA( dt_ln, FD_CSKBN_PN );
				CDBINPDataRec* drec = m_CREC;
				int max = 1;
				int pn, i;

				keybuf = NULL;

				if( IsKind() == KIND_FURIKAE ) {
					// 振替伝票で借方／貸方 に消費税あり
					if( m_SYdataCnt == 2 ) {
						max = 2;

						if( pr_dsykb != NULL ) {
							dsybuf->SYDATA.TKattr = pr_dsykb->SYDATA.TKattr;	//消費税コード 
							//貸方 前行データなし
							if( pr_csykb == NULL ) {
								csybuf->SYDATA.TKattr = dsybuf->SYDATA.TKattr;
							}
						}

						if( pr_csykb != NULL ) {
							csybuf->SYDATA.TKattr = pr_csykb->SYDATA.TKattr;	//消費税コード
							//借方 前行データなし
							if( pr_dsykb == NULL ) {
								dsybuf->SYDATA.TKattr = csybuf->SYDATA.TKattr;
							}
						}
					}
					else {
						// 消費税科目のみ
						if( ! isSYOGT_CODE( drec->m_dbt ) && m_DI.km_syattr( drec->m_dbt ) ) {
							if( pr_dsykb != NULL ) {
								dsybuf->SYDATA.TKattr =  pr_dsykb->SYDATA.TKattr;
							}
							keybuf = dsybuf;
							pn	=	FD_DBZEI_PN;
						}

						if( ! isSYOGT_CODE( drec->m_cre ) && m_DI.km_syattr( drec->m_cre ) ) {
							if( pr_csykb != NULL ) {
								csybuf->SYDATA.TKattr = pr_csykb->SYDATA.TKattr;
							}
							keybuf = csybuf;
							pn	=	FD_CRZEI_PN;
						}
					}
				}
				else if( IsKind() == KIND_SYUKKIN ) {
					if( pr_dsykb != NULL ) {
						dsybuf->SYDATA.TKattr = pr_dsykb->SYDATA.TKattr;
					}
					keybuf = dsybuf;
					pn	=	FD_DBZEI_PN;
				}
				else {
					if( pr_csykb != NULL ) {
						csybuf->SYDATA.TKattr = pr_csykb->SYDATA.TKattr;
					}
					keybuf = csybuf;
					pn	=	FD_CRZEI_PN;
				}

				int disp_syz = 0;

				if( max == 2 ) {
					for( i = 0; i < max; i++ ) {
						keybuf = (i ==0) ? dsybuf : csybuf;
						m_DI.DB_SyIniz( (drec+i) );

						pn = (i ==0) ? FD_DBZEI_PN : FD_CRZEI_PN;
						set_skbn_selsw( (drec+i), keybuf, pn );

						if( IsTekiyoSyzChg( &keybuf->SYDATA ) )  disp_syz++;
					}
				}
				else if( keybuf != NULL && IsTekiyoSyzChg(&keybuf->SYDATA) ) {
					set_skbn_selsw( drec, keybuf, pn );
					disp_syz++;
				}

				//消費税再表示
				int kind;
				kind = IsKind();

				if( disp_syz ) {
					if( IsKind() == KIND_FURIKAE ) {
						if( max == 2 ) {
							SRCTrace;
							PutsSyz( get_nowln(), FD_DSKBN_PN, dsybuf->SYDATA.SKBNtxt, dsybuf->SYDATA.sy_chg );
							PutsSyz( get_nowln(), FD_CSKBN_PN, csybuf->SYDATA.SKBNtxt, csybuf->SYDATA.sy_chg );
						}
						else {
							SRCTrace;
							pn = (pn == FD_DBZEI_PN) ? FD_DSKBN_PN : FD_CSKBN_PN;
							PutsSyz( get_nowln(), pn, keybuf->SYDATA.SKBNtxt, keybuf->SYDATA.sy_chg );
						}
					}
					else {
						SRCTrace;
						pn = (kind == KIND_SYUKKIN) ? FD_DSKBN_PN : FD_CSKBN_PN;
						PutsSyz( get_nowln(), pn, keybuf->SYDATA.SKBNtxt, keybuf->SYDATA.sy_chg );
					}
				}
			}
		}
	}

	return bRet;
}

//--------------------------------------------------------
//	現在処理伝票のダイアログを取得
//
//--------------------------------------------------------
CDnpBaseDlg* CDnpBaseClass::GetNowDnpDialog()
{
	switch( IsKind() )
	{
	case KIND_FURIKAE:	return m_pFuri;
	case KIND_NYUKIN:	
	case KIND_SYUKKIN:
		return m_pNSdlg;
		break;
	}
	return NULL;
}

//---------------------------------------------
//	消費税入力画面表示
//	int ln		現行
//	int pn		現ポジション
//	CRect rect	diag 矩形(スクリーン座標)
//
//---------------------------------------------
int CDnpBaseClass::SkbnInDisp( int ln, int pn, CRect rect )
{
	int sw;

	if (m_pSyzInp == NULL)
	{
		ErrMsgSet( ERROR_ENV, "消費税入力オブジェクトがありません。");
		return -1;
	}
	else
	{
		CSize size;

		m_pSyzInp->GetCrntRect( size );
		CRect	irect;
		irect = rect;
		m_pDnpParent->ScreenToClient( irect );
	//	irect.bottom += irect.Height();

		int dt_ln = get_dataline( ln );

		if( ! _getCND( dt_ln, pn )->INP_sg )
			return -1;

		if( IsKind() == KIND_FURIKAE ) {
			if( pn == FD_DSKBN_PN ) {
				sw = (ln <= 2) ? SYP_BOTTOMRIGHT : SYP_TOPRIGHT;
			}
			else {
				sw = (ln <= 2) ? SYP_BOTTOMLEFT : SYP_TOPLEFT;
			}
		}
		else {
			sw = (ln <= 2) ? SYP_BOTTOMLEFT : SYP_TOPLEFT;
		}

		BOOL bOK;
		bOK = (_getCND( dt_ln, pn )->EDT_sg <= 1) ? TRUE : FALSE;

		sw = m_pSyzInp->DialogON( bOK, irect, sw, m_SET_SYZINP_PN );

		//セットポジションを元に戻す
		m_SET_SYZINP_PN = -1;
	}

	return sw;
}


//---------------------------------------------------------------------
// 科目欄 入力エリア 引き延ばし＆戻し
//	int ln ... 画面伝票上の　行番号(１～最大７)
//---------------------------------------------------------------------
void CDnpBaseClass::kamoku_expan(int ln, int pn, CRect* rc, BOOL exp_sw, BOOL bOnSize/*=FALSE*/)	// ln = 該当ライン
{
	CDnpBaseDlg* dlg;
	UINT	id = IDC_ICSDBEDT_KMINP;
	dlg = GetNowDnpDialog();

	//FILETrace("tgnum_expan ln %d, exp_sw = %d\n", ln, exp_sw );

	CWnd* pwnd = GetNowDnpDialog()->GetDlgItem(id);
	ASSERT(pwnd != NULL);

	CRect rect, dlgrc, tgnrc;
	CString buf;

	if( exp_sw ) {	// 科目欄拡大
		rect = *rc;

		dlg->ScreenToClient(rect);
		pwnd->SetWindowPos(NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
		pwnd->MoveWindow(rect);
		pwnd->UpdateWindow();

		// データをセット
		int dt_ln = get_dataline(ln);
		struct _KamokuData check = { 0 };
		memcpy(check.kd_code, _getDATA(dt_ln, pn)->DP_CdNum, KMKCD_LN);
		check.kd_eda = _getDATA(dt_ln, pn)->DP_BRN;
		strcpy_s(check.kd_name, sizeof check.kd_name, _getDATA(dt_ln, pn)->DP_KNAM);

		DBkamokudata_set(dlg, id, &check, m_pZm);
		TRACE("DBEDITset(%d) keybuf->DP_BRN = %d\n", __LINE__, _getDATA(dt_ln, pn)->DP_BRN);

		m_pDnpParent->PostMessage(WM_RETROFOCUS2, id);
	}
	else {
		if( !bOnSize ) {
			struct _KamokuData check = { 0 };
			check.kd_eda = -1;
			DBkamokudata_set(dlg, id, &check, m_pZm);
		}
		pwnd->MoveWindow(0, 0, 0, 0);
	}
}



// 変動事由を入力できるか？
//	int line ... 画面伝票上の　行番号(１～最大７)
BOOL CDnpBaseClass::IsHjiyuInput( int line/*=-1*/ )
{
	int ln;
	if( line != -1 )	ln = line;
	else				ln = get_nowln();
	int dcsw = 0;
	
	int dt_ln = get_dataline( ln );

	// 現在行の借方・貸方どちらかの科目・金額入力がないと
	// 変動事由をセットできない
	if( _getCND( dt_ln, FD_DBVAL_PN )->INP_sg && _getCND( dt_ln, FD_DEBT_PN )->INP_sg ) {
		dcsw |= 0x01;
	}
	
	if( _getCND( dt_ln, FD_CRVAL_PN )->INP_sg && _getCND( dt_ln, FD_CRED_PN )->INP_sg ) {
		dcsw |= 0x02;
	}

	CString dbt, cre;
	get_linekamokucode( ln, dbt, cre );

	if( ! dcsw )	return FALSE;

	BOOL ret = FALSE;

	if( m_DI.IsHjiyuItemGyosyu() != 0 ) {
		if( IsHjiyuOrNPfuri( dbt, cre ) )
			ret = TRUE;
	}

	return ret;
}



//----------------------------------------------
// 変動事由
//	int ln ... 画面伝票上の　行番号(１～最大７)
//----------------------------------------------
int CDnpBaseClass::FocusHjiyu( int ln, CRect rect )
{
	HJIYU_DATA hd = {0};

	CString dbt, cre;
	int dcsw = 0;
	int ret = -1;
	int dt_ln = get_dataline( ln );

	// 現在行の借方・貸方どちらかの科目・金額入力がないと
	// 変動事由をセットできない
	if( _getCND( dt_ln, FD_DBVAL_PN )->INP_sg && _getCND( dt_ln, FD_DEBT_PN )->INP_sg ) {
		dcsw |= 0x01;
	}
	
	if( _getCND( dt_ln, FD_CRVAL_PN )->INP_sg && _getCND( dt_ln, FD_CRED_PN )->INP_sg ) {
		dcsw |= 0x02;
	}

	get_linekamokucode( ln, dbt, cre );

	// 選択画面に変動事由を
	SelPar sel;
	if( m_pZm->zvol->apno < 0x50 )	sel.sel_pn = SL_HJIYU_PN;
	else {
		if( IsSyafuKaisei( m_pZm->zvol ) )	sel.sel_pn = SL_NAIBU_PN;
		else								sel.sel_pn = SL_NPFURI_PN;
	}
	
	if( dcsw ) {
		sel.dbt = dbt;
		sel.cre = cre;
	}

	if( m_SelDispPN == SL_NAIBU_PN && sel.sel_pn == SL_NAIBU_PN);
	else {
		m_Seldata.SelectJob( &sel, -1 );
	}

	if( ! dcsw )	return -1;

	// 社会福祉改正マスターは内部取引常に可能
	if( IsHjiyuOrNPfuri( dbt, cre ) || IsSyafuKaisei( m_pZm->zvol ) ) {
		hd.hjy_code = _getDATA( dt_ln, FD_HJIYU_PN )->DP_BMN;
		hd.hjy_nbcd = _getDATA( dt_ln, FD_HJIYU_PN )->NAIBCD;

		CString str;
		_get_hjiyustr( hd.hjy_code, hd.hjy_nbcd, str );
		strcpy_s( hd.hjy_name, sizeof hd.hjy_name, str );

		m_pHjiyuInp->SetHjiyuData( &hd, FALSE );
		if( m_pZm->zvol->apno < 0x50 )	m_pHjiyuInp->set_mode( 0 );
		else {
			if( IsSyafuKaisei( m_pZm->zvol ) )	
				m_pHjiyuInp->set_mode( 2 );
			else if( IsGakkouH27Master( m_pZm ) )
				m_pHjiyuInp->set_mode( 4 );
			else
				m_pHjiyuInp->set_mode( 1 );
		}
		HjiyuInpDisp( rect );

		ret = 0;
	}

	return ret;
}


//------------------------------------------------------------------------
//	変動事由を表示
//		CRect rect	表示位置の基本となる入力矩形( スクリーン座標 )
//
//------------------------------------------------------------------------
void CDnpBaseClass::HjiyuInpDisp( CRect rect )
{
	if (m_pHjiyuInp == NULL)
	{
		ErrMsgSet( ERROR_ENV, "変動事由入力オブジェクトがありません。");
		return;
	}
	else
	{
		CSize size;
		m_pHjiyuInp->GetCrntRect( size );

		int sw, dt_ln;
		dt_ln = get_nowln();
		CRect irect;
		irect = rect;

		m_pDnpParent->ScreenToClient( &irect );
		irect.left = irect.right;

		BOOL bOK;
		bOK = (_getCND( dt_ln, FD_HJIYU_PN )->EDT_sg <= 1) ? TRUE : FALSE;
		if( m_DI.IsConFirmMaster() )	bOK = FALSE;

		sw = m_pHjiyuInp->DialogON( bOK, irect, HJY_TOPLEFT );
	}
}




//---------------------------------------------------------------------------------
//	付箋入力フォーカス
//		int		ln		画面伝票上の　行番号(１～最大７)
//		CRect	rect	表示位置の基本となる入力矩形( スクリーン座標 )
//
//---------------------------------------------------------------------------------
int CDnpBaseClass::FocusHusen( int ln, CRect rect )
{
	int dt_ln = get_dataline( ln );

	_getCND( dt_ln, FD_HUSEN_PN )->INP_sg = TRUE;
	set_nowpn( FD_HUSEN_PN );

	HSEN_DATA crec = {0}, srec = {0};
	int prev_ln;
	crec.hsen_inpsg	= _getCND( dt_ln, FD_HUSEN_PN )->INP_sg;
	crec.hsen_sel	= _getDATA(dt_ln, FD_HUSEN_PN )->DP_HUSEN;
	strcpy_s( crec.hsen_cmt, sizeof crec.hsen_cmt, (const char*)_getDATA(dt_ln, FD_HUSEN_PN )->DP_HCMNT );
	
	if( ln > 1 ) {
		prev_ln = dt_ln-1;
		srec.hsen_inpsg	= _getCND( prev_ln, FD_HUSEN_PN )->INP_sg;
		srec.hsen_sel	= _getDATA( prev_ln, FD_HUSEN_PN )->DP_HUSEN;
		strcpy_s( srec.hsen_cmt, sizeof srec.hsen_cmt, (const char*)_getDATA( prev_ln, FD_HUSEN_PN )->DP_HCMNT );
	}
	else {
		srec = crec;
	}

	m_pHusenInp->SetHusenData( &crec, &srec );
	HusenInpDisp( rect );

	return 0;
}

//---------------------------------------------------------------------------------
//	付箋入力表示
//		CRect rect	表示位置の基本となる入力矩形( スクリーン座標 )
//
//---------------------------------------------------------------------------------
void CDnpBaseClass::HusenInpDisp( CRect rect )
{
	if (m_pHusenInp == NULL)
	{
		ErrMsgSet( ERROR_ENV, "付箋入力オブジェクトがありません。");
		return;
	}
	else
	{
		int sw;
		CSize size;
		m_pHusenInp->GetCrntRect( size );
		CRect	irect;
		irect = rect;

		int ln = get_nowln();

		m_pDnpParent->ScreenToClient( &irect );
		irect.left = irect.right;

		BOOL bOK;
		bOK = (_getCND( get_dataline( ln ), FD_HUSEN_PN )->EDT_sg <= 1) ? TRUE : FALSE;
		if( m_DI.IsConFirmMaster() )	bOK = FALSE;

		sw = m_pHusenInp->DialogON( bOK, irect, HSN_TOPLEFT, HSN_HSEL_PN );

	}
}


// 付箋のアイコンを画面に表示する
//	int		ln		画面伝票上の　行番号(１～最大７)
//
void CDnpBaseClass::HusenDataSet( int ln )
{
	int sel;
	sel = _getDATA( get_dataline(ln), FD_HUSEN_PN )->DP_HUSEN;
	char* pcmnt;
	pcmnt = (char*)_getDATA(get_dataline(ln), FD_HUSEN_PN)->DP_HCMNT;

	GetNowDnpDialog()->SetHusen( ln, FD_HUSEN_PN, sel, pcmnt );
}


// 付箋入力ダイアログからのメッセージ処理
LRESULT CDnpBaseClass::OnHusenInpMsg( WPARAM w, LPARAM l )
{
	int		ln, dt_ln;
	int		dset = 0;

	WORD sgn, key;
	sgn = LOWORD( w );
	key = HIWORD( w );
	HSEN_DATA* pHdt;
	ln		= get_nowln();
	dt_ln	= get_dataline( ln );

	if( l ) {
		pHdt = (HSEN_DATA*)l;

		_getDATA( dt_ln, FD_HUSEN_PN )->DP_HUSEN = pHdt->hsen_sel;
		strcpy_s(	(char*)_getDATA( dt_ln, FD_HUSEN_PN )->DP_HCMNT, 
					sizeof _getDATA( dt_ln, FD_HUSEN_PN )->DP_HCMNT,
					pHdt->hsen_cmt
				);
	}

	if(	sgn == 0 ) {	// キーによる終了

		if( m_pHusenInp != NULL && key ) {

			HusenDataSet( ln );

			if( IsKind() != KIND_FURIKAE ) {
				if( key == VK_RIGHT )	key = VK_RETURN;
			}

			if( key == VK_RETURN || key == VK_TAB ) {
				// 付箋は最後の項目なので、次の行へ移動
				if( IsKind() == KIND_FURIKAE ) {
					if( FukugoLine(0) ) {
						set_position( ln, FD_DSYOG_PN );
					}
					else if( FukugoLine(1) ) {
						set_position( ln, FD_CSYOG_PN );
					}
					else {
						dset = 0;

						if( ln == getSCROLL_NL() ) {
							dset = DnpLine_Chg(1);
						}
						if( ! dset ) {
							if( set_position( (ln+1), FD_DBVAL_PN ) == -1 ) {
								set_position( 1, FD_DBVAL_PN );
							}
						}
						else {
							set_position( ln, FD_DBVAL_PN );
						}
					}
				}
				else if( IsKind() == KIND_NYUKIN ) {
					dset = 0;

					if( ln == getSCROLL_NL() ) {
						dset = DnpLine_Chg(1);
					}
					if( ! dset ) {
						if( set_position( (ln+1), FD_CRED_PN ) == -1 ) {
							set_position( 1, FD_CRED_PN );
						}
					}
					else {
						set_position( ln, FD_CRED_PN );
					}
				}
				else {
					dset = 0;

					if( ln == getSCROLL_NL() ) {
						dset = DnpLine_Chg(1);
					}
					if( ! dset ) {
						if( set_position( (ln+1), FD_DEBT_PN ) == -1 ) {
							set_position( 1, FD_DEBT_PN );
						}
					}
					else {
						set_position( ln, FD_DEBT_PN );
					}
				}
			}
			else if( key == VK_RIGHT ) {	//付箋の右は貸方に
				if( IsKind() == KIND_FURIKAE ) {
					set_position( ln, FD_CRED_PN );
				}
			}
			else if( key == VK_F2 || key == VK_LEFT ) {
			//	if( IsOKGyosyu( m_pZm->zvol->apno, SW_SOCIAL|SW_SYUKYO)/* == 0x52*/ )
				if( m_DI.IsHjiyuItemGyosyu() == 0 ) 
					set_position( ln, FD_TKY_PN );
				else {
					set_position( ln, FD_HJIYU_PN );
				}
			}
		}
	}
	else if( sgn == 1 ) {
		//表示を更新
		HusenDataSet( ln );
	}

	return 1;
}


// 取消し仕訳のカーソルポジションゲット
int CDnpBaseClass::get_delpn()
{
	int pn;

	if( IsKind() == KIND_FURIKAE ) {
		pn = FD_DBVAL_PN;
	}
	else if( IsKind() == KIND_NYUKIN ) {
		pn = FD_CRED_PN;
	}
	else {
		pn = FD_DEBT_PN;
	}

	return pn;
}



//	int ln ... 画面伝票上の　行番号(１～最大７)
BOOL CDnpBaseClass::IsTekiyoImage( int ln )
{
	int dt_ln = get_dataline( ln );

	if( IsJob() == JOB_SCAN ) {
		if( dt_ln <= 0 )	return	FALSE;
		if( dt_ln > get_MaxLine() )	return	FALSE;
		if( tblImage[dt_ln-1][0] )	return	TRUE;
	}
	return	FALSE;
}

//	int ln		画面伝票上の　行番号(１～最大７)
//	int sgn		貸方摘要イメージ？
void CDnpBaseClass::SetFocusToImage( int ln, int sgn )
{
	if( IsJob() == JOB_SCAN ) {
		GetNowDnpDialog()->SetFocusToImage( ln, sgn );
	}
}

//	int ln		画面伝票上の　行番号(１～最大７)
void CDnpBaseClass::KasiImageDisplay( int ln, BOOL sgn )
{
	int dt_ln = get_dataline( ln );

	if( sgn ) {
		IMREC	*imp;
		if( tblImageOfst[dt_ln-1][1] != -1 ) {
			if ( !m_pZm->image->SetAbsolutePosition( tblImageOfst[dt_ln-1][1] ) )
				imp	= m_pZm->image->DecodeImage();
		}
		else {
			imp	=	NULL;
		}
		GetNowDnpDialog()->SetImage( 1, ln, imp );
		GetNowDnpDialog()->KasiImageDisplay( ln, TRUE );
	}
	else {
		GetNowDnpDialog()->KasiImageDisplay( ln, FALSE );
	}
}

//	int ln		画面伝票上の　行番号(１～最大７)
void CDnpBaseClass::ImagePaintSw( int ln, int cnt, int sgn )
{
	int dt_ln	= get_dataline( ln );

	for( int i = 0; i < cnt && ln <= getSCROLL_NL(); i++, ln++ ) {
		if( tblImage[dt_ln-1][0] ) {
			GetNowDnpDialog()->ImagePaintSw( ln, sgn );
		}
	}
}

//	int ln		画面伝票上の　行番号(１～最大７)
void CDnpBaseClass::ImageBackColor( int ln, BOOL sgn )
{
	GetNowDnpDialog()->ImageBackColor( ln, sgn );
}

// 貸方摘要用の DBEdit を表示／非表示
void CDnpBaseClass::KasiTekiyoDisplay( BOOL sgn )
{
	GetNowDnpDialog()->KasiTekiyoDisplay( sgn );
}


//	int ln ... データ上の　行番号(1～100)
int	CDnpBaseClass::GetDelSgn(int ln ) 
{	
	if( ln > 0 ) {
		return tblDel[ln-1];
	}
	else {
		return 0;
	}
}

//	int ln ... データ上の　行番号(1～100)
void CDnpBaseClass::SetDelSgn(int ln, BOOL sw ) 
{ 
	if( ln > 0 ) {
		tblDel[ln-1] = sw; 
	}
}

// 仕訳取消サイン・貸方摘要などの行情報挿入
//	int ln ... データ上の　行番号(1～100)
void CDnpBaseClass::InsDelSgn( int ln ) 
{ 
	int lidx = ln - 1;

	if( lidx < 0 )	return;

	int old_delsgn = tblDel[lidx];

	int old_tblImage[2], old_tblImageOfst[2];
	CString old_tblTekiyo;
	CString old_tblTgnum[2];
	CString old_tblHcmnt[2];

	int old_tblTgdate[2], old_tblHjiyu[2], old_tblHusen[2];

	old_tblImage[0] = tblImage[lidx][0];
	old_tblImage[1] = tblImage[lidx][1];
	old_tblImageOfst[0] = tblImageOfst[lidx][0];
	old_tblImageOfst[1] = tblImageOfst[lidx][1];
	old_tblTekiyo = tblTekiyo[lidx];

	old_tblTgnum[0] = tblTgnum[lidx][0];
	old_tblTgnum[1] = tblTgnum[lidx][1];
	old_tblTgdate[0] = tblTgdate[lidx][0];
	old_tblTgdate[1] = tblTgdate[lidx][1];

	old_tblHjiyu[0] = tblHjiyu[lidx][0];
	old_tblHjiyu[1] = tblHjiyu[lidx][1];
	old_tblHusen[0] = tblHusen[lidx][0];
	old_tblHusen[1] = tblHusen[lidx][1];

	old_tblHcmnt[0] = tblHcmnt[lidx][0];
	old_tblHcmnt[1] = tblHcmnt[lidx][1];

	int i, max;
	max = get_MaxLine();

	for( i = (max - 1); i >= lidx; i-- )
	{
		if( i == lidx ) {	//挿入行
			tblDel[i] = 0;

			tblImage[i][0] = tblImage[i][1] = 0;
			tblImageOfst[i][0] = tblImageOfst[i][1] = 0;
			tblTekiyo[i].Empty();

			tblTgnum[i][0].Empty();	tblTgnum[i][1].Empty();
			tblTgdate[i][0] = tblTgdate[i][1] = 0;
			tblHjiyu[i][0] = tblHjiyu[i][1] = 0;
			tblHusen[i][0] = tblHusen[i][1] = 0;
			tblHcmnt[i][0].Empty();	tblHcmnt[i][1].Empty();
		}
		else if( i == (lidx + 1) ) {	//挿入行＋１の位置
			tblDel[i] = old_delsgn;

			tblImage[i][0] = old_tblImage[0];
			tblImage[i][1] = old_tblImage[1];
			tblImageOfst[i][0] = old_tblImageOfst[0];
			tblImageOfst[i][1] = old_tblImageOfst[1];
			tblTekiyo[i] = old_tblTekiyo;

			tblTgnum[i][0] = old_tblTgnum[0];
			tblTgnum[i][1] = old_tblTgnum[1];
			tblTgdate[i][0] = old_tblTgdate[0];
			tblTgdate[i][1] = old_tblTgdate[1];

			tblHjiyu[i][0] = old_tblHjiyu[0];
			tblHjiyu[i][1] = old_tblHjiyu[1];
			tblHusen[i][0] = old_tblHusen[0];
			tblHusen[i][1] = old_tblHusen[1];

			tblHcmnt[i][0] = old_tblHcmnt[0];
			tblHcmnt[i][1] = old_tblHcmnt[1];
		}
		else {
			int mv_idx = i - 1;
			int del_sgn = tblDel[mv_idx];
			tblDel[i] = del_sgn;

			tblImage[i][0] = tblImage[mv_idx][0];
			tblImage[i][1] = tblImage[mv_idx][1];
			tblImageOfst[i][0] = tblImageOfst[mv_idx][0];
			tblImageOfst[i][1] = tblImageOfst[mv_idx][1];
			tblTekiyo[i] = tblTekiyo[mv_idx];

			tblTgnum[i][0] = tblTgnum[mv_idx][0];
			tblTgnum[i][1] = tblTgnum[mv_idx][1];
			tblTgdate[i][0] = tblTgdate[mv_idx][0];
			tblTgdate[i][1] = tblTgdate[mv_idx][1];

			tblHjiyu[i][0] = tblHjiyu[mv_idx][0];
			tblHjiyu[i][1] = tblHjiyu[mv_idx][1];
			tblHusen[i][0] = tblHusen[mv_idx][0];
			tblHusen[i][1] = tblHusen[mv_idx][1];

			tblHcmnt[i][0] = tblHcmnt[mv_idx][0];
			tblHcmnt[i][1] = tblHcmnt[mv_idx][1];;
		}
	}
}

// 仕訳取消サイン・貸方摘要などの行情報削除
//	int ln ... データ上の　行番号(1～100)
void CDnpBaseClass::delLineDelSgn(int ln)
{
	int lidx = ln - 1;

	if( lidx < 0 )	return;

	int i, max;
	max = get_MaxLine();

	int src_idx;
	for( i = lidx; i < max; i++ ) {

		if( i == (max - 1) ) {//最終
			src_idx = i;
		}
		else {
			src_idx = i + 1;
		}

		tblDel[i] = tblDel[src_idx];
		tblImage[i][0] = tblImage[src_idx][0];
		tblImage[i][1] = tblImage[src_idx][1];
		tblImageOfst[i][0] = tblImageOfst[src_idx][0];
		tblImageOfst[i][1] = tblImageOfst[src_idx][1];
		tblTekiyo[i] = tblTekiyo[src_idx];

		tblTgnum[i][0] = tblTgnum[src_idx][0];
		tblTgnum[i][1] = tblTgnum[src_idx][1];
		tblTgdate[i][0] = tblTgdate[src_idx][0];
		tblTgdate[i][1] = tblTgdate[src_idx][1];

		tblHjiyu[i][0] = tblHjiyu[src_idx][0];
		tblHjiyu[i][1] = tblHjiyu[src_idx][1];
		tblHusen[i][0] = tblHusen[src_idx][0];
		tblHusen[i][1] = tblHusen[src_idx][1];

		tblHcmnt[i][0] = tblHcmnt[src_idx][0];
		tblHcmnt[i][1] = tblHcmnt[src_idx][1];;
	}
}


//ポジションによる表示切替
void CDnpBaseClass::disp_change()
{
BOOL sg;
int pn = get_nowpn();

	sg = TRUE;
	// 選択画面の表示色
	enable_fixval( sg );

	// 固定科目選択のディセーブル／イネーブル
	switch( pn ) {
	case DP_BMN_PN:
	case DP_KOJI_PN:
	case FD_DBMN_PN:
	case FD_DKOJI_PN:
	case FD_CBMN_PN:
	case FD_CKOJI_PN:
	case FD_TKY_PN:
	case FD_DSKBN_PN:
	case FD_CSKBN_PN:
	case FD_HJIYU_PN:
		sg = FALSE;
		break;
	default:
		sg = TRUE;
		break;
	}

	fix8disp( sg );
}


//ポジションによる表示切替（強制表示）
void CDnpBaseClass::_disp_change()
{
int pn = get_nowpn();
BOOL sg;

	switch( pn ) {
	case DP_BMN_PN:
	case DP_KOJI_PN:
	case FD_DBMN_PN:
	case FD_DKOJI_PN:
	case FD_CBMN_PN:
	case FD_CKOJI_PN:
	case FD_TKY_PN:
	case FD_DSKBN_PN:
	case FD_CSKBN_PN:
		sg = FALSE;
		break;
	default:
		sg = TRUE;
		break;
	}
	// 固定科目選択のディセーブル／イネーブル
	fix8disp( sg );
}



// フォーカスセット
void CDnpBaseClass::set_focus( int pn )
{
//FILETrace( "@CDnpBaseClass::set_focus pn = %d\n", pn );

	_set_focus( get_nowln(), pn );
}

//---------------------------------------------------
//	フォーカスセット
//		int ln	表示行(-1のときはヘッド部)
//		int pn	ポジションサイン
//
//---------------------------------------------------
void CDnpBaseClass::_set_focus( int ln, int pn )
{
//FILETrace( "@CDnpBaseClass::_set_focus ln = %d, pn = %d\n", ln, pn );

	// ヘッド部
	if( pn >= DP_DENP_PN && pn <= DP_KOJI_PN )
		ln = -1;

	m_pDnpParent->PostMessage( WM_RETROFOCUS, ln, pn );

	edaLine = ln;
	edaPn = pn;
}


LRESULT CDnpBaseClass::OnRetroFocus( WPARAM wParam, LPARAM lParam )
{
//FILETrace( "@CDnpBaseClass::OnRetroFocus %d,%d\n", wParam, lParam );

	GetNowDnpDialog()->set_focus( wParam, lParam );

	return 1;
}

// 指定IDのウィンドウにフォーカスセット
LRESULT CDnpBaseClass::OnRetroFocus2( WPARAM wParam, LPARAM lParam )
{
	CWnd* pwnd;
	pwnd = GetNowDnpDialog()->GetDlgItem( wParam );
	
	GetNowDnpDialog()->GotoDlgCtrl( pwnd );
	return 1;
}




// 諸口貸借表示
void CDnpBaseClass::syogdsp()
{
//	int ofs;
	int i, n, max;
	CDBINPDataRec *crec;
	char tl_buf[6];
	CString str;

	l_clear( SHEET_SYOG_TAI );	// 1シート分貸借クリア 

	if( IsJob() == JOB_SCAN ) {
		if( IsKind() == KIND_FURIKAE ) {
/*
			l_add( SHEET_SYOG_TAI, SHEET_SYOG_TAI, m_DPscan.GetKasiTotal() );
			l_sub( SHEET_SYOG_TAI, SHEET_SYOG_TAI, m_DPscan.GetKariTotal() );

			l_add( tl_buf, SYOG_TAI, SHEET_SYOG_TAI );
			syog_dsp( tl_buf );
*/
			syog_dsp( SYOG_TAI );

			// 現在行の消費税分リセット
		//	LineChgJob( m_CREC, get_nowln(), get_nowpn() );	// ln = 今回のライン
			return;
		}
	}

	// 諸口 仕訳に分解
	for( i = 0; i < EXPAN_MAX; i++ )	// テーブルイニシャライズ
		EXPAN_TBL[i].Reset();

	if( (max = daexpan( EXPAN_TBL )) )
	{
		// 諸口計算
		for( n = 0 ; n < max ; ++n )
		{
			crec = &EXPAN_TBL[n];

			// 外税は 税額を＋
			// 借方
			if( crec->m_dbt == SYOGT ) {
				l_add( SHEET_SYOG_TAI, SHEET_SYOG_TAI, crec->m_val );
				if( sotozei_data( crec ) ) {
					l_add( SHEET_SYOG_TAI, SHEET_SYOG_TAI, crec->m_zei );
				}
			}
			// 貸方
			if( crec->m_cre == SYOGT ) {
				l_sub( SHEET_SYOG_TAI, SHEET_SYOG_TAI, crec->m_val );
				if( sotozei_data( crec ) ) {
					l_sub( SHEET_SYOG_TAI, SHEET_SYOG_TAI, crec->m_zei );
				}
			}
		}
	}

	// 表示
	l_add( tl_buf, SYOG_TAI, SHEET_SYOG_TAI );
	syog_dsp( tl_buf );

	// 現在行の消費税分リセット
	LineChgJob( m_CREC, get_nowln(), get_nowpn() );

}

//諸口貸借表示
void CDnpBaseClass::syog_dsp( char *buf )
{
	if( IsKind() != KIND_FURIKAE )	return;

	GetNowDnpDialog()->SyogDisp( buf );
}

//諸口消去表示
void CDnpBaseClass::syog_clr()
{
	l_clear( SHEET_SYOG_TAI );	// 1シート分貸借クリア 
	syog_dsp( SYOG_TAI );	// 表示
}


///////////////////////////////////////////////////////////////////////////////
// 自動仕訳関係
//	int line ... 画面伝票上の　行番号(１～最大７)
BOOL CDnpBaseClass::IsAutoSiwake( int line )
{

	if( IsMode() != MODE_AUTO )	return	FALSE;

	int	ln, dt_ln;
	int	pn	=	get_nowpn();

	if( line < 1 )						ln	=	1;
	else if( line > getSCROLL_NL() )	ln	=	1;
	else								ln	=	line;

	dt_ln	=	get_dataline( ln );

	if( IsScanLine( dt_ln ) )
		return FALSE;

	switch( IsKind() ) {
		case	KIND_NYUKIN:
			if( _getCND( dt_ln, FD_CRED_PN )->INP_sg ) {
				return	FALSE;
			}
			break;
		case	KIND_SYUKKIN:
			if( _getCND( dt_ln, FD_DEBT_PN )->INP_sg ) {
				return	FALSE;
			}
			break;
		default:
			if( _getCND( dt_ln, FD_DEBT_PN )->INP_sg || _getCND( dt_ln, FD_CRED_PN )->INP_sg ) {
				return	FALSE;
			}
			break;
	}

	return	TRUE;
}



// 摘要欄引き延ばし＆戻し
//	int ln ... 画面伝票上の　行番号(１～最大７)
void CDnpBaseClass::tkyexpan( int ln, CRect* rc, BOOL exp_sw, BOOL bOnSize/*=FALSE*/ )	// ln = 該当ライン
{
	CDnpBaseDlg* dlg;
	VARIANT var;
	UINT id = IDC_ICSDBEDT8CTRL1;

	dlg = GetNowDnpDialog();

//FILETrace("tkyexpan ln %d, exp_sw = %d\n", ln, exp_sw );

	CWnd* pwnd = GetNowDnpDialog()->GetDlgItem( id );
ASSERT( pwnd != NULL );

	CRect rect, dlgrc, tgnrc;
	int wd, max;
	CString buf;

	if( exp_sw ) {	// 摘要欄拡大
		TkyDBedtEnable(TRUE);

		rect = *rc;
		// 手形期日矩形
		GetNowDnpDialog()->GetRect( ln, FD_TGNUM_PN, tgnrc );

		max = (m_pZm->zvol->tk_ln*2);
		LPSTR p = buf.GetBuffer( max+ 10 );
		for( int i = 0; i < max; i++ ) {
			p[i] = 'A';
		}
		p[max] = '\0';
		buf.ReleaseBuffer();

		if( IsKind() == KIND_FURIKAE ) {
			CFont font;
			COleFont ofont = ((CICSDBEDT*)pwnd)->GetFont();
			CY cy = ofont.GetSize();

			LOGFONT lf = {0};
			lf.lfHeight = (LONG)(cy.int64 / 1000);
			lf.lfCharSet = DEFAULT_CHARSET;
			strcpy_s( lf.lfFaceName, sizeof lf.lfFaceName, ofont.GetName() );

			font.CreatePointFontIndirect( &lf );

			CClientDC dc(m_pDnpParent);
			CSize sz;
			CFont* pOld;
			pOld = dc.SelectObject( &font );
			sz = dc.GetTextExtent( buf );

			dc.SelectObject( pOld );
			font.DeleteObject();

			dlg->GetWindowRect( dlgrc );

			wd = rc->Width();
			rect.left = rc->left;
			rect.right = rc->left + sz.cx + 5;

			if( rect.right < rc->right ) {
				rect.right = rc->right;
			}
			else {
				if( dlgrc.right < rect.right ) {
					rect.right = dlgrc.right;
					rect.left = rect.right - sz.cx - 5;
				}
//				rect.bottom = rc->bottom + rc->Height();
//				rect.top = rc->bottom;;
			}
		//	rect.top = rc->top;
		//	rect.bottom = rc->bottom;

			rect.top = tgnrc.bottom;
			rect.bottom = rect.top + rc->Height();
		}
		else {
			wd = rc->Width();
			rect.right = rect.left + (wd*2);

			rect.top = tgnrc.bottom;
			rect.bottom = rect.top + rc->Height();
		}

		dlg->ScreenToClient( rect );
		pwnd->SetWindowPos(NULL, 0,0,0,0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER );
		pwnd->MoveWindow( rect );
		pwnd->UpdateWindow();
		// データをセット
		int dt_ln = get_dataline( ln );

		var.pbVal = _getDATA( dt_ln, FD_TKY_PN )->DP_TKY;
		DBdata_set( GetNowDnpDialog(), id, &var, ICSDBEDT_TYPE_STRING, 0 );

		m_pDnpParent->PostMessage( WM_RETROFOCUS2, id );

		int idx = dt_ln-1;

		//貸方の摘要表示	09.03 /08
		if( !tblTekiyo[idx].IsEmpty() ) {
			CWnd*	pBase	=	GetNowDnpDialog();
			CWnd*	pTeki	=	pBase->GetDlgItem( IDC_ICSDBEDT_KASITKY );

			CRect	rect, tmp;
			
			pwnd->GetWindowRect( rect );
			((CDnpBaseDlg*)pBase)->GetRect( ln, FD_TKY_PN, tmp );

			rect.top	=	rect.bottom;
			rect.bottom	=	rect.top + tmp.Height();
			pBase->ScreenToClient( &rect );
			pTeki->SetWindowPos( &CWnd::wndTopMost, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW | SWP_NOZORDER );

			KasiTekiyoDisplay( TRUE );
			VARIANT var;
			var.pbVal = (BYTE*)(LPCTSTR)tblTekiyo[idx];

			DBdata_set( pBase, IDC_ICSDBEDT_KASITKY, &var, ICSDBEDT_TYPE_STRING, 0 );
		}
	}
	else {
		if( bOnSize ) {
			pwnd->MoveWindow( 0,0,0,0 );
			KasiTekiyoDisplay( FALSE );

			CICSDBEDT* pDedt;
			pDedt = (CICSDBEDT*)(GetNowDnpDialog()->GetDlgItem(id));
			pDedt->SetCaretPosition( 0 );	//キャレットの位置を最初にする
											//最大化した時、文字が隠れる(入金・出金)
		}
		else {
			var.pbVal = NULL;
			DBdata_set( GetNowDnpDialog(), id, &var, ICSDBEDT_TYPE_STRING, 0 );

			pwnd->MoveWindow( 0,0,0,0 );

			DBdata_set( GetNowDnpDialog(), IDC_ICSDBEDT_KASITKY, &var, ICSDBEDT_TYPE_STRING, 0 );
			KasiTekiyoDisplay( FALSE );
		}
	}
}

//---------------------------------------------------------------------
// 工事番号入力エリア 引き延ばし＆戻し
//	int ln ... 画面伝票上の　行番号(１～最大７)
//
//---------------------------------------------------------------------
void CDnpBaseClass::kjedit_expan( int ln, int pn, CRect* rc, BOOL exp_sw, BOOL bOnSize/*=FALSE*/ )	// ln = 該当ライン
{
	CDnpBaseDlg* dlg;
	VARIANT var;
	UINT id = IDC_ICSDBEDT_KOJI;

	dlg = GetNowDnpDialog();

//FILETrace("kjedit_expan ln %d, exp_sw = %d\n", ln, exp_sw );

	CWnd* pwnd = GetNowDnpDialog()->GetDlgItem( id );
ASSERT( pwnd != NULL );

	CRect rect, dlgrc, tgnrc;
	CString buf;
	char	tmp[128];

	if( exp_sw ) {	// 工事番号欄拡大
		rect = *rc;

		dlg->ScreenToClient( rect );
		pwnd->SetWindowPos(NULL, 0,0,0,0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER );
		pwnd->MoveWindow( rect );
		pwnd->UpdateWindow();

		// データをセット
		int dt_ln = get_dataline( ln );

	//	m_pZm->KojiCodeToStr( tmp, sizeof tmp, CString(_getDATA( dt_ln, pn )->DP_KOJI) );
		set_codestring( tmp, sizeof tmp, (char*)_getDATA( dt_ln, pn )->DP_KOJI, m_pZm->zvol->kjcol );
		var.pbVal = (BYTE*)tmp;
		DBdata_set( dlg, id, &var, ICSDBEDT_TYPE_STRING, 0 );
		m_pDnpParent->PostMessage( WM_RETROFOCUS2, id );
	}
	else {
		if( ! bOnSize ) {
			var.pbVal = NULL;
			DBdata_set( dlg, id, &var, ICSDBEDT_TYPE_STRING, 0 );
		}

		pwnd->MoveWindow( 0,0,0,0 );
	}
}


//---------------------------------------------------------------------
// 手形番号 入力エリア 引き延ばし＆戻し
//	int ln ... 画面伝票上の　行番号(１～最大７)
//---------------------------------------------------------------------
void CDnpBaseClass::tgnum_expan( int ln, CRect* rc, BOOL exp_sw, BOOL bOnSize/*=FALSE*/ )	// ln = 該当ライン
{
	CDnpBaseDlg* dlg;
	VARIANT var;
	UINT	id = IDC_ICSDBEDT_TGNUM;
	int		pn = FD_TGNUM_PN;

	dlg = GetNowDnpDialog();

//FILETrace("tgnum_expan ln %d, exp_sw = %d\n", ln, exp_sw );

	CWnd* pwnd = GetNowDnpDialog()->GetDlgItem( id );
ASSERT( pwnd != NULL );

	CRect rect, dlgrc, tgnrc;
	CString buf;

	if( exp_sw ) {	// 手形番号欄拡大
		rect = *rc;

		CSize sz = DBEditTextSize(id, dlg, 20);
		rect.right = rect.left + sz.cx + (sz.cx / 24);

		dlg->ScreenToClient( rect );
		pwnd->SetWindowPos(NULL, 0,0,0,0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER );
		pwnd->MoveWindow( rect );
		pwnd->UpdateWindow();

		// データをセット
		int dt_ln = get_dataline( ln );

		var.pbVal = (BYTE*)_getDATA( dt_ln, pn )->DP_SNUM;
TRACE( "tgnum_expan data[%d]= %s\n", dt_ln, var.pbVal );

		DBdata_set( dlg, id, &var, ICSDBEDT_TYPE_STRING, 0 );
		m_pDnpParent->PostMessage( WM_RETROFOCUS2, id );
	}
	else {
		if( ! bOnSize ) {
			var.pbVal = NULL;
			DBdata_set( dlg, id, &var, ICSDBEDT_TYPE_STRING, 0 );
		}

		pwnd->MoveWindow( 0,0,0,0 );
	}
}


//---------------------------------------------------------------------
// 部門番号入力エリア 引き延ばし＆戻し
//	int ln ... 画面伝票上の　行番号(１～最大７)
//
//---------------------------------------------------------------------
void CDnpBaseClass::bmnedit_expan( int ln, int pn, CRect* rc, BOOL exp_sw, BOOL bOnSize/*=FALSE*/ )	// ln = 該当ライン
{
	CDnpBaseDlg* dlg;
	VARIANT var;
	UINT id = IDC_ICSDBEDT_BMON;

	dlg = GetNowDnpDialog();

//FILETrace("bmnedit_expan ln %d, exp_sw = %d\n", ln, exp_sw );

	CWnd* pwnd = GetNowDnpDialog()->GetDlgItem( id );
ASSERT( pwnd != NULL );

	CRect rect, dlgrc, tgnrc;
	CString buf;
	char	tmp[128];

	if( exp_sw ) {	// 部門番号欄拡大
		rect = *rc;

		dlg->ScreenToClient( rect );
		pwnd->SetWindowPos(NULL, 0,0,0,0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER );
		pwnd->MoveWindow( rect );
		pwnd->UpdateWindow();

		// データをセット
		int dt_ln = get_dataline( ln );

	//	m_pZm->BumonCodeToStr( tmp, sizeof tmp, _getDATA( dt_ln, pn )->DP_BMN );
		set_codestr( tmp, sizeof tmp, _getDATA( dt_ln, pn )->DP_BMN, m_pZm->zvol->bmcol );
		var.pbVal = (BYTE*)tmp;
		DBdata_set( dlg, id, &var, ICSDBEDT_TYPE_STRING, 0 );
		m_pDnpParent->PostMessage( WM_RETROFOCUS2, id );
	}
	else {
		if( ! bOnSize ) {
			var.pbVal = NULL;
			DBdata_set( dlg, id, &var, ICSDBEDT_TYPE_STRING, 0 );
		}

		pwnd->MoveWindow( 0,0,0,0 );
	}
}


// 伝票修正の データ行か？
//	int ln ... データ上の　行番号(1～100)
BOOL CDnpBaseClass::IsScanLine( int dt_ln )
{
	if( IsJob() == JOB_INPUT )	return	FALSE;

//	int dt_ln = get_dataline( ln );

	if( m_DPscan.IsOriginData( dt_ln, 0 ) && m_DPscan.IsOriginData( dt_ln, 1 ) ) {
		return	TRUE;
	}
	else if( m_DPscan.IsOriginData( dt_ln, 0 ) ) {
		if( IsKind() == KIND_FURIKAE ) {
			if( _getCND( dt_ln, FD_CRED_PN )->INP_sg || _getCND( dt_ln, FD_CRVAL_PN )->INP_sg )
				return	FALSE;
		}
		return	TRUE;
	}
	else if( m_DPscan.IsOriginData( dt_ln, 1 ) ) {
		if( IsKind() == KIND_FURIKAE ) {
			if( _getCND( dt_ln, FD_DEBT_PN )->INP_sg || _getCND( dt_ln, FD_DBVAL_PN )->INP_sg )
				return	FALSE;
		}
		return	TRUE;
	}
	else	return	FALSE;
}



//-----------------------------------------------
//	単一仕訳・複合仕訳の切替
//
//-----------------------------------------------
void CDnpBaseClass::ChangeType( int type, BOOL bLnChg /*= FALSE*/ )
{
	if( IsKind() == KIND_FURIKAE ) {
		IsType()	=	type;
		int sub;
		char* pFuku, *pTan;

		if( IsJob() == JOB_SCAN && bLnChg ) {
			pFuku	= "複合仕訳";
			pTan	= "単一仕訳";
		}
		else {
			pFuku	= "複合入力";
			pTan	= "単一入力";
		}

		if( IsType() == TYPE_FUKUGOU ) {
			sub = (IsJob() == JOB_SCAN) ? 11 : 1;
			m_pFuri->SiwakeTypeDisp( pFuku );
		}
		else {
			sub = (IsJob() == JOB_SCAN) ? 10 : 0;
			m_pFuri->SiwakeTypeDisp( pTan );
		}
//		((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar( 4, sub );
	}
}


void CDnpBaseClass::ScanAllTotalClc()
{
	int ln, pn;
	struct _DenpData *pdata;
	struct _DNPLINE_CND *psg;
	struct _DenpData *pzdata;
	struct _DNPLINE_CND *pzsg;

	l_defn( 0x16 );

	if( IsKind() == KIND_FURIKAE ) {
		unsigned char	kari[6], kasi[6];
		unsigned char	tkari[6], tkasi[6];
		unsigned char	syog[6];
		memset( kari, 0, sizeof kari );
		memset( kasi, 0, sizeof kasi );
		memset( syog, 0, sizeof syog );
		ScanTotalClc( kari, kasi );
//L_PRINT( "ScanTotalClc kari=", kari );
//L_PRINT( "/ ScanTotalClc kasi=", kasi );

		l_add( syog, syog, kasiTotal );
		l_sub( syog, syog, kariTotal );
		l_sub( SYOG_TAI, SYOG_TAI, syog );

//L_PRINT( "/ SYOG_TAI=", SYOG_TAI );

		memmove( tkari, m_DPscan.GetKariTotal(), sizeof tkari );
		memmove( tkasi, m_DPscan.GetKasiTotal(), sizeof tkasi );
//		memmove( tkari, m_DPscan.GetKariTotal(), sizeof tkari );
//		memmove( tkasi, m_DPscan.GetKasiTotal(), sizeof tkasi );

//TRACE( "\n" );
//L_PRINT( "tkari=", tkari );
//L_PRINT( "/ tkasi=", tkasi );
//TRACE( "\n" );
//L_PRINT( "kariTotal=", kariTotal );
//L_PRINT( "/ kasiTotal=", kasiTotal );

		l_sub( tkari, tkari, kariTotal );
		l_add( tkari, tkari, kari );
		l_sub( tkasi, tkasi, kasiTotal );
		l_add( tkasi, tkasi, kasi );

//TRACE( "\n" );
//L_PRINT( "/ tkari=", tkari );
//L_PRINT( "/ tkasi=", tkasi );

		m_DPscan.SetSheetTotal( tkari, tkasi );

		memmove( DEBT_TOTAL.DP_VAL, m_DPscan.GetKariTotal(), sizeof DEBT_TOTAL.DP_VAL );
		memmove( CRED_TOTAL.DP_VAL, m_DPscan.GetKasiTotal(), sizeof CRED_TOTAL.DP_VAL );

//TRACE( "\n" );
//L_PRINT( "/ DEBT_TOTAL.DP_VAL=", DEBT_TOTAL.DP_VAL );
//L_PRINT( "/ CRED_TOTAL.DP_VAL=", CRED_TOTAL.DP_VAL );
//TRACE( "\n--------------------------------------------\n" );

		if( l_test( DEBT_TOTAL.DP_VAL ) )	DEBT_TOTAL_CND.INP_sg	=	TRUE;
		if( l_test( CRED_TOTAL.DP_VAL ) )	CRED_TOTAL_CND.INP_sg	=	TRUE;

		memmove( kariTotal, kari, sizeof kariTotal );
		memmove( kasiTotal, kasi, sizeof kasiTotal );

//TRACE( "\n" );
//L_PRINT( "kariTotal2", kariTotal );
//L_PRINT( "/ kasiTotal2", kasiTotal );
//TRACE( "\n--------------------------------------------\n" );

		l_add( SYOG_TAI, SYOG_TAI, kasiTotal );
		l_sub( SYOG_TAI, SYOG_TAI, kariTotal );
	}
	else {
		int syzpn, zeipn;
		if( IsKind() == KIND_NYUKIN ) {
			pn		=	FD_CRVAL_PN;
			pdata	=	&CRED_TOTAL;
			psg		=	&CRED_TOTAL_CND;
			pzdata	=	&ZCRED_TOTAL;
			pzsg	=	&ZCRED_TOTAL_CND;
			syzpn	=	FD_CSKBN_PN;
			zeipn	=	FD_CRZEI_PN;
		}
		else if( IsKind() == KIND_SYUKKIN ) {
			pn		=	FD_DBVAL_PN;
			pdata	=	&DEBT_TOTAL;
			psg		=	&DEBT_TOTAL_CND;
			pzdata	=	&ZDEBT_TOTAL;
			pzsg	=	&ZDEBT_TOTAL_CND;
			syzpn	=	FD_DSKBN_PN;
			zeipn	=	FD_DBZEI_PN;
		}
		memmove( pdata, pzdata, sizeof( struct _DenpData ));
		memmove( psg, pzsg, sizeof( struct _DNPLINE_CND ));
		for( ln = 1 ; ln <= get_MaxLine(); ++ln ) {
			if( !m_DPscan.DeleteLine( ln ) ) {
				if( _getCND( ln, pn )->INP_sg ) {
					l_add( pdata->DP_VAL, pdata->DP_VAL, _getDATA( ln, pn )->DP_VAL);
					psg->INP_sg	=	TRUE;

					// 外税を合計にプラス
					if( _getCND(ln, syzpn)->INP_sg ) {
						if( _sotozei_data(_getDATA(ln, syzpn)->SYDATA._DSGN) ) {
							l_add(pdata->DP_VAL, pdata->DP_VAL, _getDATA(ln, zeipn)->DP_VAL);
						}
					}
				}
			}
		}
	}
}


//伝票毎の貸借合計の計算
void CDnpBaseClass::ScanTotalClc( unsigned char* kari, unsigned char* kasi )
{
	if( IsKind() != KIND_FURIKAE )	return;

	int	ln, pn, n, syzpn, zeipn;
	unsigned char*	pVal;

	l_defn( 0x16 );

	for( n = 0 ; n < 2 ; ++n ) {
		if( !n ) {
			pn		=	FD_DBVAL_PN;
			zeipn	=	FD_DBZEI_PN;
			syzpn	=	FD_DSKBN_PN;
			pVal	=	kari;
		}
		else {
			pn		=	FD_CRVAL_PN;
			zeipn	=	FD_CRZEI_PN;
			syzpn	=	FD_CSKBN_PN;
			pVal	=	kasi;
		}
		for( ln = 1 ; ln <= get_MaxLine(); ++ln ) {
			if( !m_DPscan.DeleteLine( ln ) ) {

				if( _getCND( ln, pn )->INP_sg ) {
					l_add( pVal, pVal, _getDATA( ln, pn )->DP_VAL );

					// 外税は、貸借計算に プラス
					if( _getCND( ln, syzpn )->INP_sg ) {
						if( _sotozei_data( _getDATA( ln, syzpn )->SYDATA._DSGN ) ) {
							l_add( pVal, pVal, _getDATA( ln, zeipn )->DP_VAL );
						}
					}
				}
			}
		}
	}
}



//
// 転送仕訳の 背景色取得
//	char sgn	EDT_sg を参照
//
COLORREF	CDnpBaseClass::GetConfirmBackColor( char sgn )
{
	COLORREF col;

	switch( sgn ) 
	{
	case 1:
		col = RGB_TENSOMDFY_DATA;
		break;
	case 2:
	case 10:
		col = RGB_KAKUT_DATA;
		break;
	case 3:
		col = RGB_TENSO_DATA;
		break;
	default:
		col = RGB_WHITE;
		break;
	}
	return col;
}


//
// 指定行の 背景色取得
//	int dt_ln	データ行
//
COLORREF	CDnpBaseClass::GetLineBackColor( int dt_ln )
{
	return GetConfirmBackColor( _getCND( dt_ln, FD_TKY_PN )->EDT_sg );
}


int CDnpBaseClass::get_MaxLineEX()
{
	int max = DP_SCROLL_NL;

	if( IsJob() == JOB_SCAN && IsKind() == KIND_FURIKAE ) {
		max = m_DPscan.GetLastDataLine();
	}

	return max;
}


// 行変更
//	vect ..... 行変更方向 と 変更量
//	nofocus .. 1 = フォーカスセットしない。2 = 最終データ取得しない
//
int CDnpBaseClass::DnpLine_Chg( int vect, int nofocus/*=0*/ )
{

	CDnpBaseDlg* pDlg = GetNowDnpDialog();

//TRACE( "DnpLine_Chg TOP vect = %d, nofocus = %d\n", vect, nofocus );

	int topidx = -1;
	int ret = 0;
	int cnt = getSCROLL_NL();
	int	max = get_MaxLineEX();

	if( vect < 0 ) {
		if( m_TopIdx > 0 ) {
			topidx = m_TopIdx + vect;
			if( topidx < 0 )	topidx = 0;
		}
	}
	else {
		if( max > (m_TopIdx + cnt) ) {
			topidx = m_TopIdx + vect;
			if( (topidx + cnt) > max )
				topidx = max - cnt;
		}
	}

	int ln, pn;
	ln = get_nowln();
	pn = get_nowpn();

	if( topidx != -1 ) {
		m_bDLineMMinpIgnore = TRUE;
		m_bDLineChanging	= TRUE;
		//フォーカスオフ
		if( nofocus != 2 )	lastinput_get();

#ifdef _DEBUG_CLOSE
BYTE a[16];
memcpy( a, _getDATA( 6, FD_CSKBN_PN )->_udta._SYDATA._DSGN, 16 );
TRACE( "DnpLine_Chg move = %d,%d --> [%d,%d]\n", ln, pn, get_nowln(), get_nowpn() );
TRACE( "DnpLine_Chg dsign = %02x%02x%02x%02x%02x\n", a[0], a[1], a[2], a[3], a[4] );
#endif

		pDlg->dataline_disp( (topidx+1) );
		m_TopIdx = topidx;
		ret = 1;

		pDlg->set_scrollbar( NULL, m_TopIdx, max, cnt );

		if( ! nofocus ) {
			_set_focus(ln, pn);
		}

		// データ移動したため、消費税区分新データ作成
		LineChgJob( m_CREC, ln, pn );

		m_bDLineChanging	= FALSE;
	}

//TRACE( "DnpLine_Chg END\n" );

	return ret;
}


// 垂直スクロールバーからの データスクロール
int CDnpBaseClass::Vs_Linemove( int direction )
{
TRACE( "@Vs_Linemove m_TopIdx = %d\n", m_TopIdx );

	return DnpLine_Chg( direction );
}



// 最終入力を取得
void CDnpBaseClass::lastinput_get()
{
	// フォーカスオフの処理を呼び出す
	GetNowDnpDialog()->DiagEditOFF();

	m_bDispLno = FALSE;

	GetNowDnpDialog()->SendMessage( WM_RETROFOCUS, MAKELONG((WORD)-1,0), DP_MM_PN );

#ifdef LATER_CLOSE
	// 振替 -> 入金切替時に、IsType()が入金に切り替わった後
	// 振替の ターミネーション 0 が、送られてくるので、それを回避するために、日付にフォーカスセット
	m_selDate.SetFocus();
#endif
}



//----------------------------------------------------
//	仕訳に分解
//	CDBINPDataRec *dtbl	分解された仕訳レコード格納
//
//
//----------------------------------------------------
int CDnpBaseClass::daexpan( CDBINPDataRec *dtbl )
{
int n;
int ln;

	// 諸口＆消費税自動分離仕訳に分解
	n = 0;
	for( ln = 1; ln <= get_MaxLine(); ++ln )

	{
		n += da_make( ln, dtbl + n );
	}

	return n;
}



//	F7摘要登録ダイアログを出した後、他の画面に切り替えて、
//	また伝票形式に戻ると、摘要DBEditの KillFocus がきて
//	入力した摘要の1行目を消していたため、切り替え時には、Disableにする
//
void CDnpBaseClass::TkyDBedtEnable(BOOL bEnable)
{
	UINT id = IDC_ICSDBEDT8CTRL1;
	CDnpBaseDlg* dlg = GetNowDnpDialog();
	CWnd* pwnd = GetNowDnpDialog()->GetDlgItem( id );
	pwnd->EnableWindow( bEnable );
}



//
//	伝票修正で諸口を詰めないときモードのときの諸口側のポジションか？
//
int CDnpBaseClass::FuriSyogPosition( int ln, int pn )
{
	int	ret;

	if( IsJob()	!= JOB_SCAN || IsKind() != KIND_FURIKAE ) {
		return 0;
	}

	if( ! SyogSiwakeMode() )
		return 0;

	ret = m_DPscan.IsFuriSyogPostion( ln, pn );

	return ret;
}


//------------------------------------------------------
//	伝票ポジションデータ取得
//	int ln ... 画面伝票上の　行番号(１～最大７)
//------------------------------------------------------
DNP_POS* CDnpBaseClass::GetDNP_POS( int ln, int pn )
{
	return GetNowDnpDialog()->GetDNP_POS( ln, pn );
}


//--------------------------------------------------------
//	諸口枝番 入力＆オフフォーカス
//	long nChar		キーコード
//	CString& edit	諸口枝番コード
//--------------------------------------------------------
BOOL CDnpBaseClass::CheckSyog( long nChar, CString& edit )
{
	if( edit.IsEmpty() )
		return FALSE;

	int n = atoi( (LPCTSTR)edit );
	if( n <= 0 )	return FALSE;

	return TRUE;
}

//--------------------------------------------------------
//	諸口枝番 入力＆オフフォーカス
//	int ln		画面伝票上の　行番号(１～最大７)
//	long nChar	キーコード
//	int* bmn	諸口枝番コード
//--------------------------------------------------------
long CDnpBaseClass::SyogInp( int ln, int pn, long nChar, int *syogbrn )
{
	int set_syog;

	int dt_ln = get_dataline( ln );

	if( nChar == VK_RETURN || nChar == VK_TAB || 
		nChar == VK_UP || nChar == VK_DOWN ||
		! nChar ) {

		if( *syogbrn == -1 ) {
			set_syog = _getDATA( dt_ln, pn )->DP_cBRN;
		}
		else {
			set_syog = *syogbrn;
			_getDATA( dt_ln, pn )->DP_cBRN = set_syog;
		}

		GetNowDnpDialog()->SetSyogBrn( ln, pn, set_syog );

		if( nChar || (!nChar && set_syog != -1) )
			_getCND( dt_ln, pn )->INP_sg = TRUE;
	}

	if( nChar == VK_DELETE ) {
		getDATA( pn )->DP_cBRN = -1;
		GetNowDnpDialog()->SetSyogBrn( ln, pn, -1 );
		_getCND( dt_ln, pn )->INP_sg = FALSE;
	}

	return nChar;
}



BOOL CDnpBaseClass::IsFuriSyogBrn()
{
	//宗教法人以外は諸口.0001 がある
	if( m_pZm->zvol->apno >= 0x50 && m_pZm->zvol->apno == 0x59)
		return FALSE;

	return (SknBrnSign() == SKNBRN_INPUT);
}

BOOL CDnpBaseClass::IsZeigkMdfy()
{
	BOOL bRet = FALSE;
	if( m_pAsel->ZEIMDFY_OPT != OPT_ZEIMDFY )
		bRet = FALSE;
	else
		bRet = TRUE;

	return bRet;
};


//--------------------------------------------------------
//	現在行が複合仕訳 かどうか？
//	 int dcsw	借貸サイン
//
//--------------------------------------------------------
BOOL CDnpBaseClass::FukugoLine( int dcsw )
{
	BOOL ret = FALSE;
	int ln, dt_ln;
	ln		= get_nowln();
	dt_ln	= get_dataline( ln );

	if( IsKind() != KIND_FURIKAE )
		return FALSE;

	if( ! IsFuriSyogBrn() ) {
		return FALSE;
	}

	CString dbt, cre, tmp;
	char db_val[6] = {0}, cr_val[6] = {0};
	int d_ln, c_ln;
	int pn = dcsw == 0 ? FD_DEBT_PN : FD_CRED_PN;
	
	if( IsJob() == JOB_INPUT || (IsJob() == JOB_SCAN && !m_DPscan.IsOriginData(dt_ln, pn)) ) {

		memcpy( db_val, _getDATA( dt_ln, FD_DBVAL_PN )->DP_VAL, 6 );
		memcpy( cr_val, _getDATA( dt_ln, FD_CRVAL_PN )->DP_VAL, 6 );

		//借方--------
		if( ! dcsw ) {
			// 借方科目 入力有り
			if( _getCND( dt_ln, FD_DEBT_PN )->INP_sg ) {
				if( l_test( db_val ) != 0 ) {
					if( l_test( cr_val ) == 0 ) {
						ret = TRUE;
					}
					else {
						//借方・貸方で金額が同じ
						if( l_cmp( db_val, cr_val ) == 0 ) {
							dbt = _getDATA( dt_ln, FD_DEBT_PN )->DP_CdNum;
						//	cre = _getDATA( dt_ln, FD_CRED_PN )->DP_CdNum;
							//貸方は 未入力でも 支配科目を参照する
							get_linekamokucodeEX( ln, tmp, cre, d_ln, c_ln );
							if( m_DI.km_syattr( dbt ) && m_DI.km_syattr( cre ) ) {
								//消費税が異なる場合は、複合
								if( memcmp( _getDATA( d_ln, FD_DSKBN_PN )->SYDATA._DSGN,
											_getDATA( c_ln, FD_CSKBN_PN )->SYDATA._DSGN,
											SZ_DSGN 
										   ) != 0 ) {
									ret = TRUE;
								}
							}
						}
						else {
							ret = TRUE;
						}
					}
				}
			}
			else {
				// 借方科目 入力なし
				if( l_test( db_val ) != 0 ) {
					get_linekamokucodeEX( ln, dbt, cre, d_ln, c_ln );
					if( ! dbt.IsEmpty() ) {

						if( l_test( cr_val ) == 0 ) {
							ret = TRUE;
						}
						else {
							//借方・貸方で金額が同じ
							if( l_cmp( db_val, cr_val ) == 0 ) {

								if( m_DI.km_syattr( dbt ) && m_DI.km_syattr( cre ) ) {
									//消費税が異なる場合は、複合
									if( memcmp( _getDATA( d_ln, FD_DSKBN_PN )->SYDATA._DSGN,
												_getDATA( c_ln, FD_CSKBN_PN )->SYDATA._DSGN,
												SZ_DSGN 
											   ) != 0 ) {
										ret = TRUE;
									}
								}
							}
							else {
								ret = TRUE;
							}
						}
					}
				}
			}
		}
		else {	//貸方--------

			// 貸方科目 入力有り
			if( _getCND( dt_ln, FD_CRED_PN )->INP_sg ) {
				if( l_test( cr_val ) != 0 ) {
					if( l_test( db_val ) == 0 ) {
						ret = TRUE;
					}
					else {
						//借方・貸方で金額が同じ
						if( l_cmp( db_val, cr_val ) == 0 ) {
							cre = _getDATA( dt_ln, FD_CRED_PN )->DP_CdNum;
							//借方は 未入力でも 支配科目を参照する
						//	dbt = _getDATA( dt_ln, FD_DEBT_PN )->DP_CdNum;
							get_linekamokucodeEX( ln, dbt, tmp, d_ln, c_ln );
							if( m_DI.km_syattr( dbt ) && m_DI.km_syattr( cre ) ) {
								//消費税が異なる場合は、複合
								if( memcmp( _getDATA( d_ln, FD_DSKBN_PN )->SYDATA._DSGN,
											_getDATA( c_ln, FD_CSKBN_PN )->SYDATA._DSGN,
											SZ_DSGN 
										   ) != 0 ) {
									ret = TRUE;
								}
							}
						}
						else {
							ret = TRUE;
						}
					}
				}
			}
			else {
				// 貸方科目 入力なし
				if( l_test( cr_val ) != 0 ) {
					get_linekamokucodeEX( ln, dbt, cre, d_ln, c_ln );
					if( ! cre.IsEmpty() ) {

						if( l_test( db_val ) == 0 ) {
							ret = TRUE;
						}
						else {
							//借方・貸方で金額が同じ
							if( l_cmp( db_val, cr_val ) == 0 ) {

								if( m_DI.km_syattr( dbt ) && m_DI.km_syattr( cre ) ) {
									//消費税が異なる場合は、複合
									if( memcmp( _getDATA( d_ln, FD_DSKBN_PN )->SYDATA._DSGN,
												_getDATA( c_ln, FD_CSKBN_PN )->SYDATA._DSGN,
												SZ_DSGN 
											   ) != 0 ) {
										ret = TRUE;
									}
								}
							}
							else {
								ret = TRUE;
							}
						}
					}
				}
			}
		}
	}
	else {
		//伝票修正
		ret = m_DPscan.IsFukugoSiwake( dt_ln );

		if (!SyogSiwakeMode()) {
			if (ret) {
				if (!dcsw) {
					ret = m_DPscan.IsOriginData(dt_ln, 0);
				}
				else {
					ret = m_DPscan.IsOriginData(dt_ln, 1);
				}
			}
		}
		else {
			ret = m_DPscan.IsFuriNotSyogPostion(dt_ln, dcsw);
		}
	}

	return ret;
}


//---------------------------------------------
//	消費税ダイアログよりキーターミネーション
//		long nChar	キーコード
//---------------------------------------------
long CDnpBaseClass::TerminationSyz( long nChar )
{
	int ln, pn, vect, index;
	pn = get_nowpn();
	ln = get_nowln();

	// 輸入用 チェックインデックス
	index = (pn == FD_DSKBN_PN) ? 0 : 1;

	int valpn, zeipn;
	valpn = (pn == FD_DSKBN_PN) ? FD_DBVAL_PN : FD_CRVAL_PN;
	zeipn = (pn == FD_DSKBN_PN) ? FD_DBZEI_PN : FD_CRZEI_PN;

	if( nChar == VK_UP || nChar == VK_DOWN )
	{
		int dset = 0;

		if( nChar == VK_UP && ln == 1 ) {
			dset = DnpLine_Chg(-1);
		}
		else if( nChar == VK_DOWN && ln == getSCROLL_NL() ) {
			dset = DnpLine_Chg(1);
		}

		if( !dset ) {
			// 行移動
			if( nChar == VK_UP ) {
				--ln;
				vect	=	-1;
			}
			else {
				++ln;
				vect	=	1;
			}
			//行移動エラー（アンダー／オーバー　フロー）
			if( set_position( ln, pn, vect ) == -1 ) {
				if( nChar == VK_UP )	set_focus( DP_MM_PN );
				else {
					if( IsKind() == KIND_NYUKIN )		pn	=	FD_CRED_PN;
					else if( IsKind() == KIND_SYUKKIN )	pn	=	FD_DEBT_PN;
					else								pn	=	FD_DBVAL_PN;
					set_position( 1, pn );
				}
			}
		}
		return 0;
	}

	// カーソルセット
	switch( nChar )
	{
	case VK_RIGHT:
		if( IsKind() == KIND_NYUKIN ) {
			int dset = 0;
			if( ln == getSCROLL_NL() ) {
				dset = DnpLine_Chg(1);
			}
			if( ! dset ) {
				if( set_position( ++ln, FD_CRED_PN ) == -1 ) {
					set_position( 1, FD_CRED_PN );
				}
			}
			else {
				set_position( ln, FD_CRED_PN );
			}
		}
		else if( IsKind() == KIND_SYUKKIN ) {
			int dset = 0;
			if( ln == getSCROLL_NL() ) {
				dset = DnpLine_Chg(1);
			}
			if( ! dset ) {
				if( set_position( ++ln, FD_DEBT_PN ) == -1 )
					set_position( 1, FD_DEBT_PN );
			}
			else {
				set_position( ln, FD_DEBT_PN );
			}
		}
		else {
			if( pn == FD_DSKBN_PN ) {
				set_position( ln, FD_TKY_PN );
			}
			else {
				set_position( ln, FD_CRVAL_PN );
			}
		}
		break;
	case VK_RETURN:
	case VK_TAB:
		if( m_bSyzYunyu[index] ) {
			if( index == 0 )
				set_position( ln, FD_DBZEI_PN );
			else
				set_position( ln, FD_CRZEI_PN );

			break;
		}
		// else は 下へ
	case VK_DELETE:
		if( IsKind() == KIND_FURIKAE ) {
			if(	pn == FD_DSKBN_PN ) {
				int dt_ln = get_dataline(ln);
				// 外税＋免税事業者
				if( _getCND(dt_ln, FD_DBZEI_PN)->ETC_sg[0] ) {
					if( _getCND(dt_ln, FD_DBVAL_PN)->INP_sg ) {
						if( _getCND(dt_ln, FD_DSKBN_PN)->ETC_sg[1] == FD_DBZEI_PN ) {
							if( set_position(ln, FD_CBMN_PN) == -1 ) {
								if( set_position(ln, FD_CKOJI_PN) == -1 ) {
									set_position(ln, FD_CRED_PN);
								}
							}
							_getCND(dt_ln, FD_DSKBN_PN)->ETC_sg[1] = 0;
						}
						else {
							if( !_getCND(dt_ln, FD_DBZEI_PN)->ETC_sg[2] ) {
								//借方消費税欄から移動したサインON
								_getCND(dt_ln, FD_DBZEI_PN)->ETC_sg[1] = FD_DSKBN_PN;
								_getCND(dt_ln, FD_DBZEI_PN)->ETC_sg[2]++;
								set_position(get_nowln(), FD_DBZEI_PN);
							}
							else {
								if( set_position(ln, FD_CBMN_PN) == -1 ) {
									if( set_position(ln, FD_CKOJI_PN) == -1 ) {
										set_position(ln, FD_CRED_PN);
									}
								}
							}
						}
					}
					else {
						set_position(get_nowln(), FD_DBVAL_PN);
					}
				}
				else {
					if( set_position(ln, FD_CBMN_PN) == -1 ) {
						if( set_position(ln, FD_CKOJI_PN) == -1 ) {
							set_position(ln, FD_CRED_PN);
						}
					}
				}
			}
			else {
				int dt_ln = get_dataline(ln);
				// 外税＋免税事業者
				if( _getCND(dt_ln, FD_CRZEI_PN)->ETC_sg[0] ) {
					if( _getCND(dt_ln, FD_CRVAL_PN)->INP_sg ) {
						//借方消費税欄から移動したサインON
					//	_getCND(dt_ln, FD_CRZEI_PN)->ETC_sg[1] = FD_CSKBN_PN;
					//	set_position(get_nowln(), FD_CRZEI_PN);
						set_position(get_nowln(), FD_CRVAL_PN);
					}
					else {
						set_position(get_nowln(), FD_CRVAL_PN);
					}
				}
				else {
					if( _getCND(dt_ln, pn)->SET_sg ) {
						_getCND(dt_ln, pn)->SET_sg = 0;
						OnTermTky(ln, VK_RETURN);
					}
					else {
						if( IsType() == TYPE_FUKUGOU )
							set_position(ln, FD_CRVAL_PN);
						else {
							set_position(ln, FD_TKY_PN);
						}
					}
				}
			}
		}
		else {
			//自動仕訳で摘要から科目をセットした場合、摘要が２回表示されることになるので、OnTermTkyにまかせる
			int dt_ln = get_dataline(ln);
			if( _getCND(dt_ln, pn)->SET_sg ) {
				_getCND(dt_ln, pn)->SET_sg = 0;
				OnTermTky(ln, VK_RETURN);
			}
			else {
				int dt_ln = get_dataline(ln);
				// 外税＋免税事業者
				if( _getCND(dt_ln, zeipn)->ETC_sg[0] ) {
					if( _getCND(dt_ln, valpn)->INP_sg ) {
						if( _getCND(dt_ln, pn)->ETC_sg[1] == zeipn ) {
							set_position(ln, FD_TKY_PN);
						}
						else {
							_getCND(dt_ln, zeipn)->ETC_sg[1] = pn;
							set_position(ln, zeipn);
						}
					}
					else {
						set_position(ln, valpn);
					}
				}
				else {
					set_position(ln, FD_TKY_PN);
				}
			}
		}
		break;
	case VK_LEFT:
	case VK_F2:
		if( IsKind() == KIND_SYUKKIN )		set_position( ln, FD_DBVAL_PN );
		else if( IsKind() == KIND_SYUKKIN )	set_position( ln, FD_CRVAL_PN );
		else {
			if( pn == FD_DSKBN_PN ) {
				set_position( ln, FD_DBVAL_PN );
			}
			else {
				set_position( ln, FD_CRED_PN );
			}
		}
		break;
	default:
		Buzzer();
		break;
	}

	return 0;
}



//--------------------------------------------------------------
// 伝票ダイアログより キーターミネーション時のくくり
//	int ln		画面伝票上の　行番号(１～最大７)
//	int pn		今回のポジション
//--------------------------------------------------------------
int CDnpBaseClass::ToViewTermJob( int ln, int pn, long nChar )
{
	switch( pn ) {
	case FD_DBVAL_PN:	OnTermDebtVal( ln, nChar );	break;
	case FD_DBZEI_PN:	OnTermDebtZei( ln, nChar );	break;
	case FD_DBMN_PN:	OnTermDebtBmn( ln, nChar );	break;
	case FD_DKOJI_PN:	OnTermDebtKoji( ln, nChar );	break;
	case FD_DEBT_PN:	OnTermDebt( ln, nChar );	break;
	case FD_CRVAL_PN:	OnTermCredVal( ln, nChar );	break;
	case FD_CRZEI_PN:	OnTermCredZei( ln, nChar );	break;
	case FD_CBMN_PN: 	OnTermCredBmn( ln, nChar );	break;
	case FD_CKOJI_PN:	OnTermCredKoji( ln, nChar );	break;
	case FD_CRED_PN: 	OnTermCred( ln, nChar );	break;
	case FD_TKY_PN:		OnTermTky( ln, nChar );		break;
	case FD_DSKBN_PN:
	case FD_CSKBN_PN:	TerminationSyz( nChar );
						break;
	case FD_TGNUM_PN:	OnTermTgNum( ln, nChar );	break;
	case FD_TGDATE_PN:	OnTermTgDate( ln, nChar );	break;
	
	case FD_HJIYU_PN:	OnTermHjiyu( ln, nChar );	break;

	case FD_DSYOG_PN:	OnTermDsyog( ln, nChar );	break;
	case FD_CSYOG_PN:	OnTermCsyog( ln, nChar );	break;

	case DP_BMN_PN:
	case DP_KOJI_PN:
	case DP_SYONUM_PN:
		ToViewUpsideTermJob( pn, nChar );
		break;
	default:
		break;
	}

	return 0;
}


//--------------------------------------------------------------
// 伝票ダイアログより 上段の ターミネーション
//--------------------------------------------------------------
int CDnpBaseClass::ToViewUpsideTermJob( int pn, long nChar )
{
	int kind = IsKind();

	if( kind != KIND_FURIKAE ) {
		if( pn == DP_DD_PN ) {
			switch( nChar ) {
			case VK_RETURN:
			case VK_TAB:
			case VK_RIGHT:
				if( m_DI.IsSyonum() ) {
					_set_focus(-1, DP_SYONUM_PN);
				}
				else if( m_DI.IsBumon() ) {
					_set_focus( -1, DP_BMN_PN );
				}
				else if( m_DI.IsKoji() ) {
					_set_focus( -1, DP_KOJI_PN );
				}
				else {
					if( IsAutoSiwake(1) ) {
						set_position( 1, (kind==KIND_SYUKKIN) ? FD_DBVAL_PN : FD_CRVAL_PN );
					//	_set_focus( 1, (kind==KIND_SYUKKIN) ? FD_DBVAL_PN : FD_CRVAL_PN );
					}
					else {
						set_position( 1, (kind==KIND_SYUKKIN) ? FD_DEBT_PN : FD_CRED_PN );
					//	_set_focus( 1, (kind==KIND_SYUKKIN) ? FD_DEBT_PN : FD_CRED_PN );
					}
				}
			case VK_F2:
			case VK_LEFT:
				GetNowDnpDialog()->set_prevfocus( -1, pn );
				break;
			case VK_DOWN:
				if( IsAutoSiwake(1) ) {
					set_position( 1, (kind==KIND_SYUKKIN) ? FD_DBVAL_PN : FD_CRVAL_PN );
				//	_set_focus( 1, (kind==KIND_SYUKKIN) ? FD_DBVAL_PN : FD_CRVAL_PN );
				}
				else {
					set_position( 1, (kind==KIND_SYUKKIN) ? FD_DEBT_PN : FD_CRED_PN );
				//	_set_focus( 1, (kind==KIND_SYUKKIN) ? FD_DEBT_PN : FD_CRED_PN );
				}
				break;
			}
		}
		else if( pn == DP_BMN_PN ) {
			switch( nChar ) {
			case VK_RETURN:
			case VK_TAB:
			case VK_RIGHT:
				if( m_DI.IsKoji() ) {
					_set_focus( -1, DP_KOJI_PN );
				}
				else {
					if( IsAutoSiwake(1) ) {
						set_position( 1, (kind==KIND_SYUKKIN) ? FD_DBVAL_PN : FD_CRVAL_PN );
					//	_set_focus( 1, (kind==KIND_SYUKKIN) ? FD_DBVAL_PN : FD_CRVAL_PN );
					}
					else {
						set_position( 1, (kind==KIND_SYUKKIN) ? FD_DEBT_PN : FD_CRED_PN );
					//	_set_focus( 1, (kind==KIND_SYUKKIN) ? FD_DEBT_PN : FD_CRED_PN );
					}
				}
				break;
			case VK_F2:
			case VK_LEFT:
				GetNowDnpDialog()->set_prevfocus( -1, pn );
				break;
			case VK_DOWN:
				if( IsAutoSiwake(1) ) {
					set_position( 1, (kind==KIND_SYUKKIN) ? FD_DBVAL_PN : FD_CRVAL_PN );
				//	_set_focus( 1, (kind==KIND_SYUKKIN) ? FD_DBVAL_PN : FD_CRVAL_PN );
				}
				else {
					set_position( 1, (kind==KIND_SYUKKIN) ? FD_DEBT_PN : FD_CRED_PN );
				//	_set_focus( 1, (kind==KIND_SYUKKIN) ? FD_DEBT_PN : FD_CRED_PN );
				}
				break;
			}
		}
		else if( pn == DP_KOJI_PN ) {
			switch( nChar ) {
			case VK_RETURN:
			case VK_TAB:
			case VK_RIGHT:
			case VK_DOWN:
				if( IsAutoSiwake(1) ) {
					set_position( 1, (kind==KIND_SYUKKIN) ? FD_DBVAL_PN : FD_CRVAL_PN );
				//	_set_focus( 1, (kind==KIND_SYUKKIN) ? FD_DBVAL_PN : FD_CRVAL_PN );
				}
				else {
					set_position( 1, (kind==KIND_SYUKKIN) ? FD_DEBT_PN : FD_CRED_PN );
				//	_set_focus( 1, (kind==KIND_SYUKKIN) ? FD_DEBT_PN : FD_CRED_PN );
				}
				break;
			case VK_F2:
			case VK_LEFT:
				GetNowDnpDialog()->set_prevfocus( -1, pn );
				break;
			}
		}
		else {
			GetNowDnpDialog()->UpsideTermJob( pn, nChar );
		}
	}
	else {
		// 振替伝票
		if( pn == DP_DENP_PN ) {
			if( nChar == VK_UP || nChar == VK_F2 ) {
				// 部門 単一入力 切り替え にフォーカスをセット
				if( m_DI.IsBumon() || m_DI.IsKoji() ) {
					GetNowDnpDialog()->cursor_set( IDC_BMNINP_CHK );
					return 0;
				}
			}
		}
		else if( pn == DP_MM_PN || pn == DP_DD_PN ) {
			if( nChar == VK_UP ) {
				// 部門 単一入力 切り替え にフォーカスをセット
				if( m_DI.IsBumon() || m_DI.IsKoji() ) {
					GetNowDnpDialog()->cursor_set( IDC_BMNINP_CHK );
					return 0;
				}
			}
		}

		GetNowDnpDialog()->UpsideTermJob( pn, nChar );
	}

	return 0;
}


//--------------------------------------------------------------
// 伝票ダイアログより カナターミネーション
//--------------------------------------------------------------
void CDnpBaseClass::ToViewKanaTerm( LPCTSTR data )
{
	char kana[16] = {0};
	char k_data[16] = {0};

	CString stmp(data);
	if( stmp.GetLength() > 8 ) {
		stmp.SetAt(8, '\0');
	}
	kana_copy(k_data, stmp);
	kana_copy(kana, stmp);

	SelPar par;
	if( BRNTKY_sel ) {
		par.sel_pn = SL_BRNTKY_PN;

		BRNTKY_PAR brp;
		brp = m_Seldata.GetBrnTkyPar();

		par.tkybrn	= brp.bt_code;
		par.brn_bmn = brp.bt_bmn;
		// 絞込み
		stmp = saveITEM50kana;
		stmp += k_data;
		sprintf_s(par.brn_kana, sizeof par.brn_kana, "%.8s", stmp);
	}
	else if( m_SelDispPN == SL_KMKANA_PN ) {
		par.sel_pn = SL_KMKANA_PN;
#ifdef CLOSE // 07.17 /07
		sprintf_s( kana, sizeof kana, "%s%s", saveITEM50kana, k_data );
#endif
		memcpy( par.kmk_kana, kana, sizeof par.kmk_kana );
	}
	else {
		par.sel_pn = SL_KMKANA_PN;
		memcpy( par.kmk_kana, kana, sizeof par.kmk_kana );
	}

	int st = m_Seldata.SelectJob( &par, -1 );

	if( m_SelDispPN == SL_KMKANA_PN ) {
		if( ! st )
			strcpy_s( saveITEM50kana, sizeof saveITEM50kana, kana);	//save
	}
	else {
		::ZeroMemory( saveITEM50kana, sizeof saveITEM50kana );
		if( ! BRNTKY_sel )	Buzzer();
	}

}

// 借方金額キーターミネーション
//	int ln ... 画面伝票上の　行番号(１～最大７)
void CDnpBaseClass::OnTermDebtVal( int ln, long nChar )
{
	int vect;

//TRACE( "OnTermDebtVal TOP ln = %d, nChar = %d\n", ln, nChar );

	if( nChar == VK_UP || nChar == VK_DOWN )
	{
		int dset = 0;

		if( nChar == VK_UP && ln == 1 ) {
			dset = DnpLine_Chg(-1);
		}
		else if( nChar == VK_DOWN && ln == getSCROLL_NL() ) {
			dset = DnpLine_Chg(1);
		}

		if( !dset ) {
			// 行移動
			if( nChar == VK_UP ) {
				--ln;
				vect	=	-1;
			}
			else {
				++ln;
				vect	=	1;
			}

			//行移動エラー（アンダー／オーバー　フロー）
			if( set_position( ln, FD_DBVAL_PN, vect ) == -1 ) {
				if( nChar == VK_UP )	set_focus( DP_MM_PN );
				else {
					if( IsKind() == KIND_SYUKKIN )		set_position( 1, FD_DEBT_PN );
					else if( IsKind() == KIND_FURIKAE )	set_position( 1, FD_DBVAL_PN );
				}
			}
		}
		return;
	}
	int dt_ln = get_dataline(ln);

	// カーソルセット
	switch( nChar )
	{
	case VK_RIGHT:
	case VK_RETURN:
	case VK_TAB:
//	case VK_F3:
	case VK_DELETE:
		if( IsKind() == KIND_FURIKAE ) {
			if( _getCND(dt_ln, FD_DBZEI_PN)->ETC_sg[0] ) {
				_getCND(dt_ln, FD_DBZEI_PN)->ETC_sg[1] = FD_DBVAL_PN;
				_getCND(dt_ln, FD_DBZEI_PN)->ETC_sg[2]++;
				set_position(ln, FD_DBZEI_PN);
				break;
			}

			if( m_DI.IsBumon() && (m_FuriMode == FURI_DATABMON) ) {
				set_position( ln, FD_DBMN_PN );
			}
			else if( m_DI.IsKoji() && (m_FuriMode == FURI_DATABMON) ) {
				set_position( ln, FD_DKOJI_PN );
			}
			else {
				if( IsAutoSiwake( ln ) ) {
					if( SyzStopMode() && _getCND(dt_ln, FD_DEBT_PN)->INP_sg ) {
						set_position(ln, FD_DSKBN_PN);
					}
					else if( IsType() == TYPE_TANDOKU ) {
						set_position( ln, FD_TKY_PN );
					}
					else {
						set_position( ln, FD_CRVAL_PN );
					}
				}
				else {
					if( nChar == VK_DELETE && IsType() == TYPE_FUKUGOU && _getCND(dt_ln, FD_DEBT_PN )->INP_sg == FALSE ) {
						set_position( ln, FD_CRED_PN );
					}
					else {
						set_position( ln, FD_DEBT_PN );
					}
				}
			}
			break;
		}
		else if( IsKind() == KIND_SYUKKIN ) {
			vect = (nChar == VK_RETURN ) ? 1 : 2;

			if( vect == 1 ) {
				if( _getCND(dt_ln, FD_DBZEI_PN)->ETC_sg[0] && !_getCND(dt_ln, FD_DBZEI_PN)->ETC_sg[2] ) {
					_getCND(dt_ln, FD_DBZEI_PN)->ETC_sg[1] = FD_DBVAL_PN;
					_getCND(dt_ln, FD_DBZEI_PN)->ETC_sg[2]++;
					set_position(ln, FD_DBZEI_PN);
					break;
				}
			}
			m_SET_SYZINP_PN = m_pSyzInp->CheckSyzPn( vect );

			if( m_SET_SYZINP_PN >= 0 ) {
				set_position(ln, FD_DSKBN_PN);
			}
			else {
				if( SyzStopMode() && IsKamokuSyattr(get_nowln(), FD_DEBT_PN) ) {
					m_SET_SYZINP_PN = SY_SZKBN_PN;
					set_position(ln, FD_DSKBN_PN);
				}
				else {
					set_position(ln, FD_TKY_PN);
				}
			}
		}
		break;
	case VK_LEFT:
	case VK_F2:
		if( IsKind() == KIND_FURIKAE ) {
			if( set_position(ln - 1, FD_CRVAL_PN, -1) == -1 ) {
				if( (m_FuriMode == FURI_DATABMON) ) {
					if( m_DI.IsSyonum() ) {
						set_focus(DP_SYONUM_PN);
					}
					else {
						set_focus(DP_DD_PN);
					}
				}
				else {
					// 単一 部門・工事入力
					if( m_DI.IsKoji() )		set_focus( DP_KOJI_PN );
					else if( m_DI.IsBumon() )	set_focus( DP_BMN_PN );
					else						set_focus( DP_DD_PN );
				}
			}
		}
		else if( IsKind() == KIND_SYUKKIN ) {
			set_position( get_nowln(), FD_TKY_PN );
		}
		break;
	default:
		Buzzer();
		break;
	}
}

// 借方税額ターミネーション
//	int ln ... 画面伝票上の　行番号(１～最大７)
void CDnpBaseClass::OnTermDebtZei( int ln, long nChar )
{
	int vect;

	if( /*nChar == VK_UP ||*/ nChar == VK_DOWN )
	{
		int dset = 0;

		if( nChar == VK_UP && ln == 1 ) {
			dset = DnpLine_Chg(-1);
		}
		else if( nChar == VK_DOWN && ln == getSCROLL_NL() ) {
			dset = DnpLine_Chg(1);
		}

		if( !dset ) {
			// 行移動
			if( nChar == VK_UP ) {
				--ln;
				vect	=	-1;
			}
			else {
				++ln;
				vect	=	1;
			}

			//行移動エラー（アンダー／オーバー　フロー）
			if( set_position( ln, FD_DBZEI_PN, vect ) == -1 ) {
				if( nChar == VK_UP )	set_focus( DP_MM_PN );
				else {
					if( IsKind() == KIND_SYUKKIN )		set_position( 1, FD_DEBT_PN );
					else if( IsKind() == KIND_FURIKAE )	set_position( 1, FD_DBZEI_PN );
				}
			}
		}
		return;
	}
	int dt_ln = get_dataline(ln);

	// カーソルセット
	switch( nChar )
	{
	case VK_RIGHT:
	case VK_RETURN:
	case VK_TAB:
//	case VK_F3:
	case VK_DELETE:
		if( IsKind() == KIND_FURIKAE ) {
			if( IsAutoSiwake( ln ) ) {
				if( IsType() == TYPE_TANDOKU ) {
					set_position( ln, FD_TKY_PN );
				}
				else {
					set_position( ln, FD_CRVAL_PN );
				}
			}
			else {
				if( nChar == VK_DELETE && IsType() == TYPE_FUKUGOU && _getCND(dt_ln, FD_DEBT_PN )->INP_sg == FALSE ) {
					set_position( ln, FD_CRED_PN );
				}
				else {
					//借方科目から来た場合。
					if( _getCND(dt_ln, FD_DBZEI_PN)->ETC_sg[1] == FD_DEBT_PN ) {
						if( SyzStopMode() ) {
							_getCND(dt_ln, FD_DSKBN_PN)->ETC_sg[1] = FD_DBZEI_PN;
							set_position(ln, FD_DSKBN_PN);
						}
						else {
							set_position(ln, FD_CRED_PN);
						}
					}
					else if( _getCND(dt_ln, FD_DBZEI_PN)->ETC_sg[1] == FD_DSKBN_PN ) {
						set_position(ln, FD_CRED_PN);
					}
					else {
						_getCND(dt_ln, FD_DEBT_PN)->ETC_sg[1] = FD_DBZEI_PN;
						set_position(ln, FD_DEBT_PN);
					}
				}
			}
			_getCND(dt_ln, FD_DBZEI_PN)->ETC_sg[1] = 0;
			break;
		}
		else if( IsKind() == KIND_SYUKKIN ) {
			vect = (nChar == VK_RETURN) ? 1 : 2;
			m_SET_SYZINP_PN = m_pSyzInp->CheckSyzPn(vect);

			if( _getCND(dt_ln, FD_DBZEI_PN)->ETC_sg[0] ) {
				if( _getCND(dt_ln, FD_DBZEI_PN)->ETC_sg[1] == FD_DSKBN_PN ) {
					set_position(ln, FD_TKY_PN);
					break;
				}
			}
			if( m_SET_SYZINP_PN >= 0 ) {
				if( _getCND(dt_ln, FD_DBZEI_PN)->ETC_sg[0] ) {
					_getCND(dt_ln, FD_DSKBN_PN)->ETC_sg[1] = FD_DBZEI_PN;
				}
				set_position(ln, FD_DSKBN_PN);
			}
			else {
				if( SyzStopMode() && IsKamokuSyattr(get_nowln(), FD_DEBT_PN) ) {
					m_SET_SYZINP_PN = SY_SZKBN_PN;
					set_position(ln, FD_DSKBN_PN);
					if( _getCND(dt_ln, FD_DBZEI_PN)->ETC_sg[0] ) {
						_getCND(dt_ln, FD_DSKBN_PN)->ETC_sg[1] = FD_DBZEI_PN;
					}
				}
				else {
					set_position(ln, FD_TKY_PN);
				}
			}
		}
		break;
	case VK_LEFT:
	case VK_F2:
	case VK_UP:
		set_position(get_nowln(), FD_DBVAL_PN);
#ifdef CLOSE
		if( IsKind() == KIND_FURIKAE ) {
			if( (nChar == VK_F2 || nChar == VK_LEFT) && (ln > 1) &&
					_getCND( ln - 1, FD_CSKBN_PN )->INP_sg == TRUE ) {
				if( set_position( ln - 1, FD_CSKBN_PN, -1 ) == -1 ) {
					if( (m_FuriMode == FURI_DATABMON) )
						set_focus( DP_DD_PN );
					else {
						// 単一 部門・工事入力
						if( m_DI.IsKoji() )		set_focus( DP_KOJI_PN );
						else if( m_DI.IsBumon() )	set_focus( DP_BMN_PN );
						else						set_focus( DP_DD_PN );
					}
				}
			}
			else {
				if( set_position( ln - 1, FD_CRVAL_PN, -1 ) == -1 ) {
					if( (m_FuriMode == FURI_DATABMON) )
						set_focus( DP_DD_PN );
					else {
						// 単一 部門・工事入力
						if( m_DI.IsKoji() )		set_focus( DP_KOJI_PN );
						else if( m_DI.IsBumon())	set_focus( DP_BMN_PN );
						else				set_focus( DP_DD_PN );
					}
				}
			}
		}
		else if( IsKind() == KIND_SYUKKIN ) {
			set_position( get_nowln(), FD_TKY_PN );
		}
#endif
		break;
	default:
		Buzzer();
		break;
	}
}



void	CDnpBaseClass::OnTermHjiyu( int ln, long nChar )
{
	int vect;
	int dset;

	if( nChar == VK_UP || nChar == VK_DOWN )
	{
		dset = 0;

		if( nChar == VK_UP && ln == 1 ) {
			dset = DnpLine_Chg(-1);
		}
		else if( nChar == VK_DOWN && ln == getSCROLL_NL() ) {
			dset = DnpLine_Chg(1);
		}

		if( !dset ) {
			// 行移動
			if( nChar == VK_UP ) {
				--ln;
				vect	=	-1;
			}
			else {
				++ln;
				vect	=	1;
			}
			//行移動エラー（アンダー／オーバー　フロー）
			if( set_position( ln, FD_HJIYU_PN, vect ) == -1 ) {
				if( nChar == VK_UP )	set_focus( DP_MM_PN );
				else {
					if( IsKind() == KIND_NYUKIN )		set_position( 1, FD_CRED_PN );
					else if( IsKind() == KIND_SYUKKIN )	set_position( 1, FD_DEBT_PN );
					else								set_position( 1, FD_DBVAL_PN );
				}
			}
		}
		return;
	}

	CString dbt, cre;
	BOOL bBill = FALSE;

	// カーソルセット
	switch( nChar )
	{
	case VK_RIGHT:
		set_position( ln, FD_HUSEN_PN );
		break;
	case VK_TAB:
	case VK_RETURN:
//	case VK_F3:
	case VK_DELETE:

		if( IsAutoSiwake( ln + 1 ) ) {
			dset = 0;
			if( ln == getSCROLL_NL() ) {
				dset = DnpLine_Chg(1,1);
			}

			if( ! dset ) {
				if( IsKind() == KIND_NYUKIN ) {
					if( set_position( ++ln, FD_CRVAL_PN ) == -1 )
						set_position( 1, FD_CRVAL_PN );
				}
				else {
					if( set_position( ++ln, FD_DBVAL_PN ) == -1 )
						set_position( 1, FD_DBVAL_PN );
				}
			}
			else {
				if( IsKind() == KIND_NYUKIN ) {
					set_position( ln, FD_CRVAL_PN );
				}
				else {
					set_position( ln, FD_DBVAL_PN );
				}
			}
		}
		else {
			dset = 0;
			if( ln == getSCROLL_NL() ) {
				dset = DnpLine_Chg(1,1);
			}

			if( ! dset ) {
				if( IsKind() == KIND_NYUKIN ) {
					if( set_position( ++ln, FD_CRED_PN ) == -1 )
						set_position( 1, FD_CRED_PN );
				}
				else if( IsKind() == KIND_SYUKKIN ) {
					if( set_position( ++ln, FD_DEBT_PN ) == -1 )
						set_position( 1, FD_DEBT_PN );
				}
				else {
					if( set_position( ++ln, FD_DBVAL_PN ) == -1 )
						set_position( 1, FD_DBVAL_PN );
				}
			}
			else {
				if( IsKind() == KIND_NYUKIN ) {
					set_position( ln, FD_CRED_PN );
				}
				else if( IsKind() == KIND_SYUKKIN ) {
					set_position( ln, FD_DEBT_PN );
				}
				else {
					set_position( ln, FD_DBVAL_PN );
				}
			}
		}
		break;
	case VK_LEFT:
	case VK_F2:
		// 手形科目チェック
		get_linekamokucode( ln, dbt, cre );

		if( ! dbt.IsEmpty() )
			bBill = IsBillCodeEx( dbt, m_pZm );
		if( ! bBill ) {
			if( ! cre.IsEmpty() )
				bBill = IsBillCodeEx( cre, m_pZm );
		}

		if( bBill ) {
			// 手形番号
			set_position( ln, FD_TGDATE_PN );
		}
		else {
			if( IsKind() == KIND_NYUKIN )		set_position( ln, FD_CRED_PN );
			else								set_position( ln, FD_DEBT_PN );
		}
		break;

	case VK_PRIOR:
	case VK_NEXT:
	case VK_HOME:
		SelNaibuDsp(nChar);
		break;
	default:
		Buzzer();
		break;
	}
}


LRESULT CDnpBaseClass::OnHjiyuInEnd( WPARAM w, LPARAM l )
{
	OnTermHjiyu( get_nowln(), w );

	return 1;
}


LRESULT CDnpBaseClass::OnHjiyuSel( WPARAM w, LPARAM l )
{
	SelResult srt;
	CString	str;

	if( w > 0 ) {
		_hjiyu_selectsub( w, 0, &str );
	}
	else {
		getDATA( FD_HJIYU_PN )->DP_BMN = 0;
		getDATA( FD_HJIYU_PN )->NAIBCD = -1;
		getCND( FD_HJIYU_PN )->INP_sg = FALSE;
	}

	GetNowDnpDialog()->SetHjiyu( get_nowln(), FD_HJIYU_PN, &str );

	return 1;
}

void CDnpBaseClass::_hjiyu_selectsub( int number, int selclick, CString* pStr )
{
	BOOL bHendo, bNaibu;
	char buf[128];
	SelResult	srt;
	int old_hjiyu;

	if( m_pZm->zvol->apno < 0x50 )	bHendo = TRUE;
	else {
		bHendo = FALSE;

		if( IsSyafuKaisei( m_pZm->zvol ) )	bNaibu = TRUE;
		else								bNaibu = FALSE;
	}

	if (m_pHjiyuInp ) {
		if( ! m_pHjiyuInp->IsDisplay() ) {
			return;
		}
	}
	else {
		return;
	}

	sprintf_s( buf, sizeof buf, "%d", number );

	if( bHendo )	srt.sel_pn = SL_HJIYU_PN;
	else {
		srt.sel_pn = bNaibu ? SL_NAIBU_PN : SL_NPFURI_PN;
	}

	if( m_Seldata.GetSelResult( &srt, buf ) ) {
		old_hjiyu = getDATA( FD_HJIYU_PN )->DP_BMN;

		// 変動事由選択OK
		HJIYU_DATA hd = {0};
		hd.hjy_code = srt.code;
		strcpy_s( hd.hjy_name, sizeof hd.hjy_name, srt.selname );

		if( bNaibu ) {
			hd.hjy_nbcd = srt.bmn;
			getDATA( FD_HJIYU_PN )->NAIBCD = srt.bmn;
		}
		else {
			getDATA( FD_HJIYU_PN )->NAIBCD = -1;
		}

		m_pHjiyuInp->SetHjiyuData( &hd, TRUE );

		getDATA( FD_HJIYU_PN )->DP_BMN = srt.code;
		getCND( FD_HJIYU_PN )->INP_sg = TRUE;
		// 事由名称を摘要へ
		if( bHendo && srt.code > 0 && old_hjiyu != srt.code ) {
			hjiyu_to_tekiyo( &srt.selname );
		}

		if( selclick ) {
			GetNowDnpDialog()->SetHjiyu( get_nowln(), FD_HJIYU_PN,  &srt.selname );
		}

		if( pStr != NULL )	*pStr = srt.selname;
	}
}


// 変動事由を 仕訳摘要へ
void CDnpBaseClass::hjiyu_to_tekiyo( CString* jiyu )
{
	if( m_pAsel->HJIYU_OPT != OPT_HJIYU )
		return;

	BOOL bImg = FALSE;
	int ln		= get_nowln();
	int dt_ln	= get_dataline( ln );

	if( IsJob() == JOB_SCAN ) {
		if( tblImage[dt_ln-1][0] || tblImage[dt_ln-1][1] )
			bImg = TRUE;
	}

	if( ! bImg ) {

		CString tmp;
		tmp = _getDATA( dt_ln, FD_TKY_PN )->DP_TKY;

		// すでに同じ 文字列が摘要に含まれていないかをチェック
		if( IsIncludeString( tmp, *jiyu ) )
			return;

		char buf[128] = {0};
		strcpy_s( buf, sizeof buf, (const char*)_getDATA( dt_ln, FD_TKY_PN )->DP_TKY );
		strcat_s( buf, sizeof buf, *jiyu );

		int n = get_strcnt( buf, (m_pZm->zvol->tk_ln * 2) );
		buf[n] = '\0';

		::ZeroMemory( _getDATA( dt_ln, FD_TKY_PN ), sizeof (struct _DenpData) );
		memcpy( _getDATA( dt_ln, FD_TKY_PN )->DP_TKY, buf, n );
		_getCND( dt_ln, FD_TKY_PN )->INP_sg = TRUE;

		GetNowDnpDialog()->SetTekiyo( ln, FD_TKY_PN, (char*)_getDATA( dt_ln, FD_TKY_PN )->DP_TKY );
	}
}



// "（決修）"チェック＆表示
void CDnpBaseClass::kesyu_chkdsp()
{
	char* str = "";

	if( getCND( DP_MM_PN )->INP_sg ) {
		int ksgn = KESY_SG_GET( getDATA( DP_MM_PN )->DP_BCDDAY[0] );

		switch( ksgn ) {
		case 1:		str = "１Ｑ";	break;
		case 2:		str = "中間";	break;
		case 3:		str = "３Ｑ";	break;
		case 4:		str = "決修";	break;
		default:
			break;
		}
	}

	GetNowDnpDialog()->kesyu_dsp( str );
}




// 部門番号IMEのEND処理
int CDnpBaseClass::CheckImeEndBmon( long nChar, LPCTSTR string ) 
{
	VARIANT var;

	CString buf = string;
	int		ret = 0;
	UINT id = IDC_ICSDBEDT_BMON;

	if( nChar == VK_TAB || nChar == VK_F12 ) {
		// カナ検索
		if( ! isdigitstr( (char*)string ) ) {
			SelPar par;
			::ZeroMemory( par.kmk_kana, sizeof par.kmk_kana );
			par.sel_pn = SL_BMNKANA_PN;
			memcpy( par.kmk_kana, string, sizeof par.kmk_kana );

			m_Seldata.SelectJob( &par, -1 );
			ret = 1;
		}
		else {
			var.pbVal = (BYTE*)buf.GetBuffer(64);
			buf.ReleaseBuffer();

			DBdata_set( GetNowDnpDialog(), id, &var, ICSDBEDT_TYPE_STRING, 0 );
		}
	}
	else {
		if( isdigitstr( (char*)string ) ) {
			var.pbVal = (BYTE*)buf.GetBuffer(64);
			buf.ReleaseBuffer();

			DBdata_set( GetNowDnpDialog(), id, &var, ICSDBEDT_TYPE_STRING, 0 );
		}
		else {
			ret = 1;
		}
	}

	return ret;
}


//--------------------------------------------------------
//	部門番号入力ターミネーション
//
//--------------------------------------------------------
void CDnpBaseClass::TerminationBmonDBedit( UINT id, long nChar, long inplen, long kst )
{
	if( ! m_PRO_OK )	return;

	VARIANT var;
	int pn, ln, dt_ln, bmn, prev_bmn;
	char	data[32];

	CWnd* pDlgWnd;
	pDlgWnd = GetNowDnpDialog();
	pn	= get_nowpn();

	ln		= get_nowln();
	dt_ln	= get_dataline(ln);

	BOOL bModify_Ok;
	bModify_Ok = (_getCND( dt_ln, pn )->EDT_sg <= 1);
	if( m_DI.IsConFirmMaster() )	bModify_Ok = FALSE;

	if( pn == DP_BMN_PN ) {
		if( IsTransDenpyo()	)	bModify_Ok = FALSE;
	}

	DBdata_get( pDlgWnd, id, &var, ICSDBEDT_TYPE_STRING, 0 );

	bmn = -1;
	if( var.pbVal != NULL ) {
		CString sTmp = (const char*)var.pbVal;
		if( sTmp.GetLength() > 0 )
			bmn = atoi( (const char*)var.pbVal );
	}

	if( bModify_Ok ) {
		prev_bmn = getDATA( pn )->DP_BMN;

		// 2桁入力 は、選択画面からか？
		if( inplen > 0 ) {
			CString input;
			if( var.pbVal != NULL ) {
				input = var.pbVal;
			}

			if( Bmn_Check( pn, nChar, &bmn, &input ) ) {
				//選択画面から入力
				set_codestr( data, sizeof data, bmn, m_pZm->zvol->bmcol );
			//	m_pZm->BumonCodeToStr( data, sizeof data, bmn );

				var.pbVal = (BYTE*)data;
			 	DBdata_set( pDlgWnd, id, &var, ICSDBEDT_TYPE_STRING, 0 );
			}
			else {
				if( bmn == -1 ) {
					var.pbVal = NULL;
					DBdata_set( pDlgWnd, id, &var, ICSDBEDT_TYPE_STRING, 0 );
				}
			}
		}

		if( nChar == VK_RETURN || ! nChar ) {
			// 部門コピー
			if( nChar == VK_RETURN && ! _getCND( dt_ln, pn )->INP_sg )
				Bmn_Copy( ln, pn );

			if( bmn == -1 ) {
				bmn = _getDATA( dt_ln, pn )->DP_BMN;
			}
			else {
				_getDATA( dt_ln, pn )->DP_BMN = bmn;
			}

			struct _SET_KMKETC sk;
			sk.bmn = bmn;
			sk.code.Empty();
			sk.kno.Empty();
			sk.brn = -1;
			sk.etc.Empty();

			//部門名称を表示する場合
			if( GetBmnEtc( &sk ) ) {
				strcpy_s( getCND(pn)->ETC_msg, sizeof getCND(pn)->ETC_msg, sk.etc );
				getCND(pn)->ETC_col = sk.etc_col;

				GetNowDnpDialog()->SetBmon( ln, pn, bmn, &sk );
			}
			else {
				::ZeroMemory( getCND(pn)->ETC_msg, sizeof getCND(pn)->ETC_msg );
				getCND(pn)->ETC_col = 0;

				GetNowDnpDialog()->SetBmon( ln, pn, bmn, NULL );
			}
			// 入力サイン ON
			if( nChar || (!nChar && bmn != -1 ) )
				_getCND( dt_ln, pn )->INP_sg = TRUE;

			if( prev_bmn != bmn && ! sk.etc.IsEmpty() ) {
				bmnkoji_to_tekiyo( &sk.etc );
				bmneda_update(ln, pn);
			}
		}

		if( nChar == VK_DELETE ) {
			var.pbVal = NULL;
			DBdata_set( pDlgWnd, id, &var, ICSDBEDT_TYPE_STRING, 0 );

			_getDATA( dt_ln, pn )->DP_BMN = -1;
			_getCND( dt_ln, pn )->INP_sg = FALSE;
			bmn = -1;
			GetNowDnpDialog()->SetBmon( ln, pn, bmn, NULL );
			bmneda_update(ln, pn);
		}
	}
	else {
		if( nChar == 0x16 ) {
			prev_bmn = getDATA(pn)->DP_BMN;
			if( prev_bmn != bmn ) {
				set_codestr(data, sizeof data, prev_bmn, m_pZm->zvol->bmcol);
				var.pbVal = (BYTE*)data;
				DBdata_set(pDlgWnd, id, &var, ICSDBEDT_TYPE_STRING, 0);
			}
		}
	}

	// 工事切替
	if( nChar != 0 ) {
		nChar = SelBmonDsp( nChar );
	}

	if( nChar != 0 ) {
		// ターミネーション処理
		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			nChar = VK_F2;

		ToViewTermJob( ln, pn, nChar );
	}
}



//--------------------------------------------------------
//	部門入力チェック
//	int*		bmn		部門コードをこの関数でセット
//	CString*	edit	Diagより入力されたデータ
//	
//	返送値		1	部門入力OK
//				0	部門未入力
//			   -1	合計部門を入力したので キャンセル
//--------------------------------------------------------
int CDnpBaseClass::Bmn_Check( int pn, long& nChar, int *bmn, CString* edit )
{
	int len, number;
	CDBINPDataRec	data;
	int ret = 0;
	long nKey;

	// 部門選択 画面番号優先( VK_RETURN )
	nKey = (m_pAsel->BMNSEL_OPT == OPT_BMNSEL) ? VK_RETURN : VK_TAB;

	if( edit != NULL ) {
		len = edit->GetLength();
		
		// 2ケタ入力
		if( (len  > 0 && len <= 2) && nKey == VK_RETURN ) {
			// 選択画面より番号選択
			number = atoi( *edit );

			if(	bmncode_todata( &data, number, 0 ) != FALSE ) {
				*bmn = data.m_dbmn;
				ret = 1;
			}
			else {
				*bmn = -1;
			}
		}
		else {
			if( (len  > 0 && len <= 2) && nChar == VK_TAB && nChar == nKey ) {
				// 選択画面より番号選択
				number = atoi( *edit );

				if(	bmncode_todata( &data, number, 0 ) != FALSE ) {
					*bmn = data.m_dbmn;
					ret = 1;
				}
				else {
					*bmn = -1;
				}
			}
			else {
				number = atoi( *edit );
				if( number < 0 )
					ret = 0;
				else {
					if( m_DI.IsGokeiBumon( number ) ) {
						// 合計部門は入力不可
						m_PRO_OK = FALSE;
						myICSMessageBox("合計部門は入力できません！");
						m_PRO_OK = TRUE;

						nChar	= 0;	// カーソル移動なし
						number	= -1;
						ret = -1;
					}

					if( CheckBumonInput( number ) ) {
						nChar	= 0;	// カーソル移動なし
						number	= -1;
						ret = -1;
					}
				}

				*bmn = number >= 0 ? number : -1;
			}
		}
	}
	else {
		*bmn = -1;
		ret = 0;
	}

	return ret;
}

//--------------------------------------------------------
//	部門入力＆オフフォーカス
//	long nChar	キーコード
//	int* bmn	部門コード
//--------------------------------------------------------
long CDnpBaseClass::Bmn_Inp( int pn, long nChar, int *bmn )
{
	int setbmn;
	int dt_ln = get_dataline();

	if( nChar == VK_RETURN || nChar == VK_TAB ||
		nChar == VK_UP || nChar == VK_DOWN || ! nChar ) {

		if( nChar == VK_RETURN && ! _getCND( dt_ln/*get_nowln()*/, pn )->INP_sg )
			Bmn_Copy( get_nowln(), pn );

		if( *bmn == -1 ) {
			setbmn = _getDATA( dt_ln, pn )->DP_BMN;
		}
		else {
			setbmn = *bmn;
			_getDATA( dt_ln, pn )->DP_BMN = setbmn;
		}

		struct _SET_KMKETC sk;
		sk.bmn = setbmn;
		sk.brn = -1;

		if( GetBmnEtc( &sk ) ) {
			strcpy_s( _getCND(dt_ln, pn)->ETC_msg, sizeof _getCND(dt_ln, pn)->ETC_msg, sk.etc );
			_getCND(dt_ln, pn)->ETC_col = sk.etc_col;

			GetNowDnpDialog()->SetBmon( get_nowln(), pn, setbmn, &sk );
		}
		else {
			::ZeroMemory( _getCND(dt_ln, pn)->ETC_msg, sizeof _getCND(dt_ln, pn)->ETC_msg );
			_getCND(dt_ln, pn)->ETC_col = 0;

			GetNowDnpDialog()->SetBmon( get_nowln(), pn, setbmn, NULL );
		}

		if( nChar || (!nChar && setbmn != -1) )
			_getCND( dt_ln/*get_nowln()*/, pn )->INP_sg = TRUE;
	}

	if( nChar == VK_DELETE ) {
		_getDATA( dt_ln, pn )->DP_BMN = -1;
		GetNowDnpDialog()->SetBmon( get_nowln(), pn, -1, NULL );
		_getCND( dt_ln/*get_nowln()*/, pn )->INP_sg = FALSE;
	}

	return nChar;
}



// 部門　データＣＯＰＹ
//	int ln ... 画面伝票上の　行番号(１～最大７)
//
void CDnpBaseClass::Bmn_Copy( int ln, int pn )
{
	if( pn < 0 )	return;
	struct _DenpData *prdata;
	int dt_ln = get_dataline( ln );

	// データ行の部門のみ
	if( pn < FD_DBVAL_PN )	return;

	// 伝票修正 は コピーしない [From管理課] <01.31 /07>
	BOOL bCopy = FALSE;
#ifdef CLOSE // 01.21 /11
	if( IsJob() == JOB_INPUT &&  !_getCND( dt_ln, pn )->INP_sg )
#else
	if( ! _getCND( dt_ln, pn )->INP_sg )
#endif
		bCopy = TRUE;

	// 諸口詰めないで表示するとき
	if( FuriSyogPosition( dt_ln, pn ) )	return;

	if( bCopy && (prdata = _getPRDATA( dt_ln, pn )) ) {
		Bmn_Inp( pn, 0, &prdata->DP_BMN );
	}
}


//-----------------------------------------------------
//	部門名称を取得
//
//-----------------------------------------------------
BOOL CDnpBaseClass::GetBmnEtc( struct _SET_KMKETC* psk )
{
	BOOL bDisp, ret;
	bDisp = ((m_pAsel->BMNDSP_OPT == OPT_BMNDSP) ? TRUE : FALSE);
	ret = FALSE;

	if( m_DI._set_bmnetc( psk ) ) {
		if( bDisp ) {
			ret = TRUE;
		}
	}
	return ret;
}




//-----------------------------------------------------
//	工事名称を取得
//
//-----------------------------------------------------
BOOL CDnpBaseClass::GetKojiEtc( struct _SET_KMKETC* psk )
{
	BOOL bDisp, ret;
	bDisp = ((m_pAsel->BMNDSP_OPT == OPT_BMNDSP) ? TRUE : FALSE);
	ret = FALSE;

//	if( bDisp && _set_kojietc( psk ) ) {
	if( _set_kojietc( psk ) ) {
		if( bDisp ) {
			ret = TRUE;
		}
	}

	return ret;
}


//--------------------------------------------------------------
//	工事名称を取得
//
//--------------------------------------------------------------
BOOL CDnpBaseClass::_set_kojietc( struct _SET_KMKETC *sk )
{
	BOOL ret = FALSE;

	if( m_Seldata.GetKojiName( sk->kno, sk->etc, FALSE ) ) {
		sk->etc_col = RGB_BLUE;
		ret = TRUE;
	}

	return ret;
}


// 工事番号IMEのEND処理
int CDnpBaseClass::CheckImeEndKoji( long nChar, LPCTSTR string ) 
{
	VARIANT var;

	CString buf = string;
	int		ret = 0;
	UINT id = IDC_ICSDBEDT_KOJI;

	if( nChar == VK_TAB || nChar == VK_F12  ) {
		// カナ検索
		if( ! isdigitstr( (char*)string ) ) {
			SelPar par;
			::ZeroMemory( par.kmk_kana, sizeof par.kmk_kana );
			par.sel_pn = SL_KOJIKANA_PN;
			memcpy( par.kmk_kana, string, sizeof par.kmk_kana );

			m_Seldata.SelectJob( &par, -1 );
			ret = 1;
		}
		else {
			var.pbVal = (BYTE*)buf.GetBuffer(64);
			buf.ReleaseBuffer();

			DBdata_set( GetNowDnpDialog(), id, &var, ICSDBEDT_TYPE_STRING, 0 );
		}
	}
	else {
		kana_chk( &buf, buf.GetLength() );

		var.pbVal = (BYTE*)buf.GetBuffer(64);
		buf.ReleaseBuffer();

		DBdata_set( GetNowDnpDialog(), id, &var, ICSDBEDT_TYPE_STRING, 0 );
	}

	return ret;
}


//--------------------------------------------------------
//	工事番号入力チェック
//	CString*	kno		工事番号をこの関数でセット
//	CString*	edit	Diagより入力されたデータ
//--------------------------------------------------------
BOOL CDnpBaseClass::Koji_Check( int pn, long nChar, CString* kno, CString* edit )
{
	int len, number;
	CDBINPDataRec	data;
	BOOL ret = FALSE;
	long nKey;

	// 部門選択 画面番号優先( VK_RETURN )
	nKey = (m_pAsel->BMNSEL_OPT == OPT_BMNSEL) ? VK_RETURN : VK_TAB;

	if( edit != NULL ) {
		len = edit->GetLength();
		// 数字文字列か
		BOOL bDigit = isdigitstr( (char*)(LPCTSTR)*edit );
		// 2ケタ入力
		if( (len  > 0 && len <= 2) && nKey == VK_RETURN ) {
			
			if( bDigit ) {
				// 選択画面より番号選択
				number = atoi( *edit );

				if(	kojicode_todata( &data, NULL, number, 0 ) != FALSE ) {
					*kno = data.m_dkno;
					ret = TRUE;
				}
				else {
					kno->Empty();
				}
			}
			else {
				*kno = *edit;
				ret = TRUE;
			}
		}
		else {
			if( (len  > 0 && len <= 2) && nChar == VK_TAB && nChar == nKey && bDigit ) {
				// 選択画面より番号選択
				number = atoi( *edit );

				if(	kojicode_todata( &data, NULL, number, 0 ) != FALSE ) {
					*kno = data.m_dkno;
					ret = TRUE;
				}
				else {
					kno->Empty();
				}	
			}
			else {
				*kno = *edit;
				ret = TRUE;
			}
		}
	}
	else {
		kno->Empty();
	}

	if( ret ) {
		kana_chk( kno, kno->GetLength() );
	}

	return ret;
}




//---------------------------------------------------
//	手形期日 ターミネーション
//	int ln		画面伝票上の　行番号(１～最大７)
//---------------------------------------------------
void CDnpBaseClass::OnTermTgDate( int ln, long nChar )
{
	int vect, pn;

	if( nChar == VK_UP || nChar == VK_DOWN )
	{
		// 行移動
		int dset = 0;

		if( nChar == VK_UP && ln == 1 ) {
			dset = DnpLine_Chg(-1);
		}
		else if( nChar == VK_DOWN && ln == getSCROLL_NL() ) {
			dset = DnpLine_Chg(1);
		}

		if( !dset ) {
			if( nChar == VK_UP ) {
				--ln;
				vect	=	-1;
			}
			else {
				++ln;
				vect	=	1;
			}

			//行移動エラー（アンダー／オーバー　フロー）
			if( set_position( ln, FD_TGDATE_PN, vect ) == -1 ) {
				if( nChar == VK_UP )	set_focus( DP_MM_PN );
				else {
					if( IsKind() == KIND_SYUKKIN )		set_position( 1, FD_DEBT_PN );
					else if( IsKind() == KIND_FURIKAE )	set_position( 1, FD_DBVAL_PN );
				}
			}
		}
		return;
	}

	int DEBT_ln, CRED_ln;
	CString dbt, cre;
	int dt_ln = get_dataline( ln );

	DEBT_ln = _getMAP( dt_ln, FD_DEBT_PN ),
	CRED_ln = _getMAP( dt_ln, FD_CRED_PN );
	
	if( DEBT_ln != -1 ) {
		dbt = _getDATA( DEBT_ln, FD_DEBT_PN )->DP_CdNum;
	}
	else	dbt.Empty();
	
	if( CRED_ln != -1 ) {
		cre = _getDATA( CRED_ln, FD_CRED_PN )->DP_CdNum;
	}
	else	cre.Empty();


	// カーソルセット
	switch( nChar )
	{
	case VK_RIGHT:
		set_position( ln, FD_HUSEN_PN );
		break;
	case VK_RETURN:
	case VK_TAB:
//	case VK_F3:
	case VK_DELETE:
		if( ! IsHjiyuOrNPfuri( dbt, cre ) ) {
			if (FukugoLine(0)) {
				set_position(ln, FD_DSYOG_PN);
			}
			else if (FukugoLine(1)) {
				set_position(ln, FD_CSYOG_PN);
			}
			else {
				++ln;
				vect = 1;
				if (IsKind() == KIND_FURIKAE)	pn = FD_DBVAL_PN;
				else {
					pn = (IsKind() == KIND_NYUKIN) ? FD_CRED_PN : FD_DEBT_PN;
				}
				//行移動エラー（アンダー／オーバー　フロー）
				if (set_position(ln, pn, vect) == -1) {
					if (IsKind() == KIND_SYUKKIN) {
						if (DnpLine_Chg(1, 1)) {
							set_position((ln - 1), FD_DEBT_PN);
						}
						else {
							set_position(1, FD_DEBT_PN);
						}
					}
					else if (IsKind() == KIND_NYUKIN) {
						if (DnpLine_Chg(1, 1)) {
							set_position((ln - 1), FD_CRED_PN);
						}
						else {
							set_position(1, FD_CRED_PN);
						}
					}
					else if (IsKind() == KIND_FURIKAE) {
						if (DnpLine_Chg(1, 1)) {
							set_position((ln - 1), FD_DBVAL_PN);
						}
						else {
							set_position(1, FD_DBVAL_PN);
						}
					}
				}
			}
			return;
		}
		else {
			set_position( ln, FD_HJIYU_PN );
		}
		break;
	case VK_LEFT:
	case VK_F2:
		if( IsKind() == KIND_FURIKAE || IsKind() == KIND_SYUKKIN  ) {
			_set_focus( ln, FD_TGNUM_PN );
		}
		else if( IsKind() == KIND_SYUKKIN ) {
			_set_focus( ln, FD_TGNUM_PN );
		}
		break;
	default:
		Buzzer();
		break;
	}
}



//------------------------------------------------------------------
//	手形番号を入力( DBEdit より )
//
//------------------------------------------------------------------
void CDnpBaseClass::TerminationTgNumDBedit( UINT id, long nChar, long inplen, long kst )
{
	if( ! m_PRO_OK )	return;

	struct _DenpData* keybuf;
	int		ln		= get_nowln();
	int		dt_ln	= get_dataline(ln);
	int		pn = FD_TGNUM_PN;
	keybuf = _getDATA( dt_ln, pn );

	VARIANT var;
	CWnd* pDlgWnd;
	pDlgWnd = GetNowDnpDialog();

	BOOL bModify_Ok;
	bModify_Ok = (_getCND( dt_ln, pn )->EDT_sg <= 1);
	if( m_DI.IsConFirmMaster() )	bModify_Ok = FALSE;

	DBdata_get( pDlgWnd, id, &var, ICSDBEDT_TYPE_STRING, 0 );
	if( bModify_Ok ) {
		if( var.pbVal != NULL ) {
			strcpy_s((char*)keybuf->DP_SNUM, sizeof keybuf->DP_SNUM, (const char*)var.pbVal);
			GetNowDnpDialog()->SetTgNumber(ln, pn, (char*)keybuf->DP_SNUM);
		}
		// 削除
		if( nChar == VK_DELETE ) {
			var.pbVal = NULL;
			DBdata_set(pDlgWnd, id, &var, ICSDBEDT_TYPE_STRING, 0);

			::ZeroMemory(keybuf->DP_SNUM, sizeof keybuf->DP_SNUM);
			GetNowDnpDialog()->SetTgNumber(ln, pn, NULL);
		}
	}
	else {
		if( nChar == 0x16 ) {
			var.pbVal = (BYTE*)keybuf->DP_SNUM;
			DBdata_set(pDlgWnd, id, &var, ICSDBEDT_TYPE_STRING, 0);
		}
	}

	if( keybuf->DP_SNUM[0] )	_getCND( dt_ln, pn )->INP_sg = TRUE;
	else						_getCND( dt_ln, pn )->INP_sg = FALSE;

	// ターミネーション処理
	if( nChar != 0 ) {
		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			nChar = VK_F2;

		ToViewTermJob( ln, pn, nChar );
	}
}



//---------------------------------------------------
//	手形番号 ターミネーション
//
//---------------------------------------------------
void CDnpBaseClass::OnTermTgNum( int ln, long nChar )
{
	int vect;

	if( nChar == VK_UP || nChar == VK_DOWN )
	{
		int dset = 0;

		if( nChar == VK_UP && ln == 1 ) {
			dset = DnpLine_Chg(-1);
		}
		else if( nChar == VK_DOWN && ln == getSCROLL_NL() ) {
			dset = DnpLine_Chg(1);
		}

		if( !dset ) {
			// 行移動
			if( nChar == VK_UP ) {
				--ln;
				vect	=	-1;
			}
			else {
				++ln;
				vect	=	1;
			}

			//行移動エラー（アンダー／オーバー　フロー）
			if( set_position( ln, FD_TGNUM_PN, vect ) == -1 ) {
				if( nChar == VK_UP )	set_focus( DP_MM_PN );
				else {
					if( IsKind() == KIND_SYUKKIN )		set_position( 1, FD_DEBT_PN );
					else if( IsKind() == KIND_FURIKAE )	set_position( 1, FD_DBVAL_PN );
				}
			}
		}
		return;
	}

	// カーソルセット
	switch( nChar )
	{
	case VK_RIGHT:
	case VK_RETURN:
	case VK_TAB:
//	case VK_F3:
	case VK_DELETE:
		set_position( ln, FD_TGDATE_PN );
		break;
	case VK_LEFT:
	case VK_F2:
		if( IsKind() == KIND_FURIKAE || IsKind() == KIND_SYUKKIN  ) {
			_set_focus( ln, FD_DEBT_PN );
		}
		else if( IsKind() == KIND_SYUKKIN ) {
			_set_focus( ln, FD_CRED_PN );
		}
		break;
	default:
		Buzzer();
		break;
	}
}


//-----------------------------------------------
//	摘要ターミネーション
//	int ln ... 画面伝票上の　行番号(１～最大７)
//-----------------------------------------------
void CDnpBaseClass::OnTermTky( int ln, long nChar )
{
	int vect;

	if( nChar == VK_UP || nChar == VK_DOWN )
	{
		int dset = 0;

		if( nChar == VK_UP && ln == 1 ) {
			dset = DnpLine_Chg(-1);
		}
		else if( nChar == VK_DOWN && ln == getSCROLL_NL() ) {
			dset = DnpLine_Chg(1);
		}

		if( !dset ) {
			// 行移動
			if( nChar == VK_UP ) {
				--ln;
				vect	=	-1;
			}
			else {
				++ln;
				vect	=	1;
			}
			//行移動エラー（アンダー／オーバー　フロー）
			if( set_position( ln, FD_TKY_PN, vect ) == -1 ) {
				if( nChar == VK_UP )	set_focus( DP_MM_PN );
				else {
					if( IsKind() == KIND_NYUKIN )		set_position( 1, FD_CRED_PN );
					else if( IsKind() == KIND_SYUKKIN )	set_position( 1, FD_DEBT_PN );
					else								set_position( 1, FD_DBVAL_PN );
				}
			}
		}

		return;
	}

	CString dbt, cre;
	BOOL bBill = FALSE;
	int dt_ln = get_dataline( ln );

	// カーソルセット
	switch( nChar )
	{
	case VK_RIGHT:
		set_position( ln, FD_HUSEN_PN );
		break;
	case VK_TAB:
	case VK_RETURN:

		// 手形科目チェック
		get_linekamokucode( ln, dbt, cre );

		if( ! dbt.IsEmpty() )
			bBill = IsBillCodeEx( dbt, m_pZm );
		if( ! bBill ) {
			if( ! cre.IsEmpty() )
				bBill = IsBillCodeEx( cre, m_pZm );
		}

		if( bBill ) {
			// 手形番号
			set_position( ln, FD_TGNUM_PN );
		}
		else if( IsHjiyuInput() ){	// 変動事由チェック
			set_position( ln, FD_HJIYU_PN );
		}
		else {
			if( AutoSiwakeCheck( ln ) ) {
				//摘要欄未入力
				if( IsKind() == KIND_NYUKIN ) {
					if( ! _getCND(dt_ln, FD_CRED_PN)->INP_sg ) {
						set_position( ln, FD_CRED_PN );
						break;
					}
				}
				else {
					if( IsKind() == KIND_FURIKAE ) {
						if( ! _getCND(dt_ln, FD_DEBT_PN)->INP_sg && ! _getCND(dt_ln, FD_CRED_PN)->INP_sg ) {
							set_position( ln, FD_DEBT_PN );
							break;
						}
					}
					else {
						if( ! _getCND(dt_ln, FD_DEBT_PN)->INP_sg ) {
							set_position( ln, FD_DEBT_PN );
							break;
						}
					}
				}
			}

			if( IsAutoSiwake( ln ) ) {
				if( IsKind() == KIND_NYUKIN ) {
					set_position( ln, FD_CRED_PN );
				}
				else {
					set_position( ln, FD_DEBT_PN );
				}
				break;
			}
			else {
				if( IsMode() == MODE_AUTO ) {
					if( IsKind() == KIND_NYUKIN ) {
						if( SyzStopMode() && _getCND(dt_ln, FD_CRED_PN)->SET_sg ) {
							_getCND(ln, FD_CRED_PN)->SET_sg = 0;
							set_position(ln, FD_CSKBN_PN);
							break;
						}
					}
					if( IsKind() == KIND_SYUKKIN ) {
						if( SyzStopMode() && _getCND(dt_ln, FD_DEBT_PN)->SET_sg ) {
							_getCND(ln, FD_DEBT_PN)->SET_sg = 0;
							set_position(ln, FD_DSKBN_PN);
							break;
						}
					}
					else {
						if( _getCND(dt_ln, FD_DEBT_PN)->SET_sg ) {
							_getCND(dt_ln, FD_DEBT_PN)->SET_sg = 0;
							set_position(ln, FD_DSKBN_PN);
							break;
						}
						if( _getCND(dt_ln, FD_CRED_PN)->SET_sg ) {
							_getCND(dt_ln, FD_CRED_PN)->SET_sg = 0;
							set_position(ln, FD_CSKBN_PN);
							break;
						}
					}
				}
			}

			if( IsAutoSiwake( ln + 1 ) ) {
				if( IsKind() == KIND_NYUKIN ) {
					if( set_position( ++ln, FD_CRVAL_PN ) == -1 ) {
						if( DnpLine_Chg(1,1) ) {
							set_position( (ln-1), FD_CRVAL_PN );
						}
						else {
							set_position( 1, FD_CRVAL_PN );
						}
					}
				}
				else {
					if( set_position( ++ln, FD_DBVAL_PN ) == -1 ) {
						if( DnpLine_Chg(1,1) ) {
							set_position( (ln-1), FD_DBVAL_PN );
						}
						else {
							set_position( 1, FD_DBVAL_PN );
						}
					}
				}
			}
			else {

				if( FukugoLine(0) ) {
					set_position( ln, FD_DSYOG_PN );
				}
				else if( FukugoLine(1) ) {
					set_position( ln, FD_CSYOG_PN );
				}
				else {
					if( IsKind() == KIND_NYUKIN ) {
						if( set_position( ++ln, FD_CRED_PN ) == -1 ) {
							if( DnpLine_Chg(1,1) ) {
								set_position( (ln-1), FD_CRED_PN );
							}
							else {
								set_position( 1, FD_CRED_PN );
							}
						}
					}
					else if( IsKind() == KIND_SYUKKIN ) {
						if( set_position( ++ln, FD_DEBT_PN ) == -1 ) {
							if( DnpLine_Chg(1,1) ) {
								set_position( (ln-1), FD_DEBT_PN );
							}
							else {
								set_position( 1, FD_DEBT_PN );
							}
						}
					}
					else {
						if( set_position( ++ln, FD_DBVAL_PN ) == -1 ) {
							if( DnpLine_Chg(1,1) ) {
								set_position( (ln-1), FD_DBVAL_PN );
							}
							else {
								set_position( 1, FD_DBVAL_PN );
							}
						}
					}
				}
			}
		}
		break;
	case VK_LEFT:
	case VK_F2:
		if( IsKind() == KIND_NYUKIN )		set_position( ln, FD_CRED_PN );
		else								set_position( ln, FD_DEBT_PN );

		break;
	case VK_DELETE:
		break;
	default:
		// ここには、イメージ摘要表示時のキー操作のみがくる
		// 文字入力時は、DBEDITのターミネーション処理
		if( nChar ) {
			//適要選択欄ページめくり
			nChar = SelTkyDsp( nChar );
			if( !nChar )
				return;
		}

		if( nChar )	Buzzer();
		break;
	}
}


//
//	int ln ... 画面伝票上の　行番号(１～最大７)
//
int CDnpBaseClass::AutoSiwakeCheck(int ln)
{
	if( IsMode() != MODE_AUTO )	return	0;

	int dt_ln = get_dataline( ln );
	int ret = 0;

	// 摘要欄の入力チェック
	if( get_nowpn() == FD_TKY_PN ) {

		VARIANT var;
		CWnd* pwnd;
		pwnd = GetNowDnpDialog();

	//	CICSDBEDT* pDedt;
	//	pDedt = (CICSDBEDT*)(GetNowDnpDialog()->GetDlgItem( IDC_ICSDBEDT8CTRL1 ));

		DBdata_get( pwnd, IDC_ICSDBEDT8CTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

		int linelength = 0;
		if( var.pbVal != NULL )	{
			linelength = strlen( (const char*)var.pbVal );
		}
		if( linelength == 0 ) {
			ret = 1;
		}
	}
	else  {
		if( ! _getDATA( dt_ln, FD_TKY_PN )->DP_TKY[0] ) {
			ret = 1;
		}
	}

	return ret;
}



// 工事番号を仕訳摘要へ
void CDnpBaseClass::bmnkoji_to_tekiyo( CString* kjnam, int type/* = 0*/ )
{
	if( type ) {
		if( m_pAsel->KJTOTKY_OPT != OPT_KJTOTKY )
			return;
	}
	else {
		if( m_pAsel->BMNTOTKY_OPT != OPT_CHKON )
			return;
	}

	if( IsKind() != KIND_FURIKAE )
		return;
	else {
		// 単一部門入力のときは、摘要をセットしない
		if( m_FuriMode !=  FURI_DATABMON )
			return;
	}

	BOOL bImg = FALSE;
	int ln		= get_nowln();
	int dt_ln	= get_dataline( ln );

	if( IsJob() == JOB_SCAN ) {
		if( tblImage[dt_ln-1][0] || tblImage[dt_ln-1][1] )
			bImg = TRUE;
	}

	if( ! bImg ) {

		CString tmp;
		tmp = _getDATA( dt_ln, FD_TKY_PN )->DP_TKY;

		// すでに同じ 文字列が摘要に含まれていないかをチェック
		if( IsIncludeString( tmp, *kjnam ) )
			return;

		char buf[128] = {0};
		strcpy_s( buf, sizeof buf, (const char*)_getDATA( dt_ln, FD_TKY_PN )->DP_TKY );
		strcat_s( buf, sizeof buf, *kjnam );

		int n = get_strcnt( buf, (m_pZm->zvol->tk_ln * 2) );
		buf[n] = '\0';

		::ZeroMemory( _getDATA( dt_ln, FD_TKY_PN ), sizeof (struct _DenpData) );
		memcpy( _getDATA( dt_ln, FD_TKY_PN )->DP_TKY, buf, n );
		_getCND( dt_ln, FD_TKY_PN )->INP_sg = TRUE;

		GetNowDnpDialog()->SetTekiyo( ln, FD_TKY_PN, (char*)_getDATA( dt_ln, FD_TKY_PN )->DP_TKY );
	}
}


//転送済み仕訳があるか？
BOOL CDnpBaseClass::IsTransDenpyo()
{
	BOOL bRet = FALSE;

	if( IsJob() == JOB_SCAN ) {
		int	pn;
		if( IsKind() == KIND_NYUKIN )		pn	=	FD_CRED_PN;
		else if( IsKind() == KIND_SYUKKIN )	pn	=	FD_DEBT_PN;
		else								pn	=	FD_DBVAL_PN;

		for( int ln = 1; ln <= get_MaxLine(); ln++ ) {
			if( _getCND( ln, pn )->EDT_sg > 1 ) {
				bRet = TRUE;
				break;
			}
		}
	}

	return bRet;
}



//
//	伝票番号
//
//
// 伝票番号　入力＆オフフォーカス
long CDnpBaseClass::DenpInp(int pn, long nChar, int denp) 
{

	// オフフォーカス
	if( !nChar )
	{
		// データSET
		DenpSet( pn, denp );

		return( 0 );
	}
	else
	{
		if( nChar == VK_TAB && is_SHIFT() )
			return 0;
	}

	// 入力
	if( nChar == VK_RETURN || nChar == VK_TAB ) 
	{
		;
	}
	else if( nChar == VK_DELETE )
	{
		// 取消
		DenpDel( pn );
	}

	return( nChar );
}

// 伝票番号　データSET
void CDnpBaseClass::DenpSet( int pn, int denp )
{
	struct _DenpData *keybuf = getDATA( pn );

	//データリード
	if( pn < 0 )	return;
//	Data_Get( getINP_CTL( pn )->pIDC->IDC_X, &check );

	if( denp != -1 )
	{
		getCND( pn )->INP_sg = TRUE;

		keybuf->DP_DENP = denp;
	}
	else {
		if( keybuf->DP_DENP == -1 )
			getCND( pn )->INP_sg = FALSE;
		else
			denp = keybuf->DP_DENP;
	}

	//データセット
	GetNowDnpDialog()->SetDnpyoNo( pn, denp );

//	Data_Set( getINP_CTL( pn )->pIDC->IDC_X, keybuf );
}

// 伝票番号　ＤＥＬＥＴＥ
void CDnpBaseClass::DenpDel( int pn )
{
	struct _DenpData check, *keybuf = getDATA( pn );

	check.DP_DENP = -1;
	if( pn < 0 )	return;

	GetNowDnpDialog()->SetDnpyoNo( pn,-1);
//	Data_Set( getINP_CTL( pn )->pIDC->IDC_X, &check );

	keybuf->DP_DENP = -1;

	getCND( pn )->INP_sg = FALSE;
}



//------------------------------------------------------------------
//	手形期日を入力
//	int ln		画面伝票上の　行番号(１～最大７)
//------------------------------------------------------------------
void CDnpBaseClass::TgDateInp( int ln, int pn, long nChar, long inplen, char* tgdate )
{
	struct _DenpData* keybuf;
	int dt_ln = get_dataline( ln );

	keybuf = _getDATA( dt_ln, pn );
	int ymd = 0;

	if( ! nChar || inplen ) {
		memcpy( (char*)keybuf->DP_BCDDAY, tgdate, 3 );
		if( tgdate[0] ) {
#ifdef CLOSE
			// 平成から西暦へ
			char bf[64];
			int tmp;
			sprintf_s( bf, sizeof bf, "%02x%02x%02x", tgdate[0], tgdate[1], tgdate[2] );
			tmp = atoi( bf );
			db_datecnv( tmp, &ymd, 1, 0 );
#endif

			ymd = TgDateBcdToSeireki( (BYTE*)tgdate, NULL );

			keybuf->DP_TGDAY = ymd;
		}
		else
			keybuf->DP_TGDAY = -1;
	}

	if( nChar == VK_DELETE ) {
		::ZeroMemory( keybuf->DP_TGBCD, sizeof keybuf->DP_TGBCD );
		keybuf->DP_TGDAY = -1;
		GetNowDnpDialog()->SetTgNumber( ln, pn, NULL );
	}

	if( keybuf->DP_TGDAY > 0 )	_getCND( dt_ln, pn )->INP_sg = TRUE;
	else						_getCND( dt_ln, pn )->INP_sg = FALSE;

}



//--------------------------------------------------------
//	税額入力＆オフフォーカス
//　int ln			画面伝票上の　行番号(１～最大７)
//	long nChar		キーコード
//	long inplen		金額が入力されてたばあい１
//	char* val		金額データ
//--------------------------------------------------------
long CDnpBaseClass::ZeiInp(int ln, int pn, long nChar, long inplen, char* val ) 
{
	l_defn( 0x16 );

	// オフフォーカス
	if( !nChar )
	{
		// データセット
		ZeiSet( ln, pn, inplen, val );

		return(0);
	}
	else
	{
		if (nChar == VK_TAB && is_SHIFT()) {
			return 0;
		}
	}

	// 入力
	switch (nChar) {
	case VK_RETURN:
	case VK_TAB:  case VK_F2:
	case VK_DOWN: case VK_UP: case VK_LEFT: case VK_RIGHT:
		ZeiSet(ln, pn, inplen, val);
		break;
	case VK_DELETE:
		// データＤＥＬＥＴＥ
		nChar = ZeiDel(ln, pn);
		break;
	}

	return( nChar );
}

// 税額　データセット
//　int ln			画面伝票上の　行番号(１～最大７)
//
void CDnpBaseClass::ZeiSet(int ln, int pn, long inplen, char* val ) 
{
	int dt_ln = get_dataline( ln );

	if( pn < 0 )	return;
	struct _DenpData *keybuf, prevval;
	int	pos, cnt;

	switch( IsKind() ) {
		case	KIND_NYUKIN:
			pos		=	FD_CRZEI_PN;
			break;
		case	KIND_SYUKKIN:
			pos		=	FD_DBZEI_PN;
			break;
		default:
			pos	=	pn;
			break;
	}
	keybuf	=	_getDATA( dt_ln, pos );
	prevval	=	*_getDATA( dt_ln, pos );

	if( inplen > 0 ) {
		memmove( keybuf->DP_VAL, val, sizeof( keybuf->DP_VAL) );
		_getCND( dt_ln, pos )->INP_sg = TRUE;
		_getCND( dt_ln, pos )->SET_sg = TRUE;	// 税額手入力セット ON
	}

	//データセット
	if( _getCND( dt_ln, pos )->INP_sg == TRUE ) {
		GetNowDnpDialog()->SetVal( ln, pn, (char*)keybuf->DP_VAL );
	}
	else {
		GetNowDnpDialog()->SetVal( ln, pn, NULL );
	}

	// 貸借表示
	total_dsp();

	if( (cnt = mkSY_CREC( m_CREC, ln )) > 0 ) {	// 消費税データ作成
		int dc_sw, idx = 0;
		dc_sw = (pn == FD_DBZEI_PN) ? 0 : 1;
		if( cnt == 2 && dc_sw )	idx = 1;
		setSY_CREC( &m_CREC[idx], dc_sw, ln );	// 消費税サインをデータテーブルに

		SetTandokuSameValue(ln, pn);
	}
}

// 金額　データＤＥＬＥＴＥ
//　int ln			画面伝票上の　行番号(１～最大７)
//
long CDnpBaseClass::ZeiDel(int ln, int pn ) 
{
	if( pn < 0 )	return	-1;

	if( m_pAsel->ZEIMDFY_OPT != OPT_ZEIMDFY )
		return 0;

	int dt_ln = get_dataline( ln );

	struct _DenpData *keybuf, prvval;
	int	pos, vpos, vpn;

	switch( IsKind() ) {
		case	KIND_NYUKIN:
			pos	=	FD_CRZEI_PN;	break;
		case	KIND_SYUKKIN:
			pos	=	FD_DBZEI_PN;	break;
		default:
			pos	=	pn;
			if( pn == FD_DBZEI_PN ) {
				vpn		=	FD_CRZEI_PN;
				vpos	=	FD_CRZEI_PN;
			}
			else {
				vpn		=	FD_DBZEI_PN;
				vpos	=	FD_DBZEI_PN;
			}
			break;
	}

	if( _getCND( dt_ln, pos )->EDT_sg > 1 )
		return 0;

	keybuf	=	_getDATA( dt_ln, pos );
	prvval	=	*_getDATA( dt_ln, pos );

	GetNowDnpDialog()->SetVal( ln, pn, NULL );
	memset( (char *)keybuf->DP_VAL, (char)0, sizeof(keybuf->DP_VAL) );
	_getCND( dt_ln, pos )->INP_sg = FALSE;
	_getCND( dt_ln, pos )->SET_sg = TRUE;	// 税額手入力セット ON ( setSY_CREC で使用 )

	// 貸借表示
	total_dsp();

	return	VK_DELETE;
}




//--------------------------------------------------------
//	金額入力＆オフフォーカス
//	int ln			画面伝票上の　行番号(１～最大７)
//	long nChar		キーコード
//	long inplen		金額が入力されてたばあい１
//	char* val		金額データ
//--------------------------------------------------------
long CDnpBaseClass::ValInp(int ln, int pn, long nChar, long inplen, char* val ) 
{
	l_defn( 0x16 );

	// オフフォーカス
	if( !nChar )
	{
		// データセット
		ValSet( ln, pn, inplen, val );

		return(0);
	}
	else
	{
		if (nChar == VK_TAB && is_SHIFT()) {
			return 0;
		}
	}

	// 入力
	switch( nChar ) {
	case VK_RETURN:
	case VK_TAB:  case VK_F2:
	case VK_DOWN: case VK_UP: case VK_LEFT: case VK_RIGHT:
		ValSet( ln, pn, inplen, val );
		if( nChar == VK_RETURN )
			ValCopy( ln, pn );
		break;
	case VK_DELETE:
		// データＤＥＬＥＴＥ
		nChar	=	ValDel( ln, pn );
		break;
	}

	return( nChar );
}

// 金額　データセット
//	int ln			画面伝票上の　行番号(１～最大７)
//
void CDnpBaseClass::ValSet(int ln, int pn, long inplen, char* val ) 
{
	if( pn < 0 )	return;
	struct _DenpData *keybuf, prevval;
	int	pos, vpos, vpn, cnt, syzpn, v_syz, zeipn, v_zei;

//TRACE( "ValSet TOP ln = %d, pn = %d, inplen = %d, m_bDLineChanging = %d\n", ln, pn, inplen, m_bDLineChanging );
	if( m_bDLineChanging )	return;

	int dt_ln;
	dt_ln = get_dataline( ln );

	switch( IsKind() ) {
		case	KIND_NYUKIN:
			pos		=	FD_CRVAL_PN;
			zeipn	=	FD_CRZEI_PN;
			vpos	=	FD_DBVAL_PN;
			break;
		case	KIND_SYUKKIN:
			pos		=	FD_DBVAL_PN;
			zeipn	=	FD_DBZEI_PN;
			vpos	=	FD_CRVAL_PN;
			break;
		default:
			pos	=	pn;
			if( pn == FD_DBVAL_PN ) {
				vpos	=	FD_CRVAL_PN;
				vpn		=	FD_CRVAL_PN;
				syzpn	=	FD_DSKBN_PN;
				v_syz	=	FD_CSKBN_PN;
				zeipn	=	FD_DBZEI_PN;
				v_zei	=	FD_CRZEI_PN;
			}
			else {
				vpos	=	FD_DBVAL_PN;
				vpn		=	FD_DBVAL_PN;
				syzpn	=	FD_CSKBN_PN;
				v_syz	=	FD_DSKBN_PN;
				zeipn	=	FD_CRZEI_PN;
				v_zei	=	FD_DBZEI_PN;
			}
			break;
	}
	keybuf	=	_getDATA( dt_ln, pos );
	prevval	=	*_getDATA( dt_ln, pos );

	// 金額が入力されたら、税額の手入力サインをOFF
	if( inplen > 0 ) {
		_getCND( dt_ln, zeipn )->SET_sg = FALSE;
		_getCND(dt_ln, zeipn)->ETC_sg[3] = FALSE;
	}

	//データリード
	if( _getCND( dt_ln, pos )->SET_sg || inplen ) {

		char edt_sg;
		IsTransLine( ln, edt_sg );

		//	_FIX_04_OCT_02_
		if( IsJob() == JOB_SCAN ) {
			switch( IsKind() ) {
				case	KIND_NYUKIN:
				case	KIND_SYUKKIN:
						if( inplen > 0 && l_test( val ) == 0 ) {
							GetNowDnpDialog()->SetVal( ln, pn, (char*)prevval.DP_VAL );
							return;
						}

						if( inplen > 0 && edt_sg > 1 ) {
							GetNowDnpDialog()->SetVal( ln, pn, (char*)prevval.DP_VAL );
							return;
						}

					break;
				default:
						if( inplen > 0 && l_test( val ) == 0 ) {
							GetNowDnpDialog()->SetVal( ln, pn, (char*)prevval.DP_VAL );
							return;
//							if( !m_DPscan.IsFukugoSiwake( ln ) )	return	0;
						}
						if( inplen > 0 && edt_sg > 1 ) {
							GetNowDnpDialog()->SetVal( ln, pn, (char*)prevval.DP_VAL );
							return;
						}
					break;
			}
		}
		memmove( keybuf->DP_VAL, val, sizeof( keybuf->DP_VAL) );
		_getCND( dt_ln, pos )->INP_sg = TRUE;
		_getCND( dt_ln, pos )->SET_sg = FALSE;	// データセットサインＯＦＦ
	}

	//データセット
	if( _getCND( dt_ln, pos )->INP_sg == TRUE ) {
		GetNowDnpDialog()->SetVal( ln, pn, (char*)keybuf->DP_VAL );
	}
	else {
		GetNowDnpDialog()->SetVal( ln, pn, NULL );
	}

	//振替伝票、単独仕訳入力、もう一方の金額未入力の時と、
	//振替伝票、単独仕訳入力、借方金額と貸方金額が同金額の時は、
	//借方金額と同一金額を貸方金額にセット。
	BOOL bCheck		= FALSE;
	BOOL bSetFlg	= FALSE;
	BOOL bVsoto		= FALSE;
	BOOL sotozei	= FALSE;
	BOOL bBothSyz	= FALSE;
	char chk[6] = {0}, v_chk[6] = {0}, newval[6];
	memcpy( newval, keybuf->DP_VAL, 6 );

	int dc_sw = -1;
	if( FuriSyogPosition( dt_ln, FD_DEBT_PN ) )
		dc_sw = 0;
	else if( FuriSyogPosition( dt_ln, FD_CRED_PN ) )
		dc_sw = 1;

	if( IsKind() == KIND_FURIKAE && IsType() == TYPE_TANDOKU ) {
		if( inplen > 0 && _getCND( dt_ln, pn )->INP_sg != FALSE ) {
			// 借方／貸方消費税が同じ
			if( _getCND( dt_ln, syzpn )->INP_sg &&
				_getCND( dt_ln, v_syz )->INP_sg ) {

				if( (_getDATA( dt_ln, syzpn )->SYDATA._DSGN[0] & 0x40) &&
					(_getDATA( dt_ln, v_syz )->SYDATA._DSGN[0] & 0x40) ) {
					
					// どちらもおなじ消費税か？
					if( memcmp( _getDATA( dt_ln, syzpn )->SYDATA._DSGN,
								_getDATA( dt_ln, v_syz )->SYDATA._DSGN,
								SZ_SYZDSGN ) == 0 ) {
						bCheck = TRUE;
						bBothSyz = TRUE;

						memcpy( chk, keybuf->DP_VAL, 6 );
						memcpy( v_chk, _getDATA( get_nowln(), vpos )->DP_VAL, 6 );

						if( _sotozei_data( _getDATA( dt_ln, syzpn )->SYDATA._DSGN ) ) {
							l_add( chk, chk, _getDATA( dt_ln, zeipn )->DP_VAL );
							l_add( v_chk, v_chk, _getDATA( dt_ln, v_zei )->DP_VAL );
						}
					}
				}
			}
			else {
				bCheck = TRUE;

				memcpy( chk, prevval.DP_VAL, 6 );
				memcpy( v_chk, _getDATA( dt_ln, vpos )->DP_VAL, 6 );

				if( _getCND( dt_ln, syzpn )->INP_sg ) {
					if( _sotozei_data( _getDATA( dt_ln, syzpn )->SYDATA._DSGN ) ) {
						sotozei = TRUE;
						l_add( chk, chk, _getDATA( dt_ln, zeipn )->DP_VAL );
					}
				}
				// 反対側
				if( _getCND( dt_ln, v_syz )->INP_sg ) {
					if( _sotozei_data( _getDATA( dt_ln, v_syz )->SYDATA._DSGN ) ) {
						bVsoto = TRUE;
						l_add( v_chk, v_chk, _getDATA( dt_ln, v_zei )->DP_VAL );
					}
				}
			}

			// 単一仕訳のときは、借方／貸方 の金額を 必ずそろえるようにする
			if( dc_sw == -1 ) {
				if( _getCND( dt_ln, vpn )->INP_sg == FALSE || ( bCheck /*&& l_cmp( chk, v_chk ) == 0*/ ) ) {
					bSetFlg = TRUE;
					// 金額をセット
					keybuf	=	_getDATA( dt_ln, vpos );

					memmove( keybuf->DP_VAL, newval, sizeof( keybuf->DP_VAL) );
					_getCND( dt_ln, vpos )->INP_sg = TRUE;
					_getCND( dt_ln, vpos )->SET_sg = FALSE;	// データセットサインＯＦＦ

					GetNowDnpDialog()->SetVal( get_nowln(), vpn, (char*)keybuf->DP_VAL );
				}
			}
		}
	}

	if( (cnt = mkSY_CREC( m_CREC, ln )) > 0 ) {	// 消費税データ作成
		int dbt_cre, idx = 0;
		dbt_cre = (pn == FD_DBVAL_PN) ? 0 : 1;
		if( cnt == 2 && dbt_cre )	idx = 1;

		setSY_CREC( &m_CREC[idx], dbt_cre, ln );	// 消費税サインをデータテーブルに

		// 単一仕訳の場合は、相手側の消費税もセットする
		if( IsType() == TYPE_TANDOKU && bSetFlg && dc_sw == -1 ) {
			if( ! _getCND( get_nowln(), syzpn )->INP_sg ) {
				_getCND( dt_ln, v_zei )->SET_sg = FALSE;
				if( cnt == 2 ) {
					if( dbt_cre ) {
						setSY_CREC( &m_CREC[0], 0, ln );
					}
					else {
						setSY_CREC( &m_CREC[1], 1, ln );
					}
				}
				else {
					setSY_CREC( &m_CREC[0], dbt_cre ? 0 : 1, ln );
				}
			}
		}
	}

	if( bSetFlg ) {
		// 再セット
		// 相手方が外税の場合、税額を＋して金額をセット
		if( bVsoto ) {
			keybuf	=	_getDATA( dt_ln, vpos );
			memcpy( chk, keybuf->DP_VAL, 6 );
			l_add( chk, chk, _getDATA( dt_ln, v_zei )->DP_VAL );

			keybuf	=	_getDATA( dt_ln, pos );
			memmove( keybuf->DP_VAL, chk, sizeof( keybuf->DP_VAL) );
			_getCND( dt_ln, pos )->INP_sg = TRUE;
			_getCND( dt_ln, pos )->SET_sg = FALSE;	// データセットサインＯＦＦ

			GetNowDnpDialog()->SetVal( get_nowln(), pn, (char*)keybuf->DP_VAL );
		}
		else if( sotozei ) {
			// 相手方に、外税分をプラスする
			keybuf	=	_getDATA( dt_ln, pos );
			memcpy( chk, keybuf->DP_VAL, 6 );
			l_add( chk, chk, _getDATA( dt_ln, zeipn )->DP_VAL );

			keybuf	=	_getDATA( dt_ln, vpos );
			memmove( keybuf->DP_VAL, chk, sizeof( keybuf->DP_VAL) );
			_getCND( dt_ln, vpos )->INP_sg = TRUE;
			_getCND( dt_ln, vpos )->SET_sg = FALSE;	// データセットサインＯＦＦ

			GetNowDnpDialog()->SetVal( get_nowln(), vpn, (char*)keybuf->DP_VAL );
		}

		// どちらも消費税科目の場合
		if( bBothSyz ) {
			keybuf	=	_getDATA( dt_ln, zeipn );
			memcpy( chk, keybuf->DP_VAL, 6 );

			keybuf	=	_getDATA( dt_ln, v_zei );
			memmove( keybuf->DP_VAL, chk, sizeof( keybuf->DP_VAL) );
			_getCND( dt_ln, v_zei )->INP_sg = TRUE;
			_getCND( dt_ln, v_zei )->SET_sg = FALSE;

			GetNowDnpDialog()->SetVal( get_nowln(), v_zei, (char*)keybuf->DP_VAL );
		}
	}

	// 貸借表示
	total_dsp();

}

// 金額　データＤＥＬＥＴＥ
//	int ln			画面伝票上の　行番号(１～最大７)
//
long CDnpBaseClass::ValDel(int ln, int pn ) 
{
	if( pn < 0 )	return	-1;

	int	kpn, vpos, vpn, syzpn, v_syz, zeipn, v_zei;
	int bmn_pn, koji_pn;
	long nChar;
	int dt_ln = get_dataline( ln );

	if( IsJob() == JOB_SCAN ) {
		switch( IsKind() ) {
			case	KIND_NYUKIN:
			case	KIND_SYUKKIN:
				if( IsScanLine( dt_ln ) ) {
					return	0;
				}
//	_FIX_04_OCT_02_
				if( IsKind() == KIND_SYUKKIN )	kpn	=	FD_DEBT_PN;
				else							kpn	=	FD_CRED_PN;
				ItemDel( ln, kpn );
				break;
			default:
				if( IsScanLine( dt_ln ) ) {
					if( ! SyogSiwakeMode() ) {	//諸口仕訳詰める。
						// 単一仕訳は、F4での取消 のみ
						if( ! m_DPscan.IsFukugoSiwake( dt_ln ) ) {
							return	0;
						}
						else {
							// 確定仕訳
							if( _getCND( dt_ln, pn )->EDT_sg > 1 )
								return 0;
						}
					}
					else {
						if( FuriSyogPosition( dt_ln, pn ) )	return 0;
						else {
							if( m_DPscan.IsTanitsuSiwake( dt_ln ) )	return 0;
							// 確定仕訳
							if( _getCND( dt_ln, pn )->EDT_sg > 1 )
								return 0;
						}
					}
				}
//	_FIX_04_OCT_02_
				if( pn == FD_DBVAL_PN ) {
					kpn		=	FD_DEBT_PN;
					bmn_pn	=	FD_DBMN_PN;
					koji_pn	=	FD_DKOJI_PN;
				}
				else {
					kpn		=	FD_CRED_PN;
					bmn_pn	=	FD_CBMN_PN;
					koji_pn	=	FD_CKOJI_PN;
				}
				// 複合仕訳は、金額削除で、科目・部門･工事も削除
				nChar	=	ItemDel( ln, kpn );
				if( nChar != 0 && nChar != VK_RETURN ) {
					SyzDel( ln, (kpn==FD_DEBT_PN) ? FD_DSKBN_PN : FD_CSKBN_PN ); // 消費税も削除
				}

				if( m_DI.IsBumon() ) {
					int bmn = -1;
					Bmn_Inp( bmn_pn, VK_DELETE, &bmn );
				}
				if( m_DI.IsKoji() ) {
					CString kno;
					::ZeroMemory( _getDATA( dt_ln, koji_pn )->DP_KOJI, sizeof _getDATA( dt_ln, koji_pn )->DP_KOJI );
					_getCND(dt_ln, koji_pn)->INP_sg = FALSE;
					kno.Empty();
					GetNowDnpDialog()->SetKoji( ln, koji_pn, kno, NULL );
				}

				break;
		}
	}

	struct _DenpData *keybuf, *zeibuf, prvval, prvzei;
	int	pos;

	switch( IsKind() ) {
		case	KIND_NYUKIN:
			pos	=	FD_CRVAL_PN;	
			zeipn = FD_CRZEI_PN;
			break;
		case	KIND_SYUKKIN:
			pos	=	FD_DBVAL_PN;
			zeipn = FD_DBZEI_PN;
			break;
		default:
			pos	=	pn;
			if( pn == FD_DBVAL_PN ) {
				vpos	=	FD_CRVAL_PN;
				vpn		=	FD_CRVAL_PN;
				syzpn	=	FD_DSKBN_PN;
				v_syz	=	FD_CSKBN_PN;
				zeipn	=	FD_DBZEI_PN;
				v_zei	=	FD_CRZEI_PN;
			}
			else {
				vpos	=	FD_DBVAL_PN;
				vpn		=	FD_DBVAL_PN;
				syzpn	=	FD_CSKBN_PN;
				v_syz	=	FD_DSKBN_PN;
				zeipn	=	FD_CRZEI_PN;
				v_zei	=	FD_DBZEI_PN;
			}
			break;
	}
	keybuf	=	_getDATA( dt_ln, pos );
	zeibuf	=	_getDATA( dt_ln, zeipn );
	prvval	=	*_getDATA( dt_ln, pos );
	prvzei	=	*_getDATA( dt_ln, zeipn );

	// 金額クリア
	GetNowDnpDialog()->SetVal( ln, pn, NULL );
	memset( (char *)keybuf->DP_VAL, (char)0, sizeof(keybuf->DP_VAL) );
	_getCND( dt_ln, pos )->INP_sg = FALSE;
	// 税額クリア
	GetNowDnpDialog()->SetVal( ln, zeipn, NULL );
	memset( (char *)zeibuf->DP_VAL, (char)0, sizeof(zeibuf->DP_VAL) );
	_getCND( dt_ln, zeipn )->INP_sg = FALSE;

	//振替伝票、単独仕訳入力、借方金額と貸方金額が同金額の時は、もう一方もクリア
	if( IsKind() == KIND_FURIKAE && IsType() == TYPE_TANDOKU ) {
		if( _getCND( dt_ln, vpn )->INP_sg != FALSE ) {
			// 外税の場合は、税額を プラス
			char chk[6] = {0}, v_chk[6] = {0};

			memcpy( chk, prvval.DP_VAL, 6 );
			if( _getCND( dt_ln, syzpn )->INP_sg ) {
				if( _sotozei_data( _getDATA( dt_ln, syzpn )->SYDATA._DSGN ) )
					l_add( chk, chk, prvzei.DP_VAL );
			}

			memcpy( v_chk, _getDATA( dt_ln, vpn )->DP_VAL, 6 );
			if( _getCND( dt_ln, v_syz )->INP_sg ) {
				if( _sotozei_data( _getDATA( dt_ln, v_syz )->SYDATA._DSGN ) )
					l_add( v_chk, v_chk, _getDATA( dt_ln, v_zei )->DP_VAL );
			}
			// 反対側クリア
			if( l_cmp( chk, v_chk ) == 0 ) {
				// 金額
				keybuf	=	_getDATA( dt_ln, vpos );
				GetNowDnpDialog()->SetVal( ln, vpn, NULL );
				memset( (char *)keybuf->DP_VAL, (char)0, sizeof(keybuf->DP_VAL) );
				_getCND( dt_ln, vpos )->INP_sg = FALSE;
				// 税額
				zeibuf	=	_getDATA( dt_ln, v_zei );
				GetNowDnpDialog()->SetVal( ln, v_zei, NULL );
				memset( (char *)zeibuf->DP_VAL, (char)0, sizeof(zeibuf->DP_VAL) );
				_getCND( dt_ln, v_zei )->INP_sg = FALSE;
			}
		}
	}
	// 貸借表示
	total_dsp();

	return	VK_DELETE;
}

// 金額　データＣＯＰＹ
//	int ln			画面伝票上の　行番号(１～最大７)
//
void CDnpBaseClass::ValCopy( int ln, int pn ) 
{
	if( pn < 0 )	return;
	int	pos, vpn = -1;

	if( m_DI.IsConFirmMaster() )	return;

	switch( IsKind() ) {
		case	KIND_NYUKIN:
			pos	=	FD_CRVAL_PN;	break;
		case	KIND_SYUKKIN:
			pos	=	FD_DBVAL_PN;	break;
		default:
			if( pn == FD_DBVAL_PN )	vpn = FD_CRVAL_PN;
			else					vpn = FD_DBVAL_PN;
			pos	=	pn;	
			break;
	}
struct _DenpData *prdata, *keybuf;
int	dt_ln = get_dataline( ln );

	// 伝票修正 は コピーしない [From管理課] <01.31 /07>
	BOOL bCopy = FALSE;
	if( ! ValEnterCopyMode() ) { // [09.08/08]
#ifdef CLOSE // 01.21 /11
		if( IsJob() == JOB_INPUT && !_getCND( dt_ln, pos )->INP_sg )
#else
		if( ! _getCND( dt_ln, pos )->INP_sg )
#endif
			bCopy = TRUE;
	}

	// 諸口詰めないで表示するとき
	if( FuriSyogPosition( dt_ln, pos ) )	return;

	if( bCopy && (prdata = _getPRDATA( dt_ln, pos )) ) {
		// データセット
		GetNowDnpDialog()->SetVal( ln, pn, (char*)prdata->DP_VAL );
		BOOL old_setsg = _getCND(dt_ln, pos)->SET_sg;
		_getCND( dt_ln, pos )->SET_sg = TRUE;	// データセットサインＯＮ

		if( IsKind() == KIND_FURIKAE && IsType() == TYPE_TANDOKU ) {
			if( _getCND( dt_ln, vpn )->INP_sg == FALSE ||
				l_cmp( prdata->DP_VAL, _getDATA( dt_ln, vpn )->DP_VAL ) == 0 ) {

				keybuf	=	_getDATA( dt_ln, vpn );

				memmove( keybuf->DP_VAL, prdata->DP_VAL, sizeof( keybuf->DP_VAL) );
				_getCND( dt_ln, vpn )->INP_sg = TRUE;
				_getCND( dt_ln, vpn )->SET_sg = FALSE;	// データセットサインＯＦＦ

				GetNowDnpDialog()->SetVal( ln, vpn, (char*)keybuf->DP_VAL );
			}
		}
		//ValSet呼び出し
		if( !old_setsg ) {
			ValSet(ln, pn, 0, (char*)prdata->DP_VAL);
		}
	}
}


//-------------------------------------------------------------
//	振替伝票、単独仕訳入力、借方／貸方 金額が同じかどうか？
//	int ln			画面伝票上の　行番号(１～最大７)
//
//-------------------------------------------------------------
BOOL CDnpBaseClass::IsSameValue( int ln, int pn ) 
{
	if( IsKind() != KIND_FURIKAE )
		return FALSE;
	
	if( IsType() != TYPE_TANDOKU )
		return FALSE;

	int val_pn, val_op, zei_pn, zei_op, syz_pn, syz_op;
	int dt_ln = get_dataline( ln );

	if( IsDebtPosition( pn ) ) {
		val_pn = FD_DBVAL_PN;
		zei_pn = FD_DBZEI_PN;
		syz_pn = FD_DSKBN_PN;

		val_op = FD_CRVAL_PN;	// 逆側の項目
		zei_op = FD_CRZEI_PN;
		syz_op = FD_CSKBN_PN;
	}
	else if( IsCredPosition( pn ) ) {
		val_pn = FD_CRVAL_PN;
		zei_pn = FD_CRZEI_PN;
		syz_pn = FD_CSKBN_PN;

		val_op = FD_DBVAL_PN;	// 逆側の項目
		zei_op = FD_DBZEI_PN;
		syz_op = FD_DSKBN_PN;
	}
	else	return FALSE;

	// 金額未入力
	if( ! _getCND( dt_ln, val_pn )->INP_sg || ! _getCND( dt_ln, val_op )->INP_sg )
		return FALSE;

	struct _DenpData *keybuf, *other;
	keybuf = other = NULL;
	char chk[6] = {0}, v_chk[6] = {0};

	keybuf	= _getDATA( dt_ln, val_pn );
	other	= _getDATA( dt_ln, val_op );

	if( keybuf == NULL || other == NULL )
		return FALSE;

	// 借方／貸方消費税が同じ
	if( _getCND( dt_ln, syz_pn )->INP_sg &&
		_getCND( dt_ln, syz_op )->INP_sg ) {

		if( (_getDATA( dt_ln, syz_pn )->SYDATA._DSGN[0] & 0x40) &&
			(_getDATA( dt_ln, syz_op )->SYDATA._DSGN[0] & 0x40) ) {

			if( memcmp( _getDATA( dt_ln, syz_pn )->SYDATA._DSGN,
						_getDATA( dt_ln, syz_op )->SYDATA._DSGN,
						SZ_DSGN ) == 0 ) {

				memcpy( chk, keybuf->DP_VAL, 6 );
				memcpy( v_chk, other->DP_VAL, 6 );
				// 外税仕訳は税額を＋
				if( _sotozei_data( _getDATA( dt_ln, syz_pn )->SYDATA._DSGN ) ) {
					l_add( chk, chk, _getDATA( dt_ln, zei_pn )->DP_VAL );
					l_add( v_chk, v_chk, _getDATA( dt_ln, zei_op )->DP_VAL );
				}
			}
		}
	}
	else {
		// 片方が、消費税か、どちらも消費税科目でない
		memcpy( chk, keybuf->DP_VAL, 6 );
		memcpy( v_chk, other->DP_VAL, 6 );

		if( _getCND( dt_ln, syz_pn )->INP_sg ) {
			if( _sotozei_data( _getDATA( dt_ln, syz_pn )->SYDATA._DSGN ) ) {
				l_add( chk, chk, _getDATA( dt_ln, zei_pn )->DP_VAL );
			}
		}
		// 反対側
		if( _getCND( dt_ln, syz_op )->INP_sg ) {
			if( _sotozei_data( _getDATA( dt_ln, syz_op )->SYDATA._DSGN ) ) {
				l_add( v_chk, v_chk, _getDATA( dt_ln, zei_op )->DP_VAL );
			}
		}
	}

	if( l_cmp( chk, v_chk ) == 0 )
		return TRUE;

	return FALSE;
}


//---------------------------------------------------------
//	科目替わり に呼び出す。
//	単一仕訳のとき 借方／貸方 同一金額をセットする
//	 上の IsSameValue()が TRUE のとき セットするように
//　int ln ... 画面伝票上の　行番号(１～最大７)
//
//---------------------------------------------------------
void CDnpBaseClass::SetTandokuSameValue( int ln, int pn ) 
{
	int val_pn, val_op, zei_pn, zei_op, syz_pn, syz_op;
	int dc_sw = -1;
	int	dt_ln = get_dataline( ln );

	if( IsKind() != KIND_FURIKAE )
		return;

	if( IsType() != TYPE_TANDOKU )
		return;

	if( IsDebtPosition( pn ) )
		dc_sw = 0;
	else if( IsCredPosition( pn ) )
		dc_sw = 1;

	// 借方・貸方の 位置ではない
	if( dc_sw == -1 )	return;

	if( dc_sw == 0 ) {
		val_pn = FD_DBVAL_PN;
		zei_pn = FD_DBZEI_PN;
		syz_pn = FD_DSKBN_PN;

		val_op = FD_CRVAL_PN;	// 逆側の項目
		zei_op = FD_CRZEI_PN;
		syz_op = FD_CSKBN_PN;
	}
	else {
		val_pn = FD_CRVAL_PN;
		zei_pn = FD_CRZEI_PN;
		syz_pn = FD_CSKBN_PN;

		val_op = FD_DBVAL_PN;	// 逆側の項目
		zei_op = FD_DBZEI_PN;
		syz_op = FD_DSKBN_PN;
	}

	// 伝票修正時の諸口仕訳詰めない側の場合
	if( FuriSyogPosition( dt_ln, val_op ) )
		return;

	// 金額未入力
	if( ! _getCND( dt_ln, val_pn )->INP_sg )
		return;

	struct _DenpData *keybuf, *other;
	struct _DenpData *zeibuf, *ot_zei;

	keybuf = other = zeibuf = ot_zei = NULL;
	char val[6] = {0};

	keybuf	= _getDATA( dt_ln, val_pn );
	other	= _getDATA( dt_ln, val_op );

	if( keybuf == NULL || other == NULL )
		return;

	BOOL bSetFlg = FALSE;

	// 借方／貸方消費税が同じ
	if( _getCND( dt_ln, syz_pn )->INP_sg &&
		_getCND( dt_ln, syz_op )->INP_sg ) {

		if( (_getDATA( dt_ln, syz_pn )->SYDATA._DSGN[0] & 0x40) &&
			(_getDATA( dt_ln, syz_op )->SYDATA._DSGN[0] & 0x40) ) {

			if( memcmp( _getDATA( dt_ln, syz_pn )->SYDATA._DSGN,
						_getDATA( dt_ln, syz_op )->SYDATA._DSGN,
						SZ_SYZDSGN ) == 0 ) {

				// どちらも外税の場合は、税額も一緒にセットする
				if( _sotozei_data( _getDATA( dt_ln, syz_op )->SYDATA._DSGN ) ) {

					// 金額をセット
					zeibuf	= _getDATA( dt_ln, zei_pn );
					ot_zei	= _getDATA( dt_ln, zei_op );

					memcpy( other->DP_VAL,  keybuf->DP_VAL, sizeof( keybuf->DP_VAL ) );
					memcpy( ot_zei->DP_VAL, zeibuf->DP_VAL, sizeof( zeibuf->DP_VAL ) );

					_getCND( dt_ln, val_op )->INP_sg = TRUE;
					_getCND( dt_ln, val_op )->SET_sg = FALSE;	// データセットサインＯＦＦ

					GetNowDnpDialog()->SetVal( ln, val_op, (char*)other->DP_VAL );

					_getCND( dt_ln, zei_op )->INP_sg = TRUE;
					_getCND( dt_ln, zei_op )->SET_sg = FALSE;	// データセットサインＯＦＦ

					GetNowDnpDialog()->SetVal( ln, zei_op, (char*)ot_zei->DP_VAL );
				}
			}
			else {
				// 科目 変わりなど、税区分が異なる
				return;
			}
		}
	}
	else { 
		// 片方が、消費税か、どちらも消費税科目でない
		memcpy( val, keybuf->DP_VAL, 6 );

		// 反対側が 外税の場合、何もしない
		if( _getCND( dt_ln, syz_op )->INP_sg ) {
			if( _sotozei_data( _getDATA( dt_ln, syz_op )->SYDATA._DSGN ) ) {
				return;
			}
		}
		else {
			if( _getCND( dt_ln, syz_pn )->INP_sg ) {
				// 今回の変更側が外税
				if( _sotozei_data( _getDATA( dt_ln, syz_pn )->SYDATA._DSGN ) ) {
					l_add( val, val, _getDATA( dt_ln, zei_pn )->DP_VAL );
				}
				memcpy( other->DP_VAL, val, sizeof( val ) );

				_getCND( dt_ln, val_op )->INP_sg = TRUE;
				_getCND( dt_ln, val_op )->SET_sg = FALSE;	// データセットサインＯＦＦ

				GetNowDnpDialog()->SetVal( ln, val_op, (char*)other->DP_VAL );
			}
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////
// 科目入力

// 科目コード入力か？
BOOL check_kmkstring( LPCTSTR string )
{
	int i, max, st;
	max = strlen( string );
	BOOL	bRet = TRUE;

	for( i = 0; i < max; i++ ) {
		st = IsKindOfLetter(string, i);
		if( st != 1 ) {			// ASCII でない
			bRet = FALSE;
			break;
		}
	}

	return bRet;
}

//--------------------------------------------------------
//	科目チェック
//	long nChar ....... キーコード
//	LPCTSTR string ... 文字列(101.1 など)
//	struct _KamokuData* pKmdata ... 結果格納
//--------------------------------------------------------
BOOL CDnpBaseClass::CheckKamoku( long nChar, LPCTSTR string, struct _KamokuData* pKmdata )
{
//MyTrace( "@CheckKamoku nChar(%d), %s\n", nChar, string );

	struct _KamokuData check = {0}, ans = {0};
	BOOL bRet = FALSE;
	// 06.30 /04 -- コード表示時は 2桁 入力でも コード入力として扱う
	BOOL bCodeInput = FALSE;

	// 05.02 /12
	if( ! check_kmkstring( string )	)	return FALSE;

	check.kd_eda = -1;
	KamokuStr_ChkLength( string, &check, m_pZm->zvol->edcol );

	int kamoku = atoi( check.kd_code );

	if( m_pAsel->KMNO_OPT == OPT_KMNO ) {
		if( ! BRNTKY_sel || ( BRNTKY_sel && kamoku < 9 ) ) {
			if( check.kd_sgn == 1 || check.kd_sgn == 5 ) {
				bCodeInput = TRUE;
			}
		}
	}

	int ln = get_nowln();
	int dt_ln = get_dataline();
	int pn = get_nowpn();
	struct _DenpData *chkbuf, *keybuf=NULL;

	chkbuf = _getDATA( dt_ln, pn );

	if( chkbuf != NULL && _getCND( dt_ln, pn )->INP_sg ) {
		keybuf = chkbuf;
	}
	else {
		if( ln > 1 ) {
			// 前行のデータ
			keybuf = _getMAPtoDATA( (dt_ln-1), pn );
		}
	}
	//[. + 枝番] 科目コードコピー
	if( (check.kd_sgn == 2 || check.kd_sgn == 3) ) {
		if( keybuf != NULL ) {
			memcpy( check.kd_code, keybuf->DP_CdNum, KMKCD_LN );
		}
	}

	if( m_Seldata.InputItemCheck( ans, check, 0 ) == TRUE /*&& CheckKamokuInput( &ans ) == 0*/ ) {
		if( ! isSYOGT_CODE( ans.kd_code ) ) {
			// [コード + .] 前行枝番コピー
			if( IsJob() == JOB_INPUT || (IsJob() == JOB_SCAN && m_DPscan.IsAddData(dt_ln, pn)) ) {
				if( check.kd_sgn == 4 || check.kd_sgn == 5 ) {
					if( keybuf != NULL )
						ans.kd_eda = keybuf->DP_BRN;
				}
			}
			*pKmdata = ans;
			bRet = TRUE;
		}
	}
	if( (BYTE)ans.kd_code[0] == (BYTE)0xff ) {
		*pKmdata = ans;
	}
	
	return bRet;
}

// 科目　入力＆オフフォーカス
//	int ln ... 画面伝票上の　行番号(１～最大７)
//
long CDnpBaseClass::ItemInp( int ln, int pn, long nChar, struct _KamokuData* pKm ) 
{
	struct _DenpData* data, *keybuf;

//MyTrace( "----@ItemInp [TOP] nChar =%04x----\n", nChar );

	// オフフォーカス
	if( !nChar )
	{
		// データセット
		return ItemSet( ln, pn, 0, pKm, TRUE );
	}
	else
	{
		if( nChar == VK_TAB && is_SHIFT() )
			return 0;
	}

	// ページアップ／ダウン
	if( nChar )
	{
		nChar = SelItemDsp( nChar );
		if( !nChar )
			return(0);
	}

	if( nChar == VK_RETURN || nChar == VK_TAB ) {
		//データセット
		ItemSet( ln, pn, nChar, pKm, FALSE );
		if( nChar == VK_RETURN ) {
			// 上段コピー
			ItemCopy( ln, pn );
		}
	}

	int dt_ln = get_dataline( ln );
	keybuf = _getDATA( dt_ln, pn );

	//枝番の表示と枝番の入力へ( Tabは常に、Enterは枝番未入力時( 設定がONの時 ) )
	if( (nChar == VK_RETURN && keybuf->DP_BRN == -1) || nChar == VK_TAB ) {
		
//		if( nChar == VK_TAB ) {
//			ItemSet( ln, pn, pKm, FALSE );
//		}

		int bmn = -1;
		if( IsKind() == KIND_FURIKAE ) {
			data = _getDATA( dt_ln, (pn == FD_DEBT_PN) ? FD_DBMN_PN : FD_CBMN_PN );
		}
		else {
			data = _getDATA( dt_ln, DP_BMN_PN );
		}
		bmn = data->DP_BMN;

		CString code;
		code = keybuf->DP_stKMK.kd_code;

	//	BRNTKY_PAR bpar;
	//	bpar = m_Seldata.GetBrnTkyPar();

		if( EdabanDispReload( code ) ) {
	//	if( /*! BRNTKY_sel*/ code.CompareNoCase( bpar.bt_code ) != 0 ) {
			if( BRNTKY_select( bmn, code, NULL, (nChar == VK_TAB) ) ) {
				::ZeroMemory( saveITEM50kana, sizeof saveITEM50kana );	// カナ検索初期化
				//科目名称再表示
				nChar = 0;
			}
		}
	}
	else if( nChar == VK_DELETE )
	{
		// データＤＥＬＥＴＥ
		nChar	=	ItemDel( ln, pn );
		if( nChar != 0 && nChar != VK_RETURN ) {
			int kmk_ln = -1;
			if( ln > 1 ) {
				kmk_ln = _getMAP( dt_ln, pn );
			}
			if( kmk_ln == -1 ) {
				SyzDel( ln, (pn==FD_DEBT_PN) ? FD_DSKBN_PN : FD_CSKBN_PN );
			}
			ValInp( ln, (pn==FD_DEBT_PN) ? FD_DBVAL_PN : FD_CRVAL_PN, 0, 0, NULL );
		}
		if( nChar == VK_RETURN )	return VK_RETURN;
	}

	if( m_DI.IsEdaban() && m_pAsel->BRNKAK_OPT == OPT_BRNKAK ) {
		if( nChar != VK_DELETE ) {
			if( keybuf->DP_BRN == -1 ) {
				CString code;
				code = keybuf->DP_stKMK.kd_code;
				// 枝番残高があるか？
				if( m_DI.DB_EdabanZanSearch( code, -1 ) == 0 ) {
					nChar = 0;
				}
			}
		}
	}

	return( nChar );
}


// 部門コードを返送
int CDnpBaseClass::GetBmnCode(int ln, int pn) 
{
	struct _DenpData *bmnbuf;
	int dt_ln = get_dataline(ln);

	int bmnpn = (pn == FD_DEBT_PN) ? FD_DBMN_PN : FD_CBMN_PN;
	int bmncd = -1;

	if( IsKind() == KIND_FURIKAE ) {
		if( m_DI.IsBumon() ) {
			if( m_FuriMode == FURI_DATABMON ) {
				bmncd = _getDATA(dt_ln, bmnpn)->DP_BMN;
			}
			else {
				bmncd = getDATA(DP_BMN_PN)->DP_BMN;
			}
		}
	}
	else {
		bmncd = getDATA(DP_BMN_PN)->DP_BMN;
	}
	
	return bmncd;
}

// 科目　データセット
//	int ln ... 画面伝票上の　行番号(１～最大７)
//
int CDnpBaseClass::ItemSet( int ln, int pn, long nChar, struct _KamokuData* pKd, BOOL eda/*= FALSE*/ )
{
MyTrace( "@ItemSet [TOP] ln = %d, pn = %d, nChar = %04x, pKd = (%08x) eda(%d)\n", ln, pn, nChar, pKd, eda );

	if( pn < 0 )	return	-1;

	int dt_ln = get_dataline( ln );

	struct _DenpData check = {0}, *keybuf, *tmpbuf;
	int	pos, syzpn, zeipn;
	pos = pn;
	keybuf	=	_getDATA( dt_ln, pos );

	syzpn = (pn == FD_DEBT_PN) ? FD_DSKBN_PN : FD_CSKBN_PN;
	zeipn = (pn == FD_DEBT_PN) ? FD_DBZEI_PN : FD_CRZEI_PN;
	struct _KamokuData previtm  ={0};
	previtm = keybuf->DP_stKMK;

	struct _DNPLINE_CND* pCnd;
	SETMEISYO sm = {0};
	char edabuf[30];
	BOOL set_brtek = FALSE;	// 枝番摘要を仕訳摘要に

	pCnd = _getCND( dt_ln, pos );
	int bmncd = -1;

	//
	//データリード
	if( pKd )	memcpy( &check.DP_stKMK, pKd, sizeof check.DP_stKMK );

	if( pKd == NULL || pKd->kd_code[0] == '\0')
	{
	//	m_pZm->EdabanToStr( edabuf, sizeof edabuf, keybuf->DP_BRN );
		set_codestr( edabuf, sizeof edabuf, keybuf->DP_BRN, m_pZm->zvol->edcol );
		sprintf_s( sm.sm_name, sizeof sm.sm_name, "%.14s%s", keybuf->DP_KNAM, edabuf );

		strcpy_s( sm.sm_etc, sizeof sm.sm_etc, pCnd->ETC_msg );
		sm.sm_etccol = pCnd->ETC_col;

//		pCnd->INP_type = 0;
//		GetNowDnpDialog()->SetKnam( ln, pn, &sm );
//		return -1;
	}
	else if( pKd->kd_code[0] )
	{
		// インプットバッファにデータセット
		memmove( &keybuf->DP_stKMK, pKd, sizeof(struct _KamokuData));

//		m_pZm->EdabanToStr( edabuf, sizeof edabuf, keybuf->DP_BRN );
		set_codestr( edabuf, sizeof edabuf, keybuf->DP_BRN, m_pZm->zvol->edcol );
		sprintf_s( sm.sm_name, sizeof sm.sm_name, "%.14s%s", keybuf->DP_KNAM, edabuf );

		struct _SET_KMKETC sk;
		sk.bmn = GetBmnCode(ln, pn);;
		bmncd = sk.bmn;
		sk.code = keybuf->DP_CdNum;
		sk.brn  = keybuf->DP_BRN;
		if( nChar != VK_TAB && m_DI._set_kmketc( &sk ) ) {
			strcpy_s( sm.sm_etc, sizeof sm.sm_etc, sk.etc );
			sm.sm_etccol = sk.etc_col;
			strcpy_s( pCnd->ETC_msg, sizeof pCnd->ETC_msg, sm.sm_etc );
			pCnd->ETC_col = sk.etc_col;
		}
		else {
			::ZeroMemory( sm.sm_etc, sizeof sm.sm_etc );
			::ZeroMemory( pCnd->ETC_msg, sizeof pCnd->ETC_msg );
			pCnd->ETC_col = 0;
		}
		GetNowDnpDialog()->SetKnam( ln, pn, &sm );

		_setMAP( dt_ln, pos );	// 仕訳データマップサインセット
		pCnd->INP_sg = TRUE;
		// 入力枝番タイプセット
		int kmkeda_chg = -1;
		kmkeda_chg = brn_inptype(kmkeda_chg, previtm.kd_code, previtm.kd_eda, pKd->kd_code, pKd->kd_eda);
	//	pCnd->INP_type = brn_inptype( _getCND( dt_ln, pos )->INP_type, previtm.kd_code, previtm.kd_eda, pKd->kd_code, pKd->kd_eda );
		if( kmkeda_chg != -1 ) {
			_getCND(dt_ln, pos)->INP_type = kmkeda_chg;
		}

		DBkamokudata_set(GetNowDnpDialog(), IDC_ICSDBEDT_KMINP, pKd, m_pZm);

		if( kmkeda_chg != -1 && pCnd->INP_type == 1 ) {
			if( m_pAsel->BRTOTKY_OPT == OPT_EXEC ) {
				set_brtek = TRUE;
			}
		}
	}
	else
		pCnd->INP_sg = FALSE;

	//枝番登録チェック
	if( eda != FALSE ) {
		if( _getCND( dt_ln, pos )->INP_sg ) {
			CDBINPDataRec data;
			if( IsKind() == KIND_FURIKAE ) {
				tmpbuf = _getDATA( dt_ln, (pos == FD_DEBT_PN) ? FD_DBMN_PN : FD_CBMN_PN );
			}
			else {
				tmpbuf = _getDATA( dt_ln, DP_BMN_PN );
			}
			int	msw, dcsw, st;
			dcsw = (pos == FD_DEBT_PN) ? 0 : 1;
			data.Reset();
			if( ! dcsw ) {
				data.m_dbmn = tmpbuf->DP_BMN;
				data.m_dbt = keybuf->DP_CdNum;
				data.m_dbr = keybuf->DP_BRN;
			}
			else {
				data.m_cbmn = tmpbuf->DP_BMN;
				data.m_cre = keybuf->DP_CdNum;
				data.m_cbr = keybuf->DP_BRN;
			}

m_PRO_OK = FALSE;	// Diag へフォーカスがセットされるのを防ぐ (ActiveFrame)

			msw	=	m_DI.brchk1( &data, dcsw );

			if( msw == 99 ) {
				// 枝番登録ダイアログ
				if( ! dcsw ) {
					st = m_DI.brzan_torok( m_pDnpParent, data.m_dbmn, data.m_dbt, data.m_dbr, 0 );
				}
				else {
					st = m_DI.brzan_torok( m_pDnpParent, data.m_cbmn, data.m_cre, data.m_cbr, 1 );
				}

				if( st == 0 )
				{
					set_brtek = TRUE;
					CString kmkcd;
					int brn;

					if (!dcsw) {
						kmkcd = data.m_dbt;
						brn = data.m_dbr;
					}
					else {
						kmkcd = data. m_cre;
						brn = data.m_cbr;
					}
					if (m_DI.IsDiffEdabanSyzSgn((char*)(LPCTSTR)kmkcd, brn)) {
						m_DI.m_pSy->SyReOpen(m_DI.m_pZm);
					}
				}
				else if( st == -1 )				//キャンセル
				{
					m_DI.del_dtaeda( &data, dcsw );	//仕訳枝番消去
				}
			}

m_PRO_OK = TRUE;

			if( ! dcsw ) {
				if( data.m_dbr == -1 )	keybuf->DP_BRN = -1;
			}
			else {
				if( data.m_cbr == -1 )	keybuf->DP_BRN = -1;
			}

			// 入力枝番タイプセット
			if( keybuf->DP_BRN == -1 ) {
				pCnd->INP_type = 0;

				sprintf_s( sm.sm_name, sizeof sm.sm_name, "%.14s", keybuf->DP_KNAM );
				::ZeroMemory( sm.sm_etc, sizeof sm.sm_etc );
				sm.sm_etccol = 0;

				::ZeroMemory( pCnd->ETC_msg, sizeof pCnd->ETC_msg );
				pCnd->ETC_col = 0;
			}
			else {
				set_codestr( edabuf, sizeof edabuf, keybuf->DP_BRN, m_pZm->zvol->edcol );
			//	m_pZm->EdabanToStr( edabuf, sizeof edabuf, keybuf->DP_BRN );
				sprintf_s( sm.sm_name, sizeof sm.sm_name, "%.14s%s", keybuf->DP_KNAM, edabuf );

				struct _SET_KMKETC sk;
				sk.bmn = GetBmnCode( ln, pn );
				sk.code = keybuf->DP_CdNum;
				sk.brn  = keybuf->DP_BRN;
				if( m_DI._set_kmketc( &sk ) ) {
					strcpy_s( sm.sm_etc, sizeof sm.sm_etc, sk.etc );
					sm.sm_etccol = sk.etc_col;
					strcpy_s( pCnd->ETC_msg, sizeof pCnd->ETC_msg, sm.sm_etc );
					pCnd->ETC_col = sk.etc_col;
				}
				else {
					::ZeroMemory( sm.sm_etc, sizeof sm.sm_etc );
					::ZeroMemory( pCnd->ETC_msg, sizeof pCnd->ETC_msg );
					pCnd->ETC_col = 0;
				}
			}
			
			//枝番登録のDialogが表示されるため、フォーカスが不正になるのを対応
			if( msw ) {
				if( edaLine != -1 && edaPn != -1 ) {
					_set_focus(edaLine, edaPn);
				}
		//		if( NextClmn == -1 && NextClmn == -1 ) {
		//			_set_focus( ln, pos );
		//		}
		//		else if( NextClmn == -1 ) {
		//			SetFocusToImage( NextLine );
		//		}
		//		else {
		//			_set_focus( NextLine, NextClmn );
		//		}
			}
		}
	}
	MyTrace("@ItemSet(2)\n");

	// 枝番摘要を仕訳摘要に
	if( set_brtek ) {
		if( m_pAsel->BRTOTKY_OPT == OPT_EXEC ) {
			MyTrace("ItemSet(%d) brnitm_to_tky m_SYdataCnt(%d)\n", __LINE__, m_SYdataCnt);
			int idx = 0;

			BYTE	old_sy_chg = 0;
			old_sy_chg = _getDATA(dt_ln, syzpn)->SYDATA.sy_chg;	//消費税欄変更サイン。

			// 消費税区分をクリア
			::ZeroMemory(&_getDATA(dt_ln, syzpn)->SYDATA, sizeof _getDATA(dt_ln, syzpn)->SYDATA);
			// 税額手入力サインをクリア
			_getCND(dt_ln, zeipn)->SET_sg = FALSE;
			// 消費税変更サインは元に戻す。
			if( IsScanLine(dt_ln) ) {
				_getDATA(dt_ln, syzpn)->SYDATA.sy_chg = old_sy_chg;
			}

			mkSY_CREC(m_CREC, ln);

			if( IsKind() == KIND_FURIKAE ) {
				if( m_SYdataCnt == 2 && pn == FD_CRED_PN ) {
					idx = 1;
				}
			}
			TRACE("ItemSet(%d) brnitm_to_tky call\n", __LINE__);
			CDBINPDataRec dmy;	// 税額再計算のため、ダミー呼び出し。
			dmy.Reset();
			dmy = m_CREC[idx];
			dmy.m_dbt = SYOGT;
			dmy.m_cre = SYOGT;
			m_DI.sy_chk_txt(&dmy, NULL);

			m_DI.SetCDBData(&m_CREC[idx]);
			brnitm_to_tky( ln, bmncd, CString(keybuf->DP_CdNum), keybuf->DP_BRN );
		}
	}

	//データセット
	GetNowDnpDialog()->SetKnam( ln, pn, &sm );

	//カナの検索バッファをクリア
//	::ZeroMemory( saveITEM50kana, sizeof saveITEM50kana );

	//入金伝票及び出金伝票の時は、相手科目をセット。
	if( IsKind() != KIND_FURIKAE && _getCND( dt_ln/*get_nowln()*/, pos )->INP_sg != FALSE ) {

		struct _DenpData *buf;
		if( IsKind() == KIND_NYUKIN ) {
			buf	=	_getDATA( dt_ln/*get_nowln()*/, FD_DEBT_PN );
			pos	=	FD_DEBT_PN;
		}
		else {
			buf	=	_getDATA( dt_ln/*get_nowln()*/, FD_CRED_PN );
			pos	=	FD_CRED_PN;
		}
		strcpy_s( buf->DP_CdNum, sizeof buf->DP_CdNum, GNKINCode(m_pZm->zvol->apno) );
		_setMAP( dt_ln, pos );
		_getCND( dt_ln, pos )->INP_sg	=	TRUE;
		_getCND( dt_ln, pos )->INP_type	=	brn_inptype( _getCND( dt_ln, pos )->INP_type, previtm.kd_code, previtm.kd_eda, check.DP_CdNum, check.DP_BRN );
	}

	// 消費税関係
	// 科目が変更になった場合。
	int kmk_chg = 0;
	// set_brtek で クリア済み
	if( set_brtek /*&& (!previtm.kd_code[0] || previtm.kd_eda == -1)*/ ) {
		//枝番残高の設定のみの場合、下の setSY_CREC で再セット
		if( m_DI.IsDiffEdabanSyzSgn(keybuf->DP_CdNum, keybuf->DP_BRN) ) {
			kmk_chg = 1;
		}
	}
	else {
		if( memcmp( previtm.kd_code, keybuf->DP_CdNum, KMKCD_LN ) || (set_brtek && m_DI.IsDiffEdabanSyzSgn(keybuf->DP_CdNum, keybuf->DP_BRN)) ||
			previtm.kd_eda != keybuf->DP_BRN )
		{
			BYTE	old_sy_chg = 0;
			old_sy_chg = _getDATA( dt_ln, syzpn )->SYDATA.sy_chg;	//消費税欄変更サイン。
	
			// 消費税区分をクリア
			::ZeroMemory( &_getDATA( dt_ln, syzpn )->SYDATA, sizeof _getDATA( dt_ln, syzpn )->SYDATA );
			// 税額手入力サインをクリア
			_getCND( dt_ln, zeipn )->SET_sg = FALSE;
			// 消費税変更サインは元に戻す。
			if( IsScanLine( dt_ln ) ) {
				_getDATA( dt_ln, syzpn )->SYDATA.sy_chg = old_sy_chg;
			}
	
			kmk_chg = 1;
		}
	}


	MyTrace("ItemSet(%d) now _sy_chg (%d)\n", __LINE__, _getDATA(dt_ln, syzpn)->SYDATA.sy_chg);

//	BOOL bSame = IsSameValue( ln, pos );

	int dcnt, idx;
	dcnt = mkSY_CREC( m_CREC, ln );	// 消費税データ作成

	DBGFUNCTION;

	// 消費税サインをデータテーブルに
	idx = 0;
	if( dcnt == 2 && IsKind() == KIND_FURIKAE ) {
		if( pn == FD_CRED_PN )
			idx = 1;
	}

	// 05.11 /12 --------------------------
	if( kmk_chg ) {
		CDBINPDataRec dmy;	// 税額再計算のため、ダミー呼び出し。
		dmy.Reset();
		dmy = m_CREC[idx];
		dmy.m_dbt = SYOGT;
		dmy.m_cre = SYOGT;
		m_DI.sy_chk_txt( &dmy, NULL );
	}
	//-------------------------------------
	DBGFUNCTION;
	setSY_CREC( &m_CREC[idx], pn == FD_DEBT_PN ? 0 : 1, ln );

//	if( bSame ) {
		// 単独仕訳は、常に借方・貸方の金額を揃える
		SetTandokuSameValue( ln, pos );
//	}

	int clear = 0;
	if( IsSyafuKaisei( m_pZm->zvol ) ) {
		if( kmk_chg )	clear = 1;
	}

	if( IsKind() == KIND_FURIKAE ) {
		FuriHendJiyuCheck( m_CREC, dcnt, dt_ln, clear );
	}
	else {
		HendJiyuCheck( &m_CREC[idx], dt_ln, clear );
	}
	DBGFUNCTION;
	return 0;
}

// 科目　データＤＥＬＥＴＥ
//	int ln ... 画面伝票上の　行番号(１～最大７)
//
int CDnpBaseClass::ItemDel( int ln, int pn )
{
	if( pn < 0 )	return	-1;

	int dt_ln = get_dataline( ln );

	if( IsJob() == JOB_SCAN ) {
		switch( IsKind() ) {
			case	KIND_NYUKIN:
			case	KIND_SYUKKIN:
				if( IsScanLine( dt_ln ) ) {
					return	0;
				}
				break;
			default:
				if( IsScanLine( dt_ln ) ) {
					if( ! SyogSiwakeMode() ) {	//諸口仕訳詰める。
						// 単一仕訳は、F4 仕訳取消のみ対応
						if( ! m_DPscan.IsFukugoSiwake( dt_ln ) )	return	0;
						else {
							// 確定仕訳
							if( _getCND( dt_ln, pn )->EDT_sg > 1 )
								return 0;
						}
					}
					else {
						if( FuriSyogPosition( dt_ln, pn ) )	return 0;
						else {
							if( m_DPscan.IsTanitsuSiwake(dt_ln) )	return 0;
							// 確定仕訳
							if( _getCND( dt_ln, pn )->EDT_sg > 1 )
								return 0;
						}
					}
				}
				break;
		}
	}

	struct _DenpData *keybuf;
	int	pos;
	pos = pn;
	keybuf	=	_getDATA( dt_ln, pos );

	if( _getCND( dt_ln, pos )->INP_sg ) {
		memset( (char*)keybuf, (char)0, sizeof(struct _DenpData) );
		keybuf->DP_BRN = -1;

		_getCND( dt_ln, pos )->ETC_col = 0;
		::ZeroMemory( _getCND( dt_ln, pos )->ETC_msg, sizeof _getCND( dt_ln, pos )->ETC_msg );

		_delMAP( dt_ln, pos );	// 仕訳データマップサイン取消

		_getCND( dt_ln, pos )->INP_sg = FALSE;

		// 入力枝番タイプセット
		_getCND( dt_ln, pos )->INP_type = 0;

		GetNowDnpDialog()->SetKnam( ln, pos, NULL );

		struct _KamokuData check = { 0 };
		check.kd_eda = -1;
		CDnpBaseDlg* dlg;
		UINT	id = IDC_ICSDBEDT_KMINP;
		dlg = GetNowDnpDialog();
		DBkamokudata_set(dlg, id, &check, m_pZm);
	}
	else {
		return VK_RETURN;
	}

	return	VK_DELETE;
}



// 科目削除に伴う消費税項目削除
//	int ln ... 画面伝票上の　行番号(１～最大７)
//
int CDnpBaseClass::SyzDel( int ln, int pn )
{
	int zeipn;
	zeipn = (pn == FD_DSKBN_PN) ? FD_DBZEI_PN : FD_CRZEI_PN;

	int dt_ln = get_dataline( ln );

	_getCND( dt_ln, pn )->INP_sg = FALSE;

	_getCND( dt_ln, zeipn )->INP_sg = FALSE;
	_getCND( dt_ln, zeipn )->SET_sg = FALSE;
	::ZeroMemory( _getDATA( dt_ln, zeipn )->DP_VAL, sizeof _getDATA( dt_ln, zeipn )->DP_VAL );

	PutsSyz( ln, pn, "", _getDATA( dt_ln, zeipn )->SYDATA.sy_chg );
	GetNowDnpDialog()->SetVal( ln, zeipn, NULL );

	return 0;
}


// 科目　データＣＯＰＹ
//	int ln ... 画面伝票上の　行番号(１～最大７)
//
void CDnpBaseClass::ItemCopy( int ln, int pn )
{
	if( pn < 0 )	return;
	int	pos;
	pos = pn;
	struct _DenpData *prdata;

	if( m_DI.IsConFirmMaster() )	return;

	int dt_ln = get_dataline( ln );

	// 伝票修正 は コピーしない [From管理課] <01.31 /07>
	BOOL bCopy = FALSE;
#ifdef CLOSE // 01.21 /11
	if( IsJob() == JOB_INPUT &&  !_getCND( dt_ln, pos )->INP_sg )
#else
	if( ! _getCND( dt_ln, pos )->INP_sg )
#endif
		bCopy = TRUE;

	// 諸口詰めないで表示するとき
	if( FuriSyogPosition( dt_ln, pn ) )	return;

	if( bCopy && (prdata = _getPRDATA( dt_ln, pos )) ) {
		if( IsType() == TYPE_FUKUGOU ) {
			if( isSYOGT_CODE( prdata->DP_CdNum ) )	return;
		}

		// 現在の業種でない科目の 空うちEnter での 入力をチェックする。
		int type =  IsKojinGyosyuMaster( m_pZm );
		if( type ) {
			if( IsKojinKengyoMaster( type ) ) {
				struct _KamokuData  akd = {0}, ckd = {0};

				ckd = prdata->DP_stKMK;
				ckd.kd_sgn = 2;	// 科目コードチェック用に 枝番のみのサインセット

				if( ! m_Seldata.InputItemCheck( akd, ckd, 0 ) ) {
					Buzzer();
					return;
				}
			}
		}

		struct _DenpData cpyData;
		memcpy( &cpyData, prdata, sizeof (struct _DenpData) );
		if( m_pAsel->BRCPY_OPT == OPT_NONOPT ) {	// 08.02 /02
			cpyData.DP_stKMK.kd_eda = -1;
		}

		ItemSet( ln, pn, 0, &cpyData.DP_stKMK );
	//	ItemSet( ln, pn, 0, &prdata->DP_stKMK );
	}
}


// 科目が変更になったときの変動事由をチェック[振替伝票用]
void CDnpBaseClass::FuriHendJiyuCheck( CDBINPDataRec *pcrec, int dcnt, int dt_ln, int clear/*=0*/ )
{
	if( dcnt == 1 ) {
		return HendJiyuCheck( pcrec, dt_ln, clear );
	}

	struct _DenpData* keybuf;
	keybuf = _getDATA(dt_ln, FD_HJIYU_PN );
	pcrec->m_hjiyu	= keybuf->DP_BMN;
	pcrec->m_nbcd	= keybuf->NAIBCD;

	(pcrec+1)->m_hjiyu	= keybuf->DP_BMN;
	(pcrec+1)->m_nbcd	= keybuf->NAIBCD;

	int hj_ok1, hj_ok2;
	hj_ok1 = IsEnableHjiyu( pcrec );
	hj_ok2 = IsEnableHjiyu( (pcrec+1) );

	if( pcrec->m_hjiyu != 0 ) {
		if( clear || (! hj_ok1 && ! hj_ok2 ) ) {
			pcrec->m_hjiyu = (pcrec+1)->m_hjiyu = 0;
			keybuf->DP_BMN = 0;
			GetNowDnpDialog()->SetHjiyu( dt_ln, FD_HJIYU_PN, NULL );
			_getCND( dt_ln, FD_HJIYU_PN )->INP_sg = FALSE;
		}
		else {
			if( ! hj_ok1 ) {
				pcrec->m_hjiyu = 0;
			}
			if( ! hj_ok2 ) {
				(pcrec+1)->m_hjiyu = 0;
			}
		}
	}
}

// 科目が変更になったときの変動事由をチェック
void CDnpBaseClass::HendJiyuCheck( CDBINPDataRec *pcrec, int dt_ln, int clear/*=0*/ )
{
	struct _DenpData* keybuf;
	keybuf = _getDATA(dt_ln, FD_HJIYU_PN );
	pcrec->m_hjiyu	= keybuf->DP_BMN;
	pcrec->m_nbcd	= keybuf->NAIBCD;

	if( pcrec->m_hjiyu != 0 ) {
		if( ! IsEnableHjiyu( pcrec ) || clear ) {
			pcrec->m_hjiyu = 0;
			keybuf->DP_BMN = 0;
			GetNowDnpDialog()->SetHjiyu( dt_ln, FD_HJIYU_PN, NULL );
			_getCND( dt_ln, FD_HJIYU_PN )->INP_sg = FALSE;
		}
	}
}



/*================================================
	枝番摘要toデータ摘要
	int ln ... 画面伝票上の　行番号(１～最大７)

=================================================*/
BOOL CDnpBaseClass::brnitm_to_tky( int ln, int bmn, CString& code, int brn )
{
	int dc_sw, tno;
	CDBipTKREC tkrec;
	char *dbr_inptype, *cbr_inptype;
	int dt_ln = get_dataline( ln );

	if( brn == -1 )	return FALSE;

	// 枝番摘要を検索
	if( m_DI.IsEdaban() )
	{
		// 枝番to摘要レコードリード
		int bmncd = -1;
		if( m_pAsel->BMNTKBR_OPT == OPT_CHKON )
			bmncd = bmn;

		if( tno = m_DI.m_BrTek.get_brtek( bmncd/*bmn*/, code, brn ) )
		{
			if( tno > 0 ) {
				if( m_DI.DB_TekiyoNoRead( &tkrec, tno ) == ERR )
					return FALSE;
				//摘要toデータ
				dbr_inptype = &getCND( FD_DEBT_PN )->INP_type;
				cbr_inptype = &getCND( FD_CRED_PN )->INP_type;
				dc_sw = tky_select_to_drec( m_CREC, &tkrec, dbr_inptype, cbr_inptype );
			}
			else {
				tkrec.Reset();
				if( bmncd != -1 ) {
					// 部門枝番残高本体に登録された枝番名称
					CDBINPDataRec tmpRec;
					tmpRec.m_dbmn = bmncd;
					tmpRec.m_dbt = code;
					tmpRec.m_dbr = brn;

					if( m_DI.DB_BmnEdabanMasterSearch(&tmpRec, 0) != 0 )
						return TRUE;
					tkrec.m_tkname = m_DI.m_pZm->bmezrec->ezname;
				}
				else {
					// 枝番残高本体に登録された枝番名称
					if( m_DI.DB_EdabanZanSearch(code, brn) != 0 )
						return TRUE;
					tkrec.m_tkname = m_DI.m_pZm->ezrec->ezname;
				}
				dc_sw = 0;
			}

			char tmp[128] = {0};
			strcpy_s( tmp, sizeof tmp, (const char*)_getDATA( dt_ln, FD_TKY_PN )->DP_TKY );

			if( IsIncludeString( tmp, tkrec.m_tkname ) )
				return FALSE;

			strcat_s( tmp, sizeof tmp, tkrec.m_tkname );

			int n = get_strcnt( tmp, (m_DI.m_pZm->zvol->tk_ln * 2) );
			tmp[n] = '\0';

			::ZeroMemory( _getDATA( dt_ln, FD_TKY_PN ), sizeof (struct _DenpData) );
			memcpy( _getDATA( dt_ln, FD_TKY_PN )->DP_TKY, tmp, n );
			_getCND( dt_ln, FD_TKY_PN )->INP_sg = TRUE;

			GetNowDnpDialog()->SetTekiyo( ln, FD_TKY_PN, (char*)_getDATA( dt_ln, FD_TKY_PN )->DP_TKY );

			return TRUE;
		}
	}
	return FALSE;
}


//摘要の消費税サインをバッファヘ
void CDnpBaseClass::TKsgnToSYDATA(struct __SYDATA* syd, CDBipTKREC* ptk)
{
	syd->TKattr = ptk->m_tksgn[1];	//消費税コード 

	for( int n = 0; n < SIZE_DSIGN; n++ ) {
		syd->TKsign[n] = ptk->m_tkdsgn[n];
	}
	//摘要の登録番号データ有りか？
	if( ptk->m_tkinvno[0] ) {
		::ZeroMemory(syd->TKinvno, sizeof syd->TKinvno);

		if( ptk->m_tkinvno[0] != 'T' ) {
			sprintf_s(syd->TKinvno, sizeof(syd->TKinvno), _T("T%s"), ptk->m_tkinvno);
		}
		else {
			sprintf_s(syd->TKinvno, sizeof(syd->TKinvno), _T("%s"), ptk->m_tkinvno);
		}
	}
}


/*===============================
	選択摘要toデータ
================================*/
int CDnpBaseClass::tky_select_to_drec( CDBINPDataRec *drec, CDBipTKREC *ptk, char *dbr_inptype, char *cbr_inptype, int dataLn/*= 0*/)
{
	DBGFUNCTION;
int dt_ln;
if( dataLn != 0 )	dt_ln = dataLn;
else {
	dt_ln = get_dataline();
}

struct _DenpData *dsybuf = _getDATA( dt_ln/*get_nowln()*/, FD_DSKBN_PN );
struct _DenpData *csybuf = _getDATA( dt_ln/*get_nowln()*/, FD_CSKBN_PN );
struct _DenpData *keybuf = NULL;

struct _SY_MSG_PACK2 smpk;
int tmp, i, max = 1;
int dc_sw = 0;
int	pn;

	if( IsKind() == KIND_FURIKAE ) {
		// 振替伝票で借方／貸方 に消費税あり
		if( m_SYdataCnt == 2 ) {
			max = 2;
			TKsgnToSYDATA(&dsybuf->SYDATA, ptk);
			TKsgnToSYDATA(&csybuf->SYDATA, ptk);
			//dsybuf->SYDATA.TKattr = ptk->m_tksgn[1];	//消費税コード 
			//csybuf->SYDATA.TKattr = ptk->m_tksgn[1];	//消費税コード
		}
		else {
			// 消費税科目のみ
			if( ! isSYOGT_CODE( drec->m_dbt ) && m_DI.km_syattr( drec->m_dbt ) ) {
				TKsgnToSYDATA(&dsybuf->SYDATA, ptk);
			//	dsybuf->SYDATA.TKattr = ptk->m_tksgn[1];
				keybuf = dsybuf;
				pn	=	FD_DBZEI_PN;
			}
			
			if( ! isSYOGT_CODE( drec->m_cre ) && m_DI.km_syattr( drec->m_cre ) ) {
				TKsgnToSYDATA(&csybuf->SYDATA, ptk);
			//	csybuf->SYDATA.TKattr = ptk->m_tksgn[1];
				keybuf = csybuf;
				pn	=	FD_CRZEI_PN;
			}
		}
	}
	else if( IsKind() == KIND_SYUKKIN ) {
		TKsgnToSYDATA(&dsybuf->SYDATA, ptk);
	//	dsybuf->SYDATA.TKattr = ptk->m_tksgn[1];
		keybuf = dsybuf;
		pn	=	FD_DBZEI_PN;
	}
	else {
		TKsgnToSYDATA(&csybuf->SYDATA, ptk);
	//	csybuf->SYDATA.TKattr = ptk->m_tksgn[1];
		keybuf = csybuf;
		pn	=	FD_CRZEI_PN;
	}

	//摘要枝番を検索
	if( m_pZm->zvol->tk_br && m_DI.IsEdaban() )
	{
		for( i = 0; i < max; i++ ) {
			tmp = m_DI.tbrasrch( drec+i, ptk->m_tkcod, dbr_inptype, cbr_inptype );
			dc_sw |= tmp;

			if( *dbr_inptype == 1 ) {
				dc_sw |= 0x01;
			}
			if( *cbr_inptype == 1 ) {
				dc_sw |= 0x02;
			}
		}
	}

	//消費税セット
	if( m_pSy->IsSyohizeiMaster() && IsJob() == JOB_INPUT ) {
		if( max == 2 ) {
			for( i = 0; i < max; i++ ) {
				keybuf = (i ==0) ? dsybuf : csybuf;
				m_DI.DB_SyIniz( (drec+i) );

				pn = (i ==0) ? FD_DBZEI_PN : FD_CRZEI_PN;
				set_skbn_selsw( (drec+i), keybuf, pn );
			}
		}
		else if( keybuf != NULL && IsTekiyoSyzChg(&keybuf->SYDATA) ) {
			m_DI.DB_SyIniz(drec);

			if( IsKind() == KIND_NYUKIN ) {
				if( dc_sw & 0x02 ) {
					m_pZm->dbdata->cbr = drec->m_cbr;
				}
			}
			else if( IsKind() == KIND_SYUKKIN ) {
				if( dc_sw & 0x01 ) {
					m_pZm->dbdata->dbr = drec->m_dbr;
				}
			}
			else {
				if( dc_sw & 0x01 ) {
					m_pZm->dbdata->dbr = drec->m_dbr;
				}
				if( dc_sw & 0x02 ) {
					m_pZm->dbdata->cbr = drec->m_cbr;
				}
			}
			set_skbn_selsw( drec, keybuf, pn );
		}
	}

	return dc_sw;
}


// 摘要の消費税属性をつけたか？
BOOL CDnpBaseClass::IsTekiyoSyzChg(struct __SYDATA* syd)
{
	BOOL bRet = FALSE;

	if( syd->TKattr ) {
		bRet = TRUE;
	}
	else {
		//個別対応
		if( m_pSy->IsKobetsuMaster() ) {
			if( syd->TKsign[4] & 0x0f ) {
				bRet = TRUE;
			}
		}
		if( m_pSy->IsKaniMaster() ) {
			if( syd->TKsign[3] & 0x0f ) {
				bRet = TRUE;
			}
		}
		//インボイス
		if( m_DI.IsMasterType(MST_SYZ_INV) ) {
			if( syd->TKsign[4] & 0x10 ) {
				bRet = TRUE;
			}
			if( syd->TKinvno[0] ) {
				bRet = TRUE;
			}
		}
	}
	return bRet;
}


void CDnpBaseClass::SetSyTkrec(_SY_TKREC_ *psyTkRec, struct __SYDATA* syd)
{
	ASSERT(psyTkRec);
	ASSERT(syd);

	if( (psyTkRec == NULL) || (syd == NULL) ) {
		return;
	}

	memset(psyTkRec, '\0', sizeof(_SY_TKREC_));

	psyTkRec->tk_attr = syd->TKattr;
	for( int i = 0; i < SIZE_DSIGN; i++ ) {
		psyTkRec->tk_dsign[i] = syd->TKsign[i];
	}
	if( strlen(syd->TKinvno) > 0 ) {
		if( syd->TKinvno[0] != 'T' ) {
			sprintf_s(psyTkRec->tk_invno, sizeof(psyTkRec->tk_invno), _T("T%s"), syd->TKinvno);
		}
		else {
			sprintf_s(psyTkRec->tk_invno, sizeof(psyTkRec->tk_invno), _T("%s"), syd->TKinvno);
		}
	}
}


void CDnpBaseClass::SyTkrecMenCheck(_SY_TKREC_ *psyTkRec, CDBINPDataRec* data)
{
	//免税サイン ON を生かすため 摘要の免税もON
	if( data->m_dsign[6] & 0x20 ) {
		int attr = (psyTkRec->tk_attr & 0x0f);
		if( attr != 2 && attr != 3 ) {
			psyTkRec->tk_dsign[4] |= 0x10;
		}
	}
}


//-------------------------------------------------------------------------------
//	摘要入力で消費税変更の場合、消費税サインを入力バッファへ
//	CDBINPDataRec* pdata		このデータでSyIniz()しておく
//	struct _DenpData* pkey	サイン格納入力バッファ
//	int zeipn	税額再表示用ポジション
//-------------------------------------------------------------------------------
void CDnpBaseClass::set_skbn_selsw( CDBINPDataRec* pdata, struct _DenpData* pkey, int zeipn )
{
	int swk, ritu, zei, uri, tai, toku;
	swk = ritu = zei = uri = tai = toku = 0;
	CString txt;
	struct _SY_MSG_PACK2 smpk;
	_SY_TKREC_ syTkrec;

	SetSyTkrec(&syTkrec, &pkey->SYDATA);

	int kmk_pn = 0;
	if( zeipn == FD_DBZEI_PN ) {
		kmk_pn = FD_DEBT_PN;
	}
	else if( zeipn == FD_CRZEI_PN ) {
		kmk_pn = FD_CRED_PN;
	}
	//摘要自動仕訳の免税サインを生かすため
	if( kmk_pn ) {
		if( getCND(kmk_pn)->SET_sg ) {
			if( pdata->m_dsign[6] & 0x20 ) {
				syTkrec.tk_dsign[4] |= 0x10;
			}
		}
		else {
			//新規入力時に、免税サインが 摘要入力で落ちてしまう緊急修正依頼対応(23/11/29)
			if( IsJob() == JOB_INPUT ) {
				if( !m_DI.CheckTkrec(&syTkrec) && syTkrec.tk_attr == 0 )
					return;

				SyTkrecMenCheck(&syTkrec, pdata);
			}
		}
	}

	m_pSy->Sy_tkyo_inp(&syTkrec);

	m_pSy->SyMsg( &smpk );
	txt = m_pSy->Sy_get_message( m_pZm->dbdata, SYGET_TYPE );

	m_DI.GetCDBData( pdata );

	::ZeroMemory( pkey->SYDATA.SKBNtxt, sizeof pkey->SYDATA.SKBNtxt );
	strcpy_s( pkey->SYDATA.SKBNtxt, sizeof pkey->SYDATA.SKBNtxt, txt );

	memcpy( pkey->SYDATA._DSGN, &m_pZm->dbdata->dsign[0], sizeof pkey->SYDATA._DSGN );

	swk		= smpk.SY_MSG_SWKBN->sel_no;
	ritu	= smpk.SY_MSG_RITU->sel_no;
	zei		= smpk.SY_MSG_ZEI->sel_no;
	uri		= smpk.SY_MSG_URISIRE->sel_no;
	toku	= smpk.SY_MSG_TOKTEI->sel_no;
	tai		= smpk.SY_MSG_TAIKAKBN->sel_no;

	if( swk )	pkey->SYDATA.swk_sw = smpk.SY_MSG_SWKBN->msg_tbl[swk-1]->code;
	else		pkey->SYDATA.swk_sw = 0;
	
	if( ritu )	pkey->SYDATA.ritu_sw = smpk.SY_MSG_RITU->msg_tbl[ritu-1]->code;
	else		pkey->SYDATA.ritu_sw = 0;

	if( zei )	pkey->SYDATA.zeikbn_sw = smpk.SY_MSG_ZEI->msg_tbl[zei-1]->code;
	else		pkey->SYDATA.zeikbn_sw = 0;

	if( uri )	pkey->SYDATA.uri_sw = smpk.SY_MSG_URISIRE->msg_tbl[uri-1]->code;
	else		pkey->SYDATA.uri_sw = 0;
	//特定収入
	if( toku )	pkey->SYDATA.toku_sw = smpk.SY_MSG_TOKTEI->msg_tbl[toku-1]->code;
	else		pkey->SYDATA.toku_sw = 0;

	if( tai )	pkey->SYDATA.kts_sw = smpk.SY_MSG_TAIKAKBN->msg_tbl[tai-1]->code;
	else		pkey->SYDATA.kts_sw = 0;


	// 税額 再表示
	char	zeibf[6] = {0};
	char*	pZei = NULL;
	int		sgn = 0;
	int		sotomen = 0;
	// 有価証券非課税譲渡
	if( m_pSy->IsTaikaData( m_pZm->dbdata ) == 2 )	sgn = 1;

	l_input( zeibf, (char*)(LPCTSTR)smpk.SY_MSG_SYZEI );	// 税額をセット
	memmove( getDATA( zeipn )->DP_VAL, zeibf, 6 );

	// 消費税仕訳
	if( m_DI.is_syohizeisiwake( m_pZm->dbdata ) ) {
		_getCND( get_dataline()/*get_nowln()*/, zeipn )->INP_sg = TRUE;
		pZei = zeibf;

		COLORREF back;
		if (m_DI.isnot_defzeigaku(m_pZm->dbdata, back)) {
			sgn = 3;
			if (back != RGB_ZEI_MDFY) {
				sgn++;
			}
		}
		// 輸出仕訳
		if( m_DI.is_yusyutsusiwake(m_pZm->dbdata) )	  sgn = 5;

		//外税 免税事業者等からの
		if( m_DI.is_sotomensiwake( m_pZm->dbdata) ) {
			sotomen = 1;
		}
	}
	else {
		sgn = 2;
		_getCND( get_dataline()/*get_nowln()*/, zeipn )->INP_sg = FALSE;
		pZei = NULL;
	}
	_getCND( get_dataline(), zeipn )->INP_type = sgn;
	set_sotomen_sign(get_dataline(), zeipn, sotomen);

	// 現在レコードの 対価 をバッファにセットする
	memcpy( pkey->SYDATA.TKsyohi, pdata->m_taika, sizeof pdata->m_taika );
	// インボイス番号も同様
	memcpy( pkey->SYDATA.TKinvno, pdata->m_invno, SZ_INVNO);

	GetNowDnpDialog()->SetVal( get_nowln(), zeipn, pZei, sgn );

}


BOOL	CDnpBaseClass::ToViewAccelerator( UINT ID, long nChar )
{
	int pn;
	BOOL bRet = TRUE;

	switch( nChar ) {
		case	VK_F3:
			m_pDnpParent->PostMessage( WM_COMMAND, ID_TLBAR_F3 );
			break;
		case	VK_F4:
			m_pDnpParent->PostMessage( WM_COMMAND, ID_TLBAR_F4 );
			break;
		case	VK_F7:
			m_pDnpParent->PostMessage( WM_COMMAND, ID_TLBAR_F7 );
			break;
		case	VK_F12:
			pn = get_nowpn();
			if( pn == FD_TKY_PN ) {
				bRet = FALSE;
			}
			else {
				m_pDnpParent->PostMessage(WM_COMMAND, ID_TLBAR_F12);
			}
			break;
		case	VK_ESCAPE:
			m_pDnpParent->PostMessage( WM_COMMAND, ID_TLBAR_ESC );
			break;
		case	VK_END:
			m_pDnpParent->PostMessage( WM_COMMAND, ID_TLBAR_END );
			break;

		case VK_DELETE:
			if( ! ButtonDeleteJob() ) {
				bRet = FALSE;
			}
			pn = get_nowpn();
			switch( pn ) {
			case DP_DENP_PN:
			case DP_BMN_PN:
			case DP_KOJI_PN:
				bRet = FALSE;
			}
			break;

		case VK_PRIOR:
		case VK_NEXT:
			pn = get_nowpn();

			switch( pn ) {
			case DP_BMN_PN:
			case DP_KOJI_PN:
			case FD_DEBT_PN:
			case FD_CRED_PN:
			case FD_DBMN_PN:
			case FD_CBMN_PN:
			case FD_DKOJI_PN:
			case FD_CKOJI_PN:
			case FD_TKY_PN:
			case FD_DSYOG_PN:
			case FD_CSYOG_PN:
			case FD_HJIYU_PN:
				bRet = FALSE;
			//	ToViewTermJob( get_nowln(), pn, nChar );
				break;
			default:

				if( nChar == VK_PRIOR ) {
					GetNowDnpDialog()->ScrollbarMsgFunc( WM_VSCROLL, MAKELONG(SB_PAGEUP, 0), 0 );
				}
				else {
					GetNowDnpDialog()->ScrollbarMsgFunc( WM_VSCROLL, MAKELONG(SB_PAGEDOWN, 0), 0 );
				}
				break;
			}

			break;
	}

	return	bRet;
}


BOOL CDnpBaseClass::IsReadOnly( UINT id )
{

	if( id >= IDC_IMAGE1 && id <= IDC_IMAGE7 ) {
		int	ln;
		ln	=	(id	-	IDC_IMAGE1) + 1;
		int dt_ln;
		dt_ln = get_dataline( ln );

		int pn;
		if( IsKind() == KIND_FURIKAE )
			pn = FD_DBVAL_PN;
		else if( IsKind() == KIND_NYUKIN )
			pn = FD_CRED_PN;
		else
			pn = FD_DEBT_PN;
		
		if( _getCND( dt_ln, pn )->EDT_sg > 1 ) {
			_set_focus( get_nowln(), get_nowpn() );
			return	TRUE;
		}
		return	FALSE;
	}

	return FALSE;
}


BOOL CDnpBaseClass::IsDelete( UINT id )
{
	if( id >= IDC_IMAGE1 && id <= IDC_IMAGE7 ) {
		int	ln;
		ln	=	(id	-	IDC_IMAGE1) + 1;
		int dt_ln;
		dt_ln = get_dataline( ln );

//		GetDelSgn( ln );

		if( GetDelSgn( dt_ln ) ) {
			_set_focus( get_nowln(), get_nowpn() );

			return	TRUE;
		}
		return	FALSE;
	}

	return FALSE;
}



// 貸方金額キータミネーション
//	int ln ... 画面伝票上の　行番号(１～最大７)
void CDnpBaseClass::OnTermCredVal( int ln, long nChar )
{
	int vect;

	if( nChar == VK_UP || nChar == VK_DOWN )
	{
		int dset = 0;

		if( nChar == VK_UP && ln == 1 ) {
			dset = DnpLine_Chg(-1);
		}
		else if( nChar == VK_DOWN && ln == getSCROLL_NL() ) {
			dset = DnpLine_Chg(1);
		}

		if( !dset ) {
			// 行移動
			if( nChar == VK_UP ) {
				--ln;
				vect	=	-1;
			}
			else {
				++ln;
				vect	=	1;
			}

			//行移動エラー（アンダー／オーバー　フロー）
			if( set_position( ln, FD_CRVAL_PN, vect ) == -1 ) {
				if( nChar == VK_UP )	set_focus( DP_MM_PN );
				else {
					if( IsKind() == KIND_NYUKIN )		set_position( 1, FD_CRED_PN );
					else if( IsKind() == KIND_FURIKAE )	set_position( 1, FD_CRVAL_PN );
				}
			}
		}
		return;
	}

	// カーソルセット
	switch( nChar )
	{
	case VK_RIGHT:
	case VK_RETURN:
	case VK_TAB:
//	case VK_F3:
	case VK_DELETE:
		if( IsKind() == KIND_FURIKAE ) {
			if( _getCND(ln, FD_CRZEI_PN)->ETC_sg[0] ) {
				_getCND(ln, FD_CRZEI_PN)->ETC_sg[1] = FD_CRVAL_PN;
				set_position(ln, FD_CRZEI_PN);
				break;
			}
			set_position( ln, FD_TKY_PN );
			break;
		}
		else if( IsKind() == KIND_NYUKIN ) {
			vect = (nChar == VK_RETURN ) ? 1 : 2;

			m_SET_SYZINP_PN = m_pSyzInp->CheckSyzPn( vect );

			if( vect == 1 ) {
				if( _getCND(ln, FD_CRZEI_PN)->ETC_sg[0] ) {
					_getCND(ln, FD_CRZEI_PN)->ETC_sg[1] = FD_CRVAL_PN;
					set_position(ln, FD_CRZEI_PN);
					break;
				}
			}

			if( IsAutoSiwake(ln) ) {
				set_position(ln, FD_TKY_PN);
			}
			else {
				if( m_SET_SYZINP_PN >= 0 ) {
					set_position(ln, FD_CSKBN_PN);
				}
				else {
					if( SyzStopMode() && IsKamokuSyattr(get_nowln(), FD_CRED_PN) ) {
						set_position(ln, FD_CSKBN_PN);
					}
					else {
						set_position(ln, FD_TKY_PN);
					}
				}
			}
		}
		break;
	case VK_LEFT:
	case VK_F2:
		if( IsKind() == KIND_FURIKAE ) {
			set_position( get_nowln(), FD_CRED_PN );
		}
		else if( IsKind() == KIND_NYUKIN ) {
			set_position( get_nowln(), FD_TKY_PN );
		}
		break;
	case 0:
		break;
	default:
		Buzzer();
		break;
	}
}

// 貸方税額キータミネーション
//	int ln ... 画面伝票上の　行番号(１～最大７)
void CDnpBaseClass::OnTermCredZei( int ln, long nChar )
{
	int vect;

	if( /*nChar == VK_UP ||*/ nChar == VK_DOWN )
	{
		// 行移動
		int dset = 0;

		if( nChar == VK_UP && ln == 1 ) {
			dset = DnpLine_Chg(-1);
		}
		else if( nChar == VK_DOWN && ln == getSCROLL_NL() ) {
			dset = DnpLine_Chg(1);
		}

		if( !dset ) {
			if( nChar == VK_UP ) {
				--ln;
				vect	=	-1;
			}
			else {
				++ln;
				vect	=	1;
			}

			//行移動エラー（アンダー／オーバー　フロー）
			if( set_position( ln, FD_CRZEI_PN, vect ) == -1 ) {
				if( nChar == VK_UP )	set_focus( DP_MM_PN );
				else {
					if( IsKind() == KIND_NYUKIN )		set_position( 1, FD_CRED_PN );
					else if( IsKind() == KIND_FURIKAE )	set_position( 1, FD_CRZEI_PN );
				}
			}
		}

		return;
	}
	int dt_ln = get_dataline(ln);

	// カーソルセット
	switch( nChar )
	{
	case VK_RIGHT:
	case VK_RETURN:
	case VK_TAB:
//	case VK_F3:
	case VK_DELETE:
		if( IsKind() == KIND_FURIKAE ) {
			// 外税・免税事業者 で 金額からは摘要欄に
			if( _getCND(dt_ln, FD_CRZEI_PN)->ETC_sg[0] ) {
				if( _getCND(dt_ln, FD_CRZEI_PN)->ETC_sg[1] == FD_CRVAL_PN ) {
					set_position(ln, FD_TKY_PN);
					break;
				}
			}
		//	if( set_position( ++ln, FD_DBVAL_PN ) == -1 )
		//		set_position( 1, FD_DBVAL_PN );
			if( set_position(++ln, FD_DBVAL_PN) == -1 ) {
				if( DnpLine_Chg(1, 1) ) {
					set_position((ln - 1), FD_DBVAL_PN);
				}
				else {
					set_position(1, FD_DBVAL_PN);
				}
			}
			break;
		}
		else if( IsKind() == KIND_NYUKIN ) {
			vect = (nChar == VK_RETURN ) ? 1 : 2;

			m_SET_SYZINP_PN = m_pSyzInp->CheckSyzPn( vect );

			if( _getCND(dt_ln, FD_CRZEI_PN)->ETC_sg[0] ) {
				if( _getCND(dt_ln, FD_CRZEI_PN)->ETC_sg[1] == FD_CSKBN_PN ) {
					set_position(ln, FD_TKY_PN);
					break;
				}
			}

			if( m_SET_SYZINP_PN >= 0 ) {
				if( _getCND(dt_ln, FD_CRZEI_PN)->ETC_sg[0] ) {
					_getCND(dt_ln, FD_CSKBN_PN)->ETC_sg[1] = FD_CRZEI_PN;
				}
				set_position( ln, FD_CSKBN_PN );
			}
			else {
				if( SyzStopMode() && IsKamokuSyattr(get_nowln(), FD_CRED_PN) ) {
					m_SET_SYZINP_PN = SY_SZKBN_PN;
					set_position(ln, FD_CSKBN_PN);
					if( _getCND(dt_ln, FD_CRZEI_PN)->ETC_sg[0] ) {
						_getCND(dt_ln, FD_CSKBN_PN)->ETC_sg[1] = FD_CRZEI_PN;
					}
				}
				else {
					set_position(ln, FD_TKY_PN);
				}
			}
		}
		break;
	case VK_LEFT:
	case VK_F2:
	case VK_UP:
		set_position(get_nowln(), FD_CRVAL_PN);
#ifdef CLOSE
		if( IsKind() == KIND_FURIKAE ) {
			set_position( get_nowln(), FD_CRED_PN );
		}
		else if( IsKind() == KIND_NYUKIN ) {
			set_position( get_nowln(), FD_TKY_PN );
		}
#endif
		break;
	default:
		Buzzer();
		break;
	}
}

// 貸方部門ターミネーション
// 入金・出金伝票は 両方ともヘッド部で部門をセットするため、この関数は呼ばない
//	int ln ... 画面伝票上の　行番号(１～最大７)
//
void CDnpBaseClass::OnTermCredBmn( int ln, long nChar )
{
	if( IsKind() != KIND_FURIKAE )	return;
	int vect;

	if( nChar == VK_UP || nChar == VK_DOWN )
	{
		int dset = 0;

		if( nChar == VK_UP && ln == 1 ) {
			dset = DnpLine_Chg(-1);
		}
		else if( nChar == VK_DOWN && ln == getSCROLL_NL() ) {
			dset = DnpLine_Chg(1);
		}

		if( !dset ) {
			// 行移動
			if( nChar == VK_UP ) {
				--ln;
				vect	=	-1;
			}
			else {
				++ln;
				vect	=	1;
			}

			//行移動エラー（アンダー／オーバー　フロー）
			if( set_position( ln, FD_CBMN_PN, vect ) == -1 ) {
				if( nChar == VK_UP )	set_focus( DP_MM_PN );
				else {
					set_position( 1, FD_CBMN_PN );
				}
			}
		}
		return;
	}
	// 科目切替
	nChar = SelBmonDsp( nChar );
	if( ! nChar )	return;

	// カーソルセット
	switch( nChar )
	{
	case VK_RIGHT:
	case VK_RETURN:
	case VK_TAB:
//	case VK_F3:
	case VK_DELETE:
		if( m_DI.IsKoji() ) {
			set_position( ln, FD_CKOJI_PN );
		}
		else {
			if( IsAutoSiwake( ln ) ) {
				if( IsType() == TYPE_TANDOKU )
					set_position( ln, FD_TKY_PN );
				else
					set_position( ln, FD_CRVAL_PN );
			}
			else {
				set_position( ln, FD_CRED_PN );
			}
		}
		break;
	case VK_LEFT:
	case VK_F2:
		set_position( ln, FD_TKY_PN );
		break;
	default:
		Buzzer();
		break;
	}
}

// 貸方工事ターミネーション
// 入金・出金伝票は 両方ともヘッド部で工事をセットするため、この関数は呼ばない
//	int ln ... 画面伝票上の　行番号(１～最大７)
//
void CDnpBaseClass::OnTermCredKoji( int ln, long nChar )
{
	if( IsKind() != KIND_FURIKAE )	return;
	int vect;

	if( nChar == VK_UP || nChar == VK_DOWN )
	{
		int dset = 0;

		if( nChar == VK_UP && ln == 1 ) {
			dset = DnpLine_Chg(-1);
		}
		else if( nChar == VK_DOWN && ln == getSCROLL_NL() ) {
			dset = DnpLine_Chg(1);
		}

		if( !dset ) {
			// 行移動
			if( nChar == VK_UP ) {
				--ln;
				vect	=	-1;
			}
			else {
				++ln;
				vect	=	1;
			}

			//行移動エラー（アンダー／オーバー　フロー）
			if( set_position( ln, FD_CKOJI_PN, vect ) == -1 ) {
				if( nChar == VK_UP )	set_focus( DP_MM_PN );
				else {
					set_position( 1, FD_CKOJI_PN );
				}
			}
		}
		return;
	}
#ifdef OLD_CLOSE
	// 工事切替
	nChar = SelKojiDsp( nChar );
	if( ! nChar )	return;
#endif
	// カーソルセット
	switch( nChar )
	{
	case VK_RIGHT:
	case VK_RETURN:
	case VK_TAB:
//	case VK_F3:
	case VK_DELETE:
		if( IsAutoSiwake( ln ) ) {
			if( IsType() == TYPE_TANDOKU )
				set_position( ln, FD_TKY_PN );
			else
				set_position( ln, FD_CRVAL_PN );
		}
		else {
			set_position( ln, FD_CRED_PN );
		}
		break;
	case VK_LEFT:
	case VK_F2:
		if( set_position( ln, FD_CBMN_PN ) == -1 )
			set_position( ln, FD_TKY_PN );
		break;
	default:
		Buzzer();
		break;
	}
}


// 貸方科目ターミネーション
//	int ln ... 画面伝票上の　行番号(１～最大７)
void CDnpBaseClass::OnTermCred( int ln, long nChar )
{
	int vect;
	int dt_ln = get_dataline( ln );

	if( nChar == VK_UP || nChar == VK_DOWN )
	{
		int dset = 0;

		if( nChar == VK_UP && ln == 1 ) {
			dset = DnpLine_Chg(-1);
		}
		else if( nChar == VK_DOWN && ln == getSCROLL_NL() ) {
			dset = DnpLine_Chg(1);
		}

		if( !dset ) {
			// 行移動
			if( nChar == VK_UP ) {
				--ln;
				vect	=	-1;
			}
			else {
				++ln;
				vect	=	1;
			}
			//行移動エラー（アンダー／オーバー　フロー）
			if( set_position( ln, FD_CRED_PN, vect ) == -1 ) {
				if( nChar == VK_UP )	set_focus( DP_MM_PN );
				else {
					if( IsKind() == KIND_NYUKIN )	set_position( 1, FD_CRED_PN );
					else							set_position( 1, FD_DBVAL_PN );
				}
			}	
		}
		return;
	}

	// カーソルセット
	switch( nChar )
	{
	case VK_RIGHT:
		if( IsKind() == KIND_NYUKIN ) {
			set_position( ln, FD_TKY_PN );
			break;
		}
		else if( IsKind() == KIND_FURIKAE ) {
			set_position( ln, FD_CSKBN_PN );
			break;
		}

	case VK_TAB:
	case VK_RETURN:
//	case VK_F3:
	case VK_DELETE:
		if( IsKind() == KIND_FURIKAE ) {

			vect = (nChar == VK_RETURN ) ? 1 : 2;

			if( IsKamokuSyattr(ln, FD_CRED_PN) ) {
				m_SET_SYZINP_PN = m_pSyzInp->CheckSyzPn(vect);
			}
			else {
				m_SET_SYZINP_PN = -1;
			}

			if( m_SET_SYZINP_PN >= 0 ) {
				set_position( ln, FD_CSKBN_PN );
			}
			else {
				if( ! SyogSiwakeMode() || ! IsScanLine( dt_ln )  ) {	//諸口仕訳詰める。
					if( IsType() == TYPE_TANDOKU ) {

						if( SyzStopMode() && IsKamokuSyattr(ln, FD_CRED_PN) ) {
							set_position(ln, FD_CSKBN_PN);
						}
						else {
							//外税免税
							if( _getCND(dt_ln, FD_CRZEI_PN)->ETC_sg[0] ) {
								set_position(ln, FD_CRVAL_PN);
							}
							else {
								if( _getCND(dt_ln, FD_CRVAL_PN)->INP_sg == FALSE )
									set_position(ln, FD_CRVAL_PN);
								else
									set_position(ln, FD_TKY_PN);
							}
						}
					}
					else {
						if( nChar == VK_DELETE && _getCND( dt_ln, FD_CRVAL_PN )->INP_sg == FALSE ) {
							set_position( ln, FD_TKY_PN );
						}
						else {
							if( SyzStopMode() && IsKamokuSyattr(ln, FD_CRED_PN) ) {
								set_position(ln, FD_CSKBN_PN);
							}
							else {
								set_position(ln, FD_CRVAL_PN);
							}
						}
					}
				}
				else {
					if( IsScanLine( dt_ln ) ) {
						if( FuriSyogPosition( dt_ln, FD_CRED_PN ) ) {
							set_position( ln, FD_CRVAL_PN );
						}
						else {
							if( m_DPscan.IsTanitsuSiwake( dt_ln ) )	{
								set_position( ln, FD_TKY_PN );
							}
							else {
								if( nChar == VK_DELETE && _getCND( dt_ln, FD_CRVAL_PN )->INP_sg == FALSE ) {
									set_position( ln, FD_TKY_PN );
								}
								else {
									if( SyzStopMode() && IsKamokuSyattr(ln, FD_CRED_PN) ) {
										set_position(ln, FD_CSKBN_PN);
									}
									else {
										set_position(ln, FD_CRVAL_PN);
									}
								}
							}
						}
					}
				}
			}
		}
		else if( IsKind() == KIND_NYUKIN )	set_position( ln, FD_CRVAL_PN );

		break;
	case VK_LEFT:
	case VK_F2:
		if( IsKind() == KIND_FURIKAE ) {
			if( m_DI.IsKoji() && (m_FuriMode == FURI_DATABMON) )
				set_position( ln, FD_CKOJI_PN );
			else if( m_DI.IsBumon() && (m_FuriMode == FURI_DATABMON) )	
				set_position( ln, FD_CBMN_PN );
			else
				set_position( ln, FD_TKY_PN );
		}
		else {
			if( (nChar == VK_F2 || nChar == VK_LEFT) && ln &&
								_getCND( dt_ln - 1, FD_CSKBN_PN )->INP_sg == TRUE ) {
				if( set_position( ln - 1, FD_CSKBN_PN, -1 ) == -1 ) {
					if( m_DI.IsKoji() )
						set_focus( DP_KOJI_PN );
					else if( m_DI.IsBumon() )
						set_focus( DP_BMN_PN );
					else
						set_focus( DP_DD_PN );
				}
			}
			else {
				if( set_position( ln - 1, FD_CRVAL_PN, -1 ) == -1 ) {
					if( m_DI.IsKoji() )
						set_focus( DP_KOJI_PN );
					else if( m_DI.IsBumon() )
						set_focus( DP_BMN_PN );
					else
						set_focus( DP_DD_PN );
				}
			}
		}
		break;
	default:
		Buzzer();
		break;
	}
}

// 借方諸口ターミネーション
//	int ln ... 画面伝票上の　行番号(１～最大７)
void CDnpBaseClass::OnTermDsyog( int ln, long nChar )
{
	int vect;

	if( IsKind() != KIND_FURIKAE )
		return;

	if( nChar == VK_UP || nChar == VK_DOWN )
	{
		int dset = 0;

		if( nChar == VK_UP && ln == 1 ) {
			dset = DnpLine_Chg(-1);
		}
		else if( nChar == VK_DOWN && ln == getSCROLL_NL() ) {
			dset = DnpLine_Chg(1);
		}

		if( !dset ) {
			// 行移動
			if( nChar == VK_UP ) {
				--ln;
				vect	=	-1;
			}
			else {
				++ln;
				vect	=	1;
			}

			//行移動エラー（アンダー／オーバー　フロー）
			if( set_position( ln, FD_DSYOG_PN, vect ) == -1 ) {
				if( nChar == VK_UP )	set_focus( DP_MM_PN );
				else {
					set_position( 1, FD_DSYOG_PN );
				}
			}
		}
		return;
	}

	// 諸口枝番切替
	nChar = SelItemDsp( nChar );
	if( ! nChar )	return;

	// カーソルセット
	switch( nChar )
	{
	case VK_RIGHT:
		set_position( get_nowln(), FD_TKY_PN );
		break;

	case VK_RETURN:
	case VK_TAB:
//	case VK_F3:
	case VK_DELETE:
		if( FukugoLine(1) ) {
			set_position( get_nowln(), FD_CSYOG_PN );
			break;
		}
		else {
			if( set_position( ++ln, FD_DBVAL_PN ) == -1 ) {
				if( DnpLine_Chg(1,1) ) {
					set_position( (ln-1), FD_DBVAL_PN );
				}
				else {
					set_position( 1, FD_DBVAL_PN );
				}
			}
		}
		break;

	case VK_LEFT:
	case VK_F2:
		set_position( get_nowln(), FD_DEBT_PN );
		break;
	case 0:
		break;
	default:
		Buzzer();
		break;
	}
}


// 貸方諸口ターミネーション
//	int ln ... 画面伝票上の　行番号(１～最大７)
void CDnpBaseClass::OnTermCsyog( int ln, long nChar )
{
	int vect;

	if( IsKind() != KIND_FURIKAE )
		return;

	if( nChar == VK_UP || nChar == VK_DOWN )
	{
		int dset = 0;

		if( nChar == VK_UP && ln == 1 ) {
			dset = DnpLine_Chg(-1);
		}
		else if( nChar == VK_DOWN && ln == getSCROLL_NL() ) {
			dset = DnpLine_Chg(1);
		}

		if( !dset ) {
			// 行移動
			if( nChar == VK_UP ) {
				--ln;
				vect	=	-1;
			}
			else {
				++ln;
				vect	=	1;
			}
			//行移動エラー（アンダー／オーバー　フロー）
			if( set_position( ln, FD_CSYOG_PN, vect ) == -1 ) {
				if( nChar == VK_UP )	set_focus( DP_MM_PN );
				else {
					set_position( 1, FD_CSYOG_PN );
				}
			}
		}
		return;
	}

	// カーソルセット
	switch( nChar )
	{
	case VK_RIGHT:
		vect = (nChar == VK_RETURN ) ? 1 : 2;

		m_SET_SYZINP_PN = m_pSyzInp->CheckSyzPn( vect );

		if( m_SET_SYZINP_PN >= 0 ) {
			set_position( ln, FD_CSKBN_PN );
		}
		else {
			set_position( ln, FD_CRVAL_PN );
		}
		break;

	case VK_TAB:
	case VK_RETURN:
//	case VK_F3:
	case VK_DELETE:
		if( set_position( ++ln, FD_DBVAL_PN ) == -1 ) {
			if( DnpLine_Chg(1,1) ) {
				set_position( (ln-1), FD_DBVAL_PN );
			}
			else {
				set_position( 1, FD_DBVAL_PN );
			}
		}

		break;
	case VK_LEFT:
	case VK_F2:
		set_position( get_nowln(), FD_CRED_PN );
		break;

	default:
		Buzzer();
		break;
	}
}



// 借方ターミネーション
//	int ln ... 画面伝票上の　行番号(１～最大７)
void CDnpBaseClass::OnTermDebt( int ln, long nChar )
{
	int vect, dt_ln;
	dt_ln = get_dataline( ln );

	if( nChar == VK_UP || nChar == VK_DOWN )
	{
		int dset = 0;

		if( nChar == VK_UP && ln == 1 ) {
			dset = DnpLine_Chg(-1);
		}
		else if( nChar == VK_DOWN && ln == getSCROLL_NL() ) {
			dset = DnpLine_Chg(1);
		}

		if( !dset ) {
			// 行移動
			if( nChar == VK_UP ) {
				--ln;
				vect	=	-1;
			}
			else {
				++ln;
				vect	=	1;
			}
			//行移動エラー（アンダー／オーバー　フロー）
			if( set_position( ln, FD_DEBT_PN, vect ) == -1 ) {
				if( nChar == VK_UP )	set_focus( DP_MM_PN );
				else {
					set_position( 1, FD_DEBT_PN );
				}
			}
		}
		return;
	}

	// カーソルセット
	switch( nChar )
	{
	case VK_RIGHT:
		if( IsKind() == KIND_FURIKAE || IsKind() == KIND_SYUKKIN ) {
			set_position( get_nowln(), FD_TKY_PN );
			break;
		}
		// go through
	case VK_RETURN:
	case VK_TAB:
//	case VK_F3:
	case VK_DELETE:
		if( IsKind() == KIND_SYUKKIN ) {
			set_position( get_nowln(), FD_DBVAL_PN );
		}
		else if( IsKind() == KIND_FURIKAE ) {
			// 借方消費税で対価等があるか
			if( nChar != VK_DELETE ) {
				vect = (nChar == VK_RETURN ) ? 1 : 2;

				if( _getCND( dt_ln, FD_DSKBN_PN )->INP_sg ) {
					m_SET_SYZINP_PN = m_pSyzInp->CheckSyzPn( vect );
				}
				else {
					m_SET_SYZINP_PN = -1;
				}
				if( m_SET_SYZINP_PN == -1 ) {
					//外税・免税事業者
					if( _getCND(dt_ln, FD_DBZEI_PN)->ETC_sg[0] && _getCND(dt_ln, FD_DEBT_PN)->ETC_sg[1] != FD_DBZEI_PN ) {
						if( _getCND(dt_ln, FD_DBVAL_PN)->INP_sg ) {
							//借方科目欄から移動したサインON
							_getCND(dt_ln, FD_DBZEI_PN)->ETC_sg[1] = FD_DEBT_PN;
							_getCND(dt_ln, FD_DBZEI_PN)->ETC_sg[2]++;
							set_position(get_nowln(), FD_DBZEI_PN);
							break;
						}
					}
				}

				if( SyzStopMode() && IsKamokuSyattr(ln, FD_DEBT_PN) ) {
					//借・貸で同じ消費税科目
					if( !IsSameFuriSyzKbn(ln) ) {
						if( m_SET_SYZINP_PN == -1 ) {
							m_SET_SYZINP_PN = SY_SZKBN_PN;
						}
					}
				}

				if( m_SET_SYZINP_PN >= 0 ) {
					set_position( get_nowln(), FD_DSKBN_PN );
				}
				else {
					if( IsAutoSiwake( get_nowln() ) ) {
						set_position( get_nowln(), FD_CRED_PN );
					}
					else {
						if( set_position( get_nowln(), FD_CBMN_PN ) == -1 ) {
							if( set_position( get_nowln(), FD_CKOJI_PN ) == -1 ) {
								set_position( get_nowln(), FD_CRED_PN );
							}
						}
					}
				}
			}
			else {
				if( set_position( get_nowln(), FD_CBMN_PN ) == -1 ) {
					if( set_position( get_nowln(), FD_CKOJI_PN ) == -1 ) {
						set_position( get_nowln(), FD_CRED_PN );
					}
				}
			}
		}
		break;
	case VK_LEFT:
	case VK_F2:

		if( IsKind() == KIND_SYUKKIN ) {
			if( (nChar == VK_F2 || nChar == VK_LEFT) && ln > 1 &&
								_getCND( dt_ln - 1, FD_DSKBN_PN )->INP_sg == TRUE ) {
				if( set_position( ln - 1, FD_DSKBN_PN, -1 ) == -1 ) {
					if( m_DI.IsKoji() ) {
						set_focus( DP_KOJI_PN );
					}
					else if( m_DI.IsBumon() ) {
						set_focus( DP_BMN_PN );
					}
					else {
						set_focus( DP_DD_PN );
					}
				}
			}
			else {
				if( set_position( ln - 1, FD_DBVAL_PN, -1 ) == -1 ) {	//1行上のデータへ
					if( m_DI.IsKoji() ) {
						set_focus( DP_KOJI_PN );
					}
					else if( m_DI.IsBumon() ) {
						set_focus( DP_BMN_PN );
					}
					else {
						set_focus( DP_DD_PN );
					}
				}
			}
		}
		else if( IsKind() == KIND_FURIKAE ) {
			if( set_position( get_nowln(), FD_DKOJI_PN ) == -1 ) {
				if( set_position( get_nowln(), FD_DBMN_PN ) == -1 ) {
					set_position( get_nowln(), FD_DSKBN_PN );
				}
			}
		}
		break;
	case 0:
		break;
	default:
		Buzzer();
		break;
	}
}

// 借方工事ターミネーション
// 入金・出金伝票は 両方ともヘッド部で部門をセットするため、この関数は呼ばない
//	int ln ... 画面伝票上の　行番号(１～最大７)
//
void CDnpBaseClass::OnTermDebtKoji( int ln, long nChar )
{
	if( IsKind() != KIND_FURIKAE )	return;
	int vect;

	if( nChar == VK_UP || nChar == VK_DOWN )
	{
		int dset = 0;

		if( nChar == VK_UP && ln == 1 ) {
			dset = DnpLine_Chg(-1);
		}
		else if( nChar == VK_DOWN && ln == getSCROLL_NL() ) {
			dset = DnpLine_Chg(1);
		}

		if( !dset ) {
			// 行移動
			if( nChar == VK_UP ) {
				--ln;
				vect	=	-1;
			}
			else {
				++ln;
				vect	=	1;
			}

			//行移動エラー（アンダー／オーバー　フロー）
			if( set_position( ln, FD_DKOJI_PN, vect ) == -1 ) {
				if( nChar == VK_UP )	set_focus( DP_MM_PN );
				else {
					set_position( 1, FD_DKOJI_PN );
				}
			}
		}
		return;
	}

#ifdef OLD_CLOSE
	// 工事切替
	nChar = SelKojiDsp( nChar );
	if( ! nChar )	return;
#endif
	// カーソルセット
	switch( nChar )
	{
	case VK_RIGHT:
	case VK_RETURN:
	case VK_TAB:
//	case VK_F3:
	case VK_DELETE:
		if( IsAutoSiwake( ln ) ) {
			if( m_DI.IsBumon() && (m_FuriMode == FURI_DATABMON) )
				set_position( ln, FD_CBMN_PN );
			else if( m_DI.IsKoji() && (m_FuriMode == FURI_DATABMON) )
				set_position( ln, FD_CKOJI_PN );
			else {
				if( IsType() == TYPE_TANDOKU ) {
					set_position( ln, FD_TKY_PN );
				}
				else {
					set_position( ln, FD_CRVAL_PN );
				}
			}
		}
		else {
			set_position( ln, FD_DEBT_PN );
		}
		break;
	case VK_LEFT:
	case VK_F2:
		if( set_position( ln, FD_DBMN_PN ) == -1 ) {
			if( _getCND( ln, FD_DSKBN_PN )->INP_sg )
					set_position( ln, FD_DSKBN_PN );
			else	set_position( ln, FD_DBVAL_PN );
		}
		break;
	default:
		Buzzer();
		break;
	}
}


// 借方部門ターミネーション
// 入金・出金伝票は 両方ともヘッド部で部門をセットするため、この関数は呼ばない
//	int ln ... 画面伝票上の　行番号(１～最大７)
void CDnpBaseClass::OnTermDebtBmn( int ln, long nChar )
{
	if( IsKind() != KIND_FURIKAE )	return;
	int vect;

	if( nChar == VK_UP || nChar == VK_DOWN )
	{
		int dset = 0;

		if( nChar == VK_UP && ln == 1 ) {
			dset = DnpLine_Chg(-1);
		}
		else if( nChar == VK_DOWN && ln == getSCROLL_NL() ) {
			dset = DnpLine_Chg(1);
		}

		if( !dset ) {
			// 行移動
			if( nChar == VK_UP ) {
				--ln;
				vect	=	-1;
			}
			else {
				++ln;
				vect	=	1;
			}

			//行移動エラー（アンダー／オーバー　フロー）
			if( set_position( ln, FD_DBMN_PN, vect ) == -1 ) {
				if( nChar == VK_UP )	set_focus( DP_MM_PN );
				else {
					set_position( 1, FD_DBMN_PN );
				}
			}
		}
		return;
	}
	// 部門切替
	nChar = SelBmonDsp( nChar );
	if( ! nChar )	return;

	int dt_ln = get_dataline(ln);

	// カーソルセット
	switch( nChar )
	{
	case VK_RIGHT:
	case VK_RETURN:
	case VK_TAB:
//	case VK_F3:
	case VK_DELETE:
		if( set_position( ln, FD_DKOJI_PN ) == -1 ) {
			if( IsAutoSiwake( ln ) ) {
				if( m_DI.IsBumon()  && (m_FuriMode == FURI_DATABMON) )
					set_position( ln, FD_CBMN_PN );
				else if( m_DI.IsKoji() && (m_FuriMode == FURI_DATABMON) )
					set_position( ln, FD_CKOJI_PN );
				else {
					if( IsType() == TYPE_TANDOKU ) {
						set_position( ln, FD_TKY_PN );
					}
					else {
						set_position( ln, FD_CRVAL_PN );
					}
				}
			}
			else {
				set_position( ln, FD_DEBT_PN );
			}
		}
		break;
	case VK_LEFT:
	case VK_F2:
		if( _getCND(dt_ln, FD_DSKBN_PN )->INP_sg )
				set_position( ln, FD_DSKBN_PN );
		else	set_position( ln, FD_DBVAL_PN );
		break;
	default:
		Buzzer();
		break;
	}
}


// 工事　データＣＯＰＹ
//　int ln ... 画面伝票上の　行番号(１～最大７)
//
void CDnpBaseClass::Koji_Copy( int ln, int pn, CString& kno )
{
	if( pn < 0 )	return;
	struct _DenpData *prdata;

	int dt_ln = get_dataline(ln);

	// データ行の部門のみ
	if( pn < FD_DBVAL_PN )	return;

	// 伝票修正 は コピーしない [From管理課] <01.31 /07>
	BOOL bCopy = FALSE;
#ifdef CLOSE // 01.21 /11
	if( IsJob() == JOB_INPUT &&  !_getCND( dt_ln, pn )->INP_sg )
#else
	if( ! _getCND( dt_ln, pn )->INP_sg )
#endif
		bCopy = TRUE;

	// 諸口詰めないで表示するとき
	if( FuriSyogPosition( dt_ln, pn ) )	return;

	if( bCopy && (prdata = _getPRDATA( dt_ln, pn )) ) {
		kno = prdata->DP_KOJI;
//		Koji_Inp( pn, 0, &kno );
	}
}


//--------------------------------------------------------
//	工事番号入力ターミネーション
//
//--------------------------------------------------------
void CDnpBaseClass::TerminationKojiDBedit( UINT id, long nChar, long inplen, long kst )
{
	if( ! m_PRO_OK )	return;

	if( ! m_DI.IsKoji() )	return;

	VARIANT var;
	int pn, kj_sgn, ln, dt_ln;
	CString kno, prev_kno, kjnam;
	CWnd* pDlgWnd;
	pDlgWnd = GetNowDnpDialog();
	pn	= get_nowpn();

	ln		= get_nowln();
	dt_ln	= get_dataline(ln);

	BOOL bModify_Ok;
	bModify_Ok = (_getCND( dt_ln, pn )->EDT_sg <= 1);
	if( m_DI.IsConFirmMaster() )	bModify_Ok = FALSE;

	if( pn == DP_KOJI_PN ) {
		if( IsTransDenpyo()	)	bModify_Ok = FALSE;
	}

	DBdata_get( pDlgWnd, id, &var, ICSDBEDT_TYPE_STRING, 0 );
	if( var.pbVal != NULL ) {
		kno = var.pbVal;
	}
//MyTrace( "TerminationKojiDBedit kno = %s inplen = %d\n", kno, inplen );

	if( bModify_Ok ) {
		prev_kno = getDATA( pn )->DP_KOJI;

		// 2桁入力 は、選択画面からか？
		if( inplen > 0 ) {
			CString input;
			input = kno;
			
			if( Koji_Check( pn, nChar, &kno, &input ) ) {
				//選択画面から入力
				var.pbVal = (BYTE*)(LPCTSTR)kno;
			 	DBdata_set( pDlgWnd, id, &var, ICSDBEDT_TYPE_STRING, 0 );
			}
			else {
				if( kno.IsEmpty() ) {
					var.pbVal = NULL;
			 		DBdata_set( pDlgWnd, id, &var, ICSDBEDT_TYPE_STRING, 0 );
				}
			}

			_getCND( dt_ln, pn )->INP_sg = TRUE;
		}

		if( nChar == VK_RETURN || ! nChar ) {
			// 工事コピー
			if( nChar == VK_RETURN && ! _getCND( dt_ln, pn )->INP_sg )
				Koji_Copy( ln, pn, kno );

			trim_zero( kno.GetBuffer(128), 128, kno.GetLength() );
			kno.ReleaseBuffer();

			if( kno.IsEmpty() ) {
				kno = _getDATA( dt_ln, pn )->DP_KOJI;
			}
			else {
				int n;
				n = kno.GetLength();
				::ZeroMemory( _getDATA( dt_ln, pn )->DP_KOJI, sizeof _getDATA( dt_ln, pn )->DP_KOJI );
				memcpy( _getDATA( dt_ln, pn )->DP_KOJI, kno, n );
			}

			struct _SET_KMKETC sk;
			sk.bmn = -1;
			sk.code.Empty();
			sk.kno = kno;
			sk.brn = -1;

			kj_sgn = 0;
			//工事名称を表示する場合
			if( GetKojiEtc( &sk ) ) {
				strcpy_s( getCND(pn)->ETC_msg, sizeof getCND(pn)->ETC_msg, sk.etc );
				getCND(pn)->ETC_col = sk.etc_col;
				kj_sgn = 1;

				GetNowDnpDialog()->SetKoji( ln, pn, kno, &sk );
			}
			else {
				::ZeroMemory( getCND(pn)->ETC_msg, sizeof getCND(pn)->ETC_msg );
				getCND(pn)->ETC_col = 0;

				GetNowDnpDialog()->SetKoji( ln, pn, kno, NULL );
			}
			// 入力サイン ON
			if( nChar || (!nChar && ! kno.IsEmpty() ) )
				_getCND( dt_ln, pn )->INP_sg = TRUE;

			// 工事名称を摘要へ
			if( ! kj_sgn ) {
				if( _set_kojietc( &sk ) )
					kj_sgn = 1;
			}
			if( kno != prev_kno && kj_sgn ) {
				bmnkoji_to_tekiyo( &sk.etc, 1 );
			}
		}

		if( nChar == VK_DELETE ) {
			var.pbVal = NULL;
			DBdata_set( pDlgWnd, id, &var, ICSDBEDT_TYPE_STRING, 0 );

			::ZeroMemory( _getDATA( dt_ln, pn )->DP_KOJI, sizeof _getDATA( dt_ln, pn )->DP_KOJI );
			_getCND( dt_ln, pn )->INP_sg = FALSE;
			kno.Empty();
			GetNowDnpDialog()->SetKoji( ln, pn, kno, NULL );
		}
	}
	else {
		if( nChar == 0x16 ) {
			prev_kno = getDATA(pn)->DP_KOJI;

			if( prev_kno != kno ) {
				char data[64] = { 0 };
				set_codestring(data, sizeof data, (char*)(LPCTSTR)prev_kno, m_pZm->zvol->kjcol);
				var.pbVal = (BYTE*)data;
				DBdata_set(pDlgWnd, id, &var, ICSDBEDT_TYPE_STRING, 0);
			}
		}
	}

	// 工事切替
	if( nChar != 0 ) {
		nChar = SelKojiDsp( nChar );
	}

	if( nChar != 0 ) {
		// ターミネーション処理
		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			nChar = VK_F2;

		ToViewTermJob( ln, pn, nChar );
	}
}


/*=========================================
	摘要セットデータ表示
	BOOL bTKcut [TRUE: 摘要をセットしない。]
  =========================================*/
void CDnpBaseClass::tky_select_dsp( CDBINPDataRec *drec, struct _DP_DATA_LINE *pdat, int dc_sw, CDBipTKREC *ptk, BOOL bTKcut )
{
struct _DenpData ins_data={0};
struct _DenpData *debt, *cred, *keybuf;

SETMEISYO	sm = {0};
struct _SET_KMKETC sk;
int pn, pos;
struct _DNPLINE_CND* pCnd;
char	edabf[32] = {0};

	//摘要枝番表示
	if( dc_sw & 0x01 )
	{
		//借方枝番表示
		pn = FD_DEBT_PN;
		debt = _getDATAs( pdat, pn );
		pCnd = _getCNDs( pdat, pn );

		if( m_DI.IsBumon() ) {
			if( IsKind() == KIND_FURIKAE ) {
				keybuf = _getDATAs( pdat, FD_DBMN_PN );
			}
			else {
				keybuf = getDATA( DP_BMN_PN );
			}
			sk.bmn = keybuf->DP_BMN;
		}
		else {
			sk.bmn = -1;
		}
		sk.code = debt->DP_CdNum;
		sk.brn  = debt->DP_BRN;

		if( m_DI._set_kmketc( &sk ) ) {
			strcpy_s( pCnd->ETC_msg, sizeof pCnd->ETC_msg, sk.etc );
			pCnd->ETC_col = sk.etc_col;
			
			strcpy_s( sm.sm_name, sizeof sm.sm_name, debt->DP_stKMK.kd_name );
			strcpy_s( sm.sm_etc, sizeof sm.sm_etc, pCnd->ETC_msg );
			sm.sm_etccol = pCnd->ETC_col;
		}
		else {
			set_codestr( edabf, sizeof edabf, debt->DP_stKMK.kd_eda, m_pZm->zvol->edcol );
			sprintf_s( sm.sm_name, sizeof sm.sm_name, "%.14s%s", debt->DP_stKMK.kd_name, edabf );
		}

		GetNowDnpDialog()->SetKnam( get_nowln(), pn, &sm );
	}
	if( dc_sw & 0x02 )
	{
		//貸方枝番表示
		pn = FD_CRED_PN;
		cred = _getDATAs( pdat, pn );
		pCnd = _getCNDs( pdat, pn );

		if( m_DI.IsBumon() ) {
			if( IsKind() == KIND_FURIKAE ) {
				keybuf = _getDATAs( pdat, FD_CBMN_PN );
			}
			else {
				keybuf = getDATA( DP_BMN_PN );
			}
			sk.bmn = keybuf->DP_BMN;
		}
		else {
			sk.bmn = -1;
		}
		sk.code = cred->DP_CdNum;
		sk.brn  = cred->DP_BRN;

		if( m_DI._set_kmketc( &sk ) ) {
			strcpy_s( pCnd->ETC_msg, sizeof pCnd->ETC_msg, sk.etc );
			pCnd->ETC_col = sk.etc_col;
			
			strcpy_s( sm.sm_name, sizeof sm.sm_name, cred->DP_stKMK.kd_name );
			strcpy_s( sm.sm_etc, sizeof sm.sm_etc, pCnd->ETC_msg );
			sm.sm_etccol = pCnd->ETC_col;
		}
		else {
			set_codestr( edabf, sizeof edabf, cred->DP_stKMK.kd_eda, m_pZm->zvol->edcol );
			sprintf_s( sm.sm_name, sizeof sm.sm_name, "%.14s%s", cred->DP_stKMK.kd_name, edabf );
		}

		GetNowDnpDialog()->SetKnam( get_nowln(), pn, &sm );
	}

	//仕訳摘要表示
	char tkstr[128] = {0};
//	char tmp[128];
	VARIANT var;
	strcpy_s( tkstr, sizeof tkstr, ptk->m_tkname );

	// 枝番摘要を 仕訳摘要に
	if( get_nowpn() == FD_TKY_PN && ! bTKcut ) {
		var.pbVal = (BYTE*)tkstr;
		CWnd* pwnd;
		pwnd = GetNowDnpDialog();

		CICSDBEDT* pDedt;
		pDedt = (CICSDBEDT*)(GetNowDnpDialog()->GetDlgItem( IDC_ICSDBEDT8CTRL1 ));
		pos = pDedt->GetCaretPosition();
		pDedt->InsertData( &var, ICSDBEDT_TYPE_STRING, 0, pos );
	}
	else {
#ifdef KEEP_CLOSE
		strcpy_s( tmp, sizeof tmp, (const char*)_getDATAs( pdat, FD_TKY_PN )->DP_TKY );
		strcat_s( tmp, sizeof tmp, tkstr );

		int n = get_strcnt( tmp, (m_pZm->zvol->tk_ln * 2) );
		tmp[n] = '\0';

		::ZeroMemory( _getDATAs( pdat, FD_TKY_PN )->DP_TKY, sizeof _getDATAs( pdat, FD_TKY_PN )->DP_TKY );
		memcpy( _getDATAs( pdat, FD_TKY_PN )->DP_TKY, tmp, n );
#endif
	}

	//消費税再表示
	int kind;
	kind = IsKind();

	if( IsKind() == KIND_FURIKAE ) {
		int ln = get_nowln();
		int dt_ln = get_dataline( ln );
		int furi_tan = 1;

		if( IsScanLine( dt_ln )) {
			if( ! m_DPscan.IsFukugoSiwake( dt_ln ) ) {
				furi_tan = 1;	//単一仕訳
			}
		}

		int	put_sg[2] = {0};
		if( furi_tan ) {
			if( ! FuriSyogPosition( dt_ln, FD_DSKBN_PN ) )	put_sg[0] = 1;
			if( ! FuriSyogPosition( dt_ln, FD_CSKBN_PN ) )	put_sg[1] = 1;
		}

		keybuf = _getDATAs( pdat, FD_DSKBN_PN );
		SRCTrace;

		PutsSyz( get_nowln(), FD_DSKBN_PN, keybuf->SYDATA.SKBNtxt, put_sg[0] ? keybuf->SYDATA.sy_chg : -1 );

		keybuf = _getDATAs( pdat, FD_CSKBN_PN );

		PutsSyz( get_nowln(), FD_CSKBN_PN, keybuf->SYDATA.SKBNtxt, put_sg[1] ? keybuf->SYDATA.sy_chg : -1 );
	}
	else {
		SRCTrace;
		pn = (kind == KIND_SYUKKIN) ? FD_DSKBN_PN : FD_CSKBN_PN;
		keybuf = _getDATAs( pdat, pn );
		
		PutsSyz( get_nowln(), pn, keybuf->SYDATA.SKBNtxt, keybuf->SYDATA.sy_chg );
	}
}



//--------------------------------------------------------
//	摘要入力ターミネーション
//
//--------------------------------------------------------
void CDnpBaseClass::TerminationTekiyo( UINT id, long nChar, long inplen, long kst )
{
	if( ! m_PRO_OK )	return;

	//摘要にカーソルがある状態でタスクバーで別アプリに切り替えると、
	//次回検索時に、フォーカスOFFが来る場合があった [02.08 /16]
	if( nChar == 0 && get_nowpn() != FD_TKY_PN )
		return;

	VARIANT var;
	char buf[128] = {0};
	int pos;
	CWnd* pDlgWnd;
	pDlgWnd = GetNowDnpDialog();

	BOOL bModify_Ok;
	bModify_Ok = (getCND(FD_TKY_PN )->EDT_sg <= 1);
	if( m_DI.IsConFirmMaster() )	bModify_Ok = FALSE;

	DBdata_get( pDlgWnd, id, &var, ICSDBEDT_TYPE_STRING, 0 );
	if( var.pbVal != NULL ) {
		strcpy_s( buf, sizeof buf, (char*)var.pbVal );
	}

	teki_stringcheck( buf, m_pZm->zvol->tk_ln * 2 );

	if( nChar ) {
		//適要選択欄ページめくり
		nChar = SelTkyDsp( nChar );
		if( !nChar )
			return;
	}

	if( ! nChar ) {
		GetNowDnpDialog()->FocusTkyColor( get_nowln(), FALSE );
	}

	if( bModify_Ok ) {
		if( nChar == VK_DELETE ) {
			//カーソルポジション以降の文字列取り消し
			pos = ((CICSDBEDT*)pDlgWnd->GetDlgItem( id ))->GetCaretPosition();
			buf[pos] = '\0';
			var.pbVal = (BYTE*)buf;
			DBdata_set( pDlgWnd, id, &var, ICSDBEDT_TYPE_STRING, 0 );
			if( pos == 0 )	getCND(FD_TKY_PN)->INP_sg = FALSE;
			// 先頭から削除は、フォーカスを次へ
			if( pos == 0 )
				nChar = VK_TAB;
		}
		else if( nChar == VK_TAB ) {
			//TABキーでの1文字コピー
			if( IsJob() == JOB_INPUT ) {
				struct _DenpData *pd;
				pd = _getPRDATA( get_dataline()/*get_nowln()*/, FD_TKY_PN );
#ifdef LATER_CLOSE
				if( pd != NULL ) {
					memcpy( buf, pd->DP_TKY, sizeof pd->DP_TKY );
					if( tekyo_tabcopy( pDlgWnd, IDC_ICSDBEDT8CTRL1, buf ) ) {
						DBdata_get( pDlgWnd, id, &var, ICSDBEDT_TYPE_STRING, 0 );
						if( var.pbVal != NULL ) {
							strcpy_s( buf, sizeof buf, (char*)var.pbVal );
						}
						nChar = 0;
					}
				}
#endif
			}
		}

		if( buf[0] ) {
			if( id == IDC_ICSDBEDT8CTRL1 ) {
				strcpy_s( (char*)getDATA(FD_TKY_PN)->DP_TKY, sizeof getDATA(FD_TKY_PN)->DP_TKY, buf );
				GetNowDnpDialog()->SetTekiyo( get_nowln(), FD_TKY_PN, buf );
				getCND(FD_TKY_PN)->INP_sg = TRUE;
			}
			else {
				// 貸方摘要
				if( IsJob() == JOB_SCAN ) {
					int idx = get_dataline()-1;
					tblTekiyo[idx] = buf;
				}
			}
		}
		else {
			int sgn = 0;
			if( nChar == VK_RETURN ) {
				// 前行の摘要をコピー
				CString str;

				if( TekiyoCopy( get_nowln(), str ) ) {
					// 実際に、データをセットするのは、nChar == 0 [KillFocus]
					var.pbVal = (BYTE*)(LPCTSTR)str;
					DBdata_set( pDlgWnd, id, &var, ICSDBEDT_TYPE_STRING, 0 );
					sgn = 1;
					nChar = 0;
				}
			}

			if( ! sgn ) {
				if( id == IDC_ICSDBEDT8CTRL1 ) { 
					::ZeroMemory( getDATA(FD_TKY_PN)->DP_TKY, sizeof getDATA(FD_TKY_PN)->DP_TKY );
					GetNowDnpDialog()->SetTekiyo( get_nowln(), FD_TKY_PN, NULL );
				}
				else {
					int idx = get_dataline()-1;
					tblTekiyo[idx].Empty();
				}
			}
		}
	}
	else {
		if( nChar == 0x16 ) {
			struct _DenpData *pd;
			pd = getDATA(FD_TKY_PN);

			if( pd != NULL ) {
				::ZeroMemory(buf, sizeof buf);
				memcpy(buf, pd->DP_TKY, sizeof pd->DP_TKY);
				var.pbVal = (BYTE*)(LPCTSTR)buf;
				DBdata_set(pDlgWnd, id, &var, ICSDBEDT_TYPE_STRING, 0);
			}
		}
	}

	// コピーコード・FocusOFFは キー移動なし
	if( nChar == 0x16 || nChar == 0 )
		return;

	if (nChar == VK_F2) {
		// 先頭文字に移動した。
		if (tekiyp_F2job()) {
			return;
		}
	}

	if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
		nChar = VK_F2;

	if( id == IDC_ICSDBEDT8CTRL1 ) {
		int idx = get_dataline()-1;

		if( ! tblTekiyo[idx].IsEmpty() ) {
			if( nChar == VK_RETURN || nChar == VK_TAB || nChar == VK_DOWN ) {
				pDlgWnd->PostMessage( WM_RETROFOCUS2, IDC_ICSDBEDT_KASITKY );
				return;
			}
		}
	}
	else {
		if( nChar == VK_F2 || nChar == VK_UP ) {
			pDlgWnd->PostMessage( WM_RETROFOCUS2, IDC_ICSDBEDT8CTRL1 );
			return;
		}
	}

	ToViewTermJob( get_nowln(), FD_TKY_PN, nChar );
}

// 摘要欄でのF2処理
BOOL CDnpBaseClass::tekiyp_F2job()	// ln = 該当ライン
{
	CDnpBaseDlg* dlg;
	UINT id = IDC_ICSDBEDT8CTRL1;

	dlg = GetNowDnpDialog();

	CWnd* pfocuswnd = dlg->GetFocus();
	CWnd* pwnd;

	if (pfocuswnd != NULL) {
		int f_id = pfocuswnd->GetDlgCtrlID();
		if (f_id == IDC_ICSDBEDT_KASITKY)
			id = f_id;
	}
	pwnd = dlg->GetDlgItem(id);

	ASSERT(pwnd != NULL);

	CICSDBEDT* pDBEDT;
	pDBEDT = (CICSDBEDT*)pwnd;

	// 摘要は先頭文字へ
	short pos = pDBEDT->GetCaretPosition();
	if (pos > 0) {
		pDBEDT->SetCaretPosition(0);
		return TRUE;
	}

	return FALSE;
}


//
//	文字入力を許可するか？
//
BOOL CDnpBaseClass::IsInputCharEnable()
{
	int ln, pn, valpn, kmkpn, dt_ln;
	CString dbt, cre;
	BOOL bRet = TRUE;
	ln = get_nowln();
	pn = get_nowpn();
	dt_ln = get_dataline( ln );

	// 確定マスター
	if( m_DI.IsConFirmMaster() )
		return FALSE;

//MyTrace( "IsInputCharEnable dt_ln = %d\n", dt_ln );


	// 確定仕訳か？
	if( _getCND( dt_ln, pn )->EDT_sg > 1 ) {
		return	FALSE;
	}

	//諸口詰めないモード
	if( FuriSyogPosition( dt_ln, pn ) )
		return FALSE;


	if( pn == FD_TGNUM_PN || pn == FD_TGDATE_PN ) {
#ifdef CLOSE	// 手形は、マウスでは、入力可能にする
		get_linekamokucode( ln, dbt, cre );
	
		bRet = FALSE;
		// 手形科目チェック
		if( ! dbt.IsEmpty() )
			bRet = IsBillCodeEx( dbt, m_pZm );
		if( ! bRet ) {
			if( ! cre.IsEmpty() )
				bRet = IsBillCodeEx( cre, m_pZm );
		}
#endif
		bRet = TRUE;
	}
	else if( pn == FD_HJIYU_PN ) {
		// 変動事由チェック
		if( IsHjiyuInput() )	bRet = TRUE;
		else					bRet = FALSE;
	}
	else if( pn == FD_DBZEI_PN || pn == FD_CRZEI_PN ) {
		valpn = (pn == FD_DBZEI_PN) ? FD_DBVAL_PN : FD_CRZEI_PN;
		kmkpn = (pn == FD_DBZEI_PN) ? FD_DEBT_PN : FD_CRED_PN;

		if( _getCND( dt_ln, valpn )->INP_sg ) {
			if( _getCND(dt_ln, pn)->ETC_sg[0] ) {
				bRet = TRUE;
			}
			else if( ! IsZeigkMdfy() || _getCND( dt_ln, pn )->INP_type == 2 )
				bRet = FALSE;
			else
				bRet = TRUE;
		}
		else { 
			bRet = FALSE;
		}
	}
	else if (pn == FD_DSYOG_PN || pn == FD_CSYOG_PN) {
		char dval[6] = { 0 };
		char cval[6] = { 0 };
		int ok_sgn = 0;

		if (_getCND(dt_ln, FD_DBVAL_PN)->INP_sg) {
			memcpy(dval, _getDATA(dt_ln, FD_DBVAL_PN)->DP_VAL, 6);
			if (pn == FD_DSYOG_PN) ok_sgn++;
		}
		if (_getCND(dt_ln, FD_CRVAL_PN)->INP_sg) {
			memcpy(cval, _getDATA(dt_ln, FD_CRVAL_PN)->DP_VAL, 6);
			if (pn == FD_CSYOG_PN) ok_sgn++;
		}

		if (ok_sgn) {
			if (memcmp(dval, cval, 6) == 0) {
				bRet = FALSE;
			}
			else {
				bRet = TRUE;
			}
		}
		else {
			bRet = FALSE;
		}
	}

	// 01.24 /08
	if( IsJob() == JOB_SCAN && tblDel[dt_ln-1] )	bRet = FALSE;

	return bRet;
}




//
void CDnpBaseClass::BmnIMEkeydownJob( long nChar )
{
	HIMC	hImc;
	char	buf[128] = {0};
	char	yomi[128] = {0};

	int pn = get_nowpn();
	UINT ID = 0;
	
	if( pn == FD_DBMN_PN || pn == FD_CBMN_PN ) {
		ID = IDC_ICSDBEDT_BMON;
	}

	if( ID == 0 )
		return;

	UINT vKey = 0;

	CWnd* pwnd = GetNowDnpDialog()->GetDlgItem( ID );

	if( nChar == VK_PROCESSKEY ) {
		vKey = ImmGetVirtualKey( pwnd->m_hWnd );
		hImc = ImmGetContext( pwnd->m_hWnd );

		ImmGetCompositionString( hImc, GCS_COMPSTR, buf, sizeof buf );
		ImmGetCompositionString( hImc, GCS_COMPREADSTR, yomi, sizeof yomi );

		if( buf[0] && yomi[0] ) {
			if( isdigitstr( (char*)(LPCTSTR)buf ) ) {
				if( vKey == VK_UP || vKey == VK_DOWN ) {
					ImmNotifyIME(hImc, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
					ImmReleaseContext(pwnd->m_hWnd, hImc);
					hImc = NULL;

					if( CheckImeEndBmon( nChar, buf ) != 0 )
						return;
					TerminationBmonDBedit( ID, vKey, 1, 0 );
				}
			}
		}
		if( hImc != NULL ) {
			ImmReleaseContext(pwnd->m_hWnd, hImc);
		}
	}
}


BOOL CDnpBaseClass::BmnKojiIMEcheck()
{
	HIMC	hImc;
	char	buf[128] = {0};
	char	yomi[128] = {0};

	int pn = get_nowpn();
	UINT ID = 0;
	
	if( pn == DP_BMN_PN || pn == FD_DBMN_PN || pn == FD_CBMN_PN ) {
		ID = IDC_ICSDBEDT_BMON;
	}
	else if(pn == DP_KOJI_PN || pn == FD_DKOJI_PN || pn == FD_CKOJI_PN ) {
		ID = IDC_ICSDBEDT_KOJI;
	}

	if( ID == 0 )
		return FALSE;

	BOOL ret = FALSE;

	CWnd* pwnd = GetNowDnpDialog()->GetDlgItem( ID );

	hImc = ImmGetContext( pwnd->m_hWnd );

	if( hImc != NULL ) {
		ImmGetCompositionString( hImc, GCS_COMPSTR, buf, sizeof buf );
		ImmGetCompositionString( hImc, GCS_COMPREADSTR, yomi, sizeof yomi );

		if( buf[0] && yomi[0] ) {
			ret = TRUE;
		}
		ImmReleaseContext(pwnd->m_hWnd,hImc);
	}
	return ret;
}


BOOL CDnpBaseClass::ButtonDeleteJob()
{
	int dt_ln = get_dataline();
	BOOL bRet = FALSE;

	CDnpBaseDlg* pDlg = GetNowDnpDialog();

	if( ! pDlg->dataline_input( dt_ln ) ) {

		if( IsJob() == JOB_SCAN ) {
			m_DPscan.DeleteDenpLine( dt_ln );
		}
		// 最終行
		int max = get_MaxLine();

		for( int dlin = dt_ln; dlin < max; dlin++ ) {
			memcpy( getLINE_DATA( dlin ), getLINE_DATA( dlin+1 ),
					sizeof(struct _DP_DATA_LINE) );
		}

		data1line_clear( max );

		_delLineMAP( dt_ln );
		delLineDelSgn( dt_ln );

		//画面の表示を更新する。
		pDlg->dataline_disp( (m_TopIdx+1) );

		// DBEditにもデータセット
		int pn = get_nowpn();
		char tmp[128] = {0};
		char tmp2[128] = { 0 };
		struct _KamokuData	kd = { 0 };

		int id = -1;
		int id2 = -1;
		switch( pn ) {
		case FD_DBMN_PN:
		case FD_CBMN_PN:
			set_codestr( tmp, sizeof tmp, _getDATA( dt_ln, pn )->DP_BMN, m_pZm->zvol->bmcol );
			id = IDC_ICSDBEDT_BMON;
			break;
		case FD_DKOJI_PN:
		case FD_CKOJI_PN:
			set_codestring( tmp, sizeof tmp, (char*)_getDATA( dt_ln, pn )->DP_KOJI, m_pZm->zvol->kjcol );
			id = IDC_ICSDBEDT_KOJI;
			break;
		case FD_TKY_PN:
			strcpy_s( tmp, sizeof tmp, (const char*)_getDATA( dt_ln, pn )->DP_TKY );
			id = IDC_ICSDBEDT8CTRL1;
			if( !tblTekiyo[dt_ln - 1].IsEmpty() ) {
				strcpy_s(tmp2, sizeof tmp2, (const char*)tblTekiyo[dt_ln - 1]);
				id2 = IDC_ICSDBEDT_KASITKY;
			}
			break;
		case FD_TGNUM_PN:
			strcpy_s( tmp, sizeof tmp, (char*)_getDATA( dt_ln, pn )->DP_SNUM );
			id = IDC_ICSDBEDT_TGNUM;
			break;
		case FD_DEBT_PN:
		case FD_CRED_PN:
			memcpy(&kd, &_getDATA(dt_ln, pn)->DP_stKMK, sizeof kd);
			id = IDC_ICSDBEDT_KMINP;
			break;
		}

		//画面再表示
		VARIANT var;
		if( id != -1 ) {
			if( id == IDC_ICSDBEDT_KMINP ) {
				DBkamokudata_set(pDlg, id, &kd, m_pZm);
			}
			else {
				var.pbVal = (BYTE*)tmp;
				DBdata_set( pDlg, id, &var, ICSDBEDT_TYPE_STRING, 0 );
			}
		}
		if( id2 != -1 ) {
			var.pbVal = (BYTE*)tmp2;
			DBdata_set(pDlg, id2, &var, ICSDBEDT_TYPE_STRING, 0);
		}

		bRet = TRUE;
	}

	return bRet;
}




//
//	伝票の１データバッファをクリア
//
void CDnpBaseClass::data1line_clear( int ln )
{
	int pn;

	for( pn = FD_DBVAL_PN; pn < FD_MAX_PN; ++pn ) {
		switch( pn ) {
		case	FD_DEBT_PN:	// 借方科目
		case	FD_CRED_PN:	// 貸方科目
			::ZeroMemory( &_getDATA( ln, pn )->DP_stKMK, sizeof (struct _KamokuData) );
			_getDATA( ln, pn )->DP_BRN = -1;
			break;

		case	FD_DSYOG_PN:	//借方諸口
		case	FD_CSYOG_PN:	//貸方諸口
			_getDATA( ln, pn )->DP_cBRN = -1;
			break;

		case	FD_DBVAL_PN:	// 借方金額
		case	FD_CRVAL_PN:	// 貸方金額
		case	FD_DBZEI_PN:	// 借方税額
		case	FD_CRZEI_PN:	// 貸方税額
			::ZeroMemory( _getDATA( ln, pn )->DP_VAL, sizeof(_getDATA( ln, pn )->DP_VAL) );
			break;
		case	FD_TKY_PN:		// 摘要
			::ZeroMemory( _getDATA( ln, pn )->DP_TKY, sizeof(_getDATA( ln, pn )->DP_TKY) );
			break;
		case	FD_DBMN_PN:		// 部門
		case	FD_CBMN_PN:
			_getDATA( ln, pn )->DP_BMN = -1;
			break;
		case	FD_DKOJI_PN:	// 工事番号
		case	FD_CKOJI_PN:
			::ZeroMemory( _getDATA( ln, pn )->DP_KOJI, sizeof(_getDATA( ln, pn )->DP_KOJI) );
			break;
		case	FD_DSKBN_PN:	// 消費税
		case	FD_CSKBN_PN:
			::ZeroMemory( _getDATA( ln, pn ), sizeof( struct _DenpData ));
			// 消費税区分
			_getDATA( ln, pn )->SYDATA.swk_sw = 0;
			_getDATA( ln, pn )->SYDATA.ritu_sw = 0;
			_getDATA( ln, pn )->SYDATA.zeikbn_sw = 0;
			_getDATA( ln, pn )->SYDATA.uri_sw = 0;
			_getDATA( ln, pn )->SYDATA.toku_sw = 0;
			_getDATA( ln, pn )->SYDATA.kts_sw = 0;
			// 消費税デフォルト
			_getDATA( ln, pn )->SYDATA.swk_def = 0;
			_getDATA( ln, pn )->SYDATA.ritu_def = 0;
			_getDATA( ln, pn )->SYDATA.zeikbn_def = 0;
			_getDATA( ln, pn )->SYDATA.uri_def = 0;
			_getDATA( ln, pn )->SYDATA.toku_def = 0;
			_getDATA( ln, pn )->SYDATA.kts_def = 0;
			break;

		case	FD_TGNUM_PN:	//手形番号
			::ZeroMemory( _getDATA( ln, pn )->DP_SNUM, sizeof _getDATA( ln, pn )->DP_SNUM );
			break;
		case	FD_TGDATE_PN:	//手形期日
			_getDATA( ln, pn )->DP_TGDAY = -1;
			::ZeroMemory( _getDATA( ln, pn )->DP_TGBCD, sizeof _getDATA( ln, pn )->DP_TGBCD );
			break;
		case	FD_HJIYU_PN:	//変動事由
			_getDATA( ln, pn )->DP_BMN = 0;
			break;

		case	FD_HUSEN_PN:	//付箋
			_getDATA( ln, pn )->DP_HUSEN = 0;
			::ZeroMemory( _getDATA( ln, pn )->DP_HCMNT, sizeof _getDATA( ln, pn )->DP_HCMNT );
			break;

		default:
			break;
		}

		//コンディションサイン OFF
		::ZeroMemory( (char *)_getCND( ln, pn ), sizeof(struct _DNPLINE_CND) );
	}

}


// 右ボタンクリック処理
BOOL CDnpBaseClass::MouseHookRbuttonJob(MOUSEHOOKSTRUCT *pmh)
{
	//画面再表示
	CDnpBaseDlg* pDlg = GetNowDnpDialog();

	BOOL bRet = FALSE;
	int ln = 0;

	if( m_DI.IsConFirmMaster() )	return FALSE;

	if( pmh->wHitTestCode == HTCLIENT && m_pDnpParent->IsChild(CWnd::FromHandle(pmh->hwnd)) ) {
		ln = pDlg->IsLine1Selpos(pmh->pt);

		if( ln > 0 ) {
			int dt_ln = get_dataline(ln);
			BOOL bDel = FALSE;
			char edt_sgn;

			if( pDlg->dataline_input(dt_ln) ) {
				m_bLineCopy = TRUE;

				bDel = TRUE;
				if( IsScanLine(dt_ln) ) {
					if( IsTransLine(ln, edt_sgn) ) {
						if( edt_sgn > 1 ) {	//確定仕訳
							bDel = FALSE;
						}
					}
					if( bDel ) {
						if( !SyogSiwakeMode() ) {	//諸口仕訳詰める。
							// 単一仕訳
							if( !m_DPscan.IsFukugoSiwake(dt_ln) )
								bDel = FALSE;
						}
						else {
							if( m_DPscan.IsTanitsuSiwake(dt_ln) )	bDel = FALSE;
						}
					}
				}
			}
			else {
				m_bLineCopy = FALSE;
			}

			if( m_nCopyLine > 0 ) {
				m_bLinePaste = TRUE;

				if( IsJob() == JOB_SCAN && IsTransLine(ln, edt_sgn) ) {
					if( edt_sgn > 1 ) {
						m_bLinePaste = FALSE;
					}
				}
			}
			else {
				m_bLinePaste = FALSE;
			}

			if( m_bLineCopy || m_bLinePaste ) {
				CMenu menu;
				VERIFY(menu.LoadMenu(IDR_DNPCPYMENU));
				CMenu* pPopup = menu.GetSubMenu(0);
				ASSERT(pPopup != NULL);
				if( pPopup == NULL ) return FALSE;

				UINT flag;

				if( m_bLineCopy )	flag = MF_ENABLED;
				else				flag = MF_GRAYED;
				pPopup->EnableMenuItem(IDM_LINE_COPY, MF_BYCOMMAND | flag);

				if( m_bLinePaste )	flag = MF_ENABLED;
				else				flag = MF_GRAYED;
				pPopup->EnableMenuItem(IDM_LINE_PASTE, MF_BYCOMMAND | flag);

				if( bDel )	flag = MF_ENABLED;
				else		flag = MF_GRAYED;
				pPopup->EnableMenuItem(IDM_LINE_DELETE, MF_BYCOMMAND | flag);

				DWORD dwRetCode = pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD,
					pmh->pt.x, pmh->pt.y, m_pDnpParent);

				if( dwRetCode == IDM_LINE_COPY ) {
					OnLineCopy(ln);
				}
				else if( dwRetCode == IDM_LINE_PASTE ) {
					OnLinePaste(ln);
				}
				else if( dwRetCode == IDM_LINE_DELETE ) {
					OnLineDelete(ln);
				}

				bRet = TRUE;
			}
		}
	}

	return bRet;
}


void CDnpBaseClass::OnLineCopy(int ln)
{
	int dt_ln;
	dt_ln = get_dataline(ln);

	int old = m_nCopyLine;

	m_nCopyLine = dt_ln;

	if( old != m_nCopyLine ) {
		GetNowDnpDialog()->ShowWindow(SW_HIDE);
		m_pDnpParent->RedrawWindow();
		GetNowDnpDialog()->ShowWindow(SW_SHOW);
		//	GetNowDnpDialog()->DiagRefresh();
	}
}

//貼り付け処理
void CDnpBaseClass::OnLinePaste(int ln)
{
	int dt_ln;
	dt_ln = get_dataline(ln);

	if( m_nCopyLine > 0 ) {

		struct _DP_DATA_LINE	dtold;
		struct _DP_DATA_LINE*	pDLsrc;

		memcpy(&dtold,
			getLINE_DATA(dt_ln), sizeof(struct _DP_DATA_LINE));

		pDLsrc = getLINE_DATA(m_nCopyLine);

		memcpy(getLINE_DATA(dt_ln), pDLsrc, sizeof(struct _DP_DATA_LINE));

		//科目マップサインを調整
		int kind, pn, syz_pn;
		kind = IsKind();

		if( kind == KIND_FURIKAE ) {
			for( int i = 0; i < 2; i++ ) {
				if( i == 0 ) {
					pn = FD_DEBT_PN;
					syz_pn = FD_DSKBN_PN;
				}
				else {
					pn = FD_CRED_PN;
					syz_pn = FD_CSKBN_PN;
				}
				int idx = pn - FD_DBVAL_PN;

				if( dtold.LINE_CND[idx].INP_sg ) {
					if( !pDLsrc->LINE_CND[idx].INP_sg ) {
						_delMAP(dt_ln, pn);
					}
				}
				if( !dtold.LINE_CND[idx].INP_sg ) {
					if( pDLsrc->LINE_CND[idx].INP_sg ) {
						_setMAP(dt_ln, pn);
					}
				}
				//確定・転送済みサインは初期化
				_getDATA(dt_ln, syz_pn)->SYDATA._DSGN[7] &= ~(0xf0);
			}
		}
		else {
			if( kind == KIND_NYUKIN ) {
				pn = FD_CRED_PN;
				syz_pn = FD_CSKBN_PN;
			}
			else {
				pn = FD_DEBT_PN;
				syz_pn = FD_DSKBN_PN;
			}
			int idx = pn - FD_DBVAL_PN;

			if( dtold.LINE_CND[idx].INP_sg ) {
				if( !pDLsrc->LINE_CND[idx].INP_sg ) {
					_delMAP(dt_ln, pn);
				}
			}
			if( !dtold.LINE_CND[idx].INP_sg ) {
				if( pDLsrc->LINE_CND[idx].INP_sg ) {
					_setMAP(dt_ln, pn);
				}
			}

			//確定・転送済みサインは初期化
			_getDATA(dt_ln, syz_pn)->SYDATA._DSGN[7] &= ~(0xf0);
		}

		//転送済みサインは初期化
		for( pn = FD_DBVAL_PN; pn < FD_MAX_PN; pn++ ) {
			_getCND(dt_ln, pn)->EDT_sg = 0;
		}

		//画面の表示を更新する。
		GetNowDnpDialog()->dataline_disp((m_TopIdx + 1), ln);
	}
}



//行削除処理
void CDnpBaseClass::OnLineDelete(int ln)
{
	int dt_ln;
	dt_ln = get_dataline(ln);

	int	pn = get_nowpn();

	if( !m_bLineCopy ) {
		return;
	}

	int del_line = 0;

	if( IsScanLine(dt_ln) ) {
		// 確定仕訳
		if( _getCND(dt_ln, pn)->EDT_sg > 1 )
			return;

		if( IsKind() == KIND_FURIKAE ) {
			if( !SyogSiwakeMode() ) {
				if( !m_DPscan.IsFukugoSiwake(dt_ln) ) {
					m_DPscan.DeleteLine(dt_ln) = 1;
					del_line++;
				}
			}
			else {
				if( m_DPscan.IsTanitsuSiwake(dt_ln) ) {
					return;
				}
			}
		}

		else {
			m_DPscan.DeleteLine(dt_ln) = 1;
			del_line++;
		}
	}

	if( del_line )	return;

	data1line_clear(dt_ln);

	// 消費税区分入力非表示
	Destroy_SyzInp();
	Destroy_HjiyuInp();
	Destroy_HusenInp();

	//科目マップサインを調整
	int kind, syz_pn;
	kind = IsKind();

	if( kind == KIND_FURIKAE ) {
		for( int i = 0; i < 2; i++ ) {
			if( i == 0 ) {
				pn = FD_DEBT_PN;
				syz_pn = FD_DSKBN_PN;
			}
			else {
				pn = FD_CRED_PN;
				syz_pn = FD_CSKBN_PN;
			}

			_delMAP(dt_ln, pn);
		}
	}
	else {
		if( kind == KIND_NYUKIN ) {
			pn = FD_CRED_PN;
			syz_pn = FD_CSKBN_PN;
		}
		else {
			pn = FD_DEBT_PN;
			syz_pn = FD_DSKBN_PN;
		}

		_delMAP(dt_ln, pn);
	}

	//画面の表示を更新する。
	GetNowDnpDialog()->dataline_disp((m_TopIdx + 1), ln);

	// 貸借合計表示
	total_dsp();
	// 諸口貸借表示
	syogdsp();
}

void CDnpBaseClass::LineCopyMsgJob( MSG* pMsg )
{
	if( m_nCopyLine <= 0 ) {
		if( pMsg->message == WM_KEYDOWN ) {
			if( pMsg->wParam == VK_INSERT ) {
				OnButtonInsert();
			}
		}
		return;
	}

	if( pMsg->message == WM_KEYDOWN ) {
		switch( pMsg->wParam ) {
		case VK_UP:		case VK_DOWN:
		case VK_LEFT:	case VK_RIGHT:
			break;
		default:
			m_nCopyLine = -1;
			break;
		}
	}
	else if( pMsg->message == WM_CHAR ) {
		m_nCopyLine = -1;
	}

	if( m_nCopyLine == -1 ) {
		GetNowDnpDialog()->ShowWindow( SW_HIDE );
		m_pDnpParent->RedrawWindow();
		GetNowDnpDialog()->ShowWindow( SW_SHOW );
//		GetNowDnpDialog()->DiagRefresh();
	}
}



// 伝票データの挿入
int CDnpBaseClass::dnp_insert_job( int ln )
{
	// データバッファにデータ挿入
	CDnpBaseDlg* pDlg = GetNowDnpDialog();

	int ret = 0;

	// 最終行より
	int dlin = get_MaxLine();

	if( ! pDlg->dataline_input( dlin ) ) {
		_insMAP( ln );
		InsDelSgn( ln );

		for(  ; dlin >= ln; dlin-- ) {
			if( dlin > ln ) {
				memcpy( getLINE_DATA( dlin ), getLINE_DATA( dlin-1 ),
						sizeof(struct _DP_DATA_LINE) );
			}
			else {
				data1line_clear( ln );
			}
		}

		ret++;
	}

	return ret;
}

// Insert キー
void CDnpBaseClass::OnButtonInsert()
{
	ButtonInsertJob();
}


BOOL CDnpBaseClass::ButtonInsertJob()
{
	int ln = get_dataline();
	BOOL bRet = FALSE;

	if( ! m_DI.IsMasterType(MST_INSERT) )
		return FALSE;

	if( m_DI.IsConFirmMaster() )
		return FALSE;

	int pn = get_nowpn();
	if( pn >= DP_DENP_PN && pn <= DP_KESYU_PN ) {
		return FALSE;
	}

	if( ln >= 1 ) {

		if( IsJob() == JOB_SCAN ) {
			if( ! m_DI.IsInsertMaster() ) {
				return FALSE;
			}
		}

		if( dnp_insert_job( ln ) ) {

			if( IsJob() == JOB_SCAN ) {
				m_DPscan.InsertDenpLine( ln );
			}

			int pn = get_nowpn();
			int id = -1;
			switch( pn ) {
			case FD_DBMN_PN:
			case FD_CBMN_PN:
				id = IDC_ICSDBEDT_BMON;
				break;
			case FD_DKOJI_PN:
			case FD_CKOJI_PN:
				id = IDC_ICSDBEDT_KOJI;
				break;
			case FD_TKY_PN:
				id = IDC_ICSDBEDT8CTRL1;
				break;
			case FD_TGNUM_PN:
				id = IDC_ICSDBEDT_TGNUM;
				break;
			case FD_DEBT_PN:
			case FD_CRED_PN:
				id = IDC_ICSDBEDT_KMINP;
				break;
			}

			//画面再表示
			CDnpBaseDlg* pDlg = GetNowDnpDialog();

			if( id != -1 ) {
				if( id == IDC_ICSDBEDT_KMINP ) {
					struct _KamokuData	kd = { 0 };
					DBkamokudata_set(pDlg, id, &kd, m_pZm);
				}
				else {
					VARIANT var;
					var.pbVal = NULL;
					DBdata_set( pDlg, id, &var, ICSDBEDT_TYPE_STRING, 0 );
					if( id == IDC_ICSDBEDT8CTRL1 ) {
						DBdata_set( pDlg, IDC_ICSDBEDT_KASITKY, &var, ICSDBEDT_TYPE_STRING, 0 );
					}
				}
			}

			pDlg->dataline_disp( (m_TopIdx+1) );

			int cnt = getSCROLL_NL();
			int	max = get_MaxLineEX();

			pDlg->set_scrollbar( NULL, m_TopIdx, max, cnt );

			bRet = TRUE;
		}
	}

	return bRet;
}



//-----------------------------------------------------------------
//	DBEDIT IME END処理
//
//-----------------------------------------------------------------
void CDnpBaseClass::ImeEndJob(UINT ID, long nChar, LPCTSTR string, LPCTSTR ystring)
{
	int st, term, pos, len;
	VARIANT var;
	char	tmp[128];
	char	ystr[128];

	// 確定仕訳で入力不可か？
	if( ! IsInputCharEnable() )
		return;

	term = 1;

	// 摘要入力
	if( ID == IDC_ICSDBEDT8CTRL1 || ID == IDC_ICSDBEDT_KASITKY ) {
		len = get_strcnt( (char*)string, m_pZm->zvol->tk_ln * 2 );
		memcpy( tmp, string, len );
		tmp[len] = '\0';

		memcpy( ystr, ystring, len );
		ystr[len] = '\0';

		if( (st = TekiyoStr_Chk( tmp )) > 0 ) {
			if( TekiyoKeyMode() ) {	// 摘要コード＋Enter で 摘要をセット
				if(	st == 1 ) {
					if( nChar == VK_RETURN ) {
						_KanaTermIcsinputTky( ystr, FALSE );
						term = 0;
					}
				}
				if( nChar == VK_TAB ) {
					if( ! isdigitstr( (char*)ystr ) ) {
						_KanaTermIcsinputTky( ystr, FALSE );
						term = 0;
					}
					else {
						if( TKtabMode() ) {
							_KanaTermIcsinputTky( ystr, TRUE );
							term = 0;
						}
					}
				}
				else if( nChar == VK_F12 ) {
					if( ! isdigitstr( (char*)ystr ) ) {
						_KanaTermIcsinputTky( ystr, FALSE );
						term = 0;
					}
				}
			}
			else {
				if( nChar == VK_TAB || nChar == VK_F12 ) {
					_KanaTermIcsinputTky( ystr, FALSE );
					term = 0;
				}
			}
		}
		if( term ) {
			var.pbVal = (BYTE*)tmp;

			CICSDBEDT* pDedt;
			pDedt = (CICSDBEDT*)(GetNowDnpDialog()->GetDlgItem(ID));
			pos = pDedt->GetCaretPosition();
			pDedt->InsertData( &var, ICSDBEDT_TYPE_STRING, 0, pos );
		//	TerminationIcsinputTky( m_pDnpParent, get_nowln(), nChar );
		}
	}
	else if( ID == IDC_ICSDBEDT_KOJI ) {
		if( CheckImeEndKoji( nChar, string ) != 0 )
			return;

		TerminationKojiDBedit( ID, nChar, 1, 0 );
	}
	else if( ID == IDC_ICSDBEDT_BMON ) {
		if( CheckImeEndBmon( nChar, string ) != 0 )
			return;

		TerminationBmonDBedit( ID, nChar, 1, 0 );
	}
	else if( ID == IDC_ICSDBEDT_TGNUM ) {
		strcpy_s( tmp, sizeof tmp, string );
		var.pbVal = (BYTE*)tmp;
		CICSDBEDT* pDedt;
		pDedt = (CICSDBEDT*)(GetNowDnpDialog()->GetDlgItem(ID));
		pos = pDedt->GetCaretPosition();
		pDedt->InsertData( &var, ICSDBEDT_TYPE_STRING, 0, pos );
	//	TerminationTgNumDBedit( ID, nChar, 1, 0 );
	}
	else if( ID == IDC_ICSDBEDT_KMINP ) {
		if( !lastBrnInpStr((char*)string, tmp, sizeof tmp) )
			return;
		//科目入力
		if( CheckImeEndKamoku(nChar, tmp) != 0 )
			return;

		TerminationKmkinp(ID, nChar, 1, 0);
	}
}

//-------------------------------------------------------------------------------------
//	摘要入力処理
//		BOOL bTkcut 摘要をセットしない。
//
//-------------------------------------------------------------------------------------
void CDnpBaseClass::_KanaTermIcsinputTky(LPCTSTR data, BOOL bTkcut)
{
	CDBipTKREC tkrec;
	struct _DenpData dmy_data = {0};

	if( tky_50kana_chk((char*)data) ) {
		//５０音摘要選択画面に表示
		SelPar par;
		par.sel_pn = SL_TKYKANA_PN;
		strncpy_s( par.tky_kana, sizeof par.tky_kana, m_tky_50kana, sizeof par.tky_kana );
		m_Seldata.SelectJob( &par, -1 );

		m_DI.tkrec_kanamode() = TRUE;
		m_DI.m_BrTek.set_tkquery( TRUE );
		//５０音摘要があった
		if( m_SelDispPN == SL_TKYKANA_PN )
			return;
	}

	//摘要選択
	if( tekiyo_selset( (char*)data, bTkcut, 1 ) == -1 ) {
		Buzzer();
	}
}


// 摘要選択＆データセット
//	BOOL bTKcut [TRUE: 摘要をセットしない。]
int CDnpBaseClass::tekiyo_selset( char *data, BOOL bTKcut, int kanaterm/*=0*/)
{
int dc_sw, dt_ln;
CDBINPDataRec drec, *pdata;
CDBipTKREC tkrec;
struct _DenpData *debt, *cred;
char *dbr_inptype, *cbr_inptype;

	dt_ln = get_dataline();

	//摘要選択
	if( tky_select( &tkrec, (char *)data ) != -1 )
	{
		//自動仕訳
		if( IsAutoSiwake( get_nowln() ) || AutoSiwakeCheck(get_nowln()) ) {
			if( kanaterm ) {
				m_autoTKrec = tkrec;
				GetNowDnpDialog()->PostMessage(WM_DBDINPMSG, DWP_TKYSWKDLG);
			}
			else {
				MakeAutoSiwake(&tkrec);
				// 関数内で、m_CREC 作成＋消費税表示
			}
		}
		else {
			mkSY_CREC( m_CREC, get_nowln() );
			tekiyo_selset_subfunc(dt_ln, &tkrec, bTKcut);
		}
#ifdef CLOSE
		struct _DP_DATA_LINE *dline;
		dline = getLINE_DATA( dt_ln/*get_nowln()*/ );

		debt = _getDATAs( dline, FD_DEBT_PN );
		cred = _getDATAs( dline, FD_CRED_PN );

		// 枝番入力タイプ
		dbr_inptype = &getCND( FD_DEBT_PN )->INP_type;
		cbr_inptype = &getCND( FD_CRED_PN )->INP_type;
		dc_sw = tky_select_to_drec( m_CREC, &tkrec, dbr_inptype, cbr_inptype );	// 枝番ゲット

		// 科目が未入力のときでも、摘要枝番がセットされていた。[12.01 /10]
		// m_CREC は 現在行が複写のとき、上段の科目を取ってくる。
		struct _DNPLINE_CND* pDebCnd;
		struct _DNPLINE_CND* pCreCnd;

		pDebCnd = _getCND( dt_ln, FD_DEBT_PN );
		pCreCnd = _getCND( dt_ln, FD_CRED_PN );

		if( ! pDebCnd->INP_sg )
			dc_sw &= ~0x01;
		if( ! pCreCnd->INP_sg )
			dc_sw &= ~0x02;

		// m_CREC には、現在行の 科目がセットされていいる
		// データセット
		if( dc_sw & 0x01 )	// 借方 Found
		{
			pdata = &m_CREC[0];

			strcpy_s( debt->DP_CdNum, sizeof debt->DP_CdNum, pdata->m_dbt );
			debt->DP_BRN = pdata->m_dbr;
		}
		if( dc_sw & 0x02 )	// 貸方 Found
		{
			if( m_SYdataCnt == 2 )
					pdata = &m_CREC[1];
			else	pdata = &m_CREC[0];

			strcpy_s( cred->DP_CdNum, sizeof cred->DP_CdNum, pdata->m_cre );
			cred->DP_BRN = pdata->m_cbr;
		}

#ifdef NXTVERSCLOSE	// 06.27 /00
		//消費税コードセット
//		if( getINP_CTL( TKATTR_PN )->INP_ena && tkrec.m_tksgn[1] )
			getDATA( SKBN_PN )->SYDATA.TKattr = tkrec.m_tksgn[1];
#endif
		//表示
		tky_select_dsp( m_CREC, dline, dc_sw, &tkrec, bTKcut );
#endif

		return 0;
	}
	else
		return -1;

}


//自動仕訳時の仕訳摘要の表示
void CDnpBaseClass::swktekiyo_dspsub(CString tky, int dbedt)
{
	char tkstr[128] = { 0 };
	VARIANT var;
	strcpy_s(tkstr, sizeof tkstr, tky);
	short pos;

	int ln = get_nowln();
	BOOL bDisp = TRUE;
	int dt_ln = get_dataline(ln);

	if( ln > getSCROLL_NL() ) {
		bDisp = FALSE;
	}
	// 枝番摘要を 仕訳摘要に
	if( get_nowpn() == FD_TKY_PN && dbedt ) {
		var.pbVal = (BYTE*)tkstr;
		CWnd* pwnd;
		pwnd = GetNowDnpDialog();
		UINT dbEdt = IDC_ICSDBEDT8CTRL1;

		CICSDBEDT* pDedt;
		pDedt = (CICSDBEDT*)(GetNowDnpDialog()->GetDlgItem(dbEdt));
		pos = pDedt->GetCaretPosition();
		pDedt->InsertData(&var, ICSDBEDT_TYPE_STRING, 0, pos);

	}
	else {
		int n = get_strcnt(tkstr, (m_pZm->zvol->tk_ln * 2));
		tkstr[n] = '\0';
		strcpy_s((char*)_getDATA(dt_ln, FD_TKY_PN)->DP_TKY, sizeof _getDATA(dt_ln, FD_TKY_PN)->DP_TKY, (const char*)tkstr);

		if( bDisp ) {
			GetNowDnpDialog()->SetTekiyo(ln, FD_TKY_PN, tkstr);
		}
	}
}


void CDnpBaseClass::tekiyo_selset_subfunc(int dt_ln, CDBipTKREC* ptkrec, BOOL bTKcut)
{
	struct _DP_DATA_LINE *dline;
	dline = getLINE_DATA(dt_ln/*get_nowln()*/);

	int dc_sw;
	CDBINPDataRec drec, *pdata;
	struct _DenpData *debt, *cred;
	char *dbr_inptype, *cbr_inptype;

	debt = _getDATAs(dline, FD_DEBT_PN);
	cred = _getDATAs(dline, FD_CRED_PN);

	// 枝番入力タイプ
	dbr_inptype = &_getCND(dt_ln, FD_DEBT_PN)->INP_type;
	cbr_inptype = &_getCND(dt_ln, FD_CRED_PN)->INP_type;
	dc_sw = tky_select_to_drec(m_CREC, ptkrec, dbr_inptype, cbr_inptype, dt_ln);	// 枝番ゲット

	// 科目が未入力のときでも、摘要枝番がセットされていた。[12.01 /10]
	// m_CREC は 現在行が複写のとき、上段の科目を取ってくる。
	struct _DNPLINE_CND* pDebCnd;
	struct _DNPLINE_CND* pCreCnd;

	pDebCnd = _getCND(dt_ln, FD_DEBT_PN);
	pCreCnd = _getCND(dt_ln, FD_CRED_PN);

	if( !pDebCnd->INP_sg )
		dc_sw &= ~0x01;
	if( !pCreCnd->INP_sg )
		dc_sw &= ~0x02;

	// m_CREC には、現在行の 科目がセットされていいる
	// データセット
	if( dc_sw & 0x01 )	// 借方 Found
	{
		pdata = &m_CREC[0];

		strcpy_s(debt->DP_CdNum, sizeof debt->DP_CdNum, pdata->m_dbt);
		debt->DP_BRN = pdata->m_dbr;
	}
	if( dc_sw & 0x02 )	// 貸方 Found
	{
		if( m_SYdataCnt == 2 )
			pdata = &m_CREC[1];
		else	pdata = &m_CREC[0];

		strcpy_s(cred->DP_CdNum, sizeof cred->DP_CdNum, pdata->m_cre);
		cred->DP_BRN = pdata->m_cbr;
	}

#ifdef NXTVERSCLOSE	// 06.27 /00
	//消費税コードセット
//		if( getINP_CTL( TKATTR_PN )->INP_ena && tkrec.m_tksgn[1] )
	getDATA(SKBN_PN)->SYDATA.TKattr = tkrec.m_tksgn[1];
#endif
	//表示
	tky_select_dsp(m_CREC, dline, dc_sw, ptkrec, bTKcut);
}


//-----------------------------------------------------------
//	自動仕訳を作成
//		 CDBipTKREC* tkrec
//-----------------------------------------------------------
void CDnpBaseClass::MakeAutoSiwake( CDBipTKREC* tkrec )
{
	int	ln = get_nowln();
	int	pn = get_nowpn();

	int dt_ln = get_dataline(ln);

	CString dbt, cre;
	int		i, dbr, cbr;

	ICS_TKREC_PAC tkpac;
	int attr, tkcod, tkjourseq, jcnt, st;
	attr = 0x03;

	tkcod = tkrec->m_tkcod;

	jcnt = m_DI.pTkjour->SelectTkjourCount(tkrec->m_tkcod, attr);

	if( jcnt > 1 ) {
		st = m_DI.pTkjour->SelectTkjourDlg(tkrec->m_tkcod, attr, tkjourseq, m_pDnpParent);
	}
	else {
		st = m_DI.pTkjour->SelectTkjour(tkrec->m_tkcod, attr, tkjourseq);
	}

	if( st == 0 ) {
		if( !tkrec->m_tkname.IsEmpty() ) {
			swktekiyo_dspsub(tkrec->m_tkname, 1);
		}
		return;
	}
	CArray < ICS_TKJOUR_PAC, ICS_TKJOUR_PAC& > tkjourAry;
	//tkjour内容を取得
	m_DI.pTkjour->GetTkjour(tkjourseq, tkjourAry);

	BOOL bDisp;

	m_bAutoLoop = TRUE;

	int jourCnt = tkjourAry.GetCount();
	//選択摘要 をDBEditにセット
	if( jourCnt == 0 ) {
		tekiyo_selset_subfunc(dt_ln, tkrec, FALSE);
		return;
	}

	CString tekiyo;
	int kind = IsKind();

	//借・貸部門コードチェック
	int bmnflg = -99;
	BOOL sameBmn = TRUE;
	for( int n = 0; n < jourCnt; n++ ) {
		dbt = tkjourAry[n].dbt;
		if( !dbt.IsEmpty() ) {
			if( bmnflg == -99 ) {
				bmnflg = tkjourAry[n].dbmn;
			}
			if( bmnflg != -99 ) {
				if( bmnflg != tkjourAry[n].dbmn ) {
					sameBmn = FALSE;
					break;
				}
			}
		}
		cre = tkjourAry[n].cre;
		if( !cre.IsEmpty() ) {
			if( bmnflg == -99 ) {
				bmnflg = tkjourAry[n].cbmn;
			}
			if( bmnflg != -99 ) {
				if( bmnflg != tkjourAry[n].cbmn ) {
					sameBmn = FALSE;
					break;
				}
			}
		}
	}
	int nobmn = 0;

	if( sameBmn == TRUE ) {
		struct _DenpData	*denp;
		if( kind == KIND_FURIKAE ) {
			if( m_FuriMode == FURI_TANBMON ) {
				denp = getDATA(DP_DENP_PN);

				if( denp->DP_BMN != bmnflg ) {
					if( denp->DP_BMN != -1 ) {
						sameBmn = FALSE;
						FuriModeChange(FURI_DATABMON);
					}
				}
			}
		}
		else {
			denp = getDATA(DP_DENP_PN);
			if( denp->DP_BMN != bmnflg ) {
				if( denp->DP_BMN != -1 ) {
					sameBmn = FALSE;
					//入金・出金はデータごとの部門はセットしない
					nobmn = 1;
				}
			}
		}
	}
	//ヘッド部に部門セット
	if( sameBmn ) {
		if( (kind == KIND_FURIKAE && m_FuriMode == FURI_TANBMON)
			|| kind != KIND_FURIKAE ) {
			struct _DenpData	*denp;

			denp = getDATA(DP_BMN_PN);
			denp->DP_BMN = bmnflg;

			struct _DNPLINE_CND *bmncnd;
			bmncnd = getCND(DP_BMN_PN);
			bmncnd->INP_sg = TRUE;

			struct _SET_KMKETC sk;
			sk.bmn = bmnflg;
			sk.code.Empty();
			sk.kno.Empty();
			sk.brn = -1;
			sk.etc.Empty();

			//部門名称を表示する場合
			if( GetBmnEtc(&sk) ) {
				bmncnd->INP_sg = TRUE;
				strcpy_s(bmncnd->ETC_msg, sizeof bmncnd->ETC_msg, sk.etc);
				bmncnd->ETC_col = sk.etc_col;
				GetNowDnpDialog()->SetBmon(-1, DP_BMN_PN, bmnflg, &sk);
			}
			else {
				::ZeroMemory(bmncnd->ETC_msg, sizeof bmncnd->ETC_msg);
				bmncnd->ETC_col = 0;
				GetNowDnpDialog()->SetBmon(-1, DP_BMN_PN, bmnflg, NULL);
			}
		}
	}

	int dbmn, cbmn;

	for( int n = 0; n < jourCnt; n++ ) {
		m_AutoCnt = n;

		dbt = tkjourAry[n].dbt;
		dbr = tkjourAry[n].jour_dbr;
		dbmn = tkjourAry[n].dbmn;

		cre = tkjourAry[n].cre;
		cbr = tkjourAry[n].jour_cbr;
		cbmn = tkjourAry[n].cbmn;

		tekiyo = tkjourAry[n].tekiyo;

		if( dt_ln + n >= get_MaxLine() )
			break;

		bDisp = TRUE;
		//画面上から見えないデータ
		if( (ln + n) > getSCROLL_NL() )
			bDisp = FALSE;

		//伝票と登録状況からチェック
		if( dbt.IsEmpty() && cre.IsEmpty() )	break;
		else if( kind == KIND_FURIKAE ) {
			if( isSYOGT_CODE(dbt) || isSYOGT_CODE(cre) ) {

				if( IsJob() != JOB_INPUT )	break;
				//諸口は空白として扱う
				if( isSYOGT_CODE(dbt) )	dbt.Empty();
				if( isSYOGT_CODE(cre) )	cre.Empty();
			}
		}
		else if( kind == KIND_NYUKIN ) {
			if( cre.IsEmpty() )	break;;
			if( isSYOGT_CODE(dbt) || isSYOGT_CODE(cre) )	break;;
		}
		else if( kind == KIND_SYUKKIN ) {
			if( dbt.IsEmpty() )	break;;
			if( isSYOGT_CODE(dbt) || isSYOGT_CODE(cre) )	break;;
		}
		else	break;;

		//入金、出金伝票は現金をセット
		if( kind == KIND_NYUKIN ) {
			dbt = GNKINCode(m_pZm->zvol->apno);
		}
		else if( kind == KIND_SYUKKIN ) {
			cre = GNKINCode(m_pZm->zvol->apno);
		}

		//データ作成
		struct _DenpData	*keybuf, *bmndata;
		struct _DNPLINE_CND *cnd;
		DBKNREC	*pKn;
		struct _SET_KMKETC sk;

		if( !dbt.IsEmpty() ) {
			keybuf = _getDATA(dt_ln + n, FD_DEBT_PN);
			strcpy_s(keybuf->DP_CdNum, sizeof keybuf->DP_CdNum, dbt);
			keybuf->DP_BRN = dbr;

			cnd = _getCND(dt_ln + n, FD_DEBT_PN);

			pKn = DB_PjisToKnrec(dbt, m_pZm);
			if( pKn ) {
				KamokuString(pKn, keybuf->DP_KNAM, sizeof keybuf->DP_KNAM);
				_setMAP(dt_ln + n, FD_DEBT_PN);
				cnd->INP_sg = TRUE;
				cnd->INP_type = 0;
				if( dbr != -1 ) {	//自動仕訳に枝番あり
					cnd->INP_type = 1;
				}
				if( n == 0 ) {
					if( m_DI.km_syattr(CString(keybuf->DP_CdNum)) ) {
						cnd->SET_sg = 1;
						_getCND(dt_ln + n, FD_DSKBN_PN)->SET_sg = 1;
					}
				}

				SETMEISYO sm = { 0 };
				sk.bmn = -1;
				sk.code = dbt;
				sk.brn = dbr;
				if( m_DI._set_kmketc(&sk) ) {
					strcpy_s(cnd->ETC_msg, sizeof cnd->ETC_msg, sk.etc);
					cnd->ETC_col = sk.etc_col;
					strcpy_s(sm.sm_etc, sizeof sm.sm_etc, cnd->ETC_msg);
					sm.sm_etccol = cnd->ETC_col;
				}
				else {
					::ZeroMemory(cnd->ETC_msg, sizeof cnd->ETC_msg);
					cnd->ETC_col = RGB_BLACK;
					::ZeroMemory(sm.sm_etc, sizeof sm.sm_etc);
				}
				if( kind != KIND_NYUKIN ) {
					memmove(sm.sm_name, pKn->knnam, sizeof pKn->knnam);
					if( bDisp ) {
						GetNowDnpDialog()->SetKnam(ln + n, FD_DEBT_PN, &sm);
					}
				}
			}

			if( kind == KIND_FURIKAE && m_FuriMode == FURI_DATABMON ) {
				if( dbmn != -1 ) {
					bmndata = _getDATA(dt_ln + n, FD_DBMN_PN);
					bmndata->DP_BMN = dbmn;
					struct _DNPLINE_CND *bmncnd;

					bmncnd = _getCND(dt_ln + n, FD_DBMN_PN);
					bmncnd->INP_sg = TRUE;

					struct _SET_KMKETC sk;
					sk.bmn = dbmn;
					sk.code.Empty();
					sk.kno.Empty();
					sk.brn = -1;
					sk.etc.Empty();

					//部門名称を表示する場合
					if( GetBmnEtc(&sk) ) {
						strcpy_s(bmncnd->ETC_msg, sizeof bmncnd->ETC_msg, sk.etc);
						bmncnd->ETC_col = sk.etc_col;
						if( bDisp ) {
							GetNowDnpDialog()->SetBmon(ln + n, FD_DBMN_PN, dbmn, &sk);
						}
					}
					else {
						::ZeroMemory(bmncnd->ETC_msg, sizeof bmncnd->ETC_msg);
						bmncnd->ETC_col = 0;
						if( bDisp ) {
							GetNowDnpDialog()->SetBmon(ln + n, FD_DBMN_PN, dbmn, NULL);
						}
					}
				}
			}
		}
		else {
			_getCND(dt_ln + n, FD_DEBT_PN)->INP_sg = FALSE;
		}

		if( !cre.IsEmpty() ) {
			keybuf = _getDATA(dt_ln + n, FD_CRED_PN);
			strcpy_s(keybuf->DP_CdNum, sizeof keybuf->DP_CdNum, cre);
			keybuf->DP_BRN = cbr;
			cnd = _getCND(dt_ln + n, FD_CRED_PN);

			pKn = DB_PjisToKnrec(cre, m_pZm);
			if( pKn ) {
				KamokuString(pKn, keybuf->DP_KNAM, sizeof keybuf->DP_KNAM);
				//	memmove( keybuf->DP_KNAM, (char*)pKn->knnam, sizeof pKn->knnam );
				_setMAP(dt_ln + n, FD_CRED_PN);
				cnd->INP_sg = TRUE;
				cnd->INP_type = 0;
				if( cbr != -1 ) {	//自動仕訳に枝番あり
					cnd->INP_type = 1;
				}
				if( n == 0 ) {
					if( m_DI.km_syattr(CString(keybuf->DP_CdNum)) ) {
						_getCND(dt_ln + n, FD_CSKBN_PN)->SET_sg = 1;
						cnd->SET_sg = 1;
					}
				}

				SETMEISYO sm = { 0 };
				sk.bmn = -1;
				sk.code = cre;
				sk.brn = cbr;
				if( m_DI._set_kmketc(&sk) ) {
					strcpy_s(cnd->ETC_msg, sizeof cnd->ETC_msg, sk.etc);
					cnd->ETC_col = sk.etc_col;
					strcpy_s(sm.sm_etc, sizeof sm.sm_etc, cnd->ETC_msg);
					sm.sm_etccol = cnd->ETC_col;
				}
				else {
					::ZeroMemory(cnd->ETC_msg, sizeof cnd->ETC_msg);
					cnd->ETC_col = RGB_BLACK;
					::ZeroMemory(sm.sm_etc, sizeof sm.sm_etc);
				}
				if( IsKind() != KIND_SYUKKIN ) {
					memmove(sm.sm_name, pKn->knnam, sizeof pKn->knnam);
					if( bDisp ) {
						GetNowDnpDialog()->SetKnam(ln + n, FD_CRED_PN, &sm);
					}
				}
			}
			if( kind == KIND_FURIKAE && m_FuriMode == FURI_DATABMON ) {
				if( cbmn != -1 ) {
					bmndata = _getDATA(dt_ln + n, FD_CBMN_PN);
					bmndata->DP_BMN = cbmn;
					struct _DNPLINE_CND *bmncnd;

					bmncnd = _getCND(dt_ln + n, FD_CBMN_PN);
					bmncnd->INP_sg = TRUE;

					struct _SET_KMKETC sk;
					sk.bmn = cbmn;
					sk.code.Empty();
					sk.kno.Empty();
					sk.brn = -1;
					sk.etc.Empty();

					//部門名称を表示する場合
					if( GetBmnEtc(&sk) ) {
						strcpy_s(bmncnd->ETC_msg, sizeof bmncnd->ETC_msg, sk.etc);
						bmncnd->ETC_col = sk.etc_col;
						if( bDisp ) {
							GetNowDnpDialog()->SetBmon(ln + n, FD_CBMN_PN, cbmn, &sk);
						}
					}
					else {
						::ZeroMemory(bmncnd->ETC_msg, sizeof bmncnd->ETC_msg);
						bmncnd->ETC_col = 0;
						if( bDisp ) {
							GetNowDnpDialog()->SetBmon(ln + n, FD_CBMN_PN, cbmn, NULL);
						}
					}
				}
			}

		}
		else {
			_getCND(dt_ln + n, FD_CRED_PN)->INP_sg = FALSE;
		}

		//金額セットは複合仕訳
		if( jourCnt > 1 ) {
			CArith ar(0x16);
			char dval[6], cval[6];

			ar.l_input(dval, (void*)(LPCTSTR)tkjourAry[n].dval);
			ar.l_input(cval, (void*)(LPCTSTR)tkjourAry[n].cval);

			if( kind == KIND_FURIKAE || kind == KIND_SYUKKIN ) {
				//借方金額
				if( ar.l_test(dval) ) {
					if( bDisp )	GetNowDnpDialog()->SetVal(ln + n, FD_DBVAL_PN, (char*)dval);
					_getCND(dt_ln + n, FD_DBVAL_PN)->INP_sg = TRUE;
					_getCND(dt_ln + n, FD_DBVAL_PN)->SET_sg = FALSE;
					keybuf = _getDATA(dt_ln + n, FD_DBVAL_PN);
					memmove(keybuf->DP_VAL, dval, sizeof keybuf->DP_VAL);
				}
				else {
					//_getCND(dt_ln + n, FD_DBVAL_PN)->INP_sg = FALSE;
					//_getCND(dt_ln + n, FD_DBVAL_PN)->SET_sg = FALSE;
				}
			}

			if( kind == KIND_FURIKAE || kind == KIND_NYUKIN ) {
				//貸方金額
				if( ar.l_test(cval) ) {
					if( bDisp )	GetNowDnpDialog()->SetVal(ln + n, FD_CRVAL_PN, (char*)cval);
					_getCND(dt_ln + n, FD_CRVAL_PN)->INP_sg = TRUE;
					_getCND(dt_ln + n, FD_CRVAL_PN)->SET_sg = FALSE;
					keybuf = _getDATA(dt_ln + n, FD_CRVAL_PN);
					memmove(keybuf->DP_VAL, cval, sizeof keybuf->DP_VAL);
				}
				else {
					//_getCND(dt_ln + n, FD_CRVAL_PN)->INP_sg = FALSE;
					//_getCND(dt_ln + n, FD_CRVAL_PN)->SET_sg = FALSE;
				}
			}
		}
		// 消費税関係
		int cnt = mkSY_CREC(m_CREC, dt_ln + n, 1, 1);	// 消費税データ作成
		if( cnt == 1 ) {
			int dcsw;
			if( m_DI.km_syattr(m_CREC[0].m_dbt) )
				dcsw = 0;
			else	dcsw = 1;

			if( bDisp ) {
				setSY_CREC(m_CREC, dcsw, ln + n);
			}
			else {
				setSY_CREC(m_CREC, dcsw, dt_ln + n, 1);
			}
		}
		else {
			// どちらも消費税
			for( i = 0; i < cnt; i++ ) {
				if( bDisp ) {
					setSY_CREC(&m_CREC[i], i, ln + n);
				}
				else {
					setSY_CREC(&m_CREC[i], i, dt_ln + n, 1);
				}
			}
		}

		tekiyo_selset_subfunc(dt_ln + n, tkrec, n == 0 ? FALSE : TRUE);

		if( !tekiyo.IsEmpty() ) {
			swktekiyo_dspsub(tekiyo, n == 0 ? 1 : 0);
		}
	}
	total_dsp();

	m_bAutoLoop = FALSE;
	m_AutoCnt = 0;
}


// イメージスタティックのフォーカス処理
//
//
void CDnpBaseClass::ToViewImageStaticFocus( UINT ID )
{
	if( m_PRO_OK == FALSE )
		return;

	struct _DenpData *debt, *cred;

	int	ln	=	ID	-	IDC_IMAGE1 + 1;
	int	pn	=	FD_TKY_PN;
	int idx;
	int dt_ln = get_dataline( ln );

//TRACE( "ToViewImageStaticFocus ln = %d[%d], pn = %d[%d], \n", ln, get_nowln(), pn, get_nowpn() );

	if( ! bImgStaticUpdate ) {
		if( ln == get_nowln() && pn == get_nowpn() ) {
			return;
		}
	}
	else {
		bImgStaticUpdate = FALSE;
	}

	//自動仕訳モードの解除
	if( ln != get_nowln() ) {
		if( IsJob() == JOB_SCAN && IsKind() == KIND_FURIKAE && IsScanLine( dt_ln ) ) {
			if( IsMode() == MODE_AUTO ) {
				IsMode()	=	MODE_NORMAL;
				GetNowDnpDialog()->AutoSiwake( IsMode() );
			}
		}
	}

	CWnd* pwnd;
	pwnd = GetNowDnpDialog();

	//借方イメージへ行移動した時
	if( ID != IDC_IMAGE8 ) {
		//貸方摘要の消去
		pwnd->GetDlgItem( IDC_ICSDBEDT_KASITKY )->SetWindowPos( NULL, 0,0,0,0, SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER );
		//貸方イメージの消去
		KasiImageDisplay( ln, FALSE );
		//イメージ再表示有効
		ImagePaintSw( 1, getSCROLL_NL(), 0 );
		//背景色
		for( int i = IDC_IMAGE1; i <= IDC_IMAGE7; i++ ) {
			if( i != ID )	ImageBackColor( i - IDC_IMAGE1 + 1, FALSE );
			else			ImageBackColor( i - IDC_IMAGE1 + 1, TRUE );
		}
	}

	// 日付チェック＆アクション
	if( DateCheck_and_action() == -1 )
		return;

	// DBEditを戻す
	dbedt_putback();

	// 直前の消費税再表示
	int nowpn;
	nowpn = get_nowpn();

	if( (nowpn == FD_DSKBN_PN || nowpn == FD_CSKBN_PN ) && m_pSyzInp->IsDisplay() ) {
		setSY_CREC( m_CREC, (nowpn == FD_DSKBN_PN) ? 0 : 1,  get_nowln() );
	}

	// 消費税区分入力非表示
	Destroy_SyzInp();
	SelPar par;

	if( get_nowpn() != pn || get_nowln() != ln ) {
		debt = _getMAPtoDATA( dt_ln, FD_DEBT_PN );	// 仕訳データマップ経由の借方データゲット
		cred = _getMAPtoDATA( dt_ln, FD_CRED_PN );	// 仕訳データマップ経由の貸方データゲット

		//自動仕訳モードは自由摘要
		if( IsAutoSiwake( ln ) ) {
			par.sel_pn	= SL_TKYALL_PN;
			par.dbt		= debt ? debt->DP_CdNum : "";
			par.cre		= cred ? cred->DP_CdNum : "";
			::ZeroMemory( par.tky_kana, sizeof par.tky_kana );

			m_Seldata.SelectJob( &par, -1 );
		}
		else {
			// 連想摘要検索＆表示
			par.sel_pn	= SL_TKY_PN;
			par.dbt		= debt ? debt->DP_CdNum : "";
			par.cre		= cred ? cred->DP_CdNum : "";
			::ZeroMemory( par.tky_kana, sizeof par.tky_kana );

			m_Seldata.SelectJob( &par, -1 );
		}
	}

	if( get_nowln() != ln )
		LineChgJob( m_CREC, ln, pn );	// 消費税区分新データ作成

	idx = dt_ln-1;

	if( IsKind() == KIND_FURIKAE ) {
		//イメージの下に
		if( ID != IDC_IMAGE8 ) {
			//貸方のイメージ表示
			if( tblImage[idx][1] ) {
				//イメージ再表示無効
				ImagePaintSw( ln + 1, getSCROLL_NL(), 1 );

				KasiImageDisplay( ln, TRUE );
			}
			//貸方の摘要表示
			else if( !tblTekiyo[idx].IsEmpty() ) {

				//イメージ再表示無効
				ImagePaintSw( ln + 1, getSCROLL_NL(), 1 );

				CWnd*	pTeki	=	pwnd->GetDlgItem( IDC_ICSDBEDT_KASITKY );
				CRect	rect, tmp;
				
				m_pDnpParent->GetDlgItem( ID )->GetWindowRect( rect );
				((CDnpBaseDlg*)pwnd)->GetRect( ln, FD_TKY_PN, tmp );

				rect.top	=	rect.bottom;
				rect.bottom	=	rect.top + tmp.Height() + tmp.Height() / 3 * 2;
				pwnd->ScreenToClient( &rect );
				pTeki->SetWindowPos( &CWnd::wndTopMost, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW | SWP_NOZORDER );
//				pTeki->SetFont( pTKYorg );

				VARIANT var;
				var.pbVal = (BYTE*)(LPCTSTR)tblTekiyo[idx];

				DBdata_set( pwnd, IDC_ICSDBEDT_KASITKY, &var, ICSDBEDT_TYPE_STRING, 0 );

				set_nowln( ln );
				set_nowpn( FD_TKY_PN );
				
				SetFocusToImage( ln, 0 );
			}
		}
	}

	set_nowln( ln ); 
	set_nowpn( FD_TKY_PN );
}

// イメージスタティックの キー移動操作
//
void CDnpBaseClass::ToViewImageStaticKey( UINT ID, UINT nChar )
{
	if( m_PRO_OK == FALSE )
		return;

	int	vect, idx, dset;

	int	fpos;
	if( IsKind() == KIND_NYUKIN ) {
		fpos	=	FD_CRED_PN;

	}
	else if( IsKind() == KIND_SYUKKIN ) {
		fpos	=	FD_DEBT_PN;
	}
	else {
		fpos	=	FD_DBVAL_PN;
	}

	int	ln		=	get_nowln();
	int	dt_ln	=	get_dataline(ln);

	if( ::GetKeyState( VK_SHIFT ) < 0 ) {
		if( nChar == VK_TAB )	nChar	=	VK_LEFT;
	}

	idx = dt_ln-1;
	CDnpBaseDlg* pDlg;
	pDlg = GetNowDnpDialog();

	if( IsKind() == KIND_FURIKAE ) {
		//貸方イメージ位置から
		if( pDlg->IsKasiImage() ) {
			if( nChar == VK_UP || nChar == VK_LEFT || nChar == VK_F2 ) {
				//イメージへ
				if( tblImage[idx][0] ) {
					SetFocusToImage( ln, 0 );
					return;
				}
				//摘要へ
				else {
					tkyexpan( ln, NULL, FALSE );
					set_position( ln, FD_TKY_PN );
					return;
				}
			}
		}
		//イメージ位置から
		else {
			if( nChar == VK_DOWN || nChar == VK_RIGHT || nChar == VK_RETURN ||
					nChar == VK_TAB /*|| nChar == VK_F3*/ ) {
				//貸方イメージへ
				if( tblImage[idx][1] ) {
					SetFocusToImage( ln, 1 );
					return;
				}
				//貸方摘要へ
				else if( !tblTekiyo[idx].IsEmpty() ) {
				//	pDlgFuri->PostMessage( WM_RETROFOCUS, -1, -1 );
					return;
				}
			}
		}
	}

	if( nChar == VK_UP || nChar == VK_DOWN ) {
		dset = 0;

		if( nChar == VK_UP && ln == 1 ) {
			dset = DnpLine_Chg(-1);
		}
		else if( nChar == VK_DOWN && ln == getSCROLL_NL() ) {
			dset = DnpLine_Chg(1);
		}

		if( !dset ) {
			// 行移動
			if( nChar == VK_UP ) {
				--ln;
				vect	=	-1;
			}
			else {
				++ln;
				vect	=	1;
			}
			//行移動エラー（アンダー／オーバー　フロー）
			if( set_position( ln, FD_TKY_PN, vect ) == -1 ) {
				if( nChar == VK_UP )	set_focus( DP_MM_PN );
				else					set_position( 1, fpos );
			}
		}
		return;
	}

	m_SET_SYZINP_PN = 0;	// 消費税ダイアログフォーカスポジション

	// カーソルセット
	switch( nChar ) {
		case VK_RIGHT:
			if( IsKind() == KIND_SYUKKIN ) {
				set_position( ln, FD_DBVAL_PN );
				break;
			}
			else if( IsKind() == KIND_NYUKIN ) {
				set_position( ln, FD_CRVAL_PN );
				break;
			}
			else {
				set_position( ln, FD_CRED_PN );
				break;
			}
		case VK_RETURN:
		case VK_TAB:
//		case VK_F3:
			// 消費税マスターチェック
			if( IsAutoSiwake( ln + 1 ) ) {
				dset = 0;
				if( ln == getSCROLL_NL() ) {
					dset = DnpLine_Chg(1,1);
				}
				if( !dset ) {
					if( IsKind() == KIND_NYUKIN ) {
						if( set_position( ++ln, FD_CRVAL_PN ) == -1 )
							set_position( 1, FD_CRVAL_PN );
					}
					else {
						if( set_position( ++ln, FD_DBVAL_PN ) == -1 )
							set_position( 1, FD_DBVAL_PN );
					}
				}
				else {
					if( IsKind() == KIND_NYUKIN ) {
						set_position( ln, FD_CRVAL_PN );
					}
					else {
						set_position( ln, FD_DBVAL_PN );
					}
				}
			}
			else {
				dset = 0;
				if( ln == getSCROLL_NL() ) {
					dset = DnpLine_Chg(1,1);
				}

				if( !dset ) {
					if( IsKind() == KIND_NYUKIN ) {
						if( set_position( ++ln, FD_CRED_PN ) == -1 )
							set_position( 1, FD_CRED_PN );
					}
					else if( IsKind() == KIND_SYUKKIN ) {
						if( set_position( ++ln, FD_DEBT_PN ) == -1 )
							set_position( 1, FD_DEBT_PN );
					}
					else {
						if( set_position( ++ln, FD_DBVAL_PN ) == -1 )
							set_position( 1, FD_DBVAL_PN );
					}
				}
				else {
					if( IsKind() == KIND_NYUKIN ) {
						set_position( ln, FD_CRED_PN );
					}
					else if( IsKind() == KIND_SYUKKIN ) {
						set_position( ln, FD_DEBT_PN );
					}
					else {
						set_position( ln, FD_DBVAL_PN );
					}
				}
			}
			break;
		case VK_LEFT:
		case VK_F2:
			if( IsKind() == KIND_NYUKIN )		set_position( ln, FD_CRED_PN );
			else								set_position( ln, FD_DEBT_PN );
			break;
		case	VK_DELETE:
			//ペン用のボタンから来る事があるのでこのチェックを入れる
			if( IsJob() == JOB_SCAN && (tblImage[idx][0] || tblImage[idx][1]) ) {
				if( pDlg->IsKasiImage() ) {
					tblImage[idx][1]	=	0;

					ImagePaintSw( ln + 1, getSCROLL_NL(), 0 );
					KasiImageDisplay( get_nowln(), FALSE );
					if( tblImage[idx][0] ) {
						ImageBackColor( ln, TRUE );
						SetFocusToImage( ln, 0 );
					}
					else {
						_set_focus( ln, FD_TKY_PN );
					}
				}
				else {
					tblImage[idx][0]	=	0;

					//ペンのボタンから来る事があるのでIDではなくlnを使用する
					CWnd*	pWnd	=	pDlg->GetDlgItem( IDC_IMAGE1 + ln - 1 );
					pWnd->SetWindowPos( NULL, 0,0,0,0, SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER );
					_set_focus( ln, FD_TKY_PN );
				}
			}
			else {
				_set_focus( get_nowln(), get_nowpn() );
			}

			break;
		default:
			Buzzer();
			break;
	}
}



// DBEdit コントロール を元に戻す
void CDnpBaseClass::dbedt_putback()
{
	int line;
	line = get_nowln();

	// 摘要欄を戻す
	tkyexpan( line, NULL, FALSE );
	// 工事番号欄を戻す
	kjedit_expan( line, -1, NULL, FALSE );
	// 手形番号欄を戻す
	tgnum_expan( line, NULL, FALSE );
	// 部門番号欄を戻す
	bmnedit_expan( line, -1, NULL, FALSE );
	// 諸口枝番欄を戻す
	syobrnedit_expan(line, -1, NULL, FALSE);
	// 証憑番号欄
	syonum_expan(line, NULL, FALSE);
	// 科目欄
	kamoku_expan(line, -1, NULL, FALSE);
}



//
//	変動事由・振替科目の名称を取得
//
LPCTSTR CDnpBaseClass::get_hjnptitle()
{
//	if( IsOKGyosyu( m_pZm->zvol->apno, SW_SOCIAL|SW_SYUKYO)/* == 0x52*/ ) {
	if( m_DI.IsHjiyuItemGyosyu() == 0 ) {
		static char* p = "　　";
		return p;
	}

	return m_DI.get_hjiyutitle( 0 );
}



//-----------------------------------------------------
//	振替伝票の部門・工事入力モード
//	mode	0 = 単一部門・工事, 1 = 1仕訳ごとの部門
//-----------------------------------------------------
BOOL CDnpBaseClass::FuriModeChange( int mode )
{
	BOOL bRet = FALSE;

	if( mode == FURI_TANBMON ) {
		if( IsTanBmnEnable() ) {
			m_FuriMode = FURI_TANBMON;
			m_pFuri->FuriModeChange( mode );

			DispBmnKojiData( mode );
			bRet = TRUE;
		}
		else {
			CString msg, str;

			if( m_pZm->zvol->apno == 0x20 ) {
				switch( (m_pZm->zvol->sub_sw & 0x06) ) {
				case 0x02:
					str = _T( "部門" );	break;
				case 0x04:
					str = _T( "工事番号" );	break;
				case 0x06:
					str = _T( "部門・工事番号" );	break;
				}
			}
			else {
				if( (m_pZm->zvol->sub_sw & 0x02) ) {
					str = _T( "部門" );
				}
			}

			if( ! str.IsEmpty() ) {
				msg.Format( "借方・貸方で、異なる %s が登録されているため、\r\n単一%s入力 に切り替えることができません！", str, str );
				ICSMessageBox( msg );

				m_pFuri->SetTanbmnCheck( BST_CHECKED );

				set_focus( get_nowpn() );
			}
		}
	}
	else {
		m_FuriMode = FURI_DATABMON;
		m_pFuri->FuriModeChange( mode );

		DispBmnKojiData( mode );
		bRet = TRUE;
	}

	// 工事・摘要 のDBedit を戻す
	if( bRet ) {
		dbedt_putback();
	}

	if( bRet ) {
		int np = get_nowpn();
		
		if( np == DP_KOJI_PN || np == FD_DKOJI_PN || np == FD_CKOJI_PN ) {
			if( m_FuriMode == FURI_DATABMON )
				_set_focus( 1, FD_DKOJI_PN );
			else
				set_focus( DP_KOJI_PN );
		}
		else if( np == DP_BMN_PN || np == FD_DBMN_PN || np == FD_CBMN_PN ) {
			if( m_FuriMode == FURI_DATABMON )
				_set_focus( 1, FD_DBMN_PN );
			else
				set_focus( DP_BMN_PN );
		}
		else {
			set_focus( np );
		}
	}

	return bRet;
}


//-------------------------------------------------------------
//	振替伝票で、単一の部門・工事番号として処理できるか？
//
//-------------------------------------------------------------
BOOL CDnpBaseClass::IsTanBmnEnable()
{
	if( IsKind() != KIND_FURIKAE )
		return TRUE;

	if( ! m_DI.IsBumon() && ! m_DI.IsKoji() )
		return TRUE;

	int ln;
	int bmini, kjini;
	BOOL bRet = TRUE;

	int bmn;
	CString kjcod, kjtmp;

	bmini = kjini = 0;

	for( ln = 1; ln <= get_MaxLine(); ln++ ) {
		if( _getCND( ln, FD_DEBT_PN )->INP_sg ) {

			if( m_DI.IsBumon() ) {
				if( ! bmini ) {
					bmini = 1;
					bmn = _getDATA( ln, FD_DBMN_PN )->DP_BMN;
				}
				else {

					if( _getDATA( ln, FD_DBMN_PN )->DP_BMN != bmn ) {
						bRet = FALSE;
						break;
					}
				}
			}

			if( m_DI.IsKoji() ) {
				if( ! kjini ) {
					kjini = 1;
					kjcod = _getDATA( ln, FD_DKOJI_PN )->DP_KOJI;
				}
				else {
					kjtmp = _getDATA( ln, FD_DKOJI_PN )->DP_KOJI;
					if( kjcod != kjtmp ) {
						bRet = FALSE;
						break;
					}
				}
			}
		}

		if( _getCND( ln, FD_CRED_PN )->INP_sg ) {

			if( m_DI.IsBumon() ) {
				if( ! bmini ) {
					bmini = 1;
					bmn = _getDATA( ln, FD_CBMN_PN )->DP_BMN;
				}
				else {
					if( _getDATA( ln, FD_CBMN_PN )->DP_BMN != bmn ) {
						bRet = FALSE;
						break;
					}
				}
			}

			if( m_DI.IsKoji() ) {
				if( ! kjini ) {
					kjini = 1;
					kjcod = _getDATA( ln, FD_CKOJI_PN )->DP_KOJI;
				}
				else {
					kjtmp = _getDATA( ln, FD_CKOJI_PN )->DP_KOJI;
					if( kjcod != kjtmp ) {
						bRet = FALSE;
						break;
					}
				}
			}
		}
	}

	return bRet;
}


//
// 単一・ 仕訳ごとの部門入力 の切り替えで、部門・工事部分の表示を更新する
//		int mode:	FURI_TANBMON	単独部門入力
//					FURI_DATABMON	１仕訳部門入力
//
void CDnpBaseClass::DispBmnKojiData( int mode )
{	
	int			ln, bmn, set_sgn, dt_ln;
	CString		koji, empty_str;
	char		etc[128], kj_etc[128];
	COLORREF	etccol, kj_etccol;
	BOOL		bDisp;
	struct _SET_KMKETC	sk;
	CDnpBaseDlg* pDlg;
	pDlg = GetNowDnpDialog();

	if( IsKind() != KIND_FURIKAE )
		return;

	if( ! m_DI.IsBumon() && ! m_DI.IsKoji() )
		return;

	koji.Empty();
	empty_str.Empty();

	// 変更前の ポジションよりデータを取得する
	if( mode == FURI_DATABMON ) {
		// 単一 → 仕訳ごと
		bmn		= getDATA( DP_BMN_PN )->DP_BMN;
		getDATA( DP_BMN_PN )->DP_BMN = -1;
		// 部門名称
		::ZeroMemory( etc, sizeof etc );
		memcpy( etc, getCND( DP_BMN_PN )->ETC_msg, sizeof getCND( DP_BMN_PN )->ETC_msg );
		etccol = getCND( DP_BMN_PN )->ETC_col;

		::ZeroMemory( getCND( DP_BMN_PN )->ETC_msg, sizeof getCND( DP_BMN_PN )->ETC_msg );
		getCND( DP_BMN_PN )->ETC_col = 0;
		getCND( DP_BMN_PN )->INP_sg = FALSE;
		pDlg->SetBmon( -1, DP_BMN_PN, -1, NULL );

		if( m_DI.IsKoji() ) {
			koji = getDATA( DP_KOJI_PN )->DP_KOJI;
			::ZeroMemory( getDATA( DP_KOJI_PN )->DP_KOJI, sizeof getDATA( DP_KOJI_PN )->DP_KOJI );
			//工事名称
			::ZeroMemory( kj_etc, sizeof kj_etc );
			memcpy( kj_etc, getCND( DP_KOJI_PN )->ETC_msg, sizeof getCND( DP_KOJI_PN )->ETC_msg );
			kj_etccol = getCND( DP_KOJI_PN )->ETC_col;

			::ZeroMemory( getCND( DP_KOJI_PN )->ETC_msg, sizeof getCND( DP_KOJI_PN )->ETC_msg );
			getCND( DP_KOJI_PN )->ETC_col = 0;
			getCND( DP_KOJI_PN )->INP_sg = FALSE;
			pDlg->SetKoji( -1, DP_KOJI_PN, empty_str, NULL );
		}
	}
	else {
		// 仕訳ごと → 単一
		ln = 1;
		bmn		= _getDATA( ln, FD_DBMN_PN )->DP_BMN;
		// 部門名称
		::ZeroMemory( etc, sizeof etc );
		memcpy( etc, _getCND( ln, FD_DBMN_PN )->ETC_msg, sizeof _getCND( ln, FD_DBMN_PN )->ETC_msg );
		etccol = _getCND( ln, FD_DBMN_PN )->ETC_col;

		if( bmn == -1 )
			getCND( DP_BMN_PN)->INP_sg = FALSE;
		else
			getCND( DP_BMN_PN)->INP_sg = TRUE;

		getDATA( DP_BMN_PN )->DP_BMN = bmn;

		if( etc[0] != '\0' ) {
			strcpy_s( getCND( DP_BMN_PN )->ETC_msg, sizeof getCND( DP_BMN_PN )->ETC_msg, etc );
			getCND( DP_BMN_PN)->ETC_col = etccol;

			sk.bmn	= bmn;
			sk.kno.Empty();
			sk.code.Empty();	
			sk.brn	= -1;	
			sk.etc	= etc;	
			sk.etc_col	= etccol;;

			pDlg->SetBmon( -1, DP_BMN_PN, bmn, &sk );
		}
		else {
			::ZeroMemory( getCND( DP_BMN_PN )->ETC_msg, sizeof getCND( DP_BMN_PN )->ETC_msg );
			getCND( DP_BMN_PN )->ETC_col = 0;

			pDlg->SetBmon( -1, DP_BMN_PN, bmn, NULL );
		}

		if( m_DI.IsKoji() ) {
			koji = _getDATA( ln, FD_DKOJI_PN )->DP_KOJI;

			strcpy_s( (char*)getDATA( DP_KOJI_PN )->DP_KOJI, sizeof  getDATA( DP_KOJI_PN )->DP_KOJI, koji ); 
			if( koji.IsEmpty() )
				getCND( DP_KOJI_PN )->INP_sg = FALSE;
			else
				getCND( DP_KOJI_PN )->INP_sg = TRUE;

			//工事名称
			::ZeroMemory( kj_etc, sizeof kj_etc );
			memcpy( kj_etc, _getCND( ln, FD_DKOJI_PN )->ETC_msg, sizeof _getCND( ln, FD_DKOJI_PN )->ETC_msg );
			kj_etccol = _getCND( ln, FD_DKOJI_PN )->ETC_col;

			if( kj_etc[0] != '\0' ) {
				strcpy_s( getCND( DP_KOJI_PN )->ETC_msg, sizeof getCND( DP_KOJI_PN )->ETC_msg, kj_etc );
				getCND( DP_KOJI_PN )->ETC_col = kj_etccol;

				sk.bmn	= -1;
				sk.kno	= koji;
				sk.code.Empty();	
				sk.brn	= -1;	
				sk.etc	= kj_etc;	
				sk.etc_col	= kj_etccol;

				pDlg->SetKoji( -1, DP_KOJI_PN, koji, &sk );
			}
			else {
				::ZeroMemory( getCND( DP_KOJI_PN )->ETC_msg, sizeof getCND( DP_KOJI_PN )->ETC_msg );
				getCND( DP_KOJI_PN )->ETC_col = 0;

				pDlg->SetKoji( -1, DP_KOJI_PN, koji, NULL );
			}
		}
	}

	ln = 1;
	bDisp = FALSE;

	for( dt_ln = 1; dt_ln <= get_MaxLine(); dt_ln++ ) {
		set_sgn = 0;

		// データ行を画面表示するか？
		if( ! bDisp && ln != -1 ) {
			if( (ln + m_TopIdx) == dt_ln ) {
				bDisp = TRUE;
			}
		}
		else {
			if( ln > getSCROLL_NL() ) {
				bDisp = FALSE;
				ln = -1;
			}
		}

		//借方
		if( _getCND( dt_ln, FD_DEBT_PN )->INP_sg ) {
			if( mode == FURI_DATABMON ) {
				if( m_DI.IsBumon() ) {
					if( bmn == -1 )
						_getCND( dt_ln, FD_DBMN_PN)->INP_sg = FALSE;
					else
						_getCND( dt_ln, FD_DBMN_PN)->INP_sg = TRUE;
					
					_getDATA( dt_ln, FD_DBMN_PN )->DP_BMN = bmn;

					if( etc[0] != '\0' ) {
						strcpy_s( _getCND( dt_ln, FD_DBMN_PN)->ETC_msg, sizeof _getCND( dt_ln, FD_DBMN_PN)->ETC_msg, etc );
						_getCND( dt_ln, FD_DBMN_PN)->ETC_col = etccol;

						if( bDisp ) {
							sk.bmn	= bmn;
							sk.kno.Empty();
							sk.code.Empty();	
							sk.brn	= -1;	
							sk.etc	= etc;	
							sk.etc_col	= etccol;;

							pDlg->SetBmon( ln, FD_DBMN_PN, bmn, &sk );
						}
					}
					else {
						::ZeroMemory( _getCND( dt_ln, FD_DBMN_PN)->ETC_msg, sizeof _getCND( dt_ln, FD_DBMN_PN)->ETC_msg );
						_getCND( dt_ln, FD_DBMN_PN)->ETC_col = 0;
						if( bDisp ) {
							pDlg->SetBmon( ln, FD_DBMN_PN, bmn, NULL );
						}
					}
				}

				if( m_DI.IsKoji() ) {
					if( koji.IsEmpty() )
						_getCND( dt_ln, FD_DKOJI_PN)->INP_sg = FALSE;
					else
						_getCND( dt_ln, FD_DKOJI_PN)->INP_sg = TRUE;

					strcpy_s( (char*)_getDATA( dt_ln, FD_DKOJI_PN)->DP_KOJI, sizeof _getDATA( dt_ln, FD_DKOJI_PN)->DP_KOJI, koji );

					if( kj_etc[0] != '\0' ) {
						strcpy_s( _getCND( dt_ln, FD_DKOJI_PN)->ETC_msg, sizeof _getCND( dt_ln, FD_DKOJI_PN)->ETC_msg, kj_etc );
						_getCND( dt_ln, FD_DKOJI_PN)->ETC_col = kj_etccol;

						if( bDisp ) {
							sk.bmn	= -1;
							sk.kno	= koji;
							sk.code.Empty();	
							sk.brn	= -1;	
							sk.etc	= kj_etc;	
							sk.etc_col	= kj_etccol;

							pDlg->SetKoji( ln, FD_DKOJI_PN, koji, &sk );
						}
					}
					else {
						::ZeroMemory( _getCND( dt_ln, FD_DKOJI_PN)->ETC_msg, sizeof _getCND( dt_ln, FD_DKOJI_PN)->ETC_msg );
						_getCND( dt_ln, FD_DKOJI_PN)->ETC_col = 0;

						if( bDisp ) {
							pDlg->SetKoji( ln, FD_DKOJI_PN, koji, NULL );
						}
					}
				}

				set_sgn = 1;
			}
		}

		if( ! set_sgn ) {
			// 借方科目なしか、単一部門入力につき、仕訳データ部分はクリア
			if( m_DI.IsBumon() ) {
				if( bDisp ) {
					pDlg->SetBmon( ln, FD_DBMN_PN, -1, NULL );
				}
				_getDATA( dt_ln, FD_DBMN_PN )->DP_BMN = -1;
				_getCND( dt_ln, FD_DBMN_PN)->INP_sg = FALSE;
			}
			if( m_DI.IsKoji() ) {
				if( bDisp ) {
					pDlg->SetKoji( ln, FD_DKOJI_PN, empty_str, NULL );
				}
				::ZeroMemory( _getDATA( dt_ln, FD_DKOJI_PN)->DP_KOJI, sizeof _getDATA( dt_ln, FD_DKOJI_PN)->DP_KOJI );
				_getCND( dt_ln, FD_DKOJI_PN)->INP_sg = FALSE;
			}
		}

		set_sgn = 0;

		// 貸方
		if( _getCND( dt_ln, FD_CRED_PN )->INP_sg ) {
			if( mode == FURI_DATABMON ) {
				if( m_DI.IsBumon() ) {
					if( bmn == -1 )
						_getCND( dt_ln, FD_CBMN_PN)->INP_sg = FALSE;
					else
						_getCND( dt_ln, FD_CBMN_PN)->INP_sg = TRUE;

					_getDATA( dt_ln, FD_CBMN_PN )->DP_BMN = bmn;

					//部門名称あり
					if( etc[0] != '\0' ) {
						strcpy_s( _getCND( dt_ln, FD_CBMN_PN)->ETC_msg, sizeof _getCND( dt_ln, FD_CBMN_PN)->ETC_msg, etc );
						_getCND( dt_ln, FD_CBMN_PN)->ETC_col = etccol;

						if( bDisp ) {
							sk.bmn	= bmn;
							sk.kno.Empty();
							sk.code.Empty();	
							sk.brn	= -1;	
							sk.etc	= etc;	
							sk.etc_col	= etccol;;

							pDlg->SetBmon( ln, FD_CBMN_PN, bmn, &sk );
						}
					}
					else {
						::ZeroMemory( _getCND( dt_ln, FD_CBMN_PN)->ETC_msg, sizeof _getCND( dt_ln, FD_CBMN_PN)->ETC_msg );
						_getCND( dt_ln, FD_CBMN_PN)->ETC_col = 0;

						if( bDisp ) {
							pDlg->SetBmon( ln, FD_CBMN_PN, bmn, NULL );
						}
					}
				}

				if( m_DI.IsKoji() ) {
					if( koji.IsEmpty() )
						_getCND( dt_ln, FD_CKOJI_PN)->INP_sg = FALSE;
					else
						_getCND( dt_ln, FD_CKOJI_PN)->INP_sg = TRUE;

					strcpy_s( (char*)_getDATA( dt_ln, FD_CKOJI_PN)->DP_KOJI, sizeof _getDATA( dt_ln, FD_CKOJI_PN)->DP_KOJI, koji );

					//工事名称あり
					if( kj_etc[0] != '\0' ) {
						strcpy_s( _getCND( dt_ln, FD_CKOJI_PN)->ETC_msg, sizeof _getCND( dt_ln, FD_CKOJI_PN)->ETC_msg, kj_etc );
						_getCND( dt_ln, FD_CKOJI_PN)->ETC_col = kj_etccol;

						if( bDisp ) {
							sk.bmn	= -1;
							sk.kno	= koji;
							sk.code.Empty();	
							sk.brn	= -1;	
							sk.etc	= kj_etc;	
							sk.etc_col	= kj_etccol;

							pDlg->SetKoji( ln, FD_CKOJI_PN, koji, &sk );
						}
					}
					else {
						::ZeroMemory( _getCND( dt_ln, FD_CKOJI_PN)->ETC_msg, sizeof _getCND( dt_ln, FD_CKOJI_PN)->ETC_msg );
						_getCND( dt_ln, FD_CKOJI_PN)->ETC_col = 0;
						if( bDisp ) {
							pDlg->SetKoji( ln, FD_CKOJI_PN, koji, NULL );
						}
					}
				}
				set_sgn = 1;
			}
		}

		if( ! set_sgn ) {
			// 貸方科目なしか、単一部門入力につき、仕訳データ部分はクリア
			if( m_DI.IsBumon() ) {
				if( bDisp ) {
					pDlg->SetBmon( ln, FD_CBMN_PN, -1, NULL );
				}
				_getDATA( dt_ln, FD_CBMN_PN )->DP_BMN = -1;
				_getCND( dt_ln, FD_CBMN_PN)->INP_sg = FALSE;
			}
			if( m_DI.IsKoji() ) {
				if( bDisp ) {
					pDlg->SetKoji( ln, FD_CKOJI_PN, empty_str, NULL );
				}
				::ZeroMemory( _getDATA( dt_ln, FD_CKOJI_PN)->DP_KOJI, sizeof _getDATA( dt_ln, FD_CKOJI_PN)->DP_KOJI );
				_getCND( dt_ln, FD_CKOJI_PN)->INP_sg = FALSE;
			}
		}

		if( bDisp ) {
			ln++;
		}
	}
}



int CDnpBaseClass::ChangeDnpyoInput()
{
TRACE( "ChangeDnpyoInput TOP\n" );

	// 伝票イニシャライズ＆表示
#ifdef LATER_CLOSE
	((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar(4, IsType()==TYPE_FUKUGOU ? 1 : 0 );
	m_pZm->dbdata_Job()		= DBDATA_JOB_DENP;
	m_pZm->dbdata_Sort()	= DBDATA_SORT_SEQ;
	m_pZm->dbdata_Requery()	= TRUE;
	m_pZm->dbdata_Speed()	= FALSE;
	m_pZm->dbdata->Requery(-2,0,"seq=-1",0,0);	// for speed up from scan

//	m_selDate.Iniz( m_pZm, ICS_DATESELECT_RANGE_PERIOD|ICS_DATESELECT_EDIT_OFF|ICS_DATESELECT_INPOK_ONLY, 0, 0 );
	int sofs, eofs, sdate, edate;
	m_pZm->get_datelimit( sofs, eofs, sdate, edate );
	m_selDate.SetMonthOffset( sofs, eofs, sdate, edate );
	m_selDate.GetMonthOffset( sofs, eofs, sdate, edate );
#endif

	BYTE pack[16] = {0};
	BYTE bcd[10] = {0};

	bcd[0] = getDATA( DP_MM_PN )->DP_BCDDAY[0];
	bcd[1] = getDATA( DP_DD_PN )->DP_BCDDAY[0];

	// 通常伝票画面モードをセット
	m_DPscan.DenpScanMode() = 0;

#ifdef LATER_CLOSE
	// 以前の入力を生かす
	if( ! m_pZm->check_datelimit( bcd ) ) {
		m_pZm->GetInpDate(pack);
		getDATA( DP_MM_PN )->DP_BCDDAY[0] = pack[0];
		getCND( DP_MM_PN )->INP_sg = TRUE;
		getDATA( DP_DD_PN )->DP_BCDDAY[0] = pack[1];
		getCND( DP_DD_PN )->INP_sg = TRUE;
	}
	else {
		pack[0] = bcd[0];
		pack[1] = bcd[1];
		getCND( DP_MM_PN )->INP_sg = TRUE;
		getCND( DP_DD_PN )->INP_sg = TRUE;
	}
#endif
	char bin1[6], bin2[6];
	m_DI.CalqSyogTotal( SYOGT, (char *)bin1, (char *)bin2, (char *)SYOG_TAI );

	int paint_sgn = 0;
	// 10.16 /12
	diagctrl_job();
	m_DI.SetInpDate(-1, 0);

	ChangeKind( KIND_FURIKAE );

	if( m_DataSeq == -1 ) {
		ChangeType( IsDefaultType() );
		DenpDlgDisp( KIND_FURIKAE );
		// 日付の表示
		GetNowDnpDialog()->SetMM( DP_MM_PN, pack[0] );
		GetNowDnpDialog()->SetDD( DP_DD_PN, pack[1] );
	}
	else {
		// 伝票検索モードをセット
		m_DPscan.DenpScanMode() = 1;
		CString filter;
		filter.Format( "seq = %d", m_DataSeq );
		m_DPscan.DenpScanOpen( filter, 0 );

		if( m_DPscan.SetLastSheet(m_DataSeq) == 0 ) {

			do {
				//表示
				if( CheckDenpyoData(-1) == 0 ) {
					break;
				}
				else {
					return -1;
				}
#ifdef LATER_CLOSE
				// 表示できない伝票があった場合は、それより前の伝票を セットする
				if( m_DPscan.SetPrevSheet( TRUE ) < 0 ) {
					// スキャン終了
					// 表示する伝票がない
					return;
				}
#endif
			} while(1);

			ChangeJob( JOB_SCAN );
//			DenpDlgDisp( KIND_FURIKAE );
			DisplayScanData();
		}
		else {
			if( /*bSCAN_ONLY_MODE ||*/ m_DI.IsConFirmMaster() ) {
				return -1;
			}
		}
	}
	// "（決修）"チェック＆表示
	kesyu_chkdsp();

	if( IsKind() == KIND_FURIKAE ) {
		syogdsp();
	}

	// dbedit を元に戻す
	dbedt_putback();

	if( m_DataSeq == -1 ) {
		set_nowln( 1 );
		set_nowpn( DP_DENP_PN );
		_set_focus( -1, DP_DENP_PN );
	}
	else {
		SetDenpyoLine(m_DataSeq);
	}

TRACE( "ChangeDnpyoInput END\n" );

	return 0;
}


// 伝票を表示
void CDnpBaseClass::DenpDlgDisp( int type )
{
	CRect rect, rc, rc2;
	int ht, wd;
	m_pselFix8->GetWindowRect( &rc );
	m_pDnpParent->ScreenToClient( rc );
	// ダイアログ開始Y座標用
	m_pselSel32->GetWindowRect( &rc2 );
	m_pDnpParent->ScreenToClient( rc2 );
//TRACE( "type = %d left top %d,%d, right bottom %d,%d", type, rc.left, rc.top, rc.right, rc.bottom );

	IsKind() = type;

	CRect vwRect;
	m_pDnpParent->GetClientRect( &vwRect );

	if( type == KIND_FURIKAE ) {
		m_pFuri->GetWindowRect( rect );
		m_pDnpParent->ScreenToClient( &rect );

//		ht = vwRect.Height();
		ht = rect.Height();
		wd = vwRect.Width();

		if( wd < rect.Width() ) wd = rect.Width();

//		ht = rect.Height();
//		wd = rect.Width();

		m_pNSdlg->ShowWindow( SW_HIDE );

		rect.left = rc.left;
		rect.top = rc2.bottom;
		rect.bottom = rect.top + ht;
		rect.right = rect.left + wd;

		m_pFuri->MoveWindow( &rect );
		m_pFuri->SetWindowPos( m_pDnpParent, 0,0,0,0,SWP_NOMOVE | SWP_NOSIZE );
		m_pFuri->ShowWindow( SW_SHOW );
	
//		m_pFuri->Invalidate();
//		m_pFuri->UpdateWindow();

	}
	else {
		m_pNSdlg->GetWindowRect( rect );
		m_pDnpParent->ScreenToClient( &rect );

		ht = rect.Height();
		wd = vwRect.Width();

		if( wd < rect.Width() ) wd = rect.Width();
//		ht = rect.Height();
//		wd = rect.Width();
		rect.left = rc.left;
		rect.top = rc2.bottom;
		rect.bottom = rect.top + ht;
		rect.right = rect.left + wd;

		m_pFuri->ShowWindow( SW_HIDE );

		m_pNSdlg->MoveWindow( &rect );
		m_pNSdlg->SetWindowPos( m_pDnpParent, 0,0,0,0,SWP_NOMOVE | SWP_NOSIZE );
		m_pNSdlg->ShowWindow( SW_SHOW );
		m_pNSdlg->ChangeDenpType( type );

//		m_pNSdlg->Invalidate();
//		m_pNSdlg->UpdateWindow();

	}
}


//ダイアグラム 入力状態
void CDnpBaseClass::diagctrl_job()
{
	m_pFuri->diag_disableinz();
	m_pNSdlg->diag_disableinz();

	m_pFuri->KanaSearchUpdate();
	m_pNSdlg->KanaSearchUpdate();
}


//----------------------------------------------------
//	部門未使用マスターなど入力サインを設定する
//
//----------------------------------------------------
void CDnpBaseClass::inpctl_inz()
{
	int n, bksw;
	BOOL ena;
	
ASSERT( m_pFuri != NULL );
ASSERT( m_pNSdlg != NULL );

	// 伝票番号
	_getCND( -1, DP_DENP_PN )->EDT_sg	=	0;
	// 月
	_getCND( -1, DP_MM_PN )->EDT_sg	=	0;
	// 日
	_getCND( -1, DP_DD_PN )->EDT_sg	=	0;
	// 部門・工事
	_getCND( -1, DP_BMN_PN )->EDT_sg	=	0;
	_getCND( -1, DP_KOJI_PN )->EDT_sg	=	0;

	// ヘッド部
	for( n = DP_DENP_PN; n <= DP_KOJI_PN; n++ ) {

		switch( n ) {
		case DP_SYONUM_PN:
			if( m_DI.IsSyonum() )	ena = TRUE;
			else				ena = FALSE;
			break;

		case DP_BMN_PN:
			if( m_DI.IsBumon() )	ena = TRUE;
			else			ena = FALSE;
			break;

		case DP_KOJI_PN:
			if( m_DI.IsKoji() )	ena = TRUE;
			else			ena = FALSE;
			break;
		default:
			ena = TRUE;
			break;
		}
		if( n == DP_SYONUM_PN ) {
			m_pFuri->ctl_init(n, ena);
		}
		m_pNSdlg->ctl_init( n, ena );
	}

	// 仕訳データ部
	for( n = FD_DBVAL_PN; n < FD_MAX_PN; n++ ) {
		bksw = 0;
		switch( n ) {
		case FD_DBMN_PN:
		case FD_CBMN_PN:
			if( m_DI.IsBumon() )	ena = TRUE;
			else			ena = FALSE;
			bksw = 1;
			break;

		case FD_DKOJI_PN:
		case FD_CKOJI_PN:
			if( m_DI.IsKoji() )	ena = TRUE;
			else			ena = FALSE;
			bksw = 1;
			break;

		case FD_HJIYU_PN:
		//	if( IsOKGyosyu( m_pZm->zvol->apno, SW_SOCIAL|SW_SYUKYO)/* == 0x52*/ )
			if( m_DI.IsHjiyuItemGyosyu() == 0 ) 
				ena = FALSE;
			else
				ena = TRUE;
			break;
		default:
			ena = TRUE;
			break;
		}

		// 入金・出金は、上段の部門・工事入力で借方／貸方セットする
		m_pNSdlg->ctl_init( n, bksw ? FALSE : ena );
	}

	m_pFuri->ControlPos();
	m_pNSdlg->ControlPos();

	// 振替伝票 単一部門入力か？
	FuriModeChange( m_FuriMode );

	// 諸口枝番
	m_pFuri->syog_iniz();

	// 部門・工事の表示調整
	m_pFuri->diag_inz();
	m_pNSdlg->diag_inz();

	m_pFuri->diag_disableinz();
	m_pNSdlg->diag_disableinz();
}



//---------------------------------------------------
// 伝票イニシャライズ＆表示
//	 int mode			シートクリアサイン(DEF = SHEET_ALL)
//	 int modelessEnd	モードレスで起動している時
//---------------------------------------------------
void CDnpBaseClass::sheet_clr( int mode, int modelessEnd )
{
TRACE( "sheet_clr\n" );

	// 摘要かな検索 をしていたら、元に戻す
	if( m_DI.tkrec_kanamode() ) {
	//	m_pZm->tkrec->Requery( "", 0 );
	//	m_pZm->tkrec_kanamode() = FALSE;
		m_DI.tkrec_queryAll();
	}

	//入力データイニシャライズ
	inpdata_inz();
	//入力データマップイニシャライズ
	inzMAP();
	//入力データ表示
	inpdata_dsp();

	// 貸借データイニシャライズ
	total_inz( mode );
	// 貸借データ表示
	total_dsp();

	// "（決修）"チェック＆表示
	kesyu_chkdsp();

	// 諸口表示
	syog_dsp(SYOG_TAI);

	// 背景色クリア
	for( int ln = 1; ln <= getSCROLL_NL(); ln++ ) {
		setDATA_LINE_COLOR( ln, RGB_WHITE, FALSE );
	}
	// DBEditを戻す
	dbedt_putback();

	set_nowln( 1 );	//データ行を１行目(先頭)にセット
	set_nowpn( DP_DENP_PN );
	LineChgJob( m_CREC, get_nowln(), DP_DENP_PN );	

	if( IsKind() == KIND_FURIKAE ) {
		if( mode != SHEET_MOTIKOSI ) {
			ChangeType( IsDefaultType() );
		}
	}

	//データ位置初期化
	int cnt = getSCROLL_NL();
	int	max = get_MaxLineEX();

	m_TopIdx = 0;
	GetNowDnpDialog()->set_scrollbar( NULL, m_TopIdx, max, cnt );

	if (modelessEnd == 0) {
		set_focus(DP_DENP_PN);	//入力位置（伝票番号）にフォーカスセット
	}
}


// 入力データバッファイニシャライズ
void CDnpBaseClass::inpdata_inz()
{
	int ln, pn;

	// ヘッド部はこの関数を呼ぶ前にデータをセットする
	// 伝票番号
	memset( (char *)_getCND( 0, DP_DENP_PN ), (char)0, sizeof(struct _DNPLINE_CND) );	//コンディションサイン OFF
	if( (_getDATA( 0, DP_DENP_PN )->DP_DENP) == -1 )
		_getCND(0, DP_DENP_PN)->INP_sg = FALSE;
	else
		_getCND(0, DP_DENP_PN)->INP_sg = TRUE;

	// 月
	memset( (char *)_getCND( 0, DP_MM_PN ), (char)0, sizeof(struct _DNPLINE_CND) );	//コンディションサイン OFF
	if( _getDATA( 0, DP_MM_PN )->DP_BCDDAY[0] == 0 )
		_getCND( 0, DP_MM_PN)->INP_sg = FALSE;
	else
		_getCND( 0, DP_MM_PN)->INP_sg = TRUE;

	// 日
	memset( (char *)_getCND( 0, DP_DD_PN ), (char)0, sizeof(struct _DNPLINE_CND) );	//コンディションサイン OFF
	if( _getDATA( 0, DP_DD_PN )->DP_BCDDAY[0] == 0 )
		_getCND( 0, DP_DD_PN)->INP_sg = FALSE;
	else
		_getCND( 0, DP_DD_PN)->INP_sg = TRUE;

	//証憑番号
	if( m_DI.IsSyonum() ) {
		memset((char *)_getCND(0, DP_SYONUM_PN), (char)0, sizeof(struct _DNPLINE_CND));	//コンディションサイン OFF
		::ZeroMemory(_getDATA(0, DP_SYONUM_PN)->DP_SNUM, sizeof _getDATA(0, DP_SYONUM_PN)->DP_SNUM);
		_getCND(0, DP_SYONUM_PN)->INP_sg = FALSE;
	}

	//部門
	if( m_DI.IsBumon() ) {
		memset( (char *)_getCND( 0, DP_BMN_PN ), (char)0, sizeof(struct _DNPLINE_CND) );	//コンディションサイン OFF
		_getDATA( 0, DP_BMN_PN )->DP_BMN = -1;
		_getCND( 0, DP_BMN_PN)->INP_sg = FALSE;
	}
	//工事
	if( m_DI.IsKoji() ) {
		memset( (char *)_getCND( 0, DP_KOJI_PN ), (char)0, sizeof(struct _DNPLINE_CND) );	//コンディションサイン OFF
		::ZeroMemory( _getDATA( 0, DP_KOJI_PN )->DP_KOJI, sizeof(_getDATA( 0, DP_KOJI_PN )->DP_KOJI) );
		_getCND( 0, DP_BMN_PN)->INP_sg = FALSE;
	}

	for( ln = 1 ; ln <= get_MaxLine() ; ++ln ) {
		// 消費税モジュールのためダミーＳＥＱセット
		SEQ_DATA[ln-1] = (++DMY_SEQ ? DMY_SEQ : 1);
		++DMY_SEQ;	// 1行で2つの仕訳を作成する場合がある(消費税の借貸入力)

		for( pn = FD_DBVAL_PN; pn < FD_MAX_PN; ++pn ) {
			switch( pn ) {
			case	FD_DEBT_PN:	// 借方科目
			case	FD_CRED_PN:	// 貸方科目
				::ZeroMemory( &_getDATA( ln, pn )->DP_stKMK, sizeof (struct _KamokuData) );
				_getDATA( ln, pn )->DP_BRN = -1;
				break;

			case	FD_DSYOG_PN:	//借方諸口
			case	FD_CSYOG_PN:	//貸方諸口
				_getDATA( ln, pn )->DP_cBRN = -1;
				break;

			case	FD_DBVAL_PN:	// 借方金額
			case	FD_CRVAL_PN:	// 貸方金額
			case	FD_DBZEI_PN:	// 借方税額
			case	FD_CRZEI_PN:	// 貸方税額
				::ZeroMemory( _getDATA( ln, pn )->DP_VAL, sizeof(_getDATA( ln, pn )->DP_VAL) );
				break;
			case	FD_TKY_PN:		// 摘要
				::ZeroMemory( _getDATA( ln, pn )->DP_TKY, sizeof(_getDATA( ln, pn )->DP_TKY) );
				break;
			case	FD_DBMN_PN:		// 部門
			case	FD_CBMN_PN:
				_getDATA( ln, pn )->DP_BMN = -1;
				break;
			case	FD_DKOJI_PN:	// 工事番号
			case	FD_CKOJI_PN:
				::ZeroMemory( _getDATA( ln, pn )->DP_KOJI, sizeof(_getDATA( ln, pn )->DP_KOJI) );
				break;
			case	FD_DSKBN_PN:	// 消費税
			case	FD_CSKBN_PN:
				::ZeroMemory( _getDATA( ln, pn ), sizeof( struct _DenpData ));
				// 消費税区分
				_getDATA( ln, pn )->SYDATA.swk_sw = 0;
				_getDATA( ln, pn )->SYDATA.ritu_sw = 0;
				_getDATA( ln, pn )->SYDATA.zeikbn_sw = 0;
				_getDATA( ln, pn )->SYDATA.uri_sw = 0;
				_getDATA( ln, pn )->SYDATA.toku_sw = 0;
				_getDATA( ln, pn )->SYDATA.kts_sw = 0;
				// 消費税デフォルト
				_getDATA( ln, pn )->SYDATA.swk_def = 0;
				_getDATA( ln, pn )->SYDATA.ritu_def = 0;
				_getDATA( ln, pn )->SYDATA.zeikbn_def = 0;
				_getDATA( ln, pn )->SYDATA.uri_def = 0;
				_getDATA( ln, pn )->SYDATA.toku_def = 0;
				_getDATA( ln, pn )->SYDATA.kts_def = 0;
				break;

			case	FD_TGNUM_PN:	//手形番号
				::ZeroMemory( _getDATA( ln, pn )->DP_SNUM, sizeof _getDATA( ln, pn )->DP_SNUM );
				break;
			case	FD_TGDATE_PN:	//手形期日
				_getDATA( ln, pn )->DP_TGDAY = -1;
				::ZeroMemory( _getDATA( ln, pn )->DP_TGBCD, sizeof _getDATA( ln, pn )->DP_TGBCD );
				break;
			case	FD_HJIYU_PN:	//変動事由
				_getDATA( ln, pn )->DP_BMN = 0;
				break;

			case	FD_HUSEN_PN:	//付箋
				_getDATA( ln, pn )->DP_HUSEN = 0;
				::ZeroMemory( _getDATA( ln, pn )->DP_HCMNT, sizeof _getDATA( ln, pn )->DP_HCMNT );
				break;

			default:
				break;
			}

			//コンディションサイン OFF
			::ZeroMemory( (char *)_getCND( ln, pn ), sizeof(struct _DNPLINE_CND) );
		}
	}

	GetNowDnpDialog()->control_iniz();
}


// 入力データ表示
void CDnpBaseClass::inpdata_dsp()
{
	GetNowDnpDialog()->inpdata_dsp();
}


// 貸借持ち越し
void CDnpBaseClass::motikosi_inz( int motikosi )
{
	// 持ち越しサイン"motikosi"がたっている場合は、持ち越し残高はクリアしない。

	// 借方
	if( motikosi )
	{
		// 持ち越し
		memmove( (char*)(&ZDEBT_TOTAL), (char*)(&DEBT_TOTAL), sizeof( struct _DenpData ) );
		memmove( (char*)(&ZDEBT_TOTAL_CND), (char*)(&DEBT_TOTAL_CND), sizeof( struct _DNPLINE_CND ) );
	}
	else
	{
		// クリア
		memset( (char *)ZDEBT_TOTAL.DP_VAL, (char)0, sizeof(ZDEBT_TOTAL.DP_VAL) );	//バッファ
		ZDEBT_TOTAL_CND.INP_sg = FALSE;	// コンディションサイン
	}

	// 貸方
	if( motikosi )
	{
		// 持ち越し
		memmove( (char*)(&ZCRED_TOTAL), (char*)(&CRED_TOTAL), sizeof( struct _DenpData ) );
		memmove( (char*)(&ZCRED_TOTAL_CND), (char*)(&CRED_TOTAL_CND), sizeof( struct _DNPLINE_CND ) );
	}
	else
	{
		// クリア
		memset( (char *)ZCRED_TOTAL.DP_VAL, (char)0, sizeof(ZCRED_TOTAL.DP_VAL) );	//バッファ
		ZCRED_TOTAL_CND.INP_sg = FALSE;	// コンディションサイン
	}
}


// 貸借バッファイニシャライズ
void CDnpBaseClass::total_inz( int mode )
{
	// 持ち越しエリア
	if( mode != SHEET_TAISYAKU )
		motikosi_inz( mode == SHEET_MOTIKOSI ? 1 : 0 );

	// 借方合計
	memset( (char *)DEBT_TOTAL.DP_VAL, (char)0, sizeof(DEBT_TOTAL.DP_VAL) );	//バッファ
	DEBT_TOTAL_CND.INP_sg = FALSE;	// コンディションサイン

	// 貸方合計
	memset( (char *)CRED_TOTAL.DP_VAL, (char)0, sizeof(CRED_TOTAL.DP_VAL) );	//バッファ
	CRED_TOTAL_CND.INP_sg = FALSE;	// コンディションサイン
}



// TODO: データライン色設定[検討の余地ありか？]
//
//	int ln			画面伝票上の　行番号(１～最大７)
//	COLORREF col	変更する色
//	BOOL	fore	FALSE:背景色  TRUE:文字色
//
void CDnpBaseClass::setDATA_LINE_COLOR( int ln, COLORREF col, BOOL fore )
{
	if( ln <= 0 )	return;
	char	sgn;

	// 確定仕訳・転送済み
	if( fore == FALSE && IsJob() == JOB_SCAN && IsTransLine( ln, sgn ) ) {
		col	=	GetConfirmBackColor( sgn );
	}

	CDnpBaseDlg* pDlg = GetNowDnpDialog();
	pDlg->SetDataLineColor( ln, col, RGB_BLACK, NULL );
}



//---------------------------------------------------------------------------------
//
//	日付や、伝票番号等が、他の入力で変更されて、内部伝票番号での伝票としての
//	まとまりで表示することが　おかしいかどうかを チェックする
//	
//	return	0:	チェックOK
//			-1:	変更されているので、次 または、前の伝票を表示する
//
//---------------------------------------------------------------------------------
int CDnpBaseClass::CheckDenpyoData( int vect )
{
	CDBINPDataRec	drec;

	WORD	chkbit = 0;
	int		init_sgn = 0;
	int		base_date, base_ofset, base_cno, base_bmn;
	int		kind, ln, st, ret;
	CString	base_koji, kno, msg, base_skn;
	ret = 0;

	kind = IsKind();
//MyTrace( "CheckDenpyoData kind = %d\n", kind );
	unsigned char	dtype	=	m_DPscan.DenpyoType();
	CString dpstr;
	if( dtype == 0x30 ) {
		kind = KIND_FURIKAE;
		dpstr = "振替伝票";
	}
	else if( dtype == 0x10 ) {
		kind = KIND_NYUKIN;
		dpstr = "入金伝票";
	}
	else {
		kind = KIND_SYUKKIN;
		dpstr = "出金伝票";
	}
	//MyTrace( "CheckDenpyoData kind2 = %d\n", kind );

	for( ln = 1; ln <= get_MaxLine(); ln++ ) {

		if( m_DPscan.IsOriginData( ln, 0 ) ) {
			if( ! m_DPscan.IsCrctData( ln, 0 ) ) {
				drec	=	m_DPscan.GetOriginData( ln, 0 );

				if( ! init_sgn ) {
					base_date	= drec.m_ddate;
					base_ofset	= drec.m_mofs;

					base_cno	= drec.m_cno;
					base_skn	= drec.m_skn;

					if( kind != KIND_FURIKAE ) {
						base_bmn	= drec.m_dbmn;
						base_koji	= drec.m_dkno;
					}
					init_sgn = 1;
				}
				else {
					if( base_date != drec.m_ddate || base_ofset != drec.m_mofs ) {
						chkbit |= 0x01;
					}
					if( base_cno != drec.m_cno ) {
						chkbit |= 0x02;
					}

					if( kind != KIND_FURIKAE ) {
						if( m_DI.IsBumon() ) {
							if( base_bmn != drec.m_dbmn )
								chkbit |= 0x04;
						}
						if( m_DI.IsKoji() ) {
							kno = drec.m_dkno;
							kno.Trim();

							base_koji.Trim();

							if( base_koji != kno ) {
								chkbit |= 0x08;
							}
						}
					}
					if( m_DI.IsSyonum() ) {
						CString skn;
						skn = drec.m_skn;
						skn.Trim();

						base_skn.Trim();

						if( base_skn != skn ) {
							chkbit |= 0x10;
						}
					}
				}
			}
		}

		if( m_DPscan.IsOriginData( ln, 1 ) ) {
			if( ! m_DPscan.IsCrctData( ln, 1 ) ) {
				drec	=	m_DPscan.GetOriginData( ln, 1 );

				if( ! init_sgn ) {
					base_date	= drec.m_ddate;
					base_ofset	= drec.m_mofs;

					base_cno	= drec.m_cno;
					base_skn	= drec.m_skn;

					if( kind != KIND_FURIKAE ) {
						base_bmn	= drec.m_cbmn;
						base_koji	= drec.m_ckno;
					}
					init_sgn = 1;
				}
				else {
					if( base_date != drec.m_ddate || base_ofset != drec.m_mofs ) {
						chkbit |= 0x01;
					}
					if( base_cno != drec.m_cno ) {
						chkbit |= 0x02;
					}

					if( kind != KIND_FURIKAE ) {
						if( m_DI.IsBumon() ) {
							if( base_bmn != drec.m_cbmn )
								chkbit |= 0x04;
						}
						if( m_DI.IsKoji() ) {
							kno = drec.m_ckno;
							kno.Trim();

							base_koji.Trim();

							if( base_koji != kno ) {
								chkbit |= 0x08;
							}
						}
					}
					if( m_DI.IsSyonum() ) {
						CString skn;
						skn = drec.m_skn;
						skn.Trim();

						base_skn.Trim();

						if( base_skn != skn ) {
							chkbit |= 0x10;
						}
					}
				}
			}
		}
	}

	// 伝票で表示するのが無理な、異なる仕訳がある場合、
	if( chkbit ) {
		CString item;
		if( chkbit & 0x01 ) {
			item = "日付";
		}
		if( chkbit & 0x02 ) {
			if( !item.IsEmpty() ) item += "・";
			item += "伝票番号";
		}
		if( chkbit & 0x04 ) {
			if( !item.IsEmpty() ) item += "・";
			item += "部門";
		}
		if( chkbit & 0x08 ) {
			if( !item.IsEmpty() ) item += "・";
			item += "工事番号";
		}
		if( chkbit & 0x10 ) {
			if( !item.IsEmpty() ) item += "・";
			item += "証憑番号";
		}

		CString stmp;
		if( vect != 0 ) {
			msg.Format("%sが異なる仕訳があるため、%sを表示できません。\r\n", item, dpstr);
			stmp.Format("%sを統一して%sを表示しますか？\r\n\r\n", item, dpstr);
			msg += stmp;

			st = myICSMessageBox(msg, MB_YESNO, 0, 0, m_pDnpParent);

			if( st == IDNO )	ret = -1;
		}
		else {
			// 再読み込み時など、メッセージなし
			msg.Format("%sが異なる仕訳があるため、%sを表示できません。\r\n", item, dpstr);
			msg += _T("新規 伝票入力に戻ります。\r\n\r\n");
			ret = -1;
		}
	}

	return ret;
}



//-----------------------------------------------
//	新規伝票入力かスキャンかの切り替え
//
//-----------------------------------------------
void CDnpBaseClass::ChangeJob( int mode )
{
	IsJob()	=	mode;
	m_pFuri->ChangeJob( IsJob() );
	m_pNSdlg->ChangeJob( IsJob() );

#ifdef LATER_CLOSE
	if( mode == JOB_INPUT )
		((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar( 4, 0 );
	else if( mode == JOB_SCAN )
		((CMainFrame*)GetDBDinpMainFrame())->ChangeToolBar( 4, 10 );
#endif
}



//---------------------------------------------------------------------------
//	スキャンデータをセット
//
//---------------------------------------------------------------------------
void CDnpBaseClass::DisplayScanData()
{
	CDBINPDataRec	drec;
	int		i, ln;

	//伝票貸借合計イニシャライズ
	memset( kariTotal, 0, sizeof kariTotal );
	memset( kasiTotal, 0, sizeof kasiTotal );
	// 取消ライン
	memset( tblDel, 0, sizeof tblDel );
	//イメージバッファイニシャライズ
	memset( tblImage, 0, sizeof tblImage );
	memset( tblImageOfst, 0, sizeof tblImageOfst );

	int max = get_MaxLine();
	
	for( i = 0; i < max; i++ ) {
		tblTekiyo[i].Empty();	// 貸方文字摘要

		// 手形番号
		tblTgnum[i][0].Empty();
		tblTgnum[i][1].Empty();
		// 付箋コメント
		tblHcmnt[i][0].Empty();
		tblHcmnt[i][1].Empty();
	}

	::ZeroMemory( tblTgdate, sizeof tblTgdate );	// 手形期日
	::ZeroMemory( tblHjiyu, sizeof tblHjiyu );		// 変動事由
	::ZeroMemory( tblHusen, sizeof tblHusen );		// 付箋

#ifdef OLD_CLOSE
	//一枚だけの伝票及び、二枚以上続いている場合の一枚目
//	if( m_DPscan.GetSheetCount() <= 1 ||
//		((m_DPscan.GetSheetCount() > 1) && (m_DPscan.GetCurrentSheet() == 0) && mode != -1) ) {
	if( !mode ) {
		//入力項目状態テーブル初期化
		m_DPscan.MakeSheetTable();
		m_DPscan.SetFirstSheet();
	}
#endif

	//自動仕訳モード
	if( IsMode() == MODE_AUTO ) {
		IsMode()	=	MODE_NORMAL;
		GetNowDnpDialog()->AutoSiwake( IsMode() );
	}
	//伝票種別
	unsigned char	dtype	=	m_DPscan.DenpyoType();
	if( dtype == 0x30 )			OnButtonChangeKind( KIND_FURIKAE );
	else if( dtype == 0x10 )	OnButtonChangeKind( KIND_NYUKIN );
	else if( dtype == 0x20 )	OnButtonChangeKind( KIND_SYUKKIN );

	// 摘要かな検索 をしていたら、元に戻す
	if( m_DI.tkrec_kanamode() ) {
	//	m_pZm->tkrec->Requery( "", 0 );
	//	m_pZm->tkrec_kanamode() = FALSE;
		m_DI.tkrec_queryAll();
	}

	//入力コントロールイニシャライズ
	inpctl_inz();
	//入力データイニシャライズ
	inpdata_inz();
	//入力データマップイニシャライズ
	inzMAP();
	//入力データ表示
	inpdata_dsp();
	// 貸借データイニシャライズ
	total_inz();

	// 選択背景色を戻す
	// 背景色クリア
	for( ln = 1; ln <= getSCROLL_NL(); ln++ ) {
		setDATA_LINE_COLOR( ln, RGB_WHITE, FALSE );
	}

	//データ位置初期化
	int cnt = getSCROLL_NL();

	m_TopIdx = 0;
	GetNowDnpDialog()->set_scrollbar( NULL, -1, get_MaxLineEX(), cnt );

	// DBEditを戻す
	dbedt_putback();

	set_nowln( 1 );	//データ行を０行目にセット
	set_nowpn( DP_DENP_PN );

	int	sw	=	0;
	int seq;

	// 振替伝票のとき、単一 部門・工事入力にできるか？
	if( IsKind() == KIND_FURIKAE ) {
		if( m_DI.IsBumon() || m_DI.IsKoji() ) {
			if( CheckFuriBmnKoji() == 0 ) {
				FuriModeChange( FURI_TANBMON );
			}
			else {
				FuriModeChange( FURI_DATABMON );
			}
		}
	}

	char	sy_chg[4] = {0};

	for( ln = 1; ln <= get_MaxLine(); ln++ ) {
		sw	=	0;
		::ZeroMemory(sy_chg, sizeof sy_chg);
		MyTrace("DisplayScanData ln= %d\n", ln);

		if( m_DPscan.IsOriginData( ln, 0 ) ) {
			if( m_DPscan.IsCrctData( ln, 0 ) ) {
				m_DPscan.GetCrctData( ln, 0, &drec );
			}
			else {
				drec	=	m_DPscan.GetOriginData( ln, 0 );
			}
			sw	=	1;
		}
		else if( m_DPscan.IsAddData( ln, 0 ) ) {
			m_DPscan.GetCrctData( ln, 0, &drec );
			sw	=	1;
		}
		MyTrace("DisplayScanData sw= %d\n", sw);

		if( sw ) {
			SetScanData( drec, ln, 0 );
			seq	=	m_DPscan.GetSeq( ln, 0 );
			if( seq && seq != -1 ) {
				TransLineSign( &drec, ln );
				sy_chg[0] = _getDATA( ln, FD_DSKBN_PN )->SYDATA.sy_chg;
				sy_chg[2] = _getCND(ln, FD_DBZEI_PN)->INP_type;
				TransLine( ln, ln, sy_chg );
			}
		}

		sw	=	0;
		if( m_DPscan.IsOriginData( ln, 1 ) ) {
			if( m_DPscan.IsCrctData( ln, 1 ) ) {
				m_DPscan.GetCrctData( ln, 1, &drec );
			}
			else {
				drec	=	m_DPscan.GetOriginData( ln, 1 );
			}
			sw	=	1;
		}
		else if( m_DPscan.IsAddData( ln, 1 ) ) {
			m_DPscan.GetCrctData( ln, 1, &drec );
			sw	=	1;
		}
		MyTrace("DisplayScanData_2 sw= %d\n", sw);

		if( sw ) {
			SetScanData( drec, ln, 1 );
			seq	=	m_DPscan.GetSeq( ln, 1 );
			if( seq && seq != -1 ) {
				TransLineSign( &drec, ln );
				sy_chg[0] = _getDATA( ln, FD_DSKBN_PN )->SYDATA.sy_chg;
				sy_chg[1] = _getDATA( ln, FD_CSKBN_PN )->SYDATA.sy_chg;
				sy_chg[2] = _getCND(ln, FD_DBZEI_PN)->INP_type;
				sy_chg[3] = _getCND(ln, FD_CRZEI_PN)->INP_type;
				TransLine( ln, ln, sy_chg );
			}
		}
		if( m_DPscan.DeleteLine( ln ) ) {
			SetDelSgn( ln, 1 );
		//	GetNowDnpDialog()->DeleteLine( ln, 1 );
		//	Lineattr_dsp();
		}
	}
	SRCTrace;

	if( IsKind() == KIND_FURIKAE ) {
		//合計金額計算
		::ZeroMemory( kariTotal, sizeof kariTotal );
		::ZeroMemory( kasiTotal, sizeof kasiTotal );

		ScanTotalClc( kariTotal, kasiTotal );
	}
	SRCTrace;
//	Lineattr_dsp();
	// 消費税区分新データ作成
	LineChgJob( m_CREC, get_nowln(), get_nowpn() );

	// "（決修）"チェック＆表示
	kesyu_chkdsp();

	//貸借合計
	total_dsp();

	// 諸口貸借表示
	syogdsp();
	// 基本科目表示
	DispBaseKmk();

	SRCTrace;

	_set_focus( -1, DP_DENP_PN );
	set_nowln( 1 );
	set_nowpn( DP_DENP_PN );
}


// 伝票変更関数
void CDnpBaseClass::OnButtonChangeKind( UINT kind, BOOL bNoCheck/* = FALSE*/ )
{
	if( ! bNoCheck ) {
		if( IsKind() == kind ) {
			int	pn	=	get_nowpn();
			int	ln	=	get_nowln();
			_set_focus( ln, pn );
			return;
		}
	}
	ChangeKind( kind );
}


//---------------------------------------------------------------------------------
//
//	振替伝票時の 伝票修正で、借方・貸方の部門・工事が異なるかをチェックする
//		同じ場合は、単一部門入力モードで処理する
//
//	
//	return	0:	同じ部門・工事番号
//			D0: ON	部門が伝票で異なる
//			D1: ON	工事が伝票で異なる
//
//---------------------------------------------------------------------------------
int CDnpBaseClass::CheckFuriBmnKoji()
{
	CDBINPDataRec	drec;

	int		chkbit = 0;
	int		init_sgn = 0;
	int		base_bmn;
	int		kind, ln, ret;
	CString	base_koji, kno, msg;
	ret = 0;

	kind = IsKind();
	if( kind != KIND_FURIKAE )
		return 0;

	for( ln = 1; ln <= get_MaxLine(); ln++ ) {

		if( m_DPscan.IsOriginData( ln, 0 ) ) {
			if( ! m_DPscan.IsCrctData( ln, 0 ) ) {
				drec	=	m_DPscan.GetOriginData( ln, 0 );

				if( ! init_sgn ) {
					base_bmn	= drec.m_dbmn;
					base_koji	= drec.m_dkno;
					init_sgn = 1;
				}
				else {

					if( m_DI.IsBumon() ) {
						if( base_bmn != drec.m_dbmn )
							chkbit |= 0x01;
					}
					if( m_DI.IsKoji() ) {
						kno = drec.m_dkno;
						kno.Trim();

						base_koji.Trim();

						if( base_koji != kno ) {
							chkbit |= 0x02;
						}
					}
				}
			}
		}

		if( m_DPscan.IsOriginData( ln, 1 ) ) {
			if( ! m_DPscan.IsCrctData( ln, 1 ) ) {
				drec	=	m_DPscan.GetOriginData( ln, 1 );

				if( ! init_sgn ) {
					base_bmn	= drec.m_cbmn;
					base_koji	= drec.m_ckno;
					init_sgn = 1;
				}
				else {
					if( m_DI.IsBumon() ) {
						if( base_bmn != drec.m_cbmn )
							chkbit |= 0x01;
					}
					if( m_DI.IsKoji() ) {
						kno = drec.m_ckno;
						kno.Trim();

						base_koji.Trim();

						if( base_koji != kno ) {
							chkbit |= 0x02;
						}
					}
				}
			}
		}
	}

	return chkbit;
}


//
//	検索で取得した、伝票の仕訳を 画面に表示する
//		振替伝票の場合は、必ず、部門・工事の 単一入力 かどうか？のチェックをしてから この関数を呼ぶ
//		
//		int ln .... データ行(1から100)
//
void CDnpBaseClass::SetScanData( CDBINPDataRec& data, int ln, int karikasi )
{
	//取消仕訳
	if( data.m_dsign[0] & 0x01 ) {
		if( !m_DPscan.DeleteLine( ln ) )	return;
	}

	BOOL bDisp = FALSE;
	if( ln <= getSCROLL_NL() )	bDisp = TRUE;

	struct _DenpData	check = {0}, *keybuf;
	DBKNREC	*knrec;
	int kind, zeisgn, pos;
	CString kno;
	char	edabuf[30];
	SETMEISYO	sm;
	_SET_KMKETC	sk;

	CDnpBaseDlg* pDlg;
	pDlg = GetNowDnpDialog();

	SRCTrace;

	// 消費税を表示するポジション( 0=借方, 1=貸方, -1=なし)
	zeisgn = -1;
	if( data.m_dbt == SYOGT )	zeisgn = 1;
	else if( data.m_cre == SYOGT )	zeisgn = 0;
	else {
		// 両方とも消費税科目で、単一仕訳
		if( m_DI.km_syattr( data.m_dbt ) && m_DI.km_syattr( data.m_cre ) ) {
			zeisgn = 2;
		}
		else if( m_DI.km_syattr( data.m_dbt ) ) {
			zeisgn = 0;
		}
		else if( m_DI.km_syattr( data.m_cre ) ) {
			zeisgn = 1;
		}
	}

//	if( ln == 1 ) {
		//伝票番号
		check.DP_DENP	=	data.m_cno;
		keybuf	=	_getDATA( -1, DP_DENP_PN );
		keybuf->DP_DENP	=	check.DP_DENP;
		if( data.m_cno == -1 )	_getCND( -1, DP_DENP_PN )->INP_sg	=	FALSE;
		else					_getCND( -1, DP_DENP_PN )->INP_sg	=	TRUE;
		pDlg->SetDnpyoNo( DP_DENP_PN, check.DP_DENP );
		
		// 月日をBCDで取得
		BYTE pack[4] = {0};
		m_DI.DateConv( pack, &data, CONV2_1 );
		//月
		_getDATA( 0, DP_MM_PN )->DP_BCDDAY[0] = pack[0];
		_getCND( 0, DP_MM_PN )->INP_sg = TRUE;
		pDlg->SetMM( DP_MM_PN, pack[0] );

		//日
		_getDATA( 0, DP_DD_PN )->DP_BCDDAY[0] = pack[1];
		_getCND( 0, DP_DD_PN )->INP_sg = TRUE;
		pDlg->SetDD( DP_DD_PN, pack[1] );

		if( m_DI.IsSyonum() ) {
			//伝票番号
			strcpy_s((char*)check.DP_SNUM, sizeof check.DP_SNUM, (char*)(LPCTSTR)data.m_skn);
			keybuf = _getDATA(-1, DP_SYONUM_PN);
			memcpy(keybuf->DP_SNUM, check.DP_SNUM, sizeof check.DP_SNUM);
			if( data.m_skn.IsEmpty() )	_getCND(-1, DP_SYONUM_PN)->INP_sg = FALSE;
			else						_getCND(-1, DP_SYONUM_PN)->INP_sg = TRUE;
			pDlg->SetTgNumber(-1, DP_SYONUM_PN, (char*)check.DP_SNUM);
		}
//	}

	//部門
	kind = IsKind();
	// 振替伝票時の 単一 部門・工事入力か？
	BOOL bTanBmn = FALSE;
	if( kind == KIND_FURIKAE ) {
		if( m_FuriMode == FURI_TANBMON )
			bTanBmn = TRUE;
	}

	if( m_DI.IsBumon() )	{
		if( kind != KIND_FURIKAE || bTanBmn /*&& ln == 1*/ ) {
			if( kind == KIND_NYUKIN ) {
				check.DP_BMN = data.m_dbmn;
				//反対側に部門あり
				if( check.DP_BMN == -1 && data.m_cbmn != -1 ) {
					check.DP_BMN = data.m_cbmn;
				}
			}
			else if( kind == KIND_SYUKKIN ) {
				check.DP_BMN = data.m_cbmn;
				//反対側に部門あり
				if( check.DP_BMN == -1 && data.m_dbmn != -1 ) {
					check.DP_BMN = data.m_dbmn;
				}
			}
			else {
				if( isSYOGT_CODE( data.m_dbt ) )
					check.DP_BMN = data.m_cbmn;
				else
					check.DP_BMN = data.m_dbmn;
			}

			keybuf	=	_getDATA( -1, DP_BMN_PN );

			if( keybuf->DP_BMN == -1 ) {
				keybuf->DP_BMN = check.DP_BMN;
				if( check.DP_BMN == -1 )
					_getCND( 0, DP_BMN_PN )->INP_sg	=	FALSE;
				else
					_getCND( 0, DP_BMN_PN )->INP_sg	=	TRUE;
			}
			else {
				check.DP_BMN = keybuf->DP_BMN;
			}

			struct _SET_KMKETC sk;
			sk.bmn = check.DP_BMN;
			sk.brn = -1;

			if( GetBmnEtc( &sk ) ) {
				strcpy_s( getCND(DP_BMN_PN)->ETC_msg, sizeof getCND(DP_BMN_PN)->ETC_msg, sk.etc );
				getCND(DP_BMN_PN)->ETC_col = sk.etc_col;

				pDlg->SetBmon( -1, DP_BMN_PN, check.DP_BMN, &sk );
			}
			else {
				::ZeroMemory( getCND(DP_BMN_PN)->ETC_msg, sizeof getCND(DP_BMN_PN)->ETC_msg );
				getCND(DP_BMN_PN)->ETC_col = 0;

				pDlg->SetBmon( -1, DP_BMN_PN, check.DP_BMN, NULL );
			}
		}
	}
	//工事
	if( m_DI.IsKoji() ) {
		if( kind != KIND_FURIKAE || bTanBmn /*&& ln == 1*/ ) {
			if( kind == KIND_NYUKIN ) {
				kno = data.m_dkno;
				if( kno.IsEmpty() && ! data.m_ckno.IsEmpty() )
					kno = data.m_ckno;
			}
			else if( kind == KIND_SYUKKIN ) {
				kno = data.m_ckno;
				if( kno.IsEmpty() && ! data.m_dkno.IsEmpty() )
					kno = data.m_dkno;
			}
			else {
				if( isSYOGT_CODE( data.m_dbt ) )
					kno = data.m_ckno;
				else
					kno = data.m_dkno;
			}

			keybuf	=	_getDATA( -1, DP_KOJI_PN );

			if( keybuf->DP_KOJI[0] == 0 ) {
				strcpy_s( (char*)keybuf->DP_KOJI, sizeof keybuf->DP_KOJI, kno );

				if( kno.IsEmpty() )
					_getCND( 0, DP_KOJI_PN )->INP_sg	=	FALSE;
				else
					_getCND( 0, DP_KOJI_PN )->INP_sg	=	TRUE;
			}
			else {
				kno =  keybuf->DP_KOJI;
			}

			// 工事名称
			struct _SET_KMKETC sk;
			sk.bmn = -1;
			sk.kno = keybuf->DP_KOJI;
			sk.code.Empty();
			sk.brn = -1;

			if( GetKojiEtc( &sk ) ) {
				strcpy_s( getCND(DP_KOJI_PN)->ETC_msg, sizeof getCND(DP_KOJI_PN)->ETC_msg, sk.etc );
				getCND(DP_KOJI_PN)->ETC_col = sk.etc_col;

				pDlg->SetKoji( -1, DP_KOJI_PN, kno, &sk );
			}
			else {
				::ZeroMemory( getCND(DP_KOJI_PN)->ETC_msg, sizeof getCND(DP_KOJI_PN)->ETC_msg );
				getCND(DP_KOJI_PN)->ETC_col = 0;

				pDlg->SetKoji( -1, DP_KOJI_PN, kno, NULL );
			}
		}
	}

	SRCTrace;

	//借方金額
	if( !karikasi ) {
		memmove( check.DP_VAL, data.m_val, sizeof data.m_val );
		if( zeisgn ) {
			if( sotozei_data( &data )) {
				l_add( check.DP_VAL, check.DP_VAL, data.m_zei );
			}
		}

		if( bDisp )	pDlg->SetVal( ln, FD_DBVAL_PN, (char*)check.DP_VAL );

		_getCND( ln, FD_DBVAL_PN )->INP_sg	=	TRUE;
		_getCND( ln, FD_DBVAL_PN )->SET_sg	=	FALSE;
		keybuf	=	_getDATA( ln, FD_DBVAL_PN );
		memmove( keybuf->DP_VAL, check.DP_VAL, sizeof keybuf->DP_VAL );
	}

	if( kind == KIND_FURIKAE && !karikasi ) {
		// 借方部門 [仕訳ごとに入力]
		if( m_DI.IsBumon() && ! bTanBmn )	{
			check.DP_BMN = data.m_dbmn;
			keybuf	=	_getDATA( ln, FD_DBMN_PN );
			keybuf->DP_BMN = check.DP_BMN;

			if( check.DP_BMN == -1 )
				_getCND( ln, FD_DBMN_PN )->INP_sg	=	FALSE;
			else
				_getCND( ln, FD_DBMN_PN )->INP_sg	=	TRUE;

			struct _SET_KMKETC sk;
			sk.bmn = check.DP_BMN;
			sk.brn = -1;

			if( GetBmnEtc( &sk ) ) {
				strcpy_s( _getCND(ln, FD_DBMN_PN)->ETC_msg, sizeof _getCND(ln, FD_DBMN_PN)->ETC_msg, sk.etc );
				_getCND(ln, FD_DBMN_PN)->ETC_col = sk.etc_col;

				if( bDisp )	pDlg->SetBmon( ln, FD_DBMN_PN, check.DP_BMN, &sk );
			}
			else {
				::ZeroMemory( _getCND(ln, FD_DBMN_PN)->ETC_msg, sizeof _getCND(ln, FD_DBMN_PN)->ETC_msg );
				_getCND(ln, FD_DBMN_PN)->ETC_col = 0;

				if( bDisp )	pDlg->SetBmon( ln, FD_DBMN_PN, check.DP_BMN, NULL );
			}
		}
		// 借方工事
		if( m_DI.IsKoji() && ! bTanBmn ) {
			kno = data.m_dkno;
			keybuf	=	_getDATA( ln, FD_DKOJI_PN );
			strcpy_s( (char*)keybuf->DP_KOJI, sizeof keybuf->DP_KOJI, kno );

			if( kno.IsEmpty() )
				_getCND( ln, FD_DKOJI_PN )->INP_sg	=	FALSE;
			else
				_getCND( ln, FD_DKOJI_PN )->INP_sg	=	TRUE;

			// 工事名称
			struct _SET_KMKETC sk;
			sk.bmn = -1;
			sk.kno = kno;
			sk.code.Empty();
			sk.brn = -1;

			if( GetKojiEtc( &sk ) ) {
				strcpy_s( _getCND(ln, FD_DKOJI_PN)->ETC_msg, sizeof _getCND(ln, FD_DKOJI_PN)->ETC_msg, sk.etc );
				_getCND(ln, FD_DKOJI_PN)->ETC_col = sk.etc_col;

				if( bDisp )	pDlg->SetKoji( ln, FD_DKOJI_PN, kno, &sk );
			}
			else {
				::ZeroMemory( _getCND(ln, FD_DKOJI_PN)->ETC_msg, sizeof _getCND(ln, FD_DKOJI_PN)->ETC_msg );
				_getCND(ln, FD_DKOJI_PN)->ETC_col = 0;

				if( bDisp )	pDlg->SetKoji( ln, FD_DKOJI_PN, kno, NULL );
			}
		}
	}
	SRCTrace;

	//借方科目
	if( !karikasi ) {
		::ZeroMemory( &check, sizeof check );
		strcpy_s( check.DP_CdNum, sizeof keybuf->DP_CdNum, data.m_dbt );
		check.DP_BRN = data.m_dbr;
		knrec	=	NULL;
		knrec	=	DB_PjisToKnrec( data.m_dbt, m_pZm, TRUE );
		if( knrec ) {
			KamokuString( knrec, check.DP_KNAM, sizeof check.DP_KNAM );
		//	memmove( check.DP_KNAM, (char*)knrec->knnam, sizeof knrec->knnam );
		}

		if( m_DI.IsBumon() ) {
			if( kind == KIND_FURIKAE ) {
				sk.bmn = data.m_dbmn;
			}
			else {
				sk.bmn = getDATA( DP_BMN_PN )->DP_BMN;
			}
		}
		else {
			sk.bmn = -1;
		}
		sk.code = check.DP_CdNum;
		sk.brn  = check.DP_BRN;

		if( m_DI._set_kmketc( &sk ) ) {
			strcpy_s( _getCND( ln, FD_DEBT_PN )->ETC_msg, sizeof _getCND( ln, FD_DEBT_PN )->ETC_msg, sk.etc );
			_getCND( ln, FD_DEBT_PN )->ETC_col = sk.etc_col;
			
			strcpy_s( sm.sm_etc, sizeof sm.sm_etc, _getCND( ln, FD_DEBT_PN )->ETC_msg );
			sm.sm_etccol = _getCND( ln, FD_DEBT_PN )->ETC_col;
		}
		else {
			::ZeroMemory( sm.sm_etc, sizeof sm.sm_etc );
		}

		set_codestr( edabuf, sizeof edabuf, check.DP_BRN, m_pZm->zvol->edcol );

		if( edabuf[0] )
			sprintf_s( sm.sm_name, sizeof sm.sm_name, "%s%s", check.DP_KNAM, edabuf );
		else
			strcpy_s( sm.sm_name, sizeof sm.sm_name, check.DP_KNAM );
		
		if( bDisp )	pDlg->SetKnam( ln, FD_DEBT_PN, &sm );

		_setMAP( ln, FD_DEBT_PN );
		_getCND( ln, FD_DEBT_PN )->INP_sg		=	TRUE;
		_getCND( ln, FD_DEBT_PN )->INP_type	=	brn_dfltinptype( data.m_dbr );
		keybuf	=	_getDATA( ln, FD_DEBT_PN );
		memmove( keybuf, &check, sizeof(struct _DenpData));

		if( kind == KIND_SYUKKIN ) {
			keybuf	=	_getDATA( ln, FD_CRED_PN );
			strcpy_s( keybuf->DP_CdNum, sizeof keybuf->DP_CdNum, data.m_cre );
			keybuf->DP_BRN = data.m_cbr;
			_setMAP( ln, FD_CRED_PN );
			_getCND( ln, FD_CRED_PN )->INP_sg		=	TRUE;
			_getCND( ln, FD_CRED_PN )->INP_type	=	brn_dfltinptype( data.m_cbr );
		}
		else if( kind == KIND_FURIKAE ) {
			//諸口枝番
			if( IsFuriSyogBrn() ) {
				if( isSYOGT_CODE( data.m_cre ) ) {
					if( data.m_cbr != -1 ) {
						keybuf	=	_getDATA( ln, FD_DSYOG_PN );
						keybuf->DP_cBRN = data.m_cbr;
						if( bDisp )	pDlg->SetSyogBrn( ln, FD_DSYOG_PN, keybuf->DP_cBRN );
						_setMAP( ln, FD_DSYOG_PN );
						_getCND( ln, FD_DSYOG_PN )->INP_sg		=	TRUE;
					}
				}
			}
		}
	}
	//貸方金額
	if( karikasi ) {
		memmove( check.DP_VAL, data.m_val, sizeof data.m_val );
		if( zeisgn == 0 ) {
			if( sotozei_data( &data )) {
				l_add( check.DP_VAL, check.DP_VAL, data.m_zei );
			}
		}
		if( bDisp )	pDlg->SetVal( ln, FD_CRVAL_PN, (char*)check.DP_VAL );

		_getCND( ln, FD_CRVAL_PN )->INP_sg	=	TRUE;
		_getCND( ln, FD_CRVAL_PN )->SET_sg	=	FALSE;
		keybuf	=	_getDATA( ln, FD_CRVAL_PN );
		memmove( keybuf->DP_VAL, check.DP_VAL, sizeof keybuf->DP_VAL );
	}

	SRCTrace;

	if( kind == KIND_FURIKAE && karikasi ) {
		// 貸方部門
		if( m_DI.IsBumon() && ! bTanBmn )	{
			check.DP_BMN = data.m_cbmn;
			keybuf	=	_getDATA( ln, FD_CBMN_PN );
			keybuf->DP_BMN = check.DP_BMN;

			if( check.DP_BMN == -1 )
				_getCND( ln, FD_CBMN_PN )->INP_sg	=	FALSE;
			else
				_getCND( ln, FD_CBMN_PN )->INP_sg	=	TRUE;

			struct _SET_KMKETC sk;
			sk.bmn = check.DP_BMN;
			sk.brn = -1;

			if( GetBmnEtc( &sk ) ) {
				strcpy_s( _getCND(ln, FD_CBMN_PN)->ETC_msg, sizeof _getCND(ln, FD_CBMN_PN)->ETC_msg, sk.etc );
				_getCND(ln, FD_CBMN_PN)->ETC_col = sk.etc_col;

				if( bDisp )	pDlg->SetBmon( ln, FD_CBMN_PN, check.DP_BMN, &sk );
			}
			else {
				::ZeroMemory( _getCND(ln, FD_CBMN_PN)->ETC_msg, sizeof _getCND(ln, FD_CBMN_PN)->ETC_msg );
				_getCND(ln, FD_CBMN_PN)->ETC_col = 0;

				if( bDisp )	pDlg->SetBmon( ln, FD_CBMN_PN, check.DP_BMN, NULL );
			}
		}
		// 貸方工事
		if( m_DI.IsKoji() && ! bTanBmn ) {
			kno = data.m_ckno;
			keybuf	=	_getDATA( ln, FD_CKOJI_PN );
			strcpy_s( (char*)keybuf->DP_KOJI, sizeof keybuf->DP_KOJI, kno );

			if( kno.IsEmpty() )
				_getCND( ln, FD_CKOJI_PN )->INP_sg	=	FALSE;
			else
				_getCND( ln, FD_CKOJI_PN )->INP_sg	=	TRUE;

			// 工事名称
			struct _SET_KMKETC sk;
			sk.bmn = -1;
			sk.kno = kno;
			sk.code.Empty();
			sk.brn = -1;

			if( GetKojiEtc( &sk ) ) {
				strcpy_s( _getCND(ln, FD_CKOJI_PN)->ETC_msg, sizeof _getCND(ln, FD_CKOJI_PN)->ETC_msg, sk.etc );
				_getCND(ln, FD_CKOJI_PN)->ETC_col = sk.etc_col;

				if( bDisp )	pDlg->SetKoji( ln, FD_CKOJI_PN, kno, &sk );
			}
			else {
				::ZeroMemory( _getCND(ln, FD_CKOJI_PN)->ETC_msg, sizeof _getCND(ln, FD_CKOJI_PN)->ETC_msg );
				_getCND(ln, FD_CKOJI_PN)->ETC_col = 0;

				if( bDisp )	pDlg->SetKoji( ln, FD_CKOJI_PN, kno, NULL );
			}
		}
	}
	SRCTrace;
	//貸方科目
	if( karikasi ) {
		::ZeroMemory( &check, sizeof check );
		strcpy_s( check.DP_CdNum, sizeof keybuf->DP_CdNum, data.m_cre );
		check.DP_BRN = data.m_cbr;
		knrec	=	NULL;
		knrec	=	DB_PjisToKnrec( data.m_cre, m_pZm, TRUE );
		if( knrec ) {
			KamokuString( knrec, check.DP_KNAM, sizeof check.DP_KNAM );
		//	memmove( check.DP_KNAM, (char*)knrec->knnam, sizeof knrec->knnam );
		}
		
		if( m_DI.IsBumon() ) {
			if( kind == KIND_FURIKAE ) {
				sk.bmn = data.m_cbmn;
			}
			else {
				sk.bmn = getDATA( DP_BMN_PN )->DP_BMN;
			}
		}
		else {
			sk.bmn = -1;
		}
		sk.code = check.DP_CdNum;
		sk.brn  = check.DP_BRN;

		if( m_DI._set_kmketc( &sk ) ) {
			strcpy_s( _getCND( ln, FD_CRED_PN )->ETC_msg, sizeof _getCND( ln, FD_CRED_PN )->ETC_msg, sk.etc );
			_getCND( ln, FD_CRED_PN )->ETC_col = sk.etc_col;

			strcpy_s( sm.sm_etc, sizeof sm.sm_etc, _getCND( ln, FD_CRED_PN )->ETC_msg );
			sm.sm_etccol = _getCND( ln, FD_CRED_PN )->ETC_col;
		}
		else {
			::ZeroMemory( sm.sm_etc, sizeof sm.sm_etc );
		}

		set_codestr( edabuf, sizeof edabuf, check.DP_BRN, m_pZm->zvol->edcol );

		if( edabuf[0] )
			sprintf_s( sm.sm_name, sizeof sm.sm_name, "%s%s", check.DP_KNAM, edabuf );
		else
			strcpy_s( sm.sm_name, sizeof sm.sm_name, check.DP_KNAM );

		if( bDisp )	pDlg->SetKnam( ln, FD_CRED_PN, &sm );

		_setMAP( ln, FD_CRED_PN );
		_getCND( ln, FD_CRED_PN )->INP_sg	=	TRUE;
		_getCND( ln, FD_CRED_PN )->INP_type	=	brn_dfltinptype( data.m_cbr );;
		keybuf	=	_getDATA( ln, FD_CRED_PN );
		memmove( keybuf, &check, sizeof(struct _DenpData));

		if( kind == KIND_NYUKIN ) {
			keybuf	=	_getDATA( ln, FD_DEBT_PN );
			strcpy_s( keybuf->DP_CdNum, sizeof keybuf->DP_CdNum, data.m_dbt );
			keybuf->DP_BRN = data.m_dbr;

			_setMAP( ln, FD_DEBT_PN );
			_getCND( ln, FD_DEBT_PN )->INP_sg	=	TRUE;
			_getCND( ln, FD_DEBT_PN )->INP_type	=	brn_dfltinptype( data.m_dbr );;
		}
		else if( kind == KIND_FURIKAE ) {
			//諸口枝番
			if( IsFuriSyogBrn() ) {
				if( isSYOGT_CODE( data.m_dbt ) ) {
					if( data.m_dbr != -1 ) {
						keybuf	=	_getDATA( ln, FD_CSYOG_PN );
						keybuf->DP_cBRN = data.m_dbr;
						if( bDisp )	pDlg->SetSyogBrn( ln, FD_CSYOG_PN, keybuf->DP_cBRN );
						_setMAP( ln, FD_CSYOG_PN );
						_getCND( ln, FD_CSYOG_PN )->INP_sg		=	TRUE;
					}
				}
			}
		}
	}
	SRCTrace;

	//摘要
	CString	text, text1, text2;
	BOOL	isw	=	FALSE;
	int		l, idx;
	unsigned char	tmp[128];

//	CWnd*	pTky	=	GetDlgItem( _getINP_CTL( ln, getTKY_PN() )->pIDC->IDC_X );
//	pTky->GetWindowText( text1 );

	// 貸方摘要チェック用 [09.03 /08]
	if( _getCND( ln, FD_TKY_PN )->INP_sg ) {
		keybuf	=	_getDATA( ln, FD_TKY_PN );
		text1 = keybuf->DP_TKY;
	}

	//Image
	idx = ln-1;
	if( data.m_dsign[0] & 0x80 ) {
		//NIC転送イメージ
		if( data.m_imgsq == -1 ) {	// イメージデータがない
			if( IsKind() == KIND_FURIKAE ) {
				if( !karikasi ) {
					pDlg->SetImage( karikasi, ln, NULL );
					isw	=	TRUE;
					tblImage[idx][0]	=	1;
					tblImageOfst[idx][0]	=	-1;
				}
				else {
					tblImageOfst[idx][1]	=	-1;
					if( text1.IsEmpty() && tblImageOfst[idx][0] == 0 ) {
						pDlg->SetImage( karikasi, ln, NULL );
						isw	=	TRUE;
						tblImage[idx][0]	=	1;
						tblImageOfst[idx][0]	=	-1;
					}
					else if( tblImageOfst[idx][0] != -1 ) {
						isw	=	TRUE;
						tblImage[idx][1]	=	1;
					}
				}
			}
			else {
				pDlg->SetImage( 0, ln, NULL );
				isw	=	TRUE;
				tblImage[idx][0]	=	1;
				tblImageOfst[idx][0]	=	-1;
			}
		}
		else {
			//イメージを表示
			IMREC	*imp;
			if ( !m_pZm->image->SetAbsolutePosition( data.m_imgsq ) )
				imp	= m_pZm->image->DecodeImage();
				
			if( imp == NULL ) {
				text2.Empty();
			}
			else {
				if( IsKind() == KIND_FURIKAE ) {
					if( !karikasi ) {
						if( bDisp )	pDlg->SetImage( 0, ln, imp );
						isw	=	TRUE;
						tblImage[idx][0]	=	1;
						tblImageOfst[idx][0]	=	data.m_imgsq;
					}
					else {
						tblImageOfst[idx][1]	=	data.m_imgsq;
						if( text1.IsEmpty() && tblImageOfst[idx][0] == 0 ) {
							if( bDisp )	pDlg->SetImage( 0, ln, imp );
							isw	=	TRUE;
							tblImage[idx][0]	=	1;
//							tblImage[idx][1]	=	1;
							tblImageOfst[idx][0]	=	data.m_imgsq;
						}
						else if( tblImageOfst[idx][0] != data.m_imgsq ) {
//							GetNowDnpDialog->SetImage( 1, ln, imp );
							isw	=	TRUE;
							tblImage[idx][1]	=	1;
							tblImageOfst[idx][1]	=	data.m_imgsq;
						}
					}
				}
				else {
					if( bDisp )	pDlg->SetImage( 0, ln, imp );
					isw	=	TRUE;
					tblImage[idx][0]	=	1;
					tblImageOfst[idx][0]	=	data.m_imgsq;
				}
			}
		}
	}
	else {
		//文字摘要
		l	=	m_pZm->zvol->tk_ln * 2;
		memset( tmp, 0, sizeof tmp );
		strcpy_s( (char*)tmp, sizeof tmp, data.m_tekiyo );
		int		i;

		i	=	kjlen( tmp, sizeof tmp );
		tmp[i]	=	NULL;

		text2	=	tmp;

	}

	if( isw == FALSE ) {
		if( !text1.GetLength() && !tblImage[idx][0] ) {
			text	=	text2;
#ifdef OLD_CLOSE
			if( IsTekiyoSize() != FALSE ) {
				if( (text.GetLength() / 2 ) > TKYover )
					pTky->SetFont( pTKYnew );
				else
					pTky->SetFont( pTKYorg );
			}
#endif

			l	=	m_pZm->zvol->tk_ln * 2;
			wsprintf( (char*)tmp, _T("%s"), (LPCTSTR)text );
			memmove( check.DP_TKY, tmp, l );

			if( bDisp )	pDlg->SetTekiyo( ln, FD_TKY_PN, (char*)check.DP_TKY );
			_getCND( ln, FD_TKY_PN )->INP_sg	=	TRUE;
			keybuf	=	_getDATA( ln, FD_TKY_PN );
			memset( keybuf->DP_TKY, 0, sizeof keybuf->DP_TKY );
			memmove( keybuf->DP_TKY, check.DP_TKY, l );
		}
		else if( tblImage[idx][0] || text1 != text2 ) {
			tblTekiyo[idx]	=	text2;
		}
	}
	SRCTrace;

	// ここからの項目は、複合仕訳で借方／貸方 異なる場合は、
	// 借方の項目を優先して、表示する。
	// 入力を伴う修正があった場合は、借方／貸方 両方に同じ値をセットする
	pos = karikasi ? 1 : 0;
	// 手形番号
	tblTgnum[idx][pos] = data.m_rnumber;

	MyTrace("m_rnumber len = %d, sizeof = %d\n", data.m_rnumber.GetLength(), sizeof _getDATA(ln, FD_TGNUM_PN)->DP_SNUM);


	if( ! karikasi ) {
		SRCTrace;

		strcpy_s( (char*)_getDATA( ln, FD_TGNUM_PN )->DP_SNUM, sizeof _getDATA( ln, FD_TGNUM_PN )->DP_SNUM, data.m_rnumber );
		if( _getDATA( ln, FD_TGNUM_PN )->DP_SNUM[0] )	_getCND( ln, FD_TGNUM_PN )->INP_sg = TRUE;

		SRCTrace;

		if( bDisp )	pDlg->SetTgNumber( ln, FD_TGNUM_PN, (char*)_getDATA( ln, FD_TGNUM_PN )->DP_SNUM );

		SRCTrace;
	}
	else {
		// 借方が 未入力
		if( tblTgnum[idx][0].IsEmpty() ) {
			SRCTrace;

			strcpy_s( (char*)_getDATA( ln, FD_TGNUM_PN )->DP_SNUM, sizeof _getDATA( ln, FD_TGNUM_PN )->DP_SNUM, data.m_rnumber );
			if( _getDATA( ln, FD_TGNUM_PN )->DP_SNUM[0] )	_getCND( ln, FD_TGNUM_PN )->INP_sg = TRUE;

			SRCTrace;
		}
		if( bDisp )	pDlg->SetTgNumber( ln, FD_TGNUM_PN, (char*)_getDATA( ln, FD_TGNUM_PN )->DP_SNUM );

		SRCTrace;
	}

	SRCTrace;

	// 手形期日
	tblTgdate[idx][pos] = data.m_tdate;
	int gen_ymd, gengo;
	BYTE ymd[10] = {0};
	CVolDateDB vd;

	if( ! karikasi ) {
		_getDATA( ln, FD_TGDATE_PN )->DP_TGDAY = data.m_tdate;
		::ZeroMemory( _getDATA( ln, FD_TGDATE_PN )->DP_TGBCD, sizeof _getDATA( ln, FD_TGDATE_PN )->DP_TGBCD );

		if( data.m_tdate > 0 ) {
			vd.db_datecnvGen( 0, data.m_tdate, &gengo, &gen_ymd, 0, 0 );
			sprintf_s( (char*)tmp, sizeof tmp, "%06d", gen_ymd );
			l_pack( ymd, tmp, 6 );
			memcpy( _getDATA( ln, FD_TGDATE_PN )->DP_TGBCD, ymd, 3 );
			_getCND( ln, FD_TGDATE_PN )->INP_sg = TRUE;
			if( bDisp )	pDlg->SetTgDate( ln, FD_TGDATE_PN, _getDATA( ln, FD_TGDATE_PN )->DP_TGBCD );
		}
		else {
			if( bDisp )	pDlg->SetTgDate( ln, FD_TGDATE_PN, NULL );
		}
	}
	else {
		if( tblTgdate[idx][0] <= 0 ) {
			_getDATA( ln, FD_TGDATE_PN )->DP_TGDAY = data.m_tdate;
			::ZeroMemory( _getDATA( ln, FD_TGDATE_PN )->DP_TGBCD, sizeof _getDATA( ln, FD_TGDATE_PN )->DP_TGBCD );

			if( data.m_tdate > 0 ) {
				vd.db_datecnvGen( 0, data.m_tdate, &gengo, &gen_ymd, 0, 0 );
				sprintf_s( (char*)tmp, sizeof tmp, "%06d", gen_ymd );
				l_pack( ymd, tmp, 6 );
				memcpy( _getDATA( ln, FD_TGDATE_PN )->DP_TGBCD, ymd, 3 );
				_getCND( ln, FD_TGDATE_PN )->INP_sg = TRUE;

				if( bDisp )	pDlg->SetTgDate( ln, FD_TGDATE_PN, _getDATA( ln, FD_TGDATE_PN )->DP_TGBCD );
			}
			else {
				if( bDisp )	pDlg->SetTgDate( ln, FD_TGDATE_PN, NULL );
			}
		}
	}

	// 変動事由
	tblHjiyu[idx][pos] = data.m_hjiyu;
	text.Empty();

	if( ! karikasi ) {
		_getDATA( ln, FD_HJIYU_PN )->DP_BMN = data.m_hjiyu;
		_getDATA( ln, FD_HJIYU_PN )->NAIBCD = data.m_nbcd;
/*- '09.03.11 -*/
//		if( data.m_hjiyu > 0 )
//			m_pZm->get_hjiyustr( data.m_hjiyu, text );
/*-------------*/
		if( data.m_hjiyu > 0 ){
			_get_hjiyustr( data.m_hjiyu, data.m_nbcd, text );
			_getCND( ln, FD_HJIYU_PN )->INP_sg = TRUE;
		}
/*-------------*/

		if( bDisp )	pDlg->SetHjiyu( ln, FD_HJIYU_PN, (data.m_hjiyu > 0) ? &text : NULL );
	}
	else {
		if( tblHjiyu[idx][0] <= 0 ) {
			_getDATA( ln, FD_HJIYU_PN )->DP_BMN = data.m_hjiyu;
			_getDATA( ln, FD_HJIYU_PN )->NAIBCD = data.m_nbcd;
/*- '09.03.11 -*/
//			if( data.m_hjiyu > 0 )
//				m_pZm->get_hjiyustr( data.m_hjiyu, text );
/*-------------*/
			if( data.m_hjiyu > 0 ){
				_get_hjiyustr( data.m_hjiyu, data.m_nbcd, text );
				_getCND( ln, FD_HJIYU_PN )->INP_sg = TRUE;
			}
/*-------------*/

			if( bDisp )	pDlg->SetHjiyu( ln, FD_HJIYU_PN, (data.m_hjiyu > 0) ? &text : NULL );
		}
	}

	// 付箋
	tblHusen[idx][pos] = (data.m_dsign[7] & 0x0f);
	tblHcmnt[idx][pos] = data.m_tag;

	if( ! karikasi ) {
		_getDATA( ln, FD_HUSEN_PN )->DP_HUSEN = (data.m_dsign[7] & 0x0f);
		strcpy_s( (char*)_getDATA( ln, FD_HUSEN_PN )->DP_HCMNT, sizeof _getDATA( ln, FD_HUSEN_PN )->DP_HCMNT, data.m_tag );

		if( bDisp )	pDlg->SetHusen( ln, FD_HUSEN_PN, _getDATA( ln, FD_HUSEN_PN )->DP_HUSEN, (char*)_getDATA(ln, FD_HUSEN_PN)->DP_HCMNT);
	}
	else {
		if( tblHusen[idx][0] <= 0 ) {
			_getDATA( ln, FD_HUSEN_PN )->DP_HUSEN = (data.m_dsign[7] & 0x0f);
			strcpy_s( (char*)_getDATA( ln, FD_HUSEN_PN )->DP_HCMNT, sizeof _getDATA( ln, FD_HUSEN_PN )->DP_HCMNT, data.m_tag );
		}	

		if( bDisp )	pDlg->SetHusen( ln, FD_HUSEN_PN, _getDATA( ln, FD_HUSEN_PN )->DP_HUSEN, (char*)_getDATA(ln, FD_HUSEN_PN)->DP_HCMNT);
	}

	///////////////////////////////////////
	//消費税
	int pn = -1;
	struct _SY_MSG_PACK2 smpk;

	if( zeisgn == 0 && karikasi == 0 )
		pn = FD_DSKBN_PN;
	else if( zeisgn && karikasi )
		pn = FD_CSKBN_PN;

	if( pn != -1 ) {
		if( m_DPscan.IsAddData( ln, karikasi ) ) {

			keybuf	=	_getDATA( ln, pn );
			_getCND( ln, pn )->INP_sg	=	FALSE;

	//		m_CREC[0]	=	data;
			int cnt = mkSY_CREC( m_CREC, ln );
			if( cnt == 2 )	{
				idx = karikasi ? 1 : 0;
			}
			else {
				idx = 0;
			}

			//初期化
			//消費税区分、業種、仕入区分、対価を戻す
			memmove( &m_CREC[idx].m_dsign[0], &data.m_dsign[0], 16 );
			memmove( &m_CREC[idx].m_taika, data.m_taika, 6 );

			m_DI.DB_SyIniz( &m_CREC[idx] );

			//消費税区分を選択番号にセット
			SHEET_SYKBN sh_skbn = m_DPscan.GetSheetSykbn( ln, karikasi );

			keybuf->SYDATA.swk_sw	=	sh_skbn.swk_sw;
			keybuf->SYDATA.ritu_sw	=	sh_skbn.ritu_sw;
			keybuf->SYDATA.zeikbn_sw	=	sh_skbn.zeikbn_sw;
			keybuf->SYDATA.uri_sw	=	sh_skbn.uri_sw;
			keybuf->SYDATA.toku_sw	=	sh_skbn.toku_sw;	//特定収入
			keybuf->SYDATA.kts_sw	=	sh_skbn.kts_sw;

			if( m_pSy->SyMsg( &smpk ) == 0) {

				if( keybuf->SYDATA.swk_sw > 0 )		m_pSy->SyFnc( keybuf->SYDATA.swk_sw );
				if( keybuf->SYDATA.ritu_sw > 0 )	m_pSy->SyFnc( keybuf->SYDATA.ritu_sw );
				if( keybuf->SYDATA.zeikbn_sw > 0 )	m_pSy->SyFnc( keybuf->SYDATA.zeikbn_sw );
				if( keybuf->SYDATA.uri_sw > 0 )		m_pSy->SyFnc( keybuf->SYDATA.uri_sw );
				if( keybuf->SYDATA.toku_sw > 0 )	m_pSy->SyFnc( keybuf->SYDATA.toku_sw );		//特定収入
				if( keybuf->SYDATA.kts_sw > 0 )		m_pSy->SyFnc( keybuf->SYDATA.kts_sw );
			}

			strcpy_s( keybuf->SYDATA.SKBNtxt, sizeof keybuf->SYDATA.SKBNtxt,
												m_pSy->Sy_get_message( m_pZm->dbdata, SYGET_TYPE ) );
			setSY_CREC( &m_CREC[idx], karikasi, ln );
		}
		else if( _getCND( ln, pn )->INP_sg == FALSE ) {

			if( IsKind() == KIND_FURIKAE ) {
				// 1仕訳で両科目とも消費税科目の場合は貸方にのみセット
				if( m_DI.km_syattr( data.m_dbt ) && m_DI.km_syattr( data.m_cre ) ) {
					if( ! karikasi )	return;
				}
			}

			keybuf	=	_getDATA( ln, pn );
			_getCND( ln, pn )->INP_sg	=	FALSE;

			m_CREC[0]	=	data;
			m_DI.DB_SyIniz( &m_CREC[0] );
			// 表示テキスト
			// 消費税区分
#ifdef SYZDSP_CLOSE
			m_pSy->SyMsg( &smpk );
#endif
			CString txt;
			txt = m_pSy->Sy_get_message( m_pZm->dbdata, SYGET_TYPE );
			//消費税修正サイン
			int sg = 0;
		//	sg = SYZMDFY_BIT.bitncnd( data.m_seq - 1);
			sg = GetSyzMdfySign( data.m_seq );
			keybuf->SYDATA.sy_chg = (sg > 0) ? 1 : 0;

			// 表示
			if( bDisp )	{
				SRCTrace;
				PutsSyz( ln, pn, (char*)(LPCTSTR)txt, keybuf->SYDATA.sy_chg );
			}

			strcpy_s( keybuf->SYDATA.SKBNtxt, sizeof keybuf->SYDATA.SKBNtxt, txt );

#ifndef SYZDSP_CLOSE	// 移動
			m_pSy->SyMsg( &smpk );
#endif
			int swk, ritu, zei, uri, tai, toku;
			swk = ritu = zei = uri = tai = toku = 0;

			swk		= smpk.SY_MSG_SWKBN->sel_no;
			ritu	= smpk.SY_MSG_RITU->sel_no;
			zei		= smpk.SY_MSG_ZEI->sel_no;
			uri		= smpk.SY_MSG_URISIRE->sel_no;
			toku	= smpk.SY_MSG_TOKTEI->sel_no;
			tai		= smpk.SY_MSG_TAIKAKBN->sel_no;

			if( swk )	keybuf->SYDATA.swk_sw = smpk.SY_MSG_SWKBN->msg_tbl[swk-1]->code;
			else		keybuf->SYDATA.swk_sw = 0;
			
			if( ritu )	keybuf->SYDATA.ritu_sw = smpk.SY_MSG_RITU->msg_tbl[ritu-1]->code;
			else		keybuf->SYDATA.ritu_sw = 0;

			if( zei )	keybuf->SYDATA.zeikbn_sw = smpk.SY_MSG_ZEI->msg_tbl[zei-1]->code;
			else		keybuf->SYDATA.zeikbn_sw = 0;

			if( uri )	keybuf->SYDATA.uri_sw = smpk.SY_MSG_URISIRE->msg_tbl[uri-1]->code;
			else		keybuf->SYDATA.uri_sw = 0;
			//特定収入
			if( toku )	keybuf->SYDATA.toku_sw = smpk.SY_MSG_TOKTEI->msg_tbl[toku-1]->code;
			else		keybuf->SYDATA.toku_sw = 0;

			if( tai )	keybuf->SYDATA.kts_sw = smpk.SY_MSG_TAIKAKBN->msg_tbl[tai-1]->code;
			else		keybuf->SYDATA.kts_sw = 0;

			// 消費税区分
			memmove( &keybuf->SYDATA._DSGN, &m_pZm->dbdata->dsign[0], SZ_DSGN );
			// 対価
			l_input( keybuf->SYDATA.TKsyohi, (void*)(LPCTSTR)m_pZm->dbdata->taika );
			// インボイス番号
			sprintf_s(keybuf->SYDATA.TKinvno, sizeof(keybuf->SYDATA.TKinvno), "%s", m_pZm->dbdata->invno);

			MyTrace("SetScanData keybuf->SYDATA.TKinvno = %s\n", keybuf->SYDATA.TKinvno);

			BOOL bSkbn;
			bSkbn = (swk || ritu || zei || uri || tai );
			_getCND( ln, pn )->INP_sg	=	bSkbn;
		}
	}
	int sgn, sotomen;
	//有価証券の非課税譲渡仕訳のために 最後で表示
	//借方税額
	if( !karikasi && zeisgn == 0 ) {
		memmove( check.DP_VAL, data.m_zei, sizeof data.m_zei );
		sgn = sotomen = 0;
		if( m_pSy->IsTaikaData( m_pZm->dbdata )==2 )	sgn = 1;
		if (!m_DI.is_syohizeisiwake( m_pZm->dbdata ))	sgn = 2;
		else {
			COLORREF back;
			if (m_DI.isnot_defzeigaku(m_pZm->dbdata, back)) {
				sgn = 3;
				if (back != RGB_ZEI_MDFY) {
					sgn++;
				}
			}
			// 輸出仕訳
			if( m_DI.is_yusyutsusiwake(m_pZm->dbdata) )	  sgn = 5;

			if( m_DI.is_sotomensiwake(m_pZm->dbdata) )
				sotomen = 1;
		}

		if( bDisp )	pDlg->SetVal( ln, FD_DBZEI_PN, (char*)check.DP_VAL, sgn );

		_getCND( ln, FD_DBZEI_PN )->INP_sg	=	TRUE;
		_getCND( ln, FD_DBZEI_PN )->SET_sg	=	FALSE;
		_getCND( ln, FD_DBZEI_PN )->INP_type = sgn;
		set_sotomen_sign(ln, FD_DBZEI_PN, sotomen);

		keybuf	=	_getDATA( ln, FD_DBZEI_PN );
		memmove( keybuf->DP_VAL, check.DP_VAL, sizeof keybuf->DP_VAL );
	}

	SRCTrace;

	//貸方税額
	if( karikasi && zeisgn != 0 ) {
		memmove( check.DP_VAL, data.m_zei, sizeof data.m_zei );
		sgn = sotomen = 0;
		if( m_pSy->IsTaikaData( m_pZm->dbdata )==2 )	sgn = 1;
		if (!m_DI.is_syohizeisiwake( m_pZm->dbdata ))	sgn = 2;
		else {
			COLORREF back;
			if (m_DI.isnot_defzeigaku(m_pZm->dbdata, back)) {
				sgn = 3;
				if (back != RGB_ZEI_MDFY) {
					sgn++;
				}
			}
			// 輸出仕訳
			if( m_DI.is_yusyutsusiwake(m_pZm->dbdata) )	  sgn = 5;

			if( m_DI.is_sotomensiwake(m_pZm->dbdata) )
				sotomen = 1;
		}

		if( bDisp )	pDlg->SetVal( ln, FD_CRZEI_PN, (char*)check.DP_VAL, sgn );

		_getCND( ln, FD_CRZEI_PN )->INP_sg	=	TRUE;
		_getCND( ln, FD_CRZEI_PN )->SET_sg	=	FALSE;
		_getCND( ln, FD_CRZEI_PN )->INP_type = sgn;
		set_sotomen_sign(ln, FD_CRZEI_PN, sotomen);

		keybuf	=	_getDATA( ln, FD_CRZEI_PN );
		memmove( keybuf->DP_VAL, check.DP_VAL, sizeof keybuf->DP_VAL );
	}

	SRCTrace;

}


//
//	転送済み仕訳の情報をセットする
//	int ln ... データ上の　行番号(1～100)
//
void CDnpBaseClass::TransLineSign( CDBINPDataRec* pdt, int ln )
{
	int			sgn, st;

	
	if( m_DI.IsJzSansyo() ) {
		sgn = 10;
	}
	else {
		if( (st = m_DI.tline_chk(pdt)) < 0 ) {

			if( m_DI.Confirm_Mode() == 0 ) {			//確定マスター修正不可
				if( st == -1 ) {	// 確定仕訳
					sgn = 2;
				}
				else {				// 転送仕訳
					sgn = 3;
				}
			}
			else {
				// 確定マスターでなく、転送済み仕訳の修正可
				sgn = 1;
			}

		}
		else {
			sgn = 0;
		}
	}

	int pn;

	for( pn = FD_DBVAL_PN; pn < FD_MAX_PN; pn++ ) {
		_getCND( ln, pn )->EDT_sg = sgn;
	}
}


//	
//	int ln ... データ上の　行番号(1～100)
void CDnpBaseClass::TransLine( int dt_ln, int ln, char* sy_chg )
{
	if( ln <= getSCROLL_NL() ) {
		GetNowDnpDialog()->TransLine( dt_ln, ln, sy_chg );
		GetNowDnpDialog()->FuriSyogDataColor( dt_ln, ln );
	}
}

//	int ln ... 画面伝票上の　行番号(１～最大７)
BOOL CDnpBaseClass::IsTransLine( int ln, char& edt_sg )
{
	int	pn;
	if( IsKind() == KIND_NYUKIN )		pn	=	FD_CRED_PN;
	else if( IsKind() == KIND_SYUKKIN )	pn	=	FD_DEBT_PN;
	else								pn	=	FD_DBVAL_PN;

	int dt_ln = get_dataline( ln );
	// サインをセット
	edt_sg = _getCND( dt_ln, pn )->EDT_sg;

	if( _getCND( dt_ln, pn )->EDT_sg >= 1 )	return	TRUE;
	return	FALSE;
}


void CDnpBaseClass::DispBaseKmk()
{
	if( IsKind() != KIND_FURIKAE ) {
		CDnpBaseDlg* pDlg = GetNowDnpDialog();
		CString str;

		if( (m_pZm->zvol->ind_table&0x01) ) {
			LPCTSTR cd = GNKINCode(m_pZm->zvol->apno);
			DBKNREC* pKn;
			pKn = DB_PjisToKnrec( cd, m_pZm );

			if( pKn ) {
				char nam[64];
				KamokuString( pKn, nam, sizeof nam );

				if( IsKind() == KIND_NYUKIN )
					str.Format( "入金科目『%.14s』", nam );
				else	
					str.Format( "出金科目『%.14s』", nam );
			}

			((CNSDenpDlg*)pDlg)->DispBaseKmk( TRUE, str );
		}
		else {
			((CNSDenpDlg*)pDlg)->DispBaseKmk( FALSE, str );
		}
	}
}


//-----------------------------------------------
//	伝票の切替
//
//-----------------------------------------------
void CDnpBaseClass::ChangeKind( int nkind )
{
//	setDATA_LINE_COLOR( get_nowln(), RGB_WHITE, FALSE );

	// 摘要等にあると、フォーカスが消えるため、日付にいったん移動する。
	if( get_nowpn() == FD_TKY_PN || get_nowpn() == FD_TGNUM_PN || get_nowpn() == FD_HJIYU_PN ||
		get_nowpn() == FD_DBMN_PN || get_nowpn() == FD_CBMN_PN || get_nowpn() == DP_BMN_PN ||
		get_nowpn() == FD_DKOJI_PN || get_nowpn() == FD_CKOJI_PN || get_nowpn() == DP_KOJI_PN ) {
		GetNowDnpDialog()->SendMessage( WM_RETROFOCUS, MAKELONG((WORD)-1,0), DP_MM_PN );
	}

	// DBEditを戻す
	dbedt_putback();

	Destroy_SyzInp();
	Destroy_HjiyuInp();
	Destroy_HusenInp();

	// 最後の伝票チェック
	if( IsJob() == JOB_INPUT ) {
		int	st;
		if( is_lastdenp_contents() != FALSE ) {
m_PRO_OK = FALSE;

			st = myICSMessageBox( "最後の伝票の「仕訳登録」が行われていません。\n「仕訳登録」を行いますか？\n\n「いいえ」の場合、この伝票は破棄されます。", MB_YESNO|MB_DEFBUTTON2 );

m_PRO_OK = TRUE;

			if( st == IDYES ) {
				if( DaWrite() == FALSE )	return;
			}
//			setDATA_LINE_COLOR( get_nowln(), RGB_WHITE, FALSE );

			tkyexpan( get_nowln(), NULL, FALSE );
			Destroy_SyzInp();
		}
	}

	IsKind()	=	nkind;

	/* 諸口合計	*/
	char bin1[6], bin2[6];
	m_DI.CalqSyogTotal( SYOGT, (char *)bin1, (char *)bin2, (char *)SYOG_TAI );
	syog_dsp( (char *)SYOG_TAI );

#ifdef LATER_CLOSE
	/* 最終データと最終日付を CREC , SREC にセット
		日付はデータが無い場合でも日付はボリュームラベルから必ずセットされる */
	ldata_dsp(); //初期日付は ldata_dsp()で SREC にセットされている。
#endif

	// 入力コントロール初期化
	inpctl_inz();

	// 貸借データイニシャライズ
	total_inz();

	// 伝票番号、日付などの初期データを設定
	BYTE pack[16] = {0};
	m_DI.GetInpDate(pack);

	getDATA( DP_MM_PN )->DP_BCDDAY[0] = pack[0];
	getCND( DP_MM_PN )->INP_sg = TRUE;
	getDATA( DP_DD_PN )->DP_BCDDAY[0] = pack[1];
	getCND( DP_DD_PN )->INP_sg = TRUE;

	getDATA( DP_DENP_PN )->DP_DENP = -1;
	getDATA( DP_BMN_PN )->DP_BMN = -1;
	::ZeroMemory( getDATA( DP_KOJI_PN )->DP_KOJI, sizeof getDATA( DP_KOJI_PN )->DP_KOJI );
	::ZeroMemory(getDATA(DP_SYONUM_PN)->DP_SNUM, sizeof getDATA(DP_KOJI_PN)->DP_SNUM);
	// 伝票表示
	DenpDlgDisp( nkind );
	// 伝票イニシャライズ
	sheet_clr();
	// 自動仕訳 状態
	GetNowDnpDialog()->AutoSiwake( IsMode() );

	// 基本科目表示
	DispBaseKmk();

	//入力位置（伝票番号）にフォーカスセット
	set_focus( DP_DENP_PN );
	set_nowln( 1 );
	set_nowpn( DP_DENP_PN );
}



// 最終伝票のチェック
BOOL CDnpBaseClass::is_lastdenp_contents()
{
	int ln;

	//３種伝票ともにDATA_LINEは共通
	for( ln = 1 ; ln <= get_MaxLine() ; ++ln )
	{
		if( is_line_contents( ln, FD_CRED_PN ) )
			return TRUE;
	}

	return FALSE;
}

// 行データチェック
//	int ln ... データ上の　行番号(1～100)
BOOL CDnpBaseClass::is_line_contents( int ln, int max_pn )
{
	int pn;

	BOOL bKmk[2] = {FALSE,FALSE}, bVal[2] = {FALSE,FALSE};

	// 科目と金額がないデータは、なしとする
	for( pn = FD_DBVAL_PN ; pn <= max_pn ; ++pn )
	{
		if( IsKind() == KIND_NYUKIN ) {
			if( pn == FD_DEBT_PN || pn == FD_DBVAL_PN )	continue;
		}
		else if( IsKind() == KIND_SYUKKIN ) {
			if( pn == FD_CRED_PN || pn == FD_CRVAL_PN )	continue;
		}

		if( _getCND( ln, pn )->INP_sg == TRUE ) {
			switch( pn ) {
				case FD_DEBT_PN:	bKmk[0] = TRUE;		break;
				case FD_CRED_PN:	bKmk[1] = TRUE;		break;
				case FD_DBVAL_PN:	bVal[0] = TRUE;		break;
				case FD_CRVAL_PN:	bVal[1] = TRUE;		break;
			}
		}
	}

	if( (bKmk[0] && bVal[0]) || (bKmk[1] && bVal[1]) )
			return TRUE;
	else	return FALSE;
}


//----------------------------
// データの書き込み
//----------------------------
BOOL CDnpBaseClass::DaWrite()
{
int st, max, pn, cno, i;
int brn_ln, brn_pn;
int mode = SHEET_ALL;

	// フォーカスをビューにセットし、最終データを取得
	m_pDnpParent->SetFocus();

	// 日付チェック＆アクション
	if( DateCheck_and_action() == -1 )
		return FALSE;

#ifdef OLD_CLOSE
	//最終カーソル位置のデータをセット
	last_data_set();
#endif

	// DBEditを戻す
	dbedt_putback();

	pn = get_nowpn();

	//伝票番号チェック
	cno = getDATA( DP_DENP_PN )->DP_DENP;

	if( cno != -1 && IsUsedDenpNo( cno ) )
	{
m_PRO_OK = FALSE;
		CString msg;
		msg.Format( "伝票番号(%7d) はすでに使用されています！\r\n修正しますか？", cno );
		st = myICSMessageBox( msg, MB_YESNO );
m_PRO_OK = TRUE;

		if( st == IDYES ) {
			set_focus( DP_DENP_PN );
			return FALSE;
		}
	}

	//枝番未入力チェック
	if( CheckEdabanInput( brn_ln, brn_pn ) )
	{
		Buzzer();

m_PRO_OK = FALSE;
		if( m_pAsel->BRNKAK_OPT != OPT_BRNKAK ) {
			st = myICSMessageBox( "枝番の無い科目があります。\n伝票を修正しますか？\n\n「いいえ」の場合、このまま登録を実行します。", MB_YESNO );
		}
		else {
			st = myICSMessageBox( "枝番の無い科目があります。\n伝票を修正してください。", MB_OK );
			st = IDYES;
		}
m_PRO_OK = TRUE;

		switch( st )
		{
			case IDNO:	break;
			default:
			//	if( (pn == FD_DSKBN_PN || pn == FD_CSKBN_PN) &&  m_pSyzInp->IsDisplay() )
			//		return FALSE;

			//	set_focus( get_nowpn() );
			//	_set_focus( brn_ln, brn_pn );
				NoinpScrolChk( brn_ln, brn_pn );
				return FALSE;
		}
	}

	//工事番号チェック
	if( CheckKOJI( brn_ln, brn_pn ) )
	{
	//	_set_focus( brn_ln, brn_pn );
		NoinpScrolChk( brn_ln, brn_pn );
		return FALSE;
	}

	//工事番号 科目チェック
	if( CheckKojiKmk( brn_ln, brn_pn ) )
	{
		_set_focus( brn_ln, brn_pn );
		return FALSE;
	}

	//部門未入力チェック
	if( CheckBmnNoInp( brn_ln, brn_pn ) )
	{
	//	_set_focus( brn_ln, brn_pn );
		NoinpScrolChk( brn_ln, brn_pn );
		return FALSE;
	}

	//部門範囲チェック
	if( CheckBmnHani( brn_ln, brn_pn ) )
	{
		NoinpScrolChk( brn_ln, brn_pn );
	//	_set_focus( brn_ln, brn_pn );
		return FALSE;
	}

	//部門が登録されているかチェック
	if( CheckBmnTorok( brn_ln, brn_pn ) )
	{
		NoinpScrolChk( brn_ln, brn_pn );
		return FALSE;
	}

	//工事が登録されているかチェック
	if( CheckKojiTorok( brn_ln, brn_pn ) )
	{
		NoinpScrolChk( brn_ln, brn_pn );
		return FALSE;
	}

	// 貸借チェック
	total_clq();
	if( total_chk() )
	{
		Buzzer();
m_PRO_OK = FALSE;
		st = myICSMessageBox( "貸借金額が一致していません。\nこのまま「仕訳登録」を実行すると次の伝票に貸借金額が持ち越されます。\n伝票を修正しますか？\n\n「いいえ」の場合、「仕訳登録」を実行します。", MB_YESNO );
m_PRO_OK = TRUE;

		switch( st )
		{
		case IDNO: mode = SHEET_MOTIKOSI; break;
		default:
			if( (pn == FD_DSKBN_PN || pn == FD_CSKBN_PN) &&  m_pSyzInp->IsDisplay() )
				return FALSE;

			set_focus( get_nowpn() );
			return FALSE;
		}
	}

	// 諸口＆消費税仕訳に分解
	for( i = 0; i < EXPAN_MAX; i++ )	// テーブルイニシャライズ
		EXPAN_TBL[i].Reset();

	if( !(max = daexpan( EXPAN_TBL )) )
	{
		Buzzer();
m_PRO_OK = FALSE;
		st = myICSMessageBox( "書き込むデータが有りません。\n伝票を修正しますか？\n\n「いいえ」の場合、この伝票は破棄されます。", MB_YESNO );
m_PRO_OK = TRUE;

		switch( st )
		{
		case IDNO: sheet_clr( SHEET_TAISYAKU ); return TRUE;
		default:
			LineChgJob( m_CREC, get_nowln(), pn );	// ln = 今回のライン

			if( (pn == FD_DSKBN_PN || pn == FD_CSKBN_PN) &&  m_pSyzInp->IsDisplay() )
				return FALSE;

			set_focus( get_nowpn() );
			return FALSE;
		}
	}

	// 振替伝票で、7行 マックスで入力されており、貸借が異なる場合、
	// 伝票を持ち越すこととし、内部伝票番号の更新をしない。
	if( IsKind() == KIND_FURIKAE ) {
		if( mode == SHEET_MOTIKOSI ) {
			//振替伝票
			CDBINPDataRec* prec;
			char	kari[6] = {0};
			char	kasi[6] = {0};
			int		karicnt, kasicnt;
			karicnt = kasicnt = 0;

			for( i = 0; i < max; i++ ) {
				prec = &EXPAN_TBL[i];

				if( isSYOGT_CODE( prec->m_dbt ) || 
					isSYOGT_CODE( prec->m_cre ) ) {

					if( ! isSYOGT_CODE( prec->m_dbt ) ) {
						l_add( kari, kari, prec->m_val );
						// 外税は税額＋
						if( sotozei_data( prec )) {
							l_add( kari, kari, prec->m_zei );
						}

						karicnt++;
					}
					if( ! isSYOGT_CODE( prec->m_cre ) ) {
						l_add( kasi, kasi, prec->m_val );
						// 外税は税額＋
						if( sotozei_data( prec )) {
							l_add( kasi, kasi, prec->m_zei );
						}

						kasicnt++;
					}
					//貸借金額が一致した所で貸借位置を区切る
					if( !l_jsgn( kari, kasi, sizeof kari ) ) {
						karicnt	=	__max( karicnt, kasicnt );
						kasicnt	=	__max( karicnt, kasicnt );

						::ZeroMemory( kari, sizeof kari );
						::ZeroMemory( kasi, sizeof kasi );
					}
				}
				else {
					// 単一仕訳
					karicnt	=	__max( karicnt, kasicnt );
					kasicnt	=	__max( karicnt, kasicnt );

					karicnt++;
					kasicnt++;
				}
			}
			// 7行のときは、持ち越し
#ifdef OLD_CLOSE
			if( karicnt >= DP_SCROLL_NL || kasicnt >= DP_SCROLL_NL ) {
				if( m_InsideCno < 0 )
					m_InsideCno = -2;
			}
			else {
				m_InsideCno = -1;
			}
#endif
			// 7行でなくても、持ち越しするように変更	[02.26/ 09]
			if( m_InsideCno < 0 )
				m_InsideCno = -2;

		}
	}

	// データ書き込み
	if( AppendData( EXPAN_TBL, max, NULL ) == -1 )
		return FALSE;

	// 持ち越し 内部伝票番号の初期化 [02.25 /09]
	if( IsKind() == KIND_FURIKAE ) {
		if( mode != SHEET_MOTIKOSI ) {
			m_InsideCno = -1;
		}
	}

	// 伝票番号＋１
	if( ! (IsKind() == KIND_FURIKAE && mode == SHEET_MOTIKOSI) ) {	//持ち越しのときは＋１しない [02.26 /09]
		if( _getCND( 0, DP_DENP_PN )->INP_sg )
		{
			cno = _getDATA( 0, DP_DENP_PN )->DP_DENP;

			if( cno + 1 > (DWORD)0xffffffff - 1 )
				cno = 0;
			else
				cno++;

			_getDATA( 0, DP_DENP_PN )->DP_DENP = cno;
		}
	}

	// 諸口持越し
	syog_next();

//MyTrace( "@DaWrite \n" );

	// 伝票クリア＆表示
	sheet_clr( mode );

	// 02.08 /11 ----> Ｆ７　摘要登録をすると、伝票欄に 0000 が入っていた。
	m_pDnpParent->SetFocus();
	set_focus( DP_DENP_PN );

	return TRUE;
}


//------------------------------------------------------
// 伝票番号の使用チェック
//
//------------------------------------------------------
BOOL CDnpBaseClass::IsUsedDenpNo( int inpcno )
{
	CString filter;
	CString tmp, msg, strError;
	int sgn = 0;

	CStRecordSet st_rec( m_pZm->m_database );
	st_rec.m_sql.Format( "select count(*) as st from datarec where cno = %d", inpcno );

	TRY {
		if( ! st_rec.Open() ) {
			ICSMessageBox( _T( "伝票番号の使用チェックが行えません！" ) );
			return -1;
		}

		if( st_rec.m_st > 0 ) {
			sgn++;
		}
		st_rec.Close();
	}
	CATCH( CMemoryException, me ) {
		me->GetErrorMessage( strError.GetBuffer(_MAX_PATH), _MAX_PATH );
		strError.ReleaseBuffer();
		ICSMessageBox( strError );
		return FALSE;	
	}
	CATCH( CDBException, de ) {
		strError = de->m_strError;
		ICSMessageBox( strError );
		return FALSE;
	}
	END_CATCH

	BOOL bRet = FALSE;

	if( sgn ) {
		bRet = TRUE;
	}

	return bRet;
}


//---------------------------------------------------------
//	枝番未入力科目のチェック
//	int chk_ln	未入力科目があった場合、データ上の行番号(1～100)
//	int chk_pn	未入力科目があった場合のポジション
//	return -1 = 未入力科目あり
//			0 = なし
//---------------------------------------------------------
int CDnpBaseClass::CheckEdabanInput( int& chk_ln, int& chk_pn )
{
	//チェックしない
//	if( m_pAsel->BRN_OPT != OPT_RGSCHK )	return 0;
	if( m_pAsel->BRN_OPT != OPT_RGSCHK /*&& m_pAsel->BRNKAK_OPT != OPT_BRNKAK */)	return 0;

	struct _DenpData	*debt, *cred;
	int			ln;
	int			brn;
	CString		cod, filter;

	ezdb_init( m_pZm );

	for( ln = 1; ln <= get_MaxLine(); ln++ ) {
		debt	=	NULL;
		cred	=	NULL;

		if( IsKind() != KIND_NYUKIN ) {
			if( _getCND( ln, FD_DEBT_PN )->INP_sg ) {
				debt	=	_getDATA( ln, FD_DEBT_PN );
			}
		}
		if( IsKind() != KIND_SYUKKIN ) {
			if( _getCND( ln, FD_CRED_PN )->INP_sg ) {
				cred	=	_getDATA( ln, FD_CRED_PN );
			}
		}

		if( debt ) {
			brn	=	debt->DP_BRN;
			cod	=	debt->DP_CdNum;
			if( ! isSYOGT_CODE( cod ) && brn == -1 ) {
/*				filter.Format( "ezkcd = '%s'", cod );
				if( m_pZm->ezrec->Requery( filter, 1 ) == 0 ) {	*/
				if( m_pZm->ezrec->Find( (LPCTSTR)cod ) > 0 ) {
					chk_ln = ln;
					chk_pn = FD_DEBT_PN;
					return -1;
				}
			}
		}
		if( cred ) {
			brn	=	cred->DP_BRN;
			cod	=	cred->DP_CdNum;
			if( ! isSYOGT_CODE( cod ) && brn == -1 ) {
/*				filter.Format( "ezkcd = '%s'", cod );
				if( m_pZm->ezrec->Requery( filter, 1 ) == 0 ) {	*/
				if( m_pZm->ezrec->Find( (LPCTSTR)cod ) > 0 ) {
					chk_ln = ln;
					chk_pn = FD_CRED_PN;
					return -1;
				}
			}
		}
	}
	return	0;
}



// 未入力チェック等で、表示データのスクロールが必要か？
//	int ln ... データ上の　行番号(1～100)
//
int CDnpBaseClass::NoinpScrolChk( int chk_ln, int chk_pn )
{
	int ln = 1;

	if( chk_ln >= (m_TopIdx+1) && chk_ln <= (m_TopIdx+getSCROLL_NL()) ) {
		for( ln = 1; ln <= getSCROLL_NL(); ln++ ) {
			if( (ln + m_TopIdx) == chk_ln ) {
				_set_focus( ln, chk_pn );
				break;
			}
		}
		return 0;
	}
	else {
		//未入力データの範囲外のためスクロールする。
		int vect;

		if( chk_ln > m_TopIdx ) {
			vect = chk_ln - m_TopIdx - 1;
		}
		else {
			vect = (m_TopIdx+1) - chk_ln;
			vect *= -1;
		}
FILETrace( "NoinpScrolChk m_TopIdx = %d, chk_ln = %d, vect = %d", m_TopIdx, chk_ln, vect );

		// データ移動
		if( DnpLine_Chg(vect,1) != 0 ) {
			for( ln = 1; ln <= getSCROLL_NL(); ln++ ) {
				if( (ln + m_TopIdx) == chk_ln ) {
					_set_focus( ln, chk_pn );
					break;
				}
			}
		}
		return 1;
	}
}


//---------------------------------------------------------
//	工事番号のチェック
//	int chk_ln	未入力科目があった場合、データ上の行番号(1～100)
//	int chk_pn	チェックで修正する場合のポジション
//	return	-1 = 未入力科目あり
//			-2 = 前期以前の完成工事番号チェックに引っかかる
//			 0 = なし
//---------------------------------------------------------
int CDnpBaseClass::CheckKOJI( int& chk_ln, int& chk_pn )
{
	if( ! m_DI.IsKoji() )	return 0;

	struct _DenpData	*debt, *cred;
	int			ln, dbcr_sg, ret = 0;
	CString		kno, kno2, cod;

	m_PRO_OK = FALSE;

	for( ln = 1; ln <= get_MaxLine(); ln++ ) {
		debt	=	NULL;
		cred	=	NULL;

		kno.Empty();
		kno2.Empty();

		if( IsKind() == KIND_FURIKAE ) {
			if( m_FuriMode == FURI_DATABMON ) {
				if( _getCND( ln, FD_DKOJI_PN )->INP_sg ) {
					kno = _getDATA( ln, FD_DKOJI_PN )->DP_KOJI;
				}
				if( _getCND( ln, FD_CKOJI_PN )->INP_sg ) {
					kno2 = _getDATA( ln, FD_CKOJI_PN )->DP_KOJI;
				}
			}
			else {
				if( getCND( DP_KOJI_PN )->INP_sg ) {
					kno = getDATA( DP_KOJI_PN )->DP_KOJI;
					kno2 = kno;
				}
			}
		}
		else {
			if( getCND( DP_KOJI_PN )->INP_sg ) {
				kno = getDATA( DP_KOJI_PN )->DP_KOJI;
				kno2 = kno;
			}
		}

		if( IsKind() != KIND_NYUKIN ) {
			if( _getCND( ln, FD_DEBT_PN )->INP_sg ) {
				debt	=	_getDATA( ln, FD_DEBT_PN );
			}
		}
		if( IsKind() != KIND_SYUKKIN ) {
			if( _getCND( ln, FD_CRED_PN )->INP_sg ) {
				cred	=	_getDATA( ln, FD_CRED_PN );
			}
		}

		//借方
		if( debt ) {
			cod	=	debt->DP_CdNum;
			dbcr_sg = (IsKind() == KIND_FURIKAE) ? 0 : -1;
			if( koji_noinpcheck( kno, cod, dbcr_sg ) ) {
				ret = -1;	break;
			}

			if( koji_kanseicheck( kno, cod, dbcr_sg ) ) {
				ret = -2;	break;
			}
		}
		//貸方
		if( cred ) {
			cod	=	cred->DP_CdNum;
			dbcr_sg = (IsKind() == KIND_FURIKAE) ? 1 : -1;
			if( koji_noinpcheck( kno2, cod, dbcr_sg ) ) {
				ret = -1;	break;
			}

			if( koji_kanseicheck( kno2, cod, dbcr_sg ) ) {
				ret = -2;	break;
			}
		}
	}
	if( ret < 0 ) {
		if( IsKind() != KIND_FURIKAE || (IsKind() == KIND_FURIKAE && m_FuriMode == FURI_TANBMON) ) {
			chk_ln = -1;
			chk_pn = DP_KOJI_PN;
		}
		else {
			chk_ln = ln;
			chk_pn = (dbcr_sg == 0) ? FD_DKOJI_PN : FD_CKOJI_PN;
		}
	}

	m_PRO_OK = TRUE;

	return	ret;
}

//---------------------------------------------------------
//	部門未入力のチェック
//	int chk_ln	未入力科目があった場合、データ上の行番号(1～100)
//	int chk_pn	チェックで修正する場合のポジション
//	return	-1 = 部門未入力のチェックに引っかかる
//			 0 = なし
//---------------------------------------------------------
int CDnpBaseClass::CheckBmnNoInp( int& chk_ln, int& chk_pn )
{	
	if( ! m_DI.IsBumon() )	return 0;

#ifdef CLOSE
	typedef struct _DNP_BHCHK
	{
		CString cod;
		int		bmn;
	} DNP_BHCHK;

	DNP_BHCHK	dbck;
	CArray<DNP_BHCHK, DNP_BHCHK&>	chkArray;
#endif

	struct _DenpData	*debt, *cred;
	int			ln, dbcr_sg, ret = 0;
	CString		cod;
	int	bmn, bmn2;
	int bmn_sgn, chk_ok;

	BOOL bFuriData = FALSE;

	if( IsKind() == KIND_FURIKAE ) {
		if( m_FuriMode == FURI_DATABMON ) {
			 bFuriData = TRUE;
		}
	}

	m_PRO_OK = FALSE;

	bmn_sgn = chk_ok = 0;

	chk_ln = -1;

	for( ln = 1; ln <= get_MaxLine(); ln++ ) {
		debt	=	NULL;
		cred	=	NULL;
		bmn = bmn2 = -1;

		if( bFuriData ) {
			if( _getCND( ln, FD_DBMN_PN )->INP_sg ) {
				bmn = _getDATA( ln, FD_DBMN_PN )->DP_BMN;
			}
			if( _getCND( ln, FD_CBMN_PN )->INP_sg ) {
				bmn2 = _getDATA( ln, FD_CBMN_PN )->DP_BMN;
			}
		}
		else {
			if( getCND( DP_BMN_PN )->INP_sg ) {
				bmn = getDATA( DP_BMN_PN )->DP_BMN;
				bmn2 = bmn;
			}
		}

		if( IsKind() != KIND_NYUKIN ) {
			if( _getCND( ln, FD_DEBT_PN )->INP_sg ) {
				debt	=	_getDATA( ln, FD_DEBT_PN );
			}
		}
		if( IsKind() != KIND_SYUKKIN ) {
			if( _getCND( ln, FD_CRED_PN )->INP_sg ) {
				cred	=	_getDATA( ln, FD_CRED_PN );
			}
		}

		//借方
		if( debt && bmn < 0 ) {
			cod	=	debt->DP_CdNum;
			dbcr_sg = (IsKind() == KIND_FURIKAE) ? 0 : -1;

			if( bmn_noinpcheck( bmn, cod, dbcr_sg ) ) {
				if( bFuriData ) {
					chk_pn = FD_DBMN_PN;
				}
				else {
					chk_pn = DP_BMN_PN;
				}
				ret = -1;
#ifdef CLOSE
				dbck.cod = cod;
				dbck.bmn = bmn;
				chkArray.Add( dbck );
#endif
				if( chk_ln == -1 )	chk_ln = ln;

				break;
			}
			else {
				chk_ok++;
			}

			bmn_sgn++;
		}
		//貸方
		if( cred && bmn2 < 0 ) {
			cod	=	cred->DP_CdNum;
			dbcr_sg = (IsKind() == KIND_FURIKAE) ? 1 : -1;

			if( bmn_noinpcheck( bmn, cod, dbcr_sg ) ) {
				if( bFuriData ) {
					chk_pn = FD_CBMN_PN;
				}
				else {
					chk_pn = DP_BMN_PN;
				}
				ret = -1;
#ifdef CLOSE
				dbck.cod = cod;
				dbck.bmn = bmn2;
				chkArray.Add( dbck );
#endif
				if( chk_ln == -1 )	chk_ln = ln;

				break;
			}
			else {
				chk_ok++;
			}

			bmn_sgn++;
		}
	}

	m_PRO_OK = TRUE;

	return	ret;
}

//---------------------------------------------------------
//	部門範囲のチェック
//	int chk_ln	未入力科目があった場合、データ上の行番号(1～100)
//	int chk_pn	チェックで修正する場合のポジション
//	return	-1 = 範囲チェックに引っかかる
//			 0 = なし
//---------------------------------------------------------
int CDnpBaseClass::CheckBmnHani( int& chk_ln, int& chk_pn )
{
	if( ! m_DI.IsBumon() )	return 0;

	typedef struct _DNP_BHCHK
	{
		CString cod;
		int		bmn;
	} DNP_BHCHK;

	DNP_BHCHK	dbck;
	CArray<DNP_BHCHK, DNP_BHCHK&>	chkArray;

	struct _DenpData	*debt, *cred;
	int			ln, dbcr_sg, ret = 0;
	CString		cod;
	int	bmn, bmn2;
	int bmn_sgn, chk_ok;
	DWORD dwCode;

	BOOL bFuriData = FALSE;

	if( IsKind() == KIND_FURIKAE ) {
		if( m_FuriMode == FURI_DATABMON ) {
			 bFuriData = TRUE;
		}
	}

	m_PRO_OK = FALSE;

	bmn_sgn = chk_ok = 0;

	chk_ln = -1;

	for( ln = 1; ln <= get_MaxLine(); ln++ ) {
		debt	=	NULL;
		cred	=	NULL;
		bmn = bmn2 = -1;

		if( bFuriData ) {
			if( _getCND( ln, FD_DBMN_PN )->INP_sg ) {
				bmn = _getDATA( ln, FD_DBMN_PN )->DP_BMN;
			}
			if( _getCND( ln, FD_CBMN_PN )->INP_sg ) {
				bmn2 = _getDATA( ln, FD_CBMN_PN )->DP_BMN;
			}
		}
		else {
			if( getCND( DP_BMN_PN )->INP_sg ) {
				bmn = getDATA( DP_BMN_PN )->DP_BMN;
				bmn2 = bmn;
			}
		}

		if( IsKind() != KIND_NYUKIN ) {
			if( _getCND( ln, FD_DEBT_PN )->INP_sg ) {
				debt	=	_getDATA( ln, FD_DEBT_PN );
			}
		}
		if( IsKind() != KIND_SYUKKIN ) {
			if( _getCND( ln, FD_CRED_PN )->INP_sg ) {
				cred	=	_getDATA( ln, FD_CRED_PN );
			}
		}

		//借方
		if( debt && bmn >= 0 ) {
			cod	=	debt->DP_CdNum;
			dbcr_sg = (IsKind() == KIND_FURIKAE) ? 0 : -1;

			if( ! m_BmChk.BmnHaniCheck( bmn, cod ) ) {
				dwCode = _atoh( (LPSTR)(LPCTSTR)cod );

				if( (m_pZm->zvol->bm_isw & 0x10) && dwCode < SONEKICODE ) {
					// 損益のみ部門の貸借はチェックしない
				}
				else {
					if( bFuriData ) {
						ret = -1;
					}
					dbck.cod = cod;
					dbck.bmn = bmn;
					chkArray.Add( dbck );

					if( chk_ln == -1 )	chk_ln = ln;
				}
			}
			else {
				chk_ok++;
			}

			bmn_sgn++;
		}
		//貸方
		if( cred && bmn2 >= 0 ) {
			cod	=	cred->DP_CdNum;
			dbcr_sg = (IsKind() == KIND_FURIKAE) ? 1 : -1;

			if( ! m_BmChk.BmnHaniCheck( bmn2, cod ) ) {
				dwCode = _atoh( (LPSTR)(LPCTSTR)cod );

				if( (m_pZm->zvol->bm_isw & 0x10) && dwCode < SONEKICODE ) {
					// 損益のみ部門の貸借はチェックしない
				}
				else {
					if( bFuriData ) {
						ret = -1;
					}
					dbck.cod = cod;
					dbck.bmn = bmn2;
					chkArray.Add( dbck );

					if( chk_ln == -1 )	chk_ln = ln;
				}
			}
			else {
				chk_ok++;
			}

			bmn_sgn++;
		}
	}

	DBKNREC* pKn;
	char buf[64];
	CString msg, str;
	int st, n;

	msg = "《部門範囲チェック》\r\n";

	if( bFuriData ) {
		if( ret < 0 ) {
			chk_ln = ln;
			chk_pn = (dbcr_sg == 0) ? FD_DBMN_PN : FD_CBMN_PN;

			for( n = 0; n < chkArray.GetCount(); n++ ) {
				if( ( pKn = DB_PjisToKnrec( chkArray[n].cod, m_pZm ) ) == NULL )
					return -1;

				set_codestr(buf, sizeof buf, chkArray[n].bmn, m_pZm->zvol->bmcol);

				str.Format( "\r\n科目「%.14s」に、部門番号『%s』は設定できません。", pKn->knnam, buf );
				msg += str;
			}
			msg += "\r\n\r\n仕訳を修正しますか？\r\nいいえの場合、部門を取消して仕訳を書き込みます。";

			Buzzer();

			st = myICSMessageBox( (LPCSTR)msg, MB_YESNO );
			if( st == IDNO )	ret = 0;
			else				ret = -1;
		}
	}
	else {
		if( bmn_sgn && ! chk_ok ) {

			chk_ln = -1;
			chk_pn = DP_BMN_PN;

			for( n = 0; n < chkArray.GetCount(); n++ ) {
				if( ( pKn = DB_PjisToKnrec( chkArray[n].cod, m_pZm ) ) == NULL )
					return -1;

				set_codestr(buf, sizeof buf, chkArray[n].bmn, m_pZm->zvol->bmcol);

				str.Format( "\r\n科目「%.14s」に、部門番号『%s』は設定できません。", pKn->knnam, buf );
				msg += str;
			}
#ifdef CLOSE
			m_pZm->BumonCodeToStr( buf, sizeof buf, bmn );

			str.Format( "\r\nこの伝票で入力した科目に、部門番号『%s』は設定できません。", buf );
			msg += str;
#endif
			msg += "\r\n\r\n仕訳を修正しますか？\r\nいいえの場合、部門を取消して仕訳を書き込みます。";

			Buzzer();

			st = myICSMessageBox( (LPCSTR)msg, MB_YESNO );
			if( st == IDNO )	ret = 0;
			else				ret = -1;
		}
	}

	m_PRO_OK = TRUE;

	return	ret;
}


//---------------------------------------------------------
//	部門範囲のチェック
//	int chk_pn	チェックで修正する場合のポジション
//	return	-1 = 範囲チェックに引っかかる
//			 0 = なし
//---------------------------------------------------------
int CDnpBaseClass::CheckKojiKmk( int& chk_ln, int& chk_pn )
{
	if( ! m_DI.IsKoji() )	return 0;

	if( m_pAsel->KJIKMK_OPT != OPT_KJIKMK)
		return 0;

	typedef struct _DNP_KJCHK
	{
		CString		cod;
		CString		kno;
	} DNP_KJCHK;

	DNP_KJCHK	dbck;
	CArray<DNP_KJCHK, DNP_KJCHK&>	chkArray;

	struct _DenpData	*debt, *cred;
	int			ln, dbcr_sg, ret = 0;
	CString		cod, kno, kno2;
	int kno_sgn, chk_ok;
	DWORD dwCode;

	BOOL bFuriData = FALSE;

	if( IsKind() == KIND_FURIKAE ) {
		if( m_FuriMode == FURI_DATABMON ) {
			 bFuriData = TRUE;
		}
	}

	m_PRO_OK = FALSE;

	kno_sgn = chk_ok = 0;

	chk_ln = -1;

	for( ln = 1; ln <= get_MaxLine(); ln++ ) {
		debt	=	NULL;
		cred	=	NULL;

		kno.Empty();
		kno2.Empty();

		if( bFuriData ) {
			if( _getCND( ln, FD_DKOJI_PN )->INP_sg ) {
				kno = _getDATA( ln, FD_DKOJI_PN )->DP_KOJI;
			}
			if( _getCND( ln, FD_CKOJI_PN )->INP_sg ) {
				kno2 = _getDATA( ln, FD_CKOJI_PN )->DP_KOJI;
			}
		}
		else {
			if( getCND( DP_KOJI_PN )->INP_sg ) {
				kno = getDATA( DP_KOJI_PN )->DP_KOJI;
				kno2 = kno;
			}
		}

		if( IsKind() != KIND_NYUKIN ) {
			if( _getCND( ln, FD_DEBT_PN )->INP_sg ) {
				debt	=	_getDATA( ln, FD_DEBT_PN );
			}
		}
		if( IsKind() != KIND_SYUKKIN ) {
			if( _getCND( ln, FD_CRED_PN )->INP_sg ) {
				cred	=	_getDATA( ln, FD_CRED_PN );
			}
		}

		//借方
		if( debt && ! kno.IsEmpty() ) {
			cod	=	debt->DP_CdNum;
			dbcr_sg = (IsKind() == KIND_FURIKAE) ? 0 : -1;

			if( ! m_KjChk.KojiKmkCheck( cod ) ) {
				dwCode = _atoh( (LPSTR)(LPCTSTR)cod );

				if( bFuriData ) {
					ret = -1;
				}
				dbck.cod = cod;
				dbck.kno = kno;
				chkArray.Add( dbck );

				if( chk_ln == -1 )	chk_ln = ln;
			}
			else {
				chk_ok++;
			}

			kno_sgn++;
		}
		//貸方
		if( cred && ! kno2.IsEmpty() ) {
			cod	=	cred->DP_CdNum;
			dbcr_sg = (IsKind() == KIND_FURIKAE) ? 1 : -1;

			if( ! m_KjChk.KojiKmkCheck( cod ) ) {
				dwCode = _atoh( (LPSTR)(LPCTSTR)cod );

				if( bFuriData ) {
					ret = -1;
				}
				dbck.cod = cod;
				dbck.kno = kno2;
				chkArray.Add( dbck );

				if( chk_ln == -1 )	chk_ln = ln;
			}
			else {
				chk_ok++;
			}

			kno_sgn++;
		}
	}

	DBKNREC* pKn;
	char buf[64];
	CString msg, str;
	int st, n;

	msg = "《工事番号 科目チェック》\r\n";

	if( bFuriData ) {
		if( ret < 0 ) {
			chk_ln = ln;
			chk_pn = (dbcr_sg == 0) ? FD_DKOJI_PN : FD_CKOJI_PN;

			for( n = 0; n < chkArray.GetCount(); n++ ) {
				if( ( pKn = DB_PjisToKnrec( chkArray[n].cod, m_pZm ) ) == NULL )
					return -1;

				set_codestring( buf, sizeof buf, (char*)(LPCTSTR)chkArray[n].kno, m_pZm->zvol->kjcol );

				str.Format( "\r\n科目「%.14s」に、工事番号『%s』は設定できません。", pKn->knnam, buf );
				msg += str;
			}
			msg += "\r\n\r\n仕訳を修正しますか？\r\nいいえの場合、工事番号を取消して仕訳を書き込みます。";

			Buzzer();

		//	st = myICSMessageBox( (LPCSTR)msg, MB_YESNO );
			st = IDNO;
			if( st == IDNO )	ret = 0;
			else				ret = -1;
		}
	}
	else {
		if( kno_sgn && ! chk_ok ) {

			chk_ln = -1;
			chk_pn = DP_KOJI_PN;

			for( n = 0; n < chkArray.GetCount(); n++ ) {
				if( ( pKn = DB_PjisToKnrec( chkArray[n].cod, m_pZm ) ) == NULL )
					return -1;

				set_codestring( buf, sizeof buf, (char*)(LPCTSTR)chkArray[n].kno, m_pZm->zvol->kjcol );

				str.Format( "\r\n科目「%.14s」に、工事番号『%s』は設定できません。", pKn->knnam, buf );
				msg += str;
			}
#ifdef CLOSE
			m_pZm->BumonCodeToStr( buf, sizeof buf, kno );

			str.Format( "\r\nこの伝票で入力した科目に、部門番号『%s』は設定できません。", buf );
			msg += str;
#endif
			msg += "\r\n\r\n仕訳を修正しますか？\r\nいいえの場合、工事番号を取消して仕訳を書き込みます。";

			Buzzer();

		//	st = myICSMessageBox( (LPCSTR)msg, MB_YESNO );
			st = IDNO;
			if( st == IDNO )	ret = 0;
			else				ret = -1;
		}
	}

	m_PRO_OK = TRUE;

	return	ret;
}



//部門登録チェック
int CDnpBaseClass::CheckBmnTorok( int& chk_ln, int& chk_pn )
{
	if( m_pAsel->BMNCHK_OPT != OPT_BMNCHK )
		return FALSE;

	if( ! m_DI.IsBumon() )	return 0;

	int			ln, ret = 0;
	CString		cod;
	int	bmn, bmn2;
	int bmn_sgn, chk_ok;

	vector<int>	bmnvec;

	BOOL bFuriData = FALSE;

	if( IsKind() == KIND_FURIKAE ) {
		if( m_FuriMode == FURI_DATABMON ) {
			 bFuriData = TRUE;
		}
	}

	m_PRO_OK = FALSE;
	bmn_sgn = chk_ok = 0;

	chk_ln = -1;

	for( ln = 1; ln <= get_MaxLine(); ln++ ) {
		bmn = bmn2 = -1;

		if( bFuriData ) {
			if( _getCND( ln, FD_DBMN_PN )->INP_sg ) {
				bmn = _getDATA( ln, FD_DBMN_PN )->DP_BMN;
			}
			if( _getCND( ln, FD_CBMN_PN )->INP_sg ) {
				bmn2 = _getDATA( ln, FD_CBMN_PN )->DP_BMN;
			}
		}
		else {
			if( getCND( DP_BMN_PN )->INP_sg ) {
				bmn = getDATA( DP_BMN_PN )->DP_BMN;
				bmn2 = bmn;
			}
		}

		//借方
		if( bmn >= 0 ) {
			if( BmnTorokMessage( bmn, bmnvec ) ) {
				if( bFuriData ) {
					chk_pn = FD_DBMN_PN;
				}
				else {
					chk_pn = DP_BMN_PN;
				}
				ret = -1;
#ifdef CLOSE
				dbck.cod = cod;
				dbck.bmn = bmn;
				chkArray.Add( dbck );
#endif
				if( chk_ln == -1 )	chk_ln = ln;

				break;
			}
			else {
				chk_ok++;
			}

			bmn_sgn++;
		}
		//貸方
		if( bmn2 >= 0 ) {
			if( BmnTorokMessage( bmn2, bmnvec ) ) {
				if( bFuriData ) {
					chk_pn = FD_CBMN_PN;
				}
				else {
					chk_pn = DP_BMN_PN;
				}
				ret = -1;
#ifdef CLOSE
				dbck.cod = cod;
				dbck.bmn = bmn2;
				chkArray.Add( dbck );
#endif
				if( chk_ln == -1 )	chk_ln = ln;

				break;
			}
			else {
				chk_ok++;
			}

			bmn_sgn++;
		}
	}

	m_PRO_OK = TRUE;

	return	ret;
}



//工事登録チェック
int CDnpBaseClass::CheckKojiTorok( int& chk_ln, int& chk_pn )
{
	if( m_pAsel->BMNCHK_OPT != OPT_BMNCHK )
		return FALSE;

	// 部門入力不可
	if( ! m_DI.IsKoji() ) return FALSE;

	int			ln, ret = 0;
	CString		cod;
	CString		koji, koji2;
	int chk_ok;

	vector<CString>	kojivec;

	BOOL bFuriData = FALSE;

	if( IsKind() == KIND_FURIKAE ) {
		if( m_FuriMode == FURI_DATABMON ) {
			 bFuriData = TRUE;
		}
	}

	m_PRO_OK = FALSE;
	chk_ok = 0;

	chk_ln = -1;

	for( ln = 1; ln <= get_MaxLine(); ln++ ) {
		koji.Empty();
		koji2.Empty();

		if( bFuriData ) {
			if( _getCND( ln, FD_DKOJI_PN )->INP_sg ) {
				koji = _getDATA( ln, FD_DKOJI_PN )->DP_KOJI;
			}
			if( _getCND( ln, FD_CKOJI_PN )->INP_sg ) {
				koji2 = _getDATA( ln, FD_CKOJI_PN )->DP_KOJI;
			}
		}
		else {
			if( getCND( DP_KOJI_PN )->INP_sg ) {
				koji = getDATA( DP_KOJI_PN )->DP_KOJI;
				koji2 = koji;
			}
		}

		//借方
		if( ! koji.IsEmpty() ) {
			if( KojiTorokMessage( koji, kojivec ) ) {
				if( bFuriData ) {
					chk_pn = FD_DKOJI_PN;
				}
				else {
					chk_pn = DP_KOJI_PN;
				}
				ret = -1;
				if( chk_ln == -1 )	chk_ln = ln;

				break;
			}
			else {
				chk_ok++;
			}

		}
		//貸方
		if( ! koji2.IsEmpty() ) {
			if( KojiTorokMessage( koji2, kojivec ) ) {
				if( bFuriData ) {
					chk_pn = FD_CKOJI_PN;
				}
				else {
					chk_pn = DP_KOJI_PN;
				}
				ret = -1;
				if( chk_ln == -1 )	chk_ln = ln;

				break;
			}
			else {
				chk_ok++;
			}

		}
	}

	m_PRO_OK = TRUE;

	return	ret;
}



//-----------------------------
// データ書き込み
//-----------------------------
int CDnpBaseClass::AppendData( CDBINPDataRec *dat, int max, ADDNEWTBL* antbl )
{
int ret = 0;
int n, icno;
CDBINPDataRec *crec;

	m_pDnpParent->BeginWaitCursor();

	int bseq;

	for( n = 0 ; n < max ; ++n )
	{
		bseq = -1;
		crec = dat++;

		if( IsJob() == JOB_INPUT ) {
			if( n == 0 ) {
				// 内部伝票番号を取り込む
				if( IsKind() == KIND_FURIKAE ) {
					if( m_InsideCno < 0 ) {
						icno = m_pZm->DcntlLock( 36, 8 );
						//複数伝票をサポート
						if( m_InsideCno == -2 )
							m_InsideCno = icno;	
					}
					else {
						icno = m_InsideCno;
					}
				}
				else {
					icno = m_pZm->DcntlLock( 36, 8 );
				}
			}
		}
		else {
			icno = m_DPscan.GetSheetICNO();

			if( antbl[n].insseq > 0 )
				bseq = antbl[n].insseq;
		}
		// 内部伝票番号セット
		crec->m_icno = icno;

		// ＳＥＱ番号セット
		crec->m_seq = m_DI.m_dacnt;
		// 取消サインＯＦＦ
		crec->m_dsign[0] &= 0xfe;

#ifdef	_DEBUG
	char	kari[32], kasi[32];
	DBKNREC	*reckari, *reckasi;
	char	val[32];
	char	teki[128];

	TRACE( "Append:\n" );
	TRACE( "---------------------------------------------------------\n" );
	reckari	=	DB_PjisToKnrec( crec->m_dbt, m_pZm );
	reckasi	=	DB_PjisToKnrec( crec->m_cre, m_pZm );
	memset( kari, 0, sizeof kari );
	memset( kasi, 0, sizeof kasi );
	memmove( kari, reckari->knnam, 14 );
	memmove( kasi, reckasi->knnam, 14 );
	l_print( val, crec->m_val, "SSS,SSS,SSS,SS9" );
	memset( teki, 0, sizeof teki );
	memmove( teki, crec->m_tekiyo, crec->m_tekiyo.GetLength() );
	TRACE( "%08d %5d  %5d bm=%d,%d kj=%s,%s %s %s %s %s \n%02x %02x %02x %02x %02x",
		crec->m_ddate, crec->m_cno, crec->m_seq,
		crec->m_dbmn, crec->m_cbmn, crec->m_dkno, crec->m_ckno,
		kari, kasi, val, teki, crec->m_dsign[0], crec->m_dsign[1], crec->m_dsign[2], crec->m_dsign[3], crec->m_dsign[4] );
	if( crec->m_dtype == 0x10 )	TRACE( " 入金" );
	else if( crec->m_dtype == 0x20 )	TRACE( " 出金" );
	else if( crec->m_dtype == 0x30 )	TRACE( " 振替" );
	else	TRACE( " その他" );
	TRACE( "\n" );
#	endif

		//工事番号 科目
		CheckAppendKojiKmk( crec );
		//部門範囲
		CheckAppendBumon( crec );

		//インボイス登録番号対象か？
		m_DI.is_invnosiwake(crec);

//#ifndef	_FIX_04_OCT_02_
		// データ書き込み＆残高更新

		// DBDATAへデータセット
		if( m_DI.DB_DataAppend( crec, bseq ) != 0 ) {
			ret = -1;
			break;
		}

		m_pDENPPAR->addseq.push_back(crec->m_seq);
		m_pDENPPAR->addins_seq.push_back(bseq);

#ifdef LATER_CLOSE
		// 追加を通常入力にも反映
		DBDinpAppendParamUpdate( crec, bseq );
		//諸口・現金貸借
		syog_genk( crec, _ADD );
#endif
//		m_DPscan.AppendData( crec );

	}

	m_pDnpParent->EndWaitCursor();

	return ret;
}



// 仕訳追加時の 工事番号 科目チェック
int CDnpBaseClass::CheckAppendKojiKmk( CDBINPDataRec* rec )
{
	if( IsJob() != JOB_INPUT ) {
		return 0;
	}

	if( ! m_DI.IsKoji() )	return 0;

	if( m_pAsel->KJIKMK_OPT != OPT_KJIKMK)
		return 0;

	int chgsgn;
	CString code, kno;

	chgsgn = 0;

	for( int i = 0; i < 2; i++ ) {

		if( i == 0 ) {
			kno		= rec->m_dkno;
			code	= rec->m_dbt;
		}
		else {
			kno		= rec->m_ckno;
			code	= rec->m_cre;
		}

		if( kno.IsEmpty() )
			continue;

		if( ! m_KjChk.KojiKmkCheck( code ) ) {
			if( i == 0 )	rec->m_dkno.Empty();
			else			rec->m_ckno.Empty();

			chgsgn++;
		}
	}

	return (chgsgn) ? 1 : 0;
}



// 仕訳追加時の 部門範囲チェック
int CDnpBaseClass::CheckAppendBumon( CDBINPDataRec* rec )
{
	if( IsJob() != JOB_INPUT ) {
		return 0;
	}

	int bmn, chgsgn;
	CString code;

	chgsgn = 0;

	for( int i = 0; i < 2; i++ ) {

		if( i == 0 ) {
			bmn		= rec->m_dbmn;
			code	= rec->m_dbt;
		}
		else {
			bmn		= rec->m_cbmn;
			code	= rec->m_cre;
		}

		if( bmn == -1 )
			continue;

		if( ! m_BmChk.BmnHaniCheck( bmn, code ) ) {
			if( i == 0 )	rec->m_dbmn = -1;
			else			rec->m_cbmn = -1;

			chgsgn++;
		}
	}

	return (chgsgn) ? 1 : 0;
}


//部門登録メッセージ
BOOL CDnpBaseClass::BmnTorokMessage( int bmn, vector<int>&	bmnvec, BOOL bMsg/*=TRUE*/ )
{
	CString msg;
	char	tmp[64];
	BOOL	bRet = FALSE;

	vector<int>::iterator	it;
	it = find(bmnvec.begin(), bmnvec.end(), bmn);

	//チェック済み
	if( it != bmnvec.end() ) {
		return FALSE;
	}

	if( !m_DI.bmncode_ok( bmn ) ) {
		set_codestr( tmp, sizeof tmp, bmn, m_pZm->zvol->bmcol );

		int st;
		if( bMsg ) {
			msg.Format( " 部門[%s]は登録されていません！\n\n 登録しますか？\r\n\r\n はい…自動登録、いいえ…部門入力に戻る", tmp );
			st = myICSMessageBox( msg, MB_YESNO|MB_DEFBUTTON2 );
		}
		else {
			st = IDYES;
		}
		if( st == IDNO ) {
			bRet = TRUE;
		}
		else {
			//自動登録する部門
			bmnvec.push_back( bmn );
		}
	}

	return bRet;
}


//工事登録メッセージ
BOOL CDnpBaseClass::KojiTorokMessage( CString koji, vector<CString>&	kojivec, BOOL bMsg/*=TRUE*/ )
{
	CString msg;
	char	tmp[64];
	BOOL	bRet = FALSE;

	vector<CString>::iterator	it;
	it = find(kojivec.begin(), kojivec.end(), koji);

	//チェック済み
	if( it != kojivec.end() ) {
		return FALSE;
	}

	if( ! Kojicode_Ok( koji ) ) {
		set_codestring( tmp, sizeof tmp, (char*)(LPCTSTR)koji, m_pZm->zvol->kjcol );

		int st;
		if( bMsg ) {
			msg.Format( " 工事[%s]は登録されていません！\n\n 処理を継続しますか？\r\n\r\n はい…処理継続、いいえ…工事入力に戻る", tmp );
			st = myICSMessageBox( msg, MB_YESNO|MB_DEFBUTTON2 );
		}
		else {
			st = IDYES;
		}
		if( st == IDNO ) {
			bRet = TRUE;
		}
		else {
			//チェック済みする工事
			kojivec.push_back( koji );
		}
	}

	return bRet;
}



// 諸口持越し
BOOL CDnpBaseClass::syog_next()
{
	BOOL sg;
	
	l_add( SYOG_TAI, SYOG_TAI, SHEET_SYOG_TAI );
	l_clear( SHEET_SYOG_TAI );

	sg = l_test( SYOG_TAI ) ? TRUE : FALSE;

	return sg;
}



// 振替伝票の設定情報をセット
//
//
void CDnpBaseClass::ProfileSet() 
{
	CString	fpath;
	CString filter;
	BOOL	bOpen = FALSE;

	if( m_pZm->owntb == NULL ) {
		m_pZm->OwnTblOpen();
		bOpen = TRUE;
	}

	ICSMCTL	mctrl;
	mctl_usertbl	utbl;

	if( mctrl.SetDatabase() == 0 ) {
		mctrl.get_usertbl( utbl );
	}

#define	PRO_NAME	"DBDinp"
#define SETUP_NAME	"DBDINP_SETUP2"

	filter.Format( "apl_name = '%s' AND itm_name = '%s' AND u_id = %d", PRO_NAME, SETUP_NAME, utbl.user_id );

	m_pZm->owntb->Requery( filter );

	//CMainFrame::DispInfoWork を参照
	if( m_pZm->owntb->st == -1 ) {
		m_pZm->owntb->AddNew();
		m_pZm->owntb->apl_name = PRO_NAME;
		m_pZm->owntb->itm_name = SETUP_NAME; 
		m_pZm->owntb->itm_seq = 1;
		m_pZm->owntb->u_id = utbl.user_id;
		//複合仕訳をデフォルトで
		m_pZm->owntb->vl[0] = 1;
		//資金繰り枝番 vl[1] は 前回表示画面
		m_pZm->owntb->vl[2] = 0;
		//振替伝票 部門
		m_pZm->owntb->vl[7] = 0;
		//諸口詰めるか？
		m_pZm->owntb->vl[8] = 0;

		m_pZm->owntb->Update();
		m_pZm->owntb->Requery( filter );
	}

	if( m_pZm->owntb->vl[0] == 0 ) {
		IsDefaultType()	=	TYPE_TANDOKU;
	}
	else {
		IsDefaultType()	=	TYPE_FUKUGOU;
	}

	//資金繰り枝番
	if( m_pZm->owntb->vl[2] <= 0 ) {
		SknBrnSign()	=	SKNBRN_NONE;
	}
	else {
		SknBrnSign()	=	SKNBRN_INPUT;
	}

	//修正時、諸口仕訳表示モード
	if( m_pZm->owntb->vl[8] <= 0 ) {
		SyogSiwakeMode() = FALSE;
	}
	else {
		SyogSiwakeMode() = TRUE;
	}
	m_DPscan.SyogSiwakeMode() = SyogSiwakeMode();

	BOOL bOldSyogMode = SyogSiwakeMode();

	if( bOpen ) {
		m_pZm->OwnTblClose();
	}
}



//スキャンモード終了
//	return -1 未入力などのメッセ―ジ
//	return -2 Error
//
int CDnpBaseClass::EndScan( BOOL bUpdate/*=TRUE*/ )
{
	DBGFUNCTION;
	if (!m_PRO_OK) return 0;

	if( IsJob() != JOB_SCAN )	return 0;

	Destroy_SyzInp();

	if( bUpdate ) {
		//修正、追加処理
		if( UpdatePage( 0 ) )	return	-1;
	}

	int max = get_MaxLine();

	set_nowln( 1 );
	set_nowpn( DP_DENP_PN );

	m_DPscan.Close();

//	AfxMessageBox( "EndScan end" );


	return	0;
}

//-----------------------------------------------------------------------------------
//	伝票修正時の伝票更新
//	mode:
//		0	:	必ず更新(End)
//		1	:	PageDown
//		-1	:	PageUp
//		2	:	チェックのみ(問い合せ呼出時)
//		100 :	他のプログラムにチェインする時の更新
//
//
//-----------------------------------------------------------------------------------
int CDnpBaseClass::UpdatePage( int mode )
{
	BOOL	bMsg;
	bMsg = (mode ==100) ? FALSE : TRUE;

	//キー押下の時データ取得
	lastinput_get();

	Destroy_SyzInp();
	// DBEditを戻す
	dbedt_putback();

	if( DateCheck_and_action( bMsg ) == -1 )	return	-1;
//	last_data_set();

	// 貸借チェック
	if( ScanTotalCheck( mode, bMsg ) ) {
		return	-1;
	}

	// 確定マスターは、変更不可
	if( m_DI.IsConFirmMaster() )
		return 0;

	// 追加データ用構造体
	ADDNEWTBL	antbl[ EXPAN_MAX ] = {0};
	int oldnewcnt, syog_dbt, syog_cre;

	//修正仕訳作成
	int i;
	int	newcnt	=	0;
	int	lcnt	=	0;

	const int maxLine	= get_MaxLine();

	CArray<CDBINPDataRec, CDBINPDataRec&> drec;
//	CDBINPDataRec	drec[maxLine*2];
	drec.SetSize( maxLine*2 );

	for( i = 0; i < EXPAN_MAX; i++ )	// テーブルイニシャライズ
		EXPAN_TBL[i].Reset();
	for( i = 0; i < (maxLine*2); i++)
		drec[i].Reset();

#ifdef LATER_CLOSE
	if( m_DPscan.GetSheetCount() > 1 ) {
		m_DPscan.NewCount()	-=	m_DPscan.SheetNewCount();
	}
#endif

	for( int ln = 1; ln <= get_MaxLine(); ln++ ) {
		oldnewcnt = newcnt;

		//空白ラインであったライン
		if( !m_DPscan.IsOriginData( ln, 0 ) && !m_DPscan.IsOriginData( ln, 1 ) ) {
			newcnt	+=	MakeNewData( mode, ln, &EXPAN_TBL[newcnt] );

			int ins_seq = 0;

			if( m_DPscan.IsInsertLine( ln ) ) {
				for( int nxtln = (ln+1); nxtln <= get_MaxLine(); nxtln++ ) {
					if( m_DPscan.IsOriginData( nxtln, 0 ) ) {
						ins_seq = m_DPscan.GetSeq( nxtln, 0 );
						break;
					}
					else if( m_DPscan.IsOriginData( nxtln, 1 ) ) {
						ins_seq = m_DPscan.GetSeq( nxtln, 1 );
						break;
					}
				}
			}

			if( newcnt > oldnewcnt ) {
				if( (newcnt - oldnewcnt) == 2 ) {
					// 複合仕訳
					antbl[oldnewcnt].ln			= ln;
					antbl[oldnewcnt].karikasi	= 0;
					antbl[oldnewcnt].insseq		= ins_seq;
					antbl[oldnewcnt+1].ln		= ln;
					antbl[oldnewcnt+1].karikasi	= 1;
					antbl[oldnewcnt+1].insseq	= ins_seq;
				}
				else {
					syog_dbt = syog_cre = 0;
					if( isSYOGT_CODE( EXPAN_TBL[oldnewcnt].m_dbt ) )	syog_dbt = 1;
					if( isSYOGT_CODE( EXPAN_TBL[oldnewcnt].m_cre ) )	syog_cre = 1;

					antbl[oldnewcnt].ln		=	ln;
					antbl[oldnewcnt].insseq	= ins_seq;

					// 複合仕訳
					if( syog_dbt || syog_cre ) {
						if( syog_dbt ) {
							antbl[oldnewcnt].karikasi	= 1;
						}
						else {
							antbl[oldnewcnt].karikasi	= 0;
						}
					}
					else {
						// 単一仕訳
						antbl[oldnewcnt].karikasi	= 2;
					}
				}
			}
		}
		//振替伝票で貸借にデータがあったライン
		else if( m_DPscan.IsOriginData( ln, 0 ) && m_DPscan.IsOriginData( ln, 1 ) ) {
			ModifyTaisyakuData( mode, ln, newcnt, &EXPAN_TBL[newcnt], lcnt, &drec[lcnt] );
		}
		//振替伝票で借方のみにデータがあったか、出金伝票でデータ有り
		else if( m_DPscan.IsOriginData( ln, 0 ) ) {
			ModifyDebtData( mode, ln, newcnt, &EXPAN_TBL[newcnt], lcnt, &drec[lcnt] );
			// 貸方に 新規追加
			if( newcnt > oldnewcnt ) {
				antbl[oldnewcnt].ln			= ln;
				antbl[oldnewcnt].karikasi	= 1;
			}
		}
		//振替伝票で貸方のみにデータがあったか、入金伝票でデータ有り
		else {
			ModifyCredData( mode, ln, newcnt, &EXPAN_TBL[newcnt], lcnt, &drec[lcnt] );
			// 借方に 新規追加
			if( newcnt > oldnewcnt ) {
				antbl[oldnewcnt].ln			= ln;
				antbl[oldnewcnt].karikasi	= 0;
			}
		}
	}

	//仕訳登録数のチェック
	unsigned long	max	=	newcnt;
	if( m_DPscan.GetSheetCount() > 1 ) {
		m_DPscan.SheetNewCount()	=	newcnt;
		max = m_DPscan.SheetNewCount();
#ifdef OLD_CLOSE
		m_DPscan.NewCount()	+=	m_DPscan.SheetNewCount();
		max	=	m_DPscan.NewCount();
#endif
	}


#ifdef OLD_CLOSE
	//複数枚に渡る時
	if( m_DPscan.GetSheetCount() > 1 ) {
		//複数枚に渡る時の途中更新
		m_DPscan.UpdateSheetTable();
		if( !mode || !m_DPscan.IsMultiSheet( mode ) ) {
			//複数枚に渡る時の最終更新
			if( ModifyMultiData() )	return	-1;
		}
	}
	else {
#endif

		int chk_ln, chk_pn;

		//データ更新前の枝番チェック
		if( ModifyCheckEdabanInput( &drec[0], lcnt, chk_ln, chk_pn, bMsg ) ) {
			_set_focus( chk_ln, chk_pn );
			set_nowln( 1 );
			return	-1;
		}

		//データ更新前の工事番号 科目チェック
		if( ModifyCheckKoji( &drec[0], lcnt, chk_ln, chk_pn, bMsg ) ) {
			_set_focus( chk_ln, chk_pn );
			set_nowln( 1 );
			return	-1;
		}

		if( ModifyCheckKojikmk( &drec[0], lcnt, chk_ln, chk_pn, bMsg ) ) {
			_set_focus( chk_ln, chk_pn );
			set_nowln( 1 );
			return	-1;
		}
		//データ更新前の工事番号 登録チェック
		if( ModifyCheckKojiTorok( &drec[0], lcnt, chk_ln, chk_pn, bMsg ) ) {
			_set_focus( chk_ln, chk_pn );
			set_nowln( 1 );
			return	-1;
		}

		//データ更新前の部門未入力チェック
		if( ModifyCheckBmnNoInp( &drec[0], lcnt, chk_ln, chk_pn, bMsg ) ) {
			_set_focus( chk_ln, chk_pn );
			set_nowln( 1 );
			return	-1;
		}

		//データ更新前の部門範囲チェック
		if( ModifyCheckBmnHani( &drec[0], lcnt, chk_ln, chk_pn, bMsg ) ) {
			_set_focus( chk_ln, chk_pn );
			set_nowln( 1 );
			return	-1;
		}
		//データ更新前の部門登録チェック
		if( ModifyCheckBmnTorok( &drec[0], lcnt, chk_ln, chk_pn, bMsg ) ) {
			_set_focus( chk_ln, chk_pn );
			set_nowln( 1 );
			return	-1;
		}

		//データ更新
		if( ModifyData( &drec[0], lcnt ) )	return	-2;

		//データ追加
		if( AppendData( EXPAN_TBL, newcnt, antbl ) )	return	-2;

		//他のプログラムに行く際は、画面に伝票の状態を残しているので、
		//シートテーブル情報を更新する
	//	if( mode == 100 ) {		---> 100行対応で、新規ラインの情報を常に保存
							//	---> F5 連続押しで、新規行が何行も追加される。
			if( newcnt > 0 ) {
				m_DPscan.SheetTableUpdate( newcnt, EXPAN_TBL, antbl );
			}
	//	}
#ifdef OLD_CLOSE
	}
#endif

	return	0;
}



//UpdatePage()実行時の貸借チェック
int CDnpBaseClass::ScanTotalCheck( int mode, BOOL bMsg/*=TRUE*/ )
{
	if( IsKind() != KIND_FURIKAE )	return	0;

	int	sw	=	0;

	if( m_DPscan.GetSheetCount() > 1 ) {
		//伝票が二枚以上の時で伝票毎の途中計算
		ScanAllTotalClc();

		if( mode ) {
//			if( m_DPscan.IsMultiSheet( mode ) == FALSE )	sw	=	1;
			if( m_DPscan.IsFurikaeEnd( mode ) )	sw	=	1;
		}
		else	sw	=	1;
	}
	else	sw	=	1;

	//最終チェックが必要な時
	if( sw ) {
		if( total_chk() ) {
			int ret;

			if( bMsg ) {
				ret	=	myICSMessageBox( "貸借金額が一致していません。\n伝票を修正しますか？\n\n「いいえ」の場合、仕訳を登録します。", MB_YESNO );
				if( ret == IDYES ) {
					_set_focus( 0, DP_DENP_PN );
					set_nowln( 1 );
					return	-1;
				}
			}
		}
	}

	return	0;
}




//新規データ作成
//	int ln ... データ上の　行番号(1～100)
int CDnpBaseClass::MakeNewData( int mode, int ln, CDBINPDataRec* drec )
{
	BOOL	sw	=	FALSE;

	if( IsKind() != KIND_NYUKIN ) {
		if( _getCND( ln, FD_DEBT_PN )->INP_sg || _getCND( ln, FD_DBVAL_PN )->INP_sg )	sw	=	TRUE;
	}
	if( IsKind() != KIND_SYUKKIN ) {
		if( _getCND( ln, FD_CRED_PN )->INP_sg || _getCND( ln, FD_CRVAL_PN )->INP_sg )	sw	=	TRUE;
	}
	
	if( _getCND( ln, FD_TKY_PN )->INP_sg )	sw = TRUE;

	if( IsKind() == KIND_FURIKAE ) {
		if( _getCND( ln, FD_DSKBN_PN )->INP_sg || _getCND( ln, FD_CSKBN_PN )->INP_sg ) {
			sw	=	TRUE;
		}
	}
	else if( IsKind() == KIND_NYUKIN ) {
		if( _getCND( ln, FD_DSKBN_PN )->INP_sg )	sw = TRUE;
	}
	else {
		if( _getCND( ln, FD_CSKBN_PN )->INP_sg )	sw = TRUE;
	}

	//複数枚に渡る時
	if( m_DPscan.GetSheetCount() > 1 ) {
		if( sw == FALSE ) {
			m_DPscan.AddSheetTable( ln, 2, NULL, NULL );
			return	0;
		}

		CDBINPDataRec	drec1, drec2;
		drec1.Reset();
		drec2.Reset();

		struct _DenpData	*debt, *dbval, *dbzei, *cred, *crval, *crzei;
		struct _DenpData	*denp, *teki, *dzei, *czei;
		char	tbuf[128];
		int		klen, bmcd, c_bmn, temp;
		int		ssw;
		CString kno, c_kno, syonum;

		bmcd = c_bmn = -1;
		kno.Empty();
		c_kno.Empty();
		syonum.Empty();

		debt	=	_getMAPtoDATA( ln, FD_DEBT_PN );
		dbval	=	_getDATA( ln, FD_DBVAL_PN );
		dbzei	=	_getDATA( ln, FD_DBZEI_PN );
		cred	=	_getMAPtoDATA( ln, FD_CRED_PN );
		crval	=	_getDATA( ln, FD_CRVAL_PN );
		crzei	=	_getDATA( ln, FD_CRZEI_PN );
		teki	=	_getDATA( ln, FD_TKY_PN );
		dzei	=	_getDATA( ln, FD_DSKBN_PN );
		czei	=	_getDATA( ln, FD_CSKBN_PN );
		
		denp	=	_getDATA( 0, DP_DENP_PN );

		if( m_DI.IsSyonum() ) {
			drec1.m_skn = getDATA(DP_SYONUM_PN)->DP_SNUM;
		}
		else {
			drec1.m_skn.Empty();
		}

		if( m_DI.IsBumon() )	bmcd	=	_getDATA( 0, DP_BMN_PN )->DP_BMN;
		if( m_DI.IsKoji() )	kno		=	_getDATA( 0, DP_KOJI_PN )->DP_KOJI;

		drec1.m_cno	=	denp->DP_DENP;

		setDATE_CREC( &drec1 );

		klen	=	m_DI.GetInputStrLen( (unsigned char*)teki->DP_TKY );
		memmove( tbuf, teki->DP_TKY, klen );
		tbuf[klen] = '\0';
		drec1.m_tekiyo = tbuf;
		drec1.m_icno = m_DPscan.GetSheetICNO();

		if( IsKind() == KIND_NYUKIN ) {
			if( m_DI.IsBumon() ) {
				drec1.m_dbmn = bmcd;
				drec1.m_cbmn = bmcd;
			}
			else {
				drec1.m_dbmn = -1;
				drec1.m_cbmn = -1;
			}
			if( m_DI.IsKoji() ) {
				drec1.m_dkno = kno;
				drec1.m_ckno = kno;
			}
			else {
				drec1.m_dkno.Empty();
				drec1.m_ckno.Empty();
			}
			
			drec1.m_dbt	=	debt->DP_CdNum;
			drec1.m_dbr	=	debt->DP_BRN;
			drec1.m_cre	=	cred->DP_CdNum;
			drec1.m_cbr	=	cred->DP_BRN;
			memmove( drec1.m_val, crval->DP_VAL, sizeof crval->DP_VAL );
			memmove( drec1.m_zei, crzei->DP_VAL, sizeof crzei->DP_VAL );

			memmove( &drec1.m_dsign[0], &czei->SYDATA._DSGN, SZ_DSGN );
			drec1.m_dsign[0]	&=	0x7f;

			if( l_test( drec1.m_val ) == 0 ) {
				ssw	=	-1;
			}
			else {
				m_DPscan.AddSheetTable( ln, 1, &drec1, &czei->SYDATA );
				ssw	=	1;
			}
		}
		else if( IsKind() == KIND_SYUKKIN ) {
			if( m_DI.IsBumon() ) {
				drec1.m_dbmn = bmcd;
				drec1.m_cbmn = bmcd;
			}
			else {
				drec1.m_dbmn = -1;
				drec1.m_cbmn = -1;
			}
			if( m_DI.IsKoji() ) {
				drec1.m_dkno = kno;
				drec1.m_ckno = kno;
			}
			else {
				drec1.m_dkno.Empty();
				drec1.m_ckno.Empty();
			}

			drec1.m_dbt	=	debt->DP_CdNum;
			drec1.m_dbr	=	debt->DP_BRN;
			drec1.m_cre	=	cred->DP_CdNum;
			drec1.m_cbr	=	cred->DP_BRN;

			memmove( drec1.m_val, dbval->DP_VAL, sizeof dbval->DP_VAL );
			memmove( drec1.m_zei, dbzei->DP_VAL, sizeof dbzei->DP_VAL );
			memmove( &drec1.m_dsign[0], &dzei->SYDATA._DSGN, SZ_DSGN );
			drec1.m_dsign[0]	&=	0x7f;

			if( l_test( drec1.m_val ) == 0 ) {
				ssw	=	-1;
			}
			else {
				m_DPscan.AddSheetTable( ln, 0, &drec1, &dzei->SYDATA );
				ssw	=	0;
			}
		}
		else if( IsKind() == KIND_FURIKAE ) {
			drec2	=	drec1;
			ssw	=	-1;

			if( ! m_DI.IsBumon() )
				bmcd = c_bmn = -1;
			if( ! m_DI.IsKoji() ) {
				kno.Empty();
				c_kno.Empty();
			}

			if( m_FuriMode ==  FURI_DATABMON ) {	//仕訳ごとの部門入力
				if( m_DI.IsBumon() ) {
					bmcd	=	_getDATA( ln, FD_DBMN_PN )->DP_BMN;
					c_bmn	=	_getDATA( ln, FD_CBMN_PN )->DP_BMN;
				}
				if( m_DI.IsKoji() ) {
					kno		=	_getDATA( ln, FD_DKOJI_PN )->DP_KOJI;
					c_kno	=	_getDATA( ln, FD_CKOJI_PN )->DP_KOJI;
				}
			}
			else {
				c_bmn = bmcd;
				c_kno = kno;
			}

			if( _getCND( ln, FD_DEBT_PN )->INP_sg && _getCND( ln, FD_CRED_PN )->INP_sg ) {
				if( _getCND( ln, FD_DBVAL_PN )->INP_sg && _getCND( ln, FD_CRVAL_PN )->INP_sg ) {
					if( !l_cmp( dbval->DP_VAL, crval->DP_VAL ) ) {
						drec1.m_dbt	=	debt->DP_CdNum;
						drec1.m_dbr	=	debt->DP_BRN;
						drec1.m_dbmn	= bmcd;
						drec1.m_dkno	= kno;

						drec1.m_cre	=	cred->DP_CdNum;
						drec1.m_cbr	=	cred->DP_BRN;
						drec1.m_cbmn	= c_bmn;
						drec1.m_ckno	= c_kno;

						memmove( drec1.m_val, dbval->DP_VAL, sizeof dbval->DP_VAL );
						memmove( drec1.m_zei, dbzei->DP_VAL, sizeof dbzei->DP_VAL );
						ssw	=	2;

#ifdef LATER
						memmove( &drec1.m_dsign[0], &dzei->SYDATA._DSGN, SZ_DSGN );
						drec1.m_dsign[0]	&=	0x7f;
#endif
					}
					else {
						drec1.m_dbt	=	debt->DP_CdNum;
						drec1.m_dbr	=	debt->DP_BRN;
						drec1.m_dbmn	= bmcd;
						drec1.m_dkno	= kno;

						drec1.m_cre	=	SYOGT;
						drec1.m_cbr	=	-1;
						if( IsFuriSyogBrn() ) {
							temp = _getDATA(ln, FD_DSYOG_PN )->DP_cBRN;
							if( temp > 0 )
								drec1.m_cbr = temp;
						}

						drec1.m_cbmn	= -1;
						drec1.m_ckno.Empty();

						memmove( drec1.m_val, dbval->DP_VAL, sizeof dbval->DP_VAL );
						memmove( drec1.m_zei, dbzei->DP_VAL, sizeof dbval->DP_VAL );
						memmove( &drec1.m_dsign[0], &dzei->SYDATA._DSGN, SZ_DSGN );
						drec1.m_dsign[0]	&=	0x7f;

						drec2.m_dbt	=	SYOGT;
						drec2.m_dbr	=	-1;
						drec2.m_cre	=	cred->DP_CdNum;
						drec2.m_cbr	=	cred->DP_BRN;
						memmove( drec2.m_val, crval->DP_VAL, sizeof crval->DP_VAL );
						memmove( drec2.m_zei, crzei->DP_VAL, sizeof crzei->DP_VAL );
						memmove( &drec2.m_dsign[0], &czei->SYDATA._DSGN, SZ_DSGN );
						drec2.m_dsign[0]	&=	0x7f;

						ssw	=	3;
					}
				}
				else if( _getCND( ln, FD_DBVAL_PN )->INP_sg ) {
					drec1.m_dbt	=	debt->DP_CdNum;
					drec1.m_dbr	=	debt->DP_BRN;
					drec1.m_dbmn	= bmcd;
					drec1.m_dkno	= kno;

					drec1.m_cre	=	SYOGT;
					drec1.m_cbr	=	-1;
					if( IsFuriSyogBrn() ) {
						temp = _getDATA(ln, FD_DSYOG_PN )->DP_cBRN;
						if( temp > 0 )
							drec1.m_cbr = temp;
					}

					drec1.m_cbmn	= -1;
					drec1.m_ckno.Empty();

					memmove( drec1.m_val, dbval->DP_VAL, sizeof dbval->DP_VAL );
					memmove( drec1.m_zei, dbzei->DP_VAL, sizeof dbval->DP_VAL );
					memmove( &drec1.m_dsign[0], &dzei->SYDATA._DSGN, SZ_DSGN );
					drec1.m_dsign[0]	&=	0x7f;

					ssw	=	0;
				}
				else {
					drec1.m_dbt	=	SYOGT;
					drec1.m_dbr	=	-1;
					if( IsFuriSyogBrn() ) {
						temp = _getDATA(ln, FD_CSYOG_PN )->DP_cBRN;
						if( temp > 0 )
							drec1.m_dbr = temp;
					}

					drec1.m_dbmn	= -1;
					drec1.m_dkno.Empty();;

					drec1.m_cre	=	cred->DP_CdNum;
					drec1.m_cbr	=	cred->DP_BRN;
					drec1.m_cbmn	= c_bmn;
					drec1.m_ckno	= c_kno;

					memmove( drec1.m_val, crval->DP_VAL, sizeof crval->DP_VAL );
					memmove( drec1.m_zei, crzei->DP_VAL, sizeof crzei->DP_VAL );
					memmove( &drec1.m_dsign[0], &czei->SYDATA._DSGN, SZ_DSGN );
					drec1.m_dsign[0]	&=	0x7f;

					ssw	=	1;
				}
			}
			else if( _getCND( ln, FD_DEBT_PN )->INP_sg ) {
				if( _getCND( ln, FD_DBVAL_PN )->INP_sg ) {
					drec1.m_dbt	=	debt->DP_CdNum;
					drec1.m_dbr	=	debt->DP_BRN;
					drec1.m_dbmn	= bmcd;
					drec1.m_dkno	= kno;

					drec1.m_cre	=	SYOGT;
					drec1.m_cbr	=	-1;
					if( IsFuriSyogBrn() ) {
						temp = _getDATA(ln, FD_DSYOG_PN )->DP_cBRN;
						if( temp > 0 )
							drec1.m_cbr = temp;
					}

					drec1.m_cbmn	= c_bmn;
					drec1.m_ckno	= c_kno;

					memmove( drec1.m_val, dbval->DP_VAL, sizeof dbval->DP_VAL );
					memmove( drec1.m_zei, dbzei->DP_VAL, sizeof dbval->DP_VAL );
					memmove( &drec1.m_dsign[0], &dzei->SYDATA._DSGN, SZ_DSGN );
					drec1.m_dsign[0]	&=	0x7f;
					ssw	=	0;
				}
			}
			else {
				if( _getCND( ln, FD_CRVAL_PN )->INP_sg ) {
					drec1.m_dbt	=	SYOGT;
					drec1.m_dbr	=	-1;
					if( IsFuriSyogBrn() ) {
						temp = _getDATA(ln, FD_CSYOG_PN )->DP_cBRN;
						if( temp > 0 )
							drec1.m_dbr = temp;
					}

					drec1.m_dbmn	= -1;
					drec1.m_dkno.Empty();;

					drec1.m_cre	=	cred->DP_CdNum;
					drec1.m_cbr	=	cred->DP_BRN;
					drec1.m_cbmn	= c_bmn;
					drec1.m_ckno	= c_kno;

					memmove( drec1.m_val, crval->DP_VAL, sizeof crval->DP_VAL );
					memmove( drec1.m_zei, crzei->DP_VAL, sizeof crzei->DP_VAL );
					memmove( &drec1.m_dsign[0], &czei->SYDATA._DSGN, SZ_DSGN );
					drec1.m_dsign[0]	&=	0x7f;
					ssw	=	1;
				}
			}
			//借方のみ
			if( ssw == 0 ) {
				if( l_test( drec1.m_val ) == 0 ) {
					ssw	=	-1;
				}
				else {
					m_DPscan.AddSheetTable( ln, 0, &drec1, &dzei->SYDATA );
				}
			}
			//貸方のみ
			else if( ssw == 1 ) {
				if( l_test( drec1.m_val ) == 0 ) {
					ssw	=	-1;
				}
				else {
					m_DPscan.AddSheetTable( ln, 1, &drec1, &czei->SYDATA );
				}
			}
			//単一仕訳
			else if( ssw == 2 ) {
				if( l_test( drec1.m_val ) == 0 ) {
					ssw	=	-1;
				}
				else {
					m_DPscan.AddSheetTable( ln, 0, &drec1, &dzei->SYDATA );
					m_DPscan.AddSheetTable( ln, 1, &drec1, &czei->SYDATA );
				}
			}
			//複合仕訳
			else if( ssw == 3 ) {
				if( l_test( drec1.m_val ) == 0 ) {
					m_DPscan.AddSheetTable( ln, 1, &drec2, &czei->SYDATA );
					ssw	=	1;
				}
				else if( l_test( drec2.m_val ) == 0 ) {
					m_DPscan.AddSheetTable( ln, 0, &drec1, &dzei->SYDATA );
					ssw	=	0;
				}
				else {
					m_DPscan.AddSheetTable( ln, 0, &drec1, &dzei->SYDATA );
					m_DPscan.AddSheetTable( ln, 1, &drec2, &czei->SYDATA );
				}
#ifdef OLD_CLOSE	// 09.08 /09
				else {
					m_DPscan.AddSheetTable( ln, 0, &drec1, &dzei->SYDATA );
					ssw	=	0;
				}
#endif
			}
		}
		
		CDBINPDataRec	newrec[4];
		struct _DP_DATA_LINE *datap	=	getLINE_DATA( ln );
		int	cnt	=	0;
		int k;

		if( ssw >= 0 ) {
			cnt	=	sydata_mak( newrec, &drec1, 1, datap );

			for( k = 0; k < cnt; k++ ) {
				*drec = newrec[k];
				drec++;
			}
		}
		if( ssw >= 3 ) {
			int cnt2;
			cnt2 = sydata_mak( newrec, &drec2, 1, datap );
		//	cnt	+=	sydata_mak( newrec, &drec2, 1, datap );
			cnt += cnt2;

			for( k = 0; k < cnt2; k++ ) {
				*drec = newrec[k];
				drec++;
			}
		}

		return	cnt;
	}
	else {
		if( sw == FALSE )	return	0;

		CDBINPDataRec*	dtmp	=	drec;

		int	n	=	da_make( ln, drec );

		return	n;
	}
}


//振替伝票で貸借があった場合の修正データ作成
//	int ln ... データ上の　行番号(1～100)
void CDnpBaseClass::ModifyTaisyakuData( int mode, int ln, int& newcnt, CDBINPDataRec* newrec, int& lcnt, CDBINPDataRec* drec )
{
	CDBINPDataRec	drec1, drec2, tmp1, tmp2;

	drec1	=	m_DPscan.GetOriginData( ln, 0 );
	drec2	=	m_DPscan.GetOriginData( ln, 1 );
	tmp1	=	drec1;
	tmp2	=	drec2;

	struct _DenpData	*debt, *dbval, *dbzei, *cred, *crval, *crzei;
	struct _DenpData	*denp, *teki, *dsyz, *csyz;
	struct _DenpData	*snum, *tgdt, *hjiyu, *husen;
	char	tbuf[128];
	int		klen, idx, dc_zei, temp;
	idx = (ln-1);

	debt	=	_getMAPtoDATA( ln, FD_DEBT_PN );
	dbval	=	_getDATA( ln, FD_DBVAL_PN );
	dbzei	=	_getDATA( ln, FD_DBZEI_PN );
	cred	=	_getMAPtoDATA( ln, FD_CRED_PN );
	crval	=	_getDATA( ln, FD_CRVAL_PN );
	crzei	=	_getDATA( ln, FD_CRZEI_PN );
	teki	=	_getDATA( ln, FD_TKY_PN );
	dsyz	=	_getDATA( ln, FD_DSKBN_PN );
	csyz	=	_getDATA( ln, FD_CSKBN_PN );

	denp	=	_getDATA( 0, DP_DENP_PN );

	snum	=	_getDATA( ln, FD_TGNUM_PN );
	tgdt	=	_getDATA( ln, FD_TGDATE_PN );
	hjiyu	=	_getDATA( ln, FD_HJIYU_PN );
	husen	=	_getDATA( ln, FD_HUSEN_PN );

	int	cnt	=	0;

	int old_nbcd1, old_nbcd2;
	old_nbcd1 = drec1.m_nbcd;
	old_nbcd2 = drec1.m_nbcd;

	//複合仕訳
	if( m_DPscan.IsFukugoSiwake( ln ) ) {
		//借方
		if( (debt == NULL) ||
			(_getCND( ln, FD_DEBT_PN )->INP_sg == FALSE &&
				_getCND( ln, FD_DBVAL_PN )->INP_sg == FALSE ) ) {
			// 取消
			drec1.m_dsign[0] |= 0x01;
			*drec	= drec1;
			lcnt++;
			cnt++;
		}
		else {
			drec1.m_cno	= denp->DP_DENP;

			setDATE_CREC( &drec1 );

			if( m_DI.IsSyonum() ) {
				drec1.m_skn = getDATA(DP_SYONUM_PN)->DP_SNUM;
			}
			else {
				drec1.m_skn.Empty();
			}

			if( m_DI.IsBumon() ) {
 				if( m_FuriMode == FURI_DATABMON )	// 仕訳ごとに部門入力
					drec1.m_dbmn = _getDATA( ln, FD_DBMN_PN )->DP_BMN;
				else
					drec1.m_dbmn = getDATA( DP_BMN_PN )->DP_BMN;
			}
			if( m_DI.IsKoji() ) {
 				if( m_FuriMode == FURI_DATABMON )	// 仕訳ごとに工事入力
					drec1.m_dkno = _getDATA( ln, FD_DKOJI_PN )->DP_KOJI;
				else
					drec1.m_dkno = getDATA( DP_KOJI_PN )->DP_KOJI;
			}

			drec1.m_dbt	=	debt->DP_CdNum;
			drec1.m_dbr	=	debt->DP_BRN;

			if( isSYOGT_CODE( drec1.m_cre )) {
				if( IsFuriSyogBrn() ) {
					temp = _getDATA( ln, FD_DSYOG_PN )->DP_cBRN;
					if( temp > 0 )
						drec1.m_cbr = temp;
					else
						drec1.m_cbr = -1;
				}
			}

			memmove( drec1.m_val, dbval->DP_VAL, sizeof dbval->DP_VAL );
			memmove( drec1.m_zei, dbzei->DP_VAL, sizeof dbzei->DP_VAL );
#ifdef CLOSE // 05.19 /08
			m_DI.SetCDBData( &tmp1 );
			if( m_pSy->IsSyohizei( m_DI.dbdata ) ) {
				memmove( &drec1.m_dsign[0], &dsyz->SYDATA._DSGN, SZ_DSGN );
				memmove( drec1.m_taika, dsyz->SYDATA.TKsyohi, 6 );
			}
#else
			memmove( &drec1.m_dsign[0], &dsyz->SYDATA._DSGN, SZ_SYZDSGN );
			memmove( drec1.m_taika, dsyz->SYDATA.TKsyohi, 6 );
			memmove( drec1.m_invno, dsyz->SYDATA.TKinvno, SZ_INVNO);
#endif
			drec1.m_dtype = 0x30;

			if( !((drec1.m_dsign[0] & 0x80) && tblImage[idx][0]) ) {
				drec1.m_dsign[0]	&=	0x7f;

				klen	=	m_DI.GetInputStrLen( (unsigned char*)teki->DP_TKY );
				memmove( tbuf, teki->DP_TKY, klen );
				tbuf[klen] = '\0';
				drec1.m_tekiyo = tbuf;
			}

			// 手形・変動・付箋 は 入力があった場合のみセット
			// 手形番号
			if( strcmp( tblTgnum[idx][0], (char*)snum->DP_SNUM ) ) { 
				if( tblTgnum[idx][0].IsEmpty() ) {	//借方未入力
					if( strcmp( tblTgnum[idx][1], (char*)snum->DP_SNUM )) { 
						drec1.m_rnumber = snum->DP_SNUM;
					}
				}
				else {
					drec1.m_rnumber = snum->DP_SNUM;
				}
			}
			// 手形期日
			if( tblTgdate[idx][0] != tgdt->DP_TGDAY ) {
				if( tblTgdate[idx][0] <= 0 ) {	//借方未入力
					if( tblTgdate[idx][1] != tgdt->DP_TGDAY ) {
						drec1.m_tdate = tgdt->DP_TGDAY;
					}
				}
				else{
					drec1.m_tdate = tgdt->DP_TGDAY;
				}
			}
			// 変動事由
			if( tblHjiyu[idx][0] != hjiyu->DP_BMN ) {
				if( tblHjiyu[idx][0] <= 0 ) {	//借方未入力
					if( tblHjiyu[idx][1] != hjiyu->DP_BMN ) {
						drec1.m_hjiyu = hjiyu->DP_BMN;
						drec1.m_nbcd  = hjiyu->NAIBCD;
					}
				}
				else{
					drec1.m_hjiyu = hjiyu->DP_BMN;
					drec1.m_nbcd  = hjiyu->NAIBCD;
				}
			}
			else {
				if( old_nbcd1 != hjiyu->NAIBCD )
					drec1.m_nbcd  = hjiyu->NAIBCD;
			}

			// 付箋
			if( tblHusen[idx][0] != husen->DP_HUSEN ) {
				if( tblHusen[idx][0] <= 0 ) {	//借方未入力
					if( tblHusen[idx][1] != husen->DP_HUSEN ) {
						drec1.m_dsign[7] &= (~0x0f);
						drec1.m_dsign[7] |= husen->DP_HUSEN;
					}
				}
				else{
					drec1.m_dsign[7] &= (~0x0f);
					drec1.m_dsign[7] |= husen->DP_HUSEN;
				}
			}
			// 付箋コメント
			if( strcmp( tblHcmnt[idx][0], (char*)husen->DP_HCMNT ) ) {
				if( tblHcmnt[idx][0].IsEmpty() ) {	//借方未入力
					if( strcmp( tblHcmnt[idx][1], (char*)husen->DP_HCMNT ) ) {
						drec1.m_tag = husen->DP_HCMNT;
					}
				}
				else{
					drec1.m_tag = husen->DP_HCMNT;
				}
			}

			drec1.m_dsign[0]	&=	0xfe;
			*drec = drec1;
			lcnt++;
			cnt++;
		}
		//貸方
		if( (cred == NULL) || 
			(_getCND( ln, FD_CRED_PN )->INP_sg == FALSE &&
				_getCND( ln, FD_CRVAL_PN )->INP_sg == FALSE) ) {
			drec2.m_dsign[0]	|=	0x01;
			*(drec + cnt)	=	drec2;
			lcnt++;
		}
		else {
			drec2.m_cno	=	denp->DP_DENP;
			setDATE_CREC( &drec2 );

			if( m_DI.IsSyonum() ) {
				drec2.m_skn = getDATA(DP_SYONUM_PN)->DP_SNUM;
			}
			else {
				drec2.m_skn.Empty();
			}

			if( m_DI.IsBumon() ) {
 				if( m_FuriMode == FURI_DATABMON )	// 仕訳ごとに部門入力
					drec2.m_cbmn = _getDATA( ln, FD_CBMN_PN )->DP_BMN;
				else
					drec2.m_cbmn = getDATA( DP_BMN_PN )->DP_BMN;
			}
			if( m_DI.IsKoji() ) {
 				if( m_FuriMode == FURI_DATABMON )	// 仕訳ごとに工事入力
					drec2.m_ckno = _getDATA( ln, FD_CKOJI_PN )->DP_KOJI;
				else
					drec2.m_ckno = getDATA( DP_KOJI_PN )->DP_KOJI;
			}

			drec2.m_cre	=	cred->DP_CdNum;
			drec2.m_cbr	=	cred->DP_BRN;

			if( isSYOGT_CODE( drec2.m_dbt )) {
				if( IsFuriSyogBrn() ) {
					temp = _getDATA( ln, FD_CSYOG_PN )->DP_cBRN;
					if( temp > 0 )
						drec2.m_dbr = temp;
					else
						drec2.m_dbr = -1;
				}
			}

			memmove( drec2.m_val, crval->DP_VAL, sizeof crval->DP_VAL );
			memmove( drec2.m_zei, crzei->DP_VAL, sizeof crzei->DP_VAL );

//			if( !Sy.issydata( &tmp1 ) && Sy.issydata( &tmp2 ) ) {
#ifdef CLOSE // 05.19 /08
			m_DI.SetCDBData( &tmp2 );
			if( m_pSy->IsSyohizei( m_DI.dbdata ) ) {
				memmove( &drec2.m_dsign[0], &csyz->SYDATA._DSGN, SZ_DSGN );
				memmove( drec2.m_taika, csyz->SYDATA.TKsyohi, 6 );
			}
#else
			memmove( &drec2.m_dsign[0], &csyz->SYDATA._DSGN, SZ_SYZDSGN );
			memmove( drec2.m_taika, csyz->SYDATA.TKsyohi, 6 );
			memmove( drec2.m_invno, csyz->SYDATA.TKinvno, SZ_INVNO);
#endif
			drec2.m_dtype	=	0x30;

			int	sw	=	0;
			if( drec2.m_dsign[0] & 0x80 ) {
				if( tblImageOfst[idx][0] == tblImageOfst[idx][1] ) {
					if( tblImage[idx][0] )	sw	=	0;
					else					sw	=	1;
				}
				else if( !tblImage[idx][1] ) {
//					if( !tblTekiyo[idx].IsEmpty() )	sw	=	2;
//					else							sw	=	1;
					if( tblImage[idx][0] )	sw	=	3;
					else					sw	=	1;
				}
				else	sw	=	0;
			}
			else if( !tblTekiyo[idx].IsEmpty() ) {
				sw	=	2;
			}
			else	sw	=	1;

			if( sw == 2 ) {
				drec2.m_dsign[0]	&=	0x7f;

				klen	=	m_DI.GetInputStrLen( (unsigned char*)(LPCTSTR)tblTekiyo[idx] );
				strncpy_s( tbuf, sizeof tbuf, tblTekiyo[idx], klen );
				tbuf[klen] = '\0';

				drec2.m_tekiyo = tbuf;
			}
			else if( sw == 1 ) {
				drec2.m_dsign[0]	&=	0x7f;

				klen	=	m_DI.GetInputStrLen( (unsigned char*)teki->DP_TKY );
				memmove( tbuf,teki->DP_TKY, klen );
				tbuf[klen] = '\0';
				drec2.m_tekiyo = tbuf;
			}
			else if( sw == 3 ) {
				drec2.m_tekiyo = drec1.m_tekiyo;
			}

			// 手形・変動・付箋 は 入力があった場合のみセット
			// 手形番号
			if( strcmp( tblTgnum[idx][1], (char*)snum->DP_SNUM ) ) { 
				if( tblTgnum[idx][1].IsEmpty() ) {	//貸方未入力
					if( strcmp( tblTgnum[idx][0], (char*)snum->DP_SNUM )) { 
						drec2.m_rnumber = snum->DP_SNUM;
					}
				}
				else {
					drec2.m_rnumber = snum->DP_SNUM;
				}
			}
			// 手形期日
			if( tblTgdate[idx][1] != tgdt->DP_TGDAY ) {
				if( tblTgdate[idx][1] <= 0 ) {	//貸方未入力
					if( tblTgdate[idx][0] != tgdt->DP_TGDAY ) {
						drec2.m_tdate = tgdt->DP_TGDAY;
					}
				}
				else{
					drec2.m_tdate = tgdt->DP_TGDAY;
				}
			}
			// 変動事由
			if( tblHjiyu[idx][1] != hjiyu->DP_BMN ) {
				if( tblHjiyu[idx][1] <= 0 ) {	//貸方未入力
					if( tblHjiyu[idx][0] != hjiyu->DP_BMN ) {
						drec2.m_hjiyu = hjiyu->DP_BMN;
						drec2.m_nbcd  = hjiyu->NAIBCD;
					}
				}
				else{
					drec2.m_hjiyu = hjiyu->DP_BMN;
					drec2.m_nbcd  = hjiyu->NAIBCD;
				}
			}
			else {
				if( old_nbcd2 != hjiyu->NAIBCD )
					drec2.m_nbcd  = hjiyu->NAIBCD;
			}
			// 付箋
			if( tblHusen[idx][1] != husen->DP_HUSEN ) {
				if( tblHusen[idx][1] <= 0 ) {	//貸方未入力
					if( tblHusen[idx][0] != husen->DP_HUSEN ) {
						drec2.m_dsign[7] &= (~0x0f);
						drec2.m_dsign[7] |= husen->DP_HUSEN;
					}
				}
				else{
					drec2.m_dsign[7] &= (~0x0f);
					drec2.m_dsign[7] |= husen->DP_HUSEN;
				}
			}
			// 付箋コメント
			if( strcmp( tblHcmnt[idx][1], (char*)husen->DP_HCMNT ) ) {
				if( tblHcmnt[idx][1].IsEmpty() ) {	//貸方未入力
					if( strcmp( tblHcmnt[idx][0], (char*)husen->DP_HCMNT ) ) {
						drec2.m_tag = husen->DP_HCMNT;
					}
				}
				else{
					drec2.m_tag = husen->DP_HCMNT;
				}
			}

			drec2.m_dsign[0]	&=	0xfe;
			*(drec + cnt) = drec2;
			lcnt++;
		}
	}
	//単一仕訳
	else {
		if( m_DPscan.DeleteLine( ln ) ) {
			drec1.m_dsign[0]	|=	0x01;
			*drec = drec1;
			lcnt++;
		}
		else if( debt == NULL && cred == NULL ) {
			drec1.m_dsign[0]	|=	0x01;
			*drec = drec1;
			lcnt++;
		}
		else {
			drec1.m_cno	=	denp->DP_DENP;
			setDATE_CREC( &drec1 );

			if( m_DI.IsSyonum() ) {
				drec1.m_skn = getDATA(DP_SYONUM_PN)->DP_SNUM;
			}
			else {
				drec1.m_skn.Empty();
			}

			if( m_DI.IsBumon() ) {
				if( m_FuriMode == FURI_DATABMON ) {	// 仕訳ごとに部門入力
					drec1.m_dbmn = _getDATA( ln, FD_DBMN_PN )->DP_BMN;
					drec1.m_cbmn = _getDATA( ln, FD_CBMN_PN )->DP_BMN;
				}
				else {
					drec1.m_dbmn = getDATA( DP_BMN_PN )->DP_BMN;
					drec1.m_cbmn = getDATA( DP_BMN_PN )->DP_BMN;
				}
			}
			if( m_DI.IsKoji() ) {
				if( m_FuriMode == FURI_DATABMON ) {	// 仕訳ごとに工事入力
					drec1.m_dkno = _getDATA( ln, FD_DKOJI_PN )->DP_KOJI;
					drec1.m_ckno = _getDATA( ln, FD_CKOJI_PN )->DP_KOJI;
				}
				else {
					drec1.m_dkno = getDATA( DP_KOJI_PN )->DP_KOJI;
					drec1.m_ckno = getDATA( DP_KOJI_PN )->DP_KOJI;
				}
			}
			
			if( debt ) {
				drec1.m_dbt	=	debt->DP_CdNum;
				drec1.m_dbr	=	debt->DP_BRN;
			}
			else {
				drec1.m_dbt	=	SYOGT;
				drec1.m_dbr	=	-1;
			}

			if( cred ) {
				drec1.m_cre	=	cred->DP_CdNum;
				drec1.m_cbr	=	cred->DP_BRN;
			}
			else {
				drec1.m_cre	=	SYOGT;
				drec1.m_cbr	=	-1;
			}
			
			dc_zei = 0;
			if( m_DI.km_syattr( drec1.m_dbt ) )
				dc_zei |= 0x01;
			if( m_DI.km_syattr( drec1.m_cre ) )
				dc_zei |= 0x02;

			if( dc_zei == 0x01 ) {	//借方が消費税科目
				memmove( drec1.m_val, dbval->DP_VAL, sizeof dbval->DP_VAL );
				memmove( drec1.m_zei, dbzei->DP_VAL, sizeof dbzei->DP_VAL );
				memmove( &drec1.m_dsign[0], &dsyz->SYDATA._DSGN, SZ_SYZDSGN );
				memmove( &drec1.m_taika, dsyz->SYDATA.TKsyohi, 6 );
				memmove( drec1.m_invno, dsyz->SYDATA.TKinvno, SZ_INVNO);
			}
			else {	//貸方が消費税科目か、借方／貸方ともに消費税科目
				memmove( drec1.m_val, crval->DP_VAL, sizeof crval->DP_VAL );
				memmove( drec1.m_zei, crzei->DP_VAL, sizeof crzei->DP_VAL );
				memmove( &drec1.m_dsign[0], &csyz->SYDATA._DSGN, SZ_SYZDSGN );
				memmove( &drec1.m_taika, csyz->SYDATA.TKsyohi, 6 );
				memmove( drec1.m_invno, csyz->SYDATA.TKinvno, SZ_INVNO);
			}
			// 摘要
			if( !((drec1.m_dsign[0] & 0x80) && tblImage[idx][0]) ) {
				drec1.m_dsign[0]	&=	0x7f;

				klen	=	m_DI.GetInputStrLen( (unsigned char*)teki->DP_TKY );
				memmove( tbuf, teki->DP_TKY, klen );
				tbuf[klen] = '\0';
				drec1.m_tekiyo	=	tbuf;
			}

			// 手形番号
			drec1.m_rnumber = snum->DP_SNUM;
			// 手形期日
			drec1.m_tdate = tgdt->DP_TGDAY;
			// 変動事由
			drec1.m_hjiyu = hjiyu->DP_BMN;
			drec1.m_nbcd  = hjiyu->NAIBCD;
			// 付箋
			drec1.m_dsign[7] &= (~0x0f);
			drec1.m_dsign[7] |= husen->DP_HUSEN;
			// 付箋コメント
			drec1.m_tag = husen->DP_HCMNT;

			if( drec1.m_dbt == SYOGT || drec1.m_cre == SYOGT ) {
				drec1.m_dtype	=	0;
			}
			else {
				drec1.m_dtype	=	0x30;
			}

			drec1.m_dsign[0]	&=	0xfe;
			*drec	=	drec1;
			lcnt++;
		}
	}
}


//振替伝票で借方のみにデータがあった時及び出金伝票
//	int ln ... データ上の　行番号(1～100)
void CDnpBaseClass::ModifyDebtData( int mode, int ln, int& newcnt, CDBINPDataRec* newrec, int& lcnt, CDBINPDataRec* drec )
{
	CDBINPDataRec	drec1;
	drec1	=	m_DPscan.GetOriginData( ln, 0 );

	struct _DenpData	*debt, *dbval, *dbzei, *cred, *crval, *crzei;
	struct _DenpData	*denp, *teki, *dsyz, *csyz;
	struct _DenpData	*snum, *tgdt, *hjiyu, *husen;
	char	tbuf[128];
	int		klen, bmcd, idx, temp;
	CString kno;
	idx = (ln-1);

	debt	=	_getMAPtoDATA( ln, FD_DEBT_PN );
	dbval	=	_getDATA( ln, FD_DBVAL_PN );
	dbzei	=	_getDATA( ln, FD_DBZEI_PN );

	cred	=	_getMAPtoDATA( ln, FD_CRED_PN );
	crval	=	_getDATA( ln, FD_CRVAL_PN );
	crzei	=	_getDATA( ln, FD_CRZEI_PN );

	teki	=	_getDATA( ln, FD_TKY_PN );
	dsyz	=	_getDATA( ln, FD_DSKBN_PN );

	denp	=	_getDATA( 0, DP_DENP_PN );

	snum	=	_getDATA( ln, FD_TGNUM_PN );
	tgdt	=	_getDATA( ln, FD_TGDATE_PN );
	hjiyu	=	_getDATA( ln, FD_HJIYU_PN );
	husen	=	_getDATA( ln, FD_HUSEN_PN );

	bmcd = -1;
	if( IsKind() != KIND_FURIKAE ) {
		bmcd = _getDATA(-1, DP_BMN_PN)->DP_BMN;;

		if( m_DI.IsKoji() )
			kno = getDATA( DP_KOJI_PN )->DP_KOJI;
	}
	
	//借方
	if(	! debt || (_getCND( ln, FD_DEBT_PN )->INP_sg == FALSE &&
						_getCND( ln, FD_DBVAL_PN )->INP_sg == FALSE) ) {
		drec1.m_dsign[0]	|=	0x01;
		*drec = drec1;
		lcnt++;
	}
	else if( m_DPscan.DeleteLine( ln ) ) {
		drec1.m_dsign[0]	|=	0x01;
		*drec = drec1;
		lcnt++;
	}
	else {
		drec1.m_cno	=	denp->DP_DENP;
		setDATE_CREC( &drec1 );
		if( m_DI.IsSyonum() ) {
			drec1.m_skn = getDATA(DP_SYONUM_PN)->DP_SNUM;
		}
		else {
			drec1.m_skn.Empty();
		}

		if( m_DI.IsBumon() ) {
			if( IsKind() == KIND_FURIKAE ) {
				if( m_FuriMode == FURI_DATABMON )	// 仕訳ごとに部門入力
					drec1.m_dbmn = _getDATA( ln, FD_DBMN_PN )->DP_BMN;
				else
					drec1.m_dbmn = getDATA( DP_BMN_PN )->DP_BMN;
			}
			else {
				drec1.m_dbmn = bmcd;
				drec1.m_cbmn = bmcd;
			}
		}
		if( m_DI.IsKoji() ) {
			if( IsKind() == KIND_FURIKAE ) {
				if( m_FuriMode == FURI_DATABMON )	// 仕訳ごとに工事入力
					drec1.m_dkno = _getDATA( ln, FD_DKOJI_PN )->DP_KOJI;
				else
					drec1.m_dkno = getDATA( DP_KOJI_PN )->DP_KOJI;
			}
			else {
				drec1.m_dkno = kno;
				drec1.m_ckno = kno;
			}
		}

		drec1.m_dbt		=	debt->DP_CdNum;
		drec1.m_dbr		=	debt->DP_BRN;

		if( isSYOGT_CODE( drec1.m_cre )) {
			if( IsFuriSyogBrn() ) {
				temp = _getDATA( ln, FD_DSYOG_PN )->DP_cBRN;
				if( temp > 0 )
					drec1.m_cbr = temp;
				else
					drec1.m_cbr = -1;
			}
		}

		memmove( drec1.m_val, dbval->DP_VAL, sizeof dbval->DP_VAL );
		memmove( drec1.m_zei, dbzei->DP_VAL, sizeof dbzei->DP_VAL );

		memmove( &drec1.m_dsign[0], &dsyz->SYDATA._DSGN, SZ_SYZDSGN );
		memmove( drec1.m_taika, dsyz->SYDATA.TKsyohi, 6 );
		memmove( drec1.m_invno, dsyz->SYDATA.TKinvno, SZ_INVNO);

		if( IsKind() == KIND_FURIKAE ) {
			drec1.m_dtype	=	0x30;
		}
		else {
			drec1.m_dtype	=	0x20;
		}

		if( !((drec1.m_dsign[0] & 0x80) && tblImage[idx][0]) ) {
			drec1.m_dsign[0]	&=	0x7f;

			klen	=	m_DI.GetInputStrLen( (unsigned char*)teki->DP_TKY );
			memmove( tbuf, teki->DP_TKY, klen );
			tbuf[klen] = '\0';

			drec1.m_tekiyo = tbuf;
		}
		// 手形番号
		drec1.m_rnumber = snum->DP_SNUM;
		// 手形期日
		drec1.m_tdate = tgdt->DP_TGDAY;
		// 変動事由
		drec1.m_hjiyu = hjiyu->DP_BMN;
		drec1.m_nbcd  = hjiyu->NAIBCD;
		// 付箋
		drec1.m_dsign[7] &= (~0x0f);
		drec1.m_dsign[7] |= husen->DP_HUSEN;
		// 付箋コメント
		drec1.m_tag = husen->DP_HCMNT;

		drec1.m_dsign[0]	&=	0xfe;
		*drec	= drec1;
		lcnt++;
	}

	//振替伝票で貸方に新規 入力された時
	if( IsKind() == KIND_FURIKAE && !IsScanLine( ln ) && cred ) {
		CDBINPDataRec	dtemp;
		dtemp	=	m_DPscan.GetOriginData( ln, 0 );

		drec1.m_cno	=	denp->DP_DENP;
		setDATE_CREC( &drec1 );

		if( m_DI.IsSyonum() ) {
			drec1.m_skn = getDATA(DP_SYONUM_PN)->DP_SNUM;
		}
		else {
			drec1.m_skn.Empty();
		}

		if( m_DI.IsBumon() ) {
			if( m_FuriMode == FURI_DATABMON )	// 仕訳ごとに部門入力
				drec1.m_cbmn	=	_getDATA(ln, FD_CBMN_PN)->DP_BMN;
			else
				drec1.m_cbmn	=	getDATA(DP_BMN_PN)->DP_BMN;
		}
		if( m_DI.IsKoji() ) {
			if( m_FuriMode == FURI_DATABMON )	// 仕訳ごとに工事入力
				drec1.m_ckno	=	_getDATA(ln, FD_CKOJI_PN)->DP_KOJI;
			else
				drec1.m_ckno	=	getDATA( DP_KOJI_PN)->DP_KOJI;
		}

		drec1.m_dbt	=	SYOGT;
		drec1.m_dbr	=	-1;
		if( IsFuriSyogBrn() ) {
			temp = _getDATA( ln, FD_CSYOG_PN )->DP_cBRN;
			if( temp > 0 ) {
				drec1.m_dbr = temp;
			}
		}

		memmove( drec1.m_val, crval->DP_VAL, sizeof crval->DP_VAL );
		memmove( drec1.m_zei, crzei->DP_VAL, sizeof crzei->DP_VAL );

		drec1.m_cre	=	cred->DP_CdNum;
		drec1.m_cbr	=	cred->DP_BRN;
		csyz = _getDATA( ln, FD_CSKBN_PN );
		memmove( &drec1.m_dsign[0], &csyz->SYDATA._DSGN, SZ_DSGN );
		memmove( &drec1.m_taika, csyz->SYDATA.TKsyohi, 6 );
		memmove( drec1.m_invno, csyz->SYDATA.TKinvno, SZ_INVNO);

		//元の借方仕訳がイメージ摘要の場合、追加仕訳の摘要をイメージにする
		if( dtemp.m_dsign[0] & 0x80 ) {
			drec1.m_dsign[0]	|=	0x80;
			drec1.m_imgsq	=	dtemp.m_imgsq;
		}
		else {
			drec1.m_dsign[0]	&=	0x7f;
			drec1.m_imgsq	=	-1;

			klen	=	m_DI.GetInputStrLen( (unsigned char*)teki->DP_TKY );
			memmove( tbuf, teki->DP_TKY, klen );
			tbuf[klen] = '\0';
			drec1.m_tekiyo	=	tbuf;
		}
		// 手形番号
		drec1.m_rnumber = snum->DP_SNUM;
		// 手形期日
		drec1.m_tdate = tgdt->DP_TGDAY;
		// 変動事由
		drec1.m_hjiyu = hjiyu->DP_BMN;
		drec1.m_nbcd  = hjiyu->NAIBCD;
		// 付箋
		drec1.m_dsign[7] &= (~0x0f);
		drec1.m_dsign[7] |= husen->DP_HUSEN;
		// 付箋コメント
		drec1.m_tag = husen->DP_HCMNT;

		drec1.m_dsign[0]	&=	0xfe;
		struct _DP_DATA_LINE *datap	=	getLINE_DATA( ln );
//		newcnt	+=	sydata_mak( newrec, &drec1, 1, datap );
		int	cnt	=	sydata_mak( newrec, &drec1, 1, datap );

		for( int i = 0; i < cnt; i++ ) {
			(newrec + i)->m_dtype	=	0x30;
		}

		newcnt	+=	cnt;
	}

}

//	int ln ... データ上の　行番号(1～100)
void CDnpBaseClass::ModifyCredData( int mode, int ln, int& newcnt, CDBINPDataRec* newrec, int& lcnt, CDBINPDataRec* drec )
{
	CDBINPDataRec	drec1;
	drec1	=	m_DPscan.GetOriginData( ln, 1 );

	struct _DenpData	*debt, *dbval, *dbzei, *cred, *crval, *crzei;
	struct _DenpData	*denp, *teki, *dsyz, *csyz;
	struct _DenpData	*snum, *tgdt, *hjiyu, *husen;
	char	tbuf[128];
	int		klen, bmcd, idx, temp;
	CString kno;
	idx = (ln-1);

	debt	=	_getMAPtoDATA( ln, FD_DEBT_PN );
	dbval	=	_getDATA( ln, FD_DBVAL_PN );
	dbzei	=	_getDATA( ln, FD_DBZEI_PN );
	cred	=	_getMAPtoDATA( ln, FD_CRED_PN );
	crval	=	_getDATA( ln, FD_CRVAL_PN );
	crzei	=	_getDATA( ln, FD_CRZEI_PN );
	teki	=	_getDATA( ln, FD_TKY_PN );
	dsyz	=	_getDATA( ln, FD_DSKBN_PN );
	csyz	=	_getDATA( ln, FD_CSKBN_PN );

	denp	=	_getDATA( 0, DP_DENP_PN );

	snum	=	_getDATA( ln, FD_TGNUM_PN );
	tgdt	=	_getDATA( ln, FD_TGDATE_PN );
	hjiyu	=	_getDATA( ln, FD_HJIYU_PN );
	husen	=	_getDATA( ln, FD_HUSEN_PN );

	if( m_DI.IsBumon() )	bmcd	=	_getDATA( 0, DP_BMN_PN )->DP_BMN;
	if( m_DI.IsKoji() )	kno	=	_getDATA( 0, DP_KOJI_PN )->DP_KOJI;

	//貸方
	if( ! cred || ( _getCND( ln, FD_CRED_PN )->INP_sg == FALSE &&
						_getCND( ln, FD_CRVAL_PN )->INP_sg == FALSE) ) {
		drec1.m_dsign[0]	|=	0x01;
		*drec	=	drec1;
		lcnt++;
	}
	else if( m_DPscan.DeleteLine( ln ) ) {
		drec1.m_dsign[0]	|=	0x01;
		*drec	=	drec1;
		lcnt++;
	}
	else {
		drec1.m_cno	=	denp->DP_DENP;

		setDATE_CREC( &drec1 );
		if( m_DI.IsSyonum() ) {
			drec1.m_skn = getDATA(DP_SYONUM_PN)->DP_SNUM;
		}
		else {
			drec1.m_skn.Empty();
		}

		if( m_DI.IsBumon() ) {
			if( IsKind() == KIND_FURIKAE ) {
				if( m_FuriMode == FURI_DATABMON )	// 仕訳ごとに部門入力
					drec1.m_cbmn = _getDATA( ln, FD_CBMN_PN)->DP_BMN;
				else
					drec1.m_cbmn = getDATA( DP_BMN_PN)->DP_BMN;
			}
			else {
				drec1.m_dbmn = bmcd;
				drec1.m_cbmn = bmcd;
			}
		}
		if( m_DI.IsKoji() ) {
			if( IsKind() == KIND_FURIKAE ) {
				if( m_FuriMode == FURI_DATABMON )	// 仕訳ごとに工事入力
					drec1.m_ckno = _getDATA( ln, FD_CKOJI_PN)->DP_KOJI;
				else
					drec1.m_ckno = getDATA( DP_KOJI_PN)->DP_KOJI;
			}
			else {
				drec1.m_dkno = kno;
				drec1.m_ckno = kno;
			}
		}

		drec1.m_cre	=	cred->DP_CdNum;
		drec1.m_cbr	=	cred->DP_BRN;

		if( isSYOGT_CODE( drec1.m_dbt )) {
			if( IsFuriSyogBrn() ) {
				temp = _getDATA( ln, FD_CSYOG_PN )->DP_cBRN;
				if( temp > 0 )
					drec1.m_dbr = temp;
				else
					drec1.m_dbr = -1;
			}
		}

		memmove( drec1.m_val, crval->DP_VAL, sizeof crval->DP_VAL );
		memmove( drec1.m_zei, crzei->DP_VAL, sizeof crzei->DP_VAL );

		memmove( &drec1.m_dsign[0], &csyz->SYDATA._DSGN, SZ_SYZDSGN );
		memmove( &drec1.m_taika, csyz->SYDATA.TKsyohi, 6 );
		memmove( drec1.m_invno, csyz->SYDATA.TKinvno, SZ_INVNO);

		if( IsKind() == KIND_FURIKAE ) {
			drec1.m_dtype	=	0x30;
		}
		else {
			drec1.m_dtype	=	0x10;
		}
		//貸方のみに仕訳があった場合は、イメージは借方の方へセットしてある
		if( !((drec1.m_dsign[0] & 0x80) && tblImage[idx][0]) ) {
			drec1.m_dsign[0]	&=	0x7f;

			klen	=	m_DI.GetInputStrLen( (unsigned char*)teki->DP_TKY );
			memmove( tbuf, teki->DP_TKY, klen );
			tbuf[klen] = '\0';
			drec1.m_tekiyo	=	tbuf;
		}
		// 手形番号
		drec1.m_rnumber = snum->DP_SNUM;
		// 手形期日
		drec1.m_tdate = tgdt->DP_TGDAY;
		// 変動事由
		drec1.m_hjiyu = hjiyu->DP_BMN;
		drec1.m_nbcd  = hjiyu->NAIBCD;
		// 付箋
		drec1.m_dsign[7] &= (~0x0f);
		drec1.m_dsign[7] |= husen->DP_HUSEN;
		// 付箋コメント
		drec1.m_tag = husen->DP_HCMNT;

		drec1.m_dsign[0]	&=	0xfe;
		*drec	=	drec1;
		lcnt++;
	}
	//振替伝票で借方に入力された時
	if( IsKind() == KIND_FURIKAE && !IsScanLine( ln ) && debt ) {
		CDBINPDataRec	dtemp;
		dtemp	=	m_DPscan.GetOriginData( ln, 1 );

		drec1.m_cno	=	denp->DP_DENP;
		setDATE_CREC(&drec1);

		if( m_DI.IsSyonum() ) {
			drec1.m_skn = getDATA(DP_SYONUM_PN)->DP_SNUM;
		}
		else {
			drec1.m_skn.Empty();
		}

		if( m_DI.IsBumon() ) {
			if( m_FuriMode == FURI_DATABMON )	// 仕訳ごとに部門入力
				drec1.m_dbmn = _getDATA( ln, FD_DBMN_PN)->DP_BMN;
			else
				drec1.m_dbmn = getDATA(DP_BMN_PN)->DP_BMN;
		}
		if( m_DI.IsKoji() ) {
			if( m_FuriMode == FURI_DATABMON )	// 仕訳ごとに工事入力
				drec1.m_dkno = _getDATA( ln, FD_DKOJI_PN)->DP_KOJI;
			else
				drec1.m_dkno = getDATA(DP_KOJI_PN)->DP_KOJI;
		}

		drec1.m_cre	=	SYOGT;
		drec1.m_cbr	=	-1;
		if( IsFuriSyogBrn() ) {
			temp = _getDATA( ln, FD_DSYOG_PN )->DP_cBRN;
			if( temp > 0 )
				drec1.m_cbr = temp;
		}

		memmove( drec1.m_val, dbval->DP_VAL, sizeof dbval->DP_VAL );
		memmove( drec1.m_zei, dbzei->DP_VAL, sizeof dbzei->DP_VAL );

		drec1.m_dbt	=	debt->DP_CdNum;
		drec1.m_dbr	=	debt->DP_BRN;

		memmove( &drec1.m_dsign[0], &dsyz->SYDATA._DSGN, SZ_DSGN );
		memmove( &drec1.m_taika, dsyz->SYDATA.TKsyohi, 6 );
		memmove( drec1.m_invno, dsyz->SYDATA.TKinvno, SZ_INVNO);

		//元の貸方仕訳がイメージ摘要の場合、追加仕訳の摘要をイメージにする
		if( dtemp.m_dsign[0] & 0x80 ) {
			drec1.m_dsign[0]	|=	0x80;
			drec1.m_imgsq	=	dtemp.m_imgsq;
		}
		else {
			drec1.m_dsign[0]	&=	0x7f;

			klen	=	m_DI.GetInputStrLen( (unsigned char*)teki->DP_TKY );
			memmove( tbuf, teki->DP_TKY, klen );
			tbuf[klen] = '\0';
			drec1.m_tekiyo	=	tbuf;
		}
		// 手形番号
		drec1.m_rnumber = snum->DP_SNUM;
		// 手形期日
		drec1.m_tdate = tgdt->DP_TGDAY;
		// 変動事由
		drec1.m_hjiyu = hjiyu->DP_BMN;
		drec1.m_nbcd  = hjiyu->NAIBCD;
		// 付箋
		drec1.m_dsign[7] &= (~0x0f);
		drec1.m_dsign[7] |= husen->DP_HUSEN;
		// 付箋コメント
		drec1.m_tag = husen->DP_HCMNT;

		drec1.m_dsign[0]	&=	0xfe;
		struct _DP_DATA_LINE *datap	=	getLINE_DATA( ln );
//		newcnt	+=	sydata_mak( newrec, &drec1, 1, datap );
		int	cnt	=	sydata_mak( newrec, &drec1, 1, datap );
		for( int i = 0; i < cnt; i++ ) {
			(newrec + i)->m_dtype	=	0x30;
		}
		newcnt	+=	cnt;
	}

}



//仕訳修正更新
int CDnpBaseClass::ModifyData( CDBINPDataRec* drec, int cnt )
{
	int				ln, j, ret;
	CDBINPDataRec	data, *pNew;

	for( ln = 1, j = 0; ln <= get_MaxLine(); ln++ ) {
		if( m_DPscan.IsOriginData( ln, 0 ) ) {
			data	=	m_DPscan.GetOriginData( ln, 0 );
			pNew	=	drec + j;
			j++;

			//インボイス登録番号対象か？
			m_DI.is_invnosiwake(pNew);

			if( m_DI.isdiff_data( pNew, &data ) ) {

				ret = m_DI.DB_DataCorrect( pNew );
				if( ret == ERR )
					return -1;
				else if( ret == -2 ) {
					m_PRO_OK = FALSE;
					
					int st = myICSMessageBox( (LPCTSTR)m_DI.m_pZm->GetZmErrMsg() );
				
					m_PRO_OK = TRUE;
					m_DI.GetCDBData( pNew );
				}

				if( ret != -2 ) {
					m_pDENPPAR->mdfseq.push_back( pNew->m_seq );
				//	MODEFY_BIT.onbitn( pNew->m_seq - 1);
					if( m_DI.isSyzdiff_data( pNew, &data ) ) {
						m_pDENPPAR->syzseq.push_back( pNew->m_seq );
				//		SYZMDFY_BIT.onbitn( pNew->m_seq - 1);
					}
				}

				// 02.20 /08
				m_DPscan.SetOriginData( ln, 0, *pNew );

#ifdef LATER_CLOSE
				//諸口・現金貸借
				if( pNew->m_dsign[0] & 0x01 ){
					syog_genk( pNew, _SUB );
				}
				else{
					syog_genk( &data, _SUB );
					syog_genk( pNew, _ADD );
				}
#endif
			}
		}

		if( m_DPscan.IsOriginData( ln, 1 ) ) {
			BOOL bCheck = FALSE;

			if( IsKind() == KIND_FURIKAE ) {
				if( SyogSiwakeMode() ) {	//諸口仕訳詰めない
					if( m_DPscan.IsOriginData( ln, 0 ) ) {
						int d_seq = m_DPscan.GetSeq( ln, 0 );
						int c_seq = m_DPscan.GetSeq( ln, 1 );

						if( d_seq != c_seq ) {	//単独仕訳でない
							bCheck = TRUE;
						}
					}
					else {
						bCheck = TRUE;
					}
				}
				else {
					if( m_DPscan.IsFukugoSiwake( ln ) ) {	//単独仕訳は借方で処理している
						bCheck = TRUE;
					}
				}
			}
			else {
				bCheck = TRUE;
			}

		//	if( !(IsKind() == KIND_FURIKAE && !m_DPscan.IsFukugoSiwake( ln )) ) {
			if( bCheck ) {
				data	=	m_DPscan.GetOriginData( ln, 1 );
				pNew	=	drec + j;
				j++;

				//インボイス登録番号対象か？
				m_DI.is_invnosiwake(pNew);

				if( m_DI.isdiff_data( pNew, &data ) ) {

					ret = m_DI.DB_DataCorrect( pNew );
					if( ret == ERR )
						return -1;
					else if( ret == -2 ) {
						m_PRO_OK = FALSE;
						
						int st = myICSMessageBox( (LPCTSTR)m_DI.m_pZm->GetZmErrMsg() );
					
						m_PRO_OK = TRUE;
						m_DI.GetCDBData( pNew );
					}

					if( ret != -2 ) {
						m_pDENPPAR->mdfseq.push_back( pNew->m_seq );
					//	MODEFY_BIT.onbitn( pNew->m_seq - 1);
						if( m_DI.isSyzdiff_data( pNew, &data ) ) {
							m_pDENPPAR->syzseq.push_back( pNew->m_seq );
					//		SYZMDFY_BIT.onbitn( pNew->m_seq - 1);
						}
					}
					// 02.20 /08
					m_DPscan.SetOriginData( ln, 1, *pNew );

#ifdef LATER_CLOSE
					//諸口・現金貸借
					if( pNew->m_dsign[0] & 0x01 ){
						syog_genk( pNew, _SUB );
					}
					else{
						syog_genk( &data, _SUB );
						syog_genk( pNew, _ADD );
					}
#endif
				}
			}
		}
	}

	// 03.11 /10
	m_DPscan.SetOriginValttl();

	return	0;
}



//------------------------------------------------------------------------
// 入力バッファより仕訳に分解（メイン）
//	int				ln		分解する入力行 [データ上の行番号(1～100)]
//	CDBINPDataRec*	dtbl	分解された仕訳を格納
//
//	return 分解行数
//------------------------------------------------------------------------
int CDnpBaseClass::da_make( int ln, CDBINPDataRec *dtbl )
{
int DEBT_ln, CRED_ln;
struct _DP_DATA_LINE wk = {0};
int	line;
char	dbtval[6], creval[6];

	l_defn( 0x16 );

	LPCTSTR	genCode;
	genCode = GNKINCode(m_pZm->zvol->apno);

	memmove( &wk, getLINE_DATA( ln ), sizeof(struct _DP_DATA_LINE));		//現在行データゲット

	if( IsKind() == KIND_FURIKAE ) {
																			/*	借方科目	借方金額	貸方科目	貸方金額	*/
		/*///// 借方科目 入力有り /////*/									/*		○			？			？			？		*/
		if( _getCND( ln, FD_DEBT_PN )->INP_sg ) {
			/*///// 借方金額 ０￥ /////*/									/*		○			ゼロ		？			？		*/
			if( l_test( _getDATA( ln, FD_DBVAL_PN )->DP_VAL ) == 0 && TaikaInputCheck(ln, FD_DSKBN_PN ) == 0 ) {
				/*///// 貸方金額０￥ /////*/								/*		○			ゼロ		？			ゼロ	*/
				if( l_test( _getDATA( ln, FD_CRVAL_PN )->DP_VAL ) == 0 && TaikaInputCheck(ln, FD_CSKBN_PN ) == 0 ) {

					if( IsJob() == JOB_INPUT ) {
						if( ln <= 1 )	return	0;
						//貸方科目無は不可
						if( _getMAP( ln, FD_CRED_PN ) == -1  && _getCND( ln, FD_CRED_PN )->INP_sg == FALSE ) {
							return	0;
						}
						//摘要無は不可
						if( _getCND( ln, FD_TKY_PN )->INP_sg == FALSE )	return	0;
						DEBT_ln	=	-1;
						CRED_ln	=	-1;
						//金額入力のあるラインを取得
						for( line = ln - 1; line >= 1; line-- ) {
							if( _getCND( line, FD_DBVAL_PN )->INP_sg ) {
								if( l_test( _getDATA( line, FD_DBVAL_PN )->DP_VAL ) || TaikaInputCheck( line, FD_DSKBN_PN ) ) {
									if( DEBT_ln == -1 )	DEBT_ln	=	line;
								}
							}
							if( _getCND( line, FD_CRVAL_PN )->INP_sg ) {
								if( l_test( _getDATA( line, FD_CRVAL_PN )->DP_VAL ) || TaikaInputCheck( line, FD_CSKBN_PN ) ) {
									if( CRED_ln == -1 )	CRED_ln	=	line;
								}
							}
						}
						//金額入力がない場合は不可
						if( DEBT_ln == -1 || CRED_ln == -1 )	return	0;
						//同一ラインでない場合は不可
						if( DEBT_ln != CRED_ln )	return	0;
						//金額が同じ、つまり単一仕訳
						if( l_cmp( _getDATA( DEBT_ln, FD_DBVAL_PN )->DP_VAL, _getDATA( CRED_ln, FD_CRVAL_PN )->DP_VAL ) ) {
							return	0;
						}
						//現在行借方科目と金額入力のあるラインの借方科目が異なる場合は不可
						if( memcmp( _getDATA( ln, FD_DEBT_PN )->DP_CdNum, _getDATA( DEBT_ln, FD_DEBT_PN )->DP_CdNum, KMKCD_LN ) != 0 ) {
							return	0;
						}
						else if( _getDATA( ln, FD_DEBT_PN )->DP_BRN != _getDATA( DEBT_ln, FD_DEBT_PN )->DP_BRN ) {
							return	0;
						}
						//有効な貸方科目と金額入力のあるラインの貸方科目が異なる場合は不可
						if( memcmp( _getDATA( _getMAP( ln, FD_CRED_PN ), FD_CRED_PN )->DP_CdNum, _getDATA( CRED_ln, FD_CRED_PN )->DP_CdNum, KMKCD_LN ) != 0 ) {
							return	0;
						}
						else if( _getDATA( _getMAP( ln, FD_CRED_PN ), FD_CRED_PN )->DP_BRN != _getDATA( CRED_ln, FD_CRED_PN )->DP_BRN ) {
							return	0;
						}

						_getDATAs( &wk, FD_CRED_PN )->DP_stKMK = _getDATA( CRED_ln, FD_CRED_PN )->DP_stKMK;

						return siwake_mak( &wk, dtbl, 2 );
					}

					return 0;
				}
				/*///// 貸方金額有り /////*/								/*		○			ゼロ		？			有り	*/
				else {
					/*///// 貸方科目無し /////*/							/*		○			ゼロ		×			有り	*/
					if( (CRED_ln = _getMAP( ln, FD_CRED_PN )) == -1  && _getCND( ln, FD_CRED_PN )->INP_sg == FALSE ) {
						return 0;
					}
					/*/////	貸方科目有り /////*/	/* 前科目複写 */		/*		○			ゼロ		○			有り	*/
					else {
						_getDATAs( &wk, FD_CRED_PN )->DP_stKMK = _getDATA( CRED_ln, FD_CRED_PN )->DP_stKMK;
					}

					int set_ok = 0;

					if( TaikaInputCheck(ln, FD_CSKBN_PN ) != 0 ) {
						//対価入力時は、科目入力を生かす
						for( line = ln; line >= 1; line-- ) {
							if( _getCND( line, FD_DEBT_PN )->INP_sg ) {
								set_ok++;

								strcpy_s(	_getDATAs( &wk, FD_DEBT_PN )->DP_CdNum, sizeof _getDATAs( &wk, FD_DEBT_PN )->DP_CdNum, 
											_getDATA( line, FD_DEBT_PN )->DP_CdNum );
								break;
							}
						}
					}
					if( ! set_ok ) {
						strcpy_s( _getDATAs( &wk, FD_DEBT_PN )->DP_CdNum, sizeof _getDATAs( &wk, FD_DEBT_PN )->DP_CdNum, SYOGT );
						_getDATAs( &wk, FD_DEBT_PN )->DP_BRN = -1;
					}

					return siwake_mak( &wk, dtbl, 0 );
					
				}
			}
			/*///// 借方金額有り /////*/									/*		○			有り		？			？		*/
			else {
				/*///// 貸方金額０ /////*/									/*		○			有り		？			ゼロ	*/
				if( l_test( _getDATA( ln, FD_CRVAL_PN )->DP_VAL ) == 0 && ( ! TaikaInputCheck(ln, FD_CSKBN_PN) && !TaikaInputCheck(ln, FD_DSKBN_PN)) ) {
					strcpy_s( _getDATAs( &wk, FD_CRED_PN )->DP_CdNum, sizeof _getDATAs( &wk, FD_CRED_PN )->DP_CdNum, SYOGT );
					_getDATAs( &wk, FD_CRED_PN )->DP_BRN = -1;

					return siwake_mak( &wk, dtbl, 0 );
				}
				/*///// 貸方金額有り /////*/								/*		○			有り		？			有り	*/
				else {
					/*///// 貸方科目無し /////*/							/*		○			有り		×			有り	*/
					if( (CRED_ln = _getMAP( ln, FD_CRED_PN )) == -1 && _getCND( ln, FD_CRED_PN )->INP_sg == FALSE ) {
						return 0;
					}
					/*///// 貸方科目有り /////*/	/* 前科目複写 */		/*		○			有り		○			有り	*/
					else {
						_getDATAs( &wk, FD_CRED_PN )->DP_stKMK = _getDATA( CRED_ln, FD_CRED_PN )->DP_stKMK;
					}
					
					/*///// 借・貸　金額が等しい /////*/
					memmove( dbtval, _getDATAs( &wk, FD_DBVAL_PN )->DP_VAL, 6 );
					memmove( creval, _getDATAs( &wk, FD_CRVAL_PN )->DP_VAL, 6 );

					if( _sotozei_data( _getDATAs( &wk, FD_DSKBN_PN )->SYDATA._DSGN ) )
						l_add( dbtval, dbtval, _getDATAs( &wk, FD_DBZEI_PN )->DP_VAL );

					if( _sotozei_data( _getDATAs( &wk, FD_CSKBN_PN )->SYDATA._DSGN ) )
						l_add( creval, creval, _getDATAs( &wk, FD_CRZEI_PN )->DP_VAL );

					if( l_jsgn( dbtval, creval, 6 ) == 0 ) {
						return siwake_mak( &wk, dtbl, 2 );
					}
					/*///// 借・貸 金額が異なる /////*/
					else {
						return siwake_mak( &wk, dtbl, 1 );
					}
				}
			}
		}
		/*///// 借方科目無し /////*/										/*		？			？			？			？		*/
		else {
			/*///// 借方金額有り /////*/									/*		？			有り		？			？		*/
			if( l_test( _getDATA( ln, FD_DBVAL_PN )->DP_VAL ) || TaikaInputCheck( ln, FD_DSKBN_PN ) ) {
				/*///// 借方科目無し /////*/								/*		×			有り		？			？		*/
				if( (DEBT_ln = _getMAP( ln, FD_DEBT_PN)) == -1 && _getCND( ln, FD_DEBT_PN )->INP_sg == FALSE ) {
					return 0;
				}
				/*///// 借方科目有り /////*/	/* 前科目複写 */			/*		○			有り		？			？		*/
				else {
					_getDATAs( &wk, FD_DEBT_PN )->DP_stKMK = _getDATA( DEBT_ln, FD_DEBT_PN )->DP_stKMK;
				}
				
				/*///// 貸方金額無し /////*/								/*		○			有り		？			ゼロ	*/
				if( l_test( _getDATA( ln, FD_CRVAL_PN )->DP_VAL ) == 0 ) {
					strcpy_s( _getDATAs( &wk, FD_CRED_PN )->DP_CdNum, sizeof _getDATAs( &wk, FD_CRED_PN )->DP_CdNum, SYOGT );
					_getDATAs( &wk, FD_CRED_PN )->DP_BRN = -1;

					return siwake_mak( &wk, dtbl, 0 );
				}
				/*///// 貸方金額有り /////*/								/*		○			有り		？			有り	*/
				else {
					/*///// 貸方科目無し /////*/							/*		○			有り		×			有り	*/
					if( (CRED_ln = _getMAP( ln, FD_CRED_PN )) == -1 && _getCND( ln, FD_CRED_PN )->INP_sg == FALSE ) {
						return 0;
					}
					/*///// 貸方科目有り /////*/	/* 前科目複写 */		/*		○			有り		○			有り	*/
					else {
						_getDATAs( &wk, FD_CRED_PN )->DP_stKMK = _getDATA( CRED_ln, FD_CRED_PN )->DP_stKMK;
					}

					memmove( dbtval, _getDATAs( &wk, FD_DBVAL_PN )->DP_VAL, 6 );
					memmove( creval, _getDATAs( &wk, FD_CRVAL_PN )->DP_VAL, 6 );

					if( _sotozei_data( _getDATAs( &wk, FD_DSKBN_PN )->SYDATA._DSGN ) )
						l_add( dbtval, dbtval, _getDATAs( &wk, FD_DBZEI_PN )->DP_VAL );

					if( _sotozei_data( _getDATAs( &wk, FD_CSKBN_PN )->SYDATA._DSGN ) )
						l_add( creval, creval, _getDATAs( &wk, FD_CRZEI_PN )->DP_VAL );

					/*///// 借貸金額が等しい /////*/
					if( l_jsgn( dbtval, creval, 6 ) == 0 ) {
						return siwake_mak( &wk, dtbl, 2 );
					}
					/*///// 借貸金額が異なる /////*/
					else {
						return siwake_mak( &wk, dtbl, 1 );
					}
					
				}
			}
			/*///// 借方金額無し /////*/									/*		？			ゼロ			？			？		*/
			else {
				/*///// 貸方金額０ /////*/
				if( l_test( _getDATA( ln, FD_CRVAL_PN)->DP_VAL ) == 0 && TaikaInputCheck( ln, FD_CSKBN_PN ) == 0 ) {	/*		？			ゼロ			？			ゼロ	*/

					if( IsJob() == JOB_INPUT ) {
						if( ln <= 1 )	return	0;
						//貸方科目無は不可
						if( _getMAP( ln, FD_CRED_PN ) == -1 && _getCND( ln, FD_CRED_PN )->INP_sg == FALSE ) {
							return	0;
						}
						//摘要無は不可
						if( _getCND( ln, FD_TKY_PN )->INP_sg == FALSE )	return	0;
						DEBT_ln	=	-1;
						CRED_ln	=	-1;
						//金額入力のあるラインを取得
						for( line = ln - 1; line >= 0; line-- ) {
							if( _getCND( line, FD_DBVAL_PN )->INP_sg ) {
								if( l_test( _getDATA( line, FD_DBVAL_PN )->DP_VAL ) || TaikaInputCheck( line, FD_DSKBN_PN ) ) {
									if( DEBT_ln == -1 )	DEBT_ln	=	line;
								}
							}
							if( _getCND( line, FD_CRVAL_PN )->INP_sg ) {
								if( l_test( _getDATA( line, FD_CRVAL_PN )->DP_VAL ) || TaikaInputCheck( line, FD_CSKBN_PN ) ) {
									if( CRED_ln == -1 )	CRED_ln	=	line;
								}
							}
						}
						//金額入力がない場合は不可
						if( DEBT_ln == -1 || CRED_ln == -1 )	return	0;
						//同一ラインでない場合は不可
						if( DEBT_ln != CRED_ln )	return	0;
						//金額が同じ、つまり単一仕訳
						if( l_cmp( _getDATA( DEBT_ln, FD_DBVAL_PN )->DP_VAL, _getDATA( CRED_ln, FD_CRVAL_PN )->DP_VAL ) ) {
							return	0;
						}
						//有効な借方科目と金額入力のあるラインの借方科目が異なる場合は不可
						if( _getDATA( _getMAP( ln, FD_DEBT_PN ), FD_DEBT_PN )->DP_CdNum != _getDATA( DEBT_ln, FD_DEBT_PN )->DP_CdNum ) {
							return	0;
						}
						else if( _getDATA( _getMAP( ln, FD_DEBT_PN ), FD_DEBT_PN )->DP_BRN != _getDATA( DEBT_ln, FD_DEBT_PN )->DP_BRN ) {
							return	0;
						}
						//有効な貸方科目と金額入力のあるラインの貸方科目が異なる場合は不可
						if( _getDATA( _getMAP( ln, FD_CRED_PN ), FD_CRED_PN )->DP_CdNum != _getDATA( CRED_ln, FD_CRED_PN )->DP_CdNum ) {
							return	0;
						}
						else if( _getDATA( _getMAP( ln, FD_CRED_PN ), FD_CRED_PN )->DP_BRN != _getDATA( CRED_ln, FD_CRED_PN )->DP_BRN ) {
							return	0;
						}

						_getDATAs( &wk, FD_DEBT_PN )->DP_stKMK = _getDATA( DEBT_ln, FD_DEBT_PN )->DP_stKMK;
						_getDATAs( &wk, FD_CRED_PN )->DP_stKMK = _getDATA( CRED_ln, FD_CRED_PN )->DP_stKMK;

						return siwake_mak( &wk, dtbl, 2 );
					}

					return 0;
				}
				/*///// 貸方金額有り /////*/								/*		？			ゼロ			？			有り	*/
				else {
					/*///// 貸方科目無し /////*/							/*		？			ゼロ			×			有り	*/
					if( (CRED_ln = _getMAP( ln, FD_CRED_PN )) == -1 && _getCND( ln, FD_CRED_PN )->INP_sg == FALSE ) {
						return 0;
					}
					/*///// 貸方科目有り /////*/	/* 前科目複写 */		/*		？			ゼロ			○			有り	*/
					else {
						_getDATAs( &wk, FD_CRED_PN )->DP_stKMK = _getDATA( CRED_ln, FD_CRED_PN )->DP_stKMK;
					}

					strcpy_s( _getDATAs( &wk, FD_DEBT_PN )->DP_CdNum, sizeof _getDATAs( &wk, FD_DEBT_PN )->DP_CdNum, SYOGT );
					_getDATAs( &wk, FD_DEBT_PN )->DP_BRN = -1;

					return siwake_mak( &wk, dtbl, 0 );
				}
			}
		}
	}	// end of KIND_FURIKAE
	else if( IsKind() == KIND_NYUKIN ) {
		//科目入力有り
		if( _getCND( ln, FD_CRED_PN )->INP_sg ) {
			//金額有り
			if( l_test( _getDATA( ln, FD_CRVAL_PN )->DP_VAL ) || TaikaInputCheck( ln, FD_CSKBN_PN )) {
				_getDATAs( &wk, FD_CRED_PN )->DP_stKMK	=	_getDATA( ln, FD_CRED_PN )->DP_stKMK;

			//	_getDATAs( &wk, FD_CRED_PN )->DP_CdNum	=	_getDATA( ln, FD_CRED_PN )->DP_CdNum;
			//	_getDATAs( &wk, FD_CRED_PN )->DP_BRN	=	_getDATA( ln, FD_CRED_PN )->DP_BRN;
			
				strcpy_s( _getDATAs( &wk, FD_DEBT_PN )->DP_CdNum, sizeof _getDATAs( &wk, FD_DEBT_PN )->DP_CdNum, genCode );
				_getDATAs( &wk, FD_DEBT_PN )->DP_BRN	=	-1;

				return	siwake_mak( &wk, dtbl, 0 );
			}
			else {
				if( IsJob() == JOB_INPUT ) {
					CRED_ln	=	-1;
					//金額入力のあるラインを取得
					for( line = ln - 1; line >= 1; line-- ) {
						if( _getCND( line, FD_CRVAL_PN )->INP_sg ) {
							if( l_test( _getDATA( line, FD_CRVAL_PN )->DP_VAL ) ) {
								if( CRED_ln == -1 )	CRED_ln	=	line;
							}
						}
					}
					//金額入力がない場合は不可
					if( CRED_ln == -1 )	return	0;
					//現在行貸方科目と金額入力のあるラインの貸方科目が異なる場合は不可
					if( memcmp( _getDATA( ln, FD_CRED_PN )->DP_CdNum, _getDATA( CRED_ln, FD_CRED_PN )->DP_CdNum, KMKCD_LN ) != 0 ) {
						return	0;
					}
					else if( _getDATA( ln, FD_CRED_PN )->DP_BRN != _getDATA( CRED_ln, FD_CRED_PN )->DP_BRN ) {
						return	0;
					}
					_getDATAs( &wk, FD_CRED_PN )->DP_stKMK	=	_getDATA( ln, FD_CRED_PN )->DP_stKMK;

					strcpy_s( _getDATAs( &wk, FD_DEBT_PN )->DP_CdNum, sizeof _getDATAs( &wk, FD_DEBT_PN )->DP_CdNum, genCode );
					_getDATAs( &wk, FD_DEBT_PN )->DP_BRN	=	-1;

					return	siwake_mak( &wk, dtbl, 0 );
				}
			}

		}
		//科目入力無し
		else {
			//金額のみ入力
			if( l_test( _getDATA( ln, FD_CRVAL_PN )->DP_VAL ) || TaikaInputCheck( ln, FD_CSKBN_PN )  ) {
				//上部にも科目無し
				if( (CRED_ln = _getMAP( ln, FD_CRED_PN )) == -1 ) {
					return	0;
				}
				_getDATAs( &wk, FD_CRED_PN )->DP_stKMK	=	_getDATA( CRED_ln, FD_CRED_PN )->DP_stKMK;
				strcpy_s( _getDATAs( &wk, FD_DEBT_PN )->DP_CdNum, sizeof _getDATAs( &wk, FD_DEBT_PN )->DP_CdNum, genCode );
				_getDATAs( &wk, FD_DEBT_PN )->DP_BRN	=	-1;

				return	siwake_mak( &wk, dtbl, 0 );
			}
			else {
				if( IsJob() == JOB_INPUT ) {
					//摘要無は不可
					if( _getCND( ln, FD_TKY_PN )->INP_sg == FALSE )	return	0;

					CRED_ln	=	-1;
					//金額入力のあるラインを取得
					for( line = ln - 1; line >= 1; line-- ) {
						if( _getCND( line, FD_CRVAL_PN )->INP_sg ) {
							if( l_test( _getDATA( line, FD_CRVAL_PN )->DP_VAL ) ) {
								if( CRED_ln == -1 )	CRED_ln	=	line;
							}
						}
					}
					//金額入力がない場合は不可
					if( CRED_ln == -1 )	return	0;
					//有効な貸方科目と金額入力のあるラインの貸方科目が異なる場合は不可
					if( memcmp( _getDATA( _getMAP( ln, FD_CRED_PN ), FD_CRED_PN )->DP_CdNum, _getDATA( CRED_ln, FD_CRED_PN )->DP_CdNum, KMKCD_LN ) != 0 ) {
						return	0;
					}
					else if( _getDATA( _getMAP( ln, FD_CRED_PN ), FD_CRED_PN )->DP_BRN != _getDATA( CRED_ln, FD_CRED_PN )->DP_BRN ) {
						return	0;
					}
					_getDATAs( &wk, FD_CRED_PN )->DP_stKMK	=	_getDATA( CRED_ln, FD_CRED_PN )->DP_stKMK;
					strcpy_s( _getDATAs( &wk, FD_DEBT_PN )->DP_CdNum, sizeof _getDATAs( &wk, FD_DEBT_PN )->DP_CdNum, genCode );
					_getDATAs( &wk, FD_DEBT_PN )->DP_BRN	=	-1;

					return	siwake_mak( &wk, dtbl, 0 );
				}
			}
		}
		return	0;
	}	//KIND_NYUKIN
	else if( IsKind() == KIND_SYUKKIN ) {
		//科目入力有り
		if( _getCND( ln, FD_DEBT_PN )->INP_sg ) {
			//金額有り
			if( l_test( _getDATA( ln, FD_DBVAL_PN )->DP_VAL ) || TaikaInputCheck( ln, FD_DSKBN_PN ) ) {
				_getDATAs( &wk, FD_DEBT_PN )->DP_stKMK	=	_getDATA( ln, FD_DEBT_PN )->DP_stKMK;
				
				strcpy_s( _getDATAs( &wk, FD_CRED_PN )->DP_CdNum, sizeof _getDATAs( &wk, FD_CRED_PN )->DP_CdNum, genCode );
				_getDATAs( &wk, FD_CRED_PN )->DP_BRN	= -1;

				return	siwake_mak( &wk, dtbl, 0 );
			}
			else {
				if( IsJob() == JOB_INPUT ) {
					DEBT_ln	=	-1;
					//金額入力のあるラインを取得
					for( line = ln - 1; line >= 1; line-- ) {
						if( _getCND( line, FD_DBVAL_PN )->INP_sg ) {
							if( l_test( _getDATA( line, FD_DBVAL_PN )->DP_VAL ) || TaikaInputCheck( ln, FD_DSKBN_PN ) ) {
								if( DEBT_ln == -1 )	DEBT_ln	=	line;
							}
						}
					}
					//金額入力がない場合は不可
					if( DEBT_ln == -1 )	return	0;
					//現在行貸方科目と金額入力のあるラインの貸方科目が異なる場合は不可
					if( memcmp( _getDATA( ln, FD_DEBT_PN )->DP_CdNum, _getDATA( DEBT_ln, FD_DEBT_PN )->DP_CdNum, KMKCD_LN ) != 0 ) {
						return	0;
					}
					else if( _getDATA( ln, FD_DEBT_PN)->DP_BRN != _getDATA( DEBT_ln, FD_DEBT_PN )->DP_BRN ) {
						return	0;
					}

					_getDATAs( &wk, FD_DEBT_PN )->DP_stKMK	=	_getDATA( ln, FD_DEBT_PN )->DP_stKMK;
					strcpy_s( _getDATAs( &wk, FD_CRED_PN )->DP_CdNum, sizeof _getDATAs( &wk, FD_CRED_PN )->DP_CdNum, genCode );
					_getDATAs( &wk, FD_CRED_PN )->DP_BRN	= -1;

					return	siwake_mak( &wk, dtbl, 0 );
				}
			}

		}
		//科目入力無し
		else {
			//金額のみ入力
			if( l_test( _getDATA( ln, FD_DBVAL_PN )->DP_VAL ) || TaikaInputCheck( ln, FD_DSKBN_PN ) ) {
				//上部にも科目無し
				if( (DEBT_ln = _getMAP( ln, FD_DEBT_PN)) == -1 ) {
					return	0;
				}
				_getDATAs( &wk, FD_DEBT_PN )->DP_stKMK	=	_getDATA( DEBT_ln, FD_DEBT_PN )->DP_stKMK;
				
				strcpy_s( _getDATAs( &wk, FD_CRED_PN )->DP_CdNum, sizeof _getDATAs( &wk, FD_CRED_PN )->DP_CdNum, genCode );
				_getDATAs( &wk, FD_CRED_PN )->DP_BRN	=	-1;

				return	siwake_mak( &wk, dtbl, 0 );
			}
			else {
				if( IsJob() == JOB_INPUT ) {
					//摘要無は不可
					if( _getCND( ln, FD_TKY_PN )->INP_sg == FALSE )	return	0;

					DEBT_ln	=	-1;
					//金額入力のあるラインを取得
					for( line = ln - 1; line >= 1; line-- ) {
						if( _getCND( line, FD_DBVAL_PN )->INP_sg ) {
							if( l_test( _getDATA( line, FD_DBVAL_PN )->DP_VAL ) || TaikaInputCheck( ln, FD_DSKBN_PN )) {
								if( DEBT_ln == -1 )	DEBT_ln	=	line;
							}
						}
					}
					//金額入力がない場合は不可
					if( DEBT_ln == -1 )	return	0;
					//現在行貸方科目と金額入力のあるラインの貸方科目が異なる場合は不可
					if( memcmp( _getDATA( _getMAP( ln, FD_DEBT_PN), FD_DEBT_PN )->DP_CdNum, _getDATA( DEBT_ln, FD_DEBT_PN )->DP_CdNum, KMKCD_LN) != 0 ) {
						return	0;
					}
					else if( _getDATA( _getMAP( ln, FD_DEBT_PN), FD_DEBT_PN)->DP_BRN != _getDATA( DEBT_ln, FD_DEBT_PN )->DP_BRN ) {
						return	0;
					}

					_getDATAs( &wk, FD_DEBT_PN )->DP_stKMK	=	_getDATA( DEBT_ln, FD_DEBT_PN )->DP_stKMK;
					strcpy_s( _getDATAs( &wk, FD_CRED_PN )->DP_CdNum, sizeof _getDATAs( &wk, FD_CRED_PN )->DP_CdNum, genCode );
					_getDATAs( &wk, FD_CRED_PN )->DP_BRN	=	-1;

					return	siwake_mak( &wk, dtbl, 0 );
				}
			}
		}
		return	0;
	}	//KIND_SYUKKIN
	return	0;
}

//-------------------------------------------------------
// 仕訳に分解（サブ）
//	struct _DP_DATA_LINE *datp	入力データバッファ
//	CDBINPDataRec	*dtbl0		仕訳格納レコード
//	int	sgn			0->単一, 1->複合, 2->単一で貸借金額入力あり
//
//	
//-------------------------------------------------------
int CDnpBaseClass::siwake_mak( struct _DP_DATA_LINE *datp, CDBINPDataRec *dtbl0, int sgn )
{
int		i, n, max, dc_sw, temp;
CDBINPDataRec	*wk, tmp1, tmp2;
CDBINPDataRec *dtbl, wktbl[2];
	
	tmp1.Reset();	tmp2.Reset();
	for( i = 0; i < sizeof wktbl / sizeof wktbl[0]; i++ ) {
		wktbl[i].Reset();
		wktbl[i].m_seq = i+1;
	}

	dtbl = wktbl;

	if( IsKind() == KIND_FURIKAE ) {

		if( sgn == 1 ) {
			/*///// 複合仕訳 /////*/
			for( i = 0; i < 2; i++ ) {
				wk = dtbl+i;
				
				// 日付セット
				setDATE_CREC( wk );
				// 伝票セット
				wk->m_cno = getDATA( DP_DENP_PN )->DP_DENP;

				if( m_DI.IsSyonum() ) {
					wk->m_skn = getDATA(DP_SYONUM_PN)->DP_SNUM;
				}
				else {
					wk->m_skn.Empty();
				}

				// 手形関係
				wk->m_rnumber = _getDATAs( datp, FD_TGNUM_PN )->DP_SNUM;
				wk->m_tdate	  = _getDATAs( datp, FD_TGDATE_PN )->DP_TGDAY;
				// 変動事由
				wk->m_hjiyu	  = _getDATAs( datp, FD_HJIYU_PN )->DP_BMN;
				wk->m_nbcd	  = _getDATAs( datp, FD_HJIYU_PN )->NAIBCD;

				if( i ) {
					wk->m_dbt = SYOGT;
					wk->m_dbr = -1;
					if( IsFuriSyogBrn() ) {
						temp = _getDATAs( datp, FD_CSYOG_PN )->DP_cBRN;
						if( temp > 0 )
							wk->m_dbr = temp;
					}

					wk->m_cre = _getDATAs( datp, FD_CRED_PN )->DP_CdNum;
					wk->m_cbr = _getDATAs( datp, FD_CRED_PN )->DP_BRN;

					// 諸口のほうには、部門・工事をつけない
					if( m_DI.IsBumon() ) {
						wk->m_dbmn = -1;
						// 個別 部門番号
						if( m_FuriMode == FURI_DATABMON )
							wk->m_cbmn = _getDATAs( datp, FD_CBMN_PN )->DP_BMN;
						else
							wk->m_cbmn = getDATA( DP_BMN_PN )->DP_BMN;
					}
					else {
						wk->m_dbmn = -1;
						wk->m_cbmn = -1;
					}

					if( m_DI.IsKoji() ) {
						wk->m_dkno.Empty();
						// 個別 工事番号
						if( m_FuriMode == FURI_DATABMON )
							wk->m_ckno = _getDATAs( datp, FD_CKOJI_PN )->DP_KOJI;
						else
							wk->m_ckno = getDATA( DP_KOJI_PN )->DP_KOJI;
					}
					else {
						wk->m_dkno.Empty();
						wk->m_ckno.Empty();
					}
					// 消費税をセットしているか？
					if( _getCNDs( datp, FD_CSKBN_PN )->INP_sg ) {
						memmove( &wk->m_dsign[0], &_getDATAs( datp, FD_CSKBN_PN )->SYDATA._DSGN, SZ_DSGN );
					}
					else {
						::ZeroMemory( &wk->m_dsign[0], SZ_DSGN );
					}
					memmove( wk->m_val, _getDATAs( datp, FD_CRVAL_PN )->DP_VAL, 6 );
					memmove( wk->m_zei, _getDATAs( datp, FD_CRZEI_PN )->DP_VAL, 6 );

					wk->m_tekiyo = _getDATAs( datp, FD_TKY_PN )->DP_TKY;
					memmove( wk->m_taika, _getDATAs( datp, FD_CSKBN_PN )->SYDATA.TKsyohi, 6 );
					memmove( wk->m_invno, _getDATAs(datp, FD_CSKBN_PN)->SYDATA.TKinvno, SZ_INVNO);
				} 
				else {
					wk->m_dbt = _getDATAs( datp, FD_DEBT_PN )->DP_CdNum;
					wk->m_dbr = _getDATAs( datp, FD_DEBT_PN )->DP_BRN;
					wk->m_cre = SYOGT;
					wk->m_cbr = -1;
					if( IsFuriSyogBrn() ) {
						temp = _getDATAs( datp, FD_DSYOG_PN )->DP_cBRN;
						if( temp > 0 )
							wk->m_cbr = temp;
					}

					// 諸口のほうには、部門・工事をつけない
					if( m_DI.IsBumon() ) {
						// 個別 部門番号
						if( m_FuriMode == FURI_DATABMON )
							wk->m_dbmn = _getDATAs( datp, FD_DBMN_PN )->DP_BMN;
						else
							wk->m_dbmn = getDATA( DP_BMN_PN )->DP_BMN;

						wk->m_cbmn = -1;
					}
					else {
						wk->m_dbmn = -1;
						wk->m_cbmn = -1;
					}

					if( m_DI.IsKoji() ) {
						// 個別 工事番号
						if( m_FuriMode == FURI_DATABMON )
							wk->m_dkno = _getDATAs( datp, FD_DKOJI_PN )->DP_KOJI;
						else
							wk->m_dkno = getDATA( DP_KOJI_PN )->DP_KOJI;

						wk->m_ckno.Empty();
					}
					else {
						wk->m_dkno.Empty();
						wk->m_ckno.Empty();
					}

					// 消費税をセットしているか？
					if( _getCNDs( datp, FD_DSKBN_PN )->INP_sg ) {
						memmove( &wk->m_dsign[0], &_getDATAs( datp, FD_DSKBN_PN )->SYDATA._DSGN, SZ_DSGN );
					}
					else {
						::ZeroMemory( &wk->m_dsign[0], SZ_DSGN );
					}

					memmove( wk->m_val, _getDATAs( datp, FD_DBVAL_PN )->DP_VAL, 6 );
					memmove( wk->m_zei, _getDATAs( datp, FD_DBZEI_PN )->DP_VAL, 6 );

					wk->m_tekiyo = _getDATAs( datp, FD_TKY_PN )->DP_TKY;
					memmove( wk->m_taika, _getDATAs( datp, FD_DSKBN_PN )->SYDATA.TKsyohi, 6 );
					memmove( wk->m_invno, _getDATAs(datp, FD_DSKBN_PN)->SYDATA.TKinvno, SZ_INVNO);
				}

				// 付箋
				wk->m_dsign[7] &= (~0x0f);
				wk->m_dsign[7] |= _getDATAs( datp, FD_HUSEN_PN )->DP_HUSEN;
				wk->m_tag		= _getDATAs( datp, FD_HUSEN_PN )->DP_HCMNT;

				wk->m_dtype = 0x30;	// 伝票種別

			}
#ifdef KEEP
			tmp1 = *dtbl;
			tmp2 = *(dtbl+1);
			
			if( l_jsgn( tmp1.m_val, tmp2.m_val, 6 ) < 0 ) {
				*(dtbl+1)	= tmp1;
				*dtbl		= tmp2;
			}
#endif
			n = 2;
		}
		else {
			/*///// 単一仕訳 /////*/

			//借方／貸方の消費税チェック
			dc_sw = 0;
			if( m_DI.km_syattr( CString( _getDATAs( datp, FD_DEBT_PN )->DP_CdNum ) ) )
				dc_sw |= 0x01;

			if( m_DI.km_syattr( CString(_getDATAs( datp, FD_CRED_PN )->DP_CdNum) ) )
				dc_sw |= 0x02;

			if( dc_sw == 0x03 ) {
				if( memcmp( _getDATAs( datp, FD_DSKBN_PN )->SYDATA._DSGN,
							_getDATAs( datp, FD_CSKBN_PN )->SYDATA._DSGN,
							SZ_DSGN 
						   ) == 0 ) {
					max = 1;
				}
				else {
					max = 2;
				}
			}
			else
				max = 1;

			for( i = 0; i < max; i++ ) {
				wk = dtbl+i;
				// 日付セット
				setDATE_CREC( wk );
				// 伝票セット
				wk->m_cno = getDATA( DP_DENP_PN )->DP_DENP;

				if( m_DI.IsSyonum() ) {
					wk->m_skn = getDATA(DP_SYONUM_PN)->DP_SNUM;
				}
				else {
					wk->m_skn.Empty();
				}

				// 手形関係
				wk->m_rnumber = _getDATAs( datp, FD_TGNUM_PN )->DP_SNUM;
				wk->m_tdate	  = _getDATAs( datp, FD_TGDATE_PN )->DP_TGDAY;
				// 変動事由
				wk->m_hjiyu	  = _getDATAs( datp, FD_HJIYU_PN )->DP_BMN;
				wk->m_nbcd	  = _getDATAs( datp, FD_HJIYU_PN )->NAIBCD;

				if( max == 1 ) {
					// 借方／貸方両方には消費税はつかない
					wk->m_dbt = _getDATAs( datp, FD_DEBT_PN)->DP_CdNum;
					wk->m_dbr = _getDATAs( datp, FD_DEBT_PN)->DP_BRN;
					wk->m_cre = _getDATAs( datp, FD_CRED_PN)->DP_CdNum;
					wk->m_cbr = _getDATAs( datp, FD_CRED_PN)->DP_BRN;

					// 諸口科目チェック D0: 借方諸口, D1: 貸方諸口
					WORD syog = 0;
					if( isSYOGT_CODE( wk->m_dbt ) )
						syog |= 0x01;
					else if( isSYOGT_CODE( wk->m_cre ) )
						syog |= 0x02;

					if( m_DI.IsBumon() ) {
						// 個別 部門番号
						if( m_FuriMode == FURI_DATABMON ) {
							if( syog == 0 ) {
								wk->m_dbmn = _getDATAs( datp, FD_DBMN_PN )->DP_BMN;
								wk->m_cbmn = _getDATAs( datp, FD_CBMN_PN )->DP_BMN;
							}
							else {
								if( (syog & 0x01) ) {
									wk->m_dbmn = -1;;
									wk->m_cbmn = _getDATAs( datp, FD_CBMN_PN )->DP_BMN;
								}
								else {
									wk->m_dbmn = _getDATAs( datp, FD_DBMN_PN )->DP_BMN;
									wk->m_cbmn = -1;
								}
							}
						}
						else {	// 伝票で単一の部門番号
							if( syog == 0 ) {
								wk->m_dbmn = getDATA( DP_BMN_PN )->DP_BMN;
								wk->m_cbmn = getDATA( DP_BMN_PN )->DP_BMN;
							}
							else {
								if( (syog & 0x01) ) {
									wk->m_dbmn = -1;;
									wk->m_cbmn = getDATA( DP_BMN_PN )->DP_BMN;
								}
								else {
									wk->m_dbmn = getDATA( DP_BMN_PN )->DP_BMN;
									wk->m_cbmn = -1;
								}
							}
						}
					}
					else {
						wk->m_dbmn = -1;
						wk->m_cbmn = -1;
					}
					if( m_DI.IsKoji() ) {
						// 個別 工事番号
						if( m_FuriMode == FURI_DATABMON ) {
							if( syog == 0 ) {
								wk->m_dkno = _getDATAs( datp, FD_DKOJI_PN )->DP_KOJI;
								wk->m_ckno = _getDATAs( datp, FD_CKOJI_PN )->DP_KOJI;
							}
							else {
								if( (syog & 0x01) ) {
									wk->m_dkno.Empty();
									wk->m_ckno = _getDATAs( datp, FD_CKOJI_PN )->DP_KOJI;
								}
								else {
									wk->m_dkno = _getDATAs( datp, FD_DKOJI_PN )->DP_KOJI;
									wk->m_ckno.Empty();
								}
							}
						}
						else {	// 伝票で単一の工事番号
							if( syog == 0 ) {
								wk->m_dkno = getDATA( DP_KOJI_PN )->DP_KOJI;
								wk->m_ckno = getDATA( DP_KOJI_PN )->DP_KOJI;
							}
							else {
								if( (syog & 0x01) ) {
									wk->m_dkno.Empty();
									wk->m_ckno = getDATA( DP_KOJI_PN )->DP_KOJI;
								}
								else {
									wk->m_dkno = getDATA( DP_KOJI_PN )->DP_KOJI;
									wk->m_ckno.Empty();
								}
							}
						}
					}
					else {
						wk->m_dkno.Empty();
						wk->m_ckno.Empty();
					}

					// 借方 諸口
					if( syog & 0x01 ) {
						if( IsFuriSyogBrn() ) {
							temp = _getDATAs( datp, FD_CSYOG_PN )->DP_cBRN;
							if( temp > 0 )
								wk->m_dbr = temp;
						}
					}
					// 貸方 諸口
					if( syog & 0x02 ) {
						if( IsFuriSyogBrn() ) {
							temp = _getDATAs( datp, FD_DSYOG_PN )->DP_cBRN;
							if( temp > 0 )
								wk->m_cbr = temp;
						}
					}

					if( isSYOGT_CODE( _getDATAs( datp, FD_DEBT_PN)->DP_CdNum ) || (dc_sw & 0x02) == 0x02 ) {
						memmove( wk->m_val, _getDATAs( datp, FD_CRVAL_PN)->DP_VAL, 6 );
						memmove( wk->m_zei, _getDATAs( datp, FD_CRZEI_PN )->DP_VAL, 6 );
					}
					else {
						memmove( wk->m_val, _getDATAs( datp, FD_DBVAL_PN)->DP_VAL, 6 );
						memmove( wk->m_zei, _getDATAs( datp, FD_DBZEI_PN )->DP_VAL, 6 );
					}

					if( dc_sw & 0x01 ) {
						memmove( &wk->m_dsign[0], &_getDATAs( datp, FD_DSKBN_PN )->SYDATA._DSGN, SZ_DSGN );
						memmove( wk->m_taika, _getDATAs( datp, FD_DSKBN_PN )->SYDATA.TKsyohi, 6 );
						memmove( wk->m_invno, _getDATAs( datp, FD_DSKBN_PN )->SYDATA.TKinvno, SZ_INVNO);
					}
					else if( dc_sw & 0x02 ) {
						memmove( &wk->m_dsign[0], &_getDATAs( datp, FD_CSKBN_PN )->SYDATA._DSGN, SZ_DSGN );
						memmove( wk->m_taika, _getDATAs( datp, FD_CSKBN_PN )->SYDATA.TKsyohi, 6 );
						memmove( wk->m_invno, _getDATAs( datp, FD_CSKBN_PN )->SYDATA.TKinvno, SZ_INVNO);
					}
					else {
						// 借方 諸口
						if( syog & 0x01 ) {
							memmove( &wk->m_dsign[0], &_getDATAs( datp, FD_CSKBN_PN )->SYDATA._DSGN, SZ_DSGN );
							memmove( wk->m_taika, _getDATAs( datp, FD_CSKBN_PN )->SYDATA.TKsyohi, 6 );
							memmove( wk->m_invno, _getDATAs( datp, FD_CSKBN_PN )->SYDATA.TKinvno, SZ_INVNO);
						}
						else {
							memmove( &wk->m_dsign[0], &_getDATAs( datp, FD_DSKBN_PN )->SYDATA._DSGN, SZ_DSGN );
							memmove( wk->m_taika, _getDATAs( datp, FD_DSKBN_PN )->SYDATA.TKsyohi, 6 );
							memmove( wk->m_invno, _getDATAs( datp, FD_DSKBN_PN )->SYDATA.TKinvno, SZ_INVNO);
						}
					}

					wk->m_tekiyo = _getDATAs( datp, FD_TKY_PN )->DP_TKY;

				}
				else {
					// 借方／貸方ともに消費税属性科目 ( max==2 )
					if( i ) {
						wk->m_dbt = SYOGT;
						wk->m_dbr = -1;
						if( IsFuriSyogBrn() ) {
							temp = _getDATAs( datp, FD_CSYOG_PN )->DP_cBRN;
							if( temp > 0 )
								wk->m_dbr = temp;
						}

						wk->m_cre = _getDATAs( datp, FD_CRED_PN )->DP_CdNum;
						wk->m_cbr = _getDATAs( datp, FD_CRED_PN )->DP_BRN;

						// 諸口のほうには、部門・工事をつけない
						if( m_DI.IsBumon() ) {
							if( m_FuriMode == FURI_DATABMON ) {
								wk->m_dbmn = -1;
								wk->m_cbmn = _getDATAs( datp, FD_CBMN_PN )->DP_BMN;
							}
							else {
								// 伝票で単一の部門
								wk->m_dbmn = -1;
								wk->m_cbmn = getDATA( DP_BMN_PN )->DP_BMN;
							}
						}
						else {
							wk->m_dbmn = -1;
							wk->m_cbmn = -1;
						}

						if( m_DI.IsKoji() ) {
							if( m_FuriMode == FURI_DATABMON ) {
								wk->m_dkno.Empty();
								wk->m_ckno = _getDATAs( datp, FD_CKOJI_PN )->DP_KOJI;
							}
							else {
								// 伝票で単一の工事
								wk->m_dkno.Empty();
								wk->m_ckno = getDATA( DP_KOJI_PN )->DP_KOJI;
							}
						}
						else {
							wk->m_dkno.Empty();
							wk->m_ckno.Empty();
						}

						memmove( &wk->m_dsign[0], &_getDATAs( datp, FD_CSKBN_PN )->SYDATA._DSGN, SZ_DSGN );
						memmove( wk->m_val, _getDATAs( datp, FD_CRVAL_PN )->DP_VAL, 6 );
						memmove( wk->m_zei, _getDATAs( datp, FD_CRZEI_PN )->DP_VAL, 6 );

						wk->m_tekiyo = _getDATAs( datp, FD_TKY_PN )->DP_TKY;
						memmove( wk->m_taika, _getDATAs( datp, FD_CSKBN_PN )->SYDATA.TKsyohi, 6 );
						memmove( wk->m_invno, _getDATAs( datp, FD_CSKBN_PN )->SYDATA.TKinvno, SZ_INVNO);
					} 
					else {
						wk->m_dbt = _getDATAs( datp, FD_DEBT_PN )->DP_CdNum;
						wk->m_dbr = _getDATAs( datp, FD_DEBT_PN )->DP_BRN;
						wk->m_cre = SYOGT;
						wk->m_cbr = -1;
						if( IsFuriSyogBrn() ) {
							temp = _getDATAs( datp, FD_DSYOG_PN )->DP_cBRN;
							if( temp > 0 )
								wk->m_cbr = temp;
						}

						// 諸口のほうには、部門・工事をつけない
						if( m_DI.IsBumon() ) {
							if( m_FuriMode == FURI_DATABMON ) {
								wk->m_dbmn = _getDATAs( datp, FD_DBMN_PN )->DP_BMN;
								wk->m_cbmn = -1;
							}
							else {
								// 伝票で単一の部門
								wk->m_dbmn = getDATA( DP_BMN_PN )->DP_BMN;
								wk->m_cbmn = -1;
							}
						}
						else {
							wk->m_dbmn = -1;
							wk->m_cbmn = -1;
						}

						if( m_DI.IsKoji() ) {
							if( m_FuriMode == FURI_DATABMON ) {
								wk->m_dkno = _getDATAs( datp, FD_DKOJI_PN )->DP_KOJI;
								wk->m_ckno.Empty();
							}
							else {
								// 伝票で単一の工事
								wk->m_dkno = getDATA( DP_KOJI_PN )->DP_KOJI;
								wk->m_ckno.Empty();
							}
						}
						else {
							wk->m_dkno.Empty();
							wk->m_ckno.Empty();
						}

						memmove( &wk->m_dsign[0], &_getDATAs( datp, FD_DSKBN_PN )->SYDATA._DSGN, SZ_DSGN );
						memmove( wk->m_val, _getDATAs( datp, FD_DBVAL_PN )->DP_VAL, 6 );
						memmove( wk->m_zei, _getDATAs( datp, FD_DBZEI_PN )->DP_VAL, 6 );

						wk->m_tekiyo = _getDATAs( datp, FD_TKY_PN )->DP_TKY;
						memmove( wk->m_taika, _getDATAs( datp, FD_DSKBN_PN )->SYDATA.TKsyohi, 6 );
						memmove( wk->m_invno, _getDATAs( datp, FD_CSKBN_PN )->SYDATA.TKinvno, SZ_INVNO);
					}
				}

				// 付箋
				wk->m_dsign[7] &= (~0x0f);
				wk->m_dsign[7] |= _getDATAs( datp, FD_HUSEN_PN )->DP_HUSEN;
				wk->m_tag		= _getDATAs( datp, FD_HUSEN_PN )->DP_HCMNT;

				if( (sgn == 2 && max != 2) && (wk->m_dbt == SYOGT || wk->m_cre == SYOGT) ) {
					wk->m_dtype	=	0;	// このコードの意味がよくわからない
				}
				else {
					wk->m_dtype	=	0x30;
				}

			}
			n = max;
		}

		// 消費税仕訳作成
		return sydata_mak( dtbl0, wktbl, n, datp );
	}	//KIND_FURIKAE
	else if( IsKind() == KIND_NYUKIN ) {
		wk = dtbl;

		// 日付セット
		setDATE_CREC( wk );
		// 伝票セット
		wk->m_cno = getDATA( DP_DENP_PN )->DP_DENP;
		if( m_DI.IsSyonum() ) {
			wk->m_skn = getDATA(DP_SYONUM_PN)->DP_SNUM;
		}
		else {
			wk->m_skn.Empty();
		}

		if( m_DI.IsBumon() ) {
			wk->m_dbmn = getDATA( DP_BMN_PN )->DP_BMN;
			wk->m_cbmn = wk->m_dbmn;
		}
		else {
			wk->m_dbmn = -1;
			wk->m_cbmn = -1;
		}

		if( m_DI.IsKoji() ) {
			wk->m_dkno = getDATA( DP_KOJI_PN )->DP_KOJI;
			wk->m_ckno = wk->m_dkno;
		}
		else {
			wk->m_dkno.Empty();
			wk->m_ckno.Empty();
		}

		wk->m_dbt = _getDATAs( datp, FD_DEBT_PN)->DP_CdNum;
		wk->m_dbr = _getDATAs( datp, FD_DEBT_PN)->DP_BRN;
		wk->m_cre = _getDATAs( datp, FD_CRED_PN)->DP_CdNum;
		wk->m_cbr = _getDATAs( datp, FD_CRED_PN)->DP_BRN;
			
		memmove( &wk->m_dsign[0], &_getDATAs( datp, FD_CSKBN_PN )->SYDATA._DSGN, SZ_DSGN );
		memmove( wk->m_val, _getDATAs( datp, FD_CRVAL_PN )->DP_VAL, 6 );
		memmove( wk->m_zei, _getDATAs( datp, FD_CRZEI_PN )->DP_VAL, 6 );

		wk->m_tekiyo = _getDATAs( datp, FD_TKY_PN )->DP_TKY;
		memmove( wk->m_taika, _getDATAs( datp, FD_CSKBN_PN )->SYDATA.TKsyohi, 6 );
		memmove( wk->m_invno, _getDATAs( datp, FD_CSKBN_PN )->SYDATA.TKinvno, SZ_INVNO);
		// 手形関係
		wk->m_rnumber = _getDATAs( datp, FD_TGNUM_PN )->DP_SNUM;
		wk->m_tdate	  = _getDATAs( datp, FD_TGDATE_PN )->DP_TGDAY;
		// 変動事由
		wk->m_hjiyu	  = _getDATAs( datp, FD_HJIYU_PN )->DP_BMN;
		wk->m_nbcd	  = _getDATAs( datp, FD_HJIYU_PN )->NAIBCD;
		// 付箋
		wk->m_dsign[7] &= (~0x0f);
		wk->m_dsign[7] |= _getDATAs( datp, FD_HUSEN_PN )->DP_HUSEN;
		wk->m_tag		= _getDATAs( datp, FD_HUSEN_PN )->DP_HCMNT;

		wk->m_dtype	=	0x10;
		n = 1;

		// 消費税仕訳作成
		return sydata_mak( dtbl0, wktbl, n, datp );

	}	//KIND_NYUKIN
	else if( IsKind() == KIND_SYUKKIN ) {
		wk = dtbl;
			
		// 日付セット
		setDATE_CREC( wk );
		// 伝票セット
		wk->m_cno = getDATA( DP_DENP_PN )->DP_DENP;
		if( m_DI.IsSyonum() ) {
			wk->m_skn = getDATA(DP_SYONUM_PN)->DP_SNUM;
		}
		else {
			wk->m_skn.Empty();
		}

		if( m_DI.IsBumon() ) {
			wk->m_dbmn = getDATA( DP_BMN_PN )->DP_BMN;
			wk->m_cbmn = wk->m_dbmn;
		}
		else {
			wk->m_dbmn = -1;
			wk->m_cbmn = -1;
		}

		if( m_DI.IsKoji() ) {
			wk->m_dkno = getDATA( DP_KOJI_PN )->DP_KOJI;
			wk->m_ckno = wk->m_dkno;
		}
		else {
			wk->m_dkno.Empty();
			wk->m_ckno.Empty();
		}

		wk->m_dbt = _getDATAs( datp, FD_DEBT_PN)->DP_CdNum;
		wk->m_dbr = _getDATAs( datp, FD_DEBT_PN)->DP_BRN;
		wk->m_cre = _getDATAs( datp, FD_CRED_PN)->DP_CdNum;
		wk->m_cbr = _getDATAs( datp, FD_CRED_PN)->DP_BRN;
			
		memmove( &wk->m_dsign[0], &_getDATAs( datp, FD_DSKBN_PN )->SYDATA._DSGN, SZ_DSGN );
		memmove( wk->m_val, _getDATAs( datp, FD_DBVAL_PN )->DP_VAL, 6 );
		memmove( wk->m_zei, _getDATAs( datp, FD_DBZEI_PN )->DP_VAL, 6 );

		wk->m_tekiyo = _getDATAs( datp, FD_TKY_PN )->DP_TKY;
		memmove( wk->m_taika, _getDATAs( datp, FD_DSKBN_PN )->SYDATA.TKsyohi, 6 );
		memmove( wk->m_invno, _getDATAs( datp, FD_CSKBN_PN )->SYDATA.TKinvno, SZ_INVNO);
		// 手形関係
		wk->m_rnumber = _getDATAs( datp, FD_TGNUM_PN )->DP_SNUM;
		wk->m_tdate	  = _getDATAs( datp, FD_TGDATE_PN )->DP_TGDAY;
		// 変動事由
		wk->m_hjiyu	  = _getDATAs( datp, FD_HJIYU_PN )->DP_BMN;
		wk->m_nbcd	  = _getDATAs( datp, FD_HJIYU_PN )->NAIBCD;
		// 付箋
		wk->m_dsign[7] &= (~0x0f);
		wk->m_dsign[7] |= _getDATAs( datp, FD_HUSEN_PN )->DP_HUSEN;
		wk->m_tag		= _getDATAs( datp, FD_HUSEN_PN )->DP_HCMNT;

		wk->m_dtype	=	0x20;
		n = 1;

		// 消費税仕訳作成
		return sydata_mak( dtbl0, wktbl, n, datp );
	}	//KIND_SYUKKIN


	return	0;
}


// 消費税仕訳作成
int CDnpBaseClass::sydata_mak( CDBINPDataRec *dtbl0, CDBINPDataRec *wktbl, int wktbl_n, struct _DP_DATA_LINE *datp )
{
int i, n, pn;
struct _DenpData *keybuf;
CDBINPDataRec crec, tmprec;

	tmprec.Reset();

	n = 0;
	for( i = 0 ; i < wktbl_n ; ++i )
	{
		if( wktbl_n == 2 ) {
			pn = (i == 0) ? FD_DSKBN_PN : FD_CSKBN_PN;
		}
		else {
			if( IsKind() == KIND_NYUKIN ) {
				pn = FD_CSKBN_PN;
			}
			else if( IsKind() == KIND_SYUKKIN ) {
				pn = FD_DSKBN_PN;
			}
			else {
				if( m_DI.km_syattr( (wktbl+i)->m_dbt ) )
						pn = FD_DSKBN_PN;
				else	pn = FD_CSKBN_PN;
			}
		}

		keybuf =  _getDATAs( datp, pn );
		crec = *(wktbl+i);

		// 04.24 /12 --- seq = -1だと SetCDBdata でデータセットされない。
		if( crec.m_seq < 0 ) {
			crec.m_seq = m_DI.m_dacnt + 1 + i;
		}

		// 消費税チェック
		if( ! m_pSy->IsSyohizeiMaster() )
		{
			// 非消費税仕訳
			*dtbl0 = crec;
			dtbl0++;
			++n;
		}
		else
		{

			// 消費税モジュール内の データクリアのため
			// 一旦 科目なし+SEQ 変更で で消費税 イニシャライズ
			tmprec.Reset();
			tmprec.m_seq	= crec.m_seq+1;
			tmprec.m_ddate	= crec.m_ddate;
			tmprec.m_mofs	= crec.m_mofs;
			tmprec.m_ksign	= crec.m_ksign;
			m_DI.DB_SyIniz( &tmprec );

			// crec には、今回の仕訳の変更消費税サインがセットされている
			m_DI.DB_SyIniz( &crec );

			BYTE tmp[32] = {0};
			if( memcmp( tmp, &crec.m_dsign[0], SZ_SYZDSGN )== 0 ) {
				struct _SY_MSG_PACK2 smpk;
				m_pSy->SyMsg( &smpk );

#ifdef NAIBU_VER2
				int old_nbcd = crec.m_nbcd;
#endif
				m_DI.GetCDBData( &crec );
#ifdef NAIBU_VER2
				crec.m_nbcd = old_nbcd;
#endif
			}

			*dtbl0 = crec;
			dtbl0++;
			n++;

		}
	}

	return n;
}




//---------------------------------------------------------
//	仕訳修正する際の工事番号 科目のチェック
//
//	return	-1 = 範囲チェックに引っかかり、修正する
//			 0 = OK / 修正せずに工事番号取消
//---------------------------------------------------------
int CDnpBaseClass::ModifyCheckKojikmk( CDBINPDataRec* drec, int cnt, int& chk_ln, int& chk_pn, BOOL bMsg )
{
	if( ! m_DI.IsKoji() )	return 0;

	if( m_pAsel->KJIKMK_OPT != OPT_KJIKMK)
		return 0;

	int	ln, j, st, ret;
	CDBINPDataRec	data, *pNew;
	int dc_sw = 0, clr_sgn = 0;

	CString msg, str;
	DWORD dwCode;

	BOOL bDataKoji = FALSE;

	if( IsKind() == KIND_FURIKAE ) {
		if( m_FuriMode ==  FURI_DATABMON ) {
			bDataKoji = TRUE;
		}
	}

	if( ! bMsg )	clr_sgn = 1;

	msg = "《工事番号チェック》\r\n";

	ret = 0;
	chk_ln = chk_pn = -1;

	for( ln = 1, j = 0; ln <= get_MaxLine(); ln++ ) {

		dc_sw = 0;

		str.Empty();

		if( m_DPscan.IsOriginData( ln, 0 ) ) {
			data	=	m_DPscan.GetOriginData( ln, 0 );
			pNew	=	drec + j;
			j++;

			if( ! pNew->m_dkno.IsEmpty() ) {
				if( ! m_KjChk.KojiKmkCheck( pNew->m_dbt ) ) {
					dwCode = _atoh( (LPSTR)(LPCTSTR)pNew->m_dbt );

					dc_sw |= 0x01;

					if( clr_sgn ) {
						pNew->m_dkno.Empty();
						dc_sw &= ~0x01;
					}
				}
			}

			if( ! pNew->m_ckno.IsEmpty() ) {
				if( ! m_KjChk.KojiKmkCheck( pNew->m_cre ) ) {
					dwCode = _atoh( (LPSTR)(LPCTSTR)pNew->m_cre );

					dc_sw |= 0x02;

					if( clr_sgn ) {
						pNew->m_ckno.Empty();
						dc_sw &= ~0x02;
					}
				}
			}

			if( ! clr_sgn && dc_sw ) {
				kojimsg_make( str, dc_sw, pNew );
				msg += str;

				if( chk_ln == -1 ) {
					if( bDataKoji ) {
						chk_ln = ln;
						chk_pn = (dc_sw&0x01) ? FD_DKOJI_PN : FD_CKOJI_PN;
					}
					else {
						chk_ln = 0;
						chk_pn = DP_KOJI_PN;
					}
				}
			}
		}

		if( m_DPscan.IsOriginData( ln, 1 ) ) {
			// 振替伝票で、単一仕訳でないとき
			if( IsNotFuriTanitsu( ln ) ) {
				data	=	m_DPscan.GetOriginData( ln, 1 );
				pNew	=	drec + j;
				j++;

				if( ! pNew->m_dkno.IsEmpty() ) {
					if( ! m_KjChk.KojiKmkCheck( pNew->m_dbt ) ) {
						dwCode = _atoh( (LPSTR)(LPCTSTR)pNew->m_dbt );

						dc_sw |= 0x01;

						if( clr_sgn ) {
							pNew->m_dkno.Empty();
							dc_sw &= ~0x01;
						}
					}
				}

				if( ! pNew->m_ckno.IsEmpty() ) {
					if( ! m_KjChk.KojiKmkCheck( pNew->m_cre ) ) {
						dwCode = _atoh( (LPSTR)(LPCTSTR)pNew->m_cre );

						dc_sw |= 0x02;

						if( clr_sgn ) {
							pNew->m_ckno.Empty();
							dc_sw &= ~0x02;
						}
					}
				}

				if( ! clr_sgn && dc_sw ) {
					kojimsg_make( msg, dc_sw, pNew );
					if( chk_ln == -1 ) {
						if( bDataKoji ) {
							chk_ln = ln;
							chk_pn = (dc_sw&0x01) ? FD_DKOJI_PN : FD_CKOJI_PN;
						}
						else {
							chk_ln = 0;
							chk_pn = DP_KOJI_PN;
						}
					}
				}
			}
		}
	}

	if( chk_ln != -1 ) {

		msg += "\r\n\r\n仕訳を修正しますか？\r\nいいえの場合、工事番号を取消して仕訳を書き込みます。";

	//	st = myICSMessageBox( (LPCSTR)msg, MB_YESNO,0,0,m_pDnpParent );
		st = IDNO;

		if( st == IDNO ) {

			for( ln = 1, j = 0; ln <= get_MaxLine(); ln++ ) {

				if( m_DPscan.IsOriginData( ln, 0 ) ) {
					data	=	m_DPscan.GetOriginData( ln, 0 );
					pNew	=	drec + j;
					j++;

					if( ! pNew->m_dkno.IsEmpty() ) {
						if( ! m_KjChk.KojiKmkCheck( pNew->m_dbt ) ) {
							pNew->m_dkno.Empty();
						}
					}

					if( ! pNew->m_ckno.IsEmpty() ) {
						if( ! m_KjChk.KojiKmkCheck( pNew->m_cre ) ) {
							pNew->m_ckno.Empty();
						}
					}
				}

				if( m_DPscan.IsOriginData( ln, 1 ) ) {
					// 振替伝票で、単一仕訳でないとき
					if( IsNotFuriTanitsu( ln ) ) {
						data	=	m_DPscan.GetOriginData( ln, 1 );
						pNew	=	drec + j;
						j++;

						if( ! pNew->m_dkno.IsEmpty() ) {
							if( ! m_KjChk.KojiKmkCheck( pNew->m_dbt ) ) {
								pNew->m_dkno.Empty();
							}
						}

						if( ! pNew->m_ckno.IsEmpty() ) {
							if( ! m_KjChk.KojiKmkCheck( pNew->m_cre ) ) {
								pNew->m_ckno.Empty();
							}
						}
					}
				}
			}
			clr_sgn = 1;
		}
		else {
			ret = -1;
		}
	}

	return	ret;
}


int CDnpBaseClass::ModifyCheckKojiTorok( CDBINPDataRec* drec, int cnt, int& chk_ln, int& chk_pn, BOOL bMsg )
{
	if( ! m_DI.IsKoji() ) return 0;

	if( m_pAsel->BMNCHK_OPT != OPT_BMNCHK )
		return 0;

	int	ln, j, ret;
	CDBINPDataRec	data, *pNew;
	int dc_sw = 0, auto_sgn = 0;

	CString msg, str;

	BOOL bDataBmon = FALSE;

	if( IsKind() == KIND_FURIKAE ) {
		if( m_FuriMode ==  FURI_DATABMON ) {
			bDataBmon = TRUE;
		}
	}

	if( ! bMsg )	auto_sgn = 1;
	vector<CString>	kojivec;

	ret = 0;
	chk_ln = chk_pn = -1;

	for( ln = 1, j = 0; ln <= get_MaxLine(); ln++ ) {

		dc_sw = 0;

		str.Empty();

		if( m_DPscan.IsOriginData( ln, 0 ) ) {
			data	=	m_DPscan.GetOriginData( ln, 0 );
			pNew	=	drec + j;
			j++;

			if( ! pNew->m_dkno.IsEmpty() ) {
				if( KojiTorokMessage( pNew->m_dkno, kojivec, bMsg ) ) {
					dc_sw |= 0x01;
				}
			}
			if( dc_sw == 0 ) {
				if( ! pNew->m_ckno.IsEmpty() ) {
					if( KojiTorokMessage( pNew->m_ckno, kojivec, bMsg ) ) {
						dc_sw |= 0x02;
					}
				}
			}
			if( ! auto_sgn && dc_sw ) {
				if( chk_ln == -1 ) {
					if( bDataBmon ) {
						chk_ln = ln;
						chk_pn = (dc_sw&0x01) ? FD_DKOJI_PN : FD_CKOJI_PN;
					}
					else {
						chk_ln = 0;
						chk_pn = DP_KOJI_PN;
					}
				}
				break;
			}
		}

		if( m_DPscan.IsOriginData( ln, 1 ) ) {
			// 振替伝票で、単一仕訳でないとき
			if( IsNotFuriTanitsu( ln ) ) {
				data	=	m_DPscan.GetOriginData( ln, 1 );
				pNew	=	drec + j;
				j++;


				if( ! pNew->m_dkno.IsEmpty() ) {
					if( KojiTorokMessage( pNew->m_dkno, kojivec, bMsg ) ) {
						dc_sw |= 0x01;
					}
				}
				if( dc_sw == 0 ) {
					if( ! pNew->m_ckno.IsEmpty() ) {
						if( KojiTorokMessage( pNew->m_ckno, kojivec, bMsg ) ) {
							dc_sw |= 0x02;
						}
					}
				}
				if( ! auto_sgn && dc_sw ) {
					if( chk_ln == -1 ) {
						if( bDataBmon ) {
							chk_ln = ln;
							chk_pn = (dc_sw&0x01) ? FD_DKOJI_PN : FD_CKOJI_PN;
						}
						else {
							chk_ln = 0;
							chk_pn = DP_KOJI_PN;
						}
					}
					break;
				}
			}
		}
	}

	if( chk_ln != -1 ) {
		ret = -1;
	}

	return	ret;
}


//伝票修正時の工事未入力・完成工事チェック

int CDnpBaseClass::ModifyCheckKoji( CDBINPDataRec* drec, int cnt, int& chk_ln, int& chk_pn, BOOL bMsg )
{
	if( ! m_DI.IsKoji() ) return 0;

	int	ln, j, ret;
	CDBINPDataRec	data, *pNew;
	int dc_sw = 0;
	int dbcr_sg;

	CString msg, str;
	CString		kno, cod;

	BOOL bDataBmon = FALSE;

	if( IsKind() == KIND_FURIKAE ) {
		if( m_FuriMode ==  FURI_DATABMON ) {
			bDataBmon = TRUE;
		}
	}

	ret = 0;
	chk_ln = chk_pn = -1;

	for( ln = 1, j = 0; ln <= get_MaxLine(); ln++ ) {

		str.Empty();

		if( m_DPscan.IsOriginData( ln, 0 ) ) {
			data	=	m_DPscan.GetOriginData( ln, 0 );
			pNew	=	drec + j;
			j++;

			if( ! pNew->m_dkno.IsEmpty() ) {
				kno =	pNew->m_dkno;
				cod	=	pNew->m_dbt;

				dbcr_sg = (IsKind() == KIND_FURIKAE) ? 0 : -1;
				if( koji_noinpcheck( kno, cod, dbcr_sg ) ) {
					ret = -1;	break;
				}

				if( koji_kanseicheck( kno, cod, dbcr_sg ) ) {
					ret = -2;	break;
				}
			}
			if( ret == 0 ) {
				if( ! pNew->m_ckno.IsEmpty() ) {
					kno =	pNew->m_ckno;
					cod	=	pNew->m_cre;

					dbcr_sg = (IsKind() == KIND_FURIKAE) ? 1 : -1;
					if( koji_noinpcheck( kno, cod, dbcr_sg ) ) {
						ret = -1;	break;
					}

					if( koji_kanseicheck( kno, cod, dbcr_sg ) ) {
						ret = -2;	break;
					}
				}
			}
		}

		if( m_DPscan.IsOriginData( ln, 1 ) ) {
			// 振替伝票で、単一仕訳でないとき
			if( IsNotFuriTanitsu( ln ) ) {
				data	=	m_DPscan.GetOriginData( ln, 1 );
				pNew	=	drec + j;
				j++;

				if( ! pNew->m_dkno.IsEmpty() ) {
					kno =	pNew->m_dkno;
					cod	=	pNew->m_dbt;

					dbcr_sg = (IsKind() == KIND_FURIKAE) ? 0 : -1;
					if( koji_noinpcheck( kno, cod, dbcr_sg ) ) {
						ret = -1;	break;
					}

					if( koji_kanseicheck( kno, cod, dbcr_sg ) ) {
						ret = -2;	break;
					}
				}
				if( ret == 0 ) {
					if( ! pNew->m_ckno.IsEmpty() ) {
						kno =	pNew->m_ckno;
						cod	=	pNew->m_cre;

						dbcr_sg = (IsKind() == KIND_FURIKAE) ? 1 : -1;
						if( koji_noinpcheck( kno, cod, dbcr_sg ) ) {
							ret = -1;	break;
						}

						if( koji_kanseicheck( kno, cod, dbcr_sg ) ) {
							ret = -2;	break;
						}
					}
				}
			}
		}
	}

	if( ret < 0 ) {
		if( IsKind() != KIND_FURIKAE || (IsKind() == KIND_FURIKAE && m_FuriMode == FURI_TANBMON) ) {
			chk_ln = -1;
			chk_pn = DP_KOJI_PN;
		}
		else {
			chk_ln = ln;
			chk_pn = (dbcr_sg == 0) ? FD_DKOJI_PN : FD_CKOJI_PN;
		}
	}

	return	ret;
}



//---------------------------------------------------------
//	仕訳修正する際の部門範囲のチェック
//
//	return	-1 = 範囲チェックに引っかかり、修正する
//			 0 = OK / 修正せずに部門取消
//---------------------------------------------------------
int CDnpBaseClass::ModifyCheckBmnHani( CDBINPDataRec* drec, int cnt, int& chk_ln, int& chk_pn, BOOL bMsg )
{
	if( ! m_DI.IsBumon() )	return 0;

	int	ln, j, st, ret;
	CDBINPDataRec	data, *pNew;
	int dc_sw = 0, clr_sgn = 0;

	CString msg, str;
	DWORD dwCode;

	BOOL bDataBmon = FALSE;

	if( IsKind() == KIND_FURIKAE ) {
		if( m_FuriMode ==  FURI_DATABMON ) {
			bDataBmon = TRUE;
		}
	}

	if( ! bMsg )	clr_sgn = 1;

	msg = "《部門範囲チェック》\r\n";

	ret = 0;
	chk_ln = chk_pn = -1;

	for( ln = 1, j = 0; ln <= get_MaxLine(); ln++ ) {

		dc_sw = 0;

		str.Empty();

		if( m_DPscan.IsOriginData( ln, 0 ) ) {
			data	=	m_DPscan.GetOriginData( ln, 0 );
			pNew	=	drec + j;
			j++;

			if( ! m_BmChk.BmnHaniCheck( pNew->m_dbmn, pNew->m_dbt ) ) {
				dwCode = _atoh( (LPSTR)(LPCTSTR)pNew->m_dbt );

				if( clr_sgn ) {
					pNew->m_dbmn = -1;
					dc_sw &= ~0x01;
				}
				else {
					dc_sw |= 0x01;

					if( (m_pZm->zvol->bm_isw & 0x10)) {
						if( dwCode <= SONEKICODE ) {
							pNew->m_dbmn = -1;
							dc_sw &= ~0x01;
						}
					}
				}
			}

			if( ! m_BmChk.BmnHaniCheck( pNew->m_cbmn, pNew->m_cre ) ) {
				dwCode = _atoh( (LPSTR)(LPCTSTR)pNew->m_cre );

				dc_sw |= 0x02;

				if( clr_sgn ) {
					pNew->m_cbmn = -1;
					dc_sw &= ~0x02;
				}
				else {
					if( (m_pZm->zvol->bm_isw & 0x10)) {
						if( dwCode <= SONEKICODE ) {
							pNew->m_cbmn = -1;
							dc_sw &= ~0x02;
						}
					}
				}
			}

			if( ! clr_sgn && dc_sw ) {
				bmnmsg_make( str, dc_sw, pNew );
				msg += str;

				if( chk_ln == -1 ) {
					if( bDataBmon ) {
						chk_ln = ln;
						chk_pn = (dc_sw&0x01) ? FD_DBMN_PN : FD_CBMN_PN;
					}
					else {
						chk_ln = 0;
						chk_pn = DP_BMN_PN;
					}
				}
			}
		}

		if( m_DPscan.IsOriginData( ln, 1 ) ) {
			// 振替伝票で、単一仕訳でないとき
			if( IsNotFuriTanitsu( ln ) ) {
				data	=	m_DPscan.GetOriginData( ln, 1 );
				pNew	=	drec + j;
				j++;

				if( ! m_BmChk.BmnHaniCheck( pNew->m_dbmn, pNew->m_dbt ) ) {
					dwCode = _atoh( (LPSTR)(LPCTSTR)pNew->m_dbt );

					dc_sw |= 0x01;

					if( clr_sgn ) {
						pNew->m_dbmn = -1;
						dc_sw &= ~0x01;
					}
					else {
						if( (m_pZm->zvol->bm_isw & 0x10)) {
							if( dwCode <= SONEKICODE ) {
								pNew->m_dbmn = -1;
								dc_sw &= ~0x01;
							}
						}
					}
				}

				if( ! m_BmChk.BmnHaniCheck( pNew->m_cbmn, pNew->m_cre ) ) {
					dwCode = _atoh( (LPSTR)(LPCTSTR)pNew->m_cre );

					dc_sw |= 0x02;

					if( clr_sgn ) {
						pNew->m_cbmn = -1;
						dc_sw &= ~0x02;
					}
					else {
						if( (m_pZm->zvol->bm_isw & 0x10)) {
							if( dwCode <= SONEKICODE ) {
								pNew->m_cbmn = -1;
								dc_sw &= ~0x02;
							}
						}
					}
				}

				if( ! clr_sgn && dc_sw ) {
					bmnmsg_make( msg, dc_sw, pNew );
					if( chk_ln == -1 ) {
						if( bDataBmon ) {
							chk_ln = ln;
							chk_pn = (dc_sw&0x01) ? FD_DBMN_PN : FD_CBMN_PN;
						}
						else {
							chk_ln = 0;
							chk_pn = DP_BMN_PN;
						}
					}
				}
			}
		}
	}

	if( chk_ln != -1 ) {

		msg += "\r\n\r\n仕訳を修正しますか？\r\nいいえの場合、部門を取消して仕訳を書き込みます。";

		st = myICSMessageBox( (LPCSTR)msg, MB_YESNO,0,0, m_pDnpParent );

		if( st == IDNO ) {

			for( ln = 1, j = 0; ln <= get_MaxLine(); ln++ ) {

				if( m_DPscan.IsOriginData( ln, 0 ) ) {
					data	=	m_DPscan.GetOriginData( ln, 0 );
					pNew	=	drec + j;
					j++;

					if( ! m_BmChk.BmnHaniCheck( pNew->m_dbmn, pNew->m_dbt ) ) {
						pNew->m_dbmn = -1;
					}
					if( ! m_BmChk.BmnHaniCheck( pNew->m_cbmn, pNew->m_cre ) ) {
						pNew->m_cbmn = -1;
					}
				}

				if( m_DPscan.IsOriginData( ln, 1 ) ) {
					// 振替伝票で、単一仕訳でないとき
					if( IsNotFuriTanitsu( ln ) ) {
						data	=	m_DPscan.GetOriginData( ln, 1 );
						pNew	=	drec + j;
						j++;

						if( ! m_BmChk.BmnHaniCheck( pNew->m_dbmn, pNew->m_dbt ) ) {
							pNew->m_dbmn = -1;
						}
						if( ! m_BmChk.BmnHaniCheck( pNew->m_cbmn, pNew->m_cre ) ) {
							pNew->m_cbmn = -1;
						}
					}
				}
			}
			clr_sgn = 1;
		}
		else {
			ret = -1;
		}
	}

	return	ret;
}

//---------------------------------------------------------
//	仕訳修正する際の枝番未入力のチェック
//
//	return	-1 = 枝番未入力チェックに引っかかり、修正する
//			 0 = OK / 修正せずにもどる
//---------------------------------------------------------
int CDnpBaseClass::ModifyCheckEdabanInput( CDBINPDataRec* drec, int cnt, int& chk_ln, int& chk_pn, BOOL bMsg )
{
	if( ! m_DI.IsEdaban() )	return 0;

	if( m_pAsel->BRN_OPT != OPT_RGSCHK /*&&m_pAsel->BRNKAK_OPT != OPT_BRNKAK*/)
		return 0;

	int	ln, j, st, ret;
	CDBINPDataRec	data, *pNew;
	int dc_sw = 0;

	CString msg, str, cod;
	struct _DenpData *debt, *cred;
	int		brn;

//	if( ! bMsg )	clr_sgn = 1;

	msg = "《枝番未入力チェック》\r\n";

	const int msgMax = 30;
	int	nMsg, msgCnt = 0;

	edamap1.clear();
	edamap2.clear();

	ret = 0;
	chk_ln = chk_pn = -1;

	for( ln = 1, j = 0; ln <= get_MaxLine(); ln++ ) {

		dc_sw = 0;

		str.Empty();

		if( m_DPscan.IsOriginData( ln, 0 ) ) {
			data	=	m_DPscan.GetOriginData( ln, 0 );
			pNew	=	drec + j;
			j++;

			if( !isSYOGT_CODE( pNew->m_dbt ) ) {
				if( pNew->m_dbr == -1 ) {
					// 枝番残高があるか？
					if( m_DI.DB_EdabanZanSearch( pNew->m_dbt, -1 ) == 0 ) {
						if( IsKind() != KIND_NYUKIN )
							dc_sw |= 0x01;
					}
				}
			}

			if( ! IsNotFuriTanitsu(ln) ) {
				if( !isSYOGT_CODE( pNew->m_cre ) ) {
					if( pNew->m_cbr == -1 ) {
						// 枝番残高があるか？
						if( m_DI.DB_EdabanZanSearch( pNew->m_cre, -1 ) == 0 ) {
							if( IsKind() != KIND_SYUKKIN )
								dc_sw |= 0x02;
						}
					}
				}
			}

			if( (pNew->m_dsign[0]&0x01) ) dc_sw = 0;

			if( dc_sw ) {
				if( (nMsg = edabanmsg_make( str, dc_sw, pNew, ln )) > 0 ) {
					edabanmsg_addjob( msg, str, nMsg, msgCnt, msgMax );
					if( chk_ln == -1 ) {
						chk_ln = ln;
						chk_pn = (dc_sw&0x01) ? FD_DEBT_PN : FD_CRED_PN;
					}
				}
			}
		}
		else {
			debt	=	NULL;

			if( IsKind() != KIND_NYUKIN ) {
				if( _getCND( ln, FD_DEBT_PN )->INP_sg ) {
					debt	=	_getDATA( ln, FD_DEBT_PN );
				}
			}

			if( debt ) {
				brn	=	debt->DP_BRN;
				cod	=	debt->DP_CdNum;
				if( ! isSYOGT_CODE( cod ) && brn == -1 ) {
					if( m_DI.DB_EdabanZanSearch( cod, -1 ) == 0 ) {
						dc_sw |= 0x01;
					}
				}
				if( dc_sw ) {
					data.m_dbt = cod;
					data.m_dbr = -1;

					if( (nMsg = edabanmsg_make( str, dc_sw, &data, ln )) > 0 ) {
						edabanmsg_addjob( msg, str, nMsg, msgCnt, msgMax );
						if( chk_ln == -1 ) {
							chk_ln = ln;
							chk_pn = FD_DEBT_PN;
						}
					}
				}
			}
		}
		dc_sw = 0;
		str.Empty();

		if( m_DPscan.IsOriginData( ln, 1 ) ) {
			// 振替伝票で、単一仕訳でないとき
			if( IsNotFuriTanitsu( ln ) ) {
				data	=	m_DPscan.GetOriginData( ln, 1 );
				pNew	=	drec + j;
				j++;

				if( !isSYOGT_CODE( pNew->m_cre ) ) {
					if( pNew->m_cbr == -1 ) {
						// 枝番残高があるか？
						if( m_DI.DB_EdabanZanSearch( pNew->m_cre, -1 ) == 0 ) {
							if( IsKind() != KIND_SYUKKIN )
								dc_sw |= 0x02;
						}
					}
				}

				if( (pNew->m_dsign[0]&0x01) ) dc_sw = 0;

				if( dc_sw ) {
					if( (nMsg = edabanmsg_make( str, dc_sw, pNew, ln )) > 0 ) {
						edabanmsg_addjob( msg, str, nMsg, msgCnt, msgMax );
						if( chk_ln == -1 ) {
							chk_ln = ln;
							chk_pn = (dc_sw&0x01) ? FD_DEBT_PN : FD_CRED_PN;
						}
					}
				}
			}
		}
		else {
			cred	=	NULL;
			if( IsKind() != KIND_SYUKKIN ) {
				if( _getCND( ln, FD_CRED_PN )->INP_sg ) {
					cred	=	_getDATA( ln, FD_CRED_PN );
				}
			}

			if( cred ) {
				brn	=	cred->DP_BRN;
				cod	=	cred->DP_CdNum;
				if( ! isSYOGT_CODE( cod ) && brn == -1 ) {
					if( m_DI.DB_EdabanZanSearch( cod, -1 ) == 0 ) {
						dc_sw |= 0x02;
					}
				}
			}
			if( dc_sw ) {
				data.m_cre = cod;
				data.m_cbr = -1;

				if( (nMsg = edabanmsg_make( str, dc_sw, &data, ln )) > 0 ) {
					edabanmsg_addjob( msg, str, nMsg, msgCnt, msgMax );
					if( chk_ln == -1 ) {
						chk_ln = ln;
						chk_pn = FD_CRED_PN;
					}
				}
			}
		}
	}

	if( bMsg ) {
		if( chk_ln != -1 ) {
			if( msgCnt > msgMax ) {
				str.Format( "\r\n(他 %d 件)", msgCnt-msgMax );
				msg += str;
			}
			msg += "\r\n\r\n仕訳を修正しますか？\r\nいいえの場合、枝番が未入力の状態で仕訳が書き込まれます。";

			st = myICSMessageBox( (LPCSTR)msg, MB_YESNO,0,0,m_pDnpParent );

			if( st == IDYES ) {
				ret = -1;
			}
		}
	}

	return	ret;
}



int CDnpBaseClass::ModifyCheckBmnTorok( CDBINPDataRec* drec, int cnt, int& chk_ln, int& chk_pn, BOOL bMsg )
{
	if( ! m_DI.IsBumon() )	return 0;

	int	ln, j, ret;
	CDBINPDataRec	data, *pNew;
	int dc_sw = 0, auto_sgn = 0;

	CString msg, str;

	BOOL bDataBmon = FALSE;

	if( IsKind() == KIND_FURIKAE ) {
		if( m_FuriMode ==  FURI_DATABMON ) {
			bDataBmon = TRUE;
		}
	}

	if( ! bMsg )	auto_sgn = 1;
	vector<int>	bmnvec;

	ret = 0;
	chk_ln = chk_pn = -1;

	for( ln = 1, j = 0; ln <= get_MaxLine(); ln++ ) {

		dc_sw = 0;

		str.Empty();

		if( m_DPscan.IsOriginData( ln, 0 ) ) {
			data	=	m_DPscan.GetOriginData( ln, 0 );
			pNew	=	drec + j;
			j++;

			if( pNew->m_dbmn >= 0 ) {
				if( BmnTorokMessage( pNew->m_dbmn, bmnvec, bMsg ) ) {
					dc_sw |= 0x01;
				}
			}
			if( dc_sw == 0 ) {
				if( pNew->m_cbmn >= 0 ) {
					if( BmnTorokMessage( pNew->m_cbmn, bmnvec, bMsg ) ) {
						dc_sw |= 0x02;
					}
				}
			}
			if( ! auto_sgn && dc_sw ) {
				if( chk_ln == -1 ) {
					if( bDataBmon ) {
						chk_ln = ln;
						chk_pn = (dc_sw&0x01) ? FD_DBMN_PN : FD_CBMN_PN;
					}
					else {
						chk_ln = 0;
						chk_pn = DP_BMN_PN;
					}
				}
				break;
			}
		}

		if( m_DPscan.IsOriginData( ln, 1 ) ) {
			// 振替伝票で、単一仕訳でないとき
			if( IsNotFuriTanitsu( ln ) ) {
				data	=	m_DPscan.GetOriginData( ln, 1 );
				pNew	=	drec + j;
				j++;

				if( pNew->m_dbmn >= 0 ) {
					if( BmnTorokMessage( pNew->m_dbmn, bmnvec, bMsg ) ) {
						dc_sw |= 0x01;
					}
				}
				if( dc_sw == 0 ) {
					if( pNew->m_cbmn >= 0 ) {
						if( BmnTorokMessage( pNew->m_cbmn, bmnvec, bMsg ) ) {
							dc_sw |= 0x02;
						}
					}
				}

				if( ! auto_sgn && dc_sw ) {
					if( chk_ln == -1 ) {
						if( bDataBmon ) {
							chk_ln = ln;
							chk_pn = (dc_sw&0x01) ? FD_DBMN_PN : FD_CBMN_PN;
						}
						else {
							chk_ln = 0;
							chk_pn = DP_BMN_PN;
						}
					}
					break;
				}
			}
		}
	}

	if( chk_ln != -1 ) {
		ret = -1;
	}

	return	ret;
}


int CDnpBaseClass::mdfy_bmn_noinp_check( int bmn, CString& code, int dbcr_sg, CString* pMsg )
{
	CString stmp;
	map<CString, int>::iterator it;
	int ret = 0;

	if( bmn_noinpcheck( bmn, code, dbcr_sg, &stmp ) ) {
		if( dbcr_sg == 0 ) {
			it = edamap1.find( code );
			if( it == edamap1.end() ) {
				edamap1.insert( make_pair( code, bmn ) );
				ret = 1;
			}
		}
		else {
			it = edamap2.find( code );
			if( it == edamap2.end() ) {
				edamap2.insert( make_pair( code, bmn ) );
				ret = 1;
			}
		}
	}

	if( ret ) {
		*pMsg = stmp;
	}
	return ret;
}


//---------------------------------------------------------
//	伝票修正する際の部門未入力のチェック
//
//	return	-1 = 部門未入力チェックに引っかかり、修正する
//			 0 = OK
//---------------------------------------------------------
int CDnpBaseClass::ModifyCheckBmnNoInp( CDBINPDataRec* drec, int cnt, int& chk_ln, int& chk_pn, BOOL bMsg )
{
	if( ! m_DI.IsBumon() )	return 0;

	int	ln, j, ret;
	CDBINPDataRec	data, *pNew;
	int dbcr_sg = 0;

	CString msg, str;

	BOOL bFuriData = FALSE;

	if( IsKind() == KIND_FURIKAE ) {
		if( m_FuriMode ==  FURI_DATABMON ) {
			bFuriData = TRUE;
		}
	}

	ret = 0;
	chk_ln = chk_pn = -1;

	const int msgMax = 30;
	int	msgCnt = 0;
	CString stmp;
	msg = "《部門未入力》\n\n";

	edamap1.clear();
	edamap2.clear();

	BOOL bDelData;

	for( ln = 1, j = 0; ln <= get_MaxLine(); ln++ ) {

		dbcr_sg = 0;

		str.Empty();

		if( m_DPscan.IsOriginData( ln, 0 ) ) {
			data	=	m_DPscan.GetOriginData( ln, 0 );
			pNew	=	drec + j;
			j++;

			dbcr_sg = (IsKind() == KIND_FURIKAE) ? 0 : -1;

			bDelData = (pNew->m_dsign[0]&0x01) ? TRUE : FALSE;

			if( pNew->m_dbmn < 0 && ! bDelData ) {
				if( mdfy_bmn_noinp_check( pNew->m_dbmn, pNew->m_dbt, dbcr_sg, &stmp ) ) {
					msgCnt++;
					if( msgCnt <= msgMax ) {	
						msg += stmp;
						msg += "\r\n";
					}

					if( bFuriData ) {
						if( chk_pn == -1 )	chk_pn = FD_DBMN_PN;
					}
					else {
						if( chk_pn == -1 )	chk_pn = DP_BMN_PN;
					}

					if( chk_ln == -1 )	chk_ln = ln;

				}
			}

			//貸方
			if( pNew->m_cbmn < 0  && ! bDelData ) {
				dbcr_sg = (IsKind() == KIND_FURIKAE) ? 1 : -1;

				if( mdfy_bmn_noinp_check( pNew->m_cbmn, pNew->m_cre, dbcr_sg, &stmp ) ) {
					msgCnt++;
					if( msgCnt <= msgMax ) {	
						msg += stmp;
						msg += "\r\n";
					}
					if( bFuriData ) {
						if( chk_pn == -1 )	chk_pn = FD_CBMN_PN;
					}
					else {
						if( chk_pn == -1 )	chk_pn = DP_BMN_PN;
					}

					if( chk_ln == -1 )	chk_ln = ln;
				}
			}
		}

		if( m_DPscan.IsOriginData( ln, 1 ) ) {
			// 振替伝票で、単一仕訳でないとき
			if( IsNotFuriTanitsu( ln ) ) {
				data	=	m_DPscan.GetOriginData( ln, 1 );
				pNew	=	drec + j;
				j++;

				bDelData = (pNew->m_dsign[0]&0x01) ? TRUE : FALSE;

				if( pNew->m_dbmn < 0 && ! bDelData ) {
					dbcr_sg = (IsKind() == KIND_FURIKAE) ? 0 : -1;

					if( mdfy_bmn_noinp_check( pNew->m_dbmn, pNew->m_dbt, dbcr_sg, &stmp ) ) {
						msgCnt++;
						if( msgCnt <= msgMax ) {	
							msg += stmp;
							msg += "\r\n";
						}

						if( bFuriData ) {
							if( chk_pn == -1 )	chk_pn = FD_DBMN_PN;
						}
						else {
							if( chk_pn == -1 )	chk_pn = DP_BMN_PN;
						}

						if( chk_ln == -1 )	chk_ln = ln;

					}
				}

				//貸方
				if( pNew->m_cbmn < 0 && ! bDelData ) {
					dbcr_sg = (IsKind() == KIND_FURIKAE) ? 1 : -1;

					if( mdfy_bmn_noinp_check( pNew->m_cbmn, pNew->m_cre, dbcr_sg, &stmp ) ) {
						msgCnt++;
						if( msgCnt <= msgMax ) {	
							msg += stmp;
							msg += "\r\n";
						}

						if( bFuriData ) {
							if( chk_pn == -1 )	chk_pn = FD_CBMN_PN;
						}
						else {
							if( chk_pn == -1 )	chk_pn = DP_BMN_PN;
						}

						if( chk_ln == -1 )	chk_ln = ln;
					}
				}
			}
		}
	}
	if( bMsg ) {
		if( chk_ln != -1 ) {
			if( msgCnt > msgMax ) {
				str.Format( "\r\n(他 %d 件)", msgCnt-msgMax );
				msg += str;
			}
			msg += "\r\n\r\n仕訳を修正しますか？\r\nいいえの場合、部門が未入力の状態で仕訳が書き込まれます。";

			int st = myICSMessageBox( (LPCSTR)msg, MB_YESNO,0,0,m_pDnpParent );

			if( st == IDYES ) {
				ret = -1;
			}
		}
	}

	return	ret;
}


//
//	対価 データかどうかのチェック
//
int CDnpBaseClass::TaikaInputCheck( int ln, int syzpn )
{
	struct _DenpData *syzbuf;

	syzbuf = _getDATA( ln, syzpn );

	int ret = 0;

	if( _joto_data( syzbuf->SYDATA._DSGN ) ) {
		if( l_test( syzbuf->SYDATA.TKsyohi ) != 0 )
			ret++;
	}

	return ret;
}



// 工事番号 科目 メッセージ作成
int CDnpBaseClass::kojimsg_make( CString& msg, int dc_sw, CDBINPDataRec* drec )
{
	char buf[64];
	char km[32];
	DBKNREC* pKn;
	CString str;

	if( dc_sw & 0x01 ) {
		if( ( pKn = DB_PjisToKnrec( drec->m_dbt, m_pZm ) ) == NULL )
			return -1;

		::ZeroMemory( km, sizeof km );
		if( IsKind() == KIND_FURIKAE )	strcpy_s( km, sizeof km,  "借方" );

		set_codestring( buf, sizeof buf, (char*)(LPCTSTR)drec->m_dkno, m_pZm->zvol->kjcol );
		str.Format( "\r\n%s科目「%.14s」に、工事番号『%s』は設定できません。", km, pKn->knnam, buf );
		msg += str;
	}

	if( dc_sw & 0x02 ) {
		if( ( pKn = DB_PjisToKnrec( drec->m_cre, m_pZm ) ) == NULL )
			return -1;

		::ZeroMemory( km, sizeof km );
		if( IsKind() == KIND_FURIKAE )	strcpy_s( km, sizeof km,  "貸方" );

		set_codestring( buf, sizeof buf, (char*)(LPCTSTR)drec->m_ckno, m_pZm->zvol->kjcol );
		str.Format( "\r\n%s科目「%.14s」に、工事番号『%s』は設定できません。", km, pKn->knnam, buf );
		msg += str;
	}

	return 0;
}



// 部門範囲メッセージ作成
int CDnpBaseClass::bmnmsg_make( CString& msg, int dc_sw, CDBINPDataRec* drec )
{
	char buf[64];
	char km[32];
	DBKNREC* pKn;
	CString str;

	if( dc_sw & 0x01 ) {
		if( ( pKn = DB_PjisToKnrec( drec->m_dbt, m_pZm ) ) == NULL )
			return -1;

		::ZeroMemory( km, sizeof km );
		if( IsKind() == KIND_FURIKAE )	strcpy_s( km, sizeof km,  "借方" );

		set_codestr( buf, sizeof buf, drec->m_dbmn, m_pZm->zvol->edcol );
		str.Format( "\r\n%s科目「%.14s」に、部門番号『%s』は設定できません。", km, pKn->knnam, buf );
		msg += str;
	}

	if( dc_sw & 0x02 ) {
		if( ( pKn = DB_PjisToKnrec( drec->m_cre, m_pZm ) ) == NULL )
			return -1;

		::ZeroMemory( km, sizeof km );
		if( IsKind() == KIND_FURIKAE )	strcpy_s( km, sizeof km,  "貸方" );

		set_codestr( buf, sizeof buf, drec->m_cbmn, m_pZm->zvol->edcol );
		str.Format( "\r\n%s科目「%.14s」に、部門番号『%s』は設定できません。", km, pKn->knnam, buf );
		msg += str;
	}

	return 0;
}


// 枝番未入力メッセージ作成
int CDnpBaseClass::edabanmsg_make( CString& msg, int dc_sw, CDBINPDataRec* drec, int ln )
{
	char km[32];
	DBKNREC* pKn;
	CString str;
	int rval = 0;

	map<CString, int>::iterator it;

	if( dc_sw & 0x01 ) {
		if( ( pKn = DB_PjisToKnrec( drec->m_dbt, m_pZm ) ) == NULL )
			return -1;

		it = edamap1.find( drec->m_dbt );
		if( it == edamap1.end() ) {
			::ZeroMemory( km, sizeof km );
			if( IsKind() == KIND_FURIKAE )	strcpy_s( km, sizeof km,  "借方" );

			str.Format( "\r\n%s科目「%.14s」に、枝番が入力されていません！", km, pKn->knnam );
			msg += str;
			edamap1.insert( make_pair(drec->m_dbt, ln) );
			rval++;
		}
	}

	if( dc_sw & 0x02 ) {
		if( ( pKn = DB_PjisToKnrec( drec->m_cre, m_pZm ) ) == NULL )
			return -1;

		it = edamap2.find( drec->m_cre );
		if( it == edamap2.end() ) {
			::ZeroMemory( km, sizeof km );
			if( IsKind() == KIND_FURIKAE )	strcpy_s( km, sizeof km,  "貸方" );

			str.Format( "\r\n%s科目「%.14s」に、枝番が入力されていません！", km, pKn->knnam );
			msg += str;
			edamap2.insert( make_pair(drec->m_cre, ln) );
			rval++;
		}
	}

	return rval;

#ifdef CLOSE
	char km[32];
	DBKNREC* pKn;
	CString str;

	if( dc_sw & 0x01 ) {
		if( ( pKn = DB_PjisToKnrec( drec->m_dbt, m_pZm ) ) == NULL )
			return -1;

		::ZeroMemory( km, sizeof km );
		if( IsKind() == KIND_FURIKAE )	strcpy_s( km, sizeof km,  "借方" );

		str.Format( "\r\n%s科目「%.14s」に、枝番が入力されていません！", km, pKn->knnam );
		msg += str;
	}

	if( dc_sw & 0x02 ) {
		if( ( pKn = DB_PjisToKnrec( drec->m_cre, m_pZm ) ) == NULL )
			return -1;

		::ZeroMemory( km, sizeof km );
		if( IsKind() == KIND_FURIKAE )	strcpy_s( km, sizeof km,  "貸方" );

		str.Format( "\r\n%s科目「%.14s」に、枝番が入力されていません！", km, pKn->knnam );
		msg += str;
	}
#endif

	return 0;
}

//枝番メッセージを最大数(30)まで作成する
void CDnpBaseClass::edabanmsg_addjob( CString& addmsg, CString edamsg, int nMsg, int& MsgCnt, const int MsgMax )
{
	int add_sgn = 0;

	MsgCnt += nMsg;
	if( (MsgCnt-nMsg) < MsgMax ) {
		if( nMsg == 2 && (MsgCnt-1) == MsgMax ) {
			int idx = edamsg.Find( "貸方科目" );
			if( idx != -1 ) {
				addmsg += edamsg.Left( idx );
				add_sgn = 1;
			}
		}
		if( ! add_sgn ) {
			addmsg += edamsg;
		}
	}
}


// 振替伝票で、単一仕訳でないとき
BOOL CDnpBaseClass::IsNotFuriTanitsu( int ln ) 
{ 
	BOOL bRet = FALSE;

	if( ! SyogSiwakeMode() ) {	//諸口仕訳詰める。
		if( !(IsKind() == KIND_FURIKAE && !m_DPscan.IsFukugoSiwake( ln )) ) {
			bRet = TRUE;
		}
	}
	else {
		if( IsKind() == KIND_FURIKAE ) {
			if( ! m_DPscan.IsTanitsuSiwake( ln ) ) {
				bRet = TRUE;
			}
		}
		else {
			bRet = TRUE;
		}
	}
	return bRet;
}



// 部門コード 範囲チェック用
BOOL CDnpBaseClass::bmncode_todata2( CDBINPDataRec* data, int number, int dc_sw )
{
	int hchk;
	hchk = 0;

	return bmncode_todata( data, number, dc_sw, hchk );
}

//
// 科目・摘要 選択欄のクリック処理
//
void CDnpBaseClass::SelectedIcsselctrl(short position, long Number, int place )
{

	if (show_flg == 0) {
		return;
	}

	//CString sss;
	//sss.Format("SelectedIcsselctrl 選択欄のクリック処理  show_flg = %d", show_flg);
	//MyTrace(sss);
	CDBipTKREC tkrec;
	int dcsw, pn;
	struct _KamokuData check={0}, ans={0};
	char	buf[64] = {0};
	CString temp;

	if( m_DI.IsConFirmMaster() )	return;

	if( m_SelDispPN == BVW_SYZ_PN && place )
	{
		if( m_pSyzInp )
		{
			int sw = m_pSyzInp->SetSkbnSelNo( Number );	// 消費税区分選択番号受付

			//選択後次へ移動
		//	if( !sw )	m_pSyzInp->set_nextpos();
		//	else		m_pSyzInp->set_nowfocus();
		}
		return;
	}

	pn = get_nowpn();
//	if( getCND( pn )->EDT_sg == 2 )
//		return;

	char	sgn;
	IsTransLine( get_nowln(), sgn );
	if( sgn > 1 )	return;

	if( FuriSyogPosition( get_dataline(), pn ) )
		return;

	if( pn == FD_DEBT_PN || pn == FD_CRED_PN )
	{
		dcsw  = ((pn == FD_DEBT_PN) ? 0 : 1 );

		check = getDATA( pn )->DP_stKMK;

		// 科目ｺｰﾄﾞ or 番号 02.20/02
		if( bKamokuCode )
			sprintf_s( check.kd_code, sizeof check.kd_code, "%d", position + (place ? 9 : 1) );
		else
			sprintf_s( check.kd_code, sizeof check.kd_code, "%d", Number );	// 番号

		check.kd_sgn = 1;
		check.kd_eda = -1;

		if( m_Seldata.InputItemCheck( ans, check, dcsw, TRUE ) == TRUE )
		{
			if( kana_sysflg ) {
				//複数カナ
				HWND	hWnd;
				HIMC	hImc;
				hWnd = GetKamokuCtrlHWND();
				if( hWnd != NULL ) {
					hImc = ImmGetContext(hWnd);
					ImmNotifyIME(hImc, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
					ImmReleaseContext(hWnd, hImc);
				}
			}

			if( ! isSYOGT_CODE( ans.kd_code ) )
				ItemInp( get_nowln(), pn, 0, &ans );
		}
	}
	else if( pn == FD_TKY_PN && place )
	{
		//摘要選択
		temp.Format("%d", Number);
		m_PRO_OK = FALSE;

		tekiyo_selset( (char *)(LPCTSTR)temp, FALSE );
		m_PRO_OK = TRUE;
		
		// set_focusしない
		return;
	}
	else if( (pn == FD_DBMN_PN || pn == FD_CBMN_PN || pn == DP_BMN_PN) && place )
	{
		// 部門選択
		CDBINPDataRec rec;
		dcsw  = ((pn == FD_CBMN_PN) ? 1 : 0 );
		int bmn;
		if( bmncode_todata2( &rec, Number, dcsw ) != FALSE ) {
			bmn = dcsw ? rec.m_cbmn : rec.m_dbmn;
			getDATA( pn )->DP_BMN = bmn;
			getCND(pn)->INP_sg = TRUE;

			struct _SET_KMKETC sk;
			sk.bmn = bmn;
			sk.brn = -1;
			sk.etc.Empty();

			GetBmnEtc( &sk );

			::ZeroMemory( getCND(pn)->ETC_msg, sizeof getCND(pn)->ETC_msg );
			getCND(pn)->ETC_col = 0;
			GetNowDnpDialog()->SetBmon( get_nowln(), pn, bmn, NULL );

			VARIANT var;
			set_codestr( buf, sizeof buf, bmn, m_DI.m_pZm->zvol->bmcol );	// ゼロをつめる
			var.pbVal = (BYTE*)buf;
			DBdata_set( GetNowDnpDialog(), IDC_ICSDBEDT_BMON, &var, ICSDBEDT_TYPE_STRING, 0 );

			if( ! sk.etc.IsEmpty() ) {
				bmnkoji_to_tekiyo(&sk.etc);
			}
		}
	}
	else if( (pn == FD_DKOJI_PN || pn == FD_CKOJI_PN || pn == DP_KOJI_PN) && place )
	{
		// 工事選択
		CDBINPDataRec rec;
		dcsw  = ((pn == FD_CKOJI_PN) ? 1 : 0 );
		CString	kno, prev_kno;
		int	kj_sgn = 0;

		if( kojicode_todata( &rec, NULL, Number, dcsw ) != FALSE ) {
			if( pn == FD_DKOJI_PN || pn == FD_CKOJI_PN ) {
				prev_kno = getDATA( pn )->DP_KOJI;
			}

			kno = dcsw ? rec.m_ckno : rec.m_dkno;
			kno.TrimRight();
			strcpy_s( (char*)getDATA( pn )->DP_KOJI, sizeof getDATA( pn )->DP_KOJI, kno );
			getCND(pn)->INP_sg = TRUE;

			//カナ検索用にDBEDIT を重ねているので、
			//Diag に 名称を先に表示しておく
			struct _SET_KMKETC sk;
			sk.bmn = -1;
			sk.kno = kno;
			sk.brn = -1;
			
			if( GetKojiEtc( &sk ) ) {
				strcpy_s( getCND(pn)->ETC_msg, sizeof getCND(pn)->ETC_msg, sk.etc );
				getCND(pn)->ETC_col = sk.etc_col;
				GetNowDnpDialog()->SetKoji( get_nowln(), pn, kno, &sk );
				kj_sgn = 1;
			}
			else {
				::ZeroMemory( getCND(pn)->ETC_msg, sizeof getCND(pn)->ETC_msg );
				getCND(pn)->ETC_col = 0;
				GetNowDnpDialog()->SetKoji( get_nowln(), pn, kno, NULL );
			}

			VARIANT var;
			set_codestring( buf, sizeof buf, (char*)(LPCTSTR)kno, m_DI.m_pZm->zvol->kjcol );	// ゼロをつめる
			var.pbVal = (BYTE*)buf;
			DBdata_set( GetNowDnpDialog(), IDC_ICSDBEDT_KOJI, &var, ICSDBEDT_TYPE_STRING, 0 );

			//工事名称を仕訳摘要へ
			if( pn == FD_DKOJI_PN || pn == FD_CKOJI_PN ) {
				if( kno != prev_kno ) {
					if( ! kj_sgn ) {
						if( _set_kojietc( &sk ) ) {
							kj_sgn = 1;
						}
					}
					if( kj_sgn ) {
						bmnkoji_to_tekiyo( &sk.etc, 1 );
					}
				}
			}
		}
		// set_focus しない
		return;
	}
	else if( pn == FD_HJIYU_PN ) {
		_hjiyu_selectsub( Number, 1, NULL );
		return;
	}
	else if( pn == FD_DSYOG_PN || pn == FD_CSYOG_PN ) {
		if (!IsInputCharEnable()) return;

		dcsw  = ((pn == FD_DSYOG_PN) ? 0 : 1 );

		struct	_KamokuData kd_chk = {0};

		sprintf_s( kd_chk.kd_code, sizeof kd_chk.kd_code, "%d", Number );	// 番号

		kd_chk.kd_sgn = 1;
		kd_chk.kd_eda = -1;

		if( m_Seldata.InputItemCheck( ans, kd_chk, dcsw, TRUE ) == TRUE )
		{
			getDATA( pn )->DP_cBRN = ans.kd_eda;
			getCND(pn)->INP_sg = TRUE;

			VARIANT var;
			set_codestr(buf, sizeof buf, ans.kd_eda, m_pZm->zvol->edcol);
			var.pbVal = (BYTE*)buf;
			DBdata_set(GetNowDnpDialog(), IDC_ICSDBEDT_SYOBRN, &var, ICSDBEDT_TYPE_STRING, 0);
			GetNowDnpDialog()->SetSyogBrn( get_nowln(), pn, ans.kd_eda );
		}
	}

	set_focus( get_nowpn() );
}


void CDnpBaseClass::SetSkbnLineDisp()
{
	struct _SY_MSG_PACK2 smpk;
	m_pSy->SyMsg( &smpk );
	CString txt;
	txt = m_pSy->Sy_get_message( m_pZm->dbdata, SYGET_TYPE );

	int ln, pn, zeipn, dt_ln;
	ln		= get_nowln();
	pn		= get_nowpn();
	dt_ln	= get_dataline( ln );

	struct _DenpData* keybuf;
	keybuf = getDATA(pn);

	if( IsScanLine( dt_ln )) {
		CDBINPDataRec	dtemp;

		int zei_pn = -1;
		int dc_sw;

		if( IsKind() == KIND_FURIKAE ) {
			switch( pn ) {
			case FD_DBVAL_PN:
			case FD_DBZEI_PN:
			case FD_DBMN_PN	:
			case FD_DKOJI_PN:
			case FD_DEBT_PN	:
			case FD_DSYOG_PN:
			case FD_DSKBN_PN:
				zei_pn = FD_DSKBN_PN;	break;
			case FD_CRVAL_PN:
			case FD_CRZEI_PN:
			case FD_CBMN_PN	:
			case FD_CKOJI_PN:
			case FD_CRED_PN	:
			case FD_CSYOG_PN:
			case FD_CSKBN_PN:
				zei_pn = FD_CSKBN_PN;	break;
			}
		}
		else if( IsKind() == KIND_SYUKKIN ) {
			zei_pn	=	FD_DSKBN_PN;
		}
		else {
			zei_pn	=	FD_CSKBN_PN;
		}

		if( zei_pn != -1 ) {
			dc_sw = (zei_pn == FD_CSKBN_PN) ? 1 : 0;
			dtemp	=	m_DPscan.GetOriginData( dt_ln, dc_sw );
			CDBINPDataRec	newdrec;
			m_DI.GetCDBData( &newdrec );

			if( m_DI.isSyzdiff_data( &newdrec, &dtemp ) ) {
				keybuf->SYDATA.sy_chg = 1;
			//	SYZMDFY_BIT.onbitn( dtemp.m_seq - 1);
			}
		}
	}
	SRCTrace;
	PutsSyz( ln, pn, (char*)(LPCTSTR)txt, keybuf->SYDATA.sy_chg );

	memcpy( keybuf->SYDATA._DSGN, &m_pZm->dbdata->dsign[0], sizeof keybuf->SYDATA._DSGN );

	// 12.24 /13
	strcpy_s( keybuf->SYDATA.SKBNtxt, sizeof keybuf->SYDATA.SKBNtxt, txt );
	//インボイス対応
	sprintf_s(keybuf->SYDATA.TKinvno, sizeof keybuf->SYDATA.TKinvno, "%s", m_pZm->dbdata->invno);

	MyTrace("SetSkbnLineDisp keybuf->SYDATA.TKinvno = %s\n", keybuf->SYDATA.TKinvno);

	int swk, ritu, zei, uri, tai, toku;
	swk = ritu = zei = uri = tai = toku = 0;

	swk		= smpk.SY_MSG_SWKBN->sel_no;
	ritu	= smpk.SY_MSG_RITU->sel_no;
	zei		= smpk.SY_MSG_ZEI->sel_no;
	uri		= smpk.SY_MSG_URISIRE->sel_no;
	toku	= smpk.SY_MSG_TOKTEI->sel_no;
	tai		= smpk.SY_MSG_TAIKAKBN->sel_no;

	if( swk )	keybuf->SYDATA.swk_sw = smpk.SY_MSG_SWKBN->msg_tbl[swk-1]->code;
	else		keybuf->SYDATA.swk_sw = 0;
	
	if( ritu )	keybuf->SYDATA.ritu_sw = smpk.SY_MSG_RITU->msg_tbl[ritu-1]->code;
	else		keybuf->SYDATA.ritu_sw = 0;

	if( zei )	keybuf->SYDATA.zeikbn_sw = smpk.SY_MSG_ZEI->msg_tbl[zei-1]->code;
	else		keybuf->SYDATA.zeikbn_sw = 0;

	if( uri )	keybuf->SYDATA.uri_sw = smpk.SY_MSG_URISIRE->msg_tbl[uri-1]->code;
	else		keybuf->SYDATA.uri_sw = 0;
	// 特定収入
	if( toku )	keybuf->SYDATA.toku_sw = smpk.SY_MSG_TOKTEI->msg_tbl[toku-1]->code;
	else		keybuf->SYDATA.toku_sw = 0;

	if( tai )	keybuf->SYDATA.kts_sw = smpk.SY_MSG_TAIKAKBN->msg_tbl[tai-1]->code;
	else		keybuf->SYDATA.kts_sw = 0;

	// 税額 再表示
	char	zeibf[6] = {0};
	char*	pZei = NULL;
	int index, sgn = 0;
	int sotomen = 0;
	// 有価証券非課税譲渡
	if( m_pSy->IsTaikaData( m_pZm->dbdata ) == 2 )	sgn = 1;

	zeipn = (pn==FD_DSKBN_PN) ? FD_DBZEI_PN : FD_CRZEI_PN;

	l_input( zeibf, (char*)(LPCTSTR)smpk.SY_MSG_SYZEI );	// 税額をセット
	memmove( getDATA( zeipn )->DP_VAL, zeibf, 6 );

	// 消費税仕訳
	if( m_DI.is_syohizeisiwake( m_pZm->dbdata ) ) {
		_getCND( dt_ln, zeipn )->INP_sg = TRUE;
		pZei = zeibf;

		if( m_DI.is_sotomensiwake(m_pZm->dbdata) ) {
			sotomen = 1;
		}
	}
	else {
		sgn = 2;
		_getCND( dt_ln, zeipn )->INP_sg = FALSE;
		pZei = NULL;
	}
	_getCND( dt_ln, zeipn )->INP_type = sgn;
	set_sotomen_sign(dt_ln, zeipn, sotomen);

	// 消費税が、輸入仕入 に変更になったか？
	index = (pn==FD_DSKBN_PN) ? 0 : 1;

	if( m_pSyzInp->GetSyzswkKbn() != CD_YUNYU ) {
		if( (m_pZm->dbdata->dsign[0]&0x40) && (m_pZm->dbdata->dsign[2]&0x0f) == 0x01 && 
			m_pZm->dbdata->dsign[4] == 4 ) {
			m_bSyzYunyu[index] = (m_pAsel->ZEIMDFY_OPT == OPT_ZEIMDFY) ? TRUE : FALSE;
		}
		else {
			m_bSyzYunyu[index] = FALSE;
		}
	}
	// 現在レコードの 対価 をバッファにセットする
	for( int n = 0; n < m_SYdataCnt; n++  ) {
		if( m_CREC[n].m_seq == m_pZm->dbdata->seq ) {
			m_DI.GetCDBData( &m_CREC[n] );
			memcpy( keybuf->SYDATA.TKsyohi, m_CREC[n].m_taika, sizeof m_CREC[n].m_taika );
			memcpy( keybuf->SYDATA.TKinvno, m_CREC[n].m_invno, SZ_INVNO);

			MyTrace("SetSkbnLineDisp(2) keybuf->SYDATA.TKinvno = %s\n", keybuf->SYDATA.TKinvno);
		}
	}

	GetNowDnpDialog()->SetVal( ln/*get_nowln()*/, zeipn, pZei, sgn );

	// 単一仕訳の場合、借方・貸方の金額を揃える
	SetTandokuSameValue( ln, pn );

	total_dsp();
}



BOOL CDnpBaseClass::IsEnableToolBar( int nID )
{
	BOOL bRet = TRUE;

	switch( nID ) {
	case ID_TLBAR_F3:
		bRet = EnableButtonF3();	break;
	case ID_TLBAR_F4:
		bRet = EnableButtonF4();	break;
	case ID_TLBAR_F12:
		bRet = EnableButtonF12();	break;
	case ID_TLBAR_INS:
		bRet = EnableButtonIns();	break;
	case ID_TLBAR_DEL:
		bRet = EnableButtonDel();	break;
	case ID_TLBAR_F7:
		bRet = EnableButtonF7();	break;
	}

	return bRet;
}


BOOL CDnpBaseClass::EnableButtonF3()
{
	BOOL ret = TRUE;
	if( m_DI.IsConFirmMaster() )	ret = FALSE;

	return ret;
}

BOOL CDnpBaseClass::EnableButtonF4()
{
	BOOL ret = TRUE;
	
	if( m_DI.IsConFirmMaster() )
		ret = FALSE;
	else if( IsJob() == JOB_SCAN ) {
		int pn;
		pn = get_nowpn();
		ret = FALSE;
		switch( pn ) {
		case DP_DENP_PN:
		case DP_MM_PN:
		case DP_DD_PN:
		case DP_BMN_PN:
		case DP_KOJI_PN:
		case FD_DSKBN_PN:
		case FD_CSKBN_PN:
			break;
		default:
			if( IsScanLine( get_dataline()/*get_nowln()*/ ) ) {
				if( IsKind() == KIND_FURIKAE ) {
					if( !m_DPscan.IsFukugoSiwake( get_dataline()/*get_nowln()*/ ) ) {
						ret = TRUE;
					}
				}
				else {
					ret = TRUE;
				}
			}
			break;
		}
	}
	return ret;

}


BOOL CDnpBaseClass::EnableButtonF12()
{
	BOOL ret = TRUE;
	if( m_DI.IsConFirmMaster() )
		return FALSE;

	if( IsJob() == JOB_SCAN && IsKind() == KIND_FURIKAE && IsScanLine( get_dataline()/*get_nowln()*/ ) ) {
		if( IsMode() == MODE_AUTO ) {
			IsMode()	=	MODE_NORMAL;
			GetNowDnpDialog()->AutoSiwake( IsMode() );
		}
		return FALSE;
	}

	if( get_nowpn() == FD_TKY_PN ) {
		return FALSE;
	}
	return ret;
}


BOOL CDnpBaseClass::EnableButtonIns()
{
	if( m_DI.IsConFirmMaster() || ! m_DI.IsMasterType(MST_INSERT) ) {
		return FALSE;
	}

	int ln = get_nowln();
	int dt_ln = get_dataline( ln );
	int pn = get_nowpn();

	if( ln == -1 || pn < FD_DBVAL_PN ) {
		return FALSE;
	}

	return TRUE;
}


BOOL CDnpBaseClass::EnableButtonDel()
{
	if( m_DI.IsConFirmMaster() ) {
		return FALSE;
	}

	int ln = get_nowln();
	int dt_ln = get_dataline( ln );
	int pn = get_nowpn();

	if( ln == -1 || pn < FD_DBVAL_PN ) {
		return FALSE;
	}
	CDnpBaseDlg* pDlg = GetNowDnpDialog();

	BOOL ret = FALSE;

	if( IsKind() == KIND_FURIKAE  ) {
		if( IsJob() == JOB_SCAN ) {
			if( ! pDlg->dataline_input( dt_ln ) ) {
				ret = TRUE;
			}
		}
		else {
			if( ! pDlg->dataline_input( dt_ln ) ) {
				ret = TRUE;
			}
		}
	}
	else {
		if( ! pDlg->dataline_input( dt_ln ) ) {
			ret = TRUE;
		}
	}

	return ret;
}

// 伝票削除処理
int CDnpBaseClass::ToolF3Job()
{
	DBGFUNCTION;
	if (!m_PRO_OK)	return 0;

	if( IsJob() != JOB_SCAN )	return 0;
	int ret;
	BOOL bRet = FALSE;

	int	pn;
	if( IsKind() == KIND_NYUKIN )		pn	=	FD_CRED_PN;
	else if( IsKind() == KIND_SYUKKIN )	pn	=	FD_DEBT_PN;
	else								pn	=	FD_DBVAL_PN;

	for( int ln = 1; ln <= get_MaxLine(); ln++ ) {
		if( _getCND( ln, pn )->EDT_sg > 1 ) {
			bRet = TRUE;
			break;
		}
	}

	if( bRet ) {
		ICSMessageBox( "確定仕訳・転送済み仕訳が含まれる\r\n伝票のため、削除できません。", 0,0,0, m_pDnpParent );
		return -1;
	}

	int st = ICSMessageBox( "修正中の伝票を削除しますか？", MB_YESNO|MB_DEFBUTTON2, 0,0,m_pDnpParent );

	if( st == IDNO )	return -1;

	ret = m_DPscan.DenpDelelteJob();

	EndScan( FALSE );

	return 0;
}


// 仕訳取消
void CDnpBaseClass::ScanDeleteLine()
{
	 DBGFUNCTION;

	int	ln	=	get_nowln();
	int	pn	=	get_nowpn();
	int dt_ln	=	get_dataline(ln);

	// 確定仕訳
	if( _getCND( dt_ln, pn )->EDT_sg > 1 )
		return;

	int	sgn;
	if( m_DPscan.DeleteLine( dt_ln ) )	sgn	=	0;
	else								sgn	=	1;

	m_DPscan.DeleteLine( dt_ln )	=	sgn;

	SetDelSgn( dt_ln, sgn );
//	GetNowDnpDialog()->DeleteLine( ln, sgn );
//	Lineattr_dsp();

	total_dsp();

	if( IsKind() == KIND_FURIKAE ) {
		pn = FD_DBVAL_PN;
	}
	else if( IsKind() == KIND_NYUKIN ) {
		pn = FD_CRED_PN;
	}
	else {
		pn = FD_DEBT_PN;
	}

	_set_focus( ln, pn );
}


// 仕訳取消(スキャン時)
int CDnpBaseClass::ToolF4Job()
{
	if (!m_PRO_OK)	return 0;
	if( IsJob() != JOB_SCAN )	return 0;

	int pn;
	pn = get_nowpn();

	switch( pn ) {
	case DP_DENP_PN:
	case DP_MM_PN:
	case DP_DD_PN:
	case DP_BMN_PN:
	case DP_KOJI_PN:
	case FD_DSKBN_PN:
	case FD_CSKBN_PN:
		break;
	default:
		if( IsScanLine( get_dataline()/*get_nowln()*/ ) ) {
			if( IsKind() == KIND_FURIKAE ) {
				if( !m_DPscan.IsFukugoSiwake( get_dataline()/*get_nowln()*/ ) ) {
					ScanDeleteLine();
					return 0;
				}
			}
			else {
				ScanDeleteLine();
				return 0;
			}
		}
		break;
	}
	_set_focus( get_nowln(), get_nowpn() );

	return 0;
}


// 自動仕訳
int CDnpBaseClass::ToolF12Job()
{
	DBGFUNCTION;
	if (!m_PRO_OK)	return 0;

	if( IsJob() == JOB_SCAN && IsKind() == KIND_FURIKAE && IsScanLine( get_dataline()/*get_nowln()*/ ) ) {
//		set_focus( get_nowpn() );
		return 0;
	}
	int pn = get_nowpn();
	if( pn == DP_BMN_PN || pn == FD_DBMN_PN || pn == FD_CBMN_PN ||
		pn == DP_KOJI_PN || pn == FD_DKOJI_PN || pn == FD_CKOJI_PN ) {
		if(	BmnKojiIMEcheck() )	return 0;
	}
	if( pn == FD_TKY_PN )
		return 0;

	if( IsMode() == MODE_AUTO ) {
		IsMode() = MODE_NORMAL;
	}
	else {
		IsMode() = MODE_AUTO;
	}

	GetNowDnpDialog()->AutoSiwake( IsMode() );

	return 0;
}


int CDnpBaseClass::ToolINSJob()
{
	DBGFUNCTION;
	if (!m_PRO_OK)	return 0;

	ButtonInsertJob();
	return 0;
}


int CDnpBaseClass::ToolDELJob()
{
	DBGFUNCTION;
	if (!m_PRO_OK)	return 0;

	ButtonDeleteJob();
	return 0;
}



// 取消線の描画
void CDnpBaseClass::Lineattr_dsp()
{
//	DBGFUNCTION;
	if (!m_PRO_OK)	return;

	CDC* pDC;
	pDC = m_pDnpParent->GetDC();

	CPen p( PS_SOLID, 1, RGB_RED );
	CPen p2( PS_SOLID, 2, RGB_BLACK );

	CPen* oldp;
	int ln, dt_ln;

	if( IsJob() == JOB_SCAN || m_nCopyLine > 0 ) {
		oldp = pDC->SelectObject( &p );
		CRect rect;

		for( ln = 1 ; ln <= getSCROLL_NL(); ++ln )
		{
			dt_ln = get_dataline( ln );

			//コピー用矩形の描画
			if( m_nCopyLine == dt_ln ) {
				pDC->SelectObject( &p2 );
				GetNowDnpDialog()->GetLineRect( ln, rect );

				m_pDnpParent->ScreenToClient( rect );
				rect.left += 1;

				pDC->MoveTo(rect.left, rect.top );
				pDC->LineTo(rect.right, rect.top );
				pDC->LineTo(rect.right, rect.bottom );

				pDC->MoveTo(rect.left, rect.top );
				pDC->LineTo(rect.left, rect.bottom );
				pDC->LineTo(rect.right, rect.bottom );

				pDC->SelectObject( &p );
			}

			if( ! m_DPscan.DeleteLine( dt_ln ) ) {
				GetNowDnpDialog()->UpdateSubWindow(ln);
				continue;
			}

			GetNowDnpDialog()->GetDeleteRect( ln, rect );

			m_pDnpParent->ScreenToClient( rect );

			pDC->MoveTo(rect.left, rect.top );
			pDC->LineTo(rect.right, rect.top );

			pDC->MoveTo(rect.left, rect.bottom );
			pDC->LineTo(rect.right, rect.bottom );

#ifdef _DEBUG
			pDC->MoveTo( 0, 100 );
			pDC->LineTo( 100, 200 );
#endif
		}
		pDC->SelectObject( oldp );
	}

	// 01.14 /09
	m_pDnpParent->ReleaseDC( pDC );
}

//消費税修正サイン
BOOL CDnpBaseClass::GetSyzMdfySign( int seq )
{
	if( m_pDENPPAR->nosyzmdf )	return FALSE;

	vector<int>* pvec;
	vector<int>::iterator	it;
	pvec = &m_pDENPPAR->syzseq;

	for( it = pvec->begin(); it != pvec->end(); it++ ) {
		if( *it == seq ) {
			return TRUE;
		}
		if( *it > seq ) {
			break;
		}
	}
	return FALSE;
}


BOOL CDnpBaseClass::EnableButtonF7()
{
	BOOL ret = TRUE;
	if( m_DI.IsConFirmMaster() )	ret = FALSE;

	if( get_nowpn() != FD_TKY_PN ) {
		ret = FALSE;
	}
	return ret;
}


int CDnpBaseClass::ToolF7Job()
{
	DBGFUNCTION;
	if (!m_PRO_OK)	return 0;

	TKY_REGPAREX	par;

	int ln, pos, dt_ln;
	char	tky[128] = {0};
	CString code1, code2;

	ln		= get_nowln();
	dt_ln	= get_dataline( ln );

	if( get_nowpn() != FD_TKY_PN ) {
//		//定型伝票処理へ
//		RDDnpJob();
		return 0;
	}

	// 摘要辞書
	UINT id = IDC_ICSDBEDT8CTRL1;

	CWnd* dlg = GetNowDnpDialog();
	if( dlg ) {
		CWnd* pfocuswnd = dlg->GetFocus();

		if( pfocuswnd != NULL ) {
			int f_id = pfocuswnd->GetDlgCtrlID();
			if( f_id == IDC_ICSDBEDT_KASITKY )
				id = f_id;
		}
	}
	CICSDBEDT* pDedt;
	pDedt = (CICSDBEDT*)(GetNowDnpDialog()->GetDlgItem(id));
	pos = pDedt->GetCaretPosition();

	if( ! tblImage[dt_ln-1][0] )
	{
		pos = pDedt->GetCaretPosition();

		VARIANT var;
		var.pbVal = NULL;

		pDedt->GetData( &var, ICSDBEDT_TYPE_STRING, 0 );

		if( var.pbVal != NULL ) {
			strcpy_s( tky, sizeof tky, (char*)var.pbVal );
		}

		if( tky[0] == '\0' ) {
			pos = -1;
		}
	}
	else
	{
		pos = -1;
		tky[0] = '\0';
	}

	char invno[20] = { 0 };
	BYTE menchk = 0;

	struct _DenpData*	syzData = NULL;

	// 科目対応摘要
	if( IsKind() == KIND_FURIKAE ) {
		if( getCND( FD_DEBT_PN )->INP_sg )
			code1 = getDATA( FD_DEBT_PN )->DP_CdNum;
		else
			code1 = "";

		if( getCND( FD_CRED_PN )->INP_sg )
			code2 = getDATA( FD_CRED_PN )->DP_CdNum;
		else
			code2 = "";

		if( getCND(FD_DSKBN_PN)->INP_sg ) {
			syzData = getDATA(FD_DSKBN_PN);
		}
		else if( getCND(FD_CSKBN_PN)->INP_sg ) {
			syzData = getDATA(FD_CSKBN_PN);
		}
	}
	else if( IsKind() == KIND_NYUKIN ) {
		code1 = GNKINCode(m_pZm->zvol->apno);

		if( getCND( FD_CRED_PN )->INP_sg )
			code2 = getDATA( FD_CRED_PN )->DP_CdNum;
		else
			code2 = "";

		if( getCND(FD_CSKBN_PN)->INP_sg ) {
			syzData = getDATA(FD_CSKBN_PN);
		}
	}
	else {
		if( getCND( FD_DEBT_PN )->INP_sg )
			code1 = getDATA( FD_DEBT_PN )->DP_CdNum;
		else
			code1 = "";
	
		code2 = GNKINCode(m_pZm->zvol->apno);

		if( getCND(FD_DSKBN_PN)->INP_sg ) {
			syzData = getDATA(FD_DSKBN_PN);
		}
	}
	if( syzData != NULL ) {
		memcpy(invno, syzData->SYDATA.TKinvno, sizeof invno);
		BYTE syz6 = getDATA(FD_DSKBN_PN)->SYDATA._DSGN[6];
		if( syz6 & 0x20 ) {
			menchk = 1;
		}
	}

	// 摘要登録処理へ (DBInpSub.DLL)
	::ZeroMemory( &par, sizeof par );

	strncpy_s( par.tex_debt, sizeof par.tex_debt, code1, KMKCD_LN );
	strncpy_s( par.tex_cred, sizeof par.tex_cred, code2, KMKCD_LN );
	par.tex_pos = pos;
	memcpy( par.tex_tky, tky, sizeof par.tex_tky );
	memcpy(par.tex_invno, invno, sizeof par.tex_invno);
	par.tex_menchk = menchk;

	// 選択部分
	if( pos >= 0 ) {
		par.tex_sel	= pDedt->GetSelect();
	}

	
	BOOL bTkquery;
	int st = TkyEdaTorokDialog( &par, m_pZm, m_pSy, m_pDnpParent, &bTkquery );

	if( bTkquery ) {
		m_DI.tkrec_queryAll();
	}
	if( st == 1 ) {
		m_DI.IsModify() = TRUE;
		m_Seldata.SelectUpdate();
	}

	//枝番セット
	int dc_sw = 0;
	if( par.tex_brnsgn & 0x03 ) {
		if( IsKind() == KIND_FURIKAE ) {

			if( par.tex_brnsgn & 0x01 ) {
				if( getCND( FD_DEBT_PN )->INP_sg ) {
					getDATA( FD_DEBT_PN )->DP_BRN = par.tex_brndeb;
					dc_sw |= 0x01;
				}
			}
			if( par.tex_brnsgn & 0x02 ) {
				if( getCND( FD_CRED_PN )->INP_sg ) {
					getDATA( FD_CRED_PN )->DP_BRN = par.tex_brncre;
					dc_sw |= 0x02;
				}
			}
		}
		else if( IsKind() == KIND_NYUKIN ) {
			if( getCND( FD_CRED_PN )->INP_sg ) {
				getDATA( FD_CRED_PN )->DP_BRN = par.tex_brncre;
				dc_sw |= 0x02;
			}
		}
		else {
			if( getCND( FD_DEBT_PN )->INP_sg ) {
				getDATA( FD_DEBT_PN )->DP_BRN = par.tex_brndeb;
				dc_sw |= 0x01;
			}
		}

		struct _DP_DATA_LINE *dline;
		dline = getLINE_DATA( get_nowln() );

		TkyEda_Redisp( dc_sw, dline );
	}
	if( st == 1 ) {
		TkyInpEndUpdate();
	}

	if( pos != -1 ) {
		pDedt->SetCaretPosition( pos );
	}

	return 0;
}


void CDnpBaseClass::TkyEda_Redisp(int dc_sw, struct _DP_DATA_LINE *pdat)
{

struct _DenpData ins_data={0};
struct _DenpData *debt, *cred, *keybuf;

SETMEISYO	sm = {0};
struct _SET_KMKETC sk;
int pn;
struct _DNPLINE_CND* pCnd;
char	edabf[32] = {0};

	//摘要枝番表示
	if( dc_sw & 0x01 )
	{
		//借方枝番表示
		pn = FD_DEBT_PN;
		debt = _getDATAs( pdat, pn );
		pCnd = _getCNDs( pdat, pn );

		if( m_DI.IsBumon() ) {
			if( IsKind() == KIND_FURIKAE ) {
				keybuf = _getDATAs( pdat, FD_DBMN_PN );
			}
			else {
				keybuf = getDATA( DP_BMN_PN );
			}
			sk.bmn = keybuf->DP_BMN;
		}
		else {
			sk.bmn = -1;
		}
		sk.code = debt->DP_CdNum;
		sk.brn  = debt->DP_BRN;

		if( m_DI._set_kmketc( &sk ) ) {
			strcpy_s( pCnd->ETC_msg, sizeof pCnd->ETC_msg, sk.etc );
			pCnd->ETC_col = sk.etc_col;
			
			strcpy_s( sm.sm_name, sizeof sm.sm_name, debt->DP_stKMK.kd_name );
			strcpy_s( sm.sm_etc, sizeof sm.sm_etc, pCnd->ETC_msg );
			sm.sm_etccol = pCnd->ETC_col;
		}
		else {
			set_codestr( edabf, sizeof edabf, debt->DP_stKMK.kd_eda, m_pZm->zvol->edcol );
		//	m_DI.m_pZm->EdabanToStr( edabf, sizeof edabf, debt->DP_stKMK.kd_eda );
			sprintf_s( sm.sm_name, sizeof sm.sm_name, "%.14s%s", debt->DP_stKMK.kd_name, edabf );
		}

		GetNowDnpDialog()->SetKnam( get_nowln(), pn, &sm );
	}
	if( dc_sw & 0x02 )
	{
		//貸方枝番表示
		pn = FD_CRED_PN;
		cred = _getDATAs( pdat, pn );
		pCnd = _getCNDs( pdat, pn );

		if( m_DI.IsBumon() ) {
			if( IsKind() == KIND_FURIKAE ) {
				keybuf = _getDATAs( pdat, FD_CBMN_PN );
			}
			else {
				keybuf = getDATA( DP_BMN_PN );
			}
			sk.bmn = keybuf->DP_BMN;
		}
		else {
			sk.bmn = -1;
		}
		sk.code = cred->DP_CdNum;
		sk.brn  = cred->DP_BRN;

		if( m_DI._set_kmketc( &sk ) ) {
			strcpy_s( pCnd->ETC_msg, sizeof pCnd->ETC_msg, sk.etc );
			pCnd->ETC_col = sk.etc_col;
			
			strcpy_s( sm.sm_name, sizeof sm.sm_name, cred->DP_stKMK.kd_name );
			strcpy_s( sm.sm_etc, sizeof sm.sm_etc, pCnd->ETC_msg );
			sm.sm_etccol = pCnd->ETC_col;
		}
		else {
			set_codestr( edabf, sizeof edabf, cred->DP_stKMK.kd_eda, m_pZm->zvol->edcol );
		//	m_DI.m_pZm->EdabanToStr( edabf, sizeof edabf, cred->DP_stKMK.kd_eda );
			sprintf_s( sm.sm_name, sizeof sm.sm_name, "%.14s%s", cred->DP_stKMK.kd_name, edabf );
		}

		GetNowDnpDialog()->SetKnam( get_nowln(), pn, &sm );
	}
}


void CDnpBaseClass::SetDenpyoLine( int seq )
{
	int line, dc_sw;

	if( ! m_DPscan.GetDenpyoLineFromSeq( seq, line, dc_sw ) ) {
		set_nowln( 1 );
		set_nowpn( DP_DENP_PN );
		_set_focus( -1, DP_DENP_PN );
		return;
	}

	int linenum = getSCROLL_NL();

	int set_line, set_pn;

	switch( IsKind() ) {
	case KIND_FURIKAE:
		set_pn = (dc_sw == 0) ? FD_DBVAL_PN : FD_CRED_PN;
		break;
	case KIND_NYUKIN:
		set_pn = FD_CRED_PN;
		break;
	case KIND_SYUKKIN:
		set_pn = FD_DEBT_PN;
		break;
	}

	// 指定ＳＥＱが 一番底になるように スクロールする
	if( line > linenum ) {
		int move = line - linenum;
		DnpLine_Chg( move, 2 );
		set_line = linenum;
	}
	else {
		// 1ページ目にある場合
		set_line = line;
	}
	set_nowln( 1 );
	set_nowpn( DP_DENP_PN );
	_set_focus( set_line, set_pn );
}


void CDnpBaseClass::TkyInpEndUpdate()
{
	int pn;
	for( int ln = 1; ln <= get_MaxLine(); ln++ ) {
		//借方科目
		if( IsKind() == KIND_FURIKAE || IsKind() == KIND_SYUKKIN ) {
			pn = FD_DEBT_PN;
			KamokuUpdate( ln, pn );
		}
		//貸方科目
		if( IsKind() == KIND_FURIKAE || IsKind() == KIND_NYUKIN ) {
			pn = FD_CRED_PN;
			KamokuUpdate( ln, pn );
		}
	}
}

//部門摘要枝番　名称を更新
void CDnpBaseClass::bmneda_update(int displn, int pn)
{
	if( !m_DI.IsBumon() )	return;

	if( !m_DI.IsEdaban() )	return;

	if( m_pAsel->BMNTKBR_OPT != OPT_CHKON )
		return;

	if( m_pAsel->BRTEK_OPT != OPT_DSP )
		return;

	if( pn == DP_BMN_PN ) {
		for( int ln = 1; ln <= get_MaxLine(); ln++ ) {
			//借方科目
			KamokuUpdate(ln, FD_DEBT_PN);
			//貸方科目
			KamokuUpdate(ln, FD_CRED_PN);
		}
	}
	else {
		KamokuUpdate(displn, pn);
	}
}



//-------------------------------------------------------
//	仕訳データ部 科目部分更新
//
//-------------------------------------------------------
void CDnpBaseClass::KamokuUpdate( int ln, int pn )
{
	CString tmp;
	int		code;
	SETMEISYO sm;
	struct _SET_KMKETC sk;
	DBKNREC* pKn;
	char	edstr[64];
	CDnpBaseDlg* pDlg = GetNowDnpDialog();

	if( _getCND( ln, pn )->INP_sg ) {
		tmp		= _getDATA( ln, pn )->DP_CdNum;
		code	= _getDATA( ln, pn )->DP_BRN;

		set_codestr( edstr, sizeof edstr, code, m_pZm->zvol->edcol );
	//	m_pZm->EdabanToStr( edstr, sizeof edstr, code );

		if( (pKn = DB_PjisToKnrec( tmp, m_pZm )) != NULL ) {
			char nam[64];
			KamokuString( pKn, nam, sizeof nam );
			sprintf_s( sm.sm_name, sizeof sm.sm_name, "%.14s%s", nam, edstr );
		}
		else {
			::ZeroMemory( sm.sm_name, sizeof sm.sm_name );
		}

		sk.bmn = GetBmnCode(ln, pn);
		sk.code = tmp;
		sk.brn  = code;

		if( m_DI._set_kmketc( &sk ) ) {
			strcpy_s( _getCND( ln, pn )->ETC_msg, sizeof _getCND( ln, pn )->ETC_msg, sk.etc );
			_getCND( ln, pn )->ETC_col = sk.etc_col;
			
			strcpy_s( sm.sm_etc, sizeof sm.sm_etc, _getCND( ln, pn )->ETC_msg );
			sm.sm_etccol = _getCND( ln, pn )->ETC_col;
		}
		else {
			::ZeroMemory( _getCND( ln, pn )->ETC_msg, sizeof _getCND( ln, pn )->ETC_msg );
			_getCND( ln, pn )->ETC_col = RGB_BLACK;
			::ZeroMemory( sm.sm_etc, sizeof sm.sm_etc );
		}

		if( ln >= (1 + m_TopIdx) && ln <= (getSCROLL_NL() + m_TopIdx) ) {
			pDlg->SetKnam( (ln-m_TopIdx), pn, &sm );
		}
	}
}


BOOL CDnpBaseClass::ChangeZeiritsu()
{
	int ln, pn, zeipn, skbn_pn, dt_ln;
	ln = get_nowln();
	pn = get_nowpn();
	dt_ln = get_dataline( ln );

	struct _DenpData* keybuf;

	skbn_pn = -1;

	switch (IsKind()) {
	case KIND_FURIKAE:
		if (pn == FD_DBVAL_PN) {
			skbn_pn = FD_DSKBN_PN;
		}
		else if (pn == FD_CRVAL_PN) {
			skbn_pn = FD_CSKBN_PN;
		}
		break;

	case KIND_NYUKIN:
		if (pn == FD_CRVAL_PN) {
			skbn_pn = FD_CSKBN_PN;
		}
		break;
	case KIND_SYUKKIN:
		if (pn == FD_DBVAL_PN) {
			skbn_pn = FD_DSKBN_PN;
		}
		break;
	}
	if (skbn_pn == -1)
		return FALSE;

	// フォーカスオフの処理を呼び出す
	GetNowDnpDialog()->DiagEditOFF();

	keybuf = getDATA( skbn_pn );

	DWORD code = 0;
//MyTrace( "keybuf->SYDATA.ritu_sw = %08x, def = %08x", keybuf->SYDATA.ritu_sw, keybuf->SYDATA.ritu_def );
//MyTrace( "keybuf->SYDATA.ritu_sw = %02x%02x%02x", keybuf->SYDATA._DSGN[0], keybuf->SYDATA._DSGN[1], keybuf->SYDATA._DSGN[2] );
	//if (keybuf->SYDATA.ritu_sw == CD_RITU10) {
	//	code = CD_RD_RITU8;
	//}
	//else if (keybuf->SYDATA.ritu_sw == CD_RD_RITU8) {
	//	code = CD_RITU10;
	//}

	switch (keybuf->SYDATA._DSGN[1] & 0x0f) {
	case 4:		code = CD_RITU10;	break;	//8軽 → 10%
	case 5:		code = CD_RD_RITU8;	break;	//10% → 8軽
	}

	if (code) {
		m_pSy->SyFnc( code );

		struct _SY_MSG_PACK2 smpk;
		m_pSy->SyMsg( &smpk );
		CString txt;
		txt = m_pSy->Sy_get_message( m_pZm->dbdata, SYGET_TYPE );

		if (IsScanLine( dt_ln )) {
			CDBINPDataRec	dtemp;

			int dc_sw = (skbn_pn == FD_CSKBN_PN) ? 1 : 0;
			dtemp = m_DPscan.GetOriginData( dt_ln, dc_sw );

			CDBINPDataRec	newdrec;
			m_DI.GetCDBData( &newdrec );

			if (m_DI.isSyzdiff_data( &newdrec, &dtemp )) {
				keybuf->SYDATA.sy_chg = 1;
			}
		}
		SRCTrace;
		PutsSyz( ln, skbn_pn, ( char* )( LPCTSTR )txt, keybuf->SYDATA.sy_chg );

		memcpy( keybuf->SYDATA._DSGN, &m_pZm->dbdata->dsign[0], sizeof keybuf->SYDATA._DSGN );

		strcpy_s( keybuf->SYDATA.SKBNtxt, sizeof keybuf->SYDATA.SKBNtxt, txt );

		int swk, ritu, zei, uri, tai, toku;
		swk = ritu = zei = uri = tai = toku = 0;

		swk = smpk.SY_MSG_SWKBN->sel_no;
		ritu = smpk.SY_MSG_RITU->sel_no;
		zei = smpk.SY_MSG_ZEI->sel_no;
		uri = smpk.SY_MSG_URISIRE->sel_no;
		toku = smpk.SY_MSG_TOKTEI->sel_no;
		tai = smpk.SY_MSG_TAIKAKBN->sel_no;

		if (swk)	keybuf->SYDATA.swk_sw = smpk.SY_MSG_SWKBN->msg_tbl[swk - 1]->code;
		else		keybuf->SYDATA.swk_sw = 0;

		if (ritu)	keybuf->SYDATA.ritu_sw = smpk.SY_MSG_RITU->msg_tbl[ritu - 1]->code;
		else		keybuf->SYDATA.ritu_sw = 0;

		if (zei)	keybuf->SYDATA.zeikbn_sw = smpk.SY_MSG_ZEI->msg_tbl[zei - 1]->code;
		else		keybuf->SYDATA.zeikbn_sw = 0;

		if (uri)	keybuf->SYDATA.uri_sw = smpk.SY_MSG_URISIRE->msg_tbl[uri - 1]->code;
		else		keybuf->SYDATA.uri_sw = 0;
		// 特定収入
		if (toku)	keybuf->SYDATA.toku_sw = smpk.SY_MSG_TOKTEI->msg_tbl[toku - 1]->code;
		else		keybuf->SYDATA.toku_sw = 0;

		if (tai)	keybuf->SYDATA.kts_sw = smpk.SY_MSG_TAIKAKBN->msg_tbl[tai - 1]->code;
		else		keybuf->SYDATA.kts_sw = 0;

		// 税額 再表示
		char	zeibf[6] = { 0 };
		char*	pZei = NULL;
		int sgn = 0;

		// 有価証券非課税譲渡
		if (m_pSy->IsTaikaData( m_pZm->dbdata ) == 2)	sgn = 1;

		zeipn = (skbn_pn == FD_DSKBN_PN) ? FD_DBZEI_PN : FD_CRZEI_PN;

		l_input( zeibf, ( char* )( LPCTSTR )smpk.SY_MSG_SYZEI );	// 税額をセット
		memmove( getDATA( zeipn )->DP_VAL, zeibf, 6 );

		// 消費税仕訳
		if ( m_DI.is_syohizeisiwake( m_pZm->dbdata) ) {
			_getCND( dt_ln, zeipn )->INP_sg = TRUE;
			pZei = zeibf;
		}
		else {
			sgn = 2;
			_getCND( dt_ln, zeipn )->INP_sg = FALSE;
			pZei = NULL;
		}
		_getCND( dt_ln, zeipn )->INP_type = sgn;

#ifdef CLOSE
		// 消費税が、輸入仕入 に変更になったか？
		index = (skbn_pn == FD_DSKBN_PN) ? 0 : 1;

		if (m_pSyzInp->GetSyzswkKbn() != CD_YUNYU) {
			if ((m_DI.m_pZm->dbdata->dsign[0] & 0x40) && (m_DI.m_pZm->dbdata->dsign[2] & 0x0f) == 0x01 &&
				m_DI.m_pZm->dbdata->dsign[4] == 4) {
				m_bSyzYunyu[index] = (pAUTOSEL->ZEIMDFY_OPT == OPT_ZEIMDFY) ? TRUE : FALSE;
			}
			else {
				m_bSyzYunyu[index] = FALSE;
			}
		}
		// 現在レコードの 対価 をバッファにセットする
		for (int n = 0; n < m_SYdataCnt; n++) {
			if (m_CREC[n].m_seq == m_DI.m_pZm->dbdata->seq) {
				m_DI.m_pZm->GetCDBData( &m_CREC[n] );
				memcpy( keybuf->SYDATA.TKsyohi, m_CREC[n].m_taika, sizeof m_CREC[n].m_taika );
			}
		}
#endif

		GetNowDnpDialog()->SetVal( ln/*get_nowln()*/, zeipn, pZei, sgn );

		// 単一仕訳の場合、借方・貸方の金額を揃える
		SetTandokuSameValue( ln, pn );

		total_dsp();

		set_focus( pn );

		return TRUE;
	}
	else {
		set_focus( pn );
	}

	return TRUE;
}


//---------------------------------------------------------------------
// 諸口枝番入力エリア 引き延ばし＆戻し
//	int ln ... 画面伝票上の　行番号(１～最大７)
//
//---------------------------------------------------------------------
void CDnpBaseClass::syobrnedit_expan(int ln, int pn, CRect* rc, BOOL exp_sw, BOOL bOnSize/*=FALSE*/)	// ln = 該当ライン
{
	CDnpBaseDlg* dlg;
	VARIANT var;
	UINT id = IDC_ICSDBEDT_SYOBRN;

	if (IsKind() != KIND_FURIKAE)
		return;

	dlg = GetNowDnpDialog();

	CWnd* pwnd = GetNowDnpDialog()->GetDlgItem(id);
	ASSERT(pwnd != NULL);

	CRect rect, dlgrc, tgnrc;
	CString buf;
	char	tmp[128];

	if (exp_sw) {	// 拡大
		rect = *rc;

		dlg->ScreenToClient(rect);
		sprintf_s(tmp, sizeof tmp, "%d", _op_max_calq(m_DI.m_pZm->zvol->edcol));

		CFont font;
		COleFont ofont = ((CICSDBEDT*)pwnd)->GetFont();
		CY cy = ofont.GetSize();

		LOGFONT lf = { 0 };
		lf.lfHeight = (LONG)(cy.int64 / 1000);
		lf.lfCharSet = DEFAULT_CHARSET;
		strcpy_s(lf.lfFaceName, sizeof lf.lfFaceName, ofont.GetName());

		font.CreatePointFontIndirect(&lf);

		CClientDC dc(m_pDnpParent);
		CSize sz;
		CFont* pOld;
		pOld = dc.SelectObject(&font);
		sz = dc.GetTextExtent(tmp);

		int len = strlen(tmp);
		int mojihaba = (sz.cx / len);
		sz.cx += mojihaba;

		dc.SelectObject(pOld);
		font.DeleteObject();

		int wd = rect.Width();
		if (wd < sz.cx) {
			rect.right = rect.left + sz.cx;;
		}
		pwnd->SetWindowPos(NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
		pwnd->MoveWindow(rect);
		pwnd->UpdateWindow();

		// データをセット
		int dt_ln = get_dataline(ln);

		set_codestr(tmp, sizeof tmp, _getDATA(dt_ln, pn)->DP_cBRN, m_pZm->zvol->edcol);
//		m_DI.EdabanToStr(tmp, sizeof tmp, _getDATA(dt_ln, pn)->DP_cBRN);
		var.pbVal = (BYTE*)tmp;
		DBdata_set(dlg, id, &var, ICSDBEDT_TYPE_STRING, 0);
		m_pDnpParent->PostMessage(WM_RETROFOCUS2, id);
	}
	else {
		if (!bOnSize) {
			var.pbVal = NULL;
			DBdata_set(dlg, id, &var, ICSDBEDT_TYPE_STRING, 0);
		}

		pwnd->MoveWindow(0, 0, 0, 0);
	}
}


//--------------------------------------------------------
//	諸口枝番ターミネーション
//
//--------------------------------------------------------
void CDnpBaseClass::TerminationSyoBrnDBedit(UINT id, long nChar, long inplen, long kst)
{
//	if (!PROGRAM_OK)	return;

	VARIANT var;
	int pn, ln, dt_ln, syobrn, prev_syobrn;
	char	data[32];

	if (IsKind() != KIND_FURIKAE)
		return;

	CWnd* pDlgWnd;
	pDlgWnd = GetNowDnpDialog();
	pn = get_nowpn();

	ln = get_nowln();
	dt_ln = get_dataline(ln);

	BOOL bModify_Ok;
	bModify_Ok = (_getCND(dt_ln, pn)->EDT_sg <= 1);
	if( m_DI.IsConFirmMaster() )	bModify_Ok = FALSE;

	DBdata_get(pDlgWnd, id, &var, ICSDBEDT_TYPE_STRING, 0);

	syobrn = -1;
	if (var.pbVal != NULL) {
		CString sTmp = (const char*)var.pbVal;
		if (sTmp.GetLength() > 0)
			syobrn = atoi((const char*)var.pbVal);
	}

	if (bModify_Ok) {
		prev_syobrn = getDATA(pn)->DP_cBRN;

		if (inplen > 0) {
			CString input;
			if (var.pbVal != NULL) {
				input = var.pbVal;
			}
			set_codestr(data, sizeof data, syobrn, m_DI.m_pZm->zvol->edcol);
		//	m_pZm->EdabanToStr(data, sizeof data, syobrn);

			var.pbVal = (BYTE*)data;
			DBdata_set(pDlgWnd, id, &var, ICSDBEDT_TYPE_STRING, 0);
		}

		if (nChar == VK_RETURN || !nChar) {

			if (syobrn == -1) {
				syobrn = _getDATA(dt_ln, pn)->DP_cBRN;
			}
			else {
				_getDATA(dt_ln, pn)->DP_cBRN = syobrn;
			}

			GetNowDnpDialog()->SetSyogBrn(ln, pn, syobrn);

			// 入力サイン ON
			if (nChar || (!nChar && syobrn != -1))
				_getCND(dt_ln, pn)->INP_sg = TRUE;
		}

		if (nChar == VK_DELETE) {
			var.pbVal = NULL;
			DBdata_set(pDlgWnd, id, &var, ICSDBEDT_TYPE_STRING, 0);

			_getDATA(dt_ln, pn)->DP_cBRN = -1;
			_getCND(dt_ln, pn)->INP_sg = FALSE;
			syobrn = -1;
			GetNowDnpDialog()->SetSyogBrn(ln, pn, syobrn);
		}
	}
	else {
		if( nChar == 0x16 ) {
			prev_syobrn = getDATA(pn)->DP_cBRN;
			if( prev_syobrn != syobrn ) {
				set_codestr(data, sizeof data, syobrn, m_DI.m_pZm->zvol->edcol);
				var.pbVal = (BYTE*)data;
				DBdata_set(pDlgWnd, id, &var, ICSDBEDT_TYPE_STRING, 0);
			}
		}
	}

	//if (nChar != 0) {
	//	nChar = SelBmonDsp(nChar);
	//}

	if (nChar != 0) {
		// ターミネーション処理
		if (nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT))
			nChar = VK_F2;

		ToViewTermJob(ln, pn, nChar);
	}
}


//---------------------------------------------------------------------
// 証憑番号入力エリア 引き延ばし＆戻し
//	int ln ... 画面伝票上の　行番号(１～最大７)
//
//---------------------------------------------------------------------
void CDnpBaseClass::syonum_expan(int ln, CRect* rc, BOOL exp_sw, BOOL bOnSize/*=FALSE*/)	// ln = 該当ライン
{
	CDnpBaseDlg* dlg;
	VARIANT var;
	UINT id = IDC_ICSDBEDT_SYONUM;

	dlg = GetNowDnpDialog();

	//FILETrace("bmnedit_expan ln %d, exp_sw = %d\n", ln, exp_sw );

	CWnd* pwnd = GetNowDnpDialog()->GetDlgItem(id);
	ASSERT(pwnd != NULL);

	CRect rect, dlgrc, tgnrc;
	CString buf;
	char	tmp[128];

	if( exp_sw ) {
		rect = *rc;

		dlg->ScreenToClient(rect);
		pwnd->SetWindowPos(NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
		pwnd->MoveWindow(rect);
		pwnd->UpdateWindow();

		// データをセット
		int dt_ln = -1;
		strcpy_s(tmp, sizeof(tmp), (char*)_getDATA(dt_ln, DP_SYONUM_PN)->DP_SNUM);
		var.pbVal = (BYTE*)tmp;
		DBdata_set(dlg, id, &var, ICSDBEDT_TYPE_STRING, 0);
		m_pDnpParent->PostMessage(WM_RETROFOCUS2, id);
	}
	else {
		if( !bOnSize ) {
			var.pbVal = NULL;
			DBdata_set(dlg, id, &var, ICSDBEDT_TYPE_STRING, 0);
		}

		pwnd->MoveWindow(0, 0, 0, 0);
	}
}


//------------------------------------------------------------------
//	証憑番号を入力( DBEdit より )
//
//------------------------------------------------------------------
void CDnpBaseClass::TerminationSyoNumDBedit(UINT id, long nChar, long inplen, long kst)
{
	if( !m_PRO_OK )	return;

	struct _DenpData* keybuf;
	int		dt_ln = -1;
	int		pn = DP_SYONUM_PN;
	keybuf = _getDATA(dt_ln, pn);

	VARIANT var;
	CWnd* pDlgWnd;
	pDlgWnd = GetNowDnpDialog();

	BOOL bModify_Ok;
	bModify_Ok = (_getCND(dt_ln, pn)->EDT_sg <= 1);

	DBdata_get(pDlgWnd, id, &var, ICSDBEDT_TYPE_STRING, 0);
	if( var.pbVal != NULL ) {
		strcpy_s((char*)keybuf->DP_SNUM, sizeof keybuf->DP_SNUM, (const char*)var.pbVal);
		GetNowDnpDialog()->SetTgNumber(-1, pn, (char*)keybuf->DP_SNUM);
	}
	// 削除
	if( nChar == VK_DELETE ) {
		var.pbVal = NULL;
		DBdata_set(pDlgWnd, id, &var, ICSDBEDT_TYPE_STRING, 0);

		::ZeroMemory(keybuf->DP_SNUM, sizeof keybuf->DP_SNUM);
		GetNowDnpDialog()->SetTgNumber(-1, pn, NULL);
	}

	if( keybuf->DP_SNUM[0] )	_getCND(dt_ln, pn)->INP_sg = TRUE;
	else					_getCND(dt_ln, pn)->INP_sg = FALSE;

	// ターミネーション処理
	if( nChar != 0 ) {
		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			nChar = VK_F2;

		ToViewTermJob(-1, pn, nChar);
	}
}

//---------------------------------------------------
//	証憑番号 ターミネーション
//
//---------------------------------------------------
void CDnpBaseClass::OnTermSyoNum(int ln, long nChar)
{
	int vect;

	if( nChar == VK_UP || nChar == VK_DOWN )
	{
		int dset = 0;

		if( nChar == VK_UP && ln == 1 ) {
			dset = DnpLine_Chg(-1);
		}
		else if( nChar == VK_DOWN && ln == getSCROLL_NL() ) {
			dset = DnpLine_Chg(1);
		}

		if( !dset ) {
			// 行移動
			if( nChar == VK_UP ) {
				--ln;
				vect = -1;
			}
			else {
				++ln;
				vect = 1;
			}

			//行移動エラー（アンダー／オーバー　フロー）
			if( set_position(ln, FD_TGNUM_PN, vect) == -1 ) {
				if( nChar == VK_UP )	set_focus(DP_MM_PN);
				else {
					if( IsKind() == KIND_SYUKKIN )		set_position(1, FD_DEBT_PN);
					else if( IsKind() == KIND_FURIKAE )	set_position(1, FD_DBVAL_PN);
				}
			}
		}
		return;
	}

	// カーソルセット
	switch( nChar )
	{
	case VK_RIGHT:
	case VK_RETURN:
	case VK_TAB:
		//	case VK_F3:
	case VK_DELETE:
		set_position(ln, FD_TGDATE_PN);
		break;
	case VK_LEFT:
	case VK_F2:
		if( IsKind() == KIND_FURIKAE || IsKind() == KIND_SYUKKIN ) {
			_set_focus(ln, FD_DEBT_PN);
		}
		else if( IsKind() == KIND_SYUKKIN ) {
			_set_focus(ln, FD_CRED_PN);
		}
		break;
	default:
		Buzzer();
		break;
	}
}


//科目DBEdit からのターミネーション処理
void CDnpBaseClass::TerminationKmkinp(UINT id, long nChar, long inplen, long kst)
{
	struct _DenpData* keybuf;
	int		dt_ln = -1;
	int		pn = get_nowpn();
	if( !m_PRO_OK )	return;

	if( pn != FD_DEBT_PN && pn != FD_CRED_PN )
		return;

	int ln = get_nowln();
	dt_ln = get_dataline(ln);
	keybuf = _getDATA(dt_ln, pn);

	BOOL bModify_Ok;
	bModify_Ok = (_getCND(dt_ln, pn)->EDT_sg <= 1);
	if( m_DI.IsConFirmMaster() )	bModify_Ok = FALSE;

	CWnd* pDlgWnd;
	pDlgWnd = GetNowDnpDialog();
	struct _KamokuData check = { 0 };

	DBkamokudata_get(pDlgWnd, id, &check, m_pZm);
	TRACE("DBEDIT*get(%d) keybuf->DP_BRN = %d, check.kd_eda(%d)", __LINE__, _getDATA(dt_ln, pn)->DP_BRN, check.kd_eda);

	if( bModify_Ok ) {
		nChar = ItemInp(ln, pn, nChar, &check);
		//if( nChar == VK_DELETE ) {
		//}
	}
	else {
		if( nChar == 0x16 ) {
		}
	}
	if( nChar != 0 ) {
		// ターミネーション処理
		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			nChar = VK_F2;

		ToViewTermJob(ln, pn, nChar);
	}
}


// 借方チェックデータ
int CDnpBaseClass::CheckImeEndKamoku(long nChar, LPCTSTR string)
{
	struct _KamokuData km, *pkd;
	int ln, pn;

	ln = get_nowln();
	pn = get_nowpn();

	pkd = NULL;

	if( CheckKamoku(nChar, string, &km) )
		pkd = &km;
	else {
		if( (BYTE)km.kd_code[0] == (BYTE)-1 )	//同一科目入力コードが複数の場合 公益
			return -1;

		KnamRedisp(ln, pn);
		return -1;
	}

	int ret = ItemInp(ln, pn, nChar, pkd);

	return ret == 0 ? 1 : 0;
}



//科目DBEdit HWND 取得
HWND CDnpBaseClass::GetKamokuCtrlHWND()
{
	CWnd* pDlgWnd;
	pDlgWnd = GetNowDnpDialog();

	HWND hwnd = pDlgWnd->GetDlgItem(IDC_ICSDBEDT_KMINP)->m_hWnd;
	return hwnd;
}

//カナ検索桁数
int CDnpBaseClass::KanaSearchKeta()
{
	return kana_sysflg ? 10 : 1;
}


//科目欄 複数文字でカナ検索
int CDnpBaseClass::FukuKanaSearch(MSG* pMsg, int edaonly)
{
	HIMC	hImc;
	char	buf[128] = { 0 };
	char	yomi[128] = { 0 };
	char	yomi_org[128] = { 0 };

	if( kana_sysflg == 0 )
		return 0;

	if( pMsg->wParam == VK_RETURN || pMsg->wParam == VK_TAB ) {
		return 0;
	}

	//コントロールのHWND を取得する
	HWND	hWnd;
	hWnd = GetKamokuCtrlHWND();
	if( hWnd == NULL )
		return 0;

	if( pMsg->wParam == VK_NEXT || pMsg->wParam == VK_PRIOR ) {
		return 0;
	}

	hImc = ImmGetContext(hWnd);

	if( pMsg->wParam == VK_END ) {
		ImmNotifyIME(hImc, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
		ImmReleaseContext(hWnd, hImc);
		return 0;
	}

	ImmGetCompositionString(hImc, GCS_COMPSTR, buf, sizeof buf);
	ImmGetCompositionString(hImc, GCS_COMPREADSTR, yomi_org, sizeof yomi_org);

//	MyTrace("●CDnpBaseClass::FukuKanaSearch buf, = %s, yomi_org = %s●\n", buf, yomi_org);

	//読みにアルファベットがあるか？
	CString stmp(yomi);
	kana_check(yomi, yomi_org);

	int orglen = strlen(yomi_org);
	int searchsgn = 1;
	if( orglen > 0 ) {
		int len = strlen(yomi);
		if( len < orglen ) {
			searchsgn = 0;
		}
	}

	int rval = 0;

	if( buf[0] && yomi[0] && searchsgn ) {
		SelPar sel;

		if( BRNTKY_sel ) {
			sel.sel_pn = SL_BRNTKY_PN;
			sel.sel_flg = 1;

			BRNTKY_PAR brp;
			brp = m_Seldata.GetBrnTkyPar();
			if( isSYOGT_CODE(brp.bt_code) ) {
				return 0;
			}

			sel.tkybrn = brp.bt_code;
			sel.brn_bmn = brp.bt_bmn;
			// 絞込み
			sprintf_s(sel.brn_kana, sizeof sel.brn_kana, "%.8s", yomi);
		}
		else {
			sel.sel_pn = SL_KMKANA_PN;
			sprintf_s(sel.kmk_kana, sizeof sel.kmk_kana, "%.8s", yomi);
		}
		m_Seldata.SelectJob(&sel, -1);
	}
	else if( (buf[0] == '\0') && (yomi[0] == '\0') ) {
		if( pMsg->wParam == VK_BACK || pMsg->wParam == VK_DELETE ) {
			SelPar par;
			if( BRNTKY_sel ) {
				BRNTKY_PAR brp;
				brp = m_Seldata.GetBrnTkyPar();
				if( brp.bt_kana[0] == 0 ) {
					par.sel_pn = SL_KAMOKU_PN;
					//枝番のみの欄
					if( edaonly )
						return 0;
				}
				else {
					par.sel_pn = SL_BRNTKY_PN;
					par.sel_flg = 1;
					par.tkybrn = brp.bt_code;
					par.brn_bmn = brp.bt_bmn;
					::ZeroMemory(par.brn_kana, sizeof par.brn_kana);
				}
			}
			else {
				par.sel_pn = SL_KAMOKU_PN;
			}
			m_Seldata.SelectJob(&par, -1);
		}
		if( pMsg->wParam == VK_BACK ) {
			rval = 1;
		}
	}
	ImmReleaseContext(hWnd, hImc);

	return rval;
}


//科目欄 バックスペース対応
int CDnpBaseClass::KanaCheckVK_BACK(MSG* pMsg)
{
	HIMC	hImc;
	char	buf[128] = { 0 };
	char	yomi[128] = { 0 };
	char	yomi_org[128] = { 0 };

	if( kana_sysflg == 0 )
		return 0;

	if( pMsg->wParam != VK_BACK && pMsg->wParam != VK_F2 ) {
		return 0;
	}

	//コントロールのHWND を取得する
	HWND	hWnd;
	hWnd = GetKamokuCtrlHWND();
	if( hWnd == NULL )
		return 0;

	hImc = ImmGetContext(hWnd);
	if( pMsg->wParam == VK_F2 ) {
		ImmNotifyIME(hImc, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
		ImmReleaseContext(hWnd, hImc);
		return 0;
	}

	ImmGetCompositionString(hImc, GCS_COMPSTR, buf, sizeof buf);
	ImmGetCompositionString(hImc, GCS_COMPREADSTR, yomi_org, sizeof yomi_org);

	int rval = 0;

	if( (buf[0] == '\0') && (yomi[0] == '\0') ) {
		if( pMsg->wParam == VK_BACK ) {
			rval = 1;
		}
	}

	ImmReleaseContext(hWnd, hImc);

	return rval;
}


BOOL CDnpBaseClass::IsKamokuPosition()
{
	BOOL ret = FALSE;

	int pn = get_nowpn();
	if( pn == FD_DEBT_PN || pn == FD_CRED_PN )
		ret = TRUE;

	return ret;
}


//指定ポジションの科目が消費税科目か？
BOOL CDnpBaseClass::IsKamokuSyattr(int ln, int pn)
{
	BOOL ret = FALSE;
	int dt_ln = get_dataline(ln);

	struct _DenpData *keybuf;
	keybuf = _getDATA(dt_ln, pn);

	DBKNREC* pKn = DB_PjisToKnrec(keybuf->DP_CdNum, m_pZm);

	if( pKn ) {
		if( m_DI.km_syattr(pKn) ) {
			ret = TRUE;
		}
	}
	return ret;
}
//---------------------------------------------------------
//	単一仕訳で借方・貸方 同じ消費税か？
//　int ln ... 画面伝票上の　行番号(１～最大７)
//
//---------------------------------------------------------
BOOL CDnpBaseClass::IsSameFuriSyzKbn(int ln)
{
	int val_pn, val_op, zei_pn, zei_op, syz_pn, syz_op;
	int dc_sw = -1;
	int	dt_ln = get_dataline(ln);

	if( IsKind() != KIND_FURIKAE )
		return FALSE;

	if( IsType() != TYPE_TANDOKU )
		return FALSE;

	val_pn = FD_DBVAL_PN;
	zei_pn = FD_DBZEI_PN;
	syz_pn = FD_DSKBN_PN;

	val_op = FD_CRVAL_PN;	// 逆側の項目
	zei_op = FD_CRZEI_PN;
	syz_op = FD_CSKBN_PN;

	// 諸口仕訳を詰めない場合
	if( SyogSiwakeMode() )
		return FALSE;

	// 金額未入力
	if( !_getCND(dt_ln, val_pn)->INP_sg )
		return FALSE;

	struct _DenpData *keybuf, *other;
	struct _DenpData *zeibuf, *ot_zei;

	keybuf = other = zeibuf = ot_zei = NULL;
	char val[6] = { 0 };

	keybuf = _getDATA(dt_ln, val_pn);
	other = _getDATA(dt_ln, val_op);

	if( keybuf == NULL || other == NULL )
		return FALSE;

	//金額同じで、借方消費税ありの場合は、消費税を出す。
	if( memcmp(keybuf->DP_VAL, other->DP_VAL, 6) == 0 ) {
		if( !_getCND(dt_ln, syz_pn)->INP_sg ) {
			return TRUE;
		}
	}
	return FALSE;
}


//行変わりによる、フォーカスカウントのクリア
void CDnpBaseClass::FocusCountClear(int line)
{
	int	dt_ln = get_dataline(line);

	int pntbl[] = { FD_DBZEI_PN, FD_CRZEI_PN, FD_DSKBN_PN, FD_CSKBN_PN, -1 };

	for( int n = 0; pntbl[n] != -1; n++ ) {
		//直前のポジション
		_getCND(dt_ln, pntbl[n])->ETC_sg[1] = 0;
		//カウンタ
		_getCND(dt_ln, pntbl[n])->ETC_sg[2] = 0;
		//税額欄外税変更キー押下
		_getCND(dt_ln, pntbl[n])->ETC_sg[3] = 0;
	}
}


//外税＋免税事業者のサインセット
void CDnpBaseClass::set_sotomen_sign(int dt_ln, int pn, int sotomen)
{
	_getCND(dt_ln, pn)->ETC_sg[0] = sotomen;

	if( !sotomen ) {
		_getCND(dt_ln, pn)->ETC_sg[1] = _getCND(dt_ln, pn)->ETC_sg[2] = _getCND(dt_ln, pn)->ETC_sg[3] = 0;
	}
}


//外税免税
BOOL CDnpBaseClass::ChangeSotoMenZei()
{
	int ln, valpn, zeipn, skbn_pn, dt_ln;
	ln = get_nowln();
	zeipn = get_nowpn();
	dt_ln = get_dataline(ln);

	struct _DenpData* keybuf;
	valpn = -1;
	skbn_pn = -1;

	switch( IsKind() ) {
	case KIND_FURIKAE:
		if( zeipn == FD_DBZEI_PN ) {
			valpn = FD_DBVAL_PN;
			skbn_pn = FD_DSKBN_PN;
		}
		else if( zeipn == FD_CRZEI_PN ) {
			valpn = FD_CRVAL_PN;
			skbn_pn = FD_CSKBN_PN;
		}
		break;

	case KIND_NYUKIN:
		if( zeipn == FD_CRZEI_PN ) {
			valpn = FD_CRVAL_PN;
			skbn_pn = FD_CSKBN_PN;
		}
		break;
	case KIND_SYUKKIN:
		if( zeipn == FD_DBZEI_PN ) {
			valpn = FD_DBVAL_PN;
			skbn_pn = FD_DSKBN_PN;
		}
		break;
	}
	if( valpn == -1 )
		return FALSE;

	// フォーカスオフの処理を呼び出す
	GetNowDnpDialog()->DiagEditOFF();

	keybuf = getDATA(skbn_pn);

	BOOL bSotomen = FALSE;
	//MyTrace( "keybuf->SYDATA.ritu_sw = %08x, def = %08x", keybuf->SYDATA.ritu_sw, keybuf->SYDATA.ritu_def );
	//MyTrace( "keybuf->SYDATA.ritu_sw = %02x%02x%02x", keybuf->SYDATA._DSGN[0], keybuf->SYDATA._DSGN[1], keybuf->SYDATA._DSGN[2] );
		//if (keybuf->SYDATA.ritu_sw == CD_RITU10) {
		//	code = CD_RD_RITU8;
		//}
		//else if (keybuf->SYDATA.ritu_sw == CD_RD_RITU8) {
		//	code = CD_RITU10;
		//}

	if( _getCND(dt_ln, zeipn)->ETC_sg[3] == 0 ) {
		if( _getCND(dt_ln, skbn_pn)->INP_sg ) {
			if( _sotozei_data(_getDATA(dt_ln, skbn_pn)->SYDATA._DSGN) ) {
				if( _getDATA(dt_ln, skbn_pn)->SYDATA._DSGN[6] & 0x20 ) {
					if( m_pZm->dbdata->ddate >= SYZ_INV_YMD ) {
						bSotomen = TRUE;
					}
				}
			}
		}
	}

	if( bSotomen ) {
#ifdef CLOSE
		if( IsScanLine(dt_ln) ) {
			CDBINPDataRec	dtemp;

			int dc_sw = (skbn_pn == FD_CSKBN_PN) ? 1 : 0;
			dtemp = m_DPscan.GetOriginData(dt_ln, dc_sw);

			CDBINPDataRec	newdrec;
			pDBzm->GetCDBData(&newdrec);

			if( _isSyzdiff_data(&newdrec, &dtemp) ) {
				keybuf->SYDATA.sy_chg = 1;
			}
		}
#endif
		// 本体価格, 税額 再表示
		char	valbf[6] = { 0 };
		char*	pVal = NULL;
		int sgn = 3;

		CZmGen8 zmGen;
		zmGen.RepairMenzeiZeigaku(m_pZm->zvol, m_pZm->dbdata);

		l_input(valbf, (char*)(LPCTSTR)m_pZm->dbdata->val);	// 本体価格
		memmove(getDATA(valpn)->DP_VAL, valbf, 6);

		pVal = valbf;
		GetNowDnpDialog()->SetVal(ln, valpn, pVal);

		l_input(valbf, (char*)(LPCTSTR)m_pZm->dbdata->zei);	// 税額をセット
		memmove(getDATA(zeipn)->DP_VAL, valbf, 6);

		pVal = valbf;
		GetNowDnpDialog()->SetVal(ln, zeipn, pVal, sgn);

		//_getCND(dt_ln, zeipn)->INP_type = sgn;
		_getCND(dt_ln, zeipn)->ETC_sg[3] = 1;

		// 単一仕訳の場合、借方・貸方の金額を揃える
		SetTandokuSameValue(ln, zeipn);

		total_dsp();

		set_focus(zeipn);

		return TRUE;
	}
	else {
		set_focus(zeipn);
	}

	return TRUE;
}



void CDnpBaseClass::CancelInvNoIME()
{
	m_pSyzInp->CancelInvNoIME();
}


void CDnpBaseClass::_DSGN_MenWariFnc(struct _DenpData *keybuf)
{
	unsigned char syz6 = 0;	// 免税事業者課税仕入れ 控除割合

	syz6 = keybuf->SYDATA._DSGN[6];

	DWORD fncCode = 0;
	if( syz6 & 0x20 ) {
		int tmp = (syz6 & 0xc0);
		tmp >>= 6;
		if( tmp == 0 ) {
			//80%
			fncCode = CD_SP_EXEMPT;
		}

		if( tmp == 1 ) {
			//50%
			fncCode = CD_SP_EXEMPT_50;
		}
		else if( tmp == 2 ) {
			//0%
		}
	}
	if( fncCode ) {
		m_pSy->SyFnc(fncCode);
	}
}



