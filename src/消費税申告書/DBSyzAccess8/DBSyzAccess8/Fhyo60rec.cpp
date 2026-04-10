// 
#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//void AFXAPI RFX_Bigint(CFieldExchange* pFX, LPCTSTR szName, __int64& value);

/////////////////////////////////////////////////////////////////////////////
// CPrfkin

IMPLEMENT_DYNAMIC(CFhyo60Rec, CRecordset)

CFhyo60Rec::CFhyo60Rec( CDatabase* pdb )
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CFhyo60Rec)

	pr.Sn_SEQ = 0;
	pr.Sn_6FSDEATH = 0;
	pr.Sn_6FSDAIHYO = _T("");
	pr.Sn_6FSLIMIT = 0;

	pr.Sn_6BS_USE = 0;
	pr.Sn_6BS_ADR = _T("");
	pr.Sn_6BS_TELNO = _T("");
	pr.Sn_6BS_NMFRI = _T("");
	pr.Sn_6BS_NAME = _T("");
	pr.Sn_6BS_SONOTA = _T("");

	m_nFields = 10;
	//}}AFX_FIELD_INIT
}

void CFhyo60Rec::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CFhyo60Rec)
	pFX->SetFieldType(CFieldExchange::outputColumn);

	RFX_Int( pFX,  _T("Sn_SEQ"), pr.Sn_SEQ );
	RFX_Int( pFX,  _T("Sn_6FSDEATH"), pr.Sn_6FSDEATH );
	RFX_Text( pFX,  _T("Sn_6FSDAIHYO"), pr.Sn_6FSDAIHYO );
	RFX_Int( pFX,  _T("Sn_6FSLIMIT"), pr.Sn_6FSLIMIT );

	RFX_Int( pFX, _T("Sn_6BS_USE"), pr.Sn_6BS_USE );
	RFX_Text( pFX, _T("Sn_6BS_ADR"), pr.Sn_6BS_ADR );
	RFX_Text( pFX, _T("Sn_6BS_TELNO"), pr.Sn_6BS_TELNO );
	RFX_Text( pFX, _T("Sn_6BS_NMFRI"), pr.Sn_6BS_NMFRI );
	RFX_Text( pFX, _T("Sn_6BS_NAME"), pr.Sn_6BS_NAME );
	RFX_Text( pFX, _T("Sn_6BS_SONOTA"), pr.Sn_6BS_SONOTA );

	//}}AFX_FIELD_MAP
}				  
				  
CString CFhyo60Rec::GetDefaultSQL()
{				  
	return _T("fhyo60rec");
}

/////////////////////////////////////////////////////////////////////////////
// CFhyo60Rec ƒNƒ‰ƒX‚Ìf’f

#ifdef _DEBUG
void CFhyo60Rec::AssertValid() const
{
	CRecordset::AssertValid();
}

void CFhyo60Rec::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CFHYO60REC
CFHYO60REC::CFHYO60REC( CFhyo60Rec *pfhyo60rec )
{
	m_pfhyo60rec = pfhyo60rec;
	nodata = 0;
}

void CFHYO60REC::data()
{
	if( Sn_SEQ == -1 ){
		st = -1;
		Sn_SEQ = 0;
		Sn_6FSDEATH = 0;
		Sn_6FSDAIHYO = _T("");
		Sn_6FSLIMIT = 0;

		Sn_6BS_USE = 0;
		Sn_6BS_ADR = _T("");
		Sn_6BS_TELNO = _T("");
		Sn_6BS_NMFRI = _T("");
		Sn_6BS_NAME = _T("");
		Sn_6BS_SONOTA = _T("");
		return;
	}
	st = 0;
}

int CFHYO60REC::MoveFirst()
{
	Sn_SEQ = -1;
	if( !nodata ){
		m_pfhyo60rec->MoveFirst();
	}
	data();
	return st;
}

int CFHYO60REC::MoveNext()
{
	Sn_SEQ = -1;
	if( ! m_pfhyo60rec->IsEOF() ){
		m_pfhyo60rec->MoveNext();
	}
	data();
	return st;
}

int CFHYO60REC::MovePrev()
{
	Sn_SEQ = -1;
	if( ! m_pfhyo60rec->IsBOF() ){
		m_pfhyo60rec->MovePrev();
	}
	data();
	return st;
}

int CFHYO60REC::MoveLast()
{
	Sn_SEQ = -1;
	if( ! nodata ){
		m_pfhyo60rec->MoveLast();
	}
	data();
	return st;
}

int CFHYO60REC::Move( int nrows )
{
	Sn_SEQ = -1;
	if( nrows > 0 ) {
		if( ! m_pfhyo60rec->IsEOF() ){
			m_pfhyo60rec->Move(nrows);
		}
	}
	else if( nrows < 0 ) {
		if( ! m_pfhyo60rec->IsBOF() ){
			m_pfhyo60rec->Move(nrows);
		}
	}
	else{
		m_pfhyo60rec->Move(nrows);
	}
	data();
	return st;
}

int CFHYO60REC::SetAbsolutePosition( int nrows )
{
	Sn_SEQ = -1;
	if( ! nodata ){
		m_pfhyo60rec->SetAbsolutePosition(nrows);
	}
	data();
	return st;
}

int CFHYO60REC::Requery( CString filter )
{
	Sn_SEQ = -1;
	m_pfhyo60rec->m_strFilter = filter;
	m_pfhyo60rec->m_strSort =  _T("Sn_SEQ");
	m_pfhyo60rec->Requery();
	if( m_pfhyo60rec->IsBOF() ){
		nodata = 1;
	}
	else{
		nodata = 0;
	}
	data();
	return st;
}

int CFHYO60REC::GetRecordCount()
{
	CRecordsetStatus	rstat;
	long	cpos;

	m_pfhyo60rec->GetStatus( rstat );
	if( rstat.m_bRecordCountFinal ){
		return( m_pfhyo60rec->GetRecordCount() );
	}
	else {
		if( (cpos=rstat.m_lCurrentRecord) < 0 ){
			m_pfhyo60rec->MoveFirst();
		}
		for( ; ! m_pfhyo60rec->IsEOF(); ){
			m_pfhyo60rec->MoveNext();
		}
		if( cpos >= 0 ){
			m_pfhyo60rec->SetAbsolutePosition(cpos+1);
		}
		return( m_pfhyo60rec->GetRecordCount() );
	}
}

void CFHYO60REC::Edit()
{
	m_pfhyo60rec->m_pDatabase->BeginTrans();
	m_pfhyo60rec->Edit();
}

void CFHYO60REC::AddNew()
{
	m_pfhyo60rec->m_pDatabase->BeginTrans();
	m_pfhyo60rec->AddNew();
	Sn_SEQ = 0;
	Sn_6FSDEATH = 0;
	Sn_6FSDAIHYO = _T("");
	Sn_6FSLIMIT = 0;
}

int CFHYO60REC::Update()
{
	try{
		st = m_pfhyo60rec->Update();
	}
	catch( CDBException* dbe ){
		if( dbe->m_nRetCode == AFX_SQL_ERROR_NO_ROWS_AFFECTED ) {
			dbe->Delete();
			m_pfhyo60rec->Requery();
			if( m_pfhyo60rec->IsBOF() ){
				nodata = 1;
			}
			else{
				nodata = 0;
			}
			return dbe->m_nRetCode;
		}
		m_pfhyo60rec->m_pDatabase->Rollback();
		//if( m_pfhyo60rec ){
		//	ErrMsg = dbe->m_strError.GetBuffer();
		//}
		dbe->Delete();
		return -1;
	}

	m_pfhyo60rec->m_pDatabase->CommitTrans();
	
	return st;
}

void CFHYO60REC::Delete()
{
	if( ! nodata ) {
		m_pfhyo60rec->m_pDatabase->BeginTrans();

		for(;;){
			try{
				m_pfhyo60rec->Delete();
			}
			catch( CDBException* dbe ){
				if( dbe->m_nRetCode == AFX_SQL_ERROR_NO_ROWS_AFFECTED ) {
					dbe->Delete();
					m_pfhyo60rec->Requery();
					if( m_pfhyo60rec->IsBOF() ) {
						m_pfhyo60rec->m_pDatabase->Rollback();
						return;
					}
					nodata=0;
					continue;
				}
				m_pfhyo60rec->m_pDatabase->Rollback();
				//if( m_pfhyo60rec ){
				//	ErrMsg = dbe->m_strError.GetBuffer();
				//}
				dbe->Delete();
				return;
			}
			break;
		}
		m_pfhyo60rec->m_pDatabase->CommitTrans();
		m_pfhyo60rec->MoveNext();
	}
}

