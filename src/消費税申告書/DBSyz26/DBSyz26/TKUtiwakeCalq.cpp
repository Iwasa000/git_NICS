
#include "stdafx.h"
#include "TKUtiwake.h"
#include "TKUtiwakeIdx.h"
#include "H26HyoView.h"

//-----------------------------------------------------------------------------
// Še“ü—Í‚ÌŒvŽZ•\Ž¦
//-----------------------------------------------------------------------------
// ˆø”	atno	F	ŠJŽnŒvŽZˆÊ’u
//		sw		F	•`‰æ
//-----------------------------------------------------------------------------
void CTKUtiwake::calqx_all( int atno, int sw )
{
	while( 1 ){
		switch( atno ){
			case	1:
			case	2:
			case	3:
				calq_utiwake();
				calqx_1to3();
				if( !sw ){
					dispmony_utiwake();
				}
				// «
			case	4:
			case	5:
			case	6:
				calqx_4to6();
				if( !sw ){
					dispmony_1to6();
				}
			default	:
				break;
		}
		break;
	}
}

//-----------------------------------------------------------------------------
// 2,3.“à–óŒvŽZ
//-----------------------------------------------------------------------------
void CTKUtiwake::calq_utiwake()
{
	if( (*m_pSnHeadData)->SVmzsw ){
		m_pArith->l_clear( UTI31 );
		m_pArith->l_add( UTI31, UTI31, UTI11 );	
		m_Util.l_calq( UTI31, UTI31, 0 );

		m_pArith->l_clear( UTI32 );
		m_pArith->l_add( UTI32, UTI32, UTI12 );	
		m_Util.l_calq( UTI32, UTI32, 0 );

		m_pArith->l_clear( UTI33 );
		m_pArith->l_add( UTI33, UTI13, UTI23 );	
		m_Util.l_calq( UTI33, UTI33, 0 );
	}
	else{
		m_pArith->l_clear( UTI11 );
		m_pArith->l_clear( UTI31 );

		m_pArith->l_clear( UTI12 );
		m_pArith->l_clear( UTI32 );

		memmove( UTI13, XM2, sizeof(UTI13) );
		memmove( UTI23, XM3, sizeof(UTI23) );

		m_pArith->l_clear( UTI33 );
		m_pArith->l_add( UTI33, UTI13, UTI23 );	
		m_Util.l_calq( UTI33, UTI33, 0 );
	}
}

//-----------------------------------------------------------------------------
// 1-3.‰ÛÅ•W€Šz
//-----------------------------------------------------------------------------
void CTKUtiwake::calqx_1to3()
{
	if( (*m_pSnHeadData)->SVmzsw ){
		m_pArith->l_clear( XM2 );
		m_pArith->l_add( XM2, UTI11, UTI12 );	
		m_pArith->l_add( XM2, XM2, UTI13 );	

		m_pArith->l_clear( XM3 );
		m_pArith->l_add( XM3, XM3, UTI23 );	
	}
	else{
		;
	}

	char	svXM1[MONY_BUF_SIZE]={0};
	memmove( svXM1, XM1, MONY_BUF_SIZE );

	m_pArith->l_clear( XM1 );
//2015.11.11 UPDATE START
//	m_pArith->l_add( XM1, XM2, XM3 );
	if( !(*m_pSnHeadData)->SVmzsw ){
		memmove( XM1, UTI33, MONY_BUF_SIZE );
	}else{
//-- '15.12.02 --
//		m_pArith->l_add( XM1, XM2, XM3 );
//---------------
		m_pArith->l_add( XM1, UTI31, UTI32 );
		m_pArith->l_add( XM1, XM1, UTI33 );
//---------------
	}
//2015.11.11 UPDATE END
	m_Util.l_calq( XM1, XM1, 0 );

	if( ((*m_pSnHeadData)->SVmzsw==0) && (m_pArith->l_cmp(XM1, svXM1)!=0) ){
		// –{•\‘¤‚Ö˜A“®
		if( ((*m_pSnHeadData)->Sn_Sign4&0x80) && (((*m_pSnHeadData)->Sn_Sign4&0x01)==0x00) ){
			WriteData();
			m_pParent->PostMessage( WM_USER_RECALQ_TKKZ );
		}
	}
}

//-----------------------------------------------------------------------------
// 4-6.•ÔŠÒ“™‘Î‰¿‚ÉŒW‚éÅŠz
//-----------------------------------------------------------------------------
void CTKUtiwake::calqx_4to6()
{
	m_pArith->l_clear( XM4 );
	m_pArith->l_add( XM4, XM5, XM6 );
}

//-----------------------------------------------------------------------------
// ‹àŠz•\Ž¦(1-6)
//-----------------------------------------------------------------------------
void CTKUtiwake::dispmony_1to6()
{
	dsp_prs( IDC_ICSDIAGCTRL2, ID2XM1, XM1 );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2XM2, XM2 );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2XM3, XM3 );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2XM4, XM4 );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2XM5, XM5 );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2XM6, XM6 );
}

//-----------------------------------------------------------------------------
// ‹àŠz•\Ž¦(“à–ó)
//-----------------------------------------------------------------------------
void CTKUtiwake::dispmony_utiwake()
{
	dsp_prs( IDC_ICSDIAGCTRL2, ID2UTI13, UTI13 );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2UTI23, UTI23 );
	dsp_prs( IDC_ICSDIAGCTRL2, ID2UTI33, UTI33 );

	if( (*m_pSnHeadData)->SVmzsw ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID2UTI11, UTI11 );
		dsp_prs( IDC_ICSDIAGCTRL2, ID2UTI12, UTI12 );
		dsp_prs( IDC_ICSDIAGCTRL2, ID2UTI31, UTI31 );
		dsp_prs( IDC_ICSDIAGCTRL2, ID2UTI32, UTI32 );
	}
}
