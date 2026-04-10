// Žd“üTœÅŠz‚ÉŠÖ‚·‚é–¾×‘wŽå‚È‰ÛÅŽd“ü‚ê“™‚Ì–¾×x
#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void AFXAPI RFX_Bigint(CFieldExchange* pFX, LPCTSTR szName, __int64& value);

/////////////////////////////////////////////////////////////////////////////
// CSkjrec

IMPLEMENT_DYNAMIC(CSkjrec, CRecordset)

CSkjrec::CSkjrec( CDatabase* pdb )
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CSkjrec)
	pr.SkjType = 0;			/*- •Ô‘—’l -*/
	pr.SkjLine = 0;			/*- y–@EŒÂzy–@EŒÂzs”Ô†		(ƒL[) -*/
	pr.SkjColm = 0;			/*- y–@EŒÂzy–@EŒÂz—ñ”Ô†		(ƒL[) -*/
	pr.SkjSaveDay = 0;		/*- y–@EŒÂzy–@EŒÂz•Û‘¶ŽŠ”NŒŽ“ú(ƒL[) -*/
	pr.AsKndUpBuf = "";		/*- y–@EŒÂzŽ‘ŽY‚ÌŽí—Þiã’ij	[Š¿Žš”¼Šp8•¶Žš] -*/
	pr.AsKndLwBuf = "";		/*- y–@EŒÂzŽ‘ŽY‚ÌŽí—Þi‰º’ij	[Š¿Žš”¼Šp8•¶Žš] -*/
	pr.SyutokuDay = 0;		/*- y–@EŒÂzŽæ“¾”NŒŽ“ú -*/
	pr.TrMny = "";			/*- y–@EŒÂzŽæˆø‹àŠz“™			[BIN 12Œ…] -*/
	pr.NameUpBuf = "";		/*- y–@EŒÂzŽæˆøæŽ–¼iã’ij	[Š¿Žš”¼Šp10•¶Žš] -*/
	pr.NameLwBuf = "";		/*- y–@EŒÂzŽæˆøæŽ–¼i‰º’ij	[Š¿Žš”¼Šp10•¶Žš] -*/
	pr.AdrUpBuf = "";		/*- y–@EŒÂzŽæˆøæZŠiã’ij	[Š¿Žš”¼Šp15•¶Žš] -*/
	pr.AdrLwBuf = "";		/*- y–@EŒÂzŽæˆøæZŠi‰º’ij	[Š¿Žš”¼Šp15•¶Žš] -*/

	m_nFields = 12;

	//}}AFX_FIELD_INIT
}


void CSkjrec::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CSkjrec)
	pFX->SetFieldType(CFieldExchange::outputColumn);

	RFX_Int( pFX,  _T("SkjType"), pr.SkjType );					/*- •Ô‘—’l -*/
	RFX_Int( pFX,  _T("SkjLine"), pr.SkjLine );					/*- y–@EŒÂzy–@EŒÂzs”Ô†		(ƒL[)-*/
	RFX_Int( pFX,  _T("SkjColm"), pr.SkjColm );					/*- y–@EŒÂzy–@EŒÂz—ñ”Ô†		(ƒL[)-*/
	RFX_Long( pFX, _T("SkjSaveDay"), pr.SkjSaveDay );			/*- y–@EŒÂzy–@EŒÂz•Û‘¶ŽŠ”NŒŽ“ú(ƒL[)-*/
	RFX_Text( pFX, _T("AsKndUpBuf"), pr.AsKndUpBuf );			/*- y–@EŒÂzŽ‘ŽY‚ÌŽí—Þiã’ij	[Š¿Žš”¼Šp8•¶Žš] -*/
	RFX_Text( pFX, _T("AsKndLwBuf"), pr.AsKndLwBuf );			/*- y–@EŒÂzŽ‘ŽY‚ÌŽí—Þi‰º’ij	[Š¿Žš”¼Šp8•¶Žš] -*/
	RFX_Long( pFX, _T("SyutokuDay"), pr.SyutokuDay );			/*- y–@EŒÂzŽæ“¾”NŒŽ“ú -*/
	RFX_Text( pFX, _T("TrMny"), pr.TrMny, 16, SQL_DECIMAL );	/*- y–@EŒÂzŽæˆø‹àŠz“™			[BIN 12Œ…] -*/
	RFX_Text( pFX, _T("NameUpBuf"), pr.NameUpBuf );				/*- y–@EŒÂzŽæˆøæŽ–¼iã’ij	[Š¿Žš”¼Šp10•¶Žš] -*/
	RFX_Text( pFX, _T("NameLwBuf"), pr.NameLwBuf );				/*- y–@EŒÂzŽæˆøæŽ–¼i‰º’ij	[Š¿Žš”¼Šp10•¶Žš] -*/
	RFX_Text( pFX, _T("AdrUpBuf"), pr.AdrUpBuf );				/*- y–@EŒÂzŽæˆøæZŠiã’ij	[Š¿Žš”¼Šp15•¶Žš] -*/
	RFX_Text( pFX, _T("AdrLwBuf"), pr.AdrLwBuf );				/*- y–@EŒÂzŽæˆøæZŠi‰º’ij	[Š¿Žš”¼Šp15•¶Žš] -*/
																
	//}}AFX_FIELD_MAP
}				  
				  
CString CSkjrec::GetDefaultSQL()
{				  
	return _T("skjrec");
}

/////////////////////////////////////////////////////////////////////////////
// CSkjrec ƒNƒ‰ƒX‚Ìf’f

#ifdef _DEBUG
void CSkjrec::AssertValid() const
{
	CRecordset::AssertValid();
}

void CSkjrec::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CSKJREC
CSKJREC::CSKJREC( CSkjrec *pskjrec )
{
	m_pskjrec = pskjrec;
	nodata = 0;
}

void CSKJREC::data()
{
	if( SkjType == -1 ){	/*- y–@EŒÂz•Ô‘—’l -*/
		st = -1;
		SkjLine = 0;		/*- y–@EŒÂzy–@EŒÂzs”Ô†		(ƒL[) -*/
		SkjColm = 0;		/*- y–@EŒÂzy–@EŒÂz—ñ”Ô†		(ƒL[) -*/
		SkjSaveDay = 0;		/*- y–@EŒÂzy–@EŒÂz•Û‘¶ŽŠ”NŒŽ“ú(ƒL[) -*/
		AsKndUpBuf = "";	/*- y–@EŒÂzŽ‘ŽY‚ÌŽí—Þiã’ij	[Š¿Žš”¼Šp8•¶Žš] -*/
		AsKndLwBuf = "";	/*- y–@EŒÂzŽ‘ŽY‚ÌŽí—Þi‰º’ij	[Š¿Žš”¼Šp8•¶Žš] -*/
		SyutokuDay = 0;		/*- y–@EŒÂzŽæ“¾”NŒŽ“ú -*/
		TrMny = "0";		/*- y–@EŒÂzŽæˆø‹àŠz“™			[BIN 12Œ…] -*/
		NameUpBuf = "";		/*- y–@EŒÂzŽæˆøæŽ–¼iã’ij	[Š¿Žš”¼Šp10•¶Žš] -*/
		NameLwBuf = "";		/*- y–@EŒÂzŽæˆøæŽ–¼i‰º’ij	[Š¿Žš”¼Šp10•¶Žš] -*/
		AdrUpBuf = "";		/*- y–@EŒÂzŽæˆøæZŠiã’ij	[Š¿Žš”¼Šp15•¶Žš] -*/
		AdrLwBuf = "";		/*- y–@EŒÂzŽæˆøæZŠi‰º’ij	[Š¿Žš”¼Šp15•¶Žš] -*/

		return;
	}
	st = 0;
}

int CSKJREC::MoveFirst()
{
	SkjLine = -1;
	if( !nodata ){
		m_pskjrec->MoveFirst();
	}
	data();
	return st;
}

int CSKJREC::MoveNext()
{
	SkjLine = -1;
	if( ! m_pskjrec->IsEOF() ){
		m_pskjrec->MoveNext();
	}
	data();
	return st;
}

int CSKJREC::MovePrev()
{
	SkjLine = -1;
	if( ! m_pskjrec->IsBOF() ){
		m_pskjrec->MovePrev();
	}
	data();
	return st;
}

int CSKJREC::MoveLast()
{
	SkjLine = -1;
	if( ! nodata ){
		m_pskjrec->MoveLast();
	}
	data();
	return st;
}

int CSKJREC::Move( int nrows )
{
	SkjLine = -1;
	if( nrows > 0 ) {
		if( ! m_pskjrec->IsEOF() ){
			m_pskjrec->Move(nrows);
		}
	}
	else if( nrows < 0 ) {
		if( ! m_pskjrec->IsBOF() ){
			m_pskjrec->Move(nrows);
		}
	}
	else{
		m_pskjrec->Move(nrows);
	}
	data();
	return st;
}

int CSKJREC::SetAbsolutePosition( int nrows )
{
	SkjLine = -1;
	if( ! nodata ){
		m_pskjrec->SetAbsolutePosition(nrows);
	}
	data();
	return st;
}

int CSKJREC::Requery( CString filter )
{
	SkjLine = -1;
	m_pskjrec->m_strFilter = filter;
	m_pskjrec->m_strSort =  _T("SkjLine,SkjColm,SkjSaveDay");
	m_pskjrec->Requery();
	if( m_pskjrec->IsBOF() ){
		nodata = 1;
	}
	else{
		nodata = 0;
	}
	data();
	return st;
}

int CSKJREC::GetRecordCount()
{
	CRecordsetStatus	rstat;
	long	cpos;

	m_pskjrec->GetStatus( rstat );
	if( rstat.m_bRecordCountFinal ){
		return( m_pskjrec->GetRecordCount() );
	}
	else {
		if( (cpos=rstat.m_lCurrentRecord) < 0 ){
			m_pskjrec->MoveFirst();
		}
		for( ; ! m_pskjrec->IsEOF(); ){
			m_pskjrec->MoveNext();
		}
		if( cpos >= 0 ){
			m_pskjrec->SetAbsolutePosition(cpos+1);
		}
		return( m_pskjrec->GetRecordCount() );
	}
}

void CSKJREC::Edit()
{
	m_pskjrec->m_pDatabase->BeginTrans();
	m_pskjrec->Edit();
}

void CSKJREC::AddNew()
{
	m_pskjrec->m_pDatabase->BeginTrans();
	m_pskjrec->AddNew();
}

int CSKJREC::Update()
{
	try{
		st = m_pskjrec->Update();
	}
	catch( CDBException* dbe ){
		if( dbe->m_nRetCode == AFX_SQL_ERROR_NO_ROWS_AFFECTED ) {
			dbe->Delete();
			m_pskjrec->Requery();
			if( m_pskjrec->IsBOF() ){
				nodata = 1;
			}
			else{
				nodata = 0;
			}
			return dbe->m_nRetCode;
		}
		m_pskjrec->m_pDatabase->Rollback();
		dbe->Delete();
		return -1;
	}

	m_pskjrec->m_pDatabase->CommitTrans();
	
	return st;
}

void CSKJREC::Delete()
{
	if( ! nodata ) {
		m_pskjrec->m_pDatabase->BeginTrans();

		for(;;){
			try{
				m_pskjrec->Delete();
			}
			catch( CDBException* dbe ){
				if( dbe->m_nRetCode == AFX_SQL_ERROR_NO_ROWS_AFFECTED ) {
					dbe->Delete();
					m_pskjrec->Requery();
					if( m_pskjrec->IsBOF() ) {
						m_pskjrec->m_pDatabase->Rollback();
						return;
					}
					nodata=0;
					continue;
				}
				m_pskjrec->m_pDatabase->Rollback();
				dbe->Delete();
				return;
			}
			break;
		}
		m_pskjrec->m_pDatabase->CommitTrans();
		m_pskjrec->MoveNext();
	}
}
