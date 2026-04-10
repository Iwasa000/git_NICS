// midori 170504 add -->
#pragma once
#include "impuc000common.h"
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc171Sonota1.h"

class CimpUc171Sonota1 :
	public CimpUc000Common
{
public:
	CimpUc171Sonota1(int formSeq1,int formSeq2);
	~CimpUc171Sonota1(void);

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
	CdbUc171Sonota1*		m_pUcData;				// 内訳書テーブル
	int						m_nFormSeq2;			// フォームシーケンス

protected:	// 関数
};
// midori 170504 add <--