// dbUcLstAddress.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
//#include "UCHIWAKE.h"
#include "dbUcLstAddress.h"
#include "FuriganaConverter.h"
// midori 190301 add -->
#include "dbUc042Kasituke.h"
#include "UcFunctionCommon.h"
#include "prtConstData.h"
// midori 190301 add <--

// midori 12 add -->
extern	BOOL	bG_Kanso;
// midori 12 add -->

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CdbUcLstAddress

IMPLEMENT_DYNAMIC(CdbUcLstAddress, CdbBase)


CdbUcLstAddress::CdbUcLstAddress(CDatabase* pdb)
	: CdbBase(pdb)
	, m_SeqUserStartNum(ID_LST_USER_START)		//AdSeq(ユーザ利用番号開始値)
{
	//{{AFX_FIELD_INIT(CdbUcLstAddress)
	m_AdSeq = 0;
	m_GrSeq = 0;
	m_OrderNum = 0;
	m_AdName1 = _T("");
	m_AdName2 = _T("");
	m_AdAdd1 = _T("");
	m_AdAdd2 = _T("");
	m_FgDel = 0;
	m_AdKana = _T("");
	m_IkkatuVal = _T("");
// インボイス登録番号追加対応_23/11/06 del -->
	//m_nFields = 10;
// インボイス登録番号追加対応_23/11/06 del <--
// インボイス登録番号追加対応_23/11/06 add -->
	m_InvNo =  _T("");
	m_nFields = 11;
// インボイス登録番号追加対応_23/11/06 add <--
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	m_lo_pdb = pdb;								// データベースポインタを保持
	m_lo_TableName = STR_TBL_NAME_LST_ADDRESS;	// テーブル名を記述
}

CString CdbUcLstAddress::GetDefaultSQL()
{
	return _T("[dbo].[uc_lst_address]");
}

void CdbUcLstAddress::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CdbUcLstAddress)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[AdSeq]"), m_AdSeq);
	RFX_Int(pFX, _T("[GrSeq]"), m_GrSeq);
	RFX_Long(pFX, _T("[OrderNum]"), m_OrderNum);
	RFX_Text(pFX, _T("[AdName1]"), m_AdName1);
	RFX_Text(pFX, _T("[AdName2]"), m_AdName2);
	RFX_Text(pFX, _T("[AdAdd1]"), m_AdAdd1);
	RFX_Text(pFX, _T("[AdAdd2]"), m_AdAdd2);
	RFX_Byte(pFX, _T("[FgDel]"), m_FgDel);
	RFX_Text(pFX, _T("[AdKana]"), m_AdKana);
	RFX_Text(pFX, _T("[IkkatuVal]"), m_IkkatuVal);
	RFX_Text(pFX, _T("[Invno]"), m_InvNo);		// インボイス登録番号追加対応_23/11/06 add
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CdbUcLstAddress 診断

#ifdef _DEBUG
void CdbUcLstAddress::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbUcLstAddress::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


/**********************************************************************
	Init()
		初期化処理（Open()の代わりとして使用）

		グループ番号のレコード取得
		【条件】
			指定されたグループ番号 ＆ 削除されていない行
		【並び順】
			OrderNum（並び順番号)

	引数
			なし
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstAddress::Init(int inGrSeq)
{
	return	RequeryGrSeq(inGrSeq);
}

/**********************************************************************
	Init()
		初期化処理（Open()の代わりとして使用）

		グループ番号のレコード取得
		【条件】
			削除されていない行
		【並び順】
			OrderNum（並び順番号)

	引数
			なし
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstAddress::Init()
{
	///////////////////////////////////////////////////////////////////
	// 目的のレコードを抽出する
	m_strFilter = "FgDel = 0";		// 削除フラグ OFF（0:有効なレコード)
	m_strSort   = "OrderNum";		// 並び順でソート

	return RequeryWork();		//成功
}

/**********************************************************************
	RequeryGrSeq()
		グループ番号のレコード取得
		【条件】
			指定されたグループ番号 ＆ 削除されていない行
		【並び順】
			OrderNum（並び順番号)

	引数
			int	inGrSeq			グループ番号
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstAddress::RequeryGrSeq(int inGrSeq)
{
	///////////////////////////////////////////////////////////////////
	// 目的のレコードを抽出する
	m_strFilter.Format( "GrSeq = %d AND FgDel = 0", inGrSeq );	// 指定シーケンス番号
	m_strSort   = "OrderNum";									// 並び順でソート

	return RequeryWork();		//成功
}

// midori 161109 add -->
/**********************************************************************
	Init()
		初期化処理（Open()の代わりとして使用）

		グループ番号のレコード取得
		【条件】
			指定されたグループ番号 ＆ 削除されていない行
		【並び順】
			OrderNum（並び順番号)

	引数
			int		nSort		表示順選択
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstAddress::Init_M(int inGrSeq, int nSort)
{
	return	RequeryGrSeq_M(inGrSeq, nSort);
}

/**********************************************************************
	Init()
		初期化処理（Open()の代わりとして使用）

		グループ番号のレコード取得
		【条件】
			削除されていない行
		【並び順】
			OrderNum（並び順番号)

	引数
			int		nSort		表示順選択
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstAddress::Init_M(int nSort)
{
	///////////////////////////////////////////////////////////////////
	// 目的のレコードを抽出する
	m_strFilter = "FgDel = 0";		// 削除フラグ OFF（0:有効なレコード)
	if(nSort == 0)	{
		m_strSort = "OrderNum";		// 並び順でソート
	}
	else	{
		m_strSort = "AdKana,OrderNum";		// カナ順でソート
	}

	return RequeryWork();		//成功
}

/**********************************************************************
	RequeryGrSeq()
		グループ番号のレコード取得
		【条件】
			指定されたグループ番号 ＆ 削除されていない行
		【並び順】
			OrderNum（並び順番号)

	引数
			int	inGrSeq			グループ番号
			int	nSort			表示順選択
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstAddress::RequeryGrSeq_M(int inGrSeq, int nSort)
{
	///////////////////////////////////////////////////////////////////
	// 目的のレコードを抽出する
	m_strFilter.Format( "GrSeq = %d AND FgDel = 0", inGrSeq );	// 指定シーケンス番号
	if(nSort == 0)	{
		m_strSort = "OrderNum";									// 並び順でソート
	}
	else	{
		m_strSort = "AdKana,OrderNum";							// カナ順でソート
	}

	return RequeryWork();		//成功
}
// midori 161109 add <--

/**********************************************************************
	RequeryGrSeq()
		グループ番号のレコード取得
		【条件】
			指定されたグループ番号 ＆ 指定された並び順番号
		【並び順】
			OrderNum（並び順番号)

	引数
			int	inGrSeq			グループ番号
			int inOrder			順序番号
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstAddress::RequeryAdOrder(int inGrSeq, int inOrder)
{
	///////////////////////////////////////////////////////////////////
	// 目的のレコードを抽出する
	m_strFilter.Format( "GrSeq = %d AND OrderNum = %d", 
											inGrSeq, inOrder );	// 指定シーケンス番号
	m_strSort   = "OrderNum";									// 並び順でソート

	return RequeryWork();		//成功
}

/**********************************************************************
	RequeryGrSeq()
		グループ番号のレコード取得
		【条件】
			指定されたグループ番号 ＆ 削除されていない行
		【並び順】
			OrderNum（並び順番号)

	引数
			int	inGrSeq			グループ番号
			int inAdSeq			シーケンス番号
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstAddress::RequeryAdSeq( int inGrSeq, int inAdSeq )
{
	///////////////////////////////////////////////////////////////////
	// 目的のレコードを抽出する
	m_strFilter.Format( "GrSeq = %d AND AdSeq = %d", 
											inGrSeq, inAdSeq );	// 指定シーケンス番号
	m_strSort   = "OrderNum";									// 並び順でソート

	return RequeryWork();		//成功
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
/*
int CdbUcLstAddress::RequerySeq( int nSeq )
{
	///////////////////////////////////////////////////////////////////
	// 目的のレコードを抽出する
	m_strFilter.Format( "Seq = %d", nSeq );		// 指定シーケンス番号
	m_strSort   = "OrderNum";					// 並び順でソート

	return RequeryWork();		//成功
}
*/

/**********************************************************************
	CreateNewRecord()
		新規レコードを作成、初期値を追加
		レコードセットには、新規レコードがセットされて戻ります

	引数
			int	inGrSeq			グループ番号
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstAddress::CreateNewRecord(int inGrSeq)
{
	CRecordset	rs(m_lo_pdb);
	CString		SqlStr;
	int			retVal = DB_ERR;

	/////////////////////////////////////////////////////////////////////
	//	暫定バージョンです
	/////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////
	// 新しいシーケンス番号を算出
	SqlStr =	_T("SELECT max(AdSeq) AS MaxNum ");
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

	TRACE("dbUcLstAddress AddNew\n");
	AddNew();

	m_AdSeq = retVal;			//?? 対応を検討する必要あり
	m_GrSeq = inGrSeq;
	m_AdName1 = _T("");
	m_AdName2 = _T("");
	m_AdAdd1 = _T("");
	m_AdAdd2 = _T("");
	m_FgDel = 0;
	m_AdKana = _T("");
	m_OrderNum = 0;
	m_IkkatuVal = GetIkkatuValAddress(inGrSeq);
	m_InvNo = _T("");		// インボイス登録番号追加対応_23/11/06 add

	Update();

	// 追加した行をレコードセットとして取得
	m_strFilter.Format( "AdSeq = %d AND GrSeq = %d", retVal, inGrSeq );
	m_strSort   = "";

	return RequeryWork();
}

/**********************************************************************
	ReNumberOrder()
		並び順番号を付け直す処理

	引数
			int		inGrSeq		グループ番号
			int		inOrderNum	開始地点の並び番号(現在の値)
			int		inIncDec	移動数
									1つ進める場合： 1
									2つ進める場合： 2
									1つ戻す場合  ：-1
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstAddress::ReNumberOrder(int inGrSeq, int inOrderNum, int inIncDec)
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
	SqlStrWhe.Format("WHERE GrSeq = %d AND OrderNum >= %d AND FgDel = 0", inGrSeq, inOrderNum);

	SqlStr = SqlStrUpd + SqlStrSet + SqlStrWhe;

	///////////////////////////////////////////////////////////////
	// SQL 実行

	return ExecuteSQLWork(SqlStr);
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
//int CdbUcLstAddress::UpdateDataTableAll(BOOL isRegAuto)
// midori 190504 del <--
// midori 190504 add -->
int CdbUcLstAddress::UpdateDataTableAll(BOOL isRegAuto,int nSgn)
// midori 190504 add <--
{
	int retVal = DB_ERR_OK;

// midori 190504 del -->
//	retVal = UpdateDataTable( ID_FORMNO_021, isRegAuto );
//
//	if (retVal == DB_ERR_OK) {
//		retVal = UpdateDataTable( ID_FORMNO_031, isRegAuto );
//	}
//
//	if (retVal == DB_ERR_OK) {
//		retVal = UpdateDataTable( ID_FORMNO_041, isRegAuto );
//	}
//
//	if (retVal == DB_ERR_OK) {
//		retVal = UpdateDataTable( ID_FORMNO_081, isRegAuto );
//	}
//
//	if (retVal == DB_ERR_OK) {
//		retVal = UpdateDataTable( ID_FORMNO_091, isRegAuto );
//	}
//
//	if (retVal == DB_ERR_OK) {
//		retVal = UpdateDataTable( ID_FORMNO_101, isRegAuto );
//	}
//
//	// ver1_0_2_1から以下の様式も更新
//// midori 190301 del -->
//	//if (retVal == DB_ERR_OK) 	retVal = UpdateDataTable( ID_FORMNO_042, isRegAuto );
//// midori 190301 del <--
//// midori 190301 add -->
//	if(retVal == DB_ERR_OK)	{
//		retVal = UpdateDataTable(ID_FORMNO_042,isRegAuto);
//// midori 9 del -->
//		//// CdbUcLstAddress から挿入した住所に、改行マークが入っていないので
//		//// ここで挿入する
//		//Data042Convert();
//// midori 9 del <--
//	}
//// midori 190301 add <--
//	if (retVal == DB_ERR_OK) 	retVal = UpdateDataTable( ID_FORMNO_161, isRegAuto );
//	if (retVal == DB_ERR_OK) 	retVal = UpdateDataTable( ID_FORMNO_162, isRegAuto );
// midori 190504 del <--
// midori 190504 add -->
	retVal = UpdateDataTable( ID_FORMNO_021, isRegAuto, nSgn );

	if (retVal == DB_ERR_OK) {
		retVal = UpdateDataTable( ID_FORMNO_031, isRegAuto, nSgn );
	}

	if (retVal == DB_ERR_OK) {
		retVal = UpdateDataTable( ID_FORMNO_041, isRegAuto, nSgn );
	}

	if (retVal == DB_ERR_OK) {
		retVal = UpdateDataTable( ID_FORMNO_081, isRegAuto, nSgn );
	}

	if (retVal == DB_ERR_OK) {
		retVal = UpdateDataTable( ID_FORMNO_091, isRegAuto, nSgn );
	}

	if (retVal == DB_ERR_OK) {
		retVal = UpdateDataTable( ID_FORMNO_101, isRegAuto, nSgn );
	}

	// ver1_0_2_1から以下の様式も更新
// midori 190301 del -->
	//if (retVal == DB_ERR_OK) 	retVal = UpdateDataTable( ID_FORMNO_042, isRegAuto );
// midori 190301 del <--
// midori 190301 add -->
	if(retVal == DB_ERR_OK)	{
		retVal = UpdateDataTable(ID_FORMNO_042,isRegAuto, nSgn);
// midori 9 del -->
		//// CdbUcLstAddress から挿入した住所に、改行マークが入っていないので
		//// ここで挿入する
		//Data042Convert();
// midori 9 del <--
	}
// midori 190301 add <--
	if (retVal == DB_ERR_OK) 	retVal = UpdateDataTable( ID_FORMNO_161, isRegAuto, nSgn );
	if (retVal == DB_ERR_OK) 	retVal = UpdateDataTable( ID_FORMNO_162, isRegAuto, nSgn );
// midori 190504 add <--

	return retVal;
}


/**********************************************************************
	UpdateDataTable()
		データテーブルのデータを名称リストで更新する
		（名称リストテーブルとデータテーブルの整合をとる）

	引数
			int		inFormSeq	変更先データテーブルのフォームシーケンス番号

	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
// midori 190504 del -->
//int CdbUcLstAddress::UpdateDataTable(int inFormSeq, BOOL isRegAuto)
// midori 190504 del <--
// midori 190504 add -->
int CdbUcLstAddress::UpdateDataTable(int inFormSeq, BOOL isRegAuto, int nSgn)
// midori 190504 add <--
{
	int retVal = DB_ERR_OK;

	switch (inFormSeq) {
	case ID_FORMNO_021:
		retVal = UpdateDataTableSub( 
						STR_TBL_NAME_021	//(売)名称のみ
						, _T("AdSeq")
						, _T("AdName1")
						, _T("AdName2")
						, _T("")			// 住所フィールドを持っていない
						, _T("")			// 住所フィールドを持っていない
						, _T("AdOrder")
// midori 152137 add -->
						, _T("AdKana")
// midori 152137 add <--
						, isRegAuto
// midori 190504 add -->
						, TRUE
						, ( (nSgn == 0) ? 0 : ID_ADDRESSGR_URI ) 
// midori 190504 add <--
						);
		break;

	case ID_FORMNO_031:
		retVal = UpdateDataTableSub( 
						STR_TBL_NAME_031	//(売)
						, _T("AdSeq")
						, _T("AdName1")
						, _T("AdName2")
						, _T("AdAdd1")
						, _T("AdAdd2")
						, _T("AdOrder")
// midori 152137 add -->
						, _T("AdKana")
// midori 152137 add <--
						, isRegAuto
// midori 190504 add -->
						, TRUE
						, ( (nSgn == 0) ? 0 : ID_ADDRESSGR_URI ) 
// midori 190504 add <--
						);
		break;

	case ID_FORMNO_041:
		retVal = UpdateDataTableSub( 
						STR_TBL_NAME_041	//(買)
						, _T("AdSeq")
						, _T("AdName1")
						, _T("AdName2")
						, _T("AdAdd1")
						, _T("AdAdd2")
						, _T("AdOrder")
// midori 152137 add -->
						, _T("AdKana")
// midori 152137 add <--
						, isRegAuto
// midori 190504 add -->
						, TRUE
						, ( (nSgn == 0) ? 0 : ID_ADDRESSGR_KAI ) 
// midori 190504 add <--
						);
		break;

	case ID_FORMNO_081:
		retVal = UpdateDataTableSub( 
						STR_TBL_NAME_081	//(買)名称のみ更新
						, _T("AdSeq")
						, _T("AdName1")
						, _T("AdName2")
						, _T("")			// 住所フィールドを持っていない
						, _T("")			// 住所フィールドを持っていない
						, _T("AdOrder")
// midori 152137 add -->
						, _T("AdKana")
// midori 152137 add <--
						, isRegAuto
// midori 190504 add -->
						, TRUE
						, ( (nSgn == 0) ? 0 : ID_ADDRESSGR_KAI ) 
// midori 190504 add <--
						);
		break;

	case ID_FORMNO_091:
		retVal = UpdateDataTableSub( 
						STR_TBL_NAME_091	//(買)
						, _T("AdSeq")
						, _T("AdName1")
						, _T("AdName2")
						, _T("AdAdd1")
						, _T("AdAdd2")
						, _T("AdOrder")
// midori 152137 add -->
						, _T("AdKana")
// midori 152137 add <--
						, isRegAuto
// midori 190504 add -->
						, TRUE
						, ( (nSgn == 0) ? 0 : ID_ADDRESSGR_KAI ) 
// midori 190504 add <--
						);
		break;

	case ID_FORMNO_101:
		retVal = UpdateDataTableSub( 
						STR_TBL_NAME_101	//(売)
						, _T("AdSeq")
						, _T("AdName1")
						, _T("AdName2")
						, _T("AdAdd1")
						, _T("AdAdd2")
						, _T("AdOrder")
// midori 152137 add -->
						, _T("AdKana")
// midori 152137 add <--
						, isRegAuto
// midori 190504 add -->
						, TRUE
						, ( (nSgn == 0) ? 0 : ID_ADDRESSGR_URI ) 
// midori 190504 add <--
						);
		break;

	case ID_FORMNO_042:
		retVal = UpdateDataTableSub( 
						STR_TBL_NAME_042
						, _T("AdSeq")
						, _T("AdName1")
						, _T("AdName2")
						, _T("AdAdd1")
						, _T("AdAdd2")
						, _T("AdOrder")
// midori 152137 add -->
						, _T("AdKana")
// midori 152137 add <--
						, isRegAuto
						, FALSE
// midori 190504 add -->
						, ( (nSgn == 0) ? 0 : ID_ADDRESSGR_URI )
// midori 190504 add <--
						);
// midori 12 del -->
//// midori 9 add -->
//		// CdbUcLstAddress から挿入した住所に、改行マークが入っていないので
//		// ここで挿入する
//		Data042Convert();
//// midori 9 add <--
// midori 12 del <--
// midori 12 add -->
		// CdbUcLstAddress から挿入した住所に、改行マークが入っていないので
		// ここで挿入する
// midori 190504 del -->
		//if(bG_Kanso == TRUE) {
// midori 190504 del <--
// midori 190504 add -->
		// 簡素化かつ、末尾スペース除外ではない場合のみ処理を行う
		if(bG_Kanso == TRUE && nSgn == 0) {
// midori 190504 add <--
			Data042Convert();
		}
// midori 12 add <--
		break;

	case ID_FORMNO_161:
		retVal = UpdateDataTableSub( 
						STR_TBL_NAME_161
						, _T("AdSeq")
						, _T("AdName1")
						, _T("AdName2")
						, _T("AdAdd1")
						, _T("AdAdd2")
						, _T("AdOrder")
// midori 152137 add -->
						, _T("AdKana")
// midori 152137 add <--
						, isRegAuto
						, FALSE
// midori 190504 add -->
						, ( (nSgn == 0) ? 0 : ID_ADDRESSGR_KAI )
// midori 190504 add <--
						);
		break;

	case ID_FORMNO_162:
		retVal = UpdateDataTableSub( 
						STR_TBL_NAME_162
						, _T("AdSeq")
						, _T("AdName1")
						, _T("AdName2")
						, _T("AdAdd1")
						, _T("AdAdd2")
						, _T("AdOrder")
// midori 152137 add -->
						, _T("AdKana")
// midori 152137 add <--
						, isRegAuto
						, FALSE
// midori 190504 add -->
						, ( (nSgn == 0) ? 0 : ID_ADDRESSGR_URI )
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

	(1) シーケンス番号が同じ項目(DataTabel.AdSeq = this.AdSeq)で更新
			取引先名(AdName1, AdName2)、
			住所    (AdAdd1,  AdAdd2)
			並び順  (AdOrder)
// midori 152137 add -->
			カナ    (AdKana)
// midori 152137 add <--

  	(2)	データテーブルの名称シーケンス番号が NULL であった場合
		データテーブルのデータを以下とする
			取引先シーケンス番号(AdSeq) = 0

	(3)	名称リストテーブルに存在しないシーケンス番号が、
		データテーブルに出現した場合
		データテーブルのデータを以下とする
			取引先名(AdName1) = ""
			取引先名(BkName2) = ""
			住所    (AdAdd1)  = ""
			住所    (AdAdd2)  = ""
			並び順  (AdOrder) = 0
			シーケンス(AdSeq) = 0
// midori 152137 add -->
			カナ    (AdKana) = ""
// midori 152137 add <--
		ただし、「取引先、金融機関を自動で登録する」OFFまたは④－２、⑯－１、⑯－２はAdSeq = 0の存在が許される為、
		uc_lst_addressに登録されていなくても（されていたらバグ）許可する


	※ 取引先名称は、⑰⑱のその他帳表では、使用していないため、
	   そのデータテーブルに対する FormSeq の対応を入れていない
	   が、、、
	   ⑰⑱のデータテーブルに対しても、FormSeq対応を入れなくとも
	   大丈夫ではないかと思われる。
	   テーブル内の全データに対して、UPDATEがかかるため。

	引数
			CString inTableName		変更先データテーブル名
			CString inNameAdSeq		データテーブルのフィールド名(取引先シーケンス番号)
			CString inNameAdName1	データテーブルのフィールド名(取引先名)
			CString inNameAdName2	データテーブルのフィールド名(取引先名)
			CString inNameAdAdd1	データテーブルのフィールド名(取引先住所)
			CString inNameAdAdd2	データテーブルのフィールド名(取引先住所)
			CString inNameAdOrder	データテーブルのフィールド名(取引先順序番号)
// midori 152137 add -->
			CString inNameAdKana	データテーブルのフィールド名(カナ)
// midori 152137 add <--
			BOOL	isDeleteAdSeqZero F9自動登録しない様式（④－２、⑯－１、⑯－２）は(3)を実行しない

	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstAddress::UpdateDataTableSub(
						  CString inTableName		// データテーブル名
						, CString inNameAdSeq		// フィールド名
						, CString inNameAdName1		// フィールド名
						, CString inNameAdName2		// フィールド名
						, CString inNameAdAdd1		// フィールド名
						, CString inNameAdAdd2		// フィールド名
						, CString inNameAdOrder		// フィールド名
// midori 152137 add -->
						, CString inNameAdKana		// データテーブルのフィールド名(カナ)
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

	///////////////////////////////////////////////////////////////
	//(1)
	///////////////////////////////////////////////////////////////
	// SQL 作成

// midori 190504 add -->
	// 起動時、末尾スペースチェック(DeleteSpaceMaster)から呼ばれた場合は処理を行わない
	if(nSgn == 0) {
// midori 190504 add <--
		SqlStr  = _T("UPDATE ") + inTableName + _T(" ");
		SqlStr += _T("SET ");
		if(inNameAdName1 !="") {
			SqlStr +=			inNameAdName1 + _T(" = lst.AdName1, ");
		}
		if(inNameAdName2 !="") {
			SqlStr +=			inNameAdName2 + _T(" = lst.AdName2, ");
		}
		if(inNameAdAdd1 !="") {
			SqlStr +=			inNameAdAdd1  + _T(" = lst.AdAdd1, ");
		}
		if(inNameAdAdd2 !="") {
			SqlStr +=			inNameAdAdd2  + _T(" = lst.AdAdd2, ");
		}
// midori 152137 add -->
		SqlStr += inNameAdKana + _T(" = lst.AdKana, ");
// midori 152137 add <--
		SqlStr +=			inNameAdOrder + _T(" = lst.OrderNum ");
		SqlStr += _T("FROM ") + inTableName + _T(" AS data ");
		SqlStr += _T("JOIN ") + m_lo_TableName + _T(" AS lst ");
		SqlStr += _T("ON data.") + inNameAdSeq + _T(" = lst.AdSeq");

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
	SqlStr += _T("SET ")   + inNameAdSeq + _T(" = 0 ");
	SqlStr += _T("WHERE ") + inNameAdSeq + _T(" IS NULL");


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
		SqlStr  = _T("UPDATE ") + inTableName + _T(" ");
		SqlStr += _T("SET ") + inNameAdSeq   + _T(" = 0, ");
		if (inNameAdName1 != "") {
			SqlStr +=			inNameAdName1 + _T(" = '', ");
		}
		if (inNameAdName2 != "") {
			SqlStr +=			inNameAdName2 + _T(" = '', ");
		}
		if (inNameAdAdd1 != "") {
			SqlStr +=			inNameAdAdd1  + _T(" = '', ");
		}
		if (inNameAdAdd1 != "") {
			SqlStr +=			inNameAdAdd2  + _T(" = '', ");
		}
// midori 152137 add -->
		SqlStr += inNameAdKana + _T(" = '', ");
// midori 152137 add <--
		SqlStr += inNameAdOrder + _T(" = 0 ");
		SqlStr += _T("WHERE FgFunc = 1 ");
// midori 161104 cor -->
	//	if((isDeleteAdSeqZero != FALSE) || isRegAuto){
	//		SqlStr += _T("AND ") + inNameAdSeq + _T(" <> 0 ");
	//	}
// ---------------------
		SqlStr += _T("AND ") + inNameAdSeq + _T(" <> 0 ");
// midori 161104 cor <--
		SqlStr += _T("AND ") + inNameAdSeq + _T(" NOT IN ( ");
		SqlStr += _T("SELECT lst.AdSeq ");
		SqlStr += _T("FROM ") + m_lo_TableName  + _T(" AS lst ");
		SqlStr += _T("WHERE FgDel = 0 ");
		SqlStr += _T(") ");

		///////////////////////////////////////////////////////////////
		// SQL 実行
		retVal3 = ExecuteSQLWork(SqlStr);
// midori 190504 add -->
	}
	else {
		CString sql;
		sql  = _T("UPDATE ") + inTableName + _T(" ");
		sql += _T("SET ")  + inNameAdSeq + _T(" = lst.AdSeq, ");
		// カナ入力のある様式はカナの登録を行う
		if(inNameAdKana.IsEmpty() == FALSE)	{
			sql += inNameAdKana + _T(" = lst.AdKana, ");
		}
		sql +=				inNameAdOrder + _T(" = lst.OrderNum ");
		sql += _T("FROM ") + inTableName + _T(" AS data ");
		sql += _T("JOIN ") + m_lo_TableName + _T(" AS lst ");
// 157270 del -->
		//sql += _T("ON isnull(data.") + inNameAdName1 + _T(",'') = isnull(lst.AdName1,'') ");
		//sql += _T("AND isnull(data.") + inNameAdName2 + _T(",'') = isnull(lst.AdName2,'') ");
// 157270 del <--
// 157270 add -->
		sql += _T("ON isnull(data.") + inNameAdName1 + _T(",'') collate Japanese_CS_AS_KS_WS = isnull(lst.AdName1,'') collate Japanese_CS_AS_KS_WS ");
		sql += _T("AND isnull(data.") + inNameAdName2 + _T(",'') collate Japanese_CS_AS_KS_WS = isnull(lst.AdName2,'') collate Japanese_CS_AS_KS_WS ");
// 157270 add <--
		// 所在地の比較
		if(inTableName == STR_TBL_NAME_042 && bG_Kanso == TRUE) {
			// ④-2の簡素化様式のみ
			// データ側には改行マークが、マスター側には上段の末尾に入力したスペースが
			// 入っている場合があるので、それぞれの文字を除外して比較する
// 157270 del -->
			//if(inNameAdAdd1.IsEmpty() == FALSE) {
			//	sql += _T("AND isnull(replace(data.") + inNameAdAdd1 + _T(",CHAR(13)+CHAR(10),''),'') = ");
			//	sql += _T("isnull(replace(lst.AdAdd1,' ',''),'') ");
			//}
			//if(inNameAdAdd2.IsEmpty() == FALSE) {
			//	sql += _T("AND isnull(replace(data.") + inNameAdAdd2 + _T(",CHAR(13)+CHAR(10),''),'') = ");
			//	sql += _T("isnull(replace(lst.AdAdd2,' ',''),'') ");
			//}
// 157270 del <--
// 157270 add -->
			if(inNameAdAdd1.IsEmpty() == FALSE) {
				sql += _T("AND isnull(replace(data.") + inNameAdAdd1 + _T(",CHAR(13)+CHAR(10),''),'') collate Japanese_CS_AS_KS_WS = ");
				sql += _T("isnull(replace(lst.AdAdd1,' ',''),'') collate Japanese_CS_AS_KS_WS ");
			}
			if(inNameAdAdd2.IsEmpty() == FALSE) {
				sql += _T("AND isnull(replace(data.") + inNameAdAdd2 + _T(",CHAR(13)+CHAR(10),''),'') collate Japanese_CS_AS_KS_WS = ");
				sql += _T("isnull(replace(lst.AdAdd2,' ',''),'') collate Japanese_CS_AS_KS_WS ");
			}
// 157270 add <--
		}
		else	{
// 157270 del -->
			//if(inNameAdAdd1.IsEmpty() == FALSE) {
			//	sql += _T("AND isnull(replace(data.") + inNameAdAdd1 + _T(",CHAR(13)+CHAR(10),''),'') = isnull(lst.AdAdd1,'') ");
			//}
			//if(inNameAdAdd2.IsEmpty() == FALSE) {
			//	sql += _T("AND isnull(replace(data.") + inNameAdAdd2 + _T(",CHAR(13)+CHAR(10),''),'') = isnull(lst.AdAdd2,'') ");
			//}
// 157270 del <--
// 157270 add -->
			if(inNameAdAdd1.IsEmpty() == FALSE) {
				sql += _T("AND isnull(replace(data.") + inNameAdAdd1 + _T(",CHAR(13)+CHAR(10),''),'') collate Japanese_CS_AS_KS_WS = isnull(lst.AdAdd1,'') collate Japanese_CS_AS_KS_WS ");
			}
			if(inNameAdAdd2.IsEmpty() == FALSE) {
				sql += _T("AND isnull(replace(data.") + inNameAdAdd2 + _T(",CHAR(13)+CHAR(10),''),'') collate Japanese_CS_AS_KS_WS = isnull(lst.AdAdd2,'') collate Japanese_CS_AS_KS_WS ");
			}
// 157270 add <--
		}
		sql.Format(sql + _T("AND lst.GrSeq = %d "),nSgn);

		retVal3 = ExecuteSQLWork(sql);
		//// ④-2は、この後の処理で改行マークが挿入されるので、入っていれば除外しておく
		//if(retVal3 == DB_ERR_OK) {
		//	if(inTableName == STR_TBL_NAME_042 && bG_Kanso == TRUE) {
		//		sql  = _T("UPDATE ") + inTableName + _T(" ");
		//		sql += _T("SET ")  + inNameAdAdd1 + _T(" = replace(") + inNameAdAdd1;
		//		sql += _T(",CHAR(13)+CHAR(10),'')");
		//		sql += _T(", ")  + inNameAdAdd2 + _T(" = replace(") + inNameAdAdd2;
		//		sql += _T(",CHAR(13)+CHAR(10),'')");
		//		retVal3 = ExecuteSQLWork(sql);
		//	}
		//}
	}
// midori 190504 add <--

	if (retVal1 != DB_ERR_OK) {
		return retVal1;
	}
	if (retVal2 != DB_ERR_OK) {
		return retVal2;
	}
	return retVal3;
}

/**********************************************************************
	AutoNumOrder()
		並び順番号を自動採番
		現在登録されている OrderNum の最大値 + 1 した値を自動採番
		引数 inSeq で指定されたレコードの OrderNum に格納

	引数
			int		inAdSeq		シーケンス番号(AdSeq)
			int		inGrSeq		グループ番号  (GrSeq)
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstAddress::AutoNumOrder(int inAdSeq, int inGrSeq)
{
	CRecordset	rs(m_lo_pdb);
	CString		SqlStr;
	CString		SqlStrUpd;
	CString		SqlStrSet1;
	CString		SqlStrSet2;
	CString		SqlStrSet3;
	CString		SqlStrSet4;
	CString		SqlStrSet5;
	CString		SqlStrWhe1;

	///////////////////////////////////////////////////////////////
	// SQL 作成

	SqlStrUpd   = _T("UPDATE ") + m_lo_TableName + _T(" ");
	SqlStrSet1	= _T("SET OrderNum = ( ");
	SqlStrSet2	=     _T("SELECT TOP 1 (lst.OrderNum + 1) ");
	SqlStrSet3	=     _T("FROM ") + m_lo_TableName + _T(" AS lst ");
	SqlStrSet4.Format(   "WHERE GrSeq = %d ", inGrSeq);
	SqlStrSet5	=     _T("ORDER BY lst.OrderNum DESC ) ");
	SqlStrWhe1.Format("WHERE AdSeq = %d", inAdSeq);

	SqlStr	= SqlStrUpd  + SqlStrSet1 + SqlStrSet2 + SqlStrSet3 + SqlStrSet4 + SqlStrSet5
			+ SqlStrWhe1;

	///////////////////////////////////////////////////////////////
	// SQL 実行

	return ExecuteSQLWork(SqlStr);
}

/**********************************************************************
	CheckEntry()
		名称リスト内に指定されたエントリが登録されているかを調べる
		あれば、そのシーケンス番号(Seq)を返す
		なければ、新規レコードとして登録し、そのシーケンス番号(Seq)を返す

		成功した場合、抽出したレコードセットは、該当レコードとなっている。

	引数
			CString inAdName1		取引先名称１ (1行目)
			CString inAdName2		取引先名称２ (2行目)
			CString inAdAdd1		取引先住所１ (1行目)
			CString inAdAdd2		取引先住所２ (2行目)
			int		inGrSeq			グループ番号 (GrSeq)
			BOOL	isRegFurigana	カナ登録するかどうか TRUE:する/FALSE:しない

	戻値
			int					正の数(>0)		：シーケンス番号
								0				：登録しなかった(NULL文字列のため)
								上記以外(<0)	：失敗
***********************************************************************/
int CdbUcLstAddress::CheckEntry	( CString	inAdName1
								, CString	inAdName2
								, CString	inAdAdd1
								, CString	inAdAdd2
								, int		inGrSeq
								, BOOL	    isRegFurigana
)
{
	int			retVal = DB_ERR;

	////////////////////////////////////////////////////////////////
	// 指定された名称、住所が、NULL 文字列の場合
	//   登録せずに、処理を抜ける
	if (	(inAdName1 == "") && (inAdName2 == "")
		 &&	(inAdAdd1  == "") && (inAdAdd2  == "")	)
	{
		return	0;
	}

	///////////////////////////////////////////////////////////////
	// 抽出条件作成（第1ステップ)  名称＋住所 でのマッチング
	//	 重複確認
	retVal = CheckRepeat( inAdName1, inAdName2, inAdAdd1, inAdAdd2, inGrSeq );
	
	if (retVal != DB_ERR_OK) {
		return retVal;
	}

	retVal = 0;
	if (!IsEOF()) {			// 該当するレコードがあった
		retVal = m_AdSeq;		// 該当レコードのシーケンス番号を返す
		if (m_FgDel != 0) {		// 削除フラグの値が 有効行以外になっていたら
			Edit();
			m_FgDel = 0;				// 有効行に戻す
			Update();
			AutoNumOrder(m_AdSeq, inGrSeq);		// (自動採番) Orderに並び順をつける
		}

		// OrderNum 値が、自動採番されたので、レコードを再読込
		RequeryAdSeq( inGrSeq, retVal );
		return retVal;
	}

	///////////////////////////////////////////////////////////////
	// 抽出条件作成（第2ステップ)  名称 のみでのマッチング
	//   第1ステップでマッチングしなかった場合
	return CheckEntryAddNone(inAdName1, inAdName2, inAdAdd1, inAdAdd2, inGrSeq, TRUE, isRegFurigana);
}

/**********************************************************************
	CheckEntryAddNone()
		名称リスト内に指定されたエントリが登録されているかを調べる
		あれば、そのシーケンス番号(Seq)を返す
		なければ、新規レコードとして登録し、そのシーケンス番号(Seq)を返す

		成功した場合、抽出したレコードセットは、該当レコードとなっている。

	引数
			CString inAdName1	取引先名称１ (1行目)
			CString inAdName2	取引先名称２ (2行目)
			CString inAdAdd1	取引先住所１ (1行目)
			CString inAdAdd2	取引先住所２ (2行目)
			int		inGrSeq		グループ番号 (GrSeq)
			BOOL	fAddress	住所項目の有無
			BOOL	isRegFurigana	カナ登録するかどうか TRUE:する/FALSE:しない

	戻値
			int					正の数(>0)		：シーケンス番号
								0				：登録しなかった(NULL文字列のため)
								上記以外(<0)	：失敗
***********************************************************************/
int CdbUcLstAddress::CheckEntryAddNone	( CString	inAdName1
										, CString	inAdName2
										, CString	inAdAdd1
										, CString	inAdAdd2
										, int		inGrSeq
										, BOOL		fAddress
										, BOOL		isRegFurigana
)
{
	int			retVal = DB_ERR;
	CString		StrTmp;

	////////////////////////////////////////////////////////////////
	// 指定された名称が、NULL 文字列の場合
	//   登録せずに、処理を抜ける
	if ((fAddress == FALSE) && (inAdName1 == "") && (inAdName2 == ""))	{
		return	0;
	}

	///////////////////////////////////////////////////////////////
// 修正No.162362 del -->
	// 抽出条件作成（第1ステップ)  名称 のみでのマッチング
	//   住所の有無により処理が異なる
	if (retVal <= 0) {

		//	重複確認（※住所は対象外）
		retVal = CheckRepeat( inAdName1, inAdName2, "", "", inGrSeq, fAddress );
		
		if (retVal != DB_ERR_OK) {
			return retVal;
		}

		retVal = 0;
		if (!IsEOF()) {			// 該当するレコードがあった
			retVal = m_AdSeq;		// 該当レコードのシーケンス番号を返す

			if (fAddress == TRUE) {	// 住所ありの場合、住所を更新
				Edit();
				m_AdAdd1   = inAdAdd1;
				m_AdAdd2   = inAdAdd2;
				Update();
			}
			if (m_FgDel != 0) {		// 削除フラグの値が 有効行以外になっていたら
				Edit();
				m_FgDel = 0;				// 有効行に戻す
				Update();
				AutoNumOrder(m_AdSeq, inGrSeq);		// (自動採番) Orderに並び順をつける
			}
		}
	}
// 修正No.162362 del <--

	///////////////////////////////////////////////////////////////
	// (第3ステップ)
	// 名称リストに登録されていないので、
	// 新規レコードを作成し、格納する
	// OrderNum は、現在の最大値+1の値にする
	if (retVal <= 0) {
		int	ret = CreateNewRecord(inGrSeq);
		if (ret == DB_ERR_OK) {
			Edit();
			m_AdName1  = inAdName1;
			m_AdName2  = inAdName2;
			m_AdAdd1   = inAdAdd1;
			m_AdAdd2   = inAdAdd2;
			m_GrSeq    = inGrSeq;

			if(isRegFurigana){
				CFuriganaConverter fc;
				m_AdKana = fc.Convert(m_AdName1);
			}

			m_IkkatuVal = GetIkkatuValAddress(inGrSeq);

			m_FgDel    = 0;
			m_OrderNum = 0;
			Update();
			AutoNumOrder(m_AdSeq, inGrSeq);		//自動採番
			retVal = m_AdSeq;
		}
	}

	// OrderNum 値が、自動採番されたので、レコードを再読込
	//RequerySeq( retVal );
	RequeryAdSeq( inGrSeq, retVal );
	return retVal;

}

// midori 152137 add -->
/**********************************************************************
	CheckEntryRen()
		名称リスト内に指定されたエントリが登録されているかを調べる
		あれば、そのシーケンス番号(Seq)を返す
		なければ、新規レコードとして登録し、そのシーケンス番号(Seq)を返す

		成功した場合、抽出したレコードセットは、該当レコードとなっている。

	引数
			CString inAdName1		取引先名称１ (1行目)
			CString inAdName2		取引先名称２ (2行目)
			CString inAdAdd1		取引先住所１ (1行目)
			CString inAdAdd2		取引先住所２ (2行目)
			CString	inAdKana		取引先カナ
			int		inGrSeq			グループ番号 (GrSeq)
			BOOL	isRegFurigana	カナ登録するかどうか TRUE:する/FALSE:しない

	戻値
			int					正の数(>0)		：シーケンス番号
								0				：登録しなかった(NULL文字列のため)
								上記以外(<0)	：失敗
***********************************************************************/
int CdbUcLstAddress::CheckEntryRen	( CString	inAdName1
									, CString	inAdName2
									, CString	inAdAdd1
									, CString	inAdAdd2
									, CString	inAdKana
									, int		inGrSeq
									, BOOL	    isRegFurigana
)
{
	int			retVal = DB_ERR;

	////////////////////////////////////////////////////////////////
	// 指定された名称、住所が、NULL 文字列の場合
	//   登録せずに、処理を抜ける
	if (	(inAdName1 == "") && (inAdName2 == "")
		 &&	(inAdAdd1  == "") && (inAdAdd2  == "")	)
	{
		return	0;
	}

	///////////////////////////////////////////////////////////////
	// 抽出条件作成（第1ステップ)  名称＋住所 でのマッチング
	//	 重複確認
	retVal = CheckRepeat( inAdName1, inAdName2, inAdAdd1, inAdAdd2, inGrSeq );
	
	if (retVal != DB_ERR_OK) {
		return retVal;
	}

	retVal = 0;
	if (!IsEOF()) {			// 該当するレコードがあった
		retVal = m_AdSeq;		// 該当レコードのシーケンス番号を返す
		if (m_FgDel != 0) {		// 削除フラグの値が 有効行以外になっていたら
			Edit();
			m_FgDel = 0;				// 有効行に戻す
			Update();
			AutoNumOrder(m_AdSeq, inGrSeq);		// (自動採番) Orderに並び順をつける
		}

		// OrderNum 値が、自動採番されたので、レコードを再読込
		RequeryAdSeq( inGrSeq, retVal );
		return retVal;
	}

	///////////////////////////////////////////////////////////////
	// 抽出条件作成（第2ステップ)  名称 のみでのマッチング
	//   第1ステップでマッチングしなかった場合
	return CheckEntryAddNoneRen(inAdName1, inAdName2, inAdAdd1, inAdAdd2, inAdKana, inGrSeq, TRUE, isRegFurigana);
}

/**********************************************************************
	CheckEntryAddNoneRen(財務連動用)
		名称リスト内に指定されたエントリが登録されているかを調べる
		あれば、そのシーケンス番号(Seq)を返す
		なければ、新規レコードとして登録し、そのシーケンス番号(Seq)を返す

		成功した場合、抽出したレコードセットは、該当レコードとなっている。

	引数
			CString inAdName1	取引先名称１ (1行目)
			CString inAdName2	取引先名称２ (2行目)
			CString inAdAdd1	取引先住所１ (1行目)
			CString inAdAdd2	取引先住所２ (2行目)
			CString	inAdKana	取引先カナ
			int		inGrSeq		グループ番号 (GrSeq)
			BOOL	fAddress	住所項目の有無
			BOOL	isRegFurigana	カナ登録するかどうか TRUE:する/FALSE:しない

	戻値
			int					正の数(>0)		：シーケンス番号
								0				：登録しなかった(NULL文字列のため)
								上記以外(<0)	：失敗
***********************************************************************/
int CdbUcLstAddress::CheckEntryAddNoneRen	( CString	inAdName1
											, CString	inAdName2
											, CString	inAdAdd1
											, CString	inAdAdd2
											, CString	inAdKana
											, int		inGrSeq
											, BOOL		fAddress
											, BOOL		isRegFurigana
)
{
	int			retVal = DB_ERR;
	CString		StrTmp;

	////////////////////////////////////////////////////////////////
	// 指定された名称が、NULL 文字列の場合
	//   登録せずに、処理を抜ける
	if ((fAddress == FALSE) && (inAdName1 == "") && (inAdName2 == ""))	{
		return	0;
	}

	///////////////////////////////////////////////////////////////
	// 抽出条件作成（第1ステップ)  名称 のみでのマッチング
	//   住所の有無により処理が異なる
	if (retVal <= 0) {

		//	重複確認（※住所は対象外）
		retVal = CheckRepeat( inAdName1, inAdName2, "", "", inGrSeq, fAddress );
		
		if (retVal != DB_ERR_OK) {
			return retVal;
		}

		retVal = 0;
		if (!IsEOF()) {			// 該当するレコードがあった
			retVal = m_AdSeq;		// 該当レコードのシーケンス番号を返す

			if (fAddress == TRUE) {	// 住所ありの場合、住所を更新
				Edit();
				m_AdAdd1   = inAdAdd1;
				m_AdAdd2   = inAdAdd2;
				Update();
			}
			if (m_FgDel != 0) {		// 削除フラグの値が 有効行以外になっていたら
				Edit();
				m_FgDel = 0;				// 有効行に戻す
				Update();
				AutoNumOrder(m_AdSeq, inGrSeq);		// (自動採番) Orderに並び順をつける
			}
		}
	}

	///////////////////////////////////////////////////////////////
	// (第3ステップ)
	// 名称リストに登録されていないので、
	// 新規レコードを作成し、格納する
	// OrderNum は、現在の最大値+1の値にする
	if (retVal <= 0) {
		int	ret = CreateNewRecord(inGrSeq);
		if (ret == DB_ERR_OK) {
			Edit();
			m_AdName1  = inAdName1;
			m_AdName2  = inAdName2;
			m_AdAdd1   = inAdAdd1;
			m_AdAdd2   = inAdAdd2;
			m_GrSeq    = inGrSeq;

			if(isRegFurigana){
				CFuriganaConverter fc;
				if(inAdKana.IsEmpty() == FALSE)	m_AdKana = inAdKana;
				else							m_AdKana = fc.Convert(m_AdName1);
			}

			m_IkkatuVal = GetIkkatuValAddress(inGrSeq);

			m_FgDel    = 0;
			m_OrderNum = 0;
			Update();
			AutoNumOrder(m_AdSeq, inGrSeq);		//自動採番
			retVal = m_AdSeq;
		}
	}

	// OrderNum 値が、自動採番されたので、レコードを再読込
	//RequerySeq( retVal );
	RequeryAdSeq( inGrSeq, retVal );
	return retVal;

}
// midori 152137 add <--


/**********************************************************************
	CheckRepeat()
		名称リスト内に指定されたエントリが登録されているかを調べる
		
	引数
			CString inAdName1	取引先名称１ (1行目)
			CString inAdName2	取引先名称２ (2行目)
			CString inAdAdd1	取引先住所１ (1行目)
			CString inAdAdd2	取引先住所２ (2行目)
			CString inAdKana	取引先名称ｶﾅ１
			int		inGrSeq		グループ番号 (GrSeq)
			BOOL	fAddress	住所重複確認フラグ（TRUE：対象／FALSE：対象外）

	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstAddress::CheckRepeat ( CString	inAdName1
								 , CString	inAdName2
								 , CString	inAdAdd1
								 , CString	inAdAdd2
								 , int		inGrSeq
								 , BOOL		fAddress /*=TRUE*/
								 , CString	inAdKana/*=NULL*/
)
{
	int			retVal = DB_ERR;
	CString		StrTmp[4];
	
	////////////////////////////////////////////////////////////////
	// 名称テーブルの名称、住所フィールドが NULL のものを
	// NULL文字列('')に変更する
	retVal = UpdateStrFieldAllForNull();

	if (retVal != DB_ERR_OK) {
		return retVal;
	}
	
	//	初期化
	for(int i=0; i<4; i++){
		StrTmp[i].Empty();
	}

	// SQLコマンドでは、名称に[']が含まれている場合['']とする
	CString		strAdName1 = ConvSqlString(inAdName1);
	CString		strAdName2 = ConvSqlString(inAdName2);
	CString		strAdAdd1 = ConvSqlString(inAdAdd1);
	CString		strAdAdd2 = ConvSqlString(inAdAdd2);
	CString		strAdKana = ConvSqlString(inAdKana);

	///////////////////////////////////////////////////////////////
	// 抽出条件作成（第1ステップ)  名称、住所の両方でのマッチング
	StrTmp[2].Format("GrSeq = %d", inGrSeq);

//++++++++++++++++++++++++++++++++++++++++++++++++++
//	COLLATE Japanese_CS_AS_KS_WS について補足
//		CS	…	大文字、小文字を区別する。全角アルファベットの大文字、小文字も区別する 
//		AS	…	アクセント、濁音、破裂音を区別する
//		KS	…	ひらがなとカタカナを区別する。半角でも区別する 
//		WS	…	文字幅を区別する（全角、半角を区別する）
//++++++++++++++++++++++++++++++++++++++++++++++++++
	StrTmp[0].Format( "((AdName1 = '%s' COLLATE Japanese_CS_AS_KS_WS) AND (AdName2 = '%s' COLLATE Japanese_CS_AS_KS_WS)) ", strAdName1, strAdName2 );
	
	//	住所も重複確認対象？
	if ( fAddress == TRUE ){
		StrTmp[1].Format( "AND ((AdAdd1 = '%s' COLLATE Japanese_CS_AS_KS_WS) AND (AdAdd2 = '%s' COLLATE Japanese_CS_AS_KS_WS)) ", strAdAdd1, strAdAdd2 );		
	}

	//　ｶﾅ文字が存在する場合は重複チェック条件に含める
	if( !(inAdKana.IsEmpty()) ){
		StrTmp[3].Format( "AND (AdKana = '%s' COLLATE Japanese_CS_AS_KS_WS) ", strAdKana);
	}

	m_strFilter =	StrTmp[0] + StrTmp[1] + _T("AND ") + StrTmp[2] + _T(" ") + StrTmp[3] + _T(" ");
	m_strSort   =	_T("OrderNum");									// 並び順でソート

	retVal = RequeryWork();				// リクエリ

	return retVal;
}

/**********************************************************************
	UpdateStrFieldForNull()

	名称(AdName1, AdName2)、
	住所フィールド(AdAdd1, AdAdd2) が NULL になっているものを、
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
int CdbUcLstAddress::UpdateStrFieldAllForNull()
{
	int ret;
	
	ret = UpdateStrFieldForNull("AdName1");

	if (ret == DB_ERR_OK) {
		ret = UpdateStrFieldForNull("AdName2");
	}
	if (ret == DB_ERR_OK) {
		ret = UpdateStrFieldForNull("AdAdd1");
	}
	if (ret == DB_ERR_OK) {
		ret = UpdateStrFieldForNull("AdAdd2");
	}
// 改良No.22-0404,22-0812 add -->
	if (ret == DB_ERR_OK) {
		ret = UpdateStrFieldForNull("AdKana");
	}
// 改良No.22-0404,22-0812 add <--
// インボイス登録番号追加対応_23/11/06 add -->
	if (ret == DB_ERR_OK) {
		ret = UpdateStrFieldForNull("Invno");
	}
// インボイス登録番号追加対応_23/11/06 add <--

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
int CdbUcLstAddress::UpdateStrFieldForNull(CString inFieldName)
{
	CString		SqlStr;

	SqlStr  = _T("UPDATE ") + m_lo_TableName + _T(" ");
	SqlStr += _T("SET ") + inFieldName + _T(" = '' ");
	SqlStr += _T("WHERE ") + inFieldName + _T(" IS NULL ");

	return ExecuteSQLWork(SqlStr);
}

/**********************************************************************
		F9から参照はするが登録はしない場合、取引先名、住所からAdSeq,AdOrderを取得する
		
	引数
			adname1		取引先名称１
			adname2		取引先名称２
			adadd1		住所１
			adadd2		住所２

	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstAddress::RequeryAdName(int inGrSeq, CString adname1, CString adname2, CString adadd1, CString adadd2)
{
// midori 160602 cor -->
	//CString strGrSeq;
	//strGrSeq.Format("%d", inGrSeq);
	//m_strFilter = "GrSeq = " + strGrSeq + " AND AdName1 = '"+ adname1 +"' AND AdName2 = '"+ adname2 +"' AND ";
	//m_strFilter+= "AdAdd1 = '"+ adadd1 +"' AND AdAdd2 = '"+ adadd2 +"' ";
	//m_strSort   = "";
// ---------------------
	CString	strGrSeq=_T("");
	CString	strAdName1=_T("");
	CString	strAdName2=_T("");
	CString	strAdAdd1=_T("");
	CString	strAdAdd2=_T("");

	// シングルクォーテーションを置き換える
	// (')	 → ('')
	// ('')	 → ('''')
	// (''') → (''''')
	strAdName1=adname1;
	strAdName1.Replace(_T("'"),_T("''"));
	strAdName2=adname2;
	strAdName2.Replace(_T("'"),_T("''"));
	strAdAdd1=adadd1;
	strAdAdd1.Replace(_T("'"),_T("''"));
	strAdAdd2=adadd2;
	strAdAdd2.Replace(_T("'"),_T("''"));

	strGrSeq.Format("%d", inGrSeq);
// 157270 del -->
	//m_strFilter = "GrSeq = " + strGrSeq + " AND AdName1 = '"+ strAdName1 +"' AND AdName2 = '"+ strAdName2 +"' AND ";
	//m_strFilter+= "AdAdd1 = '"+ strAdAdd1 +"' AND AdAdd2 = '"+ strAdAdd2 +"' ";
// 157270 del <--
// 157270 add -->
	m_strFilter = "GrSeq = " + strGrSeq + " AND AdName1 collate Japanese_CS_AS_KS_WS = '"+ strAdName1 +"' AND AdName2 collate Japanese_CS_AS_KS_WS = '"+ strAdName2 +"' AND ";
	m_strFilter+= "AdAdd1 collate Japanese_CS_AS_KS_WS = '"+ strAdAdd1 +"' AND AdAdd2 collate Japanese_CS_AS_KS_WS  = '"+ strAdAdd2 +"' ";
// 157270 add <--
	m_strSort   = "";
// midori 160602 cor <--

	return RequeryWork();
}

/**********************************************************************
		F9から参照はするが登録はしない場合、取引先名のみからAdSeq,AdOrderを取得する
		もし取引先が同じで異なる所在地がある場合、オーダーが小さいほうを取得する
		
	引数
			adname1		取引先名称１
			adname2		取引先名称２

	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstAddress::RequeryAdName(int inGrSeq, CString adname1, CString adname2)
{
// midori 160602 cor -->
	//CString strGrSeq;
	//strGrSeq.Format("%d", inGrSeq);
	//m_strFilter = "GrSeq = " + strGrSeq + " AND AdName1 = '"+ adname1 +"' AND AdName2 = '"+ adname2 +"' ";
	//m_strSort   = "OrderNum";	// 同名の取引先名称が存在する為
// ---------------------
	CString strGrSeq=_T("");
	CString	strAdName1=_T("");
	CString	strAdName2=_T("");

	// シングルクォーテーションを置き換える
	// (')	 → ('')
	// ('')	 → ('''')
	// (''') → (''''')
	strAdName1=adname1;
	strAdName1.Replace(_T("'"),_T("''"));
	strAdName2=adname2;
	strAdName2.Replace(_T("'"),_T("''"));

	strGrSeq.Format("%d", inGrSeq);
// 157270 del -->
	//m_strFilter = "GrSeq = " + strGrSeq + " AND AdName1 = '"+ strAdName1 +"' AND AdName2 = '"+ strAdName2 +"' ";
// 157270 del <--
// 157270 add -->
	m_strFilter = "GrSeq = " + strGrSeq + " AND AdName1 collate Japanese_CS_AS_KS_WS = '"+ strAdName1 +"' AND AdName2 collate Japanese_CS_AS_KS_WS = '"+ strAdName2 +"' ";
// 157270 add <--
	m_strSort   = "OrderNum";	// 同名の取引先名称が存在する為
// midori 160602 cor <--

	return RequeryWork();
}

/**********************************************************************
		取引先名称、所在地から登録されていればその取引先のシーケンス、
		されていなければ0を返す
		所在地が空白でも、""をキーとしてリクエリする
		
	引数
			adname1		取引先名称１
			adname2		取引先名称２
			adadd1		取引先所在地１
			adadd2		取引先所在地２

	戻値
			int		登録されている：Seq
					登録されていない：0
					エラー：-1
***********************************************************************/
int CdbUcLstAddress::GetSeqFromAdName(int inGrSeq, CString adname1, CString adname2, CString adadd1, CString adadd2)
{
	// 名称テーブルの名称、住所フィールドが NULL のものを
	// NULL文字列('')に変更する
	int ret = UpdateStrFieldAllForNull();
	if(ret != DB_ERR_OK)	return -1;	// エラー

	ret = RequeryAdName(inGrSeq, adname1, adname2, adadd1, adadd2);
	
	if(ret != DB_ERR_OK)	return -1;	// エラー
	if(this->IsEOF())		return 0;	// 登録されていない
	return this->m_AdSeq;				// 登録済み
}

/**********************************************************************
		取引先名称のみから登録されていればその取引先のシーケンス、
		されていなければ0を返す
		所在地を条件に含めない為、オーバーロード
		
	引数
			adname1		取引先名称１
			adname2		取引先名称２
			adadd1		取引先所在地１
			adadd2		取引先所在地２

	戻値
			int		登録されている：Seq
					登録されていない：0
					エラー：-1
***********************************************************************/
int CdbUcLstAddress::GetSeqFromAdName(int inGrSeq, CString adname1, CString adname2)
{
	// 名称テーブルの名称、住所フィールドが NULL のものを
	// NULL文字列('')に変更する
	int ret = UpdateStrFieldAllForNull();
	if(ret != DB_ERR_OK)	return -1;	// エラー

	ret = RequeryAdName(inGrSeq, adname1, adname2);
	
	if(ret != DB_ERR_OK)	return -1;	// エラー
	if(this->IsEOF())		return 0;	// 登録されていない
	return this->m_AdSeq;				// 登録済み
}

int CdbUcLstAddress::SetIkkatuVal(long seq, const CString& val)
{
	if(m_IkkatuVal == val)	return 1;
	if(seq != m_AdSeq)		return 1;	// 念の為取引先SEQで一致を確認

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

void CdbUcLstAddress::GetIkkatuNeedVal(long& seq, int& order)
{
	seq		= m_AdSeq;
	order	= m_OrderNum;
}

void CdbUcLstAddress::GetIkkatuNeedVal(long& seq, int& order, CString& val, CString& name)
{
	seq		= m_AdSeq;
	order	= m_OrderNum;
	val		= m_IkkatuVal;
	name	= m_AdName1 + m_AdName2;
}

CString CdbUcLstAddress::GetIkkatuValAddress(int GrSeq)
{
	if(!(m_IkkatuVal.IsEmpty()))	return m_IkkatuVal;

	if(GrSeq == ID_ADDRESSGR_KAI)			return _T("1000000");
	else if(GrSeq == ID_ADDRESSGR_URI)		return _T("500000");

	return m_IkkatuVal;
}

// midori 190301 add -->
// CdbUcLstAddress からセットした住所に、改行マークを挿入する
void CdbUcLstAddress::Data042Convert( void )
{
	CdbUc042Kasituke	db042(m_lo_pdb);
	int					ii=0;
	int					sw=0;
	CString				cs=_T("");
	CString				cs1=_T("");
	CString				cs2=_T("");
	CString				cs3=_T("");
	CUcFunctionCommon	ufc;

	db042.Open();
	while(!db042.IsEOF()){		// ファイル終端まで
		// 2.所在地(住所)
		for(ii=0; ii<2; ii++)	{
			sw=0;
			if(ii == 0)	cs = db042.m_AdAdd1;
			else		cs = db042.m_AdAdd2;
// midori 190504 del -->
			//if(cs.IsEmpty() == FALSE)	{
// midori 190504 del <--
// midori 190504 add -->
			if(cs.IsEmpty() == FALSE && db042.m_AdOrder != 0)	{
// midori 190504 add <--
				// 全角１０文字を超えていれば改行マークを挿入して取り込む
				if(cs.GetLength() > 20)	{
					cs2 = ufc.GetSpritString(cs,&cs1,20);
					cs = cs2;
					cs3 = ufc.GetSpritString(cs,&cs2,20);
					cs = cs1 + PRT_FORMAT_CHCODE + cs2;
					sw = 1;
				}
			}
			if(sw != 0)	{
				db042.Edit();
				if(ii == 0)	db042.m_AdAdd1 = cs;
				else		db042.m_AdAdd2 = cs;
				db042.Update();
			}
		}
		db042.MoveNext();
	}
	db042.Close();
}
// midori 190301 add <--

// midori 155301 del -->
//// midori 190504 add -->
//// ----------------------------------------------------------------------------------------------------
////	RequeryAdName()
////		
////	グループ番号、取引先名、取引先場所、取引先カナ順にレコードを抽出する
////
////	引数
////			int					グループ番号			3:得意先		10:仕入先
////	戻値
////			int					DB_ERR_OK (0)     ：成功
////								DB_ERR_OK (0) 以外：失敗
//// ----------------------------------------------------------------------------------------------------
//int CdbUcLstAddress::RequeryAdName(int GrSeq)
//{
//
//	// 目的のレコードを抽出する
//	m_strFilter.Format(_T("FgDel = 0 and GrSeq = %d"),GrSeq);			// 削除フラグ OFF（0:有効なレコード)、グループ番号
//	m_strSort   = "GrSeq,AdName1,AdName2,AdAdd1,AdAdd2,AdKana";		// グループ番号、取引先名、取引先場所、取引先カナ順にソート
//
//	return RequeryWork();		//成功
//}
//// midori 190504 add <--
// midori 155301 del <--
// midori 155301 add -->
// ----------------------------------------------------------------------------------------------------
//	RequeryAdName()
//		
//	グループ番号、取引先名、取引先場所、取引先カナ順にレコードを抽出する
//
//	引数
//			int					グループ番号			3:得意先		10:仕入先
//			int					オーダーサイン
//	戻値
//			int					DB_ERR_OK (0)     ：成功
//								DB_ERR_OK (0) 以外：失敗
// ----------------------------------------------------------------------------------------------------
int CdbUcLstAddress::RequeryAdName(int GrSeq, int pSw)
{

	// 目的のレコードを抽出する
	m_strFilter.Format(_T("FgDel = 0 and GrSeq = %d"),GrSeq);			// 削除フラグ OFF（0:有効なレコード)、グループ番号
	if(pSw == 0)	{
		m_strSort   = "GrSeq,AdName1,AdName2,AdAdd1,AdAdd2,AdKana,OrderNum";	// グループ番号、取引先名、取引先場所、取引先カナ、オーダーNo順にソート
	}
	else	{
		m_strSort   = "GrSeq,OrderNum,AdName1,AdName2,AdAdd1,AdAdd2,AdKana";	// グループ番号、オーダーNo、取引先名、取引先場所、取引先カナ順にソート
	}

	return RequeryWork();		//成功
}
// midori 155301 add <--

// インボイス登録番号追加対応_23/11/06 add -->
// ----------------------------------------------------------------------------------------------------
//	RequeryAdInfo()
//		
//	AdSeqをキーにして取引先の情報を取得
//
//	引数	int					グループ番号			3:得意先		10:仕入先
//			int					ApSeq
//	戻値	int					DB_ERR_OK (0)     ：成功
//								DB_ERR_OK (0) 以外：失敗
// ----------------------------------------------------------------------------------------------------
int CdbUcLstAddress::RequeryAdInfo(int pGrSeq, int pAdSeq)
{
	int			rt1,rt2 = 0;

	// 目的のレコードを抽出する
	m_strFilter.Format(_T("FgDel = 0 and GrSeq = %d and AdSeq = %d"), pGrSeq, pAdSeq);			// 削除フラグ OFF（0:有効なレコード)
	m_strSort = "OrderNum";	// オーダーNo順にソート

	rt1 = RequeryWork();

	rt2 = 0;
	if(rt1 != DB_ERR_OK)	rt2 = -1;	// エラー
	if(this->IsEOF())		rt2 = 1;	// 登録されていない
	
	return(rt2);
}

// ----------------------------------------------------------------------------------------------------
//	↓インボイス登録番号追加版用↓
// ----------------------------------------------------------------------------------------------------
// 修正No.168402,168405 add -->
/**********************************************************************
	CheckAdSeqEntry_Inv()
		シーケンス番号をキーに名称リスト内に指定されたエントリが登録されているかを調べる
		あれば、そのシーケンス番号(Seq)を返す
		なければ、新規レコードとして登録し、そのシーケンス番号(Seq)を返す

		成功した場合、抽出したレコードセットは、該当レコードとなっている。

	引数	int		inAdSeq			シーケンス番号(Seq)
			CString inAdName1		取引先名称１ (1行目)
			CString inAdName2		取引先名称２ (2行目)
			CString inAdAdd1		取引先住所１ (1行目)
			CString inAdAdd2		取引先住所２ (2行目)
			CString inInvNo			登録番号
			int		inGrSeq			グループ番号 (GrSeq)
			BOOL	fAddress
			BOOL	isRegFurigana	カナ登録するかどうか TRUE:する/FALSE:しない
			int		isFlag

	戻値
			int					正の数(>0)		：シーケンス番号
								0				：登録しなかった(NULL文字列のため)
								上記以外(<0)	：失敗
***********************************************************************/
int CdbUcLstAddress::CheckAdSeqEntry_Inv( int		inAdSeq
										, CString	inAdName1
										, CString	inAdName2
										, CString	inAdAdd1
										, CString	inAdAdd2
										, CString	inInvNo
										, int		inGrSeq
										, BOOL		fAddress
										, BOOL		isRegFurigana
										, int		isFlag
)
{
	int			retVal = DB_ERR;
	CString		cst = _T("");
	int			ret;

	/////////////////////////////////////////////////////////////////
	// 指定された名称、住所が、NULL 文字列の場合
	//   登録せずに、処理を抜ける
	if((inAdSeq == 0) && (inAdName1 == "") && (inAdName2 == "") && (inAdAdd1 == "") && (inAdAdd2 == "") && (inInvNo == "")) {
		return	0;
	}

	/////////////////////////////////////////////////////////////////
	// 抽出条件作成（第1ステップ)  シーケンス番号と名称 でのマッチング
	//	 重複確認
	//retVal = CheckRepeat_Inv2(inAdSeq, inGrSeq);											// 修正No.168526 del
	//retVal = CheckRepeat_Inv2(inAdSeq, inAdName1, inAdName2, inGrSeq);					// 修正No.168526 add  修正No.168534,168539 del
	retVal = CheckRepeat_Inv2(inAdSeq, inAdName1, inAdName2, inAdAdd1, inAdAdd2, inGrSeq);	// 修正No.168534,168539 add
	if(retVal != DB_ERR_OK) {
		return retVal;
	}

	retVal = 0;
	//if(!IsEOF()) {													// 該当するレコードがあった							// 修正No.168448 del
	if(!IsEOF() && (m_InvNo == "" || m_InvNo.IsEmpty() == TRUE))	{	// 該当するレコードがあった 且つ 登録番号が未登録	// 修正No.168448 add
		retVal = m_AdSeq;		// 該当レコードのシーケンス番号を返す

		// m_AdSeqをキーに登録番号を上書きする
		Edit();
		m_InvNo = inInvNo;
		Update();

		if(m_FgDel != 0) {		// 削除フラグの値が 有効行以外になっていたら
			Edit();
			m_FgDel = 0;				// 有効行に戻す
			Update();
			AutoNumOrder(m_AdSeq, inGrSeq);		// (自動採番) Orderに並び順をつける
		}

		// OrderNum 値が、自動採番されたので、レコードを再読込
		RequeryAdSeq(inGrSeq, retVal);
		return retVal;
	}

	/////////////////////////////////////////////////////////////////
	// 抽出条件作成（第4ステップ)  名称 のみでのマッチング
	//   上記のステップでマッチングしなかった場合
	if(isFlag == 0) {
		ret = CheckEntry_Inv(inAdName1, inAdName2, inAdAdd1, inAdAdd2, inInvNo, inGrSeq, isRegFurigana);
	}
	else {
		ret = CheckEntryAddNone_Inv(inAdName1, inAdName2, inAdAdd1, inAdAdd2, inInvNo, inGrSeq, fAddress, isRegFurigana);
	}

	return ret;
}
// 修正No.168402,168405 add <--

/**********************************************************************
	CheckEntry_Inv()
		名称リスト内に指定されたエントリが登録されているかを調べる
		あれば、そのシーケンス番号(Seq)を返す
		なければ、新規レコードとして登録し、そのシーケンス番号(Seq)を返す

		成功した場合、抽出したレコードセットは、該当レコードとなっている。

	引数
			CString inAdName1		取引先名称１ (1行目)
			CString inAdName2		取引先名称２ (2行目)
			CString inAdAdd1		取引先住所１ (1行目)
			CString inAdAdd2		取引先住所２ (2行目)
			CString inInvNo			登録番号
			int		inGrSeq			グループ番号 (GrSeq)
			BOOL	isRegFurigana	カナ登録するかどうか TRUE:する/FALSE:しない

	戻値
			int					正の数(>0)		：シーケンス番号
								0				：登録しなかった(NULL文字列のため)
								上記以外(<0)	：失敗
***********************************************************************/
int CdbUcLstAddress::CheckEntry_Inv(  CString	inAdName1
									, CString	inAdName2
									, CString	inAdAdd1
									, CString	inAdAdd2
									, CString	inInvNo
									, int		inGrSeq
									, BOOL		isRegFurigana
)
{
	int			retVal = DB_ERR;
	CString		cst = _T("");

	/////////////////////////////////////////////////////////////////
	// 指定された名称、住所が、NULL 文字列の場合
	//   登録せずに、処理を抜ける
	if((inAdName1 == "") && (inAdName2 == "") && (inAdAdd1 == "") && (inAdAdd2 == "") && (inInvNo == "")) {
		return	0;
	}

	/////////////////////////////////////////////////////////////////
	// 抽出条件作成（第1ステップ)  名称＋住所 でのマッチング
	//	 重複確認
	if(inInvNo.IsEmpty() == FALSE)	cst = inInvNo;
	else							cst = _T("-1");
	retVal = CheckRepeat_Inv(inAdName1, inAdName2, inAdAdd1, inAdAdd2, cst, inGrSeq);
	if(retVal != DB_ERR_OK) {
		return retVal;
	}

	retVal = 0;
	if(!IsEOF()) {			// 該当するレコードがあった
		retVal = m_AdSeq;		// 該当レコードのシーケンス番号を返す

		if(m_FgDel != 0) {		// 削除フラグの値が 有効行以外になっていたら
			Edit();
			m_FgDel = 0;				// 有効行に戻す
			Update();
			AutoNumOrder(m_AdSeq, inGrSeq);		// (自動採番) Orderに並び順をつける
		}

		// OrderNum 値が、自動採番されたので、レコードを再読込
		RequeryAdSeq(inGrSeq, retVal);
		return retVal;
	}

	/////////////////////////////////////////////////////////////////
	// 登録番号が入力されている場合
	if(inInvNo.IsEmpty() == FALSE) {
		// 抽出条件作成（第2ステップ) 登録番号無し でマッチング
		//   上記のステップでマッチングしなかった場合、登録番号の有無で該当するかチェックする
		retVal = CheckRepeat_Inv(inAdName1, inAdName2, inAdAdd1, inAdAdd2, "-1", inGrSeq);
		if(retVal != DB_ERR_OK) {
			return retVal;
		}
		retVal = 0;
		if(!IsEOF()) {			// 該当するレコードがあった
			retVal = m_AdSeq;		// 該当レコードのシーケンス番号を返す

			// m_AdSeqをキーに登録番号を上書きする
			Edit();
			m_InvNo = inInvNo;
			Update();

			if(m_FgDel != 0) {		// 削除フラグの値が 有効行以外になっていたら
				Edit();
				m_FgDel = 0;				// 有効行に戻す
				Update();
				AutoNumOrder(m_AdSeq, inGrSeq);		// (自動採番) Orderに並び順をつける
			}

			// OrderNum 値が、自動採番されたので、レコードを再読込
			RequeryAdSeq(inGrSeq, retVal);
			return retVal;
		}

// 修正No.168479 add -->
		// 抽出条件作成（第3ステップ) 違う登録番号 でマッチング
		retVal = CheckRepeat_Inv3(inAdName1, inAdName2, inAdAdd1, inAdAdd2, inInvNo, inGrSeq);
		if(retVal != DB_ERR_OK) {
			return retVal;
		}
		retVal = 0;
		if(!IsEOF()) {			// 該当するレコードがあった
			/////////////////////////////////////////////////////////////////
			// 新規に登録する
			// OrderNum は、現在の最大値+1の値にする
			int	ret = CreateNewRecord(inGrSeq);
			if(ret == DB_ERR_OK) {
				Edit();
				m_AdName1 = inAdName1;
				m_AdName2 = inAdName2;
				m_AdAdd1  = inAdAdd1;
				m_AdAdd2  = inAdAdd2;
				m_InvNo   = inInvNo;
				m_GrSeq   = inGrSeq;

				if(isRegFurigana) {
					CFuriganaConverter fc;
					m_AdKana = fc.Convert(m_AdName1);
				}

				m_IkkatuVal = GetIkkatuValAddress(inGrSeq);

				m_FgDel = 0;
				m_OrderNum = 0;
				Update();
				AutoNumOrder(m_AdSeq, inGrSeq);		//自動採番
				retVal = m_AdSeq;
			}

			// OrderNum 値が、自動採番されたので、レコードを再読込
			RequeryAdSeq(inGrSeq, retVal);

			return retVal;
		}
// 修正No.168479 add <--
	}
// 修正No.168479 add -->
	// 登録番号が入力されていない場合
	else {
		// 抽出条件作成（第3ステップ) 違う登録番号 でマッチング
		retVal = CheckRepeat_Inv3(inAdName1, inAdName2, inAdAdd1, inAdAdd2, inInvNo, inGrSeq);
		if(retVal != DB_ERR_OK) {
			return retVal;
		}
		retVal = 0;
		if(!IsEOF()) {			// 該当するレコードがあった
			retVal = m_AdSeq;		// 該当レコードのシーケンス番号を返す

			if(m_FgDel != 0) {		// 削除フラグの値が 有効行以外になっていたら
				Edit();
				m_FgDel = 0;				// 有効行に戻す
				Update();
				AutoNumOrder(m_AdSeq, inGrSeq);		// (自動採番) Orderに並び順をつける
			}

			// OrderNum 値が、自動採番されたので、レコードを再読込
			RequeryAdSeq(inGrSeq, retVal);
			return retVal;
		}
	}
// 修正No.168479 add <--

	/////////////////////////////////////////////////////////////////
	// 抽出条件作成（第4ステップ)  名称 のみでのマッチング
	//   上記のステップでマッチングしなかった場合
	return CheckEntryAddNone_Inv(inAdName1, inAdName2, inAdAdd1, inAdAdd2, inInvNo, inGrSeq, TRUE, isRegFurigana);
}

/**********************************************************************
	CheckEntryAddNone_Inv()
		名称リスト内に指定されたエントリが登録されているかを調べる
		あれば、そのシーケンス番号(Seq)を返す
		なければ、新規レコードとして登録し、そのシーケンス番号(Seq)を返す

		成功した場合、抽出したレコードセットは、該当レコードとなっている。

	引数
			CString inAdName1	取引先名称１ (1行目)
			CString inAdName2	取引先名称２ (2行目)
			CString inAdAdd1	取引先住所１ (1行目)
			CString inAdAdd2	取引先住所２ (2行目)
			CString inInvNo		登録番号
			int		inGrSeq		グループ番号 (GrSeq)
			BOOL	fAddress	住所項目の有無
			BOOL	isRegFurigana	カナ登録するかどうか TRUE:する/FALSE:しない

	戻値
			int					正の数(>0)		：シーケンス番号
								0				：登録しなかった(NULL文字列のため)
								上記以外(<0)	：失敗
***********************************************************************/
int CdbUcLstAddress::CheckEntryAddNone_Inv(CString	inAdName1
										   , CString	inAdName2
										   , CString	inAdAdd1
										   , CString	inAdAdd2
										   , CString	inInvNo
										   , int		inGrSeq
										   , BOOL		fAddress
										   , BOOL		isRegFurigana
)
{
	int			retVal = DB_ERR;
	CString		StrTmp;
	CString		cst;				// 修正No.168448 add

	/////////////////////////////////////////////////////////////////
	// 指定された名称が、NULL 文字列の場合
	//   登録せずに、処理を抜ける
	if((fAddress == FALSE) && (inAdName1 == "") && (inAdName2 == "") && (inInvNo == "")) {
		return	0;
	}

	/////////////////////////////////////////////////////////////////
	// 抽出条件作成（第1ステップ)  名称 のみでのマッチング
	//   住所の有無により処理が異なる
	if(retVal <= 0) {
		//	重複確認（※住所は対象外）
// 修正No.168448 del -->
		//retVal = CheckRepeat_Inv(inAdName1, inAdName2, "", "", inInvNo, inGrSeq, fAddress);
// 修正No.168448 del <--
// 修正No.168448 add -->
		cst.Empty();
		if(inInvNo.IsEmpty() == FALSE)	cst = inInvNo;
		else							cst = _T("-1");
		retVal = CheckRepeat_Inv(inAdName1, inAdName2, "", "", cst, inGrSeq, fAddress);
// 修正No.168448 add <--

		if(retVal != DB_ERR_OK) {
			return retVal;
		}

		retVal = 0;
		if(!IsEOF()) {			// 該当するレコードがあった
			retVal = m_AdSeq;		// 該当レコードのシーケンス番号を返す

			if(fAddress == TRUE) {	// 住所ありの場合、住所を更新
				Edit();
				m_AdAdd1 = inAdAdd1;
				m_AdAdd2 = inAdAdd2;
				Update();
			}
			if(m_FgDel != 0) {		// 削除フラグの値が 有効行以外になっていたら
				Edit();
				m_FgDel = 0;				// 有効行に戻す
				Update();
				AutoNumOrder(m_AdSeq, inGrSeq);		// (自動採番) Orderに並び順をつける
			}

			// OrderNum 値が、自動採番されたので、レコードを再読込
			RequeryAdSeq(inGrSeq, retVal);
			return retVal;
		}
	}

	///////////////////////////////////////////////////////////////
	// 登録番号が入力されている場合
	if(inInvNo.IsEmpty() == FALSE) {
		// 抽出条件作成（第2ステップ)  番号番号 のみ（名称、住所空欄）でのマッチング
		//  上記のステップでマッチングしなかった場合、登録番号の有無で該当するかチェックする
		retVal = CheckRepeat_Inv("", "", "", "", inInvNo, inGrSeq);

		if(retVal != DB_ERR_OK) {
			return retVal;
		}
		retVal = 0;
		if(!IsEOF()) {			// 該当するレコードがあった
			retVal = m_AdSeq;		// 該当レコードのシーケンス番号を返す

			// m_AdSeqをキーに名称、住所を上書きする
			Edit();
			m_AdName1 = inAdName1;
			m_AdName2 = inAdName2;
			if(fAddress == TRUE) {	// 住所ありの場合、住所を更新
				m_AdAdd1 = inAdAdd1;
				m_AdAdd2 = inAdAdd2;
			}
			if(isRegFurigana) {
				CFuriganaConverter fc;
				m_AdKana = fc.Convert(m_AdName1);
			}
			Update();

			if(m_FgDel != 0) {		// 削除フラグの値が 有効行以外になっていたら
				Edit();
				m_FgDel = 0;				// 有効行に戻す
				Update();
				AutoNumOrder(m_AdSeq, inGrSeq);		// (自動採番) Orderに並び順をつける
			}

			// OrderNum 値が、自動採番されたので、レコードを再読込
			RequeryAdSeq(inGrSeq, retVal);
			return retVal;
		}

// 修正No.168534,168539 del -->
//// 修正No.168526 add -->
//		// 抽出条件作成（第3ステップ)  番号番号 のみでのマッチング
//		//  上記のステップでマッチングしなかった場合、登録番号の有無で該当するかチェックする
//		retVal = CheckRepeat_Inv("-1", "-1", "", "", inInvNo, inGrSeq, FALSE);
//
//		if(retVal != DB_ERR_OK) {
//			return retVal;
//		}
//		retVal = 0;
//		if(!IsEOF()) {			// 該当するレコードがあった
//			retVal = m_AdSeq;		// 該当レコードのシーケンス番号を返す
//
//			// m_AdSeqをキーに名称、住所を上書きする
//			Edit();
//			if(m_AdName1.IsEmpty() == TRUE)		m_AdName1 = inAdName1;
//			if(m_AdName2.IsEmpty() == TRUE)		m_AdName2 = inAdName2;
//			if(fAddress == TRUE) {	// 住所ありの場合、住所を更新
//				if(m_AdAdd1.IsEmpty() == TRUE)	m_AdAdd1 = inAdAdd1;
//				if(m_AdAdd2.IsEmpty() == TRUE)	m_AdAdd2 = inAdAdd2;
//			}
//			if(isRegFurigana) {
//				CFuriganaConverter fc;
//				m_AdKana = fc.Convert(m_AdName1);
//			}
//			Update();
//
//			if(m_FgDel != 0) {		// 削除フラグの値が 有効行以外になっていたら
//				Edit();
//				m_FgDel = 0;				// 有効行に戻す
//				Update();
//				AutoNumOrder(m_AdSeq, inGrSeq);		// (自動採番) Orderに並び順をつける
//			}
//
//			// OrderNum 値が、自動採番されたので、レコードを再読込
//			RequeryAdSeq(inGrSeq, retVal);
//			return retVal;
//		}
//// 修正No.168526 add <--
// 修正No.168534,168539 del <--
	}
// 修正No.168524 add -->
	else	{
		// 登録番号が無かった場合、名称のみでマッチング
		retVal = CheckRepeat_Inv(inAdName1, inAdName2, "", "", "", inGrSeq, fAddress);

		if(retVal != DB_ERR_OK) {
			return retVal;
		}

		retVal = 0;
		if(!IsEOF()) {			// 該当するレコードがあった
			retVal = m_AdSeq;		// 該当レコードのシーケンス番号を返す

			if(fAddress == TRUE) {	// 住所ありの場合、住所を更新
				Edit();
				m_AdAdd1 = inAdAdd1;
				m_AdAdd2 = inAdAdd2;
				Update();
			}
			if(m_FgDel != 0) {		// 削除フラグの値が 有効行以外になっていたら
				Edit();
				m_FgDel = 0;				// 有効行に戻す
				Update();
				AutoNumOrder(m_AdSeq, inGrSeq);		// (自動採番) Orderに並び順をつける
			}

			// OrderNum 値が、自動採番されたので、レコードを再読込
			RequeryAdSeq(inGrSeq, retVal);
			return retVal;
		}
	}
// 修正No.168524 add <--

	/////////////////////////////////////////////////////////////////
	// (第4ステップ)
	// 名称リストに登録されていないので、
	// 新規レコードを作成し、格納する
	// OrderNum は、現在の最大値+1の値にする
	int	ret = CreateNewRecord(inGrSeq);
	if(ret == DB_ERR_OK) {
		Edit();
		m_AdName1 = inAdName1;
		m_AdName2 = inAdName2;
		m_AdAdd1  = inAdAdd1;
		m_AdAdd2  = inAdAdd2;
		m_InvNo   = inInvNo;
		m_GrSeq   = inGrSeq;

		if(isRegFurigana) {
			CFuriganaConverter fc;
			m_AdKana = fc.Convert(m_AdName1);
		}

		m_IkkatuVal = GetIkkatuValAddress(inGrSeq);

		m_FgDel = 0;
		m_OrderNum = 0;
		Update();
		AutoNumOrder(m_AdSeq, inGrSeq);		//自動採番
		retVal = m_AdSeq;
	}

	// OrderNum 値が、自動採番されたので、レコードを再読込
	RequeryAdSeq(inGrSeq, retVal);

	return retVal;
}

/**********************************************************************
	CheckEntryRen_Inv()
		名称リスト内に指定されたエントリが登録されているかを調べる
		あれば、そのシーケンス番号(Seq)を返す
		なければ、新規レコードとして登録し、そのシーケンス番号(Seq)を返す

		成功した場合、抽出したレコードセットは、該当レコードとなっている。

	引数
			CString inAdName1		取引先名称１ (1行目)
			CString inAdName2		取引先名称２ (2行目)
			CString inAdAdd1		取引先住所１ (1行目)
			CString inAdAdd2		取引先住所２ (2行目)
			CString	inAdKana		取引先カナ
			CString	inInvNo			登録番号
			int		inGrSeq			グループ番号 (GrSeq)
			BOOL	isRegFurigana	カナ登録するかどうか TRUE:する/FALSE:しない

	戻値
			int					正の数(>0)		：シーケンス番号
								0				：登録しなかった(NULL文字列のため)
								上記以外(<0)	：失敗
***********************************************************************/
int CdbUcLstAddress::CheckEntryRen_Inv(CString	inAdName1
									   , CString	inAdName2
									   , CString	inAdAdd1
									   , CString	inAdAdd2
									   , CString	inAdKana
									   , CString	inInvNo
									   , int		inGrSeq
									   , BOOL		isRegFurigana
)
{
	int			retVal = DB_ERR;
	CString		cst = _T("");

	////////////////////////////////////////////////////////////////
	// 指定された名称、住所が、NULL 文字列の場合
	//   登録せずに、処理を抜ける
	if((inAdName1 == "") && (inAdName2 == "") && (inAdAdd1 == "") && (inAdAdd2 == "") && (inInvNo == "")) {
		return	0;
	}

	///////////////////////////////////////////////////////////////
	// 抽出条件作成（第1ステップ)  名称＋住所 でのマッチング
	//	 重複確認
	if(inInvNo.IsEmpty() == FALSE)	cst = inInvNo;
	else							cst = _T("-1");
	retVal = CheckRepeat_Inv(inAdName1, inAdName2, inAdAdd1, inAdAdd2, cst, inGrSeq);
	if(retVal != DB_ERR_OK) {
		return retVal;
	}

	retVal = 0;
	if(!IsEOF()) {			// 該当するレコードがあった
		retVal = m_AdSeq;		// 該当レコードのシーケンス番号を返す
		if(m_FgDel != 0) {		// 削除フラグの値が 有効行以外になっていたら
			Edit();
			m_FgDel = 0;				// 有効行に戻す
			Update();
			AutoNumOrder(m_AdSeq, inGrSeq);		// (自動採番) Orderに並び順をつける
		}

		// OrderNum 値が、自動採番されたので、レコードを再読込
		RequeryAdSeq(inGrSeq, retVal);
		return retVal;
	}

	// 抽出条件作成（第2ステップ)  登録番号が入力されている場合、登録番号無し でマッチング
	//   上記のステップでマッチングしなかった場合、登録番号の有無で該当するかチェックする
	if(inInvNo.IsEmpty() == FALSE)	{
		retVal = CheckRepeat_Inv(inAdName1, inAdName2, inAdAdd1, inAdAdd2, "-1", inGrSeq);
		if(retVal != DB_ERR_OK) {
			return retVal;
		}
		retVal = 0;
		if(!IsEOF()) {			// 該当するレコードがあった
			retVal = m_AdSeq;		// 該当レコードのシーケンス番号を返す

			// m_AdSeqをキーに登録番号を上書きする
			Edit();
			m_InvNo = inInvNo;
			Update();

			if(m_FgDel != 0) {		// 削除フラグの値が 有効行以外になっていたら
				Edit();
				m_FgDel = 0;				// 有効行に戻す
				Update();
				AutoNumOrder(m_AdSeq, inGrSeq);		// (自動採番) Orderに並び順をつける
			}

			// OrderNum 値が、自動採番されたので、レコードを再読込
			RequeryAdSeq(inGrSeq, retVal);
			return retVal;
		}
	}

	///////////////////////////////////////////////////////////////
	// 抽出条件作成（第3ステップ)  番号番号 のみでのマッチング
	//  上記のステップでマッチングしなかった場合、登録番号の有無で該当するかチェックする
// 231225_168498 cor -->
	//retVal = CheckRepeat_Inv("", "", "", "", inInvNo, inGrSeq);
// ---------------------
	if(inInvNo.IsEmpty() == FALSE)	cst = inInvNo;
	else							cst = _T("-1");
	retVal = CheckRepeat_Inv("", "", "", "", cst, inGrSeq);
// 231225_168498 cor <--
	if(retVal != DB_ERR_OK) {
		return retVal;
	}
	retVal = 0;
	if(!IsEOF()) {			// 該当するレコードがあった
		retVal = m_AdSeq;		// 該当レコードのシーケンス番号を返す

		// m_AdSeqをキーに名称、住所を上書きする
		Edit();
		m_AdName1  = inAdName1;
		m_AdName2  = inAdName2;
		m_AdAdd1   = inAdAdd1;
		m_AdAdd2   = inAdAdd2;
		if(isRegFurigana) {
			CFuriganaConverter fc;
			m_AdKana = fc.Convert(m_AdName1);
		}
		Update();

		if(m_FgDel != 0) {		// 削除フラグの値が 有効行以外になっていたら
			Edit();
			m_FgDel = 0;				// 有効行に戻す
			Update();
			AutoNumOrder(m_AdSeq, inGrSeq);		// (自動採番) Orderに並び順をつける
		}

		// OrderNum 値が、自動採番されたので、レコードを再読込
		RequeryAdSeq(inGrSeq, retVal);
		return retVal;
	}

	///////////////////////////////////////////////////////////////
	// 抽出条件作成（第4ステップ)  名称 のみでのマッチング
	//	上記のステップでマッチングしなかった場合
	return CheckEntryAddNoneRen_Inv(inAdName1, inAdName2, inAdAdd1, inAdAdd2, inAdKana, inInvNo, inGrSeq, TRUE, isRegFurigana);
}

/**********************************************************************
	CheckEntryAddNoneRen_Inv(財務連動用)
		名称リスト内に指定されたエントリが登録されているかを調べる
		あれば、そのシーケンス番号(Seq)を返す
		なければ、新規レコードとして登録し、そのシーケンス番号(Seq)を返す

		成功した場合、抽出したレコードセットは、該当レコードとなっている。

	引数
			CString inAdName1	取引先名称１ (1行目)
			CString inAdName2	取引先名称２ (2行目)
			CString inAdAdd1	取引先住所１ (1行目)
			CString inAdAdd2	取引先住所２ (2行目)
			CString	inAdKana	取引先カナ
			CString	inInvNo		登録番号
			int		inGrSeq		グループ番号 (GrSeq)
			BOOL	fAddress	住所項目の有無
			BOOL	isRegFurigana	カナ登録するかどうか TRUE:する/FALSE:しない

	戻値
			int					正の数(>0)		：シーケンス番号
								0				：登録しなかった(NULL文字列のため)
								上記以外(<0)	：失敗
***********************************************************************/
int CdbUcLstAddress::CheckEntryAddNoneRen_Inv(CString	inAdName1
											, CString	inAdName2
											, CString	inAdAdd1
											, CString	inAdAdd2
											, CString	inAdKana
											, CString	inInvNo
											, int		inGrSeq
											, BOOL		fAddress
											, BOOL		isRegFurigana
)
{
	int			retVal = DB_ERR;
	CString		StrTmp;

	////////////////////////////////////////////////////////////////
	// 指定された名称が、NULL 文字列の場合
	//   登録せずに、処理を抜ける
	if((fAddress == FALSE) && (inAdName1 == "") && (inAdName2 == "") && (inInvNo == "")) {
		return	0;
	}

	///////////////////////////////////////////////////////////////
	// 抽出条件作成（第1ステップ)  名称 のみでのマッチング
	//   住所の有無により処理が異なる
	if(retVal <= 0) {
		//	重複確認（※住所は対象外）
// 23/12/22_168476,168482 cor -->
		//retVal = CheckRepeat_Inv(inAdName1, inAdName2, "", "", inInvNo, inGrSeq, fAddress);
// -----------------------
		// 財務連動時、連動元に登録番号が入力されていなければ
		// 登録番号＋名称のレコードと紐づけせずに、名称のみの
		// レコードを作成する
		if(inInvNo.IsEmpty() == FALSE)	StrTmp = inInvNo;
		else							StrTmp = _T("-1");
		retVal = CheckRepeat_Inv(inAdName1, inAdName2, "", "", StrTmp, inGrSeq, fAddress);
// 23/12/22_168476,168482 cor <--

		if(retVal != DB_ERR_OK) {
			return retVal;
		}

		retVal = 0;
		if(!IsEOF()) {			// 該当するレコードがあった
			retVal = m_AdSeq;		// 該当レコードのシーケンス番号を返す

			if(fAddress == TRUE) {	// 住所ありの場合、住所を更新
				Edit();
				m_AdAdd1 = inAdAdd1;
				m_AdAdd2 = inAdAdd2;
				Update();
			}
			if(m_FgDel != 0) {		// 削除フラグの値が 有効行以外になっていたら
				Edit();
				m_FgDel = 0;				// 有効行に戻す
				Update();
				AutoNumOrder(m_AdSeq, inGrSeq);		// (自動採番) Orderに並び順をつける
			}

			// OrderNum 値が、自動採番されたので、レコードを再読込
			RequeryAdSeq(inGrSeq, retVal);
			return retVal;
		}
	}

	///////////////////////////////////////////////////////////////
	// 抽出条件作成（第2ステップ)  番号番号 のみでのマッチング
	//  上記のステップでマッチングしなかった場合、登録番号の有無で該当するかチェックする
	if(inInvNo.IsEmpty() == FALSE) {
		retVal = CheckRepeat_Inv("", "", "", "", inInvNo, inGrSeq);
		if(retVal != DB_ERR_OK) {
			return retVal;
		}
		retVal = 0;
		if(!IsEOF()) {			// 該当するレコードがあった
			retVal = m_AdSeq;		// 該当レコードのシーケンス番号を返す

			// m_AdSeqをキーに名称、住所を上書きする
			Edit();
			m_AdName1 = inAdName1;
			m_AdName2 = inAdName2;
			if(fAddress == TRUE) {	// 住所ありの場合、住所を更新
				m_AdAdd1 = inAdAdd1;
				m_AdAdd2 = inAdAdd2;
			}
			if(isRegFurigana) {
				CFuriganaConverter fc;
				m_AdKana = fc.Convert(m_AdName1);
			}
			Update();

			if(m_FgDel != 0) {		// 削除フラグの値が 有効行以外になっていたら
				Edit();
				m_FgDel = 0;				// 有効行に戻す
				Update();
				AutoNumOrder(m_AdSeq, inGrSeq);		// (自動採番) Orderに並び順をつける
			}

			// OrderNum 値が、自動採番されたので、レコードを再読込
			RequeryAdSeq(inGrSeq, retVal);
			return retVal;
		}
	}

	///////////////////////////////////////////////////////////////
	// (第3ステップ)
	// 名称リストに登録されていないので、
	// 新規レコードを作成し、格納する
	// OrderNum は、現在の最大値+1の値にする
	int	ret = CreateNewRecord(inGrSeq);
	if(ret == DB_ERR_OK) {
		Edit();
		m_AdName1 = inAdName1;
		m_AdName2 = inAdName2;
		m_AdAdd1 = inAdAdd1;
		m_AdAdd2 = inAdAdd2;
		m_InvNo = inInvNo;
		m_GrSeq = inGrSeq;

		if(isRegFurigana) {
			CFuriganaConverter fc;
			if(inAdKana.IsEmpty() == FALSE)	m_AdKana = inAdKana;
			else							m_AdKana = fc.Convert(m_AdName1);
		}

		m_IkkatuVal = GetIkkatuValAddress(inGrSeq);

		m_FgDel = 0;
		m_OrderNum = 0;
		Update();
		AutoNumOrder(m_AdSeq, inGrSeq);		//自動採番
		retVal = m_AdSeq;
	}

	// OrderNum 値が、自動採番されたので、レコードを再読込
	RequeryAdSeq(inGrSeq, retVal);

	return retVal;
}

/**********************************************************************
	CheckRepeat_Inv()
		名称リスト内に指定されたエントリが登録されているかを調べる
		
	引数
			CString inAdName1	取引先名称１ (1行目)
			CString inAdName2	取引先名称２ (2行目)
			CString inAdAdd1	取引先住所１ (1行目)
			CString inAdAdd2	取引先住所２ (2行目)
			CString inInvNo		登録番号(法人番号)
			int		inGrSeq		グループ番号 (GrSeq)
			BOOL	fAddress	住所重複確認フラグ（TRUE：対象／FALSE：対象外）
			CString inAdKana	取引先カナ

	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstAddress::CheckRepeat_Inv( CString	inAdName1
									, CString	inAdName2
									, CString	inAdAdd1
									, CString	inAdAdd2
									, CString	inInvNo
									, int		inGrSeq
									, BOOL		fAddress/*=TRUE*/
									, CString	inAdKana/*=NULL*/
)
{
	int			retVal = DB_ERR;
	CString		StrTmp[5];

	////////////////////////////////////////////////////////////////
	// 名称テーブルの名称、住所フィールドが NULL のものを
	// NULL文字列('')に変更する
	retVal = UpdateStrFieldAllForNull();

	if(retVal != DB_ERR_OK) {
		return retVal;
	}

	//	初期化
	for(int i=0; i<5; i++) {
		StrTmp[i].Empty();
	}

	// SQLコマンドでは、名称に[']が含まれている場合['']とする
	CString		strAdName1 = ConvSqlString(inAdName1);
	CString		strAdName2 = ConvSqlString(inAdName2);
	CString		strAdAdd1  = ConvSqlString(inAdAdd1);
	CString		strAdAdd2  = ConvSqlString(inAdAdd2);
	CString		strAdKana  = ConvSqlString(inAdKana);
	CString		strInvNo   = ConvSqlString(inInvNo);

	///////////////////////////////////////////////////////////////
	// 抽出条件作成（第1ステップ)  名称、住所の両方でのマッチング
	StrTmp[0].Format("GrSeq = %d ", inGrSeq);

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	//	COLLATE Japanese_CS_AS_KS_WS について補足
	//		CS	…	大文字、小文字を区別する。全角アルファベットの大文字、小文字も区別する 
	//		AS	…	アクセント、濁音、破裂音を区別する
	//		KS	…	ひらがなとカタカナを区別する。半角でも区別する 
	//		WS	…	文字幅を区別する（全角、半角を区別する）
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	//if(strAdName1.IsEmpty() == FALSE || strAdName2.IsEmpty() == FALSE) {
	if(strcmp(inAdName1,"-1")  != 0)	{		// 修正No.168526 add
		StrTmp[1].Format("((AdName1 = '%s' COLLATE Japanese_CS_AS_KS_WS) AND (AdName2 = '%s' COLLATE Japanese_CS_AS_KS_WS)) ", strAdName1, strAdName2);
	}

	// 住所も重複確認対象？
	if(fAddress == TRUE) {
	//if(fAddress == TRUE && (strAdAdd1.IsEmpty() == FALSE || strAdAdd2.IsEmpty() == FALSE)) {
		StrTmp[2].Format("((AdAdd1 = '%s' COLLATE Japanese_CS_AS_KS_WS) AND (AdAdd2 = '%s' COLLATE Japanese_CS_AS_KS_WS)) ", strAdAdd1, strAdAdd2);
	}

	// ｶﾅ文字が存在する場合は重複チェック条件に含める
	if(!(inAdKana.IsEmpty())) {
		StrTmp[3].Format("(AdKana = '%s' COLLATE Japanese_CS_AS_KS_WS) ", strAdKana);
	}

	// 登録番号がある場合も重複チェックに条件に含める
	if(inInvNo == _T("-1")) {
		StrTmp[4].Format("(Invno = NULL or Invno = '') ");
	}
	else if( !(inInvNo.IsEmpty()) ) {
		StrTmp[4].Format("Invno = '%s' ", strInvNo);
	}

	m_strFilter = _T("");
	for(int i=0; i<5; i++)	{
		if(StrTmp[i].IsEmpty() == FALSE)	{
			if(m_strFilter.IsEmpty() == FALSE)	m_strFilter += _T("AND ");
			m_strFilter += StrTmp[i];
		}
	}
	m_strSort   = _T("OrderNum");		// 並び順でソート

	retVal = RequeryWork();				// リクエリ

	return retVal;
}

// 修正No.168402,168405 add -->
/**********************************************************************
	CheckRepeat_Inv2()
		名称リスト内に指定されたエントリが登録されているかを調べる
		
	引数	int		inAdSeq		シーケンス番号
			CString inAdName1	取引先名称１ (1行目)
			CString inAdName2	取引先名称２ (2行目)
			CString inAdAdd1	取引先住所１ (上段)
			CString inAdAdd2	取引先住所２ (下段)
			int		inGrSeq		グループ番号 (GrSeq)

	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstAddress::CheckRepeat_Inv2(int		inAdSeq
									, CString	inAdName1	// 修正No.168526 add
									, CString	inAdName2	// 修正No.168526 add
									, CString	inAdAdd1	// 修正No.168534,168539 add
									, CString	inAdAdd2	// 修正No.168534,168539 add
									, int		inGrSeq
)
{
	int			retVal = DB_ERR;
	CString		StrTmp[5];

	////////////////////////////////////////////////////////////////
	// 名称テーブルの名称、住所フィールドが NULL のものを
	// NULL文字列('')に変更する
	retVal = UpdateStrFieldAllForNull();

	if(retVal != DB_ERR_OK) {
		return retVal;
	}

	//	初期化
	for(int i=0; i<5; i++) {
		StrTmp[i].Empty();
	}

	// SQLコマンドでは、名称に[']が含まれている場合['']とする
	CString		strAdName1 = ConvSqlString(inAdName1);		// 修正No.168526 add
	CString		strAdName2 = ConvSqlString(inAdName2);		// 修正No.168526 add
	CString		strAdAdd1  = ConvSqlString(inAdAdd1);		// 修正No.168534,168539 add
	CString		strAdAdd2  = ConvSqlString(inAdAdd2);		// 修正No.168534,168539 add

	///////////////////////////////////////////////////////////////
	// 抽出条件作成（第1ステップ)  名称、住所の両方でのマッチング
	StrTmp[0].Format("GrSeq = %d ", inGrSeq);

	StrTmp[1].Format("and AdSeq = %d ", inAdSeq);

// 修正No.168526 del -->
	//m_strFilter = StrTmp[0] + StrTmp[1] + StrTmp[2];
// 修正No.168526 del <--
// 修正No.168526 add -->
	if(strAdName1.IsEmpty() == FALSE || strAdName2.IsEmpty() == FALSE) {
		StrTmp[2].Format("and ((AdName1 = '%s' COLLATE Japanese_CS_AS_KS_WS) AND (AdName2 = '%s' COLLATE Japanese_CS_AS_KS_WS)) ", strAdName1, strAdName2);
	}

// 修正No.168534,168539 add -->
	if(strAdAdd1.IsEmpty() == FALSE || strAdAdd2.IsEmpty() == FALSE) {
		StrTmp[3].Format("and ((AdAdd1 = '%s' COLLATE Japanese_CS_AS_KS_WS) AND (AdAdd2 = '%s' COLLATE Japanese_CS_AS_KS_WS)) ", strAdAdd1, strAdAdd2);
	}
// 修正No.168534,168539 add <--

	//m_strFilter = StrTmp[0] + StrTmp[1] + StrTmp[2];				// 修正No.168534,168539 del
	m_strFilter = StrTmp[0] + StrTmp[1] + StrTmp[2] + StrTmp[3];	// 修正No.168534,168539 add
// 修正No.168526 add <--

	m_strSort   = _T("OrderNum");		// 並び順でソート

	retVal = RequeryWork();				// リクエリ

	return retVal;
}
// 修正No.168402,168405 add <--

// 修正No.168479 add -->
/**********************************************************************
	CheckRepeat_Inv3()
		名称リスト内に指定されたエントリが登録されているかを調べる
		
	引数
			CString inAdName1	取引先名称１ (1行目)
			CString inAdName2	取引先名称２ (2行目)
			CString inAdAdd1	取引先住所１ (1行目)
			CString inAdAdd2	取引先住所２ (2行目)
			CString inInvNo		登録番号(法人番号)
			int		inGrSeq		グループ番号 (GrSeq)
			BOOL	fAddress	住所重複確認フラグ（TRUE：対象／FALSE：対象外）
			CString inAdKana	取引先カナ

	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstAddress::CheckRepeat_Inv3( CString	inAdName1
									 , CString	inAdName2
									 , CString	inAdAdd1
									 , CString	inAdAdd2
									 , CString	inInvNo
									 , int		inGrSeq
									 , BOOL		fAddress/*=TRUE*/
									 , CString	inAdKana/*=NULL*/
)
{
	int			retVal = DB_ERR;
	CString		StrTmp[5];

	////////////////////////////////////////////////////////////////
	// 名称テーブルの名称、住所フィールドが NULL のものを
	// NULL文字列('')に変更する
	retVal = UpdateStrFieldAllForNull();

	if(retVal != DB_ERR_OK) {
		return retVal;
	}

	//	初期化
	for(int i=0; i<5; i++) {
		StrTmp[i].Empty();
	}

	// SQLコマンドでは、名称に[']が含まれている場合['']とする
	CString		strAdName1 = ConvSqlString(inAdName1);
	CString		strAdName2 = ConvSqlString(inAdName2);
	CString		strAdAdd1  = ConvSqlString(inAdAdd1);
	CString		strAdAdd2  = ConvSqlString(inAdAdd2);
	CString		strAdKana  = ConvSqlString(inAdKana);
	CString		strInvNo   = ConvSqlString(inInvNo);

	///////////////////////////////////////////////////////////////
	// 抽出条件作成（第1ステップ)  名称、住所の両方でのマッチング
	StrTmp[0].Format("GrSeq = %d ", inGrSeq);

	//++++++++++++++++++++++++++++++++++++++++++++++++++
	//	COLLATE Japanese_CS_AS_KS_WS について補足
	//		CS	…	大文字、小文字を区別する。全角アルファベットの大文字、小文字も区別する 
	//		AS	…	アクセント、濁音、破裂音を区別する
	//		KS	…	ひらがなとカタカナを区別する。半角でも区別する 
	//		WS	…	文字幅を区別する（全角、半角を区別する）
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	StrTmp[1].Format("((AdName1 = '%s' COLLATE Japanese_CS_AS_KS_WS) AND (AdName2 = '%s' COLLATE Japanese_CS_AS_KS_WS)) ", strAdName1, strAdName2);

	// 住所も重複確認対象？
	if(fAddress == TRUE) {
		StrTmp[2].Format("((AdAdd1 = '%s' COLLATE Japanese_CS_AS_KS_WS) AND (AdAdd2 = '%s' COLLATE Japanese_CS_AS_KS_WS)) ", strAdAdd1, strAdAdd2);
	}

	// ｶﾅ文字が存在する場合は重複チェック条件に含める
	if(!(inAdKana.IsEmpty())) {
		StrTmp[3].Format("(AdKana = '%s' COLLATE Japanese_CS_AS_KS_WS) ", strAdKana);
	}

	// 登録番号が既に登録されているかどうか
	StrTmp[4].Format("(Invno != '' ");
	if(strInvNo.IsEmpty() == FALSE)	{
		StrTmp[4].Format(StrTmp[4] + "and Invno != '%s' ", strInvNo);
	}
	StrTmp[4] += _T(") ");

	m_strFilter = _T("");
	for(int i=0; i<5; i++)	{
		if(StrTmp[i].IsEmpty() == FALSE)	{
			if(m_strFilter.IsEmpty() == FALSE)	m_strFilter += _T("AND ");
			m_strFilter += StrTmp[i];
		}
	}
	m_strSort   = _T("OrderNum");		// 並び順でソート

	retVal = RequeryWork();				// リクエリ

	return retVal;
}
// 修正No.168479 add <--

/**********************************************************************
	UpdateDataTableAll_Inv()
		データテーブルのデータを名称リストで更新する
		（名称リストテーブルとデータテーブルの整合をとる）
		対象の全テーブルを更新する。

	引数
			なし
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstAddress::UpdateDataTableAll_Inv(BOOL isRegAuto, int nSgn)
{
	int retVal = DB_ERR_OK;

	retVal = UpdateDataTable_Inv(ID_FORMNO_021, isRegAuto, nSgn);

	if(retVal == DB_ERR_OK) {
		retVal = UpdateDataTable_Inv(ID_FORMNO_031, isRegAuto, nSgn);
	}

	if(retVal == DB_ERR_OK) {
		retVal = UpdateDataTable_Inv(ID_FORMNO_041, isRegAuto, nSgn);
	}

	if(retVal == DB_ERR_OK) {
		retVal = UpdateDataTable_Inv(ID_FORMNO_081, isRegAuto, nSgn);
	}

	if(retVal == DB_ERR_OK) {
		retVal = UpdateDataTable_Inv(ID_FORMNO_091, isRegAuto, nSgn);
	}

	if(retVal == DB_ERR_OK) {
		retVal = UpdateDataTable_Inv(ID_FORMNO_101, isRegAuto, nSgn);
	}

	// ver1_0_2_1から以下の様式も更新
	if(retVal == DB_ERR_OK) {
		retVal = UpdateDataTable_Inv(ID_FORMNO_042, isRegAuto, nSgn);
	}
	if(retVal == DB_ERR_OK) 	retVal = UpdateDataTable_Inv(ID_FORMNO_161, isRegAuto, nSgn);
	if(retVal == DB_ERR_OK) 	retVal = UpdateDataTable_Inv(ID_FORMNO_162, isRegAuto, nSgn);

	return retVal;
}

/**********************************************************************
	UpdateDataTable_Inv()
		データテーブルのデータを名称リストで更新する
		（名称リストテーブルとデータテーブルの整合をとる）

	引数
			int		inFormSeq	変更先データテーブルのフォームシーケンス番号

	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstAddress::UpdateDataTable_Inv(int inFormSeq, BOOL isRegAuto, int nSgn)
{
	int retVal = DB_ERR_OK;

	switch (inFormSeq) {
	case ID_FORMNO_021:
		retVal = UpdateDataTableSub_Inv( 
						STR_TBL_NAME_021	//(売)名称のみ
						, _T("AdSeq")
						, _T("AdName1")
						, _T("AdName2")
						, _T("")			// 住所フィールドを持っていない
						, _T("")			// 住所フィールドを持っていない
						, _T("AdOrder")
						, _T("AdKana")
						, _T("Invno")
						, isRegAuto
						, TRUE
						, ( (nSgn == 0) ? 0 : ID_ADDRESSGR_URI ) 
						);
		break;

	case ID_FORMNO_031:
		retVal = UpdateDataTableSub_Inv( 
						STR_TBL_NAME_031	//(売)
						, _T("AdSeq")
						, _T("AdName1")
						, _T("AdName2")
						, _T("AdAdd1")
						, _T("AdAdd2")
						, _T("AdOrder")
						, _T("AdKana")
						, _T("Invno")
						, isRegAuto
						, TRUE
						, ( (nSgn == 0) ? 0 : ID_ADDRESSGR_URI ) 
						);
		break;

	case ID_FORMNO_041:
		retVal = UpdateDataTableSub_Inv( 
						STR_TBL_NAME_041	//(買)
						, _T("AdSeq")
						, _T("AdName1")
						, _T("AdName2")
						, _T("AdAdd1")
						, _T("AdAdd2")
						, _T("AdOrder")
						, _T("AdKana")
						, _T("Invno")
						, isRegAuto
						, TRUE
						, ( (nSgn == 0) ? 0 : ID_ADDRESSGR_KAI ) 
						);
		break;

	case ID_FORMNO_042:
		retVal = UpdateDataTableSub_Inv( 
						STR_TBL_NAME_042
						, _T("AdSeq")
						, _T("AdName1")
						, _T("AdName2")
						, _T("AdAdd1")
						, _T("AdAdd2")
						, _T("AdOrder")
						, _T("AdKana")
						, _T("Invno")
						, isRegAuto
						, FALSE
						, ( (nSgn == 0) ? 0 : ID_ADDRESSGR_URI )
						);
		// 簡素化かつ、末尾スペース除外ではない場合のみ処理を行う
		if(bG_Kanso == TRUE && nSgn == 0) {
			Data042Convert();
		}
		break;

	case ID_FORMNO_081:
		retVal = UpdateDataTableSub_Inv( 
						STR_TBL_NAME_081	//(買)名称のみ更新
						, _T("AdSeq")
						, _T("AdName1")
						, _T("AdName2")
						, _T("")			// 住所フィールドを持っていない
						, _T("")			// 住所フィールドを持っていない
						, _T("AdOrder")
						, _T("AdKana")
						, _T("Invno")
						, isRegAuto
						, TRUE
						, ( (nSgn == 0) ? 0 : ID_ADDRESSGR_KAI ) 
						);
		break;

	case ID_FORMNO_091:
		retVal = UpdateDataTableSub_Inv( 
						STR_TBL_NAME_091	//(買)
						, _T("AdSeq")
						, _T("AdName1")
						, _T("AdName2")
						, _T("AdAdd1")
						, _T("AdAdd2")
						, _T("AdOrder")
						, _T("AdKana")
						, _T("Invno")
						, isRegAuto
						, TRUE
						, ( (nSgn == 0) ? 0 : ID_ADDRESSGR_KAI ) 
						);
		break;

	case ID_FORMNO_101:
		retVal = UpdateDataTableSub_Inv( 
						STR_TBL_NAME_101	//(売)
						, _T("AdSeq")
						, _T("AdName1")
						, _T("AdName2")
						, _T("AdAdd1")
						, _T("AdAdd2")
						, _T("AdOrder")
						, _T("AdKana")
						, _T("Invno")
						, isRegAuto
						, TRUE
						, ( (nSgn == 0) ? 0 : ID_ADDRESSGR_URI ) 
						);
		break;

	case ID_FORMNO_161:
		retVal = UpdateDataTableSub_Inv( 
						STR_TBL_NAME_161
						, _T("AdSeq")
						, _T("AdName1")
						, _T("AdName2")
						, _T("AdAdd1")
						, _T("AdAdd2")
						, _T("AdOrder")
						, _T("AdKana")
						, _T("Invno")
						, isRegAuto
						, FALSE
						, ( (nSgn == 0) ? 0 : ID_ADDRESSGR_KAI )
						);
		break;

	case ID_FORMNO_162:
		retVal = UpdateDataTableSub_Inv( 
						STR_TBL_NAME_162
						, _T("AdSeq")
						, _T("AdName1")
						, _T("AdName2")
						, _T("AdAdd1")
						, _T("AdAdd2")
						, _T("AdOrder")
						, _T("AdKana")
						, _T("Invno")
						, isRegAuto
						, FALSE
						, ( (nSgn == 0) ? 0 : ID_ADDRESSGR_URI )
						);
		break;
	}

	return retVal;
}

/**********************************************************************
	UpdateDataTableSub_Inv()
		データテーブルのデータを名称リストで更新する
		（名称リストテーブルとデータテーブルの整合をとる）

	(1) シーケンス番号が同じ項目(DataTabel.AdSeq = this.AdSeq)で更新
			取引先名(AdName1, AdName2)、
			住所    (AdAdd1,  AdAdd2)
			並び順  (AdOrder)
			カナ    (AdKana)

  	(2)	データテーブルの名称シーケンス番号が NULL であった場合
		データテーブルのデータを以下とする
			取引先シーケンス番号(AdSeq) = 0

	(3)	名称リストテーブルに存在しないシーケンス番号が、
		データテーブルに出現した場合
		データテーブルのデータを以下とする
			取引先名(AdName1) = ""
			取引先名(BkName2) = ""
			住所    (AdAdd1)  = ""
			住所    (AdAdd2)  = ""
			並び順  (AdOrder) = 0
			シーケンス(AdSeq) = 0
			カナ    (AdKana) = ""
		ただし、「取引先、金融機関を自動で登録する」OFFまたは④－２、⑯－１、⑯－２はAdSeq = 0の存在が許される為、
		uc_lst_addressに登録されていなくても（されていたらバグ）許可する

	※ 取引先名称は、⑰⑱のその他帳表では、使用していないため、
	   そのデータテーブルに対する FormSeq の対応を入れていない
	   が、、、
	   ⑰⑱のデータテーブルに対しても、FormSeq対応を入れなくとも
	   大丈夫ではないかと思われる。
	   テーブル内の全データに対して、UPDATEがかかるため。

	引数
			CString inTableName		変更先データテーブル名
			CString inNameAdSeq		データテーブルのフィールド名(取引先シーケンス番号)
			CString inNameAdName1	データテーブルのフィールド名(取引先名)
			CString inNameAdName2	データテーブルのフィールド名(取引先名)
			CString inNameAdAdd1	データテーブルのフィールド名(取引先住所)
			CString inNameAdAdd2	データテーブルのフィールド名(取引先住所)
			CString inNameAdOrder	データテーブルのフィールド名(取引先順序番号)
			CString inNameAdKana	データテーブルのフィールド名(カナ)
			BOOL	isDeleteAdSeqZero F9自動登録しない様式（④－２、⑯－１、⑯－２）は(3)を実行しない

	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstAddress::UpdateDataTableSub_Inv(
						  CString	inTableName			// データテーブル名
						, CString	inNameAdSeq			// フィールド名
						, CString	inNameAdName1		// フィールド名
						, CString	inNameAdName2		// フィールド名
						, CString	inNameAdAdd1		// フィールド名
						, CString	inNameAdAdd2		// フィールド名
						, CString	inNameAdOrder		// フィールド名
						, CString	inNameAdKana		// データテーブルのフィールド名(カナ)
						, CString	inNameInvno			// 登録番号
						, BOOL		isRegAuto
						, BOOL		isDeleteAdSeqZero
						, int		nSgn
)
{
	CRecordset	rs(m_lo_pdb);
	CString		SqlStr;

	int			retVal1 = DB_ERR;
	int			retVal2 = DB_ERR;
	int			retVal3 = DB_ERR;

	///////////////////////////////////////////////////////////////
	//(1)
	///////////////////////////////////////////////////////////////
	// SQL 作成

	// 起動時、末尾スペースチェック(DeleteSpaceMaster)から呼ばれた場合は処理を行わない
	if(nSgn == 0) {
		SqlStr  = _T("UPDATE ") + inTableName + _T(" ");
		SqlStr += _T("SET ");
		if(inNameAdName1 != "") {
			SqlStr +=			inNameAdName1 + _T(" = lst.AdName1, ");
		}
		if(inNameAdName2 != "") {
			SqlStr +=			inNameAdName2 + _T(" = lst.AdName2, ");
		}
		if(inNameAdAdd1 != "") {
			SqlStr +=			inNameAdAdd1  + _T(" = lst.AdAdd1, ");
		}
		if(inNameAdAdd2 != "") {
			SqlStr +=			inNameAdAdd2  + _T(" = lst.AdAdd2, ");
		}
		if(inNameInvno != "") {
			SqlStr +=			inNameInvno  + _T(" = lst.Invno, ");
		}
		SqlStr += inNameAdKana + _T(" = lst.AdKana, ");
		SqlStr += inNameAdOrder + _T(" = lst.OrderNum ");
		SqlStr += _T("FROM ") + inTableName + _T(" AS data ");
		SqlStr += _T("JOIN ") + m_lo_TableName + _T(" AS lst ");
		SqlStr += _T("ON data.") + inNameAdSeq + _T(" = lst.AdSeq");

		///////////////////////////////////////////////////////////////
		// SQL 実行
		retVal1 = ExecuteSQLWork(SqlStr);
	}
	else	{
		retVal1 = DB_ERR_OK;
	}

	///////////////////////////////////////////////////////////////
	//(2)
	///////////////////////////////////////////////////////////////
	// SQL 作成

	SqlStr  = _T("UPDATE ") + inTableName + _T(" ");
	SqlStr += _T("SET ")   + inNameAdSeq + _T(" = 0 ");
	SqlStr += _T("WHERE ") + inNameAdSeq + _T(" IS NULL");

	///////////////////////////////////////////////////////////////
	// SQL 実行
	retVal2 = ExecuteSQLWork(SqlStr);

	// 起動時、末尾スペースチェック(DeleteSpaceMaster)から呼ばれた場合は処理を行わない
	if(nSgn == 0) {
		///////////////////////////////////////////////////////////////
		//(3)
		///////////////////////////////////////////////////////////////
		// SQL 作成
		SqlStr  = _T("UPDATE ") + inTableName + _T(" ");
		SqlStr += _T("SET ") + inNameAdSeq   + _T(" = 0, ");
		if (inNameAdName1 != "") {
			SqlStr +=			inNameAdName1 + _T(" = '', ");
		}
		if (inNameAdName2 != "") {
			SqlStr +=			inNameAdName2 + _T(" = '', ");
		}
		if (inNameAdAdd1 != "") {
			SqlStr +=			inNameAdAdd1  + _T(" = '', ");
		}
		if (inNameAdAdd1 != "") {
			SqlStr +=			inNameAdAdd2  + _T(" = '', ");
		}
		if(inNameInvno !="") {
			SqlStr +=			inNameInvno   + _T(" = '', ");
		}
		SqlStr += inNameAdKana + _T(" = '', ");
		SqlStr += inNameAdOrder + _T(" = 0 ");
		SqlStr += _T("WHERE FgFunc = 1 ");
		SqlStr += _T("AND ") + inNameAdSeq + _T(" <> 0 ");
		SqlStr += _T("AND ") + inNameAdSeq + _T(" NOT IN ( ");
		SqlStr += _T("SELECT lst.AdSeq ");
		SqlStr += _T("FROM ") + m_lo_TableName  + _T(" AS lst ");
		SqlStr += _T("WHERE FgDel = 0 ");
		SqlStr += _T(") ");

		///////////////////////////////////////////////////////////////
		// SQL 実行
		retVal3 = ExecuteSQLWork(SqlStr);
	}
	else {
		CString sql;
		sql  = _T("UPDATE ") + inTableName + _T(" ");
		sql += _T("SET ")  + inNameAdSeq + _T(" = lst.AdSeq, ");
		// カナ入力のある様式はカナの登録を行う
		if(inNameAdKana.IsEmpty() == FALSE)	{
			sql += inNameAdKana + _T(" = lst.AdKana, ");
		}
		sql +=				inNameAdOrder + _T(" = lst.OrderNum ");
		sql += _T("FROM ") + inTableName + _T(" AS data ");
		sql += _T("JOIN ") + m_lo_TableName + _T(" AS lst ");
		sql += _T("ON isnull(data.") + inNameAdName1 + _T(",'') collate Japanese_CS_AS_KS_WS = isnull(lst.AdName1,'') collate Japanese_CS_AS_KS_WS ");
		sql += _T("AND isnull(data.") + inNameAdName2 + _T(",'') collate Japanese_CS_AS_KS_WS = isnull(lst.AdName2,'') collate Japanese_CS_AS_KS_WS ");
		// 所在地の比較
		if(inTableName == STR_TBL_NAME_042 && bG_Kanso == TRUE) {
			// ④-2の簡素化様式のみ
			// データ側には改行マークが、マスター側には上段の末尾に入力したスペースが
			// 入っている場合があるので、それぞれの文字を除外して比較する
			if(inNameAdAdd1.IsEmpty() == FALSE) {
				sql += _T("AND isnull(replace(data.") + inNameAdAdd1 + _T(",CHAR(13)+CHAR(10),''),'') collate Japanese_CS_AS_KS_WS = ");
				sql += _T("isnull(replace(lst.AdAdd1,' ',''),'') collate Japanese_CS_AS_KS_WS ");
			}
			if(inNameAdAdd2.IsEmpty() == FALSE) {
				sql += _T("AND isnull(replace(data.") + inNameAdAdd2 + _T(",CHAR(13)+CHAR(10),''),'') collate Japanese_CS_AS_KS_WS = ");
				sql += _T("isnull(replace(lst.AdAdd2,' ',''),'') collate Japanese_CS_AS_KS_WS ");
			}
		}
		else	{
			if(inNameAdAdd1.IsEmpty() == FALSE) {
				sql += _T("AND isnull(replace(data.") + inNameAdAdd1 + _T(",CHAR(13)+CHAR(10),''),'') collate Japanese_CS_AS_KS_WS = isnull(lst.AdAdd1,'') collate Japanese_CS_AS_KS_WS ");
			}
			if(inNameAdAdd2.IsEmpty() == FALSE) {
				sql += _T("AND isnull(replace(data.") + inNameAdAdd2 + _T(",CHAR(13)+CHAR(10),''),'') collate Japanese_CS_AS_KS_WS = isnull(lst.AdAdd2,'') collate Japanese_CS_AS_KS_WS ");
			}
		}
		sql += _T("AND lst.Invno = '") + inNameInvno + _T("' ");
		sql.Format(sql + _T("AND lst.GrSeq = %d "),nSgn);

		retVal3 = ExecuteSQLWork(sql);
	}

	if (retVal1 != DB_ERR_OK) {
		return retVal1;
	}
	if (retVal2 != DB_ERR_OK) {
		return retVal2;
	}
	return retVal3;
}

/**********************************************************************
		取引先名称のみから登録されていればその取引先のシーケンス、
		されていなければ0を返す
		所在地を条件に含めない為、オーバーロード
		
	引数
			adname1		取引先名称１
			adname2		取引先名称２
			adadd1		取引先所在地１
			adadd2		取引先所在地２

	戻値
			int		登録されている：Seq
					登録されていない：0
					エラー：-1
***********************************************************************/
int CdbUcLstAddress::GetSeqFromAdName_Inv(int inGrSeq, CString adname1, CString adname2, CString invno)
{
	// 名称テーブルの名称、住所フィールドが NULL のものを
	// NULL文字列('')に変更する
	int ret = UpdateStrFieldAllForNull();
	if(ret != DB_ERR_OK)	return -1;	// エラー

	ret = RequeryAdName_Inv(inGrSeq, adname1, adname2, invno);
	
	if(ret != DB_ERR_OK)	return -1;	// エラー
	if(this->IsEOF())		return 0;	// 登録されていない
	return this->m_AdSeq;				// 登録済み
}

/**********************************************************************
		取引先名称、所在地から登録されていればその取引先のシーケンス、
		されていなければ0を返す
		所在地が空白でも、""をキーとしてリクエリする
		
	引数
			adname1		取引先名称１
			adname2		取引先名称２
			adadd1		取引先所在地１
			adadd2		取引先所在地２

	戻値
			int		登録されている：Seq
					登録されていない：0
					エラー：-1
***********************************************************************/
int CdbUcLstAddress::GetSeqFromAdName_Inv(int inGrSeq, CString adname1, CString adname2, CString adadd1, CString adadd2, CString invno)
{
	// 名称テーブルの名称、住所フィールドが NULL のものを
	// NULL文字列('')に変更する
	int ret = UpdateStrFieldAllForNull();
	if(ret != DB_ERR_OK)	return -1;	// エラー

	ret = RequeryAdName_Inv(inGrSeq, adname1, adname2, adadd1, adadd2, invno);
	
	if(ret != DB_ERR_OK)	return -1;	// エラー
	if(this->IsEOF())		return 0;	// 登録されていない
	return this->m_AdSeq;				// 登録済み
}

/**********************************************************************
		F9から参照はするが登録はしない場合、取引先名、住所からAdSeq,AdOrderを取得する
		
	引数
			adname1		取引先名称１
			adname2		取引先名称２
			adadd1		住所１
			adadd2		住所２

	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstAddress::RequeryAdName_Inv(int inGrSeq, CString adname1, CString adname2, CString adadd1, CString adadd2, CString invno)
{
	CString	strGrSeq=_T("");
	CString	strAdName1=_T("");
	CString	strAdName2=_T("");
	CString	strAdAdd1=_T("");
	CString	strAdAdd2=_T("");

	// シングルクォーテーションを置き換える
	// (')	 → ('')
	// ('')	 → ('''')
	// (''') → (''''')
	strAdName1=adname1;
	strAdName1.Replace(_T("'"),_T("''"));
	strAdName2=adname2;
	strAdName2.Replace(_T("'"),_T("''"));
	strAdAdd1=adadd1;
	strAdAdd1.Replace(_T("'"),_T("''"));
	strAdAdd2=adadd2;
	strAdAdd2.Replace(_T("'"),_T("''"));

	strGrSeq.Format("%d", inGrSeq);

	m_strFilter  = "GrSeq = " + strGrSeq + " AND ";
	m_strFilter += "AdName1 collate Japanese_CS_AS_KS_WS = '"+ strAdName1 +"' AND AdName2 collate Japanese_CS_AS_KS_WS = '"+ strAdName2 +"' AND ";
	m_strFilter += "AdAdd1 collate Japanese_CS_AS_KS_WS = '"+ strAdAdd1 +"' AND AdAdd2 collate Japanese_CS_AS_KS_WS  = '"+ strAdAdd2 +"' AND ";
	m_strFilter += "Invno = '" + invno + "' ";

	m_strSort = "";

	return RequeryWork();
}

/**********************************************************************
		F9から参照はするが登録はしない場合、取引先名のみからAdSeq,AdOrderを取得する
		もし取引先が同じで異なる所在地がある場合、オーダーが小さいほうを取得する
		
	引数
			adname1		取引先名称１
			adname2		取引先名称２

	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcLstAddress::RequeryAdName_Inv(int inGrSeq, CString adname1, CString adname2, CString invno)
{
	CString strGrSeq=_T("");
	CString	strAdName1=_T("");
	CString	strAdName2=_T("");

	// シングルクォーテーションを置き換える
	// (')	 → ('')
	// ('')	 → ('''')
	// (''') → (''''')
	strAdName1=adname1;
	strAdName1.Replace(_T("'"),_T("''"));
	strAdName2=adname2;
	strAdName2.Replace(_T("'"),_T("''"));

	strGrSeq.Format("%d", inGrSeq);

	m_strFilter  = "GrSeq = " + strGrSeq + " AND ";
	m_strFilter += "AdName1 collate Japanese_CS_AS_KS_WS = '"+ strAdName1 +"' AND AdName2 collate Japanese_CS_AS_KS_WS = '"+ strAdName2 +"' AND ";
	m_strFilter += "Invno = '" + invno + "' ";

	m_strSort = "OrderNum";	// 同名の取引先名称が存在する為

	return RequeryWork();
}

/**********************************************************************
	登録番号入力時に、参照ダイアログにとうろくされているかチェックし、
	登録されていれば名称、所在地を返送する
		
	引数	inGrSeq		グループ番号
			invno		登録番号（法人番号）

	戻値	int			0:登録済み、1:未登録、-1:エラー
***********************************************************************/
int CdbUcLstAddress::GetAddressInf_Inv(int inGrSeq, CString invno)
{
	int			rt1,rt2 = 0;
	CString		strGrSeq=_T("");

	strGrSeq.Format("%d", inGrSeq);

	m_strFilter  = "FgDel = 0 and GrSeq = " + strGrSeq + " AND ";
	m_strFilter += "Invno = '" + invno + "' ";

	m_strSort = "OrderNum";	// 同名の取引先名称が存在する為

	rt1 = RequeryWork();

	rt2 = 0;
	if(rt1 != DB_ERR_OK)	rt2 = -1;	// エラー
	if(this->IsEOF())		rt2 = 1;	// 登録されていない
	
	return(rt2);
}
// インボイス登録番号追加対応_23/11/06 add <--
