// dbIcsVtbl.h : CdbIcsVtbl クラスの実装



// CdbIcsVtbl 実装


#include "stdafx.h"
#include "dbIcsVtbl.h"
IMPLEMENT_DYNAMIC(CdbIcsVtbl, CdbBase)

CdbIcsVtbl::CdbIcsVtbl(CDatabase* pdb)
	: CdbBase(pdb)
{
	m_type = 0;
	m_objname = "";
	m_vers = 0;
	m_nFields = 3;
	m_nDefaultType = snapshot;

	m_lo_pdb = pdb;								// データベースポインタを保持
	m_lo_TableName = "[dbo].[vtbl]";				// テーブル名を記述
	m_vers = -1;								// -1 に初期化
}

CString CdbIcsVtbl::GetDefaultSQL()
{
	return _T("[dbo].[vtbl]");
}

void CdbIcsVtbl::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() および RFX_Int() などのマクロは、データベースのフィールドの型ではなく、
// メンバ 変数の型に依存しています。
// ODBC は、列の値を要求された型に自動的に変換しようとします。
	RFX_Long(pFX, _T("[type]"), m_type);
	RFX_Text(pFX, _T("[objname]"), m_objname);
	RFX_Long(pFX, _T("[vers]"), m_vers);
}
/////////////////////////////////////////////////////////////////////////////
// CdbIcsVtbl 診断

#ifdef _DEBUG
void CdbIcsVtbl::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbIcsVtbl::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

/**********************************************************************
	VersionCheck()
		テーブルバージョンを調べる

	引数
			int			inType
			CString		inObjName
	戻値
			int			0以上：	バージョン取得成功
								バージョン番号が戻る
						0未満：	エラー
								DB_ERR_EOF：レコードが存在しない→内訳書テーブル作成
								上記以外は、DBエラー扱い
***********************************************************************/
int CdbIcsVtbl::VersionCheck(int inType, CString inObjName)
{
	int	retVal;

	m_strFilter.Format("type = %d AND objname='%s'", inType, inObjName);
	m_strSort   = "";

	retVal = RequeryWork();

	if (retVal == DB_ERR_OK) {
		if ( !IsEOF() ) {		// レコードあり
			retVal = m_vers;		// バージョンが異なる場合は、現在のバージョンを返す
		}
		else {					// レコードが無かった
			retVal = DB_ERR_EOF;		// EOFエラーを返す
		}
	}

	Fin();
	return retVal;
}

