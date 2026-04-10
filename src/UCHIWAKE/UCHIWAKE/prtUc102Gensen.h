#pragma once

#include "StdAfx.h"
#include ".\prtJobFunction.h"

class CprtUc102Gensen :
	public CprtJobFunction
{
public:
	CprtUc102Gensen(CDatabase* pdb);
	~CprtUc102Gensen(void);

private:
	int					GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
	CprtJobFunction*	virGetPrintObject();
	int					virGetPrintInfo(CprtJobFunction* parent);
};
