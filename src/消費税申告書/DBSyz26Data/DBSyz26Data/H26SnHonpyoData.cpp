#include "StdAfx.h"
#include "H26SnHonpyoData.h"

// å¥ë•â€ê≈
static SH_TAGNAME	tagDataIppan[] = {
	{_T("AAB00010"), 999 },
	{_T("AAB00020"), 999 },
	{_T("AAB00030"), 999 },
	{_T("AAB00050"), 999 },
	{_T("AAB00060"), 999 },
	{_T("AAB00070"), 999 },
	{_T("AAB00110"), 999 },
	{_T("AAB00120"), 999 },
	{_T("AAB00150"), 999 },
	{_T("AAB00180"), 999 },
	{_T("AAB00190"), 999 },
	{_T("AAC00070"), 999 },
	{_T("AAC00080"), 999 },
	{_T("AAC00110"), 999 },
	{_T("AAF00000"), 999 },
	{_T("AAF00010"), 999 },
	{_T("AAF00020"), 999 },
	{_T("AAF00030"), 999 },
	{_T("AAG00140"), 999 },
	{_T("AAG00150"), 999 },
	{_T("AAG00160"), 999 },

	{_T("") }
};

// ä»à’â€ê≈
static SH_TAGNAME	tagDataKani[] = {
	{_T("ABB00010"), 999 },
	{_T("ABB00020"), 999 },
	{_T("ABB00030"), 999 },
	{_T("ABB00050"), 999 },
	{_T("ABB00060"), 999 },
	{_T("ABB00070"), 999 },
	{_T("ABB00110"), 999 },
	{_T("ABB00120"), 999 },
	{_T("ABB00150"), 999 },
	{_T("ABB00170"), 999 },
	{_T("ABB00180"), 999 },
	{_T("ABC00070"), 999 },
	{_T("ABC00080"), 999 },
	{_T("ABC00110"), 999 },
	{_T("ABE00030"), 999 },
	{_T("ABE00050"), 999 },
	{_T("ABE00070"), 999 },
	{_T("ABE00090"), 999 },
	{_T("ABE00110"), 999 },
	{_T("ABH00010"), 999 },
	{_T("ABH00020"), 999 },
	{_T("ABH00030"), 999 },

	{_T("") }
};

// å¥ë•â€ê≈(çXêVéûämîF)
static SH_TAGNAME	tagDataIppanInRenew[] = {
	{_T("AAF00000"), 999 },	// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
	{_T("AAF00020"), 999 },	// ëOÅXâÒî[ïtê≈äz
	{_T("AAF00010"), 999 },	// ëOîNÇÃâ€ê≈îÑè„çÇ
	{_T("AAF00030"), 999 },	// ëOâÒî[ïtê≈äz

	{_T("") }
};

// ä»à’â€ê≈(çXêVéûämîF)
static SH_TAGNAME	tagDataKaniInRenew[] = {
	{_T("ABB00180"), 999 },	// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
	{_T("ABH00020"), 999 },	// ëOÅXâÒî[ïtê≈äz
	{_T("ABH00010"), 999 },	// ëOîNÇÃâ€ê≈îÑè„çÇ
	{_T("ABH00030"), 999 },	// ëOâÒî[ïtê≈äz

	{_T("") }
};

//-----------------------------------------------------------------------------
// ÉRÉìÉXÉgÉâÉNÉ^
//-----------------------------------------------------------------------------
CH26SnHonpyoData::CH26SnHonpyoData()
{
	memset( Sn_KSTDM, '\0', sizeof(Sn_KSTDM) );
	memset( Sn_KSTD, '\0', sizeof(Sn_KSTD) );
	memset( Sn_TIKAM, '\0', sizeof(Sn_TIKAM) );
	memset( Sn_TIKA, '\0', sizeof(Sn_TIKA) );
	memset( Sn_SYTXM, '\0', sizeof(Sn_SYTXM) );
	memset( Sn_SYTX, '\0', sizeof(Sn_SYTX) );
	memset( Sn_KJADJZ, '\0', sizeof(Sn_KJADJZ) );
	memset( Sn_SIREZ, '\0', sizeof(Sn_SIREZ) );
	memset( Sn_HKANZ, '\0', sizeof(Sn_HKANZ) );
	memset( Sn_KSITZ, '\0', sizeof(Sn_KSITZ) );
	memset( Sn_TYNOFZ, '\0', sizeof(Sn_TYNOFZ) );
	memset( Sn_EDNOFZ, '\0', sizeof(Sn_EDNOFZ) );
	memset( Sn_KAKTIZ, '\0', sizeof(Sn_KAKTIZ) );
	memset( Sn_TTYWAR, '\0', sizeof(Sn_TTYWAR) );
	memset( Sn_TKAKTZ, '\0', sizeof(Sn_TKAKTZ) );
	memset( Sn_TEDNOF, '\0', sizeof(Sn_TEDNOF) );
	memset( Sn_KZURI, '\0', sizeof(Sn_KZURI) );
	memset( Sn_SOURI, '\0', sizeof(Sn_SOURI) );
	memset( Sn_ZZNOFZ, '\0', sizeof(Sn_ZZNOFZ) );
	memset( Sn_ZNOFZ, '\0', sizeof(Sn_ZNOFZ) );
//	memset( Sn_WURI, '\0', sizeof(Sn_WURI) );
	memset( Sn_ZZURI, '\0', sizeof(Sn_ZZURI) );
	memset( Sn_ZURI, '\0', sizeof(Sn_ZURI) );
	memset( Sn_KURI1, '\0', sizeof(Sn_KURI1) );
	memset( Sn_KURI2, '\0', sizeof(Sn_KURI2) );
	memset( Sn_KURI3, '\0', sizeof(Sn_KURI3) );
	memset( Sn_KURI4, '\0', sizeof(Sn_KURI4) );
	memset( Sn_KURI5, '\0', sizeof(Sn_KURI5) );
	memset( Sn_GENKAI, '\0', sizeof(Sn_GENKAI) );
	memset( Sn_WGENKA, '\0', sizeof(Sn_WGENKA) );

	memset( Sn_TS_TDM1, '\0', sizeof(Sn_TS_TDM1) );
	memset( Sn_TS_SYT1, '\0', sizeof(Sn_TS_SYT1) );
	memset( Sn_TS_SYT2, '\0', sizeof(Sn_TS_SYT2) );
	Sn_UP1 = Sn_UP2 = Sn_UP3 = Sn_UP4 = Sn_UP5 = 0;

	Sn_SYTsw = 0;
	Sn_ZNOsw = 0;
	memset( rsv1, '\0', sizeof(rsv1) );
	memset( rsv2, '\0', sizeof(rsv2) );
}

//-----------------------------------------------------------------------------
// ÉfÉXÉgÉâÉNÉ^
//-----------------------------------------------------------------------------
CH26SnHonpyoData::~CH26SnHonpyoData()
{
	if( m_DBSyzAccess.m_pSH_LST_VAL ){
		m_DBSyzAccess.CloseShLstVal();
	}

	if( m_DBSyzAccess.m_pSH_LST_RATE ){
		m_DBSyzAccess.CloseShLstRate();
	}
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
int CH26SnHonpyoData::GetData( CDBNpSub *pDBNpSub, int sn_seq )
{
#ifdef _DBG_VIEW_START_
	CString	hoge;
	hoge.Format("CH26SnHonpyoData::GetData()__START\n");
	DbgViewTrace(hoge);
#endif

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
		filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_IPPAN_HONPYO_HYOID, nowVersion );
	}
	else if( KzHoushiki == ID_ICSSH_KANNI ){
		filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_KANNI_HONPYO_HYOID, nowVersion );
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

	BOOL	isNodata = TRUE;
	if( KzHoushiki == ID_ICSSH_GENSOKU ){
		for( int i=0;; i++ ){
			if( i==0 ){
				if( (pShLstVal->MoveFirst()==-1) || (pShLstVal->st==-1) ){
					break;
				}
				isNodata = FALSE;
			}
			else{
				if( (pShLstVal->MoveNext()==-1) || (pShLstVal->st==-1) ){
					break;
				}
			}

			// ã‡äzÇïœä∑ÇµÇƒë„ì¸
			if( pShLstVal->itmId == _T("AAB00010") ){		// â€ê≈ïWèÄäz
				m_Util.val_to_bin( Sn_KSTD, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AAB00020") ){	// è¡îÔê≈äz
				m_Util.val_to_bin( Sn_SYTX, pShLstVal->val );
				Sn_SYTsw = pShLstVal->sign1;//[Y]
			}
			else if( pShLstVal->itmId == _T("AAB00030") ){	// çTèúâﬂëÂí≤êÆê≈äz
				m_Util.val_to_bin( Sn_KJADJZ, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AAB00050") ){	// çTèúëŒè€édì¸ê≈äz
				m_Util.val_to_bin( Sn_SIREZ, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AAB00060") ){	// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
				m_Util.val_to_bin( Sn_HKANZ, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AAB00070") ){	// ë›ì|ÇÍÇ…åWÇÈê≈äz
				m_Util.val_to_bin( Sn_KSITZ, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AAB00110") ){	// íÜä‘î[ïtê≈äz
				m_Util.val_to_bin( Sn_TYNOFZ, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AAB00120") ){	// î[ïtê≈äz
				m_Util.val_to_bin( Sn_EDNOFZ, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AAB00150") ){	// ä˘ämíËê≈äz
				m_Util.val_to_bin( Sn_KAKTIZ, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AAB00180") ){	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
				m_Util.val_to_bin( Sn_KZURI, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AAB00190") ){	// éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
				m_Util.val_to_bin( Sn_SOURI, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AAC00070") ){	// íÜä‘î[ïtè˜ìnäÑäzÅiínï˚è¡îÔê≈Åj
				m_Util.val_to_bin( Sn_TTYWAR, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AAC00080") ){	// î[ïtè˜ìnäÑäzÅiínï˚è¡îÔê≈Åj
				m_Util.val_to_bin( Sn_TEDNOF, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AAC00110") ){	// ä˘ämíËè˜ìnäÑäz
				m_Util.val_to_bin( Sn_TKAKTZ, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AAF00000") ){	// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
				m_Util.val_to_bin( Sn_ZZURI, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AAF00010") ){	// ëOîNÇÃâ€ê≈îÑè„çÇ
				m_Util.val_to_bin( Sn_ZURI, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AAF00020") ){	// ëOÅXâÒî[ïtê≈äz
				m_Util.val_to_bin( Sn_ZZNOFZ, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AAF00030") ){	// ëOâÒî[ïtê≈äz
				m_Util.val_to_bin( Sn_ZNOFZ, pShLstVal->val );
				Sn_ZNOsw = pShLstVal->sign1;
			}
			/*1ÅE2ÇÃì‡ñÛ3çsñ⁄*/
			else if( pShLstVal->itmId == _T("AAG00140") ){	
				m_Util.val_to_bin( Sn_TS_TDM1, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AAG00150") ){	
				m_Util.val_to_bin( Sn_TS_SYT1, pShLstVal->val );
			}
			/*17ÅE18ÇÃì‡ñÛ3çsñ⁄*/
			else if( pShLstVal->itmId == _T("AAG00160") ){	
				m_Util.val_to_bin( Sn_TS_SYT2, pShLstVal->val );
			}
		}
	}
	else if( KzHoushiki == ID_ICSSH_KANNI ){
		for( int i=0;; i++ ){
			if( i==0 ){
				if( (pShLstVal->MoveFirst()==-1) || (pShLstVal->st==-1) ){
					break;
				}
				isNodata = FALSE;
			}
			else{
				if( (pShLstVal->MoveNext()==-1) || (pShLstVal->st==-1) ){
					break;
				}
			}

			// ã‡äzÇïœä∑ÇµÇƒë„ì¸
			if( pShLstVal->itmId == _T("ABB00010") ){		// â€ê≈ïWèÄäz
				m_Util.val_to_bin( Sn_KSTD, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ABB00020") ){	// è¡îÔê≈äz
				m_Util.val_to_bin( Sn_SYTX, pShLstVal->val );
				Sn_SYTsw = pShLstVal->sign1;//[Y]
			}
			else if( pShLstVal->itmId == _T("ABB00030") ){	// ë›ì|âÒé˚Ç…åWÇÈè¡îÔê≈äz
				m_Util.val_to_bin( Sn_KJADJZ, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ABB00050") ){	// çTèúëŒè€édì¸ê≈äz
				m_Util.val_to_bin( Sn_SIREZ, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ABB00060") ){	// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
				m_Util.val_to_bin( Sn_HKANZ, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ABB00070") ){	// ë›ì|ÇÍÇ…åWÇÈê≈äz
				m_Util.val_to_bin( Sn_KSITZ, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ABB00110") ){	// íÜä‘î[ïtê≈äz
				m_Util.val_to_bin( Sn_TYNOFZ, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ABB00120") ){	// î[ïtê≈äz
				m_Util.val_to_bin( Sn_EDNOFZ, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ABB00150") ){	// ä˘ämíËê≈äz
				m_Util.val_to_bin( Sn_KAKTIZ, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ABB00170") ){	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
				m_Util.val_to_bin( Sn_KZURI, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ABB00180") ){	// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
//				m_Util.val_to_bin( Sn_SOURI, pShLstVal->val );
				m_Util.val_to_bin( Sn_ZZURI, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ABC00070") ){	// íÜä‘î[ïtè˜ìnäÑäzÅiínï˚è¡îÔê≈Åj
				m_Util.val_to_bin( Sn_TTYWAR, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ABC00080") ){	// î[ïtè˜ìnäÑäzÅiínï˚è¡îÔê≈Åj
				m_Util.val_to_bin( Sn_TEDNOF, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ABC00110") ){	// ä˘ämíËè˜ìnäÑäz
				m_Util.val_to_bin( Sn_TKAKTZ, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ABE00030") ){	// ëÊÇPéÌéñã∆îÑè„çÇ
				m_Util.val_to_bin( Sn_KURI1, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ABE00050") ){	// ëÊÇQéÌéñã∆îÑè„çÇ
				m_Util.val_to_bin( Sn_KURI2, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ABE00070") ){	// ëÊÇRéÌéñã∆îÑè„çÇ
				m_Util.val_to_bin( Sn_KURI3, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ABE00090") ){	// ëÊÇSéÌéñã∆îÑè„çÇ
				m_Util.val_to_bin( Sn_KURI4, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ABE00110") ){	// ëÊÇTéÌéñã∆îÑè„çÇ
				m_Util.val_to_bin( Sn_KURI5, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ABH00010") ){	// ëOîNÇÃâ€ê≈îÑè„çÇ
				m_Util.val_to_bin( Sn_ZURI, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ABH00020") ){	// ëOÅXâÒî[ïtê≈äz
				m_Util.val_to_bin( Sn_ZZNOFZ, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ABH00030") ){	// ëOâÒî[ïtê≈äz
				m_Util.val_to_bin( Sn_ZNOFZ, pShLstVal->val );
			}
		}
	}

//--> '14.04.11 INS STAET
	if( isNodata ){
		// ï â€ê≈ï˚éÆÇ…ÉfÅ[É^óLÅH
		if( KzHoushiki == ID_ICSSH_GENSOKU ){
			filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_KANNI_HONPYO_HYOID, nowVersion);
		}
		else if( KzHoushiki == ID_ICSSH_KANNI ){
			filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_IPPAN_HONPYO_HYOID, nowVersion);
		}
		
		if( m_DBSyzAccess.m_pSH_LST_VAL ){
			m_DBSyzAccess.m_pSH_LST_VAL->Requery( filter );
		}
		CSH_LST_VAL	*pShLstVal = m_DBSyzAccess.m_pSH_LST_VAL;

		if( pShLstVal->st != -1 ){
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

				if( pShLstVal->itmId == _T("AAF00000") ){		// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
					m_Util.val_to_bin( Sn_ZZURI, pShLstVal->val );
				}
				else if( pShLstVal->itmId == _T("AAF00010") ){	// ëOîNÇÃâ€ê≈îÑè„çÇ
					m_Util.val_to_bin( Sn_ZURI, pShLstVal->val );
				}
				else if( pShLstVal->itmId == _T("AAF00020") ){	// ëOÅXâÒî[ïtê≈äz
					m_Util.val_to_bin( Sn_ZZNOFZ, pShLstVal->val );
				}
				else if( pShLstVal->itmId == _T("AAF00030") ){	// ëOâÒî[ïtê≈äz
					m_Util.val_to_bin( Sn_ZNOFZ, pShLstVal->val );
					Sn_ZNOsw = pShLstVal->sign1;
				}
				else if( pShLstVal->itmId == _T("ABB00180") ){	// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
					m_Util.val_to_bin( Sn_ZZURI, pShLstVal->val );
				}
				else if( pShLstVal->itmId == _T("ABH00010") ){	// ëOîNÇÃâ€ê≈îÑè„çÇ
					m_Util.val_to_bin( Sn_ZURI, pShLstVal->val );
				}
				else if( pShLstVal->itmId == _T("ABH00020") ){	// ëOÅXâÒî[ïtê≈äz
					m_Util.val_to_bin( Sn_ZZNOFZ, pShLstVal->val );
				}
				else if( pShLstVal->itmId == _T("ABH00030") ){	// ëOâÒî[ïtê≈äz
					m_Util.val_to_bin( Sn_ZNOFZ, pShLstVal->val );
				}
			}

			int	nowVersion = 1;
			if( KzHoushiki == ID_ICSSH_GENSOKU ){
				// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
				pShLstVal->AddNew();
				pShLstVal->sn_seq = sn_seq;
				pShLstVal->version = nowVersion;
				pShLstVal->hyoId = ICS_SN_IPPAN_HONPYO_HYOID;
				pShLstVal->itmId = _T("AAF00000");
				m_Util.val_to_asci( &pShLstVal->val, Sn_ZZURI );
				pShLstVal->sign1 = 0;
				pShLstVal->Update();

				// ëOîNÇÃâ€ê≈îÑè„çÇ
				pShLstVal->AddNew();
				pShLstVal->sn_seq = sn_seq;
				pShLstVal->version = nowVersion;
				pShLstVal->hyoId = ICS_SN_IPPAN_HONPYO_HYOID;
				pShLstVal->itmId = _T("AAF00010");
				m_Util.val_to_asci( &pShLstVal->val, Sn_ZURI );
				pShLstVal->sign1 = 0;
				pShLstVal->Update();

				// ëOÅXâÒî[ïtê≈äz
				pShLstVal->AddNew();
				pShLstVal->sn_seq = sn_seq;
				pShLstVal->version = nowVersion;
				pShLstVal->hyoId = ICS_SN_IPPAN_HONPYO_HYOID;
				pShLstVal->itmId = _T("AAF00020");
				m_Util.val_to_asci( &pShLstVal->val, Sn_ZZNOFZ );
				pShLstVal->sign1 = 0;
				pShLstVal->Update();

				// ëOâÒî[ïtê≈äz
				pShLstVal->AddNew();
				pShLstVal->sn_seq = sn_seq;
				pShLstVal->version = nowVersion;
				pShLstVal->hyoId = ICS_SN_IPPAN_HONPYO_HYOID;
				pShLstVal->itmId = _T("AAF00030");
				m_Util.val_to_asci( &pShLstVal->val, Sn_ZNOFZ );
				pShLstVal->sign1 = Sn_ZNOsw;
				pShLstVal->Update();
			}
			else if( KzHoushiki == ID_ICSSH_KANNI ){
				// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
				pShLstVal->AddNew();
				pShLstVal->sn_seq = sn_seq;
				pShLstVal->version = nowVersion;
				pShLstVal->hyoId = ICS_SN_KANNI_HONPYO_HYOID;
				pShLstVal->itmId = _T("ABB00180");
				m_Util.val_to_asci( &pShLstVal->val, Sn_ZZURI );
				pShLstVal->sign1 = 0;
				pShLstVal->Update();

				// ëOîNÇÃâ€ê≈îÑè„çÇ
				pShLstVal->AddNew();
				pShLstVal->sn_seq = sn_seq;
				pShLstVal->version = nowVersion;
				pShLstVal->hyoId = ICS_SN_KANNI_HONPYO_HYOID;
				pShLstVal->itmId = _T("ABH00010");
				m_Util.val_to_asci( &pShLstVal->val, Sn_ZURI );
				pShLstVal->sign1 = 0;
				pShLstVal->Update();

				// ëOÅXâÒî[ïtê≈äz
				pShLstVal->AddNew();
				pShLstVal->sn_seq = sn_seq;
				pShLstVal->version = nowVersion;
				pShLstVal->hyoId = ICS_SN_KANNI_HONPYO_HYOID;
				pShLstVal->itmId = _T("ABH00020");
				m_Util.val_to_asci( &pShLstVal->val, Sn_ZZNOFZ );
				pShLstVal->sign1 = 0;
				pShLstVal->Update();

				// ëOâÒî[ïtê≈äz
				pShLstVal->AddNew();
				pShLstVal->sn_seq = sn_seq;
				pShLstVal->version = nowVersion;
				pShLstVal->hyoId = ICS_SN_KANNI_HONPYO_HYOID;
				pShLstVal->itmId = _T("ABH00030");
				m_Util.val_to_asci( &pShLstVal->val, Sn_ZNOFZ );
				pShLstVal->sign1 = 0;
				pShLstVal->Update();
			}
		}
	}
//<-- '14.04.11 INS END

	// îÑè„äÑçá
	if( KzHoushiki == ID_ICSSH_KANNI ){
		filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_KANNI_HONPYO_HYOID, nowVersion);

		if( m_DBSyzAccess.m_pSH_LST_RATE ){
			m_DBSyzAccess.m_pSH_LST_RATE->Requery( filter );
		}
		else{
			if( m_DBSyzAccess.OpenShLstRate(pDBNpSub->m_database, filter) ){
				return -1;
			}
		}

		CSH_LST_RATE	*pShLstRate = m_DBSyzAccess.m_pSH_LST_RATE;

		for( int i=0;; i++ ){
			if( i==0 ){
				if( (pShLstRate->MoveFirst()==-1) || (pShLstRate->st==-1) ){
					break;
				}
			}
			else{
				if( (pShLstRate->MoveNext()==-1) || (pShLstRate->st==-1) ){
					break;
				}
			}
			
			if( pShLstRate->itmId == _T("ABE00040") ){
				Sn_UP1 = pShLstRate->rate;
			}
			else if( pShLstRate->itmId == _T("ABE00060") ){
				Sn_UP2 = pShLstRate->rate;
			}
			else if( pShLstRate->itmId == _T("ABE00080") ){
				Sn_UP3 = pShLstRate->rate;
			}
			else if( pShLstRate->itmId == _T("ABE00100") ){
				Sn_UP4 = pShLstRate->rate;
			}
			else if( pShLstRate->itmId == _T("ABE00120") ){
				Sn_UP5 = pShLstRate->rate;
			}
		}
	}

	// à»â∫ÇÃå‹çÄñ⁄ÇÕ
	// ì«çûÇ›éûÅAä˙ä‘Ç…ÇÊÇËîªï 
	// èëçûÇ›ÇÕÅAêVãKÉeÅ[ÉuÉãÅ{ä˘ë∂ÉeÅ[ÉuÉã
//	Sn_TTYWAR;
//	Sn_TKAKTZ;
//	Sn_TEDNOF;
//	Sn_EDNOFZ;
//	Sn_TYNOFZ;
	if( (pDBNpSub->zvol->ss_ymd<ICS_SH_8PER_REFORM_DAY) || (pDBNpSub->zvol->ee_ymd>ICS_SH_10PER_REFORM_DAY) ){
#ifdef _DBG_VIEW_START_
		hoge.Format("CH26SnHonpyoData::GetData()__ì¡éÍä˙ä‘ì«çû\n");
		DbgViewTrace(hoge);
#endif
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
		if( pSZSNK->st != -1 ){
			m_Util.val_to_bin( Sn_TTYWAR, pSZSNK->Sn_TTYWAR );
			m_Util.val_to_bin( Sn_TKAKTZ, pSZSNK->Sn_TKAKTZ );
			m_Util.val_to_bin( Sn_EDNOFZ, pSZSNK->Sn_EDNOFZ );
			m_Util.val_to_bin( Sn_TEDNOF, pSZSNK->Sn_TEDNOF );
			m_Util.val_to_bin( Sn_TYNOFZ, pSZSNK->Sn_TYNOFZ );
		}
	}

#ifdef _DBG_VIEW_START_
	hoge.Format("CH26SnHonpyoData::GetData()__END\n");
	DbgViewTrace(hoge);
#endif

	return 0;
}

//-----------------------------------------------------------------------------
// DBÇ÷ÉfÅ[É^èëçûÇ›
//-----------------------------------------------------------------------------
// à¯êî	pDBNpSub	ÅF	ç‡ñ±ÉNÉâÉXÇÃÉ|ÉCÉìÉ^
//		sn_seq		ÅF	ëŒè€ÇÃóöóÉVÅ[ÉPÉìÉX
//		wrtype		ÅF	èëçûÇ›É^ÉCÉv	Å†Å†Å†Å† Å†Å†Å†Å† Å†Å†Å†Å† Å†Å†Å†Å† Å†Å†Å†Å† Å†Å†Å†Å† Å†Å†Å†Å† Å†Å†Å†Å°
//						 D0ÅFszsnk Ç÷ÉfÅ[É^ÇèëÇ´çûÇ‹Ç»Ç¢
//-----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	ê≥èÌèIóπ
//			-1		ÅF	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CH26SnHonpyoData::SetData( CDBNpSub *pDBNpSub, int sn_seq, int wrtype/*=0*/ )
{
#ifdef _DBG_VIEW_START_
	CString	hoge;
	hoge.Format("CH26SnHonpyoData::SetData()__START\n");
	DbgViewTrace(hoge);
#endif

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
	char	*pHyoID = NULL;
	if( KzHoushiki == ID_ICSSH_GENSOKU ){
		filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_IPPAN_HONPYO_HYOID, nowVersion );
		pHyoID = ICS_SN_IPPAN_HONPYO_HYOID;
	}
	else if( KzHoushiki == ID_ICSSH_KANNI ){
		filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_KANNI_HONPYO_HYOID, nowVersion );
		pHyoID = ICS_SN_KANNI_HONPYO_HYOID;
	}

	if( m_DBSyzAccess.m_pSH_LST_VAL ){
		m_DBSyzAccess.m_pSH_LST_VAL->Requery( filter );
	}
	else{
		if( m_DBSyzAccess.OpenShLstVal(pDBNpSub->m_database, filter) ){
			return -1;
		}
	}

//--> '15.01.14 INS START
	BOOL	isChg_ZZURI = FALSE;	// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇÇ…ïœçXóLÇËÅH
//<-- '15.01.14 INS END

	CSH_LST_VAL	*pShLstVal = m_DBSyzAccess.m_pSH_LST_VAL;

	if( (pShLstVal->MoveFirst()==-1) || (pShLstVal->st==-1) ){	// ÉfÅ[É^ñ≥Çµ

		if( KzHoushiki == ID_ICSSH_GENSOKU ){

			// â€ê≈ïWèÄäz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAB00010");
			m_Util.val_to_asci( &pShLstVal->val, Sn_KSTD );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// è¡îÔê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAB00020");
			m_Util.val_to_asci( &pShLstVal->val, Sn_SYTX );
			pShLstVal->sign1 = Sn_SYTsw;//[Y]
			pShLstVal->Update();

			// çTèúâﬂëÂí≤êÆê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAB00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_KJADJZ );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// çTèúëŒè€édì¸ê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAB00050");
			m_Util.val_to_asci( &pShLstVal->val, Sn_SIREZ );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAB00060");
			m_Util.val_to_asci( &pShLstVal->val, Sn_HKANZ );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ë›ì|ÇÍÇ…åWÇÈê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAB00070");
			m_Util.val_to_asci( &pShLstVal->val, Sn_KSITZ );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// íÜä‘î[ïtê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAB00110");
			m_Util.val_to_asci( &pShLstVal->val, Sn_TYNOFZ );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// î[ïtê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAB00120");
			m_Util.val_to_asci( &pShLstVal->val, Sn_EDNOFZ );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ä˘ämíËê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAB00150");
			m_Util.val_to_asci( &pShLstVal->val, Sn_KAKTIZ );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAB00180");
			m_Util.val_to_asci( &pShLstVal->val, Sn_KZURI );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAB00190");
			m_Util.val_to_asci( &pShLstVal->val, Sn_SOURI );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// íÜä‘î[ïtè˜ìnäÑäzÅiínï˚è¡îÔê≈Åj
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAC00070");
			m_Util.val_to_asci( &pShLstVal->val, Sn_TTYWAR );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// î[ïtè˜ìnäÑäzÅiínï˚è¡îÔê≈Åj
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAC00080");
			m_Util.val_to_asci( &pShLstVal->val, Sn_TEDNOF );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ä˘ämíËè˜ìnäÑäz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAC00110");
			m_Util.val_to_asci( &pShLstVal->val, Sn_TKAKTZ );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAF00000");
			m_Util.val_to_asci( &pShLstVal->val, Sn_ZZURI );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ëOîNÇÃâ€ê≈îÑè„çÇ
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAF00010");
			m_Util.val_to_asci( &pShLstVal->val, Sn_ZURI );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ëOÅXâÒî[ïtê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAF00020");
			m_Util.val_to_asci( &pShLstVal->val, Sn_ZZNOFZ );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ëOâÒî[ïtê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAF00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_ZNOFZ );
			pShLstVal->sign1 = Sn_ZNOsw;
			pShLstVal->Update();

			/*1ÅE2ÇÃì‡ñÛ3çsñ⁄*/
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAG00140");
			m_Util.val_to_asci( &pShLstVal->val, Sn_TS_TDM1 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();
			
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAG00150");
			m_Util.val_to_asci( &pShLstVal->val, Sn_TS_SYT1 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			/*17ÅE18ÇÃì‡ñÛ3çsñ⁄*/
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAG00160");
			m_Util.val_to_asci( &pShLstVal->val, Sn_TS_SYT2 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

		}
		else if( KzHoushiki == ID_ICSSH_KANNI ){

			// â€ê≈ïWèÄäz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABB00010");
			m_Util.val_to_asci( &pShLstVal->val, Sn_KSTD );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// è¡îÔê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABB00020");
			m_Util.val_to_asci( &pShLstVal->val, Sn_SYTX );
			pShLstVal->sign1 = Sn_SYTsw;//[Y]
			pShLstVal->Update();

			// ë›ì|âÒé˚Ç…åWÇÈè¡îÔê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABB00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_KJADJZ );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// çTèúëŒè€édì¸ê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABB00050");
			m_Util.val_to_asci( &pShLstVal->val, Sn_SIREZ );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABB00060");
			m_Util.val_to_asci( &pShLstVal->val, Sn_HKANZ );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ë›ì|ÇÍÇ…åWÇÈê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABB00070");
			m_Util.val_to_asci( &pShLstVal->val, Sn_KSITZ );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// íÜä‘î[ïtê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABB00110");
			m_Util.val_to_asci( &pShLstVal->val, Sn_TYNOFZ );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// î[ïtê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABB00120");
			m_Util.val_to_asci( &pShLstVal->val, Sn_EDNOFZ );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ä˘ämíËê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABB00150");
			m_Util.val_to_asci( &pShLstVal->val, Sn_KAKTIZ );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABB00170");
			m_Util.val_to_asci( &pShLstVal->val, Sn_KZURI );
//			m_Util.val_to_asci( &pShLstVal->val, Sn_ZZURI );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABB00180");
//			m_Util.val_to_asci( &pShLstVal->val, Sn_SOURI );
			m_Util.val_to_asci( &pShLstVal->val, Sn_ZZURI );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// íÜä‘î[ïtè˜ìnäÑäzÅiínï˚è¡îÔê≈Åj
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABC00070");
			m_Util.val_to_asci( &pShLstVal->val, Sn_TTYWAR );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// î[ïtè˜ìnäÑäzÅiínï˚è¡îÔê≈Åj
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABC00080");
			m_Util.val_to_asci( &pShLstVal->val, Sn_TEDNOF );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ä˘ämíËè˜ìnäÑäz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABC00110");
			m_Util.val_to_asci( &pShLstVal->val, Sn_TKAKTZ );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();


			// ëÊÇPéÌîÑè„çÇ
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABE00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_KURI1 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ëÊÇQéÌîÑè„çÇ
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABE00050");
			m_Util.val_to_asci( &pShLstVal->val, Sn_KURI2 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ëÊÇRéÌîÑè„çÇ
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABE00070");
			m_Util.val_to_asci( &pShLstVal->val, Sn_KURI3 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ëÊÇSéÌîÑè„çÇ
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABE00090");
			m_Util.val_to_asci( &pShLstVal->val, Sn_KURI4 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ëÊÇTéÌîÑè„çÇ
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABE00110");
			m_Util.val_to_asci( &pShLstVal->val, Sn_KURI5 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ëOîNÇÃâ€ê≈îÑè„çÇ
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABH00010");
			m_Util.val_to_asci( &pShLstVal->val, Sn_ZURI );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ëOÅXâÒî[ïtê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABH00020");
			m_Util.val_to_asci( &pShLstVal->val, Sn_ZZNOFZ );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ëOâÒî[ïtê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABH00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_ZNOFZ );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();
		}
	}
	else{						// ÉfÅ[É^óL	
		if( KzHoushiki == ID_ICSSH_GENSOKU ){
			CStringArray	updTagAry;
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
				if( pShLstVal->itmId == _T("AAB00010") ){		// â€ê≈ïWèÄäz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_KSTD );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AAB00020") ){	// è¡îÔê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_SYTX );
					pShLstVal->sign1 = Sn_SYTsw;//[Y]
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AAB00030") ){	// çTèúâﬂëÂí≤êÆê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_KJADJZ );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AAB00050") ){	// çTèúëŒè€édì¸ê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_SIREZ );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AAB00060") ){	// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_HKANZ );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AAB00070") ){	// ë›ì|ÇÍÇ…åWÇÈê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_KSITZ );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AAB00110") ){	// íÜä‘î[ïtê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_TYNOFZ );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AAB00120") ){	// î[ïtê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_EDNOFZ );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AAB00150") ){	// ä˘ämíËê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_KAKTIZ );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AAB00180") ){	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_KZURI );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AAB00190") ){	// éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_SOURI );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AAC00070") ){	// íÜä‘î[ïtè˜ìnäÑäzÅiínï˚è¡îÔê≈Åj
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_TTYWAR );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AAC00080") ){	// î[ïtè˜ìnäÑäzÅiínï˚è¡îÔê≈Åj
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_TEDNOF );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AAC00110") ){	// ä˘ämíËè˜ìnäÑäz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_TKAKTZ );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AAF00000") ){	// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
					pShLstVal->Edit();
//-- '15.01.14 --
//					m_Util.val_to_asci( &pShLstVal->val, Sn_ZZURI );
//---------------
					CString	tmpStr;
					m_Util.val_to_asci( &tmpStr, Sn_ZZURI );
					if( pShLstVal->val.Compare(tmpStr) ){
						isChg_ZZURI = TRUE;
					}
					pShLstVal->val = tmpStr;
//---------------
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AAF00010") ){	// ëOîNÇÃâ€ê≈îÑè„çÇ
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_ZURI );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AAF00020") ){	// ëOÅXâÒî[ïtê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_ZZNOFZ );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AAF00030") ){	// ëOâÒî[ïtê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_ZNOFZ );
					pShLstVal->sign1 = Sn_ZNOsw;
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}/*1ÅE2ÇÃì‡ñÛ3çsñ⁄*/
				else if( pShLstVal->itmId == _T("AAG00140") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_TS_TDM1 );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AAG00150") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_TS_SYT1 );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}/*17ÅE18ÇÃì‡ñÛ3çsñ⁄*/
				else if( pShLstVal->itmId == _T("AAG00160") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_TS_SYT2 );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
			}

			// çXêVÉfÅ[É^ämîF
			int	adCnt = 0;
			for( int i=0;; i++ ){
				if( strlen(tagDataIppan[i].tagName) == 0 ){
					break;
				}
				adCnt++;
			}
			int updCnt = (int)updTagAry.GetCount();
			if( adCnt != updCnt ){
				// ñ≥Ç¢Ç‡ÇÃÇí«â¡
				CArray<unsigned char*>	MonyPtrAry;
				MonyPtrAry.Add( Sn_KSTD );
				MonyPtrAry.Add( Sn_SYTX );
				MonyPtrAry.Add( Sn_KJADJZ );
				MonyPtrAry.Add( Sn_SIREZ );
				MonyPtrAry.Add( Sn_HKANZ );
				MonyPtrAry.Add( Sn_KSITZ );
				MonyPtrAry.Add( Sn_TYNOFZ );
				MonyPtrAry.Add( Sn_EDNOFZ );
				MonyPtrAry.Add( Sn_KAKTIZ );
				MonyPtrAry.Add( Sn_KZURI );
				MonyPtrAry.Add( Sn_SOURI );
				MonyPtrAry.Add( Sn_TTYWAR );
				MonyPtrAry.Add( Sn_TEDNOF );
				MonyPtrAry.Add( Sn_TKAKTZ );
				MonyPtrAry.Add( Sn_ZZURI );
				MonyPtrAry.Add( Sn_ZURI );
				MonyPtrAry.Add( Sn_ZZNOFZ );
				MonyPtrAry.Add( Sn_ZNOFZ );
				MonyPtrAry.Add( Sn_TS_TDM1 );
				MonyPtrAry.Add( Sn_TS_SYT1 );
				MonyPtrAry.Add( Sn_TS_SYT2 );
				int	aryMax = (int)MonyPtrAry.GetCount();

				BOOL findFlg = FALSE;
				for( int i=0;; i++ ){
					if( strlen(tagDataIppan[i].tagName) == 0 ){
						break;
					}

					findFlg = FALSE;
					for( int j=0; j<updCnt; j++ ){
						if( updTagAry[j].CompareNoCase(tagDataIppan[i].tagName) == 0 ){
							findFlg = TRUE;
							break;
						}
					}
					if( findFlg == FALSE ){
						// äYìñÉ^ÉOÇÃí«â¡
						if( i < aryMax ){
							pShLstVal->AddNew();
							pShLstVal->sn_seq = sn_seq;
							pShLstVal->version = nowVersion;
							pShLstVal->hyoId = pHyoID;
							pShLstVal->itmId = tagDataIppan[i].tagName;
							m_Util.val_to_asci( &pShLstVal->val, MonyPtrAry[i] );
//							if( tagDataIppan[i].tagName == _T("AEG00040") ){
//								pShLstVal->sign1 = Sn_2F21sw;
//							}
//							else{
//								pShLstVal->sign1 = 0;
//							}
							pShLstVal->Update();
						}
					}
				}
			}
		}
		else if( KzHoushiki == ID_ICSSH_KANNI ){
			CStringArray	updTagAry;
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
				if( pShLstVal->itmId == _T("ABB00010") ){		// â€ê≈ïWèÄäz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_KSTD );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("ABB00020") ){	// è¡îÔê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_SYTX );
					pShLstVal->sign1 = Sn_SYTsw;//[Y]
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("ABB00030") ){	// ë›ì|âÒé˚Ç…åWÇÈè¡îÔê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_KJADJZ );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("ABB00050") ){	// çTèúëŒè€édì¸ê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_SIREZ );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("ABB00060") ){	// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_HKANZ );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("ABB00070") ){	// ë›ì|ÇÍÇ…åWÇÈê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_KSITZ );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("ABB00110") ){	// íÜä‘î[ïtê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_TYNOFZ );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("ABB00120") ){	// î[ïtê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_EDNOFZ );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("ABB00150") ){	// ä˘ämíËê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_KAKTIZ );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("ABB00170") ){	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_KZURI );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("ABB00180") ){	// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
					pShLstVal->Edit();
//-- '15.01.14 --
//					m_Util.val_to_asci( &pShLstVal->val, Sn_ZZURI );
//---------------
					CString	tmpStr;
					m_Util.val_to_asci( &tmpStr, Sn_ZZURI );
					if( pShLstVal->val.Compare(tmpStr) ){
						isChg_ZZURI = TRUE;
					}
					pShLstVal->val = tmpStr;
//---------------
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("ABC00070") ){	// íÜä‘î[ïtè˜ìnäÑäzÅiínï˚è¡îÔê≈Åj
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_TTYWAR );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("ABC00080") ){	// î[ïtè˜ìnäÑäzÅiínï˚è¡îÔê≈Åj
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_TEDNOF );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("ABC00110") ){	// ä˘ämíËè˜ìnäÑäz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_TKAKTZ );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("ABE00030") ){	// ëÊÇPéÌîÑè„çÇ
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_KURI1 );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("ABE00050") ){	// ëÊÇQéÌîÑè„çÇ
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_KURI2 );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("ABE00070") ){	// ëÊÇRéÌîÑè„çÇ
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_KURI3 );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("ABE00090") ){	// ëÊÇSéÌîÑè„çÇ
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_KURI4 );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("ABE00110") ){	// ëÊÇTéÌîÑè„çÇ
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_KURI5 );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("ABH00010") ){	// ëOîNÇÃâ€ê≈îÑè„çÇ
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_ZURI );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("ABH00020") ){	// ëOÅXâÒî[ïtê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_ZZNOFZ );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("ABH00030") ){	// ëOâÒî[ïtê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_ZNOFZ );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
			}

			// çXêVÉfÅ[É^ämîF
			int	adCnt = 0;
			for( int i=0;; i++ ){
				if( strlen(tagDataKani[i].tagName) == 0 ){
					break;
				}
				adCnt++;
			}
			int updCnt = (int)updTagAry.GetCount();
			if( adCnt != updCnt ){
				// ñ≥Ç¢Ç‡ÇÃÇí«â¡
				CArray<unsigned char*>	MonyPtrAry;
				MonyPtrAry.Add( Sn_KSTD );
				MonyPtrAry.Add( Sn_SYTX );
				MonyPtrAry.Add( Sn_KJADJZ );
				MonyPtrAry.Add( Sn_SIREZ );
				MonyPtrAry.Add( Sn_HKANZ );
				MonyPtrAry.Add( Sn_KSITZ );
				MonyPtrAry.Add( Sn_TYNOFZ );
				MonyPtrAry.Add( Sn_EDNOFZ );
				MonyPtrAry.Add( Sn_KAKTIZ );
				MonyPtrAry.Add( Sn_KZURI );
				MonyPtrAry.Add( Sn_ZZURI );
				MonyPtrAry.Add( Sn_TTYWAR );
				MonyPtrAry.Add( Sn_TTYWAR );
				MonyPtrAry.Add( Sn_TEDNOF );
				MonyPtrAry.Add( Sn_TKAKTZ );
				MonyPtrAry.Add( Sn_KURI1 );
				MonyPtrAry.Add( Sn_KURI2 );
				MonyPtrAry.Add( Sn_KURI3 );
				MonyPtrAry.Add( Sn_KURI4 );
				MonyPtrAry.Add( Sn_KURI5 );
				MonyPtrAry.Add( Sn_ZURI );
				MonyPtrAry.Add( Sn_ZZNOFZ );
				MonyPtrAry.Add( Sn_ZNOFZ );

				int	aryMax = (int)MonyPtrAry.GetCount();

				BOOL findFlg = FALSE;
				for( int i=0;; i++ ){
					if( strlen(tagDataKani[i].tagName) == 0 ){
						break;
					}

					findFlg = FALSE;
					for( int j=0; j<updCnt; j++ ){
						if( updTagAry[j].CompareNoCase(tagDataKani[i].tagName) == 0 ){
							findFlg = TRUE;
							break;
						}
					}
					if( findFlg == FALSE ){
						// äYìñÉ^ÉOÇÃí«â¡
						if( i < aryMax ){
							pShLstVal->AddNew();
							pShLstVal->sn_seq = sn_seq;
							pShLstVal->version = nowVersion;
							pShLstVal->hyoId = pHyoID;
							pShLstVal->itmId = tagDataKani[i].tagName;
							m_Util.val_to_asci( &pShLstVal->val, MonyPtrAry[i] );
//							if( tagDataIppan[i].tagName == _T("AEG00040") ){
//								pShLstVal->sign1 = Sn_2F21sw;
//							}
//							else{
//								pShLstVal->sign1 = 0;
//							}
							pShLstVal->Update();
						}
					}
				}
			}

		}
	}

//---> '15.11.21 INS START
	// ï â€ê≈ï˚éÆÇÃÉfÅ[É^Ç™Ç†ÇÈéûÇ…ÇÕÅAÇªÇøÇÁÇ…Ç‡ÉfÅ[É^ÇèëÇ´çûÇﬁ
	if( KzHoushiki == ID_ICSSH_GENSOKU ){
		filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_KANNI_HONPYO_HYOID, nowVersion);
	}
	else if( KzHoushiki == ID_ICSSH_KANNI ){
		filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_IPPAN_HONPYO_HYOID, nowVersion);
	}
	
	if( m_DBSyzAccess.m_pSH_LST_VAL ){
		m_DBSyzAccess.m_pSH_LST_VAL->Requery( filter );
	}
	pShLstVal = m_DBSyzAccess.m_pSH_LST_VAL;

	if( pShLstVal->st != -1 ){
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

			if( pShLstVal->itmId == _T("AAF00000") ){		// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_ZZURI );
				pShLstVal->Update();
			}
			else if( pShLstVal->itmId == _T("AAF00010") ){	// ëOîNÇÃâ€ê≈îÑè„çÇ
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_ZURI );
				pShLstVal->Update();
			}
			else if( pShLstVal->itmId == _T("AAF00020") ){	// ëOÅXâÒî[ïtê≈äz
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_ZZNOFZ );
				pShLstVal->Update();
			}
			else if( pShLstVal->itmId == _T("AAF00030") ){	// ëOâÒî[ïtê≈äz
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_ZNOFZ );
				pShLstVal->sign1 = Sn_ZNOsw;
				pShLstVal->Update();
			}
			else if( pShLstVal->itmId == _T("ABB00180") ){	// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_ZZURI );
				pShLstVal->Update();
			}
			else if( pShLstVal->itmId == _T("ABH00010") ){	// ëOîNÇÃâ€ê≈îÑè„çÇ
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_ZURI );
				pShLstVal->Update();
			}
			else if( pShLstVal->itmId == _T("ABH00020") ){	// ëOÅXâÒî[ïtê≈äz
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_ZZNOFZ );
				pShLstVal->Update();
			}
			else if( pShLstVal->itmId == _T("ABH00030") ){	// ëOâÒî[ïtê≈äz
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_ZNOFZ );
				pShLstVal->Update();
			}
		}
	}
//<--- '15.11.21 INS END


	if( KzHoushiki == ID_ICSSH_KANNI ){
		filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_KANNI_HONPYO_HYOID, nowVersion );

		if( m_DBSyzAccess.m_pSH_LST_RATE ){
			m_DBSyzAccess.m_pSH_LST_RATE->Requery( filter );
		}
		else{
			if( m_DBSyzAccess.OpenShLstRate(pDBNpSub->m_database, filter) ){
				return -1;
			}
		}

		CSH_LST_RATE	*pShLstRate = m_DBSyzAccess.m_pSH_LST_RATE;

		if( (pShLstRate->MoveFirst()==-1) || (pShLstRate->st==-1) ){	// ÉfÅ[É^ñ≥Çµ
			// ëÊàÍéÌéñã∆Å@îÑè„äÑçá
			pShLstRate->AddNew();
			pShLstRate->sn_seq = sn_seq;
			pShLstRate->version = nowVersion;
			pShLstRate->hyoId = pHyoID;
			pShLstRate->itmId = _T("ABE00040");
			pShLstRate->rate = Sn_UP1;
			pShLstRate->sign1 = 0;
			pShLstRate->Update();

			// ëÊìÒéÌéñã∆Å@îÑè„äÑçá
			pShLstRate->AddNew();
			pShLstRate->sn_seq = sn_seq;
			pShLstRate->version = nowVersion;
			pShLstRate->hyoId = pHyoID;
			pShLstRate->itmId = _T("ABE00060");
			pShLstRate->rate = Sn_UP2;
			pShLstRate->sign1 = 0;
			pShLstRate->Update();

			// ëÊéOéÌéñã∆Å@îÑè„äÑçá
			pShLstRate->AddNew();
			pShLstRate->sn_seq = sn_seq;
			pShLstRate->version = nowVersion;
			pShLstRate->hyoId = pHyoID;
			pShLstRate->itmId = _T("ABE00080");
			pShLstRate->rate = Sn_UP3;
			pShLstRate->sign1 = 0;
			pShLstRate->Update();

			// ëÊéléÌéñã∆Å@îÑè„äÑçá
			pShLstRate->AddNew();
			pShLstRate->sn_seq = sn_seq;
			pShLstRate->version = nowVersion;
			pShLstRate->hyoId = pHyoID;
			pShLstRate->itmId = _T("ABE00100");
			pShLstRate->rate = Sn_UP4;
			pShLstRate->sign1 = 0;
			pShLstRate->Update();

			// ëÊå‹éÌéñã∆Å@îÑè„äÑçá
			pShLstRate->AddNew();
			pShLstRate->sn_seq = sn_seq;
			pShLstRate->version = nowVersion;
			pShLstRate->hyoId = pHyoID;
			pShLstRate->itmId = _T("ABE00120");
			pShLstRate->rate = Sn_UP5;
			pShLstRate->sign1 = 0;
			pShLstRate->Update();
		}
		else{
			for( int i=0;; i++ ){
				if( i==0 ){
					if( (pShLstRate->MoveFirst()==-1) || (pShLstRate->st==-1) ){
						break;
					}
				}
				else{
					if( (pShLstRate->MoveNext()==-1) || (pShLstRate->st==-1) ){
						break;
					}
				}

				// ëÊàÍéÌéñã∆Å@îÑè„äÑçá
				if( pShLstRate->itmId == _T("ABE00040") ){
					pShLstRate->Edit();
					pShLstRate->rate = Sn_UP1;
					pShLstRate->Update();
				}
				// ëÊìÒéÌéñã∆Å@îÑè„äÑçá
				else if( pShLstRate->itmId == _T("ABE00060") ){
					pShLstRate->Edit();
					pShLstRate->rate = Sn_UP2;
					pShLstRate->Update();
				}
				// ëÊàÍéÌéñã∆Å@îÑè„äÑçá
				else if( pShLstRate->itmId == _T("ABE00080") ){
					pShLstRate->Edit();
					pShLstRate->rate = Sn_UP3;
					pShLstRate->Update();
				}
				// ëÊìÒéÌéñã∆Å@îÑè„äÑçá
				else if( pShLstRate->itmId == _T("ABE00100") ){
					pShLstRate->Edit();
					pShLstRate->rate = Sn_UP4;
					pShLstRate->Update();
				}
				// ëÊìÒéÌéñã∆Å@îÑè„äÑçá
				else if( pShLstRate->itmId == _T("ABE00120") ){
					pShLstRate->Edit();
					pShLstRate->rate = Sn_UP5;
					pShLstRate->Update();
				}
			}
		}
	}

	// à»â∫ÇÃå‹çÄñ⁄ÇÕÅAì«çûÇ›éûÇ…ÇÕêVãKÉeÅ[ÉuÉãÇ©ÇÁì«Ç›çûÇﬁ
	// èëçûÇ›ÇÕÅAêVãKÉeÅ[ÉuÉãÅ{ä˘ë∂ÉeÅ[ÉuÉã
//	Sn_TTYWAR;
//	Sn_TKAKTZ;
//	Sn_TEDNOF;
//	Sn_EDNOFZ;
//	Sn_TYNOFZ;
	if( !(wrtype&0x01) ){
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
			// êVãKçÏê¨
			pSZSNK->AddNew();
			// êVãKçÏê¨éûÇ…ÉfÉtÉHÉãÉgílÇÕâ∫ÇÃÇ‡ÇÃÇæÇØÇ≈OK??
		}
		else{
			// çXêV
			pSZSNK->Edit();
		}

		m_Util.val_to_asci( &pSZSNK->Sn_TTYWAR, Sn_TTYWAR );
		m_Util.val_to_asci( &pSZSNK->Sn_TKAKTZ, Sn_TKAKTZ );
		m_Util.val_to_asci( &pSZSNK->Sn_EDNOFZ, Sn_EDNOFZ );
		m_Util.val_to_asci( &pSZSNK->Sn_TEDNOF, Sn_TEDNOF );
		m_Util.val_to_asci( &pSZSNK->Sn_TYNOFZ, Sn_TYNOFZ );

//--> '15.01.14 INS START
		if( isChg_ZZURI ){
			m_Util.val_to_asci( &pSZSNK->Sn_ZZURI, Sn_ZZURI );
		}
//<-- '15.01.14 INS END

		pSZSNK->Update();
	}

#ifdef _DBG_VIEW_START_
	hoge.Format("CH26SnHonpyoData::SetData()__END\n");
	DbgViewTrace(hoge);
#endif

	return 0;
}

//-----------------------------------------------------------------------------
// èWåvçœÇ›ÉfÅ[É^ÇÃÉZÉbÉg
//-----------------------------------------------------------------------------
// à¯êî	pSyzSyukei	ÅF	è¡îÔê≈èWåvÉNÉâÉX
//-----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	ê≥èÌèIóπ
//			-1		ÅF	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CH26SnHonpyoData::SetCalqedData( CDBNpSub *pDBNpSub, CSyzSyukei *pSyzSyukei )
{
	MoneyBasejagArray money;

	EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki( pDBNpSub->zvol );
	if( KzHoushiki == ID_ICSSH_MENZEI ){
		return 0;
	}

	if( KzHoushiki == ID_ICSSH_GENSOKU ){
		money = pSyzSyukei->GetShinkokuData( _T("10") );

		int cnt = (int)money.GetCount();
		int incnt = 0;

		for( int i=0; i<cnt; i++ ){
			incnt = (int)money[i].GetCount();
			for( int j=0; j<incnt; j++ ){
				if( money[i][j].code == _T("100100") ){
					memmove( Sn_KSTD, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("100200") ){
					memmove( Sn_SYTX, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("100300") ){
					memmove( Sn_KJADJZ, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("100400") ){
					memmove( Sn_SIREZ, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("100500") ){
					memmove( Sn_HKANZ, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("100600") ){
					memmove( Sn_KSITZ, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("100700") ){
				}
				else if( money[i][j].code == _T("100800") ){
					;
				}
				else if( money[i][j].code == _T("100900") ){
					;
				}
				else if( money[i][j].code == _T("101000") ){
//					memmove( Sn_TYNOFZ, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("101100") ){
				}
				else if( money[i][j].code == _T("101200") ){
	//				memmove( Sn_EDNOFZ, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("101300") ){
	// '14.03.07ÅFä˘ämíËê≈äzÇÕåvéZçÄñ⁄Ç≈ÇÕÇ»Ç¢ÇÃÇ≈ïsóvÅI
	//				memmove( Sn_KAKTIZ, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("101400") ){
					;
				}
				else if( money[i][j].code == _T("101500") ){
	//				memmove( Sn_KAKTIZ, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("101600") ){
	//				memmove( Sn_TTYWAR, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("101700") ){
	//				memmove( Sn_TKAKTZ, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("101800") ){
				}
				else if( money[i][j].code == _T("101900") ){
	//				memmove( Sn_TEDNOF, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("102000") ){
				}
				else if( money[i][j].code == _T("102100") ){
				}
				else if( money[i][j].code == _T("102200") ){
				}
				else if( money[i][j].code == _T("102300") ){
				}
				else if( money[i][j].code == _T("102400") ){
				}
				else if( money[i][j].code == _T("102500") ){
				}
				else if( money[i][j].code == _T("102600") ){
				}
			}
		}

/*		money = pSyzSyukei->GetShinkokuData( _T("15") );

		cnt = (int)money.GetCount();
		for( int i=0; i<cnt; i++ ){
			incnt = (int)money[i].GetCount();
			for( int j=0; j<incnt; j++ ){
				// éñã∆ï â€ê≈îÑè„çÇ
				if( money[i][j].code == _T("150700") ){
					memmove( Sn_KURI1, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("150800") ){
					memmove( Sn_KURI2, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("150900") ){
					memmove( Sn_KURI3, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("151000") ){
					memmove( Sn_KURI4, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("151100") ){
					memmove( Sn_KURI5, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("151100") ){
					memmove( Sn_KURI5, money[i][j].arith, MONY_BUF_SIZE );
				}
				// éñã∆ï â€ê≈îÑè„äÑçá
				else if( money[i][j].code == _T("153100") ){
					Sn_UP1 = (unsigned short)m_Util.val_to_int( (unsigned char*)money[i][j].arith );
				}
				else if( money[i][j].code == _T("153200") ){
					Sn_UP2 = (unsigned short)m_Util.val_to_int( (unsigned char*)money[i][j].arith );
				}
				else if( money[i][j].code == _T("153300") ){
					Sn_UP3 = (unsigned short)m_Util.val_to_int( (unsigned char*)money[i][j].arith );
				}
				else if( money[i][j].code == _T("153400") ){
					Sn_UP4 = (unsigned short)m_Util.val_to_int( (unsigned char*)money[i][j].arith );
				}
				else if( money[i][j].code == _T("153500") ){
					Sn_UP5 = (unsigned short)m_Util.val_to_int( (unsigned char*)money[i][j].arith );
				}
			}
		}*/
	}
	else if( KzHoushiki == ID_ICSSH_KANNI ){
		money = pSyzSyukei->GetShinkokuData( _T("13") );

		int cnt = (int)money.GetCount();
		int incnt = 0;

		for( int i=0; i<cnt; i++ ){
			incnt = (int)money[i].GetCount();
			for( int j=0; j<incnt; j++ ){
				if( money[i][j].code == _T("130100") ){
					memmove( Sn_KSTD, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("130200") ){
					memmove( Sn_SYTX, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("130300") ){
					memmove( Sn_KJADJZ, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("130400") ){
					memmove( Sn_SIREZ, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("130500") ){
					memmove( Sn_HKANZ, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("100600") ){
					memmove( Sn_KSITZ, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("100700") ){
					;
				}
				else if( money[i][j].code == _T("100800") ){
					;
				}
				else if( money[i][j].code == _T("100900") ){
					;
				}
				else if( money[i][j].code == _T("131000") ){
//					memmove( Sn_TYNOFZ, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("131100") ){
				}
				else if( money[i][j].code == _T("131200") ){
//					memmove( Sn_EDNOFZ, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("131300") ){
//					memmove( Sn_KAKTIZ, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("131400") ){
					;
				}
				else if( money[i][j].code == _T("131500") ){
					memmove( Sn_KZURI, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("131600") ){
//					memmove( Sn_TTYWAR, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("131700") ){
//					memmove( Sn_TKAKTZ, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("131800") ){
				}
				else if( money[i][j].code == _T("131900") ){
//					memmove( Sn_TEDNOF, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("132000") ){
				}
				else if( money[i][j].code == _T("132100") ){
				}
				else if( money[i][j].code == _T("132200") ){
				}
				else if( money[i][j].code == _T("132300") ){
				}
				else if( money[i][j].code == _T("132400") ){
				}
				else if( money[i][j].code == _T("132500") ){
				}
				else if( money[i][j].code == _T("132600") ){
				}
			}
		}

		money = pSyzSyukei->GetShinkokuData( _T("15") );

		cnt = (int)money.GetCount();
		for( int i=0; i<cnt; i++ ){
			incnt = (int)money[i].GetCount();
			for( int j=0; j<incnt; j++ ){
				// éñã∆ï â€ê≈îÑè„çÇ
				if( money[i][j].code == _T("150700") ){
					memmove( Sn_KURI1, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("150800") ){
					memmove( Sn_KURI2, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("150900") ){
					memmove( Sn_KURI3, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("151000") ){
					memmove( Sn_KURI4, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("151100") ){
					memmove( Sn_KURI5, money[i][j].arith, MONY_BUF_SIZE );
				}
				else if( money[i][j].code == _T("151100") ){
					memmove( Sn_KURI5, money[i][j].arith, MONY_BUF_SIZE );
				}
				// éñã∆ï â€ê≈îÑè„äÑçá
				else if( money[i][j].code == _T("153100") ){
					Sn_UP1 = (unsigned short)m_Util.val_to_int( (unsigned char*)money[i][j].arith );
				}
				else if( money[i][j].code == _T("153200") ){
					Sn_UP2 = (unsigned short)m_Util.val_to_int( (unsigned char*)money[i][j].arith );
				}
				else if( money[i][j].code == _T("153300") ){
					Sn_UP3 = (unsigned short)m_Util.val_to_int( (unsigned char*)money[i][j].arith );
				}
				else if( money[i][j].code == _T("153400") ){
					Sn_UP4 = (unsigned short)m_Util.val_to_int( (unsigned char*)money[i][j].arith );
				}
				else if( money[i][j].code == _T("153500") ){
					Sn_UP5 = (unsigned short)m_Util.val_to_int( (unsigned char*)money[i][j].arith );
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
int CH26SnHonpyoData::GetPastData( CDBNpSub *pDBNpSub, int sn_seq )
{
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

	char	*pHyoID = NULL;
	int		nowVersion = 1;
	if( KzHoushiki == ID_ICSSH_GENSOKU ){
		filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_IPPAN_HONPYO_HYOID, nowVersion );
		pHyoID = ICS_SN_IPPAN_HONPYO_HYOID;
	}
	else if( KzHoushiki == ID_ICSSH_KANNI ){
		filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_KANNI_HONPYO_HYOID, nowVersion );
		pHyoID = ICS_SN_KANNI_HONPYO_HYOID;
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
	}
	else{
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
				if( pShLstVal->itmId == _T("AAB00010") ){		// â€ê≈ïWèÄäz
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_KSTD;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AAB00020") ){	// è¡îÔê≈äz
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_SYTX;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AAB00030") ){	// çTèúâﬂëÂí≤êÆê≈äz
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_KJADJZ;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AAB00050") ){	// çTèúëŒè€édì¸ê≈äz
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_SIREZ;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AAB00060") ){	// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_HKANZ;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AAB00070") ){	// ë›ì|ÇÍÇ…åWÇÈê≈äz
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_KSITZ;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AAB00110") ){	// íÜä‘î[ïtê≈äz
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_TYNOFZ;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AAB00120") ){	// î[ïtê≈äz
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_EDNOFZ;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AAB00150") ){	// ä˘ämíËê≈äz
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_KAKTIZ;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AAB00180") ){	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_KZURI;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AAB00190") ){	// éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_SOURI;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AAC00070") ){	// íÜä‘î[ïtè˜ìnäÑäzÅiínï˚è¡îÔê≈Åj
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_TTYWAR;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AAC00080") ){	// î[ïtè˜ìnäÑäzÅiínï˚è¡îÔê≈Åj
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_TEDNOF;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AAC00110") ){	// ä˘ämíËè˜ìnäÑäz
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_TKAKTZ;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AAF00000") ){	// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_ZZURI;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AAF00010") ){	// ëOîNÇÃâ€ê≈îÑè„çÇ
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_ZURI;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AAF00020") ){	// ëOÅXâÒî[ïtê≈äz
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_ZZNOFZ;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AAF00030") ){	// ëOâÒî[ïtê≈äz
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_ZNOFZ;
					pShLstVal->sign1 = Sn_ZNOsw;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AAG00140") ){	// 1ÅE2ÇÃì‡ñÛ3çsñ⁄
//					pShLstVal->Edit();
//					pShLstVal->val = pSZSNK->Sn_TS_TDM1;
//					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AAG00150") ){
//					pShLstVal->Edit();
//					pShLstVal->val = pSZSNK->Sn_TS_SYT1;
//					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AAG00160") ){	// 17ÅE18ÇÃì‡ñÛ3çsñ⁄
//					pShLstVal->Edit();
//					pShLstVal->val = pSZSNK->Sn_TS_SYT2;
//					pShLstVal->Update();
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
				if( pShLstVal->itmId == _T("ABB00010") ){		// â€ê≈ïWèÄäz
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_KSTD;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ABB00020") ){	// è¡îÔê≈äz
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_SYTX;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ABB00030") ){	// ë›ì|âÒé˚Ç…åWÇÈè¡îÔê≈äz
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_KJADJZ;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ABB00050") ){	// çTèúëŒè€édì¸ê≈äz
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_SIREZ;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ABB00060") ){	// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_HKANZ;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AAB00070") ){	// ë›ì|ÇÍÇ…åWÇÈê≈äz
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_KSITZ;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ABB00110") ){	// íÜä‘î[ïtê≈äz
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_TYNOFZ;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ABB00120") ){	// î[ïtê≈äz
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_EDNOFZ;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ABB00150") ){	// ä˘ämíËê≈äz
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_KAKTIZ;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ABB00170") ){	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_KZURI;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ABB00180") ){	// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_ZZURI;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ABC00070") ){	// íÜä‘î[ïtè˜ìnäÑäzÅiínï˚è¡îÔê≈Åj
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_TTYWAR;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ABC00080") ){	// î[ïtè˜ìnäÑäzÅiínï˚è¡îÔê≈Åj
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_TEDNOF;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ABC00110") ){	// ä˘ämíËè˜ìnäÑäz
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_TKAKTZ;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ABE00030") ){	// ëÊÇPéÌîÑè„çÇ
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_KURI[0];
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ABE00050") ){	// ëÊÇQéÌîÑè„çÇ
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_KURI[1];
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ABE00070") ){	// ëÊÇRéÌîÑè„çÇ
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_KURI[2];
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ABE00090") ){	// ëÊÇSéÌîÑè„çÇ
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_KURI[3];
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ABE00110") ){	// ëÊÇTéÌîÑè„çÇ
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_KURI[4];
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ABH00010") ){	// ëOîNÇÃâ€ê≈îÑè„çÇ
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_ZURI;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ABH00020") ){	// ëOÅXâÒî[ïtê≈äz
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_ZZNOFZ;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ABH00030") ){	// ëOâÒî[ïtê≈äz
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_ZNOFZ;
					pShLstVal->Update();
				}
			}


			if( m_DBSyzAccess.m_pSH_LST_RATE ){
				m_DBSyzAccess.m_pSH_LST_RATE->Requery( filter );
			}
			else{
				if( m_DBSyzAccess.OpenShLstRate(pDBNpSub->m_database, filter) ){
					return -1;
				}
			}

			CSH_LST_RATE	*pShLstRate = m_DBSyzAccess.m_pSH_LST_RATE;

			for( int i=0;; i++ ){
				if( i==0 ){
					if( (pShLstRate->MoveFirst()==-1) || (pShLstRate->st==-1) ){
						break;
					}
				}
				else{
					if( (pShLstRate->MoveNext()==-1) || (pShLstRate->st==-1) ){
						break;
					}
				}
				
				if( pShLstRate->itmId == _T("ABE00040") ){
					pShLstRate->Edit();
					pShLstRate->rate = pSZSNK->Sn_5FUP[0];
					pShLstRate->Update();
				}
				else if( pShLstRate->itmId == _T("ABE00060") ){
					pShLstRate->Edit();
					pShLstRate->rate = pSZSNK->Sn_5FUP[1];
					pShLstRate->Update();
				}
				else if( pShLstRate->itmId == _T("ABE00080") ){
					pShLstRate->Edit();
					pShLstRate->rate = pSZSNK->Sn_5FUP[2];
					pShLstRate->Update();
				}
				else if( pShLstRate->itmId == _T("ABE00100") ){
					pShLstRate->Edit();
					pShLstRate->rate = pSZSNK->Sn_5FUP[3];
					pShLstRate->Update();
				}
				else if( pShLstRate->itmId == _T("ABE00120") ){
					pShLstRate->Edit();
					pShLstRate->rate = pSZSNK->Sn_5FUP[4];
					pShLstRate->Update();
				}
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// çXêVéûÇÃã‡äzÉNÉäÉA
//-----------------------------------------------------------------------------
int CH26SnHonpyoData::ClearDataByRenew( CDBNpSub *pDBNpSub, int type )
{
	CArith	arith(0x16);

	arith.l_clear( Sn_KSTDM );
	arith.l_clear( Sn_KSTD );
	arith.l_clear( Sn_TIKAM );
	arith.l_clear( Sn_TIKA );
	arith.l_clear( Sn_SYTXM );
	arith.l_clear( Sn_SYTX );
	arith.l_clear( Sn_KJADJZ );
	arith.l_clear( Sn_SIREZ );
	arith.l_clear( Sn_HKANZ );
	arith.l_clear( Sn_KSITZ );
	arith.l_clear( Sn_TYNOFZ );
	arith.l_clear( Sn_TTYWAR );
	arith.l_clear( Sn_EDNOFZ );
	arith.l_clear( Sn_KAKTIZ );
	arith.l_clear( Sn_KZURI );
	arith.l_clear( Sn_SOURI );

	Sn_UP1 = 0;
	Sn_UP2 = 0;
	Sn_UP3 = 0;
	Sn_UP4 = 0;
	Sn_UP5 = 0;

	arith.l_clear( Sn_KURI1 );
	arith.l_clear( Sn_KURI2 );
	arith.l_clear( Sn_KURI3 );
	arith.l_clear( Sn_KURI4 );
	arith.l_clear( Sn_KURI5 );

	arith.l_clear( Sn_GENKAI );
	arith.l_clear( Sn_WGENKA );
	arith.l_clear( Sn_TEDNOF );
	arith.l_clear( Sn_TKAKTZ );

	// âﬂãéÉfÅ[É^ëSçÌèú
	
/*	CString	filter;
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
*/
/*	EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki( pDBNpSub->zvol );
	if( KzHoushiki == ID_ICSSH_MENZEI ){
		return 0;
	}

	CString	filter;
	char	*pHyoID = NULL;
	if( KzHoushiki == ID_ICSSH_GENSOKU ){
		filter.Format( _T("sn_seq = %d and hyoId = \'%s\'"), 0, ICS_SN_IPPAN_HONPYO_HYOID );
		pHyoID = ICS_SN_IPPAN_HONPYO_HYOID;
	}
	else if( KzHoushiki == ID_ICSSH_KANNI ){
		filter.Format( _T("sn_seq = %d and hyoId = \'%s\'"), 0, ICS_SN_KANNI_HONPYO_HYOID );
		pHyoID = ICS_SN_KANNI_HONPYO_HYOID;
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
		if( KzHoushiki == ID_ICSSH_GENSOKU ){
			if( (pShLstVal->itmId==_T("AAB00010")) ||	// â€ê≈ïWèÄäz
				(pShLstVal->itmId==_T("AAB00020")) ||	// è¡îÔê≈äz
				(pShLstVal->itmId==_T("AAB00030")) ||	// çTèúâﬂëÂí≤êÆê≈äz
				(pShLstVal->itmId==_T("AAB00050")) ||	// çTèúëŒè€édì¸ê≈äz
				(pShLstVal->itmId==_T("AAB00060")) ||	// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
				(pShLstVal->itmId==_T("AAB00070")) ||	// ë›ì|ÇÍÇ…åWÇÈê≈äz
				(pShLstVal->itmId==_T("AAB00110")) ||	// íÜä‘î[ïtê≈äz
				(pShLstVal->itmId==_T("AAC00070")) ||	// íÜä‘î[ïtè˜ìnäÑäzÅiínï˚è¡îÔê≈Åj
				(pShLstVal->itmId==_T("AAB00120")) ||	// î[ïtê≈äz
				(pShLstVal->itmId==_T("AAB00150")) ||	// ä˘ämíËê≈äz
				(pShLstVal->itmId==_T("AAB00180")) ||	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
				(pShLstVal->itmId==_T("AAB00190")) ||	// éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
				(pShLstVal->itmId==_T("AAC00080")) ||	// î[ïtè˜ìnäÑäzÅiínï˚è¡îÔê≈Åj
				(pShLstVal->itmId==_T("AAC00110")) ||	// ä˘ämíËè˜ìnäÑäz
				(pShLstVal->itmId==_T("AAG00140")) ||	// 1ÅE2ÇÃì‡ñÛ3çsñ⁄
				(pShLstVal->itmId==_T("AAG00150")) ||
				(pShLstVal->itmId==_T("AAG00160")) ){	// 17ÅE18ÇÃì‡ñÛ3çsñ⁄
				pShLstVal->Edit();
				pShLstVal->val = _T("0");
				pShLstVal->sign1 = 0;
				pShLstVal->Update();
			}*/
/*			else if( pShLstVal->itmId == _T("AAF00000") ){	// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_ZZURI;
				pShLstVal->Update();
			}
			else if( pShLstVal->itmId == _T("AAF00010") ){	// ëOîNÇÃâ€ê≈îÑè„çÇ
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_ZURI;
				pShLstVal->Update();
			}
			else if( pShLstVal->itmId == _T("AAF00020") ){	// ëOÅXâÒî[ïtê≈äz
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_ZZNOFZ;
				pShLstVal->Update();
			}
			else if( pShLstVal->itmId == _T("AAF00030") ){	// ëOâÒî[ïtê≈äz
				pShLstVal->Edit();
				pShLstVal->val = pSZSNK->Sn_ZNOFZ;
				pShLstVal->Update();
			}*/
/*		}
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
				if( (pShLstVal->itmId==_T("ABB00010")) ||	// â€ê≈ïWèÄäz
					(pShLstVal->itmId==_T("ABB00020")) ||	// è¡îÔê≈äz
					(pShLstVal->itmId==_T("ABB00030")) ||	// ë›ì|âÒé˚Ç…åWÇÈè¡îÔê≈äz
					(pShLstVal->itmId==_T("ABB00050")) ||	// çTèúëŒè€édì¸ê≈äz
					(pShLstVal->itmId==_T("ABB00060")) ||	// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
					(pShLstVal->itmId==_T("AAB00070")) ||	// ë›ì|ÇÍÇ…åWÇÈê≈äz
					(pShLstVal->itmId==_T("ABB00110")) ||	// íÜä‘î[ïtê≈äz
					(pShLstVal->itmId==_T("ABC00070")) ||	// íÜä‘î[ïtè˜ìnäÑäzÅiínï˚è¡îÔê≈Åj
					(pShLstVal->itmId==_T("ABB00120")) ||	// î[ïtê≈äz
					(pShLstVal->itmId==_T("ABB00150")) ||	// ä˘ämíËê≈äz
					(pShLstVal->itmId==_T("ABB00170")) ||	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
					(pShLstVal->itmId==_T("ABB00180")) ||	// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
					(pShLstVal->itmId==_T("ABE00030")) ||	// ëÊÇPéÌîÑè„çÇ
					(pShLstVal->itmId==_T("ABE00050")) ||	// ëÊÇQéÌîÑè„çÇ
					(pShLstVal->itmId==_T("ABE00070")) ||	// ëÊÇRéÌîÑè„çÇ
					(pShLstVal->itmId==_T("ABE00090")) ||	// ëÊÇSéÌîÑè„çÇ
					(pShLstVal->itmId==_T("ABE00110")) ||	// ëÊÇTéÌîÑè„çÇ
					(pShLstVal->itmId==_T("ABC00080")) ||	// î[ïtè˜ìnäÑäzÅiínï˚è¡îÔê≈Åj
					(pShLstVal->itmId==_T("ABC00110")) ){	// ä˘ämíËè˜ìnäÑäz

					pShLstVal->Edit();
					pShLstVal->val = _T("0");
					pShLstVal->sign1 = 0;
					pShLstVal->Update();
				}*/
	/*			else if( pShLstVal->itmId == _T("ABH00010") ){	// ëOîNÇÃâ€ê≈îÑè„çÇ
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_ZURI;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ABH00020") ){	// ëOÅXâÒî[ïtê≈äz
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_ZZNOFZ;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ABH00030") ){	// ëOâÒî[ïtê≈äz
					pShLstVal->Edit();
					pShLstVal->val = pSZSNK->Sn_ZNOFZ;
					pShLstVal->Update();
				}*/
/*			}
		}
	}*/

	return 0;
}

//-----------------------------------------------------------------------------
// éËì¸óÕÉTÉCÉìÇÃÉNÉäÉA
//-----------------------------------------------------------------------------
// ï‘ëóíl	0	ÅF	ê≥èÌèIóπ
//-----------------------------------------------------------------------------
int CH26SnHonpyoData::ClearManualInputSign()
{
	Sn_SYTsw = 0;

	return 0;
}

//-----------------------------------------------------------------------------
// çXêVéûÇÃã‡äzí«â¡èëÇ´çûÇ›('15.01.24)
//-----------------------------------------------------------------------------
// à¯êî	pDBNpSub	ÅF	ç‡ñ±ÉNÉâÉXÇÃÉ|ÉCÉìÉ^
//		sn_seq		ÅF	ëŒè€ÇÃóöóÉVÅ[ÉPÉìÉX
//-----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	ê≥èÌèIóπ
//-----------------------------------------------------------------------------
int CH26SnHonpyoData::SetDataInRenew( CDBNpSub *pDBNpSub, int sn_seq )
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
		filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_KANNI_HONPYO_HYOID, nowVersion );
		pHyoID = ICS_SN_KANNI_HONPYO_HYOID;
	}
	else if( KzHoushiki == ID_ICSSH_KANNI ){
		filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_IPPAN_HONPYO_HYOID, nowVersion );
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

	if( (pShLstVal->MoveFirst()==-1) || (pShLstVal->st==-1) ){	// ÉfÅ[É^ñ≥Çµ

		if( KzHoushiki == ID_ICSSH_GENSOKU ){
			// éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABH00010");
			m_Util.val_to_asci( &pShLstVal->val, Sn_ZURI );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ëOÅXâÒî[ïtê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABH00020");
			m_Util.val_to_asci( &pShLstVal->val, Sn_ZZNOFZ );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABB00180");
			m_Util.val_to_asci( &pShLstVal->val, Sn_ZZURI );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ëOâÒî[ïtê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABH00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_ZNOFZ );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();
		}
		else if( KzHoushiki == ID_ICSSH_KANNI ){
			// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAF00000");
			m_Util.val_to_asci( &pShLstVal->val, Sn_ZZURI );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ëOÅXâÒî[ïtê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAF00020");
			m_Util.val_to_asci( &pShLstVal->val, Sn_ZZNOFZ );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();
		
			// ëOîNÇÃâ€ê≈îÑè„çÇ
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAF00010");
			m_Util.val_to_asci( &pShLstVal->val, Sn_ZURI );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ëOâÒî[ïtê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAF00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_ZNOFZ );
			pShLstVal->sign1 = Sn_ZNOsw;
			pShLstVal->Update();
		}
	}
	else{	// ÉfÅ[É^óL
		if( KzHoushiki == ID_ICSSH_GENSOKU ){
			CStringArray	updTagAry;
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

				// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
				if( pShLstVal->itmId == _T("ABB00180") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_ZZURI );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// ëOÅXâÒî[ïtê≈äz
				else if( pShLstVal->itmId == _T("ABH00020") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_ZZNOFZ );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// ëOîNÇÃâ€ê≈îÑè„çÇ
				else if( pShLstVal->itmId == _T("ABH00010") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_ZURI );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// ëOâÒî[ïtê≈äz
				else if( pShLstVal->itmId == _T("ABH00030") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_ZNOFZ );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// íÜä‘î[ïtê≈äz
				else if( pShLstVal->itmId == _T("ABB00110") ){
					pShLstVal->Edit();
					pShLstVal->val = _T("0");
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// ínï˚è¡îÔê≈Å@íÜä‘î[ïtè˜ìnäÑäz
				else if( pShLstVal->itmId == _T("ABC00070") ){
					pShLstVal->Edit();
					pShLstVal->val = _T("0");
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// â€ê≈ïWèÄäz
				else if( pShLstVal->itmId == _T("ABB00010") ){
					pShLstVal->Edit();
					pShLstVal->val = _T("0");
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// è¡îÔê≈äz
				else if( pShLstVal->itmId == _T("ABB00020") ){
					pShLstVal->Edit();
					pShLstVal->val = _T("0");
					pShLstVal->sign1 = 0;
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// Ç±ÇÃâ€ê≈ä˙ä‘ÇÃâ€ê≈îÑè„çÇ
				else if( pShLstVal->itmId == _T("ABB00170") ){
					pShLstVal->Edit();
					pShLstVal->val = _T("0");
					pShLstVal->sign1 = 0;
					pShLstVal->Update();
				}
			}
			// çXêVÉfÅ[É^ämîF
			int	adCnt = 0;
			for( int i=0;; i++ ){
				if( strlen(tagDataKaniInRenew[i].tagName) == 0 ){
					break;
				}
				adCnt++;
			}
			int updCnt = (int)updTagAry.GetCount();
			if( adCnt != updCnt ){
				// ñ≥Ç¢Ç‡ÇÃÇí«â¡
				CArray<unsigned char*>	MonyPtrAry;
				MonyPtrAry.Add( Sn_ZZURI );
				MonyPtrAry.Add( Sn_ZZNOFZ );
				MonyPtrAry.Add( Sn_ZURI );
				MonyPtrAry.Add( Sn_ZNOFZ );

				int	aryMax = (int)MonyPtrAry.GetCount();
				BOOL findFlg = FALSE;
				for( int i=0;; i++ ){
					if( strlen(tagDataKaniInRenew[i].tagName) == 0 ){
						break;
					}

					findFlg = FALSE;
					for( int j=0; j<updCnt; j++ ){
						if( updTagAry[j].CompareNoCase(tagDataKaniInRenew[i].tagName) == 0 ){
							findFlg = TRUE;
							break;
						}
					}
					if( findFlg == FALSE ){
						// äYìñÉ^ÉOÇÃí«â¡
						if( i < aryMax ){
							pShLstVal->AddNew();
							pShLstVal->sn_seq = sn_seq;
							pShLstVal->version = nowVersion;
							pShLstVal->hyoId = pHyoID;
							pShLstVal->itmId = tagDataKaniInRenew[i].tagName;
							m_Util.val_to_asci( &pShLstVal->val, MonyPtrAry[i] );
							pShLstVal->Update();
						}
					}
				}
			}
		}
		else if( KzHoushiki == ID_ICSSH_KANNI ){
			CStringArray	updTagAry;
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

				// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
				if( pShLstVal->itmId == _T("AAF00000") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_ZZURI );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// ëOÅXâÒî[ïtê≈äz
				else if( pShLstVal->itmId == _T("AAF00020") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_ZZNOFZ );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// ëOîNÇÃâ€ê≈îÑè„çÇ
				else if( pShLstVal->itmId == _T("AAF00010") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_ZURI );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// ëOâÒî[ïtê≈äz
				else if( pShLstVal->itmId == _T("AAF00030") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_ZNOFZ );
					pShLstVal->sign1 = Sn_ZNOsw;
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// íÜä‘î[ïtê≈äz
				else if( pShLstVal->itmId == _T("AAB00110") ){
					pShLstVal->Edit();
					pShLstVal->val = _T("0");
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// ínï˚è¡îÔê≈Å@íÜä‘î[ïtè˜ìnäÑäz
				else if( pShLstVal->itmId == _T("AAC00070") ){
					pShLstVal->Edit();
					pShLstVal->val = _T("0");
					pShLstVal->sign1 = 0;
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// â€ê≈ïWèÄäz
				else if( pShLstVal->itmId == _T("AAB00010") ){
					pShLstVal->Edit();
					pShLstVal->val = _T("0");
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// è¡îÔê≈äz
				else if( pShLstVal->itmId == _T("AAB00020") ){
					pShLstVal->Edit();
					pShLstVal->val = _T("0");
					pShLstVal->sign1 = 0;
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
			}
			// çXêVÉfÅ[É^ämîF
			int	adCnt = 0;
			for( int i=0;; i++ ){
				if( strlen(tagDataIppanInRenew[i].tagName) == 0 ){
					break;
				}
				adCnt++;
			}
			int updCnt = (int)updTagAry.GetCount();
			if( adCnt != updCnt ){
				CArray<unsigned char*>	MonyPtrAry;
				MonyPtrAry.Add( Sn_ZZURI );
				MonyPtrAry.Add( Sn_ZZNOFZ );
				MonyPtrAry.Add( Sn_ZURI );
				MonyPtrAry.Add( Sn_ZNOFZ );

				int	aryMax = (int)MonyPtrAry.GetCount();
				BOOL findFlg = FALSE;
				for( int i=0;; i++ ){
					if( strlen(tagDataIppanInRenew[i].tagName) == 0 ){
						break;
					}
					findFlg = FALSE;
					for( int j=0; j<updCnt; j++ ){
						if( updTagAry[j].CompareNoCase(tagDataIppanInRenew[i].tagName) == 0 ){
							findFlg = TRUE;
							break;
						}
					}
					if( findFlg == FALSE ){
						// äYìñÉ^ÉOÇÃí«â¡
						if( i < aryMax ){
							pShLstVal->AddNew();
							pShLstVal->sn_seq = sn_seq;
							pShLstVal->version = nowVersion;
							pShLstVal->hyoId = pHyoID;
							pShLstVal->itmId = tagDataIppanInRenew[i].tagName;
							m_Util.val_to_asci( &pShLstVal->val, MonyPtrAry[i] );
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
// ï ÉoÅ[ÉWÉáÉìÇ©ÇÁÇÃÉfÅ[É^ÇÃà¯Ç´åpÇ¨
//-----------------------------------------------------------------------------
// à¯êî
//		pDBNpSub	:	åˆâvÉNÉâÉX
//		sn_seq		:	óöóÉVÅ[ÉPÉìÉX
//		pSnHeadData	:	è¡îÔê≈ÉwÉbÉ_èÓïÒ
//		srcVer		:	à¯Ç´åpÇ¨å≥ÉoÅ[ÉWÉáÉì
//-----------------------------------------------------------------------------
// ï‘ëóíl
//			0		:	ê≥èÌèIóπ
//			-1		:	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CH26SnHonpyoData::TakeOverData(CDBNpSub *pDBNpSub, int sn_seq, CSnHeadData *pSnHeadData, int srcVer)
{
#ifdef _DBG_VIEW_START_
	CString	hoge;
	hoge.Format("CH26SnHonpyoData::TakeOverData()_START\n");
	DbgViewTrace(hoge);
#endif

	if( srcVer != ID_VER_SYZ31 ){
		return -1;
	}
	// à¯åpÇ¨å≥Ç∆ÇµÇƒÇPÇOÅìâ¸ê≥ÉfÅ[É^ÇµÇ©àµÇÌÇ»Ç¢

	ASSERT (pDBNpSub);
	if( pDBNpSub == NULL ) {
		return -1;
	}
	ASSERT (pDBNpSub->zvol);
	if( pDBNpSub->zvol == NULL ) {
		return -1;
	}
	ASSERT (pSnHeadData);
	if( pSnHeadData == NULL ) {
		return -1;
	}

	EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki (pDBNpSub->zvol);
	if( KzHoushiki == ID_ICSSH_MENZEI ) {
		return 0;
	}

	CString filter;
	int		dataVersion = 2;	// 10%â¸ê≥ÉfÅ[É^ÇÃÉoÅ[ÉWÉáÉì
	if( KzHoushiki == ID_ICSSH_GENSOKU ) {
		filter.Format (_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_IPPAN_HONPYO1_HYOID, dataVersion);
	}
	else if( KzHoushiki == ID_ICSSH_KANNI ) {
		filter.Format (_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_KANNI_HONPYO1_HYOID, dataVersion);
	}
	if( m_DBSyzAccess.m_pSH_LST_VAL ) {
		m_DBSyzAccess.m_pSH_LST_VAL->Requery (filter);
	}
	else {
		if( m_DBSyzAccess.OpenShLstVal (pDBNpSub->m_database, filter) ) {
			return -1;
		}
	}

	// íçà”ÅI
	// ÇWÅìë§Ç≈ÇPÇOÅìÇÃÉfÅ[É^Çì«Ç›çûÇﬁéûÇ…ÇÕÅAã‡äzÇ™ì¸Ç¡ÇƒÇ¢ÇÈèäÇ…ÅuÇOâ~ÅvÇ≈ÇÃèëÇ´í◊ÇµÇÕçsÇÌÇ»Ç¢ÅI
	// Ç±ÇÍÇ…ÇÊÇËÅAê≈äzóìÇÃã‡äzÇÅuÇOâ~ÅvÇ…ÇµÇΩÇÃÇ…ÅAÇªÇÃÉfÅ[É^Ç™à¯Ç´åpÇ™ÇÍÇ»Ç¢Ç∆Ç¢Ç§åªè€Ç™î≠ê∂Ç∑ÇÈéñÇ…Ç»ÇÈÇ™
	// ÇªÇÃólÇ»ÉPÅ[ÉXÇÕäFñ≥Ç≈Ç†ÇÎÇ§Ç∆åæÇ§ëOíÒÇ…óßÇ¡ÇƒÇ¢Ç‹Ç∑ÅB

	CSH_LST_VAL	*pShLstVal = m_DBSyzAccess.m_pSH_LST_VAL;

	for( int idx = 0; ; idx++ ) {
		if( idx == 0 ) {
			if( (pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1) ) {
				break;
			}
		}
		else {
			if( (pShLstVal->MoveNext() == -1) || (pShLstVal->st == -1) ) {
				break;
			}
		}
		if( KzHoushiki == ID_ICSSH_GENSOKU ) {
			// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
			if( pShLstVal->itmId == _T("AAM00030") ) {
				if( pShLstVal->val != _T("0") ){
					m_Util.val_to_bin (Sn_ZZURI, pShLstVal->val);
				}
			}
			// ëOÅXâÒî[ïtê≈äz
			if( pShLstVal->itmId == _T("AAF00020") ) {
				if( pShLstVal->val != _T("0") ){
					m_Util.val_to_bin (Sn_ZZNOFZ, pShLstVal->val);
				}
			}
			// ëOîNÇÃâ€ê≈îÑè„çÇ
			if( pShLstVal->itmId == _T("AAF00010") ) {
				if( pShLstVal->val != _T("0") ){
					m_Util.val_to_bin (Sn_ZURI, pShLstVal->val);
				}
			}
			// ëOâÒî[ïtê≈äz
			if( pShLstVal->itmId == _T("AAF00030") ) {
				if( pShLstVal->val != _T("0") ){
					m_Util.val_to_bin (Sn_ZNOFZ, pShLstVal->val);
				}
			}
		}
		else if( KzHoushiki == ID_ICSSH_KANNI ) {
			// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
			if( pShLstVal->itmId == _T("ABI00180") ) {
				if( pShLstVal->val != _T("0") ){
					m_Util.val_to_bin (Sn_ZZURI, pShLstVal->val);
				}
			}
			// ëOÅXâÒî[ïtê≈äz
			if( pShLstVal->itmId == _T("ABH00020") ) {
				if( pShLstVal->val != _T("0") ){
					m_Util.val_to_bin (Sn_ZZNOFZ, pShLstVal->val);
				}
			}
			// ëOîNÇÃâ€ê≈îÑè„çÇ
			if( pShLstVal->itmId == _T("ABH00010") ) {
				if( pShLstVal->val != _T("0") ){
					m_Util.val_to_bin (Sn_ZURI, pShLstVal->val);
				}
			}
			// ëOâÒî[ïtê≈äz
			if( pShLstVal->itmId == _T("ABH00030") ) {
				if( pShLstVal->val != _T("0") ){
					m_Util.val_to_bin (Sn_ZNOFZ, pShLstVal->val);
				}
			}
		}
	}

#ifdef _DBG_VIEW_START_
	hoge.Format("CH26SnHonpyoData::TakeOverData()_END\n");
	DbgViewTrace(hoge);
#endif

	return 0;
}


