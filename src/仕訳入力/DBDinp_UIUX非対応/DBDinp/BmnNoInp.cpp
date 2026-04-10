#include "stdafx.h"

#include "BmnNoInp.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////////////
//
// CBmnNoInp 実装
//

IMPLEMENT_DYNAMIC(CBmnNoInp, CRecordset)

CBmnNoInp::CBmnNoInp(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_count = 0;
	m_nFields = 1;
	m_nDefaultType = snapshot;
}
//#error Security Issue: The connection string may contain a password
// 接続文字列にはパスワードが含まれている可能性があります。
// 下の接続文字列には、暗号化されていないパスワードなど、
// 重要な情報が含まれている可能性があります。セキュリティ関連事項について、
// 接続文字列を検証した後、接続文字列を表示した後で #error を削除してください。
// パスワードを他の形式で保存するか、他のユーザー認証を使用してください。
CString CBmnNoInp::GetDefaultConnect()
{
//	return _T("DSN=T20/ICS;APP=Microsoft\x00ae Visual Studio\x00ae 2005;WSID=T20;DATABASE=Z00_C00009000_Y0703;Network=DBMSSOCN;Trusted_Connection=Yes");
	return "ODBC;DSN=mydb;";
}

CString CBmnNoInp::GetDefaultSQL()
{
	CString str;
//	str.Format( "select count(*) from bmkzrec where %s", m_query );
	str.Format( "select kzbmn from bmkzrec where %s", m_query );

	return str;
}


void CBmnNoInp::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() および RFX_Int() などのマクロは、データベースのフィールドの型ではなく、
// メンバ 変数の型に依存しています。
// ODBC は、列の値を要求された型に自動的に変換しようとします。
	RFX_Int(pFX, _T("count"), m_count);
}
/////////////////////////////////////////////////////////////////////////////
// CBmnNoInp 診断

#ifdef _DEBUG
void CBmnNoInp::AssertValid() const
{
	CRecordset::AssertValid();
}

void CBmnNoInp::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

