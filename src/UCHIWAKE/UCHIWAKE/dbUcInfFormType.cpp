// dbUcInfFormType.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
//#include "UCHIWAKE.h"
#include "dbUcInfFormType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CdbUcInfFormType

IMPLEMENT_DYNAMIC(CdbUcInfFormType, CdbBase)

CdbUcInfFormType::CdbUcInfFormType(CDatabase* pdb)
	: CdbBase(pdb)
{
	//{{AFX_FIELD_INIT(CdbUcInfFormType)
	m_FormNo = 0;
	m_FormType = 0;
	m_nFields = 2;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	m_lo_pdb = pdb;								// データベースポインタを保持
	m_lo_TableName = STR_TBL_NAME_INF_FORM_TYPE;	// テーブル名を記述
}


CString CdbUcInfFormType::GetDefaultSQL()
{
	return _T("[dbo].[uc_inf_form_type]");
}

void CdbUcInfFormType::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CdbUcInfFormType)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Byte(pFX, _T("[FormNo]"), m_FormNo);
	RFX_Byte(pFX, _T("[FormType]"), m_FormType);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CdbUcInfFormType 診断

#ifdef _DEBUG
void CdbUcInfFormType::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbUcInfFormType::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

/**********************************************************************
	Init()
		初期化処理（Open()の代わりとして使用）

	引数
			int		inFormNo	様式番号(先頭の丸番号)
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcInfFormType::Init(int inFormNo)
{
	return RequeryForm(inFormNo);
}

/**********************************************************************
	InitOrder()

	引数
			BOOL	SortType	1:昇順（デフォルト）
								0:降順
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcInfFormType::InitOrder(BOOL SortType)
{
	m_strFilter.Empty();
	m_strSort = "FormNo ASC";

	return RequeryWork();
}

/**********************************************************************
	RequeryForm()
		初期化処理（Open()の代わりとして使用）

	引数
			int		inFormNo	様式番号(先頭の丸番号)
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcInfFormType::RequeryForm(int inFormNo)
{
	///////////////////////////////////////////////////////////////////
	// 目的のレコード（1レコード）を抽出する
	m_strFilter.Format("FormNo = %d", inFormNo);
	m_strSort = "";

	return RequeryWork();
}

