// dbUc153Kougyou.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
//#include "uchiwake.h"
#include "dbUc153Kougyou.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// インボイス登録番号追加対応_23/12/25 add -->
extern BOOL bG_InvNo;			// インボイス番号使用サイン（決算期末が令和6年3月1日を超えている）
// インボイス登録番号追加対応_23/12/25 add <--

/////////////////////////////////////////////////////////////////////////////
// CdbUc153Kougyou

IMPLEMENT_DYNAMIC(CdbUc153Kougyou, CdbUc000Common)

CdbUc153Kougyou::CdbUc153Kougyou(CDatabase* pdb)
	: CdbUc000Common(pdb)
{
	//{{AFX_FIELD_INIT(CdbUc153Kougyou)
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
	m_HdName = _T("");
	m_AdSeq = 0;
	m_AdOrder = 0;
	m_AdName1 = _T("");
	m_AdName2 = _T("");
	m_AdAdd1 = _T("");
	m_AdAdd2 = _T("");
	m_KyDate = 0;
	m_KyDate2 = 0;
	m_SpDate = 0;
	m_SpDate2 = 0;
	m_Val = _T("");
	m_Teki = _T("");
	m_ShowKeiZero = 0;
// インボイス登録番号追加対応_23/10/23 del -->
	//m_nFields = 24;
// インボイス登録番号追加対応_23/10/23 del <--
// インボイス登録番号追加対応_23/10/23 add -->
	m_InvNo = _T("");
	m_nFields = 25;
// インボイス登録番号追加対応_23/10/23 add <--
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	m_lo_pdb = pdb;								// データベースポインタを保持
	m_lo_TableName = STR_TBL_NAME_153;			// テーブル名を記述
}


CString CdbUc153Kougyou::GetDefaultSQL()
{
	return _T("[dbo].[uc_153_kougyou]");
}

void CdbUc153Kougyou::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CdbUc153Kougyou)
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
	RFX_Text(pFX, _T("[HdName]"), m_HdName);
	RFX_Long(pFX, _T("[AdSeq]"), m_AdSeq);
	RFX_Long(pFX, _T("[AdOrder]"), m_AdOrder);
	RFX_Text(pFX, _T("[AdName1]"), m_AdName1);
	RFX_Text(pFX, _T("[AdName2]"), m_AdName2);
	RFX_Text(pFX, _T("[AdAdd1]"), m_AdAdd1);
	RFX_Text(pFX, _T("[AdAdd2]"), m_AdAdd2);
	RFX_Long(pFX, _T("[KyDate]"), m_KyDate);
	RFX_Long(pFX, _T("[KyDate2]"), m_KyDate2);
	RFX_Long(pFX, _T("[SpDate]"), m_SpDate);
	RFX_Long(pFX, _T("[SpDate2]"), m_SpDate2);
	RFX_Text(pFX, _T("[Val]"), m_Val);
	RFX_Text(pFX, _T("[Teki]"), m_Teki);
	RFX_Byte(pFX, _T("[ShowKeiZero]"), m_ShowKeiZero);
	RFX_Text(pFX, _T("[Invno]"), m_InvNo);	// インボイス登録番号追加対応_23/10/20 add
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CdbUc153Kougyou 診断

#ifdef _DEBUG
void CdbUc153Kougyou::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbUc153Kougyou::Dump(CDumpContext& dc) const
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
int CdbUc153Kougyou::CreateNewRecord()
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

	m_HdName = _T("");		// ("") 名称
	m_AdSeq = 0;			// (0)  支払先：(取引)シーケンス番号
	m_AdOrder = 0;			// (0)  支払先：(取引)順序番号
	m_AdName1 = _T("");		// ("") 支払先：(取引)名称(1行目)
	m_AdName2 = _T("");		// ("") 支払先：(取引)名称(2行目)
	m_AdAdd1 = _T("");		// ("") 支払先：(取引)所在地(1行目)
	m_AdAdd2 = _T("");		// ("") 支払先：(取引)所在地(2行目)
	m_KyDate = 0;			// (0)  契約期間(開始)
	m_KyDate2 = 0;			// (0)  契約期間(終了)
	m_SpDate = 0;			// (0)  支払対象期間(開始)
	m_SpDate2 = 0;			// (0)  支払対象期間(終了)
	m_Val = _T("");			// ("") 支払金額
	m_Teki = _T("");		// ("") 摘要
	m_ShowKeiZero = 0;		// (0)  ０円計表示フラグ
	m_InvNo = _T("");		// ("") 登録番号		インボイス登録番号追加対応_23/10/20 add

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
//int CdbUc153Kougyou::CreateNewRecord2(int pNumPage,int pNumRow)
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
int CdbUc153Kougyou::CreateNewRecord2(int pNumPage,int pNumRow,int pNullSw)
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

	m_HdName = _T("");		// ("") 名称
	m_AdSeq = 0;			// (0)  支払先：(取引)シーケンス番号
	m_AdOrder = 0;			// (0)  支払先：(取引)順序番号
	m_AdName1 = _T("");		// ("") 支払先：(取引)名称(1行目)
	m_AdName2 = _T("");		// ("") 支払先：(取引)名称(2行目)
	m_AdAdd1 = _T("");		// ("") 支払先：(取引)所在地(1行目)
	m_AdAdd2 = _T("");		// ("") 支払先：(取引)所在地(2行目)
	m_KyDate = 0;			// (0)  契約期間(開始)
	m_KyDate2 = 0;			// (0)  契約期間(終了)
	m_SpDate = 0;			// (0)  支払対象期間(開始)
	m_SpDate2 = 0;			// (0)  支払対象期間(終了)
	m_Val = _T("");			// ("") 支払金額
	m_Teki = _T("");		// ("") 摘要
	m_ShowKeiZero = 0;		// (0)  ０円計表示フラグ
	m_InvNo = _T("");		// ("") 登録番号		インボイス登録番号追加対応_23/10/20 add

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
//int CdbUc153Kougyou::UpdateYokukiZeroClear()
//{
//	CString	sql = GetSQLUpdateYokukiZeroClear("Val");			// 支払金額
//	sql += GetSQLUpdateYokukiZeroClear("SpDate");				// 支払対象期間（開始）
//	sql += GetSQLUpdateYokukiZeroClear("SpDate2");				// 支払対象期間（終了）
//	//CString		strCommand;
//	//CString		strTemp;
//
//	//// クリア対象項目（3項目）
//	//strCommand.Format("UPDATE %s SET ", m_lo_TableName);
//	//strCommand	+=	_T("SpDate = 0, ");						// 支払対象期間（開始）
//	//strCommand	+=	_T("SpDate2 = 0, ");					// 支払対象期間（終了）
//	//strCommand	+=	_T("Val = 0 ");							// 支払金額
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
int CdbUc153Kougyou::UpdateYokukiZeroClear( int pSw )
{
// midori 152388 add -->
	CString	cs=_T("");
// midori 152388 add <--
	CString	sql = GetSQLUpdateYokukiZeroClear("Val",pSw);			// 支払金額
	sql += GetSQLUpdateYokukiZeroClear("SpDate",0);					// 支払対象期間（開始）
	sql += GetSQLUpdateYokukiZeroClear("SpDate2",0);				// 支払対象期間（終了）
// midori 152388 add -->
	if(pSw != 0)	{
		cs.Format(_T("UPDATE %s SET FgFunc=%d WHERE FgFunc<=%d AND "),m_lo_TableName,ID_FGFUNC_NULL,ID_FGFUNC_DATA);
		cs += _T(" HdName IS NULL");
		cs += _T(" AND AdName1 IS NULL AND AdName2 IS NULL");
		cs += _T(" AND AdAdd1 IS NULL AND AdAdd2 IS NULL");
		cs += _T(" AND KyDate=0 AND KyDate2=0 AND SpDate=0 AND SpDate2=0");
		cs += _T(" AND Val IS NULL AND Teki IS NULL");
		if(bG_InvNo == TRUE)	cs += _T(" AND Invno IS NULL");			// インボイス登録番号追加対応_23/12/25 add
		sql += cs;
	}
// midori 152388 add <--
	return	ExecuteSQLWork(sql);
}
// midori 160606 cor <--

/**********************************************************************
	UpdateDataTableWork()
		ソート前にデータを整えるために実行される。

	AdName1		NULLになっているものを NULL文字列('')に更新
	AdName2		NULLになっているものを NULL文字列('')に更新


	引数
			なし
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
// virtual
int CdbUc153Kougyou::UpdateDataTableWork()
{
	int	retVal1 = UpdateDataTableNullToStrNull("AdName1");
	int	retVal2 = UpdateDataTableNullToStrNull("AdName2");

	if (retVal1 != DB_ERR_OK) {
		return	retVal1;
	}
	return retVal2;
}