// dbUc141Yakuin.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
//#include "UCHIWAKE.h"
#include "dbUc141Yakuin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CdbUc141Yakuin

IMPLEMENT_DYNAMIC(CdbUc141Yakuin, CdbUc000Common)

CdbUc141Yakuin::CdbUc141Yakuin(CDatabase* pdb)
	: CdbUc000Common(pdb)
{
	//{{AFX_FIELD_INIT(CdbUc141Yakuin)
	m_NameYaku = _T("");
	m_NameTant = _T("");
	m_AdSeq = 0;
	m_AdOrder = 0;
	m_AdName1 = _T("");
	m_AdName2 = _T("");
	m_AdAdd1 = _T("");
	m_AdAdd2 = _T("");
	m_Kankei = _T("");
	m_FpTime = 0;
	m_EmVal = _T("");				// 2006.10.23：新会社法対応
	m_PrVal = _T("");				// 2006.10.23：新会社法対応
	m_BfVal = _T("");				// 2006.10.23：新会社法対応
	m_PfVal = _T("");				// 2006.10.23：新会社法対応
	m_EtVal = _T("");				// 2006.10.23：新会社法対応
	m_RtVal = _T("");				// 2006.10.23：新会社法対応
	m_Val = _T("");					// 2006.10.23：新会社法対応
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
	m_nFields = 29;					// 2006.10.23：新会社法対応
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	m_lo_pdb = pdb;								// データベースポインタを保持
	m_lo_TableName = STR_TBL_NAME_141;			// テーブル名を記述
}


CString CdbUc141Yakuin::GetDefaultSQL()
{
	return _T("[dbo].[uc_141_yakuin]");
}

void CdbUc141Yakuin::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CdbUc141Yakuin)
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
	RFX_Text(pFX, _T("[NameYaku]"), m_NameYaku);
	RFX_Text(pFX, _T("[NameTant]"), m_NameTant);
	RFX_Long(pFX, _T("[AdSeq]"), m_AdSeq);
	RFX_Long(pFX, _T("[AdOrder]"), m_AdOrder);
	RFX_Text(pFX, _T("[AdName1]"), m_AdName1);
	RFX_Text(pFX, _T("[AdName2]"), m_AdName2);
	RFX_Text(pFX, _T("[AdAdd1]"), m_AdAdd1);
	RFX_Text(pFX, _T("[AdAdd2]"), m_AdAdd2);
	RFX_Text(pFX, _T("[Kankei]"), m_Kankei);
	RFX_Byte(pFX, _T("[FpTime]"), m_FpTime);
	RFX_Text(pFX, _T("[EmVal]"), m_EmVal);			// 2006.10.23：新会社法対応
	RFX_Text(pFX, _T("[PrVal]"), m_PrVal);			// 2006.10.23：新会社法対応
	RFX_Text(pFX, _T("[BfVal]"), m_BfVal);			// 2006.10.23：新会社法対応
	RFX_Text(pFX, _T("[PfVal]"), m_PfVal);			// 2006.10.23：新会社法対応
	RFX_Text(pFX, _T("[EtVal]"), m_EtVal);			// 2006.10.23：新会社法対応
	RFX_Text(pFX, _T("[RtVal]"), m_RtVal);			// 2006.10.23：新会社法対応
	RFX_Text(pFX, _T("[Val]"), m_Val);				// 2006.10.23：新会社法対応
	RFX_Byte(pFX, _T("[FgInput]"), m_FgInput);
	RFX_Byte(pFX, _T("[ShowKeiZero]"), m_ShowKeiZero);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CdbUc141Yakuin 診断

#ifdef _DEBUG
void CdbUc141Yakuin::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbUc141Yakuin::Dump(CDumpContext& dc) const
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
int CdbUc141Yakuin::CreateNewRecord()
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

	m_NameYaku = _T("");	// ("") 役職名
	m_NameTant = _T("");	// ("") 担当業務
	m_AdSeq = 0;			// (0)  (取引先)シーケンス番号
	m_AdOrder = 0;			// (0)  (取引先)順序番号
	m_AdName1 = _T("");		// ("") (取引先)氏名
	m_AdName2 = _T("");		// ("") (取引先)
	m_AdAdd1 = _T("");		// ("") (取引先)住所
	m_AdAdd2 = _T("");		// ("") (取引先)
	m_Kankei = _T("");		// ("") 関係
	m_FpTime = 0;			// (0)  常勤、非常勤
	m_EmVal = _T("");		// ("") 使用人職務分			// 2006.10.23：新会社法対応
	m_PrVal = _T("");		// ("") 定期同額給与			// 2006.10.23：新会社法対応
	m_BfVal = _T("");		// ("") 事前確定届出給与		// 2006.10.23：新会社法対応
	m_PfVal = _T("");		// ("") 利益連動給与			// 2006.10.23：新会社法対応
	m_EtVal = _T("");		// ("") その他					// 2006.10.23：新会社法対応
	m_RtVal = _T("");		// ("") 退職給与				// 2006.10.23：新会社法対応
	m_Val = _T("");			// ("") 役員給与計				// 2006.10.23：新会社法対応
	m_FgInput = 0;			// (0)  自動計算(0)/手入力(1)
	m_ShowKeiZero = 0;		// (0)  ０円計表示フラグ

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
//int CdbUc141Yakuin::CreateNewRecord2(int pNumPage,int pNumRow)
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
int CdbUc141Yakuin::CreateNewRecord2(int pNumPage,int pNumRow,int pNullSw)
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

	m_NameYaku = _T("");	// ("") 役職名
	m_NameTant = _T("");	// ("") 担当業務
	m_AdSeq = 0;			// (0)  (取引先)シーケンス番号
	m_AdOrder = 0;			// (0)  (取引先)順序番号
	m_AdName1 = _T("");		// ("") (取引先)氏名
	m_AdName2 = _T("");		// ("") (取引先)
	m_AdAdd1 = _T("");		// ("") (取引先)住所
	m_AdAdd2 = _T("");		// ("") (取引先)
	m_Kankei = _T("");		// ("") 関係
	m_FpTime = 0;			// (0)  常勤、非常勤
	m_EmVal = _T("");		// ("") 使用人職務分			// 2006.10.23：新会社法対応
	m_PrVal = _T("");		// ("") 定期同額給与			// 2006.10.23：新会社法対応
	m_BfVal = _T("");		// ("") 事前確定届出給与		// 2006.10.23：新会社法対応
	m_PfVal = _T("");		// ("") 利益連動給与			// 2006.10.23：新会社法対応
	m_EtVal = _T("");		// ("") その他					// 2006.10.23：新会社法対応
	m_RtVal = _T("");		// ("") 退職給与				// 2006.10.23：新会社法対応
	m_Val = _T("");			// ("") 役員給与計				// 2006.10.23：新会社法対応
	m_FgInput = 0;			// (0)  自動計算(0)/手入力(1)
	m_ShowKeiZero = 0;		// (0)  ０円計表示フラグ

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
//int CdbUc141Yakuin::UpdateYokukiZeroClear()
//{
//	CString	sql = GetSQLUpdateYokukiZeroClear("Val");			// 役員給与計
//	sql += GetSQLUpdateYokukiZeroClear("EmVal");				// 使用人職務分
//	sql += GetSQLUpdateYokukiZeroClear("PrVal");				// 定期同額給与
//	sql += GetSQLUpdateYokukiZeroClear("BfVal");				// 事前確定届出給与
//	sql += GetSQLUpdateYokukiZeroClear("PfVal");				// 利益連動給与
//	sql += GetSQLUpdateYokukiZeroClear("EtVal");				// その他
//	sql += GetSQLUpdateYokukiZeroClear("RtVal");				// 退職給与
//															
//	sql.Format(sql + "UPDATE %s SET FgInput = %d WHERE  FgFunc <> %d ", m_lo_TableName, ID_AUTOCALC_AUTO, ID_FGFUNC_NULL);		// 自動計算フラグ
//
//	//CString		strCommand;
//	//CString		strTemp;
//
//	//// クリア対象項目（7項目＋自動計算フラグ）
//	//strCommand.Format("UPDATE %s SET ", m_lo_TableName);
//	//strCommand	+=	_T("EmVal = 0, ");						// 使用人職務分
//	//strCommand	+=	_T("PrVal = 0, ");						// 定期同額給与
//	//strCommand	+=	_T("BfVal = 0, ");						// 事前確定届出給与
//	//strCommand	+=	_T("PfVal = 0, ");						// 利益連動給与
//	//strCommand	+=	_T("EtVal = 0, ");						// その他
//	//strCommand	+=	_T("RtVal = 0, ");						// 退職給与
//	//strCommand	+=	_T("Val = 0, ");						// 役員給与計
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
int CdbUc141Yakuin::UpdateYokukiZeroClear( int pSw )
{
// midori 152388 add -->
	CString	cs=_T("");
// midori 152388 add <--
	CString	sql = GetSQLUpdateYokukiZeroClear("Val",pSw);			// 役員給与計
	sql += GetSQLUpdateYokukiZeroClear("EmVal",pSw);				// 使用人職務分
	sql += GetSQLUpdateYokukiZeroClear("PrVal",pSw);				// 定期同額給与
	sql += GetSQLUpdateYokukiZeroClear("BfVal",pSw);				// 事前確定届出給与
	sql += GetSQLUpdateYokukiZeroClear("PfVal",pSw);				// 利益連動給与
	sql += GetSQLUpdateYokukiZeroClear("EtVal",pSw);				// その他
	sql += GetSQLUpdateYokukiZeroClear("RtVal",pSw);				// 退職給与
	sql.Format(sql + "UPDATE %s SET FgInput = %d WHERE  FgFunc <> %d ", m_lo_TableName, ID_AUTOCALC_AUTO, ID_FGFUNC_NULL);		// 自動計算フラグ
// midori 152388 add -->
	if(pSw != 0)	{
		cs.Format(_T("UPDATE %s SET FgFunc=%d WHERE FgFunc<=%d AND "),m_lo_TableName,ID_FGFUNC_NULL,ID_FGFUNC_DATA);
		cs += _T(" NameYaku IS NULL AND NameTant IS NULL AND AdName1 IS NULL AND AdName2 IS NULL");
		cs += _T(" AND Kankei IS NULL AND AdAdd1 IS NULL AND AdAdd2 IS NULL");
		cs += _T(" AND FpTime=0 AND EmVal IS NULL AND PrVal IS NULL");
		cs += _T(" AND BfVal IS NULL AND PfVal IS NULL AND EtVal IS NULL AND RtVal IS NULL AND Val IS NULL");
		sql += cs;
	}
// midori 152388 add <--
	return	ExecuteSQLWork(sql);
}
// midori 160606 cor <--

/**********************************************************************
	GetSQLSumColumn()
		計に表示されるフィールドを加味したSQL文を作成する

	引数
			なし
	戻値
			CString				SQL文
//@
***********************************************************************/
CString CdbUc141Yakuin::GetSQLSumColumn()
{
	CString strSQL;

	strSQL = _T("SUM(EmVal) AS 'Total2', SUM(PrVal) AS 'Total3', SUM(BfVal) AS 'Total4', ");
	strSQL = strSQL + _T("SUM(PfVal) AS 'Total5', SUM(EtVal) AS 'Total6', SUM(RtVal) AS 'Total7',");

	return strSQL;
}

/**********************************************************************
	GetColumnNum()
		計として表示するフィールド数を取得する

	引数
			なし
	戻値
			int				計の個数
//@
***********************************************************************/
int CdbUc141Yakuin::GetColumnNum()
{
	return 7;
}

/**********************************************************************
	GetSQLKeiInsert()
		０円対応で、一時テーブルに挿入するためのINSERT文で、カラムの値を設定する

	引数
			CMyStringArray	カラム名が格納されたアレイ
			CMyArray		計のフィールドを登録するアレイ
			int				FgFuncの値
			CString			計の名称
	戻値
			CString			SQL文
//@
***********************************************************************/
CString CdbUc141Yakuin::GetSQLKeiInsert(int nPage, CMyStringArray* arryColumn, CString strKei/*=0*/, int nFgFunc/*=0*/, CMyArray* arryTotalAll/*=NULL*/)
{
	int nRuikei = 0;
	CString strSQL;
	CString strTmp;
	strSQL.Empty();

	if( nFgFunc != ID_FGFUNC_RUIKEI ){
		// 累計はGrouping = 1でとりたいが、時間がないので断念
		nRuikei = 1;
	}

	for(int i=0; i<(int)arryColumn->GetCount(); i++){
		if( arryColumn->GetAt(i).Compare( _T("Val") ) == 0 ){	// 計として表示する

			if(( arryTotalAll != NULL ) && ( arryTotalAll->GetAt( nPage - nRuikei ).GetAt( 0 ).IsEmpty() == FALSE )){
				strSQL.Format( strSQL + _T("'%s'") , arryTotalAll->GetAt( nPage - nRuikei ).GetAt( 0 ) );
			}
			else{
				strSQL = strSQL + _T("NULL");
			}
		}
		else if( arryColumn->GetAt(i).Compare( _T("EmVal") ) == 0 ){	// 計として表示する2個目

			if(( arryTotalAll != NULL ) && ( arryTotalAll->GetAt( nPage - nRuikei ).GetAt( 1 ).IsEmpty() == FALSE )){
				strSQL.Format( strSQL + _T("'%s'") , arryTotalAll->GetAt( nPage - nRuikei ).GetAt( 1 ) );
			}
			else{
				strSQL = strSQL + _T("NULL");
			}
		}
		else if( arryColumn->GetAt(i).Compare( _T("PrVal") ) == 0 ){	// 計として表示する3個目

			if(( arryTotalAll != NULL ) && ( arryTotalAll->GetAt( nPage - nRuikei ).GetAt( 2 ).IsEmpty() == FALSE )){
				strSQL.Format( strSQL + _T("'%s'") , arryTotalAll->GetAt( nPage - nRuikei ).GetAt( 2 ) );
			}
			else{
				strSQL = strSQL + _T("NULL");
			}
		}
		else if( arryColumn->GetAt(i).Compare( _T("BfVal") ) == 0 ){	// 計として表示する4個目

			if(( arryTotalAll != NULL ) && ( arryTotalAll->GetAt( nPage - nRuikei ).GetAt( 3 ).IsEmpty() == FALSE )){
				strSQL.Format( strSQL + _T("'%s'") , arryTotalAll->GetAt( nPage - nRuikei ).GetAt( 3 ) );
			}
			else{
				strSQL = strSQL + _T("NULL");
			}
		}
		else if( arryColumn->GetAt(i).Compare( _T("PfVal") ) == 0 ){	// 計として表示する5個目

			if(( arryTotalAll != NULL ) && ( arryTotalAll->GetAt( nPage - nRuikei ).GetAt( 4 ).IsEmpty() == FALSE )){
				strSQL.Format( strSQL + _T("'%s'") , arryTotalAll->GetAt( nPage - nRuikei ).GetAt( 4 ) );
			}
			else{
				strSQL = strSQL + _T("NULL");
			}
		}
		else if( arryColumn->GetAt(i).Compare( _T("EtVal") ) == 0 ){	// 計として表示する6個目

			if(( arryTotalAll != NULL ) && ( arryTotalAll->GetAt( nPage - nRuikei ).GetAt( 5 ).IsEmpty() == FALSE )){
				strSQL.Format( strSQL + _T("'%s'") , arryTotalAll->GetAt( nPage - nRuikei ).GetAt( 5 ) );
			}
			else{
				strSQL = strSQL + _T("NULL");
			}
		}
		else if( arryColumn->GetAt(i).Compare( _T("RtVal") ) == 0 ){	// 計として表示する7個目

			if(( arryTotalAll != NULL ) && ( arryTotalAll->GetAt( nPage - nRuikei ).GetAt( 6 ).IsEmpty() == FALSE )){
				strSQL.Format( strSQL + _T("'%s'") , arryTotalAll->GetAt( nPage - nRuikei ).GetAt( 6 ) );
			}
			else{
				strSQL = strSQL + _T("NULL");
			}
		}
		else if( arryColumn->GetAt(i).Compare( _T("KeiStr") ) == 0 ){	//　計の名称

			if( strKei.IsEmpty() ){		// 空の場合はNULLとする
				strSQL = strSQL + _T("NULL");
			}
			else{
				strSQL.Format( strSQL + _T("'%s'") , strKei);
			}
		}
		else if( arryColumn->GetAt(i).Compare( _T("FgFunc") ) == 0 ){	// FgFunc
			strTmp.Format("%d", nFgFunc);
			strSQL = strSQL + strTmp;
		}
		else{
			strSQL = strSQL + _T("NULL");
		}

		if( i == (int)arryColumn->GetCount() -1 ){
			break;
		}

		strSQL = strSQL + _T(",");
	}

	return strSQL;
}

//********************************************************************************
//	全ての０円判定項目のSQL分を作成する
//		【引数】	なし
//					
//		【戻値】	CString SQLのWHERE句の一部
//********************************************************************************
CString CdbUc141Yakuin::GetSQLZeroJudgment()
{
	CString strSQL;

	//strSQL.Format("(%s <> 0 or EmVal <> 0 or PrVal <> 0 or BfVal <> 0 or PfVal <> 0 or EtVal <> 0 or RtVal <> 0)", GetZeroMoneyMoveField() );
	//strSQL.Format("((%s = 0 OR %s IS NULL) AND (EmVal = 0 OR EmVal IS NULL) AND (PrVal = 0 OR PrVal IS NULL) AND (BfVal = 0 OR BfVal IS NULL) AND (PfVal = 0 OR PfVal IS NULL) AND (EtVal = 0 OR EtVal IS NULL) AND (RtVal = 0 OR RtVal IS NULL)) ", 
	//	GetZeroMoneyMoveField(), GetZeroMoneyMoveField() );
	if((m_OutZeroNull & BIT_D0) && (m_OutZeroNull & BIT_D1))	{
		strSQL.Format("((%s = 0 OR %s IS NULL) AND (EmVal = 0 OR EmVal IS NULL) AND (PrVal = 0 OR PrVal IS NULL) AND "
			"(BfVal = 0 OR BfVal IS NULL) AND (PfVal = 0 OR PfVal IS NULL) AND (EtVal = 0 OR EtVal IS NULL) AND (RtVal = 0 OR RtVal IS NULL)) ", GetZeroMoneyMoveField(), GetZeroMoneyMoveField() );
	}
	else if(m_OutZeroNull & BIT_D0)
		strSQL.Format("(((%s = 0) AND (%s IS NOT NULL)) AND ((EmVal = 0) AND (EmVal IS NOT NULL)) AND "
			"((PrVal = 0) AND (PrVal IS NOT NULL)) AND ((BfVal = 0) AND (BfVal IS NOT NULL)) AND ((PfVal = 0) AND "
			"(PfVal IS NOT NULL)) AND ((EtVal = 0) AND (EtVal IS NOT NULL)) AND ((RtVal = 0) AND (RtVal IS NOT NULL))) ", GetZeroMoneyMoveField(), GetZeroMoneyMoveField() );
	else if(m_OutZeroNull & BIT_D1)
		strSQL.Format("((%s IS NULL) AND (EmVal IS NULL) AND (PrVal IS NULL) AND (BfVal IS NULL) AND (PfVal IS NULL) AND (EtVal IS NULL) AND (RtVal IS NULL)) ", GetZeroMoneyMoveField() );

	return strSQL;
}

//********************************************************************************
//	where Val <> 0 で条件付けても、ValがNULLは引っかからないので、Val is nullを付加する
//		【引数】	なし
//					
//		【戻値】	CString SQLのWHERE句の一部
//********************************************************************************
CString CdbUc141Yakuin::GetSQLValIsNull()
{
	CString strSQL;

	strSQL.Format(_T("OR ((%s IS NULL) AND (EmVal IS NULL) AND (PrVal IS NULL) AND (BfVal IS NULL) AND (PfVal IS NULL) AND (EtVal IS NULL) AND (RtVal IS NULL)) AND FgFunc <> %d"), GetZeroMoneyMoveField(), ID_FGFUNC_NULL);

	return strSQL;
}