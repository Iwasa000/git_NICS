// dbUc052Tanaoroshi2.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
//#include "UCHIWAKE.h"
#include "dbUc052Tanaoroshi2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CdbUc052Tanaoroshi2

IMPLEMENT_DYNAMIC(CdbUc052Tanaoroshi2, CdbUc000Common)

CdbUc052Tanaoroshi2::CdbUc052Tanaoroshi2(CDatabase* pdb)
	: CdbUc000Common(pdb)
{
	//{{AFX_FIELD_INIT(CdbUc052Tanaoroshi2)
	m_Method = 0;
	m_HdName = _T("");
	m_SpDate = 0;
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
	m_OutRangai = 0;
	m_nFields = 15;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	m_lo_pdb = pdb;								// データベースポインタを保持
	m_lo_TableName = STR_TBL_NAME_052;			// テーブル名を記述
}

CString CdbUc052Tanaoroshi2::GetDefaultSQL()
{
	return _T("[dbo].[uc_052_tanaoroshi2]");
}

void CdbUc052Tanaoroshi2::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CdbUc052Tanaoroshi2)
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
	RFX_Byte(pFX, _T("[Method]"), m_Method);
	RFX_Text(pFX, _T("[HdName]"), m_HdName);
	RFX_Long(pFX, _T("[SpDate]"), m_SpDate);	
	RFX_Byte(pFX, _T("[ShowKeiZero]"), m_ShowKeiZero);
	RFX_Byte(pFX, _T("[OutRangai]"), m_OutRangai);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CdbUc052Tanaoroshi2 診断

#ifdef _DEBUG
void CdbUc052Tanaoroshi2::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbUc052Tanaoroshi2::Dump(CDumpContext& dc) const
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
int CdbUc052Tanaoroshi2::CreateNewRecord()
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

	m_Method = 0;			// (0)  棚卸方法（0：未選択）
	m_HdName = _T("");		// ("") 棚卸 その他の方法
	m_SpDate = 0;			// (0)  棚卸を行った時期
	m_ShowKeiZero = 0;		// (0)  ０円計表示フラグ
	m_OutRangai = 0;		// (0)　欄外出力タイプ(0:1ページ　1:最終ページ　2:全ページ)

	Update();

	m_strFilter.Format( "Seq = %d", retVal );	// 削除フラグ OFF（0:有効なレコード)
	m_strSort   = "";							// 並び順でソート

	return RequeryWork();
}

/**********************************************************************
	Init()
		初期化処理（Open()の代わりとして使用）

	引数
								なし

	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗

***********************************************************************/
int CdbUc052Tanaoroshi2::Init()
{
	int	nRet = DB_ERR_OK;	//	戻値
	
	//	抽出条件設定（※１行しか存在しないからシーケンス番号「1」固定）
	m_strFilter.Format("Seq = 1");
	m_strSort = "";
	
	//	リクエリ実行
	nRet = RequeryWork();
	
	//	成功？
	if ( nRet == DB_ERR_OK ){
		
		//	レコードなし？
		if ( IsEOF() )	{

			//	強制的に１レコード追加
			AddNew();							//	追加
			m_Seq = 1;							//	シーケンス番号（1固定）
			m_NumPage = 1;						//	頁番号（1固定）
			m_NumRow = 1;						//	行番号（1固定）
			m_FgFunc = ID_FGFUNC_TANAOROSHI;	//	特殊行フラグ（201固定）
			m_FgShow = 0;						//	表示フラグ（0：印刷する）
			m_KeiStr = _T("");					//	「計」文言文字列格納用
			m_NumGroup = 0;						//  グループ番号
			m_Method = 4;						//	期末棚卸の方法(4:登録名称を印字)
			m_HdName = _T("");					//	登録名称
			m_SpDate = 0;						//	棚卸を行った時期
			m_ShowKeiZero = 0;					//　未使用
			m_OutRangai = 0;					//　欄外出力タイプ
			Update();							//	更新
			Requery();							//	リクエリ
		}		
	}
	
	//	戻値を返す
	return( nRet );
}

/**********************************************************************
	Init()
		初期化処理（Open()の代わりとして使用）

	引数
			int					ページ数

	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗

***********************************************************************/
int CdbUc052Tanaoroshi2::Init(int page)
{
	int	nRet = DB_ERR_OK;	//	戻値
	
	m_strFilter.Format("NumPage = %d", page);
	m_strSort = "";
	
	nRet = RequeryWork();
	
	if ( nRet == DB_ERR_OK ){
		if ( IsEOF() )	{
			//	強制的に１レコード追加
			AddNew();							//	追加
			m_Seq = page;						//	シーケンス番号（1固定→F9頁追加、削除により増減）
			m_NumPage = page;					//	頁番号（1固定→F9頁追加、削除により増減　ただし、電子申告でページ数を1～見ている為、必ず１から１ずつインクリメントすること）
			m_NumRow = 1;						//	行番号（1固定）
			m_FgFunc = ID_FGFUNC_TANAOROSHI;	//	特殊行フラグ（201固定）
			m_FgShow = 0;						//	表示フラグ（0：印字する／ID_FGSHOW_TANAOROSHI：印字しない）
			m_KeiStr = _T("");					//	「計」文言文字列格納用
			m_NumGroup = 0;						//  グループ番号
			m_Method = 4;						//	期末棚卸の方法(4:登録名称を印字)
			m_HdName = _T("");					//	登録名称
			m_SpDate = 0;						//	棚卸を行った時期
			m_ShowKeiZero = 0;					//　未使用
			m_OutRangai = 0;					//　欄外出力タイプ
			Update();							//	更新

			RequeryPage(page);
		}
	}
	
	//	戻値を返す
	return( nRet );
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
//int CdbUc052Tanaoroshi2::UpdateYokukiZeroClear()
//{
//	CString	sql = GetSQLUpdateYokukiZeroClear("SpDate");		// 印字しないフラグ
//	sql += GetSQLUpdateYokukiZeroClear("Method");		// 期末棚卸の方法（4:初期値）
//	
//	sql.Format("%s UPDATE %s SET FgShow = %d, OutRangai = 0 WHERE FgFunc <> %d AND Seq = 1", sql, m_lo_TableName, ID_FGSHOW_TANAOROSHI, ID_FGFUNC_NULL);
//
//	//CString		strCommand;
//	//CString		strTemp;
//
//	//// クリア対象項目（3項目）
//	//strCommand.Format("UPDATE %s SET ", m_lo_TableName);
//	//
//	//strTemp.Format("FgShow = %d, ", ID_FGSHOW_TANAOROSHI);	// 印字しないフラグ
//	//strCommand	+=	strTemp;
//	//strCommand	+=	_T("Method = 4, ");						// 期末棚卸の方法（4:初期値）
//	//strCommand	+=	_T("SpDate = 0, ");						// 棚卸を行った時期
//	//strCommand	+ 	_T("OutRangai = 0 ");					// 欄外出力形式
//
//	//// 抽出条件：なし
//	//strTemp = _T("");
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
int CdbUc052Tanaoroshi2::UpdateYokukiZeroClear( int pSw )
{
	CString	sql = GetSQLUpdateYokukiZeroClear("SpDate",0);		// 印字しないフラグ
	sql += GetSQLUpdateYokukiZeroClear("Method",0);				// 期末棚卸の方法（4:初期値）
	sql.Format("%s UPDATE %s SET FgShow = %d, OutRangai = 0 WHERE FgFunc <> %d AND Seq = 1", sql, m_lo_TableName, ID_FGSHOW_TANAOROSHI, ID_FGFUNC_NULL);
	return	ExecuteSQLWork(sql);
}
// midori 160606 cor <--

/**********************************************************************
	UpdateYokukiMoveToHokan()
		翌期更新により現在データを一時保管へ移動する

	引数
			なし
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUc052Tanaoroshi2::UpdateYokukiMoveToHokan()
{
	// 欄外データは実行不可
	return DB_ERR_OK;
}

/**********************************************************************
	UpdateZeroMoneyMoveToHokan()
		「一括で一時保管へ移動」により、金額が０円 or 空欄(NULL)のデータを一時保管へ移動する

	引数
			HOKAN_DATA			保管データ
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUc052Tanaoroshi2::UpdateZeroMoneyMoveToHokan( HOKAN_DATA *hDat )
{
	// 欄外データは実行不可
	return DB_ERR_OK;
}

/**********************************************************************
	GetRangaiData()
		GetPrintPageNum()共通かの為にm_OutRangaiを隠ぺい

	引数
			なし
	戻値
			int		「印字する」、「ページ共通出力」「ページ個別出力」の状態
***********************************************************************/
int CdbUc052Tanaoroshi2::GetRangaiData()
{
	return m_OutRangai;
}

/**********************************************************************
	GetRangaiTblName()
		欄外登録用の一時テーブル名を取得する

	引数
			なし
	戻値
			CString				テーブル名
***********************************************************************/
CString CdbUc052Tanaoroshi2::GetRangaiTblName()
{
	return STR_TBL_NAME_052_TMP;
}

// midori 161107 add -->
/**********************************************************************
	RequeryPageRangai( int pType )
		欄外に有効なデータがあるかを調べる
	引数
			int					0:ページ共通設定	1:ページ個別設定
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
// midori 155525 del -->
//int CdbUc052Tanaoroshi2::RequeryPageRangai( int pType )
// midori 155525 del <--
// midori 155525 add -->
int CdbUc052Tanaoroshi2::RequeryPageRangai( int pType, int pOutZero )
// midori 155525 add <--
{
	CString		strQuery="";

	strQuery.Empty();
// midori 152394 del -->
	//strQuery  = " FgShow = 0 AND ";
	//strQuery += " (Method = 1 OR Method = 2 OR Method = 3 ";
	//strQuery += " OR (Method = 4 AND HdName IS NOT NULL) ";
	//strQuery += " OR SpDate > 0 ) ";
// midori 152394 del <--
// midori 152394 add -->
	strQuery  = " FgShow = 0 AND ";
	strQuery += " (Method = 4 AND HdName IS NOT NULL) OR SpDate > 0 ";
// midori 152394 add <--
	if(pType == 0)	{
		strQuery += "AND (NumPage = 1)";
	}
	m_strFilter.Format(strQuery);

	return RequeryWork();		// リクエリを実行
}
// midori 161107 add <--
