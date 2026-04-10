#pragma once

#include "StdAfx.h"
#include ".\prtJobFunction.h"

class CprtUc031Urikakekin :
	public CprtJobFunction
{
public:
	CprtUc031Urikakekin(CDatabase* pdb);
	~CprtUc031Urikakekin(void);

private:
	int					GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
// インボイス登録番号追加対応_23/10/23 add -->
	int					GetDbDataSheetNew(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
	int					GetDbDataSheetOld(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
// インボイス登録番号追加対応_23/10/23 add <--

	CprtJobFunction*	virGetPrintObject();
	int					virGetPrintInfo(CprtJobFunction* parent);
};
