// 入力の 設定情報を扱うクラス
#include "stdafx.h"

#include "sql.h"
#include "sqlext.h"
#include "odbcinst.h"
#include <afxdb.h>
#include <afxdao.h>

#include "DinpRecordSet.h"
#include "SetUpTBL.h"



static
void data_source( int sign, const char * svr_path, unsigned char apno, int cod, int ymd, int nparam )
{
	int	ym, pm;
	char dsn[64], db[64], svr[64], attr[256], *tc;
	OSVERSIONINFO	vinfo;
	
	ym=ymd;
	ym = ym / 100;
	ym= ym % 10000;

	if( sign==0 ) pm = ODBC_ADD_DSN;
	else pm= ODBC_REMOVE_DSN;

	ZeroMemory( &vinfo, sizeof( vinfo ) );
    vinfo.dwOSVersionInfoSize = sizeof( vinfo );

	GetVersionEx( &vinfo );
	if( nparam || vinfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
		tc = "Trusted_Connection=No";
	else 
		tc = "Trusted_Connection=Yes";

	memset( attr, 0, sizeof(attr) );
	sprintf( dsn, "DSN=Z%02x_C%08d_Y%04d_Z", apno, cod, ym );
	sprintf( db, "DataBase=Z%02x_C%08d_Y%04d", apno, cod, ym );
	sprintf( svr, "Server=%s", svr_path );
	strcat( attr, dsn );
	strcat( &attr[strlen(dsn)+1], db );
	strcat( &attr[strlen(dsn)+strlen(db)+2], svr );
	strcat( &attr[strlen(dsn)+strlen(db)+strlen(svr)+3], tc );
	BOOL ret = ::SQLConfigDataSource( NULL, pm, "SQL Server", attr );
}

//////////////////////////////////////////////////////////////////////////////////////////////
// CMastSetUp
CMastSetUp::CMastSetUp()
{
	setup = NULL;
	m_database = new CDatabase;
}


CMastSetUp::~CMastSetUp()
{
	if( m_database ) {
		m_database->Close();
		delete m_database;
		m_database=NULL;
	}
}


// データベースオープン
int CMastSetUp::MSU_SetDB(char *server, unsigned char apno, int cod, int ymd, int nparam, char *id, char *pwd )
{
	char	dsn[256];
	int		ym;

//	zmfile->sServer = server; zmfile->svapno = apno; zmfile->svcod = cod; 
//	zmfile->svymd = ymd; zmfile->svnparam = nparam;
//	zmfile->sID = id; zmfile->sPWD = pwd;
	data_source( 0, server, apno, cod, ymd, nparam );
	if( m_database == NULL )
		return -1;

	ym=ymd;
	ym = ym / 100;
	ym= ym % 10000;

	if( nparam == 0 )
		sprintf( dsn, "Z%02x_C%08d_Y%04d_Z;", apno, cod, ym );
	else if( id && pwd )
		sprintf( dsn, "Z%02x_C%08d_Y%04d_Z;UID=%s;PWD=%s", apno, cod, ym, id, pwd );
	else if( id ) 
		sprintf( dsn, "Z%02x_C%08d_Y%04d_Z;UID=%s;", apno, cod, ym, id );
	else
		sprintf( dsn, "Z%02x_C%08d_Y%04d_Z;", apno, cod, ym );

	TRY
	{
		CString	st;
		if( ! m_database->IsOpen())
			m_database->Open(dsn);

		m_database->SetQueryTimeout(0);		// for debug
	}
	CATCH(CDBException, e)
	{
		AfxMessageBox(e->m_strError);
		return(-1);
	}
	END_CATCH

	return 0;
}


// データベースオープン
int CMastSetUp::MSU_SetDB(int valid_ver, char *server, unsigned char apno, int cod, int ymd, int nparam/*=0*/, char *id/*=0*/, char *pwd/*=0*/ )
{
	char	dsn[256];
	int		ym;

//	zmfile->sServer = server; zmfile->svapno = apno; zmfile->svcod = cod; 
//	zmfile->svymd = ymd; zmfile->svnparam = nparam;
//	zmfile->sID = id; zmfile->sPWD = pwd;
	data_source( 0, server, apno, cod, ymd, nparam );
	if( m_database == NULL )
		return -1;

	ym=ymd;
	ym = ym / 100;
	ym= ym % 10000;

	if( nparam == 0 )
		sprintf( dsn, "Z%02x_C%08d_Y%04d_Z;", apno, cod, ym );
	else if( id && pwd )
		sprintf( dsn, "Z%02x_C%08d_Y%04d_Z;UID=%s;PWD=%s", apno, cod, ym, id, pwd );
	else if( id ) 
		sprintf( dsn, "Z%02x_C%08d_Y%04d_Z;UID=%s;", apno, cod, ym, id );
	else
		sprintf( dsn, "Z%02x_C%08d_Y%04d_Z;", apno, cod, ym );

	TRY
	{
		CString	st;
		if( ! m_database->IsOpen())
			m_database->Open(dsn);

		m_database->SetQueryTimeout(0);		// for debug
	}
	CATCH(CDBException, e)
	{
		AfxMessageBox(e->m_strError);
		return(-1);
	}
	END_CATCH

	return 0;
}


int CMastSetUp::SetUpOpen()
{

	TRY {
		setup = new CSETUPTBL( m_database );
		setuptbl_make();
		setup->Open("");

		if( setup->st == -1 ) {
			setup->setupname = _T("setupver");
			setup->param = 0;
			setup->parastr = _T("");
			setup->Append();
		}
	}
	CATCH( CMemoryException, me ) {
		AfxMessageBox("Memory Alloc Error!");
		return -1;	
	}
	CATCH( CDBException, de ) {
		AfxMessageBox( de->m_strError );
		return -1;
	}
	END_CATCH

	return 0;
}

int CMastSetUp::SetUpClose()
{
	if( setup ) {
		setup->Close();
		delete setup;
		setup = NULL;
	}

	return 0;
}

// データベース作成
void CMastSetUp::setuptbl_make()
{
	int		i;
	CString	fname, dm;
	char	*pcn[1] = { "mastsetup" };
	
	for( i = 0; i < 1; i++ ) {
		if( ! object_check( 2, pcn[i] ) ) {
			dm.Format( "CREATE TABLE mastsetup ( [setupname] [varchar] (20) NOT NULL,\
[param] [int] NULL, [parastr] [varchar] (128) NULL ) ON [PRIMARY]" );

			m_database->ExecuteSQL( dm );
		}
	}
}



int CMastSetUp::object_check( int sign, char *objectname )
{
	CProc	*m_proc;
	CString	ms;
	int		st;

	m_proc = new CProc( m_database);
	if( sign == 0 ) 
		ms.Format( "select count(*) from dbo.sysobjects where id = object_id(N'[dbo].[%s]') and OBJECTPROPERTY(id, N'IsProcedure') = 1",
			objectname );
	else if( sign == 1 )
		ms.Format( "select count(*) from dbo.sysobjects where id = object_id(N'[dbo].[%s]') and OBJECTPROPERTY(id, N'IsView') = 1",
			objectname );
	else if( sign == 2 ) 
		ms.Format( "select count(*) from dbo.sysobjects where id = object_id(N'[dbo].[%s]') and OBJECTPROPERTY(id, N'IsUserTable') = 1",
			objectname );

	if( ! m_proc->Open(AFX_DB_USE_DEFAULT_TYPE,ms) ) {
		ErrorExit( "can't open database object table" );
		return(-1);
	}

	st = m_proc->m_st;
	delete m_proc;
	return(st);
}


////////////////////////////////////////////////////////////////////
// 設定
//
CSETUPTBL::CSETUPTBL( CDatabase* pDataBase )
{
	m_setup = new CSetuptbl( pDataBase );
	nodata	= 0;
}


int CSETUPTBL::Open( CString filter )
{
	m_setup->m_strFilter = filter;
	if( ! m_setup->Open() ) {
		ErrorExit( "can't open database mastsetup" );
		return(-1);
	}

	if( m_setup->IsBOF() ) nodata = 1;
	data();
	return(0);
}

void CSETUPTBL::Close()
{
	m_setup->Close();
	delete m_setup;
	m_setup=NULL;
}

int CSETUPTBL::Requery( CString filter )
{
//	m_setup->m_param=-1;
	m_setup->m_setupname.Empty();
	m_setup->m_strFilter = filter;
	m_setup->Requery();
	if( m_setup->IsBOF() ) nodata=1;
	else nodata=0;
	data();
	return(st);
}

#ifdef KEEP
int CSETUPTBL::Search( int sparam, char *sobj )
{
	int	i, rcnt;
	CString	ssetupname;

	ssetupname = sobj;
	rcnt = GetRecordCount();
	MoveFirst();
	for( i = 0; i < rcnt; i++ ) {
		if( sparam == param && ssetupname.Compare( setupname ) == 0 ) {
			return(parastr);
		}
		MoveNext();
	}
	return(-1);
}
#endif

int CSETUPTBL::MoveFirst()
{
//	m_setup->m_param = -1;
	m_setup->m_setupname.Empty();
	if( ! nodata ) m_setup->MoveFirst();
	data();
	return(st);
}

int CSETUPTBL::MoveNext()
{
//	m_setup->m_param = -1;
	m_setup->m_setupname.Empty();
	if( ! m_setup->IsEOF() ) m_setup->MoveNext();
	data();
	return(st);
}

int CSETUPTBL::MovePrev()
{
//	m_setup->m_param = -1;
	m_setup->m_setupname.Empty();
	if( ! m_setup->IsBOF() ) m_setup->MovePrev();
	data();
	return(st);
}

int CSETUPTBL::MoveLast()
{
//	m_setup->m_param = -1;
	m_setup->m_setupname.Empty();
	if( ! nodata ) m_setup->MoveLast();
	data();
	return(st);
}

int CSETUPTBL::Move( int nrows )
{
//	m_setup->m_param = -1;
	m_setup->m_setupname.Empty();
	if( nrows > 0 ) {
		if( ! m_setup->IsEOF() ) m_setup->Move(nrows);
	}
	else if( nrows < 0 ) {
		if( ! m_setup->IsBOF() ) m_setup->Move(nrows);
	}
	else m_setup->Move(nrows);
	data();
	return(st);
}

int CSETUPTBL::SetAbsolutePosition( int nrows )
{
//	m_setup->m_param = -1;
	m_setup->m_setupname.Empty();
	if( ! nodata ) 	m_setup->SetAbsolutePosition(nrows);
	data();
	return(st);
}

void CSETUPTBL::Append()
{
	m_setup->AddNew();
	m_setup->m_setupname = setupname;
	m_setup->m_param = param;
	m_setup->m_parastr = parastr;
	m_setup->Update();
}

void CSETUPTBL::Correct()
{
	m_setup->Edit();
	m_setup->m_setupname = setupname;
	m_setup->m_param = param;
	m_setup->m_parastr = parastr;
	m_setup->Update();
}

int CSETUPTBL::GetRecordCount()
{
	CRecordsetStatus	rstat;
	long	cpos;

	m_setup->GetStatus( rstat );
	if( rstat.m_bRecordCountFinal ) return( m_setup->GetRecordCount() );
	else {
		if( (cpos=rstat.m_lCurrentRecord) < 0 ) m_setup->MoveFirst();
		for( ; ! m_setup->IsEOF(); )
			m_setup->MoveNext();
		if( cpos >= 0 ) m_setup->SetAbsolutePosition(cpos+1);
		return( m_setup->GetRecordCount() );
	}
}

void CSETUPTBL::data()
{
	setupname = m_setup->m_setupname;

	if( setupname.IsEmpty() ) {
		st = -1;
		setupname=_T("");
		param = -1;
		parastr=_T("");
		return;
	}
	st = 0;
	param	= m_setup->m_param;
	parastr = m_setup->m_parastr;
}


