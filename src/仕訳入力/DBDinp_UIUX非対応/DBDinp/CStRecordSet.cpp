#include "stdafx.h"

#include "CStRecordSet.h"
///////////////////////////////////////////////////////////////////////////////////////
//
// CStRecordSet 実装
//
// コードの生成元 2007年1月6日, 17:07

IMPLEMENT_DYNAMIC(CStRecordSet, CRecordset)

CStRecordSet::CStRecordSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_st = 0;
	m_nFields = 1;
	m_nDefaultType = snapshot;
}
//#error Security Issue: The connection string may contain a password
// 接続文字列にはパスワードが含まれている可能性があります。
// 下の接続文字列には、暗号化されていないパスワードなど、
// 重要な情報が含まれている可能性があります。セキュリティ関連事項について、
// 接続文字列を検証した後、接続文字列を表示した後で #error を削除してください。
// パスワードを他の形式で保存するか、他のユーザー認証を使用してください。
CString CStRecordSet::GetDefaultConnect()
{
//	return _T("DSN=T20/ICS;APP=Microsoft\x00ae Visual Studio\x00ae 2005;WSID=T20;DATABASE=Z00_C00009000_Y0703;Network=DBMSSOCN;Trusted_Connection=Yes");
	return "ODBC;DSN=mydb;";
}

CString CStRecordSet::GetDefaultSQL()
{
	CString str;

	str = m_sql;

	return str;
}


void CStRecordSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() および RFX_Int() などのマクロは、データベースのフィールドの型ではなく、
// メンバ 変数の型に依存しています。
// ODBC は、列の値を要求された型に自動的に変換しようとします。
	RFX_Int(pFX, _T("st"), m_st);
}
/////////////////////////////////////////////////////////////////////////////
// CStRecordSet 診断

#ifdef _DEBUG
void CStRecordSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CStRecordSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

