#pragma once
#include "impuc000common.h"
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc042Kasituke.h"

class CimpUc042Kasituke :
	public CimpUc000Common
{
public:
	CimpUc042Kasituke(void);
	~CimpUc042Kasituke(void);

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
// 157766 del -->
//// 210113_5301_1 add -->
//	virtual	BOOL virFormatedIsNewVer(CStringArray* pstrCsvDataAry);
//// 210113_5301_1 add <--
// 157766 del <--

protected:	// 変数
	// テーブル
	CdbUc042Kasituke*		m_pUcData;				// 内訳書テーブル

protected:	// 関数
};
