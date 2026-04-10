#include "stdafx.h"
#include "resource.h"

#include "HjiyuInp.h"
#include "HjiyuDlg.h"
#include "DBInpSubFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern HINSTANCE gInstance;


//////////////////////////////////////////////////////////////////////////////////
class CHjiyuInpSub
{
public:
	CHjiyuInpSub();
	~CHjiyuInpSub();

public:
	int		SetPara( CDBZmSub* pZm, CWnd* parent );

	int		DialogON( BOOL bOK, CRect rect, int hspos );
	int		DialogOFF();
	BOOL	IsDisplay();
	void	GetCrntRect( CSize& size );
	int		SetHjiyuData( HJIYU_DATA* data, BOOL bUpdate );

	void	set_focus();
	void	set_mode( int mode );

public:
	CDBZmSub*	m_pZm;
	CWnd*		m_pParent;

	CHjiyuDlg*	m_pDialog;	
};



CHjiyuInpSub::CHjiyuInpSub()
{
	m_pZm = NULL;
	m_pDialog = NULL;
}

CHjiyuInpSub::~CHjiyuInpSub()
{
}


int CHjiyuInpSub::SetPara( CDBZmSub* pZm, CWnd* parent )
{
	m_pZm = pZm;
	m_pParent = parent;

	HINSTANCE h =  AfxGetResourceHandle();
	AfxSetResourceHandle( gInstance );

	m_pDialog = new CHjiyuDlg( parent );
	if( m_pDialog == NULL )	return -1;

	AfxSetResourceHandle( h );

	m_pDialog->GetWindowRect( &m_pDialog->m_DlgRect );

//	m_pDialog->m_pZm = pZm;

	return 0;
}


int	CHjiyuInpSub::DialogON( BOOL bOK, CRect rect, int hspos )
{
	if( m_pDialog == NULL )	return	-1;
	
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

	if( hspos == HJY_TOPLEFT ) {
		newrect.bottom = rect.top;
		newrect.top = newrect.bottom - height;
		newrect.right = rect.left;
		newrect.left = newrect.right - width;
	}
	else if( hspos == HJY_TOPRIGHT ) {
		newrect.bottom = rect.top;
		newrect.top = newrect.bottom - height;
		newrect.left = rect.left;
		newrect.right = newrect.left + width;
	}
	else if( hspos == HJY_BOTTOMLEFT ) {
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
	m_pDialog->set_fastfocus( bOK );

//	WINDOWPLACEMENT	wp;
//	m_pDialog->GetWindowPlacement( &wp );
//	wp.showCmd	=	SW_SHOWNA;
//	m_pDialog->SetWindowPlacement( &wp );

//	m_pDialog->GetWindowRect( pDialog->CrntRect );
//	m_pParent->ScreenToClient( pDialog->CrntRect );

	m_pDialog->m_Dispflg	=	TRUE;

	return 0;
}


int CHjiyuInpSub::DialogOFF()
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


BOOL CHjiyuInpSub::IsDisplay()
{
	return m_pDialog->m_Dispflg;
}


void CHjiyuInpSub::GetCrntRect( CSize& size )
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

/*	newrect.bottom = m_pDialog->m_DlgRect.top;
	newrect.top = newrect.bottom - ( height * y );
	newrect.right = m_pDialog->m_DlgRect.left + (x * 10);
	newrect.left = newrect.right - ( width * x ) + (y * 10);

	size.cx = newrect.right - newrect.left;
	size.cy = newrect.bottom - newrect.top;
	size.cx = m_pDialog->m_DlgRect.right - m_pDialog->m_DlgRect.left;
	size.cy = m_pDialog->m_DlgRect.bottom - m_pDialog->m_DlgRect.top;
*/
}

void CHjiyuInpSub::set_focus()
{
	m_pDialog->set_focus();
}

int	CHjiyuInpSub::SetHjiyuData( HJIYU_DATA* data, BOOL bUpdate )
{
	if( ! m_pDialog )	return -1;
	
	return m_pDialog->SetHjiyuData( data, bUpdate );
}


void CHjiyuInpSub::set_mode( int mode )
{
	if( ! m_pDialog )	return;

	m_pDialog->set_mode( mode );
}

////////////////////////////////////////////////////////////////////////////////////////////
// エクスポートクラス
CHjiyuInp::CHjiyuInp()
{
	pSub = NULL;
}


CHjiyuInp::~CHjiyuInp()
{
	if( pSub ) {
		delete pSub;
		pSub = NULL;
	}
}


int CHjiyuInp::SetPara( CDBZmSub* pZm, CWnd* parent )
{
	if( pSub == NULL ) {
		if( (pSub = new CHjiyuInpSub) == NULL )
			return -1;
	}

	return pSub->SetPara( pZm, parent );
}


int	CHjiyuInp::DialogON( BOOL bOK, CRect rect, int hspos )
{
	return pSub->DialogON( bOK, rect, hspos );
}


int CHjiyuInp::DialogOFF()
{
	return pSub->DialogOFF();
}

BOOL CHjiyuInp::IsDisplay()
{
	return pSub->IsDisplay();
}


void CHjiyuInp::GetCrntRect( CSize& size )
{
	pSub->GetCrntRect( size );
}


void CHjiyuInp::set_focus()
{
	pSub->set_focus();
}


int CHjiyuInp::SetHjiyuData( HJIYU_DATA* data, BOOL bUpdate )
{
	return pSub->SetHjiyuData( data, bUpdate );
}


void CHjiyuInp::set_mode( int mode )
{
	pSub->set_mode( mode );
}