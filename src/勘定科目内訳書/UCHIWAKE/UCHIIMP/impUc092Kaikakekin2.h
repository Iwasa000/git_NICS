#pragma once
#include "impuc000common.h"
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc092Kaikakekin2.h"

class CimpUc092Kaikakekin2 :
	public CimpUc000Common
{
public:
	CimpUc092Kaikakekin2(void);
	~CimpUc092Kaikakekin2(void);

public:		// 変数

public:		// 関数
	// 各帳票固有の処理
	virtual int Init(CDatabase* pDB);
	virtual int SetCsvOneLineData(CStringArray* pstrCsvDataAry);
// midori 180404 add -->
	virtual int ImpCsvOneLineData(CStringArray* pstrCsvDataAry);
// midori 180404 add <--
	virtual int GetCsvOneLineData(CStringArray* pstrCsvDataAry);
// midori 150954 add -->
	virtual int	SetCsvNullLineData091(void);
// midori 150954 add <--

protected:	// 変数
	// テーブル
	CdbUc092Kaikakekin2*	m_pUcData;				// 内訳書テーブル
// midori 150954 add -->
	CDatabase*	m_pDB;
// midori 150954 add <--

protected:	// 関数


// 未払配当金/未払役員賞与の行判断用
private:
	int iHaitou;
	int iSyouyo;
};
