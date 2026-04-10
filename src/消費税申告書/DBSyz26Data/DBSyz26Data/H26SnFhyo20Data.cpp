#include "StdAfx.h"
#include "H26SnFhyo20Data.h"

static SH_TAGNAME	tagData20[] = {
	{_T("AEB00030"), 1 },
	{_T("AEB00040"), 0 },
	{_T("AEB00050"), 3 },

	{_T("AEB00070"), 999 },
	{_T("AEB00080"), 999 },
	{_T("AEB00120"), 999 },

	{_T("AEC00020"), 1 },
/*== '14.05.13 ==*/
///*- '14.04.24 -*/
////	{_T("AEC00030"), 0 },
///*-------------*/
//	{_T("ADC00030"), 0 },
///*-------------*/
/*===============*/
	{_T("AEC00030"), 0 },
/*===============*/
	{_T("AEC00040"), 3 },

	{_T("AEC00070"), 1 },
	{_T("AEC00080"), 0 },
	{_T("AEC00090"), 3 },

	{_T("AEC00120"), 1 },
	{_T("AEC00130"), 0 },
	{_T("AEC00140"), 3 },

	{_T("AEE00030"), 1 },
	{_T("AEE00040"), 0 },
	{_T("AEE00050"), 3 },

	{_T("AEE00080"), 1 },
	{_T("AEE00090"), 0 },
	{_T("AEE00100"), 3 },

	{_T("AEE00130"), 1 },
	{_T("AEE00140"), 0 },
	{_T("AEE00150"), 3 },

	{_T("AEE00180"), 1 },
	{_T("AEE00190"), 0 },
	{_T("AEE00200"), 3 },

	{_T("AEF00020"), 1 },
	{_T("AEF00030"), 0 },
	{_T("AEF00040"), 3 },

	{_T("AEF00070"), 1 },
	{_T("AEF00080"), 0 },
	{_T("AEF00090"), 3 },

	{_T("AEG00020"), 1 },
	{_T("AEG00030"), 0 },
	{_T("AEG00040"), 3 },

	{_T("AEH00010"), 1 },
	{_T("AEH00020"), 0 },
	{_T("AEH00030"), 3 },

	{_T("") }
};


//-----------------------------------------------------------------------------
// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
//-----------------------------------------------------------------------------
CH26SnFhyo20Data::CH26SnFhyo20Data( BOOL isTransitionalMeasure )
	: m_isTransitionalMeasure(isTransitionalMeasure)
{
	memset( Sn_2F1A, '\0', sizeof(Sn_2F1A) );
	memset( Sn_2F1B, '\0', sizeof(Sn_2F1B) );
	memset( Sn_2F1C, '\0', sizeof(Sn_2F1C) );
	memset( Sn_2F2A, '\0', sizeof(Sn_2F2A) );
	memset( Sn_2F2B, '\0', sizeof(Sn_2F2B) );
	memset( Sn_2F2C, '\0', sizeof(Sn_2F2C) );
	memset( Sn_2F3A, '\0', sizeof(Sn_2F3A) );
	memset( Sn_2F3B, '\0', sizeof(Sn_2F3B) );
	memset( Sn_2F3C, '\0', sizeof(Sn_2F3C) );
	memset( Sn_2F6A, '\0', sizeof(Sn_2F6A) );
	memset( Sn_2F6B, '\0', sizeof(Sn_2F6B) );
	memset( Sn_2F6C, '\0', sizeof(Sn_2F6C) );
	memset( Sn_2F8A, '\0', sizeof(Sn_2F8A) );
	memset( Sn_2F8B, '\0', sizeof(Sn_2F8B) );
	memset( Sn_2F8C, '\0', sizeof(Sn_2F8C) );
	memset( Sn_2F9A, '\0', sizeof(Sn_2F9A) );
	memset( Sn_2F9B, '\0', sizeof(Sn_2F9B) );
	memset( Sn_2F9C, '\0', sizeof(Sn_2F9C) );
	memset( Sn_2F10A, '\0', sizeof(Sn_2F10A) );
	memset( Sn_2F10B, '\0', sizeof(Sn_2F10B) );
	memset( Sn_2F10C, '\0', sizeof(Sn_2F10C) );
	memset( Sn_2F11A, '\0', sizeof(Sn_2F11A) );
	memset( Sn_2F11B, '\0', sizeof(Sn_2F11B) );
	memset( Sn_2F11C, '\0', sizeof(Sn_2F11C) );
	memset( Sn_2F15A, '\0', sizeof(Sn_2F15A) );
	memset( Sn_2F15B, '\0', sizeof(Sn_2F15B) );
	memset( Sn_2F15C, '\0', sizeof(Sn_2F15C) );
	memset( Sn_2F16A, '\0', sizeof(Sn_2F16A) );
	memset( Sn_2F16B, '\0', sizeof(Sn_2F16B) );
	memset( Sn_2F16C, '\0', sizeof(Sn_2F16C) );
	memset( Sn_2F17A, '\0', sizeof(Sn_2F17A) );
	memset( Sn_2F17B, '\0', sizeof(Sn_2F17B) );
	memset( Sn_2F17C, '\0', sizeof(Sn_2F17C) );
	memset( Sn_2F18A, '\0', sizeof(Sn_2F18A) );
	memset( Sn_2F18B, '\0', sizeof(Sn_2F18B) );
	memset( Sn_2F18C, '\0', sizeof(Sn_2F18C) );
	memset( Sn_2F19A, '\0', sizeof(Sn_2F19A) );
	memset( Sn_2F19B, '\0', sizeof(Sn_2F19B) );
	memset( Sn_2F19C, '\0', sizeof(Sn_2F19C) );
	memset( Sn_2F20A, '\0', sizeof(Sn_2F20A) );
	memset( Sn_2F20B, '\0', sizeof(Sn_2F20B) );
	memset( Sn_2F20C, '\0', sizeof(Sn_2F20C) );
	memset( Sn_2F21A, '\0', sizeof(Sn_2F21A) );
	memset( Sn_2F21B, '\0', sizeof(Sn_2F21B) );
	memset( Sn_2F21C, '\0', sizeof(Sn_2F21C) );
	memset( Sn_2F23A, '\0', sizeof(Sn_2F23A) );
	memset( Sn_2F23B, '\0', sizeof(Sn_2F23B) );
	memset( Sn_2F23C, '\0', sizeof(Sn_2F23C) );

	Sn_2FUWS = 0;
	Sn_2FUWSEX = 0;


	Sn_2F9sw = 0;//[Y]

	Sn_2F21sw = 0;

	m_Arith.l_defn( 0x16 );

	memset( rsv1, '\0', sizeof(rsv1) );
	memset( rsv2, '\0', sizeof(rsv2) );
}

//-----------------------------------------------------------------------------
// ƒfƒXƒgƒ‰ƒNƒ^
//-----------------------------------------------------------------------------
CH26SnFhyo20Data::~CH26SnFhyo20Data()
{
	m_DBSyzAccess.CloseShLstVal();
	m_DBSyzAccess.CloseShLstRate();
}

#ifdef _20140219_EXTEND_
//-----------------------------------------------------------------------------
// DB ‚©‚çƒf[ƒ^“Ç‚İ
//-----------------------------------------------------------------------------
// ˆø”	pDBNpSub	F	à–±ƒNƒ‰ƒX‚Ìƒ|ƒCƒ“ƒ^
//		sn_seq		F	‘ÎÛ‚Ì—š—ğƒV[ƒPƒ“ƒX
//-----------------------------------------------------------------------------
// •Ô‘—’l	0		F	³íI—¹
//			-1		F	ƒGƒ‰[
//-----------------------------------------------------------------------------
int CH26SnFhyo20Data::GetData( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}

	CString	filter;

	if( m_isTransitionalMeasure ){
		filter.Format( _T("sn_seq = %d and hyoId = \'%s\'"), sn_seq, ICS_SN_FHYO_2_2_HYOID );
	}
	else{
		filter.Format( _T("sn_seq = %d and hyoId = \'%s\'"), sn_seq, ICS_SN_FHYO_2_HYOID );
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

			// ‹àŠz‚ğ•ÏŠ·‚µ‚Ä‘ã“ü
			if( pShLstVal->itmId == _T("AEB00030") ){			// ‰ÛÅ”„ãŠz
				m_Util.val_to_bin( Sn_2F1A, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AEB00040") ){
				m_Util.val_to_bin( Sn_2F1B, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AEB00050") ){
				m_Util.val_to_bin( Sn_2F1C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AEB00070") ){		// –ÆÅ”„ãŠz
				m_Util.val_to_bin( Sn_2F2C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AEB00080") ){		// —Ao‘Y‰¿Šz
				m_Util.val_to_bin( Sn_2F3C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AEB00120") ){		// ”ñ‰ÛÅ”„ãŠz
				m_Util.val_to_bin( Sn_2F6C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AEC00020") ){		// ‰ÛÅd“ü‚ÉŒW‚éx•¥‘Î‰¿‚ÌŠz
				m_Util.val_to_bin( Sn_2F8A, pShLstVal->val );
			}	
			else if( pShLstVal->itmId == _T("AEC00030") ){
				m_Util.val_to_bin( Sn_2F8B, pShLstVal->val );
			}	
			else if( pShLstVal->itmId == _T("AEC00040") ){
				m_Util.val_to_bin( Sn_2F8C, pShLstVal->val );
			}	
			else if( pShLstVal->itmId == _T("AEC00070") ){		// ‰ÛÅd“ü‚ÉŒW‚éÁ”ïÅŠz
				m_Util.val_to_bin( Sn_2F9A, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AEC00080") ){
				m_Util.val_to_bin( Sn_2F9B, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AEC00090") ){
				m_Util.val_to_bin( Sn_2F9C, pShLstVal->val );
			}
	//		else if( pShLstVal->itmId == _T("") ){		// ‰ÛÅ‰İ•¨‚ÉŒW‚éx•¥‘Î‰¿‚ÌŠz
	//			m_Util.val_to_bin( Sn_2F1C, pShLstVal->val );
	//		}
			else if( pShLstVal->itmId == _T("AEC00120") ){		// ‰ÛÅ‰İ•¨‚ÉŒW‚éÁ”ïÅŠz
				m_Util.val_to_bin( Sn_2F11A, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AEC00130") ){
				m_Util.val_to_bin( Sn_2F11B, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AEC00140") ){
				m_Util.val_to_bin( Sn_2F11C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AEE00030") ){		// ŒÂ•Ê‘Î‰F‰ÛÅ”„ã‚ÉŒW‚é
				m_Util.val_to_bin( Sn_2F15A, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AEE00040") ){
				m_Util.val_to_bin( Sn_2F15B, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AEE00050") ){
				m_Util.val_to_bin( Sn_2F15C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AEE00080") ){		// ŒÂ•Ê‘Î‰F‹¤’Ê‚Ì”„ã‚ÉŒW‚é
				m_Util.val_to_bin( Sn_2F16A, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AEE00090") ){
				m_Util.val_to_bin( Sn_2F16B, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AEE00100") ){
				m_Util.val_to_bin( Sn_2F16C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AEE00130") ){		// •t•\2¢16£-A ŒÂ•Ê‘Î‰TœÅŠzè“ü—ÍŠz
				m_Util.val_to_bin( Sn_2F17A, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AEE00140") ){		// •t•\2¢16£-B ŒÂ•Ê‘Î‰TœÅŠzè“ü—ÍŠz
				m_Util.val_to_bin( Sn_2F17B, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AEE00150") ){		// •t•\2¢16£-C ŒÂ•Ê‘Î‰TœÅŠzè“ü—ÍŠz
				m_Util.val_to_bin( Sn_2F17C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AEE00180") ){		// •t•\2¢17£-A ”ä—á”z•ªTœÅŠzè“ü—ÍŠz
				m_Util.val_to_bin( Sn_2F18A, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AEE00190") ){		// •t•\2¢17£-B ”ä—á”z•ªTœÅŠzè“ü—ÍŠz
				m_Util.val_to_bin( Sn_2F18B, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AEE00200") ){		// •t•\2¢17£-C ”ä—á”z•ªTœÅŠzè“ü—ÍŠz
				m_Util.val_to_bin( Sn_2F18C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AEF00020") ){		// TœÅŠz‚Ì’²®
				m_Util.val_to_bin( Sn_2F19A, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AEF00030") ){
				m_Util.val_to_bin( Sn_2F19B, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AEF00040") ){
				m_Util.val_to_bin( Sn_2F19C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AEF00070") ){		// TœÅŠz‚Ì’²®
				m_Util.val_to_bin( Sn_2F20A, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AEF00080") ){
				m_Util.val_to_bin( Sn_2F20B, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AEF00090") ){
				m_Util.val_to_bin( Sn_2F20C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AEG00020") ){		// •t•\2¢20£-A Tœ‘ÎÛd“üÅŠzè“ü—ÍŠz
				m_Util.val_to_bin( Sn_2F21A, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AEG00030") ){		// •t•\2¢20£-B Tœ‘ÎÛd“üÅŠzè“ü—ÍŠz
				m_Util.val_to_bin( Sn_2F21B, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AEG00040") ){		// •t•\2¢20£-C Tœ‘ÎÛd“üÅŠzè“ü—ÍŠz
				m_Util.val_to_bin( Sn_2F21C, pShLstVal->val );
				Sn_2F21sw = pShLstVal->sign1;
			}
			else if( pShLstVal->itmId == _T("AEH00010") ){		// ‘İ“|‰ñû‚ÉŒW‚éÁ”ïÅŠz
				m_Util.val_to_bin( Sn_2F23A, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AEH00020") ){
				m_Util.val_to_bin( Sn_2F23B, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("AEH00030") ){
				m_Util.val_to_bin( Sn_2F23C, pShLstVal->val );
			}
	//		else if( pShLstVal->itmId == _T("") ){				// ‰ÛÅ”„ãŠ„‡
	//			m_Util.val_to_bin( Sn_2F1C, pShLstVal->val );
	//		}
	//		else if( pShLstVal->itmId == _T("") ){				// ‰ÛÅ”„ãŠ„‡ è“ü—Í
	//			m_Util.val_to_bin( Sn_2F1C, pShLstVal->val );
	//		}
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

			// ‹àŠz‚ğ•ÏŠ·‚µ‚Ä‘ã“ü
			if( pShLstVal->itmId == _T("ADB00020") ){			// ‰ÛÅ•W€Šz
				m_Util.val_to_bin( Sn_2F1C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ADB00030") ){		// –ÆÅ”„ãŠz
				m_Util.val_to_bin( Sn_2F2C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ADB00040") ){		// —Ao‘Y‰¿Šz
				m_Util.val_to_bin( Sn_2F3C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ADB00080") ){		// ”ñ‰ÛÅ”„ãŠz
				m_Util.val_to_bin( Sn_2F6C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ADC00010") ){		// ‰ÛÅd“ü‚ÉŒW‚éx•¥‘Î‰¿‚ÌŠz
				m_Util.val_to_bin( Sn_2F8C, pShLstVal->val );
			}	
			else if( pShLstVal->itmId == _T("ADC00020") ){		// ‰ÛÅd“ü‚ÉŒW‚éÁ”ïÅŠz
				m_Util.val_to_bin( Sn_2F9C, pShLstVal->val );
			}
//			else if( pShLstVal->itmId == _T("") ){		// ‰ÛÅ‰İ•¨‚ÉŒW‚éx•¥‘Î‰¿‚ÌŠz
//				m_Util.val_to_bin( Sn_2F1C, pShLstVal->val );
//			}
			else if( pShLstVal->itmId == _T("ADC00030") ){		// ‰ÛÅ‰È–Ú‚ÉŒW‚éÁ”ïÅŠz
				m_Util.val_to_bin( Sn_2F11C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ADE00020") ){		// ŒÂ•Ê‘Î‰F‰ÛÅ”„ã‚ÉŒW‚é
				m_Util.val_to_bin( Sn_2F15C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ADE00030") ){		// ŒÂ•Ê‘Î‰F‹¤’Ê‚Ì”„ã‚ÉŒW‚é
				m_Util.val_to_bin( Sn_2F16C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ADE00040") ){		// •t•\2¢16£-A ŒÂ•Ê‘Î‰TœÅŠzè“ü—ÍŠz
				m_Util.val_to_bin( Sn_2F17C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ADE00050") ){		// •t•\2¢17£-A ”ä—á”z•ªTœÅŠzè“ü—ÍŠz
				m_Util.val_to_bin( Sn_2F18C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ADF00010") ){		// TœÅŠz‚Ì’²®
				m_Util.val_to_bin( Sn_2F19C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ADF00020") ){		// TœÅŠz‚Ì’²®
				m_Util.val_to_bin( Sn_2F20C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ADG00010") ){		// •t•\2¢20£-A Tœ‘ÎÛd“üÅŠzè“ü—ÍŠz
				m_Util.val_to_bin( Sn_2F21C, pShLstVal->val );
				Sn_2F21sw = pShLstVal->sign1;
			}
			else if( pShLstVal->itmId == _T("ADH00000") ){		// ‘İ“|‰ñû‚ÉŒW‚éÁ”ïÅŠz
				m_Util.val_to_bin( Sn_2F23C, pShLstVal->val );
			}
//			else if( pShLstVal->itmId == _T("") ){				// ‰ÛÅ”„ãŠ„‡
//				m_Util.val_to_bin( Sn_2F1C, pShLstVal->val );
//			}
//			else if( pShLstVal->itmId == _T("") ){				// ‰ÛÅ”„ãŠ„‡ è“ü—Í
//				m_Util.val_to_bin( Sn_2F1C, pShLstVal->val );
//			}
		}
	}

	return 0;
}
#else
//-----------------------------------------------------------------------------
// DB ‚©‚çƒf[ƒ^“Ç‚İ
//-----------------------------------------------------------------------------
// ˆø”	pDBNpSub	F	à–±ƒNƒ‰ƒX‚Ìƒ|ƒCƒ“ƒ^
//		sn_seq		F	‘ÎÛ‚Ì—š—ğƒV[ƒPƒ“ƒX
//-----------------------------------------------------------------------------
// •Ô‘—’l	0		F	³íI—¹
//			-1		F	ƒGƒ‰[
//-----------------------------------------------------------------------------
int CH26SnFhyo20Data::GetData( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}

	CString	filter;
	int		nowVersion = 1;
	filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_2_2_HYOID, nowVersion );

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

		// ‹àŠz‚ğ•ÏŠ·‚µ‚Ä‘ã“ü
		if( pShLstVal->itmId == _T("AEB00030") ){			// ‰ÛÅ”„ãŠz
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_2F1A, pShLstVal->val );
			}
			else{
				memset( Sn_2F1A, '\0', sizeof(Sn_2F1A) );
			}
		}
		else if( pShLstVal->itmId == _T("AEB00040") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_2F1B, pShLstVal->val );
			}
			else{
				memset( Sn_2F1B, '\0', sizeof(Sn_2F1A) );
			}
		}
		else if( pShLstVal->itmId == _T("AEB00050") ){
			m_Util.val_to_bin( Sn_2F1C, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AEB00070") ){		// –ÆÅ”„ãŠz
			m_Util.val_to_bin( Sn_2F2C, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AEB00080") ){		// —Ao‘Y‰¿Šz
			m_Util.val_to_bin( Sn_2F3C, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AEB00120") ){		// ”ñ‰ÛÅ”„ãŠz
			m_Util.val_to_bin( Sn_2F6C, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AEC00020") ){		// ‰ÛÅd“ü‚ÉŒW‚éx•¥‘Î‰¿‚ÌŠz
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_2F8A, pShLstVal->val );
			}
			else{
				memset( Sn_2F8A, '\0', sizeof(Sn_2F1A) );
			}
		}	
		else if( pShLstVal->itmId == _T("AEC00030") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_2F8B, pShLstVal->val );
			}
			else{
				memset( Sn_2F8B, '\0', sizeof(Sn_2F1A) );
			}
		}	
		else if( pShLstVal->itmId == _T("AEC00040") ){
			m_Util.val_to_bin( Sn_2F8C, pShLstVal->val );
		}	
		else if( pShLstVal->itmId == _T("AEC00070") ){		// ‰ÛÅd“ü‚ÉŒW‚éÁ”ïÅŠz
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_2F9A, pShLstVal->val );
			}
			else{
				memset( Sn_2F9A, '\0', sizeof(Sn_2F1A) );
			}
		}
		else if( pShLstVal->itmId == _T("AEC00080") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_2F9B, pShLstVal->val );
			}
			else{
				memset( Sn_2F9B, '\0', sizeof(Sn_2F1A) );
			}
		}
		else if( pShLstVal->itmId == _T("AEC00090") ){
			m_Util.val_to_bin( Sn_2F9C, pShLstVal->val );
			Sn_2F9sw = pShLstVal->sign1;//[Y]
		}
//		else if( pShLstVal->itmId == _T("") ){		// ‰ÛÅ‰İ•¨‚ÉŒW‚éx•¥‘Î‰¿‚ÌŠz
//			m_Util.val_to_bin( Sn_2F1C, pShLstVal->val );
//		}
		else if( pShLstVal->itmId == _T("AEC00120") ){		// ‰ÛÅ‰İ•¨‚ÉŒW‚éÁ”ïÅŠz
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_2F10A, pShLstVal->val );
			}
			else{
				memset( Sn_2F11A, '\0', sizeof(Sn_2F11A) );
			}
		}
		else if( pShLstVal->itmId == _T("AEC00130") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_2F10B, pShLstVal->val );
			}
			else{
				memset( Sn_2F11B, '\0', sizeof(Sn_2F11A) );
			}
		}
		else if( pShLstVal->itmId == _T("AEC00140") ){
			m_Util.val_to_bin( Sn_2F10C, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AEE00030") ){		// ŒÂ•Ê‘Î‰F‰ÛÅ”„ã‚ÉŒW‚é
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_2F15A, pShLstVal->val );
			}
			else{
				memset( Sn_2F15A, '\0', sizeof(Sn_2F1A) );
			}
		}
		else if( pShLstVal->itmId == _T("AEE00040") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_2F15B, pShLstVal->val );
			}
			else{
				memset( Sn_2F15A, '\0', sizeof(Sn_2F1A) );
			}
		}
		else if( pShLstVal->itmId == _T("AEE00050") ){
			m_Util.val_to_bin( Sn_2F15C, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AEE00080") ){		// ŒÂ•Ê‘Î‰F‹¤’Ê‚Ì”„ã‚ÉŒW‚é
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_2F16A, pShLstVal->val );
			}
			else{
				memset( Sn_2F16A, '\0', sizeof(Sn_2F1A) );
			}
		}
		else if( pShLstVal->itmId == _T("AEE00090") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_2F16B, pShLstVal->val );
			}
			else{
				memset( Sn_2F16B, '\0', sizeof(Sn_2F1A) );
			}
		}
		else if( pShLstVal->itmId == _T("AEE00100") ){
			m_Util.val_to_bin( Sn_2F16C, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AEE00130") ){		// •t•\2¢16£-A ŒÂ•Ê‘Î‰TœÅŠzè“ü—ÍŠz
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_2F17A, pShLstVal->val );
			}
			else{
				memset( Sn_2F17A, '\0', sizeof(Sn_2F1A) );
			}
		}
		else if( pShLstVal->itmId == _T("AEE00140") ){		// •t•\2¢16£-B ŒÂ•Ê‘Î‰TœÅŠzè“ü—ÍŠz
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_2F17B, pShLstVal->val );
			}
			else{
				memset( Sn_2F17B, '\0', sizeof(Sn_2F1A) );
			}
		}
		else if( pShLstVal->itmId == _T("AEE00150") ){		// •t•\2¢16£-C ŒÂ•Ê‘Î‰TœÅŠzè“ü—ÍŠz
			m_Util.val_to_bin( Sn_2F17C, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AEE00180") ){		// •t•\2¢17£-A ”ä—á”z•ªTœÅŠzè“ü—ÍŠz
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_2F18A, pShLstVal->val );
			}
			else{
				memset( Sn_2F18A, '\0', sizeof(Sn_2F1A) );
			}
		}
		else if( pShLstVal->itmId == _T("AEE00190") ){		// •t•\2¢17£-B ”ä—á”z•ªTœÅŠzè“ü—ÍŠz
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_2F18B, pShLstVal->val );
			}
			else{
				memset( Sn_2F18B, '\0', sizeof(Sn_2F1A) );
			}
		}
		else if( pShLstVal->itmId == _T("AEE00200") ){		// •t•\2¢17£-C ”ä—á”z•ªTœÅŠzè“ü—ÍŠz
			m_Util.val_to_bin( Sn_2F18C, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AEF00020") ){		// TœÅŠz‚Ì’²®
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_2F19A, pShLstVal->val );
			}
			else{
				memset( Sn_2F19A, '\0', sizeof(Sn_2F1A) );
			}
		}
		else if( pShLstVal->itmId == _T("AEF00030") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_2F19B, pShLstVal->val );
			}
			else{
				memset( Sn_2F19B, '\0', sizeof(Sn_2F1A) );
			}
		}
		else if( pShLstVal->itmId == _T("AEF00040") ){
			m_Util.val_to_bin( Sn_2F19C, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AEF00070") ){		// TœÅŠz‚Ì’²®
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_2F20A, pShLstVal->val );
			}
			else{
				memset( Sn_2F20A, '\0', sizeof(Sn_2F1A) );
			}
		}
		else if( pShLstVal->itmId == _T("AEF00080") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_2F20B, pShLstVal->val );
			}
			else{
				memset( Sn_2F20B, '\0', sizeof(Sn_2F1A) );
			}
		}
		else if( pShLstVal->itmId == _T("AEF00090") ){
			m_Util.val_to_bin( Sn_2F20C, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AEG00020") ){		// •t•\2¢20£-A Tœ‘ÎÛd“üÅŠzè“ü—ÍŠz
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_2F21A, pShLstVal->val );
			}
			else{
				memset( Sn_2F21A, '\0', sizeof(Sn_2F1A) );
			}
		}
		else if( pShLstVal->itmId == _T("AEG00030") ){		// •t•\2¢20£-B Tœ‘ÎÛd“üÅŠzè“ü—ÍŠz
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_2F21B, pShLstVal->val );
			}
			else{
				memset( Sn_2F21B, '\0', sizeof(Sn_2F1A) );
			}
		}
		else if( pShLstVal->itmId == _T("AEG00040") ){		// •t•\2¢20£-C Tœ‘ÎÛd“üÅŠzè“ü—ÍŠz
			m_Util.val_to_bin( Sn_2F21C, pShLstVal->val );
			Sn_2F21sw = pShLstVal->sign1;
		}
		else if( pShLstVal->itmId == _T("AEH00010") ){		// ‘İ“|‰ñû‚ÉŒW‚éÁ”ïÅŠz
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_2F23A, pShLstVal->val );
			}
			else{
				memset( Sn_2F23A, '\0', sizeof(Sn_2F1A) );
			}
		}
		else if( pShLstVal->itmId == _T("AEH00020") ){
			if( m_isTransitionalMeasure ){
				m_Util.val_to_bin( Sn_2F23B, pShLstVal->val );
			}
			else{
				memset( Sn_2F23B, '\0', sizeof(Sn_2F1A) );
			}
		}
		else if( pShLstVal->itmId == _T("AEH00030") ){
			m_Util.val_to_bin( Sn_2F23C, pShLstVal->val );
		}
//		else if( pShLstVal->itmId == _T("") ){				// ‰ÛÅ”„ãŠ„‡
//			m_Util.val_to_bin( Sn_2F1C, pShLstVal->val );
//		}
//		else if( pShLstVal->itmId == _T("") ){				// ‰ÛÅ”„ãŠ„‡ è“ü—Í
//			m_Util.val_to_bin( Sn_2F1C, pShLstVal->val );
//		}
	}
/*	}
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

			// ‹àŠz‚ğ•ÏŠ·‚µ‚Ä‘ã“ü
			if( pShLstVal->itmId == _T("AEB00050") ){			// ‰ÛÅ•W€Šz
				m_Util.val_to_bin( Sn_2F1C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ADB00030") ){		// –ÆÅ”„ãŠz
				m_Util.val_to_bin( Sn_2F2C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ADB00040") ){		// —Ao‘Y‰¿Šz
				m_Util.val_to_bin( Sn_2F3C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ADB00080") ){		// ”ñ‰ÛÅ”„ãŠz
				m_Util.val_to_bin( Sn_2F6C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ADC00010") ){		// ‰ÛÅd“ü‚ÉŒW‚éx•¥‘Î‰¿‚ÌŠz
				m_Util.val_to_bin( Sn_2F8C, pShLstVal->val );
			}	
			else if( pShLstVal->itmId == _T("ADC00020") ){		// ‰ÛÅd“ü‚ÉŒW‚éÁ”ïÅŠz
				m_Util.val_to_bin( Sn_2F9C, pShLstVal->val );
			}
//			else if( pShLstVal->itmId == _T("") ){		// ‰ÛÅ‰İ•¨‚ÉŒW‚éx•¥‘Î‰¿‚ÌŠz
//				m_Util.val_to_bin( Sn_2F1C, pShLstVal->val );
//			}
			else if( pShLstVal->itmId == _T("ADC00030") ){		// ‰ÛÅ‰È–Ú‚ÉŒW‚éÁ”ïÅŠz
				m_Util.val_to_bin( Sn_2F11C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ADE00020") ){		// ŒÂ•Ê‘Î‰F‰ÛÅ”„ã‚ÉŒW‚é
				m_Util.val_to_bin( Sn_2F15C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ADE00030") ){		// ŒÂ•Ê‘Î‰F‹¤’Ê‚Ì”„ã‚ÉŒW‚é
				m_Util.val_to_bin( Sn_2F16C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ADE00040") ){		// •t•\2¢16£-A ŒÂ•Ê‘Î‰TœÅŠzè“ü—ÍŠz
				m_Util.val_to_bin( Sn_2F17C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ADE00050") ){		// •t•\2¢17£-A ”ä—á”z•ªTœÅŠzè“ü—ÍŠz
				m_Util.val_to_bin( Sn_2F18C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ADF00010") ){		// TœÅŠz‚Ì’²®
				m_Util.val_to_bin( Sn_2F19C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ADF00020") ){		// TœÅŠz‚Ì’²®
				m_Util.val_to_bin( Sn_2F20C, pShLstVal->val );
			}
			else if( pShLstVal->itmId == _T("ADG00010") ){		// •t•\2¢20£-A Tœ‘ÎÛd“üÅŠzè“ü—ÍŠz
				m_Util.val_to_bin( Sn_2F21C, pShLstVal->val );
				Sn_2F21sw = pShLstVal->sign1;
			}
			else if( pShLstVal->itmId == _T("ADH00000") ){		// ‘İ“|‰ñû‚ÉŒW‚éÁ”ïÅŠz
				m_Util.val_to_bin( Sn_2F23C, pShLstVal->val );
			}
//			else if( pShLstVal->itmId == _T("") ){				// ‰ÛÅ”„ãŠ„‡
//				m_Util.val_to_bin( Sn_2F1C, pShLstVal->val );
//			}
//			else if( pShLstVal->itmId == _T("") ){				// ‰ÛÅ”„ãŠ„‡ è“ü—Í
//				m_Util.val_to_bin( Sn_2F1C, pShLstVal->val );
//			}
		}
	}*/


	filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_2_2_HYOID, nowVersion );

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
		
		if( pShLstRate->itmId == _T("AEB00140") ){
			Sn_2FUWS = pShLstRate->rate;
		}
		else if( pShLstRate->itmId == _T("AEB00141") ){
			Sn_2FUWSEX = pShLstRate->rate;
		}
	}

	return 0;
}
#endif

#ifdef _20140219_EXTEND_
//-----------------------------------------------------------------------------
// DB ‚Öƒf[ƒ^‘‚İ
//-----------------------------------------------------------------------------
// ˆø”	pDBNpSub	F	à–±ƒNƒ‰ƒX‚Ìƒ|ƒCƒ“ƒ^
//		sn_seq		F	‘ÎÛ‚Ì—š—ğƒV[ƒPƒ“ƒX
//-----------------------------------------------------------------------------
// •Ô‘—’l	0		F	³íI—¹
//			-1		F	ƒGƒ‰[
//-----------------------------------------------------------------------------
int CH26SnFhyo20Data::SetData( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT( pDBNpSub );

	CString	filter;
	char	*pHyoID = NULL;
	if( m_isTransitionalMeasure ){
		filter.Format( _T("sn_seq = %d and hyoId = \'%s\'"), sn_seq, ICS_SN_FHYO_2_2_HYOID );
		pHyoID = ICS_SN_FHYO_2_2_HYOID;
	}
	else{
		filter.Format( _T("sn_seq = %d and hyoId = \'%s\'"), sn_seq, ICS_SN_FHYO_2_HYOID );
		pHyoID = ICS_SN_FHYO_2_HYOID;
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

	if( (pShLstVal->MoveFirst()==-1) || (pShLstVal->st==-1) ){	// ƒf[ƒ^–³‚µ
		int	nowVersion = 1;

		if( m_isTransitionalMeasure ){
			// ‰ÛÅ”„ãŠz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEB00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F1A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEB00040");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F1B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEB00050");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F1C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// –ÆÅ”„ãŠz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEB00070");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F2C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// —Ao‘Y‰¿Šz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEB00080");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F3C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ”ñ‰ÛÅ”„ãŠz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEB00120");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F6C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ‰ÛÅd“ü‚ÉŒW‚éx•¥‘Î‰¿‚ÌŠz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ADC00010");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F8C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ‰ÛÅd“ü‚ÉŒW‚éÁ”ïÅŠz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEC00070");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F9A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEC00080");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F9B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEC00090");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F9C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ‰ÛÅ‰İ•¨‚ÉŒW‚éx•¥‘Î‰¿‚ÌŠz

			// ‰ÛÅ‰İ•¨‚ÉŒW‚éÁ”ïÅŠz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEC00120");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F11A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEC00130");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F11B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ADC00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F11C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ŒÂ•Ê‘Î‰F‰ÛÅ”„ã‚ÉŒW‚é
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEE00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F15A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEE00040");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F15B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEE00050");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F15C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ŒÂ•Ê‘Î‰F‹¤’Ê‚Ì”„ã‚ÉŒW‚é
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEE00080");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F16A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEE00090");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F16B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEE00100");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F16C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// •t•\2¢16£- ŒÂ•Ê‘Î‰TœÅŠzè“ü—ÍŠz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEE00130");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F17A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEE00140");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F17B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEE00150");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F17C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// •t•\2¢17£- ”ä—á”z•ªTœÅŠzè“ü—ÍŠz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEE00180");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F18A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEE00190");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F18B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEE00200");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F18C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// TœÅŠz‚Ì’²®
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEF00020");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F19A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEF00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F19B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEF00040");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F19C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// TœÅŠz‚Ì’²®
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEF00070");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F20A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEF00080");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F20B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEF00090");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F20C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// •t•\2¢20£- Tœ‘ÎÛd“üÅŠzè“ü—ÍŠz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEG00020");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F21A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEG00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F21B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEG00040");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F21C );
			pShLstVal->sign1 = Sn_2F21sw;
			pShLstVal->Update();

			// ‘İ“|‰ñû‚ÉŒW‚éÁ”ïÅŠz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEH00010");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F23A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEH00020");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F23B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEH00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F23C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();
		}
		else{
			// ‰ÛÅ”„ãŠz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ADB00020");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F1C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// –ÆÅ”„ãŠz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ADB00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F2C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// —Ao‘Y‰¿Šz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ADB00040");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F3C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ”ñ‰ÛÅ”„ãŠz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ADB00080");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F6C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ‰ÛÅd“ü‚ÉŒW‚éx•¥‘Î‰¿‚ÌŠz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ADC00010");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F8C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ‰ÛÅd“ü‚ÉŒW‚éÁ”ïÅŠz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ADC00020");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F9C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ‰ÛÅ‰İ•¨‚ÉŒW‚éx•¥‘Î‰¿‚ÌŠz

			// ‰ÛÅ‰È–Ú‚ÉŒW‚éÁ”ïÅŠz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ADC00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F11C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ŒÂ•Ê‘Î‰F‰ÛÅ”„ã‚ÉŒW‚é
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ADE00020");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F15C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ŒÂ•Ê‘Î‰F‹¤’Ê‚Ì”„ã‚ÉŒW‚é
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ADE00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F16C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// •t•\2¢17£-A ŒÂ•Ê‘Î‰TœÅŠzè“ü—ÍŠz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ADE00040");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F17C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// •t•\2¢18£-A ”ä—á”z•ªTœÅŠzè“ü—ÍŠz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ADE00050");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F18C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// TœÅŠz‚Ì’²®
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ADF00010");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F19C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// TœÅŠz‚Ì’²®
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ADF00020");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F20C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// •t•\2¢21£-A Tœ‘ÎÛd“üÅŠzè“ü—ÍŠz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ADG00010");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F21C );
			pShLstVal->sign1 = Sn_2F21sw;
			pShLstVal->Update();

			// ‘İ“|‰ñû‚ÉŒW‚éÁ”ïÅŠz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("ADH00000");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F23C );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();
		}
	}
	else{
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

				if( pShLstVal->itmId == _T("AEB00030") ){		// ‰ÛÅ”„ãŠz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F1A );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEB00040") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F1B );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEB00050") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F1C );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEB00070") ){	// –ÆÅ”„ãŠz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F2C );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEB00080") ){	// —Ao‘Y‰¿Šz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F3C );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEB00120") ){	// ”ñ‰ÛÅ”„ãŠz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F6C );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEC00020") ){	// ‰ÛÅd“ü‚ÉŒW‚éx•¥‘Î‰¿‚ÌŠz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F8A );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEC00030") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F8B );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEC00040") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F8C );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEC00070") ){	// ‰ÛÅd“ü‚ÉŒW‚éÁ”ïÅŠz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F9A );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEC00080") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F9B );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEC00090") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F9C );
					pShLstVal->Update();
				}
				// ‰ÛÅ‰İ•¨‚ÉŒW‚éx•¥‘Î‰¿‚ÌŠz
				else if( pShLstVal->itmId == _T("AEC00120") ){	// ‰ÛÅ‰İ•¨‚ÉŒW‚éÁ”ïÅŠz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F11A );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEC00130") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F11B );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEC00140") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F11C );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEE00030") ){	// ŒÂ•Ê‘Î‰F‰ÛÅ”„ã‚ÉŒW‚é
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F15A );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEE00040") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F15B );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEE00050") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F15C );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEE00080") ){	// ŒÂ•Ê‘Î‰F‹¤’Ê‚Ì”„ã‚ÉŒW‚é
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F16A );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEE00090") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F16B );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEE00100") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F16C );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEE00130") ){	// •t•\2¢16£- ŒÂ•Ê‘Î‰TœÅŠzè“ü—ÍŠz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F17A );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEE00140") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F17B );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEE00150") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F17C );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEE00180") ){	// •t•\2¢17£- ”ä—á”z•ªTœÅŠzè“ü—ÍŠz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F18A );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEE00190") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F18B );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEE00200") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F18C );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEF00020") ){	// TœÅŠz‚Ì’²®
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F19A );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEF00030") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F19B );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEF00040") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F19C );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEF00070") ){	// TœÅŠz‚Ì’²®
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F20A );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEF00080") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F20B );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEF00090") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F20C );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEG00020") ){	// •t•\2¢20£- Tœ‘ÎÛd“üÅŠzè“ü—ÍŠz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F21A );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEG00030") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F21B );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEG00040") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F21C );
					pShLstVal->sign1 = Sn_2F21sw;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEH00010") ){	// ‘İ“|‰ñû‚ÉŒW‚éÁ”ïÅŠz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F23A );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEH00020") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F23B );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("AEH00030") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F23C );
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

				if( pShLstVal->itmId == _T("ADB00020") ){		// ‰ÛÅ•W€Šz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F1C );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ADB00030") ){	// –ÆÅ”„ãŠz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F2C );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ADB00040") ){	// —Ao‘Y‰¿Šz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F3C );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ADB00080") ){	// ”ñ‰ÛÅ”„ãŠz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F6C );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ADC00010") ){	// ‰ÛÅd“ü‚ÉŒW‚éx•¥‘Î‰¿‚ÌŠz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F8C );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ADC00020") ){	// ‰ÛÅd“ü‚ÉŒW‚éÁ”ïÅŠz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F9C );
					pShLstVal->Update();
				}
				// ‰ÛÅ‰İ•¨‚ÉŒW‚éx•¥‘Î‰¿‚ÌŠz
				else if( pShLstVal->itmId == _T("ADC00030") ){	// ‰ÛÅ‰È–Ú‚ÉŒW‚éÁ”ïÅŠz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F11C );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ADE00020") ){	// ŒÂ•Ê‘Î‰F‰ÛÅ”„ã‚ÉŒW‚é
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F15C );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ADE00030") ){	// ŒÂ•Ê‘Î‰F‹¤’Ê‚Ì”„ã‚ÉŒW‚é
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F16C );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ADE00040") ){	// •t•\2¢17£-A ŒÂ•Ê‘Î‰TœÅŠzè“ü—ÍŠz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F17C );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ADE00050") ){	// •t•\2¢18£-A ”ä—á”z•ªTœÅŠzè“ü—ÍŠz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F18C );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ADF00010") ){	// TœÅŠz‚Ì’²®
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F19C );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ADF00020") ){	// TœÅŠz‚Ì’²®
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F20C );
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ADG00010") ){	// •t•\2¢21£-A Tœ‘ÎÛd“üÅŠzè“ü—ÍŠz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F21C );
					pShLstVal->sign1 = Sn_2F21sw;
					pShLstVal->Update();
				}
				else if( pShLstVal->itmId == _T("ADH00000") ){	// ‘İ“|‰ñû‚ÉŒW‚éÁ”ïÅŠz
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F23C );
					pShLstVal->Update();
				}
			}
		}
	}

	return 0;
}
#else
//-----------------------------------------------------------------------------
// DB ‚Öƒf[ƒ^‘‚İ
//-----------------------------------------------------------------------------
// ˆø”	pDBNpSub	F	à–±ƒNƒ‰ƒX‚Ìƒ|ƒCƒ“ƒ^
//		sn_seq		F	‘ÎÛ‚Ì—š—ğƒV[ƒPƒ“ƒX
//-----------------------------------------------------------------------------
// •Ô‘—’l	0		F	³íI—¹
//			-1		F	ƒGƒ‰[
//-----------------------------------------------------------------------------
int CH26SnFhyo20Data::SetData( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT( pDBNpSub );

	CString	filter;
	char	*pHyoID = NULL;
	int	nowVersion = 1;
	filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_2_2_HYOID, nowVersion );
	pHyoID = ICS_SN_FHYO_2_2_HYOID;

	if( m_DBSyzAccess.m_pSH_LST_VAL ){
		m_DBSyzAccess.m_pSH_LST_VAL->Requery( filter );
	}
	else{
		if( m_DBSyzAccess.OpenShLstVal(pDBNpSub->m_database, filter) ){
			return -1;
		}
	}

	// •s³ƒf[ƒ^íœƒtƒ‰ƒO('14.05.13)
	BOOL	WrongDataExFlg = FALSE;

	CSH_LST_VAL	*pShLstVal = m_DBSyzAccess.m_pSH_LST_VAL;

	if( (pShLstVal->MoveFirst()==-1) || (pShLstVal->st==-1) ){	// ƒf[ƒ^–³‚µ

//		if( m_isTransitionalMeasure ){
			// ‰ÛÅ”„ãŠz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEB00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F1A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEB00040");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F1B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			//------>yoshida140328
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEC00020");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F8A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEC00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F8B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();
			//<-------------------

			// ‰ÛÅd“ü‚ÉŒW‚éÁ”ïÅŠz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEC00070");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F9A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEC00080");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F9B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ‰ÛÅ‰İ•¨‚ÉŒW‚éÁ”ïÅŠz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEC00120");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F10A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEC00130");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F10B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ŒÂ•Ê‘Î‰F‰ÛÅ”„ã‚ÉŒW‚é
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEE00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F15A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEE00040");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F15B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ŒÂ•Ê‘Î‰F‹¤’Ê‚Ì”„ã‚ÉŒW‚é
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEE00080");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F16A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEE00090");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F16B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// •t•\2¢16£- ŒÂ•Ê‘Î‰TœÅŠzè“ü—ÍŠz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEE00130");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F17A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEE00140");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F17B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// •t•\2¢17£- ”ä—á”z•ªTœÅŠzè“ü—ÍŠz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEE00180");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F18A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEE00190");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F18B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// TœÅŠz‚Ì’²®
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEF00020");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F19A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEF00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F19B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// TœÅŠz‚Ì’²®
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEF00070");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F20A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEF00080");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F20B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// •t•\2¢20£- Tœ‘ÎÛd“üÅŠzè“ü—ÍŠz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEG00020");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F21A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEG00030");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F21B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			// ‘İ“|‰ñû‚ÉŒW‚éÁ”ïÅŠz
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEH00010");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F23A );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();

			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = pHyoID;
			pShLstVal->itmId = _T("AEH00020");
			m_Util.val_to_asci( &pShLstVal->val, Sn_2F23B );
			pShLstVal->sign1 = 0;
			pShLstVal->Update();
//		}

		// ‰ÛÅ”„ãŠz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AEB00050");
		m_Util.val_to_asci( &pShLstVal->val, Sn_2F1C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// –ÆÅ”„ãŠz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AEB00070");
		m_Util.val_to_asci( &pShLstVal->val, Sn_2F2C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// —Ao‘Y‰¿Šz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AEB00080");
		m_Util.val_to_asci( &pShLstVal->val, Sn_2F3C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ”ñ‰ÛÅ”„ãŠz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AEB00120");
		m_Util.val_to_asci( &pShLstVal->val, Sn_2F6C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ‰ÛÅd“ü‚ÉŒW‚éx•¥‘Î‰¿‚ÌŠz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AEC00040");//yoshida140328
//		pShLstVal->itmId = _T("ADC00010");
		m_Util.val_to_asci( &pShLstVal->val, Sn_2F8C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ‰ÛÅd“ü‚ÉŒW‚éÁ”ïÅŠz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AEC00090");
		m_Util.val_to_asci( &pShLstVal->val, Sn_2F9C );
/*- '14.05.17 -*/
		pShLstVal->sign1 = Sn_2F9sw;//[Y]
/*-------------*/
//		pShLstVal->sign1 = Sn_2F9sw&(~0x111);//[Y]
/*-------------*/
		pShLstVal->Update();

		// ‰ÛÅ‰İ•¨‚ÉŒW‚éx•¥‘Î‰¿‚ÌŠz


		// ‰ÛÅ‰İ•¨‚ÉŒW‚éÁ”ïÅŠz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
/*- '14.05.13 -*/
//		pShLstVal->itmId = _T("ADC00030");
/*-------------*/
		pShLstVal->itmId = _T("AEC00140");
/*-------------*/
		m_Util.val_to_asci( &pShLstVal->val, Sn_2F10C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ŒÂ•Ê‘Î‰F‰ÛÅ”„ã‚ÉŒW‚é
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AEE00050");
		m_Util.val_to_asci( &pShLstVal->val, Sn_2F15C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// ŒÂ•Ê‘Î‰F‹¤’Ê‚Ì”„ã‚ÉŒW‚é
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AEE00100");
		m_Util.val_to_asci( &pShLstVal->val, Sn_2F16C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// •t•\2¢16£- ŒÂ•Ê‘Î‰TœÅŠzè“ü—ÍŠz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AEE00150");
		m_Util.val_to_asci( &pShLstVal->val, Sn_2F17C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// •t•\2¢17£- ”ä—á”z•ªTœÅŠzè“ü—ÍŠz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AEE00200");
		m_Util.val_to_asci( &pShLstVal->val, Sn_2F18C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// TœÅŠz‚Ì’²®
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AEF00040");
		m_Util.val_to_asci( &pShLstVal->val, Sn_2F19C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// TœÅŠz‚Ì’²®
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AEF00090");
		m_Util.val_to_asci( &pShLstVal->val, Sn_2F20C );
		pShLstVal->sign1 = 0;
		pShLstVal->Update();

		// •t•\2¢20£- Tœ‘ÎÛd“üÅŠzè“ü—ÍŠz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AEG00040");
		m_Util.val_to_asci( &pShLstVal->val, Sn_2F21C );
		pShLstVal->sign1 = Sn_2F21sw;
		pShLstVal->Update();

		// ‘İ“|‰ñû‚ÉŒW‚éÁ”ïÅŠz
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = pHyoID;
		pShLstVal->itmId = _T("AEH00030");
		m_Util.val_to_asci( &pShLstVal->val, Sn_2F23C );
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

//			if( m_isTransitionalMeasure ){
				// ‰ÛÅ”„ãŠz
				if( pShLstVal->itmId == _T("AEB00030") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F1A );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AEB00040") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F1B );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AEC00020") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F8A );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AEC00030") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F8B );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// ‰ÛÅd“ü‚ÉŒW‚éÁ”ïÅŠz
				else if( pShLstVal->itmId == _T("AEC00070") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F9A );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AEC00080") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F9B );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// ‰ÛÅ‰İ•¨‚ÉŒW‚éx•¥‘Î‰¿‚ÌŠz
				// ‰ÛÅ‰İ•¨‚ÉŒW‚éÁ”ïÅŠz
				else if( pShLstVal->itmId == _T("AEC00120") ){	
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F10A );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AEC00130") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F10B );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// ŒÂ•Ê‘Î‰F‰ÛÅ”„ã‚ÉŒW‚é
				else if( pShLstVal->itmId == _T("AEE00030") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F15A );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AEE00040") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F15B );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// ŒÂ•Ê‘Î‰F‹¤’Ê‚Ì”„ã‚ÉŒW‚é
				else if( pShLstVal->itmId == _T("AEE00080") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F16A );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AEE00090") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F16B );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// •t•\2¢16£- ŒÂ•Ê‘Î‰TœÅŠzè“ü—ÍŠz
				else if( pShLstVal->itmId == _T("AEE00130") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F17A );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AEE00140") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F17B );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// •t•\2¢17£- ”ä—á”z•ªTœÅŠzè“ü—ÍŠz
				else if( pShLstVal->itmId == _T("AEE00180") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F18A );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AEE00190") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F18B );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// TœÅŠz‚Ì’²®
				else if( pShLstVal->itmId == _T("AEF00020") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F19A );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AEF00030") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F19B );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// TœÅŠz‚Ì’²®
				else if( pShLstVal->itmId == _T("AEF00070") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F20A );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AEF00080") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F20B );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// •t•\2¢20£- Tœ‘ÎÛd“üÅŠzè“ü—ÍŠz
				else if( pShLstVal->itmId == _T("AEG00020") ){	
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F21A );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AEG00030") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F21B );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				// ‘İ“|‰ñû‚ÉŒW‚éÁ”ïÅŠz
				else if( pShLstVal->itmId == _T("AEH00010") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F23A );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
				else if( pShLstVal->itmId == _T("AEH00020") ){
					pShLstVal->Edit();
					m_Util.val_to_asci( &pShLstVal->val, Sn_2F23B );
					pShLstVal->Update();
					updTagAry.Add( pShLstVal->itmId );
				}
//			}

			// ‰ÛÅ”„ãŠz
			if( pShLstVal->itmId == _T("AEB00050") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_2F1C );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			else if( pShLstVal->itmId == _T("AEB00070") ){	// –ÆÅ”„ãŠz
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_2F2C );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			else if( pShLstVal->itmId == _T("AEB00080") ){	// —Ao‘Y‰¿Šz
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_2F3C );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			else if( pShLstVal->itmId == _T("AEB00120") ){	// ”ñ‰ÛÅ”„ãŠz
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_2F6C );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			// ‰ÛÅd“ü‚ÉŒW‚éx•¥‘Î‰¿‚ÌŠz
			else if( pShLstVal->itmId == _T("AEC00040") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_2F8C );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			// ‰ÛÅd“ü‚ÉŒW‚éÁ”ïÅŠz
			else if( pShLstVal->itmId == _T("AEC00090") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_2F9C );
/*- '14.05.17 -*/
				pShLstVal->sign1 = Sn_2F9sw;//[Y]
/*-------------*/
//				pShLstVal->sign1 = Sn_2F9sw&(~0x111);//[Y]
/*-------------*/
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			// ‰ÛÅ‰İ•¨‚ÉŒW‚éx•¥‘Î‰¿‚ÌŠz
			// ‰ÛÅ‰İ•¨‚ÉŒW‚éÁ”ïÅŠz
			else if( pShLstVal->itmId == _T("AEC00140") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_2F10C );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			// ŒÂ•Ê‘Î‰F‰ÛÅ”„ã‚ÉŒW‚é
			else if( pShLstVal->itmId == _T("AEE00050") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_2F15C );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			// ŒÂ•Ê‘Î‰F‹¤’Ê‚Ì”„ã‚ÉŒW‚é
			else if( pShLstVal->itmId == _T("AEE00100") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_2F16C );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			// •t•\2¢16£- ŒÂ•Ê‘Î‰TœÅŠzè“ü—ÍŠz
			else if( pShLstVal->itmId == _T("AEE00150") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_2F17C );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			// •t•\2¢17£- ”ä—á”z•ªTœÅŠzè“ü—ÍŠz
			else if( pShLstVal->itmId == _T("AEE00200") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_2F18C );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			// TœÅŠz‚Ì’²®
			else if( pShLstVal->itmId == _T("AEF00040") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_2F19C );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			// TœÅŠz‚Ì’²®
			else if( pShLstVal->itmId == _T("AEF00090") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_2F20C );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			// •t•\2¢20£- Tœ‘ÎÛd“üÅŠzè“ü—ÍŠz
			else if( pShLstVal->itmId == _T("AEG00040") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_2F21C );
				pShLstVal->sign1 = Sn_2F21sw;
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
			// ‘İ“|‰ñû‚ÉŒW‚éÁ”ïÅŠz
			else if( pShLstVal->itmId == _T("AEH00030") ){
				pShLstVal->Edit();
				m_Util.val_to_asci( &pShLstVal->val, Sn_2F23C );
				pShLstVal->Update();
				updTagAry.Add( pShLstVal->itmId );
			}
//--> '14.05.14 INS START
			else if( pShLstVal->itmId == _T("ADC00030") ){
				WrongDataExFlg = TRUE;
			}
//<-- '14.05.14 INS END
		}

		// XVƒf[ƒ^Šm”F
		int	adCnt = 0;
		for( int i=0;; i++ ){
			if( strlen(tagData20[i].tagName) == 0 ){
				break;
			}
			//if( m_isTransitionalMeasure== FALSE ){
			//	if( (tagData20[i].sign1!=0) && (tagData20[i].sign1!=1) ){
			//		adCnt++;
			//	}
			//}
			//else{
				adCnt++;
			//}
		}
		int updCnt = (int)updTagAry.GetCount();
		if( adCnt != updCnt ){
			// –³‚¢‚à‚Ì‚ğ’Ç‰Á
			CArray<unsigned char*>	MonyPtrAry;
			MonyPtrAry.Add( Sn_2F1A );
			MonyPtrAry.Add( Sn_2F1B );
			MonyPtrAry.Add( Sn_2F1C );
			MonyPtrAry.Add( Sn_2F2C );
			MonyPtrAry.Add( Sn_2F3C );
			MonyPtrAry.Add( Sn_2F6C );
			MonyPtrAry.Add( Sn_2F8A );
			MonyPtrAry.Add( Sn_2F8B );
			MonyPtrAry.Add( Sn_2F8C );
			MonyPtrAry.Add( Sn_2F9A );
			MonyPtrAry.Add( Sn_2F9B );
			MonyPtrAry.Add( Sn_2F9C );
			MonyPtrAry.Add( Sn_2F10A );
			MonyPtrAry.Add( Sn_2F10B );
			MonyPtrAry.Add( Sn_2F10C );
			MonyPtrAry.Add( Sn_2F15A );
			MonyPtrAry.Add( Sn_2F15B );
			MonyPtrAry.Add( Sn_2F15C );
			MonyPtrAry.Add( Sn_2F16A );
			MonyPtrAry.Add( Sn_2F16B );
			MonyPtrAry.Add( Sn_2F16C );
			MonyPtrAry.Add( Sn_2F17A );
			MonyPtrAry.Add( Sn_2F17B );
			MonyPtrAry.Add( Sn_2F17C );
			MonyPtrAry.Add( Sn_2F18A );
			MonyPtrAry.Add( Sn_2F18B );
			MonyPtrAry.Add( Sn_2F18C );
			MonyPtrAry.Add( Sn_2F19A );
			MonyPtrAry.Add( Sn_2F19B );
			MonyPtrAry.Add( Sn_2F19C );
			MonyPtrAry.Add( Sn_2F20A );
			MonyPtrAry.Add( Sn_2F20B );
			MonyPtrAry.Add( Sn_2F20C );
			MonyPtrAry.Add( Sn_2F21A );
			MonyPtrAry.Add( Sn_2F21B );
			MonyPtrAry.Add( Sn_2F21C );
			MonyPtrAry.Add( Sn_2F23A );
			MonyPtrAry.Add( Sn_2F23B );
			MonyPtrAry.Add( Sn_2F23C );
			int	aryMax = (int)MonyPtrAry.GetCount();

			BOOL findFlg = FALSE;
			for( int i=0;; i++ ){
				if( strlen(tagData20[i].tagName) == 0 ){
					break;
				}
				//if( m_isTransitionalMeasure == FALSE ){
				//	if( (tagData20[i].sign1==0) || (tagData20[i].sign1==1) ){
				//		continue;
				//	}
				//}

				findFlg = FALSE;
				for( int j=0; j<updCnt; j++ ){
					if( updTagAry[j].CompareNoCase(tagData20[i].tagName) == 0 ){
						findFlg = TRUE;
						break;
					}
				}
				if( findFlg == FALSE ){
					// ŠY“–ƒ^ƒO‚Ì’Ç‰Á
					if( i < aryMax ){
						pShLstVal->AddNew();
						pShLstVal->sn_seq = sn_seq;
						pShLstVal->version = nowVersion;
						pShLstVal->hyoId = pHyoID;
						pShLstVal->itmId = tagData20[i].tagName;
						m_Util.val_to_asci( &pShLstVal->val, MonyPtrAry[i] );
						if( tagData20[i].tagName == _T("AEG00040") ){
							pShLstVal->sign1 = Sn_2F21sw;
						}
						else{
							pShLstVal->sign1 = 0;
						}
						pShLstVal->Update();
					}
				}
			}
		}
	}

//--> '14.05.14 INS START
	if( WrongDataExFlg ){
		filter.Format( _T("delete from dbo.SYZ_SNK_LST_VAL where sn_seq = %d and hyoId = \'%s\' and itmId = \'ADC00030\' and version = %d"), sn_seq, ICS_SN_FHYO_2_2_HYOID, nowVersion );

		pDBNpSub->m_database->BeginTrans();
		try{
			pDBNpSub->m_database->ExecuteSQL( filter );
		}
		catch( CDBException *pde ){
			pDBNpSub->m_database->Rollback();
			pde->Delete();
			return -1;
		}
		pDBNpSub->m_database->CommitTrans();
	}
//<-- '14.05.14 INS END

	filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_2_2_HYOID, nowVersion );

	if( m_DBSyzAccess.m_pSH_LST_RATE ){
		m_DBSyzAccess.m_pSH_LST_RATE->Requery( filter );
	}
	else{
		if( m_DBSyzAccess.OpenShLstRate(pDBNpSub->m_database, filter) ){
			return -1;
		}
	}

	CSH_LST_RATE	*pShLstRate = m_DBSyzAccess.m_pSH_LST_RATE;
	
//	int	nowVersion = 1;
	if( (pShLstRate->MoveFirst()==-1) || (pShLstRate->st==-1) ){	// ƒf[ƒ^–³‚µ
		// ‰ÛÅ”„ãŠ„‡
		pShLstRate->AddNew();
		pShLstRate->sn_seq = sn_seq;
		pShLstRate->version = nowVersion;
		pShLstRate->hyoId = pHyoID;
		pShLstRate->itmId = _T("AEB00140");
		pShLstRate->rate = Sn_2FUWS;
		pShLstRate->sign1 = 0;
		pShLstRate->Update();

		// ‰ÛÅ”„ãŠ„‡ è“ü—Í
		pShLstRate->AddNew();
		pShLstRate->sn_seq = sn_seq;
		pShLstRate->version = nowVersion;
		pShLstRate->hyoId = pHyoID;
		pShLstRate->itmId = _T("AEB00141");
		pShLstRate->rate = Sn_2FUWSEX;
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

			// ‰ÛÅ”„ãŠ„‡
			if( pShLstRate->itmId == _T("AEB00140") ){
				pShLstRate->Edit();
				pShLstRate->rate = Sn_2FUWS;
				pShLstRate->Update();
			}
			// ‰ÛÅ”„ãŠ„‡ è“ü—Í
			else if( pShLstRate->itmId == _T("AEB00141") ){
				pShLstRate->Edit();
				pShLstRate->rate = Sn_2FUWSEX;
				pShLstRate->Update();
			}
		}
	}

	return 0;
}
#endif

//-----------------------------------------------------------------------------
// WŒvÏ‚İƒf[ƒ^‚ÌƒZƒbƒg
//-----------------------------------------------------------------------------
// ˆø”	pSyzSyukei	F	Á”ïÅWŒvƒNƒ‰ƒX
//		isPluralZei	F	•¡”Å—¦
//-----------------------------------------------------------------------------
// •Ô‘—’l	0		F	³íI—¹
//			-1		F	ƒGƒ‰[
//-----------------------------------------------------------------------------
int CH26SnFhyo20Data::SetCalqedData( CSyzSyukei *pSyzSyukei, BOOL &isPluralZei )
{
	MoneyBasejagArray money;

	money = pSyzSyukei->GetShinkokuData( _T("12") );

	int cnt = (int)money.GetCount();
	int incnt = 0;

	unsigned	char	exZeiflg = 0x00;	//       ¡¡¡
											//  D0F3%
											//  D1F5%
											//  D2F8%

	for( int i=0; i<cnt; i++ ){
		incnt = (int)money[i].GetCount();
		for( int j=0; j<incnt; j++ ){
			if( money[i][j].code.Left(4) == _T("1201") ){
				if( money[i][j].code == _T("120101") ){
					memmove( Sn_2F1A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("120102") ){
					memmove( Sn_2F1B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("120103") ){
					memmove( Sn_2F1C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}
			else if( money[i][j].code.Left(4) == _T("1202") ){
				memmove( Sn_2F2C, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if( money[i][j].code.Left(4) == _T("1203") ){
				memmove( Sn_2F3C, money[i][j].arith, MONY_BUF_SIZE );
			}
			else if( money[i][j].code.Left(4) == _T("1206") ){
				if( money[i][j].code == _T("120600") ){
					memmove( Sn_2F6C, money[i][j].arith, MONY_BUF_SIZE );
				}
			}
			else if( money[i][j].code.Left(4) == _T("1208") ){
				if( money[i][j].code == _T("120801") ){
					memmove( Sn_2F8A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("120802") ){
					memmove( Sn_2F8B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("120803") ){
					memmove( Sn_2F8C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}
			else if( money[i][j].code.Left(4) == _T("1209") ){
				if( money[i][j].code == _T("120901") ){
					memmove( Sn_2F9A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("120902") ){
					memmove( Sn_2F9B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("120903") ){
					memmove( Sn_2F9C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}
			else if( money[i][j].code.Left(4) == _T("1210") ){
				if( money[i][j].code == _T("121001") ){
					memmove( Sn_2F10A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("121002") ){
					memmove( Sn_2F10B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("121003") ){
					memmove( Sn_2F10C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}
			else if( money[i][j].code.Left(4) == _T("1211") ){
/*				if( money[i][j].code == _T("121101") ){
					memmove( Sn_2F11A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("121102") ){
					memmove( Sn_2F11B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("121103") ){
					memmove( Sn_2F11C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}*/
			}
			else if( money[i][j].code.Left(4) == _T("1214") ){
				if( money[i][j].code == _T("121401") ){
					memmove( Sn_2F15A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("121402") ){
					memmove( Sn_2F15B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("121403") ){
					memmove( Sn_2F15C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}
			else if( money[i][j].code.Left(4) == _T("1215") ){
				if( money[i][j].code == _T("121501") ){
					memmove( Sn_2F16A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("121502") ){
					memmove( Sn_2F16B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("121503") ){
					memmove( Sn_2F16C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
					}
				}
			}
/*- '14.02.23 -*/
// ‚±‚Ì€–Ú‚ÍŒvZ‚Å‚Í‹‚ß‚ç‚ê‚È‚¢I
//			else if( money[i][j].code.Left(4) == _T("1218") ){
//				if( money[i][j].code == _T("121801") ){
//					memmove( Sn_2F19A, money[i][j].arith, MONY_BUF_SIZE );
//					if( m_Arith.l_test(money[i][j].arith) ){
//						exZeiflg |= 0x01;
//					}
//				}
//				else if( money[i][j].code == _T("121802") ){
//					memmove( Sn_2F19B, money[i][j].arith, MONY_BUF_SIZE );
//					if( m_Arith.l_test(money[i][j].arith) ){
//						exZeiflg |= 0x02;
//					}
//				}
//				else if( money[i][j].code == _T("121803") ){
//					memmove( Sn_2F19C, money[i][j].arith, MONY_BUF_SIZE );
//					if( m_Arith.l_test(money[i][j].arith) ){
//						exZeiflg |= 0x04;
//					}
//				}
//			}
//			else if( money[i][j].code.Left(4) == _T("1219") ){
//				if( money[i][j].code == _T("121901") ){
//					memmove( Sn_2F20A, money[i][j].arith, MONY_BUF_SIZE );
//					if( m_Arith.l_test(money[i][j].arith) ){
//						exZeiflg |= 0x01;
//					}
//				}
//				else if( money[i][j].code == _T("121902") ){
//					memmove( Sn_2F20B, money[i][j].arith, MONY_BUF_SIZE );
//					if( m_Arith.l_test(money[i][j].arith) ){
//						exZeiflg |= 0x02;
//					}
//				}
//				else if( money[i][j].code == _T("121903") ){
//					memmove( Sn_2F20C, money[i][j].arith, MONY_BUF_SIZE );
//					if( m_Arith.l_test(money[i][j].arith) ){
//						exZeiflg |= 0x04;
//					}
//				}
//			}
/*-------------*/
			else if( money[i][j].code.Left(4) == _T("1222") ){
				if( money[i][j].code == _T("122201") ){
					memmove( Sn_2F23A, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x01;
					}
				}
				else if( money[i][j].code == _T("122202") ){
					memmove( Sn_2F23B, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x02;
					}
				}
				else if( money[i][j].code == _T("122203") ){
					memmove( Sn_2F23C, money[i][j].arith, MONY_BUF_SIZE );
					if( m_Arith.l_test(money[i][j].arith) ){
						exZeiflg |= 0x04;
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
// Œ»sƒf[ƒ^\‘¢‚©‚ç‚Ìƒf[ƒ^‚ÌˆøŒp‚¬
//-----------------------------------------------------------------------------
// ˆø”	pDBNpSub	F	à–±ƒNƒ‰ƒX
//		sn_seq		F	—š—ğƒV[ƒPƒ“ƒX
//-----------------------------------------------------------------------------
int CH26SnFhyo20Data::GetPastData( CDBNpSub *pDBNpSub, int sn_seq )
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
		// –³‚¢H
	}

	char	*pHyoID = NULL;
	int		nowVersion = 1;
	filter.Format( _T("sn_seq = %d and hyoId = \'%s\' and version = %d"), sn_seq, ICS_SN_FHYO_2_2_HYOID, nowVersion );
	pHyoID = ICS_SN_FHYO_2_2_HYOID;

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
		if( pShLstVal->itmId == _T("AEB00030") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FURIT;
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AEB00040") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FURI[0];
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AEC00020") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FUZT[1];
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AEC00030") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FUZT[2];
			pShLstVal->Update();
		}
		// ‰ÛÅd“ü‚ÉŒW‚éÁ”ïÅŠz
		else if( pShLstVal->itmId == _T("AEC00070") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FUZT[3];//yoshida140328
//			pShLstVal->val = pSZSNK->Sn_5FURI[3];
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AEC00080") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FUZT[4];
			pShLstVal->Update();
		}
		// ‰ÛÅ‰İ•¨‚ÉŒW‚éx•¥‘Î‰¿‚ÌŠz
		// ‰ÛÅ‰İ•¨‚ÉŒW‚éÁ”ïÅŠz
		else if( pShLstVal->itmId == _T("AEC00120") ){	
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FUZK[1];//yoshida140328
//			pShLstVal->val = pSZSNK->Sn_5FUZKT;
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AEC00130") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FUZK[2];//yoshida140328
//			pShLstVal->val = pSZSNK->Sn_5FUZK[0];
			pShLstVal->Update();
		}
		// ŒÂ•Ê‘Î‰F‰ÛÅ”„ã‚ÉŒW‚é
		else if( pShLstVal->itmId == _T("AEE00030") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FUZK[3];
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AEE00040") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FUZK[4];
			pShLstVal->Update();
		}
		// ŒÂ•Ê‘Î‰F‹¤’Ê‚Ì”„ã‚ÉŒW‚é
		else if( pShLstVal->itmId == _T("AEE00080") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FSZTG;
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AEE00090") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FSZTT;
			pShLstVal->Update();
		}
		// •t•\2¢16£- ŒÂ•Ê‘Î‰TœÅŠzè“ü—ÍŠz
		else if( pShLstVal->itmId == _T("AEE00130") ){
			pShLstVal->Edit();
//			pShLstVal->val = pSZSNK->Sn_2F17A;
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AEE00140") ){
			pShLstVal->Edit();
//			pShLstVal->val = pSZSNK->Sn_2F17B;
			pShLstVal->Update();
		}
		// •t•\2¢17£- ”ä—á”z•ªTœÅŠzè“ü—ÍŠz
		else if( pShLstVal->itmId == _T("AEE00180") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_2F18A;
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AEE00190") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_2F18B;
			pShLstVal->Update();
		}
		// TœÅŠz‚Ì’²®
		else if( pShLstVal->itmId == _T("AEF00020") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FSZKG;
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AEF00030") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FSZKT;
			pShLstVal->Update();
		}
		// TœÅŠz‚Ì’²®
		else if( pShLstVal->itmId == _T("AEF00070") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5Fdm1;
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AEF00080") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5Fdm2;
			pShLstVal->Update();
		}
		// •t•\2¢20£- Tœ‘ÎÛd“üÅŠzè“ü—ÍŠz
		else if( pShLstVal->itmId == _T("AEG00020") ){	
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_2F21A;
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AEG00030") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_2F21B;
			pShLstVal->Update();
		}
		// ‘İ“|‰ñû‚ÉŒW‚éÁ”ïÅŠz
		else if( pShLstVal->itmId == _T("AEH00010") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5Fdm3;
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AEH00020") ){
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5Fdm4;
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AEB00070") ){	// –ÆÅ”„ãŠz
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FURI[2];
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AEB00080") ){	// —Ao‘Y‰¿Šz
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FURI[4];
			pShLstVal->Update();
		}
		else if( pShLstVal->itmId == _T("AEB00120") ){	// ”ñ‰ÛÅ”„ãŠz
			pShLstVal->Edit();
			pShLstVal->val = pSZSNK->Sn_5FUZT[0];
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
		if( pShLstRate->itmId == _T("AEB00140") ){
			pShLstRate->Edit();
			pShLstRate->rate = pSZSNK->Sn_5FUP[0];
			pShLstRate->Update();
		}
		else if( pShLstRate->itmId == _T("AEB00141") ){
			pShLstRate->Edit();
			pShLstRate->rate = pSZSNK->Sn_5FUP[1];
			pShLstRate->Update();
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// XV‚Ì‹àŠzƒNƒŠƒA
//-----------------------------------------------------------------------------
// ˆø”	pDBNpSub	F	
//		type		F
//		ClearType	F@D0 ONEEè“ü—Í‹àŠz‚ğƒNƒŠƒA‚µ‚È‚¢
//-----------------------------------------------------------------------------
// •Ô‘—’l	0		F	
//-----------------------------------------------------------------------------
int CH26SnFhyo20Data::ClearDataByRenew( CDBNpSub *pDBNpSub, int type , int ClearType)//yoshida140422
//int CH26SnFhyo20Data::ClearDataByRenew( CDBNpSub *pDBNpSub, int type )
{
	m_Arith.l_clear( Sn_2F1A );
	m_Arith.l_clear( Sn_2F1B );
	m_Arith.l_clear( Sn_2F1C );

	m_Arith.l_clear( Sn_2F2A );
	m_Arith.l_clear( Sn_2F2B );
	m_Arith.l_clear( Sn_2F2C );

	m_Arith.l_clear( Sn_2F3A );
	m_Arith.l_clear( Sn_2F3B );
	m_Arith.l_clear( Sn_2F3C );

	m_Arith.l_clear( Sn_2F6A );
	m_Arith.l_clear( Sn_2F6B );
	m_Arith.l_clear( Sn_2F6C );

	m_Arith.l_clear( Sn_2F8A );
	m_Arith.l_clear( Sn_2F8B );
	m_Arith.l_clear( Sn_2F8C );

	m_Arith.l_clear( Sn_2F9A );
	m_Arith.l_clear( Sn_2F9B );
	m_Arith.l_clear( Sn_2F9C );
	Sn_2F9sw = 0;

	m_Arith.l_clear( Sn_2F10A );
	m_Arith.l_clear( Sn_2F10B );
	m_Arith.l_clear( Sn_2F10C );

	m_Arith.l_clear( Sn_2F11A );
	m_Arith.l_clear( Sn_2F11B );
	m_Arith.l_clear( Sn_2F11C );

	m_Arith.l_clear( Sn_2F15A );
	m_Arith.l_clear( Sn_2F15B );
	m_Arith.l_clear( Sn_2F15C );

	m_Arith.l_clear( Sn_2F16A );
	m_Arith.l_clear( Sn_2F16B );
	m_Arith.l_clear( Sn_2F16C );

	m_Arith.l_clear( Sn_2F17A );
	m_Arith.l_clear( Sn_2F17B );
	m_Arith.l_clear( Sn_2F17C );

	m_Arith.l_clear( Sn_2F18A );
	m_Arith.l_clear( Sn_2F18B );
	m_Arith.l_clear( Sn_2F18C );

	//yoshida140422
	//è“ü—Í‹àŠz‚ğƒNƒŠƒA‚µ‚È‚¢ƒpƒ^[ƒ“‚ğ’Ç‰Á
	if((ClearType & 0x01) == 0x00){
		m_Arith.l_clear( Sn_2F19A );
		m_Arith.l_clear( Sn_2F19B );
		m_Arith.l_clear( Sn_2F19C );

		m_Arith.l_clear( Sn_2F20A );
		m_Arith.l_clear( Sn_2F20B );
		m_Arith.l_clear( Sn_2F20C );
	}

	m_Arith.l_clear( Sn_2F21A );
	m_Arith.l_clear( Sn_2F21B );
	m_Arith.l_clear( Sn_2F21C );
	Sn_2F21sw = 0;

	m_Arith.l_clear( Sn_2F23A );
	m_Arith.l_clear( Sn_2F23B );
	m_Arith.l_clear( Sn_2F23C );
	Sn_2FUWS = 0;
	Sn_2FUWSEX = 0;

	return 0;
}

//-----------------------------------------------------------------------------
// è“ü—ÍƒTƒCƒ“‚ÌƒNƒŠƒA
//-----------------------------------------------------------------------------
// •Ô‘—’l	0	F	³íI—¹
//-----------------------------------------------------------------------------
int CH26SnFhyo20Data::ClearManualInputSign( BOOL isChgKzritu )
{
	Sn_2F9sw = 0;
	if( isChgKzritu ){
		Sn_2F9sw |= 0x111;
	}
	Sn_2F21sw = 0;

	//---->yoshida140328
	// •t•\2¢16£- ŒÂ•Ê‘Î‰TœÅŠzè“ü—ÍŠz
	// •t•\2¢17£- ”ä—á”z•ªTœÅŠzè“ü—ÍŠz
	// ã‹L‚Q‚Â‚Í‹àŠz‚Ì—L‚è–³‚µ‚Åè“ü—Í‚©”»’f‚µ‚Ä‚¢‚é‚Ì‚Å‹àŠz‚ğƒNƒŠƒA‚·‚éiƒCƒR[ƒ‹è“ü—ÍƒTƒCƒ“‚ğƒNƒŠƒA‚µ‚Ä‚¢‚é‚±‚Æ‚É‚È‚éj
	memset( Sn_2F17A, '\0', sizeof(Sn_2F17A) );
	memset( Sn_2F17B, '\0', sizeof(Sn_2F17B) );
	memset( Sn_2F17C, '\0', sizeof(Sn_2F17C) );

	memset( Sn_2F18A, '\0', sizeof(Sn_2F18A) );
	memset( Sn_2F18B, '\0', sizeof(Sn_2F18B) );
	memset( Sn_2F18C, '\0', sizeof(Sn_2F18C) );
	//<--------------------------


	return 0;
}
