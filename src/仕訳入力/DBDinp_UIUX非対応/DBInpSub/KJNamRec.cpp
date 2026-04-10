#include "stdafx.h"

#include "KjNamRec.h"
#include "KJKKI.h"



CKJKKI::CKJKKI()
{
	m_rec = NULL;
	nodata = 0;
	reqst = 0;
}


int CKJKKI::MoveFirst()
{
	KKI_KJCD.Empty();
	if( ! nodata ) m_rec->MoveFirst();
	data();
	return(st);
}

int CKJKKI::MoveNext()
{
	KKI_KJCD.Empty();
	if( ! m_rec->IsEOF() ) m_rec->MoveNext();
	data();
	return(st);
}

int CKJKKI::MovePrev()
{
	KKI_KJCD.Empty();
	if( ! m_rec->IsBOF() ) m_rec->MovePrev();
	data();
	return(st);
}

int CKJKKI::MoveLast()
{
	KKI_KJCD.Empty();
	if( ! nodata ) m_rec->MoveLast();
	data();
	return(st);
}

int CKJKKI::Move(int nrows)
{
	KKI_KJCD.Empty();
	if( nrows > 0 ) {
		if( ! m_rec->IsEOF() ) m_rec->Move(nrows);
	}
	else if( nrows < 0 ) {
		if( ! m_rec->IsBOF() ) m_rec->Move(nrows);
	}
	else m_rec->Move(nrows);
	data();
	return(st);
}

int CKJKKI::SetAbsolutePosition(int nrows)
{
	KKI_KJCD.Empty();
	if( ! nodata ) m_rec->SetAbsolutePosition(nrows);
	data();
	return(st);
}

int CKJKKI::GetRecordCount()
{
	CRecordsetStatus	rstat;
	long	cpos;

	m_rec->GetStatus( rstat );
	if( rstat.m_bRecordCountFinal ) return( m_rec->GetRecordCount() );
	else {
		if( (cpos=rstat.m_lCurrentRecord) < 0 ) m_rec->MoveFirst();
		for( ; ! m_rec->IsEOF(); )
			m_rec->MoveNext();
		if( cpos >= 0 ) m_rec->SetAbsolutePosition(cpos+1);
		return( m_rec->GetRecordCount() );
	}
	return(-1);
}


int CKJKKI::Requery( LPCTSTR filter )
{
	KKI_KJCD.Empty();

	m_rec->m_strFilter = filter;
	m_rec->Requery();
	if( m_rec->IsBOF() ) nodata=1;
	else nodata=0;
	data();

	if( filter[0] != '\0' )	reqst = 1;
	else					reqst = 0;

	return(st);
}



void CKJKKI::data()
{
	if( KKI_KJCD.IsEmpty() ) {
		st = -1;
		KKI_KJCD.Empty();
		KKI_KN50.Empty();
		KKI_PGEN = (BYTE)-1;
		KKI_PYMD = -1;
		KKI_CGEN = (BYTE)-1;
		KKI_CYMD = -1;
		KKI_YGEN = (BYTE)-1;
		KKI_YYMD = -1;
		KKI_KGEN = (BYTE)-1;
		KKI_KYMD = -1;
		KKI_HAIF = (BYTE)-1;
		KKI_FURI = (BYTE)-1;
		KKI_HSAG = -1;
		KKI_HACD.Empty();
		KKI_EICD.Empty();
		KKI_GBCD.Empty();
		KKI_RIT01 = -1;
		KKI_RIT02 = -1;
		KKI_RIT03 = -1;
		KKI_NAME.Empty();
		KKI_RYNM.Empty();
		KKI_SENM.Empty();
		KKI_BIKO.Empty();
		KKI_GENB.Empty();
		REG_DATE = -1;
		REG_TIME = -1;
		REG_USER = -1;
		UPD_DATE = -1;
		UPD_TIME = -1;
		UPD_USER = -1;

		return;
	}

	st = 0;
}


////////////////////////////////////////////////////////////////////////////////////////////


CKJnamAccess::CKJnamAccess()
{
	kjnam	= NULL;
	pZm		= NULL;
}


CKJnamAccess::~CKJnamAccess()
{
	if( kjnam ) {
		KjNameClose();
		kjnam = NULL;
	}
}

void	CKJnamAccess::SetDBZmSub( CDBZmSub* pZmSub )
{
	pZm = pZmSub;
}


int	CKJnamAccess::KjNameOpen( LPCTSTR filter /*=_T("")*/)
{
	if( pZm == NULL )	return -1;

	// not found
	if( pZm->object_check( 2, _T("KJKKI") ) == 0 ) {
		return 0;
	}
	CString msg;

	TRY {
		CKJKKIrecord* pKKIrec = new CKJKKIrecord( pZm->m_database );

		pKKIrec->m_strFilter = filter;
		// 番号の小さい順
//		pKKIrec->m_strSort = _T("space( 12-len(kki_kjcd)) + kki_kjcd");
		// アルファベットの大文字・小文字を区別  _CS (区別しない場合は _CI)
		pKKIrec->m_strSort = _T("space( 12-len(kki_kjcd)) + kki_kjcd collate Japanese_CS_AS");

		kjnam = &pKKIrec->kj;

		kjnam->m_rec	= pKKIrec;
		if( filter[0]!='\0' )	kjnam->reqst = 1;

		pKKIrec->Open();
		if( pKKIrec->IsBOF() )	kjnam->nodata = 1;
		kjnam->data();

	} 
	CATCH( CMemoryException, me ) {
		msg = _T("Memory Alloc Error!");
		AfxMessageBox( msg );
		return -1;	
	}
	CATCH( CDBException, de ) {
		msg = de->m_strError;
		AfxMessageBox( msg );
		return -1;
	}
	END_CATCH

	return 0;
}



void	CKJnamAccess::KjNameClose()
{
	if( kjnam ) {
		kjnam->m_rec->Close();
		delete kjnam->m_rec;
		kjnam = NULL;
	}
}

