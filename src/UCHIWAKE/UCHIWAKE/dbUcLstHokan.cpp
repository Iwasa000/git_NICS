// dbUcLstHokan.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
//#include "UCHIWAKE.h"
#include "dbUcLstHokan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CdbUcLstHokan

IMPLEMENT_DYNAMIC(CdbUcLstHokan, CdbBase)

CdbUcLstHokan::CdbUcLstHokan(CDatabase* pdb)
	: CdbBase(pdb)
{
	//{{AFX_FIELD_INIT(CdbUcLstHokan)
	m_FormSeq	= 0;
	m_Seq		= 0;
	m_OrderNum	= 0;
	m_nFields	= 3;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	m_lo_pdb = pdb;								// データベースポインタを保持
	m_lo_TableName = STR_TBL_NAME_LST_HOKAN;		// テーブル名を記述
}


CString CdbUcLstHokan::GetDefaultSQL()
{
	return _T("[dbo].[uc_lst_hokan]");
}

void CdbUcLstHokan::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CdbUcLstHokan)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Int(pFX, _T("[FormSeq]"), m_FormSeq);
	RFX_Long(pFX, _T("[Seq]"), m_Seq);
	RFX_Long(pFX, _T("[OrderNum]"), m_OrderNum);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CdbUcLstHokan 診断

#ifdef _DEBUG
void CdbUcLstHokan::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbUcLstHokan::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

/**********************************************************************
	RequeryFormSeq()
		様式シーケンス番号 指定バージョン

	引数
			BYTE	FormSeq		様式シーケンス番号
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstHokan::RequeryFormSeq(int FormSeq)
{
	///////////////////////////////////////////////////////////////////
	// 目的のレコード（1レコード）を抽出する
	m_strFilter.Format("FormSeq = %d", FormSeq);
	m_strSort = "OrderNum ASC";

	return	RequeryWork();
}

/**********************************************************************
	RequerySeqFormSeq()
		様式シーケンス番号＆レコードシーケンス 指定バージョン

	引数
			int		FormSeq		様式シーケンス番号
			int		Seq			レコードシーケンス番号

	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstHokan::RequerySeqFormSeq(int nFormSeq, int Seq)
{
	m_strFilter.Format("FormSeq = %d AND Seq = %d", nFormSeq, Seq);
	m_strSort = "";

	return	RequeryWork();
}

/**********************************************************************
	CreateNewRecord()
		新規レコードを作成、初期値を追加
		レコードセットには、新規レコードがセットされて戻ります

	引数
			なし
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstHokan::CreateNewRecord()
{
	CString		SqlStr;
	int			retVal = DB_ERR;

	/////////////////////////////////////////////////////////////////////
	//	暫定バージョンです
	/////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////
	// 新規レコード追加作業

	int	ret = RequeryWork();
	if ( ret != DB_ERR_OK) {
		return ret;
	}

	AddNew();

	m_FormSeq	= 0;
	m_Seq		= 0;
	m_OrderNum	= 0;

	Update();

	m_strFilter.Format( "Seq = %d", 0 );		// 削除フラグ OFF（0:有効なレコード)
	m_strSort   = "";							// 並び順でソート

	return RequeryWork();
}

/**********************************************************************
	DeleteFormSeq()
		指定された様式シーケンスの保管データを全て削除する

	引数
			int					様式シーケンス
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstHokan::DeleteFormSeq(int nFormSeq)
{
	CString strSQL;

	strSQL.Format(_T("DELETE FROM [uc_lst_hokan] WHERE FormSeq = %d"), nFormSeq);

	return ExecuteSQLWork(strSQL);
}

/**********************************************************************
	GetCountFormSeq()
		指定された様式シーケンスのレコード数を返す
		未使用

	引数
			int					様式シーケンス
	戻値
			int					レコード数
***********************************************************************/
//int CdbUcLstHokan::GetCountFormSeq(int nFormSeq)
//{
//	int			ret = -1;
//	CString		sql, strCnt;
//	CRecordset  rs(m_lo_pdb);
//
//	sql.Format("SELECT COUNT(*) AS cnt FROM [uc_lst_hokan] WHERE FormSeq = %d", nFormSeq);
//
//	if ( !rs.Open( CRecordset::forwardOnly, sql ) ) {
//		rs.Close();
//		return DB_ERR_OPEN;
//	}
//
//	// 取得
//	rs.GetFieldValue( _T("cnt"), strCnt );
//	sscanf_s( strCnt, "%d", &ret);
//
//	if( strCnt.IsEmpty() != FALSE ){
//		ret = -1;
//	}
//
//	rs.Close();
//	return ret;
//}