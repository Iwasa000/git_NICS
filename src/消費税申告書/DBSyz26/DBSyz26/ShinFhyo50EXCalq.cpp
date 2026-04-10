
#include "stdafx.h"
#include "ShinFhyo50EX.h"
#include "ShinFhyo50EXIdx.h"

#include <math.h>

//-----------------------------------------------------------------------------
// Tœ‘ÎÛŽd“üÅŠzŒvŽZ‚ÌŠî‘b‚Æ‚È‚éÁ”ïÅŠz
//-----------------------------------------------------------------------------
void CShinFhyo50EX::f5calq_4()
{
	m_pArith->l_add( F54T, F51T, F52T );
	m_pArith->l_sub( F54T, F54T, F53T );
//	[07'06.07] ƒJƒbƒg˜R‚ê@‚»‚Ì‚Ü‚Üo‰×‰e‹¿–³‚µ
	if( m_pArith->l_test(F54T) < 0 ){
		memset( F54T, '\0', MONY_BUF_SIZE );
	}
	m_pArith->l_add( F54B, F51B, F52B );
	m_pArith->l_sub( F54B, F54B, F53B );
//	[07'06.07] ƒJƒbƒg˜R‚ê@‚»‚Ì‚Ü‚Üo‰×‰e‹¿–³‚µ
	if( m_pArith->l_test(F54B) < 0 ){
		memset( F54B, '\0', MONY_BUF_SIZE );
	}
	dsp_prs( IDC_ICSDIAGCTRL2, ID54T, F54T );	
	if( m_pArith->l_cmp(F54S, F54T) ){
		CQsw &= 0x0f;
	}
}

//-----------------------------------------------------------------------------
// ‚PŽí—Þ‚ÌŽ–‹Æ‚Ìê‹ÆŽÒ‚Ìê‡
//-----------------------------------------------------------------------------
void CShinFhyo50EX::f5calq_5()
{
	char			kani;
	unsigned short	per = 0;
	char			WORK0[MONY_BUF_SIZE]={0}, WORK1[MONY_BUF_SIZE]={0};

	dsp_cls( IDC_ICSDIAGCTRL2, ID55T );
	dsp_cls( IDC_ICSDIAGCTRL2, ID520T );
//	dsp_cls( IDC_ICSDIAGCTRL2, ID530T );

	kani = 0x00;
	m_typeCnt = 0;
	if( m_pArith->l_test(F57T) ){
		kani |= 0x01;
		m_typeCnt++;
	}
	if( m_pArith->l_test(F58T) ){
		kani |= 0x02;
		m_typeCnt++;
	}
	if( m_pArith->l_test(F59T) ){
		kani |= 0x04;
		m_typeCnt++;
	}
	if( m_pArith->l_test(F510T) ){
		kani |= 0x08;
		m_typeCnt++;
	}
	if( m_pArith->l_test(F511T) ){
		kani |= 0x10;
		m_typeCnt++;
	}
	if( m_pArith->l_test(F512T) ){
		kani |= 0x20;
		m_typeCnt++;
	}

	if( m_typeCnt == 1 ){
		switch( kani&0xff ){
			case 0x01 :
				per = (*m_pSnHeadData)->SVsper[0];
				break;
			case 0x02 :
				per = (*m_pSnHeadData)->SVsper[1];
				break;
			case 0x04 :
				per = (*m_pSnHeadData)->SVsper[2];
				break;
			case 0x08 :	
				per = (*m_pSnHeadData)->SVsper[3];
				break;
			case 0x10 :	
				per = (*m_pSnHeadData)->SVsper[4];
				break;
			case 0x20 :	
				per = (*m_pSnHeadData)->SVsper[5];
				break;
			default   :	
				per = 0;
				break;
		}
		sprintf_s( WORK0, sizeof(WORK0), _T("%d"), (1000 - per) );
		memset( WORK1, '\0', MONY_BUF_SIZE );
		m_pArith->l_input( WORK1, WORK0 );
		memset( WORK0, '\0', MONY_BUF_SIZE );
		m_pArith->l_input( WORK0, "1000" );

		memset( F55T, '\0', MONY_BUF_SIZE );
		m_Util.l_6calq( F55T, F54T, WORK1, WORK0 );	//, 0 );
		CQsw |= 0x01;
		SZsw = SBsw = 0x00;
		memset( F520T, '\0', MONY_BUF_SIZE );
		memset( F520B, '\0', MONY_BUF_SIZE );
		memset( F521T, '\0', MONY_BUF_SIZE );
		memset( F521B, '\0', MONY_BUF_SIZE );
	}
	else{
		memset( F55T, '\0', MONY_BUF_SIZE );
		memset( F55B, '\0', MONY_BUF_SIZE );
		CQsw &= 0xf0;
		if( m_typeCnt == 0 ){
			memset( F56T,  '\0', MONY_BUF_SIZE );
			memset( F56B,  '\0', MONY_BUF_SIZE );
			memset( F57T,  '\0', MONY_BUF_SIZE );
			memset( F57B,  '\0', MONY_BUF_SIZE );
			memset( F58T,  '\0', MONY_BUF_SIZE );
			memset( F58B,  '\0', MONY_BUF_SIZE );
			memset( F59T,  '\0', MONY_BUF_SIZE );
			memset( F59B,  '\0', MONY_BUF_SIZE );
			memset( F510T, '\0', MONY_BUF_SIZE );
			memset( F510B, '\0', MONY_BUF_SIZE );
			memset( F511T, '\0', MONY_BUF_SIZE );
			memset( F511B, '\0', MONY_BUF_SIZE );
			memset( F512T, '\0', MONY_BUF_SIZE );
			memset( F512B, '\0', MONY_BUF_SIZE );
			memset( F513T, '\0', MONY_BUF_SIZE );
			memset( F513B, '\0', MONY_BUF_SIZE );
			memset( F514T, '\0', MONY_BUF_SIZE );
			memset( F514B, '\0', MONY_BUF_SIZE );
			memset( F515T, '\0', MONY_BUF_SIZE );
			memset( F515B, '\0', MONY_BUF_SIZE );
			memset( F516T, '\0', MONY_BUF_SIZE );
			memset( F516B, '\0', MONY_BUF_SIZE );
			memset( F517T, '\0', MONY_BUF_SIZE );
			memset( F517B, '\0', MONY_BUF_SIZE );
			memset( F518T, '\0', MONY_BUF_SIZE );
			memset( F518B, '\0', MONY_BUF_SIZE );
			memset( F519T, '\0', MONY_BUF_SIZE );
			memset( F519B, '\0', MONY_BUF_SIZE );
			memset( F520T, '\0', MONY_BUF_SIZE );
			memset( F520B, '\0', MONY_BUF_SIZE );
			memset( F521T, '\0', MONY_BUF_SIZE );
			memset( F521B, '\0', MONY_BUF_SIZE );
		}
	}
	dsp_prs( IDC_ICSDIAGCTRL2, ID55T, F55T);
}

#ifdef _20140301_CUT_
//-----------------------------------------------------------------------------
// Ž–‹Æ‹æ•ª•Ê‚Ì‡ŒvÁ”ïÅŠz iŠúŠÔŒvj
//-----------------------------------------------------------------------------
// ˆø”	ssg	F	
//-----------------------------------------------------------------------------
void CShinFhyo50EX::f5calq_gts( char ssg )
{
	char	WORK0[MONY_BUF_SIZE]={0};

/*- '14.03.01 -*/
//	if( (F50c&0xff) || (m_pArith->l_test(F512T)==0) ) {
//		memset( F512T, '\0', MONY_BUF_SIZE );
//		memmove( WORK0, F56T, MONY_BUF_SIZE );
//		m_Util.percent( F512T, WORK0, 1, 63, 0 );
//		F50c &= 0xf0;
//	}
/*-------------*/
	if( !(F512sw&0x800) ){
		if( (F512sw&0x100) || (m_pArith->l_test(F512T)==0) ){
			memset( F512T, '\0', MONY_BUF_SIZE );
			memmove( WORK0, F56T, MONY_BUF_SIZE );
			m_Util.percent( F512T, WORK0, 1, 63, 0 );
		}
	}
	F512sw &= ~0x100;
/*-------------*/
	
	if( ssg >= 0 ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID512T, F512T);
	}

	if( F512sw&0x800 ){
		ChangeColor( IDC_ICSDIAGCTRL2, ID512T, 3 );
	}
	else{
		ChangeColor( IDC_ICSDIAGCTRL2, ID512T, 4 );
	}
}
#else
//-----------------------------------------------------------------------------
// Ž–‹Æ‹æ•ª•Ê‚Ì‡ŒvÁ”ïÅŠz iŠúŠÔŒvj
//-----------------------------------------------------------------------------
// ˆø”	ssg	F	
//-----------------------------------------------------------------------------
void CShinFhyo50EX::f5calq_gts( char ssg )
{
	// Á”ïÅŠzF‡ŽZ
//-- '15.01.26 --
//	m_pArith->l_add( F513T, F514T, F515T );
//	m_pArith->l_add( F513T, F513T, F516T );
//	m_pArith->l_add( F513T, F513T, F517T );
//	m_pArith->l_add( F513T, F513T, F518T );
//	m_pArith->l_add( F513T, F513T, F519T );
//---------------
	m_pArith->l_clear(F513T);
	if( m_pArith->l_test(F514T) >= 0 ){
		m_pArith->l_add( F513T, F513T, F514T );
	}
	if( m_pArith->l_test(F515T) >= 0 ){
		m_pArith->l_add( F513T, F513T, F515T );
	}
	if( m_pArith->l_test(F516T) >= 0 ){
		m_pArith->l_add( F513T, F513T, F516T );
	}
	if( m_pArith->l_test(F517T) >= 0 ){
		m_pArith->l_add( F513T, F513T, F517T );
	}
	if( m_pArith->l_test(F518T) >= 0 ){
		m_pArith->l_add( F513T, F513T, F518T );
	}
	if( m_pArith->l_test(F519T) >= 0 ){
		m_pArith->l_add( F513T, F513T, F519T );
	}
//---------------

	dsp_prs( IDC_ICSDIAGCTRL2, ID513T, F513T );
}
#endif

//-----------------------------------------------------------------------------
// Ž–‹Æ•Ê‰ÛÅ”„ãŠ„‡‚Ì•\Ž¦
//-----------------------------------------------------------------------------
void CShinFhyo50EX::f5calq_uws( void )
{
	dsp_uws( IDC_ICSDIAGCTRL2, ID57TE, (*m_pSnHonpyoData)->Sn_UP1, 1 );
	dsp_uws( IDC_ICSDIAGCTRL2, ID58TE, (*m_pSnHonpyoData)->Sn_UP2, 1 );
	dsp_uws( IDC_ICSDIAGCTRL2, ID59TE, (*m_pSnHonpyoData)->Sn_UP3, 1 );
	dsp_uws( IDC_ICSDIAGCTRL2, ID510TE, (*m_pSnHonpyoData)->Sn_UP4, 1 );
	dsp_uws( IDC_ICSDIAGCTRL2, ID511TE, (*m_pSnHonpyoData)->Sn_UP5, 1 );
	dsp_uws( IDC_ICSDIAGCTRL2, ID512TE, (*m_pSnHonpyoData)->Sn_UP6, 1 );
}

//-----------------------------------------------------------------------------
// ‘æˆêŽíŽ–‹Æ
//-----------------------------------------------------------------------------
void CShinFhyo50EX::f5calq_ku1()
{
	char	WORK0[MONY_BUF_SIZE]={0};

	dsp_cls( IDC_ICSDIAGCTRL2, ID57T );
	dsp_cls( IDC_ICSDIAGCTRL2, ID514T );
	
/*- '14.03.01 -*/
//	if( m_pArith->l_test(F57T) == 0 ){
//		memset( F513T, '\0', MONY_BUF_SIZE );
//	}
//	if( (F51c&0xff) || (m_pArith->l_test(F57T)&&(m_pArith->l_test(F513T)==0)) ){
//		memset( F513T, '\0', MONY_BUF_SIZE );
//		memmove( WORK0, F57T, MONY_BUF_SIZE );
//		m_Util.percent( F513T, WORK0, 1, 63, 0 );
//		F51c &= 0xf0;
//	}
/*-------------*/
	if( !(F514sw&0x800) ){
		if( m_pArith->l_test(F57T) == 0 ){
			memset( F514T, '\0', MONY_BUF_SIZE );
		}
		if( (F514sw&0x100) || (m_pArith->l_test(F57T)&&(m_pArith->l_test(F514T)==0)) ){
			memset( F514T, '\0', MONY_BUF_SIZE );
			memmove( WORK0, F57T, MONY_BUF_SIZE );
			m_Util.percent( F514T, WORK0, 1, 63, 0 );
		}
	}
	F514sw &= ~0x100;
/*-------------*/
	dsp_prs( IDC_ICSDIAGCTRL2, ID57T, F57T );
	dsp_prs( IDC_ICSDIAGCTRL2, ID514T, F514T );

	if( F514sw&0x800 ){
		ChangeColor( IDC_ICSDIAGCTRL2, ID514T, 3 );
	}
	else{
		ChangeColor( IDC_ICSDIAGCTRL2, ID514T, 0 );
	}
}

//-----------------------------------------------------------------------------
// ‘æ“ñŽíŽ–‹Æ
//-----------------------------------------------------------------------------
void CShinFhyo50EX::f5calq_ku2()
{
	char	WORK0[MONY_BUF_SIZE]={0};

	dsp_cls( IDC_ICSDIAGCTRL2, ID58T );
	dsp_cls( IDC_ICSDIAGCTRL2, ID515T );

/*- '14.03.01 -*/
//	if( m_pArith->l_test(F58T) == 0 ){
//		memset( F514T, '\0', MONY_BUF_SIZE );
//	}
//
//	if( (F52c&0xff) || (m_pArith->l_test(F58T)&&(m_pArith->l_test(F514T)==0)) ){
//		memset( F514T, '\0', MONY_BUF_SIZE );
//		memmove( WORK0, F58T, MONY_BUF_SIZE );
//		m_Util.percent( F514T, WORK0, 1, 63, 0 );
//		F52c &= 0xf0;
//	}
/*-------------*/
	if( !(F515sw&0x800) ){
		if( m_pArith->l_test(F58T) == 0 ){
			memset( F515T, '\0', MONY_BUF_SIZE );
		}
		if( (F515sw&0x100) || (m_pArith->l_test(F58T)&&(m_pArith->l_test(F515T)==0)) ){
			memset( F515T, '\0', MONY_BUF_SIZE );
			memmove( WORK0, F58T, MONY_BUF_SIZE );
			m_Util.percent( F515T, WORK0, 1, 63, 0 );
		}
	}
	F515sw &= ~0x100;
/*-------------*/

	dsp_prs( IDC_ICSDIAGCTRL2, ID58T, F58T );
	dsp_prs( IDC_ICSDIAGCTRL2, ID515T, F515T );

	if( F515sw&0x800 ){//[Y]150116
//	if( F514sw&0x800 ){
		ChangeColor( IDC_ICSDIAGCTRL2, ID515T, 3 );
	}
	else{
		ChangeColor( IDC_ICSDIAGCTRL2, ID515T, 0 );
	}
}

//-----------------------------------------------------------------------------
// ‘æŽOŽíŽ–‹Æ
//-----------------------------------------------------------------------------
void CShinFhyo50EX::f5calq_ku3()
{
	char	WORK0[MONY_BUF_SIZE]={0};

	dsp_cls( IDC_ICSDIAGCTRL2, ID59T );
	dsp_cls( IDC_ICSDIAGCTRL2, ID516T );

/*- '14.03.01 -*/
//	if( m_pArith->l_test(F59T) == 0 ){
//		memset( F515T, '\0', MONY_BUF_SIZE );
//	}
//	
//	if( (F53c&0xff) || (m_pArith->l_test(F59T)&&(m_pArith->l_test(F515T)==0)) ){
//		memset( F515T, '\0', MONY_BUF_SIZE );
//		memmove( WORK0, F59T, MONY_BUF_SIZE );
//		m_Util.percent( F515T, WORK0, 1, 63, 0 );
//		F53c &= 0xf0;
//	}
/*-------------*/
	if( !(F516sw&0x800) ){
		if( m_pArith->l_test(F59T) == 0 ){
			memset( F516T, '\0', MONY_BUF_SIZE );
		}
		if( (F516sw&0x100) || (m_pArith->l_test(F59T)&&(m_pArith->l_test(F516T)==0)) ){
			memset( F516T, '\0', MONY_BUF_SIZE );
			memmove( WORK0, F59T, MONY_BUF_SIZE );
			m_Util.percent( F516T, WORK0, 1, 63, 0 );
		}
	}
	F516sw &= ~0x100;
/*-------------*/
	dsp_prs( IDC_ICSDIAGCTRL2, ID59T, F59T );
	dsp_prs( IDC_ICSDIAGCTRL2, ID516T, F516T );

	if( F516sw&0x800 ){
		ChangeColor( IDC_ICSDIAGCTRL2, ID516T, 3 );
	}
	else{
		ChangeColor( IDC_ICSDIAGCTRL2, ID516T, 0 );
	}
}

//-----------------------------------------------------------------------------
// ‘æŽlŽíŽ–‹Æ
//-----------------------------------------------------------------------------
void CShinFhyo50EX::f5calq_ku4()
{
	char	WORK0[MONY_BUF_SIZE]={0};

	dsp_cls( IDC_ICSDIAGCTRL2, ID510T );
	dsp_cls( IDC_ICSDIAGCTRL2, ID517T );
/*- '14.03.01 -*/
//	if( m_pArith->l_test(F510T) == 0 ){
//		memset( F516T, '\0', MONY_BUF_SIZE );
//	}
//	
//	if( (F54c&0xff) || (m_pArith->l_test(F510T)&&(m_pArith->l_test(F516T)==0)) ){
//		memset( F516T, '\0', MONY_BUF_SIZE );
//		memmove( WORK0, F510T, MONY_BUF_SIZE );
//		m_Util.percent( F516T, WORK0, 1, 63, 0 );
//		F54c &= 0xf0;
//	}
/*-------------*/
	if( !(F517sw&0x800) ){
		if( m_pArith->l_test(F510T) == 0 ){
			memset( F517T, '\0', MONY_BUF_SIZE );
		}
		
		if( (F517sw&0x100) || (m_pArith->l_test(F510T)&&(m_pArith->l_test(F517T)==0)) ){
			memset( F517T, '\0', MONY_BUF_SIZE );
			memmove( WORK0, F510T, MONY_BUF_SIZE );
			m_Util.percent( F517T, WORK0, 1, 63, 0 );
		}
	}
	F517sw &= ~0x100;
/*-------------*/
	dsp_prs( IDC_ICSDIAGCTRL2, ID510T, F510T );
	dsp_prs( IDC_ICSDIAGCTRL2, ID517T, F517T );

	if( F517sw&0x800 ){
		ChangeColor( IDC_ICSDIAGCTRL2, ID517T, 3 );
	}
	else{
		ChangeColor( IDC_ICSDIAGCTRL2, ID517T, 0 );
	}
}

//-----------------------------------------------------------------------------
// ‘æŒÜŽíŽ–‹Æ
//-----------------------------------------------------------------------------
void CShinFhyo50EX::f5calq_ku5()
{
	char	WORK0[MONY_BUF_SIZE]={0};

	dsp_cls( IDC_ICSDIAGCTRL2, ID511T );
	dsp_cls( IDC_ICSDIAGCTRL2, ID518T );
/*- '14.03.01 -*/
//	if( m_pArith->l_test(F511T) == 0 ){
//		memset( F517T, '\0', MONY_BUF_SIZE );
//	}
//	
//	if( (F55c&0xff) || (m_pArith->l_test(F511T)&&(m_pArith->l_test(F517T)==0)) ){
//		memset( F517T, '\0', MONY_BUF_SIZE );
//		memmove( WORK0, F511T, MONY_BUF_SIZE );
//		m_Util.percent( F517T, WORK0, 1, 63, 0 );
//		F55c &= 0xf0;
//	}
/*-------------*/
	if( !(F518sw&0x800) ){
		if( m_pArith->l_test(F511T) == 0 ){
			memset( F518T, '\0', MONY_BUF_SIZE );
		}
		
		if( (F518sw&0x100) || (m_pArith->l_test(F511T)&&(m_pArith->l_test(F518T)==0)) ){
			memset( F518T, '\0', MONY_BUF_SIZE );
			memmove( WORK0, F511T, MONY_BUF_SIZE );
			m_Util.percent( F518T, WORK0, 1, 63, 0 );
		}
	}
	F518sw &= ~0x100;
/*-------------*/
	dsp_prs( IDC_ICSDIAGCTRL2, ID511T, F511T );
	dsp_prs( IDC_ICSDIAGCTRL2, ID518T, F518T );

	if( F518sw&0x800 ){
		ChangeColor( IDC_ICSDIAGCTRL2, ID518T, 3 );
	}
	else{
		ChangeColor( IDC_ICSDIAGCTRL2, ID518T, 0 );
	}
}

//----->[Y]
//-----------------------------------------------------------------------------
// ‘æ˜ZŽíŽ–‹Æ
//-----------------------------------------------------------------------------
void CShinFhyo50EX::f5calq_ku6()
{
	char	WORK0[MONY_BUF_SIZE]={0};

	dsp_cls( IDC_ICSDIAGCTRL2, ID512T );
	dsp_cls( IDC_ICSDIAGCTRL2, ID519T );
/*- '14.03.01 -*/
//	if( m_pArith->l_test(F511T) == 0 ){
//		memset( F517T, '\0', MONY_BUF_SIZE );
//	}
//	
//	if( (F55c&0xff) || (m_pArith->l_test(F511T)&&(m_pArith->l_test(F517T)==0)) ){
//		memset( F517T, '\0', MONY_BUF_SIZE );
//		memmove( WORK0, F511T, MONY_BUF_SIZE );
//		m_Util.percent( F517T, WORK0, 1, 63, 0 );
//		F55c &= 0xf0;
//	}
/*-------------*/
	if( !(F519sw&0x800) ){
		if( m_pArith->l_test(F512T) == 0 ){
			memset( F519T, '\0', MONY_BUF_SIZE );
		}
		
		if( (F519sw&0x100) || (m_pArith->l_test(F512T)&&(m_pArith->l_test(F519T)==0)) ){
			memset( F519T, '\0', MONY_BUF_SIZE );
			memmove( WORK0, F512T, MONY_BUF_SIZE );
			m_Util.percent( F519T, WORK0, 1, 63, 0 );
		}
	}
	F519sw &= ~0x100;
/*-------------*/
	dsp_prs( IDC_ICSDIAGCTRL2, ID512T, F512T );
	dsp_prs( IDC_ICSDIAGCTRL2, ID519T, F519T );

	if( F519sw&0x800 ){
		ChangeColor( IDC_ICSDIAGCTRL2, ID519T, 3 );
	}
	else{
		ChangeColor( IDC_ICSDIAGCTRL2, ID519T, 0 );
	}
}
//<------------

//-----------------------------------------------------------------------------
// Ž–‹Æ•Ê‰ÛÅ”„ãŠ„‡‚ÌŒvŽZ
//-----------------------------------------------------------------------------
void CShinFhyo50EX::f5calq_ku()
{
	char	kani;

	kani = 0x00;
	m_typeCnt = 0;
	if( m_pArith->l_test(F57T) ){
		kani |= 0x01;
		m_typeCnt++;
	}
	if( m_pArith->l_test(F58T) ){
		kani |= 0x02;
		m_typeCnt++;
	}
	if( m_pArith->l_test(F59T) ){
		kani |= 0x04;
		m_typeCnt++;
	}
	if( m_pArith->l_test(F510T) ){
		kani |= 0x08;
		m_typeCnt++;
	}
	if( m_pArith->l_test(F511T) ){
		kani |= 0x10;
		m_typeCnt++;
	}
	if( m_pArith->l_test(F512T) ){
		kani |= 0x20;
		m_typeCnt++;
	}
	if( m_typeCnt == 1 ){
		CQsw |= 0x01;
		SZsw = SBsw = 0x00;
		if( F520sw&0x800 ){
			// •\Ž¦ƒNƒŠƒA
			ChangeColor( IDC_ICSDIAGCTRL2,  ID520T, 0 );
			F520sw = 0;
		}
		if( F521sw&0x800 ){
			// •\Ž¦ƒNƒŠƒA
			ChangeColor( IDC_ICSDIAGCTRL2,  ID521T, 0 );
			F521sw = 0;
		}
		if( F522sw&0x800 ){
			// •\Ž¦ƒNƒŠƒA
			ChangeColor( IDC_ICSDIAGCTRL2,  ID522T, 0 );
			F522sw = 0;
		}
		if( F523sw&0x800 ){
			// •\Ž¦ƒNƒŠƒA
			ChangeColor( IDC_ICSDIAGCTRL2,  ID523T, 0 );
			F523sw = 0;
		}
		if( F524sw&0x800 ){
			// •\Ž¦ƒNƒŠƒA
			ChangeColor( IDC_ICSDIAGCTRL2,  ID524T, 0 );
			F524sw = 0;
		}
		if( F525sw&0x800 ){
			// •\Ž¦ƒNƒŠƒA
			ChangeColor( IDC_ICSDIAGCTRL2,  ID525T, 0 );
			F525sw = 0;
		}
		if( F526sw&0x800 ){
			// •\Ž¦ƒNƒŠƒA
			ChangeColor( IDC_ICSDIAGCTRL2,  ID526T, 0 );
			F526sw = 0;
		}
		if( F527sw&0x800 ){
			// •\Ž¦ƒNƒŠƒA
			ChangeColor( IDC_ICSDIAGCTRL2,  ID527T, 0 );
			F527sw = 0;
		}
		if( F528sw&0x800 ){
			// •\Ž¦ƒNƒŠƒA
			ChangeColor( IDC_ICSDIAGCTRL2,  ID528T, 0 );
			F528sw = 0;
		}
		if( F529sw&0x800 ){
			// •\Ž¦ƒNƒŠƒA
			ChangeColor( IDC_ICSDIAGCTRL2,  ID529T, 0 );
			F529sw = 0;
		}
		if( F530sw&0x800 ){
			// •\Ž¦ƒNƒŠƒA
			ChangeColor( IDC_ICSDIAGCTRL2,  ID530T, 0 );
			F530sw = 0;
		}
		if( F531sw&0x800 ){
			// •\Ž¦ƒNƒŠƒA
			ChangeColor( IDC_ICSDIAGCTRL2,  ID531T, 0 );
			F531sw = 0;
		}
		if( F532sw&0x800 ){
			// •\Ž¦ƒNƒŠƒA
			ChangeColor( IDC_ICSDIAGCTRL2,  ID532T, 0 );
			F532sw = 0;
		}
		if( F533sw&0x800 ){
			// •\Ž¦ƒNƒŠƒA
			ChangeColor( IDC_ICSDIAGCTRL2,  ID533T, 0 );
			F533sw = 0;
		}
		if( F534sw&0x800 ){
			// •\Ž¦ƒNƒŠƒA
			ChangeColor( IDC_ICSDIAGCTRL2,  ID534T, 0 );
			F534sw = 0;
		}
		if( F535sw&0x800 ){
			// •\Ž¦ƒNƒŠƒA
			ChangeColor( IDC_ICSDIAGCTRL2,  ID535T, 0 );
			F535sw = 0;
		}
		if( F536sw&0x800 ){
			// •\Ž¦ƒNƒŠƒA
			ChangeColor( IDC_ICSDIAGCTRL2,  ID536T, 0 );
			F536sw = 0;
		}
	}
	else{
		CQsw &= 0xf0;
		if( m_typeCnt == 0 ){
			memset( F56T,  '\0', MONY_BUF_SIZE );
			memset( F56B,  '\0', MONY_BUF_SIZE );
			memset( F57T,  '\0', MONY_BUF_SIZE );
			memset( F57B,  '\0', MONY_BUF_SIZE );
			memset( F58T,  '\0', MONY_BUF_SIZE );
			memset( F58B,  '\0', MONY_BUF_SIZE );
			memset( F59T,  '\0', MONY_BUF_SIZE );
			memset( F59B,  '\0', MONY_BUF_SIZE );
			memset( F510T, '\0', MONY_BUF_SIZE );
			memset( F510B, '\0', MONY_BUF_SIZE );
			memset( F511T, '\0', MONY_BUF_SIZE );
			memset( F511B, '\0', MONY_BUF_SIZE );
			memset( F512T, '\0', MONY_BUF_SIZE );
			memset( F512B, '\0', MONY_BUF_SIZE );
			memset( F513T, '\0', MONY_BUF_SIZE );
			memset( F513B, '\0', MONY_BUF_SIZE );
			memset( F514T, '\0', MONY_BUF_SIZE );
			memset( F514B, '\0', MONY_BUF_SIZE );
			memset( F515T, '\0', MONY_BUF_SIZE );
			memset( F515B, '\0', MONY_BUF_SIZE );
			memset( F516T, '\0', MONY_BUF_SIZE );
			memset( F516B, '\0', MONY_BUF_SIZE );
			memset( F517T, '\0', MONY_BUF_SIZE );
			memset( F517B, '\0', MONY_BUF_SIZE );
			memset( F518T, '\0', MONY_BUF_SIZE );
			memset( F518B, '\0', MONY_BUF_SIZE );
			memset( F519T, '\0', MONY_BUF_SIZE );
			memset( F519B, '\0', MONY_BUF_SIZE );
			memset( F520T, '\0', MONY_BUF_SIZE );
			memset( F520B, '\0', MONY_BUF_SIZE );
			memset( F521T, '\0', MONY_BUF_SIZE );
			memset( F521B, '\0', MONY_BUF_SIZE );
			if( F56sw&0x800 ){
				// •\Ž¦ƒNƒŠƒA
				ChangeColor( IDC_ICSDIAGCTRL2,  ID56T, 0 );
				F56sw = 0;
			}
		}
	}

	f5calq_ku1();
	f5calq_ku2();
	f5calq_ku3();
	f5calq_ku4();
	f5calq_ku5();
	f5calq_ku6();//[Y]
	if( uwariai(0) ){
		f5calq_uws();
	}
	dsp_prs( IDC_ICSDIAGCTRL2, ID56T, F56T );
	
	f5calq_gts( 0 );
	if( m_pArith->l_cmp(F56S, F56T) || m_pArith->l_cmp(F56SS, F56B) ){
		CQsw &= 0x0f;
	}

	if( m_typeCnt > 1 ){		// •¡”Ž–‹ÆŽí‘I‘ðŽž‚Ì‚Ý•\Ž¦
		// Œ´‘¥ŒvŽZ‚ð“K—p‚·‚éê‡
		f5calq_gn();
		// “Á—áŒvŽZ‚ð“K—p‚·‚éê‡
		f5calq_tk();
	}
}

#ifdef _20140327_EXTEND_
//-----------------------------------------------------------------------------
// Œ´‘¥ŒvŽZ‚ð“K—p‚·‚éê‡
//-----------------------------------------------------------------------------
void CShinFhyo50EX::f5calq_gn()
{
	char	WORK0[MONY_BUF_SIZE]={0};
	char	WORK1[MONY_BUF_SIZE]={0};
	char	PW0[MONY_BUF_SIZE], PW1[MONY_BUF_SIZE], PW2[MONY_BUF_SIZE], val[MONY_BUF_SIZE];

	if( !(CQsw&0xff) ){
		memset( PW1,  '\0', MONY_BUF_SIZE );
		m_pArith->l_input( PW1, _T("1000") );
		memset( val,  '\0', MONY_BUF_SIZE );
		memset( PW2,   '\0', MONY_BUF_SIZE );
		memset( WORK1,'\0', MONY_BUF_SIZE );
		// ‘æˆêŽíŽ–‹Æ‚ÌŒvŽZ
		if( m_pArith->l_test(F513T) ){
			memmove( WORK1, F513T, MONY_BUF_SIZE );
		}
		sprintf_s( WORK0, sizeof(WORK0), _T("%d"), (1000 - (*m_pSnHeadData)->SVsper[0]) );
		memset( PW0, '\0', MONY_BUF_SIZE );
		m_pArith->l_input( PW0, WORK0 );
		if( m_pArith->l_test(WORK1) ){
			m_Util.l_6calq( WORK1, WORK1, PW0, PW1 );	// , 0 );
			m_pArith->l_add( val, val, WORK1 );
			memset( WORK1, '\0', MONY_BUF_SIZE );
		}
		// ‘æ“ñŽíŽ–‹Æ‚ÌŒvŽZ
		if( m_pArith->l_test(F514T) ){
			memmove( WORK1, F514T, MONY_BUF_SIZE );
		}
		sprintf_s( WORK0, sizeof( WORK0 ), "%d", ( 1000 - (*m_pSnHeadData)->SVsper[1] ) );
		memset( PW0, '\0', MONY_BUF_SIZE );
		m_pArith->l_input( PW0, WORK0 );
		if( m_pArith->l_test( WORK1 ) ){
			m_Util.l_6calq( WORK1, WORK1, PW0, PW1 );	// , 0 );
			m_pArith->l_add( val, val, WORK1 );
			memset( WORK1, '\0', MONY_BUF_SIZE );
		}
		// ‘æŽOŽíŽ–‹Æ‚ÌŒvŽZ
		if( m_pArith->l_test( F515T ) ){
			memmove( WORK1, F515T, MONY_BUF_SIZE );
		}
		sprintf_s( WORK0, sizeof( WORK0 ), "%d", ( 1000 - (*m_pSnHeadData)->SVsper[2] ) );
		memset( PW0, '\0', MONY_BUF_SIZE );
		m_pArith->l_input( PW0, WORK0 );
		if( m_pArith->l_test( WORK1 ) ){
			m_Util.l_6calq( WORK1, WORK1, PW0, PW1 );	// , 0 );
			m_pArith->l_add( val, val, WORK1 );
			memset( WORK1, '\0', MONY_BUF_SIZE );
		}
		// ‘æŽlŽíŽ–‹Æ‚ÌŒvŽZ
		if( m_pArith->l_test( F516T ) ){
			memmove( WORK1, F516T, MONY_BUF_SIZE );
		}
		sprintf_s( WORK0, sizeof( WORK0 ), "%d", ( 1000 - (*m_pSnHeadData)->SVsper[3] ) );
		memset( PW0, '\0', MONY_BUF_SIZE );
		m_pArith->l_input( PW0, WORK0 );
		if( m_pArith->l_test( WORK1 ) ){
			m_Util.l_6calq( WORK1, WORK1, PW0, PW1 );	//, 0 );
			m_pArith->l_add( val, val, WORK1 );
			memset( WORK1, '\0', MONY_BUF_SIZE );
		}
		// ‘æŒÜŽíŽ–‹Æ‚ÌŒvŽZ
		if( m_pArith->l_test( F517T ) ){
			memmove( WORK1, F517T, MONY_BUF_SIZE );
		}
		sprintf_s( WORK0, sizeof( WORK0 ), "%d", ( 1000 - (*m_pSnHeadData)->SVsper[4] ) );
		memset( PW0, '\0', MONY_BUF_SIZE );
		m_pArith->l_input( PW0, WORK0 );
		if( m_pArith->l_test( WORK1 ) ){
			m_Util.l_6calq( WORK1, WORK1, PW0, PW1 );	// , 0 );
			m_pArith->l_add( val, val, WORK1 );
			memset( WORK1, '\0', MONY_BUF_SIZE );
		}

		if( m_pArith->l_test(val) && m_pArith->l_test(F512T) ){
			m_pArith->l_par100( (unsigned char *)WORK0, (unsigned char *)val,  (unsigned char *)F512T, 7, 0 );
			m_pArith->l_mul100( (unsigned char *)WORK1, (unsigned char *)F54T, (unsigned char *)WORK0, 7, 0 );

			// Œ´‘¥Žè“ü—Í
			if( !(F518sw&0x800) ){
				memmove( F518T, WORK1, MONY_BUF_SIZE );
				ChangeColor( IDC_ICSDIAGCTRL2,  ID518T, 0 );
			}
			else{
/*- '14.03.01 -*/
//				if( F518sw & 0x01 ){
//					if( !m_pArith->l_cmp(F518T, WORK1) ){
//						F518c = 0x00;
//						ChangeColor( IDC_ICSDIAGCTRL2,  ID518T, 0 );
//					}
//				}
/*--------------*/
				if( !m_pArith->l_cmp(F518T, WORK1) ){
					ChangeColor( IDC_ICSDIAGCTRL2,  ID518T, 0 );
				}
				ChangeColor( IDC_ICSDIAGCTRL2,  ID518T, 3 );
/*--------------*/
			}
		}
	}

	if( !(CQsw&0x0f) ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID518T, F518T );
	}
	else{
		memset( F518T, '\0', MONY_BUF_SIZE );
		memset( F518B, '\0', MONY_BUF_SIZE );
		F518sw = SZsw = SBsw = 0x00;
		dsp_prs( IDC_ICSDIAGCTRL2, ID518T, F518T );
	}
}
#else
//-----------------------------------------------------------------------------
// Œ´‘¥ŒvŽZ‚ð“K—p‚·‚éê‡
//-----------------------------------------------------------------------------
void CShinFhyo50EX::f5calq_gn()
{
	char	val[MONY_BUF_SIZE]={0};

	if( !(CQsw&0xff) ){

		f50calq_skj_gn( val );
		if( m_pArith->l_test(val) ){
			// Œ´‘¥Žè“ü—Í
			if( !(F520sw&0x800) ){
				memmove( F520T, val, MONY_BUF_SIZE );
				ChangeColor( IDC_ICSDIAGCTRL2,  ID520T, 0 );
			}
			else{
/*- '14.03.01 -*/
//				if( F518sw & 0x01 ){
//					if( !m_pArith->l_cmp(F518T, WORK1) ){
//						F518c = 0x00;
//						ChangeColor( IDC_ICSDIAGCTRL2,  ID518T, 0 );
//					}
//				}
/*--------------*/
//				if( !m_pArith->l_cmp(F518T, val) ){
//					ChangeColor( IDC_ICSDIAGCTRL2, ID518T, 0 );
//				}
				ChangeColor( IDC_ICSDIAGCTRL2, ID520T, 3 );
/*--------------*/
			}
		}
		else{
			if( m_pArith->l_test(F54S) <= 0 ){
				memmove( F520T, val, MONY_BUF_SIZE );
				if( F520sw&0x800 ){
					F520sw &= ~0xf00;
				}
			}
			else{
				// Œ´‘¥Žè“ü—Í
				if( !(F520sw&0x800) ){
					memmove( F520T, val, MONY_BUF_SIZE );
					ChangeColor( IDC_ICSDIAGCTRL2,  ID520T, 0 );
				}
				else{
					ChangeColor( IDC_ICSDIAGCTRL2, ID520T, 3 );
				}
			}
		}
	}

	if( !(CQsw&0x0f) ){
		dsp_prs( IDC_ICSDIAGCTRL2, ID520T, F520T );
	}
	else{
		memset( F520T, '\0', MONY_BUF_SIZE );
		memset( F520C, '\0', MONY_BUF_SIZE );
		F520sw = SZsw = SBsw = 0x00;
		dsp_prs( IDC_ICSDIAGCTRL2, ID520T, F520T );
	}
}
#endif

//-----------------------------------------------------------------------------
// Tœ‘ÎÛŽd“üÅŠz‚ÌŒvŽZ(Œ´‘¥ŒvŽZ)
//-----------------------------------------------------------------------------
// ˆø”	dest	F	ŒvŽZŒ‹‰Ê
//-----------------------------------------------------------------------------
void CShinFhyo50EX::f50calq_skj_gn( char *dest )
{
	char	*pVal14=NULL, *pVal15=NULL, *pVal16=NULL, *pVal17=NULL, *pVal18=NULL, *pVal19=NULL;
//	char	*pVal13=NULL, *pVal14=NULL, *pVal15=NULL, *pVal16=NULL, *pVal17=NULL;
	char	*pVal4=NULL, *pVal13=NULL;
//	char	*pVal4=NULL, *pVal12=NULL;

	pVal14 = F514T;
	pVal15 = F515T;
	pVal16 = F516T;
	pVal17 = F517T;
	pVal18 = F518T;
	pVal19 = F519T;
	pVal4 = F54T;
	pVal13 = F513T;

	char	bfVal14[64]={0}, bfVal15[64]={0}, bfVal16[64]={0}, bfVal17[64]={0}, bfVal18[64]={0}, bfVal19[64]={0}, bfVal4[64]={0}, bfVal13[64]={0};
//	char	bfVal13[64]={0}, bfVal14[64]={0}, bfVal15[64]={0}, bfVal16[64]={0}, bfVal17[64]={0}, bfVal4[64]={0}, bfVal12[64]={0};
	m_pArith->l_print( bfVal14, pVal14, _T("sssssssssssss9") );
	m_pArith->l_print( bfVal15, pVal15, _T("sssssssssssss9") );
	m_pArith->l_print( bfVal16, pVal16, _T("sssssssssssss9") );
	m_pArith->l_print( bfVal17, pVal17, _T("sssssssssssss9") );
	m_pArith->l_print( bfVal18, pVal18, _T("sssssssssssss9") );
	m_pArith->l_print( bfVal19, pVal19, _T("sssssssssssss9") );
	m_pArith->l_print( bfVal4, pVal4, _T("sssssssssssss9") );
	m_pArith->l_print( bfVal13, pVal13, _T("sssssssssssss9") );

	double	dbVal14, dbVal15, dbVal16, dbVal17, dbVal18, dbVal19, dbVal4, dbVal13;
//	double	dbVal13, dbVal14, dbVal15, dbVal16, dbVal17, dbVal4, dbVal12;
	dbVal14 = strtod( bfVal14, 0 );
	dbVal15 = strtod( bfVal15, 0 );
	dbVal16 = strtod( bfVal16, 0 );
	dbVal17 = strtod( bfVal17, 0 );
	dbVal18 = strtod( bfVal18, 0 );
	dbVal19 = strtod( bfVal19, 0 );
	dbVal4 = strtod( bfVal4, 0 );
	dbVal13 = strtod( bfVal13, 0 );

//--> '14.04.01 INS START
	if( dbVal4 < 0 ){
		dbVal4 = 0.0;
	}
//<-- '14.04.01 INS END

//--> '15.01.26 INS START
	if( dbVal14 < 0 ){
		dbVal14 = 0.0;
	}
	if( dbVal15 < 0 ){
		dbVal15 = 0.0;
	}
	if( dbVal16 < 0 ){
		dbVal16 = 0.0;
	}
	if( dbVal17 < 0 ){
		dbVal17 = 0.0;
	}
	if( dbVal18 < 0 ){
		dbVal18 = 0.0;
	}
	if( dbVal19 < 0 ){
		dbVal19 = 0.0;
	}
//<-- '15.01.26 INS END


	double	ans;
	if( dbVal13 != 0 ){
/*- '14.07.02 -*/
//		ans = dbVal4 * (((dbVal13*0.9) + (dbVal14*0.8) + (dbVal15*0.7) + (dbVal16*0.6) + (dbVal17*0.5)) / dbVal12);
/*-------------*/
		ans = dbVal4 * ( (((dbVal14*900.0)+(dbVal15*800.0)+(dbVal16*700.0)+(dbVal17*600.0)+(dbVal18*500.0)+(dbVal19*400.0))/1000.0) / dbVal13 );
/*-------------*/
	}
	else{
		ans = 0.0;
	}
	if( ans > 0 ){
		ans = floor( ans );
	}
	else{
		ans = ceil( ans );
	}

	char	bfAns[64]={0};
	sprintf_s( bfAns, sizeof(bfAns), _T("%.0f"), ans );
	m_pArith->l_input( dest, bfAns );
}

//-----------------------------------------------------------------------------
// Tœ‘ÎÛŽd“üÅŠz‚ÌŒvŽZŽ®‹æ•ª‚Ì•\Ž¦
//-----------------------------------------------------------------------------
void CShinFhyo50EX::f5dsp_tk()
{
	dsp_cls( IDC_ICSDIAGCTRL2, ID521T );
	dsp_cls( IDC_ICSDIAGCTRL2, ID522T );
	dsp_cls( IDC_ICSDIAGCTRL2, ID523T );
	dsp_cls( IDC_ICSDIAGCTRL2, ID524T );
	dsp_cls( IDC_ICSDIAGCTRL2, ID525T );
	dsp_cls( IDC_ICSDIAGCTRL2, ID526T );
	dsp_cls( IDC_ICSDIAGCTRL2, ID527T );
	dsp_cls( IDC_ICSDIAGCTRL2, ID528T );
	dsp_cls( IDC_ICSDIAGCTRL2, ID529T );
	dsp_cls( IDC_ICSDIAGCTRL2, ID530T );
	dsp_cls( IDC_ICSDIAGCTRL2, ID531T );
	dsp_cls( IDC_ICSDIAGCTRL2, ID532T );
	dsp_cls( IDC_ICSDIAGCTRL2, ID533T );
	dsp_cls( IDC_ICSDIAGCTRL2, ID534T );
	dsp_cls( IDC_ICSDIAGCTRL2, ID535T );
	dsp_cls( IDC_ICSDIAGCTRL2, ID536T );

	char val[6] = {0};

	if( !(CQsw&0x0f) ){
		switch( SZsw ){
			case 1 :
				dsp_prs( IDC_ICSDIAGCTRL2, ID521T, F521T );
				memmove( val , F521T , sizeof( val ) );
				break;
			case 2 :
				dsp_prs( IDC_ICSDIAGCTRL2, ID522T, F522T );
				memmove( val , F522T , sizeof( val ) );
				break;
			case 3 :
				dsp_prs( IDC_ICSDIAGCTRL2, ID523T, F523T );
				memmove( val , F523T , sizeof( val ) );
				break;
			case 4 :
				dsp_prs( IDC_ICSDIAGCTRL2, ID524T, F524T );
				memmove( val , F524T , sizeof( val ) );
				break;
			case 5 :
				dsp_prs( IDC_ICSDIAGCTRL2, ID525T, F525T );
				memmove( val , F525T , sizeof( val ) );
				break;
			case 6 :
				dsp_prs( IDC_ICSDIAGCTRL2, ID526T, F526T );
				memmove( val , F526T , sizeof( val ) );
				break;
			case 7 :
				dsp_prs( IDC_ICSDIAGCTRL2, ID527T, F527T );
				memmove( val , F527T , sizeof( val ) );
				break;
			case 8 :
				dsp_prs( IDC_ICSDIAGCTRL2, ID528T, F528T );
				memmove( val , F528T , sizeof( val ) );
				break;
			case 9 :
				dsp_prs( IDC_ICSDIAGCTRL2, ID529T, F529T );
				memmove( val , F529T , sizeof( val ) );
				break;
			case 10:
				dsp_prs( IDC_ICSDIAGCTRL2, ID530T, F530T );
				memmove( val , F530T , sizeof( val ) );
				break;
			case 11:
				dsp_prs( IDC_ICSDIAGCTRL2, ID531T, F531T );
				memmove( val , F531T , sizeof( val ) );
				break;
			case 12:
				dsp_prs( IDC_ICSDIAGCTRL2, ID532T, F532T );
				memmove( val , F532T , sizeof( val ) );
				break;
			case 13:
				dsp_prs( IDC_ICSDIAGCTRL2, ID533T, F533T );
				memmove( val , F533T , sizeof( val ) );
				break;
			case 14:
				dsp_prs( IDC_ICSDIAGCTRL2, ID534T, F534T );
				memmove( val , F534T , sizeof( val ) );
				break;
			case 15:
				dsp_prs( IDC_ICSDIAGCTRL2, ID535T, F535T );
				memmove( val , F535T , sizeof( val ) );
				break;
			case 16:
				dsp_prs( IDC_ICSDIAGCTRL2, ID536T, F536T );
				memmove( val , F536T , sizeof( val ) );
				break;
			default:
				break;
		}

		if( !(*m_pSnHeadData)->Sn_TOKUR ){

			dsp_prs( IDC_ICSDIAGCTRL2, ID537T, F520T );
			memmove( F537T , F520T , sizeof( F520T ));
		}
		else{
			dsp_prs( IDC_ICSDIAGCTRL2, ID537T, val );
			memmove( F537T , val , sizeof( val ));
		}
		if( F521sw || F522sw || F523sw || F524sw || F525sw || F526sw || F527sw || F528sw || F529sw || F530sw || F531sw || F532sw || F533sw || F534sw || F535sw || F536sw ){
			if( !(F521sw & 0x800)){
				ChangeColor( IDC_ICSDIAGCTRL2,  ID521T, 0 );
				memset( F521T , '\0' , MONY_BUF_SIZE );
			}
			if( !(F522sw & 0x800)){
				ChangeColor( IDC_ICSDIAGCTRL2,  ID522T, 0 );
				memset( F522T , '\0' , MONY_BUF_SIZE );
			}
			if( !(F523sw & 0x800)){
				ChangeColor( IDC_ICSDIAGCTRL2,  ID523T, 0 );
				memset( F523T , '\0' , MONY_BUF_SIZE );
			}
			if( !(F524sw & 0x800)){
				ChangeColor( IDC_ICSDIAGCTRL2,  ID524T, 0 );
				memset( F524T , '\0' , MONY_BUF_SIZE );
			}
			if( !(F525sw & 0x800)){
				ChangeColor( IDC_ICSDIAGCTRL2,  ID525T, 0 );
				memset( F525T , '\0' , MONY_BUF_SIZE );
			}
			if( !(F526sw & 0x800)){
				ChangeColor( IDC_ICSDIAGCTRL2,  ID526T, 0 );
				memset( F526T , '\0' , MONY_BUF_SIZE );
			}
			if( !(F527sw & 0x800)){
				ChangeColor( IDC_ICSDIAGCTRL2,  ID527T, 0 );
				memset( F527T , '\0' , MONY_BUF_SIZE );
			}
			if( !(F528sw & 0x800)){
				ChangeColor( IDC_ICSDIAGCTRL2,  ID528T, 0 );
				memset( F528T , '\0' , MONY_BUF_SIZE );
			}
			if( !(F529sw & 0x800)){
				ChangeColor( IDC_ICSDIAGCTRL2,  ID529T, 0 );
				memset( F529T , '\0' , MONY_BUF_SIZE );
			}
			if( !(F530sw & 0x800)){
				ChangeColor( IDC_ICSDIAGCTRL2,  ID530T, 0 );
				memset( F530T , '\0' , MONY_BUF_SIZE );
			}
			if( !(F531sw & 0x800)){
				ChangeColor( IDC_ICSDIAGCTRL2,  ID531T, 0 );
				memset( F531T , '\0' , MONY_BUF_SIZE );
			}
			if( !(F532sw & 0x800)){
				ChangeColor( IDC_ICSDIAGCTRL2,  ID532T, 0 );
				memset( F532T , '\0' , MONY_BUF_SIZE );
			}
			if( !(F533sw & 0x800)){
				ChangeColor( IDC_ICSDIAGCTRL2,  ID533T, 0 );
				memset( F533T , '\0' , MONY_BUF_SIZE );
			}
			if( !(F534sw & 0x800)){
				ChangeColor( IDC_ICSDIAGCTRL2,  ID534T, 0 );
				memset( F534T , '\0' , MONY_BUF_SIZE );
			}
			if( !(F535sw & 0x800)){
				ChangeColor( IDC_ICSDIAGCTRL2,  ID535T, 0 );
				memset( F535T , '\0' , MONY_BUF_SIZE );
			}
			if( !(F536sw & 0x800)){
				ChangeColor( IDC_ICSDIAGCTRL2,  ID536T, 0 );
				memset( F536T , '\0' , MONY_BUF_SIZE );
			}
		}
	}
}

//-----------------------------------------------------------------------------
// “ü—Í’¼Œã‚Ì“Á—áŒvŽZƒTƒCƒ“
//-----------------------------------------------------------------------------
void CShinFhyo50EX::Tokurei50()
{
	char val[MONY_BUF_SIZE] = {0};

	switch(SZsw){
		case 1:
			memmove( val , F521T, MONY_BUF_SIZE );
			break;
		case 2:
			memmove( val , F522T, MONY_BUF_SIZE );
			break;
		case 3:
			memmove( val , F523T, MONY_BUF_SIZE );
			break;
		case 4:
			memmove( val , F524T, MONY_BUF_SIZE );
			break;
		case 5:
			memmove( val , F525T, MONY_BUF_SIZE );
			break;
		case 6:
			memmove( val , F526T, MONY_BUF_SIZE );
			break;
		case 7:
			memmove( val , F527T, MONY_BUF_SIZE );
			break;
		case 8:
			memmove( val , F528T, MONY_BUF_SIZE );
			break;
		case 9:
			memmove( val , F529T, MONY_BUF_SIZE );
			break;
		case 10:
			memmove( val , F530T, MONY_BUF_SIZE );
			break;
		case 11:
			memmove( val , F531T, MONY_BUF_SIZE );
			break;
		case 12:
			memmove( val , F532T, MONY_BUF_SIZE );
			break;
		case 13:
			memmove( val , F533T, MONY_BUF_SIZE );
			break;
		case 14:
			memmove( val , F534T, MONY_BUF_SIZE );
			break;
		case 15:
			memmove( val , F535T, MONY_BUF_SIZE );
			break;
		case 16:
			memmove( val , F536T, MONY_BUF_SIZE );
			break;
	}

	if( m_pArith->l_cmp(F520T, val) < 0 ){
		(*m_pSnHeadData)->Sn_TOKUR = 1;
		memmove( F537T , val , sizeof( val ));
	}
	else{
		(*m_pSnHeadData)->Sn_TOKUR = 0;
		memmove( F537T , F520T , sizeof( val ));
	}
}

//-----------------------------------------------------------------------------
// Tœ‘ÎÛŽd“üÅŠz‚ÌŒvŽZ
//-----------------------------------------------------------------------------
void CShinFhyo50EX::f5calq_tk()
{
	if( F521sw || F522sw || F523sw || F524sw || F525sw || F526sw || F527sw || F528sw || F529sw || F530sw || F531sw || F532sw || F533sw || F534sw || F535sw || F536sw ){
//	if( F519sw || F520sw || F521sw || F522sw || F523sw || F524sw || F525sw || F526sw || F527sw || F528sw || F529sw ){
		return;
	}
	int				i;
	unsigned short	szsg, per1, per2;
	char			PW0[MONY_BUF_SIZE], val1[MONY_BUF_SIZE], val2[MONY_BUF_SIZE], VAL[MONY_BUF_SIZE];
	char			WORK0[MONY_BUF_SIZE]={0};
	char			WORK1[MONY_BUF_SIZE]={0};
	memset( VAL , '\0' , MONY_BUF_SIZE );
	if( !(CQsw&0xff) ){
		SZsw = 1;
		szsg = per1 = per2 = 0;
		memset( F55T,  '\0', MONY_BUF_SIZE );
		memset( F521T, '\0', MONY_BUF_SIZE );
		if( (*m_pSnHonpyoData)->Sn_UP1 >= 750 ){
			per1 = (*m_pSnHeadData)->SVsper[0];
			if( m_typeCnt > 2 ){
				if( m_pArith->l_test(F58T) ){
					szsg |= 0x0001;
				}
				if( m_pArith->l_test(F59T) ){
					szsg |= 0x0002;
				}
				if( m_pArith->l_test(F510T) ){
					szsg |= 0x0004;
				}
				if( m_pArith->l_test(F511T) ){
					szsg |= 0x0008;
				}
				if( m_pArith->l_test(F512T) ){
					szsg |= 0x0010;
				}
			}
		}
		else if( (*m_pSnHonpyoData)->Sn_UP2 >= 750 ){
			per1 = (*m_pSnHeadData)->SVsper[1];
			if( m_typeCnt > 2 ){
				if( m_pArith->l_test(F57T) ){
					szsg |= 0x0001;
				}
				if( m_pArith->l_test(F59T) ){
					szsg |= 0x0020;
				}
				if( m_pArith->l_test(F510T) ){
					szsg |= 0x0040;
				}
				if( m_pArith->l_test(F511T) ){
					szsg |= 0x0080;
				}
				if( m_pArith->l_test(F512T) ){
					szsg |= 0x0100;
				}
			}
		}
		else if( (*m_pSnHonpyoData)->Sn_UP3 >= 750 ){
			per1 = (*m_pSnHeadData)->SVsper[2];
			if( m_typeCnt > 2 ){
				if( m_pArith->l_test(F57T) ){
					szsg |= 0x0002;
				}
				if( m_pArith->l_test(F58T) ){
					szsg |= 0x0020;
				}
				if( m_pArith->l_test(F510T) ){
					szsg |= 0x0200;
				}
				if( m_pArith->l_test(F511T) ){
					szsg |= 0x0400;
				}
				if( m_pArith->l_test(F512T) ){
					szsg |= 0x0800;
				}
			}
		}
		else if( (*m_pSnHonpyoData)->Sn_UP4 >= 750 ){
			per1 = (*m_pSnHeadData)->SVsper[3];
			if( m_typeCnt > 2 ){
				if( m_pArith->l_test(F57T) ){
					szsg |= 0x0004;
				}
				if( m_pArith->l_test(F58T) ){
					szsg |= 0x0040;
				}
				if( m_pArith->l_test(F59T) ){
					szsg |= 0x0200;
				}
				if( m_pArith->l_test(F511T) ){
					szsg |= 0x1000;
				}
				if( m_pArith->l_test(F512T) ){
					szsg |= 0x2000;
				}
			}
		}
		else if( (*m_pSnHonpyoData)->Sn_UP5 >= 750 ){
			per1 = (*m_pSnHeadData)->SVsper[4];
			if( m_typeCnt > 2 ){
				if( m_pArith->l_test(F57T) ){
					szsg |= 0x0008;
				}
				if( m_pArith->l_test(F58T) ){
					szsg |= 0x0080;
				}
				if( m_pArith->l_test(F59T) ){
					szsg |= 0x0400;
				}
				if( m_pArith->l_test(F510T) ){
					szsg |= 0x1000;
				}
				if( m_pArith->l_test(F512T) ){
					szsg |= 0x4000;
				}
			}
		}
		else if( (*m_pSnHonpyoData)->Sn_UP6 >= 750 ){
			per1 = (*m_pSnHeadData)->SVsper[5];
			if( m_typeCnt > 2 ){
				if( m_pArith->l_test(F57T) ){
					szsg |= 0x0010;
				}
				if( m_pArith->l_test(F58T) ){
					szsg |= 0x0100;
				}
				if( m_pArith->l_test(F59T) ){
					szsg |= 0x0800;
				}
				if( m_pArith->l_test(F510T) ){
					szsg |= 0x2000;
				}
				if( m_pArith->l_test(F511T) ){
					szsg |= 0x4000;
				}
			}
		}
		else{
			SZsw = 0;
//--- '15.03.10 ---
////-- '15.02.25 --
////			if( m_typeCnt <= 2 ){
////---------------
//			if( m_typeCnt < 2 ){
////---------------
//-----------------
			if( m_typeCnt <= 2 ){
//-----------------
				goto CALQ;
			}

#ifdef	CUT			
			if( (pSyzShin->Snd.Sn_UP1 + pSyzShin->Snd.Sn_UP2) >= 750 )	szsg |= 0x0001;
			if( (pSyzShin->Snd.Sn_UP1 + pSyzShin->Snd.Sn_UP3) >= 750 )	szsg |= 0x0002;
			if( (pSyzShin->Snd.Sn_UP1 + pSyzShin->Snd.Sn_UP4) >= 750 )	szsg |= 0x0004;
			if( (pSyzShin->Snd.Sn_UP1 + pSyzShin->Snd.Sn_UP5) >= 750 )	szsg |= 0x0008;
			if( (pSyzShin->Snd.Sn_UP2 + pSyzShin->Snd.Sn_UP3) >= 750 )	szsg |= 0x0010;
			if( (pSyzShin->Snd.Sn_UP2 + pSyzShin->Snd.Sn_UP4) >= 750 )	szsg |= 0x0020;
			if( (pSyzShin->Snd.Sn_UP2 + pSyzShin->Snd.Sn_UP5) >= 750 )	szsg |= 0x0040;
			if( (pSyzShin->Snd.Sn_UP3 + pSyzShin->Snd.Sn_UP4) >= 750 )	szsg |= 0x0080;
			if( (pSyzShin->Snd.Sn_UP3 + pSyzShin->Snd.Sn_UP5) >= 750 )	szsg |= 0x0100;
			if( (pSyzShin->Snd.Sn_UP4 + pSyzShin->Snd.Sn_UP5) >= 750 )	szsg |= 0x0200;
#endif

			// ŠÈˆÕ‰ÛÅ@2Ží—ÞˆÈã‚Ì‹ÆŽí‚Å‚Ì“Á—áŒvŽZƒ`ƒFƒbƒN
			if( f5_toku75(F56T, F57T,  F58T) ){
				szsg |= 0x0001;
			}
			if( f5_toku75(F56T, F57T,  F59T) ){
				szsg |= 0x0002;
			}
			if( f5_toku75(F56T, F57T,  F510T) ){
				szsg |= 0x0004;
			}
			if( f5_toku75(F56T, F57T,  F511T) ){
				szsg |= 0x0008;
			}
			if( f5_toku75(F56T, F57T,  F512T) ){
				szsg |= 0x0010;
			}

			if( f5_toku75(F56T, F58T,  F59T) ){
				szsg |= 0x0020;
			}
			if( f5_toku75(F56T, F58T,  F510T) ){
				szsg |= 0x0040;
			}
			if( f5_toku75(F56T, F58T,  F511T) ){
				szsg |= 0x0080;
			}
			if( f5_toku75(F56T, F58T,  F512T) ){
				szsg |= 0x0100;
			}


			if( f5_toku75(F56T, F59T,  F510T) ){
				szsg |= 0x0200;
			}
			if( f5_toku75(F56T, F59T,  F511T) ){
				szsg |= 0x0400;
			}
			if( f5_toku75(F56T, F59T,  F512T) ){
				szsg |= 0x0800;
			}

			if( f5_toku75( F56T, F510T, F511T) ){
				szsg |= 0x1000;
			}
			if( f5_toku75( F56T, F510T, F512T) ){
				szsg |= 0x2000;
			}

			if( f5_toku75( F56T, F511T, F512T) ){
				szsg |= 0x4000;
			}
		}
		
		memset( PW0, '\0', MONY_BUF_SIZE );
		m_pArith->l_input( PW0, _T("1000") );
//		VAL = (char *) &F519T[0];
		if( SZsw && m_pArith->l_test(F54T) ){
			memset( WORK1, '\0', MONY_BUF_SIZE );
			sprintf_s( WORK0, sizeof( WORK0 ), _T("%d"), ( 1000 - per1 ) );
			m_pArith->l_input( WORK1, WORK0 );
			m_Util.l_6calq( VAL, F54T, WORK1, PW0 );	// , 0 );
		}

		char	nume1[MONY_BUF_SIZE], nume2[MONY_BUF_SIZE], rate1[MONY_BUF_SIZE], rate2[MONY_BUF_SIZE];
		for( i=0; i<15; ++i, (szsg >>= 1) ){
			if( szsg & 0x01 ) {
				memset( val1, '\0', MONY_BUF_SIZE );
				memset( val2, '\0', MONY_BUF_SIZE );
				memset( WORK1, '\0', MONY_BUF_SIZE );
				memset( nume1, '\0', MONY_BUF_SIZE );
				memset( nume2, '\0', MONY_BUF_SIZE );
				memset( rate1, '\0', MONY_BUF_SIZE );
				memset( rate2, '\0', MONY_BUF_SIZE );

				switch( i ){
					case 14 :
						memmove( val1, F518T, MONY_BUF_SIZE );
						memmove( nume1, F518T, MONY_BUF_SIZE );
						per1 = (*m_pSnHeadData)->SVsper[4];
						break;
					case 13 :
					case 12:
						memmove( val1, F517T, MONY_BUF_SIZE );
						memmove( nume1, F517T, MONY_BUF_SIZE );
						per1 = (*m_pSnHeadData)->SVsper[3];
						break;
					case 11 :
					case 10 :
					case 9 :
						memmove( val1, F516T, MONY_BUF_SIZE );
						memmove( nume1, F516T, MONY_BUF_SIZE );
						per1 = (*m_pSnHeadData)->SVsper[2];
						break;
					case 8 :
					case 7 :
					case 6 :
					case 5 :
						memmove( val1, F515T, MONY_BUF_SIZE );
						memmove( nume1, F515T, MONY_BUF_SIZE );
						per1 = (*m_pSnHeadData)->SVsper[1];
						break;

					default:
						memmove( val1, F514T, MONY_BUF_SIZE );
						memmove( nume1, F514T, MONY_BUF_SIZE );
						per1 = (*m_pSnHeadData)->SVsper[0];
						break;
				}
				switch( i ){
					//~‚W‚O“
					case 0 :
						per2 = (*m_pSnHeadData)->SVsper[1];
						break;
					//~‚V‚O“
					case 1 :
					case 5 :
						per2 = (*m_pSnHeadData)->SVsper[2];
						break;
					//~‚U‚O“
					case 2 :
					case 6 : 
					case 9 :
						per2 = (*m_pSnHeadData)->SVsper[3];
						break;
					//~‚T‚O“
					case 3 :
					case 7 : 
					case 10 :
					case 12 :
						per2 = (*m_pSnHeadData)->SVsper[4];
						break;
					//~‚S‚O“
					default:
						per2 = (*m_pSnHeadData)->SVsper[5];
						break;
				}
				m_pArith->l_sub( val2, F513T, val1 );
				memmove( nume2, val2, MONY_BUF_SIZE );

				sprintf_s( WORK0, sizeof( WORK0 ), "%d", ( 1000 - per1 ) );
				m_pArith->l_input( WORK1, WORK0 );
				m_pArith->l_input( rate1, WORK0 );
				if( m_pArith->l_test(val1) ){
					m_Util.l_6calq( val1, val1, WORK1, PW0 );	// , 0 );
				}
				sprintf_s( WORK0, sizeof( WORK0 ), "%d", ( 1000 - per2 ) );
				m_pArith->l_input( WORK1, WORK0 );
				m_pArith->l_input( rate2, WORK0 );
				if( m_pArith->l_test( val2 ) ){
					m_Util.l_6calq( val2, val2, WORK1, PW0 );	//, 0 );
				}
				m_pArith->l_add( val1, val1, val2 );
				
				if( m_pArith->l_test(val1) ){
/*- '14.03.27 -*/
//					m_pArith->l_par100( (unsigned char *)WORK1, (unsigned char *)val1, (unsigned char *)F512T, 7, 0 );
//					m_pArith->l_mul100( (unsigned char *)WORK0, (unsigned char *)F54T, (unsigned char *)WORK1, 7, 0 );
/*-------------*/
					f50calq_skj_tk( WORK0, F54T,  nume1, rate1, nume2, rate2, F513T );
/*-------------*/
				}
				else{
					memset( WORK0, '\0', MONY_BUF_SIZE );
				}
				
				if( m_pArith->l_cmp(VAL, WORK0) < 0 ){
					if( (SZsw==1) && (m_typeCnt<=2) ){
						SZsw = 0;
						goto CALQ;
					}
					memmove( VAL, WORK0, MONY_BUF_SIZE );
					SZsw = (char) (i + 2);
				}
			}
			switch(SZsw){
				case 1:
					memmove( F521T , VAL, MONY_BUF_SIZE );
					break;
				case 2:
					memmove( F522T , VAL, MONY_BUF_SIZE );
					break;
				case 3:
					memmove( F523T , VAL, MONY_BUF_SIZE );
					break;
				case 4:
					memmove( F524T , VAL, MONY_BUF_SIZE );
					break;
				case 5:
					memmove( F525T , VAL, MONY_BUF_SIZE );
					break;
				case 6:
					memmove( F526T , VAL, MONY_BUF_SIZE );
					break;
				case 7:
					memmove( F527T , VAL, MONY_BUF_SIZE );
					break;
				case 8:
					memmove( F528T , VAL, MONY_BUF_SIZE );
					break;
				case 9:
					memmove( F529T , VAL, MONY_BUF_SIZE );
					break;
				case 10:
					memmove( F530T , VAL, MONY_BUF_SIZE );
					break;
				case 11:
					memmove( F531T , VAL, MONY_BUF_SIZE );
					break;
				case 12:
					memmove( F532T , VAL, MONY_BUF_SIZE );
					break;
				case 13:
					memmove( F533T , VAL, MONY_BUF_SIZE );
					break;
				case 14:
					memmove( F534T , VAL, MONY_BUF_SIZE );
					break;
				case 15:
					memmove( F535T , VAL, MONY_BUF_SIZE );
					break;
				case 16:
					memmove( F536T , VAL, MONY_BUF_SIZE );
					break;
			}

		}
	}

CALQ:
	f5dsp_tk();
}

//-----------------------------------------------------------------------------
// Tœ‘ÎÛŽd“üÅŠz‚ÌŒvŽZ(“Á—áŒvŽZ)
//-----------------------------------------------------------------------------
// ˆø”	dest	F	ŒvŽZŒ‹‰Ê
//		src		F	”íæŽZ”
//		num1	F	•ªŽqi”íæŽZ”‚Pj
//		rate1	F	•ªŽqi”íæŽZŠ„‡‚Pj
//		num2	F	•ªŽqi”íæŽZ”‚Qj
//		rate2	F	•ªŽqi”íæŽZŠ„‡‚Qj
//		deno	F	•ª•ê
//-----------------------------------------------------------------------------
void CShinFhyo50EX::f50calq_skj_tk( char *dest, const char *src, const char *num1, const char *rate1, const char *num2, const char *rate2, const char *deno )
{
	// ¡‰ñ‚ÍAWŒvƒ‚ƒWƒ…[ƒ‹‚ðŽg—p‚¹‚¸‚ÉŽ©‘O‚ÅŒvŽZ
	char	bfSrc[64]={0}, bfNum1[64]={0}, bfRate1[64]={0}, bfNum2[64]={0}, bfRate2[64]={0}, bfDeno[64]={0};
	m_pArith->l_print( bfSrc, (char*)src, _T("sssssssssssss9") );
	m_pArith->l_print( bfNum1, (char*)num1, _T("sssssssssssss9") );
	m_pArith->l_print( bfRate1, (char*)rate1, _T("sssssssssssss9") );
	m_pArith->l_print( bfNum2, (char*)num2, _T("sssssssssssss9") );
	m_pArith->l_print( bfRate2, (char*)rate2, _T("sssssssssssss9") );
	m_pArith->l_print( bfDeno, (char*)deno, _T("sssssssssssss9") );

	double	dbSrc, dbNum1, dbRate1, dbNum2, dbRate2, dbDeno;
	dbSrc = strtod( bfSrc, 0 );
	dbNum1 = strtod( bfNum1, 0 );
	dbRate1 = strtod( bfRate1, 0 );
	dbNum2 = strtod( bfNum2, 0 );
	dbRate2 = strtod( bfRate2, 0 );
	dbDeno = strtod( bfDeno, 0 );

	double	ans;
/*- '14.07.02 -*/
//	ans = dbSrc * ( ((dbNum1*(dbRate1/1000.0)) + (dbNum2*(dbRate2/1000.0))) );
/*-------------*/
	ans = dbSrc * ( ((dbNum1*dbRate1)+(dbNum2*dbRate2)) / 1000.0 );
/*-------------*/
	if( dbDeno != 0.0 ){
		ans = ans / dbDeno;
	}
	else{
		ans = 0.0;
	}

	if( ans > 0 ){
		ans = floor( ans );
	}
	else{
		ans = ceil( ans );
	}

	char	bfAns[64]={0};
	sprintf_s( bfAns, sizeof(bfAns), _T("%.0f"), ans );
	m_pArith->l_input( dest, bfAns );
}

//-----------------------------------------------------------------------------
// Ž–‹Æ‹æ•ª•Ê‚Ì‡ŒvŠz‚ÌŒvŽZ
//-----------------------------------------------------------------------------
int CShinFhyo50EX::f5calq_6()
{
	// ‰ÛÅ”„ã‚F‡ŽZ
	if( !(F56sw&0x800) ){
		m_pArith->l_add( F56T, F57T, F58T );
		m_pArith->l_add( F56T, F56T, F59T );
		m_pArith->l_add( F56T, F56T, F510T );
		m_pArith->l_add( F56T, F56T, F511T );
		m_pArith->l_add( F56T, F56T, F512T );
//--> '15.02.10 INS START
		if( m_pArith->l_test(F56T) < 0 ){
			m_pArith->l_clear( F56T );
		}
//<-- '15.02.10 INS END
	}

	dsp_prs( IDC_ICSDIAGCTRL2, ID56T, F56T );

	if( F56sw&0x800 ){
		ChangeColor( IDC_ICSDIAGCTRL2, ID56T, 3 );
	}
	else{
		ChangeColor( IDC_ICSDIAGCTRL2, ID56T, 0 );
	}

	return 0;
}

//-----------------------------------------------------------------------------
// Še“ü—Í‚ÌŒvŽZ•\Ž¦
//-----------------------------------------------------------------------------
// ˆø”	atno	F
//-----------------------------------------------------------------------------
void CShinFhyo50EX::f5calq_all( int atno )
{
	while( 1 ){
		switch( atno ){
			case	1 :					// ‰ÛÅ•W€ŠzÁ”ïÅŠz
			case	2 :					// ‘Ý“|‰ñŽûÁ”ïÅŠz
			case	3 :					// ”„ã‘Î‰¿‚Ì•ÔŠÒ“™Á”ïÅŠz
				f5calq_4();				// i‚Pj{i‚Qj|i‚Rj
				f5calq_5();				// ’PˆêŽ–‹Æ‚Ìê‡
				f5calq_6();
			case	4 :
			case	5 :
			case	6 :
			case	7 :
			case	8 :
			case	9 :
				if( !m_Tmp ){
					f5calq_ku();		// Ž–‹Æ•Ê‰ÛÅ”„ã‚‚ÌŒvŽZ
				}
			case	10:
			case	11:
			case	12:
			case	13:
			case	14:
			case	15:
				if( m_typeCnt > 1 ){	// •¡”Ž–‹ÆŽí‘I‘ðŽž‚Ì‚Ý
					f5calq_gts( 0 );		
				}
			default	:
				break;
		}
		break;
	}
}

//-----------------------------------------------------------------------------
// ŠÈˆÕ‰ÛÅ@2Ží—ÞˆÈã‚Ì‹ÆŽí‚Å‚Ì“Á—áŒvŽZƒ`ƒFƒbƒN
//-----------------------------------------------------------------------------
// ˆø”	Base	F	
//		Add1	F	
//		Add2	F	
//-----------------------------------------------------------------------------
// •Ô‘—’l	1	F	75% ˆÈã
//			0	F	75% –¢–ž
//-----------------------------------------------------------------------------
int CShinFhyo50EX::f5_toku75( char *Base, char *Add1, char *Add2  )
{
	char	EXPW0[MONY_BUF_SIZE], EXPW1[MONY_BUF_SIZE], KZMY0[MONY_BUF_SIZE], KZMY1[MONY_BUF_SIZE], KZMY2[MONY_BUF_SIZE];

	// ŒvŽZ€”õ
	memset( EXPW0, '\0', MONY_BUF_SIZE );
	memset( EXPW1, '\0', MONY_BUF_SIZE );
	m_pArith->l_input( EXPW0, _T("750") );
	m_pArith->l_input( EXPW1, _T("1000") );

	// ‘I‘ð‚³‚ê‚½2‹ÆŽí‚ð‡Œv‚·‚é
	memset( KZMY0, '\0', 6 );
	m_pArith->l_add( KZMY0, Add1, Add2 );

	// Ž–‹Æ‹æ•ª‡Œv
	memset( KZMY1, '\0', 6 );
	memmove( KZMY1, Base, 6 );

	// ‘I‘ð‚³‚ê‚½2‹ÆŽí‚ð‡Œv‚ðŠî‚ÉŠ„‡ŒvŽZ
	memset( KZMY2, '\0', 6 );
	m_Util.l_6calq( KZMY2, KZMY0, EXPW1, KZMY1 );	// \¬”ä
	
	// “Á—á‚V‚T“”äŠr
	int	st;
	if( m_pArith->l_cmp( KZMY2, EXPW0 ) >= 0 ){
		st = 1;	// 75%ˆÈã
	}
	else{
		st = 0;
	}

	return st;
}

//-----------------------------------------------------------------------------
// ‘¼•\‚©‚ç‚ÌŒvŽZ
//-----------------------------------------------------------------------------
void CShinFhyo50EX::calq_by_OtherChange()
{
	m_DpSw = -1;
	ReadData( 1 );
	f5calq_all( 1 );
	
	// •¡”Ž–‹ÆŽí‘I‘ðŽž‚Ì‚Ý•\Ž¦
	if( m_typeCnt > 1 ){
		// Œ´‘¥ŒvŽZ‚ð“K—p‚·‚éê‡
		f5calq_gn();
		// “Á—áŒvŽZ‚ð“K—p‚·‚éê‡
		f5calq_tk();
	}
	
	WriteData();
	m_DpSw = 0;
}

//-----------------------------------------------------------------------------
// \¬”ä‚ÌŒvŽZ
//-----------------------------------------------------------------------------
// ˆø”	mm	F
//		tt	F
//-----------------------------------------------------------------------------
// •Ô‘—’l	F	\¬”ä
//-----------------------------------------------------------------------------
int CShinFhyo50EX::upersnt( char *mm, char *tt )
{
	char	w1[MONY_BUF_SIZE]={0};
	char	buf[64]={0};

	m_pArith->l_par100( (unsigned char *)w1, (unsigned char *)mm, (unsigned char *)tt, 1, 0 );
	m_pArith->l_print( buf, w1, "9999" );
	return atoi(buf);
}

//-----------------------------------------------------------------------------
// ”„ã\¬”ä‚ÌŒvŽZ
//-----------------------------------------------------------------------------
// ˆø”	ssg	F
//-----------------------------------------------------------------------------
// •Ô‘—’l	1	F	•ÏX—L‚è
//			0	F	•ÏX–³‚µ
//-----------------------------------------------------------------------------
int CShinFhyo50EX::uwariai( char ssg )
{
	char	m1[MONY_BUF_SIZE]={0}, m2[MONY_BUF_SIZE]={0}, m3[MONY_BUF_SIZE]={0}, m4[MONY_BUF_SIZE]={0}, m5[MONY_BUF_SIZE]={0}, m6[MONY_BUF_SIZE]={0}, tt[MONY_BUF_SIZE]={0};
//	char	m1[MONY_BUF_SIZE]={0}, m2[MONY_BUF_SIZE]={0}, m3[MONY_BUF_SIZE]={0}, m4[MONY_BUF_SIZE]={0}, m5[MONY_BUF_SIZE]={0}, tt[MONY_BUF_SIZE]={0};
	short	ps1=0,   ps2=0,   ps3=0,   ps4=0,   ps5=0,   ps6=0,   st=0;
//	short	ps1,   ps2,   ps3,   ps4,   ps5,   st;


	// ‘æ‚PŽíŽ–‹Æ‰ÛÅ”„ã
	memmove( m1, F57T, MONY_BUF_SIZE );

	// ‘æ‚QŽíŽ–‹Æ‰ÛÅ”„ã
	memmove( m2, F58T, MONY_BUF_SIZE );

	// ‘æ‚RŽíŽ–‹Æ‰ÛÅ”„ã 
	memmove( m3, F59T, MONY_BUF_SIZE );

	// ‘æ‚SŽíŽ–‹Æ‰ÛÅ”„ã
	memmove( m4, F510T, MONY_BUF_SIZE );
	
	// ‘æ‚TŽíŽ–‹Æ‰ÛÅ”„ã
	memmove( m5, F511T, MONY_BUF_SIZE );

	// ‘æ‚UŽíŽ–‹Æ‰ÛÅ”„ã
	memmove( m6, F512T, MONY_BUF_SIZE );
	
	// ‰ÛÅ”„ã‡Œv
	memmove( tt, F56T, MONY_BUF_SIZE );
	
	if ( m_pArith->l_test(tt) == 0 ){
		if( ssg ){
			if( F51UPb || F52UPb || F53UPb || F54UPb || F55UPb || F56UPb ){
				st = 1;
			}
			else{
				st = 0;
			}
			F51UPb = F52UPb = F53UPb = F54UPb = F55UPb = F56UPb = 0;
		}
		else{
			if( (*m_pSnHonpyoData)->Sn_UP1 || (*m_pSnHonpyoData)->Sn_UP2 || (*m_pSnHonpyoData)->Sn_UP3 ||
				(*m_pSnHonpyoData)->Sn_UP4 || (*m_pSnHonpyoData)->Sn_UP5 || (*m_pSnHonpyoData)->Sn_UP6 ){
				st = 1;
			}
			else{
				st = 0;
			}
			(*m_pSnHonpyoData)->Sn_UP1 = (*m_pSnHonpyoData)->Sn_UP2 = (*m_pSnHonpyoData)->Sn_UP3 = 0;
			(*m_pSnHonpyoData)->Sn_UP4 = (*m_pSnHonpyoData)->Sn_UP5 = (*m_pSnHonpyoData)->Sn_UP6 = 0;
		}
		return st;
	}

//-- '15.01.26 --
//	ps1 = (short) upersnt( m1, tt );
//	ps2 = (short) upersnt( m2, tt );
//	ps3 = (short) upersnt( m3, tt );
//	ps4 = (short) upersnt( m4, tt );
//	ps5 = (short) upersnt( m5, tt );
//	ps6 = (short) upersnt( m6, tt );
//---------------
	if( m_pArith->l_test(m1) >= 0 ){
		ps1 = (short) upersnt( m1, tt );
		if( ps1 > 1000 ){
			ps1 = 1000;
		}
	}
	if( m_pArith->l_test(m2) >= 0 ){
		ps2 = (short) upersnt( m2, tt );
		if( ps2 > 1000 ){
			ps2 = 1000;
		}
	}
	if( m_pArith->l_test(m3) >= 0 ){
		ps3 = (short) upersnt( m3, tt );
		if( ps3 > 1000 ){
			ps3 = 1000;
		}
	}
	if( m_pArith->l_test(m4) >= 0 ){
		ps4 = (short) upersnt( m4, tt );
		if( ps4 > 1000 ){
			ps4 = 1000;
		}
	}
	if( m_pArith->l_test(m5) >= 0 ){
		ps5 = (short) upersnt( m5, tt );
		if( ps5 > 1000 ){
			ps5 = 1000;
		}
	}
	if( m_pArith->l_test(m6) >= 0 ){
		ps6 = (short) upersnt( m6, tt );
		if( ps6 > 1000 ){
			ps6 = 1000;
		}
	}
//---------------
	
	st = 0;
	if( ssg ){
		if( ps1 != F51UPb ){
			F51UPb = ps1;
			st = 1;
		}
		if( ps2 != F52UPb ){
			F52UPb = ps2;
			st = 1;
		}
		if( ps3 != F53UPb ){
			F53UPb = ps3;
			st = 1;
		}
		if( ps4 != F54UPb ){ 
			F54UPb = ps4; 
			st = 1;
		}
		if( ps5 != F55UPb ){ 
			F55UPb = ps5;
			st = 1;
		}
		if( ps6 != F56UPb ){ 
			F56UPb = ps6;
			st = 1;
		}
	}
	else{
		if( ps1 != (*m_pSnHonpyoData)->Sn_UP1 ){
			(*m_pSnHonpyoData)->Sn_UP1 = ps1;
			st = 1;
		}
		if( ps2 != (*m_pSnHonpyoData)->Sn_UP2 ){
			(*m_pSnHonpyoData)->Sn_UP2 = ps2;
			st = 1;
		}
		if( ps3 != (*m_pSnHonpyoData)->Sn_UP3 ){ 
			(*m_pSnHonpyoData)->Sn_UP3 = ps3;
			st = 1;
		}
		if( ps4 != (*m_pSnHonpyoData)->Sn_UP4 ){ 
			(*m_pSnHonpyoData)->Sn_UP4 = ps4;
			st = 1; 
		}
		if( ps5 != (*m_pSnHonpyoData)->Sn_UP5 ){
			(*m_pSnHonpyoData)->Sn_UP5 = ps5; 
			st = 1;
		}
		if( ps6 != (*m_pSnHonpyoData)->Sn_UP6 ){
			(*m_pSnHonpyoData)->Sn_UP6 = ps6; 
			st = 1;
		}
	}
	return st;
}

