#include "stdafx.h"

#include "sys\types.h"
#include "sys\stat.h"
#include "io.h"

//
#include "stdio.h"
#include "malloc.h"
#include "string.h"
#include "fcntl.h"
#include "dos.h"
#include "direct.h"

#include "input1.h"
#include "external.h"

//func
#include "mstrw.h"

//#include "perform_chk.h"


// CDinpView
//#include "Dinp.h"
//#include "DinpDoc.h"
//#include "DinpView.h"
//extern class CDinpView *p_DinpView;



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int BRTORK_ITEM;

static int br_entry( CDBipEZREC *brec, CDBINPDataRec *data, int sgn, int msg_sg );
static int bchk_dbr( CDBINPDataRec *, int, int );
static int bchk_cbr( CDBINPDataRec *, int, int );
static void bchk_msg( int rval, int dbcr_sg, int bmn, CString& code, int brn );
static int brmsrch( CDBipEZREC *brec, CDBINPDataRec *data, int dbcr_sg);
static void err_ret( char * );

static int ac_bchk( CDBINPDataRec *dta, int dbcr_sg );
static BOOL codeinp_chk( LPCTSTR code );

/******************************

		é}î‘èWåvä÷åW
		01/22 '91

 ******************************/
//extern short	uerr_cod;

/*===========================*
	Branch entry check
	brchk0 ( mode )
*=============================*/
void brchk0( CDBINPDataRec * dta, int access_mode )
{
int rval, dbcr_sg;

	for( dbcr_sg = 0 ; dbcr_sg < 2 ; ++dbcr_sg )
	{
		rval = 0;

		if( !dbcr_sg )
		{
			rval = bchk_dbr( dta, access_mode, 0 );
		}
		else
		{
			rval = bchk_cbr( dta, access_mode, 0 );
		}		

		/* é}î‘éÊè¡	*/
		if( rval && (rval != 99) )
		{
			del_dtaeda( dta, dbcr_sg );
		}
	}
}



//------------------------------------------------
//	èoî[í†óp é}î‘ Ç™ìoò^Ç≥ÇÍÇƒÇ¢ÇÈÇ©É`ÉFÉbÉN
//
//
//------------------------------------------------
int ac_brchk1( CDBINPDataRec *dta, int dbcr_sg )
{
	int		rval;
	
	rval = 0;
	rval = ac_bchk( dta, dbcr_sg );

	/* é}î‘éÊè¡	*/
	if( rval && (rval != 99) )
	{
		del_dtaeda( dta, dbcr_sg );
	}

	return rval;
}

//------------------------------------------------
//	èoî[í†óp é}î‘ É`ÉFÉbÉN
//
//
//------------------------------------------------
static
int ac_bchk( CDBINPDataRec *dta, int dbcr_sg )
{
	int		rval;
	int		st;
	rval = 0;

	CString code; 
	int brn, bmn;

	if( dbcr_sg == 0 ) {
		bmn = dta->m_dbmn;
		code = dta->m_dbt;
		brn = dta->m_dbr;
	}
	else {
		bmn = dta->m_cbmn;
		code = dta->m_cre;
		brn = dta->m_cbr;	
	}

	if( ! codeinp_chk( code ) && (brn != -1) )
	{
		if( !BRmst ) {
			rval = 1;
			goto	AC_BCHK;
		}

		/* ä˘Ç…ìoò^óLÇË	*/
		if( brmsrch( &BREC1, dta, dbcr_sg) == 0 ) {
			goto AC_BCHK;
		}
		
		/* í«â¡ìoò^	*/
//		if( pDBzm->ezrec->GetRecordCount() < Voln1->ed_max ) {
			if( (st = br_entry( &BREC1, dta, (dbcr_sg|0x80), 1 )) == 0 ) {
				// É[Éçìoò^
				pDBzm->IsModify() = TRUE;
				goto AC_BCHK;
			}
			else if( st == 99 )
			{
				rval = 99;			// ìoò^ëIë
				goto AC_BCHK;
			}
			else {
				rval = 2; goto AC_BCHK;
			}
//		}
//		else rval = 3;
		
	}
AC_BCHK:

	if( rval ) bchk_msg( rval, -1, bmn, code, brn );
	return( rval );

}


//-------------------------------------------
//	é}î‘ Ç™ìoò^Ç≥ÇÍÇƒÇ¢ÇÈÇ©É`ÉFÉbÉN
//
//	return 99 ìoò^èàóù
//			0 É[Éçìoò^ÇµÇΩ
//			1 é}î‘Ç»Çµ
//		Å@ -1 ÉGÉâÅ[
//-------------------------------------------
int brchk1( CDBINPDataRec *dta, int dbcr_sg )
{
	int		rval;

	rval = 0;

	if( !dbcr_sg )
		rval = bchk_dbr( dta, _ENTRY, 1 );
	else
		rval = bchk_cbr( dta, _ENTRY, 1 );

	/* é}î‘éÊè¡	*/
	if( rval && (rval != 99 && rval != 90) )
	{
		del_dtaeda( dta, dbcr_sg );
	}

	return( rval );
}

//
// édñÛé}î‘ÇÃéÊÇËè¡Çµ
void del_dtaeda( CDBINPDataRec *dta, int dbcr_sg )
{
	if( !dbcr_sg )	{
		if( dta->m_dbr != -1 ) dta->m_dbr = -1;
	}
	else	{
		if( dta->m_cbr != -1 ) dta->m_cbr = -1;
	}
}


//=================================================
//	Entry branch master 
//	CDBipEZREC *brec ------- é}î‘ÉoÉbÉtÉ@
//	CDBINPDataRec *data ----- édñÛÉåÉRÅ[Éh
//	int dbcr_sg ----- D7 ON Ç≈ éÿÅ^ë›ï∂éöóÒÇï\é¶ÇµÇ»Ç¢
//					  D0-D3 0 = éÿï˚, 1 = ë›ï˚
//	int msg_sg	
//
//	return 99 ìoò^èàóù
//			0 É[Éçìoò^ÇµÇΩ
//			1 é}î‘Ç»Çµ
//		Å@ -1 ÉGÉâÅ[
//=================================================
static int br_entry( CDBipEZREC *brec, CDBINPDataRec *data, int dbcr_sg, int msg_sg )
{
	int	st,n;
	char	*msgp;
	char	pbuf[300], edabuf[20], bmnbuf[24];;
	CString ezkm_code, kmkcode;

	BRTORK_ITEM = dbcr_sg == 0 ? 0 : 1;

//	memset( (char *)brec, '\0', sizeof( CDBipEZREC ) );
	brec->Reset();

	int dc_sgn;
	dc_sgn = (dbcr_sg&0x0f);

	if( dc_sgn == 0 ) {
		if( ! (dbcr_sg & 0x80) )	msgp = "éÿï˚"; 
		else						msgp = "";
		brec->m_ezbmn = data->m_dbmn;
		brec->m_ezkcd = data->m_dbt;
		brec->m_ezecd = data->m_dbr;
	}
	else if( dc_sgn == 1 )	{
		if( ! (dbcr_sg & 0x80) )	msgp = "ë›ï˚"; 
		else						msgp = "";
		brec->m_ezbmn = data->m_cbmn;
		brec->m_ezkcd = data->m_cre;
		brec->m_ezecd = data->m_cbr;
	}

	if( msg_sg )
	{
		struct _DBKNREC	*pKn;
		pKn = pDBzm->DB_PjisToKnrec( brec->m_ezkcd );

//		sprintf( pbuf, "%sâ»ñ⁄Åu%.14sÅvÇÃé}î‘%04xÇÕìoò^Ç≥ÇÍÇƒÇ¢Ç‹ÇπÇÒÅB\n\nìoò^ÇµÇ‹Ç∑Ç©ÅH\n\nÅ@Å@Å@Å@Å@ÅuÇ¢Ç¢Ç¶ÅvÇÃèÍçáé}î‘ÇÕÉLÉÉÉìÉZÉãÇ≥ÇÍÇ‹Ç∑ÅB",
		pDBzm->EdabanToStr( edabuf, sizeof edabuf, brec->m_ezecd );

#ifdef BMEDA_CLOSE
		if( brec->m_ezbmn != -1 ) {
			pDBzm->BumonCodeToStr( bmnbuf, sizeof bmnbuf, brec->m_ezbmn );
			n = sprintf_s( pbuf, sizeof pbuf, "ÅsïîñÂé}î‘ñ¢ìoò^Åt\n\n%sïîñÂ[%s] â»ñ⁄Åu%.14sÅvÇÃé}î‘%sÇÕìoò^Ç≥ÇÍÇƒÇ¢Ç‹ÇπÇÒÅBÅ@ìoò^ÇµÇ‹Ç∑Ç©ÅH",
							msgp, bmnbuf, pKn->knnam, edabuf );
			sprintf_s( &pbuf[n], (sizeof pbuf - n), "\n\nÅuÇ¢Ç¢Ç¶ÅvÇÃèÍçáÅA%sïîñÂ[%s] â»ñ⁄Åu%.14sÅvÇÃé}î‘%sÇÕÉLÉÉÉìÉZÉãÇ≥ÇÍÇ‹Ç∑ÅB",
							msgp, bmnbuf, pKn->knnam, edabuf );
		}
		else {
#endif
			n = sprintf_s( pbuf, sizeof pbuf, "Åsé}î‘ñ¢ìoò^Åt\n\n%sâ»ñ⁄Åu%.14sÅvÇÃé}î‘%sÇÕìoò^Ç≥ÇÍÇƒÇ¢Ç‹ÇπÇÒÅBÅ@ìoò^ÇµÇ‹Ç∑Ç©ÅH",
									msgp, pKn->knnam, edabuf );
			sprintf_s( &pbuf[n], (sizeof pbuf - n), "\n\nÅuÇ¢Ç¢Ç¶ÅvÇÃèÍçáÅA%sâ»ñ⁄Åu%.14sÅvÇÃé}î‘%sÇÕÉLÉÉÉìÉZÉãÇ≥ÇÍÇ‹Ç∑ÅB",
				msgp, pKn->knnam, edabuf );
//		}


		CString title;
		title.LoadString(AFX_IDS_APP_TITLE);
		st = myICSExtBox( title, pbuf, _T( "ÇÕÇ¢(&Y)" ), _T( "É[Éçìoò^(&Z)" ), _T( "Ç¢Ç¢Ç¶(&N)" ), MB_DEFBUTTON1 );	

		if( st == /*IDOK*/ IDYES )
		{

			DWORD tmp;
			tmp = pKn->kncod;
			hl_rev( &tmp, 4 );
			kmkcode.Format( "%08x", tmp );
			kmkcode.TrimRight();
			if( kmkcode != brec->m_ezkcd ) {
				sprintf_s( pbuf, sizeof pbuf, "%sÅu%.14sÅvÇÃâ»ñ⁄ÉRÅ[ÉhÇÕìoò^Ç≥ÇÍÇƒÇ¢Ç‹ÇπÇÒÅB", msgp, pKn->knnam );
				err_ret( pbuf );
				return 1;
			}

			return 99;		// ìoò^ëIë

		}
		else if( st == IDNO ) {
			// É[Éçìoò^ ÇÕ â∫ÇÃ zroeda_torok(ä÷êîÇ≈)
		
		}
		else
		{
			return 1;
		}

	}
	return zroeda_torok( data, dbcr_sg );	// íPÇ»ÇÈìoò^
}

//---------------------------------------------------------------
// íPÇ»ÇÈìoò^
//	int dbcr_sg ----- D7 ON Ç≈ éÿÅ^ë›ï∂éöóÒÇï\é¶ÇµÇ»Ç¢
//					  D0-D3 0 = éÿï˚, 1 = ë›ï˚
//  -> 0 ... ok  -1 ... é∏îs
//---------------------------------------------------------------
int zroeda_torok(CDBINPDataRec *data, int dbcr_sg)
{
	int	att, bmn;

	char	*msgp;
	char	pbuf[300];
	char	eda_buf[20];
	CDBipEZREC *brec;

	int dc_sgn;
	dc_sgn = (dbcr_sg&0x0f);

	if( dc_sgn == 0 )
	{
		if( ! (dbcr_sg & 0x80) )	msgp = "éÿï˚"; 
		else						msgp = "";
		brec = &BREC1;

//		memset( (char *)brec, '\0', sizeof( CDBipEZREC ) );
		brec->Reset();
		
		brec->m_ezkcd = data->m_dbt;
		brec->m_ezecd = data->m_dbr;
		bmn = data->m_dbmn;
	}
	else
	{
		if( ! (dbcr_sg & 0x80) )	msgp = "ë›ï˚"; 
		else						msgp = "";
		brec = &BREC2;

//		memset( (char *)brec, '\0', sizeof( CDBipEZREC ) );
		brec->Reset();
		
		brec->m_ezkcd = data->m_cre;
		brec->m_ezecd = data->m_cbr;
		bmn = data->m_cbmn;
	}

	// â»ñ⁄ñºèÃÉäÅ[Éh
	struct _DBKNREC* pKn;
	pKn = pDBzm->DB_PjisToKnrec( brec->m_ezkcd );
	pDBzm->EdabanToStr( eda_buf, sizeof eda_buf, brec->m_ezecd );

	att = pKn->knatt;
	brec->m_ezatt |= (att & 0x01); brec->m_ezatt &= 0x7f;
	memcpy(brec->m_ezsysgn, pKn->knsgn, sizeof brec->m_ezsysgn);

	// DB Ç…é}î‘écÇìoò^
#ifdef BMEDA_CLOSE
	if( bmn == -1 ) {
#endif
		if( pDBzm->EdabanZanTorok( brec ) ) {
			sprintf_s( pbuf, sizeof pbuf, "%sÅu%.14s.%sÅvÅiÉRÅ[Éh=%s.%sÅjÇÃìoò^Ç…é∏îsÇµÇ‹ÇµÇΩÅB",
									msgp, pKn->knnam, eda_buf, (LPCTSTR)brec->m_ezkcd, eda_buf ); 
			err_ret( pbuf );
			del_dtaeda( data, dbcr_sg );	//édñÛé}î‘éÊÇËè¡Çµ
			return(-1);
		}
		else {
			if( bmn != -1 && (Voln1->bm_isw & 0x02) ) {
				//ïîñÂé}î‘é©ìÆìoò^ON
				if( pDBzm->BmnEdabanZanTorok( brec, bmn ) ) {
					sprintf_s( pbuf, sizeof pbuf, "%s ïîñÂ[%d]Åu%.14s.%sÅvÅiÉRÅ[Éh=%s.%sÅjÇÃìoò^Ç…é∏îsÇµÇ‹ÇµÇΩÅB",
											msgp, bmn, pKn->knnam, eda_buf,(LPCTSTR)brec->m_ezkcd, eda_buf ); 
					err_ret( pbuf );
					del_dtaeda( data, dbcr_sg );	//édñÛé}î‘éÊÇËè¡Çµ
					return(-1);
				}	
			}
		}

#ifdef BMEDA_CLOSE
	}
	else {
		if( pDBzm->BmnEdabanZanTorok( brec, bmn ) ) {
			sprintf_s( pbuf, sizeof pbuf, "%s ïîñÂ[%d]Åu%.14s.%sÅvÅiÉRÅ[Éh=%s.%sÅjÇÃìoò^Ç…é∏îsÇµÇ‹ÇµÇΩÅB",
									msgp, bmn, pKn->knnam, eda_buf, brec->m_ezkcd, eda_buf ); 
			err_ret( pbuf );
			del_dtaeda( data, dbcr_sg );	//édñÛé}î‘éÊÇËè¡Çµ
			return(-1);
		}	
	}
#endif

	return(0);

}

//
// éÿï˚é}î‘É`ÉFÉbÉNÅEìoò^
//
static int bchk_dbr( CDBINPDataRec *dta, int access_mode, int msg_sg )
{
	int		rval;
	int		ST;

	rval = 0;
	
	if( ! codeinp_chk( dta->m_dbt ) && (dta->m_dbr != -1) )
	{
		// é}î‘É}ÉXÉ^Å[Ç≈ÇÕÇ»Ç¢
		if( !BRmst ) {
			if( access_mode == _ENTRY ) rval = 1;	// rval == 1 : é}î‘É}ÉXÉ^Å[Ç≈ÇÕÇ»Ç¢
			goto	BCHK01;
		}

		/* ä˘Ç…ìoò^óLÇË	*/
		if( brmsrch( &BREC1, dta, 0) == 0) {
			goto	BCHK01;
		}

		if( access_mode == _SEARCH ) goto	BCHK01;

//		if( pDBzm->ezrec->GetRecordCount() < Voln1->ed_max ) {

			/*Å@í«â¡ìoò^	*/
			if(( ST = br_entry( &BREC1, dta, 0, msg_sg )) == 0 ) {
				// É[Éçìoò^
				pDBzm->IsModify() = TRUE;
				rval = 90;
				goto BCHK01;
			}
			else if( ST == 99 )
			{
				rval = 99;			// ìoò^É^ÉCÉvëIëÇçsÇ§
				goto BCHK01;
			}
			else { /* no entry */
				rval = 2;			// ÉGÉâÅ[
				goto BCHK01;
			}
//		}
//		else	rval = 3;	// é}î‘ìoò^ç≈ëÂêîÇí¥Ç¶ÇƒÇ¢ÇÈ
	}

BCHK01:

	if( rval ) bchk_msg( rval, 0, dta->m_dbmn, dta->m_dbt, dta->m_dbr );
	return( rval );
}


//
// ë›ï˚é}î‘É`ÉFÉbÉNÅEìoò^
//
static int bchk_cbr( CDBINPDataRec *dta, int access_mode, int msg_sg )
{
	int		rval;
	int		ST;

	rval = 0;

	if( ! codeinp_chk( dta->m_cre ) && (dta->m_cbr != -1) )
	{
		if( !BRmst ) {
			if( access_mode == _ENTRY ) rval = 1;
			goto	BCHK02;
		}

		/* ä˘Ç…ìoò^óLÇË	*/
		if( brmsrch( &BREC2, dta, 1) == 0 ) {
			goto BCHK02;
		}
		
		if( access_mode == _SEARCH ) goto	BCHK02;

		/* í«â¡ìoò^	*/
//		if( pDBzm->ezrec->GetRecordCount() < Voln1->ed_max ) {
			if( (ST=br_entry( &BREC2, dta, 1, msg_sg )) == 0 ) {
				// É[Éçìoò^
				rval = 90;
				pDBzm->IsModify() = TRUE;
				goto BCHK02;
			}
			else if( ST == 99 )
			{
				rval = 99;			// ìoò^ëIë
				goto BCHK02;
			}
			else {
				rval = 2; goto BCHK02;
			}
//		}
//		else rval = 3;
		
	}
BCHK02:

	if( rval ) bchk_msg( rval, 1, dta->m_cbmn, dta->m_cre, dta->m_cbr);
	return( rval );
}

//
//	dbcr_sg = -1 ÇÃÇ∆Ç´ÇÕ éÿÅ^ë› ñºèÃÇï\é¶ÇµÇ»Ç¢
//
//
static void bchk_msg( int rval, int dbcr_sg, int bmn, CString& code, int brn )
{
	char	msg[MAX_PATH], buf[20], bmnbuf[20];
	char*	str;
	int n;

	switch( dbcr_sg ) {
	case 0:	str = "éÿï˚";	break;
	case 1:	str = "ë›ï˚";	break;
	default:str = "";		break;
	}

	int ed_max = 0;
	ed_max = pDBzm->m_edmax;

	if( rval == 1 || rval == 3 )
	{
		if( rval == 1 )
			n = sprintf_s( msg, sizeof msg, "%s", "é}î‘É}ÉXÉ^Å[Ç≈ÇÕÇ»Ç¢ÇΩÇﬂé}î‘ÇÕégópÇ≈Ç´Ç‹ÇπÇÒÅB" );
		else if( rval == 3 )
			n = sprintf_s( msg, sizeof msg, "é}î‘ç≈ëÂìoò^êî(=%u)Çí¥Ç¶Çƒìoò^ÇµÇÊÇ§Ç∆ÇµÇ‹ÇµÇΩÅB", ed_max );

		struct _DBKNREC* pKn;
		pKn = pDBzm->DB_PjisToKnrec( code );

		pDBzm->EdabanToStr( buf, sizeof buf, brn );

#ifdef BMEDA_CLOSE	// ïîñÂé}î‘ÇÃç◊Ç©Ç¢É`ÉFÉbÉNÇÕÇµÇ»Ç¢
		if( bmn != -1 ) {
			pDBzm->BumonCodeToStr( bmnbuf, sizeof bmnbuf, bmn );
			sprintf_s( &msg[n], (sizeof msg - n), "\n\n%sïîñÂ[%s] â»ñ⁄Åu%.14sÅvÇÃé}î‘%sÇÕÉLÉÉÉìÉZÉãÇ≥ÇÍÇ‹Ç∑ÅB",
						str, bmnbuf, pKn->knnam, buf );
		}
		else {
#endif
			sprintf_s( &msg[n], (sizeof msg - n), "\n\n%sâ»ñ⁄Åu%.14sÅvÇÃé}î‘%sÇÕÉLÉÉÉìÉZÉãÇ≥ÇÍÇ‹Ç∑ÅB",
						str, pKn->knnam, buf );
//		}

		myICSMessageBox( msg );
	}
}


/*==============================================*
	Branch Master search
	brmsrch (fn, brec, icb, &data, sgn)
	return -1	= not found
			0	= found
*===============================================*/
static int brmsrch ( CDBipEZREC *brec, CDBINPDataRec *data, int dbcr_sg)
{
	//éÿï˚
	if ( dbcr_sg == 0) {
		if( pDBzm->DB_EdabanMasterSearch( data, 0 ) != 0 )
			return -1;

#ifdef BMEDA_CLOSE
		if( data->m_dbmn != -1 ) {
			if( pDBzm->DB_BmnEdabanMasterSearch( data, 0 ) != 0 )
				return -1;
		}
		else {
			if( pDBzm->DB_EdabanMasterSearch( data, 0 ) != 0 )
				return -1;
		}
#endif
	}
	//ë›ï˚
	else {
		if( pDBzm->DB_EdabanMasterSearch( data, 1 ) != 0 )
			return -1;

#ifdef BMEDA_CLOSE
		if( data->m_cbmn != -1 ) {
			if( pDBzm->DB_BmnEdabanMasterSearch( data, 1 ) != 0 )
				return -1;
		}
		else {
			if( pDBzm->DB_EdabanMasterSearch( data, 1 ) != 0 )
				return -1;
		}
#endif
	}

	return 0;	// Found

}

static void err_ret( char *msg )
{
	ICSMessageBox( msg );
}

/******************************************

	é}î‘ì¸óÕÉ`ÉFÉbÉN
	dbcr_sg:	-1 ÇÃÇ∆Ç´ÇÕÅAèoî[í†ÇÊÇË

		->	0 ... OK
			1 ... èCê≥
			-1 .. Error

*******************************************/
int inp_brchk( CString& code, int brn, int mode, int *sg, int dbcr_sg )
{
	CString	msg;
	int st;
	CDBipEZREC brec;
	brec.Reset();

	if( BRmst && (mode != -1) && ! codeinp_chk( code ) && (code != "") )
	{
		// ñ¢ì¸óÕÉ`ÉFÉbÉN
		if( (mode == OPT_RGSCHK) && (brn == -1 ) )
		{
			brec.m_ezkcd = code;

			CDBINPDataRec chk;
			chk.m_dbt = brec.m_ezkcd;
			chk.m_dbr = brn;
/*			msg.Format( "ezkcd = '%s'", brec.m_ezkcd );
			if( pDBzm->ezrec->Requery( msg, 1 ) == 0 )	*/ 
			ezdb_init( pDBzm );
			if( pDBzm->ezrec->Find( (LPCTSTR)code ) > 0 )
			{
				// ñ¢ì¸óÕÉÅÉbÉZÅ[ÉW
				struct _DBKNREC* pKn;
				if( ( pKn = pDBzm->DB_PjisToKnrec( code ) ) == NULL )
					return -1;

				char* p;
				switch( dbcr_sg ) {
				case 0:		p = "éÿï˚";	break;
				case 1:		p = "ë›ï˚";	break;
				default:	p = "";	break;
				}

				msg.Format( "Åsé}î‘ñ¢ì¸óÕÅt\n\n%sâ»ñ⁄Åu%.14sÅvÇ…é}î‘Ç™Ç†ÇËÇ‹ÇπÇÒÅBèCê≥ÇµÇ‹Ç∑Ç©ÅH",
								p, pKn->knnam );

				st = myICSMessageBox( (LPCSTR)msg, MB_YESNO );

				if( dbcr_sg != -1 )
						*sg |= (!dbcr_sg ? 0x11 : 0x12);	// éÿÇ‹ÇΩÇÕë›É`ÉFÉbÉNçœÇ› 
				else	*sg |= 0x10;

				if( st == IDYES )
					return 1;	// èCê≥
				else
					return 0;	// OK

			}
		}
	}
	return 0;
}

//
//é}î‘ì¸óÕâ¬î\ÇÃâ»ñ⁄Ç©ÅH
//
static BOOL codeinp_chk( LPCTSTR code )
{
	int st = 0;

/*- '08.11.28 -*/
//	if( Voln1->apno == 0x50 ) {
//		//èîå˚éëã‡ÇÕèúÇ≠
//		const char* syogskn = "00010101";
//		int len;
//		len = strlen( syogskn );
//
//		if( ! strncmp( code, syogskn, len ) )
//			st++;
//	}
/*-------------*/

	if( isSYOGT_CODE( code ) )
		st++;

	return st ? TRUE : FALSE;
}