// \‘@‰ÛÅŽæˆø‹àŠzŒvŽZ•\
#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void AFXAPI RFX_Bigint(CFieldExchange* pFX, LPCTSTR szName, __int64& value);

/////////////////////////////////////////////////////////////////////////////
// CPrfkin

IMPLEMENT_DYNAMIC(CKjsnk, CRecordset)

CKjsnk::CKjsnk( CDatabase* pdb )
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CPrfkin)
	pr.kslist = 0;		// ŒvŽZ•\@”Ô†
	pr.ksline = 0;		// s”Ô†
	pr.ksitem = 0;		// —ñ”Ô†
	pr.kssgn1 = 0x00;	// WŒvƒTƒCƒ“
	pr.kssgn2 = 0x00;	// ‹àŠzƒTƒCƒ“
	pr.kssgn3 = 0x00;	// ŽÀŠz“ü—ÍƒTƒCƒ“
	pr.kssgn4 = 0x00;	// €–ÚƒTƒCƒ“
	pr.kssgn5 = 0x00;	// ˆ—ƒTƒCƒ“
	pr.ksname = "";		// €–Ú–¼
	pr.ksval0 = "";		// ‹àŠz‚P
	pr.ksval1 = "";		// ‹àŠz‚Q
	pr.ksval2 = "";		// ‹àŠz‚R
	pr.ksval3 = "";		// ‹àŠz‚S
	pr.ksval4 = "";		// ‹àŠz‚T
	pr.ksval5 = "";		// ‹àŠz‚U
	m_nFields = 15;
	//}}AFX_FIELD_INIT

//	m_pCDBPrtblW = NULL;
}


void CKjsnk::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CKjsnk)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Int( pFX, _T("kslist"), pr.kslist );
	RFX_Int( pFX, _T("ksline"), pr.ksline );
	RFX_Int( pFX, _T("ksitem"), pr.ksitem );
	RFX_Byte( pFX, _T("kssgn1"), pr.kssgn1 );
	RFX_Byte( pFX, _T("kssgn2"), pr.kssgn2 );
	RFX_Byte( pFX, _T("kssgn3"), pr.kssgn3 );
	RFX_Byte( pFX, _T("kssgn4"), pr.kssgn4 );
	RFX_Byte( pFX, _T("kssgn5"), pr.kssgn5 );

	RFX_Text( pFX, _T("ksname"), pr.ksname );
	RFX_Text( pFX, _T("ksval0"), pr.ksval0, 16, SQL_DECIMAL );
	RFX_Text( pFX, _T("ksval1"), pr.ksval1, 16, SQL_DECIMAL );
	RFX_Text( pFX, _T("ksval2"), pr.ksval2, 16, SQL_DECIMAL );
	RFX_Text( pFX, _T("ksval3"), pr.ksval3, 16, SQL_DECIMAL );
	RFX_Text( pFX, _T("ksval4"), pr.ksval4, 16, SQL_DECIMAL );
	RFX_Text( pFX, _T("ksval5"), pr.ksval5, 16, SQL_DECIMAL );
	//}}AFX_FIELD_MAP
}				  
				  
CString CKjsnk::GetDefaultSQL()
{				  
	return _T("kjsnk");
}

/////////////////////////////////////////////////////////////////////////////
// CPrfkin ƒNƒ‰ƒX‚Ìf’f

#ifdef _DEBUG
void CKjsnk::AssertValid() const
{
	CRecordset::AssertValid();
}

void CKjsnk::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CKJSNK
CKJSNK::CKJSNK( CKjsnk *pkjsnk )
{
	m_pkjsnk = pkjsnk;
	nodata = 0;
}

void CKJSNK::data()
{
	if( kslist == -1 ){	// ŒvŽZ•\@”Ô†
		st = -1;
		ksline = 0;		// s”Ô†
		ksitem = 0;		// —ñ”Ô†
		kssgn1 = 0x00;	// WŒvƒTƒCƒ“
		kssgn2 = 0x00;	// ‹àŠzƒTƒCƒ“
		kssgn3 = 0x00;	// ŽÀŠz“ü—ÍƒTƒCƒ“
		kssgn4 = 0x00;	// €–ÚƒTƒCƒ“
		kssgn5 = 0x00;	// ˆ—ƒTƒCƒ“
		ksname = "";	// €–Ú–¼
		ksval0 = "";	// ‹àŠz‚P
		ksval1 = "";	// ‹àŠz‚Q
		ksval2 = "";	// ‹àŠz‚R
		ksval3 = "";	// ‹àŠz‚S
		ksval4 = "";	// ‹àŠz‚T
		ksval5 = "";	// ‹àŠz‚U
		return;
	}
	st = 0;
}

int CKJSNK::MoveFirst()
{
	kslist = -1;
	if( !nodata ){
		m_pkjsnk->MoveFirst();
	}
	data();
	return st;
}

int CKJSNK::MoveNext()
{
	kslist = -1;
	if( ! m_pkjsnk->IsEOF() ){
		m_pkjsnk->MoveNext();
	}
	data();
	return st;
}

int CKJSNK::MovePrev()
{
	kslist = -1;
	if( ! m_pkjsnk->IsBOF() ){
		m_pkjsnk->MovePrev();
	}
	data();
	return st;
}

int CKJSNK::MoveLast()
{
	kslist = -1;
	if( ! nodata ){
		m_pkjsnk->MoveLast();
	}
	data();
	return st;
}

int CKJSNK::Move( int nrows )
{
	kslist = -1;
	if( nrows > 0 ) {
		if( ! m_pkjsnk->IsEOF() ){
			m_pkjsnk->Move(nrows);
		}
	}
	else if( nrows < 0 ) {
		if( ! m_pkjsnk->IsBOF() ){
			m_pkjsnk->Move(nrows);
		}
	}
	else{
		m_pkjsnk->Move(nrows);
	}
	data();
	return st;
}

int CKJSNK::SetAbsolutePosition( int nrows )
{
	kslist = -1;
	if( ! nodata ){
		m_pkjsnk->SetAbsolutePosition(nrows);
	}
	data();
	return st;
}

int CKJSNK::Requery( CString filter )
{
	kslist = -1;
	m_pkjsnk->m_strFilter = filter;
	m_pkjsnk->m_strSort =  _T("kslist,ksline,ksitem");
	m_pkjsnk->Requery();
	if( m_pkjsnk->IsBOF() ){
		nodata = 1;
	}
	else{
		nodata = 0;
	}
	data();
	return st;
}

int CKJSNK::GetRecordCount()
{
	CRecordsetStatus	rstat;
	long	cpos;

	m_pkjsnk->GetStatus( rstat );
	if( rstat.m_bRecordCountFinal ){
		return( m_pkjsnk->GetRecordCount() );
	}
	else {
		if( (cpos=rstat.m_lCurrentRecord) < 0 ){
			m_pkjsnk->MoveFirst();
		}
		for( ; ! m_pkjsnk->IsEOF(); ){
			m_pkjsnk->MoveNext();
		}
		if( cpos >= 0 ){
			m_pkjsnk->SetAbsolutePosition(cpos+1);
		}
		return( m_pkjsnk->GetRecordCount() );
	}
}

void CKJSNK::Edit()
{
	m_pkjsnk->m_pDatabase->BeginTrans();
	m_pkjsnk->Edit();
}

void CKJSNK::AddNew()
{
	m_pkjsnk->m_pDatabase->BeginTrans();
	m_pkjsnk->AddNew();
}

int CKJSNK::Update()
{
	try{
		st = m_pkjsnk->Update();
	}
	catch( CDBException* dbe ){
		if( dbe->m_nRetCode == AFX_SQL_ERROR_NO_ROWS_AFFECTED ) {
			dbe->Delete();
			m_pkjsnk->Requery();
			if( m_pkjsnk->IsBOF() ){
				nodata = 1;
			}
			else{
				nodata = 0;
			}
			return dbe->m_nRetCode;
		}
		m_pkjsnk->m_pDatabase->Rollback();
//		if( m_pkjsnk->m_pCDBPrtblW ){
//			m_pkjsnk->m_pCDBPrtblW->SetErrMsg( DB_FATAL_ERROR, dbe->m_strError.GetBuffer(), dbe->m_nRetCode );
//		}
		dbe->Delete();
		return -1;
	}

	m_pkjsnk->m_pDatabase->CommitTrans();
	
	return st;
}

void CKJSNK::Delete()
{
	if( ! nodata ) {
		m_pkjsnk->m_pDatabase->BeginTrans();

		for(;;){
			try{
				m_pkjsnk->Delete();
			}
			catch( CDBException* dbe ){
				if( dbe->m_nRetCode == AFX_SQL_ERROR_NO_ROWS_AFFECTED ) {
					dbe->Delete();
					m_pkjsnk->Requery();
					if( m_pkjsnk->IsBOF() ) {
						m_pkjsnk->m_pDatabase->Rollback();
						return;
					}
					nodata=0;
					continue;
				}
				m_pkjsnk->m_pDatabase->Rollback();
//				if( m_pkjsnk->m_pCDBPrtblW ){
//					m_pkjsnk->m_pCDBPrtblW->SetErrMsg( DB_FATAL_ERROR, dbe->m_strError.GetBuffer(), dbe->m_nRetCode );
//				}
				dbe->Delete();
				return;
			}
			break;
		}
		m_pkjsnk->m_pDatabase->CommitTrans();
		m_pkjsnk->MoveNext();
	}
}
