// dbUcInfYokuki.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
//#include "UCHIWAKE.h"
#include "dbUcInfYokuki.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CdbUcInfYokuki

IMPLEMENT_DYNAMIC(CdbUcInfYokuki, CdbBase)

CdbUcInfYokuki::CdbUcInfYokuki(CDatabase* pdb)
	: CdbBase(pdb)
{
	//{{AFX_FIELD_INIT(CdbUcInfYokuki)
	m_FormSeq = 0;
	m_OpYokukiFg = 0;
	m_OpYokuki = 0;
	m_OpClearFg = 0;
	m_OpClear = 0;
	m_OpHokanFg = 0;
	m_OpHokan = 0;
// midori 152387 del -->
//	m_nFields = 7;
// midori 152387 del <--
// midori 152387 add -->
	m_OpNullClearFg = 0;
	m_OpNullClear = 0;
	m_nFields = 9;
// midori 152387 add <--
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	m_lo_pdb = pdb;								// データベースポインタを保持
	m_lo_TableName = STR_TBL_NAME_INF_YOKUKI;	// テーブル名を記述
}

CString CdbUcInfYokuki::GetDefaultSQL()
{
	return _T("[dbo].[uc_inf_yokuki]");
}

void CdbUcInfYokuki::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CdbUcInfYokuki)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Int(pFX, _T("[FormSeq]"), m_FormSeq);
	RFX_Byte(pFX, _T("[OpYokukiFg]"), m_OpYokukiFg);
	RFX_Byte(pFX, _T("[OpYokuki]"), m_OpYokuki);
	RFX_Byte(pFX, _T("[OpClearFg]"), m_OpClearFg);
	RFX_Byte(pFX, _T("[OpClear]"), m_OpClear);
	RFX_Byte(pFX, _T("[OpHokanFg]"), m_OpHokanFg);
	RFX_Byte(pFX, _T("[OpHokan]"), m_OpHokan);
// midori 152387 add -->
	RFX_Byte(pFX, _T("[OpNullClearFg]"), m_OpNullClearFg);
	RFX_Byte(pFX, _T("[OpNullClear]"), m_OpNullClear);
// midori 152387 add <--
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CdbUcInfYokuki 診断

#ifdef _DEBUG
void CdbUcInfYokuki::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbUcInfYokuki::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

/**********************************************************************
	Init()
		初期化処理（Open()の代わりとして使用）

	引数
			int		inFormSeq	様式シーケンス番号
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcInfYokuki::Init(int inFormSeq)
{
	return	RequeryFormSeq(inFormSeq);
}

/**********************************************************************
	RequeryFormSeq()
		様式シーケンス番号の一致するレコード取得

	引数
			int		inFormSeq	様式シーケンス番号
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcInfYokuki::RequeryFormSeq(int inFormSeq)
{
	///////////////////////////////////////////////////////////////////
	// 目的のレコード（1レコード）を抽出する
	m_strFilter.Format("FormSeq = %d", inFormSeq);
	m_strSort = "";

	return	RequeryWork();
}

/**********************************************************************
		様式シーケンス番号の一致するレコード取得

	引数
			BOOL	ソートタイプ	TRUE:昇順
									FALSE:降順
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcInfYokuki::InitSort(BOOL SortType)
{
	CString strSort;
	if(SortType != FALSE)	strSort = _T("ASC");
	else					strSort = _T("DESC");

	m_strSort.Format("FormSeq %s", strSort);

	return	RequeryWork();
}