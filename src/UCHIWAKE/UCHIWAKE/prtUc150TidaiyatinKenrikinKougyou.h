#pragma once

#include ".\prtJobFunction.h"

class CprtUc150TidaiyatinKenrikinKougyou :
	public CprtJobFunction
{
public:
	CprtUc150TidaiyatinKenrikinKougyou(CDatabase* pdb);
	~CprtUc150TidaiyatinKenrikinKougyou(void);

private:
	int					GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
	CprtJobFunction*	virGetPrintObject();
// インボイス登録番号追加対応_23/10/23 del -->
	//int					virGetPrintInfo(CprtJobFunction* parent);
	//int					GetDbDataUc151(int nPage, CprtJobFunction* parent, int nFormSeq, const PRT_OPTION& po );
	//int					GetDbDataUc152(int nPage, CprtJobFunction* parent, int nFormSeq, const PRT_OPTION& po );
	//int					GetDbDataUc153(int nPage, CprtJobFunction* parent, int nFormSeq, const PRT_OPTION& po );
// インボイス登録番号追加対応_23/10/23 del <--
// インボイス登録番号追加対応_23/10/23 add -->
	int					GetDbDataUc151New(int nPage, CprtJobFunction * parent, int nFormSeq, const PRT_OPTION & po);
	int					GetDbDataUc151Old(int nPage, CprtJobFunction * parent, int nFormSeq, const PRT_OPTION & po);
	int					GetDbDataUc152New(int nPage, CprtJobFunction * parent, int nFormSeq, const PRT_OPTION & po);
	int					GetDbDataUc152Old(int nPage, CprtJobFunction * parent, int nFormSeq, const PRT_OPTION & po);
	int					GetDbDataUc153New(int nPage, CprtJobFunction * parent, int nFormSeq, const PRT_OPTION & po);
	int					GetDbDataUc153Old(int nPage, CprtJobFunction * parent, int nFormSeq, const PRT_OPTION & po);
// インボイス登録番号追加対応_23/10/23 add <--
};
