// dbUc171Sonota1.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
//#include "uchiwake.h"
#include "dbUc171Sonota1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// インボイス登録番号追加対応_23/12/25 add -->
extern BOOL bG_InvNo;			// インボイス番号使用サイン（決算期末が令和6年3月1日を超えている）
// インボイス登録番号追加対応_23/12/25 add <--

/////////////////////////////////////////////////////////////////////////////
// CdbUc171Sonota1

IMPLEMENT_DYNAMIC(CdbUc171Sonota1, CdbUc000Common)

//CdbUc171Sonota1::CdbUc171Sonota1(CDatabase* pdb)
CdbUc171Sonota1::CdbUc171Sonota1(CDatabase* pdb, int inFormSeq)
	: CdbUc000Common(pdb)
{
	//{{AFX_FIELD_INIT(CdbUc171Sonota1)
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
	m_FormSeq = 0;
	m_KnSeq = 0;
	m_KnOrder = 0;
	m_KnName = _T("");
	m_AdSeq = 0;
	m_AdOrder = 0;
	m_AdName1 = _T("");
	m_AdName2 = _T("");
	m_AdAdd1 = _T("");
	m_AdAdd2 = _T("");
	m_Val = _T("");
	m_Teki = _T("");
	m_ShowKeiZero = 0;
// midori 152137 del -->
//	m_nFields = 23;
// midori 152137 del <--
// midori 152137 add -->
	m_KnKana = _T("");
// インボイス登録番号追加対応_23/10/23 del -->
	//m_nFields = 24;
// インボイス登録番号追加対応_23/10/23 del <--
// インボイス登録番号追加対応_23/10/23 add -->
	m_InvNo = _T("");
	m_nFields = 25;
// インボイス登録番号追加対応_23/10/23 add <--
// midori 152137 add <--
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	m_lo_pdb = pdb;								// データベースポインタを保持
	m_lo_TableName = STR_TBL_NAME_171;			// テーブル名を記述
	m_lo_FormSeq = inFormSeq;					// フォームシーケンス番号を保持
												// (その他帳表の場合のみ FormSeq 必要
}


CString CdbUc171Sonota1::GetDefaultSQL()
{
	return _T("[dbo].[uc_171_sonota1]");
}

void CdbUc171Sonota1::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CdbUc171Sonota1)
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
	RFX_Int(pFX, _T("[FormSeq]"), m_FormSeq);
	RFX_Long(pFX, _T("[KnSeq]"), m_KnSeq);
	RFX_Long(pFX, _T("[KnOrder]"), m_KnOrder);
	RFX_Text(pFX, _T("[KnName]"), m_KnName);
	RFX_Long(pFX, _T("[AdSeq]"), m_AdSeq);
	RFX_Long(pFX, _T("[AdOrder]"), m_AdOrder);
	RFX_Text(pFX, _T("[AdName1]"), m_AdName1);
	RFX_Text(pFX, _T("[AdName2]"), m_AdName2);
	RFX_Text(pFX, _T("[AdAdd1]"), m_AdAdd1);
	RFX_Text(pFX, _T("[AdAdd2]"), m_AdAdd2);
	RFX_Text(pFX, _T("[Val]"), m_Val);
	RFX_Text(pFX, _T("[Teki]"), m_Teki);
	RFX_Byte(pFX, _T("[ShowKeiZero]"), m_ShowKeiZero);
// midori 152137 add -->
	RFX_Text(pFX, _T("[KnKana]"), m_KnKana);
// midori 152137 add <--
	RFX_Text(pFX, _T("[Invno]"), m_InvNo);	// インボイス登録番号追加対応_23/10/20 add
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CdbUc171Sonota1 診断

#ifdef _DEBUG
void CdbUc171Sonota1::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbUc171Sonota1::Dump(CDumpContext& dc) const
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
int CdbUc171Sonota1::CreateNewRecord()
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
							
	m_Seq = retVal;				// シーケンス番号を自動付番
	m_NumPage = 0;				// (0)  ページ番号
	m_NumRow = 0;				// (0)  行番号
	m_FgFunc = 0;				// (0)  空行
	m_FgShow = 0;				// (0)  表示
	m_KeiStr = _T("");			// ("") 計名称
	m_NumGroup = 0;				// (0)  グループ番号
	m_RenKcd = _T("");			// ("") 連動元情報(科目)
	m_RenEcd = 0;				// (0)  連動元情報(枝番)
	m_RenFgTemp = 0;			// (0)  連動テンポラリ情報

	m_FormSeq = m_lo_FormSeq;	// (0)  その他の様式番号
	m_KnSeq = 0;				// (0)  科目：シーケンス番号
	m_KnOrder = 0;				// (0)  科目：順序番号
	m_KnName = _T("");			// ("") 科目：名称
	m_AdSeq = 0;				// (0)  相手先：シーケンス番号
	m_AdOrder = 0;				// (0)  相手先：順序番号
	m_AdName1 = _T("");			// ("") 相手先：名称(1行目)
	m_AdName2 = _T("");			// ("") 相手先：名称(2行目)
	m_AdAdd1 = _T("");			// ("") 相手先：所在地(1行目)
	m_AdAdd2 = _T("");			// ("") 相手先：所在地(2行目)
	m_Val = _T("");				// ("") 期末現在高
	m_Teki = _T("");			// ("") 摘要
	m_ShowKeiZero = 0;			// (0)  ０円計表示フラグ
// midori 152137 add -->
	m_KnKana = _T("");			// ("") 科目(カナ)
// midori 152137 add <--
	m_InvNo = _T("");			// ("") 登録番号		インボイス登録番号追加対応_23/10/20 add

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
//int CdbUc171Sonota1::CreateNewRecord2(int pNumPage,int pNumRow)
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
int CdbUc171Sonota1::CreateNewRecord2(int pNumPage,int pNumRow,int pNullSw)
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
							
	m_Seq = retVal;				// シーケンス番号を自動付番
	m_NumPage = pNumPage;		// (0)  ページ番号
	m_NumRow = pNumRow;			// (0)  行番号
// 157153 del -->
	//m_FgFunc = 0;				// (0)  空行
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
	m_FgShow = 0;				// (0)  表示
	m_KeiStr = _T("");			// ("") 計名称
	m_NumGroup = 0;				// (0)  グループ番号
	m_RenKcd = _T("");			// ("") 連動元情報(科目)
	m_RenEcd = 0;				// (0)  連動元情報(枝番)
	m_RenFgTemp = 0;			// (0)  連動テンポラリ情報

	m_FormSeq = m_lo_FormSeq;	// (0)  その他の様式番号
	m_KnSeq = 0;				// (0)  科目：シーケンス番号
	m_KnOrder = 0;				// (0)  科目：順序番号
	m_KnName = _T("");			// ("") 科目：名称
	m_AdSeq = 0;				// (0)  相手先：シーケンス番号
	m_AdOrder = 0;				// (0)  相手先：順序番号
	m_AdName1 = _T("");			// ("") 相手先：名称(1行目)
	m_AdName2 = _T("");			// ("") 相手先：名称(2行目)
	m_AdAdd1 = _T("");			// ("") 相手先：所在地(1行目)
	m_AdAdd2 = _T("");			// ("") 相手先：所在地(2行目)
	m_Val = _T("");				// ("") 期末現在高
	m_Teki = _T("");			// ("") 摘要
	m_ShowKeiZero = 0;			// (0)  ０円計表示フラグ
	m_KnKana = _T("");			// ("") 科目(カナ)
	m_InvNo = _T("");			// ("") 登録番号		インボイス登録番号追加対応_23/10/20 add

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
//int CdbUc171Sonota1::UpdateYokukiZeroClear()
//{
//	CString	sql = GetSQLUpdateYokukiZeroClear("Val");			// 期末現在高
//
//	sql.Format(sql + " AND (FormSeq=%d) ", m_lo_FormSeq);
//	//CString		strCommand;
//	//CString		strTemp;
//
//	//// クリア対象項目（1項目）
//	//strCommand.Format("UPDATE %s SET ", m_lo_TableName);
//	//strCommand	+=	_T("Val = 0 ");							// 期末現在高
//
//	//// 抽出条件：空行はNULLなので更新しない
//	//strTemp.Format("WHERE (FgFunc <> %d)", ID_FGFUNC_NULL);
//	//strCommand += strTemp;
//
//	//// FormSeq対応(帳表 ⑰⑱その他１、２対応)
//	//if (m_lo_FormSeq > 0) {
//	//	strTemp.Format(" AND (FormSeq=%d)", m_lo_FormSeq);
//	//	strCommand += strTemp;
//	//}
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
int CdbUc171Sonota1::UpdateYokukiZeroClear( int pSw )
{
// midori 152388 add -->
	CString	cs=_T("");
// midori 152388 add <--
	CString	sql = GetSQLUpdateYokukiZeroClear("Val",pSw);			// 期末現在高
	sql.Format(sql + " AND (FormSeq=%d) ", m_lo_FormSeq);
// midori 152388 add -->
	if(pSw != 0)	{
		cs.Format(_T("UPDATE %s SET FgFunc=%d WHERE FgFunc<=%d AND FormSeq=%d AND"),m_lo_TableName,ID_FGFUNC_NULL,ID_FGFUNC_DATA,m_lo_FormSeq);
		cs += _T(" KnOrder=0");
		cs += _T(" AND AdName1 IS NULL AND AdName2 IS NULL");
		cs += _T(" AND AdAdd1 IS NULL AND AdAdd2 IS NULL");
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
int CdbUc171Sonota1::UpdateDataTableWork()
{
	int	retVal1 = UpdateDataTableNullToStrNull("AdName1");
	int	retVal2 = UpdateDataTableNullToStrNull("AdName2");

	if (retVal1 != DB_ERR_OK) {
		return	retVal1;
	}
	return retVal2;
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
CString CdbUc171Sonota1::GetSQLKeiInsert(int nPage, CMyStringArray* arryColumn, CString strKei/*=0*/, int nFgFunc/*=0*/, CMyArray* arryTotalAll/*=NULL*/)
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

			if(( arryTotalAll != NULL ) && ( arryTotalAll->GetAt( nPage - nRuikei ).GetAt(0).IsEmpty() == FALSE )){
				strSQL.Format( strSQL + _T("'%s'") , arryTotalAll->GetAt(nPage - nRuikei).GetAt(0) );
			}
			else{
				strSQL = strSQL + _T("NULL");
			}
		}
		else if( arryColumn->GetAt(i).Compare( _T("Risoku") ) == 0 ){	// 計として表示する2個目

			if(( arryTotalAll != NULL ) && ( arryTotalAll->GetAt( nPage - nRuikei ).GetAt( 1 ).IsEmpty() == FALSE )){
				strSQL.Format( strSQL + _T("'%s'") , arryTotalAll->GetAt( nPage - nRuikei ).GetAt( 1 ) );
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
		else if( arryColumn->GetAt(i).Compare( _T("FormSeq") ) == 0 ){	// その他専用のシーケンス（－１、－２のフィルタ）
			strTmp.Format("%d", m_lo_FormSeq);
			strSQL = strSQL + strTmp;
		}
		else{
			strSQL = strSQL + _T("NULL");
		}

		if( i == (int)arryColumn->GetCount() -1 ){
			break;
		}

		strSQL = strSQL + ",";
	}

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
int CdbUc171Sonota1::GetSortIkkatuOrder()
{
	return m_KnOrder;
}