// dbUc091Kaikakekin.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
//#include "uchiwake.h"
#include "dbUc091Kaikakekin.h"
// midori 155525_2 add -->
#include "dbUc092Kaikakekin2.h"
#include "ConvOutRangai.h"
// midori 155525_2 add <--

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// インボイス登録番号追加対応_23/12/25 add -->
extern BOOL bG_InvNo;			// インボイス番号使用サイン（決算期末が令和6年3月1日を超えている）
// インボイス登録番号追加対応_23/12/25 add <--


/////////////////////////////////////////////////////////////////////////////
// CdbUc091Kaikakekin

IMPLEMENT_DYNAMIC(CdbUc091Kaikakekin, CdbUc000Common)

CdbUc091Kaikakekin::CdbUc091Kaikakekin(CDatabase* pdb)
	: CdbUc000Common(pdb)
{
	//{{AFX_FIELD_INIT(CdbUc091Kaikakekin)
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
//	m_nFields = 22;
// midori 152137 del <--
// midori 152137 add -->
	m_KnKana = _T("");
	m_AdKana = _T("");
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
	m_lo_TableName = STR_TBL_NAME_091;			// テーブル名を記述
	m_rangai_tmp_table = STR_TBL_NAME_092_TMP;
}


CString CdbUc091Kaikakekin::GetDefaultSQL()
{
	return _T("[dbo].[uc_091_kaikakekin]");
}

void CdbUc091Kaikakekin::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CdbUc091Kaikakekin)
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
	RFX_Text(pFX, _T("[AdKana]"), m_AdKana);
// midori 152137 add <--
	RFX_Text(pFX, _T("[Invno]"), m_InvNo);	// インボイス登録番号追加対応_23/10/20 add
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CdbUc091Kaikakekin 診断

#ifdef _DEBUG
void CdbUc091Kaikakekin::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbUc091Kaikakekin::Dump(CDumpContext& dc) const
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
int CdbUc091Kaikakekin::CreateNewRecord()
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

	m_KnSeq = 0;			// (0)  科目：シーケンス番号
	m_KnOrder = 0;			// (0)  科目：順序番号
	m_KnName = _T("");		// ("") 科目：名称
	m_AdSeq = 0;			// (0)  相手先：(取引)シーケンス番号
	m_AdOrder = 0;			// (0)  相手先：(取引)順序番号
	m_AdName1 = _T("");		// ("") 相手先：(取引)名称(1行目)
	m_AdName2 = _T("");		// ("") 相手先：(取引)名称(2行目)
	m_AdAdd1 = _T("");		// ("") 相手先：(取引)所在地(1行目)
	m_AdAdd2 = _T("");		// ("") 相手先：(取引)所在地(1行目)
	m_Val = _T("");			// ("") 期末現在高
	m_Teki = _T("");		// ("") 摘要
	m_ShowKeiZero = 0;		// (0)  ０円計表示フラグ
// midori 152137 add -->
	m_KnKana = _T("");		// ("") 科目(カナ)
	m_AdKana = _T("");		// ("") 相手先(カナ)
// midori 152137 add <--
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
//int CdbUc091Kaikakekin::CreateNewRecord2(int pNumPage,int pNumRow)
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
int CdbUc091Kaikakekin::CreateNewRecord2(int pNumPage,int pNumRow,int pNullSw)
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

	m_KnSeq = 0;			// (0)  科目：シーケンス番号
	m_KnOrder = 0;			// (0)  科目：順序番号
	m_KnName = _T("");		// ("") 科目：名称
	m_AdSeq = 0;			// (0)  相手先：(取引)シーケンス番号
	m_AdOrder = 0;			// (0)  相手先：(取引)順序番号
	m_AdName1 = _T("");		// ("") 相手先：(取引)名称(1行目)
	m_AdName2 = _T("");		// ("") 相手先：(取引)名称(2行目)
	m_AdAdd1 = _T("");		// ("") 相手先：(取引)所在地(1行目)
	m_AdAdd2 = _T("");		// ("") 相手先：(取引)所在地(1行目)
	m_Val = _T("");			// ("") 期末現在高
	m_Teki = _T("");		// ("") 摘要
	m_ShowKeiZero = 0;		// (0)  ０円計表示フラグ
	m_KnKana = _T("");		// ("") 科目(カナ)
	m_AdKana = _T("");		// ("") 相手先(カナ)
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
//int CdbUc091Kaikakekin::UpdateYokukiZeroClear()
//{
//	CString	sql = GetSQLUpdateYokukiZeroClear("Val");		// 期末現在高
//
//	//CString		strCommand;
//	//CString		strTemp;
//
//	//// クリア対象項目（1項目）
//	//strCommand.Format("UPDATE %s SET ", m_lo_TableName);
//	//strCommand	+=	_T("Val = 0 ");							// 期末現在高
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
int CdbUc091Kaikakekin::UpdateYokukiZeroClear( int pSw )
{
// midori 152388 add -->
	CString	cs=_T("");
// midori 152388 add <--
	CString	sql = GetSQLUpdateYokukiZeroClear("Val",pSw);		// 期末現在高
// midori 152388 add -->
	if(pSw != 0)	{
		cs.Format(_T("UPDATE %s SET FgFunc=%d WHERE FgFunc<=%d AND "),m_lo_TableName,ID_FGFUNC_NULL,ID_FGFUNC_DATA);
		cs += _T(" KnOrder=0 AND AdName1 IS NULL AND AdName2 IS NULL");
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
int CdbUc091Kaikakekin::GetKnNameFirstRow(int inPage, CString* pstrKnName, CString strTable)
{
	return GetKnNameFirstRowSub( inPage, pstrKnName, strTable );
}

/**********************************************************************
		銀行名でソートするときのBkOrderの値を返す
***********************************************************************/
long CdbUc091Kaikakekin::GetColumnSortAddressBank(BYTE ItemSeq)
{
	if(ItemSeq == 2)	return this->m_AdOrder;
	else				return -1;
}

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
int CdbUc091Kaikakekin::UpdateDataTableWork()
{
	int	retVal1 = UpdateDataTableNullToStrNull("AdName1");
	int	retVal2 = UpdateDataTableNullToStrNull("AdName2");

	if (retVal1 != DB_ERR_OK) {
		return	retVal1;
	}
	return retVal2;
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
int CdbUc091Kaikakekin::GetSortIkkatuOrder()
{
	return m_KnOrder;
}

// virtual
// midori 155525_2 del -->
//int CdbUc091Kaikakekin::CreateRangaiTempTable()
// midori 155525_2 del <--
// midori 155525_2 add -->
int CdbUc091Kaikakekin::CreateRangaiTempTable( int pOutZero )
// midori 155525_2 add <--
{
	// 一時テーブルを作成し、「印字する」レコードのみのテーブル作成（行番号つき）
	CString sql = "if object_id('tempdb.." + m_rangai_tmp_table + "', 'u') is not null drop table " + m_rangai_tmp_table + " \
				   SELECT DENSE_RANK()OVER(ORDER BY NumPage) as rank_, * \
				   INTO " + m_rangai_tmp_table + " \
				   FROM [dbo].[uc_092_kaikakekin2] \
				   WHERE FgShow = 0 ";
	ExecuteSQLWork(sql);
	// 「印字する」のみのレコードに対し、ページ数を振る
	sql =		  "DECLARE cur CURSOR for SELECT rank_, Seq FROM " + m_rangai_tmp_table + "  \
				   DECLARE @page int \
				   DECLARE @seq int \
				   OPEN cur FETCH cur INTO @page, @seq WHILE @@fetch_status = 0 \
				   BEGIN \
				   UPDATE " + m_rangai_tmp_table + " SET NumPage = @page WHERE Seq = @seq \
				   FETCH cur INTO @page, @seq \
				   END CLOSE cur DEALLOCATE cur ";
	ExecuteSQLWork(sql);

	// rowカラムを削除
	sql =		  "ALTER TABLE " + m_rangai_tmp_table + " DROP COLUMN rank_";

// midori 155525_2 del -->
	//return ExecuteSQLWork(sql);
// midori 155525_2 del <--
// midori 155525_2 add -->
	ExecuteSQLWork(sql);

	// 出力形式(先頭ページ OR 最終ページ OR 全ページ、ページ共通出力 OR ページ個別出力)を取得する
	CdbUc092Kaikakekin2	rc(m_lo_pdb);
	int					sw=0;
	try	{
		rc.RequeryPage(1);
		if(rc.IsEOF())	{
			rc.Fin();
			return(DB_ERR_OK);
		}
		CConvOutRangai	cr;
		int				PrintType=0,OutType=0;
		cr.ToMemory(rc.m_OutRangai,OutType,PrintType);
		// 全ページかつページ個別出力なら
		if(OutType == 2 && PrintType != 0)	{
			// ０円出力・空欄出力のいずれかにチェックが付いていれば
			if((pOutZero & BIT_D0) || (pOutZero & BIT_D1))	{
				sw = 1;
			}
		}
		rc.Fin();
	}
	catch(CDBException *e)	{
		ICSMessageBox(e->m_strError,MB_ICONSTOP);
		e->Delete();
		return(DB_ERR_EXESQL);
	}
	if(sw != 0)	{
		// CimpUc092Kaikakekin2::SetCsvNullLineData091と同じ処理を行う関数を呼び出す
		// (テンポラリテーブルに対して処理を行っているところのみ異なります)
		if(SetNullLineData091(pOutZero) != DB_ERR_OK)	{
			return(DB_ERR_EXESQL);
		}
	}

	return(DB_ERR_OK);
// midori 155525_2 add <--
}

// midori 155525_2 add -->
int CdbUc091Kaikakekin::SetNullLineData091( int pOutZero )
{
	int			count=0;			// カウント
	int			maxp=0;				// 最大頁
	int			curp=0;				// 現在頁
	int			curl=0;				// 現在行
	int			hno=0;				// 「未払配当金」のカレントレコードＮｏ
	int			yno=0;				// 「未払役員賞与」のカレントレコードＮｏ
	int			rv=0;				// 返送値
	int			seq=0;
	int			fgfunc=0;
	int			spdate=0;
	short		ind=0;				// インデックス
	CString		strQuery=_T("");	// SQL文
	CString		strData[4];			// データ用バッファ
	CString		val=_T("");
	CRecordset			rs(m_lo_pdb);
	typedef struct	_RANGAI_DATA_	{
		BYTE	m_FgFunc;
		long	m_SpDate;
		CString	m_Val;
	}RANGAI_DATA;
	CMap<int,int,RANGAI_DATA,RANGAI_DATA>	m_haitou;
	CMap<int,int,RANGAI_DATA,RANGAI_DATA>	m_yakuin;
	RANGAI_DATA								rdata;

	// 「未払配当金」と「未払役員賞与」に分ける
	m_haitou.RemoveAll();
	m_yakuin.RemoveAll();
	try	{
		//strQuery.Format(_T("select FgFunc,RenFgTemp,SpDate,Val from %s"),m_rangai_tmp_table);
		strQuery.Format(_T("select FgFunc,NumRow,SpDate,Val from %s"),m_rangai_tmp_table);
		// ０円出力・空欄出力ともにチェックが付いている
		if((pOutZero & BIT_D0) && (pOutZero & BIT_D1))	{
			strQuery = strQuery + _T(" where Val != 0 AND Val IS NOT NULL");
		}
		// ０円出力にのみチェックが付いている
		else if(pOutZero & BIT_D0)	{
			strQuery = strQuery + _T(" where Val != 0 OR (SpDate > 0 AND Val IS NULL)");
		}
		else if(pOutZero & BIT_D1)	{
			strQuery = strQuery + _T(" where Val IS NOT NULL");
		}
		strQuery = strQuery + _T(" order by NumPage,NumRow");
		if(!rs.Open(CRecordset::forwardOnly,strQuery))	{
			rs.Close();
			return(DB_ERR_EXESQL);
		}
		while(!rs.IsEOF())	{
			for(ind=0; ind<4; ind++)	{
				rs.GetFieldValue(ind,strData[ind]);
			}
			rs.MoveNext();
			rdata.m_FgFunc = (BYTE)_tstoi(strData[0]);
			rdata.m_SpDate = (long)_tstoi(strData[2]);
			rdata.m_Val = strData[3];
			if((_tstoi(strData[1]) % 4) != 0 && (_tstoi(strData[1]) % 4) != 3)	{
				m_haitou.SetAt((int)m_haitou.GetCount(),rdata);
			}
			else	{
				m_yakuin.SetAt((int)m_yakuin.GetCount(),rdata);
			}
		}
		rs.Close();
		// 出力対象のデータをすべてマップに書き込んだので、テンポラリｄｂからデータを削除する
		strQuery.Format(_T("delete from %s"),m_rangai_tmp_table);
		m_lo_pdb->ExecuteSQL(strQuery);
	}
	catch(CDBException *e)	{
		ICSMessageBox(e->m_strError,MB_ICONSTOP);
		e->Delete();
		return(DB_ERR_EXESQL);
	}

	// データが無ければここで終了
	if((int)(m_haitou.GetCount() + m_yakuin.GetCount()) == 0)	{
		return(DB_ERR_OK);
	}

	// 最大頁を割り出す
	if(m_haitou.GetCount() >= m_yakuin.GetCount())	{
		count = (int)m_haitou.GetCount();
	}
	else	{
		count = (int)m_yakuin.GetCount();
	}
	maxp = count / 2;
	if(count % 2)	maxp++;

	// 「未払配当金」と「未払役員賞与」を交互にレコードに追加していく
	hno = 0;
	yno = 0;
	seq = 1;
	for(curp=0; curp<maxp; curp++)	{
		for(curl=0; curl<4; curl++)	{
			if(curl == 0 || curl == 1)	{
				// 「未払配当金」
				rv = m_haitou.Lookup(hno,rdata);
				hno++;
			}
			else	{
				// 「未払役員賞与」
				rv = m_yakuin.Lookup(yno,rdata);
				yno++;
			}
			//m_pUcData->CreateNewRecord();
			//m_pUcData->Edit();
			//m_pUcData->m_NumPage	= curp + 1;
			//m_pUcData->m_NumRow		= curl + 1;
			if(rv == TRUE)	{
				fgfunc	= rdata.m_FgFunc;
				spdate	= rdata.m_SpDate;
				val		= rdata.m_Val;
			}
			else	{
				fgfunc	= 0;
				spdate	= 0;
				val		= _T("");
			}
			//m_pUcData->m_FgShow		= 0;
			//m_pUcData->m_NumGroup	= 0;
			//m_pUcData->m_OutRangai	= 0;
			//m_pUcData->Update();
			if(val.IsEmpty() == TRUE)	{
				strQuery.Format(_T("insert into %s values(%d,%d,%d,%d,%d,NULL,%d,NULL,%d,%d,%d,NULL,%d,%d)"),
					m_rangai_tmp_table,
					seq,curp+1,curl+1,fgfunc,0,0,0,0,spdate,0,0);
			}
			else	{
				strQuery.Format(_T("insert into %s values(%d,%d,%d,%d,%d,NULL,%d,NULL,%d,%d,%d,%s,%d,%d)"),
					m_rangai_tmp_table,
					seq,curp+1,curl+1,fgfunc,0,0,0,0,spdate,val,0,0);
			}
			try	{
				m_lo_pdb->ExecuteSQL(strQuery);
			}
			catch(CDBException *e)	{
				ICSMessageBox(e->m_strError,MB_ICONSTOP);
				e->Delete();
				return(DB_ERR_EXESQL);
			}
		}
	}

	return(DB_ERR_OK);
}
// midori 155525_2 add <--
