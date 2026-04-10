#pragma once
#include "impuc000common.h"
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc021Uketoritegata.h"

//////////////////////////////////////////////////////////////////

class CimpUc021Uketoritegata :
	public CimpUc000Common
{
public:
	CimpUc021Uketoritegata(void);
	~CimpUc021Uketoritegata(void);

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
	//virtual int virFormatedIsNoKamokuVer(CStringArray* pstrCsvDataAry);
// 157766 del <--
// 210113_5301_1 del -->
// ※未使用のため削除
	//virtual int	virFormatedIsKamokuAddedVer(CStringArray* pstrCsvDataAry);
// 210113_5301_1 del <--
// midori 156937 del -->
//// midori 156189,156190,156191 add -->
//	CString ImpGetSyokeiName(void);
//// midori 156189,156190,156191 add <--
// midori 156937 del <--

protected:	// 変数
	// 内訳書テーブル
	CdbUc021Uketoritegata*		m_pUcData;
// midori 156189,156190,156191 add -->
	CDatabase*					m_pDB;
	CString						m_strKmName;			// 科目名称
	int							m_KmkRowSw;				// 科目行有り
// midori 156189,156190,156191 add <--
// midori 157046 add -->
	int							m_KmkRowSgn;			// 「科目行を挿入する」 有効/無効サイン
// midori 157046 add <--

protected:	// 関数
};
