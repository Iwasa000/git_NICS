#include	"stdafx.h"
#include	"dbPrtWork3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CdbPrtWork3, CRecordset)

CdbPrtWork3::CdbPrtWork3(CDatabase* pdb,int pFormSeq, CString pstrTable)
	: CdbBase(pdb)
{
	m_row		= 0;
	m_Seq		= 0;
	m_NumPage	= 0;
	m_NumRow	= 0;
	m_FgFunc	= 0;
	m_FgShow	= 0;
	m_KeiStr	= _T("");
	m_NumGroup	= 0;
	m_KnSeq		= 0;
	m_KnOrder	= 0;
	m_KnName	= _T("");
	m_Val		= _T("");
	m_AdName1	= _T("");
	m_AdAdd2	= _T("");
	m_Risoku	= _T("");
	m_KnKana	= _T("");

	m_nFormSeq	= pFormSeq;
	m_sTblName = pstrTable;

	m_nFields = 13;			// 全様式共通のフィールド数は "13"

	switch(m_nFormSeq)	{
		case	ID_FORMNO_021:	// ②:受取手形の内訳書
		case	ID_FORMNO_081:	// ⑧:支払手形の内訳書	// 改良No.21-0086,21-0529 add
			m_nFields = m_nFields + 1;
			break;
		case	ID_FORMNO_111:	// ⑪:借入金及び支払利子の内訳書
			m_nFields = m_nFields + 2;
			break;
		default:
			break;
	}

	m_nDefaultType = snapshot;
}

CString CdbPrtWork3::GetDefaultSQL()
{
	return(m_sTblName);
}

void CdbPrtWork3::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[row]"), m_row);
	RFX_Long(pFX, _T("[Seq]"), m_Seq);
	RFX_Int(pFX, _T("[NumPage]"), m_NumPage);
	RFX_Byte(pFX, _T("[NumRow]"), m_NumRow);
	RFX_Byte(pFX, _T("[FgFunc]"), m_FgFunc);
	RFX_Byte(pFX, _T("[FgShow]"), m_FgShow);
	RFX_Text(pFX, _T("[KeiStr]"), m_KeiStr);
	RFX_Long(pFX, _T("[NumGroup]"), m_NumGroup);
	RFX_Long(pFX, _T("[KnSeq]"), m_KnSeq);
	RFX_Long(pFX, _T("[KnOrder]"), m_KnOrder);
	RFX_Text(pFX, _T("[KnName]"), m_KnName);
	RFX_Text(pFX, _T("[Val]"), m_Val);
	// 各様式個別のアイテム
	switch(m_nFormSeq)	{
		case	ID_FORMNO_021:	// ②:受取手形の内訳書
		case	ID_FORMNO_081:	// ⑧:支払手形の内訳書	// 改良No.21-0086,21-0529 add
			RFX_Text(pFX, _T("[AdName1]"), m_AdName1);
			break;
		case	ID_FORMNO_111:	// ⑪:借入金及び支払利子の内訳書
			RFX_Text(pFX, _T("[AdAdd2]"), m_AdAdd2);
			RFX_Text(pFX, _T("[Risoku]"), m_Risoku);
			break;
		default:
			break;
	}
// midori 152137 add -->
	RFX_Text(pFX, _T("[KnKana]"), m_KnKana);
// midori 152137 add <--
}

/////////////////////////////////////////////////////////////////////////////
// CdbPrtWork3 診断

#ifdef _DEBUG
void CdbPrtWork3::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbPrtWork3::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

//******************************************************************************
// ソートを実行（パラメータ可変）
//******************************************************************************
int CdbPrtWork3::RequerySortParameter(CString strFilter, CString strSort)
{
	m_strFilter = strFilter;
	m_strSort = strSort;

	return RequeryWork();		// リクエリを実行
}

//******************************************************************************
// 指定された表示フラグのレコードを頁番号/行で昇順番号してレコード抽出
//******************************************************************************
int CdbPrtWork3::RequeryFgShowSortPageRow(int inFgShow)
{
	m_strFilter.Format("FgShow = %d", inFgShow);
	m_strSort = "NumPage ASC, NumRow ASC, Seq DESC";

	return RequeryWork();		// リクエリを実行
}
