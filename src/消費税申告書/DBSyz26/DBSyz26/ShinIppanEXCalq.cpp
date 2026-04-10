
#include "stdafx.h"
#include "ShinIppanEX.h"
#include "ShinIppanEXIdx.h"

//-----------------------------------------------------------------------------
// ŒvŽZ
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ‘¼•\‚©‚ç‚ÌŒvŽZ
//-----------------------------------------------------------------------------
// ˆø”	ksw	F	ŠÈˆÕŒvŽZƒXƒCƒbƒ`
//-----------------------------------------------------------------------------
void CShinIppanEX::calq_by_OtherChange( int ksw, BOOL IsFromTkkz/*=FALSE*/ )
{
	m_DpSw = -1;
	m_kanisw = ksw;
	ReadData();
//-- '15.08.21 --
	if( IsFromTkkz ){
		if( !(m_M1c&0x08) ){
			m_M1c |= 0x01;
			calqx_all( 1, 1 );
			m_M1c &= ~0x01;
		}
		else{
			calqx_all( 6, 1 );
		}
	}
	else{
		calqx_all( 6, 1 );
	}
//---------------
	WriteData();
	m_DpSw = 0;

	// naka Q : m_kanisw‚Í–ß‚³‚È‚­‚Ä‚à—Ç‚¢H
}

//-----------------------------------------------------------------------------
// Še“ü—Í‚ÌŒvŽZ•\Ž¦
//-----------------------------------------------------------------------------
// ˆø”	atno	F	ŠJŽnŒvŽZˆÊ’u
//		sw		F	ŒvŽZƒXƒCƒbƒ`
//-----------------------------------------------------------------------------
void CShinIppanEX::calqx_all( int atno, int sw )
{
	while( 1 ){
		switch( atno ){
			case	1 :
				// ‚PD‰ÛÅ•W€Šz
				calqx_1();
			case	2 :
				// ‚QDÁ”ïÅŠz
				calqx_2();
			case	3 :
			case	4 :
			case	5 :
			case	6 :
				// ‚VDTœÅŠz¬Œv
				calqx_7( sw );
				// ‚WDTœ•s‘«ŠÒ•tÅŠzE‚XD·ˆøÅ
				calqx_89( sw );
			case	7 :
				// 10. ’†ŠÔ”[•tÅŠz
				if( !sw ){
					calqx_10();
				}
				// 11. ”[•tÅŠz ¥ 12. ’†ŠÔ”[•tŠÒ•tÅŠz
				calqx_1112( sw );
			case	8 :
				// 14. ·ˆø”[•tÅŠz
				calqx_14( sw );
			case	10 :
				// 17. ’n•ûÅETœ•s‘«ŠÒ•tÅŠz
				calqx_17( sw );
				// 18. ’n•ûÅE·ˆøÅŠz
				calqx_18( sw );
				// 19. ’n•ûÅE÷“nŠ„ŠzŠÒ•tŠzE20. ”[ÅŠz
				calqx_1920( sw );
			case	11 :
				// 21. ’†ŠÔ”[•t÷“nŠ„Šz
				if( !sw ){
					calqx_21();
				}
				// 22. ”[•t÷“nŠ„ŠzE23. ’†ŠÔ÷“nŠ„Šz
				calqx_2223( sw );
			case	12 :
				// 25. ·ˆø”[•t÷“nŠ„Šz
				calqx_25( sw );
				// 26. ‡ŒvÅŠz
				calqx_26( sw );
			default	:
				break;
		}
		break;
	}

	//‰E‘¤Ä•\Ž¦’Ç‰Áshimizu
	if( (sw == 0) && (!(*m_pSnHeadData)->SVmzsw == 1 )){
		char Mny[MONY_BUF_SIZE]={0};
		if( m_pArith->l_test( XM17 ) > 0){
			memmove( Mny , XM17 , MONY_BUF_SIZE );
//--> '14.06.03 INS START
			m_pArith->l_neg( Mny );
//<-- '14.06.03 INS END
		}
		else{
			memmove( Mny , XM18 , MONY_BUF_SIZE );
		}
		dsp_prs( IDC_ICSDIAGCTRL3, ID4SC2, Mny );
	}
}

//-----------------------------------------------------------------------------
// ‚PD‰ÛÅ•W€Šz
//-----------------------------------------------------------------------------
void CShinIppanEX::calqx_1()
{
	char	WORK0[MONY_BUF_SIZE] = {0};

	// ‚P‚O‚O‚OˆÈ‰º‚ÌØ‚èŽÌ‚Ä
	m_Util.l_calq( XM1, XM1, 0 );
	dsp_prs( IDC_ICSDIAGCTRL2, ID3XM1, XM1 );
	if( !(*m_pSnHeadData)->SVmzsw ){
		// ‰ÛÅ•W€Šz
		memset( WORK0, '\0', MONY_BUF_SIZE );
		m_Util.l_calq( WORK0, XM1, 0 );
		dsp_prs( IDC_ICSDIAGCTRL3, ID4KM3, WORK0 );
	}
}

//-----------------------------------------------------------------------------
// ‚QDÁ”ïÅŠz
//-----------------------------------------------------------------------------
void CShinIppanEX::calqx_2()
{

	if( (m_M1c&0x01) && !(m_M1c&0x08) ){//[Y]
//	if( m_M1c || (m_pArith->l_test(XM2)==0) ){
		ChangeColor( IDC_ICSDIAGCTRL2, ID3XM2, 0 );
		memset( XM2, '\0', MONY_BUF_SIZE );
//		m_Util.percent( XM2, XM1, 1, 40, 0 );
		m_Util.percent( XM2, XM1, 1, 63, 0 );
		m_M1c = 0;
	}
	dsp_prs( IDC_ICSDIAGCTRL2, ID3XM2, XM2 );
	if( !(*m_pSnHeadData)->SVmzsw ){
		// ‰ÛÅ•W€Šz
		dsp_prs( IDC_ICSDIAGCTRL3, ID4SM3, XM2 );
	}

	//[Y]
	if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
		if( !(m_M1c&0x01) && (m_M1c&0x08) ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID3XM2, 3 );	// 2
		}
	}

}

//-----------------------------------------------------------------------------
// ‚VDTœÅŠz¬Œv
//-----------------------------------------------------------------------------
// ˆø”	sw	F	ŒvŽZƒXƒCƒbƒ`
//-----------------------------------------------------------------------------
void CShinIppanEX::calqx_7( int sw )
{
	m_pArith->l_add( XM7, XM4, XM5 );
	m_pArith->l_add( XM7, XM7, XM6 );
	if( !sw ) {
		dsp_cls( IDC_ICSDIAGCTRL2, ID3XM7 );
		dsp_prs( IDC_ICSDIAGCTRL2, ID3XM7, XM7 );
	}
}

//-----------------------------------------------------------------------------
// ‚WDTœ•s‘«ŠÒ•tÅŠzE‚XD·ˆøÅŠzi‚Q{‚R|‚Vj
//-----------------------------------------------------------------------------
// ˆø”	sw	F	ŒvŽZƒXƒCƒbƒ`
//-----------------------------------------------------------------------------
void CShinIppanEX::calqx_89( int sw )
{
	memset( XM8, '\0', MONY_BUF_SIZE );
	memset( XM9, '\0', MONY_BUF_SIZE );
	if( (*m_pSnHeadData)->SVmzsw ){
//		if( m_pArith->l_test( pSyzShin->Snd.Sn_1F14C ) < 0 ){
		if( m_pArith->l_test((*m_pSnFhyo10Data)->Sn_1F10T) < 0 ){
			if( ((*m_pSnHeadData)->Sn_SKKBN&0xff) % 2 ){	// ’†ŠÔ\ˆÈŠO
				memmove( XM8, (*m_pSnFhyo10Data)->Sn_1F10T, MONY_BUF_SIZE );
				m_pArith->l_neg( XM8 );
			}
			// Tœ•s‘«ŠÒ•tÅŠz‚Ì•\Ž¦
			if( !sw ){
				dsp_prs( IDC_ICSDIAGCTRL2, ID3XM8, XM8 );	
			}
			// ŒÀŠETœ‘OÅŠz‚ÌÁ‹Ž
			memset( XM9, '\0', MONY_BUF_SIZE );
			if( !sw ){
				dsp_prs( IDC_ICSDIAGCTRL2, ID3XM9, XM9 );
			}
		}
		else{
			if( m_pArith->l_test( (*m_pSnHonpyoData)->Sn_GENKAI ) ){
				// –³‚¢‚Í‚¸HHH
//				memmove( XM9, pSyzShin->Snd.Sn_3F3, MONY_BUF_SIZE );
			}
			else{
				memmove( XM9, (*m_pSnFhyo10Data)->Sn_1F10T, MONY_BUF_SIZE );
			}
			// ‚P‚O‚OˆÈ‰º‚ÌØ‚èŽÌ‚Ä
			m_Util.l_calq( XM9, XM9, 0x10 );
			// ŒÀŠETœ‘O‚ÌÅŠz
			if( !sw ){
				dsp_prs( IDC_ICSDIAGCTRL2, ID3XM9, XM9 );
				// Tœ•s‘«ŠÒ•tÅŠz‚ÌÁ‹Ž
				memset( XM8, '\0', MONY_BUF_SIZE );
				dsp_prs( IDC_ICSDIAGCTRL2, ID3XM8, XM8 );
			}
		}
	}
	else{
		m_pArith->l_add( XM9, XM2, XM3 );
		m_pArith->l_sub( XM9, XM9, XM7 );

		// ŒÀŠETœ‘O‚ÌÅŠz‚ªƒ}ƒCƒiƒX‚ÌŽž
		if( m_pArith->l_test(XM9) < 0 ){
			if( ((*m_pSnHeadData)->Sn_SKKBN&0xff) % 2 ){	// ’†ŠÔ\ˆÈŠO
				m_pArith->l_neg( XM9 );
				memmove( XM8, XM9, MONY_BUF_SIZE );
			}
			// Tœ•s‘«ŠÒ•tÅŠz‚Ì•\Ž¦
			if( !sw ){
				dsp_prs( IDC_ICSDIAGCTRL2, ID3XM8, XM8 );
			}
			// ŒÀŠETœ‘OÅŠz‚ÌÁ‹Ž
			memset( XM9, '\0', MONY_BUF_SIZE );
			if( !sw ){
				dsp_prs( IDC_ICSDIAGCTRL2, ID3XM9, XM9 );
			}
		}
		else {	
			// ‚P‚O‚OˆÈ‰º‚ÌØ‚èŽÌ‚Ä
			m_Util.l_calq( XM9, XM9, 0x10 );
			if( !sw ) {
				// ŒÀŠETœ‘O‚ÌÅŠz
				dsp_prs( IDC_ICSDIAGCTRL2, ID3XM9, XM9 );
				// Tœ•s‘«ŠÒ•tÅŠz‚ÌÁ‹Ž
				memset( XM8, '\0', MONY_BUF_SIZE );
				dsp_prs( IDC_ICSDIAGCTRL2, ID3XM8, XM8 );
			}
		}
	}
}

//-----------------------------------------------------------------------------
// ‚P‚OD’†ŠÔ”[•tÅŠz
//-----------------------------------------------------------------------------
void CShinIppanEX::calqx_10()
{
	// ‚P‚O‚OˆÈ‰º‚ÌØ‚èŽÌ‚Ä
	m_Util.l_calq( XM10, XM10, 0x10 );
	dsp_prs( IDC_ICSDIAGCTRL2, ID3XM10, XM10);
}

//-----------------------------------------------------------------------------
// ‚P‚PD”[•tÅŠzi‚X|‚P‚OjE‚P‚QD’†ŠÔ”[•tŠÒ•t
//-----------------------------------------------------------------------------
// ˆø”	sw	F	ŒvŽZƒXƒCƒbƒ`
//-----------------------------------------------------------------------------
void CShinIppanEX::calqx_1112( int sw )
{
	memset( XM11, '\0', MONY_BUF_SIZE );
	memset( XM12, '\0', MONY_BUF_SIZE );
	m_pArith->l_sub( XM11, XM9, XM10 );
	if( m_pArith->l_test(XM11) < 0 ){
		m_pArith->l_neg( XM11 );
		memmove( XM12, XM11, MONY_BUF_SIZE );
		memset( XM11, '\0', MONY_BUF_SIZE );
		// ‚P‚O‚OˆÈ‰º‚ÌØ‚èŽÌ‚Ä
		m_Util.l_calq( XM12, XM12, 0x10 );
		if( !sw ){
			// ’†ŠÔ”[•tÅŠz‚Ì•\Ž¦
			dsp_prs( IDC_ICSDIAGCTRL2, ID3XM12, XM12 );
			// ‚P‚PD”[•tÅŠzi‚X|‚P‚Oj‚ÌÁ‹Ž
			memset( XM11, '\0', MONY_BUF_SIZE );
			dsp_prs( IDC_ICSDIAGCTRL2, ID3XM11, XM11 );
		}
	}
	else {
		// ‚P‚O‚OˆÈ‰º‚ÌØ‚èŽÌ‚Ä
		m_Util.l_calq( XM11, XM11, 0x10 );
		if( !sw ){
			// ‚P‚PD”[•tÅŠzi‚X|‚P‚Oj‚Ì•\Ž¦
			dsp_prs( IDC_ICSDIAGCTRL2, ID3XM11, XM11 );
			// ’†ŠÔ”[•tÅŠz‚ÌÁ‹Ž
			memset( XM12, '\0', MONY_BUF_SIZE );
			dsp_prs( IDC_ICSDIAGCTRL2, ID3XM12, XM12 );
		}
	}
}

//-----------------------------------------------------------------------------
// ‚P‚SD·ˆø‚«”[•tÅŠz
//-----------------------------------------------------------------------------
// ˆø”		sw		F	ŒvŽZƒXƒCƒbƒ`
//-----------------------------------------------------------------------------
// •Ô‘—’l	TRUE	F	ŒvŽZŠ®—¹
//			FALSE	F	ŒvŽZ‚¹‚¸
//-----------------------------------------------------------------------------
int CShinIppanEX::calqx_14( int sw )
{
	switch( (*m_pSnHeadData)->Sn_SKKBN&0xff ){
		case	1 :		// Šm’èE’†ŠÔ
		case	2 :
			return FALSE;
		default   :		// Šm’èC³E’†ŠÔC³
			break;
	}	
	memset( XM14, '\0', MONY_BUF_SIZE );
	if( m_pArith->l_test( XM11 ) > 0 ){				// ”[•tÅŠz‚ª—L‚éê‡
		m_pArith->l_sub( XM14, XM11, XM13 );
	}
	else{
		memmove( XM14, XM12, MONY_BUF_SIZE );
		m_pArith->l_neg( XM14 );
		m_pArith->l_sub( XM14, XM14, XM13 );
	}
	m_pArith->l_sub( XM14, XM14, XM8 );
	// ‚P‚O‚OˆÈ‰º‚ÌØ‚èŽÌ‚Ä
	m_Util.l_calq( XM14, XM14, 0x10 );
	// ·ˆø‚«”[•tÅŠzE•\Ž¦
	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID3XM14, XM14 );
	}
	return TRUE;
}

//-----------------------------------------------------------------------------
// ‚P‚VDTœ•s‘«ŠÒ•tÅŠzi’n•ûÁ”ïÅj
//-----------------------------------------------------------------------------
// ˆø”	sw	F	ŒvŽZƒXƒCƒbƒ`
//-----------------------------------------------------------------------------
void CShinIppanEX::calqx_17( int sw )
{
	memset( XM17, '\0', MONY_BUF_SIZE );
	// ’†ŠÔŠŽ‰ü³“ú‘OŠJŽn‚Í’n•ûÁ”ïÅ“ü—Í–³‚µ
//	if( !(((*m_pSnHeadData)->Sn_SKKBN&0xff) % 2) && pSyzShin->N_ver != 2 ){
	if( 0 ){
		;
	}
	else{
		if( ((*m_pSnHeadData)->Sn_SKKBN&0xff) % 2 ){	// ’†ŠÔ\ˆÈŠO
			if( (*m_pSnHeadData)->SVmzsw ){
					char	WORK0[MONY_BUF_SIZE]={0};
					char	WORK1[MONY_BUF_SIZE]={0};
					char	WORK2[MONY_BUF_SIZE]={0};
					m_pArith->l_add( WORK0 , (*m_pSnFhyo10Data)->Sn_1F8B , (*m_pSnFhyo10Data)->Sn_1F8C );//8
					m_pArith->l_add( WORK1 , (*m_pSnFhyo10Data)->Sn_1F9B , (*m_pSnFhyo10Data)->Sn_1F9C );//9
					m_pArith->l_sub( WORK2 , WORK1 , WORK0 );//9-8

					if( m_pArith->l_test(WORK2) < 0 ){
						memmove( XM17 , WORK2 , MONY_BUF_SIZE );
						m_pArith->l_neg( XM17 );
					}
				//Tœ•s‘«ŠÒ•tÅŠz‚É‚Íƒ}ƒCƒiƒX‚Ìê‡‚Ì‚Ý•\Ž¦‚·‚é
	//			if( m_pArith->l_test((*m_pSnFhyo10Data)->Sn_1F10T) < 0 ){
	//				memmove( XM17, (*m_pSnFhyo10Data)->Sn_1F10T, MONY_BUF_SIZE );
	//				m_pArith->l_neg( XM17 );
	//			}
			}
			else{
				memmove( XM17, XM8, MONY_BUF_SIZE );
			}
			// Tœ•s‘«ŠÒ•tÅŠz‚Ì•\Ž¦
			if( !sw ){
				dsp_prs( IDC_ICSDIAGCTRL2, ID3XM17, XM17 );
			}
		}
	}
}

//-----------------------------------------------------------------------------
// ‚P‚WD·ˆøÅŠzi’n•ûÁ”ïÅj
//-----------------------------------------------------------------------------
// ˆø”	sw	F	ŒvŽZƒXƒCƒbƒ`
//-----------------------------------------------------------------------------
void CShinIppanEX::calqx_18( int sw )
{
	memset( XM18, '\0', MONY_BUF_SIZE );
	// ’†ŠÔŠŽ‰ü³“ú‘OŠJŽn‚Í’n•ûÁ”ïÅ“ü—Í–³‚µ
//	if( ! (((*m_pSnHeadData)->Sn_SKKBN&0xff) % 2) && pSyzShin->N_ver != 2 ){
	if( 0 ){
		;
	}
	else {
		char	WVAL[MONY_BUF_SIZE] = {0};
		if( (*m_pSnHeadData)->SVmzsw ){
			memmove( WVAL, (*m_pSnFhyo10Data)->Sn_1F10T, MONY_BUF_SIZE );  
		}
		else{
			memmove( WVAL, XM11, MONY_BUF_SIZE );
		}
//		if( (m_pArith->l_test(WVAL)<0) && !(((*m_pSnHeadData)->Sn_SKKBN&0xff) % 2) ){
//			;
//		}
//		else{		// ŠÒ•t’†ŠÔ\ ˆÈŠO
			if( (*m_pSnHeadData)->SVmzsw ){
				char	WORK0[MONY_BUF_SIZE]={0};
				char	WORK1[MONY_BUF_SIZE]={0};
				char	WORK2[MONY_BUF_SIZE]={0};
				m_pArith->l_add( WORK0 , (*m_pSnFhyo10Data)->Sn_1F8B , (*m_pSnFhyo10Data)->Sn_1F8C );//8
				m_pArith->l_add( WORK1 , (*m_pSnFhyo10Data)->Sn_1F9B , (*m_pSnFhyo10Data)->Sn_1F9C );//9
				m_pArith->l_sub( WORK2 , WORK1 , WORK0 );//9-8

				if( m_pArith->l_test(WORK2) > 0 ){
					memmove( XM18 , WORK2 , MONY_BUF_SIZE );
				}
/*				if( m_pArith->l_test((*m_pSnHonpyoData)->Sn_GENKAI) ){
					// —L‚è“¾‚È‚¢‚Í‚¸III
//					memmove( XM18, pSyzShin->Snd.Sn_3F6, MONY_BUF_SIZE );
				}
				else{
//					memmove( XM18, pSyzShin->Snd.Sn_1F13B, MONY_BUF_SIZE );
					//·ˆøÅŠz‚É‚Íƒvƒ‰ƒX‚Ìê‡‚Ì‚Ý•\Ž¦‚·‚é
					if( m_pArith->l_test((*m_pSnFhyo10Data)->Sn_1F10T) >= 0 ){
						memmove( XM18, (*m_pSnFhyo10Data)->Sn_1F10T, MONY_BUF_SIZE );
					}
					
				}
*/			}
			else{
				memmove( XM18, XM9, MONY_BUF_SIZE );
			}
			// ‚P‚O‚OˆÈ‰º‚ÌØ‚èŽÌ‚Ä
			m_Util.l_calq( XM18, XM18, 0x10 );
			// Tœ•s‘«ŠÒ•tÅŠz‚Ì•\Ž¦
			if( !sw ){
				dsp_prs( IDC_ICSDIAGCTRL2, ID3XM18, XM18 );
			}
		}
//	}
}

//-----------------------------------------------------------------------------
// ‚P‚XD÷“nŠ„ŠzŠÒ•tŠz ¥ ‚Q‚OD÷“nŠ„Šz”[ÅŠzi’n•ûÁ”ïÅj
//-----------------------------------------------------------------------------
// ˆø”	sw	F	ŒvŽZƒXƒCƒbƒ`
//-----------------------------------------------------------------------------
void CShinIppanEX::calqx_1920( int sw )
{
	char	WORK0[MONY_BUF_SIZE] = {0};

	memset( XM19,  '\0', MONY_BUF_SIZE );
	memset( XM20, '\0', MONY_BUF_SIZE );

	// ’†ŠÔŠŽ‰ü³“ú‘OŠJŽn‚Í’n•ûÁ”ïÅ“ü—Í–³‚µ
//	if( ! (((*m_pSnHeadData)->Sn_SKKBN&0xff) % 2) && pSyzShin->N_ver != 2 ){
/*	if( 0 ){
		;
	}
	else{
		memset( WORK0, '\0', MONY_BUF_SIZE );

		m_pArith->l_input( WORK0, "250" );
		if( !sw ){
			dsp_prs( IDC_ICSDIAGCTRL2, ID3XM19, XM19 );
		}
		if( m_pArith->l_test(XM17) ){
			m_pArith->l_mul100( (unsigned char *)XM19, (unsigned char *)XM17, (unsigned char *)WORK0, 1, 0 );
			if( !sw ){
				dsp_prs( IDC_ICSDIAGCTRL2, ID3XM19, XM19 );
			}
		}
		if( !sw ){
			dsp_prs( IDC_ICSDIAGCTRL2, ID3XM20, XM20 );
		}
		if( m_pArith->l_test( XM18 ) ){
			m_pArith->l_mul100( (unsigned char *)XM20, (unsigned char *)XM18, (unsigned char *)WORK0, 1, 0 );
			// ‚P‚O‚OˆÈ‰º‚ÌØ‚èŽÌ‚Ä
			m_Util.l_calq( XM20, XM20, 0x10 );
			if( ! sw ){
				dsp_prs( IDC_ICSDIAGCTRL2, ID3XM20, XM20 );
			}
		}
	}*/

	if( 1 ){
		char	WVAL[MONY_BUF_SIZE] = {0};
		if( (*m_pSnHeadData)->SVmzsw ){
			memmove( WVAL, (*m_pSnFhyo10Data)->Sn_1F10T, MONY_BUF_SIZE );  
		}
		else{
			memmove( WVAL, XM11, MONY_BUF_SIZE );
		}

/*- '14.04.30 -*/
		if( (!m_pArith->l_test(XM17)) && (!m_pArith->l_test(XM18)) && !(((*m_pSnHeadData)->Sn_SKKBN&0xff) % 2) ){
			;
		}
		else{
			memset( WORK0, '\0', MONY_BUF_SIZE );

			m_pArith->l_input( WORK0, "250" );
			if( !sw ){
				dsp_prs( IDC_ICSDIAGCTRL2, ID3XM19, XM19 );
			}
			if( !sw ){
				dsp_prs( IDC_ICSDIAGCTRL2, ID3XM20, XM20 );
			}

			if( (*m_pSnHeadData)->SVmzsw ){
				if( m_pArith->l_test((*m_pSnHonpyoData)->Sn_GENKAI) ){
					// —L‚è“¾‚È‚¢‚Í‚¸III
//					memmove( XM18, pSyzShin->Snd.Sn_3F6, MONY_BUF_SIZE );
				}
				else{
//					memmove( XM18, pSyzShin->Snd.Sn_1F13B, MONY_BUF_SIZE );
					char	WORK0[MONY_BUF_SIZE]={0};
					char	WORK1[MONY_BUF_SIZE]={0};
					char	WORK2[MONY_BUF_SIZE]={0};
					char	WORK3[MONY_BUF_SIZE]={0};
					char	WORK4[MONY_BUF_SIZE]={0};
					m_Util.percent( WORK0, (char*)(*m_pSnFhyo10Data)->Sn_1F8B, 7, 25, 0 );
					m_Util.percent( WORK1, (char*)(*m_pSnFhyo10Data)->Sn_1F9B, 7, 25, 0 );
					m_Util.percent( WORK2, (char*)(*m_pSnFhyo10Data)->Sn_1F8C, 8, 17, 0 );
					m_Util.percent( WORK3, (char*)(*m_pSnFhyo10Data)->Sn_1F9C, 8, 17, 0 );
					m_pArith->l_add( WORK4, WORK1, WORK3 );
					m_pArith->l_sub( WORK4, WORK4, WORK0 );//shimizu
					m_pArith->l_sub( WORK4, WORK4, WORK2 );//shimizu
//					if( m_pArith->l_test(XM17) ){
//						m_pArith->l_add( XM19, WORK0, WORK2 );//shimizu
					if( m_pArith->l_test(WORK4) > 0 ){//15-14‚ªƒvƒ‰ƒX‚Ì‚Æ‚«‚Í20‚Ö
						memmove( XM20 , WORK4 , MONY_BUF_SIZE );
						// ‚P‚O‚OˆÈ‰º‚ÌØ‚èŽÌ‚Ä
						m_Util.l_calq( XM20, XM20, 0x10 );
						if( !sw ){
							dsp_prs( IDC_ICSDIAGCTRL2, ID3XM20, XM20 );
						}
					}
					else{
						m_pArith->l_neg( WORK4 );
						memmove( XM19 , WORK4 , MONY_BUF_SIZE );
						if( ! sw ){
							dsp_prs( IDC_ICSDIAGCTRL2, ID3XM19, XM19 );
						}
					}
/*
					if( m_pArith->l_test(XM18) ){
						m_pArith->l_add( XM20, WORK1, WORK3 );
						m_pArith->l_sub( XM20, XM20, WORK0 );//shimizu
						m_pArith->l_sub( XM20, XM20, WORK2 );//shimizu
						// ‚P‚O‚OˆÈ‰º‚ÌØ‚èŽÌ‚Ä
						m_Util.l_calq( XM20, XM20, 0x10 );
						if( ! sw ){
							dsp_prs( IDC_ICSDIAGCTRL2, ID3XM20, XM20 );
						}
					}
*/				}
			}
			else{
				memset( WORK0, '\0', MONY_BUF_SIZE );

//				m_pArith->l_input( WORK0, "250" );
				if( !sw ){
					dsp_prs( IDC_ICSDIAGCTRL2, ID3XM19, XM19 );
				}
				if( m_pArith->l_test(XM17) ){
					m_Util.percent( XM19, XM17, 8, 17, 0 );
					if( !sw ){
						dsp_prs( IDC_ICSDIAGCTRL2, ID3XM19, XM19 );
					}
				}
				if( !sw ){
					dsp_prs( IDC_ICSDIAGCTRL2, ID3XM20, XM20 );
				}
				if( m_pArith->l_test( XM18 ) ){
					m_Util.percent( XM20, XM18, 8, 17, 0 );
					// ‚P‚O‚OˆÈ‰º‚ÌØ‚èŽÌ‚Ä
					m_Util.l_calq( XM20, XM20, 0x10 );
					if( ! sw ){
						dsp_prs( IDC_ICSDIAGCTRL2, ID3XM20, XM20 );
					}
				}
			}
		}
/*-------------*/
		if( (*m_pSnHeadData)->SVmzsw ){
			memset( WORK0, '\0', MONY_BUF_SIZE );

			m_pArith->l_input( WORK0, "250" );
			if( !sw ){
				dsp_prs( IDC_ICSDIAGCTRL2, ID3XM19, XM19 );
			}
			if( !sw ){
				dsp_prs( IDC_ICSDIAGCTRL2, ID3XM20, XM20 );
			}

			char	WORK0[MONY_BUF_SIZE]={0};
			char	WORK1[MONY_BUF_SIZE]={0};
			char	WORK2[MONY_BUF_SIZE]={0};
			char	WORK3[MONY_BUF_SIZE]={0};
			char	WORK4[MONY_BUF_SIZE]={0};
			m_Util.percent( WORK0, (char*)(*m_pSnFhyo10Data)->Sn_1F8B, 7, 25, 0 );
			m_Util.percent( WORK1, (char*)(*m_pSnFhyo10Data)->Sn_1F9B, 7, 25, 0 );
			m_Util.percent( WORK2, (char*)(*m_pSnFhyo10Data)->Sn_1F8C, 8, 17, 0 );
			m_Util.percent( WORK3, (char*)(*m_pSnFhyo10Data)->Sn_1F9C, 8, 17, 0 );
			m_pArith->l_add( WORK4, WORK1, WORK3 );
			m_pArith->l_sub( WORK4, WORK4, WORK0 );//shimizu
			m_pArith->l_sub( WORK4, WORK4, WORK2 );//shimizu
//			if( m_pArith->l_test(XM17) ){
//				m_pArith->l_add( XM19, WORK0, WORK2 );//shimizu
			if( m_pArith->l_test(WORK4) > 0 ){//15-14‚ªƒvƒ‰ƒX‚Ì‚Æ‚«‚Í20‚Ö
				memmove( XM20 , WORK4 , MONY_BUF_SIZE );
				// ‚P‚O‚OˆÈ‰º‚ÌØ‚èŽÌ‚Ä
				m_Util.l_calq( XM20, XM20, 0x10 );
				if( !sw ){
					dsp_prs( IDC_ICSDIAGCTRL2, ID3XM20, XM20 );
				}
			}
			else{
				if( ((*m_pSnHeadData)->Sn_SKKBN&0xff) % 2 ){
					m_pArith->l_neg( WORK4 );
					memmove( XM19 , WORK4 , MONY_BUF_SIZE );
					if( ! sw ){
						dsp_prs( IDC_ICSDIAGCTRL2, ID3XM19, XM19 );
					}
				}
			}
		}
		else{
			if( (!m_pArith->l_test(XM17)) && (!m_pArith->l_test(XM18)) && !(((*m_pSnHeadData)->Sn_SKKBN&0xff) % 2) ){
				;
			}
			else{
				memset( WORK0, '\0', MONY_BUF_SIZE );

				m_pArith->l_input( WORK0, "250" );
				if( !sw ){
					dsp_prs( IDC_ICSDIAGCTRL2, ID3XM19, XM19 );
				}
				if( !sw ){
					dsp_prs( IDC_ICSDIAGCTRL2, ID3XM20, XM20 );
				}

				memset( WORK0, '\0', MONY_BUF_SIZE );

//				m_pArith->l_input( WORK0, "250" );
				if( !sw ){
					dsp_prs( IDC_ICSDIAGCTRL2, ID3XM19, XM19 );
				}
				if( m_pArith->l_test(XM17) ){
					m_Util.percent( XM19, XM17, 8, 17, 0 );
					if( !sw ){
						dsp_prs( IDC_ICSDIAGCTRL2, ID3XM19, XM19 );
					}
				}
				if( !sw ){
					dsp_prs( IDC_ICSDIAGCTRL2, ID3XM20, XM20 );
				}
				if( m_pArith->l_test( XM18 ) ){
					m_Util.percent( XM20, XM18, 8, 17, 0 );
					// ‚P‚O‚OˆÈ‰º‚ÌØ‚èŽÌ‚Ä
					m_Util.l_calq( XM20, XM20, 0x10 );
					if( ! sw ){
						dsp_prs( IDC_ICSDIAGCTRL2, ID3XM20, XM20 );
					}
				}
			}
		}
/*-------------*/
	}
}

//-----------------------------------------------------------------------------
// ‚Q‚PD’†ŠÔ”[•t÷“nŠ„Šz
//-----------------------------------------------------------------------------
void CShinIppanEX::calqx_21()
{
	// ’†ŠÔŠŽ‰ü³“ú‘OŠJŽn‚Í’n•ûÁ”ïÅ“ü—Í–³‚µ
//	if( ! (((*m_pSnHeadData)->Sn_SKKBN&0xff) % 2) && pSyzShin->N_ver != 2 ){
	if( 0 ){
		;
	}
	else{
		// ‚P‚O‚OˆÈ‰º‚ÌØ‚èŽÌ‚Ä
		m_Util.l_calq( XM21, XM21, 0x10 );
		dsp_prs( IDC_ICSDIAGCTRL2, ID3XM21, XM21 );
	}
}

//-----------------------------------------------------------------------------
// ‚Q‚QD”[•t÷“nŠ„Šzi‚Q‚O|‚Q‚PjE‚Q‚RD’†ŠÔŠÒ•t÷“nŠ„Šz
//-----------------------------------------------------------------------------
// ˆø”	sw	F	ŒvŽZƒXƒCƒbƒ`
//-----------------------------------------------------------------------------
void CShinIppanEX::calqx_2223( int sw )
{
	memset( XM22, '\0', MONY_BUF_SIZE );
	memset( XM23, '\0', MONY_BUF_SIZE );

	// ’†ŠÔŠŽ‰ü³“ú‘OŠJŽn‚Í’n•ûÁ”ïÅ“ü—Í–³‚µ
//	if( ! (((*m_pSnHeadData)->Sn_SKKBN&0xff) % 2) && pSyzShin->N_ver != 2 ){
	if( 0 ){
		;
	}
	else{
		m_pArith->l_sub( XM22, XM20, XM21 );
		if( m_pArith->l_test(XM22) < 0 ){
			if( !(((*m_pSnHeadData)->Sn_SKKBN&0xff) % 2) ){	// ’†ŠÔ\
				memset( XM22, '\0', MONY_BUF_SIZE );
				if( !sw ){
					dsp_prs( IDC_ICSDIAGCTRL2, ID3XM23, XM23 );
				}
			}
			else{
				m_pArith->l_neg( XM22 );
				memmove( XM23, XM22, MONY_BUF_SIZE );
				memset( XM22, '\0', MONY_BUF_SIZE );
				// ‚P‚O‚OˆÈ‰º‚ÌØ‚èŽÌ‚Ä
				m_Util.l_calq( XM23, XM23, 0x10 );
				// ’†ŠÔŠÒ•t÷“nŠ„Šz‚Ì•\Ž¦
				if( !sw ){
					dsp_prs( IDC_ICSDIAGCTRL2, ID3XM23, XM23 );
				}
			}
			// ‚Q‚QD”[•tÅŠzi‚Q‚O|‚Q‚Pj‚ÌÁ‹Ž
			if( !sw ){
				dsp_prs( IDC_ICSDIAGCTRL2, ID3XM22, XM22 );
			}
		}
		else{
			// ‚P‚O‚OˆÈ‰º‚ÌØ‚èŽÌ‚Ä
			m_Util.l_calq( XM22, XM22, 0x10 );
			if( !sw ){
				// ‚Q‚QD”[•tÅŠzi‚Q‚O|‚Q‚Pj‚Ì•\Ž¦
				dsp_prs( IDC_ICSDIAGCTRL2, ID3XM22, XM22 );
				// ’†ŠÔ”[•tÅŠz‚ÌÁ‹Ž
				dsp_prs( IDC_ICSDIAGCTRL2, ID3XM23, XM23 );
			}
		}
	}
}

//-----------------------------------------------------------------------------
// ‚Q‚TD·ˆø”[•t÷“nŠ„Šz
//-----------------------------------------------------------------------------
// ˆø”	sw	F	ŒvŽZƒXƒCƒbƒ`
//-----------------------------------------------------------------------------
// •Ô‘—’l	
//-----------------------------------------------------------------------------
int CShinIppanEX::calqx_25( int sw )
{
	// ’†ŠÔŠŽ‰ü³“ú‘OŠJŽn‚Í’n•ûÁ”ïÅ“ü—Í–³‚µ
//	if( ! (((*m_pSnHeadData)->Sn_SKKBN&0xff) % 2) && pSyzShin->N_ver != 2 ){
	if( 0 ){
		;
	}
	else{
		switch( (*m_pSnHeadData)->Sn_SKKBN&0xff ){
			case	1 :		// Šm’èE’†ŠÔ
			case	2 :
				return FALSE;
			default   :		// Šm’èC³E’†ŠÔC³
				break;
		}
		memset( XM25, '\0', MONY_BUF_SIZE );
		if ( m_pArith->l_test( XM22 ) > 0 ){				// ”[•tÅŠz‚ª—L‚éê‡
			memmove( XM25, XM22, MONY_BUF_SIZE );
		}
		else{
			m_pArith->l_add( XM25, XM19, XM23 );
			if( m_pArith->l_test( XM25 ) ){
				m_pArith->l_neg( XM25 );
			}
		}
		m_pArith->l_sub( XM25, XM25, XM24 );
		// ‚P‚O‚OˆÈ‰º‚ÌØ‚èŽÌ‚Ä
		m_Util.l_calq( XM25, XM25, 0x10 );
		// ·ˆø‚«”[•tÅŠzE•\Ž¦
		if( ((*m_pSnHeadData)->Sn_SKKBN&0xff) == 2 ){	// ’†ŠÔ\
			if( m_pArith->l_test( XM25 ) < 0 ){
				memset( XM25, '\0', MONY_BUF_SIZE );
			}
		}
		if( !sw ){
			dsp_prs( IDC_ICSDIAGCTRL2, ID3XM25, XM25 );
		}
	}
	return TRUE;
}

//-----------------------------------------------------------------------------
// ‚Q‚UDÁ”ïÅ‹y‚Ñ’n•ûÁ”ïÅ‚Ì‡Œvi”[•t–”‚ÍŠÒ•tjÅŠz
//-----------------------------------------------------------------------------
void CShinIppanEX::calqx_26( int sw )
{
	char	W0[MONY_BUF_SIZE] = {0};
	memset( XM26, '\0', MONY_BUF_SIZE );
	memset( W0, '\0', MONY_BUF_SIZE );

	switch( (*m_pSnHeadData)->Sn_SKKBN&0xff ){
		case 1 :
		case 2 :		// Šm’èE’†ŠÔ\
			m_pArith->l_add( XM26, XM11, XM22 );
			m_pArith->l_add( W0, XM8, XM12 );
			m_pArith->l_add( W0, W0, XM19 );
			m_pArith->l_add( W0, W0, XM23 );
			m_pArith->l_sub( XM26, XM26, W0 );
			if( ! (((*m_pSnHeadData)->Sn_SKKBN&0xff) % 2) ) {	// ’†ŠÔ\
				if( m_pArith->l_test( XM26 ) < 0 ){
					memset( XM26, '\0', MONY_BUF_SIZE );
				}
			}
			break;
		default	:				// C³\
			m_pArith->l_add( XM26, XM14, XM25 );
			break;
	}
	if( !sw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID3XM26, XM26 );
	}
}
