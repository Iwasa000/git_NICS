// midori 170504 add -->
#pragma once
#include "impuc000common.h"
#include "..\..\UCHIWAKE\UCHIWAKE\dbUc181Sonota2.h"

class CimpUc181Sonota2 :
	public CimpUc000Common
{
public:
	CimpUc181Sonota2(int formSeq1,int formSeq2,CString csv_181_item_02,CString csv_181_item_03);
	CimpUc181Sonota2(int formSeq1,int formSeq2);
	~CimpUc181Sonota2(void);

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
	CdbUc181Sonota2*		m_pUcData;				// 内訳書テーブル
	int						m_nFormSeq2;			// フォームシーケンス
protected:	// 関数
};
// midori 170504 add <--