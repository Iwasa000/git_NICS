#include "StdAfx.h"
#include "H31SnHonpyoData.h"

// SH_TAGNAME.sign1 ÇÃê≈ó¶ÉTÉCÉìÇëùÇ‚Ç∑ïKóvóL

// å¥ë•â€ê≈
static SH_TAGNAME	tagDataIppan[] = {
	{_T("AAJ00010"), 999 },
	{_T("AAJ00020"), 999 },
	{_T("AAJ00030"), 999 },
	{_T("AAJ00050"), 999 },
	{_T("AAJ00060"), 999 },
	{_T("AAJ00070"), 999 },
	{_T("AAJ00080"), 999 },
	{_T("AAJ00090"), 999 },
	{_T("AAJ00100"), 999 },
	{_T("AAJ00110"), 999 },
	{_T("AAJ00120"), 999 },
	{_T("AAJ00130"), 999 },
	{_T("AAJ00150"), 999 },
	{_T("AAJ00160"), 999 },
	{_T("AAJ00180"), 999 },
	{_T("AAJ00190"), 999 },
	{_T("AAK00020"), 999 },
	{_T("AAK00030"), 999 },
	{_T("AAK00050"), 999 },
	{_T("AAK00060"), 999 },
	{_T("AAK00070"), 999 },
	{_T("AAK00080"), 999 },
	{_T("AAK00090"), 999 },
	{_T("AAK00110"), 999 },
	{_T("AAK00120"), 999 },
	{_T("AAK00130"), 999 },
	{_T("AAM00030"), 999 },
	{_T("AAF00010"), 999 },
	{_T("AAF00020"), 999 },
	{_T("AAF00030"), 999 },
	{ _T( "FFF00000" ), 999 },	// â€ê≈ä˙ä‘Ç…Ç®ÇØÇÈâ€ê≈îÑè„çÇ

	{_T("") }
};

// ä»à’â€ê≈
static SH_TAGNAME	tagDataKani[] = {
	{_T("ABI00010"), 999 },
	{_T("ABI00020"), 999 },
	{_T("ABI00030"), 999 },
	{_T("ABI00050"), 999 },
	{_T("ABI00060"), 999 },
	{_T("ABI00070"), 999 },
	{_T("ABI00080"), 999 },
	{_T("ABI00090"), 999 },
	{_T("ABI00100"), 999 },
	{_T("ABI00110"), 999 },
	{_T("ABI00120"), 999 },
	{_T("ABI00130"), 999 },
	{_T("ABI00150"), 999 },
	{_T("ABI00160"), 999 },
	{_T("ABI00170"), 999 },
	{_T("ABI00180"), 999 },
	{_T("ABJ00020"), 999 },
	{_T("ABJ00030"), 999 },
	{_T("ABJ00050"), 999 },
	{_T("ABJ00060"), 999 },
	{_T("ABJ00070"), 999 },
	{_T("ABJ00080"), 999 },
	{_T("ABJ00090"), 999 },
	{_T("ABJ00110"), 999 },
	{_T("ABJ00120"), 999 },
	{_T("ABJ00130"), 999 },
	{_T("ABL00040"), 999 },
	{_T("ABL00070"), 999 },
	{_T("ABL00100"), 999 },
	{_T("ABL00130"), 999 },
	{_T("ABL00160"), 999 },
	{_T("ABL00190"), 999 },
	{_T("ABH00010"), 999 },
	{_T("ABH00020"), 999 },
	{_T("ABH00030"), 999 },

	{_T("") }
};

// å¥ë•â€ê≈(çXêVéûämîF)
static SH_TAGNAME	tagDataIppanInRenew[] ={
	{_T("AAM00030"), 999 },	// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
	{_T("AAF00020"), 999 },	// ëOÅXâÒî[ïtê≈äz
	{_T("AAF00010"), 999 },	// ëOîNÇÃâ€ê≈îÑè„çÇ
	{_T("AAF00030"), 999 },	// ëOâÒî[ïtê≈äz

	{_T("") }
};

// ä»à’â€ê≈(çXêVéûämîF)
static SH_TAGNAME	tagDataKaniInRenew[] ={
	{_T("ABI00180"), 999 },	// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
	{_T("ABH00020"), 999 },	// ëOÅXâÒî[ïtê≈äz
	{_T("ABH00010"), 999 },	// ëOîNÇÃâ€ê≈îÑè„çÇ
	{_T("ABH00030"), 999 },	// ëOâÒî[ïtê≈äz

	{_T("") }
};

CH31SnHonpyoData::CH31SnHonpyoData()
{
	memset ( Sn_KSTDM, '\0', MONY_BUF_SIZE );
	memset ( Sn_KSTD, '\0', MONY_BUF_SIZE );
	memset ( Sn_TIKAM, '\0', MONY_BUF_SIZE );
	memset ( Sn_TIKA, '\0', MONY_BUF_SIZE );
	memset ( Sn_SYTXM, '\0', MONY_BUF_SIZE );
	memset ( Sn_SYTX, '\0', MONY_BUF_SIZE );
	memset ( Sn_KJADJZ, '\0', MONY_BUF_SIZE );
	memset ( Sn_SIREZ, '\0', MONY_BUF_SIZE );
	memset ( Sn_HKANZ, '\0', MONY_BUF_SIZE );
	memset ( Sn_KSITZ, '\0', MONY_BUF_SIZE );
	memset ( Sn_TYNOFZ, '\0', MONY_BUF_SIZE );
	memset ( Sn_EDNOFZ, '\0', MONY_BUF_SIZE );
	memset ( Sn_KAKTIZ, '\0', MONY_BUF_SIZE );
	memset ( Sn_TTYWAR, '\0', MONY_BUF_SIZE );
	memset ( Sn_TKAKTZ, '\0', MONY_BUF_SIZE );
	memset ( Sn_TEDNOF, '\0', MONY_BUF_SIZE );
	memset ( Sn_KZURI, '\0', MONY_BUF_SIZE );
	memset ( Sn_SOURI, '\0', MONY_BUF_SIZE );
	memset ( Sn_ZZNOFZ, '\0', MONY_BUF_SIZE );
	memset ( Sn_ZNOFZ, '\0', MONY_BUF_SIZE );
	memset ( Sn_ZZURI, '\0', MONY_BUF_SIZE );
	memset ( Sn_ZURI, '\0', MONY_BUF_SIZE );
	memset ( Sn_KURI1, '\0', MONY_BUF_SIZE );
	memset ( Sn_KURI2, '\0', MONY_BUF_SIZE );
	memset ( Sn_KURI3, '\0', MONY_BUF_SIZE );
	memset ( Sn_KURI4, '\0', MONY_BUF_SIZE );
	memset ( Sn_KURI5, '\0', MONY_BUF_SIZE );
	memset ( Sn_KURI6, '\0', MONY_BUF_SIZE );
	memset ( Sn_GENKAI, '\0', MONY_BUF_SIZE );
	memset ( Sn_WGENKA, '\0', MONY_BUF_SIZE );
	memset ( Sn_TS_TDM1, '\0', MONY_BUF_SIZE );
	memset ( Sn_TS_SYT1, '\0', MONY_BUF_SIZE );
	memset ( Sn_TS_SYT2, '\0', MONY_BUF_SIZE );
	memset ( Sn_KSJT3, '\0', MONY_BUF_SIZE );
	memset ( Sn_KSJT4, '\0', MONY_BUF_SIZE );
	memset ( Sn_KSJT63, '\0', MONY_BUF_SIZE );
	memset ( Sn_KSJTKEI, '\0', MONY_BUF_SIZE );
	memset ( Sn_TKST63, '\0', MONY_BUF_SIZE );
	memset ( Sn_TKSTKEI, '\0', MONY_BUF_SIZE );
	memset ( Sn_URHKNZ, '\0', MONY_BUF_SIZE );
	memset ( Sn_TKHKNZ, '\0', MONY_BUF_SIZE );

	memset ( Sn_KJZSK, '\0', MONY_BUF_SIZE );
	memset ( Sn_KJHKZ, '\0', MONY_BUF_SIZE );
	memset ( Sn_SHZ, '\0', MONY_BUF_SIZE );
	memset ( Sn_TYNKNP, '\0', MONY_BUF_SIZE );
	memset ( Sn_SHN, '\0', MONY_BUF_SIZE );
	memset ( Sn_TKJHKZ, '\0', MONY_BUF_SIZE );
	memset ( Sn_TSHZ, '\0', MONY_BUF_SIZE );
	memset ( Sn_KNP, '\0', MONY_BUF_SIZE );
	memset ( Sn_NOZ, '\0', MONY_BUF_SIZE );
	memset ( Sn_TYNKNPJ, '\0', MONY_BUF_SIZE );
	memset ( Sn_SHNJ, '\0', MONY_BUF_SIZE );
	memset ( Sn_SYOTSG, '\0', MONY_BUF_SIZE );

	memset( Sn_KazeiUriage, '\0', MONY_BUF_SIZE );

	Sn_SYTsw = 0;
	Sn_UP1 = Sn_UP2 = Sn_UP3 = Sn_UP4 = Sn_UP5 = Sn_UP6 = 0;
	Sn_TS_KUBUN1 = Sn_TS_KUBUN2 = 0;
	Sn_ZNOsw = 0;
}

CH31SnHonpyoData::~CH31SnHonpyoData()
{
	if( m_DBSyzAccess.m_pSH_LST_VAL ){
		m_DBSyzAccess.CloseShLstVal();
	}
	if( m_DBSyzAccess.m_pSH_LST_RATE ){
		m_DBSyzAccess.CloseShLstRate();
	}
}

//-----------------------------------------------------------------------------
// ç≈èIÉfÅ[É^Ç∆ÇµÇƒÅAÇ«ÇøÇÁÇÃÉfÅ[É^Ç™écÇ¡ÇƒÇ¢ÇÈÅH
//-----------------------------------------------------------------------------
// à¯êî	pDBNpSub	ÅF	DBÇ…ê⁄ë±çœÇÃç‡ñ±ÉNÉâÉX
//		sn_seq		ÅF	ê\çêèëÉfÅ[É^ÉVÅ[ÉPÉìÉX
//		Kzhoushiki	ÅF	Ç«ÇøÇÁÇÃâ€ê≈ï˚éÆÉfÅ[É^Ç™écÇ¡ÇƒÇ¢ÇÈÅHÅiñﬂÇËílÅj
//-----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	ê≥èÌèIóπ
//			-1		ÅF	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CH31SnHonpyoData::ChkExistKzdata( CDBNpSub *pDBNpSub, int sn_seq, int& Kzhoushiki )
{
	ASSERT(pDBNpSub);
	if( pDBNpSub == NULL ){
		return -1;
	}

	Kzhoushiki = ID_ICSSH_MENZEI;

	CString	filter;
	int		nowVersion = 2;

	if( m_DBSyzAccess.m_pSH_LST_VAL == NULL ) {
		if( m_DBSyzAccess.OpenShLstVal(pDBNpSub->m_database) ){
			return -1;
		}
	}

	// å¥ë•â€ê≈ÇÃÉfÅ[É^É`ÉFÉbÉN
	filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_IPPAN_HONPYO1_HYOID, nowVersion);
	m_DBSyzAccess.m_pSH_LST_VAL->Requery(filter);
	if( m_DBSyzAccess.m_pSH_LST_VAL->GetRecordCount() > 0 ) {
		// å¥ë•â€ê≈Ç∆ÇµÇƒîªíf
		Kzhoushiki = ID_ICSSH_GENSOKU;
	}
	else {
		filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_KANNI_HONPYO1_HYOID, nowVersion);
		m_DBSyzAccess.m_pSH_LST_VAL->Requery(filter);
		if( m_DBSyzAccess.m_pSH_LST_VAL->GetRecordCount() > 0 ) {
			// ä»à’â€ê≈Ç∆ÇµÇƒîªíf
			Kzhoushiki = ID_ICSSH_KANNI;
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// DB Ç©ÇÁÉfÅ[É^ì«çûÇ›
//-----------------------------------------------------------------------------
// à¯êî	pDBNpSub	ÅF	DBÇ…ê⁄ë±çœÇÃç‡ñ±ÉNÉâÉX
//		sn_seq		ÅF	ê\çêèëÉfÅ[É^ÉVÅ[ÉPÉìÉX
//		rdtype		ÅF	èëçûÇ›É^ÉCÉv	Å†Å†Å†Å† Å†Å†Å†Å† Å†Å†Å†Å† Å†Å†Å†Å† Å†Å†Å†Å† Å†Å†Å†Å† Å†Å†Å†Å† Å†Å°Å°Å†
//						 D1ÅFñ∆ê≈éñã∆é“Ç≈Ç†Ç¡ÇƒÇ‡å¥ë•â€ê≈Ç∆ÇµÇƒÉfÅ[É^Çì«çûÇ›
//						 D2ÅFñ∆ê≈éñã∆é“Ç≈Ç†Ç¡ÇƒÇ‡ä»à’â€ê≈Ç∆ÇµÇƒÉfÅ[É^Çì«çûÇ›
//-----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	ê≥èÌèIóπ
//			-1		ÅF	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CH31SnHonpyoData::GetDataExtend(CDBNpSub *pDBNpSub, int sn_seq, int rdtype/*=0*/)
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
	if( KzHoushiki == ID_ICSSH_MENZEI ) {
		if( rdtype&0x02 ){
			KzHoushiki = ID_ICSSH_GENSOKU;
		}
		else if( rdtype&0x04 ){
			KzHoushiki = ID_ICSSH_KANNI;
		}
		else{
			return 0;
		}
	}

	return GetDataSub(pDBNpSub, sn_seq, KzHoushiki);
}

//-----------------------------------------------------------------------------
// DB Ç©ÇÁÉfÅ[É^ì«çûÇ›Åiñ{ëÃÅj
//-----------------------------------------------------------------------------
// à¯êî	pDBNpSub	ÅF	ç‡ñ±ÉNÉâÉXÇÃÉ|ÉCÉìÉ^
//		sn_seq		ÅF	ëŒè€ÇÃóöóÉVÅ[ÉPÉìÉX
//-----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	ê≥èÌèIóπ
//			-1		ÅF	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CH31SnHonpyoData::GetDataSub(CDBNpSub *pDBNpSub, int sn_seq, int KzHoushiki)
{
#ifdef _DBG_VIEW_START_
	CString	hoge;
	hoge.Format("CH31SnHonpyoData::GetDataSub()__START\n");
	DbgViewTrace(hoge);
#endif

	CString	filter;
	int		nowVersion = 2;
	if( KzHoushiki == ID_ICSSH_GENSOKU ) {
		filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_IPPAN_HONPYO1_HYOID, nowVersion);
	}
	else if( KzHoushiki == ID_ICSSH_KANNI ) {
		filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_KANNI_HONPYO1_HYOID, nowVersion);
	}

	if( m_DBSyzAccess.m_pSH_LST_VAL ) {
		m_DBSyzAccess.m_pSH_LST_VAL->Requery(filter);
	}
	else {
		if( m_DBSyzAccess.OpenShLstVal(pDBNpSub->m_database, filter) ) {
			return -1;
		}
	}

	CSH_LST_VAL	*pShLstVal = m_DBSyzAccess.m_pSH_LST_VAL;

	BOOL	isNodata = TRUE;
	if( KzHoushiki == ID_ICSSH_GENSOKU ) {
		for( int i = 0;; i++ ) {
			if( i == 0 ) {
				if( (pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1) ) {
					break;
				}
				isNodata = FALSE;
			}
			else {
				if( (pShLstVal->MoveNext() == -1) || (pShLstVal->st == -1) ) {
					break;
				}
			}

			// ã‡äzÇïœä∑ÇµÇƒë„ì¸
			if( pShLstVal->itmId == _T("AAJ00010") ) {		// â€ê≈ïWèÄäz
				m_Util.val_to_bin(Sn_KSTD, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("AAJ00020") ) {	// è¡îÔê≈äz
				m_Util.val_to_bin(Sn_SYTX, pShLstVal->val);
				Sn_SYTsw = pShLstVal->sign1;
			}
			else if( pShLstVal->itmId == _T("AAJ00030") ) {	// çTèúâﬂëÂí≤êÆê≈äz
				m_Util.val_to_bin(Sn_KJADJZ, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("AAJ00050") ) {	// çTèúëŒè€édì¸ê≈äz
				m_Util.val_to_bin(Sn_SIREZ, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("AAJ00060") ) {	// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
				m_Util.val_to_bin(Sn_HKANZ, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("AAJ00070") ) {	// ë›ì|ÇÍÇ…åWÇÈê≈äz
				m_Util.val_to_bin(Sn_KSITZ, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("AAJ00080") ) {	// çTèúê≈äzè¨åv
				m_Util.val_to_bin(Sn_KJZSK, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("AAJ00090") ) {	// çTèúïsë´ä“ïtê≈äz
				m_Util.val_to_bin(Sn_KJHKZ, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("AAJ00100") ) {	// ç∑à¯ê≈äz
				m_Util.val_to_bin(Sn_SHZ, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("AAJ00110") ) {	// íÜä‘î[ïtê≈äz
				m_Util.val_to_bin(Sn_TYNOFZ, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("AAJ00120") ) {	// î[ïtê≈äz
				m_Util.val_to_bin(Sn_EDNOFZ, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("AAJ00130") ) {	// íÜä‘î[ïtä“ïtê≈äz
				m_Util.val_to_bin(Sn_TYNKNP, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("AAJ00150") ) {	// ä˘ämíËê≈äz
				m_Util.val_to_bin(Sn_KAKTIZ, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("AAJ00160") ) {	// ç∑à¯î[ïtê≈äz
				m_Util.val_to_bin(Sn_SHN, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("AAJ00180") ) {	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
				m_Util.val_to_bin(Sn_KZURI, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("AAJ00190") ) {	// éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
				m_Util.val_to_bin(Sn_SOURI, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("AAK00020") ) {	// çTèúïsë´ä“ïtê≈äz
				m_Util.val_to_bin(Sn_TKJHKZ, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("AAK00030") ) {	// ç∑à¯ê≈äz
				m_Util.val_to_bin(Sn_TSHZ, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("AAK00050") ) {	// ä“ïtäz
				m_Util.val_to_bin(Sn_KNP, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("AAK00060") ) {	// î[ê≈äz
				m_Util.val_to_bin(Sn_NOZ, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("AAK00070") ) {	// íÜä‘î[ïtè˜ìnäÑäzÅiínï˚è¡îÔê≈Åj
				m_Util.val_to_bin(Sn_TTYWAR, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("AAK00080") ) {	// î[ïtè˜ìnäÑäzÅiínï˚è¡îÔê≈Åj
				m_Util.val_to_bin(Sn_TEDNOF, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("AAK00090") ) {	// íÜä‘î[ïtä“ïtè˜ìnäÑäz
				m_Util.val_to_bin(Sn_TYNKNPJ, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("AAK00110") ) {	// ä˘ämíËè˜ìnäÑäz
				m_Util.val_to_bin(Sn_TKAKTZ, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("AAK00120") ) {	// ç∑à¯î[ïtè˜ìnäÑäz
				m_Util.val_to_bin(Sn_SHNJ, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("AAK00130") ) {	// è¡îÔê≈ãyÇ—ínï˚è¡îÔê≈ÇÃçáåv(î[ïtñîÇÕä“ït)ê≈äz
				m_Util.val_to_bin(Sn_SYOTSG, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("AAM00030") ) {	// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
				m_Util.val_to_bin(Sn_ZZURI, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("AAF00010") ) {	// ëOîNÇÃâ€ê≈îÑè„çÇ
				m_Util.val_to_bin(Sn_ZURI, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("AAF00020") ) {	// ëOÅXâÒî[ïtê≈äz
				m_Util.val_to_bin(Sn_ZZNOFZ, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("AAF00030") ) {	// ëOâÒî[ïtê≈äz
				m_Util.val_to_bin(Sn_ZNOFZ, pShLstVal->val);
				Sn_ZNOsw = pShLstVal->sign1;
			}
			else if( pShLstVal->itmId == _T( "FFF00000" )) {	// â€ê≈ä˙ä‘Ç…Ç®ÇØÇÈâ€ê≈îÑè„çÇ
				m_Util.val_to_bin( Sn_KazeiUriage, pShLstVal->val );
			}
		}
	}
	else if( KzHoushiki == ID_ICSSH_KANNI ) {
		for( int i = 0;; i++ ) {
			if( i == 0 ) {
				if( (pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1) ) {
					break;
				}
				isNodata = FALSE;
			}
			else {
				if( (pShLstVal->MoveNext() == -1) || (pShLstVal->st == -1) ) {
					break;
				}
			}

			// ã‡äzÇïœä∑ÇµÇƒë„ì¸
			if( pShLstVal->itmId == _T("ABI00010") ) {		// â€ê≈ïWèÄäz
				m_Util.val_to_bin(Sn_KSTD, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("ABI00020") ) {	// è¡îÔê≈äz
				m_Util.val_to_bin(Sn_SYTX, pShLstVal->val);
				Sn_SYTsw = pShLstVal->sign1;
			}
			else if( pShLstVal->itmId == _T("ABI00030") ) {	// ë›ì|âÒé˚Ç…åWÇÈè¡îÔê≈äz
				m_Util.val_to_bin(Sn_KJADJZ, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("ABI00050") ) {	// çTèúëŒè€édì¸ê≈äz
				m_Util.val_to_bin(Sn_SIREZ, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("ABI00060") ) {	// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
				m_Util.val_to_bin(Sn_HKANZ, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("ABI00070") ) {	// ë›ì|ÇÍÇ…åWÇÈê≈äz
				m_Util.val_to_bin(Sn_KSITZ, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("ABI00080") ) {	// çTèúê≈äzè¨åv
				m_Util.val_to_bin(Sn_KJZSK, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("ABI00090") ) {	// çTèúïsë´ä“ïtê≈äz
				m_Util.val_to_bin(Sn_KJHKZ, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("ABI00100") ) {	// ç∑à¯ê≈äz
				m_Util.val_to_bin(Sn_SHZ, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("ABI00110") ) {	// íÜä‘î[ïtê≈äz
				m_Util.val_to_bin(Sn_TYNOFZ, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("ABI00120") ) {	// î[ïtê≈äz
				m_Util.val_to_bin(Sn_EDNOFZ, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("ABI00130") ) {	// íÜä‘î[ïtä“ïtê≈äz
				m_Util.val_to_bin(Sn_TYNKNP, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("ABI00150") ) {	// ä˘ämíËê≈äz
				m_Util.val_to_bin(Sn_KAKTIZ, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("ABI00160") ) {	// ç∑à¯î[ïtê≈äz
				m_Util.val_to_bin(Sn_SHN, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("ABI00170") ) {	// Ç±ÇÃâ€ê≈ä˙ä‘ÇÃâ€ê≈îÑè„çÇ
				m_Util.val_to_bin(Sn_KZURI, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("ABI00180") ) {	// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
				m_Util.val_to_bin(Sn_ZZURI, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("ABJ00020") ) {	// çTèúïsë´ä“ïtê≈äz
				m_Util.val_to_bin(Sn_TKJHKZ, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("ABJ00030") ) {	// ç∑à¯ê≈äz
				m_Util.val_to_bin(Sn_TSHZ, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("ABJ00050") ) {	// ä“ïtäz
				m_Util.val_to_bin(Sn_KNP, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("ABJ00060") ) {	// î[ê≈äz
				m_Util.val_to_bin(Sn_NOZ, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("ABJ00070") ) {	// íÜä‘î[ïtè˜ìnäÑäzÅiínï˚è¡îÔê≈Åj
				m_Util.val_to_bin(Sn_TTYWAR, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("ABJ00080") ) {	// î[ïtè˜ìnäÑäzÅiínï˚è¡îÔê≈Åj
				m_Util.val_to_bin(Sn_TEDNOF, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("ABJ00090") ) {	// íÜä‘î[ïtä“ïtè˜ìnäÑäz
				m_Util.val_to_bin(Sn_TYNKNPJ, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("ABJ00110") ) {	// ä˘ämíËè˜ìnäÑäz
				m_Util.val_to_bin(Sn_TKAKTZ, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("ABJ00120") ) {	// ç∑à¯î[ïtè˜ìnäÑäz
				m_Util.val_to_bin(Sn_SHNJ, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("ABJ00130") ) {	// è¡îÔê≈ãyÇ—ínï˚è¡îÔê≈ÇÃçáåv(î[ïtñîÇÕä“ït)ê≈äz
				m_Util.val_to_bin(Sn_SYOTSG, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("ABL00040") ) {	// ëÊÇPéÌéñã∆îÑè„çÇ
				m_Util.val_to_bin(Sn_KURI1, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("ABL00070") ) {	// ëÊÇQéÌéñã∆îÑè„çÇ
				m_Util.val_to_bin(Sn_KURI2, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("ABL00100") ) {	// ëÊÇRéÌéñã∆îÑè„çÇ
				m_Util.val_to_bin(Sn_KURI3, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("ABL00130") ) {	// ëÊÇSéÌéñã∆îÑè„çÇ
				m_Util.val_to_bin(Sn_KURI4, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("ABL00160") ) {	// ëÊÇTéÌéñã∆îÑè„çÇ
				m_Util.val_to_bin(Sn_KURI5, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("ABL00190") ) {	// ëÊÇUéÌ(îÑè„çÇ)
				m_Util.val_to_bin(Sn_KURI6, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("ABH00010") ) {	// ëOîNÇÃâ€ê≈îÑè„çÇ
				m_Util.val_to_bin(Sn_ZURI, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("ABH00020") ) {	// ëOÅXâÒî[ïtê≈äz
				m_Util.val_to_bin(Sn_ZZNOFZ, pShLstVal->val);
			}
			else if( pShLstVal->itmId == _T("ABH00030") ) {	// ëOâÒî[ïtê≈äz
				m_Util.val_to_bin(Sn_ZNOFZ, pShLstVal->val);
			}
		}
	}

	if( isNodata ){
		// ï â€ê≈ï˚éÆÇ…ÉfÅ[É^óLÅH
		if( KzHoushiki == ID_ICSSH_GENSOKU ){
			filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_KANNI_HONPYO1_HYOID, nowVersion);
		}
		else if( KzHoushiki == ID_ICSSH_KANNI ){
			filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_IPPAN_HONPYO1_HYOID, nowVersion);
		}

		if( m_DBSyzAccess.m_pSH_LST_VAL ){
			m_DBSyzAccess.m_pSH_LST_VAL->Requery(filter);
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

				if( pShLstVal->itmId == _T("AAM00030") ){		// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
					m_Util.val_to_bin(Sn_ZZURI, pShLstVal->val);
				}
				else if( pShLstVal->itmId == _T("AAF00010") ){	// ëOîNÇÃâ€ê≈îÑè„çÇ
					m_Util.val_to_bin(Sn_ZURI, pShLstVal->val);
				}
				else if( pShLstVal->itmId == _T("AAF00020") ){	// ëOÅXâÒî[ïtê≈äz
					m_Util.val_to_bin(Sn_ZZNOFZ, pShLstVal->val);
				}
				else if( pShLstVal->itmId == _T("AAF00030") ){	// ëOâÒî[ïtê≈äz
					m_Util.val_to_bin(Sn_ZNOFZ, pShLstVal->val);
					Sn_ZNOsw = pShLstVal->sign1;
				}
				else if( pShLstVal->itmId == _T("ABI00180") ){	// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
					m_Util.val_to_bin(Sn_ZZURI, pShLstVal->val);
				}
				else if( pShLstVal->itmId == _T("ABH00010") ){	// ëOîNÇÃâ€ê≈îÑè„çÇ
					m_Util.val_to_bin(Sn_ZURI, pShLstVal->val);
				}
				else if( pShLstVal->itmId == _T("ABH00020") ){	// ëOÅXâÒî[ïtê≈äz
					m_Util.val_to_bin(Sn_ZZNOFZ, pShLstVal->val);
				}
				else if( pShLstVal->itmId == _T("ABH00030") ){	// ëOâÒî[ïtê≈äz
					m_Util.val_to_bin(Sn_ZNOFZ, pShLstVal->val);
				}
			}

			if( KzHoushiki == ID_ICSSH_GENSOKU ){
				// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
				pShLstVal->AddNew();
				pShLstVal->sn_seq = sn_seq;
				pShLstVal->version = nowVersion;
				pShLstVal->hyoId = ICS_SN_IPPAN_HONPYO1_HYOID;
				pShLstVal->itmId = _T("AAM00030");
				m_Util.val_to_asci(&pShLstVal->val, Sn_ZZURI);
				pShLstVal->sign1 = 0;
				pShLstVal->Update();

				// ëOîNÇÃâ€ê≈îÑè„çÇ
				pShLstVal->AddNew();
				pShLstVal->sn_seq = sn_seq;
				pShLstVal->version = nowVersion;
				pShLstVal->hyoId = ICS_SN_IPPAN_HONPYO1_HYOID;
				pShLstVal->itmId = _T("AAF00010");
				m_Util.val_to_asci(&pShLstVal->val, Sn_ZURI);
				pShLstVal->sign1 = 0;
				pShLstVal->Update();

				// ëOÅXâÒî[ïtê≈äz
				pShLstVal->AddNew();
				pShLstVal->sn_seq = sn_seq;
				pShLstVal->version = nowVersion;
				pShLstVal->hyoId = ICS_SN_IPPAN_HONPYO1_HYOID;
				pShLstVal->itmId = _T("AAF00020");
				m_Util.val_to_asci(&pShLstVal->val, Sn_ZZNOFZ);
				pShLstVal->sign1 = 0;
				pShLstVal->Update();

				// ëOâÒî[ïtê≈äz
				pShLstVal->AddNew();
				pShLstVal->sn_seq = sn_seq;
				pShLstVal->version = nowVersion;
				pShLstVal->hyoId = ICS_SN_IPPAN_HONPYO1_HYOID;
				pShLstVal->itmId = _T("AAF00030");
				m_Util.val_to_asci(&pShLstVal->val, Sn_ZNOFZ);
				pShLstVal->sign1 = Sn_ZNOsw;
				pShLstVal->Update();
			}
			else if( KzHoushiki == ID_ICSSH_KANNI ){
				// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
				pShLstVal->AddNew();
				pShLstVal->sn_seq = sn_seq;
				pShLstVal->version = nowVersion;
				pShLstVal->hyoId = ICS_SN_KANNI_HONPYO1_HYOID;
				pShLstVal->itmId = _T("ABI00180");
				m_Util.val_to_asci(&pShLstVal->val, Sn_ZZURI);
				pShLstVal->sign1 = 0;
				pShLstVal->Update();

				// ëOîNÇÃâ€ê≈îÑè„çÇ
				pShLstVal->AddNew();
				pShLstVal->sn_seq = sn_seq;
				pShLstVal->version = nowVersion;
				pShLstVal->hyoId = ICS_SN_KANNI_HONPYO1_HYOID;
				pShLstVal->itmId = _T("ABH00010");
				m_Util.val_to_asci(&pShLstVal->val, Sn_ZURI);
				pShLstVal->sign1 = 0;
				pShLstVal->Update();

				// ëOÅXâÒî[ïtê≈äz
				pShLstVal->AddNew();
				pShLstVal->sn_seq = sn_seq;
				pShLstVal->version = nowVersion;
				pShLstVal->hyoId = ICS_SN_KANNI_HONPYO1_HYOID;
				pShLstVal->itmId = _T("ABH00020");
				m_Util.val_to_asci(&pShLstVal->val, Sn_ZZNOFZ);
				pShLstVal->sign1 = 0;
				pShLstVal->Update();

				// ëOâÒî[ïtê≈äz
				pShLstVal->AddNew();
				pShLstVal->sn_seq = sn_seq;
				pShLstVal->version = nowVersion;
				pShLstVal->hyoId = ICS_SN_KANNI_HONPYO1_HYOID;
				pShLstVal->itmId = _T("ABH00030");
				m_Util.val_to_asci(&pShLstVal->val, Sn_ZNOFZ);
				pShLstVal->sign1 = 0;
				pShLstVal->Update();
			}
		}
	}

	// îÑè„äÑçá
	if( KzHoushiki == ID_ICSSH_KANNI ) {
		filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_KANNI_HONPYO1_HYOID, nowVersion);

		if( m_DBSyzAccess.m_pSH_LST_RATE ) {
			m_DBSyzAccess.m_pSH_LST_RATE->Requery(filter);
		}
		else {
			if( m_DBSyzAccess.OpenShLstRate(pDBNpSub->m_database, filter) ) {
				return -1;
			}
		}

		CSH_LST_RATE	*pShLstRate = m_DBSyzAccess.m_pSH_LST_RATE;

		for( int i = 0;; i++ ) {
			if( i == 0 ) {
				if( (pShLstRate->MoveFirst() == -1) || (pShLstRate->st == -1) ) {
					break;
				}
			}
			else {
				if( (pShLstRate->MoveNext() == -1) || (pShLstRate->st == -1) ) {
					break;
				}
			}

			if( pShLstRate->itmId == _T("ABL00050") ) {
				Sn_UP1 = pShLstRate->rate;
			}
			else if( pShLstRate->itmId == _T("ABL00080") ) {
				Sn_UP2 = pShLstRate->rate;
			}
			else if( pShLstRate->itmId == _T("ABL00110") ) {
				Sn_UP3 = pShLstRate->rate;
			}
			else if( pShLstRate->itmId == _T("ABL00140") ) {
				Sn_UP4 = pShLstRate->rate;
			}
			else if( pShLstRate->itmId == _T("ABL00170") ) {
				Sn_UP5 = pShLstRate->rate;
			}
			else if( pShLstRate->itmId == _T("ABL00200") ) {
				Sn_UP6 = pShLstRate->rate;
			}
		}
	}


	// âÔé–ìoò^Ç≈å¥ë•â€ê≈Ç©ÇÁä»à’â€ê≈ÅiãtÇ‡ä‹ÇﬁÅjÇ…ïœçX
	// çƒìxä»à’â€ê≈Ç©ÇÁå¥ë•â€ê≈Ç…ïœçXÇµìdéqê\çêÇçsÇ§Ç∆ÅAñ{ï\ÇÃî[ïtê≈äzÇ∆î[ïtè˜ìnäÑäzÇ™ãÛóìÇ…Ç»Ç¡ÇƒÇ¢ÇΩÇΩÇﬂÉJÉbÉg
/*
	// à»â∫ÇÃå‹çÄñ⁄ÇÕ
	// ì«çûÇ›éûÅAä˙ä‘Ç…ÇÊÇËîªï 
	// èëçûÇ›ÇÕÅAêVãKÉeÅ[ÉuÉãÅ{ä˘ë∂ÉeÅ[ÉuÉã
//	Sn_TTYWAR;
//	Sn_TKAKTZ;
//	Sn_TEDNOF;
//	Sn_EDNOFZ;
//	Sn_TYNOFZ;
	if( pDBNpSub->zvol->ss_ymd < ICS_SH_10PER_REFORM_DAY ){
#ifdef _DBG_VIEW_START_
		hoge.Format("CH31SnHonpyoData::GetDataSub()__ì¡éÍä˙ä‘ì«çû\n");
		DbgViewTrace(hoge);
#endif
		filter.Format(_T("Sn_SEQ = %d"), sn_seq);
		if( pDBNpSub->szsnk ){
			pDBNpSub->szsnk->Requery(filter);
		}
		else{
			if( pDBNpSub->SzsnkOpen(filter) ){
				return -1;
			}
		}

		CSZSNK *pSZSNK = pDBNpSub->szsnk;
		if( pSZSNK->st != -1 ){
			m_Util.val_to_bin(Sn_TTYWAR, pSZSNK->Sn_TTYWAR);
			m_Util.val_to_bin(Sn_TKAKTZ, pSZSNK->Sn_TKAKTZ);
			m_Util.val_to_bin(Sn_EDNOFZ, pSZSNK->Sn_EDNOFZ);
			m_Util.val_to_bin(Sn_TEDNOF, pSZSNK->Sn_TEDNOF);
			m_Util.val_to_bin(Sn_TYNOFZ, pSZSNK->Sn_TYNOFZ);
		}
	}

#ifdef _DBG_VIEW_START_
	hoge.Format("CH31SnHonpyoData::GetDataSub()__END\n");
	DbgViewTrace(hoge);
#endif
*/
	return 0;
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
int CH31SnHonpyoData::GetData( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}
	ASSERT( pDBNpSub->zvol );
	if( pDBNpSub->zvol == NULL ){
		return -1;
	}

	EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki(pDBNpSub->zvol);
	if (KzHoushiki == ID_ICSSH_MENZEI) {
		return 0;
	}

	return GetDataSub(pDBNpSub, sn_seq, KzHoushiki);
}

//-----------------------------------------------------------------------------
// DBÇ÷ÉfÅ[É^èëçûÇ›
//-----------------------------------------------------------------------------
// à¯êî	pDBNpSub	ÅF	ç‡ñ±ÉNÉâÉXÇÃÉ|ÉCÉìÉ^
//		sn_seq		ÅF	ëŒè€ÇÃóöóÉVÅ[ÉPÉìÉX
//		wrtype		ÅF	èëçûÇ›É^ÉCÉv	Å†Å†Å†Å† Å†Å†Å†Å† Å†Å†Å†Å† Å†Å†Å†Å† Å†Å†Å†Å† Å†Å†Å†Å† Å†Å†Å†Å† Å†Å°Å°Å°
//						 D0ÅFszsnk Ç÷ÉfÅ[É^ÇèëÇ´çûÇ‹Ç»Ç¢
//						 D1ÅFñ∆ê≈éñã∆é“Ç≈Ç†Ç¡ÇƒÇ‡å¥ë•â€ê≈Ç∆ÇµÇƒÉfÅ[É^ÇèëÇ´çûÇﬁ
//						 D2ÅFñ∆ê≈éñã∆é“Ç≈Ç†Ç¡ÇƒÇ‡ä»à’â€ê≈Ç∆ÇµÇƒÉfÅ[É^ÇèëÇ´çûÇﬁ
//-----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	ê≥èÌèIóπ
//			-1		ÅF	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CH31SnHonpyoData::SetData( CDBNpSub *pDBNpSub, int sn_seq, int wrtype/*=0*/ )
{
#ifdef _DBG_VIEW_START_
	CString	hoge;
	hoge.Format("CH31SnHonpyoData::SetData()__START\n");
	DbgViewTrace(hoge);
#endif

	ASSERT(pDBNpSub);
	if (pDBNpSub == NULL) {
		return -1;
	}
	ASSERT(pDBNpSub->zvol);
	if (pDBNpSub->zvol == NULL) {
		return -1;
	}

	EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki(pDBNpSub->zvol);
	if ((KzHoushiki==ID_ICSSH_MENZEI) && !(wrtype&0x06)) {
		return 0;
	}
	else if( (KzHoushiki==ID_ICSSH_MENZEI) && (wrtype&0x02) ){
		KzHoushiki = ID_ICSSH_GENSOKU;
	}
	else if( (KzHoushiki==ID_ICSSH_MENZEI) && (wrtype&0x04) ){
		KzHoushiki = ID_ICSSH_KANNI;
	}

	CString	filter;
	char	*pHyoID = NULL;
	int		nowVersion = 2;
	if (KzHoushiki == ID_ICSSH_GENSOKU) {
		filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_IPPAN_HONPYO1_HYOID, nowVersion);
		pHyoID = ICS_SN_IPPAN_HONPYO1_HYOID;
	}
	else if (KzHoushiki == ID_ICSSH_KANNI) {
		filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_KANNI_HONPYO1_HYOID, nowVersion);
		pHyoID = ICS_SN_KANNI_HONPYO1_HYOID;
	}

	if (m_DBSyzAccess.m_pSH_LST_VAL) {
		m_DBSyzAccess.m_pSH_LST_VAL->Requery(filter);
	}
	else {
		if (m_DBSyzAccess.OpenShLstVal(pDBNpSub->m_database, filter)) {
			return -1;
		}
	}

	BOOL	isChg_ZZURI = FALSE;	// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇÇ…ïœçXóLÇËÅH

	CSH_LST_VAL	*pShLstVal = m_DBSyzAccess.m_pSH_LST_VAL;

	if ((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {	// ÉfÅ[É^ñ≥Çµ

		if (KzHoushiki == ID_ICSSH_GENSOKU) {

			// â€ê≈ïWèÄäz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAJ00010");
			m_Util.val_to_asci(&pShLstVal->val, Sn_KSTD);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// è¡îÔê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAJ00020");
			m_Util.val_to_asci(&pShLstVal->val, Sn_SYTX);
			pShLstVal->sign1 = Sn_SYTsw;
			pShLstVal->Update();

			// çTèúâﬂëÂí≤êÆê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAJ00030");
			m_Util.val_to_asci(&pShLstVal->val, Sn_KJADJZ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// çTèúëŒè€édì¸ê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAJ00050");
			m_Util.val_to_asci(&pShLstVal->val, Sn_SIREZ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAJ00060");
			m_Util.val_to_asci(&pShLstVal->val, Sn_HKANZ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ë›ì|ÇÍÇ…åWÇÈê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAJ00070");
			m_Util.val_to_asci(&pShLstVal->val, Sn_KSITZ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// çTèúê≈äzè¨åv
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAJ00080");
			m_Util.val_to_asci(&pShLstVal->val, Sn_KJZSK);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// çTèúïsë´ä“ïtê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAJ00090");
			m_Util.val_to_asci(&pShLstVal->val, Sn_KJHKZ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ç∑à¯ê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAJ00100");
			m_Util.val_to_asci(&pShLstVal->val, Sn_SHZ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// íÜä‘î[ïtê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAJ00110");
			m_Util.val_to_asci(&pShLstVal->val, Sn_TYNOFZ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// î[ïtê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAJ00120");
			m_Util.val_to_asci(&pShLstVal->val, Sn_EDNOFZ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// íÜä‘î[ïtä“ïtê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAJ00130");
			m_Util.val_to_asci(&pShLstVal->val, Sn_TYNKNP);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ä˘ämíËê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAJ00150");
			m_Util.val_to_asci(&pShLstVal->val, Sn_KAKTIZ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ç∑à¯î[ïtê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAJ00160");
			m_Util.val_to_asci(&pShLstVal->val, Sn_SHN);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAJ00180");
			m_Util.val_to_asci(&pShLstVal->val, Sn_KZURI);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAJ00190");
			m_Util.val_to_asci(&pShLstVal->val, Sn_SOURI);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// çTèúïsë´ä“ïtê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAK00020");
			m_Util.val_to_asci(&pShLstVal->val, Sn_TKJHKZ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ç∑à¯ê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAK00030");
			m_Util.val_to_asci(&pShLstVal->val, Sn_TSHZ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ä“ïtäz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAK00050");
			m_Util.val_to_asci(&pShLstVal->val, Sn_KNP);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// î[ê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAK00060");
			m_Util.val_to_asci(&pShLstVal->val, Sn_NOZ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// íÜä‘î[ïtè˜ìnäÑäzÅiínï˚è¡îÔê≈Åj
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAK00070");
			m_Util.val_to_asci(&pShLstVal->val, Sn_TTYWAR);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// î[ïtè˜ìnäÑäzÅiínï˚è¡îÔê≈Åj
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAK00080");
			m_Util.val_to_asci(&pShLstVal->val, Sn_TEDNOF);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// íÜä‘î[ïtä“ïtè˜ìnäÑäz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAK00090");
			m_Util.val_to_asci(&pShLstVal->val, Sn_TYNKNPJ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ä˘ämíËè˜ìnäÑäz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAK00110");
			m_Util.val_to_asci(&pShLstVal->val, Sn_TKAKTZ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ç∑à¯î[ïtè˜ìnäÑäz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAK00120");
			m_Util.val_to_asci(&pShLstVal->val, Sn_SHNJ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// è¡îÔê≈ãyÇ—ínï˚è¡îÔê≈ÇÃçáåv(î[ïtñîÇÕä“ït)ê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAK00130");
			m_Util.val_to_asci(&pShLstVal->val, Sn_SYOTSG);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAM00030");
			m_Util.val_to_asci(&pShLstVal->val, Sn_ZZURI);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ëOîNÇÃâ€ê≈îÑè„çÇ
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAF00010");
			m_Util.val_to_asci(&pShLstVal->val, Sn_ZURI);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ëOÅXâÒî[ïtê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAF00020");
			m_Util.val_to_asci(&pShLstVal->val, Sn_ZZNOFZ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ëOâÒî[ïtê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AAF00030");
			m_Util.val_to_asci(&pShLstVal->val, Sn_ZNOFZ);
			pShLstVal->sign1 = Sn_ZNOsw;
			pShLstVal->Update();

			// Ç±ÇÃâ€ê≈ä˙ä‘Ç…Ç®ÇØÇÈâ€ê≈îÑè„çÇ
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T( "FFF00000" );
			m_Util.val_to_asci( &pShLstVal->val, Sn_KazeiUriage );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

		}
		else if (KzHoushiki == ID_ICSSH_KANNI) {

			// â€ê≈ïWèÄäz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABI00010");
			m_Util.val_to_asci(&pShLstVal->val, Sn_KSTD);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// è¡îÔê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABI00020");
			m_Util.val_to_asci(&pShLstVal->val, Sn_SYTX);
			pShLstVal->sign1 = Sn_SYTsw;
			pShLstVal->Update();

			// ë›ì|âÒé˚Ç…åWÇÈè¡îÔê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABI00030");
			m_Util.val_to_asci(&pShLstVal->val, Sn_KJADJZ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// çTèúëŒè€édì¸ê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABI00050");
			m_Util.val_to_asci(&pShLstVal->val, Sn_SIREZ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABI00060");
			m_Util.val_to_asci(&pShLstVal->val, Sn_HKANZ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ë›ì|ÇÍÇ…åWÇÈê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABI00070");
			m_Util.val_to_asci(&pShLstVal->val, Sn_KSITZ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// çTèúê≈äzè¨åv
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABI00080");
			m_Util.val_to_asci(&pShLstVal->val, Sn_KJZSK);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// çTèúïsë´ä“ïtê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABI00090");
			m_Util.val_to_asci(&pShLstVal->val, Sn_KJHKZ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ç∑à¯ê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABI00100");
			m_Util.val_to_asci(&pShLstVal->val, Sn_SHZ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// íÜä‘î[ïtê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABI00110");
			m_Util.val_to_asci(&pShLstVal->val, Sn_TYNOFZ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// î[ïtê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABI00120");
			m_Util.val_to_asci(&pShLstVal->val, Sn_EDNOFZ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// íÜä‘î[ïtä“ïtê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABI00130");
			m_Util.val_to_asci(&pShLstVal->val, Sn_TYNKNP);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ä˘ämíËê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABI00150");
			m_Util.val_to_asci(&pShLstVal->val, Sn_KAKTIZ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ç∑à¯î[ïtê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABI00160");
			m_Util.val_to_asci(&pShLstVal->val, Sn_SHN);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABI00170");
			m_Util.val_to_asci(&pShLstVal->val, Sn_KZURI);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABI00180");
			m_Util.val_to_asci(&pShLstVal->val, Sn_ZZURI);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// çTèúïsë´ä“ïtê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABJ00020");
			m_Util.val_to_asci(&pShLstVal->val, Sn_TKJHKZ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ç∑à¯ê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABJ00030");
			m_Util.val_to_asci(&pShLstVal->val, Sn_TSHZ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ä“ïtäz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABJ00050");
			m_Util.val_to_asci(&pShLstVal->val, Sn_KNP);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// î[ê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABJ00060");
			m_Util.val_to_asci(&pShLstVal->val, Sn_NOZ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// íÜä‘î[ïtè˜ìnäÑäzÅiínï˚è¡îÔê≈Åj
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABJ00070");
			m_Util.val_to_asci(&pShLstVal->val, Sn_TTYWAR);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// î[ïtè˜ìnäÑäzÅiínï˚è¡îÔê≈Åj
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABJ00080");
			m_Util.val_to_asci(&pShLstVal->val, Sn_TEDNOF);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// íÜä‘î[ïtä“ïtè˜ìnäÑäz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABJ00090");
			m_Util.val_to_asci(&pShLstVal->val, Sn_TYNKNPJ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ä˘ämíËè˜ìnäÑäz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABJ00110");
			m_Util.val_to_asci(&pShLstVal->val, Sn_TKAKTZ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ç∑à¯î[ïtè˜ìnäÑäz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABJ00120");
			m_Util.val_to_asci(&pShLstVal->val, Sn_SHNJ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// è¡îÔê≈ãyÇ—ínï˚è¡îÔê≈ÇÃçáåv(î[ïtñîÇÕä“ït)ê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABJ00130");
			m_Util.val_to_asci(&pShLstVal->val, Sn_SYOTSG);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ëÊÇPéÌîÑè„çÇ
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABL00040");
			m_Util.val_to_asci(&pShLstVal->val, Sn_KURI1);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ëÊÇQéÌîÑè„çÇ
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABL00070");
			m_Util.val_to_asci(&pShLstVal->val, Sn_KURI2);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ëÊÇRéÌîÑè„çÇ
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABL00100");
			m_Util.val_to_asci(&pShLstVal->val, Sn_KURI3);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ëÊÇSéÌîÑè„çÇ
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABL00130");
			m_Util.val_to_asci(&pShLstVal->val, Sn_KURI4);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ëÊÇTéÌîÑè„çÇ
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABL00160");
			m_Util.val_to_asci(&pShLstVal->val, Sn_KURI5);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ëÊÇUéÌ
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABL00190");
			m_Util.val_to_asci(&pShLstVal->val, Sn_KURI6);
			pShLstVal->sign1 = 0; 
			pShLstVal->Update();

			// ëOîNÇÃâ€ê≈îÑè„çÇ
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABH00010");
			m_Util.val_to_asci(&pShLstVal->val, Sn_ZURI);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ëOÅXâÒî[ïtê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABH00020");
			m_Util.val_to_asci(&pShLstVal->val, Sn_ZZNOFZ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ëOâÒî[ïtê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ABH00030");
			m_Util.val_to_asci(&pShLstVal->val, Sn_ZNOFZ);
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

		}

	}
	else {						// ÉfÅ[É^óL	
		if (KzHoushiki == ID_ICSSH_GENSOKU) {
			CStringArray	updTagAry;
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
				if (pShLstVal->itmId == _T("AAJ00010")) {		// â€ê≈ïWèÄäz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_KSTD);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("AAJ00020")) {	// è¡îÔê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_SYTX);
					pShLstVal->sign1 = Sn_SYTsw;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("AAJ00030")) {	// çTèúâﬂëÂí≤êÆê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_KJADJZ);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("AAJ00050")) {	// çTèúëŒè€édì¸ê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_SIREZ);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("AAJ00060")) {	// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_HKANZ);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("AAJ00070")) {	// ë›ì|ÇÍÇ…åWÇÈê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_KSITZ);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("AAJ00080")) {	// çTèúê≈äzè¨åv
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_KJZSK);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("AAJ00090")) {	// çTèúïsë´ä“ïtê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_KJHKZ);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("AAJ00100")) {	// ç∑à¯ê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_SHZ);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("AAJ00110")) {	// íÜä‘î[ïtê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_TYNOFZ);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("AAJ00120")) {	// î[ïtê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_EDNOFZ);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("AAJ00130")) {	// íÜä‘î[ïtä“ïtê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_TYNKNP);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("AAJ00150")) {	// ä˘ämíËê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_KAKTIZ);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("AAJ00160")) {	// ç∑à¯î[ïtê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_SHN);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("AAJ00180")) {	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_KZURI);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("AAJ00190")) {	// éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_SOURI);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("AAK00020")) {	// çTèúïsë´ä“ïtê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_TKJHKZ);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("AAK00030")) {	// ç∑à¯ê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_TSHZ);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("AAK00050")) {	// ä“ïtäz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_KNP);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("AAK00060")) {	// î[ê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_NOZ);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("AAK00070")) {	// íÜä‘î[ïtè˜ìnäÑäzÅiínï˚è¡îÔê≈Åj
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_TTYWAR);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("AAK00080")) {	// î[ïtè˜ìnäÑäzÅiínï˚è¡îÔê≈Åj
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_TEDNOF);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("AAK00090")) {	// íÜä‘î[ïtä“ïtè˜ìnäÑäz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_TYNKNPJ);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("AAK00110")) {	// ä˘ämíËè˜ìnäÑäz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_TKAKTZ);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("AAK00120")) {	// ç∑à¯î[ïtè˜ìnäÑäz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_SHNJ);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("AAK00130")) {	// è¡îÔê≈ãyÇ—ínï˚è¡îÔê≈ÇÃçáåv(î[ïtñîÇÕä“ït)ê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_SYOTSG);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				else if (pShLstVal->itmId == _T("AAM00030")) {	// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
					pShLstVal->Edit();
					CString	tmpStr;
					m_Util.val_to_asci(&tmpStr, Sn_ZZURI);
					if (pShLstVal->val.Compare(tmpStr)) {
						isChg_ZZURI = TRUE;
					}
					pShLstVal->val = tmpStr;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				else if (pShLstVal->itmId == _T("AAF00010")) {	// ëOîNÇÃâ€ê≈îÑè„çÇ
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_ZURI);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("AAF00020")) {	// ëOÅXâÒî[ïtê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_ZZNOFZ);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("AAF00030")) {	// ëOâÒî[ïtê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_ZNOFZ);
					pShLstVal->sign1 = Sn_ZNOsw;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if( pShLstVal->itmId == _T( "FFF00000" )) {	// â€ê≈ä˙ä‘Ç…Ç®ÇØÇÈâ€ê≈îÑè„çÇ
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_KazeiUriage );
					pShLstVal->sign1 = 0;
					pShLstVal->Update();

					updTagAry.Add( pShLstVal->itmId );
				}
			}

			// çXêVÉfÅ[É^ämîF
			int	adCnt = 0;
			for (int i = 0;; i++) {
				if (strlen(tagDataIppan[i].tagName) == 0) {
					break;
				}
				adCnt++;
			}
			int updCnt = (int)updTagAry.GetCount();
			if (adCnt != updCnt) {
				// ñ≥Ç¢Ç‡ÇÃÇí«â¡
				CArray<unsigned char*>	MonyPtrAry;
				MonyPtrAry.Add(Sn_KSTD);
				MonyPtrAry.Add(Sn_SYTX);
				MonyPtrAry.Add(Sn_KJADJZ);
				MonyPtrAry.Add(Sn_SIREZ);
				MonyPtrAry.Add(Sn_HKANZ);
				MonyPtrAry.Add(Sn_KSITZ);
				MonyPtrAry.Add(Sn_KJZSK);
				MonyPtrAry.Add(Sn_KJHKZ);
				MonyPtrAry.Add(Sn_SHZ);
				MonyPtrAry.Add(Sn_TYNOFZ);
				MonyPtrAry.Add(Sn_EDNOFZ);
				MonyPtrAry.Add(Sn_TYNKNP);
				MonyPtrAry.Add(Sn_KAKTIZ);
				MonyPtrAry.Add(Sn_SHN);
				MonyPtrAry.Add(Sn_KZURI);
				MonyPtrAry.Add(Sn_SOURI);
				MonyPtrAry.Add(Sn_TKJHKZ);
				MonyPtrAry.Add(Sn_TSHZ);
				MonyPtrAry.Add(Sn_KNP);
				MonyPtrAry.Add(Sn_NOZ);
				MonyPtrAry.Add(Sn_TTYWAR);
				MonyPtrAry.Add(Sn_TEDNOF);
				MonyPtrAry.Add(Sn_TYNKNPJ);
				MonyPtrAry.Add(Sn_TKAKTZ);
				MonyPtrAry.Add(Sn_SHNJ);
				MonyPtrAry.Add(Sn_SYOTSG);
				MonyPtrAry.Add(Sn_ZZURI);
				MonyPtrAry.Add(Sn_ZURI);
				MonyPtrAry.Add(Sn_ZZNOFZ);
				MonyPtrAry.Add(Sn_ZNOFZ);
				MonyPtrAry.Add( Sn_KazeiUriage );
				int	aryMax = (int)MonyPtrAry.GetCount();

				BOOL findFlg = FALSE;
				for (int i = 0;; i++) {
					if (strlen(tagDataIppan[i].tagName) == 0) {
						break;
					}

					findFlg = FALSE;
					for (int j = 0; j < updCnt; j++) {
						if (updTagAry[j].CompareNoCase(tagDataIppan[i].tagName) == 0) {
							findFlg = TRUE;
							break;
						}
					}
					if (findFlg == FALSE) {
						// äYìñÉ^ÉOÇÃí«â¡
						if (i < aryMax) {
							pShLstVal->AddNew();
							pShLstVal->sn_seq = sn_seq;
							pShLstVal->version = nowVersion;
							pShLstVal->hyoId = pHyoID;
							pShLstVal->itmId = tagDataIppan[i].tagName;
							m_Util.val_to_asci(&pShLstVal->val, MonyPtrAry[i]);
							pShLstVal->Update();
						}
					}
				}
			}
		}
		else if (KzHoushiki == ID_ICSSH_KANNI) {
			CStringArray	updTagAry;
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
				if (pShLstVal->itmId == _T("ABI00010")) {		// â€ê≈ïWèÄäz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_KSTD);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("ABI00020")) {	// è¡îÔê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_SYTX);
					pShLstVal->sign1 = Sn_SYTsw;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("ABI00030")) {	// ë›ì|âÒé˚Ç…åWÇÈè¡îÔê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_KJADJZ);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("ABI00050")) {	// çTèúëŒè€édì¸ê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_SIREZ);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("ABI00060")) {	// ï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_HKANZ);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("ABI00070")) {	// ë›ì|ÇÍÇ…åWÇÈê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_KSITZ);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("ABI00080")) {	// çTèúê≈äzè¨åv
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_KJZSK);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("ABI00090")) {	// çTèúïsë´ä“ïtê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_KJHKZ);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("ABI00100")) {	// ç∑à¯ê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_SHZ);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("ABI00110")) {	// íÜä‘î[ïtê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_TYNOFZ);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("ABI00120")) {	// î[ïtê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_EDNOFZ);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("ABI00130")) {	// íÜä‘î[ïtä“ïtäz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_TYNKNP);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("ABI00150")) {	// ä˘ämíËê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_KAKTIZ);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("ABI00160")) {	// ç∑à¯î[ïtê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_SHN);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("ABI00170")) {	// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_KZURI);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("ABI00180")) {	// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
					pShLstVal->Edit();
					CString	tmpStr;
					m_Util.val_to_asci(&tmpStr, Sn_ZZURI);
					if (pShLstVal->val.Compare(tmpStr)) {
						isChg_ZZURI = TRUE;
					}
					pShLstVal->val = tmpStr;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("ABJ00020")) {	// çTèúïsë´ä“ïtê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_TKJHKZ);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("ABJ00030")) {	// ç∑à¯ê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_TSHZ);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("ABJ00050")) {	// ä“ïtäz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_KNP);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("ABJ00060")) {	// î[ê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_NOZ);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("ABJ00070")) {	// íÜä‘î[ïtè˜ìnäÑäzÅiínï˚è¡îÔê≈Åj
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_TTYWAR);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("ABJ00080")) {	// î[ïtè˜ìnäÑäzÅiínï˚è¡îÔê≈Åj
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_TEDNOF);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("ABJ00090")) {	// íÜä‘î[ïtä“ïtè˜ìnäÑäz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_TYNKNPJ);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("ABJ00110")) {	// ä˘ämíËè˜ìnäÑäz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_TKAKTZ);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("ABJ00120")) {	// ç∑à¯î[ïtè˜ìnäÑäz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_SHNJ);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("ABJ00130")) {	// è¡îÔê≈ãyÇ—ínï˚è¡îÔê≈ÇÃçáåv(î[ïtñîÇÕä“ït)ê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_SYOTSG);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("ABL00040")) {	// ëÊÇPéÌîÑè„çÇ
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_KURI1);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("ABL00070")) {	// ëÊÇQéÌîÑè„çÇ
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_KURI2);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("ABL00100")) {	// ëÊÇRéÌîÑè„çÇ
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_KURI3);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("ABL00130")) {	// ëÊÇSéÌîÑè„çÇ
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_KURI4);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("ABL00160")) {	// ëÊÇTéÌîÑè„çÇ
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_KURI5);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("ABL00190")) {  // ëÊÇUéÌîÑè„çÇ
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_KURI6);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				else if (pShLstVal->itmId == _T("ABH00010")) {	// ëOîNÇÃâ€ê≈îÑè„çÇ
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_ZURI);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("ABH00020")) {	// ëOÅXâÒî[ïtê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_ZZNOFZ);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				else if (pShLstVal->itmId == _T("ABH00030")) {	// ëOâÒî[ïtê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci(&pShLstVal->val, Sn_ZNOFZ);
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

			}

			// çXêVÉfÅ[É^ämîF
			int	adCnt = 0;
			for (int i = 0;; i++) {
				if (strlen(tagDataKani[i].tagName) == 0) {
					break;
				}
				adCnt++;
			}
			int updCnt = (int)updTagAry.GetCount();
			if (adCnt != updCnt) {
				// ñ≥Ç¢Ç‡ÇÃÇí«â¡
				CArray<unsigned char*>	MonyPtrAry;
				MonyPtrAry.Add(Sn_KSTD);
				MonyPtrAry.Add(Sn_SYTX);
				MonyPtrAry.Add(Sn_KJADJZ);
				MonyPtrAry.Add(Sn_SIREZ);
				MonyPtrAry.Add(Sn_HKANZ);
				MonyPtrAry.Add(Sn_KSITZ);
				MonyPtrAry.Add(Sn_KJZSK);
				MonyPtrAry.Add(Sn_KJHKZ);
				MonyPtrAry.Add(Sn_SHZ);
				MonyPtrAry.Add(Sn_TYNOFZ);
				MonyPtrAry.Add(Sn_EDNOFZ);
				MonyPtrAry.Add(Sn_TYNKNP);
				MonyPtrAry.Add(Sn_KAKTIZ);
				MonyPtrAry.Add(Sn_SHN);
				MonyPtrAry.Add(Sn_KZURI);
				MonyPtrAry.Add(Sn_ZZURI);
				MonyPtrAry.Add(Sn_TKJHKZ);
				MonyPtrAry.Add(Sn_TSHZ);
				MonyPtrAry.Add(Sn_KNP);
				MonyPtrAry.Add(Sn_NOZ);
				MonyPtrAry.Add(Sn_TTYWAR);
				MonyPtrAry.Add(Sn_TEDNOF);
				MonyPtrAry.Add(Sn_TYNKNPJ);
				MonyPtrAry.Add(Sn_TKAKTZ);
				MonyPtrAry.Add(Sn_SHNJ);
				MonyPtrAry.Add(Sn_SYOTSG);
				MonyPtrAry.Add(Sn_KURI1);
				MonyPtrAry.Add(Sn_KURI2);
				MonyPtrAry.Add(Sn_KURI3);
				MonyPtrAry.Add(Sn_KURI4);
				MonyPtrAry.Add(Sn_KURI5);
				MonyPtrAry.Add(Sn_KURI6);
				MonyPtrAry.Add(Sn_ZURI);
				MonyPtrAry.Add(Sn_ZZNOFZ);
				MonyPtrAry.Add(Sn_ZNOFZ);

				int	aryMax = (int)MonyPtrAry.GetCount();

				BOOL findFlg = FALSE;
				for (int i = 0;; i++) {
					if (strlen(tagDataKani[i].tagName) == 0) {
						break;
					}

					findFlg = FALSE;
					for (int j = 0; j < updCnt; j++) {
						if (updTagAry[j].CompareNoCase(tagDataKani[i].tagName) == 0) {
							findFlg = TRUE;
							break;
						}
					}
					if (findFlg == FALSE) {
						// äYìñÉ^ÉOÇÃí«â¡
						if (i < aryMax) {
							pShLstVal->AddNew();
							pShLstVal->sn_seq = sn_seq;
							pShLstVal->version = nowVersion;
							pShLstVal->hyoId = pHyoID;
							pShLstVal->itmId = tagDataKani[i].tagName;
							m_Util.val_to_asci(&pShLstVal->val, MonyPtrAry[i]);
							pShLstVal->Update();
						}
					}
				}
			}

		}
	}

	// ï â€ê≈ï˚éÆÇÃÉfÅ[É^Ç™Ç†ÇÈéûÇ…ÇÕÅAÇªÇøÇÁÇ…Ç‡ÉfÅ[É^ÇèëÇ´çûÇﬁ
	if( KzHoushiki == ID_ICSSH_GENSOKU ){
		filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_KANNI_HONPYO1_HYOID, nowVersion);
	}
	else if( KzHoushiki == ID_ICSSH_KANNI ){
		filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_IPPAN_HONPYO1_HYOID, nowVersion);
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

			if( pShLstVal->itmId == _T("AAM00030") ){		// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
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
			else if( pShLstVal->itmId == _T("ABI00180") ){	// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
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


	if (KzHoushiki == ID_ICSSH_GENSOKU) {
		filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_KANNI_HONPYO1_HYOID, nowVersion);
	}
	else if (KzHoushiki == ID_ICSSH_KANNI) {
		filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_IPPAN_HONPYO1_HYOID, nowVersion);
	}

	if (m_DBSyzAccess.m_pSH_LST_VAL) {
		m_DBSyzAccess.m_pSH_LST_VAL->Requery(filter);
	}
	pShLstVal = m_DBSyzAccess.m_pSH_LST_VAL;

	if (KzHoushiki == ID_ICSSH_KANNI) {
		filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_KANNI_HONPYO1_HYOID, nowVersion);

		if (m_DBSyzAccess.m_pSH_LST_RATE) {
			m_DBSyzAccess.m_pSH_LST_RATE->Requery(filter);
		}
		else {
			if (m_DBSyzAccess.OpenShLstRate(pDBNpSub->m_database, filter)) {
				return -1;
			}
		}

		CSH_LST_RATE	*pShLstRate = m_DBSyzAccess.m_pSH_LST_RATE;

		if ((pShLstRate->MoveFirst() == -1) || (pShLstRate->st == -1)) {	// ÉfÅ[É^ñ≥Çµ
			// ëÊàÍéÌéñã∆Å@îÑè„äÑçá
			pShLstRate->AddNew();
			pShLstRate->sn_seq = sn_seq;
			pShLstRate->version = nowVersion;
			pShLstRate->hyoId = pHyoID;
			pShLstRate->itmId = _T("ABL00050");
			pShLstRate->rate = Sn_UP1;
			pShLstRate->sign1 = 0;
			pShLstRate->Update();

			// ëÊìÒéÌéñã∆Å@îÑè„äÑçá
			pShLstRate->AddNew();
			pShLstRate->sn_seq = sn_seq;
			pShLstRate->version = nowVersion;
			pShLstRate->hyoId = pHyoID;
			pShLstRate->itmId = _T("ABL00080");
			pShLstRate->rate = Sn_UP2;
			pShLstRate->sign1 = 0;
			pShLstRate->Update();

			// ëÊéOéÌéñã∆Å@îÑè„äÑçá
			pShLstRate->AddNew();
			pShLstRate->sn_seq = sn_seq;
			pShLstRate->version = nowVersion;
			pShLstRate->hyoId = pHyoID;
			pShLstRate->itmId = _T("ABL00110");
			pShLstRate->rate = Sn_UP3;
			pShLstRate->sign1 = 0;
			pShLstRate->Update();

			// ëÊéléÌéñã∆Å@îÑè„äÑçá
			pShLstRate->AddNew();
			pShLstRate->sn_seq = sn_seq;
			pShLstRate->version = nowVersion;
			pShLstRate->hyoId = pHyoID;
			pShLstRate->itmId = _T("ABL00140");
			pShLstRate->rate = Sn_UP4;
			pShLstRate->sign1 = 0;
			pShLstRate->Update();

			// ëÊå‹éÌéñã∆Å@îÑè„äÑçá
			pShLstRate->AddNew();
			pShLstRate->sn_seq = sn_seq;
			pShLstRate->version = nowVersion;
			pShLstRate->hyoId = pHyoID;
			pShLstRate->itmId = _T("ABL00170");
			pShLstRate->rate = Sn_UP5;
			pShLstRate->sign1 = 0;
			pShLstRate->Update();

			// ëÊòZéÌéñã∆Å@îÑè„äÑçá
			pShLstRate->AddNew();
			pShLstRate->sn_seq = sn_seq;
			pShLstRate->version = nowVersion;
			pShLstRate->hyoId = pHyoID;
			pShLstRate->itmId = _T("ABL00200");
			pShLstRate->rate = Sn_UP6;
			pShLstRate->sign1 = 0;
			pShLstRate->Update();
		}
		else {
			for (int i = 0;; i++) {
				if (i == 0) {
					if ((pShLstRate->MoveFirst() == -1) || (pShLstRate->st == -1)) {
						break;
					}
				}
				else {
					if ((pShLstRate->MoveNext() == -1) || (pShLstRate->st == -1)) {
						break;
					}
				}

				// ëÊàÍéÌéñã∆Å@îÑè„äÑçá
				if (pShLstRate->itmId == _T("ABL00050")) {
					pShLstRate->Edit();
					pShLstRate->rate = Sn_UP1;
					pShLstRate->Update();
				}
				// ëÊìÒéÌéñã∆Å@îÑè„äÑçá
				else if (pShLstRate->itmId == _T("ABL00080")) {
					pShLstRate->Edit();
					pShLstRate->rate = Sn_UP2;
					pShLstRate->Update();
				}
				// ëÊéOéÌéñã∆Å@îÑè„äÑçá
				else if (pShLstRate->itmId == _T("ABL00110")) {
					pShLstRate->Edit();
					pShLstRate->rate = Sn_UP3;
					pShLstRate->Update();
				}
				// ëÊéléÌéñã∆Å@îÑè„äÑçá
				else if (pShLstRate->itmId == _T("ABL00140")) {
					pShLstRate->Edit();
					pShLstRate->rate = Sn_UP4;
					pShLstRate->Update();
				}
				// ëÊå‹éÌéñã∆Å@îÑè„äÑçá
				else if (pShLstRate->itmId == _T("ABL00170")) {
					pShLstRate->Edit();
					pShLstRate->rate = Sn_UP5;
					pShLstRate->Update();
				}
				// ëÊòZéÌéñã∆Å@îÑè„äÑçá
				else if (pShLstRate->itmId == _T("ABL00200")) {
					pShLstRate->Edit();
					pShLstRate->rate = Sn_UP6;
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
	if (!(wrtype & 0x01)) {
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
			// êVãKçÏê¨
			pSZSNK->AddNew();
			// êVãKçÏê¨éûÇ…ÉfÉtÉHÉãÉgílÇÕâ∫ÇÃÇ‡ÇÃÇæÇØÇ≈OK??
		}
		else {
			// çXêV
			pSZSNK->Edit();
		}

		m_Util.val_to_asci(&pSZSNK->Sn_TTYWAR, Sn_TTYWAR);
		m_Util.val_to_asci(&pSZSNK->Sn_TKAKTZ, Sn_TKAKTZ);
		m_Util.val_to_asci(&pSZSNK->Sn_EDNOFZ, Sn_EDNOFZ);
		m_Util.val_to_asci(&pSZSNK->Sn_TEDNOF, Sn_TEDNOF);
		m_Util.val_to_asci(&pSZSNK->Sn_TYNOFZ, Sn_TYNOFZ);

		if (isChg_ZZURI) {
			m_Util.val_to_asci(&pSZSNK->Sn_ZZURI, Sn_ZZURI);
		}

		pSZSNK->Update();
	}

#ifdef _DBG_VIEW_START_
	hoge.Format("CH31SnHonpyoData::SetData()__END\n");
	DbgViewTrace(hoge);
#endif

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
int CH31SnHonpyoData::GetPastData ( CDBNpSub *pDBNpSub, int sn_seq, CSnHeadData *pSnHeadData )
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

	// îÒòAìÆÉTÉCÉì
	int Rsign = 0;
	if ( pSnHeadData->Sn_Sign4 & 0x80 ) {
		Rsign = 1;
	}

	int nowVersion = 1;
	CString filter;

	if ( KzHoushiki == ID_ICSSH_GENSOKU ) {
		filter.Format ( _T( "sn_seq = %d and hyoId = \'%s\' and version = %d" ), sn_seq, ICS_SN_IPPAN_HONPYO_HYOID, nowVersion );
	}
	else if ( KzHoushiki == ID_ICSSH_KANNI ) {
		filter.Format ( _T( "sn_seq = %d and hyoId = \'%s\' and version = %d" ), sn_seq, ICS_SN_KANNI_HONPYO_HYOID, nowVersion );
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

		if ( KzHoushiki == ID_ICSSH_GENSOKU ) {
			// íÜä‘î[ïtê≈äz
			if ( pShLstVal->itmId == _T( "AAB00110" ) ) {
				m_Util.val_to_bin ( Sn_TYNOFZ, pShLstVal->val );
			}
			// íÜä‘î[ïtè˜ìnäÑäz
			else if ( pShLstVal->itmId == _T( "AAC00070" ) ) {
				m_Util.val_to_bin ( Sn_TTYWAR, pShLstVal->val );
			}
			// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
			else if( pShLstVal->itmId == _T("AAF00000") ) {
				m_Util.val_to_bin ( Sn_ZZURI, pShLstVal->val );
			}
			// ëOîNÇÃâ€ê≈îÑè„çÇ
			else if( pShLstVal->itmId == _T("AAF00010") ) {
				m_Util.val_to_bin ( Sn_ZURI, pShLstVal->val );
			}
			// ëOÅXâÒî[ïtê≈äz
			else if( pShLstVal->itmId == _T("AAF00020") ) {
				m_Util.val_to_bin ( Sn_ZZNOFZ, pShLstVal->val );
			}
			// ëOâÒî[ïtê≈äz
			else if( pShLstVal->itmId == _T("AAF00030") ) {
				m_Util.val_to_bin ( Sn_ZNOFZ, pShLstVal->val );
			}

			if ( Rsign == 1 ) {
				// ä˘ämíËê≈äz
				if ( pShLstVal->itmId == _T( "AAB00150" ) ) {
					m_Util.val_to_bin ( Sn_KAKTIZ, pShLstVal->val );
				}
				// ä˘ämíËè˜ìnäÑäz
				else if ( pShLstVal->itmId == _T( "AAC00110" ) ) {
					m_Util.val_to_bin ( Sn_TKAKTZ, pShLstVal->val );
				}
			}
		}
		else if ( KzHoushiki == ID_ICSSH_KANNI ) {
			// íÜä‘î[ïtê≈äz
			if ( pShLstVal->itmId == _T( "ABB00110" ) ) {
				m_Util.val_to_bin ( Sn_TYNOFZ, pShLstVal->val );
			}
			// íÜä‘î[ïtè˜ìnäÑäz
			else if ( pShLstVal->itmId == _T( "ABC00070" ) ) {
				m_Util.val_to_bin ( Sn_TTYWAR, pShLstVal->val );
			}
			// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇÅiòAìÆéûÇ…Ç‡éÊìæÅj
			else if( pShLstVal->itmId == _T("ABB00180") ) {
				m_Util.val_to_bin ( Sn_ZZURI, pShLstVal->val );
			}
			// ëOîNÇÃâ€ê≈îÑè„çÇ
			else if( pShLstVal->itmId == _T("ABH00010") ) {
				m_Util.val_to_bin ( Sn_ZURI, pShLstVal->val );
			}
			// ëOÅXâÒî[ïtê≈äz
			else if( pShLstVal->itmId == _T("ABH00020") ) {
				m_Util.val_to_bin ( Sn_ZZNOFZ, pShLstVal->val );
			}
			// ëOâÒî[ïtê≈äz
			else if( pShLstVal->itmId == _T("ABH00030") ) {
				m_Util.val_to_bin ( Sn_ZNOFZ, pShLstVal->val );
			}

			if ( Rsign ==  1 ) {
				// Ç±ÇÃâ€ê≈ä˙ä‘ÇÃâ€ê≈îÑè„çÇ
				if ( pShLstVal->itmId == _T( "ABB00170" ) ) {
					m_Util.val_to_bin ( Sn_KZURI, pShLstVal->val );
				}
				// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
				else if ( pShLstVal->itmId == _T( "ABB00180" ) ) {
					m_Util.val_to_bin ( Sn_ZZURI, pShLstVal->val );
				}
			}
		}
	}

	return 0;
}

//===============================================
// ï ÉoÅ[ÉWÉáÉìÇ©ÇÁÇÃÉfÅ[É^ÇÃà¯Ç´åpÇ¨
//-----------------------------------------------
// à¯êî
//		pDBNpSub	:	åˆâvÉNÉâÉX
//		sn_seq		:	óöóÉVÅ[ÉPÉìÉX
//		pSnHeadData	:	è¡îÔê≈ÉwÉbÉ_èÓïÒ
//		srcVer		:	à¯Ç´åpÇ¨å≥ÉoÅ[ÉWÉáÉì
//-----------------------------------------------
// ï‘ëóíl
//			0		:	ê≥èÌèIóπ
//			-1		:	ÉGÉâÅ[
//===============================================
int CH31SnHonpyoData::TakeOverData(CDBNpSub *pDBNpSub, int sn_seq, CSnHeadData *pSnHeadData, int srcVer)
{
	if( srcVer != ID_VER_SYZ26 ){
		return -1;
	}
	// à¯åpÇ¨å≥Ç∆ÇµÇƒÇWÅìâ¸ê≥ÉfÅ[É^ÇµÇ©àµÇÌÇ»Ç¢

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
	int		dataVersion = 1;	// 8%â¸ê≥ÉfÅ[É^ÇÃÉoÅ[ÉWÉáÉì
	if( KzHoushiki == ID_ICSSH_GENSOKU ) {
		filter.Format (_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_IPPAN_HONPYO_HYOID, dataVersion);
	}
	else if( KzHoushiki == ID_ICSSH_KANNI ) {
		filter.Format (_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_KANNI_HONPYO_HYOID, dataVersion);
	}
	if( m_DBSyzAccess.m_pSH_LST_VAL ) {
		m_DBSyzAccess.m_pSH_LST_VAL->Requery (filter);
	}
	else {
		if( m_DBSyzAccess.OpenShLstVal (pDBNpSub->m_database, filter) ) {
			return -1;
		}
	}

	CSH_LST_VAL *pShLstVal = m_DBSyzAccess.m_pSH_LST_VAL;

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
			if( pShLstVal->itmId == _T("AAF00000") ) {
				m_Util.val_to_bin (Sn_ZZURI, pShLstVal->val);
			}
			// ëOÅXâÒî[ïtê≈äz
			if( pShLstVal->itmId == _T("AAF00020") ) {
				m_Util.val_to_bin (Sn_ZZNOFZ, pShLstVal->val);
			}
			// ëOîNÇÃâ€ê≈îÑè„çÇ
			if( pShLstVal->itmId == _T("AAF00010") ) {
				m_Util.val_to_bin (Sn_ZURI, pShLstVal->val);
			}
			// ëOâÒî[ïtê≈äz
			if( pShLstVal->itmId == _T("AAF00030") ) {
				m_Util.val_to_bin (Sn_ZNOFZ, pShLstVal->val);
			}
		}
		else if( KzHoushiki == ID_ICSSH_KANNI ) {
			// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
			if( pShLstVal->itmId == _T("ABB00180") ) {
				m_Util.val_to_bin (Sn_ZZURI, pShLstVal->val);
			}
			// ëOÅXâÒî[ïtê≈äz
			if( pShLstVal->itmId == _T("ABH00020") ) {
				m_Util.val_to_bin (Sn_ZZNOFZ, pShLstVal->val);
			}
			// ëOîNÇÃâ€ê≈îÑè„çÇ
			if( pShLstVal->itmId == _T("ABH00010") ) {
				m_Util.val_to_bin (Sn_ZURI, pShLstVal->val);
			}
			// ëOâÒî[ïtê≈äz
			if( pShLstVal->itmId == _T("ABH00030") ) {
				m_Util.val_to_bin (Sn_ZNOFZ, pShLstVal->val);
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
int CH31SnHonpyoData::ClearDataByRenew(CDBNpSub *pDBNpSub, int type)
{
	ASSERT(pDBNpSub);
	if( pDBNpSub == NULL ){
		return -1;
	}

	CArith	arith(0x16);

	arith.l_clear(Sn_KSTDM);
	arith.l_clear(Sn_KSTD);
	arith.l_clear(Sn_TIKAM);
	arith.l_clear(Sn_TIKA);
	arith.l_clear(Sn_SYTXM);
	arith.l_clear(Sn_SYTX);
	Sn_SYTsw = 0;

	arith.l_clear(Sn_KJADJZ);
	arith.l_clear(Sn_SIREZ);
	arith.l_clear(Sn_HKANZ);
	arith.l_clear(Sn_KSITZ);
	arith.l_clear(Sn_TYNOFZ);

	arith.l_clear(Sn_EDNOFZ);
	arith.l_clear(Sn_KAKTIZ);

	arith.l_clear(Sn_TTYWAR);
	arith.l_clear(Sn_TKAKTZ);
	arith.l_clear(Sn_TEDNOF);
	arith.l_clear(Sn_KZURI);
	arith.l_clear(Sn_SOURI);

/*	Ç±ÇÃÇSÇ¬ÇÕè¡Ç∑ïKóvê´ÇÕñ≥Ç¢Ç∆évÇ¢Ç‹Ç∑ÅBÅiãåÇ©ÇÁè¡ÇµÇƒÇ¢Ç»Ç¢Åj
	arith.l_clear(Sn_ZZNOFZ);
	arith.l_clear(Sn_ZNOFZ);
	arith.l_clear(Sn_ZZURI);
	arith.l_clear(Sn_ZURI);
*/
	arith.l_clear(Sn_KURI1);
	arith.l_clear(Sn_KURI2);
	arith.l_clear(Sn_KURI3);
	arith.l_clear(Sn_KURI4);
	arith.l_clear(Sn_KURI5);
	arith.l_clear(Sn_KURI6);
	Sn_UP1 = 0;
	Sn_UP2 = 0;
	Sn_UP3 = 0;
	Sn_UP4 = 0;
	Sn_UP5 = 0;
	Sn_UP5 = 0;
	Sn_UP6 = 0;

	arith.l_clear(Sn_GENKAI);
	arith.l_clear(Sn_WGENKA);

	Sn_TS_KUBUN1 = 0;
	arith.l_clear(Sn_TS_TDM1);
	arith.l_clear(Sn_TS_SYT1);

	Sn_TS_KUBUN2 = 0;
	arith.l_clear(Sn_TS_SYT2);
	Sn_ZNOsw = 0;

	arith.l_clear(Sn_KSJT3);
	arith.l_clear(Sn_KSJT4);
	arith.l_clear(Sn_KSJT63);
	arith.l_clear(Sn_KSJTKEI);
	arith.l_clear(Sn_TKST63);
	arith.l_clear(Sn_TKSTKEI);
	arith.l_clear(Sn_URHKNZ);
	arith.l_clear(Sn_TKHKNZ);

	arith.l_clear(Sn_KJZSK);
	arith.l_clear(Sn_KJHKZ);
	arith.l_clear(Sn_SHZ);
	arith.l_clear(Sn_TYNKNP);
	arith.l_clear(Sn_SHN);
	arith.l_clear(Sn_TKJHKZ);
	arith.l_clear(Sn_TSHZ);
	arith.l_clear(Sn_KNP);
	arith.l_clear(Sn_NOZ);
	arith.l_clear(Sn_TYNKNPJ);
	arith.l_clear(Sn_SHNJ);
	arith.l_clear(Sn_SYOTSG);

	arith.l_clear( Sn_KazeiUriage );

	return 0;
}

//-----------------------------------------------------------------------------
// çXêVéûÇÃã‡äzèëÇ´çûÇ›Åiä»à’â€ê≈Åj
//-----------------------------------------------------------------------------
// à¯êî	pDBNpSub	ÅF	ç‡ñ±ÉNÉâÉXÇÃÉ|ÉCÉìÉ^
//		sn_seq		ÅF	ëŒè€ÇÃóöóÉVÅ[ÉPÉìÉX
//-----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	ê≥èÌèIóπ
//-----------------------------------------------------------------------------
int CH31SnHonpyoData::SetKanniDataInRenew(CDBNpSub *pDBNpSub, int sn_seq)
{
	CString	filter;
	int		nowVersion = 2;
	char	*pHyoID = NULL;
	filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_KANNI_HONPYO1_HYOID, nowVersion);
	pHyoID = ICS_SN_KANNI_HONPYO1_HYOID;

	if( m_DBSyzAccess.m_pSH_LST_VAL ){
		m_DBSyzAccess.m_pSH_LST_VAL->Requery(filter);
	}
	else{
		if( m_DBSyzAccess.OpenShLstVal(pDBNpSub->m_database, filter) ){
			return -1;
		}
	}

	CSH_LST_VAL	*pShLstVal = m_DBSyzAccess.m_pSH_LST_VAL;
	if( (pShLstVal->MoveFirst()==-1) || (pShLstVal->st==-1) ){	// ÉfÅ[É^ñ≥Çµ
		// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("ABI00180");
		m_Util.val_to_asci(&pShLstVal->val, Sn_ZZURI);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ëOÅXâÒî[ïtê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("ABH00020");
		m_Util.val_to_asci(&pShLstVal->val, Sn_ZZNOFZ);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ëOîNÇÃâ€ê≈îÑè„çÇ
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("ABH00010");
		m_Util.val_to_asci(&pShLstVal->val, Sn_ZURI);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ëOâÒî[ïtê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("ABH00030");
		m_Util.val_to_asci(&pShLstVal->val, Sn_ZNOFZ);
		pShLstVal->sign1 = Sn_ZNOsw;
		pShLstVal->Update();
	}
	else{	// ÉfÅ[É^óL
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
			if( pShLstVal->itmId == _T("ABI00180") ){
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_ZZURI);
				pShLstVal->Update();
				updTagAry.Add(pShLstVal->itmId);
			}
			// ëOÅXâÒî[ïtê≈äz
			else if( pShLstVal->itmId == _T("ABH00020") ){
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_ZZNOFZ);
				pShLstVal->Update();
				updTagAry.Add(pShLstVal->itmId);
			}
			// ëOîNÇÃâ€ê≈îÑè„çÇ
			else if( pShLstVal->itmId == _T("ABH00010") ){
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_ZURI);
				pShLstVal->Update();
				updTagAry.Add(pShLstVal->itmId);
			}
			// ëOâÒî[ïtê≈äz
			else if( pShLstVal->itmId == _T("ABH00030") ){
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_ZNOFZ);
				pShLstVal->Update();
				updTagAry.Add(pShLstVal->itmId);
			}
			// íÜä‘î[ïtê≈äz
			else if( pShLstVal->itmId == _T("ABI00110") ){
				pShLstVal->Edit();
				pShLstVal->val = _T("0");
				pShLstVal->Update();
				updTagAry.Add(pShLstVal->itmId);
			}
			// ínï˚è¡îÔê≈Å@íÜä‘î[ïtè˜ìnäÑäz
			else if( pShLstVal->itmId == _T("ABJ00070") ){
				pShLstVal->Edit();
				pShLstVal->val = _T("0");
				pShLstVal->Update();
				updTagAry.Add(pShLstVal->itmId);
			}
			// â€ê≈ïWèÄäz
			else if( pShLstVal->itmId == _T("ABI00010") ){
				pShLstVal->Edit();
				pShLstVal->val = _T("0");
				pShLstVal->Update();
				updTagAry.Add(pShLstVal->itmId);
			}
			// è¡îÔê≈äz
			else if( pShLstVal->itmId == _T("ABI00020") ){
				pShLstVal->Edit();
				pShLstVal->val = _T("0");
				pShLstVal->sign1 = 0;
				pShLstVal->Update();
				updTagAry.Add(pShLstVal->itmId);
			}
			// Ç±ÇÃâ€ê≈ä˙ä‘ÇÃâ€ê≈îÑè„çÇ
			else if( pShLstVal->itmId == _T("ABI00180") ){
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
			MonyPtrAry.Add(Sn_ZZURI);
			MonyPtrAry.Add(Sn_ZZNOFZ);
			MonyPtrAry.Add(Sn_ZURI);
			MonyPtrAry.Add(Sn_ZNOFZ);

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
						m_Util.val_to_asci(&pShLstVal->val, MonyPtrAry[i]);
						pShLstVal->Update();
					}
				}
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// çXêVéûÇÃã‡äzèëÇ´çûÇ›Åiå¥ë•â€ê≈Åj
//-----------------------------------------------------------------------------
// à¯êî	pDBNpSub	ÅF	ç‡ñ±ÉNÉâÉXÇÃÉ|ÉCÉìÉ^
//		sn_seq		ÅF	ëŒè€ÇÃóöóÉVÅ[ÉPÉìÉX
//-----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	ê≥èÌèIóπ
//-----------------------------------------------------------------------------
int CH31SnHonpyoData::SetGensokuDataInRenew(CDBNpSub *pDBNpSub, int sn_seq)
{
	CString	filter;
	int		nowVersion = 2;
	char	*pHyoID = NULL;
	filter.Format(_T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_IPPAN_HONPYO1_HYOID, nowVersion);
	pHyoID = ICS_SN_IPPAN_HONPYO1_HYOID;

	if( m_DBSyzAccess.m_pSH_LST_VAL ){
		m_DBSyzAccess.m_pSH_LST_VAL->Requery(filter);
	}
	else{
		if( m_DBSyzAccess.OpenShLstVal(pDBNpSub->m_database, filter) ){
			return -1;
		}
	}

	CSH_LST_VAL	*pShLstVal = m_DBSyzAccess.m_pSH_LST_VAL;
	if( (pShLstVal->MoveFirst()==-1) || (pShLstVal->st==-1) ){	// ÉfÅ[É^ñ≥Çµ
		// éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AAF00010");
		m_Util.val_to_asci(&pShLstVal->val, Sn_ZURI);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ëOÅXâÒî[ïtê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AAF00020");
		m_Util.val_to_asci(&pShLstVal->val, Sn_ZZNOFZ);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AAM00030");
		m_Util.val_to_asci(&pShLstVal->val, Sn_ZZURI);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ëOâÒî[ïtê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AAF00030");
		m_Util.val_to_asci(&pShLstVal->val, Sn_ZNOFZ);
		pShLstVal->sign1 = 0;
		pShLstVal->Update();
	}
	else{
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
			if( pShLstVal->itmId == _T("AAM00030") ){
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_ZZURI);
				pShLstVal->Update();
				updTagAry.Add(pShLstVal->itmId);
			}
			// ëOÅXâÒî[ïtê≈äz
			else if( pShLstVal->itmId == _T("AAF00020") ){
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_ZZNOFZ);
				pShLstVal->Update();
				updTagAry.Add(pShLstVal->itmId);
			}
			// ëOîNÇÃâ€ê≈îÑè„çÇ
			else if( pShLstVal->itmId == _T("AAF00010") ){
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_ZURI);
				pShLstVal->Update();
				updTagAry.Add(pShLstVal->itmId);
			}
			// ëOâÒî[ïtê≈äz
			else if( pShLstVal->itmId == _T("AAF00030") ){
				pShLstVal->Edit();
				m_Util.val_to_asci(&pShLstVal->val, Sn_ZNOFZ);
				pShLstVal->sign1 = Sn_ZNOsw;
				pShLstVal->Update();
				updTagAry.Add(pShLstVal->itmId);
			}
			// íÜä‘î[ïtê≈äz
			else if( pShLstVal->itmId == _T("AAJ00110") ){
				pShLstVal->Edit();
				pShLstVal->val = _T("0");
				pShLstVal->Update();
				updTagAry.Add(pShLstVal->itmId);
			}
			// ínï˚è¡îÔê≈Å@íÜä‘î[ïtè˜ìnäÑäz
			else if( pShLstVal->itmId == _T("AAK00070") ){
				pShLstVal->Edit();
				pShLstVal->val = _T("0");
				pShLstVal->sign1 = 0;
				pShLstVal->Update();
				updTagAry.Add(pShLstVal->itmId);
			}
			// â€ê≈ïWèÄäz
			else if( pShLstVal->itmId == _T("AAJ00010") ){
				pShLstVal->Edit();
				pShLstVal->val = _T("0");
				pShLstVal->Update();
				updTagAry.Add(pShLstVal->itmId);
			}
			// è¡îÔê≈äz
			else if( pShLstVal->itmId == _T("AAJ00020") ){
				pShLstVal->Edit();
				pShLstVal->val = _T("0");
				pShLstVal->sign1 = 0;
				pShLstVal->Update();
				updTagAry.Add(pShLstVal->itmId);
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
			MonyPtrAry.Add(Sn_ZZURI);
			MonyPtrAry.Add(Sn_ZZNOFZ);
			MonyPtrAry.Add(Sn_ZURI);
			MonyPtrAry.Add(Sn_ZNOFZ);

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
						m_Util.val_to_asci(&pShLstVal->val, MonyPtrAry[i]);
						pShLstVal->Update();
					}
				}
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// çXêVéûÇÃã‡äzí«â¡èëÇ´çûÇ›Åiñ∆ê≈Åj
//-----------------------------------------------------------------------------
// à¯êî	pDBNpSub	ÅF	ç‡ñ±ÉNÉâÉXÇÃÉ|ÉCÉìÉ^
//		sn_seq		ÅF	ëŒè€ÇÃóöóÉVÅ[ÉPÉìÉX
//-----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	ê≥èÌèIóπ
//-----------------------------------------------------------------------------
int CH31SnHonpyoData::SetMenzeiDataInRenew(CDBNpSub *pDBNpSub, int sn_seq)
{
	ASSERT(pDBNpSub);
	if( pDBNpSub == NULL ){
		return -1;
	}
	ASSERT(pDBNpSub->zvol);
	if( pDBNpSub->zvol == NULL ){
		return -1;
	}

	int ForceKzhoushiki = -1;
	if( ChkExistKzdata(pDBNpSub, 0, ForceKzhoushiki) ) {
		return -1;
	}

	EnumIdIcsShKazeihoushiki KzHoushiki = ID_ICSSH_MENZEI;
	for( int i=0; i<2; i++ ){
		if( ForceKzhoushiki == ID_ICSSH_GENSOKU ){
			if( i==0 ){
				KzHoushiki = ID_ICSSH_GENSOKU;
			}
			else{
				KzHoushiki = ID_ICSSH_KANNI;
			}
		}
		else if( ForceKzhoushiki == ID_ICSSH_KANNI ){
			if( i==0 ){
				KzHoushiki = ID_ICSSH_KANNI;
			}
			else{
				KzHoushiki = ID_ICSSH_GENSOKU;
			}
		}

		if( KzHoushiki == ID_ICSSH_GENSOKU ){
			SetGensokuDataInRenew(pDBNpSub, sn_seq);
		}
		else if( KzHoushiki == ID_ICSSH_KANNI ){
			SetKanniDataInRenew(pDBNpSub, sn_seq);
		}
	}

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
int CH31SnHonpyoData::SetDataInRenew(CDBNpSub *pDBNpSub, int sn_seq)
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
	if( (KzHoushiki!=ID_ICSSH_GENSOKU) && (KzHoushiki!=ID_ICSSH_KANNI) ){
		return SetMenzeiDataInRenew(pDBNpSub, sn_seq);
	}

	// é©ï™é©êgÇ∆à·Ç§â€ê≈ï˚éÆÇÃã‡äzÇ…èëÇ´çûÇ›
	CString	filter;
	int		nowVersion = 2;
	char	*pHyoID = NULL;
	if( KzHoushiki == ID_ICSSH_GENSOKU ){
		return SetKanniDataInRenew(pDBNpSub, sn_seq);
	}
	else if( KzHoushiki == ID_ICSSH_KANNI ){
		return SetGensokuDataInRenew(pDBNpSub, sn_seq);
	}

	return 0;
}

//yoshida191001
//-----------------------------------------------------------------------------
// èWåvçœÇ›ÉfÅ[É^ÇÃÉZÉbÉg
//-----------------------------------------------------------------------------
// à¯êî	pSyzSyukei	ÅF	è¡îÔê≈èWåvÉNÉâÉX
//-----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	ê≥èÌèIóπ
//			-1		ÅF	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CH31SnHonpyoData::SetCalqedData(CDBNpSub *pDBNpSub, CSyzSyukei *pSyzSyukei)
{
	MoneyBasejagArray money;

	EnumIdIcsShKazeihoushiki KzHoushiki = m_Util.GetKazeihoushiki(pDBNpSub->zvol);
	if (KzHoushiki == ID_ICSSH_MENZEI) {
		return 0;
	}

	if (KzHoushiki == ID_ICSSH_GENSOKU) {
		money = pSyzSyukei->GetShinkokuData(_T("30"));

		int cnt = (int)money.GetCount();
		int incnt = 0;

		for (int i = 0; i < cnt; i++) {
			incnt = (int)money[i].GetCount();
			for (int j = 0; j < incnt; j++) {
				if (money[i][j].code == _T("300100")) {
					memmove(Sn_KSTD, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("300200")) {
					memmove(Sn_SYTX, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("300300")) {
					memmove(Sn_KJADJZ, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("300400")) {
					memmove(Sn_SIREZ, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("300500")) {
					memmove(Sn_HKANZ, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("300600")) {
					memmove(Sn_KSITZ, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("300700")) {
//					memmove(Sn_KJZSK, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("300800")) {
//					memmove(Sn_KJHKZ, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("300900")) {
//					memmove(Sn_SHZ, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("301000")) {
//					memmove(Sn_TYNOFZ, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("301100")) {
//					memmove(Sn_EDNOFZ, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("301200")) {
//					memmove(Sn_TYNKNP, money[i][j].arith, MONY_BUF_SIZE);				
				}
				else if (money[i][j].code == _T("301300")) {
//					memmove(Sn_KAKTIZ, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("301400")) {
//					memmove(Sn_SHN, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("301500")) {
//					memmove(Sn_KZURI, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("301600")) {
//					memmove(Sn_SOURI, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("301700")) {
//					memmove(Sn_TKJHKZ, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("301800")) {
//					memmove(Sn_TSHZ, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("301900")) {
//					memmove(Sn_KNP, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("302000")) {
//					memmove(Sn_NOZ, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("302100")) {
//					memmove(Sn_TTYWAR, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("302200")) {
//					memmove(Sn_TEDNOF, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("302300")) {
//					memmove(Sn_TYNKNPJ, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("302400")) {
//					memmove(Sn_TKAKTZ, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("302500")) {
//					memmove(Sn_SHNJ, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("302600")) {
//					memmove(Sn_SYOTSG, money[i][j].arith, MONY_BUF_SIZE);
				}
			}
		}

	
	}
	else if (KzHoushiki == ID_ICSSH_KANNI) {
		money = pSyzSyukei->GetShinkokuData(_T("33"));

		int cnt = (int)money.GetCount();
		int incnt = 0;

		for (int i = 0; i < cnt; i++) {
			incnt = (int)money[i].GetCount();
			for (int j = 0; j < incnt; j++) {
				if (money[i][j].code == _T("330100")) {
					memmove(Sn_KSTD, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("330200")) {
					memmove(Sn_SYTX, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("330300")) {
					memmove(Sn_KJADJZ, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("330400")) {
					memmove(Sn_SIREZ, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("330500")) {
					memmove(Sn_HKANZ, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("330600")) {
					memmove(Sn_KSITZ, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("330700")) {
//					memmove(Sn_KJZSK, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("330800")) {
//					memmove(Sn_KJHKZ, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("330900")) {
//					memmove(Sn_SHZ, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("331000")) {
//					memmove(Sn_TYNOFZ, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("331100")) {
//					memmove(Sn_EDNOFZ, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("331200")) {
//					memmove(Sn_TYNKNP, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("331300")) {
//					memmove(Sn_KAKTIZ, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("331400")) {
//					memmove(Sn_SHN, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("331500")) {
//					memmove(Sn_KZURI, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("331600")) {
//					memmove(Sn_ZZURI, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("331700")) {
//					memmove(Sn_TKJHKZ, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("331800")) {
//					memmove(Sn_TSHZ, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("331900")) {
//					memmove(Sn_KNP, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("332000")) {
//					memmove(Sn_NOZ, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("332100")) {
//					memmove(Sn_TTYWAR, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("332200")) {
//					memmove(Sn_TEDNOF, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("332300")) {
//					memmove(Sn_TYNKNPJ, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("332400")) {
//					memmove(Sn_TKAKTZ, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("332500")) {
//					memmove(Sn_SHNJ, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("332600")) {
//					memmove(Sn_SYOTSG, money[i][j].arith, MONY_BUF_SIZE);
				}
			}
		}

		money = pSyzSyukei->GetShinkokuData(_T("35"));

		cnt = (int)money.GetCount();
		for (int i = 0; i < cnt; i++) {
			incnt = (int)money[i].GetCount();
			for (int j = 0; j < incnt; j++) {
				// éñã∆ï â€ê≈îÑè„çÇ
				if (money[i][j].code == _T("350700")) {
					memmove(Sn_KURI1, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("350800")) {
					memmove(Sn_KURI2, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("350900")) {
					memmove(Sn_KURI3, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("351000")) {
					memmove(Sn_KURI4, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("351100")) {
					memmove(Sn_KURI5, money[i][j].arith, MONY_BUF_SIZE);
				}
				else if (money[i][j].code == _T("351200")) {
					memmove(Sn_KURI6, money[i][j].arith, MONY_BUF_SIZE);
				}
				// éñã∆ï â€ê≈îÑè„äÑçá
				else if (money[i][j].code == _T("354100")) {
					Sn_UP1 = (unsigned short)m_Util.val_to_int((unsigned char*)money[i][j].arith);
				}
				else if (money[i][j].code == _T("354200")) {
					Sn_UP2 = (unsigned short)m_Util.val_to_int((unsigned char*)money[i][j].arith);
				}
				else if (money[i][j].code == _T("354300")) {
					Sn_UP3 = (unsigned short)m_Util.val_to_int((unsigned char*)money[i][j].arith);
				}
				else if (money[i][j].code == _T("354400")) {
					Sn_UP4 = (unsigned short)m_Util.val_to_int((unsigned char*)money[i][j].arith);
				}
				else if (money[i][j].code == _T("354500")) {
					Sn_UP5 = (unsigned short)m_Util.val_to_int((unsigned char*)money[i][j].arith);
				}
				else if (money[i][j].code == _T("354600")) {
					Sn_UP6 = (unsigned short)m_Util.val_to_int((unsigned char*)money[i][j].arith);
				}
			}
		}
	}

	//ÇQÇUÇ≈ÇÕâ∫ãLèWåvÇ‡Ç†Ç¡ÇΩÇ™ÅAÇRÇPÇ≈ÇÕégÇÌÇÍÇƒÇ»Ç≥ÇªÇ§Ç»ÇÃÇ≈ÇRÇPópÇ…èWåvÉeÅ[ÉuÉãçÏÇÁÇÍÇƒÇ¢Ç»Ç¢ÅBíuÇ´ä∑Ç¶Ç≈Ç´Ç»Ç¢ÇÃÇ≈èàóùÇÕÉRÉÅÉìÉgâª
	//money = pSyzSyukei->GetShinkokuData(_T("23"));

	//int cnt = (int)money.GetCount();
	//int incnt = 0;

	//for (int i = 0; i < cnt; i++) {
	//	incnt = (int)money[i].GetCount();
	//	for (int j = 0; j < incnt; j++) {
	//		if (money[i][j].code == _T("230701")) {
	//			memmove(Sn_KSJT3, money[i][j].arith, MONY_BUF_SIZE);
	//		}
	//		else if (money[i][j].code == _T("230702")) {
	//			memmove(Sn_KSJT4, money[i][j].arith, MONY_BUF_SIZE);
	//		}
	//		else if (money[i][j].code == _T("230703")) {
	//			memmove(Sn_KSJT63, money[i][j].arith, MONY_BUF_SIZE);
	//		}
	//		else if (money[i][j].code == _T("230700")) {
	//			memmove(Sn_KSJTKEI, money[i][j].arith, MONY_BUF_SIZE);
	//		}
	//		else if (money[i][j].code == _T("230803")) {
	//			memmove(Sn_TKST63, money[i][j].arith, MONY_BUF_SIZE);
	//		}
	//		else if (money[i][j].code == _T("230800")) {
	//			memmove(Sn_TKSTKEI, money[i][j].arith, MONY_BUF_SIZE);
	//		}
	//		else if (money[i][j].code == _T("231000")) {
	//			memmove(Sn_URHKNZ, money[i][j].arith, MONY_BUF_SIZE);
	//		}
	//		else if (money[i][j].code == _T("231100")) {
	//			memmove(Sn_TKHKNZ, money[i][j].arith, MONY_BUF_SIZE);
	//		}
	//	}
	//}



	return 0;
}