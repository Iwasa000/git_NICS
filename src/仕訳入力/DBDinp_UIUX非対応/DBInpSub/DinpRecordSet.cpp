// DBDinp RecordSet
#include "stdafx.h"

#include "sql.h"
#include "sqlext.h"
#include "odbcinst.h"
#include <afxdb.h>
#include <afxdao.h>

#include "DinpRecordSet.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSetuptbl

IMPLEMENT_DYNAMIC(CSetuptbl, CRecordset)

CSetuptbl::CSetuptbl(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CSetuptbl)
	m_setupname = _T("");
	m_param = 0;
	m_parastr = _T("");
	m_nFields = 3;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}

CString CSetuptbl::GetDefaultSQL()
{
	return _T("[dbo].[mastsetup]");
}

void CSetuptbl::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CSetuptbl)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[setupname]"), m_setupname);
	RFX_Long(pFX, _T("[param]"), m_param);
	RFX_Text(pFX, _T("[parastr]"), m_parastr);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CSetuptbl êfíf

#ifdef _DEBUG
void CSetuptbl::AssertValid() const
{
	CRecordset::AssertValid();
}

void CSetuptbl::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG



/////////////////////////////////////////////////////////////////////////////
// CProc

IMPLEMENT_DYNAMIC(CProc, CRecordset)

CProc::CProc(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CVolume)
	m_nFields = 1;
	m_st = 0;
	//}}AFX_FIELD_INIT
}

CString CProc::GetDefaultSQL()
{
	return _T(
"select count(*) from dbo.sysobjects where id = object_id(N'[dbo].[adddata]') and OBJECTPROPERTY(id, N'IsProcedure') = 1");
}

void CProc::DoFieldExchange(CFieldExchange* pFX)
{

	//{{AFX_FIELD_MAP(CCourseSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long( pFX, "st", m_st );
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CProc ÉNÉâÉXÇÃêfíf

#ifdef _DEBUG
void CProc::AssertValid() const
{
	CRecordset::AssertValid();
}

void CProc::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG