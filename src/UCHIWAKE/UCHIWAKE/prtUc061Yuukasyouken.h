#pragma once

#include "StdAfx.h"
#include ".\prtJobFunction.h"

class CprtUc061Yuukasyouken :
	public CprtJobFunction
{
public:
	CprtUc061Yuukasyouken(CDatabase* pdb);
	~CprtUc061Yuukasyouken(void);

private:
	int					GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
// midori 190301 add -->
	int					GetDbDataSheetNew(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
	int					GetDbDataSheetOld(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
// midori 190301 add <--
	CprtJobFunction*	virGetPrintObject();
	int					virGetPrintInfo(CprtJobFunction* parent);
};
