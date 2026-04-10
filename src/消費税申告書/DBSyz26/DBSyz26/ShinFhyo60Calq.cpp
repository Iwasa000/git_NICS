#include "stdafx.h"
#include "ShinFhyo60.h"
#include "ShinFhyo60Idx.h"

#include <math.h>


void CShinFhyo60::f6calq_1to6()
{
	int				i, j;

	CString str = _T("");

	char	PW0[6], PW1[6];
	memset( PW0, '\0', 6 );
	m_pArith->l_input( PW0, _T("1000") );
	memset( PW1, '\0', 6 );
	m_pArith->l_input( PW1, _T("100") );


	char	WORK0[MONY_BUF_SIZE] = {0};
	char	WORK_ALL[MONY_BUF_SIZE] = {0};
	// ŒvŽZŒ‹‰Êƒoƒbƒtƒ@
	struct _ANS {
		unsigned char	val[MONY_BUF_SIZE];
	};
	struct _ANS		ANS[30], JANS[10];
	::ZeroMemory( &ANS , sizeof( ANS ) );
	::ZeroMemory( &JANS , sizeof( JANS ) );

	char kingaku[MONY_BUF_SIZE] = {0};
	char kingaku2[MONY_BUF_SIZE] = {0};
	char kingaku3[MONY_BUF_SIZE] = {0};
	char WORK2[MONY_BUF_SIZE] = {0};
	char WORK3[MONY_BUF_SIZE] = {0};
	char WORK7[MONY_BUF_SIZE] = {0};
	char WORK8[MONY_BUF_SIZE] = {0};
	char WORK9[MONY_BUF_SIZE] = {0};
	char val100[6] = {0};
	m_pArith->l_input( val100 , "100" );

	CH26SnFhyo10Data*	PrintData;

	if( *m_pSnFhyo10Data ){
		//•t•\1ƒf[ƒ^“Çž‚ÝƒNƒ‰ƒX
		PrintData = *m_pSnFhyo10Data;
	}
	else{
		//•t•\4ƒf[ƒ^“Çž‚ÝƒNƒ‰ƒX
		PrintData = *m_pSnFhyo40Data;
	}

	//‡@
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	if( (*m_pSnHeadData)->SVmzsw == 1 ){//Œo‰ß‘[’u‚ª‚ ‚éê‡
		m_pArith->l_add( kingaku , PrintData->Sn_1F1A , PrintData->Sn_1F1B );
	}
	m_pArith->l_add( kingaku , kingaku , PrintData->Sn_1F1C );
	memmove( ANS[0].val , kingaku , MONY_BUF_SIZE);

	//‡A
	if( (*m_pSnHeadData)->SVmzsw == 1 ){//Œo‰ß‘[’u‚ª‚ ‚éê‡
		m_pArith->l_add( WORK2 , PrintData->Sn_1F2A , PrintData->Sn_1F2B );
		m_pArith->l_add( WORK2 , WORK2 , PrintData->Sn_1F2C );
	}
	else{
		memmove( WORK2 , (*m_pSnHonpyoData)->Sn_SYTX , MONY_BUF_SIZE);
	}
	memmove( ANS[1].val , WORK2 , MONY_BUF_SIZE);

	//‡B
	memset( kingaku3 , '\0' , MONY_BUF_SIZE);
	if( (*m_pSnHeadData)->SVmzsw == 1 ){//Œo‰ß‘[’u‚ª‚ ‚éê‡
		m_pArith->l_add( WORK3 , PrintData->Sn_1F3A , PrintData->Sn_1F3B );
	}
	m_pArith->l_add( WORK3 , WORK3 , PrintData->Sn_1F3C );
	memmove( ANS[2].val , WORK3 , MONY_BUF_SIZE);
	//‡C
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	if( *m_pSnFhyo40Data ){
		if( (*m_pSnHeadData)->SVmzsw == 1 ){//Œo‰ß‘[’u‚ª‚ ‚éê‡
			m_pArith->l_add( kingaku , kingaku , PrintData->Sn_1F4T );
		}
		else{
			m_pArith->l_add( kingaku , kingaku , PrintData->Sn_1F4C );
		}
	}
	else{
		if( (*m_pSnHeadData)->SVmzsw == 1 ){//Œo‰ß‘[’u‚ª‚ ‚éê‡
			m_pArith->l_add( kingaku , PrintData->Sn_1F4A , PrintData->Sn_1F4B );
		}
		m_pArith->l_add( kingaku , kingaku , PrintData->Sn_1F4C );
	}
	memmove( ANS[3].val , kingaku , MONY_BUF_SIZE);
	m_pArith->l_add( WORK7 , WORK7 ,kingaku );
	//‡D
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	if( (*m_pSnHeadData)->SVmzsw == 1 ){//Œo‰ß‘[’u‚ª‚ ‚éê‡
		m_pArith->l_add( kingaku , PrintData->Sn_1F5A , PrintData->Sn_1F5B );
	}
	m_pArith->l_add( kingaku , kingaku , PrintData->Sn_1F5C );
	memmove( ANS[4].val , kingaku , MONY_BUF_SIZE);
	m_pArith->l_add( WORK7 , WORK7 ,kingaku );
	//‡E
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	if( (*m_pSnHeadData)->SVmzsw == 1 ){//Œo‰ß‘[’u‚ª‚ ‚éê‡
		m_pArith->l_add( kingaku , PrintData->Sn_1F6A , PrintData->Sn_1F6B );
	}
	m_pArith->l_add( kingaku , kingaku , PrintData->Sn_1F6C );
	memmove( ANS[5].val , kingaku , MONY_BUF_SIZE);
	m_pArith->l_add( WORK7 , WORK7 ,kingaku );
	//‡F
	memmove( ANS[6].val , WORK7 , MONY_BUF_SIZE);
	//‡G‡H 7-2-3
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	m_pArith->l_sub( kingaku , WORK7 , WORK2 );
	m_pArith->l_sub( kingaku , kingaku , WORK3 );
	m_pArith->l_add( WORK8 , WORK8 , kingaku );
	
	if( m_pArith->l_test( kingaku )  < 0 ){//ƒ}ƒCƒiƒX‚Ìê‡
		m_pArith->l_neg( kingaku );
		memmove( ANS[8].val , kingaku , MONY_BUF_SIZE);//‡H
		m_pArith->l_div( ANS[8].val , ANS[8].val , PW1 );
		m_pArith->l_mul( ANS[8].val , ANS[8].val , PW1 );
	}
	else{//ƒvƒ‰ƒX‚Ìê‡
		if( (*m_pSnHeadData)->Sn_SKKBN%2 ){//’†ŠÔ‚Ìê‡AŠÒ•tŽž‚Í•\Ž¦‚µ‚È‚¢
			memmove( ANS[7].val , kingaku , MONY_BUF_SIZE);//‡G
		}
		else{
			memset( kingaku , '\0' , MONY_BUF_SIZE );
		}
	}

	//‡I
	memmove( ANS[9].val, (*m_pSnHonpyoData)->Sn_TYNOFZ, MONY_BUF_SIZE );

	//‡J 9-10
	m_pArith->l_sub( kingaku , ANS[8].val , (*m_pSnHonpyoData)->Sn_TYNOFZ);

	if( m_pArith->l_test( kingaku )  < 0 ){//ƒ}ƒCƒiƒX‚Ìê‡
		m_pArith->l_neg( kingaku );
		memmove( ANS[11].val , kingaku , MONY_BUF_SIZE);//‡J
	}
	else{//ƒvƒ‰ƒX‚Ìê‡
		memmove( ANS[10].val , kingaku , MONY_BUF_SIZE);//‡K
	}
	//‡L
	if( (*m_pSnHeadData)->Sn_SKKBN == 3 || (*m_pSnHeadData)->Sn_SKKBN == 4 ){//C³‚Ìê‡‚Ì‚Ý
		memmove( ANS[12].val, (*m_pSnHonpyoData)->Sn_KAKTIZ, MONY_BUF_SIZE );
		//‡M·ˆø”[•tÅŠz
		if( m_pArith->l_test(ANS[10].val) > 0 ){
			m_pArith->l_sub( ANS[13].val , ANS[10].val , (*m_pSnHonpyoData)->Sn_KAKTIZ );
		}
		else{
			m_pArith->l_sub( ANS[13].val , ANS[13].val, ANS[11].val );
			m_pArith->l_sub( ANS[13].val , ANS[13].val, (*m_pSnHonpyoData)->Sn_KAKTIZ );
		}
		m_pArith->l_sub( ANS[13].val , ANS[13].val, ANS[7].val );
	}
	//‡N‰ÛÅŽ‘ŽY‚Ì÷“n“™‚Ì‘Î‰¿‚ÌŠz
	if( !((*m_pSnHeadData)->m_s_sgn2&0x0f) && (m_pArith->l_test((*m_pSnHonpyoData)->Sn_KZURI)<0) ){
		memset( ANS[14].val, '\0', MONY_BUF_SIZE );
	}
	else{
		memmove( ANS[14].val, (*m_pSnHonpyoData)->Sn_KZURI, MONY_BUF_SIZE );
	}
	if( (*m_pSnHeadData)->m_s_sgn2&0x0f){
		//‡OŠî€ŠúŠÔ‚Ì‰ÛÅ”„ã‚
		memmove( ANS[15].val , (*m_pSnHonpyoData)->Sn_ZZURI , MONY_BUF_SIZE );
	}
	else{
		//‡OŽ‘ŽY‚Ì÷“n“™‚Ì‘Î‰¿‚ÌŠz
		if( m_pArith->l_test((*m_pSnHonpyoData)->Sn_SOURI) < 0 ){
			memset( ANS[15].val, '\0', MONY_BUF_SIZE );
		}
		else{
			memmove( ANS[15].val , (*m_pSnHonpyoData)->Sn_SOURI , MONY_BUF_SIZE );
		}
	}

	//‡PTœ•s‘«ŠÒ•tÅŠz
	//‡Q·ˆøÅŠz
	if( (*m_pSnHeadData)->SVmzsw == 1 ){//Œo‰ß‘[’u‚ª‚ ‚éê‡
		char	WORK0[MONY_BUF_SIZE]={0};
		char	WORK1[MONY_BUF_SIZE]={0};
		char	WORK2[MONY_BUF_SIZE]={0};
		m_pArith->l_add( WORK0 , PrintData->Sn_1F8B , PrintData->Sn_1F8C );//8
		m_pArith->l_add( WORK1 , PrintData->Sn_1F9B , PrintData->Sn_1F9C );//9
		m_pArith->l_sub( WORK2 , WORK1 , WORK0 );//9-8		if( m_pArith->l_test( kingaku ) < 0 ){//ƒ}ƒCƒiƒX‚Ìê‡‡P‚É
		if( m_pArith->l_test( WORK2 ) < 0 ){
			if( (*m_pSnHeadData)->Sn_SKKBN%2 ){//’†ŠÔ‚Ìê‡AŠÒ•tŽž‚Í•\Ž¦‚µ‚È‚¢
				m_pArith->l_neg( WORK2 );
				memmove(  ANS[16].val , WORK2 , MONY_BUF_SIZE );//‡Q
			}
		}
		else{//ƒvƒ‰ƒX‚Ìê‡‡Q‚É
			memmove(  ANS[17].val , WORK2 , MONY_BUF_SIZE );//‡P
		}
	}
	else{
		memmove( kingaku , WORK8 , MONY_BUF_SIZE);
		if( m_pArith->l_test( kingaku ) < 0 ){//ƒ}ƒCƒiƒX‚Ìê‡‡P‚É
			m_pArith->l_neg( kingaku );
			memmove(  ANS[17].val , kingaku , MONY_BUF_SIZE );//‡Q
			m_pArith->l_div( ANS[17].val , ANS[17].val , PW1 );
			m_pArith->l_mul( ANS[17].val , ANS[17].val , PW1 );
		}
		else{//ƒvƒ‰ƒX‚Ìê‡‡Q‚É
			if( (*m_pSnHeadData)->Sn_SKKBN != 2  ){
				if( (*m_pSnHeadData)->Sn_SKKBN%2 ){//’†ŠÔ‚Ìê‡AŠÒ•tŽž‚Í•\Ž¦‚µ‚È‚¢
					memmove(  ANS[16].val , kingaku , MONY_BUF_SIZE );//‡P
				}
			}
		}
	}

	//‡R÷“nŠ„ŠzŠÒ•tŠz
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	memset( kingaku2 , '\0' , MONY_BUF_SIZE);
		
	if( (*m_pSnHeadData)->SVmzsw == 1 ){//Œo‰ß‘[’u‚ª‚ ‚éê‡
		m_Util.percent( kingaku,(char*)PrintData->Sn_1F8B, 7, 25, 0 );
		m_pArith->l_add(kingaku2 , kingaku2 , kingaku);
	}
	m_Util.percent( kingaku,(char*)PrintData->Sn_1F8C, 8, 17, 0 );
	m_pArith->l_add(kingaku2, kingaku2 , kingaku);

	//20÷“nŠ„Šz”[ÅŠz
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	memset( kingaku3 , '\0' , MONY_BUF_SIZE);
	if( (*m_pSnHeadData)->SVmzsw == 1 ){//Œo‰ß‘[’u‚ª‚ ‚éê‡
		m_Util.percent( kingaku, (char*)PrintData->Sn_1F9B, 7, 25, 0 );
		m_pArith->l_add(kingaku3 , kingaku3 , kingaku);
		m_Util.percent( kingaku, (char*)PrintData->Sn_1F9C, 8, 17, 0 );
		m_pArith->l_add(kingaku3 , kingaku3 , kingaku);
		m_pArith->l_sub(kingaku3 , kingaku3 , kingaku2);
		if( m_pArith->l_test( kingaku3 ) < 0 ){//ƒ}ƒCƒiƒX‚Ìê‡‡R‚É
			if( ((*m_pSnHeadData)->Sn_SKKBN%2)  ){
				memmove(  ANS[18].val , kingaku3 , MONY_BUF_SIZE );
				m_pArith->l_neg(  ANS[18].val );
			}
		}
		else{
			memmove(  ANS[19].val , kingaku3 , MONY_BUF_SIZE );
			m_pArith->l_div( ANS[19].val , ANS[19].val , PW1 );
			m_pArith->l_mul( ANS[19].val , ANS[19].val , PW1 );
		}
	}
	else{
		if( m_pArith->l_test( ANS[17].val ) ){
			m_Util.percent( kingaku,(char*)ANS[17].val, 8, 17, 0 );
			m_pArith->l_add(kingaku3 , kingaku3 , kingaku);
			m_pArith->l_div( ANS[19].val , kingaku3 , PW1 );
			m_pArith->l_mul( ANS[19].val , ANS[19].val , PW1 );
		}
		else{
			m_Util.percent( kingaku,(char*)ANS[16].val, 8, 17, 0 );
			m_pArith->l_add(kingaku3 , kingaku3 , kingaku);
			memmove(  ANS[18].val , kingaku3 , MONY_BUF_SIZE );
		}
	}

	//21’†ŠÔ”[•t÷“nŠ„Šz
	memmove( ANS[20].val , (*m_pSnHonpyoData)->Sn_TTYWAR , MONY_BUF_SIZE);
	//22
	m_pArith->l_sub( kingaku , ANS[19].val, ANS[20].val );
	if( m_pArith->l_test( kingaku ) < 0 ){//ƒ}ƒCƒiƒX‚Ìê‡‡R‚É
		memmove(  ANS[22].val , kingaku , MONY_BUF_SIZE );
		m_pArith->l_neg(  ANS[22].val );
		m_pArith->l_div( ANS[22].val , ANS[22].val , PW1 );
		m_pArith->l_mul( ANS[22].val , ANS[22].val , PW1 );
	}
	else{
		memmove(  ANS[21].val , kingaku , MONY_BUF_SIZE );
	}
	if(  (*m_pSnHeadData)->Sn_SKKBN == 3 ||  (*m_pSnHeadData)->Sn_SKKBN == 4 ){
		//24
		memmove( ANS[23].val , (*m_pSnHonpyoData)->Sn_TKAKTZ , MONY_BUF_SIZE);
		//25
		//·ˆø”[•tÅŠz
		if( m_pArith->l_test(ANS[21].val) > 0 ){
			m_pArith->l_sub( ANS[24].val , ANS[21].val , (*m_pSnHonpyoData)->Sn_TKAKTZ );
		}
		else{
			char	tmpMony[MONY_BUF_SIZE]={0};
			m_pArith->l_add( tmpMony, ANS[18].val, ANS[22].val );
			if( m_pArith->l_test(tmpMony) ){
				m_pArith->l_neg( tmpMony );
			}
			m_pArith->l_sub( ANS[24].val , tmpMony, (*m_pSnHonpyoData)->Sn_TKAKTZ );
		}
		m_pArith->l_div( ANS[24].val , ANS[24].val , PW1 );
		m_pArith->l_mul( ANS[24].val , ANS[24].val , PW1 );

	}
	
//JANS
	if( (*m_pSnHeadData)->SVmzsw == 1 ){//Œo‰ß‘[’u‚ª‚ ‚éê‡
		memmove( JANS[0].val ,  PrintData->Sn_1F1A , MONY_BUF_SIZE);
		memmove( JANS[1].val ,  PrintData->Sn_1F1B , MONY_BUF_SIZE);
	}

	memmove( JANS[2].val ,  PrintData->Sn_1F1C , MONY_BUF_SIZE);

	if( (*m_pSnHeadData)->SVmzsw == 1 ){//Œo‰ß‘[’u‚ª‚ ‚éê‡
		memmove( JANS[3].val ,  PrintData->Sn_1F2A , MONY_BUF_SIZE);
		memmove( JANS[4].val ,  PrintData->Sn_1F2B , MONY_BUF_SIZE);
	}
	memmove( JANS[5].val ,  PrintData->Sn_1F2C , MONY_BUF_SIZE);

	if( (*m_pSnHeadData)->SVmzsw == 1 ){// Œo‰ß‘[’u‘ÎÛ‚Ì‰ÛÅŽ‘ŽY‚Ì÷“n“™‚ ‚è
		m_pArith->l_sub( kingaku , PrintData->Sn_1F9B , PrintData->Sn_1F8B );
		if( m_pArith->l_test( ANS[16].val ) > 0){//ŠÒ•t‚Ì‚Æ‚«
			if( !((*m_pSnHeadData)->Sn_SKKBN%2) ){
				memset( kingaku , '\0' , MONY_BUF_SIZE);
			}
		}
		if( !m_pArith->l_test( ANS[16].val ) && ( !m_pArith->l_test( ANS[17].val ) || ( m_pArith->l_cmp( ANS[17].val , val100 ) < 0 ))){
			memset( kingaku , '\0' , MONY_BUF_SIZE);
		}
		memmove( JANS[6].val ,  kingaku , MONY_BUF_SIZE);
		// ’n•ûÁ”ïÅ‚Ì‰ÛÅ•W€‚Æ‚È‚éÁ”ïÅŠz6.3%
		m_pArith->l_sub( kingaku , PrintData->Sn_1F9C , PrintData->Sn_1F8C );
		if( m_pArith->l_test( ANS[16].val ) > 0){//ŠÒ•t‚Ì‚Æ‚«
			if( !((*m_pSnHeadData)->Sn_SKKBN%2) ){
				memset( kingaku , '\0' , MONY_BUF_SIZE);
			}
		}
		if( !m_pArith->l_test( ANS[16].val ) && ( !m_pArith->l_test( ANS[17].val ) || ( m_pArith->l_cmp( ANS[17].val , val100 ) < 0 ))){
			memset( kingaku , '\0' , MONY_BUF_SIZE);
		}
		memmove( JANS[7].val ,  kingaku , MONY_BUF_SIZE);
	}
	else{
		if( m_pArith->l_test (ANS[16].val) ){
			memmove( JANS[7].val ,  ANS[16].val , MONY_BUF_SIZE);
			m_pArith->l_neg ( JANS[7].val );
		}
		else{
			memmove( JANS[7].val ,  ANS[17].val , MONY_BUF_SIZE);
		}
	}

	memset( WORK0, '\0', sizeof(WORK0) );
	i = j = 0;
	m_pArith->l_div( WORK0, ANS[j].val, PW0 );
	m_pArith->l_mul( ANS[j].val , WORK0 , PW1);


	// ‹àŠz¢9£->¢12£
	for( ++i, j=0; j<4; ++j){
		m_pArith->l_div( WORK0, ANS[j+8].val, PW1 );
		m_pArith->l_mul( ANS[j+8].val , WORK0 , PW1);
	}
	
	// ‹àŠz¢13£->¢16£
	for( ++i, j=0; j<4; ++j ){
		switch( j ){
			case 1 :
				m_pArith->l_div( WORK0, ANS[j+12].val, PW1 );
				m_pArith->l_mul( ANS[j+12].val , WORK0 , PW1);
				break;
			default:
				break;
		}
	}
	
	// ‹àŠz¢17£->¢20£
	for( ++i, j=0; j<4; ++j ){
		switch( j ){
			case 0 :
			case 2 :
				break;
			default:	
				m_pArith->l_div( WORK0, ANS[j+16].val, PW1 );
				m_pArith->l_mul( ANS[j+16].val , WORK0 , PW1);
				break;
		}
	}
	
	// ‹àŠz¢21£->¢25£
	for( ++i, j=0; j<5; ++j ){
		switch( j ){
			case 3 :
				break;
			default:
				m_pArith->l_div( WORK0, ANS[j+20].val, PW1 );
				m_pArith->l_mul( ANS[j+20].val , WORK0 , PW1);
				break;
		}
	}
	
	//26
	// Á”ïÅ‹y‚Ñ’n•ûÁ”ïÅ‚Ì‡Œvi”[•t–”‚ÍŠÒ•tjÅŠz
	switch( (*m_pSnHeadData)->Sn_SKKBN&0xff ) {
		case 1 : case 2 :		// Šm’èE’†ŠÔ\
			m_pArith->l_add( kingaku, ANS[10].val, ANS[21].val );
			m_pArith->l_add( kingaku2, ANS[7].val,  ANS[11].val );
			m_pArith->l_add( kingaku2 , kingaku2, ANS[18].val );
			m_pArith->l_add( kingaku2 , kingaku2, ANS[22].val );
			m_pArith->l_sub( ANS[25].val, kingaku, kingaku2 );
			break;
		default	:				// C³\
			m_pArith->l_add( ANS[25].val, ANS[13].val, ANS[24].val );
			break;
	}
	
	memmove( XM8, ANS[7].val, sizeof(XM8) );
	memmove( XM11, ANS[10].val, sizeof(XM11) );
	memmove( XM12, ANS[11].val, sizeof(XM12) );
	memmove( XM13, ANS[12].val, sizeof(XM13) );
	memmove( XM19, ANS[18].val, sizeof(XM19) );
	memmove( XM22, ANS[21].val, sizeof(XM22) );
	memmove( XM23, ANS[22].val, sizeof(XM23) );
	memmove( XM24, ANS[23].val, sizeof(XM24) );
	memmove( XM26, ANS[25].val, sizeof(XM26) );


	char	tmp_arith1[6] = {0},tmp_arith2[6] = {0};

	memmove( tmp_arith1, XM11, sizeof(XM11));
	m_pArith->l_sub( tmp_arith1, tmp_arith1, XM8 );
	m_pArith->l_sub( tmp_arith1, tmp_arith1, XM12 );
	m_pArith->l_sub( tmp_arith1, tmp_arith1, XM13 );

	memmove( tmp_arith2, XM22, sizeof(XM22));
	m_pArith->l_sub( tmp_arith2, tmp_arith2, XM19 );
	m_pArith->l_sub( tmp_arith2, tmp_arith2, XM23 );
	m_pArith->l_sub( tmp_arith2, tmp_arith2, XM24 );


	memset( F61, '\0', sizeof(F61));
	memset( F62, '\0', sizeof(F62));
	memset( F63, '\0', sizeof(F63));
	memset( F64, '\0', sizeof(F64));
	memset( F65, '\0', sizeof(F65));
	memset( F66, '\0', sizeof(F66));

	if( m_pArith->l_test(XM26) > 0 ){
		memmove( F61, XM26, sizeof(F61));
		memmove( F62, tmp_arith1, sizeof(F62));
		memmove( F63, tmp_arith2, sizeof(F63));
	}else if( m_pArith->l_test(XM26) < 0 ){
		memmove( F64, XM26, sizeof(F64));
		memmove( F65, tmp_arith1, sizeof(F65));
		memmove( F66, tmp_arith2, sizeof(F66));

		if( m_pArith->l_test(F64) < 0 ){
			m_pArith->l_neg(F64);
		}

		if( m_pArith->l_test( ANS[10].val ) != 0 ){
			if( m_pArith->l_test(F65) > 0 ){
				m_pArith->l_neg(F65);
			}
		}else{
			if( m_pArith->l_test(F65) < 0 ){
				m_pArith->l_neg(F65);
			}
		}

		if( m_pArith->l_test( ANS[21].val ) != 0 ){
			if( m_pArith->l_test(F66) > 0 ){
				m_pArith->l_neg(F66);
			}
		}else{
			if( m_pArith->l_test(F66) < 0 ){
				m_pArith->l_neg(F66);
			}
		}
	}

	return;
}

void CShinFhyo60::f6calq_9to14( int index )
{
	if( index == -1 ){
		return;
	}

	char bunshi[6]={0},bunbo[6]={0};
	char zei1[6]={0},chihozei1[6]={0},kei1[6]={0},
		 zei2[6]={0},chihozei2[6]={0},kei2[6]={0};
	char	PW1[6];
	char	tmp_char1[30]={0},tmp_char2[30]={0};

	memset( PW1, '\0', 6 );
	m_pArith->l_input( PW1, _T("100") );

	strcpy_s( tmp_char1, sizeof(tmp_char1), sozoku[index].Sn_6FSBUNSHI );
	strcpy_s( tmp_char2, sizeof(tmp_char2), sozoku[index].Sn_6FSBUNBO );
	m_pArith->l_input( bunshi, tmp_char1 );
	m_pArith->l_input( bunbo , tmp_char2 );

	if( m_pArith->l_test( bunbo ) == 0 ){
		m_pArith->l_clear( F69[index] );
		m_pArith->l_clear( F610[index] );
		m_pArith->l_clear( F611[index] );

		return;
	}	

	if( m_pArith->l_test(F61) != 0 ){
		m_pArith->l_mul( zei1, F62,  bunshi );
		m_pArith->l_div( zei1, zei1, bunbo  );
		m_pArith->l_div( zei1, zei1, PW1	); //100‰~–¢–žØ‚èŽÌ‚Ä
		m_pArith->l_mul( zei1, zei1, PW1	);

		m_pArith->l_mul( chihozei1, F63,	   bunshi );
		m_pArith->l_div( chihozei1, chihozei1, bunbo  );
		m_pArith->l_div( chihozei1, chihozei1, PW1	  ); //100‰~–¢–žØ‚èŽÌ‚Ä
		m_pArith->l_mul( chihozei1, chihozei1, PW1	  );

		m_pArith->l_add( kei1, zei1, chihozei1 );
	}

	memmove( F69[index],  zei1,		 MONY_BUF_SIZE );
	memmove( F610[index], chihozei1, MONY_BUF_SIZE );
	memmove( F611[index], kei1,		 MONY_BUF_SIZE );
}