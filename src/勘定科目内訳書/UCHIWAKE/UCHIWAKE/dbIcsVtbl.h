// dbIcsVtbl.h : CdbIcsVtbl の宣言

#pragma once

#include "dbBase.h"

class CdbIcsVtbl : public CdbBase
{
public:
	CdbIcsVtbl(CDatabase* pDatabase = NULL);

	int VersionCheck(int inType, CString inObjName);

	DECLARE_DYNAMIC(CdbIcsVtbl)

// フィールド/パラメータ データ

// 次の文字列型は (記載されていれば) データベース フィールドの
// 実際のデータ型を反映しています。CStringA は ANSI データ型を、また CStringW は Unicode データ型を
// 反映しています。これは、ODBC ドライバが不必要な変換を
// 行わないようにするためです。これらのメンバを CString 型に変えて、
// ODBC ドライバが必要な変換をすべて行うようにすることも可能です。
// (注 : Unicode とこれらの変換の両方をサポートするためには、
// ODBC ドライバのバージョン 3.5 かそれ以降のものを使う必要があります)

	long		m_type;
	CStringA	m_objname;
//	CString		m_objname;
	long		m_vers;

// オーバーライド
	// ウィザードで生成された仮想関数のオーバーライド
	public:

	virtual CString GetDefaultSQL(); 	// レコードセットの既定の SQL
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX サポート

// 実装
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};


