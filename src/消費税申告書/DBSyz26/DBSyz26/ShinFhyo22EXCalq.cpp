
#include "stdafx.h"
#include "ShinFhyo22EX.h"
#include "ShinFhyo22EXIdx.h"
#include "H26HyoView.h"	// '15.04.04

//-----------------------------------------------------------------------------
// ‰ÛÅ”„ãŠz
//-----------------------------------------------------------------------------
void CShinFhyo22EX::f22calq_1()
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
// ‰ÛÅŽ‘ŽY‚Ì÷“n“™‚Ì‘Î‰¿‚ÌŠz
//-----------------------------------------------------------------------------
void CShinFhyo22EX::f22calq_45()
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
// ”ñ‰ÛÅ”„ãŠz
//-----------------------------------------------------------------------------
void CShinFhyo22EX::f22calq_6()
{
	char	WORK0[MONY_BUF_SIZE] = {0};
	m_pArith->l_add( WORK0, F226A, F226B );
	m_pArith->l_add( WORK0, WORK0, F226C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID226C, WORK0 );
	memset( F226A, '\0', MONY_BUF_SIZE );
	memset( F226B, '\0', MONY_BUF_SIZE );
}

//-----------------------------------------------------------------------------
// Ž‘ŽY‚Ì÷“n“™‚Ì‘Î‰¿‚ÌŠz ¥ ‰ÛÅ”„ãŠ„‡
//-----------------------------------------------------------------------------
void CShinFhyo22EX::f22calq_7()
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

	char	PER100BUF[MONY_BUF_SIZE] = {0};
	m_pArith->l_input( PER100BUF, _T("10000") );

	DIAGRAM_DATA		DiagData;
	DiagData.data_imgdata = NULL;

	if( !m_DpSw ){	// ÄWŒv‚Ì‚Ýi•\Ž¦–³‚µj
		// ‰ÛÅ”„ã‚Š„‡
		DiagData.data_check = 0;
		diag_setdata( IDC_ICSDIAGCTRL2, ID227EX, (struct IUnknown *)&DiagData, CIcsdiagctrl );
		ChangeColor( IDC_ICSDIAGCTRL2, ID22EX, 1 );
		diag_modify( IDC_ICSDIAGCTRL2, ID22EX, DIAG_MDFY_READONLY, CIcsdiagctrl );
		memset( buf, '\0', sizeof( buf ) );
		memset( WORK0, '\0', MONY_BUF_SIZE );
		if( !((*m_pSnHeadData)->Sn_EXP100&0x01) ){
			if( *m_pShinInfo->pSgSpc ){
				// “Á’èŽû“ü—L‚è [08'11.13]
				m_pArith->l_par100( (unsigned char *)WORK0, (unsigned char *)F224C, (unsigned char *)F227C, 2, 0 );

				// ”„ãŠ„‡‚ªƒ}ƒCƒiƒX‚Ìê‡‚Í‚O“‚ÅŒvŽZ‚·‚é
				memset( PW6, '\0', sizeof( PW6 ) );
				if( m_pArith->l_cmp(F224C, PW6) < 0 ){
					memset( WORK0, '\0', MONY_BUF_SIZE );
				}
				if( m_pArith->l_cmp(F227C, PW6) < 0 ){
					memset( WORK0, '\0', MONY_BUF_SIZE );
				}
				if( m_pArith->l_cmp(WORK0, PER100BUF) > 0 ){
					memmove( WORK0, PER100BUF, sizeof(WORK0) );
				}
				ChangeColor( IDC_ICSDIAGCTRL2, ID227EX, 1 );
				diag_modify( IDC_ICSDIAGCTRL2, ID227EX, DIAG_MDFY_READONLY, CIcsdiagctrl );
			}
			else{
				if( (*m_pSnHeadData)->Sn_UPERS&0x10 ){
					// ‰ÛÅ”„ãŠ„‡@Žè“ü—Í
					DiagData.data_check = 1;
					diag_setdata( IDC_ICSDIAGCTRL2, ID227EX, (struct IUnknown *)&DiagData, CIcsdiagctrl );
					sprintf_s( buf, sizeof(buf), _T("%d"), (*m_pSnFhyo22Data)->Sn_2FUWSEX );
					m_pArith->l_input( WORK0, buf );
					if( (*m_pSnHeadData)->IsUseSemiUriRatio() ){
						memset( buf, '\0', sizeof(buf) );
						strcpy_s( buf, sizeof(buf), _T("‰Û@Å@”„@ã@Š„@‡@@(4/7)\ny‰ÛÅ”„ãŠ„‡‚É€‚¸‚éŠ„‡‚ð“K—pz") );
						DiagData.data_disp = buf;
						diag_setdata( IDC_ICSDIAGCTRL2, 18, (struct IUnknown *)&DiagData, CIcsdiagctrl );
						ChangeColor( IDC_ICSDIAGCTRL2, ID22EX, 1 );
					}
					else{
						memset( buf, '\0', sizeof(buf) );
						strcpy_s( buf, sizeof(buf), _T("‰Û@Å@”„@ã@Š„@‡@@(4/7)") );
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
					
					// ”„ãŠ„‡‚ªƒ}ƒCƒiƒX‚Ìê‡‚Í‚O“‚ÅŒvŽZ‚·‚é
					memset( PW6, '\0', sizeof(PW6) );
					if( m_pArith->l_cmp(F224C, PW6) < 0 ){
						memset( WORK0, '\0', MONY_BUF_SIZE );
					}
					if( m_pArith->l_cmp(F227C, PW6) < 0 ){
						memset( WORK0, '\0', MONY_BUF_SIZE );
					}
					if( m_pArith->l_cmp(WORK0, PER100BUF) > 0 ){
						memmove( WORK0, PER100BUF, sizeof(WORK0) );
					}
					memset( buf, '\0', sizeof(buf) );
					strcpy_s( buf, sizeof(buf), _T("‰Û@Å@”„@ã@Š„@‡@@(4/7)") );
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
		// ŒvŽZ‚Ì‚Ý
		memset( buf, '\0', sizeof( buf ) );
		memset( WORK0, '\0', MONY_BUF_SIZE );
		if( !((*m_pSnHeadData)->Sn_EXP100&0x01) ){
			// ‰ÛÅ”„ãŠ„‡
			if( *m_pShinInfo->pSgSpc ){
				// “Á’èŽû“ü—L‚è@[08'11.13]
				m_pArith->l_par100( (unsigned char *)WORK0, (unsigned char *)F224C, (unsigned char *)F227C, 2, 0 );
				// ”„ãŠ„‡‚ªƒ}ƒCƒiƒX‚Ìê‡‚Í‚O“‚ÅŒvŽZ‚·‚éB[03'05.17]
				memset( PW6, '\0', sizeof( PW6 ) );
				if( m_pArith->l_cmp(F224C, PW6) < 0 ){
					memset( WORK0, '\0', MONY_BUF_SIZE );
				}
				if( m_pArith->l_cmp(F227C, PW6) < 0 ){
					memset( WORK0, '\0', MONY_BUF_SIZE );
				}
				if( m_pArith->l_cmp(WORK0, PER100BUF) > 0 ){
					memmove( WORK0, PER100BUF, sizeof(WORK0) );
				}
			}
			else{
				if( (*m_pSnHeadData)->Sn_UPERS&0x10 ){
					sprintf_s( buf, sizeof( buf ), _T("%d"), (*m_pSnFhyo22Data)->Sn_2FUWSEX );
					m_pArith->l_input( WORK0, buf );
				}
				else{
					// ‰ÛÅ”„ãŠ„‡
					m_pArith->l_par100( (unsigned char *)WORK0, (unsigned char *)F224C, (unsigned char *)F227C, 2, 0 );
					// ”„ãŠ„‡‚ªƒ}ƒCƒiƒX‚Ìê‡‚Í‚O“‚ÅŒvŽZ‚·‚éB[03'05.17]
					memset( PW6, '\0', sizeof(PW6) );
					if( m_pArith->l_cmp(F224C, PW6) < 0 ){
						memset( WORK0, '\0', MONY_BUF_SIZE );
					}
					if( m_pArith->l_cmp(F227C, PW6) < 0 ){
						memset( WORK0, '\0', MONY_BUF_SIZE );
					}
					if( m_pArith->l_cmp(WORK0, PER100BUF) > 0 ){
						memmove( WORK0, PER100BUF, sizeof(WORK0) );
					}
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
// ‰ÛÅŽd“ü‚ÉŒW‚éŽx•¥‘Î‰¿‚ÌŠz
//-----------------------------------------------------------------------------
void CShinFhyo22EX::f22calq_8()
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
// ‰ÛÅŽd“ü‚ÉŒW‚éÁ”ïÅŠz
//-----------------------------------------------------------------------------
void CShinFhyo22EX::f22calq_9()
{
	char	WORK0[MONY_BUF_SIZE] = {0};

	if(!((*m_pSnHeadData)->Sn_Sign4&0x80) ){	//˜A“®‚Ìê‡WŒv‚µ‚È‚¢BŽ©“®WŒv’l‚ðŽg—p‚·‚éˆ×
		
	}
	else{
		// 3%
		if( !(F229c&0x80) ){
			if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){//[Y]
				ChangeColor( IDC_ICSDIAGCTRL2, ID229A, 0 );	
			}
			if( F229c&0x10 ){
				memset( F229A, '\0', MONY_BUF_SIZE );
				m_Util.percent( F229A, F228A, 0, 30, 0 );
			}
		}
		// 4%
		if( !(F229c&0x08) ){
			if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){//[Y]
				ChangeColor( IDC_ICSDIAGCTRL2, ID229B, 0 );	
			}
			if( F229c&0x01 ){
				memset( F229B, '\0', MONY_BUF_SIZE );
				m_Util.percent( F229B, F228B, 4, 40, 0 );
			}
		}
		// 6.3%
		if( !(F229c&0x800) ){
			if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){//[Y]
				ChangeColor( IDC_ICSDIAGCTRL2, ID229C, 0 );	
			}
			if( F229c&0x100 ){
				memset( F229C, '\0', MONY_BUF_SIZE );
				m_Util.percent( F229C, F228C, 9, 63, 0 );
			}
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
			ChangeColor( IDC_ICSDIAGCTRL2, ID229A, 3 );	// 2
		}
		// 4%
		if( (F229c&0x08) ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID229B, 3 );	// 2
		}
		// 6.3%
		if( (F229c&0x800) ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID229C, 3 );	// 2
		}
	}

	F229c &= ~(0x10|0x01|0x100);

}
//-----------------------------------------------------------------------------
// “Á’è‰ÛÅŽd“ü‚ê‚ÉŒW‚éŽx•¥‘Î‰¿‚ÌŠz
//-----------------------------------------------------------------------------
void CShinFhyo22EX::f22calq_10()
{
	char	WORK0[MONY_BUF_SIZE] = {0};

	m_pArith->l_add( WORK0, WORK0, F2210C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2210C, F2210C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2210D, WORK0 );
}

//-----------------------------------------------------------------------------
// “Á’è‰ÛÅŽd“ü‚ê‚ÉŒW‚éÁ”ïÅŠz
//-----------------------------------------------------------------------------
void CShinFhyo22EX::f22calq_11()
{
	char	WORK0[MONY_BUF_SIZE] = {0};

	if(!((*m_pSnHeadData)->Sn_Sign4&0x80) ){	//˜A“®‚Ìê‡WŒv‚µ‚È‚¢BŽ©“®WŒv’l‚ðŽg—p‚·‚éˆ×
		
	}
	else{
		// 6.3%
		if( !(F2211c&0x800) ){
			if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){//[Y]
//-- '15.08.21 --
//				ChangeColor( IDC_ICSDIAGCTRL2, ID2211C, 0 );
//---------------
				if( (*m_pSnHeadData)->IsNeedTkkzDisp() ){
					ChangeColor( IDC_ICSDIAGCTRL2, ID2211C, 0 );
				}
//---------------
			}
			if( F2211c&0x100 ){
				memset( F2211C, '\0', MONY_BUF_SIZE );
				m_Util.percent( F2211C, F2210C, 1, 63, 0 );
			}
		}
	}
	m_pArith->l_add( WORK0, WORK0, F2211C );

	dsp_prs( IDC_ICSDIAGCTRL2, ID2211C, F2211C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2211D, WORK0 );


	//[Y]
	if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
		// 6.3%
		if( (F2211c&0x800) ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID2211C, 3 );	// 2
		}
	}

	F2211c &= ~(0x10|0x01|0x100);

}

//-----------------------------------------------------------------------------
// ‰ÛÅ‰Ý•¨‚ÉŒW‚éÁ”ïÅŠz
//-----------------------------------------------------------------------------
//void CShinFhyo22EX::f22calq_11()
void CShinFhyo22EX::f22calq_13()
{
	char	WORK0[MONY_BUF_SIZE] = {0};

	m_pArith->l_add( WORK0, F2213A, F2213B );
	m_pArith->l_add( WORK0, WORK0, F2213C );

	dsp_prs( IDC_ICSDIAGCTRL2, ID2213A, F2213A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2213B, F2213B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2213C, F2213C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2213D, WORK0 );
}

//-----------------------------------------------------------------------------
// ‰ÛÅŽ–‹ÆÌ–ÆÅŽ–‹Æ ’²®ÅŠz
//-----------------------------------------------------------------------------
//void CShinFhyo22EX::f22calq_12()
void CShinFhyo22EX::f22calq_14()
{
	char	WORK0[MONY_BUF_SIZE] = {0};
	m_pArith->l_add( WORK0, F2214A, F2214B );
	m_pArith->l_add( WORK0, WORK0, F2214C );

	dsp_prs( IDC_ICSDIAGCTRL2, ID2214A, F2214A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2214B, F2214B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2214C, F2214C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2214D, WORK0 );
}

//-----------------------------------------------------------------------------
// ‰ÛÅŽd“ü“™‚ÌÅŠz‚Ì‡ŒvŠz
//-----------------------------------------------------------------------------
//void CShinFhyo22EX::f22calq_13()
void CShinFhyo22EX::f22calq_15()
{
	m_pArith->l_add( F2215A, F229A,  F2213A );
	m_pArith->l_add( F2215A, F2215A, F2214A );
	m_pArith->l_add( F2215B, F229B,  F2213B );
	m_pArith->l_add( F2215B, F2215B, F2214B );
	m_pArith->l_add( F2215C, F229C,  F2213C );
	if( (*m_pSnHeadData)->IsNeedTkkzDisp() ){
		m_pArith->l_add( F2215C, F2215C, F2211C );
	}
	m_pArith->l_add( F2215C, F2215C, F2214C );

	char	WORK0[MONY_BUF_SIZE] = {0};
	m_pArith->l_add( WORK0, F2215A, F2215B );
	m_pArith->l_add( WORK0, WORK0, F2215C );

	dsp_prs( IDC_ICSDIAGCTRL2, ID2215A, F2215A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2215B, F2215B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2215C, F2215C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2215D, WORK0 );
}

//-----------------------------------------------------------------------------
// ‰ÛÅ”„ãŠ„‡‚ª‚X‚T“ˆÈã‚Ìê‡
//-----------------------------------------------------------------------------
//void CShinFhyo22EX::f22calq_14()
void CShinFhyo22EX::f22calq_16()
{
	char	WORK0[MONY_BUF_SIZE] = {0};
	m_pArith->l_add( WORK0, F2215A, F2215B );
	m_pArith->l_add( WORK0, WORK0, F2215C );

	dsp_prs( IDC_ICSDIAGCTRL2, ID2216A, F2215A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2216B, F2215B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2216C, F2215C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2216D, WORK0 );

	memset( F2217A, '\0', MONY_BUF_SIZE );
	memset( F2217B, '\0', MONY_BUF_SIZE );
	memset( F2217C, '\0', MONY_BUF_SIZE );

	memset( F2218A, '\0', MONY_BUF_SIZE );
	memset( F2218B, '\0', MONY_BUF_SIZE );
	memset( F2218C, '\0', MONY_BUF_SIZE );

	memset( F2219A, '\0', MONY_BUF_SIZE );
	memset( F2219B, '\0', MONY_BUF_SIZE );
	memset( F2219C, '\0', MONY_BUF_SIZE );
	F2219c = 0;
	memset( F2220A, '\0', MONY_BUF_SIZE );
	memset( F2220B, '\0', MONY_BUF_SIZE );
	memset( F2220C, '\0', MONY_BUF_SIZE );
	F2220c = 0;
	memset( WORK0, '\0', MONY_BUF_SIZE );

	dsp_prs( IDC_ICSDIAGCTRL2, ID2217A, F2217A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2217B, F2217B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2217C, F2217C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2217D, WORK0 );

	dsp_prs( IDC_ICSDIAGCTRL2, ID2218A, F2218A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2218B, F2218B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2218C, F2218C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2218D, WORK0 );

	dsp_prs( IDC_ICSDIAGCTRL2, ID2219A, F2219A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2219B, F2219B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2219C, F2219C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2219D, WORK0 );

	dsp_prs( IDC_ICSDIAGCTRL2, ID2220A, F2220A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2220B, F2220B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2220C, F2220C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2220D, WORK0 );

	// ŠY“–€–Ú“ü—Í•s‰Â
	// ‰ÛÅ”„ã‚Š„‡‚ª95%ˆÈã
	ChangeColor( IDC_ICSDIAGCTRL2, ID2217A, 1 );
	ChangeColor( IDC_ICSDIAGCTRL2, ID2217B, 1 );
	ChangeColor( IDC_ICSDIAGCTRL2, ID2217C, 1 );

	ChangeColor( IDC_ICSDIAGCTRL2, ID2218A, 1 );
	ChangeColor( IDC_ICSDIAGCTRL2, ID2218B, 1 );
	ChangeColor( IDC_ICSDIAGCTRL2, ID2218C, 1 );

	ChangeColor( IDC_ICSDIAGCTRL2, ID2219A, 1 );
	ChangeColor( IDC_ICSDIAGCTRL2, ID2219B, 1 );
	ChangeColor( IDC_ICSDIAGCTRL2, ID2219C, 1 );

	ChangeColor( IDC_ICSDIAGCTRL2, ID2220A, 1 );
	ChangeColor( IDC_ICSDIAGCTRL2, ID2220B, 1 );
	ChangeColor( IDC_ICSDIAGCTRL2, ID2220C, 1 );

	ModifyDiagInput( ID2217A, DIAG_MDFY_READONLY );
	ModifyDiagInput( ID2217B, DIAG_MDFY_READONLY );
	ModifyDiagInput( ID2217C, DIAG_MDFY_READONLY );

	ModifyDiagInput( ID2218A, DIAG_MDFY_READONLY );
	ModifyDiagInput( ID2218B, DIAG_MDFY_READONLY );
	ModifyDiagInput( ID2218C, DIAG_MDFY_READONLY );

	ModifyDiagInput( ID2219A, DIAG_MDFY_READONLY );
	ModifyDiagInput( ID2219B, DIAG_MDFY_READONLY );
	ModifyDiagInput( ID2219C, DIAG_MDFY_READONLY );

	ModifyDiagInput( ID2220A, DIAG_MDFY_READONLY );
	ModifyDiagInput( ID2220B, DIAG_MDFY_READONLY );
	ModifyDiagInput( ID2220C, DIAG_MDFY_READONLY );
}

//-----------------------------------------------------------------------------
// ‰ÛÅ”„ã‚É‚Ì‚Ý—v‚·‚éŽd“üÅŠz
//-----------------------------------------------------------------------------
//void CShinFhyo22EX::f22calq_15()
void CShinFhyo22EX::f22calq_17()
{
	char	WORK0[MONY_BUF_SIZE] = {0};
	m_pArith->l_add( WORK0, F2217A, F2217B );
	m_pArith->l_add( WORK0, WORK0, F2217C );

	dsp_prs( IDC_ICSDIAGCTRL2, ID2217A, F2217A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2217B, F2217B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2217C, F2217C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2217D, WORK0 );
}

//-----------------------------------------------------------------------------
// ‹¤’Ê‚Ì”„ã‚É—v‚·‚éŽd“üÅŠz
//-----------------------------------------------------------------------------
//void CShinFhyo22EX::f22calq_16()
void CShinFhyo22EX::f22calq_18()
{
	char	WORK0[MONY_BUF_SIZE] = {0};
	m_pArith->l_add( WORK0, F2218A, F2218B );
	m_pArith->l_add( WORK0, WORK0, F2218C );

	dsp_prs( IDC_ICSDIAGCTRL2, ID2218A, F2218A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2218B, F2218B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2218C, F2218C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2218D, WORK0 );
}

//-----------------------------------------------------------------------------
// ŒÂ•Ê‘Î‰ž•ûŽ®TœŽd“üÅŠz
//-----------------------------------------------------------------------------
//void CShinFhyo22EX::f22calq_17()
void CShinFhyo22EX::f22calq_19()
{
	char	WORK0[MONY_BUF_SIZE] = {0};
	char	PW6[MONY_BUF_SIZE] = {0};
	char	buf[512] = {0};

	char	PER100BUF[MONY_BUF_SIZE] = {0};
	m_pArith->l_input( PER100BUF, _T("10000000") );
	
	if( !(F2219c & 0x100) ){	// Žè“ü—Í‚È‚µ
		memset( F2219C, '\0', MONY_BUF_SIZE );
		if( (*m_pSnHeadData)->Sn_UPERS & 0x10 ){

			//[Y]
			sprintf_s( buf, sizeof( buf ), _T("%d"), (*m_pSnFhyo22Data)->Sn_2FUWSEX );
			m_pArith->l_input( WORK0, buf );

			m_pArith->l_mul100( (unsigned char *)F2219C, (unsigned char *)F2218C, (unsigned char *)WORK0, 2, 0 );
		}
		else{
			m_pArith->l_par100( (unsigned char *)WORK0, (unsigned char *)F224C,  (unsigned char *)F227C,  5, 0 );

			// ”„ãŠ„‡‚ªƒ}ƒCƒiƒX‚Ìê‡‚Í‚O“‚ÅŒvŽZ‚·‚éB
			memset( PW6, '\0', sizeof(PW6) );
			if( m_pArith->l_cmp(F224C, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}
			if( m_pArith->l_cmp(F227C, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}

			if( m_pArith->l_test(WORK0) == 0 ){
				m_pArith->l_mul100( (unsigned char *)F2219C, (unsigned char *)F2218C, (unsigned char *)WORK0, 5, 0 );
			}
			else{
				if(  m_pArith->l_cmp(WORK0, PER100BUF) <= 0 ){
					m_Util.l_pardotnet_syz( F2219C, F2218C, F224C, F227C );
				}
				else{
					m_Util.l_pardotnet_syz( F2219C, F2218C, F224C, F224C );
				}
			}
		}
		m_pArith->l_add( F2219C, F2219C, F2217C );
	}
	if( !(F2219c & 0x01) ){		// Žè“ü—Í‚È‚µ
		memset( F2219B, '\0', MONY_BUF_SIZE );
		if( (*m_pSnHeadData)->Sn_UPERS & 0x10 ){
			sprintf_s( buf, sizeof( buf ), _T("%d"), (*m_pSnFhyo22Data)->Sn_2FUWSEX );
			m_pArith->l_input( WORK0, buf );
			m_pArith->l_mul100( (unsigned char *)F2219B, (unsigned char *)F2218B, (unsigned char *)WORK0, 2, 0 );
		}
		else{
			m_pArith->l_par100( (unsigned char *)WORK0, (unsigned char *)F224C,  (unsigned char *)F227C,  5, 0 );

			// ”„ãŠ„‡‚ªƒ}ƒCƒiƒX‚Ìê‡‚Í‚O“‚ÅŒvŽZ‚·‚éB
			memset( PW6, '\0', sizeof(PW6) );
			if( m_pArith->l_cmp(F224C, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}
			if( m_pArith->l_cmp(F227C, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}

			if( m_pArith->l_test(WORK0) == 0 ){
				m_pArith->l_mul100( (unsigned char *)F2219B, (unsigned char *)F2218B, (unsigned char *)WORK0, 5, 0 );
			}
			else{
				if(  m_pArith->l_cmp(WORK0, PER100BUF) <= 0 ){
					m_Util.l_pardotnet_syz( F2219B, F2218B, F224C, F227C );
				}
				else{
					m_Util.l_pardotnet_syz( F2219B, F2218B, F224C, F224C );
				}
			}
		}		
		m_pArith->l_add( F2219B, F2219B, F2217B );
	}
	if( !(F2219c & 0x10) ){	// Žè“ü—Í‚È‚µ
		memset( F2219A, '\0', MONY_BUF_SIZE );
		if( (*m_pSnHeadData)->Sn_UPERS & 0x10 ){
			//[Y]
			sprintf_s( buf, sizeof( buf ), _T("%d"), (*m_pSnFhyo22Data)->Sn_2FUWSEX );
			m_pArith->l_input( WORK0, buf );
			
			m_pArith->l_mul100( (unsigned char *)F2219A, (unsigned char *)F2218A, (unsigned char *)WORK0, 2, 0 );
		}
		else{
			m_pArith->l_par100( (unsigned char *)WORK0, (unsigned char *)F224C,  (unsigned char *)F227C,  5, 0 );

			// ”„ãŠ„‡‚ªƒ}ƒCƒiƒX‚Ìê‡‚Í‚O“‚ÅŒvŽZ‚·‚éB
			memset( PW6, '\0', sizeof(PW6) );
			if( m_pArith->l_cmp(F224C, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}
			if( m_pArith->l_cmp(F227C, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}

			if( m_pArith->l_test(WORK0) == 0 ){
				m_pArith->l_mul100( (unsigned char *)F2219A, (unsigned char *)F2218A, (unsigned char *)WORK0, 5, 0 );
			}
			else{
				if(  m_pArith->l_cmp(WORK0, PER100BUF) <= 0 ){
					m_Util.l_pardotnet_syz( F2219A, F2218A, F224C, F227C );
				}
				else{
					m_Util.l_pardotnet_syz( F2219A, F2218A, F224C, F224C );
				}
			}
		}
		m_pArith->l_add( F2219A, F2219A, F2217A );
	}
	
	m_pArith->l_add( WORK0, F2219A, F2219B );
	m_pArith->l_add( WORK0, WORK0, F2219C );

	dsp_prs( IDC_ICSDIAGCTRL2, ID2219A, F2219A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2219B, F2219B);
	dsp_prs( IDC_ICSDIAGCTRL2, ID2219C, F2219C);
	dsp_prs( IDC_ICSDIAGCTRL2, ID2219D, WORK0  );
}

//-----------------------------------------------------------------------------
// ”ä—á”z•ª•ûŽ®TœŽd“üÅŠz
//-----------------------------------------------------------------------------
//void CShinFhyo22EX::f22calq_18()
void CShinFhyo22EX::f22calq_20()
{
	char	WORK0[MONY_BUF_SIZE] = {0};
	char	PW6[MONY_BUF_SIZE] = {0};
	char	buf[512] = {0};

	char	PER100BUF[MONY_BUF_SIZE] = {0};
	m_pArith->l_input( PER100BUF, _T("10000000") );
	
	if( !(F2220c & 0x01) ){	// Žè“ü—Í‚È‚µ
		memset( F2220B, '\0', MONY_BUF_SIZE );
		if( (*m_pSnHeadData)->Sn_UPERS & 0x10 ){
			sprintf_s( buf, sizeof( buf ), _T("%d"), (*m_pSnFhyo22Data)->Sn_2FUWSEX );
			m_pArith->l_input( WORK0, buf );
			m_pArith->l_mul100( (unsigned char *)F2220B, (unsigned char *)F2215B, (unsigned char *)WORK0, 2, 0 );	// [05'06.06]
		}
		else{
			m_pArith->l_par100( (unsigned char *)WORK0, (unsigned char *)F224C,  (unsigned char *)F227C,  5, 0 );
			// ”„ãŠ„‡‚ªƒ}ƒCƒiƒX‚Ìê‡‚Í‚O“‚ÅŒvŽZ‚·‚éB
			memset( PW6, '\0', sizeof( PW6 ) );
			if( m_pArith->l_cmp(F224C, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}
			if( m_pArith->l_cmp(F227C, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}

			if( m_pArith->l_test(WORK0) == 0 ){
				m_pArith->l_mul100( (unsigned char *)F2220B, (unsigned char *)F2215B, (unsigned char *)WORK0, 5, 0 );
			}
			else{
				if( m_pArith->l_cmp(WORK0, PER100BUF) <= 0 ){
					m_Util.l_pardotnet_syz( F2220B, F2215B, F224C, F227C );
				}
				else{
					m_Util.l_pardotnet_syz( F2220B, F2215B, F224C, F224C );
				}
			}
		}
	}

	if( !(F2220c & 0x100) ){	// Žè“ü—Í‚È‚µ
		memset( F2220C, '\0', MONY_BUF_SIZE );
		if( (*m_pSnHeadData)->Sn_UPERS & 0x10 ){
			sprintf_s( buf, sizeof( buf ), _T("%d"), (*m_pSnFhyo22Data)->Sn_2FUWSEX );
			m_pArith->l_input( WORK0, buf );
			m_pArith->l_mul100( (unsigned char *)F2220C, (unsigned char *)F2215C, (unsigned char *)WORK0, 2, 0 );	// [05'06.06]
		}
		else{
			m_pArith->l_par100( (unsigned char *)WORK0, (unsigned char *)F224C,  (unsigned char *)F227C,  5, 0 );
			// ”„ãŠ„‡‚ªƒ}ƒCƒiƒX‚Ìê‡‚Í‚O“‚ÅŒvŽZ‚·‚éB
			memset( PW6, '\0', sizeof( PW6 ) );
			if( m_pArith->l_cmp(F224C, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}
			if( m_pArith->l_cmp(F227C, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}

			if( m_pArith->l_test(WORK0) == 0 ){
				m_pArith->l_mul100( (unsigned char *)F2220C, (unsigned char *)F2215C, (unsigned char *)WORK0, 5, 0 );
			}
			else{
				if( m_pArith->l_cmp(WORK0, PER100BUF) <= 0 ){
					m_Util.l_pardotnet_syz( F2220C, F2215C, F224C, F227C );
				}
				else{
					m_Util.l_pardotnet_syz( F2220C, F2215C, F224C, F224C );
				}
			}
		}
	}

	if( !(F2220c & 0x10) ){	// Žè“ü—Í‚È‚µ
		memset( F2220A, '\0', MONY_BUF_SIZE );
		if( (*m_pSnHeadData)->Sn_UPERS & 0x10 ){

			//[Y]
			sprintf_s( buf, sizeof( buf ), _T("%d"), (*m_pSnFhyo22Data)->Sn_2FUWSEX );
			m_pArith->l_input( WORK0, buf );

			m_pArith->l_mul100( (unsigned char *)F2220A, (unsigned char *)F2215A, (unsigned char *)WORK0, 2, 0 );
		}
		else{

			//[Y]
			m_pArith->l_par100( (unsigned char *)WORK0, (unsigned char *)F224C,  (unsigned char *)F227C,  5, 0 );
			// ”„ãŠ„‡‚ªƒ}ƒCƒiƒX‚Ìê‡‚Í‚O“‚ÅŒvŽZ‚·‚éB
			memset( PW6, '\0', sizeof( PW6 ) );
			if( m_pArith->l_cmp(F224C, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}
			if( m_pArith->l_cmp(F227C, PW6) < 0 ){
				memset( WORK0, '\0', MONY_BUF_SIZE );
			}

			if( m_pArith->l_test(WORK0) == 0 ){
				m_pArith->l_mul100( (unsigned char *)F2220A, (unsigned char *)F2215A, (unsigned char *)WORK0, 5, 0 );
			}
			else{
				if( m_pArith->l_cmp(WORK0, PER100BUF) <= 0 ){
					m_Util.l_pardotnet_syz( F2220A, F2215A, F224C, F227C );
				}
				else{
					m_Util.l_pardotnet_syz( F2220A, F2215A, F224C, F224C );
				}
			}
		}
	}
	
	m_pArith->l_add( WORK0, F2220A, F2220B );
	m_pArith->l_add( WORK0, WORK0, F2220C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2220A, F2220A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2220B, F2220B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2220C, F2220C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2220D, WORK0 );
}

//-----------------------------------------------------------------------------
// ‰ÛÅ”„ãŠ„‡‚ª‚X‚T“–¢–ž‚Ìê‡
//-----------------------------------------------------------------------------
void CShinFhyo22EX::f22calq_ko()
{
	char	WORK0[MONY_BUF_SIZE] = {0};

	if( (*m_pSnHeadData)->IsKobetuSiireAnbun() ){
		dsp_cls( IDC_ICSDIAGCTRL2, ID2216A );
		dsp_cls( IDC_ICSDIAGCTRL2, ID2216B );
		dsp_cls( IDC_ICSDIAGCTRL2, ID2216C );
		dsp_cls( IDC_ICSDIAGCTRL2, ID2216D );

//-- '15.08.21 --
//		f22calq_15();
//		f22calq_16();
//		f22calq_17();
//---------------
		f22calq_17();
		f22calq_18();
		f22calq_19();
//---------------
		memset( WORK0,  '\0', MONY_BUF_SIZE );
		memset( F2220A, '\0', MONY_BUF_SIZE );
		memset( F2220B, '\0', MONY_BUF_SIZE );
		memset( F2220C, '\0', MONY_BUF_SIZE );
		F2220c = 0;

		dsp_prs( IDC_ICSDIAGCTRL2, ID2220A, F2220A );
		dsp_prs( IDC_ICSDIAGCTRL2, ID2220A, F2220B );
		dsp_prs( IDC_ICSDIAGCTRL2, ID2220C, F2220C );
		dsp_prs( IDC_ICSDIAGCTRL2, ID2220D, WORK0 );

		if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
			ModifyDiagInput( ID2217A, DIAG_MDFY_EDIT );
			ModifyDiagInput( ID2217B, DIAG_MDFY_EDIT );
			ModifyDiagInput( ID2217C, DIAG_MDFY_EDIT );
			ModifyDiagInput( ID2218A, DIAG_MDFY_EDIT );
			ModifyDiagInput( ID2218B, DIAG_MDFY_EDIT );
			ModifyDiagInput( ID2218C, DIAG_MDFY_EDIT );
			ModifyDiagInput( ID2219A, DIAG_MDFY_EDIT );
			ModifyDiagInput( ID2219B, DIAG_MDFY_EDIT );
			ModifyDiagInput( ID2219C, DIAG_MDFY_EDIT );
			ChangeColor( IDC_ICSDIAGCTRL2, ID2217A, 0 );
			ChangeColor( IDC_ICSDIAGCTRL2, ID2217B, 0 );
			ChangeColor( IDC_ICSDIAGCTRL2, ID2217C, 0 );
			ChangeColor( IDC_ICSDIAGCTRL2, ID2218A, 0 );
			ChangeColor( IDC_ICSDIAGCTRL2, ID2218B, 0 );
			ChangeColor( IDC_ICSDIAGCTRL2, ID2218C, 0 );
			ChangeColor( IDC_ICSDIAGCTRL2, ID2219A, 0 );
			ChangeColor( IDC_ICSDIAGCTRL2, ID2219B, 0 );
			ChangeColor( IDC_ICSDIAGCTRL2, ID2219C, 0 );

			//[Y]
			if(F2219c & 0x10){//Žè“ü—Í
				ChangeColor( IDC_ICSDIAGCTRL2, ID2219A, 3 );
			}
			if(F2219c & 0x01){//Žè“ü—Í
				ChangeColor( IDC_ICSDIAGCTRL2, ID2219B, 3 );
			}
			if(F2219c & 0x100){//Žè“ü—Í
				ChangeColor( IDC_ICSDIAGCTRL2, ID2219C, 3 );
			}

		}
		ChangeColor( IDC_ICSDIAGCTRL2, ID2220A, 1 );
		ChangeColor( IDC_ICSDIAGCTRL2, ID2220B, 1 );
		ChangeColor( IDC_ICSDIAGCTRL2, ID2220C, 1 );

		ModifyDiagInput( ID2220A, DIAG_MDFY_READONLY );
		ModifyDiagInput( ID2220B, DIAG_MDFY_READONLY );
		ModifyDiagInput( ID2220C, DIAG_MDFY_READONLY );
	}
	else{					// ”ä—á”z•ª
		dsp_cls( IDC_ICSDIAGCTRL2, ID2216A );
		dsp_cls( IDC_ICSDIAGCTRL2, ID2216B );
		dsp_cls( IDC_ICSDIAGCTRL2, ID2216C );
		dsp_cls( IDC_ICSDIAGCTRL2, ID2216D );
//-- '15.08.21 --
//		f22calq_18();
//---------------
		f22calq_20();
//---------------
		memset( F2217A, '\0', MONY_BUF_SIZE );
		memset( F2217B, '\0', MONY_BUF_SIZE );
		memset( F2217C, '\0', MONY_BUF_SIZE );
		memset( F2218A, '\0', MONY_BUF_SIZE );
		memset( F2218B, '\0', MONY_BUF_SIZE );
		memset( F2218C, '\0', MONY_BUF_SIZE );
		memset( F2219A, '\0', MONY_BUF_SIZE );
		memset( F2219B, '\0', MONY_BUF_SIZE );
		memset( F2219C, '\0', MONY_BUF_SIZE );
		F2219c = 0;
		memset( WORK0, '\0', MONY_BUF_SIZE );

		dsp_prs( IDC_ICSDIAGCTRL2, ID2217A, F2217A );
		dsp_prs( IDC_ICSDIAGCTRL2, ID2217A, F2217B );
		dsp_prs( IDC_ICSDIAGCTRL2, ID2217C, F2217C );
		dsp_prs( IDC_ICSDIAGCTRL2, ID2217D, WORK0 );
		dsp_prs( IDC_ICSDIAGCTRL2, ID2218A, F2218A );
		dsp_prs( IDC_ICSDIAGCTRL2, ID2218A, F2218B );
		dsp_prs( IDC_ICSDIAGCTRL2, ID2218C, F2218C );
		dsp_prs( IDC_ICSDIAGCTRL2, ID2218D, WORK0 );
		dsp_prs( IDC_ICSDIAGCTRL2, ID2219A, F2219A );
		dsp_prs( IDC_ICSDIAGCTRL2, ID2219A, F2219B );
		dsp_prs( IDC_ICSDIAGCTRL2, ID2219C, F2219C );
		dsp_prs( IDC_ICSDIAGCTRL2, ID2219D, WORK0 );

		ChangeColor( IDC_ICSDIAGCTRL2, ID2217A, 1 );
		ChangeColor( IDC_ICSDIAGCTRL2, ID2217B, 1 );
		ChangeColor( IDC_ICSDIAGCTRL2, ID2217C, 1 );
		ChangeColor( IDC_ICSDIAGCTRL2, ID2218A, 1 );
		ChangeColor( IDC_ICSDIAGCTRL2, ID2218B, 1 );
		ChangeColor( IDC_ICSDIAGCTRL2, ID2218C, 1 );
		ChangeColor( IDC_ICSDIAGCTRL2, ID2219A, 1 );
		ChangeColor( IDC_ICSDIAGCTRL2, ID2219B, 1 );
		ChangeColor( IDC_ICSDIAGCTRL2, ID2219C, 1 );
		
		ModifyDiagInput( ID2217A, DIAG_MDFY_READONLY );
		ModifyDiagInput( ID2217B, DIAG_MDFY_READONLY );
		ModifyDiagInput( ID2217C, DIAG_MDFY_READONLY );
		ModifyDiagInput( ID2218A, DIAG_MDFY_READONLY );
		ModifyDiagInput( ID2218B, DIAG_MDFY_READONLY );
		ModifyDiagInput( ID2218C, DIAG_MDFY_READONLY );
		ModifyDiagInput( ID2219A, DIAG_MDFY_READONLY );
		ModifyDiagInput( ID2219B, DIAG_MDFY_READONLY );
		ModifyDiagInput( ID2219C, DIAG_MDFY_READONLY );

		if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
			ModifyDiagInput( ID2220A, DIAG_MDFY_EDIT );
			ModifyDiagInput( ID2220B, DIAG_MDFY_EDIT );
			ModifyDiagInput( ID2220C, DIAG_MDFY_EDIT );
			ChangeColor( IDC_ICSDIAGCTRL2, ID2220A, 0 );
			ChangeColor( IDC_ICSDIAGCTRL2, ID2220B, 0 );
			ChangeColor( IDC_ICSDIAGCTRL2, ID2220C, 0 );

			//[Y]
			if(F2220c & 0x10){//Žè“ü—Í
				ChangeColor( IDC_ICSDIAGCTRL2, ID2220A, 3 );
			}
			if(F2220c & 0x01){//Žè“ü—Í
				ChangeColor( IDC_ICSDIAGCTRL2, ID2220B, 3 );
			}
			if(F2220c & 0x100){//Žè“ü—Í
				ChangeColor( IDC_ICSDIAGCTRL2, ID2220C, 3 );
			}
		}
		
	}
}

//-----------------------------------------------------------------------------
// TœÅŠz‚Ì’²®i‚P‚Wj
//-----------------------------------------------------------------------------
//void CShinFhyo22EX::f22calq_19()
void CShinFhyo22EX::f22calq_21()
{
	char	WORK0[MONY_BUF_SIZE] = {0};
	m_pArith->l_add( WORK0, F2221A, F2221B );
	m_pArith->l_add( WORK0, WORK0, F2221C );

	dsp_prs( IDC_ICSDIAGCTRL2, ID2221A, F2221A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2221B, F2221B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2221C, F2221C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2221D, WORK0 );
}

//-----------------------------------------------------------------------------
// TœÅŠz‚Ì’²®i‚Q‚Oj
//-----------------------------------------------------------------------------
//void CShinFhyo22EX::f22calq_20()
void CShinFhyo22EX::f22calq_22()
{
	char	WORK0[MONY_BUF_SIZE] = {0};
	m_pArith->l_add( WORK0, F2222A, F2222B );
	m_pArith->l_add( WORK0, WORK0, F2222C );

	dsp_prs( IDC_ICSDIAGCTRL2, ID2222A, F2222A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2222B, F2222B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2222C, F2222C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2222D, WORK0 );
}

//-----------------------------------------------------------------------------
//	·ˆøTœŽd“üÅŠz‚ÌŒvŽZ
//-----------------------------------------------------------------------------
//void CShinFhyo22EX::f22calq_2122()
void CShinFhyo22EX::f22calq_2324()
{
	char	va[MONY_BUF_SIZE]={0}, vb[MONY_BUF_SIZE]={0};
	char	WORK0[MONY_BUF_SIZE] = {0};
	char	WORK1[MONY_BUF_SIZE] = {0};

	// ‚R“
	if( F2223c & 0x01 ){

		if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID2223A, 3 );	// 2
			ModifyDiagInput( ID2223A, DIAG_MDFY_EDIT );
		}
//-- '15.11.14 --
//		memmove( F2223A, (*m_pSnFhyo22Data)->Sn_2F21A, MONY_BUF_SIZE );
//---------------
		memmove( F2223A, (*m_pSnFhyo22Data)->Sn_2F22A, MONY_BUF_SIZE );
//---------------
		if( m_pArith->l_test(F2223A) < 0 ){
			m_pArith->l_neg( F2223A );
			memmove( F2224A, F2223A, MONY_BUF_SIZE );
			memset( F2223A, '\0', MONY_BUF_SIZE );
		}
		else	{
//-- '15.11.14 --
//			memmove( F2223A, (*m_pSnFhyo22Data)->Sn_2F21A, MONY_BUF_SIZE );
//---------------
			memmove( F2223A, (*m_pSnFhyo22Data)->Sn_2F22A, MONY_BUF_SIZE );
//---------------
			memset( F2224A, '\0', MONY_BUF_SIZE );
		}
		// Tœ‘ÎÛŽd“üÅŠz
		dsp_prs( IDC_ICSDIAGCTRL2, ID2223A, F2223A);
		// Tœ‰ß‘å’²®Šz
		dsp_prs( IDC_ICSDIAGCTRL2, ID2224A, F2224A);
	}
	else{
		if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID2223A, 0 );
			ModifyDiagInput( ID2223A, DIAG_MDFY_EDIT );
		}
		memset( F2224A, '\0', MONY_BUF_SIZE );
		m_pArith->l_add( WORK0, F2221A, F2222A );
		memset( va, '\0', MONY_BUF_SIZE );
		if( (*m_pSnHeadData)->IsUseSemiUriRatio() ){
			memmove( va, F2219A, MONY_BUF_SIZE );
		}
		else{
			if( F22UWS >= (*m_pSnHeadData)->m_UriRatioBorder ){
				memmove( va, F2215A, MONY_BUF_SIZE );
			}
			else{
				memmove( va, (*m_pSnHeadData)->IsKobetuSiireAnbun() ? F2219A : F2220A, MONY_BUF_SIZE );
			}
		}
		m_pArith->l_add( va, va, WORK0 );
		if( m_pArith->l_test(va) < 0 ) {
			memset( F2223A, '\0', MONY_BUF_SIZE );
			m_pArith->l_neg(va);
			memmove( F2224A, va, MONY_BUF_SIZE );
		}
		else{
			memset( F2224A, '\0', MONY_BUF_SIZE );
			memmove( F2223A, va, MONY_BUF_SIZE );
		}
		// Tœ‘ÎÛŽd“üÅŠz
		dsp_prs( IDC_ICSDIAGCTRL2, ID2223A, F2223A );
		// Tœ‰ß‘å’²®Šz
		dsp_prs( IDC_ICSDIAGCTRL2, ID2224A, F2224A );
	}

	// ‚S“
	if( F2223c & 0x10 ){
		if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID2223B, 3 );		// 2
			ModifyDiagInput( ID2223B, DIAG_MDFY_EDIT );
		}
//-- '15.11.14 --
//		memmove( F2223B, (*m_pSnFhyo22Data)->Sn_2F21B, MONY_BUF_SIZE );
//---------------
		memmove( F2223B, (*m_pSnFhyo22Data)->Sn_2F22B, MONY_BUF_SIZE );
//---------------
		if( m_pArith->l_test(F2223B) < 0 ){
			m_pArith->l_neg( F2223B );
			memmove( F2224B, F2223B, MONY_BUF_SIZE );
			memset( F2223B, '\0', MONY_BUF_SIZE );
		}
		else{
//-- '15.11.14 --
//			memmove( F2223B, (*m_pSnFhyo22Data)->Sn_2F21B, MONY_BUF_SIZE );
//---------------
			memmove( F2223B, (*m_pSnFhyo22Data)->Sn_2F22B, MONY_BUF_SIZE );
//---------------
			memset( F2224B, '\0', MONY_BUF_SIZE );
		}
		// Tœ‘ÎÛŽd“üÅŠz
		dsp_prs( IDC_ICSDIAGCTRL2, ID2223B, F2223B );
		// Tœ‰ß‘å’²®Šz
		dsp_prs( IDC_ICSDIAGCTRL2, ID2224B, F2224B);
	}
	else{
		if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID2223B, 0 );
			ModifyDiagInput( ID2223B, DIAG_MDFY_EDIT );
		}
		memset( F2224B, '\0', MONY_BUF_SIZE );
		m_pArith->l_add( WORK1, F2221B, F2222B );
		memset( vb, '\0', MONY_BUF_SIZE );
		if( (*m_pSnHeadData)->IsUseSemiUriRatio() ){
			memmove( vb, F2219B, MONY_BUF_SIZE );
		}
		else{
			if( F22UWS >= (*m_pSnHeadData)->m_UriRatioBorder ){
				memmove( vb, F2215B, MONY_BUF_SIZE );
			}
			else{
				memmove( vb, (*m_pSnHeadData)->IsKobetuSiireAnbun() ? F2219B : F2220B, MONY_BUF_SIZE );
			}
		}
		m_pArith->l_add( vb, vb, WORK1 );
		if( m_pArith->l_test(vb) < 0 ) {
			memset( F2223B, '\0', MONY_BUF_SIZE );
			m_pArith->l_neg( vb );
			memmove( F2224B, vb, MONY_BUF_SIZE );
		}
		else{
			memset( F2224B, '\0', MONY_BUF_SIZE );
			memmove( F2223B, vb, MONY_BUF_SIZE );
		}
		// Tœ‘ÎÛŽd“üÅŠz
		dsp_prs( IDC_ICSDIAGCTRL2, ID2223B, F2223B);
		// Tœ‰ß‘å’²®Šz
		dsp_prs( IDC_ICSDIAGCTRL2, ID2224B, F2224B);
	}

	// F2223C, F2224C ‚W“IIII
	if( F2223c & 0x100 ){
		if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID2223C, 3 );		// 2
			ModifyDiagInput( ID2223C, DIAG_MDFY_EDIT );
		}
//-- '15.11.14 --
//		memmove( F2223C, (*m_pSnFhyo22Data)->Sn_2F21C, MONY_BUF_SIZE );
//---------------
		memmove( F2223C, (*m_pSnFhyo22Data)->Sn_2F22C, MONY_BUF_SIZE );
//---------------
		if( m_pArith->l_test(F2223C) < 0 ){
			m_pArith->l_neg( F2223C );
			memmove( F2224C, F2223C, MONY_BUF_SIZE );
			memset( F2223C, '\0', MONY_BUF_SIZE );
		}
		else{
//-- '15.11.14 --
//			memmove( F2223C, (*m_pSnFhyo22Data)->Sn_2F21C, MONY_BUF_SIZE );
//---------------
			memmove( F2223C, (*m_pSnFhyo22Data)->Sn_2F22C, MONY_BUF_SIZE );
//---------------
			memset( F2224C, '\0', MONY_BUF_SIZE );
		}
		// Tœ‘ÎÛŽd“üÅŠz
		dsp_prs( IDC_ICSDIAGCTRL2, ID2223C, F2223C );
		// Tœ‰ß‘å’²®Šz
		dsp_prs( IDC_ICSDIAGCTRL2, ID2224C, F2224C);
	}
	else{
		if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
			ChangeColor( IDC_ICSDIAGCTRL2, ID2223C, 0 );
			ModifyDiagInput( ID2223C, DIAG_MDFY_EDIT );
		}
		memset( F2224C, '\0', MONY_BUF_SIZE );
		m_pArith->l_add( WORK1, F2221C, F2222C );
		memset( vb, '\0', MONY_BUF_SIZE );
		if( (*m_pSnHeadData)->IsUseSemiUriRatio() ){
			memmove( vb, F2219C, MONY_BUF_SIZE );
		}
		else{
			if( F22UWS >= (*m_pSnHeadData)->m_UriRatioBorder ){
				memmove( vb, F2215C, MONY_BUF_SIZE );
			}
			else{
				memmove( vb, (*m_pSnHeadData)->IsKobetuSiireAnbun() ? F2219C : F2220C, MONY_BUF_SIZE );
			}
		}
		m_pArith->l_add( vb, vb, WORK1 );
		if( m_pArith->l_test(vb) < 0 ) {
			memset( F2223C, '\0', MONY_BUF_SIZE );
			m_pArith->l_neg( vb );
			memmove( F2224C, vb, MONY_BUF_SIZE );
		}
		else{
			memset( F2224C, '\0', MONY_BUF_SIZE );
			memmove( F2223C, vb, MONY_BUF_SIZE );
		}
		// Tœ‘ÎÛŽd“üÅŠz
		dsp_prs( IDC_ICSDIAGCTRL2, ID2223C, F2223C);
		// Tœ‰ß‘å’²®Šz
		dsp_prs( IDC_ICSDIAGCTRL2, ID2224C, F2224C);
	}

	// Ä•`‰æ
	m_pArith->l_add( WORK0, F2223A, F2223B );
	m_pArith->l_add( WORK0, WORK0, F2223C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2223D, WORK0 );
	m_pArith->l_add( WORK1, F2224A, F2224B );
	m_pArith->l_add( WORK1, WORK1, F2224C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2224D, WORK1 );
}

//-----------------------------------------------------------------------------
// ‰ñŽû‚ÉŒW‚éÁ”ïÅŠz
//-----------------------------------------------------------------------------
//void CShinFhyo22EX::f22calq_23()
void CShinFhyo22EX::f22calq_25()
{
	char	WORK0[MONY_BUF_SIZE] = {0};
	m_pArith->l_add( WORK0, F2225A, F2225B );
	m_pArith->l_add( WORK0, WORK0, F2225C );

	dsp_prs( IDC_ICSDIAGCTRL2, ID2225A, F2225A );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2225B, F2225B );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2225C, F2225C );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2225D, WORK0 );
}

//-----------------------------------------------------------------------------
// Še“ü—Í‚ÌŒvŽZ•\Ž¦
//-----------------------------------------------------------------------------
// ˆø”	atno	F	
//-----------------------------------------------------------------------------
void CShinFhyo22EX::f22calq_all( int atno )
{
	while( 1 ){
		switch( atno ){
			case	1 :		// ‰ÛÅ”„ãŠz
				f22calq_1();
			case	2 :		// –ÆÅ”„ãŠz
			case	3 :		// ”ñ‰ÛÅŽ‘ŽY‚Ì—Ao
							// ‰ÛÅŽ‘ŽY‚Ì÷“n“™‘Î‰¿Šz
				f22calq_45();
			case	4 :		// ”ñ‰ÛÅ”„ãŠz
							// Ž‘ŽY‚Ì÷“n“™‘Î‰¿ŠzE‰ÛÅ”„ãŠ„‡
				f22calq_7();
			case	5 :		// ‰ÛÅŽd“ü‚ÉŒW‚éŽx•¥‘Î‰¿Šz
				f22calq_8();
			case	6 :		// ‰ÛÅŽd“ü‚ÉŒW‚éÁ”ïÅŠz
				f22calq_9();
			case	15:
				f22calq_10();
			case	16:
				f22calq_11();
			case	7 :		// ‰ÛÅ‰Ý•¨‚ÉŒW‚éÁ”ïÅŠz
							// –ÆÅÌ‰ÛÅ’²®ÅŠz
							// ‰ÛÅŽd“ü“™‚ÌÅŠz‡Œv
//				f22calq_11();				
//				f22calq_12();				
//				f22calq_13();				
				f22calq_13();				
				f22calq_14();				
				f22calq_15();				
			case	8 :	
			case	9 :
			case	10:
			case	11:
				if( (*m_pSnHeadData)->IsUseSemiUriRatio() ){
					// ‰ÛÅ”„ãŠ„‡‚É€‚¸‚éŠ„‡‚ð—LŒø
					f22calq_ko();			// ‰ÛÅ”„ãŠ„‡‚ª‚X‚T“–¢–ž
				}
				else{
					if( F22UWS >= (*m_pSnHeadData)->m_UriRatioBorder ){
						// ‰ÛÅ”„ãŠ„‡‚ª‚X‚T“ˆÈã
//						f22calq_14();
						f22calq_16();
					}
					else{
						// ‰ÛÅ”„ãŠ„‡‚ª‚X‚T“–¢–ž
						f22calq_ko();	
					}
				}
				// “Á’èŽû“ü‚ð‰Á–¡‚µ‚½Tœ‘ÎÛŽd“üÅŠz‚ÌŒvŽZ('15.04.04)
				f22calq_spc();
			case	12:		// TœÅŠz‚Ì’²®i‚P‚Xj
//				f22calq_19();
				f22calq_21();
			case	13:		// TœÅŠz‚Ì’²®i‚Q‚Oj
							// ·ˆøTœÅŠz
//				f22calq_20();				
//				f22calq_2122();				
				f22calq_22();				
				f22calq_2324();				
			case	14:		// ‘Ý“|‰ñŽû‚ÉŒW‚éÁ”ïÅŠz
//				f22calq_23();
				f22calq_25();
			default	:
				break;
		}
		break;
	}
}

//-----------------------------------------------------------------------------
// “Á’èŽû“ü‚ð‰Á–¡‚µ‚½Tœ‘ÎÛŽd“üÅŠz‚ÌŒvŽZ('15.04.04)
//-----------------------------------------------------------------------------
void CShinFhyo22EX::f22calq_spc()
{
	if( *(m_pShinInfo->pSgSpc) == 1 ){

		unsigned char svSn_Sign2 = (*m_pSnHeadData)->Sn_Sign2;

		SYZTOSPC_DATA	SyzToSpc = {0};
		BOOL			IsRatioOver95 = FALSE;
		if( F22UWS >= (*m_pSnHeadData)->m_UriRatioBorder ){
			IsRatioOver95 = TRUE;
			SyzToSpc.IsRatioOver95 |= 0x01;
		}

		// ‰ÛÅ”„ãƒZƒbƒg
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

		// Tœ‘ÎÛŽd“üÅŠzƒZƒbƒg
		if( IsRatioOver95 ){
			//== ŒvŽZ•\‚Ti‚Pj ==
			memmove( SyzToSpc.SpKgz3, F2215A, sizeof(SyzToSpc.SpKgz3) );
			memmove( SyzToSpc.SpKgz4, F2215B, sizeof(SyzToSpc.SpKgz4) );
			memmove( SyzToSpc.SpKgz63, F2215C, sizeof(SyzToSpc.SpKgz63) );
		}
		if( (*m_pSnHeadData)->IsKobetuSiireAnbun() && (IsRatioOver95==FALSE) ){
			//== ŒvŽZ•\‚Ti‚Qj ==
			memmove( SyzToSpc.SpKgz3, F2219A, sizeof(SyzToSpc.SpKgz3) );
			memmove( SyzToSpc.SpKgz4, F2219B, sizeof(SyzToSpc.SpKgz4) );
			memmove( SyzToSpc.SpKgz63, F2219C, sizeof(SyzToSpc.SpKgz63) );
		}
		if( ((*m_pSnHeadData)->IsKobetuSiireAnbun()==FALSE) && (IsRatioOver95==FALSE) ){
			//== ŒvŽZ•\‚Ti‚Rj ==
			memmove( SyzToSpc.SpKgz3, F2220A, sizeof(SyzToSpc.SpKgz3) );
			memmove( SyzToSpc.SpKgz4, F2220B, sizeof(SyzToSpc.SpKgz4) );
			memmove( SyzToSpc.SpKgz63, F2220C, sizeof(SyzToSpc.SpKgz63) );
		}
		// ÄŒvŽZ
		((CH26HyoView*)m_pParent)->RecalqSkj( &SyzToSpc );
		// ‹àŠzÄƒZƒbƒg
		if( (*m_pSnHeadData)->Sn_Sign2&0x02 ){
			if( !(svSn_Sign2&0x02) ){
				F2223c &= ~0x111;
			}
		}
		else{
			memmove( F2223A, SyzToSpc.SpSiz3, sizeof(F2223A) );
			memmove( F2223B, SyzToSpc.SpSiz4, sizeof(F2223B) );
			memmove( F2223C, SyzToSpc.SpSiz63, sizeof(F2223C) );
//-- '15.11.14 --
//			memmove( (*m_pSnFhyo22Data)->Sn_2F21A, SyzToSpc.SpSiz3, sizeof((*m_pSnFhyo22Data)->Sn_2F21A) );
//			memmove( (*m_pSnFhyo22Data)->Sn_2F21B, SyzToSpc.SpSiz4, sizeof((*m_pSnFhyo22Data)->Sn_2F21B) );
//			memmove( (*m_pSnFhyo22Data)->Sn_2F21C, SyzToSpc.SpSiz63, sizeof((*m_pSnFhyo22Data)->Sn_2F21C) );
//---------------
			memmove( (*m_pSnFhyo22Data)->Sn_2F22A, SyzToSpc.SpSiz3, sizeof((*m_pSnFhyo22Data)->Sn_2F22A) );
			memmove( (*m_pSnFhyo22Data)->Sn_2F22B, SyzToSpc.SpSiz4, sizeof((*m_pSnFhyo22Data)->Sn_2F22B) );
			memmove( (*m_pSnFhyo22Data)->Sn_2F22C, SyzToSpc.SpSiz63, sizeof((*m_pSnFhyo22Data)->Sn_2F22C) );
//---------------
			F2223c |= 0x111;
		}
	}
}
