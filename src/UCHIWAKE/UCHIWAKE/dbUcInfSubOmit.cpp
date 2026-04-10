// dbUcInfSubOmit.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
//#include "UCHIWAKE.h"
#include "dbUcInfSubOmit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CdbUcInfSubOmit

IMPLEMENT_DYNAMIC(CdbUcInfSubOmit, CdbBase)

CdbUcInfSubOmit::CdbUcInfSubOmit(CDatabase* pdb)
	: CdbBase(pdb)
{
	//{{AFX_FIELD_INIT(CdbUcInfSubOmit)
	m_FormSeq = 0;
	m_ItemName = _T("");
	m_ItemSeq = 0;
	m_Omit = 0;
	m_OmitStr = _T("");
	m_OmitMaxLen = 0;
	m_nFields = 6;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	m_lo_pdb = pdb;								// データベースポインタを保持
	m_lo_TableName = STR_TBL_NAME_INF_SUB_OMIT;	// テーブル名を記述
}


CString CdbUcInfSubOmit::GetDefaultSQL()
{
	return _T("[dbo].[uc_inf_sub_omit]");
}

void CdbUcInfSubOmit::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CdbUcInfSubOmit)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Int(pFX, _T("[FormSeq]"), m_FormSeq);
	RFX_Text(pFX, _T("[ItemName]"), m_ItemName);
	RFX_Byte(pFX, _T("[ItemSeq]"), m_ItemSeq);
	RFX_Byte(pFX, _T("[Omit]"), m_Omit);
	RFX_Text(pFX, _T("[OmitStr]"), m_OmitStr);
	RFX_Int(pFX, _T("[OmitMaxLen]"), m_OmitMaxLen);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CdbUcInfSubOmit 診断

#ifdef _DEBUG
void CdbUcInfSubOmit::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbUcInfSubOmit::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


/**********************************************************************
	Init()
		初期化処理（Open()の代わりとして使用）

	引数
			BYTE	FormSeq		様式番号
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcInfSubOmit::Init(int inFormSeq)
{
	///////////////////////////////////////////////////////////////////
	// 目的のレコードを抽出する
	m_strFilter.Format("FormSeq = %d", inFormSeq);
	m_strSort = "ItemSeq";

	return RequeryWork();
}

