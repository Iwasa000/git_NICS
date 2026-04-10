// 
#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void AFXAPI RFX_Bigint(CFieldExchange* pFX, LPCTSTR szName, __int64& value);

/////////////////////////////////////////////////////////////////////////////
// CPrfkin

IMPLEMENT_DYNAMIC(CBmeszsyk, CRecordset)

CBmeszsyk::CBmeszsyk( CDatabase* pdb )
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CBmeszsyk)

	pr.SZtype = 0;
	pr.SZmnth = 0;
	pr.SZbumn = 0;
	pr.SZkcod = _T("");
	pr.SZedbn = 0;
	pr.SZgrp1 = 0;
	pr.SZgrp2 = 0;
	pr.SZzkbn = 0;
	pr.SZitem = 0;
	pr.SZsjsg = 0;
	pr.SZrvsg = 0;
	pr.SZkatt = 0;
	pr.SZzchg = 0;
	pr.SZsgn1 = 0;
	pr.SZsgn2 = 0;

	pr.SZtaig = _T("");
	pr.SZhikz = _T("");
	pr.SZyuka = _T("");
	pr.SZkomi = _T("");
	pr.SZkomz = _T("");
	pr.SZnuki = _T("");
	pr.SZnukz = _T("");
	pr.SZbeki = _T("");
	pr.SZikkm = _T("");
	pr.SZikkz = _T("");
	pr.SZadjs = _T("");
	pr.SZiptk = _T("");
	pr.SZiptt = _T("");
	pr.SZval1 = _T("");
	pr.SZval2 = _T("");

	m_nFields = 30;
	//}}AFX_FIELD_INIT
}

void CBmeszsyk::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CBmeszsyk)
	pFX->SetFieldType(CFieldExchange::outputColumn);

	RFX_Int( pFX, _T("SZtype"), pr.SZtype );
	RFX_Int( pFX, _T("SZmnth"), pr.SZmnth );
	RFX_Long( pFX, _T("SZbumn"), pr.SZbumn );
	RFX_Text( pFX, _T("SZkcod"), pr.SZkcod, 8 );
	RFX_Long( pFX, _T("SZedbn"), pr.SZedbn );
	RFX_Byte( pFX, _T("SZgrp1"), pr.SZgrp1 );
	RFX_Byte( pFX, _T("SZgrp2"), pr.SZgrp2 );
	RFX_Byte( pFX, _T("SZzkbn"), pr.SZzkbn );
	RFX_Byte( pFX, _T("SZitem"), pr.SZitem );
	RFX_Byte( pFX, _T("SZsjsg"), pr.SZsjsg );
	RFX_Byte( pFX, _T("SZrvsg"), pr.SZrvsg );
	RFX_Byte( pFX, _T("SZkatt"), pr.SZkatt );
	RFX_Byte( pFX, _T("SZzchg"), pr.SZzchg );
	RFX_Byte( pFX, _T("SZsgn1"), pr.SZsgn1 );
	RFX_Byte( pFX, _T("SZsgn2"), pr.SZsgn2 );

	RFX_Text( pFX, _T("SZtaig"), pr.SZtaig, 16, SQL_DECIMAL );
	RFX_Text( pFX, _T("SZhikz"), pr.SZhikz, 16, SQL_DECIMAL );
	RFX_Text( pFX, _T("SZyuka"), pr.SZyuka, 16, SQL_DECIMAL );
	RFX_Text( pFX, _T("SZkomi"), pr.SZkomi, 16, SQL_DECIMAL );
	RFX_Text( pFX, _T("SZkomz"), pr.SZkomz, 16, SQL_DECIMAL );
	RFX_Text( pFX, _T("SZnuki"), pr.SZnuki, 16, SQL_DECIMAL );
	RFX_Text( pFX, _T("SZnukz"), pr.SZnukz, 16, SQL_DECIMAL );
	RFX_Text( pFX, _T("SZbeki"), pr.SZbeki, 16, SQL_DECIMAL );
	RFX_Text( pFX, _T("SZikkm"), pr.SZikkm, 16, SQL_DECIMAL );
	RFX_Text( pFX, _T("SZikkz"), pr.SZikkz, 16, SQL_DECIMAL );
	RFX_Text( pFX, _T("SZadjs"), pr.SZadjs, 16, SQL_DECIMAL );
	RFX_Text( pFX, _T("SZiptk"), pr.SZiptk, 16, SQL_DECIMAL );
	RFX_Text( pFX, _T("SZiptt"), pr.SZiptt, 16, SQL_DECIMAL );
	RFX_Text( pFX, _T("SZval1"), pr.SZval1, 16, SQL_DECIMAL );
	RFX_Text( pFX, _T("SZval2"), pr.SZval2, 16, SQL_DECIMAL );


	//}}AFX_FIELD_MAP
}				  
				  
CString CBmeszsyk::GetDefaultSQL()
{				  
	return _T("bmeszsyk");
}

/////////////////////////////////////////////////////////////////////////////
// CPrfkin ƒNƒ‰ƒX‚Ìf’f

#ifdef _DEBUG
void CBmeszsyk::AssertValid() const
{
	CRecordset::AssertValid();
}

void CBmeszsyk::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CKJSNK
CBMESZSYK::CBMESZSYK( CBmeszsyk *pbmeszsyk )
{
	m_pbmeszsyk = pbmeszsyk;
	nodata = 0;
}

void CBMESZSYK::data()
{
	if( SZtype == -1 ){
		st = -1;
		SZtype = 0;
		SZmnth = 0;
		SZbumn = 0;
		SZkcod = _T("");
		SZedbn = 0;
		SZgrp1 = 0;
		SZgrp2 = 0;
		SZzkbn = 0;
		SZitem = 0;
		SZsjsg = 0;
		SZrvsg = 0;
		SZkatt = 0;
		SZzchg = 0;
		SZsgn1 = 0;
		SZsgn2 = 0;
		SZtaig = _T("");
		SZhikz = _T("");
		SZyuka = _T("");
		SZkomi = _T("");
		SZkomz = _T("");
		SZnuki = _T("");
		SZnukz = _T("");
		SZbeki = _T("");
		SZikkm = _T("");
		SZikkz = _T("");
		SZadjs = _T("");
		SZiptk = _T("");
		SZiptt = _T("");
		SZval1 = _T("");
		SZval2 = _T("");
		return;
	}
	st = 0;
}

int CBMESZSYK::MoveFirst()
{
	SZtype = -1;
	if( !nodata ){
		m_pbmeszsyk->MoveFirst();
	}
	data();
	return st;
}

int CBMESZSYK::MoveNext()
{
	SZtype = -1;
	if( ! m_pbmeszsyk->IsEOF() ){
		m_pbmeszsyk->MoveNext();
	}
	data();
	return st;
}

int CBMESZSYK::MovePrev()
{
	SZtype = -1;
	if( ! m_pbmeszsyk->IsBOF() ){
		m_pbmeszsyk->MovePrev();
	}
	data();
	return st;
}

int CBMESZSYK::MoveLast()
{
	SZtype = -1;
	if( ! nodata ){
		m_pbmeszsyk->MoveLast();
	}
	data();
	return st;
}

int CBMESZSYK::Move( int nrows )
{
	SZtype = -1;
	if( nrows > 0 ) {
		if( ! m_pbmeszsyk->IsEOF() ){
			m_pbmeszsyk->Move(nrows);
		}
	}
	else if( nrows < 0 ) {
		if( ! m_pbmeszsyk->IsBOF() ){
			m_pbmeszsyk->Move(nrows);
		}
	}
	else{
		m_pbmeszsyk->Move(nrows);
	}
	data();
	return st;
}

int CBMESZSYK::SetAbsolutePosition( int nrows )
{
	SZtype = -1;
	if( ! nodata ){
		m_pbmeszsyk->SetAbsolutePosition(nrows);
	}
	data();
	return st;
}

int CBMESZSYK::Requery( LPCTSTR filter )
{
	SZtype = -1;
	m_pbmeszsyk->m_strFilter = filter;
	m_pbmeszsyk->m_strSort =  _T("SZtype,SZmnth,SZbumn,SZkcod,SZgrp1,SZgrp2,SZzkbn,SZitem,SZsjsg,SZrvsg");
	m_pbmeszsyk->Requery();
	if( m_pbmeszsyk->IsBOF() ){
		nodata = 1;
	}
	else{
		nodata = 0;
	}
	data();
	return st;
}

int CBMESZSYK::GetRecordCount()
{
	CRecordsetStatus	rstat;
	long	cpos;

	m_pbmeszsyk->GetStatus( rstat );
	if( rstat.m_bRecordCountFinal ){
		return( m_pbmeszsyk->GetRecordCount() );
	}
	else {
		if( (cpos=rstat.m_lCurrentRecord) < 0 ){
			m_pbmeszsyk->MoveFirst();
		}
		for( ; ! m_pbmeszsyk->IsEOF(); ){
			m_pbmeszsyk->MoveNext();
		}
		if( cpos >= 0 ){
			m_pbmeszsyk->SetAbsolutePosition(cpos+1);
		}
		return( m_pbmeszsyk->GetRecordCount() );
	}
}

void CBMESZSYK::Edit()
{
	m_pbmeszsyk->m_pDatabase->BeginTrans();
	m_pbmeszsyk->Edit();
}

void CBMESZSYK::AddNew()
{
	m_pbmeszsyk->m_pDatabase->BeginTrans();
	m_pbmeszsyk->AddNew();
	SZtype = 0;
	SZmnth = 0;
	SZbumn = 0;
	SZkcod = _T("");
	SZedbn = 0;
	SZgrp1 = 0;
	SZgrp2 = 0;
	SZzkbn = 0;
	SZitem = 0;
	SZsjsg = 0;
	SZrvsg = 0;
	SZkatt = 0;
	SZzchg = 0;
	SZsgn1 = 0;
	SZsgn2 = 0;
	SZtaig = _T("");
	SZhikz = _T("");
	SZyuka = _T("");
	SZkomi = _T("");
	SZkomz = _T("");
	SZnuki = _T("");
	SZnukz = _T("");
	SZbeki = _T("");
	SZikkm = _T("");
	SZikkz = _T("");
	SZadjs = _T("");
	SZiptk = _T("");
	SZiptt = _T("");
	SZval1 = _T("");
	SZval2 = _T("");
}

int CBMESZSYK::Update()
{
	try{
		st = m_pbmeszsyk->Update();
	}
	catch( CDBException* dbe ){
		if( dbe->m_nRetCode == AFX_SQL_ERROR_NO_ROWS_AFFECTED ) {
			dbe->Delete();
			m_pbmeszsyk->Requery();
			if( m_pbmeszsyk->IsBOF() ){
				nodata = 1;
			}
			else{
				nodata = 0;
			}
			return dbe->m_nRetCode;
		}
		m_pbmeszsyk->m_pDatabase->Rollback();
		if( m_pbmeszsyk ){
			ErrMsg = dbe->m_strError.GetBuffer();
		}
		dbe->Delete();
		return -1;
	}

	m_pbmeszsyk->m_pDatabase->CommitTrans();
	
	return st;
}

int CBMESZSYK::Delete()
{
	if( ! nodata ) {
		m_pbmeszsyk->m_pDatabase->BeginTrans();

		for(;;){
			try{
				m_pbmeszsyk->Delete();
			}
			catch( CDBException* dbe ){
				if( dbe->m_nRetCode == AFX_SQL_ERROR_NO_ROWS_AFFECTED ) {
					dbe->Delete();
					m_pbmeszsyk->Requery();
					if( m_pbmeszsyk->IsBOF() ) {
						m_pbmeszsyk->m_pDatabase->Rollback();
						return -1;
					}
					nodata=0;
					continue;
				}
				m_pbmeszsyk->m_pDatabase->Rollback();
				if( m_pbmeszsyk ){
					ErrMsg = dbe->m_strError.GetBuffer();
				}
				dbe->Delete();
				return -1;
			}
			break;
		}
		m_pbmeszsyk->m_pDatabase->CommitTrans();
		m_pbmeszsyk->MoveNext();
	}

	return 0;
}

int CBMESZSYK::AllDelete()
{
	if( ! nodata ) {
	
		CString	Sql;
		Sql.Format( _T("delete from %s"), _T("bmeszsyk") );

		m_pbmeszsyk->m_pDatabase->BeginTrans();
		try{
			m_pbmeszsyk->m_pDatabase->ExecuteSQL( Sql );
		}
		catch( CDBException* dbe ){
			m_pbmeszsyk->m_pDatabase->Rollback();
			if( m_pbmeszsyk ){
				ErrMsg = dbe->m_strError.GetBuffer();
			}
			dbe->Delete();
			return -1;
		}
		m_pbmeszsyk->m_pDatabase->CommitTrans();

	}
	return 0;
}
