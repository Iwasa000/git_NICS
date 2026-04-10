#pragma once


//////////////////////////////////////////////////////////////////////////////////
//	テーブルの個数をチェック
// 

class CStRecordSet : public CRecordset
{
public:
	CStRecordSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CStRecordSet)

// フィールド/パラメータ データ

// 次の文字列型は (記載されていれば) データベース フィールドの
// 実際のデータ型を反映しています。CStringA は ANSI データ型を、また CStringW は Unicode データ型を
// 反映しています。これは、ODBC ドライバが不必要な変換を
// 行わないようにするためです。これらのメンバを CString 型に変えて、
// ODBC ドライバが必要な変換をすべて行うようにすることも可能です。
// (注 : Unicode とこれらの変換の両方をサポートするためには、
// ODBC ドライバのバージョン 3.5 かそれ以降のものを使う必要があります)

	int		m_st;
	CString	m_sql;
/************
	m_sql.Format( "select top(1) seq as st from data where owner = 1 order by seq DESC" );
 ************/

// オーバーライド
	// ウィザードで生成された仮想関数のオーバーライド
	public:
	virtual CString GetDefaultConnect();	// 既定の接続文字列

	virtual CString GetDefaultSQL(); 	// レコードセットの既定の SQL
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX サポート

// 実装
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};


