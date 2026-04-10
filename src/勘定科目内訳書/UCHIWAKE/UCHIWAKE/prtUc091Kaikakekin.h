#pragma once

#include "StdAfx.h"
#include ".\prtJobFunction.h"

class CprtUc091Kaikakekin :
	public CprtJobFunction
{
private:
	int			m_OutType;					// 0:1ページ 1:最終ページ 2:全ページ

public:
	CprtUc091Kaikakekin(CDatabase* pdb);
	~CprtUc091Kaikakekin(void);

private:
	int					GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
	int					GetDbDataSheetNew(int nPage, CprtJobFunction * parent, const PRT_OPTION & po, int nFormSeq);	// インボイス登録番号追加対応_23/10/25 add
	int					GetDbDataSheetOld(int nPage, CprtJobFunction * parent, const PRT_OPTION & po, int nFormSeq);	// インボイス登録番号追加対応_23/10/25 add


	CprtJobFunction*	virGetPrintObject();
	int					virGetPrintInfo(CprtJobFunction* parent);
// midori 155525 del -->
	//int					GetDbDataUc092(int nPage, int nMaxPage);
// midori 155525 del <--
// midori 155525 add -->
	int					GetDbDataUc092( int nPage, int nMaxPage, const PRT_OPTION& po );
// midori 155525 add <--
	int					GetDbDataUc092New( int nPage, int nMaxPage, const PRT_OPTION& po );		// インボイス登録番号追加対応_23/10/25 add
};
