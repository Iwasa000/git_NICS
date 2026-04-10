#include "stdafx.h"
#include "H31SnHonpyo2Data.h"

// å¥ë•â€ê≈
static SH_TAGNAME tagDataIppan2[] = {
	{ _T( "AAP00000" ), 999 },
	{ _T( "AAQ00010" ), 999 },
	{ _T( "AAQ00020" ), 999 },
	{ _T( "AAQ00030" ), 999 },
	{ _T( "AAQ00040" ), 999 },
	{ _T( "AAQ00050" ), 999 },
	{ _T( "AAQ00060" ), 999 },
	{ _T( "AAR00010" ), 999 },
	{ _T( "AAR00020" ), 999 },
	{ _T( "AAR00030" ), 999 },
	{ _T( "AAS00000" ), 999 },
	{ _T( "AAT00010" ), 999 },
	{ _T( "AAT00020" ), 999 },
	{ _T( "AAT00030" ), 999 },
	{ _T( "AAT00040" ), 999 },
	{ _T( "AAT00050" ), 999 },
	{ _T( "AAU00000" ), 999 },
	{ _T( "AAV00010" ), 999 },
	{ _T( "AAV00020" ), 999 },
	{ _T( "AAW00010" ), 999 },
	{ _T( "AAW00020" ), 999 },
	{ _T( "AAW00030" ), 999 },
	{ _T( "AAW00040" ), 999 },

	{ _T( "" ) }
};

// ä»à’â€ê≈
static SH_TAGNAME tagDataKani2[] = {
	{ _T( "ABO00000" ), 999 },
	{ _T( "ABP00010" ), 999 },
	{ _T( "ABP00020" ), 999 },
	{ _T( "ABP00030" ), 999 },
	{ _T( "ABP00040" ), 999 },
	{ _T( "ABP00050" ), 999 },
	{ _T( "ABP00060" ), 999 },
	{ _T( "ABR00000" ), 999 },
	{ _T( "ABS00010" ), 999 },
	{ _T( "ABS00020" ), 999 },
	{ _T( "ABS00030" ), 999 },
	{ _T( "ABS00040" ), 999 },
	{ _T( "ABS00050" ), 999 },
	{ _T( "ABT00000" ), 999 },
	{ _T( "ABU00010" ), 999 },
	{ _T( "ABV00010" ), 999 },
	{ _T( "ABV00020" ), 999 },
	{ _T( "ABV00030" ), 999 },
	{ _T( "ABV00040" ), 999 },

	{ _T( "" ) }
};

CH31SnHonpyo2Data::CH31SnHonpyo2Data()
{
	memset ( Sn_XM1, '\0', MONY_BUF_SIZE );
	memset ( Sn_XM2, '\0', MONY_BUF_SIZE );
	memset ( Sn_XM3, '\0', MONY_BUF_SIZE );
	memset ( Sn_XM4, '\0', MONY_BUF_SIZE );
	memset ( Sn_XM5, '\0', MONY_BUF_SIZE );
	memset ( Sn_XM6, '\0', MONY_BUF_SIZE );
	memset ( Sn_XM7, '\0', MONY_BUF_SIZE );
	memset ( Sn_XM8, '\0', MONY_BUF_SIZE );
	memset ( Sn_XM9, '\0', MONY_BUF_SIZE );
	memset ( Sn_XM10, '\0', MONY_BUF_SIZE );
	memset ( Sn_XM11, '\0', MONY_BUF_SIZE );
	memset ( Sn_XM12, '\0', MONY_BUF_SIZE );
	memset ( Sn_XM13, '\0', MONY_BUF_SIZE );
	memset ( Sn_XM14, '\0', MONY_BUF_SIZE );
	memset ( Sn_XM15, '\0', MONY_BUF_SIZE );
	memset ( Sn_XM16, '\0', MONY_BUF_SIZE );
	memset ( Sn_XM17, '\0', MONY_BUF_SIZE );
	memset ( Sn_XM18, '\0', MONY_BUF_SIZE );
	memset ( Sn_XM19, '\0', MONY_BUF_SIZE );
	memset ( Sn_XM20, '\0', MONY_BUF_SIZE );
	memset ( Sn_XM21, '\0', MONY_BUF_SIZE );
	memset ( Sn_XM22, '\0', MONY_BUF_SIZE );
	memset ( Sn_XM23, '\0', MONY_BUF_SIZE );

	rsv = NULL;
}

CH31SnHonpyo2Data::~CH31SnHonpyo2Data()
{
	if ( m_DBSyzAccess.m_pSH_LST_VAL ){
		m_DBSyzAccess.CloseShLstVal();
	}
}


//======================================================
// DBÇ©ÇÁÉfÅ[É^ì«çûÇ›
//------------------------------------------------------
// à¯êî	
//		pDBNpSub	ÅF	ç‡ñ±ÉNÉâÉXÇÃÉ|ÉCÉìÉ^
//		sn_seq		ÅF	ëŒè€ÇÃóöóÉVÅ[ÉPÉìÉX
//------------------------------------------------------
// ï‘ëóíl
//			0		ÅF	ê≥èÌèIóπ
//			-1		ÅF	ÉGÉâÅ[
//======================================================
int CH31SnHonpyo2Data::GetData ( CDBNpSub *pDBNpSub, int sn_seq )
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
	if ( KzHoushiki == ID_ICSSH_GENSOKU ) {
		filter.Format ( _T( "sn_seq = %d and hyoId = \'%s\' and version = %d" ), sn_seq, ICS_SN_IPPAN_HONPYO2_HYOID, nowVersion );
	}
	else if ( KzHoushiki == ID_ICSSH_KANNI ) {
		filter.Format ( _T( "sn_seq = %d and hyoId = \'%s\' and version = %d" ), sn_seq, ICS_SN_KANNI_HONPYO2_HYOID, nowVersion );
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

	if ( KzHoushiki == ID_ICSSH_GENSOKU ) {	// å¥ë•â€ê≈
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

			if ( pShLstVal->itmId == _T( "AAP00000" ) ) {		// â€ê≈ïWèÄäz
				m_Util.val_to_bin ( Sn_XM1, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "AAQ00010" ) ) {	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz 3%ìKópï™
				m_Util.val_to_bin ( Sn_XM2, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "AAQ00020" ) ) {	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz 4%ìKópï™
				m_Util.val_to_bin ( Sn_XM3, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "AAQ00030" ) ) {	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz 6.3ìKópï™
				m_Util.val_to_bin ( Sn_XM4, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "AAQ00040" ) ) {	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz 6.24%ìKópï™
				m_Util.val_to_bin ( Sn_XM5, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "AAQ00050" ) ) {	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz 7.8%ìKópï™
				m_Util.val_to_bin ( Sn_XM6, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "AAQ00060" ) ) {	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
				m_Util.val_to_bin ( Sn_XM7, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "AAR00010" ) ) {	// ì¡íËâ€ê≈édì¸ÇÍÇ…åWÇÈéxï•Ç¢ëŒâøÇÃäz 6.3ìKópï™
				m_Util.val_to_bin ( Sn_XM8, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "AAR00020" ) ) {	// ì¡íËâ€ê≈édì¸ÇÍÇ…åWÇÈéxï•Ç¢ëŒâøÇÃäz 7.8%ìKópï™
				m_Util.val_to_bin ( Sn_XM9, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "AAR00030" ) ) {	// ì¡íËâ€ê≈édì¸ÇÍÇ…åWÇÈéxï•Ç¢ëŒâøÇÃäz
				m_Util.val_to_bin ( Sn_XM10, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "AAS00000" ) ) {	// è¡îÔê≈äz
				m_Util.val_to_bin ( Sn_XM11, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "AAT00010" ) ) {	// 11ÇÃì‡ñÛ 3%ìKópï™
				m_Util.val_to_bin ( Sn_XM12, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "AAT00020" ) ) {	// 11ÇÃì‡ñÛ 4%ìKópï™
				m_Util.val_to_bin ( Sn_XM13, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "AAT00030" ) ) {	// 11ÇÃì‡ñÛ 6.3%ìKópï™
				m_Util.val_to_bin ( Sn_XM14, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "AAT00040" ) ) {	// 11ÇÃì‡ñÛ 6.24%ìKópï™
				m_Util.val_to_bin ( Sn_XM15, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "AAT00050" ) ) {	// 11ÇÃì‡ñÛ 7.8%ìKópï™
				m_Util.val_to_bin ( Sn_XM16, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "AAU00000" ) ) {	// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
				m_Util.val_to_bin ( Sn_XM17, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "AAV00010" ) ) {	// îÑè„Ç∞ÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
				m_Util.val_to_bin ( Sn_XM18, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "AAV00020" ) ) {	// ì¡íËâ€ê≈édì¸ÇÍÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
				m_Util.val_to_bin ( Sn_XM19, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "AAW00010" ) ) {	// ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
				m_Util.val_to_bin ( Sn_XM20, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "AAW00020" ) ) {	// ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz 4%ìKópï™
				m_Util.val_to_bin ( Sn_XM21, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "AAW00030" ) ) {	// ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz 6.3%ìKópï™
				m_Util.val_to_bin ( Sn_XM22, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "AAW00040" ) ) {	// ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz 6.24%ãyÇ—7.8%ìKópï™
				m_Util.val_to_bin ( Sn_XM23, pShLstVal->val );
			}
		}
	}
	else if ( KzHoushiki == ID_ICSSH_KANNI ) {	// ä»à’â€ê≈
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

			if ( pShLstVal->itmId == _T( "ABO00000" ) ) {		// â€ê≈ïWèÄäz
				m_Util.val_to_bin ( Sn_XM1, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "ABP00010" ) ) {	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz 3%ìKópï™
				m_Util.val_to_bin ( Sn_XM2, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "ABP00020" ) ) {	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz 4%ìKópï™
				m_Util.val_to_bin ( Sn_XM3, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "ABP00030" ) ) {	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz 6.3ìKópï™
				m_Util.val_to_bin ( Sn_XM4, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "ABP00040" ) ) {	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz 6.24%ìKópï™
				m_Util.val_to_bin ( Sn_XM5, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "ABP00050" ) ) {	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz 7.8%ìKópï™
				m_Util.val_to_bin ( Sn_XM6, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "ABP00060" ) ) {	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
				m_Util.val_to_bin ( Sn_XM7, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "ABR00000" ) ) {	// è¡îÔê≈äz
				m_Util.val_to_bin ( Sn_XM11, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "ABS00010" ) ) {	// 11ÇÃì‡ñÛ 3%ìKópï™
				m_Util.val_to_bin ( Sn_XM12, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "ABS00020" ) ) {	// 11ÇÃì‡ñÛ 4%ìKópï™
				m_Util.val_to_bin ( Sn_XM13, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "ABS00030" ) ) {	// 11ÇÃì‡ñÛ 6.3%ìKópï™
				m_Util.val_to_bin ( Sn_XM14, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "ABS00040" ) ) {	// 11ÇÃì‡ñÛ 6.24%ìKópï™
				m_Util.val_to_bin ( Sn_XM15, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "ABS00050" ) ) {	// 11ÇÃì‡ñÛ 7.8%ìKópï™
				m_Util.val_to_bin ( Sn_XM16, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "ABT00000" ) ) {	// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
				m_Util.val_to_bin ( Sn_XM17, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "ABU00010" ) ) {	// îÑè„ÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
				m_Util.val_to_bin ( Sn_XM18, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "ABV00010" ) ) {	// ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
				m_Util.val_to_bin ( Sn_XM20, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "ABV00020" ) ) {	// ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz 4%ìKópï™
				m_Util.val_to_bin ( Sn_XM21, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "ABV00030" ) ) {	// ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz 6.3%ìKópï™
				m_Util.val_to_bin ( Sn_XM22, pShLstVal->val );
			}
			else if ( pShLstVal->itmId == _T( "ABV00040" ) ) {	// ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz 6.24%ãyÇ—7.8%ìKópï™
				m_Util.val_to_bin ( Sn_XM23, pShLstVal->val );
			}
		}
	}

	return 0;
}

//======================================================
// DBÇ÷ÉfÅ[É^ì«çûÇ›
//------------------------------------------------------
// à¯êî	
//		pDBNpSub	ÅF	ç‡ñ±ÉNÉâÉXÇÃÉ|ÉCÉìÉ^
//		sn_seq		ÅF	ëŒè€ÇÃóöóÉVÅ[ÉPÉìÉX
//------------------------------------------------------
// ï‘ëóíl
//			0		ÅF	ê≥èÌèIóπ
//			-1		ÅF	ÉGÉâÅ[
//======================================================
int CH31SnHonpyo2Data::SetData ( CDBNpSub *pDBNpSub, int sn_seq )
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
	if ( KzHoushiki == ID_ICSSH_GENSOKU ) {
		filter.Format ( _T( "sn_seq = %d and hyoId = \'%s\' and version = %d" ), sn_seq, ICS_SN_IPPAN_HONPYO2_HYOID, nowVersion );
		pHyoID = ICS_SN_IPPAN_HONPYO2_HYOID;
	}
	else if ( KzHoushiki == ID_ICSSH_KANNI ) {
		filter.Format ( _T( "sn_seq = %d and hyoId = \'%s\' and version = %d" ), sn_seq, ICS_SN_KANNI_HONPYO2_HYOID, nowVersion );
		pHyoID = ICS_SN_KANNI_HONPYO2_HYOID;
	}

	if ( m_DBSyzAccess.m_pSH_LST_VAL ) {
		m_DBSyzAccess.m_pSH_LST_VAL->Requery ( filter );
	}
	else {
		if ( m_DBSyzAccess.OpenShLstVal( pDBNpSub->m_database, filter ) ) {
			return -1;
		}
	}

	CSH_LST_VAL *pShLstVal = m_DBSyzAccess.m_pSH_LST_VAL;

	if ( ( pShLstVal->MoveFirst() == -1 ) || ( pShLstVal->st == -1 ) ) {
		if ( KzHoushiki == ID_ICSSH_GENSOKU ) {		// å¥ë•â€ê≈
			// â€ê≈ïWèÄäz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "AAP00000" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM1 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz 3%ìKópï™
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "AAQ00010" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM2 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz 4%ìKópï™
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "AAQ00020" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM3 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz 6.3%ìKópï™
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "AAQ00030" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM4 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz 6.24%ìKópï™
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "AAQ00040" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM5 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz 7.8%ìKópï™
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "AAQ00050" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM6 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "AAQ00060" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM7 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ì¡íËâ€ê≈édì¸ÇÍÇ…åWÇÈéxï•Ç¢ëŒâøÇÃäz 6.3%ìKópï™
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "AAR00010" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM8 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ì¡íËâ€ê≈édì¸ÇÍÇ…åWÇÈéxï•Ç¢ëŒâøÇÃäz 7.8%ìKópï™
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "AAR00020" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM9 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ì¡íËâ€ê≈édì¸ÇÍÇ…åWÇÈéxï•Ç¢ëŒâøÇÃäz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "AAR00030" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM10 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// è¡îÔê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "AAS00000" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM11 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 11ÇÃì‡ñÛ 3%ìKópï™
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "AAT00010" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM12 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 11ÇÃì‡ñÛ 4%ìKópï™
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "AAT00020" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM13 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 11ÇÃì‡ñÛ 6.3%ìKópï™
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "AAT00030" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM14 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 11ÇÃì‡ñÛ 6.24%ìKópï™
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "AAT00040" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM15 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 11ÇÃì‡ñÛ 7.8%ìKópï™
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "AAT00050" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM16 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "AAU00000" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM17 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// îÑè„Ç∞ÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "AAV00010" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM18 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ì¡íËâ€ê≈édì¸ÇÍÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "AAV00020" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM19 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "AAW00010" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM20 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz 4%ìKópï™
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "AAW00020" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM21 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz 6.3%ìKópï™
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "AAW00030" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM22 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz 6.24%ãyÇ—7.8%ìKópï™
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "AAW00040" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM23 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();
		}
		else if ( KzHoushiki == ID_ICSSH_KANNI ) {	// ä»à’â€ê≈
			// â€ê≈ïWèÄäz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "ABO00000" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM1 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz 3%ìKópï™
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "ABP00010" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM2 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz 4%ìKópï™
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "ABP00020" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM3 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz 6.3%ìKópï™
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "ABP00030" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM4 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz 6.24%ìKópï™
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "ABP00040" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM5 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz 7.8%ìKópï™
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "ABP00050" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM6 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "ABP00060" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM7 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// è¡îÔê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "ABR00000" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM11 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 11ÇÃì‡ñÛ 3%ìKópï™
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "ABS00010" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM12 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 11ÇÃì‡ñÛ 4%ìKópï™
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "ABS00020" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM13 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 11ÇÃì‡ñÛ 6.3%ìKópï™
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "ABS00030" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM14 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 11ÇÃì‡ñÛ 6.24%ìKópï™
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "ABS00040" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM15 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 11ÇÃì‡ñÛ 7.8%ìKópï™
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "ABS00050" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM16 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "ABT00000" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM17 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// îÑè„ÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "ABU00010" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM18 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "ABV00010" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM20 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz 4%ìKópï™
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "ABV00020" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM21 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz 6.3%ìKópï™
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "ABV00030" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM22 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz 6.24%ãyÇ—7.8%ìKópï™
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "ABV00040" );
			m_Util.val_to_asci ( &pShLstVal->val, Sn_XM23 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();
		}
	}
	else {
		CStringArray updTagAry;
		if ( KzHoushiki == ID_ICSSH_GENSOKU ) {	// å¥ë•â€ê≈
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

				if ( pShLstVal->itmId == _T( "AAP00000" ) ) {		// â€ê≈ïWèÄäz
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM1 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "AAQ00010" ) ) {	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz 3%ìKópï™
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM2 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "AAQ00020" ) ) {	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz 4%ìKópï™
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM3 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "AAQ00030" ) ) {	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz 6.3%ìKópï™
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM4 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "AAQ00040" ) ) {	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz 6.24%ìKópï™
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM5 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "AAQ00050" ) ) {	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz 7.8%ìKópï™
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM6 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "AAQ00060" ) ) {	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM7 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "AAR00010" ) ) {	// ì¡íËâ€ê≈édì¸ÇÍÇ…åWÇÈéxï•Ç¢ëŒâøÇÃäz 6.3%ìKópï™
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM8 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "AAR00020" ) ) {	// ì¡íËâ€ê≈édì¸ÇÍÇ…åWÇÈéxï•Ç¢ëŒâøÇÃäz 7.8%ìKópï™
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM9 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "AAR00030" ) ) {	// ì¡íËâ€ê≈édì¸ÇÍÇ…åWÇÈéxï•Ç¢ëŒâøÇÃäz
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM10 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "AAS00000" ) ) {	// è¡îÔê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM11 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "AAT00010" ) ) {	// 11ÇÃì‡ñÛ 3%ìKópï™
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM12 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "AAT00020" ) ) {	// 11ÇÃì‡ñÛ 4%ìKópï™
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM13 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "AAT00030" ) ) {	// 11ÇÃì‡ñÛ 6.3%ìKópï™
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM14 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "AAT00040" ) ) {	// 11ÇÃì‡ñÛ 6.24%ìKópï™
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM15 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "AAT00050" ) ) {	// 11ÇÃì‡ñÛ 7.8%ìKópï™
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM16 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "AAU00000" ) ) {	// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM17 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "AAV00010" ) ) {	// îÑè„ÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM18 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "AAV00020" ) ) {	// ì¡íËâ€ê≈édì¸ÇÍÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM19 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "AAW00010" ) ) {	// ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM20 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "AAW00020" ) ) {	// ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz 4%ìKópï™
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM21 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "AAW00030" ) ) {	// ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz 6.3%ìKópï™
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM22 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "AAW00040" ) ) {	// ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz 6.24%ãyÇ—7.8%ìKópï™
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM23 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
			}

			// çXêVÉfÅ[É^ämîF
			int AdCnt = 0;
			for ( int i = 0; ; i++ ) {
				if ( strlen ( tagDataIppan2[i].tagName ) == 0 ) {
					break;
				}
				AdCnt++;
			}

			int updCnt = ( int )updTagAry.GetCount();
			if ( AdCnt != updCnt ) {
				// Ç»Ç¢Ç‡ÇÃí«â¡
				CArray<unsigned char*> MonyPtrAry;
				MonyPtrAry.Add ( Sn_XM1 );
				MonyPtrAry.Add ( Sn_XM2 );
				MonyPtrAry.Add ( Sn_XM3 );
				MonyPtrAry.Add ( Sn_XM4 );
				MonyPtrAry.Add ( Sn_XM5 );
				MonyPtrAry.Add ( Sn_XM6 );
				MonyPtrAry.Add ( Sn_XM7 );
				MonyPtrAry.Add ( Sn_XM8 );
				MonyPtrAry.Add ( Sn_XM9 );
				MonyPtrAry.Add ( Sn_XM10 );
				MonyPtrAry.Add ( Sn_XM11 );
				MonyPtrAry.Add ( Sn_XM12 );
				MonyPtrAry.Add ( Sn_XM13 );
				MonyPtrAry.Add ( Sn_XM14 );
				MonyPtrAry.Add ( Sn_XM15 );
				MonyPtrAry.Add ( Sn_XM16 );
				MonyPtrAry.Add ( Sn_XM17 );
				MonyPtrAry.Add ( Sn_XM18 );
				MonyPtrAry.Add ( Sn_XM19 );
				MonyPtrAry.Add ( Sn_XM20 );
				MonyPtrAry.Add ( Sn_XM21 );
				MonyPtrAry.Add ( Sn_XM22 );
				MonyPtrAry.Add ( Sn_XM23 );

				int aryMax = ( int )MonyPtrAry.GetCount();
				BOOL findFlg = FALSE;
				
				for ( int i = 0; ; i++ ) {
					if ( strlen ( tagDataIppan2[i].tagName ) == 0 ) {
						break;
					}

					findFlg = FALSE;
					for ( int j = 0; j < updCnt; j++ ) {
						if ( updTagAry[j].CompareNoCase ( tagDataIppan2[i].tagName ) == 0 ) {
							findFlg = TRUE;
							break;
						}
					}

					if ( findFlg == FALSE ) {
						// äYìñÉ^ÉOÇÃí«â¡
						if ( i < aryMax ) {
							pShLstVal->AddNew();
							pShLstVal->sn_seq = sn_seq;
							pShLstVal->version = nowVersion;
							pShLstVal->hyoId = pHyoID;
							pShLstVal->itmId = tagDataIppan2[i].tagName;
							m_Util.val_to_asci ( &pShLstVal->val, MonyPtrAry[i] );
							pShLstVal->Update();
						}
					}
				}
			}
		}
		else if ( KzHoushiki == ID_ICSSH_KANNI ) {	// ä»à’â€ê≈
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

				if ( pShLstVal->itmId == _T( "ABO00000" ) ) {		// â€ê≈ïWèÄäz
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM1 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "ABP00010" ) ) {	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz 3%ìKópï™
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM2 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "ABP00020" ) ) {	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz 4%ìKópï™
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM3 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "ABP00030" ) ) {	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz 6.3%ìKópï™
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM4 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "ABP00040" ) ) {	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz 6.24%ìKópï™
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM5 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "ABP00050" ) ) {	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz 7.8%ìKópï™
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM6 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "ABP00060" ) ) {	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM7 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "ABR00000" ) ) {	// è¡îÔê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM11 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "ABS00010" ) ) {	// 11ÇÃì‡ñÛ 3%ìKópï™
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM12 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "ABS00020" ) ) {	// 11ÇÃì‡ñÛ 4%ìKópï™
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM13 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "ABS00030" ) ) {	// 11ÇÃì‡ñÛ 6.3%ìKópï™
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM14 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "ABS00040" ) ) {	// 11ÇÃì‡ñÛ 6.24%ìKópï™
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM15 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "ABS00050" ) ) {	// 11ÇÃì‡ñÛ 7.8%ìKópï™
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM16 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "ABT00000" ) ) {	// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM17 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "ABU00010" ) ) {	// îÑè„Ç∞ÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM18 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "ABV00010" ) ) {	// ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM20 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "ABV00020" ) ) {	// ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz 4%ìKópï™
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM21 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "ABV00030" ) ) {	// ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz 6.3%ìKópï™
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM22 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
				else if ( pShLstVal->itmId == _T( "ABV00040" ) ) {	// ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz 6.24%ãyÇ—7.8%ìKópï™
					pShLstVal->Edit();
					m_Util.val_to_asci ( &pShLstVal->val, Sn_XM23 );
					pShLstVal->Update();
					updTagAry.Add ( pShLstVal->itmId );
				}
			}

			// çXêVÉfÅ[É^ämîF
			int AdCnt = 0;
			for ( int i = 0; ; i++ ) {
				if ( strlen ( tagDataKani2[i].tagName ) == 0 ) {
					break;
				}
				AdCnt++;
			}

			int updCnt = ( int )updTagAry.GetCount();
			if ( AdCnt != updCnt ) {
				// Ç»Ç¢Ç‡ÇÃí«â¡
				CArray<unsigned char*> MonyPtrAry;
				MonyPtrAry.Add ( Sn_XM1 );
				MonyPtrAry.Add ( Sn_XM2 );
				MonyPtrAry.Add ( Sn_XM3 );
				MonyPtrAry.Add ( Sn_XM4 );
				MonyPtrAry.Add ( Sn_XM5 );
				MonyPtrAry.Add ( Sn_XM6 );
				MonyPtrAry.Add ( Sn_XM7 );
				MonyPtrAry.Add ( Sn_XM8 );
				MonyPtrAry.Add ( Sn_XM9 );
				MonyPtrAry.Add ( Sn_XM10 );
				MonyPtrAry.Add ( Sn_XM11 );
				MonyPtrAry.Add ( Sn_XM12 );
				MonyPtrAry.Add ( Sn_XM13 );
				MonyPtrAry.Add ( Sn_XM14 );
				MonyPtrAry.Add ( Sn_XM15 );
				MonyPtrAry.Add ( Sn_XM16 );
				MonyPtrAry.Add ( Sn_XM17 );
				MonyPtrAry.Add ( Sn_XM18 );
				MonyPtrAry.Add ( Sn_XM19 );
				MonyPtrAry.Add ( Sn_XM20 );
				MonyPtrAry.Add ( Sn_XM21 );
				MonyPtrAry.Add ( Sn_XM22 );
				MonyPtrAry.Add ( Sn_XM23 );

				int aryMax = ( int )MonyPtrAry.GetCount();
				BOOL findFlg = FALSE;

				for ( int i = 0; ; i++ ) {
					if ( strlen ( tagDataKani2[i].tagName ) == 0 ) {
						break;
					}

					findFlg = FALSE;
					for ( int j = 0; j < updCnt; j++ ) {
						if ( updTagAry[j].CompareNoCase ( tagDataKani2[i].tagName ) == 0 ) {
							findFlg = TRUE;
							break;
						}
					}

					if ( findFlg == FALSE ) {
						// äYìñÉ^ÉOÇÃí«â¡
						if ( i < aryMax ) {
							pShLstVal->AddNew();
							pShLstVal->sn_seq = sn_seq;
							pShLstVal->version = nowVersion;
							pShLstVal->hyoId = pHyoID;
							pShLstVal->itmId = tagDataKani2[i].tagName;
							m_Util.val_to_asci ( &pShLstVal->val, MonyPtrAry[i] );
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
// çXêVéûÇÃã‡äzÉNÉäÉA
//-----------------------------------------------------------------------------
// à¯êî	pDBNpSub	ÅF	ç‡ñ±ÉNÉâÉX
//		type		ÅF	ÉNÉäÉAéÌï 
//-----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	ê≥èÌèIóπ
//			-1		ÅF	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CH31SnHonpyo2Data::ClearDataByRenew(CDBNpSub *pDBNpSub, int type)
{
	ASSERT(pDBNpSub);
	if( pDBNpSub == NULL ){
		return -1;
	}

	CArith	arith(0x16);

	arith.l_clear(Sn_XM1);
	arith.l_clear(Sn_XM2);
	arith.l_clear(Sn_XM3);
	arith.l_clear(Sn_XM4);
	arith.l_clear(Sn_XM5);
	arith.l_clear(Sn_XM6);
	arith.l_clear(Sn_XM7);
	arith.l_clear(Sn_XM8);
	arith.l_clear(Sn_XM9);
	arith.l_clear(Sn_XM10);
	arith.l_clear(Sn_XM11);
	arith.l_clear(Sn_XM12);
	arith.l_clear(Sn_XM13);
	arith.l_clear(Sn_XM14);
	arith.l_clear(Sn_XM15);
	arith.l_clear(Sn_XM16);
	arith.l_clear(Sn_XM17);
	arith.l_clear(Sn_XM18);
	arith.l_clear(Sn_XM19);
	arith.l_clear(Sn_XM20);
	arith.l_clear(Sn_XM21);
	arith.l_clear(Sn_XM22);
	arith.l_clear(Sn_XM23);

	return 0;
}
