#include	"stdafx.h"
#include	"dbPrtWork.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern	BOOL	bG_InvNo;			// 改良No.21-0086,21-0529 add

IMPLEMENT_DYNAMIC(CdbPrtWork, CRecordset)

CdbPrtWork::CdbPrtWork(CDatabase* pdb, int pFormSeq)
	: CdbBase(pdb)
{
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
	m_Val2		= _T("");
	m_Val3		= _T("");
	m_AdName1	= _T("");
	m_HdName	= _T("");
	m_AdAdd2	= _T("");
	m_Risoku	= _T("");
	m_Naiyou	= _T("");
	m_Item		= _T("");
	m_FormSeq	= 0;
// midori 152137 add -->
	m_KnKana	= _T("");
// midori 152137 add <--
// midori 190505 add -->
	m_BkOrder	= 0;
	m_BkName1	= _T("");
	m_BkName2	= _T("");
	m_BkKana1	= _T("");
	m_BkKana2	= _T("");
	m_AdOrder	= 0;
	m_AdName2	= _T("");
	m_AdKana	= _T("");
	m_Syurui	= _T("");
	m_MsVal		= _T("");
	m_MsVal2	= _T("");
// midori 190505 add <--
// midori 157118 add -->
	m_ShowKeiZero = 0;
// midori 157118 add <--

	m_nFormSeq	= pFormSeq;
#ifdef _DEBUG
	m_sTblName.Format(_T("##temp_utiwake_tbl_%d"),m_nFormSeq);
#else
	m_sTblName.Format(_T("#temp_utiwake_tbl_%d"),m_nFormSeq);
#endif

// midori 190505 del -->
//// midori 152137 del -->
////	m_nFields = 11;			// 全様式共通のフィールド数は "10"
//// midori 152137 del <--
//// midori 152137 add -->
//	m_nFields = 12;			// 全様式共通のフィールド数は "12"
//// midori 152137 add <--
//	switch(m_nFormSeq)	{
//		case	ID_FORMNO_021:	// ②  :受取手形の内訳書
//		case	ID_FORMNO_031:	// ③  :売掛金の内訳書
//		case	ID_FORMNO_041:	// ④-1:仮払金の内訳書
//		case	ID_FORMNO_091:	// ⑨  :買掛金の内訳書
//		case	ID_FORMNO_101:	// ⑩-1:仮受金の内訳書
//			m_nFields = m_nFields + 1;
//			break;
//		case	ID_FORMNO_051:	// ⑤  :棚卸資産の内訳書
//			m_nFields = m_nFields + 1;
//			break;
//		case	ID_FORMNO_061:	// ⑥  :有価証券の内訳書
//			m_nFields = m_nFields + 2;
//			break;
//		case	ID_FORMNO_111:	// ⑪  :借入金及び支払利子の内訳書
//			m_nFields = m_nFields + 2;
//			break;
//		case	ID_FORMNO_121:	// ⑫  :土地の売上高等の内訳書
//			m_nFields = m_nFields + 1;
//			break;
//		case	ID_FORMNO_161:	// ⑯-1:雑益等の内訳書
//		case	ID_FORMNO_162:	// ⑯-2:雑益等の内訳書
//			m_nFields = m_nFields + 1;
//			break;
//		case	ID_FORMNO_171:	// ⑰  :その他の内訳書１
//		case	ID_FORMNO_172:
//		case	ID_FORMNO_173:
//		case	ID_FORMNO_174:
//		case	ID_FORMNO_175:
//		case	ID_FORMNO_176:
//		case	ID_FORMNO_177:
//		case	ID_FORMNO_178:
//		case	ID_FORMNO_179:
//		case	ID_FORMNO_1710:
//		case	ID_FORMNO_1711:
//		case	ID_FORMNO_1712:
//		case	ID_FORMNO_1713:
//		case	ID_FORMNO_1714:
//		case	ID_FORMNO_1715:
//		case	ID_FORMNO_1716:
//		case	ID_FORMNO_1717:
//		case	ID_FORMNO_1718:
//		case	ID_FORMNO_1719:
//		case	ID_FORMNO_1720:
//		case	ID_FORMNO_181:	// ⑱  :その他の内訳書２
//		case	ID_FORMNO_182:
//		case	ID_FORMNO_183:
//		case	ID_FORMNO_184:
//		case	ID_FORMNO_185:
//		case	ID_FORMNO_186:
//		case	ID_FORMNO_187:
//		case	ID_FORMNO_188:
//		case	ID_FORMNO_189:
//		case	ID_FORMNO_1810:
//		case	ID_FORMNO_1811:
//		case	ID_FORMNO_1812:
//		case	ID_FORMNO_1813:
//		case	ID_FORMNO_1814:
//		case	ID_FORMNO_1815:
//		case	ID_FORMNO_1816:
//		case	ID_FORMNO_1817:
//		case	ID_FORMNO_1818:
//		case	ID_FORMNO_1819:
//		case	ID_FORMNO_1820:
//			m_nFields = m_nFields + 2;
//			break;
//		default:
//			break;
//	}
// midori 190505 del <--
// midori 190505 add -->
// midori 157118 del -->
	//m_nFields = 7;			// 全様式共通のフィールド数は "7"
// midori 157118 del <--
// midori 157118 add -->
	m_nFields = 8;			// 全様式共通のフィールド数は "7"
// midori 157118 add <--
	// 科目指定が行えるまたは、第１ソート項目で改頁が可能な様式のみ
	switch(m_nFormSeq)	{
		case	ID_FORMNO_011:	// ①  :預貯金等の内訳書
			m_nFields = m_nFields + 10;
			break;
		case	ID_FORMNO_021:	// ②  :受取手形の内訳書
			m_nFields = m_nFields + 9;
			break;
		case	ID_FORMNO_031:	// ③  :売掛金の内訳書
			m_nFields = m_nFields + 6;
			break;
		case	ID_FORMNO_041:	// ④-1:仮払金の内訳書
			m_nFields = m_nFields + 6;
			break;
		case	ID_FORMNO_051:	// ⑤  :棚卸資産の内訳書
			m_nFields = m_nFields + 6;
			break;
		case	ID_FORMNO_061:	// ⑥  :有価証券の内訳書
			m_nFields = m_nFields + 7;
			break;
		case	ID_FORMNO_071:	// ⑦  :固定資産の内訳書
			m_nFields = m_nFields + 4;
			break;
		case	ID_FORMNO_081:	// ⑧  :支払手形の内訳書
// 改良No.21-0086,21-0529 cor -->
			//m_nFields = m_nFields + 5;
// ------------------------------
			if(bG_InvNo == TRUE)	m_nFields = m_nFields + 9;
			else					m_nFields = m_nFields + 5;
// 改良No.21-0086,21-0529 cor <--
			break;
		case	ID_FORMNO_091:	// ⑨  :買掛金の内訳書
			m_nFields = m_nFields + 6;
			break;
		case	ID_FORMNO_101:	// ⑩-1:仮受金の内訳書
			m_nFields = m_nFields + 6;
			break;
		case	ID_FORMNO_111:	// ⑪  :借入金及び支払利子の内訳書
			m_nFields = m_nFields + 8;
			break;
		case	ID_FORMNO_121:	// ⑫  :土地の売上高等の内訳書
			m_nFields = m_nFields + 6;
			break;
		case	ID_FORMNO_151:	// ⑮-1:地代家賃等の内訳書
			m_nFields = m_nFields + 5;
			break;
		case	ID_FORMNO_161:	// ⑯-1:雑益等の内訳書
		case	ID_FORMNO_162:	// ⑯-2:雑益等の内訳書
			m_nFields = m_nFields + 6;
			break;
		case	ID_FORMNO_171:	// ⑰  :その他の内訳書１
		case	ID_FORMNO_172:
		case	ID_FORMNO_173:
		case	ID_FORMNO_174:
		case	ID_FORMNO_175:
		case	ID_FORMNO_176:
		case	ID_FORMNO_177:
		case	ID_FORMNO_178:
		case	ID_FORMNO_179:
		case	ID_FORMNO_1710:
		case	ID_FORMNO_1711:
		case	ID_FORMNO_1712:
		case	ID_FORMNO_1713:
		case	ID_FORMNO_1714:
		case	ID_FORMNO_1715:
		case	ID_FORMNO_1716:
		case	ID_FORMNO_1717:
		case	ID_FORMNO_1718:
		case	ID_FORMNO_1719:
		case	ID_FORMNO_1720:
			m_nFields = m_nFields + 7;
			break;
		case	ID_FORMNO_181:	// ⑱  :その他の内訳書２
		case	ID_FORMNO_182:
		case	ID_FORMNO_183:
		case	ID_FORMNO_184:
		case	ID_FORMNO_185:
		case	ID_FORMNO_186:
		case	ID_FORMNO_187:
		case	ID_FORMNO_188:
		case	ID_FORMNO_189:
		case	ID_FORMNO_1810:
		case	ID_FORMNO_1811:
		case	ID_FORMNO_1812:
		case	ID_FORMNO_1813:
		case	ID_FORMNO_1814:
		case	ID_FORMNO_1815:
		case	ID_FORMNO_1816:
		case	ID_FORMNO_1817:
		case	ID_FORMNO_1818:
		case	ID_FORMNO_1819:
		case	ID_FORMNO_1820:
			m_nFields = m_nFields + 7;
			break;
		default:
			break;
	}
// midori 190505 add <--


	m_nDefaultType = snapshot;
}

CString CdbPrtWork::GetDefaultSQL()
{
	return(m_sTblName);
}

// midori 190505 del -->
//void CdbPrtWork::DoFieldExchange(CFieldExchange* pFX)
//{
//	pFX->SetFieldType(CFieldExchange::outputColumn);
//	RFX_Long(pFX, _T("[Seq]"), m_Seq);
//	RFX_Int(pFX, _T("[NumPage]"), m_NumPage);
//	RFX_Byte(pFX, _T("[NumRow]"), m_NumRow);
//	RFX_Byte(pFX, _T("[FgFunc]"), m_FgFunc);
//	RFX_Byte(pFX, _T("[FgShow]"), m_FgShow);
//	RFX_Text(pFX, _T("[KeiStr]"), m_KeiStr);
//	RFX_Long(pFX, _T("[NumGroup]"), m_NumGroup);
//	RFX_Long(pFX, _T("[KnSeq]"), m_KnSeq);
//	RFX_Long(pFX, _T("[KnOrder]"), m_KnOrder);
//	RFX_Text(pFX, _T("[KnName]"), m_KnName);
//	RFX_Text(pFX, _T("[Val]"), m_Val);
//	// 各様式個別のアイテム
//	switch(m_nFormSeq)	{
//		case	ID_FORMNO_021:	// ②  :受取手形の内訳書
//		case	ID_FORMNO_031:	// ③  :売掛金の内訳書
//		case	ID_FORMNO_041:	// ④-1:仮払金の内訳書
//		case	ID_FORMNO_091:	// ⑨  :買掛金の内訳書
//		case	ID_FORMNO_101:	// ⑩-1:仮受金の内訳書
//			RFX_Text(pFX, _T("[AdName1]"), m_AdName1);
//			break;
//		case	ID_FORMNO_061:	// ⑥  :有価証券の内訳書
//			RFX_Text(pFX, _T("[Val2]"), m_Val2);
//			RFX_Text(pFX, _T("[Val3]"), m_Val3);
//			break;
//		case	ID_FORMNO_051:	// ⑤  :棚卸資産の内訳書
//			RFX_Text(pFX, _T("[HdName]"), m_HdName);
//			break;
//		case	ID_FORMNO_111:	// ⑪  :借入金及び支払利子の内訳書
//			RFX_Text(pFX, _T("[AdAdd2]"), m_AdAdd2);
//			RFX_Text(pFX, _T("[Risoku]"), m_Risoku);
//			break;
//		case	ID_FORMNO_121:	// ⑫  :土地の売上高等の内訳書
//			RFX_Text(pFX, _T("[Val2]"), m_Val2);
//			break;
//		case	ID_FORMNO_161:	// ⑯-1:雑益等の内訳書
//		case	ID_FORMNO_162:	// ⑯-2:雑益等の内訳書
//			RFX_Text(pFX, _T("[Naiyou]"), m_Naiyou);
//			break;
//		case	ID_FORMNO_171:	// ⑰  :その他の内訳書１
//		case	ID_FORMNO_172:
//		case	ID_FORMNO_173:
//		case	ID_FORMNO_174:
//		case	ID_FORMNO_175:
//		case	ID_FORMNO_176:
//		case	ID_FORMNO_177:
//		case	ID_FORMNO_178:
//		case	ID_FORMNO_179:
//		case	ID_FORMNO_1710:
//		case	ID_FORMNO_1711:
//		case	ID_FORMNO_1712:
//		case	ID_FORMNO_1713:
//		case	ID_FORMNO_1714:
//		case	ID_FORMNO_1715:
//		case	ID_FORMNO_1716:
//		case	ID_FORMNO_1717:
//		case	ID_FORMNO_1718:
//		case	ID_FORMNO_1719:
//		case	ID_FORMNO_1720:
//			RFX_Text(pFX, _T("[AdName1]"), m_AdName1);
//			RFX_Int(pFX, _T("[FormSeq]"), m_FormSeq);
//			break;
//		case	ID_FORMNO_181:	// ⑱  :その他の内訳書２
//		case	ID_FORMNO_182:
//		case	ID_FORMNO_183:
//		case	ID_FORMNO_184:
//		case	ID_FORMNO_185:
//		case	ID_FORMNO_186:
//		case	ID_FORMNO_187:
//		case	ID_FORMNO_188:
//		case	ID_FORMNO_189:
//		case	ID_FORMNO_1810:
//		case	ID_FORMNO_1811:
//		case	ID_FORMNO_1812:
//		case	ID_FORMNO_1813:
//		case	ID_FORMNO_1814:
//		case	ID_FORMNO_1815:
//		case	ID_FORMNO_1816:
//		case	ID_FORMNO_1817:
//		case	ID_FORMNO_1818:
//		case	ID_FORMNO_1819:
//		case	ID_FORMNO_1820:
//			RFX_Text(pFX, _T("[Item]"), m_Item);
//			RFX_Int(pFX, _T("[FormSeq]"), m_FormSeq);
//			break;
//		default:
//			break;
//	}
//// midori 152137 add -->
//	RFX_Text(pFX, _T("[KnKana]"), m_KnKana);
//// midori 152137 add <--
//}
// midori 190505 del <--
// midori 190505 add -->
void CdbPrtWork::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[Seq]"), m_Seq);
	RFX_Int(pFX, _T("[NumPage]"), m_NumPage);
	RFX_Byte(pFX, _T("[NumRow]"), m_NumRow);
	RFX_Byte(pFX, _T("[FgFunc]"), m_FgFunc);
	RFX_Byte(pFX, _T("[FgShow]"), m_FgShow);
	RFX_Text(pFX, _T("[KeiStr]"), m_KeiStr);
	RFX_Long(pFX, _T("[NumGroup]"), m_NumGroup);		// 全様式共通 7個
// midori 157118 add -->
	RFX_Byte(pFX, _T("[ShowKeiZero]"), m_ShowKeiZero);
// midori 157118 add <--
	// 科目指定が行えるまたは、第１ソート項目で改頁が可能な様式のみ
	switch(m_nFormSeq)	{
		case	ID_FORMNO_011:	// ①  :預貯金等の内訳書
			RFX_Long(pFX, _T("[BkOrder]"), m_BkOrder);
			RFX_Text(pFX, _T("[BkName1]"), m_BkName1);
			RFX_Text(pFX, _T("[BkName2]"), m_BkName2);
			RFX_Long(pFX, _T("[KnSeq]"), m_KnSeq);
			RFX_Long(pFX, _T("[KnOrder]"), m_KnOrder);
			RFX_Text(pFX, _T("[KnName]"), m_KnName);
			RFX_Text(pFX, _T("[Val]"), m_Val);
			RFX_Text(pFX, _T("[BkKana1]"), m_BkKana1);
			RFX_Text(pFX, _T("[BkKana2]"), m_BkKana2);
			RFX_Text(pFX, _T("[KnKana]"), m_KnKana);	// 様式個別 10個
			break;
		case	ID_FORMNO_021:	// ②  :受取手形の内訳書
			RFX_Long(pFX, _T("[AdOrder]"), m_AdOrder);
			RFX_Text(pFX, _T("[AdName1]"), m_AdName1);
			RFX_Text(pFX, _T("[AdName2]"), m_AdName2);
			RFX_Long(pFX, _T("[KnOrder]"), m_KnOrder);
			RFX_Text(pFX, _T("[Val]"), m_Val);
			RFX_Long(pFX, _T("[KnSeq]"), m_KnSeq);
			RFX_Text(pFX, _T("[KnName]"), m_KnName);
			RFX_Text(pFX, _T("[AdKana]"), m_AdKana);
			RFX_Text(pFX, _T("[KnKana]"), m_KnKana);	// 様式個別  9個
			break;
		case	ID_FORMNO_031:	// ③  :売掛金の内訳書
			RFX_Long(pFX, _T("[KnSeq]"), m_KnSeq);
			RFX_Long(pFX, _T("[KnOrder]"), m_KnOrder);
			RFX_Text(pFX, _T("[KnName]"), m_KnName);
			RFX_Text(pFX, _T("[AdName1]"), m_AdName1);
			RFX_Text(pFX, _T("[Val]"), m_Val);
			RFX_Text(pFX, _T("[KnKana]"), m_KnKana);	// 様式個別  6個
			break;
		case	ID_FORMNO_041:	// ④-1:仮払金の内訳書
			RFX_Long(pFX, _T("[KnSeq]"), m_KnSeq);
			RFX_Long(pFX, _T("[KnOrder]"), m_KnOrder);
			RFX_Text(pFX, _T("[KnName]"), m_KnName);
			RFX_Text(pFX, _T("[AdName1]"), m_AdName1);
			RFX_Text(pFX, _T("[Val]"), m_Val);
			RFX_Text(pFX, _T("[KnKana]"), m_KnKana);	// 様式個別  6個
			break;
		case	ID_FORMNO_051:	// ⑤  :棚卸資産の内訳書
			RFX_Long(pFX, _T("[KnSeq]"), m_KnSeq);
			RFX_Long(pFX, _T("[KnOrder]"), m_KnOrder);
			RFX_Text(pFX, _T("[KnName]"), m_KnName);
			RFX_Text(pFX, _T("[HdName]"), m_HdName);
			RFX_Text(pFX, _T("[Val]"), m_Val);
			RFX_Text(pFX, _T("[KnKana]"), m_KnKana);	// 様式個別  6個
			break;
		case	ID_FORMNO_061:	// ⑥  :有価証券の内訳書
			RFX_Long(pFX, _T("[KnSeq]"), m_KnSeq);
			RFX_Long(pFX, _T("[KnOrder]"), m_KnOrder);
			RFX_Text(pFX, _T("[KnName]"), m_KnName);
			RFX_Text(pFX, _T("[Val]"), m_Val);
			RFX_Text(pFX, _T("[Val2]"), m_Val2);
			RFX_Text(pFX, _T("[Val3]"), m_Val3);
			RFX_Text(pFX, _T("[KnKana]"), m_KnKana);	// 様式個別  7個
			break;
		case	ID_FORMNO_071:	// ⑦  :固定資産の内訳書
			RFX_Text(pFX, _T("[Syurui]"), m_Syurui);
			RFX_Text(pFX, _T("[Val]"), m_Val);
			RFX_Text(pFX, _T("[MsVal]"), m_MsVal);
			RFX_Text(pFX, _T("[MsVal2]"), m_MsVal2);	// 様式個別  4個
			break;
		case	ID_FORMNO_081:	// ⑧  :支払手形の内訳書
			RFX_Long(pFX, _T("[AdOrder]"), m_AdOrder);
			RFX_Text(pFX, _T("[AdName1]"), m_AdName1);
			RFX_Text(pFX, _T("[AdName2]"), m_AdName2);
			RFX_Text(pFX, _T("[Val]"), m_Val);
			RFX_Text(pFX, _T("[AdKana]"), m_AdKana);	// 様式個別  5個
// 改良No.21-0086,21-0529 add -->
			if(bG_InvNo == TRUE) {
				RFX_Long(pFX, _T("[KnOrder]"), m_KnOrder);
				RFX_Long(pFX, _T("[KnSeq]"), m_KnSeq);
				RFX_Text(pFX, _T("[KnName]"), m_KnName);
				RFX_Text(pFX, _T("[KnKana]"), m_KnKana);	// 様式個別  9個
			}
// 改良No.21-0086,21-0529 add <--
			break;
		case	ID_FORMNO_091:	// ⑨  :買掛金の内訳書
			RFX_Long(pFX, _T("[KnSeq]"), m_KnSeq);
			RFX_Long(pFX, _T("[KnOrder]"), m_KnOrder);
			RFX_Text(pFX, _T("[KnName]"), m_KnName);
			RFX_Text(pFX, _T("[AdName1]"), m_AdName1);
			RFX_Text(pFX, _T("[Val]"), m_Val);
			RFX_Text(pFX, _T("[KnKana]"), m_KnKana);	// 様式個別  6個
			break;
		case	ID_FORMNO_101:	// ⑩-1:仮受金の内訳書
			RFX_Long(pFX, _T("[KnSeq]"), m_KnSeq);
			RFX_Long(pFX, _T("[KnOrder]"), m_KnOrder);
			RFX_Text(pFX, _T("[KnName]"), m_KnName);
			RFX_Text(pFX, _T("[AdName1]"), m_AdName1);
			RFX_Text(pFX, _T("[Val]"), m_Val);
			RFX_Text(pFX, _T("[KnKana]"), m_KnKana);	// 様式個別  6個
			break;
		case	ID_FORMNO_111:	// ⑪  :借入金及び支払利子の内訳書
			RFX_Long(pFX, _T("[KnSeq]"), m_KnSeq);
			RFX_Long(pFX, _T("[KnOrder]"), m_KnOrder);
			RFX_Text(pFX, _T("[KnName]"), m_KnName);
			RFX_Text(pFX, _T("[AdName1]"), m_AdName1);
			RFX_Text(pFX, _T("[AdAdd2]"), m_AdAdd2);
			RFX_Text(pFX, _T("[Val]"), m_Val);
			RFX_Text(pFX, _T("[Risoku]"), m_Risoku);
			RFX_Text(pFX, _T("[KnKana]"), m_KnKana);	// 様式個別  7個
			break;
		case	ID_FORMNO_121:	// ⑫  :土地の売上高等の内訳書
			RFX_Long(pFX, _T("[KnSeq]"), m_KnSeq);
			RFX_Long(pFX, _T("[KnOrder]"), m_KnOrder);
			RFX_Text(pFX, _T("[KnName]"), m_KnName);
			RFX_Text(pFX, _T("[Val]"), m_Val);
			RFX_Text(pFX, _T("[Val2]"), m_Val2);
			RFX_Text(pFX, _T("[KnKana]"), m_KnKana);	// 様式個別  6個
			break;
		case	ID_FORMNO_151:	// ⑮-1:地代家賃等の内訳書
			RFX_Long(pFX, _T("[KnSeq]"), m_KnSeq);
			RFX_Long(pFX, _T("[KnOrder]"), m_KnOrder);
			RFX_Text(pFX, _T("[KnName]"), m_KnName);
			RFX_Text(pFX, _T("[Val]"), m_Val);
			RFX_Text(pFX, _T("[KnKana]"), m_KnKana);	// 様式個別  5個
			break;
		case	ID_FORMNO_161:	// ⑯-1:雑益等の内訳書
		case	ID_FORMNO_162:	// ⑯-2:雑益等の内訳書
			RFX_Long(pFX, _T("[KnSeq]"), m_KnSeq);
			RFX_Long(pFX, _T("[KnOrder]"), m_KnOrder);
			RFX_Text(pFX, _T("[KnName]"), m_KnName);
			RFX_Text(pFX, _T("[Naiyou]"), m_Naiyou);
			RFX_Text(pFX, _T("[Val]"), m_Val);
			RFX_Text(pFX, _T("[KnKana]"), m_KnKana);	// 様式個別  6個
			break;
		case	ID_FORMNO_171:	// ⑰  :その他の内訳書１
		case	ID_FORMNO_172:
		case	ID_FORMNO_173:
		case	ID_FORMNO_174:
		case	ID_FORMNO_175:
		case	ID_FORMNO_176:
		case	ID_FORMNO_177:
		case	ID_FORMNO_178:
		case	ID_FORMNO_179:
		case	ID_FORMNO_1710:
		case	ID_FORMNO_1711:
		case	ID_FORMNO_1712:
		case	ID_FORMNO_1713:
		case	ID_FORMNO_1714:
		case	ID_FORMNO_1715:
		case	ID_FORMNO_1716:
		case	ID_FORMNO_1717:
		case	ID_FORMNO_1718:
		case	ID_FORMNO_1719:
		case	ID_FORMNO_1720:
			RFX_Int(pFX, _T("[FormSeq]"), m_FormSeq);
			RFX_Long(pFX, _T("[KnSeq]"), m_KnSeq);
			RFX_Long(pFX, _T("[KnOrder]"), m_KnOrder);
			RFX_Text(pFX, _T("[KnName]"), m_KnName);
			RFX_Text(pFX, _T("[AdName1]"), m_AdName1);
			RFX_Text(pFX, _T("[Val]"), m_Val);
			RFX_Text(pFX, _T("[KnKana]"), m_KnKana);	// 様式個別  7個
			break;
		case	ID_FORMNO_181:	// ⑱  :その他の内訳書２
		case	ID_FORMNO_182:
		case	ID_FORMNO_183:
		case	ID_FORMNO_184:
		case	ID_FORMNO_185:
		case	ID_FORMNO_186:
		case	ID_FORMNO_187:
		case	ID_FORMNO_188:
		case	ID_FORMNO_189:
		case	ID_FORMNO_1810:
		case	ID_FORMNO_1811:
		case	ID_FORMNO_1812:
		case	ID_FORMNO_1813:
		case	ID_FORMNO_1814:
		case	ID_FORMNO_1815:
		case	ID_FORMNO_1816:
		case	ID_FORMNO_1817:
		case	ID_FORMNO_1818:
		case	ID_FORMNO_1819:
		case	ID_FORMNO_1820:
			RFX_Int(pFX, _T("[FormSeq]"), m_FormSeq);
			RFX_Long(pFX, _T("[KnSeq]"), m_KnSeq);
			RFX_Long(pFX, _T("[KnOrder]"), m_KnOrder);
			RFX_Text(pFX, _T("[KnName]"), m_KnName);
			RFX_Text(pFX, _T("[Item]"), m_Item);
			RFX_Text(pFX, _T("[Val]"), m_Val);
			RFX_Text(pFX, _T("[KnKana]"), m_KnKana);	// 様式個別  7個
			break;
		default:
			break;
	}
}
// midori 190505 add <--

/////////////////////////////////////////////////////////////////////////////
// CdbPrtWork 診断

#ifdef _DEBUG
void CdbPrtWork::AssertValid() const
{
	CRecordset::AssertValid();
}

void CdbPrtWork::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

//******************************************************************************
// ソートで使用する "FgShow"項目、及び頁番号、行番号を更新
//******************************************************************************
int CdbPrtWork::UpdateFgShowPageRow(int nOldFgShow, int nNewFgShow, int nPage, int nRow)
{
	CString		str1 = "";		// UPDATE xxx SET FgShow=x
	CString		str2 = "";		// WHERE xxxxx
	CString		strCommand;
	
	// SQLコマンド作成
	str1.Format("UPDATE %s SET FgShow=%d, NumPage=%d, NumRow=%d", m_sTblName, nNewFgShow, nPage, nRow);

	str2.Format("WHERE (FgShow = %d)", nOldFgShow);

	strCommand = str1 + _T(" ") + str2;
	
	return	ExecuteSQLWork(strCommand);
}

//******************************************************************************
// ソートで使用する"FgShow"項目からグループ番号を０クリアする
//******************************************************************************
int CdbPrtWork::UpdateFgShowClearGroup(int nFgShow)
{
	CString			strCommand;
	
	// SQLコマンド作成
	strCommand.Format("UPDATE %s SET NumGroup=0 WHERE ", m_sTblName);

	// FormSeq対応(帳表 ⑰⑱その他１、２対応)
	CString	SqlStrSeq;
	SqlStrSeq.Format("FgShow=%d", nFgShow);
	strCommand += SqlStrSeq;
	
	return	ExecuteSQLWork(strCommand);
}

//******************************************************************************
// ソートを実行（パラメータ可変）
//******************************************************************************
int CdbPrtWork::RequerySortParameter(CString strFilter, CString strSort)
{
	m_strFilter = strFilter;
	m_strSort = strSort;

	return RequeryWork();		// リクエリを実行
}

//******************************************************************************
// フィールド名称を指定して値を取得する
//******************************************************************************
CString CdbPrtWork::GetFieldValueString(CString strValue)
{
	CDBVariant			val;
	CString				strRet;

	// パラメータチェック
	if (strValue == "") {
		return	"";
	}

	try{
		GetFieldValue(strValue, val);
		if (val.m_dwType == DBVT_NULL) {	// NULLだった場合
			return	"";
		}
		else {								// 値の取得OK
			// フィールドの型に応じて戻り値を返す
			switch (val.m_dwType) {
		case DBVT_UCHAR:
			strRet.Format("%d", val.m_chVal);			// char
			break;
		case DBVT_SHORT:
			strRet.Format("%d", val.m_iVal);			// short
			break;
		case DBVT_LONG:
			strRet.Format("%d", val.m_lVal);			// long
			break;
		case DBVT_STRING:
			strRet.Format("%s", val.m_pstring);			// CString
			break;
		case DBVT_ASTRING:
			strRet = *(CString*)val.m_pstringA;			// CStringA
			break;
		default:
			strRet = "";
			break;
			}
		}
	}
	catch(CDBException* e){
		// 何かすべき？

		// 解放
		e->Delete();
	}
	return	strRet;
}

//******************************************************************************
// 合計金額を取得（条件付き）
//******************************************************************************
int CdbPrtWork::GetSumValSub(int nFgShow, int nNumGroup, CString inFieldName, CString* pSumVal)
{
	CRecordset	rs(m_lo_pdb);
	CString		SqlStr;
	int			retVal = DB_ERR;
	CString		strWhere;

	char		prmSumVal[32];
	__int64		i64;
	BOOL		bCheckFlag = FALSE;

	*pSumVal = "";

	strWhere.Format("WHERE FgShow = %d AND NumGroup = %d", 
						nFgShow, nNumGroup);

	SqlStr =	_T("SELECT sum(") + inFieldName + _T(") AS SumVal ");
	SqlStr +=	_T("FROM ") + m_sTblName + _T(" ");
	SqlStr +=	strWhere;
	
	if ( ! OpenEx( &rs , SqlStr , m_sTblName ) ) {
		return DB_ERR_OPEN;
	};

	retVal = DB_ERR_EOF;
	if (!rs.IsEOF()) {
		CDBVariant	val;
		rs.GetFieldValue(_T("SumVal"), val);
		if (val.m_dwType == DBVT_NULL) {	// NULLだった場合
			*pSumVal = _T("0");
		}
		else {								// 値の取得OK
			*pSumVal = *val.m_pstring;

			// 値の範囲チェック：
			//	　__int64型でテーブルに格納できる範囲(14桁まで)をチェックする。
			//	　なお、SQL"sum関数"の戻りは"Decimal(38)"なので、sumの桁あふれは無視する

			// Step1: CString型→Char型→__int64型に変換
			ZeroMemory(prmSumVal, sizeof(prmSumVal));
			lstrcpy(prmSumVal, *pSumVal);					// CString型→Char型
			i64 = _strtoi64(prmSumVal, NULL, 10);			// Char型   →__int64型

			// Step2: 範囲チェック
			if (i64 < DB_DECIMAL_MIN) {
				i64 = DB_DECIMAL_MIN;
				bCheckFlag = TRUE;
			} else if (DB_DECIMAL_MAX < i64) {
				i64 = DB_DECIMAL_MAX;
				bCheckFlag = TRUE;
			}

			// Step3: 上限/下限値をセットする
			if (bCheckFlag == TRUE) {
				_i64toa_s(i64, prmSumVal, 32, 10);			// __int64型→Char型
				*pSumVal = prmSumVal;						// Char型   →CString型
			}
		}
	}

	rs.Close();

	return DB_ERR_OK;
}

//******************************************************************************
// 指定されたFgShowの値を持つレコード件数
//******************************************************************************
int CdbPrtWork::GetCountVal(int inFgShow, int inNumGroup)
{
	CRecordset	rs(m_lo_pdb);
	CString		SqlStr;
	CString		SqlStr1;
	CString		SqlStr2;
	CString		SqlStr3;
	CString		SqlStr4 = _T( "" );
	int			retVal = DB_ERR;

	SqlStr1 =	_T("SELECT count(Seq) AS Num ");
	SqlStr2 =	_T("FROM ") + m_sTblName + _T(" ");

	SqlStr3.Format("WHERE FgShow = %d AND NumGroup = %d", inFgShow, inNumGroup);

	SqlStr = SqlStr1 + SqlStr2 + SqlStr3 + SqlStr4;

	if ( ! OpenEx( &rs , SqlStr , m_sTblName ) ) {
		return DB_ERR_OPEN;
	};

	retVal = DB_ERR_EOF;
	if (!rs.IsEOF()) {
		CDBVariant	val;
		rs.GetFieldValue(_T("Num"), val);

		if (val.m_dwType == DBVT_NULL) {	// NULLだった場合
			retVal = 0;
		}
		else {								// 値の取得OK
			retVal = val.m_lVal;
		}
	}

	rs.Close();

	return retVal;
}

//******************************************************************************
// 新規レコードを作成、初期値を追加
// レコードセットには、新規レコードがセットされて戻ります
//******************************************************************************
int CdbPrtWork::CreateNewRecord()
{
	CRecordset	rs(m_lo_pdb);
	CString		SqlStr;
	int			retVal = DB_ERR;

	/////////////////////////////////////////////////////////////////////
	// 新しいシーケンス番号を算出
	SqlStr =	_T("SELECT max(Seq) AS MaxNum ");
	SqlStr +=	_T("FROM ") + m_sTblName + _T(" ");

	if ( ! OpenEx( &rs , SqlStr , m_sTblName ) ) {
		return DB_ERR_OPEN;
	};

	retVal = DB_ERR_EOF;
	if (!rs.IsEOF()) {
		CDBVariant	val;
		rs.GetFieldValue(_T("MaxNum"), val);
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
							
	m_Seq = retVal;			// 01.シーケンス番号(自動付番)
	m_NumPage = 0;			// 02.ページ番号
	m_NumRow = 0;			// 03.行番号
	m_FgFunc = 0;			// 04.特殊行フラグ
	m_FgShow = 0;			// 05.表示フラグ
	m_KeiStr = _T("");		// 06.「計」文言文字列格納用
	m_NumGroup = 0;			// 07.グループ番号
	m_KnSeq = 0;			// 08.科目 シーケンス番号
	m_KnOrder = 0;			// 09.科目 並び順
	m_KnName = _T("");		// 10.科目 名称
	m_Val = _T("");			// 11.金額
	m_AdName1 = _T("");		// 12.振出人等
	m_HdName = _T("");		// 13.品目
	m_AdAdd2 = _T("");		// 14.借入先（所在地２）
	m_Risoku = _T("");		// 15.期中の支払利子額
	m_Naiyou = _T("");		// 16.取引の内容
	m_Item = _T("");		// 17.項目
	m_FormSeq = m_nFormSeq;	// 18.その他の様式番号
// midori 152137 add -->
	m_KnKana = _T("");		// 19.科目 カナ
// midori 152137 add <--
// midori 190505 add -->
	m_BkOrder = 0;			// 20.金融機関 並び順
	m_BkName1 = _T("");		// 21.金融機関名
	m_BkName2 = _T("");		// 22.支店名
	m_BkKana1 = _T("");		// 23.金融機関名カナ
	m_BkKana2 = _T("");		// 24.支店名カナ
	m_AdOrder = 0;			// 25.支払(得意)先 並び順
	m_AdName2 = _T("");		// 26.支払(得意)先名　下段
	m_AdKana = _T("");		// 27.支払(得意)先名　カナ
	m_Syurui = _T("");		// 28.種類・構造
	m_MsVal = _T("");		// 29.取得(処分)価額
	m_MsVal2 = _T("");		// 30.異動直前の帳簿価額
// midori 190505 add <--
// midori 157118 add -->
	m_ShowKeiZero = 0;
// midori 157118 add <--

	Update();

	m_strFilter.Format( "Seq = %d", retVal );	// 削除フラグ OFF（0:有効なレコード)
	m_strSort   = "";							// 並び順でソート

	return RequeryWork();
}

//******************************************************************************
// 指定された表示フラグのレコードを頁番号/行で昇順番号してレコード抽出
//******************************************************************************
int CdbPrtWork::RequeryFgShowSortPageRow(int inFgShow)
{
	m_strFilter.Format("FgShow = %d", inFgShow);
	m_strSort = "NumPage ASC, NumRow ASC, Seq DESC";

	return RequeryWork();		// リクエリを実行
}

//******************************************************************************
// 最大ページ番号を取得（総ページ数）
//******************************************************************************
int CdbPrtWork::GetNumPage()
{
	CRecordset	rs(m_lo_pdb);
	CString		SqlStr;
	int			retVal = DB_ERR;

	SqlStr =	_T("SELECT max(NumPage) AS MaxNum ");
	SqlStr +=	_T("FROM ") + m_sTblName + _T(" ");

	if ( ! OpenEx( &rs , SqlStr , m_sTblName ) ) {
		return DB_ERR_OPEN;
	};

	retVal = DB_ERR_EOF;
	if (!rs.IsEOF()) {
		CDBVariant	val;
		rs.GetFieldValue(_T("MaxNum"), val);
		if (val.m_dwType == DBVT_NULL) {	// NULLだった場合
			retVal = 0;
		}
		else {								// 値の取得OK
			retVal = val.m_iVal;
		}
	}

	rs.Close();

	return retVal;
}

//******************************************************************************
// パラメータで指定された条件の行数を取得
//******************************************************************************
int CdbPrtWork::GetNumRowFgFuncFgShow(int inPage, int inFgFunc, int inFgShow)
{
	CRecordset	rs(m_lo_pdb);
	CString		SqlStr;
	CString		SqlStrSub;
	int			retVal = DB_ERR;

	SqlStrSub.Format("NumPage = %d AND FgFunc = %d AND FgShow = %d", inPage, inFgFunc, inFgShow);

	SqlStr =	_T("SELECT count(*) AS RowNum ");
	SqlStr +=	_T("FROM ") + m_sTblName + _T(" ");
	SqlStr +=	_T("WHERE ") + SqlStrSub;

	if ( ! OpenEx( &rs , SqlStr , m_sTblName ) ) {
		return DB_ERR_OPEN;
	};

	retVal = DB_ERR_EOF;
	if (!rs.IsEOF()) {
		CDBVariant	val;
		rs.GetFieldValue(_T("RowNum"), val);
		if (val.m_dwType == DBVT_NULL) {	// NULLだった場合
			retVal = 0;
		}
		else {								// 値の取得OK
			retVal = val.m_chVal;
		}
	}

	rs.Close();

	return retVal;
}

//******************************************************************************
// パラメータで指定された条件の行数を取得
//******************************************************************************
int CdbPrtWork::GetNumRowFgShow(int inPage, int FgShow)
{
	CRecordset	rs(m_lo_pdb);
	CString		SqlStr;
	CString		SqlStrSub;
	int			retVal = DB_ERR;

	SqlStrSub.Format("NumPage = %d AND FgShow = %d", inPage, FgShow);

	SqlStr =	_T("SELECT count(*) AS RowNum ");
	SqlStr +=	_T("FROM ") + m_sTblName + _T(" ");
	SqlStr +=	_T("WHERE ") + SqlStrSub;

	if ( ! OpenEx( &rs , SqlStr , m_sTblName ) ) {
		return DB_ERR_OPEN;
	};

	retVal = DB_ERR_EOF;
	if (!rs.IsEOF()) {
		CDBVariant	val;
		rs.GetFieldValue(_T("RowNum"), val);
		if (val.m_dwType == DBVT_NULL) {	// NULLだった場合
			retVal = 0;
		}
		else {								// 値の取得OK
			retVal = val.m_chVal;
		}
	}

	rs.Close();

	return retVal;
}

//******************************************************************************
// パラメータで指定された頁以降を削除
//******************************************************************************
int CdbPrtWork::DeletePage(int inPage)
{
	CString		SqlStr;		// SQL全文
	CString		SqlStrSub;	// SQL条件指定部

	SqlStrSub.Format("(NumPage >= %d)", inPage);

	SqlStr =	_T("DELETE FROM ") + m_sTblName + _T(" ");
	SqlStr +=	_T("WHERE ") + SqlStrSub;

	return	ExecuteSQLWork(SqlStr);
}

//******************************************************************************
// 各帳表の小計など集計処理で使用する集計結果のレコード更新
//******************************************************************************
int CdbPrtWork::UpdateCalcKei(int intSeq, CALCKEI_INFO uCalcInfo)
{
	CString			str1 = "";		// UPDATE xxx SET xxx=999, xxx=999
	CString			str2 = "";		// WHERE Seq = xx
	CString			str3 = "";		// , ShowKeiZero = xx
	CString			strTemp = "";
	CString			strCommand;
	int				i;
	int				nExistNum = 0;

	// フィールド名と集計値から更新情報を作成
	for (i = 0; i < uCalcInfo.intMaxCount; i++) {
		// 複数金額に対応
		nExistNum |= uCalcInfo.bExistNum[i] << i;

		if (strTemp != "") {
			strTemp = strTemp + ", ";
		}
		if (uCalcInfo.strKei[i] == "") {
			strTemp = strTemp + uCalcInfo.strField[i] + "=NULL";
		}
		else {
			strTemp = strTemp + uCalcInfo.strField[i] + "=" + uCalcInfo.strKei[i];
		}
		if( (i+1) == uCalcInfo.intMaxCount ){
			// ０円計表示
			//str3.Format( ", ShowKeiZero = %d", uCalcInfo.bExistNum[i] );
			str3.Format( ", ShowKeiZero = %d", nExistNum );
			strTemp = strTemp + str3;
		}
	}

	// SQLコマンド作成
	str1.Format("UPDATE %s SET %s", m_sTblName, strTemp);

	str2.Format("WHERE Seq = %d", intSeq);

	strCommand = str1 + _T(" ") + str2;
	
	return	ExecuteSQLWork(strCommand);
}

//******************************************************************************
// CalcSykeiAfterSort()
//******************************************************************************
BOOL CdbPrtWork::CalcSykeiAfterSort(CALCKEI_INFO uCalcInfo, CString strSortItem1, CString strSortItem2)
{
	CString strSQL;
	CString strSonota;

	// 一時テーブルが存在する場合は削除
	strSQL.Format(_T("if object_id('tempdb..#row_id_sykei_tbl', 'u') is not null drop table #row_id_sykei_tbl "
					 "if object_id('tempdb..#key_row_tbl', 'u') is not null drop table #key_row_tbl "));

	// 一意のページ数を無視した行番号(row)を振る
	strSQL.Format(strSQL + _T("SELECT ROW_NUMBER()OVER(ORDER BY NumPage,NumRow) as row,* "
							 "INTO #row_id_sykei_tbl "
							 "FROM %s "
							 "WHERE (FgShow = %d OR FgShow = %d) %s"),
							 GetDefaultSQL(), ID_FGSHOW_OFF, ID_FGSHOW_IKKATU, strSonota);

	// グループ化項目が以下の場合は@orderの型をintからvarchar(60)へ変更
	CString strOrder1Type;
	if( strSortItem1.Compare(_T("AdName1")) == 0 || strSortItem1.Compare(_T("Syurui")) == 0 || strSortItem1.Compare(_T("HdName")) == 0 ){
		strOrder1Type = _T("varchar(60)");

		// NULLを判断できないので、NULLを""(空白)に変換
		strSQL.Format(strSQL + _T("UPDATE #row_id_sykei_tbl SET %s = '' WHERE %s IS NULL AND FgFunc <> 22 "), strSortItem1, strSortItem1);
		if( !(strSortItem2.IsEmpty() )){
			strSQL.Format(strSQL + _T("UPDATE #row_id_sykei_tbl SET %s = '' WHERE %s IS NULL AND FgFunc <> 22 "), strSortItem2, strSortItem2);
		}
	}
	else{
		strOrder1Type = _T("int");
	}

	// 小計のみの共通テーブル作成
	strSQL.Format(strSQL + _T(";with "
							  "row_sykei_tbl(row) as ( "
							  "SELECT row "
							  "FROM #row_id_sykei_tbl "
							  "WHERE FgFunc = %d ),"), ID_FGFUNC_SYOKEI);

	// 小計の1行前のレコードのみの共通テーブル作成
	if( strSortItem2.IsEmpty() ){
		strSQL.Format(strSQL + _T("sykei_prev_tbl(key_order1, row, numgroup) as ( "
			"SELECT #row_id_sykei_tbl.%s, row_sykei_tbl.row, #row_id_sykei_tbl.NumGroup "
			"FROM row_sykei_tbl, #row_id_sykei_tbl "
			"WHERE row_sykei_tbl.row-1 = #row_id_sykei_tbl.row ) "),
			strSortItem1);
	}
	else{
		strSQL.Format(strSQL + _T("sykei_prev_tbl(key_order1, row, numgroup, key_order2) as ( "
			"SELECT #row_id_sykei_tbl.%s, row_sykei_tbl.row, #row_id_sykei_tbl.NumGroup, #row_id_sykei_tbl.%s "
			"FROM row_sykei_tbl, #row_id_sykei_tbl "
			"WHERE row_sykei_tbl.row-1 = #row_id_sykei_tbl.row ) "),
			strSortItem1, strSortItem2);
	}

	// 共通テーブルを一時テーブルにコピー
	strSQL.Format(strSQL + _T("SELECT * INTO #key_row_tbl FROM sykei_prev_tbl "));

	// 一括金額行にソート項目を反映(元データにはソート項目を持っていない)
	if( strSortItem2.IsEmpty() ){
		strSQL.Format(strSQL + _T("DECLARE cur cursor for "
			"SELECT row, NumGroup FROM #key_row_tbl "
			"DECLARE @numgroup int DECLARE @row int "
			"OPEN cur FETCH cur INTO @row, @numgroup WHILE @@fetch_status = 0 "
			"BEGIN "
				"UPDATE #key_row_tbl SET key_order1 = "
				"(SELECT DISTINCT %s "
				"FROM #row_id_sykei_tbl, #key_row_tbl "
				"WHERE #row_id_sykei_tbl.NumGroup = #key_row_tbl.NumGroup AND #key_row_tbl.NumGroup = @numgroup AND #row_id_sykei_tbl.FgShow = %d) "
				"WHERE row = @row AND numgroup <> 0 "
			"FETCH cur INTO @row, @numgroup "
			"END CLOSE cur DEALLOCATE cur "),
			strSortItem1, ID_FGSHOW_IKKATU);
	}
	else{
		strSQL.Format(strSQL + _T("DECLARE cur cursor for "
			"SELECT row, NumGroup FROM #key_row_tbl "
			"DECLARE @numgroup int DECLARE @row int "
			"OPEN cur FETCH cur INTO @row, @numgroup WHILE @@fetch_status = 0 "
			"BEGIN "
				"UPDATE #key_row_tbl SET key_order1 = "
				"(SELECT DISTINCT %s "
				"FROM #row_id_sykei_tbl, #key_row_tbl "
				"WHERE #row_id_sykei_tbl.NumGroup = #key_row_tbl.NumGroup AND #key_row_tbl.NumGroup = @numgroup AND #row_id_sykei_tbl.FgShow = %d) "
				"WHERE row = @row AND numgroup <> 0 "
				"UPDATE #key_row_tbl SET key_order2 = "
				"(SELECT DISTINCT %s "
				"FROM #row_id_sykei_tbl, #key_row_tbl "
				"WHERE #row_id_sykei_tbl.NumGroup = #key_row_tbl.NumGroup AND #key_row_tbl.NumGroup = @numgroup AND #row_id_sykei_tbl.FgShow = %d) "
				"WHERE row = @row AND numgroup <> 0 "
			"FETCH cur INTO @row, @numgroup "
			"END CLOSE cur DEALLOCATE cur "),
			strSortItem1, ID_FGSHOW_IKKATU, strSortItem2, ID_FGSHOW_IKKATU);
	}

	// 集計
	CString strUpdateTmpTbl;
	CString strUpdateBaseTbl;
	for(int i=0; i<uCalcInfo.intMaxCount; i++){
		// 一時テーブル用
		if( strSortItem2.IsEmpty() ){
			// サブ項目(strSortItem2)なし
			strUpdateTmpTbl.Format(strUpdateTmpTbl + 
			_T("UPDATE #row_id_sykei_tbl "
			"SET %s = (SELECT SUM(%s) FROM #row_id_sykei_tbl "
			"WHERE (%s = @order1 AND %s IS NOT NULL) AND (NumGroup = @numgroup OR NumGroup = 0) AND (FgShow = %d) "
			"GROUP BY %s) WHERE row = @row "),
			uCalcInfo.strField[i], uCalcInfo.strField[i], strSortItem1, uCalcInfo.strField[i], ID_FGSHOW_OFF, strSortItem1);
		}
		else{
			// サブ項目(strSortItem2)あり
			strUpdateTmpTbl.Format(strUpdateTmpTbl + 
			_T("UPDATE #row_id_sykei_tbl "
			"SET %s = (SELECT SUM(%s) FROM #row_id_sykei_tbl "
			"WHERE (%s = @order1 AND %s = @order2 AND %s IS NOT NULL) AND (NumGroup = @numgroup OR NumGroup = 0) AND (FgShow = %d) "
			"GROUP BY %s, %s ) WHERE row = @row "),
			uCalcInfo.strField[i], uCalcInfo.strField[i], strSortItem1, strSortItem2, 
			uCalcInfo.strField[i], ID_FGSHOW_OFF, strSortItem1, strSortItem2);
		}
		

		// 元テーブル用
		strUpdateBaseTbl.Format(strUpdateBaseTbl + 
			_T("UPDATE %s "
			"SET %s = #row_id_sykei_tbl.%s "
			"FROM #row_id_sykei_tbl "
			"WHERE %s.FgFunc = %d and %s.seq = #row_id_sykei_tbl.seq "),
			GetDefaultSQL(), uCalcInfo.strField[i], uCalcInfo.strField[i], GetDefaultSQL(), ID_FGFUNC_SYOKEI, GetDefaultSQL());
	}

	// 小計集計
	if( strSortItem2.IsEmpty()){
		// サブ項目(strSortItem2)なし
		strSQL.Format(strSQL + _T("DECLARE cur cursor for SELECT key_order1, row, numgroup FROM #key_row_tbl "
							  "DECLARE @order1 %s "
							  "OPEN cur FETCH cur INTO @order1, @row, @numgroup WHILE @@fetch_status = 0 "
							  "BEGIN "
							  "%s "
							  "FETCH cur INTO @order1, @row, @numgroup "
							  "END CLOSE cur DEALLOCATE cur "),
							  strOrder1Type, strUpdateTmpTbl);
	}
	else{
		// サブ項目(strSortItem2)あり
		strSQL.Format(strSQL + _T("DECLARE cur cursor for SELECT key_order1, row, numgroup, key_order2 FROM #key_row_tbl "
							  "DECLARE @order1 %s DECLARE @order2 varchar(60) "
							  "OPEN cur FETCH cur INTO @order1, @row, @numgroup, @order2 WHILE @@fetch_status = 0 "
							  "BEGIN "
							  "%s "
							  "FETCH cur INTO @order1, @row, @numgroup, @order2 "
							  "END CLOSE cur DEALLOCATE cur "),
							  strOrder1Type, strUpdateTmpTbl);
	}
	

	// 元のテーブルを更新
	strSQL.Format(strSQL + strUpdateBaseTbl);

	try{
		// SQL実行
		ExecuteSQLWork( strSQL );
	}
	catch(CException* e ){
		e->Delete();
	}

	return TRUE;
}

// midori 190505 add -->
// ----------------------------------------------------------------------------------------
//	UpdateFgFunc()
//		ソートで使用する "FgFunc"項目を更新
//		
//	引数
//			int					"FgFunc"の更新後の値
//			int					更新を行う"FgFunc"の値
//	戻値
//			int					DB_ERR_OK (0)     ：成功
//								DB_ERR_OK (0) 以外：失敗
// ----------------------------------------------------------------------------------------
int CdbPrtWork::UpdateFgFunc(int nFgFunc,int nFgFuncW)
{
	CString		strCommand;

	// SQLコマンド作成
	strCommand.Format("UPDATE %s SET FgFunc=%d WHERE FgFunc=%d", m_sTblName, nFgFunc, nFgFuncW);

	return	ExecuteSQLWork(strCommand);
}
// midori 190505 add <--
