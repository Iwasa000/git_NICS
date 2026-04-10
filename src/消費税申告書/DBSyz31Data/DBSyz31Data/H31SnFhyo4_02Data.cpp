#include "StdAfx.h"
#include "H31SnFhyo4_02Data.h"

static SH_TAGNAME tagData402[] = {
	// â€ê≈ïWèÄäz
	{ _T("BBB00010"), 0 },
	{ _T("BBB00020"), 1 },
	{ _T("BBB00030"), 3 },
	{ _T("BBB00040"), 999 },
	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
	{ _T("BBL00010"), 0 },
	{ _T("BBL00020"), 1 },
	{ _T("BBL00030"), 3 },
	{ _T("BBL00040"), 999 },
	// è¡îÔê≈äz
	{ _T("BBC00010"), 0 },
	{ _T("BBC00020"), 1 },
	{ _T("BBC00030"), 3 },
	{ _T("BBC00040"), 999 },
	// ë›ì|âÒé˚Ç…åWÇÈè¡îÔê≈äz
	{ _T("BBD00010"), 0 },
	{ _T("BBD00020"), 1 },
	{ _T("BBD00030"), 3 },
	{ _T("BBD00040"), 999 },
	// çTèúëŒè€édì¸ê≈äz
	{ _T("BBE00020"), 0 },
	{ _T("BBE00030"), 1 },
	{ _T("BBE00040"), 3 },
	{ _T("BBE00050"), 999 },
	// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
	{ _T("BBE00070"), 0 },
	{ _T("BBE00080"), 1 },
	{ _T("BBE00090"), 3 },
	{ _T("BBE00100"), 999 },
	// ë›ì|ÇÍÇ…åWÇÈê≈äz
	{ _T("BBE00120"), 0 },
	{ _T("BBE00130"), 1 },
	{ _T("BBE00140"), 3 },
	{ _T("BBE00150"), 999 },
	// çTèúê≈äzè¨åv
	{ _T("BBE00170"), 0 },
	{ _T("BBE00180"), 1 },
	{ _T("BBE00190"), 3 },
	{ _T("BBE00200"), 999 },
	// çTèúïsë´ä“ïtê≈äz
	{ _T("BBF00010"), 0 },
	{ _T("BBF00020"), 1 },
	{ _T("BBF00030"), 3 },
	{ _T("BBF00040"), 999 },
	// ç∑à¯ê≈äz
	{ _T("BBG00010"), 0 },
	{ _T("BBG00020"), 1 },
	{ _T("BBG00030"), 3 },
	{ _T("BBG00040"), 999 },
	// çTèúïsë´ä“ïtê≈äz
	{ _T("BBI00020"), 1 },
	{ _T("BBI00030"), 3 },
	{ _T("BBI00040"), 999 },
	// ç∑à¯ê≈äz
	{ _T("BBI00060"), 1 },
	{ _T("BBI00070"), 3 },
	{ _T("BBI00080"), 999 },
	// çáåvç∑à¯ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
	{ _T("BBM00010"), 1 },
	{ _T("BBM00020"), 3 },
	{ _T("BBM00030"), 999 },
	// ä“ïtäz
	{ _T("BBJ00020"), 1 },
	{ _T("BBJ00030"), 3 },
	{ _T("BBJ00040"), 999 },
	// î[ïtäz
	{ _T("BBJ00060"), 1 },
	{ _T("BBJ00070"), 3 },
	{ _T("BBJ00080"), 999 },

	{ _T("") }

};

CH31SnFhyo4_02Data::CH31SnFhyo4_02Data( BOOL isTransitionalMeasure )
	: m_isTransitionalMeasure ( isTransitionalMeasure )
{
	memset ( Sn_F402_1A, '\0', sizeof ( Sn_F402_1A ) );
	memset ( Sn_F402_1B, '\0', sizeof ( Sn_F402_1B ) );
	memset ( Sn_F402_1C, '\0', sizeof ( Sn_F402_1C ) );
	memset ( Sn_F402_1X, '\0', sizeof ( Sn_F402_1X ) );

	memset ( Sn_F402_1_1A, '\0', sizeof ( Sn_F402_1_1A ) );
	memset ( Sn_F402_1_1B, '\0', sizeof ( Sn_F402_1_1B ) );
	memset ( Sn_F402_1_1C, '\0', sizeof ( Sn_F402_1_1C ) );
	memset ( Sn_F402_1_1X, '\0', sizeof ( Sn_F402_1_1X ) );

	memset ( Sn_F402_2A, '\0', sizeof ( Sn_F402_2A ) );
	memset ( Sn_F402_2B, '\0', sizeof ( Sn_F402_2B ) );
	memset ( Sn_F402_2C, '\0', sizeof ( Sn_F402_2C ) );
	memset ( Sn_F402_2X, '\0', sizeof ( Sn_F402_2X ) );

	memset ( Sn_F402_3A, '\0', sizeof ( Sn_F402_3A ) );
	memset ( Sn_F402_3B, '\0', sizeof ( Sn_F402_3B ) );
	memset ( Sn_F402_3C, '\0', sizeof ( Sn_F402_3C ) );
	memset ( Sn_F402_3X, '\0', sizeof ( Sn_F402_3X ) );

	memset ( Sn_F402_4A, '\0', sizeof ( Sn_F402_4A ) );
	memset ( Sn_F402_4B, '\0', sizeof ( Sn_F402_4B ) );
	memset ( Sn_F402_4C, '\0', sizeof ( Sn_F402_4C ) );
	memset ( Sn_F402_4X, '\0', sizeof ( Sn_F402_4X ) );

	memset ( Sn_F402_5A, '\0', sizeof ( Sn_F402_5A ) );
	memset ( Sn_F402_5B, '\0', sizeof ( Sn_F402_5B ) );
	memset ( Sn_F402_5C, '\0', sizeof ( Sn_F402_5C ) );
	memset ( Sn_F402_5X, '\0', sizeof ( Sn_F402_5X ) );

	memset ( Sn_F402_6A, '\0', sizeof ( Sn_F402_6A ) );
	memset ( Sn_F402_6B, '\0', sizeof ( Sn_F402_6B ) );
	memset ( Sn_F402_6C, '\0', sizeof ( Sn_F402_6C ) );
	memset ( Sn_F402_6X, '\0', sizeof ( Sn_F402_6X ) );

	memset ( Sn_F402_7A, '\0', sizeof ( Sn_F402_7A ) );
	memset ( Sn_F402_7B, '\0', sizeof ( Sn_F402_7B ) );
	memset ( Sn_F402_7C, '\0', sizeof ( Sn_F402_7C ) );
	memset ( Sn_F402_7X, '\0', sizeof ( Sn_F402_7X ) );

	memset ( Sn_F402_8A, '\0', sizeof ( Sn_F402_8A ) );
	memset ( Sn_F402_8B, '\0', sizeof ( Sn_F402_8B ) );
	memset ( Sn_F402_8C, '\0', sizeof ( Sn_F402_8C ) );
	memset ( Sn_F402_8X, '\0', sizeof ( Sn_F402_8X ) );

	memset ( Sn_F402_9A, '\0', sizeof ( Sn_F402_9A ) );
	memset ( Sn_F402_9B, '\0', sizeof ( Sn_F402_9B ) );
	memset ( Sn_F402_9C, '\0', sizeof ( Sn_F402_9C ) );
	memset ( Sn_F402_9X, '\0', sizeof ( Sn_F402_9X ) );

	memset ( Sn_F402_11B, '\0', sizeof ( Sn_F402_11B ) );
	memset ( Sn_F402_11C, '\0', sizeof ( Sn_F402_11C ) );
	memset ( Sn_F402_11X, '\0', sizeof ( Sn_F402_11X ) );

	memset ( Sn_F402_12B, '\0', sizeof ( Sn_F402_12B ) );
	memset ( Sn_F402_12C, '\0', sizeof ( Sn_F402_12C ) );
	memset ( Sn_F402_12X, '\0', sizeof ( Sn_F402_12X ) );

	memset ( Sn_F402_13B, '\0', sizeof ( Sn_F402_13B ) );
	memset ( Sn_F402_13C, '\0', sizeof ( Sn_F402_13C ) );
	memset ( Sn_F402_13X, '\0', sizeof ( Sn_F402_13X ) );

	memset ( Sn_F402_14B, '\0', sizeof ( Sn_F402_14B ) );
	memset ( Sn_F402_14C, '\0', sizeof ( Sn_F402_14C ) );
	memset ( Sn_F402_14X, '\0', sizeof ( Sn_F402_14X ) );

	memset ( Sn_F402_15B, '\0', sizeof ( Sn_F402_15B ) );
	memset ( Sn_F402_15C, '\0', sizeof ( Sn_F402_15C ) );
	memset ( Sn_F402_15X, '\0', sizeof ( Sn_F402_15X ) );

	Sn_F402_2sw = 0;

	m_Arith.l_defn ( 0x16 );

	rsv = NULL;
}

CH31SnFhyo4_02Data::~CH31SnFhyo4_02Data()
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
int CH31SnFhyo4_02Data::GetData( CDBNpSub *pDBNpSub, int sn_seq )
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
	filter.Format ( _T( "sn_seq = %d and hyoId = \'%s\' and version = %d" ), sn_seq, ICS_SN_FHYO_4_02_HYOID, nowVersion );

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
		if ( pShLstVal->itmId == _T( "BBB00010" ) ) {
			m_Util.val_to_bin ( Sn_F402_1A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBB00020" ) ) {
			m_Util.val_to_bin ( Sn_F402_1B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBB00030" ) ) {
			m_Util.val_to_bin ( Sn_F402_1C, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBB00040" ) ) {
			m_Util.val_to_bin ( Sn_F402_1X, pShLstVal->val );
		}
		// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
		else if ( pShLstVal->itmId == _T( "BBL00010" ) ) {
			m_Util.val_to_bin ( Sn_F402_1_1A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBL00020" ) ) {
			m_Util.val_to_bin ( Sn_F402_1_1B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBL00030" ) ) {
			m_Util.val_to_bin ( Sn_F402_1_1C, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBL00040" ) ) {
			m_Util.val_to_bin ( Sn_F402_1_1X, pShLstVal->val );
		}
		// è¡îÔê≈äz
		else if ( pShLstVal->itmId == _T( "BBC00010" ) ) {
			m_Util.val_to_bin ( Sn_F402_2A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBC00020" ) ) {
			m_Util.val_to_bin ( Sn_F402_2B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBC00030" ) ) {
			m_Util.val_to_bin ( Sn_F402_2C, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBC00040" ) ) {
			m_Util.val_to_bin ( Sn_F402_2X, pShLstVal->val );
			Sn_F402_2sw = pShLstVal->sign1;
		}
		// ë›ì|âÒé˚Ç…åWÇÈè¡îÔê≈äz
		else if ( pShLstVal->itmId == _T( "BBD00010" ) ) {
			m_Util.val_to_bin ( Sn_F402_3A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBD00020" ) ) {	
			m_Util.val_to_bin ( Sn_F402_3B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBD00030" ) ) {
			m_Util.val_to_bin ( Sn_F402_3C, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBD00040" ) ) {
			m_Util.val_to_bin ( Sn_F402_3X, pShLstVal->val );
		}
		// çTèúëŒè€édì¸ê≈äz
		else if ( pShLstVal->itmId == _T( "BBE00020" ) ) {
			m_Util.val_to_bin ( Sn_F402_4A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBE00030" ) ) {
			m_Util.val_to_bin ( Sn_F402_4B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBE00040" ) ) {
			m_Util.val_to_bin ( Sn_F402_4C, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBE00050" ) ) {
			m_Util.val_to_bin ( Sn_F402_4X, pShLstVal->val );
		}
		// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
		else if ( pShLstVal->itmId == _T( "BBE00070" ) ) {
			m_Util.val_to_bin ( Sn_F402_5A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBE00080" ) ) {
			m_Util.val_to_bin ( Sn_F402_5B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBE00090" ) ) {
			m_Util.val_to_bin ( Sn_F402_5C, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBE00100" ) ) {
			m_Util.val_to_bin ( Sn_F402_5X, pShLstVal->val );
		}
		// ë›ì|ÇÍÇ…åWÇÈê≈äz
		else if ( pShLstVal->itmId == _T( "BBE00120" ) ) {
			m_Util.val_to_bin ( Sn_F402_6A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBE00130" ) ) {
			m_Util.val_to_bin ( Sn_F402_6B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBE00140" ) ) {
			m_Util.val_to_bin ( Sn_F402_6C, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBE00150" ) ) {
			m_Util.val_to_bin ( Sn_F402_6X, pShLstVal->val );
		}
		// çTèúê≈äzè¨åv
		else if ( pShLstVal->itmId == _T( "BBE00170" ) ) {
			m_Util.val_to_bin ( Sn_F402_7A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBE00180" ) ) {
			m_Util.val_to_bin ( Sn_F402_7B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBE00190" ) ) {
			m_Util.val_to_bin ( Sn_F402_7C, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBE00200" ) ) {
			m_Util.val_to_bin ( Sn_F402_7X, pShLstVal->val );
		}
		// çTèúïsë´ä“ïtê≈äz
		else if ( pShLstVal->itmId == _T( "BBF00010" ) ) {
			m_Util.val_to_bin ( Sn_F402_8A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBF00020" ) ) {
			m_Util.val_to_bin ( Sn_F402_8B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBF00030" ) ) {
			m_Util.val_to_bin ( Sn_F402_8C, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBF00040" ) ) {
			m_Util.val_to_bin ( Sn_F402_8X, pShLstVal->val );
		}
		// ç∑à¯ê≈äz
		else if ( pShLstVal->itmId == _T( "BBG00010" ) ) {
			m_Util.val_to_bin ( Sn_F402_9A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBG00020" ) ) {
			m_Util.val_to_bin ( Sn_F402_9B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBG00030" ) ) {
			m_Util.val_to_bin ( Sn_F402_9C, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBG00040" ) ) {
			m_Util.val_to_bin ( Sn_F402_9X, pShLstVal->val );
		}
		// çTèúïsë´ä“ïtê≈äz
		else if ( pShLstVal->itmId == _T( "BBI00020" ) ) {
			m_Util.val_to_bin ( Sn_F402_11B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBI00030" ) ) {
			m_Util.val_to_bin ( Sn_F402_11C, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBI00040" ) ) {
			m_Util.val_to_bin ( Sn_F402_11X, pShLstVal->val );
		}
		// ç∑à¯ê≈äz
		else if ( pShLstVal->itmId == _T( "BBI00060" ) ) {
			m_Util.val_to_bin ( Sn_F402_12B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBI00070" ) ) {
			m_Util.val_to_bin ( Sn_F402_12C, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBI00080" ) ) {
			m_Util.val_to_bin ( Sn_F402_12X, pShLstVal->val );
		}
		// çáåvç∑à¯ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
		else if ( pShLstVal->itmId == _T( "BBM00010" ) ) {
			m_Util.val_to_bin ( Sn_F402_13B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBM00020" ) ) {
			m_Util.val_to_bin ( Sn_F402_13C, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBM00030" ) ) {
			m_Util.val_to_bin ( Sn_F402_13X, pShLstVal->val );
		}
		// ä“ïtäz
		else if ( pShLstVal->itmId == _T( "BBJ00020" ) ) {
			m_Util.val_to_bin ( Sn_F402_14B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBJ00030" ) ) {
			m_Util.val_to_bin ( Sn_F402_14C, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBJ00040" ) ) {
			m_Util.val_to_bin ( Sn_F402_14X, pShLstVal->val );
		}
		// î[ê≈äz
		else if ( pShLstVal->itmId == _T( "BBJ00060" ) ) {
			m_Util.val_to_bin ( Sn_F402_15B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBJ00070" ) ) {
			m_Util.val_to_bin ( Sn_F402_15C, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "BBJ00080" ) ) {
			m_Util.val_to_bin ( Sn_F402_15X, pShLstVal->val );
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
int CH31SnFhyo4_02Data::SetData( CDBNpSub *pDBNpSub, int sn_seq )
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
	char *pHyoID = NULL;
	int		nowVersion = 2;
	filter.Format ( _T( "sn_seq = %d and hyoId = \'%s\' and version = %d" ), sn_seq, ICS_SN_FHYO_4_02_HYOID, nowVersion );
	pHyoID = ICS_SN_FHYO_4_02_HYOID;

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
		pShLstVal->itmId = _T( "BBB00010" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_1A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBB00020" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_1B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBB00030" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_1C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBB00040" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_1X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBL00010" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_1_1A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBL00020" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_1_1B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBL00030" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_1_1C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBL00040" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_1_1X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// è¡îÔê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBC00010" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_2A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBC00020" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_2B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBC00030" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_2C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBC00040" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_2X );
		pShLstVal->sign1 = Sn_F402_2sw;
		pShLstVal->Update();

		// ë›ì|âÒé˚Ç…åWÇÈè¡îÔê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBD00010" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_3A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBD00020" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_3B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBD00030" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_3C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBD00040" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_3X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// çTèúëŒè€édì¸ê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBE00020" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_4A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBE00030" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_4B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBE00040" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_4C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBE00050" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_4X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBE00070" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_5A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBE00080" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_5B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBE00090" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_5C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBE00100" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_5X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ë›ì|ÇÍÇ…åWÇÈê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBE00120" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_6A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBE00130" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_6B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBE00140" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_6C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBE00150" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_6X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// çTèúê≈äzè¨åv
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBE00170" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_7A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBE00180" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_7B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBE00190" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_7C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBE00200" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_7X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// çTèúïsë´ä“ïtê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBF00010" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_8A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBF00020" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_8B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBF00030" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_8C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBF00040" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_8X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ç∑à¯ê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBG00010" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_9A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBG00020" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_9B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBG00030" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_9C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBG00040" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_9X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// çTèúïsë´ä“ïtê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBI00020" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_11B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBI00030" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_11C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBI00040" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_11X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();


		// ç∑à¯ê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBI00060" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_12B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBI00070" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_12C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBI00080" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_12X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// çáåvç∑à¯ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBM00010" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_13B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBM00020" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_13C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBM00030" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_13X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ä“ïtäz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBJ00020" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_14B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBJ00030" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_14C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBJ00040" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_14X );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// î[ê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBJ00060" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_15B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBJ00070" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_15C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T( "BBJ00080" );
		m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_15X );
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
			if ( pShLstVal->itmId == _T( "BBB00010" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_1A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBB00020" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_1B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBB00030" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_1C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBB00040" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_1X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
			else if ( pShLstVal->itmId == _T( "BBL00010" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_1_1A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBL00020" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_1_1B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBL00030" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_1_1C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBL00040" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_1_1X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// è¡îÔê≈äz
			else if ( pShLstVal->itmId == _T( "BBC00010" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_2A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBC00020" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_2B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBC00030" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_2C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBC00040" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_2X );
				pShLstVal->sign1 = Sn_F402_2sw;
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// ë›ì|âÒé˚Ç…åWÇÈè¡îÔê≈äz
			else if ( pShLstVal->itmId == _T( "BBD00010" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_3A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBD00020" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_3B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBD00030" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_3C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBD00040" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_3X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// çTèúëŒè€édì¸ê≈äz
			else if ( pShLstVal->itmId == _T( "BBE00020" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_4A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBE00030" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_4B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBE00040" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_4C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBE00050" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_4X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
			else if ( pShLstVal->itmId == _T( "BBE00070" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_5A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBE00080" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_5B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBE00090" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_5C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBE00100" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_5X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// ë›ì|ÇÍÇ…åWÇÈê≈äz
			else if ( pShLstVal->itmId == _T( "BBE00120" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_6A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBE00130" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_6B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBE00140" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_6C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBE00150" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_6X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// çTèúê≈äzè¨åv
			else if ( pShLstVal->itmId == _T( "BBE00170" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_7A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBE00180" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_7B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBE00190" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_7C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBE00200" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_7X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// çTèúïsë´ä“ïtê≈äz
			else if ( pShLstVal->itmId == _T( "BBF00010" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_8A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBF00020" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_8B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBF00030" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_8C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBF00040" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_8X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// ç∑à¯ê≈äz
			else if ( pShLstVal->itmId == _T( "BBG00010" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_9A );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBG00020" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_9B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBG00030" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_9C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBG00040" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_9X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// çTèúïsë´ä“ïtê≈äz
			else if ( pShLstVal->itmId == _T( "BBI00020" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_11B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBI00030" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_11C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBI00040" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_11X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// ç∑à¯ê≈äz
			else if ( pShLstVal->itmId == _T( "BBI00060" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_12B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBI00070" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_12C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBI00080" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_12X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// çáåvç∑à¯ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
			else if ( pShLstVal->itmId == _T( "BBM00010" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_13B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBM00020" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_13C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBM00030" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_13X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// ä“ïtäz
			else if ( pShLstVal->itmId == _T( "BBJ00020" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_14B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBJ00030" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_14C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBJ00040" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_14X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			// î[ê≈äz
			else if ( pShLstVal->itmId == _T( "BBJ00060" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_15B );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBJ00070" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_15C );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
			else if ( pShLstVal->itmId == _T( "BBJ00080" ) ) {
				pShLstVal->Edit();
				m_Util.val_to_asci ( &pShLstVal->val, Sn_F402_15X );
				pShLstVal->Update();

				updTagAry.Add ( pShLstVal->itmId );
			}
		}

		int adCnt = 0;
		for ( int i = 0; ; i++ ) {
			if ( strlen ( tagData402[i].tagName ) == 0 ) {
				break;
			}
			adCnt++;
		}

		// Ç»Ç¢Ç‡ÇÃí«â¡
		int updCnt = ( int )updTagAry.GetCount();
		if ( adCnt != updCnt ) {
			CArray<unsigned char*> MonyPtrAry;

			MonyPtrAry.Add ( Sn_F402_1A );
			MonyPtrAry.Add ( Sn_F402_1B );
			MonyPtrAry.Add ( Sn_F402_1C );
			MonyPtrAry.Add ( Sn_F402_1X );
			MonyPtrAry.Add ( Sn_F402_1_1A );
			MonyPtrAry.Add ( Sn_F402_1_1B );
			MonyPtrAry.Add ( Sn_F402_1_1C );
			MonyPtrAry.Add ( Sn_F402_1_1X );
			MonyPtrAry.Add ( Sn_F402_2A );
			MonyPtrAry.Add ( Sn_F402_2B );
			MonyPtrAry.Add ( Sn_F402_2C );
			MonyPtrAry.Add ( Sn_F402_2X );
			MonyPtrAry.Add ( Sn_F402_3A );
			MonyPtrAry.Add ( Sn_F402_3B );
			MonyPtrAry.Add ( Sn_F402_3C );
			MonyPtrAry.Add ( Sn_F402_3X );
			MonyPtrAry.Add ( Sn_F402_4A );
			MonyPtrAry.Add ( Sn_F402_4B );
			MonyPtrAry.Add ( Sn_F402_4C );
			MonyPtrAry.Add ( Sn_F402_4X );
			MonyPtrAry.Add ( Sn_F402_5A );
			MonyPtrAry.Add ( Sn_F402_5B );
			MonyPtrAry.Add ( Sn_F402_5C );
			MonyPtrAry.Add ( Sn_F402_5X );
			MonyPtrAry.Add ( Sn_F402_6A );
			MonyPtrAry.Add ( Sn_F402_6B );
			MonyPtrAry.Add ( Sn_F402_6C );
			MonyPtrAry.Add ( Sn_F402_6X );
			MonyPtrAry.Add ( Sn_F402_7A );
			MonyPtrAry.Add ( Sn_F402_7B );
			MonyPtrAry.Add ( Sn_F402_7C );
			MonyPtrAry.Add ( Sn_F402_7X );
			MonyPtrAry.Add ( Sn_F402_8A );
			MonyPtrAry.Add ( Sn_F402_8B );
			MonyPtrAry.Add ( Sn_F402_8C );
			MonyPtrAry.Add ( Sn_F402_8X );
			MonyPtrAry.Add ( Sn_F402_9A );
			MonyPtrAry.Add ( Sn_F402_9B );
			MonyPtrAry.Add ( Sn_F402_9C );
			MonyPtrAry.Add ( Sn_F402_9X );
			MonyPtrAry.Add ( Sn_F402_11B );
			MonyPtrAry.Add ( Sn_F402_11C );
			MonyPtrAry.Add ( Sn_F402_11X );
			MonyPtrAry.Add ( Sn_F402_12B );
			MonyPtrAry.Add ( Sn_F402_12C );
			MonyPtrAry.Add ( Sn_F402_12X );
			MonyPtrAry.Add ( Sn_F402_13B );
			MonyPtrAry.Add ( Sn_F402_13C );
			MonyPtrAry.Add ( Sn_F402_13X );
			MonyPtrAry.Add ( Sn_F402_14B );
			MonyPtrAry.Add ( Sn_F402_14C );
			MonyPtrAry.Add ( Sn_F402_14X );
			MonyPtrAry.Add ( Sn_F402_15B );
			MonyPtrAry.Add ( Sn_F402_15C );
			MonyPtrAry.Add ( Sn_F402_15X );

			BOOL findFlg = FALSE;
			for ( int i = 0; ; i++ ) {
				if ( strlen ( tagData402[i].tagName ) == 0 ) {
					break;
				}

				findFlg = FALSE;
				for ( int j = 0; j < updCnt; j++ ) {
					if ( updTagAry[j].CompareNoCase ( tagData402[i].tagName ) == 0 ) {
						findFlg = TRUE;
						break;
					}
				}
				if ( findFlg == FALSE ) {
					// äYìñÉ^ÉOÇÃí«â¡
					pShLstVal->AddNew();
					pShLstVal->sn_seq = sn_seq;
					pShLstVal->version = nowVersion;
					pShLstVal->hyoId = pHyoID;
					pShLstVal->itmId = tagData402[i].tagName;
					m_Util.val_to_asci ( &pShLstVal->val, MonyPtrAry[i] );
					pShLstVal->sign1 = 0;
					pShLstVal->Update();
				}
			}
		}
	}

	return 0;
}

//===============================================
// èWåvçœÇ›ÉfÅ[É^ÇÃÉZÉbÉg
//-----------------------------------------------
// à¯êî
//		pSyzSyukei		:		è¡îÔê≈èWåvÉNÉâÉX
//		isPluralZei		:		ï°êîê≈ó¶
//-----------------------------------------------
// ï‘ëóíl
//			0			:		ê≥èÌèIóπ
//			-1			:		ÉGÉâÅ[
//===============================================
int CH31SnFhyo4_02Data::SetCalqedData ( CSyzSyukei *pSyzSyukei, BOOL &isPluralZei )
{
	isPluralZei = FALSE;

	MoneyBasejagArray money;

	money = pSyzSyukei->GetShinkokuData ( _T( "34" ) );

	int cnt = ( int )money.GetCount();
	int incnt = 0;

	unsigned char exZeiFlg = 0x00;	// Å†Å†Å†Å† Å†Å°Å°Å°
									// D0ÅFê≈ó¶3%
									// D1ÅFê≈ó¶4%
									// D2ÅFê≈ó¶6.3%

	for ( int i = 0; i < cnt; i++ ) {
		incnt = ( int )money[i].GetCount();
		for ( int j = 0; j < incnt; j++ ) {
			// â€ê≈ïWèÄäz
			if ( money[i][j].code.Left ( 4 ) == _T( "3401" ) ) {
				if ( money[i][j].code == _T( "340100" ) ) {
					memmove ( Sn_F402_1X, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "340101" ) ) {
					memmove ( Sn_F402_1A, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "340102" ) ) {
					memmove ( Sn_F402_1B, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "340103" ) ) {
					memmove ( Sn_F402_1C, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
			}
			// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
			else if ( money[i][j].code.Left ( 4 ) == _T( "3402" ) ) {
				if ( money[i][j].code == _T( "340200" ) ) {
					memmove ( Sn_F402_1_1X, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "340201" ) ) {
					memmove ( Sn_F402_1_1A, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "340202" ) ) {
					memmove ( Sn_F402_1_1B, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "340203" ) ) {
					memmove ( Sn_F402_1_1C, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
			}
			// è¡îÔê≈äz
			else if ( money[i][j].code.Left ( 4 ) == _T( "3403" ) ) {
				if ( money[i][j].code == _T( "340300" ) ) {
					memmove ( Sn_F402_2X, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "340301" ) ) {
					memmove ( Sn_F402_2A, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "340302" ) ) {
					memmove ( Sn_F402_2B, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "340303" ) ) {
					memmove ( Sn_F402_2C, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
			}
			// ë›ì|âÒé˚Ç…åWÇÈè¡îÔê≈äz
			else if ( money[i][j].code.Left ( 4 ) == _T( "3404" ) ) {
				if ( money[i][j].code == _T( "340400" ) ) {
					memmove ( Sn_F402_3X, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "340401" ) ) {
					memmove ( Sn_F402_3A, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "340402" ) ) {
					memmove ( Sn_F402_3B, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "340403" ) ) {
					memmove ( Sn_F402_3C, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
			}
			// çTèúëŒè€édì¸ê≈äz
			else if ( money[i][j].code.Left ( 4 ) == _T( "3405" ) ) {
				if ( money[i][j].code == _T( "340500" ) ) {
					memmove ( Sn_F402_4X, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "340501" ) ) {
					memmove ( Sn_F402_4A, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "340502" ) ) {
					memmove ( Sn_F402_4B, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "340503" ) ) {
					memmove ( Sn_F402_4C, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
			}
			// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
			else if ( money[i][j].code.Left ( 4 ) == _T( "3406" ) ) {
				if ( money[i][j].code == _T( "340600" ) ) {
					memmove ( Sn_F402_5X, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "340601" ) ) {
					memmove ( Sn_F402_5A, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "340602" ) ) {
					memmove ( Sn_F402_5B, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "340603" ) ) {
					memmove ( Sn_F402_5C, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
			}
			// ë›ì|ÇÍÇ…åWÇÈê≈äz
			else if ( money[i][j].code.Left ( 4 ) == _T( "3407" ) ) {
				if ( money[i][j].code == _T( "340700" ) ) {
					memmove ( Sn_F402_6X, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "340701" ) ) {
					memmove ( Sn_F402_6A, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "340702" ) ) {
					memmove ( Sn_F402_6B, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "340703" ) ) {
					memmove ( Sn_F402_6C, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
			}
			// çTèúê≈äzè¨åv
			else if ( money[i][j].code.Left ( 4 ) == _T( "3408" ) ) {
				if ( money[i][j].code == _T( "340800" ) ) {
					memmove ( Sn_F402_7X, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "340801" ) ) {
					memmove ( Sn_F402_7A, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "340802" ) ) {
					memmove ( Sn_F402_7B, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "340803" ) ) {
					memmove ( Sn_F402_7C, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
			}
			// çTèúïsë´ä“ïtê≈äz
			else if ( money[i][j].code.Left ( 4 ) == _T( "3409" ) ) {
				if ( money[i][j].code == _T( "340900" ) ) {
					memmove ( Sn_F402_8X, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "340901" ) ) {
					memmove ( Sn_F402_8A, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "340902" ) ) {
					memmove ( Sn_F402_8B, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "340903" ) ) {
					memmove ( Sn_F402_8C, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
			}
			// ç∑à¯ê≈äz
			else if ( money[i][j].code.Left ( 4 ) == _T( "3410" ) ) {
				if ( money[i][j].code == _T( "341000" ) ) {
					memmove ( Sn_F402_9X, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "341001" ) ) {
					memmove ( Sn_F402_9A, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "341002" ) ) {
					memmove ( Sn_F402_9B, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "341003" ) ) {
					memmove ( Sn_F402_9C, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
			}
			// çTèúïsë´ä“ïtê≈äz
			else if ( money[i][j].code.Left ( 4 ) == _T( "3412" ) ) {
				if ( money[i][j].code == _T( "341200" ) ) {
					memmove ( Sn_F402_11X, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "341202" ) ) {
					memmove ( Sn_F402_11B, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "341203" ) ) {
					memmove ( Sn_F402_11C, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
			}
			// ç∑à¯ê≈äz
			else if ( money[i][j].code.Left ( 4 ) == _T( "3413" ) ) {
				if ( money[i][j].code == _T( "341300" ) ) {
					memmove ( Sn_F402_12X, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "341302" ) ) {
					memmove ( Sn_F402_12B, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "341303" ) ) {
					memmove ( Sn_F402_12C, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
			}
			// çáåvç∑à¯ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
			else if ( money[i][j].code.Left ( 4 ) == _T( "3414" ) ) {
				if ( money[i][j].code == _T( "341400" ) ) {
					memmove ( Sn_F402_13X, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "341402" ) ) {
					memmove ( Sn_F402_13B, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "341403" ) ) {
					memmove ( Sn_F402_13C, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
			}
			// ä“ïtäz
			else if ( money[i][j].code.Left ( 4 ) == _T( "3415" ) ) {
				if ( money[i][j].code == _T( "341500" ) ) {
					memmove ( Sn_F402_14X, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "341502" ) ) {
					memmove ( Sn_F402_14B, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "341503" ) ) {
					memmove ( Sn_F402_14C, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
			}
			// î[ê≈äz
			else if ( money[i][j].code.Left ( 4 ) == _T( "3416" ) ) {
				if ( money[i][j].code == _T( "341600" ) ) {
					memmove ( Sn_F402_15X, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if ( money[i][j].code == _T( "341602" ) ) {
					memmove ( Sn_F402_15B, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
				else if ( money[i][j].code == _T( "341603" ) ) {
					memmove ( Sn_F402_15C, money[i][j].arith, MONY_BUF_SIZE );
					if ( m_Arith.l_test ( money[i][j].arith ) ) {
						exZeiFlg |= 0x01;
					}
				}
			}
		}
	}


	if ( ( ( exZeiFlg & 0x07 ) == 0x01 ) || ( ( exZeiFlg & 0x07 ) == 0x02 ) || ( ( exZeiFlg & 0x04 ) == 0x04 ) || ( exZeiFlg == 0x00 ) ) {
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
int CH31SnFhyo4_02Data::ClearDataByRenew(CDBNpSub *pDBNpSub, int type)
{

	m_Arith.l_clear(Sn_F402_1A);
	m_Arith.l_clear(Sn_F402_1B);
	m_Arith.l_clear(Sn_F402_1C);
	m_Arith.l_clear(Sn_F402_1X);

	m_Arith.l_clear(Sn_F402_1_1A);
	m_Arith.l_clear(Sn_F402_1_1B);
	m_Arith.l_clear(Sn_F402_1_1C);
	m_Arith.l_clear(Sn_F402_1_1X);

	m_Arith.l_clear(Sn_F402_2A);
	m_Arith.l_clear(Sn_F402_2B);
	m_Arith.l_clear(Sn_F402_2C);
	m_Arith.l_clear(Sn_F402_2X);

	m_Arith.l_clear(Sn_F402_3A);
	m_Arith.l_clear(Sn_F402_3B);
	m_Arith.l_clear(Sn_F402_3C);
	m_Arith.l_clear(Sn_F402_3X);

	m_Arith.l_clear(Sn_F402_4A);
	m_Arith.l_clear(Sn_F402_4B);
	m_Arith.l_clear(Sn_F402_4C);
	m_Arith.l_clear(Sn_F402_4X);

	m_Arith.l_clear(Sn_F402_5A);
	m_Arith.l_clear(Sn_F402_5B);
	m_Arith.l_clear(Sn_F402_5C);
	m_Arith.l_clear(Sn_F402_5X);

	m_Arith.l_clear(Sn_F402_6A);
	m_Arith.l_clear(Sn_F402_6B);
	m_Arith.l_clear(Sn_F402_6C);
	m_Arith.l_clear(Sn_F402_6X);

	m_Arith.l_clear(Sn_F402_7A);
	m_Arith.l_clear(Sn_F402_7B);
	m_Arith.l_clear(Sn_F402_7C);
	m_Arith.l_clear(Sn_F402_7X);

	m_Arith.l_clear(Sn_F402_8A);
	m_Arith.l_clear(Sn_F402_8B);
	m_Arith.l_clear(Sn_F402_8C);
	m_Arith.l_clear(Sn_F402_8X);

	m_Arith.l_clear(Sn_F402_9A);
	m_Arith.l_clear(Sn_F402_9B);
	m_Arith.l_clear(Sn_F402_9C);
	m_Arith.l_clear(Sn_F402_9X);

	m_Arith.l_clear(Sn_F402_11B);
	m_Arith.l_clear(Sn_F402_11C);
	m_Arith.l_clear(Sn_F402_11X);

	m_Arith.l_clear(Sn_F402_12B);
	m_Arith.l_clear(Sn_F402_12C);
	m_Arith.l_clear(Sn_F402_12X);

	m_Arith.l_clear(Sn_F402_13B);
	m_Arith.l_clear(Sn_F402_13C);
	m_Arith.l_clear(Sn_F402_13X);

	m_Arith.l_clear(Sn_F402_14B);
	m_Arith.l_clear(Sn_F402_14C);
	m_Arith.l_clear(Sn_F402_14X);

	m_Arith.l_clear(Sn_F402_15B);
	m_Arith.l_clear(Sn_F402_15C);
	m_Arith.l_clear(Sn_F402_15X);
	

	Sn_F402_2sw = 0;

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
int CH31SnFhyo4_02Data::ClearManualInputSign(BOOL isChgKzritu)
{

//	Sn_F402_2sw = 0;//yoshida190909

	//if (isChgKzritu) {
	//	Sn_F402_2sw = 0x111;
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
int CH31SnFhyo4_02Data::GetPastData ( CDBNpSub *pDBNpSub, int sn_seq, CSnHeadData *pSnHeadData )
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

	// òAìÆéûÇÕãåóléÆÇÃÉfÅ[É^ÇéÊìæÇµÇ»Ç¢
	if ( !( pSnHeadData->Sn_Sign4 & 0x80 ) ) {
		return 0;
	}


	int nowVersion = 1;
	CString filter;
	filter.Format ( _T( "sn_seq = %d and hyoId = \'%s\' and version = %d" ), sn_seq, ICS_SN_FHYO_4_HYOID, nowVersion );

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

		// â€ê≈ïWèÄäz
		if ( pShLstVal->itmId == _T( "AFB00010" ) ) {
			m_Util.val_to_bin ( Sn_F402_1_1A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AFB00020" ) ) {
			m_Util.val_to_bin ( Sn_F402_1_1B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AFB00030" ) ) {
			m_Util.val_to_bin ( Sn_F402_1_1C, pShLstVal->val );
		}
		// è¡îÔê≈äz
		else if ( pShLstVal->itmId == _T( "AFC00010" ) ) {
			m_Util.val_to_bin ( Sn_F402_2A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AFC00020" ) ) {
			m_Util.val_to_bin ( Sn_F402_2B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AFC00030" ) ) {
			m_Util.val_to_bin ( Sn_F402_2C, pShLstVal->val );
			Sn_F402_2sw = pShLstVal->sign1;
		}
		// ë›ì|âÒé˚Ç…åWÇÈè¡îÔê≈äz
		else if ( pShLstVal->itmId == _T( "AFD00010" ) ) {
			m_Util.val_to_bin ( Sn_F402_3A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AFD00020" ) ) {
			m_Util.val_to_bin ( Sn_F402_3B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AFD00030" ) ) {
			m_Util.val_to_bin ( Sn_F402_3C, pShLstVal->val );
		}
		// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
		else if ( pShLstVal->itmId == _T( "AFE00070" ) ) {
			m_Util.val_to_bin ( Sn_F402_5A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AFE00080" ) ) {
			m_Util.val_to_bin ( Sn_F402_5B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AFE00090" ) ) {
			m_Util.val_to_bin ( Sn_F402_5C, pShLstVal->val );
		}
		// ë›ì|ÇÍÇ…åWÇÈê≈äz
		else if ( pShLstVal->itmId == _T( "AFE00120" ) ) {
			m_Util.val_to_bin ( Sn_F402_6A, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AFE00130" ) ) {
			m_Util.val_to_bin ( Sn_F402_6B, pShLstVal->val );
		}
		else if ( pShLstVal->itmId == _T( "AFE00140" ) ) {
			m_Util.val_to_bin ( Sn_F402_6C, pShLstVal->val );
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
bool CH31SnFhyo4_02Data::IsExistData(CDBNpSub *pDBNpSub, int sn_seq)
{
	ASSERT(pDBNpSub);
	if( pDBNpSub == NULL ) {
		return false;
	}

	int nowVersion = 2;
	CString filter;
	if( sn_seq != -1 ){
		filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_4_02_HYOID, nowVersion);
	}
	else{
		filter.Format(_T("hyoId = \'%s\' and version = %d"), ICS_SN_FHYO_4_02_HYOID, nowVersion);
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
