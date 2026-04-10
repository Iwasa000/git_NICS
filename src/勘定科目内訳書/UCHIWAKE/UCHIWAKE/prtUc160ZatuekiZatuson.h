#pragma once

#include ".\prtJobFunction.h"

class CprtUc160ZatuekiZatuson :
	public CprtJobFunction
{
public:
	CprtUc160ZatuekiZatuson(CDatabase* pdb);
	~CprtUc160ZatuekiZatuson(void);

private:
	int					GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
	CprtJobFunction*	virGetPrintObject();
// インボイス登録番号追加対応_23/11/01 del -->
	//int					virGetPrintInfo(CprtJobFunction* parent);
	//int					GetDbDataUc161(int nPage, CprtJobFunction* parent, const PRT_OPTION& po);
	//int					GetDbDataUc162(int nPage, CprtJobFunction* parent, const PRT_OPTION& po);
// インボイス登録番号追加対応_23/11/01 del <--
// インボイス登録番号追加対応_23/11/01 add -->
	int					GetDbDataUc161New(int nPage, CprtJobFunction * parent, const PRT_OPTION & po);
	int					GetDbDataUc161Old(int nPage, CprtJobFunction * parent, const PRT_OPTION & po);
	int					GetDbDataUc162New(int nPage, CprtJobFunction * parent, const PRT_OPTION & po);
	int					GetDbDataUc162Old(int nPage, CprtJobFunction * parent, const PRT_OPTION & po);
// インボイス登録番号追加対応_23/11/01 add <--
};
