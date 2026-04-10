// dbUcInfSub.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
//#include "UCHIWAKE.h"
#include "dbUcInfSub.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CdbUcInfSub

IMPLEMENT_DYNAMIC(CdbUcInfSub, CdbBase)

CdbUcInfSub::CdbUcInfSub(CDatabase* pdb)
	: CdbBase(pdb)
{
	//{{AFX_FIELD_INIT(CdbUcInfSub)
	m_FormSeq = 0;
	m_FormNo = 0;
	m_FormNo2 = 0;
	m_FormNo3 = 0;
	m_FormOrder = 0;
	m_Title = _T("");
	m_TitleNo = _T("");
	m_TitleNo2 = _T("");
	m_TitleOrg = _T("");
	m_TitleOrgTujo = _T("");
	m_TitleSave = _T("");
	m_TitleSaveTujo = _T("");
	m_Sort1 = 0;
	m_Sort2 = 0;
	m_Sort3 = 0;
	m_OpCykeiKakko = 0;
	m_OpCykeiName = _T("");
	m_OpIkkatu = 0;
	m_OpIkkatuFg = 0;
	m_OpIkkatuVal = _T("");
	m_OpIkkatuValOrg = _T("");
	m_OpKaiPage = 0;
	m_OpOutNullLine = 0;
	m_OpOutSyokei = 0;
	m_OpSykeiAuto = 0;
	m_OpSykeiKakko = 0;
	m_OpSykeiName = _T("");
	m_OpTitleKn = 0;
	m_OpTitleKnFg = 0;
	m_OutKei = 0;
	m_CurCtlIndex = 0;
	m_CurField = 0;
	m_CurPage = 0;
	m_CurRow = 0;
	m_GeneralConstVal.RemoveAll();
	m_GeneralVar.RemoveAll();
	m_nFields = 36;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
	m_lo_pdb = pdb;								// データベースポインタを保持
	m_lo_TableName = STR_TBL_NAME_INF_SUB;		// テーブル名を記述
}


CString CdbUcInfSub::GetDefaultSQL()
{
	return _T("[dbo].[uc_inf_sub]");
}

void CdbUcInfSub::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CdbUcInfSub)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Int(pFX, _T("[CurCtlIndex]"), m_CurCtlIndex);
	RFX_Byte(pFX, _T("[CurField]"), m_CurField);
	RFX_Int(pFX, _T("[CurPage]"), m_CurPage);
	RFX_Byte(pFX, _T("[CurRow]"), m_CurRow);
	RFX_Byte(pFX, _T("[FormNo]"), m_FormNo);
	RFX_Byte(pFX, _T("[FormNo2]"), m_FormNo2);
	RFX_Byte(pFX, _T("[FormNo3]"), m_FormNo3);
	RFX_Int(pFX, _T("[FormOrder]"), m_FormOrder);
	RFX_Int(pFX, _T("[FormSeq]"), m_FormSeq);
	RFX_Byte(pFX, _T("[OpCykeiKakko]"), m_OpCykeiKakko);
	RFX_Text(pFX, _T("[OpCykeiName]"), m_OpCykeiName);
	RFX_Byte(pFX, _T("[OpIkkatu]"), m_OpIkkatu);
	RFX_Byte(pFX, _T("[OpIkkatuFg]"), m_OpIkkatuFg);
	RFX_Text(pFX, _T("[OpIkkatuVal]"), m_OpIkkatuVal);
	RFX_Text(pFX, _T("[OpIkkatuValOrg]"), m_OpIkkatuValOrg);
	RFX_Byte(pFX, _T("[OpKaiPage]"), m_OpKaiPage);
	RFX_Byte(pFX, _T("[OpOutNullLine]"), m_OpOutNullLine);
	RFX_Byte(pFX, _T("[OpOutSyokei]"), m_OpOutSyokei);
	RFX_Byte(pFX, _T("[OpSykeiAuto]"), m_OpSykeiAuto);
	RFX_Byte(pFX, _T("[OpSykeiKakko]"), m_OpSykeiKakko);
	RFX_Text(pFX, _T("[OpSykeiName]"), m_OpSykeiName);
	RFX_Byte(pFX, _T("[OpTitleKn]"), m_OpTitleKn);
	RFX_Byte(pFX, _T("[OpTitleKnFg]"), m_OpTitleKnFg);
	RFX_Byte(pFX, _T("[OutKei]"), m_OutKei);
	RFX_Byte(pFX, _T("[Sort1]"), m_Sort1);
	RFX_Byte(pFX, _T("[Sort2]"), m_Sort2);
	RFX_Byte(pFX, _T("[Sort3]"), m_Sort3);
	RFX_Text(pFX, _T("[Title]"), m_Title);
	RFX_Text(pFX, _T("[TitleNo]"), m_TitleNo);
	RFX_Text(pFX, _T("[TitleNo2]"), m_TitleNo2);
	RFX_Text(pFX, _T("[TitleOrg]"), m_TitleOrg);
	RFX_Text(pFX, _T("[TitleOrgTujo]"), m_TitleOrgTujo);
	RFX_Text(pFX, _T("[TitleSave]"), m_TitleSave);
	RFX_Text(pFX, _T("[TitleSaveTujo]"), m_TitleSaveTujo);
	RFX_Binary(pFX, _T("[GeneralConstVal]"), m_GeneralConstVal );
	RFX_Binary(pFX, _T("[GeneralVar]"), m_GeneralVar );
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CdbUcInfSub 診断

#ifdef _DEBUG
void CdbUcInfSub::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbUcInfSub::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

/**********************************************************************
	Init()
		初期化処理（Open()の代わりとして使用）
		様式並び順番号(表示インデックス) 指定バージョン

	引数
			BYTE	FormIndex	様式番号(並び順番号)
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcInfSub::Init(int FormIndex)
{
	return RequeryFormOrder(FormIndex);
}

/**********************************************************************
	InitOrder()
		初期化処理（Open()の代わりとして使用）
		FormOrderで順次処理用

	引数
			BOOL	SortType	TRUE:ASC
								FALSE:DESC
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcInfSub::InitOrder(BOOL SortType/*=1*/)
{
	CString strSort;
	m_strFilter.Empty();
	if(SortType != FALSE)	strSort = _T("ASC");
	else					strSort = _T("DESC");

	m_strSort.Format("FormOrder %s", strSort);

	return RequeryWork();
}


/**********************************************************************
	RequeryFormSeq()
		初期化処理（Open()の代わりとして使用）
		様式シーケンス番号 指定バージョン

	引数
			BYTE	FormSeq		様式シーケンス番号
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcInfSub::RequeryFormSeq(int FormSeq)
{
	///////////////////////////////////////////////////////////////////
	// 目的のレコード（1レコード）を抽出する
	m_strFilter.Format("FormSeq = %d", FormSeq);
	m_strSort = "";

	return	RequeryWork();
}


/**********************************************************************
	RequeryFormOrder()
		初期化処理（Open()の代わりとして使用）
		様式並び順番号(表示インデックス) 指定バージョン

	引数
			BYTE	FormOrder	様式並び順番号
	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗
***********************************************************************/
int CdbUcInfSub::RequeryFormOrder(int FormOrder)
{
	///////////////////////////////////////////////////////////////////
	// 目的のレコード（1レコード）を抽出する
	m_strFilter.Format("FormOrder = %d", FormOrder);
	m_strSort = "";

	return	RequeryWork();
}

/**********************************************************************
	UpdateTitleTK()
		通常/個別の切り替え時に、タイトル変更のために呼び出す。

		指定された FormSeq がもつ FormNo と同じレコードのデータが
		更新対象になります。

		例) FormSeq=16 (⑭-1) の場合、FormNoが14なので、
			FormNo =14 のデータ（⑭-1、⑭-2）が更新対象になります。

	引数
			int			inFormSeq		FormSeq番号
			int			inTK			0：通常 / 1：個別 を指定
										ID_FORMTYPE_NORMAL = 0,	//	 0	通常
										ID_FORMTYPE_KOBETU 		//	 1	個別

	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗

***********************************************************************/
int CdbUcInfSub::UpdateTitleTK(int inFormSeq, int inTK)
{
	CString		Str1;
	CString		StrSql;
	
	// SQLコマンド作成
	Str1.Format("%d", inFormSeq);

	StrSql  = _T("UPDATE ") + m_lo_TableName + _T(" ");

	if (inTK == ID_FORMTYPE_NORMAL) {	// 0 通常
		StrSql += _T("SET Title = TitleSaveTujo ");
	}
	else {								// 1 個別		
		StrSql += _T("SET Title = TitleSave ");
	}

	StrSql += _T("WHERE FormNo IN ( ");
	StrSql += _T("SELECT FormNo ");
	StrSql +=	_T("FROM ") + m_lo_TableName + _T(" ");
	StrSql +=	_T("WHERE FormSeq =  ") + Str1 + _T(" ");
	StrSql +=	_T(") ");

	return ExecuteSQLWork(StrSql);
}

/**********************************************************************
	UpdateCurInfo()
		カレント位置情報をクリア（頁、行、列、コントロール番号）

	引数
			なし

	戻値
			int					DB_ERR_OK (0)     ：成功
								DB_ERR_OK (0) 以外：失敗

***********************************************************************/
int CdbUcInfSub::UpdateCurInfo()
{
	CString		StrSql;
	
	// SQLコマンド作成 ･･･ 抽出条件なし
	StrSql	=	_T("UPDATE ") + m_lo_TableName + _T(" ");
	StrSql	+=	_T("SET CurPage=1, CurRow=1, CurField=1, CurCtlIndex=0 ");

	return	ExecuteSQLWork(StrSql);
}
