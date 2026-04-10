#include	"stdafx.h"
#include	"dbPrtWork2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CdbPrtWork2, CRecordset)

CdbPrtWork2::CdbPrtWork2(CDatabase* pdb, int pFormSeq)
	: CdbBase(pdb)
{
	m_row		= 0;
	m_Seq		= 0;
	m_NumPage	= 0;
	m_NumRow	= 0;
	m_FgFunc	= 0;
	m_FgShow	= 0;

	m_nFormSeq	= pFormSeq;
#ifdef _DEBUG
	m_sTblName.Format(_T("##row_id_tbl_%d"),m_nFormSeq);
#else
	m_sTblName.Format(_T("#row_id_tbl_%d"),m_nFormSeq);
#endif

	m_nFields = 6;

	m_nDefaultType = snapshot;
}

CString CdbPrtWork2::GetDefaultSQL()
{
	return(m_sTblName);
}

void CdbPrtWork2::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[row]"), m_row);
	RFX_Long(pFX, _T("[Seq]"), m_Seq);
	RFX_Int(pFX, _T("[NumPage]"), m_NumPage);
	RFX_Byte(pFX, _T("[NumRow]"), m_NumRow);
	RFX_Byte(pFX, _T("[FgFunc]"), m_FgFunc);
	RFX_Byte(pFX, _T("[FgShow]"), m_FgShow);
}

/////////////////////////////////////////////////////////////////////////////
// CdbPrtWork2 診断

#ifdef _DEBUG
void CdbPrtWork2::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbPrtWork2::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

//******************************************************************************
// 新規レコードを作成、初期値を追加
// レコードセットには、新規レコードがセットされて戻ります
//******************************************************************************
int CdbPrtWork2::CreateNewRecord()
{
	CRecordset	rs(m_lo_pdb);
	CString		SqlStr;
	int			retVal = DB_ERR;

	/////////////////////////////////////////////////////////////////////
	// 新しいシーケンス番号を算出
	SqlStr =	_T("SELECT max(Seq) AS MaxSeq ");
	SqlStr +=	_T("FROM ") + m_sTblName + _T(" ");

	if ( ! OpenEx( &rs , SqlStr , m_sTblName ) ) {
		return DB_ERR_OPEN;
	};

	retVal = DB_ERR_EOF;
	if (!rs.IsEOF()) {
		CDBVariant	val;
		rs.GetFieldValue(_T("MaxSeq"), val);
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

	int	ret = RequeryWork();
	if ( ret != DB_ERR_OK) {
		return ret;
	}

	AddNew();

	m_row = 0;				// 01.行番号
	m_Seq = retVal;			// 02.シーケンス番号(自動付番)
	m_NumPage = 0;			// 03.ページ番号
	m_NumRow = 0;			// 04.行番号
	m_FgFunc = 0;			// 05.特殊行フラグ
	m_FgShow = 0;			// 06.表示フラグ

	Update();

	m_strFilter.Format( "Seq = %d", retVal );	// 削除フラグ OFF（0:有効なレコード)
	m_strSort   = "";							// 並び順でソート

	return RequeryWork();
}

//******************************************************************************
// 指定された表示フラグのレコードを頁番号/行で昇順番号してレコード抽出
//******************************************************************************
int CdbPrtWork2::RequeryFgShowSortPageRow(int inFgShow)
{
	m_strFilter.Format("FgShow = %d", inFgShow);
	m_strSort = "NumPage ASC, NumRow ASC, Seq DESC";

	return RequeryWork();		// リクエリを実行
}

//******************************************************************************
// 指定された条件のレコード抽出
//******************************************************************************
int CdbPrtWork2::RequeryPageRow(CString pRequery)
{
	m_strFilter = pRequery;
	m_strSort = _T("");

	return RequeryWork();		// リクエリを実行
}
