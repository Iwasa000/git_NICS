// SubDataDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DBDinp.h"

#include "SubDataInp.h"
#include "SubDataDlg.h"

#include "MstRw.h"

#include "input1.h"
#include "external.h"

#include "icsdisp.h"
#include "icsdbedt.h"

// CSubDataDlg ダイアログ
///////////////////////////////////////////////////////////////
//
//
#define SD_PN_MAX	SD_HUSEN_PN+1


static struct _InpCtl SD_INPCTL[SD_PN_MAX] = {
	{IDC_ICSDBEDT_TGNO,0,0,0,-1},	// 手形番号
	{IDC_ICSDBEDT_TGDATE,0,0,0,-1},	// 手形期日
	{IDC_ICSDBEDT_HUSEN,0,0,0,-1},	// 付箋
};



IMPLEMENT_DYNAMIC(CSubDataDlg, ICSDialog)

CSubDataDlg::CSubDataDlg(CWnd* pParent /*=NULL*/, int type)
	: ICSDialog(CSubDataDlg::IDD, pParent)
{
	NOW_PN = 0;
	PREV_PN = 0;
	m_MODEFY_OK = FALSE;
	m_THIS_DIALOG_OK = FALSE;

	m_Dispflg = FALSE;
	m_DlgRect.SetRectEmpty();
	m_pHusenInp = NULL;
	m_pParent = pParent;

	m_Type = type;

	Create( CSubDataDlg::IDD, pParent );

	//アクセラレータテーブルロード
	m_hAccelTable = ::LoadAccelerators( AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
}


CSubDataDlg::~CSubDataDlg()
{
	//付箋入力画面を削除
	if (m_pHusenInp ) {
		delete m_pHusenInp;
		m_pHusenInp = NULL;
	}
}

void CSubDataDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSubDataDlg, ICSDialog)
	ON_MESSAGE(WM_HSINP_INEND, OnHsInpInEnd)
	ON_MESSAGE(WM_RETROFOCUS, RetroFocus)
END_MESSAGE_MAP()


// CSubDataDlg メッセージ ハンドラ

BOOL CSubDataDlg::OnInitDialog()
{
	ICSDialog::OnInitDialog();

	if (m_pHusenInp == NULL)
	{
		if( !(m_pHusenInp = new CHusenInp()) )
		{
			ermset( ERROR_MEMORY, "付箋入力オブジェクトが作成できません。");
			EndDialog(-1);
			return TRUE;
		}
	}

	if( m_pHusenInp->SetPara( pDBzm, this ) != 0 ) {
		ermset( ERROR_MEMORY, "付箋入力オブジェクトが作成できません。");
		EndDialog(-1);
		return TRUE;
	}

	// タイトルの色を変更
	ICSColorInfo info;

	((CDBDinpApp*)AfxGetApp())->GetStanderdColor( info );
	COLORREF cl = info.TitleColor();

	((CICSDisp*)GetDlgItem(IDC_ICSDISP8CTRL1))->SetBackColor( cl );
	((CICSDisp*)GetDlgItem(IDC_ICSDISP8CTRL2))->SetBackColor( cl );

	((CICSDBEDT*)GetDlgItem( IDC_ICSDBEDT_TGNO ))->EnableDelete(0);;
	((CICSDBEDT*)GetDlgItem( IDC_ICSDBEDT_TGDATE ))->EnableDelete(0);;

	//ダイアログ大きさの調整
	if( m_Type ) {
		CRect dlgRect, rect;

		GetDlgItem( IDC_ICSDISP8CTRL2 )->GetWindowRect( &rect );
		int x = rect.right += 2;

		GetWindowRect( &dlgRect );

		int tegata_wd = x - dlgRect.left;
		int husen_wd = dlgRect.right - x;

		double xRate = (double)2.36;

		int new_wd = husen_wd * xRate;

//MyTrace( "CSubDataDlg --> dlgRect.left %d, ctrl right = %d, t = %d, hu = %d\n", dlgRect.left, rect.right, tegata_wd, husen_wd );

		MoveWindow( dlgRect.left, dlgRect.top, tegata_wd+new_wd, dlgRect.Height() );
	}

	ICSDialog::OnInitDialogEX();

	// 付箋ダイアログ
/*
	CRect rect;

	GetDlgItem( IDC_ICSDISP8CTRL2 )->GetWindowRect( &rect );
	ScreenToClient( &rect );
	rect.right += 2;
	rect.left = rect.right;
	rect.bottom = rect.top = 0;

	m_pHusenInp->DialogON( m_MODEFY_OK, &rect, HSN_BOTTOMRIGHT, HSN_HSEL_PN );
*/
	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CSubDataDlg::PostNcDestroy()
{
	ICSDialog::PostNcDestroy();

	delete this;
}


void CSubDataDlg::HusenDlgDisp()
{
	CRect rect;

	GetDlgItem( IDC_ICSDISP8CTRL2 )->GetWindowRect( &rect );
	ScreenToClient( &rect );
	rect.right += 2;
	rect.left = rect.right;
	rect.bottom = rect.top = 0;
	
	set_nowpn( SD_HUSEN_PN );

	m_pHusenInp->DialogON( m_MODEFY_OK, &rect, HSN_BOTTOMRIGHT, HSN_HSEL_PN );

}


// IDC get
struct _InpCtl *CSubDataDlg::getINP_CTL( int pn )
{
	if( pn >= 0 && pn < SD_PN_MAX )
		return( &SD_INPCTL[pn] );
	else
		return( &SD_INPCTL[0] );	//ERROR
}

void CSubDataDlg::set_fastfocus( int pn )
{
	m_THIS_DIALOG_OK = TRUE;

	PostMessage( WM_RETROFOCUS, pn );
}

void CSubDataDlg::set_nowfocus()
{
	set_focus( get_nowpn() );
}

// カーソルセット
void CSubDataDlg::set_focus( int pn )
{
	if( pn >= 0 )
	{
		PostMessage(WM_RETROFOCUS, pn);
	}
}


// ユーザーメッセージ フォーカス処理
LRESULT CSubDataDlg::RetroFocus( WPARAM wParam, LPARAM lParam )
{
	CRect rect;

	switch( wParam ) {
	case SD_TGNO_PN:
	case SD_TGDATE_PN:
		c_set( getINP_CTL( wParam )->IDC_X );
		break;
	case SD_HUSEN_PN:
		HusenDlgDisp();
		break;
	}

	return 1;
}


// 現在ポジションセット
int CSubDataDlg::set_nowpn( int pn )
{
	PREV_PN = NOW_PN;
	NOW_PN = pn;

	return( PREV_PN );	//直前のポジション
}

// 現在ポジションゲット
int CSubDataDlg::get_nowpn( void )
{
	return( NOW_PN );
}

// 直前のポジションゲット
int CSubDataDlg::get_prevpn()
{
	return( PREV_PN );
}

//
//	現在の仕訳データをセット
//
void CSubDataDlg::set_cdata( BOOL bMdfyOK, CDBINPDataRec* data )
{
	m_Data = *data;
	m_MODEFY_OK = bMdfyOK;

	char buf[128] = {0};
	VARIANT var;

	// 手形部分にデータをセット
	if( ! m_Data.m_rnumber.IsEmpty() ) {
		strcpy_s( (char*)buf, sizeof buf, m_Data.m_rnumber );
		var.pbVal = (BYTE*)buf;
	}
	else {
		var.pbVal = NULL;
	}
	DBdata_set( this, IDC_ICSDBEDT_TGNO, &var, ICSDBEDT_TYPE_STRING, 0 );	

	if( m_Data.m_tdate != -1 ) {
		int gen_tgdate, d_gengo;
		CVolDateDB vd;
		vd.db_datecnvGen( 0, m_Data.m_tdate, &d_gengo, &gen_tgdate, 0, 0 );
		sprintf_s( (char*)buf, sizeof buf, "%06d", gen_tgdate );
		var.pbVal = (BYTE*)buf;
		DBdata_set( this, IDC_ICSDBEDT_TGDATE, &var, ICSDBEDT_TYPE_STRING, 0 );	

		IntsrekitoGenstr( buf, sizeof buf, m_Data.m_tdate );
		DBSetDispString( this, IDC_ICSDBEDT_TGDATE, buf );
	}
	else {
		var.pbVal = NULL;
		DBdata_set( this, IDC_ICSDBEDT_TGDATE, &var, ICSDBEDT_TYPE_STRING, 0 );	
		::ZeroMemory( buf, sizeof buf );
		DBSetDispString( this, IDC_ICSDBEDT_TGDATE, buf );
	}

	// 付箋ダイアログにデータセット
	HSEN_DATA crec = {0}, srec = {0};

	crec.hsen_inpsg = TRUE;
	crec.hsen_sel = (m_Data.m_dsign[7]&0x0f);
	strcpy_s( crec.hsen_cmt, sizeof crec.hsen_cmt, m_Data.m_tag );

	srec.hsen_sel = (m_Data.m_dsign[7]&0x0f);
	strcpy_s( srec.hsen_cmt, sizeof srec.hsen_cmt, m_Data.m_tag );

	m_pHusenInp->SetHusenData( &crec, &srec );

}


//
//	親ウィンドウに メッセージ送付
//		long nChar	キーコード [ 0: フォーカスOFF ]
//
void CSubDataDlg::to_parent( long nChar )
{
	int sgn = 1;
	if( nChar )	sgn = 0;	// キー入力処理

	m_pParent->SendMessage( WM_SUBDATA_INEND, MAKELONG(sgn, nChar), (LPARAM)&m_Data );
}



BOOL CSubDataDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if( (m_hAccelTable != NULL) && ::TranslateAccelerator( m_hWnd, m_hAccelTable, pMsg))
	{
		return TRUE;
	}
	if( pMsg->message == WM_CHAR ) {
		if( ! m_MODEFY_OK )
			return TRUE;
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}

BEGIN_EVENTSINK_MAP(CSubDataDlg, ICSDialog)
	ON_EVENT(CSubDataDlg, IDC_ICSDBEDT_TGNO, 3, CSubDataDlg::SetFocusIcsdbedtTgno, VTS_NONE)
	ON_EVENT(CSubDataDlg, IDC_ICSDBEDT_TGNO, 2, CSubDataDlg::KillFocusIcsdbedtTgno, VTS_I4)
	ON_EVENT(CSubDataDlg, IDC_ICSDBEDT_TGNO, 1, CSubDataDlg::TerminationIcsdbedtTgno, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CSubDataDlg, IDC_ICSDBEDT_TGDATE, 3, CSubDataDlg::SetFocusIcsdbedtTgdate, VTS_NONE)
	ON_EVENT(CSubDataDlg, IDC_ICSDBEDT_TGDATE, 2, CSubDataDlg::KillFocusIcsdbedtTgdate, VTS_I4)
	ON_EVENT(CSubDataDlg, IDC_ICSDBEDT_TGDATE, 1, CSubDataDlg::TerminationIcsdbedtTgdate, VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

///////////////////////////////////////////////////////////////////////
// 手形番号

void CSubDataDlg::SetFocusIcsdbedtTgno()
{
	set_nowpn( SD_TGNO_PN );
}

void CSubDataDlg::KillFocusIcsdbedtTgno(long inplen)
{
	TgnoInp( 0,inplen,0 );
}

void CSubDataDlg::TerminationIcsdbedtTgno(long nChar, long inplen, long kst)
{
	nChar = TgnoInp( nChar, inplen, kst );

	switch( nChar ) {
	case VK_TAB:
	case VK_RETURN:
	case VK_RIGHT:
		set_focus(SD_TGDATE_PN);
		break;
	case VK_DOWN:
	case VK_UP:
	case VK_LEFT:
	case VK_F2:
		to_parent( nChar );
		break;
	}
}


// 証憑番号 入力＆オフフォーカス
long CSubDataDlg::TgnoInp( long nChar, long inplen, long kst ) 
{
	struct _ItemData check;
	char	buf[30] = {0};
	VARIANT var;

	// オフフォーカス
	if( !nChar )
	{
		//データリード
		if( m_MODEFY_OK ) {
			DBdata_get( this, IDC_ICSDBEDT_TGNO, &var, ICSDBEDT_TYPE_STRING, 0 );

			if( var.pbVal[0] != 0 )	strcpy_s( (char*)check.IP_SNUM, sizeof check.IP_SNUM, (char*)var.pbVal );
			else					::ZeroMemory( check.IP_SNUM, sizeof check.IP_SNUM );

			if( check.IP_SNUM[0] != 0 )
			{
				m_Data.m_rnumber = check.IP_SNUM;
			}
			else {
				m_Data.m_rnumber.Empty();
			}

			to_parent( 0 );
		}

		//データセット
		return( 0 );
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
		if( m_MODEFY_OK ) {
			m_Data.m_rnumber.Empty();

			var.pbVal = NULL;
			DBdata_set( this, IDC_ICSDBEDT_TGNO, &var, ICSDBEDT_TYPE_STRING, 0 );

			to_parent( 0 );
		}
	}

	return( nChar );
}



///////////////////////////////////////////////////////////////////////
// 手形期日

void CSubDataDlg::SetFocusIcsdbedtTgdate()
{
	set_nowpn( SD_TGDATE_PN );
}

void CSubDataDlg::KillFocusIcsdbedtTgdate(long inplen)
{
	if( inplen > 0 ) CheckDataIcsinputTgDate( 0 );
	// 入力＆オフフォーカス
	long nChar;

	if( !(nChar = TgdateInp( 0, inplen, 0)) )
		return;
}

void CSubDataDlg::TerminationIcsdbedtTgdate(long nChar, long inplen, long kst)
{

	if( inplen > 0 || nChar == 0x16 ) CheckDataIcsinputTgDate( nChar );
	// 入力＆オフフォーカス
	if( !(nChar = TgdateInp( nChar, inplen, kst)) )
		return;

	switch( nChar ) {
	case VK_TAB:
	case VK_RETURN:
	case VK_RIGHT:
		// 付箋に移動
		HusenDlgDisp();
		break;
	case VK_UP:
	case VK_DOWN:
		to_parent( nChar );
		break;
	case VK_LEFT:
	case VK_F2:
		set_focus( SD_TGNO_PN );
		break;
	}
}


/**************************
	手形期日チェックデータ
***************************/
long CSubDataDlg::CheckDataIcsinputTgDate( long nChar ) 
{
	char check[3] = {0};
	char ymd[3] = {0};
	char data[20] = {0};
	char result[30];
	char buf[40];
	VARIANT var;
	CString str;
	int ret;
	CVolDateDB vd;

	//
	try
	{
		DBdata_get( this, IDC_ICSDBEDT_TGDATE, &var, ICSDBEDT_TYPE_STRING, 0 );
		strcpy_s( data, sizeof data, (char*)var.pbVal );

		if( (ret = TgDateInputCheck( data, result, sizeof result, Voln1->ss_ymd, Voln1->ee_ymd, -1 )) <= 0 ) {
			// 不正データが入力された
			if( m_Data.m_tdate > 0 ) {
				int gendate, d_gengo;
				vd.db_datecnvGen( 0, m_Data.m_tdate, &d_gengo, &gendate, 0, 0 );

				sprintf_s( result, sizeof result, "%d", gendate );
				var.pbVal = (BYTE*)result;
			}
			else	var.pbVal = NULL;

			DBdata_set( this, IDC_ICSDBEDT_TGDATE, &var, ICSDBEDT_TYPE_STRING, 0 );
			IntsrekitoGenstr( buf, sizeof buf, m_Data.m_tdate );
			DBSetDispString( this, IDC_ICSDBEDT_TGDATE, buf );
			return -1;
		}
		else {
//			memcpy( check, bcd, 3 );
		}

		if( m_MODEFY_OK ) {
			var.pbVal = (BYTE*)result;
			DBdata_set( this, IDC_ICSDBEDT_TGDATE, &var, ICSDBEDT_TYPE_STRING, 0 );
		}
		else {
			Buzzer();
			var.pbVal = NULL;
			DBdata_set( this, IDC_ICSDBEDT_TGDATE, &var, ICSDBEDT_TYPE_STRING, 0 );
			
			IntsrekitoGenstr( str.GetBuffer(30), 30, m_Data.m_tdate );
			str.ReleaseBuffer( -1 );
			DBSetDispString( this, IDC_ICSDBEDT_TGDATE, str );

			return -1;
		}
	}
	catch ( CErrBlk E )
	{
		ErrExit( E.ErrCode, E.ErrMsg );
		return -1;
	}

	return nChar;
}

// 手形期日　入力＆オフフォーカス
long CSubDataDlg::TgdateInp( long nChar, long inplen, long kst ) 
{
	struct _ItemData check;
	char	buf[30] = {0};
	VARIANT var;
	CVolDateDB vd;

	// オフフォーカス
	if( !nChar )
	{
		//データリード
		DBdata_get( this, IDC_ICSDBEDT_TGDATE, &var, ICSDBEDT_TYPE_STRING, 0 );
		if( var.pbVal[0] != 0 ) {
			int gendate;
			gendate = atoi( (LPCTSTR)var.pbVal );
			sprintf_s( buf, sizeof buf, "%06d", gendate );
			BYTE bcddate[10] = {0};
			l_pack( bcddate, buf, 6 );
			check.IP_TGDAY = TgDateBcdToSeireki( bcddate, NULL );
		//	vd.db_datecnv( gendate, &check.IP_TGDAY, 1, 0 );
		}
		else	check.IP_TGDAY = -1;

		if( check.IP_TGDAY != -1 )
		{
			m_Data.m_tdate	= check.IP_TGDAY;
		}
		else {
			m_Data.m_tdate	= -1;
		}

		//データセット
		IntsrekitoGenstr( buf, sizeof buf, m_Data.m_tdate );
		DBSetDispString( this, IDC_ICSDBEDT_TGDATE, buf );
		return( 0 );
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
		check.IP_TGDAY = -1;
		var.pbVal = NULL;
		DBdata_set( this, IDC_ICSDBEDT_TGDATE, &var, ICSDBEDT_TYPE_STRING, 0 );

		m_Data.m_tdate =  check.IP_TGDAY;
	}

	return( nChar );
}


///////////////////////////////////////////////////////////////////////
// 付箋 キー処理

LRESULT CSubDataDlg::OnHsInpInEnd( WPARAM wParam, LPARAM lParam )
{
	WORD sgn, key;
	sgn = LOWORD( wParam );
	key = HIWORD( wParam );
	HSEN_DATA* pHdt;

	if( lParam ) {
		pHdt = (HSEN_DATA*)lParam;
		m_Data.m_dsign[7] &= (~0x0f);
		m_Data.m_dsign[7] |= pHdt->hsen_sel;
		m_Data.m_tag = pHdt->hsen_cmt;
	}

	switch( key ) {
	case VK_RETURN:
	case VK_TAB:
	case VK_RIGHT:
		to_parent( key );
		break;
	case VK_LEFT:
	case VK_F2:
		if( ! get_billsgn() )
			to_parent( key );
		else
			set_focus( SD_TGDATE_PN );
		break;
	}

	return 1;
}
