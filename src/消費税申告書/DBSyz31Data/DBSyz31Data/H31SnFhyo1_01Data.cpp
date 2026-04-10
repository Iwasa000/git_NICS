#include "StdAfx.h"
#include "H31SnFhyo1_01Data.h"

static SH_TAGNAME tagData101[] = {
	// â€ê≈ïWèÄäz
	{ _T("CQB00010"), 0 },
	{ _T("CQB00020"), 1 },
	{ _T("CQB00030"), 3 },
	{ _T("CQB00040"), 999 },
	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
	{ _T("CQC00020"), 0 },
	{ _T("CQC00030"), 1 },
	{ _T("CQC00040"), 3 },
	{ _T("CQC00050"), 999 },
	// ì¡íËâ€ê≈édì¸ÇÍÇ…åWÇÈéxï•Ç¢ëŒâøÇÃäz
	{ _T("CQC00070"), 0 },
	{ _T("CQC00080"), 3 },
	{ _T("CQC00090"), 999 },
	// è¡îÔê≈äz
	{ _T("CQD00010"), 0 },
	{ _T("CQD00020"), 1 },
	{ _T("CQD00030"), 3 },
	{ _T("CQD00040"), 999 },
	// çTèúâﬂëÂí≤êÆê≈äz
	{ _T("CQE00010"), 0 },
	{ _T("CQE00020"), 1 },
	{ _T("CQE00030"), 3 },
	{ _T("CQE00040"), 999 },
	// çTèúëŒè€édì¸ê≈äz
	{ _T("CQF00020"), 0 },
	{ _T("CQF00030"), 1 },
	{ _T("CQF00040"), 3 },
	{ _T("CQF00050"), 999 },
	// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
	{ _T("CQF00070"), 0 },
	{ _T("CQF00080"), 1 },
	{ _T("CQF00090"), 3 },
	{ _T("CQF00100"), 999 },
	// îÑè„ÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
	{ _T("CQF00130"), 0 },
	{ _T("CQF00140"), 1 },
	{ _T("CQF00150"), 3 },
	{ _T("CQF00160"), 999 },
	// ì¡íËâ€ê≈édì¸ÇÍÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
	{ _T("CQF00180"), 0 },
	{ _T("CQF00190"), 3 },
	{ _T("CQF00200"), 999 },
	// ë›ì|ÇÍÇ…åWÇÈê≈äz
	{ _T("CQF00220"), 0 },
	{ _T("CQF00230"), 1 },
	{ _T("CQF00240"), 3 },
	{ _T("CQF00250"), 999 },
	// çTèúê≈äzè¨åv
	{ _T("CQF00270"), 0 },
	{ _T("CQF00280"), 1 },
	{ _T("CQF00290"), 3 },
	{ _T("CQF00300"), 999 },
	// çTèúïsë´ä“ïtê≈äz
	{ _T("CQG00010"), 0 },
	{ _T("CQG00020"), 1 },
	{ _T("CQG00030"), 3 },
	{ _T("CQG00040"), 999 },
	// ç∑à¯ê≈äz
	{ _T("CQH00010"), 0 },
	{ _T("CQH00020"), 1 },
	{ _T("CQH00030"), 3 },
	{ _T("CQH00040"), 999 },
	// çáåvç∑à¯ê≈äz
	{ _T("CQI00000"), 999 },
	// çTèúïsë´ä“ïtê≈äz
	{ _T("CQJ00020"), 0 },
	{ _T("CQJ00030"), 3 },
	{ _T("CQJ00040"), 999 },
	// ç∑à¯ê≈äz
	{ _T("CQJ00060"), 0 },
	{ _T("CQJ00070"), 3 },
	{ _T("CQJ00080"), 999 },
	// çáåvç∑à¯ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
	{ _T("CQK00010"), 0 },
	{ _T("CQK00020"), 3 },
	{ _T("CQK00030"), 999 },
	// ä“ïtê≈äz
	{ _T("CQL00020"), 0 },
	{ _T("CQL00030"), 3 },
	{ _T("CQL00040"), 999 },
	// î[ê≈äz
	{ _T("CQL00060"), 0 },
	{ _T("CQL00070"), 3 },
	{ _T("CQL00080"), 999 },
	// çáåvç∑à¯è˜ìnäÑäz
	{ _T("CQM00000"), 999 },

	{ _T("") }
};

CH31SnFhyo1_01Data::CH31SnFhyo1_01Data( BOOL isTransitionalMeasure)
	: m_isTransitionalMeasure( isTransitionalMeasure )
{
	memset(Sn_F101_1X, '\0', sizeof(Sn_F101_1X));
	memset(Sn_F101_1D, '\0', sizeof(Sn_F101_1D));
	memset(Sn_F101_1E, '\0', sizeof(Sn_F101_1E));
	memset(Sn_F101_1F, '\0', sizeof(Sn_F101_1F));
	memset(Sn_F101_1_1X, '\0', sizeof(Sn_F101_1_1X));
	memset(Sn_F101_1_1D, '\0', sizeof(Sn_F101_1_1D));
	memset(Sn_F101_1_1E, '\0', sizeof(Sn_F101_1_1E));
	memset(Sn_F101_1_1F, '\0', sizeof(Sn_F101_1_1F));
	memset(Sn_F101_1_2X, '\0', sizeof(Sn_F101_1_2X));
	memset(Sn_F101_1_2E, '\0', sizeof(Sn_F101_1_2E));
	memset(Sn_F101_1_2F, '\0', sizeof(Sn_F101_1_2F));
	memset(Sn_F101_2X, '\0', sizeof(Sn_F101_2X));
	memset(Sn_F101_2D, '\0', sizeof(Sn_F101_2D));
	memset(Sn_F101_2E, '\0', sizeof(Sn_F101_2E));
	memset(Sn_F101_2F, '\0', sizeof(Sn_F101_2F));
	memset(Sn_F101_3X, '\0', sizeof(Sn_F101_3X));
	memset(Sn_F101_3D, '\0', sizeof(Sn_F101_3D));
	memset(Sn_F101_3E, '\0', sizeof(Sn_F101_3E));
	memset(Sn_F101_3F, '\0', sizeof(Sn_F101_3F));
	memset(Sn_F101_4X, '\0', sizeof(Sn_F101_4X));
	memset(Sn_F101_4D, '\0', sizeof(Sn_F101_4D));
	memset(Sn_F101_4E, '\0', sizeof(Sn_F101_4E));
	memset(Sn_F101_4F, '\0', sizeof(Sn_F101_4F));
	memset(Sn_F101_5X, '\0', sizeof(Sn_F101_5X));
	memset(Sn_F101_5D, '\0', sizeof(Sn_F101_5D));
	memset(Sn_F101_5E, '\0', sizeof(Sn_F101_5E));
	memset(Sn_F101_5F, '\0', sizeof(Sn_F101_5F));
	memset(Sn_F101_5_1X, '\0', sizeof(Sn_F101_5_1X));
	memset(Sn_F101_5_1D, '\0', sizeof(Sn_F101_5_1D));
	memset(Sn_F101_5_1E, '\0', sizeof(Sn_F101_5_1E));
	memset(Sn_F101_5_1F, '\0', sizeof(Sn_F101_5_1F));
	memset(Sn_F101_5_2X, '\0', sizeof(Sn_F101_5_2X));
	memset(Sn_F101_5_2E, '\0', sizeof(Sn_F101_5_2E));
	memset(Sn_F101_5_2F, '\0', sizeof(Sn_F101_5_2F));
	memset(Sn_F101_6X, '\0', sizeof(Sn_F101_6X));
	memset(Sn_F101_6D, '\0', sizeof(Sn_F101_6D));
	memset(Sn_F101_6E, '\0', sizeof(Sn_F101_6E));
	memset(Sn_F101_6F, '\0', sizeof(Sn_F101_6F));
	memset(Sn_F101_7X, '\0', sizeof(Sn_F101_7X));
	memset(Sn_F101_7D, '\0', sizeof(Sn_F101_7D));
	memset(Sn_F101_7E, '\0', sizeof(Sn_F101_7E));
	memset(Sn_F101_7F, '\0', sizeof(Sn_F101_7F));
	memset(Sn_F101_8X, '\0', sizeof(Sn_F101_8X));
	memset(Sn_F101_8D, '\0', sizeof(Sn_F101_8D));
	memset(Sn_F101_8E, '\0', sizeof(Sn_F101_8E));
	memset(Sn_F101_8F, '\0', sizeof(Sn_F101_8F));
	memset(Sn_F101_9X, '\0', sizeof(Sn_F101_9X));
	memset(Sn_F101_9D, '\0', sizeof(Sn_F101_9D));
	memset(Sn_F101_9E, '\0', sizeof(Sn_F101_9E));
	memset(Sn_F101_9F, '\0', sizeof(Sn_F101_9F));
	memset(Sn_F101_10F, '\0', sizeof(Sn_F101_10F));
	memset(Sn_F101_11X, '\0', sizeof(Sn_F101_11X));
	memset(Sn_F101_11E, '\0', sizeof(Sn_F101_11E));
	memset(Sn_F101_11F, '\0', sizeof(Sn_F101_11F));
	memset(Sn_F101_12X, '\0', sizeof(Sn_F101_12X));
	memset(Sn_F101_12E, '\0', sizeof(Sn_F101_12E));
	memset(Sn_F101_12F, '\0', sizeof(Sn_F101_12F));
	memset(Sn_F101_13X, '\0', sizeof(Sn_F101_13X));
	memset(Sn_F101_13E, '\0', sizeof(Sn_F101_13E));
	memset(Sn_F101_13F, '\0', sizeof(Sn_F101_13F));
	memset(Sn_F101_14X, '\0', sizeof(Sn_F101_14X));
	memset(Sn_F101_14E, '\0', sizeof(Sn_F101_14E));
	memset(Sn_F101_14F, '\0', sizeof(Sn_F101_14F));
	memset(Sn_F101_15X, '\0', sizeof(Sn_F101_15X));
	memset(Sn_F101_15E, '\0', sizeof(Sn_F101_15E));
	memset(Sn_F101_15F, '\0', sizeof(Sn_F101_15F));
	memset(Sn_F101_16E, '\0', sizeof(Sn_F101_16E));
	memset(Sn_F101_16F, '\0', sizeof(Sn_F101_16F));

	Sn_F101_2c = 0;

	rsv = NULL;

	m_Arith.l_defn(0x16);

	memset(rsv1, '\0', sizeof(rsv1));
	memset(rsv2, '\0', sizeof(rsv2));

}

CH31SnFhyo1_01Data::~CH31SnFhyo1_01Data()
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
int CH31SnFhyo1_01Data::GetData(CDBNpSub *pDBNpSub, int sn_seq)
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


	int nowVersion = 2;
	CString filter = "";
	filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_1_01_HYOID, nowVersion);


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
		if (pShLstVal->itmId == _T( "CQB00010" )) {
			m_Util.val_to_bin(Sn_F101_1X, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T( "CQB00020" )) {
			m_Util.val_to_bin(Sn_F101_1D, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T( "CQB00030" )) {
			m_Util.val_to_bin(Sn_F101_1E, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("CQB00040")) {
			m_Util.val_to_bin(Sn_F101_1F, pShLstVal->val);
		}

		// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
		else if (pShLstVal->itmId == _T( "CQC00020" )) {
			m_Util.val_to_bin(Sn_F101_1_1X, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T( "CQC00030" )) {
			m_Util.val_to_bin(Sn_F101_1_1D, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T( "CQC00040" )) {
			m_Util.val_to_bin(Sn_F101_1_1E, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("CQC00050")) {
			m_Util.val_to_bin(Sn_F101_1_1F, pShLstVal->val);
		}

		// ì¡íËâ€ê≈édì¸ÇÍÇ…åWÇÈéxï•Ç¢ëŒâøÇÃäz
		else if (pShLstVal->itmId == _T( "CQC00070" )) {
			m_Util.val_to_bin(Sn_F101_1_2X, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T( "CQC00080" )) {
			m_Util.val_to_bin(Sn_F101_1_2E, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("CQC00090")) {
			m_Util.val_to_bin(Sn_F101_1_2F, pShLstVal->val);
		}


		// è¡îÔê≈äz
		else if (pShLstVal->itmId == _T( "CQD00010" )) {
			m_Util.val_to_bin(Sn_F101_2X, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T( "CQD00020" )) {
			m_Util.val_to_bin(Sn_F101_2D, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T( "CQD00030" )) {
			m_Util.val_to_bin(Sn_F101_2E, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("CQD00040")) {
			m_Util.val_to_bin(Sn_F101_2F, pShLstVal->val);
			Sn_F101_2c = pShLstVal->sign1;
		}


		// çTèúâﬂëÂí≤êÆê≈äz
		else if (pShLstVal->itmId == _T( "CQE00010" )) {	
			m_Util.val_to_bin(Sn_F101_3X, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T( "CQE00020" )) {
			m_Util.val_to_bin(Sn_F101_3D, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T( "CQE00030" )) {
			m_Util.val_to_bin(Sn_F101_3E, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("CQE00040")) {
			m_Util.val_to_bin(Sn_F101_3F, pShLstVal->val);
		}

		// çTèúëŒè€édì¸ê≈äz
		else if (pShLstVal->itmId == _T( "CQF00020" )) {
			m_Util.val_to_bin(Sn_F101_4X, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T( "CQF00030" )) {
			m_Util.val_to_bin(Sn_F101_4D, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T( "CQF00040" )) {
			m_Util.val_to_bin(Sn_F101_4E, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("CQF00050")) {
			m_Util.val_to_bin(Sn_F101_4F, pShLstVal->val);
		}

		// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
		else if (pShLstVal->itmId == _T( "CQF00070" )) {
			m_Util.val_to_bin(Sn_F101_5X, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T( "CQF00080" )) {
			m_Util.val_to_bin(Sn_F101_5D, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T( "CQF00090" )) {
			m_Util.val_to_bin(Sn_F101_5E, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("CQF00100")) {
			m_Util.val_to_bin(Sn_F101_5F, pShLstVal->val);
		}

		// îÑè„ÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
		else if (pShLstVal->itmId == _T( "CQF00130" )) {
			m_Util.val_to_bin(Sn_F101_5_1X, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T( "CQF00140" )) {
			m_Util.val_to_bin(Sn_F101_5_1D, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T( "CQF00150" )) {
			m_Util.val_to_bin(Sn_F101_5_1E, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("CQF00160")) {
			m_Util.val_to_bin(Sn_F101_5_1F, pShLstVal->val);
		}

		// ì¡íËâ€ê≈édì¸ÇÍÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
		else if (pShLstVal->itmId == _T("CQF00180")) {
			m_Util.val_to_bin(Sn_F101_5_2X, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T( "CQF00190" )) {
			m_Util.val_to_bin(Sn_F101_5_2E, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("CQF00200")) {
			m_Util.val_to_bin(Sn_F101_5_2F, pShLstVal->val);
		}

		// ë›ì|ÇÍÇ…åWÇÈê≈äz
		else if (pShLstVal->itmId == _T( "CQF00220" )) {
			m_Util.val_to_bin(Sn_F101_6X, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T( "CQF00230" )) {
			m_Util.val_to_bin(Sn_F101_6D, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T( "CQF00240" )) {
			m_Util.val_to_bin(Sn_F101_6E, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("CQF00250")) {
			m_Util.val_to_bin(Sn_F101_6F, pShLstVal->val);
		}

		// çTèúê≈äzè¨åv
		else if (pShLstVal->itmId == _T( "CQF00270" )) {
			m_Util.val_to_bin(Sn_F101_7X, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T( "CQF00280" )) {
			m_Util.val_to_bin(Sn_F101_7D, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T( "CQF00290" )) {
			m_Util.val_to_bin(Sn_F101_7E, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("CQF00300")) {
			m_Util.val_to_bin(Sn_F101_7F, pShLstVal->val);
		}

		// çTèúïsë´ä“ïtê≈äz
		else if (pShLstVal->itmId == _T( "CQG00010" )) {
			m_Util.val_to_bin(Sn_F101_8X, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T( "CQG00020" )) {
			m_Util.val_to_bin(Sn_F101_8D, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T( "CQG00030" )) {
			m_Util.val_to_bin(Sn_F101_8E, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("CQG00040")) {
			m_Util.val_to_bin(Sn_F101_8F, pShLstVal->val);
		}

		// ç∑à¯ê≈äz
		else if (pShLstVal->itmId == _T( "CQH00010" )) {
			m_Util.val_to_bin(Sn_F101_9X, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T( "CQH00020" )) {
			m_Util.val_to_bin(Sn_F101_9D, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T( "CQH00030" )) {
			m_Util.val_to_bin(Sn_F101_9E, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("CQH00040")) {
			m_Util.val_to_bin(Sn_F101_9F, pShLstVal->val);
		}

		// çáåvç∑à¯ê≈äz
		else if ( pShLstVal->itmId == _T( "CQI00000" ) ) {
			m_Util.val_to_bin(Sn_F101_10F, pShLstVal->val);
		}

		// ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
		// çTèúïsë´ä“ïtê≈äz
		else if (pShLstVal->itmId == _T( "CQJ00020" )) {
			m_Util.val_to_bin(Sn_F101_11X, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T( "CQJ00030" )) {
			m_Util.val_to_bin(Sn_F101_11E, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("CQJ00040")) {
			m_Util.val_to_bin(Sn_F101_11F, pShLstVal->val);
		}

		// ç∑à¯ê≈äz
		else if (pShLstVal->itmId == _T( "CQJ00060" )) {
			m_Util.val_to_bin(Sn_F101_12X, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T( "CQJ00070" )) {
			m_Util.val_to_bin(Sn_F101_12E, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("CQJ00080")) {
			m_Util.val_to_bin(Sn_F101_12F, pShLstVal->val);
		}

		// çáåvç∑à¯ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
		else if (pShLstVal->itmId == _T( "CQK00010" )) {
			m_Util.val_to_bin(Sn_F101_13X, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T( "CQK00020" )) {
			m_Util.val_to_bin(Sn_F101_13E, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("CQK00030")) {
			m_Util.val_to_bin(Sn_F101_13F, pShLstVal->val);
		}

		// è˜ìnäÑäz
		// ä“ïtäz
		else if (pShLstVal->itmId == _T( "CQL00020" )) {
			m_Util.val_to_bin(Sn_F101_14X, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T( "CQL00030" )) {
			m_Util.val_to_bin(Sn_F101_14E, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("CQL00040")) {
			m_Util.val_to_bin(Sn_F101_14F, pShLstVal->val);
		}

		// î[ê≈äz
		else if (pShLstVal->itmId == _T( "CQL00060" )) {
			m_Util.val_to_bin(Sn_F101_15X, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T( "CQL00070" )) {
			m_Util.val_to_bin(Sn_F101_15E, pShLstVal->val);
		}
		else if (pShLstVal->itmId == _T("CQL00080")) {
			m_Util.val_to_bin(Sn_F101_15F, pShLstVal->val);
		}

		// çáåvç∑à¯è˜ìnäÑäz
		/*else if (pShLstVal->itmId == _T("ACL00030")) {
			m_Util.val_to_bin(Sn_F101_16E, pShLstVal->val);
		}*/
		else if ( pShLstVal->itmId == _T( "CQM00000" ) ) {
			m_Util.val_to_bin(Sn_F101_16F, pShLstVal->val);
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
int CH31SnFhyo1_01Data::SetData( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT( pDBNpSub );
	if ( pDBNpSub == NULL ) {
		return -1;
	}

	ASSERT( pDBNpSub->zvol );
	if ( pDBNpSub->zvol == NULL ) {
		return -1;
	}

	CString filter = "";
	char	*pHyoId = NULL;
	int		nowVersion = 2;
	filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_1_01_HYOID, nowVersion);
	pHyoId = ICS_SN_FHYO_1_01_HYOID;

	EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki( pDBNpSub->zvol );
	if ( KzHoushiki == ID_ICSSH_MENZEI) {
		return 0;
	}

	if ( m_DBSyzAccess.m_pSH_LST_VAL ) {
		m_DBSyzAccess.m_pSH_LST_VAL->Requery(filter);
	}
	else {
		if ( m_DBSyzAccess.OpenShLstVal( pDBNpSub->m_database, filter ) ) {
			return -1;
		}
	}


	CSH_LST_VAL		*pShLstVal = m_DBSyzAccess.m_pSH_LST_VAL;

	if (( pShLstVal->MoveFirst() == -1 ) || (pShLstVal->st == -1)) {	// ÉfÅ[É^Ç»Çµ
		
		// â€ê≈ïWèÄäz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T( "CQB00010" );
		m_Util.val_to_asci( &pShLstVal->val, Sn_F101_1X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQB00020");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_1D);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQB00030");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_1E);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQB00040");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_1F);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQC00020");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_1_1X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQC00030");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_1_1D);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQC00040");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_1_1E);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQC00050");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_1_1F);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ì¡íËâ€ê≈édì¸ÇÍÇ…åWÇÈéxï•Ç¢ëŒâøÇÃäz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQC00070");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_1_2X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQC00080");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_1_2E);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQC00090");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_1_2F);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// è¡îÔê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQD00010");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_2X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQD00020");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_2D);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQD00030");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_2E);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQD00040");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_2F);
		pShLstVal->sign1 = Sn_F101_2c;
		pShLstVal->Update();

		// çTèúâﬂëÂí≤êÆê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQE00010");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_3X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQE00020");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_3D);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQE00030");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_3E);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQE00040");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_3F);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// çTèúëŒè€édì¸ê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQF00020");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_4X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQF00030");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_4D);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQF00040");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_4E);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQF00050");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_4F);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQF00070");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_5X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQF00080");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_5D);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQF00090");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_5E);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQF00100");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_5F);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// îÑè„ÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQF00130");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_5_1X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQF00140");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_5_1D);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQF00150");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_5_1E);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQF00160");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_5_1F);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ì¡íËâ€ê≈édì¸ÇÍÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQF00180");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_5_2X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQF00190");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_5_2E);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQF00200");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_5_2F);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ë›ì|Ç…åWÇÈê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQF00220");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_6X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQF00230");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_6D);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQF00240");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_6E);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQF00250");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_6F);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// çTèúê≈äzè¨åv
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQF00270");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_7X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQF00280");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_7D);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQF00290");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_7E);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQF00300");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_7F);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// çTèúïsë´ä“ïtê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQG00010");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_8X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQG00020");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_8D);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQG00030");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_8E);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQG00040");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_8F);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ç∑à¯ê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQH00010");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_9X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQH00020");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_9D);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQH00030");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_9E);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQH00040");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_9F);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// çáåvç∑à¯ê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQI00000");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_10F);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// çTèúïsë´ä“ïtê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQJ00020");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_11X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQJ00030");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_11E);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQJ00040");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_11F);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ç∑à¯ê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQJ00060");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_12X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQJ00070");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_12E);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQJ00080");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_12F);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// çáåvç∑à¯ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQK00010");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_13X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQK00020");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_13E);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQK00030");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_13F);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ä“ïtäz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQL00020");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_14X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQL00030");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_14E);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQL00040");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_14F);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// î[ê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQL00060");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_15X);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQL00070");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_15E);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQL00080");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_15F);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// çáåvç∑à¯è˜ìnäÑäz
		/*
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("ACL00030");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_16E);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();
		*/

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoId;
		pShLstVal->itmId = _T("CQM00000");
		m_Util.val_to_asci(&pShLstVal->val, Sn_F101_16F);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();
	}
	else {
		CStringArray	updTagAry;

		for (int i = 0; ; i++ ) {
			if ( i == 0 ) {
				if ( (pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1) ) {
					break;
				}
			}
			else {
				if ( (pShLstVal->MoveNext() == -1) || (pShLstVal->st == -1) ) {
					break;
				}
			}

			// â€ê≈ïWèÄäz
			if ( pShLstVal->itmId == _T( "CQB00010" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_F101_1X);
				pShLstVal->Update();

				updTagAry.Add( pShLstVal->itmId );
			}
			else if (pShLstVal->itmId == _T("CQB00020")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_1D);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQB00030")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_1E);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQB00040")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_1F);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
			else if (pShLstVal->itmId == _T("CQC00020")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_1_1X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQC00030")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_1_1D);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQC00040")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_1_1E);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQC00050")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_1_1F);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// ì¡íËâ€ê≈édì¸ÇÍÇ…åWÇÈéxï•Ç¢ëŒâøÇÃäz
			else if (pShLstVal->itmId == _T("CQC00070")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_1_2X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQC00080")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_1_2E);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQC00090")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_1_2F);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// è¡îÔê≈äz
			else if (pShLstVal->itmId == _T("CQD00010")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_2X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQD00020")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_2D);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQD00030")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_2E);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQD00040")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_2F);
				pShLstVal->sign1 = Sn_F101_2c;
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// çTèúâﬂëÂí≤êÆê≈äz
			else if (pShLstVal->itmId == _T("CQE00010")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_3X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQE00020")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_3D);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQE00030")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_3E);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQE00040")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_3F);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// çTèúëŒè€édì¸ê≈äz
			else if (pShLstVal->itmId == _T("CQF00020")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_4X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQF00030")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_4D);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQF00040")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_4E);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQF00050")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_4F);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
			else if (pShLstVal->itmId == _T("CQF00070")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_5X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQF00080")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_5D);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQF00090")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_5E);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQF00100")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_5F);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// îÑè„ÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
			else if (pShLstVal->itmId == _T("CQF00130")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_5_1X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQF00140")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_5_1D);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQF00150")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_5_1E);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQF00160")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_5_1F);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// ì¡íËâ€ê≈édì¸ÇÍÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
			else if (pShLstVal->itmId == _T("CQF00180")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_5_2X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQF00190")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_5_2E);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQF00200")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_5_2F);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// ë›ì|ÇÍÇ…åWÇÈê≈äz
			else if (pShLstVal->itmId == _T("CQF00220")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_6X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQF00230")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_6D);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQF00240")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_6E);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQF00250")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_6F);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// çTèúê≈äzè¨åv
			else if (pShLstVal->itmId == _T("CQF00270")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_7X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQF00280")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_7D);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQF00290")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_7E);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQF00300")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_7F);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// çTèúïsë´ä“ïtê≈äz
			else if (pShLstVal->itmId == _T("CQG00010")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_8X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQG00020")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_8D);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQG00030")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_8E);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQG00040")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_8F);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// ç∑à¯ê≈äz
			else if (pShLstVal->itmId == _T("CQH00010")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_9X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQH00020")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_9D);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQH00030")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_9E);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQH00040")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_9F);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// çáåvç∑à¯ê≈äz
			else if (pShLstVal->itmId == _T("CQI00000")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_10F);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// çTèúïsë´ä“ïtê≈äz
			else if (pShLstVal->itmId == _T("CQJ00020")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_11X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQJ00030")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_11E);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQJ00040")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_11F);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// ç∑à¯ê≈äz
			else if (pShLstVal->itmId == _T("CQJ00060")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_12X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQJ00070")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_12E);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQJ00080")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_12F);
				pShLstVal->Update();
			
				updTagAry.Add(pShLstVal->itmId);
			}

			// çáåvç∑à¯ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈ê≈äz
			else if (pShLstVal->itmId == _T("CQK00010")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_13X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQK00020")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_13E);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQK00030")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_13F);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// ä“ïtäz
			else if (pShLstVal->itmId == _T("CQL00020")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_14X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQL00030")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_14E);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQL00040")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_14F);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// î[ê≈äz
			else if (pShLstVal->itmId == _T("CQL00060")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_15X);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQL00070")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_15E);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
			else if (pShLstVal->itmId == _T("CQL00080")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_15F);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}

			// çáåvç∑à¯è˜ìnäÑäz
			/*
			else if (pShLstVal->itmId == _T("ACL00030")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_16E);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}*/
			else if (pShLstVal->itmId == _T("CQM00000")) {
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_F101_16F);
				pShLstVal->Update();

				updTagAry.Add(pShLstVal->itmId);
			}
		}

		int adCnt = 0;
		for (int i = 0; ; i++ ) {
			if ( strlen( tagData101[i].tagName ) == 0 ) {
				break;
			}
			adCnt++;
		}

		int updCnt = (int)updTagAry.GetCount();
		if (adCnt != updCnt) {
			CArray<unsigned char*> MonyPrtAry;

			MonyPrtAry.Add(Sn_F101_1X);
			MonyPrtAry.Add(Sn_F101_1D);
			MonyPrtAry.Add(Sn_F101_1E);
			MonyPrtAry.Add(Sn_F101_1F);
			MonyPrtAry.Add(Sn_F101_1_1X);
			MonyPrtAry.Add(Sn_F101_1_1D);
			MonyPrtAry.Add(Sn_F101_1_1E);
			MonyPrtAry.Add(Sn_F101_1_1F);
			MonyPrtAry.Add(Sn_F101_1_2X);
			MonyPrtAry.Add(Sn_F101_1_2E);
			MonyPrtAry.Add(Sn_F101_1_2F);
			MonyPrtAry.Add(Sn_F101_2X);
			MonyPrtAry.Add(Sn_F101_2D);
			MonyPrtAry.Add(Sn_F101_2E);
			MonyPrtAry.Add(Sn_F101_2F);
			MonyPrtAry.Add(Sn_F101_3X);
			MonyPrtAry.Add(Sn_F101_3D);
			MonyPrtAry.Add(Sn_F101_3E);
			MonyPrtAry.Add(Sn_F101_3F);
			MonyPrtAry.Add(Sn_F101_4X);
			MonyPrtAry.Add(Sn_F101_4D);
			MonyPrtAry.Add(Sn_F101_4E);
			MonyPrtAry.Add(Sn_F101_4F);
			MonyPrtAry.Add(Sn_F101_5X);
			MonyPrtAry.Add(Sn_F101_5D);
			MonyPrtAry.Add(Sn_F101_5E);
			MonyPrtAry.Add(Sn_F101_5F);
			MonyPrtAry.Add(Sn_F101_5_1X);
			MonyPrtAry.Add(Sn_F101_5_1D);
			MonyPrtAry.Add(Sn_F101_5_1E);
			MonyPrtAry.Add(Sn_F101_5_1F);
			MonyPrtAry.Add(Sn_F101_5_2X);
			MonyPrtAry.Add(Sn_F101_5_2E);
			MonyPrtAry.Add(Sn_F101_5_2F);
			MonyPrtAry.Add(Sn_F101_6X);
			MonyPrtAry.Add(Sn_F101_6D);
			MonyPrtAry.Add(Sn_F101_6E);
			MonyPrtAry.Add(Sn_F101_6F);
			MonyPrtAry.Add(Sn_F101_7X);
			MonyPrtAry.Add(Sn_F101_7D);
			MonyPrtAry.Add(Sn_F101_7E);
			MonyPrtAry.Add(Sn_F101_7F);
			MonyPrtAry.Add(Sn_F101_8X);
			MonyPrtAry.Add(Sn_F101_8D);
			MonyPrtAry.Add(Sn_F101_8E);
			MonyPrtAry.Add(Sn_F101_8F);
			MonyPrtAry.Add(Sn_F101_9X);
			MonyPrtAry.Add(Sn_F101_9D);
			MonyPrtAry.Add(Sn_F101_9E);
			MonyPrtAry.Add(Sn_F101_9F);
			MonyPrtAry.Add(Sn_F101_10F);
			MonyPrtAry.Add(Sn_F101_11X);
			MonyPrtAry.Add(Sn_F101_11E);
			MonyPrtAry.Add(Sn_F101_11F);
			MonyPrtAry.Add(Sn_F101_12X);
			MonyPrtAry.Add(Sn_F101_12E);
			MonyPrtAry.Add(Sn_F101_12F);
			MonyPrtAry.Add(Sn_F101_13X);
			MonyPrtAry.Add(Sn_F101_13E);
			MonyPrtAry.Add(Sn_F101_13F);
			MonyPrtAry.Add(Sn_F101_14X);
			MonyPrtAry.Add(Sn_F101_14E);
			MonyPrtAry.Add(Sn_F101_14F);
			MonyPrtAry.Add(Sn_F101_15X);
			MonyPrtAry.Add(Sn_F101_15E);
			MonyPrtAry.Add(Sn_F101_15F);

			//MonyPrtAry.Add(Sn_F101_16E);
			MonyPrtAry.Add(Sn_F101_16F);

			BOOL findFlg = FALSE;
			for (int i = 0; ; i++) {
				if (strlen(tagData101[i].tagName) == 0) {
					break;
				}

				findFlg = FALSE;
				for (int j = 0; j < updCnt; j++) {
					if (updTagAry[j].CompareNoCase(tagData101[j].tagName) == 0) {
						findFlg = TRUE;
						break;
					}

					if (findFlg == FALSE) {
						// äYìñÉ^ÉOÇÃí«â¡
						pShLstVal->AddNew();
						pShLstVal->sn_seq = sn_seq;
						pShLstVal->version = nowVersion;
						pShLstVal->hyoId = pHyoId;
						pShLstVal->itmId = tagData101[i].tagName;
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

//=======================================================
// èWåvçœÇ›ÉfÅ[É^ÇÃÉZÉbÉg(ïtï\1Å|1)
//-------------------------------------------------------
// à¯êî		pDBNpSub	ÅF	ç‡ñ±ÉNÉâÉX
//			pSyzSyukeiÅ@ÅFÅ@è¡îÔê≈èWåvÉNÉâÉX
//			isPluralZei	ÅFÅ@ï°êîê≈ó¶
//-------------------------------------------------------
// ï‘ëóíl	 0			ÅFÅ@ê≥èÌèIóπ
//			-1			ÅFÅ@ÉGÉâÅ[
//=======================================================
int CH31SnFhyo1_01Data::SetCalqed1_01Data(CSyzSyukei *pSyzSyukei, BOOL &isPluralZei)
{
	isPluralZei = FALSE;

	MoneyBasejagArray	money;

	money = pSyzSyukei->GetShinkokuData(_T("31"));

	int cnt = (int)money.GetCount();
	int incnt = 0;

	unsigned char exZeiflg = 0x00;	// D0ÅFãåê≈ó¶
									// D1ÅF6.24%
									// D2ÅF7.8%
									// D3ÅFçáåv

	for (int i = 0; i < cnt; i++) {
		incnt = (int)money[i].GetCount();
		for (int j = 0; j < incnt; j++) {

			// â€ê≈ïWèÄäz
			if (money[i][j].code.Left(4) == _T("3121")) {
				if (money[i][j].code == _T("312101")) {
					memmove(Sn_F101_1X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("312102")) {
					memmove(Sn_F101_1D, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("312103")) {
					memmove(Sn_F101_1E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("312100")) {
					memmove(Sn_F101_1F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
			else if (money[i][j].code.Left(4) == _T("3122")) {
				if (money[i][j].code == _T("312201")) {
					memmove(Sn_F101_1_1X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("312202")) {
					memmove(Sn_F101_1_1D, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("312203")) {
					memmove(Sn_F101_1_1E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("312200")) {
					memmove(Sn_F101_1_1F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// ì¡íËâ€ê≈édì¸ÇÍÇ…åWÇÈéxï•Ç¢ëŒâøÇÃäz
			else if (money[i][j].code.Left(4) == _T("3123")) {
				if ( money[i][j].code == _T("312301") ) {
					memmove(Sn_F101_1_2X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("312303")) {
					memmove(Sn_F101_1_2E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("312300")) {
					memmove(Sn_F101_1_2F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// è¡îÔê≈äz
			else if (money[i][j].code.Left(4) == _T("3124")) {
				if (money[i][j].code == _T("312401")) {
					memmove(Sn_F101_2X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				
				else if (money[i][j].code == _T("312402")) {
					memmove(Sn_F101_2D, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("312403")) {
					memmove(Sn_F101_2E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("312400")) {
					memmove(Sn_F101_2F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// çTèúâﬂëÂí≤êÆê≈äz
			else if (money[i][j].code.Left(4) == _T("3125")) {
				if (money[i][j].code == _T("312501")) {
					memmove(Sn_F101_3X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("312502")) {
					memmove(Sn_F101_3D, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("312503")) {
					memmove(Sn_F101_3E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("312500")) {
					memmove(Sn_F101_3F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// çTèúëŒè€édì¸ê≈äz
			else if (money[i][j].code.Left(4) == _T("3126")) {
				if (money[i][j].code == _T("312601")) {
					memmove(Sn_F101_4X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("312602")) {
					memmove(Sn_F101_4D, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("312603")) {
					memmove(Sn_F101_4E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("312600")) {
					memmove(Sn_F101_4F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
			else if (money[i][j].code.Left(4) == _T("3127")) {
				if (money[i][j].code == _T("312701")) {
					memmove(Sn_F101_5X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("312702")) {
					memmove(Sn_F101_5D, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("312703")) {
					memmove(Sn_F101_5E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("312700")) {
					memmove(Sn_F101_5F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// îÑè„ÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
			else if (money[i][j].code.Left(4) == _T("3128")) {
				if (money[i][j].code == _T("312801")) {
					memmove(Sn_F101_5_1X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("312802")) {
					memmove(Sn_F101_5_1D, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("312803")) {
					memmove(Sn_F101_5_1E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("312800")) {
					memmove(Sn_F101_5_1F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// ì¡íËâ€ê≈édì¸ÇÍÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
			else if (money[i][j].code.Left(4) == _T("3129")) {
				if (money[i][j].code == _T("312901")) {
					memmove(Sn_F101_5_2X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("312903")) {
					memmove(Sn_F101_5_2E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("312900")) {
					memmove(Sn_F101_5_2F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// ë›ì|ÇÍÇ…åWÇÈê≈äz
			else if (money[i][j].code.Left(4) == _T("3130")) {
				if (money[i][j].code == _T("313001")) {
					memmove(Sn_F101_6X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("313002")) {
					memmove(Sn_F101_6D, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("313003")) {
					memmove(Sn_F101_6E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("313000")) {
					memmove(Sn_F101_6F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// çTèúê≈äzè¨åv
			else if (money[i][j].code.Left(4) == _T("3131")) {
				if (money[i][j].code == _T("313101")) {
					memmove(Sn_F101_7X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("313102")) {
					memmove(Sn_F101_7D, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("313103")) {
					memmove(Sn_F101_7E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("313100")) {
					memmove(Sn_F101_7F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// çTèúïsë´ä“ïtê≈äz
			else if (money[i][j].code.Left(4) == _T("3132")) {
				if (money[i][j].code == _T("313201")) {
					memmove(Sn_F101_8X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("313202")) {
					memmove(Sn_F101_8D, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("313203")) {
					memmove(Sn_F101_8E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("313200")) {
					memmove(Sn_F101_8F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// ç∑à¯ê≈äz
			else if (money[i][j].code.Left(4) == _T("3133")) {
				if (money[i][j].code == _T("313301")) {
					memmove(Sn_F101_9X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("313302")) {
					memmove(Sn_F101_9D, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("313303")) {
					memmove(Sn_F101_9E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("313300")) {
					memmove(Sn_F101_9F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// çáåvç∑à¯ê≈äz
			else if (money[i][j].code.Left(4) == _T("3134")) {
				if (money[i][j].code == _T("313400")) {
					memmove(Sn_F101_10F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// çTèúïsë´ä“ïtê≈äz
			else if (money[i][j].code.Left(4) == _T("3135")) {
				if (money[i][j].code == _T("313501")) {
					memmove(Sn_F101_11X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}

				else if (money[i][j].code == _T("313503")) {
					memmove(Sn_F101_11E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("313500")) {
					memmove(Sn_F101_11F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// ç∑à¯ê≈äz
			else if (money[i][j].code.Left(4) == _T("3136")) {
				if (money[i][j].code == _T("313601")) {
					memmove(Sn_F101_12X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}

				else if (money[i][j].code == _T("313603")) {
					memmove(Sn_F101_12E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("313600")) {
					memmove(Sn_F101_12F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// çáåvç∑à¯ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
			else if (money[i][j].code.Left(4) == _T("3137")) {
				if (money[i][j].code == _T("313701")) {
					memmove(Sn_F101_13X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}

				else if (money[i][j].code == _T("313703")) {
					memmove(Sn_F101_13E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("313700")) {
					memmove(Sn_F101_13F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// ä“ïtäz
			else if (money[i][j].code.Left(4) == _T("3138")) {
				if (money[i][j].code == _T("313801")) {
					memmove(Sn_F101_14X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}

				else if (money[i][j].code == _T("313803")) {
					memmove(Sn_F101_14E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("313800")) {
					memmove(Sn_F101_14F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// î[ê≈äz
			else if (money[i][j].code.Left(4) == _T("3139")) {
				if (money[i][j].code == _T("313901")) {
					memmove(Sn_F101_15X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}

				else if (money[i][j].code == _T("313903")) {
					memmove(Sn_F101_15E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("313900")) {
					memmove(Sn_F101_15F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// çáåvç∑à¯è˜ìnäÑäz
			else if (money[i][j].code.Left(4) == _T("3140")) {
				/*if (money[i][j].code == _T("314003")) {
					memmove(Sn_F101_16E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}*/
				if (money[i][j].code == _T("314000")) {
					memmove(Sn_F101_16F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}
		}
	}

	if (((exZeiflg & 0x08) == 0x01) || ((exZeiflg & 0x08) == 0x02) || ((exZeiflg & 0x08) == 0x04) || ((exZeiflg & 0x08) == 0x08) || (exZeiflg == 0x00)) {
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
int CH31SnFhyo1_01Data::ClearDataByRenew(CDBNpSub *pDBNpSub, int type)
{
	m_Arith.l_clear(Sn_F101_1X);
	m_Arith.l_clear(Sn_F101_1D);
	m_Arith.l_clear(Sn_F101_1E);
	m_Arith.l_clear(Sn_F101_1F);

	m_Arith.l_clear(Sn_F101_1_1X);
	m_Arith.l_clear(Sn_F101_1_1D);
	m_Arith.l_clear(Sn_F101_1_1E);
	m_Arith.l_clear(Sn_F101_1_1F);

	m_Arith.l_clear(Sn_F101_1_2X);
	m_Arith.l_clear(Sn_F101_1_2E);
	m_Arith.l_clear(Sn_F101_1_2F);

	m_Arith.l_clear(Sn_F101_2X);
	m_Arith.l_clear(Sn_F101_2D);
	m_Arith.l_clear(Sn_F101_2E);
	m_Arith.l_clear(Sn_F101_2F);

	m_Arith.l_clear(Sn_F101_3X);
	m_Arith.l_clear(Sn_F101_3D);
	m_Arith.l_clear(Sn_F101_3E);
	m_Arith.l_clear(Sn_F101_3F);

	m_Arith.l_clear(Sn_F101_4X);
	m_Arith.l_clear(Sn_F101_4D);
	m_Arith.l_clear(Sn_F101_4E);
	m_Arith.l_clear(Sn_F101_4F);

	m_Arith.l_clear(Sn_F101_5X);
	m_Arith.l_clear(Sn_F101_5D);
	m_Arith.l_clear(Sn_F101_5E);
	m_Arith.l_clear(Sn_F101_5F);

	m_Arith.l_clear(Sn_F101_5_1X);
	m_Arith.l_clear(Sn_F101_5_1D);
	m_Arith.l_clear(Sn_F101_5_1E);
	m_Arith.l_clear(Sn_F101_5_1F);

	m_Arith.l_clear(Sn_F101_5_2X);
	m_Arith.l_clear(Sn_F101_5_2E);
	m_Arith.l_clear(Sn_F101_5_2F);

	m_Arith.l_clear(Sn_F101_6X);
	m_Arith.l_clear(Sn_F101_6D);
	m_Arith.l_clear(Sn_F101_6E);
	m_Arith.l_clear(Sn_F101_6F);

	m_Arith.l_clear(Sn_F101_7X);
	m_Arith.l_clear(Sn_F101_7D);
	m_Arith.l_clear(Sn_F101_7E);
	m_Arith.l_clear(Sn_F101_7F);

	m_Arith.l_clear(Sn_F101_8X);
	m_Arith.l_clear(Sn_F101_8D);
	m_Arith.l_clear(Sn_F101_8E);
	m_Arith.l_clear(Sn_F101_8F);

	m_Arith.l_clear(Sn_F101_9X);
	m_Arith.l_clear(Sn_F101_9D);
	m_Arith.l_clear(Sn_F101_9E);
	m_Arith.l_clear(Sn_F101_9F);

	m_Arith.l_clear(Sn_F101_10F);

	m_Arith.l_clear(Sn_F101_11X);
	m_Arith.l_clear(Sn_F101_11E);
	m_Arith.l_clear(Sn_F101_11F);

	m_Arith.l_clear(Sn_F101_12X);
	m_Arith.l_clear(Sn_F101_12E);
	m_Arith.l_clear(Sn_F101_12F);

	m_Arith.l_clear(Sn_F101_13X);
	m_Arith.l_clear(Sn_F101_13E);
	m_Arith.l_clear(Sn_F101_13F);

	m_Arith.l_clear(Sn_F101_14X);
	m_Arith.l_clear(Sn_F101_14E);
	m_Arith.l_clear(Sn_F101_14F);

	m_Arith.l_clear(Sn_F101_15X);
	m_Arith.l_clear(Sn_F101_15E);
	m_Arith.l_clear(Sn_F101_15F);

	m_Arith.l_clear(Sn_F101_16E);
	m_Arith.l_clear(Sn_F101_16F);

	Sn_F101_2c = 0;

	return 0;
}

//-----------------------------------------------------------------------------
// éËì¸óÕÉTÉCÉìÇÃÉNÉäÉA
//-----------------------------------------------------------------------------
// ï‘ëóíl	0	ÅF	ê≥èÌèIóπ
//-----------------------------------------------------------------------------
int CH31SnFhyo1_01Data::ClearManualInputSign(BOOL isChgKzritu)
{
//	Sn_F101_2c = 0;//yoshida190909


	//if (isChgKzritu) {
	//	Sn_F101_2c |= 0x111;
	//}

	return 0;
}

//-----------------------------------------------------------------------------
// çXêVéûÇÃã‡äzí«â¡èëÇ´çûÇ›
//-----------------------------------------------------------------------------
// à¯êî	pDBNpSub	ÅF	ç‡ñ±ÉNÉâÉXÇÃÉ|ÉCÉìÉ^
//		sn_seq		ÅF	ëŒè€ÇÃóöóÉVÅ[ÉPÉìÉX
//-----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	ê≥èÌèIóπ
//-----------------------------------------------------------------------------
int CH31SnFhyo1_01Data::SetDataInRenew(CDBNpSub *pDBNpSub, int sn_seq)
{
	ASSERT(pDBNpSub);
	if( pDBNpSub == NULL ){
		return -1;
	}
	ASSERT(pDBNpSub->zvol);
	if( pDBNpSub->zvol == NULL ){
		return -1;
	}

	EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki(pDBNpSub->zvol);
	if( KzHoushiki != ID_ICSSH_GENSOKU ){
		return 0;
	}

	// é©ï™é©êgÇ∆à·Ç§â€ê≈ï˚éÆÇÃã‡äzÇ…èëÇ´çûÇ›ÅiëäéËï˚ÅFïtï\ÇSÅ|ÇPÅj
	CString	filter;
	int		nowVersion = 2;
	filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_4_01_HYOID, nowVersion);

	if( m_DBSyzAccess.m_pSH_LST_VAL ){
		m_DBSyzAccess.m_pSH_LST_VAL->Requery(filter);
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
		pShLstVal->Edit();
		pShLstVal->val = _T("0");
		pShLstVal->sign1 = 0;
		pShLstVal->Update();
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
bool CH31SnFhyo1_01Data::IsExistData( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT(pDBNpSub);
	if( pDBNpSub == NULL ) {
		return false;
	}

	int nowVersion = 2;
	CString filter;
	if( sn_seq != -1 ){
		filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_1_01_HYOID, nowVersion);
	}
	else{
		filter.Format(_T("hyoId = \'%s\' and version = %d"), ICS_SN_FHYO_1_01_HYOID, nowVersion);
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

