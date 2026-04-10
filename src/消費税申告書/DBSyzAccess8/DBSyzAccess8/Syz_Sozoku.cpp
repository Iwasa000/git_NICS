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

IMPLEMENT_DYNAMIC(CSyz_Sozoku, CRecordset)

CSyz_Sozoku::CSyz_Sozoku( CDatabase* pdb )
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CSyz_Sozoku)

	pr.Sn_SEQ = 0;
	pr.Sn_NO = 0;
	pr.Sn_6FSJUSYO = _T("");
	pr.Sn_6FSFURI = _T("");
	pr.Sn_6FSNAME = _T("");
	pr.Sn_6FSHASH = _T("");
	pr.Sn_6FSSYOKU = _T("");
	pr.Sn_6FSTSUDUKI = _T("");
	pr.Sn_6FSBIRTH = 0;
	pr.Sn_6FSTEL1 = _T(""); //10
	pr.Sn_6FSTEL2 = _T("");
	pr.Sn_6FSTEL3 = _T("");
	pr.Sn_6FSHOTEI = 0;
	pr.Sn_6FSBUNSHI = _T("");
	pr.Sn_6FSBUNBO = _T("");
	pr.Sn_6FSKAGAKU = _T("");
	pr.Sn_6FSNOFUZEI = _T("");
	pr.Sn_6FSNOFUCHIHO = _T("");
	pr.Sn_6FSKANPUZEI = _T("");
	pr.Sn_6FSKANPUCHIHO = _T(""); //20
	pr.Sn_6FSNOFUKANPUsw = 0;
	pr.Sn_6FSGNAME = _T("");
	pr.Sn_6FSGSEL = 0;
	pr.Sn_6FSSNAME = _T("");
	pr.Sn_6FSSSEL = 0;
	pr.Sn_6FSYOKIN = _T("");
	pr.Sn_6FSKOUZA = _T("");
	pr.Sn_6FSKIGO1 = _T("");
	pr.Sn_6FSKIGO2 = _T("");
	pr.Sn_6FSYUBIN = _T(""); //30
	
	m_nFields = 30;
	//}}AFX_FIELD_INIT
}

void CSyz_Sozoku::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CSyz_Sozoku)
	pFX->SetFieldType(CFieldExchange::outputColumn);

	RFX_Int( pFX,  _T("Sn_SEQ"), pr.Sn_SEQ );
	RFX_Int( pFX,  _T("Sn_NO"), pr.Sn_NO );
	RFX_Text( pFX,  _T("Sn_6FSJUSYO"), pr.Sn_6FSJUSYO );
	RFX_Text( pFX,  _T("Sn_6FSFURI"), pr.Sn_6FSFURI );
	RFX_Text( pFX,  _T("Sn_6FSNAME"), pr.Sn_6FSNAME );
	RFX_Text( pFX,  _T("Sn_6FSHASH"), pr.Sn_6FSHASH );
	RFX_Text( pFX,  _T("Sn_6FSSYOKU"), pr.Sn_6FSSYOKU );
	RFX_Text( pFX,  _T("Sn_6FSTSUDUKI"), pr.Sn_6FSTSUDUKI );
	RFX_Int( pFX,  _T("Sn_6FSBIRTH"), pr.Sn_6FSBIRTH );
	RFX_Text( pFX,  _T("Sn_6FSTEL1"), pr.Sn_6FSTEL1 );
	RFX_Text( pFX,  _T("Sn_6FSTEL2"), pr.Sn_6FSTEL2 );
	RFX_Text( pFX,  _T("Sn_6FSTEL3"), pr.Sn_6FSTEL3 );
	RFX_Int( pFX,  _T("Sn_6FSHOTEI"), pr.Sn_6FSHOTEI );
	RFX_Text( pFX,  _T("Sn_6FSBUNSHI"), pr.Sn_6FSBUNSHI );
	RFX_Text( pFX,  _T("Sn_6FSBUNBO"), pr.Sn_6FSBUNBO );
	RFX_Text( pFX,  _T("Sn_6FSKAGAKU"), pr.Sn_6FSKAGAKU );
	RFX_Text( pFX,  _T("Sn_6FSNOFUZEI"), pr.Sn_6FSNOFUZEI );
	RFX_Text( pFX,  _T("Sn_6FSNOFUCHIHO"), pr.Sn_6FSNOFUCHIHO );
	RFX_Text( pFX,  _T("Sn_6FSKANPUZEI"), pr.Sn_6FSKANPUZEI );
	RFX_Text( pFX,  _T("Sn_6FSKANPUCHIHO"), pr.Sn_6FSKANPUCHIHO );
	RFX_Int( pFX,  _T("Sn_6FSNOFUKANPUsw"), pr.Sn_6FSNOFUKANPUsw  );
	RFX_Text( pFX,  _T("Sn_6FSGNAME"), pr.Sn_6FSGNAME );
	RFX_Int( pFX,  _T("Sn_6FSGSEL"), pr.Sn_6FSGSEL  );
	RFX_Text( pFX,  _T("Sn_6FSSNAME"), pr.Sn_6FSSNAME );
	RFX_Int( pFX,  _T("Sn_6FSSSEL"), pr.Sn_6FSSSEL  );
	RFX_Text( pFX,  _T("Sn_6FSYOKIN"), pr.Sn_6FSYOKIN );
	RFX_Text( pFX,  _T("Sn_6FSKOUZA"), pr.Sn_6FSKOUZA );
	RFX_Text( pFX,  _T("Sn_6FSKIGO1"), pr.Sn_6FSKIGO1 );
	RFX_Text( pFX,  _T("Sn_6FSKIGO2"), pr.Sn_6FSKIGO2 );
	RFX_Text( pFX,  _T("Sn_6FSYUBIN"), pr.Sn_6FSYUBIN );

	//}}AFX_FIELD_MAP
}				  
				  
CString CSyz_Sozoku::GetDefaultSQL()
{				  
	return _T("syz_sozoku");
}

/////////////////////////////////////////////////////////////////////////////
// CSyz_Sozoku ƒNƒ‰ƒX‚Ìf’f

#ifdef _DEBUG
void CSyz_Sozoku::AssertValid() const
{
	CRecordset::AssertValid();
}

void CSyz_Sozoku::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CSYZ_SOZOKU
CSYZ_SOZOKU::CSYZ_SOZOKU( CSyz_Sozoku *psyz_sozoku )
{
	m_psyz_sozoku = psyz_sozoku;
	nodata = 0;
}

void CSYZ_SOZOKU::data()
{
	if( Sn_SEQ == -1 ){
		st = -1;
		Sn_SEQ = 0;
		Sn_NO = 0;
		Sn_6FSJUSYO = _T("");
		Sn_6FSFURI = _T("");
		Sn_6FSNAME = _T("");
		Sn_6FSHASH = _T("");
		Sn_6FSSYOKU = _T("");
		Sn_6FSTSUDUKI = _T("");
		Sn_6FSBIRTH = 0;
		Sn_6FSTEL1 = _T("");
		Sn_6FSTEL2 = _T("");
		Sn_6FSTEL3 = _T("");
		Sn_6FSHOTEI = 0;
		Sn_6FSBUNSHI = _T("");
		Sn_6FSBUNBO = _T("");
		Sn_6FSKAGAKU = _T("");
		Sn_6FSNOFUZEI = _T("");
		Sn_6FSNOFUCHIHO = _T("");
		Sn_6FSKANPUZEI = _T("");
		Sn_6FSKANPUCHIHO = _T("");
		Sn_6FSNOFUKANPUsw = 0;
		Sn_6FSGNAME = _T("");
		Sn_6FSGSEL = 0;
		Sn_6FSSNAME = _T("");
		Sn_6FSSSEL = 0;
		Sn_6FSYOKIN = _T("");
		Sn_6FSKOUZA = _T("");
		Sn_6FSKIGO1 = _T("");
		Sn_6FSKIGO2 = _T("");
		Sn_6FSYUBIN = _T("");
		return;
	}
	st = 0;
}

int CSYZ_SOZOKU::MoveFirst()
{
	Sn_SEQ = -1;
	if( !nodata ){
		m_psyz_sozoku->MoveFirst();
	}
	data();
	return st;
}

int CSYZ_SOZOKU::MoveNext()
{
	Sn_SEQ = -1;
	if( ! m_psyz_sozoku->IsEOF() ){
		m_psyz_sozoku->MoveNext();
	}
	data();
	return st;
}

int CSYZ_SOZOKU::MovePrev()
{
	Sn_SEQ = -1;
	if( ! m_psyz_sozoku->IsBOF() ){
		m_psyz_sozoku->MovePrev();
	}
	data();
	return st;
}

int CSYZ_SOZOKU::MoveLast()
{
	Sn_SEQ = -1;
	if( ! nodata ){
		m_psyz_sozoku->MoveLast();
	}
	data();
	return st;
}

int CSYZ_SOZOKU::Move( int nrows )
{
	Sn_SEQ = -1;
	if( nrows > 0 ) {
		if( ! m_psyz_sozoku->IsEOF() ){
			m_psyz_sozoku->Move(nrows);
		}
	}
	else if( nrows < 0 ) {
		if( ! m_psyz_sozoku->IsBOF() ){
			m_psyz_sozoku->Move(nrows);
		}
	}
	else{
		m_psyz_sozoku->Move(nrows);
	}
	data();
	return st;
}

int CSYZ_SOZOKU::SetAbsolutePosition( int nrows )
{
	Sn_SEQ = -1;
	if( ! nodata ){
		m_psyz_sozoku->SetAbsolutePosition(nrows);
	}
	data();
	return st;
}

int CSYZ_SOZOKU::Requery( CString filter )
{
	Sn_SEQ = -1;
	m_psyz_sozoku->m_strFilter = filter;
	m_psyz_sozoku->m_strSort =  _T("Sn_SEQ,Sn_NO");
	m_psyz_sozoku->Requery();
	if( m_psyz_sozoku->IsBOF() ){
		nodata = 1;
	}
	else{
		nodata = 0;
	}
	data();
	return st;
}

int CSYZ_SOZOKU::GetRecordCount()
{
	CRecordsetStatus	rstat;
	long	cpos;

	m_psyz_sozoku->GetStatus( rstat );
	if( rstat.m_bRecordCountFinal ){
		return( m_psyz_sozoku->GetRecordCount() );
	}
	else {
		if( (cpos=rstat.m_lCurrentRecord) < 0 ){
			m_psyz_sozoku->MoveFirst();
		}
		for( ; ! m_psyz_sozoku->IsEOF(); ){
			m_psyz_sozoku->MoveNext();
		}
		if( cpos >= 0 ){
			m_psyz_sozoku->SetAbsolutePosition(cpos+1);
		}
		return( m_psyz_sozoku->GetRecordCount() );
	}
}

void CSYZ_SOZOKU::Edit()
{
	m_psyz_sozoku->m_pDatabase->BeginTrans();
	m_psyz_sozoku->Edit();
}

void CSYZ_SOZOKU::AddNew()
{
	m_psyz_sozoku->m_pDatabase->BeginTrans();
	m_psyz_sozoku->AddNew();
}

int CSYZ_SOZOKU::Update()
{
	try{
		st = m_psyz_sozoku->Update();
	}
	catch( CDBException* dbe ){
		if( dbe->m_nRetCode == AFX_SQL_ERROR_NO_ROWS_AFFECTED ) {
			dbe->Delete();
			m_psyz_sozoku->Requery();
			if( m_psyz_sozoku->IsBOF() ){
				nodata = 1;
			}
			else{
				nodata = 0;
			}
			return dbe->m_nRetCode;
		}
		m_psyz_sozoku->m_pDatabase->Rollback();
		//if( m_psyz_sozoku ){
		//	ErrMsg = dbe->m_strError.GetBuffer();
		//}
		dbe->Delete();
		return -1;
	}

	m_psyz_sozoku->m_pDatabase->CommitTrans();
	
	return st;
}

void CSYZ_SOZOKU::Delete()
{
	if( ! nodata ) {
		m_psyz_sozoku->m_pDatabase->BeginTrans();

		for(;;){
			try{
				m_psyz_sozoku->Delete();
			}
			catch( CDBException* dbe ){
				if( dbe->m_nRetCode == AFX_SQL_ERROR_NO_ROWS_AFFECTED ) {
					dbe->Delete();
					m_psyz_sozoku->Requery();
					if( m_psyz_sozoku->IsBOF() ) {
						m_psyz_sozoku->m_pDatabase->Rollback();
						return;
					}
					nodata=0;
					continue;
				}
				m_psyz_sozoku->m_pDatabase->Rollback();
				//if( m_psyz_sozoku ){
				//	ErrMsg = dbe->m_strError.GetBuffer();
				//}
				dbe->Delete();
				return;
			}
			break;
		}
		m_psyz_sozoku->m_pDatabase->CommitTrans();
		m_psyz_sozoku->MoveNext();
	}
}

