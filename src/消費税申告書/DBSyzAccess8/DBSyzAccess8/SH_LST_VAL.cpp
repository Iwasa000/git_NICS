// 
#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSh_lst_val

IMPLEMENT_DYNAMIC(CSh_lst_val, CRecordset)

CSh_lst_val::CSh_lst_val( CDatabase* pdb )
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CSh_lst_val)

	sh.sn_seq = 0;
	sh.version = 0;
	sh.hyoId = _T("");
	sh.itmId = _T("");
	sh.val = _T("");
	sh.sign1 = 0;
	sh.ksname = _T("");		// 230718Aw add
	sh.kssgn1 = 0;			// 230718Aw add

	//m_nFields = 6;		// 230718Aw del
	m_nFields = 8;			// 230718Aw add
	//}}AFX_FIELD_INIT
}

void CSh_lst_val::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CSh_lst_val)
	pFX->SetFieldType(CFieldExchange::outputColumn);

	RFX_Int( pFX,  _T("sn_seq"), sh.sn_seq );
	RFX_Int( pFX,  _T("version"), sh.version );
	RFX_Text( pFX, _T("hyoId"), sh.hyoId );
	RFX_Text( pFX, _T("itmId"), sh.itmId );
	RFX_Text( pFX, _T("val"), sh.val, 16, SQL_DECIMAL );
	RFX_Int( pFX,  _T("sign1"), sh.sign1 );
	RFX_Text( pFX, _T("ksname"), sh.ksname );	// 230718Aw add
	RFX_Int( pFX,  _T("kssgn1"), sh.kssgn1 );	// 230718Aw add

	//}}AFX_FIELD_MAP
}				  
				  
CString CSh_lst_val::GetDefaultSQL()
{				  
	return SH_LST_VAL_TBL;
}

/////////////////////////////////////////////////////////////////////////////
// CSh_lst_val ƒNƒ‰ƒX‚Ìf’f

#ifdef _DEBUG
void CSh_lst_val::AssertValid() const
{
	CRecordset::AssertValid();
}

void CSh_lst_val::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSH_LST_VAL
CSH_LST_VAL::CSH_LST_VAL( CSh_lst_val *pSh_lst_val )
{
	m_pSh_lst_val = pSh_lst_val;
	nodata = 0;
}

void CSH_LST_VAL::data()
{
	if( sn_seq == -1 ){
		st = -1;
		version = 0;
		hyoId = _T("");
		itmId = _T("");
		val = _T("");
		sign1 = 0;
		ksname = _T("");	// 230718Aw add
		kssgn1 = 0;			// 230718Aw add
		return;
	}
	st = 0;
}

int CSH_LST_VAL::MoveFirst()
{
	sn_seq = -1;
	if( !nodata ){
		m_pSh_lst_val->MoveFirst();
	}
	data();
	return st;
}

int CSH_LST_VAL::MoveNext()
{
	sn_seq = -1;
	if( ! m_pSh_lst_val->IsEOF() ){
		m_pSh_lst_val->MoveNext();
	}
	data();
	return st;
}

int CSH_LST_VAL::MovePrev()
{
	sn_seq = -1;
	if( ! m_pSh_lst_val->IsBOF() ){
		m_pSh_lst_val->MovePrev();
	}
	data();
	return st;
}

int CSH_LST_VAL::MoveLast()
{
	sn_seq = -1;
	if( ! nodata ){
		m_pSh_lst_val->MoveLast();
	}
	data();
	return st;
}

int CSH_LST_VAL::Move( int nrows )
{
	sn_seq = -1;
	if( nrows > 0 ) {
		if( ! m_pSh_lst_val->IsEOF() ){
			m_pSh_lst_val->Move(nrows);
		}
	}
	else if( nrows < 0 ) {
		if( ! m_pSh_lst_val->IsBOF() ){
			m_pSh_lst_val->Move(nrows);
		}
	}
	else{
		m_pSh_lst_val->Move(nrows);
	}
	data();
	return st;
}

int CSH_LST_VAL::SetAbsolutePosition( int nrows )
{
	sn_seq = -1;
	if( ! nodata ){
		m_pSh_lst_val->SetAbsolutePosition(nrows);
	}
	data();
	return st;
}

int CSH_LST_VAL::Requery( CString filter/*=_T("")*/ )
{
	sn_seq = -1;
	m_pSh_lst_val->m_strFilter = filter;
	m_pSh_lst_val->m_strSort =  SH_LST_VAL_SORT;
	m_pSh_lst_val->Requery();
	if( m_pSh_lst_val->IsBOF() ){
		nodata = 1;
	}
	else{
		nodata = 0;
	}
	data();
	return st;
}

int CSH_LST_VAL::GetRecordCount()
{
	CRecordsetStatus	rstat;
	long	cpos;

	m_pSh_lst_val->GetStatus( rstat );
	if( rstat.m_bRecordCountFinal ){
		return( m_pSh_lst_val->GetRecordCount() );
	}
	else {
		if( (cpos=rstat.m_lCurrentRecord) < 0 ){
			m_pSh_lst_val->MoveFirst();
		}
		for( ; ! m_pSh_lst_val->IsEOF(); ){
			m_pSh_lst_val->MoveNext();
		}
		if( cpos >= 0 ){
			m_pSh_lst_val->SetAbsolutePosition(cpos+1);
		}
		return( m_pSh_lst_val->GetRecordCount() );
	}
}

void CSH_LST_VAL::Edit()
{
	m_pSh_lst_val->m_pDatabase->BeginTrans();
	m_pSh_lst_val->Edit();
}

void CSH_LST_VAL::AddNew()
{
	m_pSh_lst_val->m_pDatabase->BeginTrans();
	m_pSh_lst_val->AddNew();
	sn_seq = 0;
	version = 0;
	hyoId = _T("");
	itmId = _T("");
	val = _T("");
	sign1 = 0;
	ksname = _T("");	// 230718Aw add
	kssgn1 = 0;			// 230718Aw add
}

int CSH_LST_VAL::Update()
{
	try{
		st = m_pSh_lst_val->Update();
	}
	catch( CDBException* dbe ){
		if( dbe->m_nRetCode == AFX_SQL_ERROR_NO_ROWS_AFFECTED ) {
			dbe->Delete();
			m_pSh_lst_val->Requery();
			if( m_pSh_lst_val->IsBOF() ){
				nodata = 1;
			}
			else{
				nodata = 0;
			}
			return dbe->m_nRetCode;
		}
		m_pSh_lst_val->m_pDatabase->Rollback();
		if( m_pSh_lst_val ){
//			ErrMsg = dbe->m_strError.GetBuffer();
		}
		dbe->Delete();
		return -1;
	}

	m_pSh_lst_val->m_pDatabase->CommitTrans();
	
	return st;
}

int CSH_LST_VAL::Delete()
{
	if( ! nodata ) {
		m_pSh_lst_val->m_pDatabase->BeginTrans();

		for(;;){
			try{
				m_pSh_lst_val->Delete();
			}
			catch( CDBException* dbe ){
				if( dbe->m_nRetCode == AFX_SQL_ERROR_NO_ROWS_AFFECTED ) {
					dbe->Delete();
					m_pSh_lst_val->Requery();
					if( m_pSh_lst_val->IsBOF() ) {
						m_pSh_lst_val->m_pDatabase->Rollback();
						return -1;
					}
					nodata=0;
					continue;
				}
				m_pSh_lst_val->m_pDatabase->Rollback();
				if( m_pSh_lst_val ){
//					ErrMsg = dbe->m_strError.GetBuffer();
				}
				dbe->Delete();
				return -1;
			}
			break;
		}
		m_pSh_lst_val->m_pDatabase->CommitTrans();
		m_pSh_lst_val->MoveNext();
	}

	return 0;
}

/*int CSH_LST_VAL::AllDelete()
{
	if( ! nodata ) {
	
		CString	Sql;
		Sql.Format( _T("delete from %s"), _T("refundrec") );

		m_pSh_lst_val->m_pDatabase->BeginTrans();
		try{
			m_pSh_lst_val->m_pDatabase->ExecuteSQL( Sql );
		}
		catch( CDBException* dbe ){
			m_pSh_lst_val->m_pDatabase->Rollback();
			if( m_pSh_lst_val ){
				ErrMsg = dbe->m_strError.GetBuffer();
			}
			dbe->Delete();
			return -1;
		}
		m_pSh_lst_val->m_pDatabase->CommitTrans();

	}
	return 0;
}*/
