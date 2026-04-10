// ACSelDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DBDinp.h"

#include "input1.h"
#include "external.h"

#include "ACSelDlg.h"

#include "subfnc.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CACSelDlg ダイアログ


#define BRNSEL_NONE		0
#define BRNSEL_TKYON	1



CACSelDlg::CACSelDlg(CWnd* pParent /*=NULL*/)
	: ICSDialog(CACSelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CACSelDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
	NowPN = -1;
	m_bDIALOG_OK = FALSE;
	m_bSEL_DISP = FALSE;
	m_pZsel = NULL;

	BRNTKY_sel = 0;
	m_bBM_MAST = m_bKJ_MAST = FALSE;
}


CACSelDlg::~CACSelDlg()
{
	if( m_pZsel ) {
		delete m_pZsel;
		m_pZsel = NULL;
	}
}

void CACSelDlg::DoDataExchange(CDataExchange* pDX)
{
	ICSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CACSelDlg)
	DDX_Control(pDX, IDC_ICSDBEDT_KMK, m_DBkmk);
	DDX_Control(pDX, IDC_ICSDBEDT_KOJI, m_DBkoji);
	DDX_Control(pDX, IDC_ICSDBEDT_DATE, m_DBdate);
	DDX_Control(pDX, IDC_ICSDBEDT_BMN, m_DBbmn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CACSelDlg, ICSDialog)
	//{{AFX_MSG_MAP(CACSelDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_RETROFOCUS, OnRetroFocus)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CACSelDlg メッセージ ハンドラ

BOOL CACSelDlg::OnInitDialog() 
{
	ICSDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください

	m_pZsel = new CZmselDB;

	if( m_pZsel == NULL ) {
		EndDialog(-1);
		return TRUE;
	}

ASSERT( m_pZm != NULL );
ASSERT( m_pSy != NULL );

	m_pZsel->SetPara( m_pZm, this, m_pSy, callback );

	// 建設マスター& 部門 チェック
	m_bBM_MAST = (m_pZm->zvol->sub_sw&0x02) ? TRUE : FALSE;
	if( ! m_bBM_MAST ) {
		m_DBbmn.EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC2)->EnableWindow(SW_HIDE);	
	}

	if( m_pZm->zvol->apno != 0x20 ) {
		m_bKJ_MAST = FALSE;
		m_DBkoji.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC3)->ShowWindow(SW_HIDE);	
	}
	else {
		m_bKJ_MAST = (m_pZm->zvol->sub_sw&0x04) ? TRUE : FALSE;
		if( ! m_bKJ_MAST ) {
			m_DBkoji.EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC3)->ShowWindow(SW_HIDE);	
		}
	}

	// 初期科目名セット
	m_DBkmk.ImeEndMode( TRUE );

	struct _KamokuData check = {0};
	struct _DBKNREC* pKn;
		
	strncpy_s( check.kd_code, sizeof check.kd_code, m_code, m_code.GetLength() );
	check.kd_eda = m_brn;
	pKn = m_pZm->DB_PjisToKnrec( m_code );
	if( pKn ) {
		memcpy( check.kd_name, pKn->knnam, sizeof pKn->knnam );
	}

	DBkamokudata_set( this, IDC_ICSDBEDT_KMK, &check, m_pZm );

	// 日付 セット
	char buf[40];

	int mm, dd;
	date_devide( m_date, &mm, &dd );

	m_pZm->minfo->MoveFirst();
	do {
		if( m_pZm->minfo->ofset == m_ofset ) {
			switch( m_pZm->minfo->kes_sgn ) {
			case 1:	sprintf_s( m_datestr, sizeof m_datestr, "61%02d", dd ); break;
			case 2:	sprintf_s( m_datestr, sizeof m_datestr, "62%02d", dd ); break;
			case 3:	sprintf_s( m_datestr, sizeof m_datestr, "63%02d", dd ); break;
			case 4:	sprintf_s( m_datestr, sizeof m_datestr, "00%02d", dd ); break;
			default:
				sprintf_s( m_datestr, sizeof m_datestr, "%02d%02d", mm, dd );
				break;
			}

			break;
		}
	} while( m_pZm->minfo->MoveNext() == 0 );

	VARIANT var;
	var.pbVal = (BYTE*)m_datestr;
	DBdata_set(this, IDC_ICSDBEDT_DATE, &var, ICSDBEDT_TYPE_STRING, 0 );

	if( m_ksign )	sprintf_s( buf, sizeof buf, "%02d/%02d", mm, dd );
	else			sprintf_s( buf, sizeof buf, "%02d.%02d", mm, dd );
	DBSetDispString( this, IDC_ICSDBEDT_DATE, buf );

	NowPN = AS_KAMOKU_PN;
	m_bDIALOG_OK = TRUE;

	ICSDialog::OnInitDialogEX();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}



void CALLBACK CACSelDlg::callback(UINT n,void* p,CWnd* pwnd )
{
//	pwnd
	CACSelDlg* pDlg;
	pDlg = (CACSelDlg*)pwnd;

	if( n == IDOK ) {
		ZmselDBData* zd = (ZmselDBData*)p;

TRACE( "*** callback IDOK mode = %d, errflg %d\n", zd->selmode, zd->errflg );

		if( pDlg ) {
			pDlg->ZmSelSet( zd );
		}
	}
	else {
	//	if( pDlg ) {
	//		pDlg->ZmSelBackFocus();
	//	}
		TRACE( "--- callback not IDOK \n" );
	}
}

BEGIN_EVENTSINK_MAP(CACSelDlg, ICSDialog)
    //{{AFX_EVENTSINK_MAP(CACSelDlg)
	ON_EVENT(CACSelDlg, IDC_ICSDBEDT_KMK, 3 /* SetFocus */, OnSetFocusIcsdbedtkmk, VTS_NONE)
	ON_EVENT(CACSelDlg, IDC_ICSDBEDT_KMK, 5 /* ImeEndComposition */, OnImeEndCompositionIcsdbedtkmk, VTS_I2 VTS_BSTR VTS_BSTR)
	ON_EVENT(CACSelDlg, IDC_ICSDBEDT_KMK, 4 /* KanaTermination */, OnKanaTerminationIcsdbedtkmk, VTS_BSTR)
	ON_EVENT(CACSelDlg, IDC_ICSDBEDT_KMK, 1 /* Termination */, OnTerminationIcsdbedtkmk, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CACSelDlg, IDC_ICSDBEDT_DATE, 3 /* SetFocus */, OnSetFocusIcsdbedtdate, VTS_NONE)
	ON_EVENT(CACSelDlg, IDC_ICSDBEDT_DATE, 2 /* KillFocus */, OnKillFocusIcsdbedtdate, VTS_I4)
	ON_EVENT(CACSelDlg, IDC_ICSDBEDT_DATE, 1 /* Termination */, OnTerminationIcsdbedtdate, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CACSelDlg, IDC_ICSDBEDT_KMK, 2 /* KillFocus */, OnKillFocusIcsdbedtkmk, VTS_I4)
	ON_EVENT(CACSelDlg, IDC_ICSDBEDT_BMN, 3 /* SetFocus */, OnSetFocusIcsdbedtBmn, VTS_NONE)
	ON_EVENT(CACSelDlg, IDC_ICSDBEDT_BMN, 1 /* Termination */, OnTerminationIcsdbedtBmn, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CACSelDlg, IDC_ICSDBEDT_BMN, 2 /* KillFocus */, OnKillFocusIcsdbedtBmn, VTS_I4)
	ON_EVENT(CACSelDlg, IDC_ICSDBEDT_KOJI, 3 /* SetFocus */, OnSetFocusIcsdbedtKoji, VTS_NONE)
	ON_EVENT(CACSelDlg, IDC_ICSDBEDT_KOJI, 2 /* KillFocus */, OnKillFocusIcsdbedtKoji, VTS_I4)
	ON_EVENT(CACSelDlg, IDC_ICSDBEDT_KOJI, 1 /* Termination */, OnTerminationIcsdbedtKoji, VTS_I4 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


void CACSelDlg::OnSetFocusIcsdbedtkmk() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( ! m_bDIALOG_OK )	return;
	
	if( m_bSEL_DISP ) {
		if( m_pZsel->GetCrntMode() != ZSEL_KAMOKU || ! m_pZsel->IsDisplay() ) {
			SelDialogON( ZSEL_KAMOKU );
		}
	}

	set_nowpn( AS_KAMOKU_PN );
}


void CACSelDlg::OnKillFocusIcsdbedtkmk(long inplen) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( ! m_bDIALOG_OK )	return;
	
}


void CACSelDlg::OnImeEndCompositionIcsdbedtkmk(short nChar, LPCTSTR string, LPCTSTR ystring) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	
	if( CheckDataKamoku( string ) <= 0 )
		return;
	TerminationKamoku( nChar, 1, 0 );
	
}

void CACSelDlg::OnKanaTerminationIcsdbedtkmk(LPCTSTR Kana) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	
}

void CACSelDlg::OnTerminationIcsdbedtkmk(long nChar, long inplen, long kst) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( ! m_bDIALOG_OK )	return;

	TerminationKamoku( nChar, inplen, kst );
}


// 科目チェックデータ
long CACSelDlg::CheckDataKamoku( LPCTSTR string ) 
{
	CString kmkstr;
	int edaban = -1;
	struct _KamokuData check = {0}, ans = {0};
	struct _DBKNREC* pKn;

	KamokuStr_Chk( string, &check );

	int sgn = KMCD_Disp ? 0 : 1;
	if( BRNTKY_sel != BRNSEL_NONE )
		sgn = 1;
	else if( KMCD_Disp ) {
		sgn = 0;
	}
	else {
		if( check.kd_sgn == 1 || check.kd_sgn == 5 )
				sgn = 1;
		else	sgn = 0;
	}

	ZmselDBData zsel;

	if( BRNTKY_sel != BRNSEL_NONE )
		m_pZsel->InputData( sgn, atoi( check.kd_code ), check.kd_eda, ZSEL_BRNTKY );
	else
		m_pZsel->InputData( sgn, atoi( check.kd_code ), check.kd_eda, ZSEL_KAMOKU );

	m_pZsel->ResultData( &zsel );

	int set_ok = 0;

	if( zsel.errflg != -1 ) {
		pKn = m_pZm->PjisToKnrec( zsel.dwkcode );
		if( pKn ) {
			strcpy_s( ans.kd_code, sizeof ans.kd_code, zsel.kcode );
			ans.kd_eda = zsel.edaban;
			memcpy( ans.kd_name, pKn->knnam, sizeof pKn->knnam );
			DBkamokudata_set( this, IDC_ICSDBEDT_KMK, &ans, m_pZm );
			set_ok = 1;
		}
	}

	if( !set_ok ) {
		MessageBeep( 0xffffffff );

		pKn = m_pZm->DB_PjisToKnrec( m_code );
		if( pKn ) {
			strcpy_s( ans.kd_code, sizeof ans.kd_code, m_code );
			ans.kd_eda = m_brn;
			memcpy( ans.kd_name, pKn->knnam, sizeof pKn->knnam );
		}
		else {
			ans.kd_eda = -1;
		}

		DBkamokudata_set( this, IDC_ICSDBEDT_KMK, &ans, m_pZm );
	}

	return 0;
}


void CACSelDlg::TerminationKamoku( long nChar, long inplen, long kst, ZmselDBData* zd/*= NULL*/ )
{
	VARIANT var;
	char	buf[64];
	var.pbVal = (BYTE*)buf;
	CString tmpcode;

	struct _KamokuData check = {0}, ans = {0};

	if( ! zd ) {
		if( nChar == VK_DELETE ) {
			check.kd_eda = -1;
			DBkamokudata_set( this, IDC_ICSDBEDT_KMK, &check, m_pZm );
		}
		else {
			DBkamokudata_get( this, IDC_ICSDBEDT_KMK, &check, m_pZm );
		}
	}
	else {
		memcpy( check.kd_code, zd->kcode, KMKCD_LN );
		check.kd_eda = zd->edaban;
		DBKNREC* pKn = m_pZm->DB_PjisToKnrec( check.kd_code );
		if( pKn ) {
			memcpy( check.kd_name, pKn->knnam, sizeof pKn->knnam );
		}
		DBkamokudata_set( this, IDC_ICSDBEDT_KMK, &check, m_pZm );
	}

	m_code = check.kd_code;
	m_brn = check.kd_eda;

	BOOL bShift = FALSE;
	if( kst & ICSDBEDT_KST_SHIFT ) {
		bShift	=	TRUE;
	}
	tmpcode = m_pZm->dbdata->dbt;
	m_pZm->dbdata->dbt = m_code;
	m_pZsel->SetRecord( m_pZm->dbdata );
	m_pZsel->SetBrnTkyDCsgn( 0 );
	m_pZm->dbdata->dbt = tmpcode;

	if( nChar == VK_RETURN ) {
		// ダミーの科目コードをセット
		if( m_pZsel->GetCrntMode() != ZSEL_BRNTKY ) {
			if( m_pZsel->DialogON( ZSEL_BRNTKY, ZSEL_BTMLEFT, NULL ) == 0 ) {
				nChar = 0;
				BRNTKY_sel = BRNSEL_TKYON;
			}
		}
	}
	else if( (nChar == VK_TAB && ! bShift) ) {
		if( m_pZsel->GetCrntMode() != ZSEL_BRNTKY ) {
			if( m_pZsel->DialogON( ZSEL_BRNTKY, ZSEL_BTMLEFT, NULL ) == 0 ) {
				nChar = 0;
				BRNTKY_sel = BRNSEL_TKYON;
			}
		}
	}

	if( nChar == VK_RETURN || (nChar == VK_TAB && ! bShift) || nChar == VK_F3 || nChar == VK_RIGHT || nChar == VK_DOWN ) {
		set_focus( AS_DATE_PN );	
	}
	else if( nChar == VK_LEFT ) {
		if( m_bKJ_MAST )		set_focus( AS_KOJI_PN );
		else if( m_bBM_MAST )	set_focus( AS_BMON_PN );
		else					set_focus( AS_DATE_PN );	
	}
	else if( nChar == VK_UP ) {
		if( m_bBM_MAST )		set_focus( AS_BMON_PN );
		else if( m_bKJ_MAST )	set_focus( AS_KOJI_PN );
		else					set_focus( AS_DATE_PN );
	}
	else if( nChar == VK_PRIOR ) {
		m_pZsel->PageUp();
	}
	else if( nChar == VK_NEXT ) {
		m_pZsel->PageDown();
	}
	else if( nChar == VK_INSERT ) {

	}
}



void CACSelDlg::OnSetFocusIcsdbedtdate() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( ! m_bDIALOG_OK ) return;

	if( m_pZsel->IsDisplay() ) {
		m_pZsel->DialogOFF();
	}

	set_nowpn( AS_DATE_PN );
}

void CACSelDlg::OnKillFocusIcsdbedtdate(long inplen) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( ! m_bDIALOG_OK )	return;
	
	if( inplen ) {
		if( CheckDataDate( 0 ) < 0 )
			return;
	}
	TerminationDate( 0, inplen, 0 ); 
}


void CACSelDlg::OnTerminationIcsdbedtdate(long nChar, long inplen, long kst) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( ! m_bDIALOG_OK )	return;

	if( inplen ) {
		if( CheckDataDate( nChar ) < 0 )
			return;
	}
	TerminationDate( nChar, inplen, kst ); 
}


int CACSelDlg::CheckDataDate( long nChar )
{
	char check[3] = {0};
	char ymd[3] = {0};
	char data[20] = {0};
	char buf[56];
	BYTE bcd[10] = {0};
	VARIANT var;
	CString str;
	int mm, dd;

	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	//
	DBdata_get( this, IDC_ICSDBEDT_DATE, &var, ICSDBEDT_TYPE_STRING, 0 );

	int len;

	if( var.pbVal != NULL )	{
		len = strlen( (char*)var.pbVal );
		if( len > 2 ) {
			strncpy_s( data, sizeof data, (char*)var.pbVal, 4 );
		}
		else {
			int sgn = 0;
			if( len == 2 ) {
				if( var.pbVal[0] == '6' || (var.pbVal[0] == '0' && var.pbVal[1] == '0') ) {
					strncpy_s( data, sizeof data, (char*)var.pbVal, len );
					sgn = 1;
				}
			}

			if( !sgn ) {
				strcpy_s( data, sizeof data, m_datestr );

				if( len == 1 ) {
					data[2] = '0';
					data[3] = var.pbVal[0];
				}
				else	strncpy_s( &data[2], sizeof(data)-2, (char*)var.pbVal, 2 );
			}
		}
	}
	else {
		strcpy_s( data, sizeof data, m_datestr );

		if( var.pbVal != NULL ) {
			len = strlen( (char*)var.pbVal );
			if( len == 1 ) {
				data[2] = '0';
				data[3] = var.pbVal[0];
			}
			else	strncpy_s( &data[2], sizeof(data) -2, (char*)var.pbVal, 2 );
		}
		else {
			data[2] = data[3] = '0';
		}
	}

	if( DateInputCheck( data, bcd ) != 0 ) {
		// 不正データが入力された
		var.pbVal = (BYTE*)m_datestr;
		DBdata_set( this, IDC_ICSDBEDT_DATE, &var, ICSDBEDT_TYPE_STRING, 0 );

		date_devide( m_date, &mm, &dd );
		if( m_ksign )	sprintf_s( buf, sizeof buf, "%02d/%02d", mm, dd );
		else			sprintf_s( buf, sizeof buf, "%02d.%02d", mm, dd );
		DBSetDispString( this, IDC_ICSDBEDT_DATE, buf );
		return -1;
	}
	else {
		memcpy( &check[1], bcd, 2 );
	}

/*	if( !check[1] )
	{
		check[0] = 0;
		check[1] = (char)0xff;
	}
*/

	VDPAC_BCD pac;
	BYTE tmp[3];
	memcpy( tmp, check, 3 );
	_db_vd_year((BYTE*)&tmp[1], (BYTE*)&tmp[0], m_pZm );
	check[0] = tmp[0];

	if( ! _db_vd_check( (BYTE*)&check[1], m_pZm ) && ! _db_vd_dpac( (BYTE*)check, &pac, m_pZm ) )
//	if( !m_pZm->Myvd_chek( (BYTE*)check ) && !m_pZm->Myvd_sign( (unsigned char*)check, (unsigned char*)ymd ) )
	{
		memmove( bcd, &pac.Dymd[0], 2 );

		sprintf_s( m_datestr, sizeof m_datestr, "%02x%02x", check[1], check[2] );
		m_ofset = pac.Doffset;
		if( (pac.Dymd[1] & 0x80) )	sprintf_s( buf, sizeof buf, "%02x/%02x", (pac.Dymd[1]&0x1f), pac.Dymd[2] );
		else						sprintf_s( buf, sizeof buf, "%02x.%02x", (pac.Dymd[1]&0x1f), pac.Dymd[2] );
		m_ksign = (pac.Dymd[1] & 0x80) ? 1 : 0;

		var.pbVal = (BYTE*)m_datestr;
		DBdata_set( this, IDC_ICSDBEDT_DATE, &var, ICSDBEDT_TYPE_STRING, 0 );
		DBSetDispString( this, IDC_ICSDBEDT_DATE, buf );

	}
	else
	{
		MessageBeep( 0xffffffff );
		date_devide( m_date, &mm, &dd );
		if( m_ksign )	sprintf_s( buf, sizeof buf, "%02d.%02d", mm, dd );
		else			sprintf_s( buf, sizeof buf, "%02d/%02d", mm, dd );
		DBSetDispString( this, IDC_ICSDBEDT_DATE, buf );

		return -1;
	}

	return 0;
}



void CACSelDlg::TerminationDate( long nChar, long inplen, long kst )
{

	BOOL bShift = FALSE;
	if( kst & ICSDBEDT_KST_SHIFT ) {
		bShift	=	TRUE;
	}

	if( nChar == VK_RETURN || (nChar == VK_TAB && ! bShift) || nChar == VK_F3 || nChar == VK_RIGHT ) {
		set_focus( AS_OK_PN );	
	}
	else if( nChar == VK_LEFT ) {
		set_focus( AS_KAMOKU_PN );	
	}
	else if( nChar == VK_UP ) {
		set_focus( AS_KAMOKU_PN );	
	}
	else if( nChar == VK_DOWN ) {
		if( m_bBM_MAST )	set_focus( AS_BMON_PN );	
		else				set_focus( AS_KAMOKU_PN );
	}
	else if( nChar == VK_DELETE ) {
//		var.pbVal = NULL;
//		DBdata_set( this, IDC_ICSDBEDT_BMN, &var, ICSDBEDT_TYPE_STRING, 0 );
	}
}


// 部門 セットフォーカス
void CACSelDlg::OnSetFocusIcsdbedtBmn() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( ! m_bDIALOG_OK ) return;

	if( m_bSEL_DISP ) {
		if( m_pZsel->GetCrntMode() != ZSEL_BUMON || ! m_pZsel->IsDisplay() ) {
			SelDialogON( ZSEL_BUMON );
		}
	}

	set_nowpn( AS_BMON_PN );
}

// 部門 キルフォーカス
void CACSelDlg::OnKillFocusIcsdbedtBmn(long inplen) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( ! m_bDIALOG_OK )	return;

	TerminationBumon( 0, inplen, 0 );
}

// 部門 ターミネーション
void CACSelDlg::OnTerminationIcsdbedtBmn(long nChar, long inplen, long kst) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( ! m_bDIALOG_OK )	return;

	TerminationBumon( nChar, inplen, kst );
}

// 部門 ターミネーション 実処理
void CACSelDlg::TerminationBumon( long nChar, long inplen, long kst, ZmselDBData* zd/*=NULL*/ )
{
	VARIANT var;
	char	buf[64];
	var.pbVal = (BYTE*)buf;
	int setbmn = -1;

	if( inplen && ! zd ) {
		DBdata_get( this, IDC_ICSDBEDT_BMN, &var, ICSDBEDT_TYPE_STRING, 0 );

		int len = strlen( (char*)var.pbVal );
		int bmn = atoi( (char*)var.pbVal );
		if( bmn < 100 && len < 3 ) {
			// 選択番号より
			ZmselDBData zsel;

			m_pZsel->InputData( 0, bmn, -1, ZSEL_BUMON );
			m_pZsel->ResultData( &zsel );

			if( zsel.errflg != -1 ) {
				m_bmn = zsel.bmn;
				setbmn = zsel.bmn;
			}
		}
		else {
			m_bmn = bmn;
			setbmn = bmn;
		}
		if( setbmn != -1 ) {			
			pDBzm->BumonCodeToStr( (char*)buf, sizeof buf, setbmn );
			var.pbVal = (BYTE*)buf;
			DBdata_set( this, IDC_ICSDBEDT_BMN, &var, ICSDBEDT_TYPE_STRING, 0 );
		}
	}
	else if( zd ) {
		m_bmn = zd->bmn;
		pDBzm->BumonCodeToStr( (char*)buf, sizeof buf, zd->bmn );
		var.pbVal = (BYTE*)buf;
		DBdata_set( this, IDC_ICSDBEDT_BMN, &var, ICSDBEDT_TYPE_STRING, 0 );
	}

	BOOL bShift = FALSE;
	if( kst & ICSDBEDT_KST_SHIFT ) {
		bShift	=	TRUE;
	}

	if( nChar == VK_RETURN || (nChar == VK_TAB && ! bShift) || nChar == VK_F3 || nChar == VK_RIGHT ) {
		if( m_bKJ_MAST )	set_focus( AS_KOJI_PN );	
		else				set_focus( AS_KAMOKU_PN );
	}
	else if( nChar == VK_LEFT ) {
		set_focus( AS_CANCEL_PN );	
	}
	else if( nChar == VK_UP ) {
		set_focus( AS_DATE_PN );	
	}
	else if( nChar == VK_DOWN ) {
		set_focus( AS_KAMOKU_PN );	
	}
	else if( nChar == VK_DELETE ) {
		var.pbVal = NULL;
		DBdata_set( this, IDC_ICSDBEDT_BMN, &var, ICSDBEDT_TYPE_STRING, 0 );
		m_bmn = -1;	
	}
}


void CACSelDlg::OnSetFocusIcsdbedtKoji() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( ! m_bDIALOG_OK ) return;
	
	if( m_bSEL_DISP ) {
		if( m_pZsel->GetCrntMode() != ZSEL_KOJI ) {
			SelDialogON( ZSEL_KOJI );
		}
	}

	set_nowpn( AS_KOJI_PN );
}

void CACSelDlg::OnKillFocusIcsdbedtKoji(long inplen) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( ! m_bDIALOG_OK ) return;

	TerminationKoji( 0, inplen, 0 );	
}

void CACSelDlg::OnTerminationIcsdbedtKoji(long nChar, long inplen, long kst) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( ! m_bDIALOG_OK ) return;

	TerminationKoji( nChar, inplen, kst );
}


// 工事番号 ターミネーション 実処理
void CACSelDlg::TerminationKoji( long nChar, long inplen, long kst, ZmselDBData* zd/*=NULL*/ )
{
	VARIANT var;
	char	buf[64];
	var.pbVal = (BYTE*)buf;
	CString setkoji;

	if( inplen && ! zd ) {
		DBdata_get( this, IDC_ICSDBEDT_KOJI, &var, ICSDBEDT_TYPE_STRING, 0 );

		int len = strlen( (char*)var.pbVal );
		if( len < 3 ) {
			int koji = atoi( (char*)var.pbVal );
			if( koji < 100 && len < 3 ) {
				// 選択番号より
				ZmselDBData zsel;

				m_pZsel->InputData( 0, koji, -1, ZSEL_BUMON );
				m_pZsel->ResultData( &zsel );

				if( zsel.errflg != -1 ) {
					m_koji = zsel.koji;
					setkoji = zsel.koji;
				}
			}
			else {
				m_koji = (char*)var.pbVal;
				setkoji = m_koji;
			}
		}
		else {
			m_koji = (char*)var.pbVal;
			setkoji = m_koji;
		}
		if( ! setkoji.IsEmpty() ) {			
			pDBzm->KojiCodeToStr( (char*)buf, sizeof buf, setkoji );
			var.pbVal = (BYTE*)buf;
			DBdata_set( this, IDC_ICSDBEDT_KOJI, &var, ICSDBEDT_TYPE_STRING, 0 );
		}
	}
	else if( zd ) {
		m_koji = zd->koji;
		pDBzm->KojiCodeToStr( (char*)buf, sizeof buf, zd->koji );
		var.pbVal = (BYTE*)buf;
		DBdata_set( this, IDC_ICSDBEDT_KOJI, &var, ICSDBEDT_TYPE_STRING, 0 );
	}

	BOOL bShift = FALSE;
	if( kst & ICSDBEDT_KST_SHIFT ) {
		bShift	=	TRUE;
	}

	if( nChar == VK_RETURN || (nChar == VK_TAB && ! bShift) || nChar == VK_F3 || nChar == VK_RIGHT ) {
		if( m_bKJ_MAST )	set_focus( AS_KAMOKU_PN );	
		else				set_focus( AS_KAMOKU_PN );
	}
	else if( nChar == VK_LEFT ) {
		if( m_bBM_MAST )	set_focus( AS_BMON_PN );	
		else				set_focus( AS_CANCEL_PN );
	}
	else if( nChar == VK_UP ) {
		set_focus( AS_DATE_PN );	
	}
	else if( nChar == VK_DOWN ) {
		set_focus( AS_KAMOKU_PN );	
	}
	else if( nChar == VK_DELETE ) {
		var.pbVal = NULL;
		DBdata_set( this, IDC_ICSDBEDT_KOJI, &var, ICSDBEDT_TYPE_STRING, 0 );
		m_koji.Empty();	
	}
}



// フォーカスセット
void CACSelDlg::set_focus( UINT id )
{
	int ctrl;

	switch( id ) {
	case AS_KAMOKU_PN:	ctrl = IDC_ICSDBEDT_KMK;	break;
	case AS_BMON_PN	:	ctrl = IDC_ICSDBEDT_BMN;	break;
	case AS_KOJI_PN	:	ctrl = IDC_ICSDBEDT_KOJI;	break;
	case AS_DATE_PN	:	ctrl = IDC_ICSDBEDT_DATE;	break;

	case AS_SEL_PN :	ctrl = IDC_BUTTON1;		break;
	case AS_OK_PN :		ctrl = IDOK;			break;
	case AS_CANCEL_PN :	ctrl = IDCANCEL;		break;
	default:
		ctrl = -1;
		break;
	}

	if( ctrl != -1 )	PostMessage( WM_RETROFOCUS, ctrl, 0 );

}


// フォーカスセット
LRESULT CACSelDlg::OnRetroFocus( WPARAM wParam, LPARAM lParam )
{

	GotoDlgCtrl( GetDlgItem( wParam ) );

	return 1;
}


// 選択部分よりの設定
void CACSelDlg::ZmSelSet( ZmselDBData* zd )
{
	int pn = get_nowpn();

	switch( pn ) {
	case AS_KAMOKU_PN:
		if( zd->selmode == ZSEL_KAMOKU || zd->selmode == ZSEL_KMKANA || zd->selmode == ZSEL_BRNTKY) {
			TerminationKamoku( 0, 0, 0, zd );
		}
		break;
	case AS_BMON_PN:
//		if( zd->selmode == ZSEL_BUMON ) {
//			TerminationBmon( get_nowln(), (pn == DATA_DEB_BMON) ? 0 : 1,  0, 0, 0, zd );
//		}
		break;

	case AS_KOJI_PN:


		break;
	}
}


// 選択画面ダイアログにフォーカスがある場合で ESCAPE などが押された
void CACSelDlg::ZmSelBackFocus()
{
	set_focus( get_nowpn() );
}

void CACSelDlg::set_nowpn( int pn )
{
	NowPN = pn;
}

int	CACSelDlg::get_nowpn()
{
	return NowPN;
}


BOOL CACSelDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください

	if( m_bDIALOG_OK ) {
		CWnd* pwnd;
		int id = -1;
		pwnd = GetFocus();
		if( pwnd ) {
			if(( id = pwnd->GetDlgCtrlID() ) ) {
				if( id == IDOK ) {
					set_nowpn(AS_OK_PN);
				}
				else if( id == IDCANCEL ) {
					set_nowpn(AS_CANCEL_PN);
				}
			}
		}

		if(id != -1 ) {
			if( pMsg->message == WM_KEYDOWN ) {
				if( PreTransKeyDown( id, pMsg->wParam ) ) {
					return TRUE;
				}
			}
		}
	}

	return ICSDialog::PreTranslateMessage(pMsg);
}


BOOL CACSelDlg::PreTransKeyDown(int id, UINT key) 
{
	BOOL bRet = FALSE;

	switch( id ) {
	case IDC_BUTTON1:
		if( key == VK_UP ) {
			set_focus( AS_CANCEL_PN );
			bRet = TRUE;
		}
		else if( key == VK_DOWN || key == VK_RIGHT ) {
			set_focus( AS_OK_PN );
			bRet = TRUE;
		}
		else if( key == VK_LEFT ) {
			set_focus( AS_DATE_PN );
			bRet = TRUE;
		}
		else if( key == VK_RETURN ) {

		}
		break;
	case IDOK:
		if( key == VK_UP ) {
			set_focus( AS_SEL_PN );
			bRet = TRUE;
		}
		else if( key == VK_DOWN || key == VK_RIGHT ) {
			set_focus( AS_CANCEL_PN );
			bRet = TRUE;
		}
		else if( key == VK_LEFT ) {
			set_focus( AS_DATE_PN );
			bRet = TRUE;
		}
		else if( key == VK_RETURN ) {

		}
		break;
	case IDCANCEL:
		if( key == VK_UP ) {
			set_focus( AS_OK_PN );
			bRet = TRUE;
		}
		else if( key == VK_DOWN || key == VK_RIGHT ) {
			set_focus( AS_SEL_PN );
			bRet = TRUE;
		}
		else if( key == VK_LEFT ) {
			set_focus( AS_DATE_PN );
			bRet = TRUE;
		}
		else if( key == VK_RETURN ) {

		}
		break;
	}

	return bRet;
}



void CACSelDlg::OnButton1() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_bSEL_DISP = ! m_bSEL_DISP;

	int pn, selmode;
	selmode = -1;
	pn = get_nowpn();
	switch( pn ) {
	case AS_KAMOKU_PN:
		selmode = ZSEL_KAMOKU;	break;
	case AS_BMON_PN:	
		selmode = ZSEL_BUMON;	break;
	case AS_KOJI_PN:
		selmode = ZSEL_KOJI;	break;	
	default:
		selmode = ZSEL_KAMOKU;
		pn = AS_KAMOKU_PN;
		break;
	}

	if( m_bSEL_DISP && selmode != -1 ) {
		SelDialogON( selmode );
		set_focus( pn );
	}
	else {
		m_pZsel->DialogOFF();
	}
}


// selmode = -1 ---- 以前の表示モードで 選択画面を表示
//
void CACSelDlg::SelDialogON( int selmode /*=-1*/) 
{

	CRect inprect, listrect, frmrect;
	GetClientRect( inprect );
	GetParentFrame()->GetWindowRect( frmrect );
	int pos;
	CSize sz(0,0);
	m_pZsel->GetCrntRect( sz );

	if( frmrect.top < 0 )	frmrect.OffsetRect( 0, abs( frmrect.top ) );
	if( frmrect.left < 0 )	frmrect.OffsetRect( abs( frmrect.left ), 0 );

	int nowmode = m_pZsel->GetCrntMode();

//	if( nowmode != -1 && nowmode != ZSEL_KAMOKU ) {
//		selmode = nowmode;
//	}

	ScreenToClient( frmrect );
//	ScreenToClient( inprect );

	listrect.SetRectEmpty();
	listrect.left = frmrect.left + 10;
	if( (inprect.top - sz.cy) < frmrect.top ) {
		listrect.top = inprect.bottom;
		pos = ZSEL_BTMLEFT;
	}
	else {
		listrect.top = inprect.top;
		int ty = GetSystemMetrics( SM_CYCAPTION );
		listrect.top -= ty;
		pos = ZSEL_TOPLEFT;
	}

	m_pZsel->DialogON( selmode, pos, &listrect );

}


