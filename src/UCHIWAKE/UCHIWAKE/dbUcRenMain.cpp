// dbUcRenMain.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
//#include "UCHIWAKE.h"
#include "dbUcRenMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CdbUcRenMain

IMPLEMENT_DYNAMIC(CdbUcRenMain, CdbBase)

CdbUcRenMain::CdbUcRenMain(CDatabase* pdb)
	: CdbBase(pdb)
{
	//{{AFX_FIELD_INIT(CdbUcRenMain)
	m_FormSeq = 0;
	m_OpRenType = 0;
	m_OpRenTypeKz = 0;
	m_OpRenTypeEz = 0;
	m_OpRenTypeDa = 0;
	m_OpRenTypeKs = 0;
	m_OpTkcodFrom = 0;
	m_OpTkcodTo = 0;
	m_OpOutZeroEn = 0;
	m_OpGassan = 0;
	m_OpUpdType = 0;
	m_OpOldRenType = 0;
// midori 152153 del -->
//// midori 161110 del -->
//	//m_nFields = 12;
//// midori 161110 del <--
//// midori 161110 add -->
//	m_OpEdaren = 0;
//	m_nFields = 13;
//// midori 161110 add <--
// midori 152153 del <--
// midori 152153 add -->
	m_OpEdaren = 0;
	m_OpKmkname = 0;
	m_OpF9Upd = 0;
	m_nFields = 15;
// midori 152153 add <--
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	m_lo_pdb = pdb;								// データベースポインタを保持
	m_lo_TableName = STR_TBL_NAME_REN_MAIN;		// テーブル名を記述
}

CString CdbUcRenMain::GetDefaultSQL()
{
	return _T("[dbo].[uc_ren_main]");
}

void CdbUcRenMain::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CdbUcRenMain)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Int(pFX, _T("[FormSeq]"), m_FormSeq);
	RFX_Int(pFX, _T("[OpRenType]"), m_OpRenType);
	RFX_Byte(pFX, _T("[OpRenTypeKz]"), m_OpRenTypeKz);
	RFX_Byte(pFX, _T("[OpRenTypeEz]"), m_OpRenTypeEz);
	RFX_Byte(pFX, _T("[OpRenTypeDa]"), m_OpRenTypeDa);
	RFX_Byte(pFX, _T("[OpRenTypeKs]"), m_OpRenTypeKs);
	RFX_Long(pFX, _T("[OpTkcodFrom]"), m_OpTkcodFrom);
	RFX_Long(pFX, _T("[OpTkcodTo]"), m_OpTkcodTo);
	RFX_Byte(pFX, _T("[OpOutZeroEn]"), m_OpOutZeroEn);
	RFX_Byte(pFX, _T("[OpGassan]"), m_OpGassan);
	RFX_Byte(pFX, _T("[OpUpdType]"), m_OpUpdType);
	RFX_Int(pFX, _T("[OpOldRenType]"), m_OpOldRenType);
// midori 161110 add -->
	RFX_Byte(pFX, _T("[OpEdaren]"), m_OpEdaren);
// midori 161110 add <--
// midori 152153 add -->
	RFX_Byte(pFX, _T("[OpKmkname]"), m_OpKmkname);
	RFX_Byte(pFX, _T("[OpF9Upd]"), m_OpF9Upd);
// midori 152153 add <--
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CdbUcRenMain 診断

#ifdef _DEBUG
void CdbUcRenMain::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbUcRenMain::Dump(CDumpContext& dc) const
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
int CdbUcRenMain::Init(int inFormSeq)
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
int CdbUcRenMain::RequeryFormSeq(int inFormSeq)
{
	///////////////////////////////////////////////////////////////////
	// 目的のレコード（1レコード）を抽出する
	m_strFilter.Format("FormSeq = %d", inFormSeq);
	m_strSort = "";

	return	RequeryWork();
}
