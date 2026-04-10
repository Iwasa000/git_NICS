/*******************************************************

		File Name	:	dbzmgdtsub.cpp
		Auth.		:	麓
		Date		:	99年2月1日

Date		Comment
---------	--------------------------------------------
02.01 /99	Begin.
********************************************************/
#include "stdafx.h"
#include "io.h"
#include "BrnTkyDef.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/***********************************
	Big <-> Little Endian for WORD
************************************/
DBSUB_FUNC
unsigned short WINAPI hl_lev( short wd )
{
	return ((wd<<8)&0xff00) | ((wd>>8)&0x00ff);
}

/*************************
	itoHEX( long )
**************************/
DBSUB_FUNC
long WINAPI itoHEX( unsigned long integer )
{
char tmp[20];
long longint;

	sprintf_s( tmp, sizeof tmp, "%d", integer );
	sscanf_s( tmp, "%x", &longint );

	return longint;
}

/*************************
	HEXtoi( long )
**************************/
DBSUB_FUNC
long WINAPI HEXtoi( unsigned long integer )
{
char tmp[20];

	sprintf_s( tmp, sizeof tmp, "%x", integer );
	return atol( tmp );
}

/*************************
	諸口該当コード？
**************************/
DBSUB_FUNC
BOOL WINAPI isSYOGT_CODE( LPCTSTR code, int nFlag /*=-1*/ )
{	
	int len;
	len = strlen( SYOGT );

	return ! strncmp( code, SYOGT, len );
}

/*************************
	現金該当コード？
**************************/
DBSUB_FUNC
BOOL WINAPI isGNKIN_CODE( LPCTSTR code, int nFlag /*=-1*/  )
{
//	int len;
//	len = strlen( GNKIN );
//	return ! strncmp( code, GNKIN, len );

	LPCTSTR pCd;

	if( nFlag != -1 ) 
		pCd = GNKINCode( nFlag );
	else
		pCd = GNKINCode( 0 );

	int len;
	len = strlen( pCd );

	return ! strncmp( code, pCd, len );
}


/**********************************
		ffind()
	ファイル検索
		-> "file name"
			NULL ... Not Found
***********************************/
DBSUB_FUNC
LPCSTR WINAPI ffind( LPCSTR path, UINT sw )
{
	static CString ret;
	long hfnd;
	struct _finddata_t fndinfo;

	if( (hfnd = _findfirst( path, &fndinfo )) == -1 )
		return NULL;
	_findclose( hfnd );

	if( !sw ? ! (fndinfo.attrib & (unsigned)_A_SUBDIR) : (fndinfo.attrib & (unsigned)_A_SUBDIR) )
	{
		ret = fndinfo.name;
		return ret;
	}
	return NULL;
}

