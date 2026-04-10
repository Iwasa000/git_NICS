// dbBase.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dbBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CdbBase

IMPLEMENT_DYNAMIC(CdbBase, CRecordset)

CdbBase::CdbBase(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CdbBase)
	//}}AFX_FIELD_INIT
	m_lo_pdb = pdb;								// データベースポインタを保持
}

// デストラクタ
CdbBase::~CdbBase()
{
	// 開いたままになっていたらクローズする
	if ( IsOpen() ){
		Fin();
	}
}

void CdbBase::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CdbBase)
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CdbUcBase 診断
#ifdef _DEBUG
void CdbBase::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbBase::Dump(CDumpContext& dc) const
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
int CdbBase::Init()
{
	return RequeryWork();
}

/**********************************************************************
	Fin()
		終了処理（Close()の代わりとして使用）

	引数
			なし
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbBase::Fin()
{
	Close();
	return	DB_ERR_OK;
}

/**********************************************************************
	RequeryWork()
		リクエリを実行する 補助メソッド

	引数
			なし
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbBase::RequeryWork()
{
	if ( ! IsOpen() ) {		// オープンしていない場合
		if ( ! OpenEx() ) {
			return DB_ERR_OPEN;		// オープンに失敗
		}
	}
	else {					// すでにオープンしている場合
		if ( ! Requery() ) {
			return DB_ERR_REQU;		// リクエリに失敗
		}
	}

	return DB_ERR_OK;
}

/**********************************************************************
	ExecuteSQLWork()
		SQL文を実行する 補助メソッド
		INSERT, UPDATE, DELETE などのSQL文を実行するための補助メソッド

	引数
			CString	inStrSql	実行する SQL文
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbBase::ExecuteSQLWork(CString inStrSql)
{
	int			retVal = DB_ERR;

	TRY
	{
		m_lo_pdb->ExecuteSQL(inStrSql);
		retVal = DB_ERR_OK;						//成功
	}
	CATCH(CDBException, e)
	{
		// エラー コードは e->m_nRetCode に入ります。
		retVal = DB_ERR_EXESQL;					// ExecuteSQL() 失敗
	}

	END_CATCH

	return retVal;
}

/**********************************************************************
	OpenEx()
		Open()関数機能拡張関数（Open+DBエラー対応）

	引数
			なし
	戻値
			BOOL					FALSE	(1)  ：成功
									TRUE	(0)  ：失敗
***********************************************************************/
BOOL CdbBase::OpenEx()
{
	BOOL bRet = FALSE;	// 戻り値※オープン失敗値で初期化

	try{
		// テーブルオープン
		bRet = Open();

	}catch( CDBException *e){

		// DBエラーメッセージ表示～アプリ終了(ICSExit)
		DBErrExit( e->m_strError , m_lo_TableName );

		// 開放
		e->Delete();

		bRet = FALSE;
	}

	return bRet;

}

/**********************************************************************
	OpenEx()
		Open()関数機能拡張関数（Open+DBエラー対応）
		CRecordsetクラスからのオープン用

	引数
			CString		strTblName	エラーメッセージに表示するテーブル名
			CRecordset	rs			Openするテーブルクラスのポインタ
									※ポインタがNULLの場合は現在のクラスのテーブルを開く

			CString		strSqlStr	上記テーブルOpen時に指定する条件クエリ文
	戻値
			BOOL					FALSE	(1)  ：成功
									TRUE	(0)  ：失敗
***********************************************************************/
BOOL CdbBase::OpenEx( CRecordset* rs , CString strSqlStr , CString strTblName )
{
	BOOL bRet = FALSE;	// 戻り値※オープン失敗値で初期化

	try{
		if ( rs == NULL ){
			// テーブルオープン
			bRet = Open(CRecordset::forwardOnly, strSqlStr);		}
		else{
			// テーブルオープン
			bRet = rs->Open(CRecordset::forwardOnly, strSqlStr);
		}

	}catch( CDBException *e){

		// DBエラーメッセージ表示～アプリ終了(ICSExit)
		DBErrExit( e->m_strError , strTblName );

		// 開放
		e->Delete();


		bRet = FALSE;
	}

	return bRet;

}

/**********************************************************************
	ConvSqlString()
		SQLコマンドで使用する文字列変換処理（[']→['']）

	引数
			CString		strOrg		変換後の文字列
	戻値
			CString					変換後の文字列
***********************************************************************/
CString CdbBase::ConvSqlString(CString strOrg)
{
	strOrg.Replace(_T("'"), _T("''"));
	return strOrg;
}

/**********************************************************************
	DBErrExit()
		テーブル参照時のエラー表示～アプリ終了

	引数
			CString		strError	例外エラーメッセージ
			CString		strTblName	エラーメッセージに表示するテーブル名
	戻値
			なし
***********************************************************************/
void CdbBase::DBErrExit( CString strError , CString strTblName )
{
	CString strErrMsg;		// エラーメッセージ

	// 初期化
	strErrMsg.Empty();

	//	DBエラーメッセージ1行目を作成
	strErrMsg = strTblName + _T("が開けません。");

	// "。"で改行する
	strErrMsg.Replace( "。" , "。\r\n" );

	//	DBエラーメッセージ文字列 取得
	strErrMsg += strError;

	//	2007.05.21 メッセージダイアログ変更(ICSExit→ICSMessageBox)
	//	エラーメッセージウィンドウ表示
//	ICSExit( 0 , (char*)(LPCSTR)strErrMsg);

	// メッセージ表示
	ICSMessageBox( strErrMsg , MB_ICONSTOP );

	// 終了メッセージ送信
	AfxGetMainWnd()->PostMessage( WM_CLOSE, 1, 0 );

}



