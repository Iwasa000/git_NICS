// 
#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void AFXAPI RFX_Bigint(CFieldExchange* pFX, LPCTSTR szName, __int64& value);

/////////////////////////////////////////////////////////////////////////////
// CShinPayment

IMPLEMENT_DYNAMIC(CShinPayment, CRecordset)

CShinPayment::CShinPayment( CDatabase* pdb )
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CShinPayment)

	pr.NFItem = 0;
	pr.NFLine = 0;
	pr.NFResv = 0;
	pr.NFSequ = 0;
	pr.NFKskb = 0;
	pr.NFKsdm = 0;
	pr.NFStnm =  _T("");
	pr.NFJflg = 0;
	pr.NFHval = _T("");
	pr.NFJval = _T("");
	pr.NFKval = _T("");
	pr.NFRval = _T("");
	pr.NFEval = _T("");
	pr.NFGval = _T("");
	pr.NFHjvl = _T("");
	pr.NFVal1 = _T("");
	pr.NFVal2 = _T("");
	pr.NFSign.SetSize(128);
	for( int i = 0; i != 128; i++ ) {
		pr.NFSign[i] = 0x00;
	}
	m_nFields = 18;
	//}}AFX_FIELD_INIT
}

void CShinPayment::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CShinPayment)
	pFX->SetFieldType(CFieldExchange::outputColumn);

	RFX_Int( pFX, _T("NFItem"), pr.NFItem );
	RFX_Int( pFX, _T("NFLine"), pr.NFLine );
	RFX_Int( pFX, _T("NFResv"), pr.NFResv );
	RFX_Int( pFX, _T("NFSequ"), pr.NFSequ );
	RFX_Byte( pFX,_T("NFKskb"), pr.NFKskb ); 
	RFX_Byte( pFX,_T("NFKsdm"), pr.NFKsdm ); 
	RFX_Text( pFX,_T("NFStnm"), pr.NFStnm );
	RFX_Int( pFX,_T("NFJflg"), pr.NFJflg ); 
	RFX_Text( pFX,_T("NFHval"), pr.NFHval, 16, SQL_DECIMAL ); 
	RFX_Text( pFX,_T("NFJval"), pr.NFJval, 16, SQL_DECIMAL ); 
	RFX_Text( pFX,_T("NFKval"), pr.NFKval, 16, SQL_DECIMAL ); 
	RFX_Text( pFX,_T("NFRval"), pr.NFRval, 16, SQL_DECIMAL ); 
	RFX_Text( pFX,_T("NFEval"), pr.NFEval, 16, SQL_DECIMAL ); 
	RFX_Text( pFX,_T("NFGval"), pr.NFGval, 16, SQL_DECIMAL ); 
	RFX_Text( pFX,_T("NFHjvl"), pr.NFHjvl, 16, SQL_DECIMAL ); 
	RFX_Text( pFX,_T("NFVal1"), pr.NFVal1, 16, SQL_DECIMAL ); 
	RFX_Text( pFX,_T("NFVal2"), pr.NFVal2, 16, SQL_DECIMAL ); 
	RFX_Binary( pFX, _T("NFSign"), (CByteArray&)pr.NFSign, 128 );

	//}}AFX_FIELD_MAP
}				  
				  
CString CShinPayment::GetDefaultSQL()
{				  
	return _T("shinpayment");
}

/////////////////////////////////////////////////////////////////////////////
// CPrfkin ƒNƒ‰ƒX‚Ìf’f

#ifdef _DEBUG
void CShinPayment::AssertValid() const
{
	CRecordset::AssertValid();
}

void CShinPayment::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CKJSNK
CSHINPAYMENT::CSHINPAYMENT( CShinPayment *pshinpayment )
{
	m_pshinpayment = pshinpayment;
	nodata = 0;
}

void CSHINPAYMENT::data()
{
	if( NFItem == -1 ){
		st = -1;
		NFItem = 0;
		NFLine = 0;
		NFResv = 0;
		NFSequ = 0;
		NFKskb = 0;
		NFKsdm = 0;
		NFStnm =  _T("");
		NFJflg = 0;
		NFHval = _T("");
		NFJval = _T("");
		NFKval = _T("");
		NFRval = _T("");
		NFEval = _T("");
		NFGval = _T("");
		NFHjvl = _T("");
		NFVal1 = _T("");
		NFVal2 = _T("");
		NFSign.SetSize(128);
		for( int i = 0; i != 128; i++ ) {
			NFSign[i] = 0x00;
		}
		return;
	}
	st = 0;
}

int CSHINPAYMENT::MoveFirst()
{
	NFItem = -1;
	if( !nodata ){
		m_pshinpayment->MoveFirst();
	}
	data();
	return st;
}

int CSHINPAYMENT::MoveNext()
{
	NFItem = -1;
	if( ! m_pshinpayment->IsEOF() ){
		m_pshinpayment->MoveNext();
	}
	data();
	return st;
}

int CSHINPAYMENT::MovePrev()
{
	NFItem = -1;
	if( ! m_pshinpayment->IsBOF() ){
		m_pshinpayment->MovePrev();
	}
	data();
	return st;
}

int CSHINPAYMENT::MoveLast()
{
	NFItem = -1;
	if( ! nodata ){
		m_pshinpayment->MoveLast();
	}
	data();
	return st;
}

int CSHINPAYMENT::Move( int nrows )
{
	NFItem = -1;
	if( nrows > 0 ) {
		if( ! m_pshinpayment->IsEOF() ){
			m_pshinpayment->Move(nrows);
		}
	}
	else if( nrows < 0 ) {
		if( ! m_pshinpayment->IsBOF() ){
			m_pshinpayment->Move(nrows);
		}
	}
	else{
		m_pshinpayment->Move(nrows);
	}
	data();
	return st;
}

int CSHINPAYMENT::SetAbsolutePosition( int nrows )
{
	NFItem = -1;
	if( ! nodata ){
		m_pshinpayment->SetAbsolutePosition(nrows);
	}
	data();
	return st;
}

int CSHINPAYMENT::Requery( LPCTSTR filter )
{
	NFItem = -1;
	m_pshinpayment->m_strFilter = filter;
	m_pshinpayment->m_strSort =  _T("NFItem,NFLine,NFResv,NFSequ");
	m_pshinpayment->Requery();
	if( m_pshinpayment->IsBOF() ){
		nodata = 1;
	}
	else{
		nodata = 0;
	}
	data();
	return st;
}

int CSHINPAYMENT::GetRecordCount()
{
	CRecordsetStatus	rstat;
	long	cpos;

	m_pshinpayment->GetStatus( rstat );
	if( rstat.m_bRecordCountFinal ){
		return( m_pshinpayment->GetRecordCount() );
	}
	else {
		if( (cpos=rstat.m_lCurrentRecord) < 0 ){
			m_pshinpayment->MoveFirst();
		}
		for( ; ! m_pshinpayment->IsEOF(); ){
			m_pshinpayment->MoveNext();
		}
		if( cpos >= 0 ){
			m_pshinpayment->SetAbsolutePosition(cpos+1);
		}
		return( m_pshinpayment->GetRecordCount() );
	}
}

void CSHINPAYMENT::Edit()
{
	m_pshinpayment->m_pDatabase->BeginTrans();
	m_pshinpayment->Edit();
}

void CSHINPAYMENT::AddNew()
{
	m_pshinpayment->m_pDatabase->BeginTrans();
	m_pshinpayment->AddNew();
	NFItem = 0;
	NFLine = 0;
	NFResv = 0;
	NFSequ = 0;
	NFKskb = 0;
	NFKsdm = 0;
	NFStnm =  _T("");
	NFJflg = 0;
	NFHval = _T("");
	NFJval = _T("");
	NFKval = _T("");
	NFRval = _T("");
	NFEval = _T("");
	NFGval = _T("");
	NFHjvl = _T("");
	NFVal1 = _T("");
	NFVal2 = _T("");
	NFSign.SetSize(128);
	for( int i = 0; i != 128; i++ ) {
		NFSign[i] = 0x00;
	}
}

int CSHINPAYMENT::Update()
{
	try{
		st = m_pshinpayment->Update();
	}
	catch( CDBException* dbe ){
		if( dbe->m_nRetCode == AFX_SQL_ERROR_NO_ROWS_AFFECTED ) {
			dbe->Delete();
			m_pshinpayment->Requery();
			if( m_pshinpayment->IsBOF() ){
				nodata = 1;
			}
			else{
				nodata = 0;
			}
			return dbe->m_nRetCode;
		}
		m_pshinpayment->m_pDatabase->Rollback();
		if( m_pshinpayment ){
			ErrMsg = dbe->m_strError.GetBuffer();
		}
		dbe->Delete();
		return -1;
	}

	m_pshinpayment->m_pDatabase->CommitTrans();
	
	return st;
}

int CSHINPAYMENT::Delete()
{
	if( ! nodata ) {
		m_pshinpayment->m_pDatabase->BeginTrans();

		for(;;){
			try{
				m_pshinpayment->Delete();
			}
			catch( CDBException* dbe ){
				if( dbe->m_nRetCode == AFX_SQL_ERROR_NO_ROWS_AFFECTED ) {
					dbe->Delete();
					m_pshinpayment->Requery();
					if( m_pshinpayment->IsBOF() ) {
						m_pshinpayment->m_pDatabase->Rollback();
						return -1;
					}
					nodata=0;
					continue;
				}
				m_pshinpayment->m_pDatabase->Rollback();
				if( m_pshinpayment ){
					ErrMsg = dbe->m_strError.GetBuffer();
				}
				dbe->Delete();
				return -1;
			}
			break;
		}
		m_pshinpayment->m_pDatabase->CommitTrans();
		m_pshinpayment->MoveNext();
	}

	return 0;
}

int CSHINPAYMENT::AllDelete()
{
	if( ! nodata ) {
	
		CString	Sql;
		Sql.Format( _T("delete from %s"), _T("shinpayment") );

		m_pshinpayment->m_pDatabase->BeginTrans();
		try{
			m_pshinpayment->m_pDatabase->ExecuteSQL( Sql );
		}
		catch( CDBException* dbe ){
			m_pshinpayment->m_pDatabase->Rollback();
			if( m_pshinpayment ){
				ErrMsg = dbe->m_strError.GetBuffer();
			}
			dbe->Delete();
			return -1;
		}
		m_pshinpayment->m_pDatabase->CommitTrans();

	}
	return 0;
}
