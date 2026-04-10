// 改良No.21-0086,21-0529 add -->
// dbUc081SiharaitegataEx.h : ヘッダー ファイル
//
#pragma once
#include "dbUc081Siharaitegata.h"

/////////////////////////////////////////////////////////////////////////////

class CdbUc081SiharaitegataEx :
	public CdbUc081Siharaitegata
{
private:
	BOOL MyOpen();

public:
	virtual int RequeryWork();

	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX サポート

public:
	CdbUc081SiharaitegataEx(CDatabase* db);
	virtual ~CdbUc081SiharaitegataEx(void);
	virtual long GetColumnSortAddressBank(BYTE ItemSeq);
	virtual int UpdateDataTableWork();
};

// 改良No.21-0086,21-0529 add <--
