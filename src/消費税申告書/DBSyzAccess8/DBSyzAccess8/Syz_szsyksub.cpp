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

IMPLEMENT_DYNAMIC(CSzsyksub, CRecordset)

CSzsyksub::CSzsyksub(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CSzsyksub)

	pr.Szkcod = _T("");
	pr.Szgrp1 = 0;
	pr.Szgrp2 = 0;
	pr.SZzkbn = 0;
	pr.Szitm = 0;
	pr.SZsjsg = 0;
	pr.Szkatt = 0;
	pr.SZzchg = 0;
	pr.SZrsv.SetSize(1);
    pr.SZrsv[0] = 0x00;
    pr.Syksubofs = 0; 
	pr.Sztaig = _T("");
	pr.Szhikz = _T("");
	pr.Szyuka = _T("");
	pr.Szkomi = _T("");
	pr.Szkomz = _T("");
	pr.Sznuki = _T("");
	pr.Sznukz = _T("");
    pr.Szbeki = _T("");

	m_nFields = 18;
	//}}AFX_FIELD_INIT

}

void CSzsyksub::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CSzsyksub)
	pFX->SetFieldType(CFieldExchange::outputColumn);

	RFX_Text(pFX, _T("Szkcod"), pr.Szkcod);
	RFX_Byte(pFX, _T("Szgrp1"), pr.Szgrp1);
	RFX_Byte(pFX, _T("Szgrp2"), pr.Szgrp2);
	RFX_Byte(pFX, _T("SZzkbn"), pr.SZzkbn);
	RFX_Byte(pFX, _T("Szitm"), pr.Szitm);
	RFX_Byte(pFX, _T("SZsjsg"), pr.SZsjsg);
	RFX_Byte(pFX, _T("Szkatt"), pr.Szkatt);
	RFX_Byte(pFX, _T("SZzchg"), pr.SZzchg);
    RFX_Binary(pFX, _T("SZrsv"), (CByteArray&)pr.SZrsv, 1);
	RFX_Byte(pFX, _T("Syksubofs"), pr.Syksubofs);
	RFX_Text(pFX, _T("Sztaig"), pr.Sztaig);
	RFX_Text(pFX, _T("Szhikz"), pr.Szhikz);
	RFX_Text(pFX, _T("Szyuka"), pr.Szyuka);
	RFX_Text(pFX, _T("Szkomi"), pr.Szkomi);
	RFX_Text(pFX, _T("Szkomz"), pr.Szkomz);
	RFX_Text(pFX, _T("Sznuki"), pr.Sznuki);
	RFX_Text(pFX, _T("Sznukz"), pr.Sznukz);
	RFX_Text(pFX, _T("Szbeki"), pr.Szbeki);

	//}}AFX_FIELD_MAP
}

CString CSzsyksub::GetDefaultSQL()
{
	return _T("szsyksub");
}

/////////////////////////////////////////////////////////////////////////////
// CSzsyksub ƒNƒ‰ƒX‚Ìf’f

#ifdef _DEBUG
void CSzsyksub::AssertValid() const
{
	CRecordset::AssertValid();
}

void CSzsyksub::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CSzsyksub
CSZSYKSUB::CSZSYKSUB(CSzsyksub *pszsyksub)
{
	m_pSzsyksub = pszsyksub;
	nodata = 0;
}

void CSZSYKSUB::data()
{
	if (Syksubofs == UCHAR_MAX) {
        st = -1;
		Szkcod = _T("");
		Szgrp1 = 0;
		Szgrp2 = 0;
		SZzkbn = 0;
		Szitm = 0;
		SZsjsg = 0;
		Szkatt = 0;
		SZzchg = 0;
		SZrsv.SetSize(1);
		SZrsv[0] = 0x00;
		Syksubofs = 0;
		Sztaig = _T("");
		Szhikz = _T("");
		Szyuka = _T("");
		Szkomi = _T("");
		Szkomz = _T("");
		Sznuki = _T("");
		Sznukz = _T("");
		Szbeki = _T("");
		return;
	}
	st = 0;
}

int CSZSYKSUB::MoveFirst()
{
	Syksubofs = UCHAR_MAX;
	if (!nodata) {
		m_pSzsyksub->MoveFirst();
	}
	data();
	return st;
}

int CSZSYKSUB::MoveNext()
{
	Syksubofs = UCHAR_MAX;
	if (!m_pSzsyksub->IsEOF()) {
		m_pSzsyksub->MoveNext();
	}
	data();
	return st;
}

int CSZSYKSUB::MovePrev()
{
	Syksubofs = UCHAR_MAX;
	if (!m_pSzsyksub->IsBOF()) {
		m_pSzsyksub->MovePrev();
	}
	data();
	return st;
}

int CSZSYKSUB::MoveLast()
{
	Syksubofs = UCHAR_MAX;
	if (!nodata) {
		m_pSzsyksub->MoveLast();
	}
	data();
	return st;
}

int CSZSYKSUB::Move(int nrows)
{
	Syksubofs = UCHAR_MAX;
	if (nrows > 0) {
		if (!m_pSzsyksub->IsEOF()) {
			m_pSzsyksub->Move(nrows);
		}
	}
	else if (nrows < 0) {
		if (!m_pSzsyksub->IsBOF()) {
			m_pSzsyksub->Move(nrows);
		}
	}
	else {
		m_pSzsyksub->Move(nrows);
	}
	data();
	return st;
}

int CSZSYKSUB::SetAbsolutePosition(int nrows)
{
	Syksubofs = UCHAR_MAX;
	if (!nodata) {
		m_pSzsyksub->SetAbsolutePosition(nrows);
	}
	data();
	return st;
}

int CSZSYKSUB::Requery(CString filter)
{
	Syksubofs = UCHAR_MAX;
	m_pSzsyksub->m_strFilter = filter;
	m_pSzsyksub->m_strSort = SYZ_SZSYKSUB_VAL_SORT;
	m_pSzsyksub->Requery();
	if (m_pSzsyksub->IsBOF()) {
		nodata = 1;
	}
	else {
		nodata = 0;
	}
	data();
	return st;
}

int CSZSYKSUB::GetRecordCount()
{
	CRecordsetStatus	rstat;
	long	cpos;

	m_pSzsyksub->GetStatus(rstat);
	if (rstat.m_bRecordCountFinal) {
		return(m_pSzsyksub->GetRecordCount());
	}
	else {
		if ((cpos = rstat.m_lCurrentRecord) < 0) {
			m_pSzsyksub->MoveFirst();
		}
		for (; !m_pSzsyksub->IsEOF(); ) {
			m_pSzsyksub->MoveNext();
		}
		if (cpos >= 0) {
			m_pSzsyksub->SetAbsolutePosition(cpos + 1);
		}
		return(m_pSzsyksub->GetRecordCount());
	}
}

void CSZSYKSUB::Edit()
{
	m_pSzsyksub->m_pDatabase->BeginTrans();
	m_pSzsyksub->Edit();
}

void CSZSYKSUB::AddNew()
{
	m_pSzsyksub->m_pDatabase->BeginTrans();
	m_pSzsyksub->AddNew();
}

int CSZSYKSUB::Update()
{
	try {
		st = m_pSzsyksub->Update();
	}
	catch (CDBException* dbe) {
		if (dbe->m_nRetCode == AFX_SQL_ERROR_NO_ROWS_AFFECTED) {
			dbe->Delete();
			m_pSzsyksub->Requery();
			if (m_pSzsyksub->IsBOF()) {
				nodata = 1;
			}
			else {
				nodata = 0;
			}
			return dbe->m_nRetCode;
		}
		m_pSzsyksub->m_pDatabase->Rollback();
		dbe->Delete();
		return -1;
	}

	m_pSzsyksub->m_pDatabase->CommitTrans();

	return st;
}

void CSZSYKSUB::Delete()
{
	if (!nodata) {
		m_pSzsyksub->m_pDatabase->BeginTrans();

		for (;;) {
			try {
				m_pSzsyksub->Delete();
			}
			catch (CDBException* dbe) {
				if (dbe->m_nRetCode == AFX_SQL_ERROR_NO_ROWS_AFFECTED) {
					dbe->Delete();
					m_pSzsyksub->Requery();
					if (m_pSzsyksub->IsBOF()) {
						m_pSzsyksub->m_pDatabase->Rollback();
						return;
					}
					nodata = 0;
					continue;
				}
				m_pSzsyksub->m_pDatabase->Rollback();
				dbe->Delete();
				return;
			}
			break;
		}
		m_pSzsyksub->m_pDatabase->CommitTrans();
		m_pSzsyksub->MoveNext();
	}
}

