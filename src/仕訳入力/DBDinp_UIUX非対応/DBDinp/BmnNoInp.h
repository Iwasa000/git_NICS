#pragma once


//////////////////////////////////////////////////////////////////////////////////
//	テーブルの個数をチェック
// 

class CBmnNoInp : public CRecordset
{
public:
	CBmnNoInp(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CBmnNoInp)

// フィールド/パラメータ データ

// 次の文字列型は (記載されていれば) データベース フィールドの
// 実際のデータ型を反映しています。CStringA は ANSI データ型を、また CStringW は Unicode データ型を
// 反映しています。これは、ODBC ドライバが不必要な変換を
// 行わないようにするためです。これらのメンバを CString 型に変えて、
// ODBC ドライバが必要な変換をすべて行うようにすることも可能です。
// (注 : Unicode とこれらの変換の両方をサポートするためには、
// ODBC ドライバのバージョン 3.5 かそれ以降のものを使う必要があります)

	CString	m_query;
	int		m_count;

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


