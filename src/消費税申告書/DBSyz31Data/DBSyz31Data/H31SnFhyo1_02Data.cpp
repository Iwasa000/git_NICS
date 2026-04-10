#include "StdAfx.h"
#include "H31SnFhyo1_02Data.h"

static SH_TAGNAME tagData102[] = {
	// â€ê≈ïWèÄäz
	{ _T("BAB00010"), 0 },
	{ _T("BAB00020"), 1 },
	{ _T("BAB00030"), 3 },
	{ _T("BAB00040"), 999 },
	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
	{ _T("BAM00020"), 0 },
	{ _T("BAM00030"), 1 },
	{ _T("BAM00040"), 3 },
	{ _T("BAM00050"), 999 },
	// ì¡íËâ€ê≈édì¸ÇÍÇ…åWÇÈéxï•Ç¢ëŒâøÇÃäz
	{ _T("BAM00070"), 3 },
	{ _T("BAM00080"), 999 },
	// è¡îÔê≈äz
	{ _T("BAC00010"), 0 },
	{ _T("BAC00020"), 1 },
	{ _T("BAC00030"), 3 },
	{ _T("BAC00040"), 999 },
	// çTèúâﬂëÂí≤êÆê≈äz
	{ _T("BAD00010"), 0 },
	{ _T("BAD00020"), 1 },
	{ _T("BAD00030"), 3 },
	{ _T("BAD00040"), 999 },
	// çTèúëŒè€édì¸ê≈äz
	{ _T("BAE00020"), 0 },
	{ _T("BAE00030"), 1 },
	{ _T("BAE00040"), 3 },
	{ _T("BAE00050"), 999 },
	// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
	{ _T("BAE00070"), 0 },
	{ _T("BAE00080"), 1 },
	{ _T("BAE00090"), 3 },
	{ _T("BAE00100"), 999 },
	// îÑè„ÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
	{ _T("BAE00230"), 0 },
	{ _T("BAE00240"), 1 },
	{ _T("BAE00250"), 3 },
	{ _T("BAE00260"), 999 },
	// ì¡íËâ€ê≈édì¸ÇÍÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
	{ _T("BAE00280"), 3 },
	{ _T("BAE00290"), 999 },
	// ë›ì|ÇÍÇ…åWÇÈê≈äz
	{ _T("BAE00120"), 0 },
	{ _T("BAE00130"), 1 },
	{ _T("BAE00140"), 3 },
	{ _T("BAE00150"), 999 },
	// çTèúê≈äzè¨åv
	{ _T("BAE00170"), 0 },
	{ _T("BAE00180"), 1 },
	{ _T("BAE00190"), 3 },
	{ _T("BAE00200"), 999 },
	// çTèúïsë´ä“ïtê≈äz
	{ _T("BAF00010"), 0 },
	{ _T("BAF00020"), 1 },
	{ _T("BAF00030"), 3 },
	{ _T("BAF00040"), 999 },
	// ç∑à¯ê≈äz
	{ _T("BAG00010"), 0 },
	{ _T("BAG00020"), 1 },
	{ _T("BAG00030"), 3 },
	{ _T("BAG00040"), 999 },
	// çTèúïsë´ä“ïtê≈äz
	{ _T("BAI00020"), 1 },
	{ _T("BAI00030"), 3 },
	{ _T("BAI00040"), 999 },
	// ç∑à¯ê≈äz
	{ _T("BAI00060"), 1 },
	{ _T("BAI00070"), 3 },
	{ _T("BAI00080"), 999 },
	// çáåvç∑à¯ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
	{ _T("BAN00000"), 1 },
	{ _T("BAO00000"), 3 },
	{ _T("BAJ00000"), 999 },
	// ä“ïtê≈äz
	{ _T("BAK00020"), 1 },
	{ _T("BAK00030"), 3 },
	{ _T("BAK00040"), 999 },
	// î[ê≈äz
	{ _T("BAK00060"), 1 },
	{ _T("BAK00070"), 3 },
	{ _T("BAK00080"), 999 },

	{ _T("") }

};

CH31SnFhyo1_02Data::CH31SnFhyo1_02Data( BOOL isTransitionalMeasure)
	: m_isTransitionalMeasure(isTransitionalMeasure)
{
	memset(Sn_F102_1A, '\0', sizeof(Sn_F102_1A));
	memset(Sn_F102_1B, '\0', sizeof(Sn_F102_1B));
	memset(Sn_F102_1C, '\0', sizeof(Sn_F102_1C));
	memset(Sn_F102_1X, '\0', sizeof(Sn_F102_1X));

	memset(Sn_F102_1_1A, '\0', sizeof(Sn_F102_1_1A));
	memset(Sn_F102_1_1B, '\0', sizeof(Sn_F102_1_1B));
	memset(Sn_F102_1_1C, '\0', sizeof(Sn_F102_1_1C));
	memset(Sn_F102_1_1X, '\0', sizeof(Sn_F102_1_1X));

	memset(Sn_F102_1_2C, '\0', sizeof(Sn_F102_1_2C));
	memset(Sn_F102_1_2X, '\0', sizeof(Sn_F102_1_2X));

	memset(Sn_F102_2A, '\0', sizeof(Sn_F102_2A));
	memset(Sn_F102_2B, '\0', sizeof(Sn_F102_2B));
	memset(Sn_F102_2C, '\0', sizeof(Sn_F102_2C));
	memset(Sn_F102_2X, '\0', sizeof(Sn_F102_2X));

	memset(Sn_F102_3A, '\0', sizeof(Sn_F102_3A));
	memset(Sn_F102_3B, '\0', sizeof(Sn_F102_3B));
	memset(Sn_F102_3C, '\0', sizeof(Sn_F102_3C));
	memset(Sn_F102_3X, '\0', sizeof(Sn_F102_3X));

	memset(Sn_F102_4A, '\0', sizeof(Sn_F102_4A));
	memset(Sn_F102_4B, '\0', sizeof(Sn_F102_4B));
	memset(Sn_F102_4C, '\0', sizeof(Sn_F102_4C));
	memset(Sn_F102_4X, '\0', sizeof(Sn_F102_4X));

	memset(Sn_F102_5A, '\0', sizeof(Sn_F102_5A));
	memset(Sn_F102_5B, '\0', sizeof(Sn_F102_5B));
	memset(Sn_F102_5C, '\0', sizeof(Sn_F102_5C));
	memset(Sn_F102_5X, '\0', sizeof(Sn_F102_5X));

	memset(Sn_F102_5_1A, '\0', sizeof(Sn_F102_5_1A));
	memset(Sn_F102_5_1B, '\0', sizeof(Sn_F102_5_1B));
	memset(Sn_F102_5_1C, '\0', sizeof(Sn_F102_5_1C));
	memset(Sn_F102_5_1X, '\0', sizeof(Sn_F102_5_1X));

	memset(Sn_F102_5_2C, '\0', sizeof(Sn_F102_5_2C));
	memset(Sn_F102_5_2X, '\0', sizeof(Sn_F102_5_2X));

	memset(Sn_F102_6A, '\0', sizeof(Sn_F102_6A));
	memset(Sn_F102_6B, '\0', sizeof(Sn_F102_6B));
	memset(Sn_F102_6C, '\0', sizeof(Sn_F102_6C));
	memset(Sn_F102_6X, '\0', sizeof(Sn_F102_6X));

	memset(Sn_F102_7A, '\0', sizeof(Sn_F102_7A));
	memset(Sn_F102_7B, '\0', sizeof(Sn_F102_7B));
	memset(Sn_F102_7C, '\0', sizeof(Sn_F102_7C));
	memset(Sn_F102_7X, '\0', sizeof(Sn_F102_7X));

	memset(Sn_F102_8A, '\0', sizeof(Sn_F102_8A));
	memset(Sn_F102_8B, '\0', sizeof(Sn_F102_8B));
	memset(Sn_F102_8C, '\0', sizeof(Sn_F102_8C));
	memset(Sn_F102_8X, '\0', sizeof(Sn_F102_8X));

	memset(Sn_F102_9A, '\0', sizeof(Sn_F102_9A));
	memset(Sn_F102_9B, '\0', sizeof(Sn_F102_9B));
	memset(Sn_F102_9C, '\0', sizeof(Sn_F102_9C));
	memset(Sn_F102_9X, '\0', sizeof(Sn_F102_9X));

	memset(Sn_F102_11B, '\0', sizeof(Sn_F102_11B));
	memset(Sn_F102_11C, '\0', sizeof(Sn_F102_11C));
	memset(Sn_F102_11X, '\0', sizeof(Sn_F102_11X));

	memset(Sn_F102_12B, '\0', sizeof(Sn_F102_12B));
	memset(Sn_F102_12C, '\0', sizeof(Sn_F102_12C));
	memset(Sn_F102_12X, '\0', sizeof(Sn_F102_12X));

	memset(Sn_F102_13B, '\0', sizeof(Sn_F102_13B));
	memset(Sn_F102_13C, '\0', sizeof(Sn_F102_13C));
	memset(Sn_F102_13X, '\0', sizeof(Sn_F102_13X));

	memset(Sn_F102_14B, '\0', sizeof(Sn_F102_14B));
	memset(Sn_F102_14C, '\0', sizeof(Sn_F102_14C));
	memset(Sn_F102_14X, '\0', sizeof(Sn_F102_14X));

	memset(Sn_F102_15B, '\0', sizeof(Sn_F102_15B));
	memset(Sn_F102_15C, '\0', sizeof(Sn_F102_15C));
	memset(Sn_F102_15X, '\0', sizeof(Sn_F102_15X));

	Sn_F102_2c = 0;

	rsv = NULL;

	m_Arith.l_defn(0x16);

	memset(rsv1, '\0', sizeof(rsv1));
	memset(rsv2, '\0', sizeof(rsv2));

}

CH31SnFhyo1_02Data::~CH31SnFhyo1_02Data()
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
int CH31SnFhyo1_02Data::GetData( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT(pDBNpSub);
	if (pDBNpSub == NULL) {
		return -1;
	}
	ASSERT(pDBNpSub->zvol);
	if (pDBNpSub->zvol == NULL) {
		return -1;
	}

	EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki(pDBNpSub->zvol);
	if (KzHoushiki == ID_ICSSH_MENZEI) {
		return 0;
	}


	CString filter;
	int		nowVersion = 2;
	filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_1_02_HYOID, nowVersion);


	if (m_DBSyzAccess.m_pSH_LST_VAL) {
		m_DBSyzAccess.m_pSH_LST_VAL->Requery(filter);
	}
	else {
		if (m_DBSyzAccess.OpenShLstVal(pDBNpSub->m_database, filter)) {
			return -1;
		}
	}


	CSH_LST_VAL		*pShLstVal = m_DBSyzAccess.m_pSH_LST_VAL;
	for (int i = 0; ; i++) {
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

		// â€ê≈ïWèÄäz
		if (pShLstVal->itmId == _T("BAB00010")) {
			m_Util.val_to_bin(Sn_F102_1A, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAB00020")) {
			m_Util.val_to_bin(Sn_F102_1B, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAB00030")) {
			m_Util.val_to_bin(Sn_F102_1C, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAB00040")) {
			m_Util.val_to_bin(Sn_F102_1X, pShLstVal->val);
		}

		// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
		else if (pShLstVal->itmId == _T("BAM00020")) {
			m_Util.val_to_bin(Sn_F102_1_1A, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAM00030")) {
			m_Util.val_to_bin(Sn_F102_1_1B, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAM00040")) {
			m_Util.val_to_bin(Sn_F102_1_1C, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAM00050")) {
			m_Util.val_to_bin(Sn_F102_1_1X, pShLstVal->val);
		}

		// ì¡íËâ€ê≈édì¸ÇÍÇ…åWÇÈéxï•Ç¢ëŒâøÇÃäz
		else if (pShLstVal->itmId == _T("BAM00070")) {
			m_Util.val_to_bin(Sn_F102_1_2C, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAM00080")) {
			m_Util.val_to_bin(Sn_F102_1_2X, pShLstVal->val);
		}


		// è¡îÔê≈äz
		else if (pShLstVal->itmId == _T("BAC00010")) {
			m_Util.val_to_bin(Sn_F102_2A, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAC00020")) {
			m_Util.val_to_bin(Sn_F102_2B, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAC00030")) {
			m_Util.val_to_bin(Sn_F102_2C, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAC00040")) {
			m_Util.val_to_bin(Sn_F102_2X, pShLstVal->val);
			Sn_F102_2c = pShLstVal->sign1;	// éËì¸óÕÉTÉCÉì
		}


		// çTèúâﬂëÂí≤êÆê≈äz
		else if (pShLstVal->itmId == _T("BAD00010")) {
			m_Util.val_to_bin(Sn_F102_3A, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAD00020")) {
			m_Util.val_to_bin(Sn_F102_3B, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAD00030")) {
			m_Util.val_to_bin(Sn_F102_3C, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAD00040")) {
			m_Util.val_to_bin(Sn_F102_3X, pShLstVal->val);
		}

		// çTèúëŒè€édì¸ê≈äz
		else if (pShLstVal->itmId == _T("BAE00020")) {
			m_Util.val_to_bin(Sn_F102_4A, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAE00030")) {
			m_Util.val_to_bin(Sn_F102_4B, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAE00040")) {
			m_Util.val_to_bin(Sn_F102_4C, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAE00050")) {
			m_Util.val_to_bin(Sn_F102_4X, pShLstVal->val);
		}

		// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
		else if (pShLstVal->itmId == _T("BAE00070")) {
			m_Util.val_to_bin(Sn_F102_5A, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAE00080")) {
			m_Util.val_to_bin(Sn_F102_5B, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAE00090")) {
			m_Util.val_to_bin(Sn_F102_5C, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAE00100")) {
			m_Util.val_to_bin(Sn_F102_5X, pShLstVal->val);
		}

		// îÑè„ÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
		else if (pShLstVal->itmId == _T("BAE00230")) {
			m_Util.val_to_bin(Sn_F102_5_1A, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAE00240")) {
			m_Util.val_to_bin(Sn_F102_5_1B, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAE00250")) {
			m_Util.val_to_bin(Sn_F102_5_1C, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAE00260")) {
			m_Util.val_to_bin(Sn_F102_5_1X, pShLstVal->val);
		}

		// ì¡íËâ€ê≈édì¸ÇÍÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
		else if (pShLstVal->itmId == _T("BAE00280")) {
			m_Util.val_to_bin(Sn_F102_5_2C, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAE00290")) {
			m_Util.val_to_bin(Sn_F102_5_2X, pShLstVal->val);
		}

		// ë›ì|ÇÍÇ…åWÇÈê≈äz
		else if (pShLstVal->itmId == _T("BAE00120")) {
			m_Util.val_to_bin(Sn_F102_6A, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAE00130")) {
			m_Util.val_to_bin(Sn_F102_6B, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAE00140")) {
			m_Util.val_to_bin(Sn_F102_6C, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAE00150")) {
			m_Util.val_to_bin(Sn_F102_6X, pShLstVal->val);
		}

		// çTèúê≈äzè¨åv
		else if (pShLstVal->itmId == _T("BAE00170")) {
			m_Util.val_to_bin(Sn_F102_7A, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAE00180")) {
			m_Util.val_to_bin(Sn_F102_7B, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAE00190")) {
			m_Util.val_to_bin(Sn_F102_7C, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAE00200")) {
			m_Util.val_to_bin(Sn_F102_7X, pShLstVal->val);
		}

		// çTèúïsë´ä“ïtê≈äz
		else if (pShLstVal->itmId == _T("BAF00010")) {
			m_Util.val_to_bin(Sn_F102_8A, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAF00020")) {
			m_Util.val_to_bin(Sn_F102_8B, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAF00030")) {
			m_Util.val_to_bin(Sn_F102_8C, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAF00040")) {
			m_Util.val_to_bin(Sn_F102_8X, pShLstVal->val);
		}

		// ç∑à¯ê≈äz
		else if (pShLstVal->itmId == _T("BAG00010")) {
			m_Util.val_to_bin(Sn_F102_9A, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAG00020")) {
			m_Util.val_to_bin(Sn_F102_9B, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAG00030")) {
			m_Util.val_to_bin(Sn_F102_9C, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAG00040")) {
			m_Util.val_to_bin(Sn_F102_9X, pShLstVal->val);
		}

		// ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
		// çTèúïsë´ä“ïtê≈äz
		else if (pShLstVal->itmId == _T("BAI00020")) {
			m_Util.val_to_bin(Sn_F102_11B, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAI00030")) {
			m_Util.val_to_bin(Sn_F102_11C, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAI00040")) {
			m_Util.val_to_bin(Sn_F102_11X, pShLstVal->val);
		}

		// ç∑à¯ê≈äz
		else if (pShLstVal->itmId == _T("BAI00060")) {
			m_Util.val_to_bin(Sn_F102_12B, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAI00070")) {
			m_Util.val_to_bin(Sn_F102_12C, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAI00080")) {
			m_Util.val_to_bin(Sn_F102_12X, pShLstVal->val);
		}

		// çáåvç∑à¯ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
		else if (pShLstVal->itmId == _T("BAN00000")) {
			m_Util.val_to_bin(Sn_F102_13B, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAO00000")) {
			m_Util.val_to_bin(Sn_F102_13C, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAJ00000")) {
			m_Util.val_to_bin(Sn_F102_13X, pShLstVal->val);
		}

		// è˜ìnäÑäz
		// ä“ïtäz
		else if (pShLstVal->itmId == _T("BAK00020")) {
			m_Util.val_to_bin(Sn_F102_14B, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAK00030")) {
			m_Util.val_to_bin(Sn_F102_14C, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAK00040")) {
			m_Util.val_to_bin(Sn_F102_14X, pShLstVal->val);
		}

		// î[ê≈äz
		else if (pShLstVal->itmId == _T("BAK00060")) {
			m_Util.val_to_bin(Sn_F102_15B, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAK00070")) {
			m_Util.val_to_bin(Sn_F102_15C, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("BAK00080")) {
			m_Util.val_to_bin(Sn_F102_15X, pShLstVal->val);
		}
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
int CH31SnFhyo1_02Data::SetData( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT(pDBNpSub);
	if (pDBNpSub == NULL) {
		return -1;
	}

	ASSERT(pDBNpSub->zvol);
	if (pDBNpSub->zvol == NULL) {
		return -1;
	}

	CString filter = "";
	char	*pHyoId = NULL;
	int		nowVersion = 2;
	filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_1_02_HYOID, nowVersion);
	pHyoId = ICS_SN_FHYO_1_02_HYOID;

	EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki(pDBNpSub->zvol);
	if (KzHoushiki == ID_ICSSH_MENZEI) {
		return 0;
	}

	if (m_DBSyzAccess.m_pSH_LST_VAL) {
		m_DBSyzAccess.m_pSH_LST_VAL->Requery(filter);
	}
	else {
		if (m_DBSyzAccess.OpenShLstVal(pDBNpSub->m_database, filter)) {
			return -1;
		}
	}


	CSH_LST_VAL		*pShLstVal = m_DBSyzAccess.m_pSH_LST_VAL;

	if ((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {	// ÉfÅ[É^Ç»Çµ

		// â€ê≈ïWèÄäz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAB00010");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_1A);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAB00020");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_1B);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAB00030");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_1C);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAB00040");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_1X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAM00020");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_1_1A);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAM00030");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_1_1B);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAM00040");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_1_1C);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAM00050");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_1_1X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ì¡íËâ€ê≈édì¸ÇÍÇ…åWÇÈéxï•Ç¢ëŒâøÇÃäz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAM00070");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_1_2C);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAM00080");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_1_2X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// è¡îÔê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAC00010");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_2A);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAC00020");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_2B);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAC00030");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_2C);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAC00040");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_2X);
		pShLstVal->sign1 = Sn_F102_2c;
		pShLstVal->Update();

		// çTèúâﬂëÂí≤êÆê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAD00010");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_3A);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAD00020");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_3B);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAD00030");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_3C);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAD00040");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_3X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// çTèúëŒè€édì¸ê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAE00020");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_4A);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAE00030");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_4B);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAE00040");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_4C);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAE00050");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_4X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAE00070");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_5A);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAE00080");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_5B);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAE00090");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_5C);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAE00100");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_5X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// îÑè„ÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAE00230");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_5_1A);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAE00240");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_5_1B);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAE00250");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_5_1C);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAE00260");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_5_1X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ì¡íËâ€ê≈édì¸ÇÍÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAE00280");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_5_2C);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAE00290");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_5_2X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ë›ì|Ç…åWÇÈê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAE00120");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_6A);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAE00130");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_6B);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAE00140");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_6C);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAE00150");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_6X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// çTèúê≈äzè¨åv
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAE00170");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_7A);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAE00180");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_7B);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAE00190");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_7C);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAE00200");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_7X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// çTèúïsë´ä“ïtê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAF00010");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_8A);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAF00020");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_8B);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAF00030");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_8C);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAF00040");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_8X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ç∑à¯ê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAG00010");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_9A);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAG00020");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_9B);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAG00030");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_9C);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAG00040");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_9X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// çTèúïsë´ä“ïtê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAI00020");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_11B);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAI00030");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_11C);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAI00040");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_11X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ç∑à¯ê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAI00060");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_12B);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAI00070");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_12C);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAI00080");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_12X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// çáåvç∑à¯ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAN00000");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_13B);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAO00000");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_13C);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAJ00000");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_13X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ä“ïtäz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAK00020");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_14B);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAK00030");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_14C);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAK00040");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_14X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// î[ê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAK00060");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_15B);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAK00070");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_15C);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("BAK00080");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F102_15X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();
	}
	else {
		CStringArray	updTagAry;

		for (int i = 0; ; i++) {
			if( i == 0 ) {
				if ((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {
					break;
				}
			}
			else {
				if ((pShLstVal->MoveNext() == -1) || (pShLstVal->st == -1)) {
					break;
				}
			}

			// â€ê≈ïWèÄäz
			if (pShLstVal->itmId == _T("BAB00010")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_1A);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAB00020")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_1B);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAB00030")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_1C);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAB00040")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_1X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
			else if (pShLstVal->itmId == _T("BAM00020")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_1_1A);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAM00030")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_1_1B);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAM00040")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_1_1C);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAM00050")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_1_1X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// ì¡íËâ€ê≈édì¸ÇÍÇ…åWÇÈéxï•Ç¢ëŒâøÇÃäz
			else if (pShLstVal->itmId == _T("BAM00070")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_1_2C);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAM00080")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_1_2X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// è¡îÔê≈äz
			else if (pShLstVal->itmId == _T("BAC00010")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_2A);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAC00020")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_2B);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAC00030")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_2C);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAC00040")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_2X);
				pShLstVal->sign1 = Sn_F102_2c;
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// çTèúâﬂëÂí≤êÆê≈äz
			else if (pShLstVal->itmId == _T("BAD00010")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_3A);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAD00020")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_3B);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAD00030")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_3C);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAD00040")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_3X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// çTèúëŒè€édì¸ê≈äz
			else if (pShLstVal->itmId == _T("BAE00020")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_4A);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAE00030")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_4B);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAE00040")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_4C);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAE00050")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_4X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
			else if (pShLstVal->itmId == _T("BAE00070")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_5A);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAE00080")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_5B);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAE00090")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_5C);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAE00100")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_5X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// îÑè„ÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
			else if (pShLstVal->itmId == _T("BAE00230")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_5_1A);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAE00240")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_5_1B);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAE00250")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_5_1C);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAE00260")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_5_1X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// ì¡íËâ€ê≈édì¸ÇÍÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
			else if (pShLstVal->itmId == _T("BAE00280")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_5_2C);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAE00290")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_5_2X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// ë›ì|ÇÍÇ…åWÇÈê≈äz
			else if (pShLstVal->itmId == _T("BAE00120")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_6A);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAE00130")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_6B);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAE00140")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_6C);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAE00150")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_6X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// çTèúê≈äzè¨åv
			else if (pShLstVal->itmId == _T("BAE00170")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_7A);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAE00180")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_7B);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAE00190")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_7C);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAE00200")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_7X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// çTèúïsë´ä“ïtê≈äz
			else if (pShLstVal->itmId == _T("BAF00010")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_8A);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAF00020")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_8B);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAF00030")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_8C);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAF00040")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_8X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// ç∑à¯ê≈äz
			else if (pShLstVal->itmId == _T("BAG00010")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_9A);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAG00020")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_9B);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAG00030")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_9C);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAG00040")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_9X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// çTèúïsë´ä“ïtê≈äz
			else if (pShLstVal->itmId == _T("BAI00020")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_11B);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAI00030")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_11C);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAI00040")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_11X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// ç∑à¯ê≈äz
			else if (pShLstVal->itmId == _T("BAI00060")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_12B);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAI00070")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_12C);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAI00080")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_12X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// çáåvç∑à¯ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈ê≈äz
			else if (pShLstVal->itmId == _T("BAN00000")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_13B);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAO00000")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_13C);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAJ00000")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_13X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// ä“ïtäz
			else if (pShLstVal->itmId == _T("BAK00020")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_14B);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAK00030")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_14C);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAK00040")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_14X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// î[ê≈äz
			else if (pShLstVal->itmId == _T("BAK00060")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_15B);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAK00070")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_15C);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("BAK00080")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F102_15X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
		}

		int adCnt = 0;
		for (int i = 0; ; i++) {
			if (strlen(tagData102[i].tagName) == 0) {
				break;
			}
			adCnt++;
		}

		int updCnt = (int)updTagAry.GetCount();
		if (adCnt != updCnt) {
			CArray<unsigned char*> MonyPrtAry;

			MonyPrtAry.Add(Sn_F102_1A);
			MonyPrtAry.Add(Sn_F102_1B);
			MonyPrtAry.Add(Sn_F102_1C);
			MonyPrtAry.Add(Sn_F102_1X);
			MonyPrtAry.Add(Sn_F102_1_1A);
			MonyPrtAry.Add(Sn_F102_1_1B);
			MonyPrtAry.Add(Sn_F102_1_1C);
			MonyPrtAry.Add(Sn_F102_1_1X);
			MonyPrtAry.Add(Sn_F102_1_2C);
			MonyPrtAry.Add(Sn_F102_1_2X);
			MonyPrtAry.Add(Sn_F102_2A);
			MonyPrtAry.Add(Sn_F102_2B);
			MonyPrtAry.Add(Sn_F102_2C);
			MonyPrtAry.Add(Sn_F102_2X);
			MonyPrtAry.Add(Sn_F102_3A);
			MonyPrtAry.Add(Sn_F102_3B);
			MonyPrtAry.Add(Sn_F102_3C);
			MonyPrtAry.Add(Sn_F102_3X);
			MonyPrtAry.Add(Sn_F102_4A);
			MonyPrtAry.Add(Sn_F102_4B);
			MonyPrtAry.Add(Sn_F102_4C);
			MonyPrtAry.Add(Sn_F102_4X);
			MonyPrtAry.Add(Sn_F102_5A);
			MonyPrtAry.Add(Sn_F102_5B);
			MonyPrtAry.Add(Sn_F102_5C);
			MonyPrtAry.Add(Sn_F102_5X);
			MonyPrtAry.Add(Sn_F102_5_1A);
			MonyPrtAry.Add(Sn_F102_5_1B);
			MonyPrtAry.Add(Sn_F102_5_1C);
			MonyPrtAry.Add(Sn_F102_5_1X);
			MonyPrtAry.Add(Sn_F102_5_2C);
			MonyPrtAry.Add(Sn_F102_5_2X);
			MonyPrtAry.Add(Sn_F102_6A);
			MonyPrtAry.Add(Sn_F102_6B);
			MonyPrtAry.Add(Sn_F102_6C);
			MonyPrtAry.Add(Sn_F102_6X);
			MonyPrtAry.Add(Sn_F102_7A);
			MonyPrtAry.Add(Sn_F102_7B);
			MonyPrtAry.Add(Sn_F102_7C);
			MonyPrtAry.Add(Sn_F102_7X);
			MonyPrtAry.Add(Sn_F102_8A);
			MonyPrtAry.Add(Sn_F102_8B);
			MonyPrtAry.Add(Sn_F102_8C);
			MonyPrtAry.Add(Sn_F102_8X);
			MonyPrtAry.Add(Sn_F102_9A);
			MonyPrtAry.Add(Sn_F102_9B);
			MonyPrtAry.Add(Sn_F102_9C);
			MonyPrtAry.Add(Sn_F102_9X);
			MonyPrtAry.Add(Sn_F102_11B);
			MonyPrtAry.Add(Sn_F102_11C);
			MonyPrtAry.Add(Sn_F102_11X);
			MonyPrtAry.Add(Sn_F102_12B);
			MonyPrtAry.Add(Sn_F102_12C);
			MonyPrtAry.Add(Sn_F102_12X);
			MonyPrtAry.Add(Sn_F102_13B);
			MonyPrtAry.Add(Sn_F102_13C);
			MonyPrtAry.Add(Sn_F102_13X);
			MonyPrtAry.Add(Sn_F102_14B);
			MonyPrtAry.Add(Sn_F102_14C);
			MonyPrtAry.Add(Sn_F102_14X);
			MonyPrtAry.Add(Sn_F102_15B);
			MonyPrtAry.Add(Sn_F102_15C);
			MonyPrtAry.Add(Sn_F102_15X);

			BOOL findFlg = FALSE;
			for (int i = 0; ; i++) {
				if (strlen(tagData102[i].tagName) == 0) {
					break;
				}

				findFlg = FALSE;
				for (int j = 0; j < updCnt; j++) {
					if (updTagAry[j].CompareNoCase(tagData102[j].tagName) == 0) {
						findFlg = TRUE;
						break;
					}

					if (findFlg == FALSE) {
						// äYìñÉ^ÉOÇÃí«â¡
						pShLstVal->AddNew();
						pShLstVal->sn_seq = sn_seq;
						pShLstVal->version = nowVersion;
						pShLstVal->hyoId = pHyoId;
						pShLstVal->itmId = tagData102[i].tagName;
						m_Util.val_to_asci(&pShLstVal->val, MonyPrtAry[i]);
						pShLstVal->sign1 = 0;
						pShLstVal->Update();
					}
				}
			}
		}
	}


	return 0;
}



//yoshida
//-----------------------------------------------------------------------------
// èWåvçœÇ›ÉfÅ[É^ÇÃÉZÉbÉg(ïtï\ÇP)
//-----------------------------------------------------------------------------
// à¯êî	pSyzSyukei	ÅF	è¡îÔê≈èWåvÉNÉâÉX
//		isPluralZei	ÅF	ï°êîê≈ó¶
//-----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	ê≥èÌèIóπ
//			-1		ÅF	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CH31SnFhyo1_02Data::SetCalqed1_02Data(CSyzSyukei *pSyzSyukei, BOOL &isPluralZei)
{

	isPluralZei = FALSE;

	MoneyBasejagArray money;

	money = pSyzSyukei->GetShinkokuData(_T("31"));

	int cnt = (int)money.GetCount();
	int incnt = 0;

	unsigned	char	exZeiflg = 0x00;	// Å†Å†Å†Å† Å°Å°Å°Å°
											//  D0ÅF3%
											//  D1ÅF4%
											//  D2ÅF6.3%

	for (int i = 0; i < cnt; i++) {
		incnt = (int)money[i].GetCount();
		for (int j = 0; j < incnt; j++) {
			if (money[i][j].code.Left(4) == _T("3101")) {

				if (money[i][j].code == _T("310100")) {//â€ê≈ïWèÄäz ãåê≈ó¶ï™è¨åv X
					memmove(Sn_F102_1X, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("310101")) {//â€ê≈ïWèÄäz ÇRÅììKópï™
					memmove(Sn_F102_1A, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("310102")) {//â€ê≈ïWèÄäz ÇSÅììKópï™
					memmove(Sn_F102_1B, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("310103")) {//â€ê≈ïWèÄäz ÇUÅDÇRÅììKópï™
					memmove(Sn_F102_1C, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}

			}

			else if (money[i][j].code.Left(4) == _T("3102")) {
				if (money[i][j].code == _T("310200")) {//â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
					memmove(Sn_F102_1_1X, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("310201")) {//â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäzÅ@ÇRÅììKópï™
					memmove(Sn_F102_1_1A, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("310202")) {//â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäzÅ@ÇSÅììKópï™
					memmove(Sn_F102_1_1B, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("310203")) {//â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäzÅ@ÇUÅDÇRÅììKópï™
					memmove(Sn_F102_1_1C, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
			}

			else if (money[i][j].code.Left(4) == _T("3103")) {

				if (money[i][j].code == _T("310300")) {//ì¡íËâ€ê≈édì¸ÇÍÇ…åWÇÈéxï•ëŒâøÇÃäz
					memmove(Sn_F102_1_2X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}

				else if (money[i][j].code == _T("310303")) {//ì¡íËâ€ê≈édì¸ÇÍÇ…åWÇÈéxï•ëŒâøÇÃäzÅ@ÇUÅDÇRÅììKópï™
					memmove(Sn_F102_1_2C, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
			}

			else if (money[i][j].code.Left(4) == _T("3104")) {
				if (money[i][j].code == _T("310400")) {//è¡îÔê≈äz
					memmove(Sn_F102_2X, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("310401")) {//è¡îÔê≈äzÅ@ÇRÅììKópï™
					memmove(Sn_F102_2A, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("310402")) {//è¡îÔê≈äzÅ@ÇSÅììKópï™
					memmove(Sn_F102_2B, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("310403")) {//è¡îÔê≈äzÅ@ÇUÅDÇRÅììKópï™
					memmove(Sn_F102_2C, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
			}

			else if (money[i][j].code.Left(4) == _T("3105")) {

				if (money[i][j].code == _T("310500")) {//çTèúâﬂëÂí≤êÆê≈äz
					memmove(Sn_F102_3X, money[i][j].arith, MONY_BUF_SIZE);
				}

				else if (money[i][j].code == _T("310501")) {//çTèúâﬂëÂí≤êÆê≈äzÅ@ÇRÅììKópï™
					memmove(Sn_F102_3A, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("310502")) {//çTèúâﬂëÂí≤êÆê≈äzÅ@ÇSÅììKópï™
					memmove(Sn_F102_3B, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("310503")) {//çTèúâﬂëÂí≤êÆê≈äzÅ@ÇUÅDÇRÅììKópï™
					memmove(Sn_F102_3C, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
			}

			else if (money[i][j].code.Left(4) == _T("3106")) {
				if (money[i][j].code == _T("310600")) {//çTèúëŒè€édì¸ê≈äz
					memmove(Sn_F102_4X, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("310601")) {//çTèúëŒè€édì¸ê≈äzÅ@ÇRÅììKópï™
					memmove(Sn_F102_4A, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("310602")) {//çTèúëŒè€édì¸ê≈äzÅ@ÇSÅììKópï™
					memmove(Sn_F102_4B, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("310603")) {//çTèúëŒè€édì¸ê≈äzÅ@ÇUÅDÇRÅììKópï™
					memmove(Sn_F102_4C, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
			}

			else if (money[i][j].code.Left(4) == _T("3107")) {
				if (money[i][j].code == _T("310700")) {//ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
					memmove(Sn_F102_5X, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("310701")) {//ï‘ä“ìôëŒâøÇ…åWÇÈê≈äzÅ@ÇRÅììKópï™
					memmove(Sn_F102_5A, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("310702")) {//ï‘ä“ìôëŒâøÇ…åWÇÈê≈äzÅ@ÇSÅììKópï™
					memmove(Sn_F102_5B, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("310703")) {//ï‘ä“ìôëŒâøÇ…åWÇÈê≈äzÅ@ÇUÅDÇRÅììKópï™
					memmove(Sn_F102_5C, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
			}

			else if (money[i][j].code.Left(4) == _T("3108")) {
				if (money[i][j].code == _T("310800")) {//îÑè„Ç∞ÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
					memmove(Sn_F102_5_1X, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("310801")) {//îÑè„Ç∞ÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äzÅ@ÇRÅììKópï™
					memmove(Sn_F102_5_1A, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("310802")) {//îÑè„Ç∞ÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äzÅ@ÇSÅììKópï™
					memmove(Sn_F102_5_1B, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("310803")) {//îÑè„Ç∞ÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äzÅ@ÇUÅDÇRÅììKópï™
					memmove(Sn_F102_5_1C, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
			}

			else if (money[i][j].code.Left(4) == _T("3109")) {
				if (money[i][j].code == _T("310900")) {//ì¡íËâ€ê≈édì¸ÇÍÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
					memmove(Sn_F102_5_2X, money[i][j].arith, MONY_BUF_SIZE);
				}
				
				else if (money[i][j].code == _T("310903")) {//ì¡íËâ€ê≈édì¸ÇÍÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äzÅ@ÇUÅDÇRÅììKópï™
					memmove(Sn_F102_5_2C, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
			}

			else if (money[i][j].code.Left(4) == _T("3110")) {
				if (money[i][j].code == _T("311000")) {//ë›ì|ÇÍÇ…åWÇÈê≈äz
					memmove(Sn_F102_6X, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("311001")) {//ë›ì|ÇÍÇ…åWÇÈê≈äzÅ@ÇRÅììKópï™
					memmove(Sn_F102_6A, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("311002")) {//ë›ì|ÇÍÇ…åWÇÈê≈äzÅ@ÇSÅììKópï™
					memmove(Sn_F102_6B, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("311003")) {//ë›ì|ÇÍÇ…åWÇÈê≈äzÅ@ÇUÅDÇRÅììKópï™
					memmove(Sn_F102_6C, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
			}

			else if (money[i][j].code.Left(4) == _T("3111")) {
				if (money[i][j].code == _T("311100")) {//çTèúê≈äzè¨åv
					memmove(Sn_F102_7X, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("311101")) {//çTèúê≈äzè¨åvÅ@ÇRÅììKópï™
					memmove(Sn_F102_7A, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("311102")) {//çTèúê≈äzè¨åvÅ@ÇSÅììKópï™
					memmove(Sn_F102_7B, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("311103")) {//çTèúê≈äzè¨åvÅ@ÇUÅDÇRÅììKópï™
					memmove(Sn_F102_7C, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
			}

			else if (money[i][j].code.Left(4) == _T("3112")) {
				if (money[i][j].code == _T("311201")) {//çTèúïsë´ä“ïtê≈äz
					memmove(Sn_F102_8X, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("311201")) {//çTèúïsë´ä“ïtê≈äzÅ@ÇRÅììKópï™
					memmove(Sn_F102_8A, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("311202")) {//çTèúïsë´ä“ïtê≈äzÅ@ÇSÅììKópï™
					memmove(Sn_F102_8B, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("311203")) {//çTèúïsë´ä“ïtê≈äzÅ@ÇUÅDÇRÅììKópï™
					memmove(Sn_F102_8C, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
			}

			else if (money[i][j].code.Left(4) == _T("3113")) {
				if (money[i][j].code == _T("311301")) {//çTèúïsë´ä“ïtê≈äz
					memmove(Sn_F102_9X, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("311301")) {//ç∑à¯ê≈äzÅ@ÇRÅììKópï™
					memmove(Sn_F102_9A, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("311302")) {//ç∑à¯ê≈äzÅ@ÇSÅììKópï™
					memmove(Sn_F102_9B, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("311303")) {//ç∑à¯ê≈äzÅ@ÇUÅDÇRÅììKópï™
					memmove(Sn_F102_9C, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
			}

			else if (money[i][j].code.Left(4) == _T("3114")) {//çáåvç∑à¯ê≈äz
				//çÄñ⁄Ç»Çµ
			}

			else if (money[i][j].code.Left(4) == _T("3115")) {
				if (money[i][j].code == _T("311500")) {//çTèúïsë´ä“ïtê≈äz
					memmove(Sn_F102_11X, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("311502")) {//çTèúïsë´ä“ïtê≈äzÅ@ÇSÅììKópï™
					memmove(Sn_F102_11B, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("311503")) {//çTèúïsë´ä“ïtê≈äzÅ@ÇUÅDÇRÅììKópï™
					memmove(Sn_F102_11C, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
			}

			else if (money[i][j].code.Left(4) == _T("3116")) {
				if (money[i][j].code == _T("311600")) {//ç∑à¯ê≈äz
					memmove(Sn_F102_12X, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("311602")) {//ç∑à¯ê≈äzÅ@ÇSÅììKópï™
					memmove(Sn_F102_12B, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("311603")) {//ç∑à¯ê≈äzÅ@ÇUÅDÇRÅììKópï™
					memmove(Sn_F102_12C, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
			}

			else if (money[i][j].code.Left(4) == _T("3117")) {
				if (money[i][j].code == _T("311700")) {//çáåvç∑à¯ê≈äzç∑à¯ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
					memmove(Sn_F102_13X, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("311702")) {//çáåvç∑à¯ê≈äzç∑à¯ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äzÅ@ÇSÅììKópï™
					memmove(Sn_F102_13B, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("311703")) {//çáåvç∑à¯ê≈äzç∑à¯ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äzÅ@ÇUÅDÇRÅììKópï™
					memmove(Sn_F102_13C, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
			}

			else if (money[i][j].code.Left(4) == _T("3118")) {
				if (money[i][j].code == _T("311800")) {//ä“ïtäz
					memmove(Sn_F102_14X, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("311802")) {//ä“ïtäzÅ@ÇSÅììKópï™
					memmove(Sn_F102_14B, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("311803")) {//ä“ïtäzÅ@ÇUÅDÇRÅììKópï™
					memmove(Sn_F102_14C, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
			}

			else if (money[i][j].code.Left(4) == _T("3119")) {
				if (money[i][j].code == _T("311900")) {//î[ê≈äz
					memmove(Sn_F102_15X, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("311902")) {//î[ê≈äzÅ@ÇSÅììKópï™
					memmove(Sn_F102_15B, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("311903")) {//î[ê≈äzÅ@ÇUÅDÇRÅììKópï™
					memmove(Sn_F102_15C, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
			}

			else if (money[i][j].code.Left(4) == _T("3120")) {//çáåvç∑à¯ê≈äz
				//çÄñ⁄Ç»Çµ
			}
	
		}
	}

	if (((exZeiflg & 0x07) == 0x01) || ((exZeiflg & 0x07) == 0x02) || ((exZeiflg & 0x07) == 0x04) || (exZeiflg == 0x00)) {
		isPluralZei = FALSE;
	}
	else {
		isPluralZei = TRUE;
	}


	return 0;
}


//-----------------------------------------------------------------------------
// çXêVéûÇÃã‡äzÉNÉäÉA
//-----------------------------------------------------------------------------
// à¯êî	pDBNpSub	ÅF	
//		type		ÅF	
//-----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	
//-----------------------------------------------------------------------------
int CH31SnFhyo1_02Data::ClearDataByRenew(CDBNpSub *pDBNpSub, int type)
{

	m_Arith.l_clear(Sn_F102_1A);
	m_Arith.l_clear(Sn_F102_1B);
	m_Arith.l_clear(Sn_F102_1C);
	m_Arith.l_clear(Sn_F102_1X);

	m_Arith.l_clear(Sn_F102_1_1A);
	m_Arith.l_clear(Sn_F102_1_1B);
	m_Arith.l_clear(Sn_F102_1_1C);
	m_Arith.l_clear(Sn_F102_1_1X);

	m_Arith.l_clear(Sn_F102_1_2C);
	m_Arith.l_clear(Sn_F102_1_2X);
	
	m_Arith.l_clear(Sn_F102_2A);
	m_Arith.l_clear(Sn_F102_2B);
	m_Arith.l_clear(Sn_F102_2C);
	m_Arith.l_clear(Sn_F102_2X);

	m_Arith.l_clear(Sn_F102_3A);
	m_Arith.l_clear(Sn_F102_3B);
	m_Arith.l_clear(Sn_F102_3C);
	m_Arith.l_clear(Sn_F102_3X);

	m_Arith.l_clear(Sn_F102_4A);
	m_Arith.l_clear(Sn_F102_4B);
	m_Arith.l_clear(Sn_F102_4C);
	m_Arith.l_clear(Sn_F102_4X);

	m_Arith.l_clear(Sn_F102_5A);
	m_Arith.l_clear(Sn_F102_5B);
	m_Arith.l_clear(Sn_F102_5C);
	m_Arith.l_clear(Sn_F102_5X);

	m_Arith.l_clear(Sn_F102_5_1A);
	m_Arith.l_clear(Sn_F102_5_1B);
	m_Arith.l_clear(Sn_F102_5_1C);
	m_Arith.l_clear(Sn_F102_5_1X);

	m_Arith.l_clear(Sn_F102_5_2C);
	m_Arith.l_clear(Sn_F102_5_2X);

	m_Arith.l_clear(Sn_F102_6A);
	m_Arith.l_clear(Sn_F102_6B);
	m_Arith.l_clear(Sn_F102_6C);
	m_Arith.l_clear(Sn_F102_6X);

	m_Arith.l_clear(Sn_F102_7A);
	m_Arith.l_clear(Sn_F102_7B);
	m_Arith.l_clear(Sn_F102_7C);
	m_Arith.l_clear(Sn_F102_7X);

	m_Arith.l_clear(Sn_F102_8A);
	m_Arith.l_clear(Sn_F102_8B);
	m_Arith.l_clear(Sn_F102_8C);
	m_Arith.l_clear(Sn_F102_8X);

	m_Arith.l_clear(Sn_F102_9A);
	m_Arith.l_clear(Sn_F102_9B);
	m_Arith.l_clear(Sn_F102_9C);
	m_Arith.l_clear(Sn_F102_9X);

	m_Arith.l_clear(Sn_F102_11B);
	m_Arith.l_clear(Sn_F102_11C);
	m_Arith.l_clear(Sn_F102_11X);

	m_Arith.l_clear(Sn_F102_12B);
	m_Arith.l_clear(Sn_F102_12C);
	m_Arith.l_clear(Sn_F102_12X);

	m_Arith.l_clear(Sn_F102_13B);
	m_Arith.l_clear(Sn_F102_13C);
	m_Arith.l_clear(Sn_F102_13X);

	m_Arith.l_clear(Sn_F102_14B);
	m_Arith.l_clear(Sn_F102_14C);
	m_Arith.l_clear(Sn_F102_14X);

	m_Arith.l_clear(Sn_F102_15B);
	m_Arith.l_clear(Sn_F102_15C);
	m_Arith.l_clear(Sn_F102_15X);

	Sn_F102_2c = 0;

	return 0;
}



//-----------------------------------------------------------------------------
// éËì¸óÕÉTÉCÉìÇÃÉNÉäÉA
//-----------------------------------------------------------------------------
// ï‘ëóíl	0	ÅF	ê≥èÌèIóπ
//-----------------------------------------------------------------------------
int CH31SnFhyo1_02Data::ClearManualInputSign(BOOL isChgKzritu)
{
//	Sn_F102_2c = 0;//yoshida190909

	//if (isChgKzritu) {
	//	Sn_F102_2c |= 0x111;
	//}

	return 0;
}

//===============================================
// ãåóléÆÇÃÉfÅ[É^éÊìæ
//-----------------------------------------------
// à¯êî
//		pDBNpSub	:	åˆâvÉNÉâÉX
//		sn_seq		:	óöóÉVÅ[ÉPÉìÉX
//		pSnHeadData	:	è¡îÔê≈ÉwÉbÉ_èÓïÒ
//-----------------------------------------------
// ï‘ëóíl
//			0		:	ê≥èÌèIóπ
//			-1		:	ÉGÉâÅ[
//===============================================
int CH31SnFhyo1_02Data::GetPastData ( CDBNpSub *pDBNpSub, int sn_seq, CSnHeadData *pSnHeadData )
{
	ASSERT ( pDBNpSub );
	if ( pDBNpSub == NULL ) {
		return -1;
	}

	ASSERT ( pDBNpSub->zvol );
	if ( pDBNpSub->zvol == NULL ) {
		return -1;
	}

	ASSERT ( pSnHeadData );
	if ( pSnHeadData == NULL ) {
		return -1;
	}

	EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki ( pDBNpSub->zvol );
	if ( KzHoushiki == ID_ICSSH_MENZEI ) {
		return 0;
	}

	// òAìÆéûÇÕãåóléÆÇÃÉfÅ[É^ÇéÊìæÇµÇ»Ç¢
	if ( !( pSnHeadData->Sn_Sign4 & 0x80 ) ) {
		return 0;
	}

	int nowVersion = 1;
	CString filter;

	if ( pSnHeadData->SVmzsw == 1 ) {
		filter.Format ( _T( "sn_seq = %d and hyoId = \'%s\' and version = %d" ), sn_seq, ICS_SN_FHYO_1_HYOID, nowVersion );
	}
	else {
		filter.Format ( _T( "( sn_seq = %d and version = %d ) and ( hyoId = \'%s\' or hyoId = \'%s\' )" ), sn_seq, nowVersion, ICS_SN_FHYO_1_HYOID, ICS_SN_IPPAN_HONPYO_HYOID );
	}

	if ( m_DBSyzAccess.m_pSH_LST_VAL ) {
		m_DBSyzAccess.m_pSH_LST_VAL->Requery ( filter );
	}
	else {
		if ( m_DBSyzAccess.OpenShLstVal ( pDBNpSub->m_database, filter ) ) {
			return -1;
		}
	}

	CSH_LST_VAL *pShLstVal = m_DBSyzAccess.m_pSH_LST_VAL;

	for ( int idx = 0; ; idx++ ) {
		if ( idx == 0 ) {
			if ( ( pShLstVal->MoveFirst() == -1 ) || ( pShLstVal->st == -1 ) ) {
				break;
			}
		}
		else {
			if ( ( pShLstVal->MoveNext() == -1 ) || ( pShLstVal->st == -1 ) ) {
				break;
			}
		}

		if ( pSnHeadData->IsNeedTkkzDisp() ) {	// ì¡íËâ€ê≈édì¸
			// ì¡íËâ€ê≈édì¸ÇÍÇ…åWÇÈéxï•Ç¢ëŒâøÇÃäz
			if ( pShLstVal->itmId == _T( "ACB00110" ) ) {
				m_Util.val_to_bin ( Sn_F102_1_2C, pShLstVal->val );
			}
			// ì¡íËâ€ê≈édì¸ÇÍÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
			else if ( pShLstVal->itmId == _T( "ACE00270" ) ) {
				m_Util.val_to_bin ( Sn_F102_5_2C, pShLstVal->val );
			}
		}

		if ( pSnHeadData->SVmzsw == 1 ) {	// åoâﬂë[íuëŒè€ÇÃâ€ê≈éëéYÇÃè˜ìnìôóLÇË
			// è¡îÔê≈äz
			if ( pShLstVal->itmId == _T( "ACC00010" ) ) {
				m_Util.val_to_bin ( Sn_F102_2A, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "ACC00020" ) ) {
				m_Util.val_to_bin ( Sn_F102_2B, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "ACC00030" ) ) {
				m_Util.val_to_bin ( Sn_F102_2C, pShLstVal->val );
				Sn_F102_2c = pShLstVal->sign1;
			}
			// ë›ì|ÇÍÇ…åWÇÈê≈äz
			else if ( pShLstVal->itmId == _T( "ACE00120" ) ) {
				m_Util.val_to_bin ( Sn_F102_6A, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "ACE00130" ) ) {
				m_Util.val_to_bin ( Sn_F102_6B, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "ACE00140" ) ) {
				m_Util.val_to_bin ( Sn_F102_6C, pShLstVal->val );
			}
		}
		else {
			// è¡îÔê≈äz
			if ( pShLstVal->itmId == _T( "AAB00020" ) ) {
				m_Util.val_to_bin ( Sn_F102_2C, pShLstVal->val );
				Sn_F102_2c = pShLstVal->sign1;

				// éËì¸óÕÉTÉCÉìçXêV
				if ( Sn_F102_2c & 0x08 ) {
					Sn_F102_2c &= ~0x08;
					Sn_F102_2c |= 0x800;
				}
			}
			// ë›ì|ÇÍÇ…åWÇÈê≈äz
			else if ( pShLstVal->itmId == _T( "AAB00070" ) ) {
				m_Util.val_to_bin ( Sn_F102_6C, pShLstVal->val );
			}
		}

		// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
		if ( pShLstVal->itmId == _T( "ACB00060" ) ) {
			m_Util.val_to_bin ( Sn_F102_1_1A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "ACB00070" ) ) {
			m_Util.val_to_bin ( Sn_F102_1_1B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "ACB00080" ) ) {
			m_Util.val_to_bin ( Sn_F102_1_1C, pShLstVal->val );
		}
		// îÑè„ÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
		else if ( pShLstVal->itmId == _T( "ACE00220" ) ) {
			m_Util.val_to_bin ( Sn_F102_5_1A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "ACE00230" ) ) {
			m_Util.val_to_bin ( Sn_F102_5_1B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "ACE00240" ) ) {
			m_Util.val_to_bin ( Sn_F102_5_1C, pShLstVal->val );
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// DBè„Ç…ÉfÅ[É^óLÇËÅH
//-----------------------------------------------------------------------------
// à¯êî		pDBNpSubÅF	ç‡ñ±ÉNÉâÉXÇÃÉ|ÉCÉìÉ^
//			sn_seq	ÅF	É`ÉFÉbÉNëŒè€ÉVÅ[ÉPÉìÉXÅi-1:ëSÉfÅ[É^Åj
//-----------------------------------------------------------------------------
// ï‘ëóíl	true	ÅF	ë∂ç›Ç∑ÇÈ
//			false	ÅF	ë∂ç›ÇµÇ»Ç¢
//-----------------------------------------------------------------------------
bool CH31SnFhyo1_02Data::IsExistData(CDBNpSub *pDBNpSub, int sn_seq)
{
	ASSERT(pDBNpSub);
	if( pDBNpSub == NULL ) {
		return false;
	}

	int nowVersion = 2;
	CString filter;
	if( sn_seq != -1 ){
		filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_1_02_HYOID, nowVersion);
	}
	else{
		filter.Format(_T("hyoId = \'%s\' and version = %d"), ICS_SN_FHYO_1_02_HYOID, nowVersion);
	}

	if( m_DBSyzAccess.m_pSH_LST_VAL ){
		m_DBSyzAccess.m_pSH_LST_VAL->Requery(filter);
	}
	else{
		if( m_DBSyzAccess.OpenShLstVal(pDBNpSub->m_database, filter) ){
			return false;
		}
	}

	if( m_DBSyzAccess.m_pSH_LST_VAL->GetRecordCount() ){
		return true;
	}
	else{
		return false;
	}
}
