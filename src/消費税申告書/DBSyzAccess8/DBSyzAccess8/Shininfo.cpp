// 
#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void AFXAPI RFX_Bigint(CFieldExchange* pFX, LPCTSTR szName, __int64& value);

/////////////////////////////////////////////////////////////////////////////
// CShininfo

IMPLEMENT_DYNAMIC(CShininfo, CRecordset)

CShininfo::CShininfo( CDatabase* pdb )
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CShininfo)

	pr.SIitem = 0;
	pr.SIsequ = 0;
	pr.SIsnum = 0;
	pr.SIskey = 0;
	pr.SIskbn = 0;
	pr.SIsign = 0;
	pr.SIksdy = 0;
	pr.SIkedy = 0;
	pr.SImsdy = 0;
	pr.SImedy = 0;
	pr.SIcsdy = 0;
	pr.SIcedy = 0;
	pr.SIpday = 0;
	pr.SIkday = 0;
	pr.SIuser = _T("");
	pr.SIday1 = 0;
	pr.SIusr1 = _T("");
	pr.SIday2 = 0;
	pr.SIusr2 = _T("");
	pr.SImflg.SetSize(128);

	m_nFields = 20;
	//}}AFX_FIELD_INIT
}

void CShininfo::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CShininfo)
	pFX->SetFieldType(CFieldExchange::outputColumn);

	RFX_Int( pFX,  _T("SIitem"), pr.SIitem );
	RFX_Int( pFX,  _T("SIsequ"), pr.SIsequ );
	RFX_Int( pFX,  _T("SIsnum"), pr.SIsnum );
	RFX_Int( pFX,  _T("SIskey"), pr.SIskey );
	RFX_Byte( pFX, _T("SIskbn"), pr.SIskbn );
	RFX_Byte( pFX, _T("SIsign"), pr.SIsign );
	RFX_Int( pFX, _T("SIksdy"),  pr.SIksdy );
	RFX_Int( pFX, _T("SIkedy"),  pr.SIkedy );
	RFX_Int( pFX, _T("SImsdy"),  pr.SImsdy );
	RFX_Int( pFX, _T("SImedy"),  pr.SImedy );
	RFX_Int( pFX, _T("SIcsdy"),  pr.SIcsdy );
	RFX_Int( pFX, _T("SIcedy"),  pr.SIcedy );
	RFX_Int( pFX, _T("SIpday"),  pr.SIpday );
	RFX_Int( pFX, _T("SIkday"),  pr.SIkday );
	RFX_Text( pFX, _T("SIuser"), pr.SIuser );
	RFX_Int( pFX, _T("SIday1"),  pr.SIday1 );
	RFX_Text( pFX, _T("SIusr1"), pr.SIusr1 );
	RFX_Int( pFX, _T("SIday2"),  pr.SIday2 );
	RFX_Text( pFX, _T("SIusr2"), pr.SIusr2 );
	RFX_Binary( pFX, _T("SImflg"), (CByteArray&)pr.SImflg, 128 );

	//}}AFX_FIELD_MAP
}				  
				  
CString CShininfo::GetDefaultSQL()
{				  
	return _T("shininfo");
}

/////////////////////////////////////////////////////////////////////////////
// CPrfkin ƒNƒ‰ƒX‚Ìf’f

#ifdef _DEBUG
void CShininfo::AssertValid() const
{
	CRecordset::AssertValid();
}

void CShininfo::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CKJSNK
CSHININFO::CSHININFO( CShininfo *pshininfo )
{
	m_pshininfo = pshininfo;
	nodata = 0;
}

void CSHININFO::data()
{
	if( SIitem == -1 ){
		st = -1;
		SIitem = 0;
		SIsequ = 0;
		SIsnum = 0;
		SIskey = 0;
		SIskbn = 0;
		SIsign = 0;
		SIksdy = 0;
		SIkedy = 0;
		SImsdy = 0;
		SImedy = 0;
		SIcsdy = 0;
		SIcedy = 0;
		SIpday = 0;
		SIkday = 0;
		SIuser = _T("");
		SIday1 = 0;
		SIusr1 = _T("");
		SIday2 = 0;
		SIusr2 = _T("");
		memset( &SImflg[0], '\0', 128 );
		return;
	}
	st = 0;
}

int CSHININFO::MoveFirst()
{
	SIitem = -1;
	if( !nodata ){
		m_pshininfo->MoveFirst();
	}
	data();
	return st;
}

int CSHININFO::MoveNext()
{
	SIitem = -1;
	if( ! m_pshininfo->IsEOF() ){
		m_pshininfo->MoveNext();
	}
	data();
	return st;
}

int CSHININFO::MovePrev()
{
	SIitem = -1;
	if( ! m_pshininfo->IsBOF() ){
		m_pshininfo->MovePrev();
	}
	data();
	return st;
}

int CSHININFO::MoveLast()
{
	SIitem = -1;
	if( ! nodata ){
		m_pshininfo->MoveLast();
	}
	data();
	return st;
}

int CSHININFO::Move( int nrows )
{
	SIitem = -1;
	if( nrows > 0 ) {
		if( ! m_pshininfo->IsEOF() ){
			m_pshininfo->Move(nrows);
		}
	}
	else if( nrows < 0 ) {
		if( ! m_pshininfo->IsBOF() ){
			m_pshininfo->Move(nrows);
		}
	}
	else{
		m_pshininfo->Move(nrows);
	}
	data();
	return st;
}

int CSHININFO::SetAbsolutePosition( int nrows )
{
	SIitem = -1;
	if( ! nodata ){
		m_pshininfo->SetAbsolutePosition(nrows);
	}
	data();
	return st;
}

int CSHININFO::Requery( LPCTSTR filter )
{
	SIitem = -1;
	m_pshininfo->m_strFilter = filter;
	m_pshininfo->m_strSort =  _T("SIitem,SIsequ,SIsnum,SIskey");
	m_pshininfo->Requery();
	if( m_pshininfo->IsBOF() ){
		nodata = 1;
	}
	else{
		nodata = 0;
	}
	data();
	return st;
}

int CSHININFO::GetRecordCount()
{
	CRecordsetStatus	rstat;
	long	cpos;

	m_pshininfo->GetStatus( rstat );
	if( rstat.m_bRecordCountFinal ){
		return( m_pshininfo->GetRecordCount() );
	}
	else {
		if( (cpos=rstat.m_lCurrentRecord) < 0 ){
			m_pshininfo->MoveFirst();
		}
		for( ; ! m_pshininfo->IsEOF(); ){
			m_pshininfo->MoveNext();
		}
		if( cpos >= 0 ){
			m_pshininfo->SetAbsolutePosition(cpos+1);
		}
		return( m_pshininfo->GetRecordCount() );
	}
}

void CSHININFO::Edit()
{
	m_pshininfo->m_pDatabase->BeginTrans();
	m_pshininfo->Edit();
}

void CSHININFO::AddNew()
{
	m_pshininfo->m_pDatabase->BeginTrans();
	m_pshininfo->AddNew();
	SIitem = 0;
	SIsequ = 0;
	SIsnum = 0;
	SIskey = 0;
	SIskbn = 0;
	SIsign = 0;
	SIksdy = 0;
	SIkedy = 0;
	SImsdy = 0;
	SImedy = 0;
	SIcsdy = 0;
	SIcedy = 0;
	SIpday = 0;
	SIkday = 0;
	SIuser = _T("");
	SIday1 = 0;
	SIusr1 = _T("");
	SIday2 = 0;
	SIusr2 = _T("");
	SImflg.SetSize(128);
	memset( &SImflg[0], '\0',128 );
}

int CSHININFO::Update()
{
	try{
		st = m_pshininfo->Update();
	}
	catch( CDBException* dbe ){
		if( dbe->m_nRetCode == AFX_SQL_ERROR_NO_ROWS_AFFECTED ) {
			dbe->Delete();
			m_pshininfo->Requery();
			if( m_pshininfo->IsBOF() ){
				nodata = 1;
			}
			else{
				nodata = 0;
			}
			return dbe->m_nRetCode;
		}
		m_pshininfo->m_pDatabase->Rollback();
		if( m_pshininfo ){
			ErrMsg = dbe->m_strError.GetBuffer();
		}
		dbe->Delete();
		return -1;
	}

	m_pshininfo->m_pDatabase->CommitTrans();
	
	return st;
}

int CSHININFO::Delete()
{
	if( ! nodata ) {
		m_pshininfo->m_pDatabase->BeginTrans();

		for(;;){
			try{
				m_pshininfo->Delete();
			}
			catch( CDBException* dbe ){
				if( dbe->m_nRetCode == AFX_SQL_ERROR_NO_ROWS_AFFECTED ) {
					dbe->Delete();
					m_pshininfo->Requery();
					if( m_pshininfo->IsBOF() ) {
						m_pshininfo->m_pDatabase->Rollback();
						return -1;
					}
					nodata=0;
					continue;
				}
				m_pshininfo->m_pDatabase->Rollback();
				if( m_pshininfo ){
					ErrMsg = dbe->m_strError.GetBuffer();
				}
				dbe->Delete();
				return -1;
			}
			break;
		}
		m_pshininfo->m_pDatabase->CommitTrans();
		m_pshininfo->MoveNext();
	}

	return 0;
}

int CSHININFO::AllDelete()
{
	if( ! nodata ) {
	
		CString	Sql;
		Sql.Format( _T("delete from %s"), _T("shininfo") );

		m_pshininfo->m_pDatabase->BeginTrans();
		try{
			m_pshininfo->m_pDatabase->ExecuteSQL( Sql );
		}
		catch( CDBException* dbe ){
			m_pshininfo->m_pDatabase->Rollback();
			if( m_pshininfo ){
				ErrMsg = dbe->m_strError.GetBuffer();
			}
			dbe->Delete();
			return -1;
		}
		m_pshininfo->m_pDatabase->CommitTrans();

	}
	return 0;
}
