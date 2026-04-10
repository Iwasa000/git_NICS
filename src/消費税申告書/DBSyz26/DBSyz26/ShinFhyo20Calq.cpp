
#include "stdafx.h"
#include "ShinFhyo20.h"
#include "ShinFhyo20Idx.h"
#include "H26HyoView.h"		// '15.04.03

//-----------------------------------------------------------------------------
// ‰ÛÅŽ‘ŽY‚Ì÷“n“™‚Ì‘Î‰¿‚ÌŠz
//-----------------------------------------------------------------------------
void CShinFhyo20::f2calq_45()
{
	m_pArith->l_add( F24C, F21C, F22C );
	m_pArith->l_add( F24C, F24C, F23C );

//2016.06.22 UPDATE START
//	dsp_prs( IDC_ICSDIAGCTRL2, ID24A, F24C );
	if( m_pArith->l_test( F24C ) < 0 ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID24A, FZERO );
	}else{
		dsp_prs( IDC_ICSDIAGCTRL2, ID24A, F24C );
	}
//2016.06.22 UPDATE END
	dsp_prs( IDC_ICSDIAGCTRL2, ID25A, F24C );
}

//-----------------------------------------------------------------------------
// Ž‘ŽY‚Ì÷“n“™‚Ì‘Î‰¿‚ÌŠz ¥ ‰ÛÅ”„ãŠ„‡
//-----------------------------------------------------------------------------
void CShinFhyo20::f2calq_7()
{
	char				WORK0[MONY_BUF_SIZE] = {0};
	char				PW6[MONY_BUF_SIZE] = {0};
	char				buf[512] = {0};
	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

//--> '15.02.06 INS START
	char	PER100BUF[MONY_BUF_SIZE] = {0};
	m_pArith->l_input( PER100BUF, _T("10000") );
//<-- '15.02.06 INS END

	// Ž‘ŽY‚Ì÷“n“™‚Ì‘Î‰¿‚ÌŠz
	m_pArith->l_add( F27C, F24C, F26C );
//2016.06.22 UPDATE START
//	dsp_prs( IDC_ICSDIAGCTRL2, ID27A, F27C );	
	if( m_pArith->l_test( F27C ) < 0 ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID27A, FZERO );
	}else{
		dsp_prs( IDC_ICSDIAGCTRL2, ID27A, F27C );
	}
//2016.06.22 UPDATE END

	///‰ÛÅ”„ã‚Š„‡
	DiagData.data_check = 0;
	diag_setdata( IDC_ICSDIAGCTRL2, ID27EX, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	ChangeColor( IDC_ICSDIAGCTRL2, ID2EX, 1 );
	diag_modify( IDC_ICSDIAGCTRL2, ID2EX, DIAG_MDFY_READONLY, CIcsdiagctrl );
	memset( buf, '\0', sizeof( buf ) );
	memset( WORK0, '\0', MONY_BUF_SIZE );
	if( !((*m_pSnHeadData)->Sn_EXP100&0x01) ){
//-- '15.04.04 --
//		if( m_pShinInfo->sgSpc ){
//---------------
		if( *m_pShinInfo->pSgSpc ){
//---------------
			// “Á’èŽû“üˆ—‚ ‚è [08'11.13]
			m_pArith->l_par100( (unsigned char *)WORK0, (unsigned char *)F24C, (unsigned char *)F27C, 2, 0 );
			// ”„ãŠ„‡‚ªƒ}ƒCƒiƒX‚Ìê‡‚Í‚O“‚ÅŒvŽZ‚·‚éB[03'05.17]
			memset( PW6, '\0', sizeof(PW6) );
			if( m_pArith->l_cmp(F24C, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}
			if( m_pArith->l_cmp(F27C, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}
//--> '15.02.06 INS START
			if( m_pArith->l_cmp(WORK0, PER100BUF) > 0 ){
				memmove( WORK0, PER100BUF, sizeof(WORK0) );
			}
//<-- '15.02.06 INS END
			ChangeColor( IDC_ICSDIAGCTRL2, ID27EX, 1 );
			diag_modify( IDC_ICSDIAGCTRL2, ID27EX, DIAG_MDFY_READONLY, CIcsdiagctrl );
		}
		else{
			if( (*m_pSnHeadData)->Sn_UPERS&0x10 ){
				// ‰ÛÅ”„ãŠ„‡@Žè“ü—Í
				DiagData.data_check = 1;
				diag_setdata( IDC_ICSDIAGCTRL2, ID27EX, (struct IUnknown *)&DiagData, CIcsdiagctrl );
				sprintf_s( buf, sizeof( buf ), _T("%d"), (*m_pSnFhyo20Data)->Sn_2FUWSEX );
				m_pArith->l_input( WORK0, buf );
//				if( pSyzShin->RATIO&0x02 ){
				if( (*m_pSnHeadData)->IsUseSemiUriRatio() ){
					memset( buf, '\0', sizeof( buf ) );
					strcpy_s( buf, sizeof( buf ), "‰Û@Å@”„@ã@Š„@‡@@(4/7)\ny‰ÛÅ”„ãŠ„‡‚É€‚¸‚éŠ„‡‚ð“K—pz" );
					DiagData.data_disp = buf;
					diag_setdata( IDC_ICSDIAGCTRL2, 16, (struct IUnknown *)&DiagData, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL2, ID2EX, 3 );
				}
				else{
					memset( buf, '\0', sizeof( buf ) );
					strcpy_s( buf, sizeof( buf ), "‰Û@Å@”„@ã@Š„@‡@@(4/7)" );
					DiagData.data_disp = buf;
					diag_setdata( IDC_ICSDIAGCTRL2, 16, (struct IUnknown *)&DiagData, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL2, ID2EX, 1 );
				}
				if( !((*m_pSnHeadData)->Sn_Sign4&0x01) ){
					diag_modify( IDC_ICSDIAGCTRL2, ID2EX, DIAG_MDFY_EDIT, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL2, ID2EX, 0 );
				}
				else{
					ChangeColor( IDC_ICSDIAGCTRL2, ID2EX, 2 );
				}
			}
			else{
				m_pArith->l_par100 ( (unsigned char *)WORK0, (unsigned char *)F24C, (unsigned char *)F27C, 2, 0 );
				// ”„ãŠ„‡‚ªƒ}ƒCƒiƒX‚Ìê‡‚Í‚O“‚ÅŒvŽZ‚·‚éB[03'05.17]
				memset( PW6, '\0', sizeof( PW6 ) );
				if( m_pArith->l_cmp(F24C, PW6) < 0 ){
					memset( WORK0, '\0', MONY_BUF_SIZE );
				}
				if( m_pArith->l_cmp(F27C, PW6) < 0 ){
					memset( WORK0, '\0', MONY_BUF_SIZE );
				}
//--> '15.02.06 INS START
				if( m_pArith->l_cmp(WORK0, PER100BUF) > 0 ){
					memmove( WORK0, PER100BUF, sizeof(WORK0) );
				}
//<-- '15.02.06 INS END
				memset( buf, '\0', sizeof( buf ) );
				strcpy_s( buf, sizeof( buf ), "‰Û@Å@”„@ã@Š„@‡@@(4/7)" );
				DiagData.data_disp = buf;
				diag_setdata( IDC_ICSDIAGCTRL2, 16, (struct IUnknown *)&DiagData, CIcsdiagctrl );

				if( !((*m_pSnHeadData)->Sn_Sign4&0x01) ){//[Y]
					ChangeColor( IDC_ICSDIAGCTRL2, ID2EX, 1 );
				}
				else{
					ChangeColor( IDC_ICSDIAGCTRL2, ID2EX, 2 );
				}
			}
		}
	}
	else	{
		m_pArith->l_input( WORK0, "10000" );
		ChangeColor( IDC_ICSDIAGCTRL2, ID27EX, 1 );
		diag_modify( IDC_ICSDIAGCTRL2, ID27EX, DIAG_MDFY_READONLY, CIcsdiagctrl );
	}
	m_pArith->l_print( buf, WORK0, "SSSSS" );
	F2UWS = atoi( buf );
	m_pArith->l_print( buf, WORK0, "SS9.99" );
	DiagData.data_edit = buf;
	diag_setdata( IDC_ICSDIAGCTRL2, ID2EX, (struct IUnknown *)&DiagData, CIcsdiagctrl );
}

//-----------------------------------------------------------------------------
// ‰ÛÅŽd“ü‚ÉŒW‚éÁ”ïÅŠz
//-----------------------------------------------------------------------------
void CShinFhyo20::f2calq_9()
{

	if( !((*m_pSnHeadData)->Sn_Sign4&0x80) ){	//˜A“®‚Ìê‡WŒv‚µ‚È‚¢BŽ©“®WŒv’l‚ðŽg—p‚·‚éˆ×
		
	}
	else{
		if(!(F29c&0x800) ){
	//	if( (F29c&0x100) && !(F29c&0x800) ){//[Y]
			if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
				ChangeColor( IDC_ICSDIAGCTRL2, ID29A, 0 );//[Y]
			}
/*- '14.05.16 -*/
//			memset( F29C, '\0', MONY_BUF_SIZE );
//			m_Util.percent( F29C, F28C, 9, 63, 0 );
/*-------------*/
			if( F29c&0x100 ){
				memset( F29C, '\0', MONY_BUF_SIZE );
				m_Util.percent( F29C, F28C, 9, 63, 0 );
			}
/*-------------*/
		}
	}
	dsp_prs( IDC_ICSDIAGCTRL2, ID29A, F29C );

	//[Y]
	if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){

		if( (F29c&0x800) ){
//		if( !(F29c&0x100) && (F29c&0x800) ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID29A, 3 );	// 2
		}

	}

// --> '14.05.16 INS START
	F29c &= ~0x111;
//<--  '14.05.16 INS END
}

//-----------------------------------------------------------------------------
// ‰ÛÅ‰Ý•¨‚ÉŒW‚éÁ”ïÅŠz
//-----------------------------------------------------------------------------
void CShinFhyo20::f2calq_11()
{
	memset( F211A, '\0', MONY_BUF_SIZE );
	memset( F211B, '\0', MONY_BUF_SIZE );
	dsp_prs( IDC_ICSDIAGCTRL2, ID211A, F211C );
}

//-----------------------------------------------------------------------------
// ‰ÛÅŽd“ü“™‚ÌÅŠz‚Ì‡ŒvŠz
//-----------------------------------------------------------------------------
void CShinFhyo20::f2calq_13()
{
	m_pArith->l_add( F213C, F29C,  F211C );
	m_pArith->l_add( F213C, F213C, F212C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID213A, F213C );
}

//-----------------------------------------------------------------------------
// ‰ÛÅ”„ãŠ„‡‚ª‚X‚T“ˆÈã‚Ìê‡
//-----------------------------------------------------------------------------
void CShinFhyo20::f2calq_14()
{
	dsp_prs( IDC_ICSDIAGCTRL2, ID213A, F213C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID214A, F213C );
	memset( F215A, '\0', MONY_BUF_SIZE );
	memset( F215B, '\0', MONY_BUF_SIZE );
	memset( F215C, '\0', MONY_BUF_SIZE );
	memset( F216A, '\0', MONY_BUF_SIZE );
	memset( F216B, '\0', MONY_BUF_SIZE );
	memset( F216C, '\0', MONY_BUF_SIZE );
	memset( F217A, '\0', MONY_BUF_SIZE );
	memset( F217B, '\0', MONY_BUF_SIZE );
	memset( F217C, '\0', MONY_BUF_SIZE );
	F217c = 0;
	memset( F218A, '\0', MONY_BUF_SIZE );
	memset( F218B, '\0', MONY_BUF_SIZE );
	memset( F218C, '\0', MONY_BUF_SIZE );
	F218c = 0;
	dsp_prs( IDC_ICSDIAGCTRL2, ID215A, F215C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID216A, F216C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID217A, F217C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID218A, F218C );

	// ŠY“–€–Ú“ü—Í•s‰Â
	// ‰ÛÅ”„ã‚Š„‡‚ª95%ˆÈã
	ChangeColor( IDC_ICSDIAGCTRL2, ID215A, 1 );
	ChangeColor( IDC_ICSDIAGCTRL2, ID216A, 1 );
	ChangeColor( IDC_ICSDIAGCTRL2, ID217A, 1 );
	ChangeColor( IDC_ICSDIAGCTRL2, ID218A, 1 );

	diag_modify( IDC_ICSDIAGCTRL2, ID215A, DIAG_MDFY_READONLY, CIcsdiagctrl );
	diag_modify( IDC_ICSDIAGCTRL2, ID216A, DIAG_MDFY_READONLY, CIcsdiagctrl );
	diag_modify( IDC_ICSDIAGCTRL2, ID217A, DIAG_MDFY_READONLY, CIcsdiagctrl );
	diag_modify( IDC_ICSDIAGCTRL2, ID218A, DIAG_MDFY_READONLY, CIcsdiagctrl );
}

//== ‰ÛÅ”„ãŠ„‡‚ª‚X‚T“–¢–ž‚Ìê‡ ==

//-----------------------------------------------------------------------------
// ‰ÛÅ”„ã‚É‚Ì‚Ý—v‚·‚éŽd“üÅŠz
//-----------------------------------------------------------------------------
void CShinFhyo20::f2calq_15()
{
	dsp_prs( IDC_ICSDIAGCTRL2, ID215A, F215C );
}

//-----------------------------------------------------------------------------
// ‹¤’Ê‚Ì”„ã‚É—v‚·‚éŽd“üÅŠz
//-----------------------------------------------------------------------------
void CShinFhyo20::f2calq_16()
{
	dsp_prs( IDC_ICSDIAGCTRL2, ID216A, F216C );
}

//-----------------------------------------------------------------------------
// ŒÂ•Ê‘Î‰ž•ûŽ®TœŽd“üÅŠz
//-----------------------------------------------------------------------------
void CShinFhyo20::f2calq_17()
{
	char	WORK0[MONY_BUF_SIZE] = {0};
	char	PW6[MONY_BUF_SIZE] = {0};
	char	buf[64] = {0};

//--> '15.02.06 INS START
	char	PER100BUF[MONY_BUF_SIZE] = {0};
	m_pArith->l_input( PER100BUF, _T("10000000") );
//<-- '15.02.06 INS END

	if( !(F217c & 0x100) ){	//[Y] Žè“ü—Í‚È‚µ
		memset( F217C, '\0', MONY_BUF_SIZE );
		if( (*m_pSnHeadData)->Sn_UPERS&0x10 ){
			sprintf_s( buf, sizeof( buf ), _T("%d"), (*m_pSnFhyo20Data)->Sn_2FUWSEX );
			m_pArith->l_input( WORK0, buf );
			m_pArith->l_mul100( (unsigned char *)F217C, (unsigned char *)F216C, (unsigned char *)WORK0, 2, 0 );
		}
		else{
			m_pArith->l_par100( (unsigned char *)WORK0, (unsigned char *)F24C, (unsigned char *)F27C,  5, 0 );

			// ”„ãŠ„‡‚ªƒ}ƒCƒiƒX‚Ìê‡‚Í‚O“‚ÅŒvŽZ‚·‚é
			memset( PW6, '\0', sizeof(PW6) );
			if( m_pArith->l_cmp(F24C, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}
			if( m_pArith->l_cmp(F27C, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}

			if( m_pArith->l_test(WORK0) == 0 ){
				m_pArith->l_mul100( (unsigned char *)F217C, (unsigned char *)F216C, (unsigned char *)WORK0, 5, 0 );
			}
			else{
//-- '15.02.06 --
//				m_Util.l_pardotnet_syz( F217C, F216C, F24C, F27C );
//---------------
				if(  m_pArith->l_cmp(WORK0, PER100BUF) <= 0 ){
					m_Util.l_pardotnet_syz( F217C, F216C, F24C, F27C );
				}
				else{
					m_Util.l_pardotnet_syz( F217C, F216C, F24C, F24C );
				}
//---------------
			}
		}
		m_pArith->l_add( F217C, F217C, F215C );
	}

	dsp_prs( IDC_ICSDIAGCTRL2, ID217A, F217C );
}

//-----------------------------------------------------------------------------
// ”ä—á”z•ª•ûŽ®TœŽd“üÅŠz
//-----------------------------------------------------------------------------
void CShinFhyo20::f2calq_18()
{
	char	WORK0[MONY_BUF_SIZE] = {0};
	char	PW6[MONY_BUF_SIZE] = {0};
	char	buf[64] = {0};

//--> '15.02.06 INS START
	char	PER100BUF[MONY_BUF_SIZE] = {0};
	m_pArith->l_input( PER100BUF, _T("10000000") );
//<-- '15.02.06 INS END

	if( !(F218c & 0x100) ){	// Žè“ü—Í‚È‚µ
		memset( F218C, '\0', MONY_BUF_SIZE );
		if( (*m_pSnHeadData)->Sn_UPERS&0x10 ){
			sprintf_s( buf, sizeof( buf ), _T("%d"), (*m_pSnFhyo20Data)->Sn_2FUWSEX );
			m_pArith->l_input( WORK0, buf );
			m_pArith->l_mul100( (unsigned char *)F218C, (unsigned char *)F213C, (unsigned char *)WORK0, 2, 0 );
		}
		else{
			m_pArith->l_par100( (unsigned char *)WORK0, (unsigned char *)F24C, (unsigned char *)F27C,  5, 0 );

			// ”„ãŠ„‡‚ªƒ}ƒCƒiƒX‚Ìê‡‚Í‚O“‚ÅŒvŽZ‚·‚é
			memset( PW6, '\0', sizeof(PW6) );
			if( m_pArith->l_cmp(F24C, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}
			if( m_pArith->l_cmp(F27C, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}

			if( m_pArith->l_test(WORK0) == 0 ){
				m_pArith->l_mul100( (unsigned char *)F218C, (unsigned char *)F213C, (unsigned char *)WORK0, 5, 0 );
			}
			else{
//-- '15.02.06 --
//				m_Util.l_pardotnet_syz( F218C, F213C, F24C, F27C );
//---------------
				if(  m_pArith->l_cmp(WORK0, PER100BUF) <= 0 ){
					m_Util.l_pardotnet_syz( F218C, F213C, F24C, F27C );
				}
				else{
					m_Util.l_pardotnet_syz( F218C, F213C, F24C, F24C );
				}
//---------------
			}
		}
	}

	dsp_prs( IDC_ICSDIAGCTRL2, ID218A, F218C);
}

//-----------------------------------------------------------------------------
// ‰ÛÅ”„ãŠ„‡‚ª‚X‚T“–¢–ž‚Ìê‡
//-----------------------------------------------------------------------------
void CShinFhyo20::f2calq_ko()
{
	if( (*m_pSnHeadData)->IsKobetuSiireAnbun() ){			// ŒÂ•Ê‘Î‰ž

		dsp_cls( IDC_ICSDIAGCTRL2, ID214A );

		f2calq_15();
		f2calq_16();
		f2calq_17();
		memset( F218A, '\0', MONY_BUF_SIZE );
		memset( F218B, '\0', MONY_BUF_SIZE );
		memset( F218C, '\0', MONY_BUF_SIZE );
		F218c = 0;
		dsp_prs( IDC_ICSDIAGCTRL2, ID218A, F218C );

		if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
			diag_modify( IDC_ICSDIAGCTRL2, ID215A, DIAG_MDFY_EDIT, CIcsdiagctrl );
			diag_modify( IDC_ICSDIAGCTRL2, ID216A, DIAG_MDFY_EDIT, CIcsdiagctrl );
			diag_modify( IDC_ICSDIAGCTRL2, ID217A, DIAG_MDFY_EDIT, CIcsdiagctrl );
			ChangeColor( IDC_ICSDIAGCTRL2, ID215A, 0 );
			ChangeColor( IDC_ICSDIAGCTRL2, ID216A, 0 );
			ChangeColor( IDC_ICSDIAGCTRL2, ID217A, 0 );

			//[Y]
			if(F217c&0x100){//Žè“ü—Í
				ChangeColor( IDC_ICSDIAGCTRL2, ID217A, 3 );
			}
			else{//Ž©“®ŒvŽZ
				ChangeColor( IDC_ICSDIAGCTRL2, ID217A, 0 );
			}
		}

		ChangeColor( IDC_ICSDIAGCTRL2, ID218A, 1 );
		diag_modify( IDC_ICSDIAGCTRL2, ID218A, DIAG_MDFY_READONLY, CIcsdiagctrl );
	}
	else{					// ”ä—á”z•ª

		dsp_cls( IDC_ICSDIAGCTRL2, ID214A );

		f2calq_18();
		memset( F215A, '\0', MONY_BUF_SIZE );
		memset( F215B, '\0', MONY_BUF_SIZE );
		memset( F215C, '\0', MONY_BUF_SIZE );
		memset( F216A, '\0', MONY_BUF_SIZE );
		memset( F216B, '\0', MONY_BUF_SIZE );
		memset( F216C, '\0', MONY_BUF_SIZE );
		memset( F217A, '\0', MONY_BUF_SIZE );
		memset( F217B, '\0', MONY_BUF_SIZE );
		memset( F217C, '\0', MONY_BUF_SIZE );
		F217c = 0;
		dsp_prs( IDC_ICSDIAGCTRL2, ID215A, F215C );
		dsp_prs( IDC_ICSDIAGCTRL2, ID216A, F216C );
		dsp_prs( IDC_ICSDIAGCTRL2, ID217A, F217C );

		ChangeColor( IDC_ICSDIAGCTRL2, ID215A, 1 );
		ChangeColor( IDC_ICSDIAGCTRL2, ID216A, 1 );
		ChangeColor( IDC_ICSDIAGCTRL2, ID217A, 1 );
		
		diag_modify( IDC_ICSDIAGCTRL2, ID215A, DIAG_MDFY_READONLY, CIcsdiagctrl );
		diag_modify( IDC_ICSDIAGCTRL2, ID216A, DIAG_MDFY_READONLY, CIcsdiagctrl );
		diag_modify( IDC_ICSDIAGCTRL2, ID217A, DIAG_MDFY_READONLY, CIcsdiagctrl );
		if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
			diag_modify( IDC_ICSDIAGCTRL2, ID218A, DIAG_MDFY_EDIT, CIcsdiagctrl );

			//[Y]
			if(F218c&0x100){//Žè“ü—Í
				ChangeColor( IDC_ICSDIAGCTRL2, ID218A, 3 );
			}
			else{//Ž©“®ŒvŽZ
				ChangeColor( IDC_ICSDIAGCTRL2, ID218A, 0 );
			}
		}
	}
}

//-----------------------------------------------------------------------------
// ·ˆøTœŽd“üÅŠz‚ÌŒvŽZ
//-----------------------------------------------------------------------------
void CShinFhyo20::f2calq_2122()
{
	char	va[MONY_BUF_SIZE]={0}, vb[MONY_BUF_SIZE]={0};
	char	WORK1[MONY_BUF_SIZE] = {0};

	if( F221c&0x100 ){	// Žè“ü—Í

		if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID221A, 3 );
			diag_modify( IDC_ICSDIAGCTRL2, ID221A, DIAG_MDFY_EDIT, CIcsdiagctrl );
		}

		memmove( F221C, (*m_pSnFhyo20Data)->Sn_2F21C, MONY_BUF_SIZE );//[Y]

		if( m_pArith->l_test(F221C) < 0 ){
			m_pArith->l_neg( F221C );
			memmove( F222C, F221C, MONY_BUF_SIZE );
			memset( F221C, '\0', MONY_BUF_SIZE );
		}
		else{
			memmove( F221C, (*m_pSnFhyo20Data)->Sn_2F21C, MONY_BUF_SIZE );
			memset( F222C, '\0', MONY_BUF_SIZE );
		}
		dsp_prs( IDC_ICSDIAGCTRL2, ID221A, F221C );	// Tœ‘ÎÛŽd“üÅŠz
		dsp_prs( IDC_ICSDIAGCTRL2, ID222A, F222C );	// Tœ‰ß‘å’²®Šz
	}
	else{	// Ž©“®ŒvŽZ

		if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID221A, 0 );
			diag_modify( IDC_ICSDIAGCTRL2, ID221A, DIAG_MDFY_EDIT, CIcsdiagctrl );
		}

		memset( F222A, '\0', MONY_BUF_SIZE );
		memset( F222B, '\0', MONY_BUF_SIZE );
		memset( F222C, '\0', MONY_BUF_SIZE );
		m_pArith->l_add( WORK1, F219C, F220C );
		memset( va, '\0', MONY_BUF_SIZE );
		memset( vb, '\0', MONY_BUF_SIZE );
//		if( pSyzShin->RATIO&0x02 ){
		if( (*m_pSnHeadData)->IsUseSemiUriRatio() ){
			// ‰ÛÅ”„ãŠ„‡‚É€‚¸‚éŠ„‡‚ð—LŒø
			memmove( vb, F217C, MONY_BUF_SIZE );
		}
		else{
//			if( F2UWS >= pSyzShin->RatioInt ){
			if( F2UWS >= (*m_pSnHeadData)->m_UriRatioBorder ){
				memmove( vb, F213C, MONY_BUF_SIZE );
			}
			else{
				memmove( vb, (*m_pSnHeadData)->IsKobetuSiireAnbun() ? F217C : F218C, MONY_BUF_SIZE );
			}
		}										
		m_pArith->l_add( vb, vb, WORK1 );
		if( m_pArith->l_test( vb ) < 0 ) {
			memset( F221A, '\0', MONY_BUF_SIZE );
			memset( F221B, '\0', MONY_BUF_SIZE );
			memset( F221C, '\0', MONY_BUF_SIZE );
			m_pArith->l_neg( vb );
			memmove( F222C, vb, MONY_BUF_SIZE );
		}
		else {
			memmove( F221C, vb, MONY_BUF_SIZE );
			memset( F222A, '\0', MONY_BUF_SIZE );
			memset( F222B, '\0', MONY_BUF_SIZE );
			memset( F222C, '\0', MONY_BUF_SIZE );
		}
		dsp_prs( IDC_ICSDIAGCTRL2, ID221A, F221C);
		dsp_prs( IDC_ICSDIAGCTRL2, ID222A, F222C);
	}
}

//-----------------------------------------------------------------------------
// Še“ü—Í‚ÌŒvŽZ•\Ž¦
//-----------------------------------------------------------------------------
// ˆø”	atno	F	
//-----------------------------------------------------------------------------
void CShinFhyo20::f2calq_all( int atno )
{
	while( 1 ){
		switch( atno ){
			case	1 :							// ‰ÛÅ”„ãŠz
			case	2 :							// –ÆÅ”„ãŠz
			case	3 :							// ”ñ‰ÛÅŽ‘ŽY‚Ì—Ao
				f2calq_45();					// ‰ÛÅŽ‘ŽY‚Ì÷“n“™‘Î‰¿Šz
			case	4 :							// ”ñ‰ÛÅ”„ãŠz
				f2calq_7();						// Ž‘ŽY‚Ì÷“n“™‘Î‰¿ŠzE‰ÛÅ”„ãŠ„‡
			case	5 :							// ‰ÛÅŽd“ü‚ÉŒW‚éŽx•¥‘Î‰¿Šz
			case	6 :
				f2calq_9();						// ‰ÛÅŽd“ü‚ÉŒW‚éÁ”ïÅŠz
			case	7 :
				f2calq_11();					// ‰ÛÅ‰Ý•¨‚ÉŒW‚éÁ”ïÅŠz
												// –ÆÅÌ‰ÛÅ’²®ÅŠz
				f2calq_13();					// ‰ÛÅŽd“ü“™‚ÌÅŠz‡Œv
			case	8 :	
			case	9 :
			case	10:
			case	11:	
//				if( pSyzShin->RATIO&0x02 ){
				if( (*m_pSnHeadData)->IsUseSemiUriRatio() ){
					// ‰ÛÅ”„ãŠ„‡‚É€‚¸‚éŠ„‡‚ð—LŒø
					f2calq_ko();				// ‰ÛÅ”„ãŠ„‡‚ª‚X‚T“–¢–ž
				}
				else{
//					if( F2UWS >= pSyzShin->RatioInt ){				// ‰ÛÅ”„ãŠ„‡‚ª‚X‚T“ˆÈã
					if( F2UWS >= (*m_pSnHeadData)->m_UriRatioBorder ){	// ‰ÛÅ”„ãŠ„‡‚ª‚X‚T“ˆÈã
						f2calq_14();
					}
					else{
						f2calq_ko();			// ‰ÛÅ”„ãŠ„‡‚ª‚X‚T“–¢–ž
					}
				}
//--> '15.04.04. INS START
				f2calq_spc();
//<-- '15.04.04. INS END
			case	12:							// TœÅŠz‚Ì’²®i‚P‚Xj
			case	13:							// TœÅŠz‚Ì’²®i‚Q‚Oj
				f2calq_2122();					// ·ˆøTœÅŠz
			case	14:
			default	:
				break;
		}
		break;
	}
}

//-----------------------------------------------------------------------------
// “Á’èŽû“ü‚ð‰Á–¡‚µ‚½Tœ‘ÎÛŽd“üÅŠz‚ÌŒvŽZ('15.04.04)
//-----------------------------------------------------------------------------
void CShinFhyo20::f2calq_spc()
{
	if( *(m_pShinInfo->pSgSpc) == 1 ){

//--> '15.05.18 INS START
		unsigned char svSn_Sign2 = (*m_pSnHeadData)->Sn_Sign2;
//<-- '15.05.18 INS END

		SYZTOSPC_DATA	SyzToSpc = {0};
		BOOL			IsRatioOver95 = FALSE;
		if( F2UWS >= (*m_pSnHeadData)->m_UriRatioBorder ){
			IsRatioOver95 = TRUE;
			SyzToSpc.IsRatioOver95 |= 0x01;
		}

		// ‰ÛÅ”„ãƒZƒbƒg
		m_pArith->l_add( SyzToSpc.SpKzur, F21A, F21B );
		m_pArith->l_add( SyzToSpc.SpKzur, SyzToSpc.SpKzur, F21C );
		m_pArith->l_add( SyzToSpc.SpMzur, F22A, F22B );
		m_pArith->l_add( SyzToSpc.SpMzur, SyzToSpc.SpMzur, F22C );
		m_pArith->l_add( SyzToSpc.SpMzur, SyzToSpc.SpMzur, F23A );
		m_pArith->l_add( SyzToSpc.SpMzur, SyzToSpc.SpMzur, F23B );
		m_pArith->l_add( SyzToSpc.SpMzur, SyzToSpc.SpMzur, F23C );
		m_pArith->l_add( SyzToSpc.SpHkur, F26A, F26B );
		m_pArith->l_add( SyzToSpc.SpHkur, SyzToSpc.SpHkur, F26C );
//2017.03.01 INSERT START
		memmove( SyzToSpc.SpKzur4, F21B, sizeof( SyzToSpc.SpKzur4 ));
		memmove( SyzToSpc.SpKzur63, F21C, sizeof( SyzToSpc.SpKzur63 ));
//2017.03.01 INSERT END

		// Tœ‘ÎÛŽd“üÅŠzƒZƒbƒg
		if( IsRatioOver95 ){
			//== ŒvŽZ•\‚Ti‚Pj ==
			memmove( SyzToSpc.SpKgz3, F213A, sizeof(SyzToSpc.SpKgz3) );
			memmove( SyzToSpc.SpKgz4, F213B, sizeof(SyzToSpc.SpKgz4) );
			memmove( SyzToSpc.SpKgz63, F213C, sizeof(SyzToSpc.SpKgz63) );
		}
		if( (*m_pSnHeadData)->IsKobetuSiireAnbun() && (IsRatioOver95==FALSE) ){
			//== ŒvŽZ•\‚Ti‚Qj ==
			memmove( SyzToSpc.SpKgz3, F217A, sizeof(SyzToSpc.SpKgz3) );
			memmove( SyzToSpc.SpKgz4, F217B, sizeof(SyzToSpc.SpKgz4) );
			memmove( SyzToSpc.SpKgz63, F217C, sizeof(SyzToSpc.SpKgz63) );
		}
		if( ((*m_pSnHeadData)->IsKobetuSiireAnbun()==FALSE) && (IsRatioOver95==FALSE) ){
			//== ŒvŽZ•\‚Ti‚Rj ==
			memmove( SyzToSpc.SpKgz3, F218A, sizeof(SyzToSpc.SpKgz3) );
			memmove( SyzToSpc.SpKgz4, F218B, sizeof(SyzToSpc.SpKgz4) );
			memmove( SyzToSpc.SpKgz63, F218C, sizeof(SyzToSpc.SpKgz63) );
		}
		// ÄŒvŽZ
		((CH26HyoView*)m_pParent)->RecalqSkj( &SyzToSpc );
		// ‹àŠzÄƒZƒbƒg
		if( (*m_pSnHeadData)->Sn_Sign2&0x02 ){
//-- '15.05.18 --
//			F221c &= ~0x100;
//---------------
			if( !(svSn_Sign2&0x02) ){
				F221c &= ~0x100;
			}
//---------------
		}
		else{
			memmove( F221A, SyzToSpc.SpSiz3, sizeof(F221A) );
			memmove( F221B, SyzToSpc.SpSiz4, sizeof(F221B) );
			memmove( F221C, SyzToSpc.SpSiz63, sizeof(F221C) );
			memmove( (*m_pSnFhyo20Data)->Sn_2F21A, SyzToSpc.SpSiz3, sizeof((*m_pSnFhyo20Data)->Sn_2F21A) );
			memmove( (*m_pSnFhyo20Data)->Sn_2F21B, SyzToSpc.SpSiz4, sizeof((*m_pSnFhyo20Data)->Sn_2F21B) );
			memmove( (*m_pSnFhyo20Data)->Sn_2F21C, SyzToSpc.SpSiz63, sizeof((*m_pSnFhyo20Data)->Sn_2F21C) );
			F221c |= 0x100;
		}
	}
}
