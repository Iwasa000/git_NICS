// dbUcLstBank.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
//#include "UCHIWAKE.h"
#include "dbUcLstBank.h"
// midori 190301 add -->
#include "dbUc011Yotyokin.h"
#include "UcFunctionCommon.h"
#include "prtConstData.h"
// midori 190301 add <--
// インボイス登録番号追加対応_23/11/06 add -->
#include "dbUc021Uketoritegata.h"
#include "dbUc081Siharaitegata.h"
// インボイス登録番号追加対応_23/11/06 add <--

// midori 11 add -->
extern	BOOL	bG_Kanso;
// midori 11 add -->
extern	BOOL	bG_InvNo;				// インボイス登録番号追加対応_23/11/06 add

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CdbUcLstBank

IMPLEMENT_DYNAMIC(CdbUcLstBank, CdbBase)

CdbUcLstBank::CdbUcLstBank(CDatabase* pdb)
	: CdbBase(pdb)
	, m_SeqUserStartNum(ID_LST_USER_START)		//Seq(ユーザ利用番号開始値)
{
	//{{AFX_FIELD_INIT(CdbUcLstBank)
	m_BkName1 = _T("");
	m_BkName2 = _T("");
	m_FgDel = 0;
	m_OrderNum = 0;
	m_Seq = 0;
	m_BkKana1 = _T("");
	m_BkKana2 = _T("");
	m_nFields = 7;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	m_lo_pdb = pdb;								// データベースポインタを保持
	m_lo_TableName = STR_TBL_NAME_LST_BANK;		// テーブル名を記述
}


CString CdbUcLstBank::GetDefaultSQL()
{
	return _T("[dbo].[uc_lst_bank]");
}

void CdbUcLstBank::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CdbUcLstBank)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[BkName1]"), m_BkName1);
	RFX_Text(pFX, _T("[BkName2]"), m_BkName2);
	RFX_Byte(pFX, _T("[FgDel]"), m_FgDel);
	RFX_Long(pFX, _T("[OrderNum]"), m_OrderNum);
	RFX_Long(pFX, _T("[Seq]"), m_Seq);
	RFX_Text(pFX, _T("[BkKana1]"), m_BkKana1);
	RFX_Text(pFX, _T("[BkKana2]"), m_BkKana2);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CdbUcLstBank 診断

#ifdef _DEBUG
void CdbUcLstBank::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbUcLstBank::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


/**********************************************************************
	Init()
		初期化処理（Open()の代わりとして使用）

	引数
			なし
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstBank::Init()
{
	///////////////////////////////////////////////////////////////////
	// 目的のレコードを抽出する
	m_strFilter = "FgDel = 0";		// 削除フラグ OFF（0:有効なレコード)
	m_strSort   = "OrderNum";		// 並び順でソート

	return RequeryWork();		//成功
}

// midori 161109 add -->
/**********************************************************************
	Init()
		初期化処理（Open()の代わりとして使用）

	引数
			int	nSort			表示順選択
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstBank::Init_M(int nSort)
{
	///////////////////////////////////////////////////////////////////
	// 目的のレコードを抽出する
	m_strFilter = "FgDel = 0";		// 削除フラグ OFF（0:有効なレコード)
	if(nSort == 0)	{
		m_strSort = "OrderNum";		// 並び順でソート
	}
	else	{
		m_strSort = "BkKana1, BkKana2, OrderNum";		// カナ順でソート
	}

	return RequeryWork();		//成功
}
// midori 161109 add <--

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
/*
// 正式版完成：とりあえず暫定版で運用、時期を見て切り替える
// (自動採番管理テーブル等の準備が必要なため)
int CdbUcLstBank::CreateNewRecord()
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
		SqlStr =	_T("{CALL usp_UcAddNewRecord_UcLstBank}");

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

		m_strFilter.Format( "Seq = %d", NewSeq );	// 削除フラグ OFF（0:有効なレコード)
		m_strSort   = "";							// 並び順でソート

		retVal = RequeryWork();

		if (retVal != DB_ERR_OK) {					// リクエリに失敗した場合
			return retVal;
		}

		// 新規レコードの全フィールド初期化（主キーは除く)
		retVal = DB_ERR_EOF;
		if (!IsEOF()) {				// レコードが取得できている
			Edit();
			m_BkName1 = _T("");		// フィールドを初期化
			m_BkName2 = _T("");
			m_FgDel = 0;
			m_OrderNum = 0;
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

int CdbUcLstBank::CreateNewRecord()
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
	m_Seq = retVal;			// シーケンス番号を自動付番
	m_BkName1 = _T("");		// 残りのフィールドを初期化
	m_BkName2 = _T("");
	m_FgDel = 0;
	m_OrderNum = 0;
	Update();

	m_strFilter.Format( "Seq = %d", retVal );	// 削除フラグ OFF（0:有効なレコード)
	m_strSort   = "";							// 並び順でソート

	return RequeryWork();
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
			CString inBkName1	金融機関名
			CString inBkName2	支店名

	戻値
			int					正の数(>0)		：シーケンス番号(エントリーあり)
								0				：登録しなかった(NULL文字列のため)
								上記以外(<0)	：失敗
***********************************************************************/
int CdbUcLstBank::CheckEntry(CString inBkName1, CString inBkName2)
{
	int			retVal = DB_ERR;

	////////////////////////////////////////////////////////////////
	// 指定された名称が、両方とも NULL 文字列の場合
	if ((inBkName1 == "") && (inBkName2 == "")) {
		return	0;
	}

	//	重複確認
	retVal = CheckRepeat( inBkName1, inBkName2 );

	if (retVal != DB_ERR_OK) {
		return retVal;
	}

	retVal = 0;
	if (!IsEOF()) {			// 該当するレコードがあった
		retVal = m_Seq;		// 該当レコードのシーケンス番号を返す
		if (m_FgDel != 0) {		// 削除フラグの値が 有効行以外になっていたら
			Edit();
			m_FgDel = 0;				// 有効行に戻す
			Update();
			AutoNumOrder(m_Seq);		// (自動採番) Orderに並び順をつける
		}
	}

	///////////////////////////////////////////////////////////////
	// 名称リストに登録されていないので、
	// 新規レコードを作成し、格納する
	// OrderNum は、現在の最大値+1の値にする
	if (retVal <= 0) {
		int	ret = CreateNewRecord();
		if (ret == DB_ERR_OK) {
			CFuriganaConverter fc;

			Edit();
			m_BkName1	= inBkName1;
			m_BkName2	= inBkName2;
			m_OrderNum	= 0;
			m_FgDel		= 0;
			m_BkKana1	= fc.Convert(inBkName1);
			m_BkKana2	= fc.Convert(inBkName2);
			Update();
			AutoNumOrder(m_Seq);		//自動採番
			retVal = m_Seq;
		}
	}

	// OrderNum 値が、自動採番されたので、レコードを再読込
	RequerySeq( retVal );

	return retVal;

}

// midori 152137 add -->
/**********************************************************************
	CheckEntryRen()
		名称リスト内に指定されたエントリが登録されているかを調べる
		あれば、そのシーケンス番号(Seq)を返す
		なければ、新規レコードとして登録し、そのシーケンス番号(Seq)を返す

		成功した場合(戻値 > 0)、抽出したレコードセットは、
		該当レコードとなっている。

		指定された名称が、すべて NULL文字列("") の場合は、
		名称テーブルには登録せず、0 を返す。
		レコードセットの状態も不定。

	引数
			CString inBkName1	金融機関名
			CString inBkName2	支店名
			CString inBkKana	金融機関名（カナ）

	戻値
			int					正の数(>0)		：シーケンス番号(エントリーあり)
								0				：登録しなかった(NULL文字列のため)
								上記以外(<0)	：失敗
***********************************************************************/
int CdbUcLstBank::CheckEntryRen(CString inBkName1, CString inBkName2, CString inBkKana )
{
	int			retVal = DB_ERR;

	////////////////////////////////////////////////////////////////
	// 指定された名称が、両方とも NULL 文字列の場合
	if ((inBkName1 == "") && (inBkName2 == "")) {
		return	0;
	}
	
	//	重複確認
	retVal = CheckRepeat( inBkName1, inBkName2 );

	if (retVal != DB_ERR_OK) {
		return retVal;
	}

	retVal = 0;
	if (!IsEOF()) {			// 該当するレコードがあった
		retVal = m_Seq;		// 該当レコードのシーケンス番号を返す
		if (m_FgDel != 0) {		// 削除フラグの値が 有効行以外になっていたら
			Edit();
			m_FgDel = 0;				// 有効行に戻す
			Update();
			AutoNumOrder(m_Seq);		// (自動採番) Orderに並び順をつける
		}
	}

	///////////////////////////////////////////////////////////////
	// 名称リストに登録されていないので、
	// 新規レコードを作成し、格納する
	// OrderNum は、現在の最大値+1の値にする
	if (retVal <= 0) {
		int	ret = CreateNewRecord();
		if (ret == DB_ERR_OK) {
			CFuriganaConverter fc;

			Edit();
			m_BkName1	= inBkName1;
			m_BkName2	= inBkName2;
			m_OrderNum	= 0;
			m_FgDel		= 0;
			if(inBkKana.IsEmpty() == FALSE)	{
				m_BkKana1	= inBkKana;
				m_BkKana2	= "";
			}
			else	{
				m_BkKana1	= fc.Convert(inBkName1);
				m_BkKana2	= fc.Convert(inBkName2);
			}
			Update();
			AutoNumOrder(m_Seq);		//自動採番
			retVal = m_Seq;
		}
	}

	// OrderNum 値が、自動採番されたので、レコードを再読込
	RequerySeq( retVal );

	return retVal;

}
// midori 152137 add <--

/**********************************************************************
	CheckRepeat()
		名称リスト内に指定されたエントリが登録されているかを調べる
		
	引数
			CString inBkName1	金融機関名
			CString inBkName2	支店名
			CString inBkKana2	金融機関名ｶﾅ
			CString inBkKana2	支店名ｶﾅ

	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstBank::CheckRepeat(CString inBkName1, CString inBkName2, CString inBkKana1/*=NULL*/, CString inBkKana2/*=NULL*/)
{
	int			retVal = DB_ERR;
	
	retVal = UpdateStrFieldAllForNull();

	if (retVal != DB_ERR_OK) {
		return retVal;
	}

	// SQLコマンドでは、名称に[']が含まれている場合['']とする
	CString		strBkName1 = ConvSqlString(inBkName1);
	CString		strBkName2 = ConvSqlString(inBkName2);
	CString		strBkKana1 = ConvSqlString(inBkKana1);
	CString		strBkKana2 = ConvSqlString(inBkKana2);

	///////////////////////////////////////////////////////////////
	// 抽出条件作成
	m_strFilter =	_T("BkName1 = '") + strBkName1 + _T("' COLLATE Japanese_CS_AS_KS_WS ");
	m_strFilter +=	_T("AND BkName2 = '") + strBkName2 + _T("' COLLATE Japanese_CS_AS_KS_WS ");
	if( !(inBkKana1.IsEmpty()) ){
		m_strFilter +=	_T("AND BkKana1 = '") + strBkKana1 + _T("' COLLATE Japanese_CS_AS_KS_WS ");
	}
	if( !(inBkKana2.IsEmpty()) ){
		m_strFilter +=	_T("AND BkKana2 = '") + strBkKana2 + _T("' COLLATE Japanese_CS_AS_KS_WS ");
	}
	m_strSort   =	_T("OrderNum");									// 並び順でソート

	retVal = RequeryWork();				// リクエリ

	return retVal;
}

/**********************************************************************
	UpdateStrFieldForNull()

	銀行名(BkName1)、支店名(BkName2) が NULL になっているものを、
	NULL文字列('')に変更する

	・下記の現象を回避するために実装
		NULL のままだと、以降で使用するSQLで、
		抽出条件 AdName1 + AdName2 (2フィールドの文字列連結)が
		うまく生成されないため。
		どちらかのフィールドに NULL が含まれていると、
		この結果も NULL となってしまう。

	引数
			CString		inFieldName		更新対象のフィールド名
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstBank::UpdateStrFieldAllForNull()
{
	int ret;
	
	ret = UpdateStrFieldForNull("BkName1");

	if (ret == DB_ERR_OK) {
		ret = UpdateStrFieldForNull("BkName2");
	}

// midori 152137 add -->
	if (ret == DB_ERR_OK) {
		ret = UpdateStrFieldForNull("BkKana1");
	}
	if (ret == DB_ERR_OK) {
		ret = UpdateStrFieldForNull("BkKana2");
	}
// midori 152137 add <--

	return ret;
}

/**********************************************************************
	UpdateStrFieldForNull()
		指定フィールドに格納されている値が、NULLの場合、
		NULL文字列（'')に変更する

	引数
			CString		inFieldName		更新対象のフィールド名
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstBank::UpdateStrFieldForNull(CString inFieldName)
{
	CString		SqlStr;

	SqlStr  = _T("UPDATE ") + m_lo_TableName + _T(" ");
	SqlStr += _T("SET ") + inFieldName + _T(" = '' ");
	SqlStr += _T("WHERE ") + inFieldName + _T(" IS NULL ");

	return ExecuteSQLWork(SqlStr);
}

/**********************************************************************
	AutoNumOrder()
		並び順番号を自動採番
		現在登録されている OrderNum の最大値 + 1 した値を自動採番
		引数 inSeq で指定されたレコードの OrderNum に格納

	引数
			int		inSeq		シーケンス番号(Seq)
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstBank::AutoNumOrder(int inSeq)
{
	CRecordset	rs(m_lo_pdb);
	CString		SqlStr;
	CString		SqlStrUpd;
	CString		SqlStrSet1;
	CString		SqlStrSet2;
	CString		SqlStrSet3;
	CString		SqlStrSet4;
	CString		SqlStrWhe1;


	///////////////////////////////////////////////////////////////
	// SQL 作成

	SqlStrUpd   = _T("UPDATE ") + m_lo_TableName + _T(" ");
	SqlStrSet1	= _T("SET OrderNum = ( ");
	SqlStrSet2	=     _T("SELECT TOP 1 (lst.OrderNum + 1) ");
	SqlStrSet3	=     _T("FROM ") + m_lo_TableName + _T(" AS lst ");
	SqlStrSet4	=     _T("ORDER BY lst.OrderNum DESC ) ");
	SqlStrWhe1.Format("WHERE Seq = %d", inSeq);

	SqlStr	= SqlStrUpd  + SqlStrSet1 + SqlStrSet2 + SqlStrSet3 + SqlStrSet4
			+ SqlStrWhe1;

	///////////////////////////////////////////////////////////////
	// SQL 実行

	return ExecuteSQLWork(SqlStr);
}

/**********************************************************************
	ReNumberOrder()
		並び順番号を付け直す処理

	引数
			int		inOrderNum	開始地点の並び番号(現在の値)
			int		inIncDec	移動数
									1つ進める場合： 1
									2つ進める場合： 2
									1つ戻す場合  ：-1
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstBank::ReNumberOrder(int inOrderNum, int inIncDec)
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
	SqlStrSet.Format("SET OrderNum = OrderNum + (%d) ", inIncDec);
	SqlStrWhe.Format("WHERE OrderNum >= %d AND FgDel = 0", inOrderNum);

	SqlStr = SqlStrUpd + SqlStrSet + SqlStrWhe;

	///////////////////////////////////////////////////////////////
	// SQL 実行

	return ExecuteSQLWork(SqlStr);
}

/**********************************************************************
	RequerySeq()
		指定シーケンス番号のレコード取得

	引数
			nSeq				シーケンス番号
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstBank::RequerySeq( int nSeq )
{
	///////////////////////////////////////////////////////////////////
	// 目的のレコードを抽出する
	m_strFilter.Format( "Seq = %d", nSeq );		// 指定シーケンス番号
	m_strSort   = "OrderNum";					// 並び順でソート

	return RequeryWork();		//成功
}


/**********************************************************************
	UpdateDataTable()
		データテーブルのデータを名称リストで更新する
		（名称リストテーブルとデータテーブルの整合をとる）
		対象の全テーブルを更新する。

	引数
			なし
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
// midori 190504 del -->
//int CdbUcLstBank::UpdateDataTableAll(BOOL isRegAuto)
// midori 190504 del <--
// midori 190504 add -->
int CdbUcLstBank::UpdateDataTableAll(BOOL isRegAuto,int nSgn)
// midori 190504 add <--
{
	int retVal = DB_ERR_OK;

// midori 190504 del -->
//	retVal = UpdateDataTable( ID_FORMNO_011, isRegAuto );
//// midori 9 del -->
////// midori 190301 add -->
////	// CdbUcLstBank から挿入した支店名に、改行マークが入っていないので
////	// ここで挿入する
////	Data011Convert();
////// midori 190301 add <--
//// midori 9 del <--
//
//	if (retVal == DB_ERR_OK) {
//		retVal = UpdateDataTable( ID_FORMNO_021, isRegAuto );
//	}
//
//	if (retVal == DB_ERR_OK) {
//		retVal = UpdateDataTable( ID_FORMNO_081, isRegAuto );
//	}
//
//	// ver1_0_2_1から以下の様式も更新
//	if (retVal == DB_ERR_OK) 	retVal = UpdateDataTable( ID_FORMNO_111, isRegAuto );
// midori 190504 del <--
// midori 190504 add -->
	retVal = UpdateDataTable( ID_FORMNO_011, isRegAuto, nSgn );

	if (retVal == DB_ERR_OK) {
		retVal = UpdateDataTable( ID_FORMNO_021, isRegAuto, nSgn );
	}

	if (retVal == DB_ERR_OK) {
		retVal = UpdateDataTable( ID_FORMNO_081, isRegAuto, nSgn );
	}

	// ver1_0_2_1から以下の様式も更新
	if (retVal == DB_ERR_OK) 	retVal = UpdateDataTable( ID_FORMNO_111, isRegAuto, nSgn );
// midori 190504 add <--

	return retVal;
}

/**********************************************************************
	UpdateDataTable()
		データテーブルのデータを名称リストで更新する
		（名称リストテーブルとデータテーブルの整合をとる）

	動作の詳細は、UpdateDataTableSub() を参照。
		
	引数
			int		inFormSeq	変更先データテーブルのフォームシーケンス番号

	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
//int CdbUcLstBank::UpdateDataTable(CString inTableName)
// midori 190504 del -->
//int CdbUcLstBank::UpdateDataTable(int inFormSeq, BOOL isRegAuto)
// midori 190504 del <--
// midori 190504 add -->
int CdbUcLstBank::UpdateDataTable(int inFormSeq, BOOL isRegAuto,int nSgn)
// midori 190504 add <--
{
	int retVal = DB_ERR_OK;

	switch (inFormSeq) {
	case ID_FORMNO_011:
		retVal = UpdateDataTableSub( 
						STR_TBL_NAME_011
						, _T("BkSeq")
						, _T("BkName1")
						, _T("BkName2")
						, _T("BkOrder")
// midori 152137 add -->
						, _T("BkKana1")
						, _T("BkKana2")
// midori 152137 add <--
						, isRegAuto
// midori 190504 add -->
						, ((nSgn == 0) ? TRUE : FALSE)
						, nSgn
// midori 190504 add <--
						);

// midori 11 del -->
//// midori 9 del -->
//		// CdbUcLstBank から挿入した支店名に、改行マークが入っていないので
//		// ここで挿入する
//		Data011Convert();
//// midori 9 del <--
// midori 11 del <--
// midori 11 add -->
		// CdbUcLstBank から挿入した支店名に、改行マークが入っていないので
		// ここで挿入する
// midori 190504 del -->
		//if(bG_Kanso == TRUE) {
// midori 190504 del <--
// midori 190504 add -->
		// 簡素化かつ、末尾スペース除外ではない場合のみ処理を行う
		if(bG_Kanso == TRUE && nSgn == 0) {
// midori 190504 add <--
			Data011Convert();
		}
// midori 11 add <--

		break;

	case ID_FORMNO_021:

		retVal = UpdateDataTableSub( 
						STR_TBL_NAME_021
						, _T("BkSeq")
						, _T("BkName1")
						, _T("BkName2")
						, _T("BkOrder")
// midori 152137 add -->
						, _T("")
						, _T("")
// midori 152137 add <--
						, isRegAuto
// midori 190504 add -->
						, ((nSgn == 0) ? TRUE : FALSE)
						, nSgn
// midori 190504 add <--
						);

// インボイス登録番号追加対応_23/11/06 add -->
		if(bG_InvNo == TRUE && nSgn == 0)	{
			// 上書きされた支払先銀行、支店名に改行マークを差し込む。
			Data021Convert();
		}
// インボイス登録番号追加対応_23/11/06 add <--

		if (retVal == DB_ERR_OK) {
			retVal = UpdateDataTableSub( 
						STR_TBL_NAME_021
						, _T("Bk2Seq")
						, _T("Bk2Name1")
						, _T("Bk2Name2")
						, _T("Bk2Order")
// midori 152137 add -->
						, _T("")
						, _T("")
// midori 152137 add <--
						, isRegAuto
// midori 190504 add -->
						, ((nSgn == 0) ? TRUE : FALSE)
						, nSgn
// midori 190504 add <--
						);
		}
		break;

	case ID_FORMNO_081:
		retVal = 	UpdateDataTableSub( 
						STR_TBL_NAME_081
						, _T("BkSeq")
						, _T("BkName1")
						, _T("BkName2")
						, _T("BkOrder")
// midori 152137 add -->
						, _T("")
						, _T("")
// midori 152137 add <--
						, isRegAuto
// midori 190504 add -->
						, ((nSgn == 0) ? TRUE : FALSE)
						, nSgn
// midori 190504 add <--
						);

// インボイス登録番号追加対応_23/11/06 add -->
		if(bG_InvNo == TRUE && nSgn == 0)	{
			// 上書きされた支払先銀行、支店名に改行マークを差し込む。
			Data081Convert();
		}
// インボイス登録番号追加対応_23/11/06 add <--

		break;

	case ID_FORMNO_111:
		retVal = 	UpdateDataTableSub( 
						STR_TBL_NAME_111
						, _T("AdSeq")
						, _T("AdName1")
						, _T("AdName2")
						, _T("AdOrder")
// midori 152137 add -->
						, _T("AdKana1")
						, _T("AdKana2")
// midori 152137 add <--
						, isRegAuto
						, FALSE
// midori 190504 add -->
						, nSgn
// midori 190504 add <--
						);

		break;

	}

	return retVal;

}

/**********************************************************************
	UpdateDataTableSub()
		データテーブルのデータを名称リストで更新する
		（名称リストテーブルとデータテーブルの整合をとる）

	(1) シーケンス番号が同じ項目(DataTabel.BkSeq = this.Seq)で更新
			銀行名(BkName1)、支店名(BkName2)、並び順(BkOrder)
// midori 152137 add -->
			カナ(金融機関)(BkKana1)、カナ(支店)(BkKana2)
// midori 152137 add <--

  	(2)	データテーブルの名称シーケンス番号が NULL であった場合
		データテーブルのデータを以下とする
			銀行シーケンス番号(BkSeq) = 0

	(3)	名称リストテーブルに存在しないシーケンス番号が、
		データテーブルに出現した場合(データテーブルで FgDel=0：有効 を対象とする)
		データテーブルのデータを以下とする
			銀行名(BkName1) = ""
			支店名(BkName2) = ""
			並び順(BkOrder) = 0
			シーケンス(BkSeq) = 0
// midori 152137 add -->
			カナ(金融機関)(BkKana1) = ""
			カナ(支店)(BkKana2) = ""
// midori 152137 add <--
	(4) 金融機関シーケンスを銀行名、支店名から更新（念の為オーダーも）

	※ 金融機関名称は、⑰⑱のその他帳表では、使用していないため、
	   そのデータテーブルに対する FormSeq の対応を入れていない

	引数
			CString inTableName		変更先データテーブル名
			CString inNameBkSeq		データテーブルのフィールド名（銀行シーケンス番号)
			CString inNameBkName1	データテーブルのフィールド名（銀行名)
			CString inNameBkName2	データテーブルのフィールド名（支店名)
			CString inNameBkOrder	データテーブルのフィールド名（順序番号)
// midori 152137 add -->
			CString inNameBkKana1	データテーブルのフィールド名（カナ（金融機関））
			CString inNameBkKana2	データテーブルのフィールド名（カナ（支店））
// midori 152137 add <--
			BOOL	isDeleteAdSeqZero F9自動登録しない様式（⑪）は(3)を実行しない

	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstBank::UpdateDataTableSub(
						  CString inTableName		// データテーブル名
						, CString inNameBkSeq		// フィールド名
						, CString inNameBkName1		// フィールド名
						, CString inNameBkName2		// フィールド名
						, CString inNameBkOrder		// フィールド名
// midori 152137 add -->
						, CString inNameBkKana1		// フィールド名（カナ（金融機関））
						, CString inNameBkKana2		// フィールド名（カナ（支店））
// midori 152137 add <--
						, BOOL isRegAuto
						, BOOL isDeleteAdSeqZero
// midori 190504 add -->
						, int nSgn
// midori 190504 add <--
)
{
	CRecordset	rs(m_lo_pdb);
	CString		SqlStr;

	int			retVal1 = DB_ERR;
	int			retVal2 = DB_ERR;
	int			retVal3 = DB_ERR;
	int			retVal4 = DB_ERR;

	///////////////////////////////////////////////////////////////
	//(1)
	///////////////////////////////////////////////////////////////
	// SQL 作成

// midori 190504 add -->
	// 起動時、末尾スペースチェック(DeleteSpaceMaster)から呼ばれた場合は処理を行わない
	if(nSgn == 0) {
// midori 190504 add <--
		SqlStr  = _T("UPDATE ") + inTableName + _T(" ");
		SqlStr += _T("SET ")  + inNameBkName1 + _T(" = lst.BkName1, ");
		SqlStr +=				inNameBkName2 + _T(" = lst.BkName2, ");
// midori 152137 add -->
		// カナ入力のある様式はカナの登録を行う
		if(inNameBkKana1.IsEmpty() == FALSE) {
			SqlStr += inNameBkKana1 + _T(" = lst.BkKana1, ");
		}
		if(inNameBkKana2.IsEmpty() == FALSE) {
			SqlStr += inNameBkKana2 + _T(" = lst.BkKana2, ");
		}
// midori 152137 add <--
		SqlStr +=				inNameBkOrder + _T(" = lst.OrderNum ");
		SqlStr += _T("FROM ") + inTableName + _T(" AS data ");
		SqlStr += _T("JOIN ") + m_lo_TableName + _T(" AS lst ");
		SqlStr += _T("ON data.") + inNameBkSeq + _T(" = lst.Seq");

		///////////////////////////////////////////////////////////////
		// SQL 実行

		retVal1 = ExecuteSQLWork(SqlStr);
// midori 190504 add -->
	}
	else	{
		retVal1 = DB_ERR_OK;
	}
// midori 190504 add <--

	///////////////////////////////////////////////////////////////
	//(2)
	///////////////////////////////////////////////////////////////
	// SQL 作成

	SqlStr  = _T("UPDATE ") + inTableName + _T(" ");
	SqlStr += _T("SET ")   + inNameBkSeq + _T(" = 0 ");
	SqlStr += _T("WHERE ") + inNameBkSeq + _T(" IS NULL");

	///////////////////////////////////////////////////////////////
	// SQL 実行

	retVal2 = ExecuteSQLWork(SqlStr);

// midori 190504 add -->
	// 起動時、末尾スペースチェック(DeleteSpaceMaster)から呼ばれた場合は処理を行わない
	if(nSgn == 0) {
// midori 190504 add <--
		///////////////////////////////////////////////////////////////
		//(3)
		///////////////////////////////////////////////////////////////
		// SQL 作成
		//2015/10/06 ver1.0.2.1 「金融機関を自動で登録しない」機能が追加され、uc_lst_bankに登録されていない
		//inNameBkSeqが0の金融機関も存在を許すことになった
		//↓↓↓ここから↓↓↓
		SqlStr  = _T("UPDATE ") + inTableName + _T(" ");
		SqlStr += _T("SET ") + inNameBkSeq   + _T(" = 0, ");
		SqlStr +=              inNameBkName1 + _T(" = '', ");
		SqlStr +=              inNameBkName2 + _T(" = '', ");
// midori 152137 add -->
			// カナ入力のある様式はカナの初期化を行う
		if(inNameBkKana1.IsEmpty() == FALSE) {
			SqlStr += inNameBkKana1 + _T(" = '', ");
		}
		if(inNameBkKana2.IsEmpty() == FALSE) {
			SqlStr += inNameBkKana2 + _T(" = '', ");
		}
// midori 152137 add <--
		SqlStr += inNameBkOrder + _T(" = 0 ");
		SqlStr += _T("WHERE FgFunc = 1 ");
		//if((isDeleteAdSeqZero == FALSE) || (isRegAuto)){
			//SqlStr += _T("AND BkSeq <> 0 ");
		SqlStr += _T("AND ") + inNameBkSeq + _T(" <> 0 ");
		//}
		SqlStr += _T("AND ") + inNameBkSeq + _T(" NOT IN ( ");
		SqlStr += _T("SELECT lst.Seq ");
		SqlStr += _T("FROM ") + m_lo_TableName  + _T(" AS lst ");
		SqlStr += _T("WHERE FgDel = 0 ");
		SqlStr += _T(") ");


		///////////////////////////////////////////////////////////////
		// SQL 実行

		retVal3 = ExecuteSQLWork(SqlStr);
		// ↑↑↑ここまで↑↑↑
// midori 190504 add -->
	}
	else	{
		retVal3 = DB_ERR_OK;
	}
// midori 190504 add <--

	///////////////////////////////////////////////////////////////
	//(4)
	///////////////////////////////////////////////////////////////
	if(isDeleteAdSeqZero == FALSE){
		CString sql;
		sql  = _T("UPDATE ") + inTableName + _T(" ");
		sql += _T("SET ")  + inNameBkSeq + _T(" = lst.Seq, ");
// midori 152137 add -->
		// カナ入力のある様式はカナの登録を行う
		if(inNameBkKana1.IsEmpty() == FALSE)	{
			sql += inNameBkKana1 + _T(" = lst.BkKana1, ");
		}
		if(inNameBkKana2.IsEmpty() == FALSE)	{
			sql += inNameBkKana2 + _T(" = lst.BkKana2, ");
		}
// midori 152137 add <--
		sql +=				inNameBkOrder + _T(" = lst.OrderNum ");
		sql += _T("FROM ") + inTableName + _T(" AS data ");
		sql += _T("JOIN ") + m_lo_TableName + _T(" AS lst ");
// 157270 del -->
//// midori 190504 del -->
//		//sql += _T("ON data.") + inNameBkName1 + _T(" = lst.BkName1 ");
//		//sql += _T("AND data.") + inNameBkName2 + _T(" = lst.BkName2 ");
//// midori 190504 del <--
//// midori 190504 add -->
//		// ①NULLを''に置き換える
//		// ②支店名は改行文字[CHAR(13)+CHAR(10)]を''に置き換える
//		sql += _T("ON isnull(data.") + inNameBkName1 + _T(",'') = isnull(lst.BkName1,'') ");
//		sql += _T("AND isnull(replace(data.") + inNameBkName2 + _T(",CHAR(13)+CHAR(10),''),'') = ");
//		sql += _T("isnull(lst.BkName2,'') ");
//// midori 190504 add <--
// 157270 del <--
// 157270 add -->
		// ①NULLを''に置き換える
		// ②支店名は改行文字[CHAR(13)+CHAR(10)]を''に置き換える
		// ③全角・半角を区別する
		sql += _T("ON isnull(data.") + inNameBkName1 + _T(",'') collate Japanese_CS_AS_KS_WS = isnull(lst.BkName1,'') collate Japanese_CS_AS_KS_WS ");
		sql += _T("AND isnull(replace(data.") + inNameBkName2 + _T(",CHAR(13)+CHAR(10),''),'') collate Japanese_CS_AS_KS_WS = ");
		sql += _T("isnull(lst.BkName2,'') collate Japanese_CS_AS_KS_WS ");
// 157270 add <--

		retVal4 = ExecuteSQLWork(sql);

// midori 190504 add -->
		//// この後の処理で改行マークが挿入されるので、入っていれば除外しておく
		//if(retVal4 == DB_ERR_OK) {
		//	if(bG_Kanso == TRUE) {
		//		sql  = _T("UPDATE ") + inTableName + _T(" ");
		//		sql += _T("SET ")  + inNameBkName2 + _T(" = replace(") + inNameBkName2;
		//		sql += _T(",CHAR(13)+CHAR(10),'')");
		//		retVal4 = ExecuteSQLWork(sql);
		//	}
		//}
// midori 190504 add <--

	}
	else	retVal4 = DB_ERR_OK;

	if (retVal1 != DB_ERR_OK) {
		return retVal1;
	}
	if (retVal2 != DB_ERR_OK) {
		return retVal2;
	}
	if(retVal3 != DB_ERR_OK){
		return retVal3;
	}
	return retVal4;
}

/**********************************************************************
		F9から参照はするが登録はしない場合、銀行名からAdSeq,AdOrderを取得する
		
	引数
			bkname1		銀行名
			bkname2		支店名

	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstBank::RequeryBkName(CString bkname1, CString bkname2)
{
// midori 160602 cor -->
	//m_strFilter = "BkName1 = '"+ bkname1 +"' AND BkName2 = '"+ bkname2 +"' ";
	//m_strSort   = "";
// ---------------------
	CString	strBkName1=_T("");
	CString	strBkName2=_T("");

	// シングルクォーテーションを置き換える
	// (')	 → ('')
	// ('')	 → ('''')
	// (''') → (''''')
	strBkName1=bkname1;
	strBkName1.Replace(_T("'"),_T("''"));
	strBkName2=bkname2;
	strBkName2.Replace(_T("'"),_T("''"));

// 157270 del -->
	//m_strFilter = "BkName1 = '"+ strBkName1 +"' AND BkName2 = '"+ strBkName2 +"' ";
// 157270 del <--
// 157270 add -->
	m_strFilter = "BkName1 collate Japanese_CS_AS_KS_WS = '"+ strBkName1 +"' AND BkName2 collate Japanese_CS_AS_KS_WS = '"+ strBkName2 +"' ";
// 157270 add <--
	m_strSort   = "";
// midori 160602 cor <--

	return RequeryWork();
}

/**********************************************************************
		銀行名から登録されていればその銀行名のシーケンス、
		されていなければ0を返す
		
	引数
			bkname1		銀行名
			bkname2		支店名

	戻値
			int		登録されている：Seq
					登録されていない：0
					エラー：-1
***********************************************************************/
int CdbUcLstBank::GetSeqFromBkName(CString bkname1, CString bkname2)
{
	// 銀行名、支店名のどちらかがNULLの場合がある
	int ret = UpdateStrFieldAllForNull();
	if(ret != DB_ERR_OK)	return -1;	// エラー

	ret = RequeryBkName(bkname1, bkname2);

	if(ret != DB_ERR_OK)	return -1;	// エラー
	if(this->IsEOF())		return 0;	// 登録されていない
	return this->m_Seq;					// 登録済み
}

// midori 190301 add -->
// CdbUcLstBank からセットした支店名に、改行マークを挿入する
void CdbUcLstBank::Data011Convert( void )
{
	CdbUc011Yotyokin	db011(m_lo_pdb);
	int					sw=0;
	CString				cs=_T("");
	CString				cs1=_T("");
	CString				cs2=_T("");
	CString				cs3=_T("");
	CUcFunctionCommon	ufc;

	db011.Open();
	while(!db011.IsEOF()){		// ファイル終端まで
		sw=0;
		cs = db011.m_BkName2;
// midori 190504 del -->
		//if(cs.IsEmpty() == FALSE)	{
// midori 190504 del <--
// midori 190504 add -->
		if(cs.IsEmpty() == FALSE && db011.m_BkOrder != 0)	{
// midori 190504 add <--
			// 全角５文字を超えていれば改行マークを挿入して取り込む
			if(cs.GetLength() > 10)	{
				cs2 = ufc.GetSpritString(cs,&cs1,10);
				cs = cs2;
				cs3 = ufc.GetSpritString(cs,&cs2,10);
				cs = cs1 + PRT_FORMAT_CHCODE + cs2;
				sw = 1;
			}
		}
		if(sw != 0)	{
			db011.Edit();
			db011.m_BkName2 = cs;
			db011.Update();
		}
		db011.MoveNext();
	}
	db011.Close();
}
// midori 190301 add <--

// インボイス登録番号追加対応_23/11/06 add -->
// [様式②]支払先銀行および支店名に改行マークを差し込む
void CdbUcLstBank::Data021Convert( void )
{
	CdbUc021Uketoritegata	db021(m_lo_pdb);
	int						sw=0;
	CString					bk1=_T(""),bk2=_T("");
	CString					cs=_T("");
	CString					cs1=_T("");
	CString					cs2=_T("");
	CString					cs3=_T("");
	CUcFunctionCommon		ufc;

	db021.Open();
	while(!db021.IsEOF()){		// ファイル終端まで
		sw=0;
		bk1.Empty();
		bk2.Empty();
		// 銀行名
		cs = db021.m_BkName1;
		if(cs.IsEmpty() == FALSE && db021.m_BkOrder != 0)	{
			// 全角５文字を超えていれば改行マークを挿入して取り込む
			if(cs.GetLength() > 10)	{
				cs2 = ufc.GetSpritString(cs,&cs1,10);
				cs = cs2;
				cs3 = ufc.GetSpritString(cs,&cs2,10);
				bk1 = cs1 + PRT_FORMAT_CHCODE + cs2;
				sw = 1;
			}
			else {
				bk1 = cs;
			}
		}
		// 支店名
		cs = db021.m_BkName2;
		if(cs.IsEmpty() == FALSE && db021.m_BkOrder != 0)	{
			// 全角５文字を超えていれば改行マークを挿入して取り込む
			if(cs.GetLength() > 10)	{
				cs2 = ufc.GetSpritString(cs,&cs1,10);
				cs = cs2;
				cs3 = ufc.GetSpritString(cs,&cs2,10);
				bk2 = cs1 + PRT_FORMAT_CHCODE + cs2;
				sw = 1;
			}
			else {
				bk2 = cs;
			}
		}
		if(sw != 0)	{
			db021.Edit();
			db021.m_BkName1 = bk1;
			db021.m_BkName2 = bk2;
			db021.Update();
		}
		db021.MoveNext();
	}
	db021.Close();
}

// [様式⑧]支払先銀行および支店名に改行マークを差し込む
void CdbUcLstBank::Data081Convert( void )
{
	CdbUc081Siharaitegata	db081(m_lo_pdb);
	int						sw=0;
	CString					bk1=_T(""),bk2=_T("");
	CString					cs=_T("");
	CString					cs1=_T("");
	CString					cs2=_T("");
	CString					cs3=_T("");
	CUcFunctionCommon		ufc;

	db081.Open();
	while(!db081.IsEOF()){		// ファイル終端まで
		sw=0;
		bk1.Empty();
		bk2.Empty();
		// 銀行名
		cs = db081.m_BkName1;
		if(cs.IsEmpty() == FALSE && db081.m_BkOrder != 0)	{
			// 全角５文字を超えていれば改行マークを挿入して取り込む
			if(cs.GetLength() > 10)	{
				cs2 = ufc.GetSpritString(cs,&cs1,10);
				cs = cs2;
				cs3 = ufc.GetSpritString(cs,&cs2,10);
				bk1 = cs1 + PRT_FORMAT_CHCODE + cs2;
				sw = 1;
			}
			else {
				bk1 = cs;
			}
		}
		// 支店名
		cs = db081.m_BkName2;
		if(cs.IsEmpty() == FALSE && db081.m_BkOrder != 0)	{
			// 全角５文字を超えていれば改行マークを挿入して取り込む
			if(cs.GetLength() > 10)	{
				cs2 = ufc.GetSpritString(cs,&cs1,10);
				cs = cs2;
				cs3 = ufc.GetSpritString(cs,&cs2,10);
				bk2 = cs1 + PRT_FORMAT_CHCODE + cs2;
				sw = 1;
			}
			else {
				bk2 = cs;
			}
		}
		if(sw != 0)	{
			db081.Edit();
			db081.m_BkName1 = bk1;
			db081.m_BkName2 = bk2;
			db081.Update();
		}
		db081.MoveNext();
	}
	db081.Close();
}
// インボイス登録番号追加対応_23/11/06 add <--

// midori 190504 add -->
// ----------------------------------------------------------------------------------------------------
//	RequeryBkName()
//		
//	銀行名、銀行名カナ、支店名、支店名カナ順にレコードを抽出する
//
//	引数
//			なし
//	戻値
//			int					DB_ERR_OK (0)     ：成功
//								DB_ERR_OK (0) 以外：失敗
// ----------------------------------------------------------------------------------------------------
int CdbUcLstBank::RequeryBkName()
{
	// 目的のレコードを抽出する
	m_strFilter = "FgDel = 0";								// 削除フラグ OFF（0:有効なレコード)
// midori 155278 del -->
//	m_strSort   = "BkName1,BkKana1,BkName2,BkKana2";		// 銀行名、銀行名カナ、支店名、支店名カナ順にソート
// midori 155278 del <--
// midori 155278 add -->
	m_strSort   = "BkName1,BkKana1,BkName2,BkKana2,OrderNum";	// 銀行名、銀行名カナ、支店名、支店名カナ、オーダーNo順にソート
// midori 155278 add <--

	return RequeryWork();		//成功
}
// midori 190504 add <--
