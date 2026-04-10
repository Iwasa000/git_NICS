#pragma once
#include "impuc000common.h"
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc111Kariirekin.h"

class CimpUc111Kariirekin :
	public CimpUc000Common
{
public:
	CimpUc111Kariirekin(void);
	~CimpUc111Kariirekin(void);

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
// midori 156937 del -->
//// midori 156189,156190,156191 add -->
//	CString ImpGetSyokeiName(void);
//// midori 156189,156190,156191 add <--
// midori 156937 del <--
// 157766 del -->
//// 210113_5301_1 add -->
//	virtual	BOOL virFormatedIsNewVer(CStringArray* pstrCsvDataAry);
//// 210113_5301_1 add <--
// 157766 del <--

protected:	// 変数
	// テーブル
	CdbUc111Kariirekin*		m_pUcData;				// 内訳書テーブル
// midori 156189,156190,156191 add -->
	CDatabase*				m_pDB;
	CString					m_strKmName;			// 科目名称
	int						m_KmkRowSw;				// 科目行有り
// midori 156189,156190,156191 add <--
// midori 157046 add -->
	int						m_KmkRowSgn;			// 「科目行を挿入する」 有効/無効サイン
// midori 157046 add <--

protected:	// 関数
};
