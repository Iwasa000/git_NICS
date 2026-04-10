#pragma once
#include "impuc000common.h"
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc161Zatueki.h"

class CimpUc161Zatueki :
	public CimpUc000Common
{
public:
	CimpUc161Zatueki(void);
	~CimpUc161Zatueki(void);

public:		// 変数

public:		// 関数
	// 各帳票固有の処理
	virtual int Init(CDatabase* pDB);
	virtual int SetCsvOneLineData(CStringArray* pstrCsvDataAry);
// midori 180404 add -->
	virtual int ImpCsvOneLineData(CStringArray* pstrCsvDataAry);
// midori 180404 add <--
	virtual int GetCsvOneLineData(CStringArray* pstrCsvDataAry);
	virtual	void virTableEditIkkatuLine( int nType, CdbUc000Common* rsData, CString strName, CALCKEI_INFO uCalcKei, SORTKAMOKU_INFO uKamoku );

protected:	// 変数
	// テーブル
	CdbUc161Zatueki*		m_pUcData;				// 内訳書テーブル

protected:	// 関数
};
