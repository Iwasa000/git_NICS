#pragma once

#include "StdAfx.h"
#include ".\prtJobFunction.h"

class CprtUc131Uriagedaka :
	public CprtJobFunction
{
public:
	CprtUc131Uriagedaka(CDatabase* pdb);
	~CprtUc131Uriagedaka(void);

private:
	int					GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
	CprtJobFunction*	virGetPrintObject();
	int					virGetPrintInfo(CprtJobFunction* parent);
};
