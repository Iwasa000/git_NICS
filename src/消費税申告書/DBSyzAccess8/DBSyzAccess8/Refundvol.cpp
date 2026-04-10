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

IMPLEMENT_DYNAMIC(CRefundvol, CRecordset)

CRefundvol::CRefundvol( CDatabase* pdb )
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CRefundvol)

	pr.KPItem = 0;
	pr.KPLine = 0;
	pr.KPResv = 0;
	pr.KPSequ = 0;
	pr.KPType = 0;
	pr.KPRsgn = 0;
	pr.KPRsdm = 0;
	pr.KPRrup = _T("");
	pr.KPRrlw = _T("");
	pr.KPRrdm = _T("");
	pr.KPKuck = 0;
	pr.KPKusg = 0;
	pr.KPEbnk = _T("");
	pr.KPEbsg = 0;
	pr.KPEbdm = 0;
	pr.KPEstn = _T("");
	pr.KPEssg = 0;
	pr.KPEsdm = 0;
	pr.KPEykn = _T("");
	pr.KPEkoz = _T("");
	pr.KPEyn1 = _T("");
	pr.KPEyn2 = _T("");
	pr.KPEsup = _T("");
	pr.KPEslw = _T("");
	pr.KPEaup = _T("");
	pr.KPEalw = _T("");
	pr.KPSsgn = 0;
	pr.KPSsta = 0;
	pr.KPSnam = _T("");
	pr.KPTsck = 0;
	pr.KPTsgn = 0;
	pr.KPKsck = 0;
	pr.KPKsgn = 0;
	pr.KPSnen = 0;
	pr.KPSndm = 0;
	pr.KPSpn1 = _T("");
	pr.KPSpn2 = _T("");
	pr.KPSpn3 = _T("");
	pr.KPSpn4 = _T("");
	pr.KPSpn5 = _T("");
	pr.KPSpn6 = _T("");

	m_nFields = 41;
	//}}AFX_FIELD_INIT
}

void CRefundvol::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CRefundvol)
	pFX->SetFieldType(CFieldExchange::outputColumn);

	RFX_Int( pFX, _T("KPItem"), pr.KPItem );
	RFX_Int( pFX, _T("KPLine"), pr.KPLine );
	RFX_Int( pFX, _T("KPResv"), pr.KPResv );
	RFX_Int( pFX, _T("KPSequ"), pr.KPSequ );
	RFX_Int( pFX, _T("KPType"), pr.KPType );
	RFX_Byte( pFX, _T("KPRsgn"), pr.KPRsgn );
	RFX_Byte( pFX, _T("KPRsdm"), pr.KPRsdm );
	RFX_Text( pFX, _T("KPRrup"), pr.KPRrup );
	RFX_Text( pFX, _T("KPRrlw"), pr.KPRrlw );
	RFX_Text( pFX, _T("KPRrdm"), pr.KPRrlw );
	RFX_Byte( pFX, _T("KPKuck"), pr.KPKuck );
	RFX_Byte( pFX, _T("KPKusg"), pr.KPKusg );
	RFX_Text( pFX, _T("KPEbnk"), pr.KPEbnk );
	RFX_Byte( pFX, _T("KPEbsg"), pr.KPEbsg );
	RFX_Byte( pFX, _T("KPEbdm"), pr.KPEbdm );
	RFX_Text( pFX, _T("KPEstn"), pr.KPEstn );
	RFX_Byte( pFX, _T("KPEssg"), pr.KPEssg );
	RFX_Byte( pFX, _T("KPEsdm"), pr.KPEsdm );
	RFX_Text( pFX, _T("KPEykn"), pr.KPEykn );
	RFX_Text( pFX, _T("KPEkoz"), pr.KPEkoz );
	RFX_Text( pFX, _T("KPEyn1"), pr.KPEyn1 );
	RFX_Text( pFX, _T("KPEyn2"), pr.KPEyn2 );
	RFX_Text( pFX, _T("KPEsup"), pr.KPEsup );
	RFX_Text( pFX, _T("KPEslw"), pr.KPEslw );
	RFX_Text( pFX, _T("KPEaup"), pr.KPEaup );
	RFX_Text( pFX, _T("KPEalw"), pr.KPEalw );
	RFX_Byte( pFX, _T("KPSsgn"), pr.KPSsgn );
	RFX_Byte( pFX, _T("KPSsta"), pr.KPSsta );
	RFX_Text( pFX, _T("KPSnam"), pr.KPSnam );
	RFX_Byte( pFX, _T("KPTsck"), pr.KPTsck );
	RFX_Byte( pFX, _T("KPTsgn"), pr.KPTsgn );
	RFX_Byte( pFX, _T("KPKsck"), pr.KPKsck );
	RFX_Byte( pFX, _T("KPKsgn"), pr.KPKsgn );
	RFX_Byte( pFX, _T("KPSnen"), pr.KPSnen );
	RFX_Byte( pFX, _T("KPSndm"), pr.KPSndm );
	RFX_Text( pFX, _T("KPSpn1"), pr.KPSpn1 );
	RFX_Text( pFX, _T("KPSpn2"), pr.KPSpn2 );
	RFX_Text( pFX, _T("KPSpn3"), pr.KPSpn3 );
	RFX_Text( pFX, _T("KPSpn4"), pr.KPSpn4 );
	RFX_Text( pFX, _T("KPSpn5"), pr.KPSpn5 );
	RFX_Text( pFX, _T("KPSpn6"), pr.KPSpn6 );

	//}}AFX_FIELD_MAP
}				  
				  
CString CRefundvol::GetDefaultSQL()
{				  
	return _T("refundvol");
}

/////////////////////////////////////////////////////////////////////////////
// CPrfkin ƒNƒ‰ƒX‚Ìf’f

#ifdef _DEBUG
void CRefundvol::AssertValid() const
{
	CRecordset::AssertValid();
}

void CRefundvol::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CKJSNK
CREFUNDVOL::CREFUNDVOL( CRefundvol *pbmeszsyk )
{
	m_prefundvol = pbmeszsyk;
	nodata = 0;
}

void CREFUNDVOL::data()
{
	if( KPItem == -1 ){
		st = -1;
		KPItem = 0;
		KPLine = 0;
		KPResv = 0;
		KPSequ = 0;
		KPType = 0;
		KPRsgn = 0;
		KPRsdm = 0;
		KPRrup = _T("");
		KPRrlw = _T("");
		KPRrdm = _T("");
		KPKuck = 0;
		KPKusg = 0;
		KPEbnk = _T("");
		KPEbsg = 0;
		KPEbdm = 0;
		KPEstn = _T("");
		KPEssg = 0;
		KPEsdm = 0;
		KPEykn = _T("");
		KPEkoz = _T("");
		KPEyn1 = _T("");
		KPEyn2 = _T("");
		KPEsup = _T("");
		KPEslw = _T("");
		KPEaup = _T("");
		KPEalw = _T("");
		KPSsgn = 0;
		KPSsta = 0;
		KPSnam = _T("");
		KPTsck = 0;
		KPTsgn = 0;
		KPKsck = 0;
		KPKsgn = 0;
		KPSnen = 0;
		KPSndm = 0;
		KPSpn1 = _T("");
		KPSpn2 = _T("");
		KPSpn3 = _T("");
		KPSpn4 = _T("");
		KPSpn5 = _T("");
		KPSpn6 = _T("");
		return;
	}
	st = 0;
}

int CREFUNDVOL::MoveFirst()
{
	KPItem = -1;
	if( !nodata ){
		m_prefundvol->MoveFirst();
	}
	data();
	return st;
}

int CREFUNDVOL::MoveNext()
{
	KPItem = -1;
	if( ! m_prefundvol->IsEOF() ){
		m_prefundvol->MoveNext();
	}
	data();
	return st;
}

int CREFUNDVOL::MovePrev()
{
	KPItem = -1;
	if( ! m_prefundvol->IsBOF() ){
		m_prefundvol->MovePrev();
	}
	data();
	return st;
}

int CREFUNDVOL::MoveLast()
{
	KPItem = -1;
	if( ! nodata ){
		m_prefundvol->MoveLast();
	}
	data();
	return st;
}

int CREFUNDVOL::Move( int nrows )
{
	KPItem = -1;
	if( nrows > 0 ) {
		if( ! m_prefundvol->IsEOF() ){
			m_prefundvol->Move(nrows);
		}
	}
	else if( nrows < 0 ) {
		if( ! m_prefundvol->IsBOF() ){
			m_prefundvol->Move(nrows);
		}
	}
	else{
		m_prefundvol->Move(nrows);
	}
	data();
	return st;
}

int CREFUNDVOL::SetAbsolutePosition( int nrows )
{
	KPItem = -1;
	if( ! nodata ){
		m_prefundvol->SetAbsolutePosition(nrows);
	}
	data();
	return st;
}

int CREFUNDVOL::Requery( LPCTSTR filter )
{
	KPItem = -1;
	m_prefundvol->m_strFilter = filter;
	m_prefundvol->m_strSort =  _T("KPItem,KPLine,KPResv,KPSequ");
	m_prefundvol->Requery();
	if( m_prefundvol->IsBOF() ){
		nodata = 1;
	}
	else{
		nodata = 0;
	}
	data();
	return st;
}

int CREFUNDVOL::GetRecordCount()
{
	CRecordsetStatus	rstat;
	long	cpos;

	m_prefundvol->GetStatus( rstat );
	if( rstat.m_bRecordCountFinal ){
		return( m_prefundvol->GetRecordCount() );
	}
	else {
		if( (cpos=rstat.m_lCurrentRecord) < 0 ){
			m_prefundvol->MoveFirst();
		}
		for( ; ! m_prefundvol->IsEOF(); ){
			m_prefundvol->MoveNext();
		}
		if( cpos >= 0 ){
			m_prefundvol->SetAbsolutePosition(cpos+1);
		}
		return( m_prefundvol->GetRecordCount() );
	}
}

void CREFUNDVOL::Edit()
{
	m_prefundvol->m_pDatabase->BeginTrans();
	m_prefundvol->Edit();
}

void CREFUNDVOL::AddNew()
{
	m_prefundvol->m_pDatabase->BeginTrans();
	m_prefundvol->AddNew();
	KPItem = 0;
	KPLine = 0;
	KPResv = 0;
	KPSequ = 0;
	KPType = 0;
	KPRsgn = 0;
	KPRsdm = 0;
	KPRrup = _T("");
	KPRrlw = _T("");
	KPRrdm = _T("");
	KPKuck = 0;
	KPKusg = 0;
	KPEbnk = _T("");
	KPEbsg = 0;
	KPEbdm = 0;
	KPEstn = _T("");
	KPEssg = 0;
	KPEsdm = 0;
	KPEykn = _T("");
	KPEkoz = _T("");
	KPEyn1 = _T("");
	KPEyn2 = _T("");
	KPEsup = _T("");
	KPEslw = _T("");
	KPEaup = _T("");
	KPEalw = _T("");
	KPSsgn = 0;
	KPSsta = 0;
	KPSnam = _T("");
	KPTsck = 0;
	KPTsgn = 0;
	KPKsck = 0;
	KPKsgn = 0;
	KPSnen = 0;
	KPSndm = 0;
	KPSpn1 = _T("");
	KPSpn2 = _T("");
	KPSpn3 = _T("");
	KPSpn4 = _T("");
	KPSpn5 = _T("");
	KPSpn6 = _T("");
}

int CREFUNDVOL::Update()
{
	try{
		st = m_prefundvol->Update();
	}
	catch( CDBException* dbe ){
		if( dbe->m_nRetCode == AFX_SQL_ERROR_NO_ROWS_AFFECTED ) {
			dbe->Delete();
			m_prefundvol->Requery();
			if( m_prefundvol->IsBOF() ){
				nodata = 1;
			}
			else{
				nodata = 0;
			}
			return dbe->m_nRetCode;
		}
		m_prefundvol->m_pDatabase->Rollback();
		if( m_prefundvol ){
			ErrMsg = dbe->m_strError.GetBuffer();
		}
		dbe->Delete();
		return -1;
	}

	m_prefundvol->m_pDatabase->CommitTrans();
	
	return st;
}

int CREFUNDVOL::Delete()
{
	if( ! nodata ) {
		m_prefundvol->m_pDatabase->BeginTrans();

		for(;;){
			try{
				m_prefundvol->Delete();
			}
			catch( CDBException* dbe ){
				if( dbe->m_nRetCode == AFX_SQL_ERROR_NO_ROWS_AFFECTED ) {
					dbe->Delete();
					m_prefundvol->Requery();
					if( m_prefundvol->IsBOF() ) {
						m_prefundvol->m_pDatabase->Rollback();
						return -1;
					}
					nodata=0;
					continue;
				}
				m_prefundvol->m_pDatabase->Rollback();
				if( m_prefundvol ){
					ErrMsg = dbe->m_strError.GetBuffer();
				}
				dbe->Delete();
				return -1;
			}
			break;
		}
		m_prefundvol->m_pDatabase->CommitTrans();
		m_prefundvol->MoveNext();
	}

	return 0;
}

int CREFUNDVOL::AllDelete()
{
	if( ! nodata ) {
	
		CString	Sql;
		Sql.Format( _T("delete from %s"), _T("refundvol") );

		m_prefundvol->m_pDatabase->BeginTrans();
		try{
			m_prefundvol->m_pDatabase->ExecuteSQL( Sql );
		}
		catch( CDBException* dbe ){
			m_prefundvol->m_pDatabase->Rollback();
			if( m_prefundvol ){
				ErrMsg = dbe->m_strError.GetBuffer();
			}
			dbe->Delete();
			return -1;
		}
		m_prefundvol->m_pDatabase->CommitTrans();

	}
	return 0;
}
