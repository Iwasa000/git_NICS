#include "StdAfx.h"
#include "H28SnFhyo10Data.h"

static SH_TAGNAME	tagData10_28[] = {
	{_T("ACB00060"), 1 },
	{_T("ACB00070"), 0 },
	{_T("ACB00080"), 3 },

	{_T("ACB00110"), 3 },
	
	{_T("ACE00220"), 1 },
	{_T("ACE00230"), 0 },
	{_T("ACE00240"), 3 },

	{_T("ACE00270"), 3 },
	
	{_T("") }
};

//-----------------------------------------------------------------------------
// ÉRÉìÉXÉgÉâÉNÉ^
//-----------------------------------------------------------------------------
CH28SnFhyo10Data::CH28SnFhyo10Data( BOOL isTransitionalMeasure )
	: CH26SnFhyo10Data(isTransitionalMeasure)
	, m_isTransitionalMeasure(isTransitionalMeasure)
{

	memset( Sn_1F1_1A, '\0', sizeof(Sn_1F1_1A) );
	memset( Sn_1F1_1B, '\0', sizeof(Sn_1F1_1B) );
	memset( Sn_1F1_1C, '\0', sizeof(Sn_1F1_1C) );

	memset( Sn_1F1_2C, '\0', sizeof(Sn_1F1_2C) );

	memset( Sn_1F5_1A, '\0', sizeof(Sn_1F5_1A) );
	memset( Sn_1F5_1B, '\0', sizeof(Sn_1F5_1B) );
	memset( Sn_1F5_1C, '\0', sizeof(Sn_1F5_1C) );

	memset( Sn_1F5_2C, '\0', sizeof(Sn_1F5_2C) );
	

	m_Arith.l_defn( 0x16 );

	memset( rsv1, '\0', sizeof(rsv1) );
	memset( rsv2, '\0', sizeof(rsv2) );

}

//-----------------------------------------------------------------------------
// ÉfÉXÉgÉâÉNÉ^
//-----------------------------------------------------------------------------
CH28SnFhyo10Data::~CH28SnFhyo10Data()
{
	if( m_DBSyzAccess.m_pSH_LST_VAL ){
		m_DBSyzAccess.CloseShLstVal();
	}
}

//-----------------------------------------------------------------------------
// DB Ç©ÇÁÉfÅ[É^ì«çûÇ›
//-----------------------------------------------------------------------------
// à¯êî	pDBNpSub	ÅF	ç‡ñ±ÉNÉâÉXÇÃÉ|ÉCÉìÉ^
//		sn_seq		ÅF	ëŒâûÇÃóöóÉVÅ[ÉPÉìÉX
//-----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	ê≥èÌèIóπ	
//			-1		ÅF	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CH28SnFhyo10Data::GetData( CDBNpSub *pDBNpSub, int sn_seq )
{

	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}

	// äÓíÍÉNÉâÉXÇ≈ÇÃÉfÅ[É^ÇÃéÊìæ
	if( CH26SnFhyo10Data::GetData(pDBNpSub, sn_seq) ){
		return -1;
	}

	CString	filter;
	int		nowVersion = 1;
	filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_1_HYOID, nowVersion );

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


		// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
		if( pShLstVal->itmId == _T("ACB00060") ){//ê≈ó¶ÇRÅììKópï™
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_1F1_1A, pShLstVal->val );
			}
			else{
				memset( Sn_1F1_1A, '\0', sizeof(Sn_1F1_1A) );
			}
		}
		else if( pShLstVal->itmId == _T("ACB00070") ){//ê≈ó¶ÇSÅììKópï™
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_1F1_1B, pShLstVal->val );
			}
			else{
				memset( Sn_1F1_1B, '\0', sizeof(Sn_1F1_1B) );
			}
		}
		else if( pShLstVal->itmId == _T("ACB00080") ){//ê≈ó¶ÇUÅDÇRÅììKópï™
			m_Util.val_to_bin( Sn_1F1_1C, pShLstVal->val );			
		}

		// ì¡íËâ€ê≈édì¸ÇÍÇ…åWÇÈéxï•ëŒâøÇÃäz
		else if( pShLstVal->itmId == _T("ACB00110") ){//ê≈ó¶ÇUÅDÇRÅììKópï™
			m_Util.val_to_bin( Sn_1F1_2C, pShLstVal->val );			
		}

		//îÑè„Ç∞ÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
		else if( pShLstVal->itmId == _T("ACE00220") ){//ê≈ó¶ÇRÅììKópï™
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_1F5_1A, pShLstVal->val );
			}
			else{
				memset( Sn_1F5_1A, '\0', sizeof(Sn_1F5_1A) );
			}
		}
		else if( pShLstVal->itmId == _T("ACE00230") ){//ê≈ó¶ÇSÅììKópï™
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_1F5_1B, pShLstVal->val );
			}
			else{
				memset( Sn_1F5_1B, '\0', sizeof(Sn_1F5_1B) );
			}
		}
		else if( pShLstVal->itmId == _T("ACE00240") ){//ê≈ó¶ÇUÅDÇRÅììKópï™
			m_Util.val_to_bin( Sn_1F5_1C, pShLstVal->val );			
		}

		//ì¡íËâ€ê≈édì¸ÇÍÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
		else if( pShLstVal->itmId == _T("ACE00270") ){//ê≈ó¶ÇUÅDÇRÅììKópï™
			m_Util.val_to_bin( Sn_1F5_2C, pShLstVal->val );			
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
int CH28SnFhyo10Data::SetData( CDBNpSub *pDBNpSub, int sn_seq )
{

	ASSERT( pDBNpSub );

	if( pDBNpSub == NULL ){
		return -1;
	}

	// äÓíÍÉNÉâÉXÇ≈ÇÃÉfÅ[É^ÇÃÉZÉbÉg
	if( CH26SnFhyo10Data::SetData(pDBNpSub, sn_seq) ){
		return -1;
	}

	
	CString	filter;
	char	*pHyoID = NULL;
	int	nowVersion = 1;
	filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_1_HYOID, nowVersion );
	pHyoID = ICS_SN_FHYO_1_HYOID;

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

		// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz ê≈ó¶ÇRÅììKópï™
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("ACB00060");
		m_Util.val_to_asci( &pShLstVal->val, Sn_1F1_1A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz ê≈ó¶ÇSÅììKópï™
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("ACB00070");
		m_Util.val_to_asci( &pShLstVal->val, Sn_1F1_1B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz ê≈ó¶ÇUÅDÇRÅììKópï™
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("ACB00080");
		m_Util.val_to_asci( &pShLstVal->val, Sn_1F1_1C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ì¡íËâ€ê≈édì¸ÇÍÇ…åWÇÈéxï•ëŒâøÇÃäz  ê≈ó¶ÇUÅDÇRÅììKópï™
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("ACB00110");
		m_Util.val_to_asci( &pShLstVal->val, Sn_1F1_2C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		//îÑè„Ç∞ÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz ê≈ó¶ÇRÅììKópï™
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("ACE00220");
		m_Util.val_to_asci( &pShLstVal->val, Sn_1F5_1A );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		//îÑè„Ç∞ÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz ê≈ó¶ÇSÅììKópï™
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("ACE00230");
		m_Util.val_to_asci( &pShLstVal->val, Sn_1F5_1B );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		//îÑè„Ç∞ÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz ê≈ó¶ÇUÅDÇRÅììKópï™
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("ACE00240");
		m_Util.val_to_asci( &pShLstVal->val, Sn_1F5_1C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		//ì¡íËâ€ê≈édì¸ÇÍÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz ê≈ó¶ÇUÅDÇRÅììKópï™
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("ACE00270");
		m_Util.val_to_asci( &pShLstVal->val, Sn_1F5_2C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();


	}
	else{// ÉfÅ[É^óLÇË*/
		
		
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

			// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz ê≈ó¶ÇRÅììKópï™
			if( pShLstVal->itmId == _T("ACB00060") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_1F1_1A );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz ê≈ó¶ÇSÅììKópï™
			else if( pShLstVal->itmId == _T("ACB00070") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_1F1_1B );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz ê≈ó¶ÇUÅDÇRÅììKópï™
			else if( pShLstVal->itmId == _T("ACB00080") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_1F1_1C );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			// ì¡íËâ€ê≈édì¸ÇÍÇ…åWÇÈéxï•ëŒâøÇÃäz  ê≈ó¶ÇUÅDÇRÅììKópï™
			else if( pShLstVal->itmId == _T("ACB00110") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_1F1_2C );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			//îÑè„Ç∞ÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz ê≈ó¶ÇRÅììKópï™
			else if( pShLstVal->itmId == _T("ACE00220") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_1F5_1A );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			//îÑè„Ç∞ÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz ê≈ó¶ÇSÅììKópï™
			else if( pShLstVal->itmId == _T("ACE00230") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_1F5_1B );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			//îÑè„Ç∞ÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz ê≈ó¶ÇUÅDÇRÅììKópï™
			else if( pShLstVal->itmId == _T("ACE00240") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_1F5_1C );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			//ì¡íËâ€ê≈édì¸ÇÍÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz ê≈ó¶ÇUÅDÇRÅììKópï™
			else if( pShLstVal->itmId == _T("ACE00270") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_1F5_2C );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}


		}

		// çXêVÉfÅ[É^ämîF
		int	adCnt = 0;
		for( int i=0;; i++ ){
			if( strlen(tagData10_28[i].tagName) == 0 ){
				break;
			}
			if( m_isTransitionalMeasure== FALSE ){
				if( (tagData10_28[i].sign1!=0) && (tagData10_28[i].sign1!=1) ){
					adCnt++;
				}
			}
			else{
				adCnt++;
			}
		}

		int updCnt = (int)updTagAry.GetCount();
		if( adCnt != updCnt ){

			// ñ≥Ç¢Ç‡ÇÃÇí«â¡
			CArray<unsigned char*>	MonyPtrAry;
			MonyPtrAry.Add( Sn_1F1_1A );
			MonyPtrAry.Add( Sn_1F1_1B );
			MonyPtrAry.Add( Sn_1F1_1C );

			MonyPtrAry.Add( Sn_1F1_2C );

			MonyPtrAry.Add( Sn_1F5_1A );
			MonyPtrAry.Add( Sn_1F5_1B );
			MonyPtrAry.Add( Sn_1F5_1C );

			MonyPtrAry.Add( Sn_1F5_2C );

			BOOL findFlg = FALSE;
			for( int i=0;; i++ ){
				if( strlen(tagData10_28[i].tagName) == 0 ){
					break;
				}
				if( m_isTransitionalMeasure == FALSE ){
					if( (tagData10_28[i].sign1==0) || (tagData10_28[i].sign1==1) ){
						continue;
					}
				}

				findFlg = FALSE;
				for( int j=0; j<updCnt; j++ ){
					if( updTagAry[j].CompareNoCase(tagData10_28[i].tagName) == 0 ){
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
					pShLstVal->itmId = tagData10_28[i].tagName;
					m_Util.val_to_asci( &pShLstVal->val, MonyPtrAry[i] );
					pShLstVal->sign1 = 0;
					pShLstVal->Update();
				}
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
int CH28SnFhyo10Data::SetCalqedData( CSyzSyukei *pSyzSyukei, BOOL &isPluralZei )
{

	isPluralZei = FALSE;

	MoneyBasejagArray money;

	money = pSyzSyukei->GetShinkokuData( _T("21") );

	int cnt = (int)money.GetCount();
	int incnt = 0;

	unsigned	char	exZeiflg = 0x00;	// Å†Å†Å†Å† Å†Å°Å°Å°
											//  D0ÅF3%
											//  D1ÅF5%
											//  D2ÅF8%

	for( int i=0; i<cnt; i++ ){
		incnt = (int)money[i].GetCount();
		for( int j=0; j<incnt; j++ ){
			if( money[i][j].code.Left(4) == _T("2101") ){
				if( money[i][j].code == _T("210101") ){//â€ê≈ïWèÄäzÇRÅììKópï™
					memmove( Sn_1F1A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("210102") ){//â€ê≈ïWèÄäzÇSÅììKópï™
					memmove( Sn_1F1B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("210103") ){//â€ê≈ïWèÄäzÇUÅDÇRÅììKópï™
					memmove( Sn_1F1C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}

			else if( money[i][j].code.Left(4) == _T("2102") ){
				if( money[i][j].code == _T("210201") ){//â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäzÅ@ÇRÅììKópï™
					memmove( Sn_1F1_1A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("210202") ){//â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäzÅ@ÇSÅììKópï™
					memmove( Sn_1F1_1B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("210203") ){//â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäzÅ@ÇUÅDÇRÅììKópï™
					memmove( Sn_1F1_1C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}

			else if( money[i][j].code.Left(4) == _T("2103") ){
				
				if( money[i][j].code == _T("210303") ){//ì¡íËâ€ê≈édì¸ÇÍÇ…åWÇÈéxï•ëŒâøÇÃäzÅ@ÇUÅDÇRÅììKópï™
					memmove( Sn_1F1_2C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}

			else if( money[i][j].code.Left(4) == _T("2104") ){
				
				if( money[i][j].code == _T("210401") ){//è¡îÔê≈äzÅ@ÇRÅììKópï™
					memmove( Sn_1F2A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
				else if( money[i][j].code == _T("210402") ){//è¡îÔê≈äzÅ@ÇSÅììKópï™
					memmove( Sn_1F2B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("210403") ){//è¡îÔê≈äzÅ@ÇUÅDÇRÅììKópï™
					memmove( Sn_1F2C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}
			/*
			else if( money[i][j].code.Left(4) == _T("2105") ){
				if( money[i][j].code == _T("210501") ){//çTèúâﬂëÂí≤êÆê≈äzÅ@ÇRÅììKópï™
					memmove( Sn_1F3A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("210502") ){//çTèúâﬂëÂí≤êÆê≈äzÅ@ÇSÅììKópï™
					memmove( Sn_1F3B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("210503") ){//çTèúâﬂëÂí≤êÆê≈äzÅ@ÇUÅDÇRÅììKópï™
					memmove( Sn_1F3C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}
			*/

			else if( money[i][j].code.Left(4) == _T("2106") ){
				if( money[i][j].code == _T("210601") ){//çTèúëŒè€édì¸ê≈äzÅ@ÇRÅììKópï™
					memmove( Sn_1F4A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("210602") ){//çTèúëŒè€édì¸ê≈äzÅ@ÇSÅììKópï™
					memmove( Sn_1F4B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("210603") ){//çTèúëŒè€édì¸ê≈äzÅ@ÇUÅDÇRÅììKópï™
					memmove( Sn_1F4C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}
			else if( money[i][j].code.Left(4) == _T("2107") ){
				if( money[i][j].code == _T("210701") ){//ï‘ä“ìôëŒâøÇ…åWÇÈê≈äzÅ@ÇRÅììKópï™
					memmove( Sn_1F5A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("210702") ){//ï‘ä“ìôëŒâøÇ…åWÇÈê≈äzÅ@ÇSÅììKópï™
					memmove( Sn_1F5B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("210703") ){//ï‘ä“ìôëŒâøÇ…åWÇÈê≈äzÅ@ÇUÅDÇRÅììKópï™
					memmove( Sn_1F5C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}

			else if( money[i][j].code.Left(4) == _T("2108") ){
				if( money[i][j].code == _T("210801") ){//îÑè„Ç∞ÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äzÅ@ÇRÅììKópï™
					memmove( Sn_1F5_1A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("210802") ){//îÑè„Ç∞ÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äzÅ@ÇSÅììKópï™
					memmove( Sn_1F5_1B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("210803") ){//îÑè„Ç∞ÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äzÅ@ÇUÅDÇRÅììKópï™
					memmove( Sn_1F5_1C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}
			else if( money[i][j].code.Left(4) == _T("2109") ){
				
				if( money[i][j].code == _T("210903") ){//ì¡íËâ€ê≈édì¸ÇÍÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äzÅ@ÇUÅDÇRÅììKópï™
					memmove( Sn_1F5_2C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}

			else if( money[i][j].code.Left(4) == _T("2110") ){
				if( money[i][j].code == _T("211001") ){//ë›ì|ÇÍÇ…åWÇÈê≈äzÅ@ÇRÅììKópï™
					memmove( Sn_1F6A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("211002") ){//ë›ì|ÇÍÇ…åWÇÈê≈äzÅ@ÇSÅììKópï™
					memmove( Sn_1F6B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("211003") ){//ë›ì|ÇÍÇ…åWÇÈê≈äzÅ@ÇUÅDÇRÅììKópï™
					memmove( Sn_1F6C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}

			else if( money[i][j].code.Left(4) == _T("2112") ){
				if( money[i][j].code == _T("211201") ){//çTèúïsë´ä“ïtê≈äzÅ@ÇRÅììKópï™
					memmove( Sn_1F8A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("211202") ){//çTèúïsë´ä“ïtê≈äzÅ@ÇSÅììKópï™
					memmove( Sn_1F8B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("211203") ){//çTèúïsë´ä“ïtê≈äzÅ@ÇUÅDÇRÅììKópï™
					memmove( Sn_1F8C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}

			else if( money[i][j].code.Left(4) == _T("2113") ){
				if( money[i][j].code == _T("211301") ){//ç∑à¯ê≈äzÅ@ÇRÅììKópï™
					memmove( Sn_1F9A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("211302") ){//ç∑à¯ê≈äzÅ@ÇSÅììKópï™
					memmove( Sn_1F9B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("211303") ){//ç∑à¯ê≈äzÅ@ÇUÅDÇRÅììKópï™
					memmove( Sn_1F9C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}
			else if( money[i][j].code.Left(4) == _T("2114") ){//çáåvç∑à¯ê≈äz
				if( money[i][j].code == _T("211400") ){
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
// çXêVéûÇÃã‡äzÉNÉäÉA
//-----------------------------------------------------------------------------
// à¯êî	pDBNpSub	ÅF	ç‡ñ±ÉNÉâÉX
//		type		ÅF	
//-----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	ê≥èÌèIóπ
//-----------------------------------------------------------------------------
int CH28SnFhyo10Data::ClearDataByRenew( CDBNpSub *pDBNpSub, int type )
{
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}

	// äÓíÍÉNÉâÉXÇ≈ÇÃÉfÅ[É^ÇÃéÊìæ
	if( CH26SnFhyo10Data::ClearDataByRenew(pDBNpSub, type) ){
		return -1;
	}

	//â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
	m_Arith.l_clear( Sn_1F1_1A );
	m_Arith.l_clear( Sn_1F1_1B );
	m_Arith.l_clear( Sn_1F1_1C );

	//ì¡íËâ€ê≈édì¸ÇÍÇ…åWÇÈéxï•ëŒâøÇÃäz
	m_Arith.l_clear( Sn_1F1_2C );

	//îÑè„Ç∞ÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
	m_Arith.l_clear( Sn_1F5_1A );
	m_Arith.l_clear( Sn_1F5_1B );
	m_Arith.l_clear( Sn_1F5_1C );

	//ì¡íËâ€ê≈édì¸ÇÍÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
	m_Arith.l_clear( Sn_1F5_2C );


	return 0;

}