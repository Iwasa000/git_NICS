#pragma once
#include "impuc000common.h"
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc141Yakuin.h"

class CimpUc141Yakuin :
	public CimpUc000Common
{
public:
	CimpUc141Yakuin(void);
	~CimpUc141Yakuin(void);

public:		// 変数

public:		// 関数
	// 各帳票固有の処理
	virtual int Init(CDatabase* pDB);
	virtual int SetCsvOneLineData(CStringArray* pstrCsvDataAry);
// midori 180404 add -->
	virtual int ImpCsvOneLineData(CStringArray* pstrCsvDataAry);
// midori 180404 add <--
	virtual int GetCsvOneLineData(CStringArray* pstrCsvDataAry);

protected:	// 変数
	// テーブル
	CdbUc141Yakuin*			m_pUcData;				// 内訳書テーブル

protected:	// 関数
	// データ加工（チェックを含む）
	BOOL		ConvFpTimeDbValToStr(BYTE cFpTime, CString* pstrFpTime);
	BOOL		ConvFpTimeStrToDbVal(CString strFpTime, BYTE* pcFpTime);
};
