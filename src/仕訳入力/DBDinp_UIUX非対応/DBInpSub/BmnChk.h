///////////////////////////////////////////////////////////
// 部門範囲チェック クラス
//		DBInpSub.lib
//		DBInpSub.dll
//
//


#pragma once


class CBmnChkSub;


// 部門範囲チェック クラス
#ifdef _DBINPSUB
class AFX_EXT_CLASS CBmnHaniCheck
#else
class CBmnHaniCheck
#endif
{
public:
	CBmnHaniCheck();
	virtual ~CBmnHaniCheck();

	//-----------------------------------------------------------
	//	マスターのDB に接続済みの財務クラスをセット
	//	VolumeOpen(), BmnameOpen() は 必須
	//		返送値	-1: エラー
	//				 0: OK
	//
	//-----------------------------------------------------------
	int		SetDBZmSub( CDBZmSub* pZmSub );

	//-----------------------------------------------------------
	//	部門チェッククラスの初期化
	//	内部で、bmname->Requery( "","" )を呼んでいます。
	//		返送値	-1: エラー
	//				 0: OK
	//
	//-----------------------------------------------------------
	int		BmnCheckInit();

	//-----------------------------------------------------------
	//	部門名称を追加したときや、他プログラムからの戻りなど、
	//  内容の更新が必要な時
	//	内部で、bmname->Requery( "","" )を呼んでいます。
	//-----------------------------------------------------------
	int		BmnCheckUpdate();

	//-----------------------------------------------------------
	//	部門チェック
	//		int		bmn			部門コード
	//		DWORD	kmkcode		内部コード (DBKNREC*のKNcod)
	//
	//		zvol->bm_isw と bmname->grpsgn にしたがって、
	//		部門範囲のチェックをします。
	//		return TRUE ... チェックＯＫ
	//
	//-----------------------------------------------------------
	BOOL	BmnHaniCheck( int bmn, DWORD kmkcode );

	//-----------------------------------------------------------
	//	部門チェック(科目コード文字列版)
	//		int		bmn			部門コード
	//		CString	strCode		内部コード (文字列 dbdata->dbt,cre)
	//
	//-----------------------------------------------------------
	BOOL	BmnHaniCheck( int bmn, CString strCode );

private:
	CBmnChkSub*	pSub;
	char	rsv[64];
};







class CKojiChkSub;


// 工事科目 チェック クラス
#ifdef _DBINPSUB
class AFX_EXT_CLASS CKojiDataCheck
#else
class CKojiDataCheck
#endif
{
public:
	CKojiDataCheck();
	virtual ~CKojiDataCheck();

	//-----------------------------------------------------------
	//	マスターのDB に接続済みの財務クラスをセット
	//	VolumeOpen(), BmnameOpen() は 必須
	//		返送値	-1: エラー
	//				 0: OK
	//
	//-----------------------------------------------------------
	int		SetDBZmSub( CDBZmSub* pZmSub );

	//-----------------------------------------------------------
	//	部門チェッククラスの初期化
	//	内部で、bmname->Requery( "","" )を呼んでいます。
	//		返送値	-1: エラー
	//				 0: OK
	//
	//-----------------------------------------------------------
	int		KojiCheckInit();

	//-----------------------------------------------------------
	//	他プログラムからの戻りなど、
	//  内容の更新が必要な時
	//	内部 工事科目コードテーブルを更新します。
	//-----------------------------------------------------------
	int		KojiCheckUpdate();

	//-----------------------------------------------------------
	//	工事番号チェック
	//		DWORD	kmkcode		内部コード (DBKNREC*のKNcod)
	//
	//		return TRUE ... チェックＯＫ
	//
	//-----------------------------------------------------------
	BOOL	KojiKmkCheck( DWORD kmkcode );

	//-----------------------------------------------------------
	//	部門チェック(科目コード文字列版)
	//		CString	strCode		内部コード (文字列 dbdata->dbt,cre)
	//
	//-----------------------------------------------------------
	BOOL	KojiKmkCheck( CString strCode );

private:
	CKojiChkSub*	pSub;
	char	rsv[64];
};