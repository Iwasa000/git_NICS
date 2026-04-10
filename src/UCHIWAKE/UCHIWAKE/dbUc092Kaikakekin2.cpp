// dbUc092Kaikakekin2.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
//#include "uchiwake.h"
#include "dbUc092Kaikakekin2.h"
// midori 155525_2 add -->
#include "ConvOutRangai.h"
// midori 155525_2 add <--

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CdbUc092Kaikakekin2

IMPLEMENT_DYNAMIC(CdbUc092Kaikakekin2, CdbUc000Common)

CdbUc092Kaikakekin2::CdbUc092Kaikakekin2(CDatabase* pdb)
	: CdbUc000Common(pdb)
{
	//{{AFX_FIELD_INIT(CdbUc092Kaikakekin2)
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
	m_Val = _T("");
	m_ShowKeiZero = 0;
	m_OutRangai = 0;
	m_nFields = 14;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	m_lo_pdb = pdb;								// データベースポインタを保持
	m_lo_TableName = STR_TBL_NAME_092;			// テーブル名を記述
}


CString CdbUc092Kaikakekin2::GetDefaultSQL()
{
	return _T("[dbo].[uc_092_kaikakekin2]");
}

void CdbUc092Kaikakekin2::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CdbUc092Kaikakekin2)
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
	RFX_Text(pFX, _T("[Val]"), m_Val);
	RFX_Byte(pFX, _T("[ShowKeiZero]"), m_ShowKeiZero);
	RFX_Byte(pFX, _T("[OutRangai]"), m_OutRangai);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CdbUc092Kaikakekin2 診断

#ifdef _DEBUG
void CdbUc092Kaikakekin2::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbUc092Kaikakekin2::Dump(CDumpContext& dc) const
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
int CdbUc092Kaikakekin2::CreateNewRecord()
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

	m_SpDate = 0;			// (0)  支払確定年月日
	m_Val = _T("");			// ("") 期末現在高
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
int CdbUc092Kaikakekin2::Init()
{
	int	nRet = DB_ERR_OK;	//	戻値
	
	m_strFilter = "";
	m_strSort = "";
	
	//	リクエリ実行
	nRet = RequeryWork();
	
	//	成功？
	if ( nRet == DB_ERR_OK ){
		
		//	レコードなし？
		if ( IsEOF() )	{

			//	強制的に４レコード追加
			for( int i = 1; i <= 4; i++ ){

				AddNew();							//	追加
				m_Seq = i;							//	シーケンス番号（1～4固定）
				m_NumPage = 1;						//	頁番号（1固定）
				m_NumRow = i;						//	行番号（1～4固定）
				m_FgFunc = 1;						//	特殊行フラグ（1固定）
				m_FgShow = 0;						//	表示フラグ（0固定）
				m_KeiStr = _T("");					//	「計」文言文字列格納用
				m_NumGroup = 0;						//  グループ番号
				m_SpDate = 0;						//  支払確定年月日
				m_Val = _T("");						//  期末現在高
				m_OutRangai = 0;					//　欄外出力タイプ
				Update();							//	更新
				Requery();							//	リクエリ
			}
		}		
	}
	
	//	戻値を返す
	return( nRet );
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
int CdbUc092Kaikakekin2::Init(int page)
{
	int	nRet = DB_ERR_OK;	//	戻値
	
	m_strFilter.Format("NumPage = %d", page);
	m_strSort = "NumRow ASC";
	
	//	リクエリ実行
	nRet = RequeryWork();
	
	//	成功？
	if ( nRet == DB_ERR_OK ){
		
		//	レコードなし？
		if ( IsEOF() )	{
			//	強制的に４レコード追加
			int max =  4 + ((page - 1) * 4);
			int row = 1;
			for( int i = 1 + ((page - 1) * 4); i <= max; i++, row++ ){	// 2ページ目以降対応

				AddNew();							//	追加
				m_Seq = i;							//	シーケンス番号（1～4固定）
				m_NumPage = page;					//	頁番号（1固定→F9頁追加、削除により増減　ただし、電子申告でページ数を1～見ている為、必ず１から１ずつインクリメントすること）
				m_NumRow = row;						//	行番号（1～4固定）
				m_FgFunc = 1;						//	特殊行フラグ（1固定）
				m_FgShow = 0;						//	表示フラグ（0固定）
				m_KeiStr = _T("");					//	「計」文言文字列格納用
				m_NumGroup = 0;						//  グループ番号
				m_SpDate = 0;						//  支払確定年月日
				m_Val = _T("");						//  期末現在高
				m_OutRangai = 0;					//　欄外出力タイプ
				Update();							//	更新
			}
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
//int CdbUc092Kaikakekin2::UpdateYokukiZeroClear()
//{
//	CString	sql = GetSQLUpdateYokukiZeroClear("SpDate");		// 印字しないフラグ
//	sql += GetSQLUpdateYokukiZeroClear("Val");					// 期末棚卸の方法（4:初期値）
//	sql.Format("%s UPDATE %s SET FgShow = %d, OutRangai = 0 WHERE FgFunc <> %d AND Seq = 1", sql, m_lo_TableName, ID_FGSHOW_KAIKAKEKIN, ID_FGFUNC_NULL);
//	//CString		strCommand;
//	//CString		strTemp;
//
//	//// クリア対象項目（2項目）
//	//strCommand.Format("UPDATE %s SET ", m_lo_TableName);
//	//strCommand	+=	_T("SpDate = 0, ");						// 支払確定年月日
//	//strCommand	+=	_T("Val = 0, ");						// 期末現在高
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
int CdbUc092Kaikakekin2::UpdateYokukiZeroClear( int pSw )
{
	CString	sql = GetSQLUpdateYokukiZeroClear("SpDate",0);		// 印字しないフラグ
	sql += GetSQLUpdateYokukiZeroClear("Val",pSw);				// 期末棚卸の方法（4:初期値）
	sql.Format("%s UPDATE %s SET FgShow = %d, OutRangai = 0 WHERE FgFunc <> %d AND Seq = 1", sql, m_lo_TableName, ID_FGSHOW_KAIKAKEKIN, ID_FGFUNC_NULL);
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
int CdbUc092Kaikakekin2::UpdateYokukiMoveToHokan()
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
int CdbUc092Kaikakekin2::UpdateZeroMoneyMoveToHokan( HOKAN_DATA *hDat )
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
int CdbUc092Kaikakekin2::GetRangaiData()
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
CString CdbUc092Kaikakekin2::GetRangaiTblName()
{
	return STR_TBL_NAME_092_TMP;
}

/**********************************************************************
	GetCountFgShow()
		引数で指定されたレコード数を返す　FgShow版
		買掛金の内訳書欄外登録は1pにつき4行ある為、4で割る
	引数
			int					m_FgShowの属性
	戻値
			int					データ件数
***********************************************************************/
int CdbUc092Kaikakekin2::GetCountFgShow(int FgShow)
{
// 155525_2 del -->
	//int cnt = CdbUc000Common::GetCountFgShow(FgShow);
	//if(cnt == -1)	return 0;
// 155525_2 del <--
// 155525_2 add -->
	int		cnt=0;

	// 出力形式(先頭ページ OR 最終ページ OR 全ページ、ページ共通出力 OR ページ個別出力)を取得する
	int				sw=0;
	int				OutZero=0;
	int				PrintType=0,OutType=0;
	CdbUcInfSub		mfcSub(m_lo_pdb);
	CConvOutRangai	cr;
// 155525_3 add -->
	CRecordset		rs(m_lo_pdb);
	CString			strQuery=_T("");
	CString			dt=_T("");
// 155525_3 add <--

	try	{

		mfcSub.RequeryFormSeq(ID_FORMNO_091);
		CCtrlSubGeneralVar cs(mfcSub);
		if(cs.bPrintZeroData)	OutZero |= BIT_D0;
		if(cs.bPrintNullData)	OutZero |= BIT_D1;
		mfcSub.Fin();

		RequeryPage(1);
		if(IsEOF())	{
			Fin();
			return(0);
		}
		cr.ToMemory(m_OutRangai,OutType,PrintType);
		// 全ページかつページ個別出力なら
		if(OutType == 2 && PrintType != 0)	{
			// ０円出力・空欄出力のいずれかにチェックが付いていれば
			if((OutZero & BIT_D0) || (OutZero & BIT_D1))	{
// 155525_3 del -->
				//sw = 1;
// 155525_3 del <--
// 155525_3 add -->
				// 出力用テンポラリテーブルが作成されていれば
				strQuery.Format(_T("select object_id('tempdb..%s'),'u'"),STR_TBL_NAME_092_TMP);
				rs.Open(CRecordset::forwardOnly,strQuery);
				rs.GetFieldValue((short)0,dt);
				rs.Close();
				if(dt.IsEmpty() == FALSE)	{
					sw = 1;
				}
// 155525_3 add <--
			}
		}
		Fin();
	}
	catch(CDBException *e)	{
		ICSMessageBox(e->m_strError,MB_ICONSTOP);
		e->Delete();
		return(0);
	}
	if(sw == 0)	{
		cnt = CdbUc000Common::GetCountFgShow(FgShow);
	}
	else	{
		cnt = GetCountFgShowEx(FgShow);
	}
	if(cnt == -1)	return 0;
// 155525_2 add <--

	return cnt / 4;
}

// 155525_2 add -->
int CdbUc092Kaikakekin2::GetCountFgShowEx(int FgShow)
{
	int retVal = 0;
	CString strSQL, strVal;
	CRecordset	rs(m_lo_pdb);

	strSQL.Format("SELECT COUNT(*) AS cnt FROM %s ", STR_TBL_NAME_092_TMP);

	CString	SqlStrSeq;
	SqlStrSeq.Format("WHERE FgShow = %d ", FgShow);
	strSQL += SqlStrSeq;

	try{
		if ( ! OpenEx( &rs , strSQL , STR_TBL_NAME_092_TMP ) ) {
			return DB_ERR_OPEN;
		}

		if (!rs.IsEOF()) {
			rs.GetFieldValue("cnt", strVal);
			sscanf_s( strVal, _T("%d"), &retVal);
		}
		else{
			return 0;
		}
	}
	catch(...){
		return 0;
	}

	return retVal;
}
// 155525_2 add <--

// midori 161107 add -->
/**********************************************************************
	RequeryPageRangai()
		欄外に有効なデータがあるかを調べる
	引数
			int					0:ページ共通設定	1:ページ個別設定
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
// midori 155525 del -->
//int CdbUc092Kaikakekin2::RequeryPageRangai( int pType )
// midori 155525 del <--
// midori 155525 add -->
int CdbUc092Kaikakekin2::RequeryPageRangai( int pType, int pOutZero )
// midori 155525 add <--
{
// midori 155525 del -->
	//m_strFilter.Format("FgShow = 0 AND (SpDate > 0 OR Val IS NOT NULL) ");
// midori 155525 del <--
// midori 155525 add -->
	if((pOutZero & BIT_D0) && (pOutZero & BIT_D1))	{
		m_strFilter.Format("FgShow = 0 AND (Val != 0 AND Val IS NOT NULL) ");
	}
	else if(pOutZero & BIT_D0)	{
		//m_strFilter.Format("FgShow = 0 AND (Val != 0) ");
		m_strFilter.Format("FgShow = 0 AND (Val != 0 OR (SpDate > 0 AND Val IS NULL)) ");
	}
	else if(pOutZero & BIT_D1)	{
		m_strFilter.Format("FgShow = 0 AND (Val IS NOT NULL) ");
	}
	else	{
		m_strFilter.Format("FgShow = 0 AND (SpDate > 0 OR Val IS NOT NULL) ");
	}
// midori 155525 add <--
	if(pType == 0)	{
		m_strFilter += "AND (NumPage = 1)";
	}

	return RequeryWork();		// リクエリを実行
}
// midori 161107 add <--
