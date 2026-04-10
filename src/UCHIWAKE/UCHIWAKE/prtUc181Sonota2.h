#pragma once

#include "StdAfx.h"
#include ".\prtJobFunction.h"

class CprtUc181Sonota2 :
	public CprtJobFunction
{
public:
	CprtUc181Sonota2(CDatabase* pdb);
	~CprtUc181Sonota2(void);

private:
	int					GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int nFormSeq = 0);
	CprtJobFunction*	virGetPrintObject();
	int					virGetPrintInfo(CprtJobFunction* parent);
};
