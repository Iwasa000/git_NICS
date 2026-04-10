
#include "stdafx.h"
#include "ShinFhyo22.h"
#include "ShinFhyo22Idx.h"
#include "H26HyoView.h"	// '15.04.04

//-----------------------------------------------------------------------------
// â€ê≈îÑè„äz
//-----------------------------------------------------------------------------
void CShinFhyo22::f22calq_1()
{
	char	WORK0[MONY_BUF_SIZE] = {0};
	m_pArith->l_add( WORK0, F221A, F221B );
	m_pArith->l_add( WORK0, WORK0, F221C );

	dsp_prs( IDC_ICSDIAGCTRL2, ID221A, F221A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID221B, F221B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID221C, F221C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID221D, WORK0 );
}

//-----------------------------------------------------------------------------
// â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
//-----------------------------------------------------------------------------
void CShinFhyo22::f22calq_45()
{
	char	WORK0[MONY_BUF_SIZE] = {0};
	m_pArith->l_add( WORK0, F221A, F221B );
	m_pArith->l_add( WORK0, WORK0, F221C );
	m_pArith->l_add( F224C, WORK0, F222C );
	m_pArith->l_add( F224C, F224C, F223C );

	memset( F222A, '\0', MONY_BUF_SIZE );
	memset( F222B, '\0', MONY_BUF_SIZE );
	memset( F223A, '\0', MONY_BUF_SIZE );
	memset( F223B, '\0', MONY_BUF_SIZE );

//2016.06.22 UPDATE START
//	dsp_prs( IDC_ICSDIAGCTRL2, ID224C, F224C );
	if( m_pArith->l_test( F224C ) < 0 ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID224C, FZERO );
	}else{
		dsp_prs( IDC_ICSDIAGCTRL2, ID224C, F224C );
	}
//2016.06.22 UPDATE END
	dsp_prs( IDC_ICSDIAGCTRL2, ID225C, F224C );
}

//-----------------------------------------------------------------------------
// îÒâ€ê≈îÑè„äz
//-----------------------------------------------------------------------------
void CShinFhyo22::f22calq_6()
{
	char	WORK0[MONY_BUF_SIZE] = {0};
	m_pArith->l_add( WORK0, F226A, F226B );
	m_pArith->l_add( WORK0, WORK0, F226C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID226C, WORK0 );
	memset( F226A, '\0', MONY_BUF_SIZE );
	memset( F226B, '\0', MONY_BUF_SIZE );
}

//-----------------------------------------------------------------------------
// éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz • â€ê≈îÑè„äÑçá
//-----------------------------------------------------------------------------
void CShinFhyo22::f22calq_7()
{
	char	WORK0[MONY_BUF_SIZE] = {0};
	char	PW6[MONY_BUF_SIZE] = {0};
	char	buf[512]={0};

	m_pArith->l_add( F227C, F224C, F226C );
//2016.06.22 UPDATE START
//	dsp_prs( IDC_ICSDIAGCTRL2, ID227C, F227C );
	if( m_pArith->l_test( F227C ) < 0 ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID227C, FZERO );
	}else{
		dsp_prs( IDC_ICSDIAGCTRL2, ID227C, F227C );
	}
//2016.06.22 UPDATE END


//--> '15.02.06 INS START
	char	PER100BUF[MONY_BUF_SIZE] = {0};
	m_pArith->l_input( PER100BUF, _T("10000") );
//<-- '15.02.06 INS END

	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	if( !m_DpSw ){	// çƒèWåvÇÃÇ›Åiï\é¶ñ≥ÇµÅj
		// â€ê≈îÑè„çÇäÑçá
		DiagData.data_check = 0;
		diag_setdata( IDC_ICSDIAGCTRL2, ID227EX, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		ChangeColor( IDC_ICSDIAGCTRL2, ID22EX, 1 );
		diag_modify( IDC_ICSDIAGCTRL2, ID22EX, DIAG_MDFY_READONLY, CIcsdiagctrl );
		memset( buf, '\0', sizeof( buf ) );
		memset( WORK0, '\0', MONY_BUF_SIZE );
//		if( !(pSyzShin->Snd.Sn_EXP100&0x01) ){
		if( !((*m_pSnHeadData)->Sn_EXP100&0x01) ){
//-- '15.04.04 --
//			if( m_pShinInfo->sgSpc ){
//---------------
			if( *m_pShinInfo->pSgSpc ){
//---------------
				// ì¡íËé˚ì¸óLÇË [08'11.13]
				m_pArith->l_par100( (unsigned char *)WORK0, (unsigned char *)F224C, (unsigned char *)F227C, 2, 0 );

				// îÑè„äÑçáÇ™É}ÉCÉiÉXÇÃèÍçáÇÕÇOÅìÇ≈åvéZÇ∑ÇÈ
				memset( PW6, '\0', sizeof( PW6 ) );
				if( m_pArith->l_cmp(F224C, PW6) < 0 ){
					memset( WORK0, '\0', MONY_BUF_SIZE );
				}
				if( m_pArith->l_cmp(F227C, PW6) < 0 ){
					memset( WORK0, '\0', MONY_BUF_SIZE );
				}
//--> '15.02.06 INS START
				if( m_pArith->l_cmp(WORK0, PER100BUF) > 0 ){
					memmove( WORK0, PER100BUF, sizeof(WORK0) );
				}
//<-- '15.02.06 INS END
				ChangeColor( IDC_ICSDIAGCTRL2, ID227EX, 1 );
				diag_modify( IDC_ICSDIAGCTRL2, ID227EX, DIAG_MDFY_READONLY, CIcsdiagctrl );
			}
			else{
				if( (*m_pSnHeadData)->Sn_UPERS&0x10 ){
					// â€ê≈îÑè„äÑçáÅ@éËì¸óÕ
					DiagData.data_check = 1;
					diag_setdata( IDC_ICSDIAGCTRL2, ID227EX, (struct IUnknown *)&DiagData, CIcsdiagctrl );
					sprintf_s( buf, sizeof(buf), _T("%d"), (*m_pSnFhyo22Data)->Sn_2FUWSEX );
					m_pArith->l_input( WORK0, buf );
					if( (*m_pSnHeadData)->IsUseSemiUriRatio() ){
						memset( buf, '\0', sizeof(buf) );
						strcpy_s( buf, sizeof(buf), _T("â€Å@ê≈Å@îÑÅ@è„Å@äÑÅ@çáÅ@Å@(4/7)\nÅyâ€ê≈îÑè„äÑçáÇ…èÄÇ∏ÇÈäÑçáÇìKópÅz") );
						DiagData.data_disp = buf;
						diag_setdata( IDC_ICSDIAGCTRL2, 18, (struct IUnknown *)&DiagData, CIcsdiagctrl );
						ChangeColor( IDC_ICSDIAGCTRL2, ID22EX, 1 );
					}
					else{
						memset( buf, '\0', sizeof(buf) );
						strcpy_s( buf, sizeof(buf), _T("â€Å@ê≈Å@îÑÅ@è„Å@äÑÅ@çáÅ@Å@(4/7)") );
						DiagData.data_disp = buf;
						diag_setdata( IDC_ICSDIAGCTRL2, 18, (struct IUnknown *)&DiagData, CIcsdiagctrl );
						ChangeColor( IDC_ICSDIAGCTRL2, ID22EX, 1 );
					}

					if( !((*m_pSnHeadData)->Sn_Sign4&0x01) ){
						ChangeColor( IDC_ICSDIAGCTRL2, ID22EX, 0 );
						diag_modify( IDC_ICSDIAGCTRL2, ID22EX, DIAG_MDFY_EDIT, CIcsdiagctrl );
					}
					else{
						ChangeColor( IDC_ICSDIAGCTRL2, ID22EX, 2 );
					}
				}
				else{
					m_pArith->l_par100( (unsigned char *)WORK0, (unsigned char *)F224C, (unsigned char *)F227C, 2, 0 );
					
					// îÑè„äÑçáÇ™É}ÉCÉiÉXÇÃèÍçáÇÕÇOÅìÇ≈åvéZÇ∑ÇÈ
					memset( PW6, '\0', sizeof(PW6) );
					if( m_pArith->l_cmp(F224C, PW6) < 0 ){
						memset( WORK0, '\0', MONY_BUF_SIZE );
					}
					if( m_pArith->l_cmp(F227C, PW6) < 0 ){
						memset( WORK0, '\0', MONY_BUF_SIZE );
					}
//--> '15.02.06 INS START
					if( m_pArith->l_cmp(WORK0, PER100BUF) > 0 ){
						memmove( WORK0, PER100BUF, sizeof(WORK0) );
					}
//<-- '15.02.06 INS END
					memset( buf, '\0', sizeof(buf) );
					strcpy_s( buf, sizeof(buf), _T("â€Å@ê≈Å@îÑÅ@è„Å@äÑÅ@çáÅ@Å@(4/7)") );
					DiagData.data_disp = buf;
					diag_setdata( IDC_ICSDIAGCTRL2, 18, (struct IUnknown *)&DiagData, CIcsdiagctrl );
					ChangeColor( IDC_ICSDIAGCTRL2, ID22EX, 1 );

					

				}
			}
		}
		else{
			m_pArith->l_input( WORK0, _T("10000") );
			ChangeColor( IDC_ICSDIAGCTRL2, ID227EX, 1 );
			diag_modify( IDC_ICSDIAGCTRL2, ID227EX, DIAG_MDFY_READONLY, CIcsdiagctrl );
		}
		m_pArith->l_print( buf, WORK0, _T("SSSSS") );
		F22UWS = atoi( buf );
		m_pArith->l_print( buf, WORK0, _T("SS9.99") );
		DiagData.data_edit = buf;
		diag_setdata( IDC_ICSDIAGCTRL2, ID22EX, (struct IUnknown *)&DiagData, CIcsdiagctrl );
	}
	else{
		// åvéZÇÃÇ›
		memset( buf, '\0', sizeof( buf ) );
		memset( WORK0, '\0', MONY_BUF_SIZE );
//		if( !(pSyzShin->Snd.Sn_EXP100&0x01) ){
		if( !((*m_pSnHeadData)->Sn_EXP100&0x01) ){
			// â€ê≈îÑè„äÑçá
//-- '15.04.04 --
//			if( m_pShinInfo->sgSpc ){
//---------------
			if( *m_pShinInfo->pSgSpc ){
//---------------
				// ì¡íËé˚ì¸óLÇËÅ@[08'11.13]
				m_pArith->l_par100( (unsigned char *)WORK0, (unsigned char *)F224C, (unsigned char *)F227C, 2, 0 );
				// îÑè„äÑçáÇ™É}ÉCÉiÉXÇÃèÍçáÇÕÇOÅìÇ≈åvéZÇ∑ÇÈÅB[03'05.17]
				memset( PW6, '\0', sizeof( PW6 ) );
				if( m_pArith->l_cmp(F224C, PW6) < 0 ){
					memset( WORK0, '\0', MONY_BUF_SIZE );
				}
				if( m_pArith->l_cmp(F227C, PW6) < 0 ){
					memset( WORK0, '\0', MONY_BUF_SIZE );
				}
//--> '15.02.06 INS START
				if( m_pArith->l_cmp(WORK0, PER100BUF) > 0 ){
					memmove( WORK0, PER100BUF, sizeof(WORK0) );
				}
//<-- '15.02.06 INS END
			}
			else{
				if( (*m_pSnHeadData)->Sn_UPERS&0x10 ){
					sprintf_s( buf, sizeof( buf ), _T("%d"), (*m_pSnFhyo22Data)->Sn_2FUWSEX );
					m_pArith->l_input( WORK0, buf );
				}
				else{
					// â€ê≈îÑè„äÑçá
					m_pArith->l_par100( (unsigned char *)WORK0, (unsigned char *)F224C, (unsigned char *)F227C, 2, 0 );
					// îÑè„äÑçáÇ™É}ÉCÉiÉXÇÃèÍçáÇÕÇOÅìÇ≈åvéZÇ∑ÇÈÅB[03'05.17]
					memset( PW6, '\0', sizeof(PW6) );
					if( m_pArith->l_cmp(F224C, PW6) < 0 ){
						memset( WORK0, '\0', MONY_BUF_SIZE );
					}
					if( m_pArith->l_cmp(F227C, PW6) < 0 ){
						memset( WORK0, '\0', MONY_BUF_SIZE );
					}
//--> '15.02.06 INS START
					if( m_pArith->l_cmp(WORK0, PER100BUF) > 0 ){
						memmove( WORK0, PER100BUF, sizeof(WORK0) );
					}
//<-- '15.02.06 INS END
				}
			}
		}
		else{
			m_pArith->l_input( WORK0, _T("10000") );
		}
		m_pArith->l_print( buf, WORK0, _T("SSSSS") );

		F22UWS = atoi( buf );
	}
}

//-----------------------------------------------------------------------------
// â€ê≈édì¸Ç…åWÇÈéxï•ëŒâøÇÃäz
//-----------------------------------------------------------------------------
void CShinFhyo22::f22calq_8()
{
	char	WORK0[MONY_BUF_SIZE] = {0};

	m_pArith->l_add( WORK0, F228B, F228A );
	m_pArith->l_add( WORK0, WORK0, F228C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID228A, F228A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID228B, F228B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID228C, F228C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID228D, WORK0 );
}

//-----------------------------------------------------------------------------
// â€ê≈édì¸Ç…åWÇÈè¡îÔê≈äz
//-----------------------------------------------------------------------------
void CShinFhyo22::f22calq_9()
{
	char	WORK0[MONY_BUF_SIZE] = {0};

	if(!((*m_pSnHeadData)->Sn_Sign4&0x80) ){	//òAìÆÇÃèÍçáèWåvÇµÇ»Ç¢ÅBé©ìÆèWåvílÇégópÇ∑ÇÈà◊
		
	}
	else{
		// 3%
		if( !(F229c&0x80) ){
	//	if( (F229c&0x10) && !(F229c&0x80) ){
			if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){//[Y]
				ChangeColor( IDC_ICSDIAGCTRL2, ID229A, 0 );	
			}
/*- '14.05.16 -*/
//			memset( F229A, '\0', MONY_BUF_SIZE );
//			m_Util.percent( F229A, F228A, 0, 30, 0 );
/*-------------*/
			if( F229c&0x10 ){
				memset( F229A, '\0', MONY_BUF_SIZE );
				m_Util.percent( F229A, F228A, 0, 30, 0 );
			}
/*-------------*/
		}
		// 4%
		if( !(F229c&0x08) ){
	//	if( (F229c&0x01) && !(F229c&0x08) ){
			if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){//[Y]
				ChangeColor( IDC_ICSDIAGCTRL2, ID229B, 0 );	
			}
/*- '14.05.16 -*/
//			memset( F229B, '\0', MONY_BUF_SIZE );
//			m_Util.percent( F229B, F228B, 4, 40, 0 );
/*-------------*/
			if( F229c&0x01 ){
				memset( F229B, '\0', MONY_BUF_SIZE );
				m_Util.percent( F229B, F228B, 4, 40, 0 );
			}
/*-------------*/
		}
		// 6.3%
		if( !(F229c&0x800) ){
	//	if( (F229c&0x100) && !(F229c&0x800) ){
			if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){//[Y]
				ChangeColor( IDC_ICSDIAGCTRL2, ID229C, 0 );	
			}
/*- '14.05.16 -*/
//			memset( F229C, '\0', MONY_BUF_SIZE );
//			m_Util.percent( F229C, F228C, 9, 63, 0 );
/*-------------*/
			if( F229c&0x100 ){
				memset( F229C, '\0', MONY_BUF_SIZE );
				m_Util.percent( F229C, F228C, 9, 63, 0 );
			}
/*-------------*/
		}
	}
	m_pArith->l_add( WORK0, F229A, F229B );
	m_pArith->l_add( WORK0, WORK0, F229C );

	dsp_prs( IDC_ICSDIAGCTRL2, ID229A, F229A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID229B, F229B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID229C, F229C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID229D, WORK0 );


	//[Y]
	if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
		// 3%
		if( (F229c&0x80) ){
//		if( !(F229c&0x10) && (F229c&0x80) ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID229A, 3 );	// 2
		}
		// 4%
		if( (F229c&0x08) ){
//		if( !(F229c&0x01) && (F229c&0x08) ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID229B, 3 );	// 2
		}
		// 6.3%
		if( (F229c&0x800) ){
//		if( !(F229c&0x100) && (F229c&0x800) ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID229C, 3 );	// 2
		}
	}

// --> '14.05.16 INS START
	F229c &= ~(0x10|0x01|0x100);
//<--  '14.05.16 INS END

}

//-----------------------------------------------------------------------------
// â€ê≈â›ï®Ç…åWÇÈè¡îÔê≈äz
//-----------------------------------------------------------------------------
void CShinFhyo22::f22calq_11()
{
	char	WORK0[MONY_BUF_SIZE] = {0};

//	memset( F2210A, '\0', MONY_BUF_SIZE );
//	memset( F2210B, '\0', MONY_BUF_SIZE );
//	memset( F2210C, '\0', MONY_BUF_SIZE );

	m_pArith->l_add( WORK0, F2211A, F2211B );
	m_pArith->l_add( WORK0, WORK0, F2211C );

	dsp_prs( IDC_ICSDIAGCTRL2, ID2211A, F2211A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2211B, F2211B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2211C, F2211C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2211D, WORK0 );
}

//-----------------------------------------------------------------------------
// â€ê≈éñã∆ÅÃñ∆ê≈éñã∆ í≤êÆê≈äz
//-----------------------------------------------------------------------------
void CShinFhyo22::f22calq_12()
{
	char	WORK0[MONY_BUF_SIZE] = {0};
	m_pArith->l_add( WORK0, F2212A, F2212B );
	m_pArith->l_add( WORK0, WORK0, F2212C );

	dsp_prs( IDC_ICSDIAGCTRL2, ID2212A, F2212A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2212B, F2212B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2212C, F2212C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2212D, WORK0 );
}

//-----------------------------------------------------------------------------
// â€ê≈édì¸ìôÇÃê≈äzÇÃçáåväz
//-----------------------------------------------------------------------------
void CShinFhyo22::f22calq_13()
{
	m_pArith->l_add( F2213A, F229A,  F2211A );
	m_pArith->l_add( F2213A, F2213A, F2212A );
	m_pArith->l_add( F2213B, F229B,  F2211B );
	m_pArith->l_add( F2213B, F2213B, F2212B );
	m_pArith->l_add( F2213C, F229C,  F2211C );
	m_pArith->l_add( F2213C, F2213C, F2212C );

	char	WORK0[MONY_BUF_SIZE] = {0};
	m_pArith->l_add( WORK0, F2213A, F2213B );
	m_pArith->l_add( WORK0, WORK0, F2213C );

	dsp_prs( IDC_ICSDIAGCTRL2, ID2213A, F2213A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2213B, F2213B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2213C, F2213C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2213D, WORK0 );
}

//-----------------------------------------------------------------------------
// â€ê≈îÑè„äÑçáÇ™ÇXÇTÅìà»è„ÇÃèÍçá
//-----------------------------------------------------------------------------
void CShinFhyo22::f22calq_14()
{
	char	WORK0[MONY_BUF_SIZE] = {0};
	m_pArith->l_add( WORK0, F2213A, F2213B );
	m_pArith->l_add( WORK0, WORK0, F2213C );

	dsp_prs( IDC_ICSDIAGCTRL2, ID2214A, F2213A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2214B, F2213B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2214C, F2213C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2214D, WORK0 );

	memset( F2215A, '\0', MONY_BUF_SIZE );
	memset( F2215B, '\0', MONY_BUF_SIZE );
	memset( F2215C, '\0', MONY_BUF_SIZE );

	memset( F2216A, '\0', MONY_BUF_SIZE );
	memset( F2216B, '\0', MONY_BUF_SIZE );
	memset( F2216C, '\0', MONY_BUF_SIZE );

	memset( F2217A, '\0', MONY_BUF_SIZE );
	memset( F2217B, '\0', MONY_BUF_SIZE );
	memset( F2217C, '\0', MONY_BUF_SIZE );
	F2217c = 0;
	memset( F2218A, '\0', MONY_BUF_SIZE );
	memset( F2218B, '\0', MONY_BUF_SIZE );
	memset( F2218C, '\0', MONY_BUF_SIZE );
	F2218c = 0;
	memset( WORK0, '\0', MONY_BUF_SIZE );

	dsp_prs( IDC_ICSDIAGCTRL2, ID2215A, F2215A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2215B, F2215B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2215C, F2215C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2215D, WORK0 );

	dsp_prs( IDC_ICSDIAGCTRL2, ID2216A, F2216A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2216B, F2216B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2216C, F2216C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2216D, WORK0 );

	dsp_prs( IDC_ICSDIAGCTRL2, ID2217A, F2217A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2217B, F2217B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2217C, F2217C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2217D, WORK0 );

	dsp_prs( IDC_ICSDIAGCTRL2, ID2218A, F2218A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2218B, F2218B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2218C, F2218C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2218D, WORK0 );

	// äYìñçÄñ⁄ì¸óÕïsâ¬
	// â€ê≈îÑè„çÇäÑçáÇ™95%à»è„
	ChangeColor( IDC_ICSDIAGCTRL2, ID2215A, 1 );
	ChangeColor( IDC_ICSDIAGCTRL2, ID2215B, 1 );
	ChangeColor( IDC_ICSDIAGCTRL2, ID2215C, 1 );

	ChangeColor( IDC_ICSDIAGCTRL2, ID2216A, 1 );
	ChangeColor( IDC_ICSDIAGCTRL2, ID2216B, 1 );
	ChangeColor( IDC_ICSDIAGCTRL2, ID2216C, 1 );

	ChangeColor( IDC_ICSDIAGCTRL2, ID2217A, 1 );
	ChangeColor( IDC_ICSDIAGCTRL2, ID2217B, 1 );
	ChangeColor( IDC_ICSDIAGCTRL2, ID2217C, 1 );

	ChangeColor( IDC_ICSDIAGCTRL2, ID2218A, 1 );
	ChangeColor( IDC_ICSDIAGCTRL2, ID2218B, 1 );
	ChangeColor( IDC_ICSDIAGCTRL2, ID2218C, 1 );

	ModifyDiagInput( ID2215A, DIAG_MDFY_READONLY );
	ModifyDiagInput( ID2215B, DIAG_MDFY_READONLY );
	ModifyDiagInput( ID2215C, DIAG_MDFY_READONLY );

	ModifyDiagInput( ID2216A, DIAG_MDFY_READONLY );
	ModifyDiagInput( ID2216B, DIAG_MDFY_READONLY );
	ModifyDiagInput( ID2216C, DIAG_MDFY_READONLY );

	ModifyDiagInput( ID2217A, DIAG_MDFY_READONLY );
	ModifyDiagInput( ID2217B, DIAG_MDFY_READONLY );
	ModifyDiagInput( ID2217C, DIAG_MDFY_READONLY );

	ModifyDiagInput( ID2218A, DIAG_MDFY_READONLY );
	ModifyDiagInput( ID2218B, DIAG_MDFY_READONLY );
	ModifyDiagInput( ID2218C, DIAG_MDFY_READONLY );
}

//-----------------------------------------------------------------------------
// â€ê≈îÑè„Ç…ÇÃÇ›óvÇ∑ÇÈédì¸ê≈äz
//-----------------------------------------------------------------------------
void CShinFhyo22::f22calq_15()
{
	char	WORK0[MONY_BUF_SIZE] = {0};
	m_pArith->l_add( WORK0, F2215A, F2215B );
	m_pArith->l_add( WORK0, WORK0, F2215C );

	dsp_prs( IDC_ICSDIAGCTRL2, ID2215A, F2215A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2215B, F2215B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2215C, F2215C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2215D, WORK0 );
}

//-----------------------------------------------------------------------------
// ã§í ÇÃîÑè„Ç…óvÇ∑ÇÈédì¸ê≈äz
//-----------------------------------------------------------------------------
void CShinFhyo22::f22calq_16()
{
	char	WORK0[MONY_BUF_SIZE] = {0};
	m_pArith->l_add( WORK0, F2216A, F2216B );
	m_pArith->l_add( WORK0, WORK0, F2216C );

	dsp_prs( IDC_ICSDIAGCTRL2, ID2216A, F2216A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2216B, F2216B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2216C, F2216C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2216D, WORK0 );
}

//-----------------------------------------------------------------------------
// å¬ï ëŒâûï˚éÆçTèúédì¸ê≈äz
//-----------------------------------------------------------------------------
void CShinFhyo22::f22calq_17()
{
	char	WORK0[MONY_BUF_SIZE] = {0};
	char	PW6[MONY_BUF_SIZE] = {0};
	char	buf[512] = {0};

//--> '15.02.06 INS START
	char	PER100BUF[MONY_BUF_SIZE] = {0};
	m_pArith->l_input( PER100BUF, _T("10000000") );
//<-- '15.02.06 INS END
	
	if( !(F2217c & 0x100) ){	// éËì¸óÕÇ»Çµ
		memset( F2217C, '\0', MONY_BUF_SIZE );
		if( (*m_pSnHeadData)->Sn_UPERS & 0x10 ){

			//[Y]
			sprintf_s( buf, sizeof( buf ), _T("%d"), (*m_pSnFhyo22Data)->Sn_2FUWSEX );
			m_pArith->l_input( WORK0, buf );

			m_pArith->l_mul100( (unsigned char *)F2217C, (unsigned char *)F2216C, (unsigned char *)WORK0, 2, 0 );
		}
		else{
			m_pArith->l_par100( (unsigned char *)WORK0, (unsigned char *)F224C,  (unsigned char *)F227C,  5, 0 );

			// îÑè„äÑçáÇ™É}ÉCÉiÉXÇÃèÍçáÇÕÇOÅìÇ≈åvéZÇ∑ÇÈÅB
			memset( PW6, '\0', sizeof(PW6) );
			if( m_pArith->l_cmp(F224C, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}
			if( m_pArith->l_cmp(F227C, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}

			if( m_pArith->l_test(WORK0) == 0 ){
				m_pArith->l_mul100( (unsigned char *)F2217C, (unsigned char *)F2216C, (unsigned char *)WORK0, 5, 0 );
			}
			else{
//-- '15.02.06 --
//				m_Util.l_pardotnet_syz( F2217C, F2216C, F224C, F227C );
//---------------
				if(  m_pArith->l_cmp(WORK0, PER100BUF) <= 0 ){
					m_Util.l_pardotnet_syz( F2217C, F2216C, F224C, F227C );
				}
				else{
					m_Util.l_pardotnet_syz( F2217C, F2216C, F224C, F224C );
				}
//---------------
			}
		}
		m_pArith->l_add( F2217C, F2217C, F2215C );
	}
	if( !(F2217c & 0x01) ){		// éËì¸óÕÇ»Çµ
		memset( F2217B, '\0', MONY_BUF_SIZE );
		if( (*m_pSnHeadData)->Sn_UPERS & 0x10 ){
			sprintf_s( buf, sizeof( buf ), _T("%d"), (*m_pSnFhyo22Data)->Sn_2FUWSEX );
			m_pArith->l_input( WORK0, buf );
			m_pArith->l_mul100( (unsigned char *)F2217B, (unsigned char *)F2216B, (unsigned char *)WORK0, 2, 0 );
		}
		else{
			m_pArith->l_par100( (unsigned char *)WORK0, (unsigned char *)F224C,  (unsigned char *)F227C,  5, 0 );

			// îÑè„äÑçáÇ™É}ÉCÉiÉXÇÃèÍçáÇÕÇOÅìÇ≈åvéZÇ∑ÇÈÅB
			memset( PW6, '\0', sizeof(PW6) );
			if( m_pArith->l_cmp(F224C, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}
			if( m_pArith->l_cmp(F227C, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}

			if( m_pArith->l_test(WORK0) == 0 ){
				m_pArith->l_mul100( (unsigned char *)F2217B, (unsigned char *)F2216B, (unsigned char *)WORK0, 5, 0 );
			}
			else{
//-- '15.02.06 --
//				m_Util.l_pardotnet_syz( F2217B, F2216B, F224C, F227C );
//---------------
				if(  m_pArith->l_cmp(WORK0, PER100BUF) <= 0 ){
					m_Util.l_pardotnet_syz( F2217B, F2216B, F224C, F227C );
				}
				else{
					m_Util.l_pardotnet_syz( F2217B, F2216B, F224C, F224C );
				}
//---------------
			}
		}		
		m_pArith->l_add( F2217B, F2217B, F2215B );
	}
	if( !(F2217c & 0x10) ){	// éËì¸óÕÇ»Çµ
		memset( F2217A, '\0', MONY_BUF_SIZE );
		if( (*m_pSnHeadData)->Sn_UPERS & 0x10 ){
			//[Y]
			sprintf_s( buf, sizeof( buf ), _T("%d"), (*m_pSnFhyo22Data)->Sn_2FUWSEX );
			m_pArith->l_input( WORK0, buf );
			
			m_pArith->l_mul100( (unsigned char *)F2217A, (unsigned char *)F2216A, (unsigned char *)WORK0, 2, 0 );
		}
		else{
			m_pArith->l_par100( (unsigned char *)WORK0, (unsigned char *)F224C,  (unsigned char *)F227C,  5, 0 );

			// îÑè„äÑçáÇ™É}ÉCÉiÉXÇÃèÍçáÇÕÇOÅìÇ≈åvéZÇ∑ÇÈÅB
			memset( PW6, '\0', sizeof(PW6) );
			if( m_pArith->l_cmp(F224C, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}
			if( m_pArith->l_cmp(F227C, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}

			if( m_pArith->l_test(WORK0) == 0 ){
				m_pArith->l_mul100( (unsigned char *)F2217A, (unsigned char *)F2216A, (unsigned char *)WORK0, 5, 0 );
			}
			else{
//-- '15.02.06 --
//				m_Util.l_pardotnet_syz( F2217A, F2216A, F224C, F227C );
//---------------
				if(  m_pArith->l_cmp(WORK0, PER100BUF) <= 0 ){
					m_Util.l_pardotnet_syz( F2217A, F2216A, F224C, F227C );
				}
				else{
					m_Util.l_pardotnet_syz( F2217A, F2216A, F224C, F224C );
				}
//---------------
			}
		}
		m_pArith->l_add( F2217A, F2217A, F2215A );
	}
	
	m_pArith->l_add( WORK0, F2217A, F2217B );
	m_pArith->l_add( WORK0, WORK0, F2217C );

	dsp_prs( IDC_ICSDIAGCTRL2, ID2217A, F2217A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2217B, F2217B);
	dsp_prs( IDC_ICSDIAGCTRL2, ID2217C, F2217C);
	dsp_prs( IDC_ICSDIAGCTRL2, ID2217D, WORK0  );
}

//-----------------------------------------------------------------------------
// î‰ó·îzï™ï˚éÆçTèúédì¸ê≈äz
//-----------------------------------------------------------------------------
void CShinFhyo22::f22calq_18()
{
	char	WORK0[MONY_BUF_SIZE] = {0};
	char	PW6[MONY_BUF_SIZE] = {0};
	char	buf[512] = {0};

//--> '15.02.06 INS START
	char	PER100BUF[MONY_BUF_SIZE] = {0};
	m_pArith->l_input( PER100BUF, _T("10000000") );
//<-- '15.02.06 INS END
	
	if( !(F2218c & 0x01) ){	// éËì¸óÕÇ»Çµ
		memset( F2218B, '\0', MONY_BUF_SIZE );
		if( (*m_pSnHeadData)->Sn_UPERS & 0x10 ){
			sprintf_s( buf, sizeof( buf ), _T("%d"), (*m_pSnFhyo22Data)->Sn_2FUWSEX );
			m_pArith->l_input( WORK0, buf );
			m_pArith->l_mul100( (unsigned char *)F2218B, (unsigned char *)F2213B, (unsigned char *)WORK0, 2, 0 );	// [05'06.06]
		}
		else{
			m_pArith->l_par100( (unsigned char *)WORK0, (unsigned char *)F224C,  (unsigned char *)F227C,  5, 0 );
			// îÑè„äÑçáÇ™É}ÉCÉiÉXÇÃèÍçáÇÕÇOÅìÇ≈åvéZÇ∑ÇÈÅB
			memset( PW6, '\0', sizeof( PW6 ) );
			if( m_pArith->l_cmp(F224C, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}
			if( m_pArith->l_cmp(F227C, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}

			if( m_pArith->l_test(WORK0) == 0 ){
				m_pArith->l_mul100( (unsigned char *)F2218B, (unsigned char *)F2213B, (unsigned char *)WORK0, 5, 0 );
			}
			else{
//-- '15.02.06 --
//				m_Util.l_pardotnet_syz( F2218B, F2213B, F224C, F227C );
//---------------
				if( m_pArith->l_cmp(WORK0, PER100BUF) <= 0 ){
					m_Util.l_pardotnet_syz( F2218B, F2213B, F224C, F227C );
				}
				else{
					m_Util.l_pardotnet_syz( F2218B, F2213B, F224C, F224C );
				}
//---------------
			}
		}
	}

	if( !(F2218c & 0x100) ){	// éËì¸óÕÇ»Çµ
		memset( F2218C, '\0', MONY_BUF_SIZE );
		if( (*m_pSnHeadData)->Sn_UPERS & 0x10 ){
			sprintf_s( buf, sizeof( buf ), _T("%d"), (*m_pSnFhyo22Data)->Sn_2FUWSEX );
			m_pArith->l_input( WORK0, buf );
			m_pArith->l_mul100( (unsigned char *)F2218C, (unsigned char *)F2213C, (unsigned char *)WORK0, 2, 0 );	// [05'06.06]
		}
		else{
			m_pArith->l_par100( (unsigned char *)WORK0, (unsigned char *)F224C,  (unsigned char *)F227C,  5, 0 );
			// îÑè„äÑçáÇ™É}ÉCÉiÉXÇÃèÍçáÇÕÇOÅìÇ≈åvéZÇ∑ÇÈÅB
			memset( PW6, '\0', sizeof( PW6 ) );
			if( m_pArith->l_cmp(F224C, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}
			if( m_pArith->l_cmp(F227C, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}

			if( m_pArith->l_test(WORK0) == 0 ){
				m_pArith->l_mul100( (unsigned char *)F2218C, (unsigned char *)F2213C, (unsigned char *)WORK0, 5, 0 );
			}
			else{
//-- '15.02.06 --
//				m_Util.l_pardotnet_syz( F2218C, F2213C, F224C, F227C );
//---------------
				if( m_pArith->l_cmp(WORK0, PER100BUF) <= 0 ){
					m_Util.l_pardotnet_syz( F2218C, F2213C, F224C, F227C );
				}
				else{
					m_Util.l_pardotnet_syz( F2218C, F2213C, F224C, F224C );
				}
//---------------
			}
		}
	}

	if( !(F2218c & 0x10) ){	// éËì¸óÕÇ»Çµ
		memset( F2218A, '\0', MONY_BUF_SIZE );
		if( (*m_pSnHeadData)->Sn_UPERS & 0x10 ){

			//[Y]
			sprintf_s( buf, sizeof( buf ), _T("%d"), (*m_pSnFhyo22Data)->Sn_2FUWSEX );
			m_pArith->l_input( WORK0, buf );

			m_pArith->l_mul100( (unsigned char *)F2218A, (unsigned char *)F2213A, (unsigned char *)WORK0, 2, 0 );
		}
		else{

			//[Y]
			m_pArith->l_par100( (unsigned char *)WORK0, (unsigned char *)F224C,  (unsigned char *)F227C,  5, 0 );
			// îÑè„äÑçáÇ™É}ÉCÉiÉXÇÃèÍçáÇÕÇOÅìÇ≈åvéZÇ∑ÇÈÅB
			memset( PW6, '\0', sizeof( PW6 ) );
			if( m_pArith->l_cmp(F224C, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}
			if( m_pArith->l_cmp(F227C, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}

			if( m_pArith->l_test(WORK0) == 0 ){
				m_pArith->l_mul100( (unsigned char *)F2218A, (unsigned char *)F2213A, (unsigned char *)WORK0, 5, 0 );
			}
			else{
//-- '15.02.06 --
//				m_Util.l_pardotnet_syz( F2218A, F2213A, F224C, F227C );
//---------------
				if( m_pArith->l_cmp(WORK0, PER100BUF) <= 0 ){
					m_Util.l_pardotnet_syz( F2218A, F2213A, F224C, F227C );
				}
				else{
					m_Util.l_pardotnet_syz( F2218A, F2213A, F224C, F224C );
				}
//---------------
			}
		}
	}
	
	m_pArith->l_add( WORK0, F2218A, F2218B );
	m_pArith->l_add( WORK0, WORK0, F2218C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2218A, F2218A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2218B, F2218B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2218C, F2218C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2218D, WORK0 );
}

//-----------------------------------------------------------------------------
// â€ê≈îÑè„äÑçáÇ™ÇXÇTÅìñ¢ñûÇÃèÍçá
//-----------------------------------------------------------------------------
void CShinFhyo22::f22calq_ko()
{
	char	WORK0[MONY_BUF_SIZE] = {0};

	if( (*m_pSnHeadData)->IsKobetuSiireAnbun() ){
		dsp_cls( IDC_ICSDIAGCTRL2, ID2214A );
		dsp_cls( IDC_ICSDIAGCTRL2, ID2214B );
		dsp_cls( IDC_ICSDIAGCTRL2, ID2214C );
		dsp_cls( IDC_ICSDIAGCTRL2, ID2214D );

		f22calq_15();
		f22calq_16();
		f22calq_17();
		memset( WORK0,  '\0', MONY_BUF_SIZE );
		memset( F2218A, '\0', MONY_BUF_SIZE );
		memset( F2218B, '\0', MONY_BUF_SIZE );
		memset( F2218C, '\0', MONY_BUF_SIZE );
		F2218c = 0;

		dsp_prs( IDC_ICSDIAGCTRL2, ID2218A, F2218A );
		dsp_prs( IDC_ICSDIAGCTRL2, ID2218A, F2218B );
		dsp_prs( IDC_ICSDIAGCTRL2, ID2218C, F2218C );
		dsp_prs( IDC_ICSDIAGCTRL2, ID2218D, WORK0 );

		if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
			ModifyDiagInput( ID2215A, DIAG_MDFY_EDIT );
			ModifyDiagInput( ID2215B, DIAG_MDFY_EDIT );
			ModifyDiagInput( ID2215C, DIAG_MDFY_EDIT );
			ModifyDiagInput( ID2216A, DIAG_MDFY_EDIT );
			ModifyDiagInput( ID2216B, DIAG_MDFY_EDIT );
			ModifyDiagInput( ID2216C, DIAG_MDFY_EDIT );
			ModifyDiagInput( ID2217A, DIAG_MDFY_EDIT );
			ModifyDiagInput( ID2217B, DIAG_MDFY_EDIT );
			ModifyDiagInput( ID2217C, DIAG_MDFY_EDIT );
			ChangeColor( IDC_ICSDIAGCTRL2, ID2215A, 0 );
			ChangeColor( IDC_ICSDIAGCTRL2, ID2215B, 0 );
			ChangeColor( IDC_ICSDIAGCTRL2, ID2215C, 0 );
			ChangeColor( IDC_ICSDIAGCTRL2, ID2216A, 0 );
			ChangeColor( IDC_ICSDIAGCTRL2, ID2216B, 0 );
			ChangeColor( IDC_ICSDIAGCTRL2, ID2216C, 0 );
			ChangeColor( IDC_ICSDIAGCTRL2, ID2217A, 0 );
			ChangeColor( IDC_ICSDIAGCTRL2, ID2217B, 0 );
			ChangeColor( IDC_ICSDIAGCTRL2, ID2217C, 0 );

			//[Y]
			if(F2217c & 0x10){//éËì¸óÕ
				ChangeColor( IDC_ICSDIAGCTRL2, ID2217A, 3 );
			}
			if(F2217c & 0x01){//éËì¸óÕ
				ChangeColor( IDC_ICSDIAGCTRL2, ID2217B, 3 );
			}
			if(F2217c & 0x100){//éËì¸óÕ
				ChangeColor( IDC_ICSDIAGCTRL2, ID2217C, 3 );
			}

		}
		ChangeColor( IDC_ICSDIAGCTRL2, ID2218A, 1 );
		ChangeColor( IDC_ICSDIAGCTRL2, ID2218B, 1 );
		ChangeColor( IDC_ICSDIAGCTRL2, ID2218C, 1 );

		ModifyDiagInput( ID2218A, DIAG_MDFY_READONLY );
		ModifyDiagInput( ID2218B, DIAG_MDFY_READONLY );
		ModifyDiagInput( ID2218C, DIAG_MDFY_READONLY );
	}
	else{					// î‰ó·îzï™
		dsp_cls( IDC_ICSDIAGCTRL2, ID2214A );
		dsp_cls( IDC_ICSDIAGCTRL2, ID2214B );
		dsp_cls( IDC_ICSDIAGCTRL2, ID2214C );
		dsp_cls( IDC_ICSDIAGCTRL2, ID2214D );
		f22calq_18();
		memset( F2215A, '\0', MONY_BUF_SIZE );
		memset( F2215B, '\0', MONY_BUF_SIZE );
		memset( F2215C, '\0', MONY_BUF_SIZE );
		memset( F2216A, '\0', MONY_BUF_SIZE );
		memset( F2216B, '\0', MONY_BUF_SIZE );
		memset( F2216C, '\0', MONY_BUF_SIZE );
		memset( F2217A, '\0', MONY_BUF_SIZE );
		memset( F2217B, '\0', MONY_BUF_SIZE );
		memset( F2217C, '\0', MONY_BUF_SIZE );
		F2217c = 0;
		memset( WORK0, '\0', MONY_BUF_SIZE );

		dsp_prs( IDC_ICSDIAGCTRL2, ID2215A, F2215A );
		dsp_prs( IDC_ICSDIAGCTRL2, ID2215A, F2215B );
		dsp_prs( IDC_ICSDIAGCTRL2, ID2215C, F2215C );
		dsp_prs( IDC_ICSDIAGCTRL2, ID2215D, WORK0 );
		dsp_prs( IDC_ICSDIAGCTRL2, ID2216A, F2216A );
		dsp_prs( IDC_ICSDIAGCTRL2, ID2216A, F2216B );
		dsp_prs( IDC_ICSDIAGCTRL2, ID2216C, F2216C );
		dsp_prs( IDC_ICSDIAGCTRL2, ID2216D, WORK0 );
		dsp_prs( IDC_ICSDIAGCTRL2, ID2217A, F2217A );
		dsp_prs( IDC_ICSDIAGCTRL2, ID2217A, F2217B );
		dsp_prs( IDC_ICSDIAGCTRL2, ID2217C, F2217C );
		dsp_prs( IDC_ICSDIAGCTRL2, ID2217D, WORK0 );

		ChangeColor( IDC_ICSDIAGCTRL2, ID2215A, 1 );
		ChangeColor( IDC_ICSDIAGCTRL2, ID2215B, 1 );
		ChangeColor( IDC_ICSDIAGCTRL2, ID2215C, 1 );
		ChangeColor( IDC_ICSDIAGCTRL2, ID2216A, 1 );
		ChangeColor( IDC_ICSDIAGCTRL2, ID2216B, 1 );
		ChangeColor( IDC_ICSDIAGCTRL2, ID2216C, 1 );
		ChangeColor( IDC_ICSDIAGCTRL2, ID2217A, 1 );
		ChangeColor( IDC_ICSDIAGCTRL2, ID2217B, 1 );
		ChangeColor( IDC_ICSDIAGCTRL2, ID2217C, 1 );
		
		ModifyDiagInput( ID2215A, DIAG_MDFY_READONLY );
		ModifyDiagInput( ID2215B, DIAG_MDFY_READONLY );
		ModifyDiagInput( ID2215C, DIAG_MDFY_READONLY );
		ModifyDiagInput( ID2216A, DIAG_MDFY_READONLY );
		ModifyDiagInput( ID2216B, DIAG_MDFY_READONLY );
		ModifyDiagInput( ID2216C, DIAG_MDFY_READONLY );
		ModifyDiagInput( ID2217A, DIAG_MDFY_READONLY );
		ModifyDiagInput( ID2217B, DIAG_MDFY_READONLY );
		ModifyDiagInput( ID2217C, DIAG_MDFY_READONLY );

		if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
			ModifyDiagInput( ID2218A, DIAG_MDFY_EDIT );
			ModifyDiagInput( ID2218B, DIAG_MDFY_EDIT );
			ModifyDiagInput( ID2218C, DIAG_MDFY_EDIT );
			ChangeColor( IDC_ICSDIAGCTRL2, ID2218A, 0 );
			ChangeColor( IDC_ICSDIAGCTRL2, ID2218B, 0 );
			ChangeColor( IDC_ICSDIAGCTRL2, ID2218C, 0 );

			//[Y]
			if(F2218c & 0x10){//éËì¸óÕ
				ChangeColor( IDC_ICSDIAGCTRL2, ID2218A, 3 );
			}
			if(F2218c & 0x01){//éËì¸óÕ
				ChangeColor( IDC_ICSDIAGCTRL2, ID2218B, 3 );
			}
			if(F2218c & 0x100){//éËì¸óÕ
				ChangeColor( IDC_ICSDIAGCTRL2, ID2218C, 3 );
			}
		}
		
	}
}

//-----------------------------------------------------------------------------
// çTèúê≈äzÇÃí≤êÆÅiÇPÇWÅj
//-----------------------------------------------------------------------------
void CShinFhyo22::f22calq_19()
{
	char	WORK0[MONY_BUF_SIZE] = {0};
	m_pArith->l_add( WORK0, F2219A, F2219B );
	m_pArith->l_add( WORK0, WORK0, F2219C );

	dsp_prs( IDC_ICSDIAGCTRL2, ID2219A, F2219A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2219B, F2219B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2219C, F2219C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2219D, WORK0 );
}

//-----------------------------------------------------------------------------
// çTèúê≈äzÇÃí≤êÆÅiÇQÇOÅj
//-----------------------------------------------------------------------------
void CShinFhyo22::f22calq_20()
{
	char	WORK0[MONY_BUF_SIZE] = {0};
	m_pArith->l_add( WORK0, F2220A, F2220B );
	m_pArith->l_add( WORK0, WORK0, F2220C );

	dsp_prs( IDC_ICSDIAGCTRL2, ID2220A, F2220A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2220B, F2220B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2220C, F2220C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2220D, WORK0 );
}

//-----------------------------------------------------------------------------
//	ç∑à¯çTèúédì¸ê≈äzÇÃåvéZ
//-----------------------------------------------------------------------------
void CShinFhyo22::f22calq_2122()
{
	char	va[MONY_BUF_SIZE]={0}, vb[MONY_BUF_SIZE]={0};
	char	WORK0[MONY_BUF_SIZE] = {0};
	char	WORK1[MONY_BUF_SIZE] = {0};

	// ÇRÅì
	if( F2221c & 0x01 ){

		if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID2221A, 3 );	// 2
			ModifyDiagInput( ID2221A, DIAG_MDFY_EDIT );
		}
		memmove( F2221A, (*m_pSnFhyo22Data)->Sn_2F21A, MONY_BUF_SIZE );	//Åö[13'11.21]
		if( m_pArith->l_test(F2221A) < 0 ){
			m_pArith->l_neg( F2221A );
			memmove( F2222A, F2221A, MONY_BUF_SIZE );
			memset( F2221A, '\0', MONY_BUF_SIZE );
		}
		else	{
			memmove( F2221A, (*m_pSnFhyo22Data)->Sn_2F21A, MONY_BUF_SIZE );
			memset( F2222A, '\0', MONY_BUF_SIZE );					//Åö[13'11.21]
		//	l_add(  WORK1, F2219A, F2220A ); 
		//	l_add( F2221A, F2221A, WORK1 );
		}
		// çTèúëŒè€édì¸ê≈äz
		dsp_prs( IDC_ICSDIAGCTRL2, ID2221A, F2221A);
		// çTèúâﬂëÂí≤êÆäz
		dsp_prs( IDC_ICSDIAGCTRL2, ID2222A, F2222A);
	}
	else{
		if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID2221A, 0 );
			ModifyDiagInput( ID2221A, DIAG_MDFY_EDIT );
		}
		memset( F2222A, '\0', MONY_BUF_SIZE );
		m_pArith->l_add( WORK0, F2219A, F2220A );
		memset( va, '\0', MONY_BUF_SIZE );
		if( (*m_pSnHeadData)->IsUseSemiUriRatio() ){
			memmove( va, F2217A, MONY_BUF_SIZE );
		}
		else{
			if( F22UWS >= (*m_pSnHeadData)->m_UriRatioBorder ){
				memmove( va, F2213A, MONY_BUF_SIZE );
			}
			else{
				memmove( va, (*m_pSnHeadData)->IsKobetuSiireAnbun() ? F2217A : F2218A, MONY_BUF_SIZE );
			}
		}
		m_pArith->l_add( va, va, WORK0 );
		if( m_pArith->l_test(va) < 0 ) {
			memset( F2221A, '\0', MONY_BUF_SIZE );
			m_pArith->l_neg(va);
			memmove( F2222A, va, MONY_BUF_SIZE );
		}
		else{
			memset( F2222A, '\0', MONY_BUF_SIZE );
			memmove( F2221A, va, MONY_BUF_SIZE );
		}
		// çTèúëŒè€édì¸ê≈äz
		dsp_prs( IDC_ICSDIAGCTRL2, ID2221A, F2221A );
		// çTèúâﬂëÂí≤êÆäz
		dsp_prs( IDC_ICSDIAGCTRL2, ID2222A, F2222A );
	}

	// ÇSÅì
	if( F2221c & 0x10 ){
		if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID2221B, 3 );		// 2
			ModifyDiagInput( ID2221B, DIAG_MDFY_EDIT );
		}
		memmove( F2221B, (*m_pSnFhyo22Data)->Sn_2F21B, MONY_BUF_SIZE );	//Åö[13'11.21]
		if( m_pArith->l_test(F2221B) < 0 ){
			m_pArith->l_neg( F2221B );
			memmove( F2222B, F2221B, MONY_BUF_SIZE );
			memset( F2221B, '\0', MONY_BUF_SIZE );
		}
		else{
			memmove( F2221B, (*m_pSnFhyo22Data)->Sn_2F21B, MONY_BUF_SIZE );
			memset( F2222B, '\0', MONY_BUF_SIZE );					//Åö[13'11.21]
		//	l_add(  WORK1, F2219B, F2220B ); 
		//	l_add( F2221B, F2221B, WORK1 );
		}
		// çTèúëŒè€édì¸ê≈äz
		dsp_prs( IDC_ICSDIAGCTRL2, ID2221B, F2221B );
		// çTèúâﬂëÂí≤êÆäz
		dsp_prs( IDC_ICSDIAGCTRL2, ID2222B, F2222B);
	}
	else{
		if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID2221B, 0 );
			ModifyDiagInput( ID2221B, DIAG_MDFY_EDIT );
		}
		memset( F2222B, '\0', MONY_BUF_SIZE );
		m_pArith->l_add( WORK1, F2219B, F2220B );
		memset( vb, '\0', MONY_BUF_SIZE );
		if( (*m_pSnHeadData)->IsUseSemiUriRatio() ){
			memmove( vb, F2217B, MONY_BUF_SIZE );
		}
		else{
			if( F22UWS >= (*m_pSnHeadData)->m_UriRatioBorder ){
				memmove( vb, F2213B, MONY_BUF_SIZE );
			}
			else{
				memmove( vb, (*m_pSnHeadData)->IsKobetuSiireAnbun() ? F2217B : F2218B, MONY_BUF_SIZE );
			}
		}
		m_pArith->l_add( vb, vb, WORK1 );
		if( m_pArith->l_test(vb) < 0 ) {
			memset( F2221B, '\0', MONY_BUF_SIZE );
			m_pArith->l_neg( vb );
			memmove( F2222B, vb, MONY_BUF_SIZE );
		}
		else{
			memset( F2222B, '\0', MONY_BUF_SIZE );
			memmove( F2221B, vb, MONY_BUF_SIZE );
		}
		// çTèúëŒè€édì¸ê≈äz
		dsp_prs( IDC_ICSDIAGCTRL2, ID2221B, F2221B);
		// çTèúâﬂëÂí≤êÆäz
		dsp_prs( IDC_ICSDIAGCTRL2, ID2222B, F2222B);
	}

	// F2221C, F2222C ÇWÅìÅIÅIÅIÅI
	if( F2221c & 0x100 ){
		if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID2221C, 3 );		// 2
			ModifyDiagInput( ID2221C, DIAG_MDFY_EDIT );
		}
		memmove( F2221C, (*m_pSnFhyo22Data)->Sn_2F21C, MONY_BUF_SIZE );	//Åö[13'11.21]
		if( m_pArith->l_test(F2221C) < 0 ){
			m_pArith->l_neg( F2221C );
			memmove( F2222C, F2221C, MONY_BUF_SIZE );
			memset( F2221C, '\0', MONY_BUF_SIZE );
		}
		else{
			memmove( F2221C, (*m_pSnFhyo22Data)->Sn_2F21C, MONY_BUF_SIZE );
			memset( F2222C, '\0', MONY_BUF_SIZE );					//Åö[13'11.21]
		//	l_add(  WORK1, F2219B, F2220B ); 
		//	l_add( F2221B, F2221B, WORK1 );
		}
		// çTèúëŒè€édì¸ê≈äz
		dsp_prs( IDC_ICSDIAGCTRL2, ID2221C, F2221C );
		// çTèúâﬂëÂí≤êÆäz
		dsp_prs( IDC_ICSDIAGCTRL2, ID2222C, F2222C);
	}
	else{
		if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID2221C, 0 );
			ModifyDiagInput( ID2221C, DIAG_MDFY_EDIT );
		}
		memset( F2222C, '\0', MONY_BUF_SIZE );
		m_pArith->l_add( WORK1, F2219C, F2220C );
		memset( vb, '\0', MONY_BUF_SIZE );
		if( (*m_pSnHeadData)->IsUseSemiUriRatio() ){
			memmove( vb, F2217C, MONY_BUF_SIZE );
		}
		else{
			if( F22UWS >= (*m_pSnHeadData)->m_UriRatioBorder ){
				memmove( vb, F2213C, MONY_BUF_SIZE );
			}
			else{
				memmove( vb, (*m_pSnHeadData)->IsKobetuSiireAnbun() ? F2217C : F2218C, MONY_BUF_SIZE );
			}
		}
		m_pArith->l_add( vb, vb, WORK1 );
		if( m_pArith->l_test(vb) < 0 ) {
			memset( F2221C, '\0', MONY_BUF_SIZE );
			m_pArith->l_neg( vb );
			memmove( F2222C, vb, MONY_BUF_SIZE );
		}
		else{
			memset( F2222C, '\0', MONY_BUF_SIZE );
			memmove( F2221C, vb, MONY_BUF_SIZE );
		}
		// çTèúëŒè€édì¸ê≈äz
		dsp_prs( IDC_ICSDIAGCTRL2, ID2221C, F2221C);
		// çTèúâﬂëÂí≤êÆäz
		dsp_prs( IDC_ICSDIAGCTRL2, ID2222C, F2222C);
	}

	// çƒï`âÊ
	m_pArith->l_add( WORK0, F2221A, F2221B );
	m_pArith->l_add( WORK0, WORK0, F2221C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2221D, WORK0 );
	m_pArith->l_add( WORK1, F2222A, F2222B );
	m_pArith->l_add( WORK1, WORK1, F2222C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2222D, WORK1 );
}

//-----------------------------------------------------------------------------
// âÒé˚Ç…åWÇÈè¡îÔê≈äz
//-----------------------------------------------------------------------------
void CShinFhyo22::f22calq_23()
{
	char	WORK0[MONY_BUF_SIZE] = {0};
	m_pArith->l_add( WORK0, F2223A, F2223B );
	m_pArith->l_add( WORK0, WORK0, F2223C );

	dsp_prs( IDC_ICSDIAGCTRL2, ID2223A, F2223A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2223B, F2223B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2223C, F2223C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2223D, WORK0 );
}

//-----------------------------------------------------------------------------
// äeì¸óÕÇÃåvéZï\é¶
//-----------------------------------------------------------------------------
// à¯êî	atno	ÅF	
//-----------------------------------------------------------------------------
void CShinFhyo22::f22calq_all( int atno )
{
	while( 1 ){
		switch( atno ){
			case	1 :		// â€ê≈îÑè„äz
				f22calq_1();
			case	2 :		// ñ∆ê≈îÑè„äz
			case	3 :		// îÒâ€ê≈éëéYÇÃóAèo
							// â€ê≈éëéYÇÃè˜ìnìôëŒâøäz
				f22calq_45();
			case	4 :		// îÒâ€ê≈îÑè„äz
							// éëéYÇÃè˜ìnìôëŒâøäzÅEâ€ê≈îÑè„äÑçá
				f22calq_7();
			case	5 :		// â€ê≈édì¸Ç…åWÇÈéxï•ëŒâøäz
				f22calq_8();
			case	6 :		// â€ê≈édì¸Ç…åWÇÈè¡îÔê≈äz
				f22calq_9();
			case	7 :		// â€ê≈â›ï®Ç…åWÇÈè¡îÔê≈äz
							// ñ∆ê≈ÅÃâ€ê≈í≤êÆê≈äz
							// â€ê≈édì¸ìôÇÃê≈äzçáåv
				f22calq_11();				
				f22calq_12();				
				f22calq_13();				
			case	8 :	
			case	9 :
			case	10:
			case	11:
				if( (*m_pSnHeadData)->IsUseSemiUriRatio() ){
					// â€ê≈îÑè„äÑçáÇ…èÄÇ∏ÇÈäÑçáÇóLå¯
					f22calq_ko();			// â€ê≈îÑè„äÑçáÇ™ÇXÇTÅìñ¢ñû
				}
				else{
					if( F22UWS >= (*m_pSnHeadData)->m_UriRatioBorder ){
						// â€ê≈îÑè„äÑçáÇ™ÇXÇTÅìà»è„
						f22calq_14();
					}
					else{
						// â€ê≈îÑè„äÑçáÇ™ÇXÇTÅìñ¢ñû
						f22calq_ko();	
					}
				}
//--> '15.04.04 INS START
				// ì¡íËé˚ì¸Çâ¡ñ°ÇµÇΩçTèúëŒè€édì¸ê≈äzÇÃåvéZ('15.04.04)
				f22calq_spc();
//<-- '15.04.04 INS END
			case	12:		// çTèúê≈äzÇÃí≤êÆÅiÇPÇXÅj
				f22calq_19();
			case	13:		// çTèúê≈äzÇÃí≤êÆÅiÇQÇOÅj
							// ç∑à¯çTèúê≈äz
				f22calq_20();				
				f22calq_2122();				
			case	14:		// ë›ì|âÒé˚Ç…åWÇÈè¡îÔê≈äz
				f22calq_23();
			default	:
				break;
		}
		break;
	}
}

//-----------------------------------------------------------------------------
// ì¡íËé˚ì¸Çâ¡ñ°ÇµÇΩçTèúëŒè€édì¸ê≈äzÇÃåvéZ('15.04.04)
//-----------------------------------------------------------------------------
void CShinFhyo22::f22calq_spc()
{
	if( *(m_pShinInfo->pSgSpc) == 1 ){

//--> '15.05.18 INS START
		unsigned char svSn_Sign2 = (*m_pSnHeadData)->Sn_Sign2;
//<-- '15.05.18 INS END

		SYZTOSPC_DATA	SyzToSpc = {0};
		BOOL			IsRatioOver95 = FALSE;
		if( F22UWS >= (*m_pSnHeadData)->m_UriRatioBorder ){
			IsRatioOver95 = TRUE;
			SyzToSpc.IsRatioOver95 |= 0x01;
		}

		// â€ê≈îÑè„ÉZÉbÉg
		m_pArith->l_add( SyzToSpc.SpKzur, F221A, F221B );
		m_pArith->l_add( SyzToSpc.SpKzur, SyzToSpc.SpKzur, F221C );
		m_pArith->l_add( SyzToSpc.SpMzur, SyzToSpc.SpMzur, F222C );
		m_pArith->l_add( SyzToSpc.SpMzur, SyzToSpc.SpMzur, F223C );
		m_pArith->l_add( SyzToSpc.SpHkur, F226A, F226B );
		m_pArith->l_add( SyzToSpc.SpHkur, SyzToSpc.SpHkur, F226C );
//2017.03.01 INSERT START
		memmove( SyzToSpc.SpKzur4, F221B, sizeof( SyzToSpc.SpKzur4 ));
		memmove( SyzToSpc.SpKzur63, F221C, sizeof( SyzToSpc.SpKzur63 ));
//2017.03.01 INSERT END

		// çTèúëŒè€édì¸ê≈äzÉZÉbÉg
		if( IsRatioOver95 ){
			//== åvéZï\ÇTÅiÇPÅj ==
			memmove( SyzToSpc.SpKgz3, F2213A, sizeof(SyzToSpc.SpKgz3) );
			memmove( SyzToSpc.SpKgz4, F2213B, sizeof(SyzToSpc.SpKgz4) );
			memmove( SyzToSpc.SpKgz63, F2213C, sizeof(SyzToSpc.SpKgz63) );
		}
		if( (*m_pSnHeadData)->IsKobetuSiireAnbun() && (IsRatioOver95==FALSE) ){
			//== åvéZï\ÇTÅiÇQÅj ==
			memmove( SyzToSpc.SpKgz3, F2217A, sizeof(SyzToSpc.SpKgz3) );
			memmove( SyzToSpc.SpKgz4, F2217B, sizeof(SyzToSpc.SpKgz4) );
			memmove( SyzToSpc.SpKgz63, F2217C, sizeof(SyzToSpc.SpKgz63) );
		}
		if( ((*m_pSnHeadData)->IsKobetuSiireAnbun()==FALSE) && (IsRatioOver95==FALSE) ){
			//== åvéZï\ÇTÅiÇRÅj ==
			memmove( SyzToSpc.SpKgz3, F2218A, sizeof(SyzToSpc.SpKgz3) );
			memmove( SyzToSpc.SpKgz4, F2218B, sizeof(SyzToSpc.SpKgz4) );
			memmove( SyzToSpc.SpKgz63, F2218C, sizeof(SyzToSpc.SpKgz63) );
		}
		// çƒåvéZ
		((CH26HyoView*)m_pParent)->RecalqSkj( &SyzToSpc );
		// ã‡äzçƒÉZÉbÉg
		if( (*m_pSnHeadData)->Sn_Sign2&0x02 ){
//-- '15.05.18 --
//			F2221c &= ~0x111;
//---------------
			if( !(svSn_Sign2&0x02) ){
				F2221c &= ~0x111;
			}
//---------------
		}
		else{
			memmove( F2221A, SyzToSpc.SpSiz3, sizeof(F2221A) );
			memmove( F2221B, SyzToSpc.SpSiz4, sizeof(F2221B) );
			memmove( F2221C, SyzToSpc.SpSiz63, sizeof(F2221C) );
			memmove( (*m_pSnFhyo22Data)->Sn_2F21A, SyzToSpc.SpSiz3, sizeof((*m_pSnFhyo22Data)->Sn_2F21A) );
			memmove( (*m_pSnFhyo22Data)->Sn_2F21B, SyzToSpc.SpSiz4, sizeof((*m_pSnFhyo22Data)->Sn_2F21B) );
			memmove( (*m_pSnFhyo22Data)->Sn_2F21C, SyzToSpc.SpSiz63, sizeof((*m_pSnFhyo22Data)->Sn_2F21C) );
			F2221c |= 0x111;
		}
	}
}
