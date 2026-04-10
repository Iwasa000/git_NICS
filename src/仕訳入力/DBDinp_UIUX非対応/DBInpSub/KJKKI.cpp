// KJKKI.h : CKJKKIrecord クラスの実装

// CKJKKIrecord 実装

// コードの生成元 2007年2月1日, 9:43

#include "stdafx.h"

#include "KJNamRec.h"
#include "KJKKI.h"


IMPLEMENT_DYNAMIC(CKJKKIrecord, CRecordset)

CKJKKIrecord::CKJKKIrecord(CDatabase* pdb)
	: CRecordset(pdb)
{
	kj.KKI_KJCD = "";
	kj.KKI_KN50 = "";
	kj.KKI_PGEN = 0;
	kj.KKI_PYMD = 0;
	kj.KKI_CGEN = 0;
	kj.KKI_CYMD = 0;
	kj.KKI_YGEN = 0;
	kj.KKI_YYMD = 0;
	kj.KKI_KGEN = 0;
	kj.KKI_KYMD = 0;
	kj.KKI_HAIF = 0;
	kj.KKI_FURI = 0;
	kj.KKI_HSAG = 0;
	kj.KKI_HACD = "";
	kj.KKI_EICD = "";
	kj.KKI_GBCD = "";
	kj.KKI_RIT01 = 0;
	kj.KKI_RIT02 = 0;
	kj.KKI_RIT03 = 0;
	kj.KKI_NAME = "";
	kj.KKI_RYNM = "";
	kj.KKI_SENM = "";
	kj.KKI_BIKO = "";
	kj.KKI_GENB = "";
#ifdef CUT_CLOSE
	kj.KKI_DMCD01 = "";
	kj.KKI_DMCD02 = "";
	kj.KKI_DMCD03 = "";
	kj.KKI_DMSW01 = 0;
	kj.KKI_DMSW02 = 0;
	kj.KKI_DMSW03 = 0;
	kj.KKI_DMSW04 = 0;
	kj.KKI_DMSW05 = 0;
	kj.KKI_DMSW06 = 0;
	kj.KKI_DMSW07 = 0;
	kj.KKI_DMSW08 = 0;
	kj.KKI_DMSW09 = 0;
	kj.KKI_DMSW10 = 0;
	kj.KKI_DMVL01 = 0.0;
	kj.KKI_DMVL02 = 0.0;
	kj.KKI_DMVL03 = 0.0;
	kj.KKI_DMVL04 = 0.0;
	kj.KKI_DMVL05 = 0.0;
	kj.KKI_DMNM01 = "";
	kj.KKI_DMNM02 = "";
	kj.KKI_DMNM03 = "";
#endif
	kj.REG_DATE = 0;
	kj.REG_TIME = 0;
	kj.REG_USER = 0;
	kj.UPD_DATE = 0;
	kj.UPD_TIME = 0;
	kj.UPD_USER = 0;
	m_nFields = 30;
	m_nDefaultType = snapshot;
}

//#error Security Issue: The connection string may contain a password
// 接続文字列にはパスワードが含まれている可能性があります。
// 下の接続文字列には、暗号化されていないパスワードなど、
// 重要な情報が含まれている可能性があります。セキュリティ関連事項について、
// 接続文字列を検証した後、接続文字列を表示した後で #error を削除してください。
// パスワードを他の形式で保存するか、他のユーザー認証を使用してください。
CString CKJKKIrecord::GetDefaultConnect()
{
//	return _T("DSN=T20/ICS;APP=Microsoft\x00ae Visual Studio\x00ae 2005;WSID=T20;DATABASE=Z20_C02010001_Y0712;Network=DBMSSOCN;Trusted_Connection=Yes");
	return "ODBC;DSN=mydb;";
}

CString CKJKKIrecord::GetDefaultSQL()
{
	return _T("[dbo].[KJKKI]");
}

void CKJKKIrecord::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() および RFX_Int() などのマクロは、データベースのフィールドの型ではなく、
// メンバ 変数の型に依存しています。
// ODBC は、列の値を要求された型に自動的に変換しようとします。
	RFX_Text(pFX, _T("[KKI_KJCD]"), kj.KKI_KJCD);
	RFX_Text(pFX, _T("[KKI_KN50]"), kj.KKI_KN50);
	RFX_Byte(pFX, _T("[KKI_PGEN]"), kj.KKI_PGEN);
	RFX_Long(pFX, _T("[KKI_PYMD]"), kj.KKI_PYMD);
	RFX_Byte(pFX, _T("[KKI_CGEN]"), kj.KKI_CGEN);
	RFX_Long(pFX, _T("[KKI_CYMD]"), kj.KKI_CYMD);
	RFX_Byte(pFX, _T("[KKI_YGEN]"), kj.KKI_YGEN);
	RFX_Long(pFX, _T("[KKI_YYMD]"), kj.KKI_YYMD);
	RFX_Byte(pFX, _T("[KKI_KGEN]"), kj.KKI_KGEN);
	RFX_Long(pFX, _T("[KKI_KYMD]"), kj.KKI_KYMD);
	RFX_Byte(pFX, _T("[KKI_HAIF]"), kj.KKI_HAIF);
	RFX_Byte(pFX, _T("[KKI_FURI]"), kj.KKI_FURI);
	RFX_Long(pFX, _T("[KKI_HSAG]"), kj.KKI_HSAG);
	RFX_Text(pFX, _T("[KKI_HACD]"), kj.KKI_HACD);
	RFX_Text(pFX, _T("[KKI_EICD]"), kj.KKI_EICD);
	RFX_Text(pFX, _T("[KKI_GBCD]"), kj.KKI_GBCD);
	RFX_Long(pFX, _T("[KKI_RIT01]"), kj.KKI_RIT01);
	RFX_Long(pFX, _T("[KKI_RIT02]"), kj.KKI_RIT02);
	RFX_Long(pFX, _T("[KKI_RIT03]"), kj.KKI_RIT03);
	RFX_Text(pFX, _T("[KKI_NAME]"), kj.KKI_NAME);
	RFX_Text(pFX, _T("[KKI_RYNM]"), kj.KKI_RYNM);
	RFX_Text(pFX, _T("[KKI_SENM]"), kj.KKI_SENM);
	RFX_Text(pFX, _T("[KKI_BIKO]"), kj.KKI_BIKO);
	RFX_Text(pFX, _T("[KKI_GENB]"), kj.KKI_GENB);	//24

/*
	RFX_Text(pFX, _T("[KKI_DMCD01]"), kj.KKI_DMCD01);
	RFX_Text(pFX, _T("[KKI_DMCD02]"), kj.KKI_DMCD02);
	RFX_Text(pFX, _T("[KKI_DMCD03]"), kj.KKI_DMCD03);
	RFX_Byte(pFX, _T("[KKI_DMSW01]"), kj.KKI_DMSW01);
	RFX_Byte(pFX, _T("[KKI_DMSW02]"), kj.KKI_DMSW02);
	RFX_Byte(pFX, _T("[KKI_DMSW03]"), kj.KKI_DMSW03);
	RFX_Byte(pFX, _T("[KKI_DMSW04]"), kj.KKI_DMSW04);
	RFX_Byte(pFX, _T("[KKI_DMSW05]"), kj.KKI_DMSW05);
	RFX_Long(pFX, _T("[KKI_DMSW06]"), kj.KKI_DMSW06);
	RFX_Long(pFX, _T("[KKI_DMSW07]"), kj.KKI_DMSW07);
	RFX_Long(pFX, _T("[KKI_DMSW08]"), kj.KKI_DMSW08);
	RFX_Long(pFX, _T("[KKI_DMSW09]"), kj.KKI_DMSW09);
	RFX_Long(pFX, _T("[KKI_DMSW10]"), kj.KKI_DMSW10);
	RFX_Double(pFX, _T("[KKI_DMVL01]"), kj.KKI_DMVL01);
	RFX_Double(pFX, _T("[KKI_DMVL02]"), kj.KKI_DMVL02);
	RFX_Double(pFX, _T("[KKI_DMVL03]"), kj.KKI_DMVL03);
	RFX_Double(pFX, _T("[KKI_DMVL04]"), kj.KKI_DMVL04);
	RFX_Double(pFX, _T("[KKI_DMVL05]"), kj.KKI_DMVL05);
	RFX_Text(pFX, _T("[KKI_DMNM01]"), kj.KKI_DMNM01);
	RFX_Text(pFX, _T("[KKI_DMNM02]"), kj.KKI_DMNM02);
	RFX_Text(pFX, _T("[KKI_DMNM03]"), kj.KKI_DMNM03);
*/
	RFX_Long(pFX, _T("[REG_DATE]"), kj.REG_DATE);
	RFX_Long(pFX, _T("[REG_TIME]"), kj.REG_TIME);
	RFX_Int(pFX, _T("[REG_USER]"), kj.REG_USER);
	RFX_Long(pFX, _T("[UPD_DATE]"), kj.UPD_DATE);
	RFX_Long(pFX, _T("[UPD_TIME]"), kj.UPD_TIME);
	RFX_Int(pFX, _T("[UPD_USER]"), kj.UPD_USER);

}
/////////////////////////////////////////////////////////////////////////////
// CKJKKIrecord 診断

#ifdef _DEBUG
void CKJKKIrecord::AssertValid() const
{
	CRecordset::AssertValid();
}

void CKJKKIrecord::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


