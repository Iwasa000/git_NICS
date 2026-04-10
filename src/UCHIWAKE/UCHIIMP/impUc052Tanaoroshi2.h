#pragma once
#include "impuc000common.h"
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc052Tanaoroshi2.h"

class CimpUc052Tanaoroshi2 :
	public CimpUc000Common
{
public:
	CimpUc052Tanaoroshi2(void);
	~CimpUc052Tanaoroshi2(void);

public:		// 変数
// midori 180404 add -->
	BYTE m_Method;
// midori 180404 add <--
	
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
	CdbUc052Tanaoroshi2*	m_pUcData;				// 内訳書テーブル

protected:	// 関数
	// データ加工（チェックを含む）
	BOOL		ConvMethodStrToDbVal(CString strMethod, BYTE* pcMethod);
	BOOL		ConvMethodDbValToStr(BYTE cMethod, CString* pstrMethod);
	BOOL		ConvRegNameDbValToStr(BYTE cMethod, CString* pstrMethod);
};
