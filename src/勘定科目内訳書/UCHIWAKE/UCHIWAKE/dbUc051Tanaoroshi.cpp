// dbUc051Tanaoroshi.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
//#include "UCHIWAKE.h"
#include "dbUc051Tanaoroshi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CdbUc051Tanaoroshi

IMPLEMENT_DYNAMIC(CdbUc051Tanaoroshi, CdbUc000Common)

CdbUc051Tanaoroshi::CdbUc051Tanaoroshi(CDatabase* pdb)
	: CdbUc000Common(pdb)
{
	//{{AFX_FIELD_INIT(CdbUc051Tanaoroshi)
	m_KnSeq = 0;
	m_KnOrder = 0;
	m_KnName = _T("");
	m_HdName = _T("");
	m_Suuryou = _T("");
	m_Tanni = _T("");
	m_Price = _T("");
	m_Val = _T("");
	m_Teki = _T("");
	m_FgInput = 0;
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
// midori 152137 del -->
//	m_nFields = 21;
// midori 152137 del <--
// midori 152137 add -->
	m_KnKana = _T("");
	m_nFields = 22;
// midori 152137 add <--
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	m_lo_pdb = pdb;								// データベースポインタを保持
	m_lo_TableName = STR_TBL_NAME_051;			// テーブル名を記述
	m_rangai_tmp_table = STR_TBL_NAME_052_TMP;
}

CString CdbUc051Tanaoroshi::GetDefaultSQL()
{
	return _T("[dbo].[uc_051_tanaoroshi]");
}

void CdbUc051Tanaoroshi::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CdbUc051Tanaoroshi)
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
	RFX_Long(pFX, _T("[KnSeq]"), m_KnSeq);
	RFX_Long(pFX, _T("[KnOrder]"), m_KnOrder);
	RFX_Text(pFX, _T("[KnName]"), m_KnName);
	RFX_Text(pFX, _T("[HdName]"), m_HdName);
	RFX_Text(pFX, _T("[Suuryou]"), m_Suuryou);
	RFX_Text(pFX, _T("[Tanni]"), m_Tanni);
	RFX_Text(pFX, _T("[Price]"), m_Price);
	RFX_Text(pFX, _T("[Val]"), m_Val);
	RFX_Text(pFX, _T("[Teki]"), m_Teki);
	RFX_Byte(pFX, _T("[FgInput]"), m_FgInput);
	RFX_Byte(pFX, _T("[ShowKeiZero]"), m_ShowKeiZero);
// midori 152137 add -->
	RFX_Text(pFX, _T("[KnKana]"), m_KnKana);
// midori 152137 add <--
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CdbUc051Tanaoroshi 診断

#ifdef _DEBUG
void CdbUc051Tanaoroshi::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbUc051Tanaoroshi::Dump(CDumpContext& dc) const
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
int CdbUc051Tanaoroshi::CreateNewRecord()
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

	m_KnSeq = 0;			// (0)  科目：(科目)シーケンス番号
	m_KnOrder = 0;			// (0)  科目：(科目)順序番号
	m_KnName = _T("");		// ("") 科目：(科目)名称
	m_HdName = _T("");		// ("") 品名
	m_Suuryou = _T("");		// ("") 数量
	m_Tanni = _T("");		// ("") 数量（単位）
	m_Price = _T("");		// ("") 単価
	m_Val = _T("");			// ("") 期末現在高（金額）
	m_Teki = _T("");		// ("") 摘要
	m_FgInput = 0;			// (0)  期末現在高 （0：自動計算/1：手入力)
	m_ShowKeiZero = 0;		// (0)  ０円計表示フラグ
// midori 152137 add -->
	m_KnKana = _T("");		// ("") 科目(カナ)
// midori 152137 add <--

	Update();

	m_strFilter.Format( "Seq = %d", retVal );	// 削除フラグ OFF（0:有効なレコード)
	m_strSort   = "";							// 並び順でソート

	return RequeryWork();
}

// midori 156968 add -->
// 157153 del -->
///**********************************************************************
//	CreateNewRecord2()
//		新規レコードを作成、初期値を追加
//		レコードセットには、新規レコードがセットされて戻ります
//
//	引数
//			なし
//	戻値
//			int					DB_ERR_OK (0)     ：成功
//								DB_ERR_OK (0) 以外：失敗
//***********************************************************************/
//int CdbUc051Tanaoroshi::CreateNewRecord2(int pNumPage,int pNumRow)
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
int CdbUc051Tanaoroshi::CreateNewRecord2(int pNumPage,int pNumRow,int pNullSw)
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
	m_NumPage = pNumPage;	// (0)  ページ番号
	m_NumRow = pNumRow;		// (0)  行番号
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

	m_KnSeq = 0;			// (0)  科目：(科目)シーケンス番号
	m_KnOrder = 0;			// (0)  科目：(科目)順序番号
	m_KnName = _T("");		// ("") 科目：(科目)名称
	m_HdName = _T("");		// ("") 品名
	m_Suuryou = _T("");		// ("") 数量
	m_Tanni = _T("");		// ("") 数量（単位）
	m_Price = _T("");		// ("") 単価
	m_Val = _T("");			// ("") 期末現在高（金額）
	m_Teki = _T("");		// ("") 摘要
	m_FgInput = 0;			// (0)  期末現在高 （0：自動計算/1：手入力)
	m_ShowKeiZero = 0;		// (0)  ０円計表示フラグ
	m_KnKana = _T("");		// ("") 科目(カナ)

	Update();

	return(0);
}
// midori 156968 add <--

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
//int CdbUc051Tanaoroshi::UpdateYokukiZeroClear()
//{
//	CString	sql = GetSQLUpdateYokukiZeroClear("Val");		// 期末現在高
//	sql += GetSQLUpdateYokukiZeroClear("Suuryou");			// 数量
//	
//	sql.Format("%s UPDATE %s SET FgInput = %d WHERE FgFunc <> %d ", sql, m_lo_TableName, ID_AUTOCALC_AUTO, ID_FGFUNC_NULL);	// 自動計算フラグ
//	//CString		strCommand;
//	//CString		strTemp;
//
//	//// クリア対象項目（2項目＋自動計算フラグ）
//	//strCommand.Format("UPDATE %s SET ", m_lo_TableName);
//	//strCommand	+=	_T("Suuryou = 0, ");					// 数量
//	//strCommand	+=	_T("Val = 0, ");						// 期末現在高
//
//	//strTemp.Format("FgInput = %d ", ID_AUTOCALC_AUTO);		// 自動計算フラグ
//	//strCommand += strTemp;
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
int CdbUc051Tanaoroshi::UpdateYokukiZeroClear( int pSw )
{
// midori 152388 del -->
	//CString	sql = GetSQLUpdateYokukiZeroClear("Val",pSw);		// 期末現在高
	//sql += GetSQLUpdateYokukiZeroClear("Suuryou",pSw);			// 数量
	//sql.Format("%s UPDATE %s SET FgInput = %d WHERE FgFunc <> %d ", sql, m_lo_TableName, ID_AUTOCALC_AUTO, ID_FGFUNC_NULL);	// 自動計算フラグ
// midori 152388 del <--
// midori 152388 add -->
	CString	cs=_T("");
	CString	sql = GetSQLUpdateYokukiZeroClear("Val",pSw);		// 期末現在高
	sql += GetSQLUpdateYokukiZeroClear("Suuryou",pSw);			// 数量

	cs.Format("UPDATE %s SET FgInput = %d WHERE FgFunc <> %d ", m_lo_TableName, ID_AUTOCALC_AUTO, ID_FGFUNC_NULL);	// 自動計算フラグ
	sql += cs;

	if(pSw != 0)	{
		cs.Format(_T("UPDATE %s SET FgFunc=%d WHERE FgFunc<=%d AND "),m_lo_TableName,ID_FGFUNC_NULL,ID_FGFUNC_DATA);
		cs += _T(" HdName IS NULL AND Suuryou IS NULL");
		cs += _T(" AND Tanni IS NULL AND Price IS NULL AND Val IS NULL");
		cs += _T(" AND Teki IS NULL");
		cs += _T(" AND KnOrder=0 ");		// 修正24/06/14 add
		sql += cs;
	}
// midori 152388 add <--
	return	ExecuteSQLWork(sql);
}
// midori 160606 cor <--

/**********************************************************************
	GetKnNameFirstRow()
		指定ページの最初に出現する科目の名称を取得

	引数
			int			inPage		(in)頁番号
			CString*	pstrKnName	(out)科目名称
			CString		strTable	(in)取得先のテーブル名
	戻値
			int						正の数：行数
									負の数：失敗
									0     ：行なし
***********************************************************************/
int CdbUc051Tanaoroshi::GetKnNameFirstRow(int inPage, CString* pstrKnName, CString strTable)
{
	return GetKnNameFirstRowSub( inPage, pstrKnName, strTable );
}

/**********************************************************************
	GetSortIkkatuOrder()
		ソートダイアログの一括金額の詳細設定ダイアログで
		設定された項目（科目or取引先）のおーだお

	引数
			なし
	戻値
			一括金額が設定された項目のオーダー
***********************************************************************/
// virtual
int CdbUc051Tanaoroshi::GetSortIkkatuOrder()
{
	return m_KnOrder;
}

// virtual
// midori 155525_2 del -->
//int CdbUc051Tanaoroshi::CreateRangaiTempTable()
// midori 155525_2 del <--
// midori 155525_2 add -->
int CdbUc051Tanaoroshi::CreateRangaiTempTable( int pOutZero )
// midori 155525_2 add <--
{
	// 一時テーブルを作成し、「印字する」レコードのみのテーブル作成（行番号つき）
	CString sql = "if object_id('tempdb.." + m_rangai_tmp_table + "', 'u') is not null drop table " + m_rangai_tmp_table + " \
				   SELECT ROW_NUMBER()OVER(ORDER BY NumPage,NumRow) as row,* \
				   INTO " + m_rangai_tmp_table + " \
				   FROM [dbo].[uc_052_tanaoroshi2] \
				   WHERE FgShow = 0 ";
	ExecuteSQLWork(sql);
	// 「印字する」のみのレコードに対し、ページ数を振る
	sql =		  "DECLARE cur CURSOR for SELECT row, Seq FROM " + m_rangai_tmp_table + " DECLARE @page int \
				   DECLARE @seq int \
				   OPEN cur FETCH cur INTO @page, @seq WHILE @@fetch_status = 0 \
				   BEGIN \
				   UPDATE " + m_rangai_tmp_table + " SET NumPage = @page WHERE Seq = @seq \
				   FETCH cur INTO @page, @seq \
				   END CLOSE cur DEALLOCATE cur ";
	ExecuteSQLWork(sql);

	// rowカラムを削除
	sql =		  "ALTER TABLE " + m_rangai_tmp_table + " DROP COLUMN row";

	return ExecuteSQLWork(sql);
}