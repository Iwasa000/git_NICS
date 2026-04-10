#pragma once
#include "impuc000common.h"
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc011Yotyokin.h"


class CimpUc011Yotyokin :
	public CimpUc000Common
{
public:
	CimpUc011Yotyokin(void);
	~CimpUc011Yotyokin(void);

public:		// 変数

public:		// 関数
	// 各帳票固有の処理
	virtual int Init(CDatabase* pDB);
	virtual int SetCsvOneLineData(CStringArray* pstrCsvDataAry);
// midori 180404 add -->
	virtual int ImpCsvOneLineData(CStringArray* pstrCsvDataAry);
// midori 180404 add <--
	virtual int GetCsvOneLineData(CStringArray* pstrCsvDataAry);
	//virtual int virSpecialLineToNull(int nPage, int MaxRow);

protected:	// 変数
	// テーブル
	CdbUc011Yotyokin*		m_pUcData;				// 内訳書テーブル

protected:	// 関数
};
