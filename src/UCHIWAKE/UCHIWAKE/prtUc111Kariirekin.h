#pragma once

#include "StdAfx.h"
#include ".\prtJobFunction.h"

class CprtUc111Kariirekin :
	public CprtJobFunction
{
public:
	CprtUc111Kariirekin(CDatabase* pdb);
	~CprtUc111Kariirekin(void);

private:
	int					GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
	CprtJobFunction*	virGetPrintObject();
	int					virGetPrintInfo(CprtJobFunction* parent);
};
