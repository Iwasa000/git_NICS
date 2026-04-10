#include "StdAfx.h"
#include "H26SnFhyo50Data.h"

static SH_TAGNAME	tagData50[] = {
	{_T("AHG00040"), 1 },
	{_T("AHG00050"), 0 },
	{_T("AHG00060"), 3 },
	{_T("AHG00070"), 999 },

	{_T("AHG00090"), 1 },
	{_T("AHG00100"), 0 },
	{_T("AHG00110"), 3 },

	{_T("AHG00150"), 1 },
	{_T("AHG00160"), 0 },
	{_T("AHG00170"), 3 },

	{_T("AHG00210"), 1 },
	{_T("AHG00220"), 0 },
	{_T("AHG00230"), 3 },

	{_T("AHG00270"), 1 },
	{_T("AHG00280"), 0 },
	{_T("AHG00290"), 3 },

	{_T("AHG00330"), 1 },
	{_T("AHG00340"), 0 },
	{_T("AHG00350"), 3 },

//	{_T("AHG00400"), 1 },
//	{_T("AHG00410"), 0 },
//	{_T("AHG00420"), 3 },
	{_T("AHG00430"), 999 },

	{_T("AHG00450"), 1 },
	{_T("AHG00460"), 0 },
	{_T("AHG00470"), 3 },
	{_T("AHG00480"), 999 },

	{_T("AHG00500"), 1 },
	{_T("AHG00510"), 0 },
	{_T("AHG00520"), 3 },
	{_T("AHG00530"), 999 },

	{_T("AHG00550"), 1 },
	{_T("AHG00560"), 0 },
	{_T("AHG00570"), 3 },
	{_T("AHG00580"), 999 },

	{_T("AHG00600"), 1 },
	{_T("AHG00610"), 0 },
	{_T("AHG00620"), 3 },
	{_T("AHG00630"), 999 },

	{_T("AHG00650"), 1 },
	{_T("AHG00660"), 0 },
	{_T("AHG00670"), 3 },
	{_T("AHG00680"), 999 },

	{_T("AHG00830"), 1 },
	{_T("AHG00840"), 0 },
	{_T("AHG00850"), 3 },
	{_T("AHG00860"), 999 },	// '14.12.19

	{_T("AHG00880"), 1 },
	{_T("AHG00890"), 0 },
	{_T("AHG00900"), 3 },
	{_T("AHG00910"), 999 },	// '14.12.19

	{_T("AHG00930"), 1 },
	{_T("AHG00940"), 0 },
	{_T("AHG00950"), 3 },
	{_T("AHG00960"), 999 },	// '14.12.19

	{_T("AHG00980"), 1 },
	{_T("AHG00990"), 0 },
	{_T("AHG01000"), 3 },
	{_T("AHG01010"), 999 },	// '14.12.19

	{_T("AHG01030"), 1 },
	{_T("AHG01040"), 0 },
	{_T("AHG01050"), 3 },
	{_T("AHG01060"), 999 },	// '14.12.19

	{_T("AHG01080"), 1 },
	{_T("AHG01090"), 0 },
	{_T("AHG01100"), 3 },
	{_T("AHG01110"), 999 },	// '14.12.19

	{_T("AHG01130"), 1 },
	{_T("AHG01140"), 0 },
	{_T("AHG01150"), 3 },
	{_T("AHG01160"), 999 },	// '14.12.19

	{_T("AHG01180"), 1 },
	{_T("AHG01190"), 0 },
	{_T("AHG01200"), 3 },
	{_T("AHG01210"), 999 },	// '14.12.19

	{_T("AHG01230"), 1 },
	{_T("AHG01240"), 0 },
	{_T("AHG01250"), 3 },
	{_T("AHG01260"), 999 },	// '14.12.19

	{_T("AHG01280"), 1 },
	{_T("AHG01290"), 0 },
	{_T("AHG01300"), 3 },
	{_T("AHG01310"), 999 },	// '14.12.19

	{_T("AHG00710"), 1 },
	{_T("AHG00720"), 0 },
	{_T("AHG00730"), 3 },
	{_T("AHG00740"), 999 },
	{_T("AHG00770"), 1 },
	{_T("AHG00780"), 0 },
	{_T("AHG00790"), 3 },
	{_T("AHG00800"), 999 },

	{_T("") }
};

//-----------------------------------------------------------------------------
// ÉRÉìÉXÉgÉâÉNÉ^
//-----------------------------------------------------------------------------
CH26SnFhyo50Data::CH26SnFhyo50Data( BOOL isTransitionalMeasure )
	: m_isTransitionalMeasure(isTransitionalMeasure)
{
	memset( Sn_5FURIK, '\0', sizeof(Sn_5FURIK) );
	memset( Sn_5FURITA, '\0', sizeof(Sn_5FURITA) );
	memset( Sn_5FURITB, '\0', sizeof(Sn_5FURITB) );
	memset( Sn_5FURITC, '\0', sizeof(Sn_5FURITC) );

	memset( Sn_5FURI1A, '\0', sizeof(Sn_5FURI1A) );
	memset( Sn_5FURI2A, '\0', sizeof(Sn_5FURI2A) );
	memset( Sn_5FURI3A, '\0', sizeof(Sn_5FURI3A) );
	memset( Sn_5FURI4A, '\0', sizeof(Sn_5FURI4A) );
	memset( Sn_5FURI5A, '\0', sizeof(Sn_5FURI5A) );

	memset( Sn_5FURI1B, '\0', sizeof(Sn_5FURI1B) );
	memset( Sn_5FURI2B, '\0', sizeof(Sn_5FURI2B) );
	memset( Sn_5FURI3B, '\0', sizeof(Sn_5FURI3B) );
	memset( Sn_5FURI4B, '\0', sizeof(Sn_5FURI4B) );
	memset( Sn_5FURI5B, '\0', sizeof(Sn_5FURI5B) );

	memset( Sn_5FURI1C, '\0', sizeof(Sn_5FURI1C) );
	memset( Sn_5FURI2C, '\0', sizeof(Sn_5FURI2C) );
	memset( Sn_5FURI3C, '\0', sizeof(Sn_5FURI3C) );
	memset( Sn_5FURI4C, '\0', sizeof(Sn_5FURI4C) );
	memset( Sn_5FURI5C, '\0', sizeof(Sn_5FURI5C) );

	memset( Sn_5FUZTT, '\0', sizeof(Sn_5FUZTT) );
	memset( Sn_5FUZT1, '\0', sizeof(Sn_5FUZT1) );
	memset( Sn_5FUZT2, '\0', sizeof(Sn_5FUZT2) );
	memset( Sn_5FUZT3, '\0', sizeof(Sn_5FUZT3) );
	memset( Sn_5FUZT4, '\0', sizeof(Sn_5FUZT4) );
	memset( Sn_5FUZT5, '\0', sizeof(Sn_5FUZT5) );

	memset( Sn_5FUZKTA, '\0', sizeof(Sn_5FUZKTA) );
	memset( Sn_5FUZK1A, '\0', sizeof(Sn_5FUZK1A) );
	memset( Sn_5FUZK2A, '\0', sizeof(Sn_5FUZK2A) );
	memset( Sn_5FUZK3A, '\0', sizeof(Sn_5FUZK3A) );
	memset( Sn_5FUZK4A, '\0', sizeof(Sn_5FUZK4A) );
	memset( Sn_5FUZK5A, '\0', sizeof(Sn_5FUZK5A) );

	memset( Sn_5FUZKTB, '\0', sizeof(Sn_5FUZKTB) );
	memset( Sn_5FUZK1B, '\0', sizeof(Sn_5FUZK1B) );
	memset( Sn_5FUZK2B, '\0', sizeof(Sn_5FUZK2B) );
	memset( Sn_5FUZK3B, '\0', sizeof(Sn_5FUZK3B) );
	memset( Sn_5FUZK4B, '\0', sizeof(Sn_5FUZK4B) );
	memset( Sn_5FUZK5B, '\0', sizeof(Sn_5FUZK5B) );

	memset( Sn_5FUZKTC, '\0', sizeof(Sn_5FUZKTC) );
	memset( Sn_5FUZK1C, '\0', sizeof(Sn_5FUZK1C) );
	memset( Sn_5FUZK2C, '\0', sizeof(Sn_5FUZK2C) );
	memset( Sn_5FUZK3C, '\0', sizeof(Sn_5FUZK3C) );
	memset( Sn_5FUZK4C, '\0', sizeof(Sn_5FUZK4C) );
	memset( Sn_5FUZK5C, '\0', sizeof(Sn_5FUZK5C) );

	memset( Sn_5FSZTG, '\0', sizeof(Sn_5FSZTG) );
	memset( Sn_5FSZKGA, '\0', sizeof(Sn_5FSZKGA) );
	memset( Sn_5FSZKGB, '\0', sizeof(Sn_5FSZKGB) );
	memset( Sn_5FSZKGC, '\0', sizeof(Sn_5FSZKGC) );

	memset( Sn_5FSZTT, '\0', sizeof(Sn_5FSZTT) );
	memset( Sn_5FSZKTA, '\0', sizeof(Sn_5FSZKTA) );
	memset( Sn_5FSZKTB, '\0', sizeof(Sn_5FSZKTB) );
	memset( Sn_5FSZKTC, '\0', sizeof(Sn_5FSZKTC) );

	memset( Sn_5FSZT20T, '\0', sizeof(Sn_5FSZT20T) );
	memset( Sn_5FSZT20A, '\0', sizeof(Sn_5FSZT20A) );
	memset( Sn_5FSZT20B, '\0', sizeof(Sn_5FSZT20B) );
	memset( Sn_5FSZT20C, '\0', sizeof(Sn_5FSZT20C) );

	memset( Sn_5FSZT21T, '\0', sizeof(Sn_5FSZT21T) );
	memset( Sn_5FSZT21A, '\0', sizeof(Sn_5FSZT21A) );
	memset( Sn_5FSZT21B, '\0', sizeof(Sn_5FSZT21B) );
	memset( Sn_5FSZT21C, '\0', sizeof(Sn_5FSZT21C) );

	memset( Sn_5FSZT22T, '\0', sizeof(Sn_5FSZT22T) );
	memset( Sn_5FSZT22A, '\0', sizeof(Sn_5FSZT22A) );
	memset( Sn_5FSZT22B, '\0', sizeof(Sn_5FSZT22B) );
	memset( Sn_5FSZT22C, '\0', sizeof(Sn_5FSZT22C) );

	memset( Sn_5FSZT23T, '\0', sizeof(Sn_5FSZT23T) );
	memset( Sn_5FSZT23A, '\0', sizeof(Sn_5FSZT23A) );
	memset( Sn_5FSZT23B, '\0', sizeof(Sn_5FSZT23B) );
	memset( Sn_5FSZT23C, '\0', sizeof(Sn_5FSZT23C) );

	memset( Sn_5FSZT24T, '\0', sizeof(Sn_5FSZT24T) );
	memset( Sn_5FSZT24A, '\0', sizeof(Sn_5FSZT24A) );
	memset( Sn_5FSZT24B, '\0', sizeof(Sn_5FSZT24B) );
	memset( Sn_5FSZT24C, '\0', sizeof(Sn_5FSZT24C) );

	memset( Sn_5FSZT25T, '\0', sizeof(Sn_5FSZT25T) );
	memset( Sn_5FSZT25A, '\0', sizeof(Sn_5FSZT25A) );
	memset( Sn_5FSZT25B, '\0', sizeof(Sn_5FSZT25B) );
	memset( Sn_5FSZT25C, '\0', sizeof(Sn_5FSZT25C) );

	memset( Sn_5FSZT26T, '\0', sizeof(Sn_5FSZT26T) );
	memset( Sn_5FSZT26A, '\0', sizeof(Sn_5FSZT26A) );
	memset( Sn_5FSZT26B, '\0', sizeof(Sn_5FSZT26B) );
	memset( Sn_5FSZT26C, '\0', sizeof(Sn_5FSZT26C) );

	memset( Sn_5FSZT27T, '\0', sizeof(Sn_5FSZT27T) );
	memset( Sn_5FSZT27A, '\0', sizeof(Sn_5FSZT27A) );
	memset( Sn_5FSZT27B, '\0', sizeof(Sn_5FSZT27B) );
	memset( Sn_5FSZT27C, '\0', sizeof(Sn_5FSZT27C) );

	memset( Sn_5FSZT28T, '\0', sizeof(Sn_5FSZT28T) );
	memset( Sn_5FSZT28A, '\0', sizeof(Sn_5FSZT28A) );
	memset( Sn_5FSZT28B, '\0', sizeof(Sn_5FSZT28B) );
	memset( Sn_5FSZT28C, '\0', sizeof(Sn_5FSZT28C) );

	memset( Sn_5FSZT29T, '\0', sizeof(Sn_5FSZT29T) );
	memset( Sn_5FSZT29A, '\0', sizeof(Sn_5FSZT29A) );
	memset( Sn_5FSZT29B, '\0', sizeof(Sn_5FSZT29B) );
	memset( Sn_5FSZT29C, '\0', sizeof(Sn_5FSZT29C) );

	Sn_5FUP1 = Sn_5FUP2 = Sn_5FUP3 = Sn_5FUP4 = Sn_5FUP5 = 0;
	Sn_5FSZsw = Sn_5FSBsw = Sn_5FCQsw = Sn_5FGKsw = 0;
	Sn_5FUZK1sw = Sn_5FUZK2sw = Sn_5FUZK3sw = Sn_5FUZK4sw = Sn_5FUZK5sw = 0;
	Sn_5FURITsw = 0;
	Sn_5FUZKTsw = 0;

	Sn_5FSZT19sw = 0;
	Sn_5FSZT20sw = Sn_5FSZT21sw = Sn_5FSZT22sw = Sn_5FSZT23sw = Sn_5FSZT24sw = 0;
	Sn_5FSZT25sw = Sn_5FSZT26sw = Sn_5FSZT27sw = Sn_5FSZT28sw = Sn_5FSZT29sw = 0;

	m_Arith.l_defn( 0x16 );

	memset( rsv1, '\0', sizeof(rsv1) );
	memset( rsv2, '\0', sizeof(rsv2) );
}

//-----------------------------------------------------------------------------
// ÉfÉXÉgÉâÉNÉ^
//-----------------------------------------------------------------------------
CH26SnFhyo50Data::~CH26SnFhyo50Data()
{
	m_DBSyzAccess.CloseShLstVal();
}

#ifdef _20130219_EXTEND_
//-----------------------------------------------------------------------------
// DB Ç©ÇÁÉfÅ[É^ì«çûÇ›
//-----------------------------------------------------------------------------
// à¯êî	pDBNpSub	ÅF	ç‡ñ±ÉNÉâÉXÇÃÉ|ÉCÉìÉ^
//		sn_seq		ÅF	ëŒâûÇÃóöóÉVÅ[ÉPÉìÉX
//-----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	ê≥èÌèIóπ	
//			-1		ÅF	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CH26SnFhyo50Data::GetData( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}

	CString	filter;
	if( m_isTransitionalMeasure ){
		filter.Format( _T("sn_seq = %d and hyoId = \'%s\'"), sn_seq, ICS_SN_FHYO_5_2_HYOID );
	}
	else{
		filter.Format( _T("sn_seq = %d and hyoId = \'%s\'"), sn_seq, ICS_SN_FHYO_5_HYOID );
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

	if( m_isTransitionalMeasure ){
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

			if( pShLstVal->itmId == _T("AHG00070") ){			// 6.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz
				m_Util.val_to_bin( Sn_5FURIK, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AHG00040") ){
				m_Util.val_to_bin( Sn_5FURITA, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AHG00050") ){
				m_Util.val_to_bin( Sn_5FURITB, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AHG00060") ){
				m_Util.val_to_bin( Sn_5FURITC, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AHG00090") ){		// 7.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ëÊÇPã∆éÌ)
				m_Util.val_to_bin( Sn_5FURI1A, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AHG00100") ){
				m_Util.val_to_bin( Sn_5FURI1B, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AHG00110") ){
				m_Util.val_to_bin( Sn_5FURI1C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AHG00150") ){		// 8.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ëÊÇQã∆éÌ)
				m_Util.val_to_bin( Sn_5FURI2A, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AHG00160") ){
				m_Util.val_to_bin( Sn_5FURI2B, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AHG00170") ){
				m_Util.val_to_bin( Sn_5FURI2C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AHG00210") ){		// 9.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ëÊÇRã∆éÌ)
				m_Util.val_to_bin( Sn_5FURI3A, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AHG00220") ){
				m_Util.val_to_bin( Sn_5FURI3B, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AHG00230") ){
				m_Util.val_to_bin( Sn_5FURI3C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AHG00270") ){		// 10.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ëÊÇSã∆éÌ)
				m_Util.val_to_bin( Sn_5FURI4A, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AHG00280") ){
				m_Util.val_to_bin( Sn_5FURI4B, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AHG00290") ){
				m_Util.val_to_bin( Sn_5FURI4C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AHG00330") ){		// 11.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ëÊÇTã∆éÌ)
				m_Util.val_to_bin( Sn_5FURI5A, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AHG00340") ){
				m_Util.val_to_bin( Sn_5FURI5B, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AHG00350") ){
				m_Util.val_to_bin( Sn_5FURI5C, pShLstVal->val );
			}



			else if( pShLstVal->itmId == _T("AHG00430") ){	// 12.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväz(ä˙ä‘åv)
				m_Util.val_to_bin( Sn_5FUZTT, pShLstVal->val );
			}			
			else if( pShLstVal->itmId == _T("AHG00480") ){	// 13.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊàÍéÌÅj
				m_Util.val_to_bin( Sn_5FUZT1, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AHG00530") ){	// 14.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊìÒéÌÅj
				m_Util.val_to_bin( Sn_5FUZT2, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AHG00580") ){	// 15.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊéOéÌÅj
				m_Util.val_to_bin( Sn_5FUZT3, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AHG00630") ){	// 16.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊéléÌÅj
				m_Util.val_to_bin( Sn_5FUZT4, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AHG00680") ){	// 17.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊå‹éÌÅj
				m_Util.val_to_bin( Sn_5FUZT5, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AHG00740") ){	// 18.å¥ë•åvéZÇìKópÇ∑ÇÈèÍçáÇÃçTèúëŒè€édì¸ê≈äz
				m_Util.val_to_bin( Sn_5FSZTG, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AHG00800") ){	// 19.ì¡ó·åvéZÇìKópÇ∑ÇÈèÍçáÇÃçTèúëŒè€édì¸ê≈äz
				m_Util.val_to_bin( Sn_5FSZTG, pShLstVal->val );
			}
		}
	}
	else{
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

			// â€ê≈ïWèÄäzÇ…ëŒÇ∑ÇÈè¡îÔê≈äz
//			if( pShLstVal->itmId == _T("AGB00000") ){
//				pShLstVal->Edit();
//				m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZTT );
//				pShLstVal->Update();
//			}

			// ë›ì|âÒé˚Ç…åWÇÈè¡îÔê≈äz
//			if( pShLstVal->itmId == _T("AGC00000") ){
//				pShLstVal->Edit();
//				m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZTT );
//				pShLstVal->Update();
//			}

			// îÑè„ëŒâøÇÃï‘ä“
//			if( pShLstVal->itmId == _T("AGD00000") ){
//				pShLstVal->Edit();
//				m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZTT );
//				pShLstVal->Update();
//			}

			if( pShLstVal->itmId == _T("AGG00030") ){		// 6.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ä˙ä‘åv)
				m_Util.val_to_bin( Sn_5FURIK, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AGG00200") ){	// 12.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväz(ä˙ä‘åv)
				m_Util.val_to_bin( Sn_5FUZTT, pShLstVal->val );
			}			
			else if( pShLstVal->itmId == _T("AGG00210") ){	// 13.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊàÍéÌÅj
				m_Util.val_to_bin( Sn_5FUZT1, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AGG00220") ){	// 14.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊìÒéÌÅj
				m_Util.val_to_bin( Sn_5FUZT2, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AGG00230") ){	// 15.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊéOéÌÅj
				m_Util.val_to_bin( Sn_5FUZT3, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AGG00240") ){	// 16.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊéléÌÅj
				m_Util.val_to_bin( Sn_5FUZT4, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AGG00250") ){	// 17.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊå‹éÌÅj
				m_Util.val_to_bin( Sn_5FUZT5, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AGG00270") ){	// 18.å¥ë•åvéZÇìKópÇ∑ÇÈèÍçáÇÃçTèúëŒè€édì¸ê≈äz
				m_Util.val_to_bin( Sn_5FSZTG, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AGG00310") ){	// 19.ì¡ó·åvéZÇìKópÇ∑ÇÈèÍçáÇÃçTèúëŒè€édì¸ê≈äz
				m_Util.val_to_bin( Sn_5FSZTG, pShLstVal->val );
			}
		}
	}

	return 0;
}
#else
//-----------------------------------------------------------------------------
// DB Ç©ÇÁÉfÅ[É^ì«çûÇ›
//-----------------------------------------------------------------------------
// à¯êî	pDBNpSub	ÅF	ç‡ñ±ÉNÉâÉXÇÃÉ|ÉCÉìÉ^
//		sn_seq		ÅF	ëŒâûÇÃóöóÉVÅ[ÉPÉìÉX
//-----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	ê≥èÌèIóπ	
//			-1		ÅF	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CH26SnFhyo50Data::GetData( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}

	CString	filter;
	int		nowVersion = 1;
	filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_5_2_HYOID, nowVersion );

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

		if( pShLstVal->itmId == _T("AHG00070") ){			// 6.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz
			m_Util.val_to_bin( Sn_5FURIK, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AHG00040") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FURITA, pShLstVal->val );
			}
			else{
				memset( Sn_5FURITA, '\0', sizeof(Sn_5FURITA) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG00050") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FURITB, pShLstVal->val );
			}
			else{
				memset( Sn_5FURITB, '\0', sizeof(Sn_5FURITB) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG00060") ){
			m_Util.val_to_bin( Sn_5FURITC, pShLstVal->val );
			Sn_5FURITsw = pShLstVal->sign1;
		}
		else if( pShLstVal->itmId == _T("AHG00090") ){		// 7.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ëÊÇPã∆éÌ)
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FURI1A, pShLstVal->val );
			}
			else{
				memset( Sn_5FURI1A, '\0', sizeof(Sn_5FURI1A) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG00100") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FURI1B, pShLstVal->val );
			}
			else{
				memset( Sn_5FURI1B, '\0', sizeof(Sn_5FURI1B) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG00110") ){
			m_Util.val_to_bin( Sn_5FURI1C, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AHG00150") ){		// 8.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ëÊÇQã∆éÌ)
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FURI2A, pShLstVal->val );
			}
			else{
				memset( Sn_5FURI2A, '\0', sizeof(Sn_5FURI2A) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG00160") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FURI2B, pShLstVal->val );
			}
			else{
				memset( Sn_5FURI2B, '\0', sizeof(Sn_5FURI2B) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG00170") ){
			m_Util.val_to_bin( Sn_5FURI2C, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AHG00210") ){		// 9.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ëÊÇRã∆éÌ)
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FURI3A, pShLstVal->val );
			}
			else{
				memset( Sn_5FURI3A, '\0', sizeof(Sn_5FURI3A) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG00220") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FURI3B, pShLstVal->val );
			}
			else{
				memset( Sn_5FURI3B, '\0', sizeof(Sn_5FURI3B) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG00230") ){
			m_Util.val_to_bin( Sn_5FURI3C, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AHG00270") ){		// 10.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ëÊÇSã∆éÌ)
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FURI4A, pShLstVal->val );
			}
			else{
				memset( Sn_5FURI4A, '\0', sizeof(Sn_5FURI4A) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG00280") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FURI4B, pShLstVal->val );
			}
			else{
				memset( Sn_5FURI4B, '\0', sizeof(Sn_5FURI4B) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG00290") ){
			m_Util.val_to_bin( Sn_5FURI4C, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AHG00330") ){		// 11.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ëÊÇTã∆éÌ)
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FURI5A, pShLstVal->val );
			}
			else{
				memset( Sn_5FURI5A, '\0', sizeof(Sn_5FURI5A) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG00340") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FURI5B, pShLstVal->val );
			}
			else{
				memset( Sn_5FURI5B, '\0', sizeof(Sn_5FURI5B) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG00350") ){
			m_Util.val_to_bin( Sn_5FURI5C, pShLstVal->val );
		}


		else if( pShLstVal->itmId == _T("AHG00430") ){	// 12.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväz(ä˙ä‘åv)
			m_Util.val_to_bin( Sn_5FUZTT, pShLstVal->val );
			Sn_5FUZKTsw = pShLstVal->sign1;
		}
		else if( pShLstVal->itmId == _T("AHG00450") ){	//[Y] 13.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇPéÌÅj
			m_Util.val_to_bin( Sn_5FUZK1A, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AHG00460") ){	//[Y] 13.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇPéÌÅj
			m_Util.val_to_bin( Sn_5FUZK1B, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AHG00470") ){	//[Y] 13.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇPéÌÅj
			m_Util.val_to_bin( Sn_5FUZK1C, pShLstVal->val );
			Sn_5FUZK1sw = pShLstVal->sign1;
		}

		else if( pShLstVal->itmId == _T("AHG00480") ){	// 13.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊàÍéÌÅj
			m_Util.val_to_bin( Sn_5FUZT1, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AHG00500") ){	//[Y] 14.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇQéÌÅj
			m_Util.val_to_bin( Sn_5FUZK2A, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AHG00510") ){	//[Y] 14.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇQéÌÅj
			m_Util.val_to_bin( Sn_5FUZK2B, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AHG00520") ){	//[Y] 14.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇQéÌÅj
			m_Util.val_to_bin( Sn_5FUZK2C, pShLstVal->val );
			Sn_5FUZK2sw = pShLstVal->sign1;
		}
		else if( pShLstVal->itmId == _T("AHG00530") ){	//[Y] 14.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊìÒéÌÅj
			m_Util.val_to_bin( Sn_5FUZT2, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AHG00550") ){	//[Y] 15.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇRéÌÅj
			m_Util.val_to_bin( Sn_5FUZK3A, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AHG00560") ){	//[Y] 15.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇRéÌÅj
			m_Util.val_to_bin( Sn_5FUZK3B, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AHG00570") ){	//[Y] 15.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇRéÌÅj
			m_Util.val_to_bin( Sn_5FUZK3C, pShLstVal->val );
			Sn_5FUZK3sw = pShLstVal->sign1;
		}
		else if( pShLstVal->itmId == _T("AHG00580") ){	// 15.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊéOéÌÅj
			m_Util.val_to_bin( Sn_5FUZT3, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AHG00600") ){	//[Y] 16.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇSéÌÅj
			m_Util.val_to_bin( Sn_5FUZK4A, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AHG00610") ){	//[Y] 16.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇSéÌÅj
			m_Util.val_to_bin( Sn_5FUZK4B, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AHG00620") ){	//[Y] 16.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇSéÌÅj
			m_Util.val_to_bin( Sn_5FUZK4C, pShLstVal->val );
			Sn_5FUZK4sw = pShLstVal->sign1;
		}

		else if( pShLstVal->itmId == _T("AHG00630") ){	// 16.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊéléÌÅj
			m_Util.val_to_bin( Sn_5FUZT4, pShLstVal->val );
		}

		else if( pShLstVal->itmId == _T("AHG00650") ){	//[Y] 17.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇTéÌÅj
			m_Util.val_to_bin( Sn_5FUZK5A, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AHG00660") ){	//[Y] 17.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇTéÌÅj
			m_Util.val_to_bin( Sn_5FUZK5B, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AHG00670") ){	//[Y] 17.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇTéÌÅj
			m_Util.val_to_bin( Sn_5FUZK5C, pShLstVal->val );
			Sn_5FUZK5sw = pShLstVal->sign1;
		}

		else if( pShLstVal->itmId == _T("AHG00680") ){	// 17.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊå‹éÌÅj
			m_Util.val_to_bin( Sn_5FUZT5, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AHG00740") ){	// 18.å¥ë•åvéZÇìKópÇ∑ÇÈèÍçáÇÃçTèúëŒè€édì¸ê≈äz
			m_Util.val_to_bin( Sn_5FSZTG, pShLstVal->val );
			Sn_5FGKsw = pShLstVal->sign1;
		}
		else if( pShLstVal->itmId == _T("AHG00710") ){	// 18.
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZKGA, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZKGA, '\0', sizeof(Sn_5FSZKGA) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG00720") ){	// 18.
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZKGB, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZKGB, '\0', sizeof(Sn_5FSZKGB) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG00730") ){	// 18.
			m_Util.val_to_bin( Sn_5FSZKGC, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AHG00800") ){	// 19.ì¡ó·åvéZÇìKópÇ∑ÇÈèÍçáÇÃçTèúëŒè€édì¸ê≈äz
			m_Util.val_to_bin( Sn_5FSZTT, pShLstVal->val );//yoshida140401
//			m_Util.val_to_bin( Sn_5FSZTG, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AHG00770") ){	// 19.
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZKTA, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZKTA, '\0', sizeof(Sn_5FSZKTA) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG00780") ){	// 19.
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZKTB, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZKTB, '\0', sizeof(Sn_5FSZKTB) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG00790") ){	// 19.
			m_Util.val_to_bin( Sn_5FSZKTC, pShLstVal->val );
			Sn_5FSZT19sw = pShLstVal->sign1;
		}

		else if( pShLstVal->itmId == _T("AHG00830") ){		// 20
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZT20A, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZT20A, '\0', sizeof(Sn_5FSZT20A) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG00840") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZT20B, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZT20B, '\0', sizeof(Sn_5FSZT20B) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG00850") ){
			m_Util.val_to_bin( Sn_5FSZT20C, pShLstVal->val );
			Sn_5FSZT20sw = pShLstVal->sign1;
		}
//--> '14.12.19 INS START
		else if( pShLstVal->itmId == _T("AHG00860") ){
			m_Util.val_to_bin( Sn_5FSZT20T, pShLstVal->val );
		}
//--> '14.12.19 INS END

		else if( pShLstVal->itmId == _T("AHG00880") ){		// 21
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZT21A, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZT21A, '\0', sizeof(Sn_5FSZT21A) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG00890") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZT21B, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZT21B, '\0', sizeof(Sn_5FSZT21B) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG00900") ){
			m_Util.val_to_bin( Sn_5FSZT21C, pShLstVal->val );
			Sn_5FSZT21sw = pShLstVal->sign1;
		}
//--> '14.12.19 INS START
		else if( pShLstVal->itmId == _T("AHG00910") ){
			m_Util.val_to_bin( Sn_5FSZT21T, pShLstVal->val );
		}
//--> '14.12.19 INS END

		else if( pShLstVal->itmId == _T("AHG00930") ){		// 22
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZT22A, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZT22A, '\0', sizeof(Sn_5FSZT22A) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG00940") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZT22B, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZT22B, '\0', sizeof(Sn_5FSZT22B) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG00950") ){
			m_Util.val_to_bin( Sn_5FSZT22C, pShLstVal->val );
			Sn_5FSZT22sw = pShLstVal->sign1;
		}
//--> '14.12.19 INS START
		else if( pShLstVal->itmId == _T("AHG00960") ){
			m_Util.val_to_bin( Sn_5FSZT22T, pShLstVal->val );
		}
//--> '14.12.19 INS END

		else if( pShLstVal->itmId == _T("AHG00980") ){		// 23
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZT23A, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZT23A, '\0', sizeof(Sn_5FSZT23A) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG00990") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZT23B, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZT23B, '\0', sizeof(Sn_5FSZT23B) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG01000") ){
			m_Util.val_to_bin( Sn_5FSZT23C, pShLstVal->val );
			Sn_5FSZT23sw = pShLstVal->sign1;
		}
//--> '14.12.19 INS START
		else if( pShLstVal->itmId == _T("AHG01010") ){
			m_Util.val_to_bin( Sn_5FSZT23T, pShLstVal->val );
		}
//--> '14.12.19 INS END

		else if( pShLstVal->itmId == _T("AHG01030") ){		// 24
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZT24A, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZT24A, '\0', sizeof(Sn_5FSZT24A) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG01040") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZT24B, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZT24B, '\0', sizeof(Sn_5FSZT24B) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG01050") ){
			m_Util.val_to_bin( Sn_5FSZT24C, pShLstVal->val );
			Sn_5FSZT24sw = pShLstVal->sign1;
		}
//--> '14.12.19 INS START
		else if( pShLstVal->itmId == _T("AHG01060") ){
			m_Util.val_to_bin( Sn_5FSZT24T, pShLstVal->val );
		}
//--> '14.12.19 INS END

		else if( pShLstVal->itmId == _T("AHG01080") ){		// 25
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZT25A, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZT25A, '\0', sizeof(Sn_5FSZT25A) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG01090") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZT25B, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZT25B, '\0', sizeof(Sn_5FSZT25B) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG01100") ){
			m_Util.val_to_bin( Sn_5FSZT25C, pShLstVal->val );
			Sn_5FSZT25sw = pShLstVal->sign1;
		}
//--> '14.12.19 INS START
		else if( pShLstVal->itmId == _T("AHG01110") ){
			m_Util.val_to_bin( Sn_5FSZT25T, pShLstVal->val );
		}
//--> '14.12.19 INS END

		else if( pShLstVal->itmId == _T("AHG01130") ){		// 26
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZT26A, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZT26A, '\0', sizeof(Sn_5FSZT26A) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG01140") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZT26B, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZT26B, '\0', sizeof(Sn_5FSZT26B) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG01150") ){
			m_Util.val_to_bin( Sn_5FSZT26C, pShLstVal->val );
			Sn_5FSZT26sw = pShLstVal->sign1;
		}
//--> '14.12.19 INS START
		else if( pShLstVal->itmId == _T("AHG01160") ){
			m_Util.val_to_bin( Sn_5FSZT26T, pShLstVal->val );
		}
//--> '14.12.19 INS END

		else if( pShLstVal->itmId == _T("AHG01180") ){		// 27
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZT27A, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZT27A, '\0', sizeof(Sn_5FSZT27A) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG01190") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZT27B, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZT27B, '\0', sizeof(Sn_5FSZT27B) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG01200") ){
			m_Util.val_to_bin( Sn_5FSZT27C, pShLstVal->val );
			Sn_5FSZT27sw = pShLstVal->sign1;
		}
//--> '14.12.19 INS START
		else if( pShLstVal->itmId == _T("AHG01210") ){
			m_Util.val_to_bin( Sn_5FSZT27T, pShLstVal->val );
		}
//--> '14.12.19 INS END

		else if( pShLstVal->itmId == _T("AHG01230") ){		// 28
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZT28A, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZT28A, '\0', sizeof(Sn_5FSZT28A) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG01240") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZT28B, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZT28B, '\0', sizeof(Sn_5FSZT28B) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG01250") ){
			m_Util.val_to_bin( Sn_5FSZT28C, pShLstVal->val );
			Sn_5FSZT28sw = pShLstVal->sign1;
		}
//--> '14.12.19 INS START
		else if( pShLstVal->itmId == _T("AHG01260") ){
			m_Util.val_to_bin( Sn_5FSZT28T, pShLstVal->val );
		}
//--> '14.12.19 INS END

		else if( pShLstVal->itmId == _T("AHG01280") ){		// 29
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZT29A, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZT29A, '\0', sizeof(Sn_5FSZT29A) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG01290") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_5FSZT29B, pShLstVal->val );
			}
			else{
				memset( Sn_5FSZT29B, '\0', sizeof(Sn_5FSZT29B) );
			}
		}
		else if( pShLstVal->itmId == _T("AHG01300") ){
			m_Util.val_to_bin( Sn_5FSZT29C, pShLstVal->val );
			Sn_5FSZT29sw = pShLstVal->sign1;
		}
//--> '14.12.19 INS START
		else if( pShLstVal->itmId == _T("AHG01310") ){
			m_Util.val_to_bin( Sn_5FSZT29T, pShLstVal->val );
		}
//--> '14.12.19 INS END
	}

	// ê\çêèëÉfÅ[É^
	filter.Format( _T("Sn_SEQ = %d"), sn_seq );
	BOOL openFlg = FALSE;
	if( pDBNpSub->szsnk == NULL ){
		if( pDBNpSub->SzsnkOpen(filter) ){
			return -1;
		}
		openFlg = TRUE;
	}
	else{
		pDBNpSub->szsnk->Requery( filter );
	}

	if( (pDBNpSub->szsnk->MoveFirst()!=-1) && (pDBNpSub->szsnk->st!=-1) ){
		Sn_5FSZsw = pDBNpSub->szsnk->Sn_5FSZsw;
		Sn_5FSBsw = pDBNpSub->szsnk->Sn_5FSBsw;
		Sn_5FCQsw = pDBNpSub->szsnk->Sn_5FCQsw;
//		Sn_5FGKsw = pDBNpSub->szsnk->Sn_5FGKsw;
	}

	if( openFlg ){
		pDBNpSub->SzsnkClose();
	}

	return 0;
}
#endif

#ifdef _20140219_EXTEND_
//-----------------------------------------------------------------------------
// DBÇ÷ÉfÅ[É^èëçûÇ›
//-----------------------------------------------------------------------------
// à¯êî	pDBNpSub	ÅF	ç‡ñ±ÉNÉâÉXÇÃÉ|ÉCÉìÉ^
//		sn_seq		ÅF	ëŒâûÇÃóöóÉVÅ[ÉPÉìÉX
//-----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	ê≥èÌèIóπ
//			-1		ÅF	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CH26SnFhyo50Data::SetData( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT( pDBNpSub );

	CString	filter;
	char	*pHyoID = NULL;
	if( m_isTransitionalMeasure ){
		filter.Format( _T("sn_seq = %d and hyoId = \'%s\'"), sn_seq, ICS_SN_FHYO_5_2_HYOID );
		pHyoID = ICS_SN_FHYO_5_2_HYOID;
	}
	else{
		filter.Format( _T("sn_seq = %d and hyoId = \'%s\'"), sn_seq, ICS_SN_FHYO_5_HYOID );
		pHyoID = ICS_SN_FHYO_5_HYOID;
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

		if( m_isTransitionalMeasure ){
			// 6.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ä˙ä‘åv)
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00070");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FURIK );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00040");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FURITA );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00050");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FURITB );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00060");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FURITC );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 7.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväzÅiëÊÇPéÌÅj
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00090");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI1A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00100");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI1B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00110");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI1C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 8.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväzÅiëÊÇQéÌÅj
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00150");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI2A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00160");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI2B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00170");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI2C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 9.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväzÅiëÊÇRéÌÅj
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00210");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI3A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00220");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI3B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00230");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI3C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 10.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväzÅiëÊÇSéÌÅj
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00270");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI4A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00280");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI4B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00290");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI4C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 11.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväzÅiëÊÇTéÌÅj
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00330");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI5A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00340");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI5B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00350");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI5C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();


			// 12.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväz(ä˙ä‘åv)
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00430");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZTT );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 13.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊàÍéÌÅj
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00480");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZT1 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();
		
			// 14.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊìÒéÌÅj
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00530");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZT2 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();
		
			// 15.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊéOéÌÅj
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00580");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZT3 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 16.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊéléÌÅj
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00630");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZT4 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 17.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊå‹éÌÅj
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00680");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZT5 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 18.å¥ë•åvéZÇìKópÇ∑ÇÈèÍçáÇÃçTèúëŒè€édì¸ê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00740");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZTG );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();
		
			// 19.ì¡ó·åvéZÇìKópÇ∑ÇÈèÍçáÇÃçTèúëŒè€édì¸ê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00800");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZTG );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();
		}
		else{
			// 6.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ä˙ä‘åv)
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AGG00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FURIK );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 12.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväz(ä˙ä‘åv)
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AGG00200");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZTT );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 13.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊàÍéÌÅj
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AGG00210");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZT1 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();
		
			// 14.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊìÒéÌÅj
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AGG00220");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZT2 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();
		
			// 15.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊéOéÌÅj
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AGG00230");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZT3 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 16.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊéléÌÅj
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AGG00240");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZT4 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 17.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊå‹éÌÅj
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AGG00250");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZT5 );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 18.å¥ë•åvéZÇìKópÇ∑ÇÈèÍçáÇÃçTèúëŒè€édì¸ê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AGG00270");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZTG );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();
		
			// 19.ì¡ó·åvéZÇìKópÇ∑ÇÈèÍçáÇÃçTèúëŒè€édì¸ê≈äz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AGG00310");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZTG );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();
		}
	}
	else{														// ÉfÅ[É^óLÇË
		if( m_isTransitionalMeasure ){
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

				if( pShLstVal->itmId == _T("AGG00030") ){		// 6.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ä˙ä‘åv)
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FURIK );
					pShLstVal->Update();
				}

				if( pShLstVal->itmId == _T("AHG00070") ){			// 6.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FURIK );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AHG00040") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FURITA );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AHG00050") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FURITB );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AHG00060") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FURITC );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AHG00090") ){		// 7.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ëÊÇPã∆éÌ)
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI1A );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AHG00100") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI1B );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AHG00110") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI1C );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AHG00150") ){		// 8.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ëÊÇQã∆éÌ)
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI2A );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AHG00160") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI2B );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AHG00170") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI2C );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AHG00210") ){		// 9.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ëÊÇRã∆éÌ)
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI3A );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AHG00220") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI3B );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AHG00230") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI3C );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AHG00270") ){		// 10.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ëÊÇSã∆éÌ)
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI4A );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AHG00280") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI4B );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AHG00290") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI4C );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AHG00330") ){		// 11.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ëÊÇTã∆éÌ)
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI5A );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AHG00340") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI5B );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AHG00350") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI5C );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AHG00430") ){	// 12.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväz(ä˙ä‘åv)
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZTT );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AHG00480") ){	// 13.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊàÍéÌÅj
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZT1 );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AHG00530") ){	// 14.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊìÒéÌÅj
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZT2 );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AHG00580") ){	// 15.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊéOéÌÅj
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZT3 );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AHG00630") ){	// 16.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊéléÌÅj
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZT4 );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AHG00680") ){	// 17.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊå‹éÌÅj
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZT5 );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AHG00740") ){	// 18.å¥ë•åvéZÇìKópÇ∑ÇÈèÍçáÇÃçTèúëŒè€édì¸ê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZTG );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AHG00800") ){	// 19.ì¡ó·åvéZÇìKópÇ∑ÇÈèÍçáÇÃçTèúëŒè€édì¸ê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZTG );
					pShLstVal->Update();
				}
			}
		}
		else{
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

				if( pShLstVal->itmId == _T("AGG00030") ){		// 6.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ä˙ä‘åv)
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FURIK );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AGG00200") ){	// 12.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväz(ä˙ä‘åv)
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZTT );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AGG00210") ){	// 13.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊàÍéÌÅj
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZT1 );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AGG00220") ){	// 14.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊìÒéÌÅj
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZT2 );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AGG00230") ){	// 15.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊéOéÌÅj
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZT3 );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AGG00240") ){	// 16.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊéléÌÅj
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZT4 );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AGG00250") ){	// 17.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊå‹éÌÅj
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZT5 );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AGG00270") ){	// 18.å¥ë•åvéZÇìKópÇ∑ÇÈèÍçáÇÃçTèúëŒè€édì¸ê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZTG );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AGG00310") ){	// 19.ì¡ó·åvéZÇìKópÇ∑ÇÈèÍçáÇÃçTèúëŒè€édì¸ê≈äz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZTG );
					pShLstVal->Update();
				}
			}
		}
	}

	return 0;
}
#else
//-----------------------------------------------------------------------------
// DBÇ÷ÉfÅ[É^èëçûÇ›
//-----------------------------------------------------------------------------
// à¯êî	pDBNpSub	ÅF	ç‡ñ±ÉNÉâÉXÇÃÉ|ÉCÉìÉ^
//		sn_seq		ÅF	ëŒâûÇÃóöóÉVÅ[ÉPÉìÉX
//-----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	ê≥èÌèIóπ
//			-1		ÅF	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CH26SnFhyo50Data::SetData( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT( pDBNpSub );

	CString	filter;
	int		nowVersion = 1;
	char	*pHyoID = NULL;
	filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_5_2_HYOID, nowVersion );
	pHyoID = ICS_SN_FHYO_5_2_HYOID;

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

//		if( m_isTransitionalMeasure ){
			// 6.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ä˙ä‘åv)
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00040");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FURITA );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00050");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FURITB );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 7.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväzÅiëÊÇPéÌÅj
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00090");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI1A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00100");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI1B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 8.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväzÅiëÊÇQéÌÅj
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00150");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI2A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00160");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI2B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 9.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväzÅiëÊÇRéÌÅj
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00210");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI3A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00220");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI3B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 10.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväzÅiëÊÇSéÌÅj
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00270");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI4A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00280");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI4B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 11.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväzÅiëÊÇTéÌÅj
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00330");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI5A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00340");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI5B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 13.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇPéÌÅj
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00450");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK1A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00460");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK1B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 14.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇQéÌÅj
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00500");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK2A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00510");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK2B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 15.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇRéÌÅj
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00550");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK3A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00560");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK3B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 16.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇSéÌÅj
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00600");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK4A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00610");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK4B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 17.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇTéÌÅj
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00650");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK5A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00660");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK5B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 18.å¥ë•åvéZÇìKópÇ∑ÇÈèÍçá
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00710");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZKGA );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00720");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZKGB );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 19.ì¡ó·åvéZÇìKópÇ∑ÇÈèÍçá
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00770");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZKTA );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00780");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZKTB );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 20
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00830");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT20A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00840");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT20B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 21
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00880");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT21A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00890");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT21B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 22
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00930");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT22A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00940");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT22B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 23
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00980");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT23A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG00990");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT23B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 24
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG01030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT24A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG01040");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT24B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 25
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG01080");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT25A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG01090");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT25B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 26
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG01130");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT26A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG01140");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT26B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 27
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG01180");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT27A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG01190");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT27B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 28
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG01230");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT28A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG01240");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT28B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// 29
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG01280");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT29A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AHG01290");
			m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT29B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();
//		}

		// 6.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ä˙ä‘åv)
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG00070");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FURIK );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG00060");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FURITC );
		pShLstVal->sign1 = Sn_5FURITsw;//[Y]
		pShLstVal->Update();

		// 7.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväzÅiëÊÇPéÌÅj
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG00110");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI1C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 8.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväzÅiëÊÇQéÌÅj
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG00170");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI2C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 9.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväzÅiëÊÇRéÌÅj
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG00230");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI3C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 10.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväzÅiëÊÇSéÌÅj
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG00290");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI4C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 11.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväzÅiëÊÇTéÌÅj
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG00350");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI5C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 12.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväz(ä˙ä‘åv)
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG00430");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZTT );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 13.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊàÍéÌÅj
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG00480");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZT1 );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();
	
		// 14.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊìÒéÌÅj
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG00530");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZT2 );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();
	
		// 15.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊéOéÌÅj
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG00580");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZT3 );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 16.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊéléÌÅj
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG00630");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZT4 );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 17.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊå‹éÌÅj
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG00680");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZT5 );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 13.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇPéÌÅj
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG00470");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK1C );
/*- '14.05.17 -*/
		pShLstVal->sign1 = Sn_5FUZK1sw;//[Y]
/*-------------*/
//		pShLstVal->sign1 = Sn_5FUZK1sw&(~0x111);//[Y]
/*-------------*/
		pShLstVal->Update();

		// 14.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇQéÌÅj
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG00520");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK2C );
/*- '14.05.17 -*/
		pShLstVal->sign1 = Sn_5FUZK2sw;//[Y]
/*-------------*/
//		pShLstVal->sign1 = Sn_5FUZK2sw&(~0x111);//[Y]
/*-------------*/
		pShLstVal->Update();

		// 15.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇRéÌÅj
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG00570");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK3C );
/*- '14.05.17 -*/
		pShLstVal->sign1 = Sn_5FUZK3sw;//[Y]
/*-------------*/
//		pShLstVal->sign1 = Sn_5FUZK3sw&(~0x111);//[Y]
/*-------------*/
		pShLstVal->Update();

		// 16.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇSéÌÅj
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG00620");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK4C );
/*- '14.05.17 -*/
		pShLstVal->sign1 = Sn_5FUZK4sw;//[Y]
/*-------------*/
//		pShLstVal->sign1 = Sn_5FUZK4sw&(~0x111);//[Y]
/*-------------*/
		pShLstVal->Update();

		// 17.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇTéÌÅj
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG00670");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK5C );
/*- '14.05.17 -*/
		pShLstVal->sign1 = Sn_5FUZK5sw;//[Y]
/*-------------*/
//		pShLstVal->sign1 = Sn_5FUZK5sw&(~0x111);//[Y]
/*-------------*/
		pShLstVal->Update();

		// 18.å¥ë•åvéZÇìKópÇ∑ÇÈèÍçáÇÃçTèúëŒè€édì¸ê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG00730");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZKGC );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 18.å¥ë•åvéZÇìKópÇ∑ÇÈèÍçáÇÃçTèúëŒè€édì¸ê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG00740");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZTG );
		pShLstVal->sign1 = Sn_5FGKsw;
		pShLstVal->Update();
	
		// 19.ì¡ó·åvéZÇìKópÇ∑ÇÈèÍçáÇÃçTèúëŒè€édì¸ê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG00790");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZKTC );
		pShLstVal->sign1 = Sn_5FSZT19sw;
		pShLstVal->Update();

		// 19.ì¡ó·åvéZÇìKópÇ∑ÇÈèÍçáÇÃçTèúëŒè€édì¸ê≈äz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG00800");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZTT );//yoshida140401
//		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZTG );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// 20
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG00850");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT20C );
		pShLstVal->sign1 = Sn_5FSZT20sw;
		pShLstVal->Update();

//-->'14.12.19 INS START
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG00860");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT20T );
		pShLstVal->Update();
//-->'14.12.19 INS END

		// 21
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG00900");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT21C );
		pShLstVal->sign1 = Sn_5FSZT21sw;
		pShLstVal->Update();

//-->'14.12.19 INS START
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG00910");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT21T );
		pShLstVal->Update();
//-->'14.12.19 INS END

		// 22
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG00950");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT22C );
		pShLstVal->sign1 = Sn_5FSZT22sw;
		pShLstVal->Update();

//-->'14.12.19 INS START
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG00960");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT22T );
		pShLstVal->Update();
//-->'14.12.19 INS END

		// 23
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01000");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT23C );
		pShLstVal->sign1 = Sn_5FSZT23sw;
		pShLstVal->Update();

//-->'14.12.19 INS START
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01010");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT23T );
		pShLstVal->Update();
//-->'14.12.19 INS END

		// 24
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01050");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT24C );
		pShLstVal->sign1 = Sn_5FSZT24sw;
		pShLstVal->Update();

//-->'14.12.19 INS START
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01060");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT24T );
		pShLstVal->Update();
//-->'14.12.19 INS END

		// 25
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01100");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT25C );
		pShLstVal->sign1 = Sn_5FSZT25sw;
		pShLstVal->Update();

//-->'14.12.19 INS START
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01110");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT25T );
		pShLstVal->Update();
//-->'14.12.19 INS END

		// 26
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01150");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT26C );
		pShLstVal->sign1 = Sn_5FSZT26sw;
		pShLstVal->Update();

//-->'14.12.19 INS START
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01160");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT26T );
		pShLstVal->Update();
//-->'14.12.19 INS END

		// 27
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01200");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT27C );
		pShLstVal->sign1 = Sn_5FSZT27sw;
		pShLstVal->Update();

//-->'14.12.19 INS START
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01210");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT27T );
		pShLstVal->Update();
//-->'14.12.19 INS END

		// 28
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01250");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT28C );
		pShLstVal->sign1 = Sn_5FSZT28sw;
		pShLstVal->Update();

//-->'14.12.19 INS START
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01260");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT28T );
		pShLstVal->Update();
//-->'14.12.19 INS END

		// 29
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01300");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT29C );
		pShLstVal->sign1 = Sn_5FSZT29sw;
		pShLstVal->Update();

//-->'14.12.19 INS START
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AHG01310");
		m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT29T );
		pShLstVal->Update();
//-->'14.12.19 INS END
	}
	else{														// ÉfÅ[É^óLÇË

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

//			if( m_isTransitionalMeasure ){
				// 6.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz
				if( pShLstVal->itmId == _T("AHG00040") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FURITA );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AHG00050") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FURITB );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// 7.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ëÊÇPã∆éÌ)
				else if( pShLstVal->itmId == _T("AHG00090") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI1A );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AHG00100") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI1B );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// 8.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ëÊÇQã∆éÌ)
				else if( pShLstVal->itmId == _T("AHG00150") ){		
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI2A );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AHG00160") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI2B );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// 9.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ëÊÇRã∆éÌ)
				else if( pShLstVal->itmId == _T("AHG00210") ){		
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI3A );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AHG00220") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI3B );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// 10.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ëÊÇSã∆éÌ)
				else if( pShLstVal->itmId == _T("AHG00270") ){		
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI4A );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AHG00280") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI4B );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// 11.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ëÊÇTã∆éÌ)
				else if( pShLstVal->itmId == _T("AHG00330") ){		
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI5A );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AHG00340") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI5B );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// 13.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇPéÌÅj
				else if( pShLstVal->itmId == _T("AHG00450") ){		
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK1A );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AHG00460") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK1B );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// 14.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇQéÌÅj
				else if( pShLstVal->itmId == _T("AHG00500") ){		
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK2A );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AHG00510") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK2B );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// 15.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇRéÌÅj
				else if( pShLstVal->itmId == _T("AHG00550") ){		
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK3A );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AHG00560") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK3B );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// 16.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇSéÌÅj
				else if( pShLstVal->itmId == _T("AHG00600") ){		
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK4A );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AHG00610") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK4B );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// 17.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇTéÌÅj
				else if( pShLstVal->itmId == _T("AHG00650") ){		
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK5A );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AHG00660") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK5B );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// 18.å¥ë•åvéZÇìKópÇ∑ÇÈèÍçá
				else if( pShLstVal->itmId == _T("AHG00710") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZKGA );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AHG00720") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZKGB );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// 19.ì¡ó·åvéZÇìKópÇ∑ÇÈèÍçá
				else if( pShLstVal->itmId == _T("AHG00770") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZKTA );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AHG00780") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZKTB );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// 20
				else if( pShLstVal->itmId == _T("AHG00830") ){		
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT20A );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AHG00840") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT20B );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}

				// 21
				else if( pShLstVal->itmId == _T("AHG00880") ){		
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT21A );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AHG00890") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT21B );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}

				// 22
				else if( pShLstVal->itmId == _T("AHG00930") ){		
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT22A );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AHG00940") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT22B );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}

				// 23
				else if( pShLstVal->itmId == _T("AHG00980") ){		
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT23A );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AHG00990") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT23B );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}

				// 24
				else if( pShLstVal->itmId == _T("AHG01030") ){		
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT24A );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AHG01040") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT24B );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}

				// 25
				else if( pShLstVal->itmId == _T("AHG01080") ){		
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT25A );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AHG01090") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT25B );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}

				// 26
				else if( pShLstVal->itmId == _T("AHG01130") ){		
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT26A );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AHG01140") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT26B );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}

				// 27
				else if( pShLstVal->itmId == _T("AHG01180") ){		
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT27A );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AHG01190") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT27B );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}

				// 28
				else if( pShLstVal->itmId == _T("AHG01230") ){		
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT28A );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AHG01240") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT28B );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}

				// 29
				else if( pShLstVal->itmId == _T("AHG01280") ){		
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT29A );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AHG01290") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT29B );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
//			}

			// 6.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ä˙ä‘åv)
//-- '15.02.20 --
//			if( pShLstVal->itmId == _T("AGG00030") ){
//				pShLstVal->Edit();
//				m_Util.val_to_asci( &pShLstVal->val, Sn_5FURIK );
//				pShLstVal->Update();
//				updTagAry.Add( pShLstVal->itmId );
//			}
//			else if( pShLstVal->itmId == _T("AHG00070") ){	
//				pShLstVal->Edit();
//				m_Util.val_to_asci( &pShLstVal->val, Sn_5FURIK );
//				pShLstVal->Update();
//				updTagAry.Add( pShLstVal->itmId );
//			}
//---------------
			if( pShLstVal->itmId == _T("AHG00070") ){	
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FURIK );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
//---------------
			// 6.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz
			else if( pShLstVal->itmId == _T("AHG00060") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FURITC );
				pShLstVal->sign1 = Sn_5FURITsw;//[Y]
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			// 7.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ëÊÇPã∆éÌ)
			else if( pShLstVal->itmId == _T("AHG00110") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI1C );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			// 8.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ëÊÇQã∆éÌ)
			else if( pShLstVal->itmId == _T("AHG00170") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI2C );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			// 9.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ëÊÇRã∆éÌ)
			else if( pShLstVal->itmId == _T("AHG00230") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI3C );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			// 10.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ëÊÇSã∆éÌ)
			else if( pShLstVal->itmId == _T("AHG00290") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI4C );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			// 11.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ëÊÇTã∆éÌ)
			else if( pShLstVal->itmId == _T("AHG00350") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FURI5C );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			else if( pShLstVal->itmId == _T("AHG00430") ){	// 12.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväz(ä˙ä‘åv)
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZTT );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			else if( pShLstVal->itmId == _T("AHG00480") ){	// 13.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊàÍéÌÅj
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZT1 );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			else if( pShLstVal->itmId == _T("AHG00530") ){	// 14.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊìÒéÌÅj
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZT2 );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			else if( pShLstVal->itmId == _T("AHG00580") ){	// 15.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊéOéÌÅj
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZT3 );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			else if( pShLstVal->itmId == _T("AHG00630") ){	// 16.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊéléÌÅj
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZT4 );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			else if( pShLstVal->itmId == _T("AHG00680") ){	// 17.éñã∆ãÊï™ï  è¡îÔê≈äzçáåväzÅiëÊå‹éÌÅj
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZT5 );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			else if( pShLstVal->itmId == _T("AHG00730") ){	// 18.å¥ë•åvéZÇìKópÇ∑ÇÈèÍçáÇÃçTèúëŒè€édì¸ê≈äz
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZKGC );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			else if( pShLstVal->itmId == _T("AHG00740") ){	// 18.å¥ë•åvéZÇìKópÇ∑ÇÈèÍçáÇÃçTèúëŒè€édì¸ê≈äz
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZTG );
				pShLstVal->sign1 = Sn_5FGKsw;
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			else if( pShLstVal->itmId == _T("AHG00790") ){	// 19.ì¡ó·åvéZÇìKópÇ∑ÇÈèÍçáÇÃçTèúëŒè€édì¸ê≈äz
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZKTC );
				pShLstVal->sign1 = Sn_5FSZT19sw;
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			else if( pShLstVal->itmId == _T("AHG00800") ){	// 19.ì¡ó·åvéZÇìKópÇ∑ÇÈèÍçáÇÃçTèúëŒè€édì¸ê≈äz
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZTT );//yoshida140401
//				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZTG );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			// 13.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇPéÌÅj
			else if( pShLstVal->itmId == _T("AHG00470") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK1C );
/*- '14.05.17 -*/
				pShLstVal->sign1 = Sn_5FUZK1sw;//[Y]
/*-------------*/
//				pShLstVal->sign1 = Sn_5FUZK1sw&(~0x111);//[Y]
/*-------------*/
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			// 14.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇQéÌÅj
			else if( pShLstVal->itmId == _T("AHG00520") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK2C );
/*- '14.05.17 -*/
				pShLstVal->sign1 = Sn_5FUZK2sw;//[Y]
/*-------------*/
//				pShLstVal->sign1 = Sn_5FUZK2sw&(~0x111);//[Y]
/*-------------*/
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			// 15.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇRéÌÅj
			else if( pShLstVal->itmId == _T("AHG00570") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK3C );
/*- '14.05.17 -*/
				pShLstVal->sign1 = Sn_5FUZK3sw;//[Y]
/*-------------*/
//				pShLstVal->sign1 = Sn_5FUZK3sw&(~0x111);//[Y]
/*-------------*/
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			// 16.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇSéÌÅj
			else if( pShLstVal->itmId == _T("AHG00620") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK4C );
/*- '14.05.17 -*/
				pShLstVal->sign1 = Sn_5FUZK4sw;//[Y]
/*-------------*/
//				pShLstVal->sign1 = Sn_5FUZK4sw&(~0x111);//[Y]
/*-------------*/
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			// 17.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇTéÌÅj
			else if( pShLstVal->itmId == _T("AHG00670") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FUZK5C );
/*- '14.05.17 -*/
				pShLstVal->sign1 = Sn_5FUZK5sw;//[Y]
/*-------------*/
//				pShLstVal->sign1 = Sn_5FUZK5sw&(~0x111);//[Y]
/*-------------*/
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			// 20
			else if( pShLstVal->itmId == _T("AHG00850") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT20C );
				pShLstVal->sign1 = Sn_5FSZT20sw;
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
//--> '14.12.19 INS START
			else if( pShLstVal->itmId == _T("AHG00860") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT20T );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
//--> '14.12.19 INS END
			// 21
			else if( pShLstVal->itmId == _T("AHG00900") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT21C );
				pShLstVal->sign1 = Sn_5FSZT21sw;
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
//--> '14.12.19 INS START
			else if( pShLstVal->itmId == _T("AHG00910") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT21T );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
//--> '14.12.19 INS END
			// 22
			else if( pShLstVal->itmId == _T("AHG00950") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT22C );
				pShLstVal->sign1 = Sn_5FSZT22sw;
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
//--> '14.12.19 INS START
			else if( pShLstVal->itmId == _T("AHG00960") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT22T );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
//--> '14.12.19 INS END
			// 23
			else if( pShLstVal->itmId == _T("AHG01000") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT23C );
				pShLstVal->sign1 = Sn_5FSZT23sw;
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
//--> '14.12.19 INS START
			else if( pShLstVal->itmId == _T("AHG01010") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT23T );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
//--> '14.12.19 INS END
			// 24
			else if( pShLstVal->itmId == _T("AHG01050") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT24C );
				pShLstVal->sign1 = Sn_5FSZT24sw;
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
//--> '14.12.19 INS START
			else if( pShLstVal->itmId == _T("AHG01060") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT24T );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
//--> '14.12.19 INS END
			// 25
			else if( pShLstVal->itmId == _T("AHG01100") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT25C );
				pShLstVal->sign1 = Sn_5FSZT25sw;
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
//--> '14.12.19 INS START
			else if( pShLstVal->itmId == _T("AHG01110") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT25T );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
//--> '14.12.19 INS END
			// 26
			else if( pShLstVal->itmId == _T("AHG01150") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT26C );
				pShLstVal->sign1 = Sn_5FSZT26sw;
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
//--> '14.12.19 INS START
			else if( pShLstVal->itmId == _T("AHG01160") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT26T );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
//--> '14.12.19 INS END
			// 27
			else if( pShLstVal->itmId == _T("AHG01200") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT27C );
				pShLstVal->sign1 = Sn_5FSZT27sw;
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
//--> '14.12.19 INS START
			else if( pShLstVal->itmId == _T("AHG01210") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT27T );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
//--> '14.12.19 INS END
			// 28
			else if( pShLstVal->itmId == _T("AHG01250") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT28C );
				pShLstVal->sign1 = Sn_5FSZT28sw;
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
//--> '14.12.19 INS START
			else if( pShLstVal->itmId == _T("AHG01260") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT28T );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
//--> '14.12.19 INS END
			// 29
			else if( pShLstVal->itmId == _T("AHG01300") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT29C );
				pShLstVal->sign1 = Sn_5FSZT29sw;
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
//--> '14.12.19 INS START
			else if( pShLstVal->itmId == _T("AHG01310") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_5FSZT29T );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
//--> '14.12.19 INS END
		}

		// çXêVÉfÅ[É^ämîF
		int	adCnt = 0;
		for( int i=0;; i++ ){
			if( strlen(tagData50[i].tagName) == 0 ){
				break;
			}
			if( m_isTransitionalMeasure== FALSE ){
				if( (tagData50[i].sign1!=0) && (tagData50[i].sign1!=1) ){
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
			MonyPtrAry.Add( Sn_5FURIK );
			MonyPtrAry.Add( Sn_5FURITA );
			MonyPtrAry.Add( Sn_5FURITB );
			MonyPtrAry.Add( Sn_5FURITC );
			MonyPtrAry.Add( Sn_5FURI1A );
			MonyPtrAry.Add( Sn_5FURI1B );
			MonyPtrAry.Add( Sn_5FURI1C );

			MonyPtrAry.Add( Sn_5FURI2A );
			MonyPtrAry.Add( Sn_5FURI2B );
			MonyPtrAry.Add( Sn_5FURI2C );

			MonyPtrAry.Add( Sn_5FURI3A );
			MonyPtrAry.Add( Sn_5FURI3B );
			MonyPtrAry.Add( Sn_5FURI3C );

			MonyPtrAry.Add( Sn_5FURI4A );
			MonyPtrAry.Add( Sn_5FURI4B );
			MonyPtrAry.Add( Sn_5FURI4C );

			MonyPtrAry.Add( Sn_5FURI5A );
			MonyPtrAry.Add( Sn_5FURI5B );
			MonyPtrAry.Add( Sn_5FURI5C );

			MonyPtrAry.Add( Sn_5FUZKTA );
			MonyPtrAry.Add( Sn_5FUZKTB );
			MonyPtrAry.Add( Sn_5FUZKTC );
			MonyPtrAry.Add( Sn_5FUZTT );

			MonyPtrAry.Add( Sn_5FUZK1A );
			MonyPtrAry.Add( Sn_5FUZK1B );
			MonyPtrAry.Add( Sn_5FUZK1C );
			MonyPtrAry.Add( Sn_5FUZT1 );

			MonyPtrAry.Add( Sn_5FUZK2A );
			MonyPtrAry.Add( Sn_5FUZK2B );
			MonyPtrAry.Add( Sn_5FUZK2C );
			MonyPtrAry.Add( Sn_5FUZT2 );

			MonyPtrAry.Add( Sn_5FUZK3A );
			MonyPtrAry.Add( Sn_5FUZK3B );
			MonyPtrAry.Add( Sn_5FUZK3C );
			MonyPtrAry.Add( Sn_5FUZT3 );

			MonyPtrAry.Add( Sn_5FUZK4A );
			MonyPtrAry.Add( Sn_5FUZK4B );
			MonyPtrAry.Add( Sn_5FUZK4C );
			MonyPtrAry.Add( Sn_5FUZT4 );

			MonyPtrAry.Add( Sn_5FUZK5A );
			MonyPtrAry.Add( Sn_5FUZK5B );
			MonyPtrAry.Add( Sn_5FUZK5C );
			MonyPtrAry.Add( Sn_5FUZT5 );

			MonyPtrAry.Add( Sn_5FSZT20A );
			MonyPtrAry.Add( Sn_5FSZT20B );
			MonyPtrAry.Add( Sn_5FSZT20C );
//--> '14.12.19 INS START
			MonyPtrAry.Add( Sn_5FSZT20T );
//--> '14.12.19 INS END

			MonyPtrAry.Add( Sn_5FSZT21A );
			MonyPtrAry.Add( Sn_5FSZT21B );
			MonyPtrAry.Add( Sn_5FSZT21C );
//--> '14.12.19 INS START
			MonyPtrAry.Add( Sn_5FSZT21T );
//--> '14.12.19 INS END

			MonyPtrAry.Add( Sn_5FSZT22A );
			MonyPtrAry.Add( Sn_5FSZT22B );
			MonyPtrAry.Add( Sn_5FSZT22C );
//--> '14.12.19 INS START
			MonyPtrAry.Add( Sn_5FSZT22T );
//--> '14.12.19 INS END

			MonyPtrAry.Add( Sn_5FSZT23A );
			MonyPtrAry.Add( Sn_5FSZT23B );
			MonyPtrAry.Add( Sn_5FSZT23C );
//--> '14.12.19 INS START
			MonyPtrAry.Add( Sn_5FSZT23T );
//--> '14.12.19 INS END

			MonyPtrAry.Add( Sn_5FSZT24A );
			MonyPtrAry.Add( Sn_5FSZT24B );
			MonyPtrAry.Add( Sn_5FSZT24C );
//--> '14.12.19 INS START
			MonyPtrAry.Add( Sn_5FSZT24T );
//--> '14.12.19 INS END

			MonyPtrAry.Add( Sn_5FSZT25A );
			MonyPtrAry.Add( Sn_5FSZT25B );
			MonyPtrAry.Add( Sn_5FSZT25C );
//--> '14.12.19 INS START
			MonyPtrAry.Add( Sn_5FSZT25T );
//--> '14.12.19 INS END

			MonyPtrAry.Add( Sn_5FSZT26A );
			MonyPtrAry.Add( Sn_5FSZT26B );
			MonyPtrAry.Add( Sn_5FSZT26C );
//--> '14.12.19 INS START
			MonyPtrAry.Add( Sn_5FSZT26T );
//--> '14.12.19 INS END

			MonyPtrAry.Add( Sn_5FSZT27A );
			MonyPtrAry.Add( Sn_5FSZT27B );
			MonyPtrAry.Add( Sn_5FSZT27C );
//--> '14.12.19 INS START
			MonyPtrAry.Add( Sn_5FSZT27T );
//--> '14.12.19 INS END

			MonyPtrAry.Add( Sn_5FSZT28A );
			MonyPtrAry.Add( Sn_5FSZT28B );
			MonyPtrAry.Add( Sn_5FSZT28C );
//--> '14.12.19 INS START
			MonyPtrAry.Add( Sn_5FSZT28T );
//--> '14.12.19 INS END

			MonyPtrAry.Add( Sn_5FSZT29A );
			MonyPtrAry.Add( Sn_5FSZT29B );
			MonyPtrAry.Add( Sn_5FSZT29C );
//--> '14.12.19 INS START
			MonyPtrAry.Add( Sn_5FSZT29T );
//--> '14.12.19 INS END

			MonyPtrAry.Add( Sn_5FSZKGA );
			MonyPtrAry.Add( Sn_5FSZKGB );
			MonyPtrAry.Add( Sn_5FSZKGC );
			MonyPtrAry.Add( Sn_5FSZTG );

			MonyPtrAry.Add( Sn_5FSZKTA );
			MonyPtrAry.Add( Sn_5FSZKTB );
			MonyPtrAry.Add( Sn_5FSZKTC );
			MonyPtrAry.Add( Sn_5FSZTT );

			BOOL findFlg = FALSE;
			for( int i=0;; i++ ){
				if( strlen(tagData50[i].tagName) == 0 ){
					break;
				}
				if( m_isTransitionalMeasure == FALSE ){
					if( (tagData50[i].sign1==0) || (tagData50[i].sign1==1) ){
						continue;
					}
				}

				findFlg = FALSE;
				for( int j=0; j<updCnt; j++ ){
					if( updTagAry[j].CompareNoCase(tagData50[i].tagName) == 0 ){
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
					pShLstVal->itmId = tagData50[i].tagName;
					m_Util.val_to_asci( &pShLstVal->val, MonyPtrAry[i] );
					pShLstVal->sign1 = 0;
					pShLstVal->Update();
				}
			}
		}
	}

	// ê\çêèëÉfÅ[É^
	filter.Format( _T("Sn_SEQ = %d"), sn_seq );
	BOOL openFlg = FALSE;
	if( pDBNpSub->szsnk == NULL ){
		if( pDBNpSub->SzsnkOpen(filter) ){
			return -1;
		}
		openFlg = TRUE;
	}
	else{
		pDBNpSub->szsnk->Requery( filter );
	}

	if( (pDBNpSub->szsnk->MoveFirst()!=-1) && (pDBNpSub->szsnk->st!=-1) ){

		pDBNpSub->szsnk->Edit();
		pDBNpSub->szsnk->Sn_5FSZsw = Sn_5FSZsw;
		pDBNpSub->szsnk->Sn_5FSBsw = Sn_5FSBsw;
		pDBNpSub->szsnk->Sn_5FCQsw = Sn_5FCQsw;
//		pDBNpSub->szsnk->Sn_5FGKsw = Sn_5FGKsw;
		pDBNpSub->szsnk->Update();
	}

	if( openFlg ){
		pDBNpSub->SzsnkClose();
	}

	return 0;
}
#endif

//-----------------------------------------------------------------------------
// èWåvçœÇ›ÉfÅ[É^ÇÃÉZÉbÉg
//-----------------------------------------------------------------------------
// à¯êî	pSyzSyukei	ÅF	è¡îÔê≈èWåvÉNÉâÉX
//		isPluralZei	ÅF	ï°êîê≈ó¶
//-----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	ê≥èÌèIóπ
//			-1		ÅF	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CH26SnFhyo50Data::SetCalqedData( CSyzSyukei *pSyzSyukei, BOOL &isPluralZei )
{
	MoneyBasejagArray money;

	money = pSyzSyukei->GetShinkokuData( _T("15") );

	int cnt = (int)money.GetCount();
	int incnt = 0;

	unsigned	char	exZeiflg = 0x00;	// Å†Å†Å†Å† Å†Å°Å°Å°
											//  D0ÅF3%
											//  D1ÅF5%
											//  D2ÅF8%

	for( int i=0; i<cnt; i++ ){
		incnt = (int)money[i].GetCount();
		for( int j=0; j<incnt; j++ ){
			if( money[i][j].code.Left(4) == _T("1506") ){
				if( money[i][j].code == _T("150601") ){
					memmove( Sn_5FURITA, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("150602") ){
					memmove( Sn_5FURITB, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("150603") ){
					memmove( Sn_5FURITC, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("150600") ){
					memmove( Sn_5FURIK, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("1507") ){
				if( money[i][j].code == _T("150701") ){
					memmove( Sn_5FURI1A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("150702") ){
					memmove( Sn_5FURI1B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("150703") ){
					memmove( Sn_5FURI1C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
			}
			else if( money[i][j].code.Left(4) == _T("1508") ){
				if( money[i][j].code == _T("150801") ){
					memmove( Sn_5FURI2A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("150802") ){
					memmove( Sn_5FURI2B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("150803") ){
					memmove( Sn_5FURI2C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
			}
			else if( money[i][j].code.Left(4) == _T("1509") ){
				if( money[i][j].code == _T("150901") ){
					memmove( Sn_5FURI3A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("150902") ){
					memmove( Sn_5FURI3B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("150903") ){
					memmove( Sn_5FURI3C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
			}
			else if( money[i][j].code.Left(4) == _T("1510") ){
				if( money[i][j].code == _T("151001") ){
					memmove( Sn_5FURI4A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("151002") ){
					memmove( Sn_5FURI4B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("151003") ){
					memmove( Sn_5FURI4C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
			}
			else if( money[i][j].code.Left(4) == _T("1511") ){
				if( money[i][j].code == _T("151101") ){
					memmove( Sn_5FURI5A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("151102") ){
					memmove( Sn_5FURI5B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("151103") ){
					memmove( Sn_5FURI5C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
			}
			else if( money[i][j].code.Left(4) == _T("1512") ){
				if( money[i][j].code == _T("151201") ){
					memmove( Sn_5FUZKTA, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("151202") ){
					memmove( Sn_5FUZKTB, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("151203") ){
					memmove( Sn_5FUZKTC, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("151200") ){
					memmove( Sn_5FUZTT, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("1513") ){
				if( money[i][j].code == _T("151301") ){
					memmove( Sn_5FUZK1A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("151302") ){
					memmove( Sn_5FUZK1B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("151303") ){
					memmove( Sn_5FUZK1C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("151300") ){
					memmove( Sn_5FUZT1, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("1514") ){
				if( money[i][j].code == _T("151401") ){
					memmove( Sn_5FUZK2A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("151402") ){
					memmove( Sn_5FUZK2B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("151403") ){
					memmove( Sn_5FUZK2C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("151400") ){
					memmove( Sn_5FUZT2, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("1515") ){
				if( money[i][j].code == _T("151501") ){
					memmove( Sn_5FUZK3A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("151502") ){
					memmove( Sn_5FUZK3B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("151503") ){
					memmove( Sn_5FUZK3C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("151500") ){
					memmove( Sn_5FUZT3, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("1516") ){
				if( money[i][j].code == _T("151601") ){
					memmove( Sn_5FUZK4A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("151602") ){
					memmove( Sn_5FUZK4B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("151603") ){
					memmove( Sn_5FUZK4C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("151600") ){
					memmove( Sn_5FUZT4, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("1517") ){
				if( money[i][j].code == _T("151701") ){
					memmove( Sn_5FUZK5A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("151702") ){
					memmove( Sn_5FUZK5B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("151703") ){
					memmove( Sn_5FUZK5C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("151700") ){
					memmove( Sn_5FUZT5, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("1518") ){
				if( money[i][j].code == _T("151801") ){
					memmove( Sn_5FSZKGA, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("151802") ){
					memmove( Sn_5FSZKGB, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("151803") ){
					memmove( Sn_5FSZKGC, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("151800") ){
					memmove( Sn_5FSZTG, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("1519") ){
				if( money[i][j].code == _T("151901") ){
					memmove( Sn_5FSZKTA, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("151902") ){
					memmove( Sn_5FSZKTB, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("151903") ){
					memmove( Sn_5FSZKTC, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("151900") ){
					memmove( Sn_5FSZTT, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("1520") ){
				if( money[i][j].code == _T("152001") ){
					memmove( Sn_5FSZT20A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("152002") ){
					memmove( Sn_5FSZT20B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("152003") ){
					memmove( Sn_5FSZT20C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("152000") ){
					memmove( Sn_5FSZT20T, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("1521") ){
				if( money[i][j].code == _T("152101") ){
					memmove( Sn_5FSZT21A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("152102") ){
					memmove( Sn_5FSZT21B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("152103") ){
					memmove( Sn_5FSZT21C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("152100") ){
					memmove( Sn_5FSZT21T, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("1522") ){
				if( money[i][j].code == _T("152201") ){
					memmove( Sn_5FSZT22A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("152202") ){
					memmove( Sn_5FSZT22B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("152203") ){
					memmove( Sn_5FSZT22C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("152200") ){
					memmove( Sn_5FSZT22T, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("1523") ){
				if( money[i][j].code == _T("152301") ){
					memmove( Sn_5FSZT23A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("152302") ){
					memmove( Sn_5FSZT23B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("152303") ){
					memmove( Sn_5FSZT23C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("152300") ){
					memmove( Sn_5FSZT23T, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("1524") ){
				if( money[i][j].code == _T("152401") ){
					memmove( Sn_5FSZT24A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("152402") ){
					memmove( Sn_5FSZT24B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("152403") ){
					memmove( Sn_5FSZT24C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("152400") ){
					memmove( Sn_5FSZT24T, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("1525") ){
				if( money[i][j].code == _T("152501") ){
					memmove( Sn_5FSZT25A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("152502") ){
					memmove( Sn_5FSZT25B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("152503") ){
					memmove( Sn_5FSZT25C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("152500") ){
					memmove( Sn_5FSZT25T, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("1526") ){
				if( money[i][j].code == _T("152601") ){
					memmove( Sn_5FSZT26A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("152602") ){
					memmove( Sn_5FSZT26B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("152603") ){
					memmove( Sn_5FSZT26C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("152600") ){
					memmove( Sn_5FSZT26T, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("1527") ){
				if( money[i][j].code == _T("152701") ){
					memmove( Sn_5FSZT27A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("152702") ){
					memmove( Sn_5FSZT27B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("152703") ){
					memmove( Sn_5FSZT27C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("152700") ){
					memmove( Sn_5FSZT27T, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("1528") ){
				if( money[i][j].code == _T("152801") ){
					memmove( Sn_5FSZT28A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("152802") ){
					memmove( Sn_5FSZT28B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("152803") ){
					memmove( Sn_5FSZT28C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("152800") ){
					memmove( Sn_5FSZT28T, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("1529") ){
				if( money[i][j].code == _T("152901") ){
					memmove( Sn_5FSZT29A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("152902") ){
					memmove( Sn_5FSZT29B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("152903") ){
					memmove( Sn_5FSZT29C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("152900") ){
					memmove( Sn_5FSZT29T, money[i][j].arith, MONY_BUF_SIZE );
				}
			}

			else{
				break;
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
int CH26SnFhyo50Data::GetPastData( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}

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

	int nowVersion = 1;
	filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_5_2_HYOID, nowVersion );

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

		// 6.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz
		if( pShLstVal->itmId == _T("AHG00040") ){
			pShLstVal->Edit();
//			pShLstVal->val = Sn_5FURITB;
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AHG00050") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FURIT;
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AHG00070") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_KURIA;
			pShLstVal->Update();
		}
		// 7.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ëÊÇPã∆éÌ)
		else if( pShLstVal->itmId == _T("AHG00090") ){
			pShLstVal->Edit();
//			pShLstVal->val = pSZSNK->Sn_5FURI1A;
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AHG00100") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FURI[0];
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AHG00120") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_KURI[0];
			pShLstVal->Update();
		}
		// 8.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ëÊÇQã∆éÌ)
		else if( pShLstVal->itmId == _T("AHG00150") ){		
			pShLstVal->Edit();
//			pShLstVal->val = Sn_5FURI2A;
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AHG00160") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FURI[1];
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AHG00180") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_KURI[1];
			pShLstVal->Update();
		}
		// 9.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ëÊÇRã∆éÌ)
		else if( pShLstVal->itmId == _T("AHG00210") ){		
			pShLstVal->Edit();
//			pShLstVal->val = pSZSNK->Sn_5FURI3A;
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AHG00220") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FURI[2];
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AHG00240") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_KURI[2];
			pShLstVal->Update();
		}
		// 10.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ëÊÇSã∆éÌ)
		else if( pShLstVal->itmId == _T("AHG00270") ){		
			pShLstVal->Edit();
//			pShLstVal->val = pSZSNK->Sn_5FURI4A;
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AHG00280") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FURI[3];
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AHG00300") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_KURI[3];
			pShLstVal->Update();
		}
		// 11.éñã∆ãÊï™ï  â€ê≈îÑè„çÇçáåväz(ëÊÇTã∆éÌ)
		else if( pShLstVal->itmId == _T("AHG00330") ){		
			pShLstVal->Edit();
//			pShLstVal->val = pSZSNK->Sn_5FURI5A;
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AHG00340") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FURI[4];
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AHG00360") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_KURI[4];
			pShLstVal->Update();
		}
		// 13.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇPéÌÅj
		else if( pShLstVal->itmId == _T("AHG00450") ){		
			pShLstVal->Edit();
//			pShLstVal->val = pSZSNK->Sn_5FUZK1A;
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AHG00460") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FUZK[0];
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AHG00480") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FUZT[0];
			pShLstVal->Update();
		}
		// 14.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇQéÌÅj
		else if( pShLstVal->itmId == _T("AHG00500") ){		
			pShLstVal->Edit();
//			pShLstVal->val = pSZSNK->Sn_5FUZK2A;
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AHG00510") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FUZK[1];
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AHG00530") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FUZT[1];
			pShLstVal->Update();
		}
		// 15.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇRéÌÅj
		else if( pShLstVal->itmId == _T("AHG00550") ){		
			pShLstVal->Edit();
//			pShLstVal->val = pSZSNK->Sn_5FUZK3A;
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AHG00560") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FUZK[2];
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AHG00580") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FUZT[2];
			pShLstVal->Update();
		}
		// 16.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇSéÌÅj
		else if( pShLstVal->itmId == _T("AHG00600") ){		
			pShLstVal->Edit();
//			pShLstVal->val = pSZSNK->Sn_5FUZK4A;
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AHG00610") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FUZK[3];
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AHG00630") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FUZT[3];
			pShLstVal->Update();
		}
		// 17.éñã∆ãÊï™ï  è¡îÔê≈äzÅiëÊÇTéÌÅj
		else if( pShLstVal->itmId == _T("AHG00650") ){		
			pShLstVal->Edit();
//			pShLstVal->val = pSZSNK->Sn_5FUZK5A;
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AHG00660") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FUZK[4];
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AHG00680") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FUZT[4];
			pShLstVal->Update();
		}
		// 18.å¥ë•åvéZÇìKópÇ∑ÇÈèÍçáÇÃçTèúëŒè€édì¸ê≈äz
		else if( pShLstVal->itmId == _T("AHG00710") ){	
			pShLstVal->Edit();
//			pShLstVal->val = pSZSNK->Sn_5FSZTG;
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AHG00720") ){	
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FSZKG;
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AHG00740") ){	
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FSZTG;
			pShLstVal->sign1 = pSZSNK->Sn_5FGKsw;
			pShLstVal->Update();
		}
		// 19.ì¡ó·åvéZÇìKópÇ∑ÇÈèÍçáÇÃçTèúëŒè€édì¸ê≈äz
		else if( pShLstVal->itmId == _T("AHG00770") ){
			pShLstVal->Edit();
//			pShLstVal->val = pSZSNK->Sn_5FSZKT;
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AHG00780") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FSZKT;
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AHG00800") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FSZTT;
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
int CH26SnFhyo50Data::ClearDataByRenew( CDBNpSub *pDBNpSub, int type )
{
	m_Arith.l_clear( Sn_5FURIK );
	m_Arith.l_clear( Sn_5FURITA );
	m_Arith.l_clear( Sn_5FURITB );
	m_Arith.l_clear( Sn_5FURITC );
	Sn_5FURITsw = 0;

	m_Arith.l_clear( Sn_5FURI1A );
	m_Arith.l_clear( Sn_5FURI2A );
	m_Arith.l_clear( Sn_5FURI3A );
	m_Arith.l_clear( Sn_5FURI4A );
	m_Arith.l_clear( Sn_5FURI5A );

	m_Arith.l_clear( Sn_5FURI1B );
	m_Arith.l_clear( Sn_5FURI2B );
	m_Arith.l_clear( Sn_5FURI3B );
	m_Arith.l_clear( Sn_5FURI4B );
	m_Arith.l_clear( Sn_5FURI5B );

	m_Arith.l_clear( Sn_5FURI1C );
	m_Arith.l_clear( Sn_5FURI2C );
	m_Arith.l_clear( Sn_5FURI3C );
	m_Arith.l_clear( Sn_5FURI4C );
	m_Arith.l_clear( Sn_5FURI5C );

	m_Arith.l_clear( Sn_5FUZKTA );
	m_Arith.l_clear( Sn_5FUZK1A );
	m_Arith.l_clear( Sn_5FUZK2A );
	m_Arith.l_clear( Sn_5FUZK3A );
	m_Arith.l_clear( Sn_5FUZK4A );
	m_Arith.l_clear( Sn_5FUZK5A );

	m_Arith.l_clear( Sn_5FUZKTB );
	m_Arith.l_clear( Sn_5FUZK1B );
	m_Arith.l_clear( Sn_5FUZK2B );
	m_Arith.l_clear( Sn_5FUZK3B );
	m_Arith.l_clear( Sn_5FUZK4B );
	m_Arith.l_clear( Sn_5FUZK5B );

	m_Arith.l_clear( Sn_5FUZKTC );
	m_Arith.l_clear( Sn_5FUZK1C );
	m_Arith.l_clear( Sn_5FUZK2C );
	m_Arith.l_clear( Sn_5FUZK3C );
	m_Arith.l_clear( Sn_5FUZK4C );
	m_Arith.l_clear( Sn_5FUZK5C );

	Sn_5FUZK1sw = 0;
	Sn_5FUZK2sw = 0;
	Sn_5FUZK3sw = 0;
	Sn_5FUZK4sw = 0;
	Sn_5FUZK5sw = 0;
	Sn_5FUZKTsw = 0;

	m_Arith.l_clear( Sn_5FUZTT );
	m_Arith.l_clear( Sn_5FUZT1 );
	m_Arith.l_clear( Sn_5FUZT2 );
	m_Arith.l_clear( Sn_5FUZT3 );
	m_Arith.l_clear( Sn_5FUZT4 );
	m_Arith.l_clear( Sn_5FUZT5 );

	m_Arith.l_clear( Sn_5FSZTG );
	m_Arith.l_clear( Sn_5FSZKGA );
	m_Arith.l_clear( Sn_5FSZKGB );
	m_Arith.l_clear( Sn_5FSZKGC );
	Sn_5FGKsw = 0;

	m_Arith.l_clear( Sn_5FSZTT );
	m_Arith.l_clear( Sn_5FSZKTA );
	m_Arith.l_clear( Sn_5FSZKTB );
	m_Arith.l_clear( Sn_5FSZKTC );
	Sn_5FSZT19sw = 0;

	m_Arith.l_clear( Sn_5FSZT20T );
	m_Arith.l_clear( Sn_5FSZT20A );
	m_Arith.l_clear( Sn_5FSZT20B );
	m_Arith.l_clear( Sn_5FSZT20C );
	Sn_5FSZT20sw = 0;

	m_Arith.l_clear( Sn_5FSZT21T );
	m_Arith.l_clear( Sn_5FSZT21A );
	m_Arith.l_clear( Sn_5FSZT21B );
	m_Arith.l_clear( Sn_5FSZT21C );
	Sn_5FSZT21sw = 0;

	m_Arith.l_clear( Sn_5FSZT22T );
	m_Arith.l_clear( Sn_5FSZT22A );
	m_Arith.l_clear( Sn_5FSZT22B );
	m_Arith.l_clear( Sn_5FSZT22C );
	Sn_5FSZT22sw = 0;

	m_Arith.l_clear( Sn_5FSZT23T );
	m_Arith.l_clear( Sn_5FSZT23A );
	m_Arith.l_clear( Sn_5FSZT23B );
	m_Arith.l_clear( Sn_5FSZT23C );
	Sn_5FSZT23sw = 0;

	m_Arith.l_clear( Sn_5FSZT24T );
	m_Arith.l_clear( Sn_5FSZT24A );
	m_Arith.l_clear( Sn_5FSZT24B );
	m_Arith.l_clear( Sn_5FSZT24C );
	Sn_5FSZT24sw = 0;

	m_Arith.l_clear( Sn_5FSZT25T );
	m_Arith.l_clear( Sn_5FSZT25A );
	m_Arith.l_clear( Sn_5FSZT25B );
	m_Arith.l_clear( Sn_5FSZT25C );
	Sn_5FSZT25sw = 0;

	m_Arith.l_clear( Sn_5FSZT26T );
	m_Arith.l_clear( Sn_5FSZT26A );
	m_Arith.l_clear( Sn_5FSZT26B );
	m_Arith.l_clear( Sn_5FSZT26C );
	Sn_5FSZT26sw = 0;

	m_Arith.l_clear( Sn_5FSZT27T );
	m_Arith.l_clear( Sn_5FSZT27A );
	m_Arith.l_clear( Sn_5FSZT27B );
	m_Arith.l_clear( Sn_5FSZT27C );
	Sn_5FSZT27sw = 0;

	m_Arith.l_clear( Sn_5FSZT28T );
	m_Arith.l_clear( Sn_5FSZT28A );
	m_Arith.l_clear( Sn_5FSZT28B );
	m_Arith.l_clear( Sn_5FSZT28C );
	Sn_5FSZT28sw = 0;

	m_Arith.l_clear( Sn_5FSZT29T );
	m_Arith.l_clear( Sn_5FSZT29A );
	m_Arith.l_clear( Sn_5FSZT29B );
	m_Arith.l_clear( Sn_5FSZT29C );
	Sn_5FSZT29sw = 0;

	Sn_5FUP1 = 0;
	Sn_5FUP2 = 0;
	Sn_5FUP3 = 0;
	Sn_5FUP4 = 0;
	Sn_5FUP5 = 0;
	Sn_5FSZsw = 0;
	Sn_5FSBsw = 0;
	Sn_5FCQsw = 0;

	return 0;
}

//----------------------------------------------------------------------------
// éËì¸óÕÉTÉCÉìÇÃÉNÉäÉA
//----------------------------------------------------------------------------
// à¯êî	isChgKzritu	ÅF	åoâﬂê≈ó¶ÇÃóLñ≥Ç…à·Ç¢Ç™Ç†ÇÈÇ©Ç«Ç§Ç©
//----------------------------------------------------------------------------
// ï‘ëóíl	0		ÅF	ê≥èÌèIóπ
//----------------------------------------------------------------------------
int CH26SnFhyo50Data::ClearManualInputSign( BOOL isChgKzritu )
{
	Sn_5FURITsw = 0;

	Sn_5FUZK1sw = 0;
	Sn_5FUZK2sw = 0;
	Sn_5FUZK3sw = 0;
	Sn_5FUZK4sw = 0;
	Sn_5FUZK5sw = 0;
//--> '14.05.17 INS START
	if( isChgKzritu ){
		Sn_5FUZK1sw |= 0x111;
		Sn_5FUZK2sw |= 0x111;
		Sn_5FUZK3sw |= 0x111;
		Sn_5FUZK4sw |= 0x111;
		Sn_5FUZK5sw |= 0x111;
	}
//<-- '14.05.17 INS END
	Sn_5FUZKTsw = 0;
	Sn_5FGKsw = 0;
	Sn_5FSZT19sw = 0;
	Sn_5FSZT20sw = 0;
	Sn_5FSZT21sw = 0;
	Sn_5FSZT22sw = 0;
	Sn_5FSZT23sw = 0;
	Sn_5FSZT24sw = 0;
	Sn_5FSZT25sw = 0;
	Sn_5FSZT26sw = 0;
	Sn_5FSZT27sw = 0;
	Sn_5FSZT28sw = 0;
	Sn_5FSZT29sw = 0;

	Sn_5FSZsw = 0;
	Sn_5FSBsw = 0;
	Sn_5FCQsw = 0;

	return 0;
}

//----------------------------------------------------------------------------
// â°åvÇ≈ÇÕÇ»Ç¢ó·äOåvéZÇ™ïKóvÅH ('14.10.17)
//----------------------------------------------------------------------------
// ï‘ëóíl	TRUE	ÅF	ïKóv
//			FALSE	ÅF	ïsóv
//----------------------------------------------------------------------------
BOOL CH26SnFhyo50Data::IsNeedExpCalq()
{
	BOOL	bRt = FALSE;

	// É}ÉCÉiÉXè¡îÔê≈äz
	if( m_Arith.l_test(Sn_5FUZK1A) < 0 ){
		bRt = TRUE;
	}
	else if( m_Arith.l_test(Sn_5FUZK2A) < 0 ){
		bRt = TRUE;
	}
	else if( m_Arith.l_test(Sn_5FUZK3A) < 0 ){
		bRt = TRUE;
	}
	else if( m_Arith.l_test(Sn_5FUZK4A) < 0 ){
		bRt = TRUE;
	}
	else if( m_Arith.l_test(Sn_5FUZK5A) < 0 ){
		bRt = TRUE;
	}
	else if( m_Arith.l_test(Sn_5FUZK1B) < 0 ){
		bRt = TRUE;
	}
	else if( m_Arith.l_test(Sn_5FUZK2B) < 0 ){
		bRt = TRUE;
	}
	else if( m_Arith.l_test(Sn_5FUZK3B) < 0 ){
		bRt = TRUE;
	}
	else if( m_Arith.l_test(Sn_5FUZK4B) < 0 ){
		bRt = TRUE;
	}
	else if( m_Arith.l_test(Sn_5FUZK5B) < 0 ){
		bRt = TRUE;
	}
	else if( m_Arith.l_test(Sn_5FUZK1C) < 0 ){
		bRt = TRUE;
	}
	else if( m_Arith.l_test(Sn_5FUZK2C) < 0 ){
		bRt = TRUE;
	}
	else if( m_Arith.l_test(Sn_5FUZK3C) < 0 ){
		bRt = TRUE;
	}
	else if( m_Arith.l_test(Sn_5FUZK4C) < 0 ){
		bRt = TRUE;
	}
	else if( m_Arith.l_test(Sn_5FUZK5C) < 0 ){
		bRt = TRUE;
	}

	return bRt;
}
