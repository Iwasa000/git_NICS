// dbUcLstKamoku.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
//#include "UCHIWAKE.h"
#include "dbUcLstKamoku.h"
#include "dbUcInfSub.h"			// 一括金額デフォルト取得の為

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CdbUcLstKamoku

IMPLEMENT_DYNAMIC(CdbUcLstKamoku, CdbBase)

CdbUcLstKamoku::CdbUcLstKamoku(CDatabase* pdb)
	: CdbBase(pdb)
	, m_SeqUserStartNum(ID_LST_USER_START)		//KnSeq(ユーザ利用番号開始値)
{
	//{{AFX_FIELD_INIT(CdbUcLstKamoku)
	m_FgDel = 0;
	m_FgDft = 0;
	m_FormSeq = 0;
	m_KnCode = _T("");
	m_KnName = _T("");
	m_KnOrder = 0;
	m_KnSeq = 0;
	m_KnKana = _T("");
	m_IkkatuVal = _T("");
// midori 160610 cor -->
//	m_nFields = 9;
// ---------------------
	m_PrSign = 0;
	m_nFields = 10;
// midori 160610 cor <--
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	m_lo_pdb = pdb;								// データベースポインタを保持
	m_lo_TableName = STR_TBL_NAME_LST_KAMOKU;	// テーブル名を記述
}


CString CdbUcLstKamoku::GetDefaultSQL()
{
	return _T("[dbo].[uc_lst_kamoku]");
}

void CdbUcLstKamoku::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CdbUcLstKamoku)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Byte(pFX, _T("[FgDel]"), m_FgDel);
	RFX_Byte(pFX, _T("[FgDft]"), m_FgDft);
	RFX_Int(pFX, _T("[FormSeq]"), m_FormSeq);
	RFX_Text(pFX, _T("[KnCode]"), m_KnCode);
	RFX_Text(pFX, _T("[KnName]"), m_KnName);
	RFX_Long(pFX, _T("[KnOrder]"), m_KnOrder);
	RFX_Long(pFX, _T("[KnSeq]"), m_KnSeq);
	RFX_Text(pFX, _T("[KnKana]"), m_KnKana);
	RFX_Text(pFX, _T("[IkkatuVal]"), m_IkkatuVal);
// midori 160610 add -->
	RFX_Byte(pFX, _T("[PrSign]"), m_PrSign);
// midori 160610 add <--
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CdbUcLstKamoku 診断

#ifdef _DEBUG
void CdbUcLstKamoku::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbUcLstKamoku::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

/**********************************************************************
	Init()
		初期化処理（Open()の代わりとして使用）

		指定された「様式シーケンス番号」に属する「項目」を抽出する

	引数
			int		FormSeq		様式シーケンス番号
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstKamoku::Init(int FormSeq)
{
	return	RequeryForm(FormSeq);

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
int CdbUcLstKamoku::Init()
{
	///////////////////////////////////////////////////////////////////
	// 目的のレコードを抽出する
	m_strFilter = "FgDel = 0";		// 削除フラグ OFF（0:有効なレコード)
	m_strSort   = "KnOrder";		// 並び順でソート

	return RequeryWork();		//成功
}

/**********************************************************************
	RequeryForm()

		指定された「様式シーケンス番号」に属する「科目」を抽出する
		並び順は、KnOrder の小さい値順

	引数
			int		FormSeq		様式シーケンス番号
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstKamoku::RequeryForm(int FormSeq)
{
	///////////////////////////////////////////////////////////////////
	// 目的のレコードを抽出する
								// 指定された様式 かつ 有効な行である で抽出
	m_strFilter.Format("(FormSeq = %d) AND (FgDel = 0)", FormSeq);
	m_strSort = "KnOrder";		// 項目番号 順

	return	RequeryWork();
}

// midori 161109 add -->
/**********************************************************************
	Init()
		初期化処理（Open()の代わりとして使用）

		指定された「様式シーケンス番号」に属する「項目」を抽出する

	引数
			int		FormSeq		様式シーケンス番号
			int		nSort		並び順
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstKamoku::Init_M(int FormSeq, int nSort)
{
	return	RequeryForm_M(FormSeq, nSort);

}

/**********************************************************************
	Init()
		初期化処理（Open()の代わりとして使用）

	引数
			int		nSort		表示順選択
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstKamoku::Init_M(int nSort)
{
	///////////////////////////////////////////////////////////////////
	// 目的のレコードを抽出する
	m_strFilter = "FgDel = 0";			// 削除フラグ OFF（0:有効なレコード)
	if(nSort == 0)	{
		m_strSort = "KnOrder";			// 並び順でソート
	}
	else	{
		m_strSort = "KnKana,KnOrder";	// カナ順でソート
	}

	return RequeryWork();		//成功
}

/**********************************************************************
	RequeryForm()

		指定された「様式シーケンス番号」に属する「科目」を抽出する
		並び順は、KnOrder の小さい値順

	引数
			int		FormSeq		様式シーケンス番号
			int		nSort		表示順選択
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstKamoku::RequeryForm_M(int FormSeq, int nSort)
{
	///////////////////////////////////////////////////////////////////
	// 目的のレコードを抽出する
								// 指定された様式 かつ 有効な行である で抽出
	m_strFilter.Format("(FormSeq = %d) AND (FgDel = 0)", FormSeq);
	if(nSort == 0)	{
		m_strSort = "KnOrder";				// 項目番号 順
	}
	else			{
		m_strSort = "KnKana,KnOrder";		// カナ順でソート
	}

	return	RequeryWork();
}
// midori 161109 add <--

/**********************************************************************
	CreateNewRecord()
		新規レコードを作成、初期値を追加
		レコードセットには、新規レコードがセットされて戻ります

	引数
			int		inFormSeq	様式シーケンス番号
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstKamoku::CreateNewRecord(int inFormSeq)
{
	CRecordset	rs(m_lo_pdb);
	CString		SqlStr;
//	CString		SqlStrWhe;
	int			retVal = DB_ERR;

	/////////////////////////////////////////////////////////////////////
	//	暫定バージョンです
	/////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////
	// 新しいシーケンス番号を算出
	SqlStr =		_T("SELECT max(KnSeq) AS MaxNum ");
	SqlStr +=		_T("FROM ") + m_lo_TableName + _T(" ");
//	SqlStrWhe.Format(  "WHERE FormSeq = %d ", inFormSeq);
//	SqlStr += SqlStrWhe;
	
	if ( ! OpenEx( &rs , SqlStr , m_lo_TableName ) ) {
		return DB_ERR_OPEN;
	};

	retVal = DB_ERR_EOF;
	if (!rs.IsEOF()) {
		CDBVariant	val;
		rs.GetFieldValue(_T("MaxNum"), val);
		if (val.m_dwType == DBVT_NULL) {	// NULLだった場合
			retVal = m_SeqUserStartNum;
		}
		else {								// 値の取得OK
//			retVal = val.m_iVal;
			switch (val.m_dwType) {
			case DBVT_UCHAR:
				retVal = val.m_chVal;		// char
				break;
			case DBVT_SHORT:
				retVal = val.m_iVal;		// short
				break;
			case DBVT_LONG:
				retVal = val.m_lVal;		// long
				break;
			default:
				retVal = val.m_lVal;		// default:long
				break;
			}
			//ユーザ使用開始値より小さい場合（ユーザ登録データが未登録の場合)
			if (retVal < m_SeqUserStartNum) {
				retVal = m_SeqUserStartNum;
			}
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
	//m_FormSeq = 0;
	m_FormSeq	= inFormSeq;	// フォームシーケンス番号
	m_KnSeq		= retVal;		// シーケンス番号を自動付番
	m_KnOrder	= 0;
	m_KnCode	= _T("");
	m_KnName	= _T("");
	m_FgDel		= 0;
	m_FgDft		= 0;
	m_KnKana	= _T("");
	if(m_IkkatuVal.IsEmpty()){
		m_IkkatuVal = GetIkkatuOrg(inFormSeq);
	}
// midori 160610 add -->
	m_PrSign	= 0;
// midori 160610 add <--

	Update();

	// 新規レコードをレコードセットとして取り出す
	m_strFilter.Format( "FormSeq = %d AND KnSeq = %d", inFormSeq, retVal );	
	m_strSort   = "";

	return RequeryWork();
}

/*
// 正式版完成：とりあえず暫定版で運用、時期を見て切り替える
// (自動採番管理テーブル等の準備が必要なため)
int CdbUcLstKamoku::CreateNewRecord(int inFormSeq)
{
	CRecordset	rs(m_lo_pdb);			// 新規レコード作成用 レコードセット
	CString		SqlStr;					// SQL文（ストアドプロシージャ呼び出し）
	int			NewSeq = -1;			// 新規レコードの Seq 番号格納用
	int			retVal = DB_ERR;		// エラー値格納用

	/////////////////////////////////////////////////////////////////////
	// 新規レコード取得(自動採番付) 用ストアドプロシージャを呼び出す
	//   ストアドプロシージャからは、自動採番された 主キー Seq が戻ってくる
	TRY
	{
		// 新規レコード作成 ストアドプロシージャ実行
		SqlStr.Format("{CALL usp_UcAddNewRecord_UcLstKamoku (%d)}", inFormSeq);

		if ( ! rs.Open(CRecordset::forwardOnly, SqlStr) ) {
			return DB_ERR_OPEN;			// オープンに失敗
		};

		// 主キーの値を取得
		retVal = DB_ERR_EOF;			//レコード(戻り)がなければ エラー番号
		if (!rs.IsEOF()) {					// 戻り値がレコードとして取得できている
			CDBVariant	val;
			rs.GetFieldValue(_T("Seq"), val);	// フィールドの値を取得(主キーの値)
			if (val.m_dwType != DBVT_NULL) {	// NULLでない
				NewSeq = val.m_iVal;			// 値の取得OK
			}
			else {								// フィールドの値 NULL である
				retVal = DB_ERR_NULL;
			}
		}
		rs.Close();

		// 取得した主キーの値をチェック。自動採番できると 1 以上が戻ってくる
		if (NewSeq < 1) {						// 自動採番できなかった
			return DB_ERR_VAL;					// 取得した番号が不正
		}

		/////////////////////////////////////////////////////////////////////
		// 新規レコード初期化作業
		//   新しく作成したレコードを、本クラスのレコードセットとして取得し、
		//   各フィールドを初期化し、テーブルへ反映させる
		//   レコードセットは保持したまま、本メソッドを抜ける

		m_strFilter.Format( "FormSeq = %d AND KnSeq = %d", inFormSeq, NewSeq );	// 削除フラグ OFF（0:有効なレコード)
		m_strSort   = "";							// 並び順でソート

		retVal = RequeryWork();

		if (retVal != DB_ERR_OK) {					// リクエリに失敗した場合
			return retVal;
		}

		// 新規レコードの全フィールド初期化（主キーは除く)
		retVal = DB_ERR_EOF;
		if (!IsEOF()) {				// レコードが取得できている
			Edit();
			// フィールドを初期化
			m_FgDel = 0;
			m_FgDft = 0;
			m_KnCode = _T("");
			m_KnName = _T("");
			m_KnOrder = 0;
			Update();
			retVal = DB_ERR_OK;		// 成功
		}

	}
	CATCH(CDBException, e)
	{
		// エラー コードは e->m_nRetCode に入ります。
		retVal = DB_ERR_EXESQL;					// ExecuteSQL() 失敗
	}

	END_CATCH

	return retVal;
}
*/

/**********************************************************************
	ReNumberOrder()
		並び順番号を付け直す処理

	引数
			int		inFormSeq	該当様式シーケンス番号
			int		inKnOrder	開始地点の並び番号(現在の値)
			int		inIncDec	移動数
									1つ進める場合： 1
									2つ進める場合： 2
									1つ戻す場合  ：-1
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstKamoku::ReNumberOrder(int inFormSeq, int inKnOrder, int inIncDec)
{
	CRecordset	rs(m_lo_pdb);
	CString		SqlStr;
	CString		SqlStrUpd;
	CString		SqlStrSet;
	CString		SqlStrWhe;
//	int			retVal = DB_ERR;

	///////////////////////////////////////////////////////////////
	// SQL 作成

	SqlStrUpd   = _T("UPDATE ") + m_lo_TableName + _T(" ");
	SqlStrSet.Format("SET KnOrder = KnOrder + (%d) ", inIncDec);
	SqlStrWhe.Format("WHERE FormSeq = %d AND KnOrder >= %d AND FgDel = 0", inFormSeq, inKnOrder);

	SqlStr = SqlStrUpd + SqlStrSet + SqlStrWhe;

	///////////////////////////////////////////////////////////////
	// SQL 実行

	return ExecuteSQLWork(SqlStr);
}

/**********************************************************************
	RequeryKnSeq()
		指定科目シーケンス番号のレコード取得

	引数
			nFormSeq			様式シーケンス番号
			nKnSeq				科目シーケンス番号
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstKamoku::RequeryKnSeq( int nFormSeq, int nKnSeq )
{
	///////////////////////////////////////////////////////////////////
	// 目的のレコードを抽出する
	m_strFilter.Format( "FormSeq = %d AND KnSeq = %d",nFormSeq, nKnSeq );	// 指定シーケンス番号
	m_strSort   = "KnOrder";												// 並び順でソート

	return RequeryWork();		//成功
}


/**********************************************************************
	UpdateDataTable()
		データテーブルのデータを名称リストで更新する
		（名称リストテーブルとデータテーブルの整合をとる）

	動作の詳細は、UpdateDataTableSub() を参照。

	引数
			CString inFormSeq	変更先データテーブルを示すFormSeq番号
			int		mode		0:F9参照　1:他マスターから設定情報のコピー

	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstKamoku::UpdateDataTable(int inFormSeq, int mode/*=0*/)
{
	int retVal = DB_ERR_OK;

	switch (inFormSeq) {

	case ID_FORMNO_011:
		retVal = UpdateDataTableSub( STR_TBL_NAME_011, inFormSeq, mode );
		break;
	case ID_FORMNO_021:
		retVal = UpdateDataTableSub( STR_TBL_NAME_021, inFormSeq, mode );
		break;

	case ID_FORMNO_031:
		retVal = UpdateDataTableSub( STR_TBL_NAME_031, inFormSeq, mode );
		break;

	case ID_FORMNO_041:
		retVal = UpdateDataTableSub( STR_TBL_NAME_041, inFormSeq, mode );
		break;

	case ID_FORMNO_051:
		retVal = UpdateDataTableSub( STR_TBL_NAME_051, inFormSeq, mode );
		break;

	case ID_FORMNO_061:
		retVal = UpdateDataTableSub( STR_TBL_NAME_061, inFormSeq, mode );
		break;

// 改良No.21-0086,21-0529 add -->
	case ID_FORMNO_081:
		retVal = UpdateDataTableSub( STR_TBL_NAME_081, inFormSeq, mode );
		break;
// 改良No.21-0086,21-0529 add <--

	case ID_FORMNO_091:
		retVal = UpdateDataTableSub( STR_TBL_NAME_091, inFormSeq, mode );
		break;

	case ID_FORMNO_101:
		retVal = UpdateDataTableSub( STR_TBL_NAME_101, inFormSeq, mode );
		break;

	case ID_FORMNO_102:
		retVal = UpdateDataTableSub( STR_TBL_NAME_102, inFormSeq, mode );
		break;

	case ID_FORMNO_111:
		retVal = UpdateDataTableSub( STR_TBL_NAME_111, inFormSeq, mode );
		break;

	case ID_FORMNO_121:
		retVal = UpdateDataTableSub( STR_TBL_NAME_121, inFormSeq, mode );
		break;

	case ID_FORMNO_151:
		retVal = UpdateDataTableSub( STR_TBL_NAME_151, inFormSeq, mode );
		break;

	case ID_FORMNO_161:
		retVal = UpdateDataTableSub( STR_TBL_NAME_161, inFormSeq, mode );
		break;

	case ID_FORMNO_162:
		retVal = UpdateDataTableSub( STR_TBL_NAME_162, inFormSeq, mode );
		break;

	case ID_FORMNO_171:
	case ID_FORMNO_172:
	case ID_FORMNO_173:
	case ID_FORMNO_174:
	case ID_FORMNO_175:
	case ID_FORMNO_176:
	case ID_FORMNO_177:
	case ID_FORMNO_178:
	case ID_FORMNO_179:
	case ID_FORMNO_1710:
	case ID_FORMNO_1711:
	case ID_FORMNO_1712:
	case ID_FORMNO_1713:
	case ID_FORMNO_1714:
	case ID_FORMNO_1715:
	case ID_FORMNO_1716:
	case ID_FORMNO_1717:
	case ID_FORMNO_1718:
	case ID_FORMNO_1719:
	case ID_FORMNO_1720:

		retVal = UpdateDataTableSub( STR_TBL_NAME_171, inFormSeq, mode );
		break;

	case ID_FORMNO_181:
	case ID_FORMNO_182:
	case ID_FORMNO_183:
	case ID_FORMNO_184:
	case ID_FORMNO_185:
	case ID_FORMNO_186:
	case ID_FORMNO_187:
	case ID_FORMNO_188:
	case ID_FORMNO_189:
	case ID_FORMNO_1810:
	case ID_FORMNO_1811:
	case ID_FORMNO_1812:
	case ID_FORMNO_1813:
	case ID_FORMNO_1814:
	case ID_FORMNO_1815:
	case ID_FORMNO_1816:
	case ID_FORMNO_1817:
	case ID_FORMNO_1818:
	case ID_FORMNO_1819:
	case ID_FORMNO_1820:

		retVal = UpdateDataTableSub( STR_TBL_NAME_181, inFormSeq, mode );
		break;
	}

	return retVal;
}

/**********************************************************************
	UpdateDataTableSub()
		データテーブルのデータを名称リストで更新する
		（名称リストテーブルとデータテーブルの整合をとる）

	(1) シーケンス番号が同じ項目(DataTabel.KnSeq = this.Seq)で更新
			科目名(KnName)、並び順(knOrder)
// midori 152137 add -->
			カナ(KnKana)
// midori 152137 add <--

	(2)	名称リストテーブルに存在しないシーケンス番号が、
		データテーブルに出現した場合(データテーブルで FgDel=0：有効 を対象とする)
		データテーブルのフィールドを下記のように更新する
			シーケンス(KnSeq)を NULL
			並び順(KnOrder)を 0
			科目名(KnName) を NULL
// midori 152137 add -->
			科目ｶﾅ名称(KnKana) を NULL
// midori 152137 add <--

	(3)	データテーブルの名称シーケンス番号が NULL であった場合
		データテーブルのフィールドを下記のように更新する
			並び順(KnOrder)を 0
			科目名(KnName) を NULL
// midori 152137 add -->
			科目ｶﾅ名称(KnKana) を NULL
// midori 152137 add <--

	引数
			CString inTableName	変更先データテーブル名
			int		mode		0:F9参照　1:他マスターから設定情報のコピー

	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstKamoku::UpdateDataTableSub(CString inTableName, int inFormSeq, int mode)
{
	CRecordset	rs(m_lo_pdb);
	CString		SqlStr;
	CString		SqlStrFormSeq;

	int			retVal1 = DB_ERR;
	int			retVal2 = DB_ERR;
	int			retVal3 = DB_ERR;

	///////////////////////////////////////////////////////////////
	//(1)
	///////////////////////////////////////////////////////////////
	// SQL 作成
	SqlStrFormSeq.Format( "%d", inFormSeq );

	SqlStr  = _T("UPDATE ") + inTableName + _T(" ");
	SqlStr += _T("SET KnName = lst.KnName, ");
// midori 152137 add -->
	SqlStr += _T("KnKana = lst.KnKana, ");						// 50音カナ
// midori 152137 add <--
	if( mode == 0 )	SqlStr += _T("KnOrder = lst.KnOrder ");	// F9参照
	else			SqlStr += _T("KnSeq = lst.KnSeq ");		// 他マスターから設定情報のコピー
	SqlStr += _T("FROM ")      + inTableName + _T(" AS data ");
	SqlStr += _T("JOIN ") + m_lo_TableName + _T(" AS lst ");
	SqlStr += _T("ON (");

	if (inFormSeq >= ID_FORMNO_171) {
		SqlStr +=	_T("data.FormSeq = lst.FormSeq ");
		SqlStr +=	_T("AND ");
	}

	if( mode == 0 )		SqlStr +=		_T("data.KnSeq = lst.KnSeq) ");
	else				SqlStr +=		_T("data.KnOrder = lst.KnOrder) ");
	SqlStr +=_T("WHERE lst.FormSeq = ") + SqlStrFormSeq + _T(" ");

	///////////////////////////////////////////////////////////////
	// SQL 実行

	retVal1 = ExecuteSQLWork(SqlStr);


	///////////////////////////////////////////////////////////////
	//(2)
	///////////////////////////////////////////////////////////////
	// SQL 作成
	SqlStr  = _T("UPDATE ") + inTableName + _T(" ");
// midori 152137 del -->
//	SqlStr += _T("SET KnOrder = 0, KnName = NULL, KnSeq = NULL ");
// midori 152137 del <--
// midori 152137 add -->
	SqlStr += _T("SET KnOrder = 0, KnName = NULL, KnSeq = NULL, KnKana = NULL ");
// midori 152137 add <--
	SqlStr += _T("FROM ") + inTableName + _T(" as data ");
	SqlStr += _T("WHERE ");

	if (inFormSeq >= ID_FORMNO_171) {
		SqlStr +=	_T("data.FormSeq = ") + SqlStrFormSeq + _T(" ");
		SqlStr += _T("AND ");
	}

	SqlStr += _T("data.KnSeq NOT IN ( ");
	SqlStr +=	_T("SELECT lst.KnSeq ");
	SqlStr +=	_T("FROM ") + m_lo_TableName  + _T(" AS lst ");
	SqlStr +=   _T("WHERE lst.FormSeq = ");

	if (inFormSeq >= ID_FORMNO_171) {
		SqlStr +=	_T("data.FormSeq ");
	}
	else {
		SqlStr +=	SqlStrFormSeq + _T(" ");
	}

	SqlStr +=	_T("AND FgDel = 0 ");
	SqlStr +=	_T(") ");


	///////////////////////////////////////////////////////////////
	// SQL 実行

	retVal2 = ExecuteSQLWork(SqlStr);


	///////////////////////////////////////////////////////////////
	//(3)
	///////////////////////////////////////////////////////////////
	// SQL 作成
	SqlStr  = _T("UPDATE ") + inTableName + _T(" ");
// midori 152137 del -->
//	SqlStr += _T("SET KnOrder = 0, KnName = NULL ");
// midori 152137 del <--
// midori 152137 add -->
	SqlStr += _T("SET KnOrder = 0, KnName = NULL, KnKana = NULL ");
// midori 152137 add <--
	SqlStr += _T("WHERE ");

	if (inFormSeq >= ID_FORMNO_171) {
		SqlStr +=	_T("FormSeq = ") + SqlStrFormSeq + _T(" ");
		SqlStr +=	_T("AND ");
	}

	SqlStr += _T("KnSeq IS NULL");

	///////////////////////////////////////////////////////////////
	// SQL 実行

	retVal3 = ExecuteSQLWork(SqlStr);

	if (retVal1 != DB_ERR_OK) {
		return retVal1;
	}
	if (retVal2 != DB_ERR_OK) {
		return retVal2;
	}
	return retVal3;
}

/**********************************************************************
	RequeryKnOrder()
		指定科目シーケンス番号のレコード取得

	引数
			nFormSeq			様式シーケンス番号
			nKnOrder			科目並び順
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstKamoku::RequeryKnOrder(int inFormSeq, int inKnOrder)
{
	///////////////////////////////////////////////////////////////////
	// 目的のレコードを抽出する
	m_strFilter.Format( "FormSeq = %d AND KnOrder = %d",inFormSeq, inKnOrder );	//抽出条件
	m_strSort   = "";											// ソート条件

	return RequeryWork();		//成功
}

/**********************************************************************
	CheckRepeat()
		科目名称の重複確認

	引数
			nFormSeq			様式シーケンス番号
			szKnName			科目名称
			szKnKana			科目ｶﾅ名称
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstKamoku::CheckRepeat( int nFormSeq, CString szKnName, CString szKnKana )
{
	int		nRet = DB_ERR;

	// SQLコマンドでは、名称に[']が含まれている場合['']とする
	CString		strKnName = ConvSqlString(szKnName);
	CString		strKnKana = ConvSqlString(szKnKana);

	//	SQL作成
	m_strFilter.Format( "FormSeq = %d ", nFormSeq );											// 指定シーケンス番号
	m_strFilter +=	_T( "AND KnName = '") + strKnName + _T("' COLLATE Japanese_CS_AS_KS_WS ");	// 科目名称
	if( !(szKnKana.IsEmpty()) ){
		m_strFilter +=	_T( "AND KnKana = '") + strKnKana + _T("' COLLATE Japanese_CS_AS_KS_WS ");	// 科目ｶﾅ名称
	}
	m_strSort   =	_T( "");																	// 並び順でソート

	// リクエリ
	nRet = RequeryWork();				
	
	//	戻値を返す
	return( nRet );
}

/**********************************************************************
	CheckEntry()
		名称リスト内に指定されたエントリが登録されているかを調べる
		あれば、そのシーケンス番号(Seq)を返す
		なければ、新規レコードとして登録し、そのシーケンス番号(Seq)を返す

		成功した場合(戻値 > 0)、抽出したレコードセットは、
		該当レコードとなっている。

		指定された名称が、すべて NULL文字列("") の場合は、
		名称テーブルには登録せず、0 を返す。
		レコードセットの状態も不定。

	引数
		CString inKnName
		int		nFormSeq
		CString inKnKana
	戻値
			int					正の数(>0)		：シーケンス番号(エントリーあり)
								0				：登録しなかった(NULL文字列のため)
								上記以外(<0)	：失敗
***********************************************************************/
long CdbUcLstKamoku::CheckEntry(CString inKnName , int nFormSeq, CString inKnKana )
{
	CRecordset	rs(m_lo_pdb);
	CString		SqlStr;
	CString		SqlStrForm;
	int			retVal = DB_ERR;
	long		nKnOrder = 0;

	////////////////////////////////////////////////////////////////
	// 指定された科目名称がNULL 文字列の場合
	if (inKnName == "") {
		return	0;
	}
	
	//	重複確認
	if ( CheckRepeat( nFormSeq , inKnName ) == DB_ERR_OK ){
		//	重複あり？
		if ( !IsEOF() ){
			//	重複あり！
			retVal = m_KnSeq;
			Fin();
			return retVal;
		}
	}
	else{
		return 0;
	}
	
	// 指定された様式のレコードが１件でもあるか確認
	RequeryForm( nFormSeq );
	if( !IsEOF() ){
		/////////////////////////////////////////////////////////////////////
		// 科目の最大順序番号を取得
		SqlStr =		_T("SELECT max(KnOrder) AS MaxNum ");
		SqlStr +=		_T("FROM ") + m_lo_TableName + _T("  ");
		SqlStrForm.Format("WHERE FormSeq = %d" , nFormSeq );
		SqlStr +=		SqlStrForm;

		if ( ! OpenEx( &rs , SqlStr , m_lo_TableName ) ) {
			return 0;
		};

		if (!rs.IsEOF()) {
			CDBVariant	val;
			rs.GetFieldValue(_T("MaxNum"), val);
			if (val.m_dwType == DBVT_NULL) {	// NULLだった場合
				nKnOrder = DBVT_NULL;
			}
			else {								// 値の取得OK
//				nKnOrder = val.m_iVal;
				switch (val.m_dwType) {
				case DBVT_UCHAR:
					nKnOrder = val.m_chVal;		// char
					break;
				case DBVT_SHORT:
					nKnOrder = val.m_iVal;		// short
					break;
				case DBVT_LONG:
					nKnOrder = val.m_lVal;		// long
					break;
				default:
					nKnOrder = val.m_lVal;		// default:long
					break;
				}
			}
		}
		rs.Close();

		// 科目の順序番号が取得できなかった場合
		if ( nKnOrder == 0 ){
			return 0;
		}
		// 順序番号をインクリメント
		nKnOrder++;
	}
	else{
		// 指定された様式の科目が１件も登録されていない場合1を指定
		nKnOrder = 1;
	}

	///////////////////////////////////////////////////////////////
	// 名称リストに登録されていないので、
	// 新規レコードを作成し、格納する
	// KnOrder は、現在の最大値+1の値にする
	if (retVal <= 0) {
		int	ret = CreateNewRecord( nFormSeq );
		if (ret == DB_ERR_OK) {
			Edit();
			m_FormSeq = nFormSeq;			//	様式シーケンス番号設定
			m_KnOrder = nKnOrder;			//	順序番号設定
			m_KnCode = _T("");				//	科目コード設定
			m_KnName = inKnName;			//	科目名称設定
			m_KnKana = inKnKana;			//  科目カナ設定
			m_FgDel = 0;					//	削除フラグ設定
			m_FgDft = 0;					//	デフォルト値フラグ設定
//			m_IkkatuVal = _T("");			//  一括金額→CreateNewRecord()内でセットしている
			if(m_IkkatuVal.IsEmpty()){
				m_IkkatuVal = GetIkkatuOrg(nFormSeq);
			}
// midori 160610 add -->
			m_PrSign = 0;					//	科目出力サイン
// midori 160610 add <--
			Update();						//	レコード更新
			retVal = m_KnSeq;
		}
	}

	// KnOrder 値が、自動採番されたので、レコードを再読込
	RequeryForm( nFormSeq );

	return retVal;

}

// midori 161109 add -->
/**********************************************************************
	CheckEntry_M()
		名称リスト内に指定されたエントリが登録されているかを調べる
		あれば、そのシーケンス番号(Seq)を返す
		なければ、新規レコードとして登録し、そのシーケンス番号(Seq)を返す

		成功した場合(戻値 > 0)、抽出したレコードセットは、
		該当レコードとなっている。

		指定された名称が、すべて NULL文字列("") の場合は、
		名称テーブルには登録せず、0 を返す。
		レコードセットの状態も不定。

	引数
		CString inKnName
		int		nFormSeq
		CString inKnKana
	戻値
			int					正の数(>0)		：シーケンス番号(エントリーあり)
								0				：登録しなかった(NULL文字列のため)
								上記以外(<0)	：失敗
***********************************************************************/
long CdbUcLstKamoku::CheckEntry_M(CString inKnName, int nFormSeq, int nSort, CString inKnKana)
{
	CRecordset	rs(m_lo_pdb);
	CString		SqlStr;
	CString		SqlStrForm;
	int			retVal = DB_ERR;
	long		nKnOrder = 0;

	////////////////////////////////////////////////////////////////
	// 指定された科目名称がNULL 文字列の場合
	if (inKnName == "") {
		return	0;
	}
	
	//	重複確認
	if ( CheckRepeat( nFormSeq , inKnName ) == DB_ERR_OK ){
		//	重複あり？
		if ( !IsEOF() ){
			//	重複あり！
			retVal = m_KnSeq;
			Fin();
			return retVal;
		}
	}
	else{
		return 0;
	}
	
	// 指定された様式のレコードが１件でもあるか確認
	RequeryForm_M( nFormSeq, nSort );
	if( !IsEOF() ){
		/////////////////////////////////////////////////////////////////////
		// 科目の最大順序番号を取得
		SqlStr =		_T("SELECT max(KnOrder) AS MaxNum ");
		SqlStr +=		_T("FROM ") + m_lo_TableName + _T("  ");
		SqlStrForm.Format("WHERE FormSeq = %d" , nFormSeq );
		SqlStr +=		SqlStrForm;

		if ( ! OpenEx( &rs , SqlStr , m_lo_TableName ) ) {
			return 0;
		};

		if (!rs.IsEOF()) {
			CDBVariant	val;
			rs.GetFieldValue(_T("MaxNum"), val);
			if (val.m_dwType == DBVT_NULL) {	// NULLだった場合
				nKnOrder = DBVT_NULL;
			}
			else {								// 値の取得OK
//				nKnOrder = val.m_iVal;
				switch (val.m_dwType) {
				case DBVT_UCHAR:
					nKnOrder = val.m_chVal;		// char
					break;
				case DBVT_SHORT:
					nKnOrder = val.m_iVal;		// short
					break;
				case DBVT_LONG:
					nKnOrder = val.m_lVal;		// long
					break;
				default:
					nKnOrder = val.m_lVal;		// default:long
					break;
				}
			}
		}
		rs.Close();

		// 科目の順序番号が取得できなかった場合
		if ( nKnOrder == 0 ){
			return 0;
		}
		// 順序番号をインクリメント
		nKnOrder++;
	}
	else{
		// 指定された様式の科目が１件も登録されていない場合1を指定
		nKnOrder = 1;
	}

	///////////////////////////////////////////////////////////////
	// 名称リストに登録されていないので、
	// 新規レコードを作成し、格納する
	// KnOrder は、現在の最大値+1の値にする
	if (retVal <= 0) {
		int	ret = CreateNewRecord( nFormSeq );
		if (ret == DB_ERR_OK) {
			Edit();
			m_FormSeq = nFormSeq;			//	様式シーケンス番号設定
			m_KnOrder = nKnOrder;			//	順序番号設定
			m_KnCode = _T("");				//	科目コード設定
			m_KnName = inKnName;			//	科目名称設定
			m_KnKana = inKnKana;			//  科目カナ設定
			m_FgDel = 0;					//	削除フラグ設定
			m_FgDft = 0;					//	デフォルト値フラグ設定
//			m_IkkatuVal = _T("");			//  一括金額→CreateNewRecord()内でセットしている
			if(m_IkkatuVal.IsEmpty()){
				m_IkkatuVal = GetIkkatuOrg(nFormSeq);
			}
// midori 160610 add -->
			m_PrSign = 0;					//	科目出力サイン
// midori 160610 add <--
			Update();						//	レコード更新
			retVal = m_KnSeq;
		}
	}

	// KnOrder 値が、自動採番されたので、レコードを再読込
	RequeryForm_M( nFormSeq, nSort );

	return retVal;

}
// midori 161109 add <--

int CdbUcLstKamoku::SetIkkatuVal(long seq, const CString& val)
{
	if(m_IkkatuVal == val)	return 1;
	if(seq != m_KnSeq)		return 1;	// 念の為科目SEQで一致を確認

	try{
		Edit();

		m_IkkatuVal = val;

		Update();
	}
	catch(...){
		return -1;
	}

	return 0;
}

void CdbUcLstKamoku::GetIkkatuNeedVal(long& seq, int& order)
{
	seq		= m_KnSeq;
	order	= m_KnOrder;
}

void CdbUcLstKamoku::GetIkkatuNeedVal(long& seq, int& order, CString& val, CString& name)
{
	seq		= m_KnSeq;
	order	= m_KnOrder;
	val		= m_IkkatuVal;
	name	= m_KnName;
}

CString CdbUcLstKamoku::GetIkkatuOrg(int nFormSeq)
{
	CString ret;

	// 一括金額のデフォルト値をuc_inf_subから取得
	CdbUcInfSub mfcSub(m_lo_pdb);
	try{
		mfcSub.Init(nFormSeq);
		if(!mfcSub.IsEOF())	ret = mfcSub.m_OpIkkatuValOrg;
		mfcSub.Fin();
	}
	catch(...)
	{
		if(mfcSub.IsOpen())		mfcSub.Fin();
	}
	
	return ret;
}
