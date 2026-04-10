#pragma once

#include "StdAfx.h"
#include ".\prtJobFunction.h"

class CprtUc142Jinkenhi :
	public CprtJobFunction
{
public:
	CprtUc142Jinkenhi(CDatabase* pdb);
	~CprtUc142Jinkenhi(void);

private:
	int					GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
	CprtJobFunction*	virGetPrintObject();
	int					virGetPrintInfo(CprtJobFunction* parent);
};
