#pragma once

#include "StdAfx.h"
#include ".\prtJobFunction.h"

class CprtUc021Uketoritegata :
	public CprtJobFunction
{
public:
	CprtUc021Uketoritegata(CDatabase* pdb);
	~CprtUc021Uketoritegata(void);

private:
	int					GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
	int					GetDbDataSheetNew2(int nPage, CprtJobFunction * parent, const PRT_OPTION & po, int nFormSeq); // ƒCƒ“ƒ{ƒCƒX“o˜^”Ô†’Ç‰Á‘Î‰ž_23/10/25 add
// midori 190301 add -->
	int					GetDbDataSheetNew(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
	int					GetDbDataSheetOld(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
// midori 190301 add <--
	CprtJobFunction*	virGetPrintObject();
	int					virGetPrintInfo(CprtJobFunction* parent);
};
