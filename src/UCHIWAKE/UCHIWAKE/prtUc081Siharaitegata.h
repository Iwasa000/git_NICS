#pragma once

#include "StdAfx.h"
#include ".\prtJobFunction.h"

class CprtUc081Siharaitegata :
	public CprtJobFunction
{
public:
	CprtUc081Siharaitegata(CDatabase* pdb);
	~CprtUc081Siharaitegata(void);

private:
	int					GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
// midori 190301 add -->
	int					GetDbDataSheetNew(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
	int					GetDbDataSheetOld(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
// midori 190301 add <--
	int					GetDbDataSheetNew2(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0); // ƒCƒ“ƒ{ƒCƒX“o˜^’Ç‰Á‘Î‰ž_23/10/25 add

	CprtJobFunction*	virGetPrintObject();
	int					virGetPrintInfo(CprtJobFunction* parent);
};
