#pragma once

#include "StdAfx.h"
#include ".\prtJobFunction.h"

class CprtUc041Karibarai :
	public CprtJobFunction
{
public:
	CprtUc041Karibarai(CDatabase* pdb);
	~CprtUc041Karibarai(void);
	int					GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
// インボイス登録番号追加対応_23/10/25 add -->
	int					GetDbDataSheetOld(int nPage, CprtJobFunction * parent, const PRT_OPTION & po, int nFormSeq);
	int					GetDbDataSheetNew(int nPage, CprtJobFunction * parent, const PRT_OPTION & po, int nFormSeq);
// インボイス登録番号追加対応_23/10/25 add <--

private:
	CprtJobFunction*	virGetPrintObject();
	int					virGetPrintInfo(CprtJobFunction* parent);
};
