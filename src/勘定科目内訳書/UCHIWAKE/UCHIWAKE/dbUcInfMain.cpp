// dbUcInfMain.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
//#include "UCHIWAKE.h"
#include "dbUcInfMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CdbUcInfMain

IMPLEMENT_DYNAMIC(CdbUcInfMain, CdbBase)

CdbUcInfMain::CdbUcInfMain(CDatabase* pdb)
	: CdbBase(pdb)
{
	//{{AFX_FIELD_INIT(CdbUcInfMain)
	m_FormSeq = 0;
	m_OutKikan = 0;
	m_OutKingaku = 0;
	m_OutPage = 0;
	m_OutZero = 0;
	m_Seq = 0;
	m_FgUchiConv = 0;
	m_ssymd = 0;
	m_eeymd = 0;
	m_OutCoName = 0;
	m_GeneralConstVal.RemoveAll();
	m_GeneralVar.RemoveAll();
	m_nFields = 11;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	m_lo_pdb = pdb;								// データベースポインタを保持
	m_lo_TableName = STR_TBL_NAME_INF_MAIN;		// テーブル名を記述
}


CString CdbUcInfMain::GetDefaultSQL()
{
	return _T("[dbo].[uc_inf_main]");
}

void CdbUcInfMain::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CdbUcInfMain)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Int(pFX, _T("[Seq]"), m_Seq);
	RFX_Int(pFX, _T("[FormSeq]"), m_FormSeq);
	RFX_Byte(pFX, _T("[OutKikan]"), m_OutKikan);
	RFX_Byte(pFX, _T("[OutKingaku]"), m_OutKingaku);
	RFX_Byte(pFX, _T("[OutPage]"), m_OutPage);
	RFX_Byte(pFX, _T("[FgUchiConv]"), m_FgUchiConv);
	RFX_Long(pFX, _T("[ssymd]"), m_ssymd);
	RFX_Long(pFX, _T("[eeymd]"), m_eeymd);
	RFX_Byte(pFX, _T("[OutCoName]"), m_OutCoName);
	RFX_Binary(pFX, _T("[GeneralConstVal]"), m_GeneralConstVal );
	RFX_Binary(pFX, _T("[GeneralVar]"), m_GeneralVar );
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CdbUcInfMain 診断

#ifdef _DEBUG
void CdbUcInfMain::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbUcInfMain::Dump(CDumpContext& dc) const
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
int CdbUcInfMain::Init()
{
	///////////////////////////////////////////////////////////////////
	// 目的のレコード（1レコード）を抽出する
	// uc_inf_main は、Seq = 1 のみが有効行
	m_strFilter = "Seq = 1";
	m_strSort = "";

	int retVal = RequeryWork();
	
	if ( retVal != DB_ERR_OK ) {
		return retVal;
	}

	///////////////////////////////////////////////////////////////////
	// テーブル内にレコードが無かった場合にレコード作成
	// デフォルト値をセットする
	if ( IsEOF() )	{
		AddNew();
		m_Seq = 1;			// 1(固定)
		m_FormSeq = 1;
		m_OutKikan = 0;
		m_OutKingaku = 0;
		m_OutPage = 0;
		m_FgUchiConv = 0;
		m_ssymd = 0;
		m_eeymd = 0;
		Update();			//?? エラー処理必要？
		Requery();			//?? エラー処理必要？
	}

	return DB_ERR_OK;		//0：成功、0以外：失敗
}

/**********************************************************************
	UpdateFgUchiConv()
		内訳書コンバージョン実行フラグ更新

	引数
			BYTE	bFgUchiConv	実行フラグ（0:未実行、1:実行済み）

	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗

***********************************************************************/
int CdbUcInfMain::UpdateFgUchiConv(BYTE bFgUchiConv)
{
	CString		StrSql;
	
	// SQLコマンド作成
	StrSql.Format("UPDATE %s SET FgUchiConv=%d ", m_lo_TableName, bFgUchiConv);

	return	ExecuteSQLWork(StrSql);
}

/**********************************************************************
	UpdateSSymdEEymd()
		内訳書の決算期間更新

	引数
			long	lngSSymd	決算期間（期首）
			long	lngEEymd	決算期間（期末）

	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗

***********************************************************************/
int CdbUcInfMain::UpdateSSymdEEymd(long lngSSymd, long lngEEymd)
{
	CString		StrSql;

// midori 154696 del -->
//// midori 190301 del -->
//	//// SQLコマンド作成
//	//StrSql.Format("UPDATE %s SET ssymd=%d, eeymd=%d", m_lo_TableName, lngSSymd, lngEEymd);
//
//	//return	ExecuteSQLWork(StrSql);
//// midori 190301 del <--
//// midori 190301 add -->
//	int			retVal=0;
//	int			symd=0,eymd=0;
//
//	// 更新前の決算期間を取得
//	if(Init() == DB_ERR_OK)	{
//		symd = m_ssymd;
//		eymd = m_eeymd;
//		Fin();
//	}
//
//	// SQLコマンド作成
//	StrSql.Format("UPDATE %s SET ssymd=%d, eeymd=%d", m_lo_TableName, lngSSymd, lngEEymd);
//	retVal = ExecuteSQLWork(StrSql);
//	if(retVal != DB_ERR_OK)	{
//		return(retVal);
//	}
//
//	// 更新後の期首が 31年 4月 1日以降であれば
//	// メッセージを表示するためにサインをたてる
//	if((symd != 0) || (eymd != 0)) {
//		if(Init() == DB_ERR_OK)	{
//			Edit();
//			if(lngSSymd >= 20190401)	{
//				m_GeneralVar[2] |= BIT_D4;
//			}
//			// それ以外はサインをおろす
//			else	{
//				m_GeneralVar[2] &= ~BIT_D4;
//			}
//			Update();
//		}
//		Fin();
//	}
//
//	return(DB_ERR_OK);
//// midori 190301 add <--
// midori 154696 del <--
// midori 154696 add -->
	// SQLコマンド作成
	StrSql.Format("UPDATE %s SET ssymd=%d, eeymd=%d", m_lo_TableName, lngSSymd, lngEEymd);

	return	ExecuteSQLWork(StrSql);
// midori 154696 add <--
}
