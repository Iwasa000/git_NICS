#include "StdAfx.h"
#include "H31SnFhyo4_01Data.h"

static SH_TAGNAME tagData401[] = {
	// â€ê≈ïWèÄäz
	{ _T("CSB00010"), 0 },
	{ _T("CSB00020"), 1 },
	{ _T("CSB00030"), 3 },
	{ _T("CSB00040"), 999 },
	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
	{ _T("CSC00010"), 0 },
	{ _T("CSC00020"), 1 },
	{ _T("CSC00030"), 3 },
	{ _T("CSC00040"), 999 },
	// è¡îÔê≈äz
	{ _T("CSD00010"), 0 },
	{ _T("CSD00020"), 1 },
	{ _T("CSD00030"), 3 },
	{ _T("CSD00040"), 999 },
	// ë›ì|âÒé˚Ç…åWÇÈè¡îÔê≈äz
	{ _T("CSE00010"), 0 },
	{ _T("CSE00020"), 1 },
	{ _T("CSE00030"), 3 },
	{ _T("CSE00040"), 999 },
	// çTèúëŒè€édì¸ê≈äz
	{ _T("CSF00020"), 0 },
	{ _T("CSF00030"), 1 },
	{ _T("CSF00040"), 3 },
	{ _T("CSF00050"), 999 },
	// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
	{ _T("CSF00070"), 0 },
	{ _T("CSF00080"), 1 },
	{ _T("CSF00090"), 3 },
	{ _T("CSF00100"), 999 },
	// ë›ì|ÇÍÇ…åWÇÈê≈äz
	{ _T("CSF00120"), 0 },
	{ _T("CSF00130"), 1 },
	{ _T("CSF00140"), 3 },
	{ _T("CSF00150"), 999 },
	// çTèúê≈äzè¨åv
	{ _T("CSF00170"), 0 },
	{ _T("CSF00180"), 1 },
	{ _T("CSF00190"), 3 },
	{ _T("CSF00200"), 999 },
	// çTèúïsë´ä“ïtê≈äz
	{ _T("CSG00010"), 0 },
	{ _T("CSG00020"), 1 },
	{ _T("CSG00030"), 3 },
	{ _T("CSG00040"), 999 },
	// ç∑à¯ê≈äz
	{ _T("CSH00010"), 0 },
	{ _T("CSH00020"), 1 },
	{ _T("CSH00030"), 3 },
	{ _T("CSH00040"), 999 },
	// çáåvç∑à¯ê≈äz
	{ _T("CSI00000"), 999 },
	// çTèúïsë´ä“ïtê≈äz
	{ _T("CSJ00020"), 0 },
	{ _T("CSJ00030"), 3 },
	{ _T("CSJ00040"), 999 },
	// ç∑à¯ê≈äz
	{ _T("CSJ00060"), 0 },
	{ _T("CSJ00070"), 3 },
	{ _T("CSJ00080"), 999 },
	// çáåvç∑à¯ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
	{ _T("CSK00010"), 0 },
	{ _T("CSK00020"), 3 },
	{ _T("CSK00030"), 999 },
	// ä“ïtäz
	{ _T("CSL00020"), 0 },
	{ _T("CSL00030"), 3 },
	{ _T("CSL00040"), 999 },
	// î[ïtäz
	{ _T("CSL00060"), 0 },
	{ _T("CSL00070"), 3 },
	{ _T("CSL00080"), 999 },
	// çáåvç∑à¯è˜ìnäÑäz
	{ _T("CSM00000"), 999 },

	{ _T("") }

};

CH31SnFhyo4_01Data::CH31SnFhyo4_01Data( BOOL isTransitionalMeasure )
	: m_isTransitionalMeasure ( isTransitionalMeasure )
{
	memset ( Sn_F401_1X, '\0', sizeof ( Sn_F401_1X ) );
	memset ( Sn_F401_1D, '\0', sizeof ( Sn_F401_1D ) );
	memset ( Sn_F401_1E, '\0', sizeof ( Sn_F401_1E ) );
	memset ( Sn_F401_1F, '\0', sizeof ( Sn_F401_1F ) );

	memset ( Sn_F401_1_1X, '\0', sizeof ( Sn_F401_1_1X ) );
	memset ( Sn_F401_1_1D, '\0', sizeof ( Sn_F401_1_1D ) );
	memset ( Sn_F401_1_1E, '\0', sizeof ( Sn_F401_1_1E ) );
	memset ( Sn_F401_1_1F, '\0', sizeof ( Sn_F401_1_1F ) );

	memset ( Sn_F401_2X, '\0', sizeof ( Sn_F401_2X ) );
	memset ( Sn_F401_2D, '\0', sizeof ( Sn_F401_2D ) );
	memset ( Sn_F401_2E, '\0', sizeof ( Sn_F401_2E ) );
	memset ( Sn_F401_2F, '\0', sizeof ( Sn_F401_2F ) );

	memset ( Sn_F401_3X, '\0', sizeof ( Sn_F401_3X ) );
	memset ( Sn_F401_3D, '\0', sizeof ( Sn_F401_3D ) );
	memset ( Sn_F401_3E, '\0', sizeof ( Sn_F401_3E ) );
	memset ( Sn_F401_3F, '\0', sizeof ( Sn_F401_3F ) );

	memset ( Sn_F401_4X, '\0', sizeof ( Sn_F401_4X ) );
	memset ( Sn_F401_4D, '\0', sizeof ( Sn_F401_4D ) );
	memset ( Sn_F401_4E, '\0', sizeof ( Sn_F401_4E ) );
	memset ( Sn_F401_4F, '\0', sizeof ( Sn_F401_4F ) );

	memset ( Sn_F401_5X, '\0', sizeof ( Sn_F401_5X ) );
	memset ( Sn_F401_5D, '\0', sizeof ( Sn_F401_5D ) );
	memset ( Sn_F401_5E, '\0', sizeof ( Sn_F401_5E ) );
	memset ( Sn_F401_5F, '\0', sizeof ( Sn_F401_5F ) );

	memset ( Sn_F401_6X, '\0', sizeof ( Sn_F401_6X ) );
	memset ( Sn_F401_6D, '\0', sizeof ( Sn_F401_6D ) );
	memset ( Sn_F401_6E, '\0', sizeof ( Sn_F401_6E ) );
	memset ( Sn_F401_6F, '\0', sizeof ( Sn_F401_6F ) );

	memset ( Sn_F401_7X, '\0', sizeof ( Sn_F401_7X ) );
	memset ( Sn_F401_7D, '\0', sizeof ( Sn_F401_7D ) );
	memset ( Sn_F401_7E, '\0', sizeof ( Sn_F401_7E ) );
	memset ( Sn_F401_7F, '\0', sizeof ( Sn_F401_7F ) );

	memset ( Sn_F401_8X, '\0', sizeof ( Sn_F401_8X ) );
	memset ( Sn_F401_8D, '\0', sizeof ( Sn_F401_8D ) );
	memset ( Sn_F401_8E, '\0', sizeof ( Sn_F401_8E ) );
	memset ( Sn_F401_8F, '\0', sizeof ( Sn_F401_8F ) );

	memset ( Sn_F401_9X, '\0', sizeof ( Sn_F401_9X ) );
	memset ( Sn_F401_9D, '\0', sizeof ( Sn_F401_9D ) );
	memset ( Sn_F401_9E, '\0', sizeof ( Sn_F401_9E ) );
	memset ( Sn_F401_9F, '\0', sizeof ( Sn_F401_9F ) );

	memset ( Sn_F401_10F, '\0', sizeof ( Sn_F401_10F ) );

	memset ( Sn_F401_11X, '\0', sizeof ( Sn_F401_11X ) );
	memset ( Sn_F401_11E, '\0', sizeof ( Sn_F401_11E ) );
	memset ( Sn_F401_11F, '\0', sizeof ( Sn_F401_11F ) );

	memset ( Sn_F401_12X, '\0', sizeof ( Sn_F401_12X ) );
	memset ( Sn_F401_12E, '\0', sizeof ( Sn_F401_12E ) );
	memset ( Sn_F401_12F, '\0', sizeof ( Sn_F401_12F ) );

	memset ( Sn_F401_13X, '\0', sizeof ( Sn_F401_13X ) );
	memset ( Sn_F401_13E, '\0', sizeof ( Sn_F401_13E ) );
	memset ( Sn_F401_13F, '\0', sizeof ( Sn_F401_13F ) );

	memset ( Sn_F401_14X, '\0', sizeof ( Sn_F401_14X ) );
	memset ( Sn_F401_14E, '\0', sizeof ( Sn_F401_14E ) );
	memset ( Sn_F401_14F, '\0', sizeof ( Sn_F401_14F ) );

	memset ( Sn_F401_15X, '\0', sizeof ( Sn_F401_15X ) );
	memset ( Sn_F401_15E, '\0', sizeof ( Sn_F401_15E ) );
	memset ( Sn_F401_15F, '\0', sizeof ( Sn_F401_15F ) );

	memset ( Sn_F401_16F, '\0', sizeof ( Sn_F401_16F ) );

	Sn_F401_2sw = 0;

	m_Arith.l_defn ( 0x16 );

	rsv = NULL;
}


CH31SnFhyo4_01Data::~CH31SnFhyo4_01Data()
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
int CH31SnFhyo4_01Data::GetData( CDBNpSub *pDBNpSub, int sn_seq )
{
	
	ASSERT ( pDBNpSub );
	if ( pDBNpSub == NULL ) {
		return -1;
	}

	ASSERT ( pDBNpSub->zvol );
	if ( pDBNpSub->zvol == NULL ) {
		return -1;
	}

	EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki ( pDBNpSub->zvol );
	if ( KzHoushiki == ID_ICSSH_MENZEI ) {
		return 0;
	}

	CString filter;
	int		nowVersion = 2;
	filter.Format ( _T( "sn_seq = %d and hyoId = \'%s\' and version = %d" ), sn_seq, ICS_SN_FHYO_4_01_HYOID, nowVersion );

	if ( m_DBSyzAccess.m_pSH_LST_VAL ) {
		m_DBSyzAccess.m_pSH_LST_VAL->Requery ( filter );
	}
	else {
		if ( m_DBSyzAccess.OpenShLstVal ( pDBNpSub->m_database, filter ) ) {
			return -1;
		}
	}

	CSH_LST_VAL *pShLstVal = m_DBSyzAccess.m_pSH_LST_VAL;

	for ( int i = 0; ; i++ ) {
		if ( i == 0 ) {
			if ( ( pShLstVal->MoveFirst() == -1 ) || ( pShLstVal->st == -1 ) ) {
				break;
			}
		}
		else {
			if ( ( pShLstVal->MoveNext() == -1 ) || ( pShLstVal->st == -1 ) ) {
				break;
			}
		}
		
		// â€ê≈ïWèÄäz
		if ( pShLstVal->itmId == _T( "CSB00010" ) ) {
			m_Util.val_to_bin ( Sn_F401_1X, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSB00020" ) ) {
			m_Util.val_to_bin ( Sn_F401_1D, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSB00030" ) ) {
			m_Util.val_to_bin ( Sn_F401_1E, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSB00040" ) ) {
			m_Util.val_to_bin ( Sn_F401_1F, pShLstVal->val );
		}
		// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
		else if ( pShLstVal->itmId == _T( "CSC00010" ) ) {
			m_Util.val_to_bin ( Sn_F401_1_1X, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSC00020" ) ) {
			m_Util.val_to_bin ( Sn_F401_1_1D, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSC00030" ) ) {
			m_Util.val_to_bin ( Sn_F401_1_1E, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSC00040" ) ) {
			m_Util.val_to_bin ( Sn_F401_1_1F, pShLstVal->val );
		}
		// è¡îÔê≈äz
		else if ( pShLstVal->itmId == _T( "CSD00010" ) ) {
			m_Util.val_to_bin ( Sn_F401_2X, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSD00020" ) ) {
			m_Util.val_to_bin ( Sn_F401_2D, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSD00030" ) ) {
			m_Util.val_to_bin ( Sn_F401_2E, pShLstVal->val );
			//Sn_F401_2sw = pShLstVal->sign1;
		}
		else if ( pShLstVal->itmId == _T( "CSD00040" ) ) {
			m_Util.val_to_bin ( Sn_F401_2F, pShLstVal->val );
			Sn_F401_2sw = pShLstVal->sign1;
		}
		// ë›ì|âÒé˚Ç…åWÇÈè¡îÔê≈äz
		else if ( pShLstVal->itmId == _T( "CSE00010" ) ) {
			m_Util.val_to_bin ( Sn_F401_3X, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSE00020" ) ) {
			m_Util.val_to_bin ( Sn_F401_3D, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSE00030" ) ) {
			m_Util.val_to_bin ( Sn_F401_3E, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSE00040" ) ) {
			m_Util.val_to_bin ( Sn_F401_3F, pShLstVal->val );
		}
		// çTèúëŒè€édì¸ê≈äz
		else if ( pShLstVal->itmId == _T( "CSF00020" ) ) {
			m_Util.val_to_bin ( Sn_F401_4X, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSF00030" ) ) {
			m_Util.val_to_bin ( Sn_F401_4D, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSF00040" ) ) {
			m_Util.val_to_bin ( Sn_F401_4E, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSF00050" ) ) {
			m_Util.val_to_bin ( Sn_F401_4F, pShLstVal->val );
		}
		// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
		else if ( pShLstVal->itmId == _T( "CSF00070" ) ) {
			m_Util.val_to_bin ( Sn_F401_5X, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSF00080" ) ) {
			m_Util.val_to_bin ( Sn_F401_5D, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSF00090" ) ) {
			m_Util.val_to_bin ( Sn_F401_5E, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSF00100" ) ) {
			m_Util.val_to_bin ( Sn_F401_5F, pShLstVal->val );
		}
		// ë›ì|ÇÍÇ…åWÇÈê≈äz
		else if ( pShLstVal->itmId == _T( "CSF00120" ) ) {
			m_Util.val_to_bin ( Sn_F401_6X, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSF00130" ) ) {
			m_Util.val_to_bin ( Sn_F401_6D, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSF00140" ) ) {
			m_Util.val_to_bin ( Sn_F401_6E, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSF00150" ) ) {
			m_Util.val_to_bin ( Sn_F401_6F, pShLstVal->val );
		}
		// çTèúê≈äzè¨åv
		else if ( pShLstVal->itmId == _T( "CSF00170" ) ) {
			m_Util.val_to_bin ( Sn_F401_7X, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSF00180" ) ) {
			m_Util.val_to_bin ( Sn_F401_7D, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSF00190" ) ) {
			m_Util.val_to_bin ( Sn_F401_7E, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSF00200" ) ) {
			m_Util.val_to_bin ( Sn_F401_7F, pShLstVal->val );
		}
		// çTèúïsë´ä“ïtê≈äz
		else if ( pShLstVal->itmId == _T( "CSG00010" ) ) {
			m_Util.val_to_bin ( Sn_F401_8X, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSG00020" ) ) {
			m_Util.val_to_bin ( Sn_F401_8D, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSG00030" ) ) {
			m_Util.val_to_bin ( Sn_F401_8E, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSG00040" ) ) {
			m_Util.val_to_bin ( Sn_F401_8F, pShLstVal->val );
		}
		// ç∑à¯ê≈äz
		else if ( pShLstVal->itmId == _T( "CSH00010" ) ) {
			m_Util.val_to_bin ( Sn_F401_9X, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSH00020" ) ) {
			m_Util.val_to_bin ( Sn_F401_9D, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSH00030" ) ) {
			m_Util.val_to_bin ( Sn_F401_9E, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSH00040" ) ) {
			m_Util.val_to_bin ( Sn_F401_9F, pShLstVal->val );
		}
		// çáåvç∑à¯ê≈äz
		else if ( pShLstVal->itmId == _T( "CSI00000" ) ) {
			m_Util.val_to_bin ( Sn_F401_10F, pShLstVal->val );
		}
		// çTèúïsë´ä“ïtê≈äz
		else if ( pShLstVal->itmId == _T( "CSJ00020" ) ) {
			m_Util.val_to_bin ( Sn_F401_11X, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSJ00030" ) ) {
			m_Util.val_to_bin ( Sn_F401_11E, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSJ00040" ) ) {
			m_Util.val_to_bin ( Sn_F401_11F, pShLstVal->val );
		}
		// ç∑à¯ê≈äz
		else if ( pShLstVal->itmId == _T( "CSJ00060" ) ) {
			m_Util.val_to_bin ( Sn_F401_12X, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSJ00070" ) ) {
			m_Util.val_to_bin ( Sn_F401_12E, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSJ00080" ) ) {
			m_Util.val_to_bin ( Sn_F401_12F, pShLstVal->val );
		}
		// çáåvç∑à¯ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
		else if ( pShLstVal->itmId == _T( "CSK00010" ) ) {
			m_Util.val_to_bin ( Sn_F401_13X, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSK00020" ) ) {
			m_Util.val_to_bin ( Sn_F401_13E, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSK00030" ) ) {
			m_Util.val_to_bin ( Sn_F401_13F, pShLstVal->val );
		}
		// ä“ïtäz
		else if ( pShLstVal->itmId == _T( "CSL00020" ) ) {
			m_Util.val_to_bin ( Sn_F401_14X, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSL00030" ) ) {
			m_Util.val_to_bin ( Sn_F401_14E, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSL00040" ) ) {
			m_Util.val_to_bin ( Sn_F401_14F, pShLstVal->val );
		}
		// î[ïtäz
		else if ( pShLstVal->itmId == _T( "CSL00060" ) ) {
			m_Util.val_to_bin ( Sn_F401_15X, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSL00070" ) ) {
			m_Util.val_to_bin ( Sn_F401_15E, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "CSL00080" ) ) {
			m_Util.val_to_bin ( Sn_F401_15F, pShLstVal->val );
		}
		// çáåvç∑à¯è˜ìnäÑäz
		else if ( pShLstVal->itmId == _T( "CSM00000" ) ) {
			m_Util.val_to_bin ( Sn_F401_16F, pShLstVal->val );
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
int CH31SnFhyo4_01Data::SetData( CDBNpSub *pDBNpSub, int sn_seq )
{
	
	ASSERT ( pDBNpSub );
	if ( pDBNpSub == NULL ) {
		return -1;
	}
	ASSERT ( pDBNpSub->zvol );
	if ( pDBNpSub->zvol == NULL ) {
		return -1;
	}

	EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki ( pDBNpSub->zvol );
	if ( KzHoushiki == ID_ICSSH_MENZEI ) {
		return 0;
	}

	CString filter;
	char	*pHyoID = NULL;
	int		nowVersion = 2;
	filter.Format ( _T( "sn_seq = %d and hyoId = \'%s\' and version = %d" ), sn_seq, ICS_SN_FHYO_4_01_HYOID, nowVersion );
	pHyoID = ICS_SN_FHYO_4_01_HYOID;

	if ( m_DBSyzAccess.m_pSH_LST_VAL ) {
		m_DBSyzAccess.m_pSH_LST_VAL->Requery ( filter );
	}
	else {
		if ( m_DBSyzAccess.OpenShLstVal ( pDBNpSub->m_database, filter ) ) {
			return -1;
		}
	}

	CSH_LST_VAL *pShLstVal = m_DBSyzAccess.m_pSH_LST_VAL;
	if ( ( pShLstVal->MoveFirst() == -1 ) || ( pShLstVal->st == -1 ) ) {	// ÉfÅ[É^Ç»Çµ

		// â€ê≈ïWèÄäz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSB00010" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_1X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSB00020" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_1D );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSB00030" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_1E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSB00040" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_1F );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSC00010" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_1_1X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSC00020" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_1_1D );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSC00030" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_1_1E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSC00040" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_1_1F );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();
		
		// è¡îÔê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSD00010" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_2X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSD00020" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_2D );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSD00030" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_2E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSD00040" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_2F );
		pShLstVal->sign1 = Sn_F401_2sw;
		pShLstVal->Update();

		// ë›ì|âÒé˚Ç…åWÇÈè¡îÔê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSE00010" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_3X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSE00020" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_3D );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSE00030" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_3E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSE00040" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_3F );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// çTèúëŒè€édì¸ê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSF00020" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_4X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSF00030" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_4D );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSF00040" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_4E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSF00050" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_4F );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSF00070" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_5X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSF00080" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_5D );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSF00090" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_5E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSF00100" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_5F );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ë›ì|ÇÍÇ…åWÇÈê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSF00120" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_6X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSF00130" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_6D );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSF00140" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_6E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSF00150" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_6F );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// çTèúê≈äzè¨åv
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSF00170" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_7X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSF00180" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_7D );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSF00190" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_7E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSF00200" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_7F );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// çTèúïsë´ä“ïtê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSG00010" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_8X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSG00020" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_8D );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSG00030" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_8E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSG00040" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_8F );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ç∑à¯ê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSH00010" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_9X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSH00020" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_9D );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSH00030" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_9E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSH00040" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_9F );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// çáåvç∑à¯ê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSI00000" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_10F );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// çTèúïsë´ä“ïtê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSJ00020" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_11X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSJ00030" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_11E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSJ00040" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_11F );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ç∑à¯ê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSJ00060" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_12X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSJ00070" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_12E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSJ00080" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_12F );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// çáåvç∑à¯ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSK00010" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_13X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSK00020" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_13E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSK00030" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_13F );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ä“ïtäz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSL00020" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_14X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSL00030" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_14E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSL00040" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_14F );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// î[ïtäz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSL00060" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_15X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSL00070" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_15E );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSL00080" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_15F );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// çáåvç∑à¯è˜ìnäÑäz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "CSM00000" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_16F );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();
	}
	else {
		CStringArray updTagAry;

		for ( int i = 0; ; i++ ) {
			if ( i == 0 ) {
				if ( ( pShLstVal->MoveFirst() == -1 ) || ( pShLstVal->st == -1 ) ) {
					break;
				}
			}
			else {
				if ( ( pShLstVal->MoveNext() == -1 ) || ( pShLstVal->st == -1 ) ) {
					break;
				}
			}

			// â€ê≈ïWèÄäz
			if ( pShLstVal->itmId == _T( "CSB00010" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_1X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSB00020" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_1D );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSB00030" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_1E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSB00040" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_1F );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
			else if ( pShLstVal->itmId == _T( "CSC00010" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_1_1X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSC00020" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_1_1D );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSC00030" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_1_1E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSC00040" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_1_1F );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// è¡îÔê≈äz
			else if ( pShLstVal->itmId == _T( "CSD00010" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_2X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSD00020" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_2D );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSD00030" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_2E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSD00040" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_2F );
				pShLstVal->sign1 = Sn_F401_2sw;
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// ë›ì|âÒé˚Ç…åWÇÈè¡îÔê≈äz
			else if ( pShLstVal->itmId == _T( "CSE00010" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_3X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSE00020" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_3D );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSE00030" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_3E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSE00040" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_3F );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// çTèúëŒè€édì¸ê≈äz
			else if ( pShLstVal->itmId == _T( "CSF00020" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_4X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSF00030" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_4D );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSF00040" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_4E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSF00050" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_4F );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
			else if ( pShLstVal->itmId == _T( "CSF00070" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_5X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSF00080" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_5D );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSF00090" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_5E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSF00100" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_5F );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// ë›ì|ÇÍÇ…åWÇÈê≈äz
			else if ( pShLstVal->itmId == _T( "CSF00120" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_6X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSF00130" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_6D );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSF00140" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_6E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSF00150" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_6F );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// çTèúê≈äzè¨åv
			else if ( pShLstVal->itmId == _T( "CSF00170" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_7X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSF00180" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_7D );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSF00190" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_7E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSF00200" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_7F );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// çTèúïsë´ä“ïtê≈äz
			else if ( pShLstVal->itmId == _T( "CSG00010" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_8X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSG00020" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_8D );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSG00030" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_8E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSG00040" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_8F );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// ç∑à¯ê≈äz
			else if ( pShLstVal->itmId == _T( "CSH00010" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_9X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSH00020" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_9D );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSH00030" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_9E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSH00040" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_9F );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// çáåvç∑à¯ê≈äz
			else if ( pShLstVal->itmId == _T( "CSI00000" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_10F );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// çTèúïsë´ä“ïtê≈äz
			else if ( pShLstVal->itmId == _T( "CSJ00020" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_11X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSJ00030" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_11E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSJ00040" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_11F );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// ç∑à¯ê≈äz
			else if ( pShLstVal->itmId == _T( "CSJ00060" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_12X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSJ00070" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_12E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSJ00080" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_12F );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// çáåvç∑à¯ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
			else if ( pShLstVal->itmId == _T( "CSK00010" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_13X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSK00020" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_13E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSK00030" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_13F );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// ä“ïtäz
			else if ( pShLstVal->itmId == _T( "CSL00020" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_14X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSL00030" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_14E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSL00040" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_14F );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// î[ïtäz
			else if ( pShLstVal->itmId == _T( "CSL00060" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_15X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSL00070" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_15E );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "CSL00080" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_15F );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// çáåvç∑à¯è˜ìnäÑäz
			else if ( pShLstVal->itmId == _T( "CSM00000" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F401_16F );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
		}

		int adCnt = 0;
		for ( int i = 0; ; i++ ) {
			if ( strlen ( tagData401[i].tagName ) == 0 ) {
				break;
			}
			adCnt++;
		}

		int updCnt = ( int )updTagAry.GetCount();
		if ( adCnt != updCnt ) {
			CArray<unsigned char*> MonyPrtAry;

			MonyPrtAry.Add ( Sn_F401_1X );
			MonyPrtAry.Add ( Sn_F401_1D );
			MonyPrtAry.Add ( Sn_F401_1E );
			MonyPrtAry.Add ( Sn_F401_1F );
			MonyPrtAry.Add ( Sn_F401_1_1X );
			MonyPrtAry.Add ( Sn_F401_1_1D );
			MonyPrtAry.Add ( Sn_F401_1_1E );
			MonyPrtAry.Add ( Sn_F401_1_1F );
			MonyPrtAry.Add ( Sn_F401_2X );
			MonyPrtAry.Add ( Sn_F401_2D );
			MonyPrtAry.Add ( Sn_F401_2E );
			MonyPrtAry.Add ( Sn_F401_2F );
			MonyPrtAry.Add ( Sn_F401_3X );
			MonyPrtAry.Add ( Sn_F401_3D );
			MonyPrtAry.Add ( Sn_F401_3E );
			MonyPrtAry.Add ( Sn_F401_3F );
			MonyPrtAry.Add ( Sn_F401_4X );
			MonyPrtAry.Add ( Sn_F401_4D );
			MonyPrtAry.Add ( Sn_F401_4E );
			MonyPrtAry.Add ( Sn_F401_4F );
			MonyPrtAry.Add ( Sn_F401_5X );
			MonyPrtAry.Add ( Sn_F401_5D );
			MonyPrtAry.Add ( Sn_F401_5E );
			MonyPrtAry.Add ( Sn_F401_5F );
			MonyPrtAry.Add ( Sn_F401_6X );
			MonyPrtAry.Add ( Sn_F401_6D );
			MonyPrtAry.Add ( Sn_F401_6E );
			MonyPrtAry.Add ( Sn_F401_6F );
			MonyPrtAry.Add ( Sn_F401_7X );
			MonyPrtAry.Add ( Sn_F401_7D );
			MonyPrtAry.Add ( Sn_F401_7E );
			MonyPrtAry.Add ( Sn_F401_7F );
			MonyPrtAry.Add ( Sn_F401_8X );
			MonyPrtAry.Add ( Sn_F401_8D );
			MonyPrtAry.Add ( Sn_F401_8E );
			MonyPrtAry.Add ( Sn_F401_8F );
			MonyPrtAry.Add ( Sn_F401_9X );
			MonyPrtAry.Add ( Sn_F401_9D );
			MonyPrtAry.Add ( Sn_F401_9E );
			MonyPrtAry.Add ( Sn_F401_9F );
			MonyPrtAry.Add ( Sn_F401_10F );
			MonyPrtAry.Add ( Sn_F401_11X );
			MonyPrtAry.Add ( Sn_F401_11E );
			MonyPrtAry.Add ( Sn_F401_11F );
			MonyPrtAry.Add ( Sn_F401_12X );
			MonyPrtAry.Add ( Sn_F401_12E );
			MonyPrtAry.Add ( Sn_F401_12F );
			MonyPrtAry.Add ( Sn_F401_13X );
			MonyPrtAry.Add ( Sn_F401_13E );
			MonyPrtAry.Add ( Sn_F401_13F );
			MonyPrtAry.Add ( Sn_F401_14X );
			MonyPrtAry.Add ( Sn_F401_14E );
			MonyPrtAry.Add ( Sn_F401_14F );
			MonyPrtAry.Add ( Sn_F401_15X );
			MonyPrtAry.Add ( Sn_F401_15E );
			MonyPrtAry.Add ( Sn_F401_15F );
			MonyPrtAry.Add ( Sn_F401_16F );

			BOOL findFlg = FALSE;
			for ( int i = 0; ; i++ ) {
				if ( strlen ( tagData401[i].tagName ) == 0 ) {
					break;
				}

				findFlg = FALSE;
				for ( int j = 0; j < updCnt; j++ ) {
					if ( updTagAry[j].CompareNoCase ( tagData401[i].tagName ) == 0 ) {
						findFlg = TRUE;
						break;
					}

					if ( findFlg == FALSE ) {
						// äYìñÉ^ÉOÇÃí«â¡
						pShLstVal->AddNew();
						pShLstVal->sn_seq = sn_seq;
						pShLstVal->version = nowVersion;
						pShLstVal->hyoId = pHyoID;
						pShLstVal->itmId = tagData401[i].tagName;
						m_Util.val_to_asci ( &pShLstVal->val, MonyPrtAry[i] );
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
// èWåvçœÇ›ÉfÅ[É^ÇÃÉZÉbÉg(ïtï\4Å|1)
//-------------------------------------------------------
// à¯êî		pDBNpSub	ÅF	ç‡ñ±ÉNÉâÉX
//			pSyzSyukeiÅ@ÅFÅ@è¡îÔê≈èWåvÉNÉâÉX
//			isPluralZei	ÅFÅ@ï°êîê≈ó¶
//-------------------------------------------------------
// ï‘ëóíl	 0			ÅFÅ@ê≥èÌèIóπ
//			-1			ÅFÅ@ÉGÉâÅ[
//=======================================================
int CH31SnFhyo4_01Data::SetCalqedData(CSyzSyukei *pSyzSyukei, BOOL &isPluralZei)
{
	isPluralZei = FALSE;

	MoneyBasejagArray	money;

	money = pSyzSyukei->GetShinkokuData(_T("34"));

	int cnt = (int)money.GetCount();
	int incnt = 0;

	unsigned char exZeiflg = 0x00;	// D0ÅFãåê≈ó¶
									// D1ÅF6.24%
									// D2ÅF7.8%
									// D3ÅFçáåv

	for (int i = 0; i < cnt; i++ ) {
		incnt = (int)money[i].GetCount();
		for (int j = 0; j < incnt; j++ ) {

			// â€ê≈ïWèÄäz
			if (money[i][j].code.Left(4) == _T("3418")) {
				if (money[i][j].code == _T("341801")) {
					memmove(Sn_F401_1X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("341802")) {
					memmove(Sn_F401_1D, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("341803")) {
					memmove(Sn_F401_1E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("341800")) {
					memmove(Sn_F401_1F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
			else if (money[i][j].code.Left(4) == _T("3419")) {
				if (money[i][j].code == _T("341901")) {
					memmove(Sn_F401_1_1X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("341902")) {
					memmove(Sn_F401_1_1D, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("341903")) {
					memmove(Sn_F401_1_1E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("341900")) {
					memmove(Sn_F401_1_1F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// è¡îÔê≈äz
			else if (money[i][j].code.Left(4) == _T("3420")) {
				if (money[i][j].code == _T("342001")) {
					memmove(Sn_F401_2X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("342002")) {
					memmove(Sn_F401_2D, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
//						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("342003")) {
					memmove(Sn_F401_2E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
//						exZeiflg |= 0x08;
					}
				}
				else if (money[i][j].code == _T("342000")) {
					memmove(Sn_F401_2F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// ë›ì|ÇÍâÒé˚Ç…åWÇÈè¡îÔê≈äz
			else if (money[i][j].code.Left(4) == _T("3421")) {
				if (money[i][j].code == _T("342101")) {
					memmove(Sn_F401_3X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("342102")) {
					memmove(Sn_F401_3D, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("342103")) {
					memmove(Sn_F401_3E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("342100")) {
					memmove(Sn_F401_3F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// çTèúëŒè€édì¸ê≈äz
			else if (money[i][j].code.Left(4) == _T("3422")) {
				if (money[i][j].code == _T("342201")) {
					memmove(Sn_F401_4X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("342202")) {
					memmove(Sn_F401_4D, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("342203")) {
					memmove(Sn_F401_4E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("342200")) {
					memmove(Sn_F401_4F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
			else if (money[i][j].code.Left(4) == _T("3423")) {
				if (money[i][j].code == _T("342301")) {
					memmove(Sn_F401_5X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("342302")) {
					memmove(Sn_F401_5D, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("342303")) {
					memmove(Sn_F401_5E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("342300")) {
					memmove(Sn_F401_5F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// ë›ì|ÇÍÇ…åWÇÈê≈äz
			else if (money[i][j].code.Left(4) == _T("3424")) {
				if (money[i][j].code == _T("342401")) {
					memmove(Sn_F401_6X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("342402")) {
					memmove(Sn_F401_6D, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("342403")) {
					memmove(Sn_F401_6E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("342400")) {
					memmove(Sn_F401_6F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// çTèúê≈äzè¨åv
			else if (money[i][j].code.Left(4) == _T("3425")) {
				if (money[i][j].code == _T("342501")) {
					memmove(Sn_F401_7X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("342502")) {
					memmove(Sn_F401_7D, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("342503")) {
					memmove(Sn_F401_7E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("342500")) {
					memmove(Sn_F401_7F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// çTèúïsë´ä“ïtê≈äz
			else if (money[i][j].code.Left(4) == _T("3426")) {
				if (money[i][j].code == _T("342601")) {
					memmove(Sn_F401_8X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("342602")) {
					memmove(Sn_F401_8D, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("342603")) {
					memmove(Sn_F401_8E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("342600")) {
					memmove(Sn_F401_8F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// ç∑à¯ê≈äz
			else if (money[i][j].code.Left(4) == _T("3427")) {
				if (money[i][j].code == _T("342701")) {
					memmove(Sn_F401_9X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("342702")) {
					memmove(Sn_F401_9D, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x02;
					}
				}
				else if (money[i][j].code == _T("342703")) {
					memmove(Sn_F401_9E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("342700")) {
					memmove(Sn_F401_9F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// çáåvç∑à¯ê≈äz
			else if (money[i][j].code.Left(4) == _T("3428")) {
				if (money[i][j].code == _T("342800")) {
					memmove(Sn_F401_10F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// çTèúïsë´ä“ïtê≈äz
			else if (money[i][j].code.Left(4) == _T("3429")) {
				if (money[i][j].code == _T("342901")) {
					memmove(Sn_F401_11X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("342903")) {
					memmove(Sn_F401_11E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("342900")) {
					memmove(Sn_F401_11F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// ç∑à¯ê≈äz
			else if (money[i][j].code.Left(4) == _T("3430")) {
				if (money[i][j].code == _T("343001")) {
					memmove(Sn_F401_12X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("343003")) {
					memmove(Sn_F401_12E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("343000")) {
					memmove(Sn_F401_12F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// çáåvç∑à¯ínï˚è¡îÔê≈äzÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
			else if (money[i][j].code.Left(4) == _T("3431")) {
				if (money[i][j].code == _T("343101")) {
					memmove(Sn_F401_13X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}
				else if (money[i][j].code == _T("343103")) {
					memmove(Sn_F401_13E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("343100")) {
					memmove(Sn_F401_13F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// ä“ïtäz
			else if (money[i][j].code.Left(4) == _T("3432")) {
				if (money[i][j].code == _T("343201")) {
					memmove(Sn_F401_14X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}

				else if (money[i][j].code == _T("343203")) {
					memmove(Sn_F401_14E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("343200")) {
					memmove(Sn_F401_14F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// î[ïtäz
			else if (money[i][j].code.Left(4) == _T("3433")) {
				if (money[i][j].code == _T("343301")) {
					memmove(Sn_F401_15X, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x01;
					}
				}

				else if (money[i][j].code == _T("343303")) {
					memmove(Sn_F401_15E, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x04;
					}
				}
				else if (money[i][j].code == _T("343300")) {
					memmove(Sn_F401_15F, money[i][j].arith, MONY_BUF_SIZE);
					if (m_Arith.l_test(money[i][j].arith)) {
						exZeiflg |= 0x08;
					}
				}
			}

			// çáåvç∑à¯ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
			else if (money[i][j].code.Left(4) == _T("3434")) {
				if (money[i][j].code == _T("343400")) {
					memmove(Sn_F401_16F, money[i][j].arith, MONY_BUF_SIZE);
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
int CH31SnFhyo4_01Data::ClearDataByRenew(CDBNpSub *pDBNpSub, int type)
{

	m_Arith.l_clear(Sn_F401_1X);
	m_Arith.l_clear(Sn_F401_1D);
	m_Arith.l_clear(Sn_F401_1E);
	m_Arith.l_clear(Sn_F401_1F);

	m_Arith.l_clear(Sn_F401_1_1X);
	m_Arith.l_clear(Sn_F401_1_1D);
	m_Arith.l_clear(Sn_F401_1_1E);
	m_Arith.l_clear(Sn_F401_1_1F);

	m_Arith.l_clear(Sn_F401_2X);
	m_Arith.l_clear(Sn_F401_2D);
	m_Arith.l_clear(Sn_F401_2E);
	m_Arith.l_clear(Sn_F401_2F);

	m_Arith.l_clear(Sn_F401_3X);
	m_Arith.l_clear(Sn_F401_3D);
	m_Arith.l_clear(Sn_F401_3E);
	m_Arith.l_clear(Sn_F401_3F);

	m_Arith.l_clear(Sn_F401_4X);
	m_Arith.l_clear(Sn_F401_4D);
	m_Arith.l_clear(Sn_F401_4E);
	m_Arith.l_clear(Sn_F401_4F);

	m_Arith.l_clear(Sn_F401_5X);
	m_Arith.l_clear(Sn_F401_5D);
	m_Arith.l_clear(Sn_F401_5E);
	m_Arith.l_clear(Sn_F401_5F);

	m_Arith.l_clear(Sn_F401_6X);
	m_Arith.l_clear(Sn_F401_6D);
	m_Arith.l_clear(Sn_F401_6E);
	m_Arith.l_clear(Sn_F401_6F);

	m_Arith.l_clear(Sn_F401_7X);
	m_Arith.l_clear(Sn_F401_7D);
	m_Arith.l_clear(Sn_F401_7E);
	m_Arith.l_clear(Sn_F401_7F);

	m_Arith.l_clear(Sn_F401_8X);
	m_Arith.l_clear(Sn_F401_8D);
	m_Arith.l_clear(Sn_F401_8E);
	m_Arith.l_clear(Sn_F401_8F);

	m_Arith.l_clear(Sn_F401_9X);
	m_Arith.l_clear(Sn_F401_9D);
	m_Arith.l_clear(Sn_F401_9E);
	m_Arith.l_clear(Sn_F401_9F);

	m_Arith.l_clear(Sn_F401_10F);

	m_Arith.l_clear(Sn_F401_11X);
	m_Arith.l_clear(Sn_F401_11E);
	m_Arith.l_clear(Sn_F401_11F);

	m_Arith.l_clear(Sn_F401_12X);
	m_Arith.l_clear(Sn_F401_12E);
	m_Arith.l_clear(Sn_F401_12F);

	m_Arith.l_clear(Sn_F401_13X);
	m_Arith.l_clear(Sn_F401_13E);
	m_Arith.l_clear(Sn_F401_13F);

	m_Arith.l_clear(Sn_F401_14X);
	m_Arith.l_clear(Sn_F401_14E);
	m_Arith.l_clear(Sn_F401_14F);

	m_Arith.l_clear(Sn_F401_15X);
	m_Arith.l_clear(Sn_F401_15E);
	m_Arith.l_clear(Sn_F401_15F);

	m_Arith.l_clear(Sn_F401_16F);
	

	Sn_F401_2sw = 0;

	rsv = NULL;

	memset(rsv1, '\0', sizeof(rsv1));
	memset(rsv2, '\0', sizeof(rsv2));



	return 0;
}


//-----------------------------------------------------------------------------
// éËì¸óÕÉTÉCÉìÇÃÉNÉäÉA
//-----------------------------------------------------------------------------
// ï‘ëóíl	0	ÅF	ê≥èÌèIóπ
//-----------------------------------------------------------------------------
int CH31SnFhyo4_01Data::ClearManualInputSign(BOOL isChgKzritu)
{

//	Sn_F401_2sw = 0;//yoshida190909

	//if (isChgKzritu) {
	//	Sn_F401_2sw |= 0x111;
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
int CH31SnFhyo4_01Data::SetDataInRenew(CDBNpSub *pDBNpSub, int sn_seq)
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
	if( KzHoushiki != ID_ICSSH_KANNI ){
		return 0;
	}

	// é©ï™é©êgÇ∆à·Ç§â€ê≈ï˚éÆÇÃã‡äzÇ…èëÇ´çûÇ›ÅiëäéËï˚ÅFïtï\ÇPÅ|ÇPÅj
	CString	filter;
	int		nowVersion = 2;
	filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_1_01_HYOID, nowVersion);

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
bool CH31SnFhyo4_01Data::IsExistData(CDBNpSub *pDBNpSub, int sn_seq)
{
	ASSERT(pDBNpSub);
	if( pDBNpSub == NULL ) {
		return false;
	}

	int nowVersion = 2;
	CString filter;
	if( sn_seq != -1 ){
		filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_4_01_HYOID, nowVersion);
	}
	else{
		filter.Format(_T("hyoId = \'%s\' and version = %d"), ICS_SN_FHYO_4_01_HYOID, nowVersion);
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
