// H26HyoPrint.cpp : àÛç¸ïî é¿ëïÉtÉ@ÉCÉã
//

#include "stdafx.h"
#include "H26HyoView.h"
#include "PrnReportDlg.h"

#include "ShinFhyo52.h"
#include "ShinFhyo52EX.h"//[Y]
#include "voldatedb8.h"

#define SHIMIZU_TEST//ç≈èIìIÇ…è¡ÇµÇ‹Ç∑ÅB

extern	HINSTANCE g_hInstance;

const	int	KSK_BUF_SIZE	=	512*20;

//2018.03.13 INSERT START
const	int CULM_PER_PAGE	=	4;
//2018.03.13 INSERT END

//-----------------------------------------------------------------------------
// àÛç¸èÓïÒÇÃÉZÉbÉg
//-----------------------------------------------------------------------------
// à¯êî	pVprn			ÅF	èoóÕÉNÉâÉX
//		pPrtPar			ÅF	àÛç¸èÓïÒ
//		pPrintCmInfo	ÅF	àÛç¸ã§óLèÓïÒ
//-----------------------------------------------------------------------------
// ï‘ëóíl	0			ÅF	ê≥èÌèIóπ
//			-1			ÅF	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CH26HyoView::SetPrtPar( CVPrint *pVprn, NPRT_PAR *pPrtPar, SH_PRINT_CMINFO *pPrintCmInfo )
{
	// èÓïÒÉZÉbÉg
	ASSERT( pVprn );
	if( pVprn == NULL ){
		return -1;
	}
	m_pVprn = pVprn;

	ASSERT( pPrtPar );
	if( pPrtPar == NULL ){
		return -1;
	}
	m_pPset = pPrtPar;

	ASSERT( pPrintCmInfo );
	if( pPrintCmInfo == NULL ){
		return -1;
	}
	m_pPrintCmInfo = pPrintCmInfo;

//	memset( pPrtPar, '\0', sizeof(NPRT_PAR) );
//	pPrtPar->psPaperSize	= DMPAPER_A4;			// ópéÜÉTÉCÉY
//	pPrtPar->psOrientation	= DMORIENT_PORTRAIT;	// ópéÜï˚å¸
//	pPrtPar->psPrevFlg		= 0x0100;				// ÉvÉåÉrÉÖÅ[â¬
//	pPrtPar->psPageNum		= 0;					// ÉyÅ[ÉWêî
////	pPrtPar->psColorSet       = &m_Cset;

//	GetIcsofnDirectory( (LPTSTR)pPrtPar->psSigCopath );
//	char prnam[80] = {0};
//	memset( prnam, '\0', sizeof(prnam) );
//	if( PCOM_GetString( "SelectPrinter", prnam) != -1) {
//		strcpy_s( (char*)pPrtPar->psPrinterName, sizeof(pPrtPar->psPrinterName), prnam );
//	}
//	else{
//		pPrtPar->psPrinterName[0] = 0;
//	}

	return 0;
}

//-----------------------------------------------------------------------------
// àÛç¸
//-----------------------------------------------------------------------------
// ï‘ëóíl	0	ÅF	ê≥èÌèIóπ
//			-1	ÅF	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CH26HyoView::PrintSyzHyo()
{
	SF_PAR	PrtFont;
	memset( &PrtFont, '\0', sizeof(SF_PAR) );
	PrtFont.sf_ksiz = FNT200z;

	// àÛç¸ÉeÉXÉg
//	m_pVprn->DrawString( 0, 300, 1000, _T("Ç`ÇaÇb"), &PrtFont );

	return 0;
}

//-----------------------------------------------------------------------------
// àÛç¸éwíËÉ_ÉCÉAÉçÉOÇÃï\é¶
//-----------------------------------------------------------------------------
// ï‘ëóíl	0	ÅF	ê≥èÌèIóπ
//			-1	ÅF	ÉGÉâÅ[
//-----------------------------------------------------------------------------
int CH26HyoView::DispPrnDlg()
{
	HINSTANCE svhInstResource = AfxGetResourceHandle();
	AfxSetResourceHandle( g_hInstance );

	CPrnReportDlg	dlg;
	dlg.DoModal();

	AfxSetResourceHandle( svhInstResource );

	return 0;
}

#ifdef _20160711_EXTEND_
//-----------------------------------------------------------------------------
// ñ{ï\àÛç¸Å@ê¥êÖï“èW
//-----------------------------------------------------------------------------
// à¯êî	pDC		ÅF	ÉfÉoÉCÉXÉRÉìÉeÉLÉXÉg
//		isHikae	ÅF	çTÇ¶àÛç¸Ç©Ç«Ç§Ç©
//		fno		ÅF	
//-----------------------------------------------------------------------------
// ï‘ëóíl	
//-----------------------------------------------------------------------------
int CH26HyoView::PrintHonPyo( CDC *pDC, BOOL isHikae, int PPno , int fno )
{
	int				i, j, k, h;
	unsigned int	size;
//	char			*AP, *dp, buf[30], PW0[6], PW1[6];
	char			*AP, *dp, buf[512], PW0[6], PW1[6];
	unsigned short	SEno;
	// ê≈óùémñ@êl
	int				Zlng, Zsw;
//-- '15.02.27 --
//	char			Znam[50], Smei[50];
//---------------
	char			Znam[128]={0}, Smei[128]={0};
//---------------
	CString str = _T("");
	BOOL			isNoImage = FALSE;

//	if( m_pPrintCmInfo->isColorPrn && m_Vprn._IsColorMode() );	// ÉÅÉìÉoïœêî
//		PcSw = 1;
	SEno = fno;
/*
	if( m_pPrintCmInfo->PRtype&0x80 ){
		isNoImage = FALSE;
		if( isHikae ){
			if( !PcSw && (m_pSnHeadData->Sn_TYPE&0x04) ){
				// ÉJÉâÅ[
				if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
//					if( m_TaxReforms )	SEno = MAKEWORD( 4, 4 );		// ï¬Ç∂çûÇ‹Ç»Ç¢ÅEñ¢èoóÕ
//					else				SEno = MAKEWORD( 2, 2 );		// ï¬Ç∂çûÇ‹Ç»Ç¢ÅEñ¢èoóÕ
					SEno = MAKEWORD( 4, 4 );		// ï¬Ç∂çûÇ‹Ç»Ç¢ÅEñ¢èoóÕ
				}
				else{
					if( m_pSnHeadData->IsSoleProprietor() ){
						SEno = MAKEWORD( 2, 2 );		// ï¬Ç∂çûÇ‹Ç»Ç¢ÅEñ¢èoóÕ
					}
					else{
						SEno = MAKEWORD( 4, 4 );		// ï¬Ç∂çûÇ‹Ç»Ç¢ÅEñ¢èoóÕ
					}
				}
			}
			else{
				// ÉÇÉmÉNÉç
				if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
//					if( m_TaxReforms )	SEno = MAKEWORD( 2, 2 ); 		// â¸ê≥å„
//					else				SEno = MAKEWORD( 1, 1 );		// â¸ê≥ëO
					SEno = MAKEWORD( 2, 2 ); 		// â¸ê≥å„
				}
				else{
					if( m_pSnHeadData->IsSoleProprietor() ){
						SEno = MAKEWORD( 1, 1 ); 		// å¬êlçTóp
					}
					else{
						SEno = MAKEWORD( 1, 1 );		// ñ@êlçTóp
					}
				}
			}
		}
		else{
			// ÇXÇTÅìÉãÅ[Éãâ¸ê≥ëŒâûä˙ä‘ÉTÉCÉì
			if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
				if( !(m_pSnHeadData->Sn_TYPE&0x02) || PcSw ){							// ÉÇÉmÉNÉçíÒèoóp				
//					if( m_TaxReforms )	SEno = MAKEWORD( 2, 2 ); 		// â¸ê≥å„
//					else				SEno = MAKEWORD( 1, 1 );		// â¸ê≥ëO
					SEno = MAKEWORD( 2, 2 ); 		// â¸ê≥å„
				}
				else{
//					if( m_TaxReforms ) {
//						if( m_Kojin )		SEno = MAKEWORD( 4, 4 ); 	// ï¬Ç∂çûÇ‹Ç»Ç¢ÅEñ¢èoóÕ
//						else				SEno = MAKEWORD( 3, 3 );	// ï¬Ç∂çûÇ‹Ç»Ç¢ÅEèoóÕ
//					}
//					else	{
//						if( m_Kojin )		SEno = MAKEWORD( 2, 2 ); 	// ï¬Ç∂çûÇ‹Ç»Ç¢ÅEñ¢èoóÕ
//						else				SEno = MAKEWORD( 1, 1 );	// ï¬Ç∂çûÇ‹Ç»Ç¢ÅEèoóÕ
//					}
					if( m_pSnHeadData->IsSoleProprietor() ){
						SEno = MAKEWORD( 4, 4 ); 	// ï¬Ç∂çûÇ‹Ç»Ç¢ÅEñ¢èoóÕ
					}
					else{
						SEno = MAKEWORD( 3, 3 );	// ï¬Ç∂çûÇ‹Ç»Ç¢ÅEèoóÕ
					}
				}
			}
			else{
				if( !(m_pSnHeadData->Sn_TYPE&0x02) || PcSw ){
					SEno = MAKEWORD( 1, 1 );	// ÉÇÉmÉNÉçíÒèoóp
				}
				else{
					if( m_pSnHeadData->IsSoleProprietor() ){
						SEno = MAKEWORD( 4, 4 );		// ï¬Ç∂çûÇ‹Ç»Ç¢ÅEñ¢èoóÕ
					}
					else{
						SEno = MAKEWORD( 1, 1 );		// ï¬Ç∂çûÇ‹Ç»Ç¢ÅEèoóÕ
					}
				}
			}
		}
	}
	else{
		isNoImage = TRUE;
	}
*/
	if( m_pPrintCmInfo->PRtype&&0x80 ){
		isNoImage = FALSE;
	}
	else{
		isNoImage = TRUE;
	}

	size = 2048;			// 1024 Å® 2048
	if( (AP = new char[size]) == NULL ){
		return -1;
	}

	PBUF	PF1[30];
	int		PP = PPno;

	memset( PW0, '\0', 6 );
	m_Arith.l_input( PW0, _T("1000") );
	memset( PW1, '\0', 6 );
	m_Arith.l_input( PW1, _T("100") );
/*
	if( isHikae ){
		if( (m_pSnHeadData->Sn_TYPE&0x04) && PcSw ){
			PP = 1;			// ÉJÉâÅ[çTóp
		}
		else{
			PP = 3;			// ÉÇÉmÉNÉççTóp
		}
	}
	else{
		if( m_pSnHeadData->Sn_TYPE&0x01 ){
			if( (m_pSnHeadData->Sn_TYPE&0x02) && PcSw ){
				PP = 1;			// ÉJÉâÅ[íÒèoóp
			}
			else{
				PP = 2;			// ÉÇÉmÉNÉçíÒèoóp
			}
		}
		else{
			PP = 1;			// ÇnÇbÇqàÛç¸
		}
	}
*/
	dp = AP;
	memset( dp, '\0', size );

	// íÒèoîNåéì˙
	char	yymmdd[3] = {0};
	m_pSnHeadData->GetYmdDataGen( ID_ICSSH_PRESENT_DATE, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
	if( (yymmdd[0]&0xff) || (yymmdd[1]&0xff) || (yymmdd[2]&0xff) ){
		for( i=j=0; j<3; ++j, dp += 3 ){
			if( yymmdd[j]&0xff ){
				wsprintf( dp, _T("%2X"), yymmdd[j]&0xff );
			}
			else{
				strcpy_s( dp, 2+1, "  " );						//?
			}
			PF1[i].PITM[j] = dp;
		}
	}
	else{
		pbclr( 0, 3, &PF1[(i = 0)] );
	}

	// ê≈ñ±èêí∑ñº
	i++;
	memmove( dp, m_pSnHeadData->Sn_ZMSYO, 6*2 );
	PF1[i++].PITM[0] = dp;
	dp += 13;

	// î[ê≈ín
	if( !m_pPrintCmInfo->OP3 ){	
		// óXï÷î‘çÜ
		if( j = (int)strlen( (char *)m_pSnHeadData->Sn_ZIP ) ){	
			strcpy_s( dp, 2+1, "Åß" );
			memmove( (dp+2), m_pSnHeadData->Sn_ZIP, j );
			PF1[i++].PITM[0] = dp;
			dp += (j + 3);
		}
		else{
			PF1[i++].PITM[0] = (char *)0;
		}

		// î[ê≈ínÅiÇQçsÅj
		memmove( dp, &m_pSnHeadData->Sn_NOZEI[0],  20*2 );				
		PF1[i].PITM[0] = dp;
		dp += 41;
		memmove( dp, &m_pSnHeadData->Sn_NOZEI[40], 20*2 );
		PF1[i++].PITM[1] = dp;
		dp += 41;
	}
	else{
		pbclr( 0, 1, &PF1[i++] );
		pbclr( 0, 2, &PF1[i++] );
	}

	// î[ê≈ínìdòbî‘çÜ
	if( !m_pPrintCmInfo->OPE ){
		memset( buf, '\0', sizeof( buf ) );
		// ìdòbî‘çÜ
		memmove( buf, m_pSnHeadData->Sn_TELNO1, 6 );
		str.Format( _T("%s") , buf );
		str.TrimRight();
		memset( buf, '\0', sizeof( buf ) );
		memmove( buf , str , str.GetLength());
		j = (int)strlen( buf );
		memmove( dp, "      ", 6 );
		memmove( (dp+(6-j)), buf, 6 );
		PF1[i].PITM[0] = dp;
		dp += 7;

		memset( buf, '\0', sizeof( buf ) );
		memmove( buf, m_pSnHeadData->Sn_TELNO2, 4 );
		str.Format( _T("%s") , buf );
		str.TrimRight();
		memset( buf, '\0', sizeof( buf ) );
		memmove( buf , str , str.GetLength());
		memmove( dp , buf, sizeof( buf ) );

		PF1[i].PITM[1] = dp;	dp += 5;

		memset( buf, '\0', sizeof( buf ) );
		memmove( buf, m_pSnHeadData->Sn_TELNO3, 4 );
		str.Format( _T("%s") , buf );
		str.TrimRight();
		memset( buf, '\0', sizeof( buf ) );
		memmove( buf , str , str.GetLength());
		memmove( dp , buf, sizeof( buf ) );
		PF1[i++].PITM[2] = dp;
		dp += 5;
	}
	else{
		pbclr( 0, 3, &PF1[i++] );
	}

	// éÅñºñîÇÕâÆçÜ	
	if( !m_pPrintCmInfo->OPF ){	
		// ñ@êlñºÅiêUÇËÇ™Ç»Åj
		memmove( dp, m_pSnHeadData->Sn_COFRI, 68 );	
		PF1[i++].PITM[0] = dp;
		dp += 69;
	}
	else{
		pbclr( 0, 1, &PF1[i++] );
	}

	// ñ@êlñº
	if( !m_pPrintCmInfo->OP4 && !m_pPrintCmInfo->OPM ){
		memmove( dp, m_pSnHeadData->Sn_CONAM, 20*2 );
		PF1[i++].PITM[0] = dp;
		dp += 41;
	}
	else{
		pbclr( 0, 1, &PF1[i++] );
	}

	// ë„ï\é“ÅiêUÇËÇ™Ç»Åj
	if( !m_pPrintCmInfo->OPG ){
		memmove( dp, m_pSnHeadData->Sn_DIFRI, 68 );
		PF1[i++].PITM[0] = dp;	
		dp += 69;
	}
	else{
		pbclr( 0, 1, &PF1[i++] );
	}

	// ë„ï\é“ñº
	if( !m_pPrintCmInfo->OP5 ){
		memmove( dp, m_pSnHeadData->Sn_DIHYO, 20*2 );
		PF1[i++].PITM[0] = dp;
		dp += 41;

	}
	else{
		pbclr( 0, 1, &PF1[i++] );
	}

//2015.09.16 UPDATE START
	//// åoóùíSìñé“ñº
	//if( !m_pPrintCmInfo->OP6 ){
	//	memmove( dp, m_pSnHeadData->Sn_KEIRI, 20*2 );					
	//	PF1[i++].PITM[0] = dp;
	//	dp += 41;
	//}
	//else{
	//	pbclr( 0, 1, &PF1[i++] );
	//}

//2016.06.22 UPDATE START
//	if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){

	BOOL mynum_form = FALSE;
	if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
			mynum_form = TRUE;
		}
	}else{
		if( m_pSnHeadData->Sn_KDAYE >=  20160401 ){
			mynum_form = TRUE;
		}
	}

	if( mynum_form == TRUE ){
//2016.06.22 UPDATE END
		//memmove( dp, m_pSnHeadData->Sn_KEIRI, 26 );					
		//PF1[i++].PITM[0] = dp;
		//dp += 27;

//2015.10.20 UPDATE START
//		pbclr( 0, 1, &PF1[i++] ); //ébÇ≠ÇÕèoóÕïsâ¬

		//å¬êlî‘çÜÅEñ@êlî‘çÜ
//2015.12.03 UPDATE START
		//CString myno;
		//if( m_pSnHeadData->GetMyNumber( myno ) == 0 ){
		//	if( m_pSnHeadData->IsSoleProprietor() == TRUE ){
		//		myno = ' ' + myno;
		//	}
		//	memmove( dp, myno, 13 );
		//	PF1[i++].PITM[0] = dp;
		//	dp += 14;
		//}else{
		//	pbclr( 0, 1, &PF1[i++] );
		//}

		if( m_ShRevType >= ID_ICSSH_MYNUMBER_REVTYPE ){
			CICSMNSub8 mnsub;

			if(( isHikae == TRUE )&&( m_pSnHeadData->IsSoleProprietor() )){
				pbclr( 0, 1, &PF1[i++] );
			}else{
				CString myno_str,myno_prn;
				if( m_pSnHeadData->GetMyNumber( myno_str ) == 0 ){
					if( m_pPset->psSigCoprn&0x4000){
						myno_prn = mnsub.MakePrintKojinNo( myno_str,  m_pVprn->Get_KojinPrintFlg() );
					}else{
						myno_prn = myno_str;
					}
					if( m_pSnHeadData->IsSoleProprietor() == TRUE ){
						myno_prn = ' ' + myno_prn;
					}
					memmove( dp, myno_prn, 13 );
					PF1[i++].PITM[0] = dp;
					dp += 14;
				}else{
					pbclr( 0, 1, &PF1[i++] );
				}
			}
		}else{
			pbclr( 0, 1, &PF1[i++] );
		}
//2015.12.03 UPDATE END

	}else{
		// åoóùíSìñé“ñº
		if( !m_pPrintCmInfo->OP6 ){
			memmove( dp, m_pSnHeadData->Sn_KEIRI, 20*2 );					
			PF1[i++].PITM[0] = dp;
			dp += 41;
		}
		else{
			pbclr( 0, 1, &PF1[i++] );
		}
	}

//2015.09.16 UPDATE END

	// àÍòAî‘çÜ
	if( ((m_pSnHeadData->Sn_ITIREN[0]&0xff)!=0xff ) && (m_pSnHeadData->IsSoleProprietor()==FALSE) ){

		memset( buf, '\0', sizeof( buf ) );
		m_Arith.l_unpac( buf, m_pSnHeadData->Sn_ITIREN, 8 );
		for( j=(int)strlen(buf); j>0; ){
			if( buf[--j] != 0x20 ){
				break;
			}
		}
		++j;
		if( j < 8 ){
			memmove( dp, "        ", 8-j );
		}
		memmove( (dp+(8-j)), buf, j );					
		PF1[i++].PITM[0] = dp;
		dp += 9;
	}
	else{
		PF1[i++].PITM[0] = (char *)0;
	}

int CCSw = 0;

	// êÆóùî‘çÜìôèoóÕÉTÉCÉì
	if( (m_pPrintCmInfo->PRtype&0x80) || !m_pPrintCmInfo->OP1 ){
		if( (m_pSnHeadData->Sn_SEIRI[0]&0xff) != 0xff ){
			memset( buf, '\0', sizeof( buf ) );
			m_Arith.l_unpac( buf, m_pSnHeadData->Sn_SEIRI, 8 );
			for( j=(int)strlen(buf); j>0; ){
				if( buf[--j] != 0x20 ){
					break;
				}
			}
			++j;
			if( j < 8 ){
				memmove( dp, "        ", 8-j );
			}
			// êÆóùî‘çÜ
			memmove( (dp+(8-j)), buf, j );				
			PF1[i++].PITM[0] = dp;
			dp += 9;
		}
		else{
			PF1[i++].PITM[0] = (char *)0;
		}

		if( CCSw ){	// ã≠êßä˙ä‘ïœçX
			for( j=0; j<3; ++j, dp+=3 ){				// ê\çêä˙ä‘é©
//				wsprintf( dp, "%02X", CC_KDAYS[j]&0xff );
				PF1[i].PITM[j] = dp;
			}
			for( ; j<6; ++j, dp += 3 ){					// ê\çêä˙ä‘éä
//				wsprintf( dp, "%02X", CC_KDAYE[j-3]&0xff );
				PF1[i].PITM[j] = dp;
			}
		}
		else{
			// ê\çêä˙ä‘é©
			memset( yymmdd, '\0', sizeof(yymmdd) );
			m_pSnHeadData->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
			for( j = 0; j < 3; ++j, dp+=3 ){	
				wsprintf( dp, _T("%02X"), yymmdd[j]&0xff );
				PF1[i].PITM[j] = dp;
			}
			// ê\çêä˙ä‘éä
			memset( yymmdd, '\0', sizeof(yymmdd) );
			m_pSnHeadData->GetYmdDataGen( ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
			for( ; j<6; ++j, dp+=3 ){
				wsprintf( dp, _T("%02X"), yymmdd[j-3]&0xff );
				PF1[i].PITM[j] = dp;
			}
		}
	}
	else{
		pbclr( 0, 1, &PF1[i++] );
		pbclr( 0, 6, &PF1[i] );
	}

	switch( m_pSnHeadData->Sn_SKKBN ){
		case 1 :
			if( !(m_pPrintCmInfo->PRtype&0x80) ){
				// ÇnÇbÇqópéÜ
				if( m_pPrintCmInfo->OPC ){
					PF1[++i].PITM[0] =  (char *)0;	
				}
				else{
					PF1[++i].PITM[0] = _T("Å@ämíËÅ@");
				}
			}
			else{
				PF1[++i].PITM[0] = _T("Å@ämíËÅ@");
			}
			break;
		case 2 :
			PF1[++i].PITM[0] = _T("Å@íÜä‘Å@");
			break;
		case 3 :
			PF1[++i].PITM[0] = _T("èCê≥ämíË");
			break;
		case 4 :
			PF1[++i].PITM[0] = _T("èCê≥íÜä‘");
			break;
	}

	// êÆóùî‘çÜìôèoóÕÉTÉCÉì
	if( (m_pPrintCmInfo->PRtype&0x80) || !m_pPrintCmInfo->OP1 ){						
		//íÜä‘ê\çêä˙ä‘é©•éä
		switch( m_pSnHeadData->Sn_SKKBN ){
			case 1 : 
			case 3 :
				pbclr( 0, 6, &PF1[++i] );
				break;
			default:
				if( CCSw ){	// ã≠êßä˙ä‘ïœçXÅ@02'01.21
					for( ++i, j=0; j<3; ++j, dp+=3 ){
//						wsprintf( dp, "%02X", CC_MDAYS[j]&0xff );
						PF1[i].PITM[j] = dp;
					}
					for( ; j<6; ++j, dp+=3 ){
//						wsprintf( dp, "%02X", CC_MDAYE[j-3]&0xff );
						PF1[i].PITM[j] = dp;
					}
				}
				else{
					memset( yymmdd, '\0', sizeof(yymmdd) );
					m_pSnHeadData->GetYmdDataGen( ID_ICSSH_CHUUKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
					for( ++i, j=0; j<3; ++j, dp += 3 ){
						wsprintf( dp, _T("%02X"), yymmdd[j]&0xff );
						PF1[i].PITM[j] = dp;
					}
					memset( yymmdd, '\0', sizeof(yymmdd) );
					m_pSnHeadData->GetYmdDataGen( ID_ICSSH_CHUUKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2] );
					for( ; j<6; ++j, dp += 3 ){
						wsprintf( dp, _T("%02X"), yymmdd[j-3]&0xff );
						PF1[i].PITM[j] = dp;
					}
				}
				break;
		}
	}
	else{
		pbclr( 0, 6, &PF1[++i] );
	}

	if( j = _xprn( PP, 1, PF1, 0, pDC, isNoImage, SEno ) ){
		goto POUTn;
	}

	char	WORK0[MONY_BUF_SIZE] = {0};
	char	WORK_ALL[MONY_BUF_SIZE] = {0};
	// åvéZåãâ ÉoÉbÉtÉ@
	struct _ANS {
		unsigned char	val[MONY_BUF_SIZE];
	};
	struct _ANS		ANS[30], JANS[10];
	::ZeroMemory( &ANS , sizeof( ANS ) );
	::ZeroMemory( &JANS , sizeof( JANS ) );

	//Ç±Ç±Ç…Ç´Ç¡Ç∆åvéZÇ™ì¸ÇÈÇ∆évÇÌÇÍ
////////////////////////////////////////////////////////////////////////////
	char kingaku[MONY_BUF_SIZE] = {0};
	char kingaku2[MONY_BUF_SIZE] = {0};
	char kingaku3[MONY_BUF_SIZE] = {0};
	char WORK2[MONY_BUF_SIZE] = {0};
	char WORK3[MONY_BUF_SIZE] = {0};
	char WORK7[MONY_BUF_SIZE] = {0};
	char WORK8[MONY_BUF_SIZE] = {0};
	char WORK9[MONY_BUF_SIZE] = {0};
	char val100[6] = {0};
	m_Arith.l_input( val100 , "100" );

	CH26SnFhyo10Data*	PrintData;

	if( m_pSnFhyo10Data ){
		//ïtï\1ÉfÅ[É^ì«çûÇ›ÉNÉâÉX
		PrintData = m_pSnFhyo10Data;
	}
	else{
		//ïtï\4ÉfÅ[É^ì«çûÇ›ÉNÉâÉX
		PrintData = m_pSnFhyo40Data;
	}
//àÍî ñ{ï\//honpyo/1/2/2-2

//ä»à’ñ{ï\//honpyo/4/5/5-2

	//á@
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		m_Arith.l_add( kingaku , PrintData->Sn_1F1A , PrintData->Sn_1F1B );
	}
	m_Arith.l_add( kingaku , kingaku , PrintData->Sn_1F1C );
	memmove( ANS[0].val , kingaku , MONY_BUF_SIZE);

	//áA
	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		m_Arith.l_add( WORK2 , PrintData->Sn_1F2A , PrintData->Sn_1F2B );
		m_Arith.l_add( WORK2 , WORK2 , PrintData->Sn_1F2C );
	}
	else{
		memmove( WORK2 , m_pSnHonpyoData->Sn_SYTX , MONY_BUF_SIZE);
	}
	memmove( ANS[1].val , WORK2 , MONY_BUF_SIZE);

	//áB
	memset( kingaku3 , '\0' , MONY_BUF_SIZE);
	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		m_Arith.l_add( WORK3 , PrintData->Sn_1F3A , PrintData->Sn_1F3B );
	}
	m_Arith.l_add( WORK3 , WORK3 , PrintData->Sn_1F3C );
	memmove( ANS[2].val , WORK3 , MONY_BUF_SIZE);
	//áC
	memset( kingaku , '\0' , MONY_BUF_SIZE);
//-- '14.10.17 --
//	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
//		m_Arith.l_add( kingaku , PrintData->Sn_1F4A , PrintData->Sn_1F4B );
//	}
//	m_Arith.l_add( kingaku , kingaku , PrintData->Sn_1F4C );
//---------------
	if( m_pSnFhyo40Data ){
		if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
			m_Arith.l_add( kingaku , kingaku , PrintData->Sn_1F4T );
		}
		else{
			m_Arith.l_add( kingaku , kingaku , PrintData->Sn_1F4C );
		}
	}
	else{
		if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
			m_Arith.l_add( kingaku , PrintData->Sn_1F4A , PrintData->Sn_1F4B );
		}
		m_Arith.l_add( kingaku , kingaku , PrintData->Sn_1F4C );
	}
//---------------
	memmove( ANS[3].val , kingaku , MONY_BUF_SIZE);
	m_Arith.l_add( WORK7 , WORK7 ,kingaku );
	//áD
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		m_Arith.l_add( kingaku , PrintData->Sn_1F5A , PrintData->Sn_1F5B );
	}
	m_Arith.l_add( kingaku , kingaku , PrintData->Sn_1F5C );
	memmove( ANS[4].val , kingaku , MONY_BUF_SIZE);
	m_Arith.l_add( WORK7 , WORK7 ,kingaku );
	//áE
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		m_Arith.l_add( kingaku , PrintData->Sn_1F6A , PrintData->Sn_1F6B );
	}
	m_Arith.l_add( kingaku , kingaku , PrintData->Sn_1F6C );
	memmove( ANS[5].val , kingaku , MONY_BUF_SIZE);
	m_Arith.l_add( WORK7 , WORK7 ,kingaku );
	//áF
	memmove( ANS[6].val , WORK7 , MONY_BUF_SIZE);
	//áGáH 7-2-3
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	m_Arith.l_sub( kingaku , WORK7 , WORK2 );
	m_Arith.l_sub( kingaku , kingaku , WORK3 );
	m_Arith.l_add( WORK8 , WORK8 , kingaku );
	
	if( m_Arith.l_test( kingaku )  < 0 ){//É}ÉCÉiÉXÇÃèÍçá
		m_Arith.l_neg( kingaku );
		memmove( ANS[8].val , kingaku , MONY_BUF_SIZE);//áH
		m_Arith.l_div( ANS[8].val , ANS[8].val , PW1 );
		m_Arith.l_mul( ANS[8].val , ANS[8].val , PW1 );
	}
	else{//ÉvÉâÉXÇÃèÍçá
		if( m_pSnHeadData->Sn_SKKBN%2 ){//íÜä‘ÇÃèÍçáÅAä“ïtéûÇÕï\é¶ÇµÇ»Ç¢
			memmove( ANS[7].val , kingaku , MONY_BUF_SIZE);//áG
		}
		else{
			memset( kingaku , '\0' , MONY_BUF_SIZE );
		}
	}

	//áI
	memmove( ANS[9].val, m_pSnHonpyoData->Sn_TYNOFZ, MONY_BUF_SIZE );

	//áJ 9-10
//	if(  m_Arith.l_test( m_pSnHonpyoData->Sn_TYNOFZ )){//íÜä‘î[ïtê≈äzÇ™Ç†ÇÍÇŒ
		m_Arith.l_sub( kingaku , ANS[8].val , m_pSnHonpyoData->Sn_TYNOFZ);

		if( m_Arith.l_test( kingaku )  < 0 ){//É}ÉCÉiÉXÇÃèÍçá
			m_Arith.l_neg( kingaku );
			memmove( ANS[11].val , kingaku , MONY_BUF_SIZE);//áJ
		}
		else{//ÉvÉâÉXÇÃèÍçá
			memmove( ANS[10].val , kingaku , MONY_BUF_SIZE);//áK
		}
//	}
	//áL
	if( m_pSnHeadData->Sn_SKKBN == 3 || m_pSnHeadData->Sn_SKKBN == 4 ){//èCê≥ÇÃèÍçáÇÃÇ›
		memmove( ANS[12].val, m_pSnHonpyoData->Sn_KAKTIZ, MONY_BUF_SIZE );
		//áMç∑à¯î[ïtê≈äz
/*- '14.04.29 -*/
//		if( m_Arith.l_test( ANS[10].val ) || m_Arith.l_test( ANS[11].val )){
//			m_Arith.l_sub( ANS[13].val , ANS[10].val , m_pSnHonpyoData->Sn_KAKTIZ);
//			m_Arith.l_sub( ANS[13].val , ANS[13].val , ANS[11].val );
//		}
/*-------------*/
		if( m_Arith.l_test(ANS[10].val) > 0 ){
			m_Arith.l_sub( ANS[13].val , ANS[10].val , m_pSnHonpyoData->Sn_KAKTIZ );
		}
		else{
			m_Arith.l_sub( ANS[13].val , ANS[13].val, ANS[11].val );
			m_Arith.l_sub( ANS[13].val , ANS[13].val, m_pSnHonpyoData->Sn_KAKTIZ );
		}
		m_Arith.l_sub( ANS[13].val , ANS[13].val, ANS[7].val );
/*-------------*/
	}
	//áNâ€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
	memmove( ANS[14].val , m_pSnHonpyoData->Sn_KZURI , MONY_BUF_SIZE );

	if( m_pSnHeadData->m_s_sgn2&0x0f){
		//ä»à’â€ê≈éû
		memmove( ANS[15].val , m_pSnHonpyoData->Sn_ZZURI , MONY_BUF_SIZE );
	}
	else{
		//áOéëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
		memmove( ANS[15].val , m_pSnHonpyoData->Sn_SOURI , MONY_BUF_SIZE );
	}
	//áPçTèúïsë´ä“ïtê≈äz
	//áQç∑à¯ê≈äz
	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		char	WORK0[MONY_BUF_SIZE]={0};
		char	WORK1[MONY_BUF_SIZE]={0};
		char	WORK2[MONY_BUF_SIZE]={0};
		m_Arith.l_add( WORK0 , PrintData->Sn_1F8B , PrintData->Sn_1F8C );//8
		m_Arith.l_add( WORK1 , PrintData->Sn_1F9B , PrintData->Sn_1F9C );//9
		m_Arith.l_sub( WORK2 , WORK1 , WORK0 );//9-8		if( m_Arith.l_test( kingaku ) < 0 ){//É}ÉCÉiÉXÇÃèÍçááPÇ…
		if( m_Arith.l_test( WORK2 ) < 0 ){
			if( m_pSnHeadData->Sn_SKKBN%2 ){//íÜä‘ÇÃèÍçáÅAä“ïtéûÇÕï\é¶ÇµÇ»Ç¢
				m_Arith.l_neg( WORK2 );
				memmove(  ANS[16].val , WORK2 , MONY_BUF_SIZE );//áQ
			}
		}
		else{//ÉvÉâÉXÇÃèÍçááQÇ…
//			if( m_pSnHeadData->Sn_SKKBN != 2 ){
				memmove(  ANS[17].val , WORK2 , MONY_BUF_SIZE );//áP
//			}
		}
	}
	else{
		memmove( kingaku , WORK8 , MONY_BUF_SIZE);
		if( m_Arith.l_test( kingaku ) < 0 ){//É}ÉCÉiÉXÇÃèÍçááPÇ…
			m_Arith.l_neg( kingaku );
			memmove(  ANS[17].val , kingaku , MONY_BUF_SIZE );//áQ
			m_Arith.l_div( ANS[17].val , ANS[17].val , PW1 );
			m_Arith.l_mul( ANS[17].val , ANS[17].val , PW1 );
		}
		else{//ÉvÉâÉXÇÃèÍçááQÇ…
			if( m_pSnHeadData->Sn_SKKBN != 2  ){
				if( m_pSnHeadData->Sn_SKKBN%2 ){//íÜä‘ÇÃèÍçáÅAä“ïtéûÇÕï\é¶ÇµÇ»Ç¢
					memmove(  ANS[16].val , kingaku , MONY_BUF_SIZE );//áP
				}
			}
		}
	}

	//áRè˜ìnäÑäzä“ïtäz
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	memset( kingaku2 , '\0' , MONY_BUF_SIZE);
		
	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		m_Util.percent( kingaku,(char*)PrintData->Sn_1F8B, 7, 25, 0 );
		m_Arith.l_add(kingaku2 , kingaku2 , kingaku);
	}
	m_Util.percent( kingaku,(char*)PrintData->Sn_1F8C, 8, 17, 0 );
	m_Arith.l_add(kingaku2, kingaku2 , kingaku);

	//20è˜ìnäÑäzî[ê≈äz
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	memset( kingaku3 , '\0' , MONY_BUF_SIZE);
	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		m_Util.percent( kingaku, (char*)PrintData->Sn_1F9B, 7, 25, 0 );
		m_Arith.l_add(kingaku3 , kingaku3 , kingaku);
		m_Util.percent( kingaku, (char*)PrintData->Sn_1F9C, 8, 17, 0 );
		m_Arith.l_add(kingaku3 , kingaku3 , kingaku);
		m_Arith.l_sub(kingaku3 , kingaku3 , kingaku2);
		if( m_Arith.l_test( kingaku3 ) < 0 ){//É}ÉCÉiÉXÇÃèÍçááRÇ…
			if( (m_pSnHeadData->Sn_SKKBN%2)  ){
				memmove(  ANS[18].val , kingaku3 , MONY_BUF_SIZE );
				m_Arith.l_neg(  ANS[18].val );
		//		m_Arith.l_div( ANS[18].val , ANS[18].val , PW1 );
		//		m_Arith.l_mul( ANS[18].val , ANS[18].val , PW1 );
			}
		}
		else{
			memmove(  ANS[19].val , kingaku3 , MONY_BUF_SIZE );
			m_Arith.l_div( ANS[19].val , ANS[19].val , PW1 );
			m_Arith.l_mul( ANS[19].val , ANS[19].val , PW1 );
		}
	}
	else{
		if( m_Arith.l_test( ANS[17].val ) ){
			m_Util.percent( kingaku,(char*)ANS[17].val, 8, 17, 0 );
			m_Arith.l_add(kingaku3 , kingaku3 , kingaku);
			m_Arith.l_div( ANS[19].val , kingaku3 , PW1 );
			m_Arith.l_mul( ANS[19].val , ANS[19].val , PW1 );
		}
		else{
			m_Util.percent( kingaku,(char*)ANS[16].val, 8, 17, 0 );
			m_Arith.l_add(kingaku3 , kingaku3 , kingaku);
			memmove(  ANS[18].val , kingaku3 , MONY_BUF_SIZE );
//			m_Arith.l_neg(  ANS[18].val );
		}

	}


	//21íÜä‘î[ïtè˜ìnäÑäz
	memmove( ANS[20].val , m_pSnHonpyoData->Sn_TTYWAR , MONY_BUF_SIZE);
	//22
	m_Arith.l_sub( kingaku , ANS[19].val, ANS[20].val );
	if( m_Arith.l_test( kingaku ) < 0 ){//É}ÉCÉiÉXÇÃèÍçááRÇ…
		memmove(  ANS[22].val , kingaku , MONY_BUF_SIZE );
		m_Arith.l_neg(  ANS[22].val );
		m_Arith.l_div( ANS[22].val , ANS[22].val , PW1 );
		m_Arith.l_mul( ANS[22].val , ANS[22].val , PW1 );
	}
	else{
		memmove(  ANS[21].val , kingaku , MONY_BUF_SIZE );
	}
	if(  m_pSnHeadData->Sn_SKKBN == 3 ||  m_pSnHeadData->Sn_SKKBN == 4 ){
		//24
		memmove( ANS[23].val , m_pSnHonpyoData->Sn_TKAKTZ , MONY_BUF_SIZE);
		//25
		//ç∑à¯î[ïtê≈äz
/*- '14.04.29 -*/
//		if( m_Arith.l_test( ANS[21].val ) || m_Arith.l_test( ANS[22].val )){
//			m_Arith.l_sub( ANS[24].val , ANS[21].val , m_pSnHonpyoData->Sn_TKAKTZ);
//			m_Arith.l_sub( ANS[24].val , ANS[24].val , ANS[22].val );
//		}
/*-------------*/
		if( m_Arith.l_test(ANS[21].val) > 0 ){
			m_Arith.l_sub( ANS[24].val , ANS[21].val , m_pSnHonpyoData->Sn_TKAKTZ );
		}
		else{
			char	tmpMony[MONY_BUF_SIZE]={0};
			m_Arith.l_add( tmpMony, ANS[18].val, ANS[22].val );
			if( m_Arith.l_test(tmpMony) ){
				m_Arith.l_neg( tmpMony );
			}
			m_Arith.l_sub( ANS[24].val , tmpMony, m_pSnHonpyoData->Sn_TKAKTZ );
		}
/*-------------*/
		m_Arith.l_div( ANS[24].val , ANS[24].val , PW1 );
		m_Arith.l_mul( ANS[24].val , ANS[24].val , PW1 );

	}
	
//JANS
	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		memmove( JANS[0].val ,  PrintData->Sn_1F1A , MONY_BUF_SIZE);
		memmove( JANS[1].val ,  PrintData->Sn_1F1B , MONY_BUF_SIZE);
	}

	memmove( JANS[2].val ,  PrintData->Sn_1F1C , MONY_BUF_SIZE);

	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		memmove( JANS[3].val ,  PrintData->Sn_1F2A , MONY_BUF_SIZE);
		memmove( JANS[4].val ,  PrintData->Sn_1F2B , MONY_BUF_SIZE);
	}
	memmove( JANS[5].val ,  PrintData->Sn_1F2C , MONY_BUF_SIZE);

	if( m_pSnHeadData->SVmzsw == 1 ){// åoâﬂë[íuëŒè€ÇÃâ€ê≈éëéYÇÃè˜ìnìôÇ†ÇË
		m_Arith.l_sub( kingaku , PrintData->Sn_1F9B , PrintData->Sn_1F8B );
		if( m_Arith.l_test( ANS[16].val ) > 0){//ä“ïtÇÃÇ∆Ç´
			if( !(m_pSnHeadData->Sn_SKKBN%2) ){
				memset( kingaku , '\0' , MONY_BUF_SIZE);
			}
			else{
/*- '14.06.03_CUT -*/
//				m_Arith.l_neg ( kingaku );
/*-----------------*/
			}
		}
		if( !m_Arith.l_test( ANS[16].val ) && ( !m_Arith.l_test( ANS[17].val ) || ( m_Arith.l_cmp( ANS[17].val , val100 ) < 0 ))){
			memset( kingaku , '\0' , MONY_BUF_SIZE);
		}
		memmove( JANS[6].val ,  kingaku , MONY_BUF_SIZE);
		// ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz6.3%
		m_Arith.l_sub( kingaku , PrintData->Sn_1F9C , PrintData->Sn_1F8C );
		if( m_Arith.l_test( ANS[16].val ) > 0){//ä“ïtÇÃÇ∆Ç´
			if( !(m_pSnHeadData->Sn_SKKBN%2) ){
				memset( kingaku , '\0' , MONY_BUF_SIZE);
			}
			else{
/*- '14.06.03_CUT -*/
//				m_Arith.l_neg ( kingaku );
/*-----------------*/
			}
		}
		if( !m_Arith.l_test( ANS[16].val ) && ( !m_Arith.l_test( ANS[17].val ) || ( m_Arith.l_cmp( ANS[17].val , val100 ) < 0 ))){
			memset( kingaku , '\0' , MONY_BUF_SIZE);
		}
		memmove( JANS[7].val ,  kingaku , MONY_BUF_SIZE);
	}
	else{
		if( m_Arith.l_test (ANS[16].val) ){
			memmove( JANS[7].val ,  ANS[16].val , MONY_BUF_SIZE);
//--> '14.06.03 INS START
			m_Arith.l_neg ( JANS[7].val );
//<-- '14.06.03 INS END
		}
		else{
			memmove( JANS[7].val ,  ANS[17].val , MONY_BUF_SIZE);
		}
	}
	int FontFlg = 0;
//2015.08.04 INSERT START
	int FontFlg2 = 0; //äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇÇÃÉtÉHÉìÉgÉTÉCÉYÉtÉâÉO 0:ïWèÄÅHÉTÉCÉYÅ@1:è¨Ç≥Ç¢ÉTÉCÉY
//2015.08.04 INSERT END
	char buff[32] = {0};
	int DefSize = 0 , Size = 0;
	if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
		Size = 15;
	}
	else{
		Size = 15;
	}
	for( int idx = 0; idx < 3; idx++){
		::memset( buff , '\0' , sizeof( buff ) );
		m_Arith.l_print( buff , JANS[idx].val , FMT16_EX );
		str.Format( _T("%s") , buff );
		str.TrimLeft();
		if( str.GetLength() > Size ){
			FontFlg = 1;
			break;
		}
	}
	for( int idx = 3; idx < 6; idx++){
		::memset( buff , '\0' , sizeof( buff ) );
		m_Arith.l_print( buff , JANS[idx].val , FMT16_EX );
		str.Format( _T("%s") , buff );
		str.TrimLeft();
		if( str.GetLength() > 11 ){
			FontFlg = 1;
			break;
		}
	}

	dp = AP;
	memset( dp, '\0', size );
	memset( WORK0, '\0', sizeof(WORK0) );
	i = j = 0;
	m_Arith.l_div( WORK0, ANS[j].val, PW0 );
	m_Arith.l_print( dp, WORK0, FMT011EX );
//	strcat_s( dp, 13+1, "   " );
	m_Arith.l_mul( ANS[j].val , WORK0 , PW1);
	PF1[i].PITM[j] = dp;
	dp+=17;
	i++;
	// ã‡äz¢1£->¢3£
	for( j=0; j<2; ++j, dp+=17 ){	
//		switch( j ){
//			case 0 :
//				m_Arith.l_div( WORK0, ANS[j].val, PW0 );
//				m_Arith.l_print( dp, WORK0, FMT09 );
//				strcat_s( dp, 13+1, "   " );
//				m_Arith.l_mul( ANS[j].val , WORK0 , PW1);
//				break;
//			default:
				m_Arith.l_print( dp, ANS[j+1].val, FMT12EX );
//				break;
//		}
		PF1[i].PITM[j] = dp;
	}
	
	// ã‡äz¢4£->¢8£
	for( ++i, j=0; j<5; ++j, dp+=17 ){
		m_Arith.l_print( dp, ANS[j+3].val, FMT12EX );
		PF1[i].PITM[j] = dp;
	}

	// ã‡äz¢9£->¢12£
	for( ++i, j=0; j<4; ++j, dp += 17 ){
		switch( j ){
			case 1 :
				// íÜä‘î[ïtê≈äz
				if( !(m_pPrintCmInfo->PRtype&0x80) && m_pPrintCmInfo->OPB ){		
					PF1[i].PITM[j] = (char *)0;
					continue;
				}
				// Å´â∫Ç÷
			default:
				m_Arith.l_div( WORK0, ANS[j+8].val, PW1 );
				m_Arith.l_print( dp, WORK0, FMT12EX );
				strcat_s( dp, 16+1, "  " );
				m_Arith.l_mul( ANS[j+8].val , WORK0 , PW1);
				break;
		}
		PF1[i].PITM[j] = dp;
	}
	
	// ã‡äz¢13£->¢16£
	for( ++i, j=0; j<4; ++j, dp += 18 ){
		switch( j ){
			case 1 :
				m_Arith.l_div( WORK0, ANS[j+12].val, PW1 );
				m_Arith.l_print( dp, WORK0, FMT12 );
				strcat_s( dp, 17+1, "  " );
				m_Arith.l_mul( ANS[j+12].val , WORK0 , PW1);
				break;
			default:
				m_Arith.l_print( dp, ANS[j+12].val, FMT13EX );
				break;
		}
		PF1[i].PITM[j] = dp;
	}
	
	// ã‡äz¢17£->¢20£
	for( ++i, j=0; j<4; ++j, dp+=17 ){
		switch( j ){
			case 0 :
			case 2 :
				m_Arith.l_print( dp, ANS[j+16].val, FMT12EX );
				break;
			default:	
				m_Arith.l_div( WORK0, ANS[j+16].val, PW1 );
				m_Arith.l_print( dp, WORK0, FMT011EX );
				strcat_s( dp, 16+1, "  " );
				m_Arith.l_mul( ANS[j+16].val , WORK0 , PW1);
				break;
		}
		PF1[i].PITM[j] = dp;
	}
	
	// ã‡äz¢21£->¢25£
	for( ++i, j=0; j<5; ++j, dp+=18 ){
		switch( j ){
			case 3 :
				m_Arith.l_print( dp, ANS[j+20].val, FMT13EX );
				break;
			case 0 :
				// íÜä‘î[ïtè˜ìnäÑäz
				if( ! (m_pPrintCmInfo->PRtype&0x80) && m_pPrintCmInfo->OPB ){
					PF1[i].PITM[j] = (char *)0;
					continue;
				}
				// Å´â∫Ç÷
			default:
				m_Arith.l_div( WORK0, ANS[j+20].val, PW1 );
				m_Arith.l_print( dp, WORK0, FMT12 );
				strcat_s( dp, 17+1, "  " );
				m_Arith.l_mul( ANS[j+20].val , WORK0 , PW1);
				break;
		}
		PF1[i].PITM[j] = dp;
	}
	
	// ê≈äzç∑à¯Ç´ÇOâ~É`ÉFÉbÉN
	for( k=0; k<26; k++ ){
		if( m_Arith.l_test( ANS[k].val ) ){
			k = 0;
			break;
		}
	}
	//26
	// è¡îÔê≈ãyÇ—ínï˚è¡îÔê≈ÇÃçáåvÅiî[ïtñîÇÕä“ïtÅjê≈äz
	switch( m_pSnHeadData->Sn_SKKBN&0xff ) {
		case 1 : case 2 :		// ämíËÅEíÜä‘ê\çê
			m_Arith.l_add( kingaku, ANS[10].val, ANS[21].val );
			m_Arith.l_add( kingaku2, ANS[7].val,  ANS[11].val );
			m_Arith.l_add( kingaku2 , kingaku2, ANS[18].val );
			m_Arith.l_add( kingaku2 , kingaku2, ANS[22].val );
			m_Arith.l_sub( ANS[25].val, kingaku, kingaku2 );
			break;
		default	:				// èCê≥ê\çê
			m_Arith.l_add( ANS[25].val, ANS[13].val, ANS[24].val );
			break;
	}
	// ã‡äz¢26£
	m_Arith.l_print( dp, ANS[25].val, ( k ? FMT12EX : FMT120EX ) );	
	PF1[++i].PITM[0] = dp;
	dp += 17;
	
	if( j = _xprn( PP, 2, PF1, 0, pDC, 0, SEno ) ){
		goto POUTn;
	}

	// ê≈óùémñ@Å@é{çsëOí†ï[	ÉXÉPÉWÉÖÅ[ÉãÅ@ÉXÉLÉbÉv
	dp = AP;
	i = j = 0;
	memset( dp, '\0', size );

	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_KAPPU ? "Åõ" : "Å@");	// ïtãLéñçÄ
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_KAPPU ? "Å@" : "Åõ");
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_NOBE  ? "Åõ" : "Å@");
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_NOBE  ? "Å@" : "Åõ");
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_KOJI  ? "Åõ" : "Å@");
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_KOJI  ? "Å@" : "Åõ");
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_GENKN ? "Åõ" : "Å@");
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_GENKN ? "Å@" : "Åõ");
//-- '15.03.06 --
////	PF1[i].PITM[j++] = (K221sw ? "Åõ" : "Å@");
//	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_TUMIAG ? "Åõ" : "Å@");
////	PF1[i++].PITM[j] = (K221sw ? "Å@" : "Åõ");
//	PF1[i++].PITM[j] = (m_pSnHeadData->Sn_TUMIAG ? "Å@" : "Åõ");
//---------------
	PF1[i].PITM[j++] = ((m_pSnHeadData->Sn_TUMIAG&0x01) ? "Åõ" : "Å@");
	PF1[i++].PITM[j] = ((m_pSnHeadData->Sn_TUMIAG&0x01) ? "Å@" : "Åõ");
//---------------
	
	j = 0;
	if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
/*- '14.06.16 -*/
//		if( m_pSnHeadData->IsUseSemiUriRatio() ){
//			PF1[i].PITM[j++] = "Åõ";
//			PF1[i].PITM[j++] = "Å@";
//			PF1[i].PITM[j++] = (char *)0;
//		}
//		else{
//			if( m_pSnHeadData->IsRatioOver95() == FALSE ){	// 95%ñ¢ñû
//				if( m_pSnHeadData->IsKobetuSiireAnbun() ){
//					PF1[i].PITM[j++] = ("Åõ");
//					PF1[i].PITM[j++] = ("Å@");
//				}
//				else{
//					PF1[i].PITM[j++] = ("Å@");
//					PF1[i].PITM[j++] = ("Åõ");
//				}
//				PF1[i].PITM[j++] = (char *)0;
//			}
//			else{											// ëSäzçTèú
//				PF1[i].PITM[j++] = (char *)0;
//				PF1[i].PITM[j++] = (char *)0;
//				PF1[i].PITM[j++] = "Åõ";
//			}
//		}
/*-------------*/
		if( m_pSnHeadData->IsUseSemiUriRatio() ){
			PF1[i].PITM[j++] = "Åõ";
			PF1[i].PITM[j++] = "Å@";
			PF1[i].PITM[j++] = (char *)0;
		}
		else{
//-- '15.04.18 --
//			if( (m_pSnHeadData->IsRatioOver95()==TRUE) || (m_pSnHeadData->Sn_EXP100&0x01) ){	// ëSäzçTèú
//---------------
			if( !(m_pSnHeadData->Sn_Sign4&0x10) &&
				((m_pSnHeadData->IsRatioOver95()==TRUE)||(m_pSnHeadData->Sn_EXP100&0x01)) ){	// ëSäzçTèú
//---------------
				PF1[i].PITM[j++] = (char *)0;
				PF1[i].PITM[j++] = (char *)0;
				PF1[i].PITM[j++] = "Åõ";
			}
			else{
				if( m_pSnHeadData->IsKobetuSiireAnbun() ){
					PF1[i].PITM[j++] = ("Åõ");
					PF1[i].PITM[j++] = ("Å@");
				}
				else{
					PF1[i].PITM[j++] = ("Å@");
					PF1[i].PITM[j++] = ("Åõ");
				}
				PF1[i].PITM[j++] = (char *)0;
			}
		}
/*-------------*/
	}
	else{
		// éñã∆ï â€ê≈îÑè„çÇ
//--> '15.02.09 INS START
		unsigned char		exSaleFlg = 0x00;	// D0:ëÊÇPéÌ, D1:ëÊÇQéÌ, D2:ëÊÇRéÌ, D3:ëÊÇSéÌ, D4:ëÊÇTéÌ
		CH26SnFhyo50Data	*pH26SnFhyo50Data = NULL;
		if( m_pSnHeadData->SVmzsw ){
			pH26SnFhyo50Data = m_pSnFhyo52Data;
		}
		else{
			pH26SnFhyo50Data = m_pSnFhyo50Data;
		}
//<-- '15.02.09 INS END
		memset( WORK0    , '\0', sizeof( WORK0 ) );
		memset( WORK_ALL , '\0', sizeof( WORK_ALL ) );
//-- '15.01.26 --
//		m_Util.l_calq( WORK0, (char *)m_pSnHonpyoData->Sn_KURI1, 2 );
//---------------
		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI1) >= 0 ){
			m_Util.l_calq( WORK0, (char *)m_pSnHonpyoData->Sn_KURI1, 2 );
//--> '15.02.09 INS START
			exSaleFlg |= 0x01;
//<-- '15.02.09 INS END
		}
		else{
			memset( WORK0    , '\0', sizeof( WORK0 ) );
		}
//---------------
		m_Arith.l_div( WORK0, WORK0, PW0 );
//-- '15.02.03 --
//		m_Arith.l_print( dp, WORK0, FMT11 );
//---------------
		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI1) == 0 ){
			if( pH26SnFhyo50Data &&
				(m_Arith.l_test(pH26SnFhyo50Data->Sn_5FURI1A)||m_Arith.l_test(pH26SnFhyo50Data->Sn_5FURI1B)||m_Arith.l_test(pH26SnFhyo50Data->Sn_5FURI1C)) ){
				m_Arith.l_print( dp, WORK0, FMT11Z );
			}
			else{
				m_Arith.l_print( dp, WORK0, FMT11 );
//--> '15.02.09 INS START
				exSaleFlg &= ~0x01;
//<-- '15.02.09 INS END
			}
		}
//--> '15.02.10 INS START
		else if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI1) < 0 ){
			m_Arith.l_print( dp, WORK0, FMT11Z );
			exSaleFlg |= 0x01;
		}
//<-- '15.02.10 INS END
		else{
			m_Arith.l_print( dp, WORK0, FMT11 );
		}
//---------------
		m_Arith.l_add( WORK_ALL , WORK_ALL , m_pSnHonpyoData->Sn_KURI1);
		PF1[i].PITM[j++] = dp;
		dp += 12;

//-- '15.01.26 --
//		m_Util.l_calq( WORK0, (char *)m_pSnHonpyoData->Sn_KURI2, 2 );
//---------------
		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI2) >= 0 ){
			m_Util.l_calq( WORK0, (char *)m_pSnHonpyoData->Sn_KURI2, 2 );
//--> '15.02.09 INS START
			exSaleFlg |= 0x02;
//<-- '15.02.09 INS END
		}
		else{
			memset( WORK0    , '\0', sizeof( WORK0 ) );
		}
//---------------
		m_Arith.l_div( WORK0, WORK0, PW0 );
//-- '15.02.03 --
//		m_Arith.l_print( dp, WORK0, FMT11 );
//---------------
		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI2) == 0 ){
			if( pH26SnFhyo50Data &&
				(m_Arith.l_test(pH26SnFhyo50Data->Sn_5FURI2A)||m_Arith.l_test(pH26SnFhyo50Data->Sn_5FURI2B)||m_Arith.l_test(pH26SnFhyo50Data->Sn_5FURI2C)) ){
				m_Arith.l_print( dp, WORK0, FMT11Z );
			}
			else{
				m_Arith.l_print( dp, WORK0, FMT11 );
//--> '15.02.09 INS START
				exSaleFlg &= ~0x02;
//<-- '15.02.09 INS END
			}
		}
//--> '15.02.10 INS START
		else if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI2) < 0 ){
			m_Arith.l_print( dp, WORK0, FMT11Z );
			exSaleFlg |= 0x02;
		}
//<-- '15.02.10 INS END
		else{
			m_Arith.l_print( dp, WORK0, FMT11 );
		}
//---------------
		m_Arith.l_add( WORK_ALL , WORK_ALL , m_pSnHonpyoData->Sn_KURI2);
		PF1[i].PITM[j++] = dp;
		dp += 12;

//-- '15.01.26 --
//		m_Util.l_calq( WORK0, (char *)m_pSnHonpyoData->Sn_KURI3, 2 );
//---------------
		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI3) >= 0 ){
			m_Util.l_calq( WORK0, (char *)m_pSnHonpyoData->Sn_KURI3, 2 );
//--> '15.02.09 INS START
			exSaleFlg |= 0x04;
//<-- '15.02.09 INS END
		}
		else{
			memset( WORK0    , '\0', sizeof( WORK0 ) );
		}
//---------------
		m_Arith.l_div( WORK0, WORK0, PW0 );
//-- '15.02.03 --
//		m_Arith.l_print( dp, WORK0, FMT11 );
//---------------
		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI3) == 0 ){
			if( pH26SnFhyo50Data &&
				(m_Arith.l_test(pH26SnFhyo50Data->Sn_5FURI3A)||m_Arith.l_test(pH26SnFhyo50Data->Sn_5FURI3B)||m_Arith.l_test(pH26SnFhyo50Data->Sn_5FURI3C)) ){
				m_Arith.l_print( dp, WORK0, FMT11Z );
			}
			else{
				m_Arith.l_print( dp, WORK0, FMT11 );
//--> '15.02.09 INS START
				exSaleFlg &= ~0x04;
//<-- '15.02.09 INS END
			}
		}
//--> '15.02.10 INS START
		else if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI3) < 0 ){
			m_Arith.l_print( dp, WORK0, FMT11Z );
			exSaleFlg |= 0x04;
		}
//<-- '15.02.10 INS END
		else{
			m_Arith.l_print( dp, WORK0, FMT11 );
		}
//---------------
		m_Arith.l_add( WORK_ALL , WORK_ALL , m_pSnHonpyoData->Sn_KURI3);
		PF1[i].PITM[j++] = dp;
		dp += 12;

//-- '15.01.26 --
//		m_Util.l_calq( WORK0, (char *)m_pSnHonpyoData->Sn_KURI4, 2 );
//---------------
		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI4) >= 0 ){
			m_Util.l_calq( WORK0, (char *)m_pSnHonpyoData->Sn_KURI4, 2 );
//--> '15.02.09 INS START
			exSaleFlg |= 0x08;
//<-- '15.02.09 INS END
		}
		else{
			memset( WORK0    , '\0', sizeof( WORK0 ) );
		}
//---------------
		m_Arith.l_div( WORK0, WORK0, PW0 );
//-- '15.02.03 --
//		m_Arith.l_print( dp, WORK0, FMT11 );
//---------------
		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI4) == 0 ){
			if( pH26SnFhyo50Data &&
				(m_Arith.l_test(pH26SnFhyo50Data->Sn_5FURI4A)||m_Arith.l_test(pH26SnFhyo50Data->Sn_5FURI4B)||m_Arith.l_test(pH26SnFhyo50Data->Sn_5FURI4C)) ){
				m_Arith.l_print( dp, WORK0, FMT11Z );
			}
			else{
				m_Arith.l_print( dp, WORK0, FMT11 );
//--> '15.02.09 INS START
				exSaleFlg &= ~0x08;
//<-- '15.02.09 INS END
			}
		}
//--> '15.02.10 INS START
		else if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI4) < 0 ){
			m_Arith.l_print( dp, WORK0, FMT11Z );
			exSaleFlg |= 0x08;
		}
//<-- '15.02.10 INS END
		else{
			m_Arith.l_print( dp, WORK0, FMT11 );
		}
//---------------
		m_Arith.l_add( WORK_ALL , WORK_ALL , m_pSnHonpyoData->Sn_KURI4);
		PF1[i].PITM[j++] = dp;
		dp += 12;

//-- '15.01.26 --
//		m_Util.l_calq( WORK0, (char *)m_pSnHonpyoData->Sn_KURI5, 2 );
//---------------
		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI5) >= 0 ){
			m_Util.l_calq( WORK0, (char *)m_pSnHonpyoData->Sn_KURI5, 2 );
//--> '15.02.09 INS START
			exSaleFlg |= 0x10;
//<-- '15.02.09 INS END
		}
		else{
			memset( WORK0    , '\0', sizeof( WORK0 ) );
		}
//---------------
		m_Arith.l_div( WORK0, WORK0, PW0 );
//-- '15.02.03 --
//		m_Arith.l_print( dp, WORK0, FMT11 );
//---------------
		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI5) == 0 ){
			if( pH26SnFhyo50Data &&
				(m_Arith.l_test(pH26SnFhyo50Data->Sn_5FURI5A)||m_Arith.l_test(pH26SnFhyo50Data->Sn_5FURI5B)||m_Arith.l_test(pH26SnFhyo50Data->Sn_5FURI5C)) ){
				m_Arith.l_print( dp, WORK0, FMT11Z );
			}
			else{
				m_Arith.l_print( dp, WORK0, FMT11 );
//--> '15.02.09 INS START
				exSaleFlg &= ~0x10;
//<-- '15.02.09 INS END
			}
		}
//--> '15.02.10 INS START
		else if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI5) < 0 ){
			m_Arith.l_print( dp, WORK0, FMT11Z );
			exSaleFlg |= 0x10;
		}
//<-- '15.02.10 INS END
		else{
			m_Arith.l_print( dp, WORK0, FMT11 );
		}
//---------------
		m_Arith.l_add( WORK_ALL , WORK_ALL , m_pSnHonpyoData->Sn_KURI5);
		PF1[i].PITM[j++] = dp;
		dp += 12;


		//[Y]
		if(m_SixKindFlg == TRUE){
			//òZéÌÇÃèÍçá
//-- '15.01.26 --
//			m_Util.l_calq( WORK0, (char *)((CH27SnHonpyoData*)m_pSnHonpyoData)->Sn_KURI6, 2 );
//---------------
			if( m_Arith.l_test(((CH27SnHonpyoData*)m_pSnHonpyoData)->Sn_KURI6 ) >= 0 ){
				m_Util.l_calq( WORK0, (char *)((CH27SnHonpyoData*)m_pSnHonpyoData)->Sn_KURI6, 2 );
				//--> '15.02.09 INS START
				exSaleFlg |= 0x20;
				//<-- '15.02.09 INS END
			}
			else{
				memset( WORK0    , '\0', sizeof( WORK0 ) );
			}
//---------------
			m_Arith.l_div( WORK0, WORK0, PW0 );
//-- '15.02.03 --
//			m_Arith.l_print( dp, WORK0, FMT11 );
//---------------
			if( m_Arith.l_test(((CH27SnHonpyoData*)m_pSnHonpyoData)->Sn_KURI6) == 0 ){

				if( (CH27SnFhyo50Data*)pH26SnFhyo50Data &&
					(m_Arith.l_test(((CH27SnFhyo50Data*)pH26SnFhyo50Data)->Sn_5FURI6A)||m_Arith.l_test(((CH27SnFhyo50Data*)pH26SnFhyo50Data)->Sn_5FURI6B)||m_Arith.l_test(((CH27SnFhyo50Data*)pH26SnFhyo50Data )->Sn_5FURI6C)) ){
					m_Arith.l_print( dp, WORK0, FMT11Z );
				}
				else{
					m_Arith.l_print( dp, WORK0, FMT11 );
	//--> '15.02.09 INS START
					exSaleFlg &= ~0x20;
	//<-- '15.02.09 INS END
				}
			}
			//--> '15.02.10 INS START
			else if( m_Arith.l_test(((CH27SnHonpyoData*)m_pSnHonpyoData)->Sn_KURI6) < 0 ){
				m_Arith.l_print( dp, WORK0, FMT11Z );
				exSaleFlg |= 0x20;
			}
	//<-- '15.02.10 INS END
			else{
				m_Arith.l_print( dp, WORK0, FMT11 );
			}
//---------------
			m_Arith.l_add( WORK_ALL , WORK_ALL , ((CH27SnHonpyoData*)m_pSnHonpyoData)->Sn_KURI6 );
			PF1[i++].PITM[j] = dp;
			dp += 12;
		}
		else{
			// çáåv
			m_Util.l_calq( WORK0, (char *)m_pSnHeadData->Sn_KURIA, 2 );
			m_Arith.l_div( WORK0, WORK0, PW0 );
	//		m_Arith.l_div( WORK_ALL, WORK_ALL, PW0 );
//-- '15.02.09 --
//			m_Arith.l_print( dp, WORK0, FMT11 );
//---------------
			if( exSaleFlg ){
				m_Arith.l_print( dp, WORK0, FMT11Z );
			}
			else{
				m_Arith.l_print( dp, WORK0, FMT11 );
			}
//---------------

			PF1[i++].PITM[j] = dp;
			dp += 12;
		}
		
		/*//[Y]
		// çáåv
		m_Util.l_calq( WORK0, (char *)m_pSnHeadData->Sn_KURIA, 2 );
		m_Arith.l_div( WORK0, WORK0, PW0 );
//		m_Arith.l_div( WORK_ALL, WORK_ALL, PW0 );
//-- '15.02.09 --
//		m_Arith.l_print( dp, WORK0, FMT11 );
//---------------
		if( exSaleFlg ){
			m_Arith.l_print( dp, WORK0, FMT11Z );
		}
		else{
			m_Arith.l_print( dp, WORK0, FMT11 );
		}
//---------------
		PF1[i++].PITM[j] = dp;
		dp += 12;
		*/
		
		// éñã∆ï â€ê≈îÑè„äÑçá
		j = 0;
//-- '15.02.10 --
//		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI1) ){
//---------------
		if( (m_Arith.l_test(m_pSnHonpyoData->Sn_KURI1)>0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA)>0) ){
//---------------
			wsprintf( dp, "%3d", m_pSnHonpyoData->Sn_UP1 / 10 );
		}
		PF1[i].PITM[j++] = dp;
		dp += 4;

//-- '15.02.10 --
//		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI1) ){
//---------------
		if( (m_Arith.l_test(m_pSnHonpyoData->Sn_KURI1)>0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA)>0) ){
//---------------
			wsprintf( dp, "%01d", m_pSnHonpyoData->Sn_UP1 % 10 );
		}
		strcat_s( dp, 4+1, "  " );
		PF1[i].PITM[j++] = dp;
		dp += 4;

//-- '15.02.10 --
//		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI2) ){
//---------------
		if( (m_Arith.l_test(m_pSnHonpyoData->Sn_KURI2)>0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA)>0) ){
//---------------
			wsprintf( dp, "%3d", m_pSnHonpyoData->Sn_UP2 / 10 );
		}
		PF1[i].PITM[j++] = dp;
		dp += 4;

//-- '15.02.10 --
//		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI2) ){
//---------------
		if( (m_Arith.l_test(m_pSnHonpyoData->Sn_KURI2)>0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA)>0) ){
//---------------
			wsprintf( dp, "%01d", m_pSnHonpyoData->Sn_UP2 % 10 );
		}
		strcat_s( dp, 4+1, "  " );
		PF1[i].PITM[j++] = dp;
		dp += 4;

//-- '15.02.10 --
//		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI3) ){
//---------------
		if( (m_Arith.l_test(m_pSnHonpyoData->Sn_KURI3)>0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA)>0) ){
//---------------
			wsprintf( dp, "%3d", m_pSnHonpyoData->Sn_UP3 / 10 );
		}
		PF1[i].PITM[j++] = dp;
		dp += 4;

//-- '15.02.10 --
//		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI3) ){
//---------------
		if( (m_Arith.l_test(m_pSnHonpyoData->Sn_KURI3)>0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA)>0) ){
//---------------
			wsprintf( dp, "%01d", m_pSnHonpyoData->Sn_UP3 % 10 );
		}
		strcat_s( dp, 4+1, "  " );
		PF1[i].PITM[j++] = dp;
		dp += 4;

//-- '15.02.10 --
//		if( m_Arith.l_test( m_pSnHonpyoData->Sn_KURI4 ) ){
//---------------
		if( (m_Arith.l_test(m_pSnHonpyoData->Sn_KURI4)>0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA)>0) ){
//---------------
			wsprintf( dp, "%3d", m_pSnHonpyoData->Sn_UP4 / 10 );
		}
		PF1[i].PITM[j++] = dp;
		dp += 4;

//-- '15.02.10 --
//		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI4 ) ){
//---------------
		if( (m_Arith.l_test(m_pSnHonpyoData->Sn_KURI4)>0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA)>0) ){
//---------------
			wsprintf( dp, "%01d", m_pSnHonpyoData->Sn_UP4 % 10 );
		}
		strcat_s( dp, 4+1, "  " );
		PF1[i].PITM[j++] = dp;
		dp += 4;

//-- '15.02.10 --
//		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI5) ){
//---------------
		if( (m_Arith.l_test(m_pSnHonpyoData->Sn_KURI5)>0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA)>0) ){
//---------------
			wsprintf( dp, "%3d", m_pSnHonpyoData->Sn_UP5 / 10 );
		}
		PF1[i].PITM[j++] = dp;
		dp += 4;

//-- '15.02.10 --
//		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI5) ){
//---------------
		if( (m_Arith.l_test(m_pSnHonpyoData->Sn_KURI5)>0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA)>0) ){
//---------------
			wsprintf( dp, "%01d", m_pSnHonpyoData->Sn_UP5 % 10 );
		}
		strcat_s( dp, 4+1, "  " );
//-- '15.02.26 --
//		PF1[i].PITM[j++] = dp;//[Y]
////		PF1[i++].PITM[j] = dp;
//---------------
		if( m_SixKindFlg == TRUE ){
			PF1[i].PITM[j++] = dp;
		}
		else{
			PF1[i++].PITM[j] = dp;
		}
//---------------
		dp += 4;

		//--->//[Y]
		if(m_SixKindFlg == TRUE){
			if( m_Arith.l_test(((CH27SnHonpyoData*)m_pSnHonpyoData)->Sn_KURI6) ){
				wsprintf( dp, "%3d", ((CH27SnHonpyoData*)m_pSnHonpyoData)->Sn_UP6 / 10 );
			}
			PF1[i].PITM[j++] = dp;
			dp += 4;

			if( m_Arith.l_test(((CH27SnHonpyoData*)m_pSnHonpyoData)->Sn_KURI6) ){
				wsprintf( dp, "%01d", ((CH27SnHonpyoData*)m_pSnHonpyoData)->Sn_UP6 % 10 );
			}
			strcat_s( dp, 4+1, "  " );
			PF1[i++].PITM[j] = dp;
			dp += 4;
		}
		//<----------------


		
//		PF1[i].PITM[0] = (toku ? "Åõ" : "Å@");			// ì¡ó·åvéZìKóp
		PF1[i].PITM[0] = (m_pSnHeadData->Sn_TOKUR ? "Åõ" : "Å@");			// ì¡ó·åvéZìKóp
//		PF1[i].PITM[1] = (toku ? "Å@" : "Åõ");
		PF1[i].PITM[1] = (m_pSnHeadData->Sn_TOKUR ? "Å@" : "Åõ");
	}

	// ê≈óùémñ@Å@é{çsí†ï[
	// ãåê≈ó¶ñºèÃ
	char	KZMY[6], SZMY[6] , SZMY2[6];

	memset( KZMY, '\0', sizeof( KZMY ) );	// â€ê≈ïWèÄäzÅ@ãåê≈ó¶
	memset( SZMY, '\0', sizeof( SZMY ) );	// è¡îÔê≈Å@Å@Å@ÅV
	memset( SZMY2, '\0', sizeof( SZMY2 ) );	// è¡îÔê≈Å@Å@Å@ÅV
	i++;
	if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
//Ç†Ç∆Ç≈ïúäà		m_Arith.l_add( KZMY, JANS[0].val, JANS[1].val );
//Ç†Ç∆Ç≈ïúäà		m_Arith.l_add( KZMY, KZMY, JANS[2].val );
		memmove( KZMY , m_pSnHonpyoData->Sn_TS_TDM1 , MONY_BUF_SIZE );
		m_Arith.l_mul( KZMY, KZMY, PW0 );
		memset( buff , '\0' , sizeof(buff));
		m_Arith.l_print( buff , KZMY , FMT16_EX );
		str.Format( _T("%s") , buff );
		str.TrimLeft();
		if( str.GetLength() > 12 ){
			FontFlg = 1;
		}

		if( m_Arith.l_test(KZMY) ){
			PF1[i].PITM[0] = "6+";	// "ÇUÅ{ÇSÅDÇTÅìï™";
			PF1[i].PITM[1] = "4.5Åìï™";
		}
		else{
			PF1[i].PITM[0] = (char *)0;
			PF1[i].PITM[1] = (char *)0;
		}
		i++;

//Ç†Ç∆Ç≈ïúäà		m_Arith.l_add( SZMY, JANS[3].val, JANS[4].val );
//Ç†Ç∆Ç≈ïúäà		m_Arith.l_add( SZMY, SZMY, JANS[5].val );
		memmove( SZMY , m_pSnHonpyoData->Sn_TS_SYT1 , MONY_BUF_SIZE );
		memset( buff , '\0' , sizeof(buff));
		m_Arith.l_print( buff , SZMY , FMT16_EX );
		str.Format( _T("%s") , buff );
		str.TrimLeft();
		if( str.GetLength() > 12 ){
			FontFlg = 1;
		}

		if( m_Arith.l_test(SZMY) ){
			PF1[i].PITM[0] = "6+";	// "ÇUÅ{ÇSÅDÇTÅìï™";
			PF1[i].PITM[1] = "4.5Åìï™";
		}
		else{
			PF1[i].PITM[0] = (char *)0;		
			PF1[i].PITM[1] = (char *)0;		
		}
		i++;
		memmove( SZMY2 , m_pSnHonpyoData->Sn_TS_SYT2 , MONY_BUF_SIZE );

//2015.08.04 INSERT START
		//ì¡íËâ€ê≈édì¸ÇÍÇ…åWÇÈï ï\ÇÃíÒèoóL
		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
			if( m_pSnHeadData->m_DispTabSgn&0x01 ){
				PF1[i].PITM[0] = ("Åõ");
			}else{
				PF1[i].PITM[0] = (char *)0;	
			}
			i++;
		}
//2015.08.04 INSERT END

//	if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
//2015.08.04 UPDATE START
		//m_Arith.l_print( dp, m_pSnHonpyoData->Sn_ZZURI, FMT16_EX );
		//PF1[i].PITM[0] = dp;
		//dp += 19;
		//i++;

		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
			FontFlg2 = 0;
			memset( buff , '\0' , sizeof(buff));
			memset( WORK0, '\0', sizeof(WORK0) );
//			m_Arith.l_print( buff , m_pSnHonpyoData->Sn_ZZURI , FMT110_EX );
			m_Arith.l_div( WORK0, m_pSnHonpyoData->Sn_ZZURI, PW0 );
			m_Arith.l_print( buff , WORK0 , FMT14 );
			str.Format( _T("%s") , buff );
			str.TrimLeft();
			if( str.GetLength() > 12 ){
				FontFlg2 = 1;
			}
			if( FontFlg2 == 0 ){
				memset( WORK0, '\0', sizeof(WORK0) );
//				m_Arith.l_print( dp, m_pSnHonpyoData->Sn_ZZURI, FMT16_EX );
				m_Arith.l_div( WORK0, m_pSnHonpyoData->Sn_ZZURI, PW0 );
				m_Arith.l_print( dp, WORK0, FMT14 );
				PF1[i].PITM[0] = dp;
				dp += 19;
			}else{ //è¨Ç≥Ç¢ÉtÉHÉìÉgÇÕå„Ç≈àÛéö
				PF1[i].PITM[0] = (char *)0;	
			}
		}else{
			m_Arith.l_print( dp, m_pSnHonpyoData->Sn_ZZURI, FMT16_EX );
			PF1[i].PITM[0] = dp;
			dp += 19;
		}
		i++;
//2015.08.04 UPDATE END

		// á@ãyÇ—áAÇÃì‡ñÛÅ@
		//â€ê≈ïWèÄäz
		if( !FontFlg ){//ïÅí ÇÃÉtÉHÉìÉgÇ≈èoÇ∑èÍçá
			int t = 0;
			for( j=0; j<4; j++, dp+=16 ){
				switch( j ) {
					case 0 :
						m_Arith.l_div( WORK0, JANS[0].val, PW0 );
						m_Arith.l_print( dp, WORK0, FMT15 );
	//					strcat_s( dp, 16+1, "    " );
						t = 0;
						break;
					case 1 :
						m_Arith.l_div( WORK0, JANS[1].val, PW0 );
						m_Arith.l_print( dp, WORK0, FMT15 );
	//					strcat_s( dp, 16+1, "    " );
						t = 1;
						break;
					case 2 :
						m_Arith.l_div( WORK0, JANS[2].val, PW0 );
						m_Arith.l_print( dp, WORK0, FMT15 );
	//					strcat_s( dp, 16+1, "    " );
						t = 2;
						break;
					case 3 ://Ç∆ÇËÇ†Ç¶Ç∏2Ç∆ìØÇ∂
						m_Arith.l_print( dp, KZMY, FMT15 );
	//					strcat_s( dp, 16+1, "    " );
						i++;
						t = 0;
						break;
					default:
						break;
				}
				PF1[i].PITM[t] = dp;
			}
			i++;
			//è¡îÔê≈äz
			for( j=0; j<4; j++, dp+=18 ){
				switch( j ) {
					case 0 :
						m_Arith.l_print( dp, JANS[3].val, FMT15_EX );
						break;
					case 1 :
						m_Arith.l_print( dp, JANS[4].val, FMT15_EX );
						break;
					case 2 :
						m_Arith.l_print( dp, JANS[5].val, FMT15_EX );
						break;
					case 3 ://Ç∆ÇËÇ†Ç¶Ç∏2Ç∆ìØÇ∂
						m_Arith.l_print( dp, SZMY, FMT15_EX );
						break;
					default:
						break;
				}
				PF1[i].PITM[j] = dp;
			}
			i++;
			//áPñîÇÕáQÇÃì‡ñÛ
			//ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
			for( j=0; j<3; j++, dp+=18 ){
				switch( j ) {
					case 0 :
						m_Arith.l_print( dp, JANS[6].val, FMT15_EX );
						break;
					case 1 :
						m_Arith.l_print( dp, JANS[7].val, FMT15_EX );
						break;
					case 2 :
						m_Arith.l_print( dp, SZMY2, FMT15_EX );
						break;
					default:
						break;
				}
				PF1[i].PITM[j] = dp;
			}
		}
		else{
			pbclr( 0, 3, &PF1[i++] );
			pbclr( 0, 1, &PF1[i++] );
			pbclr( 0, 4, &PF1[i++] );
			pbclr( 0, 3, &PF1[i] );
		}
	}
	else{
		if( !FontFlg ){//ïÅí ÇÃÉtÉHÉìÉgÇ≈èoÇ∑èÍçá
			//â€ê≈ïWèÄäz
			for( j=0; j<3; j++, dp+=16 ){
				switch( j ) {
					case 0 :
						m_Arith.l_div( WORK0, JANS[0].val, PW0 );
						m_Arith.l_print( dp, WORK0, FMT11 );
						strcat_s( dp, 16+1, "    " );
						break;
					case 1 :
						m_Arith.l_div( WORK0, JANS[1].val, PW0 );
						m_Arith.l_print( dp, WORK0, FMT11 );
						strcat_s( dp, 16+1, "    " );
						break;
					case 2 ://Ç∆ÇËÇ†Ç¶Ç∏1Ç∆ìØÇ∂
						m_Arith.l_div( WORK0, JANS[2].val, PW0 );
						m_Arith.l_print( dp, WORK0, FMT11 );
						strcat_s( dp, 16+1, "    " );
						break;
					default:
						break;
				}
				PF1[i].PITM[j] = dp;
			}
			i++;
			//è¡îÔê≈äz
			for( j=0; j<3; j++, dp+=16 ){
				switch( j ) {
					case 0 :
						m_Arith.l_print( dp, JANS[3].val, FMT15 );
						break;
					case 1 :
						m_Arith.l_print( dp, JANS[4].val, FMT15 );
						break;
					case 2 ://Ç∆ÇËÇ†Ç¶Ç∏1Ç∆ìØÇ∂
						m_Arith.l_print( dp, JANS[5].val, FMT15 );
						break;
					default:
						break;
				}
				PF1[i].PITM[j] = dp;
			}
			i++;
			//áPñîÇÕáQÇÃì‡ñÛ
			//ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
			for( j=0; j<2; j++, dp+=16 ){
				switch( j ) {
					case 0 :
						m_Arith.l_print( dp, JANS[6].val, FMT15 );
						break;
					case 1 :
						m_Arith.l_print( dp, JANS[7].val, FMT15 );
						break;
					default:
						break;
				}
				PF1[i].PITM[j] = dp;
			}
		}
		else{
			i++;
			i++;
		}
	}
	pbclr( 0, 2, &PF1[++i] );
	pbclr( 0, 1, &PF1[i+1] );
	pbclr( 0, 1, &PF1[i+2] );
	pbclr( 0, 1, &PF1[i+3] );
	pbclr( 0, 1, &PF1[i+4] );

	if( (m_Arith.l_test(ANS[25].val)< 0) || m_pPrintCmInfo->OPI ){	// ä“ïtã‡óZã@ä÷
		if( !m_pSnHeadData->Sn_BYsw ){			// ã‚çs
			memmove( dp, m_pSnHeadData->Sn_BANK, 10*2 );
			PF1[i].PITM[0] = dp;
			dp += 21;

			memmove( dp, m_pSnHeadData->Sn_SITEN, 10*2 );
			PF1[i++].PITM[1] = dp;
			dp += 21;
			
			memmove( dp, m_pSnHeadData->Sn_YOKIN, 4*2 );
			PF1[i++].PITM[0] = dp;	
			dp += 9;
			
			memset( buf, '\0', sizeof( buf ) );
			m_Arith.l_unpac( buf, m_pSnHeadData->Sn_KOZA, 10 );
			for( j=(int)strlen(buf); j>0; ){
				if( buf[--j] != 0x20 ){
					break;
				}
			}
			++j;

			if( j < 10 ){
				memmove( dp, "          ", 10-j );
			}
			// å˚ç¿î‘çÜ
			memmove( (dp+(10-j)), buf, j );	
			PF1[i++].PITM[0] = dp;
			dp += 11;
		}
		else{
			i += 3;
			memmove( dp, m_pSnHeadData->Sn_YUBIN, 8*2 );
			PF1[i++].PITM[0] = dp;
			dp += 17;
			
			memset( buf, '\0', sizeof( buf ) );
			memmove( buf, m_pSnHeadData->Sn_YUNUM1, 5 );
			j = (int)strlen( buf );
			memset( buf, '\0', sizeof( buf ) );
			memmove( buf, m_pSnHeadData->Sn_YUNUM2, 8 );
			k = (int)strlen( buf );
			if( j || k ){
				strcpy_s( buf, 8+1, "        " );											//?
				if( j ){
					memmove( &buf[0], m_pSnHeadData->Sn_YUNUM1, j );
				}
				if( k ){
					memmove( &buf[((m_pPrintCmInfo->PRtype&0x80) ? 7 :8)], m_pSnHeadData->Sn_YUNUM2, k );
				}
				dp = &buf[0];
			}
			PF1[i++].PITM[0] = dp;
		}
	}

	if( j = _xprn( PP, 3, PF1, 0, pDC, 0, SEno ) ){
		goto POUTn;
	}

	dp = AP;
	i = j = 0;
	memset( dp, '\0', size );
	pbclr( 0, 1, &PF1[i] );

	// êÆóùî‘çÜìôèoóÕÉTÉCÉì
	if( (m_pPrintCmInfo->PRtype&0x80) || !m_pPrintCmInfo->OP1 ){		
		if( (m_pSnHeadData->Sn_ZSEIRI[0]&0xff) != 0xff ) {
			char	tmpbuf[512] = {0};
			memset( tmpbuf, '\0', sizeof(tmpbuf) );
			if( (m_pSnHeadData->Sn_ZSEIRI[3]&0x0f) != 0x0f ){
				j = 8;
			}
			else{
				j = 7;
			}
			l_unpac( tmpbuf, m_pSnHeadData->Sn_ZSEIRI, j );
			if( j == 8 ){				// 8åÖì¸óÕÇÃèÍçá:êÊì™0ÇÕÉJÉbÉg
				if( tmpbuf[0] != 0x30 ){
					tmpbuf[7] = '\0';		// à»äOÇÕå„1åÖÉJÉbÉg
				}
				else{
					for( j = 0; j < 7; ++j ){
						tmpbuf[j] = tmpbuf[j+1];
					}
					tmpbuf[j] = '\0';
				}
				j = 7;
			}
			for( j = (int)strlen( tmpbuf ); j > 0; ){
				if( tmpbuf[--j] != 0x20 ){
					break;
				}
			}
			++j;

			// ê≈ñ±èêêÆóùî‘çÜ
			memset( buf, '\0', sizeof( buf ) );
			memmove( buf, "        ", 8 );
			memmove( &buf[7-j], tmpbuf, j );
			memmove( dp, &buf[0], 8 );
			PF1[i].PITM[0] = dp;
			dp += 9;
		}
	}
	
	// ê≈óùémñºèoóÕ
	for( ++i, j=0, k=0; j<2; ++j ){
		pbclr( 0, 1, &PF1[i+j] );
	}
	Zlng = Zsw = 0;
//-- '15.02.27 --
//	memset( Znam, '\0', 50 );
//	memset( Smei, '\0', 50 );
//---------------
	memset( Znam, '\0', sizeof(Znam) );
	memset( Smei, '\0', sizeof(Smei) );
//---------------

//--> '15.03.03 INS START
	int	SmlFontZrtype = 0;	// 0:í èÌ, 1:ëSäp20ï∂éöÇÊÇËëÂ
//<-- '15.03.03 INS END

	if( !m_pPrintCmInfo->OP2 ){
		if( (m_pPrintCmInfo->OPA==1) && (m_pPrintCmInfo->OPsign&0x01) ){
			// ê≈óùéméÅñºÇéËèëÇ´Ç∑ÇÈà◊Ç…ÅAéññ±èêñºÅïê≈óùémñ@êlñºÇèkè¨Ç≈àÛç¸
			// ÉtÉBÅ[ÉãÉh No.13Ç≈àÛç¸
			if( (j = (int)strlen(m_pShinInfo->pO_DATA->o_ofcnam)) > 40 ){
				j = 40;
			}
			memmove( Znam, m_pShinInfo->pO_DATA->o_ofcnam, j );
			Zlng = m_Util.MixSpaceCutLength( Znam, j );
			memmove( dp, "Å@", 2 );
			PF1[i].PITM[0] = dp;
			dp += (2 + 1);
		}
		else{
			if( m_pPrintCmInfo->OPA != 2 ){
				// ê≈óùéméÅñºñîÇÕê≈óùémñ@êlñº
//-- '15.02.27 --
//				if( m_pPrintCmInfo->OPA ){
//					// ê≈óùémñ@êlñº
//					if( (j = (int)strlen(m_pShinInfo->pO_DATA->o_ofcnam)) > 40 ){
//						j = 40;
//					}
//					memmove( Znam, m_pShinInfo->pO_DATA->o_ofcnam, j );
//				}
//				else{
//					// ê≈óùéméÅñº
//					if( (j = (int)strlen(m_pShinInfo->pO_DATA->o_zeinam )) > 40 ){
//						j = 40;
//					}
//					memmove( Znam, m_pShinInfo->pO_DATA->o_zeinam, j );
//				}
//---------------
				MakeOutZeirishiName( Znam, sizeof(Znam) );
				j = (int)strlen( Znam );
//---------------
				Zlng = m_Util.MixSpaceCutLength( Znam, j );

				if( Zlng > 40 ){
					memmove( dp, "Å@", 2 );
					PF1[i].PITM[0] = dp;
					dp += (2 + 1);
					SmlFontZrtype = 1;
				}
				else if( Zlng > 24 ){
					memmove( dp, Znam, Zlng );
					PF1[i].PITM[0] = dp;
					dp += (j + 1);
				}
				else{					// ÇPÇQï∂éöà»â∫ÇÕëSäpÇ≈ÉtÉBÅ[ÉãÉh No.11Ç≈àÛç¸
					memmove( dp, "Å@", 2 );
					PF1[i].PITM[0] = dp;
					dp += (2 + 1);
					Zsw = 1;
				}
			}
		}
	}

	// ê≈óùémìdòbî‘çÜ
	if( !m_pPrintCmInfo->OP9 ){
		for( j=0; j<19; ++j ){
			buf[j] = 0x20;
		}
		buf[j] = '\0';
		for( ++i, j=k=0; (j<30)||m_pShinInfo->pO_DATA->o_telno[j]; ++j ){
			if( (m_pShinInfo->pO_DATA->o_telno[j]=='-') || (m_pShinInfo->pO_DATA->o_telno[j]=='∞') ||
				(m_pShinInfo->pO_DATA->o_telno[j]=='(') || (m_pShinInfo->pO_DATA->o_telno[j]==')') ){
				switch( k ){
					case 0 :
						k = ( (j < 6) ? j : 6);
						memmove( &buf[6-k], &m_pShinInfo->pO_DATA->o_telno[0], k );
						k = 1;	h = j + 1;
						continue;
					case 1 :
						if( (j-h) < 5 ){
							k = (j - h);
						}
						else{
							k = 5;
						}
						memmove( &buf[8],  &m_pShinInfo->pO_DATA->o_telno[h],   k );
						memmove( &buf[14], &m_pShinInfo->pO_DATA->o_telno[++j], 5 );
						k = 2;
						break;
					default:
						break;
				}
				if( k > 1 ){
					break;
				}
			}
		}
		if( !k ){
			memmove( buf, &m_pShinInfo->pO_DATA->o_telno[0], (j > 19 ? 19 : j) );
		}
		else{
			if( k == 1 ){
				memmove( &buf[8],  &m_pShinInfo->pO_DATA->o_telno[h], 4 );
			}
		}
		memmove( dp, buf, 19 );
		
		PF1[i].PITM[0] = dp;
		dp += 20;
	}
	
	if( j = _xprn( PP, 4, PF1, 0, pDC, 0, SEno ) ){
		goto POUTn;
	}

	// ä“ïtÇéÛÇØÇÊÇ§Ç∆Ç∑ÇÈã‡óZä˙ä‘	
	dp = AP;
	i = j = 0;
	memset( dp, '\0', size );

	pbclr( 0, 1, &PF1[ i ] );
	pbclr( 0, 1, &PF1[i+1] );
	pbclr( 0, 1, &PF1[i+2] );
	pbclr( 0, 1, &PF1[i+3] );
	pbclr( 0, 1, &PF1[i+4] );
	pbclr( 0, 1, &PF1[i+5] );
	pbclr( 0, 1, &PF1[i+6] );
	pbclr( 0, 1, &PF1[i+7] );
	pbclr( 0, 1, &PF1[i+8] );
	pbclr( 0, 1, &PF1[i+9] );

	if( (m_Arith.l_test(ANS[25].val) < 0) || m_pPrintCmInfo->OPI ){	// ä“ïtã‡óZã@ä÷
		if( !m_pSnHeadData->Sn_BYsw ){			// ã‚çs
			switch( m_pSnHeadData->Sn_FINANCE&0x0f ){
				case 0 :
					PF1[ i ].PITM[0] = "Åù";
					break;
				case 1 :
					PF1[i+1].PITM[0] = "Åù";
					break;
				case 2 :
					PF1[i+2].PITM[0] = "Åù";
					break;
				case 3 :
					PF1[i+3].PITM[0] = "Åù";
					break;
				case 4 :	
					PF1[i+4].PITM[0] = "Åù";
					break;
				default:
					break;
			}

			switch( m_pSnHeadData->Sn_CHAIN & 0x0f ){
				case 0 :
					PF1[i+5].PITM[0] = "Åù";
					break;
				case 1 :
					PF1[i+6].PITM[0] = "Åù";
					break;
				case 2 :	
					PF1[i+7].PITM[0] = "Åù";
					break;
				case 3 :
					PF1[i+8].PITM[0] = "Åù";
					break;
				case 5 :
					PF1[i+9].PITM[0] = "Åù";
					break;
				default:
					break;
			}
		}
	}

	if( j = _xprn( PP, 5, PF1, 0, pDC, 0, SEno ) ){
		goto POUTn;
	}

	// óìäOàÛéöçÄñ⁄èoóÕ
	dp = AP;
	i = j = 0;
	memset( dp, '\0', size );

	pbclr( 0, 1, &PF1[ i ] );
	pbclr( 0, 1, &PF1[i+1] );
	pbclr( 0, 1, &PF1[i+2] );
	pbclr( 0, 1, &PF1[i+3] );

	pbclr( 0, 1, &PF1[i+4] );		// å¬êl	â∫íiá@
	pbclr( 0, 1, &PF1[i+5] );		// ÅV	å¬êl
	pbclr( 0, 1, &PF1[i+6] );		// ÅV	è„íi
	pbclr( 0, 1, &PF1[i+7] );		// ÅV	â∫íi
	pbclr( 0, 1, &PF1[i+8] );		// ÅV	ê≈ñ±èêèàóùóìÅ@ãÊï™ÇR
	// ägí£		
	pbclr( 0, 1, &PF1[i+9] );		// ñ@êl	â∫íi
	pbclr( 0, 1, &PF1[i+10] );		// ÅVÅ@	èêï àÍòAî‘çÜ
	// èää«
	pbclr( 0, 1, &PF1[i+11] );

	// óìäOâ∫ägí£
	char	ADRDX[128], ADRD2[20], ADRD4[50];
	int		ln2, ln4;
	memset( ADRDX, '\0', sizeof( ADRDX ) );
	memset( ADRDX, 0x20, 60 );
	memset( ADRD2, '\0', sizeof( ADRD2 ) );
	memmove( ADRD2, m_pSnHeadData->Sn_ADRD2, 16 );
	memset( ADRD4, '\0', sizeof( ADRD4 ) );
	memmove( ADRD4, m_pSnHeadData->Sn_ADRD4, 44 );;
	ln2 = (int)strlen( ADRD2 );
	ln4 = (int)strlen( ADRD4 );
	memmove( ADRDX, ADRD4, ln4 );
	memmove( &ADRDX[44], ADRD2, ln2 );

	if( !m_pPrintCmInfo->OP7 && (m_pSnHeadData->IsSoleProprietor()==FALSE) ){
		memmove( dp, m_pSnHeadData->Sn_ADRU1, 30 );
		PF1[ i ].PITM[0] = dp;
		dp += 32;
		
		memmove( dp, m_pSnHeadData->Sn_ADRU2, 20 );
		PF1[i+1].PITM[0] = dp;
		dp += 22;

		memmove( dp, m_pSnHeadData->Sn_ADRC1, 20 );
		PF1[i+2].PITM[0] = dp;	
		dp += 22;

		memmove( dp, m_pSnHeadData->Sn_ADRC2, 20 );
		PF1[i+3].PITM[0] = dp;
		dp += 22;

		if( m_pSnHeadData->IsSoleProprietor() ){
			memmove( dp, ADRDX, 60 );
			PF1[i+4].PITM[0] = dp;
			dp += 62;

			if( m_pSnHeadData->Sn_PPTYP&0x01 )	{
				memmove( dp, "å¬êl", 4 );
				PF1[i+5].PITM[0] = dp;
				dp += 6;
			}
			memmove( dp, m_pSnHeadData->Sn_ADKUP, 10 );
			PF1[i+6].PITM[0] = dp;
			dp += 12;

			memmove( dp, m_pSnHeadData->Sn_ADKDW, 10 );
			PF1[i+7].PITM[0] = dp;	
			dp += 12;

			memmove( dp, m_pSnHeadData->Sn_ADKNO, 1 );
			PF1[i+8].PITM[0] = dp;	
			dp += 2;
		}
		else{
			memmove( dp, ADRDX, 60 );
			PF1[i+9].PITM[0] = dp;
			dp += 62;
			memmove( dp, m_pSnHeadData->Sn_ADSNO, 7 );
			PF1[i+10].PITM[0] = dp;	
			dp += 9;
		}
	}

	if( (m_pPrintCmInfo->PRtype&0x80) || !m_pPrintCmInfo->OP1 ){		// êÆóùî‘çÜìôèoóÕÉTÉCÉì
		memmove( dp, m_pSnHeadData->Sn_SYOKN, 3 );
		PF1[i+11].PITM[0] = dp;	
		dp += 5;
	}

	if( j = _xprn( PP, 9, PF1, 0, pDC, 0, SEno ) ){
		goto POUTn;
	}

	// ê≈óùémñ@Å@é{çså„í†ï[
	dp = AP;
	i = j = 0;
	memset( dp, '\0', size );
	pbclr( 0, 1, &PF1[ 0 ] );
	pbclr( 0, 1, &PF1[ 1 ] );

	if( m_pSnHeadData->Sn_ZeiHou&0x01 ){
		PF1[0].PITM[0] = "Åõ";
	}
	else{
		PF1[0].PITM[0] = (char *)0;
	}

	if( m_pSnHeadData->Sn_ZeiHou&0x02 ){
		PF1[1].PITM[0] = "Åõ";
	}
	else{
		PF1[1].PITM[0] = (char *)0;
	}
	// å¬êlâ¸ê≥í†ï[ëŒâû [05'01.06]
	if( m_pSnHeadData->IsSoleProprietor() ){		// å¬êl
		// àÍòAî‘çÜ
		if( (m_pSnHeadData->Sn_ITIREN[0]&0xff) != 0xff ){
			memset( buf, '\0', sizeof( buf ) );
			m_Arith.l_unpac( buf, m_pSnHeadData->Sn_ITIREN, 8 );
			for( j=(int)strlen(buf); j>0; ){
				if( buf[--j] != 0x20 ){
					break;
				}
			}
			++j;
			if( j < 8 ){
				memmove( dp, "        ", 8-j );
			}
			memmove( (dp+(8-j)), buf, j );
			PF1[2].PITM[0] = dp;
			dp += 9;
		}
		else{
			PF1[2].PITM[0] = (char *)0;
		}

		// ëóït
		if( (m_pSnHeadData->Sn_ZeiHou & 0x04) ){
			PF1[3].PITM[0] = "Åõ";	
		}
		else{
			PF1[3].PITM[0] = (char *)0;
		}
	}
	else{
		PF1[2].PITM[0] = (char *)0;

		// ëóït
		if( (m_pSnHeadData->Sn_ZeiHou & 0x04) ){
			PF1[3].PITM[0] = "Åõ";	
		}
		else{
			PF1[3].PITM[0] = (char *)0;
		}
	}

	if( j = _xprn( PP, 10, PF1, 0, pDC, 0, SEno ) ){
		goto POUTn;
	}

	if( !m_pPrintCmInfo->OP2 ){
		if( (m_pPrintCmInfo->OPA==1) && (m_pPrintCmInfo->OPsign&0x01) ){
			// éññ±èêñºÅïê≈óùémñ@êlñºÇèkè¨Ç≈àÛç¸
			// ê≈óùémñ@ é{çså„
			pbclr( 0, 1, &PF1[0] );
			if( j = _xprn( PP, 11, PF1, 0, pDC, 0, SEno ) ){
				goto POUTn;
			}
			// ê≈óùémñ@Å@é{çsëO
			pbclr( 0, 1, &PF1[0] );
			if( j = _xprn( PP, 12, PF1, 0, pDC, 0, SEno ) ){
				goto POUTn;
			}
			// ê≈óùémñ@ é{çså„
			pbclr( 0, 1, &PF1[0] );
			memmove( dp, Znam, Zlng );
			PF1[0].PITM[0] = dp;
			dp += (Zlng + 1);
			if( j = _xprn( PP, 13, PF1, 0, pDC, 0, SEno ) ){
				goto POUTn;
			}
			// ê≈óùémñ@Å@é{çsëO
			pbclr( 0, 1, &PF1[0] );
			if( j = _xprn( PP, 14, PF1, 0, pDC, 0, SEno ) ){
				goto POUTn;
			}
			// ê≈óùémñ@Å@é{çså„ í«â¡ÅI
			pbclr( 0, 1, &PF1[0] );
			if( j = _xprn( PP, 15, PF1, 0, pDC, 0, SEno ) ){
				goto POUTn;
			}
		}
		else{
			if( m_pPrintCmInfo->OPA == 2 ){
				// éññ±èêñºÅïê≈óùémñ@êlñºÇèkè¨Ç≈àÛç¸
				// ê≈óùémñ@ é{çså„
				pbclr( 0, 1, &PF1[0] );
				if( j = _xprn( PP, 11, PF1, 0, pDC, 0, SEno ) ){
					goto POUTn;
				}
				// ê≈óùémñ@Å@é{çsëO
				pbclr( 0, 1, &PF1[0] );
				if( j = _xprn( PP, 12, PF1, 0, pDC, 0, SEno ) ){
					goto POUTn;
				}
				// ê≈óùémñ@ é{çså„
				if( (Zlng = (int)strlen( m_pShinInfo->pO_DATA->o_ofcnam )) > 40 ){
					Zlng = 40;
				}
				memmove( Znam, m_pShinInfo->pO_DATA->o_ofcnam, Zlng );
				pbclr( 0, 1, &PF1[0] );
				memmove( dp, Znam, Zlng );
				PF1[0].PITM[0] = dp;
				dp += (Zlng + 1);
				if( j = _xprn( PP, 13, PF1, 0, pDC, 0, SEno ) ){
					goto POUTn;
				}
				// ê≈óùémñ@Å@é{çsëO
				pbclr( 0, 1, &PF1[0] );
				if( j = _xprn( PP, 14, PF1, 0, pDC, 0, SEno ) ){
					goto POUTn;
				}
				// ê≈óùémñ@Å@é{çså„Å@ê≈óùéméÅñº
//-- '15.02.27 --
//				if( (Zlng = (int)strlen( m_pShinInfo->pO_DATA->o_zeinam )) > 40 ){
//					Zlng = 40;
//				}
//				memmove( Smei, m_pShinInfo->pO_DATA->o_zeinam, Zlng );
//----------------
				MakeOutZeirishiName( Smei, sizeof(Smei) );
				Zlng = (int)strlen(Smei);
//----------------
//-- '15.03.03 --
//				pbclr( 0, 1, &PF1[0] );
//				memmove( dp, Smei, Zlng );
//				PF1[0].PITM[0] = dp;
//				dp += (Zlng + 1);
//---------------
				if( Zlng > 40 ){
					memset( Znam, '\0', sizeof(Znam) );
					memmove( Znam, Smei, Zlng );
					pbclr( 0, 1, &PF1[0] );
					SmlFontZrtype = 2;
				}
				else{
					pbclr( 0, 1, &PF1[0] );
					memmove( dp, Smei, Zlng );
					PF1[0].PITM[0] = dp;
					dp += (Zlng + 1);
				}
//---------------
				if( j = _xprn( PP, 15, PF1, 0, pDC, 0, SEno ) ){
					goto POUTn;
				}
			}
			else{
				if( Zsw ){
					// ê≈óùémñ@ é{çså„
					pbclr( 0, 1, &PF1[0] );
					memmove( dp, Znam, Zlng );
					PF1[0].PITM[0] = dp;
					dp += (Zlng + 1);
					if( j = _xprn( PP, 11, PF1, 0, pDC, 0, SEno ) ){
						goto POUTn;
					}
					// ê≈óùémñ@Å@é{çsëO
					pbclr( 0, 1, &PF1[0] );
					if( j = _xprn( PP, 12, PF1, 0, pDC, 0, SEno ) ){
						goto POUTn;
					}
					// ê≈óùémñ@ é{çså„
					pbclr( 0, 1, &PF1[0] );
					if( j = _xprn( PP, 13, PF1, 0, pDC, 0, SEno ) ){
						goto POUTn;
					}
					// ê≈óùémñ@Å@é{çsëO
					pbclr( 0, 1, &PF1[0] );
					if( j = _xprn( PP, 14, PF1, 0, pDC, 0, SEno ) ){
						goto POUTn;
					}
					// ê≈óùémñ@Å@é{çså„ í«â¡ÅI
					pbclr( 0, 1, &PF1[0] );
					if( j = _xprn( PP, 15, PF1, 0, pDC, 0, SEno ) ){
						goto POUTn;
					}
				}
			}
		}
	}

	if( m_pPrintCmInfo->OPM ){
		dp = AP;
		i = j = 0;
		memset( dp, '\0', size );
		pbclr( 0, 1, &PF1[ 0 ] );
		pbclr( 0, 1, &PF1[ 1 ] );
	
		// ñ@êlñº
		if( !m_pPrintCmInfo->OP4 ){
			memmove( dp, m_pSnHeadData->Sn_CONAM, 20*2 );
			PF1[0].PITM[0] = dp;
			dp += 41;
		}
		memmove( dp, m_pSnHeadData->Sn_GPNAM, 20*2 );
		PF1[1].PITM[0] = dp;
		dp += 41;
		if( j = _xprn( PP, 16, PF1, 0, pDC, 0, SEno ) ){
			goto POUTn;
		}
	}

	i = 0;
	if( FontFlg ){//è¨Ç≥Ç¢ÉtÉHÉìÉgÇ≈èoÇ∑èÍçá
		// á@ãyÇ—áAÇÃì‡ñÛÅ@
		//â€ê≈ïWèÄäz
		int t = 0;
		if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
			for( j=0; j<4; j++, dp+=16 ){
				switch( j ) {
					case 0 :
						m_Arith.l_div( WORK0, JANS[0].val, PW0 );
						m_Arith.l_print( dp, WORK0, FMT15 );
	//					strcat_s( dp, 16+1, "    " );
						t = 0;
						break;
					case 1 :
						m_Arith.l_div( WORK0, JANS[1].val, PW0 );
						m_Arith.l_print( dp, WORK0, FMT15 );
	//					strcat_s( dp, 16+1, "    " );
						t = 1;
						break;
					case 2 :
						m_Arith.l_div( WORK0, JANS[2].val, PW0 );
						m_Arith.l_print( dp, WORK0, FMT15 );
	//					strcat_s( dp, 16+1, "    " );
						t = 2;
						break;
					case 3 ://Ç∆ÇËÇ†Ç¶Ç∏2Ç∆ìØÇ∂
						m_Arith.l_print( dp, KZMY, FMT15 );
	//					strcat_s( dp, 16+1, "    " );
						i++;
						t = 0;
						break;
					default:
						break;
				}
				PF1[i].PITM[t] = dp;
			}
			i++;
			//è¡îÔê≈äz
			for( j=0; j<4; j++, dp+=18 ){
				switch( j ) {
					case 0 :
						m_Arith.l_print( dp, JANS[3].val, FMT15_EX );
						break;
					case 1 :
						m_Arith.l_print( dp, JANS[4].val, FMT15_EX );
						break;
					case 2 :
						m_Arith.l_print( dp, JANS[5].val, FMT15_EX );
						break;
					case 3 ://Ç∆ÇËÇ†Ç¶Ç∏2Ç∆ìØÇ∂
						m_Arith.l_print( dp, SZMY, FMT15_EX );
						break;
					default:
						break;
				}
				PF1[i].PITM[j] = dp;
			}
			i++;
			//áPñîÇÕáQÇÃì‡ñÛ
			//ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
			for( j=0; j<3; j++, dp+=18 ){
				switch( j ) {
					case 0 :
						m_Arith.l_print( dp, JANS[6].val, FMT15_EX );
						break;
					case 1 :
						m_Arith.l_print( dp, JANS[7].val, FMT15_EX );
						break;
					case 2 :
						m_Arith.l_print( dp, SZMY2, FMT15_EX );
						break;
					default:
						break;
				}
				PF1[i].PITM[j] = dp;
			}
		}
		else{
			//â€ê≈ïWèÄäz
			for( j=0; j<3; j++, dp+=16 ){
				switch( j ) {
					case 0 :
						m_Arith.l_div( WORK0, JANS[0].val, PW0 );
						m_Arith.l_print( dp, WORK0, FMT11 );
						strcat_s( dp, 16+1, "    " );
						break;
					case 1 :
						m_Arith.l_div( WORK0, JANS[1].val, PW0 );
						m_Arith.l_print( dp, WORK0, FMT11 );
						strcat_s( dp, 16+1, "    " );
						break;
					case 2 ://Ç∆ÇËÇ†Ç¶Ç∏1Ç∆ìØÇ∂
						m_Arith.l_div( WORK0, JANS[2].val, PW0 );
						m_Arith.l_print( dp, WORK0, FMT11 );
						strcat_s( dp, 16+1, "    " );
						break;
					default:
						break;
				}
				PF1[i].PITM[j] = dp;
			}
			i++;
			//è¡îÔê≈äz
			for( j=0; j<3; j++, dp+=16 ){
				switch( j ) {
					case 0 :
						m_Arith.l_print( dp, JANS[3].val, FMT15 );
						break;
					case 1 :
						m_Arith.l_print( dp, JANS[4].val, FMT15 );
						break;
					case 2 ://Ç∆ÇËÇ†Ç¶Ç∏1Ç∆ìØÇ∂
						m_Arith.l_print( dp, JANS[5].val, FMT15 );
						break;
					default:
						break;
				}
				PF1[i].PITM[j] = dp;
			}
			i++;
			//áPñîÇÕáQÇÃì‡ñÛ
			//ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
			for( j=0; j<2; j++, dp+=16 ){
				switch( j ) {
					case 0 :
						m_Arith.l_print( dp, JANS[6].val, FMT15 );
						break;
					case 1 :
						m_Arith.l_print( dp, JANS[7].val, FMT15 );
						break;
					default:
						break;
				}
				PF1[i].PITM[j] = dp;
			}
		}
		if( j = _xprn( PP, 17, PF1, 0, pDC, 0, SEno ) ){
			goto POUTn;
		}
	}

//--> '15.03.03 INS START
	// ê≈óùémñºÇ≈ç≈è¨ÉtÉHÉìÉg
	dp = AP;
	memset( dp, '\0', size );
	i = j = 0;
	if( SmlFontZrtype == 1 ){
		memmove( dp, Znam, Zlng );
		PF1[i].PITM[0] = dp;
		dp += (Zlng + 1);
	}
	else{
		pbclr( 0, 1, &PF1[0] );
	}
	if( j = _xprn( PP, 18, PF1, 0, pDC, 0, SEno ) ){
		goto POUTn;
	}
	// éññ±èäñºÅ{ê≈óùémñºÇ≈ç≈è¨ÉtÉHÉìÉg
	dp = AP;
	memset( dp, '\0', size );
	i = j = 0;
	if( SmlFontZrtype == 2 ){
		memmove( dp, Znam, Zlng );
		PF1[i].PITM[0] = dp;
		dp += (Zlng + 1);
	}
	else{
		pbclr( 0, 1, &PF1[0] );
	}
	if( j = _xprn( PP, 19, PF1, 0, pDC, 0, SEno ) ){
		goto POUTn;
	}
//<-- '15.03.03 INS END

//2015.08.04 INSERT START
	//äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇÇè¨Ç≥Ç¢ÉtÉHÉìÉgÇ≈
	if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
		if( FontFlg2 == 1 ){
			dp = AP;
			memset( dp, '\0', size );
			memset( WORK0, '\0', sizeof(WORK0) );
//			m_Arith.l_print( dp, m_pSnHonpyoData->Sn_ZZURI, FMT16_EX );
			m_Arith.l_div( WORK0, m_pSnHonpyoData->Sn_ZZURI, PW0 );
			m_Arith.l_print( dp, WORK0, FMT14 );
			PF1[0].PITM[0] = dp;
			dp += 19;
			if( j = _xprn( PP, 20, PF1, 0, pDC, 0, SEno ) ){
				goto POUTn;
			}
		}
	}
//2015.08.04 INSERT END

//2016.02.23 INSERT START
	//dp = AP;
	//i = j = 0;
	//memset( dp, '\0', size );

	//pbclr( 0, 1, &PF1[ i ] );

	//if( m_pSnHeadData->Sn_SPECIAL&0x04 ){
	//	if( !m_pPrintCmInfo->OPT && (m_pSnHeadData->IsSoleProprietor()==TRUE) ){

	//		//char memo_tmp[183];
	//		//::ZeroMemory( memo_tmp, sizeof(memo_tmp));
	//		//memmove( memo_tmp, m_pSnHeadData->Sn_MEMO, sizeof(memo_tmp));
	//		memset( buf, '\0', sizeof( buf ) );
	//		memmove( buf, m_pSnHeadData->Sn_MEMO, 186 );

	//		str.Empty();
	//		//str.Format((char)m_pSnHeadData->Sn_MEMO);
	//		str.Format( _T("%s") , buf );
	//		if( m_pSnHeadData->Sn_KAIGYO != 0x00 ){
	//			str.Replace("\n","Å@");
	//			memmove( dp, str, 186 );
	//			PF1[0].PITM[0] = dp;
	//			dp += 187;
	//		}else{
	//			str.Replace("\n","");
	//			memmove( dp, str, 186 );
	//			PF1[0].PITM[0] = dp;
	//			dp += 187;
	//		}
	//	}
	//}

	//int len = str.GetLength();
	//if( len <= 101 ){
	//	if( j = _xprn( PP, 21, PF1, 0, pDC, 0, SEno ) ){ //ÉtÉHÉìÉgëÂ
	//		goto POUTn;
	//	}
	//}else if(( 101 < len )&&( len <= 138)){
	//	if( j = _xprn( PP, 22, PF1, 0, pDC, 0, SEno ) ){ //ÉtÉHÉìÉgíÜ
	//		goto POUTn;
	//	}
	//}else{
	//	if( j = _xprn( PP, 23, PF1, 0, pDC, 0, SEno ) ){ //ÉtÉHÉìÉgè¨
	//		goto POUTn;
	//	}
	//}

//2016.02.23 INSERT END

//2015.12.03 INSERT START
	if(( m_ShRevType >= ID_ICSSH_MYNUMBER_REVTYPE ) && m_pSnHeadData->IsSoleProprietor()){
		if( isHikae == TRUE ){
//			m_pVprn->DrawSline( 0, LKnrW0, 3100, 5530, 9880, 4930 ); //å¬êlî‘çÜóìÇ…éŒê¸
			m_pVprn->DrawSline( 0, LKnrW0, 3150, 5360, 9750, 4850 ); //å¬êlî‘çÜóìÇ…éŒê¸			
		}
	}
//2015.12.03 INSERT END


	// ìdéqê\çêäÆóπçœ 
   if(*m_pShinInfo->pFinish && *m_pShinInfo->pFinishSw && strlen( m_pShinInfo->pFinishPrn) && isHikae ){
   		int yPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[0].HyoTn]._PIVad;
		int xPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[0].HyoTn]._PIHad;
		long	PH  = 2540L / 10L;		// 1 character
		long	PV1 = 2540L / 6L;		// 1/6 LF
  		long YPosition = ( PV1 * 1L ) + (PV1 / 4) + yPlus;
		long XPosition = ( PH *  4L ) + xPlus;
		int	Gothic095 = m_pVprn->RegisterFont( 90, 0, 0, "ÇlÇrÅ@ÉSÉVÉbÉN" );
		int	Gothic108 = m_pVprn->RegisterFont( MAKELONG( 108, 1 ), 0, 0, "ÇlÇrÅ@ÉSÉVÉbÉN" );
		struct	_SF_PAR		SfPar;
		memset( (char *)&SfPar, '\0', sizeof( struct _SF_PAR ) );
		SfPar.sf_fcsg =	( SFp_xl | SFp_yb );					// ÉtÉ@ÉìÉNÉVÉáÉìÉTÉCÉì
		SfPar.sf_hvsg = 0;										// èoóÕï˚å¸
		SfPar.sf_xfld = (11L * PH);								// â°èoóÕóÃàÊïù
		SfPar.sf_yfld =	PV1;									// ècèoóÕóÃàÊïù
		SfPar.sf_mrgn[0] = 0;									// óÃàÊì‡É}Å[ÉWÉìÅ@ç∂
		SfPar.sf_mrgn[1] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@âE
		SfPar.sf_mrgn[2] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@è„
		SfPar.sf_mrgn[3] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@â∫
		SfPar.sf_kpit = 0;										// ï∂éöÉsÉbÉ`ïùÅ@X0.1Œﬂ≤›ƒ
		SfPar.sf_ksiz =	Gothic108;								// ï∂éöÉTÉCÉYÅ@Å@X0.1Œﬂ≤›ƒ		
		SfPar.sf_erto = 0;										// ï∂éöägëÂèkè¨ó¶
		if( m_pVprn->DrawString( 0, XPosition, YPosition, _T("ìdéqê\çêçœ"), &SfPar ) ){
			goto POUTn;
		}

  		YPosition = PV1 + (PV1 / 4) + yPlus;
		XPosition = ( PH *  12L ) + xPlus;
		memset( (char *)&SfPar, '\0', sizeof( struct _SF_PAR ) );
		SfPar.sf_fcsg =	( SFp_xl | SFp_yb );					// ÉtÉ@ÉìÉNÉVÉáÉìÉTÉCÉì
		SfPar.sf_hvsg = 0;										// èoóÕï˚å¸
		SfPar.sf_xfld = (35L * PH);								// â°èoóÕóÃàÊïù
		SfPar.sf_yfld =	PV1;									// ècèoóÕóÃàÊïù
		SfPar.sf_mrgn[0] = 0;									// óÃàÊì‡É}Å[ÉWÉìÅ@ç∂
		SfPar.sf_mrgn[1] = 10;									// Å@Å@ÅVÅ@Å@Å@Å@Å@âE
		SfPar.sf_mrgn[2] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@è„
		SfPar.sf_mrgn[3] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@â∫
		SfPar.sf_kpit = 0;										// ï∂éöÉsÉbÉ`ïùÅ@X0.1Œﬂ≤›ƒ
		SfPar.sf_ksiz =	Gothic095;								// ï∂éöÉTÉCÉYÅ@Å@X0.1Œﬂ≤›ƒ		
		SfPar.sf_erto = 0;										// ï∂éöägëÂèkè¨ó¶
		if( m_pVprn->DrawString( 0, XPosition, YPosition, m_pShinInfo->pFinishPrn, &SfPar ) ){
			goto POUTn;
		}

		m_pVprn->DeleteRegFont( Gothic095 );
		m_pVprn->DeleteRegFont( Gothic108 );

   }

   // çTóp
   if( isHikae && m_pPrintCmInfo->OPD ){
		int yPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[0].HyoTn]._PIVad;
		int xPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[0].HyoTn]._PIHad;
		long	PH  = 2540L / 10L ;		// 1 character
		long	PV1 = 2540L / 6L;		// 1/6 LF
  		long YPosition = (PV1*2)+(PV1/2) + yPlus;
		long XPosition = (PH*56L)+(PH/3) + xPlus;
		int	Gothic108 = m_pVprn->RegisterFont( MAKELONG( 108, 1 ), 0, 0, "ÇlÇrÅ@ÉSÉVÉbÉN" );
		struct	_SF_PAR		SfPar;
		memset( (char *)&SfPar, '\0', sizeof( struct _SF_PAR ) );
		SfPar.sf_fcsg =	( SFp_xl | SFp_yb | SFp_yc );			// ÉtÉ@ÉìÉNÉVÉáÉìÉTÉCÉì
		SfPar.sf_hvsg = 0;										// èoóÕï˚å¸
		SfPar.sf_xfld = (8L * PH);								// â°èoóÕóÃàÊïù
		SfPar.sf_yfld =	PV1;									// ècèoóÕóÃàÊïù
		SfPar.sf_mrgn[0] = 0;									// óÃàÊì‡É}Å[ÉWÉìÅ@ç∂
		SfPar.sf_mrgn[1] = 10;									// Å@Å@ÅVÅ@Å@Å@Å@Å@âE
		SfPar.sf_mrgn[2] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@è„
		SfPar.sf_mrgn[3] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@â∫
		SfPar.sf_kpit = 0;										// ï∂éöÉsÉbÉ`ïùÅ@X0.1Œﬂ≤›ƒ
		SfPar.sf_ksiz =	Gothic108;								// ï∂éöÉTÉCÉYÅ@Å@X0.1Œﬂ≤›ƒ		
		SfPar.sf_erto = 0;										// ï∂éöägëÂèkè¨ó¶
		if( m_pVprn->DrawString( 0, XPosition, YPosition, _T("ÅyçTópÅz"), &SfPar ) ){
			goto POUTn;
		}

		m_pVprn->DeleteRegFont( Gothic108 );
	}

POUTn:
   if( AP ){
		delete [] AP;
		AP = NULL;
   }

	return j;
}
#endif

//-----------------------------------------------------------------------------
// ñ{ï\àÛç¸
//-----------------------------------------------------------------------------
// à¯êî	pDC		ÅF	ÉfÉoÉCÉXÉRÉìÉeÉLÉXÉg
//		isHikae	ÅF	çTÇ¶àÛç¸Ç©Ç«Ç§Ç©
//		PPno	ÅF
//		fno		ÅF	
//-----------------------------------------------------------------------------
// ï‘ëóíl	
//-----------------------------------------------------------------------------
int CH26HyoView::PrintHonPyo( CDC *pDC, BOOL isHikae, int PPno, int fno )
{
	int				i, j, k, h;
	char			*AP, *dp, buf[512];
	unsigned short	SEno;
	// ê≈óùémñ@êl
	int				Zlng, Zsw;
	char			Znam[128]={0}, Smei[128]={0};
	CString str = _T("");
	BOOL			isNoImage = FALSE;

	SEno = fno;
	if( m_pPrintCmInfo->PRtype&&0x80 ){
		isNoImage = FALSE;
	}
	else{
		isNoImage = TRUE;
	}

	unsigned int	size=2048;
	if( (AP = new char[size]) == NULL ){
		return -1;
	}
	dp = AP;
	memset( dp, '\0', size );

	PBUF	PF1[30];
	int		PP = PPno;

	char	PW0[6], PW1[6];
	memset( PW0, '\0', 6 );
	m_Arith.l_input( PW0, _T("1000") );
	memset( PW1, '\0', 6 );
	m_Arith.l_input( PW1, _T("100") );

	// íÒèoîNåéì˙
	char	yymmdd[3] = {0};
	if( IsJcContinuePrint(true) ){
		m_pSnHeadData->GetYmdData(ID_ICSSH_PRESENT_DATE, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	}
	else{
		m_pSnHeadData->GetYmdDataGen(ID_ICSSH_PRESENT_DATE, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	}
	if( (yymmdd[0]&0xff) || (yymmdd[1]&0xff) || (yymmdd[2]&0xff) ){
		for( i=j=0; j<3; ++j, dp += 3 ){
			if( yymmdd[j]&0xff ){
				wsprintf( dp, _T("%2X"), yymmdd[j]&0xff );
			}
			else{
				strcpy_s( dp, 2+1, "  " );
			}
			PF1[i].PITM[j] = dp;
		}
	}
	else{
		pbclr( 0, 3, &PF1[(i = 0)] );
	}

	// ê≈ñ±èêí∑ñº
	i++;
	memmove( dp, m_pSnHeadData->Sn_ZMSYO, 6*2 );
	PF1[i++].PITM[0] = dp;
	dp += 13;

	// î[ê≈ín
	if( !m_pPrintCmInfo->OP3 ){	
		// óXï÷î‘çÜ
		if( j = (int)strlen( (char *)m_pSnHeadData->Sn_ZIP ) ){	
			strcpy_s( dp, 2+1, "Åß" );
			memmove( (dp+2), m_pSnHeadData->Sn_ZIP, j );
			PF1[i++].PITM[0] = dp;
			dp += (j + 3);
		}
		else{
			PF1[i++].PITM[0] = (char *)0;
		}

		// î[ê≈ínÅiÇQçsÅj
		memmove( dp, &m_pSnHeadData->Sn_NOZEI[0],  20*2 );				
		PF1[i].PITM[0] = dp;
		dp += 41;
		memmove( dp, &m_pSnHeadData->Sn_NOZEI[40], 20*2 );
		PF1[i++].PITM[1] = dp;
		dp += 41;
	}
	else{
		pbclr( 0, 1, &PF1[i++] );
		pbclr( 0, 2, &PF1[i++] );
	}

	// î[ê≈ínìdòbî‘çÜ
	if( !m_pPrintCmInfo->OPE ){
		memset( buf, '\0', sizeof( buf ) );
		// ìdòbî‘çÜ
		memmove( buf, m_pSnHeadData->Sn_TELNO1, 6 );
		str.Format( _T("%s") , buf );
		str.TrimRight();
		memset( buf, '\0', sizeof( buf ) );
		memmove( buf , str , str.GetLength());
		j = (int)strlen( buf );
		memmove( dp, "      ", 6 );
		memmove( (dp+(6-j)), buf, 6 );
		PF1[i].PITM[0] = dp;
		dp += 7;

		memset( buf, '\0', sizeof( buf ) );
		memmove( buf, m_pSnHeadData->Sn_TELNO2, 4 );
		str.Format( _T("%s") , buf );
		str.TrimRight();
		memset( buf, '\0', sizeof( buf ) );
		memmove( buf , str , str.GetLength());
		memmove( dp , buf, sizeof( buf ) );

		PF1[i].PITM[1] = dp;	dp += 5;

		memset( buf, '\0', sizeof( buf ) );
		memmove( buf, m_pSnHeadData->Sn_TELNO3, 4 );
		str.Format( _T("%s") , buf );
		str.TrimRight();
		memset( buf, '\0', sizeof( buf ) );
		memmove( buf , str , str.GetLength());
		memmove( dp , buf, sizeof( buf ) );
		PF1[i++].PITM[2] = dp;
		dp += 5;
	}
	else{
		pbclr( 0, 3, &PF1[i++] );
	}

	// éÅñºñîÇÕâÆçÜ	
	if( !m_pPrintCmInfo->OPF ){	
		// ñ@êlñºÅiêUÇËÇ™Ç»Åj
//2016.10.20 UPDATE START
//		memmove( dp, m_pSnHeadData->Sn_COFRI, 68 );	
//		PF1[i++].PITM[0] = dp;
//		dp += 69;
		memmove( dp, m_pSnHeadData->Sn_COFRI, 80 );	
		PF1[i++].PITM[0] = dp;
		dp += 81;
//2016.10.20 UPDATE END
	}
	else{
		pbclr( 0, 1, &PF1[i++] );
	}

	// ñ@êlñº
	if( !m_pPrintCmInfo->OP4 && !m_pPrintCmInfo->OPM ){
		memmove( dp, m_pSnHeadData->Sn_CONAM, 20*2 );
		PF1[i++].PITM[0] = dp;
		dp += 41;
	}
	else{
		pbclr( 0, 1, &PF1[i++] );
	}

	// ë„ï\é“ÅiêUÇËÇ™Ç»Åj
	if( !m_pPrintCmInfo->OPG ){
		memmove( dp, m_pSnHeadData->Sn_DIFRI, 68 );
		PF1[i++].PITM[0] = dp;	
		dp += 69;
	}
	else{
		pbclr( 0, 1, &PF1[i++] );
	}

	// ë„ï\é“ñº
	if( !m_pPrintCmInfo->OP5 ){
		memmove( dp, m_pSnHeadData->Sn_DIHYO, 20*2 );
		PF1[i++].PITM[0] = dp;
		dp += 41;
	}
	else{
		pbclr( 0, 1, &PF1[i++] );
	}

	// É}ÉCÉiÉìÉoÅ[
	BOOL mynum_form = FALSE;
	if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
			mynum_form = TRUE;
		}
	}else{
		if( m_pSnHeadData->Sn_KDAYS >= ICS_SH_MYNUMBER_REFORM_DAY ){
			mynum_form = TRUE;
		}
		else if( m_pSnHeadData->IsPrintWithMynoHonpyo() ){
			mynum_form = TRUE;
		}
	}

	if( mynum_form == TRUE ){
		if( m_ShRevType >= ID_ICSSH_MYNUMBER_REVTYPE ){
			CICSMNSub8 mnsub;
//-- '17.03.22 --
//			if(( isHikae == TRUE )&&( m_pSnHeadData->IsSoleProprietor() )){
//---------------
			if( (isHikae&&m_pSnHeadData->IsSoleProprietor()) || IsNotPrintHojinNo() ){
//---------------
				pbclr( 0, 1, &PF1[i++] );
			}else{
				CString myno_str,myno_prn;
				if( m_pSnHeadData->GetMyNumber(myno_str) == 0 ){
					if( m_pPset->psSigCoprn&0x4000){
						myno_prn = mnsub.MakePrintKojinNo( myno_str,  m_pVprn->Get_KojinPrintFlg() );
					}else{
						myno_prn = myno_str;
					}
					if( m_pSnHeadData->IsSoleProprietor() == TRUE ){
						myno_prn = ' ' + myno_prn;
					}
					memmove( dp, myno_prn, 13 );
					PF1[i++].PITM[0] = dp;
					dp += 14;
				}else{
					pbclr( 0, 1, &PF1[i++] );
				}
			}
		}else{
			pbclr( 0, 1, &PF1[i++] );
		}
	}else{
		// åoóùíSìñé“ñº
		if( !m_pPrintCmInfo->OP6 ){
			memmove( dp, m_pSnHeadData->Sn_KEIRI, 20*2 );					
			PF1[i++].PITM[0] = dp;
			dp += 41;
		}
		else{
			pbclr( 0, 1, &PF1[i++] );
		}
	}

	// àÍòAî‘çÜ
	if( ((m_pSnHeadData->Sn_ITIREN[0]&0xff)!=0xff ) && (m_pSnHeadData->IsSoleProprietor()==FALSE) ){

		memset( buf, '\0', sizeof( buf ) );
		m_Arith.l_unpac( buf, m_pSnHeadData->Sn_ITIREN, 8 );
		for( j=(int)strlen(buf); j>0; ){
			if( buf[--j] != 0x20 ){
				break;
			}
		}
		++j;
		if( j < 8 ){
			memmove( dp, "        ", 8-j );
		}
		memmove( (dp+(8-j)), buf, j );					
		PF1[i++].PITM[0] = dp;
		dp += 9;
	}
	else{
		PF1[i++].PITM[0] = (char *)0;
	}

	int CCSw = 0;

	// êÆóùî‘çÜìôèoóÕÉTÉCÉì
	if( (m_pPrintCmInfo->PRtype&0x80) || 
		((m_pPrintCmInfo->OP1==0) || (IsNotPrintGengoCircle()==false)) ){

		int gengo1 = 0, gengo2 = 0;
		if( !(m_pPrintCmInfo->PRtype&0x80) && m_pPrintCmInfo->OP1 ){
			pbclr(0, 1, &PF1[i++]);
			pbclr(0, 6, &PF1[i]);
			if( IsSwitch_HeiseiReiwa() && (IsNotPrintGengoCircle()==false) ){
				// ê\çêä˙ä‘é©
				memset(yymmdd, '\0', sizeof(yymmdd));
				m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1);
				// ê\çêä˙ä‘éä
				memset(yymmdd, '\0', sizeof(yymmdd));
				m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo2);
			}
		}
		else{
			if( (m_pSnHeadData->Sn_SEIRI[0]&0xff) != 0xff ){
				memset(buf, '\0', sizeof(buf));
				m_Arith.l_unpac(buf, m_pSnHeadData->Sn_SEIRI, 8);
				for( j=(int)strlen(buf); j>0; ){
					if( buf[--j] != 0x20 ){
						break;
					}
				}
				++j;
				if( j < 8 ){
					memmove(dp, "        ", 8-j);
				}
				// êÆóùî‘çÜ
				memmove((dp+(8-j)), buf, j);
				PF1[i++].PITM[0] = dp;
				dp += 9;
			}
			else{
				PF1[i++].PITM[0] = (char *)0;
			}
			if( CCSw ){	// ã≠êßä˙ä‘ïœçX
				for( j=0; j<3; ++j, dp+=3 ){				// ê\çêä˙ä‘é©
					PF1[i].PITM[j] = dp;
				}
				for( ; j<6; ++j, dp += 3 ){					// ê\çêä˙ä‘éä
					PF1[i].PITM[j] = dp;
				}
			}
			else{
				// ê\çêä˙ä‘é©
				memset(yymmdd, '\0', sizeof(yymmdd));
				if( IsJcContinuePrint(true) ){
					m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
				}
				else{
					//m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
					m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1);
				}
				for( j = 0; j < 3; ++j, dp+=3 ){
					wsprintf(dp, _T("%02X"), yymmdd[j]&0xff);
					PF1[i].PITM[j] = dp;
				}
				// ê\çêä˙ä‘éä
				memset(yymmdd, '\0', sizeof(yymmdd));
				if( IsJcContinuePrint(true) ){
					m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
				}
				else{
					//m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
					m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo2);
				}
				for( ; j<6; ++j, dp+=3 ){
					wsprintf(dp, _T("%02X"), yymmdd[j-3]&0xff);
					PF1[i].PITM[j] = dp;
				}
			}
		}

		if( IsSwitch_HeiseiReiwa() ){
			if( IsNotPrintGengoCircle() ){
				pbclr(0, 2, &PF1[++i]);
				pbclr(0, 2, &PF1[++i]);
			}
			else{
				if( CCSw ){	// ã≠êßä˙ä‘ïœçX
					i++;
					PF1[i].PITM[0] = (char *)0;
					PF1[i].PITM[1] = (char *)0;
					i++;
					PF1[i].PITM[0] = (char *)0;
					PF1[i].PITM[1] = (char *)0;
				}
				else{
					i++;
					if( IsNotPrintGengoCircle() ){
						gengo1 = -1;
						gengo2 = -1;
					}

					if( gengo1 == ID_ICSSH_REIWA_GENGO ) {
						memmove(dp, "    ", 4);
						PF1[i].PITM[0] = (char *)0;
						PF1[i].PITM[1] = dp;
						dp += 5;
					}
					else if( gengo1 == ID_ICSSH_HEISEI_GENGO ) {
						memmove(dp, "    ", 4);
						PF1[i].PITM[0] = dp;
						PF1[i].PITM[1] = (char *)0;
						dp += 5;
					}
					else{
						PF1[i].PITM[0] = (char *)0;
						PF1[i].PITM[1] = (char *)0;
					}

					i++;
					if( gengo2 == ID_ICSSH_REIWA_GENGO ) {
						memmove(dp, "    ", 4);
						PF1[i].PITM[0] = (char *)0;
						PF1[i].PITM[1] = dp;
						dp += 5;
					}
					else if( gengo2 == ID_ICSSH_HEISEI_GENGO ) {
						memmove(dp, "    ", 4);
						PF1[i].PITM[0] = dp;
						PF1[i].PITM[1] = (char *)0;
						dp += 5;
					}
					else{
						PF1[i].PITM[0] = (char *)0;
						PF1[i].PITM[1] = (char *)0;
					}
				}
			}
		}
	}
	else{
		pbclr(0, 1, &PF1[i++]);
		pbclr(0, 6, &PF1[i]);
		if(IsSwitch_HeiseiReiwa()){
			pbclr(0, 2, &PF1[++i]);
			pbclr(0, 2, &PF1[++i]);
		}
	}

	switch( m_pSnHeadData->Sn_SKKBN ){
		case 1 :
			if( !(m_pPrintCmInfo->PRtype&0x80) ){
				// ÇnÇbÇqópéÜ
				if( m_pPrintCmInfo->OPC ){
					PF1[++i].PITM[0] =  (char *)0;	
				}
				else{
//2018.03.13 UPDATE START
//					PF1[++i].PITM[0] = _T("Å@ämíËÅ@");
					if( m_pSnHeadData->Sn_Syuusei&0x10 ){
						PF1[++i].PITM[0] = _T("èÄämíË");
					}else{
						PF1[++i].PITM[0] = _T("Å@ämíËÅ@");
					}
//2018.03.13 UPDATE END
				}
			}
			else{
//2018.03.13 UPDATE START
//				PF1[++i].PITM[0] = _T("Å@ämíËÅ@");
				if( m_pSnHeadData->Sn_Syuusei&0x10 ){
					PF1[++i].PITM[0] = _T("èÄämíË");
				}else{
					PF1[++i].PITM[0] = _T("Å@ämíËÅ@");
				}
//2018.03.13 UPDATE END
			}
			break;
		case 2 :
			PF1[++i].PITM[0] = _T("Å@íÜä‘Å@");
			break;
		case 3 :
			PF1[++i].PITM[0] = _T("èCê≥ämíË");
			break;
		case 4 :
			PF1[++i].PITM[0] = _T("èCê≥íÜä‘");
			break;
	}

	// êÆóùî‘çÜìôèoóÕÉTÉCÉì
	if( (m_pPrintCmInfo->PRtype&0x80) ||
		(m_pPrintCmInfo->OP1==0) || (IsNotPrintGengoCircle()==false) ){

		int gengo1 = 0, gengo2 = 0;
		//íÜä‘ê\çêä˙ä‘é©•éä
		switch( m_pSnHeadData->Sn_SKKBN ){
			case 1 : 
			case 3 :
				pbclr( 0, 6, &PF1[++i] );

				if(IsSwitch_HeiseiReiwa()){
					pbclr(0, 2, &PF1[++i]);
					pbclr(0, 2, &PF1[++i]);
				}

				break;
			default:

				if( !(m_pPrintCmInfo->PRtype&0x80) && m_pPrintCmInfo->OP1 ){
					pbclr(0, 6, &PF1[++i]);
					if( IsSwitch_HeiseiReiwa() && (IsNotPrintGengoCircle()==false) ){
						memset(yymmdd, '\0', sizeof(yymmdd));
						m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1);
						memset(yymmdd, '\0', sizeof(yymmdd));
						m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo2);
					}
				}
				else{
					if( CCSw ){	// ã≠êßä˙ä‘ïœçXÅ@02'01.21
						for( ++i, j=0; j<3; ++j, dp+=3 ){
							PF1[i].PITM[j] = dp;
						}
						for( ; j<6; ++j, dp+=3 ){
							PF1[i].PITM[j] = dp;
						}
					}
					else{
						memset(yymmdd, '\0', sizeof(yymmdd));
						if( IsJcContinuePrint(true) ){
							m_pSnHeadData->GetYmdData(ID_ICSSH_CHUUKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
						}
						else{
							//m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
							m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1);
						}
						for( ++i, j=0; j<3; ++j, dp += 3 ){
							wsprintf(dp, _T("%02X"), yymmdd[j]&0xff);
							PF1[i].PITM[j] = dp;
						}
						memset(yymmdd, '\0', sizeof(yymmdd));
						if( IsJcContinuePrint(true) ){
							m_pSnHeadData->GetYmdData(ID_ICSSH_CHUUKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
						}
						else{
							//m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
							m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo2);
						}
						for( ; j<6; ++j, dp += 3 ){
							wsprintf(dp, _T("%02X"), yymmdd[j-3]&0xff);
							PF1[i].PITM[j] = dp;
						}
					}
				}

				if( IsSwitch_HeiseiReiwa() ){
					if( IsNotPrintGengoCircle() ){
						pbclr(0, 2, &PF1[++i]);
						pbclr(0, 2, &PF1[++i]);
					}
					else{
						if( CCSw ){	// ã≠êßä˙ä‘ïœçX
							i++;
							PF1[i].PITM[0] = (char *)0;
							PF1[i].PITM[1] = (char *)0;
							i++;
							PF1[i].PITM[0] = (char *)0;
							PF1[i].PITM[1] = (char *)0;
						}
						else{
							if( IsNotPrintGengoCircle() ){
								gengo1 = -1;
								gengo2 = -1;
							}

							i++;
							if( gengo1 == ID_ICSSH_REIWA_GENGO ) {
								memmove(dp, "    ", 4);
								PF1[i].PITM[0] = (char *)0;
								PF1[i].PITM[1] = dp;
								dp += 5;
							}
							else if( gengo1 == ID_ICSSH_HEISEI_GENGO ) {
								memmove(dp, "    ", 4);
								PF1[i].PITM[0] = dp;
								PF1[i].PITM[1] = (char *)0;
								dp += 5;
							}
							else{
								PF1[i].PITM[0] = (char *)0;
								PF1[i].PITM[1] = (char *)0;
							}

							i++;
							if( gengo2 == ID_ICSSH_REIWA_GENGO ) {
								memmove(dp, "    ", 4);
								PF1[i].PITM[0] = (char *)0;
								PF1[i].PITM[1] = dp;
								dp += 5;
							}
							else if( gengo2 == ID_ICSSH_HEISEI_GENGO ) {
								memmove(dp, "    ", 4);
								PF1[i].PITM[0] = dp;
								PF1[i].PITM[1] = (char *)0;
								dp += 5;
							}
							else{
								PF1[i].PITM[0] = (char *)0;
								PF1[i].PITM[1] = (char *)0;
							}
						}
					}
				}
				break;
		}
	}
	else{
		pbclr( 0, 6, &PF1[++i] );
		if(IsSwitch_HeiseiReiwa()){
			pbclr(0, 2, &PF1[++i]);
			pbclr(0, 2, &PF1[++i]);
		}
	}

	if( j = _xprn( PP, 1, PF1, 0, pDC, isNoImage, SEno ) ){
		goto POUTn;
	}

	char	WORK0[MONY_BUF_SIZE] = {0};
	char	WORK_ALL[MONY_BUF_SIZE] = {0};
	// åvéZåãâ ÉoÉbÉtÉ@
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
	m_Arith.l_input( val100 , "100" );

	CH26SnFhyo10Data*	PrintData;

	if( m_pSnFhyo10Data ){
		//ïtï\1ÉfÅ[É^ì«çûÇ›ÉNÉâÉX
		PrintData = m_pSnFhyo10Data;
	}
	else{
		//ïtï\4ÉfÅ[É^ì«çûÇ›ÉNÉâÉX
		PrintData = m_pSnFhyo40Data;
	}

	//á@
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		m_Arith.l_add( kingaku , PrintData->Sn_1F1A , PrintData->Sn_1F1B );
	}
	m_Arith.l_add( kingaku , kingaku , PrintData->Sn_1F1C );
	memmove( ANS[0].val , kingaku , MONY_BUF_SIZE);

	//áA
	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		m_Arith.l_add( WORK2 , PrintData->Sn_1F2A , PrintData->Sn_1F2B );
		m_Arith.l_add( WORK2 , WORK2 , PrintData->Sn_1F2C );
	}
	else{
		memmove( WORK2 , m_pSnHonpyoData->Sn_SYTX , MONY_BUF_SIZE);
	}
	memmove( ANS[1].val , WORK2 , MONY_BUF_SIZE);

	//áB
	memset( kingaku3 , '\0' , MONY_BUF_SIZE);
	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		m_Arith.l_add( WORK3 , PrintData->Sn_1F3A , PrintData->Sn_1F3B );
	}
	m_Arith.l_add( WORK3 , WORK3 , PrintData->Sn_1F3C );
	memmove( ANS[2].val , WORK3 , MONY_BUF_SIZE);
	//áC
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	if( m_pSnFhyo40Data ){
		if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
			m_Arith.l_add( kingaku , kingaku , PrintData->Sn_1F4T );
		}
		else{
			m_Arith.l_add( kingaku , kingaku , PrintData->Sn_1F4C );
		}
	}
	else{
		if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
			m_Arith.l_add( kingaku , PrintData->Sn_1F4A , PrintData->Sn_1F4B );
		}
		m_Arith.l_add( kingaku , kingaku , PrintData->Sn_1F4C );
	}
	memmove( ANS[3].val , kingaku , MONY_BUF_SIZE);
	m_Arith.l_add( WORK7 , WORK7 ,kingaku );
	//áD
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		m_Arith.l_add( kingaku , PrintData->Sn_1F5A , PrintData->Sn_1F5B );
	}
	m_Arith.l_add( kingaku , kingaku , PrintData->Sn_1F5C );
	memmove( ANS[4].val , kingaku , MONY_BUF_SIZE);
	m_Arith.l_add( WORK7 , WORK7 ,kingaku );
	//áE
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		m_Arith.l_add( kingaku , PrintData->Sn_1F6A , PrintData->Sn_1F6B );
	}
	m_Arith.l_add( kingaku , kingaku , PrintData->Sn_1F6C );
	memmove( ANS[5].val , kingaku , MONY_BUF_SIZE);
	m_Arith.l_add( WORK7 , WORK7 ,kingaku );
	//áF
	memmove( ANS[6].val , WORK7 , MONY_BUF_SIZE);
	//áGáH 7-2-3
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	m_Arith.l_sub( kingaku , WORK7 , WORK2 );
	m_Arith.l_sub( kingaku , kingaku , WORK3 );
	m_Arith.l_add( WORK8 , WORK8 , kingaku );
	
	if( m_Arith.l_test( kingaku )  < 0 ){//É}ÉCÉiÉXÇÃèÍçá
		m_Arith.l_neg( kingaku );
		memmove( ANS[8].val , kingaku , MONY_BUF_SIZE);//áH
		m_Arith.l_div( ANS[8].val , ANS[8].val , PW1 );
		m_Arith.l_mul( ANS[8].val , ANS[8].val , PW1 );
	}
	else{//ÉvÉâÉXÇÃèÍçá
		if( m_pSnHeadData->Sn_SKKBN%2 ){//íÜä‘ÇÃèÍçáÅAä“ïtéûÇÕï\é¶ÇµÇ»Ç¢
			memmove( ANS[7].val , kingaku , MONY_BUF_SIZE);//áG
		}
		else{
			memset( kingaku , '\0' , MONY_BUF_SIZE );
		}
	}

	//áI
	memmove( ANS[9].val, m_pSnHonpyoData->Sn_TYNOFZ, MONY_BUF_SIZE );

	//áJ 9-10
	m_Arith.l_sub( kingaku , ANS[8].val , m_pSnHonpyoData->Sn_TYNOFZ);

	if( m_Arith.l_test( kingaku )  < 0 ){//É}ÉCÉiÉXÇÃèÍçá
		m_Arith.l_neg( kingaku );
		memmove( ANS[11].val , kingaku , MONY_BUF_SIZE);//áJ
	}
	else{//ÉvÉâÉXÇÃèÍçá
		memmove( ANS[10].val , kingaku , MONY_BUF_SIZE);//áK
	}
	//áL
	if( m_pSnHeadData->Sn_SKKBN == 3 || m_pSnHeadData->Sn_SKKBN == 4 ){//èCê≥ÇÃèÍçáÇÃÇ›
		memmove( ANS[12].val, m_pSnHonpyoData->Sn_KAKTIZ, MONY_BUF_SIZE );
		//áMç∑à¯î[ïtê≈äz
		if( m_Arith.l_test(ANS[10].val) > 0 ){
			m_Arith.l_sub( ANS[13].val , ANS[10].val , m_pSnHonpyoData->Sn_KAKTIZ );
		}
		else{
			m_Arith.l_sub( ANS[13].val , ANS[13].val, ANS[11].val );
			m_Arith.l_sub( ANS[13].val , ANS[13].val, m_pSnHonpyoData->Sn_KAKTIZ );
		}
		m_Arith.l_sub( ANS[13].val , ANS[13].val, ANS[7].val );
	}
//-- '16.12.26 --
//	//áNâ€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
//	memmove( ANS[14].val , m_pSnHonpyoData->Sn_KZURI , MONY_BUF_SIZE );
//
//	if( m_pSnHeadData->m_s_sgn2&0x0f){
//		//ä»à’â€ê≈éû
//		memmove( ANS[15].val , m_pSnHonpyoData->Sn_ZZURI , MONY_BUF_SIZE );
//	}
//	else{
//		//áOéëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
//		memmove( ANS[15].val , m_pSnHonpyoData->Sn_SOURI , MONY_BUF_SIZE );
//	}
//---------------
	//áNâ€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
	if( !(m_pSnHeadData->m_s_sgn2&0x0f) && (m_Arith.l_test(m_pSnHonpyoData->Sn_KZURI)<0) ){
		memset( ANS[14].val, '\0', MONY_BUF_SIZE );
	}
	else{
		memmove( ANS[14].val, m_pSnHonpyoData->Sn_KZURI, MONY_BUF_SIZE );
	}
	if( m_pSnHeadData->m_s_sgn2&0x0f){
		//áOäÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
		memmove( ANS[15].val , m_pSnHonpyoData->Sn_ZZURI , MONY_BUF_SIZE );
	}
	else{
		//áOéëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
		if( m_Arith.l_test(m_pSnHonpyoData->Sn_SOURI) < 0 ){
			memset( ANS[15].val, '\0', MONY_BUF_SIZE );
		}
		else{
			memmove( ANS[15].val , m_pSnHonpyoData->Sn_SOURI , MONY_BUF_SIZE );
		}
	}

//---------------
	//áPçTèúïsë´ä“ïtê≈äz
	//áQç∑à¯ê≈äz
	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		char	WORK0[MONY_BUF_SIZE]={0};
		char	WORK1[MONY_BUF_SIZE]={0};
		char	WORK2[MONY_BUF_SIZE]={0};
		m_Arith.l_add( WORK0 , PrintData->Sn_1F8B , PrintData->Sn_1F8C );//8
		m_Arith.l_add( WORK1 , PrintData->Sn_1F9B , PrintData->Sn_1F9C );//9
		m_Arith.l_sub( WORK2 , WORK1 , WORK0 );//9-8		if( m_Arith.l_test( kingaku ) < 0 ){//É}ÉCÉiÉXÇÃèÍçááPÇ…
		if( m_Arith.l_test( WORK2 ) < 0 ){
			if( m_pSnHeadData->Sn_SKKBN%2 ){//íÜä‘ÇÃèÍçáÅAä“ïtéûÇÕï\é¶ÇµÇ»Ç¢
				m_Arith.l_neg( WORK2 );
				memmove(  ANS[16].val , WORK2 , MONY_BUF_SIZE );//áQ
			}
		}
		else{//ÉvÉâÉXÇÃèÍçááQÇ…
			memmove(  ANS[17].val , WORK2 , MONY_BUF_SIZE );//áP
		}
	}
	else{
		memmove( kingaku , WORK8 , MONY_BUF_SIZE);
		if( m_Arith.l_test( kingaku ) < 0 ){//É}ÉCÉiÉXÇÃèÍçááPÇ…
			m_Arith.l_neg( kingaku );
			memmove(  ANS[17].val , kingaku , MONY_BUF_SIZE );//áQ
			m_Arith.l_div( ANS[17].val , ANS[17].val , PW1 );
			m_Arith.l_mul( ANS[17].val , ANS[17].val , PW1 );
		}
		else{//ÉvÉâÉXÇÃèÍçááQÇ…
			if( m_pSnHeadData->Sn_SKKBN != 2  ){
				if( m_pSnHeadData->Sn_SKKBN%2 ){//íÜä‘ÇÃèÍçáÅAä“ïtéûÇÕï\é¶ÇµÇ»Ç¢
					memmove(  ANS[16].val , kingaku , MONY_BUF_SIZE );//áP
				}
			}
		}
	}

	//áRè˜ìnäÑäzä“ïtäz
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	memset( kingaku2 , '\0' , MONY_BUF_SIZE);
		
	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		m_Util.percent( kingaku,(char*)PrintData->Sn_1F8B, 7, 25, 0 );
		m_Arith.l_add(kingaku2 , kingaku2 , kingaku);
	}
	m_Util.percent( kingaku,(char*)PrintData->Sn_1F8C, 8, 17, 0 );
	m_Arith.l_add(kingaku2, kingaku2 , kingaku);

	//20è˜ìnäÑäzî[ê≈äz
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	memset( kingaku3 , '\0' , MONY_BUF_SIZE);
	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		m_Util.percent( kingaku, (char*)PrintData->Sn_1F9B, 7, 25, 0 );
		m_Arith.l_add(kingaku3 , kingaku3 , kingaku);
		m_Util.percent( kingaku, (char*)PrintData->Sn_1F9C, 8, 17, 0 );
		m_Arith.l_add(kingaku3 , kingaku3 , kingaku);
		m_Arith.l_sub(kingaku3 , kingaku3 , kingaku2);
		if( m_Arith.l_test( kingaku3 ) < 0 ){//É}ÉCÉiÉXÇÃèÍçááRÇ…
			if( (m_pSnHeadData->Sn_SKKBN%2)  ){
				memmove(  ANS[18].val , kingaku3 , MONY_BUF_SIZE );
				m_Arith.l_neg(  ANS[18].val );
			}
		}
		else{
			memmove(  ANS[19].val , kingaku3 , MONY_BUF_SIZE );
			m_Arith.l_div( ANS[19].val , ANS[19].val , PW1 );
			m_Arith.l_mul( ANS[19].val , ANS[19].val , PW1 );
		}
	}
	else{
		if( m_Arith.l_test( ANS[17].val ) ){
			m_Util.percent( kingaku,(char*)ANS[17].val, 8, 17, 0 );
			m_Arith.l_add(kingaku3 , kingaku3 , kingaku);
			m_Arith.l_div( ANS[19].val , kingaku3 , PW1 );
			m_Arith.l_mul( ANS[19].val , ANS[19].val , PW1 );
		}
		else{
			m_Util.percent( kingaku,(char*)ANS[16].val, 8, 17, 0 );
			m_Arith.l_add(kingaku3 , kingaku3 , kingaku);
			memmove(  ANS[18].val , kingaku3 , MONY_BUF_SIZE );
		}
	}

	//21íÜä‘î[ïtè˜ìnäÑäz
	memmove( ANS[20].val , m_pSnHonpyoData->Sn_TTYWAR , MONY_BUF_SIZE);
	//22
	m_Arith.l_sub( kingaku , ANS[19].val, ANS[20].val );
	if( m_Arith.l_test( kingaku ) < 0 ){//É}ÉCÉiÉXÇÃèÍçááRÇ…
		memmove(  ANS[22].val , kingaku , MONY_BUF_SIZE );
		m_Arith.l_neg(  ANS[22].val );
		m_Arith.l_div( ANS[22].val , ANS[22].val , PW1 );
		m_Arith.l_mul( ANS[22].val , ANS[22].val , PW1 );
	}
	else{
		memmove(  ANS[21].val , kingaku , MONY_BUF_SIZE );
	}
	if(  m_pSnHeadData->Sn_SKKBN == 3 ||  m_pSnHeadData->Sn_SKKBN == 4 ){
		//24
		memmove( ANS[23].val , m_pSnHonpyoData->Sn_TKAKTZ , MONY_BUF_SIZE);
		//25
		//ç∑à¯î[ïtê≈äz
		if( m_Arith.l_test(ANS[21].val) > 0 ){
			m_Arith.l_sub( ANS[24].val , ANS[21].val , m_pSnHonpyoData->Sn_TKAKTZ );
		}
		else{
			char	tmpMony[MONY_BUF_SIZE]={0};
			m_Arith.l_add( tmpMony, ANS[18].val, ANS[22].val );
			if( m_Arith.l_test(tmpMony) ){
				m_Arith.l_neg( tmpMony );
			}
			m_Arith.l_sub( ANS[24].val , tmpMony, m_pSnHonpyoData->Sn_TKAKTZ );
		}
		m_Arith.l_div( ANS[24].val , ANS[24].val , PW1 );
		m_Arith.l_mul( ANS[24].val , ANS[24].val , PW1 );

	}
	
//JANS
	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		memmove( JANS[0].val ,  PrintData->Sn_1F1A , MONY_BUF_SIZE);
		memmove( JANS[1].val ,  PrintData->Sn_1F1B , MONY_BUF_SIZE);
	}

	memmove( JANS[2].val ,  PrintData->Sn_1F1C , MONY_BUF_SIZE);

	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		memmove( JANS[3].val ,  PrintData->Sn_1F2A , MONY_BUF_SIZE);
		memmove( JANS[4].val ,  PrintData->Sn_1F2B , MONY_BUF_SIZE);
	}
	memmove( JANS[5].val ,  PrintData->Sn_1F2C , MONY_BUF_SIZE);

	if( m_pSnHeadData->SVmzsw == 1 ){// åoâﬂë[íuëŒè€ÇÃâ€ê≈éëéYÇÃè˜ìnìôÇ†ÇË
		m_Arith.l_sub( kingaku , PrintData->Sn_1F9B , PrintData->Sn_1F8B );
		if( m_Arith.l_test( ANS[16].val ) > 0){//ä“ïtÇÃÇ∆Ç´
			if( !(m_pSnHeadData->Sn_SKKBN%2) ){
				memset( kingaku , '\0' , MONY_BUF_SIZE);
			}
		}
		if( !m_Arith.l_test( ANS[16].val ) && ( !m_Arith.l_test( ANS[17].val ) || ( m_Arith.l_cmp( ANS[17].val , val100 ) < 0 ))){
			memset( kingaku , '\0' , MONY_BUF_SIZE);
		}
		memmove( JANS[6].val ,  kingaku , MONY_BUF_SIZE);
		// ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz6.3%
		m_Arith.l_sub( kingaku , PrintData->Sn_1F9C , PrintData->Sn_1F8C );
		if( m_Arith.l_test( ANS[16].val ) > 0){//ä“ïtÇÃÇ∆Ç´
			if( !(m_pSnHeadData->Sn_SKKBN%2) ){
				memset( kingaku , '\0' , MONY_BUF_SIZE);
			}
		}
		if( !m_Arith.l_test( ANS[16].val ) && ( !m_Arith.l_test( ANS[17].val ) || ( m_Arith.l_cmp( ANS[17].val , val100 ) < 0 ))){
			memset( kingaku , '\0' , MONY_BUF_SIZE);
		}
		memmove( JANS[7].val ,  kingaku , MONY_BUF_SIZE);
	}
	else{
		if( m_Arith.l_test (ANS[16].val) ){
			memmove( JANS[7].val ,  ANS[16].val , MONY_BUF_SIZE);
			m_Arith.l_neg ( JANS[7].val );
		}
		else{
			memmove( JANS[7].val ,  ANS[17].val , MONY_BUF_SIZE);
		}
	}
	int FontFlg = 0;
	int FontFlg2 = 0; //äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇÇÃÉtÉHÉìÉgÉTÉCÉYÉtÉâÉO 0:ïWèÄÅHÉTÉCÉYÅ@1:è¨Ç≥Ç¢ÉTÉCÉY
	char buff[32] = {0};
	int DefSize = 0 , Size = 0;
	if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
		Size = 15;
	}
	else{
		Size = 15;
	}
	for( int idx = 0; idx < 3; idx++){
		::memset( buff , '\0' , sizeof( buff ) );
		m_Arith.l_print( buff , JANS[idx].val , FMT16_EX );
		str.Format( _T("%s") , buff );
		str.TrimLeft();
		if( str.GetLength() > Size ){
			FontFlg = 1;
			break;
		}
	}
	for( int idx = 3; idx < 6; idx++){
		::memset( buff , '\0' , sizeof( buff ) );
		m_Arith.l_print( buff , JANS[idx].val , FMT16_EX );
		str.Format( _T("%s") , buff );
		str.TrimLeft();
		if( str.GetLength() > 11 ){
			FontFlg = 1;
			break;
		}
	}

	dp = AP;
	memset( dp, '\0', size );
	memset( WORK0, '\0', sizeof(WORK0) );
	i = j = 0;
	m_Arith.l_div( WORK0, ANS[j].val, PW0 );
	m_Arith.l_print( dp, WORK0, FMT011EX );
	m_Arith.l_mul( ANS[j].val , WORK0 , PW1);
	PF1[i].PITM[j] = dp;
	dp+=17;
	i++;
	// ã‡äz¢1£->¢3£
	for( j=0; j<2; ++j, dp+=17 ){	
		m_Arith.l_print( dp, ANS[j+1].val, FMT12EX );
		PF1[i].PITM[j] = dp;
	}
	
	// ã‡äz¢4£->¢8£
	for( ++i, j=0; j<5; ++j, dp+=17 ){
		m_Arith.l_print( dp, ANS[j+3].val, FMT12EX );
		PF1[i].PITM[j] = dp;
	}

	// ã‡äz¢9£->¢12£
	for( ++i, j=0; j<4; ++j, dp += 17 ){
		switch( j ){
			case 1 :
				// íÜä‘î[ïtê≈äz
				if( !(m_pPrintCmInfo->PRtype&0x80) && m_pPrintCmInfo->OPB ){		
					PF1[i].PITM[j] = (char *)0;
					continue;
				}
				// Å´â∫Ç÷
			default:
				m_Arith.l_div( WORK0, ANS[j+8].val, PW1 );
				m_Arith.l_print( dp, WORK0, FMT12EX );
				strcat_s( dp, 16+1, "  " );
				m_Arith.l_mul( ANS[j+8].val , WORK0 , PW1);
				break;
		}
		PF1[i].PITM[j] = dp;
	}
	
	// ã‡äz¢13£->¢16£
	for( ++i, j=0; j<4; ++j, dp += 18 ){
		switch( j ){
			case 1 :
				m_Arith.l_div( WORK0, ANS[j+12].val, PW1 );
				m_Arith.l_print( dp, WORK0, FMT12 );
				strcat_s( dp, 17+1, "  " );
				m_Arith.l_mul( ANS[j+12].val , WORK0 , PW1);
				break;
			default:
				m_Arith.l_print( dp, ANS[j+12].val, FMT13EX );
				break;
		}
		PF1[i].PITM[j] = dp;
	}
	
	// ã‡äz¢17£->¢20£
	for( ++i, j=0; j<4; ++j, dp+=17 ){
		switch( j ){
			case 0 :
			case 2 :
				m_Arith.l_print( dp, ANS[j+16].val, FMT12EX );
				break;
			default:	
				m_Arith.l_div( WORK0, ANS[j+16].val, PW1 );
				m_Arith.l_print( dp, WORK0, FMT011EX );
				strcat_s( dp, 16+1, "  " );
				m_Arith.l_mul( ANS[j+16].val , WORK0 , PW1);
				break;
		}
		PF1[i].PITM[j] = dp;
	}
	
	// ã‡äz¢21£->¢25£
	for( ++i, j=0; j<5; ++j, dp+=18 ){
		switch( j ){
			case 3 :
				m_Arith.l_print( dp, ANS[j+20].val, FMT13EX );
				break;
			case 0 :
				// íÜä‘î[ïtè˜ìnäÑäz
				if( ! (m_pPrintCmInfo->PRtype&0x80) && m_pPrintCmInfo->OPB ){
					PF1[i].PITM[j] = (char *)0;
					continue;
				}
				// Å´â∫Ç÷
			default:
				m_Arith.l_div( WORK0, ANS[j+20].val, PW1 );
				m_Arith.l_print( dp, WORK0, FMT12 );
				strcat_s( dp, 17+1, "  " );
				m_Arith.l_mul( ANS[j+20].val , WORK0 , PW1);
				break;
		}
		PF1[i].PITM[j] = dp;
	}
	
	// ê≈äzç∑à¯Ç´ÇOâ~É`ÉFÉbÉN
	for( k=0; k<26; k++ ){
		if( m_Arith.l_test( ANS[k].val ) ){
			k = 0;
			break;
		}
	}
	//26
	// è¡îÔê≈ãyÇ—ínï˚è¡îÔê≈ÇÃçáåvÅiî[ïtñîÇÕä“ïtÅjê≈äz
	switch( m_pSnHeadData->Sn_SKKBN&0xff ) {
		case 1 : case 2 :		// ämíËÅEíÜä‘ê\çê
			m_Arith.l_add( kingaku, ANS[10].val, ANS[21].val );
			m_Arith.l_add( kingaku2, ANS[7].val,  ANS[11].val );
			m_Arith.l_add( kingaku2 , kingaku2, ANS[18].val );
			m_Arith.l_add( kingaku2 , kingaku2, ANS[22].val );
			m_Arith.l_sub( ANS[25].val, kingaku, kingaku2 );
			break;
		default	:				// èCê≥ê\çê
			m_Arith.l_add( ANS[25].val, ANS[13].val, ANS[24].val );
			break;
	}
	// ã‡äz¢26£
	m_Arith.l_print( dp, ANS[25].val, ( k ? FMT12EX : FMT120EX ) );	
	PF1[++i].PITM[0] = dp;
	dp += 17;
	
	if( j = _xprn( PP, 2, PF1, 0, pDC, 0, SEno ) ){
		goto POUTn;
	}

	// ê≈óùémñ@Å@é{çsëOí†ï[	ÉXÉPÉWÉÖÅ[ÉãÅ@ÉXÉLÉbÉv
	dp = AP;
	i = j = 0;
	memset( dp, '\0', size );

	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_KAPPU ? "Åõ" : "Å@");	// ïtãLéñçÄ
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_KAPPU ? "Å@" : "Åõ");
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_NOBE  ? "Åõ" : "Å@");
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_NOBE  ? "Å@" : "Åõ");
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_KOJI  ? "Åõ" : "Å@");
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_KOJI  ? "Å@" : "Åõ");
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_GENKN ? "Åõ" : "Å@");
	PF1[i].PITM[j++] = (m_pSnHeadData->Sn_GENKN ? "Å@" : "Åõ");
	PF1[i].PITM[j++] = ((m_pSnHeadData->Sn_TUMIAG&0x01) ? "Åõ" : "Å@");
	PF1[i++].PITM[j] = ((m_pSnHeadData->Sn_TUMIAG&0x01) ? "Å@" : "Åõ");
	
	j = 0;
	if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
		if( m_pSnHeadData->IsUseSemiUriRatio() ){
			PF1[i].PITM[j++] = "Åõ";
			PF1[i].PITM[j++] = "Å@";
			PF1[i].PITM[j++] = (char *)0;
		}
		else{
			if( !(m_pSnHeadData->Sn_Sign4&0x10) &&
				((m_pSnHeadData->IsRatioOver95()==TRUE)||(m_pSnHeadData->Sn_EXP100&0x01)) ){	// ëSäzçTèú
				PF1[i].PITM[j++] = (char *)0;
				PF1[i].PITM[j++] = (char *)0;
				PF1[i].PITM[j++] = "Åõ";
			}
			else{
				if( m_pSnHeadData->IsKobetuSiireAnbun() ){
					PF1[i].PITM[j++] = ("Åõ");
					PF1[i].PITM[j++] = ("Å@");
				}
				else{
					PF1[i].PITM[j++] = ("Å@");
					PF1[i].PITM[j++] = ("Åõ");
				}
				PF1[i].PITM[j++] = (char *)0;
			}
		}
	}
	else{
		// éñã∆ï â€ê≈îÑè„çÇ
		unsigned char		exSaleFlg = 0x00;	// D0:ëÊÇPéÌ, D1:ëÊÇQéÌ, D2:ëÊÇRéÌ, D3:ëÊÇSéÌ, D4:ëÊÇTéÌ
		CH26SnFhyo50Data	*pH26SnFhyo50Data = NULL;
		if( m_pSnHeadData->SVmzsw ){
			pH26SnFhyo50Data = m_pSnFhyo52Data;
		}
		else{
			pH26SnFhyo50Data = m_pSnFhyo50Data;
		}
		memset( WORK0    , '\0', sizeof( WORK0 ) );
		memset( WORK_ALL , '\0', sizeof( WORK_ALL ) );
		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI1) >= 0 ){
			m_Util.l_calq( WORK0, (char *)m_pSnHonpyoData->Sn_KURI1, 2 );
			exSaleFlg |= 0x01;
		}
		else{
			memset( WORK0    , '\0', sizeof( WORK0 ) );
		}
		m_Arith.l_div( WORK0, WORK0, PW0 );
		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI1) == 0 ){
			if( pH26SnFhyo50Data &&
				(m_Arith.l_test(pH26SnFhyo50Data->Sn_5FURI1A)||m_Arith.l_test(pH26SnFhyo50Data->Sn_5FURI1B)||m_Arith.l_test(pH26SnFhyo50Data->Sn_5FURI1C)) ){
				m_Arith.l_print( dp, WORK0, FMT11Z );
			}
			else{
				m_Arith.l_print( dp, WORK0, FMT11 );
				exSaleFlg &= ~0x01;
			}
		}
		else if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI1) < 0 ){
			m_Arith.l_print( dp, WORK0, FMT11Z );
			exSaleFlg |= 0x01;
		}
		else{
			m_Arith.l_print( dp, WORK0, FMT11 );
		}
		m_Arith.l_add( WORK_ALL , WORK_ALL , m_pSnHonpyoData->Sn_KURI1);
		PF1[i].PITM[j++] = dp;
		dp += 12;

		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI2) >= 0 ){
			m_Util.l_calq( WORK0, (char *)m_pSnHonpyoData->Sn_KURI2, 2 );
			exSaleFlg |= 0x02;
		}
		else{
			memset( WORK0    , '\0', sizeof( WORK0 ) );
		}
		m_Arith.l_div( WORK0, WORK0, PW0 );
		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI2) == 0 ){
			if( pH26SnFhyo50Data &&
				(m_Arith.l_test(pH26SnFhyo50Data->Sn_5FURI2A)||m_Arith.l_test(pH26SnFhyo50Data->Sn_5FURI2B)||m_Arith.l_test(pH26SnFhyo50Data->Sn_5FURI2C)) ){
				m_Arith.l_print( dp, WORK0, FMT11Z );
			}
			else{
				m_Arith.l_print( dp, WORK0, FMT11 );
				exSaleFlg &= ~0x02;
			}
		}
		else if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI2) < 0 ){
			m_Arith.l_print( dp, WORK0, FMT11Z );
			exSaleFlg |= 0x02;
		}
		else{
			m_Arith.l_print( dp, WORK0, FMT11 );
		}
		m_Arith.l_add( WORK_ALL , WORK_ALL , m_pSnHonpyoData->Sn_KURI2);
		PF1[i].PITM[j++] = dp;
		dp += 12;

		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI3) >= 0 ){
			m_Util.l_calq( WORK0, (char *)m_pSnHonpyoData->Sn_KURI3, 2 );
			exSaleFlg |= 0x04;
		}
		else{
			memset( WORK0    , '\0', sizeof( WORK0 ) );
		}
		m_Arith.l_div( WORK0, WORK0, PW0 );
		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI3) == 0 ){
			if( pH26SnFhyo50Data &&
				(m_Arith.l_test(pH26SnFhyo50Data->Sn_5FURI3A)||m_Arith.l_test(pH26SnFhyo50Data->Sn_5FURI3B)||m_Arith.l_test(pH26SnFhyo50Data->Sn_5FURI3C)) ){
				m_Arith.l_print( dp, WORK0, FMT11Z );
			}
			else{
				m_Arith.l_print( dp, WORK0, FMT11 );
				exSaleFlg &= ~0x04;
			}
		}
		else if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI3) < 0 ){
			m_Arith.l_print( dp, WORK0, FMT11Z );
			exSaleFlg |= 0x04;
		}
		else{
			m_Arith.l_print( dp, WORK0, FMT11 );
		}
		m_Arith.l_add( WORK_ALL , WORK_ALL , m_pSnHonpyoData->Sn_KURI3);
		PF1[i].PITM[j++] = dp;
		dp += 12;

		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI4) >= 0 ){
			m_Util.l_calq( WORK0, (char *)m_pSnHonpyoData->Sn_KURI4, 2 );
			exSaleFlg |= 0x08;
		}
		else{
			memset( WORK0    , '\0', sizeof( WORK0 ) );
		}
		m_Arith.l_div( WORK0, WORK0, PW0 );
		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI4) == 0 ){
			if( pH26SnFhyo50Data &&
				(m_Arith.l_test(pH26SnFhyo50Data->Sn_5FURI4A)||m_Arith.l_test(pH26SnFhyo50Data->Sn_5FURI4B)||m_Arith.l_test(pH26SnFhyo50Data->Sn_5FURI4C)) ){
				m_Arith.l_print( dp, WORK0, FMT11Z );
			}
			else{
				m_Arith.l_print( dp, WORK0, FMT11 );
				exSaleFlg &= ~0x08;
			}
		}
		else if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI4) < 0 ){
			m_Arith.l_print( dp, WORK0, FMT11Z );
			exSaleFlg |= 0x08;
		}
		else{
			m_Arith.l_print( dp, WORK0, FMT11 );
		}
		m_Arith.l_add( WORK_ALL , WORK_ALL , m_pSnHonpyoData->Sn_KURI4);
		PF1[i].PITM[j++] = dp;
		dp += 12;

		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI5) >= 0 ){
			m_Util.l_calq( WORK0, (char *)m_pSnHonpyoData->Sn_KURI5, 2 );
			exSaleFlg |= 0x10;
		}
		else{
			memset( WORK0    , '\0', sizeof( WORK0 ) );
		}
		m_Arith.l_div( WORK0, WORK0, PW0 );
		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI5) == 0 ){
			if( pH26SnFhyo50Data &&
				(m_Arith.l_test(pH26SnFhyo50Data->Sn_5FURI5A)||m_Arith.l_test(pH26SnFhyo50Data->Sn_5FURI5B)||m_Arith.l_test(pH26SnFhyo50Data->Sn_5FURI5C)) ){
				m_Arith.l_print( dp, WORK0, FMT11Z );
			}
			else{
				m_Arith.l_print( dp, WORK0, FMT11 );
				exSaleFlg &= ~0x10;
			}
		}
		else if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI5) < 0 ){
			m_Arith.l_print( dp, WORK0, FMT11Z );
			exSaleFlg |= 0x10;
		}
		else{
			m_Arith.l_print( dp, WORK0, FMT11 );
		}
		m_Arith.l_add( WORK_ALL , WORK_ALL , m_pSnHonpyoData->Sn_KURI5);
		PF1[i].PITM[j++] = dp;
		dp += 12;

		if(m_SixKindFlg == TRUE){
			//òZéÌÇÃèÍçá
			if( m_Arith.l_test(((CH27SnHonpyoData*)m_pSnHonpyoData)->Sn_KURI6 ) >= 0 ){
				m_Util.l_calq( WORK0, (char *)((CH27SnHonpyoData*)m_pSnHonpyoData)->Sn_KURI6, 2 );
				exSaleFlg |= 0x20;
			}
			else{
				memset( WORK0    , '\0', sizeof( WORK0 ) );
			}
			m_Arith.l_div( WORK0, WORK0, PW0 );
			if( m_Arith.l_test(((CH27SnHonpyoData*)m_pSnHonpyoData)->Sn_KURI6) == 0 ){

				if( (CH27SnFhyo50Data*)pH26SnFhyo50Data &&
					(m_Arith.l_test(((CH27SnFhyo50Data*)pH26SnFhyo50Data)->Sn_5FURI6A)||m_Arith.l_test(((CH27SnFhyo50Data*)pH26SnFhyo50Data)->Sn_5FURI6B)||m_Arith.l_test(((CH27SnFhyo50Data*)pH26SnFhyo50Data )->Sn_5FURI6C)) ){
					m_Arith.l_print( dp, WORK0, FMT11Z );
				}
				else{
					m_Arith.l_print( dp, WORK0, FMT11 );
					exSaleFlg &= ~0x20;
				}
			}
			else if( m_Arith.l_test(((CH27SnHonpyoData*)m_pSnHonpyoData)->Sn_KURI6) < 0 ){
				m_Arith.l_print( dp, WORK0, FMT11Z );
				exSaleFlg |= 0x20;
			}
			else{
				m_Arith.l_print( dp, WORK0, FMT11 );
			}
			m_Arith.l_add( WORK_ALL , WORK_ALL , ((CH27SnHonpyoData*)m_pSnHonpyoData)->Sn_KURI6 );
			PF1[i++].PITM[j] = dp;
			dp += 12;
		}
		else{
			// çáåv
			m_Util.l_calq( WORK0, (char *)m_pSnHeadData->Sn_KURIA, 2 );
			m_Arith.l_div( WORK0, WORK0, PW0 );
			if( exSaleFlg ){
				m_Arith.l_print( dp, WORK0, FMT11Z );
			}
			else{
				m_Arith.l_print( dp, WORK0, FMT11 );
			}

			PF1[i++].PITM[j] = dp;
			dp += 12;
		}
			
		// éñã∆ï â€ê≈îÑè„äÑçá
		j = 0;
		if( (m_Arith.l_test(m_pSnHonpyoData->Sn_KURI1)>0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA)>0) ){
			wsprintf( dp, "%3d", m_pSnHonpyoData->Sn_UP1 / 10 );
		}
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if( (m_Arith.l_test(m_pSnHonpyoData->Sn_KURI1)>0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA)>0) ){
			wsprintf( dp, "%01d", m_pSnHonpyoData->Sn_UP1 % 10 );
		}
		strcat_s( dp, 4+1, "  " );
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if( (m_Arith.l_test(m_pSnHonpyoData->Sn_KURI2)>0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA)>0) ){
			wsprintf( dp, "%3d", m_pSnHonpyoData->Sn_UP2 / 10 );
		}
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if( (m_Arith.l_test(m_pSnHonpyoData->Sn_KURI2)>0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA)>0) ){
			wsprintf( dp, "%01d", m_pSnHonpyoData->Sn_UP2 % 10 );
		}
		strcat_s( dp, 4+1, "  " );
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if( (m_Arith.l_test(m_pSnHonpyoData->Sn_KURI3)>0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA)>0) ){
			wsprintf( dp, "%3d", m_pSnHonpyoData->Sn_UP3 / 10 );
		}
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if( (m_Arith.l_test(m_pSnHonpyoData->Sn_KURI3)>0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA)>0) ){
			wsprintf( dp, "%01d", m_pSnHonpyoData->Sn_UP3 % 10 );
		}
		strcat_s( dp, 4+1, "  " );
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if( (m_Arith.l_test(m_pSnHonpyoData->Sn_KURI4)>0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA)>0) ){
			wsprintf( dp, "%3d", m_pSnHonpyoData->Sn_UP4 / 10 );
		}
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if( (m_Arith.l_test(m_pSnHonpyoData->Sn_KURI4)>0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA)>0) ){
			wsprintf( dp, "%01d", m_pSnHonpyoData->Sn_UP4 % 10 );
		}
		strcat_s( dp, 4+1, "  " );
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if( (m_Arith.l_test(m_pSnHonpyoData->Sn_KURI5)>0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA)>0) ){
			wsprintf( dp, "%3d", m_pSnHonpyoData->Sn_UP5 / 10 );
		}
		PF1[i].PITM[j++] = dp;
		dp += 4;

		if( (m_Arith.l_test(m_pSnHonpyoData->Sn_KURI5)>0) && (m_Arith.l_test(m_pSnHeadData->Sn_KURIA)>0) ){
			wsprintf( dp, "%01d", m_pSnHonpyoData->Sn_UP5 % 10 );
		}
		strcat_s( dp, 4+1, "  " );
		if( m_SixKindFlg == TRUE ){
			PF1[i].PITM[j++] = dp;
		}
		else{
			PF1[i++].PITM[j] = dp;
		}
		dp += 4;

		if(m_SixKindFlg == TRUE){
			if( m_Arith.l_test(((CH27SnHonpyoData*)m_pSnHonpyoData)->Sn_KURI6) ){
				wsprintf( dp, "%3d", ((CH27SnHonpyoData*)m_pSnHonpyoData)->Sn_UP6 / 10 );
			}
			PF1[i].PITM[j++] = dp;
			dp += 4;

			if( m_Arith.l_test(((CH27SnHonpyoData*)m_pSnHonpyoData)->Sn_KURI6) ){
				wsprintf( dp, "%01d", ((CH27SnHonpyoData*)m_pSnHonpyoData)->Sn_UP6 % 10 );
			}
			strcat_s( dp, 4+1, "  " );
			PF1[i++].PITM[j] = dp;
			dp += 4;
		}
		
		PF1[i].PITM[0] = (m_pSnHeadData->Sn_TOKUR ? "Åõ" : "Å@");			// ì¡ó·åvéZìKóp
		PF1[i].PITM[1] = (m_pSnHeadData->Sn_TOKUR ? "Å@" : "Åõ");
	}

	// ê≈óùémñ@Å@é{çsí†ï[
	// ãåê≈ó¶ñºèÃ
	char	KZMY[6], SZMY[6] , SZMY2[6];

	memset( KZMY, '\0', sizeof( KZMY ) );	// â€ê≈ïWèÄäzÅ@ãåê≈ó¶
	memset( SZMY, '\0', sizeof( SZMY ) );	// è¡îÔê≈Å@Å@Å@ÅV
	memset( SZMY2, '\0', sizeof( SZMY2 ) );	// è¡îÔê≈Å@Å@Å@ÅV
	i++;
	if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
		memmove( KZMY , m_pSnHonpyoData->Sn_TS_TDM1 , MONY_BUF_SIZE );
		m_Arith.l_mul( KZMY, KZMY, PW0 );
		memset( buff , '\0' , sizeof(buff));
		m_Arith.l_print( buff , KZMY , FMT16_EX );
		str.Format( _T("%s") , buff );
		str.TrimLeft();
		if( str.GetLength() > 12 ){
			FontFlg = 1;
		}

		if( m_Arith.l_test(KZMY) ){
			PF1[i].PITM[0] = "6+";	// "ÇUÅ{ÇSÅDÇTÅìï™";
			PF1[i].PITM[1] = "4.5Åìï™";
		}
		else{
			PF1[i].PITM[0] = (char *)0;
			PF1[i].PITM[1] = (char *)0;
		}
		i++;

		memmove( SZMY , m_pSnHonpyoData->Sn_TS_SYT1 , MONY_BUF_SIZE );
		memset( buff , '\0' , sizeof(buff));
		m_Arith.l_print( buff , SZMY , FMT16_EX );
		str.Format( _T("%s") , buff );
		str.TrimLeft();
		if( str.GetLength() > 12 ){
			FontFlg = 1;
		}

		if( m_Arith.l_test(SZMY) ){
			PF1[i].PITM[0] = "6+";	// "ÇUÅ{ÇSÅDÇTÅìï™";
			PF1[i].PITM[1] = "4.5Åìï™";
		}
		else{
			PF1[i].PITM[0] = (char *)0;		
			PF1[i].PITM[1] = (char *)0;		
		}
		i++;
		memmove( SZMY2 , m_pSnHonpyoData->Sn_TS_SYT2 , MONY_BUF_SIZE );

		//ì¡íËâ€ê≈édì¸ÇÍÇ…åWÇÈï ï\ÇÃíÒèoóL
		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
			if( m_pSnHeadData->m_DispTabSgn&0x01 ){
				PF1[i].PITM[0] = ("Åõ");
			}else{
				PF1[i].PITM[0] = (char *)0;	
			}
			i++;
		}

		if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
			FontFlg2 = 0;
			memset( buff , '\0' , sizeof(buff));
			memset( WORK0, '\0', sizeof(WORK0) );
			m_Arith.l_div( WORK0, m_pSnHonpyoData->Sn_ZZURI, PW0 );
			m_Arith.l_print( buff , WORK0 , FMT14 );
			str.Format( _T("%s") , buff );
			str.TrimLeft();
			if( str.GetLength() > 12 ){
				FontFlg2 = 1;
			}
			if( FontFlg2 == 0 ){
				memset( WORK0, '\0', sizeof(WORK0) );
				m_Arith.l_div( WORK0, m_pSnHonpyoData->Sn_ZZURI, PW0 );
				m_Arith.l_print( dp, WORK0, FMT14 );
				PF1[i].PITM[0] = dp;
				dp += 19;
			}else{ //è¨Ç≥Ç¢ÉtÉHÉìÉgÇÕå„Ç≈àÛéö
				PF1[i].PITM[0] = (char *)0;	
			}
		}else{
			m_Arith.l_print( dp, m_pSnHonpyoData->Sn_ZZURI, FMT16_EX );
			PF1[i].PITM[0] = dp;
			dp += 19;
		}
		i++;

		// á@ãyÇ—áAÇÃì‡ñÛÅ@
		//â€ê≈ïWèÄäz
		if( !FontFlg ){//ïÅí ÇÃÉtÉHÉìÉgÇ≈èoÇ∑èÍçá
			int t = 0;
			for( j=0; j<4; j++, dp+=16 ){
				switch( j ) {
					case 0 :
						m_Arith.l_div( WORK0, JANS[0].val, PW0 );
						m_Arith.l_print( dp, WORK0, FMT15 );
						t = 0;
						break;
					case 1 :
						m_Arith.l_div( WORK0, JANS[1].val, PW0 );
						m_Arith.l_print( dp, WORK0, FMT15 );
						t = 1;
						break;
					case 2 :
						m_Arith.l_div( WORK0, JANS[2].val, PW0 );
						m_Arith.l_print( dp, WORK0, FMT15 );
						t = 2;
						break;
					case 3 ://Ç∆ÇËÇ†Ç¶Ç∏2Ç∆ìØÇ∂
						m_Arith.l_print( dp, KZMY, FMT15 );
						i++;
						t = 0;
						break;
					default:
						break;
				}
				PF1[i].PITM[t] = dp;
			}
			i++;
			//è¡îÔê≈äz
			for( j=0; j<4; j++, dp+=18 ){
				switch( j ) {
					case 0 :
						m_Arith.l_print( dp, JANS[3].val, FMT15_EX );
						break;
					case 1 :
						m_Arith.l_print( dp, JANS[4].val, FMT15_EX );
						break;
					case 2 :
						m_Arith.l_print( dp, JANS[5].val, FMT15_EX );
						break;
					case 3 ://Ç∆ÇËÇ†Ç¶Ç∏2Ç∆ìØÇ∂
						m_Arith.l_print( dp, SZMY, FMT15_EX );
						break;
					default:
						break;
				}
				PF1[i].PITM[j] = dp;
			}
			i++;
			//áPñîÇÕáQÇÃì‡ñÛ
			//ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
			for( j=0; j<3; j++, dp+=18 ){
				switch( j ) {
					case 0 :
						m_Arith.l_print( dp, JANS[6].val, FMT15_EX );
						break;
					case 1 :
						m_Arith.l_print( dp, JANS[7].val, FMT15_EX );
						break;
					case 2 :
						m_Arith.l_print( dp, SZMY2, FMT15_EX );
						break;
					default:
						break;
				}
				PF1[i].PITM[j] = dp;
			}
		}
		else{
			pbclr( 0, 3, &PF1[i++] );
			pbclr( 0, 1, &PF1[i++] );
			pbclr( 0, 4, &PF1[i++] );
			pbclr( 0, 3, &PF1[i] );
		}
	}
	else{
		if( !FontFlg ){//ïÅí ÇÃÉtÉHÉìÉgÇ≈èoÇ∑èÍçá
			//â€ê≈ïWèÄäz
			for( j=0; j<3; j++, dp+=16 ){
				switch( j ) {
					case 0 :
						m_Arith.l_div( WORK0, JANS[0].val, PW0 );
						m_Arith.l_print( dp, WORK0, FMT11 );
						strcat_s( dp, 16+1, "    " );
						break;
					case 1 :
						m_Arith.l_div( WORK0, JANS[1].val, PW0 );
						m_Arith.l_print( dp, WORK0, FMT11 );
						strcat_s( dp, 16+1, "    " );
						break;
					case 2 ://Ç∆ÇËÇ†Ç¶Ç∏1Ç∆ìØÇ∂
						m_Arith.l_div( WORK0, JANS[2].val, PW0 );
						m_Arith.l_print( dp, WORK0, FMT11 );
						strcat_s( dp, 16+1, "    " );
						break;
					default:
						break;
				}
				PF1[i].PITM[j] = dp;
			}
			i++;
			//è¡îÔê≈äz
			for( j=0; j<3; j++, dp+=16 ){
				switch( j ) {
					case 0 :
						m_Arith.l_print( dp, JANS[3].val, FMT15 );
						break;
					case 1 :
						m_Arith.l_print( dp, JANS[4].val, FMT15 );
						break;
					case 2 ://Ç∆ÇËÇ†Ç¶Ç∏1Ç∆ìØÇ∂
						m_Arith.l_print( dp, JANS[5].val, FMT15 );
						break;
					default:
						break;
				}
				PF1[i].PITM[j] = dp;
			}
			i++;
			//áPñîÇÕáQÇÃì‡ñÛ
			//ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
			for( j=0; j<2; j++, dp+=16 ){
				switch( j ) {
					case 0 :
						m_Arith.l_print( dp, JANS[6].val, FMT15 );
						break;
					case 1 :
						m_Arith.l_print( dp, JANS[7].val, FMT15 );
						break;
					default:
						break;
				}
				PF1[i].PITM[j] = dp;
			}
		}
		else{
			i++;
			i++;
		}
	}
	pbclr( 0, 2, &PF1[++i] );
	pbclr( 0, 1, &PF1[i+1] );
	pbclr( 0, 1, &PF1[i+2] );
	pbclr( 0, 1, &PF1[i+3] );
	pbclr( 0, 1, &PF1[i+4] );

	if( (m_Arith.l_test(ANS[25].val)< 0) || m_pPrintCmInfo->OPI ){	// ä“ïtã‡óZã@ä÷
		if( !m_pSnHeadData->Sn_BYsw ){			// ã‚çs
			memmove( dp, m_pSnHeadData->Sn_BANK, 10*2 );
			PF1[i].PITM[0] = dp;
			dp += 21;

			memmove( dp, m_pSnHeadData->Sn_SITEN, 10*2 );
			PF1[i++].PITM[1] = dp;
			dp += 21;
			
			memmove( dp, m_pSnHeadData->Sn_YOKIN, 4*2 );
			PF1[i++].PITM[0] = dp;	
			dp += 9;
			
			memset( buf, '\0', sizeof( buf ) );
			m_Arith.l_unpac( buf, m_pSnHeadData->Sn_KOZA, 10 );
			for( j=(int)strlen(buf); j>0; ){
				if( buf[--j] != 0x20 ){
					break;
				}
			}
			++j;

			if( j < 10 ){
				memmove( dp, "          ", 10-j );
			}
			// å˚ç¿î‘çÜ
			memmove( (dp+(10-j)), buf, j );	
			PF1[i++].PITM[0] = dp;
			dp += 11;
		}
		else{
			i += 3;
			memmove( dp, m_pSnHeadData->Sn_YUBIN, 8*2 );
			PF1[i++].PITM[0] = dp;
			dp += 17;
			
			memset( buf, '\0', sizeof( buf ) );
			memmove( buf, m_pSnHeadData->Sn_YUNUM1, 5 );
			j = (int)strlen( buf );
			memset( buf, '\0', sizeof( buf ) );
			memmove( buf, m_pSnHeadData->Sn_YUNUM2, 8 );
			k = (int)strlen( buf );
			if( j || k ){
				strcpy_s( buf, 8+1, "        " );											//?
				if( j ){
					memmove( &buf[0], m_pSnHeadData->Sn_YUNUM1, j );
				}
				if( k ){
					memmove( &buf[((m_pPrintCmInfo->PRtype&0x80) ? 7 :8)], m_pSnHeadData->Sn_YUNUM2, k );
				}
				dp = &buf[0];
			}
			PF1[i++].PITM[0] = dp;
		}
	}

	if( j = _xprn( PP, 3, PF1, 0, pDC, 0, SEno ) ){
		goto POUTn;
	}

	dp = AP;
	i = j = 0;
	memset( dp, '\0', size );
	pbclr( 0, 1, &PF1[i] );

	// êÆóùî‘çÜìôèoóÕÉTÉCÉì
	if( (m_pPrintCmInfo->PRtype&0x80) || !m_pPrintCmInfo->OP1 ){		
		if( (m_pSnHeadData->Sn_ZSEIRI[0]&0xff) != 0xff ) {
			char	tmpbuf[512] = {0};
			memset( tmpbuf, '\0', sizeof(tmpbuf) );
			if( (m_pSnHeadData->Sn_ZSEIRI[3]&0x0f) != 0x0f ){
				j = 8;
			}
			else{
				j = 7;
			}
			l_unpac( tmpbuf, m_pSnHeadData->Sn_ZSEIRI, j );
			if( j == 8 ){				// 8åÖì¸óÕÇÃèÍçá:êÊì™0ÇÕÉJÉbÉg
				if( tmpbuf[0] != 0x30 ){
					tmpbuf[7] = '\0';		// à»äOÇÕå„1åÖÉJÉbÉg
				}
				else{
					for( j = 0; j < 7; ++j ){
						tmpbuf[j] = tmpbuf[j+1];
					}
					tmpbuf[j] = '\0';
				}
				j = 7;
			}
			for( j = (int)strlen( tmpbuf ); j > 0; ){
				if( tmpbuf[--j] != 0x20 ){
					break;
				}
			}
			++j;

			// ê≈ñ±èêêÆóùî‘çÜ
			memset( buf, '\0', sizeof( buf ) );
			memmove( buf, "        ", 8 );
			memmove( &buf[7-j], tmpbuf, j );
			memmove( dp, &buf[0], 8 );
			PF1[i].PITM[0] = dp;
			dp += 9;
		}
	}
	
	// ê≈óùémñºèoóÕ
	for( ++i, j=0, k=0; j<2; ++j ){
		pbclr( 0, 1, &PF1[i+j] );
	}
	Zlng = Zsw = 0;
	memset( Znam, '\0', sizeof(Znam) );
	memset( Smei, '\0', sizeof(Smei) );

	int	SmlFontZrtype = 0;	// 0:í èÌ, 1:ëSäp20ï∂éöÇÊÇËëÂ

	if( !m_pPrintCmInfo->OP2 ){
		if( (m_pPrintCmInfo->OPA==1) && (m_pPrintCmInfo->OPsign&0x01) ){
			// ê≈óùéméÅñºÇéËèëÇ´Ç∑ÇÈà◊Ç…ÅAéññ±èêñºÅïê≈óùémñ@êlñºÇèkè¨Ç≈àÛç¸
			// ÉtÉBÅ[ÉãÉh No.13Ç≈àÛç¸
			if( (j = (int)strlen(m_pShinInfo->pO_DATA->o_ofcnam)) > 40 ){
				j = 40;
			}
			memmove( Znam, m_pShinInfo->pO_DATA->o_ofcnam, j );
			Zlng = m_Util.MixSpaceCutLength( Znam, j );
			memmove( dp, "Å@", 2 );
			PF1[i].PITM[0] = dp;
			dp += (2 + 1);
		}
		else{
			if( m_pPrintCmInfo->OPA != 2 ){
				// ê≈óùéméÅñºñîÇÕê≈óùémñ@êlñº
				MakeOutZeirishiName( Znam, sizeof(Znam) );
				j = (int)strlen( Znam );
				Zlng = m_Util.MixSpaceCutLength( Znam, j );

				if( Zlng > 40 ){
					memmove( dp, "Å@", 2 );
					PF1[i].PITM[0] = dp;
					dp += (2 + 1);
					SmlFontZrtype = 1;
				}
				else if( Zlng > 24 ){
					memmove( dp, Znam, Zlng );
					PF1[i].PITM[0] = dp;
					dp += (j + 1);
				}
				else{					// ÇPÇQï∂éöà»â∫ÇÕëSäpÇ≈ÉtÉBÅ[ÉãÉh No.11Ç≈àÛç¸
					memmove( dp, "Å@", 2 );
					PF1[i].PITM[0] = dp;
					dp += (2 + 1);
					Zsw = 1;
				}
			}
		}
	}

	// ê≈óùémìdòbî‘çÜ
	if( !m_pPrintCmInfo->OP9 ){
		for( j=0; j<19; ++j ){
			buf[j] = 0x20;
		}
		buf[j] = '\0';
		for( ++i, j=k=0; (j<30)||m_pShinInfo->pO_DATA->o_telno[j]; ++j ){
			if( (m_pShinInfo->pO_DATA->o_telno[j]=='-') || (m_pShinInfo->pO_DATA->o_telno[j]=='∞') ||
				(m_pShinInfo->pO_DATA->o_telno[j]=='(') || (m_pShinInfo->pO_DATA->o_telno[j]==')') ){
				switch( k ){
					case 0 :
						k = ( (j < 6) ? j : 6);
						memmove( &buf[6-k], &m_pShinInfo->pO_DATA->o_telno[0], k );
						k = 1;	h = j + 1;
						continue;
					case 1 :
						if( (j-h) < 5 ){
							k = (j - h);
						}
						else{
							k = 5;
						}
						memmove( &buf[8],  &m_pShinInfo->pO_DATA->o_telno[h],   k );
						memmove( &buf[14], &m_pShinInfo->pO_DATA->o_telno[++j], 5 );
						k = 2;
						break;
					default:
						break;
				}
				if( k > 1 ){
					break;
				}
			}
		}
		if( !k ){
			memmove( buf, &m_pShinInfo->pO_DATA->o_telno[0], (j > 19 ? 19 : j) );
		}
		else{
			if( k == 1 ){
				memmove( &buf[8],  &m_pShinInfo->pO_DATA->o_telno[h], 4 );
			}
		}
		memmove( dp, buf, 19 );
		
		PF1[i].PITM[0] = dp;
		dp += 20;
	}
	
	if( j = _xprn( PP, 4, PF1, 0, pDC, 0, SEno ) ){
		goto POUTn;
	}

	// ä“ïtÇéÛÇØÇÊÇ§Ç∆Ç∑ÇÈã‡óZä˙ä‘	
	dp = AP;
	i = j = 0;
	memset( dp, '\0', size );

	pbclr( 0, 1, &PF1[ i ] );
	pbclr( 0, 1, &PF1[i+1] );
	pbclr( 0, 1, &PF1[i+2] );
	pbclr( 0, 1, &PF1[i+3] );
	pbclr( 0, 1, &PF1[i+4] );
	pbclr( 0, 1, &PF1[i+5] );
	pbclr( 0, 1, &PF1[i+6] );
	pbclr( 0, 1, &PF1[i+7] );
	pbclr( 0, 1, &PF1[i+8] );
	pbclr( 0, 1, &PF1[i+9] );

	if( (m_Arith.l_test(ANS[25].val) < 0) || m_pPrintCmInfo->OPI ){	// ä“ïtã‡óZã@ä÷
		if( !m_pSnHeadData->Sn_BYsw ){			// ã‚çs
			switch( m_pSnHeadData->Sn_FINANCE&0x0f ){
//2017.12.04 UPDATE START
				//case 0 :
				//	PF1[ i ].PITM[0] = "Åù";
				//	break;
				//case 1 :
				//	PF1[i+1].PITM[0] = "Åù";
				//	break;
				//case 2 :
				//	PF1[i+2].PITM[0] = "Åù";
				//	break;
				//case 3 :
				//	PF1[i+3].PITM[0] = "Åù";
				//	break;
				//case 4 :	
				//	PF1[i+4].PITM[0] = "Åù";
				//	break;

				case 0 :
					PF1[ i ].PITM[0] = " ";
					break;
				case 1 :
					PF1[i+1].PITM[0] = " ";
					break;
				case 2 :
					PF1[i+2].PITM[0] = " ";
					break;
				case 3 :
					PF1[i+3].PITM[0] = " ";
					break;
				case 4 :	
					PF1[i+4].PITM[0] = " ";
					break;

//2017.12.04 UPDATE END
				default:
					break;
			}

			switch( m_pSnHeadData->Sn_CHAIN & 0x0f ){
//2017.12.04 UPDATE START
				//case 0 :
				//	PF1[i+5].PITM[0] = "Åù";
				//	break;
				//case 1 :
				//	PF1[i+6].PITM[0] = "Åù";
				//	break;
				//case 2 :	
				//	PF1[i+7].PITM[0] = "Åù";
				//	break;
				//case 3 :
				//	PF1[i+8].PITM[0] = "Åù";
				//	break;
				//case 5 :
				//	PF1[i+9].PITM[0] = "Åù";
				//	break;

				case 0 :
					PF1[i+5].PITM[0] = " ";
					break;
				case 1 :
					PF1[i+6].PITM[0] = " ";
					break;
				case 2 :	
					PF1[i+7].PITM[0] = " ";
					break;
				case 3 :
					PF1[i+8].PITM[0] = " ";
					break;
				case 5 :
					PF1[i+9].PITM[0] = " ";
					break;

//2017.12.04 UPDATE END
				default:
					break;
			}
		}
	}

	if( j = _xprn( PP, 5, PF1, 0, pDC, 0, SEno ) ){
		goto POUTn;
	}

	// óìäOàÛéöçÄñ⁄èoóÕ
	dp = AP;
	i = j = 0;
	memset( dp, '\0', size );

	pbclr( 0, 1, &PF1[ i ] );
	pbclr( 0, 1, &PF1[i+1] );
	pbclr( 0, 1, &PF1[i+2] );
	pbclr( 0, 1, &PF1[i+3] );

	pbclr( 0, 1, &PF1[i+4] );		// å¬êl	â∫íiá@
	pbclr( 0, 1, &PF1[i+5] );		// ÅV	å¬êl
	pbclr( 0, 1, &PF1[i+6] );		// ÅV	è„íi
	pbclr( 0, 1, &PF1[i+7] );		// ÅV	â∫íi
	pbclr( 0, 1, &PF1[i+8] );		// ÅV	ê≈ñ±èêèàóùóìÅ@ãÊï™ÇR
	// ägí£		
	pbclr( 0, 1, &PF1[i+9] );		// ñ@êl	â∫íi
	pbclr( 0, 1, &PF1[i+10] );		// ÅVÅ@	èêï àÍòAî‘çÜ
	// èää«
	pbclr( 0, 1, &PF1[i+11] );

	// óìäOâ∫ägí£
	char	ADRDX[128], ADRD2[20], ADRD4[50];
	int		ln2, ln4;
	memset( ADRDX, '\0', sizeof( ADRDX ) );
	memset( ADRDX, 0x20, 60 );
	memset( ADRD2, '\0', sizeof( ADRD2 ) );
	memmove( ADRD2, m_pSnHeadData->Sn_ADRD2, 16 );
	memset( ADRD4, '\0', sizeof( ADRD4 ) );
	memmove( ADRD4, m_pSnHeadData->Sn_ADRD4, 44 );;
	ln2 = (int)strlen( ADRD2 );
	ln4 = (int)strlen( ADRD4 );
	memmove( ADRDX, ADRD4, ln4 );
	memmove( &ADRDX[44], ADRD2, ln2 );

	if( !m_pPrintCmInfo->OP7 && (m_pSnHeadData->IsSoleProprietor()==FALSE) ){
		memmove( dp, m_pSnHeadData->Sn_ADRU1, 30 );
		PF1[ i ].PITM[0] = dp;
		dp += 32;
		
		memmove( dp, m_pSnHeadData->Sn_ADRU2, 20 );
		PF1[i+1].PITM[0] = dp;
		dp += 22;

		memmove( dp, m_pSnHeadData->Sn_ADRC1, 20 );
		PF1[i+2].PITM[0] = dp;	
		dp += 22;

		memmove( dp, m_pSnHeadData->Sn_ADRC2, 20 );
		PF1[i+3].PITM[0] = dp;
		dp += 22;

		if( m_pSnHeadData->IsSoleProprietor() ){
			memmove( dp, ADRDX, 60 );
			PF1[i+4].PITM[0] = dp;
			dp += 62;

			if( m_pSnHeadData->Sn_PPTYP&0x01 )	{
				memmove( dp, "å¬êl", 4 );
				PF1[i+5].PITM[0] = dp;
				dp += 6;
			}
			memmove( dp, m_pSnHeadData->Sn_ADKUP, 10 );
			PF1[i+6].PITM[0] = dp;
			dp += 12;

			memmove( dp, m_pSnHeadData->Sn_ADKDW, 10 );
			PF1[i+7].PITM[0] = dp;	
			dp += 12;

			memmove( dp, m_pSnHeadData->Sn_ADKNO, 1 );
			PF1[i+8].PITM[0] = dp;	
			dp += 2;
		}
		else{
			memmove( dp, ADRDX, 60 );
			PF1[i+9].PITM[0] = dp;
			dp += 62;
			memmove( dp, m_pSnHeadData->Sn_ADSNO, 7 );
			PF1[i+10].PITM[0] = dp;	
			dp += 9;
		}
	}

	if( (m_pPrintCmInfo->PRtype&0x80) || !m_pPrintCmInfo->OP1 ){		// êÆóùî‘çÜìôèoóÕÉTÉCÉì
		memmove( dp, m_pSnHeadData->Sn_SYOKN, 3 );
		PF1[i+11].PITM[0] = dp;	
		dp += 5;
	}

	if( j = _xprn( PP, 9, PF1, 0, pDC, 0, SEno ) ){
		goto POUTn;
	}

	// ê≈óùémñ@Å@é{çså„í†ï[
	dp = AP;
	i = j = 0;
	memset( dp, '\0', size );
	pbclr( 0, 1, &PF1[ 0 ] );
	pbclr( 0, 1, &PF1[ 1 ] );

	if( m_pSnHeadData->Sn_ZeiHou&0x01 ){
		PF1[0].PITM[0] = "Åõ";
	}
	else{
		PF1[0].PITM[0] = (char *)0;
	}

	if( m_pSnHeadData->Sn_ZeiHou&0x02 ){
		PF1[1].PITM[0] = "Åõ";
	}
	else{
		PF1[1].PITM[0] = (char *)0;
	}
	// å¬êlâ¸ê≥í†ï[ëŒâû [05'01.06]
	if( m_pSnHeadData->IsSoleProprietor() ){		// å¬êl
		// àÍòAî‘çÜ
		if( (m_pSnHeadData->Sn_ITIREN[0]&0xff) != 0xff ){
			memset( buf, '\0', sizeof( buf ) );
			m_Arith.l_unpac( buf, m_pSnHeadData->Sn_ITIREN, 8 );
			for( j=(int)strlen(buf); j>0; ){
				if( buf[--j] != 0x20 ){
					break;
				}
			}
			++j;
			if( j < 8 ){
				memmove( dp, "        ", 8-j );
			}
			memmove( (dp+(8-j)), buf, j );
			PF1[2].PITM[0] = dp;
			dp += 9;
		}
		else{
			PF1[2].PITM[0] = (char *)0;
		}

		// ëóït
//-- '18.12.18 --
//		if( (m_pSnHeadData->Sn_ZeiHou & 0x04) ){
//			PF1[3].PITM[0] = "Åõ";	
//		}
//		else{
//			PF1[3].PITM[0] = (char *)0;
//		}
//---------------
		if( IsNoDeliForm() ){
			PF1[3].PITM[0] = (char *)0;
		}
		else{
			if( (m_pSnHeadData->Sn_ZeiHou & 0x04) ){
				PF1[3].PITM[0] = "Åõ";	
			}
			else{
				PF1[3].PITM[0] = (char *)0;
			}
		}
//---------------
	}
	else{
		PF1[2].PITM[0] = (char *)0;

		// ëóït
//-- '18.12.18 --
//		if( (m_pSnHeadData->Sn_ZeiHou & 0x04) ){
//			PF1[3].PITM[0] = "Åõ";	
//		}
//		else{
//			PF1[3].PITM[0] = (char *)0;
//		}
//---------------
		if( IsNoDeliForm() ){
			PF1[3].PITM[0] = (char *)0;
		}
		else{
			if( (m_pSnHeadData->Sn_ZeiHou & 0x04) ){
				PF1[3].PITM[0] = "Åõ";	
			}
			else{
				PF1[3].PITM[0] = (char *)0;
			}
		}
//---------------
	}

	if( j = _xprn( PP, 10, PF1, 0, pDC, 0, SEno ) ){
		goto POUTn;
	}

	if( !m_pPrintCmInfo->OP2 ){
		if( (m_pPrintCmInfo->OPA==1) && (m_pPrintCmInfo->OPsign&0x01) ){
			// éññ±èêñºÅïê≈óùémñ@êlñºÇèkè¨Ç≈àÛç¸
			// ê≈óùémñ@ é{çså„
			pbclr( 0, 1, &PF1[0] );
			if( j = _xprn( PP, 11, PF1, 0, pDC, 0, SEno ) ){
				goto POUTn;
			}
			// ê≈óùémñ@Å@é{çsëO
			pbclr( 0, 1, &PF1[0] );
			if( j = _xprn( PP, 12, PF1, 0, pDC, 0, SEno ) ){
				goto POUTn;
			}
			// ê≈óùémñ@ é{çså„
			pbclr( 0, 1, &PF1[0] );
			memmove( dp, Znam, Zlng );
			PF1[0].PITM[0] = dp;
			dp += (Zlng + 1);
			if( j = _xprn( PP, 13, PF1, 0, pDC, 0, SEno ) ){
				goto POUTn;
			}
			// ê≈óùémñ@Å@é{çsëO
			pbclr( 0, 1, &PF1[0] );
			if( j = _xprn( PP, 14, PF1, 0, pDC, 0, SEno ) ){
				goto POUTn;
			}
			// ê≈óùémñ@Å@é{çså„ í«â¡ÅI
			pbclr( 0, 1, &PF1[0] );
			if( j = _xprn( PP, 15, PF1, 0, pDC, 0, SEno ) ){
				goto POUTn;
			}
		}
		else{
			if( m_pPrintCmInfo->OPA == 2 ){
				// éññ±èêñºÅïê≈óùémñ@êlñºÇèkè¨Ç≈àÛç¸
				// ê≈óùémñ@ é{çså„
				pbclr( 0, 1, &PF1[0] );
				if( j = _xprn( PP, 11, PF1, 0, pDC, 0, SEno ) ){
					goto POUTn;
				}
				// ê≈óùémñ@Å@é{çsëO
				pbclr( 0, 1, &PF1[0] );
				if( j = _xprn( PP, 12, PF1, 0, pDC, 0, SEno ) ){
					goto POUTn;
				}
				// ê≈óùémñ@ é{çså„
				if( (Zlng = (int)strlen( m_pShinInfo->pO_DATA->o_ofcnam )) > 40 ){
					Zlng = 40;
				}
				memmove( Znam, m_pShinInfo->pO_DATA->o_ofcnam, Zlng );
				pbclr( 0, 1, &PF1[0] );
				memmove( dp, Znam, Zlng );
				PF1[0].PITM[0] = dp;
				dp += (Zlng + 1);
				if( j = _xprn( PP, 13, PF1, 0, pDC, 0, SEno ) ){
					goto POUTn;
				}
				// ê≈óùémñ@Å@é{çsëO
				pbclr( 0, 1, &PF1[0] );
				if( j = _xprn( PP, 14, PF1, 0, pDC, 0, SEno ) ){
					goto POUTn;
				}
				// ê≈óùémñ@Å@é{çså„Å@ê≈óùéméÅñº
				MakeOutZeirishiName( Smei, sizeof(Smei) );
				Zlng = (int)strlen(Smei);
				if( Zlng > 40 ){
					memset( Znam, '\0', sizeof(Znam) );
					memmove( Znam, Smei, Zlng );
					pbclr( 0, 1, &PF1[0] );
					SmlFontZrtype = 2;
				}
				else{
					pbclr( 0, 1, &PF1[0] );
					memmove( dp, Smei, Zlng );
					PF1[0].PITM[0] = dp;
					dp += (Zlng + 1);
				}
				if( j = _xprn( PP, 15, PF1, 0, pDC, 0, SEno ) ){
					goto POUTn;
				}
			}
			else{
				if( Zsw ){
					// ê≈óùémñ@ é{çså„
					pbclr( 0, 1, &PF1[0] );
					memmove( dp, Znam, Zlng );
					PF1[0].PITM[0] = dp;
					dp += (Zlng + 1);
					if( j = _xprn( PP, 11, PF1, 0, pDC, 0, SEno ) ){
						goto POUTn;
					}
					// ê≈óùémñ@Å@é{çsëO
					pbclr( 0, 1, &PF1[0] );
					if( j = _xprn( PP, 12, PF1, 0, pDC, 0, SEno ) ){
						goto POUTn;
					}
					// ê≈óùémñ@ é{çså„
					pbclr( 0, 1, &PF1[0] );
					if( j = _xprn( PP, 13, PF1, 0, pDC, 0, SEno ) ){
						goto POUTn;
					}
					// ê≈óùémñ@Å@é{çsëO
					pbclr( 0, 1, &PF1[0] );
					if( j = _xprn( PP, 14, PF1, 0, pDC, 0, SEno ) ){
						goto POUTn;
					}
					// ê≈óùémñ@Å@é{çså„ í«â¡ÅI
					pbclr( 0, 1, &PF1[0] );
					if( j = _xprn( PP, 15, PF1, 0, pDC, 0, SEno ) ){
						goto POUTn;
					}
				}
			}
		}
	}

	if( m_pPrintCmInfo->OPM ){
		dp = AP;
		i = j = 0;
		memset( dp, '\0', size );
		pbclr( 0, 1, &PF1[ 0 ] );
		pbclr( 0, 1, &PF1[ 1 ] );
	
		// ñ@êlñº
		if( !m_pPrintCmInfo->OP4 ){
			memmove( dp, m_pSnHeadData->Sn_CONAM, 20*2 );
			PF1[0].PITM[0] = dp;
			dp += 41;
		}
		memmove( dp, m_pSnHeadData->Sn_GPNAM, 20*2 );
		PF1[1].PITM[0] = dp;
		dp += 41;
		if( j = _xprn( PP, 16, PF1, 0, pDC, 0, SEno ) ){
			goto POUTn;
		}
	}

	i = 0;
	if( FontFlg ){//è¨Ç≥Ç¢ÉtÉHÉìÉgÇ≈èoÇ∑èÍçá
		// á@ãyÇ—áAÇÃì‡ñÛÅ@
		//â€ê≈ïWèÄäz
		int t = 0;
		if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){
			for( j=0; j<4; j++, dp+=16 ){
				switch( j ) {
					case 0 :
						m_Arith.l_div( WORK0, JANS[0].val, PW0 );
						m_Arith.l_print( dp, WORK0, FMT15 );
						t = 0;
						break;
					case 1 :
						m_Arith.l_div( WORK0, JANS[1].val, PW0 );
						m_Arith.l_print( dp, WORK0, FMT15 );
						t = 1;
						break;
					case 2 :
						m_Arith.l_div( WORK0, JANS[2].val, PW0 );
						m_Arith.l_print( dp, WORK0, FMT15 );
						t = 2;
						break;
					case 3 ://Ç∆ÇËÇ†Ç¶Ç∏2Ç∆ìØÇ∂
						m_Arith.l_print( dp, KZMY, FMT15 );
						i++;
						t = 0;
						break;
					default:
						break;
				}
				PF1[i].PITM[t] = dp;
			}
			i++;
			//è¡îÔê≈äz
			for( j=0; j<4; j++, dp+=18 ){
				switch( j ) {
					case 0 :
						m_Arith.l_print( dp, JANS[3].val, FMT15_EX );
						break;
					case 1 :
						m_Arith.l_print( dp, JANS[4].val, FMT15_EX );
						break;
					case 2 :
						m_Arith.l_print( dp, JANS[5].val, FMT15_EX );
						break;
					case 3 ://Ç∆ÇËÇ†Ç¶Ç∏2Ç∆ìØÇ∂
						m_Arith.l_print( dp, SZMY, FMT15_EX );
						break;
					default:
						break;
				}
				PF1[i].PITM[j] = dp;
			}
			i++;
			//áPñîÇÕáQÇÃì‡ñÛ
			//ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
			for( j=0; j<3; j++, dp+=18 ){
				switch( j ) {
					case 0 :
						m_Arith.l_print( dp, JANS[6].val, FMT15_EX );
						break;
					case 1 :
						m_Arith.l_print( dp, JANS[7].val, FMT15_EX );
						break;
					case 2 :
						m_Arith.l_print( dp, SZMY2, FMT15_EX );
						break;
					default:
						break;
				}
				PF1[i].PITM[j] = dp;
			}
		}
		else{
			//â€ê≈ïWèÄäz
			for( j=0; j<3; j++, dp+=16 ){
				switch( j ) {
					case 0 :
						m_Arith.l_div( WORK0, JANS[0].val, PW0 );
						m_Arith.l_print( dp, WORK0, FMT11 );
						strcat_s( dp, 16+1, "    " );
						break;
					case 1 :
						m_Arith.l_div( WORK0, JANS[1].val, PW0 );
						m_Arith.l_print( dp, WORK0, FMT11 );
						strcat_s( dp, 16+1, "    " );
						break;
					case 2 ://Ç∆ÇËÇ†Ç¶Ç∏1Ç∆ìØÇ∂
						m_Arith.l_div( WORK0, JANS[2].val, PW0 );
						m_Arith.l_print( dp, WORK0, FMT11 );
						strcat_s( dp, 16+1, "    " );
						break;
					default:
						break;
				}
				PF1[i].PITM[j] = dp;
			}
			i++;
			//è¡îÔê≈äz
			for( j=0; j<3; j++, dp+=16 ){
				switch( j ) {
					case 0 :
						m_Arith.l_print( dp, JANS[3].val, FMT15 );
						break;
					case 1 :
						m_Arith.l_print( dp, JANS[4].val, FMT15 );
						break;
					case 2 ://Ç∆ÇËÇ†Ç¶Ç∏1Ç∆ìØÇ∂
						m_Arith.l_print( dp, JANS[5].val, FMT15 );
						break;
					default:
						break;
				}
				PF1[i].PITM[j] = dp;
			}
			i++;
			//áPñîÇÕáQÇÃì‡ñÛ
			//ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz
			for( j=0; j<2; j++, dp+=16 ){
				switch( j ) {
					case 0 :
						m_Arith.l_print( dp, JANS[6].val, FMT15 );
						break;
					case 1 :
						m_Arith.l_print( dp, JANS[7].val, FMT15 );
						break;
					default:
						break;
				}
				PF1[i].PITM[j] = dp;
			}
		}
		if( j = _xprn( PP, 17, PF1, 0, pDC, 0, SEno ) ){
			goto POUTn;
		}
	}

	// ê≈óùémñºÇ≈ç≈è¨ÉtÉHÉìÉg
	dp = AP;
	memset( dp, '\0', size );
	i = j = 0;
	if( SmlFontZrtype == 1 ){
		memmove( dp, Znam, Zlng );
		PF1[i].PITM[0] = dp;
		dp += (Zlng + 1);
	}
	else{
		pbclr( 0, 1, &PF1[0] );
	}
	if( j = _xprn( PP, 18, PF1, 0, pDC, 0, SEno ) ){
		goto POUTn;
	}
	// éññ±èäñºÅ{ê≈óùémñºÇ≈ç≈è¨ÉtÉHÉìÉg
	dp = AP;
	memset( dp, '\0', size );
	i = j = 0;
	if( SmlFontZrtype == 2 ){
		memmove( dp, Znam, Zlng );
		PF1[i].PITM[0] = dp;
		dp += (Zlng + 1);
	}
	else{
		pbclr( 0, 1, &PF1[0] );
	}
	if( j = _xprn( PP, 19, PF1, 0, pDC, 0, SEno ) ){
		goto POUTn;
	}

	//äÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇÇè¨Ç≥Ç¢ÉtÉHÉìÉgÇ≈
	if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
		if( FontFlg2 == 1 ){
			dp = AP;
			memset( dp, '\0', size );
			memset( WORK0, '\0', sizeof(WORK0) );
			m_Arith.l_div( WORK0, m_pSnHonpyoData->Sn_ZZURI, PW0 );
			m_Arith.l_print( dp, WORK0, FMT14 );
			PF1[0].PITM[0] = dp;
			dp += 19;
			if( j = _xprn( PP, 20, PF1, 0, pDC, 0, SEno ) ){
				goto POUTn;
			}
		}
	}

//2018.03.13 INSERT START
	//èÄämíËÇÃèÍçáëIëìIÇ…ï∂åæÇèoóÕ
	if( m_pSnHeadData->Sn_Syuusei&0x10 ){
		if( !m_pPrintCmInfo->OPV ){
			dp = AP;
			memset( dp, '\0', size );
			PF1[0].PITM[0] = _T("îÌëäë±êl");
			if( j = _xprn( PP, 21, PF1, 0, pDC, 0, SEno ) ){
				goto POUTn;
			}
		}
	}
//2018.03.13 INSERT END

//-- '17.03.22 --
//	if(( m_ShRevType >= ID_ICSSH_MYNUMBER_REVTYPE ) && m_pSnHeadData->IsSoleProprietor()){
//		if( isHikae == TRUE ){
////2016.10.06 UPDATE START
////			m_pVprn->DrawSline( 0, LKnrW0, 3150, 5360, 9750, 4850 ); //å¬êlî‘çÜóìÇ…éŒê¸
//   			int yPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[0].HyoTn]._PIVad;
//			int xPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[0].HyoTn]._PIHad;
////2016.10.20 UPDATE START
////			m_pVprn->DrawSline( 0, LKnrW0, 3150+xPlus, 5360+yPlus, 9750+xPlus, 4850+yPlus ); //å¬êlî‘çÜóìÇ…éŒê¸
//			m_pVprn->DrawSline( 0, LKnrW0, 3650+xPlus, 5360+yPlus, 9750+xPlus, 4850+yPlus ); //å¬êlî‘çÜóìÇ…éŒê¸
////2016.10.20 UPDATE END
////2016.10.06 UPDATE END
//		}
//	}
//---------------
	// å¬êlî‘çÜóìÇ…éŒê¸
	if( (m_ShRevType>=ID_ICSSH_MYNUMBER_REVTYPE) && m_pSnHeadData->IsSoleProprietor() ){
		if( isHikae ){
   			int yPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[0].HyoTn]._PIVad;
			int xPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[0].HyoTn]._PIHad;
			m_pVprn->DrawSline( 0, LKnrW0, 3650+xPlus, 5360+yPlus, 9750+xPlus, 4850+yPlus );
		}
	}
//---------------

	// ìdéqê\çêäÆóπçœ 
   if(*m_pShinInfo->pFinish && *m_pShinInfo->pFinishSw && strlen( m_pShinInfo->pFinishPrn) && isHikae ){
   		int yPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[0].HyoTn]._PIVad;
		int xPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[0].HyoTn]._PIHad;
		long	PH  = 2540L / 10L;		// 1 character
		long	PV1 = 2540L / 6L;		// 1/6 LF
  		long YPosition = ( PV1 * 1L ) + (PV1 / 4) + yPlus;
		long XPosition = ( PH *  4L ) + xPlus;
		int	Gothic095 = m_pVprn->RegisterFont( 90, 0, 0, "ÇlÇrÅ@ÉSÉVÉbÉN" );
		int	Gothic108 = m_pVprn->RegisterFont( MAKELONG( 108, 1 ), 0, 0, "ÇlÇrÅ@ÉSÉVÉbÉN" );
		struct	_SF_PAR		SfPar;
		memset( (char *)&SfPar, '\0', sizeof( struct _SF_PAR ) );
		SfPar.sf_fcsg =	( SFp_xl | SFp_yb );					// ÉtÉ@ÉìÉNÉVÉáÉìÉTÉCÉì
		SfPar.sf_hvsg = 0;										// èoóÕï˚å¸
		SfPar.sf_xfld = (11L * PH);								// â°èoóÕóÃàÊïù
		SfPar.sf_yfld =	PV1;									// ècèoóÕóÃàÊïù
		SfPar.sf_mrgn[0] = 0;									// óÃàÊì‡É}Å[ÉWÉìÅ@ç∂
		SfPar.sf_mrgn[1] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@âE
		SfPar.sf_mrgn[2] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@è„
		SfPar.sf_mrgn[3] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@â∫
		SfPar.sf_kpit = 0;										// ï∂éöÉsÉbÉ`ïùÅ@X0.1Œﬂ≤›ƒ
		SfPar.sf_ksiz =	Gothic108;								// ï∂éöÉTÉCÉYÅ@Å@X0.1Œﬂ≤›ƒ		
		SfPar.sf_erto = 0;										// ï∂éöägëÂèkè¨ó¶
		if( m_pVprn->DrawString( 0, XPosition, YPosition, _T("ìdéqê\çêçœ"), &SfPar ) ){
			goto POUTn;
		}

  		YPosition = PV1 + (PV1 / 4) + yPlus;
		XPosition = ( PH *  12L ) + xPlus;
		memset( (char *)&SfPar, '\0', sizeof( struct _SF_PAR ) );
		SfPar.sf_fcsg =	( SFp_xl | SFp_yb );					// ÉtÉ@ÉìÉNÉVÉáÉìÉTÉCÉì
		SfPar.sf_hvsg = 0;										// èoóÕï˚å¸
		SfPar.sf_xfld = (35L * PH);								// â°èoóÕóÃàÊïù
		SfPar.sf_yfld =	PV1;									// ècèoóÕóÃàÊïù
		SfPar.sf_mrgn[0] = 0;									// óÃàÊì‡É}Å[ÉWÉìÅ@ç∂
		SfPar.sf_mrgn[1] = 10;									// Å@Å@ÅVÅ@Å@Å@Å@Å@âE
		SfPar.sf_mrgn[2] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@è„
		SfPar.sf_mrgn[3] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@â∫
		SfPar.sf_kpit = 0;										// ï∂éöÉsÉbÉ`ïùÅ@X0.1Œﬂ≤›ƒ
		SfPar.sf_ksiz =	Gothic095;								// ï∂éöÉTÉCÉYÅ@Å@X0.1Œﬂ≤›ƒ		
		SfPar.sf_erto = 0;										// ï∂éöägëÂèkè¨ó¶
		if( m_pVprn->DrawString( 0, XPosition, YPosition, m_pShinInfo->pFinishPrn, &SfPar ) ){
			goto POUTn;
		}

		m_pVprn->DeleteRegFont( Gothic095 );
		m_pVprn->DeleteRegFont( Gothic108 );

   }

   // çTóp
   if( isHikae && m_pPrintCmInfo->OPD ){
		int yPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[0].HyoTn]._PIVad;
		int xPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[0].HyoTn]._PIHad;
		long	PH  = 2540L / 10L ;		// 1 character
		long	PV1 = 2540L / 6L;		// 1/6 LF
  		long YPosition = (PV1*2)+(PV1/2) + yPlus;
		long XPosition = (PH*56L)+(PH/3) + xPlus;
		int	Gothic108 = m_pVprn->RegisterFont( MAKELONG( 108, 1 ), 0, 0, "ÇlÇrÅ@ÉSÉVÉbÉN" );
		struct	_SF_PAR		SfPar;
		memset( (char *)&SfPar, '\0', sizeof( struct _SF_PAR ) );
		SfPar.sf_fcsg =	( SFp_xl | SFp_yb | SFp_yc );			// ÉtÉ@ÉìÉNÉVÉáÉìÉTÉCÉì
		SfPar.sf_hvsg = 0;										// èoóÕï˚å¸
		SfPar.sf_xfld = (8L * PH);								// â°èoóÕóÃàÊïù
		SfPar.sf_yfld =	PV1;									// ècèoóÕóÃàÊïù
		SfPar.sf_mrgn[0] = 0;									// óÃàÊì‡É}Å[ÉWÉìÅ@ç∂
		SfPar.sf_mrgn[1] = 10;									// Å@Å@ÅVÅ@Å@Å@Å@Å@âE
		SfPar.sf_mrgn[2] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@è„
		SfPar.sf_mrgn[3] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@â∫
		SfPar.sf_kpit = 0;										// ï∂éöÉsÉbÉ`ïùÅ@X0.1Œﬂ≤›ƒ
		SfPar.sf_ksiz =	Gothic108;								// ï∂éöÉTÉCÉYÅ@Å@X0.1Œﬂ≤›ƒ		
		SfPar.sf_erto = 0;										// ï∂éöägëÂèkè¨ó¶
		if( m_pVprn->DrawString( 0, XPosition, YPosition, _T("ÅyçTópÅz"), &SfPar ) ){
			goto POUTn;
		}

		m_pVprn->DeleteRegFont( Gothic108 );
	}

POUTn:
   if( AP ){
		delete [] AP;
		AP = NULL;
   }

	return j;
}

//-----------------------------------------------------------------------------
// ïtï\ÇPÅEÇSèoóÕ ê¥êÖï“èWÅõ
//-----------------------------------------------------------------------------
// à¯êî	pDC
//		kojin
//		pcnt 
//-----------------------------------------------------------------------------
// ï‘ëóíl
//-----------------------------------------------------------------------------
int CH26HyoView::PrintFuhyo10( CDC *pDC, int kojin, int pcnt )
{
	int				i, j;
	unsigned int	size;
	char			*AP, *dp, PW0[MONY_BUF_SIZE];
	
	BOOL	isNoImage = FALSE;
	if( m_pPrintCmInfo->PRtype&0x80 ){
		isNoImage = FALSE;
	}
	else{
		isNoImage = TRUE;
	}

	size = 512;
	if( (AP = new char[size]) == NULL ){
		return -1;
	}
	
	// èoóÕëOèWåv
//	FuHyo14Calqe();
	
	char	WORK0[MONY_BUF_SIZE] = {0};
	char	WORK1[MONY_BUF_SIZE] = {0};	
	char	WORK2[MONY_BUF_SIZE] = {0};
	char	WORK3[MONY_BUF_SIZE] = {0};
	char	WORK8[MONY_BUF_SIZE] = {0};
	char	WORK9[MONY_BUF_SIZE] = {0};
	//çáåvóÒÇ…ïKóvÇ»Ç‡ÇÃ
	char	GK_WORK7[MONY_BUF_SIZE] = {0};
	char	GK_WORK8[MONY_BUF_SIZE] = {0};
	char	GK_WORK9[MONY_BUF_SIZE] = {0};
	char	GK_WORK11[MONY_BUF_SIZE] = {0};
	char	GK_WORK12[MONY_BUF_SIZE] = {0};
	char	GK_WORK14[MONY_BUF_SIZE] = {0};
	char	GK_WORK15[MONY_BUF_SIZE] = {0};

	memset( GK_WORK7, '\0', MONY_BUF_SIZE );
	memset( GK_WORK8, '\0', MONY_BUF_SIZE );
	memset( GK_WORK9, '\0', MONY_BUF_SIZE );
	memset( GK_WORK11, '\0', MONY_BUF_SIZE );
	memset( GK_WORK12, '\0', MONY_BUF_SIZE );
	memset( GK_WORK14, '\0', MONY_BUF_SIZE );
	memset( GK_WORK15, '\0', MONY_BUF_SIZE );

	memset( PW0, '\0', MONY_BUF_SIZE );
	m_Arith.l_input( PW0, _T("1000") );

	memset( WORK0, '\0', MONY_BUF_SIZE );
	
	char WORK_KOUJYOZEI[MONY_BUF_SIZE] = {0};

PBUF	PF1[30];
int		PP;
#ifdef SHIMIZU_TEST
	CH26SnFhyo10Data*	PrintData;
	// ïtï\ÇSÉfÅ[É^ì«çûÇ›ÉNÉâÉX
	if( m_pSnFhyo10Data ){
		PrintData = m_pSnFhyo10Data;
	}
	else{
		PrintData = m_pSnFhyo40Data;
	}
#endif
	PP = 5;		//Åö//[12'09.28]

	dp = AP;
	memset( dp, '\0', size );
	i = 0;

int CCSw = 0;

	// ã≠êßâ€ê≈ä˙ä‘àÛç¸
	if( CCSw ){		// ã≠êßä˙ä‘ïœçX
//		wsprintf( dp, _T("%02X %02X %02X"), CC_KDAYS[0]&0xff, CC_KDAYS[1]&0xff, CC_KDAYS[2]&0xff );
//		PF1[i].PITM[0] = dp;
//		dp += 9;
//		wsprintf( dp, _T("%02X %02X %02X"), CC_KDAYE[0]&0xff, CC_KDAYE[1]&0xff, CC_KDAYE[2]&0xff );
//		PF1[i++].PITM[1] = dp;
//		dp += 9;
	}
	else{
		char	yymmdd[3] = {0};
		if( IsJcContinuePrint(true) ){
			m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		else{
			m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		wsprintf( dp, _T("%02X %02X %02X"), yymmdd[0]&0xff, yymmdd[1]&0xff, yymmdd[2]&0xff );
		PF1[i].PITM[0] = dp;
		dp += 9;

		memset( yymmdd, '\0', sizeof(yymmdd) );
		if( IsJcContinuePrint(true) ){
			m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		else{
			m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		wsprintf( dp, _T("%02X %02X %02X"), yymmdd[0]&0xff, yymmdd[1]&0xff, yymmdd[2]&0xff );
		PF1[i++].PITM[1] = dp;
		dp += 9;
	}
	memmove( dp, _T("Å@"), 2 );
	PF1[i++].PITM[0] = dp;
	dp += (2 + 1);

//ê≈ó¶3%ìKópï™Ç`
	j = 0;
	//Ç`-á@
	m_Arith.l_div( WORK0, PrintData->Sn_1F1A, PW0 );
	m_Arith.l_print( dp, WORK0, FMT110_EX );
	strcat_s( dp, 18+1, _T("   ") );
	PF1[i].PITM[j++] = dp;
	dp += 19;	
	i++;
	j = 0;
	//Ç`-áA
	m_Arith.l_print( dp, PrintData->Sn_1F2A, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ç`-áB
	m_Arith.l_print( dp, PrintData->Sn_1F3A, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ç`-áC
	m_Arith.l_print( dp, PrintData->Sn_1F4A,  FMT15_EX );
	m_Arith.l_add( WORK_KOUJYOZEI, WORK_KOUJYOZEI, PrintData->Sn_1F4A );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ç`-áD
	m_Arith.l_print( dp, PrintData->Sn_1F5A, FMT15_EX );
	m_Arith.l_add( WORK_KOUJYOZEI, WORK_KOUJYOZEI, PrintData->Sn_1F5A );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ç`-áE
	m_Arith.l_print( dp, PrintData->Sn_1F6A, FMT15_EX );
	m_Arith.l_add( WORK_KOUJYOZEI, WORK_KOUJYOZEI, PrintData->Sn_1F6A );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ç`-áFçTèúê≈äzè¨åv
	m_Arith.l_print( dp, WORK_KOUJYOZEI, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	m_Arith.l_add( GK_WORK7, GK_WORK7, WORK_KOUJYOZEI );//çáåvë´ÇµÇ±Ç›

	//Ç`-áGçTèúïsë´ä“ïtê≈äz
	memset( WORK0 , '\0', MONY_BUF_SIZE);
	m_Arith.l_sub( WORK0 , WORK_KOUJYOZEI , PrintData->Sn_1F2A );
	m_Arith.l_sub( WORK0 , WORK0 , PrintData->Sn_1F3A );

	if( m_Arith.l_test( WORK0 ) < 0 ){//É}ÉCÉiÉXÇÃèÍçá
		PF1[i].PITM[j++] = (char *)0;
		m_Arith.l_neg( WORK0 );
		m_Arith.l_print( dp, WORK0, FMT15_EX );
		PF1[i].PITM[j++] = dp;
		dp += 19;
		m_Arith.l_add( GK_WORK9, GK_WORK9, WORK0 );//çáåvë´ÇµÇ±Ç›
	}
	else{
		m_Arith.l_print( dp, WORK0, FMT15_EX );
		PF1[i].PITM[j++] = dp;
		dp += 19;
		PF1[i].PITM[j++] = (char *)0;
		m_Arith.l_add( GK_WORK8, GK_WORK8, WORK0 );//çáåvë´ÇµÇ±Ç›
	}

	//Ç`-áIÅ`áO
	PF1[i].PITM[j++] = (char *)0;
	PF1[i].PITM[j++] = (char *)0;
	PF1[i].PITM[j++] = (char *)0;
	PF1[i].PITM[j++] = (char *)0;
	PF1[i].PITM[j++] = (char *)0;
	PF1[i].PITM[j++] = (char *)0;
	PF1[i].PITM[j++] = (char *)0;

	if( j = _xprn( PP, 1, PF1, 0, pDC, isNoImage, 1 ) ){
		goto POUT1;
	}

//ê≈ó¶4%ìKópï™Ça
	dp = AP;
	i = j = 0;
	memset( dp, '\0', size );					// ê≈ó¶4%ìKópï™Ça á@->áD
	memset( WORK0, '\0', MONY_BUF_SIZE );
	memset( WORK8, '\0', MONY_BUF_SIZE );
	memset( WORK9, '\0', MONY_BUF_SIZE );
	memset( WORK_KOUJYOZEI , '\0', MONY_BUF_SIZE);
	//Ça-á@
	m_Arith.l_div( WORK0, PrintData->Sn_1F1B, PW0 );
	m_Arith.l_print( dp, WORK0, FMT110_EX );
	strcat_s( dp, 18+1, _T("   ") );
	PF1[i].PITM[j++] = dp;
	dp += 19;	

	i++;
	j = 0;
	//Ça-áA
	m_Arith.l_print( dp, PrintData->Sn_1F2B, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ça-áB
	m_Arith.l_print( dp, PrintData->Sn_1F3B, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ça-áC
	m_Arith.l_print( dp, PrintData->Sn_1F4B,  FMT15_EX );
	m_Arith.l_add( WORK_KOUJYOZEI, WORK_KOUJYOZEI, PrintData->Sn_1F4B );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ça-áD
	m_Arith.l_print( dp, PrintData->Sn_1F5B, FMT15_EX );
	m_Arith.l_add( WORK_KOUJYOZEI, WORK_KOUJYOZEI, PrintData->Sn_1F5B );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ça-áE
	m_Arith.l_print( dp, PrintData->Sn_1F6B, FMT15_EX );
	m_Arith.l_add( WORK_KOUJYOZEI, WORK_KOUJYOZEI, PrintData->Sn_1F6B );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ça-áFçTèúê≈äzè¨åv
	m_Arith.l_print( dp, WORK_KOUJYOZEI, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	m_Arith.l_add( GK_WORK7, GK_WORK7, WORK_KOUJYOZEI );//çáåvë´ÇµÇ±Ç›

	//Ça-áGçTèúïsë´ä“ïtê≈äz
	memset( WORK0 , '\0', MONY_BUF_SIZE);
	m_Arith.l_sub( WORK0 , WORK_KOUJYOZEI , PrintData->Sn_1F2B );
	m_Arith.l_sub( WORK0 , WORK0 , PrintData->Sn_1F3B );
	if( m_Arith.l_test( WORK0 ) < 0 ){//É}ÉCÉiÉXÇÃèÍçá
		PF1[i].PITM[j++] = (char *)0;
		memmove( WORK9 , WORK0 , MONY_BUF_SIZE );
		m_Arith.l_neg( WORK9 );
		m_Arith.l_print( dp, WORK9, FMT15_EX );
		PF1[i].PITM[j++] = dp;
		dp += 19;
		m_Arith.l_add( GK_WORK9, GK_WORK9, WORK9 );//çáåvë´ÇµÇ±Ç›
		m_Arith.l_add( GK_WORK12, GK_WORK12 , WORK9 );//çáåvë´ÇµÇ±Ç›
	}
	else{
		memmove( WORK8 , WORK0 , MONY_BUF_SIZE );
		m_Arith.l_print( dp, WORK8, FMT15_EX );
		PF1[i].PITM[j++] = dp;
		dp += 19;
		PF1[i].PITM[j++] = (char *)0;
		m_Arith.l_add( GK_WORK8, GK_WORK8, WORK8 );//çáåvë´ÇµÇ±Ç›
		m_Arith.l_add( GK_WORK11, GK_WORK11 , WORK8 );//çáåvë´ÇµÇ±Ç›
	}

	//Ça-áI
	PF1[i].PITM[j++] = (char *)0;

	//Ça-áJ
	m_Arith.l_print( dp, WORK8, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ça-áK
	m_Arith.l_print( dp, WORK9, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ça-áL
	PF1[i].PITM[j++] = (char *)0;
	//Ça-áM áJ*25/100
	if( m_Arith.l_test( WORK8 )) {
		memset( WORK3 , '\0', MONY_BUF_SIZE);
		m_Util.percent( WORK3,(char*)WORK8, 7, 25, 0 );
		m_Arith.l_print( dp, WORK3, FMT15_EX );
		PF1[i].PITM[j++] = dp;
		dp += 19;
		m_Arith.l_add( GK_WORK14, GK_WORK14, WORK3 );//çáåvë´ÇµÇ±Ç›
		PF1[i].PITM[j++] = (char *)0;
	}
	else{
		PF1[i].PITM[j++] = (char *)0;
		//Ça-áN áK*25/100
		memset( WORK3 , '\0', MONY_BUF_SIZE);
		m_Util.percent( WORK3,(char*)WORK9, 7, 25, 0 );
		m_Arith.l_print( dp, WORK3, FMT15_EX );
		PF1[i].PITM[j++] = dp;
		dp += 19;
		m_Arith.l_add( GK_WORK15, GK_WORK15, WORK3 );//çáåvë´ÇµÇ±Ç›
	}	

	//Ça-áO
	PF1[i].PITM[j++] = (char *)0;

	if( j = _xprn( PP, 2, PF1, 0, pDC, isNoImage, 1 ) ){
		goto POUT1;
	}

//ê≈ó¶6.3%ìKópï™Çb
	dp = AP;
	i = j = 0;
	memset( dp, '\0', size );				
	memset( WORK0, '\0', MONY_BUF_SIZE );
	memset( WORK8, '\0', MONY_BUF_SIZE );
	memset( WORK9, '\0', MONY_BUF_SIZE );
	memset( WORK_KOUJYOZEI , '\0', MONY_BUF_SIZE);
	//Çb-á@
	m_Arith.l_div( WORK0, PrintData->Sn_1F1C, PW0 );
	m_Arith.l_print( dp, WORK0, FMT110_EX );
	strcat_s( dp, 18+1, _T("   ") );
	PF1[i].PITM[j++] = dp;
	dp += 19;	

	i++;
	j = 0;
	//Çb-áA
	m_Arith.l_print( dp, PrintData->Sn_1F2C, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Çb-áB
	m_Arith.l_print( dp, PrintData->Sn_1F3C, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Çb-áC
	m_Arith.l_print( dp, PrintData->Sn_1F4C,  FMT15_EX );
	m_Arith.l_add( WORK_KOUJYOZEI, WORK_KOUJYOZEI, PrintData->Sn_1F4C );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Çb-áD
	m_Arith.l_print( dp, PrintData->Sn_1F5C, FMT15_EX );
	m_Arith.l_add( WORK_KOUJYOZEI, WORK_KOUJYOZEI, PrintData->Sn_1F5C );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Çb-áE
	m_Arith.l_print( dp, PrintData->Sn_1F6C, FMT15_EX );
	m_Arith.l_add( WORK_KOUJYOZEI, WORK_KOUJYOZEI, PrintData->Sn_1F6C );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Çb-áFçTèúê≈äzè¨åv
	m_Arith.l_print( dp, WORK_KOUJYOZEI, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	m_Arith.l_add( GK_WORK7, GK_WORK7, WORK_KOUJYOZEI );//çáåvë´ÇµÇ±Ç›

//--> '14.10.17 INS START
	if( IsNeedExpCalq() ){
		m_Arith.l_sub( GK_WORK7, GK_WORK7, PrintData->Sn_1F4A );
		m_Arith.l_sub( GK_WORK7, GK_WORK7, PrintData->Sn_1F4B );
		m_Arith.l_sub( GK_WORK7, GK_WORK7, PrintData->Sn_1F4C );
		m_Arith.l_add( GK_WORK7, GK_WORK7, PrintData->Sn_1F4T );
	}
//<-- '14.10.17 INS END

	//Çb-áGçTèúïsë´ä“ïtê≈äz
	memset( WORK0, '\0', MONY_BUF_SIZE );
	m_Arith.l_sub( WORK0 , WORK_KOUJYOZEI , PrintData->Sn_1F2C );
	m_Arith.l_sub( WORK0 , WORK0 , PrintData->Sn_1F3C );

	if( m_Arith.l_test( WORK0 ) < 0 ){//É}ÉCÉiÉXÇÃèÍçá
		memmove( WORK9 , WORK0 , MONY_BUF_SIZE );
		PF1[i].PITM[j++] = (char *)0;
		m_Arith.l_neg( WORK9 );
		m_Arith.l_print( dp, WORK9, FMT15_EX );
		PF1[i].PITM[j++] = dp;
		dp += 19;
		m_Arith.l_add( GK_WORK9, GK_WORK9, WORK9 );//çáåvë´ÇµÇ±Ç›
		m_Arith.l_add( GK_WORK12, GK_WORK12 , WORK9 );//çáåvë´ÇµÇ±Ç›
	}
	else{
		memmove( WORK8 , WORK0 , MONY_BUF_SIZE );
		m_Arith.l_print( dp, WORK8, FMT15_EX );
		PF1[i].PITM[j++] = dp;
		dp += 19;
		PF1[i].PITM[j++] = (char *)0;
		m_Arith.l_add( GK_WORK8, GK_WORK8, WORK8 );//çáåvë´ÇµÇ±Ç›
		m_Arith.l_add( GK_WORK11, GK_WORK11 , WORK8 );//çáåvë´ÇµÇ±Ç›
	}
	//Çb-áI
	PF1[i].PITM[j++] = (char *)0;
	if(  m_Arith.l_test( WORK8 ) ){//É}ÉCÉiÉXÇÃèÍçá
		//Çb-áJ
		m_Arith.l_print( dp, WORK8, FMT15_EX );
		PF1[i].PITM[j++] = dp;
		dp += 19;
		//Çb-áK
		PF1[i].PITM[j++] = (char *)0;
	}
	else{
		//Çb-áJ
		PF1[i].PITM[j++] = (char *)0;
		//Çb-áK
		m_Arith.l_print( dp, WORK9, FMT15_EX );
		PF1[i].PITM[j++] = dp;
		dp += 19;

	}
	//Çb-áL
	PF1[i].PITM[j++] = (char *)0;

	if(  m_Arith.l_test( WORK8 ) ){//É}ÉCÉiÉXÇÃèÍçá
		//Çb-áM áJ*17/63
		::ZeroMemory( WORK3 , sizeof ( WORK3 ) );
		m_Util.percent( WORK3,(char*)WORK8, 8, 17, 0 );
		m_Arith.l_print( dp, WORK3, FMT15_EX );
		PF1[i].PITM[j++] = dp;
		dp += 19;
		m_Arith.l_add( GK_WORK14, GK_WORK14, WORK3 );//çáåvë´ÇµÇ±Ç›
		//Çb-áN áK*17/63
		PF1[i].PITM[j++] = (char *)0;
	}
	else{
		//Çb-áM áJ*17/63
		PF1[i].PITM[j++] = (char *)0;
		//Çb-áN áK*17/63		
		memset( WORK3, '\0', MONY_BUF_SIZE );
		m_Util.percent( WORK3,(char*)WORK9, 8, 17, 0 );
		m_Arith.l_print( dp, WORK3, FMT15_EX );
		PF1[i].PITM[j++] = dp;
		dp += 19;
		m_Arith.l_add( GK_WORK15, GK_WORK15, WORK3 );//çáåvë´ÇµÇ±Ç›

	}

	//Çb-áO
	PF1[i].PITM[j++] = (char *)0;

	if( j = _xprn( PP, 3, PF1, 0, pDC, isNoImage, 1 ) ){
		goto POUT1;
	}
	
//çáåvÅiÇ`Å{ÇaÅ{ÇbÅjá@
	i = 0;
	j = 0;	
	//Çc-á@
	memset( WORK0, '\0', MONY_BUF_SIZE );	
	memset( WORK1, '\0', MONY_BUF_SIZE );
	m_Arith.l_div( WORK1, PrintData->Sn_1F1A, PW0 );
	m_Arith.l_add( WORK0, WORK0, WORK1 );
	m_Arith.l_div( WORK1, PrintData->Sn_1F1B, PW0 );
	m_Arith.l_add( WORK0, WORK0, WORK1 );
	m_Arith.l_div( WORK1, PrintData->Sn_1F1C, PW0 );
	m_Arith.l_add( WORK0, WORK0, WORK1 );
	m_Arith.l_print( dp, WORK0, FMT110_EX );
	strcat_s( dp, 18+1, _T("   ") );
	PF1[i].PITM[0] = dp;
	dp += 19;			

	i++;
	j = 0;	
	//Çc-áA
	m_Arith.l_add( WORK0, PrintData->Sn_1F2A, PrintData->Sn_1F2B );
	m_Arith.l_add( WORK0, WORK0, PrintData->Sn_1F2C );
	m_Arith.l_print( dp, WORK0, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;			
	//Çc-áB
	m_Arith.l_add( WORK0, PrintData->Sn_1F3A, PrintData->Sn_1F3B );
	m_Arith.l_add( WORK0, WORK0, PrintData->Sn_1F3C );
	m_Arith.l_print( dp, WORK0, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;			
	//Çc-áC
//-- '14.10.17 --
//	m_Arith.l_add( WORK0, PrintData->Sn_1F4A, PrintData->Sn_1F4B );
//	m_Arith.l_add( WORK0, WORK0, PrintData->Sn_1F4C );
//	m_Arith.l_print( dp, WORK0, FMT15_EX );
//---------------
	if( IsNeedExpCalq() ){
//--- '15.01.27 ---
//		m_Arith.l_print( dp, PrintData->Sn_1F4T, FMT15_EX );
//-----------------
		if( m_Arith.l_test(PrintData->Sn_1F4A) || m_Arith.l_test(PrintData->Sn_1F4B) || m_Arith.l_test(PrintData->Sn_1F4C) ){
			m_Arith.l_print( dp, PrintData->Sn_1F4T, FMT15_EXZ );
		}
		else{
			m_Arith.l_print( dp, PrintData->Sn_1F4T, FMT15_EX );
		}
//-----------------
	}
	else{
		m_Arith.l_add( WORK0, PrintData->Sn_1F4A, PrintData->Sn_1F4B );
		m_Arith.l_add( WORK0, WORK0, PrintData->Sn_1F4C );
		m_Arith.l_print( dp, WORK0, FMT15_EX );
	}
//---------------

	PF1[i].PITM[j++] = dp;
	dp += 19;		
	//Çc-áD
	m_Arith.l_add( WORK0, PrintData->Sn_1F5A, PrintData->Sn_1F5B );
	m_Arith.l_add( WORK0, WORK0, PrintData->Sn_1F5C );
	m_Arith.l_print( dp, WORK0, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;			
	//Çc-áE
	m_Arith.l_add( WORK0, PrintData->Sn_1F6A, PrintData->Sn_1F6B );
	m_Arith.l_add( WORK0, WORK0, PrintData->Sn_1F6C );
	m_Arith.l_print( dp, WORK0, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;			
	//Çc-áF
	m_Arith.l_print( dp, GK_WORK7, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
//-- '14.10.17 --
//	//Çc-áG
//	m_Arith.l_print( dp, GK_WORK8, FMT15_EX );
//	PF1[i].PITM[j++] = dp;
//	dp += 19;			
//	//Çc-áH
//	m_Arith.l_print( dp, GK_WORK9, FMT15_EX );
//	PF1[i].PITM[j++] = dp;
//	dp += 19;			
//---------------
	if( IsNeedExpCalq() ){
		// ã‡äzåvéZ
		char	TATE_WORK8[MONY_BUF_SIZE]={0}, TATE_WORK9[MONY_BUF_SIZE]={0};
		m_Arith.l_add( TATE_WORK8, TATE_WORK8, GK_WORK7 );
		m_Arith.l_sub( TATE_WORK8, TATE_WORK8, PrintData->Sn_1F2A );
		m_Arith.l_sub( TATE_WORK8, TATE_WORK8, PrintData->Sn_1F2B );
		m_Arith.l_sub( TATE_WORK8, TATE_WORK8, PrintData->Sn_1F2C );
		m_Arith.l_sub( TATE_WORK8, TATE_WORK8, PrintData->Sn_1F3A );
		m_Arith.l_sub( TATE_WORK8, TATE_WORK8, PrintData->Sn_1F3B );
		m_Arith.l_sub( TATE_WORK8, TATE_WORK8, PrintData->Sn_1F3C );

		// îªíË
		if( m_Arith.l_test(TATE_WORK8) >= 0 ){
			//Çc-áG
			m_Arith.l_print( dp, TATE_WORK8, FMT15_EX );
			PF1[i].PITM[j++] = dp;
			dp += 19;			

			//Çc-áH
			PF1[i].PITM[j++] = dp;
			dp += 19;			
		}
		else{
			//Çc-áG
			PF1[i].PITM[j++] = dp;
			dp += 19;			

			//Çc-áH
			memmove( TATE_WORK9, TATE_WORK8, sizeof(TATE_WORK9) );
			m_Arith.l_neg( TATE_WORK9 );
			m_Arith.l_print( dp, TATE_WORK9, FMT15_EX );
			PF1[i].PITM[j++] = dp;
			dp += 19;			
		}
	}
	else{
		//Çc-áG
		m_Arith.l_print( dp, GK_WORK8, FMT15_EX );
		PF1[i].PITM[j++] = dp;
		dp += 19;			
		//Çc-áH
		m_Arith.l_print( dp, GK_WORK9, FMT15_EX );
		PF1[i].PITM[j++] = dp;
		dp += 19;			
	}
//---------------
	//Çc-áI
	m_Arith.l_print( dp, PrintData->Sn_1F10T, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;			

	//Çc-áJ
	m_Arith.l_print( dp, GK_WORK11, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;			
	//Çc-áK
	m_Arith.l_print( dp, GK_WORK12, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;			
	//Çc-áL
	memset( WORK0, '\0', MONY_BUF_SIZE );
	m_Arith.l_sub( WORK0 , GK_WORK12, GK_WORK11 );
	m_Arith.l_print( dp, WORK0, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;			
	//Çc-áM
	m_Arith.l_print( dp, GK_WORK14, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;			
	//Çc-áN
	m_Arith.l_print( dp, GK_WORK15, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;	

	//Çc-áL
	memset( WORK0, '\0', MONY_BUF_SIZE );
	m_Arith.l_sub( WORK0 , GK_WORK15, GK_WORK14 );
	m_Arith.l_print( dp, WORK0, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	if( j = _xprn( PP, 4, PF1, 0, pDC, 0, 1 ) ){
		goto POUT1;
	}

	// éÅñº
	dp = AP;	i = j = 0;
	memset( dp, '\0', size );
	if( kojin ){	// å¬êléñã∆é“ÇÃèÍçáÇÕë„ï\é“éÅñº
		j = sizeof( m_pSnHeadData->Sn_DIHYO );
		memmove( dp, m_pSnHeadData->Sn_DIHYO, j );
	}
	else{			// ñ@êlñº
		j = sizeof( m_pSnHeadData->Sn_CONAM );
		memmove( dp, m_pSnHeadData->Sn_CONAM, j );
	}
	PF1[0].PITM[0] = dp;
	if( j = _xprn( PP, 5, PF1, 2, pDC, 0, 1 ) ){
		goto POUT1;
	}

POUT1:
	delete [] AP;
	
	return j;
}

//2015.08.04 INSERT START
//-----------------------------------------------------------------------------
// ïtï\ÇPÅEÇSèoóÕ ÉäÉoÅ[ÉXÉ`ÉÉÅ[ÉWëŒâû
//-----------------------------------------------------------------------------
// à¯êî	pDC
//		kojin
//		pcnt 
//-----------------------------------------------------------------------------
// ï‘ëóíl
//-----------------------------------------------------------------------------
int CH26HyoView::PrintFuhyo10Ex( CDC *pDC, int kojin, int pcnt )
{
	int				i, j;
	unsigned int	size;
	char			*AP, *dp, PW0[MONY_BUF_SIZE];
	
	BOOL	isNoImage = FALSE;
	if( m_pPrintCmInfo->PRtype&0x80 ){
		isNoImage = FALSE;
	}
	else{
		isNoImage = TRUE;
	}

	size = 1024;
	if( (AP = new char[size]) == NULL ){
		return -1;
	}
	
	// èoóÕëOèWåv
//	FuHyo14Calqe();
	
	char	WORK0[MONY_BUF_SIZE] = {0};
	char	WORK1[MONY_BUF_SIZE] = {0};	
	char	WORK2[MONY_BUF_SIZE] = {0};
	char	WORK3[MONY_BUF_SIZE] = {0};
	char	WORK8[MONY_BUF_SIZE] = {0};
	char	WORK9[MONY_BUF_SIZE] = {0};
	//çáåvóÒÇ…ïKóvÇ»Ç‡ÇÃ
	char	GK_WORK7[MONY_BUF_SIZE] = {0};
	char	GK_WORK8[MONY_BUF_SIZE] = {0};
	char	GK_WORK9[MONY_BUF_SIZE] = {0};
	char	GK_WORK11[MONY_BUF_SIZE] = {0};
	char	GK_WORK12[MONY_BUF_SIZE] = {0};
	char	GK_WORK14[MONY_BUF_SIZE] = {0};
	char	GK_WORK15[MONY_BUF_SIZE] = {0};

	memset( GK_WORK7, '\0', MONY_BUF_SIZE );
	memset( GK_WORK8, '\0', MONY_BUF_SIZE );
	memset( GK_WORK9, '\0', MONY_BUF_SIZE );
	memset( GK_WORK11, '\0', MONY_BUF_SIZE );
	memset( GK_WORK12, '\0', MONY_BUF_SIZE );
	memset( GK_WORK14, '\0', MONY_BUF_SIZE );
	memset( GK_WORK15, '\0', MONY_BUF_SIZE );

	memset( PW0, '\0', MONY_BUF_SIZE );
	m_Arith.l_input( PW0, _T("1000") );

	memset( WORK0, '\0', MONY_BUF_SIZE );
	
	char WORK_KOUJYOZEI[MONY_BUF_SIZE] = {0};

PBUF	PF1[30];
int		PP;
#ifdef SHIMIZU_TEST
	CH26SnFhyo10Data*	PrintData;
	// ïtï\ÇSÉfÅ[É^ì«çûÇ›ÉNÉâÉX
	if( m_pSnFhyo10Data ){
		PrintData = m_pSnFhyo10Data;
	}
	else{
		PrintData = m_pSnFhyo40Data;
	}

#endif
	PP = 5;

	dp = AP;
	memset( dp, '\0', size );
	i = 0;

int CCSw = 0;

	// ã≠êßâ€ê≈ä˙ä‘àÛç¸
	if( CCSw ){		// ã≠êßä˙ä‘ïœçX
	}
	else{
		char	yymmdd[3] = {0};
		if( IsJcContinuePrint(true) ){
			m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		else{
			m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		wsprintf( dp, _T("%02X %02X %02X"), yymmdd[0]&0xff, yymmdd[1]&0xff, yymmdd[2]&0xff );
		PF1[i].PITM[0] = dp;
		dp += 9;

		memset( yymmdd, '\0', sizeof(yymmdd) );
		if( IsJcContinuePrint(true) ){
			m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		else{
			m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		wsprintf( dp, _T("%02X %02X %02X"), yymmdd[0]&0xff, yymmdd[1]&0xff, yymmdd[2]&0xff );
		PF1[i++].PITM[1] = dp;
		dp += 9;
	}
	memmove( dp, _T("Å@"), 2 );
	PF1[i++].PITM[0] = dp;
	dp += (2 + 1);

//ê≈ó¶3%ìKópï™Ç`
	j = 0;
	//Ç`-á@
	m_Arith.l_div( WORK0, PrintData->Sn_1F1A, PW0 );
	m_Arith.l_print( dp, WORK0, FMT110_EX );
	strcat_s( dp, 18+1, _T("   ") );
	PF1[i].PITM[j++] = dp;
	dp += 19;	
	i++;
	j = 0;
	//Ç`-á@-1
//-- '15.11.13 --
//	m_Arith.l_print( dp, ((CH28SnFhyo10Data*)PrintData)->Sn_1F1_1A, FMT16_EX );
//	PF1[i].PITM[j++] = dp;
//---------------
	if( m_pSnHeadData->IsNeedTkkzDisp() ){
		m_Arith.l_print( dp, ((CH28SnFhyo10Data*)PrintData)->Sn_1F1_1A, FMT16_EX );
		PF1[i].PITM[j++] = dp;
	}
	else{
		PF1[i].PITM[j++] = (char *)0;
	}
//---------------
	dp += 19;
	//Ç`-á@-2
	PF1[i].PITM[j++] = (char *)0;
	dp += 19;
	//Ç`-áA
	m_Arith.l_print( dp, PrintData->Sn_1F2A, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ç`-áB
	m_Arith.l_print( dp, PrintData->Sn_1F3A, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ç`-áC
	m_Arith.l_print( dp, PrintData->Sn_1F4A,  FMT15_EX );
	m_Arith.l_add( WORK_KOUJYOZEI, WORK_KOUJYOZEI, PrintData->Sn_1F4A );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ç`-áD
	m_Arith.l_print( dp, PrintData->Sn_1F5A, FMT15_EX );
	m_Arith.l_add( WORK_KOUJYOZEI, WORK_KOUJYOZEI, PrintData->Sn_1F5A );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ç`-áD-1
//-- '15.11.13 --
//	m_Arith.l_print( dp, ((CH28SnFhyo10Data*)PrintData)->Sn_1F5_1A, FMT15_EX );
//	PF1[i].PITM[j++] = dp;
//---------------
	if( m_pSnHeadData->IsNeedTkkzDisp() ){
		m_Arith.l_print( dp, ((CH28SnFhyo10Data*)PrintData)->Sn_1F5_1A, FMT15_EX );
		PF1[i].PITM[j++] = dp;
	}
	else{
		PF1[i].PITM[j++] = (char *)0;
	}
//---------------
	dp += 19;
	//Ç`-áD-2
	PF1[i].PITM[j++] = (char *)0;
	dp += 19;
	//Ç`-áE
	m_Arith.l_print( dp, PrintData->Sn_1F6A, FMT15_EX );
	m_Arith.l_add( WORK_KOUJYOZEI, WORK_KOUJYOZEI, PrintData->Sn_1F6A );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ç`-áFçTèúê≈äzè¨åv
	m_Arith.l_print( dp, WORK_KOUJYOZEI, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	m_Arith.l_add( GK_WORK7, GK_WORK7, WORK_KOUJYOZEI );//çáåvë´ÇµÇ±Ç›

	//Ç`-áGçTèúïsë´ä“ïtê≈äz
	memset( WORK0 , '\0', MONY_BUF_SIZE);
	m_Arith.l_sub( WORK0 , WORK_KOUJYOZEI , PrintData->Sn_1F2A );
	m_Arith.l_sub( WORK0 , WORK0 , PrintData->Sn_1F3A );

	if( m_Arith.l_test( WORK0 ) < 0 ){//É}ÉCÉiÉXÇÃèÍçá
		PF1[i].PITM[j++] = (char *)0;
		m_Arith.l_neg( WORK0 );
		m_Arith.l_print( dp, WORK0, FMT15_EX );
		PF1[i].PITM[j++] = dp;
		dp += 19;
		m_Arith.l_add( GK_WORK9, GK_WORK9, WORK0 );//çáåvë´ÇµÇ±Ç›
	}
	else{
		m_Arith.l_print( dp, WORK0, FMT15_EX );
		PF1[i].PITM[j++] = dp;
		dp += 19;
		PF1[i].PITM[j++] = (char *)0;
		m_Arith.l_add( GK_WORK8, GK_WORK8, WORK0 );//çáåvë´ÇµÇ±Ç›
	}

	//Ç`-áIÅ`áO
	PF1[i].PITM[j++] = (char *)0;
	PF1[i].PITM[j++] = (char *)0;
	PF1[i].PITM[j++] = (char *)0;
	i++;
	j = 0;
	PF1[i].PITM[j++] = (char *)0;
	PF1[i].PITM[j++] = (char *)0;
	PF1[i].PITM[j++] = (char *)0;
	PF1[i].PITM[j++] = (char *)0;

	if( j = _xprn( PP, 1, PF1, 0, pDC, isNoImage, 1 ) ){
		goto POUT1;
	}

//ê≈ó¶4%ìKópï™Ça
	dp = AP;
	i = j = 0;
	memset( dp, '\0', size );					// ê≈ó¶4%ìKópï™Ça á@->áD
	memset( WORK0, '\0', MONY_BUF_SIZE );
	memset( WORK8, '\0', MONY_BUF_SIZE );
	memset( WORK9, '\0', MONY_BUF_SIZE );
	memset( WORK_KOUJYOZEI , '\0', MONY_BUF_SIZE);
	//Ça-á@
	m_Arith.l_div( WORK0, PrintData->Sn_1F1B, PW0 );
	m_Arith.l_print( dp, WORK0, FMT110_EX );
	strcat_s( dp, 18+1, _T("   ") );
	PF1[i].PITM[j++] = dp;
	dp += 19;	

	i++;
	j = 0;
	//Ça-á@-1
//-- '15.11.13 --
//	m_Arith.l_print( dp, ((CH28SnFhyo10Data*)PrintData)->Sn_1F1_1B, FMT16_EX );
//	PF1[i].PITM[j++] = dp;
//---------------
	if( m_pSnHeadData->IsNeedTkkzDisp() ){
		m_Arith.l_print( dp, ((CH28SnFhyo10Data*)PrintData)->Sn_1F1_1B, FMT16_EX );
		PF1[i].PITM[j++] = dp;
	}
	else{
		PF1[i].PITM[j++] = (char *)0;
	}
//---------------
	dp += 19;
	//Ça-á@-2
	PF1[i].PITM[j++] = (char *)0;
	dp += 19;
	//Ça-áA
	m_Arith.l_print( dp, PrintData->Sn_1F2B, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ça-áB
	m_Arith.l_print( dp, PrintData->Sn_1F3B, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ça-áC
	m_Arith.l_print( dp, PrintData->Sn_1F4B,  FMT15_EX );
	m_Arith.l_add( WORK_KOUJYOZEI, WORK_KOUJYOZEI, PrintData->Sn_1F4B );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ça-áD
	m_Arith.l_print( dp, PrintData->Sn_1F5B, FMT15_EX );
	m_Arith.l_add( WORK_KOUJYOZEI, WORK_KOUJYOZEI, PrintData->Sn_1F5B );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ça-áD-1
//-- '15.11.13 --
//	m_Arith.l_print( dp, ((CH28SnFhyo10Data*)PrintData)->Sn_1F5_1B, FMT15_EX );
//	PF1[i].PITM[j++] = dp;
//---------------
	if( m_pSnHeadData->IsNeedTkkzDisp() ){
		m_Arith.l_print( dp, ((CH28SnFhyo10Data*)PrintData)->Sn_1F5_1B, FMT15_EX );
		PF1[i].PITM[j++] = dp;
	}
	else{
		PF1[i].PITM[j++] = (char *)0;
	}
//---------------
	dp += 19;
	//Ç`-áD-2
	PF1[i].PITM[j++] = (char *)0;
	dp += 19;
	//Ça-áE
	m_Arith.l_print( dp, PrintData->Sn_1F6B, FMT15_EX );
	m_Arith.l_add( WORK_KOUJYOZEI, WORK_KOUJYOZEI, PrintData->Sn_1F6B );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Ça-áFçTèúê≈äzè¨åv
	m_Arith.l_print( dp, WORK_KOUJYOZEI, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	m_Arith.l_add( GK_WORK7, GK_WORK7, WORK_KOUJYOZEI );//çáåvë´ÇµÇ±Ç›

	//Ça-áGçTèúïsë´ä“ïtê≈äz
	memset( WORK0 , '\0', MONY_BUF_SIZE);
	m_Arith.l_sub( WORK0 , WORK_KOUJYOZEI , PrintData->Sn_1F2B );
	m_Arith.l_sub( WORK0 , WORK0 , PrintData->Sn_1F3B );
	if( m_Arith.l_test( WORK0 ) < 0 ){//É}ÉCÉiÉXÇÃèÍçá
		PF1[i].PITM[j++] = (char *)0;
		memmove( WORK9 , WORK0 , MONY_BUF_SIZE );
		m_Arith.l_neg( WORK9 );
		m_Arith.l_print( dp, WORK9, FMT15_EX );
		PF1[i].PITM[j++] = dp;
		dp += 19;
		m_Arith.l_add( GK_WORK9, GK_WORK9, WORK9 );//çáåvë´ÇµÇ±Ç›
		m_Arith.l_add( GK_WORK12, GK_WORK12 , WORK9 );//çáåvë´ÇµÇ±Ç›
	}
	else{
		memmove( WORK8 , WORK0 , MONY_BUF_SIZE );
		m_Arith.l_print( dp, WORK8, FMT15_EX );
		PF1[i].PITM[j++] = dp;
		dp += 19;
		PF1[i].PITM[j++] = (char *)0;
		m_Arith.l_add( GK_WORK8, GK_WORK8, WORK8 );//çáåvë´ÇµÇ±Ç›
		m_Arith.l_add( GK_WORK11, GK_WORK11 , WORK8 );//çáåvë´ÇµÇ±Ç›
	}

	//Ça-áI
	PF1[i].PITM[j++] = (char *)0;

	//Ça-áJ
	m_Arith.l_print( dp, WORK8, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Ça-áK
	m_Arith.l_print( dp, WORK9, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	i++;
	j = 0;
	//Ça-áL
	PF1[i].PITM[j++] = (char *)0;
	//Ça-áM áJ*25/100
	if( m_Arith.l_test( WORK8 )) {
		memset( WORK3 , '\0', MONY_BUF_SIZE);
		m_Util.percent( WORK3,(char*)WORK8, 7, 25, 0 );
		m_Arith.l_print( dp, WORK3, FMT15_EX );
		PF1[i].PITM[j++] = dp;
		dp += 19;
		m_Arith.l_add( GK_WORK14, GK_WORK14, WORK3 );//çáåvë´ÇµÇ±Ç›
		PF1[i].PITM[j++] = (char *)0;
	}
	else{
		PF1[i].PITM[j++] = (char *)0;
		//Ça-áN áK*25/100
		memset( WORK3 , '\0', MONY_BUF_SIZE);
		m_Util.percent( WORK3,(char*)WORK9, 7, 25, 0 );
		m_Arith.l_print( dp, WORK3, FMT15_EX );
		PF1[i].PITM[j++] = dp;
		dp += 19;
		m_Arith.l_add( GK_WORK15, GK_WORK15, WORK3 );//çáåvë´ÇµÇ±Ç›
	}	

	//Ça-áO
	PF1[i].PITM[j++] = (char *)0;

	if( j = _xprn( PP, 2, PF1, 0, pDC, isNoImage, 1 ) ){
		goto POUT1;
	}

//ê≈ó¶6.3%ìKópï™Çb
	dp = AP;
	i = j = 0;
	memset( dp, '\0', size );				
	memset( WORK0, '\0', MONY_BUF_SIZE );
	memset( WORK8, '\0', MONY_BUF_SIZE );
	memset( WORK9, '\0', MONY_BUF_SIZE );
	memset( WORK_KOUJYOZEI , '\0', MONY_BUF_SIZE);
	//Çb-á@
	m_Arith.l_div( WORK0, PrintData->Sn_1F1C, PW0 );
	m_Arith.l_print( dp, WORK0, FMT110_EX );
	strcat_s( dp, 18+1, _T("   ") );
	PF1[i].PITM[j++] = dp;
	dp += 19;	

	i++;
	j = 0;
	//Çb-á@-1
//-- '15.11.13 --
//	m_Arith.l_print( dp, ((CH28SnFhyo10Data*)PrintData)->Sn_1F1_1C, FMT16_EX );
//	PF1[i].PITM[j++] = dp;
//---------------
	if( m_pSnHeadData->IsNeedTkkzDisp() ){
		m_Arith.l_print( dp, ((CH28SnFhyo10Data*)PrintData)->Sn_1F1_1C, FMT16_EX );
		PF1[i].PITM[j++] = dp;
	}
	else{
		PF1[i].PITM[j++] = (char *)0;
	}
//---------------
	dp += 19;
	//Çb-á@-2
//-- '15.11.13 --
//	m_Arith.l_print( dp, ((CH28SnFhyo10Data*)PrintData)->Sn_1F1_2C, FMT16_EX );
//	PF1[i].PITM[j++] = dp;
//---------------
	if( m_pSnHeadData->IsNeedTkkzDisp() ){
		m_Arith.l_print( dp, ((CH28SnFhyo10Data*)PrintData)->Sn_1F1_2C, FMT16_EX );
		PF1[i].PITM[j++] = dp;
	}
	else{
		PF1[i].PITM[j++] = (char *)0;
	}
//---------------
	dp += 19;
	//Çb-áA
	m_Arith.l_print( dp, PrintData->Sn_1F2C, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Çb-áB
	m_Arith.l_print( dp, PrintData->Sn_1F3C, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Çb-áC
	m_Arith.l_print( dp, PrintData->Sn_1F4C,  FMT15_EX );
	m_Arith.l_add( WORK_KOUJYOZEI, WORK_KOUJYOZEI, PrintData->Sn_1F4C );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Çb-áD
	m_Arith.l_print( dp, PrintData->Sn_1F5C, FMT15_EX );
	m_Arith.l_add( WORK_KOUJYOZEI, WORK_KOUJYOZEI, PrintData->Sn_1F5C );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	//Çb-áD-1
//-- '15.11.13 --
//	m_Arith.l_print( dp, ((CH28SnFhyo10Data*)PrintData)->Sn_1F5_1C, FMT15_EX );
//	PF1[i].PITM[j++] = dp;
//---------------
	if( m_pSnHeadData->IsNeedTkkzDisp() ){
		m_Arith.l_print( dp, ((CH28SnFhyo10Data*)PrintData)->Sn_1F5_1C, FMT15_EX );
		PF1[i].PITM[j++] = dp;
	}
	else{
		PF1[i].PITM[j++] = (char *)0;
	}
//---------------
	dp += 19;
	//Çb-áD-2
//-- '15.11.13 --
//	m_Arith.l_print( dp, ((CH28SnFhyo10Data*)PrintData)->Sn_1F5_2C, FMT15_EX );
//	PF1[i].PITM[j++] = dp;
//---------------
	if( m_pSnHeadData->IsNeedTkkzDisp() ){
		m_Arith.l_print( dp, ((CH28SnFhyo10Data*)PrintData)->Sn_1F5_2C, FMT15_EX );
		PF1[i].PITM[j++] = dp;
	}
	else{
		PF1[i].PITM[j++] = (char *)0;
	}
//---------------
	dp += 19;
	//Çb-áE
	m_Arith.l_print( dp, PrintData->Sn_1F6C, FMT15_EX );
	m_Arith.l_add( WORK_KOUJYOZEI, WORK_KOUJYOZEI, PrintData->Sn_1F6C );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//Çb-áFçTèúê≈äzè¨åv
	m_Arith.l_print( dp, WORK_KOUJYOZEI, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	m_Arith.l_add( GK_WORK7, GK_WORK7, WORK_KOUJYOZEI );//çáåvë´ÇµÇ±Ç›

	if( IsNeedExpCalq() ){
		m_Arith.l_sub( GK_WORK7, GK_WORK7, PrintData->Sn_1F4A );
		m_Arith.l_sub( GK_WORK7, GK_WORK7, PrintData->Sn_1F4B );
		m_Arith.l_sub( GK_WORK7, GK_WORK7, PrintData->Sn_1F4C );
		m_Arith.l_add( GK_WORK7, GK_WORK7, PrintData->Sn_1F4T );
	}

	//Çb-áGçTèúïsë´ä“ïtê≈äz
	memset( WORK0, '\0', MONY_BUF_SIZE );
	m_Arith.l_sub( WORK0 , WORK_KOUJYOZEI , PrintData->Sn_1F2C );
	m_Arith.l_sub( WORK0 , WORK0 , PrintData->Sn_1F3C );

	if( m_Arith.l_test( WORK0 ) < 0 ){//É}ÉCÉiÉXÇÃèÍçá
		memmove( WORK9 , WORK0 , MONY_BUF_SIZE );
		PF1[i].PITM[j++] = (char *)0;
		m_Arith.l_neg( WORK9 );
		m_Arith.l_print( dp, WORK9, FMT15_EX );
		PF1[i].PITM[j++] = dp;
		dp += 19;
		m_Arith.l_add( GK_WORK9, GK_WORK9, WORK9 );//çáåvë´ÇµÇ±Ç›
		m_Arith.l_add( GK_WORK12, GK_WORK12 , WORK9 );//çáåvë´ÇµÇ±Ç›
	}
	else{
		memmove( WORK8 , WORK0 , MONY_BUF_SIZE );
		m_Arith.l_print( dp, WORK8, FMT15_EX );
		PF1[i].PITM[j++] = dp;
		dp += 19;
		PF1[i].PITM[j++] = (char *)0;
		m_Arith.l_add( GK_WORK8, GK_WORK8, WORK8 );//çáåvë´ÇµÇ±Ç›
		m_Arith.l_add( GK_WORK11, GK_WORK11 , WORK8 );//çáåvë´ÇµÇ±Ç›
	}
	//Çb-áI
	PF1[i].PITM[j++] = (char *)0;
	if(  m_Arith.l_test( WORK8 ) ){//É}ÉCÉiÉXÇÃèÍçá
		//Çb-áJ
		m_Arith.l_print( dp, WORK8, FMT15_EX );
		PF1[i].PITM[j++] = dp;
		dp += 19;
		//Çb-áK
		PF1[i].PITM[j++] = (char *)0;
	}
	else{
		//Çb-áJ
		PF1[i].PITM[j++] = (char *)0;
		//Çb-áK
		m_Arith.l_print( dp, WORK9, FMT15_EX );
		PF1[i].PITM[j++] = dp;
		dp += 19;

	}

	i++;
	j = 0;
	//Çb-áL
	PF1[i].PITM[j++] = (char *)0;

	if(  m_Arith.l_test( WORK8 ) ){//É}ÉCÉiÉXÇÃèÍçá
		//Çb-áM áJ*17/63
		::ZeroMemory( WORK3 , sizeof ( WORK3 ) );
		m_Util.percent( WORK3,(char*)WORK8, 8, 17, 0 );
		m_Arith.l_print( dp, WORK3, FMT15_EX );
		PF1[i].PITM[j++] = dp;
		dp += 19;
		m_Arith.l_add( GK_WORK14, GK_WORK14, WORK3 );//çáåvë´ÇµÇ±Ç›
		//Çb-áN áK*17/63
		PF1[i].PITM[j++] = (char *)0;
	}
	else{
		//Çb-áM áJ*17/63
		PF1[i].PITM[j++] = (char *)0;
		//Çb-áN áK*17/63		
		memset( WORK3, '\0', MONY_BUF_SIZE );
		m_Util.percent( WORK3,(char*)WORK9, 8, 17, 0 );
		m_Arith.l_print( dp, WORK3, FMT15_EX );
		PF1[i].PITM[j++] = dp;
		dp += 19;
		m_Arith.l_add( GK_WORK15, GK_WORK15, WORK3 );//çáåvë´ÇµÇ±Ç›

	}

	//Çb-áO
	PF1[i].PITM[j++] = (char *)0;

	if( j = _xprn( PP, 3, PF1, 0, pDC, isNoImage, 1 ) ){
		goto POUT1;
	}
	
//çáåvÅiÇ`Å{ÇaÅ{ÇbÅjá@
	i = 0;
	j = 0;	
	//Çc-á@
	memset( WORK0, '\0', MONY_BUF_SIZE );	
	memset( WORK1, '\0', MONY_BUF_SIZE );
	m_Arith.l_div( WORK1, PrintData->Sn_1F1A, PW0 );
	m_Arith.l_add( WORK0, WORK0, WORK1 );
	m_Arith.l_div( WORK1, PrintData->Sn_1F1B, PW0 );
	m_Arith.l_add( WORK0, WORK0, WORK1 );
	m_Arith.l_div( WORK1, PrintData->Sn_1F1C, PW0 );
	m_Arith.l_add( WORK0, WORK0, WORK1 );
	m_Arith.l_print( dp, WORK0, FMT110_EX );
	strcat_s( dp, 18+1, _T("   ") );
	PF1[i].PITM[0] = dp;
	dp += 19;			

	i++;
	j = 0;
	//Çc-á@-1
	m_Arith.l_add( WORK0, ((CH28SnFhyo10Data*)PrintData)->Sn_1F1_1A, ((CH28SnFhyo10Data*)PrintData)->Sn_1F1_1B );
	m_Arith.l_add( WORK0, WORK0, ((CH28SnFhyo10Data*)PrintData)->Sn_1F1_1C );
//-- '15.11.13 --
//	m_Arith.l_print( dp, WORK0, FMT16_EX );
//	PF1[i].PITM[j++] = dp;
//---------------
	if( m_pSnHeadData->IsNeedTkkzDisp() ){
		m_Arith.l_print( dp, WORK0, FMT16_EX );
		PF1[i].PITM[j++] = dp;
	}
	else{
		PF1[i].PITM[j++] = (char *)0;
	}
//---------------
	dp += 19;			
	//Çc-á@-2
//-- '15.11.13 --
//	m_Arith.l_print( dp,  ((CH28SnFhyo10Data*)PrintData)->Sn_1F1_2C, FMT16_EX );
//	PF1[i].PITM[j++] = dp;
//---------------
	if( m_pSnHeadData->IsNeedTkkzDisp() ){
		m_Arith.l_print( dp,  ((CH28SnFhyo10Data*)PrintData)->Sn_1F1_2C, FMT16_EX );
		PF1[i].PITM[j++] = dp;
	}
	else{
		PF1[i].PITM[j++] = (char *)0;
	}
//---------------
	dp += 19;			
	//Çc-áA
	m_Arith.l_add( WORK0, PrintData->Sn_1F2A, PrintData->Sn_1F2B );
	m_Arith.l_add( WORK0, WORK0, PrintData->Sn_1F2C );
	m_Arith.l_print( dp, WORK0, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;			
	//Çc-áB
	m_Arith.l_add( WORK0, PrintData->Sn_1F3A, PrintData->Sn_1F3B );
	m_Arith.l_add( WORK0, WORK0, PrintData->Sn_1F3C );
	m_Arith.l_print( dp, WORK0, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;			
	//Çc-áC
	if( IsNeedExpCalq() ){
		if( m_Arith.l_test(PrintData->Sn_1F4A) || m_Arith.l_test(PrintData->Sn_1F4B) || m_Arith.l_test(PrintData->Sn_1F4C) ){
			m_Arith.l_print( dp, PrintData->Sn_1F4T, FMT15_EXZ );
		}
		else{
			m_Arith.l_print( dp, PrintData->Sn_1F4T, FMT15_EX );
		}
	}
	else{
		m_Arith.l_add( WORK0, PrintData->Sn_1F4A, PrintData->Sn_1F4B );
		m_Arith.l_add( WORK0, WORK0, PrintData->Sn_1F4C );
		m_Arith.l_print( dp, WORK0, FMT15_EX );
	}

	PF1[i].PITM[j++] = dp;
	dp += 19;		
	//Çc-áD
	m_Arith.l_add( WORK0, PrintData->Sn_1F5A, PrintData->Sn_1F5B );
	m_Arith.l_add( WORK0, WORK0, PrintData->Sn_1F5C );
	m_Arith.l_print( dp, WORK0, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;			
	//Çc-áD-1
	m_Arith.l_add( WORK0, ((CH28SnFhyo10Data*)PrintData)->Sn_1F5_1A, ((CH28SnFhyo10Data*)PrintData)->Sn_1F5_1B );
	m_Arith.l_add( WORK0, WORK0, ((CH28SnFhyo10Data*)PrintData)->Sn_1F5_1C );
//-- '15.11.13 --
//	m_Arith.l_print( dp, WORK0, FMT15_EX );
//	PF1[i].PITM[j++] = dp;
//---------------
	if( m_pSnHeadData->IsNeedTkkzDisp() ){
		m_Arith.l_print( dp, WORK0, FMT15_EX );
		PF1[i].PITM[j++] = dp;
	}
	else{
		PF1[i].PITM[j++] = (char *)0;
	}
//---------------
	dp += 19;			
	//Çc-áD-2
//-- '15.11.13 --
//	m_Arith.l_print( dp,  ((CH28SnFhyo10Data*)PrintData)->Sn_1F5_2C, FMT15_EX );
//	PF1[i].PITM[j++] = dp;
//---------------
	if( m_pSnHeadData->IsNeedTkkzDisp() ){
		m_Arith.l_print( dp,  ((CH28SnFhyo10Data*)PrintData)->Sn_1F5_2C, FMT15_EX );
		PF1[i].PITM[j++] = dp;
	}
	else{
		PF1[i].PITM[j++] = (char *)0;
	}
//---------------
	dp += 19;			
	//Çc-áE
	m_Arith.l_add( WORK0, PrintData->Sn_1F6A, PrintData->Sn_1F6B );
	m_Arith.l_add( WORK0, WORK0, PrintData->Sn_1F6C );
	m_Arith.l_print( dp, WORK0, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;			
	//Çc-áF
	m_Arith.l_print( dp, GK_WORK7, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	if( IsNeedExpCalq() ){
		// ã‡äzåvéZ
		char	TATE_WORK8[MONY_BUF_SIZE]={0}, TATE_WORK9[MONY_BUF_SIZE]={0};
		m_Arith.l_add( TATE_WORK8, TATE_WORK8, GK_WORK7 );
		m_Arith.l_sub( TATE_WORK8, TATE_WORK8, PrintData->Sn_1F2A );
		m_Arith.l_sub( TATE_WORK8, TATE_WORK8, PrintData->Sn_1F2B );
		m_Arith.l_sub( TATE_WORK8, TATE_WORK8, PrintData->Sn_1F2C );
		m_Arith.l_sub( TATE_WORK8, TATE_WORK8, PrintData->Sn_1F3A );
		m_Arith.l_sub( TATE_WORK8, TATE_WORK8, PrintData->Sn_1F3B );
		m_Arith.l_sub( TATE_WORK8, TATE_WORK8, PrintData->Sn_1F3C );

		// îªíË
		if( m_Arith.l_test(TATE_WORK8) >= 0 ){
			//Çc-áG
			m_Arith.l_print( dp, TATE_WORK8, FMT15_EX );
			PF1[i].PITM[j++] = dp;
			dp += 19;			

			//Çc-áH
			PF1[i].PITM[j++] = dp;
			dp += 19;			
		}
		else{
			//Çc-áG
			PF1[i].PITM[j++] = dp;
			dp += 19;			

			//Çc-áH
			memmove( TATE_WORK9, TATE_WORK8, sizeof(TATE_WORK9) );
			m_Arith.l_neg( TATE_WORK9 );
			m_Arith.l_print( dp, TATE_WORK9, FMT15_EX );
			PF1[i].PITM[j++] = dp;
			dp += 19;			
		}
	}
	else{
		//Çc-áG
		m_Arith.l_print( dp, GK_WORK8, FMT15_EX );
		PF1[i].PITM[j++] = dp;
		dp += 19;			
		//Çc-áH
		m_Arith.l_print( dp, GK_WORK9, FMT15_EX );
		PF1[i].PITM[j++] = dp;
		dp += 19;			
	}

	//Çc-áI
	m_Arith.l_print( dp, PrintData->Sn_1F10T, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;			

	//Çc-áJ
	m_Arith.l_print( dp, GK_WORK11, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;			
	//Çc-áK
	m_Arith.l_print( dp, GK_WORK12, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;		

	i++;
	j = 0;
	//Çc-áL
	memset( WORK0, '\0', MONY_BUF_SIZE );
	m_Arith.l_sub( WORK0 , GK_WORK12, GK_WORK11 );
	m_Arith.l_print( dp, WORK0, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;			
	//Çc-áM
	m_Arith.l_print( dp, GK_WORK14, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;			
	//Çc-áN
	m_Arith.l_print( dp, GK_WORK15, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;	

	//Çc-áL
	memset( WORK0, '\0', MONY_BUF_SIZE );
	m_Arith.l_sub( WORK0 , GK_WORK15, GK_WORK14 );
	m_Arith.l_print( dp, WORK0, FMT15_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	if( j = _xprn( PP, 4, PF1, 0, pDC, 0, 1 ) ){
		goto POUT1;
	}

	// éÅñº
	dp = AP;	i = j = 0;
	memset( dp, '\0', size );
	if( kojin ){	// å¬êléñã∆é“ÇÃèÍçáÇÕë„ï\é“éÅñº
		j = sizeof( m_pSnHeadData->Sn_DIHYO );
		memmove( dp, m_pSnHeadData->Sn_DIHYO, j );
	}
	else{			// ñ@êlñº
		j = sizeof( m_pSnHeadData->Sn_CONAM );
		memmove( dp, m_pSnHeadData->Sn_CONAM, j );
	}
	PF1[0].PITM[0] = dp;
	if( j = _xprn( PP, 5, PF1, 2, pDC, 0, 1 ) ){
		goto POUT1;
	}

POUT1:
	delete [] AP;
	
	return j;
}
//2015.08.04 INSERT END

//-----------------------------------------------------------------------------
// ïtï\ÇQèoóÕ ê¥êÖï“èW
//-----------------------------------------------------------------------------
// à¯êî	
//
//
//-----------------------------------------------------------------------------
// ï‘ëóíl
//-----------------------------------------------------------------------------
int CH26HyoView::PrintFuhyo20( CDC *pDC, int kojin, int pcnt )
{
	int				i, j;
	unsigned int	size;
	char			*AP, *dp;

	unsigned short	SEno = 1;
	if( m_pPrintCmInfo->PRtype&0x80 ){
/*- '14.02.04 -*/
//		if( m_TaxReforms )	SEno = MAKEWORD( 2, 2 );	// ïΩê¨ÇQÇRîNè¡îÔê≈â¸ê≥î≈
//		else				SEno = MAKEWORD( 1, 1 );
/*-------------*/
//		SEno = MAKEWORD( 2, 2 );
/*-------------*/
	}
	else{
//		SEno = PcSw;
	}
	
	BOOL	isNoImage = FALSE;
	if( m_pPrintCmInfo->PRtype&0x80 ){
		isNoImage = FALSE;
	}
	else{
		isNoImage = TRUE;
	}
	
	size = 512;
	if( (AP = new char[size]) == NULL ){
		return -1;
	}

PBUF	PF1[30];
int		PP;
	
	// èoóÕëOèWåv
//	FuHyo22Calqe();
	PP = 6;			//Åö//[12'09.28]

	dp = AP;
	memset( dp, '\0', size );
	i = 0;

	char kingaku[6] = {0};
	char kingaku2[6] = {0};
	char WORK4[6] = {0};
	char WORK7[6] = {0};

//2016.10.20 INSERT START
	char YEN_ZERO[MONY_BUF_SIZE] = {0}; //0â~ï\ãLóp
//2016.10.20 INSERT END

int CCSw = 0;

	// ã≠êßâ€ê≈ä˙ä‘àÛç¸
	if( CCSw ){	// ã≠êßä˙ä‘ïœçX
//		wsprintf( dp, _T("%02X %02X %02X"), CC_KDAYS[0]&0xff, CC_KDAYS[1]&0xff, CC_KDAYS[2]&0xff );
//		PF1[i].PITM[0] = dp;
//		dp += 9;
//		wsprintf( dp, _T("%02X %02X %02X"), CC_KDAYE[0]&0xff, CC_KDAYE[1]&0xff, CC_KDAYE[2]&0xff );
//		PF1[i++].PITM[1] = dp;
//		dp += 9;
	}
	else{
		char	yymmdd[3] = {0};
		if( IsJcContinuePrint(true) ){
			m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		else{
			m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		wsprintf( dp, _T("%02X %02X %02X"), yymmdd[0]&0xff, yymmdd[1]&0xff, yymmdd[2]&0xff );
		PF1[i].PITM[0] = dp;
		dp += 9;
		memset( yymmdd, '\0', sizeof(yymmdd) );
		if( IsJcContinuePrint(true) ){
			m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		else{
			m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		wsprintf( dp, _T("%02X %02X %02X"), yymmdd[0]&0xff, yymmdd[1]&0xff, yymmdd[2]&0xff );
		PF1[i++].PITM[1] = dp;
		dp += 9;
	}

	memmove( dp, _T("Å@"), 2 );
	PF1[i++].PITM[0] = dp;
	dp += (2 + 1);

	j = 0;		
	// ã‡äzá@->áA
	// á@
	m_Arith.l_print( dp, m_pSnFhyo20Data->Sn_2F1C, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;			
	m_Arith.l_add( kingaku , kingaku , m_pSnFhyo20Data->Sn_2F1C );
	// áA	
	m_Arith.l_print( dp, m_pSnFhyo20Data->Sn_2F2C, FMT16_EX );
	PF1[i++].PITM[j] = dp;
	dp += 19;
	m_Arith.l_add( kingaku , kingaku , m_pSnFhyo20Data->Sn_2F2C );
	
	j = 0;
	// áB
	m_Arith.l_print( dp, m_pSnFhyo20Data->Sn_2F3C, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;		
	m_Arith.l_add( kingaku , kingaku , m_pSnFhyo20Data->Sn_2F3C );

	// áJ
/*- '14.08.21 -*/
//	if( m_pSnHeadData->IsKobetuSiireAnbun() ){
//		m_Arith.l_add( kingaku2 , kingaku2 , m_pSnTanaData->Sn_mnz63[0] );
//		m_Arith.l_add( kingaku2 , kingaku2 , m_pSnTanaData->Sn_mnz63[1] );
//		m_Arith.l_add( kingaku2 , kingaku2 , m_pSnTanaData->Sn_mnz63[2] );
//	}
//	else{
//		m_Arith.l_add( kingaku2 , kingaku2 , m_pSnTanaData->Sn_MNZ63 );
//	}
/*-------------*/
	if( m_pSnTanaData->Sn_MENZsw & 0x10 ){
		if( m_pSnHeadData->IsKobetuSiireAnbun() ){
			m_Arith.l_add( kingaku2 , kingaku2 , m_pSnTanaData->Sn_mnz63[0] );
			m_Arith.l_add( kingaku2 , kingaku2 , m_pSnTanaData->Sn_mnz63[1] );
			m_Arith.l_add( kingaku2 , kingaku2 , m_pSnTanaData->Sn_mnz63[2] );
		}
		else{
			m_Arith.l_add( kingaku2 , kingaku2 , m_pSnTanaData->Sn_MNZ63 );
		}
	}
/*-------------*/
	if( m_pSnTanaData->Sn_MENZsw & 0x01 ) m_Arith.l_neg( kingaku2 );
	m_Arith.l_print( dp, kingaku2, FMT16_EX );
	PF1[i++].PITM[j] = dp;
	dp += 19;			
	
	j = 0;							
	// ã‡äzáC->áF
	// áC
//2016.06.22 UPDATE START
//	m_Arith.l_print( dp, kingaku, FMT16_EX );
	if( m_Arith.l_test( kingaku ) >= 0 ){
		m_Arith.l_print( dp, kingaku, FMT16_EX );
	}else{
		m_Arith.l_print( dp, YEN_ZERO, FMT16_EXZ );
	}
//2016.06.22 UPDATE END
	PF1[i].PITM[j++] = dp;
	dp += 19;		
	m_Arith.l_add( WORK4 , WORK4 , kingaku );
	// áF
	m_Arith.l_add( kingaku , kingaku , m_pSnFhyo20Data->Sn_2F6C );
//2016.06.22 UPDATE START
//	m_Arith.l_print( dp, kingaku, FMT16_EX );
	if( m_Arith.l_test( kingaku ) >= 0 ){
		m_Arith.l_print( dp, kingaku, FMT16_EX );
	}else{
		m_Arith.l_print( dp, YEN_ZERO, FMT16_EXZ );
	}
//2016.06.22 UPDATE END
	PF1[i++].PITM[j] = dp;	
	dp += 19;			
	m_Arith.l_add( WORK7 , WORK7 , kingaku );

	j = 0;	
	// ã‡äzáD->áE
	// áD
	m_Arith.l_print( dp, WORK4, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;		
	// áE
	m_Arith.l_print( dp, m_pSnFhyo20Data->Sn_2F6C, FMT16_EX );
	PF1[i++].PITM[j] = dp;
	dp += 19;			
	
	wsprintf( dp, _T("%3d.%02d"), m_pSnFhyo20Data->Sn_2FUWS / 100, m_pSnFhyo20Data->Sn_2FUWS % 100 );
	PF1[i++].PITM[0] = dp;
	dp += 7;		// îÑè„äÑçá
	
	memset( kingaku , '\0' , 6);

	j = 0;		
	// áG
	m_Arith.l_print( dp, m_pSnFhyo20Data->Sn_2F8C, FMT16_EX );			// áG
	PF1[i].PITM[j++] = dp;
	dp += 19;
	// áH
	m_Arith.l_print( dp, m_pSnFhyo20Data->Sn_2F9C, FMT16_EX );
	PF1[i].PITM[j++] = dp;	
	dp += 19;			
	m_Arith.l_add( kingaku2 , kingaku2 , m_pSnFhyo20Data->Sn_2F9C );
	//áI
	m_Arith.l_print( dp, m_pSnFhyo20Data->Sn_2F10C, FMT16_EX );
	PF1[i++].PITM[j] = dp;
	dp += 19;			// áI				//áJ
	m_Arith.l_add( kingaku2 , kingaku2 , m_pSnFhyo20Data->Sn_2F10C );
	m_Arith.l_add( kingaku2 , kingaku2 , m_pSnFhyo20Data->Sn_2F11C );

	if( j = _xprn( PP, 1, PF1, 0, pDC, isNoImage, SEno ) ){
		goto POUT2;
	}

	dp = AP;	i = j = 0;
	memset( dp, '\0', size );					// ã‡äzáKÅ®áM		//áL->áN
	// áK
	m_Arith.l_print( dp, kingaku2, FMT16_EX );
	PF1[i].PITM[j++] = dp;						
	dp += 19;	

	int Srx = 0;
	int Anbun = 0;
	int Flg12 = 0;
	char W0[6] = {0};

/*- '14.06.16 -*/
//	if( m_pSnHeadData->IsRatioOver95() == TRUE ) Srx = 1;//95%à»è„
/*-------------*/
	if( (m_pSnHeadData->IsRatioOver95()==TRUE) || (m_pSnHeadData->Sn_EXP100&0x01) ){
		Srx = 1;//95%à»è„
	}
/*-------------*/
	if( *m_pShinInfo->pOver500MillionSw  ) Anbun = 1;
	//áL	
	if( m_pSnHeadData->IsUseSemiUriRatio() ){
		PF1[i].PITM[j++] = (char *)0;
	}
	else{
//-- '15.04.18 --
//		if(Srx){
//---------------
		if( Srx && !(m_pSnHeadData->Sn_Sign4&0x10) ){
//---------------
			m_Arith.l_print( dp, kingaku2, FMT16_EX );
			PF1[i].PITM[j++] = dp;		
			dp += 19;
			PF1[i].PITM[j++] = (char *)0;
			m_Arith.l_add( W0 , W0 , kingaku2 );
			Flg12 = 1;
		}
		else{
			PF1[i].PITM[j++] = (char *)0;
		}
	}
	if( m_pSnHeadData->IsUseSemiUriRatio() ){
		if( m_pSnHeadData->IsKobetuSiireAnbun() ){
			m_Arith.l_print( dp, m_pSnFhyo20Data->Sn_2F15C, FMT16_EX );
			PF1[i].PITM[j++] = dp;					// áM				//áN
			dp += 19;	
		}
		else{
			PF1[i].PITM[j++] = (char *)0;
		}
	}
	else{
		if( Srx && Anbun ){
			m_Arith.l_print( dp, m_pSnFhyo20Data->Sn_2F15C, FMT16_EX );
			PF1[i].PITM[j++] = dp;					// áM				//áN
			dp += 19;	
		}
		else{
			if( m_pSnHeadData->IsKobetuSiireAnbun() ){
				m_Arith.l_print( dp, m_pSnFhyo20Data->Sn_2F15C, FMT16_EX );
				PF1[i].PITM[j++] = dp;					// áM				//áN
				dp += 19;	
			}
			else{
				PF1[i].PITM[j++] = (char *)0;
			}
		}
	}
	pbclr( 0, 7, &PF1[++i] );					// ã‡äzáNÅ®Åu22Åv	//áO->¢23£

	//îÑè„äÑçáÇÃåvéZ
	char wari[MONY_BUF_SIZE] = {0};
	char PW6[MONY_BUF_SIZE] = {0};
	char buf[10] = {0};
	char WORK0[MONY_BUF_SIZE] = {0};
	char WORK17ALL[10] = {0};

//--> '15.02.10 INS START
	char	PER100BUF[MONY_BUF_SIZE] = {0};
	m_Arith.l_input( PER100BUF, _T("10000000") );
//<-- '15.02.10 INS END

	if( !Flg12 ){
		if( m_pSnHeadData->IsKobetuSiireAnbun() ){//å¬ï ëŒâûéû
			m_Arith.l_print( dp, m_pSnFhyo20Data->Sn_2F16C, FMT16_EX );
			PF1[i].PITM[0] = dp;
			dp += 19;		// áN				//áO
			j = 1;
			if( m_Arith.l_test( m_pSnFhyo20Data->Sn_2F17C ) ){
				m_Arith.l_print( dp, m_pSnFhyo20Data->Sn_2F17C, FMT16_EX );
				m_Arith.l_add( W0 , W0 , m_pSnFhyo20Data->Sn_2F17C );
			}
			else{
				if( m_pSnHeadData->Sn_UPERS&0x10 ){
					sprintf_s( buf, sizeof( buf ), _T("%d"), m_pSnFhyo20Data->Sn_2FUWSEX );
					m_Arith.l_input( wari, buf );
					m_Arith.l_mul100( (unsigned char *)WORK17ALL, (unsigned char *) m_pSnFhyo20Data->Sn_2F16C, (unsigned char *)wari, 2, 0 );
				}
				else{
					m_Arith.l_par100( (unsigned char *)wari, (unsigned char *)WORK4, (unsigned char *)WORK7,  5, 0 );
					// îÑè„äÑçáÇ™É}ÉCÉiÉXÇÃèÍçáÇÕÇOÅìÇ≈åvéZÇ∑ÇÈ
					memset( PW6, '\0', sizeof(PW6) );
					if( m_Arith.l_cmp(WORK4, PW6) < 0 ){
						memset( wari, '\0', MONY_BUF_SIZE );
					}
					if( m_Arith.l_cmp(WORK7, PW6) < 0 ){
						memset( wari, '\0', MONY_BUF_SIZE );
					}
					if( m_Arith.l_test(wari) == 0 ){
						m_Arith.l_mul100( (unsigned char *)WORK17ALL, (unsigned char *)m_pSnFhyo20Data->Sn_2F16C, (unsigned char *)wari, 5, 0 );
					}
					else{
//-- '15.02.10 --
//						m_Util.l_pardotnet_syz( WORK17ALL, (char *)m_pSnFhyo20Data->Sn_2F16C, WORK4, WORK7 );
//---------------
						if( m_Arith.l_cmp(wari, PER100BUF) <= 0 ){
							m_Util.l_pardotnet_syz( WORK17ALL, (char *)m_pSnFhyo20Data->Sn_2F16C, WORK4, WORK7 );
						}
						else{
							m_Util.l_pardotnet_syz( WORK17ALL, (char *)m_pSnFhyo20Data->Sn_2F16C, WORK4, WORK4 );
						}
//---------------
					}
				}
				m_Arith.l_add( WORK17ALL, WORK17ALL, m_pSnFhyo20Data->Sn_2F15C );
				m_Arith.l_print( dp, WORK17ALL, FMT16_EX );
				m_Arith.l_add( W0 , W0 , WORK17ALL );
			}
			PF1[i].PITM[1] = dp;
			dp += 19;		// áOÅ@ORÅ@áP		//áP or áQ
			PF1[i].PITM[2] = (char *)0;

		}
		else{
			//î‰ó·
			PF1[i].PITM[0] = (char *)0;
			PF1[i].PITM[1] = (char *)0;
			if( m_pSnHeadData->Sn_UPERS & 0x10 ){
				memset( WORK0 , '\0' , MONY_BUF_SIZE);
				sprintf_s( buf, sizeof( buf ), _T("%d"), m_pSnFhyo20Data->Sn_2FUWSEX );
				m_Arith.l_input( wari, buf );
			}
			else{
				m_Arith.l_par100( (unsigned char *)wari, (unsigned char *)WORK4,  (unsigned char *)WORK7,  5, 0 );
				// îÑè„äÑçáÇ™É}ÉCÉiÉXÇÃèÍçáÇÕÇOÅìÇ≈åvéZÇ∑ÇÈÅB
				memset( PW6, '\0', MONY_BUF_SIZE );
//-- '16.12.26 --
//				if( m_Arith.l_cmp(wari, PW6) < 0 ){
//					memset( wari, '\0', MONY_BUF_SIZE );
//				}
//---------------
				if( m_Arith.l_cmp(WORK4, PW6) < 0 ){
					memset( wari, '\0', MONY_BUF_SIZE );
				}
				if( m_Arith.l_cmp(WORK7, PW6) < 0 ){
					memset( wari, '\0', MONY_BUF_SIZE );
				}
//---------------
			}

			if( m_Arith.l_test( m_pSnFhyo20Data->Sn_2F18C )){
				m_Arith.l_print( dp, m_pSnFhyo20Data->Sn_2F18C, FMT16_EX );
				m_Arith.l_add( W0 , W0 , m_pSnFhyo20Data->Sn_2F18C );
			}
			else{
				if( m_pSnHeadData->Sn_UPERS & 0x10 ){
					m_Arith.l_mul100( (unsigned char *)WORK0, (unsigned char *)kingaku2, (unsigned char *)wari, 2, 0 );//12*4/7
					m_Arith.l_print( dp, WORK0 , FMT16_EX );
					m_Arith.l_add( W0 , W0 , WORK0 );

				}
				else{
					if( m_Arith.l_test( wari ) == 0 ){
						m_Arith.l_mul100( (unsigned char *)WORK0, (unsigned char *)kingaku2, (unsigned char *)wari, 5, 0 );//12*4/7

					}
					else{
//-- '15.02.10 --
//						m_Util.l_pardotnet_syz( WORK0, kingaku2, WORK4, WORK7 );
//---------------
						if( m_Arith.l_cmp(wari, PER100BUF) <= 0 ){
							m_Util.l_pardotnet_syz( WORK0, kingaku2, WORK4, WORK7 );
						}
						else{
							m_Util.l_pardotnet_syz( WORK0, kingaku2, WORK4, WORK4 );
						}
//---------------
					}
					m_Arith.l_print( dp, WORK0 , FMT16_EX );
					m_Arith.l_add( W0 , W0 , WORK0 );
				}
			}
			PF1[i].PITM[2] = dp;
			dp += 19;	// áOÅ@ORÅ@áP		//áP or áQ
		}
	
	}
	else{
		PF1[i].PITM[0] = (char *)0;
		PF1[i].PITM[1] = (char *)0;
		PF1[i].PITM[2] = (char *)0;
	}

	m_Arith.l_print( dp, m_pSnFhyo20Data->Sn_2F19C, FMT16_EX );
	PF1[i].PITM[3] = dp;
	dp += 19;		// áQ		
	m_Arith.l_add( W0 , W0 , m_pSnFhyo20Data->Sn_2F19C );

	//áR
	m_Arith.l_print( dp, m_pSnFhyo20Data->Sn_2F20C, FMT16_EX );
	PF1[i].PITM[4] = dp;
	dp += 19;		// áR		
	m_Arith.l_add( W0 , W0 , m_pSnFhyo20Data->Sn_2F20C );
	//áS
	if( m_pSnFhyo20Data->Sn_2F21sw & 0x100 ){
		memset( W0 , '\0' , MONY_BUF_SIZE );
		memmove( W0 , m_pSnFhyo20Data->Sn_2F21C , MONY_BUF_SIZE );
	}
	if( m_Arith.l_test( W0 ) > 0 ){					// Åi20ÅjorÅi21Åj	//(21)or(22)
		m_Arith.l_print( dp, W0, FMT16_EX );
		PF1[i].PITM[5] = dp;	dp += 19;
		j++;
	}
	else if( m_Arith.l_test( W0 ) < 0 ){
		j++;
		m_Arith.l_neg( W0 );
		m_Arith.l_print( dp, W0, FMT16_EX );
		PF1[i].PITM[6] = dp;
		dp += 19;
	}

	m_Arith.l_print( dp, m_pSnFhyo20Data->Sn_2F23C, FMT16_EX );
	PF1[i].PITM[7] = dp;
	dp += 19;		//Å@Åi22Åj			//(23)
	
	if( j = _xprn( PP, 2, PF1, 0, pDC, 0, 1 ) ){
		goto POUT2;
	}

	// éÅñº
	dp = AP;
	i = j = 0;
	memset( dp, '\0', size );
	if( kojin ){	// å¬êléñã∆é“ÇÃèÍçáÇÕë„ï\é“éÅñº
		j = sizeof( m_pSnHeadData->Sn_DIHYO );
		memmove( dp, m_pSnHeadData->Sn_DIHYO, j );
	}
	else {			// ñ@êlñº
		j = sizeof( m_pSnHeadData->Sn_CONAM );
		memmove( dp, m_pSnHeadData->Sn_CONAM, j );
	}
	PF1[0].PITM[0] = dp;
	if( j = _xprn( PP, 3, PF1, 2, pDC, 0, 1 ) ){
		goto POUT2;
	}

POUT2:
	if( AP ){
		delete [] AP;
	}
	
	return j;
}

//2015.08.04 INSERT START
//-----------------------------------------------------------------------------
// ïtï\ÇQèoóÕ ÉäÉoÅ[ÉXÉ`ÉÉÅ[ÉW
//-----------------------------------------------------------------------------
// à¯êî	
//
//
//-----------------------------------------------------------------------------
// ï‘ëóíl
//-----------------------------------------------------------------------------
int CH26HyoView::PrintFuhyo20Ex( CDC *pDC, int kojin, int pcnt )
{
	int				i, j;
	unsigned int	size;
	char			*AP, *dp;

	unsigned short	SEno = 1;
	if( m_pPrintCmInfo->PRtype&0x80 ){
	}
	else{
	}
	
	BOOL	isNoImage = FALSE;
	if( m_pPrintCmInfo->PRtype&0x80 ){
		isNoImage = FALSE;
	}
	else{
		isNoImage = TRUE;
	}
	
//	size = 512;
	size = 1024;
	if( (AP = new char[size]) == NULL ){
		return -1;
	}

PBUF	PF1[30];
int		PP;

	CH28SnFhyo20Data*	PrintData;
	PrintData = (CH28SnFhyo20Data *)m_pSnFhyo20Data;
	
	// èoóÕëOèWåv
	PP = 6;

	dp = AP;
	memset( dp, '\0', size );
	i = 0;

	char kingaku[6] = {0};
	char kingaku2[6] = {0};
	char WORK4[6] = {0};
	char WORK7[6] = {0};
//2016.10.20 INSERT START
	char	YEN_ZERO[MONY_BUF_SIZE] = {0}; //0â~ï\ãLóp
//2016.10.20 INSERT END


int CCSw = 0;

	// ã≠êßâ€ê≈ä˙ä‘àÛç¸
	if( CCSw ){	// ã≠êßä˙ä‘ïœçX
	}
	else{
		char	yymmdd[3] = {0};
		if( IsJcContinuePrint(true) ){
			m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		else{
			m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		wsprintf( dp, _T("%02X %02X %02X"), yymmdd[0]&0xff, yymmdd[1]&0xff, yymmdd[2]&0xff );
		PF1[i].PITM[0] = dp;
		dp += 9;
		memset( yymmdd, '\0', sizeof(yymmdd) );
		if( IsJcContinuePrint(true) ){
			m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		else{
			m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		wsprintf( dp, _T("%02X %02X %02X"), yymmdd[0]&0xff, yymmdd[1]&0xff, yymmdd[2]&0xff );
		PF1[i++].PITM[1] = dp;
		dp += 9;
	}

	memmove( dp, _T("Å@"), 2 );
	PF1[i++].PITM[0] = dp;
	dp += (2 + 1);

	j = 0;		
	// ã‡äzá@->áA
	// á@
	m_Arith.l_print( dp, PrintData->Sn_2F1C, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;			
	m_Arith.l_add( kingaku , kingaku , PrintData->Sn_2F1C );
	// áA	
	m_Arith.l_print( dp, PrintData->Sn_2F2C, FMT16_EX );
	PF1[i++].PITM[j] = dp;
	dp += 19;
	m_Arith.l_add( kingaku , kingaku , PrintData->Sn_2F2C );
	
	j = 0;
	// áB
	m_Arith.l_print( dp, PrintData->Sn_2F3C, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;		
	m_Arith.l_add( kingaku , kingaku , PrintData->Sn_2F3C );

	// áJ
	if( m_pSnTanaData->Sn_MENZsw & 0x10 ){
		if( m_pSnHeadData->IsKobetuSiireAnbun() ){
			m_Arith.l_add( kingaku2 , kingaku2 , m_pSnTanaData->Sn_mnz63[0] );
			m_Arith.l_add( kingaku2 , kingaku2 , m_pSnTanaData->Sn_mnz63[1] );
			m_Arith.l_add( kingaku2 , kingaku2 , m_pSnTanaData->Sn_mnz63[2] );
		}
		else{
			m_Arith.l_add( kingaku2 , kingaku2 , m_pSnTanaData->Sn_MNZ63 );
		}
	}
	if( m_pSnTanaData->Sn_MENZsw & 0x01 ) m_Arith.l_neg( kingaku2 );
	m_Arith.l_print( dp, kingaku2, FMT16_EX );
	PF1[i++].PITM[j] = dp;
	dp += 19;			
	
	j = 0;							
	// ã‡äzáC->áF
	// áC
//2016.06.22 UPDATE START
//	m_Arith.l_print( dp, kingaku, FMT16_EX );
	if( m_Arith.l_test( kingaku ) >= 0 ){
		m_Arith.l_print( dp, kingaku, FMT16_EX );
	}else{
		m_Arith.l_print( dp, YEN_ZERO, FMT16_EXZ );
	}
//2016.06.22 UPDATE END
	PF1[i].PITM[j++] = dp;
	dp += 19;		
	m_Arith.l_add( WORK4 , WORK4 , kingaku );
	// áF
	m_Arith.l_add( kingaku , kingaku , PrintData->Sn_2F6C );
//2016.06.22 UPDATE START
//	m_Arith.l_print( dp, kingaku, FMT16_EX );
	if( m_Arith.l_test( kingaku ) >= 0 ){
		m_Arith.l_print( dp, kingaku, FMT16_EX );
	}else{
		m_Arith.l_print( dp, YEN_ZERO, FMT16_EXZ );
	}
//2016.06.22 UPDATE END
	PF1[i++].PITM[j] = dp;	
	dp += 19;			
	m_Arith.l_add( WORK7 , WORK7 , kingaku );

	j = 0;	
	// ã‡äzáD->áE
	// áD
	m_Arith.l_print( dp, WORK4, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;		
	// áE
	m_Arith.l_print( dp, PrintData->Sn_2F6C, FMT16_EX );
	PF1[i++].PITM[j] = dp;
	dp += 19;			
	
	wsprintf( dp, _T("%3d.%02d"), PrintData->Sn_2FUWS / 100, PrintData->Sn_2FUWS % 100 );
	PF1[i++].PITM[0] = dp;
	dp += 7;		// îÑè„äÑçá
	
	memset( kingaku , '\0' , 6);

	j = 0;		
	// áG
	m_Arith.l_print( dp, PrintData->Sn_2F8C, FMT16_EX );			// áG
	PF1[i].PITM[j++] = dp;
	dp += 19;
	// áH
	m_Arith.l_print( dp, PrintData->Sn_2F9C, FMT16_EX );
	PF1[i++].PITM[j++] = dp;	
	dp += 19;			
	m_Arith.l_add( kingaku2 , kingaku2 , PrintData->Sn_2F9C );

	j = 0;		
	//áI
//-- '15.11.13 --
//	m_Arith.l_print( dp, PrintData->Sn_2F10C, FMT16_EX );
//---------------
	if( m_pSnHeadData->IsNeedTkkzDisp() ){
		m_Arith.l_print( dp, PrintData->Sn_2F10C, FMT16_EX );
	}
	else{
		m_Arith.l_print( dp, kingaku, FMT16_EX );
	}
//---------------
	PF1[i].PITM[j++] = dp;	
	dp += 19;			
	//áJ
//-- '15.11.13 --
//	m_Arith.l_print( dp, PrintData->Sn_2F11C, FMT16_EX );
//---------------
	if( m_pSnHeadData->IsNeedTkkzDisp() ){
		m_Arith.l_print( dp, PrintData->Sn_2F11C, FMT16_EX );
	}
	else{
		m_Arith.l_print( dp, kingaku, FMT16_EX );
	}
//---------------
	PF1[i++].PITM[j++] = dp;	
	dp += 19;
//--> '15.10.23 INS START
	m_Arith.l_add( kingaku2 , kingaku2 , PrintData->Sn_2F11C );
//<-- '15.10.23 INS END

	j = 0;		
	//áK
	m_Arith.l_print( dp, PrintData->Sn_2F12C, FMT16_EX );
	PF1[i++].PITM[j] = dp;
	dp += 19;			// áL				//áM
	m_Arith.l_add( kingaku2 , kingaku2 , PrintData->Sn_2F12C );
	m_Arith.l_add( kingaku2 , kingaku2 , PrintData->Sn_2F13C );

	if( j = _xprn( PP, 1, PF1, 0, pDC, isNoImage, SEno ) ){
		goto POUT2;
	}

	dp = AP;	i = j = 0;
	memset( dp, '\0', size );					// ã‡äzáMÅ®áO		//áN->áP
	// áM
	m_Arith.l_print( dp, kingaku2, FMT16_EX );
	PF1[i].PITM[j++] = dp;						
	dp += 19;	

	int Srx = 0;
	int Anbun = 0;
	int Flg12 = 0;
	char W0[6] = {0};

	if( (m_pSnHeadData->IsRatioOver95()==TRUE) || (m_pSnHeadData->Sn_EXP100&0x01) ){
		Srx = 1;//95%à»è„
	}
	if( *m_pShinInfo->pOver500MillionSw  ) Anbun = 1;
	//áN	
	if( m_pSnHeadData->IsUseSemiUriRatio() ){
		PF1[i].PITM[j++] = (char *)0;
	}
	else{
		if( Srx && !(m_pSnHeadData->Sn_Sign4&0x10) ){
			m_Arith.l_print( dp, kingaku2, FMT16_EX );
			PF1[i].PITM[j++] = dp;		
			dp += 19;
			PF1[i].PITM[j++] = (char *)0;
			m_Arith.l_add( W0 , W0 , kingaku2 );
			Flg12 = 1;
		}
		else{
			PF1[i].PITM[j++] = (char *)0;
		}
	}
	if( m_pSnHeadData->IsUseSemiUriRatio() ){
		if( m_pSnHeadData->IsKobetuSiireAnbun() ){
			m_Arith.l_print( dp, PrintData->Sn_2F16C, FMT16_EX );
			PF1[i].PITM[j++] = dp;					// áM				//áN
			dp += 19;	
		}
		else{
			PF1[i].PITM[j++] = (char *)0;
		}
	}
	else{
		if( Srx && Anbun ){
			m_Arith.l_print( dp, PrintData->Sn_2F16C, FMT16_EX );
			PF1[i].PITM[j++] = dp;					// áM				//áN
			dp += 19;	
		}
		else{
			if( m_pSnHeadData->IsKobetuSiireAnbun() ){
				m_Arith.l_print( dp, PrintData->Sn_2F16C, FMT16_EX );
				PF1[i].PITM[j++] = dp;					// áM				//áN
				dp += 19;	
			}
			else{
				PF1[i].PITM[j++] = (char *)0;
			}
		}
	}
	pbclr( 0, 7, &PF1[++i] );					// ã‡äzáNÅ®Åu22Åv	//áO->¢23£

	//îÑè„äÑçáÇÃåvéZ
	char wari[MONY_BUF_SIZE] = {0};
	char PW6[MONY_BUF_SIZE] = {0};
	char buf[10] = {0};
	char WORK0[MONY_BUF_SIZE] = {0};
	char WORK17ALL[10] = {0};

	char	PER100BUF[MONY_BUF_SIZE] = {0};
	m_Arith.l_input( PER100BUF, _T("10000000") );

	if( !Flg12 ){
		if( m_pSnHeadData->IsKobetuSiireAnbun() ){//å¬ï ëŒâûéû
			m_Arith.l_print( dp, PrintData->Sn_2F17C, FMT16_EX );
			PF1[i].PITM[0] = dp;
			dp += 19;		// áN				//áO
			j = 1;
			if( m_Arith.l_test( PrintData->Sn_2F18C ) ){
				m_Arith.l_print( dp, PrintData->Sn_2F18C, FMT16_EX );
				m_Arith.l_add( W0 , W0 , PrintData->Sn_2F18C );
			}
			else{
				if( m_pSnHeadData->Sn_UPERS&0x10 ){
					sprintf_s( buf, sizeof( buf ), _T("%d"), PrintData->Sn_2FUWSEX );
					m_Arith.l_input( wari, buf );
					m_Arith.l_mul100( (unsigned char *)WORK17ALL, (unsigned char *) PrintData->Sn_2F17C, (unsigned char *)wari, 2, 0 );
				}
				else{
					m_Arith.l_par100( (unsigned char *)wari, (unsigned char *)WORK4, (unsigned char *)WORK7,  5, 0 );
					// îÑè„äÑçáÇ™É}ÉCÉiÉXÇÃèÍçáÇÕÇOÅìÇ≈åvéZÇ∑ÇÈ
					memset( PW6, '\0', sizeof(PW6) );
					if( m_Arith.l_cmp(WORK4, PW6) < 0 ){
						memset( wari, '\0', MONY_BUF_SIZE );
					}
					if( m_Arith.l_cmp(WORK7, PW6) < 0 ){
						memset( wari, '\0', MONY_BUF_SIZE );
					}
					if( m_Arith.l_test(wari) == 0 ){
						m_Arith.l_mul100( (unsigned char *)WORK17ALL, (unsigned char *)PrintData->Sn_2F17C, (unsigned char *)wari, 5, 0 );
					}
					else{
						if( m_Arith.l_cmp(wari, PER100BUF) <= 0 ){
							m_Util.l_pardotnet_syz( WORK17ALL, (char *)PrintData->Sn_2F17C, WORK4, WORK7 );
						}
						else{
							m_Util.l_pardotnet_syz( WORK17ALL, (char *)PrintData->Sn_2F17C, WORK4, WORK4 );
						}
					}
				}
				m_Arith.l_add( WORK17ALL, WORK17ALL, PrintData->Sn_2F16C );
				m_Arith.l_print( dp, WORK17ALL, FMT16_EX );
				m_Arith.l_add( W0 , W0 , WORK17ALL );
			}
			PF1[i].PITM[1] = dp;
			dp += 19;		// áOÅ@ORÅ@áP		//áP or áQ
			PF1[i].PITM[2] = (char *)0;

		}
		else{
			//î‰ó·
			PF1[i].PITM[0] = (char *)0;
			PF1[i].PITM[1] = (char *)0;
			if( m_pSnHeadData->Sn_UPERS & 0x10 ){
				memset( WORK0 , '\0' , MONY_BUF_SIZE);
				sprintf_s( buf, sizeof( buf ), _T("%d"), PrintData->Sn_2FUWSEX );
				m_Arith.l_input( wari, buf );
			}
			else{
				m_Arith.l_par100( (unsigned char *)wari, (unsigned char *)WORK4,  (unsigned char *)WORK7,  5, 0 );
				// îÑè„äÑçáÇ™É}ÉCÉiÉXÇÃèÍçáÇÕÇOÅìÇ≈åvéZÇ∑ÇÈÅB
				memset( PW6, '\0', MONY_BUF_SIZE );
//-- '16.12.26 --
//				if( m_Arith.l_cmp(wari, PW6) < 0 ){
//					memset( wari, '\0', MONY_BUF_SIZE );
//				}
//---------------
				if( m_Arith.l_cmp(WORK4, PW6) < 0 ){
					memset( wari, '\0', MONY_BUF_SIZE );
				}
				if( m_Arith.l_cmp(WORK7, PW6) < 0 ){
					memset( wari, '\0', MONY_BUF_SIZE );
				}
//---------------
			}

			if( m_Arith.l_test( PrintData->Sn_2F19C )){
				m_Arith.l_print( dp, PrintData->Sn_2F19C, FMT16_EX );
				m_Arith.l_add( W0 , W0 , PrintData->Sn_2F19C );
			}
			else{
				if( m_pSnHeadData->Sn_UPERS & 0x10 ){
					m_Arith.l_mul100( (unsigned char *)WORK0, (unsigned char *)kingaku2, (unsigned char *)wari, 2, 0 );//12*4/7
					m_Arith.l_print( dp, WORK0 , FMT16_EX );
					m_Arith.l_add( W0 , W0 , WORK0 );

				}
				else{
					if( m_Arith.l_test( wari ) == 0 ){
						m_Arith.l_mul100( (unsigned char *)WORK0, (unsigned char *)kingaku2, (unsigned char *)wari, 5, 0 );//12*4/7

					}
					else{
						if( m_Arith.l_cmp(wari, PER100BUF) <= 0 ){
							m_Util.l_pardotnet_syz( WORK0, kingaku2, WORK4, WORK7 );
						}
						else{
							m_Util.l_pardotnet_syz( WORK0, kingaku2, WORK4, WORK4 );
						}
					}
					m_Arith.l_print( dp, WORK0 , FMT16_EX );
					m_Arith.l_add( W0 , W0 , WORK0 );
				}
			}
			PF1[i].PITM[2] = dp;
			dp += 19;	// áOÅ@ORÅ@áP		//áP or áQ
		}
	
	}
	else{
		PF1[i].PITM[0] = (char *)0;
		PF1[i].PITM[1] = (char *)0;
		PF1[i].PITM[2] = (char *)0;
	}

	m_Arith.l_print( dp, PrintData->Sn_2F20C, FMT16_EX );
	PF1[i].PITM[3] = dp;
	dp += 19;		// áQ		
	m_Arith.l_add( W0 , W0 , PrintData->Sn_2F20C );

	//áR
	m_Arith.l_print( dp, PrintData->Sn_2F21C, FMT16_EX );
	PF1[i].PITM[4] = dp;
	dp += 19;		// áR		
	m_Arith.l_add( W0 , W0 , PrintData->Sn_2F21C );
	//áS
	if( PrintData->Sn_2F22sw & 0x100 ){
		memset( W0 , '\0' , MONY_BUF_SIZE );
		memmove( W0 , PrintData->Sn_2F22C , MONY_BUF_SIZE );
	}
	if( m_Arith.l_test( W0 ) > 0 ){					// Åi20ÅjorÅi21Åj	//(21)or(22)
		m_Arith.l_print( dp, W0, FMT16_EX );
		PF1[i].PITM[5] = dp;	dp += 19;
		j++;
	}
	else if( m_Arith.l_test( W0 ) < 0 ){
		j++;
		m_Arith.l_neg( W0 );
		m_Arith.l_print( dp, W0, FMT16_EX );
		PF1[i].PITM[6] = dp;
		dp += 19;
	}

	m_Arith.l_print( dp, PrintData->Sn_2F24C, FMT16_EX );
	PF1[i].PITM[7] = dp;
	dp += 19;		//Å@Åi22Åj			//(23)
	
	if( j = _xprn( PP, 2, PF1, 0, pDC, 0, 1 ) ){
		goto POUT2;
	}

	// éÅñº
	dp = AP;
	i = j = 0;
	memset( dp, '\0', size );
	if( kojin ){	// å¬êléñã∆é“ÇÃèÍçáÇÕë„ï\é“éÅñº
		j = sizeof( m_pSnHeadData->Sn_DIHYO );
		memmove( dp, m_pSnHeadData->Sn_DIHYO, j );
	}
	else {			// ñ@êlñº
		j = sizeof( m_pSnHeadData->Sn_CONAM );
		memmove( dp, m_pSnHeadData->Sn_CONAM, j );
	}
	PF1[0].PITM[0] = dp;
	if( j = _xprn( PP, 3, PF1, 2, pDC, 0, 1 ) ){
		goto POUT2;
	}

POUT2:
	if( AP ){
		delete [] AP;
	}
	
	return j;
}
//2015.08.04 INSERT END

//-----------------------------------------------------------------------------
// ïtï\ÇTèoóÕÅ@ê¥êÖï“èW
//-----------------------------------------------------------------------------
// à¯êî	
//
//
//-----------------------------------------------------------------------------
// ï‘ëóíl
//-----------------------------------------------------------------------------
int CH26HyoView::PrintFuhyo50( CDC *pDC, int kojin, int pcnt )
{
	int				i, j;
	unsigned int	size;
	char			*AP, *dp;
	
	BOOL	isNoImage = FALSE;
	if( m_pPrintCmInfo->PRtype&0x80 ){
		isNoImage = FALSE;	// ÉCÉÅÅ[ÉWèoóÕóLÇË
	}
	else{
		isNoImage = TRUE;	// ÉCÉÅÅ[ÉWèoóÕóLÇË
	}
	
	size = 512;
	if( (AP = new char[size]) == NULL ){
		return -1;
	}
	
PBUF	PF1[30];
int		PP;

	// èoóÕëOèWåv
//	FuHyo52Calqe();
	PP = 6;
	dp = AP;
	memset( dp, '\0', size );
	i = 0;
	char	WORK0[MONY_BUF_SIZE] = {0};
	char	WORK_ALL[MONY_BUF_SIZE] = {0};

int CCSw = 0;

	// ã≠êßâ€ê≈ä˙ä‘àÛç¸
	if( CCSw ){	// ã≠êßä˙ä‘ïœçX
//		wsprintf( dp, _T("%02X %02X %02X"), CC_KDAYS[0]&0xff, CC_KDAYS[1]&0xff, CC_KDAYS[2]&0xff );
//		PF1[i].PITM[0] = dp;
//		dp += 9;
//		wsprintf( dp, _T("%02X %02X %02X"), CC_KDAYE[0]&0xff, CC_KDAYE[1]&0xff, CC_KDAYE[2]&0xff );
//		PF1[i++].PITM[1] = dp;
//		dp += 9;
	}
	else{
		char yymmdd[3] = {0};
		if( IsJcContinuePrint(true) ){
			m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		else{
			m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		wsprintf( dp, _T("%02X %02X %02X"), yymmdd[0]&0xff, yymmdd[1]&0xff, yymmdd[2]&0xff );
		PF1[i].PITM[0] = dp;
		dp += 9;
		memset( yymmdd, '\0', sizeof(yymmdd) );
		if( IsJcContinuePrint(true) ){
			m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		else{
			m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		wsprintf( dp, _T("%02X %02X %02X"), yymmdd[0]&0xff, yymmdd[1]&0xff, yymmdd[2]&0xff );
		PF1[i++].PITM[1] = dp;
		dp += 9;
	}


	memmove( dp, _T("Å@"), 2 );
	PF1[i++].PITM[0] = dp;
	dp += (2 + 1);

	j = 0;										// ã‡äzá@->áD
	m_Arith.l_print( dp , m_pSnFhyo40Data->Sn_1F2C ,  FMT15 );//è¡Ç∑SHIMIZU_TEST
	PF1[i].PITM[j++] = dp;	dp += 16;			// á@
	m_Arith.l_print( dp , m_pSnFhyo40Data->Sn_1F3C ,  FMT15 );//è¡Ç∑SHIMIZU_TEST
	PF1[i].PITM[j++] = dp;	dp += 16;			// áA

	m_Arith.l_print( dp , m_pSnFhyo40Data->Sn_1F5C ,  FMT15 );//è¡Ç∑SHIMIZU_TEST
	PF1[i].PITM[j++] = dp;	dp += 16;			// áB

	m_Arith.l_add( WORK0 , m_pSnFhyo40Data->Sn_1F2C ,  m_pSnFhyo40Data->Sn_1F3C );
	m_Arith.l_sub( WORK0 , WORK0 ,  m_pSnFhyo40Data->Sn_1F5C );
	if( m_Arith.l_test( WORK0 ) < 0 ){
		memset( WORK0, '\0', MONY_BUF_SIZE );
	}
	m_Arith.l_print( dp , WORK0 ,  FMT15 );//è¡Ç∑SHIMIZU_TEST
	PF1[i].PITM[j++] = dp;	dp += 16;			// áC

	int Flg = 0;
	if( m_Arith.l_test( m_pSnHonpyoData->Sn_KURI1 )) Flg++;
	if( m_Arith.l_test( m_pSnHonpyoData->Sn_KURI2 )) Flg++;
	if( m_Arith.l_test( m_pSnHonpyoData->Sn_KURI3 )) Flg++;
	if( m_Arith.l_test( m_pSnHonpyoData->Sn_KURI4 )) Flg++;
	if( m_Arith.l_test( m_pSnHonpyoData->Sn_KURI5 )) Flg++;

	if( Flg == 1 ){
		m_Arith.l_print( dp , m_pSnFhyo40Data->Sn_1F4C ,  FMT15 );//è¡Ç∑SHIMIZU_TEST
		PF1[i++].PITM[j] = dp;	dp += 16;			// áD
	}
	else{
		PF1[i++].PITM[j] = (char *)0;
	}
	
	j = 0;											// ã‡äzáE->áJ
	if( Flg >= 2 ){		// ï°êîéñã∆é“î≠ê∂éûÇÃÇ›
//-- '15.02.10 --
//		m_Arith.l_print( dp, m_pSnHeadData->Sn_KURIA, FMT15 );
//---------------
		if( m_Arith.l_test(m_pSnHeadData->Sn_KURIA) ||
			m_Arith.l_test(m_pSnHonpyoData->Sn_KURI1) || m_Arith.l_test(m_pSnHonpyoData->Sn_KURI2) || m_Arith.l_test(m_pSnHonpyoData->Sn_KURI3) ||
			m_Arith.l_test(m_pSnHonpyoData->Sn_KURI4) || m_Arith.l_test(m_pSnHonpyoData->Sn_KURI5) ){
			m_Arith.l_print( dp, m_pSnHeadData->Sn_KURIA, FMT16 );
		}
		else{
			m_Arith.l_print( dp, m_pSnHeadData->Sn_KURIA, FMT15 );
		}
//---------------
		PF1[i].PITM[j++] = dp;
		dp += 16;			// áE
//-- '15.01.26 --
//		m_Arith.l_print( dp, m_pSnHonpyoData->Sn_KURI1, FMT15 );
//		PF1[i].PITM[j++] = dp;
//		dp += 16;			// áF
//		m_Arith.l_print( dp, m_pSnHonpyoData->Sn_KURI2, FMT15 );
//		PF1[i].PITM[j++] = dp;
//		dp += 16;			// áG
//		m_Arith.l_print( dp, m_pSnHonpyoData->Sn_KURI3, FMT15 );
//		PF1[i].PITM[j++] = dp;
//		dp += 16;			// áH
//		m_Arith.l_print( dp, m_pSnHonpyoData->Sn_KURI4, FMT15 );
//		PF1[i].PITM[j++] = dp;
//		dp += 16;			// áI
////		if( ! TBhead.SV_5sw ){
//		if( 1 ){
//			m_Arith.l_print( dp, m_pSnHonpyoData->Sn_KURI5, FMT15 );
//			PF1[i++].PITM[j] = dp;
//			dp += 16;		// áJ
//		}
//		else{
//			PF1[i++].PITM[j] = (char *)0;
//		}
//---------------
		char	zeroMony[MONY_BUF_SIZE]={0};
		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI1) >= 0 ){
			m_Arith.l_print( dp, m_pSnHonpyoData->Sn_KURI1, FMT15 );
		}
		else{
			m_Arith.l_print( dp, zeroMony, FMT16 );
		}
		PF1[i].PITM[j++] = dp;
		dp += 16;			// áF
		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI2) >= 0 ){
			m_Arith.l_print( dp, m_pSnHonpyoData->Sn_KURI2, FMT15 );
		}
		else{
			m_Arith.l_print( dp, zeroMony, FMT16 );
		}
		PF1[i].PITM[j++] = dp;
		dp += 16;			// áG
		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI3) >= 0 ){
			m_Arith.l_print( dp, m_pSnHonpyoData->Sn_KURI3, FMT15 );
		}
		else{
			m_Arith.l_print( dp, zeroMony, FMT16 );
		}
		PF1[i].PITM[j++] = dp;
		dp += 16;			// áH
		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI4) >= 0 ){
			m_Arith.l_print( dp, m_pSnHonpyoData->Sn_KURI4, FMT15 );
		}
		else{
			m_Arith.l_print( dp, zeroMony, FMT16 );
		}
		PF1[i].PITM[j++] = dp;
		dp += 16;			// áI
		if( m_Arith.l_test(m_pSnHonpyoData->Sn_KURI5) >= 0 ){
			m_Arith.l_print( dp, m_pSnHonpyoData->Sn_KURI5, FMT15 );
		}
		else{
			m_Arith.l_print( dp, zeroMony, FMT16 );
		}
		PF1[i++].PITM[j] = dp;
		dp += 16;		// áJ
//---------------
		
		j = 0;										// äÑçááF->áJ
		if( m_pSnHonpyoData->Sn_UP1 ){ 
			wsprintf( dp, _T("%3d.%01d"), m_pSnHonpyoData->Sn_UP1 / 10, m_pSnHonpyoData->Sn_UP1 % 10 );
		}
		PF1[i].PITM[j++] = dp;
		dp += 6;		// îÑè„äÑçááF
		if( m_pSnHonpyoData->Sn_UP2 ){
			wsprintf( dp, _T("%3d.%01d"), m_pSnHonpyoData->Sn_UP2 / 10, m_pSnHonpyoData->Sn_UP2 % 10 );
		}
		PF1[i].PITM[j++] = dp;
		dp += 6;		// îÑè„äÑçááG
		if( m_pSnHonpyoData->Sn_UP3 ){ 
			wsprintf( dp, _T("%3d.%01d"), m_pSnHonpyoData->Sn_UP3 / 10, m_pSnHonpyoData->Sn_UP3 % 10 );
		}
		PF1[i].PITM[j++] = dp;	
		dp += 6;		// îÑè„äÑçááH
		if( m_pSnHonpyoData->Sn_UP4 ){
			wsprintf( dp, _T("%3d.%01d"), m_pSnHonpyoData->Sn_UP4 / 10, m_pSnHonpyoData->Sn_UP4 % 10 );
		}
		PF1[i].PITM[j++] = dp;
		dp += 6;		// îÑè„äÑçááI
		if( m_pSnHonpyoData->Sn_UP5 ){ 
			wsprintf( dp, _T("%3d.%01d"), m_pSnHonpyoData->Sn_UP5 / 10, m_pSnHonpyoData->Sn_UP5 % 10 );
		}
		PF1[i++].PITM[j] = dp;
		dp += 6;		// îÑè„äÑçááJ
		
		j = 0;											// ê≈äzáK->áP
//-- '15.02.10 --
//		m_Arith.l_print( dp, m_pSnFhyo50Data->Sn_5FUZTT, FMT15 );
//---------------
		if( m_Arith.l_test(m_pSnFhyo50Data->Sn_5FUZTT) ||
			m_Arith.l_test(m_pSnFhyo50Data->Sn_5FUZT1) || m_Arith.l_test(m_pSnFhyo50Data->Sn_5FUZT2) || m_Arith.l_test(m_pSnFhyo50Data->Sn_5FUZT3) ||
			m_Arith.l_test(m_pSnFhyo50Data->Sn_5FUZT4) || m_Arith.l_test(m_pSnFhyo50Data->Sn_5FUZT5) ){
			m_Arith.l_print( dp, m_pSnFhyo50Data->Sn_5FUZTT, FMT16 );
		}
		else{
			m_Arith.l_print( dp, m_pSnFhyo50Data->Sn_5FUZTT, FMT15 );
		}
//---------------
		PF1[i].PITM[j++] = dp;
		dp += 16;			// áK

//-- '15.01.26 --
//		m_Arith.l_print( dp, m_pSnFhyo50Data->Sn_5FUZT1, FMT15 );
//		PF1[i].PITM[j++] = dp;
//		dp += 16;			// áL
//		m_Arith.l_print( dp, m_pSnFhyo50Data->Sn_5FUZT2, FMT15 );
//		PF1[i].PITM[j++] = dp;	
//		dp += 16;			// áM
//		m_Arith.l_print( dp, m_pSnFhyo50Data->Sn_5FUZT3, FMT15 );
//		PF1[i].PITM[j++] = dp;
//		dp += 16;			// áN
//		m_Arith.l_print( dp, m_pSnFhyo50Data->Sn_5FUZT4, FMT15 );
//		PF1[i].PITM[j++] = dp;
//		dp += 16;			// áO
////		if( !TBhead.SV_5sw ){
//		if( 1 ){
//			m_Arith.l_print( dp, m_pSnFhyo50Data->Sn_5FUZT5, FMT15 );
//			PF1[i].PITM[j] = dp;
//			dp += 16;		// áP
//		}
//		else{
//			PF1[i].PITM[j] = (char *)0;
//		}
//---------------
		if( m_Arith.l_test(m_pSnFhyo50Data->Sn_5FUZT1) >= 0 ){
			m_Arith.l_print( dp, m_pSnFhyo50Data->Sn_5FUZT1, FMT15 );
		}
		else{
			m_Arith.l_print( dp, zeroMony, FMT16 );
		}
		PF1[i].PITM[j++] = dp;
		dp += 16;			// áL
		if( m_Arith.l_test(m_pSnFhyo50Data->Sn_5FUZT2) >= 0 ){
			m_Arith.l_print( dp, m_pSnFhyo50Data->Sn_5FUZT2, FMT15 );
		}
		else{
			m_Arith.l_print( dp, zeroMony, FMT16 );
		}
		PF1[i].PITM[j++] = dp;	
		dp += 16;			// áM
		if( m_Arith.l_test(m_pSnFhyo50Data->Sn_5FUZT3) >= 0 ){
			m_Arith.l_print( dp, m_pSnFhyo50Data->Sn_5FUZT3, FMT15 );
		}
		else{
			m_Arith.l_print( dp, zeroMony, FMT16 );
		}
		PF1[i].PITM[j++] = dp;
		dp += 16;			// áN
		if( m_Arith.l_test(m_pSnFhyo50Data->Sn_5FUZT4) >= 0 ){
			m_Arith.l_print( dp, m_pSnFhyo50Data->Sn_5FUZT4, FMT15 );
		}
		else{
			m_Arith.l_print( dp, zeroMony, FMT16 );
		}
		PF1[i].PITM[j++] = dp;
		dp += 16;			// áO
		if( m_Arith.l_test(m_pSnFhyo50Data->Sn_5FUZT5) >= 0 ){
			m_Arith.l_print( dp, m_pSnFhyo50Data->Sn_5FUZT5, FMT15 );
		}
		else{
			m_Arith.l_print( dp, zeroMony, FMT16 );
		}
		PF1[i].PITM[j] = dp;
		dp += 16;		// áP
//---------------
	}
	else{		// íPàÍéñã∆
		pbclr( 0, 6, &PF1[i++] );
		pbclr( 0, 5, &PF1[i++] );
		pbclr( 0, 6, &PF1[i++] );
	}
	
	if( j = _xprn( PP, 1, PF1, 0, pDC, isNoImage, 1 ) ){
		goto POUT5;
	}

	i = j = 0;

	if( Flg == 1 ){
		// íPàÍéñã∆é“
		// éÅñº
		dp = AP;	i = j = 0;
		memset( dp, '\0', size );
		if( kojin ){	// å¬êléñã∆é“ÇÃèÍçáÇÕë„ï\é“éÅñº
			j = sizeof( m_pSnHeadData->Sn_DIHYO );
			memmove( dp, m_pSnHeadData->Sn_DIHYO, j );
		}
		else {			// ñ@êlñº
			j = sizeof( m_pSnHeadData->Sn_CONAM );
			memmove( dp, m_pSnHeadData->Sn_CONAM, j );
		}
		PF1[0].PITM[0] = dp;
		if( j = _xprn( PP, 3, PF1, 2, pDC, 0, 1 ) ){
			goto POUT5;
		}

		// íPàÍéñã∆é“Å@Åõ
		dp = AP;	i = j = 0;
		memset( dp, '\0', size );
		pbclr( 0, 5, &PF1[0] );			// [07'09.25]

		if( m_Arith.l_test( m_pSnFhyo50Data->Sn_5FUZT1 )){
			PF1[0].PITM[0] = _T("Å@");
		}
		if( m_Arith.l_test( m_pSnFhyo50Data->Sn_5FUZT2 )){
			PF1[0].PITM[1] = _T("Å@");
		}
		if( m_Arith.l_test( m_pSnFhyo50Data->Sn_5FUZT3 )){
			PF1[0].PITM[2] = _T("Å@");
		}
		if( m_Arith.l_test( m_pSnFhyo50Data->Sn_5FUZT4 )){
			PF1[0].PITM[3] = _T("Å@");
		}
		if( m_Arith.l_test( m_pSnFhyo50Data->Sn_5FUZT5 )){
			PF1[0].PITM[4] = _T("Å@");
		}
		m_pVprn->SetSfncEnv( SElfnc, 1 );
		if( j = _xprn( PP, 4, PF1, 0, pDC, 0, 1 ) ){
			goto POUT5;
		}

		goto POUT5;		// íPàÍéñã∆
	}

	dp = AP;
	int m_75sw = 0;
	memset( dp, '\0', size );
	pbclr( 0, 2,  &PF1[i] );
	pbclr( 0, 10, &PF1[i+1] );
	m_Arith.l_print( dp, m_pSnFhyo50Data->Sn_5FSZTG, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;		// áQ

	if( m_pSnFhyo50Data->Sn_5FSZsw  == 1 ){
		m_Arith.l_print( dp, m_pSnFhyo50Data->Sn_5FSZTT, FMT15 );
		PF1[i].PITM[j++] = dp;
		dp += 16;	// áR
	}
	else{
		PF1[i].PITM[j++] = (char *)0;
	}
	i++;
//	}
// ä»à’â€ê≈Å@2éÌóﬁà»è„ÇÃã∆éÌÇ≈ÇÃì¡ó·åvéZÉ`ÉFÉbÉN
//2éÌóﬁéñã∆Ç≈75%à»è„
	char val[MONY_BUF_SIZE] = {0};
	for( j = 0; j<10; ++j ){			// áS->(29)
		if( ( m_pSnFhyo50Data->Sn_5FSZsw - 2) == j ){
			switch( m_pSnFhyo50Data->Sn_5FSZsw ){
				case 2:
					m_Arith.l_print( dp, m_pSnFhyo50Data->Sn_5FSZT20C, FMT15 );
					break;
				case 3:
					m_Arith.l_print( dp, m_pSnFhyo50Data->Sn_5FSZT21C, FMT15 );
					break;			
				case 4:
					m_Arith.l_print( dp, m_pSnFhyo50Data->Sn_5FSZT22C, FMT15 );
					break;			
				case 5:
					m_Arith.l_print( dp, m_pSnFhyo50Data->Sn_5FSZT23C, FMT15 );
					break;			
				case 6:
					m_Arith.l_print( dp, m_pSnFhyo50Data->Sn_5FSZT24C, FMT15 );
					break;
				case 7:
					m_Arith.l_print( dp, m_pSnFhyo50Data->Sn_5FSZT25C, FMT15 );
					break;			
				case 8:
					m_Arith.l_print( dp, m_pSnFhyo50Data->Sn_5FSZT26C, FMT15 );
					break;			
				case 9:
					m_Arith.l_print( dp, m_pSnFhyo50Data->Sn_5FSZT27C, FMT15 );
					break;			
				case 10:
					m_Arith.l_print( dp, m_pSnFhyo50Data->Sn_5FSZT28C, FMT15 );
					break;			
				case 11:
					m_Arith.l_print( dp, m_pSnFhyo50Data->Sn_5FSZT29C, FMT15 );
					break;			
			}
			PF1[i].PITM[j] = dp;
			dp += 16;
		}
		else{
			PF1[i].PITM[j] = (char *)0;
		}
	}
	
	m_Arith.l_print( dp, m_pSnFhyo40Data->Sn_1F4C, FMT15 );
	PF1[++i].PITM[0] = dp;
	dp += 16;		// (30)
	
	if( j = _xprn( PP, 2, PF1, 0, pDC, 0, 1 ) ){
		goto POUT5;
	}

	// éÅñº
	dp = AP;	i = j = 0;
	memset( dp, '\0', size );
	if( kojin ){	// å¬êléñã∆é“ÇÃèÍçáÇÕë„ï\é“éÅñº
		j = sizeof( m_pSnHeadData->Sn_DIHYO );
		memmove( dp, m_pSnHeadData->Sn_DIHYO, j );
	}
	else {			// ñ@êlñº
		j = sizeof( m_pSnHeadData->Sn_CONAM );
		memmove( dp, m_pSnHeadData->Sn_CONAM, j );
	}
	PF1[0].PITM[0] = dp;
	if( j = _xprn( PP, 3, PF1, 2, pDC, 0, 1 ) ){
		goto POUT5;
	}

	// ì¡ó·åvéZÇìKópÇ≈ÇPéÌóﬁÇÃéñã∆Ç≈ÇVÇTÅìà»è„ÇÃèÍçá
	if( m_pSnFhyo50Data->Sn_5FSZsw == 1){
		dp = AP;
		i = j = 0;
		memset( dp, '\0', size );
		pbclr( 0, 5,  &PF1[i] );

		if( m_pSnHonpyoData->Sn_UP1 >= 750 ){
			PF1[0].PITM[0] = _T("Å@");
		}
		if( m_pSnHonpyoData->Sn_UP2 >= 750 ){
			PF1[0].PITM[1] = _T("Å@");
		}
		if( m_pSnHonpyoData->Sn_UP3 >= 750 ){
			PF1[0].PITM[2] = _T("Å@");
		}
		if( m_pSnHonpyoData->Sn_UP4 >= 750 ){
			PF1[0].PITM[3] = _T("Å@");
		}
		if( m_pSnHonpyoData->Sn_UP5 >= 750 ){
			PF1[0].PITM[4] = _T("Å@");
		}
		//ÉtÉåÅ[ÉÄéÌÇóLå¯Ç∆Ç∑ÇÈà◊ÇÃëOèàóù
		m_pVprn->SetSfncEnv( SElfnc, 1 );
		if( j = _xprn( PP, 5, PF1, 0, pDC, 0, 1 ) ){
			goto POUT5;
		}
	}

POUT5:
	if( AP ){
		delete [] AP;
	}
	
	return j;
}

//-----------------------------------------------------------------------------
// ïtï\ÇQÅ|ÇQèoóÕ ê¥êÖï“èWÅ@ã‡äzÇÃÇ∆Ç±ÇÎå„Ç≈
//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CH26HyoView::PrintFuhyo22( CDC *pDC, int kojin, int pcnt )
{
	int				i, j;
	unsigned int	size;
	char			*AP, *dp, ii;

	unsigned short	SEno = 1;
	if( m_pPrintCmInfo->PRtype&0x80 ){
//		if( m_TaxReforms )	SEno = MAKEWORD( 2, 2 );	// ïΩê¨ÇQÇRîNè¡îÔê≈â¸ê≥î≈
//		else				SEno = MAKEWORD( 1, 1 );
//		SEno = MAKEWORD( 3, 3 );	// ïΩê¨ÇQÇRîNè¡îÔê≈â¸ê≥î≈
//	}
//	else{
//		SEno = PcSw;
	}
	
	BOOL	isNoImage = FALSE;
	if( m_pPrintCmInfo->PRtype&0x80 ){
		isNoImage = 0;	// ÉCÉÅÅ[ÉWèoóÕóLÇË
	}
	else{
		isNoImage = 1;	// ÉCÉÅÅ[ÉWèoóÕñ≥Çµ
	}

	size = 512;
	if( (AP = new char[size]) == NULL ){
		return -1;
	}
	
	// èoóÕëOèWåv
//	FuHyo22Calqe();

	ASSERT( m_pSnFhyo22Data );
	if( m_pSnFhyo22Data == NULL ){
		return 0;
	}

PBUF	PF1[30];
int		PP;

	PP = 7;

	dp = AP;
	memset( dp, '\0', size );
	i = 0;

int	CCSw = 0;
char	WORK0[MONY_BUF_SIZE] = {0};
char	W0[MONY_BUF_SIZE] = {0};
char	WORK4[MONY_BUF_SIZE] = {0};
char	WORK7[MONY_BUF_SIZE] = {0};
char	WORK16A[MONY_BUF_SIZE] = {0};
char	WORK16B[MONY_BUF_SIZE] = {0};
char	WORK16C[MONY_BUF_SIZE] = {0};
char	WORK16D[MONY_BUF_SIZE] = {0};
char	WORK11D[MONY_BUF_SIZE] = {0};
char	WORK12A[MONY_BUF_SIZE] = {0};
char	WORK12B[MONY_BUF_SIZE] = {0};
char	WORK12C[MONY_BUF_SIZE] = {0};
char	WORK12D[MONY_BUF_SIZE] = {0};

char	WORK20A[MONY_BUF_SIZE] = {0};
char	WORK20B[MONY_BUF_SIZE] = {0};
char	WORK20C[MONY_BUF_SIZE] = {0};

//2016.10.20 INSERT START
char	YEN_ZERO[MONY_BUF_SIZE] = {0}; //0â~ï\ãLóp
//2016.10.20 INSERT END


int	FontFlg = 0;//0:12åÖà»â∫ÇÃèÍçáÅAëÂÇ´Ç≠ï\é¶ 1:13åÖà»è„ÇÃèÍçáÅAè¨Ç≥Ç≠Ç∑ÇÈ
CString str = _T("");

	// ã≠êßâ€ê≈ä˙ä‘àÛç¸
	if( CCSw ){	// ã≠êßä˙ä‘ïœçX
//		wsprintf( dp, "%02X %02X %02X", CC_KDAYS[0]&0xff, CC_KDAYS[1]&0xff, CC_KDAYS[2]&0xff );
//		PF1[i].PITM[0] = dp;
//		dp += 9;
//		wsprintf( dp, "%02X %02X %02X", CC_KDAYE[0]&0xff, CC_KDAYE[1]&0xff, CC_KDAYE[2]&0xff );
//		PF1[i++].PITM[1] = dp;
//		dp += 9;
	}
	else{
		char	yymmdd[3] = {0};
		if( IsJcContinuePrint(true) ){
			m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		else{
			m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		wsprintf( dp, _T("%02X %02X %02X"), yymmdd[0]&0xff, yymmdd[1]&0xff, yymmdd[2]&0xff );
		PF1[i].PITM[0] = dp;
		dp += 9;
		memset( yymmdd, '\0', sizeof(yymmdd) );
		if( IsJcContinuePrint(true) ){
			m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		else{
			m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		wsprintf( dp, _T("%02X %02X %02X"), yymmdd[0]&0xff, yymmdd[1]&0xff, yymmdd[2]&0xff );
		PF1[i++].PITM[1] = dp;
		dp += 9;
	}

	memmove( dp, _T("Å@"), 2 );
	PF1[i++].PITM[0] = dp;	
	dp += (2 + 1);
	
	if( j = _xprn( PP, 1, PF1, 0, pDC, isNoImage, SEno ) ){
		goto POUT22;
	}
	i = 0;
	j = 0;										// ã‡äzá@->áB
	// á@-A
	m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F1A, FMT16_EX );
	
	PF1[i].PITM[j++] = dp;
	dp += 19;
	// á@-B
	m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F1B, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	// á@-C
	m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F1C, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	// á@-D
	m_Arith.l_add( WORK0, m_pSnFhyo22Data->Sn_2F1A, m_pSnFhyo22Data->Sn_2F1B );
	m_Arith.l_add( WORK0, WORK0, m_pSnFhyo22Data->Sn_2F1C );
	m_Arith.l_print( dp, WORK0, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// áA-A
	PF1[i].PITM[j++] = (char *)0;
	// áA-B
	PF1[i].PITM[j++] = (char *)0;
	// áA-C
	PF1[i].PITM[j++] = (char *)0;
	// áA-D
	m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F2C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	m_Arith.l_add( WORK0, WORK0, m_pSnFhyo22Data->Sn_2F2C );//ë´ÇµÇ±Ç›
	
	// áB-A
	PF1[i].PITM[j++] = (char *)0;
	// áB-B
	PF1[i].PITM[j++] = (char *)0;
	// áB-C
	PF1[i].PITM[j++] = (char *)0;
	// áB-D
	m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F3C, FMT15 );
	PF1[i++].PITM[j] = dp;
	dp += 16;
	m_Arith.l_add( WORK0, WORK0, m_pSnFhyo22Data->Sn_2F3C );//ë´ÇµÇ±Ç›

	// ã‡äzáC•áD•áF
	j = 0;
	// áC
//2016.06.22 UPDATE START
//	m_Arith.l_print( dp, WORK0, FMT16_EX );
	if( m_Arith.l_test( WORK0 ) >= 0 ){
		m_Arith.l_print( dp, WORK0, FMT16_EX );
	}else{
		m_Arith.l_print( dp, YEN_ZERO, FMT16_EXZ );
	}
//2016.06.22 UPDATE END
	PF1[i].PITM[j++] = dp;
//2016.06.22 INSERT START
	dp += 19;
	m_Arith.l_print( dp, WORK0, FMT16_EX );
//2016.06.22 INSERT END
	m_Arith.l_add( WORK4, WORK4, WORK0 );//ë´ÇµÇ±Ç›
	// áD
	PF1[i].PITM[j++] = dp;
	dp += 19;
	// áE
	m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F6C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	// áF
	m_Arith.l_add( WORK0, WORK0, m_pSnFhyo22Data->Sn_2F6C );//ë´ÇµÇ±Ç›
//2016.06.22 UPDATE START
//	m_Arith.l_print( dp, WORK0, FMT16_EX );
	if( m_Arith.l_test( WORK0 ) >= 0 ){
		m_Arith.l_print( dp, WORK0, FMT16_EX );
	}else{
		m_Arith.l_print( dp, YEN_ZERO, FMT16_EXZ );
	}
//2016.06.22 UPDATE END
	PF1[i++].PITM[j] = dp;
	dp += 19;
	m_Arith.l_add( WORK7, WORK7, WORK0 );//ë´ÇµÇ±Ç›
	
	j = 0;
	// ã‡äzáE•áG
	// áG-A
	m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F8A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	// áG-B
	m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F8B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	// áG-C
	m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F8C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	// áG-D
	m_Arith.l_add( WORK0, m_pSnFhyo22Data->Sn_2F8A, m_pSnFhyo22Data->Sn_2F8B );
	m_Arith.l_add( WORK0, WORK0, m_pSnFhyo22Data->Sn_2F8C );
	m_Arith.l_print( dp, WORK0, FMT15 );
	PF1[i++].PITM[j] = dp;
	dp += 16;
	
	// îÑè„äÑçá
	wsprintf( dp, _T("%3d.%02d"), m_pSnFhyo22Data->Sn_2FUWS / 100, m_pSnFhyo22Data->Sn_2FUWS % 100 );
	PF1[i++].PITM[0] = dp;
	dp += 7;

	for( int t = 0; t < 12; t++){
		if( PF1[0].PITM[t] == NULL ) continue;
		str.Format( _T("%s") , PF1[0].PITM[t] );
		str.TrimLeft();
		if( str.GetLength() > 15 ){
			FontFlg = 1;
			break;
		}
	}
	if( !FontFlg ){
		for( int t = 0; t < 4; t++){
			if( PF1[1].PITM[t] == NULL ) continue;
			str.Format( _T("%s") , PF1[1].PITM[t] );
			str.TrimLeft();
			if( str.GetLength() > 15 ){
				FontFlg = 1;
				break;
			}
		}
	}
	if( !FontFlg ){
		for( int t = 0; t < 4; t++){
			if( PF1[2].PITM[t] == NULL ) continue;
			str.Format( _T("%s") , PF1[2].PITM[t] );
			str.TrimLeft();
			if( str.GetLength() > 15 ){
				FontFlg = 1;
				break;
			}
		}
	}
	if( FontFlg ){
		if( j = _xprn( PP, 3, PF1, 0, pDC, isNoImage, SEno ) ){
			goto POUT22;
		}
	}
	else{
		if( j = _xprn( PP, 2, PF1, 0, pDC, isNoImage, SEno ) ){
			goto POUT22;
		}
	}
	dp = AP;
	i = j = 0;
	memset( dp, '\0', size );
	
	// ã‡äzáHÅEáJ		//áH•áK	
	// áH-A
	m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F9A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	m_Arith.l_add( WORK12A , WORK12A , m_pSnFhyo22Data->Sn_2F9A );
	// áH-B
	m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F9B, FMT15 );
	PF1[i].PITM[j++] = dp;
	m_Arith.l_add( WORK12B , WORK12B , m_pSnFhyo22Data->Sn_2F9B );
	dp += 16;
	// áH-C
	m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F9C, FMT15 );
	PF1[i].PITM[j++] = dp;	
	m_Arith.l_add( WORK12C , WORK12C , m_pSnFhyo22Data->Sn_2F9C );
	dp += 16;
	// áH-D
	m_Arith.l_add( WORK0, m_pSnFhyo22Data->Sn_2F9A, m_pSnFhyo22Data->Sn_2F9B );
	m_Arith.l_add( WORK0, WORK0, m_pSnFhyo22Data->Sn_2F9C );
	m_Arith.l_print( dp, WORK0, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	m_Arith.l_add( WORK12D , WORK12D , WORK0 );

/*- '14.08.21 -*/
//	if( m_pSnHeadData->IsKobetuSiireAnbun() ){
//		// áJ-A		// áK-A
//		m_Arith.l_add( WORK0 , m_pSnTanaData->Sn_menzz[0] , m_pSnTanaData->Sn_menzz[1] );
//		m_Arith.l_add( WORK0 , WORK0 , m_pSnTanaData->Sn_menzz[2] );
//		if( m_pSnTanaData->Sn_MENZsw & 0x01 ) m_Arith.l_neg( WORK0 );
//		m_Arith.l_print( dp, WORK0, FMT15 );
//		PF1[i].PITM[j++] = dp;	
//		dp += 16;
//		m_Arith.l_add( WORK12A , WORK12A , WORK0 );
//		m_Arith.l_add( WORK11D , WORK11D , WORK0 );
//
//		// áJ-BÅ@	// áK-B
//		m_Arith.l_add( WORK0 , m_pSnTanaData->Sn_mnz40[0] , m_pSnTanaData->Sn_mnz40[1] );
//		m_Arith.l_add( WORK0 , WORK0 , m_pSnTanaData->Sn_mnz40[2] );
//		if( m_pSnTanaData->Sn_MENZsw & 0x01 ) m_Arith.l_neg( WORK0 );
//		m_Arith.l_print( dp, WORK0, FMT15 );
//		PF1[i].PITM[j++] = dp;	
//		dp += 16;
//		m_Arith.l_add( WORK12B , WORK12B , WORK0 );
//		m_Arith.l_add( WORK11D , WORK11D , WORK0 );
//		// áJ-C
//		m_Arith.l_add( WORK0 , m_pSnTanaData->Sn_mnz63[0] , m_pSnTanaData->Sn_mnz63[1] );
//		m_Arith.l_add( WORK0 , WORK0 , m_pSnTanaData->Sn_mnz63[2] );
//		if( m_pSnTanaData->Sn_MENZsw & 0x01 ) m_Arith.l_neg( WORK0 );
//		m_Arith.l_print( dp, WORK0, FMT15 );
//		PF1[i].PITM[j++] = dp;	
//		dp += 16;
//		m_Arith.l_add( WORK12C , WORK12C , WORK0);
//		m_Arith.l_add( WORK11D , WORK11D , WORK0 );
//		// áJ-D		// áK-C
//		m_Arith.l_print( dp, WORK11D, FMT15 );
//		PF1[i++].PITM[j++] = dp;	dp += 16;
//		m_Arith.l_add( WORK12D , WORK12D , WORK11D);
//	}
//	else{
//	// áJ
//		memmove( WORK0 , m_pSnTanaData->Sn_MENZZ , MONY_BUF_SIZE );
//		if( m_pSnTanaData->Sn_MENZsw & 0x01 ) m_Arith.l_neg( WORK0 );
//		m_Arith.l_print( dp, WORK0 , FMT15 );
//
//		PF1[i].PITM[j++] = dp;	
//		dp += 16;
//		m_Arith.l_add( WORK12A , WORK12A , WORK0 );
//		m_Arith.l_add( WORK11D , WORK11D , WORK0 );
//		
//		memmove( WORK0 , m_pSnTanaData->Sn_MNZ40 , MONY_BUF_SIZE );
//		if( m_pSnTanaData->Sn_MENZsw & 0x01 ) m_Arith.l_neg( WORK0 );
//		m_Arith.l_print( dp, WORK0, FMT15 );
//		PF1[i].PITM[j++] = dp;	
//		dp += 16;
//		m_Arith.l_add( WORK12B , WORK12B , WORK0 );
//		m_Arith.l_add( WORK11D , WORK11D , WORK0 );
//
//		memmove( WORK0 , m_pSnTanaData->Sn_MNZ63 , MONY_BUF_SIZE );
//		if( m_pSnTanaData->Sn_MENZsw & 0x01 ) m_Arith.l_neg( WORK0 );
//		m_Arith.l_print( dp, WORK0, FMT15 );
//		PF1[i].PITM[j++] = dp;	
//		dp += 16;
//		m_Arith.l_add( WORK12C , WORK12C , WORK0 );
//		m_Arith.l_add( WORK11D , WORK11D , WORK0 );
//
//		m_Arith.l_print( dp, WORK11D, FMT15 );
//		PF1[i++].PITM[j++] = dp;	
//		dp += 16;
//		m_Arith.l_add( WORK12D , WORK12D , WORK11D );
//	}
/*-------------*/
	if( m_pSnTanaData->Sn_MENZsw&0x10 ){
		if( m_pSnHeadData->IsKobetuSiireAnbun() ){
			// áJ-A		// áK-A
			m_Arith.l_add( WORK0 , m_pSnTanaData->Sn_menzz[0] , m_pSnTanaData->Sn_menzz[1] );
			m_Arith.l_add( WORK0 , WORK0 , m_pSnTanaData->Sn_menzz[2] );
			if( m_pSnTanaData->Sn_MENZsw & 0x01 ) m_Arith.l_neg( WORK0 );
			m_Arith.l_print( dp, WORK0, FMT15 );
			PF1[i].PITM[j++] = dp;	
			dp += 16;
			m_Arith.l_add( WORK12A , WORK12A , WORK0 );
			m_Arith.l_add( WORK11D , WORK11D , WORK0 );

			// áJ-BÅ@	// áK-B
			m_Arith.l_add( WORK0 , m_pSnTanaData->Sn_mnz40[0] , m_pSnTanaData->Sn_mnz40[1] );
			m_Arith.l_add( WORK0 , WORK0 , m_pSnTanaData->Sn_mnz40[2] );
			if( m_pSnTanaData->Sn_MENZsw & 0x01 ) m_Arith.l_neg( WORK0 );
			m_Arith.l_print( dp, WORK0, FMT15 );
			PF1[i].PITM[j++] = dp;	
			dp += 16;
			m_Arith.l_add( WORK12B , WORK12B , WORK0 );
			m_Arith.l_add( WORK11D , WORK11D , WORK0 );
			// áJ-C
			m_Arith.l_add( WORK0 , m_pSnTanaData->Sn_mnz63[0] , m_pSnTanaData->Sn_mnz63[1] );
			m_Arith.l_add( WORK0 , WORK0 , m_pSnTanaData->Sn_mnz63[2] );
			if( m_pSnTanaData->Sn_MENZsw & 0x01 ) m_Arith.l_neg( WORK0 );
			m_Arith.l_print( dp, WORK0, FMT15 );
			PF1[i].PITM[j++] = dp;	
			dp += 16;
			m_Arith.l_add( WORK12C , WORK12C , WORK0);
			m_Arith.l_add( WORK11D , WORK11D , WORK0 );
			// áJ-D		// áK-C
			m_Arith.l_print( dp, WORK11D, FMT15 );
			PF1[i++].PITM[j++] = dp;	dp += 16;
			m_Arith.l_add( WORK12D , WORK12D , WORK11D);
		}
		else{
		// áJ
			memmove( WORK0 , m_pSnTanaData->Sn_MENZZ , MONY_BUF_SIZE );
			if( m_pSnTanaData->Sn_MENZsw & 0x01 ) m_Arith.l_neg( WORK0 );
			m_Arith.l_print( dp, WORK0 , FMT15 );

			PF1[i].PITM[j++] = dp;	
			dp += 16;
			m_Arith.l_add( WORK12A , WORK12A , WORK0 );
			m_Arith.l_add( WORK11D , WORK11D , WORK0 );
			
			memmove( WORK0 , m_pSnTanaData->Sn_MNZ40 , MONY_BUF_SIZE );
			if( m_pSnTanaData->Sn_MENZsw & 0x01 ) m_Arith.l_neg( WORK0 );
			m_Arith.l_print( dp, WORK0, FMT15 );
			PF1[i].PITM[j++] = dp;	
			dp += 16;
			m_Arith.l_add( WORK12B , WORK12B , WORK0 );
			m_Arith.l_add( WORK11D , WORK11D , WORK0 );

			memmove( WORK0 , m_pSnTanaData->Sn_MNZ63 , MONY_BUF_SIZE );
			if( m_pSnTanaData->Sn_MENZsw & 0x01 ) m_Arith.l_neg( WORK0 );
			m_Arith.l_print( dp, WORK0, FMT15 );
			PF1[i].PITM[j++] = dp;	
			dp += 16;
			m_Arith.l_add( WORK12C , WORK12C , WORK0 );
			m_Arith.l_add( WORK11D , WORK11D , WORK0 );

			m_Arith.l_print( dp, WORK11D, FMT15 );
			PF1[i++].PITM[j++] = dp;	
			dp += 16;
			m_Arith.l_add( WORK12D , WORK12D , WORK11D );
		}
	}
	else{
//-- '14.09.08 --
//		i++
//---------------
		PF1[i].PITM[j++] = dp;	
		dp += 16;
		PF1[i].PITM[j++] = dp;	
		dp += 16;
		PF1[i].PITM[j++] = dp;	
		dp += 16;
		PF1[i++].PITM[j++] = dp;	
		dp += 16;
//---------------
	}
/*-------------*/

	j = 0;
	// ã‡äzáI	// áJ
	// áI-AÅ@	// áJ-A
	m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F10A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	m_Arith.l_add( WORK12A , WORK12A , m_pSnFhyo22Data->Sn_2F10A );
	// áI-BÅ@	// áJ-B
	m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F10B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	m_Arith.l_add( WORK12B , WORK12B , m_pSnFhyo22Data->Sn_2F10B );
	// áI-C
	m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F10C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	m_Arith.l_add( WORK12C , WORK12C , m_pSnFhyo22Data->Sn_2F10C );
	// áI-C		// áJ-C
	m_Arith.l_add( WORK0, m_pSnFhyo22Data->Sn_2F10A, m_pSnFhyo22Data->Sn_2F10B );
	m_Arith.l_add( WORK0, WORK0, m_pSnFhyo22Data->Sn_2F10C );
	m_Arith.l_print( dp, WORK0, FMT15 );
	PF1[i++].PITM[j] = dp;
	dp += 16;
	m_Arith.l_add( WORK12D , WORK12D , WORK0 );

	// ã‡äzáKÅ®áM	// áL->áN
	pbclr( 0, 4*3, &PF1[i] );					
	j = 0;
	// áK-AÅ@		// áL-A
	m_Arith.l_print( dp, WORK12A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	// áK-B			// áL-B
	m_Arith.l_print( dp, WORK12B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	// áK-C			// áL-B
	m_Arith.l_print( dp, WORK12C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	// áK-D			// áL-C
	m_Arith.l_print( dp, WORK12D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	int Srx = 0;
	int Anbun = 0;
	int Flg12 = 0;
/*- '14.06.16 -*/
//	if( m_pSnHeadData->IsRatioOver95() == TRUE ) Srx = 1;//95%à»è„
/*-------------*/
	if( (m_pSnHeadData->IsRatioOver95()==TRUE) || (m_pSnHeadData->Sn_EXP100&0x01) ){
		Srx = 1;//95%à»è„
	}
/*-------------*/
	if( *m_pShinInfo->pOver500MillionSw  ) Anbun = 1;

	if( m_pSnHeadData->IsUseSemiUriRatio() ){
	}
	else{
//--- '15.04.18 ---
//		if(Srx){
//-----------------
		if( !(m_pSnHeadData->Sn_Sign4&0x10) && Srx ){
//-----------------
			// áL-A		// áM-A
			m_Arith.l_print( dp, WORK12A, FMT15 );
			PF1[i].PITM[j++] = dp;
			dp += 16;
			m_Arith.l_add( WORK20A , WORK20A , WORK12A );
			// áL-B		// áM-B
			m_Arith.l_print( dp, WORK12B, FMT15 );
			PF1[i].PITM[j++] = dp;
			dp += 16;
			m_Arith.l_add( WORK20B , WORK20B , WORK12B );
			// áL-C		// áM-C
			m_Arith.l_print( dp, WORK12C, FMT15 );
			PF1[i].PITM[j++] = dp;
			dp += 16;
			m_Arith.l_add( WORK20C , WORK20C , WORK12C );
			// áL-D		// áM-C
			m_Arith.l_print( dp, WORK12D, FMT15 );
			PF1[i].PITM[j++] = dp;
			dp += 16;
			Flg12 = 1;
		}
	}
	if( m_pSnHeadData->IsUseSemiUriRatio() ){
		if( m_pSnHeadData->IsKobetuSiireAnbun() ){
			j += 4;
			// áM-A		// áN-A
			m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F15A, FMT15 );
			PF1[i].PITM[j++] = dp;
			dp += 16;
			// áM-B		// áN-B
			m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F15B, FMT15 );
			PF1[i].PITM[j++] = dp;
			dp += 16;
			// áM-C		// áN-B
			m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F15C, FMT15 );
			PF1[i].PITM[j++] = dp;
			dp += 16;
			// áM-C		// áN-C
			m_Arith.l_add( WORK0, m_pSnFhyo22Data->Sn_2F15A, m_pSnFhyo22Data->Sn_2F15B );
			m_Arith.l_add( WORK0, WORK0, m_pSnFhyo22Data->Sn_2F15C );
			m_Arith.l_print( dp, WORK0, FMT15 );
			PF1[i++].PITM[j] = dp;
			dp += 16;
		}
		else{
			j = j + 4;
		}
	}
	else{
		if( Srx && Anbun ){
			j += 4;
			// áM-A		// áN-A
			m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F15A, FMT15 );
			PF1[i].PITM[j++] = dp;
			dp += 16;
			// áM-B		// áN-B
			m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F15B, FMT15 );
			PF1[i].PITM[j++] = dp;
			dp += 16;
			// áM-C		// áN-B
			m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F15C, FMT15 );
			PF1[i].PITM[j++] = dp;
			dp += 16;
			// áM-C		// áN-C
			m_Arith.l_add( WORK0, m_pSnFhyo22Data->Sn_2F15A, m_pSnFhyo22Data->Sn_2F15B );
			m_Arith.l_add( WORK0, WORK0, m_pSnFhyo22Data->Sn_2F15C );
			m_Arith.l_print( dp, WORK0, FMT15 );
			PF1[i++].PITM[j] = dp;
			dp += 16;
		}
		else{
			if( m_pSnHeadData->IsKobetuSiireAnbun() ){
				j += 4;
				// áM-A		// áN-A
				m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F15A, FMT15 );
				PF1[i].PITM[j++] = dp;
				dp += 16;
				m_Arith.l_add( WORK16A , WORK16A ,m_pSnFhyo22Data->Sn_2F15A);
				// áM-B		// áN-B
				m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F15B, FMT15 );
				PF1[i].PITM[j++] = dp;
				dp += 16;
				m_Arith.l_add( WORK16B , WORK16B ,m_pSnFhyo22Data->Sn_2F15B);

				// áM-C		// áN-B
				m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F15C, FMT15 );
				PF1[i].PITM[j++] = dp;
				dp += 16;
				m_Arith.l_add( WORK16C , WORK16C ,m_pSnFhyo22Data->Sn_2F15C);

				// áM-D		// áN-C
				m_Arith.l_add( WORK0, m_pSnFhyo22Data->Sn_2F15A, m_pSnFhyo22Data->Sn_2F15B );
				m_Arith.l_add( WORK0, WORK0, m_pSnFhyo22Data->Sn_2F15C );
				m_Arith.l_print( dp, WORK0, FMT15 );
				PF1[i++].PITM[j] = dp;
				dp += 16;
			}
			else{
				j = j + 4;
			}
		}
	}
	if( FontFlg ){
		if( j = _xprn( PP, 5, PF1, 0, pDC, 0, 1 ) ){
			goto POUT22;
		}
	}
	else{
		if( j = _xprn( PP, 4, PF1, 0, pDC, 0, 1 ) ){
			goto POUT22;
		}
	}

	//îÑè„äÑçáÇÃåvéZ
	char wari[MONY_BUF_SIZE] = {0};
	char PW6[MONY_BUF_SIZE] = {0};
	char buf[10] = {0};
	char WORK17ALL[10] = {0};
	dp = AP;
	i = j = 0;
	memset( dp, '\0', size );
	pbclr( 0, 4*8, &PF1[i] );

//--> '15.02.06 INS START
	char	PER100BUF[MONY_BUF_SIZE] = {0};
	m_Arith.l_input( PER100BUF, _T("10000000") );
//<-- '15.02.06 INS END

	if( m_pSnHeadData->Sn_UPERS&0x10 ){
		sprintf_s( buf, sizeof( buf ), _T("%d"), m_pSnFhyo22Data->Sn_2FUWSEX );
		m_Arith.l_input( wari, buf );
	}
	else{
		m_Arith.l_par100( (unsigned char *)wari, (unsigned char *)WORK4, (unsigned char *)WORK7,  5, 0 );
		// îÑè„äÑçáÇ™É}ÉCÉiÉXÇÃèÍçáÇÕÇOÅìÇ≈åvéZÇ∑ÇÈ
		memset( PW6, '\0', sizeof(PW6) );
		if( m_Arith.l_cmp(WORK4, PW6) < 0 ){
			memset( wari, '\0', MONY_BUF_SIZE );
		}
		if( m_Arith.l_cmp(WORK7, PW6) < 0 ){
			memset( wari, '\0', MONY_BUF_SIZE );
		}
	}
	if( !Flg12 ){
		// ã‡äzáNÅ®Åi22ÅjÅ@Å@//áO->(23)
		if( m_pSnHeadData->IsKobetuSiireAnbun() ){
		// áNÅ|AÅ@	// áO-A
			m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F16A, FMT15 );
			PF1[i].PITM[j++] = dp;
			dp += 16;

			// áNÅ|BÅ@	// áO-B
			m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F16B, FMT15 );
			PF1[i].PITM[j++] = dp;
			dp += 16;

			// áNÅ|CÅ@	// áO-B
			m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F16C, FMT15 );
			PF1[i].PITM[j++] = dp;
			dp += 16;


			// áNÅ|DÅ@	// áO-C
			m_Arith.l_add( WORK0, m_pSnFhyo22Data->Sn_2F16A, m_pSnFhyo22Data->Sn_2F16B );
			m_Arith.l_add( WORK0, WORK0, m_pSnFhyo22Data->Sn_2F16C );
			m_Arith.l_print( dp, WORK0, FMT15 );
			PF1[i].PITM[j++] = dp;
			dp += 16;

			// áOoráP-A	// áP or áQ -A
			if( m_Arith.l_test(m_pSnFhyo22Data->Sn_2F17A)){
				m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F17A, FMT15 );
				m_Arith.l_add( WORK20A , WORK20A , m_pSnFhyo22Data->Sn_2F17A );
				m_Arith.l_add( WORK16D , WORK16D , m_pSnFhyo22Data->Sn_2F17A );
			}
			else{
				if( m_pSnHeadData->Sn_UPERS&0x10 ){
					m_Arith.l_mul100( (unsigned char *)WORK16A, (unsigned char *) m_pSnFhyo22Data->Sn_2F16A, (unsigned char *)wari, 2, 0 );
				}
				else{
					if( m_Arith.l_test( wari ) == 0 ){
						m_Arith.l_mul100( (unsigned char *)WORK16A, (unsigned char *)m_pSnFhyo22Data->Sn_2F16A, (unsigned char *)wari, 5, 0 );
					}
					else{
//-- '15.02.10 --
//						m_Util.l_pardotnet_syz( WORK16A, (char *)m_pSnFhyo22Data->Sn_2F16A, WORK4, WORK7 );
//---------------
						if( m_Arith.l_cmp(wari, PER100BUF) <= 0 ){
							m_Util.l_pardotnet_syz( WORK16A, (char *)m_pSnFhyo22Data->Sn_2F16A, WORK4, WORK7 );
						}
						else{
							m_Util.l_pardotnet_syz( WORK16A, (char *)m_pSnFhyo22Data->Sn_2F16A, WORK4, WORK4 );
						}
//---------------
					}
				}
				m_Arith.l_add( WORK16A , WORK16A , m_pSnFhyo22Data->Sn_2F15A );
				m_Arith.l_print(dp, WORK16A, FMT15 );
				m_Arith.l_add( WORK20A , WORK20A , WORK16A );
				m_Arith.l_add( WORK16D , WORK16D , WORK16A );
			}
			PF1[i].PITM[j++] = dp;
			dp += 16;

			// áOoráP-BÅ@	// áP or áQ -B
			if( m_Arith.l_test(m_pSnFhyo22Data->Sn_2F17B)){
				m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F17B, FMT15 );
				m_Arith.l_add( WORK20B , WORK20B , m_pSnFhyo22Data->Sn_2F17B );
				m_Arith.l_add( WORK16D , WORK16D , m_pSnFhyo22Data->Sn_2F17B );
			}
			else{
				if( m_pSnHeadData->Sn_UPERS&0x10 ){
					m_Arith.l_mul100( (unsigned char *)WORK16B, (unsigned char *) m_pSnFhyo22Data->Sn_2F16B, (unsigned char *)wari, 2, 0 );
				}
				else{
					if( m_Arith.l_test( wari ) == 0 ){
						m_Arith.l_mul100( (unsigned char *)WORK16B, (unsigned char *)m_pSnFhyo22Data->Sn_2F16B, (unsigned char *)wari, 5, 0 );
					}
					else{
//-- '15.02.10 --
//						m_Util.l_pardotnet_syz( WORK16B, (char *)m_pSnFhyo22Data->Sn_2F16B, WORK4, WORK7 );
//---------------
						if( m_Arith.l_cmp(wari, PER100BUF) <= 0 ){
							m_Util.l_pardotnet_syz( WORK16B, (char *)m_pSnFhyo22Data->Sn_2F16B, WORK4, WORK7 );
						}
						else{
							m_Util.l_pardotnet_syz( WORK16B, (char *)m_pSnFhyo22Data->Sn_2F16B, WORK4, WORK4 );
						}
//---------------
					}
				}
				m_Arith.l_add( WORK16B , WORK16B , m_pSnFhyo22Data->Sn_2F15B );
				m_Arith.l_print(dp, WORK16B, FMT15 );
				m_Arith.l_add( WORK20B , WORK20B , WORK16B );
				m_Arith.l_add( WORK16D , WORK16D , WORK16B );
			}
			PF1[i].PITM[j++] = dp;
			dp += 16;
			// áOoráP-CÅ@	// áP or áQ -B
			if( m_Arith.l_test(m_pSnFhyo22Data->Sn_2F17C)){
				m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F17C, FMT15 );
				m_Arith.l_add( WORK20C , WORK20C , m_pSnFhyo22Data->Sn_2F17C );
				m_Arith.l_add( WORK16D , WORK16D , m_pSnFhyo22Data->Sn_2F17C );
			}
			else{
				if( m_pSnHeadData->Sn_UPERS&0x10 ){
					m_Arith.l_mul100( (unsigned char *)WORK16C, (unsigned char *) m_pSnFhyo22Data->Sn_2F16C, (unsigned char *)wari, 2, 0 );
				}
				else{
					if( m_Arith.l_test( wari ) == 0 ){
						m_Arith.l_mul100( (unsigned char *)WORK16C, (unsigned char *)m_pSnFhyo22Data->Sn_2F16C, (unsigned char *)wari, 5, 0 );
					}
					else{
//-- '15.02.10 --
//						m_Util.l_pardotnet_syz( WORK16C, (char *)m_pSnFhyo22Data->Sn_2F16C, WORK4, WORK7 );
//---------------
						if( m_Arith.l_cmp(wari, PER100BUF) <= 0 ){
							m_Util.l_pardotnet_syz( WORK16C, (char *)m_pSnFhyo22Data->Sn_2F16C, WORK4, WORK7 );
						}
						else{
							m_Util.l_pardotnet_syz( WORK16C, (char *)m_pSnFhyo22Data->Sn_2F16C, WORK4, WORK4 );
						}
//---------------
					}
				}
				m_Arith.l_add( WORK16C , WORK16C , m_pSnFhyo22Data->Sn_2F15C );
				m_Arith.l_print(dp, WORK16C, FMT15 );
				m_Arith.l_add( WORK20C , WORK20C , WORK16C );
				m_Arith.l_add( WORK16D , WORK16D , WORK16C );
			}
			PF1[i].PITM[j++] = dp;
			dp += 16;
			// áOoráP-DÅ@	// áP or áQ -C
			m_Arith.l_print( dp, WORK16D, FMT15 );
			PF1[i].PITM[j++] = dp;	dp += 16;
			j += 4;
		}
		else{
			j += 8;
			// áOoráP-A		// áP or áQ -A
			if( m_Arith.l_test( m_pSnFhyo22Data->Sn_2F18A ) ){
				m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F18A, FMT15 );
				m_Arith.l_add( WORK17ALL , WORK17ALL , m_pSnFhyo22Data->Sn_2F18A );
				m_Arith.l_add( WORK20A , WORK20A , m_pSnFhyo22Data->Sn_2F18A );
			}
			else{
				if( m_pSnHeadData->Sn_UPERS&0x10 ){
					m_Arith.l_mul100( (unsigned char *)WORK0, (unsigned char *)WORK12A, (unsigned char *)wari, 2, 0 );
				}
				else{
					if( m_Arith.l_test( wari ) == 0 ){
						m_Arith.l_mul100( (unsigned char *)WORK0, (unsigned char *)WORK12A, (unsigned char *)wari, 5, 0 );
					}
					else{
//-- '15.02.10 --
//						m_Util.l_pardotnet_syz( WORK0, WORK12A , WORK4, WORK7 );
//---------------
						if( m_Arith.l_cmp(wari, PER100BUF) <= 0 ){
							m_Util.l_pardotnet_syz( WORK0, WORK12A, WORK4, WORK7 );
						}
						else{
							m_Util.l_pardotnet_syz( WORK0, WORK12A, WORK4, WORK4 );
						}
//---------------
					}
				}
				m_Arith.l_print( dp, WORK0, FMT15 );
				m_Arith.l_add( WORK17ALL , WORK17ALL , WORK0 );
				m_Arith.l_add( WORK20A , WORK20A , WORK0 );
			}
			PF1[i].PITM[j++] = dp;
			dp += 16;


			// áOoráP-BÅ@	// áP or áQ -B
			if( m_Arith.l_test( m_pSnFhyo22Data->Sn_2F18B ) ){
				m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F18B, FMT15 );
				m_Arith.l_add( WORK17ALL , WORK17ALL , m_pSnFhyo22Data->Sn_2F18B );
				m_Arith.l_add( WORK20B , WORK20B , m_pSnFhyo22Data->Sn_2F18B );
			}
			else{
				if( m_pSnHeadData->Sn_UPERS&0x10 ){
					m_Arith.l_mul100( (unsigned char *)WORK0, (unsigned char *)WORK12B, (unsigned char *)wari, 2, 0 );
				}
				else{
					if( m_Arith.l_test( wari ) == 0 ){
						m_Arith.l_mul100( (unsigned char *)WORK0, (unsigned char *)WORK12B, (unsigned char *)wari, 5, 0 );
					}
					else{
//-- '15.02.10 --
//						m_Util.l_pardotnet_syz( WORK0, WORK12B , WORK4, WORK7 );
//---------------
						if( m_Arith.l_cmp(wari, PER100BUF) <= 0 ){
							m_Util.l_pardotnet_syz( WORK0, WORK12B, WORK4, WORK7 );
						}
						else{
							m_Util.l_pardotnet_syz( WORK0, WORK12B, WORK4, WORK4 );
						}
//---------------
					}
				}
				m_Arith.l_print( dp, WORK0, FMT15 );
				m_Arith.l_add( WORK17ALL , WORK17ALL , WORK0 );
				m_Arith.l_add( WORK20B , WORK20B , WORK0 );
			}
			PF1[i].PITM[j++] = dp;
			dp += 16;
			// áOoráP-CÅ@	// áP or áQ -B
			if( m_Arith.l_test( m_pSnFhyo22Data->Sn_2F18C ) ){
				m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F18C, FMT15 );
				m_Arith.l_add( WORK17ALL , WORK17ALL , m_pSnFhyo22Data->Sn_2F18C );
				m_Arith.l_add( WORK20C, WORK20C, m_pSnFhyo22Data->Sn_2F18C );
			}
			else{
				if( m_pSnHeadData->Sn_UPERS&0x10 ){
					m_Arith.l_mul100( (unsigned char *)WORK0, (unsigned char *)WORK12C, (unsigned char *)wari, 2, 0 );
				}
				else{
					if( m_Arith.l_test( wari ) == 0 ){
						m_Arith.l_mul100( (unsigned char *)WORK0, (unsigned char *)WORK12C, (unsigned char *)wari, 5, 0 );
					}
					else{
//-- '15.02.10 --
//						m_Util.l_pardotnet_syz( WORK0, WORK12C , WORK4, WORK7 );
//---------------
						if( m_Arith.l_cmp(wari, PER100BUF) <= 0 ){
							m_Util.l_pardotnet_syz( WORK0, WORK12C , WORK4, WORK7 );
						}
						else{
							m_Util.l_pardotnet_syz( WORK0, WORK12C , WORK4, WORK4 );
						}
//---------------
					}
				}
				m_Arith.l_print( dp, WORK0, FMT15 );
				m_Arith.l_add( WORK17ALL , WORK17ALL , WORK0 );
				m_Arith.l_add( WORK20C, WORK20C, WORK0 );
			}
			PF1[i].PITM[j++] = dp;
			dp += 16;

			// áOoráP-DÅ@	// áP or áQ -C
			m_Arith.l_print( dp, WORK17ALL, FMT15 );
			PF1[i].PITM[j++] = dp;	dp += 16;
		}
	}
	else{
		j = j + 12;
	}
	// áQ-A		// áR-A
	m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F19A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	m_Arith.l_add( WORK20A , WORK20A , m_pSnFhyo22Data->Sn_2F19A );

	// áQ-BÅ@	// áR-B
	m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F19B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	m_Arith.l_add( WORK20B , WORK20B , m_pSnFhyo22Data->Sn_2F19B );
	// áQ-CÅ@	// áR-B
	m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F19C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	m_Arith.l_add( WORK20C , WORK20C , m_pSnFhyo22Data->Sn_2F19C );
	// áQ-D		// áR-C
	m_Arith.l_add( WORK0, m_pSnFhyo22Data->Sn_2F19A, m_pSnFhyo22Data->Sn_2F19B );
	m_Arith.l_add( WORK0, WORK0, m_pSnFhyo22Data->Sn_2F19C );
	m_Arith.l_print( dp, WORK0, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	// áR-A		// áS-A
	m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F20A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	m_Arith.l_add( WORK20A , WORK20A , m_pSnFhyo22Data->Sn_2F20A );
	// áR-BÅ@	// áS-B
	m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F20B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	m_Arith.l_add( WORK20B , WORK20B , m_pSnFhyo22Data->Sn_2F20B );
	// áR-CÅ@	// áS-B
	m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F20C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	m_Arith.l_add( WORK20C , WORK20C , m_pSnFhyo22Data->Sn_2F20C );
	// áR-DÅ@	// áS-C
	m_Arith.l_add( WORK0, m_pSnFhyo22Data->Sn_2F20A, m_pSnFhyo22Data->Sn_2F20B );
	m_Arith.l_add( WORK0, WORK0, m_pSnFhyo22Data->Sn_2F20C );
	m_Arith.l_print( dp, WORK0, FMT15 );
	PF1[i].PITM[j++] = dp;	
	dp += 16;
	ii = 0x00;

	//Åi20Åj-A		//(21)-A
	if( m_pSnFhyo22Data->Sn_2F21sw & 0x01){//éËì¸óÕÇÃèÍçá
		memset( WORK20A , '\0' , MONY_BUF_SIZE );
		memmove( WORK20A , m_pSnFhyo22Data->Sn_2F21A , MONY_BUF_SIZE );
	}
	if( m_Arith.l_test(WORK20A) > 0 ){
		m_Arith.l_print( dp, WORK20A, FMT15 );
		PF1[i].PITM[j++] = dp;
		dp += 16;
		ii |= 0x01;
		m_Arith.l_add( W0 , W0 , WORK20A );
	}
	else{
		j++;
	}
// Åi20Åj-BÅ@Å@	// (21)-B
	if( m_pSnFhyo22Data->Sn_2F21sw & 0x10){//éËì¸óÕÇÃèÍçá
		memset( WORK20B , '\0' , MONY_BUF_SIZE );
		memmove( WORK20B , m_pSnFhyo22Data->Sn_2F21B , MONY_BUF_SIZE );
	}
	if( m_Arith.l_test(WORK20B) > 0 ){
		m_Arith.l_print( dp, WORK20B, FMT15 );
		PF1[i].PITM[j++] = dp;
		dp += 16;
		ii |= 0x10;
		m_Arith.l_add( W0 , W0 , WORK20B );
	}
	else{
		j++;
	}
	if( m_pSnFhyo22Data->Sn_2F21sw & 0x100){//éËì¸óÕÇÃèÍçá
		memset( WORK20C , '\0' , MONY_BUF_SIZE );
		memmove( WORK20C , m_pSnFhyo22Data->Sn_2F21C , MONY_BUF_SIZE );
	}
	if( m_Arith.l_test(WORK20C) > 0 ){
		m_Arith.l_print( dp, WORK20C, FMT15 );
		PF1[i].PITM[j++] = dp;
		dp += 16;
		ii |= 0x10;
		m_Arith.l_add( W0 , W0 , WORK20C );
	}
	else{
		j++;
	}

	//Åi20Åj-D		// (21)-C

	m_Arith.l_print( dp, W0, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	memset( W0 , '\0' , MONY_BUF_SIZE );
//Åi21Åj-AÅ@	// (22)-A
	if( m_Arith.l_test(WORK20A) < 0 ){
		m_Arith.l_neg(WORK20A);
		m_Arith.l_print( dp, WORK20A, FMT15 );
		PF1[i].PITM[j++] = dp;
		dp += 16;
		ii |= 0x01;
		m_Arith.l_add( W0 , W0 , WORK20A );

	}
	else{
		j++;
	}
//Åi21Åj-B		// (22)-B
	if( m_Arith.l_test(WORK20B) < 0 ){
		m_Arith.l_neg(WORK20B);
		m_Arith.l_print( dp, WORK20B, FMT15 );
		PF1[i].PITM[j++] = dp;
		dp += 16;
		ii |= 0x10;
		m_Arith.l_add( W0 , W0 , WORK20B );
	}
	else{
		j++;
	}
	//Åi21Åj-B		// (22)-B
	if( m_Arith.l_test(WORK20C) < 0 ){
		m_Arith.l_neg(WORK20C);
		m_Arith.l_print( dp, WORK20C, FMT15 );
		PF1[i].PITM[j++] = dp;
		dp += 16;
		ii |= 0x10;
		m_Arith.l_add( W0 , W0 , WORK20C );
	}
	else{
		j++;
	}
	//Åi21Åj-D		// (21)-C

	m_Arith.l_print( dp, W0, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	//Åi22Åj-AÅ@	// (23)-A
	m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F23A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	//Åi22Åj-B		// (23)-B
	m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F23B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	//Åi22Åj-C		// (23)-B
	m_Arith.l_print( dp, m_pSnFhyo22Data->Sn_2F23C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	//Åi22Åj-DÅ@Å@	// (23)-C
	m_Arith.l_add( WORK0, m_pSnFhyo22Data->Sn_2F23A, m_pSnFhyo22Data->Sn_2F23B );
	m_Arith.l_add( WORK0, WORK0, m_pSnFhyo22Data->Sn_2F23C );
	m_Arith.l_print( dp, WORK0, FMT15 );
	PF1[i].PITM[j++] = dp;	
	dp += 16;
	
	if( FontFlg ){
		if( j = _xprn( PP, 7, PF1, 0, pDC, 0, 1 ) ){
			goto POUT22;
		}
	}
	else{
		if( j = _xprn( PP, 6, PF1, 0, pDC, 0, 1 ) ){
			goto POUT22;
		}
	}
	// éÅñº
	dp = AP;
	i = j = 0;
	memset( dp, '\0', size );
	if( kojin ){	// å¬êléñã∆é“ÇÃèÍçáÇÕë„ï\é“éÅñº
		j = sizeof( m_pSnHeadData->Sn_DIHYO );
		memmove( dp, m_pSnHeadData->Sn_DIHYO, j );
	}
	else{			// ñ@êlñº
		j = sizeof( m_pSnHeadData->Sn_CONAM );
		memmove( dp, m_pSnHeadData->Sn_CONAM, j );
	}
	PF1[0].PITM[0] = dp;
	if( j = _xprn( PP, 8, PF1, 2, pDC, 0, 1 ) ){
		goto POUT22;
	}

POUT22:
	if( AP ){
		delete [] AP;
	}
	
	return j;
}

//2015.08.04 INSERT START
//-----------------------------------------------------------------------------
// ïtï\ÇQÅ|ÇQèoóÕ ÉäÉoÅ[ÉXÉ`ÉÉÅ[ÉW
//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CH26HyoView::PrintFuhyo22Ex( CDC *pDC, int kojin, int pcnt )
{
	int				i, j;
	unsigned int	size;
	char			*AP, *dp, ii;
	CString			hoge;

	unsigned short	SEno = 1;
	if( m_pPrintCmInfo->PRtype&0x80 ){
	}
	
	BOOL	isNoImage = FALSE;
	if( m_pPrintCmInfo->PRtype&0x80 ){
		isNoImage = 0;	// ÉCÉÅÅ[ÉWèoóÕóLÇË
	}
	else{
		isNoImage = 1;	// ÉCÉÅÅ[ÉWèoóÕñ≥Çµ
	}

	size = 512;
	if( (AP = new char[size]) == NULL ){
		return -1;
	}
	
	ASSERT( m_pSnFhyo22Data );
	if( m_pSnFhyo22Data == NULL ){
		return 0;
	}

PBUF	PF1[30];
int		PP;

	CH28SnFhyo20Data*	PrintData;
	PrintData = (CH28SnFhyo20Data*)m_pSnFhyo22Data;

	PP = 7;

	dp = AP;
	memset( dp, '\0', size );
	i = 0;

int	CCSw = 0;
char	WORK0[MONY_BUF_SIZE] = {0};
char	W0[MONY_BUF_SIZE] = {0};
char	WORK4[MONY_BUF_SIZE] = {0};
char	WORK7[MONY_BUF_SIZE] = {0};
char	WORK16A[MONY_BUF_SIZE] = {0};
char	WORK16B[MONY_BUF_SIZE] = {0};
char	WORK16C[MONY_BUF_SIZE] = {0};
char	WORK16D[MONY_BUF_SIZE] = {0};
char	WORK11D[MONY_BUF_SIZE] = {0};
char	WORK12A[MONY_BUF_SIZE] = {0};
char	WORK12B[MONY_BUF_SIZE] = {0};
char	WORK12C[MONY_BUF_SIZE] = {0};
char	WORK12D[MONY_BUF_SIZE] = {0};

char	WORK20A[MONY_BUF_SIZE] = {0};
char	WORK20B[MONY_BUF_SIZE] = {0};
char	WORK20C[MONY_BUF_SIZE] = {0};

int	FontFlg = 0;//0:12åÖà»â∫ÇÃèÍçáÅAëÂÇ´Ç≠ï\é¶ 1:13åÖà»è„ÇÃèÍçáÅAè¨Ç≥Ç≠Ç∑ÇÈ
CString str = _T("");

//2016.10.20 INSERT START
char	YEN_ZERO[MONY_BUF_SIZE] = {0}; //0â~ï\ãLóp
//2016.10.20 INSERT END

	// ã≠êßâ€ê≈ä˙ä‘àÛç¸
	if( CCSw ){	// ã≠êßä˙ä‘ïœçX
	}
	else{
		char	yymmdd[3] = {0};
		if( IsJcContinuePrint(true) ){
			m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		else{
			m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		wsprintf( dp, _T("%02X %02X %02X"), yymmdd[0]&0xff, yymmdd[1]&0xff, yymmdd[2]&0xff );
		PF1[i].PITM[0] = dp;
		dp += 9;
		memset( yymmdd, '\0', sizeof(yymmdd) );
		if( IsJcContinuePrint(true) ){
			m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		else{
			m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		wsprintf( dp, _T("%02X %02X %02X"), yymmdd[0]&0xff, yymmdd[1]&0xff, yymmdd[2]&0xff );
		PF1[i++].PITM[1] = dp;
		dp += 9;
	}

	memmove( dp, _T("Å@"), 2 );
	PF1[i++].PITM[0] = dp;	
	dp += (2 + 1);
	
	if( j = _xprn( PP, 1, PF1, 0, pDC, isNoImage, SEno ) ){
		goto POUT22;
	}
	i = 0;
	j = 0;										// ã‡äzá@->áB
	// á@-A
	m_Arith.l_print( dp, PrintData->Sn_2F1A, FMT16_EX );
	
	PF1[i].PITM[j++] = dp;
	dp += 19;
	// á@-B
	m_Arith.l_print( dp, PrintData->Sn_2F1B, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	// á@-C
	m_Arith.l_print( dp, PrintData->Sn_2F1C, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;
	// á@-D
	m_Arith.l_add( WORK0, PrintData->Sn_2F1A, PrintData->Sn_2F1B );
	m_Arith.l_add( WORK0, WORK0, PrintData->Sn_2F1C );
	m_Arith.l_print( dp, WORK0, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	// áA-A
	PF1[i].PITM[j++] = (char *)0;
	// áA-B
	PF1[i].PITM[j++] = (char *)0;
	// áA-C
	PF1[i].PITM[j++] = (char *)0;
	// áA-D
	m_Arith.l_print( dp, PrintData->Sn_2F2C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	m_Arith.l_add( WORK0, WORK0, PrintData->Sn_2F2C );//ë´ÇµÇ±Ç›
	
	// áB-A
	PF1[i].PITM[j++] = (char *)0;
	// áB-B
	PF1[i].PITM[j++] = (char *)0;
	// áB-C
	PF1[i].PITM[j++] = (char *)0;
	// áB-D
	m_Arith.l_print( dp, PrintData->Sn_2F3C, FMT15 );
	PF1[i++].PITM[j] = dp;
	dp += 16;
	m_Arith.l_add( WORK0, WORK0, PrintData->Sn_2F3C );//ë´ÇµÇ±Ç›

	// ã‡äzáC•áD•áF
	j = 0;
	// áC
//2016.06.22 UPDATE START
//	m_Arith.l_print( dp, WORK0, FMT16_EX );
	if( m_Arith.l_test( WORK0 ) >= 0 ){
		m_Arith.l_print( dp, WORK0, FMT16_EX );
	}else{
		m_Arith.l_print( dp, YEN_ZERO, FMT16_EXZ );
	}
//2016.06.22 UPDATE END
	PF1[i].PITM[j++] = dp;
//2016.06.22 INSERT START
	dp += 19;
	m_Arith.l_print( dp, WORK0, FMT16_EX );
//2016.06.22 INSERT END
	m_Arith.l_add( WORK4, WORK4, WORK0 );//ë´ÇµÇ±Ç›
	// áD
	PF1[i].PITM[j++] = dp;
	dp += 19;
	// áE
	m_Arith.l_print( dp, PrintData->Sn_2F6C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	// áF
	m_Arith.l_add( WORK0, WORK0, PrintData->Sn_2F6C );//ë´ÇµÇ±Ç›
//2016.06.22 UPDATE START
//	m_Arith.l_print( dp, WORK0, FMT16_EX );
	if( m_Arith.l_test( WORK0 ) >= 0 ){
		m_Arith.l_print( dp, WORK0, FMT16_EX );
	}else{
		m_Arith.l_print( dp, YEN_ZERO, FMT16_EXZ );
	}
//2016.06.22 UPDATE END
	PF1[i++].PITM[j] = dp;
	dp += 19;
	m_Arith.l_add( WORK7, WORK7, WORK0 );//ë´ÇµÇ±Ç›
	
	j = 0;
	// ã‡äzáE•áG
	// áG-A
	m_Arith.l_print( dp, PrintData->Sn_2F8A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	// áG-B
	m_Arith.l_print( dp, PrintData->Sn_2F8B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	// áG-C
	m_Arith.l_print( dp, PrintData->Sn_2F8C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	// áG-D
	m_Arith.l_add( WORK0, PrintData->Sn_2F8A, PrintData->Sn_2F8B );
	m_Arith.l_add( WORK0, WORK0, PrintData->Sn_2F8C );
	m_Arith.l_print( dp, WORK0, FMT15 );
	PF1[i++].PITM[j] = dp;
	dp += 16;
	
	// îÑè„äÑçá
	wsprintf( dp, _T("%3d.%02d"), PrintData->Sn_2FUWS / 100, PrintData->Sn_2FUWS % 100 );
	PF1[i++].PITM[0] = dp;
	dp += 7;

	for( int t = 0; t < 12; t++){
		if( PF1[0].PITM[t] == NULL ) continue;
		str.Format( _T("%s") , PF1[0].PITM[t] );
		str.TrimLeft();
		if( str.GetLength() > 15 ){
			FontFlg = 1;
			break;
		}
	}
	if( !FontFlg ){
		for( int t = 0; t < 4; t++){
			if( PF1[1].PITM[t] == NULL ) continue;
			str.Format( _T("%s") , PF1[1].PITM[t] );
			str.TrimLeft();
			if( str.GetLength() > 15 ){
				FontFlg = 1;
				break;
			}
		}
	}
	if( !FontFlg ){
		for( int t = 0; t < 4; t++){
			if( PF1[2].PITM[t] == NULL ) continue;
			str.Format( _T("%s") , PF1[2].PITM[t] );
			str.TrimLeft();
			if( str.GetLength() > 15 ){
				FontFlg = 1;
				break;
			}
		}
	}
	if( FontFlg ){
		if( j = _xprn( PP, 3, PF1, 0, pDC, isNoImage, SEno ) ){
			goto POUT22;
		}
	}
	else{
		if( j = _xprn( PP, 2, PF1, 0, pDC, isNoImage, SEno ) ){
			goto POUT22;
		}
	}
	dp = AP;
	i = j = 0;
	memset( dp, '\0', size );
	
	// ã‡äzáHÅEáJ		//áH•áK	
	// áH-A
	m_Arith.l_print( dp, PrintData->Sn_2F9A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	m_Arith.l_add( WORK12A , WORK12A , PrintData->Sn_2F9A );
	// áH-B
	m_Arith.l_print( dp, PrintData->Sn_2F9B, FMT15 );
	PF1[i].PITM[j++] = dp;
	m_Arith.l_add( WORK12B , WORK12B , PrintData->Sn_2F9B );
	dp += 16;
	// áH-C
	m_Arith.l_print( dp, PrintData->Sn_2F9C, FMT15 );
	PF1[i].PITM[j++] = dp;	
	m_Arith.l_add( WORK12C , WORK12C , PrintData->Sn_2F9C );
	dp += 16;
	// áH-D
	m_Arith.l_add( WORK0, PrintData->Sn_2F9A, PrintData->Sn_2F9B );
	m_Arith.l_add( WORK0, WORK0, PrintData->Sn_2F9C );
	m_Arith.l_print( dp, WORK0, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	m_Arith.l_add( WORK12D , WORK12D , WORK0 );

	if( m_pSnTanaData->Sn_MENZsw&0x10 ){
		if( m_pSnHeadData->IsKobetuSiireAnbun() ){
			// áL-A		// áM-A
			m_Arith.l_add( WORK0 , m_pSnTanaData->Sn_menzz[0] , m_pSnTanaData->Sn_menzz[1] );
			m_Arith.l_add( WORK0 , WORK0 , m_pSnTanaData->Sn_menzz[2] );
			if( m_pSnTanaData->Sn_MENZsw & 0x01 ) m_Arith.l_neg( WORK0 );
			m_Arith.l_print( dp, WORK0, FMT15 );
			PF1[i].PITM[j++] = dp;	
			dp += 16;
			m_Arith.l_add( WORK12A , WORK12A , WORK0 );
			m_Arith.l_add( WORK11D , WORK11D , WORK0 );

			// áL-BÅ@	// áM-B
			m_Arith.l_add( WORK0 , m_pSnTanaData->Sn_mnz40[0] , m_pSnTanaData->Sn_mnz40[1] );
			m_Arith.l_add( WORK0 , WORK0 , m_pSnTanaData->Sn_mnz40[2] );
			if( m_pSnTanaData->Sn_MENZsw & 0x01 ) m_Arith.l_neg( WORK0 );
			m_Arith.l_print( dp, WORK0, FMT15 );
			PF1[i].PITM[j++] = dp;	
			dp += 16;
			m_Arith.l_add( WORK12B , WORK12B , WORK0 );
			m_Arith.l_add( WORK11D , WORK11D , WORK0 );
			// áL-C
			m_Arith.l_add( WORK0 , m_pSnTanaData->Sn_mnz63[0] , m_pSnTanaData->Sn_mnz63[1] );
			m_Arith.l_add( WORK0 , WORK0 , m_pSnTanaData->Sn_mnz63[2] );
			if( m_pSnTanaData->Sn_MENZsw & 0x01 ) m_Arith.l_neg( WORK0 );
			m_Arith.l_print( dp, WORK0, FMT15 );
			PF1[i].PITM[j++] = dp;	
			dp += 16;
			m_Arith.l_add( WORK12C , WORK12C , WORK0);
			m_Arith.l_add( WORK11D , WORK11D , WORK0 );
			// áL-D		// áM-C
			m_Arith.l_print( dp, WORK11D, FMT15 );
			PF1[i++].PITM[j++] = dp;	dp += 16;
			m_Arith.l_add( WORK12D , WORK12D , WORK11D);
		}
		else{
		// áL
			memmove( WORK0 , m_pSnTanaData->Sn_MENZZ , MONY_BUF_SIZE );
			if( m_pSnTanaData->Sn_MENZsw & 0x01 ) m_Arith.l_neg( WORK0 );
			m_Arith.l_print( dp, WORK0 , FMT15 );

			PF1[i].PITM[j++] = dp;	
			dp += 16;
			m_Arith.l_add( WORK12A , WORK12A , WORK0 );
			m_Arith.l_add( WORK11D , WORK11D , WORK0 );
			
			memmove( WORK0 , m_pSnTanaData->Sn_MNZ40 , MONY_BUF_SIZE );
			if( m_pSnTanaData->Sn_MENZsw & 0x01 ) m_Arith.l_neg( WORK0 );
			m_Arith.l_print( dp, WORK0, FMT15 );
			PF1[i].PITM[j++] = dp;	
			dp += 16;
			m_Arith.l_add( WORK12B , WORK12B , WORK0 );
			m_Arith.l_add( WORK11D , WORK11D , WORK0 );

			memmove( WORK0 , m_pSnTanaData->Sn_MNZ63 , MONY_BUF_SIZE );
			if( m_pSnTanaData->Sn_MENZsw & 0x01 ) m_Arith.l_neg( WORK0 );
			m_Arith.l_print( dp, WORK0, FMT15 );
			PF1[i].PITM[j++] = dp;	
			dp += 16;
			m_Arith.l_add( WORK12C , WORK12C , WORK0 );
			m_Arith.l_add( WORK11D , WORK11D , WORK0 );

			m_Arith.l_print( dp, WORK11D, FMT15 );
			PF1[i++].PITM[j++] = dp;	
			dp += 16;
			m_Arith.l_add( WORK12D , WORK12D , WORK11D );
		}
	}
	else{
		PF1[i].PITM[j++] = dp;	
		dp += 16;
		PF1[i].PITM[j++] = dp;	
		dp += 16;
		PF1[i].PITM[j++] = dp;	
		dp += 16;
		PF1[i++].PITM[j++] = dp;	
		dp += 16;
	}

	j = 0;
	// áI-A
	PF1[i].PITM[j++] = (char *)0;
	// áI-B
	PF1[i].PITM[j++] = (char *)0;
	// áI-C
//-- '15.11.13 --
//	m_Arith.l_print( dp, PrintData->Sn_2F10C, FMT15 );
//---------------
	if( m_pSnHeadData->IsNeedTkkzDisp() ){
		m_Arith.l_print( dp, PrintData->Sn_2F10C, FMT15 );
	}
	else{
		PF1[i].PITM[j++] = (char *)0;
	}
//---------------
	PF1[i].PITM[j++] = dp;
	dp += 16;
	// áI-D
//-- '15.11.13 --
//	m_Arith.l_print( dp, PrintData->Sn_2F10C, FMT15 );
//---------------
	if( m_pSnHeadData->IsNeedTkkzDisp() ){
		m_Arith.l_print( dp, PrintData->Sn_2F10C, FMT15 );
	}
	else{
		PF1[i].PITM[j++] = (char *)0;
	}
//---------------
	PF1[i].PITM[j++] = dp;
	dp += 16;
	// áJ-A
	PF1[i].PITM[j++] = (char *)0;
	// áJ-B
	PF1[i].PITM[j++] = (char *)0;
	// áJ-C
	m_Arith.l_print( dp, PrintData->Sn_2F11C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	m_Arith.l_add( WORK12C , WORK12C , PrintData->Sn_2F11C );
	// áJ-D
	m_Arith.l_print( dp, PrintData->Sn_2F11C, FMT15 );
	PF1[i++].PITM[j++] = dp;
	dp += 16;

	j = 0;
	// ã‡äzáK	// áL
	// áK-AÅ@	// áL-A
	m_Arith.l_print( dp, PrintData->Sn_2F12A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	m_Arith.l_add( WORK12A , WORK12A , PrintData->Sn_2F12A );
	// áK-BÅ@	// áL-B
	m_Arith.l_print( dp, PrintData->Sn_2F12B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	m_Arith.l_add( WORK12B , WORK12B , PrintData->Sn_2F12B );
	// áK-C
	m_Arith.l_print( dp, PrintData->Sn_2F12C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	m_Arith.l_add( WORK12C , WORK12C , PrintData->Sn_2F12C );
	// áK-C		// áL-C
	m_Arith.l_add( WORK0, PrintData->Sn_2F12A, PrintData->Sn_2F12B );
	m_Arith.l_add( WORK0, WORK0, PrintData->Sn_2F12C );
//	m_Arith.l_add( WORK0, WORK0, PrintData->Sn_2F11C ); //ébíËèàíuÅH

	m_Arith.l_print( dp, WORK0, FMT15 );
	PF1[i++].PITM[j] = dp;
	dp += 16;
	m_Arith.l_add( WORK12D , WORK12D , WORK0 );

	// ã‡äzáMÅ®áO	// áN->áP
	pbclr( 0, 4*3, &PF1[i] );					
	j = 0;
	// áM-AÅ@		// áN-A
	m_Arith.l_print( dp, WORK12A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	// áM-B			// áN-B
	m_Arith.l_print( dp, WORK12B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	// áM-C			// áN-B
	m_Arith.l_print( dp, WORK12C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
//--> '15.10.23 INS START
	m_Arith.l_add( WORK12D, WORK12A, WORK12B );
	m_Arith.l_add( WORK12D, WORK12D, WORK12C );
//<-- '15.10.23 INS END
	// áM-D			// áN-C
	m_Arith.l_print( dp, WORK12D, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	int Srx = 0;
	int Anbun = 0;
	int Flg12 = 0;
	if( (m_pSnHeadData->IsRatioOver95()==TRUE) || (m_pSnHeadData->Sn_EXP100&0x01) ){
		Srx = 1;//95%à»è„
	}
	if( *m_pShinInfo->pOver500MillionSw  ) Anbun = 1;

	if( m_pSnHeadData->IsUseSemiUriRatio() ){
	}
	else{
		if( !(m_pSnHeadData->Sn_Sign4&0x10) && Srx ){
			// áL-A		// áM-A
			m_Arith.l_print( dp, WORK12A, FMT15 );
			PF1[i].PITM[j++] = dp;
			dp += 16;
			m_Arith.l_add( WORK20A , WORK20A , WORK12A );
			// áL-B		// áM-B
			m_Arith.l_print( dp, WORK12B, FMT15 );
			PF1[i].PITM[j++] = dp;
			dp += 16;
			m_Arith.l_add( WORK20B , WORK20B , WORK12B );
			// áL-C		// áM-C
			m_Arith.l_print( dp, WORK12C, FMT15 );
			PF1[i].PITM[j++] = dp;
			dp += 16;
			m_Arith.l_add( WORK20C , WORK20C , WORK12C );
			// áL-D		// áM-C
			m_Arith.l_print( dp, WORK12D, FMT15 );
			PF1[i].PITM[j++] = dp;
			dp += 16;
			Flg12 = 1;
		}
	}
	if( m_pSnHeadData->IsUseSemiUriRatio() ){
		if( m_pSnHeadData->IsKobetuSiireAnbun() ){
			j += 4;
			// áO-A		// áP-A
			m_Arith.l_print( dp, PrintData->Sn_2F16A, FMT15 );
			PF1[i].PITM[j++] = dp;
			dp += 16;
			// áO-B		// áP-B
			m_Arith.l_print( dp, PrintData->Sn_2F16B, FMT15 );
			PF1[i].PITM[j++] = dp;
			dp += 16;
			// áO-C		// áP-B
			m_Arith.l_print( dp, PrintData->Sn_2F16C, FMT15 );
			PF1[i].PITM[j++] = dp;
			dp += 16;
			// áO-C		// áP-C
			m_Arith.l_add( WORK0, PrintData->Sn_2F16A, PrintData->Sn_2F16B );
			m_Arith.l_add( WORK0, WORK0, PrintData->Sn_2F16C );
			m_Arith.l_print( dp, WORK0, FMT15 );
			PF1[i++].PITM[j] = dp;
			dp += 16;
		}
		else{
			j = j + 4;
		}
	}
	else{
		if( Srx && Anbun ){
			j += 4;
			// áO-A		// áP-A
			m_Arith.l_print( dp, PrintData->Sn_2F16A, FMT15 );
			PF1[i].PITM[j++] = dp;
			dp += 16;
			// áO-B		// áP-B
			m_Arith.l_print( dp, PrintData->Sn_2F16B, FMT15 );
			PF1[i].PITM[j++] = dp;
			dp += 16;
			// áO-C		// áP-B
			m_Arith.l_print( dp, PrintData->Sn_2F16C, FMT15 );
			PF1[i].PITM[j++] = dp;
			dp += 16;
			// áO-C		// áP-C
			m_Arith.l_add( WORK0, PrintData->Sn_2F16A, PrintData->Sn_2F16B );
			m_Arith.l_add( WORK0, WORK0, PrintData->Sn_2F16C );
			m_Arith.l_print( dp, WORK0, FMT15 );
			PF1[i++].PITM[j] = dp;
			dp += 16;
		}
		else{
			if( m_pSnHeadData->IsKobetuSiireAnbun() ){
				j += 4;
				// áO-A		// áP-A
				m_Arith.l_print( dp, PrintData->Sn_2F16A, FMT15 );
				PF1[i].PITM[j++] = dp;
				dp += 16;
				m_Arith.l_add( WORK16A , WORK16A ,PrintData->Sn_2F15A);
				// áO-B		// áP-B
				m_Arith.l_print( dp, PrintData->Sn_2F16B, FMT15 );
				PF1[i].PITM[j++] = dp;
				dp += 16;
				m_Arith.l_add( WORK16B , WORK16B ,PrintData->Sn_2F15B);

				// áO-C		// áP-B
				m_Arith.l_print( dp, PrintData->Sn_2F16C, FMT15 );
				PF1[i].PITM[j++] = dp;
				dp += 16;
				m_Arith.l_add( WORK16C , WORK16C ,PrintData->Sn_2F16C);

				// áO-D		// áP-C
				m_Arith.l_add( WORK0, PrintData->Sn_2F16A, PrintData->Sn_2F16B );
				m_Arith.l_add( WORK0, WORK0, PrintData->Sn_2F16C );
				m_Arith.l_print( dp, WORK0, FMT15 );
				PF1[i++].PITM[j] = dp;
				dp += 16;
			}
			else{
				j = j + 4;
			}
		}
	}
	if( FontFlg ){
		if( j = _xprn( PP, 5, PF1, 0, pDC, 0, 1 ) ){
			goto POUT22;
		}
	}
	else{
		if( j = _xprn( PP, 4, PF1, 0, pDC, 0, 1 ) ){
			goto POUT22;
		}
	}

	//îÑè„äÑçáÇÃåvéZ
	char wari[MONY_BUF_SIZE] = {0};
	char PW6[MONY_BUF_SIZE] = {0};
	char buf[10] = {0};
	char WORK17ALL[10] = {0};
	dp = AP;
	i = j = 0;
	memset( dp, '\0', size );
	pbclr( 0, 4*8, &PF1[i] );

	char	PER100BUF[MONY_BUF_SIZE] = {0};
	m_Arith.l_input( PER100BUF, _T("10000000") );

	if( m_pSnHeadData->Sn_UPERS&0x10 ){
		sprintf_s( buf, sizeof( buf ), _T("%d"), PrintData->Sn_2FUWSEX );
		m_Arith.l_input( wari, buf );
	}
	else{
		m_Arith.l_par100( (unsigned char *)wari, (unsigned char *)WORK4, (unsigned char *)WORK7,  5, 0 );
		// îÑè„äÑçáÇ™É}ÉCÉiÉXÇÃèÍçáÇÕÇOÅìÇ≈åvéZÇ∑ÇÈ
		memset( PW6, '\0', sizeof(PW6) );
		if( m_Arith.l_cmp(WORK4, PW6) < 0 ){
			memset( wari, '\0', MONY_BUF_SIZE );
		}
		if( m_Arith.l_cmp(WORK7, PW6) < 0 ){
			memset( wari, '\0', MONY_BUF_SIZE );
		}
	}
	if( !Flg12 ){
		// ã‡äzáPÅ®Åi24ÅjÅ@Å@//áQ->(25)
		if( m_pSnHeadData->IsKobetuSiireAnbun() ){
		// áPÅ|AÅ@	// áQ-A
			m_Arith.l_print( dp, PrintData->Sn_2F17A, FMT15 );
			PF1[i].PITM[j++] = dp;
			dp += 16;

			// áPÅ|BÅ@	// áQ-B
			m_Arith.l_print( dp, PrintData->Sn_2F17B, FMT15 );
			PF1[i].PITM[j++] = dp;
			dp += 16;

			// áPÅ|CÅ@	// áQ-B
			m_Arith.l_print( dp, PrintData->Sn_2F17C, FMT15 );
			PF1[i].PITM[j++] = dp;
			dp += 16;


			// áPÅ|DÅ@	// áQ-C
			m_Arith.l_add( WORK0, PrintData->Sn_2F17A, PrintData->Sn_2F17B );
			m_Arith.l_add( WORK0, WORK0, PrintData->Sn_2F17C );
			m_Arith.l_print( dp, WORK0, FMT15 );
			PF1[i].PITM[j++] = dp;
			dp += 16;

			// áQoráR-A	// áR or áS -A
			if( m_Arith.l_test(PrintData->Sn_2F18A)){
				m_Arith.l_print( dp, PrintData->Sn_2F18A, FMT15 );
				m_Arith.l_add( WORK20A , WORK20A , PrintData->Sn_2F18A );
				m_Arith.l_add( WORK16D , WORK16D , PrintData->Sn_2F18A );
			}
			else{
				if( m_pSnHeadData->Sn_UPERS&0x10 ){
					m_Arith.l_mul100( (unsigned char *)WORK16A, (unsigned char *) PrintData->Sn_2F17A, (unsigned char *)wari, 2, 0 );
				}
				else{
					if( m_Arith.l_test( wari ) == 0 ){
						m_Arith.l_mul100( (unsigned char *)WORK16A, (unsigned char *)PrintData->Sn_2F17A, (unsigned char *)wari, 5, 0 );
					}
					else{
						if( m_Arith.l_cmp(wari, PER100BUF) <= 0 ){
							m_Util.l_pardotnet_syz( WORK16A, (char *)PrintData->Sn_2F17A, WORK4, WORK7 );
						}
						else{
							m_Util.l_pardotnet_syz( WORK16A, (char *)PrintData->Sn_2F17A, WORK4, WORK4 );
						}
					}
				}
				m_Arith.l_add( WORK16A , WORK16A , PrintData->Sn_2F16A );
				m_Arith.l_print(dp, WORK16A, FMT15 );
				m_Arith.l_add( WORK20A , WORK20A , WORK16A );
				m_Arith.l_add( WORK16D , WORK16D , WORK16A );
			}
			PF1[i].PITM[j++] = dp;
			dp += 16;

			// áOoráP-BÅ@	// áP or áQ -B
			if( m_Arith.l_test(PrintData->Sn_2F18B)){
				m_Arith.l_print( dp, PrintData->Sn_2F18B, FMT15 );
				m_Arith.l_add( WORK20B , WORK20B , PrintData->Sn_2F18B );
				m_Arith.l_add( WORK16D , WORK16D , PrintData->Sn_2F18B );
			}
			else{
				if( m_pSnHeadData->Sn_UPERS&0x10 ){
					m_Arith.l_mul100( (unsigned char *)WORK16B, (unsigned char *) PrintData->Sn_2F17B, (unsigned char *)wari, 2, 0 );
				}
				else{
					if( m_Arith.l_test( wari ) == 0 ){
						m_Arith.l_mul100( (unsigned char *)WORK16B, (unsigned char *)PrintData->Sn_2F17B, (unsigned char *)wari, 5, 0 );
					}
					else{
						if( m_Arith.l_cmp(wari, PER100BUF) <= 0 ){
							m_Util.l_pardotnet_syz( WORK16B, (char *)PrintData->Sn_2F17B, WORK4, WORK7 );
						}
						else{
							m_Util.l_pardotnet_syz( WORK16B, (char *)PrintData->Sn_2F17B, WORK4, WORK4 );
						}
					}
				}
				m_Arith.l_add( WORK16B , WORK16B , PrintData->Sn_2F16B );
				m_Arith.l_print(dp, WORK16B, FMT15 );
				m_Arith.l_add( WORK20B , WORK20B , WORK16B );
				m_Arith.l_add( WORK16D , WORK16D , WORK16B );
			}
			PF1[i].PITM[j++] = dp;
			dp += 16;
			// áOoráP-CÅ@	// áP or áQ -B
			if( m_Arith.l_test(PrintData->Sn_2F18C)){
				m_Arith.l_print( dp, PrintData->Sn_2F18C, FMT15 );
//-- '16.02.05 --
//				m_Arith.l_add( WORK20C , WORK20C , PrintData->Sn_2F17C );
//				m_Arith.l_add( WORK16D , WORK16D , PrintData->Sn_2F17C );
//---------------
				m_Arith.l_add( WORK20C , WORK20C , PrintData->Sn_2F18C );
				m_Arith.l_add( WORK16D , WORK16D , PrintData->Sn_2F18C );
//---------------
			}
			else{
				if( m_pSnHeadData->Sn_UPERS&0x10 ){
					m_Arith.l_mul100( (unsigned char *)WORK16C, (unsigned char *) PrintData->Sn_2F17C, (unsigned char *)wari, 2, 0 );
				}
				else{
					if( m_Arith.l_test( wari ) == 0 ){
						m_Arith.l_mul100( (unsigned char *)WORK16C, (unsigned char *)PrintData->Sn_2F17C, (unsigned char *)wari, 5, 0 );
					}
					else{
						if( m_Arith.l_cmp(wari, PER100BUF) <= 0 ){
							m_Util.l_pardotnet_syz( WORK16C, (char *)PrintData->Sn_2F17C, WORK4, WORK7 );
						}
						else{
							m_Util.l_pardotnet_syz( WORK16C, (char *)PrintData->Sn_2F17C, WORK4, WORK4 );
						}
					}
				}
				m_Arith.l_add( WORK16C , WORK16C , PrintData->Sn_2F16C );
				m_Arith.l_print(dp, WORK16C, FMT15 );
				m_Arith.l_add( WORK20C , WORK20C , WORK16C );
				m_Arith.l_add( WORK16D , WORK16D , WORK16C );
			}
			PF1[i].PITM[j++] = dp;
			dp += 16;
			// áQoráR-DÅ@	// áR or áS -C
			m_Arith.l_print( dp, WORK16D, FMT15 );
			PF1[i].PITM[j++] = dp;	dp += 16;
			j += 4;
		}
		else{
			j += 8;
			// áQoráR-A		// áR or áS -A
			if( m_Arith.l_test( PrintData->Sn_2F19A ) ){
				m_Arith.l_print( dp, PrintData->Sn_2F19A, FMT15 );
				m_Arith.l_add( WORK17ALL , WORK17ALL , PrintData->Sn_2F19A );
				m_Arith.l_add( WORK20A , WORK20A , PrintData->Sn_2F19A );
			}
			else{
				if( m_pSnHeadData->Sn_UPERS&0x10 ){
					m_Arith.l_mul100( (unsigned char *)WORK0, (unsigned char *)WORK12A, (unsigned char *)wari, 2, 0 );
				}
				else{
					if( m_Arith.l_test( wari ) == 0 ){
						m_Arith.l_mul100( (unsigned char *)WORK0, (unsigned char *)WORK12A, (unsigned char *)wari, 5, 0 );
					}
					else{
						if( m_Arith.l_cmp(wari, PER100BUF) <= 0 ){
							m_Util.l_pardotnet_syz( WORK0, WORK12A, WORK4, WORK7 );
						}
						else{
							m_Util.l_pardotnet_syz( WORK0, WORK12A, WORK4, WORK4 );
						}
					}
				}
				m_Arith.l_print( dp, WORK0, FMT15 );
				m_Arith.l_add( WORK17ALL , WORK17ALL , WORK0 );
				m_Arith.l_add( WORK20A , WORK20A , WORK0 );
			}
			PF1[i].PITM[j++] = dp;
			dp += 16;


			// áOoráP-BÅ@	// áP or áQ -B
//-- '15.12.18 --
//			if( m_Arith.l_test( PrintData->Sn_2F18B ) ){
//				m_Arith.l_print( dp, PrintData->Sn_2F18B, FMT15 );
//				m_Arith.l_add( WORK17ALL , WORK17ALL , PrintData->Sn_2F18B );
//				m_Arith.l_add( WORK20B , WORK20B , PrintData->Sn_2F18B );
//			}
//---------------
			if( m_Arith.l_test( PrintData->Sn_2F19B ) ){
				m_Arith.l_print( dp, PrintData->Sn_2F19B, FMT15 );
				m_Arith.l_add( WORK17ALL , WORK17ALL , PrintData->Sn_2F19B );
				m_Arith.l_add( WORK20B , WORK20B , PrintData->Sn_2F19B );
			}
//---------------
			else{
				if( m_pSnHeadData->Sn_UPERS&0x10 ){
					m_Arith.l_mul100( (unsigned char *)WORK0, (unsigned char *)WORK12B, (unsigned char *)wari, 2, 0 );
				}
				else{
					if( m_Arith.l_test( wari ) == 0 ){
						m_Arith.l_mul100( (unsigned char *)WORK0, (unsigned char *)WORK12B, (unsigned char *)wari, 5, 0 );
					}
					else{
						if( m_Arith.l_cmp(wari, PER100BUF) <= 0 ){
							m_Util.l_pardotnet_syz( WORK0, WORK12B, WORK4, WORK7 );
						}
						else{
							m_Util.l_pardotnet_syz( WORK0, WORK12B, WORK4, WORK4 );
						}
					}
				}
				m_Arith.l_print( dp, WORK0, FMT15 );
				m_Arith.l_add( WORK17ALL , WORK17ALL , WORK0 );
				m_Arith.l_add( WORK20B , WORK20B , WORK0 );
			}
			PF1[i].PITM[j++] = dp;
			dp += 16;
			// áQoráR-CÅ@	// áR or áS -B
			if( m_Arith.l_test( PrintData->Sn_2F19C ) ){
				m_Arith.l_print( dp, PrintData->Sn_2F19C, FMT15 );
				m_Arith.l_add( WORK17ALL , WORK17ALL , PrintData->Sn_2F19C );
				m_Arith.l_add( WORK20C, WORK20C, PrintData->Sn_2F19C );
			}
			else{
				if( m_pSnHeadData->Sn_UPERS&0x10 ){
					m_Arith.l_mul100( (unsigned char *)WORK0, (unsigned char *)WORK12C, (unsigned char *)wari, 2, 0 );
				}
				else{
					if( m_Arith.l_test( wari ) == 0 ){
						m_Arith.l_mul100( (unsigned char *)WORK0, (unsigned char *)WORK12C, (unsigned char *)wari, 5, 0 );
					}
					else{
						if( m_Arith.l_cmp(wari, PER100BUF) <= 0 ){
							m_Util.l_pardotnet_syz( WORK0, WORK12C , WORK4, WORK7 );
						}
						else{
							m_Util.l_pardotnet_syz( WORK0, WORK12C , WORK4, WORK4 );
						}
					}
				}
				m_Arith.l_print( dp, WORK0, FMT15 );
				m_Arith.l_add( WORK17ALL , WORK17ALL , WORK0 );
				m_Arith.l_add( WORK20C, WORK20C, WORK0 );
			}
			PF1[i].PITM[j++] = dp;
			dp += 16;

			// áQoráR-DÅ@	// áR or áS -C
			m_Arith.l_print( dp, WORK17ALL, FMT15 );
			PF1[i].PITM[j++] = dp;	dp += 16;
		}
	}
	else{
		j = j + 12;
	}
	// áS-A		// 21-A
	m_Arith.l_print( dp, PrintData->Sn_2F20A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	m_Arith.l_add( WORK20A , WORK20A , PrintData->Sn_2F20A );

	// áS-BÅ@	// 21-B
	m_Arith.l_print( dp, PrintData->Sn_2F20B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	m_Arith.l_add( WORK20B , WORK20B , PrintData->Sn_2F20B );
	// áS-CÅ@	// 21-B
	m_Arith.l_print( dp, PrintData->Sn_2F20C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	m_Arith.l_add( WORK20C , WORK20C , PrintData->Sn_2F20C );
	// áS-D		// 21-C
	m_Arith.l_add( WORK0, PrintData->Sn_2F20A, PrintData->Sn_2F20B );
	m_Arith.l_add( WORK0, WORK0, PrintData->Sn_2F20C );
	m_Arith.l_print( dp, WORK0, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	// áR-A		// 22-A
	m_Arith.l_print( dp, PrintData->Sn_2F21A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	m_Arith.l_add( WORK20A , WORK20A , PrintData->Sn_2F21A );
	// áR-BÅ@	// 22-B
	m_Arith.l_print( dp, PrintData->Sn_2F21B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	m_Arith.l_add( WORK20B , WORK20B , PrintData->Sn_2F21B );
	// áR-CÅ@	// 22-B
	m_Arith.l_print( dp, PrintData->Sn_2F21C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	m_Arith.l_add( WORK20C , WORK20C , PrintData->Sn_2F21C );
	// áR-DÅ@	// 22-C
	m_Arith.l_add( WORK0, PrintData->Sn_2F21A, PrintData->Sn_2F21B );
	m_Arith.l_add( WORK0, WORK0, PrintData->Sn_2F21C );
	m_Arith.l_print( dp, WORK0, FMT15 );
	PF1[i].PITM[j++] = dp;	
	dp += 16;
	ii = 0x00;

	//Åi20Åj-A		//(21)-A
	if( PrintData->Sn_2F22sw & 0x01){//éËì¸óÕÇÃèÍçá
		memset( WORK20A , '\0' , MONY_BUF_SIZE );
		memmove( WORK20A , PrintData->Sn_2F22A , MONY_BUF_SIZE );
	}
	if( m_Arith.l_test(WORK20A) > 0 ){
		m_Arith.l_print( dp, WORK20A, FMT15 );
		PF1[i].PITM[j++] = dp;
		dp += 16;
		ii |= 0x01;
		m_Arith.l_add( W0 , W0 , WORK20A );
	}
	else{
		j++;
	}

// Åi20Åj-BÅ@Å@	// (21)-B
	if( PrintData->Sn_2F22sw & 0x10){//éËì¸óÕÇÃèÍçá
		memset( WORK20B , '\0' , MONY_BUF_SIZE );
		memmove( WORK20B , PrintData->Sn_2F22B , MONY_BUF_SIZE );
	}
	if( m_Arith.l_test(WORK20B) > 0 ){
		m_Arith.l_print( dp, WORK20B, FMT15 );
		PF1[i].PITM[j++] = dp;
		dp += 16;
		ii |= 0x10;
		m_Arith.l_add( W0 , W0 , WORK20B );
	}
	else{
		j++;
	}

	if( PrintData->Sn_2F22sw & 0x100){//éËì¸óÕÇÃèÍçá
		memset( WORK20C , '\0' , MONY_BUF_SIZE );
		memmove( WORK20C , PrintData->Sn_2F22C , MONY_BUF_SIZE );
	}
	if( m_Arith.l_test(WORK20C) > 0 ){
		m_Arith.l_print( dp, WORK20C, FMT15 );
		PF1[i].PITM[j++] = dp;
		dp += 16;
		ii |= 0x10;
		m_Arith.l_add( W0 , W0 , WORK20C );
	}
	else{
		j++;
	}

	//Åi20Åj-D		// (21)-C

	m_Arith.l_print( dp, W0, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	memset( W0 , '\0' , MONY_BUF_SIZE );
//Åi21Åj-AÅ@	// (22)-A
	if( m_Arith.l_test(WORK20A) < 0 ){
		m_Arith.l_neg(WORK20A);
		m_Arith.l_print( dp, WORK20A, FMT15 );
		PF1[i].PITM[j++] = dp;
		dp += 16;
		ii |= 0x01;
		m_Arith.l_add( W0 , W0 , WORK20A );

	}
	else{
		j++;
	}
//Åi21Åj-B		// (22)-B
	if( m_Arith.l_test(WORK20B) < 0 ){
		m_Arith.l_neg(WORK20B);
		m_Arith.l_print( dp, WORK20B, FMT15 );
		PF1[i].PITM[j++] = dp;
		dp += 16;
		ii |= 0x10;
		m_Arith.l_add( W0 , W0 , WORK20B );
	}
	else{
		j++;
	}
	//Åi21Åj-B		// (22)-B
	if( m_Arith.l_test(WORK20C) < 0 ){
		m_Arith.l_neg(WORK20C);
		m_Arith.l_print( dp, WORK20C, FMT15 );
		PF1[i].PITM[j++] = dp;
		dp += 16;
		ii |= 0x10;
		m_Arith.l_add( W0 , W0 , WORK20C );
	}
	else{
		j++;
	}
	//Åi21Åj-D		// (21)-C

	m_Arith.l_print( dp, W0, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;

	//Åi22Åj-AÅ@	// (23)-A
	m_Arith.l_print( dp, PrintData->Sn_2F24A, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	//Åi22Åj-B		// (23)-B
	m_Arith.l_print( dp, PrintData->Sn_2F24B, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	//Åi22Åj-C		// (23)-B
	m_Arith.l_print( dp, PrintData->Sn_2F24C, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;
	//Åi22Åj-DÅ@Å@	// (23)-C
	m_Arith.l_add( WORK0, PrintData->Sn_2F24A, PrintData->Sn_2F24B );
	m_Arith.l_add( WORK0, WORK0, PrintData->Sn_2F24C );
	m_Arith.l_print( dp, WORK0, FMT15 );
	PF1[i].PITM[j++] = dp;	
	dp += 16;
	
	if( FontFlg ){
		if( j = _xprn( PP, 7, PF1, 0, pDC, 0, 1 ) ){
			goto POUT22;
		}
	}
	else{
		if( j = _xprn( PP, 6, PF1, 0, pDC, 0, 1 ) ){
			goto POUT22;
		}
	}
	// éÅñº
	dp = AP;
	i = j = 0;
	memset( dp, '\0', size );
	if( kojin ){	// å¬êléñã∆é“ÇÃèÍçáÇÕë„ï\é“éÅñº
		j = sizeof( m_pSnHeadData->Sn_DIHYO );
		memmove( dp, m_pSnHeadData->Sn_DIHYO, j );
	}
	else{			// ñ@êlñº
		j = sizeof( m_pSnHeadData->Sn_CONAM );
		memmove( dp, m_pSnHeadData->Sn_CONAM, j );
	}
	PF1[0].PITM[0] = dp;
	if( j = _xprn( PP, 8, PF1, 2, pDC, 0, 1 ) ){
		goto POUT22;
	}

POUT22:
	if( AP ){
		delete [] AP;
	}
	
	return j;
}
//2015.08.04 INSERT END

//-----------------------------------------------------------------------------
// ïtï\ÇTÅ|ÇQèoóÕ
//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CH26HyoView::PrintFuhyo52( CDC *pDC, int kojin, int pcnt )
{
	int j = 0;

	j = PrintFuhyo52_1( pDC , kojin , pcnt );
//	j = PrintFuhyo52_2( pDC , kojin , pcnt );

	return( j );
}

//-----------------------------------------------------------------------------
// ä“ïtïtï\Åiå¬êlÅj
//-----------------------------------------------------------------------------
// à¯êî	pDC	ÅF	
//		pno	ÅF	
//-----------------------------------------------------------------------------
void CH26HyoView::PrintKojinRefund( CDC* pDC,int pno )
{
	CRefundBaseDialog::KojinPrint( pDC, pno );
}

//-----------------------------------------------------------------------------
// ä“ïtïtï\Åiñ@êlÅj
//-----------------------------------------------------------------------------
// à¯êî	pDC	ÅF	
//		pno	ÅF	
//-----------------------------------------------------------------------------
void CH26HyoView::PrintHojinRefund( CDC* pDC,int pno )
{
	CRefundBaseDialog::HojinPrint( pDC, pno );
}
//-----------------------------------------------------------------------------
// ïtï\ÇTÅ|ÇQ_ÇPèoóÕ
//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CH26HyoView::PrintFuhyo52_1( CDC *pDC, int kojin, int pcnt )
{
	int				i, j;
	unsigned int	size;
	char			*AP, *dp;
	
	BOOL	isNoImage = FALSE;
	if( m_pPrintCmInfo->PRtype&0x80 ){
		isNoImage = FALSE;	// ÉCÉÅÅ[ÉWèoóÕóLÇË
	}
	else{
		isNoImage = TRUE;	// ÉCÉÅÅ[ÉWèoóÕñ≥Çµ
	}
	
	size = 512;
	if( (AP = new char[size]) == NULL ){
		return -1;
	}
	
	// èoóÕëOèWåv
//	FuHyo52Calqe();

PBUF	PF1[30];
int		PP;


	PP = 7;

	dp = AP;
	memset( dp, '\0', size );
	i = 0;

int CCSw = 0;
char	W0[MONY_BUF_SIZE] = {0};
char	W1[MONY_BUF_SIZE] = {0};
char	W2[MONY_BUF_SIZE] = {0};
char	W3[MONY_BUF_SIZE] = {0};
char	W4[MONY_BUF_SIZE] = {0};
char	W5[MONY_BUF_SIZE] = {0};
char	W6[MONY_BUF_SIZE] = {0};
char	W7[MONY_BUF_SIZE] = {0};
char	W8[MONY_BUF_SIZE] = {0};
char	W9[MONY_BUF_SIZE] = {0};

int		Flg = 0;

	// ã≠êßâ€ê≈ä˙ä‘àÛç¸
	if( CCSw ){	// ã≠êßä˙ä‘ïœçX
//		wsprintf( dp, "%02X %02X %02X", CC_KDAYS[0]&0xff, CC_KDAYS[1]&0xff, CC_KDAYS[2]&0xff );
//		PF1[i].PITM[0] = dp;
//		dp += 9;
//		wsprintf( dp, "%02X %02X %02X", CC_KDAYE[0]&0xff, CC_KDAYE[1]&0xff, CC_KDAYE[2]&0xff );
//		PF1[i++].PITM[1] = dp;	dp += 9;
	}
	else{
		char	yymmdd[3] = {0};
		if( IsJcContinuePrint(true) ){
			m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		else{
			m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		wsprintf( dp, "%02X %02X %02X", yymmdd[0]&0xff, yymmdd[1]&0xff, yymmdd[2]&0xff );
		PF1[i].PITM[0] = dp;
		dp += 9;
		memset( yymmdd, '\0', sizeof(yymmdd) );
		if( IsJcContinuePrint(true) ){
			m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		else{
			m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		wsprintf( dp, "%02X %02X %02X", yymmdd[0]&0xff, yymmdd[1]&0xff, yymmdd[2]&0xff );
		PF1[i++].PITM[1] = dp;	
		dp += 9;
	}

	memmove( dp, "Å@", 2);
	PF1[i++].PITM[0] = dp;
	dp += (2 + 1);

	j = 0;											
	// ã‡äzá@->áC
	// á@
	m_Arith.l_print( dp, m_pSnFhyo40Data->Sn_1F2A, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// á@-A
	m_Arith.l_add( W1 , W1 , m_pSnFhyo40Data->Sn_1F2A );//áCÇ…ë´ÇµÇ±Ç›

	m_Arith.l_print( dp, m_pSnFhyo40Data->Sn_1F2B, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// á@-B
	m_Arith.l_add( W2 , W2 , m_pSnFhyo40Data->Sn_1F2B );//áCÇ…ë´ÇµÇ±Ç›

	m_Arith.l_print( dp, m_pSnFhyo40Data->Sn_1F2C, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// á@-C
	m_Arith.l_add( W3 , W3 , m_pSnFhyo40Data->Sn_1F2C );//áCÇ…ë´ÇµÇ±Ç›

	m_Arith.l_add( W0 , m_pSnFhyo40Data->Sn_1F2A , m_pSnFhyo40Data->Sn_1F2B );
	m_Arith.l_add( W0 , W0 , m_pSnFhyo40Data->Sn_1F2C );
	m_Arith.l_print( dp, W0, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// á@-D

//-->'15.01.26 INS START
	m_Arith.l_add( W4 , W4 , W0 );
//-->'15.01.26 INS END

	//áA
	m_Arith.l_print( dp, m_pSnFhyo40Data->Sn_1F3A, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// áA-A
	m_Arith.l_add( W1 , W1 , m_pSnFhyo40Data->Sn_1F3A );//áCÇ…ë´ÇµÇ±Ç›

	m_Arith.l_print( dp, m_pSnFhyo40Data->Sn_1F3B, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// áA-B
	m_Arith.l_add( W2 , W2 , m_pSnFhyo40Data->Sn_1F3B );//áCÇ…ë´ÇµÇ±Ç›

	m_Arith.l_print( dp, m_pSnFhyo40Data->Sn_1F3C, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// áA-C
	m_Arith.l_add( W3 , W3 , m_pSnFhyo40Data->Sn_1F3C );//áCÇ…ë´ÇµÇ±Ç›

	m_Arith.l_add( W0 , m_pSnFhyo40Data->Sn_1F3A , m_pSnFhyo40Data->Sn_1F3B );
	m_Arith.l_add( W0 , W0 , m_pSnFhyo40Data->Sn_1F3C );
	m_Arith.l_print( dp, W0, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// áA-D

//-->'15.01.26 INS START
	m_Arith.l_add( W4 , W4 , W0 );
//-->'15.01.26 INS END

	//áB
	m_Arith.l_print( dp, m_pSnFhyo40Data->Sn_1F5A, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// áB-A
	m_Arith.l_sub( W1 , W1 , m_pSnFhyo40Data->Sn_1F5A );//áCÇ…ë´ÇµÇ±Ç›

	m_Arith.l_print( dp, m_pSnFhyo40Data->Sn_1F5B, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// áB-B
	m_Arith.l_sub( W2 , W2 , m_pSnFhyo40Data->Sn_1F5B );//áCÇ…ë´ÇµÇ±Ç›

	m_Arith.l_print( dp, m_pSnFhyo40Data->Sn_1F5C, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// áB-C
	m_Arith.l_sub( W3 , W3 , m_pSnFhyo40Data->Sn_1F5C );//áCÇ…ë´ÇµÇ±Ç›

	m_Arith.l_add( W0 , m_pSnFhyo40Data->Sn_1F5A , m_pSnFhyo40Data->Sn_1F5B );
	m_Arith.l_add( W0 , W0 , m_pSnFhyo40Data->Sn_1F5C );
	m_Arith.l_print( dp, W0, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// áB-D

//-->'15.01.26 INS START
	m_Arith.l_sub( W4 , W4 , W0 );
//-->'15.01.26 INS END

	//áC
//-- '15.01.26 --
//	if( m_Arith.l_test(W1) < 0  ){
//		memset( W1 , '\0', MONY_BUF_SIZE );
//	}
//	if( m_Arith.l_test(W2) < 0  ){
//		memset( W2 , '\0', MONY_BUF_SIZE );
//	}
//	if( m_Arith.l_test(W3) < 0  ){
//		memset( W3 , '\0', MONY_BUF_SIZE );
//	}
//
//	m_Arith.l_print( dp, W1, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// áC-A
//	m_Arith.l_print( dp, W2, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// áC-B
//	m_Arith.l_print( dp, W3, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// áC-C
//
//	m_Arith.l_add( W4 , W1 , W2 );
//	m_Arith.l_add( W4 , W4 , W3 );
//	if( m_Arith.l_test(W4) < 0  ){
//		memset( W4 , '\0', MONY_BUF_SIZE );
//	}
//	m_Arith.l_print( dp, W4, FMT15 );
//	PF1[i++].PITM[j++] = dp;	dp += 16;			// áC-D
//---------------
	// áC-A
	if( m_Arith.l_test(W1) < 0  ){
		memset( W1 , '\0', MONY_BUF_SIZE );
		m_Arith.l_print( dp, W1, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W1, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;

	// áC-B
	if( m_Arith.l_test(W2) < 0  ){
		memset( W2 , '\0', MONY_BUF_SIZE );
		m_Arith.l_print( dp, W2, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W2, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;

	// áC-C
	if( m_Arith.l_test(W3) < 0  ){
		memset( W3 , '\0', MONY_BUF_SIZE );
		m_Arith.l_print( dp, W3, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W3, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;

	// áC-D
	if( m_Arith.l_test(W4) < 0  ){
		memset( W4 , '\0', MONY_BUF_SIZE );
		m_Arith.l_print( dp, W4, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W4, FMT15 );
	}
	PF1[i++].PITM[j++] = dp;	dp += 16;			// áC-D
//---------------

	Flg = 0;
//--> '15.02.10 INS START
	unsigned char	gyoFlg = 0x00;	// D0:ëÊÇPéÌ, D1:ëÊÇQéÌ, D2:ëÊÇRéÌ, D3:ëÊÇSéÌ, D4:ëÊÇTéÌ
//--> '15.02.10 INS END
//-- '15.01.21 --
//	if( m_Arith.l_test( m_pSnHonpyoData->Sn_KURI1 )) Flg++;
//	if( m_Arith.l_test( m_pSnHonpyoData->Sn_KURI2 )) Flg++;
//	if( m_Arith.l_test( m_pSnHonpyoData->Sn_KURI3 )) Flg++;
//	if( m_Arith.l_test( m_pSnHonpyoData->Sn_KURI4 )) Flg++;
//	if( m_Arith.l_test( m_pSnHonpyoData->Sn_KURI5 )) Flg++;
//---------------
	if( (m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI1A)||m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI1B)||m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI1C)) ||
		m_Arith.l_test(m_pSnHonpyoData->Sn_KURI1) ){
		Flg++;
//--> '15.02.10 INS START
		gyoFlg |= 0x01;
//--> '15.02.10 INS END
	}
	if( (m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI2A)||m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI2B)||m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI2C)) ||
		m_Arith.l_test(m_pSnHonpyoData->Sn_KURI2) ){
		Flg++;
//--> '15.02.10 INS START
		gyoFlg |= 0x02;
//--> '15.02.10 INS END
	}
	if( (m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI3A)||m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI3B)||m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI3C)) ||
		m_Arith.l_test(m_pSnHonpyoData->Sn_KURI3) ){
		Flg++;
//--> '15.02.10 INS START
		gyoFlg |= 0x04;
//--> '15.02.10 INS END
	}
	if( (m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI4A)||m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI4B)||m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI4C)) ||
		m_Arith.l_test(m_pSnHonpyoData->Sn_KURI4) ){
		Flg++;
//--> '15.02.10 INS START
		gyoFlg |= 0x08;
//--> '15.02.10 INS END
	}
	if( (m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI5A)||m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI5B)||m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI5C)) ||
		m_Arith.l_test(m_pSnHonpyoData->Sn_KURI5) ){
		Flg++;
//--> '15.02.10 INS START
		gyoFlg |= 0x10;
//--> '15.02.10 INS END
	}
//---------------

	//áD
	j = 0;
	if( Flg == 1 ){
		m_Arith.l_print( dp, m_pSnFhyo40Data->Sn_1F4A , FMT15 );
		PF1[i].PITM[j++] = dp;	dp += 16;			// áC-A
		m_Arith.l_print( dp, m_pSnFhyo40Data->Sn_1F4B, FMT15 );
		PF1[i].PITM[j++] = dp;	dp += 16;			// áC-B
		m_Arith.l_print( dp, m_pSnFhyo40Data->Sn_1F4C, FMT15 );
		PF1[i].PITM[j++] = dp;	dp += 16;			// áC-C
//-- '15.01.28 --
//		m_Arith.l_add( W0 , m_pSnFhyo40Data->Sn_1F4A , m_pSnFhyo40Data->Sn_1F4B );
//		m_Arith.l_add( W0 , W0 , m_pSnFhyo40Data->Sn_1F4C );
//---------------
		if( IsNeedExpCalq() ){
			m_Arith.l_clear( W0 );
			m_Arith.l_add( W0, W0, m_pSnFhyo40Data->Sn_1F4T );
		}
		else{
			m_Arith.l_add( W0, m_pSnFhyo40Data->Sn_1F4A, m_pSnFhyo40Data->Sn_1F4B );
			m_Arith.l_add( W0, W0, m_pSnFhyo40Data->Sn_1F4C );
		}
//---------------
		m_Arith.l_print( dp, W0, FMT15 );
		PF1[i].PITM[j++] = dp;	dp += 16;			// áC-D
	}
	else{
		PF1[i].PITM[j++] = (char *)0;
		PF1[i].PITM[j++] = (char *)0;
		PF1[i].PITM[j++] = (char *)0;
		PF1[i].PITM[j++] = (char *)0;
	}
	if( j = _xprn( PP, 1, PF1, 0, pDC, isNoImage, 1 ) )	goto POUT52;

	i = j = 0;
	
	if( Flg == 1 )	{
	// íPàÍéñã∆é“
		// éÅñº
		dp = AP;	i = j = 0;
		memset( dp, '\0', size );
		if( kojin ) {	// å¬êléñã∆é“ÇÃèÍçáÇÕë„ï\é“éÅñº
			j = sizeof( m_pSnHeadData->Sn_DIHYO );
			memmove( dp, m_pSnHeadData->Sn_DIHYO, j );
		}
		else {			// ñ@êlñº
			j = sizeof( m_pSnHeadData->Sn_CONAM );
			memmove( dp, m_pSnHeadData->Sn_CONAM, j );
		}
		PF1[0].PITM[0] = dp;
		if( j = _xprn( PP, 4, PF1, 2, pDC, 0, 1 ) )	goto POUT52;

		// íPàÍéñã∆é“Å@Åõ
		dp = AP;	i = j = 0;
		memset( dp, '\0', size );
		pbclr( 0, 5, &PF1[0] );			// [07'09.25]
//-- '15.02.10 --
//		if( m_pSnHonpyoData->Sn_UP1 >= 1000 ) PF1[ 0 ].PITM[ 0 ] = "Å@";
//		if( m_pSnHonpyoData->Sn_UP2 >= 1000 ) PF1[ 0 ].PITM[ 1 ] = "Å@";
//		if( m_pSnHonpyoData->Sn_UP3 >= 1000 ) PF1[ 0 ].PITM[ 2 ] = "Å@";
//		if( m_pSnHonpyoData->Sn_UP4 >= 1000 ) PF1[ 0 ].PITM[ 3 ] = "Å@";
//		if( m_pSnHonpyoData->Sn_UP5 >= 1000 ) PF1[ 0 ].PITM[ 4 ] = "Å@";
//---------------
		if( IsNeedExpCalq() == FALSE ){
			if( m_pSnHonpyoData->Sn_UP1 >= 1000 ) PF1[ 0 ].PITM[ 0 ] = "Å@";
			if( m_pSnHonpyoData->Sn_UP2 >= 1000 ) PF1[ 0 ].PITM[ 1 ] = "Å@";
			if( m_pSnHonpyoData->Sn_UP3 >= 1000 ) PF1[ 0 ].PITM[ 2 ] = "Å@";
			if( m_pSnHonpyoData->Sn_UP4 >= 1000 ) PF1[ 0 ].PITM[ 3 ] = "Å@";
			if( m_pSnHonpyoData->Sn_UP5 >= 1000 ) PF1[ 0 ].PITM[ 4 ] = "Å@";
		}
		else{
			if( gyoFlg&0x01 ){
				PF1[ 0 ].PITM[ 0 ] = "Å@";
			}
			else if( gyoFlg&0x02 ){
				PF1[ 0 ].PITM[ 1 ] = "Å@";
			}
			else if( gyoFlg&0x04 ){
				PF1[ 0 ].PITM[ 2 ] = "Å@";
			}
			else if( gyoFlg&0x08 ){
				PF1[ 0 ].PITM[ 3 ] = "Å@";
			}
			else if( gyoFlg&0x10 ){
				PF1[ 0 ].PITM[ 4 ] = "Å@";
			}
		}
//---------------
		m_pVprn->SetSfncEnv( SElfnc, 1 );
		if( j = _xprn( PP, 5, PF1, 2, pDC, 0, 1 ) )	goto POUT52;

		goto POUT52;		// íPàÍéñã∆
	}

	i = 0, j = 0;
	dp = AP;
	memset( dp, '\0', size );
	pbclr( 0, 2, &PF1[i] );
	pbclr( 0, 1, &PF1[i+1] );
	pbclr( 0, 15, &PF1[i+2] );

	// â€ê≈îÑè„çÇ3%ìKópï™A
//-- '15.02.03 --
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FURITA, FMT15 );
//---------------
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI1A) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI2A) ||
		m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI3A) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI4A) || 
		m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI5A) ){
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FURITA, FMT16 );
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FURITA, FMT15 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// áE-åv
	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FURI1A, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// áF-åv
	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FURI2A, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// áG-åv
	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FURI3A, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// áH-åv
	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FURI4A, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// áI-åv
//-- '15.02.03 --
////	if( ! m_pSnHeadData->SV_5sw ) {
//	if( 1 ) {
//		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FURI5A, FMT15 );
//		PF1[i++].PITM[j] = dp;	dp += 16;		// áJ-åv
//	}
//	else	PF1[i++].PITM[j] = (char *)0;
//---------------
	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FURI5A, FMT15 );
	PF1[i++].PITM[j] = dp;	dp += 16;		// áJ-åv
//---------------
	
	j = 0;				
	// â€ê≈îÑè„çÇ4%ìKópï™B
//-- '15.02.03 --
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FURITB, FMT15 );
//---------------
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI1B) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI2B) ||
		m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI3B) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI4B) || 
		m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI5B) ){
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FURITB, FMT16 );
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FURITB, FMT15 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// áE-åv
	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FURI1B, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// áF-åv
	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FURI2B, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// áG-åv
	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FURI3B, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// áH-åv
	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FURI4B, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// áI-åv
//-- '15.02.03 --
////	if( ! m_pSnHeadData->SV_5sw ) {
//	if( 1 ) {
//		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FURI5B, FMT15 );
//		PF1[i++].PITM[j] = dp;	dp += 16;		// áJ-åv
//	}
//	else	PF1[i++].PITM[j] = (char *)0;
//---------------
	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FURI5B, FMT15 );
	PF1[i++].PITM[j] = dp;	dp += 16;		// áJ-åv
//---------------
	
	j = 0;				
	// â€ê≈îÑè„çÇ6.3%ìKópï™C
//-- '15.02.03 --
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FURITC, FMT15 );
//---------------
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI1C) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI2C) ||
		m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI3C) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI4C) || 
		m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI5C) ){
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FURITC, FMT16 );
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FURITC, FMT15 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// áE-åv
	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FURI1C, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// áF-åv
	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FURI2C, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// áG-åv
	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FURI3C, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// áH-åv
	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FURI4C, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// áI-åv
//-- '15.02.03 --
////	if( ! m_pSnHeadData->SV_5sw ) {
//	if( 1 ) {
//		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FURI5C, FMT15 );
//		PF1[i++].PITM[j] = dp;	dp += 16;		// áJ-åv
//	}
//	else	PF1[i++].PITM[j] = (char *)0;
//---------------
	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FURI5C, FMT15 );
	PF1[i++].PITM[j] = dp;	dp += 16;		// áJ-åv
//---------------

	j = 0;	
	// â€ê≈îÑè„çÇçáåvD
	m_Arith.l_add( W0 , m_pSnFhyo52Data->Sn_5FURITA , m_pSnFhyo52Data->Sn_5FURITB );
	m_Arith.l_add( W0 , W0 , m_pSnFhyo52Data->Sn_5FURITC );
//--> '14.10.07 INS START
	if( m_Arith.l_test(W0) < 0 ){
		m_Arith.l_clear(W0);
	}
//<-- '14.10.07 INS END
//-- '15.01.27 --
//	m_Arith.l_print( dp, W0, FMT15 );
//---------------
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURITA) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURITB) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURITC) ){
		m_Arith.l_print( dp, W0, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W0, FMT15 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// á@
	m_Arith.l_add( W0 , m_pSnFhyo52Data->Sn_5FURI1A , m_pSnFhyo52Data->Sn_5FURI1B );
	m_Arith.l_add( W0 , W0 , m_pSnFhyo52Data->Sn_5FURI1C );
//--> '14.10.07 INS START
	if( m_Arith.l_test(W0) < 0 ){
		m_Arith.l_clear(W0);
	}
//<-- '14.10.07 INS END
//-- '15.01.27 --
//	m_Arith.l_print( dp, W0, FMT15 );
//---------------
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI1A) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI1B) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI1C) ){
		m_Arith.l_print( dp, W0, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W0, FMT15 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// áA
	m_Arith.l_add( W0 , m_pSnFhyo52Data->Sn_5FURI2A , m_pSnFhyo52Data->Sn_5FURI2B );
	m_Arith.l_add( W0 , W0 , m_pSnFhyo52Data->Sn_5FURI2C );
//--> '14.10.07 INS START
	if( m_Arith.l_test(W0) < 0 ){
		m_Arith.l_clear(W0);
	}
//<-- '14.10.07 INS END
//-- '15.01.27 --
//	m_Arith.l_print( dp, W0, FMT15 );
//---------------
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI2A) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI2B) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI2C) ){
		m_Arith.l_print( dp, W0, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W0, FMT15 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// áB
	m_Arith.l_add( W0 , m_pSnFhyo52Data->Sn_5FURI3A , m_pSnFhyo52Data->Sn_5FURI3B );
	m_Arith.l_add( W0 , W0 , m_pSnFhyo52Data->Sn_5FURI3C );
//--> '14.10.07 INS START
	if( m_Arith.l_test(W0) < 0 ){
		m_Arith.l_clear(W0);
	}
//<-- '14.10.07 INS END
//-- '15.01.27 --
//	m_Arith.l_print( dp, W0, FMT15 );
//---------------
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI3A) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI3B) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI3C) ){
		m_Arith.l_print( dp, W0, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W0, FMT15 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// áC
	m_Arith.l_add( W0 , m_pSnFhyo52Data->Sn_5FURI4A , m_pSnFhyo52Data->Sn_5FURI4B );
	m_Arith.l_add( W0 , W0 , m_pSnFhyo52Data->Sn_5FURI4C );
//--> '14.10.07 INS START
	if( m_Arith.l_test(W0) < 0 ){
		m_Arith.l_clear(W0);
	}
//<-- '14.10.07 INS END
//-- '15.01.27 --
//	m_Arith.l_print( dp, W0, FMT15 );
//---------------
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI4A) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI4B) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI4C) ){
		m_Arith.l_print( dp, W0, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W0, FMT15 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// áD
	m_Arith.l_add( W0 , m_pSnFhyo52Data->Sn_5FURI5A , m_pSnFhyo52Data->Sn_5FURI5B );
	m_Arith.l_add( W0 , W0 , m_pSnFhyo52Data->Sn_5FURI5C );
//--> '14.10.07 INS START
	if( m_Arith.l_test(W0) < 0 ){
		m_Arith.l_clear(W0);
	}
//<-- '14.10.07 INS END
//-- '15.01.27 --
//	m_Arith.l_print( dp, W0, FMT15 );
//---------------
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI5A) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI5B) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FURI5C) ){
		m_Arith.l_print( dp, W0, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W0, FMT15 );
	}
//---------------
	PF1[i++].PITM[j++] = dp;	dp += 16;			// áE

	pbclr( 0, 5, &PF1[i] );
	pbclr( 0, 5, &PF1[i+1] );
	j = 0;											// äÑçááF->áJ
	if( m_pSnHonpyoData->Sn_UP1 ){
		wsprintf( dp, "%3d.%01d", m_pSnHonpyoData->Sn_UP1 / 10, m_pSnHonpyoData->Sn_UP1 % 10 );
	}
	if( m_pSnHonpyoData->Sn_UP1 >= 1000 ){
		PF1[i+1].PITM[j++] = dp;//ëÂÇ´Ç¢Ç∆Ç´ÇÕ2î‘ñ⁄ÇÃílÇ≈àÛéö
	}
	else{
		PF1[i].PITM[j++] = dp;
	}
	dp += 6;		// îÑè„äÑçááF-åv
	if( m_pSnHonpyoData->Sn_UP2 ){
		wsprintf( dp, "%3d.%01d", m_pSnHonpyoData->Sn_UP2 / 10, m_pSnHonpyoData->Sn_UP2 % 10 );
	}
	if( m_pSnHonpyoData->Sn_UP2 >= 1000 ){
		PF1[i+1].PITM[j++] = dp;//ëÂÇ´Ç¢Ç∆Ç´ÇÕ2î‘ñ⁄ÇÃílÇ≈àÛéö
	}
	else{
		PF1[i].PITM[j++] = dp;
	}
	dp += 6;		// îÑè„äÑçááF-åv
	if( m_pSnHonpyoData->Sn_UP3 ){
		wsprintf( dp, "%3d.%01d", m_pSnHonpyoData->Sn_UP3 / 10, m_pSnHonpyoData->Sn_UP3 % 10 );
	}
	if( m_pSnHonpyoData->Sn_UP3 >= 1000 ){
		PF1[i+1].PITM[j++] = dp;//ëÂÇ´Ç¢Ç∆Ç´ÇÕ2î‘ñ⁄ÇÃílÇ≈àÛéö
	}
	else{
		PF1[i].PITM[j++] = dp;
	}
	dp += 6;		// îÑè„äÑçááF-åv
	if( m_pSnHonpyoData->Sn_UP4 ){
		wsprintf( dp, "%3d.%01d", m_pSnHonpyoData->Sn_UP4 / 10, m_pSnHonpyoData->Sn_UP4 % 10 );
	}
	if( m_pSnHonpyoData->Sn_UP4 >= 1000 ){
		PF1[i+1].PITM[j++] = dp;//ëÂÇ´Ç¢Ç∆Ç´ÇÕ2î‘ñ⁄ÇÃílÇ≈àÛéö
	}
	else{
		PF1[i].PITM[j++] = dp;
	}
	dp += 6;		// îÑè„äÑçááF-åv
//	if( ! m_pSnHeadData->SV_5sw ) {
	if( 1 ) {
		if( m_pSnHonpyoData->Sn_UP5 ){
			wsprintf( dp, "%3d.%01d", m_pSnHonpyoData->Sn_UP5 / 10, m_pSnHonpyoData->Sn_UP5 % 10 );
		}
		if( m_pSnHonpyoData->Sn_UP5 >= 1000 ){
			PF1[i+1].PITM[j++] = dp;//ëÂÇ´Ç¢Ç∆Ç´ÇÕ2î‘ñ⁄ÇÃílÇ≈àÛéö
		}
		else{
			PF1[i].PITM[j++] = dp;
		}
	}
	else {
		PF1[i].PITM[j++] = (char *)0;
		PF1[i++].PITM[j] = (char *)0;
	}

	if( j = _xprn( PP, 2, PF1, 0, pDC, 0, 1 ) )	goto POUT52;

	i = 0, j = 0;
	dp = AP;
	memset( dp, '\0', size );
	pbclr( 0, 2, &PF1[i] );
	pbclr( 0, 1, &PF1[i+1] );
	pbclr( 0, 15, &PF1[i+2] );

//--> '14.10.20 INS START
	unsigned char	zeroBuf[MONY_BUF_SIZE] = {0};
//<-- '14.10.20 INS END

//--> '15.02.03 INS START
	BOOL	isTotalZeroOut = FALSE;	// 12DÇÃ0â~ÇèoóÕÇ∑ÇÈÇ©Ç«Ç§Ç©
//--> '15.02.03 INS END

	j = 0;
	// è¡îÔê≈äzÇÃñæç◊
	// è¡îÔê≈äz3%ìKópï™A
//-- '15.01.27 --
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZKTA, FMT15 );
//---------------
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK1A) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK2A) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK3A) ||
		m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK4A) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK5A) ){
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZKTA, FMT16 );
//--> '15.02.03 INS START
		isTotalZeroOut = TRUE;
//--> '15.02.03 INS END
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZKTA, FMT15 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// áK-åv
//-- '14.10.20 --
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK1A, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// 13
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK2A, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// 14
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK3A, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// 15
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK4A, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// 16
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK5A, FMT15 );
//	PF1[i++].PITM[j] = dp;	dp += 16;			// 17
//---------------
	// 13
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK1A) >= 0 ){
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK1A, FMT15 );
	}
	else{
		m_Arith.l_print( dp, zeroBuf, FMT16 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;
	// 14
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK2A) >= 0 ){
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK2A, FMT15 );
	}
	else{
		m_Arith.l_print( dp, zeroBuf, FMT16 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;
	// 15
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK3A) >= 0 ){
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK3A, FMT15 );
	}
	else{
		m_Arith.l_print( dp, zeroBuf, FMT16 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;
	// 16
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK4A) >= 0 ){
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK4A, FMT15 );
	}
	else{
		m_Arith.l_print( dp, zeroBuf, FMT16 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;
	// 17
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK5A) >= 0 ){
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK5A, FMT15 );
	}
	else{
		m_Arith.l_print( dp, zeroBuf, FMT16 );
	}
	PF1[i++].PITM[j] = dp;	dp += 16;
//---------------

	j = 0;				
	// è¡îÔê≈äz4%ìKópï™B
//-- '15.01.27 --
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZKTB, FMT15 );
//---------------
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK1B) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK2B) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK3B) ||
		m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK4B) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK5B) ){
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZKTB, FMT16 );
//--> '15.02.03 INS START
		isTotalZeroOut = TRUE;
//--> '15.02.03 INS END
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZKTB, FMT15 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// áK-åv
//-- '14.10.20 --
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK1B, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// 13
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK2B, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// 14
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK3B, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// 15
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK4B, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// 16
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK5B, FMT15 );
//	PF1[i++].PITM[j] = dp;	dp += 16;			// 17
//---------------
	// 13
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK1B) >= 0 ){
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK1B, FMT15 );
	}
	else{
		m_Arith.l_print( dp, zeroBuf, FMT16 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;
	// 14
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK2B) >= 0 ){
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK2B, FMT15 );
	}
	else{
		m_Arith.l_print( dp, zeroBuf, FMT16 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;
	// 15
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK3B) >= 0 ){
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK3B, FMT15 );
	}
	else{
		m_Arith.l_print( dp, zeroBuf, FMT16 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;
	// 16
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK4B) >= 0 ){
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK4B, FMT15 );
	}
	else{
		m_Arith.l_print( dp, zeroBuf, FMT16 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;
	// 17
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK5B) >= 0 ){
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK5B, FMT15 );
	}
	else{
		m_Arith.l_print( dp, zeroBuf, FMT16 );
	}
	PF1[i++].PITM[j] = dp;	dp += 16;
//---------------
		
	j = 0;				
	// è¡îÔê≈äz6.3%ìKópï™C
//-- '15.01.27 --
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZKTC, FMT15 );
//---------------
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK1C) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK2C) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK3C) ||
		m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK4C) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK5C) ){
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZKTC, FMT16 );
//--> '15.02.03 INS START
		isTotalZeroOut = TRUE;
//--> '15.02.03 INS END
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZKTC, FMT15 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// áK-åv
//-- '14.10.20 --
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK1C, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// 13
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK2C, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// 14
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK3C, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// 15
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK4C, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// 16
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK5C, FMT15 );
//	PF1[i++].PITM[j] = dp;	dp += 16;			// 17
//---------------
	// 13
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK1C) >= 0 ){
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK1C, FMT15 );
	}
	else{
		m_Arith.l_print( dp, zeroBuf, FMT16 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;
	// 14
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK2C) >= 0 ){
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK2C, FMT15 );
	}
	else{
		m_Arith.l_print( dp, zeroBuf, FMT16 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;
	// 15
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK3C) >= 0 ){
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK3C, FMT15 );
	}
	else{
		m_Arith.l_print( dp, zeroBuf, FMT16 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;
	// 16
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK4C) >= 0 ){
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK4C, FMT15 );
	}
	else{
		m_Arith.l_print( dp, zeroBuf, FMT16 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;
	// 17
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK5C) >= 0 ){
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK5C, FMT15 );
	}
	else{
		m_Arith.l_print( dp, zeroBuf, FMT16 );
	}
	PF1[i++].PITM[j] = dp;	dp += 16;
//---------------

	j = 0;	
	// è¡îÔê≈äzçáåvD
//-- '15.01.26 --
//	m_Arith.l_add( W0 , m_pSnFhyo52Data->Sn_5FUZKTA , m_pSnFhyo52Data->Sn_5FUZKTB );
//	m_Arith.l_add( W0 , W0 , m_pSnFhyo52Data->Sn_5FUZKTC );
//---------------
	m_Arith.l_clear( W0 );
	m_Arith.l_add( W0, W0, m_pSnFhyo52Data->Sn_5FUZTT );
//---------------
//--> '14.10.07 INS START
	if( m_Arith.l_test(W0) < 0 ){
		m_Arith.l_clear(W0);
	}
//<-- '14.10.07 INS END
//-- '15.01.27 --
//	m_Arith.l_print( dp, W0, FMT15 );
//---------------
//--- '15.02.02 ---
//	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZKTA) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZKTB) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZKTC) ){
//-----------------
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZKTA) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZKTB) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZKTC) || isTotalZeroOut ){
//-----------------
		m_Arith.l_print( dp, W0, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W0, FMT15 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// á@
	m_Arith.l_add( W0 , m_pSnFhyo52Data->Sn_5FUZK1A , m_pSnFhyo52Data->Sn_5FUZK1B );
	m_Arith.l_add( W0 , W0 , m_pSnFhyo52Data->Sn_5FUZK1C );
//--> '14.10.07 INS START
	if( m_Arith.l_test(W0) < 0 ){
		m_Arith.l_clear(W0);
	}
//<-- '14.10.07 INS END
//-- '15.01.27 --
//	m_Arith.l_print( dp, W0, FMT15 );
//---------------
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK1A) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK1B) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK1C)){
		m_Arith.l_print( dp, W0, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W0, FMT15 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// áA
	m_Arith.l_add( W0 , m_pSnFhyo52Data->Sn_5FUZK2A , m_pSnFhyo52Data->Sn_5FUZK2B );
	m_Arith.l_add( W0 , W0 , m_pSnFhyo52Data->Sn_5FUZK2C );
//--> '14.10.07 INS START
	if( m_Arith.l_test(W0) < 0 ){
		m_Arith.l_clear(W0);
	}
//<-- '14.10.07 INS END
//-- '15.01.27 --
//	m_Arith.l_print( dp, W0, FMT15 );
//---------------
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK2A) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK2B) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK2C)){
		m_Arith.l_print( dp, W0, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W0, FMT15 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// áB
	m_Arith.l_add( W0 , m_pSnFhyo52Data->Sn_5FUZK3A , m_pSnFhyo52Data->Sn_5FUZK3B );
	m_Arith.l_add( W0 , W0 , m_pSnFhyo52Data->Sn_5FUZK3C );
//--> '14.10.07 INS START
	if( m_Arith.l_test(W0) < 0 ){
		m_Arith.l_clear(W0);
	}
//<-- '14.10.07 INS END
//-- '15.01.27 --
//	m_Arith.l_print( dp, W0, FMT15 );
//---------------
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK3A) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK3B) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK3C)){
		m_Arith.l_print( dp, W0, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W0, FMT15 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// áC
	m_Arith.l_add( W0 , m_pSnFhyo52Data->Sn_5FUZK4A , m_pSnFhyo52Data->Sn_5FUZK4B );
	m_Arith.l_add( W0 , W0 , m_pSnFhyo52Data->Sn_5FUZK4C );
//--> '14.10.07 INS START
	if( m_Arith.l_test(W0) < 0 ){
		m_Arith.l_clear(W0);
	}
//<-- '14.10.07 INS END
//-- '15.01.27 --
//	m_Arith.l_print( dp, W0, FMT15 );
//---------------
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK4A) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK4B) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK4C)){
		m_Arith.l_print( dp, W0, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W0, FMT15 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// áD
	m_Arith.l_add( W0 , m_pSnFhyo52Data->Sn_5FUZK5A , m_pSnFhyo52Data->Sn_5FUZK5B );
	m_Arith.l_add( W0 , W0 , m_pSnFhyo52Data->Sn_5FUZK5C );
//--> '14.10.07 INS START
	if( m_Arith.l_test(W0) < 0 ){
		m_Arith.l_clear(W0);
	}
//<-- '14.10.07 INS END
//-- '15.01.27 --
//	m_Arith.l_print( dp, W0, FMT15 );
//---------------
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK5A) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK5B) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZK5C)){
		m_Arith.l_print( dp, W0, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W0, FMT15 );
	}
//---------------
	PF1[i++].PITM[j++] = dp;	dp += 16;		// áE
/*
//	if( ! TBhead.SV_5sw ) {
	if( 1 ) {
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZT5, FMT14 );
		PF1[i].PITM[j++] = dp;	dp += 15;		// áP-åv
//		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK5, FMT14 );
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK5B, FMT14 );
		PF1[i++].PITM[j] = dp;	dp += 15;		// áP-4%
	}
	else {
		PF1[i].PITM[j++] = (char *)0;
		PF1[i++].PITM[j] = (char *)0;
	}
*/
	if( j = _xprn( PP, 3, PF1, 0, pDC, 0, 1 ) )	goto POUT52;

	// éÅñº
	dp = AP;	i = j = 0;
	memset( dp, '\0', size );
	if( kojin ) {	// å¬êléñã∆é“ÇÃèÍçáÇÕë„ï\é“éÅñº
		j = sizeof( m_pSnHeadData->Sn_DIHYO );
		memmove( dp, m_pSnHeadData->Sn_DIHYO, j );
	}
	else {			// ñ@êlñº
		j = sizeof( m_pSnHeadData->Sn_CONAM );
		memmove( dp, m_pSnHeadData->Sn_CONAM, j );
	}
	PF1[0].PITM[0] = dp;
	if( j = _xprn( PP, 4, PF1, 2, pDC, 0, 1 ) )	goto POUT52;

/*	// ì¡ó·åvéZÇìKópÇ≈ÇPéÌóﬁÇÃéñã∆Ç≈ÇVÇTÅìà»è„ÇÃèÍçá
//	if( (Snd.Sn_5FSZsw&0xff) == 0x01 ) {
	if( 1 ) {
		dp = AP;	i = j = 0;
		memset( dp, '\0', size );
		pbclr( 0, 5,  &PF1[i] );

		if( m_pSnHonpyoData->Sn_UP1 >= 750 ) PF1[ 0 ].PITM[ 0 ] = "Å@";
		if( m_pSnHonpyoData->Sn_UP2 >= 750 ) PF1[ 0 ].PITM[ 1 ] = "Å@";
		if( m_pSnHonpyoData->Sn_UP3 >= 750 ) PF1[ 0 ].PITM[ 2 ] = "Å@";
		if( m_pSnHonpyoData->Sn_UP4 >= 750 ) PF1[ 0 ].PITM[ 3 ] = "Å@";
		if( m_pSnHonpyoData->Sn_UP5 >= 750 ) PF1[ 0 ].PITM[ 4 ] = "Å@";
		//ÉtÉåÅ[ÉÄéÌÇóLå¯Ç∆Ç∑ÇÈà◊ÇÃëOèàóù
		m_pVprn->SetSfncEnv( SElfnc, 1 );
		if( j = _xprn( PP, 6, PF1, 0, pDC, 0, 1 ) )	goto POUT52;
	}
*/
POUT52:
	free( AP );
	
	return( j );
}

//-----------------------------------------------------------------------------
// ïtï\ÇTÅ|ÇQ_ÇQèoóÕ
//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CH26HyoView::PrintFuhyo52_2( CDC *pDC, int kojin, int pcnt )
{

	int				i, j;
	unsigned int	size;
	char			*AP, *dp;
	
	BOOL	isNoImage = FALSE;
	if( m_pPrintCmInfo->PRtype&0x80 ){
		isNoImage = FALSE;	// ÉCÉÅÅ[ÉWèoóÕóLÇË
	}
	else{
		isNoImage = TRUE;	// ÉCÉÅÅ[ÉWèoóÕñ≥Çµ
	}
	
	size = 512;
	if( (AP = new char[size]) == NULL ){
		return -1;
	}
	
	// èoóÕëOèWåv
//	FuHyo52Calqe();

PBUF	PF1[30];
int		PP;


	PP = 8;

	dp = AP;
	memset( dp, '\0', size );
	i = 0;

int CCSw = 0;
char	W0[MONY_BUF_SIZE] = {0};

int		Fno = 0;
	i = 0;
	j = 0;		

	
	// áQ
	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZKGA, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// áE-åv
	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZKGB, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// áF-åv
	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZKGC, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// áF-åv
//-- '14.10.17 --
//	m_Arith.l_add( W0 , m_pSnFhyo52Data->Sn_5FSZKGA , m_pSnFhyo52Data->Sn_5FSZKGB );
//	m_Arith.l_add( W0 , W0 , m_pSnFhyo52Data->Sn_5FSZKGC );
//	m_Arith.l_print( dp, W0 , FMT15 );
//---------------
//--- '15.01.27 ---
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZTG , FMT15 );
//-----------------
	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FSZKGA) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FSZKGB) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FSZKGC) ){
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZTG , FMT16 );
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZTG , FMT15 );
	}
//-----------------
//---------------
	PF1[i++].PITM[j++] = dp;	dp += 16;			// áI-åv
	// áR
	j = 0;

//--> '15.03.10 INS START
	BOOL	IsOneOver750 = FALSE;
	if( m_pFhyo52 ){
		int dspSkjSw = m_pFhyo52->GetMostProfitCalq();
		if( dspSkjSw&0x02 ){
			IsOneOver750 = TRUE;
		}
	}
//<-- '15.03.10 INS END

//-- '15.03.10 --
//	if( m_pSnFhyo52Data->Sn_5FSZsw == 1 ){
//---------------
	if( IsOneOver750 ){
//---------------
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZKTA, FMT15 );
		PF1[i].PITM[j++] = dp;	dp += 16;			// áE-åv
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZKTB, FMT15 );
		PF1[i].PITM[j++] = dp;	dp += 16;			// áF-åv
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZKTC, FMT15 );
		PF1[i].PITM[j++] = dp;	dp += 16;			// áF-åv
//-- '14.10.17 --
//		m_Arith.l_add( W0 , m_pSnFhyo52Data->Sn_5FSZKTA , m_pSnFhyo52Data->Sn_5FSZKTB );
//		m_Arith.l_add( W0 , W0 , m_pSnFhyo52Data->Sn_5FSZKTC );
//		m_Arith.l_print( dp, W0 , FMT15 );
//---------------
//--- '15.01.27 ---
//		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZTT, FMT15 );
//-----------------
		if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FSZKTA) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FSZKTB) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FSZKTC) ){
			m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZTT, FMT16 );
		}
		else{
			m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZTT, FMT15 );
		}
//-----------------
//---------------
		PF1[i++].PITM[j++] = dp;	dp += 16;			// áI-åv
	}
	else{
		PF1[i].PITM[j++] = (char *)0;
		PF1[i].PITM[j++] = (char *)0;
		PF1[i].PITM[j++] = (char *)0;
		PF1[i].PITM[j++] = (char *)0;
	}
	if( j = _xprn( PP, 1, PF1, 0, pDC, isNoImage, 1 ) )	goto POUT52;

	i = j = 0;
	
	dp = AP;
	memset( dp, '\0', size );

	char	W1[MONY_BUF_SIZE] = {0};
	char	W2[MONY_BUF_SIZE] = {0};
	char	W3[MONY_BUF_SIZE] = {0};
	char	W4[MONY_BUF_SIZE] = {0};

//-- '14.10.17 --
//	int Sign = m_pSnFhyo52Data->Sn_5FSZsw -2;
//---------------
	int Sign = m_pSnFhyo52Data->Sn_5FSZsw -2;
	if( m_pFhyo52 ){
		int dspSkjSw = m_pFhyo52->GetMostProfitCalq();

		Sign = 0;
		for( int i=0, chkflg=0x04; i<10; i++, chkflg<<=1 ){
			if( dspSkjSw&chkflg ){
				break;
			}
			Sign++;
		}
	}
//---------------
	if( Sign >= 0 ){
		switch( Sign ){
			case 0: 
				memmove( W1 , m_pSnFhyo52Data->Sn_5FSZT20A , MONY_BUF_SIZE );
				memmove( W2 , m_pSnFhyo52Data->Sn_5FSZT20B , MONY_BUF_SIZE );
				memmove( W3 , m_pSnFhyo52Data->Sn_5FSZT20C , MONY_BUF_SIZE );
				memmove( W4 , m_pSnFhyo52Data->Sn_5FSZT20T , MONY_BUF_SIZE );
				break;
			case 1: 
				memmove( W1 , m_pSnFhyo52Data->Sn_5FSZT21A , MONY_BUF_SIZE );
				memmove( W2 , m_pSnFhyo52Data->Sn_5FSZT21B , MONY_BUF_SIZE );
				memmove( W3 , m_pSnFhyo52Data->Sn_5FSZT21C , MONY_BUF_SIZE );
				memmove( W4 , m_pSnFhyo52Data->Sn_5FSZT21T , MONY_BUF_SIZE );
				break;

			case 2: 
				memmove( W1 , m_pSnFhyo52Data->Sn_5FSZT22A , MONY_BUF_SIZE );
				memmove( W2 , m_pSnFhyo52Data->Sn_5FSZT22B , MONY_BUF_SIZE );
				memmove( W3 , m_pSnFhyo52Data->Sn_5FSZT22C , MONY_BUF_SIZE );
				memmove( W4 , m_pSnFhyo52Data->Sn_5FSZT22T , MONY_BUF_SIZE );
				break;

			case 3: 
				memmove( W1 , m_pSnFhyo52Data->Sn_5FSZT23A , MONY_BUF_SIZE );
				memmove( W2 , m_pSnFhyo52Data->Sn_5FSZT23B , MONY_BUF_SIZE );
				memmove( W3 , m_pSnFhyo52Data->Sn_5FSZT23C , MONY_BUF_SIZE );
				memmove( W4 , m_pSnFhyo52Data->Sn_5FSZT23T , MONY_BUF_SIZE );
				break;

			case 4: 
				memmove( W1 , m_pSnFhyo52Data->Sn_5FSZT24A , MONY_BUF_SIZE );
				memmove( W2 , m_pSnFhyo52Data->Sn_5FSZT24B , MONY_BUF_SIZE );
				memmove( W3 , m_pSnFhyo52Data->Sn_5FSZT24C , MONY_BUF_SIZE );
				memmove( W4 , m_pSnFhyo52Data->Sn_5FSZT24T , MONY_BUF_SIZE );
				break;

			case 5: 
				memmove( W1 , m_pSnFhyo52Data->Sn_5FSZT25A , MONY_BUF_SIZE );
				memmove( W2 , m_pSnFhyo52Data->Sn_5FSZT25B , MONY_BUF_SIZE );
				memmove( W3 , m_pSnFhyo52Data->Sn_5FSZT25C , MONY_BUF_SIZE );
				memmove( W4 , m_pSnFhyo52Data->Sn_5FSZT25T , MONY_BUF_SIZE );
				break;

			case 6: 
				memmove( W1 , m_pSnFhyo52Data->Sn_5FSZT26A , MONY_BUF_SIZE );
				memmove( W2 , m_pSnFhyo52Data->Sn_5FSZT26B , MONY_BUF_SIZE );
				memmove( W3 , m_pSnFhyo52Data->Sn_5FSZT26C , MONY_BUF_SIZE );
				memmove( W4 , m_pSnFhyo52Data->Sn_5FSZT26T , MONY_BUF_SIZE );
				break;
			case 7: 
				memmove( W1 , m_pSnFhyo52Data->Sn_5FSZT27A , MONY_BUF_SIZE );
				memmove( W2 , m_pSnFhyo52Data->Sn_5FSZT27B , MONY_BUF_SIZE );
				memmove( W3 , m_pSnFhyo52Data->Sn_5FSZT27C , MONY_BUF_SIZE );
				memmove( W4 , m_pSnFhyo52Data->Sn_5FSZT27T , MONY_BUF_SIZE );
				break;
			case 8: 
				memmove( W1 , m_pSnFhyo52Data->Sn_5FSZT28A , MONY_BUF_SIZE );
				memmove( W2 , m_pSnFhyo52Data->Sn_5FSZT28B , MONY_BUF_SIZE );
				memmove( W3 , m_pSnFhyo52Data->Sn_5FSZT28C , MONY_BUF_SIZE );
				memmove( W4 , m_pSnFhyo52Data->Sn_5FSZT28T , MONY_BUF_SIZE );
				break;
			case 9: 
				memmove( W1 , m_pSnFhyo52Data->Sn_5FSZT29A , MONY_BUF_SIZE );
				memmove( W2 , m_pSnFhyo52Data->Sn_5FSZT29B , MONY_BUF_SIZE );
				memmove( W3 , m_pSnFhyo52Data->Sn_5FSZT29C , MONY_BUF_SIZE );
				memmove( W4 , m_pSnFhyo52Data->Sn_5FSZT29T , MONY_BUF_SIZE );
				break;
		}
	}
	//20Å`29
//-- '14.10.17 --
//	for(j = 0; j < 10; j++ ) {			// áS->(29)
//		if( m_pSnFhyo52Data->Sn_5FSZsw >= 2 ){
//			if( m_pSnFhyo52Data->Sn_5FSZsw == (j+2) ){
//				m_Arith.l_print( dp, W1, FMT14 );
//				PF1[0].PITM[j] = dp;		dp += 15;
//				m_Arith.l_print( dp, W2, FMT14 );
//				PF1[1].PITM[j] = dp;	dp += 15;
//			}
//			else{
//				PF1[0].PITM[j] = (char *)0;
//				PF1[1].PITM[j] = (char *)0;
//			}
//		}
//		else{
//			PF1[0].PITM[j] = (char *)0;
//			PF1[1].PITM[j] = (char *)0;
//		}
//	}
//---------------
	for( j=0; j<10; j++ ){			// 20 -> 29
		if( Sign >= 0 ){
			if( Sign == j ){
				m_Arith.l_print( dp, W1, FMT14 );
				PF1[0].PITM[j] = dp;		dp += 15;
				m_Arith.l_print( dp, W2, FMT14 );
				PF1[1].PITM[j] = dp;	dp += 15;
			}
			else{
				PF1[0].PITM[j] = (char *)0;
				PF1[1].PITM[j] = (char *)0;
			}
		}
		else{
			PF1[0].PITM[j] = (char *)0;
			PF1[1].PITM[j] = (char *)0;
		}
	}
//---------------
	if( j = _xprn( PP, 2, PF1, 0, pDC, 0, 1 ) )	goto POUT52;

	i = 0, j = 0;
	dp = AP;
	memset( dp, '\0', size );
//-- '14.10.17 --
//	for(j = 0; j < 10; j++ ) {			// áS->(29)
//		if( m_pSnFhyo52Data->Sn_5FSZsw >= 2 ){
//			if( m_pSnFhyo52Data->Sn_5FSZsw == (j+2) ){
//				m_Arith.l_print( dp, W3, FMT14 );
//				PF1[0].PITM[j] = dp;	dp += 15;
//
//				m_Arith.l_print( dp, W4, FMT14 );
//				PF1[1].PITM[j] = dp;	dp += 15;
//			}
//			else{
//				PF1[0].PITM[j] = (char *)0;
//				PF1[1].PITM[j] = (char *)0;
//			}
//		}
//		else{
//			PF1[0].PITM[j] = (char *)0;
//			PF1[1].PITM[j] = (char *)0;
//		}
//	}
//---------------
	for( j=0; j<10; j++ ){			// 20 -> 29
		if( Sign >= 0 ){
			if( Sign == j ){
				m_Arith.l_print( dp, W3, FMT14 );
				PF1[0].PITM[j] = dp;	dp += 15;

//-- '15.01.27 --
//				m_Arith.l_print( dp, W4, FMT14 );
//---------------
				if( m_Arith.l_test(W1) || m_Arith.l_test(W2) || m_Arith.l_test(W3) ){
					m_Arith.l_print( dp, W4, FMT14Z );
				}
				else{
					m_Arith.l_print( dp, W4, FMT14 );
				}
//---------------
				PF1[1].PITM[j] = dp;	dp += 15;
			}
			else{
				PF1[0].PITM[j] = (char *)0;
				PF1[1].PITM[j] = (char *)0;
			}
		}
		else{
			PF1[0].PITM[j] = (char *)0;
			PF1[1].PITM[j] = (char *)0;
		}
	}
//---------------

	if( j = _xprn( PP, 3, PF1, 0, pDC, 0, 1 ) )	goto POUT52;

	dp = AP;	i = j = 0;
	memset( dp, '\0', size );

	// 30ÅFçTèúëŒè€édì¸ê≈äz
/*- '14.03.03 -*/
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZKTA, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// áE-åv
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZKTB, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// áF-åv
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZKTC, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// áF-åv
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZTT , FMT15 );
//	PF1[i++].PITM[j++] = dp;	dp += 16;			// áI-åv
/*-------------*/

	if( m_pSnFhyo52Data->Sn_5FSZsw&0xff ){
		if( m_pSnHeadData->Sn_TOKUR ){
//-- '14.10.17 --
//			memset( W0 , '\0' , sizeof( W0 ) );
//			m_Arith.l_print( dp, m_pSnFhyo40Data->Sn_1F4A, FMT15 );
//			m_Arith.l_add( W0 , W0 , m_pSnFhyo40Data->Sn_1F4A);
//			PF1[i].PITM[j++] = dp;	dp += 16;
//			m_Arith.l_print( dp, m_pSnFhyo40Data->Sn_1F4B, FMT15 );
//			m_Arith.l_add( W0 , W0 , m_pSnFhyo40Data->Sn_1F4B);
//			PF1[i].PITM[j++] = dp;	dp += 16;
//			m_Arith.l_print( dp, m_pSnFhyo40Data->Sn_1F4C, FMT15 );
//			m_Arith.l_add( W0 , W0 , m_pSnFhyo40Data->Sn_1F4C);
//			PF1[i].PITM[j++] = dp;	dp += 16;
//			m_Arith.l_print( dp, W0 , FMT15 );
//			PF1[i++].PITM[j++] = dp;	dp += 16;
//---------------
			m_Arith.l_print( dp, m_pSnFhyo40Data->Sn_1F4A, FMT15 );
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print( dp, m_pSnFhyo40Data->Sn_1F4B, FMT15 );
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print( dp, m_pSnFhyo40Data->Sn_1F4C, FMT15 );
			PF1[i].PITM[j++] = dp;	dp += 16;
//--- '15.01.27 ---
//			m_Arith.l_print( dp, m_pSnFhyo40Data->Sn_1F4T, FMT15 );
//-----------------
			if( m_Arith.l_test(m_pSnFhyo40Data->Sn_1F4A) || m_Arith.l_test(m_pSnFhyo40Data->Sn_1F4B) || m_Arith.l_test(m_pSnFhyo40Data->Sn_1F4C) ){
				m_Arith.l_print( dp, m_pSnFhyo40Data->Sn_1F4T, FMT16 );
			}
			else{
				m_Arith.l_print( dp, m_pSnFhyo40Data->Sn_1F4T, FMT15 );
			}
//-----------------
			PF1[i++].PITM[j++] = dp;	dp += 16;
//---------------
		}
		else{
			m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZKGA, FMT15 );
			PF1[i].PITM[j++] = dp;	dp += 16;			// áE-åv
			m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZKGB, FMT15 );
			PF1[i].PITM[j++] = dp;	dp += 16;			// áF-åv
			m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZKGC, FMT15 );
			PF1[i].PITM[j++] = dp;	dp += 16;			// áF-åv
//-- '15.01.27 --
//			m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZTG , FMT15 );
//---------------
			if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FSZKGA) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FSZKGB) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FSZKGC) ){
				m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZTG , FMT16 );
			}
			else{
				m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZTG , FMT15 );
			}
//---------------
			PF1[i++].PITM[j++] = dp;	dp += 16;			// áI-åv
		}
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZKGA, FMT15 );
		PF1[i].PITM[j++] = dp;	dp += 16;
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZKGB, FMT15 );
		PF1[i].PITM[j++] = dp;	dp += 16;
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZKGC, FMT15 );
		PF1[i].PITM[j++] = dp;	dp += 16;
//-- '15.01.27 --
//		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZTG , FMT15 );
//---------------
		if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FSZKGA) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FSZKGB) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FSZKGC) ){
			m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZTG , FMT16 );
		}
		else{
			m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZTG , FMT15 );
		}
//---------------
		PF1[i++].PITM[j++] = dp;	dp += 16;
	}
/*
	memset( W0 , '\0' , sizeof( W0 ) );
	if( m_pSnFhyo52Data->Sn_5FSZsw&0xff ){//ï°êîéñã∆ÇÃèÍçáÇÃÇ›
		m_Arith.l_print( dp, m_pSnFhyo40Data->Sn_1F4A, FMT15 );
		m_Arith.l_add( W0 , W0 , m_pSnFhyo40Data->Sn_1F4A);
		PF1[i].PITM[j++] = dp;	dp += 16;
		m_Arith.l_print( dp, m_pSnFhyo40Data->Sn_1F4B, FMT15 );
		m_Arith.l_add( W0 , W0 , m_pSnFhyo40Data->Sn_1F4B);
		PF1[i].PITM[j++] = dp;	dp += 16;
		m_Arith.l_print( dp, m_pSnFhyo40Data->Sn_1F4C, FMT15 );
		m_Arith.l_add( W0 , W0 , m_pSnFhyo40Data->Sn_1F4C);
		PF1[i].PITM[j++] = dp;	dp += 16;
		m_Arith.l_print( dp, W0 , FMT15 );
		PF1[i++].PITM[j++] = dp;	dp += 16;
	}
	else{
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZKGA, FMT15 );
		PF1[i].PITM[j++] = dp;	dp += 16;
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZKGB, FMT15 );
		PF1[i].PITM[j++] = dp;	dp += 16;
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZKGC, FMT15 );
		PF1[i].PITM[j++] = dp;	dp += 16;
		m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZTG , FMT15 );
		PF1[i++].PITM[j++] = dp;	dp += 16;
	}
*/
/*-------------*/

	if( j = _xprn( PP, 4, PF1, 0, pDC, 0, 1 ) )	goto POUT52;

	// ì¡ó·åvéZÇìKópÇ≈ÇPéÌóﬁÇÃéñã∆Ç≈ÇVÇTÅìà»è„ÇÃèÍçá
//-- '15.03.10 --
//	if( m_pSnFhyo52Data->Sn_5FSZsw == 1 ) {
//---------------
	if( IsOneOver750 ){
//---------------
		dp = AP;	i = j = 0;
		memset( dp, '\0', size );
		pbclr( 0, 5,  &PF1[i] );

//-- '14.10.08 --
//		if( m_pSnHonpyoData->Sn_UP1 >= 750 ) PF1[ 0 ].PITM[ 0 ] = "Å@";
//		if( m_pSnHonpyoData->Sn_UP2 >= 750 ) PF1[ 0 ].PITM[ 1 ] = "Å@";
//		if( m_pSnHonpyoData->Sn_UP3 >= 750 ) PF1[ 0 ].PITM[ 2 ] = "Å@";
//		if( m_pSnHonpyoData->Sn_UP4 >= 750 ) PF1[ 0 ].PITM[ 3 ] = "Å@";
//		if( m_pSnHonpyoData->Sn_UP5 >= 750 ) PF1[ 0 ].PITM[ 4 ] = "Å@";
//---------------
		if( m_pSnHonpyoData->Sn_UP1 >= 750 ){
			PF1[ 0 ].PITM[ 0 ] = "Å@";
		}
		else if( m_pSnHonpyoData->Sn_UP2 >= 750 ){
			PF1[ 0 ].PITM[ 1 ] = "Å@";
		}
		else if( m_pSnHonpyoData->Sn_UP3 >= 750 ){
			PF1[ 0 ].PITM[ 2 ] = "Å@";
		}
		else if( m_pSnHonpyoData->Sn_UP4 >= 750 ){
			PF1[ 0 ].PITM[ 3 ] = "Å@";
		}
		else if( m_pSnHonpyoData->Sn_UP5 >= 750 ){
			PF1[ 0 ].PITM[ 4 ] = "Å@";
		}
//---------------
		//ÉtÉåÅ[ÉÄéÌÇóLå¯Ç∆Ç∑ÇÈà◊ÇÃëOèàóù
		m_pVprn->SetSfncEnv( SElfnc, 1 );
		if( j = _xprn( PP, 5, PF1, 0, pDC, 0, 1 ) )	goto POUT52;
	}

POUT52:
	free( AP );
	
	return( j );
}

//2015.08.04 INSERT START
//int CH26HyoView::PrintUchiwake( CDC *pDC, int pcnt )
int CH26HyoView::PrintUchiwake( CDC *pDC, BOOL isHikae, int PPno , int pcnt )
{
	int				i=0, j=0;
	unsigned int	size;
	char			*AP, *dp, buf[512], PW0[6];

	CString str = _T("");
	
	BOOL	isNoImage = FALSE;

	if( m_pPrintCmInfo->PRtype&0x80 ){
		isNoImage = FALSE;	// ÉCÉÅÅ[ÉWèoóÕóLÇË
	}
	else{
		isNoImage = TRUE;	// ÉCÉÅÅ[ÉWèoóÕóLÇË
	}
	
	size = 2048;
	if( (AP = new char[size]) == NULL ){
		return -1;
	}

	PBUF	PF1[30];
	//int		PP=27;
	int		PP=PPno;

	memset( PW0, '\0', 6 );
	m_Arith.l_input( PW0, _T("1000") );

	dp = AP;
	memset( dp, '\0', size );

	char	yymmdd[3] = {0};

	// î[ê≈ín
	if( !m_pPrintCmInfo->OP3 ){	
		// óXï÷î‘çÜ
		if( j = (int)strlen( (char *)m_pSnHeadData->Sn_ZIP ) ){	
			strcpy_s( dp, 2+1, "Åß" );
			memmove( (dp+2), m_pSnHeadData->Sn_ZIP, j );
			PF1[i++].PITM[0] = dp;
			dp += (j + 3);
		}
		else{
			PF1[i++].PITM[0] = (char *)0;
		}

		// î[ê≈ínÅiÇQçsÅj
		memmove( dp, &m_pSnHeadData->Sn_NOZEI[0],  20*2 );				
		PF1[i].PITM[0] = dp;
		dp += 41;
		memmove( dp, &m_pSnHeadData->Sn_NOZEI[40], 20*2 );
		PF1[i++].PITM[1] = dp;
		dp += 41;
	}
	else{
		pbclr( 0, 1, &PF1[i++] );
		pbclr( 0, 2, &PF1[i++] );
	}

	// î[ê≈ínìdòbî‘çÜ
	if( !m_pPrintCmInfo->OPE ){
		memset( buf, '\0', sizeof( buf ) );
		// ìdòbî‘çÜ
		memmove( buf, m_pSnHeadData->Sn_TELNO1, 6 );
		str.Format( _T("%s") , buf );
		str.TrimRight();
		memset( buf, '\0', sizeof( buf ) );
		memmove( buf , str , str.GetLength());
		j = (int)strlen( buf );
		memmove( dp, "      ", 6 );
		memmove( (dp+(6-j)), buf, 6 );
		PF1[i].PITM[0] = dp;
		dp += 7;

		memset( buf, '\0', sizeof( buf ) );
		memmove( buf, m_pSnHeadData->Sn_TELNO2, 4 );
		str.Format( _T("%s") , buf );
		str.TrimRight();
		memset( buf, '\0', sizeof( buf ) );
		memmove( buf , str , str.GetLength());
		memmove( dp , buf, sizeof( buf ) );

		PF1[i].PITM[1] = dp;	dp += 5;

		memset( buf, '\0', sizeof( buf ) );
		memmove( buf, m_pSnHeadData->Sn_TELNO3, 4 );
		str.Format( _T("%s") , buf );
		str.TrimRight();
		memset( buf, '\0', sizeof( buf ) );
		memmove( buf , str , str.GetLength());
		memmove( dp , buf, sizeof( buf ) );
		PF1[i++].PITM[2] = dp;
		dp += 5;
	}
	else{
		pbclr( 0, 3, &PF1[i++] );
	}

	// éÅñºñîÇÕâÆçÜ	
	if( !m_pPrintCmInfo->OPF ){	
		// ñ@êlñºÅiêUÇËÇ™Ç»Åj
//2016.10.20 UPDATE START
		//memmove( dp, m_pSnHeadData->Sn_COFRI, 68 );	
		//PF1[i++].PITM[0] = dp;
		//dp += 69;
		memmove( dp, m_pSnHeadData->Sn_COFRI, 80 );	
		PF1[i++].PITM[0] = dp;
		dp += 81;
//2016.10.20 UPDATE END
	}
	else{
		pbclr( 0, 1, &PF1[i++] );
	}

	// ñ@êlñº
	if( !m_pPrintCmInfo->OP4 && !m_pPrintCmInfo->OPM ){
		memmove( dp, m_pSnHeadData->Sn_CONAM, 20*2 );
		PF1[i++].PITM[0] = dp;
		dp += 41;
	}
	else{
		pbclr( 0, 1, &PF1[i++] );
	}

	// ë„ï\é“ÅiêUÇËÇ™Ç»Åj
	if( !m_pPrintCmInfo->OPG ){
		memmove( dp, m_pSnHeadData->Sn_DIFRI, 68 );
		PF1[i++].PITM[0] = dp;	
		dp += 69;
	}
	else{
		pbclr( 0, 1, &PF1[i++] );
	}

	// ë„ï\é“ñº
	if( !m_pPrintCmInfo->OP5 ){
		memmove( dp, m_pSnHeadData->Sn_DIHYO, 20*2 );
		PF1[i++].PITM[0] = dp;
		dp += 41;

	}
	else{
		pbclr( 0, 1, &PF1[i++] );
	}

int CCSw = 0;

	// êÆóùî‘çÜìôèoóÕÉTÉCÉì
	if( (m_pPrintCmInfo->PRtype&0x80) || 
		((m_pPrintCmInfo->OP1==0) ||(IsNotPrintGengoCircle()==false)) ){

		int gengo1 = 0, gengo2 = 0;
		if( !(m_pPrintCmInfo->PRtype&0x80) && m_pPrintCmInfo->OP1 ){
			pbclr(0, 1, &PF1[i++]);
			pbclr(0, 6, &PF1[i]);
			if( IsSwitch_HeiseiReiwa() && (IsNotPrintGengoCircle()==false) ){
				// ê\çêä˙ä‘é©
				memset(yymmdd, '\0', sizeof(yymmdd));
				m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1);
				// ê\çêä˙ä‘éä
				memset(yymmdd, '\0', sizeof(yymmdd));
				m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo2);
			}
		}
		else{
			if( (m_pSnHeadData->Sn_SEIRI[0]&0xff) != 0xff ){
				memset(buf, '\0', sizeof(buf));
				m_Arith.l_unpac(buf, m_pSnHeadData->Sn_SEIRI, 8);
				for( j=(int)strlen(buf); j>0; ){
					if( buf[--j] != 0x20 ){
						break;
					}
				}
				++j;
				if( j < 8 ){
					memmove(dp, "        ", 8-j);
				}
				// êÆóùî‘çÜ
				memmove((dp+(8-j)), buf, j);
				PF1[i++].PITM[0] = dp;
				dp += 9;
			}
			else{
				PF1[i++].PITM[0] = (char *)0;
			}

			if( CCSw ){	// ã≠êßä˙ä‘ïœçX
				for( j=0; j<3; ++j, dp+=3 ){	// ê\çêä˙ä‘é©
					PF1[i].PITM[j] = dp;
				}
				for( ; j<6; ++j, dp += 3 ){		// ê\çêä˙ä‘éä
					PF1[i].PITM[j] = dp;
				}
			}
			else{
				// ê\çêä˙ä‘é©
				memset(yymmdd, '\0', sizeof(yymmdd));
				if( IsJcContinuePrint(true) ){
					m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
				}
				else{
					//m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
					m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1);
				}
				for( j = 0; j < 3; ++j, dp+=3 ){
					wsprintf(dp, _T("%02X"), yymmdd[j]&0xff);
					PF1[i].PITM[j] = dp;
				}
				// ê\çêä˙ä‘éä
				memset(yymmdd, '\0', sizeof(yymmdd));
				if( IsJcContinuePrint(true) ){
					m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
				}
				else{
					//m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
					m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo2);
				}
				for( ; j<6; ++j, dp+=3 ){
					wsprintf(dp, _T("%02X"), yymmdd[j-3]&0xff);
					PF1[i].PITM[j] = dp;
				}
			}
		}

		if( IsSwitch_HeiseiReiwa() ){
			if( IsNotPrintGengoCircle() ){
				pbclr(0, 2, &PF1[++i]);
				pbclr(0, 2, &PF1[++i]);
			}
			else{
				if( CCSw ){	// ã≠êßä˙ä‘ïœçX
					i++;
					PF1[i].PITM[0] = (char *)0;
					PF1[i].PITM[1] = (char *)0;
					i++;
					PF1[i].PITM[0] = (char *)0;
					PF1[i].PITM[1] = (char *)0;
				}
				if( IsNotPrintGengoCircle() ){
					gengo1 = -1;
					gengo2 = -1;
				}
				else{
					i++;
					if( gengo1 == ID_ICSSH_REIWA_GENGO ) {
						memmove(dp, "    ", 4);
						PF1[i].PITM[0] = (char *)0;
						PF1[i].PITM[1] = dp;
						dp += 5;
					}
					else if( gengo1 == ID_ICSSH_HEISEI_GENGO ) {
						memmove(dp, "    ", 4);
						PF1[i].PITM[0] = dp;
						PF1[i].PITM[1] = (char *)0;
						dp += 5;
					}
					else{
						PF1[i].PITM[0] = (char *)0;
						PF1[i].PITM[1] = (char *)0;
					}

					i++;
					if( gengo2 == ID_ICSSH_REIWA_GENGO ) {
						memmove(dp, "    ", 4);
						PF1[i].PITM[0] = (char *)0;
						PF1[i].PITM[1] = dp;
						dp += 5;
					}
					else if( gengo2 == ID_ICSSH_HEISEI_GENGO ) {
						memmove(dp, "    ", 4);
						PF1[i].PITM[0] = dp;
						PF1[i].PITM[1] = (char *)0;
						dp += 5;
					}
					else{
						PF1[i].PITM[0] = (char *)0;
						PF1[i].PITM[1] = (char *)0;
					}
				}
			}
		}
	}
	else{
		pbclr( 0, 1, &PF1[i++] );
		pbclr( 0, 6, &PF1[i] );
		if(IsSwitch_HeiseiReiwa()){
			pbclr(0, 2, &PF1[++i]);
			pbclr(0, 2, &PF1[++i]);
		}
	}

	switch( m_pSnHeadData->Sn_SKKBN ){
		case 1 :
//-- '15.11.18 --
//			PF1[++i].PITM[0] = _T("Å@ämíËÅ@");
//---------------
			if( !(m_pPrintCmInfo->PRtype&0x80) ){
				// ÇnÇbÇqópéÜ
				if( m_pPrintCmInfo->OPC ){
					PF1[++i].PITM[0] = (char *)0;
				}
				else{
					PF1[++i].PITM[0] = _T("Å@ämíËÅ@");
				}
			}
			else{
				PF1[++i].PITM[0] = _T("Å@ämíËÅ@");
			}
//---------------
			break;
		case 2 :
			PF1[++i].PITM[0] = _T("Å@íÜä‘Å@");
			break;
		case 3 :
			PF1[++i].PITM[0] = _T("èCê≥ämíË");
			break;
		case 4 :
			PF1[++i].PITM[0] = _T("èCê≥íÜä‘");
			break;
	}

	// êÆóùî‘çÜìôèoóÕÉTÉCÉì
	if( (m_pPrintCmInfo->PRtype&0x80) ||
		(m_pPrintCmInfo->OP1==0) || (IsNotPrintGengoCircle()==false) ){

		int gengo1 = 0, gengo2 = 0;
		//íÜä‘ê\çêä˙ä‘é©•éä
		switch( m_pSnHeadData->Sn_SKKBN ){
			case 1 : 
			case 3 :
				pbclr( 0, 6, &PF1[++i] );

				if(IsSwitch_HeiseiReiwa()){
					pbclr(0, 2, &PF1[++i]);
					pbclr(0, 2, &PF1[++i]);
				}

				break;
			default:

				if( !(m_pPrintCmInfo->PRtype&0x80) && m_pPrintCmInfo->OP1 ){
					pbclr(0, 6, &PF1[++i]);
					if( IsSwitch_HeiseiReiwa() && (IsNotPrintGengoCircle()==false) ){
						memset(yymmdd, '\0', sizeof(yymmdd));
						m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1);
						memset(yymmdd, '\0', sizeof(yymmdd));
						m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo2);
					}
				}
				else{
					if( CCSw ){	// ã≠êßä˙ä‘ïœçXÅ@02'01.21
						for( ++i, j=0; j<3; ++j, dp+=3 ){
							PF1[i].PITM[j] = dp;
						}
						for( ; j<6; ++j, dp+=3 ){
							PF1[i].PITM[j] = dp;
						}
					}
					else{
						memset(yymmdd, '\0', sizeof(yymmdd));
						if( IsJcContinuePrint(true) ){
							m_pSnHeadData->GetYmdData(ID_ICSSH_CHUUKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
						}
						else{
							m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1);
						}
						for( ++i, j=0; j<3; ++j, dp += 3 ){
							wsprintf(dp, _T("%02X"), yymmdd[j]&0xff);
							PF1[i].PITM[j] = dp;
						}
						memset(yymmdd, '\0', sizeof(yymmdd));
						if( IsJcContinuePrint(true) ){
							m_pSnHeadData->GetYmdData(ID_ICSSH_CHUUKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
						}
						else{
							m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo2);
						}
						for( ; j<6; ++j, dp += 3 ){
							wsprintf(dp, _T("%02X"), yymmdd[j-3]&0xff);
							PF1[i].PITM[j] = dp;
						}
					}
				}

				if( IsSwitch_HeiseiReiwa() ){
					if( IsNotPrintGengoCircle() ){
						pbclr(0, 2, &PF1[++i]);
						pbclr(0, 2, &PF1[++i]);
					}
					else{
						if( CCSw ){	// ã≠êßä˙ä‘ïœçXÅ@02'01.21
							i++;
							PF1[i].PITM[0] = (char *)0;
							PF1[i].PITM[1] = (char *)0;
							i++;
							PF1[i].PITM[0] = (char *)0;
							PF1[i].PITM[1] = (char *)0;
						}
						else{
							if( IsNotPrintGengoCircle() ){
								gengo1 = -1;
								gengo2 = -1;
							}

							i++;
							if( gengo1 == ID_ICSSH_REIWA_GENGO ) {
								memmove(dp, "    ", 4);
								PF1[i].PITM[0] = (char *)0;
								PF1[i].PITM[1] = dp;
								dp += 5;
							}
							else if( gengo1 == ID_ICSSH_HEISEI_GENGO ) {
								memmove(dp, "    ", 4);
								PF1[i].PITM[0] = dp;
								PF1[i].PITM[1] = (char *)0;
								dp += 5;
							}
							else{
								PF1[i].PITM[0] = (char *)0;
								PF1[i].PITM[1] = (char *)0;
							}

							i++;
							if( gengo2 == ID_ICSSH_REIWA_GENGO ) {
								memmove(dp, "    ", 4);
								PF1[i].PITM[0] = (char *)0;
								PF1[i].PITM[1] = dp;
								dp += 5;
							}
							else if( gengo2 == ID_ICSSH_HEISEI_GENGO ) {
								memmove(dp, "    ", 4);
								PF1[i].PITM[0] = dp;
								PF1[i].PITM[1] = (char *)0;
								dp += 5;
							}
							else{
								PF1[i].PITM[0] = (char *)0;
								PF1[i].PITM[1] = (char *)0;
							}
						}
					}
				}
				break;
		}
	}
	else{
		pbclr( 0, 6, &PF1[++i] );
		if(IsSwitch_HeiseiReiwa()){
			pbclr(0, 2, &PF1[++i]);
			pbclr(0, 2, &PF1[++i]);
		}
	}

//	if( j = _xprn( PP, 1, PF1, 0, pDC, isNoImage, SEno ) ){
	if( j = _xprn( PP, 1, PF1, 0, pDC, isNoImage, 1 ) ){
		goto POUT1;
	}

	dp = AP;
	i = j = 0;
	memset( dp, '\0', size );

	char kingaku[MONY_BUF_SIZE] = {0};

	//á@â€ê≈ïWèÄäz
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		m_Arith.l_add( kingaku , ((CH28SnFhyo10Data *)m_pSnFhyo10Data)->Sn_1F1A , ((CH28SnFhyo10Data *)m_pSnFhyo10Data)->Sn_1F1B );
	}
	m_Arith.l_add( kingaku, kingaku, ((CH28SnFhyo10Data *)m_pSnFhyo10Data)->Sn_1F1C );
	m_Arith.l_div( kingaku, kingaku, PW0 );
	m_Arith.l_print( dp, kingaku, FMT011EX );

	PF1[i].PITM[j] = dp;
	dp+=17;

	i++;
	j = 0;
	//áAâ€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		m_Arith.l_add( kingaku , ((CH28SnFhyo10Data *)m_pSnFhyo10Data)->Sn_1F1_1A , ((CH28SnFhyo10Data *)m_pSnFhyo10Data)->Sn_1F1_1B );
	}
	m_Arith.l_add( kingaku , kingaku , ((CH28SnFhyo10Data *)m_pSnFhyo10Data)->Sn_1F1_1C );
	m_Arith.l_print( dp, kingaku, FMT13EX );

	PF1[i].PITM[j] = dp;
	j++;
	dp+=17;

	//áBì¡íËâ€ê≈édì¸ÇÍÇ…åWÇÈéxï•ëŒâøÇÃäz
	memset( kingaku , '\0' , MONY_BUF_SIZE);
//	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
//		m_Arith.l_add( kingaku , (*m_pSnFhyo10Data)->Sn_1F1_2A , (*m_pSnFhyo10Data)->Sn_1F1_2B );
//	}
	m_Arith.l_add( kingaku , kingaku , ((CH28SnFhyo10Data *)m_pSnFhyo10Data)->Sn_1F1_2C );
	m_Arith.l_print( dp, kingaku, FMT13EX );

	PF1[i].PITM[j] = dp;
	j++;
	dp+=17;

	//áCï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		m_Arith.l_add( kingaku , ((CH28SnFhyo10Data *)m_pSnFhyo10Data)->Sn_1F5A , ((CH28SnFhyo10Data *)m_pSnFhyo10Data)->Sn_1F5B );
	}
	m_Arith.l_add( kingaku , kingaku , ((CH28SnFhyo10Data *)m_pSnFhyo10Data)->Sn_1F5C );
	m_Arith.l_print( dp, kingaku, FMT13EX );

	PF1[i].PITM[j] = dp;
	j++;
	dp+=17;

	//áDîÑè„Ç∞ÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		m_Arith.l_add( kingaku , ((CH28SnFhyo10Data *)m_pSnFhyo10Data)->Sn_1F5_1A, ((CH28SnFhyo10Data *)m_pSnFhyo10Data)->Sn_1F5_1B );
	}
	m_Arith.l_add( kingaku , kingaku , ((CH28SnFhyo10Data *)m_pSnFhyo10Data)->Sn_1F5_1C );
	m_Arith.l_print( dp, kingaku, FMT13EX );

	PF1[i].PITM[j] = dp;
	j++;
	dp+=17;

	//áEì¡íËâ€ê≈édì¸ÇÍÇÃï‘ä“ìôëŒâøÇ…åWÇÈê≈äz
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	//if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
	//	m_Arith.l_add( kingaku , ((CH28SnFhyo10Data *)m_pSnFhyo10Data)->Sn_1F5_2A, ((CH28SnFhyo10Data *)m_pSnFhyo10Data)->Sn_1F5_2B );
	//}
	m_Arith.l_add( kingaku , kingaku , ((CH28SnFhyo10Data *)m_pSnFhyo10Data)->Sn_1F5_2C );
	m_Arith.l_print( dp, kingaku, FMT13EX );

	PF1[i].PITM[j] = dp;
	j++;
	dp+=17;

	i++;
	j = 0;
	//â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäzÅ@ÇRÅìï™
	m_Arith.l_print( dp, ((CH28SnFhyo10Data *)m_pSnFhyo10Data)->Sn_1F1_1A, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäzÅ@ÇSÅìï™
	m_Arith.l_print( dp, ((CH28SnFhyo10Data *)m_pSnFhyo10Data)->Sn_1F1_1B, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//â€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäzÅ@ÇUÅDÇRÅìï™
	m_Arith.l_print( dp, ((CH28SnFhyo10Data *)m_pSnFhyo10Data)->Sn_1F1_1C, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	i++;
	j= 0;
	//ì¡íËâ€ê≈édì¸ÇÍÇ…åWÇÈéxï•ëŒâøÇÃäzÅ@ÇRÅìï™
	PF1[i].PITM[j++] = (char *)0;
	dp += 19;
									  
	//ì¡íËâ€ê≈édì¸ÇÍÇ…åWÇÈéxï•ëŒâøÇÃäzÅ@ÇSÅìï™
	PF1[i].PITM[j++] = (char *)0;
	dp += 19;
									  
	//ì¡íËâ€ê≈édì¸ÇÍÇ…åWÇÈéxï•ëŒâøÇÃäzÅ@ÇUÅDÇRÅìï™
	m_Arith.l_print( dp, ((CH28SnFhyo10Data *)m_pSnFhyo10Data)->Sn_1F1_2C, FMT16_EX );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	i++;
	j= 0;
	//çáåvÅ@ÇRÅìï™
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	m_Arith.l_div( kingaku, ((CH28SnFhyo10Data *)m_pSnFhyo10Data)->Sn_1F1_1A, PW0 );
	m_Arith.l_print( dp, kingaku, FMT14 );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//çáåvÅ@ÇSÅìï™
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	m_Arith.l_div( kingaku, ((CH28SnFhyo10Data *)m_pSnFhyo10Data)->Sn_1F1_1B, PW0 );
	m_Arith.l_print( dp, kingaku, FMT14 );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//çáåvÅ@ÇUÅDÇRÅìï™
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	m_Arith.l_add( kingaku , ((CH28SnFhyo10Data *)m_pSnFhyo10Data)->Sn_1F1_1C, ((CH28SnFhyo10Data *)m_pSnFhyo10Data)->Sn_1F1_2C );
	m_Arith.l_div( kingaku, kingaku, PW0 );
	m_Arith.l_print( dp, kingaku, FMT14 );
//	m_Arith.l_print( dp, kingaku, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 19;

	//ï∂éöêîÇÃÇÊÇÈÉtÉHÉìÉgí≤êÆ
	int FontFlg = 0;
//	for(int cnt1 = 2; cnt1 < 5; cnt1++){
	for(int cnt1 = 2; cnt1 < 4; cnt1++){
		for(int cnt2 = 0; cnt2 < 3; cnt2++){
			str.Format( _T("%s") ,PF1[cnt1].PITM[cnt2] );
			str.TrimLeft();
			if( str.GetLength() > 14 ){
				FontFlg = 1;
				break;
			}
		}
	}
	for(int cnt2 = 0; cnt2 < 3; cnt2++){
		str.Format( _T("%s") ,PF1[4].PITM[cnt2] );
		str.TrimLeft();
		if( str.GetLength() > 10 ){
			FontFlg = 1;
			break;
		}
	}

	if( FontFlg == 0 ){
		if( j = _xprn( PP, 2, PF1, 0, pDC, isNoImage, 1 ) ){
			goto POUT1;
		}
	}else if( FontFlg == 1 ){
		if( j = _xprn( PP, 3, PF1, 0, pDC, isNoImage, 1 ) ){
			goto POUT1;
		}
	}

	if( m_pPrintCmInfo->OPM ){
		dp = AP;
		i = j = 0;
		memset( dp, '\0', size );
		pbclr( 0, 1, &PF1[ 0 ] );
		pbclr( 0, 1, &PF1[ 1 ] );
	
		// ñ@êlñº
		if( !m_pPrintCmInfo->OP4 ){
			memmove( dp, m_pSnHeadData->Sn_CONAM, 20*2 );
			PF1[0].PITM[0] = dp;
			dp += 41;
		}
		memmove( dp, m_pSnHeadData->Sn_GPNAM, 20*2 );
		PF1[1].PITM[0] = dp;
		dp += 41;
		if( j = _xprn( PP, 4, PF1, 0, pDC, 0, 1 ) ){
			goto POUT1;
		}
	}

//--> '15.11.30 INS START
   // çTóp
   if( isHikae && m_pPrintCmInfo->OPD ){
		int yPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[14].HyoTn]._PIVad;
		int xPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[14].HyoTn]._PIHad;
		long	PH  = 2540L / 10L ;		// 1 character
		long	PV1 = 2540L / 6L;		// 1/6 LF
  		long YPosition = (PV1*2)+(PV1/2) + yPlus;
		long XPosition = (PH*56L)+(PH/3) + xPlus;
		int	Gothic108 = m_pVprn->RegisterFont( MAKELONG( 108, 1 ), 0, 0, "ÇlÇrÅ@ÉSÉVÉbÉN" );
		struct	_SF_PAR		SfPar;
		memset( (char *)&SfPar, '\0', sizeof( struct _SF_PAR ) );
		SfPar.sf_fcsg =	( SFp_xl | SFp_yb | SFp_yc );			// ÉtÉ@ÉìÉNÉVÉáÉìÉTÉCÉì
		SfPar.sf_hvsg = 0;										// èoóÕï˚å¸
		SfPar.sf_xfld = (8L * PH);								// â°èoóÕóÃàÊïù
		SfPar.sf_yfld =	PV1;									// ècèoóÕóÃàÊïù
		SfPar.sf_mrgn[0] = 0;									// óÃàÊì‡É}Å[ÉWÉìÅ@ç∂
		SfPar.sf_mrgn[1] = 10;									// Å@Å@ÅVÅ@Å@Å@Å@Å@âE
		SfPar.sf_mrgn[2] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@è„
		SfPar.sf_mrgn[3] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@â∫
		SfPar.sf_kpit = 0;										// ï∂éöÉsÉbÉ`ïùÅ@X0.1Œﬂ≤›ƒ
		SfPar.sf_ksiz =	Gothic108;								// ï∂éöÉTÉCÉYÅ@Å@X0.1Œﬂ≤›ƒ		
		SfPar.sf_erto = 0;										// ï∂éöägëÂèkè¨ó¶
		if( m_pVprn->DrawString( 0, XPosition, YPosition, _T("ÅyçTópÅz"), &SfPar ) ){
			goto POUT1;
		}

		m_pVprn->DeleteRegFont( Gothic108 );
	}
//<-- '15.11.30 INS END

POUT1:
	delete [] AP;
	
	return j;
}
//2015.08.04 INSERT END

//-----------------------------------------------------------------------------
// çXê≥ÇÃêøãÅÅ@å¬êlóp 
//-----------------------------------------------------------------------------
int CH26HyoView::data_kjn( CDC* pDC )
{
	int		dp, st;
	
	char	ZHnam[128]={0};		// ê≈óùémñ@êlñº
	char	ZName[128]={0};		// ê≈óùéméÅñº
	memmove( ZHnam, m_pShinInfo->pO_DATA->o_ofcnam, sizeof(m_pShinInfo->pO_DATA->o_ofcnam) );
	memmove( ZName, m_pShinInfo->pZ_DATA->z_zeinam, sizeof(m_pShinInfo->pZ_DATA->z_zeinam) );

	BOOL	isNoImage = FALSE;
	if( m_pPrintCmInfo->PRtype&0x80 ){
		isNoImage = FALSE;	// ÉCÉÅÅ[ÉWèoóÕÅ@óL
	}
	else{
		isNoImage = TRUE;	// Å@Å@ÅVÅ@Å@Å@Å@ñ≥
	}

	char	*BP=NULL, *BP1=NULL;

	if( (BP = new char[KSK_BUF_SIZE]) == NULL ){
		return -1;
	}

	BP1 = BP;
	l_defn( 0x16 );
	st = 0;

	//int Pno = 16;
	int Pno = 0;
	if ( ( st = GetGengoNo_Ksk() ) == -1 ) {
		return -1;
	}
		
	if ( st == ID_ICSSH_REIWA_GENGO ) {
		Pno = 26;
	}
	else {
		Pno = 25;
	}

	for( dp = 1; dp <= 6; ++dp ) {	
		switch( dp ) {
			case	1:	st = kjn_dprn1( dp, pDC, BP1, isNoImage );		break;
			case	2:	st = kjn_dprn2( dp, pDC, BP1, isNoImage );		break;
			case	3:	st = kjn_dprn3( dp, pDC, BP1, isNoImage );		break;
			case	4:	st = kjn_dprn4( dp, pDC, BP1, isNoImage );		break;
			case	5:	st = kjn_dprn5( dp, pDC, BP1, isNoImage );		break;
			case	6:	st = kjn_dprn6( dp, pDC, BP1, isNoImage );		break;
		}
		if( st )	break;
	}

	delete [] BP;

//Åö//[12'08.10]///ìdéqê\çêäÆóπçœ 
	if( *m_pShinInfo->pFinish && *m_pShinInfo->pFinishSw && strlen( m_pShinInfo->pFinishPrn)){
		int yPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[3].HyoTn]._PIVad;
		int xPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[3].HyoTn]._PIHad;

		long	PH  = 2540L / 10L;		// 1 character
		long	PV1 = 2540L / 6L;		// 1/6 LF
  		long YPosition = ( PV1 * 2L ) + yPlus;
//		long XPosition = ( PH *  17L );
		long XPosition = ( PH *  9L )+ xPlus;
		int	Gothic095 = m_pVprn->RegisterFont( 90, 0, 0, _T("ÇlÇrÅ@ÉSÉVÉbÉN") );
		int	Gothic108 = m_pVprn->RegisterFont( MAKELONG( 108, 1 ), 0, 0, _T("ÇlÇrÅ@ÉSÉVÉbÉN") );
		struct	_SF_PAR		SfPar;
		memset( (char *)&SfPar, '\0', sizeof( struct _SF_PAR ) );
		SfPar.sf_fcsg =	( SFp_xl | SFp_yb );					// ÉtÉ@ÉìÉNÉVÉáÉìÉTÉCÉì
		SfPar.sf_hvsg = 0;										// èoóÕï˚å¸
		SfPar.sf_xfld = (11L * PH);								// â°èoóÕóÃàÊïù
		SfPar.sf_yfld =	PV1;									// ècèoóÕóÃàÊïù
		SfPar.sf_mrgn[0] = 0;									// óÃàÊì‡É}Å[ÉWÉìÅ@ç∂
		SfPar.sf_mrgn[1] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@âE
		SfPar.sf_mrgn[2] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@è„
		SfPar.sf_mrgn[3] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@â∫
		SfPar.sf_kpit = 0;										// ï∂éöÉsÉbÉ`ïùÅ@X0.1Œﬂ≤›ƒ
		SfPar.sf_ksiz =	Gothic108;								// ï∂éöÉTÉCÉYÅ@Å@X0.1Œﬂ≤›ƒ		
		SfPar.sf_erto = 0;										// ï∂éöägëÂèkè¨ó¶
		if( m_pVprn->DrawString( 0, XPosition, YPosition, _T("ìdéqê\çêçœ"), &SfPar ) )	return( 1 );

  		YPosition = ( PV1 * 2L ) + yPlus;
//		XPosition = ( PH *  25L );
		XPosition = ( PH *  17L ) + xPlus;
		memset( (char *)&SfPar, '\0', sizeof( struct _SF_PAR ) );
		SfPar.sf_fcsg =	( SFp_xl | SFp_yb );					// ÉtÉ@ÉìÉNÉVÉáÉìÉTÉCÉì
		SfPar.sf_hvsg = 0;										// èoóÕï˚å¸
		SfPar.sf_xfld = (35L * PH);								// â°èoóÕóÃàÊïù
		SfPar.sf_yfld =	PV1;									// ècèoóÕóÃàÊïù
		SfPar.sf_mrgn[0] = 0;									// óÃàÊì‡É}Å[ÉWÉìÅ@ç∂
		SfPar.sf_mrgn[1] = 10;									// Å@Å@ÅVÅ@Å@Å@Å@Å@âE
		SfPar.sf_mrgn[2] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@è„
		SfPar.sf_mrgn[3] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@â∫
		SfPar.sf_kpit = 0;										// ï∂éöÉsÉbÉ`ïùÅ@X0.1Œﬂ≤›ƒ
		SfPar.sf_ksiz =	Gothic095;								// ï∂éöÉTÉCÉYÅ@Å@X0.1Œﬂ≤›ƒ		
		SfPar.sf_erto = 0;										// ï∂éöägëÂèkè¨ó¶
		if( m_pVprn->DrawString( 0, XPosition, YPosition, m_pShinInfo->pFinishPrn, &SfPar ) ){
			return( 1 );
		}

		m_pVprn->DeleteRegFont( Gothic095 );
		m_pVprn->DeleteRegFont( Gothic108 );

   }
///////////////////

	return( st );
}

//-----------------------------------------------------------------------------
// àÛç¸ä÷êî
//-----------------------------------------------------------------------------
int CH26HyoView::kjn_dprn1( int dp, CDC* pDC, char *BP, BOOL isNoImage )
{
	int		i, j, sw;
	char	buf[256];
	int		len;

	PBUF	PF[30];
	memset( PF, '\0', sizeof(PF) );
	//int Pno = 16;
	int Pno = 0;
	int st = 0;
	if ( ( st = GetGengoNo_Ksk() ) == -1 ) {
		return -1;
	}

	if ( st == ID_ICSSH_REIWA_GENGO ) {
		Pno = 26;
	}
	else {
		Pno = 25;
	}

	i = 0;
	memset( BP, '\0', KSK_BUF_SIZE );
	char *BP1 = BP;

	// ê≈ñ±èêñº
	::ZeroMemory( buf, sizeof( buf ));
	m_Util.LineSpaceSet( (char*)m_pSnHeadData->Sn_ZMSYO, buf, 12 );
	memmove( BP1, buf, 12 );
	PF[i++].PITM[0] = BP1;
	BP1 += ( 12+1 );

	if( !m_pPrintCmInfo->OP3 ){	
		// óXï÷î‘çÜ
		::ZeroMemory( buf, sizeof( buf ));
		m_Util.LineSpaceSet( (char*)m_pSnHeadData->Sn_ZIP, buf, 10 );
		memmove( BP1, buf, 10 );
		PF[i++].PITM[0] = BP1;	BP1 += ( 10+1 );

		// î[ê≈ín
		for( j = 0; j < 2; j++ ){
			::ZeroMemory( buf, sizeof( buf ));
			m_Util.LineSpaceSet( (char*)&m_pSnHeadData->Sn_NOZEI[40*j], buf, 40 );
			memmove( BP1, buf, 40 );
			PF[i].PITM[j] = BP1;	BP1 += ( 40+1 );
		}
	}
	else{
		PF[i++].PITM[0] = (char *)0;
		PF[i].PITM[0] = (char *)0;
		PF[i].PITM[1] = (char *)0;
	}
	i++;

	// î[ê≈ínìdòbî‘çÜ
	if( !m_pPrintCmInfo->OPE ){
		::ZeroMemory( buf, sizeof( buf ));
		TelBufSet( buf, 1 );
		len = (int)strlen( buf );
		memmove( BP1, buf, len );
		PF[i++].PITM[0] = BP1;	BP1 += ( len+1 );
	}
	else{
		PF[i++].PITM[0] = (char *)0;
	}

	// ë„ï\é“ÅiêUÇËÇ™Ç»Åj
	if( !m_pPrintCmInfo->OPG ){
		::ZeroMemory( buf, sizeof( buf ));
		m_Util.LineSpaceSet( (char*)m_pSnHeadData->Sn_DIFRI, buf, 80 );
		memmove( BP1, buf, 80 );
		PF[i++].PITM[0] = BP1;	BP1 += ( 80+1 );
	}
	else{
		PF[i++].PITM[0] = (char *)0;
	}

	// ë„ï\é“ñº
	if( !m_pPrintCmInfo->OP5 ){
		::ZeroMemory( buf, sizeof( buf ));
		m_Util.LineSpaceSet( (char*)m_pSnHeadData->Sn_DIHYO, buf, 40 );
		memmove( BP1, buf, 40 );
		PF[i++].PITM[0] = BP1;	BP1 += ( 40+1 );
	}
	else{
		PF[i++].PITM[0] = (char *)0;
	}

//2015.10.20 INSERT START
//2015.12.03 UPDATE START
	////å¬êlî‘çÜ
	//BOOL myno_flg = FALSE;
	//if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){ //å¥ë•
	//	if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
	//		myno_flg = TRUE;
	//	}
	//}else{ //ä»à’
	//	if( m_ShRevType >= ID_ICSSH_MYNUMBER_REVTYPE ){
	//		myno_flg = TRUE;
	//	}
	//}

	//if( myno_flg == TRUE ){
	//	CString myno_str;
	//	if( m_pSnHeadData->GetMyNumber( myno_str ) == 0 ){
	//		memmove( BP1, myno_str, 12 );
	//		PF[i++].PITM[0] = BP1; BP1 += ( 12+1 );
	//	}else{
	//		pbclr( 0, 1, &PF[i++] );
	//	}
	//}

//2016.02.22 UPDATE START
	//int myno_sgn = 0;
	//if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){ //å¥ë•
	//	if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
	//		if( m_ShRevType >= ID_ICSSH_MYNUMBER_REVTYPE ){
	//			myno_sgn = 2; //É}ÉCÉiÉìÉoÅ[Å@èoóÕóLÅ@ÉåÉCÉAÉEÉgëŒâû
	//		}else{
	//			myno_sgn = 1; //É}ÉCÉiÉìÉoÅ[Å@èoóÕñ≥Å@ÉåÉCÉAÉEÉgëŒâû
	//		}
	//	}else{
	//		myno_sgn = 0; //É}ÉCÉiÉìÉoÅ[Å@èoóÕñ≥Å@ÉåÉCÉAÉEÉgîÒëŒâû
	//	}
	//}else{ //ä»à’
	//	if( m_ShRevType >= ID_ICSSH_MYNUMBER_REVTYPE ){
	//		myno_sgn = 2; //É}ÉCÉiÉìÉoÅ[Å@èoóÕóLÅ@ÉåÉCÉAÉEÉgëŒâû
	//	}else{
	//		myno_sgn = 0; //É}ÉCÉiÉìÉoÅ[Å@èoóÕñ≥Å@ÉåÉCÉAÉEÉgîÒëŒâû
	//	}
	//}

	//CICSMNSub8 mnsub;
	//CString myno_str,myno_prn;

	//switch( myno_sgn ){
	//	case 0:
	//		break;
	//	case 1:
	//		pbclr( 0, 1, &PF[i++] );
	//		break;
	//	case 2:
	//		if( m_pSnHeadData->GetMyNumber( myno_str ) == 0 ){
	//			if( m_pPset->psSigCoprn&0x4000){
	//				myno_prn = mnsub.MakePrintKojinNo( myno_str,  m_pVprn->Get_KojinPrintFlg() );
	//			}else{
	//				myno_prn = myno_str;
	//			}
	//			memmove( BP1, myno_prn, 12 );
	//			PF[i++].PITM[0] = BP1; BP1 += ( 12+1 );
	//		}else{
	//			pbclr( 0, 1, &PF[i++] );
	//		}
	//		break;
	//	default:
	//		break;
	//}

	CICSMNSub8 mnsub;
	CString myno_str,myno_prn;

	if( m_pSnHeadData->GetMyNumber( myno_str ) == 0 ){
		if( m_pPset->psSigCoprn&0x4000){
			myno_prn = mnsub.MakePrintKojinNo( myno_str,  m_pVprn->Get_KojinPrintFlg() );
		}else{
			myno_prn = myno_str;
		}
		memmove( BP1, myno_prn, 12 );
		PF[i++].PITM[0] = BP1; BP1 += ( 12+1 );
	}else{
		pbclr( 0, 1, &PF[i++] );
	}
//2016.02.22 UPDATE END

//2015.12.03 UPDATE END
//2015.10.20 INSERT END
	// â€ê≈ä˙ä‘
	int gengo1 = 0, gengo2 = 0;
	char	yymmdd[3]={0};
	//if( IsJcContinuePrint(false) ){
	//	m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	//}
	//else{
		m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1);
	//}
	for( j = 0; j < 3; j++, BP1 += 3 ) {
		sw = 0;
		if( j == 0 ){
			if( yymmdd[j] ){
				sw = 1;
			}
			else{
				if( yymmdd[j+1] ){
					sw = 1;
				}
			}
		}
		else{
			if( yymmdd[j] ){
				sw = 1;
			}
		}
		if( sw ){
			sprintf_s( BP1, 3, "%2x", yymmdd[j] );
		}
		else{
			memset( BP1, '\0', 2 );
		}
		PF[i].PITM[j] = BP1;
	}

	i++;
	memset( yymmdd, '\0', sizeof(yymmdd) );
	//if( IsJcContinuePrint(false) ){
	//	m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	//}
	//else{
		m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo2);
	//}
	for( j = 0; j < 3; j++, BP1 += 3 ) {
		sw = 0;
		if( j == 0 ){
			if( yymmdd[j] ){
				sw = 1;
			}
			else{
				if( yymmdd[j+1] ){
					sw = 1;
				}
			}
		}
		else{
			if( yymmdd[j] ){
				sw = 1;
			}
		}
		if( sw ){
			sprintf_s( BP1, 3, "%2x", yymmdd[j] );
		}
		else{
			memset( BP1, '\0', 2 );
		}
		PF[i].PITM[j] = BP1;
	}
	i++;

	//if (IsSwitch_HeiseiReiwa()) {
	if (IsSwitch_HeiseiReiwa() || (st == ID_ICSSH_REIWA_GENGO)) {
		if (gengo1 == ID_ICSSH_REIWA_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = (char *)0;
			PF[i].PITM[1] = BP1;
			BP1 += 5;
		}
		else if (gengo1 == ID_ICSSH_HEISEI_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = BP1;
			PF[i].PITM[1] = (char *)0;
			BP1 += 5;
		}
		else {
			PF[i].PITM[0] = (char *)0;
			PF[i].PITM[1] = (char *)0;
		}
		i++;

		if (gengo2 == ID_ICSSH_REIWA_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = (char *)0;
			PF[i].PITM[1] = BP1;
			BP1 += 5;
		}
		else if (gengo2 == ID_ICSSH_HEISEI_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = BP1;
			PF[i].PITM[1] = (char *)0;
			BP1 += 5;
		}
		else {
			PF[i].PITM[0] = (char *)0;
			PF[i].PITM[1] = (char *)0;
		}
		i++;
	}


	if( m_pSnHeadData->Sn_Syuusei & 0x04 ){
		return( _xprn( Pno, dp, PF, 0, pDC, isNoImage, 0x0202 ));
	}
	else{
		return( _xprn( Pno, dp, PF, 0, pDC, isNoImage, 0x0101 ));
	}
}

//-----------------------------------------------------------------------------
// àÛç¸ä÷êî
//-----------------------------------------------------------------------------
int CH26HyoView::kjn_dprn2( int dp, CDC* pDC, char *BP, BOOL isNoImage )
{
	int		i, j, x, sw;

	PBUF	PF[30];
	memset( PF, '\0', sizeof(PF) );

	i = 0;
	memset( BP, '\0', KSK_BUF_SIZE );
	char *BP1 = BP;
	//int Pno = 16;
	int Pno = 0;
	int st = 0;
	if ( ( st = GetGengoNo_Ksk() ) == -1 ) {
		return -1;
	}

	if ( st == ID_ICSSH_REIWA_GENGO ) {
		Pno = 26;
	}
	else {
		Pno = 25;
	}

	// íÒèoîNåéì˙
	for( x = 1, j = 0; x < 4; j++, x++, BP1 += 3 ) {
		sw = 0;
		if( x == 0 ){
			if( m_pSnKskData->KsData.KSdata[x] ){
				sw = 1;
			}
			else{
				if( m_pSnKskData->KsData.KSdata[x+1] ){
					sw = 1;
				}
			}
		}
		else{
			if( m_pSnKskData->KsData.KSdata[x] ){
				sw = 1;
			}
		}
		if( sw ){
			//if( (x==1) && IsJcContinuePrint(false) && (m_pSnKskData->KsData.KSdata[0]==0x05) ){
			//	sprintf_s(BP1, 3, "%2x", m_pSnKskData->KsData.KSdata[x]+0x30);
			//}
			//else{
				sprintf_s(BP1, 3, "%2x", m_pSnKskData->KsData.KSdata[x]);
			//}
		}
		else{
			memset( BP1, '\0', 2 );
		}
		PF[i].PITM[j] = BP1;
	}
	i++;

	// çXê≥åàíËí ímèëì˙ït
	for( x = 1, j = 0; x < 4; j++, x++, BP1 += 3 ) {
		sw = 0;
		if( x == 0 ){
			if( m_pSnKskData->KsData.KSktdt[x] ){
				sw = 1;
			}
			else{
				if( m_pSnKskData->KsData.KSktdt[x+1] ){
					sw = 1;
				}
			}
		}
		else{
			if( m_pSnKskData->KsData.KSktdt[x] ){
				sw = 1;
			}
		}
		if( sw ){
			//if( (x==1) && IsJcContinuePrint(false) && (m_pSnKskData->KsData.KSktdt[0]==0x05) ){
			//	sprintf_s(BP1, 3, "%2x", m_pSnKskData->KsData.KSktdt[x]+0x30);
			//}
			//else{
				sprintf_s(BP1, 3, "%2x", m_pSnKskData->KsData.KSktdt[x]);
			//}
		}
		else{
			memset( BP1, '\0', 2 );
		}
		PF[i].PITM[j] = BP1;
	}
	i++;

	//if (IsSwitch_HeiseiReiwa(false)) {
	if( (IsSwitch_HeiseiReiwa(false)) || (st == ID_ICSSH_REIWA_GENGO) ){
		if (m_pSnKskData->KsData.KSktdt[0] == ID_ICSSH_REIWA_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = (char *)0;
			PF[i].PITM[1] = BP1;
			BP1 += 5;
		}
		else if (m_pSnKskData->KsData.KSktdt[0] == ID_ICSSH_HEISEI_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = BP1;
			PF[i].PITM[1] = (char *)0;
			BP1 += 5;
		}
		else {
			PF[i].PITM[0] = (char *)0;
			PF[i].PITM[1] = (char *)0;
		}
		i++;
	}

	// ê\çêÅAçXê≥ÅAåàíË
	for( j = 0; j < 3; j++, BP1 += 2 ){
		if( j == m_pSnKskData->KsData.KSktsw ){
			sprintf_s( BP1, 2, " " );
		}
		else{
			memset( BP1, '\0', 1 );
		}
		PF[i].PITM[j] = BP1;
	}
	i++;

	return( _xprn( Pno, dp, PF, 0, pDC, isNoImage, 0x0101 ));
}

//-----------------------------------------------------------------------------
// àÛç¸ä÷êî
//-----------------------------------------------------------------------------
int CH26HyoView::kjn_dprn3( int dp, CDC* pDC, char *BP, BOOL isNoImage )
{
	int		i, j, x;
	char	buf[128];
	int		len;
	KSIVAL*	pK;

	PBUF	PF[30];
	memset( PF, '\0', sizeof(PF) );
	//int Pno = 16;
	int Pno = 0;
	int st = 0;
	if ( ( st = GetGengoNo_Ksk() ) == -1 ) {
		return -1;
	}

	if ( st == ID_ICSSH_REIWA_GENGO ) {
		Pno = 26;
	}
	else {
		Pno = 25;
	}

	i = 0;
	memset( BP, '\0', KSK_BUF_SIZE );
	char *BP1 = BP;

	// ämíËäz
	for( j = x = 0; x < 10; j++, x++ ){
		pK = &m_pSnKskData->KsData.KSval[x];

		::ZeroMemory( buf, sizeof( buf ));
/*- '14.09.02 -*/
//		m_Arith.l_print( buf, pK->KVsval, FMT12S );
/*-------------*/
		m_Arith.l_print( buf, pK->KVsval, FMT16_EX );
/*-------------*/
		len = (int)strlen( buf );
		memmove( BP1, buf, len );
		PF[i].PITM[j] = BP1;	BP1 += ( len+1 );
	}
	i++;
	for( j = 0, x = 10; x < 19; j++, x++ ){
		pK = &m_pSnKskData->KsData.KSval[x];

		::ZeroMemory( buf, sizeof( buf ));
/*- '14.09.02 -*/
//		m_Arith.l_print( buf, pK->KVsval, FMT12S );
/*-------------*/
		m_Arith.l_print( buf, pK->KVsval, FMT16_EX );
/*-------------*/
		len = (int)strlen( buf );
		memmove( BP1, buf, len );
		PF[i].PITM[j] = BP1;	BP1 += ( len+1 );
	}
	i++;

	// ê≥ìñÇ∆Ç∑ÇÈã‡äz
	for( j = x = 0; x < 10; j++, x++ ){
		pK = &m_pSnKskData->KsData.KSval[x];

		::ZeroMemory( buf, sizeof( buf ));
/*- '14.09.02 -*/
//		m_Arith.l_print( buf, pK->KVkval, FMT12S );
/*-------------*/
		m_Arith.l_print( buf, pK->KVkval, FMT16_EX );
/*-------------*/
		len = (int)strlen( buf );
		memmove( BP1, buf, len );
		PF[i].PITM[j] = BP1;	BP1 += ( len+1 );
	}
	i++;
	for( j = 0, x = 10; x < 19; j++, x++ ){
		pK = &m_pSnKskData->KsData.KSval[x];

		::ZeroMemory( buf, sizeof( buf ));
/*- '14.09.02 -*/
//		m_Arith.l_print( buf, pK->KVkval, FMT12S );
/*-------------*/
		m_Arith.l_print( buf, pK->KVkval, FMT16_EX );
/*-------------*/
		len = (int)strlen( buf );
		memmove( BP1, buf, len );
		PF[i].PITM[j] = BP1;	BP1 += ( len+1 );
	}
	i++;

	return( _xprn( Pno, dp, PF, 0, pDC, isNoImage, 0x0101 ));
}

//-----------------------------------------------------------------------------
// àÛç¸ä÷êî
//-----------------------------------------------------------------------------
int CH26HyoView::kjn_dprn4( int dp, CDC* pDC, char *BP, BOOL isNoImage )
{
	int		i, j, x, sw;
	char	buf[256];
	int		len;

	PBUF	PF[30];
	memset( PF, '\0', sizeof(PF) );
	//int Pno = 16;
	int Pno = 0;
	int st = 0;
	if ( ( st = GetGengoNo_Ksk() ) == -1 ) {
		return -1;
	}

	if ( st == ID_ICSSH_REIWA_GENGO ) {
		Pno = 26;
	}
	else {
		Pno = 25;
	}

	i = 0;
	memset( BP, '\0', KSK_BUF_SIZE );
	char *BP1 = BP;

// çXê≥ÇÃêøãÅÇÇ∑ÇÈóùóRìô
	for( j = 0, len = 80; j < 3; j++ ){
		::ZeroMemory( buf, sizeof( buf ));
		m_Util.LineSpaceSet( &m_pSnKskData->KsData.KSreas[len*j], buf, len );
		memmove( BP1, buf, len );
		PF[i].PITM[j] = BP1;	BP1 += ( len+1 );
	}
	i++;

// èCê≥ê\çêíÒèoîNåéì˙ñîÇÕçXê≥åàíËí ímèëéÛóùîNåéì˙
	for( x = 1, j = 0; x < 4; j++, x++, BP1 += 3 ) {
		sw = 0;
		if( x == 0 ){
			if( m_pSnKskData->KsData.KSstdt[x] )	sw = 1;
			else{
				if( m_pSnKskData->KsData.KSstdt[x+1] )	sw = 1;
			}
		}
		else{
			if( m_pSnKskData->KsData.KSstdt[x] )	sw = 1;
		}
		if( sw ){
			//if( (x==1) && IsJcContinuePrint(false) && (m_pSnKskData->KsData.KSstdt[0]==0x05) ){
			//	sprintf_s(BP1, 3, "%2x", m_pSnKskData->KsData.KSstdt[x]+0x30);
			//}
			//else{
				sprintf_s(BP1, 3, "%2x", m_pSnKskData->KsData.KSstdt[x]);
			//}
		}
		else{
			memset(BP1, '\0', 2);
		}
		PF[i].PITM[j] = BP1;
	}
	i++;

	//if (IsSwitch_HeiseiReiwa()) {
	if (IsSwitch_HeiseiReiwa() || (st == ID_ICSSH_REIWA_GENGO)) {
		if (m_pSnKskData->KsData.KSstdt[0] == ID_ICSSH_REIWA_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = (char *)0;
			PF[i].PITM[1] = BP1;
			BP1 += 5;
		}
		else if (m_pSnKskData->KsData.KSstdt[0] == ID_ICSSH_HEISEI_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = BP1;
			PF[i].PITM[1] = (char *)0;
			BP1 += 5;
		}
		else {
			PF[i].PITM[0] = (char *)0;
			PF[i].PITM[1] = (char *)0;
		}
		i++;
	}

// ìYïtèëóﬁ
	for( j = 0, len = 30; j < 2; j++ ){
		::ZeroMemory( buf, sizeof( buf ));
		m_Util.LineSpaceSet( &m_pSnKskData->KsData.KSdocu[len*j], buf, len );
		memmove( BP1, buf, len );
		PF[i].PITM[j] = BP1;	BP1 += ( len+1 );
	}
	i++;

// ämíËäzÅiÅ@Å@äzÅj
	::ZeroMemory( buf, sizeof( buf ));
	switch( m_pSnKskData->KsData.KSktsw ){
	case 0:	strcpy_s( buf, sizeof( buf ), "ê\Å@çê" );	break;
	case 1:	strcpy_s( buf, sizeof( buf ), "çXÅ@ê≥" );	break;
	case 2:	strcpy_s( buf, sizeof( buf ), "åàÅ@íË" );	break;
	}
	len = (int)strlen( buf );
	memmove( BP1, buf, len );
	PF[i++].PITM[0] = BP1;	BP1 += ( len+1 );

	return( _xprn( Pno, dp, PF, 0, pDC, isNoImage, 0x0101 ));
}

//-----------------------------------------------------------------------------
// àÛç¸ä÷êî
//-----------------------------------------------------------------------------
int CH26HyoView::kjn_dprn5( int dp, CDC* pDC, char *BP, BOOL isNoImage )
{
	int		i, j, len;
	char	st1[128], st2[128], cbf[256];

	PBUF	PF[30];
	memset( PF, '\0', sizeof(PF) );
	//int Pno = 16;
	int Pno = 0;
	int st = 0;
	if ( ( st = GetGengoNo_Ksk() ) == -1 ) {
		return -1;
	}

	if ( st == ID_ICSSH_REIWA_GENGO ) {
		Pno = 26;
	}
	else {
		Pno = 25;
	}

	i = 0;
	memset( BP, '\0', KSK_BUF_SIZE );
	char *BP1 = BP;
	memset( cbf, ' ', sizeof( cbf ));

	if( !m_pSnKskData->KsData.KSbpsw ){	// ã‚çs
	// ã‚çsìôñºèÃ
		::ZeroMemory( st1, sizeof( st1 ));
		::ZeroMemory( st2, sizeof( st2 ));
		m_Util.LineStrSet( m_pSnKskData->KsData.KSbank.Bname, st1, st2, 10, 12 );
		if( (int)strlen( st2 ) ){
			len = (int)strlen( st1 );
			memmove( BP1, st1, len );
			PF[i].PITM[0] = BP1;	BP1 += ( len+1 );
			len = (int)strlen( st2 );
			memmove( BP1, st2, len );
			PF[i++].PITM[1] = BP1;	BP1 += ( len+1 );
			memmove( BP1, cbf, 20 );
			PF[i++].PITM[0] = BP1;	BP1 += ( 20+1 );
		}
		else{
			memmove( BP1, cbf, 20 );
			PF[i].PITM[0] = BP1;	BP1 += ( 20+1 );
			memmove( BP1, cbf, 20 );
			PF[i++].PITM[1] = BP1;	BP1 += ( 20+1 );
			len = (int)strlen( st1 );
			memmove( BP1, st1, len );
			PF[i++].PITM[0] = BP1;	BP1 += ( len+1 );
		}

	// ã‚çsìôãÊï™
		if( (int)strlen( m_pSnKskData->KsData.KSbank.Bname )){

			if( !m_pSnKskData->KsData.KSbank.Bkbn )	sprintf_s( BP1, 2, " " );
			else								memset( BP1, '\0', 1 );
			PF[i].PITM[0] = BP1;
			BP1 += 2;
			i++;

			for( j = 0; j< 2; j++, BP1 += 2 ){
				if( j+1 == m_pSnKskData->KsData.KSbank.Bkbn )	sprintf_s( BP1, 2, " " );
				else										memset( BP1, '\0', 1 );
				PF[i].PITM[j] = BP1;
			}
			i++;

			for( j = 0; j< 2; j++, BP1 += 2 ){
				if( j+3 == m_pSnKskData->KsData.KSbank.Bkbn )	sprintf_s( BP1, 2, " " );
				else										memset( BP1, '\0', 1 );
				PF[i].PITM[j] = BP1;
			}
			i++;
		}
		else{

			memset( BP1, '\0', 1 );
			PF[i].PITM[0] = BP1;
			BP1 += 2;
			i++;

			for( j = 0; j< 2; j++, BP1 += 2 ){
				memset( BP1, '\0', 1 );
				PF[i].PITM[j] = BP1;
			}
			i++;
			for( j = 0; j< 2; j++, BP1 += 2 ){
				memset( BP1, '\0', 1 );
				PF[i].PITM[j] = BP1;
			}
			i++;
		}

	// ñ{éxìXñºèÃ
		::ZeroMemory( st1, sizeof( st1 ));
		::ZeroMemory( st2, sizeof( st2 ));
		m_Util.LineStrSet( m_pSnKskData->KsData.KSbank.Sname, st1, st2, 10, 12 );
		if( (int)strlen( st2 ) ){
			len = (int)strlen( st1 );
			memmove( BP1, st1, len );
			PF[i].PITM[0] = BP1;	BP1 += ( len+1 );
			len = (int)strlen( st2 );
			memmove( BP1, st2, len );
			PF[i++].PITM[1] = BP1;	BP1 += ( len+1 );
			memmove( BP1, cbf, 20 );
			PF[i++].PITM[0] = BP1;	BP1 += ( 20+1 );
		}
		else{
			memmove( BP1, cbf, 20 );
			PF[i].PITM[0] = BP1;	BP1 += ( 20+1 );
			memmove( BP1, cbf, 20 );
			PF[i++].PITM[1] = BP1;	BP1 += ( 20+1 );
			len = (int)strlen( st1 );
			memmove( BP1, st1, len );
			PF[i++].PITM[0] = BP1;	BP1 += ( len+1 );
		}

	// ñ{éxìXãÊï™
//-- '15.03.13 --
//		if( (int)strlen( m_pSnKskData->KsData.KSbank.Sname )){
//			for( j = 0; j< 2; j++, BP1 += 2 ){
//				if( j+1 == m_pSnKskData->KsData.KSbank.Skbn )	sprintf_s( BP1, 2, " " );
//				else										memset( BP1, '\0', 1 );
//				PF[i].PITM[j] = BP1;
//			}
//			i++;
//			for( j = 0; j< 2; j++, BP1 += 2 ){
//				if( j+3 == m_pSnKskData->KsData.KSbank.Skbn )	sprintf_s( BP1, 2, " " );
//				else										memset( BP1, '\0', 1 );
//				PF[i].PITM[j] = BP1;
//			}
//			i++;
//
//			if( 5 == m_pSnKskData->KsData.KSbank.Skbn )	sprintf_s( BP1, 2, " " );
//			else									memset( BP1, '\0', 1 );
//			PF[i].PITM[0] = BP1;
//			BP1 += 2;
//			i++;
//		}
//		else{
//			for( j = 0; j< 2; j++, BP1 += 2 ){
//				memset( BP1, '\0', 1 );
//				PF[i].PITM[j] = BP1;
//			}
//			i++;
//			for( j = 0; j< 2; j++, BP1 += 2 ){
//				memset( BP1, '\0', 1 );
//				PF[i].PITM[j] = BP1;
//			}
//			i++;
//
//			memset( BP1, '\0', 1 );
//			PF[i].PITM[0] = BP1;
//			BP1 += 2;
//			i++;
//		}
//---------------
		for( j=0; j<2; j++, BP1+=2 ){
			if( j+1 == m_pSnKskData->KsData.KSbank.Skbn ){
				sprintf_s( BP1, 2, " " );
			}
			else{
				memset( BP1, '\0', 1 );
			}
			PF[i].PITM[j] = BP1;
		}
		i++;
		for( j=0; j<2; j++, BP1+=2 ){
			if( j+3 == m_pSnKskData->KsData.KSbank.Skbn ){
				sprintf_s( BP1, 2, " " );
			}
			else{
				memset( BP1, '\0', 1 );
			}
			PF[i].PITM[j] = BP1;
		}
		i++;

		if( 5 == m_pSnKskData->KsData.KSbank.Skbn ){
			sprintf_s( BP1, 2, " " );
		}
		else{
			memset( BP1, '\0', 1 );
		}
		PF[i].PITM[0] = BP1;
		BP1 += 2;
		i++;
//---------------

	// óaã‡
		memmove( BP1, m_pSnKskData->KsData.KSbank.Dname, 12 );
		PF[i++].PITM[0] = BP1;	BP1 += ( 12+1 );

	// å˚ç¿î‘çÜ
		memmove( BP1, m_pSnKskData->KsData.KSbank.Accnt, 10 );
		PF[i++].PITM[0] = BP1;	BP1 += ( 10+1 );
	}
	else	{
		for( i = 0; i != 30; i++ ) {	
			pbclr( 0, 30, &PF[i] );
		}
	}

	return( _xprn( Pno, dp, PF, 0, pDC, isNoImage, 0x0101 ));
}

//-----------------------------------------------------------------------------
// àÛç¸ä÷êî
//-----------------------------------------------------------------------------
int CH26HyoView::kjn_dprn6( int dp, CDC* pDC, char *BP, BOOL isNoImage )
{
	PBUF	PF[30];
	memset( PF, '\0', sizeof(PF) );
	//int Pno = 16;
	int Pno = 0;
	int st = 0;
	if ( ( st = GetGengoNo_Ksk() ) == -1 ) {
		return -1;
	}

	if ( st == ID_ICSSH_REIWA_GENGO ) {
		Pno = 26;
	}
	else {
		Pno = 25;
	}

	int		i;
	char	buf[256], cbf[256];

	i = 0;
	memset( BP, '\0', KSK_BUF_SIZE );
	char *BP1 = BP;
	memset( cbf, ' ', sizeof( cbf ));

// óXï÷ã«
	// å˚ç¿î‘çÜ1
	if( !m_pSnKskData->KsData.KSbpsw )	memmove( BP1, cbf, 5 );
	else					memmove( BP1, m_pSnKskData->KsData.KSpost.Acnt1, 5 );
	PF[i++].PITM[0] = BP1;	BP1 += ( 5+1 );
	::ZeroMemory( buf, sizeof(buf));
	if( !m_pSnKskData->KsData.KSbpsw )	strcpy_s(buf, sizeof( buf ), "  ");
	else					strcpy_s(buf, sizeof( buf ), "Å@");
	memmove( BP1, buf, 2 );
	PF[i++].PITM[0] = BP1;	BP1 += ( 2+1 );
	// å˚ç¿î‘çÜ2
	if( !m_pSnKskData->KsData.KSbpsw )	memmove( BP1, cbf, 8 );
	else					memmove( BP1, m_pSnKskData->KsData.KSpost.Acnt2, 8 );
	PF[i++].PITM[0] = BP1;	BP1 += ( 8+1 );
	// óXï÷ã«ñºèÃ
	if( !m_pSnKskData->KsData.KSbpsw )	memmove( BP1, cbf, 12 );
	else					memmove( BP1, m_pSnKskData->KsData.KSpost.Pname, 16 );
	PF[i++].PITM[0] = BP1;	BP1 += ( 16+1 );

	char	ZHnam[128]={0};		// ê≈óùémñ@êlñº
	char	ZName[128]={0};		// ê≈óùéméÅñº
	memmove( ZHnam, m_pShinInfo->pO_DATA->o_ofcnam, sizeof(m_pShinInfo->pO_DATA->o_ofcnam) );
//-- '15.02.28 --
//	memmove( ZName, m_pShinInfo->pZ_DATA->z_zeinam, sizeof(m_pShinInfo->pZ_DATA->z_zeinam) );
//---------------
	MakeOutZeirishiName( ZName, sizeof(ZName) );
//---------------

// ê≈óùémñº
	if( m_pPrintCmInfo->OP2 ){
		memmove( BP1, cbf, 40 );
		PF[i].PITM[0] = BP1;	BP1 += ( 40+1 );
		memmove( BP1, cbf, 40 );
		PF[i++].PITM[1] = BP1;	BP1 += ( 40+1 );
		memmove( BP1, cbf, 40 );
		PF[i++].PITM[0] = BP1;	BP1 += ( 40+1 );
	}
	else{
		if( m_pPrintCmInfo->OPA == 2 || ( m_pPrintCmInfo->OPA == 1 && (m_pPrintCmInfo->OPsign&0x01) )){
			m_Util.LineSpaceSet( ZHnam, buf, 40 );
			memmove( BP1, buf, 40 );
			PF[i].PITM[0] = BP1;	BP1 += ( 40+1 );
			if( m_pPrintCmInfo->OPA == 1 && (m_pPrintCmInfo->OPsign&0x01) )
				memmove( BP1, cbf, 40 );
			else{
//-- '15.02.28 --
//				m_Util.LineSpaceSet( ZName, buf, 40 );
//				memmove( BP1, buf, 40 );
//---------------
				int slen = (int)strlen(ZName) + 1;
				memmove( BP1, ZName, slen );
//---------------
			}
			PF[i++].PITM[1] = BP1;	BP1 += ( 40+1 );
			memmove( BP1, cbf, 40 );
			PF[i++].PITM[0] = BP1;	BP1 += ( 40+1 );
		}
		else{
			memmove( BP1, cbf, 40 );
			PF[i].PITM[0] = BP1;	BP1 += ( 40+1 );
			memmove( BP1, cbf, 40 );
			PF[i++].PITM[1] = BP1;	BP1 += ( 40+1 );

//-- '15.02.28 --
//			if( m_pPrintCmInfo->OPA == 1 )	m_Util.LineSpaceSet( ZHnam, buf, 40 );
//			else						m_Util.LineSpaceSet( ZName, buf, 40 );
//			memmove( BP1, buf, 40 );
//			PF[i++].PITM[0] = BP1;	BP1 += ( 40+1 );
//---------------
			int slen=0;
			if( m_pPrintCmInfo->OPA == 1 ){
				slen = 40;
				m_Util.LineSpaceSet( ZHnam, buf, slen );
				memmove( BP1, buf, slen );
			}
			else{
				slen = (int)strlen(ZName);
				memmove( BP1, ZName, slen );
			}
			PF[i++].PITM[0] = BP1;
			BP1 += ( slen+1 );
//---------------
		}
	}

	return( _xprn( Pno, dp, PF, 0, pDC, isNoImage, 0x0101 ));
}

//-----------------------------------------------------------------------------
// çXê≥ÇÃêøãÅÅ@ñ@êlóp 
//-----------------------------------------------------------------------------
int CH26HyoView::data_hjn( CDC* pDC )
{
	int		dp, st;

	char	ZHnam[128]={0};		// ê≈óùémñ@êlñº
	char	ZName[128]={0};		// ê≈óùéméÅñº
	memcpy( ZHnam, m_pShinInfo->pO_DATA->o_ofcnam, sizeof(m_pShinInfo->pO_DATA->o_ofcnam) );
	memcpy( ZName, m_pShinInfo->pZ_DATA->z_zeinam, sizeof(m_pShinInfo->pZ_DATA->z_zeinam) );

	BOOL isNoImage = FALSE;
	if( m_pPrintCmInfo->PRtype&0x80 ){
		isNoImage = FALSE;	// ÉCÉÅÅ[ÉWèoóÕÅ@óL
	}
	else{
		isNoImage = TRUE;	// Å@Å@ÅVÅ@Å@Å@Å@ñ≥
	}

	//char	*BP;
	char	*BP = NULL;
	if( (BP = new char[KSK_BUF_SIZE]) == NULL ){
		return -1;
	}
	st = 0;

	//int Pno = 15;			//Åö//[12'09.28]
	int Pno = 0;
	if ( ( st = GetGengoNo_Ksk() ) == -1 ) {
		return -1;
	}
		
	if ( st == ID_ICSSH_REIWA_GENGO ) {
		Pno = 21;
	}
	else {
		Pno = 20;
	}

	st = 0;

	for( dp = 1; dp <= 6; ++dp ) {	
		switch( dp ) {
			case	1:	st = hjn_dprn1( dp, pDC, BP, isNoImage );		break;
			case	2:	st = hjn_dprn2( dp, pDC, BP, isNoImage );		break;
			case	3:	st = hjn_dprn3( dp, pDC, BP, isNoImage );		break;
			case	4:	st = hjn_dprn4( dp, pDC, BP, isNoImage );		break;
			case	5:	st = hjn_dprn5( dp, pDC, BP, isNoImage );		break;
			case	6:	st = hjn_dprn6( dp, pDC, BP, isNoImage );		break;
		}
		if( st )	break;
	}

	delete [] BP;

//Åö//[12'08.10]///ìdéqê\çêäÆóπçœ 
   if( *m_pShinInfo->pFinish && *m_pShinInfo->pFinishSw && strlen( m_pShinInfo->pFinishPrn)){
   		int yPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[3].HyoTn]._PIVad;
		int xPlus = (int)m_pPrintCmInfo->pPrMove[m_pPrintCmInfo->pMvRec[3].HyoTn]._PIHad;

		long	PH  = 2540L / 10L;		// 1 character
		long	PV1 = 2540L / 6L;		// 1/6 LF
  		long YPosition = ( PV1 * 2L ) + yPlus;
//		long XPosition = ( PH *  17L );
		long XPosition = ( PH *  7L ) + ( PH / 2 ) + xPlus;
		int	Gothic095 = m_pVprn->RegisterFont( 90, 0, 0, "ÇlÇrÅ@ÉSÉVÉbÉN" );
		int	Gothic108 = m_pVprn->RegisterFont( MAKELONG( 108, 1 ), 0, 0, "ÇlÇrÅ@ÉSÉVÉbÉN" );
		struct	_SF_PAR		SfPar;
		memset( (char *)&SfPar, '\0', sizeof( struct _SF_PAR ) );
		SfPar.sf_fcsg =	( SFp_xl | SFp_yb );					// ÉtÉ@ÉìÉNÉVÉáÉìÉTÉCÉì
		SfPar.sf_hvsg = 0;										// èoóÕï˚å¸
		SfPar.sf_xfld = (11L * PH);								// â°èoóÕóÃàÊïù
		SfPar.sf_yfld =	PV1;									// ècèoóÕóÃàÊïù
		SfPar.sf_mrgn[0] = 0;									// óÃàÊì‡É}Å[ÉWÉìÅ@ç∂
		SfPar.sf_mrgn[1] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@âE
		SfPar.sf_mrgn[2] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@è„
		SfPar.sf_mrgn[3] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@â∫
		SfPar.sf_kpit = 0;										// ï∂éöÉsÉbÉ`ïùÅ@X0.1Œﬂ≤›ƒ
		SfPar.sf_ksiz =	Gothic108;								// ï∂éöÉTÉCÉYÅ@Å@X0.1Œﬂ≤›ƒ		
		SfPar.sf_erto = 0;										// ï∂éöägëÂèkè¨ó¶
		if( m_pVprn->DrawString( 0, XPosition, YPosition, _T("ìdéqê\çêçœ"), &SfPar ) )	return( 1 );

  		YPosition = ( PV1 * 2L ) + yPlus;;
//		XPosition = ( PH *  25L );
		XPosition = ( PH *  15L ) + ( PH / 2 ) + xPlus;
		memset( (char *)&SfPar, '\0', sizeof( struct _SF_PAR ) );
		SfPar.sf_fcsg =	( SFp_xl | SFp_yb );					// ÉtÉ@ÉìÉNÉVÉáÉìÉTÉCÉì
		SfPar.sf_hvsg = 0;										// èoóÕï˚å¸
		SfPar.sf_xfld = (35L * PH);								// â°èoóÕóÃàÊïù
		SfPar.sf_yfld =	PV1;									// ècèoóÕóÃàÊïù
		SfPar.sf_mrgn[0] = 0;									// óÃàÊì‡É}Å[ÉWÉìÅ@ç∂
		SfPar.sf_mrgn[1] = 10;									// Å@Å@ÅVÅ@Å@Å@Å@Å@âE
		SfPar.sf_mrgn[2] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@è„
		SfPar.sf_mrgn[3] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@â∫
		SfPar.sf_kpit = 0;										// ï∂éöÉsÉbÉ`ïùÅ@X0.1Œﬂ≤›ƒ
		SfPar.sf_ksiz =	Gothic095;								// ï∂éöÉTÉCÉYÅ@Å@X0.1Œﬂ≤›ƒ		
		SfPar.sf_erto = 0;										// ï∂éöägëÂèkè¨ó¶
		if( m_pVprn->DrawString( 0, XPosition, YPosition, m_pShinInfo->pFinishPrn, &SfPar ) )	return( 1 );

		m_pVprn->DeleteRegFont( Gothic095 );
		m_pVprn->DeleteRegFont( Gothic108 );

   }
///////////////////

	return( st );
}

//-----------------------------------------------------------------------------
// àÛç¸ä÷êî
//-----------------------------------------------------------------------------
int CH26HyoView::hjn_dprn1( int dp, CDC* pDC, char *BP, BOOL isNoImage )
{
	PBUF	PF[30];
	memset( PF, '\0', sizeof(PF) );
	//int Pno = 15;
	int Pno = 0;
	int st = 0;
	if ( ( st = GetGengoNo_Ksk() ) == -1 ) {
		return -1;
	}

	if ( st == ID_ICSSH_REIWA_GENGO ) {
		Pno = 21;
	}
	else {
		Pno = 20;
	}

	int		i, j, k, sw;
	char	buf[128];

	i = 0;
	memset( BP, '\0', KSK_BUF_SIZE );
	char *BP1 = BP;

// ê≈ñ±èêñº
	::ZeroMemory( buf, sizeof( buf ));
	m_Util.LineSpaceSet( (char*)m_pSnHeadData->Sn_ZMSYO, buf, 12 );
	memmove( BP1, buf, 12 );
	PF[i++].PITM[0] = BP1;	BP1 += ( 12+1 );

	if( !m_pPrintCmInfo->OP3 ){	
	// óXï÷î‘çÜ
		::ZeroMemory( buf, sizeof( buf ));
		m_Util.LineSpaceSet( (char*)m_pSnHeadData->Sn_ZIP, buf, 10 );
		memmove( BP1, buf, 10 );
		PF[i++].PITM[0] = BP1;	BP1 += ( 10+1 );

	// î[ê≈ín
		for( j = 0; j < 2; j++ ){
			::ZeroMemory( buf, sizeof( buf ));
			m_Util.LineSpaceSet( (char*)&m_pSnHeadData->Sn_NOZEI[40*j], buf, 40 );
			memmove( BP1, buf, 40 );
			PF[i].PITM[j] = BP1;	BP1 += ( 40+1 );
		}
	}
	else{
		PF[i++].PITM[0] = (char *)0;
		PF[i].PITM[0] = (char *)0;
		PF[i].PITM[1] = (char *)0;
	}
	i++;

	// î[ê≈ínìdòbî‘çÜ
	if( !m_pPrintCmInfo->OPE ){
		::ZeroMemory( buf, sizeof( buf ));
		m_Util.LineSpaceSet( (char*)m_pSnHeadData->Sn_TELNO1, buf, 6 );
		memmove( BP1, buf, 6 );
		PF[i++].PITM[0] = BP1;	BP1 += ( 6+1 );
		::ZeroMemory( buf, sizeof( buf ));
		m_Util.LineSpaceSet( (char*)m_pSnHeadData->Sn_TELNO2, buf, 4 );
		memmove( BP1, buf, 4 );
		PF[i++].PITM[0] = BP1;	BP1 += ( 4+1 );
		::ZeroMemory( buf, sizeof( buf ));
		m_Util.LineSpaceSet( (char*)m_pSnHeadData->Sn_TELNO3, buf, 4 );
		memmove( BP1, buf, 4 );
		PF[i++].PITM[0] = BP1;	BP1 += ( 4+1 );
	}
	else{
		PF[i++].PITM[0] = (char *)0;
		PF[i++].PITM[0] = (char *)0;
		PF[i++].PITM[0] = (char *)0;
	}
	if( !m_pPrintCmInfo->OPF ){	
	// ñ@êlñº ÉtÉäÉKÉi
		::ZeroMemory( buf, sizeof( buf ));
		m_Util.LineSpaceSet( (char*)m_pSnHeadData->Sn_COFRI, buf, 80 );
		memmove( BP1, buf, 80 );
		PF[i++].PITM[0] = BP1;	BP1 += ( 80+1 );
	}
	else{
		PF[i++].PITM[0] = (char *)0;
	}

	// ñ@êlñº
	if( !m_pPrintCmInfo->OP4  ){
		::ZeroMemory( buf, sizeof( buf ));
		m_Util.LineSpaceSet( (char*)m_pSnHeadData->Sn_CONAM, buf, 40 );
		memmove( BP1, buf, 40 );
		PF[i++].PITM[0] = BP1;	BP1 += ( 40+1 );
	}
	else{
		PF[i++].PITM[0] = (char *)0;
	}

//2015.10.20 INSERT START
//2015.12.03 UPDATE START
	////ñ@êlî‘çÜ
	//BOOL myno_flg = FALSE;
	//if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){ //å¥ë•
	//	if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
	//		myno_flg = TRUE;
	//	}
	//}else{ //ä»à’
	//	if( m_ShRevType >= ID_ICSSH_MYNUMBER_REVTYPE ){
	//		myno_flg = TRUE;
	//	}
	//}

	//if( myno_flg == TRUE ){
	//	CString myno_str;
	//	if( m_pSnHeadData->GetMyNumber( myno_str ) == 0 ){
	//		//if( m_pSnHeadData->IsSoleProprietor() == TRUE ){
	//		//	myno = ' ' + myno_str;
	//		//}
	//		memmove( BP1, myno_str, 13 );
	//		PF[i++].PITM[0] = BP1; BP1 += ( 13+1 );
	//	}else{
	//		pbclr( 0, 1, &PF[i++] );
	//	}
	//}

//2016.02.22 UPDATE START
	//int myno_sgn = 0;
	//if( m_Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_GENSOKU ){ //å¥ë•
	//	if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
	//		if( m_ShRevType >= ID_ICSSH_MYNUMBER_REVTYPE ){
	//			myno_sgn = 2; //É}ÉCÉiÉìÉoÅ[Å@èoóÕóLÅ@ÉåÉCÉAÉEÉgëŒâû
	//		}else{
	//			myno_sgn = 1; //É}ÉCÉiÉìÉoÅ[Å@èoóÕñ≥Å@ÉåÉCÉAÉEÉgëŒâû
	//		}
	//	}else{
	//		myno_sgn = 0; //É}ÉCÉiÉìÉoÅ[Å@èoóÕñ≥Å@ÉåÉCÉAÉEÉgîÒëŒâû
	//	}
	//}else{ //ä»à’
	//	if( m_ShRevType >= ID_ICSSH_MYNUMBER_REVTYPE ){
	//		myno_sgn = 2; //É}ÉCÉiÉìÉoÅ[Å@èoóÕóLÅ@ÉåÉCÉAÉEÉgëŒâû
	//	}else{
	//		myno_sgn = 0; //É}ÉCÉiÉìÉoÅ[Å@èoóÕñ≥Å@ÉåÉCÉAÉEÉgîÒëŒâû
	//	}
	//}

	//CICSMNSub8 mnsub;
	//CString myno_str,myno_prn;

	//switch( myno_sgn ){
	//	case 0:
	//		break;
	//	case 1:
	//		pbclr( 0, 1, &PF[i++] );
	//		break;
	//	case 2:
	//		if( m_pSnHeadData->GetMyNumber( myno_str ) == 0 ){
	//			if( m_pPset->psSigCoprn&0x4000){
	//				myno_prn = mnsub.MakePrintKojinNo( myno_str,  m_pVprn->Get_KojinPrintFlg() );
	//			}else{
	//				myno_prn = myno_str;
	//			}
	//			memmove( BP1, myno_prn, 13 );
	//			PF[i++].PITM[0] = BP1; BP1 += ( 13+1 );
	//		}else{
	//			pbclr( 0, 1, &PF[i++] );
	//		}
	//		break;
	//	default:
	//		break;
	//}

	CICSMNSub8 mnsub;
	CString myno_str,myno_prn;

	if( m_pSnHeadData->GetMyNumber( myno_str ) == 0 ){
		if( m_pPset->psSigCoprn&0x4000){
			myno_prn = mnsub.MakePrintKojinNo( myno_str,  m_pVprn->Get_KojinPrintFlg() );
		}else{
			myno_prn = myno_str;
		}
		memmove( BP1, myno_prn, 13 );
		PF[i++].PITM[0] = BP1; BP1 += ( 13+1 );
	}else{
		pbclr( 0, 1, &PF[i++] );
	}

//2016.02.22 UPDATE END

//2015.12.03 UPDATE END
//2015.10.20 INSERT END

	// ë„ï\é“ÅiêUÇËÇ™Ç»Åj
	if( !m_pPrintCmInfo->OPG ){
		::ZeroMemory( buf, sizeof( buf ));
		m_Util.LineSpaceSet( (char*)m_pSnHeadData->Sn_DIFRI, buf, 80 );
		memmove( BP1, buf, 80 );
		PF[i++].PITM[0] = BP1;	BP1 += ( 80+1 );
	}
	else{
		PF[i++].PITM[0] = (char *)0;
	}

	// ë„ï\é“ñº
	if( !m_pPrintCmInfo->OP5 ){
		::ZeroMemory( buf, sizeof( buf ));
		m_Util.LineSpaceSet( (char*)m_pSnHeadData->Sn_DIHYO, buf, 40 );
		memmove( BP1, buf, 40 );
		PF[i++].PITM[0] = BP1;	BP1 += ( 40+1 );
	}
	else{
		PF[i++].PITM[0] = (char *)0;
	}

// â€ê≈ä˙ä‘
	k = i;
	pbclr( 0, 30, &PF[k++] );
	pbclr( 0, 30, &PF[k++] );
	pbclr( 0, 30, &PF[k++] );
	pbclr( 0, 30, &PF[k++] );
	if (IsSwitch_HeiseiReiwa()) {
		pbclr(0, 30, &PF[k++]);
		pbclr(0, 30, &PF[k++]);
	}

	int gengo1 = 0, gengo2 = 0;
	char	yymmdd[3] = {0};
	//if( IsJcContinuePrint(true) ){
	//	m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	//}
	//else{
		//m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1);
	//}

	//if( m_pSnHeadData->Sn_Syuusei & 0x04 )	i += 2;
	if (m_pSnHeadData->Sn_Syuusei & 0x04)	i += 4;
	for( j = 0; j < 3; j++, BP1 += 3 ) {
		sw = 0;
		if( j == 0 ){
			if( yymmdd[j] )	sw = 1;
			else{
				if( yymmdd[j+1] )	sw = 1;
			}
		}
		else{
			if( yymmdd[j] )	sw = 1;
		}
		if( sw )	sprintf_s( BP1, 3, "%2x", yymmdd[j] );
		else		memset( BP1, '\0', 2 );
		PF[i].PITM[j] = BP1;
	}
	
	i++;
	memset( yymmdd, '\0', sizeof(yymmdd) );
	//if( IsJcContinuePrint(true) ){
	//	m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	//}
	//else{
		//m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo2);
	//}
	for( j = 0; j < 3; j++, BP1 += 3 ) {
		sw = 0;
		if( j == 0 ){
			if( yymmdd[j] )	sw = 1;
			else{
				if( yymmdd[j+1] )	sw = 1;
			}
		}
		else{
			if( yymmdd[j] )	sw = 1;
		}
		if( sw )
			sprintf_s( BP1, 3, "%2x", yymmdd[j] );
		else
			memset( BP1, '\0', 2 );
		PF[i].PITM[j] = BP1;
	}
	i++;

	//if (IsSwitch_HeiseiReiwa()) {
	if (IsSwitch_HeiseiReiwa() || (st == ID_ICSSH_REIWA_GENGO)) {
		if (gengo1 == ID_ICSSH_REIWA_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = (char *)0;
			PF[i].PITM[1] = BP1;
			BP1 += 5;
		}
		else if (gengo1 == ID_ICSSH_HEISEI_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = BP1;
			PF[i].PITM[1] = (char *)0;
			BP1 += 5;
		}
		else {
			PF[i].PITM[0] = (char *)0;
			PF[i].PITM[1] = (char *)0;
		}
		i++;

		if (gengo2 == ID_ICSSH_REIWA_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = (char *)0;
			PF[i].PITM[1] = BP1;
			BP1 += 5;
		}
		else if (gengo2 == ID_ICSSH_HEISEI_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = BP1;
			PF[i].PITM[1] = (char *)0;
			BP1 += 5;
		}
		else {
			PF[i].PITM[0] = (char *)0;
			PF[i].PITM[1] = (char *)0;
		}
		i++;
	}

	if( m_pSnHeadData->Sn_Syuusei & 0x04 )
			return( _xprn( Pno, dp, PF, 0, pDC, isNoImage, 0x0202 ));
	else	return( _xprn( Pno, dp, PF, 0, pDC, isNoImage, 0x0101 ));
}

//-----------------------------------------------------------------------------
// àÛç¸ä÷êî
//-----------------------------------------------------------------------------
int CH26HyoView::hjn_dprn2( int dp, CDC* pDC, char *BP, BOOL isNoImage )
{
	PBUF	PF[30];
	memset( PF, '\0', sizeof(PF) );
	//int Pno = 15;
	int Pno = 0;
	int st = 0;
	if ( ( st = GetGengoNo_Ksk() ) == -1 ) {
		return -1;
	}

	if ( st == ID_ICSSH_REIWA_GENGO ) {
		Pno = 21;
	}
	else {
		Pno = 20;
	}

	int		i, j, k, x, sw;

	i = 0;
	memset( BP, '\0', KSK_BUF_SIZE );
	char *BP1 = BP;

// íÒèoîNåéì˙
	for( x = 1, j = 0; x < 4; j++, x++, BP1 += 3 ) {
		sw = 0;
		if( x == 0 ){
			if( m_pSnKskData->KsData.KSdata[x] )	sw = 1;
			else{
				if( m_pSnKskData->KsData.KSdata[x+1] )	sw = 1;
			}
		}
		else{
			if( m_pSnKskData->KsData.KSdata[x] )	sw = 1;
		}
		if( sw ){
			//if( (x==1) && IsJcContinuePrint(true) && (m_pSnKskData->KsData.KSdata[0]==0x05) ){
			//	sprintf_s(BP1, 3, "%2x", m_pSnKskData->KsData.KSdata[x]+0x30 );
			//}
			//else{
				sprintf_s(BP1, 3, "%2x", m_pSnKskData->KsData.KSdata[x]);
			//}
		}
		else{
			memset(BP1, '\0', 2);
		}
		PF[i].PITM[j] = BP1;
	}
	i++;

	k = i;
	pbclr( 0, 30, &PF[k++] );
	pbclr( 0, 30, &PF[k++] );
	pbclr( 0, 30, &PF[k++] );
	pbclr( 0, 30, &PF[k++] );
	pbclr( 0, 30, &PF[k++] );
	pbclr( 0, 30, &PF[k++] );
	//if (IsSwitch_HeiseiReiwa()) {
	if (IsSwitch_HeiseiReiwa() || (st == ID_ICSSH_REIWA_GENGO)) {
		pbclr(0, 30, &PF[k++]);
	}
	if( m_pSnHeadData->Sn_Syuusei & 0x04 )	{
		i += 4;
	// çXê≥åàíËí ímèëì˙ït
		for( x = 1, j = 0; x < 4; j++, x++, BP1 += 3 ) {
			sw = 0;
			if( x == 0 ){
				if( m_pSnKskData->KsData.KSktdt[x] )	sw = 1;
				else{
					if( m_pSnKskData->KsData.KSktdt[x+1] )	sw = 1;
				}
			}
			else{
				if( m_pSnKskData->KsData.KSktdt[x] )	sw = 1;
			}
			if( sw ){
				//if( (x==1) && IsJcContinuePrint(true) && (m_pSnKskData->KsData.KSktdt[0]==0x05) ){
				//	sprintf_s(BP1, 3, "%2x", m_pSnKskData->KsData.KSktdt[x]+0x30);
				//}
				//else{
					sprintf_s(BP1, 3, "%2x", m_pSnKskData->KsData.KSktdt[x]);
				//}
			}
			else{
				memset(BP1, '\0', 2);
			}
			PF[i].PITM[j] = BP1;
		}
		i++;

	// ê\çêÅAçXê≥ÅAåàíË
		for( j = 0; j < 3; j++, BP1 += 2 ){
			if( j == m_pSnKskData->KsData.KSktsw )	sprintf_s( BP1, 2, " " );
			else						memset( BP1, '\0', 1 );
			PF[i].PITM[j] = BP1;
		}

		return( _xprn( Pno, dp, PF, 0, pDC, isNoImage, 0x0202 ));

	}
	else	{
	// çXê≥åàíËí ímèëì˙ït
		for( x = 1, j = 0; x < 4; j++, x++, BP1 += 3 ) {
			sw = 0;
			if( x == 0 ){
				if( m_pSnKskData->KsData.KSktdt[x] )	sw = 1;
				else{
					if( m_pSnKskData->KsData.KSktdt[x+1] )	sw = 1;
				}
			}
			else{
				if( m_pSnKskData->KsData.KSktdt[x] )	sw = 1;
			}
			if( sw ){
				//if( (x==1) && IsJcContinuePrint(true) && (m_pSnKskData->KsData.KSktdt[0]==0x05) ){
				//	sprintf_s(BP1, 3, "%2x", m_pSnKskData->KsData.KSktdt[x]+0x30);
				//}
				//else{
					sprintf_s(BP1, 3, "%2x", m_pSnKskData->KsData.KSktdt[x]);
				//}
			}
			else{
				memset(BP1, '\0', 2);
			}
			PF[i].PITM[j] = BP1;
		}
		i++;

		//if (IsSwitch_HeiseiReiwa()) {
		if (IsSwitch_HeiseiReiwa() || (st == ID_ICSSH_REIWA_GENGO)) {
			if (m_pSnKskData->KsData.KSktdt[0] == ID_ICSSH_REIWA_GENGO) {
				memmove(BP1, "    ", 4);
				PF[i].PITM[0] = (char *)0;
				PF[i].PITM[1] = BP1;
				BP1 += 5;
			}
			else if (m_pSnKskData->KsData.KSktdt[0] == ID_ICSSH_HEISEI_GENGO) {
				memmove(BP1, "    ", 4);
				PF[i].PITM[0] = BP1;
				PF[i].PITM[1] = (char *)0;
				BP1 += 5;
			}
			else {
				PF[i].PITM[0] = (char *)0;
				PF[i].PITM[1] = (char *)0;
			}
			i++;
		}

	// ê\çêÅAçXê≥ÅAåàíË
		for( j = 0; j < 3; j++, BP1 += 2 ){
			if( j == m_pSnKskData->KsData.KSktsw )	sprintf_s( BP1, 2, " " );
			else						memset( BP1, '\0', 1 );
			PF[i].PITM[j] = BP1;
		}
		i++;

		for( j = 0; j < 2; j++, BP1 += 2 ){
			if( j+1 == m_pSnKskData->KsData.KSthsw )	sprintf_s( BP1, 2, " " );
			else									memset( BP1, '\0', 1 );
			PF[i].PITM[j] = BP1;
		}
		i++;
		if( m_pSnKskData->KsData.KSthsw == 3 )		sprintf_s( BP1, 2, " " );
		else									memset( BP1, '\0', 1 );									
		PF[i].PITM[0] = BP1;

		return( _xprn( Pno, dp, PF, 0, pDC, isNoImage, 0x0101 ));
	}

}

//-----------------------------------------------------------------------------
// àÛç¸ä÷êî
//-----------------------------------------------------------------------------
int CH26HyoView::hjn_dprn3( int dp, CDC* pDC, char *BP, BOOL isNoImage )
{
	PBUF	PF[30];
	memset( PF, '\0', sizeof(PF) );
	//int Pno = 15;
	int Pno = 0;
	int st = 0;
	if ( ( st = GetGengoNo_Ksk() ) == -1 ) {
		return -1;
	}

	if ( st == ID_ICSSH_REIWA_GENGO ) {
		Pno = 21;
	}
	else {
		Pno = 20;
	}

	int		i, j, x;
	char	buf[128];
	int		len;
	KSIVAL*	pK;

	i = 0;
	memset( BP, '\0', KSK_BUF_SIZE );
	char *BP1 = BP;

// Ç±ÇÃêøãÅëOÇÃã‡äz
	for( j = x = 0; x < 10; j++, x++ ){
		pK = &m_pSnKskData->KsData.KSval[x];

		::ZeroMemory( buf, sizeof( buf ));
/*- '14.09.02 -*/
//		l_print( buf, pK->KVsval, FMT12S );
/*-------------*/
		m_Arith.l_print( buf, pK->KVsval, FMT16_EX );
/*-------------*/
		len = (int)strlen( buf );
		memmove( BP1, buf, len );
		PF[i].PITM[j] = BP1;	BP1 += ( len+1 );
	}
	i++;
	for( j = 0, x = 10; x < 19; j++, x++ ){
		pK = &m_pSnKskData->KsData.KSval[x];

		::ZeroMemory( buf, sizeof( buf ));
/*- '14.09.02 -*/
//		l_print( buf, pK->KVsval, FMT12S );
/*-------------*/
		m_Arith.l_print( buf, pK->KVsval, FMT16_EX );
/*-------------*/
		len = (int)strlen( buf );
		memmove( BP1, buf, len );
		PF[i].PITM[j] = BP1;	BP1 += ( len+1 );
	}
	i++;

// çXê≥ÇÃêøãÅã‡äz
	for( j = x = 0; x < 10; j++, x++ ){
		pK = &m_pSnKskData->KsData.KSval[x];

		::ZeroMemory( buf, sizeof( buf ));
/*- '14.09.02 -*/
//		l_print( buf, pK->KVkval, FMT12S );
/*-------------*/
		m_Arith.l_print( buf, pK->KVkval, FMT16_EX );
/*-------------*/
		len = (int)strlen( buf );
		memmove( BP1, buf, len );
		PF[i].PITM[j] = BP1;	BP1 += ( len+1 );
	}
	i++;
	for( j = 0, x = 10; x < 19; j++, x++ ){
		pK = &m_pSnKskData->KsData.KSval[x];

		::ZeroMemory( buf, sizeof( buf ));
/*- '14.09.02 -*/
//		l_print( buf, pK->KVkval, FMT12S );
/*-------------*/
		m_Arith.l_print( buf, pK->KVkval, FMT16_EX );
/*-------------*/
		len = (int)strlen( buf );
		memmove( BP1, buf, len );
		PF[i].PITM[j] = BP1;	BP1 += ( len+1 );
	}
	i++;

	return( _xprn( Pno, dp, PF, 0, pDC, isNoImage, 0x0101 ));
}

//-----------------------------------------------------------------------------
// àÛç¸ä÷êî
//-----------------------------------------------------------------------------
int CH26HyoView::hjn_dprn4( int dp, CDC* pDC, char *BP, BOOL isNoImage )
{
	PBUF	PF[30];
	memset( PF, '\0', sizeof(PF) );
	//int Pno = 15;
	int Pno = 0;
	int st = 0;
	if ( ( st = GetGengoNo_Ksk() ) == -1 ) {
		return -1;
	}

	if ( st == ID_ICSSH_REIWA_GENGO ) {
		Pno = 21;
	}
	else {
		Pno = 20;
	}

	int		i, j, x, sw;
	char	buf[256];
	int		len;

	i = 0;
	memset( BP, '\0', KSK_BUF_SIZE );
	char *BP1 = BP;

// çXê≥ÇÃêøãÅÇÇ∑ÇÈóùóRìô
	for( j = 0, len = 80; j < 3; j++ ){
		::ZeroMemory( buf, sizeof( buf ));
		m_Util.LineSpaceSet( &m_pSnKskData->KsData.KSreas[len*j], buf, len );
		memmove( BP1, buf, len );
		PF[i].PITM[j] = BP1;	BP1 += ( len+1 );
	}
	i++;

// èCê≥ê\çêíÒèoîNåéì˙
	for( x = 1, j = 0; x < 4; j++, x++, BP1 += 3 ) {
		sw = 0;
		if( x == 0 ){
			if( m_pSnKskData->KsData.KSstdt[x] )	sw = 1;
			else{
				if( m_pSnKskData->KsData.KSstdt[x+1] )	sw = 1;
			}
		}
		else{
			if( m_pSnKskData->KsData.KSstdt[x] )	sw = 1;
		}
		if( sw ){
			//if( (x==1) && IsJcContinuePrint(true) && (m_pSnKskData->KsData.KSstdt[0]==0x05) ){
			//	sprintf_s(BP1, 3, "%2x", m_pSnKskData->KsData.KSstdt[x]+0x30);
			//}
			//else{
				sprintf_s(BP1, 3, "%2x", m_pSnKskData->KsData.KSstdt[x]);
			//}
		}
		else{
			memset(BP1, '\0', 2);
		}
		PF[i].PITM[j] = BP1;
	}
	i++;

	//if (IsSwitch_HeiseiReiwa()) {
	if (IsSwitch_HeiseiReiwa() || (st == ID_ICSSH_REIWA_GENGO)) {
		if (m_pSnKskData->KsData.KSstdt[0] == ID_ICSSH_REIWA_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = (char *)0;
			PF[i].PITM[1] = BP1;
			BP1 += 5;
		}
		else if (m_pSnKskData->KsData.KSstdt[0] == ID_ICSSH_HEISEI_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = BP1;
			PF[i].PITM[1] = (char *)0;
			BP1 += 5;
		}
		else {
			PF[i].PITM[0] = (char *)0;
			PF[i].PITM[1] = (char *)0;
		}
		i++;
	}


// çXê≥åàíËí ímèëéÛóùîNåéì˙
	for( x = 1, j = 0; x < 4; j++, x++, BP1 += 3 ) {
		sw = 0;
		if( x == 0 ){
			if( m_pSnKskData->KsData.KSjrdt[x] )	sw = 1;
			else{
				if( m_pSnKskData->KsData.KSjrdt[x+1] )	sw = 1;
			}
		}
		else{
			if( m_pSnKskData->KsData.KSjrdt[x] )	sw = 1;
		}
		if( sw ){
			//if( (x==1) && IsJcContinuePrint(true) && (m_pSnKskData->KsData.KSjrdt[0]==0x05) ){
			//	sprintf_s(BP1, 3, "%2x", m_pSnKskData->KsData.KSjrdt[x]+0x30);
			//}
			//else{
				sprintf_s(BP1, 3, "%2x", m_pSnKskData->KsData.KSjrdt[x]);
			//}
		}
		else{
			memset(BP1, '\0', 2);
		}
		PF[i].PITM[j] = BP1;
	}
	i++;

	//if (IsSwitch_HeiseiReiwa()) {
	if (IsSwitch_HeiseiReiwa() || (st == ID_ICSSH_REIWA_GENGO)) {
		if (m_pSnKskData->KsData.KSjrdt[0] == ID_ICSSH_REIWA_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = (char *)0;
			PF[i].PITM[1] = BP1;
			BP1 += 5;
		}
		else if (m_pSnKskData->KsData.KSjrdt[0] == ID_ICSSH_HEISEI_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = BP1;
			PF[i].PITM[1] = (char *)0;
			BP1 += 5;
		}
		else {
			PF[i].PITM[0] = (char *)0;
			PF[i].PITM[1] = (char *)0;
		}
		i++;
	}

// ìYïtèëóﬁ
	for( j = 0, len = 30; j < 2; j++ ){
		::ZeroMemory( buf, sizeof( buf ));
		m_Util.LineSpaceSet( &m_pSnKskData->KsData.KSdocu[len*j], buf, len );
		memmove( BP1, buf, len );
		PF[i].PITM[j] = BP1;	BP1 += ( len+1 );
	}
	i++;

	return( _xprn( Pno, dp, PF, 0, pDC, isNoImage, 0x0101 ));
}

//-----------------------------------------------------------------------------
// àÛç¸ä÷êî
//-----------------------------------------------------------------------------
int CH26HyoView::hjn_dprn5( int dp, CDC* pDC, char *BP, BOOL isNoImage )
{
	PBUF	PF[30];
	memset( PF, '\0', sizeof(PF) );
	//int Pno = 15;
	int Pno = 0;
	int st = 0;
	if ( ( st = GetGengoNo_Ksk() ) == -1 ) {
		return -1;
	}

	if ( st == ID_ICSSH_REIWA_GENGO ) {
		Pno = 21;
	}
	else {
		Pno = 20;
	}

	int		i, j, len;
	char	st1[128], st2[128], cbf[256];

	i = 0;
	memset( BP, '\0', KSK_BUF_SIZE );
	char *BP1 = BP;
	memset( cbf, ' ', sizeof( cbf ));

	if( !m_pSnKskData->KsData.KSbpsw ){	// ã‚çs
	// ã‚çsìôñºèÃ
		::ZeroMemory( st1, sizeof( st1 ));
		::ZeroMemory( st2, sizeof( st2 ));
		m_Util.LineStrSet( m_pSnKskData->KsData.KSbank.Bname, st1, st2, 10, 12 );
		if( (int)strlen( st2 ) ){
			len = (int)strlen( st1 );
			memmove( BP1, st1, len );
			PF[i].PITM[0] = BP1;	BP1 += ( len+1 );
			len = (int)strlen( st2 );
			memmove( BP1, st2, len );
			PF[i++].PITM[1] = BP1;	BP1 += ( len+1 );
			memmove( BP1, cbf, 20 );
			PF[i++].PITM[0] = BP1;	BP1 += ( 20+1 );
		}
		else{
			memmove( BP1, cbf, 20 );
			PF[i].PITM[0] = BP1;	BP1 += ( 20+1 );
			memmove( BP1, cbf, 20 );
			PF[i++].PITM[1] = BP1;	BP1 += ( 20+1 );
			len = (int)strlen( st1 );
			memmove( BP1, st1, len );
			PF[i++].PITM[0] = BP1;	BP1 += ( len+1 );
		}

	// ã‚çsìôãÊï™
		if( (int)strlen( m_pSnKskData->KsData.KSbank.Bname )){

			if( !m_pSnKskData->KsData.KSbank.Bkbn )	sprintf_s( BP1, 2, " " );
			else								memset( BP1, '\0', 1 );
			PF[i].PITM[0] = BP1;	
			BP1 += 2;
			i++;

			for( j = 0; j< 2; j++, BP1 += 2 ){
				if( j+1 == m_pSnKskData->KsData.KSbank.Bkbn )	sprintf_s( BP1, 2, " " );
				else									memset( BP1, '\0', 1 );
				PF[i].PITM[j] = BP1;
			}
			i++;

			for( j = 0; j< 2; j++, BP1 += 2 ){
				if( j+3 == m_pSnKskData->KsData.KSbank.Bkbn )	sprintf_s( BP1, 2, " " );
				else										memset( BP1, '\0', 1 );
				PF[i].PITM[j] = BP1;
			}
			i++;
		}
		else{

			memset( BP1, '\0', 1 );
			PF[i].PITM[0] = BP1;
			BP1 += 2;
			i++;

			for( j = 0; j< 2; j++, BP1 += 2 ){
				memset( BP1, '\0', 1 );
				PF[i].PITM[j] = BP1;
			}
			i++;
			for( j = 0; j< 2; j++, BP1 += 2 ){
				memset( BP1, '\0', 1 );
				PF[i].PITM[j] = BP1;
			}
			i++;
		}

	// ñ{éxìXñºèÃ
		::ZeroMemory( st1, sizeof( st1 ));
		::ZeroMemory( st2, sizeof( st2 ));
		m_Util.LineStrSet( m_pSnKskData->KsData.KSbank.Sname, st1, st2, 10, 12 );
		if( (int)strlen( st2 ) ){
			len = (int)strlen( st1 );
			memmove( BP1, st1, len );
			PF[i].PITM[0] = BP1;	BP1 += ( len+1 );
			len = (int)strlen( st2 );
			memmove( BP1, st2, len );
			PF[i++].PITM[1] = BP1;	BP1 += ( len+1 );
			memmove( BP1, cbf, 20 );
			PF[i++].PITM[0] = BP1;	BP1 += ( 20+1 );
		}
		else{
			memmove( BP1, cbf, 20 );
			PF[i].PITM[0] = BP1;	BP1 += ( 20+1 );
			memmove( BP1, cbf, 20 );
			PF[i++].PITM[1] = BP1;	BP1 += ( 20+1 );
			len = (int)strlen( st1 );
			memmove( BP1, st1, len );
			PF[i++].PITM[0] = BP1;	BP1 += ( len+1 );
		}

	// ñ{éxìXãÊï™
//-- '15.03.13 --
//		if( (int)strlen( m_pSnKskData->KsData.KSbank.Sname )){
//			for( j = 0; j< 2; j++, BP1 += 2 ){
//				if( j+1 == m_pSnKskData->KsData.KSbank.Skbn )	sprintf_s( BP1, 2, " " );
//				else										memset( BP1, '\0', 1 );
//				PF[i].PITM[j] = BP1;
//			}
//			i++;
//			for( j = 0; j< 2; j++, BP1 += 2 ){
//				if( j+3 == m_pSnKskData->KsData.KSbank.Skbn )	sprintf_s( BP1, 2, " " );
//				else										memset( BP1, '\0', 1 );
//				PF[i].PITM[j] = BP1;
//			}
//			i++;
//			if( 5 == m_pSnKskData->KsData.KSbank.Skbn )	sprintf_s( BP1, 2, " " );
//			else									memset( BP1, '\0', 1 );
//			PF[i].PITM[0] = BP1;
//			BP1 += 2;
//			i++;
//		}
//		else{
//			for( j = 0; j< 2; j++, BP1 += 2 ){
//				memset( BP1, '\0', 1 );
//				PF[i].PITM[j] = BP1;
//			}
//			i++;
//			for( j = 0; j< 2; j++, BP1 += 2 ){
//				memset( BP1, '\0', 1 );
//				PF[i].PITM[j] = BP1;
//			}
//			i++;
//			memset( BP1, '\0', 1 );
//			PF[i].PITM[0] = BP1;
//			BP1 += 2;
//			i++;
//		}
//---------------
		for( j=0; j<2; j++, BP1+=2 ){
			if( j+1 == m_pSnKskData->KsData.KSbank.Skbn ){
				sprintf_s( BP1, 2, " " );
			}
			else{
				memset( BP1, '\0', 1 );
			}
			PF[i].PITM[j] = BP1;
		}
		i++;
		for( j=0; j<2; j++, BP1+=2 ){
			if( j+3 == m_pSnKskData->KsData.KSbank.Skbn ){
				sprintf_s( BP1, 2, " " );
			}
			else{
				memset( BP1, '\0', 1 );
			}
			PF[i].PITM[j] = BP1;
		}
		i++;
		if( 5 == m_pSnKskData->KsData.KSbank.Skbn ){
			sprintf_s( BP1, 2, " " );
		}
		else{
			memset( BP1, '\0', 1 );
		}
		PF[i].PITM[0] = BP1;
		BP1 += 2;
		i++;
//---------------

	// óaã‡
		memmove( BP1, m_pSnKskData->KsData.KSbank.Dname, 12 );
		PF[i++].PITM[0] = BP1;	BP1 += ( 12+1 );

	// å˚ç¿î‘çÜ
		memmove( BP1, m_pSnKskData->KsData.KSbank.Accnt, 10 );
		PF[i++].PITM[0] = BP1;	BP1 += ( 10+1 );
	}
	else	{
		for( i = 0; i != 30; i++ ) {	
			pbclr( 0, 30, &PF[i] );
		}
	}

	return( _xprn( Pno, dp, PF, 0, pDC, isNoImage, 0x0101 ));
}

//-----------------------------------------------------------------------------
// àÛç¸ä÷êî
//-----------------------------------------------------------------------------
int CH26HyoView::hjn_dprn6( int dp, CDC* pDC, char *BP, BOOL isNoImage )
{
	char	ZHnam[128]={0};		// ê≈óùémñ@êlñº
	char	ZName[128]={0};		// ê≈óùéméÅñº
	memcpy( ZHnam, m_pShinInfo->pO_DATA->o_ofcnam, sizeof(m_pShinInfo->pO_DATA->o_ofcnam) );
	memcpy( ZName, m_pShinInfo->pZ_DATA->z_zeinam, sizeof(m_pShinInfo->pZ_DATA->z_zeinam) );

	PBUF	PF[30];
	memset( PF, '\0', sizeof(PF) );
	//int Pno = 15;
	int Pno = 0;
	int st = 0;
	if ( ( st = GetGengoNo_Ksk() ) == -1 ) {
		return -1;
	}

	if ( st == ID_ICSSH_REIWA_GENGO ) {
		Pno = 21;
	}
	else {
		Pno = 20;
	}

	int		i, len;
	char	buf[256], cbf[256];

	i = 0;
	memset( BP, '\0', KSK_BUF_SIZE );
	char *BP1 = BP;
	memset( cbf, ' ', sizeof( cbf ));

// óXï÷ã«
	// å˚ç¿î‘çÜ1
	if( !m_pSnKskData->KsData.KSbpsw )	memmove( BP1, cbf, 5 );
	else					memmove( BP1, m_pSnKskData->KsData.KSpost.Acnt1, 5 );
	PF[i++].PITM[0] = BP1;	BP1 += ( 5+1 );
	// å˚ç¿î‘çÜ2
	if( !m_pSnKskData->KsData.KSbpsw )	memmove( BP1, cbf, 8 );
	else					memmove( BP1, m_pSnKskData->KsData.KSpost.Acnt2, 8 );
	PF[i++].PITM[0] = BP1;	BP1 += ( 8+1 );
	// óXï÷ã«ñºèÃ
	if( !m_pSnKskData->KsData.KSbpsw )	memmove( BP1, cbf, 12 );
	else					memmove( BP1, m_pSnKskData->KsData.KSpost.Pname, 16 );
	PF[i++].PITM[0] = BP1;	BP1 += ( 16+1 );

// ê≈óùémñº
	char	zbuf[256];
	::ZeroMemory( zbuf, sizeof( zbuf ));
	::ZeroMemory( buf, sizeof( buf ));
	if( m_pPrintCmInfo->OP2 ){
		memmove( BP1, "\0", 40 );
		PF[i].PITM[0] = BP1;	BP1 += ( 40+1 );
	}
	else{
//-- '15.02.28 --
//		switch( m_pPrintCmInfo->OPA ){
//		case 1:		memcpy( zbuf, ZHnam, sizeof( ZHnam ));	break;						// ñ@êlñº
//		case 2:		sprintf_s( zbuf, sizeof( zbuf ), "%sÅ@%s", ZHnam, ZName );	break;	// éÅñº&ñ@êlñº
//		default:	memcpy( zbuf, ZName, sizeof( ZName ));	break;						// éÅñº
//		}
//		len = (40*2)+2;
//		m_Util.LineSpaceSet( zbuf, buf, len );
//		memmove( BP1, buf, len );
//---------------
		char	tmpbuf[256]={0};
		MakeOutZeirishiName( tmpbuf, sizeof(tmpbuf) );
		if( m_pPrintCmInfo->OPA == 2 ){	// ñ@êlñº & éÅñº
			sprintf_s( zbuf, sizeof( zbuf ), _T("%sÅ@%s"), ZHnam, tmpbuf );
		}
		else{
			memmove( zbuf, tmpbuf, sizeof(zbuf) );
		}
		len = (int)strlen(zbuf);
		memmove( BP1, zbuf, len );
//---------------
	}
	PF[i].PITM[0] = BP1;	BP1 += ( len+1 );

	return( _xprn( Pno, dp, PF, 0, pDC, isNoImage, 0x0101 ));
}

//-----------------------------------------------------------------------------
// ëOîNé¿ê—Ç…ÇÊÇÈíÜä‘ê\çê
//-----------------------------------------------------------------------------
int CH26HyoView::data_mast( CDC* pDC )
{
	int		dp, st;
	
	char	*BP = NULL;
	if( (BP = new char[KSK_BUF_SIZE]) == NULL ){
		return -1;
	}
	memset( BP, '\0', KSK_BUF_SIZE );
	char	*BP1 = BP;

	st = 0;

	for( dp=1; dp<=5; ++dp ){	
		switch( dp ) {
			case	1:	st = _dprn1( dp, pDC, BP1 );		break;
			case	2:	st = _dprn2( dp, pDC, BP1 );		break;
			case	3:	st = _dprn3( dp, pDC, BP1 );		break;
			case	4:	st = _dprn4( dp, pDC, BP1 );		break;
		}
		if( st )	break;
	}

	delete( BP );

//--> '15.12.22 INSERT START
	if(( m_ShRevType >= ID_ICSSH_MYNUMBER_REVTYPE ) && m_pSnHeadData->IsSoleProprietor()){
		m_pVprn->DrawSline( 0, LKnrW0, 1500, 21230, 8070, 20530 ); //å¬êlî‘çÜóìÇ…éŒê¸
	}
//<-- '15.12.22 INSERT END


//Åö//[12'08.10]///ìdéqê\çêäÆóπçœ 
   if( *m_pShinInfo->pFinish && *m_pShinInfo->pFinishSw && strlen(m_pShinInfo->pFinishPrn) ){
		long	PH  = 2540L / 10L;		// 1 character
		long	PV1 = 2540L / 6L;		// 1/6 LF
  		long YPosition = ( PV1 * 36L ) + ( PV1 / 2 );
		long XPosition = ( PH / 2 );
		int	Gothic095 = m_pVprn->RegisterFont( 90, 0, 0, "ÇlÇrÅ@ÉSÉVÉbÉN" );
		int	Gothic108 = m_pVprn->RegisterFont( MAKELONG( 108, 1 ), 0, 0, "ÇlÇrÅ@ÉSÉVÉbÉN" );
		struct	_SF_PAR		SfPar;
		memset( (char *)&SfPar, '\0', sizeof( struct _SF_PAR ) );
		SfPar.sf_fcsg =	( SFp_xl | SFp_yb );					// ÉtÉ@ÉìÉNÉVÉáÉìÉTÉCÉì
		SfPar.sf_hvsg = 0;										// èoóÕï˚å¸
		SfPar.sf_xfld = (11L * PH);								// â°èoóÕóÃàÊïù
		SfPar.sf_yfld =	PV1;									// ècèoóÕóÃàÊïù
		SfPar.sf_mrgn[0] = 0;									// óÃàÊì‡É}Å[ÉWÉìÅ@ç∂
		SfPar.sf_mrgn[1] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@âE
		SfPar.sf_mrgn[2] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@è„
		SfPar.sf_mrgn[3] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@â∫
		SfPar.sf_kpit = 0;										// ï∂éöÉsÉbÉ`ïùÅ@X0.1Œﬂ≤›ƒ
		SfPar.sf_ksiz =	Gothic108;								// ï∂éöÉTÉCÉYÅ@Å@X0.1Œﬂ≤›ƒ		
		SfPar.sf_erto = 0;										// ï∂éöägëÂèkè¨ó¶
		if( m_pVprn->DrawString( 0, XPosition, YPosition, _T("ìdéqê\çêçœ"), &SfPar ) )	return( 1 );

  		YPosition = ( PV1 * 36L ) + ( PV1 / 2 );
		XPosition = ( PH *  8L ) + ( PH / 2 );
		memset( (char *)&SfPar, '\0', sizeof( struct _SF_PAR ) );
		SfPar.sf_fcsg =	( SFp_xl | SFp_yb );					// ÉtÉ@ÉìÉNÉVÉáÉìÉTÉCÉì
		SfPar.sf_hvsg = 0;										// èoóÕï˚å¸
		SfPar.sf_xfld = (35L * PH);								// â°èoóÕóÃàÊïù
		SfPar.sf_yfld =	PV1;									// ècèoóÕóÃàÊïù
		SfPar.sf_mrgn[0] = 0;									// óÃàÊì‡É}Å[ÉWÉìÅ@ç∂
		SfPar.sf_mrgn[1] = 10;									// Å@Å@ÅVÅ@Å@Å@Å@Å@âE
		SfPar.sf_mrgn[2] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@è„
		SfPar.sf_mrgn[3] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@â∫
		SfPar.sf_kpit = 0;										// ï∂éöÉsÉbÉ`ïùÅ@X0.1Œﬂ≤›ƒ
		SfPar.sf_ksiz =	Gothic095;								// ï∂éöÉTÉCÉYÅ@Å@X0.1Œﬂ≤›ƒ		
		SfPar.sf_erto = 0;										// ï∂éöägëÂèkè¨ó¶
		if( m_pVprn->DrawString( 0, XPosition, YPosition, m_pShinInfo->pFinishPrn, &SfPar ) )	return( 1 );

		m_pVprn->DeleteRegFont( Gothic095 );
		m_pVprn->DeleteRegFont( Gothic108 );

   }
///////////////////

	return( st );
}

#ifdef _20150127_EXTEND_
//-----------------------------------------------------------------------------
// ëOîNé¿ê—Ç…ÇÊÇÈíÜä‘ê\çê(é¿ïîÇP)
//-----------------------------------------------------------------------------
int CH26HyoView::_dprn1( int dp, CDC* pDC, char *BP )
{
	int		i, j;
	char	buf[128];
	char	bf1[10], bf2[10], bf3[10];
	int		len;
	PBUF	PF[30];

	i = 0;
	memset( BP, '\0', KSK_BUF_SIZE );
	char *BP1 = BP;

// î[ê≈ín
	for( j = 0; j < 2; j++ ){
		memmove( BP1, &m_pSnHeadData->Sn_NOZEI[0], 40 );
		PF[i].PITM[j] = BP1;	BP1 += ( 40+1 );
	}
	i++;
	for( j = 0; j < 2; j++ ){
		memmove( BP1, &m_pSnHeadData->Sn_NOZEI[40], 40 );
		PF[i].PITM[j] = BP1;	BP1 += ( 40+1 );
	}
	i++;

// Tel
	memset( buf, '\0', sizeof( buf ));
	memset( bf1, '\0', sizeof( bf1 ));
	memset( bf2, '\0', sizeof( bf2 ));
	memset( bf3, '\0', sizeof( bf3 ));
	len = kjlen( m_pSnHeadData->Sn_TELNO1, 6 );
	memmove( bf1, m_pSnHeadData->Sn_TELNO1, len );
	len = kjlen( m_pSnHeadData->Sn_TELNO2, 4 );
	memmove( bf2, m_pSnHeadData->Sn_TELNO2, len );
	len = kjlen( m_pSnHeadData->Sn_TELNO3, 4 );
	memmove( bf3, m_pSnHeadData->Sn_TELNO3, len );
	wsprintf( buf, "%6s Å|%4s Å|%4s", bf1, bf2, bf3 );
	len = (int)strlen( buf );
	for( j = 0; j < 2; j++ ){
		memmove( BP1, buf, len );
		PF[i].PITM[j] = BP1;	BP1 += ( len+1 );
	}
	i++;

// ñºèÃñîÇÕâÆçÜ ÉtÉäÉKÉi
	for( j = 0; j < 2; j++ ){
		memmove( BP1, m_pSnHeadData->Sn_COFRI, 80 );
		PF[i].PITM[j] = BP1;	BP1 += ( 80+1 );
	}
	i++;

// ñºèÃñîÇÕâÆçÜ
	for( j = 0; j < 2; j++ ){
		memmove( BP1, m_pSnHeadData->Sn_CONAM, 40 );
		PF[i].PITM[j] = BP1;	BP1 += ( 40+1 );
	}
	i++;

// ë„ï\é“éÅñº ÉtÉäÉKÉi
	for( j = 0; j < 2; j++ ){
		memmove( BP1, m_pSnHeadData->Sn_DIFRI, 80 );
		PF[i].PITM[j] = BP1;	BP1 += ( 80+1 );
	}
	i++;

// ë„ï\é“éÅñº
	for( j = 0; j < 2; j++ ){
		memmove( BP1, m_pSnHeadData->Sn_DIHYO, 40 );
		PF[i].PITM[j] = BP1;	BP1 += ( 40+1 );
	}
	i++;
	
	return( _xprn( 1, dp, PF, 0, pDC, 0, 0x0102) );
}
#else
//-----------------------------------------------------------------------------
// ëOîNé¿ê—Ç…ÇÊÇÈíÜä‘ê\çê(é¿ïîÇP)
//-----------------------------------------------------------------------------
int CH26HyoView::_dprn1( int dp, CDC* pDC, char *BP )
{
	int		i, j;
	char	buf[128];
	char	bf1[10], bf2[10], bf3[10];
	int		len;
	PBUF	PF[30];

	i = 0;
	memset( BP, '\0', KSK_BUF_SIZE );
	char *BP1 = BP;

	// î[ê≈ín
	if( !m_pPrintCmInfo->OP3 ){
		for( j = 0; j < 2; j++ ){
			memmove( BP1, &m_pSnHeadData->Sn_NOZEI[0], 40 );
			PF[i].PITM[j] = BP1;	BP1 += ( 40+1 );
		}
		i++;
		for( j = 0; j < 2; j++ ){
			memmove( BP1, &m_pSnHeadData->Sn_NOZEI[40], 40 );
			PF[i].PITM[j] = BP1;	BP1 += ( 40+1 );
		}
		i++;
	}
	else{
		pbclr( 0, 2, &PF[i++] );
		pbclr( 0, 2, &PF[i++] );
	}

	// Tel
	if( !m_pPrintCmInfo->OPE ){
		memset( buf, '\0', sizeof( buf ));
		memset( bf1, '\0', sizeof( bf1 ));
		memset( bf2, '\0', sizeof( bf2 ));
		memset( bf3, '\0', sizeof( bf3 ));
		len = kjlen( m_pSnHeadData->Sn_TELNO1, 6 );
		memmove( bf1, m_pSnHeadData->Sn_TELNO1, len );
		len = kjlen( m_pSnHeadData->Sn_TELNO2, 4 );
		memmove( bf2, m_pSnHeadData->Sn_TELNO2, len );
		len = kjlen( m_pSnHeadData->Sn_TELNO3, 4 );
		memmove( bf3, m_pSnHeadData->Sn_TELNO3, len );
		wsprintf( buf, "%6s Å|%4s Å|%4s", bf1, bf2, bf3 );
		len = (int)strlen( buf );
		for( j = 0; j < 2; j++ ){
			memmove( BP1, buf, len );
			PF[i].PITM[j] = BP1;	BP1 += ( len+1 );
		}
		i++;
	}
	else{
		pbclr( 0, 2, &PF[i++] );
	}

	// ñºèÃñîÇÕâÆçÜ ÉtÉäÉKÉi
	if( !m_pPrintCmInfo->OPF ){	
		for( j = 0; j < 2; j++ ){
			memmove( BP1, m_pSnHeadData->Sn_COFRI, 80 );
			PF[i].PITM[j] = BP1;	BP1 += ( 80+1 );
		}
		i++;
	}
	else{
		pbclr( 0, 2, &PF[i++] );
	}

	// ñºèÃñîÇÕâÆçÜ
	if( !m_pPrintCmInfo->OP4 ){
		for( j = 0; j < 2; j++ ){
			memmove( BP1, m_pSnHeadData->Sn_CONAM, 40 );
			PF[i].PITM[j] = BP1;	BP1 += ( 40+1 );
		}
		i++;
	}
	else{
		pbclr( 0, 2, &PF[i++] );
	}

//2015.10.20 INSERT START
//2015.12.03 UPDATE START
	//if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
	//	CString myno_str;
	//	if( m_pSnHeadData->GetMyNumber( myno_str ) == 0 ){
	//		for( j = 0; j < 2; j++ ){
	//			if( m_pSnHeadData->IsSoleProprietor() == TRUE ){
	//				myno_str = ' ' + myno_str;
	//			}
	//			memmove( BP1, myno_str, 13 );
	//			PF[i].PITM[j] = BP1; BP1 += ( 13+1 );
	//		}
	//		i++;
	//	}else{
	//		pbclr( 0, 2, &PF[i++] );
	//	}
	//}

//-- '15.12.19 --
//	if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
//		if( m_ShRevType >= ID_ICSSH_MYNUMBER_REVTYPE ){
//			CString myno_str,myno_prn;
//			if( m_pSnHeadData->GetMyNumber( myno_str ) == 0 ){
//				if( m_pPset->psSigCoprn&0x4000){
//					myno_prn = mnsub.MakePrintKojinNo( myno_str,  m_pVprn->Get_KojinPrintFlg() );
//				}else{
//					myno_prn = myno_str;
//				}
//				if( m_pSnHeadData->IsSoleProprietor() == TRUE ){
//					myno_prn = ' ' + myno_prn;
//				}
//				for( j = 0; j < 2; j++ ){
//					memmove( BP1, myno_prn, 13 );
//					PF[i].PITM[j] = BP1; BP1 += ( 13+1 );
//				}
//				i++;
//			}else{
//				pbclr( 0, 2, &PF[i++] );
//			}
//		}else{
//			pbclr( 0, 2, &PF[i++] );
//		}
//	}
//---------------
	if( m_ShRevType >= ID_ICSSH_MYNUMBER_REVTYPE ){
		CICSMNSub8 mnsub;
		CString myno_str,myno_prn;
		if( m_pSnHeadData->GetMyNumber( myno_str ) == 0 ){
			if( m_pPset->psSigCoprn&0x4000){
				myno_prn = mnsub.MakePrintKojinNo( myno_str,  m_pVprn->Get_KojinPrintFlg() );
			}else{
				myno_prn = myno_str;
			}
			if( m_pSnHeadData->IsSoleProprietor() == TRUE ){
				myno_prn = ' ' + myno_prn;
			}
			for( j = 0; j < 2; j++ ){
				memmove( BP1, myno_prn, 13 );
//--> '15.12.24 INS START
				if( (j==1) && m_pSnHeadData->IsSoleProprietor() ){
					memset( BP1, '\0', 13 );
				}
//<-- '15.12.24 INS END
				PF[i].PITM[j] = BP1; BP1 += ( 13+1 );
			}
			i++;
		}else{
			pbclr( 0, 2, &PF[i++] );
		}
	}
//---------------
//2015.12.03 UPDATE END
//2015.10.20 INSERT END

	// ë„ï\é“éÅñº ÉtÉäÉKÉi
	if( !m_pPrintCmInfo->OPG ){
		for( j = 0; j < 2; j++ ){
			memmove( BP1, m_pSnHeadData->Sn_DIFRI, 80 );
			PF[i].PITM[j] = BP1;	BP1 += ( 80+1 );
		}
		i++;
	}
	else{
		pbclr( 0, 2, &PF[i++] );
	}

	// ë„ï\é“éÅñº
	if( !m_pPrintCmInfo->OP5 ){
		for( j = 0; j < 2; j++ ){
			memmove( BP1, m_pSnHeadData->Sn_DIHYO, 40 );
			PF[i].PITM[j] = BP1;	BP1 += ( 40+1 );
		}
		i++;
	}
	else{
		pbclr( 0, 2, &PF[i++] );
	}
	
	return( _xprn( 1, dp, PF, 0, pDC, 0, 0x0102) );
}
#endif

//-----------------------------------------------------------------------------
// ëOîNé¿ê—Ç…ÇÊÇÈíÜä‘ê\çê(é¿ïîÇQ)
//-----------------------------------------------------------------------------
int CH26HyoView::_dprn2( int dp, CDC* pDC, char *BP )
{
	int		i, j;
	int		len;
	char	buf[128];
	PBUF	PF[30];

	i = 0;
	memset( BP, '\0', KSK_BUF_SIZE );
	char *BP1 = BP;

// éÅñºÅïñ@êlñºí«â¡ 08/04/22
	for( j = 0; j < 2; j++ ){	// è„ãlÇﬂÅ@ñ@êlñº
//2017.07.06 INSERT START
		if( m_pPrintCmInfo->OP2 != 0 ){
			;
		}else
//2017.07.06 INSERT END
		if( m_pPrintCmInfo->OPA == 2 || ( m_pPrintCmInfo->OPA == 1 && m_pPrintCmInfo->OPsign&0x01 )){
			memmove( BP1, m_pShinInfo->pO_DATA->o_ofcnam, 40 );
		}
		PF[i].PITM[j] = BP1;
		BP1 += ( 40+1 );
	}
	i++;

//-- '15.02.28 --
//	for( j = 0; j < 2; j++ ){	// éÅñºÅiéÅñºÅïñ@êlñºÅj
//		if( m_pPrintCmInfo->OPA == 2 )
//			memmove( BP1, m_pShinInfo->pO_DATA->o_zeinam, 40 );
//		PF[i].PITM[j] = BP1;
//		BP1 += ( 40+1 );
//	}
//	i++;
//---------------
	// éÅñºÅiéÅñºÅïñ@êlñºÅj
	int	slen = 0;
//2017.07.06 INSERT START
	if( m_pPrintCmInfo->OP2 != 0 ){
		;
	}else
//2017.07.06 INSERT END

	if( m_pPrintCmInfo->OPA == 2 ){
		memset( buf, '\0', sizeof( buf ));
		MakeOutZeirishiName( buf, sizeof(buf) );
		slen = (int)strlen( buf );
	}
	if( slen > 40 ){
		pbclr( 0, 2, &PF[i++] );

		memmove( BP1, buf, slen );
		for( j=0; j<2; j++ ){
			memmove( BP1, buf, slen );
			PF[i].PITM[j] = BP1;
			BP1 += ( slen+1 );
		}
		i++;
	}
	else if( slen > 0 ){
		for( j=0; j<2; j++ ){
			memmove( BP1, buf, slen );
			PF[i].PITM[j] = BP1;
			BP1 += ( slen+1 );
		}
		i++;

		pbclr( 0, 2, &PF[i++] );
	}
	else{
		pbclr( 0, 2, &PF[i++] );
		pbclr( 0, 2, &PF[i++] );
	}
//---------------

//-- '15.02.28 --
//	for( j = 0; j < 2; j++ ){
//		if( m_pPrintCmInfo->OPA == 2 || ( m_pPrintCmInfo->OPA == 1 && m_pPrintCmInfo->OPsign&0x01 ));
//		else
//			memmove( BP1, m_pPrintCmInfo->Zname, 40 );
//		PF[i].PITM[j] = BP1;
//		BP1 += ( 40+1 );
//	}
//	i++;
//---------------
	// éÅñºorñ@êlñº
	slen = 0;
//2017.07.06 INSERT START
	memset( buf, '\0', sizeof( buf ));
//2017.07.06 INSERT END
	if( m_pPrintCmInfo->OP2 != 0 ){
		;
	}
	else if( (m_pPrintCmInfo->OPA==2) || ((m_pPrintCmInfo->OPA==1)&&(m_pPrintCmInfo->OPsign&0x01)) ){
		;
	}
	else{
		memset( buf, '\0', sizeof( buf ));
		MakeOutZeirishiName( buf, sizeof(buf) );
		slen = (int)strlen( buf );
	}

	if( slen > 40 ){
		pbclr( 0, 2, &PF[i++] );
		for( j=0; j<2; j++ ){
			memmove( BP1, buf, slen );
			PF[i].PITM[j] = BP1;
			BP1 += ( slen+1 );
		}
		i++;
	}
	else if( slen > 0 ){
		for( j=0; j<2; j++ ){
			memmove( BP1, buf, slen );
			PF[i].PITM[j] = BP1;
			BP1 += ( slen+1 );
		}
		i++;
		pbclr( 0, 2, &PF[i++] );
	}
	else{
		pbclr( 0, 2, &PF[i++] );
		pbclr( 0, 2, &PF[i++] );
	}
//---------------

// Tel
	memset( buf, '\0', sizeof( buf ));
	ZTel_Set( buf, m_pPrintCmInfo->ZtelP, m_pPrintCmInfo->OP9, 16, 30 );
	len = (int)strlen( buf );
	for( j = 0; j < 2; j++ ){
		memmove( BP1, buf, len );
		PF[i].PITM[j] = BP1;
		BP1 += ( len+1 );
	}
	i++;

	return( _xprn( 1, dp, PF, 0, pDC, 0, 0x0102 ));
}

//-----------------------------------------------------------------------------
// ëOîNé¿ê—Ç…ÇÊÇÈíÜä‘ê\çê(é¿ïîÇR)
//-----------------------------------------------------------------------------
int CH26HyoView::_dprn3( int dp, CDC* pDC, char *BP )
{
	int		i, j, k, x;
	char	buf[128];
	CString str;
	int		len, sw = 0;
	PBUF	PF[30];

	i = 0;
	memset( BP, '\0', KSK_BUF_SIZE );
	char *BP1 = BP;

// íÒèoîNåéì˙
	char	yymmdd[4]={0};
	if( IsJcContinuePrint(false) ){
		m_pSnHeadData->GetYmdData(ID_ICSSH_PRESENT_DATE, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	}
	else{
		m_pSnHeadData->GetYmdDataGen(ID_ICSSH_PRESENT_DATE, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	}
	for( j = k = 0; k < 2; k++ ){  
		for( x = 0; x < 3; j++, x++, BP1 += 3 ) {
			sw = 0;
			if( x == 0 ){
				if( yymmdd[x] )	sw = 1;
				else{
					if( yymmdd[x+1] )	sw = 1;
				}
			}
			else{
				if( yymmdd[x] )	sw = 1;
			}
			if( sw )
				wsprintf( BP1, "%02X", yymmdd[x] );
			else
				memset( BP1, '\0', 2 );
			PF[i].PITM[j] = BP1;
		}
	}
	i++;

// ê≈ñ±èêñº
	for( j = 0; j < 2; j++ ){
		memmove( BP1, m_pSnHeadData->Sn_ZMSYO, 12 );
		PF[i].PITM[j] = BP1;	BP1 += ( 12+1 );
	}
	i++;

// êÆóùî‘çÜ
	memset( buf, '\0', sizeof( buf ));
	l_unpac( buf, m_pSnHeadData->Sn_SEIRI, 8 );
	len = (int)strlen( buf );
	str.Empty();
	for( j = 8; j > 0; j-- ){
		if( len == j ){
			str += buf;		break;
		}
		str += ' ';
	}
	for( j = 0; j < 2; j++ ){

		wsprintf( BP1, "%s", str );
		PF[i].PITM[j] = BP1;	BP1 += ( 8+1 );
	}
	i++;

// â€ê≈ä˙ä‘
	int gengo1 = 0, gengo2 = 0;
	memset( yymmdd, '\0', sizeof(yymmdd) );
	if( IsJcContinuePrint(false) ){
		m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	}
	else{
		//m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2],&gengo1);
	}
	for( j = k = 0; k < 2; k++ ){  
		for( x = 0; x < 3; j++, x++, BP1 += 3 ) {
			sw = 0;
			if( x == 0 ){
				if( yymmdd[x] )	sw = 1;
				else{
					if( yymmdd[x+1] )	sw = 1;
				}
			}
			else{
				if( yymmdd[x] )	sw = 1;
			}
			if( sw )
				wsprintf( BP1, "%02X", yymmdd[x] );
			else
				memset( BP1, '\0', 2 );
			PF[i].PITM[j] = BP1;
		}
	}
	i++;
	memset( yymmdd, '\0', sizeof(yymmdd) );
	if( IsJcContinuePrint(false) ){
		m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	}
	else{
		//m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo2);
	}
	for( j = k = 0; k < 2; k++ ){  
		for( x = 0; x < 3; j++, x++, BP1 += 3 ) {
			sw = 0;
			if( x == 0 ){
				if( yymmdd[x] )	sw = 1;
				else{
					if( yymmdd[x+1] )	sw = 1;
				}
			}
			else{
				if( yymmdd[x] )	sw = 1;
			}
			if( sw )
				wsprintf( BP1, "%02X", yymmdd[x] );
			else
				memset( BP1, '\0', 2 );
			PF[i].PITM[j] = BP1;
		}
	}
	i++;

	if(IsSwitch_HeiseiReiwa(false)){
		int ii = 0;
		for (j = 0; j < 2;j++) {
			ii = i;
			if (gengo1 == ID_ICSSH_REIWA_GENGO) {
				memmove(BP1, "    ", 4);
				PF[ii++].PITM[j] = (char *)0;
				PF[ii++].PITM[j] = BP1;
				BP1 += 5;
			}
			else if (gengo1 == ID_ICSSH_HEISEI_GENGO) {
				memmove(BP1, "    ", 4);
				PF[ii++].PITM[j] = BP1;
				PF[ii++].PITM[j] = (char *)0;
				BP1 += 5;
			}
			else {
				PF[ii++].PITM[j] = (char *)0;
				PF[ii++].PITM[j] = (char *)0;
			}

			if (gengo2 == ID_ICSSH_REIWA_GENGO) {
				memmove(BP1, "    ", 4);
				PF[ii++].PITM[j] = (char *)0;
				PF[ii++].PITM[j] = BP1;
				BP1 += 5;
			}
			else if (gengo2 == ID_ICSSH_HEISEI_GENGO) {
				memmove(BP1, "    ", 4);
				PF[ii++].PITM[j] = BP1;
				PF[ii++].PITM[j] = (char *)0;
				BP1 += 5;
			}
			else {
				PF[ii++].PITM[j] = (char *)0;
				PF[ii++].PITM[j] = (char *)0;
			}
		}
		i = ii;
	}


// íÜä‘ê\çêÇÃåvéZä˙ä‘
	gengo1 = gengo2 = 0;
	memset( yymmdd, '\0', sizeof(yymmdd) );
	if( IsJcContinuePrint(false) ){
		m_pSnHeadData->GetYmdData(ID_ICSSH_CHUUKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	}
	else{
		//m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1);
	}
	for( j = k = 0; k < 2; k++ ){  
		for( x = 0; x < 3; j++, x++, BP1 += 3 ) {
			sw = 0;
			if( x == 0 ){
				if( yymmdd[x] )	sw = 1;
				else{
					if( yymmdd[x+1] )	sw = 1;
				}
			}
			else{
				if( yymmdd[x] )	sw = 1;
			}
			if( sw )
				wsprintf( BP1, "%02X", yymmdd[x] );
			else
				memset( BP1, '\0', 2 );
			PF[i].PITM[j] = BP1;
		}
	}
	i++;
	memset( yymmdd, '\0', sizeof(yymmdd) );
	if( IsJcContinuePrint(false) ){
		m_pSnHeadData->GetYmdData(ID_ICSSH_CHUUKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	}
	else{
		//m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo2);
	}
	for( j = k = 0; k < 2; k++ ){  
		for( x = 0; x < 3; j++, x++, BP1 += 3 ) {
			sw = 0;
			if( x == 0 ){
				if( yymmdd[x] )	sw = 1;
				else{
					if( yymmdd[x+1] )	sw = 1;
				}
			}
			else{
				if( yymmdd[x] )	sw = 1;
			}
			if( sw )
				wsprintf( BP1, "%02X", yymmdd[x] );
			else
				memset( BP1, '\0', 2 );
			PF[i].PITM[j] = BP1;
		}
	}
	i++;

	if(IsSwitch_HeiseiReiwa(false)){
		int ii = 0;
		for (j = 0; j < 2; j++) {
			ii = i;

			if (gengo1 == ID_ICSSH_REIWA_GENGO) {
				memmove(BP1, "    ", 4);
				PF[ii++].PITM[j] = (char *)0;
				PF[ii++].PITM[j] = BP1;
				BP1 += 5;
			}
			else if (gengo1 == ID_ICSSH_HEISEI_GENGO) {
				memmove(BP1, "    ", 4);
				PF[ii++].PITM[j] = BP1;
				PF[ii++].PITM[j] = (char *)0;
				BP1 += 5;
			}
			else {
				PF[ii++].PITM[j] = (char *)0;
				PF[ii++].PITM[j] = (char *)0;
			}

			if (gengo2 == ID_ICSSH_REIWA_GENGO) {
				memmove(BP1, "    ", 4);
				PF[ii++].PITM[j] = (char *)0;
				PF[ii++].PITM[j] = BP1;
				BP1 += 5;
			}
			else if (gengo2 == ID_ICSSH_HEISEI_GENGO) {
				memmove(BP1, "    ", 4);
				PF[ii++].PITM[j] = BP1;
				PF[ii++].PITM[j] = (char *)0;
				BP1 += 5;
			}
			else {
				PF[ii++].PITM[j] = (char *)0;
				PF[ii++].PITM[j] = (char *)0;
			}
		}
		i = ii;
	}

	return( _xprn( 1, dp, PF, 0, pDC, 0, 0x0102 ));
}

//-----------------------------------------------------------------------------
// ëOîNé¿ê—Ç…ÇÊÇÈíÜä‘ê\çê(é¿ïîÇS)
//-----------------------------------------------------------------------------
int CH26HyoView::_dprn4( int dp, CDC* pDC, char *BP )
{
	int		i, j, k, x;
	char	buf[128];
	int		len, sw;
	char	val[6];
	PBUF	PF[30];

	i = 0;
	memset( BP, '\0', KSK_BUF_SIZE );
	char *BP1 = BP;

// ëOâ€ê≈ä˙ä‘
	int gengo1 = 0, gengo2 = 0;
	char	yymmdd[4]={0};
	if( IsJcContinuePrint(false) ){
		m_pSnChskData->GetYmdData(ID_ZENKI_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	}
	else{
		//m_pSnChskData->GetYmdDataGen(ID_ZENKI_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		m_pSnChskData->GetYmdDataGen(ID_ZENKI_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1);
	}
	for( j = k = 0; k < 2; k++ ){  
		for( x = 0; x < 3; j++, x++, BP1 += 3 ) {
			sw = 0;
			if( x == 0 ){
				if( yymmdd[x] )	sw = 1;
				else{
					if( yymmdd[x+1] )	sw = 1;
				}
			}
			else{
				if( yymmdd[x] )	sw = 1;
			}
			if( sw )
				wsprintf( BP1, "%02X", yymmdd[x] );
			else
				memset( BP1, '\0', 2 );
			PF[i].PITM[j] = BP1;
		}
	}
	i++;
	memset( yymmdd, '\0', sizeof(yymmdd) );
	if( IsJcContinuePrint(false) ){
		m_pSnChskData->GetYmdData(ID_ZENKI_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	}
	else{
		//m_pSnChskData->GetYmdDataGen(ID_ZENKI_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		m_pSnChskData->GetYmdDataGen(ID_ZENKI_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2],&gengo2);
	}
	for( j = k = 0; k < 2; k++ ){  
		for( x = 0; x < 3; j++, x++, BP1 += 3 ) {
			sw = 0;
			if( x == 0 ){
				if( yymmdd[x] )	sw = 1;
				else{
					if( yymmdd[x+1] )	sw = 1;
				}
			}
			else{
				if( yymmdd[x] )	sw = 1;
			}
			if( sw )
				wsprintf( BP1, "%02X", yymmdd[x] );
			else
				memset( BP1, '\0', 2 );
			PF[i].PITM[j] = BP1;
		}
	}
	i++;

	if(IsSwitch_HeiseiReiwa(false)){
		int ii = 0;
		for (j = 0; j < 2; j++) {
			ii = i;
			if (gengo1 == ID_ICSSH_REIWA_GENGO) {
				memmove(BP1, "    ", 4);
				PF[ii++].PITM[j] = (char *)0;
				PF[ii++].PITM[j] = BP1;
				BP1 += 5;
			}
			else if (gengo1 == ID_ICSSH_HEISEI_GENGO) {
				memmove(BP1, "    ", 4);
				PF[ii++].PITM[j] = BP1;
				PF[ii++].PITM[j] = (char *)0;
				BP1 += 5;
			}
			else {
				PF[ii++].PITM[j] = (char *)0;
				PF[ii++].PITM[j] = (char *)0;
			}

			if (gengo2 == ID_ICSSH_REIWA_GENGO) {
				memmove(BP1, "    ", 4);
				PF[ii++].PITM[j] = (char *)0;
				PF[ii++].PITM[j] = BP1;
				BP1 += 5;
			}
			else if (gengo2 == ID_ICSSH_HEISEI_GENGO) {
				memmove(BP1, "    ", 4);
				PF[ii++].PITM[j] = BP1;
				PF[ii++].PITM[j] = (char *)0;
				BP1 += 5;
			}
			else {
				PF[ii++].PITM[j] = (char *)0;
				PF[ii++].PITM[j] = (char *)0;
			}
		}
		i = ii;
	}

// èCê≥ÅEçXê≥ÅEåàíËÇÃîNåéì˙
	gengo1 = gengo2 = 0;
	memset( yymmdd, '\0', sizeof(yymmdd) );
	if( IsJcContinuePrint(false) ){
		m_pSnChskData->GetYmdData(ID_SYUZEI_DECIDE_DAY, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	}
	else{
		//m_pSnChskData->GetYmdDataGen(ID_SYUZEI_DECIDE_DAY, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		m_pSnChskData->GetYmdDataGen(ID_SYUZEI_DECIDE_DAY, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1);
	}
	for( j = k = 0; k < 2; k++ ){  
		for( x = 0; x < 3; j++, x++, BP1 += 3 ) {
			sw = 0;
			if( x == 0 ){
				if( yymmdd[x] )	sw = 1;
				else{
					if( yymmdd[x+1] )	sw = 1;
				}
			}
			else{
				if( yymmdd[x] )	sw = 1;
			}
			if( sw )
				wsprintf( BP1, "%02X", yymmdd[x] );
			else
				memset( BP1, '\0', 2 );
			PF[i].PITM[j] = BP1;
		}
	}
	i++;

	if(IsSwitch_HeiseiReiwa(false)){
		int ii = 0;
		for (j = 0; j < 2; j++) {
			ii = i;
			if (gengo1 == ID_ICSSH_REIWA_GENGO) {
				memmove(BP1, "    ", 4);
				PF[ii++].PITM[j] = (char *)0;
				PF[ii++].PITM[j] = BP1;
				BP1 += 5;
			}
			else if (gengo1 == ID_ICSSH_HEISEI_GENGO) {
				memmove(BP1, "    ", 4);
				PF[ii++].PITM[j] = BP1;
				PF[ii++].PITM[j] = (char *)0;
				BP1 += 5;
			}
			else {
				PF[ii++].PITM[j] = (char *)0;
				PF[ii++].PITM[j] = (char *)0;
			}
		}
		i = ii;
	}

// ëOâ€ê≈ä˙ä‘ÇÃè¡îÔê≈äz
	len = val_set( buf, m_pSnHonpyoData->Sn_ZNOFZ );
	for( j = 0; j < 2; j++ ){  
		memcpy( BP1, buf, len );
		PF[i].PITM[j] = BP1;	BP1 += (len+1);
	}
	i++;

// åéêîä∑éZ
	memset( buf, '\0', sizeof( buf ));
	if( m_pSnChskData->Sn_BSMON ){
		wsprintf( buf, "%2d", m_pSnChskData->Sn_BSMON );
	}
	len = (int)strlen( buf );
	for( j = 0; j < 2; j++ ){
		memmove( BP1, buf, len );
		PF[i].PITM[j] = BP1;
		BP1 += ( len+1 );
	}
	i++;
	memset( buf, '\0', sizeof( buf ));
	if( m_pSnChskData->Sn_BBMON ){
		wsprintf( buf, "%2d", m_pSnChskData->Sn_BBMON );
	}
	len = (int)strlen( buf );
	for( j = 0; j < 2; j++ ){
		memmove( BP1, buf, len );
		PF[i].PITM[j] = BP1;
		BP1 += ( len+1 );
	}
	i++;

// èCê≥ê\çê
	if( m_pSnHeadData->Sn_SKKBN == 4 ){
		for( x = 0; x < 5; x++, i++ ){
			memset( val, '\0', 6 );
			switch( x ){
			case 0:		// è¡îÔê≈Å@Ç±ÇÃê\çêëOÇÃê≈äz
				memmove( val, m_pSnChskData->Sn_ZSKZN, 6 );	break;
			case 1:		// è¡îÔê≈Å@Ç±ÇÃê\çêÇ…ÇÊÇËëùâ¡Ç∑ÇÈê≈äz
				memmove( val, m_pSnChskData->Sn_ZSKAD, 6 );	break;
			case 2:		// ínï˚è¡îÔê≈Å@Ç±ÇÃê\çêëOÇÃê≈äz
				memmove( val, m_pSnChskData->Sn_ZSTZN, 6 );	break;
			case 3:		// ínï˚è¡îÔê≈Å@Ç±ÇÃê\çêÇ…ÇÊÇËëùâ¡Ç∑ÇÈê≈äz
				memmove( val, m_pSnChskData->Sn_ZSTAD, 6 );	break;
			case 4:		// çáåvî[ïtê≈äz
				memmove( val, m_pSnChskData->Sn_ZSKTZ, 6 );	break;
			}
			len = val_set( buf, (unsigned char*)val );
			for( j = 0; j < 2; j++ ){  
				memcpy( BP1, buf, len );
				PF[i].PITM[j] = BP1;	BP1 += (len+1);
			}
		}
	}
	else{
		memset( buf, '\0', sizeof( buf ));
		for( x = 0; x < 5; x++, i++ ){
			for( j = 0; j < 2; j++ ){  
				memcpy( BP1, buf, 11 );
				PF[i].PITM[j] = BP1;	BP1 += (11+1);
			}
		}
	}

// î[ïtÇ∑Ç◊Ç´è¡îÔê≈äz
	len = val_set( buf, m_pSnHonpyoData->Sn_EDNOFZ );
	for( j = 0; j < 2; j++ ){  
		memcpy( BP1, buf, len );
		PF[i].PITM[j] = BP1;	BP1 += (len+1);
	}
	i++;

// î[ïtÇ∑Ç◊Ç´ínï˚è¡îÔê≈äz
	len = val_set( buf, m_pSnHonpyoData->Sn_TEDNOF );
	for( j = 0; j < 2; j++ ){  
		memcpy( BP1, buf, len );
		PF[i].PITM[j] = BP1;	BP1 += (len+1);
	}
	i++;

// çáåvî[ïtê≈äz
	len = val_set( buf, m_pSnChskData->Sn_KTNFZ );
	for( j = 0; j < 2; j++ ){  
		memcpy( BP1, buf, len );
		PF[i].PITM[j] = BP1;	BP1 += (len+1);
	}
	i++;

	return( _xprn( 1, dp, PF, 0, pDC, 0, 0x0102 ));
}

//-----------------------------------------------------------------------------
// ã‡äzÇÉZÉbÉgÅiíÜä‘ê\çêópÅj
//-----------------------------------------------------------------------------
int CH26HyoView::val_set( char *bf, unsigned char *val )
{
	char	num1[6], num2[6];
	char	buf[128];
	CString str1, str2, ans;
	int		js, len;

	memset( bf, '\0', 128 );
	memset( buf, '\0', sizeof( buf ));
	memcpy( num1, val, 6 );
	m_Arith.l_input( num2, "100" );
	m_Arith.l_div( num1, num1, num2 );
	m_Arith.l_print( buf, num1, FM10H );
	str1 = buf;
	ans.Empty();
	for( int i = 0; i < 2; i++ ){
		if( i == 0 )	js = 5;
		else			js = 3;
		ans += str1.Left( js );
		ans += " ";
		str2 = str1.Mid( js );
		str1 = str2;
	}
	ans += str1;
	wsprintf( bf, "%s", ans );
	len = (int)strlen( bf );

	return len;
}

//-----------------------------------------------------------------------------
// ìdòbî‘çÜÇÉZÉbÉgÅiíÜä‘ê\çêópÅj
//-----------------------------------------------------------------------------
void CH26HyoView::ZTel_Set( char *buf, char *telno, char sw, int len, int lng )
{
	char	*fp, tel[128], ttel[4][32];
	int		cnt;
	char	bf1[32], bf2[32], bf3[32];

	memset( bf1, '\0', 32 );
	memset( bf2, '\0', 32 );
	memset( bf3, '\0', 32 );
	memset( ttel, '\0', 32*4 );
	memset( tel, '\0', sizeof( tel ));
	memcpy( tel, telno, lng );

	if( sw ){
		wsprintf( buf, "       Å|     Å|" );
		return;
	}

	cnt = 2;
	if( fp = (char *) strrchr( tel, '-' ) ) {
		strcpy_s( ttel[cnt--], 32, (fp+1) );			/* å¬êlî‘çÜ */
		*fp = '\0';
		if( fp = (char *) strrchr( tel, '-' ) ) {
			strcpy_s( ttel[cnt--], 32, (fp+1) );		/* ésì‡ã«î‘ */
			*fp = '\0';
			strcpy_s( ttel[cnt], 32, tel );				/* ésäOã«î‘ */
		}
		else	strcpy_s( ttel[cnt], 32, tel );			/* ésì‡ã«î‘ */

		while( 1 ) {
			switch( cnt ) {
				case 0:	/* ésäOã«î‘ */
					memcpy( bf1, ttel[cnt], 32 );
					++cnt;	continue;
				case 1:	/* ésì‡ã«î‘ */
					memcpy( bf2, ttel[cnt], 32 );
					++cnt;	continue;
				case 2:	/* å¬êlî‘çÜ */
					memcpy( bf3, ttel[cnt], 32 );
					++cnt;	continue;
				default:	break;
			}
			break;
		}
		if(( kjlen( bf1, 32 ) > 6 )||( kjlen( bf2, 32 ) > 4 )||( kjlen( bf3, 32 ) > 4 )){
			wsprintf( buf, "%sÅ|%sÅ|%s", bf1, bf2, bf3 );
			if( kjlen( buf, 32 ) > 22 )	buf[22] = 0;
		}
		else	wsprintf( buf, "%6s Å|%4s Å|%4s", bf1, bf2, bf3 );
	}
	else{
		if( kjlen( telno, 30 ) == 0 )
			wsprintf( buf, "       Å|     Å|" );
		else
			memcpy( buf, telno, 26 );
	}
}

//-----------------------------------------------------------------------------
// êÿéÊê¸
//-----------------------------------------------------------------------------
void CH26HyoView::Lin_Print()
{
	long	sx, sy, ex, ey;
	long	dotx, doty;
	dotx = doty = 0L;
	//	íPà ÅF1/100mm Ç…Ç∑ÇÈÅB
	doty = (m_pPrintCmInfo->dat1 * 10L);
	dotx = (m_pPrintCmInfo->dat2 * 10L);
	for( int i = 0; i < 16; i++ ){ 
		sx = sy = ex = ey = 0L;
		switch( i ){
		case  0:	sx =     0;	sy =   400;	ex =   500;	ey =   400;	break;	// â°	íÒèoópÅ@è„ç∂
		case  1:	sx =     0;	sy =   250;	ex =     0;	ey =   550;	break;	// èc
		case  2:	sx = 19400;	sy =   400;	ex = 19900;	ey =   400;	break;	// â°	íÒèoópÅ@è„âE
		case  3:	sx = 19900;	sy =   250;	ex = 19900;	ey =   550;	break;	// èc
		case  4:	sx =     0;	sy =  9850;	ex =   500;	ey =  9850;	break;	// â°	íÒèoópÅ@â∫ç∂
		case  5:	sx =     0;	sy =  9700;	ex =     0;	ey = 10000;	break;	// èc
		case  6:	sx = 19400;	sy =  9850;	ex = 19900;	ey =  9850;	break;	// â°	íÒèoópÅ@â∫âE
		case  7:	sx = 19900;	sy =  9700;	ex = 19900;	ey = 10000;	break;	// èc
		case  8:	sx =     0;	sy = 14850;	ex =   500;	ey = 14850;	break;	// â°	çTópÅ@Å@è„ç∂
		case  9:	sx =     0;	sy = 14700;	ex =     0;	ey = 15000;	break;	// èc
		case 10:	sx = 19400;	sy = 14850;	ex = 19900;	ey = 14850;	break;	// â°	çTópÅ@Å@è„âE
		case 11:	sx = 19900;	sy = 14700;	ex = 19900;	ey = 15000;	break;	// èc
		case 12:	sx =     0;	sy = 24300;	ex =   500;	ey = 24300;	break;	// â°	çTópÅ@Å@â∫ç∂
		case 13:	sx =     0;	sy = 24150;	ex =     0;	ey = 24450;	break;	// èc
		case 14:	sx = 19400;	sy = 24300;	ex = 19900;	ey = 24300;	break;	// â°	çTópÅ@Å@â∫âE
		case 15:	sx = 19900;	sy = 24150;	ex = 19900;	ey = 24450;	break;	// èc
		}
		sx += dotx;	sy += doty;	ex += dotx;	ey += doty;
		m_pVprn->DrawSline( 0, LKnrW0, sx, sy, ex, ey );
	}
}

//-----------------------------------------------------------------------------
// ëOîNé¿ê—Ç…ÇÊÇÈíÜä‘ê\çêÇQ
//-----------------------------------------------------------------------------
int CH26HyoView::data_mast_EX( CDC* pDC )
{
	int		dp, st;
	
	char	*BP = NULL;
	if( (BP = new char[KSK_BUF_SIZE]) == NULL ){
		return -1;
	}
	char *BP1 = BP;

	st = 0;

	for( dp = 1; dp <= 5; ++dp ) {	
		switch( dp ) {
			case	1:	st = _dprn1_EX( dp, pDC, BP1 );		break;
			case	2:	st = _dprn2_EX( dp, pDC, BP1 );		break;
			case	3:	st = _dprn3_EX( dp, pDC, BP1 );		break;
			case	4:	st = _dprn4_EX( dp, pDC, BP1 );		break;
		}
		if( st )	break;
	}

	delete( BP );

//2015.12.03 INSERT START
	if(( m_ShRevType >= ID_ICSSH_MYNUMBER_REVTYPE ) && m_pSnHeadData->IsSoleProprietor()){
		m_pVprn->DrawSline( 0, LKnrW0, 5750, 11300, 12300, 10600 ); //å¬êlî‘çÜóìÇ…éŒê¸
	}
//2015.12.03 INSERT END

//Åö//[12'08.10]///ìdéqê\çêäÆóπçœ 
   if( *m_pShinInfo->pFinish && *m_pShinInfo->pFinishSw && strlen(m_pShinInfo->pFinishPrn) ){
		long	PH  = 2540L / 10L;		// 1 character
		long	PV1 = 2540L / 6L;		// 1/6 LF
  		long YPosition = ( PV1 * 12L );
		long XPosition = ( PH * 15L ) + ( PH / 2 );
		int	Gothic095 = m_pVprn->RegisterFont( 90, 0, 0, "ÇlÇrÅ@ÉSÉVÉbÉN" );
		int	Gothic108 = m_pVprn->RegisterFont( MAKELONG( 108, 1 ), 0, 0, "ÇlÇrÅ@ÉSÉVÉbÉN" );
		struct	_SF_PAR		SfPar;
		memset( (char *)&SfPar, '\0', sizeof( struct _SF_PAR ) );
		SfPar.sf_fcsg =	( SFp_xl | SFp_yb );					// ÉtÉ@ÉìÉNÉVÉáÉìÉTÉCÉì
		SfPar.sf_hvsg = 0;										// èoóÕï˚å¸
		SfPar.sf_xfld = (11L * PH);								// â°èoóÕóÃàÊïù
		SfPar.sf_yfld =	PV1;									// ècèoóÕóÃàÊïù
		SfPar.sf_mrgn[0] = 0;									// óÃàÊì‡É}Å[ÉWÉìÅ@ç∂
		SfPar.sf_mrgn[1] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@âE
		SfPar.sf_mrgn[2] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@è„
		SfPar.sf_mrgn[3] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@â∫
		SfPar.sf_kpit = 0;										// ï∂éöÉsÉbÉ`ïùÅ@X0.1Œﬂ≤›ƒ
		SfPar.sf_ksiz =	Gothic108;								// ï∂éöÉTÉCÉYÅ@Å@X0.1Œﬂ≤›ƒ		
		SfPar.sf_erto = 0;										// ï∂éöägëÂèkè¨ó¶
		if( m_pVprn->DrawString( 0, XPosition, YPosition, _T("ìdéqê\çêçœ"), &SfPar ) )	return( 1 );

  		YPosition = ( PV1 * 12L );
		XPosition = ( PH *  23L ) + ( PH / 2 );
		memset( (char *)&SfPar, '\0', sizeof( struct _SF_PAR ) );
		SfPar.sf_fcsg =	( SFp_xl | SFp_yb );					// ÉtÉ@ÉìÉNÉVÉáÉìÉTÉCÉì
		SfPar.sf_hvsg = 0;										// èoóÕï˚å¸
		SfPar.sf_xfld = (35L * PH);								// â°èoóÕóÃàÊïù
		SfPar.sf_yfld =	PV1;									// ècèoóÕóÃàÊïù
		SfPar.sf_mrgn[0] = 0;									// óÃàÊì‡É}Å[ÉWÉìÅ@ç∂
		SfPar.sf_mrgn[1] = 10;									// Å@Å@ÅVÅ@Å@Å@Å@Å@âE
		SfPar.sf_mrgn[2] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@è„
		SfPar.sf_mrgn[3] = 0;									// Å@Å@ÅVÅ@Å@Å@Å@Å@â∫
		SfPar.sf_kpit = 0;										// ï∂éöÉsÉbÉ`ïùÅ@X0.1Œﬂ≤›ƒ
		SfPar.sf_ksiz =	Gothic095;								// ï∂éöÉTÉCÉYÅ@Å@X0.1Œﬂ≤›ƒ		
		SfPar.sf_erto = 0;										// ï∂éöägëÂèkè¨ó¶
		if( m_pVprn->DrawString( 0, XPosition, YPosition, m_pShinInfo->pFinishPrn, &SfPar ) )	return( 1 );

		m_pVprn->DeleteRegFont( Gothic095 );
		m_pVprn->DeleteRegFont( Gothic108 );

	}
///////////////////

	return( st );
}

#ifdef _20150127_EXTEND_
//-----------------------------------------------------------------------------
// ëOîNé¿ê—Ç…ÇÊÇÈíÜä‘ê\çêÇQ(é¿ïîÇP)
//-----------------------------------------------------------------------------
int CH26HyoView::_dprn1_EX( int dp, CDC* pDC, char *BP )
{
	int		i, j;
	char	buf[128];
	char	bf1[10], bf2[10], bf3[10];
	int		len;
	PBUF	PF[30];

	i = 0;
	memset( BP, '\0', KSK_BUF_SIZE );
	char *BP1 = BP;

// î[ê≈ín
	for( j = 0; j < 1; j++ ){
		memmove( BP1, &m_pSnHeadData->Sn_NOZEI[0], 40 );
		PF[i].PITM[j] = BP1;	BP1 += ( 40+1 );
	}
	i++;
	for( j = 0; j < 1; j++ ){
		memmove( BP1, &m_pSnHeadData->Sn_NOZEI[40], 40 );
		PF[i].PITM[j] = BP1;	BP1 += ( 40+1 );
	}
	i++;

// Tel
	memset( buf, '\0', sizeof( buf ));
	memset( bf1, '\0', sizeof( bf1 ));
	memset( bf2, '\0', sizeof( bf2 ));
	memset( bf3, '\0', sizeof( bf3 ));
	len = kjlen( m_pSnHeadData->Sn_TELNO1, 6 );
	memmove( bf1, m_pSnHeadData->Sn_TELNO1, len );
	len = kjlen( m_pSnHeadData->Sn_TELNO2, 4 );
	memmove( bf2, m_pSnHeadData->Sn_TELNO2, len );
	len = kjlen( m_pSnHeadData->Sn_TELNO3, 4 );
	memmove( bf3, m_pSnHeadData->Sn_TELNO3, len );
	wsprintf( buf, "%6s Å|%4s Å|%4s", bf1, bf2, bf3 );
	len = (int)strlen( buf );
	for( j = 0; j < 1; j++ ){
		memmove( BP1, buf, len );
		PF[i].PITM[j] = BP1;	BP1 += ( len+1 );
	}
	i++;

// ñºèÃñîÇÕâÆçÜ ÉtÉäÉKÉi
	for( j = 0; j < 1; j++ ){
		memmove( BP1, m_pSnHeadData->Sn_COFRI, 80 );
		PF[i].PITM[j] = BP1;	BP1 += ( 80+1 );
	}
	i++;

// ñºèÃñîÇÕâÆçÜ
	for( j = 0; j < 1; j++ ){
		memmove( BP1, m_pSnHeadData->Sn_CONAM, 40 );
		PF[i].PITM[j] = BP1;	BP1 += ( 40+1 );
	}
	i++;

// ë„ï\é“éÅñº ÉtÉäÉKÉi
	for( j = 0; j < 1; j++ ){
		memmove( BP1, m_pSnHeadData->Sn_DIFRI, 80 );
		PF[i].PITM[j] = BP1;	BP1 += ( 80+1 );
	}
	i++;

// ë„ï\é“éÅñº
	for( j = 0; j < 1; j++ ){
		memmove( BP1, m_pSnHeadData->Sn_DIHYO, 40 );
		PF[i].PITM[j] = BP1;	BP1 += ( 40+1 );
	}
	i++;
	
	return( _xprn( 2, dp, PF, 0, pDC, 0, 0x0101) );
}
#else
//-----------------------------------------------------------------------------
// ëOîNé¿ê—Ç…ÇÊÇÈíÜä‘ê\çêÇQ(é¿ïîÇP)
//-----------------------------------------------------------------------------
int CH26HyoView::_dprn1_EX( int dp, CDC* pDC, char *BP )
{
	int		i, j;
	char	buf[128];
	char	bf1[10], bf2[10], bf3[10];
	int		len;
	PBUF	PF[30];

	i = 0;
	memset( BP, '\0', KSK_BUF_SIZE );
	char *BP1 = BP;

	// î[ê≈ín
	if( !m_pPrintCmInfo->OP3 ){	
		for( j = 0; j < 1; j++ ){
			memmove( BP1, &m_pSnHeadData->Sn_NOZEI[0], 40 );
			PF[i].PITM[j] = BP1;	BP1 += ( 40+1 );
		}
		i++;
		for( j = 0; j < 1; j++ ){
			memmove( BP1, &m_pSnHeadData->Sn_NOZEI[40], 40 );
			PF[i].PITM[j] = BP1;	BP1 += ( 40+1 );
		}
		i++;
	}
	else{
		pbclr( 0, 1, &PF[i++] );
		pbclr( 0, 1, &PF[i++] );
	}

	// Tel
	if( !m_pPrintCmInfo->OPE ){
		memset( buf, '\0', sizeof( buf ));
		memset( bf1, '\0', sizeof( bf1 ));
		memset( bf2, '\0', sizeof( bf2 ));
		memset( bf3, '\0', sizeof( bf3 ));
		len = kjlen( m_pSnHeadData->Sn_TELNO1, 6 );
		memmove( bf1, m_pSnHeadData->Sn_TELNO1, len );
		len = kjlen( m_pSnHeadData->Sn_TELNO2, 4 );
		memmove( bf2, m_pSnHeadData->Sn_TELNO2, len );
		len = kjlen( m_pSnHeadData->Sn_TELNO3, 4 );
		memmove( bf3, m_pSnHeadData->Sn_TELNO3, len );
		wsprintf( buf, "%6s Å|%4s Å|%4s", bf1, bf2, bf3 );
		len = (int)strlen( buf );
		for( j = 0; j < 1; j++ ){
			memmove( BP1, buf, len );
			PF[i].PITM[j] = BP1;	BP1 += ( len+1 );
		}
		i++;
	}
	else{
		pbclr( 0, 1, &PF[i++] );
	}

	// ñºèÃñîÇÕâÆçÜ ÉtÉäÉKÉi
	if( !m_pPrintCmInfo->OPF ){	
		for( j = 0; j < 1; j++ ){
			memmove( BP1, m_pSnHeadData->Sn_COFRI, 80 );
			PF[i].PITM[j] = BP1;	BP1 += ( 80+1 );
		}
		i++;
	}
	else{
		pbclr( 0, 1, &PF[i++] );
	}

	// ñºèÃñîÇÕâÆçÜ
	if( !m_pPrintCmInfo->OP4 ){
		for( j = 0; j < 1; j++ ){
			memmove( BP1, m_pSnHeadData->Sn_CONAM, 40 );
			PF[i].PITM[j] = BP1;	BP1 += ( 40+1 );
		}
		i++;
	}
	else{
		pbclr( 0, 1, &PF[i++] );
	}

//2015.10.20 INSERT START
//2015.12.03 UPDATE START
	//if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
	//	CString myno_str;
	//	if( m_pSnHeadData->GetMyNumber( myno_str ) == 0 ){
	//		for( j = 0; j < 1; j++ ){
	//			if( m_pSnHeadData->IsSoleProprietor() == TRUE ){
	//				myno_str = ' ' + myno_str;
	//			}
	//			memmove( BP1, myno_str, 13 );
	//			PF[i].PITM[j] = BP1; BP1 += ( 13+1 );
	//		}
	//		i++;
	//	}else{
	//		pbclr( 0, 1, &PF[i++] );
	//	}
	//}

//-- '15.12.19 --
//	if( m_ShRevType >= ID_ICSSH_TKKZSIIRE_REVTYPE ){
//		if( m_ShRevType >= ID_ICSSH_MYNUMBER_REVTYPE ){
//			if(m_pSnHeadData->IsSoleProprietor()){
//				pbclr( 0, 1, &PF[i++] );
//			}else{
//				CString myno_str,myno_prn;
//				if( m_pSnHeadData->GetMyNumber( myno_str ) == 0 ){
//					if( m_pPset->psSigCoprn&0x4000){
//						myno_prn = mnsub.MakePrintKojinNo( myno_str,  m_pVprn->Get_KojinPrintFlg() );
//					}else{
//						myno_prn = myno_str;
//					}
//					if( m_pSnHeadData->IsSoleProprietor() == TRUE ){
//						myno_prn = ' ' + myno_prn;
//					}
//					for( j = 0; j < 1; j++ ){
//						memmove( BP1, myno_prn, 13 );
//						PF[i].PITM[j] = BP1; BP1 += ( 13+1 );
//					}
//					i++;
//				}else{
//					pbclr( 0, 1, &PF[i++] );
//				}
//			}
//		}else{
//			pbclr( 0, 1, &PF[i++] );
//		}
//	}
//---------------
	if( m_ShRevType >= ID_ICSSH_MYNUMBER_REVTYPE ){
		if(m_pSnHeadData->IsSoleProprietor()){
			pbclr( 0, 1, &PF[i++] );
		}else{

			CICSMNSub8 mnsub;
			CString myno_str,myno_prn;
			if( m_pSnHeadData->GetMyNumber( myno_str ) == 0 ){
				if( m_pPset->psSigCoprn&0x4000){
					myno_prn = mnsub.MakePrintKojinNo( myno_str,  m_pVprn->Get_KojinPrintFlg() );
				}else{
					myno_prn = myno_str;
				}
				if( m_pSnHeadData->IsSoleProprietor() == TRUE ){
					myno_prn = ' ' + myno_prn;
				}
				for( j = 0; j < 1; j++ ){
					memmove( BP1, myno_prn, 13 );
					PF[i].PITM[j] = BP1; BP1 += ( 13+1 );
				}
				i++;
			}else{
				pbclr( 0, 1, &PF[i++] );
			}
		}
	}
//---------------
//2015.12.03 UPDATE END
//2015.10.20 INSERT END

	// ë„ï\é“éÅñº ÉtÉäÉKÉi
	if( !m_pPrintCmInfo->OPG ){
		for( j = 0; j < 1; j++ ){
			memmove( BP1, m_pSnHeadData->Sn_DIFRI, 80 );
			PF[i].PITM[j] = BP1;	BP1 += ( 80+1 );
		}
		i++;
	}
	else{
		pbclr( 0, 1, &PF[i++] );
	}

	// ë„ï\é“éÅñº
	if( !m_pPrintCmInfo->OP5 ){
		for( j = 0; j < 1; j++ ){
			memmove( BP1, m_pSnHeadData->Sn_DIHYO, 40 );
			PF[i].PITM[j] = BP1;	BP1 += ( 40+1 );
		}
		i++;
	}
	else{
		pbclr( 0, 1, &PF[i++] );
	}
	
	return( _xprn( 2, dp, PF, 0, pDC, 0, 0x0101) );
}
#endif

//-----------------------------------------------------------------------------
// ëOîNé¿ê—Ç…ÇÊÇÈíÜä‘ê\çêÇQ(é¿ïîÇQ)
//-----------------------------------------------------------------------------
int CH26HyoView::_dprn2_EX( int dp, CDC* pDC, char *BP )
{
	int		i, j;
	int		len;
	char	buf[128];
	PBUF	PF[30];

	i = 0;
	memset( BP, '\0', KSK_BUF_SIZE );
	char *BP1 = BP;

	// éÅñºÅïñ@êlñºí«â¡ 08/04/22
	for( j = 0; j < 1; j++ ){	// è„ãlÇﬂÅ@ñ@êlñº
//2017.07.06 INSERT START
		if( m_pPrintCmInfo->OP2 != 0 ){
			;
		}else
//2017.07.06 INSERT END
		if( m_pPrintCmInfo->OPA == 2 || ( m_pPrintCmInfo->OPA == 1 && m_pPrintCmInfo->OPsign&0x01 ))
			memmove( BP1, m_pShinInfo->pO_DATA->o_ofcnam, 40 );
		PF[i].PITM[j] = BP1;
		BP1 += ( 40+1 );
	}
	i++;

//-- '15.02.28 --
//	for( j = 0; j < 1; j++ ){	// éÅñºÅiéÅñºÅïñ@êlñºÅj
//		int slen = 0;
//		if( m_pPrintCmInfo->OPA == 2 ){
//			memset( buf, '\0', sizeof( buf ));
//			MakeOutZeirishiName( buf, sizeof(buf) );
//			slen = (int)strlen( buf );
//			memmove( BP1, buf, slen );
//		}
//		PF[i].PITM[j] = BP1;
//		BP1 += ( slen+1 );
//	}
//	i++;
//---------------
	// éÅñºÅiéÅñºÅïñ@êlñºÅj
	int slen = 0;
//2017.07.06 INSERT START
	if( m_pPrintCmInfo->OP2 != 0 ){
		;
	}else
//2017.07.06 INSERT END
	if( m_pPrintCmInfo->OPA == 2 ){
		memset( buf, '\0', sizeof( buf ));
		MakeOutZeirishiName( buf, sizeof(buf) );
		slen = (int)strlen( buf );
	}
	if( slen > 40 ){
		pbclr( 0, 1, &PF[i++] );
		for( j=0; j<1; j++ ){
			memmove( BP1, buf, slen );
			PF[i].PITM[j] = BP1;
			BP1 += ( slen+1 );
		}
		i++;
	}
	else if( slen > 0 ){
		for( j=0; j<1; j++ ){
			memmove( BP1, buf, slen );
			PF[i].PITM[j] = BP1;
			BP1 += ( slen+1 );
		}
		i++;
		pbclr( 0, 1, &PF[i++] );
	}
	else{
		pbclr( 0, 1, &PF[i++] );
		pbclr( 0, 1, &PF[i++] );
	}
//---------------

//-- '15.02.28 --
//	for( j = 0; j < 1; j++ ){	// éÅñºorñ@êlñº
//		if( m_pPrintCmInfo->OPA == 2 || ( m_pPrintCmInfo->OPA == 1 && m_pPrintCmInfo->OPsign&0x01 ));
//		else
//			memmove( BP1, m_pPrintCmInfo->Zname, 40 );
//		PF[i].PITM[j] = BP1;
//		BP1 += ( 40+1 );
//	}
//	i++;
//---------------
	slen = 0;
	memset( buf, '\0', sizeof( buf ));
	if( m_pPrintCmInfo->OP2 != 0 ){
		;
	}
	else if( (m_pPrintCmInfo->OPA==2) || ((m_pPrintCmInfo->OPA==1)&&(m_pPrintCmInfo->OPsign&0x01)) ){
		;
	}
	else{
		memset( buf, '\0', sizeof( buf ));
		MakeOutZeirishiName( buf, sizeof(buf) );
		slen = (int)strlen( buf );
	}
	if( slen > 40 ){
		pbclr( 0, 1, &PF[i++] );
		for( j=0; j<1; j++ ){
			memmove( BP1, buf, slen );
			PF[i].PITM[j] = BP1;
			BP1 += ( slen+1 );
		}
		i++;
	}
	else if( slen > 0 ){
		for( j=0; j<1; j++ ){
			memmove( BP1, buf, slen );
			PF[i].PITM[j] = BP1;
			BP1 += ( slen+1 );
		}
		i++;
		pbclr( 0, 1, &PF[i++] );
	}
	else{
		pbclr( 0, 1, &PF[i++] );
		pbclr( 0, 1, &PF[i++] );
	}
//---------------

	// Tel
	memset( buf, '\0', sizeof( buf ));
	ZTel_Set( buf, m_pPrintCmInfo->ZtelP, m_pPrintCmInfo->OP9, 16, 30 );
	len = (int)strlen( buf );
	for( j = 0; j < 1; j++ ){
		memmove( BP1, buf, len );
		PF[i].PITM[j] = BP1;
		BP1 += ( len+1 );
	}
	i++;

	return( _xprn( 2, dp, PF, 0, pDC, 0, 0x0101 ));
}

//-----------------------------------------------------------------------------
// ëOîNé¿ê—Ç…ÇÊÇÈíÜä‘ê\çêÇQ(é¿ïîÇR)
//-----------------------------------------------------------------------------
int CH26HyoView::_dprn3_EX( int dp, CDC* pDC, char *BP )
{
	int		i, j, k, x;
	char	buf[128];
	CString str;
	int		len, sw = 0;
	PBUF	PF[30];

	i = 0;
	memset( BP, '\0', KSK_BUF_SIZE );
	char *BP1 = BP;

// íÒèoîNåéì˙
	char	yymmdd[4]={0};
	if( IsJcContinuePrint(false) ){
		m_pSnHeadData->GetYmdData(ID_ICSSH_PRESENT_DATE, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	}
	else{
		m_pSnHeadData->GetYmdDataGen(ID_ICSSH_PRESENT_DATE, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	}
	for( j = k = 0; k < 1; k++ ){  
		for( x = 0; x < 3; j++, x++, BP1 += 3 ) {
			sw = 0;
			if( x == 0 ){
				if( yymmdd[x] )	sw = 1;
				else{
					if( yymmdd[x+1] )	sw = 1;
				}
			}
			else{
				if( yymmdd[x] )	sw = 1;
			}
			if( sw )
				wsprintf( BP1, "%02X", yymmdd[x] );
			else
				memset( BP1, '\0', 2 );
			PF[i].PITM[j] = BP1;
		}
	}
	i++;

// ê≈ñ±èêñº
	for( j = 0; j < 1; j++ ){
		memmove( BP1, m_pSnHeadData->Sn_ZMSYO, 12 );
		PF[i].PITM[j] = BP1;	BP1 += ( 12+1 );
	}
	i++;

// êÆóùî‘çÜ
	memset( buf, '\0', sizeof( buf ));
	m_Arith.l_unpac( buf, m_pSnHeadData->Sn_SEIRI, 8 );
	len = (int)strlen( buf );
	str.Empty();
	for( j = 8; j > 0; j-- ){
		if( len == j ){
			str += buf;		break;
		}
		str += ' ';
	}
	for( j = 0; j < 1; j++ ){

		wsprintf( BP1, "%s", str );
		PF[i].PITM[j] = BP1;	BP1 += ( 8+1 );
	}
	i++;

// â€ê≈ä˙ä‘
	int gengo1 = 0, gengo2 = 0;
	memset( yymmdd, '\0', sizeof(yymmdd) );
	if( IsJcContinuePrint(false) ){
		m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	}
	else{
		//m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1);
	}
	for( j = k = 0; k < 1; k++ ){  
		for( x = 0; x < 3; j++, x++, BP1 += 3 ) {
			sw = 0;
			if( x == 0 ){
				if( yymmdd[x] )	sw = 1;
				else{
					if( yymmdd[x+1] )	sw = 1;
				}
			}
			else{
				if( yymmdd[x] )	sw = 1;
			}
			if( sw )
				wsprintf( BP1, "%02X", yymmdd[x] );
			else
				memset( BP1, '\0', 2 );
			PF[i].PITM[j] = BP1;
		}
	}
	i++;
	memset( yymmdd, '\0', sizeof(yymmdd) );
	if( IsJcContinuePrint(false) ){
		m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	}
	else{
		//m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo2);
	}
	for( j = k = 0; k < 1; k++ ){  
		for( x = 0; x < 3; j++, x++, BP1 += 3 ) {
			sw = 0;
			if( x == 0 ){
				if( yymmdd[x] )	sw = 1;
				else{
					if( yymmdd[x+1] )	sw = 1;
				}
			}
			else{
				if( yymmdd[x] )	sw = 1;
			}
			if( sw )
				wsprintf( BP1, "%02X", yymmdd[x] );
			else
				memset( BP1, '\0', 2 );
			PF[i].PITM[j] = BP1;
		}
	}
	i++;

	if(IsSwitch_HeiseiReiwa(false)){
		if (gengo1 == ID_ICSSH_REIWA_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = (char *)0;
			PF[i].PITM[1] = BP1;
			BP1 += 5;
		}
		else if (gengo1 == ID_ICSSH_HEISEI_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = BP1;
			PF[i].PITM[1] = (char *)0;
			BP1 += 5;
		}
		else {
			PF[i].PITM[0] = (char *)0;
			PF[i].PITM[1] = (char *)0;
		}

		i++;

		if (gengo2 == ID_ICSSH_REIWA_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = (char *)0;
			PF[i].PITM[1] = BP1;
			BP1 += 5;
		}
		else if (gengo2 == ID_ICSSH_HEISEI_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = BP1;
			PF[i].PITM[1] = (char *)0;
			BP1 += 5;
		}
		else {
			PF[i].PITM[0] = (char *)0;
			PF[i].PITM[1] = (char *)0;
		}

		i++;
	}

// íÜä‘ê\çêÇÃåvéZä˙ä‘
	gengo1 = gengo2 = 0;
	memset( yymmdd, '\0', sizeof(yymmdd) );
	if( IsJcContinuePrint(false) ){
		m_pSnHeadData->GetYmdData(ID_ICSSH_CHUUKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	}
	else{
		//m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1);
	}
	for( j = k = 0; k < 1; k++ ){  
		for( x = 0; x < 3; j++, x++, BP1 += 3 ) {
			sw = 0;
			if( x == 0 ){
				if( yymmdd[x] )	sw = 1;
				else{
					if( yymmdd[x+1] )	sw = 1;
				}
			}
			else{
				if( yymmdd[x] )	sw = 1;
			}
			if( sw )
				wsprintf( BP1, "%02X", yymmdd[x] );
			else
				memset( BP1, '\0', 2 );
			PF[i].PITM[j] = BP1;
		}
	}
	i++;
	memset( yymmdd, '\0', sizeof(yymmdd) );
	if( IsJcContinuePrint(false) ){
		m_pSnHeadData->GetYmdData(ID_ICSSH_CHUUKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	}
	else{
		//m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		m_pSnHeadData->GetYmdDataGen(ID_ICSSH_CHUUKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo2);
	}
	for( j = k = 0; k < 1; k++ ){  
		for( x = 0; x < 3; j++, x++, BP1 += 3 ) {
			sw = 0;
			if( x == 0 ){
				if( yymmdd[x] )	sw = 1;
				else{
					if( yymmdd[x+1] )	sw = 1;
				}
			}
			else{
				if( yymmdd[x] )	sw = 1;
			}
			if( sw )
				wsprintf( BP1, "%02X", yymmdd[x] );
			else
				memset( BP1, '\0', 2 );
			PF[i].PITM[j] = BP1;
		}
	}
	i++;

	if(IsSwitch_HeiseiReiwa(false)){
		if (gengo1 == ID_ICSSH_REIWA_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = (char *)0;
			PF[i].PITM[1] = BP1;
			BP1 += 5;
		}
		else if (gengo1 == ID_ICSSH_HEISEI_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = BP1;
			PF[i].PITM[1] = (char *)0;
			BP1 += 5;
		}
		else {
			PF[i].PITM[0] = (char *)0;
			PF[i].PITM[1] = (char *)0;
		}

		i++;

		if (gengo2 == ID_ICSSH_REIWA_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = (char *)0;
			PF[i].PITM[1] = BP1;
			BP1 += 5;
		}
		else if (gengo2 == ID_ICSSH_HEISEI_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = BP1;
			PF[i].PITM[1] = (char *)0;
			BP1 += 5;
		}
		else {
			PF[i].PITM[0] = (char *)0;
			PF[i].PITM[1] = (char *)0;
		}

		i++;
	}

	return( _xprn( 2, dp, PF, 0, pDC, 0, 0x0101 ));
}

//-----------------------------------------------------------------------------
// ëOîNé¿ê—Ç…ÇÊÇÈíÜä‘ê\çêÇQ(é¿ïîÇS)
//-----------------------------------------------------------------------------
int CH26HyoView::_dprn4_EX( int dp, CDC* pDC, char *BP )
{
	int		i, j, k, x;
	char	buf[128];
	int		len, sw;
	char	val[6];
	PBUF	PF[30];

	i = 0;
	memset( BP, '\0', KSK_BUF_SIZE );
	char *BP1 = BP;

// ëOâ€ê≈ä˙ä‘
	int gengo1 = 0, gengo2 = 0;
	char	yymmdd[4]={0};
	if( IsJcContinuePrint(false) ){
		m_pSnChskData->GetYmdData(ID_ZENKI_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	}
	else{
		//m_pSnChskData->GetYmdDataGen(ID_ZENKI_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		m_pSnChskData->GetYmdDataGen(ID_ZENKI_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1);
	}
	for( j = k = 0; k < 1; k++ ){  
		for( x = 0; x < 3; j++, x++, BP1 += 3 ) {
			sw = 0;
			if( x == 0 ){
				if( yymmdd[x] )	sw = 1;
				else{
					if( yymmdd[x+1] )	sw = 1;
				}
			}
			else{
				if( yymmdd[x] )	sw = 1;
			}
			if( sw )
				wsprintf( BP1, "%02X", yymmdd[x] );
			else
				memset( BP1, '\0', 2 );
			PF[i].PITM[j] = BP1;
		}
	}
	i++;
	memset( yymmdd, '\0', sizeof(yymmdd) );
	if( IsJcContinuePrint(false) ){
		m_pSnChskData->GetYmdData(ID_ZENKI_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	}
	else{
		//m_pSnChskData->GetYmdDataGen(ID_ZENKI_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		m_pSnChskData->GetYmdDataGen(ID_ZENKI_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo2
		);
	}
	for( j = k = 0; k < 1; k++ ){  
		for( x = 0; x < 3; j++, x++, BP1 += 3 ) {
			sw = 0;
			if( x == 0 ){
				if( yymmdd[x] )	sw = 1;
				else{
					if( yymmdd[x+1] )	sw = 1;
				}
			}
			else{
				if( yymmdd[x] )	sw = 1;
			}
			if( sw )
				wsprintf( BP1, "%02X", yymmdd[x] );
			else
				memset( BP1, '\0', 2 );
			PF[i].PITM[j] = BP1;
		}
	}
	i++;

	if(IsSwitch_HeiseiReiwa(false)){
		if (gengo1 == ID_ICSSH_REIWA_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = (char *)0;
			PF[i].PITM[1] = BP1;
			BP1 += 5;
		}
		else if (gengo1 == ID_ICSSH_HEISEI_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = BP1;
			PF[i].PITM[1] = (char *)0;
			BP1 += 5;
		}
		else {
			PF[i].PITM[0] = (char *)0;
			PF[i].PITM[1] = (char *)0;
		}

		i++;

		if (gengo2 == ID_ICSSH_REIWA_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = (char *)0;
			PF[i].PITM[1] = BP1;
			BP1 += 5;
		}
		else if (gengo2 == ID_ICSSH_HEISEI_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = BP1;
			PF[i].PITM[1] = (char *)0;
			BP1 += 5;
		}
		else {
			PF[i].PITM[0] = (char *)0;
			PF[i].PITM[1] = (char *)0;
		}

		i++;
	}

// èCê≥ÅEçXê≥ÅEåàíËÇÃîNåéì˙
	gengo1 = gengo2 = 0;
	memset( yymmdd, '\0', sizeof(yymmdd) );
	if( IsJcContinuePrint(false) ){
		m_pSnChskData->GetYmdData(ID_SYUZEI_DECIDE_DAY, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	}
	else{
		//m_pSnChskData->GetYmdDataGen(ID_SYUZEI_DECIDE_DAY, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		m_pSnChskData->GetYmdDataGen(ID_SYUZEI_DECIDE_DAY, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1);
	}
	for( j = k = 0; k < 1; k++ ){  
		for( x = 0; x < 3; j++, x++, BP1 += 3 ) {
			sw = 0;
			if( x == 0 ){
				if( yymmdd[x] )	sw = 1;
				else{
					if( yymmdd[x+1] )	sw = 1;
				}
			}
			else{
				if( yymmdd[x] )	sw = 1;
			}
			if( sw )
				wsprintf( BP1, "%02X", yymmdd[x] );
			else
				memset( BP1, '\0', 2 );
			PF[i].PITM[j] = BP1;
		}
	}
	i++;

	if(IsSwitch_HeiseiReiwa(false)){
		if (gengo1 == ID_ICSSH_REIWA_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = (char *)0;
			PF[i].PITM[1] = BP1;
			BP1 += 5;
		}
		else if (gengo1 == ID_ICSSH_HEISEI_GENGO) {
			memmove(BP1, "    ", 4);
			PF[i].PITM[0] = BP1;
			PF[i].PITM[1] = (char *)0;
			BP1 += 5;
		}
		else {
			PF[i].PITM[0] = (char *)0;
			PF[i].PITM[1] = (char *)0;
		}

		i++;
	}

// ëOâ€ê≈ä˙ä‘ÇÃè¡îÔê≈äz
	len = val_set( buf, m_pSnHonpyoData->Sn_ZNOFZ );
	for( j = 0; j < 1; j++ ){  
		memcpy( BP1, buf, len );
		PF[i].PITM[j] = BP1;	BP1 += (len+1);
	}
	i++;

// åéêîä∑éZ
	memset( buf, '\0', sizeof( buf ));
	if( m_pSnChskData->Sn_BSMON ){
		wsprintf( buf, "%2d", m_pSnChskData->Sn_BSMON );
	}
	len = (int)strlen( buf );
	for( j = 0; j < 1; j++ ){
		memmove( BP1, buf, len );
		PF[i].PITM[j] = BP1;
		BP1 += ( len+1 );
	}
	i++;
	memset( buf, '\0', sizeof( buf ));
	if( m_pSnChskData->Sn_BBMON ){
		wsprintf( buf, "%2d", m_pSnChskData->Sn_BBMON );
	}
	len = (int)strlen( buf );
	for( j = 0; j < 1; j++ ){
		memmove( BP1, buf, len );
		PF[i].PITM[j] = BP1;
		BP1 += ( len+1 );
	}
	i++;

// èCê≥ê\çê
	if( m_pSnHeadData->Sn_SKKBN == 4 ){
		for( x = 0; x < 5; x++, i++ ){
			memset( val, '\0', 6 );
			switch( x ){
			case 0:		// è¡îÔê≈Å@Ç±ÇÃê\çêëOÇÃê≈äz
				memmove( val, m_pSnChskData->Sn_ZSKZN, 6 );	break;
			case 1:		// è¡îÔê≈Å@Ç±ÇÃê\çêÇ…ÇÊÇËëùâ¡Ç∑ÇÈê≈äz
				memmove( val, m_pSnChskData->Sn_ZSKAD, 6 );	break;
			case 2:		// ínï˚è¡îÔê≈Å@Ç±ÇÃê\çêëOÇÃê≈äz
				memmove( val, m_pSnChskData->Sn_ZSTZN, 6 );	break;
			case 3:		// ínï˚è¡îÔê≈Å@Ç±ÇÃê\çêÇ…ÇÊÇËëùâ¡Ç∑ÇÈê≈äz
				memmove( val, m_pSnChskData->Sn_ZSTAD, 6 );	break;
			case 4:		// çáåvî[ïtê≈äz
				memmove( val, m_pSnChskData->Sn_ZSKTZ, 6 );	break;
			}
			len = val_set( buf, (unsigned char*)val );
			for( j = 0; j < 1; j++ ){  
				memcpy( BP1, buf, len );
				PF[i].PITM[j] = BP1;	BP1 += (len+1);
			}
		}
	}
	else{
		memset( buf, '\0', sizeof( buf ));
		for( x = 0; x < 5; x++, i++ ){
			for( j = 0; j < 1; j++ ){  
				memcpy( BP1, buf, 11 );
				PF[i].PITM[j] = BP1;	BP1 += (11+1);
			}
		}
	}

// î[ïtÇ∑Ç◊Ç´è¡îÔê≈äz
	len = val_set( buf, m_pSnHonpyoData->Sn_EDNOFZ );
	for( j = 0; j < 1; j++ ){  
		memcpy( BP1, buf, len );
		PF[i].PITM[j] = BP1;	BP1 += (len+1);
	}
	i++;

// î[ïtÇ∑Ç◊Ç´ínï˚è¡îÔê≈äz
	len = val_set( buf, m_pSnHonpyoData->Sn_TEDNOF );
	for( j = 0; j < 1; j++ ){  
		memcpy( BP1, buf, len );
		PF[i].PITM[j] = BP1;	BP1 += (len+1);
	}
	i++;

// çáåvî[ïtê≈äz
	len = val_set( buf, m_pSnChskData->Sn_KTNFZ );
	for( j = 0; j < 1; j++ ){  
		memcpy( BP1, buf, len );
		PF[i].PITM[j] = BP1;	BP1 += (len+1);
	}
	i++;

	return( _xprn( 2, dp, PF, 0, pDC, 0, 0x0101 ));
}
void CH26HyoView::TelBufSet( char *buf, int mode ) 
{
	char	bf0[64], bf1[8], bf2[8], bf3[8];
	int		len;

	memset( bf1, '\0', sizeof( bf1 ));
	memset( bf2, '\0', sizeof( bf2 ));
	memset( bf3, '\0', sizeof( bf3 ));

	len = kjlen( m_pSnHeadData->Sn_TELNO1, 6 );
	memmove( bf1, m_pSnHeadData->Sn_TELNO1, len );
	len = kjlen( m_pSnHeadData->Sn_TELNO2, 4 );
	memmove( bf2, m_pSnHeadData->Sn_TELNO2, len );
	len = kjlen( m_pSnHeadData->Sn_TELNO3, 4 );
	memmove( bf3, m_pSnHeadData->Sn_TELNO3, len );
	memset( bf0, '\0', sizeof( bf0 ));
	if( !mode )
		sprintf_s( bf0, sizeof( bf0 ), "(%-6s) %-4s Å| %-4s", bf1, bf2, bf3 );
	else
		sprintf_s( bf0, sizeof( bf0 ), "%-6sÅ| %-4s Å| %-4s", bf1, bf2, bf3 );
	len = (int)strlen( bf0 );
	memmove( buf, bf0, len ); 
}













//-------------------------------------------------------------------->//[Y]
//ä»à’â€ê≈ëÊòZéÌëŒâû
//â∫ãLí«â¡ä÷êî
//-----------------------------------------------------------------------------
// ëÊòZéÌëŒâû ïtï\ÇTèoóÕ
//-----------------------------------------------------------------------------
// à¯êî	
//
//
//-----------------------------------------------------------------------------
// ï‘ëóíl
//-----------------------------------------------------------------------------
int CH26HyoView::PrintFuhyo50_EX( CDC *pDC, int kojin, int pcnt )
{
	int				i, j;
	unsigned int	size;
	char			*AP, *dp;
	
	BOOL	isNoImage = FALSE;
	if( m_pPrintCmInfo->PRtype&0x80 ){
		isNoImage = FALSE;	// ÉCÉÅÅ[ÉWèoóÕóLÇË
	}
	else{
		isNoImage = TRUE;	// ÉCÉÅÅ[ÉWèoóÕóLÇË
	}
	
	size = 512;
	if( (AP = new char[size]) == NULL ){
		return -1;
	}

	CH27SnFhyo10Data*	m_pH27SnFhyo40Data=NULL;
	CH27SnFhyo50Data*	m_pH27SnFhyo50Data=NULL;
	CH27SnHonpyoData*	m_pH27SnHonpyoData=NULL;

	m_pH27SnFhyo40Data = (CH27SnFhyo10Data*)m_pSnFhyo40Data;
	m_pH27SnFhyo50Data = (CH27SnFhyo50Data*)m_pSnFhyo50Data;
	m_pH27SnHonpyoData = (CH27SnHonpyoData*)m_pSnHonpyoData;
	
	PBUF	PF1[30];
	int		PP;

	// èoóÕëOèWåv
//	FuHyo52Calqe();
	PP = 6;
	dp = AP;
	memset( dp, '\0', size );
	i = 0;
	char	WORK0[MONY_BUF_SIZE] = {0};
	char	WORK_ALL[MONY_BUF_SIZE] = {0};

	int CCSw = 0;

	// ã≠êßâ€ê≈ä˙ä‘àÛç¸
	if( CCSw ){	// ã≠êßä˙ä‘ïœçX
//		wsprintf( dp, _T("%02X %02X %02X"), CC_KDAYS[0]&0xff, CC_KDAYS[1]&0xff, CC_KDAYS[2]&0xff );
//		PF1[i].PITM[0] = dp;
//		dp += 9;
//		wsprintf( dp, _T("%02X %02X %02X"), CC_KDAYE[0]&0xff, CC_KDAYE[1]&0xff, CC_KDAYE[2]&0xff );
//		PF1[i++].PITM[1] = dp;
//		dp += 9;
	}
	else{
		char yymmdd[3] = {0};
		if( IsJcContinuePrint(true) ){
			m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		else{
			m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		wsprintf( dp, _T("%02X %02X %02X"), yymmdd[0]&0xff, yymmdd[1]&0xff, yymmdd[2]&0xff );
		PF1[i].PITM[0] = dp;
		dp += 9;
		memset( yymmdd, '\0', sizeof(yymmdd) );
		if( IsJcContinuePrint(true) ){
			m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		else{
			m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		wsprintf( dp, _T("%02X %02X %02X"), yymmdd[0]&0xff, yymmdd[1]&0xff, yymmdd[2]&0xff );
		PF1[i++].PITM[1] = dp;
		dp += 9;
	}

	memmove( dp, _T("Å@"), 2 );
	PF1[i++].PITM[0] = dp;
	dp += (2 + 1);

	j = 0;										// ã‡äz 1->5
	m_Arith.l_print( dp , m_pH27SnFhyo40Data->Sn_1F2C ,  FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 1
	m_Arith.l_print( dp , m_pH27SnFhyo40Data->Sn_1F3C ,  FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 2

	m_Arith.l_print( dp , m_pH27SnFhyo40Data->Sn_1F5C ,  FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 3

	m_Arith.l_add( WORK0 , m_pH27SnFhyo40Data->Sn_1F2C ,  m_pH27SnFhyo40Data->Sn_1F3C );
	m_Arith.l_sub( WORK0 , WORK0 ,  m_pH27SnFhyo40Data->Sn_1F5C );
	if( m_Arith.l_test( WORK0 ) < 0 ){
		memset( WORK0, '\0', MONY_BUF_SIZE );
	}
	m_Arith.l_print( dp , WORK0 ,  FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 4

	int Flg = 0;
	if( m_Arith.l_test( m_pH27SnHonpyoData->Sn_KURI1 )) Flg++;
	if( m_Arith.l_test( m_pH27SnHonpyoData->Sn_KURI2 )) Flg++;
	if( m_Arith.l_test( m_pH27SnHonpyoData->Sn_KURI3 )) Flg++;
	if( m_Arith.l_test( m_pH27SnHonpyoData->Sn_KURI4 )) Flg++;
	if( m_Arith.l_test( m_pH27SnHonpyoData->Sn_KURI5 )) Flg++;
	if( m_SixKindFlg ){
		if( m_Arith.l_test(m_pH27SnHonpyoData->Sn_KURI6) ){
			Flg++;
		}
	}

	if( Flg == 1 ){
		m_Arith.l_print( dp , m_pH27SnFhyo40Data->Sn_1F4C ,  FMT15 );
		PF1[i++].PITM[j] = dp;	dp += 16;		// 5
	}
	else{
		PF1[i++].PITM[j] = (char *)0;
	}
	
	j = 0;											// ã‡äz 6->12
	if( Flg >= 2 ){		// ï°êîéñã∆é“î≠ê∂éûÇÃÇ›
//-- '15.02.10 --
//		m_Arith.l_print( dp, m_pSnHeadData->Sn_KURIA, FMT15 );
//---------------
		if( m_Arith.l_test(m_pSnHeadData->Sn_KURIA) ||
			m_Arith.l_test(m_pSnHonpyoData->Sn_KURI1) || m_Arith.l_test(m_pSnHonpyoData->Sn_KURI2) || m_Arith.l_test(m_pSnHonpyoData->Sn_KURI3) ||
			m_Arith.l_test(m_pSnHonpyoData->Sn_KURI4) || m_Arith.l_test(m_pSnHonpyoData->Sn_KURI5) ){
			m_Arith.l_print( dp, m_pSnHeadData->Sn_KURIA, FMT16 );
		}
		else{
			m_Arith.l_print( dp, m_pSnHeadData->Sn_KURIA, FMT15 );
		}
//---------------		
		PF1[i].PITM[j++] = dp;
		dp += 16;			// 6
//-- '15.01.26 --
//		m_Arith.l_print( dp, m_pH27SnHonpyoData->Sn_KURI1, FMT15 );
//		PF1[i].PITM[j++] = dp;
//		dp += 16;			// áF
//		m_Arith.l_print( dp, m_pH27SnHonpyoData->Sn_KURI2, FMT15 );
//		PF1[i].PITM[j++] = dp;
//		dp += 16;			// áG
//		m_Arith.l_print( dp, m_pH27SnHonpyoData->Sn_KURI3, FMT15 );
//		PF1[i].PITM[j++] = dp;
//		dp += 16;			// áH
//		m_Arith.l_print( dp, m_pH27SnHonpyoData->Sn_KURI4, FMT15 );
//		PF1[i].PITM[j++] = dp;
//		dp += 16;			// áI
////		if( ! TBhead.SV_5sw ){
//		if( 1 ){
//			m_Arith.l_print( dp, m_pH27SnHonpyoData->Sn_KURI5, FMT15 );
//			PF1[i++].PITM[j] = dp;
//			dp += 16;		// áJ
//		}
//		else{
//			PF1[i++].PITM[j] = (char *)0;
//		}
//---------------
		char	zeroMony[MONY_BUF_SIZE]={0};
		if( m_Arith.l_test(m_pH27SnHonpyoData->Sn_KURI1) >= 0 ){
			m_Arith.l_print( dp, m_pH27SnHonpyoData->Sn_KURI1, FMT15 );
		}
		else{
			m_Arith.l_print( dp, zeroMony, FMT16 );
		}
		PF1[i].PITM[j++] = dp;
		dp += 16;			// 7
		if( m_Arith.l_test(m_pH27SnHonpyoData->Sn_KURI2) >= 0 ){
			m_Arith.l_print( dp, m_pH27SnHonpyoData->Sn_KURI2, FMT15 );
		}
		else{
			m_Arith.l_print( dp, zeroMony, FMT16 );
		}
		PF1[i].PITM[j++] = dp;
		dp += 16;			// 8
		if( m_Arith.l_test(m_pH27SnHonpyoData->Sn_KURI3) >= 0 ){
			m_Arith.l_print( dp, m_pH27SnHonpyoData->Sn_KURI3, FMT15 );
		}
		else{
			m_Arith.l_print( dp, zeroMony, FMT16 );
		}
		PF1[i].PITM[j++] = dp;
		dp += 16;			// 9
		if( m_Arith.l_test(m_pH27SnHonpyoData->Sn_KURI4) >= 0 ){
			m_Arith.l_print( dp, m_pH27SnHonpyoData->Sn_KURI4, FMT15 );
		}
		else{
			m_Arith.l_print( dp, zeroMony, FMT16 );
		}
		PF1[i].PITM[j++] = dp;
		dp += 16;			// 10
		if( m_Arith.l_test(m_pH27SnHonpyoData->Sn_KURI5) >= 0 ){
			m_Arith.l_print( dp, m_pH27SnHonpyoData->Sn_KURI5, FMT15 );
		}
		else{
			m_Arith.l_print( dp, zeroMony, FMT16 );
		}
		PF1[i].PITM[j++] = dp;
		dp += 16;			// 11
		if( m_Arith.l_test(m_pH27SnHonpyoData->Sn_KURI6) >= 0 ){
			m_Arith.l_print( dp, m_pH27SnHonpyoData->Sn_KURI6, FMT15 );
		}
		else{
			m_Arith.l_print( dp, zeroMony, FMT16 );
		}
		PF1[i++].PITM[j] = dp;
		dp += 16;			// 12
//---------------
		
		j = 0;										// äÑçá 7->12
		if( m_pH27SnHonpyoData->Sn_UP1 ){ 
			wsprintf( dp, _T("%3d.%01d"), m_pH27SnHonpyoData->Sn_UP1 / 10, m_pH27SnHonpyoData->Sn_UP1 % 10 );
		}
		PF1[i].PITM[j++] = dp;
		dp += 6;		// îÑè„äÑçá7
		if( m_pH27SnHonpyoData->Sn_UP2 ){
			wsprintf( dp, _T("%3d.%01d"), m_pH27SnHonpyoData->Sn_UP2 / 10, m_pH27SnHonpyoData->Sn_UP2 % 10 );
		}
		PF1[i].PITM[j++] = dp;
		dp += 6;		// îÑè„äÑçá8
		if( m_pH27SnHonpyoData->Sn_UP3 ){ 
			wsprintf( dp, _T("%3d.%01d"), m_pH27SnHonpyoData->Sn_UP3 / 10, m_pH27SnHonpyoData->Sn_UP3 % 10 );
		}
		PF1[i].PITM[j++] = dp;	
		dp += 6;		// îÑè„äÑçá9
		if( m_pH27SnHonpyoData->Sn_UP4 ){
			wsprintf( dp, _T("%3d.%01d"), m_pH27SnHonpyoData->Sn_UP4 / 10, m_pH27SnHonpyoData->Sn_UP4 % 10 );
		}
		PF1[i].PITM[j++] = dp;
		dp += 6;		// îÑè„äÑçá10
		if( m_pH27SnHonpyoData->Sn_UP5 ){ 
			wsprintf( dp, _T("%3d.%01d"), m_pH27SnHonpyoData->Sn_UP5 / 10, m_pH27SnHonpyoData->Sn_UP5 % 10 );
		}
		PF1[i].PITM[j++] = dp;
		dp += 6;		// îÑè„äÑçá11
		if( m_pH27SnHonpyoData->Sn_UP6 ){ 
			wsprintf( dp, _T("%3d.%01d"), m_pH27SnHonpyoData->Sn_UP6 / 10, m_pH27SnHonpyoData->Sn_UP6 % 10 );
		}
		PF1[i++].PITM[j++] = dp;
		dp += 6;		// îÑè„äÑçá12

		j = 0;											// ê≈äz13->19
//-- '15.02.10 --
//		m_Arith.l_print( dp, m_pSnFhyo50Data->Sn_5FUZTT, FMT15 );
//---------------
		if( m_Arith.l_test(m_pSnFhyo50Data->Sn_5FUZTT) ||
			m_Arith.l_test(m_pSnFhyo50Data->Sn_5FUZT1) || m_Arith.l_test(m_pSnFhyo50Data->Sn_5FUZT2) || m_Arith.l_test(m_pSnFhyo50Data->Sn_5FUZT3) ||
			m_Arith.l_test(m_pSnFhyo50Data->Sn_5FUZT4) || m_Arith.l_test(m_pSnFhyo50Data->Sn_5FUZT5) ){
			m_Arith.l_print( dp, m_pSnFhyo50Data->Sn_5FUZTT, FMT16 );
		}
		else{
			m_Arith.l_print( dp, m_pSnFhyo50Data->Sn_5FUZTT, FMT15 );
		}
//---------------
		PF1[i].PITM[j++] = dp;
		dp += 16;			// 13

//-- '15.01.26 --
//		m_Arith.l_print( dp, m_pH27SnFhyo50Data->Sn_5FUZT1, FMT15 );
//		PF1[i].PITM[j++] = dp;
//		dp += 16;			// áL
//		m_Arith.l_print( dp, m_pH27SnFhyo50Data->Sn_5FUZT2, FMT15 );
//		PF1[i].PITM[j++] = dp;	
//		dp += 16;			// áM
//		m_Arith.l_print( dp, m_pH27SnFhyo50Data->Sn_5FUZT3, FMT15 );
//		PF1[i].PITM[j++] = dp;
//		dp += 16;			// áN
//		m_Arith.l_print( dp, m_pH27SnFhyo50Data->Sn_5FUZT4, FMT15 );
//		PF1[i].PITM[j++] = dp;
//		dp += 16;			// áO
////	if( !TBhead.SV_5sw ){
//		if( 1 ){
//			m_Arith.l_print( dp, m_pH27SnFhyo50Data->Sn_5FUZT5, FMT15 );
//			PF1[i].PITM[j] = dp;
//			dp += 16;		// áP
//		}
//		else{
//			PF1[i].PITM[j] = (char *)0;
//		}
//---------------
		if( m_Arith.l_test(m_pH27SnFhyo50Data->Sn_5FUZT1) >= 0 ){
			m_Arith.l_print( dp, m_pH27SnFhyo50Data->Sn_5FUZT1, FMT15 );
		}
		else{
			m_Arith.l_print( dp, zeroMony, FMT16 );
		}
		PF1[i].PITM[j++] = dp;
		dp += 16;			// 14
		if( m_Arith.l_test(m_pH27SnFhyo50Data->Sn_5FUZT2) >= 0 ){
			m_Arith.l_print( dp, m_pH27SnFhyo50Data->Sn_5FUZT2, FMT15 );
		}
		else{
			m_Arith.l_print( dp, zeroMony, FMT16 );
		}
		PF1[i].PITM[j++] = dp;	
		dp += 16;			// 15
		if( m_Arith.l_test(m_pH27SnFhyo50Data->Sn_5FUZT3) >= 0 ){
			m_Arith.l_print( dp, m_pH27SnFhyo50Data->Sn_5FUZT3, FMT15 );
		}
		else{
			m_Arith.l_print( dp, zeroMony, FMT16 );
		}
		PF1[i].PITM[j++] = dp;
		dp += 16;			// 16
		if( m_Arith.l_test(m_pH27SnFhyo50Data->Sn_5FUZT4) >= 0 ){
			m_Arith.l_print( dp, m_pH27SnFhyo50Data->Sn_5FUZT4, FMT15 );
		}
		else{
			m_Arith.l_print( dp, zeroMony, FMT16 );
		}
		PF1[i].PITM[j++] = dp;
		dp += 16;			// 17
		if( m_Arith.l_test(m_pH27SnFhyo50Data->Sn_5FUZT5) >= 0 ){
			m_Arith.l_print( dp, m_pH27SnFhyo50Data->Sn_5FUZT5, FMT15 );
		}
		else{
			m_Arith.l_print( dp, zeroMony, FMT16 );
		}
		PF1[i].PITM[j++] = dp;
		dp += 16;			// 18
//---------------

		if( m_Arith.l_test(m_pH27SnFhyo50Data->Sn_5FUZT6) >= 0 ){
			m_Arith.l_print( dp, m_pH27SnFhyo50Data->Sn_5FUZT6, FMT15 );
		}
		else{
			m_Arith.l_print( dp, zeroMony, FMT16 );
		}
		PF1[i].PITM[j] = dp;
		dp += 16;			// 19
	}
	else{		// íPàÍéñã∆
		pbclr( 0, 7, &PF1[i++] );
		pbclr( 0, 6, &PF1[i++] );
		pbclr( 0, 7, &PF1[i++] );
	}
	
	if( j = _xprn( PP, 1, PF1, 0, pDC, isNoImage, 1 ) ){
		goto POUT5;
	}

	i = j = 0;

	if( Flg == 1 ){
		// íPàÍéñã∆é“
		// éÅñº
		dp = AP;	i = j = 0;
		memset( dp, '\0', size );
		if( kojin ){	// å¬êléñã∆é“ÇÃèÍçáÇÕë„ï\é“éÅñº
			j = sizeof( m_pSnHeadData->Sn_DIHYO );
			memmove( dp, m_pSnHeadData->Sn_DIHYO, j );
		}
		else {			// ñ@êlñº
			j = sizeof( m_pSnHeadData->Sn_CONAM );
			memmove( dp, m_pSnHeadData->Sn_CONAM, j );
		}
		PF1[0].PITM[0] = dp;
		if( j = _xprn( PP, 3, PF1, 2, pDC, 0, 1 ) ){
			goto POUT5;
		}

		// íPàÍéñã∆é“Å@Åõ
		dp = AP;	i = j = 0;
		memset( dp, '\0', size );
		pbclr( 0, 6, &PF1[0] );			// [07'09.25]

		if( m_Arith.l_test( m_pH27SnFhyo50Data->Sn_5FUZT1 )){
			PF1[0].PITM[0] = _T("Å@");
		}
		if( m_Arith.l_test( m_pH27SnFhyo50Data->Sn_5FUZT2 )){
			PF1[0].PITM[1] = _T("Å@");
		}
		if( m_Arith.l_test( m_pH27SnFhyo50Data->Sn_5FUZT3 )){
			PF1[0].PITM[2] = _T("Å@");
		}
		if( m_Arith.l_test( m_pH27SnFhyo50Data->Sn_5FUZT4 )){
			PF1[0].PITM[3] = _T("Å@");
		}
		if( m_Arith.l_test( m_pH27SnFhyo50Data->Sn_5FUZT5 )){
			PF1[0].PITM[4] = _T("Å@");
		}
		if( m_Arith.l_test( m_pH27SnFhyo50Data->Sn_5FUZT6 )){
			PF1[0].PITM[5] = _T("Å@");
		}
		m_pVprn->SetSfncEnv( SElfnc, 1 );
		if( j = _xprn( PP, 4, PF1, 0, pDC, 0, 1 ) ){
			goto POUT5;
		}

		goto POUT5;		// íPàÍéñã∆
	}

	dp = AP;
	int m_75sw = 0;
	memset( dp, '\0', size );
	pbclr( 0, 2,  &PF1[i] );
	pbclr( 0, 15, &PF1[i+1] );
	m_Arith.l_print( dp, m_pH27SnFhyo50Data->Sn_5FSZTG, FMT15 );
	PF1[i].PITM[j++] = dp;
	dp += 16;		// 20

	if( m_pH27SnFhyo50Data->Sn_5FSZsw  == 1 ){
		m_Arith.l_print( dp, m_pH27SnFhyo50Data->Sn_5FSZTT, FMT15 );
		PF1[i].PITM[j++] = dp;
		dp += 16;	// 21
	}
	else{
		PF1[i].PITM[j++] = (char *)0;
	}
	i++;
	j = 0;
	// ä»à’â€ê≈Å@2éÌóﬁà»è„ÇÃã∆éÌÇ≈ÇÃì¡ó·åvéZÉ`ÉFÉbÉN
	// 2éÌóﬁéñã∆Ç≈75%à»è„
	char val[MONY_BUF_SIZE] = {0};
	for( int t = 0; t<15; ++t ){			// 22->36
		if( ( m_pH27SnFhyo50Data->Sn_5FSZsw - 2) == t ){
			switch( m_pH27SnFhyo50Data->Sn_5FSZsw ){
				case 2:
					m_Arith.l_print( dp, m_pH27SnFhyo50Data->Sn_5FSZT22C, FMT15 );
					break;			
				case 3:
					m_Arith.l_print( dp, m_pH27SnFhyo50Data->Sn_5FSZT23C, FMT15 );
					break;			
				case 4:
					m_Arith.l_print( dp, m_pH27SnFhyo50Data->Sn_5FSZT24C, FMT15 );
					break;
				case 5:
					m_Arith.l_print( dp, m_pH27SnFhyo50Data->Sn_5FSZT25C, FMT15 );
					break;			
				case 6:
					m_Arith.l_print( dp, m_pH27SnFhyo50Data->Sn_5FSZT26C, FMT15 );
					break;			
				case 7:
					m_Arith.l_print( dp, m_pH27SnFhyo50Data->Sn_5FSZT27C, FMT15 );
					break;			
				case 8:
					m_Arith.l_print( dp, m_pH27SnFhyo50Data->Sn_5FSZT28C, FMT15 );
					break;			
				case 9:
					m_Arith.l_print( dp, m_pH27SnFhyo50Data->Sn_5FSZT29C, FMT15 );
					break;
				case 10:
					m_Arith.l_print( dp, m_pH27SnFhyo50Data->Sn_5FSZT30C, FMT15 );
					break;
				case 11:
					m_Arith.l_print( dp, m_pH27SnFhyo50Data->Sn_5FSZT31C, FMT15 );
					break;
				case 12:
					m_Arith.l_print( dp, m_pH27SnFhyo50Data->Sn_5FSZT32C, FMT15 );
					break;
				case 13:
					m_Arith.l_print( dp, m_pH27SnFhyo50Data->Sn_5FSZT33C, FMT15 );
					break;
				case 14:
					m_Arith.l_print( dp, m_pH27SnFhyo50Data->Sn_5FSZT34C, FMT15 );
					break;
				case 15:
					m_Arith.l_print( dp, m_pH27SnFhyo50Data->Sn_5FSZT35C, FMT15 );
					break;
				case 16:
					m_Arith.l_print( dp, m_pH27SnFhyo50Data->Sn_5FSZT36C, FMT15 );
					break;
			}
			PF1[i].PITM[j++] = dp;
			dp += 16;
		}
		else{
			PF1[i].PITM[j++] = (char *)0;
		}
	}
	
	m_Arith.l_print( dp, m_pH27SnFhyo40Data->Sn_1F4C, FMT15 );
	PF1[++i].PITM[0] = dp;
	dp += 16;		// (37)
	
	if( j = _xprn( PP, 2, PF1, 0, pDC, 0, 1 ) ){
		goto POUT5;
	}

	// éÅñº
	dp = AP;	i = j = 0;
	memset( dp, '\0', size );
	if( kojin ){	// å¬êléñã∆é“ÇÃèÍçáÇÕë„ï\é“éÅñº
		j = sizeof( m_pSnHeadData->Sn_DIHYO );
		memmove( dp, m_pSnHeadData->Sn_DIHYO, j );
	}
	else {			// ñ@êlñº
		j = sizeof( m_pSnHeadData->Sn_CONAM );
		memmove( dp, m_pSnHeadData->Sn_CONAM, j );
	}
	PF1[0].PITM[0] = dp;
	if( j = _xprn( PP, 3, PF1, 2, pDC, 0, 1 ) ){
		goto POUT5;
	}

	// ì¡ó·åvéZÇìKópÇ≈ÇPéÌóﬁÇÃéñã∆Ç≈ÇVÇTÅìà»è„ÇÃèÍçá
	if( m_pH27SnFhyo50Data->Sn_5FSZsw == 1){
		dp = AP;
		i = j = 0;
		memset( dp, '\0', size );
		pbclr( 0, 6,  &PF1[i] );

		if( m_pH27SnHonpyoData->Sn_UP1 >= 750 ){
			PF1[0].PITM[0] = _T("Å@");
		}
		if( m_pH27SnHonpyoData->Sn_UP2 >= 750 ){
			PF1[0].PITM[1] = _T("Å@");
		}
		if( m_pH27SnHonpyoData->Sn_UP3 >= 750 ){
			PF1[0].PITM[2] = _T("Å@");
		}
		if( m_pH27SnHonpyoData->Sn_UP4 >= 750 ){
			PF1[0].PITM[3] = _T("Å@");
		}
		if( m_pH27SnHonpyoData->Sn_UP5 >= 750 ){
			PF1[0].PITM[4] = _T("Å@");
		}
		//òZéÌ
		if( m_pH27SnHonpyoData->Sn_UP6 >= 750 ){
			PF1[0].PITM[5] = _T("Å@");
		}
		//ÉtÉåÅ[ÉÄéÌÇóLå¯Ç∆Ç∑ÇÈà◊ÇÃëOèàóù
		m_pVprn->SetSfncEnv( SElfnc, 1 );
		if( j = _xprn( PP, 5, PF1, 0, pDC, 0, 1 ) ){
			goto POUT5;
		}
	}

POUT5:
	if( AP ){
		delete [] AP;
	}
	
	return j;

}

//// ëÊòZéÌëŒâû ïtï\ÇTÅ|ÇQèoóÕ
//int PrintFuhyo52_EX( CDC *pDC, int kojin, int pcnt )
//{
//	return 0;
//}

//-----------------------------------------------------------------------------
// ëÊòZéÌëŒâû ïtï\ÇTÅ|ÇQÅQÇPèoóÕ
//-----------------------------------------------------------------------------
// à¯êî	pDC		ÅF
//		kojin	ÅF
//		pcnt	ÅF
//-----------------------------------------------------------------------------
// ï‘ëóíl
//-----------------------------------------------------------------------------
int CH26HyoView::PrintFuhyo52_1_EX( CDC *pDC, int kojin, int pcnt )
{
	int				i, j;
	unsigned int	size;
	char			*AP, *dp;
	
	BOOL	isNoImage = FALSE;
	if( m_pPrintCmInfo->PRtype&0x80 ){
		isNoImage = FALSE;	// ÉCÉÅÅ[ÉWèoóÕóLÇË
	}
	else{
		isNoImage = TRUE;	// ÉCÉÅÅ[ÉWèoóÕñ≥Çµ
	}
	
	size = 512;
	if( (AP = new char[size]) == NULL ){
		return -1;
	}
	memset( AP, '\0', (sizeof(char)*size) );

	CH27SnFhyo10Data*	m_pH27SnFhyo40Data;
	CH27SnFhyo50Data*	m_pH27SnFhyo52Data;
	CH27SnHonpyoData*	m_pH27SnHonpyoData;

	m_pH27SnFhyo40Data = (CH27SnFhyo10Data*)m_pSnFhyo40Data;
	m_pH27SnFhyo52Data = (CH27SnFhyo50Data*)m_pSnFhyo52Data;
	m_pH27SnHonpyoData = (CH27SnHonpyoData*)m_pSnHonpyoData;

	
	// èoóÕëOèWåv
//	FuHyo52Calqe();

	PBUF	PF1[30]={0};
	int		PP;

	PP = 7;

	dp = AP;
	memset( dp, '\0', size );
	i = 0;

	int		CCSw = 0;
	char	W0[MONY_BUF_SIZE] = {0};
	char	W1[MONY_BUF_SIZE] = {0};
	char	W2[MONY_BUF_SIZE] = {0};
	char	W3[MONY_BUF_SIZE] = {0};
	char	W4[MONY_BUF_SIZE] = {0};
	char	W5[MONY_BUF_SIZE] = {0};
	char	W6[MONY_BUF_SIZE] = {0};
	char	W7[MONY_BUF_SIZE] = {0};
	char	W8[MONY_BUF_SIZE] = {0};
	char	W9[MONY_BUF_SIZE] = {0};

	int		Flg = 0;

	// ã≠êßâ€ê≈ä˙ä‘àÛç¸
	if( CCSw ){	// ã≠êßä˙ä‘ïœçX
//		wsprintf( dp, "%02X %02X %02X", CC_KDAYS[0]&0xff, CC_KDAYS[1]&0xff, CC_KDAYS[2]&0xff );
//		PF1[i].PITM[0] = dp;
//		dp += 9;
//		wsprintf( dp, "%02X %02X %02X", CC_KDAYE[0]&0xff, CC_KDAYE[1]&0xff, CC_KDAYE[2]&0xff );
//		PF1[i++].PITM[1] = dp;	dp += 9;
	}
	else{
		char	yymmdd[3] = {0};
		if( IsJcContinuePrint(true) ){
			m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		else{
			m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		wsprintf( dp, "%02X %02X %02X", yymmdd[0]&0xff, yymmdd[1]&0xff, yymmdd[2]&0xff );
		PF1[i].PITM[0] = dp;
		dp += 9;
		memset( yymmdd, '\0', sizeof(yymmdd) );
		if( IsJcContinuePrint(true) ){
			m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		else{
			m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		}
		wsprintf( dp, "%02X %02X %02X", yymmdd[0]&0xff, yymmdd[1]&0xff, yymmdd[2]&0xff );
		PF1[i++].PITM[1] = dp;	
		dp += 9;
	}

	memmove( dp, "Å@", 2);
	PF1[i++].PITM[0] = dp;
	dp += (2 + 1);

	j = 0;											
	// ã‡äz1->4
	// 1
	m_Arith.l_print( dp, m_pH27SnFhyo40Data->Sn_1F2A, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 1-A
	m_Arith.l_add( W1 , W1 , m_pH27SnFhyo40Data->Sn_1F2A );// 4Ç…ë´ÇµÇ±Ç›

	m_Arith.l_print( dp, m_pH27SnFhyo40Data->Sn_1F2B, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 1-B
	m_Arith.l_add( W2 , W2 , m_pH27SnFhyo40Data->Sn_1F2B );// 4Ç…ë´ÇµÇ±Ç›

	m_Arith.l_print( dp, m_pH27SnFhyo40Data->Sn_1F2C, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 1-C
	m_Arith.l_add( W3 , W3 , m_pH27SnFhyo40Data->Sn_1F2C );// 4Ç…ë´ÇµÇ±Ç›

	m_Arith.l_add( W0 , m_pH27SnFhyo40Data->Sn_1F2A , m_pH27SnFhyo40Data->Sn_1F2B );
	m_Arith.l_add( W0 , W0 , m_pH27SnFhyo40Data->Sn_1F2C );
	m_Arith.l_print( dp, W0, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 1-D

//-->'15.01.26 INS START
	m_Arith.l_add( W4 , W4 , W0 );
//-->'15.01.26 INS END

	// 2
	m_Arith.l_print( dp, m_pH27SnFhyo40Data->Sn_1F3A, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 2-A
	m_Arith.l_add( W1 , W1 , m_pH27SnFhyo40Data->Sn_1F3A );// 4Ç…ë´ÇµÇ±Ç›

	m_Arith.l_print( dp, m_pH27SnFhyo40Data->Sn_1F3B, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 2-B
	m_Arith.l_add( W2 , W2 , m_pH27SnFhyo40Data->Sn_1F3B );// 4Ç…ë´ÇµÇ±Ç›

	m_Arith.l_print( dp, m_pH27SnFhyo40Data->Sn_1F3C, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 2-C
	m_Arith.l_add( W3 , W3 , m_pH27SnFhyo40Data->Sn_1F3C );// 4Ç…ë´ÇµÇ±Ç›

	m_Arith.l_add( W0 , m_pH27SnFhyo40Data->Sn_1F3A , m_pH27SnFhyo40Data->Sn_1F3B );
	m_Arith.l_add( W0 , W0 , m_pH27SnFhyo40Data->Sn_1F3C );
	m_Arith.l_print( dp, W0, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 2-D

//-->'15.01.26 INS START
	m_Arith.l_add( W4 , W4 , W0 );
//-->'15.01.26 INS END

	// 3
	m_Arith.l_print( dp, m_pH27SnFhyo40Data->Sn_1F5A, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 3-A
	m_Arith.l_sub( W1 , W1 , m_pH27SnFhyo40Data->Sn_1F5A );// 4Ç…ë´ÇµÇ±Ç›

	m_Arith.l_print( dp, m_pH27SnFhyo40Data->Sn_1F5B, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 3-B
	m_Arith.l_sub( W2 , W2 , m_pH27SnFhyo40Data->Sn_1F5B );// 4Ç…ë´ÇµÇ±Ç›

	m_Arith.l_print( dp, m_pH27SnFhyo40Data->Sn_1F5C, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 3-C
	m_Arith.l_sub( W3 , W3 , m_pH27SnFhyo40Data->Sn_1F5C );// 4Ç…ë´ÇµÇ±Ç›

	m_Arith.l_add( W0 , m_pH27SnFhyo40Data->Sn_1F5A , m_pH27SnFhyo40Data->Sn_1F5B );
	m_Arith.l_add( W0 , W0 , m_pH27SnFhyo40Data->Sn_1F5C );
	m_Arith.l_print( dp, W0, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 3-D

//-->'15.01.26 INS START
	m_Arith.l_sub( W4 , W4 , W0 );
//-->'15.01.26 INS END

	// 4
//-- '15.01.26 --
//	if( m_Arith.l_test(W1) < 0  ){
//		memset( W1 , '\0', MONY_BUF_SIZE );
//	}
//	if( m_Arith.l_test(W2) < 0  ){
//		memset( W2 , '\0', MONY_BUF_SIZE );
//	}
//	if( m_Arith.l_test(W3) < 0  ){
//		memset( W3 , '\0', MONY_BUF_SIZE );
//	}
//
//	m_Arith.l_print( dp, W1, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// áC-A
//	m_Arith.l_print( dp, W2, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// áC-B
//	m_Arith.l_print( dp, W3, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// áC-C
//
//	m_Arith.l_add( W4 , W1 , W2 );
//	m_Arith.l_add( W4 , W4 , W3 );
//	if( m_Arith.l_test(W4) < 0  ){
//		memset( W4 , '\0', MONY_BUF_SIZE );
//	}
//	m_Arith.l_print( dp, W4, FMT15 );
//	PF1[i++].PITM[j++] = dp;	dp += 16;			// áC-D
//---------------
	// 4-A
	if( m_Arith.l_test(W1) < 0  ){
		memset( W1 , '\0', MONY_BUF_SIZE );
		m_Arith.l_print( dp, W1, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W1, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;

	// 4-B
	if( m_Arith.l_test(W2) < 0  ){
		memset( W2 , '\0', MONY_BUF_SIZE );
		m_Arith.l_print( dp, W2, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W2, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;

	// 4-C
	if( m_Arith.l_test(W3) < 0  ){
		memset( W3 , '\0', MONY_BUF_SIZE );
		m_Arith.l_print( dp, W3, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W3, FMT15 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;

	// 4-D
	if( m_Arith.l_test(W4) < 0  ){
		memset( W4 , '\0', MONY_BUF_SIZE );
		m_Arith.l_print( dp, W4, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W4, FMT15 );
	}
	PF1[i++].PITM[j++] = dp;	dp += 16;
//---------------

	Flg = 0;
//--> '15.02.10 INS START
	unsigned char	gyoFlg = 0x00;	// D0:ëÊÇPéÌ, D1:ëÊÇQéÌ, D2:ëÊÇRéÌ, D3:ëÊÇSéÌ, D4:ëÊÇTéÌ, D5:ëÊÇUéÌ
//--> '15.02.10 INS END

//-- '15.01.21 --
//	if( m_Arith.l_test( m_pH27SnHonpyoData->Sn_KURI1 )) Flg++;
//	if( m_Arith.l_test( m_pH27SnHonpyoData->Sn_KURI2 )) Flg++;
//	if( m_Arith.l_test( m_pH27SnHonpyoData->Sn_KURI3 )) Flg++;
//	if( m_Arith.l_test( m_pH27SnHonpyoData->Sn_KURI4 )) Flg++;
//	if( m_Arith.l_test( m_pH27SnHonpyoData->Sn_KURI5 )) Flg++;
//---------------
	if( (m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI1A)||m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI1B)||m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI1C)) ||
		m_Arith.l_test(m_pH27SnHonpyoData->Sn_KURI1) ){
		Flg++;
//--> '15.02.10 INS START
		gyoFlg |= 0x01;
//--> '15.02.10 INS END
	}
	if( (m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI2A)||m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI2B)||m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI2C)) ||
		m_Arith.l_test(m_pH27SnHonpyoData->Sn_KURI2) ){
		Flg++;
//--> '15.02.10 INS START
		gyoFlg |= 0x02;
//--> '15.02.10 INS END
	}
	if( (m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI3A)||m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI3B)||m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI3C)) ||
		m_Arith.l_test(m_pH27SnHonpyoData->Sn_KURI3) ){
		Flg++;
//--> '15.02.10 INS START
		gyoFlg |= 0x04;
//--> '15.02.10 INS END
	}
	if( (m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI4A)||m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI4B)||m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI4C)) ||
		m_Arith.l_test(m_pH27SnHonpyoData->Sn_KURI4) ){
		Flg++;
//--> '15.02.10 INS START
		gyoFlg |= 0x08;
//--> '15.02.10 INS END
	}
	if( (m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI5A)||m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI5B)||m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI5C)) ||
		m_Arith.l_test(m_pH27SnHonpyoData->Sn_KURI5) ){
		Flg++;
//--> '15.02.10 INS START
		gyoFlg |= 0x10;
//--> '15.02.10 INS END
	}
	if( (m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI6A)||m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI6B)||m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI6C)) ||
		m_Arith.l_test(m_pH27SnHonpyoData->Sn_KURI6) ){
		Flg++;
//--> '15.02.10 INS START
		gyoFlg |= 0x20;
//--> '15.02.10 INS END
	}
//---------------

	// 5
	j = 0;
	if( Flg == 1 ){
		m_Arith.l_print( dp, m_pH27SnFhyo40Data->Sn_1F4A , FMT15 );
		PF1[i].PITM[j++] = dp;	dp += 16;			// 5-A
		m_Arith.l_print( dp, m_pH27SnFhyo40Data->Sn_1F4B, FMT15 );
		PF1[i].PITM[j++] = dp;	dp += 16;			// 5-B
		m_Arith.l_print( dp, m_pH27SnFhyo40Data->Sn_1F4C, FMT15 );
		PF1[i].PITM[j++] = dp;	dp += 16;			// 5-C
//-- '15.01.28 --
//		m_Arith.l_add( W0 , m_pH27SnFhyo40Data->Sn_1F4A , m_pH27SnFhyo40Data->Sn_1F4B );
//		m_Arith.l_add( W0 , W0 , m_pH27SnFhyo40Data->Sn_1F4C );
//---------------
		if( IsNeedExpCalq() ){
			m_Arith.l_clear( W0 );
			m_Arith.l_add( W0, W0, m_pH27SnFhyo40Data->Sn_1F4T );
		}
		else{
			m_Arith.l_add( W0, m_pH27SnFhyo40Data->Sn_1F4A, m_pH27SnFhyo40Data->Sn_1F4B );
			m_Arith.l_add( W0, W0, m_pH27SnFhyo40Data->Sn_1F4C );
		}
//---------------
		m_Arith.l_print( dp, W0, FMT15 );
		PF1[i].PITM[j++] = dp;	dp += 16;			// 5-D
	}
	else{
		PF1[i].PITM[j++] = (char *)0;
		PF1[i].PITM[j++] = (char *)0;
		PF1[i].PITM[j++] = (char *)0;
		PF1[i].PITM[j++] = (char *)0;
	}
	if( j = _xprn( PP, 1, PF1, 0, pDC, isNoImage, 1 ) )	goto POUT52;

	i = j = 0;
	
	if( Flg == 1 ){
		// íPàÍéñã∆é“
		// éÅñº
		dp = AP;	i = j = 0;
		memset( dp, '\0', size );
		if( kojin ){	// å¬êléñã∆é“ÇÃèÍçáÇÕë„ï\é“éÅñº
			j = sizeof( m_pSnHeadData->Sn_DIHYO );
			memmove( dp, m_pSnHeadData->Sn_DIHYO, j );
		}
		else{			// ñ@êlñº
			j = sizeof( m_pSnHeadData->Sn_CONAM );
			memmove( dp, m_pSnHeadData->Sn_CONAM, j );
		}
		PF1[0].PITM[0] = dp;
		if( j = _xprn( PP, 4, PF1, 2, pDC, 0, 1 ) )	goto POUT52;

		// íPàÍéñã∆é“Å@Åõ
		dp = AP;	i = j = 0;
		memset( dp, '\0', size );
		pbclr( 0, 5, &PF1[0] );			// [07'09.25]
//-- '15.02.10 --
//		if( m_pSnHonpyoData->Sn_UP1 >= 1000 ) PF1[ 0 ].PITM[ 0 ] = "Å@";
//		if( m_pSnHonpyoData->Sn_UP2 >= 1000 ) PF1[ 0 ].PITM[ 1 ] = "Å@";
//		if( m_pSnHonpyoData->Sn_UP3 >= 1000 ) PF1[ 0 ].PITM[ 2 ] = "Å@";
//		if( m_pSnHonpyoData->Sn_UP4 >= 1000 ) PF1[ 0 ].PITM[ 3 ] = "Å@";
//		if( m_pSnHonpyoData->Sn_UP5 >= 1000 ) PF1[ 0 ].PITM[ 4 ] = "Å@";
//---------------
		if( IsNeedExpCalq() == FALSE ){
			if( m_pH27SnHonpyoData->Sn_UP1 >= 1000 ) PF1[ 0 ].PITM[ 0 ] = "Å@";
			if( m_pH27SnHonpyoData->Sn_UP2 >= 1000 ) PF1[ 0 ].PITM[ 1 ] = "Å@";
			if( m_pH27SnHonpyoData->Sn_UP3 >= 1000 ) PF1[ 0 ].PITM[ 2 ] = "Å@";
			if( m_pH27SnHonpyoData->Sn_UP4 >= 1000 ) PF1[ 0 ].PITM[ 3 ] = "Å@";
			if( m_pH27SnHonpyoData->Sn_UP5 >= 1000 ) PF1[ 0 ].PITM[ 4 ] = "Å@";
			if( m_pH27SnHonpyoData->Sn_UP6 >= 1000 ) PF1[ 0 ].PITM[ 5 ] = "Å@";
		}
		else{
			if( gyoFlg&0x01 ){
				PF1[ 0 ].PITM[ 0 ] = "Å@";
			}
			else if( gyoFlg&0x02 ){
				PF1[ 0 ].PITM[ 1 ] = "Å@";
			}
			else if( gyoFlg&0x04 ){
				PF1[ 0 ].PITM[ 2 ] = "Å@";
			}
			else if( gyoFlg&0x08 ){
				PF1[ 0 ].PITM[ 3 ] = "Å@";
			}
			else if( gyoFlg&0x10 ){
				PF1[ 0 ].PITM[ 4 ] = "Å@";
			}
			else if( gyoFlg&0x20 ){
				PF1[ 0 ].PITM[ 5 ] = "Å@";
			}
		}
//---------------
	
		m_pVprn->SetSfncEnv( SElfnc, 1 );
		if( j = _xprn( PP, 5, PF1, 2, pDC, 0, 1 ) )	goto POUT52;

		goto POUT52;		// íPàÍéñã∆
	}

	i = 0, j = 0;
	dp = AP;
	memset( dp, '\0', size );
	pbclr( 0, 2, &PF1[i] );
	pbclr( 0, 1, &PF1[i+1] );
	pbclr( 0, 15, &PF1[i+2] );

	// â€ê≈îÑè„çÇ3%ìKópï™A
//-- '15.02.03 --
//	m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FURITA, FMT15 );
//---------------
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI1A) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI2A) ||
		m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI3A) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI4A) || 
		m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI5A) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI6A) ){
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FURITA, FMT16 );
	}
	else{
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FURITA, FMT15 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// 6-åv
	m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FURI1A, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 7-åv
	m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FURI2A, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 8-åv
	m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FURI3A, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 9-åv
	m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FURI4A, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 10-åv
//-- '15.02.03 --
////	if( ! m_pSnHeadData->SV_5sw ) {
//	if( 1 ) {
//		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FURI5A, FMT15 );
//		PF1[i++].PITM[j] = dp;	dp += 16;		// 11-åv
//	}
//	else	PF1[i++].PITM[j] = (char *)0;
//---------------
	m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FURI5A, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 11-åv
//---------------
	m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FURI6A, FMT15 );
	PF1[i++].PITM[j] = dp;	dp += 16;			// 12-åv
	
	j = 0;				
	// â€ê≈îÑè„çÇ4%ìKópï™B
//-- '15.02.03 --
//	m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FURITB, FMT15 );
//---------------
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI1B) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI2B) ||
		m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI3B) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI4B) || 
		m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI5B) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI6B) ){
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FURITB, FMT16 );
	}
	else{
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FURITB, FMT15 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// 6-åv
	m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FURI1B, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 7-åv
	m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FURI2B, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 8-åv
	m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FURI3B, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 9-åv
	m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FURI4B, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 10-åv
//-- '15.02.03 --
////	if( ! m_pSnHeadData->SV_5sw ) {
//	if( 1 ) {
//		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FURI5B, FMT15 );
//		PF1[i++].PITM[j] = dp;	dp += 16;		// 11-åv
//	}
//	else	PF1[i++].PITM[j] = (char *)0;
//---------------
	m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FURI5B, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 11-åv
//---------------
	m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FURI6B, FMT15 );
	PF1[i++].PITM[j] = dp;	dp += 16;			// 12-åv
	
	j = 0;				
	// â€ê≈îÑè„çÇ6.3%ìKópï™C
//-- '15.02.03 --
//	m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FURITC, FMT15 );
//---------------
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI1C) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI2C) ||
		m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI3C) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI4C) || 
		m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI5C) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI6C) ){
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FURITC, FMT16 );
	}
	else{
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FURITC, FMT16 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// 6-åv
	m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FURI1C, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 7-åv
	m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FURI2C, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 8-åv
	m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FURI3C, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 9-åv
	m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FURI4C, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 10-åv
//-- '15.02.03 --
////	if( ! m_pSnHeadData->SV_5sw ) {
//	if( 1 ) {
//		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FURI5C, FMT15 );
//		PF1[i++].PITM[j] = dp;	dp += 16;		// 11-åv
//	}
//	else	PF1[i++].PITM[j] = (char *)0;
//---------------
	m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FURI5C, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 11-åv
//---------------
	m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FURI6C, FMT15 );
	PF1[i++].PITM[j] = dp;	dp += 16;			// 12-åv

	j = 0;	
	// â€ê≈îÑè„çÇçáåvD
	m_Arith.l_add( W0 , m_pH27SnFhyo52Data->Sn_5FURITA , m_pH27SnFhyo52Data->Sn_5FURITB );
	m_Arith.l_add( W0 , W0 , m_pH27SnFhyo52Data->Sn_5FURITC );
//--> '14.10.07 INS START
	if( m_Arith.l_test(W0) < 0 ){
		m_Arith.l_clear(W0);
	}
//<-- '14.10.07 INS END
//-- '15.01.27 --
//	m_Arith.l_print( dp, W0, FMT15 );
//---------------
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURITA) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURITB) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURITC) ){
		m_Arith.l_print( dp, W0, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W0, FMT15 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// 6
	m_Arith.l_add( W0 , m_pH27SnFhyo52Data->Sn_5FURI1A , m_pH27SnFhyo52Data->Sn_5FURI1B );
	m_Arith.l_add( W0 , W0 , m_pH27SnFhyo52Data->Sn_5FURI1C );
//--> '14.10.07 INS START
	if( m_Arith.l_test(W0) < 0 ){
		m_Arith.l_clear(W0);
	}
//<-- '14.10.07 INS END
//-- '15.01.27 --
//	m_Arith.l_print( dp, W0, FMT15 );
//---------------
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI1A) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI1B) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI1C) ){
		m_Arith.l_print( dp, W0, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W0, FMT15 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// 7
	m_Arith.l_add( W0 , m_pH27SnFhyo52Data->Sn_5FURI2A , m_pH27SnFhyo52Data->Sn_5FURI2B );
	m_Arith.l_add( W0 , W0 , m_pH27SnFhyo52Data->Sn_5FURI2C );
//--> '14.10.07 INS START
	if( m_Arith.l_test(W0) < 0 ){
		m_Arith.l_clear(W0);
	}
//<-- '14.10.07 INS END
//-- '15.01.27 --
//	m_Arith.l_print( dp, W0, FMT15 );
//---------------
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI2A) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI2B) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI2C) ){
		m_Arith.l_print( dp, W0, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W0, FMT15 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// 8
	m_Arith.l_add( W0 , m_pH27SnFhyo52Data->Sn_5FURI3A , m_pH27SnFhyo52Data->Sn_5FURI3B );
	m_Arith.l_add( W0 , W0 , m_pH27SnFhyo52Data->Sn_5FURI3C );
//--> '14.10.07 INS START
	if( m_Arith.l_test(W0) < 0 ){
		m_Arith.l_clear(W0);
	}
//<-- '14.10.07 INS END
//-- '15.01.27 --
//	m_Arith.l_print( dp, W0, FMT15 );
//---------------
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI3A) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI3B) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI3C) ){
		m_Arith.l_print( dp, W0, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W0, FMT15 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// 9
	m_Arith.l_add( W0 , m_pH27SnFhyo52Data->Sn_5FURI4A , m_pH27SnFhyo52Data->Sn_5FURI4B );
	m_Arith.l_add( W0 , W0 , m_pH27SnFhyo52Data->Sn_5FURI4C );
//--> '14.10.07 INS START
	if( m_Arith.l_test(W0) < 0 ){
		m_Arith.l_clear(W0);
	}
//<-- '14.10.07 INS END
//-- '15.01.27 --
//	m_Arith.l_print( dp, W0, FMT15 );
//---------------
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI4A) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI4B) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI4C) ){
		m_Arith.l_print( dp, W0, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W0, FMT15 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// 10

	m_Arith.l_add( W0 , m_pH27SnFhyo52Data->Sn_5FURI5A , m_pH27SnFhyo52Data->Sn_5FURI5B );
	m_Arith.l_add( W0 , W0 , m_pH27SnFhyo52Data->Sn_5FURI5C );
//--> '14.10.07 INS START
	if( m_Arith.l_test(W0) < 0 ){
		m_Arith.l_clear(W0);
	}
//<-- '14.10.07 INS END
//-- '15.01.27 --
//	m_Arith.l_print( dp, W0, FMT15 );
//---------------
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI5A) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI5B) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI5C) ){
		m_Arith.l_print( dp, W0, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W0, FMT15 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// 11

	m_Arith.l_add( W0 , m_pH27SnFhyo52Data->Sn_5FURI6A , m_pH27SnFhyo52Data->Sn_5FURI6B );
	m_Arith.l_add( W0 , W0 , m_pH27SnFhyo52Data->Sn_5FURI6C );
	if( m_Arith.l_test(W0) < 0 ){
		m_Arith.l_clear(W0);
	}
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI6A) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI6B) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FURI6C) ){
		m_Arith.l_print( dp, W0, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W0, FMT15 );
	}
	PF1[i++].PITM[j++] = dp;	dp += 16;		// 12

	pbclr( 0, 6, &PF1[i] );
	pbclr( 0, 6, &PF1[i+1] );
	j = 0;										// äÑçá7->12
	if( m_pH27SnHonpyoData->Sn_UP1 ){
		wsprintf( dp, "%3d.%01d", m_pH27SnHonpyoData->Sn_UP1 / 10, m_pH27SnHonpyoData->Sn_UP1 % 10 );
	}
	if( m_pH27SnHonpyoData->Sn_UP1 >= 1000 ){
		PF1[i+1].PITM[j++] = dp;//ëÂÇ´Ç¢Ç∆Ç´ÇÕ2î‘ñ⁄ÇÃílÇ≈àÛéö
	}
	else{
		PF1[i].PITM[j++] = dp;
	}
	dp += 6;		// îÑè„äÑçá7-åv
	if( m_pH27SnHonpyoData->Sn_UP2 ){
		wsprintf( dp, "%3d.%01d", m_pH27SnHonpyoData->Sn_UP2 / 10, m_pH27SnHonpyoData->Sn_UP2 % 10 );
	}
	if( m_pH27SnHonpyoData->Sn_UP2 >= 1000 ){
		PF1[i+1].PITM[j++] = dp;//ëÂÇ´Ç¢Ç∆Ç´ÇÕ2î‘ñ⁄ÇÃílÇ≈àÛéö
	}
	else{
		PF1[i].PITM[j++] = dp;
	}
	dp += 6;		// îÑè„äÑçá8-åv
	if( m_pH27SnHonpyoData->Sn_UP3 ){
		wsprintf( dp, "%3d.%01d", m_pH27SnHonpyoData->Sn_UP3 / 10, m_pH27SnHonpyoData->Sn_UP3 % 10 );
	}
	if( m_pH27SnHonpyoData->Sn_UP3 >= 1000 ){
		PF1[i+1].PITM[j++] = dp;//ëÂÇ´Ç¢Ç∆Ç´ÇÕ2î‘ñ⁄ÇÃílÇ≈àÛéö
	}
	else{
		PF1[i].PITM[j++] = dp;
	}
	dp += 6;		// îÑè„äÑçá9-åv
	if( m_pH27SnHonpyoData->Sn_UP4 ){
		wsprintf( dp, "%3d.%01d", m_pH27SnHonpyoData->Sn_UP4 / 10, m_pH27SnHonpyoData->Sn_UP4 % 10 );
	}
	if( m_pH27SnHonpyoData->Sn_UP4 >= 1000 ){
		PF1[i+1].PITM[j++] = dp;//ëÂÇ´Ç¢Ç∆Ç´ÇÕ2î‘ñ⁄ÇÃílÇ≈àÛéö
	}
	else{
		PF1[i].PITM[j++] = dp;
	}
	dp += 6;		// îÑè„äÑçá10-åv
	if( m_pH27SnHonpyoData->Sn_UP5 ){
		wsprintf( dp, "%3d.%01d", m_pH27SnHonpyoData->Sn_UP5 / 10, m_pH27SnHonpyoData->Sn_UP5 % 10 );
	}
	if( m_pH27SnHonpyoData->Sn_UP5 >= 1000 ){
		PF1[i+1].PITM[j++] = dp;//ëÂÇ´Ç¢Ç∆Ç´ÇÕ2î‘ñ⁄ÇÃílÇ≈àÛéö
	}
	else{
		PF1[i].PITM[j++] = dp;
	}
	dp += 6;		// îÑè„äÑçá11-åv
	if( m_pH27SnHonpyoData->Sn_UP6 ){
		wsprintf( dp, "%3d.%01d", m_pH27SnHonpyoData->Sn_UP6 / 10, m_pH27SnHonpyoData->Sn_UP6 % 10 );
	}
	if( m_pH27SnHonpyoData->Sn_UP6 >= 1000 ){
		PF1[i+1].PITM[j++] = dp;//ëÂÇ´Ç¢Ç∆Ç´ÇÕ2î‘ñ⁄ÇÃílÇ≈àÛéö
	}
	else{
		PF1[i].PITM[j++] = dp;
	}
	dp += 6;		// îÑè„äÑçá12-åv

	if( j = _xprn( PP, 2, PF1, 0, pDC, 0, 1 ) )	goto POUT52;

	i = 0, j = 0;
	dp = AP;
	memset( dp, '\0', size );
	pbclr( 0, 2, &PF1[i] );
	pbclr( 0, 1, &PF1[i+1] );
	pbclr( 0, 15, &PF1[i+2] );

//--> '14.10.20 INS START
	unsigned char	zeroBuf[MONY_BUF_SIZE] = {0};
//<-- '14.10.20 INS END

//--> '15.02.03 INS START
	BOOL	isTotalZeroOut = FALSE;	// 13DÇÃ0â~ÇèoóÕÇ∑ÇÈÇ©Ç«Ç§Ç©
//--> '15.02.03 INS END

	j = 0;
	// è¡îÔê≈äzÇÃñæç◊
	// è¡îÔê≈äz3%ìKópï™A
//-- '15.01.27 --
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZKTA, FMT15 );
//---------------
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK1A) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK2A) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK3A) ||
		m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK4A) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK5A) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK6A) ){
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FUZKTA, FMT16 );
//--> '15.02.03 INS START
		isTotalZeroOut = TRUE;
//--> '15.02.03 INS END
	}
	else{
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FUZKTA, FMT15 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// 13-åv
//-- '14.10.20 --
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK1A, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// 13
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK2A, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// 14
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK3A, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// 15
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK4A, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// 16
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK5A, FMT15 );
//	PF1[i++].PITM[j] = dp;	dp += 16;			// 17
//---------------
	// 14
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK1A) >= 0 ){
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FUZK1A, FMT15 );
	}
	else{
		m_Arith.l_print( dp, zeroBuf, FMT16 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;
	// 15
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK2A) >= 0 ){
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FUZK2A, FMT15 );
	}
	else{
		m_Arith.l_print( dp, zeroBuf, FMT16 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;
	// 16
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK3A) >= 0 ){
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FUZK3A, FMT15 );
	}
	else{
		m_Arith.l_print( dp, zeroBuf, FMT16 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;
	// 17
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK4A) >= 0 ){
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FUZK4A, FMT15 );
	}
	else{
		m_Arith.l_print( dp, zeroBuf, FMT16 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;
	// 18
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK5A) >= 0 ){
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FUZK5A, FMT15 );
	}
	else{
		m_Arith.l_print( dp, zeroBuf, FMT16 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;
	// 19
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK6A) >= 0 ){
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FUZK6A, FMT15 );
	}
	else{
		m_Arith.l_print( dp, zeroBuf, FMT16 );
	}
	PF1[i++].PITM[j] = dp;	dp += 16;
//---------------

	j = 0;				
	// è¡îÔê≈äz4%ìKópï™B
//-- '15.01.27 --
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZKTB, FMT15 );
//---------------
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK1B) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK2B) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK3B) ||
		m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK4B) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK5B) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK6B) ){
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FUZKTB, FMT16 );
//--> '15.02.03 INS START
		isTotalZeroOut = TRUE;
//--> '15.02.03 INS END
	}
	else{
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FUZKTB, FMT15 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// 13-åv
//-- '14.10.20 --
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK1B, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// 13
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK2B, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// 14
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK3B, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// 15
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK4B, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// 16
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK5B, FMT15 );
//	PF1[i++].PITM[j] = dp;	dp += 16;			// 17
//---------------
	// 14
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK1B) >= 0 ){
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FUZK1B, FMT15 );
	}
	else{
		m_Arith.l_print( dp, zeroBuf, FMT16 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;
	// 15
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK2B) >= 0 ){
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FUZK2B, FMT15 );
	}
	else{
		m_Arith.l_print( dp, zeroBuf, FMT16 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;
	// 16
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK3B) >= 0 ){
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FUZK3B, FMT15 );
	}
	else{
		m_Arith.l_print( dp, zeroBuf, FMT16 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;
	// 17
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK4B) >= 0 ){
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FUZK4B, FMT15 );
	}
	else{
		m_Arith.l_print( dp, zeroBuf, FMT16 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;
	// 18
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK5B) >= 0 ){
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FUZK5B, FMT15 );
	}
	else{
		m_Arith.l_print( dp, zeroBuf, FMT16 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;
	// 19
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK6B) >= 0 ){
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FUZK6B, FMT15 );
	}
	else{
		m_Arith.l_print( dp, zeroBuf, FMT16 );
	}
	PF1[i++].PITM[j] = dp;	dp += 16;
//---------------
		
	j = 0;				
	// è¡îÔê≈äz6.3%ìKópï™C
//-- '15.01.27 --
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZKTC, FMT15 );
//---------------
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK1C) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK2C) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK3C) ||
		m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK4C) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK5C) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK6C) ){
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FUZKTC, FMT16 );
//--> '15.02.03 INS START
		isTotalZeroOut = TRUE;
//--> '15.02.03 INS END
	}
	else{
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FUZKTC, FMT15 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// 13-åv
//-- '14.10.20 --
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK1C, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// 13
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK2C, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// 14
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK3C, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// 15
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK4C, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// 16
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FUZK5C, FMT15 );
//	PF1[i++].PITM[j] = dp;	dp += 16;			// 17
//---------------
	// 14
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK1C) >= 0 ){
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FUZK1C, FMT15 );
	}
	else{
		m_Arith.l_print( dp, zeroBuf, FMT16 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;
	// 15
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK2C) >= 0 ){
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FUZK2C, FMT15 );
	}
	else{
		m_Arith.l_print( dp, zeroBuf, FMT16 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;
	// 16
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK3C) >= 0 ){
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FUZK3C, FMT15 );
	}
	else{
		m_Arith.l_print( dp, zeroBuf, FMT16 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;
	// 17
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK4C) >= 0 ){
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FUZK4C, FMT15 );
	}
	else{
		m_Arith.l_print( dp, zeroBuf, FMT16 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;
	// 18
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK5C) >= 0 ){
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FUZK5C, FMT15 );
	}
	else{
		m_Arith.l_print( dp, zeroBuf, FMT16 );
	}
	PF1[i].PITM[j++] = dp;	dp += 16;
	// 19
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK6C) >= 0 ){
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FUZK6C, FMT15 );
	}
	else{
		m_Arith.l_print( dp, zeroBuf, FMT16 );
	}
	PF1[i++].PITM[j] = dp;	dp += 16;
//---------------

	j = 0;	
	// è¡îÔê≈äzçáåvD
//-- '15.01.26 --
//	m_Arith.l_add( W0 , m_pH27SnFhyo52Data->Sn_5FUZKTA , m_pH27SnFhyo52Data->Sn_5FUZKTB );
//	m_Arith.l_add( W0 , W0 , m_pH27SnFhyo52Data->Sn_5FUZKTC );
//---------------
	m_Arith.l_clear( W0 );
	m_Arith.l_add( W0, W0, m_pH27SnFhyo52Data->Sn_5FUZTT );
//---------------
//--> '14.10.07 INS START
	if( m_Arith.l_test(W0) < 0 ){
		m_Arith.l_clear(W0);
	}
//<-- '14.10.07 INS END

//-- '15.01.27 --
//	m_Arith.l_print( dp, W0, FMT15 );
//---------------
//--- '15.02.02 ---
//	if( m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZKTA) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZKTB) || m_Arith.l_test(m_pSnFhyo52Data->Sn_5FUZKTC) ){
//-----------------
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZKTA) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZKTB) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZKTC) || isTotalZeroOut ){
//-----------------
		m_Arith.l_print( dp, W0, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W0, FMT15 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// 13
	m_Arith.l_add( W0 , m_pH27SnFhyo52Data->Sn_5FUZK1A , m_pH27SnFhyo52Data->Sn_5FUZK1B );
	m_Arith.l_add( W0 , W0 , m_pH27SnFhyo52Data->Sn_5FUZK1C );
//--> '14.10.07 INS START
	if( m_Arith.l_test(W0) < 0 ){
		m_Arith.l_clear(W0);
	}
//<-- '14.10.07 INS END
//-- '15.01.27 --
//	m_Arith.l_print( dp, W0, FMT15 );
//---------------
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK1A) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK1B) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK1C)){
		m_Arith.l_print( dp, W0, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W0, FMT15 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// 14
	m_Arith.l_add( W0 , m_pH27SnFhyo52Data->Sn_5FUZK2A , m_pH27SnFhyo52Data->Sn_5FUZK2B );
	m_Arith.l_add( W0 , W0 , m_pH27SnFhyo52Data->Sn_5FUZK2C );
//--> '14.10.07 INS START
	if( m_Arith.l_test(W0) < 0 ){
		m_Arith.l_clear(W0);
	}
//<-- '14.10.07 INS END
//-- '15.01.27 --
//	m_Arith.l_print( dp, W0, FMT15 );
//---------------
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK2A) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK2B) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK2C)){
		m_Arith.l_print( dp, W0, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W0, FMT15 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// 15
	m_Arith.l_add( W0 , m_pH27SnFhyo52Data->Sn_5FUZK3A , m_pH27SnFhyo52Data->Sn_5FUZK3B );
	m_Arith.l_add( W0 , W0 , m_pH27SnFhyo52Data->Sn_5FUZK3C );
//--> '14.10.07 INS START
	if( m_Arith.l_test(W0) < 0 ){
		m_Arith.l_clear(W0);
	}
//<-- '14.10.07 INS END
//-- '15.01.27 --
//	m_Arith.l_print( dp, W0, FMT15 );
//---------------
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK3A) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK3B) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK3C)){
		m_Arith.l_print( dp, W0, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W0, FMT15 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// 16
	m_Arith.l_add( W0 , m_pH27SnFhyo52Data->Sn_5FUZK4A , m_pH27SnFhyo52Data->Sn_5FUZK4B );
	m_Arith.l_add( W0 , W0 , m_pH27SnFhyo52Data->Sn_5FUZK4C );
//--> '14.10.07 INS START
	if( m_Arith.l_test(W0) < 0 ){
		m_Arith.l_clear(W0);
	}
//<-- '14.10.07 INS END
//-- '15.01.27 --
//	m_Arith.l_print( dp, W0, FMT15 );
//---------------
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK4A) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK4B) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK4C)){
		m_Arith.l_print( dp, W0, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W0, FMT15 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// 17
	m_Arith.l_add( W0 , m_pH27SnFhyo52Data->Sn_5FUZK5A , m_pH27SnFhyo52Data->Sn_5FUZK5B );
	m_Arith.l_add( W0 , W0 , m_pH27SnFhyo52Data->Sn_5FUZK5C );
//--> '14.10.07 INS START
	if( m_Arith.l_test(W0) < 0 ){
		m_Arith.l_clear(W0);
	}
//<-- '14.10.07 INS END
//-- '15.01.27 --
//	m_Arith.l_print( dp, W0, FMT15 );
//---------------
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK5A) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK5B) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK5C)){
		m_Arith.l_print( dp, W0, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W0, FMT15 );
	}
//---------------
	PF1[i].PITM[j++] = dp;	dp += 16;			// 18
	m_Arith.l_add( W0 , m_pH27SnFhyo52Data->Sn_5FUZK6A , m_pH27SnFhyo52Data->Sn_5FUZK6B );
	m_Arith.l_add( W0 , W0 , m_pH27SnFhyo52Data->Sn_5FUZK6C );
	if( m_Arith.l_test(W0) < 0 ){
		m_Arith.l_clear(W0);
	}
//-- '15.01.27 --
//	m_Arith.l_print( dp, W0, FMT15 );
//---------------
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK6A) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK6B) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FUZK6C)){
		m_Arith.l_print( dp, W0, FMT16 );
	}
	else{
		m_Arith.l_print( dp, W0, FMT15 );
	}
//---------------
	PF1[i++].PITM[j++] = dp;	dp += 16;		// 19

	if( j = _xprn( PP, 3, PF1, 0, pDC, 0, 1 ) )	goto POUT52;

	// éÅñº
	dp = AP;	i = j = 0;
	memset( dp, '\0', size );
	if( kojin ) {	// å¬êléñã∆é“ÇÃèÍçáÇÕë„ï\é“éÅñº
		j = sizeof( m_pSnHeadData->Sn_DIHYO );
		memmove( dp, m_pSnHeadData->Sn_DIHYO, j );
	}
	else {			// ñ@êlñº
		j = sizeof( m_pSnHeadData->Sn_CONAM );
		memmove( dp, m_pSnHeadData->Sn_CONAM, j );
	}
	PF1[0].PITM[0] = dp;
	if( j = _xprn( PP, 4, PF1, 2, pDC, 0, 1 ) )	goto POUT52;

POUT52:
	free( AP );
	
	return( j );
}

//-----------------------------------------------------------------------------
// ëÊòZéÌëŒâû ïtï\ÇTÅ|ÇQÅQÇQèoóÕ
//-----------------------------------------------------------------------------
// à¯êî	pDC		ÅF
//		kojin	ÅF
//		pcnt	ÅF
//-----------------------------------------------------------------------------
// ï‘ëóíl
//-----------------------------------------------------------------------------
int CH26HyoView::PrintFuhyo52_2_EX( CDC *pDC, int kojin, int pcnt )
{

	int				i, j;
	unsigned int	size;
	char			*AP, *dp;
	
	BOOL	isNoImage = FALSE;
	if( m_pPrintCmInfo->PRtype&0x80 ){
		isNoImage = FALSE;	// ÉCÉÅÅ[ÉWèoóÕóLÇË
	}
	else{
		isNoImage = TRUE;	// ÉCÉÅÅ[ÉWèoóÕñ≥Çµ
	}
	
	size = 512;
	if( (AP = new char[size]) == NULL ){
		return -1;
	}
	memset( AP, '\0', (sizeof(char)*size) );

	CH27SnFhyo10Data*	m_pH27SnFhyo40Data=NULL;
	CH27SnFhyo50Data*	m_pH27SnFhyo52Data=NULL;
	CH27SnHonpyoData*	m_pH27SnHonpyoData=NULL;

	m_pH27SnFhyo40Data = (CH27SnFhyo10Data*)m_pSnFhyo40Data;
	m_pH27SnFhyo52Data = (CH27SnFhyo50Data*)m_pSnFhyo52Data;
	m_pH27SnHonpyoData = (CH27SnHonpyoData*)m_pSnHonpyoData;

	// èoóÕëOèWåv
//	FuHyo52Calqe();

	PBUF	PF1[30];
	int		PP;

	PP = 8;

	dp = AP;
	memset( dp, '\0', size );
	i = 0;

	int CCSw = 0;
	char	W0[MONY_BUF_SIZE] = {0};

	int		Fno = 0;
	i = 0;
	j = 0;		

	// 20
	m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FSZKGA, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 20-A
	m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FSZKGB, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 20-B
	m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FSZKGC, FMT15 );
	PF1[i].PITM[j++] = dp;	dp += 16;			// 20-C
//-- '14.10.17 --
//	m_Arith.l_add( W0 , m_pSnFhyo52Data->Sn_5FSZKGA , m_pSnFhyo52Data->Sn_5FSZKGB );
//	m_Arith.l_add( W0 , W0 , m_pSnFhyo52Data->Sn_5FSZKGC );
//	m_Arith.l_print( dp, W0 , FMT15 );
//---------------
//--- '15.01.27 ---
//	m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FSZTG , FMT15 );
//---------------
	if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FSZKGA) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FSZKGB) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FSZKGC) ){
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FSZTG , FMT16 );
	}
	else{
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FSZTG , FMT15 );
	}
//-----------------
//---------------
	PF1[i++].PITM[j++] = dp;	dp += 16;		// 20-åv

//--> '15.03.10 INS START
	BOOL	IsOneOver750 = FALSE;
	if( m_pFhyo52EX ){
		int dspSkjSw = m_pFhyo52EX->GetMostProfitCalq();
		if( dspSkjSw&0x02 ){
			IsOneOver750 = TRUE;
		}
	}
//<-- '15.03.10 INS END

	// 21
	j = 0;
//-- '15.03.10 --
//	if( m_pH27SnFhyo52Data->Sn_5FSZsw == 1 ){
//---------------
	if( IsOneOver750 ){
//---------------
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FSZKTA, FMT15 );
		PF1[i].PITM[j++] = dp;	dp += 16;			// 21-A
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FSZKTB, FMT15 );
		PF1[i].PITM[j++] = dp;	dp += 16;			// 21-B
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FSZKTC, FMT15 );
		PF1[i].PITM[j++] = dp;	dp += 16;			// 21-C
//-- '14.10.17 --
//		m_Arith.l_add( W0 , m_pSnFhyo52Data->Sn_5FSZKTA , m_pSnFhyo52Data->Sn_5FSZKTB );
//		m_Arith.l_add( W0 , W0 , m_pSnFhyo52Data->Sn_5FSZKTC );
//		m_Arith.l_print( dp, W0 , FMT15 );
//---------------
//--- '15.01.27 ---
//		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FSZTT, FMT15 );
//---------------
		if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FSZKTA) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FSZKTB) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FSZKTC) ){
			m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FSZTT, FMT16 );
		}
		else{
			m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FSZTT, FMT15 );
		}
//-----------------
//---------------
		PF1[i++].PITM[j++] = dp;	dp += 16;		// 21-åv
	}
	else{
		PF1[i].PITM[j++] = (char *)0;
		PF1[i].PITM[j++] = (char *)0;
		PF1[i].PITM[j++] = (char *)0;
		PF1[i].PITM[j++] = (char *)0;
	}
	if( j = _xprn( PP, 1, PF1, 0, pDC, isNoImage, 1 ) )	goto POUT52;

	i = j = 0;
	
	dp = AP;
	memset( dp, '\0', size );

	char	W1[MONY_BUF_SIZE] = {0};
	char	W2[MONY_BUF_SIZE] = {0};
	char	W3[MONY_BUF_SIZE] = {0};
	char	W4[MONY_BUF_SIZE] = {0};

//-- '14.10.17 --
//	int Sign = m_pSnFhyo52Data->Sn_5FSZsw -2;
//---------------
	int Sign = m_pH27SnFhyo52Data->Sn_5FSZsw -2;
	if( m_pFhyo52EX ){
		int dspSkjSw = m_pFhyo52EX->GetMostProfitCalq();

		Sign = 0;
		for( int i=0, chkflg=0x04; i<15; i++, chkflg<<=1 ){
			if( dspSkjSw&chkflg ){
				break;
			}
			Sign++;
		}
	}
//---------------
	if( Sign >= 0 ){
		switch( Sign ){
			case 0: 
				memmove( W1 , m_pH27SnFhyo52Data->Sn_5FSZT22A , MONY_BUF_SIZE );
				memmove( W2 , m_pH27SnFhyo52Data->Sn_5FSZT22B , MONY_BUF_SIZE );
				memmove( W3 , m_pH27SnFhyo52Data->Sn_5FSZT22C , MONY_BUF_SIZE );
				memmove( W4 , m_pH27SnFhyo52Data->Sn_5FSZT22T , MONY_BUF_SIZE );
				break;
			case 1: 
				memmove( W1 , m_pH27SnFhyo52Data->Sn_5FSZT23A , MONY_BUF_SIZE );
				memmove( W2 , m_pH27SnFhyo52Data->Sn_5FSZT23B , MONY_BUF_SIZE );
				memmove( W3 , m_pH27SnFhyo52Data->Sn_5FSZT23C , MONY_BUF_SIZE );
				memmove( W4 , m_pH27SnFhyo52Data->Sn_5FSZT23T , MONY_BUF_SIZE );
				break;
			case 2: 
				memmove( W1 , m_pH27SnFhyo52Data->Sn_5FSZT24A , MONY_BUF_SIZE );
				memmove( W2 , m_pH27SnFhyo52Data->Sn_5FSZT24B , MONY_BUF_SIZE );
				memmove( W3 , m_pH27SnFhyo52Data->Sn_5FSZT24C , MONY_BUF_SIZE );
				memmove( W4 , m_pH27SnFhyo52Data->Sn_5FSZT24T , MONY_BUF_SIZE );
				break;
			case 3: 
				memmove( W1 , m_pH27SnFhyo52Data->Sn_5FSZT25A , MONY_BUF_SIZE );
				memmove( W2 , m_pH27SnFhyo52Data->Sn_5FSZT25B , MONY_BUF_SIZE );
				memmove( W3 , m_pH27SnFhyo52Data->Sn_5FSZT25C , MONY_BUF_SIZE );
				memmove( W4 , m_pH27SnFhyo52Data->Sn_5FSZT25T , MONY_BUF_SIZE );
				break;
			case 4: 
				memmove( W1 , m_pH27SnFhyo52Data->Sn_5FSZT26A , MONY_BUF_SIZE );
				memmove( W2 , m_pH27SnFhyo52Data->Sn_5FSZT26B , MONY_BUF_SIZE );
				memmove( W3 , m_pH27SnFhyo52Data->Sn_5FSZT26C , MONY_BUF_SIZE );
				memmove( W4 , m_pH27SnFhyo52Data->Sn_5FSZT26T , MONY_BUF_SIZE );
				break;
			case 5: 
				memmove( W1 , m_pH27SnFhyo52Data->Sn_5FSZT27A , MONY_BUF_SIZE );
				memmove( W2 , m_pH27SnFhyo52Data->Sn_5FSZT27B , MONY_BUF_SIZE );
				memmove( W3 , m_pH27SnFhyo52Data->Sn_5FSZT27C , MONY_BUF_SIZE );
				memmove( W4 , m_pH27SnFhyo52Data->Sn_5FSZT27T , MONY_BUF_SIZE );
				break;
			case 6: 
				memmove( W1 , m_pH27SnFhyo52Data->Sn_5FSZT28A , MONY_BUF_SIZE );
				memmove( W2 , m_pH27SnFhyo52Data->Sn_5FSZT28B , MONY_BUF_SIZE );
				memmove( W3 , m_pH27SnFhyo52Data->Sn_5FSZT28C , MONY_BUF_SIZE );
				memmove( W4 , m_pH27SnFhyo52Data->Sn_5FSZT28T , MONY_BUF_SIZE );
				break;
			case 7: 
				memmove( W1 , m_pH27SnFhyo52Data->Sn_5FSZT29A , MONY_BUF_SIZE );
				memmove( W2 , m_pH27SnFhyo52Data->Sn_5FSZT29B , MONY_BUF_SIZE );
				memmove( W3 , m_pH27SnFhyo52Data->Sn_5FSZT29C , MONY_BUF_SIZE );
				memmove( W4 , m_pH27SnFhyo52Data->Sn_5FSZT29T , MONY_BUF_SIZE );
				break;
			case 8: 
				memmove( W1 , m_pH27SnFhyo52Data->Sn_5FSZT30A , MONY_BUF_SIZE );
				memmove( W2 , m_pH27SnFhyo52Data->Sn_5FSZT30B , MONY_BUF_SIZE );
				memmove( W3 , m_pH27SnFhyo52Data->Sn_5FSZT30C , MONY_BUF_SIZE );
				memmove( W4 , m_pH27SnFhyo52Data->Sn_5FSZT30T , MONY_BUF_SIZE );
				break;
			case 9: 
				memmove( W1 , m_pH27SnFhyo52Data->Sn_5FSZT31A , MONY_BUF_SIZE );
				memmove( W2 , m_pH27SnFhyo52Data->Sn_5FSZT31B , MONY_BUF_SIZE );
				memmove( W3 , m_pH27SnFhyo52Data->Sn_5FSZT31C , MONY_BUF_SIZE );
				memmove( W4 , m_pH27SnFhyo52Data->Sn_5FSZT31T , MONY_BUF_SIZE );
				break;
			case 10: 
				memmove( W1 , m_pH27SnFhyo52Data->Sn_5FSZT32A , MONY_BUF_SIZE );
				memmove( W2 , m_pH27SnFhyo52Data->Sn_5FSZT32B , MONY_BUF_SIZE );
				memmove( W3 , m_pH27SnFhyo52Data->Sn_5FSZT32C , MONY_BUF_SIZE );
				memmove( W4 , m_pH27SnFhyo52Data->Sn_5FSZT32T , MONY_BUF_SIZE );
				break;
			case 11: 
				memmove( W1 , m_pH27SnFhyo52Data->Sn_5FSZT33A , MONY_BUF_SIZE );
				memmove( W2 , m_pH27SnFhyo52Data->Sn_5FSZT33B , MONY_BUF_SIZE );
				memmove( W3 , m_pH27SnFhyo52Data->Sn_5FSZT33C , MONY_BUF_SIZE );
				memmove( W4 , m_pH27SnFhyo52Data->Sn_5FSZT33T , MONY_BUF_SIZE );
				break;
			case 12: 
				memmove( W1 , m_pH27SnFhyo52Data->Sn_5FSZT34A , MONY_BUF_SIZE );
				memmove( W2 , m_pH27SnFhyo52Data->Sn_5FSZT34B , MONY_BUF_SIZE );
				memmove( W3 , m_pH27SnFhyo52Data->Sn_5FSZT34C , MONY_BUF_SIZE );
				memmove( W4 , m_pH27SnFhyo52Data->Sn_5FSZT34T , MONY_BUF_SIZE );
				break;
			case 13: 
				memmove( W1 , m_pH27SnFhyo52Data->Sn_5FSZT35A , MONY_BUF_SIZE );
				memmove( W2 , m_pH27SnFhyo52Data->Sn_5FSZT35B , MONY_BUF_SIZE );
				memmove( W3 , m_pH27SnFhyo52Data->Sn_5FSZT35C , MONY_BUF_SIZE );
				memmove( W4 , m_pH27SnFhyo52Data->Sn_5FSZT35T , MONY_BUF_SIZE );
				break;
			case 14: 
				memmove( W1 , m_pH27SnFhyo52Data->Sn_5FSZT36A , MONY_BUF_SIZE );
				memmove( W2 , m_pH27SnFhyo52Data->Sn_5FSZT36B , MONY_BUF_SIZE );
				memmove( W3 , m_pH27SnFhyo52Data->Sn_5FSZT36C , MONY_BUF_SIZE );
				memmove( W4 , m_pH27SnFhyo52Data->Sn_5FSZT36T , MONY_BUF_SIZE );
				break;
		}
	}
	// 22Å`36
//-- '14.10.17 --
//	for(j = 0; j < 10; j++ ) {			// áS->(29)
//		if( m_pSnFhyo52Data->Sn_5FSZsw >= 2 ){
//			if( m_pSnFhyo52Data->Sn_5FSZsw == (j+2) ){
//				m_Arith.l_print( dp, W1, FMT14 );
//				PF1[0].PITM[j] = dp;		dp += 15;
//				m_Arith.l_print( dp, W2, FMT14 );
//				PF1[1].PITM[j] = dp;	dp += 15;
//			}
//			else{
//				PF1[0].PITM[j] = (char *)0;
//				PF1[1].PITM[j] = (char *)0;
//			}
//		}
//		else{
//			PF1[0].PITM[j] = (char *)0;
//			PF1[1].PITM[j] = (char *)0;
//		}
//	}
//---------------
	for( j=0; j<15; j++ ){			// 22 -> 36
		if( Sign >= 0 ){
			if( Sign == j ){
				m_Arith.l_print( dp, W1, FMT14 );
				PF1[0].PITM[j] = dp;		dp += 15;
				m_Arith.l_print( dp, W2, FMT14 );
				PF1[1].PITM[j] = dp;	dp += 15;
			}
			else{
				PF1[0].PITM[j] = (char *)0;
				PF1[1].PITM[j] = (char *)0;
			}
		}
		else{
			PF1[0].PITM[j] = (char *)0;
			PF1[1].PITM[j] = (char *)0;
		}
	}
//---------------
	if( j = _xprn( PP, 2, PF1, 0, pDC, 0, 1 ) )	goto POUT52;

	i = 0, j = 0;
	dp = AP;
	memset( dp, '\0', size );
//-- '14.10.17 --
//	for(j = 0; j < 10; j++ ) {			// áS->(29)
//		if( m_pSnFhyo52Data->Sn_5FSZsw >= 2 ){
//			if( m_pSnFhyo52Data->Sn_5FSZsw == (j+2) ){
//				m_Arith.l_print( dp, W3, FMT14 );
//				PF1[0].PITM[j] = dp;	dp += 15;
//
//				m_Arith.l_print( dp, W4, FMT14 );
//				PF1[1].PITM[j] = dp;	dp += 15;
//			}
//			else{
//				PF1[0].PITM[j] = (char *)0;
//				PF1[1].PITM[j] = (char *)0;
//			}
//		}
//		else{
//			PF1[0].PITM[j] = (char *)0;
//			PF1[1].PITM[j] = (char *)0;
//		}
//	}
//---------------
	for( j=0; j<15; j++ ){			// 22 -> 36
		if( Sign >= 0 ){
			if( Sign == j ){
				m_Arith.l_print( dp, W3, FMT14 );
				PF1[0].PITM[j] = dp;	dp += 15;

				m_Arith.l_print( dp, W4, FMT14 );
				PF1[1].PITM[j] = dp;	dp += 15;
			}
			else{
				PF1[0].PITM[j] = (char *)0;
				PF1[1].PITM[j] = (char *)0;
			}
		}
		else{
			PF1[0].PITM[j] = (char *)0;
			PF1[1].PITM[j] = (char *)0;
		}
	}
//---------------

	if( j = _xprn( PP, 3, PF1, 0, pDC, 0, 1 ) )	goto POUT52;

	dp = AP;	i = j = 0;
	memset( dp, '\0', size );

	// 37ÅFçTèúëŒè€édì¸ê≈äz
/*- '14.03.03 -*/
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZKTA, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// áE-åv
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZKTB, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// áF-åv
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZKTC, FMT15 );
//	PF1[i].PITM[j++] = dp;	dp += 16;			// áF-åv
//	m_Arith.l_print( dp, m_pSnFhyo52Data->Sn_5FSZTT , FMT15 );
//	PF1[i++].PITM[j++] = dp;	dp += 16;			// áI-åv
/*-------------*/

	if( m_pH27SnFhyo52Data->Sn_5FSZsw&0xff ){
		if( m_pSnHeadData->Sn_TOKUR ){
//-- '14.10.17 --
//			memset( W0 , '\0' , sizeof( W0 ) );
//			m_Arith.l_print( dp, m_pSnFhyo40Data->Sn_1F4A, FMT15 );
//			m_Arith.l_add( W0 , W0 , m_pSnFhyo40Data->Sn_1F4A);
//			PF1[i].PITM[j++] = dp;	dp += 16;
//			m_Arith.l_print( dp, m_pSnFhyo40Data->Sn_1F4B, FMT15 );
//			m_Arith.l_add( W0 , W0 , m_pSnFhyo40Data->Sn_1F4B);
//			PF1[i].PITM[j++] = dp;	dp += 16;
//			m_Arith.l_print( dp, m_pSnFhyo40Data->Sn_1F4C, FMT15 );
//			m_Arith.l_add( W0 , W0 , m_pSnFhyo40Data->Sn_1F4C);
//			PF1[i].PITM[j++] = dp;	dp += 16;
//			m_Arith.l_print( dp, W0 , FMT15 );
//			PF1[i++].PITM[j++] = dp;	dp += 16;
//---------------
			m_Arith.l_print( dp, m_pH27SnFhyo40Data->Sn_1F4A, FMT15 );
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print( dp, m_pH27SnFhyo40Data->Sn_1F4B, FMT15 );
			PF1[i].PITM[j++] = dp;	dp += 16;
			m_Arith.l_print( dp, m_pH27SnFhyo40Data->Sn_1F4C, FMT15 );
			PF1[i].PITM[j++] = dp;	dp += 16;
//--- '15.01.27 ---
//			m_Arith.l_print( dp, m_pH27SnFhyo40Data->Sn_1F4T, FMT15 );
//---------------
			if( m_Arith.l_test(m_pH27SnFhyo40Data->Sn_1F4A) || m_Arith.l_test(m_pH27SnFhyo40Data->Sn_1F4B) || m_Arith.l_test(m_pH27SnFhyo40Data->Sn_1F4C) ){
				m_Arith.l_print( dp, m_pH27SnFhyo40Data->Sn_1F4T, FMT16 );
			}
			else{
				m_Arith.l_print( dp, m_pH27SnFhyo40Data->Sn_1F4T, FMT15 );
			}
//-----------------
			PF1[i++].PITM[j++] = dp;	dp += 16;
//---------------
		}
		else{
			m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FSZKGA, FMT15 );
			PF1[i].PITM[j++] = dp;	dp += 16;			// áE-åv
			m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FSZKGB, FMT15 );
			PF1[i].PITM[j++] = dp;	dp += 16;			// áF-åv
			m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FSZKGC, FMT15 );
			PF1[i].PITM[j++] = dp;	dp += 16;			// áF-åv
//-- '15.01.27 --
//			m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FSZTG , FMT15 );
//-----------------
			if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FSZKGA) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FSZKGB) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FSZKGC) ){
				m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FSZTG , FMT16 );
			}
			else{
				m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FSZTG , FMT15 );
			}
//---------------
			PF1[i++].PITM[j++] = dp;	dp += 16;			// áI-åv
		}
	}
	else{
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FSZKGA, FMT15 );
		PF1[i].PITM[j++] = dp;	dp += 16;
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FSZKGB, FMT15 );
		PF1[i].PITM[j++] = dp;	dp += 16;
		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FSZKGC, FMT15 );
		PF1[i].PITM[j++] = dp;	dp += 16;
//-- '15.01.27 --
//		m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FSZTG , FMT15 );
//---------------
		if( m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FSZKGA) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FSZKGB) || m_Arith.l_test(m_pH27SnFhyo52Data->Sn_5FSZKGC) ){
			m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FSZTG , FMT16 );
		}
		else{
			m_Arith.l_print( dp, m_pH27SnFhyo52Data->Sn_5FSZTG , FMT15 );
		}
//---------------
		PF1[i++].PITM[j++] = dp;	dp += 16;
	}
/*-------------*/

	if( j = _xprn( PP, 4, PF1, 0, pDC, 0, 1 ) )	goto POUT52;

	// ì¡ó·åvéZÇìKópÇ≈ÇPéÌóﬁÇÃéñã∆Ç≈ÇVÇTÅìà»è„ÇÃèÍçá
//-- '15.03.10 --
//	if( m_pH27SnFhyo52Data->Sn_5FSZsw == 1 ) {
//---------------
	if( IsOneOver750 ){
//---------------
		dp = AP;	i = j = 0;
		memset( dp, '\0', size );
		pbclr( 0, 5,  &PF1[i] );

//-- '14.10.08 --
//		if( m_pSnHonpyoData->Sn_UP1 >= 750 ) PF1[ 0 ].PITM[ 0 ] = "Å@";
//		if( m_pSnHonpyoData->Sn_UP2 >= 750 ) PF1[ 0 ].PITM[ 1 ] = "Å@";
//		if( m_pSnHonpyoData->Sn_UP3 >= 750 ) PF1[ 0 ].PITM[ 2 ] = "Å@";
//		if( m_pSnHonpyoData->Sn_UP4 >= 750 ) PF1[ 0 ].PITM[ 3 ] = "Å@";
//		if( m_pSnHonpyoData->Sn_UP5 >= 750 ) PF1[ 0 ].PITM[ 4 ] = "Å@";
//---------------
		if( m_pH27SnHonpyoData->Sn_UP1 >= 750 ){
			PF1[ 0 ].PITM[ 0 ] = "Å@";
		}
		else if( m_pH27SnHonpyoData->Sn_UP2 >= 750 ){
			PF1[ 0 ].PITM[ 1 ] = "Å@";
		}
		else if( m_pH27SnHonpyoData->Sn_UP3 >= 750 ){
			PF1[ 0 ].PITM[ 2 ] = "Å@";
		}
		else if( m_pH27SnHonpyoData->Sn_UP4 >= 750 ){
			PF1[ 0 ].PITM[ 3 ] = "Å@";
		}
		else if( m_pH27SnHonpyoData->Sn_UP5 >= 750 ){
			PF1[ 0 ].PITM[ 4 ] = "Å@";
		}
		else if( m_pH27SnHonpyoData->Sn_UP6 >= 750 ){
			PF1[ 0 ].PITM[ 5 ] = "Å@";
		}
//---------------
		//ÉtÉåÅ[ÉÄéÌÇóLå¯Ç∆Ç∑ÇÈà◊ÇÃëOèàóù
		m_pVprn->SetSfncEnv( SElfnc, 1 );
		if( j = _xprn( PP, 5, PF1, 0, pDC, 0, 1 ) )	goto POUT52;
	}

POUT52:
	free( AP );
	
	return( j );

}
//<---------------------------------------------------------------------------------

//2018/03/13 INSERT START
//ïtï\ÇU
int CH26HyoView::PrintFuhyo60( CDC *pDC, int pcnt )
{

	int				i, j;
	char			*AP, *dp, buf[512];
	int ret = 0;

	CString str = _T("");
	unsigned short	SEno = 1;
	BOOL			isNoImage = FALSE;

	if( m_pPrintCmInfo->PRtype&&0x80 ){
		isNoImage = FALSE;
	}
	else{
		isNoImage = TRUE;
	}

	unsigned int	size=2048;
	if( (AP = new char[size]) == NULL ){
		return -1;
	}
	dp = AP;
	memset( dp, '\0', size );
	i=j=0;

	PBUF	PF1[30]={0};
	int		PP = 9;

	char	PW0[6], PW1[6];
	memset( PW0, '\0', 6 );
	m_Arith.l_input( PW0, _T("1000") );
	memset( PW1, '\0', 6 );
	m_Arith.l_input( PW1, _T("100") );


	//ñ{ï\ÉfÅ[É^åvéZ
	ARITH_VAL F60val[6];
	CalqFuhyo60( F60val );

	BOOL IsNofu = FALSE,IsKanpu = FALSE;
	if( m_Arith.l_test( F60val[0].val ) != 0 ){
		IsNofu = TRUE;
	}
	if( m_Arith.l_test( F60val[3].val ) != 0 ){
		IsKanpu = TRUE;
	}


	// ê\çêä˙ä‘é©
	int gengo1 = 0, gengo2 = 0;
	char	yymmdd[3] = {0};
	memset( yymmdd, '\0', sizeof(yymmdd) );
	if( IsJcContinuePrint(false) ){
		m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	}
	else{
		//m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_FROM, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo1);
	}
	wsprintf( dp, _T("%2X   %2X   %2X"), yymmdd[0]&0xff, yymmdd[1]&0xff, yymmdd[2]&0xff );
	PF1[i].PITM[0] = dp;
	dp += 13;

	// ê\çêä˙ä‘éä
	memset( yymmdd, '\0', sizeof(yymmdd) );
	if( IsJcContinuePrint(false) ){
		m_pSnHeadData->GetYmdData(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
	}
	else{
		//m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2]);
		m_pSnHeadData->GetYmdDataGen(ID_ICSSH_KAZEIKIKAN_TO, &yymmdd[0], &yymmdd[1], &yymmdd[2], &gengo2);
	}
	//for( ; j<6; ++j, dp+=3 ){
	//	wsprintf( dp, _T("%02X"), yymmdd[j-3]&0xff );
	//	PF1[i].PITM[j] = dp;
	//}
	wsprintf( dp, _T("%2X   %2X   %2X"), yymmdd[0]&0xff, yymmdd[1]&0xff, yymmdd[2]&0xff );
	PF1[i++].PITM[1] = dp;
	dp += 13;

	if(IsSwitch_HeiseiReiwa(false)){
		if (gengo1 == ID_ICSSH_REIWA_GENGO) {
			memmove(dp, "    ", 4);
			PF1[i].PITM[0] = (char *)0;
			PF1[i].PITM[1] = dp;
			dp += 5;
		}
		else if (gengo1 == ID_ICSSH_HEISEI_GENGO) {
			memmove(dp, "    ", 4);
			PF1[i].PITM[0] = dp;
			PF1[i].PITM[1] = (char *)0;
			dp += 5;
		}
		else{
			PF1[i].PITM[0] = (char *)0;
			PF1[i].PITM[1] = (char *)0;
		}
		i++;

		if (gengo2 == ID_ICSSH_REIWA_GENGO) {
			memmove(dp, "    ", 4);
			PF1[i].PITM[0] = (char *)0;
			PF1[i].PITM[1] = dp;
			dp += 5;
		}
		else if (gengo2 == ID_ICSSH_HEISEI_GENGO) {
			memmove(dp, "    ", 4);
			PF1[i].PITM[0] = dp;
			PF1[i].PITM[1] = (char *)0;
			dp += 5;
		}
		else{
			PF1[i].PITM[0] = dp;
			PF1[i].PITM[1] = (char *)0;
		}
		i++;
	}


	// êÆóùî‘çÜ
	if( (m_pSnHeadData->Sn_SEIRI[0]&0xff) != 0xff ){
		memset( buf, '\0', sizeof( buf ) );
		m_Arith.l_unpac( buf, m_pSnHeadData->Sn_SEIRI, 8 );
		int k;
		for( k=(int)strlen(buf); k>0; ){
			if( buf[--k] != 0x20 ){
				break;
			}
		}
		++k;
		if( k < 8 ){
			memmove( dp, "        ", 8-k );
		}
		memmove( (dp+(8-k)), buf, k );				
		PF1[i++].PITM[0] = dp;
		dp += 9;
	}
	else{
		PF1[i++].PITM[0] = (char *)0;
	}

	if( pcnt == 1 ){
		// î[ê≈ínÅiÇQçsÅj
		memmove( dp, &m_pSnHeadData->Sn_NOZEI[0],  20*2 );				
		PF1[i].PITM[0] = dp;
		dp += 41;
		memmove( dp, &m_pSnHeadData->Sn_NOZEI[40], 20*2 );
		PF1[i++].PITM[1] = dp;
		dp += 41;

		// ë„ï\é“ÅiÉtÉäÉKÉiÅj
		//memmove( dp, m_pSnHeadData->Sn_DIFRI, 68 );
		memmove( dp, m_pSnHeadData->Sn_DIFRI, 50 );
		int len = m_Util.MixSpaceCutLength( (char *)m_pSnHeadData->Sn_DIFRI, sizeof(m_pSnHeadData->Sn_DIFRI) );
		if( len <= 56 ){
			PF1[i].PITM[0] = dp;
		}else{
			PF1[i+1].PITM[0] = dp;
		}
		i=i+2;
		dp += 69;
		
		// ë„ï\é“ñº
		memmove( dp, m_pSnHeadData->Sn_DIHYO, 20*2 );
		len = m_Util.MixSpaceCutLength( (char *)m_pSnHeadData->Sn_DIHYO, sizeof(m_pSnHeadData->Sn_DIHYO) );
		if( len <= 28 ){
			PF1[i].PITM[0] = dp;
		}else{
			PF1[i+1].PITM[0] = dp;
		}
		i=i+2;
		dp += 41;

		//éÄñSîNåéì˙
		if( m_pSnFhyo60Data->Sn_6FSDEATH ){
			//wsprintf( dp, _T("%02X %02X %02X"), m_pSnFhyo60Data->Sn_6FSDEATH[1]&0xff, m_pSnFhyo60Data->Sn_6FSDEATH[2]&0xff, m_pSnFhyo60Data->Sn_6FSDEATH[3]&0xff );
			CVolDateDB	voldate;
			char yy,mm,dd;
			int	yymmdd = 0, g = 0;
//			m_Util.ADtoJC( &nn, &yy, &mm, &dd, m_pSnFhyo60Data->Sn_6FSDEATH );
			if( m_pSnFhyo60Data->Sn_6FSDEATH ){
				//voldate.db_datecnvGen( 0, m_pSnFhyo60Data->Sn_6FSDEATH, &g, &yymmdd, 0, 0 );
				voldate.db_datecnvGen(0, m_pSnFhyo60Data->Sn_6FSDEATH, &g, &yymmdd, 0, 0);
				m_Arith.int_bcd(&yy, yymmdd/10000, 1);
				if( IsJcContinuePrint(false) && (g==5) ){
					yy += 0x30;
				}
				m_Arith.int_bcd( &mm, (yymmdd/100)%100, 1 );
				m_Arith.int_bcd( &dd, yymmdd%100, 1 );
			}
			wsprintf( dp, _T("%2X   %2X   %2X"), yy, mm, dd );
			PF1[i++].PITM[0] = dp;
			dp += 13;

			if(IsSwitch_HeiseiReiwa(false)){
				if (g == ID_ICSSH_REIWA_GENGO) {
					memmove(dp, "    ", 4);
					PF1[i].PITM[0] = (char *)0;
					PF1[i].PITM[1] = dp;
					dp += 5;
				}
				else {
					memmove(dp, "    ", 4);
					PF1[i].PITM[0] = dp;
					PF1[i].PITM[1] = (char *)0;
					dp += 5;
				}
				i++;
			}
		}else{
			i++;
			if (IsSwitch_HeiseiReiwa(false)) {
				i++;
			}
		}

		//ëäë±êlìôÇÃë„ï\é“ÇÃéÅñº
		//memmove( dp, m_pSnFhyo60Data->Sn_6FSDAIHYO, m_pSnFhyo60Data->Sn_6FSDAIHYO.GetLength());
		memset( buf, '\0', sizeof( buf ) );
		wsprintf( buf, _T("%s"), m_pSnFhyo60Data->Sn_6FSDAIHYO.GetBuffer() );
		memmove( dp, buf, (int)strlen( buf ) );
		PF1[i++].PITM[0] = dp;
		dp += 21;

		//å¿íËè≥îF
		if( m_pSnFhyo60Data->Sn_6FSLIMIT == 0 ){
			//pbclr( j, j, &PF1[i++] );
			PF1[i++].PITM[0] = (char *)0;
		}else{
			PF1[i++].PITM[0] = " ";
		}

		//á@Å@î[ÇﬂÇÈè¡îÔê≈ãyÇ—ínï˚è¡îÔê≈ÇÃçáåväz
		m_Arith.l_print( dp, F60val[0].val, FMT16_EX );
		//PF1[i++].PITM[j] = dp;
		PF1[i].PITM[j] = dp;
		dp+=19;
		//áAÅ@á@ÇÃÇ§Çøè¡îÔê≈
		m_Arith.l_print( dp, F60val[1].val, FMT16_EX );
		//PF1[i++].PITM[j] = dp;
		PF1[i].PITM[j+1] = dp;
		dp+=19;
		//áBÅ@á@ÇÃÇ§Çøínï˚è¡îÔê≈
		m_Arith.l_print( dp, F60val[2].val, FMT16_EX );
		PF1[i++].PITM[j+2] = dp;
		dp+=19;
		//áCÅ@ä“ïtÇ≥ÇÍÇÈè¡îÔê≈ãyÇ—ínï˚è¡îÔê≈ÇÃçáåväz
		m_Arith.l_print( dp, F60val[3].val, FMT16_EX );
		//PF1[i++].PITM[j] = dp;
		PF1[i].PITM[j] = dp;
		dp+=19;
		//áDÅ@áCÇÃÇ§Çøè¡îÔê≈
		m_Arith.l_print( dp, F60val[4].val, FMT16_EX );
		//PF1[i++].PITM[j] = dp;
		PF1[i].PITM[j+1] = dp;
		dp+=19;
		//áEÅ@áCÇÃÇ§Çøínï˚è¡îÔê≈
		m_Arith.l_print( dp, F60val[5].val, FMT16_EX );
		//PF1[i++].PITM[j] = dp;
		PF1[i++].PITM[j+2] = dp;
		dp+=19;
	}
	if( ret = _xprn( PP, 1, PF1, 0, pDC, isNoImage, SEno ) ){
		goto POUT60;
	}


	//int CULM_PER_PAGE = 4;
	int idx = (pcnt-1)*CULM_PER_PAGE;

	dp = AP;
	i = j = 0;
	memset( dp, '\0', size );

	for( j = 0; j < CULM_PER_PAGE; idx++, j++ ){
		i=0;

		//èZèä
		int line_end = 0;
		int line_start = 0;
		int isEnd = 0;

		for( int line_num = 0; line_num < 4; line_num++ ){
			if( isEnd == -1 ){
				PF1[i++].PITM[j] =(char *)0;
				continue;
			}
			str = m_pSnFhyo60Data->Sn_6FSJUSYO[idx];
			isEnd = Getindex_toNewLineCode( line_start, &line_end, str );
			if( line_end > 0 ){
				str = m_pSnFhyo60Data->Sn_6FSJUSYO[idx].Mid( line_start, line_end );
				if( str.GetLength() > 0 ){
					line_start += line_end+2;
					memset( buf, '\0', sizeof( buf ) );
					memmove( buf , str , str.GetLength());
					memmove( dp, buf, (int)strlen( buf ) );
					PF1[i++].PITM[j] = dp;
					dp += 21;
				}else{
					PF1[i++].PITM[j] =(char *)0;
				}
			}else{
				line_start += 2;
				PF1[i++].PITM[j] =(char *)0;
			}
		}

		//Ç”ÇËÇ™Ç»
		str = m_pSnFhyo60Data->Sn_6FSFURI[idx];
		memset( buf, '\0', sizeof( buf ) );
		memmove( buf , str , str.GetLength());
		memmove( dp, buf, (int)strlen( buf ) );
		PF1[i++].PITM[j] = dp;
		dp += 21;

		//éÅñº
		str = m_pSnFhyo60Data->Sn_6FSNAME[idx];
		memset( buf, '\0', sizeof( buf ) );
		memmove( buf , str , str.GetLength());
		memmove( dp, buf, (int)strlen( buf ) );
		PF1[i++].PITM[j] = dp;
		dp += 21;

		//å¬êlî‘çÜ
		int size = sizeof(m_pSnFhyo60Data->Sn_6FSHASH[idx]);
		BOOL mynum_empty = TRUE;
		for( int h_cnt = 0; h_cnt < size; h_cnt++ ){
			if( m_pSnFhyo60Data->Sn_6FSHASH[idx][h_cnt] == '\0' ){
				break;
			}
			if( m_pSnFhyo60Data->Sn_6FSHASH[idx][h_cnt] != 0 ){
				mynum_empty = FALSE;
				break;
			}
		}

		if( mynum_empty == FALSE ){
			NODATATBL ndtbl = {0};
			
			NTBMOD8_InitNoDataTbl( 0, &ndtbl );

			memmove( ndtbl.arrow, m_pSnFhyo60Data->Sn_6FSHASH[idx], sizeof( ndtbl.arrow ));

			if( NTBMOD8_GetNoDataTbl( 0, &ndtbl ) == 0 ){

				CICSMNSub8 mnsub;
				CString myno_prn;

				str.Format(ndtbl.no);

				if( m_pPset->psSigCoprn&0x4000){
					myno_prn = mnsub.MakePrintKojinNo( str,  m_pVprn->Get_KojinPrintFlg() );
				}else{
					myno_prn = str;
				}

				memmove( dp, myno_prn, 12 );
				PF1[i++].PITM[j] = dp;
				dp += 13;
			}else{
				PF1[i++].PITM[j] = (char *)0;//pbclr( j, j, &PF1[i++] );
			}
		}else{
			PF1[i++].PITM[j] =(char *)0;//pbclr( j, j, &PF1[i++] );
		}

		//êEã∆
		line_end = 0;
		line_start = 0;
		isEnd = 0;

		str = m_pSnFhyo60Data->Sn_6FSSYOKU[idx];
		if( str.GetLength() <= 10 ){ //ÇPçs
			PF1[i++].PITM[j] = (char *)0;

			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str , str.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 11;

			PF1[i++].PITM[j] = (char *)0;
			PF1[i++].PITM[j] = (char *)0;
			PF1[i++].PITM[j] = (char *)0;
		}else
		if( str.GetLength() <= 22 ){ //ÇQçs
			PF1[i++].PITM[j] = (char *)0;
			PF1[i++].PITM[j] = (char *)0;
			PF1[i++].PITM[j] = (char *)0;

			CString str1,str2;
			str1 = str.Mid(0,10);
			str2 = str.Mid(12,str.GetLength()-12);

			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str1 , str1.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 11;

			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str2 , str2.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 11;
		}else{ //ÇRçs
			CString str1,str2,str3;
			str1 = str.Mid(0,10);
			str2 = str.Mid(12,10);
			str3 = str.Mid(24, str.GetLength()-24);

			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str1 , str1.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 11;

			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str2 , str2.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 11;

			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str3 , str3.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 11;

			PF1[i++].PITM[j] = (char *)0;
			PF1[i++].PITM[j] = (char *)0;
		}


		//ë±ïø
		str = m_pSnFhyo60Data->Sn_6FSTSUDUKI[idx];
		if( str.GetLength() <= 6 ){
			PF1[i++].PITM[j] = (char *)0;
			PF1[i++].PITM[j] = (char *)0;

			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str , str.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 7;
		}else{
			CString str1,str2;
			str1 = str.Mid(0,6);
			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str1 , str1.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 7;

			str2 = str.Mid(6,4);
			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str2 , str2.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 5;

			PF1[i++].PITM[j] = (char *)0;
		}


		//ê∂îNåéì˙
		if(m_pSnFhyo60Data->Sn_6FSBIRTH[idx]){
			char yy,mm,dd;
			CVolDateDB	voldate;
			int	yymmdd = 0, g = 0;
			if( m_pSnFhyo60Data->Sn_6FSBIRTH[idx] <= 19890107 ){	
				//m_Util.ADtoJC( &nengo, &yy, &mm, &dd, m_pSnFhyo60Data->Sn_6FSBIRTH[idx] );
				//g = m_Arith.bcd_int( &nengo, 1 );
				voldate.vd_CeraToJcal( m_pSnFhyo60Data->Sn_6FSBIRTH[idx], &g, &yymmdd );
			}else{
				//db_datecnvGenÇÕïΩê¨à»ç~ÇÃÇ›ëŒâû
				voldate.db_datecnvGen( 0, m_pSnFhyo60Data->Sn_6FSBIRTH[idx], &g, &yymmdd, 0, 0 );
				//wsprintf( &yy, "%s", yymmdd/10000 );
				//wsprintf( &mm, "%s", (yymmdd/100)%100 );
				//wsprintf( &dd, "%s", yymmdd%100 );
			}
			m_Arith.int_bcd( &yy, yymmdd/10000, 1 );
			m_Arith.int_bcd( &mm, (yymmdd/100)%100, 1 );
			m_Arith.int_bcd( &dd, yymmdd%100, 1 );

 			if( g == 0 ){		//ëIëÇ»Çµ
				;
			}else if( g == ID_ICSSH_MEIJI_GENGO ){
				PF1[i].PITM[j] = " ";
			}else if( g == ID_ICSSH_TAISHO_GENGO ){
				PF1[i+1].PITM[j] = " ";
			}else if( g == ID_ICSSH_SHOWA_GENGO ){
				PF1[i+2].PITM[j] = " ";
			}else if( g == ID_ICSSH_HEISEI_GENGO ){
				PF1[i+3].PITM[j] = " ";
			}
			
			if(IsSwitch_HeiseiReiwa(false)){
				if( g == ID_ICSSH_REIWA_GENGO ){
					PF1[i+4].PITM[j] = " ";
				}
				i = i + 5;
			}else{
				i = i + 4;
			}

			wsprintf( dp, _T("%2X    %2X    %2X"), yy, mm, dd );
			PF1[i++].PITM[j] = dp;
			dp += 15;
		}else{
			if(IsSwitch_HeiseiReiwa(false)){
				i = i + 6;
			}
			else {
				i = i + 5;
			}
		}

		//ìdòbî‘çÜ	
		m_pSnFhyo60Data->Sn_6FSTEL1[idx];
		m_pSnFhyo60Data->Sn_6FSTEL2[idx];
		m_pSnFhyo60Data->Sn_6FSTEL3[idx];
		wsprintf( dp, _T("%6s"), m_pSnFhyo60Data->Sn_6FSTEL1[idx]);
		PF1[i++].PITM[j] = dp;
		dp += 7;
		wsprintf( dp, _T("%4s"), m_pSnFhyo60Data->Sn_6FSTEL2[idx]);
		PF1[i++].PITM[j] = dp;
		dp += 5;
		wsprintf( dp, _T("%4s"), m_pSnFhyo60Data->Sn_6FSTEL3[idx] );
		PF1[i++].PITM[j] = dp;
		dp += 5;

		//ëäë±ï™
		PF1[i].PITM[j] = (char *)0;
		PF1[i+1].PITM[j] = (char *)0;
		if( m_pSnFhyo60Data->Sn_6FSHOTEI[idx] == 1 ){
			PF1[i].PITM[j] = " ";
		}else if( m_pSnFhyo60Data->Sn_6FSHOTEI[idx] == 2 ){
			PF1[i+1].PITM[j] = " ";
		}
		i=i+2;

		str = m_pSnFhyo60Data->Sn_6FSBUNSHI[idx];
		memset( buf, '\0', sizeof( buf ) );
		memmove( buf , str , str.GetLength());
		memmove( dp, buf, (int)strlen( buf ) );
		PF1[i++].PITM[j] = dp;
		dp += 4;

		str = m_pSnFhyo60Data->Sn_6FSBUNBO[idx];
		memset( buf, '\0', sizeof( buf ) );
		memmove( buf , str , str.GetLength());
		memmove( dp, buf, (int)strlen( buf ) );
		PF1[i++].PITM[j] = dp;
		dp += 4;

		//ëäë±ç‡éYÇÃâøäz
		m_Arith.l_print( dp, m_pSnFhyo60Data->Sn_6FSKAGAKU[idx], FMT16_EX );
		PF1[i++].PITM[j] = dp;
		dp+=19;
	}

	if( ret = _xprn( PP, 2, PF1, 0, pDC, isNoImage, SEno ) ){
		goto POUT60;
	}

	idx = (pcnt-1)*CULM_PER_PAGE;

	dp = AP;
	i = j = 0;
	memset( dp, '\0', size );

	char bunshi[MONY_BUF_SIZE] = {0};
	char bunbo[MONY_BUF_SIZE] = {0};

	char zei_nofu[CULM_PER_PAGE][MONY_BUF_SIZE] = {0};
	char chihozei_nofu[CULM_PER_PAGE][MONY_BUF_SIZE] = {0};
	char total_nofu[CULM_PER_PAGE][MONY_BUF_SIZE] = {0};
	char zei_kanpu[CULM_PER_PAGE][MONY_BUF_SIZE] = {0};
	char chihozei_kanpu[CULM_PER_PAGE][MONY_BUF_SIZE] = {0};
	char total_kanpu[CULM_PER_PAGE][MONY_BUF_SIZE] = {0};


	for( j = 0; j < CULM_PER_PAGE; idx++, j++ ){

		i=0;

		//î[ït(ä“ït)ê≈äzÇÃåvéZ

		memset( buf, '\0', sizeof( buf ) );
		wsprintf( buf, _T("%s"), m_pSnFhyo60Data->Sn_6FSBUNBO[idx].GetBuffer() );
		m_Arith.l_input( bunbo, buf );

		memset( buf, '\0', sizeof( buf ) );
		wsprintf( buf, _T("%s"), m_pSnFhyo60Data->Sn_6FSBUNSHI[idx].GetBuffer() );
		m_Arith.l_input( bunshi, buf );

		if( IsNofu ){
			//áH
			if( m_pSnFhyo60Data->Sn_6FSNOFUKANPUsw[idx]&0x01 ){
				memmove( zei_nofu[j], m_pSnFhyo60Data->Sn_6FSNOFUZEI[idx], MONY_BUF_SIZE );
			}else{
				if( m_Arith.l_test( bunbo ) != 0 ){
					m_Arith.l_mul( zei_nofu[j], bunshi, F60val[1].val );
					m_Arith.l_div( zei_nofu[j], zei_nofu[j], bunbo );
					m_Arith.l_div( zei_nofu[j], zei_nofu[j], PW1 ); //100â~ñ¢ñûêÿÇËéÃÇƒ
					m_Arith.l_mul( zei_nofu[j], zei_nofu[j], PW1 );
				}
			}
			//áI
			if( m_pSnFhyo60Data->Sn_6FSNOFUKANPUsw[idx]&0x02 ){
				memmove( chihozei_nofu[j], m_pSnFhyo60Data->Sn_6FSNOFUCHIHO[idx], MONY_BUF_SIZE );
			}else{
				if( m_Arith.l_test( bunbo ) != 0 ){
					m_Arith.l_mul( chihozei_nofu[j], bunshi, F60val[2].val );
					m_Arith.l_div( chihozei_nofu[j], chihozei_nofu[j], bunbo );
					m_Arith.l_div( chihozei_nofu[j], chihozei_nofu[j], PW1 ); //100â~ñ¢ñûêÿÇËéÃÇƒ
					m_Arith.l_mul( chihozei_nofu[j], chihozei_nofu[j], PW1 );
				}
			}
			//áJ
			m_Arith.l_add( total_nofu[j], zei_nofu[j], chihozei_nofu[j] );
		}
		if( IsKanpu ){
			//áK
			//m_Arith.l_mul( zei_kanpu[j], bunshi, F60val[4].val );
			//m_Arith.l_div( zei_kanpu[j], zei_kanpu[j], bunbo ); //ä“ïtÇÕ1â~ñ¢ñûêÿÇËéÃÇƒ
			memmove( zei_kanpu[j],m_pSnFhyo60Data->Sn_6FSKANPUZEI[idx],MONY_BUF_SIZE );
			//áL
			//m_Arith.l_mul( chihozei_kanpu[j], bunshi, F60val[5].val );
			//m_Arith.l_div( chihozei_kanpu[j], chihozei_kanpu[j], bunbo ); //ä“ïtÇÕ1â~ñ¢ñûêÿÇËéÃÇƒ
			memmove( chihozei_kanpu[j],m_pSnFhyo60Data->Sn_6FSKANPUCHIHO[idx],MONY_BUF_SIZE );
			//áM
			m_Arith.l_add( total_kanpu[j], zei_kanpu[j], chihozei_kanpu[j] );
			if( m_Arith.l_test(total_kanpu[j]) < 0 ){
				m_Arith.l_neg(total_kanpu[j]);
			}
		}

		m_Arith.l_print( dp, zei_nofu[j], FMT16_EX );
		PF1[i++].PITM[j] = dp;
		dp+=19;
		m_Arith.l_print( dp, chihozei_nofu[j], FMT16_EX );
		PF1[i++].PITM[j] = dp;
		dp+=19;
		m_Arith.l_print( dp, total_nofu[j], FMT16_EX );
		PF1[i++].PITM[j] = dp;
		dp+=19;
		m_Arith.l_print( dp, zei_kanpu[j], FMT16_EX );
		PF1[i++].PITM[j] = dp;
		dp+=19;
		m_Arith.l_print( dp, chihozei_kanpu[j], FMT16_EX );
		PF1[i++].PITM[j] = dp;
		dp+=19;
		m_Arith.l_print( dp, total_kanpu[j], FMT16_EX );
		PF1[i++].PITM[j] = dp;
		dp+=19;
	}
	if( ret = _xprn( PP, 3, PF1, 0, pDC, isNoImage, SEno ) ){
		goto POUT60;
	}


	if(( IsKanpu == TRUE )|| m_pPrintCmInfo->OPI ){

		idx = (pcnt-1)*CULM_PER_PAGE;

		dp = AP;
		i = j = 0;
		memset( dp, '\0', size );

		for( j = 0; j < CULM_PER_PAGE; idx++, j++ ){

			i=0;

			//ã‚çsñºìô
			if( m_pSnFhyo60Data->Sn_6FSGNAME[idx].GetLength() > 10 ){
				str = m_pSnFhyo60Data->Sn_6FSGNAME[idx].Mid(0,10);
				memset( buf, '\0', sizeof( buf ) );
				memmove( buf , str , str.GetLength());
				memmove( dp, buf, (int)strlen( buf ) );
				PF1[i++].PITM[j] = dp;
				dp += 11;

				str = m_pSnFhyo60Data->Sn_6FSGNAME[idx].Mid(10,10);
				memset( buf, '\0', sizeof( buf ) );
				memmove( buf , str , str.GetLength());
				memmove( dp, buf, (int)strlen( buf ) );
				PF1[i++].PITM[j] = dp;
				dp += 11;

				PF1[i++].PITM[j] = (char *)0;
			}else{
				PF1[i++].PITM[j] = (char *)0;
				PF1[i++].PITM[j] = (char *)0;

				str = m_pSnFhyo60Data->Sn_6FSGNAME[idx];
				memset( buf, '\0', sizeof( buf ) );
				memmove( buf , str , str.GetLength());
				memmove( dp, buf, (int)strlen( buf ) );
				PF1[i++].PITM[j] = dp;
				dp += 11;
			}

			PF1[i].PITM[j] = (char *)0;
			PF1[i+1].PITM[j] = (char *)0;
			PF1[i+2].PITM[j] = (char *)0;
			PF1[i+3].PITM[j] = (char *)0;
			PF1[i+4].PITM[j] = (char *)0;
			CString str = m_pSnFhyo60Data->Sn_6FSGNAME[idx];
			str.Replace(_T("Å@"),"");
			if( str.Trim().GetLength() ){
				switch(m_pSnFhyo60Data->Sn_6FSGSEL[idx]){
					case 0: //ã‚çs
						PF1[i].PITM[j] = " ";
						break;
					case 1: //ã‡å…
						PF1[i+1].PITM[j] = " ";
						break;
					case 2: //ëgçá
						PF1[i+2].PITM[j] = " ";
						break;
					case 3: //î_ã¶
						PF1[i+3].PITM[j] = " ";
						break;
					case 4: //ãôã¶
						PF1[i+4].PITM[j] = " ";
						break;
					default:
						break;
				}
			}
			i = i+5;

			//éxìXñºìô
			if( m_pSnFhyo60Data->Sn_6FSSNAME[idx].GetLength() > 10 ){
				str = m_pSnFhyo60Data->Sn_6FSSNAME[idx].Mid(0,10);
				memset( buf, '\0', sizeof( buf ) );
				memmove( buf , str , str.GetLength());
				memmove( dp, buf, (int)strlen( buf ) );
				PF1[i++].PITM[j] = dp;
				dp += 11;

				str = m_pSnFhyo60Data->Sn_6FSSNAME[idx].Mid(10,10);
				memset( buf, '\0', sizeof( buf ) );
				memmove( buf , str , str.GetLength());
				memmove( dp, buf, (int)strlen( buf ) );
				PF1[i++].PITM[j] = dp;
				dp += 11;

				PF1[i++].PITM[j] = (char *)0;
			}else{
				PF1[i++].PITM[j] = (char *)0;
				PF1[i++].PITM[j] = (char *)0;

				str = m_pSnFhyo60Data->Sn_6FSSNAME[idx];
				memset( buf, '\0', sizeof( buf ) );
				memmove( buf , str , str.GetLength());
				memmove( dp, buf, (int)strlen( buf ) );
				PF1[i++].PITM[j] = dp;
				dp += 11;
			}

			PF1[i].PITM[j] = (char *)0;
			PF1[i+1].PITM[j] = (char *)0;
			PF1[i+2].PITM[j] = (char *)0;
			PF1[i+3].PITM[j] = (char *)0;
			PF1[i+4].PITM[j] = (char *)0;
			CString str1,str2;
			str1 = m_pSnFhyo60Data->Sn_6FSGNAME[idx];
			str2 = m_pSnFhyo60Data->Sn_6FSSNAME[idx];
			str1.Replace(_T("Å@"),"");
			str2.Replace(_T("Å@"),"");
			if(( str1.Trim().GetLength() != 0 )||( str2.Trim().GetLength() != 0 )){ //ã‚çsñºì¸óÕçœÇ≈Ç‡ñ{ìXéxìXëIëï\é¶
				switch(m_pSnFhyo60Data->Sn_6FSSSEL[idx]){
					case 0: //ñ{ìX
						PF1[i].PITM[j] = " ";
						break;
					case 1: //éxìX
						PF1[i+1].PITM[j] = " ";
						break;
					case 2: //ñ{èä
						PF1[i+3].PITM[j] = " ";
						break;
					case 3: //éxèä
						PF1[i+4].PITM[j] = " ";
						break;
					case 5: //èoí£èä
						PF1[i+2].PITM[j] = " ";
						break;
					case 4: //Ç»Çµ
					case 6: //è„ãLà»äO
						break;
					default:
						break;
				}
			}
			i = i+5;

			//óaã‡ÇÃéÌóﬁ
			str = m_pSnFhyo60Data->Sn_6FSYOKIN[idx];
			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str , str.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 17;

			//å˚ç¿î‘çÜ
			str = m_pSnFhyo60Data->Sn_6FSKOUZA[idx];
			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str , str.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 11;

			//ãLçÜî‘çÜ
			str = m_pSnFhyo60Data->Sn_6FSKIGO1[idx];
			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str , str.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 6;

			str = m_pSnFhyo60Data->Sn_6FSKIGO2[idx];
			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str , str.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
			dp += 9;

			//óXï÷ã«ñº
			str = m_pSnFhyo60Data->Sn_6FSYUBIN[idx];
			memset( buf, '\0', sizeof( buf ) );
			memmove( buf , str , str.GetLength());
			memmove( dp, buf, (int)strlen( buf ) );
			PF1[i++].PITM[j] = dp;
//2018.08.21 INSERT START
			dp += 17;
//2018.08.21 INSERT END
		}
		if( ret = _xprn( PP, 4, PF1, 0, pDC, isNoImage, SEno ) ){
			goto POUT60;
		}
	}

POUT60:
	if( AP ){
		delete [] AP;
	}

	return ret;
}

int CH26HyoView::CalqFuhyo60( ARITH_VAL *F60val )
{
	int				i, j;

	CString str = _T("");

	char	PW0[6], PW1[6];
	memset( PW0, '\0', 6 );
	m_Arith.l_input( PW0, _T("1000") );
	memset( PW1, '\0', 6 );
	m_Arith.l_input( PW1, _T("100") );


	char	WORK0[MONY_BUF_SIZE] = {0};
	char	WORK_ALL[MONY_BUF_SIZE] = {0};
	// åvéZåãâ ÉoÉbÉtÉ@
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
	m_Arith.l_input( val100 , "100" );

	CH26SnFhyo10Data*	PrintData;

	if( m_pSnFhyo10Data ){
		//ïtï\1ÉfÅ[É^ì«çûÇ›ÉNÉâÉX
		PrintData = m_pSnFhyo10Data;
	}
	else{
		//ïtï\4ÉfÅ[É^ì«çûÇ›ÉNÉâÉX
		PrintData = m_pSnFhyo40Data;
	}

	//á@
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		m_Arith.l_add( kingaku , PrintData->Sn_1F1A , PrintData->Sn_1F1B );
	}
	m_Arith.l_add( kingaku , kingaku , PrintData->Sn_1F1C );
	memmove( ANS[0].val , kingaku , MONY_BUF_SIZE);

	//áA
	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		m_Arith.l_add( WORK2 , PrintData->Sn_1F2A , PrintData->Sn_1F2B );
		m_Arith.l_add( WORK2 , WORK2 , PrintData->Sn_1F2C );
	}
	else{
		memmove( WORK2 , m_pSnHonpyoData->Sn_SYTX , MONY_BUF_SIZE);
	}
	memmove( ANS[1].val , WORK2 , MONY_BUF_SIZE);

	//áB
	memset( kingaku3 , '\0' , MONY_BUF_SIZE);
	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		m_Arith.l_add( WORK3 , PrintData->Sn_1F3A , PrintData->Sn_1F3B );
	}
	m_Arith.l_add( WORK3 , WORK3 , PrintData->Sn_1F3C );
	memmove( ANS[2].val , WORK3 , MONY_BUF_SIZE);
	//áC
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	if( m_pSnFhyo40Data ){
		if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
			m_Arith.l_add( kingaku , kingaku , PrintData->Sn_1F4T );
		}
		else{
			m_Arith.l_add( kingaku , kingaku , PrintData->Sn_1F4C );
		}
	}
	else{
		if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
			m_Arith.l_add( kingaku , PrintData->Sn_1F4A , PrintData->Sn_1F4B );
		}
		m_Arith.l_add( kingaku , kingaku , PrintData->Sn_1F4C );
	}
	memmove( ANS[3].val , kingaku , MONY_BUF_SIZE);
	m_Arith.l_add( WORK7 , WORK7 ,kingaku );
	//áD
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		m_Arith.l_add( kingaku , PrintData->Sn_1F5A , PrintData->Sn_1F5B );
	}
	m_Arith.l_add( kingaku , kingaku , PrintData->Sn_1F5C );
	memmove( ANS[4].val , kingaku , MONY_BUF_SIZE);
	m_Arith.l_add( WORK7 , WORK7 ,kingaku );
	//áE
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		m_Arith.l_add( kingaku , PrintData->Sn_1F6A , PrintData->Sn_1F6B );
	}
	m_Arith.l_add( kingaku , kingaku , PrintData->Sn_1F6C );
	memmove( ANS[5].val , kingaku , MONY_BUF_SIZE);
	m_Arith.l_add( WORK7 , WORK7 ,kingaku );
	//áF
	memmove( ANS[6].val , WORK7 , MONY_BUF_SIZE);
	//áGáH 7-2-3
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	m_Arith.l_sub( kingaku , WORK7 , WORK2 );
	m_Arith.l_sub( kingaku , kingaku , WORK3 );
	m_Arith.l_add( WORK8 , WORK8 , kingaku );
	
	if( m_Arith.l_test( kingaku )  < 0 ){//É}ÉCÉiÉXÇÃèÍçá
		m_Arith.l_neg( kingaku );
		memmove( ANS[8].val , kingaku , MONY_BUF_SIZE);//áH
		m_Arith.l_div( ANS[8].val , ANS[8].val , PW1 );
		m_Arith.l_mul( ANS[8].val , ANS[8].val , PW1 );
	}
	else{//ÉvÉâÉXÇÃèÍçá
		if( m_pSnHeadData->Sn_SKKBN%2 ){//íÜä‘ÇÃèÍçáÅAä“ïtéûÇÕï\é¶ÇµÇ»Ç¢
			memmove( ANS[7].val , kingaku , MONY_BUF_SIZE);//áG
		}
		else{
			memset( kingaku , '\0' , MONY_BUF_SIZE );
		}
	}

	//áI
	memmove( ANS[9].val, m_pSnHonpyoData->Sn_TYNOFZ, MONY_BUF_SIZE );

	//áJ 9-10
	m_Arith.l_sub( kingaku , ANS[8].val , m_pSnHonpyoData->Sn_TYNOFZ);

	if( m_Arith.l_test( kingaku )  < 0 ){//É}ÉCÉiÉXÇÃèÍçá
		m_Arith.l_neg( kingaku );
		memmove( ANS[11].val , kingaku , MONY_BUF_SIZE);//áJ
	}
	else{//ÉvÉâÉXÇÃèÍçá
		memmove( ANS[10].val , kingaku , MONY_BUF_SIZE);//áK
	}
	//áL
	if( m_pSnHeadData->Sn_SKKBN == 3 || m_pSnHeadData->Sn_SKKBN == 4 ){//èCê≥ÇÃèÍçáÇÃÇ›
		memmove( ANS[12].val, m_pSnHonpyoData->Sn_KAKTIZ, MONY_BUF_SIZE );
		//áMç∑à¯î[ïtê≈äz
		if( m_Arith.l_test(ANS[10].val) > 0 ){
			m_Arith.l_sub( ANS[13].val , ANS[10].val , m_pSnHonpyoData->Sn_KAKTIZ );
		}
		else{
			m_Arith.l_sub( ANS[13].val , ANS[13].val, ANS[11].val );
			m_Arith.l_sub( ANS[13].val , ANS[13].val, m_pSnHonpyoData->Sn_KAKTIZ );
		}
		m_Arith.l_sub( ANS[13].val , ANS[13].val, ANS[7].val );
	}
	//áNâ€ê≈éëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
	if( !(m_pSnHeadData->m_s_sgn2&0x0f) && (m_Arith.l_test(m_pSnHonpyoData->Sn_KZURI)<0) ){
		memset( ANS[14].val, '\0', MONY_BUF_SIZE );
	}
	else{
		memmove( ANS[14].val, m_pSnHonpyoData->Sn_KZURI, MONY_BUF_SIZE );
	}
	if( m_pSnHeadData->m_s_sgn2&0x0f){
		//áOäÓèÄä˙ä‘ÇÃâ€ê≈îÑè„çÇ
		memmove( ANS[15].val , m_pSnHonpyoData->Sn_ZZURI , MONY_BUF_SIZE );
	}
	else{
		//áOéëéYÇÃè˜ìnìôÇÃëŒâøÇÃäz
		if( m_Arith.l_test(m_pSnHonpyoData->Sn_SOURI) < 0 ){
			memset( ANS[15].val, '\0', MONY_BUF_SIZE );
		}
		else{
			memmove( ANS[15].val , m_pSnHonpyoData->Sn_SOURI , MONY_BUF_SIZE );
		}
	}

	//áPçTèúïsë´ä“ïtê≈äz
	//áQç∑à¯ê≈äz
	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		char	WORK0[MONY_BUF_SIZE]={0};
		char	WORK1[MONY_BUF_SIZE]={0};
		char	WORK2[MONY_BUF_SIZE]={0};
		m_Arith.l_add( WORK0 , PrintData->Sn_1F8B , PrintData->Sn_1F8C );//8
		m_Arith.l_add( WORK1 , PrintData->Sn_1F9B , PrintData->Sn_1F9C );//9
		m_Arith.l_sub( WORK2 , WORK1 , WORK0 );//9-8		if( m_Arith.l_test( kingaku ) < 0 ){//É}ÉCÉiÉXÇÃèÍçááPÇ…
		if( m_Arith.l_test( WORK2 ) < 0 ){
			if( m_pSnHeadData->Sn_SKKBN%2 ){//íÜä‘ÇÃèÍçáÅAä“ïtéûÇÕï\é¶ÇµÇ»Ç¢
				m_Arith.l_neg( WORK2 );
				memmove(  ANS[16].val , WORK2 , MONY_BUF_SIZE );//áQ
			}
		}
		else{//ÉvÉâÉXÇÃèÍçááQÇ…
			memmove(  ANS[17].val , WORK2 , MONY_BUF_SIZE );//áP
		}
	}
	else{
		memmove( kingaku , WORK8 , MONY_BUF_SIZE);
		if( m_Arith.l_test( kingaku ) < 0 ){//É}ÉCÉiÉXÇÃèÍçááPÇ…
			m_Arith.l_neg( kingaku );
			memmove(  ANS[17].val , kingaku , MONY_BUF_SIZE );//áQ
			m_Arith.l_div( ANS[17].val , ANS[17].val , PW1 );
			m_Arith.l_mul( ANS[17].val , ANS[17].val , PW1 );
		}
		else{//ÉvÉâÉXÇÃèÍçááQÇ…
			if( m_pSnHeadData->Sn_SKKBN != 2  ){
				if( m_pSnHeadData->Sn_SKKBN%2 ){//íÜä‘ÇÃèÍçáÅAä“ïtéûÇÕï\é¶ÇµÇ»Ç¢
					memmove(  ANS[16].val , kingaku , MONY_BUF_SIZE );//áP
				}
			}
		}
	}

	//áRè˜ìnäÑäzä“ïtäz
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	memset( kingaku2 , '\0' , MONY_BUF_SIZE);
		
	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		m_Util.percent( kingaku,(char*)PrintData->Sn_1F8B, 7, 25, 0 );
		m_Arith.l_add(kingaku2 , kingaku2 , kingaku);
	}
	m_Util.percent( kingaku,(char*)PrintData->Sn_1F8C, 8, 17, 0 );
	m_Arith.l_add(kingaku2, kingaku2 , kingaku);

	//20è˜ìnäÑäzî[ê≈äz
	memset( kingaku , '\0' , MONY_BUF_SIZE);
	memset( kingaku3 , '\0' , MONY_BUF_SIZE);
	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		m_Util.percent( kingaku, (char*)PrintData->Sn_1F9B, 7, 25, 0 );
		m_Arith.l_add(kingaku3 , kingaku3 , kingaku);
		m_Util.percent( kingaku, (char*)PrintData->Sn_1F9C, 8, 17, 0 );
		m_Arith.l_add(kingaku3 , kingaku3 , kingaku);
		m_Arith.l_sub(kingaku3 , kingaku3 , kingaku2);
		if( m_Arith.l_test( kingaku3 ) < 0 ){//É}ÉCÉiÉXÇÃèÍçááRÇ…
			if( (m_pSnHeadData->Sn_SKKBN%2)  ){
				memmove(  ANS[18].val , kingaku3 , MONY_BUF_SIZE );
				m_Arith.l_neg(  ANS[18].val );
			}
		}
		else{
			memmove(  ANS[19].val , kingaku3 , MONY_BUF_SIZE );
			m_Arith.l_div( ANS[19].val , ANS[19].val , PW1 );
			m_Arith.l_mul( ANS[19].val , ANS[19].val , PW1 );
		}
	}
	else{
		if( m_Arith.l_test( ANS[17].val ) ){
			m_Util.percent( kingaku,(char*)ANS[17].val, 8, 17, 0 );
			m_Arith.l_add(kingaku3 , kingaku3 , kingaku);
			m_Arith.l_div( ANS[19].val , kingaku3 , PW1 );
			m_Arith.l_mul( ANS[19].val , ANS[19].val , PW1 );
		}
		else{
			m_Util.percent( kingaku,(char*)ANS[16].val, 8, 17, 0 );
			m_Arith.l_add(kingaku3 , kingaku3 , kingaku);
			memmove(  ANS[18].val , kingaku3 , MONY_BUF_SIZE );
		}
	}

	//21íÜä‘î[ïtè˜ìnäÑäz
	memmove( ANS[20].val , m_pSnHonpyoData->Sn_TTYWAR , MONY_BUF_SIZE);
	//22
	m_Arith.l_sub( kingaku , ANS[19].val, ANS[20].val );
	if( m_Arith.l_test( kingaku ) < 0 ){//É}ÉCÉiÉXÇÃèÍçááRÇ…
		memmove(  ANS[22].val , kingaku , MONY_BUF_SIZE );
		m_Arith.l_neg(  ANS[22].val );
		m_Arith.l_div( ANS[22].val , ANS[22].val , PW1 );
		m_Arith.l_mul( ANS[22].val , ANS[22].val , PW1 );
	}
	else{
		memmove(  ANS[21].val , kingaku , MONY_BUF_SIZE );
	}
	if(  m_pSnHeadData->Sn_SKKBN == 3 ||  m_pSnHeadData->Sn_SKKBN == 4 ){
		//24
		memmove( ANS[23].val , m_pSnHonpyoData->Sn_TKAKTZ , MONY_BUF_SIZE);
		//25
		//ç∑à¯î[ïtê≈äz
		if( m_Arith.l_test(ANS[21].val) > 0 ){
			m_Arith.l_sub( ANS[24].val , ANS[21].val , m_pSnHonpyoData->Sn_TKAKTZ );
		}
		else{
			char	tmpMony[MONY_BUF_SIZE]={0};
			m_Arith.l_add( tmpMony, ANS[18].val, ANS[22].val );
			if( m_Arith.l_test(tmpMony) ){
				m_Arith.l_neg( tmpMony );
			}
			m_Arith.l_sub( ANS[24].val , tmpMony, m_pSnHonpyoData->Sn_TKAKTZ );
		}
		m_Arith.l_div( ANS[24].val , ANS[24].val , PW1 );
		m_Arith.l_mul( ANS[24].val , ANS[24].val , PW1 );

	}
	
//JANS
	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		memmove( JANS[0].val ,  PrintData->Sn_1F1A , MONY_BUF_SIZE);
		memmove( JANS[1].val ,  PrintData->Sn_1F1B , MONY_BUF_SIZE);
	}

	memmove( JANS[2].val ,  PrintData->Sn_1F1C , MONY_BUF_SIZE);

	if( m_pSnHeadData->SVmzsw == 1 ){//åoâﬂë[íuÇ™Ç†ÇÈèÍçá
		memmove( JANS[3].val ,  PrintData->Sn_1F2A , MONY_BUF_SIZE);
		memmove( JANS[4].val ,  PrintData->Sn_1F2B , MONY_BUF_SIZE);
	}
	memmove( JANS[5].val ,  PrintData->Sn_1F2C , MONY_BUF_SIZE);

	if( m_pSnHeadData->SVmzsw == 1 ){// åoâﬂë[íuëŒè€ÇÃâ€ê≈éëéYÇÃè˜ìnìôÇ†ÇË
		m_Arith.l_sub( kingaku , PrintData->Sn_1F9B , PrintData->Sn_1F8B );
		if( m_Arith.l_test( ANS[16].val ) > 0){//ä“ïtÇÃÇ∆Ç´
			if( !(m_pSnHeadData->Sn_SKKBN%2) ){
				memset( kingaku , '\0' , MONY_BUF_SIZE);
			}
		}
		if( !m_Arith.l_test( ANS[16].val ) && ( !m_Arith.l_test( ANS[17].val ) || ( m_Arith.l_cmp( ANS[17].val , val100 ) < 0 ))){
			memset( kingaku , '\0' , MONY_BUF_SIZE);
		}
		memmove( JANS[6].val ,  kingaku , MONY_BUF_SIZE);
		// ínï˚è¡îÔê≈ÇÃâ€ê≈ïWèÄÇ∆Ç»ÇÈè¡îÔê≈äz6.3%
		m_Arith.l_sub( kingaku , PrintData->Sn_1F9C , PrintData->Sn_1F8C );
		if( m_Arith.l_test( ANS[16].val ) > 0){//ä“ïtÇÃÇ∆Ç´
			if( !(m_pSnHeadData->Sn_SKKBN%2) ){
				memset( kingaku , '\0' , MONY_BUF_SIZE);
			}
		}
		if( !m_Arith.l_test( ANS[16].val ) && ( !m_Arith.l_test( ANS[17].val ) || ( m_Arith.l_cmp( ANS[17].val , val100 ) < 0 ))){
			memset( kingaku , '\0' , MONY_BUF_SIZE);
		}
		memmove( JANS[7].val ,  kingaku , MONY_BUF_SIZE);
	}
	else{
		if( m_Arith.l_test (ANS[16].val) ){
			memmove( JANS[7].val ,  ANS[16].val , MONY_BUF_SIZE);
			m_Arith.l_neg ( JANS[7].val );
		}
		else{
			memmove( JANS[7].val ,  ANS[17].val , MONY_BUF_SIZE);
		}
	}

	memset( WORK0, '\0', sizeof(WORK0) );
	i = j = 0;
	m_Arith.l_div( WORK0, ANS[j].val, PW0 );
	m_Arith.l_mul( ANS[j].val , WORK0 , PW1);


	// ã‡äz¢9£->¢12£
	for( ++i, j=0; j<4; ++j){
		m_Arith.l_div( WORK0, ANS[j+8].val, PW1 );
		m_Arith.l_mul( ANS[j+8].val , WORK0 , PW1);
	}
	
	// ã‡äz¢13£->¢16£
	for( ++i, j=0; j<4; ++j ){
		switch( j ){
			case 1 :
				m_Arith.l_div( WORK0, ANS[j+12].val, PW1 );
				m_Arith.l_mul( ANS[j+12].val , WORK0 , PW1);
				break;
			default:
				break;
		}
	}
	
	// ã‡äz¢17£->¢20£
	for( ++i, j=0; j<4; ++j ){
		switch( j ){
			case 0 :
			case 2 :
				break;
			default:	
				m_Arith.l_div( WORK0, ANS[j+16].val, PW1 );
				m_Arith.l_mul( ANS[j+16].val , WORK0 , PW1);
				break;
		}
	}
	
	// ã‡äz¢21£->¢25£
	for( ++i, j=0; j<5; ++j ){
		switch( j ){
			case 3 :
				break;
			default:
				m_Arith.l_div( WORK0, ANS[j+20].val, PW1 );
				m_Arith.l_mul( ANS[j+20].val , WORK0 , PW1);
				break;
		}
	}
	
	//26
	// è¡îÔê≈ãyÇ—ínï˚è¡îÔê≈ÇÃçáåvÅiî[ïtñîÇÕä“ïtÅjê≈äz
	switch( m_pSnHeadData->Sn_SKKBN&0xff ) {
		case 1 : case 2 :		// ämíËÅEíÜä‘ê\çê
			m_Arith.l_add( kingaku, ANS[10].val, ANS[21].val );
			m_Arith.l_add( kingaku2, ANS[7].val,  ANS[11].val );
			m_Arith.l_add( kingaku2 , kingaku2, ANS[18].val );
			m_Arith.l_add( kingaku2 , kingaku2, ANS[22].val );
			m_Arith.l_sub( ANS[25].val, kingaku, kingaku2 );
			break;
		default	:				// èCê≥ê\çê
			m_Arith.l_add( ANS[25].val, ANS[13].val, ANS[24].val );
			break;
	}
	
	//memmove( XM8, ANS[7].val, sizeof(XM8) );
	//memmove( XM11, ANS[10].val, sizeof(XM11) );
	//memmove( XM12, ANS[11].val, sizeof(XM12) );
	//memmove( XM19, ANS[18].val, sizeof(XM19) );
	//memmove( XM22, ANS[21].val, sizeof(XM22) );
	//memmove( XM23, ANS[22].val, sizeof(XM23) );
	//memmove( XM26, ANS[25].val, sizeof(XM26) );


	char	tmp_arith1[6] = {0},tmp_arith2[6] = {0};

	memmove( tmp_arith1, ANS[10].val, sizeof(ANS[10].val));
	m_Arith.l_sub( tmp_arith1, tmp_arith1, ANS[7].val );
	m_Arith.l_sub( tmp_arith1, tmp_arith1, ANS[11].val );
	m_Arith.l_sub( tmp_arith1, tmp_arith1, ANS[12].val );

	memmove( tmp_arith2, ANS[21].val, sizeof(ANS[21].val));
	m_Arith.l_sub( tmp_arith2, tmp_arith2, ANS[18].val );
	m_Arith.l_sub( tmp_arith2, tmp_arith2, ANS[22].val );
	m_Arith.l_sub( tmp_arith2, tmp_arith2, ANS[23].val );


	for( int cnt = 0; cnt < 6; cnt++ ){
		memset( F60val[cnt].val, '\0', MONY_BUF_SIZE);
	}

	if( m_Arith.l_test(ANS[25].val) > 0 ){
		memmove( F60val[0].val, ANS[25].val, MONY_BUF_SIZE);
		memmove( F60val[1].val, tmp_arith1, MONY_BUF_SIZE);
		memmove( F60val[2].val, tmp_arith2, MONY_BUF_SIZE);
	}else if( m_Arith.l_test(ANS[25].val) < 0 ){
		memmove( F60val[3].val, ANS[25].val, MONY_BUF_SIZE);
		memmove( F60val[4].val, tmp_arith1, MONY_BUF_SIZE);
		memmove( F60val[5].val, tmp_arith2, MONY_BUF_SIZE);

		if( m_Arith.l_test(F60val[3].val) < 0 ){
			m_Arith.l_neg(F60val[3].val);
		}

		if( m_Arith.l_test( ANS[10].val ) != 0 ){
			if( m_Arith.l_test(F60val[4].val) > 0 ){
				m_Arith.l_neg(F60val[4].val);
			}
		}else{
			if( m_Arith.l_test(F60val[4].val) < 0 ){
				m_Arith.l_neg(F60val[4].val);
			}
		}
		if( m_Arith.l_test( ANS[21].val ) != 0 ){
			if( m_Arith.l_test(F60val[5].val) > 0 ){
				m_Arith.l_neg(F60val[5].val);
			}
		}else{
			if( m_Arith.l_test(F60val[5].val) < 0 ){
				m_Arith.l_neg(F60val[5].val);
			}
		}
	}

	return 0;
}

int CH26HyoView::GetMxListFhyo60()
{
	int CULM_PER_PAGE = 4;
	int ret = 0;
	int max = 0;
	for( int cnt = 0; cnt < SOZOKU_MAX; cnt++ ){
		if( m_pSnFhyo60Data->Sn_NO[cnt]+1 > 0 ){ //Sn_NOÇÕÇOÉIÉäÉWÉìÇ»ÇÃÇ≈+1
			max = m_pSnFhyo60Data->Sn_NO[cnt]+1;
		}
	}
	if( max%CULM_PER_PAGE ){
		ret = (max/CULM_PER_PAGE)+1;
	}else{
		ret = (max/CULM_PER_PAGE);
	}

	if( ret == 0 ){ //ÉfÅ[É^ÇOÇ≈Ç‡ÇPÉyÅ[ÉWèoÇµÇ∆Ç≠
		ret = 1;
	}

	return ret;
}
//2018/03/13 INSERT END


//-----------------------------------------------------------------------------
// èoóÕópê≈óùémñºÇÃçÏê¨('15.02.27)
//-----------------------------------------------------------------------------
// à¯êî	pOutBuf	ÅF	èoóÕópÉoÉbÉtÉ@
//		len		ÅF	èoóÕópÉoÉbÉtÉ@í∑
//-----------------------------------------------------------------------------
void CH26HyoView::MakeOutZeirishiName( char *pOutBuf, int len )
{
	const	int	ZNAME_BUF_LEN = 40;
	int		tmplen;
	char	zbuf[128]={0};
	if( m_pPrintCmInfo->OPA == 2 ){
		if( (tmplen = (int)strlen(m_pShinInfo->pO_DATA->o_zeinam)) > ZNAME_BUF_LEN ){
			tmplen = ZNAME_BUF_LEN;
		}
		memmove( zbuf, m_pShinInfo->pO_DATA->o_zeinam, tmplen );
	}
	else {
		if( m_pPrintCmInfo->OPA ){
			// ê≈óùémñ@êlñº
			if( (tmplen = (int)strlen(m_pShinInfo->pO_DATA->o_ofcnam)) > ZNAME_BUF_LEN ){
				tmplen = ZNAME_BUF_LEN;
			}
			memmove( zbuf, m_pShinInfo->pO_DATA->o_ofcnam, tmplen );
		}
		else{
			// ê≈óùéméÅñº
			if( (tmplen = (int)strlen(m_pShinInfo->pO_DATA->o_zeinam )) > ZNAME_BUF_LEN ){
				tmplen = ZNAME_BUF_LEN;
			}
			memmove( zbuf, m_pShinInfo->pO_DATA->o_zeinam, tmplen );
		}
	}
	
	if( m_pPrintCmInfo->zr_kbn == 2 ){
		if( m_pSnHeadData->m_ZroutSgn&0x01 ){		// ê≈óùémãÊï™ÇèoóÕ
			CString	kbName;
			GetZeirishiKubunName( m_pPrintCmInfo->zr_kbn, kbName );
			CString	outZname;
			outZname.Format( _T("%s %s"), kbName, zbuf );
			if( m_pSnHeadData->m_ZroutSgn&0x02 ){	// íºê⁄éÛîCÇèoóÕ
				outZname += _T("Åiíºê⁄éÛîCÅj");
			}
			memset( pOutBuf, '\0', len );
			wsprintf( pOutBuf, _T("%s"), outZname.GetBuffer() );
		}
		else{
			wsprintf( pOutBuf, _T("%s"), zbuf );
		}
	}
	else{
		wsprintf( pOutBuf, _T("%s"), zbuf );
	}
}

//-----------------------------------------------------------------------------
// ñ@êlã∆éÌÇ≈ñ@êlî‘çÜñ¢èoóÕÅH
//-----------------------------------------------------------------------------
// ï‘ëóíl	true	ÅF	ñ@êlî‘çÜñ¢èoóÕ
//			false	ÅF	ñ@êlî‘çÜèoóÕ
//-----------------------------------------------------------------------------
bool CH26HyoView::IsNotPrintHojinNo()
{
	if( (m_pSnHeadData->IsSoleProprietor()==FALSE) && (m_pPrintCmInfo->OPU==0) && !(m_pPrintCmInfo->PRtype&0x80) ){
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// å≥çÜÇÃÅõàÛ ñ¢èoóÕÅH
//-----------------------------------------------------------------------------
// ï‘ëóíl	true	ÅF	ñ¢èoóÕ
//			false	ÅF	èoóÕ
//-----------------------------------------------------------------------------
bool CH26HyoView::IsNotPrintGengoCircle()
{
	if( !(m_pPrintCmInfo->PRtype&0x80) && (m_pPrintCmInfo->OP_GENGO==0) ){
		return true;
	}
	return false;
}



