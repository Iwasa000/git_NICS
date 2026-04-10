#pragma once

#include "StdAfx.h"
#include ".\prtJobFunction.h"

class CprtUc051Tanaorosi :
	public CprtJobFunction
{
private:
	int			m_OutType;					// 0:1ページ 1:最終ページ 2:全ページ

public:
	CprtUc051Tanaorosi(CDatabase* pdb);
	~CprtUc051Tanaorosi(void);

private:
	int					GetDbDataSheet(int nPage, CprtJobFunction* parent, const PRT_OPTION& po, int FormSeq = 0);
	CprtJobFunction*	virGetPrintObject();
	int					virGetPrintInfo(CprtJobFunction* parent);
	int					GetDbDataUc052(int nPage, int nMaxPage);
	void				SetRangaiData(const CdbUc052Tanaoroshi2& db);
	int					GetPrintPage(int position, int max_page, int cur_page);
};
