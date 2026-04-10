/*====================

	zanask.c

======================*/
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
#include "zanask.h"

#include "etcdef.h"
#include "TaiData.h"

static struct _TTLDSP_TBL TTLDSP_TBL;	// defined to "etcdef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//extern	int	errno;


//-----------------------------------------------------
//	貸借バランス表示
//	mode は TaiData.hの MODE_XXXXXX
//-----------------------------------------------------
struct _TTLDSP_TBL* ttldsp( int mode )
{
char	GN[_BIN&0x0f] = {0}, GS[_BIN&0x0f] = {0}, GT[_BIN&0x0f] = {0}, NS[_BIN&0x0f] = {0},
		NT[_BIN&0x0f] = {0}, NX[_BIN&0x0f] = {0}, DV2[_BIN&0x0f] = {0}, DVX[_BIN&0x0f] = {0},
		GS_D[_BIN&0x0f] = {0}, GS_C[_BIN&0x0f] = {0};

int i, ofs;
long	TL;
register int	SSW, rno;
char pbuf[128], tmp[10] = {0};
char wk1[6] = {0}, wk2[6] = {0};
int syoguti_err = FALSE;

	// データ更新のため クローズ ＆ オープン
	if( pDBzm->taisyaku ) {
		pDBzm->TaisyakuFree();
	}
	if( pDBzm->datalin ) {
		pDBzm->DatalinFree();
	}

	if( pDBzm->TaisyakuRead() == ERR )
		return NULL;
	if( pDBzm->DatalinRead() == ERR )
		return NULL;

	/* 諸口科目レコード Get	*/
	CString strCode;
	strCode = SYOGT;
	if( !(rno = pjsksrc( strCode )) )
		return NULL;
	rno -= 1;

	if ( mode == MODE_NORMAL ) {
		TTLDSP_TBL.TITLE_BAR = "各月合計";	//"合計問合せ";
	}
	else if( mode == MODE_END ) {
		TTLDSP_TBL.TITLE_BAR = "諸口貸借が一致していない月があります。仕訳入力を終了しますか?";
	}
	else {
		//会社再選択時
		TTLDSP_TBL.TITLE_BAR = "諸口貸借が一致していない月があります。";
	}

	TTLDSP_TBL.m_Mode = mode;

	TTLDSP_TBL.TITLE[0] = "月";
	TTLDSP_TBL.TITLE[1] = "通常合計     ";
	TTLDSP_TBL.TITLE[2] = "借方諸口     ";
	TTLDSP_TBL.TITLE[3] = "貸方諸口     ";
	TTLDSP_TBL.TITLE[4] = "諸口差額     ";
	TTLDSP_TBL.TITLE[5] = "入力件数 ";

	l_input( DV2, "2" );

	TL = 0;

	i = ofs = 0;
	pDBzm->minfo->MoveFirst();
	if( pDBzm->minfo->st == -1 )	return NULL;
	l_defn (_BIN);
	
	do
	{
		if( pDBzm->minfo->ofset > 15 )	break;

		if( !(pDBzm->minfo->insw & 0x01) )
			continue;

		if( pDBzm->minfo->kes_sgn && !(pDBzm->minfo->insw & 0x01) )
			continue;

		ofs = pDBzm->minfo->ofset;

		l_clear(NS);
		l_clear(NT);

		// TOTAL 仕訳数
		TL = TL + pDBzm->datalin->dcnt[ofs];

		//諸口貸借チェック
		if ( l_jsgn (pDBzm->taisyaku->syoguchi[ofs].Kari, pDBzm->taisyaku->syoguchi[ofs].Kasi, 6) == 0 ) 
			{ SSW = 0; }
		else 
			{ SSW = 4; syoguti_err = TRUE;}
		//月分
		switch( pDBzm->minfo->kes_sgn ) {
		case 1:TTLDSP_TBL.DSP_REC[i].GATUBUN = "Q1"; break;
		case 2:TTLDSP_TBL.DSP_REC[i].GATUBUN = "半"; break;
		case 3:TTLDSP_TBL.DSP_REC[i].GATUBUN = "Q3"; break;
	//	case 1:case 2: case 3:
	//			TTLDSP_TBL.DSP_REC[i].GATUBUN = "中"; break;
		case 4:	TTLDSP_TBL.DSP_REC[i].GATUBUN = "決"; break;
		default:
			int mn = ( pDBzm->minfo->emd / 100);
			mn = (mn%100);
			TTLDSP_TBL.DSP_REC[i].GATUBUN.Format( "%2d", mn );
			break;
		}

		TTLDSP_TBL.DSP_REC[i].GATUBUN_ATTR = SSW;

		//入力件数
		sprintf_s( pbuf, sizeof pbuf, "%d", pDBzm->datalin->dcnt[ofs] );
		l_input( TTLDSP_TBL.DSP_REC[i].INPLINE, pbuf );

		//通常
//L_PRINT( "tsujyo kari", pDBzm->taisyaku->tsujyo[ofs].Kari ); 
//L_PRINT( " tsujyo kasi", pDBzm->taisyaku->tsujyo[ofs].Kasi ); 
//L_PRINT( " syog kari", pDBzm->taisyaku->syoguchi[ofs].Kari ); 
//L_PRINT( " syog kasi", pDBzm->taisyaku->syoguchi[ofs].Kasi ); 
//TRACE( "\n" );

		// 通常から諸口分を差し引く
//		l_sub( wk1, pDBzm->taisyaku->tsujyo[ofs].Kari, pDBzm->taisyaku->syoguchi[ofs].Kasi );
//		l_sub( wk2, pDBzm->taisyaku->tsujyo[ofs].Kasi, pDBzm->taisyaku->syoguchi[ofs].Kari );
//		if( l_cmp( wk1, wk2 ) >= 0 )
//				memmove( TTLDSP_TBL.DSP_REC[i].TUJYO, wk1, 6 );
//		else	memmove( TTLDSP_TBL.DSP_REC[i].TUJYO, wk2, 6 );
		memmove( wk1, pDBzm->taisyaku->tsujyo[ofs].Kari, 6 );
		memmove( TTLDSP_TBL.DSP_REC[i].TUJYO, wk1, 6 );

		//諸口
		memmove( TTLDSP_TBL.DSP_REC[i].SYOG_DEB, pDBzm->taisyaku->syoguchi[ofs].Kari, 6 );
		memmove( TTLDSP_TBL.DSP_REC[i].SYOG_CRE, pDBzm->taisyaku->syoguchi[ofs].Kasi, 6 );

//		l_add( tmp, pDBzm->taisyaku->tsujyo[ofs].Kari, pDBzm->taisyaku->tsujyo[ofs].Kasi );
		memcpy( tmp, wk1, 6 );
		l_add (GN, GN, tmp );

		l_add (NS, pDBzm->taisyaku->syoguchi[ofs].Kari, pDBzm->taisyaku->syoguchi[ofs].Kasi );
		// 借・貸 諸口
		l_add( GS_D, GS_D, pDBzm->taisyaku->syoguchi[ofs].Kari );
		l_add( GS_C, GS_C, pDBzm->taisyaku->syoguchi[ofs].Kasi );

		l_add (GS, GS, NS) ; l_div (NX, NS, DV2);
		l_add (NT, NX, tmp ) ; l_add (GT, GT, NT);
		l_print( pbuf, NT, FMT18 );

#ifdef CLOSE // この ifdef を ON にすると DSP_REC[0].GATUBUN が 0 になったり、Endで 落ちる
		volp->tai_mn = pbuf;
//		memmove(volp->tai_mn, NT, 6);
#endif
		//貸借
		memmove( TTLDSP_TBL.DSP_REC[i].TAISYAKU, NT, 6 );

		i++;
	} while( pDBzm->minfo->MoveNext() == 0 );

	// i == 14
	TTLDSP_TBL.DSP_REC[i].GATUBUN = "計";

	//入力件数合計
	l_let( TTLDSP_TBL.DSP_REC[i].INPLINE, &TL, 0x40|sizeof(TL) );

	//通常合計
	memmove( TTLDSP_TBL.DSP_REC[i].TUJYO, GN, 6 );
	//諸口合計
//	memmove( TTLDSP_TBL.DSP_REC[i].SYOGUTI, GS, 6 );
	memmove( TTLDSP_TBL.DSP_REC[i].SYOG_DEB, GS_D, 6 );
	memmove( TTLDSP_TBL.DSP_REC[i].SYOG_CRE, GS_C, 6 );

	//貸借合計
	memmove( TTLDSP_TBL.DSP_REC[i].TAISYAKU, GT, 6 );
	TTLDSP_TBL.TTL_CNT = (i+1);
 
	return((mode!=MODE_NORMAL && !syoguti_err) ? NULL : &TTLDSP_TBL );
	
}


/*=================================================*
	Tatal balanced display(未使用)
*==================================================*/
struct _TTLDSP_TBL* ttldsp( int mode, CMINFO *volp )
{
char	GN[_BIN&0x0f] = {0}, GS[_BIN&0x0f] = {0}, GT[_BIN&0x0f] = {0}, NS[_BIN&0x0f] = {0},
		NT[_BIN&0x0f] = {0}, NX[_BIN&0x0f] = {0}, DV2[_BIN&0x0f] = {0}, DVX[_BIN&0x0f] = {0},
		GS_D[_BIN&0x0f] = {0}, GS_C[_BIN&0x0f] = {0};

int i, ofs;
long	TL;
register int	SSW, rno;
char pbuf[128], tmp[10] = {0};
int syoguti_err = FALSE;

struct	_DIP_KZREC	mbuf;
//struct	_KZREC	mbuf;
//struct	_KNREC	knam;

	/* 諸口科目レコード Get	*/
	CString strCode;
	strCode = SYOGT;
	if( !(rno = pjsksrc( strCode )) )
		return NULL;
	rno -= 1;

	if( zrecget2( strCode, &mbuf ))
		return NULL;

	if ( mode ) {
		TTLDSP_TBL.TITLE_BAR = "各月合計";	//"合計問合せ";
	}
	else
	{
		TTLDSP_TBL.TITLE_BAR = "諸口貸借が一致していない月があります。仕訳入力を終了しますか?";
	}

	TTLDSP_TBL.m_Mode = mode;

	TTLDSP_TBL.TITLE[0] = "月";
	TTLDSP_TBL.TITLE[1] = "通常合計     ";
	TTLDSP_TBL.TITLE[2] = "借方諸口     ";
	TTLDSP_TBL.TITLE[3] = "貸方諸口     ";
	TTLDSP_TBL.TITLE[4] = "諸口差額     ";
	TTLDSP_TBL.TITLE[5] = "入力件数 ";

	l_input( DV2, "2" );

	TL = 0;

	i = ofs = 0;
	volp->MoveFirst();
	if( volp->st == -1 )	return NULL;
	l_defn (_BIN);
	
	do
	{
		if( volp->ofset > 15 )	break;
		if( volp->kes_sgn && !(volp->insw & 0x01) )
			continue;

		ofs = volp->ofset;

TRACE( "a - GATUBUN %s, i = %d, ofs = %d\n", TTLDSP_TBL.DSP_REC[0].GATUBUN, i, ofs );

		l_clear(NS);
		l_clear(NT);
		l_input( tmp, (void*)(LPCTSTR)volp->tol_mn );
		l_add (GN, GN, tmp );
		l_add (NS, mbuf.kz_kari[ofs], mbuf.kz_kasi[ofs] );
		// 借・貸 諸口
		l_add( GS_D, GS_D, mbuf.kz_kari[ofs] );
		l_add( GS_C, GS_C, mbuf.kz_kasi[ofs] );

		l_add (GS, GS, NS) ; l_div (NX, NS, DV2);
		l_add (NT, NX, tmp ) ; l_add (GT, GT, NT);
		l_print( pbuf, NT, FMT18 );

#ifdef CLOSE // この ifdef を ON にすると DSP_REC[0].GATUBUN が 0 になったり、Endで 落ちる
		volp->tai_mn = pbuf;
//		memmove(volp->tai_mn, NT, 6);
#endif
		TL = TL + volp->lin_mn;

		//諸口貸借チェック
		if ( l_jsgn (mbuf.kz_kari[ofs], mbuf.kz_kasi[ofs], 6) == 0 ) 
			{ SSW = 0; }
		else 
			{ SSW = 4; syoguti_err = TRUE;}
		//月分
		switch( volp->kes_sgn ) {
	//	case 1:TTLDSP_TBL.DSP_REC[i].GATUBUN = "１Ｑ"; break;
	//	case 2:TTLDSP_TBL.DSP_REC[i].GATUBUN = "中"; break;
	//	case 3:TTLDSP_TBL.DSP_REC[i].GATUBUN = "３Ｑ"; break;
		case 1:case 2: case 3:
				TTLDSP_TBL.DSP_REC[i].GATUBUN = "中"; break;
		case 4:	TTLDSP_TBL.DSP_REC[i].GATUBUN = "決"; break;
		default:
			int mn = (volp->emd / 100);
			mn = (mn%100);
			TTLDSP_TBL.DSP_REC[i].GATUBUN.Format( "%2d", mn );
			break;
		}

		TTLDSP_TBL.DSP_REC[i].GATUBUN_ATTR = SSW;

		//入力件数
		sprintf_s( pbuf, sizeof pbuf, "%d", volp->lin_mn );
		l_input( TTLDSP_TBL.DSP_REC[i].INPLINE, pbuf );
//		l_let( TTLDSP_TBL.DSP_REC[i].INPLINE, &volp->D_TBL[i].LIN_MN, 0x40|sizeof(volp->D_TBL[i].LIN_MN) );

		//通常
		l_input( TTLDSP_TBL.DSP_REC[i].TUJYO, (void*)(LPCTSTR)volp->tol_mn );
		//諸口
//		memmove( TTLDSP_TBL.DSP_REC[i].SYOGUTI, NS, 6 );
		memmove( TTLDSP_TBL.DSP_REC[i].SYOG_DEB, mbuf.kz_kari[ofs], 6 );
		memmove( TTLDSP_TBL.DSP_REC[i].SYOG_CRE, mbuf.kz_kasi[ofs], 6 );

		//貸借
		memmove( TTLDSP_TBL.DSP_REC[i].TAISYAKU, NT, 6 );

		i++;
	} while( volp->MoveNext() == 0 );

	// i == 14
	TTLDSP_TBL.DSP_REC[i].GATUBUN = "計";

	//入力件数合計
	l_let( TTLDSP_TBL.DSP_REC[i].INPLINE, &TL, 0x40|sizeof(TL) );

	//通常合計
	memmove( TTLDSP_TBL.DSP_REC[i].TUJYO, GN, 6 );
	//諸口合計
//	memmove( TTLDSP_TBL.DSP_REC[i].SYOGUTI, GS, 6 );
	memmove( TTLDSP_TBL.DSP_REC[i].SYOG_DEB, GS_D, 6 );
	memmove( TTLDSP_TBL.DSP_REC[i].SYOG_CRE, GS_C, 6 );

	//貸借合計
	memmove( TTLDSP_TBL.DSP_REC[i].TAISYAKU, GT, 6 );
	TTLDSP_TBL.TTL_CNT = (i+1);
 
	return((!mode && !syoguti_err) ? NULL : &TTLDSP_TBL );
	
}


int zrecget2( LPCTSTR code, struct _DIP_KZREC *mrec )
{
	pDBzm->kzrec->MoveFirst();
	int i, sgn = 0;
	::ZeroMemory( mrec, sizeof(struct _DIP_KZREC) );
	CString tmp;

	BOOL bSyog;
	bSyog = isSYOGT_CODE( code );

	do {
		tmp	= pDBzm->kzrec->kzkcd;
		tmp.TrimRight();

		if( bSyog ) {
			if( isSYOGT_CODE( tmp ) ) {	// 複数諸口がある場合のため
				mrec->kz_att	= pDBzm->kzrec->kzatt;
				char tmp[6];
				// 集計
				l_input( tmp, (void*)(LPCTSTR)pDBzm->kzrec->kzzan );
				l_add( mrec->kz_zan, mrec->kz_zan, tmp );	// 開始残高
				for( i = 0 ; i < 16 ; ++i )					// 各月
				{
					l_input( tmp, (void*)(LPCTSTR)pDBzm->kzrec->lhs[i] );
					l_add( mrec->kz_kari[i], mrec->kz_kari[i], tmp );	// 借方
					l_input( tmp, (void*)(LPCTSTR)pDBzm->kzrec->rhs[i] );
					l_add( mrec->kz_kasi[i], mrec->kz_kasi[i], tmp );	// 貸方
				}
				sgn++;
			}
			else if( sgn )	break;
		}
		else {
			if( tmp == code ) {
				strcpy_s( mrec->kz_code, sizeof mrec->kz_code, pDBzm->kzrec->kzkcd );
				mrec->kz_att	= pDBzm->kzrec->kzatt;
				l_input( mrec->kz_zan, (void*)(LPCTSTR)pDBzm->kzrec->kzzan );

				for( int i = 0; i < 16; i++ ) {
					l_input( mrec->kz_kari[i], (void*)(LPCTSTR)pDBzm->kzrec->lhs[i] );
					l_input( mrec->kz_kasi[i], (void*)(LPCTSTR)pDBzm->kzrec->rhs[i] );
				}
				
				break;
			}
		}

	} while( pDBzm->kzrec->MoveNext() == 0 );

// クエリーバージョン　データ件数多いと、1件のクエリでも時間がかかる
// ので、不採用
#ifdef _KEEP_CLOSE
	tmp.Format( "kzkcd = '%s'", code );
	pDBzm->kzrec->Requery( tmp );
	if( pDBzm->kzrec->st == 0 ) {
		strcpy_s( mrec->kz_code, sizeof mrec->kz_code, pDBzm->kzrec->kzkcd );
		mrec->kz_att	= pDBzm->kzrec->kzatt;
		l_input( mrec->kz_zan, (void*)(LPCTSTR)pDBzm->kzrec->kzzan );

		for( int i = 0; i < 16; i++ ) {
			l_input( mrec->kz_kari[i], (void*)(LPCTSTR)pDBzm->kzrec->lhs[i] );
			l_input( mrec->kz_kasi[i], (void*)(LPCTSTR)pDBzm->kzrec->rhs[i] );
		}
	}
#endif

	return 0;
}

//
//	速度改善のため、メモリーの科目残高を使用
//		KamokuZanRead で時間がかかるので、なし
//
int zrecget2X( LPCTSTR code, struct _DBKZREC *mrec )
{
	int i, sgn = 0;
	DWORD	dwCode;
	CString tmp, strCode;
	struct _DBKZREC*	pz;
	BOOL bSyog;

	::ZeroMemory( mrec, sizeof(struct _DBKZREC) );
	bSyog = isSYOGT_CODE( code );

	for( i = 0; i < pDBzm->kzn_info->reccount; i++ ) {
		pz		= (pDBzm->pKzrec+i);
		dwCode	= pz->kzkcd;
		hl_rev( &dwCode, 4 );
		strCode.Format( "%08x", dwCode );

		if( bSyog ) {
			if( isSYOGT_CODE( strCode ) ) {	// 複数諸口がある場合のため
				mrec->kzatt	= pz->kzatt;
				
				// 集計
				l_add( mrec->kzzan, mrec->kzzan, pz->kzzan );	// 開始残高
				for( i = 0 ; i < 19; ++i )		// 各月
				{
					l_add( mrec->kzval[i].Kari, mrec->kzval[i].Kari, pz->kzval[i].Kari );	// 借方
					l_add( mrec->kzval[i].Kasi, mrec->kzval[i].Kasi, pz->kzval[i].Kasi );	// 貸方
				}
				sgn++;
			}
			else if( sgn )	break;
		}
		else {
			if( strCode.CompareNoCase( code ) ) {
				memcpy( mrec, pz, sizeof(struct _DBKZREC) );
				break;
			}
		}
	}

	return 0;
}
