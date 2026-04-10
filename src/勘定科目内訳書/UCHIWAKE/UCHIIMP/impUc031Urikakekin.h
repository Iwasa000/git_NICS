#pragma once
#include "impuc000common.h"
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc031Urikakekin.h"

class CimpUc031Urikakekin :
	public CimpUc000Common
{
public:
	CimpUc031Urikakekin(void);
	~CimpUc031Urikakekin(void);

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
	CdbUc031Urikakekin*		m_pUcData;				// 内訳書テーブル

protected:	// 関数
};
