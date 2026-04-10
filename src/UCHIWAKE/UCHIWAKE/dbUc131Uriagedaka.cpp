// dbUc131Uriagedaka.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
//#include "uchiwake.h"
#include "dbUc131Uriagedaka.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CdbUc131Uriagedaka

IMPLEMENT_DYNAMIC(CdbUc131Uriagedaka, CdbUc000Common)

CdbUc131Uriagedaka::CdbUc131Uriagedaka(CDatabase* pdb)
	: CdbUc000Common(pdb)
{
	//{{AFX_FIELD_INIT(CdbUc131Uriagedaka)
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
	m_AdAdd1 = _T("");
	m_AdAdd2 = _T("");
	m_HdName = _T("");
	m_Kankei = _T("");
	m_Naiyou = _T("");
	m_Val = _T("");
	m_Val2 = _T("");
	m_Worker = _T("");
	m_Area = _T("");
	m_HdName2 = _T("");
	m_Teki = _T("");
	m_ShowKeiZero = 0;
	m_nFields = 26;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	m_lo_pdb = pdb;								// データベースポインタを保持
	m_lo_TableName = STR_TBL_NAME_131;			// テーブル名を記述
}


CString CdbUc131Uriagedaka::GetDefaultSQL()
{
	return _T("[dbo].[uc_131_uriagedaka]");
}

void CdbUc131Uriagedaka::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CdbUc131Uriagedaka)
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
	RFX_Text(pFX, _T("[AdAdd1]"), m_AdAdd1);
	RFX_Text(pFX, _T("[AdAdd2]"), m_AdAdd2);
	RFX_Text(pFX, _T("[HdName]"), m_HdName);
	RFX_Text(pFX, _T("[Kankei]"), m_Kankei);
	RFX_Text(pFX, _T("[Naiyou]"), m_Naiyou);
	RFX_Text(pFX, _T("[Val]"), m_Val);
	RFX_Text(pFX, _T("[Val2]"), m_Val2);
	RFX_Text(pFX, _T("[Worker]"), m_Worker);
	RFX_Text(pFX, _T("[Area]"), m_Area);
	RFX_Text(pFX, _T("[HdName2]"), m_HdName2);
	RFX_Text(pFX, _T("[Teki]"), m_Teki);
	RFX_Byte(pFX, _T("[ShowKeiZero]"), m_ShowKeiZero);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CdbUc131Uriagedaka 診断

#ifdef _DEBUG
void CdbUc131Uriagedaka::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbUc131Uriagedaka::Dump(CDumpContext& dc) const
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
int CdbUc131Uriagedaka::CreateNewRecord()
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

	m_AdSeq = 0;			// (0)  事業所：(取引)シーケンス番号
	m_AdOrder = 0;			// (0)  事業所：(取引)順序番号
	m_AdName1 = _T("");		// ("") 事業所：(取引)事業所の名称(1行目)
	m_AdName2 = _T("");		// ("") 事業所：(取引)事業所の名称(1行目)
	m_AdAdd1 = _T("");		// ("") 事業所：(取引)所在地
	m_AdAdd2 = _T("");		// ("") 事業所：(取引)所在地
	m_HdName = _T("");		// ("") 責任者氏名
	m_Kankei = _T("");		// ("") 代表者との関係
	m_Naiyou = _T("");		// ("") 事業等の内容
	m_Val = _T("");			// ("") 売上高
	m_Val2 = _T("");		// ("") 期末残高棚卸
	m_Worker = _T("");		// ("") 期末従業員数
	m_Area = _T("");		// ("") 使用建物の延面積
	m_HdName2 = _T("");		// ("") 源泉所得税納付先署
	m_Teki = _T("");		// ("") 摘要
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
//int CdbUc131Uriagedaka::CreateNewRecord2(int pNumPage,int pNumRow)
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
int CdbUc131Uriagedaka::CreateNewRecord2(int pNumPage,int pNumRow,int pNullSw)
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

	m_AdSeq = 0;			// (0)  事業所：(取引)シーケンス番号
	m_AdOrder = 0;			// (0)  事業所：(取引)順序番号
	m_AdName1 = _T("");		// ("") 事業所：(取引)事業所の名称(1行目)
	m_AdName2 = _T("");		// ("") 事業所：(取引)事業所の名称(1行目)
	m_AdAdd1 = _T("");		// ("") 事業所：(取引)所在地
	m_AdAdd2 = _T("");		// ("") 事業所：(取引)所在地
	m_HdName = _T("");		// ("") 責任者氏名
	m_Kankei = _T("");		// ("") 代表者との関係
	m_Naiyou = _T("");		// ("") 事業等の内容
	m_Val = _T("");			// ("") 売上高
	m_Val2 = _T("");		// ("") 期末残高棚卸
	m_Worker = _T("");		// ("") 期末従業員数
	m_Area = _T("");		// ("") 使用建物の延面積
	m_HdName2 = _T("");		// ("") 源泉所得税納付先署
	m_Teki = _T("");		// ("") 摘要
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
//int CdbUc131Uriagedaka::UpdateYokukiZeroClear()
//{
//	CString	sql = GetSQLUpdateYokukiZeroClear("Val");		// 売上高
//	sql += GetSQLUpdateYokukiZeroClear("Val2");				// 期末棚卸高
//
//	//CString		strCommand;
//	//CString		strTemp;
//
//	//// クリア対象項目（2項目）
//	//strCommand.Format("UPDATE %s SET ", m_lo_TableName);
//	//strCommand	+=	_T("Val = 0, ");						// 売上高
//	//strCommand	+=	_T("Val2 = 0 ");						// 期末棚卸高
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
int CdbUc131Uriagedaka::UpdateYokukiZeroClear( int pSw )
{
// midori 152388 add -->
	CString	cs=_T("");
// midori 152388 add <--
	CString	sql = GetSQLUpdateYokukiZeroClear("Val",pSw);		// 売上高
	sql += GetSQLUpdateYokukiZeroClear("Val2",pSw);				// 期末棚卸高
// midori 152388 add -->
	if(pSw != 0)	{
		cs.Format(_T("UPDATE %s SET FgFunc=%d WHERE FgFunc<=%d AND "),m_lo_TableName,ID_FGFUNC_NULL,ID_FGFUNC_DATA);
		cs += _T(" AdName1 IS NULL AND AdAdd1 IS NULL AND AdAdd2 IS NULL");
		cs += _T(" AND HdName IS NULL AND Kankei IS NULL AND Naiyou IS NULL");
		cs += _T(" AND Val IS NULL AND Val2 IS NULL AND Worker IS NULL");
		cs += _T(" AND Area IS NULL AND HdName2 IS NULL AND Teki IS NULL");
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
int CdbUc131Uriagedaka::UpdateDataTableWork()
{
	int	retVal1 = UpdateDataTableNullToStrNull("AdName1");
	int	retVal2 = UpdateDataTableNullToStrNull("AdName2");

	if (retVal1 != DB_ERR_OK) {
		return	retVal1;
	}
	return retVal2;
}

/**********************************************************************
	GetSQLSumColumn()
		計に表示されるフィールドを加味したSQL文を作成する

	引数
			なし
	戻値
			CString				SQL文
//@
***********************************************************************/
CString CdbUc131Uriagedaka::GetSQLSumColumn()
{
	CString strSQL;

	strSQL = _T("SUM(Val2) AS 'Total2', SUM(Worker) AS 'Total3', SUM(Area) AS 'Total4',");

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
int CdbUc131Uriagedaka::GetColumnNum()
{
	return 4;
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
CString CdbUc131Uriagedaka::GetSQLKeiInsert(int nPage, CMyStringArray* arryColumn, CString strKei/*=0*/, int nFgFunc/*=0*/, CMyArray* arryTotalAll/*=NULL*/)
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
		else if( arryColumn->GetAt(i).Compare( _T("Val2") ) == 0 ){	// 計として表示する2個目

			if(( arryTotalAll != NULL ) && ( arryTotalAll->GetAt( nPage - nRuikei ).GetAt( 1 ).IsEmpty() == FALSE )){
				strSQL.Format( strSQL + _T("'%s'") , arryTotalAll->GetAt( nPage - nRuikei ).GetAt( 1 ) );
			}
			else{
				strSQL = strSQL + _T("NULL");
			}
		}
		else if( arryColumn->GetAt(i).Compare( _T("Worker") ) == 0 ){	// 計として表示する3個目

			if(( arryTotalAll != NULL ) && ( arryTotalAll->GetAt(nPage - nRuikei).GetAt( 2 ).IsEmpty() == FALSE )){
				strSQL.Format( strSQL + _T("'%s'") , arryTotalAll->GetAt( nPage - nRuikei ).GetAt( 2 ) );
			}
			else{
				strSQL = strSQL + _T("NULL");
			}
		}
		else if( arryColumn->GetAt(i).Compare( _T("Area") ) == 0 ){	// 計として表示する4個目

			if(( arryTotalAll != NULL ) && ( arryTotalAll->GetAt( nPage - nRuikei ).GetAt( 3 ).IsEmpty() == FALSE )){
				strSQL.Format( strSQL + _T("'%s'") , arryTotalAll->GetAt( nPage - nRuikei ).GetAt( 3 ) );
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
CString CdbUc131Uriagedaka::GetSQLZeroJudgment()
{
	CString strSQL;

	//strSQL.Format("(%s <> 0 or Val2 <> 0 or Worker <> 0 or Area <> 0)", GetZeroMoneyMoveField() );
	//strSQL.Format("((%s = 0 OR %s IS NULL) AND (Val2 = 0 OR Val2 IS NULL)) ", GetZeroMoneyMoveField(), GetZeroMoneyMoveField() );
	if((m_OutZeroNull & BIT_D0) && (m_OutZeroNull & BIT_D1))
		strSQL.Format("((%s = 0 OR %s IS NULL) AND (Val2 = 0 OR Val2 IS NULL)) ", GetZeroMoneyMoveField(), GetZeroMoneyMoveField() );
	else if(m_OutZeroNull & BIT_D0)
		strSQL.Format("(((%s = 0) AND (%s IS NOT NULL)) AND ((Val2 = 0) AND (Val2 IS NOT NULL))) ", GetZeroMoneyMoveField(), GetZeroMoneyMoveField() );
	else if(m_OutZeroNull & BIT_D1)
		strSQL.Format("((%s IS NULL) AND (Val2 IS NULL)) ", GetZeroMoneyMoveField() );

	return strSQL;
}

//********************************************************************************
//	where Val <> 0 で条件付けても、ValがNULLは引っかからないので、Val is nullを付加する
//		【引数】	なし
//					
//		【戻値】	CString SQLのWHERE句の一部
//********************************************************************************
CString CdbUc131Uriagedaka::GetSQLValIsNull()
{
	CString strSQL;

	strSQL.Format(_T("OR ((%s IS NULL) AND (Val2 IS NULL)) AND FgFunc <> %d"), GetZeroMoneyMoveField(), ID_FGFUNC_NULL);

	return strSQL;
}