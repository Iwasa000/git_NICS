#pragma once


class CDataZeiCalqSub;

#ifdef _DATAZEICALQ_
class AFX_EXT_CLASS CDataZeiCalq
#else
class CDataZeiCalq
#endif
{
public:
	CDataZeiCalq();
	virtual ~CDataZeiCalq();

public:
	int		DZCIniz(CDBZmSub* pZm);
	int		DZCClose();
	LPCTSTR GetDefZeiData(CDBipDataRec* pdata);
	LPCTSTR GetDefZeiData(CDBINPDataRec* pdata);

private:
	class	CDataZeiCalqSub* pSub;
	char	rsv[256];
};
