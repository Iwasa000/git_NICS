// dbUcInfVer.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
//#include "uchiwake.h"
#include "dbUcInfVer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CdbUcInfVer

IMPLEMENT_DYNAMIC(CdbUcInfVer, CdbBase)

CdbUcInfVer::CdbUcInfVer(CDatabase* pdb)
	: CdbBase(pdb)
{
	//{{AFX_FIELD_INIT(CdbUcInfVer)
	m_Seq = 0;
	m_Version = 0;
	m_nFields = 2;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	m_lo_pdb = pdb;								// データベースポインタを保持
	m_lo_TableName = STR_TBL_NAME_INF_VER;		// テーブル名を記述
	m_Version = -1;								// -1 に初期化
}


CString CdbUcInfVer::GetDefaultSQL()
{
	return _T("[dbo].[uc_inf_ver]");
}

void CdbUcInfVer::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CdbUcInfVer)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Int(pFX, _T("[Seq]"), m_Seq);
	RFX_Int(pFX, _T("[Version]"), m_Version);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CdbUcInfVer 診断

#ifdef _DEBUG
void CdbUcInfVer::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbUcInfVer::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

/**********************************************************************
	VersionCheck()
		内訳書テーブル群のバージョンを調べる

	引数
			int		inVer
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcInfVer::VersionCheck(int inVer)
{
	int	retVal;

	m_strFilter = "Seq = 1";		
	m_strSort   = "";

	if ( ! OpenEx() ) {
		return DB_ERR_OPEN;		// オープンに失敗	
	}

	retVal = m_Version;		// バージョンが異なる場合は、現在のバージョンを返す
	if ( !IsEOF() ) {
		if ( m_Version == inVer) {	// バージョン OK
			retVal = DB_ERR_OK;		// 0：成功
		}
	}

	Close();
	return retVal;

}
