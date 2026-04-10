// dbUcInfSonotaItem.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
//#include "uchiwake.h"
#include "dbUcInfSonotaItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CdbUcInfSonotaItem

IMPLEMENT_DYNAMIC(CdbUcInfSonotaItem, CdbBase)

CdbUcInfSonotaItem::CdbUcInfSonotaItem(CDatabase* pdb)
	: CdbBase(pdb)
{
	//{{AFX_FIELD_INIT(CdbUcInfSonotaItem)
	m_FormSeq = 0;
	m_ItemName = _T("");
	m_nFields = 2;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	m_lo_pdb = pdb;								// データベースポインタを保持
	m_lo_TableName = STR_TBL_NAME_INF_SONOTA_ITEM;// テーブル名を記述
}


CString CdbUcInfSonotaItem::GetDefaultSQL()
{
	return _T("[dbo].[uc_inf_sonota_item]");
}

void CdbUcInfSonotaItem::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CdbUcInfSonotaItem)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Int(pFX, _T("[FormSeq]"), m_FormSeq);
	RFX_Text(pFX, _T("[ItemName]"), m_ItemName);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CdbUcInfSonotaItem 診断

#ifdef _DEBUG
void CdbUcInfSonotaItem::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbUcInfSonotaItem::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

/**********************************************************************
	Init()
		初期化処理（Open()の代わりとして使用）
		様式並び順番号(表示インデックス) 指定バージョン

	引数
			int		FormSeq		様式シーケンス番号
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcInfSonotaItem::Init(int FormSeq)
{
	return RequeryFormSeq(FormSeq);
}


/**********************************************************************
	RequeryFormSeq()
		初期化処理（Open()の代わりとして使用）
		様式シーケンス番号 指定バージョン

	引数
			int		FormSeq		様式シーケンス番号
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcInfSonotaItem::RequeryFormSeq(int FormSeq)
{
	///////////////////////////////////////////////////////////////////
	// 目的のレコード（1レコード）を抽出する
	m_strFilter.Format("FormSeq = %d", FormSeq);
	m_strSort = "";

	return	RequeryWork();
}
