// KJVOL.h : CKJVOL クラスの実装



// CKJVOL 実装

// コードの生成元 2011年9月12日, 15:01

#include "stdafx.h"
#include "KJVOL.h"
IMPLEMENT_DYNAMIC(CKJVOL, CRecordset)

CKJVOL::CKJVOL(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_VOL_IDCD = "";
	m_VOL_CODE = "";
	m_VOL_APNO = 0;
	m_VOL_MTYP = 0;
	m_VOL_PEDA01 = 0;
	m_VOL_PEDA02 = 0;
	m_VOL_PEDA03 = 0;
	m_VOL_PUCD01 = "";
	m_VOL_PUCD02 = "";
	m_VOL_PUCD03 = "";
	m_VOL_PUCD04 = "";
	m_VOL_PUCD05 = "";
	m_VOL_PUCD06 = "";
	m_VOL_PUCD07 = "";
	m_VOL_PUCD08 = "";
	m_VOL_PUCD09 = "";
	m_VOL_PUCD10 = "";
	m_VOL_HFSW = 0;
	m_VOL_HKJC = "";
	m_VOL_HTUK01 = 0;
	m_VOL_HTUK02 = 0;
	m_VOL_HTUK03 = 0;
	m_VOL_HTUK04 = 0;
	m_VOL_HTUK05 = 0;
	m_VOL_HTUK06 = 0;
	m_VOL_HTUK07 = 0;
	m_VOL_HTUK08 = 0;
	m_VOL_HTUK09 = 0;
	m_VOL_HTUK10 = 0;
	m_VOL_HTUK11 = 0;
	m_VOL_HTUK12 = 0;
	m_VOL_HTUK13 = 0;
	m_VOL_HTUK14 = 0;
	m_VOL_HTUK15 = 0;
	m_VOL_HTUK16 = 0;
	m_VOL_HCSW01 = 0;
	m_VOL_HCSW02 = 0;
	m_VOL_HCSW03 = 0;
	m_VOL_HCSW04 = 0;
	m_VOL_HCSW05 = 0;
	m_VOL_HCSW06 = 0;
	m_VOL_HCLQ01 = 0;
	m_VOL_HCLQ02 = 0;
	m_VOL_HCLQ03 = 0;
	m_VOL_HCLQ04 = 0;
	m_VOL_FKSW01 = 0;
	m_VOL_FKSW02 = 0;
	m_VOL_FKSW03 = 0;
	m_VOL_FKKM = 0;
	m_VOL_FKKT = 0;
	m_VOL_PRKM = 0;
	m_VOL_PRKT = 0;
	m_VOL_PRSW01 = 0;
	m_VOL_PRSW02 = 0;
	m_VOL_PRSW03 = 0;
	m_VOL_DMSW01 = 0;
	m_VOL_DMSW02 = 0;
	m_VOL_DMSW03 = 0;
	m_VOL_DMSW04 = 0;
	m_VOL_DMSW05 = 0;
	m_VOL_DMSW06 = 0;
	m_VOL_DMSW07 = 0;
	m_VOL_DMSW08 = 0;
	m_VOL_DMSW09 = 0;
	m_VOL_DMSW10 = 0;
	m_VOL_DMNO01 = 0;
	m_VOL_DMNO02 = 0;
	m_VOL_DMNO03 = 0;
	m_VOL_DMNO04 = 0;
	m_VOL_DMNO05 = 0;
	m_VOL_DMNM01 = "";
	m_VOL_DMNM02 = "";
	m_VOL_DMNM03 = "";
	m_VOL_DMNM04 = "";
	m_VOL_DMNM05 = "";
	m_REG_DATE = 0;
	m_REG_TIME = 0;
	m_REG_USER = 0;
	m_UPD_DATE = 0;
	m_UPD_TIME = 0;
	m_UPD_USER = 0;
	m_nFields = 81;
	m_nDefaultType = snapshot;
}
//#error Security Issue: The connection string may contain a password
// 接続文字列にはパスワードが含まれている可能性があります。
// 下の接続文字列には、暗号化されていないパスワードなど、
// 重要な情報が含まれている可能性があります。セキュリティ関連事項について、
// 接続文字列を検証した後、接続文字列を表示した後で #error を削除してください。
// パスワードを他の形式で保存するか、他のユーザー認証を使用してください。
CString CKJVOL::GetDefaultConnect()
{
//	return _T("DRIVER=SQL Server;SERVER=YUEDA-PC\\ICS;UID=sa;PWD=icsicsics;APP=MicrosoftR WindowsR Operating System;WSID=YUEDA-PC;DATABASE=Z20_C07190001_Y0712");
	return "ODBC;DSN=mydb;";
}

CString CKJVOL::GetDefaultSQL()
{
	return _T("[dbo].[KJVOL]");
}

void CKJVOL::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() および RFX_Int() などのマクロは、データベースのフィールドの型ではなく、
// メンバ 変数の型に依存しています。
// ODBC は、列の値を要求された型に自動的に変換しようとします。
	RFX_Text(pFX, _T("[VOL_IDCD]"), m_VOL_IDCD);
	RFX_Text(pFX, _T("[VOL_CODE]"), m_VOL_CODE);
	RFX_Int(pFX, _T("[VOL_APNO]"), m_VOL_APNO);
	RFX_Byte(pFX, _T("[VOL_MTYP]"), m_VOL_MTYP);
	RFX_Long(pFX, _T("[VOL_PEDA01]"), m_VOL_PEDA01);
	RFX_Long(pFX, _T("[VOL_PEDA02]"), m_VOL_PEDA02);
	RFX_Long(pFX, _T("[VOL_PEDA03]"), m_VOL_PEDA03);
	RFX_Text(pFX, _T("[VOL_PUCD01]"), m_VOL_PUCD01);
	RFX_Text(pFX, _T("[VOL_PUCD02]"), m_VOL_PUCD02);
	RFX_Text(pFX, _T("[VOL_PUCD03]"), m_VOL_PUCD03);
	RFX_Text(pFX, _T("[VOL_PUCD04]"), m_VOL_PUCD04);
	RFX_Text(pFX, _T("[VOL_PUCD05]"), m_VOL_PUCD05);
	RFX_Text(pFX, _T("[VOL_PUCD06]"), m_VOL_PUCD06);
	RFX_Text(pFX, _T("[VOL_PUCD07]"), m_VOL_PUCD07);
	RFX_Text(pFX, _T("[VOL_PUCD08]"), m_VOL_PUCD08);
	RFX_Text(pFX, _T("[VOL_PUCD09]"), m_VOL_PUCD09);
	RFX_Text(pFX, _T("[VOL_PUCD10]"), m_VOL_PUCD10);
	RFX_Byte(pFX, _T("[VOL_HFSW]"), m_VOL_HFSW);
	RFX_Text(pFX, _T("[VOL_HKJC]"), m_VOL_HKJC);
	RFX_Int(pFX, _T("[VOL_HTUK01]"), m_VOL_HTUK01);
	RFX_Int(pFX, _T("[VOL_HTUK02]"), m_VOL_HTUK02);
	RFX_Int(pFX, _T("[VOL_HTUK03]"), m_VOL_HTUK03);
	RFX_Int(pFX, _T("[VOL_HTUK04]"), m_VOL_HTUK04);
	RFX_Int(pFX, _T("[VOL_HTUK05]"), m_VOL_HTUK05);
	RFX_Int(pFX, _T("[VOL_HTUK06]"), m_VOL_HTUK06);
	RFX_Int(pFX, _T("[VOL_HTUK07]"), m_VOL_HTUK07);
	RFX_Int(pFX, _T("[VOL_HTUK08]"), m_VOL_HTUK08);
	RFX_Int(pFX, _T("[VOL_HTUK09]"), m_VOL_HTUK09);
	RFX_Int(pFX, _T("[VOL_HTUK10]"), m_VOL_HTUK10);
	RFX_Int(pFX, _T("[VOL_HTUK11]"), m_VOL_HTUK11);
	RFX_Int(pFX, _T("[VOL_HTUK12]"), m_VOL_HTUK12);
	RFX_Int(pFX, _T("[VOL_HTUK13]"), m_VOL_HTUK13);
	RFX_Int(pFX, _T("[VOL_HTUK14]"), m_VOL_HTUK14);
	RFX_Int(pFX, _T("[VOL_HTUK15]"), m_VOL_HTUK15);
	RFX_Int(pFX, _T("[VOL_HTUK16]"), m_VOL_HTUK16);
	RFX_Byte(pFX, _T("[VOL_HCSW01]"), m_VOL_HCSW01);
	RFX_Byte(pFX, _T("[VOL_HCSW02]"), m_VOL_HCSW02);
	RFX_Byte(pFX, _T("[VOL_HCSW03]"), m_VOL_HCSW03);
	RFX_Byte(pFX, _T("[VOL_HCSW04]"), m_VOL_HCSW04);
	RFX_Byte(pFX, _T("[VOL_HCSW05]"), m_VOL_HCSW05);
	RFX_Byte(pFX, _T("[VOL_HCSW06]"), m_VOL_HCSW06);
	RFX_Byte(pFX, _T("[VOL_HCLQ01]"), m_VOL_HCLQ01);
	RFX_Byte(pFX, _T("[VOL_HCLQ02]"), m_VOL_HCLQ02);
	RFX_Byte(pFX, _T("[VOL_HCLQ03]"), m_VOL_HCLQ03);
	RFX_Byte(pFX, _T("[VOL_HCLQ04]"), m_VOL_HCLQ04);
	RFX_Byte(pFX, _T("[VOL_FKSW01]"), m_VOL_FKSW01);
	RFX_Byte(pFX, _T("[VOL_FKSW02]"), m_VOL_FKSW02);
	RFX_Byte(pFX, _T("[VOL_FKSW03]"), m_VOL_FKSW03);
	RFX_Int(pFX, _T("[VOL_FKKM]"), m_VOL_FKKM);
	RFX_Int(pFX, _T("[VOL_FKKT]"), m_VOL_FKKT);
	RFX_Int(pFX, _T("[VOL_PRKM]"), m_VOL_PRKM);
	RFX_Int(pFX, _T("[VOL_PRKT]"), m_VOL_PRKT);
	RFX_Byte(pFX, _T("[VOL_PRSW01]"), m_VOL_PRSW01);
	RFX_Byte(pFX, _T("[VOL_PRSW02]"), m_VOL_PRSW02);
	RFX_Byte(pFX, _T("[VOL_PRSW03]"), m_VOL_PRSW03);
	RFX_Byte(pFX, _T("[VOL_DMSW01]"), m_VOL_DMSW01);
	RFX_Byte(pFX, _T("[VOL_DMSW02]"), m_VOL_DMSW02);
	RFX_Byte(pFX, _T("[VOL_DMSW03]"), m_VOL_DMSW03);
	RFX_Byte(pFX, _T("[VOL_DMSW04]"), m_VOL_DMSW04);
	RFX_Byte(pFX, _T("[VOL_DMSW05]"), m_VOL_DMSW05);
	RFX_Byte(pFX, _T("[VOL_DMSW06]"), m_VOL_DMSW06);
	RFX_Byte(pFX, _T("[VOL_DMSW07]"), m_VOL_DMSW07);
	RFX_Byte(pFX, _T("[VOL_DMSW08]"), m_VOL_DMSW08);
	RFX_Byte(pFX, _T("[VOL_DMSW09]"), m_VOL_DMSW09);
	RFX_Byte(pFX, _T("[VOL_DMSW10]"), m_VOL_DMSW10);
	RFX_Byte(pFX, _T("[VOL_DMNO01]"), m_VOL_DMNO01);
	RFX_Byte(pFX, _T("[VOL_DMNO02]"), m_VOL_DMNO02);
	RFX_Byte(pFX, _T("[VOL_DMNO03]"), m_VOL_DMNO03);
	RFX_Byte(pFX, _T("[VOL_DMNO04]"), m_VOL_DMNO04);
	RFX_Byte(pFX, _T("[VOL_DMNO05]"), m_VOL_DMNO05);
	RFX_Text(pFX, _T("[VOL_DMNM01]"), m_VOL_DMNM01);
	RFX_Text(pFX, _T("[VOL_DMNM02]"), m_VOL_DMNM02);
	RFX_Text(pFX, _T("[VOL_DMNM03]"), m_VOL_DMNM03);
	RFX_Text(pFX, _T("[VOL_DMNM04]"), m_VOL_DMNM04);
	RFX_Text(pFX, _T("[VOL_DMNM05]"), m_VOL_DMNM05);
	RFX_Long(pFX, _T("[REG_DATE]"), m_REG_DATE);
	RFX_Long(pFX, _T("[REG_TIME]"), m_REG_TIME);
	RFX_Long(pFX, _T("[REG_USER]"), m_REG_USER);
	RFX_Long(pFX, _T("[UPD_DATE]"), m_UPD_DATE);
	RFX_Long(pFX, _T("[UPD_TIME]"), m_UPD_TIME);
	RFX_Long(pFX, _T("[UPD_USER]"), m_UPD_USER);

}
/////////////////////////////////////////////////////////////////////////////
// CKJVOL 診断

#ifdef _DEBUG
void CKJVOL::AssertValid() const
{
	CRecordset::AssertValid();
}

void CKJVOL::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


