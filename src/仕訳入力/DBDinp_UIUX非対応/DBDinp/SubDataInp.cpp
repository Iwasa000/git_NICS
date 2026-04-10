//////////////////////////////////////////////////////////////////////////////
// サブ 入力画面

#include "stdafx.h"
#include "DBDinp.h"

#include "SubDataDlg.h"

#include "MstRw.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



// ダイアログの伸縮率を取得
void Dlg_GetRate( ICSDialog* pDlg, double& grx, double& gry )
{
	if( IsNotMultiWindow() ) {
		grx = gry = 0;
	}
	else {
		pDlg->GetRate( grx, gry);
	}
}


class CSubDataSub
{
public:
	CSubDataSub();
	~CSubDataSub();

public:
	int		SetPara( CDBZmSub* pZm, CWnd* parent );

	int		DialogON( CRect rect, int hspos, int pn );
	int		DialogOFF();
	BOOL	IsDisplay();
	void	GetCrntRect( CSize& size );
	int		SetCurData( BOOL bMdfyOK, CDBINPDataRec* rec );

	void	set_focus();
	void	F2Job();

public:
	CDBZmSub*	m_pZm;
	CWnd*		m_pParent;

	CSubDataDlg*	m_pDialog;

};


CSubDataSub::CSubDataSub()
{
	m_pZm = NULL;
	m_pDialog = NULL;
}


CSubDataSub::~CSubDataSub()
{
}


int CSubDataSub::SetPara( CDBZmSub* pZm, CWnd* parent )
{
	m_pZm = pZm;
	m_pParent = parent;

//	HINSTANCE h =  AfxGetResourceHandle();
//	AfxSetResourceHandle( gInstance );

	if( m_pDialog != NULL ) {
		m_pDialog->DestroyWindow();
	}

	int type = 0;
	if( IsMasterType(MST_INSERT) ) {
		type = 1;
	}

	m_pDialog = new CSubDataDlg( parent, type );
	if( m_pDialog == NULL )	return -1;

//	AfxSetResourceHandle( h );

	m_pDialog->GetWindowRect( &m_pDialog->m_DlgRect );

	return 0;
}


int	CSubDataSub::DialogON( CRect rect, int hspos, int pn )
{
	if( m_pDialog == NULL )	return	-1;
	
	RECT	newrect;
	double width, height;

	width = m_pDialog->m_DlgRect.Width();
	height = m_pDialog->m_DlgRect.Height();

	double grx, gry;	
	Dlg_GetRate( m_pDialog, grx, gry);

	if( grx != 0 && gry != 0 ) {
		width	= grx * width;
		height	= gry * height;
	}

	if( hspos == SDT_TOPLEFT ) {
		newrect.bottom = rect.top;
		newrect.top = newrect.bottom - height;
		newrect.right = rect.left;
		newrect.left = newrect.right - width;
	}
	else if( hspos == SDT_TOPRIGHT ) {
		newrect.bottom = rect.top;
		newrect.top = newrect.bottom - height;
		newrect.left = rect.left;
		newrect.right = newrect.left + width;
	}
	else if( hspos == SDT_BOTTOMLEFT ) {
		newrect.top = rect.bottom;
		newrect.bottom = newrect.top + height;
		newrect.right = rect.left;
		newrect.left = newrect.right - width;
	}
	else {
		newrect.top = rect.bottom;
		newrect.bottom = newrect.top + height;
		newrect.left = rect.left;
		newrect.right = rect.left + width;
	}

	m_pDialog->EnableWindow( TRUE );
	WINDOWPLACEMENT	wp;
	m_pDialog->GetWindowPlacement( &wp );
	wp.showCmd	=	SW_SHOWNA;
	m_pDialog->SetWindowPlacement( &wp );
	m_pDialog->SetWindowPos( &CWnd::wndTop, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_SHOWWINDOW );

	m_pDialog->HusenDlgDisp();
	m_pDialog->set_fastfocus( pn );

//	WINDOWPLACEMENT	wp;
//	m_pDialog->GetWindowPlacement( &wp );
//	wp.showCmd	=	SW_SHOWNA;
//	m_pDialog->SetWindowPlacement( &wp );

//	m_pDialog->GetWindowRect( pDialog->CrntRect );
//	m_pParent->ScreenToClient( pDialog->CrntRect );

	m_pDialog->m_Dispflg	=	TRUE;

	return 0;
}


int CSubDataSub::DialogOFF()
{
	if( m_pDialog == NULL )	return	-1;
//	m_pDialog->GetWindowRect( m_pDialog->DlgRect );
//	pParent->ScreenToClient( m_pDialog->DlgRect );
	m_pDialog->ShowWindow( SW_HIDE );
	m_pDialog->MoveWindow(0,0,0,0);
	m_pDialog->EnableWindow( FALSE );

	m_pDialog->m_Dispflg	=	FALSE;

	return 0;
}


BOOL CSubDataSub::IsDisplay()
{
	return m_pDialog->m_Dispflg;
}


void CSubDataSub::GetCrntRect( CSize& size )
{
	if( m_pDialog == NULL ) {
		size.cx = 0; size.cy = 0;
		return;
	}

	RECT	newrect;
	double width, height;

	width = m_pDialog->m_DlgRect.Width();
	height = m_pDialog->m_DlgRect.Height();

	// 09.09 /10
	double grx, gry;	
	m_pDialog->GetRate( grx, gry);

	if( grx != 0 && gry != 0 ) {
		width	= grx * width;
		height	= gry * height;
	}

	newrect.top		= m_pDialog->m_DlgRect.top;
	newrect.left	= m_pDialog->m_DlgRect.left;
	newrect.bottom	= newrect.top + height;
	newrect.right	= newrect.left + width;

	size.cx = newrect.right - newrect.left;
	size.cy = newrect.bottom - newrect.top;

/*	newrect.bottom = m_pDialog->m_DlgRect.top;
	newrect.top = newrect.bottom - ( height * y );
	newrect.right = m_pDialog->m_DlgRect.left + (x * 10);
	newrect.left = newrect.right - ( width * x ) + (y * 10);

	size.cx = newrect.right - newrect.left;
	size.cy = newrect.bottom - newrect.top;
*/
//	size.cx = m_pDialog->m_DlgRect.right - m_pDialog->m_DlgRect.left;
//	size.cy = m_pDialog->m_DlgRect.bottom - m_pDialog->m_DlgRect.top;

}


void CSubDataSub::set_focus()
{
	m_pDialog->set_focus( m_pDialog->get_nowpn() );
}


void	CSubDataSub::F2Job()
{
	int pn = m_pDialog->get_nowpn();

	BOOL bBill = m_pDialog->get_billsgn();

	// 手形項目でない
	if( ! bBill ) {
		m_pDialog->to_parent( VK_F2 );
		return;
	}

	switch( pn ) {
	case SD_TGNO_PN:
		m_pDialog->to_parent( VK_F2 );
		break;
	case SD_TGDATE_PN:
		m_pDialog->set_focus( SD_TGNO_PN );
		break;
	case SD_HUSEN_PN:
		m_pDialog->set_focus( SD_TGDATE_PN );
		break;
	}
}


int CSubDataSub::SetCurData( BOOL bMdfyOK, CDBINPDataRec* prec )
{
	m_pDialog->set_cdata( bMdfyOK, prec );

	BOOL bRet = FALSE;
	int rval;

	if( ! prec->m_dbt.IsEmpty() ) {
		bRet = IsBillCodeEx( prec->m_dbt, m_pZm );
	}
	if( ! bRet ) {
		if( ! prec->m_cre.IsEmpty() ) {
			bRet = IsBillCodeEx( prec->m_cre, m_pZm );
		}
	}

	m_pDialog->set_billsgn( bRet );

	if( bRet )	rval = SD_TGNO_PN;
	else		rval = SD_HUSEN_PN;

	return rval;
}

////////////////////////////////////////////////////////////////////////////////////
// 実クラス

CSubDataInp::CSubDataInp()
{
	pSub = NULL;
}


CSubDataInp::~CSubDataInp()
{
	if( pSub ) {
		delete pSub;
		pSub = NULL;
	}
}


int CSubDataInp::SetPara( CDBZmSub* pZm, CWnd* parent )
{
	if( pSub == NULL ) {
		if( (pSub = new CSubDataSub) == NULL )
			return -1;
	}

	return pSub->SetPara( pZm, parent );
}


int	CSubDataInp::DialogON( CRect rect, int hspos, int pn )
{
	return pSub->DialogON( rect, hspos, pn );
}


int CSubDataInp::DialogOFF()
{
	return pSub->DialogOFF();
}

BOOL CSubDataInp::IsDisplay()
{
	return pSub->IsDisplay();
}


void CSubDataInp::GetCrntRect( CSize& size )
{
	pSub->GetCrntRect( size );
}


void CSubDataInp::set_focus()
{
	pSub->set_focus();
}

// F2 前項目に 戻る
void CSubDataInp::F2Job()
{
	pSub->F2Job();
}

//---------------------------------------------
//	DialogON()を呼び出す前に必ず よぶこと
//		BOOL bMdfyOK		修正可能か？
//		CDBINPDataRec* rec	仕訳レコード
//
//---------------------------------------------
int CSubDataInp::SetCurData( BOOL bMdfyOK, CDBINPDataRec* rec )
{
	return pSub->SetCurData( bMdfyOK, rec );
}



