#pragma once

#include "StdAfx.h"
#include ".\prtJobFunction.h"

class CprtUc011Yotyokin :
	public CprtJobFunction
{
public:
	CprtUc011Yotyokin(CDatabase* pdb);
	~CprtUc011Yotyokin(void);

private:
	int					GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
// midori 190301 add -->
	int					GetDbDataSheetNew(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
	int					GetDbDataSheetOld(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
// midori 190301 add <--
	CprtJobFunction*	virGetPrintObject();
	int					virGetPrintInfo(CprtJobFunction* parent);
};
