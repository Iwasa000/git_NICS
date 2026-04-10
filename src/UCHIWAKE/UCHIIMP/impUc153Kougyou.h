#pragma once
#include "impuc000common.h"
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc153Kougyou.h"

class CimpUc153Kougyou :
	public CimpUc000Common
{
public:
	CimpUc153Kougyou(void);
	~CimpUc153Kougyou(void);

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
	CdbUc153Kougyou*		m_pUcData;				// 内訳書テーブル

protected:	// 関数
};
