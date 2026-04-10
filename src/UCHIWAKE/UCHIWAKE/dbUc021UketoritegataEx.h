#pragma once
#include "dbuc021uketoritegata.h"

class CdbUc021UketoritegataEx :
	public CdbUc021Uketoritegata
{
private:
	BOOL MyOpen();

public:
	virtual int RequeryWork();

	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX サポート

public:
	CdbUc021UketoritegataEx(CDatabase* db);
	virtual ~CdbUc021UketoritegataEx(void);
	virtual long GetColumnSortAddressBank(BYTE ItemSeq);
	virtual int UpdateDataTableWork();
};
