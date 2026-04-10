#include "stdafx.h"
#include "H31SnTanaData.h"

static SH_TAGNAME tagDataTana1[] = {
	{ _T("AYB00010"), 0 },
	{ _T("AYB00020"), 0 },
	{ _T("AYB00030"), 0 },
	{ _T("AYB00040"), 0 },
	{ _T("AYB00050"), 0 },
	{ _T("AYB00060"), 0 },
	{ _T("AYB00070"), 1 },
	{ _T("AYB00080"), 1 },
	{ _T("AYB00090"), 1 },
	{ _T("AYB00100"), 1 },
	{ _T("AYB00110"), 1 },
	{ _T("AYB00120"), 1 },
	{ _T("AYB00130"), 3 },
	{ _T("AYB00140"), 3 },
	{ _T("AYB00150"), 3 },
	{ _T("AYB00160"), 3 },
	{ _T("AYB00170"), 3 },
	{ _T("AYB00180"), 3 },
	{ _T("AYC00010"), 1 },
	{ _T("AYC00020"), 1 },
	{ _T("AYC00030"), 1 },
	{ _T("AYC00040"), 1 },
	{ _T("AYC00050"), 1 },
	{ _T("AYC00060"), 1 },
	{ _T("AYC00070"), 3 },
	{ _T("AYC00080"), 3 },
	{ _T("AYC00090"), 3 },
	{ _T("AYC00100"), 3 },
	{ _T("AYC00110"), 3 },
	{ _T("AYC00120"), 3 },

	{ _T("") }

};

static SH_TAGNAME tagDataTana2[] = {
	{ _T("AXA00010"), 0 },
	{ _T("AXA00020"), 0 },
	{ _T("AXA00030"), 0 },
	{ _T("AXA00040"), 0 },
	{ _T("AXA00050"), 0 },
	{ _T("AXA00060"), 0 },
	{ _T("AXB00010"), 0 },
	{ _T("AXB00020"), 0 },
	{ _T("AXB00030"), 0 },
	{ _T("AXB00040"), 0 },

	{ _T("") }

};


CH31SnTanaData::CH31SnTanaData(BOOL isTransitionalMeasure )
	: m_IsIndiviAnbun(FALSE)
	, m_isTransitionalMeasure(isTransitionalMeasure)
{
	memset(Sn_MNK40, '\0', sizeof(Sn_MNK40));
	memset(Sn_MNZ40, '\0', sizeof(Sn_MNZ40));
	memset(Sn_MENZK, '\0', sizeof(Sn_MENZK));
	memset(Sn_MENZZ, '\0', sizeof(Sn_MENZZ));
	memset(Sn_MNK63, '\0', sizeof(Sn_MNK63));
	memset(Sn_MNZ63, '\0', sizeof(Sn_MNZ63));

	memset(Sn_mnk40, '\0', sizeof(Sn_mnk40));
	memset(Sn_mnz40, '\0', sizeof(Sn_mnz40));
	memset(Sn_menzk, '\0', sizeof(Sn_menzk));
	memset(Sn_menzz, '\0', sizeof(Sn_menzz));
	memset(Sn_mnk63, '\0', sizeof(Sn_mnk63));
	memset(Sn_mnz63, '\0', sizeof(Sn_mnz63));

	memset(Sn_MNK624, '\0', sizeof(Sn_mnk40));
	memset(Sn_MNZ624, '\0', sizeof(Sn_mnz40));
	memset(Sn_MNK78, '\0', sizeof(Sn_menzk));
	memset(Sn_MNZ78, '\0', sizeof(Sn_menzz));

	memset(Sn_mnk624, '\0', sizeof(Sn_mnk63));
	memset(Sn_mnz624, '\0', sizeof(Sn_mnz63));
	memset(Sn_mnk78, '\0', sizeof(Sn_mnk63));
	memset(Sn_mnz78, '\0', sizeof(Sn_mnz63));


	Sn_MENZsw = 0;
	Sn_tansw = 0;
	m_Arith.l_defn(0x16);

	memset(rsv1, '\0', sizeof(rsv1));
	memset(rsv2, '\0', sizeof(rsv2));
}


CH31SnTanaData::~CH31SnTanaData()
{
	m_DBSyzAccess.CloseShLstVal();
}


//-----------------------------------------------------------------------------
// DB Ç©ÇÁÉfÅ[É^ì«çûÇ›
//-----------------------------------------------------------------------------
// à¯êî	pDBNpSub	ÅF	ç‡ñ±ÉNÉâÉXÇÃÉ|ÉCÉìÉ^
//		sn_seq		ÅF	ëŒè€ÇÃóöóÉVÅ[ÉPÉìÉX
//-----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	ê≥èÌèIóπ
//			-1		ÅF	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CH31SnTanaData::GetData(CDBNpSub *pDBNpSub, int sn_seq)
{
	ASSERT(pDBNpSub);
	if (pDBNpSub == NULL) {
		return -1;
	}

	// É{ÉäÉÖÅ[ÉÄÉâÉxÉãÉfÅ[É^
	BOOL openFlg = FALSE;
	if (pDBNpSub->zvol == NULL) {
		if (pDBNpSub->VolumeOpen()) {
			return -1;
		}
		openFlg = TRUE;
	}
	if (pDBNpSub->zvol->s_sgn4 & 0x80) {
		m_IsIndiviAnbun = TRUE;
	}
	else {
		m_IsIndiviAnbun = FALSE;
	}
	if (openFlg) {
		pDBNpSub->VolumeClose();
		openFlg = FALSE;
	}

	// ê\çêèëÉfÅ[É^
	CString	filter;
	filter.Format(_T("Sn_SEQ = %d"), sn_seq);
	openFlg = FALSE;
	if (pDBNpSub->szsnk == NULL) {
		if (pDBNpSub->SzsnkOpen(filter)) {
			return -1;
		}
		openFlg = TRUE;
	}
	else {
		pDBNpSub->szsnk->Requery(filter);
	}

	if ((pDBNpSub->szsnk->MoveFirst() != -1) && (pDBNpSub->szsnk->st != -1)) {
		Sn_MENZsw = pDBNpSub->szsnk->Sn_MENZsw;
		Sn_tansw = pDBNpSub->szsnk->Sn_tansw;
	}

	if (openFlg) {
		pDBNpSub->SzsnkClose();
	}

	// ã‡äzÉfÅ[É^
	if (GetDataMonySub(pDBNpSub, sn_seq)) {
		return -1;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// DB Ç÷ÉfÅ[É^èëçûÇ›
//-----------------------------------------------------------------------------
// à¯êî	pDBNpSub	ÅF	ç‡ñ±ÉNÉâÉXÇÃÉ|ÉCÉìÉ^
//		sn_seq		ÅF	ëŒè€ÇÃóöóÉVÅ[ÉPÉìÉX
//-----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	ê≥èÌèIóπ
//			-1		ÅF	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CH31SnTanaData::SetData(CDBNpSub *pDBNpSub, int sn_seq)
{

	ASSERT(pDBNpSub);

	// ê\çêèëÉfÅ[É^
	CString	filter;
	filter.Format(_T("Sn_SEQ = %d"), sn_seq);
	BOOL openFlg = FALSE;
	if (pDBNpSub->szsnk == NULL) {
		if (pDBNpSub->SzsnkOpen(filter)) {
			return -1;
		}
		openFlg = TRUE;
	}
	else {
		pDBNpSub->szsnk->Requery(filter);
	}
	if ((pDBNpSub->szsnk->MoveFirst() != -1) && (pDBNpSub->szsnk->st != -1)) {
		pDBNpSub->szsnk->Edit();
		pDBNpSub->szsnk->Sn_MENZsw = Sn_MENZsw;
		pDBNpSub->szsnk->Sn_tansw = Sn_tansw;
		pDBNpSub->szsnk->Update();
	}
	else {
		// Ç±ÇÃíiäKÇ≈ê\çêèëÉfÅ[É^Ç™ñ≥Ç¢Ç±Ç∆ÇÕëzíËÇ∑Ç◊Ç´ÅHÅHÅHÅH
	}

	if (openFlg) {
		pDBNpSub->SzsnkClose();
		openFlg = FALSE;
	}

	// ã‡äzÉfÅ[É^
	if (SetDataMonySub(pDBNpSub, sn_seq)) {
		return -1;
	}


	return 0;
}

//-----------------------------------------------------------------------------
// DB Ç©ÇÁÉfÅ[É^ì«çûÇ›Åiã‡äzâ”èäÅj
//-----------------------------------------------------------------------------
// à¯êî	pDBNpSub	ÅF	ç‡ñ±ÉNÉâÉXÇÃÉ|ÉCÉìÉ^
//		sn_seq		ÅF	ëŒè€ÇÃóöóÉVÅ[ÉPÉìÉX
//-----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	ê≥èÌèIóπ
//			-1		ÅF	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CH31SnTanaData::GetDataMonySub(CDBNpSub *pDBNpSub, int sn_seq)
{
	CString	filter;

	//--->yoshida200107
	//ÉoÅ[ÉWÉáÉìå©ÇÍÇƒÇ»Ç¢ÇÃÇ≈èCê≥
	int nowVersion = 2;
	if (m_IsIndiviAnbun) {
		filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_TANA_KOBETU_HYOID, nowVersion);
	}
	else {
		filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_TANA_HIREI_HYOID, nowVersion);
	}
	//----

	//if (m_IsIndiviAnbun) {
	//	filter.Format(_T("sn_seq = %d and hyoId = \'%s\'"), sn_seq, ICS_SN_TANA_KOBETU_HYOID);
	//}
	//else {
	//	filter.Format(_T("sn_seq = %d and hyoId = \'%s\'"), sn_seq, ICS_SN_TANA_HIREI_HYOID);
	//}
	//<-----

	if (m_DBSyzAccess.m_pSH_LST_VAL) {
		m_DBSyzAccess.m_pSH_LST_VAL->Requery(filter);
	}
	else {
		if (m_DBSyzAccess.OpenShLstVal(pDBNpSub->m_database, filter)) {
			return -1;
		}
	}

	CSH_LST_VAL	*pShLstVal = m_DBSyzAccess.m_pSH_LST_VAL;
	if (m_IsIndiviAnbun) {
		for (int i = 0;; i++) {
			if (i == 0) {
				if ((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {
					break;
				}
			}
			else {
				if ((pShLstVal->MoveNext() == -1) || (pShLstVal->st == -1)) {
					break;
				}
			}

			// ã‡äzÇïœä∑ÇµÇƒë„ì¸
			if (pShLstVal->itmId == _T("AYB00010")) {		// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz(êVê≈ó¶ï™)ÅFâ€ê≈Ç…åWÇÈ
				m_Util.val_to_bin(Sn_menzk[0], pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AYB00020")) {	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz(êVê≈ó¶ï™)ÅFîÒâ€ê≈Ç…åWÇÈ
				m_Util.val_to_bin(Sn_menzk[1], pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AYB00030")) {	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz(êVê≈ó¶ï™)ÅFã§í Ç…åWÇÈ
				m_Util.val_to_bin(Sn_menzk[2], pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AYB00040")) {	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéY è¡îÔê≈äz(êVê≈ó¶ï™)ÅFâ€ê≈Ç…åWÇÈ
				m_Util.val_to_bin(Sn_menzz[0], pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AYB00050")) {	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéY è¡îÔê≈äz(êVê≈ó¶ï™)ÅFîÒâ€ê≈Ç…åWÇÈ
				m_Util.val_to_bin(Sn_menzz[1], pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AYB00060")) {	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéY è¡îÔê≈äz(êVê≈ó¶ï™)ÅFã§í Ç…åWÇÈ
				m_Util.val_to_bin(Sn_menzz[2], pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AYB00070")) {	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz(ãåê≈ó¶ï™)ÅFâ€ê≈Ç…åWÇÈ
				m_Util.val_to_bin(Sn_mnk40[0], pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AYB00080")) {	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz(ãåê≈ó¶ï™)ÅFîÒâ€ê≈Ç…åWÇÈ
				m_Util.val_to_bin(Sn_mnk40[1], pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AYB00090")) {	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz(ãåê≈ó¶ï™)ÅFã§í Ç…åWÇÈ
				m_Util.val_to_bin(Sn_mnk40[2], pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AYB00100")) {	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéY è¡îÔê≈äz(ãåê≈ó¶ï™)ÅFâ€ê≈Ç…åWÇÈ
				m_Util.val_to_bin(Sn_mnz40[0], pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AYB00110")) {	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéY è¡îÔê≈äz(ãåê≈ó¶ï™)ÅFîÒâ€ê≈Ç…åWÇÈ
				m_Util.val_to_bin(Sn_mnz40[1], pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AYB00120")) {	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéY è¡îÔê≈äz(ãåê≈ó¶ï™)ÅFã§í Ç…åWÇÈ
				m_Util.val_to_bin(Sn_mnz40[2], pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AYB00130")) {	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz(ãåê≈ó¶ï™)ÅFâ€ê≈Ç…åWÇÈ
				m_Util.val_to_bin(Sn_mnk63[0], pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AYB00140")) {	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz(ãåê≈ó¶ï™)ÅFîÒâ€ê≈Ç…åWÇÈ
				m_Util.val_to_bin(Sn_mnk63[1], pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AYB00150")) {	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz(ãåê≈ó¶ï™)ÅFã§í Ç…åWÇÈ
				m_Util.val_to_bin(Sn_mnk63[2], pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AYB00160")) {	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéY è¡îÔê≈äz(ãåê≈ó¶ï™)ÅFâ€ê≈Ç…åWÇÈ
				m_Util.val_to_bin(Sn_mnz63[0], pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AYB00170")) {	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéY è¡îÔê≈äz(ãåê≈ó¶ï™)ÅFîÒâ€ê≈Ç…åWÇÈ
				m_Util.val_to_bin(Sn_mnz63[1], pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AYB00180")) {	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéY è¡îÔê≈äz(ãåê≈ó¶ï™)ÅFã§í Ç…åWÇÈ
				m_Util.val_to_bin(Sn_mnz63[2], pShLstVal->val);
			}

			// ïtï\2-1
			else if (pShLstVal->itmId == _T("AYC00010")) {	// É^ÉOñºÇ™ï™Ç©ÇÁÇ»Ç©Ç¡ÇΩÇÃÇ≈âºÇ≈êUÇ¡ÇƒÇÈ
				m_Util.val_to_bin(Sn_mnk624[0], pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AYC00020")) {
				m_Util.val_to_bin(Sn_mnk624[1], pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AYC00030")) {
				m_Util.val_to_bin(Sn_mnk624[2], pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AYC00040")) {
				m_Util.val_to_bin(Sn_mnz624[0], pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AYC00050")) {
				m_Util.val_to_bin(Sn_mnz624[1], pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AYC00060")) {
				m_Util.val_to_bin(Sn_mnz624[2], pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AYC00070")) {
				m_Util.val_to_bin(Sn_mnk78[0], pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AYC00080")) {
				m_Util.val_to_bin(Sn_mnk78[1], pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AYC00090")) {
				m_Util.val_to_bin(Sn_mnk78[2], pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AYC00100")) {
				m_Util.val_to_bin(Sn_mnz78[0], pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AYC00110")) {
				m_Util.val_to_bin(Sn_mnz78[1], pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AYC00120")) {
				m_Util.val_to_bin(Sn_mnz78[2], pShLstVal->val);
			}
		}
	}
	else {
		for (int i = 0;; i++) {
			if (i == 0) {
				if ((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {
					break;
				}
			}
			else {
				if ((pShLstVal->MoveNext() == -1) || (pShLstVal->st == -1)) {
					break;
				}
			}
			// ã‡äzÇïœä∑ÇµÇƒë„ì¸
			if (pShLstVal->itmId == _T("AXA00010")) {		//íIâµí≤êÆäzÅ@â€ê≈Ç…åWÇÈ3Åì
				m_Util.val_to_bin(Sn_MENZK, pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AXA00020")) {	//íIâµí≤êÆê≈äzÅ@â€ê≈Ç…åWÇÈ3Åì 
				m_Util.val_to_bin(Sn_MENZZ, pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AXA00030")) {	//íIâµí≤êÆäzÅ@â€ê≈Ç…åWÇÈ4Åì
				m_Util.val_to_bin(Sn_MNK40, pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AXA00040")) {	//íIâµí≤êÆê≈äzÅ@â€ê≈Ç…åWÇÈ4Åì
				m_Util.val_to_bin(Sn_MNZ40, pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AXA00050")) {	//íIâµí≤êÆäzÅ@â€ê≈Ç…åWÇÈ6.3Åì
				m_Util.val_to_bin(Sn_MNK63, pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AXA00060")) {	//íIâµí≤êÆê≈äzÅ@â€ê≈Ç…åWÇÈ6.3Åì
				m_Util.val_to_bin(Sn_MNZ63, pShLstVal->val);
			}

			// ïtï\2-1
			else if (pShLstVal->itmId == _T("AXB00010")) {	// É^ÉOñºÇ™ï™Ç©ÇÁÇ»Ç©Ç¡ÇΩÇÃÇ≈âºÇ≈êUÇ¡ÇƒÇÈ
				m_Util.val_to_bin(Sn_MNK624, pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AXB00020")) {
				m_Util.val_to_bin(Sn_MNZ624, pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AXB00030")) {
				m_Util.val_to_bin(Sn_MNK78, pShLstVal->val);
			}
			else if (pShLstVal->itmId == _T("AXB00040")) {
				m_Util.val_to_bin(Sn_MNZ78, pShLstVal->val);
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// DB Ç÷ÉfÅ[É^èëçûÇ›Åiã‡äzâ”èäÅj
//-----------------------------------------------------------------------------
// à¯êî	pDBNpSub	ÅF	ç‡ñ±ÉNÉâÉXÇÃÉ|ÉCÉìÉ^
//		sn_seq		ÅF	ëŒè€ÇÃóöóÉVÅ[ÉPÉìÉX
//-----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	ê≥èÌèIóπ
//			-1		ÅF	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CH31SnTanaData::SetDataMonySub(CDBNpSub *pDBNpSub, int sn_seq)
{
	CString	filter;
	char	*pHyoID = NULL;
	int	nowVersion = 2;

	if (m_IsIndiviAnbun) {
		filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_TANA_KOBETU_HYOID, nowVersion );
		pHyoID = ICS_SN_TANA_KOBETU_HYOID;
	}
	else {
		filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_TANA_HIREI_HYOID, nowVersion );
		pHyoID = ICS_SN_TANA_HIREI_HYOID;
	}

	if (m_DBSyzAccess.m_pSH_LST_VAL) {
		m_DBSyzAccess.m_pSH_LST_VAL->Requery(filter);
	}
	else {
		if (m_DBSyzAccess.OpenShLstVal(pDBNpSub->m_database, filter)) {
			return -1;
		}
	}

	CSH_LST_VAL	*pShLstVal = m_DBSyzAccess.m_pSH_LST_VAL;
	if ((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {	// ÉfÅ[É^ñ≥Çµ
	
		if (m_IsIndiviAnbun) {
			// 
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AYB00010");
			m_Util.val_to_asci(&pShLstVal->val, Sn_menzk[0]);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AYB00020");
			m_Util.val_to_asci(&pShLstVal->val, Sn_menzk[1]);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AYB00030");
			m_Util.val_to_asci(&pShLstVal->val, Sn_menzk[2]);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AYB00040");
			m_Util.val_to_asci(&pShLstVal->val, Sn_menzz[0]);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AYB00050");
			m_Util.val_to_asci(&pShLstVal->val, Sn_menzz[1]);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AYB00060");
			m_Util.val_to_asci(&pShLstVal->val, Sn_menzz[2]);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AYB00070");
			m_Util.val_to_asci(&pShLstVal->val, Sn_mnk40[0]);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AYB00080");
			m_Util.val_to_asci(&pShLstVal->val, Sn_mnk40[1]);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AYB00090");
			m_Util.val_to_asci(&pShLstVal->val, Sn_mnk40[2]);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AYB00100");
			m_Util.val_to_asci(&pShLstVal->val, Sn_mnz40[0]);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AYB00110");
			m_Util.val_to_asci(&pShLstVal->val, Sn_mnz40[1]);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AYB00120");
			m_Util.val_to_asci(&pShLstVal->val, Sn_mnz40[2]);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AYB00130");
			m_Util.val_to_asci(&pShLstVal->val, Sn_mnk63[0]);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AYB00140");
			m_Util.val_to_asci(&pShLstVal->val, Sn_mnk63[1]);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AYB00150");
			m_Util.val_to_asci(&pShLstVal->val, Sn_mnk63[2]);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AYB00160");
			m_Util.val_to_asci(&pShLstVal->val, Sn_mnz63[0]);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AYB00170");
			m_Util.val_to_asci(&pShLstVal->val, Sn_mnz63[1]);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AYB00180");
			m_Util.val_to_asci(&pShLstVal->val, Sn_mnz63[2]);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();


			// ïtï\2-1
			//
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AYC00010");
			m_Util.val_to_asci(&pShLstVal->val, Sn_mnk624[0]);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			//
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AYC00020");
			m_Util.val_to_asci(&pShLstVal->val, Sn_mnk624[1]);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AYC00030");
			m_Util.val_to_asci(&pShLstVal->val, Sn_mnk624[2]);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AYC00040");
			m_Util.val_to_asci(&pShLstVal->val, Sn_mnz624[0]);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AYC00050");
			m_Util.val_to_asci(&pShLstVal->val, Sn_mnz624[1]);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AYC00060");
			m_Util.val_to_asci(&pShLstVal->val, Sn_mnz624[2]);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			//
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AYC00070");
			m_Util.val_to_asci(&pShLstVal->val, Sn_mnk78[0]);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			//
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AYC00080");
			m_Util.val_to_asci(&pShLstVal->val, Sn_mnk78[1]);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AYC00090");
			m_Util.val_to_asci(&pShLstVal->val, Sn_mnk78[2]);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AYC00100");
			m_Util.val_to_asci(&pShLstVal->val, Sn_mnz78[0]);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AYC00110");
			m_Util.val_to_asci(&pShLstVal->val, Sn_mnz78[1]);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AYC00120");
			m_Util.val_to_asci(&pShLstVal->val, Sn_mnz78[2]);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();


		}
		else {
			// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz(3%ï™)
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AXA00010");
			m_Util.val_to_asci(&pShLstVal->val, Sn_MENZK);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµ è¡îÔê≈äz(3%ï™)
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AXA00020");
			m_Util.val_to_asci(&pShLstVal->val, Sn_MENZZ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz(4%ï™)
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AXA00030");
			m_Util.val_to_asci(&pShLstVal->val, Sn_MNK40);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµ è¡îÔê≈äz(4%ï™)
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AXA00040");
			m_Util.val_to_asci(&pShLstVal->val, Sn_MNZ40);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz(6.3%ï™)
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AXA00050");
			m_Util.val_to_asci(&pShLstVal->val, Sn_MNK63);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµ è¡îÔê≈äz(6.3%ï™)
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AXA00060");
			m_Util.val_to_asci(&pShLstVal->val, Sn_MNZ63);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();


			// ïtï\2-1
			// 
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AXB00010");	// É^ÉOñºÇ™ï™Ç©ÇÁÇ»Ç©Ç¡ÇΩÇÃÇ≈âºÇ≈êUÇ¡ÇƒÇÈ
			m_Util.val_to_asci(&pShLstVal->val, Sn_MNK624);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AXB00020");
			m_Util.val_to_asci(&pShLstVal->val, Sn_MNZ624);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AXB00030");
			m_Util.val_to_asci(&pShLstVal->val, Sn_MNK78);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AXB00040");
			m_Util.val_to_asci(&pShLstVal->val, Sn_MNZ78);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

		}
	}
	else {
		CStringArray updTagAry;

		if (m_IsIndiviAnbun) {
			for (int i = 0;; i++) {
				if (i == 0) {
					if ((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {
						break;
					}
				}
				else {
					if ((pShLstVal->MoveNext() == -1) || (pShLstVal->st == -1)) {
						break;
					}
				}

				if (pShLstVal->itmId == _T("AYB00010")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_menzk[0]);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AYB00020")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_menzk[1]);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AYB00030")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_menzk[2]);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AYB00040")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_menzz[0]);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AYB00050")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_menzz[1]);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AYB00060")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_menzz[2]);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AYB00070")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_mnk40[0]);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AYB00080")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_mnk40[1]);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AYB00090")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_mnk40[2]);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AYB00100")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_mnz40[0]);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AYB00110")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_mnz40[1]);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AYB00120")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_mnz40[2]);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AYB00130")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_mnk63[0]);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AYB00140")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_mnk63[1]);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AYB00150")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_mnk63[2]);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AYB00160")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_mnz63[0]);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AYB00170")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_mnz63[1]);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AYB00180")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_mnz63[2]);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}

				// ïtï\2-1
				else if (pShLstVal->itmId == _T("AYC00010")) {	// É^ÉOñºÇ™ï™Ç©ÇÁÇÒÇÃÇ≈âºÇ≈êUÇ¡ÇƒÇÈ
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_mnk624[0]);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AYC00020")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_mnk624[1]);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AYC00030")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_mnk624[2]);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AYC00040")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_mnz624[0]);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AYC00050")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_mnz624[1]);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AYC00060")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_mnz624[2]);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}

				else if (pShLstVal->itmId == _T("AYC00070")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_mnk78[0]);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AYC00080")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_mnk78[1]);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AYC00090")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_mnk78[2]);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AYC00100")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_mnz78[0]);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AYC00110")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_mnz78[1]);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AYC00120")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_mnz78[2]);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
			}

			int adCnt = 0;
			for ( int i = 0; ; i++ ) {
				if ( strlen ( tagDataTana1[i].tagName ) == 0 ) {
					break;
				}
				adCnt++;
			}

			int updCnt = ( int )updTagAry.GetCount();
			if (adCnt != updCnt) {
				CArray<unsigned char*> MonyPrtAry;

				MonyPrtAry.Add(Sn_menzk[0]);
				MonyPrtAry.Add(Sn_menzk[1]);
				MonyPrtAry.Add(Sn_menzk[2]);
				MonyPrtAry.Add(Sn_menzz[0]);
				MonyPrtAry.Add(Sn_menzz[1]);
				MonyPrtAry.Add(Sn_menzz[2]);
				MonyPrtAry.Add(Sn_mnk40[0]);
				MonyPrtAry.Add(Sn_mnk40[1]);
				MonyPrtAry.Add(Sn_mnk40[2]);
				MonyPrtAry.Add(Sn_mnz40[0]);
				MonyPrtAry.Add(Sn_mnz40[1]);
				MonyPrtAry.Add(Sn_mnz40[2]);
				MonyPrtAry.Add(Sn_mnk63[0]);
				MonyPrtAry.Add(Sn_mnk63[1]);
				MonyPrtAry.Add(Sn_mnk63[2]);
				MonyPrtAry.Add(Sn_mnz63[0]);
				MonyPrtAry.Add(Sn_mnz63[1]);
				MonyPrtAry.Add(Sn_mnz63[2]);
				MonyPrtAry.Add(Sn_mnk624[0]);
				MonyPrtAry.Add(Sn_mnk624[1]);
				MonyPrtAry.Add(Sn_mnk624[2]);
				MonyPrtAry.Add(Sn_mnz624[0]);
				MonyPrtAry.Add(Sn_mnz624[1]);
				MonyPrtAry.Add(Sn_mnz624[2]);
				MonyPrtAry.Add(Sn_mnk78[0]);
				MonyPrtAry.Add(Sn_mnk78[1]);
				MonyPrtAry.Add(Sn_mnk78[2]);
				MonyPrtAry.Add(Sn_mnz78[0]);
				MonyPrtAry.Add(Sn_mnz78[1]);
				MonyPrtAry.Add(Sn_mnz78[2]);

				BOOL findFlg = FALSE;
				for (int i = 0; ; i++) {
					if (strlen(tagDataTana1[i].tagName) == 0) {
						break;
					}

					findFlg = FALSE;
					for (int j = 0; j < updCnt; j++) {
						if (updTagAry[j].CompareNoCase(tagDataTana1[j].tagName) == 0) {
							findFlg = TRUE;
							break;
						}

						if (findFlg == FALSE) {
							// äYìñÉ^ÉOÇÃí«â¡
							pShLstVal->AddNew();
							pShLstVal->sn_seq = sn_seq;
							pShLstVal->version = nowVersion;
							pShLstVal->hyoId = pHyoID;
							pShLstVal->itmId = tagDataTana1[i].tagName;
							m_Util.val_to_asci(&pShLstVal->val, MonyPrtAry[i]);
							pShLstVal->sign1 = 0;
							pShLstVal->Update();
						}
					}
				}
			}

		}
		else {
			for (int i = 0;; i++) {
				if (i == 0) {
					if ((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {
						break;
					}
				}
				else {
					if ((pShLstVal->MoveNext() == -1) || (pShLstVal->st == -1)) {
						break;
					}
				}
				if (pShLstVal->itmId == _T("AXA00010")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_MENZK);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AXA00020")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_MENZZ);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AXA00030")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_MNK40);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AXA00040")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_MNZ40);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AXA00050")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_MNK63);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AXA00060")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_MNZ63);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}

				// ïtï\2-1
				else if (pShLstVal->itmId == _T("AXB00010")) {	// É^ÉOñºÇ™ï™Ç©ÇÁÇ»Ç©Ç¡ÇΩÇÃÇ≈âºÇ≈êUÇ¡ÇƒÇÈ
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_MNK624);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AXB00020")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_MNZ624);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AXB00030")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_MNK78);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
				else if (pShLstVal->itmId == _T("AXB00040")) {
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_MNZ78);
					pShLstVal->Update();

					updTagAry.Add ( pShLstVal->itmId );
				}
			}

			int adCnt = 0;
			for ( int i = 0; ; i++ ) {
				if ( strlen ( tagDataTana2[i].tagName ) == 0 ) {
					break;
				}
				adCnt++;
			}

			int updCnt = ( int )updTagAry.GetCount();
			if (adCnt != updCnt) {
				CArray<unsigned char*> MonyPrtAry;

				MonyPrtAry.Add(Sn_MENZK);
				MonyPrtAry.Add(Sn_MENZZ);
				MonyPrtAry.Add(Sn_MNK40);
				MonyPrtAry.Add(Sn_MNZ40);
				MonyPrtAry.Add(Sn_MNK63);
				MonyPrtAry.Add(Sn_MNZ63);
				MonyPrtAry.Add(Sn_MNK624);
				MonyPrtAry.Add(Sn_MNZ624);
				MonyPrtAry.Add(Sn_MNK78);
				MonyPrtAry.Add(Sn_MNZ78);

				BOOL findFlg = FALSE;
				for (int i = 0; ; i++) {
					if (strlen(tagDataTana2[i].tagName) == 0) {
						break;
					}

					findFlg = FALSE;
					for (int j = 0; j < updCnt; j++) {
						if (updTagAry[j].CompareNoCase(tagDataTana2[i].tagName) == 0) {
							findFlg = TRUE;
							break;
						}

						if (findFlg == FALSE) {
							// äYìñÉ^ÉOÇÃí«â¡
							pShLstVal->AddNew();
							pShLstVal->sn_seq = sn_seq;
							pShLstVal->version = nowVersion;
							pShLstVal->hyoId = pHyoID;
							pShLstVal->itmId = tagDataTana2[i].tagName;
							m_Util.val_to_asci(&pShLstVal->val, MonyPrtAry[i]);
							pShLstVal->sign1 = 0;
							pShLstVal->Update();
						}
					}
				}
			}
		}
	}

	return 0;
}


//-----------------------------------------------------------------------------
// ç‡ñ±òAìÆ
//-----------------------------------------------------------------------------
// à¯êî	pDBNpSub	ÅF	
//		pSnHeadData	ÅF	
//		pSyzSyukei	ÅF	
//-----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	ê≥èÌèIóπ
//-----------------------------------------------------------------------------
int CH31SnTanaData::ConnectZmdata(CDBNpSub *pDBNpSub, CSnHeadData *pSnHeadData, CSyzSyukei *pSyzSyukei)
{
	ASSERT(pDBNpSub);
	ASSERT(pSnHeadData);
	ASSERT(pSyzSyukei);

	MoneyBasejagArray money;

	money = pSyzSyukei->GetShisanData(PR_SHIIRE);
	int max = (int)money.GetCount();
	int inmax = 0;
	int	flg = 0;
	int	idx = 0;
	
	char	WORK0_3[3][MONY_BUF_SIZE] = { 0 };
	char	WORK0_5[3][MONY_BUF_SIZE] = { 0 };
	char	WORK0_8[3][MONY_BUF_SIZE] = { 0 };
	char	WORK0_3z[3][MONY_BUF_SIZE] = { 0 };
	char	WORK0_5z[3][MONY_BUF_SIZE] = { 0 };
	char	WORK0_8z[3][MONY_BUF_SIZE] = { 0 };

	//--->yoshida190628
	char	WORK0_8k[3][MONY_BUF_SIZE] = { 0 };
	char	WORK0_10[3][MONY_BUF_SIZE] = { 0 };

	char	WORK0_8kz[3][MONY_BUF_SIZE] = { 0 };
	char	WORK0_10z[3][MONY_BUF_SIZE] = { 0 };
	//<----

	char	WORK1_3[3][MONY_BUF_SIZE] = { 0 };
	char	WORK1_5[3][MONY_BUF_SIZE] = { 0 };
	char	WORK1_8[3][MONY_BUF_SIZE] = { 0 };
	char	WORK1_3z[3][MONY_BUF_SIZE] = { 0 };
	char	WORK1_5z[3][MONY_BUF_SIZE] = { 0 };
	char	WORK1_8z[3][MONY_BUF_SIZE] = { 0 };

	//--->yoshida190628
	char	WORK1_8k[3][MONY_BUF_SIZE] = { 0 };
	char	WORK1_10[3][MONY_BUF_SIZE] = { 0 };

	char	WORK1_8kz[3][MONY_BUF_SIZE] = { 0 };
	char	WORK1_10z[3][MONY_BUF_SIZE] = { 0 };
	//<-----


	//ëOâÒÇÃíIâµédñÛì¸óÕóLñ≥ÉTÉCÉìï€ë∂
	unsigned char Old_Sn_tansw = 0x00;
	Old_Sn_tansw = Sn_tansw;

	Sn_tansw = 0x00;
	
	pSnHeadData->SVsign &= ~0x10;
	int shinsw = 0;
	if (pDBNpSub->zvol->s_sgn1 & 0xc0) {
		shinsw = 0;	// î≤Ç´Åuî≤Ç´ñîÇÕàÍäáê≈î≤Ç´Åv
	}
	else {
		pSnHeadData->SVsign |= 0x10;
		shinsw = 1;	// çûÇ›ÅuçûÇ›Åv
	}

	int	tanaCalq = 0;

	for (int i = 0; i < max; i++) {
		inmax = (int)money[i].GetCount();
		if (inmax == 0) {
			continue;
		}

		if (m_IsIndiviAnbun == FALSE) {

			if ((money[i][0].attr.shisan_gp_kubun == (char)0x84) && (money[i][0].attr.shisan_syukei_type == 1)) {

				if (money[i][0].attr.shisan_zino == 1) {
					if (shinsw) {	// çûÇ›
						m_Arith.l_add(WORK0_3[0], WORK0_3[0], money[i][2].arith);
					}
					else {
						m_Arith.l_add(WORK0_3[0], WORK0_3[0], money[i][0].arith);
					}
					m_Arith.l_add(WORK0_3z[0], WORK0_3z[0], money[i][4].arith);
					Sn_tansw |= 0x01;
				}
				else if (money[i][0].attr.shisan_zino == 0) {
					if (shinsw) {	// çûÇ›
						m_Arith.l_add(WORK0_5[0], WORK0_5[0], money[i][2].arith);
					}
					else {
						m_Arith.l_add(WORK0_5[0], WORK0_5[0], money[i][0].arith);
					}
					m_Arith.l_add(WORK0_5z[0], WORK0_5z[0], money[i][4].arith);
					Sn_tansw |= 0x01;
				}
				else if (money[i][0].attr.shisan_zino == 3) {
					if (shinsw) {	// çûÇ›
						m_Arith.l_add(WORK0_8[0], WORK0_8[0], money[i][2].arith);
					}
					else {
						m_Arith.l_add(WORK0_8[0], WORK0_8[0], money[i][0].arith);
					}
					m_Arith.l_add(WORK0_8z[0], WORK0_8z[0], money[i][4].arith);
					Sn_tansw |= 0x01;
				}

				//--->yoshida190628
				else if (money[i][0].attr.shisan_zino == 4) {
					if (shinsw) {	// çûÇ›
						m_Arith.l_add(WORK0_8k[0], WORK0_8k[0], money[i][2].arith);
					}
					else {
						m_Arith.l_add(WORK0_8k[0], WORK0_8k[0], money[i][0].arith);
					}
					m_Arith.l_add(WORK0_8kz[0], WORK0_8kz[0], money[i][4].arith);
					Sn_tansw |= 0x01;
				}

				else if (money[i][0].attr.shisan_zino == 5) {
					if (shinsw) {	// çûÇ›
						m_Arith.l_add(WORK0_10[0], WORK0_10[0], money[i][2].arith);
					}
					else {
						m_Arith.l_add(WORK0_10[0], WORK0_10[0], money[i][0].arith);
					}
					m_Arith.l_add(WORK0_10z[0], WORK0_10z[0], money[i][4].arith);
					Sn_tansw |= 0x01;
				}
				//<------

			}
			else if ((money[i][0].attr.shisan_gp_kubun == (char)0x85) && (money[i][0].attr.shisan_syukei_type == 1)) {
				if (money[i][0].attr.shisan_zino == 1) {
					if (shinsw) {	// çûÇ›
						m_Arith.l_add(WORK1_3[0], WORK1_3[0], money[i][2].arith);
					}
					else {
						m_Arith.l_add(WORK1_3[0], WORK1_3[0], money[i][0].arith);
					}
					m_Arith.l_add(WORK1_3z[0], WORK1_3z[0], money[i][4].arith);
					Sn_tansw |= 0x01;
				}
				else if (money[i][0].attr.shisan_zino == 0) {
					if (shinsw) {	// çûÇ›
						m_Arith.l_add(WORK1_5[0], WORK1_5[0], money[i][2].arith);
					}
					else {
						m_Arith.l_add(WORK1_5[0], WORK1_5[0], money[i][0].arith);
					}
					m_Arith.l_add(WORK1_5z[0], WORK1_5z[0], money[i][4].arith);
					Sn_tansw |= 0x01;
				}
				else if (money[i][0].attr.shisan_zino == 3) {
					if (shinsw) {	// çûÇ›
						m_Arith.l_add(WORK1_8[0], WORK1_8[0], money[i][2].arith);
					}
					else {
						m_Arith.l_add(WORK1_8[0], WORK1_8[0], money[i][0].arith);
					}
					m_Arith.l_add(WORK1_8z[0], WORK1_8z[0], money[i][4].arith);
					Sn_tansw |= 0x01;
				}

				//---->yoshida190628
				else if (money[i][0].attr.shisan_zino == 4) {
					if (shinsw) {	// çûÇ›
						m_Arith.l_add(WORK1_8k[0], WORK1_8k[0], money[i][2].arith);
					}
					else {
						m_Arith.l_add(WORK1_8k[0], WORK1_8k[0], money[i][0].arith);
					}
					m_Arith.l_add(WORK1_8kz[0], WORK1_8kz[0], money[i][4].arith);
					Sn_tansw |= 0x01;
				}

				else if (money[i][0].attr.shisan_zino == 5) {
					if (shinsw) {	// çûÇ›
						m_Arith.l_add(WORK1_10[0], WORK1_10[0], money[i][2].arith);
					}
					else {
						m_Arith.l_add(WORK1_10[0], WORK1_10[0], money[i][0].arith);
					}
					m_Arith.l_add(WORK1_10z[0], WORK1_10z[0], money[i][4].arith);
					Sn_tansw |= 0x01;
				}
				//<-----
			}
		}
		else {

			CString	hoge = money[i][0].name;

			if (money[i][0].name == _T("Åmâ€îÑÅEíIâµâ¡éZäzÅn")) {
				tanaCalq = 1;
			}
			else if (money[i][0].name == _T("ÅmîÒâ€îÑÅEíIâµâ¡éZäzÅn")) {
				tanaCalq = 2;
			}
			else if (money[i][0].name == _T("Åmã§í ÅEíIâµâ¡éZäzÅn")) {
				tanaCalq = 3;
			}
			else if (money[i][0].name == _T("Åmâ€îÑÅEíIâµå∏éZäzÅn")) {
				tanaCalq = 4;

			}
			else if (money[i][0].name == _T("ÅmîÒâ€îÑÅEíIâµå∏éZäzÅn")) {
				tanaCalq = 5;
			}
			else if (money[i][0].name == _T("Åmã§í ÅEíIâµå∏éZäzÅn")) {
				tanaCalq = 6;
			}
			else if (money[i][0].name == _T("ÅyíIâµâ¡éZäzÅz")) {
				tanaCalq = 0;
			}

			else if (money[i][0].name == _T("ÅyíIâµå∏éZäzÅz")) {
				tanaCalq = 0;
			}

			char naka = money[i][0].attr.shisan_uriage_shiire;

			if ((money[i][0].attr.shisan_gp_kubun == (char)0x84) && (money[i][0].attr.shisan_syukei_type == 1)) {



				if (tanaCalq == 1) {
					idx = 0;
				}
				else if (tanaCalq == 2) {
					idx = 1;
				}
				else if (tanaCalq == 3) {
					idx = 2;
				}
				else {
					continue;
				}
				if (money[i][0].attr.shisan_zino == 1) {
					if (shinsw) {	// çûÇ›
						m_Arith.l_add(WORK0_3[idx], WORK0_3[idx], money[i][2].arith);
					}
					else {
						m_Arith.l_add(WORK0_3[idx], WORK0_3[idx], money[i][0].arith);
					}
					m_Arith.l_add(WORK0_3z[idx], WORK0_3z[idx], money[i][4].arith);
					Sn_tansw |= 0x01;
				}
				else if (money[i][0].attr.shisan_zino == 0) {
					if (shinsw) {	// çûÇ›
						m_Arith.l_add(WORK0_5[idx], WORK0_5[idx], money[i][2].arith);
					}
					else {
						m_Arith.l_add(WORK0_5[idx], WORK0_5[idx], money[i][0].arith);
					}
					m_Arith.l_add(WORK0_5z[idx], WORK0_5z[idx], money[i][4].arith);
					Sn_tansw |= 0x01;
				}
				else if (money[i][0].attr.shisan_zino == 3) {
					if (shinsw) {	// çûÇ›
						m_Arith.l_add(WORK0_8[idx], WORK0_8[idx], money[i][2].arith);
					}
					else {
						m_Arith.l_add(WORK0_8[idx], WORK0_8[idx], money[i][0].arith);
					}
					m_Arith.l_add(WORK0_8z[idx], WORK0_8z[idx], money[i][4].arith);
					Sn_tansw |= 0x01;
				}

				//---->yoshida190628
				else if (money[i][0].attr.shisan_zino == 4) {
					if (shinsw) {	// çûÇ›
						m_Arith.l_add(WORK0_8k[idx], WORK0_8k[idx], money[i][2].arith);
					}
					else {
						m_Arith.l_add(WORK0_8k[idx], WORK0_8k[idx], money[i][0].arith);
					}
					m_Arith.l_add(WORK0_8kz[idx], WORK0_8kz[idx], money[i][4].arith);
					Sn_tansw |= 0x01;
				}
				else if (money[i][0].attr.shisan_zino == 5) {
					if (shinsw) {	// çûÇ›
						m_Arith.l_add(WORK0_10[idx], WORK0_10[idx], money[i][2].arith);
					}
					else {
						m_Arith.l_add(WORK0_10[idx], WORK0_10[idx], money[i][0].arith);
					}
					m_Arith.l_add(WORK0_10z[idx], WORK0_10z[idx], money[i][4].arith);
					Sn_tansw |= 0x01;
				}
				//<----
			}
			else if ((money[i][0].attr.shisan_gp_kubun == (char)0x85) && (money[i][0].attr.shisan_syukei_type == 1)) {
				if (tanaCalq == 4) {
					idx = 0;
				}
				else if (tanaCalq == 5) {
					idx = 1;
				}
				else if (tanaCalq == 6) {
					idx = 2;
				}
				else {
					continue;
				}

				if (money[i][0].attr.shisan_zino == 1) {
					if (shinsw) {	// çûÇ›
						m_Arith.l_add(WORK1_3[idx], WORK1_3[idx], money[i][2].arith);
					}
					else {
						m_Arith.l_add(WORK1_3[idx], WORK1_3[idx], money[i][0].arith);
					}
					m_Arith.l_add(WORK1_3z[idx], WORK1_3z[idx], money[i][4].arith);
					Sn_tansw |= 0x01;
				}
				else if (money[i][0].attr.shisan_zino == 0) {
					if (shinsw) {	// çûÇ›
						m_Arith.l_add(WORK1_5[idx], WORK1_5[idx], money[i][2].arith);
					}
					else {
						m_Arith.l_add(WORK1_5[idx], WORK1_5[idx], money[i][0].arith);
					}
					m_Arith.l_add(WORK1_5z[idx], WORK1_5z[idx], money[i][4].arith);
					Sn_tansw |= 0x01;
				}
				else if (money[i][0].attr.shisan_zino == 3) {
					if (shinsw) {	// çûÇ›
						m_Arith.l_add(WORK1_8[idx], WORK1_8[idx], money[i][2].arith);
					}
					else {
						m_Arith.l_add(WORK1_8[idx], WORK1_8[idx], money[i][0].arith);
					}
					m_Arith.l_add(WORK1_8z[idx], WORK1_8z[idx], money[i][4].arith);
					Sn_tansw |= 0x01;
				}
				//--->yoshida190628
				else if (money[i][0].attr.shisan_zino == 4) {
					if (shinsw) {	// çûÇ›
						m_Arith.l_add(WORK1_8k[idx], WORK1_8k[idx], money[i][2].arith);
					}
					else {
						m_Arith.l_add(WORK1_8k[idx], WORK1_8k[idx], money[i][0].arith);
					}
					m_Arith.l_add(WORK1_8kz[idx], WORK1_8kz[idx], money[i][4].arith);
					Sn_tansw |= 0x01;
				}
				else if (money[i][0].attr.shisan_zino == 5) {
					if (shinsw) {	// çûÇ›
						m_Arith.l_add(WORK1_10[idx], WORK1_10[idx], money[i][2].arith);
					}
					else {
						m_Arith.l_add(WORK1_10[idx], WORK1_10[idx], money[i][0].arith);
					}
					m_Arith.l_add(WORK1_10z[idx], WORK1_10z[idx], money[i][4].arith);
					Sn_tansw |= 0x01;
				}
				//<----

			}
		}
	}

	if (m_IsIndiviAnbun == FALSE) {

		//íIâµÇµédñÛÇÃî≠ê∂Ç™Ç†ÇÈèÍçáÇÃÇ›ÉZÉbÉgÇ∑ÇÈ
		if (Sn_tansw & 0x01 || (Old_Sn_tansw != Sn_tansw)) {

			Sn_MENZsw = 0x00;

			// íIâµäzÇÃåvéZ
			char	WORK0[MONY_BUF_SIZE] = { 0 };
			char	WORK1[MONY_BUF_SIZE] = { 0 };

			m_Arith.l_add(WORK0, WORK0_3[0], WORK0_5[0]);
			m_Arith.l_add(WORK0, WORK0, WORK0_8[0]);
			//--->yoshida190628
			m_Arith.l_add(WORK0, WORK0, WORK0_8k[0]);
			m_Arith.l_add(WORK0, WORK0, WORK0_10[0]);
			//<----

			m_Arith.l_add(WORK1, WORK1_3[0], WORK1_5[0]);
			m_Arith.l_add(WORK1, WORK1, WORK1_8[0]);
			//--->yoshida190628
			m_Arith.l_add(WORK1, WORK1, WORK1_8k[0]);
			m_Arith.l_add(WORK1, WORK1, WORK1_10[0]);
			//<---

			if ((m_Arith.l_test(WORK0) == 0) && (m_Arith.l_test(WORK1) == 0)) {
				Sn_MENZsw = 0x00;
				
				// ÉfÅ[É^ÉNÉäÉA
				memset(Sn_MENZK, '\0', sizeof(Sn_MENZK));
				memset(Sn_MENZZ, '\0', sizeof(Sn_MENZZ));
				memset(Sn_MNK40, '\0', sizeof(Sn_MNK40));
				memset(Sn_MNZ40, '\0', sizeof(Sn_MNZ40));
				memset(Sn_MNK63, '\0', sizeof(Sn_MNK63));
				memset(Sn_MNZ63, '\0', sizeof(Sn_MNZ63));
				//-->yoshida190628
				memset(Sn_MNK624, '\0', sizeof(Sn_MNK624));
				memset(Sn_MNZ624, '\0', sizeof(Sn_MNZ624));
				memset(Sn_MNK78, '\0', sizeof(Sn_MNK78));
				memset(Sn_MNZ78, '\0', sizeof(Sn_MNZ78));
				//<----
				
				return 0;
			}

			int st = m_Arith.l_cmp(WORK0, WORK1);
			if (st) {
				Sn_MENZsw |= 0x10;
				if (st > 0) {
					// íIâµí≤êÆâ¡éZ
					;
				}
				else if (st < 0) {
					// íIâµí≤êÆå∏éZ
					Sn_MENZsw |= 0x01;
				}
			}
			else {
				Sn_MENZsw = 0x00;
				return 0;
			}

			// ÉfÅ[É^ÉNÉäÉA
			memset(Sn_MENZK, '\0', sizeof(Sn_MENZK));
			memset(Sn_MENZZ, '\0', sizeof(Sn_MENZZ));
			memset(Sn_MNK40, '\0', sizeof(Sn_MNK40));
			memset(Sn_MNZ40, '\0', sizeof(Sn_MNZ40));
			memset(Sn_MNK63, '\0', sizeof(Sn_MNK63));
			memset(Sn_MNZ63, '\0', sizeof(Sn_MNZ63));

			//-->yoshida190628
			memset(Sn_MNK624, '\0', sizeof(Sn_MNK624));
			memset(Sn_MNZ624, '\0', sizeof(Sn_MNZ624));
			memset(Sn_MNK78, '\0', sizeof(Sn_MNK78));
			memset(Sn_MNZ78, '\0', sizeof(Sn_MNZ78));
			//<----


			if (Sn_MENZsw & 0x01) {	// íIâµå∏éZ
				// 3%
				m_Arith.l_add(Sn_MENZK, Sn_MENZK, WORK1_3);
				m_Arith.l_sub(Sn_MENZK, Sn_MENZK, WORK0_3);
				m_Arith.l_add(Sn_MENZZ, Sn_MENZZ, WORK1_3z);
				m_Arith.l_sub(Sn_MENZZ, Sn_MENZZ, WORK0_3z);
				// 5%
				m_Arith.l_add(Sn_MNK40, Sn_MNK40, WORK1_5);
				m_Arith.l_sub(Sn_MNK40, Sn_MNK40, WORK0_5);
				m_Arith.l_add(Sn_MNZ40, Sn_MNZ40, WORK1_5z);
				m_Arith.l_sub(Sn_MNZ40, Sn_MNZ40, WORK0_5z);
				// 8%
				m_Arith.l_add(Sn_MNK63, Sn_MNK63, WORK1_8);
				m_Arith.l_sub(Sn_MNK63, Sn_MNK63, WORK0_8);
				m_Arith.l_add(Sn_MNZ63, Sn_MNZ63, WORK1_8z);
				m_Arith.l_sub(Sn_MNZ63, Sn_MNZ63, WORK0_8z);

				//---->yoshida190628
				// 8%åyå∏
				m_Arith.l_add(Sn_MNK624, Sn_MNK624, WORK1_8k);
				m_Arith.l_sub(Sn_MNK624, Sn_MNK624, WORK0_8k);
				m_Arith.l_add(Sn_MNZ624, Sn_MNZ624, WORK1_8kz);
				m_Arith.l_sub(Sn_MNZ624, Sn_MNZ624, WORK0_8kz);

				// 10Åì
				m_Arith.l_add(Sn_MNK78, Sn_MNK78, WORK1_10);
				m_Arith.l_sub(Sn_MNK78, Sn_MNK78, WORK0_10);
				m_Arith.l_add(Sn_MNZ78, Sn_MNZ78, WORK1_10z);
				m_Arith.l_sub(Sn_MNZ78, Sn_MNZ78, WORK0_10z);
				//<----

			}
			else {					// íIâµâ¡éZ
				// 3%
				m_Arith.l_add(Sn_MENZK, Sn_MENZK, WORK0_3);
				m_Arith.l_sub(Sn_MENZK, Sn_MENZK, WORK1_3);
				m_Arith.l_add(Sn_MENZZ, Sn_MENZZ, WORK0_3z);
				m_Arith.l_sub(Sn_MENZZ, Sn_MENZZ, WORK1_3z);
				// 5%
				m_Arith.l_add(Sn_MNK40, Sn_MNK40, WORK0_5);
				m_Arith.l_sub(Sn_MNK40, Sn_MNK40, WORK1_5);
				m_Arith.l_add(Sn_MNZ40, Sn_MNZ40, WORK0_5z);
				m_Arith.l_sub(Sn_MNZ40, Sn_MNZ40, WORK1_5z);
				// 8%
				m_Arith.l_add(Sn_MNK63, Sn_MNK63, WORK0_8);
				m_Arith.l_sub(Sn_MNK63, Sn_MNK63, WORK1_8);
				m_Arith.l_add(Sn_MNZ63, Sn_MNZ63, WORK0_8z);
				m_Arith.l_sub(Sn_MNZ63, Sn_MNZ63, WORK1_8z);

				//---->yoshida190628
				// 8%åyå∏
				m_Arith.l_add(Sn_MNK624, Sn_MNK624, WORK0_8k);
				m_Arith.l_sub(Sn_MNK624, Sn_MNK624, WORK1_8k);
				m_Arith.l_add(Sn_MNZ624, Sn_MNZ624, WORK0_8kz);
				m_Arith.l_sub(Sn_MNZ624, Sn_MNZ624, WORK1_8kz);

				// 10Åì
				m_Arith.l_add(Sn_MNK78, Sn_MNK78, WORK0_10);
				m_Arith.l_sub(Sn_MNK78, Sn_MNK78, WORK1_10);
				m_Arith.l_add(Sn_MNZ78, Sn_MNZ78, WORK0_10z);
				m_Arith.l_sub(Sn_MNZ78, Sn_MNZ78, WORK1_10z);
				//<----

			}
		}
	}
	else {
		// íIâµäzÇÃåvéZ

		//íIâµÇµédñÛÇÃî≠ê∂Ç™Ç†ÇÈèÍçáÇÃÇ›ÉZÉbÉgÇ∑ÇÈ
		if (Sn_tansw & 0x01 || (Old_Sn_tansw != Sn_tansw)) {

			Sn_MENZsw = 0x00;

			char	WORK0[3][MONY_BUF_SIZE] = { 0 };
			char	WORK1[3][MONY_BUF_SIZE] = { 0 };
			char	sumWork0[MONY_BUF_SIZE] = { 0 };
			char	sumWork1[MONY_BUF_SIZE] = { 0 };

			for (int i = 0; i < 3; i++) {
				m_Arith.l_add(WORK0[i], WORK0_3[i], WORK0_5[i]);
				m_Arith.l_add(WORK0[i], WORK0[i], WORK0_8[i]);
				//--->yoshida190628
				m_Arith.l_add(WORK0[i], WORK0[i], WORK0_8k[i]);
				m_Arith.l_add(WORK0[i], WORK0[i], WORK0_10[i]);
				//<----
				m_Arith.l_add(sumWork0, sumWork0, WORK0[i]);

				m_Arith.l_add(WORK1[i], WORK1_3[i], WORK1_5[i]);
				m_Arith.l_add(WORK1[i], WORK1[i], WORK1_8[i]);
				//--->yoshida190628
				m_Arith.l_add(WORK1[i], WORK1[i], WORK1_8k[i]);
				m_Arith.l_add(WORK1[i], WORK1[i], WORK1_10[i]);
				//<----
				m_Arith.l_add(sumWork1, sumWork1, WORK1[i]);
			}

			if (((m_Arith.l_test(WORK0[0]) == 0) && (m_Arith.l_test(WORK1[0]) == 0)) &&
				((m_Arith.l_test(WORK0[1]) == 0) && (m_Arith.l_test(WORK1[1]) == 0)) &&
				((m_Arith.l_test(WORK0[2]) == 0) && (m_Arith.l_test(WORK1[2]) == 0))) {
				Sn_MENZsw = 0x00;
				
				// ÉfÅ[É^ÉNÉäÉA
				for (int i = 0; i < 3; i++) {
					memset(Sn_menzk[i], '\0', sizeof(Sn_menzk[i]));
					memset(Sn_menzz[i], '\0', sizeof(Sn_menzz[i]));
					memset(Sn_mnk40[i], '\0', sizeof(Sn_mnk40[i]));
					memset(Sn_mnz40[i], '\0', sizeof(Sn_mnz40[i]));
					memset(Sn_mnk63[i], '\0', sizeof(Sn_mnk63[i]));
					memset(Sn_mnz63[i], '\0', sizeof(Sn_mnz63[i]));
					//-->yoshida190628
					memset(Sn_mnk624[i], '\0', sizeof(Sn_mnk624[i]));
					memset(Sn_mnz624[i], '\0', sizeof(Sn_mnz624[i]));
					memset(Sn_mnk78[i], '\0', sizeof(Sn_mnk78[i]));
					memset(Sn_mnz78[i], '\0', sizeof(Sn_mnz78[i]));
					//<----
				}
				
				return 0;
			}


			int st = m_Arith.l_cmp(sumWork0, sumWork1);
			if (st) {
				Sn_MENZsw |= 0x10;
				if (st > 0) {
					// íIâµí≤êÆâ¡éZ
					;
				}
				else if (st < 0) {
					// íIâµí≤êÆå∏éZ
					Sn_MENZsw |= 0x01;
				}
				pSnHeadData->SVzchg = Sn_MENZsw;
			}
			else {
				Sn_MENZsw = 0x00;
				return 0;
			}

			// ÉfÅ[É^ÉNÉäÉA
			for (int i = 0; i < 3; i++) {
				memset(Sn_menzk[i], '\0', sizeof(Sn_menzk[i]));
				memset(Sn_menzz[i], '\0', sizeof(Sn_menzz[i]));
				memset(Sn_mnk40[i], '\0', sizeof(Sn_mnk40[i]));
				memset(Sn_mnz40[i], '\0', sizeof(Sn_mnz40[i]));
				memset(Sn_mnk63[i], '\0', sizeof(Sn_mnk63[i]));
				memset(Sn_mnz63[i], '\0', sizeof(Sn_mnz63[i]));
				//--->yoshida190628
				memset(Sn_mnk624[i], '\0', sizeof(Sn_mnk624[i]));
				memset(Sn_mnz624[i], '\0', sizeof(Sn_mnz624[i]));
				memset(Sn_mnk78[i], '\0', sizeof(Sn_mnk78[i]));
				memset(Sn_mnz78[i], '\0', sizeof(Sn_mnz78[i]));
				//<----
			}

			if (Sn_MENZsw & 0x01) {	// íIâµå∏éZ
				for (int i = 0; i < 3; i++) {
					// 3%
					m_Arith.l_add(Sn_menzk[i], Sn_menzk[i], WORK1_3[i]);
					m_Arith.l_sub(Sn_menzk[i], Sn_menzk[i], WORK0_3[i]);
					m_Arith.l_add(Sn_menzz[i], Sn_menzz[i], WORK1_3z[i]);
					m_Arith.l_sub(Sn_menzz[i], Sn_menzz[i], WORK0_3z[i]);
					// 5%
					m_Arith.l_add(Sn_mnk40[i], Sn_mnk40[i], WORK1_5[i]);
					m_Arith.l_sub(Sn_mnk40[i], Sn_mnk40[i], WORK0_5[i]);
					m_Arith.l_add(Sn_mnz40[i], Sn_mnz40[i], WORK1_5z[i]);
					m_Arith.l_sub(Sn_mnz40[i], Sn_mnz40[i], WORK0_5z[i]);
					// 8%
					m_Arith.l_add(Sn_mnk63[i], Sn_mnk63[i], WORK1_8[i]);
					m_Arith.l_sub(Sn_mnk63[i], Sn_mnk63[i], WORK0_8[i]);
					m_Arith.l_add(Sn_mnz63[i], Sn_mnz63[i], WORK1_8z[i]);
					m_Arith.l_sub(Sn_mnz63[i], Sn_mnz63[i], WORK0_8z[i]);

					//--->yoshida190628
					// 8%åyå∏
					m_Arith.l_add(Sn_mnk624[i], Sn_mnk624[i], WORK1_8k[i]);
					m_Arith.l_sub(Sn_mnk624[i], Sn_mnk624[i], WORK0_8k[i]);
					m_Arith.l_add(Sn_mnz624[i], Sn_mnz624[i], WORK1_8kz[i]);
					m_Arith.l_sub(Sn_mnz624[i], Sn_mnz624[i], WORK0_8kz[i]);

					// 10%
					m_Arith.l_add(Sn_mnk78[i], Sn_mnk78[i], WORK1_10[i]);
					m_Arith.l_sub(Sn_mnk78[i], Sn_mnk78[i], WORK0_10[i]);
					m_Arith.l_add(Sn_mnz78[i], Sn_mnz78[i], WORK1_10z[i]);
					m_Arith.l_sub(Sn_mnz78[i], Sn_mnz78[i], WORK0_10z[i]);
					//<----
				}
			}
			else {					// íIâµâ¡éZ
				for (int i = 0; i < 3; i++) {
					// 3%
					m_Arith.l_add(Sn_menzk[i], Sn_menzk[i], WORK0_3[i]);
					m_Arith.l_sub(Sn_menzk[i], Sn_menzk[i], WORK1_3[i]);
					m_Arith.l_add(Sn_menzz[i], Sn_menzz[i], WORK0_3z[i]);
					m_Arith.l_sub(Sn_menzz[i], Sn_menzz[i], WORK1_3z[i]);
					// 5%
					m_Arith.l_add(Sn_mnk40[i], Sn_mnk40[i], WORK0_5[i]);
					m_Arith.l_sub(Sn_mnk40[i], Sn_mnk40[i], WORK1_5[i]);
					m_Arith.l_add(Sn_mnz40[i], Sn_mnz40[i], WORK0_5z[i]);
					m_Arith.l_sub(Sn_mnz40[i], Sn_mnz40[i], WORK1_5z[i]);
					// 8%
					m_Arith.l_add(Sn_mnk63[i], Sn_mnk63[i], WORK0_8[i]);
					m_Arith.l_sub(Sn_mnk63[i], Sn_mnk63[i], WORK1_8[i]);
					m_Arith.l_add(Sn_mnz63[i], Sn_mnz63[i], WORK0_8z[i]);
					m_Arith.l_sub(Sn_mnz63[i], Sn_mnz63[i], WORK1_8z[i]);

					//--->yoshida190628
					// 8%åyå∏
					m_Arith.l_add(Sn_mnk624[i], Sn_mnk624[i], WORK0_8k[i]);
					m_Arith.l_sub(Sn_mnk624[i], Sn_mnk624[i], WORK1_8k[i]);
					m_Arith.l_add(Sn_mnz624[i], Sn_mnz624[i], WORK0_8kz[i]);
					m_Arith.l_sub(Sn_mnz624[i], Sn_mnz624[i], WORK1_8kz[i]);

					// 10%
					m_Arith.l_add(Sn_mnk78[i], Sn_mnk78[i], WORK0_10[i]);
					m_Arith.l_sub(Sn_mnk78[i], Sn_mnk78[i], WORK1_10[i]);
					m_Arith.l_add(Sn_mnz78[i], Sn_mnz78[i], WORK0_10z[i]);
					m_Arith.l_sub(Sn_mnz78[i], Sn_mnz78[i], WORK1_10z[i]);
					//<----
				}
			}
		}
	}

	return 0;
}


//-----------------------------------------------------------------------------
// åªçsÉfÅ[É^ç\ë¢Ç©ÇÁÇÃÉfÅ[É^ÇÃà¯åpÇ¨
//-----------------------------------------------------------------------------
// à¯êî	pDBNpSub	ÅF	ç‡ñ±ÉNÉâÉX
//		sn_seq		ÅF	óöóÉVÅ[ÉPÉìÉX
//-----------------------------------------------------------------------------
int CH31SnTanaData::GetPastData(CDBNpSub *pDBNpSub, int sn_seq)
{
	ASSERT(pDBNpSub);
	if (pDBNpSub == NULL) {
		return -1;
	}

	CString	filter;
	filter.Format(_T("Sn_SEQ = %d"), sn_seq);
	if (pDBNpSub->szsnk) {
		pDBNpSub->szsnk->Requery(filter);
	}
	else {
		if (pDBNpSub->SzsnkOpen(filter)) {
			return -1;
		}
	}
	CSZSNK *pSZSNK = pDBNpSub->szsnk;
	if (pSZSNK->st == -1) {
		// ñ≥Ç¢ÅH
	}

	char	*pHyoID = NULL;

	//--->yoshida200107
	//ÉoÅ[ÉWÉáÉìå©ÇÍÇƒÇ»Ç¢ÇÃÇ≈èCê≥
	int nowVersion = 2;
	if (m_IsIndiviAnbun) {
		filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_TANA_KOBETU_HYOID, nowVersion);
		pHyoID = ICS_SN_TANA_KOBETU_HYOID;
	}
	else {
		filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_TANA_HIREI_HYOID, nowVersion);
		pHyoID = ICS_SN_TANA_HIREI_HYOID;
	}
	//----
	//if (m_IsIndiviAnbun) {
	//	filter.Format(_T("sn_seq = %d and hyoId = \'%s\'"), sn_seq, ICS_SN_TANA_KOBETU_HYOID);
	//	pHyoID = ICS_SN_TANA_KOBETU_HYOID;
	//}
	//else {
	//	filter.Format(_T("sn_seq = %d and hyoId = \'%s\'"), sn_seq, ICS_SN_TANA_HIREI_HYOID);
	//	pHyoID = ICS_SN_TANA_HIREI_HYOID;
	//}
	//<-----

	if (m_DBSyzAccess.m_pSH_LST_VAL) {
		m_DBSyzAccess.m_pSH_LST_VAL->Requery(filter);
	}
	else {
		if (m_DBSyzAccess.OpenShLstVal(pDBNpSub->m_database, filter)) {
			return -1;
		}
	}

	CSH_LST_VAL	*pShLstVal = m_DBSyzAccess.m_pSH_LST_VAL;

	if (m_IsIndiviAnbun) {
		for (int i = 0;; i++) {
			if (i == 0) {
				if ((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {
					break;
				}
			}
			else {
				if ((pShLstVal->MoveNext() == -1) || (pShLstVal->st == -1)) {
					break;
				}
			}

			// 3% çTèúëŒè€édì¸äz
			if (pShLstVal->itmId == _T("AYB00010")) {
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_menzk[0];
				pShLstVal->Update();
			}
			else if (pShLstVal->itmId == _T("AYB00020")) {
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_menzk[1];
				pShLstVal->Update();
			}
			else if (pShLstVal->itmId == _T("AYB00030")) {
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_menzk[2];
				pShLstVal->Update();
			}
			// 3% è¡îÔê≈äz
			else if (pShLstVal->itmId == _T("AYB00040")) {
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_mebzz[0];
				pShLstVal->Update();
			}
			else if (pShLstVal->itmId == _T("AYB00050")) {
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_mebzz[1];
				pShLstVal->Update();
			}
			else if (pShLstVal->itmId == _T("AYB00060")) {
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_mebzz[2];
				pShLstVal->Update();
			}
			// 4% çTèúëŒè€édì¸äz
			else if (pShLstVal->itmId == _T("AYB00070")) {
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_mnk40[0];
				pShLstVal->Update();
			}
			else if (pShLstVal->itmId == _T("AYB00080")) {
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_mnk40[1];
				pShLstVal->Update();
			}
			else if (pShLstVal->itmId == _T("AYB00090")) {
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_mnk40[2];
				pShLstVal->Update();
			}
			// 4% è¡îÔê≈äz
			else if (pShLstVal->itmId == _T("AYB00100")) {
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_mnz40[0];
				pShLstVal->Update();
			}
			else if (pShLstVal->itmId == _T("AYB00110")) {
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_mnz40[1];
				pShLstVal->Update();
			}
			else if (pShLstVal->itmId == _T("AYB00120")) {
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_mnz40[2];
				pShLstVal->Update();
			}
		}
	}
	else {

		for (int i = 0;; i++) {
			if (i == 0) {
				if ((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {
					break;
				}
			}
			else {
				if ((pShLstVal->MoveNext() == -1) || (pShLstVal->st == -1)) {
					break;
				}
			}

			// 3%
			if (pShLstVal->itmId == _T("AXA00010")) {
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_MENZK;
				pShLstVal->Update();
			}
			else if (pShLstVal->itmId == _T("AXA00020")) {
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_MENZZ;
				pShLstVal->Update();
			}
			// 4%
			else if (pShLstVal->itmId == _T("AXA00030")) {
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_MNK40;
				pShLstVal->Update();
			}
			else if (pShLstVal->itmId == _T("AXA00040")) {
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_MNZ40;
				pShLstVal->Update();
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// èWåvçœÇ›ÉfÅ[É^ÇÃÉZÉbÉg
//-----------------------------------------------------------------------------
// à¯êî	pSyzSyukei	ÅF	è¡îÔê≈èWåvÉNÉâÉX
//		isPluralZei	ÅF	ï°êîê≈ó¶
//-----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	ê≥èÌèIóπ
//			-1		ÅF	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CH31SnTanaData::SetCalqedData(CSyzSyukei *pSyzSyukei)
{
	// èWåvÉfÅ[É^ñ≥ÇµÅHÅHÅH


	return 0;
}


//-----------------------------------------------------------------------------
// çXêVéûÇÃã‡äzÉNÉäÉA
//-----------------------------------------------------------------------------
// à¯êî	pDBNpSub	ÅF
//		type		ÅF
//-----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	ê≥èÌèIóπ
//-----------------------------------------------------------------------------
int CH31SnTanaData::ClearDataByRenew(CDBNpSub *pDBNpSub, int type)
{
	m_Arith.l_clear(Sn_MENZK);
	m_Arith.l_clear(Sn_MENZZ);
	m_Arith.l_clear(Sn_MNK40);
	m_Arith.l_clear(Sn_MNZ40);
	m_Arith.l_clear(Sn_MNK63);
	m_Arith.l_clear(Sn_MNZ63);

	for (int i = 0; i < 3; i++) {
		m_Arith.l_clear(Sn_menzk[i]);
		m_Arith.l_clear(Sn_menzz[i]);
		m_Arith.l_clear(Sn_mnk40[i]);
		m_Arith.l_clear(Sn_mnz40[i]);
		m_Arith.l_clear(Sn_mnk63[i]);
		m_Arith.l_clear(Sn_mnz63[i]);
	}
	Sn_MENZsw = 0;
	Sn_tansw = 0;


	/*	CString	filter;
		filter.Format( _T("sn_seq = %d and (hyoId = \'%s\' or hyoId = \'%s\')"), 0, ICS_SN_TANA_KOBETU_HYOID, ICS_SN_TANA_HIREI_HYOID );

		if( m_DBSyzAccess.m_pSH_LST_VAL ){
			m_DBSyzAccess.m_pSH_LST_VAL->Requery( filter );
		}
		else{
			if( m_DBSyzAccess.OpenShLstVal(pDBNpSub->m_database, filter) ){
				return -1;
			}
		}

		CSH_LST_VAL	*pShLstVal = m_DBSyzAccess.m_pSH_LST_VAL;

		for( int i=0;; i++ ){
			if( i==0 ){
				if( (pShLstVal->MoveFirst()==-1) || (pShLstVal->st==-1) ){
					break;
				}
			}
			else{
				if( (pShLstVal->MoveNext()==-1) || (pShLstVal->st==-1) ){
					break;
				}
			}

			// ã‡äzÉNÉäÉA
			if( (pShLstVal->itmId==_T("AYB00010")) ||	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz(êVê≈ó¶ï™)ÅFâ€ê≈Ç…åWÇÈ
				(pShLstVal->itmId==_T("AYB00020")) ||	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz(êVê≈ó¶ï™)ÅFîÒâ€ê≈Ç…åWÇÈ
				(pShLstVal->itmId==_T("AYB00030")) ||	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz(êVê≈ó¶ï™)ÅFã§í Ç…åWÇÈ
				(pShLstVal->itmId==_T("AYB00040")) ||	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéY è¡îÔê≈äz(êVê≈ó¶ï™)ÅFâ€ê≈Ç…åWÇÈ
				(pShLstVal->itmId==_T("AYB00050")) ||	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéY è¡îÔê≈äz(êVê≈ó¶ï™)ÅFîÒâ€ê≈Ç…åWÇÈ
				(pShLstVal->itmId==_T("AYB00060")) ||	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéY è¡îÔê≈äz(êVê≈ó¶ï™)ÅFã§í Ç…åWÇÈ
				(pShLstVal->itmId==_T("AYB00070")) ||	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz(ãåê≈ó¶ï™)ÅFâ€ê≈Ç…åWÇÈ
				(pShLstVal->itmId==_T("AYB00080")) ||	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz(ãåê≈ó¶ï™)ÅFîÒâ€ê≈Ç…åWÇÈ
				(pShLstVal->itmId==_T("AYB00090")) ||	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz(ãåê≈ó¶ï™)ÅFã§í Ç…åWÇÈ
				(pShLstVal->itmId==_T("AYB00100")) ||	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéY è¡îÔê≈äz(ãåê≈ó¶ï™)ÅFâ€ê≈Ç…åWÇÈ
				(pShLstVal->itmId==_T("AYB00110")) ||	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéY è¡îÔê≈äz(ãåê≈ó¶ï™)ÅFîÒâ€ê≈Ç…åWÇÈ
				(pShLstVal->itmId==_T("AYB00120")) ||	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéY è¡îÔê≈äz(ãåê≈ó¶ï™)ÅFã§í Ç…åWÇÈ
				(pShLstVal->itmId==_T("AYB00130")) ||	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz(ãåê≈ó¶ï™)ÅFâ€ê≈Ç…åWÇÈ
				(pShLstVal->itmId==_T("AYB00140")) ||	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz(ãåê≈ó¶ï™)ÅFîÒâ€ê≈Ç…åWÇÈ
				(pShLstVal->itmId==_T("AYB00150")) ||	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéYäz(ãåê≈ó¶ï™)ÅFã§í Ç…åWÇÈ
				(pShLstVal->itmId==_T("AYB00160")) ||	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéY è¡îÔê≈äz(ãåê≈ó¶ï™)ÅFâ€ê≈Ç…åWÇÈ
				(pShLstVal->itmId==_T("AYB00170")) ||	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéY è¡îÔê≈äz(ãåê≈ó¶ï™)ÅFîÒâ€ê≈Ç…åWÇÈ
				(pShLstVal->itmId==_T("AYB00180")) ||	// í≤êÆâ€ê≈édì¸ìôÇ…åWÇÈíIâµéëéY è¡îÔê≈äz(ãåê≈ó¶ï™)ÅFã§í Ç…åWÇÈ
				(pShLstVal->itmId==_T("AXA00010")) ||	// íIâµí≤êÆäzÅ@â€ê≈Ç…åWÇÈ3Åì
				(pShLstVal->itmId==_T("AXA00020")) ||	// íIâµí≤êÆê≈äzÅ@â€ê≈Ç…åWÇÈ3Åì
				(pShLstVal->itmId==_T("AXA00030")) ||	// íIâµí≤êÆäzÅ@â€ê≈Ç…åWÇÈ4Åì
				(pShLstVal->itmId==_T("AXA00040")) ||	// íIâµí≤êÆê≈äzÅ@â€ê≈Ç…åWÇÈ4Åì
				(pShLstVal->itmId==_T("AXA00050")) ||	// íIâµí≤êÆäzÅ@â€ê≈Ç…åWÇÈ6.3Åì
				(pShLstVal->itmId==_T("AXA00060")) ){	// íIâµí≤êÆê≈äzÅ@â€ê≈Ç…åWÇÈ6.3Åì

				pShLstVal->Edit();
				pShLstVal->val = _T("0");
				pShLstVal->sign1 = 0;
				pShLstVal->Update();
			}
		}*/

	return 0;
}