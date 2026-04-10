#include	"stdafx.h"
#include	"dbPrtWorkAd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CdbPrtWorkAd, CRecordset)

// midori UC_0014 del-->
//CdbPrtWorkAd::CdbPrtWorkAd(CDatabase* pdb, int pFormSeq)
// midori UC_0014 del <--
// midori UC_0014 add-->
CdbPrtWorkAd::CdbPrtWorkAd(CDatabase* pdb, int pFormSeq, int tblId)
// midori UC_0014 add<--
	: CdbBase(pdb)
{
// midori UC_0014 del -->
//	m_AdOrder2 = 0;
//	m_Seq = 0;
//	m_FgFunc = 0;
//	m_AdSeq = 0;
//	m_AdOrder = 0;
//	m_AdName1 = _T("");
//	m_AdName2 = _T("");
//	m_AdKana = _T("");
//
//	m_nFormSeq	= pFormSeq;
//#ifdef _DEBUG
//	m_sTblName.Format(_T("##id_tbl2_%d"),m_nFormSeq);
//#else
//	m_sTblName.Format(_T("#id_tbl2_%d"),m_nFormSeq);
//#endif
//
//	m_nFields = 8;
// midori UC_0014 del <--
// midori UC_0014 add -->
	m_tblId = tblId;

	if(m_tblId == 2)	{
		m_AdOrder2 = 0;
	}
	m_Seq = 0;
	m_NumPage = 0;
	m_FgFunc = 0;
	m_NumGroup = 0;
	m_AdSeq = 0;
	m_AdOrder = 0;
	m_AdName1 = _T("");
	m_AdName2 = _T("");
	m_AdKana = _T("");

	m_nFormSeq	= pFormSeq;
#ifdef _DEBUG
	m_sTblName.Format(_T("##id_tbl%d_%d"),m_tblId,m_nFormSeq);
#else
	m_sTblName.Format(_T("#id_tbl%d_%d"),m_tblId,m_nFormSeq);
#endif

	m_nFields = 9;
	if(m_tblId == 2)	m_nFields += 1;
// midori UC_0014 add <--

	m_nDefaultType = snapshot;
}

CString CdbPrtWorkAd::GetDefaultSQL()
{
	return(m_sTblName);
}

void CdbPrtWorkAd::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// midori UC_0014 del -->
//	RFX_Long(pFX, _T("[AdOrder2]"), m_AdOrder2);
// midori UC_0014 del <--
// midori UC_0014 add -->
	if(m_tblId == 2)	{
		RFX_Long(pFX, _T("[AdOrder2]"), m_AdOrder2);
	}
// midori UC_0014 add <--
	RFX_Long(pFX, _T("[Seq]"), m_Seq);
// midori UC_0014 add -->
	RFX_Long(pFX, _T("[NumPage]"), m_NumPage);
// midori UC_0014 add <--
	RFX_Byte(pFX, _T("[FgFunc]"), m_FgFunc);
// midori UC_0014 add -->
	RFX_Long(pFX, _T("[NumGroup]"), m_NumGroup);
// midori UC_0014 add <--
	RFX_Long(pFX, _T("[AdSeq]"), m_AdSeq);
	RFX_Long(pFX, _T("[AdOrder]"), m_AdOrder);
	RFX_Text(pFX, _T("[AdName1]"), m_AdName1);
	RFX_Text(pFX, _T("[AdName2]"), m_AdName2);
	RFX_Text(pFX, _T("[AdKana]"), m_AdKana);
}

/////////////////////////////////////////////////////////////////////////////
// CdbPrtWorkAd 診断

#ifdef _DEBUG
void CdbPrtWorkAd::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbPrtWorkAd::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

//******************************************************************************
// 番号順にデータを呼び出す
// nType				0:番号、頁順	0:番号順	1:50音順
// CString	strSort2	第２、第３ソート項目
//******************************************************************************
int CdbPrtWorkAd::RequerySort( int nType, CString strSort2 )
{
	if(nType == 0)	{
// midori UC_0014 del -->
//		m_strFilter	= _T("AdOrder > 0");
// midori UC_0014 del <--
// midori UC_0014 add -->
		m_strFilter.Format(_T("AdOrder > 0 AND FgShow = %d"),ID_FGSHOW_OFF);
// midori UC_0014 add <--
		m_strSort	= _T("AdOrder ASC,NumPage ASC, NumRow ASC");
	}
	else if(nType == 1)	{
// midori UC_0014 del -->
//		m_strFilter	= _T("AdOrder > 0");
//		if(strSort2.IsEmpty() == TRUE)	{
//			m_strSort	= _T("AdOrder ASC,(isnull(AdName1,'') + isnull(AdName2,'')) COLLATE Japanese_CS_AS_KS_WS ASC, FgFunc ASC ,NumPage ASC, NumRow ASC");
//		}
//		else	{
//			m_strSort	= _T("AdOrder ASC,(isnull(AdName1,'') + isnull(AdName2,'')) COLLATE Japanese_CS_AS_KS_WS ASC, FgFunc ASC,") + strSort2 + _T(",NumPage ASC, NumRow ASC");
//		}
// midori UC_0014 del <--
// midori UC_0014 add -->
		m_strFilter.Format(_T("AdOrder > 0 AND FgShow = %d"),ID_FGSHOW_OFF);
		m_strFilter	= _T("");
		if(strSort2.IsEmpty() == TRUE)	{
			m_strSort	= _T("AdOrder ASC, FgFunc ASC,(isnull(AdName1,'') + isnull(AdName2,'')) COLLATE Japanese_CS_AS_KS_WS ASC ,NumPage ASC, NumRow ASC");
		}
		else	{
			m_strSort	= _T("AdOrder ASC, FgFunc ASC,(isnull(AdName1,'') + isnull(AdName2,'')) COLLATE Japanese_CS_AS_KS_WS ASC,") + strSort2 + _T(",NumPage ASC, NumRow ASC");
		}
// midori UC_0014 add <--
	}
	else	{
// midori UC_0014 del -->
//		m_strFilter	= _T("AdOrder > 0");
// midori UC_0014 del <--
// midori UC_0014 add -->
		m_strFilter.Format(_T("AdOrder > 0 AND FgShow = %d"),ID_FGSHOW_OFF);
// midori UC_0014 add <--
// midori 180537 del -->
		//if(strSort2.IsEmpty() == TRUE)	{
		//	m_strSort	= _T("isnull(AdKana,'ﾝﾝﾝﾝﾝﾝ') COLLATE Japanese_CS_AS_KS_WS ASC,AdOrder ASC,FgFunc ASC");
		//}
		//else	{
		//	m_strSort	= _T("isnull(AdKana,'ﾝﾝﾝﾝﾝﾝ') COLLATE Japanese_CS_AS_KS_WS ASC,AdOrder ASC,FgFunc ASC,") + strSort2;
		//}
// midori 180537 del <--
// midori 180537 add -->
		if(strSort2.IsEmpty() == TRUE)	{
			m_strSort	= _T("isnull(AdKana,'ﾝﾝﾝﾝﾝﾝ') COLLATE Japanese_CS_AS_KS_WS ASC,AdOrder ASC,FgFunc ASC, NumPage ASC, NumRow ASC");
		}
		else	{
			m_strSort	= _T("isnull(AdKana,'ﾝﾝﾝﾝﾝﾝ') COLLATE Japanese_CS_AS_KS_WS ASC,AdOrder ASC,FgFunc ASC,") + strSort2 + _T(",NumPage ASC, NumRow ASC");
		}
// midori 180537 add <--
	}
	return RequeryWork();		// リクエリを実行
}

// midori UC_0014 add -->
//------------------------------------------------------------------------------
// 頁、行順にデータを呼び出す
// strFilter				WHERE文
//------------------------------------------------------------------------------
int CdbPrtWorkAd::RequerySort2(CString strFilter)
{
	m_strFilter	= strFilter;
	m_strSort	= _T("NumPage ASC, NumRow ASC");

	return RequeryWork();		// リクエリを実行
}
// midori UC_0014 add <--
