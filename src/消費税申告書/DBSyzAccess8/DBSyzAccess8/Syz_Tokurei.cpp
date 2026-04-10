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

IMPLEMENT_DYNAMIC(CSyz_Tokurei, CRecordset)

CSyz_Tokurei::CSyz_Tokurei(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CSyz_Tokurei)

    pr.sn_seq = 0;
    pr.version = 0;
    pr.hyoId = _T("");
    pr.itemId = _T("");
    pr.idx = 0;
    pr.kind = 0;
    pr.val =_T("");
    pr.rate = 0;
    pr.str = _T("");
    pr.sign1 = 0;

	m_nFields = 10;
	//}}AFX_FIELD_INIT

}

void CSyz_Tokurei::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CSyz_Tokurei)
	pFX->SetFieldType(CFieldExchange::outputColumn);

	RFX_Int(pFX, _T("sn_seq"), pr.sn_seq);
	RFX_Int(pFX, _T("version"), pr.version);
	RFX_Text(pFX, _T("hyoId"), pr.hyoId);
	RFX_Text(pFX, _T("itmId"), pr.itemId);
	RFX_Int(pFX, _T("idx"), pr.idx);
	RFX_Int(pFX, _T("kind"), pr.kind);
	RFX_Text(pFX, _T("val"), pr.val, 16, SQL_DECIMAL);
	RFX_Int(pFX, _T("rate"), pr.rate);
	RFX_Text(pFX, _T("str"), pr.str);
	RFX_Int(pFX, _T("sign1"), pr.sign1);

	//}}AFX_FIELD_MAP
}

CString CSyz_Tokurei::GetDefaultSQL()
{
	return _T("syz_tokurei");
}

/////////////////////////////////////////////////////////////////////////////
// CSyz_Tokurei ƒNƒ‰ƒX‚Ìf’f

#ifdef _DEBUG
void CSyz_Tokurei::AssertValid() const
{
	CRecordset::AssertValid();
}

void CSyz_Tokurei::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CSYZ_TOKUREI
CSYZ_TOKUREI::CSYZ_TOKUREI(CSyz_Tokurei *psyz_tokurei)
{
	m_psyz_tokurei = psyz_tokurei;
	nodata = 0;
}

void CSYZ_TOKUREI::data()
{
	if (sn_seq == -1) {
		st = -1;
		sn_seq = 0;
		version = 0;
		hyoId = _T("");
		itemId = _T("");
		idx = 0;
		kind = 0;
		val = _T("");
		rate = 0;
		str = _T("");
		sign1 = 0;
		return;
	}
	st = 0;
}

int CSYZ_TOKUREI::MoveFirst()
{
	sn_seq = -1;
	if (!nodata) {
		m_psyz_tokurei->MoveFirst();
	}
	data();
	return st;
}

int CSYZ_TOKUREI::MoveNext()
{
	sn_seq = -1;
	if (!m_psyz_tokurei->IsEOF()) {
		m_psyz_tokurei->MoveNext();
	}
	data();
	return st;
}

int CSYZ_TOKUREI::MovePrev()
{
	sn_seq = -1;
	if (!m_psyz_tokurei->IsBOF()) {
		m_psyz_tokurei->MovePrev();
	}
	data();
	return st;
}

int CSYZ_TOKUREI::MoveLast()
{
	sn_seq = -1;
	if (!nodata) {
		m_psyz_tokurei->MoveLast();
	}
	data();
	return st;
}

int CSYZ_TOKUREI::Move(int nrows)
{
	sn_seq = -1;
	if (nrows > 0) {
		if (!m_psyz_tokurei->IsEOF()) {
			m_psyz_tokurei->Move(nrows);
		}
	}
	else if (nrows < 0) {
		if (!m_psyz_tokurei->IsBOF()) {
			m_psyz_tokurei->Move(nrows);
		}
	}
	else {
		m_psyz_tokurei->Move(nrows);
	}
	data();
	return st;
}

int CSYZ_TOKUREI::SetAbsolutePosition(int nrows)
{
	sn_seq = -1;
	if (!nodata) {
		m_psyz_tokurei->SetAbsolutePosition(nrows);
	}
	data();
	return st;
}

int CSYZ_TOKUREI::Requery(CString filter)
{
	sn_seq = -1;
	m_psyz_tokurei->m_strFilter = filter;
	m_psyz_tokurei->m_strSort = SYZ_TOKUREI_VAL_SORT;
	m_psyz_tokurei->Requery();
	if (m_psyz_tokurei->IsBOF()) {
		nodata = 1;
	}
	else {
		nodata = 0;
	}
	data();
	return st;
}

int CSYZ_TOKUREI::GetRecordCount()
{
	CRecordsetStatus	rstat;
	long	cpos;

	m_psyz_tokurei->GetStatus(rstat);
	if (rstat.m_bRecordCountFinal) {
		return(m_psyz_tokurei->GetRecordCount());
	}
	else {
		if ((cpos = rstat.m_lCurrentRecord) < 0) {
			m_psyz_tokurei->MoveFirst();
		}
		for (; !m_psyz_tokurei->IsEOF(); ) {
			m_psyz_tokurei->MoveNext();
		}
		if (cpos >= 0) {
			m_psyz_tokurei->SetAbsolutePosition(cpos + 1);
		}
		return(m_psyz_tokurei->GetRecordCount());
	}
}

void CSYZ_TOKUREI::Edit()
{
	m_psyz_tokurei->m_pDatabase->BeginTrans();
	m_psyz_tokurei->Edit();
}

void CSYZ_TOKUREI::AddNew()
{
	m_psyz_tokurei->m_pDatabase->BeginTrans();
	m_psyz_tokurei->AddNew();
}

int CSYZ_TOKUREI::Update()
{
	try {
		st = m_psyz_tokurei->Update();
	}
	catch (CDBException* dbe) {
		if (dbe->m_nRetCode == AFX_SQL_ERROR_NO_ROWS_AFFECTED) {
			dbe->Delete();
			m_psyz_tokurei->Requery();
			if (m_psyz_tokurei->IsBOF()) {
				nodata = 1;
			}
			else {
				nodata = 0;
			}
			return dbe->m_nRetCode;
		}
		m_psyz_tokurei->m_pDatabase->Rollback();
		dbe->Delete();
		return -1;
	}

	m_psyz_tokurei->m_pDatabase->CommitTrans();

	return st;
}

void CSYZ_TOKUREI::Delete()
{
	if (!nodata) {
		m_psyz_tokurei->m_pDatabase->BeginTrans();

		for (;;) {
			try {
				m_psyz_tokurei->Delete();
			}
			catch (CDBException* dbe) {
				if (dbe->m_nRetCode == AFX_SQL_ERROR_NO_ROWS_AFFECTED) {
					dbe->Delete();
					m_psyz_tokurei->Requery();
					if (m_psyz_tokurei->IsBOF()) {
						m_psyz_tokurei->m_pDatabase->Rollback();
						return;
					}
					nodata = 0;
					continue;
				}
				m_psyz_tokurei->m_pDatabase->Rollback();
				dbe->Delete();
				return;
			}
			break;
		}
		m_psyz_tokurei->m_pDatabase->CommitTrans();
		m_psyz_tokurei->MoveNext();
	}
}

