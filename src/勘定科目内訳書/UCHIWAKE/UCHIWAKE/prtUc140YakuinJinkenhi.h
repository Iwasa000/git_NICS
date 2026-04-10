#pragma once

#include ".\prtJobFunction.h"

class CprtUc140YakuinJinkenhi :
	public CprtJobFunction
{
public:
	CprtUc140YakuinJinkenhi(CDatabase* pdb);
	~CprtUc140YakuinJinkenhi(void);

private:
// 改良No.23-0961 add -->
	int					m_141ValSign;			// 様式⑭-1、出力するデータを見て、9桁以上入力された金額があるかどうかのサイン
// 改良No.23-0961 add <--

	int					GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
	CprtJobFunction*	virGetPrintObject();
	//int					virGetPrintInfo(CprtJobFunction* parent);	// ※未使用のためコメント
	int					GetDbDataUc141(int nPage, CprtJobFunction* parent,const PRT_OPTION& po);
// 調査依頼20240712_24/09/19 add -->
	int					GetDbDataUc141K(int nPage, CprtJobFunction* parent,const PRT_OPTION& po);
// 調査依頼20240712_24/09/19 add <--
	int					GetDbDataUc142(int nPage, CprtJobFunction* parent,const PRT_OPTION& po);
};
