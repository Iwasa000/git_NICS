// dbUc102Gensen.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
//#include "uchiwake.h"
#include "dbUc102Gensen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CdbUc102Gensen

IMPLEMENT_DYNAMIC(CdbUc102Gensen, CdbUc000Common)

CdbUc102Gensen::CdbUc102Gensen(CDatabase* pdb)
	: CdbUc000Common(pdb)
{
	//{{AFX_FIELD_INIT(CdbUc102Gensen)
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
	m_SpDate = 0;
	m_KnSeq = 0;
	m_KnOrder = 0;
	m_KnName = _T("");
	m_Val = _T("");
	m_ShowKeiZero = 0;
// midori 152137 del -->
//	m_nFields = 16;
// midori 152137 del <--
// midori 152137 add -->
	m_KnKana = _T("");
	m_nFields = 17;
// midori 152137 add <--
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	m_lo_pdb = pdb;								// データベースポインタを保持
	m_lo_TableName = STR_TBL_NAME_102;			// テーブル名を記述
}


CString CdbUc102Gensen::GetDefaultSQL()
{
	return _T("[dbo].[uc_102_gensen]");
}

void CdbUc102Gensen::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CdbUc102Gensen)
	pFX->SetFieldType(CFieldExchange::outputColumn);
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
	RFX_Long(pFX, _T("[SpDate]"), m_SpDate);
	RFX_Long(pFX, _T("[KnSeq]"), m_KnSeq);
	RFX_Long(pFX, _T("[KnOrder]"), m_KnOrder);
	RFX_Text(pFX, _T("[KnName]"), m_KnName);
	RFX_Text(pFX, _T("[Val]"), m_Val);
	RFX_Byte(pFX, _T("[ShowKeiZero]"), m_ShowKeiZero);
// midori 152137 add -->
	RFX_Text(pFX, _T("[KnKana]"), m_KnKana);
// midori 152137 add <--
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CdbUc102Gensen 診断

#ifdef _DEBUG
void CdbUc102Gensen::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbUc102Gensen::Dump(CDumpContext& dc) const
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
int CdbUc102Gensen::CreateNewRecord()
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

	m_SpDate = 0;			// (0)  年月日
	m_KnSeq = 0;			// (0)  科目：シーケンス番号
	m_KnOrder = 0;			// (0)  科目：順序番号
	m_KnName = _T("");		// ("") 科目：名称
	m_Val = _T("");			// ("") 期末現在高
	m_ShowKeiZero = 0;		// (0)  ０円計表示フラグ
// midori 152137 add -->
	m_KnKana = _T("");		// ("") 所得の種類(カナ)
// midori 152137 add <--

	Update();

	m_strFilter.Format( "Seq = %d", retVal );	// 削除フラグ OFF（0:有効なレコード)
	m_strSort   = "";							// 並び順でソート

	return RequeryWork();
}

// midori 152765 add -->
// 157153 del -->
///**********************************************************************
//	CreateNewRecord2()
//		新規レコードを作成、初期値を追加
//		レコードセットには、新規レコードがセットされて戻ります（出力形式変更で使用）
//
//	引数
//			なし
//	戻値
//			int					DB_ERR_OK (0)     ：成功
//								DB_ERR_OK (0) 以外：失敗
//***********************************************************************/
//int CdbUc102Gensen::CreateNewRecord2(int pNumPage,int pNumRow)
// 157153 del <--
// 157153 add -->
// ------------------------------------------------------------------------------------------------
//	CreateNewRecord2()
//		新規レコードを作成
//		レコードセットには、新規レコードがセットされて戻ります
//	引数
//			int			作成するレコードの頁番号
//			int			作成するレコードの行番号
//			int			作成する行の種類	0:空行を作成	1:小計NULL行を作成
//	戻値
//			int					DB_ERR_OK (0)     ：成功
//								DB_ERR_OK (0) 以外：失敗
//
// ------------------------------------------------------------------------------------------------
int CdbUc102Gensen::CreateNewRecord2(int pNumPage,int pNumRow,int pNullSw)
// 157153 add <--
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
			retVal = val.m_lVal;
		}
	}
	rs.Close();

	retVal++;				//	自動付番シーケンス番号カウントアップ


	/////////////////////////////////////////////////////////////////////
	// 新規レコード追加作業

	AddNew();
							
	m_Seq = retVal;			// シーケンス番号を自動付番
	m_NumPage = pNumPage;	// ページ番号
	m_NumRow = pNumRow;		// 行番号
// 157153 del -->
	//m_FgFunc = 0;			// (0)  空行
// 157153 del <--
// 157153 add -->
	// (0)  空行
	if(pNullSw == 0) {
		m_FgFunc = ID_FGFUNC_NULL;			
	}
	else {
		m_FgFunc = ID_FGFUNC_SYOKEINULL;
	}
// 157153 add <--
	m_FgShow = 0;			// (0)  表示
	m_KeiStr = _T("");		// ("") 計名称
	m_NumGroup = 0;			// (0)  グループ番号
	m_RenKcd = _T("");		// ("") 連動元情報(科目)
	m_RenEcd = 0;			// (0)  連動元情報(枝番)
	m_RenFgTemp = 0;		// (0)  連動テンポラリ情報

	m_SpDate = 0;			// (0)  年月日
	m_KnSeq = 0;			// (0)  科目：シーケンス番号
	m_KnOrder = 0;			// (0)  科目：順序番号
	m_KnName = _T("");		// ("") 科目：名称
	m_Val = _T("");			// ("") 期末現在高
	m_ShowKeiZero = 0;		// (0)  ０円計表示フラグ
	m_KnKana = _T("");		// ("") 所得の種類(カナ)

	Update();

	return(0);
}
// midori 152765 add <--

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
//int CdbUc102Gensen::UpdateYokukiZeroClear()
//{
//	CString	sql = GetSQLUpdateYokukiZeroClear("Val");			// 期末現在高
//	sql += GetSQLUpdateYokukiZeroClear("SpDate");				// 年月日
//	//CString		strCommand;
//	//CString		strTemp;
//
//	//// クリア対象項目（2項目）
//	//strCommand.Format("UPDATE %s SET ", m_lo_TableName);
//	//strCommand	+=	_T("SpDate = 0, ");						// 年月日
//	//strCommand	+=	_T("Val = 0 ");							// 期末現在高
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
int CdbUc102Gensen::UpdateYokukiZeroClear( int pSw )
{
// midori 152388 add -->
	CString	cs=_T("");
// midori 152388 add <--
	CString	sql = GetSQLUpdateYokukiZeroClear("Val",pSw);		// 期末現在高
	sql += GetSQLUpdateYokukiZeroClear("SpDate",0);				// 年月日
// midori 152388 add -->
	if(pSw != 0)	{
		cs.Format(_T("UPDATE %s SET FgFunc=%d WHERE FgFunc<=%d AND "),m_lo_TableName,ID_FGFUNC_NULL,ID_FGFUNC_DATA);
		cs += _T(" SpDate=0 AND KnOrder=0 AND Val IS NULL");
		sql += cs;
	}
// midori 152388 add <--
	return	ExecuteSQLWork(sql);
}
// midori 160606 cor <--
