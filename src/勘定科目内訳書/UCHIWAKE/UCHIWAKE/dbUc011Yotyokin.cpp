// dbUc011Yotyokin.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dbUc011Yotyokin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CdbUc011Yotyokin

IMPLEMENT_DYNAMIC(CdbUc011Yotyokin, CdbUc000Common)

CdbUc011Yotyokin::CdbUc011Yotyokin(CDatabase* pdb)
	: CdbUc000Common(pdb)
{
	//{{AFX_FIELD_INIT(CdbUc011Yotyokin)
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
	m_BkSeq = 0;
	m_BkOrder = 0;
	m_BkName1 = _T("");
	m_BkName2 = _T("");
	m_KnSeq = 0;
	m_KnOrder = 0;
	m_KnName = _T("");
	m_AcNum = _T("");
	m_Val = _T("");
	m_Teki = _T("");
	m_ShowKeiZero = 0;
// midori 152137 del -->
//	m_nFields = 21;
// midori 152137 del <--
// midori 152137 add -->
	m_BkKana1 = _T("");
	m_BkKana2 = _T("");
	m_KnKana = _T("");
	m_nFields = 24;
// midori 152137 add <--
	
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	m_lo_pdb = pdb;								// データベースポインタを保持
	m_lo_TableName = STR_TBL_NAME_011;			// テーブル名を記述
}


CString CdbUc011Yotyokin::GetDefaultSQL()
{
	return _T("[dbo].[uc_011_yotyokin]");
}

void CdbUc011Yotyokin::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CdbUc011Yotyokin)
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
	RFX_Long(pFX, _T("[BkSeq]"), m_BkSeq);
	RFX_Long(pFX, _T("[BkOrder]"), m_BkOrder);
	RFX_Text(pFX, _T("[BkName1]"), m_BkName1);
	RFX_Text(pFX, _T("[BkName2]"), m_BkName2);
	RFX_Long(pFX, _T("[KnSeq]"), m_KnSeq);
	RFX_Long(pFX, _T("[KnOrder]"), m_KnOrder);
	RFX_Text(pFX, _T("[KnName]"), m_KnName);
	RFX_Text(pFX, _T("[AcNum]"), m_AcNum);
	RFX_Text(pFX, _T("[Val]"), m_Val);
	RFX_Text(pFX, _T("[Teki]"), m_Teki);	
	RFX_Byte(pFX, _T("[ShowKeiZero]"), m_ShowKeiZero);
// midori 152137 add -->
	RFX_Text(pFX, _T("[BkKana1]"), m_BkKana1);	
	RFX_Text(pFX, _T("[BkKana2]"), m_BkKana2);	
	RFX_Text(pFX, _T("[KnKana]"), m_KnKana);	
// midori 152137 add <--
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CdbUc011Yotyokin 診断

#ifdef _DEBUG
void CdbUc011Yotyokin::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbUc011Yotyokin::Dump(CDumpContext& dc) const
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
int CdbUc011Yotyokin::CreateNewRecord()
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

	m_BkSeq = 0;			// (0)  金融機関：(金融)シーケンス番号
	m_BkOrder = 0;			// (0)  金融機関：(金融)順序番号
	m_BkName1 = _T("");		// ("") 金融機関：(金融)銀行名
	m_BkName2 = _T("");		// ("") 金融機関：(金融)支店名
	m_KnSeq = 0;			// (0)  種類：(科目)シーケンス番号
	m_KnOrder = 0;			// (0)  種類：(科目)順序番号
	m_KnName = _T("");		// ("") 種類：(科目)名称
	m_AcNum = _T("");		// ("") 口座番号
	m_Val = _T("");			// ("") 期末現在高（金額）
	m_Teki = _T("");		// ("") 摘要
	m_ShowKeiZero = 0;		// (0)  ０円計表示フラグ
// midori 152137 add -->
	m_BkKana1 = _T("");		// ("") 金融機関(銀行名)(カナ)
	m_BkKana2 = _T("");		// ("") 金融機関(支店名)(カナ)
	m_KnKana = _T("");		// ("") カナ(科目)
// midori 152137 add <--

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
//int CdbUc011Yotyokin::UpdateYokukiZeroClear()
//{
//	CString	sql = GetSQLUpdateYokukiZeroClear("Val");			// 期末現在高
//
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
int CdbUc011Yotyokin::UpdateYokukiZeroClear( int pSw )
{
// midori 152388 add -->
	CString	cs=_T("");
// midori 152388 add <--
	CString	sql = GetSQLUpdateYokukiZeroClear("Val",pSw);			// 期末現在高
// midori 152388 add -->
	if(pSw != 0)	{
		cs.Format(_T("UPDATE %s SET FgFunc=%d WHERE FgFunc<=%d AND "),m_lo_TableName,ID_FGFUNC_NULL,ID_FGFUNC_DATA);
		cs += _T(" BkName1 IS NULL AND BkName2 IS NULL AND KnOrder=0 AND AcNum IS NULL");
		cs += _T(" AND Val IS NULL AND Teki IS NULL");
		sql += cs;
	}
// midori 152388 add <--
	return	ExecuteSQLWork(sql);
}
// midori 160606 cor <--

/**********************************************************************
		銀行名でソートするときのBkOrderの値を返す
***********************************************************************/
long CdbUc011Yotyokin::GetColumnSortAddressBank(BYTE ItemSeq)
{
	if(ItemSeq == 1)	return this->m_BkOrder;
	else				return -1;
}

/**********************************************************************
		ソート前にデータを整えるために実行される。

	BkName1		NULLになっているものを NULL文字列('')に更新
	BkName2		NULLになっているものを NULL文字列('')に更新


	引数
			なし
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUc011Yotyokin::UpdateDataTableWork()
{
	int	retVal1 = UpdateDataTableNullToStrNull("BkName1");
	int	retVal2 = UpdateDataTableNullToStrNull("BkName2");

	if (retVal1 != DB_ERR_OK) {
		return	retVal1;
	}
	return retVal2;
}

//********************************************************************************
//	ソートキーのタイプがintと異なる場合、そのタイプを返す。
//		IN		なし
//		RET		預貯金の内訳書の第一ソート項目が「金融機関名」のときはvarchar(400)（[OrderStr]のサイズ300+余分100）
//********************************************************************************
CString CdbUc011Yotyokin::GetSortItemType()
{
	CString		strSortItem = "int";
	CdbUcInfSub	mfcInfSub( m_lo_pdb );

	try{
		mfcInfSub.RequeryFormSeq( ID_FORMNO_011 );

		if( !mfcInfSub.IsEOF() ){
			if( mfcInfSub.m_Sort1 == 1 ){	// 1 = 「金融機関名」
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
//int CdbUc011Yotyokin::CreateNewRecord2(int pNumPage,int pNumRow)
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
int CdbUc011Yotyokin::CreateNewRecord2(int pNumPage,int pNumRow,int pNullSw)
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
	m_NumPage = pNumPage;			// (0)  ページ番号
	m_NumRow = pNumRow;			// (0)  行番号
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

	m_BkSeq = 0;			// (0)  金融機関：(金融)シーケンス番号
	m_BkOrder = 0;			// (0)  金融機関：(金融)順序番号
	m_BkName1 = _T("");		// ("") 金融機関：(金融)銀行名
	m_BkName2 = _T("");		// ("") 金融機関：(金融)支店名
	m_KnSeq = 0;			// (0)  種類：(科目)シーケンス番号
	m_KnOrder = 0;			// (0)  種類：(科目)順序番号
	m_KnName = _T("");		// ("") 種類：(科目)名称
	m_AcNum = _T("");		// ("") 口座番号
	m_Val = _T("");			// ("") 期末現在高（金額）
	m_Teki = _T("");		// ("") 摘要
	m_ShowKeiZero = 0;		// (0)  ０円計表示フラグ
	m_BkKana1 = _T("");		// ("") 金融機関(銀行名)(カナ)
	m_BkKana2 = _T("");		// ("") 金融機関(支店名)(カナ)
	m_KnKana = _T("");		// ("") カナ(科目)

	Update();

	return(0);
}
// midori 156968 add <--
