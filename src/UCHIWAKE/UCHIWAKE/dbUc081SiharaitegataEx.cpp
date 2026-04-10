// 改良No.21-0086,21-0529 add -->
#include "StdAfx.h"
#include "dbUc081SiharaitegataEx.h"

/////////////////////////////////////////////////////////////////////////////

CdbUc081SiharaitegataEx::CdbUc081SiharaitegataEx(CDatabase* db) : CdbUc081Siharaitegata(db)
{
}

CdbUc081SiharaitegataEx::~CdbUc081SiharaitegataEx(void)
{
}

void CdbUc081SiharaitegataEx::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CdbUc081Siharaitegata)
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
	RFX_Long(pFX, _T("[AdSeq]"), m_AdSeq);
	RFX_Long(pFX, _T("[AdOrder]"), m_AdOrder);
	RFX_Text(pFX, _T("[AdName1]"), m_AdName1);
	RFX_Text(pFX, _T("[AdName2]"), m_AdName2);
	RFX_Long(pFX, _T("[SpDate]"), m_SpDate);
	RFX_Long(pFX, _T("[PmDate]"), m_PmDate);
	RFX_Long(pFX, _T("[BkSeq]"), m_BkSeq);
	RFX_Long(pFX, _T("[BkOrder]"), m_BkOrder);
	RFX_Text(pFX, _T("[BkName1]"), m_BkName1);
	RFX_Text(pFX, _T("[BkName2]"), m_BkName2);
	RFX_Long(pFX, _T("[KnOrder]"), m_KnOrder);
	RFX_Text(pFX, _T("[Val]"), m_Val);
	RFX_Text(pFX, _T("[Teki]"), m_Teki);
	RFX_Byte(pFX, _T("[ShowKeiZero]"), m_ShowKeiZero);
	RFX_Text(pFX, _T("[AdKana]"), m_AdKana);
	RFX_Text(pFX, _T("[Invno]"), m_InvNo);
	RFX_Long(pFX, _T("[KnSeq]"), m_KnSeq);
	RFX_Text(pFX, _T("[KnName]"), m_KnName);
	RFX_Text(pFX, _T("[KnKana]"), m_KnKana);
	//}}AFX_FIELD_MAP
}

BOOL CdbUc081SiharaitegataEx::MyOpen()
{
	BOOL bRet = FALSE;	// 戻り値※オープン失敗値で初期化

	try {
		// テーブルオープン
		bRet = Open(AFX_DB_USE_DEFAULT_TYPE, CString("select [Seq],[NumPage],[NumRow],[FgFunc],[FgShow],[KeiStr],[NumGroup],[RenKcd],[RenEcd],[RenFgTemp],[AdSeq],[AdOrder],[AdName1],[AdName2],[SpDate],[PmDate],[BkSeq],[BkOrder],[BkName1],[BkName2],[KnOrder],[Val],[Teki],[ShowKeiZero],[AdKana],[Invno],[KnSeq],[KnName],[KnKana] from [dbo].[uc_081_siharaitegata] "));
	}
	catch(CDBException *e) {
		// DBエラーメッセージ表示～アプリ終了(ICSExit)
		DBErrExit( e->m_strError , m_lo_TableName );
		// 開放
		e->Delete();
		bRet = FALSE;
	}

	return bRet;
}

int CdbUc081SiharaitegataEx::RequeryWork()
{
	if(!IsOpen()) {		// オープンしていない場合
		if(!MyOpen()) {
			return DB_ERR_OPEN;		// オープンに失敗
		}
	}
	else {					// すでにオープンしている場合
		if(!Requery()) {
			return DB_ERR_REQU;		// リクエリに失敗
		}
	}

	return DB_ERR_OK;
}

/**********************************************************************
***********************************************************************/
long CdbUc081SiharaitegataEx::GetColumnSortAddressBank(BYTE ItemSeq)
{
	if(ItemSeq == 1)	return this->m_AdOrder;
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
int CdbUc081SiharaitegataEx::UpdateDataTableWork()
{
	int	retVal1 = UpdateDataTableNullToStrNull("AdName1");
	int	retVal2 = UpdateDataTableNullToStrNull("AdName2");

	if (retVal1 != DB_ERR_OK) {
		return	retVal1;
	}
	return retVal2;
}

// 改良No.21-0086,21-0529 add <--
