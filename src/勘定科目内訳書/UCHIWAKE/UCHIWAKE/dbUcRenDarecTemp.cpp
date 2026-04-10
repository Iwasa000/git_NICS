// dbUcRenDarecTemp.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
//#include "UCHIWAKE.h"
#include "dbUcRenDarecTemp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CdbUcRenDarecTemp

IMPLEMENT_DYNAMIC(CdbUcRenDarecTemp, CdbBase)

CdbUcRenDarecTemp::CdbUcRenDarecTemp(CDatabase* pdb)
	: CdbBase(pdb)
	, m_SeqUserStartNum(1)		//Seq(ユーザ利用番号開始値)
{
	//{{AFX_FIELD_INIT(CdbUcRenDarecTemp)
	m_seq = 0;
	m_ddate = 0;
	m_dbt = _T("");
	m_cre = _T("");
	m_val = _T("");
	m_tekiyo = _T("");
	m_FgConv = 0;
// インボイス登録番号追加対応_23/11/27 del -->
	//m_nFields = 7;
// インボイス登録番号追加対応_23/11/27 del <--
// インボイス登録番号追加対応_23/11/27 add -->
	m_invono = _T("");
	m_nFields = 8;
// インボイス登録番号追加対応_23/11/27 add <--
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	m_lo_pdb = pdb;									// データベースポインタを保持
	m_lo_TableName = STR_TBL_NAME_REN_DAREC_TEMP;	// テーブル名を記述
}


CString CdbUcRenDarecTemp::GetDefaultSQL()
{
	return _T("[dbo].[uc_ren_darec_temp]");
}

void CdbUcRenDarecTemp::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CdbUcRenDarecTemp)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[seq]"), m_seq);
	RFX_Long(pFX, _T("[ddate]"), m_ddate);
	RFX_Text(pFX, _T("[dbt]"), m_dbt);
	RFX_Text(pFX, _T("[cre]"), m_cre);
	RFX_Text(pFX, _T("[val]"), m_val);
	RFX_Text(pFX, _T("[tekiyo]"), m_tekiyo);
	RFX_Text(pFX, _T("[invno]"), m_invono);		// インボイス登録番号追加対応_23/11/27 add
	RFX_Byte(pFX, _T("[FgConv]"), m_FgConv);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CdbUcRenDarecTemp 診断

#ifdef _DEBUG
void CdbUcRenDarecTemp::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbUcRenDarecTemp::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

/**********************************************************************
	Init()
		初期化処理（Open()の代わりとして使用）

	引数
			long	inSeq		シーケンス番号
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcRenDarecTemp::Init(long inSeq)
{
	return	RequerySeq(inSeq);
}

/**********************************************************************
	RequerySeq()
		シーケンス番号の一致するレコード取得

	引数
			long	inSeq		シーケンス番号
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcRenDarecTemp::RequerySeq(long inSeq)
{
	///////////////////////////////////////////////////////////////////
	// 目的のレコード（1レコード）を抽出する
	m_strFilter.Format("Seq = %d", inSeq);
	m_strSort = "";

	return	RequeryWork();
}

/**********************************************************************
	RequeryFgConvLikeTekiyo()
		摘要欄を文字列検索する（データ取込済みは対象外）

	引数
			BYTE	inFgConv	データ取込フラグ
			CString	inTekiyo	摘要欄の検索文字列
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcRenDarecTemp::RequeryFgConvLikeTekiyo(BYTE inFgConv, CString inTekiyo)
{
	// SQLコマンドでは、名称に[']が含まれている場合['']とする
	// （摘要リストテーブルに[']付きで登録された場合の対応）
	CString		strTekiyo = ConvSqlString(inTekiyo);

	///////////////////////////////////////////////////////////////////
	// 目的のレコードを抽出する
	m_strFilter.Format("(FgConv = %d) AND (tekiyo LIKE '%%%s%%' COLLATE Japanese_CS_AS_KS_WS)", inFgConv, strTekiyo);
	m_strSort = "seq ASC";

	return	RequeryWork();
}

/**********************************************************************
	RequeryFgConv()
		データ取込フラグの一致するデータを取得

	引数
			BYTE	inFgConv	データ取込フラグ
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcRenDarecTemp::RequeryFgConv(BYTE inFgConv)
{
	///////////////////////////////////////////////////////////////////
	// 目的のレコードを抽出する
	m_strFilter.Format("FgConv = %d", inFgConv);
	m_strSort = "seq ASC";

	return	RequeryWork();
}

/**********************************************************************
	CreateNewRecord()
		新規レコードを作成、初期値を追加
		レコードセットには、新規レコードがセットされて戻ります

	引数
			inSeq				シーケンス番号
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcRenDarecTemp::CreateNewRecord(long inSeq)
{
	int			retVal = DB_ERR;

	// シーケンス番号（キー）の重複チェック
	if (RequerySeq(inSeq) != DB_ERR_OK) {
		return retVal;
	}
	if (m_seq == inSeq) {
		return retVal;
	}

	/////////////////////////////////////////////////////////////////////
	// 新規レコード追加作業
	int	ret = RequeryWork();
	if ( ret != DB_ERR_OK) {
		return ret;
	}

	AddNew();
	m_seq = inSeq;				// シーケンス番号を自動付番
	m_ddate = 0;				// 日付
	m_dbt = _T("");				// 借方発生科目
	m_cre = _T("");				// 貸方発生科目
	m_val = _T("");				// 発生金額
	m_tekiyo = _T("");			// 摘要
	m_invono = _T("");			// 登録番号		// インボイス登録番号追加対応_23/11/27 add
	m_FgConv = 0;				// データ取込フラグ
	Update();

	m_strFilter.Format("seq = %d", inSeq);		// 追加レコードを抽出
	m_strSort   = "";							// 並び順でソート

	return RequeryWork();
}

/**********************************************************************
	DeleteAllRecord()
		テーブル内の全レコードを削除

	引数
			なし
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcRenDarecTemp::DeleteAllRecord()
{
	CString		SqlStr;		// SQL全文

	///////////////////////////////////////////////////////////////
	// SQL 作成
	SqlStr =	_T("DELETE FROM ") + m_lo_TableName;

	///////////////////////////////////////////////////////////////
	// SQL 実行
	return	ExecuteSQLWork(SqlStr);
}
