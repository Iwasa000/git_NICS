// dbUc071Koteishisan.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
//#include "UCHIWAKE.h"
#include "dbUc071Koteishisan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// インボイス登録番号追加対応_23/12/25 add -->
extern BOOL bG_InvNo;			// インボイス番号使用サイン（決算期末が令和6年3月1日を超えている）
// インボイス登録番号追加対応_23/12/25 add <--

/////////////////////////////////////////////////////////////////////////////
// CdbUc071Koteishisan

IMPLEMENT_DYNAMIC(CdbUc071Koteishisan, CdbUc000Common)

CdbUc071Koteishisan::CdbUc071Koteishisan(CDatabase* pdb)
	: CdbUc000Common(pdb)
{
	//{{AFX_FIELD_INIT(CdbUc071Koteishisan)
	m_Syurui = _T("");
	m_Youto = _T("");
	m_Area = _T("");
	m_Val = _T("");
	m_SpDate = 0;
	m_Riyuu = _T("");
	m_MsVal = _T("");
	m_MsVal2 = _T("");
	m_AdSeq = 0;
	m_AdOrder = 0;
	m_AdName1 = _T("");
	m_AdName2 = _T("");
	m_AdAdd1 = _T("");
	m_AdAdd2 = _T("");
	m_SpDate2 = 0;
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
	m_Ad2Add1 = _T("");
	m_Ad2Add2 = _T("");
	m_ShowKeiZero = 0;
// インボイス登録番号追加対応_23/10/23 del -->
	//m_nFields = 28;
// インボイス登録番号追加対応_23/10/23 del <--
// インボイス登録番号追加対応_23/10/23 add -->
	m_InvNo = _T("");
	m_nFields = 29;
// インボイス登録番号追加対応_23/10/23 add <--
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	m_lo_pdb = pdb;								// データベースポインタを保持
	m_lo_TableName = STR_TBL_NAME_071;			// テーブル名を記述
}


CString CdbUc071Koteishisan::GetDefaultSQL()
{
	return _T("[dbo].[uc_071_koteishisan]");
}

void CdbUc071Koteishisan::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CdbUc071Koteishisan)
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
	RFX_Text(pFX, _T("[Syurui]"), m_Syurui);
	RFX_Text(pFX, _T("[Youto]"), m_Youto);
	RFX_Text(pFX, _T("[Area]"), m_Area);
	RFX_Text(pFX, _T("[Ad2Add1]"), m_Ad2Add1);
	RFX_Text(pFX, _T("[Ad2Add2]"), m_Ad2Add2);
	RFX_Text(pFX, _T("[Val]"), m_Val);
	RFX_Long(pFX, _T("[SpDate]"), m_SpDate);
	RFX_Text(pFX, _T("[Riyuu]"), m_Riyuu);
	RFX_Text(pFX, _T("[MsVal]"), m_MsVal);
	RFX_Text(pFX, _T("[MsVal2]"), m_MsVal2);
	RFX_Long(pFX, _T("[AdSeq]"), m_AdSeq);
	RFX_Long(pFX, _T("[AdOrder]"), m_AdOrder);
	RFX_Text(pFX, _T("[AdName1]"), m_AdName1);
	RFX_Text(pFX, _T("[AdName2]"), m_AdName2);
	RFX_Text(pFX, _T("[AdAdd1]"), m_AdAdd1);
	RFX_Text(pFX, _T("[AdAdd2]"), m_AdAdd2);
	RFX_Long(pFX, _T("[SpDate2]"), m_SpDate2);
	RFX_Byte(pFX, _T("[ShowKeiZero]"), m_ShowKeiZero);
	RFX_Text(pFX, _T("[Invno]"), m_InvNo);	// インボイス登録番号追加対応_23/10/20 add
	//}}AFX_FIELD_MAP

}

/////////////////////////////////////////////////////////////////////////////
// CdbUc071Koteishisan 診断

#ifdef _DEBUG
void CdbUc071Koteishisan::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbUc071Koteishisan::Dump(CDumpContext& dc) const
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
int CdbUc071Koteishisan::CreateNewRecord()
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

	m_Syurui = _T("");		// ("")	物件：種類・構造
	m_Youto = _T("");		// ("") 物件：用途
	m_Area = _T("");		// ("") 物件：面積
//	m_Address = _T("");		// ("") 物件：物件の所在地
	m_Ad2Add1 = _T("");		// ("") 物件：物件の所在地1
	m_Ad2Add2 = _T("");		// ("") 物件：物件の所在地2
	m_Val = _T("");			// ("") 期末現在高
	m_SpDate = 0;			// (0)  明細：異動年月日
	m_Riyuu = _T("");		// ("") 明細：異動事由
	m_MsVal = _T("");		// ("") 明細：取得価額
	m_MsVal2 = _T("");		// ("") 明細：異動直前の帳簿価額
	m_AdSeq = 0;			// (0)  明細：売却先(取引先 シーケンス番号)
	m_AdOrder = 0;			// (0)  明細：売却先(取引先 順序番号)
	m_AdName1 = _T("");		// ("") 明細：売却先 名称1
	m_AdName2 = _T("");		// ("") 明細：売却先 名称2
	m_AdAdd1 = _T("");		// ("") 明細：売却先 所在地1
	m_AdAdd2 = _T("");		// ("") 明細：売却先 所在地2
	m_SpDate2 = 0;			// (0)  明細：売却物件の取得年月
	m_ShowKeiZero = 0;		// (0)  ０円計表示フラグ
	m_InvNo = _T("");		// ("") 登録番号		インボイス登録番号追加対応_23/10/20 add

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
//int CdbUc071Koteishisan::CreateNewRecord2(int pNumPage,int pNumRow)
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
int CdbUc071Koteishisan::CreateNewRecord2(int pNumPage,int pNumRow,int pNullSw)
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

	m_Syurui = _T("");		// ("")	物件：種類・構造
	m_Youto = _T("");		// ("") 物件：用途
	m_Area = _T("");		// ("") 物件：面積
	m_Ad2Add1 = _T("");		// ("") 物件：物件の所在地1
	m_Ad2Add2 = _T("");		// ("") 物件：物件の所在地2
	m_Val = _T("");			// ("") 期末現在高
	m_SpDate = 0;			// (0)  明細：異動年月日
	m_Riyuu = _T("");		// ("") 明細：異動事由
	m_MsVal = _T("");		// ("") 明細：取得価額
	m_MsVal2 = _T("");		// ("") 明細：異動直前の帳簿価額
	m_AdSeq = 0;			// (0)  明細：売却先(取引先 シーケンス番号)
	m_AdOrder = 0;			// (0)  明細：売却先(取引先 順序番号)
	m_AdName1 = _T("");		// ("") 明細：売却先 名称1
	m_AdName2 = _T("");		// ("") 明細：売却先 名称2
	m_AdAdd1 = _T("");		// ("") 明細：売却先 所在地1
	m_AdAdd2 = _T("");		// ("") 明細：売却先 所在地2
	m_SpDate2 = 0;			// (0)  明細：売却物件の取得年月
	m_ShowKeiZero = 0;		// (0)  ０円計表示フラグ
	m_InvNo = _T("");		// ("") 登録番号		インボイス登録番号追加対応_23/10/20 add

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
//int CdbUc071Koteishisan::UpdateYokukiZeroClear()
//{
//	CString	sql = GetSQLUpdateYokukiZeroClear("Val");		// 期末現在高
//	sql += GetSQLUpdateYokukiZeroClear("SpDate");			// 異動年月日
//	sql += GetSQLUpdateYokukiZeroClear("MsVal2");			// 異動直前の帳簿価額
//	//CString		strCommand;
//	//CString		strTemp;
//
//	//// クリア対象項目（3項目）
//	//strCommand.Format("UPDATE %s SET ", m_lo_TableName);
//	//strCommand	+=	_T("Val = 0, ");						// 期末現在高
//	//strCommand	+=	_T("SpDate = 0, ");						// 異動年月日
//	//strCommand	+=	_T("MsVal2 = 0 ");						// 異動直前の帳簿価額
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
int CdbUc071Koteishisan::UpdateYokukiZeroClear( int pSw )
{
// midori 152388 add -->
	CString	cs=_T("");
// midori 152388 add <--
	CString	sql = GetSQLUpdateYokukiZeroClear("Val",pSw);		// 期末現在高
	sql += GetSQLUpdateYokukiZeroClear("SpDate",0);				// 異動年月日
	sql += GetSQLUpdateYokukiZeroClear("MsVal2",pSw);			// 異動直前の帳簿価額
// midori 152388 add -->
	if(pSw != 0)	{
		cs.Format(_T("UPDATE %s SET FgFunc=%d WHERE FgFunc<=%d AND "),m_lo_TableName,ID_FGFUNC_NULL,ID_FGFUNC_DATA);
		cs += _T(" Syurui IS NULL AND Youto IS NULL AND Area IS NULL AND Ad2Add1 IS NULL AND Ad2Add2 IS NULL");
		cs += _T(" AND Val IS NULL AND SpDate=0 AND Riyuu IS NULL");
		cs += _T(" AND MsVal IS NULL AND MsVal2 IS NULL AND AdName1 IS NULL AND AdName2 IS NULL");
		cs += _T(" AND AdAdd1 IS NULL AND AdAdd2 IS NULL AND SpDate2=0");
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

	Ad2Add1		NULLになっているものを NULL文字列('')に更新
	Ad2Add1		NULLになっているものを NULL文字列('')に更新


	引数
			なし
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
// virtual
int CdbUc071Koteishisan::UpdateDataTableWork()
{
	int	retVal1 = UpdateDataTableNullToStrNull("Ad2Add1");
	int	retVal2 = UpdateDataTableNullToStrNull("Ad2Add2");

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
CString CdbUc071Koteishisan::GetSQLSumColumn()
{
	CString strSQL;

	strSQL = _T("SUM(MsVal) AS 'Total2', SUM(MsVal2) AS 'Total3',");

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
int CdbUc071Koteishisan::GetColumnNum()
{
	return 3;
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
CString CdbUc071Koteishisan::GetSQLKeiInsert(int nPage, CMyStringArray* arryColumn, CString strKei/*=0*/, int nFgFunc/*=0*/, CMyArray* arryTotalAll/*=NULL*/)
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
		else if( arryColumn->GetAt(i).Compare( _T("MsVal") ) == 0 ){	// 計として表示する2個目

			if(( arryTotalAll != NULL ) && ( arryTotalAll->GetAt( nPage - nRuikei ).GetAt( 1 ).IsEmpty() == FALSE )){
				strSQL.Format( strSQL + _T("'%s'") , arryTotalAll->GetAt( nPage - nRuikei ).GetAt( 1 ) );
			}
			else{
				strSQL = strSQL + _T("NULL");
			}
		}
		else if( arryColumn->GetAt(i).Compare( _T("MsVal2") ) == 0 ){	// 計として表示する3個目

			if(( arryTotalAll != NULL ) && ( arryTotalAll->GetAt( nPage - nRuikei ).GetAt( 2 ).IsEmpty() == FALSE )){
				strSQL.Format( strSQL + _T("'%s'") , arryTotalAll->GetAt( nPage - nRuikei ).GetAt( 2 ) );
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
CString CdbUc071Koteishisan::GetSQLZeroJudgment()
{
	CString strSQL;

	//strSQL.Format("(%s <> 0 or MsVal <> 0 or MsVal2 <> 0)", GetZeroMoneyMoveField() );
	//strSQL.Format("((%s = 0 OR %s IS NULL) AND (MsVal = 0 OR MsVal IS NULL) AND (MsVal2 = 0 OR MsVal2 IS NULL)) ", GetZeroMoneyMoveField(), GetZeroMoneyMoveField() );
	if((m_OutZeroNull & BIT_D0) && (m_OutZeroNull & BIT_D1))
		strSQL.Format("((%s = 0 OR %s IS NULL) AND (MsVal = 0 OR MsVal IS NULL) AND (MsVal2 = 0 OR MsVal2 IS NULL)) ", GetZeroMoneyMoveField(), GetZeroMoneyMoveField() );
	else if(m_OutZeroNull & BIT_D0)
		strSQL.Format("(((%s = 0) AND (%s IS NOT NULL)) AND ((MsVal = 0) AND (MsVal IS NOT NULL)) AND ((MsVal2 = 0) AND (MsVal2 IS NOT NULL))) ", 
						GetZeroMoneyMoveField(), GetZeroMoneyMoveField() );
	else if(m_OutZeroNull & BIT_D1)
		strSQL.Format("((%s IS NULL) AND (MsVal IS NULL) AND (MsVal2 IS NULL)) ", GetZeroMoneyMoveField() );

	return strSQL;
}

//********************************************************************************
//	ソートキーのタイプがintと異なる場合、そのタイプを返す。
//		IN		なし
//		RET		固定資産の内訳書の第一ソート項目が「種類・構造」のときはvarchar(60)
//********************************************************************************
CString CdbUc071Koteishisan::GetSortItemType()
{
	CString		strSortItem = "int";
	CdbUcInfSub	mfcInfSub( m_lo_pdb );

	try{
		mfcInfSub.RequeryFormSeq( ID_FORMNO_071 );

		if( !mfcInfSub.IsEOF() ){
			if( mfcInfSub.m_Sort1 == 1 ){	// 1 = 「種類・構造」
				strSortItem = "varchar(60)";
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

//********************************************************************************
//	where Val <> 0 で条件付けても、ValがNULLは引っかからないので、Val is nullを付加する
//		【引数】	なし
//					
//		【戻値】	CString SQLのWHERE句の一部
//********************************************************************************
CString CdbUc071Koteishisan::GetSQLValIsNull()
{
	CString strSQL;

	strSQL.Format(_T("OR ((%s IS NULL) AND (MsVal IS NULL) AND (MsVal2 IS NULL)) AND FgFunc <> %d"), GetZeroMoneyMoveField(), ID_FGFUNC_NULL);

	return strSQL;
}