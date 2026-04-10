#pragma once

#include ".\prtJobFunction.h"

class CprtUc040KaribaraiKasituke :
	public CprtJobFunction
{
public:
	CprtUc040KaribaraiKasituke(CDatabase* pdb);
	~CprtUc040KaribaraiKasituke(void);
	
private:
	int					GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
	CprtJobFunction*	virGetPrintObject();
// インボイス登録番号追加対応_23/10/23 del -->
	//int					virGetPrintInfo(CprtJobFunction* parent);
	//int					GetDbDataUc041(int nPage, CprtJobFunction* parent, const PRT_OPTION& po);
	//int					GetDbDataUc042(int nPage, CprtJobFunction* parent, const PRT_OPTION& po);
// インボイス登録番号追加対応_23/10/23 del <--
// インボイス登録番号追加対応_23/10/23 add -->
	int					GetDbDataUc041New(int nPage, CprtJobFunction * parent, const PRT_OPTION & po);
	int					GetDbDataUc041Old(int nPage, CprtJobFunction * parent, const PRT_OPTION & po);
	int					GetDbDataUc042New(int nPage, CprtJobFunction * parent, const PRT_OPTION & po);
	int					GetDbDataUc042Old(int nPage, CprtJobFunction * parent, const PRT_OPTION & po);
// インボイス登録番号追加対応_23/10/23 add <--
};
