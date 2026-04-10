// 
#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void AFXAPI RFX_Bigint(CFieldExchange* pFX, LPCTSTR szName, __int64& value);

/////////////////////////////////////////////////////////////////////////////
// CPayment

IMPLEMENT_DYNAMIC(CPayment, CRecordset)

CPayment::CPayment( CDatabase* pdb )
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CPayment)

	pr.NFItem = 0;
	pr.NFLine = 0;
	pr.NFResv = 0;
	pr.NFSequ = 0;
	pr.NFNend = 0;
	pr.NFZeim =  _T("");
	pr.NFZmnm =  _T("");
	pr.NFZmno =  _T("");
	pr.NFSrno =  _T("");
	pr.NFNsdy = 0;
	pr.NFNedy = 0;
	pr.NFNsgn = 0;
	pr.NFNsdm = 0;
	pr.NFHval =  _T("");
	pr.NFJval =  _T("");
	pr.NFKsgn = 0;
	pr.NFKsdm = 0;
	pr.NFKval = _T("");
	pr.NFRval = _T("");
	pr.NFEval = _T("");
	pr.NFGval = _T("");

	m_nFields = 21;
	//}}AFX_FIELD_INIT
}

void CPayment::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CPayment)
	pFX->SetFieldType(CFieldExchange::outputColumn);

	RFX_Int( pFX, _T("NFItem"), pr.NFItem );
	RFX_Int( pFX, _T("NFLine"), pr.NFLine );
	RFX_Int( pFX, _T("NFResv"), pr.NFResv );
	RFX_Int( pFX, _T("NFSequ"), pr.NFSequ );
	RFX_Int( pFX, _T("NFNend"), pr.NFNend );
	RFX_Text( pFX,_T("NFZeim"), pr.NFZeim );
	RFX_Text( pFX,_T("NFZmnm"), pr.NFZmnm ); 
	RFX_Text( pFX,_T("NFZmno"), pr.NFZmno ); 
	RFX_Text( pFX,_T("NFSrno"), pr.NFSrno ); 
	RFX_Int( pFX, _T("NFNsdy"), pr.NFNsdy ); 
	RFX_Int( pFX, _T("NFNedy"), pr.NFNedy ); 
	RFX_Byte( pFX,_T("NFNsgn"), pr.NFNsgn ); 
	RFX_Byte( pFX,_T("NFNsdm"), pr.NFNsdm ); 
	RFX_Text( pFX,_T("NFHval"), pr.NFHval, 16, SQL_DECIMAL ); 
	RFX_Text( pFX,_T("NFJval"), pr.NFJval, 16, SQL_DECIMAL ); 
	RFX_Byte( pFX,_T("NFKsgn"), pr.NFKsgn ); 
	RFX_Byte( pFX,_T("NFKsdm"), pr.NFKsdm ); 
	RFX_Text( pFX,_T("NFKval"), pr.NFKval, 16, SQL_DECIMAL ); 
	RFX_Text( pFX,_T("NFRval"), pr.NFRval, 16, SQL_DECIMAL ); 
	RFX_Text( pFX,_T("NFEval"), pr.NFEval, 16, SQL_DECIMAL ); 
	RFX_Text( pFX,_T("NFGval"), pr.NFGval, 16, SQL_DECIMAL ); 

	//}}AFX_FIELD_MAP
}				  
				  
CString CPayment::GetDefaultSQL()
{				  
	return _T("payment");
}

/////////////////////////////////////////////////////////////////////////////
// CPrfkin ƒNƒ‰ƒX‚Ìf’f

#ifdef _DEBUG
void CPayment::AssertValid() const
{
	CRecordset::AssertValid();
}

void CPayment::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CKJSNK
CPAYMENT::CPAYMENT( CPayment *ppayment )
{
	m_ppayment = ppayment;
	nodata = 0;
}

void CPAYMENT::data()
{
	if( NFItem == -1 ){
		st = -1;
		NFItem = 0;
		NFLine = 0;
		NFResv = 0;
		NFSequ = 0;
		NFNend = 0;
		NFZeim =  _T("");
		NFZmnm =  _T("");
		NFZmno =  _T("");
		NFSrno =  _T("");
		NFNsdy = 0;
		NFNedy = 0;
		NFNsgn = 0;
		NFNsdm = 0;
		NFHval =  _T("");
		NFJval =  _T("");
		NFKsgn = 0;
		NFKsdm = 0;
		NFKval = _T("");
		NFRval = _T("");
		NFEval = _T("");
		NFGval = _T("");
		return;
	}
	st = 0;
}

int CPAYMENT::MoveFirst()
{
	NFItem = -1;
	if( !nodata ){
		m_ppayment->MoveFirst();
	}
	data();
	return st;
}

int CPAYMENT::MoveNext()
{
	NFItem = -1;
	if( ! m_ppayment->IsEOF() ){
		m_ppayment->MoveNext();
	}
	data();
	return st;
}

int CPAYMENT::MovePrev()
{
	NFItem = -1;
	if( ! m_ppayment->IsBOF() ){
		m_ppayment->MovePrev();
	}
	data();
	return st;
}

int CPAYMENT::MoveLast()
{
	NFItem = -1;
	if( ! nodata ){
		m_ppayment->MoveLast();
	}
	data();
	return st;
}

int CPAYMENT::Move( int nrows )
{
	NFItem = -1;
	if( nrows > 0 ) {
		if( ! m_ppayment->IsEOF() ){
			m_ppayment->Move(nrows);
		}
	}
	else if( nrows < 0 ) {
		if( ! m_ppayment->IsBOF() ){
			m_ppayment->Move(nrows);
		}
	}
	else{
		m_ppayment->Move(nrows);
	}
	data();
	return st;
}

int CPAYMENT::SetAbsolutePosition( int nrows )
{
	NFItem = -1;
	if( ! nodata ){
		m_ppayment->SetAbsolutePosition(nrows);
	}
	data();
	return st;
}

int CPAYMENT::Requery( LPCTSTR filter )
{
	NFItem = -1;
	m_ppayment->m_strFilter = filter;
	m_ppayment->m_strSort =  _T("NFItem,NFLine,NFResv,NFSequ");
	m_ppayment->Requery();
	if( m_ppayment->IsBOF() ){
		nodata = 1;
	}
	else{
		nodata = 0;
	}
	data();
	return st;
}

int CPAYMENT::GetRecordCount()
{
	CRecordsetStatus	rstat;
	long	cpos;

	m_ppayment->GetStatus( rstat );
	if( rstat.m_bRecordCountFinal ){
		return( m_ppayment->GetRecordCount() );
	}
	else {
		if( (cpos=rstat.m_lCurrentRecord) < 0 ){
			m_ppayment->MoveFirst();
		}
		for( ; ! m_ppayment->IsEOF(); ){
			m_ppayment->MoveNext();
		}
		if( cpos >= 0 ){
			m_ppayment->SetAbsolutePosition(cpos+1);
		}
		return( m_ppayment->GetRecordCount() );
	}
}

void CPAYMENT::Edit()
{
	m_ppayment->m_pDatabase->BeginTrans();
	m_ppayment->Edit();
}

void CPAYMENT::AddNew()
{
	m_ppayment->m_pDatabase->BeginTrans();
	m_ppayment->AddNew();
	NFItem = 0;
	NFLine = 0;
	NFResv = 0;
	NFSequ = 0;
	NFNend = 0;
	NFZeim =  _T("");
	NFZmnm =  _T("");
	NFZmno =  _T("");
	NFSrno =  _T("");
	NFNsdy = 0;
	NFNedy = 0;
	NFNsgn = 0;
	NFNsdm = 0;
	NFHval =  _T("");
	NFJval =  _T("");
	NFKsgn = 0;
	NFKsdm = 0;
	NFKval = _T("");
	NFRval = _T("");
	NFEval = _T("");
	NFGval = _T("");
}

int CPAYMENT::Update()
{
	try{
		st = m_ppayment->Update();
	}
	catch( CDBException* dbe ){
		if( dbe->m_nRetCode == AFX_SQL_ERROR_NO_ROWS_AFFECTED ) {
			dbe->Delete();
			m_ppayment->Requery();
			if( m_ppayment->IsBOF() ){
				nodata = 1;
			}
			else{
				nodata = 0;
			}
			return dbe->m_nRetCode;
		}
		m_ppayment->m_pDatabase->Rollback();
		if( m_ppayment ){
			ErrMsg = dbe->m_strError.GetBuffer();
		}
		dbe->Delete();
		return -1;
	}

	m_ppayment->m_pDatabase->CommitTrans();
	
	return st;
}

int CPAYMENT::Delete()
{
	if( ! nodata ) {
		m_ppayment->m_pDatabase->BeginTrans();

		for(;;){
			try{
				m_ppayment->Delete();
			}
			catch( CDBException* dbe ){
				if( dbe->m_nRetCode == AFX_SQL_ERROR_NO_ROWS_AFFECTED ) {
					dbe->Delete();
					m_ppayment->Requery();
					if( m_ppayment->IsBOF() ) {
						m_ppayment->m_pDatabase->Rollback();
						return -1;
					}
					nodata=0;
					continue;
				}
				m_ppayment->m_pDatabase->Rollback();
				if( m_ppayment ){
					ErrMsg = dbe->m_strError.GetBuffer();
				}
				dbe->Delete();
				return -1;
			}
			break;
		}
		m_ppayment->m_pDatabase->CommitTrans();
		m_ppayment->MoveNext();
	}

	return 0;
}

int CPAYMENT::AllDelete()
{
	if( ! nodata ) {
	
		CString	Sql;
		Sql.Format( _T("delete from %s"), _T("payment") );

		m_ppayment->m_pDatabase->BeginTrans();
		try{
			m_ppayment->m_pDatabase->ExecuteSQL( Sql );
		}
		catch( CDBException* dbe ){
			m_ppayment->m_pDatabase->Rollback();
			if( m_ppayment ){
				ErrMsg = dbe->m_strError.GetBuffer();
			}
			dbe->Delete();
			return -1;
		}
		m_ppayment->m_pDatabase->CommitTrans();

	}
	return 0;
}
