#include "stdafx.h"
#include "resource.h"

#include "HusenInp.h"
#include "HusenDlg.h"
#include "DBInpSubFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern HINSTANCE gInstance;


//////////////////////////////////////////////////////////////////////////////////
class CHusenInpSub
{
public:
	CHusenInpSub();
	~CHusenInpSub();

public:
	int		SetPara( CDBZmSub* pZm, CWnd* parent );
	void	SetHusenData( HSEN_DATA* cdata, HSEN_DATA* sdata );

	int		DialogON( BOOL bOK, CRect rect, int hspos, int pn );
	int		DialogOFF();
	BOOL	IsDisplay();
	void	GetCrntRect( CSize& size );

	void	set_nextpos();
	void	set_nowfocus();
	
public:
	CDBZmSub*	m_pZm;
	CWnd*		m_pParent;

	HSEN_DATA	m_cdata;
	HSEN_DATA	m_sdata;

	CHusenDlg*	m_pDialog;	
};



CHusenInpSub::CHusenInpSub()
{
	m_pZm = NULL;
	m_pDialog = NULL;

	::ZeroMemory( &m_cdata, sizeof m_cdata );
	::ZeroMemory( &m_sdata, sizeof m_sdata );
}

CHusenInpSub::~CHusenInpSub()
{
}


int CHusenInpSub::SetPara( CDBZmSub* pZm, CWnd* parent )
{
	m_pZm = pZm;
	m_pParent = parent;

	HINSTANCE h =  AfxGetResourceHandle();
	AfxSetResourceHandle( gInstance );

	if( m_pDialog != NULL ) {
		m_pDialog->DestroyWindow();
	}

	CZmGen8		ZmGen;
	int vver = ZmGen.CheckValiableVersion( pZm->m_database, ID_H26SYZ_VUP_CHK );
//	int vver = GetMasterVver( pZm );
	int type = 0;
	if( vver >= 1 )	type = 1;

	m_pDialog = new CHusenDlg( parent, type );
	if( m_pDialog == NULL )	return -1;

	AfxSetResourceHandle( h );

	m_pDialog->GetWindowRect( &m_pDialog->m_DlgRect );

//	m_pDialog->m_pZm = pZm;

	return 0;
}

void CHusenInpSub::SetHusenData( HSEN_DATA* cdata, HSEN_DATA* sdata )
{
	memcpy( &m_cdata, cdata, sizeof HSEN_DATA );
	memcpy( &m_sdata, sdata, sizeof HSEN_DATA );

	m_pDialog->HusenInp_Init( &m_cdata, &m_sdata );
}



int	CHusenInpSub::DialogON( BOOL bOK, CRect rect, int hspos, int pn )
{
	if( m_pDialog == NULL )	return	-1;
	
	int setpn = pn;

	RECT	newrect;
	double width, height;

	width = m_pDialog->m_DlgRect.Width();
	height = m_pDialog->m_DlgRect.Height();

	// 09.09 /10
	double grx, gry;	
	Dlg_GetRate( m_pDialog, grx, gry );

	if( grx != 0 && gry != 0 ) {
		width	= grx * width;
		height	= gry * height;
	}

	if( hspos == HSN_TOPLEFT ) {
		newrect.bottom = rect.top;
		newrect.top = newrect.bottom - height;
		newrect.right = rect.left;
		newrect.left = newrect.right - width;
	}
	else if( hspos == HSN_TOPRIGHT ) {
		newrect.bottom = rect.top;
		newrect.top = newrect.bottom - height;
		newrect.left = rect.left;
		newrect.right = newrect.left + width;
	}
	else if( hspos == HSN_BOTTOMLEFT ) {
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

	if( newrect.left < 0 ) {
		int tmp = abs(newrect.left);
		newrect.left = 0;
		newrect.right += tmp;
	}

	m_pDialog->EnableWindow( TRUE );
	WINDOWPLACEMENT	wp;
	m_pDialog->GetWindowPlacement( &wp );
	wp.showCmd	=	SW_SHOWNA;
	m_pDialog->SetWindowPlacement( &wp );
	m_pDialog->SetWindowPos( &CWnd::wndTop, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_SHOWWINDOW );
	m_pDialog->set_fastfocus( bOK, setpn );

//	WINDOWPLACEMENT	wp;
//	m_pDialog->GetWindowPlacement( &wp );
//	wp.showCmd	=	SW_SHOWNA;
//	m_pDialog->SetWindowPlacement( &wp );

//	m_pDialog->GetWindowRect( pDialog->CrntRect );
//	m_pParent->ScreenToClient( pDialog->CrntRect );

	m_pDialog->m_Dispflg	=	TRUE;

	return 0;
}


int CHusenInpSub::DialogOFF()
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


BOOL CHusenInpSub::IsDisplay()
{
	return m_pDialog->m_Dispflg;
}


void CHusenInpSub::GetCrntRect( CSize& size )
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
	Dlg_GetRate( m_pDialog, grx, gry );

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

/*
	newrect.top		= m_pDialog->m_DlgRect.top;
	newrect.bottom	= m_pDialog->m_DlgRect.bottom;
	newrect.right	= m_pDialog->m_DlgRect.right;
	newrect.left	= m_pDialog->m_DlgRect.left;

	size.cx = newrect.right - newrect.left;
	size.cy = newrect.bottom - newrect.top;
*/
}


void CHusenInpSub::set_nextpos()
{
	m_pDialog->set_nextpos();
}

void CHusenInpSub::set_nowfocus()
{
	m_pDialog->set_nowfocus();
}


////////////////////////////////////////////////////////////////////////////////////////////
// エクスポートクラス
CHusenInp::CHusenInp()
{
	pSub = NULL;
}


CHusenInp::~CHusenInp()
{
	if( pSub ) {
		delete pSub;
		pSub = NULL;
	}
}


int CHusenInp::SetPara( CDBZmSub* pZm, CWnd* parent )
{
	if( pSub == NULL ) {
		if( (pSub = new CHusenInpSub) == NULL )
			return -1;
	}

	return pSub->SetPara( pZm, parent );
}


void CHusenInp::SetHusenData( HSEN_DATA* cdata, HSEN_DATA* sdata )
{
	pSub->SetHusenData( cdata, sdata );
}


// DialogON を 呼び出す前に 該当仕訳で、CDBSyohiのSyIniz()を呼び出す必要あり
int	CHusenInp::DialogON( BOOL bOK, CRect rect, int hspos, int pn )
{
	return pSub->DialogON( bOK, rect, hspos, pn );
}


int CHusenInp::DialogOFF()
{
	return pSub->DialogOFF();
}

BOOL CHusenInp::IsDisplay()
{
	return pSub->IsDisplay();
}


void CHusenInp::GetCrntRect( CSize& size )
{
	pSub->GetCrntRect( size );
}


void CHusenInp::set_nextpos()
{
	pSub->set_nextpos();
}


void CHusenInp::set_nowfocus()
{
	pSub->set_nowfocus();
}

