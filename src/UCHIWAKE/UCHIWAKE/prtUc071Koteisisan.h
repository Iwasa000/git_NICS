#pragma once

#include "StdAfx.h"
#include ".\prtJobFunction.h"

class CprtUc071Koteisisan :
	public CprtJobFunction
{
public:
	CprtUc071Koteisisan(CDatabase* pdb);
	~CprtUc071Koteisisan(void);

private:
	int					GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
//	インボイス登録番号追加対応_23/10/23 add -->
	int					GetDbDataSheetNew(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
	int					GetDbDataSheetOld(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
//	インボイス登録番号追加対応_23/10/23 add <--

	CprtJobFunction*	virGetPrintObject();
	int					virGetPrintInfo(CprtJobFunction* parent);
};
