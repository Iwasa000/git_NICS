#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSh_lst_rate

IMPLEMENT_DYNAMIC(CSh_lst_rate, CRecordset)

CSh_lst_rate::CSh_lst_rate( CDatabase* pdb )
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CSh_lst_rate)

	sh.sn_seq = 0;
	sh.version = 0;
	sh.hyoId = _T("");
	sh.itmId = _T("");
	sh.rate = 0;
	sh.sign1 = 0;

	m_nFields = 6;
	//}}AFX_FIELD_INIT
}

void CSh_lst_rate::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CSh_lst_rate)
	pFX->SetFieldType(CFieldExchange::outputColumn);

	RFX_Int( pFX,  _T("sn_seq"), sh.sn_seq );
	RFX_Int( pFX,  _T("version"), sh.version );
	RFX_Text( pFX, _T("hyoId"), sh.hyoId );
	RFX_Text( pFX, _T("itmId"), sh.itmId );
	RFX_Int( pFX,  _T("rate"), sh.rate );
	RFX_Int( pFX,  _T("sign1"), sh.sign1 );

	//}}AFX_FIELD_MAP
}				  
				  
CString CSh_lst_rate::GetDefaultSQL()
{				  
	return SH_LST_RATE_TBL;
}

/////////////////////////////////////////////////////////////////////////////
// CSh_lst_rate ƒNƒ‰ƒX‚Ìf’f

#ifdef _DEBUG
void CSh_lst_rate::AssertValid() const
{
	CRecordset::AssertValid();
}

void CSh_lst_rate::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSH_LST_RATE
CSH_LST_RATE::CSH_LST_RATE( CSh_lst_rate *pSh_lst_val )
{
	m_pSh_lst_rate = pSh_lst_val;
	nodata = 0;
}

void CSH_LST_RATE::data()
{
	if( sn_seq == -1 ){
		st = -1;
		version = 0;
		hyoId = _T("");
		itmId = _T("");
		rate = 0;
		sign1 = 0;
		return;
	}
	st = 0;
}

int CSH_LST_RATE::MoveFirst()
{
	sn_seq = -1;
	if( !nodata ){
		m_pSh_lst_rate->MoveFirst();
	}
	data();
	return st;
}

int CSH_LST_RATE::MoveNext()
{
	sn_seq = -1;
	if( ! m_pSh_lst_rate->IsEOF() ){
		m_pSh_lst_rate->MoveNext();
	}
	data();
	return st;
}

int CSH_LST_RATE::MovePrev()
{
	sn_seq = -1;
	if( ! m_pSh_lst_rate->IsBOF() ){
		m_pSh_lst_rate->MovePrev();
	}
	data();
	return st;
}

int CSH_LST_RATE::MoveLast()
{
	sn_seq = -1;
	if( ! nodata ){
		m_pSh_lst_rate->MoveLast();
	}
	data();
	return st;
}

int CSH_LST_RATE::Move( int nrows )
{
	sn_seq = -1;
	if( nrows > 0 ) {
		if( ! m_pSh_lst_rate->IsEOF() ){
			m_pSh_lst_rate->Move(nrows);
		}
	}
	else if( nrows < 0 ) {
		if( ! m_pSh_lst_rate->IsBOF() ){
			m_pSh_lst_rate->Move(nrows);
		}
	}
	else{
		m_pSh_lst_rate->Move(nrows);
	}
	data();
	return st;
}

int CSH_LST_RATE::SetAbsolutePosition( int nrows )
{
	sn_seq = -1;
	if( ! nodata ){
		m_pSh_lst_rate->SetAbsolutePosition(nrows);
	}
	data();
	return st;
}

int CSH_LST_RATE::Requery( CString filter/*=_T("")*/ )
{
	sn_seq = -1;
	m_pSh_lst_rate->m_strFilter = filter;
	m_pSh_lst_rate->m_strSort =  SH_LST_RATE_SORT;
	m_pSh_lst_rate->Requery();
	if( m_pSh_lst_rate->IsBOF() ){
		nodata = 1;
	}
	else{
		nodata = 0;
	}
	data();
	return st;
}

int CSH_LST_RATE::GetRecordCount()
{
	CRecordsetStatus	rstat;
	long	cpos;

	m_pSh_lst_rate->GetStatus( rstat );
	if( rstat.m_bRecordCountFinal ){
		return( m_pSh_lst_rate->GetRecordCount() );
	}
	else {
		if( (cpos=rstat.m_lCurrentRecord) < 0 ){
			m_pSh_lst_rate->MoveFirst();
		}
		for( ; ! m_pSh_lst_rate->IsEOF(); ){
			m_pSh_lst_rate->MoveNext();
		}
		if( cpos >= 0 ){
			m_pSh_lst_rate->SetAbsolutePosition(cpos+1);
		}
		return( m_pSh_lst_rate->GetRecordCount() );
	}
}

void CSH_LST_RATE::Edit()
{
	m_pSh_lst_rate->m_pDatabase->BeginTrans();
	m_pSh_lst_rate->Edit();
}

void CSH_LST_RATE::AddNew()
{
	m_pSh_lst_rate->m_pDatabase->BeginTrans();
	m_pSh_lst_rate->AddNew();
	sn_seq = 0;
	version = 0;
	hyoId = _T("");
	itmId = _T("");
	rate = 0;
	sign1 = 0;
}

int CSH_LST_RATE::Update()
{
	try{
		st = m_pSh_lst_rate->Update();
	}
	catch( CDBException* dbe ){
		if( dbe->m_nRetCode == AFX_SQL_ERROR_NO_ROWS_AFFECTED ) {
			dbe->Delete();
			m_pSh_lst_rate->Requery();
			if( m_pSh_lst_rate->IsBOF() ){
				nodata = 1;
			}
			else{
				nodata = 0;
			}
			return dbe->m_nRetCode;
		}
		m_pSh_lst_rate->m_pDatabase->Rollback();
		if( m_pSh_lst_rate ){
//			ErrMsg = dbe->m_strError.GetBuffer();
		}
		dbe->Delete();
		return -1;
	}

	m_pSh_lst_rate->m_pDatabase->CommitTrans();
	
	return st;
}

int CSH_LST_RATE::Delete()
{
	if( ! nodata ) {
		m_pSh_lst_rate->m_pDatabase->BeginTrans();

		for(;;){
			try{
				m_pSh_lst_rate->Delete();
			}
			catch( CDBException* dbe ){
				if( dbe->m_nRetCode == AFX_SQL_ERROR_NO_ROWS_AFFECTED ) {
					dbe->Delete();
					m_pSh_lst_rate->Requery();
					if( m_pSh_lst_rate->IsBOF() ) {
						m_pSh_lst_rate->m_pDatabase->Rollback();
						return -1;
					}
					nodata=0;
					continue;
				}
				m_pSh_lst_rate->m_pDatabase->Rollback();
				if( m_pSh_lst_rate ){
//					ErrMsg = dbe->m_strError.GetBuffer();
				}
				dbe->Delete();
				return -1;
			}
			break;
		}
		m_pSh_lst_rate->m_pDatabase->CommitTrans();
		m_pSh_lst_rate->MoveNext();
	}

	return 0;
}

/*int CSH_LST_RATE::AllDelete()
{
	if( ! nodata ) {
	
		CString	Sql;
		Sql.Format( _T("delete from %s"), _T("refundrec") );

		m_pSh_lst_rate->m_pDatabase->BeginTrans();
		try{
			m_pSh_lst_rate->m_pDatabase->ExecuteSQL( Sql );
		}
		catch( CDBException* dbe ){
			m_pSh_lst_rate->m_pDatabase->Rollback();
			if( m_pSh_lst_rate ){
				ErrMsg = dbe->m_strError.GetBuffer();
			}
			dbe->Delete();
			return -1;
		}
		m_pSh_lst_rate->m_pDatabase->CommitTrans();

	}
	return 0;
}*/
