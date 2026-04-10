#pragma once
#include "impuc000common.h"
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc081Siharaitegata.h"

class CimpUc081Siharaitegata :
	public CimpUc000Common
{
public:
	CimpUc081Siharaitegata(void);
	~CimpUc081Siharaitegata(void);

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
	CdbUc081Siharaitegata*	m_pUcData;				// 内訳書テーブル

// 改良No.21-0086,21-0529 add -->
	CDatabase*				m_pDB;
	CString					m_strKmName;			// 科目名称
	int						m_KmkRowSw;				// 科目行有り
	int						m_KmkRowSgn;			// 「科目行を挿入する」 有効/無効サイン
// 改良No.21-0086,21-0529 add <--

protected:	// 関数
};
