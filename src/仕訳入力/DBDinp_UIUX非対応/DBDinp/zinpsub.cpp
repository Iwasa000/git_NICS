/*=========================

	input sub module
		source	zinpsub.c

==========================*/

#include "stdafx.h"
#include "SubFnc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CDBDinpView
#include "DBDinp.h"
#include "DBDinpDoc.h"
#include "DBDinpView.h"
extern class CDBDinpView *p_DBDinpView;

#include "sys\types.h"
#include "sys\stat.h"
#include "io.h"

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

#include "font.h"

//
#include "Dinpsub.h"

#include "bit.h"
extern CBit	MODEFY_BIT;		//修正仕訳ビットテーブル
extern CBit	SYZMDFY_BIT;	//消費税サイン修正ビットテーブル

extern CBrnTky m_BrTek;


extern struct _DataTable SelFix8[2*4];
extern struct _DataTable SelVal32[8*4];

extern struct _AUTOSEL *pAUTOSEL;	// 02.20 /02 科目コード表示のため

extern int DB_SyIniz( CDBINPDataRec* data );

#ifdef _SLIP_ACCOUNTS_
extern	int	_IS_SLIP_MASTER;	// 原票保存マスター？
#endif

#include "BmnChk.h"
extern CBmnHaniCheck	BmChk;


#ifdef DBG_SYINIZ
extern;
void DB_SyInizTrace(CDBINPDataRec* rec, int line/* = 0*/, char* func/*= NULL*/, char* fname/* = NULL*/);

#define DB_SyIniz( data )	DB_SyInizTrace(data, __LINE__, __FUNCTION__, __FILE__)
#endif
	

int ispread( int locate, CDBINPDataRec *buff )
{
// 仕様変更 により、dbdata は常に、全データを RecordSet にのせない
ASSERT( FALSE );
return -1;

	int ret = -1;

	// 他ユーザが仕訳を追加している場合があるので、 最大チェックはやめる
	if( locate >= 0 /*&& locate < pDBzm->m_dacnt*/ ) {

		if( pDBzm->DBDATA_SetPosition( locate+1 ) == 0 ) {
			pDBzm->GetCDBData( buff );
			ret = 0;
		}
		else {
			ASSERT( FALSE );
		}
	}

	return ret;
}
 
// CDATA より、データを取得
int cdata_read( int locate, CDBINPDataRec *buff )
{
	int ret = -1;

	if( locate >= 0 ) {
		CString filter;

		if( pDBzm->data_Requery() ) {
		//	filter.Format( "seq = %d", locate+1 );
		//	pDBzm->data->Requery( -2, 0, filter, 0, 0 );
			pDBzm->data->Requery( -2, 0, "", 0, 0 );
			pDBzm->data_Requery() = FALSE;
		}
	//	else {
			pDBzm->data->SetAbsolutePosition( locate+1 );
	//	}

		if( pDBzm->data->st == 0 ) {
			pDBzm->GetCData( buff );
			ret = 0;
		}
		else {
			ASSERT(FALSE);
		}
	}

	return ret;
}


void nline_del( struct _DATA_LINE *ddata, int ddata_ofs )
{
int n;

	n = ddata_ofs;	//SCROLL_NL - (DATA_HL - lin );

	inpline_inz( ddata + n );
}


// 科目コードをint で返送 ... 02.20 /02
int get_hyojikncod( int n ) 
{
	int kmcod = -1;

	struct _DBKNREC* pKn;

	if( (pKn = kmtbl_get( n )) != NULL ) {
		kmcod = pKn->knicod;
	}

	return kmcod;
}


//
// 選択表示用科目名称作成（消費税属性付）
//										04.17 /98 追加
char *get_hyojiknam( int n )
{
	static CString pbuf;
	CString fmt;
	unsigned char	ch;
	char *chmsg;
	BOOL bKcod;
	char codbuf[20] = {0};
	int kcod;

	struct _DBKNREC* pKn;
	pKn = kmtbl_get( n );

	fmt.Format( _T("%%c%%.%ds%%s"), KMNLEN );

	if( bKcod = (pAUTOSEL->KMNO_OPT == OPT_KMNO) ) {	//02.20 /02 -- 科目コード表示
		kcod = pKn->knicod;
		if( kcod == -1 )
			strcpy_s( codbuf, sizeof codbuf, "    " );
		else
			sprintf_s( codbuf, sizeof codbuf, "%04d", kcod );

		fmt.Format( _T("%%s%%c%%.%ds%%s"), KMNLEN );

#ifdef CUT //#* 08.05 /02
		// 科目コードが3桁の時 ｽﾍﾟｰｽをｾｯﾄ
		if( !(kcod /1000) ) {
			if( !(kcod /100) ) {
				if( !(kcod /10) ) {
					fmt.Format( _T("   %%c%%.%ds%%s"), KMNLEN );
				}
				else {
					fmt.Format( _T("  %%c%%.%ds%%s"), KMNLEN );	
				}
			}
			else {
				fmt.Format( _T(" %%c%%.%ds%%s"), KMNLEN );		
			}
		}
#endif
	}

	switch(  (pKn->knsgn[2] & 0x0f) )
	{
	case 0x01:	ch = '*'; chmsg = bKcod ? _T(" /不") : _T("／不課"); break;
	case 0x03:	ch = '*'; chmsg = bKcod ? _T(" /非") : _T("／非課"); break;
	default:	ch = ' '; chmsg = _T(""); break;
	}

	if( !bKcod ) pbuf.Format( fmt, ch, pKn->knnam, chmsg );	
	else		 pbuf.Format( fmt, codbuf, ch, pKn->knnam, chmsg );

	return (char*)(LPCTSTR)pbuf;

}



/*-------------------------------
	pjis code to kamoku pointer
	pjsksrc( pjcd, rwtbl )
		found -- TRUE
	not found -- FALSE
--------------------------------*/
int pjsksrc( LPCTSTR pjis_code )
{
int rno;
	rno = pDBzm->DB_PjisToKmkOfst( pjis_code );

	if( rno >= 0 )	rno++;
	else			rno = 0;

	return rno;
}



/*-------------------------------
	Pjis -> DBKNREC
--------------------------------*/
struct _DBKNREC* pjs_knrec( LPCTSTR pjis_code )
{
//int rno;
struct _DBKNREC	*pKn;

	if( pKn = pDBzm->DB_PjisToKnrec( pjis_code ) )
	{
		return pKn;
	}
	return NULL;
}



/*-----------------------------------------------------
	rensou block tekiyo code search
	rentksr( short arg2 )
		return # of counts
------------------------------------------------------*/
int rentksr( short arg2 )
{
	int		i, rtno;	

	rtno = arg2;

	pDBzm->rtrec->MoveFirst();
	i = 0;
	do {
		if( rtno == pDBzm->rtrec->rtpgn ) {
			if( (pDBzm->rtrec->rtcod < 1) ) continue;
			
			i++;
		}
	} while( pDBzm->rtrec->MoveNext() == 0 );

	return i;
}


/*---------------------------------------------
	ﾃﾞｰﾀ ﾋｮｳｼﾞ (2ﾗｲﾝ ｽｸﾛｰﾙ ﾌﾞ)
----------------------------------------------*/
void recdsp( CDBINPDataRec *data, int data_ofs, struct _DATA_LINE *ddata, int ddata_ofs )

{
	int n;

	data += data_ofs;

	if( data->m_ddate == 0 ) return;

	n = ddata_ofs;

	//SEQ
	(ddata + n)->_XXDTA.dd_seq = data->m_seq;
	(ddata + n)->LINE_CND[ SEQ_PN ].INP_sg = TRUE;
	
	_recdsp( data, ddata+n );

	(ddata + n)->ZREC_NO = data_ofs + 1;
}

void _recdsp( CDBINPDataRec *data, struct _DATA_LINE *ddata, BOOL bEdaDisp/*=TRUE*/, BOOL invDisp/*= FALSE*/)
{
	char	pbuf[128];
	int i = 0, sg;

	(ddata)->_XXDTA.dd_ddate = data->m_ddate;
	(ddata)->_XXDTA.dd_ksign = data->m_ksign;
	(ddata)->_XXDTA.dd_mofs = data->m_mofs;
	pDBzm->DateConv( (ddata)->BcdDate, data, CV2_1 );

	(ddata)->LINE_CND[ DATE_PN ].INP_sg = TRUE;

	CString stmp;
	if( data->m_icno > 0 ) {
		DenpTypeString( data->m_dtype, stmp );
	}

	if( stmp.IsEmpty() ) {
		::ZeroMemory( (ddata)->Denp, sizeof (ddata)->Denp );
		(ddata)->_XXDTA.dd_udtbl[0].userdata = NULL;
	}
	else {
		strcpy_s( (ddata)->Denp, sizeof (ddata)->Denp, stmp );
		(ddata)->_XXDTA.dd_udtbl[0].userdata = (BYTE*)(ddata)->Denp;
	}

	(ddata)->_XXDTA.dd_cno = data->m_cno;
	(ddata)->LINE_CND[ DENP_PN ].INP_sg = TRUE;

	if( BMON_LNG ){
		(ddata)->_XXDTA.dd_dbmn = data->m_dbmn;
		_set_bmndsp( ddata, BMON_PN );

		(ddata)->_XXDTA.dd_cbmn = data->m_cbmn;
		_set_bmndsp( ddata, CBMN_PN );

	}
	(ddata)->LINE_CND[ BMON_PN ].INP_sg = TRUE;
	(ddata)->LINE_CND[ CBMN_PN ].INP_sg = TRUE;

	if( M_KOJI ) {
		(ddata)->_XXDTA.dd_dkno = data->m_dkno;
		_set_bmndsp( ddata, KOJI_PN );

		(ddata)->_XXDTA.dd_ckno = data->m_ckno;
		_set_bmndsp( ddata, CKJI_PN );
	}
	(ddata)->LINE_CND[ KOJI_PN ].INP_sg = TRUE;
	(ddata)->LINE_CND[ CKJI_PN ].INP_sg = TRUE;

	/* ｶﾘｶﾀ */
	set_kamoku( data, ddata, DEBT_PN, bEdaDisp );
	
	/* ｶｼｶﾀ */
	set_kamoku( data, ddata, CRED_PN, bEdaDisp );

	/* ｷﾝｶﾞｸ */
	memmove( (ddata)->_XXDTA.dd_val, data->m_val, sizeof(data->m_val));
	(ddata)->_XXDTA.val_flag = TRUE;
	(ddata)->LINE_CND[ VAL_PN ].INP_sg = TRUE;

	/* ｾﾞｲｶﾞｸ */
	memmove( (ddata)->_XXDTA.dd_zei, data->m_zei, sizeof(data->m_zei));

	if( is_syohizeisiwake( data ) ) {
		(ddata)->_XXDTA.zei_flag = TRUE;
		(ddata)->LINE_CND[ ZEI_PN ].INP_sg = TRUE;
	}
	else {
		(ddata)->_XXDTA.zei_flag = FALSE;
		(ddata)->LINE_CND[ ZEI_PN ].INP_sg = FALSE;
	}

	sg = 0;
	COLORREF back;
	if (isnot_defzeigaku(data, back)) sg = 1;
	if (sg > 0)	(ddata)->_XXDTA.dd_ctbl[ZEI_PN].back = back;
	else		(ddata)->_XXDTA.dd_ctbl[ZEI_PN].back = (COLORREF)-1;

	/* 消費税 */
	DB_SyIniz( data );

	(ddata)->_XXDTA.dd_syz_str = sy_line_txt( data );
	sg = SYZMDFY_BIT.bitncnd( data->m_seq - 1);
	if( sg > 0 )	(ddata)->_XXDTA.dd_ctbl[SKBN_PN].back = RGB_SYZ_MDFY;
	else			(ddata)->_XXDTA.dd_ctbl[SKBN_PN].back = (COLORREF)-1;
	(ddata)->LINE_CND[ SKBN_PN ].INP_sg = TRUE;

	/* 非課税有価証券譲渡仕訳 */
	if( pDBsy->IsTaikaData( pDBzm->dbdata ) == 2)
		(ddata)->_XXDTA.zei_flag |= 0x10;

	/* ﾃｷﾖｳ */
	if( (uchar)data->m_dsign[0] & 0x80 )
	{
		(ddata)->LINE_CND[ TKY_PN ].IMG_sg = 1;
		(ddata)->ImgSEQ = data->m_imgsq;
		strcpy_s( pbuf, sizeof pbuf, ""/*Image*/ );
	}
	else {
		(ddata)->LINE_CND[ TKY_PN ].IMG_sg = 0;
	//	i = get_inpl( (unsigned char *)data->_TKS, Voln1->TK_LN ) * 2;
		i = get_inpl( (unsigned char *)(LPCTSTR)data->m_tekiyo, Voln1->tk_ln );	// 02.09 /02 -ANKINP-
		if( i )	strncpy_s( pbuf, sizeof pbuf, (LPCTSTR)data->m_tekiyo, i );
		pbuf[i] = 0;
	}
	(ddata)->_XXDTA.dd_tek.tekiyo = pbuf;
	(ddata)->LINE_CND[ TKY_PN ].INP_sg = TRUE;

	/* ﾃｶﾞﾀﾊﾞﾝｺﾞｳ */
	(ddata)->_XXDTA.dd_snumber = data->m_rnumber;
	(ddata)->LINE_CND[ SYON_PN ].INP_sg = TRUE;

	/* ﾃｶﾞﾀｷｼﾞﾂ */
	(ddata)->_XXDTA.dd_tdate = data->m_tdate;
	LPSTR p;
	p = (ddata)->_XXDTA.dd_tdstr.GetBuffer(30);
	IntsrekitoGenstr( p, 30, (ddata)->_XXDTA.dd_tdate );
	(ddata)->_XXDTA.dd_tdstr.ReleaseBuffer(-1);
	(ddata)->LINE_CND[ TGDT_PN ].INP_sg = TRUE;
	/* 付箋 */
	(ddata)->_XXDTA.dd_husen = (data->m_dsign[7] & 0x0f);
	(ddata)->_XXDTA.dd_hscomment = data->m_tag;
	/* 変動事由 or 振替区分[公益] */
	_get_hjiyustr( data->m_hjiyu, data->m_nbcd, (ddata)->_XXDTA.dd_hjiyu );

	/* 証憑番号 */
	(ddata)->_XXDTA.dd_docevi = data->m_skn;
	(ddata)->LINE_CND[ DOCEVI_PN ].INP_sg = TRUE;

	/* サイン */
	(ddata)->_XXDTA.dd_dsign = 0;
	if( data->m_dsign[0] & 0x01 )
		(ddata)->_XXDTA.dd_dsign |= 0x01;	// 取消表示
	if( (sg = tline_chk( data )) < 0 ) {
		if( sg == -1 )	(ddata)->_XXDTA.dd_dsign |= 0x02;	// 確定仕訳で修正不可
		else			(ddata)->_XXDTA.dd_dsign |= 0x08;	// 転送済みで修正不可

		// SEQ の Back Color (全体が黄色で見にくくなるため)
		(ddata)->_XXDTA.dd_ctbl[DATA_SEQ].back = SCAN_TITLE_BackCOL;
	}
	else {
		(ddata)->_XXDTA.dd_ctbl[DATA_SEQ].back = (COLORREF)-1;
	}

	//修正サイン
	sg = MODEFY_BIT.bitncnd( (ddata)->_XXDTA.dd_seq - 1);
	if( sg > 0 )	(ddata)->_XXDTA.dd_dsign |= 0x04;

#ifdef _SLIP_ACCOUNTS_
	//原票イメージサイン
	if( _IS_SLIP_MASTER && (data->m_dsign[6]&0x01) ){
		(ddata)->_XXDTA.dd_dsign |= 0x800;	//原票イメージ有
	}
#endif

	if( invDisp ) {
		//免税
		if( !bInvMaster || !bInvUse ) {
			::ZeroMemory((ddata)->Menz, sizeof(ddata)->Menz);
			(ddata)->_XXDTA.dd_udtbl[1].userdata = NULL;
		}
		else {
			if( (data->m_dsign[6] & 0x20) ) {
				strcpy_s((ddata)->Menz, sizeof(ddata)->Menz, "〇");
				(ddata)->_XXDTA.dd_udtbl[1].userdata = (BYTE*)(ddata)->Menz;
			}
			else {
				::ZeroMemory((ddata)->Menz, sizeof(ddata)->Menz);
				(ddata)->_XXDTA.dd_udtbl[1].userdata = NULL;
			}
		}

		//登録番号
		if( !bInvMaster || !bInvUse || !bInvDisp ) {
			::ZeroMemory((ddata)->Invno, sizeof(ddata)->Invno);
			(ddata)->_XXDTA.dd_udtbl[2].userdata = NULL;
		}
		else {
			if( (data->m_invno[0]) ) {
				strcpy_s((ddata)->Invno, sizeof(ddata)->Invno, data->m_invno);
				(ddata)->_XXDTA.dd_udtbl[2].userdata = (BYTE*)(ddata)->Invno;
			}
			else {
				::ZeroMemory((ddata)->Invno, sizeof(ddata)->Invno);
				(ddata)->_XXDTA.dd_udtbl[2].userdata = NULL;
			}
		}
	}
}


#ifdef REGDATA	// 08.07 /00

//----------------------------------------------------------
//	定型仕訳データを 入力バッファにセット
//		返送値：フォーカスをセットするポジション
//----------------------------------------------------------
int _rd_recdsp( CDBINPDataRec *data, struct _DATA_LINE *ddata, BOOL* cnd )
{
	int pn;
	int i;

	if( BMON_MST ) {
		pn = BMON_PN;
	}
	else if( M_KOJI ) {
		pn = KOJI_PN;
	}
	else 
		pn = DEBT_PN;

	if( data->m_ddate > 0 )
	{
		(ddata)->_XXDTA.dd_ddate = data->m_ddate;
		(ddata)->_XXDTA.dd_ksign = data->m_ksign;
		(ddata)->_XXDTA.dd_mofs = data->m_mofs;
		pDBzm->DateConv( (ddata)->BcdDate, data, CV2_1 );

		(ddata)->LINE_CND[ DATE_PN ].INP_sg = TRUE;
	}

	if( data->m_cno >= 0 )
	{
		(ddata)->_XXDTA.dd_cno = data->m_cno;
		(ddata)->LINE_CND[ DENP_PN ].INP_sg = TRUE;
	}
	// 部門
	if( data->m_dbmn >= 0 )
	{
		if( BMON_LNG ) {
			(ddata)->_XXDTA.dd_dbmn = data->m_dbmn;
			if( pn == BMON_PN ) {
				if( M_KOJI )	pn = KOJI_PN;
				else			pn = DEBT_PN;
			}
			_set_bmndsp( ddata, BMON_PN );
		}
		(ddata)->LINE_CND[ BMON_PN ].INP_sg = TRUE;
	}

	if( data->m_cbmn >= 0 )
	{
		if( BMON_LNG ) {
			(ddata)->_XXDTA.dd_cbmn = data->m_cbmn;
			if( pn == BMON_PN ) {
				if( M_KOJI )	pn = KOJI_PN;
				else			pn = DEBT_PN;
			}
			_set_bmndsp( ddata, CBMN_PN );
		}
		(ddata)->LINE_CND[ CBMN_PN ].INP_sg = TRUE;
	}

	// 工事番号
	if( ! data->m_dkno.IsEmpty() )
	{
		if( M_KOJI ) {
			(ddata)->_XXDTA.dd_dkno = data->m_dkno;
			if( pn == KOJI_PN )
				pn = DEBT_PN;

			_set_bmndsp( ddata, KOJI_PN );
		}
		(ddata)->LINE_CND[ KOJI_PN ].INP_sg = TRUE;
	}

	if( ! data->m_ckno.IsEmpty() )
	{
		if( M_KOJI ) {
			(ddata)->_XXDTA.dd_ckno = data->m_ckno;
			if( pn == KOJI_PN )
				pn = DEBT_PN;

			_set_bmndsp( ddata, CKJI_PN );
		}
		(ddata)->LINE_CND[ CKJI_PN ].INP_sg = TRUE;
	}

	/* ｶﾘｶﾀ */
	if( ! data->m_dbt.IsEmpty() )
	{
		set_kamoku( data, ddata, DEBT_PN );

		if( pn == DEBT_PN )
			pn = CRED_PN;
	}
	
	/* ｶｼｶﾀ */
	if( ! data->m_cre.IsEmpty() )
	{
		set_kamoku( data, ddata, CRED_PN );

		if( pn == CRED_PN )
			pn = VAL_PN;
	}

	/* ｷﾝｶﾞｸ */
	if( cnd[0] )
	{
		memmove( (ddata)->_XXDTA.dd_val, data->m_val, sizeof(data->m_val));
		(ddata)->LINE_CND[ VAL_PN ].INP_sg = TRUE;

		if( pn == VAL_PN )
			pn = TKY_PN;
	}

	if( cnd[1] && is_syohizeisiwake( data ) )
	{
		memmove( (ddata)->_XXDTA.dd_zei, data->m_zei, sizeof(data->m_zei));
		(ddata)->LINE_CND[ ZEI_PN ].INP_sg = TRUE;

		if( pn == ZEI_PN )
			pn = TKY_PN;
	}

	/* 消費税 */
	(ddata)->_XXDTA.dd_syz_str = sy_line_txt( data );
	(ddata)->LINE_CND[ SKBN_PN ].INP_sg = TRUE;

	if( (ddata)->LINE_CND[ZEI_PN].INP_sg ) {
		if( pDBsy->IsTaikaData( pDBzm->dbdata) == 2 ){
			(ddata)->_XXDTA.zei_flag |= 0x10;
		}
	}
	/* ﾃｷﾖｳ */
	if( ! data->m_tekiyo.IsEmpty() )
	{
		if( (uchar)data->m_dsign[0] & 0x80 )
		{
			(ddata)->LINE_CND[ TKY_PN ].IMG_sg = 1;

		//	if( IMG_master != ERR )
		//		memmove( &(ddata)->IMGINFO, data->_TKS, sizeof(DAIMG));
		//	strcpy( pbuf, ""/*Image*/ );

			ddata->_XXDTA.dd_tek.tekiyo.Empty();
		}
		else {
			(ddata)->LINE_CND[ TKY_PN ].IMG_sg = 0;

			i = get_inpl( (unsigned char *)(LPCTSTR)data->m_tekiyo, Voln1->tk_ln );	// 02.09 /02 -ANKINP-

			ddata->_XXDTA.dd_tek.tekiyo = data->m_tekiyo;
			if( i ) {
				LPSTR p;
				int lngth = ddata->_XXDTA.dd_tek.tekiyo.GetLength();

				if( lngth > i ) {
					p = ddata->_XXDTA.dd_tek.tekiyo.GetBuffer( lngth + 10 );
					*(p + i) = '\0';
					ddata->_XXDTA.dd_tek.tekiyo.ReleaseBuffer();
				}
			}

		}
		(ddata)->LINE_CND[ TKY_PN ].INP_sg = TRUE;
	}

	return pn;
}
#endif


//-------------------------------------
// その他の表示を含む科目データセット
//-------------------------------------
void set_kamoku( CDBINPDataRec *data, struct _DATA_LINE *ddata, int pn, BOOL bEdaDisp/* = TRUE*/ )
{
/*unsigned short itm, brn, tmp*/;
int		/*itm,*/ brn;
CString kmk_code;
struct _DBKNREC	*pKn;
	
	if( pn == DEBT_PN || pn == CRED_PN )
	{
		if( pn == DEBT_PN )
		{
			if( pKn = pDBzm->DB_PjisToKnrec( data->m_dbt, TRUE ) ) {
				KamokuString( pKn, ddata->_XXDTA.dd_deb_name );
			}
			else
				ddata->_XXDTA.dd_deb_name.Empty();

			ddata->_XXDTA.dd_dbt = data->m_dbt;
			ddata->_XXDTA.dd_dbr = data->m_dbr;
			brn = data->m_dbr;
		}
		else
		{
			if( pKn = pDBzm->DB_PjisToKnrec( data->m_cre, TRUE ) ) {
				KamokuString( pKn, ddata->_XXDTA.dd_cre_name );
			}
			else
				ddata->_XXDTA.dd_cre_name.Empty();

			ddata->_XXDTA.dd_cre = data->m_cre;
			ddata->_XXDTA.dd_cbr = data->m_cbr;
			brn = data->m_cbr;
		}
		if( bEdaDisp ) {
			// 資金繰諸口枝番
			if( pn == DEBT_PN ) {
				_set_etcdsp( ddata, pn, ddata->LINE_CND );
			}
			else {
				_set_etcdsp( ddata, pn, ddata->LINE_CND ); 
			}
		}
		// 入力済みサインON
		ddata->LINE_CND[ pn ].INP_sg = TRUE;

		// 枝番入力タイプセット
		ddata->LINE_CND[ pn ].INP_type = brn_dfltinptype( brn );
	}
}


//-------------------------------------------------------------------
//　その他科目名称 表示項目コントロールデータレコードをセット
//-------------------------------------------------------------------
void _set_etcdsp( struct _DATA_LINE* dtline, int pn, struct _LINE_CND* lcnd, BOOL full/*=TRUE*/ )
{
CString *etc;
struct _SET_KMKETC	sk;

	switch( pn ) {
	case DEBT_PN:
		sk.bmn	= dtline->_XXDTA.dd_dbmn;
		sk.code	= dtline->_XXDTA.dd_dbt;
		sk.brn	= dtline->_XXDTA.dd_dbr;
		etc = &dtline->_XXDTA.dd_deb_etc;
		break;
	case CRED_PN:
		sk.bmn	= dtline->_XXDTA.dd_cbmn;
		sk.code	= dtline->_XXDTA.dd_cre;
		sk.brn	= dtline->_XXDTA.dd_cbr;
		etc = &dtline->_XXDTA.dd_cre_etc;
		break;
	default:
		ASSERT( FALSE );
		return;
	}

	if( _set_kmketc( &sk, full ) ) {
		etc->Format( "%s", sk.etc );
		dtline->_XXDTA.dd_ctbl[pn].text = sk.etc_col;
		lcnd[pn].ETC_TXT_sg = TRUE;
		lcnd[pn].ETC_TXT_ForeCOL = sk.etc_col;		// 文字色
//		if( obj->IP_CdNum == SYOGT )	// 資金繰諸口枝番
//			cnd->ETC_TXT_ForeCOL = SYOGBR_ForeCOL;	// 文字色
//		else							// 枝番摘要
//			cnd->ETC_TXT_ForeCOL = BRTEK_ForeCOL;	// 文字色
	}
	else {
		etc->Empty();
		dtline->_XXDTA.dd_ctbl[pn].text = (DWORD)-1;
		lcnd[pn].ETC_TXT_sg = FALSE;
	}
}


//-------------------------------------------------------------------
//　追加名称を取得
//	return TRUE ... 追加名称あり, FALSE ... 追加名称なし
//
//-------------------------------------------------------------------
BOOL _set_kmketc( struct _SET_KMKETC* psk, BOOL full/*=TRUE*/ )
{
	BOOL bRet = FALSE;	
	BRN_NAME *p;
//MyTrace( "@_set_kmketc pAUTOSEL->BMNTKBR_OPT(%02x) bmn = %d, code = %s, brn = %06d\n", pAUTOSEL->BMNTKBR_OPT, psk->bmn,  psk->code, psk->brn );

	// 摘要かな検索 をしていたら、元に戻す
	if( pDBzm->tkrec_kanamode() ) {
	//	pDBzm->tkrec->Requery( "", 0 );
	//	pDBzm->tkrec_kanamode() = FALSE;
		tkrec_queryAll();
	}

	int bmncd = -1;
	if( pAUTOSEL->BMNTKBR_OPT == OPT_CHKON ) {
		bmncd = psk->bmn;
		if( ! BmChk.BmnHaniCheck( bmncd, psk->code ) ) {
			bmncd = -1;
		}
	}

	if( p = m_BrTek.get_brtekmsg( bmncd/*psk->bmn*/, psk->code, psk->brn, full, _GetBrTekMode() ) )
	{
		bRet = TRUE;
		psk->etc.Format( "%s", p->Name );
		psk->etc_col = p->ForeColor;
	}
//MyTrace( "@_set_kmketc bmn = %d, code = %s, brn = %06d, etc = %s\n",  bmncd, psk->code, psk->brn, psk->etc );

	return bRet;
}


//--------------------------------
// その他項目　表示
//--------------------------------
BOOL _dsp_etc( CWnd *pwnd, struct _ItemData *data, struct _LINE_CND *cnd, struct _InpCtl *ctl, double x_rate, BOOL dspsw, BOOL focusoff_disp )
{
CWnd *itemorg;
CWnd *etcorg;
CWnd *prevorg;
WINDOWPLACEMENT itemplace;
RECT newrect;
LONG height, wd, cnt;
int sc;
DISPDATA dispdata = {0};
CSize sz;

#ifdef _DEBUG
if( ctl == NULL )	return FALSE;
#endif

	VERIFY(etcorg = pwnd->GetDlgItem( ctl->IDC_X2 ));
	for( int i = 1; ;i++ ) {
		if( (ctl-i)->IDC_X != -1 ) {
			VERIFY(prevorg = pwnd->GetDlgItem( (ctl-i)->IDC_X ));
			break;
		}
	}
//	VERIFY(prevorg = pwnd->GetDlgItem( (ctl-1)->IDC_X ));
	VERIFY(itemorg = pwnd->GetDlgItem( ctl->IDC_X ));

	dispdata.xpos = 3;
	dispdata.ypos = 3;
	dispdata.str = data->ETC_TXT;
	((CICSDisp*)etcorg)->ClearDisp();

	if( cnd->ETC_TXT_sg && dspsw )
	{
		// 枝番摘要表示
		itemorg->GetWindowPlacement( &itemplace );

		CClientDC dc( etcorg );
		CSize sz, charsz;
#ifndef TEST_CLOSE
		COleFont of;
		of = ((CICSDisp*)etcorg)->GetFont();
		CY cy;
		cy = of.GetSize();
		CString fontname = of.GetName();

		CFont font, *pOldFont;
		font.CreatePointFont( (cy.int64/1000), fontname );
		pOldFont = dc.SelectObject( &font );
#endif
		size_t n = strlen(data->ETC_TXT);
		if( n < 20 )	n = 21;
		else			n++;

		sz		= dc.GetTextExtent( data->ETC_TXT, n );
		charsz	= dc.GetTextExtent( _T("1"), 1 );

		// その他表示欄を科目表示欄の上に
		itemorg->GetWindowPlacement( &itemplace );
		newrect = itemplace.rcNormalPosition;

		wd = newrect.right - newrect.left;
		cnt = (wd / charsz.cx);

		int mrgn = (sz.cx / n);
		sz.cx += (mrgn/2);

		dc.SelectObject( pOldFont );

		if( pwnd->GetFocus() == itemorg )
		{
			height = (newrect.bottom - newrect.top);
			height += (height / 5);
			newrect.bottom = newrect.top /*+ vh*/;
			newrect.top = newrect.bottom - height;

			etcorg->SetWindowPos( &pwnd->wndTop, newrect.left, newrect.top, sz.cx, newrect.bottom - newrect.top, SWP_NOACTIVATE|SWP_SHOWWINDOW );
//			etcorg->SetWindowPos( &pwnd->wndTop, newrect.left, newrect.top - height, wd, height, SWP_NOACTIVATE );

			dispdata.backcl = BRTEK_BackCOL;
			dispdata.forecl = cnd->ETC_TXT_ForeCOL;		// 文字色;
			((CICSDisp*)etcorg)->DotDisp( (LPUNKNOWN)(&dispdata) );

			return TRUE;
		}
		else if( focusoff_disp == TRUE )
		{
			sc = get_strcnt( (char*)(LPCTSTR)data->ETC_TXT, cnt );
			char bf[128];
			memcpy( bf, data->ETC_TXT, sc );
			bf[sc] = '\0';

			etcorg->SetWindowPos( &pwnd->wndTop, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_NOACTIVATE|SWP_SHOWWINDOW  );

			dispdata.backcl = BRTEK_BackCOL;
			dispdata.forecl = cnd->ETC_TXT_ForeCOL;		// 文字色;
			((CICSDisp*)etcorg)->DotDisp( (LPUNKNOWN)(&dispdata) );

			return TRUE;
		}
		else
		{
			// その他表示欄を最小に
			etcorg->SetWindowPos( prevorg, 0, 0, 0, 0, SWP_NOACTIVATE|SWP_NOMOVE );
			return FALSE;
		}
	}
	else
	{
		// その他表示欄を最小に
		etcorg->SetWindowPos( prevorg, 0, 0, 0, 0, SWP_NOACTIVATE|SWP_NOMOVE );
		return FALSE;
	}
}



//-----------------------------
//	その他項目　表示
//-----------------------------
void _dsp_etcwnd( struct _SET_KMKETC* sk, CWnd *pwnd, ICSStatic* pst, UINT nID, BOOL focusoff_disp )
{
CWnd *itemorg;
WINDOWPLACEMENT itemplace;
RECT newrect;
int wd, height, cnt, sc;

	VERIFY(itemorg = pwnd->GetDlgItem( nID ));

	// その他の名称がある
	if( ! sk->etc.IsEmpty() ) {
//		pst->SetBackColor( RGB_WHITE );
//		pst->SetTextColor( sk->etc_col );

		CClientDC dc( pst );

		CFont* pft, *pOldFont;
		pft = pst->GetFont();
		pOldFont = dc.SelectObject( pft );
	//	LOGFONT lf;
	//	pft->GetLogFont( &lf );

		CSize sz, charsz;

		sz		= dc.GetTextExtent( sk->etc, sk->etc.GetLength() );
		charsz	= dc.GetTextExtent( _T("1"), 1 );

		itemorg->GetWindowPlacement( &itemplace );
		// その他表示欄を科目表示欄の上に
		itemorg->GetWindowPlacement( &itemplace );
		newrect = itemplace.rcNormalPosition;

		wd = newrect.right - newrect.left;
		cnt = (wd / charsz.cx);

		if( pwnd->GetFocus() == itemorg )
		{
			height = (newrect.bottom - newrect.top);
			newrect.bottom = newrect.top /*+ vh*/;
			newrect.top = newrect.bottom - height;

			pst->UpdateCaption( sk->etc, NULL );
			pst->SetWindowPos( &pwnd->wndTop, newrect.left, newrect.top, sz.cx, newrect.bottom - newrect.top, SWP_NOACTIVATE|SWP_SHOWWINDOW );
		}
		else if( focusoff_disp == TRUE )
		{
			sc = get_strcnt( (char*)(LPCTSTR)sk->etc, cnt );
			char bf[128];
			memcpy( bf, sk->etc, sc );
			bf[sc] = '\0';
			pst->UpdateCaption( bf, NULL );
			pst->SetWindowPos( &pwnd->wndTop, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_NOACTIVATE|SWP_SHOWWINDOW  );
		}
		else
		{
			// その他表示欄を最小に
			pst->ShowWindow( SW_HIDE );
		//	pst->SetWindowPos( itemorg, 0, 0, 0, 0, SWP_NOACTIVATE|SWP_NOMOVE );
		}
		// 08.18 /11
		pst->SetBackColor( RGB_WHITE );
		pst->SetTextColor( sk->etc_col );
	}
	else {
		// その他表示欄を最小に
		pst->ShowWindow( SW_HIDE );
	//	pst->SetWindowPos( itemorg, 0, 0, 0, 0, SWP_NOACTIVATE|SWP_NOMOVE );
	}

}

// 08.18 /11
void _clr_kmketc( struct _SET_KMKETC* psk )
{
	psk->bmn = -1;
	psk->kno.Empty();
	psk->code.Empty();
	psk->brn = -1;
	psk->etc.Empty();
	psk->etc_col = RGB_BLACK;
}


#ifdef _SPEED_FIX_1207_07_
/*------------------------------
	現金貸借と諸口貸借を計算
	[12.07 /07]
  ------------------------------*/
int calq_gstotal( int zanread )
{
	if( zanread ) {
		if( pDBzm->pKzrec != NULL )	pDBzm->KamokuZanFree();
		pDBzm->KamokuZanRead();
	}

	/* 諸口合計	*/
	g_stotal( SYOGT, (char *)SYOG_d, (char *)SYOG_c, (char *)SG_def );

	/* 現金合計	*/
	g_stotal( /*GNKIN*/pAUTOSEL->DCKM_CODE, (char *)GENK_d, (char *)GENK_c, (char *)GK_def );

	return 0;
}
#endif

/*--------------------------------
	Total, calucrate
  --------------------------------*/
int g_stotal( LPCTSTR KMCD, char *tsgd, char *tsgc, char *zan )
{
	unsigned int	search_code;
#ifdef _SPEED_FIX_1207_07_
	struct _DBKZREC		mrec;
#else
	struct _DIP_KZREC	mrec;
#endif
	int		i;
	char	val[6] = {0};

	_fillbuf( tsgd, _BIN_LNG, 0 );
	_fillbuf( tsgc, _BIN_LNG, 0 );
	
	CString strCode;
	strCode = KMCD;
	search_code = pjsksrc( strCode );

#ifndef _SPEED_FIX_1207_07_
	if( pDBzm->kzrec_Requery() ) {
		pDBzm->kzrec->Requery("");
		pDBzm->kzrec_Requery() = FALSE;
	}
#endif

	if( search_code )
	{
		l_defn(_BIN);	// 07.16 /02 ... この位置に移動

#ifdef _SPEED_FIX_1207_07_
		if( zrecget2X( strCode, &mrec ))
#else
		if( zrecget2( strCode, &mrec ))
#endif
		{
//			ermset( ERROR_STOP, 0 );
			return(-1);
		}

//		l_defn(_BIN);

		// 未払金等など貸方科目の場合は、反転
#ifdef _SPEED_FIX_1207_07_
		memcpy( val, mrec.kzzan, _BIN_LNG );
		if( (mrec.kzatt & 0x01) )
			l_neg( val );

		l_add( tsgd, tsgd, val );

		for( i = 0; i < 16; i++ )
		{
			l_add( tsgd, tsgd, (char *)&mrec.kzval[i].Kari );
			l_add( tsgc, tsgc, (char *)&mrec.kzval[i].Kasi );
		}
#else
		memcpy( val, mrec.kz_zan, _BIN_LNG );
		if( (mrec.kz_att & 0x01) )
			l_neg( val );

		l_add( tsgd, tsgd, val );

		for( i = 0; i < 16; i++ )
		{
			l_add( tsgd, tsgd, (char *)&mrec.kz_kari[i] );
			l_add( tsgc, tsgc, (char *)&mrec.kz_kasi[i] );
		}
#endif
	}

	l_sub( zan, tsgd, tsgc );

	// 未払金等など貸方科目の場合は、反転
#ifdef _SPEED_FIX_1207_07_
	if( (mrec.kzatt & 0x01) )	l_neg( zan );
#else
	if( (mrec.kz_att & 0x01) )	l_neg( zan );
#endif

	return(0);
	
}

/* 諸口貸借表示	*/
void syogdsp( char *buff )
{
	p_DBDinpView->syog_dsp( buff );
}


/* 現金貸借表示	*/
void genkdsp( char *buff )
{
	p_DBDinpView->genk_dsp( buff );
}



/**************************************

		TkNamTrimRight()

	摘要名称後ろスペースカット

***************************************/
void TkNamTrimRight( CDBipTKREC *tek )
{
	const int max_ln = 50;
	LPSTR  p = tek->m_tkname.GetBuffer( max_ln );
	int n;
	int max = tek->m_tkname.GetLength();
	int def;
	if( max > max_ln )	max = max_ln;

	n = kjlen( p, max );
	def = max - n * 2;
	if( def > 0 )
		memset( p + (n * 2), '\0', def );
	tek->m_tkname.ReleaseBuffer();

}

/*ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ
	ｶﾝｼﾞ ﾆｮｳﾘｮｸ ﾓｼﾞ ｽｳ -- ｲｾﾞﾝ
	02.09 /02 ﾖﾘ 入力ﾊﾞｲﾄ数 ｦ ﾍﾝｿｳ
ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ*/
int get_inpl( uchar *buff, int lngs )
{
#ifdef CLOSE // 02.09 /02 -ANKINP-
	buff = buff + (lngs - 1) * 2;

	for( ; lngs > 0; lngs--, buff--, buff-- ) {
		if( *buff == 0x81 && *(buff + 1) == 0x40 ) continue;
		if( *buff == 0 || *(buff + 1) == 0 ) continue;
		break;
	}
	return( lngs );

#endif
	int ln;
	lngs *= 2;	// 漢字文字数よりバイト数

	ln = kjlen( buff, lngs );

	return ln;
}

int get_tklen( unsigned char *p )
{
	char	tkbf[22];

	memmove( tkbf, (char *)p, 20 );
	tkbf[20] = 0;

	return( strlen( tkbf ) / 2 );

}

//
// 金額表示
static char	FMT18[] = "SS,SSS,SSS,SSS,SS9";
char *val_txt( char *txt, int txtsiz, char *bin, int t_l, char *fronttxt, char *fmt, char *endtxt )
{
	int l_r;
	char *pfmt;
	int n;

	if( t_l )
	{
		l_r = l_retn();
		l_defn( t_l );
	}
	
	n = 0;
	if( fronttxt )
		n = sprintf_s( txt, txtsiz, "%s", fronttxt );

	pfmt = fmt ? fmt : FMT18;
	l_print ( txt+n, bin, pfmt );
	*(txt+n+strlen(pfmt)) = '\0';

	if( endtxt )
		strcat_s( txt, txtsiz, endtxt );

	if( t_l )
	{
		l_defn( t_l );
	}

	return txt;
}

void val_disp( CWnd *pwnd, char *bin, OLE_COLOR def_fore, int t_l, char *fronttxt, char *fmt, char *endtxt )
{
	char txt[128];

	// 表示
	pwnd->SetWindowText( val_txt( txt, sizeof txt, bin, t_l, fronttxt, fmt, endtxt ) );

	// 文字色
	((CICSDisp*)pwnd)->SetForeColor( l_test( bin ) < 0 ? RGB_RED : def_fore );
}

/*****************************************************
	現在のビュー拡大比率に従って表示サイズを変える
******************************************************/
void kakudai(CWnd* wnd)
{
	RECT	newrect;
	double rate_w, rate_h;

	get_viewrate( rate_w, rate_h );	//ビューサイズ比率をゲット
	wnd->GetWindowRect( &newrect );
	newrect.bottom = newrect.top + ((newrect.bottom - newrect.top ) * rate_h);
	newrect.right = newrect.left + ((newrect.right - newrect.left ) * rate_w);

	wnd->SetWindowPos( 0, newrect.left, newrect.top, newrect.right - newrect.left, newrect.bottom - newrect.top, SWP_NOMOVE|SWP_NOZORDER );
}


/*-----------------------------------------------
	02.09 /02
	指定バイトのうち、文字列として正しい
	バイト数を返送します
  -----------------------------------------------*/
int get_strcnt(char* str, int byte)
{
	int k, st, btcnt;

	btcnt = 0;

	for(k = 0; k < byte && *(str+k) != 0; ) {

		st = IsKindOfLetter( (LPCTSTR)str, k );

		switch( st ) {
		case 1:		// ASCII
			k++;	
			if( k <= byte ) btcnt += 1;
			break;
		case 2:		// MB文字1バイト目
			k += 2;
			if( k <= byte ) btcnt += 2;
			break;
		default:
			return 0;
		}

	}

	return btcnt;
}



/*-----------------------------------------------
	08.19 /02
	部門・工事名称のその他表示
  -----------------------------------------------*/
void _set_bmndsp( struct _DATA_LINE *line, int pn, BOOL full /*= TRUE*/ )
{
int bmn;
CString koji, *etc;

	switch( pn ) {
	case BMON_PN:
		bmn = line->_XXDTA.dd_dbmn;
		etc = &line->_XXDTA.dd_dbmn_etc;
		break;
	case CBMN_PN:
		bmn = line->_XXDTA.dd_cbmn;
		etc = &line->_XXDTA.dd_cbmn_etc;
		break;
	case KOJI_PN:
		koji = line->_XXDTA.dd_dkno;
		etc = &line->_XXDTA.dd_dkno_etc;
		bmn = -1;
		break;
	case CKJI_PN:
		koji = line->_XXDTA.dd_ckno;
		etc = &line->_XXDTA.dd_ckno_etc;
		bmn = -1;
		break;
	default:
		ASSERT( FALSE );
		return;
	}

	struct _SET_KMKETC sk;
	BOOL bDisp;
	bDisp = ((pAUTOSEL->BMNDSP_OPT == OPT_BMNDSP) ? TRUE : FALSE);

	if( pn == BMON_PN || pn == CBMN_PN ) {
		// 部門
		sk.bmn = bmn;

		if( bDisp && _set_bmnetc( &sk ) ) {
			etc->Format( "%s", sk.etc );
			line->LINE_CND[pn].ETC_TXT_sg = TRUE;
			line->LINE_CND[pn].ETC_TXT_ForeCOL = sk.etc_col;	// 文字色
			line->_XXDTA.dd_ctbl[pn].text = sk.etc_col;
		}
		else {
			etc->Empty();
			line->LINE_CND[pn].ETC_TXT_sg = FALSE;
			line->_XXDTA.dd_ctbl[pn].text = (DWORD)-1;
		}
	}
	else {
		// 工事番号
		koji.TrimRight();
		sk.kno = koji;

		if( bDisp && _set_kojietc( &sk ) ) {
			etc->Format( "%s", sk.etc );
			line->LINE_CND[pn].ETC_TXT_sg = TRUE;
			line->LINE_CND[pn].ETC_TXT_ForeCOL = sk.etc_col;	// 文字色
			line->_XXDTA.dd_ctbl[pn].text = sk.etc_col;
		}
		else {
			etc->Empty();
			line->LINE_CND[pn].ETC_TXT_sg = FALSE;
			line->_XXDTA.dd_ctbl[pn].text = (DWORD)-1;
		}
	}
}

//------------------------------------------------------------------------
//	部門名称を取得
//	struct _SET_KMKETC *sk
//				
//------------------------------------------------------------------------
BOOL _set_bmnetc( struct _SET_KMKETC *sk )
{
	BOOL ret = FALSE;

	if( ! pDBzm->bmname )
		return FALSE;

	pDBzm->bmname->MoveFirst();

	do {
		if( sk->bmn == pDBzm->bmname->bnbcd ) {
			if( IsSyafuKaisei( Voln1 ) && pDBzm->IsRegisterNphw() ) {
				sk->etc = pDBzm->bmname->bnbnm;

				if( !sk->etc.IsEmpty() ) {
					if( kjlen( (void*)(LPCTSTR)sk->etc, sk->etc.GetLength() ) ) {
						sk->etc_col = RGB_BLUE;
						ret = TRUE;
					}
				}
			}
			else {
				if( ! (pDBzm->bmname->bnhsg[0] & 0x02) ) {
					sk->etc = pDBzm->bmname->bnbnm;

					if( !sk->etc.IsEmpty() ) {
						if( kjlen( (void*)(LPCTSTR)sk->etc, sk->etc.GetLength() ) ) {
							sk->etc_col = RGB_BLUE;
							ret = TRUE;
						}
					}
				}
			}
			break;
		}
	} while( pDBzm->bmname->MoveNext() == 0 );

	return ret;
}

//--------------------------------------------------------------
//	工事名称を取得
//
//--------------------------------------------------------------
BOOL _set_kojietc( struct _SET_KMKETC *sk )
{
	BOOL ret = FALSE;

	if( p_DBDinpView->m_Seldata.GetKojiName( sk->kno, sk->etc, FALSE ) ) {
		sk->etc_col = RGB_BLUE;
		ret = TRUE;
	}

	return ret;
}

//--------------------------------------------------------------
//	変動事由コードエリアより 名称を取得
//	[マスターによって 変動事由名称か、振替科目区分か]
//
//--------------------------------------------------------------
BOOL _get_hjiyustr( int hjcd, int nbcd, CString& str )
{
	BOOL ret = FALSE;

	if( pDBzm->zvol->apno < 0x50 ) {
		str.Empty();
		pDBzm->get_hjiyustr( hjcd, str );
		if( ! str.IsEmpty() )	ret = TRUE;
	}
	else {
		if( ! IsSyafuKaisei( pDBzm->zvol ) ) {
			if( p_DBDinpView->m_Seldata.GetNPfuriStr( hjcd, str ) ) {
				ret = TRUE;
			}
		}
		else {
			if( p_DBDinpView->m_Seldata.GetNaibuString( hjcd, nbcd, str ) ) {
				ret = TRUE;
			}
		}
	}

	return ret;
}

// 振替科目 検索用 文字列
void NPfuri_Query( int npcd, CString& qstr )
{
	char	tmp[64];

	if( pDBzm->zvol->apno == 0x50 || pDBzm->zvol->apno == 0x51 ) {
		DWORD scod, ecod;
		p_DBDinpView->m_Seldata.NPfuriHani( scod, ecod );

		qstr.Format( "((dbt >= '%08x' and dbt <= '%08x') or (cre >= '%08x' and cre <= '%08x')) ", 
							scod, ecod, scod, ecod );

		if( npcd == HJIYU_MITOROK ) {	// 振替科目未登録
			// 振替科目テーブルに登録されていて有効なコードか
			strcpy_s( tmp, sizeof tmp, "AND hjiyu NOT IN (select npscno from npsszrc)" );
			qstr += tmp;
		}
		else {
			if( npcd != HJIYU_TAISYO ) {	//振替科目対象仕訳
				//各振替科目に合致するものみ
				sprintf_s( tmp, sizeof tmp, "AND hjiyu = %d", npcd );
				qstr += tmp;
			}
		}
	}
	else if( IsSyafuKaisei( pDBzm->zvol ) ) {
#ifdef NAIBU_VER2
		if( npcd == 100 ) {		// 内部取引・該当区分
			qstr = "hjiyu = 0 and ((SUBSTRING( dbt, 1, 6 ) =ANY(select substring(gkcod,1,6) from gkrec where (cast(gksgn as int)&0x80) = 0x80)) or\
 (SUBSTRING( cre, 1, 6 ) =ANY(select substring(gkcod,1,6) from gkrec where (cast(gksgn as int)&0x80) = 0x80)))";
		}
		else if( npcd == 101 ) {	// 内部取引・該当区分
			strcpy_s( tmp, sizeof tmp, "hjiyu = 1" );
			qstr += tmp;
		}
		else if( npcd == 102 ) {
			// 内部取引・相手部門
			strcpy_s( tmp, sizeof tmp, "hjiyu = 2" );
			qstr += tmp;
		}
#else
		if( npcd == HJIYU_MITOROK ) {		// 内部取引・未登録
			qstr = "hjiyu = 0 and ((SUBSTRING( dbt, 1, 6 ) =ANY(select substring(gkcod,1,6) from gkrec where (cast(gksgn as int)&0x80) = 0x80)) or\
 (SUBSTRING( cre, 1, 6 ) =ANY(select substring(gkcod,1,6) from gkrec where (cast(gksgn as int)&0x80) = 0x80)))";
		}
		else if( npcd == HJIYU_TAISYO ) {	// 内部取引・対象仕訳
			qstr = "((SUBSTRING( dbt, 1, 6 ) =ANY(select substring(gkcod,1,6) from gkrec where (cast(gksgn as int)&0x80) = 0x80)) or\
 (SUBSTRING( cre, 1, 6 ) =ANY(select substring(gkcod,1,6) from gkrec where (cast(gksgn as int)&0x80) = 0x80)))";
		}
		else {
			// 内部取引 各区分
			sprintf_s( tmp, sizeof tmp, "hjiyu = %d", (npcd-110) );
			qstr = tmp;
		}
#endif
	}
	else if( IsGakkouH27Master( pDBzm ) ) {
		if( npcd == HJIYU_MITOROK ) {		// 資金調整・未登録
			qstr = "hjiyu = 0 and ((SUBSTRING( dbt, 1, 6 ) =ANY(select substring(npkncd,1,6) from npknrec where (cast(npksg8 as int)&0x01) = 0x01)) or\
 (SUBSTRING( cre, 1, 6 ) =ANY(select substring(npkncd,1,6) from npknrec where (cast(npksg8 as int)&0x01) = 0x01)))";
		}
		else if( npcd == HJIYU_TAISYO ) {	// 資金調整・対象仕訳
			qstr = "((SUBSTRING( dbt, 1, 6 ) =ANY(select substring(npkncd,1,6) from npknrec where (cast(npksg8 as int)&0x01) = 0x01)) or\
 (SUBSTRING( cre, 1, 6 ) =ANY(select substring(npkncd,1,6) from npknrec where (cast(npksg8 as int)&0x01) = 0x01)))";
		}
		else {
			// 資金調整 各区分
			sprintf_s( tmp, sizeof tmp, "hjiyu = %d", npcd );
			qstr = tmp;
		}
	}
}


COLORREF PixelAlpha(COLORREF clrSrc, double src_darken, COLORREF clrDest, double dest_darken)
{
	return RGB (GetRValue (clrSrc) * src_darken + GetRValue (clrDest) * dest_darken, 
				GetGValue (clrSrc) * src_darken + GetGValue (clrDest) * dest_darken, 
				GetBValue (clrSrc) * src_darken + GetBValue (clrDest) * dest_darken);
	
} //End PixelAlpha


#ifdef CUT_CLOSE

/*=============================================
		自動仕訳の摘要 サーチ 12.09 /02
  =============================================*/
int tkat_srch( short flg, CString deb, CString cre, unsigned short *tkr )
{
	int tkput = 0;
	CString filter;
	char buf[30], buf2[30], tandeb[30] = {0}, tancre[30] = {0};
	struct _DBKNREC* pKn;

	if( deb.IsEmpty() )	strcpy_s( buf, sizeof buf, "stdbt IS NULL" );
	else {
		sprintf_s( buf, sizeof buf, "stdbt = '%s'", deb );

		if( pKn = pDBzm->DB_PjisToKnrec( deb ) ) {
			if( !(pKn->knatt & 0x01) )
				sprintf_s( tandeb, sizeof tandeb, " OR stdbt = '%s'", deb );
		}
	}

	if( deb.IsEmpty() )	strcpy_s( buf2, sizeof buf2, "stcre IS NULL" );
	else {
		sprintf_s( buf2, sizeof buf2, "stcre = '%s'", cre );
		if( pKn = pDBzm->DB_PjisToKnrec( cre ) ) {
			if( (pKn->knatt & 0x01) )
				sprintf_s( tancre, sizeof tancre, " OR stcre = '%s'", cre );
		}
	}

	filter.Format( "(%s AND %s)", buf, buf2 );
	if( tandeb[0] )	filter += tandeb;
	if( tancre[0] ) filter += tancre;

	if( pDBzm->strec->Requery( filter ) == 0 ) {
		tkput = pDBzm->strec->GetRecordCount();
	}

	return tkput;
}


/*=============================================
	自動仕訳摘要を 選択ﾊﾞｯﾌｧ に ｾｯﾄ 12.09 /02
  =============================================*/
void tkauto_dsp( int pg, int cnt )
{
	int		i, n, tkln, l;
	CDBipTKREC	tekiyo;
	int selpnt;
	char temp[40];
	CString tmp;

	fixclr();
	p_DBDinpView->set_valtitle( "《仕訳対応摘要》" );

	n = selpnt = 0;

	pDBzm->strec->MoveFirst();
	::ZeroMemory( TKSEL_PG, sizeof TKSEL_PG );
	i = 0;
	do {
		if( pDBzm->strec->st != 0 )	break;

		if( ( AC_MAX * (pg-1) ) > i++ )
			continue;

		if( pDBzm->DB_TekiyoNoRead( &tekiyo, pDBzm->strec->stcod ) == 0 ) {
			tmp = pDBzm->tkrec->tkname;
			tkln = tmp.GetLength();
			memcpy(temp, tmp, tkln);
			temp[tkln] = '\0';
			l = get_strcnt( (char*)(LPCTSTR)tmp, tkln );
			temp[l] = '\0';

			TKSEL_PG[selpnt] = pDBzm->tkrec->tkcod;

			SelVal32[selpnt].number = selpnt+(isEntrySys() ? 9 : 1);
			SelVal32[selpnt++].Data = temp;

			if( ++n >= AC_MAX )
				break;
			//	i++;
		}
	} while( pDBzm->strec->MoveNext() == 0 );

	if( selpnt < AC_MAX ) {
		for( i = selpnt; i < AC_MAX; i++ ) {
			SelVal32[i].number = i+(isEntrySys() ? 9 : 1);
			SelVal32[i].Data = "";
		}
	}
}


#endif


//
// 枝番一時預かり
static int save_brn = -1;

void set_brn( int brn )
{
	save_brn = brn;
}

int get_brn()
{
	return save_brn;
}


// 科目名称文字列 作成
void KamokuString( DBKNREC* pKn, CString& string )
{
	KamokuString( pKn, string.GetBuffer(64), 64 );
	string.ReleaseBuffer();
}

void KamokuString( DBKNREC* pKn, char* buf, int bufsize )
{
	if( pAUTOSEL->SCNCD_OPT == OPT_SCNCD ) {
		kmkcode_string( 2, pKn, buf, bufsize );
	}
	else {
		sprintf_s( buf, bufsize, "%.14s", pKn->knnam );
	}
}


//CSV 文字列チェック

int csvstr_check( char* wk, int wksize, LPCTSTR bf, int valsg/*=0*/ )
{
	int k, st, dcnt, i;

	i = dcnt = 0;

	for(k = 0; *(bf+k) != 0; ) {
		st = IsKindOfLetter( (LPCTSTR)bf, k );
		switch( st ) {
		case 1:		// ASCII
			if( isdigit( *(bf+k) ) ) {	//数字
				*(wk + i++) = *(bf+k);
				k++;
				dcnt++;
			}
			else if( *(bf+k) == ' ' ) {
				k++;
				continue;
			}
			else {
				// 金額アイテム時はマイナスも許可
				if( valsg ) {
					if( *(bf+k) == '-' ) {
						*(wk + i++) = *(bf+k);
						k++;
						dcnt++;
						continue;
					}
				}
				return 0;
			}
			break;
		case 2:		// MB文字1バイト目
			return 0;
			break;
		default:
			return 0;
		}
	}

	*(wk+i) = '\0';

	return dcnt;
}


// 個別対応の科目か？
BOOL IsKobetsuKamoku( DBKNREC* pKn, BYTE dsgn2 )
{
	BOOL bKmChk = FALSE;

	/* 仕入 */
	if( (pKn->knsgn[0]&0xff) == 0x04 )
		bKmChk = TRUE;

	/* 経費、営業外費用、特別損失 */
	if( (pKn->knsgn[0]&0xff) == (BYTE)0x08 ) {
		if( (pKn->knsgn[1]&0xff) == 0 || (pKn->knsgn[1]&0xff) == 0x01 || (pKn->knsgn[1]&0xff) == 0x02 ) {
			bKmChk = TRUE;
		}
	}
	/* 資産取得 */
	if( (pKn->knsgn[0]&0xff) == (BYTE)0x01 && (pKn->knsgn[1]&0xff) == 0 ) {
		if( (/*data->dsign[2]*/dsgn2 & 0x31) == 0x11 )
			bKmChk = TRUE;
	}
	if( (pKn->knsgn[0]&0xff) == (BYTE)0x11 && (pKn->knsgn[1]&0xff) == 0x02 ) {
		if( (/*data->dsign[2]*/dsgn2 & 0x31) == 0x11 )
			bKmChk = TRUE;
	}

	return bKmChk;
}

//--------------------------------------------------------------
//	社会福祉改正マスターか？
//
//--------------------------------------------------------------
BOOL IsSyafuKaisei( CZVOLUME* zv )
{
	if( IsOKGyosyu( zv->apno, SW_SOCIAL ) && zv->m_ver == 0x11 )
		return TRUE;

	return FALSE;
}


//--------------------------------------------------------------
//	簡易課税 デフォルト区分
//
//--------------------------------------------------------------
BYTE KaniDefaultKubun( CZVOLUME* zv )
{
	BYTE defKbn = 0;
	if( (zv->s_sgn2&0x0f) >= 2 ) {
		defKbn = (zv->s_sgn2 & 0x0f) - 1;
	}

	return defKbn;
}


// 定型仕訳
extern CMKRDSub Mkrd;

// 定型仕訳データリクエリー
int mkrd_data_requery(int lbnum)
{
	CString filter;
	int	owner;

	if( p_DBDinpView->m_CmnId > 0 )	owner = p_DBDinpView->m_CmnId;
	else							owner = 0;

	filter.Format("rd_lbnum = %d AND rd_owner = %d", lbnum, owner);
	Mkrd.data->Requery(filter);

	return -1;
}
