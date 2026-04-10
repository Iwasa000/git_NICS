#pragma once
#include "prtjobfunction.h"

class CprtUc100KariukekinGensen :
	public CprtJobFunction
{
public:
	CprtUc100KariukekinGensen(CDatabase* pdb);
	~CprtUc100KariukekinGensen(void);

private:
	int					GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
	CprtJobFunction*	virGetPrintObject();
// インボイス登録番号追加対応_23/10/31 del -->
	//int					virGetPrintInfo(CprtJobFunction* parent);
	//int					GetDbDataUc101(int nPage, CprtJobFunction* parent, const PRT_OPTION& po);
// インボイス登録番号追加対応_23/10/31 del <--
// インボイス登録番号追加対応_23/10/31 add -->
	int					GetDbDataUc101New(int nPage, CprtJobFunction * parent, const PRT_OPTION & po);
	int					GetDbDataUc101Old(int nPage, CprtJobFunction * parent, const PRT_OPTION & po);
// インボイス登録番号追加対応_23/10/31 add <--
	int					GetDbDataUc102(int nPage, CprtJobFunction* parent, const PRT_OPTION& po);
};
