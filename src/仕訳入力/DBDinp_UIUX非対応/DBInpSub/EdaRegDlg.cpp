// EdaRegDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "resource.h"
#include "EdaRegDlg.h"

#include "ksrcstr.h"
#include "perform_chk.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEdaRegDlg ダイアログ

#include "DBInpSubFunc.h"

extern
int get_strcnt(char* str, int byte);


CEdaRegDlg::CEdaRegDlg(CWnd* pParent /*=NULL*/)
	: ICSDialog(CEdaRegDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEdaRegDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
	::ZeroMemory( &m_ERbuf, sizeof m_ERbuf );

	::ZeroMemory( m_KEzan, sizeof m_KEzan );
	m_bKEzanFlg = FALSE;
	m_bKEzanInp = FALSE;

	m_bDialogOK = FALSE;

	m_bDBupdate	= FALSE;
	m_TkMax = 20000;
}


void CEdaRegDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEdaRegDlg)
	DDX_Control(pDX, IDC_LIST1, m_TkList);
	DDX_Control(pDX, IDC_ICSDBEDTCTRL1, m_DBedt1);
	DDX_Control(pDX, IDC_ICSDBEDTCTRL2, m_DBedt2);
	DDX_Control(pDX, IDC_ICSDBEDTCTRL3, m_DBedt3);
	DDX_Control(pDX, IDC_ICSDBEDTKANA, m_DBkana);
	DDX_Control(pDX, IDC_ICSDBEDTNAME, m_DBname);
	DDX_Control(pDX, IDC_ICSDBEDT_KEZAN, m_DBKezan);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDC_CHK_TKYREG, m_ChkReg);
}

void CEdaRegDlg::InitEdaRegDlg( EDA_RECORD* pErec, CDBZmSub* pZm)
{
	m_pEDrec = pErec;
	m_pZm = pZm;

// 科目枝番のほうを優先し、部門枝番はZVOLの bm_isw の自動登録サインがON のときに登録する
#ifdef BMNBRN_CLOSE
	// 部門枝番時にその科目枝番が未登録か？
	if( m_pEDrec->ed_bmn != -1 ) {
		CString filter;
		filter.Format( "ezkcd = '%s' AND ezecd = %d", m_pEDrec->ed_kcod, m_pEDrec->ed_ecod );

		if( m_pZm->ezrec->Requery( filter, 1 ) != 0 ) {
			//未登録
			m_bKEzanFlg = TRUE;
		}
		else {
			// 登録済み
			l_input( m_KEzan, (void*)(LPCTSTR)m_pZm->ezrec->ezzan );
		}
	}
#endif
}

BEGIN_MESSAGE_MAP(CEdaRegDlg, ICSDialog)
	//{{AFX_MSG_MAP(CEdaRegDlg)
	ON_LBN_DBLCLK(IDC_LIST1, OnDblclkList1)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_ERDRETROFOCUS, RetroFocus )
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdaRegDlg メッセージ ハンドラ

BOOL CEdaRegDlg::OnInitDialog() 
{
//	ControlInit(295,190,-1);

	ICSDialog::OnInitDialog();

	// TODO: この位置に初期化の補足処理を追加してください
	char tmp[128];
	struct _DBKNREC* pKn;

#ifdef BMNBRN_CLOSE
	char	buf[128];
	// 部門名を表示
	if( m_pEDrec->ed_bmn != -1 ) {
		set_codestr( buf, sizeof buf, m_pEDrec->ed_bmn, m_pZm->zvol->bmcol );

		// 部門名称を検索
		sprintf_s( tmp, sizeof tmp, "bnbcd = %d", m_pEDrec->ed_bmn );

		m_pZm->bmname->MoveFirst();
		if( m_pZm->bmname->st != -1 ) {
			do {
				if( m_pZm->bmname->bnbcd == m_pEDrec->ed_bmn ) {
					sprintf_s( tmp, sizeof tmp, "%s", m_pZm->bmname->bnbnm );
					strcat_s( buf, sizeof buf, tmp );

					int n = get_strcnt( buf, 25 );
					buf[n] = '\0';
					break;
				}
			} while( m_pZm->bmname->MoveNext() == 0 );
		}

		GetDlgItem(IDC_STD_BMNAM)->SetWindowText( buf );
	}
	else {
#endif
		GetDlgItem( IDC_STATIC_BMN )->ShowWindow(SW_HIDE);
		GetDlgItem( IDC_STD_BMNAM )->ShowWindow(SW_HIDE);	
#ifdef BMNBRN_CLOSE
	}
#endif

	pKn = DB_PjisToKnrec( m_pEDrec->ed_kcod, m_pZm, TRUE );
	if( pKn ) {
		sprintf_s( tmp, sizeof tmp, "%.14s", pKn->knnam );
		GetDlgItem(IDC_STD_KNAM)->SetWindowText( tmp );
		set_codestr( tmp, sizeof tmp, m_pEDrec->ed_ecod, m_pZm->zvol->edcol );
		GetDlgItem(IDC_STD_EDA)->SetWindowText( tmp );

		tmp[0] = '\0';
		GetDlgItem(IDC_STD_EDNAM)->SetWindowText( tmp );
	}
	else {
		EndDialog( -1 );
	}

	// カナ検索用
	m_DBedt3.ImeEndMode( TRUE );
	// Delete 処理
	m_DBedt1.EnableDelete(0);
	m_DBedt2.EnableDelete(0);
	m_DBedt3.EnableDelete(0);
	m_DBkana.EnableDelete(0);
	m_DBname.EnableDelete(0);
	m_DBKezan.EnableDelete(0);

	m_bKanaSearch = TRUE;

	VARIANT var;
#ifndef BMNBRN_CLOSE
	GetDlgItem( IDC_STATIC_EDAZAN )->SetWindowText( "科目枝番開始残高" );
	var.pbVal = NULL;
	DBdata_set(this, IDC_ICSDBEDT_KEZAN, &var, ICSDBEDT_TYPE_ARITH, 0 );

	GetDlgItem( IDC_STATIC_KEZAN )->ShowWindow(SW_HIDE);
	m_DBKezan.ShowWindow( FALSE );
#else
	// 
	if( m_bKEzanFlg ) {
		GetDlgItem( IDC_STATIC_EDAZAN )->SetWindowText( "部門枝番開始残高" );
		GetDlgItem( IDC_STATIC_KEZAN )->ShowWindow(SW_SHOW);
		m_DBKezan.ShowWindow( TRUE );
	}
	else {
		if( m_pEDrec->ed_bmn != -1 ) {
			GetDlgItem( IDC_STATIC_EDAZAN )->SetWindowText( "部門枝番開始残高" );
			// 科目残高が登録済み
			var.pbVal = (BYTE*)m_KEzan;
			DBdata_set(this, IDC_ICSDBEDT_KEZAN, &var, ICSDBEDT_TYPE_ARITH, 0 );

			GetDlgItem( IDC_STATIC_KEZAN )->ShowWindow(SW_SHOW);
			m_DBKezan.EnableWindow( FALSE );
		}
		else {
			GetDlgItem( IDC_STATIC_EDAZAN )->SetWindowText( "科目枝番開始残高" );
			var.pbVal = NULL;
			DBdata_set(this, IDC_ICSDBEDT_KEZAN, &var, ICSDBEDT_TYPE_ARITH, 0 );

			GetDlgItem( IDC_STATIC_KEZAN )->ShowWindow(SW_HIDE);
			m_DBKezan.ShowWindow( FALSE );
		}
	}
#endif

	BOOL bOpen = FALSE;
	if( ZmsubOwntbOpen( m_pZm, bOpen, FALSE ) != 0 ) {
		EndDialog(-1);
	}
	if( m_pZm->owntb->st != -1 ) {
		int chk = atoi( m_pZm->owntb->code[0] );
		int nState = chk ? BST_CHECKED : BST_UNCHECKED;
		m_ChkReg.SetCheck( nState );
	}

	if( bOpen ) {
		m_pZm->OwnTblClose();
	}

	ICSDialog::OnInitDialogEX();

//END_PERFORMANCE( "edareg::InitDialogEnd()" )

//BEGIN_PERFORMANCE

	return TRUE;
//	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

BEGIN_EVENTSINK_MAP(CEdaRegDlg, ICSDialog)
    //{{AFX_EVENTSINK_MAP(CEdaRegDlg)
	ON_EVENT(CEdaRegDlg, IDC_ICSDBEDTCTRL1, 3 /* SetFocus */, OnSetFocusIcsdbedtctrl1, VTS_NONE)
	ON_EVENT(CEdaRegDlg, IDC_ICSDBEDTCTRL1, 2 /* KillFocus */, OnKillFocusIcsdbedtctrl1, VTS_I4)
	ON_EVENT(CEdaRegDlg, IDC_ICSDBEDTCTRL1, 1 /* Termination */, OnTerminationIcsdbedtctrl1, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CEdaRegDlg, IDC_ICSDBEDTCTRL2, 3 /* SetFocus */, OnSetFocusIcsdbedtctrl2, VTS_NONE)
	ON_EVENT(CEdaRegDlg, IDC_ICSDBEDTCTRL2, 2 /* KillFocus */, OnKillFocusIcsdbedtctrl2, VTS_I4)
	ON_EVENT(CEdaRegDlg, IDC_ICSDBEDTCTRL2, 1 /* Termination */, OnTerminationIcsdbedtctrl2, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CEdaRegDlg, IDC_ICSDBEDTCTRL3, 3 /* SetFocus */, OnSetFocusIcsdbedtctrl3, VTS_NONE)
	ON_EVENT(CEdaRegDlg, IDC_ICSDBEDTCTRL3, 2 /* KillFocus */, OnKillFocusIcsdbedtctrl3, VTS_I4)
	ON_EVENT(CEdaRegDlg, IDC_ICSDBEDTCTRL3, 1 /* Termination */, OnTerminationIcsdbedtctrl3, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CEdaRegDlg, IDC_ICSDBEDTCTRL3, 5 /* ImeEndComposition */, OnImeEndCompositionIcsdbedtctrl3, VTS_I2 VTS_BSTR VTS_BSTR)
	ON_EVENT(CEdaRegDlg, IDC_ICSDBEDTNAME, 3 /* SetFocus */, OnSetFocusIcsdbedtname, VTS_NONE)
	ON_EVENT(CEdaRegDlg, IDC_ICSDBEDTNAME, 1 /* Termination */, OnTerminationIcsdbedtname, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CEdaRegDlg, IDC_ICSDBEDTKANA, 3 /* SetFocus */, OnSetFocusIcsdbedtkana, VTS_NONE)
	ON_EVENT(CEdaRegDlg, IDC_ICSDBEDTKANA, 1 /* Termination */, OnTerminationIcsdbedtkana, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CEdaRegDlg, IDC_ICSDBEDTKANA, 2 /* KillFocus */, OnKillFocusIcsdbedtkana, VTS_I4)
	ON_EVENT(CEdaRegDlg, IDC_ICSDBEDTNAME, 2 /* KillFocus */, OnKillFocusIcsdbedtname, VTS_I4)
	//}}AFX_EVENTSINK_MAP
	ON_EVENT(CEdaRegDlg, IDC_ICSDBEDT_KEZAN, 3, CEdaRegDlg::SetFocusIcsdbedtKezan, VTS_NONE)
	ON_EVENT(CEdaRegDlg, IDC_ICSDBEDT_KEZAN, 2, CEdaRegDlg::KillFocusIcsdbedtKezan, VTS_I4)
	ON_EVENT(CEdaRegDlg, IDC_ICSDBEDT_KEZAN, 1, CEdaRegDlg::TerminationIcsdbedtKezan, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CEdaRegDlg, IDC_ICSDBEDTNAME, 5, CEdaRegDlg::ImeEndCompositionIcsdbedtname, VTS_I2 VTS_BSTR VTS_BSTR)
END_EVENTSINK_MAP()

// 摘要コード入力
void CEdaRegDlg::OnSetFocusIcsdbedtctrl1() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( ! m_bDialogOK )	return;

	GetDlgItem( IDC_STATIC_GD )->ShowWindow(SW_SHOW);
}

void CEdaRegDlg::OnKillFocusIcsdbedtctrl1(long inplen) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( ! m_bDialogOK )	return;

	TkyoNoInp( IDC_ICSDBEDTCTRL1, 0, inplen, 0 );	

	GetDlgItem( IDC_STATIC_GD )->ShowWindow(SW_HIDE);
}

// 摘要番号入力 キーターミネーション
void CEdaRegDlg::OnTerminationIcsdbedtctrl1(long nChar, long inplen, long kst) 
{
	if( ! m_bDialogOK )	return;

	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( ! TkyoNoInp( IDC_ICSDBEDTCTRL1, nChar, inplen, kst ) ) {
		return;
	}

	if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )	
		nChar = VK_F2;

	switch( nChar ) {
	case VK_TAB:
	case VK_RETURN:
	case VK_DOWN :
//	case VK_F3 :
		if( m_ERbuf.er_inpsg[EDR_TKCOD] ) {
			set_focus( IDC_ICSDBEDTCTRL2 );	
		}
		else {
			set_focus( IDC_ICSDBEDTCTRL3 );
		//	set_focus( IDC_ICSDBEDTNAME );
		}
		break;
	case VK_RIGHT:	//カナ検索
		set_focus( IDC_ICSDBEDTCTRL3 );
		break;
	case VK_LEFT:
	case VK_F2 :
		set_focus( IDCANCEL );
		break;

	case VK_PRIOR:
	case VK_NEXT:
		m_TkList.PostMessage( WM_KEYDOWN, nChar );
		break;

	case VK_HOME:	// 全摘要をリストに表示
		{
			CWaitCursor cur;
			if( m_bKanaSearch ) {
				m_pZm->tkrec->Requery("", 0);
				m_bKanaSearch = FALSE;
			}
			m_pZm->tkrec->MoveFirst();
			m_TkList.ResetContent();
			int idx = 0;

			do {
				CString str;
				str.Format( "[%6d] %-40.40s", m_pZm->tkrec->tkcod, m_pZm->tkrec->tkname );
				m_TkList.AddString( str );
				m_TkList.SetItemData(idx++, m_pZm->tkrec->tkcod);
			} while( m_pZm->tkrec->MoveNext() == 0 );

			// カナ検索をクリア
			VARIANT var;
			var.pbVal = NULL;
			DBdata_set( this, IDC_ICSDBEDTCTRL3, &var, ICSDBEDT_TYPE_STRING, 0 );
		}
		break;

	case VK_ESCAPE:
		EndDialog( IDCANCEL );
		break;
	}
}

// 摘要番号入力
long CEdaRegDlg::TkyoNoInp( UINT ID, long nChar, long inplen, long kst )
{
	VARIANT var;
	int		tkcod = 0;

	if( inplen > 0 ) {
		DBdata_get( this, ID, &var, ICSDBEDT_TYPE_STRING, 0 );

		tkcod = atoi( (char*)var.pbVal );
		m_ERbuf.er_tkcod = tkcod;
		m_ERbuf.er_inpsg[EDR_TKCOD] = tkcod > 0 ? TRUE : FALSE;
	}

	// オフフォーカス
	if( !nChar )
	{
		//データセット
		if( m_ERbuf.er_inpsg[EDR_TKCOD] )
		{
			if( m_bKanaSearch ) {
				m_pZm->tkrec->Requery("", 0);
				m_bKanaSearch = FALSE;
			}
			// 摘要検索セット
			EdaTkyoSet( m_ERbuf.er_tkcod );
		}
		else
		{
			var.pbVal = NULL;
			DBdata_set( this, ID, &var, ICSDBEDT_TYPE_STRING, 0 );
		}
		TkyRegCondition();

		return(0);
	}
	else
	{
		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			return VK_F2;
	}

	// 入力
	if( nChar == VK_RETURN || nChar == VK_TAB ) 
	{
	}
	else if( nChar == VK_DELETE )
	{
		var.pbVal = NULL;
		DBdata_set( this, ID, &var, ICSDBEDT_TYPE_STRING, 0 );

		m_ERbuf.er_tkcod = 0;
		m_ERbuf.er_inpsg[EDR_TKCOD] = FALSE;

		// 現時点で残っている枝番名称を手入力部にセット
		VARIANT var;
		DBdata_get( this, IDC_ICSDBEDTNAME, &var, ICSDBEDT_TYPE_STRING, 0 );
		strcpy_s( m_ERbuf.er_name, sizeof m_ERbuf.er_name, (const char*)var.pbVal );
		DBdata_get( this, IDC_ICSDBEDTKANA, &var, ICSDBEDT_TYPE_STRING, 0 );
		strcpy_s( m_ERbuf.er_kana, sizeof m_ERbuf.er_kana, (const char*)var.pbVal );
	}

	TkyRegCondition();

	return( nChar );
}



// カナ検索入力
void CEdaRegDlg::OnSetFocusIcsdbedtctrl3() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( ! m_bDialogOK )	return;
	
}

void CEdaRegDlg::OnKillFocusIcsdbedtctrl3(long inplen) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( ! m_bDialogOK )	return;
	
}

// 摘要かな検索キーターミネーション
void CEdaRegDlg::OnTerminationIcsdbedtctrl3(long nChar, long inplen, long kst) 
{
	if( ! m_bDialogOK )	return;

	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
		nChar = VK_F2;

	VARIANT var;

	switch( nChar ) {
	case VK_TAB:
	case VK_RETURN:
	case VK_DOWN :
//	case VK_F3 :
		if( m_ERbuf.er_inpsg[EDR_TKCOD] ) {
			set_focus( IDC_ICSDBEDTCTRL2 );
		}
		else {
			set_focus( IDC_ICSDBEDTNAME );
		}
		break;
	case VK_UP:
	case VK_LEFT:
	case VK_F2 :
		set_focus( IDC_ICSDBEDTCTRL1 );
		break;
	case VK_DELETE:
		var.pbVal = NULL;
		DBdata_set( this, IDC_ICSDBEDTCTRL3, &var, ICSDBEDT_TYPE_STRING, 0 );
		break;

	case VK_ESCAPE:
		EndDialog( IDCANCEL );
		break;
	}	
}

void CEdaRegDlg::OnImeEndCompositionIcsdbedtctrl3(short nChar, LPCTSTR string, LPCTSTR ystring) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( ! m_bDialogOK )	return;

	// カナ検索
	CString filter;
	filter.Format( "tkana LIKE '%s%%' COLLATE Japanese_BIN", ystring );
	int i = 0;
	VARIANT var;

	m_bKanaSearch = TRUE;
	if( m_pZm->tkrec->Requery( filter, 0 ) == 0 ) {
		i = m_pZm->tkrec->GetRecordCount();
	}
//	m_pZm->tkrec_kanamode() = TRUE;

	m_TkList.ResetContent();
	int idx = 0;

	if( i ) {
		m_pZm->tkrec->MoveFirst();
		do {
			CString str;
	
			int n = get_strcnt( (char*)(LPCTSTR)m_pZm->tkrec->tkname, 40 );

			if( n == 39 )	str.Format( "[%6d] %-39.39s", m_pZm->tkrec->tkcod, m_pZm->tkrec->tkname );
			else			str.Format( "[%6d] %-40.40s", m_pZm->tkrec->tkcod, m_pZm->tkrec->tkname );

			m_TkList.AddString( str );
			m_TkList.SetItemData(idx++, m_pZm->tkrec->tkcod);
		
		} while( m_pZm->tkrec->MoveNext() == 0 );

		var.pbVal = (BYTE*)ystring;
		DBdata_set( this, IDC_ICSDBEDTCTRL3, &var, ICSDBEDT_TYPE_STRING, 0 );
	}
	else {
		var.pbVal = NULL;
		DBdata_set( this, IDC_ICSDBEDTCTRL3, &var, ICSDBEDT_TYPE_STRING, 0 );
	}
}


// 開始残高入力
void CEdaRegDlg::OnSetFocusIcsdbedtctrl2() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( ! m_bDialogOK )	return;
	
}

void CEdaRegDlg::OnKillFocusIcsdbedtctrl2(long inplen) 
{
	if( ! m_bDialogOK )	return;

	ValInp( IDC_ICSDBEDTCTRL2, 0, inplen, 0 );	
}

// 開始残高入力 キーターミネーション
void CEdaRegDlg::OnTerminationIcsdbedtctrl2(long nChar, long inplen, long kst) 
{
	if( ! m_bDialogOK )	return;

	if( ! ValInp( IDC_ICSDBEDTCTRL2, nChar, inplen, kst ) ) 
		return;

	if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
		nChar = VK_F2;

	switch( nChar ) {
	case VK_TAB:
	case VK_RETURN:
	case VK_DOWN :
//	case VK_F3 :
		if( m_bKEzanFlg )
			set_focus( IDC_ICSDBEDT_KEZAN );
		else {
			if( m_ERbuf.er_inpsg[EDR_TKCOD] )
				set_focus( IDOK );
			else
				set_focus( IDC_CHK_TKYREG );
		}
		break;
	case VK_UP:
		if( m_ERbuf.er_inpsg[EDR_TKCOD] )
			set_focus( IDC_ICSDBEDTCTRL1 );
		else
			set_focus( IDC_ICSDBEDTNAME );
		break;
	case VK_LEFT:
	case VK_F2 :
		if( m_ERbuf.er_inpsg[EDR_TKCOD] )
			set_focus( IDC_ICSDBEDTCTRL3 );
		else
			set_focus( IDC_ICSDBEDTKANA );
		break;

	case VK_ESCAPE:
		EndDialog( IDCANCEL );
		break;
	}

}


// 部門枝番のとき、科目枝番が未登録の場合
//
void CEdaRegDlg::SetFocusIcsdbedtKezan()
{
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( ! m_bDialogOK )	return;
}

void CEdaRegDlg::KillFocusIcsdbedtKezan(long inplen)
{
	if( ! m_bDialogOK )	return;

	ValInp( IDC_ICSDBEDT_KEZAN, 0, inplen, 0 );	
}

void CEdaRegDlg::TerminationIcsdbedtKezan(long nChar, long inplen, long kst)
{
	if( ! m_bDialogOK )	return;

	if( ! ValInp( IDC_ICSDBEDT_KEZAN, nChar, inplen, kst ) ) 
		return;

	if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )	
		nChar = VK_F2;

	switch( nChar ) {
	case VK_TAB:
	case VK_RETURN:
	case VK_DOWN :
//	case VK_F3 :
		if( m_ERbuf.er_inpsg[EDR_TKCOD] )
			set_focus( IDOK );
		else
			set_focus( IDC_CHK_TKYREG );
		break;
	case VK_UP:
		set_focus( IDC_ICSDBEDTCTRL2 );
		break;
	case VK_LEFT:
	case VK_F2 :
		set_focus( IDC_ICSDBEDTCTRL2 );
		break;
	}

}


//
// 金額　入力＆オフフォーカス
long CEdaRegDlg::ValInp( UINT ID, long nChar, long inplen, long kst )
{
	VARIANT var;
	char	tmp[6] = {0};

	char* pval;
	BOOL* pbInp;

	if( ID == IDC_ICSDBEDTCTRL2 ) {
		pval	= m_ERbuf.er_zan;
		pbInp	= &m_ERbuf.er_inpsg[EDR_ZAN];
	}
	else {
		pval	= m_KEzan;
		pbInp	= &m_bKEzanInp;
	}

	if( inplen > 0 ) {
		l_defn( 0x16 );
		DBdata_get( this, ID, &var, ICSDBEDT_TYPE_ARITH, 0 );
		memcpy( tmp, var.pbVal, sizeof tmp );

		memcpy( pval, tmp, 6 );
		*pbInp = TRUE;
	}
	
	// オフフォーカス
	if( !nChar )
	{

		//データセット
		if( *pbInp )
		{
			var.pbVal = (BYTE*)pval;
			DBdata_set( this, ID, &var, ICSDBEDT_TYPE_ARITH, 0 );
		}
		else
		{
			var.pbVal = NULL;
			DBdata_set( this, ID, &var, ICSDBEDT_TYPE_ARITH, 0 );
		}
		return(0);
	}
	else
	{
		if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )
			return VK_F2;
	}

	// 入力
	if( nChar == VK_RETURN || nChar == VK_TAB ) 
	{

	}
	else if( nChar == VK_DELETE )
	{
		var.pbVal = NULL;
		DBdata_set( this, ID, &var, ICSDBEDT_TYPE_ARITH, 0 );

		memset( pval, (char)0, 6 );
		*pbInp = FALSE;
	}

	return( nChar );
}


void CEdaRegDlg::OnOK() 
{
	if( m_ERbuf.er_inpsg[EDR_TKCOD] ) {
		m_pEDrec->ed_tkcod = m_ERbuf.er_tkcod;
	}
	else {
		m_pEDrec->ed_tkcod	= 0;
	}
	if( ! m_pEDrec->ed_tkcod ) {
		strcpy_s(m_pEDrec->ed_name, sizeof m_pEDrec->ed_name, m_ERbuf.er_name );
		strcpy_s(m_pEDrec->ed_kana, sizeof m_pEDrec->ed_kana, m_ERbuf.er_kana );
	}
	//同じ枝番に、同じ摘要があるかを調べる。

	CString sql;
	sql.Format("ezkcd = '%s'", m_pEDrec->ed_kcod);
	m_pZm->ezrec->Requery(sql);
	CString msg, edaname, edastr;
	char edacode[24] = { 0 };
	CStringArray dupliEdaNam;

	CString chkEdaName;

	if (m_pEDrec->ed_tkcod) {
		if (m_bKanaSearch) {
			m_pZm->tkrec->Requery("", 0);
			m_bKanaSearch = FALSE;
			tkquery_flag() = FALSE;
		}
		//
		if (tkyngetr(m_pEDrec->ed_tkcod, m_pZm) == 0) {
			chkEdaName = m_pZm->tkrec->tkname;
		}
	}
	else if (m_pEDrec->ed_name[0]) {
		chkEdaName = m_pEDrec->ed_name;
	}

	m_pZm->ezrec->MoveFirst();

	if(! chkEdaName.IsEmpty() && m_pZm->ezrec->st != -1 ) {
		do {
			if (m_pZm->ezrec->eztcd != 0 ) {	
				if (m_bKanaSearch) {
					m_pZm->tkrec->Requery("", 0);
					m_bKanaSearch = FALSE;
					tkquery_flag() = FALSE;
				}
				//
				if (tkyngetr(m_pZm->ezrec->eztcd, m_pZm) == 0) {
					edaname = m_pZm->tkrec->tkname;
				}
			}
			else {
				edaname = m_pZm->ezrec->ezname;
			}

			if (chkEdaName.Compare(edaname) == 0) {
				set_codestr(edacode, sizeof edacode, m_pZm->ezrec->ezecd, m_pZm->zvol->edcol);
				edastr.Format("[%s] %s", edacode, edaname);
				dupliEdaNam.Add(edastr);
			}

		} while (m_pZm->ezrec->MoveNext() == 0);
	}

	if (dupliEdaNam.GetCount() > 0) {
		msg.Format("同じ科目に同一名称が既に登録済みです。\r\nこのまま登録を行ってもよろしいですか？\r\n\r\n");
		msg += "重複している枝番コード・名称：";

		for (int n = 0; n < dupliEdaNam.GetCount(); n++) {
			if( n > 0 ) {
				if (n == 10) {
					msg += "　…";
					break;
				}
				msg += "\r\n　　　　　　　　　　　　　　　";
			}
			msg += dupliEdaNam[n];
		}
		int st = ICSMessageBox(msg, MB_YESNO | MB_DEFBUTTON2, 0, 0, this);
		if (st == IDNO)
			return;
	}

	struct _DBKNREC* pKn;
	if( (pKn = DB_PjisToKnrec( m_pEDrec->ed_kcod, m_pZm, TRUE )) == NULL ) {
		ICSMessageBox("科目名称を取得できません！",0,0,0,this);
		EndDialog( IDCANCEL );
		return;
	}

	UpdateData();

	int stchk = m_ChkReg.GetCheck();
	if( stchk == 1 ) {
		int tkcod;
		if( TkyregJob( tkcod ) != 0 )	return;
		else {
			if( tkcod > 0 )	m_pEDrec->ed_tkcod = tkcod;
		}
	}

	BOOL bOpen = FALSE;
	if( ZmsubOwntbOpen( m_pZm, bOpen, TRUE ) != 0 ) {
		EndDialog(-1);
	}
	m_pZm->owntb->Edit();
	m_pZm->owntb->code[0] = stchk ? "1" : "0";
	m_pZm->owntb->Update();

	if( bOpen ) {
		m_pZm->OwnTblClose();
	}

//	if( pKn->knatt & 0x01 )	l_neg( m_ERbuf.er_zan );
	memcpy( m_pEDrec->ed_zan, m_ERbuf.er_zan, 6 );
	
	ICSDialog::OnOK();
}

BOOL CEdaRegDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	if( pMsg->message == WM_KEYDOWN ) {

//_DEBUG_FILEOUT( "edareg::Pretrans\n" );
		if( pMsg->wParam == VK_ESCAPE ) {
			EndDialog(IDCANCEL);
			return TRUE;
		}

		CWnd* pwnd = GetFocus();
		int id;
		if( pwnd ) {
			id = pwnd->GetDlgCtrlID();
		
			switch( id ) {
			case IDOK:
				if( ! TerminationOnOK( pMsg->wParam ) )
					return TRUE;
				break;
			case IDCANCEL:
				if( ! TerminationOnCancel( pMsg->wParam ) )
					return TRUE;
				break;
			case IDC_LIST1:
				if( ! TerminationList1( pMsg->wParam ) )
					return TRUE;
				break;
			case IDC_CHK_TKYREG:
				if( ! TerminationCheckTkyReg( pMsg->wParam ) )
					return TRUE;
				break;
			}
		}	
	}
	return ICSDialog::PreTranslateMessage(pMsg);
}


long CEdaRegDlg::TerminationOnOK( long nChar )
{
	if( nChar == VK_TAB && is_SHIFT() )
		nChar = VK_F2;

	switch( nChar ) {
	case VK_UP:
	case VK_F2:
	case VK_LEFT:
		if( m_bKEzanFlg ) {
			set_focus( IDC_ICSDBEDT_KEZAN );
		}
		else {
			if( m_ERbuf.er_inpsg[EDR_TKCOD] )
				set_focus( IDC_ICSDBEDTCTRL2 );
			else
				set_focus( IDC_CHK_TKYREG );
		}
		nChar = 0;
		break;
	case VK_RIGHT:
//	case VK_F3:
	case VK_DOWN:
		set_focus( IDCANCEL );
		nChar = 0;
		break;
	}
	return nChar;
}

long CEdaRegDlg::TerminationOnCancel( long nChar )
{
	if( nChar == VK_TAB && is_SHIFT() )
		nChar = VK_F2;

	switch( nChar ) {
	case VK_UP:
	case VK_F2:
	case VK_LEFT:
		set_focus( IDOK );
		nChar = 0;
		break;
	case VK_RIGHT:
//	case VK_F3:
	case VK_DOWN:
		set_focus( IDC_ICSDBEDTCTRL1 );
		nChar = 0;
		break;
	case VK_RETURN:
		EndDialog( IDCANCEL );
		nChar = 0;
		break;
	}
	return nChar;
}


long CEdaRegDlg::TerminationList1( long nChar )
{
	if( nChar == VK_TAB && is_SHIFT() )
		nChar = VK_F2;

	switch( nChar ) {
	case VK_F2:
	case VK_LEFT:
	case VK_RIGHT:
//	case VK_F3:
		set_focus( IDC_ICSDBEDTCTRL1 );
		nChar = 0;
		break;
	case VK_RETURN:
		OnDblclkList1();
		nChar = 0;
		break;
	}
	return nChar;
}


long CEdaRegDlg::TerminationCheckTkyReg( long nChar )
{
	if( nChar == VK_TAB && is_SHIFT() )
		nChar = VK_F2;

	switch( nChar ) {
	case VK_F2:
	case VK_LEFT:
	case VK_UP:
//	case VK_F3:
		set_focus( IDC_ICSDBEDTCTRL2 );
		nChar = 0;
		break;
	case VK_DOWN:
	case VK_RIGHT:
	case VK_RETURN:
	case VK_TAB:
		set_focus( IDOK );
		nChar = 0;
		break;
	}
	return nChar;
}


void CEdaRegDlg::OnDblclkList1() 
{
	
	int nIndex = m_TkList.GetCurSel();
	int tkcod = 0;

	if( nIndex >= 0 ) {
		CString tmp, num;
		tkcod = m_TkList.GetItemData(nIndex);
		//m_TkList.GetText( nIndex, tmp );
		//num = tmp.Mid( 1, 6 );
		//tkcod = atoi( num );
		//if( m_bKanaSearch ) {
		//	m_pZm->tkrec->SetAbsolutePosition( nIndex+1 );
		//	if( m_pZm->tkrec->st != -1 ) {
		//		tkcod = m_pZm->tkrec->tkcod;
		//	}
		//}
		//else {
		//	m_pZm->tkrec->SetAbsolutePosition(nIndex + 1);
		//	if( m_pZm->tkrec->st != -1 ) {
		//		tkcod = m_pZm->tkrec->tkcod;
		//	}
		//}

		m_ERbuf.er_tkcod = tkcod;
		m_ERbuf.er_inpsg[EDR_TKCOD] = tkcod > 0 ? TRUE : FALSE;

		EdaTkyoSet( tkcod );
		set_focus( IDC_ICSDBEDTCTRL2 );
	}
}


void CEdaRegDlg::EdaTkyoSet( int tkcod )
{
	int set_ok = 0;
	VARIANT var;
	char buf[128];

	if( m_pZm->tkrec->MoveFirst() == 0 ) {
		 do {
			if( tkcod == m_pZm->tkrec->tkcod ) {
//				GetDlgItem(IDC_STD_EDNAM)->SetWindowText( m_pZm->tkrec->tkname );	

				var.pbVal = (BYTE*)(LPCTSTR)m_pZm->tkrec->tkname ;
				DBdata_set( this, IDC_ICSDBEDTNAME, &var, ICSDBEDT_TYPE_STRING, 0 );

				var.pbVal = (BYTE*)(LPCTSTR)m_pZm->tkrec->tkana ;
				DBdata_set( this, IDC_ICSDBEDTKANA, &var, ICSDBEDT_TYPE_STRING, 0 );

				sprintf_s( buf, sizeof buf, "%d", tkcod );
				var.pbVal = (BYTE*)buf;
				DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );
				set_ok = 1;
				TkyRegCondition();
				break;
			}
		 } while( m_pZm->tkrec->MoveNext() == 0 );
	}

	if( ! set_ok ) {	 
		m_ERbuf.er_inpsg[EDR_TKCOD] = FALSE;
//		GetDlgItem(IDC_STD_EDNAM)->SetWindowText( "" );	
		var.pbVal = (BYTE*)NULL;
		DBdata_set( this, IDC_ICSDBEDTCTRL1, &var, ICSDBEDT_TYPE_STRING, 0 );

//		var.pbVal = (BYTE*)NULL;
//		DBdata_set( this, IDC_ICSDBEDTNAME, &var, ICSDBEDT_TYPE_STRING, 0 );
//		var.pbVal = (BYTE*)NULL;
//		DBdata_set( this, IDC_ICSDBEDTKANA, &var, ICSDBEDT_TYPE_STRING, 0 );
	}
}


LRESULT CEdaRegDlg::RetroFocus( WPARAM wParam, LPARAM lParam )
{
	c_set( wParam );
	return 1;
}

void CEdaRegDlg::set_focus( UINT ID )
{
	PostMessage( WM_ERDRETROFOCUS, ID );
}


void CEdaRegDlg::OnSetFocusIcsdbedtname() 
{
	if( ! m_bDialogOK )	return;

	if( m_ERbuf.er_inpsg[EDR_TKCOD] ) {
		set_focus(IDC_ICSDBEDTCTRL2);
	}	
}

void CEdaRegDlg::OnKillFocusIcsdbedtname(long inplen) 
{
	if( ! m_bDialogOK )	return;

	EdaNameInp(inplen);	
}

// 枝番名称 入力
void CEdaRegDlg::ImeEndCompositionIcsdbedtname(short nChar, LPCTSTR string, LPCTSTR ystring)
{
	if( ! m_bDialogOK )	return;

	if( ! m_ERbuf.er_inpsg[EDR_TKCOD] ) {
		if( m_ERbuf.er_name[0] == '\0' ) {
			if( m_ERbuf.er_kana[0] == '\0' ) {
				::ZeroMemory( m_ERbuf.er_kana, sizeof m_ERbuf.er_kana );
				strncpy_s( m_ERbuf.er_kana, sizeof m_ERbuf.er_kana, ystring, 4 );
			} 
		}
	}
}


void CEdaRegDlg::OnTerminationIcsdbedtname(long nChar, long inplen, long kst) 
{
	if( ! m_bDialogOK )	return;

	EdaNameInp(inplen);

	if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )	
		nChar = VK_F2;

	VARIANT var;

	switch( nChar ) {
	case VK_TAB:
	case VK_RETURN:
	case VK_DOWN :
//	case VK_F3 :
		if( ! m_ERbuf.er_inpsg[EDR_TKCOD] )
			set_focus( IDC_ICSDBEDTKANA );
		else
			set_focus( IDC_ICSDBEDTCTRL2 );
		break;
	case VK_UP:
		set_focus( IDC_ICSDBEDTCTRL1 );
	case VK_LEFT:
	case VK_F2 :
		set_focus( IDC_ICSDBEDTCTRL3 );
		break;
	case VK_DELETE:
		var.pbVal = NULL;
		DBdata_set( this, IDC_ICSDBEDTNAME, &var, ICSDBEDT_TYPE_STRING, 0 );
		::ZeroMemory( m_ERbuf.er_name, sizeof m_ERbuf.er_name );
		break;

	case VK_ESCAPE:
		EndDialog( IDCANCEL );
		break;
	}	
}


int CEdaRegDlg::EdaNameInp(long inplen)
{
	VARIANT var;

	if( ! m_ERbuf.er_inpsg[EDR_TKCOD] ) {
	//	if( inplen > 0 ) {
			DBdata_get( this, IDC_ICSDBEDTNAME, &var, ICSDBEDT_TYPE_STRING, 0 );
			int len = strlen( (const char*)var.pbVal );
			memcpy( m_ERbuf.er_name, var.pbVal, len );
			m_ERbuf.er_name[len] = '\0';

			if( m_ERbuf.er_kana[0] ) {
				var.pbVal = (BYTE*)m_ERbuf.er_kana;
				DBdata_set( this, IDC_ICSDBEDTKANA, &var, ICSDBEDT_TYPE_STRING, 0 );
			}
	//	}
	}

	return 0;
}


void CEdaRegDlg::OnSetFocusIcsdbedtkana() 
{
	if( ! m_bDialogOK )	return;

	if( m_ERbuf.er_inpsg[EDR_TKCOD] ) {
		set_focus(IDC_ICSDBEDTCTRL2);
	}	
}

void CEdaRegDlg::OnKillFocusIcsdbedtkana(long inplen) 
{
	if( ! m_bDialogOK )	return;

	EdaKanaInp( inplen );	
}

void CEdaRegDlg::OnTerminationIcsdbedtkana(long nChar, long inplen, long kst) 
{
	if( ! m_bDialogOK )	return;

	EdaKanaInp( inplen );

	if( nChar == VK_TAB && (kst & ICSDBEDT_KST_SHIFT) )	
		nChar = VK_F2;

	VARIANT var;

	switch( nChar ) {
	case VK_TAB:
	case VK_RETURN:
	case VK_DOWN :
//	case VK_F3 :
		set_focus( IDC_ICSDBEDTCTRL2 );
		break;
	case VK_UP:
	case VK_LEFT:
	case VK_F2 :
		if( ! m_ERbuf.er_inpsg[EDR_TKCOD] )
			set_focus( IDC_ICSDBEDTNAME );
		else
			set_focus( IDC_ICSDBEDTCTRL1 );
		break;
	case VK_DELETE:
		var.pbVal = NULL;
		DBdata_set( this, IDC_ICSDBEDTKANA, &var, ICSDBEDT_TYPE_STRING, 0 );
		::ZeroMemory( m_ERbuf.er_kana, sizeof m_ERbuf.er_kana );
		break;

	case VK_ESCAPE:
		EndDialog( IDCANCEL );
		break;
	}		
}


int CEdaRegDlg::EdaKanaInp(long inplen)
{
	VARIANT var;

	if( ! m_ERbuf.er_inpsg[EDR_TKCOD] ) {
		if( inplen > 0 ) {
			DBdata_get( this, IDC_ICSDBEDTKANA, &var, ICSDBEDT_TYPE_STRING, 0 );
			int len = strlen( (const char*)var.pbVal );
			memcpy( m_ERbuf.er_kana, var.pbVal, len );
			m_ERbuf.er_kana[len] = '\0';
		}
	}

	return 0;
}


void CEdaRegDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
//END_PERFORMANCE("Edareg::OnShowWindow")

	ICSDialog::OnShowWindow(bShow, nStatus);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	if( bShow ) {
		m_bDialogOK = TRUE;

	}
}


//摘要登録ボタン
int CEdaRegDlg::TkyRegCondition()
{
	BOOL bEnable;

	if( ! m_ERbuf.er_inpsg[EDR_TKCOD] ) {
		bEnable = TRUE;
	}
	else {
		bEnable = FALSE;
	}
	GetDlgItem(IDC_CHK_TKYREG)->EnableWindow(bEnable);
	
	return 0;
}


//摘要登録
//	 int& tkycode	追加・置換したコード
//
int CEdaRegDlg::TkyregJob( int& tkycode )
{

	if( m_ERbuf.er_inpsg[EDR_TKCOD] ) {
		tkycode = m_ERbuf.er_tkcod;
		return 0;
	}

	if( ! m_ERbuf.er_name[0] && ! m_ERbuf.er_kana[0] ) {
		ICSMessageBox("[摘要登録]\r\n\r\n摘要が入力されていません！",0,0,0,this);
		return -1;
	}

	char buf[64] = {0};
	memcpy( buf, m_ERbuf.er_name, sizeof m_ERbuf.er_name );

	int tkcod, ret, st;
	//同一摘要の検索
	ret = SrchTKname( buf, &tkcod );
	CString str;

	tkycode = 0;

	if( ret == CKsrcStr::EV_ADD || ! ret ) {

		//追加用摘要コードを セットする
		m_pZm->tkrec->MoveLast();
		int cnt = m_pZm->tkrec->GetRecordCount();
		int tknum;

		if( cnt <= 0 )	tknum = 0;
		else			tknum = m_pZm->tkrec->tkcod;

		if( cnt < m_TkMax ) {
			if( (tknum+1) < 999999 )	// 6桁まで入力
				tknum++;
			else {
				m_pZm->tkrec->MovePrev();
				if( m_pZm->tkrec->st != -1 ) {
					do {
						if( (tknum - m_pZm->tkrec->tkcod) > 1 ) {
							tknum = (m_pZm->tkrec->tkcod + 1);
							break;
						}
						tknum = m_pZm->tkrec->tkcod;
					} while( m_pZm->tkrec->MovePrev() == 0 );
				}
			}
		}

		if( ! tkyngetr( tknum, m_pZm) ) {
			str.Format( "[摘要登録]\r\n\r\n指定された摘要番号(%d)は登録されています。\r\n摘要の置換をしますか？", tknum );

			st = ICSMessageBox( str, MB_YESNO, 0,0,this );
			if( st == IDYES ) {
				m_pZm->tkrec->Edit();
				m_pZm->tkrec->tkname	= buf;
				m_pZm->tkrec->tkana		= m_ERbuf.er_kana;
				m_pZm->tkrec->Update();
				m_bDBupdate = TRUE;

				m_pZm->tkrec->Requery( "", 0 );
				tkycode = tknum;
			}
		}
		else if( cnt < m_TkMax ) 
		{
			// 新規に追加
			m_pZm->tkrec->AddNew();
			m_pZm->tkrec->tkcod		= tknum;
			m_pZm->tkrec->tkname	= buf;
			m_pZm->tkrec->tkana		= m_ERbuf.er_kana;
			m_pZm->tkrec->Update();
			m_bDBupdate = TRUE;

			m_pZm->tkrec->Requery( "", 0 );
			tkycode = tknum;
		}		
		else {
			st = ICSMessageBox("[摘要登録]\r\n\r\n最大登録数まで登録されいるため登録できません！",0,0,0,this);	
		}
	}
	else if( ret == CKsrcStr::EV_REP ) {
		//摘要置換
		if( ! tkyngetr( tkcod, m_pZm) ) {
			m_pZm->tkrec->Edit();
			m_pZm->tkrec->tkname	= buf;
			m_pZm->tkrec->tkana		= m_ERbuf.er_kana;
			m_pZm->tkrec->Update();
			m_bDBupdate = TRUE;

			m_pZm->tkrec->Requery( "", 0 );
			tkycode = tkcod;
		}
	}
	else if( ret == CKsrcStr::EV_CANCEL ) {
		return -1;
	}

	return 0;
}



// 摘要名称サーチ
//	char* teki ..... チェック用 摘要名称
//	int* tkno ...... 置換が選ばれた場合の、その摘要番号
//	-------------------------------------------------------------
//	返送値は KsrcStr.h の enum KEnd_Val
//			EV_ADD ..... 追加
//			EV_REP ..... 置換
//			EV_OTHER ... 他登録
//			EV_CANCEL... キャンセル	
//			0 .......... 同一なし 
//
int CEdaRegDlg::SrchTKname( char* teki, int* tkno )
{
	int tkcnt;
	CStringArray	strArray;

	if( m_bKanaSearch ) {
		m_pZm->tkrec->Requery("", 0);
		m_bKanaSearch = FALSE;
		tkquery_flag() = FALSE;
	}
	tkcnt = get_tknum( m_pZm );

	CTKREC* ptk;
	ptk = m_pZm->tkrec;
	ptk->MoveFirst();

	if( ptk->st != -1 ) {
		do {
			
			CString strName;
			strName.Format( "%.40s", ptk->tkname );

			// 摘要名称文字列の完全なマッチングをとる
			if( strcmp( teki, strName )) continue;

			CString str;
			str.Format( "%6d   %-40.40s  [%-4.4s]", ptk->tkcod, strName, ptk->tkana );
			strArray.Add( str );
				
		} while( ptk->MoveNext() == 0 );
	}

	int	maxflg = 0;
	if( tkcnt >= m_TkMax ) maxflg = 1;

	int	st = 0;
	int	cnt = strArray.GetSize();
	if( cnt ) {

		ICSMessageBox( _T( "同様な摘要辞書が見つかりました！\r\n\r\n一覧を表示します。" ), MB_ICONINFORMATION, 0,0,this );

		// 摘要表示
		CKsrcStr	Tkstr( this );

		Tkstr.strArray.Copy( strArray );
		Tkstr.Maxflg = maxflg;
		Tkstr.bOther = FALSE;
		RECT	rc1;
		GetDlgItem( IDC_LIST1 )->GetWindowRect( &rc1 );

		Tkstr.posx = rc1.left;
		Tkstr.posy = rc1.top;

		// 返送値は KsrcStr.h の enum KEnd_Val
		st = Tkstr.DoModal();

		*tkno = Tkstr.m_RecNo;
	}

	return st;
}



