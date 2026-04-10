#pragma once

//-----------------------------------------------------------------------------
// 付表１、付表４データ（平成27年改正）
//-----------------------------------------------------------------------------

class CDBNpSub;
class CDBSyzAccess;
class CSyzUtil;

class SYZSHIN26DATA_CLASS_EXPORT CH27SnFhyo10Data : public CH26SnFhyo10Data
{
public:
	CH27SnFhyo10Data( BOOL isTransitionalMeasure );   // 標準コンストラクタ
	virtual ~CH27SnFhyo10Data();
};
