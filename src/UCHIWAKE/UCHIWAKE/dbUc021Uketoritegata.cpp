// dbUc021Uketoritegata.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
//#include "uchiwake.h"
#include "dbUc021Uketoritegata.h"
//#include "UCHIWAKE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// インボイス登録番号追加対応_23/12/25 add -->
extern BOOL bG_InvNo;			// インボイス番号使用サイン（決算期末が令和6年3月1日を超えている）
// インボイス登録番号追加対応_23/12/25 add <--

/////////////////////////////////////////////////////////////////////////////
// CdbUc021Uketoritegata

IMPLEMENT_DYNAMIC(CdbUc021Uketoritegata, CdbUc000Common)

CdbUc021Uketoritegata::CdbUc021Uketoritegata(CDatabase* pdb)
	: CdbUc000Common(pdb)
{
	//{{AFX_FIELD_INIT(CdbUc021Uketoritegata)
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
	m_AdSeq = 0;
	m_AdOrder = 0;
	m_AdName1 = _T("");
	m_AdName2 = _T("");
	m_SpDate = 0;
	m_PmDate = 0;
	m_BkSeq = 0;
	m_BkOrder = 0;
	m_BkName1 = _T("");
	m_BkName2 = _T("");
	m_Val = _T("");
	m_Bk2Seq = 0;
	m_Bk2Order = 0;
	m_Bk2Name1 = _T("");
	m_Bk2Name2 = _T("");
	m_Teki = _T("");
	m_ShowKeiZero = 0;
	m_KnSeq = 0;
	m_KnOrder = 0;
	m_KnName = _T("");
// midori 152137 del -->
//	m_nFields = 30;
// midori 152137 del <--
// midori 152137 add -->
	m_AdKana = _T("");
	m_KnKana = _T("");
// インボイス登録番号追加対応_23/10/23 del -->
	//m_nFields = 32;
// インボイス登録番号追加対応_23/10/23 del <--
// インボイス登録番号追加対応_23/10/23 add -->
	m_InvNo = _T("");
	m_nFields = 33;
// インボイス登録番号追加対応_23/10/23 add <--
// midori 152137 add <--
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	m_lo_pdb = pdb;									// データベースポインタを保持
	m_lo_TableName = STR_TBL_NAME_021;				// テーブル名を記述
}


CString CdbUc021Uketoritegata::GetDefaultSQL()
{
	return _T("[dbo].[uc_021_uketoritegata]");
}

void CdbUc021Uketoritegata::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CdbUc021Uketoritegata)
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
	RFX_Long(pFX, _T("[AdSeq]"), m_AdSeq);
	RFX_Long(pFX, _T("[AdOrder]"), m_AdOrder);
	RFX_Text(pFX, _T("[AdName1]"), m_AdName1);
	RFX_Text(pFX, _T("[AdName2]"), m_AdName2);
	RFX_Long(pFX, _T("[SpDate]"), m_SpDate);
	RFX_Long(pFX, _T("[PmDate]"), m_PmDate);
	RFX_Long(pFX, _T("[BkSeq]"), m_BkSeq);
	RFX_Long(pFX, _T("[BkOrder]"), m_BkOrder);
	RFX_Text(pFX, _T("[BkName1]"), m_BkName1);
	RFX_Text(pFX, _T("[BkName2]"), m_BkName2);
	RFX_Text(pFX, _T("[Val]"), m_Val);
	RFX_Long(pFX, _T("[Bk2Seq]"), m_Bk2Seq);
	RFX_Long(pFX, _T("[Bk2Order]"), m_Bk2Order);
	RFX_Text(pFX, _T("[Bk2Name1]"), m_Bk2Name1);
	RFX_Text(pFX, _T("[Bk2Name2]"), m_Bk2Name2);
	RFX_Text(pFX, _T("[Teki]"), m_Teki);
	RFX_Byte(pFX, _T("[ShowKeiZero]"), m_ShowKeiZero);
	RFX_Long(pFX, _T("[KnSeq]"), m_KnSeq);
	RFX_Long(pFX, _T("[KnOrder]"), m_KnOrder);
	RFX_Text(pFX, _T("[KnName]"), m_KnName);
// midori 152137 add -->
	RFX_Text(pFX, _T("[AdKana]"), m_AdKana);
	RFX_Text(pFX, _T("[KnKana]"), m_KnKana);
// midori 152137 add <--
	RFX_Text(pFX, _T("[Invno]"), m_InvNo);	// インボイス登録番号追加対応_23/10/20 add
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CdbUc021Uketoritegata 診断

#ifdef _DEBUG
void CdbUc021Uketoritegata::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbUc021Uketoritegata::Dump(CDumpContext& dc) const
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
int CdbUc021Uketoritegata::CreateNewRecord()
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

	m_AdSeq = 0;			// (0)  振出人：(取引)シーケンス番号
	m_AdOrder = 0;			// (0)  振出人：(取引)順序番号
	m_AdName1 = _T("");		// ("") 振出人：(取引)名称(1行目)
	m_AdName2 = _T("");		// ("") 振出人：(取引)名称(2行目)
	m_SpDate = 0;			// (0)  振出年月日
	m_PmDate = 0;			// (0)  支払期日
	m_BkSeq = 0;			// (0)  支払銀行名：(金融)シーケンス番号
	m_BkOrder = 0;			// (0)  支払銀行名：(金融)順序番号
	m_BkName1 = _T("");		// ("") 支払銀行名：(金融)銀行名
	m_BkName2 = _T("");		// ("") 支払銀行名：(金融)支店名
	m_Val = _T("");			// ("") 金額
	m_Bk2Seq = 0;			// (0)  割引銀行名等：(金融)シーケンス番号
	m_Bk2Order = 0;			// (0)  割引銀行名等：(金融)順序番号
	m_Bk2Name1 = _T("");	// ("") 割引銀行名等：(金融)銀行名
	m_Bk2Name2 = _T("");	// ("") 割引銀行名等：(金融)支店名
	m_Teki = _T("");		// ("") 摘要
	m_ShowKeiZero = 0;		// (0)  ０円計表示フラグ
	m_KnSeq = 0;			// (0)  科目：シーケンス番号
	m_KnOrder = 0;			// (0)  科目：順序番号
	m_KnName = _T("");		// ("") 科目：名称
// midori 152137 add -->
	m_AdKana = _T("");		// ("") 振出人(カナ)
	m_KnKana = _T("");		// ("") 科目(カナ)
// midori 152137 add <--
	m_InvNo = _T("");		// ("") 登録番号		インボイス登録番号追加対応_23/10/20 add

	Update();

	m_strFilter.Format( "Seq = %d", retVal );	// 削除フラグ OFF（0:有効なレコード)
	m_strSort   = "";							// 並び順でソート

	return RequeryWork();
}

// midori 156188 add -->
//// 157153 del -->
//// ------------------------------------------------------------------------------------------------------------------------------------------------------
////	CreateNewRecord()
////		新規レコードを作成、初期値を追加
////		レコードセットには、新規レコードがセットされて戻ります（指定した頁、行に空行を挿入するために使用）
////
////	引数
////			なし
////	戻値
////			int					DB_ERR_OK (0)     ：成功
////								DB_ERR_OK (0) 以外：失敗
//// ------------------------------------------------------------------------------------------------------------------------------------------------------
//int CdbUc021Uketoritegata::CreateNewRecord2(int pNumPage,int pNumRow)
// 157153 del <--
// 157153 add -->
// ------------------------------------------------------------------------------------------------------------------------------------------------------
//	CreateNewRecord()
//		新規レコードを作成、初期値を追加
//		レコードセットには、新規レコードがセットされて戻ります（指定した頁、行に空行を挿入するために使用）
//
//	引数
//			int			作成するレコードの頁番号
//			int			作成するレコードの行番号
//			int			作成する行の種類	0:空行を作成	1:小計NULL行を作成
//	戻値
//			int					DB_ERR_OK (0)     ：成功
//								DB_ERR_OK (0) 以外：失敗
// ------------------------------------------------------------------------------------------------------------------------------------------------------
int CdbUc021Uketoritegata::CreateNewRecord2(int pNumPage,int pNumRow,int pNullSw)
// 157153 add <--
{
	CRecordset	rs(m_lo_pdb);
	CString		SqlStr;
	int			retVal = DB_ERR;

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

	m_AdSeq = 0;			// (0)  振出人：(取引)シーケンス番号
	m_AdOrder = 0;			// (0)  振出人：(取引)順序番号
	m_AdName1 = _T("");		// ("") 振出人：(取引)名称(1行目)
	m_AdName2 = _T("");		// ("") 振出人：(取引)名称(2行目)
	m_SpDate = 0;			// (0)  振出年月日
	m_PmDate = 0;			// (0)  支払期日
	m_BkSeq = 0;			// (0)  支払銀行名：(金融)シーケンス番号
	m_BkOrder = 0;			// (0)  支払銀行名：(金融)順序番号
	m_BkName1 = _T("");		// ("") 支払銀行名：(金融)銀行名
	m_BkName2 = _T("");		// ("") 支払銀行名：(金融)支店名
	m_Val = _T("");			// ("") 金額
	m_Bk2Seq = 0;			// (0)  割引銀行名等：(金融)シーケンス番号
	m_Bk2Order = 0;			// (0)  割引銀行名等：(金融)順序番号
	m_Bk2Name1 = _T("");	// ("") 割引銀行名等：(金融)銀行名
	m_Bk2Name2 = _T("");	// ("") 割引銀行名等：(金融)支店名
	m_Teki = _T("");		// ("") 摘要
	m_ShowKeiZero = 0;		// (0)  ０円計表示フラグ
	m_KnSeq = 0;			// (0)  科目：シーケンス番号
	m_KnOrder = 0;			// (0)  科目：順序番号
	m_KnName = _T("");		// ("") 科目：名称
	m_AdKana = _T("");		// ("") 振出人(カナ)
	m_KnKana = _T("");		// ("") 科目(カナ)
	m_InvNo = _T("");		// ("") 登録番号		インボイス登録番号追加対応_23/10/20 add

	Update();

	return(0);
}

// 157153 del -->
//// ------------------------------------------------------------------------------------------------
////	CreateNewRecord3()
////		新規レコードを作成（小計NULL行を作成）（出力形式変更時に使用）
////		レコードセットには、新規レコードがセットされて戻ります
////	引数
////			なし
////	戻値
////			int					DB_ERR_OK (0)     ：成功
////								DB_ERR_OK (0) 以外：失敗
////
////	新規レコード追加作業を CdbUc00Common にて実行
////	各データテーブルでは、初期値の代入を実施する。
//// ------------------------------------------------------------------------------------------------
//// virtual
//int CdbUc021Uketoritegata::CreateNewRecord3(int pNumPage,int pNumRow)
//{
//	CRecordset	rs(m_lo_pdb);
//	CString		SqlStr;
//	int			retVal = DB_ERR;
//
//	/////////////////////////////////////////////////////////////////////
//	// 新しいシーケンス番号を算出
//	SqlStr =	_T("SELECT max(Seq) AS MaxNum ");
//	SqlStr +=	_T("FROM ") + m_lo_TableName + _T(" ");
//
//	if ( ! OpenEx( &rs , SqlStr , m_lo_TableName ) ) {
//		return DB_ERR_OPEN;
//	};
//
//	retVal = DB_ERR_EOF;
//	if (!rs.IsEOF()) {
//		CDBVariant	val;
//		rs.GetFieldValue(_T("MaxNum"), val);
//		if (val.m_dwType == DBVT_NULL) {	// NULLだった場合
//			retVal = 0;
//		}
//		else {								// 値の取得OK
//			retVal = val.m_lVal;
//		}
//	}
//	rs.Close();
//
//	retVal++;				//	自動付番シーケンス番号カウントアップ
//
//
//	/////////////////////////////////////////////////////////////////////
//	// 新規レコード追加作業
//
//	AddNew();
//							
//	m_Seq = retVal;							// シーケンス番号を自動付番
//	m_NumPage = pNumPage;					// (0)  ページ番号
//	m_NumRow = pNumRow;						// (0)  行番号
//	m_FgFunc = ID_FGFUNC_SYOKEINULL;		// (0)  空行
//	m_FgShow = 0;							// (0)  表示
//	m_KeiStr = _T("");						// ("") 計名称
//	m_NumGroup = 0;							// (0)  グループ番号
//	m_RenKcd = _T("");						// ("") 連動元情報(科目)
//	m_RenEcd = 0;							// (0)  連動元情報(枝番)
//	m_RenFgTemp = 0;						// (0)  連動テンポラリ情報
//
//	m_AdSeq = 0;							// (0)  振出人：(取引)シーケンス番号
//	m_AdOrder = 0;							// (0)  振出人：(取引)順序番号
//	m_AdName1 = _T("");						// ("") 振出人：(取引)名称(1行目)
//	m_AdName2 = _T("");						// ("") 振出人：(取引)名称(2行目)
//	m_SpDate = 0;							// (0)  振出年月日
//	m_PmDate = 0;							// (0)  支払期日
//	m_BkSeq = 0;							// (0)  支払銀行名：(金融)シーケンス番号
//	m_BkOrder = 0;							// (0)  支払銀行名：(金融)順序番号
//	m_BkName1 = _T("");						// ("") 支払銀行名：(金融)銀行名
//	m_BkName2 = _T("");						// ("") 支払銀行名：(金融)支店名
//	m_Val = _T("");							// ("") 金額
//	m_Bk2Seq = 0;							// (0)  割引銀行名等：(金融)シーケンス番号
//	m_Bk2Order = 0;							// (0)  割引銀行名等：(金融)順序番号
//	m_Bk2Name1 = _T("");					// ("") 割引銀行名等：(金融)銀行名
//	m_Bk2Name2 = _T("");					// ("") 割引銀行名等：(金融)支店名
//	m_Teki = _T("");						// ("") 摘要
//	m_ShowKeiZero = 0;						// (0)  ０円計表示フラグ
//	m_KnSeq = 0;							// (0)  科目：シーケンス番号
//	m_KnOrder = 0;							// (0)  科目：順序番号
//	m_KnName = _T("");						// ("") 科目：名称
//	m_AdKana = _T("");						// ("") 振出人(カナ)
//	m_KnKana = _T("");						// ("") 科目(カナ)
//
//	Update();
//
//	return(0);
//}
// 157153 del <--
// midori 156188 add <--

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
//int CdbUc021Uketoritegata::UpdateYokukiZeroClear()
//{
//	CString	sql = GetSQLUpdateYokukiZeroClear("Val");	// 金額
//	sql += GetSQLUpdateYokukiZeroClear("SpDate");		// 振出年月日
//	sql += GetSQLUpdateYokukiZeroClear("PmDate");		// 支払期日
//	//CString		strCommand;
//	//CString		strTemp;
//
//	//// クリア対象項目（3項目）
//	//strCommand.Format("UPDATE %s SET ", m_lo_TableName);
//	//strCommand	+=	_T("SpDate = 0, ");						// 振出年月日
//	//strCommand	+=	_T("PmDate = 0, ");						// 支払期日
//	//strCommand	+=	_T("Val = 0 ");							// 金額
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
int CdbUc021Uketoritegata::UpdateYokukiZeroClear( int pSw )
{
// midori 152388 add -->
	CString	cs=_T("");
// midori 152388 add <--
	CString	sql = GetSQLUpdateYokukiZeroClear("Val",pSw);	// 金額
	sql += GetSQLUpdateYokukiZeroClear("SpDate",0);			// 振出年月日
	sql += GetSQLUpdateYokukiZeroClear("PmDate",0);			// 支払期日
// midori 152388 add -->
	if(pSw != 0)	{
		cs.Format(_T("UPDATE %s SET FgFunc=%d WHERE FgFunc<=%d AND "),m_lo_TableName,ID_FGFUNC_NULL,ID_FGFUNC_DATA);
		cs += _T(" KnOrder=0 AND AdName1 IS NULL AND AdName2 IS NULL AND SpDate=0 AND PmDate=0");
		cs += _T(" AND BkName1 IS NULL AND BkName2 IS NULL AND Val IS NULL");
		cs += _T(" AND Bk2Name1 IS NULL AND Bk2Name2 IS NULL AND Teki IS NULL");
		if(bG_InvNo == TRUE)	cs += _T(" AND Invno IS NULL");			// インボイス登録番号追加対応_23/12/25 add
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
int CdbUc021Uketoritegata::GetKnNameFirstRow(int inPage, CString* pstrKnName, CString strTable)
{
	return GetKnNameFirstRowSub( inPage, pstrKnName, strTable );
}

/**********************************************************************
	GetSQLInsertColumns()
		科目行を挿入するInsert文を作成する

	引数
			CString					更新対象のテーブル名
	戻値
			CString					INSERT文
***********************************************************************/
// virtual
CString CdbUc021Uketoritegata::GetSQLInsertColumns(CString BaseTbl)
{
	CString strSQL;

// インボイス登録番号追加対応_23/10/23 del -->
//// midori 152137 del -->
////	strSQL.Format(_T("INSERT INTO %s VALUES(NULL,0,@page,@row,3,0,NULL,0,NULL,0,0,0,0,NULL,NULL,0,0,0,0,NULL,NULL,NULL,0,0,NULL,NULL,NULL,0,@knseq,@knorder,@knname)"), BaseTbl);
//// midori 152137 del <--
//// midori 152137 add -->
//	strSQL.Format(_T("INSERT INTO %s VALUES(NULL,0,@page,@row,3,0,NULL,0,NULL,0,0,0,0,NULL,NULL,0,0,0,0,NULL,NULL,NULL,0,0,NULL,NULL,NULL,0,@knseq,@knorder,@knname,NULL,@knkana)"), BaseTbl);
//// midori 152137 add <--
// インボイス登録番号追加対応_23/10/23 del <--
// インボイス登録番号追加対応_23/10/23 add -->
	strSQL.Format(_T("INSERT INTO %s VALUES(NULL,0,@page,@row,3,0,NULL,0,NULL,0,0,0,0,NULL,NULL,0,0,0,0,NULL,NULL,NULL,0,0,NULL,NULL,NULL,0,@knseq,@knorder,@knname,NULL,@knkana,NULL)"), BaseTbl);
// インボイス登録番号追加対応_23/10/23 add <--

	return strSQL;
}

/**********************************************************************
	CalcPageRowNumToTempTable()
		ページ番号、行番号を振り直す。
		②受取手形の内訳書は科目行を挿入する。

	引数
			CString					最終結果をもつ一時テーブル
			CString					元となるテーブル
			int						そのページの最大行数（計行を除く）
			CString					カラム配列
// midori 152138 add -->
			CString					編集元のテーブル名
			int						様式Seq
			int						1:小計行の次行に空行を挿入した
// midori 152138 add <--
	戻値
			int						成功 : 0(DB_ERR_OK)
									失敗 : -200(DB_ERR_EXESQL)
***********************************************************************/
// virtual
// midori 152137 del -->
//// midori 161111 cor -->
////// midori 160610 cor -->
//////int CdbUc021Uketoritegata::CalcPageRowNumToTempTable(CString TargetTable, CString BaseTable, int MaxRow, CString strColumn)
////// ---------------------
////int CdbUc021Uketoritegata::CalcPageRowNumToTempTable(CString TargetTable, CString BaseTable, int MaxRow, CString strColumn, CString strMoto)
////// midori 160610 cor <--
//// ---------------------
//int CdbUc021Uketoritegata::CalcPageRowNumToTempTable(CString TargetTable, CString BaseTable, int MaxRow, CString strColumn, CString strMoto, int pFormSeq)
//// midori 161111 cor <--
//{
//	BOOL			bFlgKamoku = FALSE;						// 科目行追加する？
//	CString			strSQL;
//	CdbUcInfSub		mfcRecSub(m_lo_pdb);
//// midori 161111 add -->
//	CdbPrtWork2*	rsData;
//	int				isNullLine=0;
//// midori 161111 add <--
//
//	if( mfcRecSub.RequeryFormSeq( ID_FORMNO_021 ) == DB_ERR_OK ){
//		if( !(mfcRecSub.IsEOF()) ){
//			if( mfcRecSub.m_GeneralConstVal[15] & BIT_D1 ){
//				CCtrlSubGeneralVar cs( mfcRecSub );
//				if( cs.bOutKamokuRow ){
//					bFlgKamoku = TRUE;
//				}
//			}
//		}
//		mfcRecSub.Fin();
//	}
//
//	// 「金額０円データ及び空欄データを出力しない」ON　+　「科目行を出力する」ON
//	if( bFlgKamoku != FALSE ){
//		// 一時テーブルが存在する場合、それを削除
//		strSQL.Format(_T("declare @per_page_num int set @per_page_num = %d "
//			"if object_id('tempdb..%s', 'u') is not null drop table %s ; with "), MaxRow, TargetTable, TargetTable );
//
//		// ソート頁、行順、（科目行の為に行種降順）に一意な仮行番号(row)の作成
//		strSQL.Format( strSQL + _T("rownum_tbl(seq,NumRow,FgFunc,KnSeq,KnOrder,KnName) as ( SELECT seq, ROW_NUMBER() OVER ( ORDER BY NumPage,NumRow,FgFunc ASC ),FgFunc,KnSeq,KnOrder, KnName "
//			"FROM %s ), ")
//			, BaseTable);
//
//		// ページ導出
//		strSQL += _T("rowpagenum_tbl(seq,NumRow,NumPage,FgFunc,KnSeq,KnOrder,KnName) "
//			"as( "
//			"SELECT seq, NumRow , CAST(( CEILING( CAST( NumRow AS FLOAT ) / CAST( @per_page_num AS FLOAT ) ) ) AS smallint ),FgFunc,KnSeq,KnOrder,KnName "
//			"FROM rownum_tbl "
//			"), ");
//
//		// ページをパーティションにし、行導出
//		strSQL += _T("new_rowpagenum_tbl(seq,NumRow,NumPage,FgFunc,KnSeq,KnOrder,KnName) "
//			"as( "
//			"SELECT seq, ROW_NUMBER()OVER(PARTITION BY NumPage ORDER BY NumRow),NumPage,FgFunc,KnSeq,KnOrder,KnName "
//			"FROM rowpagenum_tbl "
//			") ");
//
//		// 従来のテーブルとseqで結合し、新しい行番号、新しいページ番号を再付番
//// midori 160610 cor -->
//		//strSQL.Format( strSQL + _T("SELECT new_rowpagenum_tbl.seq, new_rowpagenum_tbl.NumPage, new_rowpagenum_tbl.NumRow, new_rowpagenum_tbl.FgFunc %s "
//		//	"INTO %s FROM new_rowpagenum_tbl LEFT OUTER JOIN %s ON new_rowpagenum_tbl.seq = %s.seq "),
//		//	GetSQLResultSelect("new_rowpagenum_tbl"), TargetTable, GetDefaultSQL(), GetDefaultSQL() );
//// ---------------------
//		strSQL.Format( strSQL + _T("SELECT new_rowpagenum_tbl.seq, new_rowpagenum_tbl.NumPage, new_rowpagenum_tbl.NumRow, new_rowpagenum_tbl.FgFunc %s "
//			"INTO %s FROM new_rowpagenum_tbl LEFT OUTER JOIN %s ON new_rowpagenum_tbl.seq = %s.seq "),
//			GetSQLResultSelect("new_rowpagenum_tbl"), TargetTable, strMoto, strMoto );
//// midori 160610 cor <--
//	}
//	// 「金額０円データ及び空欄データを出力しない」ON　+　「科目行を出力する」OFF
//	else{
//
//// midori 161111 add -->
//		// 最終行の小計の次行に、空白行を挿入したかを調べる
//		rsData = new CdbPrtWork2(m_lo_pdb,pFormSeq);
//		strSQL.Format(_T("row IS NULL AND Seq=0 AND FgFunc=%d"),ID_FGFUNC_NULL);
//		rsData->RequeryPageRow(strSQL);
//		if(!rsData->IsEOF())	{
//			isNullLine = 1;
//		}
//		delete	rsData;
//// midori 161111 add <--
//
//		// CdbUc000CommonのCalcPageRowNumToTempTableと同様
//		// 一時テーブルが存在する場合、それを削除
//		strSQL.Format(_T("declare @per_page_num int set @per_page_num = %d "
//			"if object_id('tempdb..%s', 'u') is not null drop table %s ; with "), MaxRow, TargetTable, TargetTable );
//
//		// ソート頁、行順、（科目行の為に行種降順）に一意な仮行番号(row)の作成
//		strSQL.Format( strSQL + _T("rownum_tbl(seq,NumRow) as ( SELECT seq, ROW_NUMBER() OVER ( ORDER BY NumPage,NumRow,FgFunc ASC ) "
//			"FROM %s ), ")
//			, BaseTable);
//
//		// ページ導出
//		strSQL += _T("rowpagenum_tbl(seq,NumRow,NumPage) "
//			"as( "
//			"SELECT seq, NumRow , CAST(( CEILING( CAST( NumRow AS FLOAT ) / CAST( @per_page_num AS FLOAT ) ) ) AS smallint ) "
//			"FROM rownum_tbl "
//			"), ");
//
//		// ページをパーティションにし、行導出
//		strSQL += _T("new_rowpagenum_tbl(seq,NumRow,NumPage) "
//			"as( "
//			"SELECT seq, ROW_NUMBER()OVER(PARTITION BY NumPage ORDER BY NumRow), NumPage "
//			"FROM rowpagenum_tbl "
//			") ");
//
//		// 従来のテーブルとseqで結合し、新しい行番号、新しいページ番号を再付番
//// midori 161111 cor -->
////// midori 160610 cor -->
//////		strSQL.Format( strSQL + _T("SELECT new_rowpagenum_tbl.seq, new_rowpagenum_tbl.NumPage, new_rowpagenum_tbl.NumRow, FgFunc %s "
//////			"INTO %s FROM new_rowpagenum_tbl, %s WHERE new_rowpagenum_tbl.seq = %s.seq "),
//////			strColumn, TargetTable, GetDefaultSQL(), GetDefaultSQL() );
////// ---------------------
////		strSQL.Format( strSQL + _T("SELECT new_rowpagenum_tbl.seq, new_rowpagenum_tbl.NumPage, new_rowpagenum_tbl.NumRow, FgFunc %s "
////			"INTO %s FROM new_rowpagenum_tbl, %s WHERE new_rowpagenum_tbl.seq = %s.seq "),
////			strColumn, TargetTable, strMoto, strMoto );
////// midori 160610 cor <--
//// ---------------------
//		if(isNullLine != 0)	{
//			// 最終行の小計の次行に、空白行を挿入した場合は
//			// 科目行挿入を行った場合と同様に、外部結合(LEFT JOIN)を行う
//			strSQL.Format( strSQL + _T("SELECT new_rowpagenum_tbl.seq, new_rowpagenum_tbl.NumPage, new_rowpagenum_tbl.NumRow, FgFunc %s "
//				"INTO %s FROM new_rowpagenum_tbl LEFT OUTER JOIN %s ON new_rowpagenum_tbl.seq = %s.seq "),
//							strColumn, TargetTable, strMoto, strMoto );
//		}
//		else	{
//			strSQL.Format( strSQL + _T("SELECT new_rowpagenum_tbl.seq, new_rowpagenum_tbl.NumPage, new_rowpagenum_tbl.NumRow, FgFunc %s "
//				"INTO %s FROM new_rowpagenum_tbl, %s WHERE new_rowpagenum_tbl.seq = %s.seq "),
//				strColumn, TargetTable, strMoto, strMoto );
//		}
//// midori 161111 cor <--
//	}
//
//	// SQL実行
//	if( ExecuteSQLWork( strSQL ) != 0 ){
//		return DB_ERR_EXESQL;
//	}
//
//// midori 161111 add -->
//	if(isNullLine != 0)	{
//		// 最終行の小計の次行に、空白行を挿入した場合は
//		// 挿入した空白行の特殊行フラグ(FgFunc)が元テーブル(uc_999_xxxx)から取得できずに
//		// NULLになっているので、ここで０に置き換える
//		strSQL.Format(_T("UPDATE %s SET FgFunc=%d WHERE FgFunc IS NULL"),TargetTable,ID_FGFUNC_NULL);
//		// SQL実行
//		if( ExecuteSQLWork( strSQL ) != 0 ){
//			return DB_ERR_EXESQL;
//		}
//	}
//// midori 161111 add <--
//
//	return DB_ERR_OK;
//}
// midori 152137 del <--
// midori 152137 add -->
// midori 152138 del -->
//int CdbUc021Uketoritegata::CalcPageRowNumToTempTable(CString TargetTable, CString BaseTable, int MaxRow, CString strColumn, CString strMoto, int pFormSeq)
// midori 152138 del <--
// midori 152138 add -->
int CdbUc021Uketoritegata::CalcPageRowNumToTempTable(CString TargetTable, CString BaseTable, int MaxRow, CString strColumn, CString strMoto, int pFormSeq, int pNullAdd)
// midori 152138 add <--
{
// midori 190505 del -->
	//BOOL			bFlgKamoku = FALSE;						// 科目行追加する？
// midori 190505 del <--
// midori 156188 add -->
	BOOL			bFlgKamoku = FALSE;						// 科目行追加する？
// midori 156188_add <--
	CString			strSQL;
	CdbUcInfSub		mfcRecSub(m_lo_pdb);
// midori 152138 del -->
	//CdbPrtWork2*	rsData;
	//int				isNullLine=0;
// midori 152138 del <--

// midori 190505 del -->
//	if( mfcRecSub.RequeryFormSeq( ID_FORMNO_021 ) == DB_ERR_OK ){
//		if( !(mfcRecSub.IsEOF()) ){
//			if( mfcRecSub.m_GeneralConstVal[15] & BIT_D1 ){
//				CCtrlSubGeneralVar cs( mfcRecSub );
//				if( cs.bOutKamokuRow ){
//					bFlgKamoku = TRUE;
//				}
//			}
//		}
//		mfcRecSub.Fin();
//	}
//
//	// 「金額０円データ及び空欄データを出力しない」ON　+　「科目行を出力する」ON
//	if( bFlgKamoku != FALSE ){
//		// 一時テーブルが存在する場合、それを削除
//		strSQL.Format(_T("declare @per_page_num int set @per_page_num = %d "
//			"if object_id('tempdb..%s', 'u') is not null drop table %s ; with "), MaxRow, TargetTable, TargetTable );
//
//		// ソート頁、行順、（科目行の為に行種降順）に一意な仮行番号(row)の作成
//		strSQL.Format( strSQL + _T("rownum_tbl(seq,NumRow,FgFunc,KnSeq,KnOrder,KnName,KnKana) as ( SELECT seq, ROW_NUMBER() OVER ( ORDER BY NumPage,NumRow,FgFunc ASC ),FgFunc,KnSeq,KnOrder,KnName,KnKana "
//			"FROM %s ), ")
//			, BaseTable);
//
//		// ページ導出
//		strSQL += _T("rowpagenum_tbl(seq,NumRow,NumPage,FgFunc,KnSeq,KnOrder,KnName,KnKana) "
//			"as( "
//			"SELECT seq, NumRow , CAST(( CEILING( CAST( NumRow AS FLOAT ) / CAST( @per_page_num AS FLOAT ) ) ) AS smallint ),FgFunc,KnSeq,KnOrder,KnName,KnKana "
//			"FROM rownum_tbl "
//			"), ");
//
//		// ページをパーティションにし、行導出
//		strSQL += _T("new_rowpagenum_tbl(seq,NumRow,NumPage,FgFunc,KnSeq,KnOrder,KnName,KnKana) "
//			"as( "
//			"SELECT seq, ROW_NUMBER()OVER(PARTITION BY NumPage ORDER BY NumRow),NumPage,FgFunc,KnSeq,KnOrder,KnName,KnKana "
//			"FROM rowpagenum_tbl "
//			") ");
//
//		// 従来のテーブルとseqで結合し、新しい行番号、新しいページ番号を再付番
//		strSQL.Format( strSQL + _T("SELECT new_rowpagenum_tbl.seq, new_rowpagenum_tbl.NumPage, new_rowpagenum_tbl.NumRow, new_rowpagenum_tbl.FgFunc %s "
//			"INTO %s FROM new_rowpagenum_tbl LEFT OUTER JOIN %s ON new_rowpagenum_tbl.seq = %s.seq "),
//			GetSQLResultSelect("new_rowpagenum_tbl"), TargetTable, strMoto, strMoto );
//	}
//	// 「金額０円データ及び空欄データを出力しない」ON　+　「科目行を出力する」OFF
//	else{
//
//// midori 152138 del -->
//		//// 最終行の小計の次行に、空白行を挿入したかを調べる
//		//rsData = new CdbPrtWork2(m_lo_pdb,pFormSeq);
//		//strSQL.Format(_T("row IS NULL AND Seq=0 AND FgFunc=%d"),ID_FGFUNC_NULL);
//		//rsData->RequeryPageRow(strSQL);
//		//if(!rsData->IsEOF())	{
//		//	isNullLine = 1;
//		//}
//		//delete	rsData;
//// midori 152138 del <--
//
//		// CdbUc000CommonのCalcPageRowNumToTempTableと同様
//		// 一時テーブルが存在する場合、それを削除
//		strSQL.Format(_T("declare @per_page_num int set @per_page_num = %d "
//			"if object_id('tempdb..%s', 'u') is not null drop table %s ; with "), MaxRow, TargetTable, TargetTable );
//
//		// ソート頁、行順、（科目行の為に行種降順）に一意な仮行番号(row)の作成
//		strSQL.Format( strSQL + _T("rownum_tbl(seq,NumRow) as ( SELECT seq, ROW_NUMBER() OVER ( ORDER BY NumPage,NumRow,FgFunc ASC ) "
//			"FROM %s ), ")
//			, BaseTable);
//
//		// ページ導出
//		strSQL += _T("rowpagenum_tbl(seq,NumRow,NumPage) "
//			"as( "
//			"SELECT seq, NumRow , CAST(( CEILING( CAST( NumRow AS FLOAT ) / CAST( @per_page_num AS FLOAT ) ) ) AS smallint ) "
//			"FROM rownum_tbl "
//			"), ");
//
//		// ページをパーティションにし、行導出
//		strSQL += _T("new_rowpagenum_tbl(seq,NumRow,NumPage) "
//			"as( "
//			"SELECT seq, ROW_NUMBER()OVER(PARTITION BY NumPage ORDER BY NumRow), NumPage "
//			"FROM rowpagenum_tbl "
//			") ");
//
//		// 従来のテーブルとseqで結合し、新しい行番号、新しいページ番号を再付番
//// midori 152138 del -->
//		//if(isNullLine != 0)	{
//// midori 152138 del <--
//// midori 152138 add -->
//		if(pNullAdd != 0)	{
//// midori 152138 add <--
//			// 最終行の小計の次行に、空白行を挿入した場合は
//			// 科目行挿入を行った場合と同様に、外部結合(LEFT JOIN)を行う
//			strSQL.Format( strSQL + _T("SELECT new_rowpagenum_tbl.seq, new_rowpagenum_tbl.NumPage, new_rowpagenum_tbl.NumRow, FgFunc %s "
//				"INTO %s FROM new_rowpagenum_tbl LEFT OUTER JOIN %s ON new_rowpagenum_tbl.seq = %s.seq "),
//							strColumn, TargetTable, strMoto, strMoto );
//		}
//		else	{
//			strSQL.Format( strSQL + _T("SELECT new_rowpagenum_tbl.seq, new_rowpagenum_tbl.NumPage, new_rowpagenum_tbl.NumRow, FgFunc %s "
//				"INTO %s FROM new_rowpagenum_tbl, %s WHERE new_rowpagenum_tbl.seq = %s.seq "),
//				strColumn, TargetTable, strMoto, strMoto );
//		}
//	}
// midori 190505 del <--
// midori 190505 add -->
// midori 156188 add -->
	if( mfcRecSub.RequeryFormSeq( ID_FORMNO_021 ) == DB_ERR_OK ){
		if( !(mfcRecSub.IsEOF()) ){
			if( mfcRecSub.m_GeneralConstVal[15] & BIT_D1 ){
				CCtrlSubGeneralVar cs( mfcRecSub );
				if( cs.bOutKamokuRow ){
					bFlgKamoku = TRUE;
				}
			}
		}
		mfcRecSub.Fin();
	}

	// 「金額０円データ及び空欄データを出力しない」ON　+　「科目行を出力する」ON
	if( bFlgKamoku != FALSE ){
		// 一時テーブルが存在する場合、それを削除
		strSQL.Format(_T("declare @per_page_num int set @per_page_num = %d "
			"if object_id('tempdb..%s', 'u') is not null drop table %s ; with "), MaxRow, TargetTable, TargetTable );

		// ソート頁、行順、（科目行の為に行種降順）に一意な仮行番号(row)の作成
		strSQL.Format( strSQL + _T("rownum_tbl(seq,NumRow,FgFunc,KnSeq,KnOrder,KnName,KnKana) as ( SELECT seq, ROW_NUMBER() OVER ( ORDER BY NumPage,NumRow,FgFunc ASC ),FgFunc,KnSeq,KnOrder,KnName,KnKana "
			"FROM %s ), ")
			, BaseTable);

		// ページ導出
		strSQL += _T("rowpagenum_tbl(seq,NumRow,NumPage,FgFunc,KnSeq,KnOrder,KnName,KnKana) "
			"as( "
			"SELECT seq, NumRow , CAST(( CEILING( CAST( NumRow AS FLOAT ) / CAST( @per_page_num AS FLOAT ) ) ) AS smallint ),FgFunc,KnSeq,KnOrder,KnName,KnKana "
			"FROM rownum_tbl "
			"), ");

		// ページをパーティションにし、行導出
		strSQL += _T("new_rowpagenum_tbl(seq,NumRow,NumPage,FgFunc,KnSeq,KnOrder,KnName,KnKana) "
			"as( "
			"SELECT seq, ROW_NUMBER()OVER(PARTITION BY NumPage ORDER BY NumRow),NumPage,FgFunc,KnSeq,KnOrder,KnName,KnKana "
			"FROM rowpagenum_tbl "
			") ");

		// 従来のテーブルとseqで結合し、新しい行番号、新しいページ番号を再付番
		strSQL.Format( strSQL + _T("SELECT new_rowpagenum_tbl.seq, new_rowpagenum_tbl.NumPage, new_rowpagenum_tbl.NumRow, new_rowpagenum_tbl.FgFunc %s "
			"INTO %s FROM new_rowpagenum_tbl LEFT OUTER JOIN %s ON new_rowpagenum_tbl.seq = %s.seq "),
			GetSQLResultSelect("new_rowpagenum_tbl"), TargetTable, strMoto, strMoto );
	}
	// 「金額０円データ及び空欄データを出力しない」ON　+　「科目行を出力する」OFF
	else{
// midori 156188 add <--
		// CdbUc000CommonのCalcPageRowNumToTempTableと同様
		// 一時テーブルが存在する場合、それを削除
		strSQL.Format(_T("declare @per_page_num int set @per_page_num = %d "
			"if object_id('tempdb..%s', 'u') is not null drop table %s ; with "), MaxRow, TargetTable, TargetTable );

		// ソート頁、行順、（科目行の為に行種降順）に一意な仮行番号(row)の作成
		strSQL.Format( strSQL + _T("rownum_tbl(seq,NumRow) as ( SELECT seq, ROW_NUMBER() OVER ( ORDER BY NumPage,NumRow,FgFunc ASC ) "
			"FROM %s ), ")
			, BaseTable);

		// ページ導出
		strSQL += _T("rowpagenum_tbl(seq,NumRow,NumPage) "
			"as( "
			"SELECT seq, NumRow , CAST(( CEILING( CAST( NumRow AS FLOAT ) / CAST( @per_page_num AS FLOAT ) ) ) AS smallint ) "
			"FROM rownum_tbl "
			"), ");

		// ページをパーティションにし、行導出
		strSQL += _T("new_rowpagenum_tbl(seq,NumRow,NumPage) "
			"as( "
			"SELECT seq, ROW_NUMBER()OVER(PARTITION BY NumPage ORDER BY NumRow), NumPage "
			"FROM rowpagenum_tbl "
			") ");

		// 従来のテーブルとseqで結合し、新しい行番号、新しいページ番号を再付番
		if(pNullAdd != 0)	{
			// 最終行の小計の次行に、空白行を挿入した場合は
			// 科目行挿入を行った場合と同様に、外部結合(LEFT JOIN)を行う
			strSQL.Format( strSQL + _T("SELECT new_rowpagenum_tbl.seq, new_rowpagenum_tbl.NumPage, new_rowpagenum_tbl.NumRow, FgFunc %s "
				"INTO %s FROM new_rowpagenum_tbl LEFT OUTER JOIN %s ON new_rowpagenum_tbl.seq = %s.seq "),
							strColumn, TargetTable, strMoto, strMoto );
		}
		else	{
			strSQL.Format( strSQL + _T("SELECT new_rowpagenum_tbl.seq, new_rowpagenum_tbl.NumPage, new_rowpagenum_tbl.NumRow, FgFunc %s "
				"INTO %s FROM new_rowpagenum_tbl, %s WHERE new_rowpagenum_tbl.seq = %s.seq "),
				strColumn, TargetTable, strMoto, strMoto );
		}
// midori 190505 add <--
// midori 156188 add -->
	}
// midori 156188 add <--

	// SQL実行
	if( ExecuteSQLWork( strSQL ) != 0 ){
		return DB_ERR_EXESQL;
	}

// midori 152138 del -->
	//if(isNullLine != 0)	{
// midori 152138 del <--
// midori 152138 add -->
	if(pNullAdd != 0)	{
// midori 152138 add <--
		// 最終行の小計の次行に、空白行を挿入した場合は
		// 挿入した空白行の特殊行フラグ(FgFunc)が元テーブル(uc_999_xxxx)から取得できずに
		// NULLになっているので、ここで０に置き換える
		strSQL.Format(_T("UPDATE %s SET FgFunc=%d WHERE FgFunc IS NULL"),TargetTable,ID_FGFUNC_NULL);
		// SQL実行
		if( ExecuteSQLWork( strSQL ) != 0 ){
			return DB_ERR_EXESQL;
		}
	}

	return DB_ERR_OK;
}
// midori 152137 add <--

/**********************************************************************
	GetSQLInsertColumns()
		科目行を挿入するInsert文を作成する

	引数
			CString					更新対象のテーブル名
	戻値
			CString					INSERT文
***********************************************************************/
// virtual
CString CdbUc021Uketoritegata::GetSQLResultSelect(CString strTable)
{
	CString strSQL;

// インボイス登録番号追加対応_23/10/23 del -->
//// midori 152137 del -->
////	strSQL.Format(_T(", FgShow, KeiStr, NumGroup, RenKcd, RenEcd, RenFgTemp, AdSeq, AdOrder, AdName1, AdName2, SpDate, PmDate, BkSeq, BkOrder, BkName1, BkName2, Val, Bk2Seq, Bk2Order, Bk2Name1, Bk2Name2, Teki, ShowKeiZero, %s.KnSeq, %s.KnOrder, %s.KnName "),
////		strTable, strTable, strTable);
//// midori 152137 del <--
//// midori 152137 add -->
//	strSQL.Format(_T(", FgShow, KeiStr, NumGroup, RenKcd, RenEcd, RenFgTemp, AdSeq, AdOrder, AdName1, AdName2, SpDate, PmDate, BkSeq, BkOrder, BkName1, BkName2, Val, Bk2Seq, Bk2Order, Bk2Name1, Bk2Name2, Teki, ShowKeiZero, %s.KnSeq, %s.KnOrder, %s.KnName, AdKana, %s.KnKana "),
//		strTable, strTable, strTable, strTable);
//// midori 152137 add <--
// インボイス登録番号追加対応_23/10/23 del <--
// インボイス登録番号追加対応_23/10/23 add -->
	strSQL.Format(_T(", FgShow, KeiStr, NumGroup, RenKcd, RenEcd, RenFgTemp, AdSeq, AdOrder, AdName1, AdName2, SpDate, PmDate, BkSeq, BkOrder, BkName1, BkName2, Val, Bk2Seq, Bk2Order, Bk2Name1, Bk2Name2, Teki, ShowKeiZero, %s.KnSeq, %s.KnOrder, %s.KnName, AdKana, %s.KnKana, Invno "),
		strTable, strTable, strTable, strTable);
// インボイス登録番号追加対応_23/10/23 add <--
	return strSQL;
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
int CdbUc021Uketoritegata::GetSortIkkatuOrder()
{
	return m_KnOrder;
}

//********************************************************************************
//	ソートキーのタイプがintと異なる場合、そのタイプを返す。
//		IN		なし
//		RET		受取手形の内訳書の第一ソート項目が「振出人」のときはvarchar(400)（[OrderStr]のサイズ300+余分100）
//********************************************************************************
CString CdbUc021Uketoritegata::GetSortItemType()
{
	CString		strSortItem = "int";
	CdbUcInfSub	mfcInfSub( m_lo_pdb );

	try{
		mfcInfSub.RequeryFormSeq( ID_FORMNO_021 );

		if( !mfcInfSub.IsEOF() ){
			if( mfcInfSub.m_Sort1 == 1 ){	// 1 = 「振出人」
				strSortItem = "varchar(400)";
			}
		}

		mfcInfSub.Fin();
	}
	catch(...){
		//	レコードセット開かれてる？
		if ( mfcInfSub.IsOpen() ){
			//	レコード終了
			mfcInfSub.Fin();
		}
	}
	return strSortItem;
}

// midori 190505 add -->
// -----------------------------------------------------------------------------------------------------------------------
//	全ての０円判定項目のSQL文を作成する
//		【引数】	なし
//					
//		【戻値】	CString SQLのWHERE句の一部
// -----------------------------------------------------------------------------------------------------------------------
CString CdbUc021Uketoritegata::GetSQLZeroJudgment()
{
	CString strSQL;

	if( (m_OutZeroNull & BIT_D0) && (m_OutZeroNull & BIT_D1) )	strSQL.Format(_T("(FgFunc <> %d and (%s = 0 or %s IS NULL))"), ID_FGFUNC_KAMOKU, GetZeroMoneyMoveField(), GetZeroMoneyMoveField());
	else if( m_OutZeroNull & BIT_D0 )							strSQL.Format(_T("((%s = 0) AND (%s IS NOT NULL))"), GetZeroMoneyMoveField(), GetZeroMoneyMoveField());
	else if( m_OutZeroNull & BIT_D1 )							strSQL.Format(_T("(FgFunc <> %d and (%s IS NULL))"), ID_FGFUNC_KAMOKU, GetZeroMoneyMoveField());

	return strSQL;
}
// midori 190505 add <--
