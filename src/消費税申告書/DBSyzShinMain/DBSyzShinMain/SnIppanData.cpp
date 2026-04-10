#include "StdAfx.h"
#include "SnIppanData.h"

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
// ˆø”	pDBNpSub	F	à–±ƒNƒ‰ƒX‚Ìƒ|ƒCƒ“ƒ^
//		sn_seq		F	‘ÎÛ‚Ì—š—ðƒV[ƒPƒ“ƒX
//-----------------------------------------------------------------------------
void CSnIppanData::SetData( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT( pDBNpSub );

	CString	filter;
	filter.Format( _T("sn_seq = %d and hyoId = SHA010"), sn_seq );

	if( m_DBSyzAccess.m_pSH_LST_VAL ){
		m_DBSyzAccess.m_pSH_LST_VAL->Requery( filter );
	}
	else{
		m_DBSyzAccess.OpenShLstVal( pDBNpSub->m_database, filter );
	}

	// 
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

		// ‹àŠz‚ð•ÏŠ·‚µ‚Ä‘ã“ü
		if( pShLstVal->itmId == _T("AAB00010") ){		// ‰ÛÅ•W€Šz
			m_Util.val_to_bin( Sn_KSTD, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AAB00020") ){	// Á”ïÅŠz
			m_Util.val_to_bin( Sn_SYTX, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AAB00030") ){	// Tœ‰ß‘å’²®ÅŠz
			m_Util.val_to_bin( Sn_KJADJZ, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AAB00050") ){	// Tœ‘ÎÛŽd“üÅŠz
			m_Util.val_to_bin( Sn_SIREZ, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AAB00060") ){	// •ÔŠÒ“™‘Î‰¿‚ÉŒW‚éÅŠz
			m_Util.val_to_bin( Sn_HKANZ, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AAB00070") ){	// ‘Ý“|‚ê‚ÉŒW‚éÅŠz
			m_Util.val_to_bin( Sn_KSITZ, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AAB00110") ){	// ’†ŠÔ”[•tÅŠz
			m_Util.val_to_bin( Sn_TYNOFZ, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AAB00120") ){	// ”[•tÅŠz
			m_Util.val_to_bin( Sn_EDNOFZ, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AAB00150") ){	// ŠùŠm’èÅŠz
			m_Util.val_to_bin( Sn_KAKTIZ, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AAB00180") ){	// ‰ÛÅŽ‘ŽY‚Ì÷“n“™‚Ì‘Î‰¿‚ÌŠz
			m_Util.val_to_bin( Sn_KZURI, pShLstVal->val );
		}
		else if( pShLstVal->itmId == _T("AAB00190") ){	// Ž‘ŽY‚Ì÷“n“™‚Ì‘Î‰¿‚ÌŠz
			m_Util.val_to_bin( Sn_SOURI, pShLstVal->val );
		}
	}

}

