#include "StdAfx.h"
#include "H26SnFhyo10Data.h"

//--> '14.10.17 INS START
static SH_TAGNAME	tagData40[] = {
	{_T("AFB00010"), 1 },
	{_T("AFB00020"), 0 },
	{_T("AFB00030"), 3 },

	{_T("AFC00010"), 1 },
	{_T("AFC00020"), 0 },
	{_T("AFC00030"), 3 },

	{_T("AFD00010"), 1 },
	{_T("AFD00020"), 0 },
	{_T("AFD00030"), 3 },

	{_T("AFE00020"), 1 },
	{_T("AFE00030"), 0 },
	{_T("AFE00040"), 3 },
	{_T("AFE00050"), 999 },

	{_T("AFE00070"), 1 },
	{_T("AFE00080"), 0 },
	{_T("AFE00090"), 3 },

	{_T("AFE00120"), 1 },
	{_T("AFE00130"), 0 },
	{_T("AFE00140"), 3 },

	{_T("AFF00010"), 1 },
	{_T("AFF00020"), 0 },
	{_T("AFF00030"), 3 },

	{_T("AFG00010"), 1 },
	{_T("AFG00020"), 0 },
	{_T("AFG00030"), 3 },

	{_T("AFH00010"), 999 },

	{_T("") }
};
//<-- '14.10.17 INS END

//--> '16.01.05 INS START
static SH_TAGNAME	tagData10[] = {
	{_T("ACB00010"), 1 },
	{_T("ACB00020"), 0 },
	{_T("ACB00030"), 3 },

	{_T("ACC00010"), 1 },
	{_T("ACC00020"), 0 },
	{_T("ACC00030"), 3 },

	{_T("ACD00010"), 1 },
	{_T("ACD00020"), 0 },
	{_T("ACD00030"), 3 },

	{_T("ACE00020"), 1 },
	{_T("ACE00030"), 0 },
	{_T("ACE00040"), 3 },
//	{_T("AFE00050"), 999 },

	{_T("ACE00070"), 1 },
	{_T("ACE00080"), 0 },
	{_T("ACE00090"), 3 },

	{_T("ACE00120"), 1 },
	{_T("ACE00130"), 0 },
	{_T("ACE00140"), 3 },

	{_T("ACF00010"), 1 },
	{_T("ACF00020"), 0 },
	{_T("ACF00030"), 3 },

	{_T("ACG00010"), 1 },
	{_T("ACG00020"), 0 },
	{_T("ACG00030"), 3 },

	{_T("ACH00010"), 999 },

	{_T("") }
};
//<-- '16.01.05 INS END

//-----------------------------------------------------------------------------
// ÉRÉìÉXÉgÉâÉNÉ^
//-----------------------------------------------------------------------------
CH26SnFhyo10Data::CH26SnFhyo10Data( BOOL isTransitionalMeasure )
	: m_isTransitionalMeasure(isTransitionalMeasure)
{
	memset( Sn_1F1A, '\0', sizeof(Sn_1F1A) );
	memset( Sn_1F1B, '\0', sizeof(Sn_1F1B) );
	memset( Sn_1F1C, '\0', sizeof(Sn_1F1C) );

	memset( Sn_1F2A, '\0', sizeof(Sn_1F2A) );
	memset( Sn_1F2B, '\0', sizeof(Sn_1F2B) );
	memset( Sn_1F2C, '\0', sizeof(Sn_1F2C) );

	memset( Sn_1F3A, '\0', sizeof(Sn_1F3A) );
	memset( Sn_1F3B, '\0', sizeof(Sn_1F3B) );
	memset( Sn_1F3C, '\0', sizeof(Sn_1F3C) );

	memset( Sn_1F4A, '\0', sizeof(Sn_1F4A) );
	memset( Sn_1F4B, '\0', sizeof(Sn_1F4B) );
	memset( Sn_1F4C, '\0', sizeof(Sn_1F4C) );
//--> '14.10.17 INS START
	memset( Sn_1F4T, '\0', sizeof(Sn_1F4T) );
//--> '14.10.17 INS END

	memset( Sn_1F5A, '\0', sizeof(Sn_1F5A) );
	memset( Sn_1F5B, '\0', sizeof(Sn_1F5B) );
	memset( Sn_1F5C, '\0', sizeof(Sn_1F5C) );

	memset( Sn_1F6A, '\0', sizeof(Sn_1F6A) );
	memset( Sn_1F6B, '\0', sizeof(Sn_1F6B) );
	memset( Sn_1F6C, '\0', sizeof(Sn_1F6C) );

	memset( Sn_1F8A, '\0', sizeof(Sn_1F8A) );
	memset( Sn_1F8B, '\0', sizeof(Sn_1F8B) );
	memset( Sn_1F8C, '\0', sizeof(Sn_1F8C) );

	memset( Sn_1F9A, '\0', sizeof(Sn_1F9A) );
	memset( Sn_1F9B, '\0', sizeof(Sn_1F9B) );
	memset( Sn_1F9C, '\0', sizeof(Sn_1F9C) );

	memset( Sn_1F10T, '\0', sizeof(Sn_1F10T) );

	Sn_1F2sw = 0;

	m_Arith.l_defn( 0x16 );

	memset( rsv1, '\0', sizeof(rsv1) );
	memset( rsv2, '\0', sizeof(rsv2) );
}

//-----------------------------------------------------------------------------
// ÉfÉXÉgÉâÉNÉ^
//-----------------------------------------------------------------------------
CH26SnFhyo10Data::~CH26SnFhyo10Data()
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
int CH26SnFhyo10Data::GetData( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}
	ASSERT( pDBNpSub->zvol );
	if( pDBNpSub->zvol == NULL ){
		return -1;
	}

	EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki( pDBNpSub->zvol );
	if( KzHoushiki == ID_ICSSH_MENZEI ){
		return 0;
	}

	CString	filter;
	int		nowVersion = 1;
	if( KzHoushiki == ID_ICSSH_GENSOKU ){
		filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_1_HYOID, nowVersion );
	}
	else if( KzHoushiki == ID_ICSSH_KANNI ){
		filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_4_HYOID, nowVersion );
	}

	if( m_DBSyzAccess.m_pSH_LST_VAL ){
		m_DBSyzAccess.m_pSH_LST_VAL->Requery( filter );
	}
	else{
		if( m_DBSyzAccess.OpenShLstVal(pDBNpSub->m_database, filter) ){
			return -1;
		}
	}

	CSH_LST_VAL	*pShLstVal = m_DBSyzAccess.m_pSH_LST_VAL;

	if( KzHoushiki == ID_ICSSH_GENSOKU ){	// å¥ë•â€ê≈
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

			// â€ê≈ïWèÄäz
			if( pShLstVal->itmId == _T("ACB00010") ){
				if( m_isTransitionalMeasure ){
					m_Util.val_to_bin( Sn_1F1A, pShLstVal->val );
				}
				else{
					memset( Sn_1F1A, '\0', sizeof(Sn_1F1A) );
				}
			}
			else if( pShLstVal->itmId == _T("ACB00020") ){
				if( m_isTransitionalMeasure ){
					m_Util.val_to_bin( Sn_1F1B, pShLstVal->val );
				}
				else{
					memset( Sn_1F1B, '\0', sizeof(Sn_1F1B) );
				}
			}
			else if( pShLstVal->itmId == _T("ACB00030") ){
				m_Util.val_to_bin( Sn_1F1C, pShLstVal->val );
			}
			// è¡îÔê≈äz
			else if( pShLstVal->itmId == _T("ACC00010") ){
				if( m_isTransitionalMeasure ){
					m_Util.val_to_bin( Sn_1F2A, pShLstVal->val );
				}
				else{
					memset( Sn_1F2A, '\0', sizeof(Sn_1F2A) );
				}
			}
			else if( pShLstVal->itmId == _T("ACC00020") ){
				if( m_isTransitionalMeasure ){
					m_Util.val_to_bin( Sn_1F2B, pShLstVal->val );
				}
				else{
					memset( Sn_1F2B, '\0', sizeof(Sn_1F2B) );
				}
			}
			else if( pShLstVal->itmId == _T("ACC00030") ){
				m_Util.val_to_bin( Sn_1F2C, pShLstVal->val );
				Sn_1F2sw = pShLstVal->sign1;//[Y]
			}
			// çTèúâﬂëÂí≤êÆê≈äz
			else if( pShLstVal->itmId == _T("ACD00010") ){
				if( m_isTransitionalMeasure ){
					m_Util.val_to_bin( Sn_1F3A, pShLstVal->val );
				}
				else{
					memset( Sn_1F3A, '\0', sizeof(Sn_1F3A) );
				}
			}
			else if( pShLstVal->itmId == _T("ACD00020") ){
				if( m_isTransitionalMeasure ){
					m_Util.val_to_bin( Sn_1F3B, pShLstVal->val );
				}
				else{
					memset( Sn_1F3B, '\0', sizeof(Sn_1F3B) );
				}
			}
			else if( pShLstVal->itmId == _T("ACD00030") ){
				m_Util.val_to_bin( Sn_1F3C, pShLstVal->val );
			}
			// çTèúëŒè€édì¸ê≈äz
			else if( pShLstVal->itmId == _T("ACE00020") ){
				if( m_isTransitionalMeasure ){
					m_Util.val_to_bin( Sn_1F4A, pShLstVal->val );
				}
				else{
					memset( Sn_1F4A, '\0', sizeof(Sn_1F4A) );
				}
			}
			else if( pShLstVal->itmId == _T("ACE00030") ){
				if( m_isTransitionalMeasure ){
					m_Util.val_to_bin( Sn_1F4B, pShLstVal->val );
				}
				else{
					memset( Sn_1F4B, '\0', sizeof(Sn_1F4B) );
				}
			}
			else if( pShLstVal->itmId == _T("ACE00040") ){
				m_Util.val_to_bin( Sn_1F4C, pShLstVal->val );
			}
			// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
			else if( pShLstVal->itmId == _T("ACE00070") ){
				if( m_isTransitionalMeasure ){
					m_Util.val_to_bin( Sn_1F5A, pShLstVal->val );
				}
				else{
					memset( Sn_1F5A, '\0', sizeof(Sn_1F5A) );
				}
			}
			else if( pShLstVal->itmId == _T("ACE00080") ){
				if( m_isTransitionalMeasure ){
					m_Util.val_to_bin( Sn_1F5B, pShLstVal->val );
				}
				else{
					memset( Sn_1F5B, '\0', sizeof(Sn_1F5B) );
				}
			}
			else if( pShLstVal->itmId == _T("ACE00090") ){
				m_Util.val_to_bin( Sn_1F5C, pShLstVal->val );
			}
			// ë›ì|Ç…åWÇÈê≈äz
			else if( pShLstVal->itmId == _T("ACE00120") ){
				if( m_isTransitionalMeasure ){
					m_Util.val_to_bin( Sn_1F6A, pShLstVal->val );
				}
				else{
					memset( Sn_1F6A, '\0', sizeof(Sn_1F6A) );
				}
			}
			else if( pShLstVal->itmId == _T("ACE00130") ){
				if( m_isTransitionalMeasure ){
					m_Util.val_to_bin( Sn_1F6B, pShLstVal->val );
				}
				else{
					memset( Sn_1F6B, '\0', sizeof(Sn_1F6B) );
				}
			}
			else if( pShLstVal->itmId == _T("ACE00140") ){
				m_Util.val_to_bin( Sn_1F6C, pShLstVal->val );
			}
			// çTèúïsë´ê≈äz
			else if( pShLstVal->itmId == _T("ACF00010") ){
				if( m_isTransitionalMeasure ){
					m_Util.val_to_bin( Sn_1F8A, pShLstVal->val );
				}
				else{
					memset( Sn_1F8A, '\0', sizeof(Sn_1F8A) );
				}
			}
			else if( pShLstVal->itmId == _T("ACF00020") ){
				if( m_isTransitionalMeasure ){
					m_Util.val_to_bin( Sn_1F8B, pShLstVal->val );
				}
				else{
					memset( Sn_1F8B, '\0', sizeof(Sn_1F8B) );
				}
			}
			else if( pShLstVal->itmId == _T("ACF00030") ){
				m_Util.val_to_bin( Sn_1F8C, pShLstVal->val );
			}
			// ç∑à¯ê≈äz
			else if( pShLstVal->itmId == _T("ACG00010") ){
				if( m_isTransitionalMeasure ){
					m_Util.val_to_bin( Sn_1F9A, pShLstVal->val );
				}
				else{
					memset( Sn_1F9A, '\0', sizeof(Sn_1F9A) );
				}
			}
			else if( pShLstVal->itmId == _T("ACG00020") ){
				if( m_isTransitionalMeasure ){
					m_Util.val_to_bin( Sn_1F9B, pShLstVal->val );
				}
				else{
					memset( Sn_1F9B, '\0', sizeof(Sn_1F9B) );
				}
			}
			else if( pShLstVal->itmId == _T("ACG00030") ){
				m_Util.val_to_bin( Sn_1F9C, pShLstVal->val );
			}
			// çáåvç∑à¯ê≈äz
			else if( pShLstVal->itmId == _T("ACH00010") ){
				m_Util.val_to_bin( Sn_1F10T, pShLstVal->val );
			}
		}
	}
	else if( KzHoushiki == ID_ICSSH_KANNI ){
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

			// â€ê≈ïWèÄäz
			if( pShLstVal->itmId == _T("AFB00010") ){
				if( m_isTransitionalMeasure ){
					m_Util.val_to_bin( Sn_1F1A, pShLstVal->val );
				}
				else{
					memset( Sn_1F1A, '\0', sizeof(Sn_1F1A) );
				}
			}
			else if( pShLstVal->itmId == _T("AFB00020") ){
				if( m_isTransitionalMeasure ){
					m_Util.val_to_bin( Sn_1F1B, pShLstVal->val );
				}
				else{
					memset( Sn_1F1B, '\0', sizeof(Sn_1F1B) );
				}
			}
			else if( pShLstVal->itmId == _T("AFB00030") ){
				m_Util.val_to_bin( Sn_1F1C, pShLstVal->val );
			}
			// è¡îÔê≈äz
			else if( pShLstVal->itmId == _T("AFC00010") ){
				if( m_isTransitionalMeasure ){
					m_Util.val_to_bin( Sn_1F2A, pShLstVal->val );
				}
				else{
					memset( Sn_1F2A, '\0', sizeof(Sn_1F2A) );
				}
			}
			else if( pShLstVal->itmId == _T("AFC00020") ){
				if( m_isTransitionalMeasure ){
					m_Util.val_to_bin( Sn_1F2B, pShLstVal->val );
				}
				else{
					memset( Sn_1F2B, '\0', sizeof(Sn_1F2B) );
				}
			}
			else if( pShLstVal->itmId == _T("AFC00030") ){
				m_Util.val_to_bin( Sn_1F2C, pShLstVal->val );
				Sn_1F2sw = pShLstVal->sign1;//[Y]
			}
			// ë›ì|âÒé˚Ç…åWÇÈè¡îÔê≈äz
			else if( pShLstVal->itmId == _T("AFD00010") ){
				if( m_isTransitionalMeasure ){
					m_Util.val_to_bin( Sn_1F3A, pShLstVal->val );
				}
				else{
					memset( Sn_1F3A, '\0', sizeof(Sn_1F3A) );
				}
			}
			else if( pShLstVal->itmId == _T("AFD00020") ){
				if( m_isTransitionalMeasure ){
					m_Util.val_to_bin( Sn_1F3B, pShLstVal->val );
				}
				else{
					memset( Sn_1F3B, '\0', sizeof(Sn_1F3B) );
				}
			}
			else if( pShLstVal->itmId == _T("AFD00030") ){
				m_Util.val_to_bin( Sn_1F3C, pShLstVal->val );
			}
			// çTèúëŒè€édì¸ê≈äz
			else if( pShLstVal->itmId == _T("AFE00020") ){
				if( m_isTransitionalMeasure ){
					m_Util.val_to_bin( Sn_1F4A, pShLstVal->val );
				}
				else{
					memset( Sn_1F4A, '\0', sizeof(Sn_1F4A) );
				}
			}
			else if( pShLstVal->itmId == _T("AFE00030") ){
				if( m_isTransitionalMeasure ){
					m_Util.val_to_bin( Sn_1F4B, pShLstVal->val );
				}
				else{
					memset( Sn_1F4B, '\0', sizeof(Sn_1F4B) );
				}
			}
			else if( pShLstVal->itmId == _T("AFE00040") ){
				m_Util.val_to_bin( Sn_1F4C, pShLstVal->val );
			}
//--> '14.10.17 INS START
			else if( pShLstVal->itmId == _T("AFE00050") ){
				m_Util.val_to_bin( Sn_1F4T, pShLstVal->val );
			}
//<-- '14.10.17 INS END
			// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
			else if( pShLstVal->itmId == _T("AFE00070") ){
				if( m_isTransitionalMeasure ){
					m_Util.val_to_bin( Sn_1F5A, pShLstVal->val );
				}
				else{
					memset( Sn_1F5A, '\0', sizeof(Sn_1F5A) );
				}
			}
			else if( pShLstVal->itmId == _T("AFE00080") ){
				if( m_isTransitionalMeasure ){
					m_Util.val_to_bin( Sn_1F5B, pShLstVal->val );
				}
				else{
					memset( Sn_1F5B, '\0', sizeof(Sn_1F5B) );
				}
			}
			else if( pShLstVal->itmId == _T("AFE00090") ){
				m_Util.val_to_bin( Sn_1F5C, pShLstVal->val );
			}
			// ë›ì|Ç…åWÇÈê≈äz
			else if( pShLstVal->itmId == _T("AFE00120") ){
				if( m_isTransitionalMeasure ){
					m_Util.val_to_bin( Sn_1F6A, pShLstVal->val );
				}
				else{
					memset( Sn_1F6A, '\0', sizeof(Sn_1F6A) );
				}
			}
			else if( pShLstVal->itmId == _T("AFE00130") ){
				if( m_isTransitionalMeasure ){
					m_Util.val_to_bin( Sn_1F6B, pShLstVal->val );
				}
				else{
					memset( Sn_1F6B, '\0', sizeof(Sn_1F6B) );
				}
			}
			else if( pShLstVal->itmId == _T("AFE00140") ){
				m_Util.val_to_bin( Sn_1F6C, pShLstVal->val );
			}
			// çTèúïsë´ê≈äz
			else if( pShLstVal->itmId == _T("AFF00010") ){
				if( m_isTransitionalMeasure ){
					m_Util.val_to_bin( Sn_1F8A, pShLstVal->val );
				}
				else{
					memset( Sn_1F8A, '\0', sizeof(Sn_1F8A) );
				}
			}
			else if( pShLstVal->itmId == _T("AFF00020") ){
				if( m_isTransitionalMeasure ){
					m_Util.val_to_bin( Sn_1F8B, pShLstVal->val );
				}
				else{
					memset( Sn_1F8B, '\0', sizeof(Sn_1F8B) );
				}
			}
			else if( pShLstVal->itmId == _T("AFF00030") ){
				m_Util.val_to_bin( Sn_1F8C, pShLstVal->val );
			}
			// ç∑à¯ê≈äz
			else if( pShLstVal->itmId == _T("AFG00010") ){
				if( m_isTransitionalMeasure ){
					m_Util.val_to_bin( Sn_1F9A, pShLstVal->val );
				}
				else{
					memset( Sn_1F9A, '\0', sizeof(Sn_1F9A) );
				}
			}
			else if( pShLstVal->itmId == _T("AFG00020") ){
				if( m_isTransitionalMeasure ){
					m_Util.val_to_bin( Sn_1F9B, pShLstVal->val );
				}
				else{
					memset( Sn_1F9B, '\0', sizeof(Sn_1F9B) );
				}
			}
			else if( pShLstVal->itmId == _T("AFG00030") ){
				m_Util.val_to_bin( Sn_1F9C, pShLstVal->val );
			}
			// çáåvç∑à¯ê≈äz
			else if( pShLstVal->itmId == _T("AFH00010") ){
				m_Util.val_to_bin( Sn_1F10T, pShLstVal->val );
			}
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
int CH26SnFhyo10Data::SetData( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}
	ASSERT( pDBNpSub->zvol );
	if( pDBNpSub->zvol == NULL ){
		return -1;
	}

	EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki( pDBNpSub->zvol );
	if( KzHoushiki == ID_ICSSH_MENZEI ){
		return 0;
	}

	CString	filter;
	char	*pHyoID = NULL;
	int		nowVersion = 1;
	if( KzHoushiki == ID_ICSSH_GENSOKU ){
		filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_1_HYOID, nowVersion );
		pHyoID = ICS_SN_FHYO_1_HYOID;
	}
	else if( KzHoushiki == ID_ICSSH_KANNI ){
		filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_4_HYOID, nowVersion );
		pHyoID = ICS_SN_FHYO_4_HYOID;
	}

	if( m_DBSyzAccess.m_pSH_LST_VAL ){
		m_DBSyzAccess.m_pSH_LST_VAL->Requery( filter );
	}
	else{
		if( m_DBSyzAccess.OpenShLstVal(pDBNpSub->m_database, filter) ){
			return -1;
		}
	}

	CSH_LST_VAL	*pShLstVal = m_DBSyzAccess.m_pSH_LST_VAL;
	if( (pShLstVal->MoveFirst()==-1) || (pShLstVal->st==-1) ){	// ÉfÅ[É^ñ≥Çµ
		
		int	nowVersion = 1;
		if( KzHoushiki == ID_ICSSH_GENSOKU ){
			// â€ê≈ïWèÄäz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ACB00010");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F1A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ACB00020");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F1B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ACB00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F1C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// è¡îÔê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ACC00010");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F2A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ACC00020");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F2B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ACC00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F2C );
/*- '14.05.17 -*/
			pShLstVal->sign1 = Sn_1F2sw;//[Y]
/*-------------*/
//			pShLstVal->sign1 = Sn_1F2sw&(~0x111);//[Y]
/*-------------*/
			pShLstVal->Update();

			// çTèúâﬂëÂí≤êÆê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ACD00010");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F3A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ACD00020");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F3B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ACD00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F3C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// çTèúëŒè€édì¸ê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ACE00020");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F4A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ACE00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F4B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ACE00040");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F4C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ACE00070");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F5A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ACE00080");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F5B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ACE00090");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F5C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ë›ì|Ç…åWÇÈê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ACE00120");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F6A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ACE00130");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F6B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ACE00140");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F6C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// çTèúïsë´ê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ACF00010");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F8A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ACF00020");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F8B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ACF00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F8C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ç∑à¯ê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ACG00010");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F9A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ACG00020");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F9B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ACG00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F9C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// çáåvç∑à¯ê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ACH00010");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F10T );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();
		}
		else if( KzHoushiki == ID_ICSSH_KANNI ){

			// â€ê≈ïWèÄäz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AFB00010");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F1A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();
		
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AFB00020");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F1B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AFB00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F1C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// è¡îÔê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AFC00010");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F2A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();
		
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AFC00020");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F2B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AFC00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F2C );
/*- '14.05.17 -*/
			pShLstVal->sign1 = Sn_1F2sw;//[Y]
/*-------------*/
//			pShLstVal->sign1 = (Sn_1F2sw&(~0x111));//[Y]
/*-------------*/
			pShLstVal->Update();

			// ë›ì|âÒé˚Ç…åWÇÈè¡îÔê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AFD00010");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F3A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();
		
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AFD00020");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F3B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AFD00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F3C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// çTèúëŒè€édì¸ê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AFE00020");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F4A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();
		
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AFE00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F4B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AFE00040");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F4C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

//--> '14.10.17 INS START
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AFE00050");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F4T );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();
//<-- '14.10.17 INS END

			// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AFE00070");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F5A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();
		
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AFE00080");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F5B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AFE00090");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F5C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ë›ì|Ç…åWÇÈê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AFE00120");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F6A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();
		
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AFE00130");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F6B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AFE00140");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F6C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// çTèúïsë´ê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AFF00010");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F8A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();
		
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AFF00020");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F8B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AFF00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F8C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ç∑à¯ê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AFG00010");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F9A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();
		
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AFG00020");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F9B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AFG00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F9C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// çáåvç∑à¯ê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AFH00010");
			m_Util.val_to_asci( &pShLstVal->val, Sn_1F10T );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();
		}
	}
	else{
		if( KzHoushiki == ID_ICSSH_GENSOKU ){

//--> '16.01.05 INS START
			CStringArray	updTagAry;
			int	nowVersion = 1;
//<-- '16.01.05 INS END

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

				// â€ê≈ïWèÄäz
				if( pShLstVal->itmId == _T("ACB00010") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F1A );
					pShLstVal->Update();
//--> '16.01.05 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '16.01.05 INS END
				}
				else if( pShLstVal->itmId == _T("ACB00020") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F1B );
					pShLstVal->Update();
//--> '16.01.05 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '16.01.05 INS END
				}
				else if( pShLstVal->itmId == _T("ACB00030") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F1C );
					pShLstVal->Update();
//--> '16.01.05 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '16.01.05 INS END
				}
				// è¡îÔê≈äz
				else if( pShLstVal->itmId == _T("ACC00010") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F2A );
					pShLstVal->Update();
//--> '16.01.05 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '16.01.05 INS END
				}
				else if( pShLstVal->itmId == _T("ACC00020") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F2B );
					pShLstVal->Update();
//--> '16.01.05 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '16.01.05 INS END
				}
				else if( pShLstVal->itmId == _T("ACC00030") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F2C );
/*- '14.05.17 -*/
					pShLstVal->sign1 = Sn_1F2sw;//[Y]
/*-------------*/
//					pShLstVal->sign1 = Sn_1F2sw&(~0x111);//[Y]
/*-------------*/
					pShLstVal->Update();
//--> '16.01.05 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '16.01.05 INS END
				}
				// çTèúâﬂëÂí≤êÆê≈äz
				else if( pShLstVal->itmId == _T("ACD00010") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F3A );
					pShLstVal->Update();
//--> '16.01.05 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '16.01.05 INS END
				}
				else if( pShLstVal->itmId == _T("ACD00020") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F3B );
					pShLstVal->Update();
//--> '16.01.05 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '16.01.05 INS END
				}
				else if( pShLstVal->itmId == _T("ACD00030") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F3C );
					pShLstVal->Update();
//--> '16.01.05 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '16.01.05 INS END
				}
				// çTèúëŒè€édì¸ê≈äz
				else if( pShLstVal->itmId == _T("ACE00020") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F4A );
					pShLstVal->Update();
//--> '16.01.05 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '16.01.05 INS END
				}
				else if( pShLstVal->itmId == _T("ACE00030") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F4B );
					pShLstVal->Update();
//--> '16.01.05 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '16.01.05 INS END
				}
				else if( pShLstVal->itmId == _T("ACE00040") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F4C );
					pShLstVal->Update();
//--> '16.01.05 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '16.01.05 INS END
				}
				// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
				else if( pShLstVal->itmId == _T("ACE00070") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F5A );
					pShLstVal->Update();
//--> '16.01.05 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '16.01.05 INS END
				}
				else if( pShLstVal->itmId == _T("ACE00080") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F5B );
					pShLstVal->Update();
//--> '16.01.05 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '16.01.05 INS END
				}
				else if( pShLstVal->itmId == _T("ACE00090") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F5C );
					pShLstVal->Update();
//--> '16.01.05 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '16.01.05 INS END
				}
				// ë›ì|Ç…åWÇÈê≈äz
				else if( pShLstVal->itmId == _T("ACE00120") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F6A );
					pShLstVal->Update();
//--> '16.01.05 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '16.01.05 INS END
				}
				else if( pShLstVal->itmId == _T("ACE00130") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F6B );
					pShLstVal->Update();
//--> '16.01.05 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '16.01.05 INS END
				}
				else if( pShLstVal->itmId == _T("ACE00140") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F6C );
					pShLstVal->Update();
//--> '16.01.05 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '16.01.05 INS END
				}
				// çTèúïsë´ê≈äz
				else if( pShLstVal->itmId == _T("ACF00010") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F8A );
					pShLstVal->Update();
//--> '16.01.05 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '16.01.05 INS END
				}
				else if( pShLstVal->itmId == _T("ACF00020") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F8B );
					pShLstVal->Update();
//--> '16.01.05 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '16.01.05 INS END
				}
				else if( pShLstVal->itmId == _T("ACF00030") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F8C );
					pShLstVal->Update();
//--> '16.01.05 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '16.01.05 INS END
				}
				// ç∑à¯ê≈äz
				else if( pShLstVal->itmId == _T("ACG00010") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F9A );
					pShLstVal->Update();
//--> '16.01.05 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '16.01.05 INS END
				}
				else if( pShLstVal->itmId == _T("ACG00020") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F9B );
					pShLstVal->Update();
//--> '16.01.05 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '16.01.05 INS END
				}
				else if( pShLstVal->itmId == _T("ACG00030") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F9C );
					pShLstVal->Update();
//--> '16.01.05 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '16.01.05 INS END
				}
				// çáåvç∑à¯ê≈äz
				else if( pShLstVal->itmId == _T("ACH00010") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F10T );
					pShLstVal->Update();
//--> '16.01.05 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '16.01.05 INS END
				}
			}

//--> '16.01.05 INS START
			// í«â¡çXêV
			int	adCnt = 0;
			for( int i=0;; i++ ){
				if( strlen(tagData10[i].tagName) == 0 ){
					break;
				}
				adCnt++;
			}

			// ñ≥Ç¢Ç‡ÇÃÇí«â¡
			int updCnt = (int)updTagAry.GetCount();
			if( adCnt != updCnt ){
				CArray<unsigned char*>	MonyPtrAry;

				MonyPtrAry.Add( Sn_1F1A );
				MonyPtrAry.Add( Sn_1F1B );
				MonyPtrAry.Add( Sn_1F1C );
				MonyPtrAry.Add( Sn_1F2A );
				MonyPtrAry.Add( Sn_1F2B );
				MonyPtrAry.Add( Sn_1F2C );
				MonyPtrAry.Add( Sn_1F3A );
				MonyPtrAry.Add( Sn_1F3B );
				MonyPtrAry.Add( Sn_1F3C );
				MonyPtrAry.Add( Sn_1F4A );
				MonyPtrAry.Add( Sn_1F4B );
				MonyPtrAry.Add( Sn_1F4C );
//				MonyPtrAry.Add( Sn_1F4T );
				MonyPtrAry.Add( Sn_1F5A );
				MonyPtrAry.Add( Sn_1F5B );
				MonyPtrAry.Add( Sn_1F5C );
				MonyPtrAry.Add( Sn_1F6A );
				MonyPtrAry.Add( Sn_1F6B );
				MonyPtrAry.Add( Sn_1F6C );
				MonyPtrAry.Add( Sn_1F8A );
				MonyPtrAry.Add( Sn_1F8B );
				MonyPtrAry.Add( Sn_1F8C );
				MonyPtrAry.Add( Sn_1F9A );
				MonyPtrAry.Add( Sn_1F9B );
				MonyPtrAry.Add( Sn_1F9C );
				MonyPtrAry.Add( Sn_1F10T );


				BOOL findFlg = FALSE;
				for( int i=0;; i++ ){
					if( strlen(tagData10[i].tagName) == 0 ){
						break;
					}

					findFlg = FALSE;
					for( int j=0; j<updCnt; j++ ){
						if( updTagAry[j].CompareNoCase(tagData10[i].tagName) == 0 ){
							findFlg = TRUE;
							break;
						}
					}
					if( findFlg == FALSE ){
						// äYìñÉ^ÉOÇÃí«â¡
						pShLstVal->AddNew();
						pShLstVal->sn_seq = sn_seq;
						pShLstVal->version = nowVersion;
						pShLstVal->hyoId = pHyoID;
						pShLstVal->itmId = tagData10[i].tagName;
						m_Util.val_to_asci( &pShLstVal->val, MonyPtrAry[i] );
						if( tagData10[i].tagName == _T("ACC00030") ){
							pShLstVal->sign1 = Sn_1F2sw;
						}
						else{
							pShLstVal->sign1 = 0;
						}
						pShLstVal->Update();
					}
				}
			}
//<-- '16.01.05 INS END
		}
		else if( KzHoushiki == ID_ICSSH_KANNI ){

//--> '14.10.17 INS START
			CStringArray	updTagAry;
			int	nowVersion = 1;
//<-- '14.10.17 INS END

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

				// â€ê≈ïWèÄäz
				if( pShLstVal->itmId == _T("AFB00010") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F1A );
					pShLstVal->Update();
//--> '14.10.17 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '14.10.17 INS END
				}
				else if( pShLstVal->itmId == _T("AFB00020") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F1B );
					pShLstVal->Update();
//--> '14.10.17 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '14.10.17 INS END
				}
				else if( pShLstVal->itmId == _T("AFB00030") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F1C );
					pShLstVal->Update();
//--> '14.10.17 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '14.10.17 INS END
				}
				// è¡îÔê≈äz
				else if( pShLstVal->itmId == _T("AFC00010") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F2A );
					pShLstVal->Update();
//--> '14.10.17 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '14.10.17 INS END
				}
				else if( pShLstVal->itmId == _T("AFC00020") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F2B );
					pShLstVal->Update();
//--> '14.10.17 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '14.10.17 INS END
				}
				else if( pShLstVal->itmId == _T("AFC00030") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F2C );
/*- '14.05.17 -*/
					pShLstVal->sign1 = Sn_1F2sw;//[Y]
/*-------------*/
//					pShLstVal->sign1 = Sn_1F2sw&(~0x111);//[Y]
/*-------------*/
					pShLstVal->Update();
//--> '14.10.17 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '14.10.17 INS END
				}
				// ë›ì|âÒé˚Ç…åWÇÈè¡îÔê≈äz
				else if( pShLstVal->itmId == _T("AFD00010") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F3A );
					pShLstVal->Update();
//--> '14.10.17 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '14.10.17 INS END
				}
				else if( pShLstVal->itmId == _T("AFD00020") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F3B );
					pShLstVal->Update();
//--> '14.10.17 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '14.10.17 INS END
				}
				else if( pShLstVal->itmId == _T("AFD00030") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F3C );
					pShLstVal->Update();
//--> '14.10.17 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '14.10.17 INS END
				}
				// çTèúëŒè€édì¸ê≈äz
				else if( pShLstVal->itmId == _T("AFE00020") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F4A );
					pShLstVal->Update();
//--> '14.10.17 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '14.10.17 INS END
				}
				else if( pShLstVal->itmId == _T("AFE00030") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F4B );
					pShLstVal->Update();
//--> '14.10.17 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '14.10.17 INS END
				}
				else if( pShLstVal->itmId == _T("AFE00040") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F4C );
					pShLstVal->Update();
//--> '14.10.17 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '14.10.17 INS END
				}
//--> '14.10.17 INS START
				else if( pShLstVal->itmId == _T("AFE00050") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F4T );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
//<-- '14.10.17 INS END
				// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
				else if( pShLstVal->itmId == _T("AFE00070") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F5A );
					pShLstVal->Update();
//--> '14.10.17 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '14.10.17 INS END
				}
				else if( pShLstVal->itmId == _T("AFE00080") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F5B );
					pShLstVal->Update();
//--> '14.10.17 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '14.10.17 INS END
				}
				else if( pShLstVal->itmId == _T("AFE00090") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F5C );
					pShLstVal->Update();
//--> '14.10.17 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '14.10.17 INS END
				}
				// ë›ì|Ç…åWÇÈê≈äz
				else if( pShLstVal->itmId == _T("AFE00120") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F6A );
					pShLstVal->Update();
//--> '14.10.17 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '14.10.17 INS END
				}
				else if( pShLstVal->itmId == _T("AFE00130") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F6B );
					pShLstVal->Update();
//--> '14.10.17 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '14.10.17 INS END
				}
				else if( pShLstVal->itmId == _T("AFE00140") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F6C );
					pShLstVal->Update();
//--> '14.10.17 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '14.10.17 INS END
				}
				// çTèúïsë´ê≈äz
				else if( pShLstVal->itmId == _T("AFF00010") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F8A );
					pShLstVal->Update();
//--> '14.10.17 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '14.10.17 INS END
				}
				else if( pShLstVal->itmId == _T("AFF00020") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F8B );
					pShLstVal->Update();
//--> '14.10.17 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '14.10.17 INS END
				}
				else if( pShLstVal->itmId == _T("AFF00030") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F8C );
					pShLstVal->Update();
//--> '14.10.17 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '14.10.17 INS END
				}
				// ç∑à¯ê≈äz
				else if( pShLstVal->itmId == _T("AFG00010") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F9A );
					pShLstVal->Update();
//--> '14.10.17 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '14.10.17 INS END
				}
				else if( pShLstVal->itmId == _T("AFG00020") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F9B );
					pShLstVal->Update();
//--> '14.10.17 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '14.10.17 INS END
				}
				else if( pShLstVal->itmId == _T("AFG00030") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F9C );
					pShLstVal->Update();
//--> '14.10.17 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '14.10.17 INS END
				}
				// çáåvç∑à¯ê≈äz
				else if( pShLstVal->itmId == _T("AFH00010") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_1F10T );
					pShLstVal->Update();
//--> '14.10.17 INS START
					updTagAry.Add( pShLstVal->itmId );
//<-- '14.10.17 INS END
				}
			}
			
//--> '14.10.17 INS START
			// í«â¡çXêV
			int	adCnt = 0;
			for( int i=0;; i++ ){
				if( strlen(tagData40[i].tagName) == 0 ){
					break;
				}
				adCnt++;
			}

			// ñ≥Ç¢Ç‡ÇÃÇí«â¡
			int updCnt = (int)updTagAry.GetCount();
			if( adCnt != updCnt ){
				CArray<unsigned char*>	MonyPtrAry;

				MonyPtrAry.Add( Sn_1F1A );
				MonyPtrAry.Add( Sn_1F1B );
				MonyPtrAry.Add( Sn_1F1C );
				MonyPtrAry.Add( Sn_1F2A );
				MonyPtrAry.Add( Sn_1F2B );
				MonyPtrAry.Add( Sn_1F2C );
				MonyPtrAry.Add( Sn_1F3A );
				MonyPtrAry.Add( Sn_1F3B );
				MonyPtrAry.Add( Sn_1F3C );
				MonyPtrAry.Add( Sn_1F4A );
				MonyPtrAry.Add( Sn_1F4B );
				MonyPtrAry.Add( Sn_1F4C );
				MonyPtrAry.Add( Sn_1F4T );
				MonyPtrAry.Add( Sn_1F5A );
				MonyPtrAry.Add( Sn_1F5B );
				MonyPtrAry.Add( Sn_1F5C );
				MonyPtrAry.Add( Sn_1F6A );
				MonyPtrAry.Add( Sn_1F6B );
				MonyPtrAry.Add( Sn_1F6C );
				MonyPtrAry.Add( Sn_1F8A );
				MonyPtrAry.Add( Sn_1F8B );
				MonyPtrAry.Add( Sn_1F8C );
				MonyPtrAry.Add( Sn_1F9A );
				MonyPtrAry.Add( Sn_1F9B );
				MonyPtrAry.Add( Sn_1F9C );
				MonyPtrAry.Add( Sn_1F10T );


				BOOL findFlg = FALSE;
				for( int i=0;; i++ ){
					if( strlen(tagData40[i].tagName) == 0 ){
						break;
					}

					findFlg = FALSE;
					for( int j=0; j<updCnt; j++ ){
						if( updTagAry[j].CompareNoCase(tagData40[i].tagName) == 0 ){
							findFlg = TRUE;
							break;
						}
					}
					if( findFlg == FALSE ){
						// äYìñÉ^ÉOÇÃí«â¡
						pShLstVal->AddNew();
						pShLstVal->sn_seq = sn_seq;
						pShLstVal->version = nowVersion;
						pShLstVal->hyoId = pHyoID;
						pShLstVal->itmId = tagData40[i].tagName;
						m_Util.val_to_asci( &pShLstVal->val, MonyPtrAry[i] );
						pShLstVal->sign1 = 0;
						pShLstVal->Update();
					}
				}
			}
//--> '14.10.17 INS START
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// èWåvçœÇ›ÉfÅ[É^ÇÃÉZÉbÉg
//-----------------------------------------------------------------------------
// à¯êî	pDBNpSub	ÅF	
//		pSyzSyukei	ÅF	è¡îÔê≈èWåvÉNÉâÉX
//		isPluralZei	ÅF	ï°êîê≈ó¶
//-----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	ê≥èÌèIóπ
//			-1		ÅF	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CH26SnFhyo10Data::SetCalqedData( CDBNpSub *pDBNpSub, CSyzSyukei *pSyzSyukei, BOOL &isPluralZei )
{
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}
	ASSERT( pDBNpSub->zvol );
	if( pDBNpSub->zvol == NULL ){
		return -1;
	}

	EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki( pDBNpSub->zvol );
	if( KzHoushiki == ID_ICSSH_MENZEI ){
		return 0;
	}
	else if( KzHoushiki == ID_ICSSH_GENSOKU ){
		return SetCalqed10Data( pSyzSyukei, isPluralZei );
	}
	else if( KzHoushiki == ID_ICSSH_KANNI ){
		return SetCalqed40Data( pSyzSyukei, isPluralZei );
	}

	return 0;
}

//-----------------------------------------------------------------------------
// èWåvçœÇ›ÉfÅ[É^ÇÃÉZÉbÉg(ïtï\ÇP)
//-----------------------------------------------------------------------------
// à¯êî	pSyzSyukei	ÅF	è¡îÔê≈èWåvÉNÉâÉX
//		isPluralZei	ÅF	ï°êîê≈ó¶
//-----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	ê≥èÌèIóπ
//			-1		ÅF	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CH26SnFhyo10Data::SetCalqed10Data( CSyzSyukei *pSyzSyukei, BOOL &isPluralZei )
{
	isPluralZei = FALSE;

	MoneyBasejagArray money;

	money = pSyzSyukei->GetShinkokuData( _T("11") );

	int cnt = (int)money.GetCount();
	int incnt = 0;

	unsigned	char	exZeiflg = 0x00;	// Å†Å†Å†Å† Å†Å°Å°Å°
											//  D0ÅF3%
											//  D1ÅF5%
											//  D2ÅF8%

	for( int i=0; i<cnt; i++ ){
		incnt = (int)money[i].GetCount();
		for( int j=0; j<incnt; j++ ){
			if( money[i][j].code.Left(4) == _T("1101") ){
				if( money[i][j].code == _T("110101") ){
					memmove( Sn_1F1A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("110102") ){
					memmove( Sn_1F1B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("110103") ){
					memmove( Sn_1F1C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}
			else if( money[i][j].code.Left(4) == _T("1102") ){
				if( money[i][j].code == _T("110201") ){
					memmove( Sn_1F2A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("110202") ){
					memmove( Sn_1F2B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("110203") ){
					memmove( Sn_1F2C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}
			else if( money[i][j].code.Left(4) == _T("1104") ){
				if( money[i][j].code == _T("110401") ){
					memmove( Sn_1F4A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("110402") ){
					memmove( Sn_1F4B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("110403") ){
					memmove( Sn_1F4C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}
			else if( money[i][j].code.Left(4) == _T("1105") ){
				if( money[i][j].code == _T("110501") ){
					memmove( Sn_1F5A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("110502") ){
					memmove( Sn_1F5B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("110503") ){
					memmove( Sn_1F5C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}
			else if( money[i][j].code.Left(4) == _T("1106") ){
				if( money[i][j].code == _T("110601") ){
					memmove( Sn_1F6A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("110602") ){
					memmove( Sn_1F6B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("110603") ){
					memmove( Sn_1F6C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}
			else if( money[i][j].code.Left(4) == _T("1108") ){
				if( money[i][j].code == _T("110801") ){
					memmove( Sn_1F8A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("110802") ){
					memmove( Sn_1F8B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("110803") ){
					memmove( Sn_1F8C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}
			else if( money[i][j].code.Left(4) == _T("1109") ){
				if( money[i][j].code == _T("110901") ){
					memmove( Sn_1F9A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("110902") ){
					memmove( Sn_1F9B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("110903") ){
					memmove( Sn_1F9C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}
			else if( money[i][j].code.Left(4) == _T("1110") ){
				if( money[i][j].code == _T("111000") ){
					memmove( Sn_1F10T, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
//						exZeiflg |= 0x01;
					}
				}
			}
		}
	}

	if( ((exZeiflg&0x07)==0x01) || ((exZeiflg&0x07)==0x02) || ((exZeiflg&0x07)==0x04) || (exZeiflg==0x00) ){
		isPluralZei = FALSE;
	}
	else{
		isPluralZei = TRUE;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// èWåvçœÇ›ÉfÅ[É^ÇÃÉZÉbÉg(ïtï\ÇS)
//-----------------------------------------------------------------------------
// à¯êî	pSyzSyukei	ÅF	è¡îÔê≈èWåvÉNÉâÉX
//		isPluralZei	ÅF	ï°êîê≈ó¶
//-----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	ê≥èÌèIóπ
//			-1		ÅF	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CH26SnFhyo10Data::SetCalqed40Data( CSyzSyukei *pSyzSyukei, BOOL &isPluralZei )
{
	isPluralZei = FALSE;

	MoneyBasejagArray money;

	money = pSyzSyukei->GetShinkokuData( _T("14") );

	int cnt = (int)money.GetCount();
	int incnt = 0;

	unsigned	char	exZeiflg = 0x00;	// Å†Å†Å†Å† Å†Å°Å°Å°
											//  D0ÅF3%
											//  D1ÅF5%
											//  D2ÅF8%

	for( int i=0; i<cnt; i++ ){
		incnt = (int)money[i].GetCount();
		for( int j=0; j<incnt; j++ ){
			if( money[i][j].code.Left(4) == _T("1401") ){
				if( money[i][j].code == _T("140101") ){
					memmove( Sn_1F1A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("140102") ){
					memmove( Sn_1F1B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("140103") ){
					memmove( Sn_1F1C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}
			else if( money[i][j].code.Left(4) == _T("1402") ){
				if( money[i][j].code == _T("140201") ){
					memmove( Sn_1F2A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("140202") ){
					memmove( Sn_1F2B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("140203") ){
					memmove( Sn_1F2C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}
			else if( money[i][j].code.Left(4) == _T("1403") ){
				if( money[i][j].code == _T("140301") ){
					memmove( Sn_1F3A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("140302") ){
					memmove( Sn_1F3B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("140303") ){
					memmove( Sn_1F3C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}
			else if( money[i][j].code.Left(4) == _T("1404") ){
				if( money[i][j].code == _T("140401") ){
					memmove( Sn_1F4A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("140402") ){
					memmove( Sn_1F4B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("140403") ){
					memmove( Sn_1F4C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
//--> '14.10.17 INS START
				else if( money[i][j].code == _T("140400") ){
					memmove( Sn_1F4T, money[i][j].arith, MONY_BUF_SIZE );
				}
//<-- '14.10.17 INS END
			}
			else if( money[i][j].code.Left(4) == _T("1405") ){
				if( money[i][j].code == _T("140501") ){
					memmove( Sn_1F5A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("140502") ){
					memmove( Sn_1F5B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("140503") ){
					memmove( Sn_1F5C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}
			else if( money[i][j].code.Left(4) == _T("1406") ){
				if( money[i][j].code == _T("140601") ){
					memmove( Sn_1F6A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("140602") ){
					memmove( Sn_1F6B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("140603") ){
					memmove( Sn_1F6C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}
			else if( money[i][j].code.Left(4) == _T("1408") ){
				if( money[i][j].code == _T("140801") ){
					memmove( Sn_1F8A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("140802") ){
					memmove( Sn_1F8B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("140803") ){
					memmove( Sn_1F8C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}
			else if( money[i][j].code.Left(4) == _T("1409") ){
				if( money[i][j].code == _T("140901") ){
					memmove( Sn_1F9A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("140902") ){
					memmove( Sn_1F9B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("140903") ){
					memmove( Sn_1F9C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}
			else if( money[i][j].code.Left(4) == _T("1410") ){
				if( money[i][j].code == _T("141000") ){
					memmove( Sn_1F10T, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
//						exZeiflg |= 0x01;
					}
				}
			}
		}
	}

	if( ((exZeiflg&0x07)==0x01) || ((exZeiflg&0x07)==0x02) || ((exZeiflg&0x07)==0x04) || (exZeiflg==0x00) ){
		isPluralZei = FALSE;
	}
	else{
		isPluralZei = TRUE;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// åªçsÉfÅ[É^ç\ë¢Ç©ÇÁÇÃÉfÅ[É^ÇÃà¯åpÇ¨
//-----------------------------------------------------------------------------
// à¯êî	pDBNpSub	ÅF	ç‡ñ±ÉNÉâÉX
//		sn_seq		ÅF	óöóÉVÅ[ÉPÉìÉX
//-----------------------------------------------------------------------------
int CH26SnFhyo10Data::GetPastData( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT( pDBNpSub );

	CString	filter;
	filter.Format( _T("Sn_SEQ = %d"), sn_seq );
	if( pDBNpSub->szsnk ){
		pDBNpSub->szsnk->Requery( filter );
	}
	else{
		if( pDBNpSub->SzsnkOpen(filter) ){
			return -1;
		}
	}
	CSZSNK *pSZSNK = pDBNpSub->szsnk;
	if( pSZSNK->st == -1 ){
		// ñ≥Ç¢ÅH
	}

	EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki( pDBNpSub->zvol );
	if( KzHoushiki == ID_ICSSH_MENZEI ){
		return 0;
	}

	int	nowVersion = 1;
	if( KzHoushiki == ID_ICSSH_GENSOKU ){
		filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_1_HYOID, nowVersion );
	}
	else if( KzHoushiki == ID_ICSSH_KANNI ){
		filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_4_HYOID, nowVersion );
	}

	if( m_DBSyzAccess.m_pSH_LST_VAL ){
		m_DBSyzAccess.m_pSH_LST_VAL->Requery( filter );
	}
	else{
		if( m_DBSyzAccess.OpenShLstVal(pDBNpSub->m_database, filter) ){
			return -1;
		}
	}

	CSH_LST_VAL	*pShLstVal = m_DBSyzAccess.m_pSH_LST_VAL;

	if( KzHoushiki == ID_ICSSH_GENSOKU ){
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

			// â€ê≈ïWèÄäz
			if( pShLstVal->itmId == _T("ACB00010") ){
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_1F2A;
				pShLstVal->Update();
			}
			else if( pShLstVal->itmId == _T("ACB00020") ){
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_1F1B;
				pShLstVal->Update();
			}
			// è¡îÔê≈äz
			else if( pShLstVal->itmId == _T("ACC00010") ){
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_1F5A;
				pShLstVal->Update();
			}
			else if( pShLstVal->itmId == _T("ACC00020") ){
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_1F4B;
				pShLstVal->Update();
			}
			// çTèúâﬂëÂí≤êÆê≈äz
			else if( pShLstVal->itmId == _T("ACD00010") ){
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_1F7A;
				pShLstVal->Update();
			}
			else if( pShLstVal->itmId == _T("ACD00020") ){
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_1F7B;
				pShLstVal->Update();
			}
			// çTèúëŒè€édì¸ê≈äz
			else if( pShLstVal->itmId == _T("ACE00020") ){
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_1F8A;
				pShLstVal->Update();
			}
			else if( pShLstVal->itmId == _T("ACE00030") ){
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_1F8B;
				pShLstVal->Update();
			}
//			else if( pShLstVal->itmId == _T("ACE00040") ){
//				pShLstVal->Edit();
//				m_Util.val_to_asci( &pShLstVal->val, Sn_1F4C );
//				pShLstVal->Update();
//			}
			// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
			else if( pShLstVal->itmId == _T("ACE00070") ){
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_1F9A;
				pShLstVal->Update();
			}
			else if( pShLstVal->itmId == _T("ACE00080") ){
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_1F9B;
				pShLstVal->Update();
			}
//			else if( pShLstVal->itmId == _T("ACE00090") ){
//				pShLstVal->Edit();
//				m_Util.val_to_asci( &pShLstVal->val, Sn_1F5C );
//				pShLstVal->Update();
//			}
			// ë›ì|Ç…åWÇÈê≈äz
			else if( pShLstVal->itmId == _T("ACE00120") ){
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_1F10A;
				pShLstVal->Update();
			}
			else if( pShLstVal->itmId == _T("ACE00130") ){
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_1F10B;
				pShLstVal->Update();
			}
			// çTèúïsë´ê≈äz
//			else if( pShLstVal->itmId == _T("ACF00010") ){
//				pShLstVal->Edit();
//				pShLstVal->val = Sn_1F8A;
//				pShLstVal->Update();
//			}
			else if( pShLstVal->itmId == _T("ACF00020") ){
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_1F12B;
				pShLstVal->Update();
			}
//			else if( pShLstVal->itmId == _T("ACF00030") ){
//				pShLstVal->Edit();
//				pShLstVal->val = Sn_1F8C;
//				pShLstVal->Update();
//			}
			// ç∑à¯ê≈äz
//			else if( pShLstVal->itmId == _T("ACG00010") ){
//				pShLstVal->Edit();
//				pShLstVal->val = Sn_1F9A;
//				pShLstVal->Update();
//			}
			else if( pShLstVal->itmId == _T("ACG00020") ){
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_1F13B;
				pShLstVal->Update();
			}
//			else if( pShLstVal->itmId == _T("ACG00030") ){
//				pShLstVal->Edit();
//				pShLstVal->val = Sn_1F9C;
//				pShLstVal->Update();
//			}
			// çáåvç∑à¯ê≈äz
			else if( pShLstVal->itmId == _T("ACH00010") ){
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_1F14C;
				pShLstVal->Update();
			}
		}
	}
	else if( KzHoushiki == ID_ICSSH_KANNI ){
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

			// â€ê≈ïWèÄäz
			if( pShLstVal->itmId == _T("AFB00010") ){
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_1F2A;
				pShLstVal->Update();
			}
			else if( pShLstVal->itmId == _T("AFB00020") ){
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_1F1B;
				pShLstVal->Update();
			}
			// è¡îÔê≈äz
			else if( pShLstVal->itmId == _T("AFC00010") ){
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_1F5A;
				pShLstVal->Update();
			}
			else if( pShLstVal->itmId == _T("AFC00020") ){
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_1F4B;
				pShLstVal->Update();
			}
			// ë›ì|âÒé˚Ç…åWÇÈè¡îÔê≈äz
			else if( pShLstVal->itmId == _T("AFD00010") ){
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_1F7A;
				pShLstVal->Update();
			}
			else if( pShLstVal->itmId == _T("AFD0000") ){
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_1F7B;
				pShLstVal->Update();
			}
			// çTèúëŒè€édì¸ê≈äz
			else if( pShLstVal->itmId == _T("AFE00020") ){
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_1F8A;
				pShLstVal->Update();
			}
			else if( pShLstVal->itmId == _T("AFE00030") ){
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_1F8B;
				pShLstVal->Update();
			}
//			else if( pShLstVal->itmId == _T("AFE00040") ){
//				pShLstVal->Edit();
//				m_Util.val_to_asci( &pShLstVal->val, Sn_1F4C );
//				pShLstVal->Update();
//			}
			// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
			else if( pShLstVal->itmId == _T("AFE00070") ){
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_1F9A;
				pShLstVal->Update();
			}
			else if( pShLstVal->itmId == _T("AFE00080") ){
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_1F9B;
				pShLstVal->Update();
			}
//			else if( pShLstVal->itmId == _T("AFE00090") ){
//				pShLstVal->Edit();
//				m_Util.val_to_asci( &pShLstVal->val, Sn_1F5C );
//				pShLstVal->Update();
//			}
			// ë›ì|Ç…åWÇÈê≈äz
			else if( pShLstVal->itmId == _T("AFE00120") ){
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_1F10A;
				pShLstVal->Update();
			}
			else if( pShLstVal->itmId == _T("AFE00130") ){
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_1F10B;
				pShLstVal->Update();
			}
			// çTèúïsë´ê≈äz
//			else if( pShLstVal->itmId == _T("AFF00010") ){
//				pShLstVal->Edit();
//				pShLstVal->val = Sn_1F8A;
//				pShLstVal->Update();
//			}
			else if( pShLstVal->itmId == _T("AFF00020") ){
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_1F12B;
				pShLstVal->Update();
			}
//			else if( pShLstVal->itmId == _T("AFF00030") ){
//				pShLstVal->Edit();
//				pShLstVal->val = Sn_1F8C;
//				pShLstVal->Update();
//			}
			// ç∑à¯ê≈äz
//			else if( pShLstVal->itmId == _T("AFG00010") ){
//				pShLstVal->Edit();
//				pShLstVal->val = Sn_1F9A;
//				pShLstVal->Update();
//			}
			else if( pShLstVal->itmId == _T("AFG00020") ){
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_1F13B;
				pShLstVal->Update();
			}
//			else if( pShLstVal->itmId == _T("AFG00030") ){
//				pShLstVal->Edit();
//				pShLstVal->val = Sn_1F9C;
//				pShLstVal->Update();
//			}
			// çáåvç∑à¯ê≈äz
			else if( pShLstVal->itmId == _T("AFH00010") ){
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_1F14C;
				pShLstVal->Update();
			}
		}
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
int CH26SnFhyo10Data::ClearDataByRenew( CDBNpSub *pDBNpSub, int type )
{
	m_Arith.l_clear( Sn_1F1A );
	m_Arith.l_clear( Sn_1F1B );
	m_Arith.l_clear( Sn_1F1C );

	m_Arith.l_clear( Sn_1F2A );
	m_Arith.l_clear( Sn_1F2B );
	m_Arith.l_clear( Sn_1F2C );

	m_Arith.l_clear( Sn_1F3A );
	m_Arith.l_clear( Sn_1F3B );
	m_Arith.l_clear( Sn_1F3C );

	m_Arith.l_clear( Sn_1F4A );
	m_Arith.l_clear( Sn_1F4B );
	m_Arith.l_clear( Sn_1F4C );
//--> '14.10.17 INS START
	m_Arith.l_clear( Sn_1F4T );
//--> '14.10.17 INS END

	m_Arith.l_clear( Sn_1F5A );
	m_Arith.l_clear( Sn_1F5B );
	m_Arith.l_clear( Sn_1F5C );

	m_Arith.l_clear( Sn_1F6A );
	m_Arith.l_clear( Sn_1F6B );
	m_Arith.l_clear( Sn_1F6C );

	m_Arith.l_clear( Sn_1F8A );
	m_Arith.l_clear( Sn_1F8B );
	m_Arith.l_clear( Sn_1F8C );

	m_Arith.l_clear( Sn_1F9A );
	m_Arith.l_clear( Sn_1F9B );
	m_Arith.l_clear( Sn_1F9C );

	m_Arith.l_clear( Sn_1F9A );
	m_Arith.l_clear( Sn_1F9B );
	m_Arith.l_clear( Sn_1F9C );

	m_Arith.l_clear( Sn_1F10T );

	return 0;
}

//-----------------------------------------------------------------------------
// éËì¸óÕÉTÉCÉìÇÃÉNÉäÉA
//-----------------------------------------------------------------------------
// ï‘ëóíl	0	ÅF	ê≥èÌèIóπ
//-----------------------------------------------------------------------------
int CH26SnFhyo10Data::ClearManualInputSign( BOOL isChgKzritu )
{
	Sn_1F2sw = 0;
	if( isChgKzritu ){
		Sn_1F2sw |= 0x111;
	}

	return 0;
}

//----------------------------------------------------------------------------
// â°åvÇ≈ÇÕÇ»Ç¢ó·äOåvéZÇ™ïKóvÅH ('14.10.17)
//----------------------------------------------------------------------------
// à¯êî	pDBNpSub	ÅF	ç‡ñ±ÉNÉâÉXÇÃÉ|ÉCÉìÉ^
//----------------------------------------------------------------------------
// ï‘ëóíl	TRUE	ÅF	ïKóv
//			FALSE	ÅF	ïsóv
//----------------------------------------------------------------------------
BOOL CH26SnFhyo10Data::IsNeedExpCalq( CDBNpSub *pDBNpSub )
{
	BOOL	bRt = FALSE;

	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return bRt;
	}
	ASSERT( pDBNpSub->zvol );
	if( pDBNpSub->zvol == NULL ){
		return bRt;
	}

	EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki( pDBNpSub->zvol );
	if( KzHoushiki == ID_ICSSH_KANNI ){
		// ë›ì|âÒé˚
		if( m_Arith.l_test(Sn_1F3A) ){
			bRt = TRUE;
		}
		else if( m_Arith.l_test(Sn_1F3B) ){
			bRt = TRUE;
		}
		else if( m_Arith.l_test(Sn_1F3C) ){
			bRt = TRUE;
		}
	}

	return bRt;
}

//-----------------------------------------------------------------------------
// çXêVéûÇÃã‡äzí«â¡èëÇ´çûÇ›('17.12.08)
//-----------------------------------------------------------------------------
// à¯êî	pDBNpSub	ÅF	ç‡ñ±ÉNÉâÉXÇÃÉ|ÉCÉìÉ^
//		sn_seq		ÅF	ëŒè€ÇÃóöóÉVÅ[ÉPÉìÉX
//-----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	ê≥èÌèIóπ
//-----------------------------------------------------------------------------
int CH26SnFhyo10Data::SetDataInRenew( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}
	ASSERT( pDBNpSub->zvol );
	if( pDBNpSub->zvol == NULL ){
		return -1;
	}

	EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki( pDBNpSub->zvol );
	if( (KzHoushiki!=ID_ICSSH_GENSOKU) && (KzHoushiki!=ID_ICSSH_KANNI) ){
		return 0;
	}

	// é©ï™é©êgÇ∆à·Ç§â€ê≈ï˚éÆÇÃã‡äzÇ…èëÇ´çûÇ›
	CString	filter;
	int		nowVersion = 1;
	char	*pHyoID = NULL;
	if( KzHoushiki == ID_ICSSH_GENSOKU ){
		filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_4_HYOID, nowVersion );
		pHyoID = ICS_SN_KANNI_HONPYO_HYOID;
	}
	else if( KzHoushiki == ID_ICSSH_KANNI ){
		filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_1_HYOID, nowVersion );
		pHyoID = ICS_SN_IPPAN_HONPYO_HYOID;
	}

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
		pShLstVal->Edit();
		pShLstVal->val = _T("0");
		pShLstVal->sign1 = 0;
		pShLstVal->Update();
	}

	return 0;
}
