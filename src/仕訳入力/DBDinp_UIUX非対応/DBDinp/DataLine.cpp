#include "stdafx.h"
#include "DBDinp.h"

#include "DataListCtrl.h"
#include "DataLine.h"

#include "DBDinpDoc.h"
#include "DBDinpView.h"

#include "DinpSub.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////

#ifdef _SLIP_ACCOUNTS_
int _IS_SLIP_MASTER = 0;	// 原票保存マスター？
#endif

struct _DATA_LINE	INP_LINE;
// data display
struct _DATA_LINE DDATA_LINE[ DDATA_LNMAX ];

static short DDATA_LN = -1;		// 現在処理行
static short KSGEN_LN = -1;		// 現在表示行(下段ダイアログの 修正時のみ) 
static short DINP_CTRL = -1;
static short DPRV_CTRL = -1;


extern CDBDinpView *p_DBDinpView;


// コントロールテーブル

extern struct _InpCtl DLIST_INPCTRL[]; // DBDinpView.cpp



/////////////////////////////////////////////////

int	get_nowln()
{
	return DDATA_LN;
}

void _set_nowln( int ln )
{
	DDATA_LN = ln;
}

// get line data adr.
struct _DATA_LINE *getLINE_DATA( void )
{
	return _getLINE_DATA( get_nowln() );
}


struct _DATA_LINE *_getLINE_DATA( int ln )
{
	if( ln == APPEND_LINE )
	{
		return( &INP_LINE );
	}
	else
	{
		return( &DDATA_LINE[ ln - 1 ] );
	}
}


int getLINE_BumonData( int dc_sw )
{
//	int bmn = -1;

	if( !dc_sw )
		return _getLINE_DATA( get_nowln() )->_XXDTA.dd_dbmn;
	else
		return _getLINE_DATA( get_nowln() )->_XXDTA.dd_cbmn;

//	return bmn;
}


CString getLINE_KojiData( int dc_sw )
{
	if( !dc_sw )
		return _getLINE_DATA( get_nowln() )->_XXDTA.dd_dkno;
	else
		return _getLINE_DATA( get_nowln() )->_XXDTA.dd_ckno;
}


struct _InpCtl *_getINP_CTL( int ln, int pn )
{
	struct _InpCtl* pCtl;
	if( ln == APPEND_LINE )
		pCtl = &DLIST_INPCTRL[0];
	else
		pCtl = &DLIST_INPCTRL[1];
		
	pCtl->INP_ena = p_DBDinpView->GetDtListEnable( ln, pn );
	return pCtl;
	return 0;
}


struct _InpCtl *getINP_CTL( int pn )
{	
	return _getINP_CTL( get_nowln(), pn );
}


struct _InpCtl* getINP_CTL( CWnd* pwnd, int pn )
{
ASSERT( pwnd != NULL );
	struct _InpCtl* pCtl;
	int ln = get_nowln();

	if( ln == APPEND_LINE )
		pCtl = &DLIST_INPCTRL[0];
	else
		pCtl = &DLIST_INPCTRL[1];
	
	pCtl->INP_ena = p_DBDinpView->GetDtListEnable( ln, pn );
	return pCtl;
}

struct _InpCtl* _getINP_CTL( CWnd* pwnd, int ln, int pn )
{
	struct _InpCtl* pCtl;
	if( ln == APPEND_LINE )
		pCtl = &DLIST_INPCTRL[0];
	else
		pCtl = &DLIST_INPCTRL[1];

	pCtl->INP_ena = p_DBDinpView->GetDtListEnable( ln, pn );
	return pCtl;

}


// デバッグ用
void DbgDataLine()
{
	for( int n = 0; n < DDATA_LNMAX; n++ ) {
		MyTrace( "*Dbg* DLINE[%d], seq = %d, adr userdata = %08x\n", n,  DDATA_LINE[ n ]._XXDTA.dd_seq,
			DDATA_LINE[ n ]._XXDTA.dd_udtbl[0].userdata );
	}
}

