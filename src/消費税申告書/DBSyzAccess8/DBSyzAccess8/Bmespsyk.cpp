#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void AFXAPI RFX_Bigint(CFieldExchange* pFX, LPCTSTR szName, __int64& value);

/////////////////////////////////////////////////////////////////////////////
// CPrfkin

IMPLEMENT_DYNAMIC(CBmespsyk, CRecordset)

CBmespsyk::CBmespsyk( CDatabase* pdb )
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CBmespsyk)

	pr.SPtype = 0;
	pr.SPmnth = 0;
	pr.SPbumn = 0;
	pr.SPkcod = _T("");
	pr.SPedbn = 0;
	pr.SPgrp1 = 0;
	pr.SPgrp2 = 0;
	pr.SPzkbn = 0;
	pr.SPitem = 0;
	pr.SPrvsg = 0;
	pr.SPkatt = 0;
	pr.SPsgn1 = 0;
	pr.SPsgn2 = 0;

	pr.SPtkzn = _T("");
	pr.SPthzn = _T("");
	pr.SPtczn = _T("");
	pr.SPtszn = _T("");
	pr.SPtfzn = _T("");
	pr.SPval1 = _T("");
	pr.SPval2 = _T("");

	m_nFields = 30;
	//}}AFX_FIELD_INIT
}

void CBmespsyk::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CBmespsyk)
	pFX->SetFieldType(CFieldExchange::outputColumn);

	RFX_Int( pFX, _T("SPtype"), pr.SPtype );
	RFX_Int( pFX, _T("SPmnth"), pr.SPmnth );
	RFX_Long( pFX, _T("SPbumn"), pr.SPbumn );
	RFX_Text( pFX, _T("SPkcod"), pr.SPkcod, 8 );
	RFX_Long( pFX, _T("SPedbn"), pr.SPedbn );
	RFX_Byte( pFX, _T("SPgrp1"), pr.SPgrp1 );
	RFX_Byte( pFX, _T("SPgrp2"), pr.SPgrp2 );
	RFX_Byte( pFX, _T("SPzkbn"), pr.SPzkbn );
	RFX_Byte( pFX, _T("SPitem"), pr.SPitem );
	RFX_Byte( pFX, _T("SPrvsg"), pr.SPrvsg );
	RFX_Byte( pFX, _T("SPkatt"), pr.SPkatt );
	RFX_Byte( pFX, _T("SPsgn1"), pr.SPsgn1 );
	RFX_Byte( pFX, _T("SPsgn2"), pr.SPsgn2 );

	RFX_Text( pFX, _T("SPtkzn"), pr.SPtkzn, 16, SQL_DECIMAL );
	RFX_Text( pFX, _T("SPthzn"), pr.SPthzn, 16, SQL_DECIMAL );
	RFX_Text( pFX, _T("SPtczn"), pr.SPtczn, 16, SQL_DECIMAL );
	RFX_Text( pFX, _T("SPtszn"), pr.SPtszn, 16, SQL_DECIMAL );
	RFX_Text( pFX, _T("SPtfzn"), pr.SPtfzn, 16, SQL_DECIMAL );
	RFX_Text( pFX, _T("SPval1"), pr.SPval1, 16, SQL_DECIMAL );
	RFX_Text( pFX, _T("SPval2"), pr.SPval2, 16, SQL_DECIMAL );

	//}}AFX_FIELD_MAP
}				  
				  
CString CBmespsyk::GetDefaultSQL()
{				  
	return _T("bmespsyk");
}

/////////////////////////////////////////////////////////////////////////////
// CPrfkin ƒNƒ‰ƒX‚Ìf’f

#ifdef _DEBUG
void CBmespsyk::AssertValid() const
{
	CRecordset::AssertValid();
}

void CBmespsyk::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CKJSNK
CBMESPSYK::CBMESPSYK( CBmespsyk *pbmespsyk )
{
	m_pbmespsyk = pbmespsyk;
	nodata = 0;
}

void CBMESPSYK::data()
{
	if( SPtype == -1 ){
		st = -1;
		SPtype = 0;
		SPmnth = 0;
		SPbumn = 0;
		SPkcod = _T("");
		SPedbn = 0;
		SPgrp1 = 0;
		SPgrp2 = 0;
		SPzkbn = 0;
		SPitem = 0;
		SPrvsg = 0;
		SPkatt = 0;
		SPsgn1 = 0;
		SPsgn2 = 0;
		SPtkzn = _T("");
		SPthzn = _T("");
		SPtczn = _T("");
		SPtszn = _T("");
		SPtfzn = _T("");
		SPval1 = _T("");
		SPval2 = _T("");
		return;
	}
	st = 0;
}

int CBMESPSYK::MoveFirst()
{
	SPtype = -1;
	if( !nodata ){
		m_pbmespsyk->MoveFirst();
	}
	data();
	return st;
}

int CBMESPSYK::MoveNext()
{
	SPtype = -1;
	if( ! m_pbmespsyk->IsEOF() ){
		m_pbmespsyk->MoveNext();
	}
	data();
	return st;
}

int CBMESPSYK::MovePrev()
{
	SPtype = -1;
	if( ! m_pbmespsyk->IsBOF() ){
		m_pbmespsyk->MovePrev();
	}
	data();
	return st;
}

int CBMESPSYK::MoveLast()
{
	SPtype = -1;
	if( ! nodata ){
		m_pbmespsyk->MoveLast();
	}
	data();
	return st;
}

int CBMESPSYK::Move( int nrows )
{
	SPtype = -1;
	if( nrows > 0 ) {
		if( ! m_pbmespsyk->IsEOF() ){
			m_pbmespsyk->Move(nrows);
		}
	}
	else if( nrows < 0 ) {
		if( ! m_pbmespsyk->IsBOF() ){
			m_pbmespsyk->Move(nrows);
		}
	}
	else{
		m_pbmespsyk->Move(nrows);
	}
	data();
	return st;
}

int CBMESPSYK::SetAbsolutePosition( int nrows )
{
	SPtype = -1;
	if( ! nodata ){
		m_pbmespsyk->SetAbsolutePosition(nrows);
	}
	data();
	return st;
}

int CBMESPSYK::Requery( LPCTSTR filter )
{
	SPtype = -1;
	m_pbmespsyk->m_strFilter = filter;
	m_pbmespsyk->m_strSort =  _T("SPtype,SPmnth,SPbumn,SPkcod,SPedbn,SPgrp1,SPgrp2,SPzkbn,SPitem,SPrvsg");
	m_pbmespsyk->Requery();
	if( m_pbmespsyk->IsBOF() ){
		nodata = 1;
	}
	else{
		nodata = 0;
	}
	data();
	return st;
}

int CBMESPSYK::GetRecordCount()
{
	CRecordsetStatus	rstat;
	long	cpos;

	m_pbmespsyk->GetStatus( rstat );
	if( rstat.m_bRecordCountFinal ){
		return( m_pbmespsyk->GetRecordCount() );
	}
	else {
		if( (cpos=rstat.m_lCurrentRecord) < 0 ){
			m_pbmespsyk->MoveFirst();
		}
		for( ; ! m_pbmespsyk->IsEOF(); ){
			m_pbmespsyk->MoveNext();
		}
		if( cpos >= 0 ){
			m_pbmespsyk->SetAbsolutePosition(cpos+1);
		}
		return( m_pbmespsyk->GetRecordCount() );
	}
}

void CBMESPSYK::Edit()
{
	m_pbmespsyk->m_pDatabase->BeginTrans();
	m_pbmespsyk->Edit();
}

void CBMESPSYK::AddNew()
{
	m_pbmespsyk->m_pDatabase->BeginTrans();
	m_pbmespsyk->AddNew();
	SPtype = 0;
	SPmnth = 0;
	SPbumn = 0;
	SPkcod = _T("");
	SPedbn = 0;
	SPgrp1 = 0;
	SPgrp2 = 0;
	SPzkbn = 0;
	SPitem = 0;
	SPrvsg = 0;
	SPkatt = 0;
	SPsgn1 = 0;
	SPsgn2 = 0;
	SPtkzn = _T("");
	SPthzn = _T("");
	SPtczn = _T("");
	SPtszn = _T("");
	SPtfzn = _T("");
	SPval1 = _T("");
	SPval2 = _T("");
}

int CBMESPSYK::Update()
{
	try{
		st = m_pbmespsyk->Update();
	}
	catch( CDBException* dbe ){
		if( dbe->m_nRetCode == AFX_SQL_ERROR_NO_ROWS_AFFECTED ) {
			dbe->Delete();
			m_pbmespsyk->Requery();
			if( m_pbmespsyk->IsBOF() ){
				nodata = 1;
			}
			else{
				nodata = 0;
			}
			return dbe->m_nRetCode;
		}
		m_pbmespsyk->m_pDatabase->Rollback();
		if( m_pbmespsyk ){
			ErrMsg = dbe->m_strError.GetBuffer();
		}
		dbe->Delete();
		return -1;
	}

	m_pbmespsyk->m_pDatabase->CommitTrans();
	
	return st;
}

int CBMESPSYK::Delete()
{
	if( ! nodata ) {
		m_pbmespsyk->m_pDatabase->BeginTrans();

		for(;;){
			try{
				m_pbmespsyk->Delete();
			}
			catch( CDBException* dbe ){
				if( dbe->m_nRetCode == AFX_SQL_ERROR_NO_ROWS_AFFECTED ) {
					dbe->Delete();
					m_pbmespsyk->Requery();
					if( m_pbmespsyk->IsBOF() ) {
						m_pbmespsyk->m_pDatabase->Rollback();
						return -1;
					}
					nodata=0;
					continue;
				}
				m_pbmespsyk->m_pDatabase->Rollback();
				if( m_pbmespsyk ){
					ErrMsg = dbe->m_strError.GetBuffer();
				}
				dbe->Delete();
				return -1;
			}
			break;
		}
		m_pbmespsyk->m_pDatabase->CommitTrans();
		m_pbmespsyk->MoveNext();
	}

	return 0;
}

int CBMESPSYK::AllDelete()
{
	if( ! nodata ) {
	
		CString	Sql;
		Sql.Format( _T("delete from %s"), _T("bmespsyk") );

		m_pbmespsyk->m_pDatabase->BeginTrans();
		try{
			m_pbmespsyk->m_pDatabase->ExecuteSQL( Sql );
		}
		catch( CDBException* dbe ){
			m_pbmespsyk->m_pDatabase->Rollback();
			if( m_pbmespsyk ){
				ErrMsg = dbe->m_strError.GetBuffer();
			}
			dbe->Delete();
			return -1;
		}
		m_pbmespsyk->m_pDatabase->CommitTrans();

	}
	return 0;
}
