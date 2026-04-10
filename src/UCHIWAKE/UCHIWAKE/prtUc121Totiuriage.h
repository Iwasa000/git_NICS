#pragma once

#include "StdAfx.h"
#include ".\prtJobFunction.h"

class CprtUc121Totiuriage :
	public CprtJobFunction
{
public:
	CprtUc121Totiuriage(CDatabase* pdb);
	~CprtUc121Totiuriage(void);

private:
	int					GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
	// インボイス登録番号追加対応_23/10/25 add -->
	int					GetDbDataSheetNew(int nPage, CprtJobFunction * parent, const PRT_OPTION & po, int nFormSeq);
	int					GetDbDataSheetOld(int nPage, CprtJobFunction * parent, const PRT_OPTION & po, int nFormSeq);
	// インボイス登録番号追加対応_23/10/25 add <--

	CprtJobFunction*	virGetPrintObject();
	int					virGetPrintInfo(CprtJobFunction* parent);
};
