// dbUcLstItemSort.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
//#include "UCHIWAKE.h"
#include "dbUcLstItemSort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CdbUcLstItemSort

IMPLEMENT_DYNAMIC(CdbUcLstItemSort, CdbBase)

CdbUcLstItemSort::CdbUcLstItemSort(CDatabase* pdb)
	: CdbBase(pdb)
{
	//{{AFX_FIELD_INIT(CdbUcLstItemSort)
	m_FormSeq = 0;
	m_ItemSeq = 0;
	m_ItemName = _T("");
	m_FgPage = 0;
	m_FgSykei = 0;
	m_FgItem = 0;
	m_OrderStr = _T("");
	m_FieldSykei = _T("");
	m_FieldSykei2 = _T("");
// midori 152137 del -->
//	m_nFields = 9;
// midori 152137 del <--
// midori 152137 add -->
	m_OrderStr2 = _T("");
	m_nFields = 10;
// midori 152137 add <--
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	m_lo_pdb = pdb;								// データベースポインタを保持
	m_lo_TableName = STR_TBL_NAME_LST_ITEM_SORT;	// テーブル名を記述
}

CString CdbUcLstItemSort::GetDefaultSQL()
{
	return _T("[dbo].[uc_lst_item_sort]");
}

void CdbUcLstItemSort::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CdbUcLstItemSort)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Byte(pFX, _T("[FgPage]"), m_FgPage);
	RFX_Text(pFX, _T("[FieldSykei]"), m_FieldSykei);
	RFX_Int(pFX, _T("[FormSeq]"), m_FormSeq);
	RFX_Text(pFX, _T("[ItemName]"), m_ItemName);
	RFX_Byte(pFX, _T("[ItemSeq]"), m_ItemSeq);
	RFX_Text(pFX, _T("[OrderStr]"), m_OrderStr);
	RFX_Byte(pFX, _T("[FgItem]"), m_FgItem);
	RFX_Byte(pFX, _T("[FgSykei]"), m_FgSykei);
	RFX_Text(pFX, _T("[FieldSykei2]"), m_FieldSykei2);
// midori 152137 add -->
	RFX_Text(pFX, _T("[OrderStr2]"), m_OrderStr2);
// midori 152137 add <--
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CdbUcLstItemSort 診断

#ifdef _DEBUG
void CdbUcLstItemSort::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbUcLstItemSort::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

/**********************************************************************
	Init()
		初期化処理（Open()の代わりとして使用）

		指定された「様式シーケンス番号」に属する「項目」を抽出する

	引数
			int		FormSeq		様式シーケンス番号
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstItemSort::Init(int FormSeq)
{
	return	RequeryForm(FormSeq);
}


/**********************************************************************
	RequeryForm()

		初期化処理（Open()の代わりとして使用）

		指定された「様式シーケンス番号」に属する「項目」を抽出する
		並び順は、項目番号(ItemSeq)の小さい値順

	引数
			int		FormSeq		様式シーケンス番号
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstItemSort::RequeryForm(int FormSeq)
{
	///////////////////////////////////////////////////////////////////
	// 目的のレコードを抽出する
	m_strFilter.Format("FormSeq = %d", FormSeq);	// 様式で抽出
	m_strSort = "ItemSeq";							// 項目番号 順

	return	RequeryWork();
}

/**********************************************************************
	RequeryFormItem()
		様式シーケンス番号＋項目番号を指定してデータを取得

	引数
			int					様式シーケンス番号
			int					項目番号
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstItemSort::RequeryFormItem(int FormSeq, int ItemSeq)
{
	m_strFilter.Format("FormSeq = %d AND ItemSeq = %d", FormSeq, ItemSeq);
	m_strSort = "FormSeq, ItemSeq";

	return	RequeryWork();
}
