#pragma once

//#include "StdAfx.h"
#include ".\prtJobFunction.h"

class CprtUc141Yakuin :
	public CprtJobFunction
{
public:
	CprtUc141Yakuin(CDatabase* pdb);
	~CprtUc141Yakuin(void);

private:
// 改良No.23-0961 add -->
	int					m_141ValSign;			// 様式⑭-1、出力するデータを見て、9桁以上入力された金額があるかどうかのサイン
// 改良No.23-0961 add <--

	int					GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
// 調査依頼20240712_24/09/19 add -->
	int					GetDbDataSheetOld(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
	int					GetDbDataSheetNew(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
// 調査依頼20240712_24/09/19 add <--
	CprtJobFunction*	virGetPrintObject();
	//int					virGetPrintInfo(CprtJobFunction* parent);	// ※未使用のためコメント
};
