#include "stdafx.h"

#include "DBInpSubFunc.h"
#include "BmnChk.h"

#include "KJVOL.h"


// ïîñÂÉ`ÉFÉbÉNÉNÉâÉX

// ïîñÂÉfÅ[É^äiî[
typedef struct _BMN_CHK
{
	long	bnbcd;
//	CString	bnbnm;
//	CString	bnkidx;
//	CByteArray	bnsgn;
	BYTE	bnhsg[5];
	long	bnhw[5];
	long	prbcd;
	long	klevel;
	long	pseq;
	long	dseq;
	BYTE	grpsgn;
	BYTE	sgn1;
	BYTE	sgn2;

	BYTE	dmy[8];

} BMN_CHK;




class CBmnChkSub
{
public:
	CBmnChkSub();
	virtual ~CBmnChkSub();

	int	 SetDBZmSub( CDBZmSub* pZmSub );
	int	 BmnCheckInit();
	int	 BmnCheckUpdate();
	BOOL BmnHaniCheck( int bmn, DWORD kmkcode );
	BOOL BmnHaniCheck( int bmn, CString strCode );

protected:
	void BmnAddJob();

public:
	CArray<BMN_CHK, BMN_CHK&>	BmnArray;

	CDBZmSub* pZm;

	BYTE	apno;
	BYTE	bm_isw;
};


CBmnChkSub::CBmnChkSub()
{
	pZm = NULL;
	apno = 0;
	bm_isw = 0;
}

CBmnChkSub::~CBmnChkSub()
{

}

int CBmnChkSub::SetDBZmSub( CDBZmSub* pZmSub )
{
	pZm = pZmSub;
	return 0;
}

int	CBmnChkSub::BmnCheckInit()
{
	BmnArray.RemoveAll();

	if( pZm->zvol == NULL )
		return -1;

	if( ! (pZm->zvol->sub_sw&0x02) )
		return 0;

	apno	= pZm->zvol->apno;
	bm_isw	= pZm->zvol->bm_isw;

	if( pZm->bmname == NULL )
		return 0;

	BmnAddJob();

	return 0;
}


void CBmnChkSub::BmnAddJob()
{
	pZm->bmname->Requery( "", "" );

	BMN_CHK bc;
	::ZeroMemory( bc.dmy, sizeof bc.dmy );
	
	if( pZm->bmname->MoveFirst() == 0 ) {

		do {
			bc.bnbcd	=	pZm->bmname->bnbcd;
			//bc.bnbnm	=	pZm->bmname->;
			//bc.bnkidx	=	pZm->bmname->;
			//bc.bnsgn;
			memcpy( bc.bnhsg, pZm->bmname->bnhsg, sizeof pZm->bmname->bnhsg );
			memcpy( bc.bnhw, pZm->bmname->bnhw,  sizeof pZm->bmname->bnhw );

			bc.prbcd	=	pZm->bmname->prbcd;
			bc.klevel	=	pZm->bmname->klevel;
			bc.pseq		=	pZm->bmname->pseq;
			bc.dseq		=	pZm->bmname->dseq;
			bc.grpsgn	=	pZm->bmname->grpsgn;
			bc.sgn1		=	pZm->bmname->sgn1;
			bc.sgn2		=	pZm->bmname->sgn2;

			BmnArray.Add( bc );

		} while( pZm->bmname->MoveNext() == 0 );
	
	}
}


int	CBmnChkSub::BmnCheckUpdate()
{
	BmnArray.RemoveAll();

	if( pZm->zvol == NULL )
		return 0;

	if( ! (pZm->zvol->sub_sw&0x02) )
		return 0;

	apno	= pZm->zvol->apno;
	bm_isw	= pZm->zvol->bm_isw;

	if( pZm->bmname == NULL )
		return 0;

	BmnAddJob();

	return 0;
}


BOOL CBmnChkSub::BmnHaniCheck( int bmn, DWORD kmkcode )
{
	INT_PTR	cnt;
	BOOL bCheck = FALSE;

	cnt = BmnArray.GetCount();

	DWORD	dwTmp;
	dwTmp = kmkcode;
	hl_rev( &dwTmp, 4 );
	int bmn_sgn = 0;

	for( int i = 0; i < cnt; i++ ) {
		//çáåvïîñÂÇÕÉ`ÉFÉbÉNçœÇ›
		if( (BmnArray[i].bnhsg[i] & 0x02) )	continue;

		if( BmnArray[i].bnbcd == bmn ) {
			if( kmkcode ) {
				if( bm_isw & 0x10 ) {// ëπâvÇÃÇ›

					/* 0x00=äÓñ{Ç«Ç®ÇËÅ@0x01=ë›éÿÅ@0x02=àÍî ä«óùîÔÅ@0x04=êªë¢å¥âø, 0x08=îÑè„, 0x10=îÑè„å¥âø */
					if( BmnArray[i].grpsgn == 0x00 ) {
						if( dwTmp >= 0x08000000 )
							bCheck = TRUE;
					}
					else {
		
						if( BmnArray[i].grpsgn & 0x01 ) {
							//ëπâvÇÃÇ›ÇÃÇΩÇﬂ ë›éÿÇÕÇ»Çµ
						//	if( dwTmp < 0x08000000 )
						//		bCheck = TRUE;
						}
						if( BmnArray[i].grpsgn & 0x02 ) {
							if( apno <= 0x30 ) {
								if( dwTmp >= 0x0a000000 && dwTmp < 0x0b000000 ){
									 bCheck = TRUE;
								}
							}
							else {
								// åˆâvà»ç~
								if( dwTmp >= 0x08000000 )
									bCheck = TRUE;
							}
						}

						if( BmnArray[i].grpsgn & 0x04 ) {
							if( apno < 0x30 ) {
								if( dwTmp >= 0x0f000000 )
									 bCheck = TRUE;
							}
							else {
								// åˆâvà»ç~
								if( dwTmp >= 0x08000000 )
									bCheck = TRUE;
							}
						}
						if( BmnArray[i].grpsgn & 0x08 ) {	//îÑè„
							if( apno <= 0x30 ) {
								if( dwTmp >= 0x08000000 && dwTmp < 0x09000000 )
									 bCheck = TRUE;
							}
							else {
								if( dwTmp >= 0x08000000 )
									bCheck = TRUE;
							}
						}
						if( BmnArray[i].grpsgn & 0x10 ) {	//îÑè„å¥âø
							if( apno <= 0x30 ) {
								if( (dwTmp >= 0x09000000 && dwTmp < 0x0a000000) )
									 bCheck = TRUE;
							}
							else {
								if( dwTmp >= 0x08000000 )
									bCheck = TRUE;
							}
						}
					}
				}
				else {
					//ëSâ»ñ⁄
					/* 0x00=äÓñ{Ç«Ç®ÇËÅ@0x01=ë›éÿÅ@0x02=àÍî ä«óùîÔÅ@0x04=êªë¢å¥âø, 0x08=îÑè„, 0x10=îÑè„å¥âø */
					if( BmnArray[i].grpsgn == 0x00 ) {
						bCheck = TRUE;
					}
					else {
		
						if( BmnArray[i].grpsgn & 0x01 ) {
							if( dwTmp < 0x08000000 )
								bCheck = TRUE;
						}
						if( BmnArray[i].grpsgn & 0x02 ) {
							if( apno <= 0x30 ) {
								if( dwTmp >= 0x0a000000 && dwTmp < 0x0b000000 ){
									 bCheck = TRUE;
								}
							}
							else {
								// åˆâvà»ç~
								if( dwTmp >= 0x08000000 )
									bCheck = TRUE;
							}
						}

						if( BmnArray[i].grpsgn & 0x04 ) {
							if( apno < 0x30 ) {
								if( dwTmp >= 0x0f000000 ) 
									 bCheck = TRUE;
							}
							else {
								// åˆâvà»ç~
								if( dwTmp >= 0x08000000 )
									bCheck = TRUE;
							}
						}

						if( BmnArray[i].grpsgn & 0x08 ) {	//îÑè„
							if( apno <= 0x30 ) {
								if( dwTmp >= 0x08000000 && dwTmp < 0x09000000 )
									 bCheck = TRUE;
							}
							else {
								if( dwTmp >= 0x08000000 )
									bCheck = TRUE;
							}
						}
						if( BmnArray[i].grpsgn & 0x10 ) {	//îÑè„å¥âø
							if( apno <= 0x30 ) {
								if( (dwTmp >= 0x09000000 && dwTmp < 0x0a000000) )
									 bCheck = TRUE;
							}
							else {
								if( dwTmp >= 0x08000000 )
									bCheck = TRUE;
							}
						}
					}
				}
			}
			bmn_sgn = 1;
			break;
		} // if( BmnArray[i].bnbcd == bmn )
	}

	// äYìñïîñÂÇ™Ç»Ç¢Ç∆Ç´Ç≈Ç‡ÅAêVãKìoò^ÇÕãñâ¬Ç∑ÇÈ
	if( ! bCheck ) {
		if( dwTmp == 0 ) {	//â»ñ⁄Ç»ÇµÇÃÇ∆Ç´ÇÕ ÇnÇj
			bCheck = TRUE;
		}
		else {
			if( ! bmn_sgn ) {
				if( bm_isw & 0x10 ) {// ëπâvÇÃÇ›
					if( dwTmp >= 0x08000000 )
						bCheck = TRUE;
				}
				else {
					bCheck = TRUE;
				}
			}
		}
	}

	return bCheck;
}


BOOL CBmnChkSub::BmnHaniCheck( int bmn, CString strCode )
{
	DWORD dwCode;

	dwCode = _atoh( (LPSTR)(LPCTSTR)strCode );
	hl_rev( &dwCode, 4 );

	return BmnHaniCheck( bmn, dwCode );
}


/////////////////////////////////////////////////////////////////////////////////////
CBmnHaniCheck::CBmnHaniCheck()
{
	pSub = NULL;
}

CBmnHaniCheck::~CBmnHaniCheck()
{
	if( pSub != NULL )
		delete pSub;
}


int CBmnHaniCheck::SetDBZmSub( CDBZmSub* pZmSub )
{
	if (pSub != NULL) {
		delete pSub;
		pSub = NULL;
	}

	pSub = new CBmnChkSub;

	if( pSub == NULL )	return -1;

	return pSub->SetDBZmSub( pZmSub );
}

int	CBmnHaniCheck::BmnCheckInit()
{
	if( pSub == NULL )	return -1;

	return pSub->BmnCheckInit();
}


int	CBmnHaniCheck::BmnCheckUpdate()
{
	if( pSub == NULL )	return -1;

	return pSub->BmnCheckUpdate();
}


BOOL CBmnHaniCheck::BmnHaniCheck( int bmn, DWORD kmkcode )
{
	if( pSub == NULL )	return FALSE;

	return pSub->BmnHaniCheck( bmn, kmkcode );
}


BOOL CBmnHaniCheck::BmnHaniCheck( int bmn, CString strCode )
{
	if( pSub == NULL )	return FALSE;

	return pSub->BmnHaniCheck( bmn, strCode );
}


///////////////////////////////////////////////////////////////////////////////////////////////


// çHéñê›íËâ»ñ⁄ ïîñÂÉfÅ[É^äiî[
typedef struct _KOJI_CHK
{
	DWORD	chkCode;
	DWORD	chkMask;

} KOJI_CHK;



class CKojiChkSub
{
public:
	CKojiChkSub();
	virtual ~CKojiChkSub();

	int		SetDBZmSub( CDBZmSub* pZmSub );
	int		KojiCheckInit();
	int		KojiCheckUpdate();

	BOOL	KojiKmkCheck( DWORD kmkcode );
	BOOL	KojiKmkCheck( CString strCode );

protected:
	void	make_kjarray();

private:
	CArray<KOJI_CHK, KOJI_CHK&>	kjArray;
//	CArray<KOJI_CHK, KOJI_CHK&>	kjBase;

	CDBZmSub* pZm;

	char	rsv[64];
};



CKojiChkSub::CKojiChkSub()
{
}


CKojiChkSub::~CKojiChkSub()
{
}


int	CKojiChkSub::SetDBZmSub( CDBZmSub* pZmSub )
{
	pZm = pZmSub;

	return 0;
}


int	CKojiChkSub::KojiCheckInit()
{
	kjArray.RemoveAll();
	if( pZm->zvol->apno != 0x20 )
		return 0;

	make_kjarray();

	return 0;
}

int	CKojiChkSub::KojiCheckUpdate()
{
	kjArray.RemoveAll();
	if( pZm->zvol->apno != 0x20 )
		return 0;

	make_kjarray();

	return 0;
}


BOOL CKojiChkSub::KojiKmkCheck( DWORD kmkcode )
{
	BOOL ret = FALSE;
	int i;

	for( i = 0; i < kjArray.GetCount(); i++ ) {
		if( kjArray[i].chkCode == (kmkcode & kjArray[i].chkMask) ) {
			ret = TRUE;
			break;
		}
	}

	return ret;
}

BOOL CKojiChkSub::KojiKmkCheck( CString strCode )
{
	DWORD dwCode = _atoh( (LPSTR)(LPCTSTR)strCode );

	return KojiKmkCheck( dwCode );
}


void CKojiChkSub::make_kjarray()
{

KOJI_CHK	kc1[] = {
	{0x01040300, 0xffffff00},
	{0x01060200, 0xffffff00},
	{0x04050200, 0xffffff00},
	{0x08010100, 0xffffff00},
	{0x08020100, 0xffffff00},
	{0x0F000000, 0xff000000},
};

DBGKREC* pGk, *pBaseGk;
KOJI_CHK	kc;
int i, max;
DWORD dwCode, work;
CString strError;

	max = sizeof kc1 / sizeof kc1[0] ;
//	kjBase.RemoveAll();

	for( i = 0; i < max; i++ ) {
		kjArray.Add( kc1[i] );
//		kjBase.Add( kc1[i] );
	}

	// çHéñÉ{ÉäÉÖÅ[ÉÄÇÊÇËéÛì¸ã‡ÇéÊìæÇ∑ÇÈÅB
	CKJVOL kjv( pZm->m_database );

	TRY {
		if( kjv.Open() ) {
			while( ! kjv.IsEOF() ) {
				kjv.MoveNext();
			}
			if( kjv.GetRecordCount() > 0 ) {
				CString *pucd;

				for( int n = 0; n < 10; n++ ) {
					switch(n) {
					case 0:	pucd = &kjv.m_VOL_PUCD01;	break;
					case 1:	pucd = &kjv.m_VOL_PUCD02;	break;
					case 2:	pucd = &kjv.m_VOL_PUCD03;	break;
					case 3:	pucd = &kjv.m_VOL_PUCD04;	break;
					case 4:	pucd = &kjv.m_VOL_PUCD05;	break;
					case 5:	pucd = &kjv.m_VOL_PUCD06;	break;
					case 6:	pucd = &kjv.m_VOL_PUCD07;	break;
					case 7:	pucd = &kjv.m_VOL_PUCD08;	break;
					case 8:	pucd = &kjv.m_VOL_PUCD09;	break;
					default:
						pucd = &kjv.m_VOL_PUCD10;		break;
					}
					pucd->Trim();
					if( ! pucd->IsEmpty() ) {
						//éÛì¸ã‡ÇÕñæç◊Ç≈ìoò^Ç≥ÇÍÇÈÇΩÇﬂÅAägí£ï™ÇÕñ≥éãÇ∑ÇÈÅB
						kc.chkCode = _atoh( (LPSTR)(LPCTSTR)*pucd );
						kc.chkMask = 0xffffffff;
						kjArray.Add( kc );
					}
				}
			}
			kjv.Close();
		}
	}
	CATCH( CMemoryException, me ) {
		me->GetErrorMessage( strError.GetBuffer(_MAX_PATH), _MAX_PATH );
		strError.ReleaseBuffer();
		TRACE( "%s\n", strError );
//		ermset( 0, (char*)(LPCTSTR)strError );
//		return;	
	}
	CATCH( CDBException, de ) {
		strError = de->m_strError;
		TRACE( "%s\n", strError );
//		ermset( 0, (char*)(LPCTSTR)strError );
//		return;
	}
	END_CATCH

	// è¨ï™óﬁÇ≈í«â¡Ç≥ÇÍÇΩçÄñ⁄ÇÉ`ÉFÉbÉNÇ∑ÇÈ
	for( i = 0; i < pZm->gkn_info->reccount; i++ ) {
		pGk = pZm->pGkrec + i;

		if( pGk->gksrc > 0 ) {
			dwCode = pGk->gksrc;
			hl_rev( &dwCode, 4 );

			if( (dwCode & 0x0000F000) < 0x00008000 ) {
				pBaseGk = pGk;
			}
			else {
				// í«â¡å≥Ç™ÅAÇ‹ÇæÅAí«â¡è¨ï™óﬁÇÃÇ∆Ç´
				// í«â¡Ç≈Ç»Ç¢çáåvñºèÃÉTÅ[É`
				pBaseGk = get_gksrc( pGk->gksrc, pZm );

				dwCode = pBaseGk->gksrc;
				hl_rev( &dwCode, 4 );
			}

			// í«â¡â»ñ⁄Ç≈ÅAçHéñî‘çÜì¸óÕòRÇÍÇÃâ»ñ⁄
			switch( (dwCode & 0xffffff00) ) {
			case 0x01040300:	// äÆê¨çHéññ¢é˚ã‡
			case 0x01060200:	// ñ¢ê¨çHéñéxèoã‡
			case 0x04050200:	// ñ¢ê¨çHéñéÛì¸ã‡
			case 0x08010100:	// äÆê¨çHéñçÇ
			case 0x08020100:	// äÆê¨çHéñílà¯

				work = pGk->gkcod;
				hl_rev( &work, 4 );
				kc.chkCode = (work & 0xffffff00);
				kc.chkMask = 0xffffff00;
				kjArray.Add( kc );

				break;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
CKojiDataCheck::CKojiDataCheck()
{
	pSub = NULL;
}


CKojiDataCheck::~CKojiDataCheck()
{
	if( pSub != NULL ) {
		delete pSub;
		pSub = NULL;
	}
}


int	CKojiDataCheck::SetDBZmSub( CDBZmSub* pZmSub )
{
	if (pSub != NULL) {
		delete pSub;
		pSub = NULL;
	}

	pSub = new CKojiChkSub;

	if( pSub == NULL )	return -1;

	return pSub->SetDBZmSub( pZmSub );
}


int	CKojiDataCheck::KojiCheckInit()
{
	if( pSub == NULL )	return -1;

	return pSub->KojiCheckInit();
}

int	CKojiDataCheck::KojiCheckUpdate()
{
	if( pSub == NULL )	return -1;

	return pSub->KojiCheckUpdate();
}

BOOL CKojiDataCheck::KojiKmkCheck( DWORD kmkcode )
{
	if( pSub == NULL )	return -1;

	return pSub->KojiKmkCheck(kmkcode);
}

BOOL CKojiDataCheck::KojiKmkCheck( CString strCode )
{
	if( pSub == NULL )	return -1;

	return pSub->KojiKmkCheck(strCode);
}