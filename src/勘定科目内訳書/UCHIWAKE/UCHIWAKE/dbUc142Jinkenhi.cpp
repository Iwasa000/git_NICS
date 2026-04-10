// dbUc142Jinkenhi.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
//#include "UCHIWAKE.h"
#include "dbUc142Jinkenhi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CdbUc142Jinkenhi

IMPLEMENT_DYNAMIC(CdbUc142Jinkenhi, CdbUc000Common)

CdbUc142Jinkenhi::CdbUc142Jinkenhi(CDatabase* pdb)
	: CdbUc000Common(pdb)
{
	//{{AFX_FIELD_INIT(CdbUc142Jinkenhi)
	m_Val = _T("");
	m_Val2 = _T("");
	m_Seq = 0;
	m_NumPage = 0;
	m_NumRow = 0;
	m_FgFunc = 0;
	m_FgShow = 0;
	m_KeiStr = _T("");
	m_NumGroup = 0;
	m_RenKcd = _T("");
	m_RenEcd = 0;
	m_RenFgTemp = 0;
	m_ShowKeiZero = 0;
	m_nFields = 13;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	m_lo_pdb = pdb;								// データベースポインタを保持
	m_lo_TableName = STR_TBL_NAME_142;			// テーブル名を記述
}


CString CdbUc142Jinkenhi::GetDefaultSQL()
{
	return _T("[dbo].[uc_142_jinkenhi]");
}

void CdbUc142Jinkenhi::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CdbUc142Jinkenhi)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[Val]"), m_Val);
	RFX_Text(pFX, _T("[Val2]"), m_Val2);
	RFX_Long(pFX, _T("[Seq]"), m_Seq);
	RFX_Int(pFX, _T("[NumPage]"), m_NumPage);
	RFX_Byte(pFX, _T("[NumRow]"), m_NumRow);
	RFX_Byte(pFX, _T("[FgFunc]"), m_FgFunc);
	RFX_Byte(pFX, _T("[FgShow]"), m_FgShow);
	RFX_Text(pFX, _T("[KeiStr]"), m_KeiStr);
	RFX_Long(pFX, _T("[NumGroup]"), m_NumGroup);
	RFX_Text(pFX, _T("[RenKcd]"), m_RenKcd);
	RFX_Long(pFX, _T("[RenEcd]"), m_RenEcd);
	RFX_Byte(pFX, _T("[RenFgTemp]"), m_RenFgTemp);
	RFX_Byte(pFX, _T("[ShowKeiZero]"), m_ShowKeiZero);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CdbUc142Jinkenhi 診断

#ifdef _DEBUG
void CdbUc142Jinkenhi::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbUc142Jinkenhi::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

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
int CdbUc142Jinkenhi::CreateNewRecord()
{
	CRecordset	rs(m_lo_pdb);
	CString		SqlStr;
	int			retVal = DB_ERR;

	/////////////////////////////////////////////////////////////////////
	//	暫定バージョンです
	/////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////
	// 新しいシーケンス番号を算出
	SqlStr =	_T("SELECT max(Seq) AS MaxNum ");
	SqlStr +=	_T("FROM ") + m_lo_TableName + _T(" ");

	if ( ! OpenEx( &rs , SqlStr , m_lo_TableName ) ) {
		return DB_ERR_OPEN;
	};

	retVal = DB_ERR_EOF;
	if (!rs.IsEOF()) {
		CDBVariant	val;
		rs.GetFieldValue(_T("MaxNum"), val);
		if (val.m_dwType == DBVT_NULL) {	// NULLだった場合
			retVal = 0;
		}
		else {								// 値の取得OK
//			retVal = val.m_iVal;
			retVal = val.m_lVal;
		}
	}
	rs.Close();

	retVal++;				//	自動付番シーケンス番号カウントアップ


	/////////////////////////////////////////////////////////////////////
	// 新規レコード追加作業

	int	ret = RequeryWork();
	if ( ret != DB_ERR_OK) {
		return ret;
	}

	AddNew();
							
	m_Seq = retVal;			// シーケンス番号を自動付番
	m_NumPage = 0;			// (0)  ページ番号
	m_NumRow = 0;			// (0)  行番号
	m_FgFunc = 0;			// (0)  空行
	m_FgShow = 0;			// (0)  表示
	m_KeiStr = _T("");		// ("") 計名称
	m_NumGroup = 0;			// (0)  グループ番号
	m_RenKcd = _T("");		// ("") 連動元情報(科目)
	m_RenEcd = 0;			// (0)  連動元情報(枝番)
	m_RenFgTemp = 0;		// (0)  連動テンポラリ情報

	m_Val = _T("");			// ("") 総額
	m_Val2 = _T("");		// ("") 総額のうち代表者...
	m_ShowKeiZero = 0;		// (0)  ０円計表示フラグ

	Update();

	m_strFilter.Format( "Seq = %d", retVal );	// 削除フラグ OFF（0:有効なレコード)
	m_strSort   = "";							// 並び順でソート

	return RequeryWork();
}

// midori 160606 cor -->
///**********************************************************************
//	UpdateYokukiZeroClear()
//		翌期更新のゼロクリアを行う
//
//	引数
//			なし
//	戻値
//			int					DB_ERR_OK (0)     ：成功
//								DB_ERR_OK (0) 以外：失敗
//***********************************************************************/
//int CdbUc142Jinkenhi::UpdateYokukiZeroClear()
//{
//	CString	sql = GetSQLUpdateYokukiZeroClear("Val");			// 総額
//	sql += GetSQLUpdateYokukiZeroClear("Val2");					// 総額のうち代表者及びその家族分
//	//CString		strCommand;
//	//CString		strTemp;
//
//	//// クリア対象項目（2項目）
//	//strCommand.Format("UPDATE %s SET ", m_lo_TableName);
//	//strCommand	+=	_T("Val = 0, ");						// 総額
//	//strCommand	+=	_T("Val2 = 0 ");						// 総額のうち代表者及びその家族分
//
//	//// 抽出条件：空行はNULLなので更新しない
//	//strTemp.Format("WHERE FgFunc <> %d", ID_FGFUNC_NULL);
//	//strCommand += strTemp;
//
//	return	ExecuteSQLWork(sql);
//}
// ---------------------
/**********************************************************************
	UpdateYokukiZeroClear()
		翌期更新のゼロクリアを行う

	引数
			int					0:ゼロクリア　1:空欄クリア
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUc142Jinkenhi::UpdateYokukiZeroClear( int pSw )
{
// midori 152388 add -->
	CString	cs=_T("");
// midori 152388 add <--
	CString	sql = GetSQLUpdateYokukiZeroClear("Val",pSw);			// 総額
	sql += GetSQLUpdateYokukiZeroClear("Val2",pSw);					// 総額のうち代表者及びその家族分
// midori 152388 add -->
	if(pSw != 0)	{
		cs.Format(_T("UPDATE %s SET FgFunc=%d WHERE FgFunc<=%d AND "),m_lo_TableName,ID_FGFUNC_NULL,ID_FGFUNC_DATA);
		cs += _T(" Val IS NULL AND Val2 IS NULL");
		sql += cs;
	}
// midori 152388 add <--
	return	ExecuteSQLWork(sql);
}
// midori 160606 cor <--

/**********************************************************************
	UpdateZeroMoneyMoveToHokan()
		「一括で一時保管へ移動」により、金額が０円 or 空欄(NULL)のデータを一時保管へ移動する

	引数
			HOKAN_DATA			保管データ
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUc142Jinkenhi::UpdateZeroMoneyMoveToHokan( HOKAN_DATA *hDat )
{
	// ⑭-2は一時保管の対象外
	return DB_ERR_OK;
}