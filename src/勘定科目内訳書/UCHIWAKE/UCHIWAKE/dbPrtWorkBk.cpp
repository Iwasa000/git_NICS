#include	"stdafx.h"
#include	"dbPrtWorkBk.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CdbPrtWorkBk, CRecordset)

CdbPrtWorkBk::CdbPrtWorkBk(CDatabase* pdb, int pFormSeq)
	: CdbBase(pdb)
{
	m_BkOrder2 = 0;
	m_Seq = 0;
	m_FgFunc = 0;
	m_BkSeq = 0;
	m_BkOrder = 0;
	m_KnOrder = 0;
	m_BkName1 = _T("");
	m_BkName2 = _T("");
	m_BkKana1 = _T("");
	m_BkKana2 = _T("");

	m_nFormSeq	= pFormSeq;
#ifdef _DEBUG
	m_sTblName.Format(_T("##id_tbl2_%d"),m_nFormSeq);
#else
	m_sTblName.Format(_T("#id_tbl2_%d"),m_nFormSeq);
#endif

	m_nFields = 10;

	m_nDefaultType = snapshot;
}

CString CdbPrtWorkBk::GetDefaultSQL()
{
	return(m_sTblName);
}

void CdbPrtWorkBk::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[BkOrder2]"), m_BkOrder2);
	RFX_Long(pFX, _T("[Seq]"), m_Seq);
	RFX_Byte(pFX, _T("[FgFunc]"), m_FgFunc);
	RFX_Long(pFX, _T("[BkSeq]"), m_BkSeq);
	RFX_Long(pFX, _T("[BkOrder]"), m_BkOrder);
	RFX_Long(pFX, _T("[KnOrder]"), m_KnOrder);
	RFX_Text(pFX, _T("[BkName1]"), m_BkName1);
	RFX_Text(pFX, _T("[BkName2]"), m_BkName2);
	RFX_Text(pFX, _T("[BkKana1]"), m_BkKana1);
	RFX_Text(pFX, _T("[BkKana2]"), m_BkKana2);
}

/////////////////////////////////////////////////////////////////////////////
// CdbPrtWorkBk 診断

#ifdef _DEBUG
void CdbPrtWorkBk::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbPrtWorkBk::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

//******************************************************************************
// 番号順にデータを呼び出す
// nType				0:番号、頁順	0:番号順	1:50音順
// CString	strSort2	第２、第３ソート項目
//******************************************************************************
int CdbPrtWorkBk::RequerySort( int nType, CString strSort2 )
{
	if(nType == 0)	{
		m_strFilter	= _T("BkOrder > 0");
		m_strSort	= _T("BkOrder ASC,NumPage ASC, NumRow ASC");
	}
	else if(nType == 1)	{
		m_strFilter	= _T("BkOrder > 0");
		if(strSort2.IsEmpty() == TRUE)	{
			m_strSort	= _T("BkOrder ASC,(isnull(BkName1,'') + isnull(BkName2,'')) COLLATE Japanese_CS_AS_KS_WS ASC,FgFunc ASC,NumPage ASC, NumRow ASC");
		}
		else	{
			m_strSort	= _T("BkOrder ASC,(isnull(BkName1,'') + isnull(BkName2,'')) COLLATE Japanese_CS_AS_KS_WS ASC,FgFunc ASC,") + strSort2 + _T(",NumPage ASC, NumRow ASC");
		}
	}
	else	{
		m_strFilter	= _T("BkOrder > 0");
// midori UC_0008 del -->
		//if(strSort2.IsEmpty() == TRUE)	{
		//	m_strSort	= _T("(isnull(BkKana1,'ﾝﾝﾝﾝﾝﾝ') + isnull(BkKana2,'ﾝﾝﾝﾝﾝﾝ')) COLLATE Japanese_CS_AS_KS_WS ASC,FgFunc ASC");
		//}
		//else	{
		//	m_strSort	= _T("(isnull(BkKana1,'ﾝﾝﾝﾝﾝﾝ') + isnull(BkKana2,'ﾝﾝﾝﾝﾝﾝ')) COLLATE Japanese_CS_AS_KS_WS ASC,FgFunc ASC,") + strSort2;
		//}
// midori UC_0008 del <--
// midori UC_0008 add -->
		// すべての金融機関のカナが同じ場合、出力順序が
		// ｢りそな・Ａ支店｣ → ｢りそな・Ｂ支店｣ → ｢みずほ・Ａ支店｣ → ｢みずほ・Ｂ支店｣ → ｢小計｣ → ｢小計｣
		// となるので、FgFunc の前に BkOrder を追加しました
		m_strSort = _T("(isnull(BkKana1,'ﾝﾝﾝﾝﾝﾝ') + isnull(BkKana2,'ﾝﾝﾝﾝﾝﾝ')) COLLATE Japanese_CS_AS_KS_WS ASC,BkOrder ASC,FgFunc ASC");
		if(strSort2.IsEmpty() != TRUE)	{
			m_strSort = m_strSort + _T(",") + strSort2;
		}
// midori UC_0008 add <--
// midori 180537 add -->
		m_strSort += _T(",NumPage ASC, NumRow ASC");
// midori 180537 add <--
	}
	return RequeryWork();		// リクエリを実行
}

// midori 190505 add -->
//------------------------------------------------------------------------------
// 頁、行順にデータを呼び出す
// strFilter				WHERE文
//------------------------------------------------------------------------------
int CdbPrtWorkBk::RequerySort2(CString strFilter)
{
	m_strFilter	= strFilter;
	m_strSort	= _T("NumPage ASC, NumRow ASC");

	return RequeryWork();		// リクエリを実行
}
// midori 190505 add <--
