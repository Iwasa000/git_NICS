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

IMPLEMENT_DYNAMIC(CRefundrec, CRecordset)

CRefundrec::CRefundrec( CDatabase* pdb )
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CRefundrec)

	pr.KPItem = 0;
	pr.KPLine = 0;
	pr.KPResv = 0;
	pr.KPSequ = 0;
	pr.KPSvl1 = _T("");
	pr.KPSvl2 = _T("");
	pr.KPSvl3 = _T("");
	pr.KPSvl4 = _T("");
	pr.KPSsup = _T("");
	pr.KPSslw = _T("");
	pr.KPSday = 0;
	pr.KPSdys = 0;
	pr.KPSddm = 0;
	pr.KPSval = _T("");
	pr.KPSnup = _T("");
	pr.KPSnlw = _T("");
	pr.KPSaup = _T("");
	pr.KPSalw = _T("");
	pr.KPStup = _T("");
	pr.KPStlw = _T("");
	pr.KPSzup = _T("");
	pr.KPSzlw = _T("");
	pr.KPInvno = _T("");	// 20230704

//	m_nFields = 22;
	m_nFields = 23; // 20230704
	//}}AFX_FIELD_INIT
}

void CRefundrec::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CRefundrec)
	pFX->SetFieldType(CFieldExchange::outputColumn);

	RFX_Int( pFX,  _T("KPItem"), pr.KPItem );
	RFX_Int( pFX,  _T("KPLine"), pr.KPLine );
	RFX_Int( pFX,  _T("KPResv"), pr.KPResv );
	RFX_Int( pFX,  _T("KPSequ"), pr.KPSequ );
	RFX_Text( pFX, _T("KPSvl1"), pr.KPSvl1, 16, SQL_DECIMAL );
	RFX_Text( pFX, _T("KPSvl2"), pr.KPSvl2, 16, SQL_DECIMAL );
	RFX_Text( pFX, _T("KPSvl3"), pr.KPSvl3, 16, SQL_DECIMAL );
	RFX_Text( pFX, _T("KPSvl4"), pr.KPSvl4, 16, SQL_DECIMAL );
	RFX_Text( pFX, _T("KPSsup"), pr.KPSsup );
	RFX_Text( pFX, _T("KPSslw"), pr.KPSslw );
	RFX_Int( pFX,  _T("KPSday"), pr.KPSday );
	RFX_Byte( pFX, _T("KPSdys"), pr.KPSdys );
	RFX_Byte( pFX, _T("KPSddm"), pr.KPSddm );
	RFX_Text( pFX, _T("KPSval"), pr.KPSval, 16, SQL_DECIMAL );
	RFX_Text( pFX, _T("KPSnup"), pr.KPSnup );
	RFX_Text( pFX, _T("KPSnlw"), pr.KPSnlw );
	RFX_Text( pFX, _T("KPSaup"), pr.KPSaup );
	RFX_Text( pFX, _T("KPSalw"), pr.KPSalw );
	RFX_Text( pFX, _T("KPStup"), pr.KPStup );
	RFX_Text( pFX, _T("KPStlw"), pr.KPStlw );
	RFX_Text( pFX, _T("KPSzup"), pr.KPSzup );
	RFX_Text( pFX, _T("KPSzlw"), pr.KPSzlw );
	RFX_Text( pFX, _T("KPInvno"), pr.KPInvno );	// 20230704

	//}}AFX_FIELD_MAP
}				  
				  
CString CRefundrec::GetDefaultSQL()
{				  
	return _T("refundrec");
}

/////////////////////////////////////////////////////////////////////////////
// CPrfkin ƒNƒ‰ƒX‚Ìf’f

#ifdef _DEBUG
void CRefundrec::AssertValid() const
{
	CRecordset::AssertValid();
}

void CRefundrec::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CKJSNK
CREFUNDREC::CREFUNDREC( CRefundrec *prefundrec )
{
	m_prefundrec = prefundrec;
	nodata = 0;
}

void CREFUNDREC::data()
{
	if( KPItem == -1 ){
		st = -1;
		KPItem = 0;
		KPLine = 0;
		KPResv = 0;
		KPSequ = 0;
		KPSvl1 = _T("");
		KPSvl2 = _T("");
		KPSvl3 = _T("");
		KPSvl4 = _T("");
		KPSsup = _T("");
		KPSslw = _T("");
		KPSday = 0;
		KPSdys = 0;
		KPSddm = 0;
		KPSval = _T("");
		KPSnup = _T("");
		KPSnlw = _T("");
		KPSaup = _T("");
		KPSalw = _T("");
		KPStup = _T("");
		KPStlw = _T("");
		KPSzup = _T("");
		KPSzlw = _T("");
		KPInvno = _T("");	// 20230704
		return;
	}
	st = 0;
}

int CREFUNDREC::MoveFirst()
{
	KPItem = -1;
	if( !nodata ){
		m_prefundrec->MoveFirst();
	}
	data();
	return st;
}

int CREFUNDREC::MoveNext()
{
	KPItem = -1;
	if( ! m_prefundrec->IsEOF() ){
		m_prefundrec->MoveNext();
	}
	data();
	return st;
}

int CREFUNDREC::MovePrev()
{
	KPItem = -1;
	if( ! m_prefundrec->IsBOF() ){
		m_prefundrec->MovePrev();
	}
	data();
	return st;
}

int CREFUNDREC::MoveLast()
{
	KPItem = -1;
	if( ! nodata ){
		m_prefundrec->MoveLast();
	}
	data();
	return st;
}

int CREFUNDREC::Move( int nrows )
{
	KPItem = -1;
	if( nrows > 0 ) {
		if( ! m_prefundrec->IsEOF() ){
			m_prefundrec->Move(nrows);
		}
	}
	else if( nrows < 0 ) {
		if( ! m_prefundrec->IsBOF() ){
			m_prefundrec->Move(nrows);
		}
	}
	else{
		m_prefundrec->Move(nrows);
	}
	data();
	return st;
}

int CREFUNDREC::SetAbsolutePosition( int nrows )
{
	KPItem = -1;
	if( ! nodata ){
		m_prefundrec->SetAbsolutePosition(nrows);
	}
	data();
	return st;
}

int CREFUNDREC::Requery( LPCTSTR filter )
{
	KPItem = -1;
	m_prefundrec->m_strFilter = filter;
	m_prefundrec->m_strSort =  _T("KPItem,KPLine,KPResv,KPSequ");
	m_prefundrec->Requery();
	if( m_prefundrec->IsBOF() ){
		nodata = 1;
	}
	else{
		nodata = 0;
	}
	data();
	return st;
}

int CREFUNDREC::GetRecordCount()
{
	CRecordsetStatus	rstat;
	long	cpos;

	m_prefundrec->GetStatus( rstat );
	if( rstat.m_bRecordCountFinal ){
		return( m_prefundrec->GetRecordCount() );
	}
	else {
		if( (cpos=rstat.m_lCurrentRecord) < 0 ){
			m_prefundrec->MoveFirst();
		}
		for( ; ! m_prefundrec->IsEOF(); ){
			m_prefundrec->MoveNext();
		}
		if( cpos >= 0 ){
			m_prefundrec->SetAbsolutePosition(cpos+1);
		}
		return( m_prefundrec->GetRecordCount() );
	}
}

void CREFUNDREC::Edit()
{
	m_prefundrec->m_pDatabase->BeginTrans();
	m_prefundrec->Edit();
}

void CREFUNDREC::AddNew()
{
	m_prefundrec->m_pDatabase->BeginTrans();
	m_prefundrec->AddNew();
	KPItem = 0;
	KPLine = 0;
	KPResv = 0;
	KPSequ = 0;
	KPSvl1 = _T("");
	KPSvl2 = _T("");
	KPSvl3 = _T("");
	KPSvl4 = _T("");
	KPSsup = _T("");
	KPSslw = _T("");
	KPSday = 0;
	KPSdys = 0;
	KPSddm = 0;
	KPSval = _T("");
	KPSnup = _T("");
	KPSnlw = _T("");
	KPSaup = _T("");
	KPSalw = _T("");
	KPStup = _T("");
	KPStlw = _T("");
	KPSzup = _T("");
	KPSzlw = _T("");
	KPInvno = _T("");	// 20230704
}

int CREFUNDREC::Update()
{
	try{
		st = m_prefundrec->Update();
	}
	catch( CDBException* dbe ){
		if( dbe->m_nRetCode == AFX_SQL_ERROR_NO_ROWS_AFFECTED ) {
			dbe->Delete();
			m_prefundrec->Requery();
			if( m_prefundrec->IsBOF() ){
				nodata = 1;
			}
			else{
				nodata = 0;
			}
			return dbe->m_nRetCode;
		}
		m_prefundrec->m_pDatabase->Rollback();
		if( m_prefundrec ){
			ErrMsg = dbe->m_strError.GetBuffer();
		}
		dbe->Delete();
		return -1;
	}

	m_prefundrec->m_pDatabase->CommitTrans();
	
	return st;
}

int CREFUNDREC::Delete()
{
	if( ! nodata ) {
		m_prefundrec->m_pDatabase->BeginTrans();

		for(;;){
			try{
				m_prefundrec->Delete();
			}
			catch( CDBException* dbe ){
				if( dbe->m_nRetCode == AFX_SQL_ERROR_NO_ROWS_AFFECTED ) {
					dbe->Delete();
					m_prefundrec->Requery();
					if( m_prefundrec->IsBOF() ) {
						m_prefundrec->m_pDatabase->Rollback();
						return -1;
					}
					nodata=0;
					continue;
				}
				m_prefundrec->m_pDatabase->Rollback();
				if( m_prefundrec ){
					ErrMsg = dbe->m_strError.GetBuffer();
				}
				dbe->Delete();
				return -1;
			}
			break;
		}
		m_prefundrec->m_pDatabase->CommitTrans();
		m_prefundrec->MoveNext();
	}

	return 0;
}

int CREFUNDREC::AllDelete()
{
	if( ! nodata ) {
	
		CString	Sql;
		Sql.Format( _T("delete from %s"), _T("refundrec") );

		m_prefundrec->m_pDatabase->BeginTrans();
		try{
			m_prefundrec->m_pDatabase->ExecuteSQL( Sql );
		}
		catch( CDBException* dbe ){
			m_prefundrec->m_pDatabase->Rollback();
			if( m_prefundrec ){
				ErrMsg = dbe->m_strError.GetBuffer();
			}
			dbe->Delete();
			return -1;
		}
		m_prefundrec->m_pDatabase->CommitTrans();

	}
	return 0;
}
