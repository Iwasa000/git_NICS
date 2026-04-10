#include "stdafx.h"

#include "input1.h"
#include "scan.h"
#include "SEQ.h"

#include "BmnChk.h"

#include "Mstrw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern DB_SCANINFO		SCAN_PGTBL;

extern CSEQ		SCMDFY_SEQ;
extern CSEQ		MDFYDATA_SEQ;

extern 
CBmnHaniCheck	BmChk;

extern BOOL		M_MISEIKOJI_TYPE;


#ifdef NAIBU_VER2
extern CDBNpCal* pDBnc;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDipZmSub


CDipZmSub::CDipZmSub()
{
	l_defn(0x16);

#ifdef DB_OLD_CLOSE
	m_BMNSEL_MOD = 0;
	::ZeroMemory( &m_Btb, sizeof(m_Btb));
	::ZeroMemory( &m_BtbKOJI, sizeof(m_BtbKOJI));
	::ZeroMemory( &m_Btb_PAGE, sizeof(m_Btb));
#endif

	m_pKmTBL	= NULL;

	m_DBstart	= -1;
	m_DBend		= -1;

	dbdata_Sort()	= DBDATA_SORT_SEQ;
	dbdata_Job()	= DBDATA_JOB_NORMAL;
	dbdata_Requery()	= FALSE;
	dbdata_Speed()	= FALSE;
	dbdata_Reload()	= FALSE;

	data_Requery()		= FALSE;
	data_speed()		= FALSE;
	kzrec_Requery()		= FALSE;
	tkrec_kanamode()	= FALSE;

	m_database	= NULL;

	m_inpSofs = m_inpEofs = -1;
	m_inpSy = m_inpEy = -1;
	m_lastdata.Reset();

	m_damax = 0;
	m_edmax = 0;
	m_tkmax = 0;

	::ZeroMemory( m_InpBcd, sizeof m_InpBcd );

	IsModify()	= FALSE;
	IsDelScan()	= FALSE;
	IsDupliScan() = FALSE;
	IsWizScan() = FALSE;

	m_Jgykcd = 0;
	m_Knrkcd = 0;

	IsEdaAdd() = FALSE;

	insseq_make()	= FALSE;
	m_insCnt		= 0;

	m_sortMode = 0;

	dbdata_scan_insert() = FALSE;
}


CDipZmSub::~CDipZmSub()
{
	if( m_pKmTBL ) {
		delete [] m_pKmTBL;	m_pKmTBL = NULL;
	}
}


void invmove(char* invbf, int szbf, CString invno)
{
	::ZeroMemory(invbf, szbf);
	int len = invno.GetLength();

	if( len > szbf ) len = szbf;

	if( len > 0 ) {
		memcpy(invbf, invno.GetBuffer(64), len);
		invno.ReleaseBuffer();
	}
}



void CDipZmSub::GetCDBData( CDBINPDataRec* rec, CDBDATA* ptr/*= NULL*/ )
{
	CDBDATA* data;
	
	if( ptr != NULL ) {
		data = ptr;
	}
	else {
		data = dbdata;
	}
	if( data->seq == -1 ) {
		rec->Reset();
	}
	else {
		rec->m_seq	= data->seq;
		rec->m_ksign = data->ksign;
		rec->m_ddate = data->ddate;
		rec->m_mofs	= data->mofs;
		rec->m_cno	= data->cno;
		rec->m_dbmn	= data->dbmn;
		rec->m_dkno	= data->dkno;
		rec->m_dkno.TrimRight();
		rec->m_dbt	= data->dbt;
		rec->m_dbt.TrimRight();
		rec->m_dbr	= data->dbr;

		rec->m_cbmn	= data->cbmn;
		rec->m_ckno	= data->ckno;
		rec->m_ckno.TrimRight();
		rec->m_cre	= data->cre;
		rec->m_cre.TrimRight();
		rec->m_cbr	= data->cbr;

		l_input( rec->m_val, (void*)(LPCTSTR)data->val );
		l_input( rec->m_zei, (void*)(LPCTSTR)data->zei );

		rec->m_dsign.Copy( data->dsign );
		rec->m_owner	= data->owner;
		rec->m_dtype	= data->dtype;

		rec->m_tekiyo	= data->tekiyo;
		rec->m_imgsq	= data->imgsq;
		rec->m_snumber	= data->snumber;
		rec->m_tdate	= data->tdate;
		rec->m_edate	= data->edate;
		rec->m_udate	= data->udate;
		rec->m_icno		= data->icno;

		rec->m_skn		= data->skn;
		rec->m_tag		= data->tag;

		rec->m_hjiyu	= data->hjiyu;
		l_input( rec->m_taika, (void*)(LPCTSTR)data->taika );

		rec->m_nbcd = -1;
	//	sprintf_s(rec->m_invno, sizeof(rec->m_invno), _T("%s"), data->invno);
		invmove(rec->m_invno, sizeof(rec->m_invno), data->invno);

		rec->m_rnumber = data->rnumber;

#ifdef NAIBU_VER2
		if( IsSyafuKaisei( zvol ) ) {
			CString filter;
			filter.Format( "seq = %d", data->seq );
			pDBnc->m_pNbtdarec->RequeryEx( filter );
			if( pDBnc->m_pNbtdarec->st == 0 ) {
				rec->m_nbcd = pDBnc->m_pNbtdarec->m_nbtbmn;
			}
		}
#endif
	}
}


void CDipZmSub::GetCData( CDBINPDataRec* rec )
{
	if( data->seq == -1 ) {
		rec->Reset();
	}
	else {
		rec->m_seq	= data->seq;
		rec->m_ksign = data->ksign;
		rec->m_ddate = data->ddate;
		rec->m_mofs	= data->mofs;
		rec->m_cno	= data->cno;
		rec->m_dbmn	= data->dbmn;
		rec->m_dkno	= data->dkno;
		rec->m_dkno.TrimRight();
		rec->m_dbt	= data->dbt;
		rec->m_dbt.TrimRight();
		rec->m_dbr	= data->dbr;

		rec->m_cbmn	= data->cbmn;
		rec->m_ckno	= data->ckno;
		rec->m_ckno.TrimRight();
		rec->m_cre	= data->cre;
		rec->m_cre.TrimRight();
		rec->m_cbr	= data->cbr;

		l_input( rec->m_val, (void*)(LPCTSTR)data->val );
		l_input( rec->m_zei, (void*)(LPCTSTR)data->zei );

		rec->m_dsign.Copy( data->dsign );
		rec->m_owner	= data->owner;
		rec->m_dtype	= data->dtype;

		rec->m_tekiyo	= data->tekiyo;
		rec->m_imgsq	= data->imgsq;
		rec->m_snumber	= data->snumber;
		rec->m_tdate	= data->tdate;
		rec->m_edate	= data->edate;
		rec->m_udate	= data->udate;
		rec->m_icno		= data->icno;

		rec->m_skn		= data->skn;
		rec->m_tag		= data->tag;

		rec->m_hjiyu	= data->hjiyu;
		l_input( rec->m_taika, (void*)(LPCTSTR)data->taika );

		rec->m_nbcd = -1;

		// インボイス対応
	//	sprintf_s( rec->m_invno, sizeof( rec->m_invno ), _T( "%s" ), data->invno );
		invmove(rec->m_invno, sizeof(rec->m_invno), data->invno);

		rec->m_rnumber = data->rnumber;

#ifdef NAIBU_VER2
		if( IsSyafuKaisei( zvol ) ) {
			CString filter;
			filter.Format( "seq = %d", data->seq );
			pDBnc->m_pNbtdarec->RequeryEx( filter );
			if( pDBnc->m_pNbtdarec->st == 0 ) {
				rec->m_nbcd = pDBnc->m_pNbtdarec->m_nbtbmn;
			}
		}
#endif
	}
}

void CDipZmSub::SetCDBData( CDBINPDataRec* rec )
{
	if( rec->m_seq <= 0 )	return;

	dbdata->seq		= rec->m_seq;
	dbdata->ksign	= rec->m_ksign;
	dbdata->ddate	= rec->m_ddate; 
	dbdata->mofs	= rec->m_mofs;
	dbdata->cno		= rec->m_cno;
	dbdata->dbmn	= rec->m_dbmn;
	dbdata->dkno	= rec->m_dkno;
	dbdata->dbt		= rec->m_dbt;
	dbdata->dbr		= rec->m_dbr;

	dbdata->cbmn	= rec->m_cbmn;
	dbdata->ckno	= rec->m_ckno;
	dbdata->cre		= rec->m_cre;
	dbdata->cbr		= rec->m_cbr;

	l_print( dbdata->val.GetBuffer(100), rec->m_val, "sssssssssss9" );
	dbdata->val.ReleaseBuffer();
	l_print( dbdata->zei.GetBuffer(100), rec->m_zei, "sssssssssss9" );
	dbdata->zei.ReleaseBuffer();

	dbdata->dsign.Copy(	rec->m_dsign );
	dbdata->owner = rec->m_owner;/*DBUserNo()*/;
	dbdata->dtype =	rec->m_dtype;

	dbdata->tekiyo = rec->m_tekiyo;
	dbdata->imgsq = rec->m_imgsq;
	dbdata->snumber = rec->m_snumber;
	dbdata->tdate = rec->m_tdate;
	dbdata->edate = rec->m_edate;
	dbdata->udate = rec->m_udate;
	dbdata->icno	= rec->m_icno;

	dbdata->skn = rec->m_skn;
	dbdata->tag = rec->m_tag;

	dbdata->hjiyu	= rec->m_hjiyu;
	l_print( dbdata->taika.GetBuffer(100), rec->m_taika, "sssssssssss9" );
	dbdata->taika.ReleaseBuffer();
	dbdata->invno = rec->m_invno;
	dbdata->rnumber = rec->m_rnumber;
}

//============================================================
// インボイス対応
//	摘要情報のコピー
//------------------------------------------------------------
// 引数
//		pTkRec			:			取得先
//		rec				:			取得元
//============================================================
void CDipZmSub::SetSyTkrec( _SY_TKREC_ *psyTkRec, CDBipTKREC *rec, CDBINPDataRec* data )
{
	ASSERT( psyTkRec );
	ASSERT( rec );

	if(( psyTkRec == NULL ) || ( rec == NULL )) {
		return;
	}

	memset( psyTkRec, '\0', sizeof( _SY_TKREC_ ));

	psyTkRec->tk_attr = rec->m_tksgn[1];
	for( int i = 0; i < SIZE_DSIGN; i++ ) {
		psyTkRec->tk_dsign[i] = rec->m_tkdsgn[i];
	}

	if( strlen( rec->m_tkinvno ) > 0 ) {
		sprintf_s( psyTkRec->tk_invno, sizeof( psyTkRec->tk_invno ), _T( "T%s" ), rec->m_tkinvno );
	} 
	//免税サイン ON を生かすため 摘要の免税もON
	if( data->m_dsign[6] & 0x20 ) {
		int attr = (psyTkRec->tk_attr & 0x0f);
		if( attr != 2 && attr != 3 ) {
			psyTkRec->tk_dsign[4] |= 0x10;
		}
	}
} 

//============================================================
// インボイス対応
//	摘要情報のチェック true: sy_tkyo_inp() を呼び出す
//------------------------------------------------------------
// 引数
//		rec				:			摘要情報
//============================================================
bool CDipZmSub::CheckTkrec( CDBipTKREC *pTkrec )
{
	ASSERT( pTkrec );
	if( pTkrec == NULL ) {
		return false;
	}

	// 摘要に免税事業者からの課税仕入れサインかに登録番号が設定されているかチェック
	bool bRet = false;
	if(( pTkrec->m_tkdsgn[4] & 0x10 ) || ( strlen( pTkrec->m_tkinvno ) > 0 )) {
		bRet = true;
	}

	//個別対応・簡易課税
	if( IsKaniSyz() ) {
		if( (pTkrec->m_tkdsgn[3] & 0x0f) > 0 ) {
			bRet = true;
		}
	}
	else if( IsKobetsuSyz() ) {
		if( (pTkrec->m_tkdsgn[4] & 0x0f) > 0 ) {
			bRet = true;
		}
	}

	return bRet;
} 


//============================================================
// インボイス対応
//	摘要情報のチェック true: sy_tkyo_inp() を呼び出す
//------------------------------------------------------------
// BYTE *tkdsgn		
//		rec				:			摘要情報
//============================================================
bool CDipZmSub::CheckTkrec(_SY_TKREC_ *syrec)
{
	ASSERT(syrec);
	if( syrec == NULL ) {
		return false;
	}

	// 摘要に免税事業者からの課税仕入れサインかに登録番号が設定されているかチェック
	bool bRet = false;
	if( (syrec->tk_dsign[4] & 0x10) || (strlen(syrec->tk_invno) > 0) ) {
		bRet = true;
	}

	//個別対応・簡易課税
	if( IsKaniSyz() ) {
		if( (syrec->tk_dsign[3] & 0x0f) > 0 ) {
			bRet = true;
		}
	}
	else if( IsKobetsuSyz() ) {
		if( (syrec->tk_dsign[4] & 0x0f) > 0 ) {
			bRet = true;
		}
	}

	return bRet;
}




//現在の摘要レコードセットを 摘要バッファクラスへ
int CDipZmSub::FuncTekiyoToRecord(CDBipTKREC* tek_rec)
{
	int tkyo_len = TKYO_LEN;
	if( !IsMasterType(MST_TKY20) )	tkyo_len = (TKYO_LEN / 2);

	tek_rec->m_tkcod = tkrec->tkcod;
	tek_rec->m_tkana = tkrec->tkana;
	tek_rec->m_tksgn.Copy(tkrec->tksgn);
	tek_rec->m_tkname = tkrec->tkname;
	// 後ろスペースをカット
	LPSTR	p = tek_rec->m_tkname.GetBuffer(256);
	int n = kjlen(p, tkyo_len);
	*(p + n) = '\0';
	tek_rec->m_tkname.ReleaseBuffer();

	tek_rec->m_tkari = tkrec->tkari;
	tek_rec->m_tkasi = tkrec->tkasi;

	if( tkrec->dsign.GetSize() != SIZE_DSIGN ) {
		tkrec->dsign.SetSize(SIZE_DSIGN);
	}

	// インボイス対応
	for( int i = 0; i < SIZE_DSIGN; i++ ) {
		tek_rec->m_tkdsgn[i] = tkrec->dsign[i];
	}

	if( !tkrec->invno.IsEmpty() ) {
		CString tmpInvno = tkrec->invno;
		tmpInvno.Replace(_T("T"), _T(""));
		if( tmpInvno.GetLength() < 14 ) {
			sprintf_s(tek_rec->m_tkinvno, sizeof(tek_rec->m_tkinvno), _T("%s"), tmpInvno);
		}
	}
	return 0;
}


// データベース 摘要読み込み
int CDipZmSub::DB_TekiyoNoRead( CDBipTKREC* tek_rec, int no )
{
	BOOL bRet = FALSE;
	if( no < 1 )	return -1;

//	tkrec->SetAbsolutePosition( no );
	int tkyo_len = TKYO_LEN;
	if( ! IsMasterType(MST_TKY20) )	tkyo_len = (TKYO_LEN/2);

#ifndef CLOSE
	if( tkrec_kanamode() ) {
		tkrec->Requery( "", 0 );
		tkrec_kanamode() = FALSE;
	}
	if( tkrec->MoveFirst() == 0 ) {
		do {
			if( no == tkrec->tkcod ) {
				FuncTekiyoToRecord(tek_rec);
#ifdef CLOSE //24.07.04
				tek_rec->m_tkcod = tkrec->tkcod;
				tek_rec->m_tkana = tkrec->tkana;
				tek_rec->m_tksgn.Copy( tkrec->tksgn );
				tek_rec->m_tkname = tkrec->tkname;
				// 後ろスペースをカット
				LPSTR	p = tek_rec->m_tkname.GetBuffer(256);
				int n = kjlen( p, tkyo_len );
				*(p + n) = '\0';
				tek_rec->m_tkname.ReleaseBuffer();

				tek_rec->m_tkari = tkrec->tkari;
				tek_rec->m_tkasi = tkrec->tkasi;

				if( tkrec->dsign.GetSize() != SIZE_DSIGN ) {
					tkrec->dsign.SetSize( SIZE_DSIGN );
				}

				// インボイス対応
				for( int i = 0; i < SIZE_DSIGN; i++ ) {
					tek_rec->m_tkdsgn[i] = tkrec->dsign[i];
				}

				if( !tkrec->invno.IsEmpty() ) {
					CString tmpInvno = tkrec->invno;
					tmpInvno.Replace( _T( "T" ), _T( "" ));
					if( tmpInvno.GetLength() < 14 ) {
						sprintf_s( tek_rec->m_tkinvno, sizeof( tek_rec->m_tkinvno ), _T( "%s" ), tmpInvno );
					}
				}
#endif
				bRet = TRUE;
				break;
			}
		} while( tkrec->MoveNext() == 0 );
	}
#else
	CString filter;
	filter.Format( "tkcod = %d", no );
	tkrec->Requery( filter, 0 );

	if( tkrec->st == 0 ) {
		if( no == tkrec->tkcod ) {
			tek_rec->m_tkcod = tkrec->tkcod;
			tek_rec->m_tkana = tkrec->tkana;
			tek_rec->m_tksgn.Copy( tkrec->tksgn );
			tek_rec->m_tkname = tkrec->tkname;
			tek_rec->m_tkari = tkrec->tkari;
			tek_rec->m_tkasi = tkrec->tkasi;

			bRet = TRUE;
		}
	}
#endif

	return bRet ? 0 : -1;
}

//	CDBDATA の DataCorrect(), DataAppend()は直接呼ばない！
//
// データベース データ修正
//
int CDipZmSub::DB_DataCorrect( CDBINPDataRec* data )
{
	if( data->m_seq <= 0 )
		return 0;

	// マスター全体確定
	if( ! (zvol->tl_cor & 0x10) || IsJzSansyo() )
		return 0;

//_DEBUG_FILEOUT( "@DB_DataCorrect data->m_seq = %d\n", data->m_seq );
//MyTrace( "DB_DataCorrect (%d), disgn[6] = %02x\n", data->m_seq, data->m_dsign[6] );

	if( DBDATA_SetPosition( data->m_seq ) == 0 ) {

//_DEBUG_FILEOUT( "@DB_DataCorrect dbdata seq = %d\n", dbdata->seq );
#ifdef NAIBU_VER2
		if( IsSyafuKaisei( zvol ) ) {
			if( data->m_hjiyu == 1 && data->m_nbcd == 0 )
				data->m_hjiyu = 0;
		}
#endif
		SRCTrace;
		SetCDBData( data );

//_DEBUG_FILEOUT( "@DB_DataCorrect after data->m_seq = %d\n", data->m_seq );
//_DEBUG_FILEOUT( "@DB_DataCorrect after dbdata seq = %d\n", dbdata->seq );

		if( data->m_dbmn != -1 ) {
			BumonZanTorok( data->m_dbmn, data->m_dbt );
		}
		if( data->m_cbmn != -1 && ((data->m_dbmn != data->m_cbmn) || (data->m_dbt != data->m_cre)) ) {
			BumonZanTorok( data->m_cbmn, data->m_cre );
		}

		SRCTrace;
		// チェックリストの出力サインを落とす。
//		dbdata->dsign[0] &= ~0x02;
		dbdata->dsign[0] &= ~0x0E;

		// 追加修正サイン
		IsModify() = TRUE;
		// 科目残高 クエリーサイン
		kzrec_Requery() = TRUE;
		// CDATA クエリーサイン ON --- 表示を更新するため
		data_Requery()	= TRUE;

		// 仮受・仮払自動枝番チェック
		int old_edcnt = 0;
		old_edcnt = GetKariAutoBrnCnt();

		SRCTrace;

		int st = dbdata->DataCorrect();

		SRCTrace;

		if( old_edcnt != GetKariAutoBrnCnt() ) {
			ezquery_flag() = TRUE;
		}
#ifdef NAIBU_VER2
		if( IsSyafuKaisei( zvol ) ) {
			pDBnc->NaibutorihikiDataAdd( dbdata->seq, data->m_nbcd );
		}
#endif
//MyTrace( "@DB_DataCorrect end dbdata seq = %d, st = %d\n", dbdata->seq, st );
		return st;
	}
	
	return -1;
}


// データベース データ修正
//		DBDATA_SetPosition() をした後、CDBDATAを直接変更した場合の
//		修正に使用
//
int CDipZmSub::DB_DataCorrect()
{
	// マスター全体確定
	if( ! (zvol->tl_cor & 0x10) || IsJzSansyo() )
		return 0;

	if( dbdata->dbmn != -1 ) {
		BumonZanTorok( dbdata->dbmn, dbdata->dbt );
	}
	if( dbdata->cbmn != -1 && ((dbdata->dbmn != dbdata->cbmn) || (dbdata->dbt != dbdata->cre)) ) {
		BumonZanTorok( dbdata->cbmn, dbdata->cre );
	}

	// 追加修正サイン
	IsModify() = TRUE;
	// 科目残高 クエリーサイン
	kzrec_Requery() = TRUE;

	// 仮受・仮払自動枝番チェック
	int old_edcnt = 0;
	old_edcnt = GetKariAutoBrnCnt();

	int st = dbdata->DataCorrect();

	if( old_edcnt != GetKariAutoBrnCnt() ) {
		ezquery_flag() = TRUE;
	}

	return st;
}


// データベース データ追加
//	return -1		エラー
//			1		最大まで登録
//			0		登録OK
//
int CDipZmSub::DB_DataAppend( CDBINPDataRec* data, int bseq )
{
	if( data->m_seq <= 0 )
		return 0;
	
	// マスター全体確定
	if( ! (zvol->tl_cor & 0x10) || IsJzSansyo() )
		return 0;

	CString filter;
	// MAXチェック
	filter = _T( "seq = 1" );
	dcntl->Requery( filter );
	if( dcntl->st != -1 ) {
		m_dacnt	= dcntl->cnt;
	}

	// 仮受・仮払自動枝番チェック
	int old_edcnt = 0;
	old_edcnt = GetKariAutoBrnCnt();

#ifdef MAXCHECK_CUT
	// 最大まで登録
	if( m_dacnt >= m_damax )
		return 1;
#endif

	data->m_owner = DBUserNo();

#ifdef NAIBU_VER2
	if( IsSyafuKaisei( zvol ) ) {
		if( data->m_hjiyu == 1 && data->m_nbcd == 0 )
			data->m_hjiyu = 0;
	}
#endif

	SetCDBData( data );
	int ret = -1;

	if( data->m_dbmn != -1 ) {
		BumonZanTorok( data->m_dbmn, data->m_dbt );
	}
	if( data->m_cbmn != -1 && ((data->m_dbmn != data->m_cbmn) || (data->m_dbt != data->m_cre)) ) {
		BumonZanTorok( data->m_cbmn, data->m_cre );
	}

//MyTrace("DataAppend seq = %d, ddate = %d, dbt = %s, cre = %s\n", data->m_seq, data->m_ddate, data->m_dbt, data->m_cre );
//MyTrace("DataAppend ddate = %d\n", dbdata->ddate);
	dbdata->dsign[8] = 6;

	if( bseq != -1 ) {
		//挿入
		if( dbdata->DataAppend( bseq ) == 0 ) {
			ret = 0;
		}
	}
	else {
		if( dbdata->DataAppend() == 0 ) {
			ret = 0;
		}
	}
	if( ! ret ) {
		insseq_make() = FALSE;	//SEQテーブル再作成
		m_insCnt = 0;
	}

	// 番号更新や、登録日等を反映
#ifdef NAIBU_VER2
	int old_nbcd = data->m_nbcd;
#endif
	GetCDBData( data );

#ifdef NAIBU_VER2
	//内部取引消去
	data->m_nbcd = old_nbcd;
	if( IsSyafuKaisei( zvol ) ) {
		pDBnc->NaibutorihikiDataAdd( data->m_seq, data->m_nbcd );
	}
#endif

	// 最終書き込みデータを保存
	m_lastdata = *data;
	m_lastdata.m_seq++;		// 次データ用にSEQセット
	
	MakeInpDate( &m_lastdata );

	// 追加時のみ リクエリ が必要
	dbdata_Requery() = TRUE;

	// CDATA クエリーサイン ON --- 表示を更新するため
	data_Requery()	= TRUE;

	// 追加修正サイン
	IsModify() = TRUE;
	// 科目残高 クエリーサイン
	kzrec_Requery() = TRUE;

	// 現在仕訳数を再セット
	filter = _T( "seq = 1" );
	dcntl->Requery( filter );
	if( dcntl->st != -1 ) {
		m_dacnt	= dcntl->cnt;
	}

	// 仮受・仮払自動枝番された？
	if( old_edcnt != GetKariAutoBrnCnt() ) {
		ezquery_flag() = TRUE;
	}

#ifdef MAXCHECK_CUT
	// 最大まで登録
	if( m_dacnt >= m_damax )
		ret = 1;
#endif

	return ret;
}


//#define USE_TOP_PARA

//********************************************************
//	dbdataのテーブルデータを指定SEQの位置にする
//
//	dbdata->Requery() を直接コードには書かない
//	
//	#define USE_TOP_PARA の場合
//	dbdata 上には TOP_COUNT 分 のデータを
//	SEQが順番に並んだ状態にして、データにアクセスする
//
//********************************************************/
int CDipZmSub::DBDATA_SetPosition( int seq )
{	
	int job, search, npos;
	CString filter;
	job = dbdata_Job();

	if( job == DBDATA_JOB_NORMAL ) {
		search = 0;
	}
	else if( job == DBDATA_JOB_SCAN ) {
		if( SCAN_PGTBL.sort_id != -1 ) {
			search = 3;	//カラムソート順
		}
		else {
			if( dbdata_Sort() == DBDATA_SORT_SEQ ) {
			//	search = 0;
				search = 2;	//挿入SEQ順
			}
			else {
				search = 1;	//スキャン時の日付順
			}
		}
	}
	else {
		search = -1;
		// 伝票形式・出納帳形式
		filter.Format( "seq = %d", seq );
		dbdata->Requery( -2, 0, filter, 0, 0 );
		dbdata_Requery() = TRUE;
	}
MyTrace(" CDipZmSub::DBDATA_SetPosition argc(seq = %d) search = %d\n", seq, search );

	if( search == 0 ) {

		if( dbdata_Speed() ) {
			BOOL bOK = FALSE;

			// SEQ 順で検索
			dbdata->MoveNext();

			if( dbdata->st == -1 )
				dbdata->MovePrev();


			if( dbdata->seq == seq ) {
				bOK = TRUE;
			}
			else {
				int cmpst = datarec_seqcmp( 0, dbdata->seq, seq );

				if( cmpst > 0 ) {
					while(1) {
						dbdata->MovePrev();

						if( dbdata->st == -1 )	break;

						if( dbdata->seq == seq ) {
							bOK = TRUE;
							break;
						}
						else if( datarec_seqcmp( 0, dbdata->seq, seq ) < 0 )
							break;
					}
				}
				else if( cmpst < 0 ) {
					while(1) {
						dbdata->MoveNext();

						if( dbdata->st == -1 ) {
							break;
						}
						if( dbdata->seq == seq ) {
							bOK = TRUE;
							break;
						}
						else if( datarec_seqcmp( 0, dbdata->seq, seq ) > 0 )
							break;
					}
				}
			}

			if( ! bOK ) {
				dbdata->Requery( -2/*DOPEN_MODE*/, 0, filter, 0, 0 );
				dbdata_Speed() = FALSE;
				npos = seq;
				dbdata->SetAbsolutePosition(npos);

				dbdata_Requery()	=	FALSE;
			}
		}
		else {
			// SEQ 順で検索
			dbdata->MoveNext();

			if( dbdata->st == -1 )
				dbdata->MovePrev();

			if( job == DBDATA_JOB_NORMAL && dbdata_Requery() ) {
				// 追加はされていた場合は、追加分がテーブルにのっていない。
				dbdata->Requery( -2, 0, "", 0, 0 );
				dbdata_Requery() = FALSE;
				dbdata->MoveLast();
				if( dbdata->st == -1 )
					return -1;
			}

			npos = seq;
			dbdata->SetAbsolutePosition(npos);
		}

		if( dbdata->st == -1 )	return -1;

#ifdef INS_CLOSE
		
		if( dbdata->seq > seq ) {
			while(1) {
				dbdata->MovePrev();

				if( dbdata->st == -1 )	return -1;

				if( dbdata->seq == seq )
					break;
				else if( dbdata->seq < seq )
					return -1;
			}
		}
		else if( dbdata->seq < seq ) {
			while(1) {
				dbdata->MoveNext();

				if( dbdata->st == -1 ) {
					if( job == DBDATA_JOB_NORMAL && dbdata_Requery() ) {
						// 追加はされていた場合は、追加分がテーブルにのっていない。
						dbdata->Requery( -2, 0, "", 0, 0 );
						dbdata_Requery() = FALSE;
						dbdata->MoveLast();
						goto RE_SEARCH;
					}
					else {
						return -1;
					}
				}

				if( dbdata->seq == seq )
					break;
				else if( dbdata->seq > seq )
					return -1;
			}
		}
#endif
	}
	else if( search == 1 ) {
		if( dbdata_scan_insert() ) {
			filter.Format( "seq = %d", seq );
			dbdata->Requery( -2, 0, filter, 0, 0 );
		}
		else {
			// SCAN 日付順で検索
			make_scandateseq();

			int order_seq	= SCMDFY_SEQ.ngetseq( seq-1 );

			dbdata->SetAbsolutePosition( order_seq );
		}
	}
	else if( search == 2 ) {
		if( dbdata_scan_insert() ) {
			filter.Format( "seq = %d", seq );
			dbdata->Requery( -2, 0, filter, 0, 0 );
		}
		else {
		MyTrace(" CDipZmSub::DBDATA_SetPosition dbdata->seq = %d\n", dbdata->seq);

			// 挿入SEQ順スキャン
			dbdata->MoveNext();

			SRCTrace;

			if( dbdata->st == -1 )
				dbdata->MovePrev();

			SRCTrace;

			int cmpst = datarec_seqcmp( 0, dbdata->seq, seq );

			SRCTrace;
			MyTrace(" CDipZmSub::DBDATA_SetPosition cmpst = %d\n", cmpst);

			if( cmpst > 0 ) {
				while(1) {
					MyTrace(" CDipZmSub::DBDATA_SetPosition loop(1) dbdata->seq = %d\n", dbdata->seq);

					dbdata->MovePrev();

					MyTrace(" CDipZmSub::DBDATA_SetPosition loop(2) dbdata->seq = %d\n", dbdata->seq);

					if( dbdata->st == -1 )	return -1;

					MyTrace(" CDipZmSub::DBDATA_SetPosition loop(3) dbdata->seq = %d\n", dbdata->seq);

					if( dbdata->seq == seq ) {
						break;
					}
					else if( datarec_seqcmp( 0, dbdata->seq, seq ) < 0 )
						return -1;
				}
			}
			else if( cmpst < 0 ) {
				while(1) {
					dbdata->MoveNext();

					if( dbdata->st == -1 ) {
						return -1;
					}
					if( dbdata->seq == seq ) {
						break;
					}
					else if( datarec_seqcmp( 0, dbdata->seq, seq ) > 0 )
						return -1;
				}
			}
		}
	}
	else if( search == 3 ) {
		check_scan_insertquery();
		int dbpos = MDFYDATA_SEQ.ngetseq( seq-1 );

		dbdata->SetAbsolutePosition( dbpos );
	}

#ifdef _SPEED_CLOSE

#ifndef USE_TOP_PARA
	if( dbdata_Requery() ) {
		dbdata->Requery( -2, 0, "", 0, 0 );
		dbdata_Requery() = FALSE;
	}
	dbdata->SetAbsolutePosition( seq );

#else 
	CString filter;
	int nStart;
	nStart = seq - (TOP_COUNT/2);
	if( nStart <= 0 )	nStart = 1;

	do {
		if( dbdata_Requery() ) {
			filter.Format( "seq >= %d", nStart );
			if( dbdata->Requery( -2, TOP_COUNT, filter, 0, 0 ) == 0 ) {

				m_DBstart = nStart;
				dbdata->MoveLast();
				m_DBend = dbdata->seq;

			}
			else {
				m_DBstart = -1;
				m_DBend = -1;
			}
			dbdata_Requery() = FALSE;
			break;
		}
		else {
			if( m_DBstart == -1 || m_DBend == -1 ) {
				ASSERT( FALSE );
				break;
			}
			if( seq < m_DBstart ) {
				// 現在の範囲より 小さい
				nStart = seq - TOP_COUNT + 1;
				if( nStart <= 0 )	nStart = 1;

				dbdata_Requery() = TRUE;
				continue;
			}
			if( seq > m_DBend ) {
				// 現在の範囲より 大きい
				nStart = seq;

				dbdata_Requery() = TRUE;
				continue;
			}
			break;
		}
	} while( 1 );

	int pos = seq - m_DBstart;
	dbdata->SetAbsolutePosition( pos + 1 );
#endif

#endif

ASSERT( dbdata->seq == seq );
	if( dbdata->seq == seq && dbdata->st != -1 )
		return 0;

	return -1;
}



struct _DBKNREC* CDipZmSub::DB_PjisToKnrec( CString pjiscode, BOOL bALL/*=FALSE*/ )
{
#ifdef DB_VER1_CLOSE
	const int kmcd_max = 6;

	CString tmp;
	tmp = pjiscode;
	int l, n, i;
	l = tmp.GetLength();
	if( (n = (kmcd_max - l)) > 0 ) {
		for( i = 0; i < n; i++ ) {
			tmp += " ";
		}
	}
#endif
	DWORD tmp;
	tmp = _atoh( (char*)(LPCTSTR)pjiscode );
	hl_rev( &tmp, 4 );

	DBKNREC* pKn;
	pKn = PjisToKnrec( tmp );

	if( pKn ) {
		if( bALL )
			return pKn;
		else {
			if( (pKn->knvoid & 0x01) )
				return NULL;
			else
				return pKn;
		}
	}
	return NULL;
}



struct _DBKNREC* CDipZmSub::DB_PjisToKnrec( DWORD dwPjis, BOOL bALL/*=FALSE*/)
{
	DBKNREC* pKn;
	pKn = PjisToKnrec( dwPjis );

	if( pKn ) {
		if( bALL )
			return pKn;
		else {
			if( (pKn->knvoid & 0x01) )
				return NULL;
			else
				return pKn;
		}
	}
	return NULL;
}


int CDipZmSub::DB_PjisToKmkOfst( CString pjiscode )
{
	DWORD tmp;
	tmp = _atoh( (char*)(LPCTSTR)pjiscode );
	hl_rev( &tmp, 4 );

	return DB_PjisToKmkOfst( tmp );
}


int CDipZmSub::DB_PjisToKmkOfst( DWORD pjis )
{
	int ofs = -1;

	for( int i = 0; i < knm_info->reccount; i++ ) {
		if( (pKnrec+i)->kncod == pjis ) {
			ofs = i;
			break;
		}
	}

	return ofs;
}


// 科目コードより該当の科目名称レコードをサーチ
int CDipZmSub::DB_KmkRecPjisSearch( CString pjiscode )
{
	DWORD tmp;
	tmp = _atoh( (char*)(LPCTSTR)pjiscode );
	hl_rev( &tmp, 4 );
	
	if( PjisToKnrec( tmp ) == 0 ) {
		return 0;
	}
	return -1;
/*	CString code;
	int ret = -1;

	knrec->MoveFirst();
	if( knrec->st != 0 )
		return -1;

	do {
		pjiscode.TrimRight();
		code = knrec->kncod;
		code.TrimRight();

		if( pjiscode == code ) {
			ret = 0;
			break;
		}
	} while( knrec->MoveNext() == 0 );

	return ret;
	//------------------------------
	CString filter;
	filter.Format( "kncod = '%s'", pjiscode );

	ASSERT( knrec != NULL );

	return knrec->Requery( filter, 0 );
*/
}

//入力コードより該当の科目名称レコードをサーチ
int CDipZmSub::DB_KmkRecInpcSearch( CString inpc )
{
	long inpcode;
	int ret = -1;

	knrec->MoveFirst();
	if( knrec->st != 0 )
		return -1;

	do {
		inpcode = atoi( inpc );

		if( inpcode == knrec->knicod ) {
			ret = 0;
			break;
		}
	} while( knrec->MoveNext() == 0 );

	return ret;

/*	CString filter;
	filter.Format( "knicod = '%s'", inpc );
	ASSERT( knrec != NULL );

	return knrec->Requery( filter, 0 );
*/
}

// 科目レコード番号より(0 ～)
int CDipZmSub::DB_KmkRecRnoSearch( int rno )
{
	ASSERT( FALSE );

	int ret = -1;

	knrec->MoveFirst();
	if( knrec->st != 0 )
		return -1;

	do {
		if( (rno+1) == knrec->knseq ) {
			ret = 0;
			break;
		}
	} while( knrec->MoveNext() == 0 );

	return ret;

/*	CString filter;
	filter.Format( "knseq = '%d'", rno+1 );
	ASSERT( knrec != NULL );

	return knrec->Requery( filter, 0 );
*/
}


// 摘要コードからデータレコードに摘要枝番をセット
int CDipZmSub::DB_TekiyoEdabanSet( CDBINPDataRec *data, int tkcode, BOOL bBmn )
{
	if( ! bBmn ) {
		for( int n = 0; n < 2; n++ ) {

			if( n == 0 ) {
				if( DB_EdabanZanSearch2( data->m_dbt, tkcode ) == 0 ) {
					if( data->m_dbr == -1 )
						data->m_dbr = ezrec->ezecd;
					if( data->m_dbt == data->m_cre ) {
						if( data->m_cbr == -1 )
							data->m_cbr = ezrec->ezecd;
						break;
					}
				}
			}
			else {
				if( DB_EdabanZanSearch2( data->m_cre, tkcode ) == 0 ) {
					if( data->m_cbr == -1 )
						data->m_cbr = ezrec->ezecd;
				}
			}
		}
	}
	else {
		//部門枝番
		for( int n = 0; n < 2; n++ ) {
			if( n == 0 ) {

				if( data->m_dbmn != -1 ) {
					if( DB_BmnEdabanMasterSearch( data, 0, tkcode ) == 0 ) {
						if( data->m_dbr == -1 )
							data->m_dbr = bmezrec->ezecd;
						if( data->m_dbt == data->m_cre && data->m_dbmn == data->m_cbmn ) {
							if( data->m_cbr == -1 )
								data->m_cbr = bmezrec->ezecd;
							break;
						}
					}
				}
				else {
					if( DB_EdabanZanSearch2( data->m_dbt, tkcode ) == 0 ) {
						if( data->m_dbr == -1 )
							data->m_dbr = ezrec->ezecd;
						if( data->m_dbt == data->m_cre && data->m_dbmn == data->m_cbmn ) {
							if( data->m_cbr == -1 )
								data->m_cbr = ezrec->ezecd;
							break;
						}
					}
				}
			}
			else {
				if( data->m_cbmn != -1 ) {
					if( DB_BmnEdabanMasterSearch( data, 1, tkcode ) == 0 ) {
						if( data->m_cbr == -1 )
							data->m_cbr = bmezrec->ezecd;
					}
				}
				else {
					if( DB_EdabanZanSearch2( data->m_cre, tkcode ) == 0 ) {
						if( data->m_cbr == -1 )
							data->m_cbr = ezrec->ezecd;
					}
				}
			}
		}
	}

	return 0;
}


static char Format_S[] = "S";


// 伝票番号 Format 作成
LPCTSTR CDipZmSub::GetDenpFormat()
{
	static CString fmt;
#ifdef LATER_CHECK
	if( zvol->dpsgn ) {
#endif
		fmt = "";
		for( int i = 0; i < 6; i++ ) {
			fmt += Format_S;
		}
#ifdef LATER_CHECK
	}
	else	fmt = "";
#endif
	return fmt;
}

// 部門番号 Format 作成
LPCTSTR CDipZmSub::GetBumonFormat()
{
	static CString fmt;
	if( (zvol->sub_sw & 0x02) ) {
		fmt = "";
		for( int i = 0; i < zvol->bmcol; i++ ) {
			fmt += Format_S;
		}
	}
	else	fmt = "";
	return fmt;
}

// 工事番号 Format 作成
LPCTSTR  CDipZmSub::GetKojiFormat()
{
	static CString fmt;

	if( (zvol->sub_sw&0x04) ) {
		fmt = "";
		for( int i = 0; i < zvol->kjcol; i++ ) {
			fmt += Format_S;
		}
	}
	else	fmt = "";
	return fmt;
}


// 部門コードを文字列に
void CDipZmSub::BumonCodeToStr( char* str, int strsize, int bmoncode )
{
	char fmt[10];

	sprintf_s( fmt, sizeof fmt, "%%0%dd", zvol->bmcol );

	if( bmoncode != -1 )	sprintf_s( str, strsize, fmt, bmoncode );
	else					str[0] = '\0';
}

// 工事番号を文字列に
void CDipZmSub::KojiCodeToStr( char* str, int strsize, CString kojicode )
{
	char fmt[10];

#ifdef ANKOK_CLOSE
	strcpy_s( fmt, sizeof fmt, "%s" );
#else	// ゼロをつめる
	sprintf_s( fmt, sizeof fmt, "%%0%ds", zvol->kjcol );
#endif

	if( ! kojicode.IsEmpty() ) {
		kojicode.TrimRight();	//左右のスペース除去
		kojicode.TrimLeft();
		sprintf_s( str, strsize, fmt, kojicode );
	}
	else
		::ZeroMemory( str, strsize );
}

// 枝番番号を文字列に
void CDipZmSub::EdabanToStr( char* str, int strsize, int edacode )
{
	char fmt[10];

	sprintf_s( fmt, sizeof fmt, "%%0%dd", zvol->edcol );

	if( edacode != -1 )	sprintf_s( str, strsize, fmt, edacode );
	else				str[0] = '\0';
}


#ifdef CLOSE_1ST_VER
// 月のオフセット取得
int CDipZmSub::Myvd_ofset( int ddate )
{
	int ofs = -1;

	minfo->MoveFirst();
	do {
		if( minfo->st == 0 ) {
			//TRACE( "%d--%d%d\n", ddate, minfo->MTB[i].smd, minfo->MTB[i].emd );
			if( ddate >= minfo->smd && ddate <= minfo->emd ) {
				ofs = minfo->ofset;
				break;
			}
		}
	} while( minfo->MoveNext() == 0 );

ASSERT( ofs != -1 );
	return ofs;
}


int CDipZmSub::Myvd_ofset( BYTE* bcddate )
{
	int ofs = -1;
	int ks_sgn = 0;
	BYTE bcd_smd[6] = {0};
	BYTE bcd_emd[6] = {0};
	BYTE chk[6] = {0};

	switch( bcddate[0] ) {
	case 0x00:	
		ks_sgn = 4;	break;
	case 0x61:	
	case 0x62:
	case 0x63:
		ks_sgn = (bcddate[0] & 0x0f);
		break;
	default:
		ks_sgn = KESY_SG_GET( bcddate[0] );
		break;
	}

	minfo->MoveFirst();
	do {
		if( minfo->st == 0 ) {
			if( ks_sgn > 0 ) {
				if( ks_sgn == minfo->kes_sgn ) {
					ofs = minfo->ofset;
					break;
				}
			}
			else if( minfo->kes_sgn == 0 ) {
				
				YmdConv( bcd_smd, (int*)&minfo->smd, CV2_1 );
				YmdConv( bcd_emd, (int*)&minfo->emd, CV2_1 );

				if( bcddate[1] != (BYTE)0xff ) {
					if( bcddate[1] >= bcd_smd[3] ) {
						memcpy( chk, bcd_smd, 3 );
						chk[3] = bcddate[1];
					}
					else if( bcddate[1] <= bcd_emd[3] ) {
						memcpy( chk, bcd_emd, 3 );
						chk[3] = bcddate[1];
					}
				}
				else {
					memcpy( chk, bcd_emd, 4 );
				}

				if( memcmp( chk, bcd_smd, 4 ) >= 0 && 
					memcmp( chk, bcd_emd, 4 ) <= 0 ) {
					ofs = minfo->ofset;
					break;
				}
			}
		}
	} while( minfo->MoveNext() == 0 );

ASSERT( ofs != -1 );
	return ofs;
}

#endif


int CDipZmSub::Myvd_chek( BYTE* date )
{
	BYTE bcd_smd[6] = {0};
	BYTE bcd_emd[6] = {0};
	BYTE chk[6] = {0};
	int ret = -1;
	int ks_sgn = 0;

	if( minfo == NULL )
		return -1;

	ASSERT( (date[1] > 0x00 && date[1] <= 0x31) || date[1] == (BYTE)0xff );

	switch( date[0] ) {
	case 0x00: ks_sgn = 4;	break;
	case 0x61: 
	case 0x62:
	case 0x63:
		ks_sgn = (date[0] & 0x0f);
		break;
	default:
		break;
	}

	CVolDateDB vd;

	if( vd.db_vd_check( date, this ) == 0 )
		ret = 0;

#ifdef OLD_KEEP
//	if( *date == 0x00 ) {
	if( ks_sgn ) {
		minfo->MoveFirst();
		do {
		//	if( minfo->ofset == 13 ) {
			if( ks_sgn == minfo->kes_sgn ) {
				ret = 1;
				if( minfo->insw & 0x01 ) {
					YmdConv( bcd_smd, (int*)&minfo->smd, CV2_1 );
					YmdConv( bcd_emd, (int*)&minfo->emd, CV2_1 );

					if( date[1] != (BYTE)0xff ) {
						if( date[1] >= bcd_smd[3] ) {
							memcpy( chk, bcd_smd, 3 );
							chk[3] = date[1];
						}
						else if( date[1] <= bcd_emd[3] ) {
							memcpy( chk, bcd_emd, 3 );
							chk[3] = date[1];
						}
					}
					else {
						memcpy( chk, bcd_emd, 4 );
					}

					if( memcmp( chk, bcd_smd, 4 ) >= 0 && 
						memcmp( chk, bcd_emd, 4 ) <= 0 ) {
						ret = 0;
					}
				}
				break;
			}
		} while( minfo->MoveNext() == 0 );
	}
#ifdef CLOSE /////////////////////////////////////////////
	else if( *date == 0x60 ) {
		minfo->MoveFirst();
		do {
			if( minfo->kes_sgn && minfo->ofset < 13 ) {
				ret = 1;
				if( minfo->insw & 0x01 ) {
					YmdConv( bcd_smd, (int*)&minfo->smd, CV2_1 );
					YmdConv( bcd_emd, (int*)&minfo->emd, CV2_1 );

					if( date[1] != (BYTE)0xff ) {
						if( date[1] >= bcd_smd[3] ) {
							memcpy( chk, bcd_smd, 3 );
							chk[3] = date[1];
						}
						else if( date[1] <= bcd_emd[3] ) {
							memcpy( chk, bcd_emd, 3 );
							chk[3] = date[1];
						}
					}
					else {
						memcpy( chk, bcd_emd, 4 );
					}

					if( memcmp( chk, bcd_smd, 4 ) >= 0 && 
						memcmp( chk, bcd_emd, 4 ) <= 0 ) {
						ret = 0;
					}
				}
			}
		} while( minfo->MoveNext() == 0 );
	}
#endif ///////////////////////////////////////////////////////////////////////
	else {
		minfo->MoveFirst();

		do {
			YmdConv( bcd_smd, (int*)&minfo->smd, CV2_1 );
			YmdConv( bcd_emd, (int*)&minfo->emd, CV2_1 );

			::ZeroMemory( chk, sizeof chk );
			if( date[0] == bcd_smd[2] ) {
				memcpy( chk, bcd_smd, 2 );
				memcpy( chk+2, date, 2 );
			}
			else if( date[0] == bcd_emd[2] ) {
				memcpy( chk, bcd_emd, 2 );
				memcpy( chk+2, date, 2 );
			}
			if( chk[0] ) {
				if( memcmp( chk, bcd_smd, 4 ) >= 0 && 
					memcmp( chk, bcd_emd, 4 ) <= 0 ) {
					if( minfo->insw & 0x01 )
						ret = 0;
					else
						ret = 1;
					break;
				}
			}
		} while( minfo->MoveNext() == 0 );
	}
#endif

	return ret;
}

// 月日をBCDで返送
//	date[0] = 0x80->決修, 0x60 中間, 0x00 
//
//  dst_date[0],[1] には 通常の日付をセット
//	dst_date[0] ... D7-D5 bit が 0 = 通常, 1 = 決修, 2 = 中間
// 
int CDipZmSub::Myvd_sign( BYTE* date, BYTE* dst_date )
{
	if( minfo->GetRecordCount() > 14 )
		return ::Myvd_sign( date, dst_date, this );
	else {
		int ret = -1;

		BYTE work[6] = {0};
		BYTE smd[4] = {0};
		BYTE emd[4] = {0};
		BYTE sgn = 0;

		ASSERT( (date[1] > 0x00 && date[1] <= 0x31) || date[1] == (BYTE)0xff );

		if( *date == 0x00) {	//決修
			minfo->MoveFirst();
			do {
				if( minfo->ofset == 13 ) {
					DateConv( smd, (int*)&minfo->smd, CV2_1 );
					DateConv( emd, (int*)&minfo->emd, CV2_1 );
			
					if( date[1] != (BYTE)0xff ) {
						if( date[1] >= smd[1] ) {
							work[0] = smd[0];
						}
						else if( date[1] <= emd[0] ) {
							work[0] = emd[0];
						}
						work[1] = date[1];
					}
					else {
						memcpy( work, emd, 2 );
					}
					break;
				}
			} while( minfo->MoveNext() == 0 );
			sgn = 1;
		}
		else if( *date == 0x60 ) {	//中間
			minfo->MoveFirst();
			do {
				if( minfo->kes_sgn && minfo->ofset < 13 ) {
					DateConv( smd, (int*)&minfo->smd, CV2_1 );
					DateConv( emd, (int*)&minfo->emd, CV2_1 );

					if( date[1] != (BYTE)0xff ) {
						if( date[1] >= smd[1] ) {
							work[0] = smd[0];
						}
						else if( date[1] <= emd[0] ) {
							work[0] = emd[0];
						}
						work[1] = date[1];
					}
					else {
						memcpy( work, emd, 2 );
					}
					break;
				}
			} while( minfo->MoveNext() == 0 );
			sgn = 2;
		}
		else {	//　通常
			minfo->MoveFirst();
			int yy1 = minfo->emd;
			do {

				DateConv( work, (int*)&minfo->emd, CV2_1 );
				if( date[0] == work[0] ) {
					work[1] = date[1];
					break;
				}
			} while( minfo->MoveNext() == 0 );

		}
		if( work[0] ) {
			// うるう年のチェック
			if( work[0] == (BYTE)0x02 && work[1] == (BYTE)0x29 ) {
				int yy, mm;

				minfo->MoveFirst();
				do {
					yy = minfo->smd/10000;
					mm = (minfo->smd%10000) / 100;
					if( mm == 2 ) {
						// うるう年のチェック
						if( ! uru_year( yy ) )
							return -1;

						break;
					}
				} while( minfo->MoveNext() == 0 );
			}
			memcpy( dst_date, work, 2 );
			
			KESY_SG_SET( dst_date[0], sgn );
			ret = 0;
		}

		return ret;
	}

}


// 月日を 変換 仕訳レコードより
int	CDipZmSub::DateConv( BYTE* bcddate, CDBINPDataRec* d_date, int sgn )
{
	char buf[30];

	if( sgn == CV1_2 ) {
		// BCD -> Record	
		BcdDateToYmd( bcddate, &d_date->m_ddate );

		int ks_sgn;
		ks_sgn = KESY_SG_GET( bcddate[0] );

		if( ks_sgn >= 1 && ks_sgn <= 4 ) {	// 中間・決修
			minfo->MoveFirst();
			do {
				if( minfo->kes_sgn == ks_sgn ) {
					d_date->m_ksign = 1;
					d_date->m_mofs = minfo->ofset;
					break;
				}
			} while( minfo->MoveNext() == 0 );
		}
		else {
			BYTE tmp[10] = {0};
			tmp[0] = (bcddate[0]&0x1f);
			tmp[1] = (bcddate[1]&0xff);

			d_date->m_ksign = 0;
			CVolDateDB vd;
			int ofs;

			if( vd.db_vd_offset(tmp, &ofs, this) == -1 ) {
				return -1;
			}
			d_date->m_mofs = (BYTE)ofs;
		}
	}
	else {
		// Record → BCD
		sprintf_s( buf, sizeof buf, "%d", d_date->m_ddate );
		if( strlen( buf ) == 8 ) {
			l_pack( bcddate, &buf[4], 4 );
		}
		else {
			l_pack( bcddate, &buf[0], 4 );	
		}
		BYTE sgn = 0;
		if( d_date->m_ksign ) {
			minfo->MoveFirst();
			do {
				if( minfo->kes_sgn ) {
					if( d_date->m_ddate >= minfo->smd &&
						d_date->m_ddate <= minfo->emd ) {
						sgn = minfo->kes_sgn;
						break;
					}
				}

			} while( minfo->MoveNext() == 0 );

			if( ! sgn ) {
				ASSERT( FALSE );
				return -1;
			}

			KESY_SG_SET( bcddate[0], sgn );
		}
	}

	return 0;
}



// 月日を 変換
int	CDipZmSub::DateConv( BYTE* bcddate, int* date, int sgn )
{
	char buf[20];

	if( sgn == CV1_2 ) {
		// BCD -> int	
		sprintf_s( buf, sizeof buf, "%02x%02x", (*bcddate)&0x1f, *(bcddate+1) );
		*date = atoi( buf );
	}
	else {
		// int → BCD
		sprintf_s( buf, sizeof buf, "%d", *date );
		if( strlen( buf ) == 8 ) {
			l_pack( bcddate, &buf[4], 4 );
		}
		else {
			l_pack( bcddate, &buf[0], 4 );
		}
	}

	return 0;
}


// 年月日を 変換
int	CDipZmSub::YmdConv( BYTE* bcdymd, int* date, int sgn )
{
	char buf[30];

	if( CV1_2 ) {
		// BCD -> int	
		l_unpac( buf, bcdymd, 8 );
		*date = atoi( buf );
	}
	else {
		// int → BCD
		sprintf_s( buf, sizeof buf, "%d", *date );
		l_pack( bcdymd, &buf[0], 8 );
	}

	return 0;
}


// BCD 日付 から int 日付へ 変換
int CDipZmSub::BcdDateToYmd( BYTE* bcddate, int* ymd )
{
	BYTE bcd[10] = {0};
	char buf[64];
	int date, sgn, mmdd, y_sw, ret, gengo;
	ret = -1;

	CVolDateDB vd;

	sgn = KESY_SG_GET( bcddate[0] );

	switch( sgn ) {
	case 1:	sprintf_s( buf, sizeof buf, "61%02x", bcddate[1] );	break;
	case 2:	sprintf_s( buf, sizeof buf, "62%02x", bcddate[1] );	break;
	case 3:	sprintf_s( buf, sizeof buf, "63%02x", bcddate[1] );	break;
	case 4:	sprintf_s( buf, sizeof buf, "00%02x", bcddate[1] );	break;
	default:
		sprintf_s( buf, sizeof buf, "%02x%02x", bcddate[0], bcddate[1] );	break;
		break;
	}

	mmdd = atoi( buf );

	if( vd.db_vd_yearGen( 0, mmdd, &gengo, &date, &y_sw, this ) == 0 ) {
		int seireki, d_gengo;

		vd.db_datecnvGen( gengo, date, &d_gengo, &seireki, 1, 0 );

		*ymd = seireki;
		ret = 0;
	}

	return ret;
}

// BCD を文字列に
//  BCD[0],[1] には 通常の日付をセット
//	BCD[0] ... D7-D5 bit が 0 = 通常, 1 = 決修, 2 = 中間
void CDipZmSub::BcdDateToString( char* str, int strsize, BYTE* bcddate, BOOL bMnthFix /*=FALSE*/)
{
#ifdef CLOSE
	if( ! bMnthFix ) {
		if( bcddate[0] != 0 && bcddate[1] != 0 ) {
			if( KESY_SG_GET( bcddate[0] ) == 0 )
				sprintf( str, "%02x.%02x", (bcddate[0]&0x1f), bcddate[1] );
			else
				sprintf( str, "%02x/%02x", (bcddate[0]&0x1f), bcddate[1] );
		}
		else	strcpy( str, "      " );
	}
	else {
		if( bcddate[0] != 0 && bcddate[1] != 0 ) {
			sprintf( str, "%02x", bcddate[1] );
		}
		else	strcpy( str, "      " );	
	}
#endif

	::BcdDateToString( str, strsize, bcddate, bMnthFix );
}

void CDipZmSub::BcdDateToString( CString& string, BYTE* bcddate, BOOL bMnthFix /*=FALSE*/)
{
#ifdef CLOSE
	LPTSTR p;
	p = string.GetBuffer(20);
	BcdDateToString( p, bcddate, bMnthFix );
	string.ReleaseBuffer(-1);
#endif
	::BcdDateToString( string, bcddate, bMnthFix );
}


#ifdef CUT // DLL のほうを使う
// int 西暦日付を 平成年月日 文字列に
void CDipZmSub::IntsrekitoGenstr( char* str, int ddate )
{
	int genyy, mm, dd, yy;

	if( ddate <= 0 ) str[0] = '\0';
	else {
		CVolDateDB vd;
		yy = (ddate / 10000);
		mm = (ddate % 10000) / 100;
		dd = (ddate % 10000) % 100;

	//	sprintf( str, "%d", ddate );
		yy *= 10000;
		if( vd.db_datecnv( yy, &genyy, 0, 2 ) == 0 ) {
			sprintf( str, sizeof str, "%02d/%02d/%02d", genyy, mm, dd );
		}
		else {
			 str[0] = '\0';
		}
	}
}

#endif



// 科目コードを 変換
void CDipZmSub::TV_kamokucodeConv( WORD& kmkcode, CString& codestr, int sgn )
{
	if( sgn == CV2_1 ) {
		int tmp = _atoh( (char*)(LPCTSTR)codestr );
		kmkcode = hl_lev( tmp );
	}
	else {
		codestr.Format( "%04x", hl_lev( kmkcode ) );
	}
}


// 枝番コードを 変換
void CDipZmSub::TV_EdabancodeConv( WORD& edaban, int& new_eda, int sgn )
{
	char buf[20];
	if( sgn == CV2_1 ) {
		EdabanToStr( buf, sizeof buf, new_eda );
		if( buf[0] != '\0' ) {
			WORD tmpeda = _atoh( buf );
			edaban = hl_lev( tmpeda );
		}
		else {
			edaban = 0xffff;
		}
	}
	else {
		if( edaban != (WORD)0xffff ) {
			sprintf_s( buf, sizeof buf, "%04x", hl_lev( edaban ) );
			new_eda = atoi( buf );
		}
		else {
			new_eda = -1;
		}
	}
}


// データベース 枝番マスター 検索
int CDipZmSub::DB_EdabanMasterSearch( CDBINPDataRec* data, int dbcr_sg )
{
	CString filter, buf;

#ifdef CLOSE
	if( pDBzm->ezrec->MoveFirst() == 0 ) {
		do {
			int tmp;
			if( ! dbcr_sg ) {
				buf.Format( "%04x",  hl_lev(data->darec._DBT) );
				sprintf( ecdbf, "%04x", hl_lev(data->darec._DBR) );
				tmp = atoi( ecdbf );
			}
			else {
				buf.Format( "%04x",  hl_lev(data->darec._CRE) );
				sprintf( ecdbf, "%04x", hl_lev(data->darec._CBR) );
				tmp = atoi( ecdbf );
			}
			if( ! strncmp( pDBzm->ezrec->ezkcd, buf, 4 ) && tmp == pDBzm->ezrec->ezecd ) {
				brec->EZkcd = _atoh( (char*)(LPCTSTR)pDBzm->ezrec->ezkcd );
				hl_rev( &brec->EZkcd );

				sprintf( ecdbf, "%04d", pDBzm->ezrec->ezecd );
				brec->EZecd = _atoh( ecdbf );
				hl_rev( &brec->EZecd );

				brec->EZtcd = pDBzm->ezrec->eztcd;
				hl_rev( &brec->EZtcd );
				brec->EZatt = pDBzm->ezrec->ezatt[0];

				return 0;
			}
		} while( pDBzm->ezrec->MoveNext() == 0 );
	}

#else

	ezdb_init( this );
	if( ! dbcr_sg ) {
		if( data->m_dbr == -1 )
			return -1;
//		filter.Format( "ezkcd = '%s' AND ezecd = %d", data->m_dbt, data->m_dbr );
		if( ezrec->Find( (LPCTSTR)data->m_dbt, data->m_dbr ) > 0 ) return 0;
	}
	else {
		if( data->m_cbr == -1 )
			return -1;
//		filter.Format( "ezkcd = '%s' AND ezecd = %d", data->m_cre, data->m_cbr );
		if( ezrec->Find( (LPCTSTR)data->m_cre, data->m_cbr ) > 0 ) return 0;
	}

/*	if( ezrec->Requery( filter, 1 ) == 0 ) {
		return 0;
	}*/
#endif

	return -1;
}


// 枝番残高検索
int CDipZmSub::DB_EdabanZanSearch( CString& kmkcode, int brn )
{
//	CString filter;

	ezdb_init( this );

	if( brn != -1 ) {
//		filter.Format( "ezkcd = '%s' AND ezecd = %d", kmkcode, brn );
		if( ezrec->Find( (LPCTSTR)kmkcode, brn ) > 0 ) return 0;
	}
	else {
//		filter.Format( "ezkcd = '%s'", kmkcode );
		if( ezrec->Find( (LPCTSTR)kmkcode ) > 0 ) return 0;
	}

/*	if( ezrec->Requery( filter, 1 ) == 0 ) {
		return 0;
	}*/

	return -1;
}


// 枝番残高検索
int CDipZmSub::DB_EdabanZanSearch2( CString& kmkcode, int tkcod )
{
//	CString filter;

	ezdb_init( this );

	if( tkcod != -1 ) {
//		filter.Format( "ezkcd = '%s' AND eztcd = %d", kmkcode, tkcod );
		if( ezrec->Find2( (LPCTSTR)kmkcode, tkcod ) > 0 ) return 0;
	}
	else {
//		filter.Format( "ezkcd = '%s'", kmkcode );
		if( ezrec->Find( (LPCTSTR)kmkcode ) > 0 ) return 0;
	}

/*	if( ezrec->Requery( filter, 1 ) == 0 ) {
		return 0;
	}*/

	return -1;
}


// 枝番残高登録
int CDipZmSub::EdabanZanTorok( CDBipEZREC* ez )
{
	CString filter;

	filter.Format( "ezkcd = '%s' AND ezecd = %d", ez->m_ezkcd, ez->m_ezecd );

	if( ezrec->Requery( filter, 1 ) != 0 ) {

		ezrec->AddNew();
		// 先に追加だけしておく
		ezrec->ezkcd	= ez->m_ezkcd;
		ezrec->ezecd	= ez->m_ezecd;
		ezrec->eztcd	= ez->m_eztcd;
		ezrec->ezatt	= ez->m_ezatt;

		memcpy(&ezrec->ezsysgn[2], &ez->m_ezsysgn[2], 4);
		ezrec->ezsysgn[5] = 0;

		ezrec->Update();
	//	ezrec->Requery("", 1);

		IsEdaAdd() = TRUE;

		if ((zvol->s_sgn2 & 0x10)) {
			ezrec->Requery("", 1);
			ezquery_flag() = FALSE;
		}

#ifdef CUT_CLOSE
		// 続いて編集
		if( ezrec->Requery( filter, 1 ) != 0 ) {
			ASSERT( FALSE );
			return -1;
		}
		ezrec->Edit();
//		ezrec->ezsign.Copy( ez->m_ezsign );
//		ezrec->ezname	= ez->m_ezname;
//		ezrec->ezritsu1 = ez->m_ezritsu1;
//		ezrec->ezritsu2 = ez->m_ezritsu2;
		ezrec->ezzan	= ez->m_ezzan;

/*		int max = sizeof ezrec->lhs / sizeof ezrec->lhs[0];
		for( int i = 0; i < max; i++ ) {
			ezrec->lhs[i] = ez->m_lhs[i];
			ezrec->rhs[i] = ez->m_rhs[i];
		} */

		ezrec->Update();
#endif
	}

	return 0;
}


// 部門枝番残高登録
int CDipZmSub::BmnEdabanZanTorok( CDBipEZREC* ez, int bmn )
{
	CString filter;

	filter.Format( "ezbmn = %d AND ezkcd = '%s' AND ezecd = %d", bmn, ez->m_ezkcd, ez->m_ezecd );

	if( bmezrec->Requery( filter, 1 ) != 0 ) {

		bmezrec->AddNew();
		// 先に追加だけしておく
		bmezrec->ezbmn	= bmn;
		bmezrec->ezkcd	= ez->m_ezkcd;
		bmezrec->ezecd	= ez->m_ezecd;
		bmezrec->eztcd	= ez->m_eztcd;
		bmezrec->ezatt	= ez->m_ezatt;
	
		bmezrec->Update();

#ifdef CUT_CLOSE
		// 続いて編集
		if( bmezrec->Requery( filter, 1 ) != 0 ) {
			ASSERT( FALSE );
			return -1;
		}

		bmezrec->Edit();
//		bmezrec->ezsign.Copy( ez->m_ezsign );
//		bmezrec->ezname	= ez->m_ezname;
//		bmezrec->ezritsu1 = ez->m_ezritsu1;
//		bmezrec->ezritsu2 = ez->m_ezritsu2;
		bmezrec->ezzan	= ez->m_ezzan;

/*		int max = sizeof bmezrec->lhs / sizeof bmezrec->lhs[0];
		for( int i = 0; i < max; i++ ) {
			bmezrec->lhs[i] = ez->m_lhs[i];
			bmezrec->rhs[i] = ez->m_rhs[i];
		} */

		bmezrec->Update();
#endif
	}

	return 0;
}


// データベース 部門枝番マスター 検索
int CDipZmSub::DB_BmnEdabanMasterSearch( CDBINPDataRec* data, int dbcr_sg )
{
	CString filter, buf;

	if( ! dbcr_sg ) {
		if( data->m_dbmn == -1 || data->m_dbr == -1 )
			return -1;
		filter.Format( "ezbmn = %d AND ezkcd = '%s' AND ezecd = %d", data->m_dbmn, data->m_dbt, data->m_dbr );
	}
	else {
		if( data->m_cbmn == -1 || data->m_cbr == -1 )
			return -1;
		filter.Format( "ezbmn = %d AND ezkcd = '%s' AND ezecd = %d", data->m_cbmn, data->m_cre, data->m_cbr );
	}

	if( bmezrec->Requery( filter, 1 ) == 0 ) {
		return 0;
	}

	return -1;
}


// データベース 部門枝番マスター 検索( 摘要コード )
int CDipZmSub::DB_BmnEdabanMasterSearch( CDBINPDataRec* data, int dbcr_sg, int tkcod )
{
	CString filter, buf;

	if( ! dbcr_sg ) {
		if( data->m_dbmn == -1 || tkcod == -1 )
			return -1;
		filter.Format( "ezbmn = %d AND ezkcd = '%s' AND eztcd = %d", data->m_dbmn, data->m_dbt, tkcod );
	}
	else {
		if( data->m_cbmn == -1 || tkcod == -1 )
			return -1;
		filter.Format( "ezbmn = %d AND ezkcd = '%s' AND eztcd = %d", data->m_cbmn, data->m_cre, tkcod );
	}

	if( bmezrec->Requery( filter, 1 ) == 0 ) {
		return 0;
	}

	return -1;
}


// 部門残高登録( 0として登録 )
int CDipZmSub::BumonZanTorok( int bmn, CString  kmcod )
{
	CString filter;
	BOOL bKmkOK;

	if( ! (zvol->sub_sw & 0x02) )
		return 0;

	// 該当科目コードが削除されている恐れがあるので
	// 科目コードチェック
	filter.Format( "kncod = '%s'", kmcod );
	knrec->Requery( filter, 0 );
	if( knrec->st != 0 )	bKmkOK = FALSE;
	else					bKmkOK = TRUE;
	knrec->Requery( "", 0 );

ASSERT( bmname != NULL );
ASSERT( bmkzrec != NULL );

	// 名称ファイルがあるか
	filter.Format( "bnbcd = %d", bmn );
	int dseq = -1;
	int nam_sw = 0;
	int msai_bmn = 0;

	bmname->Requery( "", "" );
	if( bmname->MoveFirst() == 0 ) {
		do {
			if( bmn == bmname->bnbcd ) {
				nam_sw = 1;
				if( ! (bmname->bnhsg[0] & 0x02) )
					msai_bmn = 1;
				
				break;
			}
			if( bmname->klevel == 0 ) {
				if( dseq < bmname->dseq ) {
					dseq = bmname->dseq;
				}
			}
		} while( bmname->MoveNext() == 0 );
	}

	if( ! nam_sw ) {
		dseq++;
		bmname->AddNew();
		bmname->bnbcd = bmn;
		bmname->prbcd = -1;
		bmname->pseq = -1;
		bmname->dseq = dseq;

		msai_bmn = 1;
		bmname->Update();
		// 選択欄に表示用
		bmname->Requery( "", "" );

		BmChk.BmnCheckUpdate();
	}

	if( ! bKmkOK )	return 0;

#ifdef ZMSUB_CLOSE // 財務クラスで追加する
	if( msai_bmn ) {
		filter.Format( "kzbmn = %d AND kzkcd = '%s'", bmn, kmcod );

		if( bmkzrec->Requery( filter ) != 0 ) {
			bmkzrec->AddNew();
			bmkzrec->kzbmn	= bmn;
			bmkzrec->kzkcd	= kmcod;
			struct _DBKNREC* pKn;
			pKn = DB_PjisToKnrec( kmcod );
			bmkzrec->kzatt	= pKn->knatt;
			bmkzrec->Update();
		//	bmkzrec->Requery("", 0);
		}
	}
#endif
	return 0;
}



// 枝番MAX 取得
int CDipZmSub::GetEdabanMaxDecimal()
{
	return _op_max_calq( zvol->edcol );
}

// 部門 MAX 取得
int CDipZmSub::GetBumonMaxDecimal()
{
	return _op_max_calq( zvol->bmcol );
}

// 工事番号 MAX 取得
int CDipZmSub::GetKojiMaxDecimal()
{
	return _op_max_calq( zvol->kjcol );
}


/////////////////////////////////////////////////////////////////////////////////////////////
// 部門選択関係

#ifdef DB_OLD_CLOSE

void CDipZmSub::Bm_CloseWork()
{
	// メモリー開放
	if( m_Btb.pBSEL_REC ) {
		delete [] m_Btb.pBSEL_REC;
		m_Btb.pBSEL_REC = NULL;
	}
	if( m_BtbKOJI.pBSEL_REC ) {
		delete [] m_BtbKOJI.pBSEL_REC;
		m_BtbKOJI.pBSEL_REC = NULL;
	}
}


int CDipZmSub::mk_BmnSelTbl()
{
	if( zvol->apno == 0x20 ) {
		if( _mk_KojiSelTbl() != 0 )
			return -1;
	}

	if( _mk_BmnSelTbl() != 0 )	return -1;

	return 0;
}


// 工事台帳 の読み込み
int CDipZmSub::_mk_KojiSelTbl()
{
	BOOL bOK;
	CString path;	

	if( !(zvol->sub_sw & 0x04) )
		return 0;

	// 工事台帳レコード
	bOK = FALSE;

#ifdef LATER_CLOSE
	if( (m_BtbKOJI.pBSEL_REC = new BMNSEL_REC[kjvol.KV_KJCUR] ) == NULL )
		goto KOJIERR;
	
	n = 0;
	while(1) {
		if((fd = opn_sam( (void*)(LPCTSTR)path, "r", 2 )) == -1 ) break;
		
		for( i = 0;  ; i++ ) {
			if( sgetr( fd, &kjmas ) != 0 )	break;

			// 消去レコードのスキップ
			if( kjmas.KJ_KKBN[0] == (BYTE)0xff )
				continue;
#ifdef KEEP
			nxt = 0;
			pchk = (char*)&kjmas;
			for( j = 0; j < 10; j++ ) {
				if( *(pchk+j) < 0x30 || *(pchk+j) > 0x39 ) {
					nxt = 1;	break;
				}
			}
			if( !nxt ) {
#endif
				memcpy( kjno, kjmas.KJ_CODE, sizeof(kjmas.KJ_CODE));
				kjno[sizeof(kjmas.KJ_CODE)] = '\0';
				l_pack( (m_BtbKOJI.pBSEL_REC + n)->bmncod, kjno, strlen(kjno));
				(m_BtbKOJI.pBSEL_REC + n)->s_txt.Format( "%s%.30s", kjno, kjmas.KJ_NAME );
				
				if( ++n >= kjvol.KV_KJCUR )
					break;
#ifdef KEEP
			}
#endif
		}
		if( n ) {
			m_BtbKOJI.bninp = kjvol.KV_KJCUR;
			m_BtbKOJI.page_max = 1;
			m_BtbKOJI.page_now = 1;
			m_BtbKOJI.page_recn = 1;
			m_BtbKOJI.rec_n = n;

			qsort( m_BtbKOJI.pBSEL_REC, n, sizeof(BMNSEL_REC), KojiSort);
		}
		else {
			m_BtbKOJI.bninp = 0; 
			m_BtbKOJI.page_max = 0;
			m_BtbKOJI.page_now = 0;
			m_BtbKOJI.page_recn = 0;
			m_BtbKOJI.rec_n = 0;		
		}

		bOK = TRUE;
		break;
	}

	if( bOK == FALSE ) goto KOJIERR;

	m_BMNSEL_MOD = 1;
#endif
	
	return 0;

KOJIERR:
	m_BMNSEL_MOD = -1;

	return -1;
	
}


// 部門名称ファイル読み込み
int CDipZmSub::_mk_BmnSelTbl()
{
	int n, bmcnt;
	BOOL bOK;
	char buf[20];

	if( !(zvol->sub_sw & 0x02) )
		return 0;

	if( bmname == NULL )
		return 0;

	// 部門名称レコード
	bmname->Requery("", "");
	bmcnt = bmname->GetRecordCount();
	bmname->MoveFirst();
	bOK = FALSE;

	if( (m_Btb.pBSEL_REC = new BMNSEL_REC[bmcnt] ) == NULL )
		goto BMERR;
	
	n = 0;

	do {
		if( bmname->st == 0 ) {	// 明細のみ
			if( (bmname->bnhsg & 0x02) )	continue;

			(m_Btb.pBSEL_REC + n)->bkcod.Empty();
			BumonCodeToStr( buf, sizeof buf, bmname->bnbcd );
			(m_Btb.pBSEL_REC + n)->bkcod = buf;

			(m_Btb.pBSEL_REC + n)->s_txt.Format( "%s%.20s", buf, bmname->bnbnm);
			n++;
		}
		else	break;
	} while ( bmname->MoveNext() == 0 ) ;

	if( n ) {
		m_Btb.bninp = bmcnt;
		m_Btb.page_max = 1;
		m_Btb.page_now = 1;
		m_Btb.page_recn = 1;
		m_Btb.rec_n = n;
	}
	else {
		m_Btb.bninp = 0; 
		m_Btb.page_max = 0;
		m_Btb.page_now = 0;
		m_Btb.page_recn = 0;
		m_Btb.rec_n = 0;		
	}
		
	m_BMNSEL_MOD = 1;
	
	return 0;

BMERR:
	m_BMNSEL_MOD = -1;

	return -1;
	
}


// 部門の表示ページの情報をセット
// int page ... 0 から始まる
// int type ... 0 = 部門, 1 = 工事
BOOL CDipZmSub::getBmnSel( BMNSEL_TB **tbl, int page_sz, int page, int type/*=0*/ )
{
	int ofs;
	BMNSEL_TB *ptbl;
	BOOL ret = FALSE;

//ASSERT( (zvol->apno != 0x20 && type != 1) );

	if( zvol->apno == 0x20 && type )	// 作成ずみテーブル
		ptbl = &m_BtbKOJI;
	else
		ptbl = &m_Btb;

	if( m_BMNSEL_MOD > 0 )
	{
		// 該当ページテーブル(m_BRN_PAGE)作成
		if( page_sz <= 0 )	// All Table
		{
			page_sz = ptbl->rec_n;
		}
		m_Btb_PAGE.page_max = (ptbl->rec_n + page_sz - 1) / page_sz;	// 最大ページ

		if( m_Btb_PAGE.page_max > 0 ) {
			if( page < 0 )	// アンダーフロー
				page = 0;
			else if( page >= m_Btb_PAGE.page_max )	// オーバーフロー
				page = m_Btb_PAGE.page_max - 1;
		}
		else	page = 0;

		ofs = page * page_sz;

		m_Btb_PAGE.bninp = ptbl->bninp;
		m_Btb_PAGE.rec_n = (ofs + page_sz < ptbl->rec_n ? page_sz : ptbl->rec_n - ofs); 
		m_Btb_PAGE.pBSEL_REC = ptbl->pBSEL_REC + ofs;

		m_Btb_PAGE.page_recn = page_sz;	// 1ページ分のレコード数
		m_Btb_PAGE.page_now = page + 1;	// 現在ページ( 1 から始まる)
	
	//	m_BRN_PAGE.ForeCol = ptbl->ForeCol;	// 表示文字色

		ret = TRUE;
	}
	*tbl = &m_Btb_PAGE;

	return ret;
}


// 部門コードを取得
// 
BOOL CDipZmSub::get_bmncod( int* pbmn, int number )
{
	BOOL ret = FALSE;

	if( number > 0 && number <= m_Btb_PAGE.rec_n ) {
		*pbmn = atoi( (m_Btb_PAGE.pBSEL_REC + (number-1))->bkcod );
		ret = TRUE;
	}

	return ret;
}


// 工事コードを取得
// 
BOOL CDipZmSub::get_kojicod( CString* koji, int number )
{
	BOOL ret = FALSE;

	if( number > 0 && number <= m_Btb_PAGE.rec_n ) {
		*koji = (m_Btb_PAGE.pBSEL_REC + (number-1))->bkcod;
		ret = TRUE;
	}

	return ret;
}


// 現在ページをセット
//
void CDipZmSub::set_bmnnowpg( WORD page )
{
	if( m_BMNSEL_MOD > 0 )
	{
		if( m_Btb_PAGE.page_max > 0 ) {
			if( page <= m_Btb_PAGE.page_max ) 
				m_Btb_PAGE.page_now = page;
		}
		else
			m_Btb_PAGE.page_now = 0;

	}
}


// 部門名称を取得
LPCTSTR CDipZmSub::get_bmnmsg( int* bmn, BOOL full, int mode )
{
	BMNSEL_TB *ptbl;
	int i, cnt;
	static CString str;
	str.Empty();
	LPCTSTR rval = NULL;

	ptbl = &m_Btb;	// 作成ずみテーブル
	cnt = zvol->bmcol;

	if( m_BMNSEL_MOD > 0 )
	{
//		if( mode & NOMALBRN_TEK )
		if( mode )
		{
			for( i = 0; i < ptbl->bninp; i++ ) {
				if( *bmn == atoi( (ptbl->pBSEL_REC+i)->bkcod ) ) {

					str.Format( "%s", (ptbl->pBSEL_REC+i)->s_txt.Mid( cnt ) );
					break;
				}
			}
		}
	}
	if( !str.IsEmpty() ) {
		// ALL スペースチェック ... 11.06 /02
		if( kjlen( (void*)(LPCTSTR)str, str.GetLength() ) )
			rval = (LPCTSTR)str;
	}

	return (LPCTSTR)rval;
}


// 工事名称を取得
LPCTSTR CDipZmSub::get_kojimsg( CString* koji, BOOL full, int mode )
{
	BMNSEL_TB *ptbl;
	int i, cnt;
	static CString str;
	str.Empty();
	LPCTSTR rval = NULL;

	ptbl = &m_BtbKOJI;// 作成ずみテーブル
	cnt = zvol->kjcol;

	if( m_BMNSEL_MOD > 0 )
	{
//		if( mode & NOMALBRN_TEK )
		if( mode )
		{
			for( i = 0; i < ptbl->bninp; i++ ) {
				if( *koji == (ptbl->pBSEL_REC+i)->bkcod ) {

					str.Format( "%s", (ptbl->pBSEL_REC+i)->s_txt.Mid( cnt ) );
					break;
				}
			}
		}
	}
	if( !str.IsEmpty() ) {
		// ALL スペースチェック ... 11.06 /02
		if( kjlen( (void*)(LPCTSTR)str, str.GetLength() ) )
			rval = (LPCTSTR)str;
	}

	return (LPCTSTR)rval;
}

#endif


// 合計部門かどうか？
BOOL CDipZmSub::IsGokeiBumon( int bmn )
{
	if( ! bmname )	return FALSE;

	BOOL bGk = FALSE;

	//社会福祉改正マスターは合計部門入力可
	if( IsSyafuKaisei( zvol ) && m_bNPanbn ) {
		return FALSE;
	}

	bmname->MoveFirst();

	do {
		if( bmname->st == 0 ) {
			if( bmname->bnbcd == bmn ) {
				if( (bmname->bnhsg[0] & 0x02) ) bGk = TRUE;
				
				break;
			}
		}
		else	break;
	} while ( bmname->MoveNext() == 0 ) ;

	return bGk;
}

// 部門／工事コードが 登録されたものか？
BOOL CDipZmSub::bmncode_ok( int bmn )
{
	if( ! bmname ) return FALSE;

	BOOL ret = FALSE;
	bmname->MoveFirst();

	do {
		if( bmn == bmname->bnbcd ) {
			//社会福祉改正マスターは合計部門入力可
			if( IsSyafuKaisei( zvol ) && m_bNPanbn ) {
				ret = TRUE;
			}
			else {
				if( ! (bmname->bnhsg[0] & 0x02) ) {
					ret = TRUE;
				}
			}
			break;
		}
	} while( bmname->MoveNext() == 0 );

	return ret;
}


// 日付の入力範囲取得
void CDipZmSub::get_datelimit( int& sofs, int& eofs, int& sy, int& ey )
{
	 sofs	= m_inpSofs;
	 eofs	= m_inpEofs;
	 sy		= m_inpSy;
	 ey		= m_inpEy;
}

// 日付の入力範囲セット
void CDipZmSub::set_datelimit( int sofs, int eofs, int sy, int ey )
{
	m_inpSofs	= sofs;
	m_inpEofs	= eofs;
	m_inpSy		= sy;
	m_inpEy		= ey;
}

// 入力日付が入力範囲内にあるか？
BOOL CDipZmSub::check_datelimit( BYTE* bcddate )
{
	if( m_inpSofs == -1 )
		return TRUE;

	CVolDateDB vd;
	int ofset;

//MyTrace( "::check_datelimit %02x%02x\n", bcddate[0], bcddate[1] );

	if( vd.db_vd_offset( bcddate, &ofset, this ) != 0 )
		return FALSE;

//MyTrace( "::check_datelimit ofs = %d, m_inpSofs %d, m_inpSofs %d\n", ofset, m_inpSofs, m_inpEofs );

	if( ofset > m_inpSofs && ofset < m_inpEofs )
		return TRUE;

	int arg_date, date, tmp, d_gengo;
	char buf[64];
	VDPAC vpac;

	if( bcddate[1] != (BYTE)0xff )
		sprintf_s( buf, sizeof buf, "%02x%02x", bcddate[0], bcddate[1] );
	else
		sprintf_s( buf, sizeof buf, "%02x00", bcddate[0] );

	tmp = atoi( buf );

	//MyTrace( "::check_datelimit tmp = %d, m_inpSy = %d, m_inpEy = %d\n", tmp, m_inpSy, m_inpEy );

	if( ofset == m_inpSofs ) {
		// 開始日付と入力日付をチェック
		if( vd.db_vd_dpacGen( 1, 0, tmp, &vpac, this ) != 0 )
			return FALSE;

		vd.db_datecnvGen( vpac.Dgengo, vpac.Dymd, &d_gengo, &arg_date, 1, 0 );

#ifdef CLOSE
		if( vd.db_vd_dpacGen( 1, 0,  m_inpSy, &vpac, this ) != 0 )
			return FALSE;
		vd.db_datecnvGen( vpac.Dgengo, vpac.Dymd, &d_gengo, &date, 1, 0 );
#endif
		date = m_inpSy;

		if( arg_date >= date )
			return TRUE;
	}
	else if( ofset == m_inpEofs ) {
		// 終了日付と入力日付をチェック
		if( vd.db_vd_dpacGen( 1, 0, tmp, &vpac, this ) != 0 )
			return FALSE;

		vd.db_datecnvGen( vpac.Dgengo, vpac.Dymd, &d_gengo, &arg_date, 1, 0 );

#ifdef CLOSE
		if( vd.db_vd_dpacGen( 1, 0,  m_inpEy, &vpac, this ) != 0 )
			return FALSE;
		vd.db_datecnvGen( vpac.Dgengo, vpac.Dymd, &d_gengo, &date, 1, 0 );
#endif
		date = m_inpEy;

		if( arg_date <= date )
			return TRUE;
	}

	return FALSE;
}


// 指定オフセットの日付を BCDで返送
//	int ofs			オフセット
//	int sgn			0 = 開始日, 1 = 終了日
//	BYTE* bcddate 
//
void CDipZmSub::get_ofsdate( int ofs, int sgn, BYTE* bcddate )
{
	CDBINPDataRec tmp;
	BYTE check[10] = {0};
	int flg = 0;

	minfo->MoveFirst();
	do {
		if( minfo->ofset == ofs ) {
			tmp.m_ddate	= (sgn == 0) ? minfo->smd : minfo->emd;
			tmp.m_mofs	= minfo->ofset;
			tmp.m_ksign = minfo->kes_sgn ? 1 : 0;
			flg = 1;
			break;
		}
	} while( minfo->MoveNext() == 0 );

	if( flg ) {
		// 入力開始より 前の日
		if( tmp.m_ddate < zvol->ip_ymd ) {
			CVolDateDB vd;
			int date, ofset;
			date = (zvol->ip_ymd%10000);
		
			if( vd.db_vd_offset( 0, date, &ofset, this ) != -1 ) {
				if( ofset == ofs ) {
					tmp.m_ddate = zvol->ip_ymd;
				}
				else {
					tmp.m_ddate = 0;
				}
			}
			else {
				tmp.m_ddate = 0;
			}
		}

		if( tmp.m_ddate > 0 ) {
			DateConv( check, &tmp, CV2_1 );
			memcpy( bcddate, check, 2 );
		}
	}
}


// 指定オフセットの日付を BCDで返送(決算修正は 末日)
//	int ofs			オフセット
//	BYTE* bcddate 
//
void CDipZmSub::get_ofsdate( int ofs, BYTE* bcddate )
{
	CDBINPDataRec tmp;
	BYTE check[10] = {0};
	int flg = 0;

	minfo->MoveFirst();
	do {
		if( minfo->ofset == ofs ) {
			tmp.m_mofs	= minfo->ofset;
			tmp.m_ksign = minfo->kes_sgn ? 1 : 0;
		
			if(tmp.m_ksign == 0) {
				if( zvol->ip_ymd > minfo->smd )
					tmp.m_ddate	= zvol->ip_ymd;
				else
					tmp.m_ddate = minfo->smd;
			}
			else {
				tmp.m_ddate	= minfo->emd;
			}

			flg = 1;
			break;
		}
	} while( minfo->MoveNext() == 0 );

	if( flg ) {
		DateConv( check, &tmp, CV2_1 );
		memcpy( bcddate, check, 2 );
	}
}


// 入力用 BCD 日付を作成
void CDipZmSub::MakeInpDate( CDBINPDataRec* rec )
{
	if( zvol == NULL )	return;

	if( m_inpSofs == -1 )	return;

	BYTE tmp[4] = {0}; 
	BYTE check[4] = {0};

	DateConv( tmp, rec, CV2_1 );
	int ksgn = KESY_SG_GET( tmp[0] );

	switch( ksgn ) {
	case 1:	check[0] = 0x61;	break;
	case 2:	check[0] = 0x62;	break;
	case 3: check[0] = 0x63;	break;
	case 4:	check[0] = 0x00;	break;
	default:
		check[0] = tmp[0];
		break;
	}
	check[1] = tmp[1];

	if( ! check_datelimit( check ) ) {
		get_ofsdate( m_inpEofs, m_InpBcd );
	}
	else {
		memcpy( m_InpBcd, tmp, 3 );
	}
}

// 入力用 BCD 日付を仕訳レコードにセット
void CDipZmSub::GetInpDate( CDBINPDataRec* rec )
{
	if( ! m_InpBcd[0] )
		return;

	DateConv( m_InpBcd, rec, CV1_2 );
}

// 入力用 BCD 日付を取得
void CDipZmSub::GetInpDate( BYTE* bcddate )
{
	memcpy( bcddate, m_InpBcd, 3 );
}

// 入力用 BCD 日付をセット
void CDipZmSub::SetInpDate( BYTE* bcddate )
{
	memcpy( m_InpBcd, bcddate, 3 );
}

// 変動事由コードより名称を取得
void CDipZmSub::get_hjiyustr( BYTE h_code, CString& str )
{
	kshnam->MoveFirst();

	do {
		if( kshnam->kofs > 0 )
			break;

		if( kshnam->hnjcod == h_code && (kshnam->hnpflg & 0x01) ) {
			str = kshnam->hnname;
			break;
		}
	} while( kshnam->MoveNext() == 0 );
}


//------------------------------------------------------------
//	科目が消費税科目か？
//
//	return	0以外	消費税科目(1=不課税, 2=課税, 3=非課税)
//					D8  ON: 棚卸し科目
//					D9  ON: 仮受・仮払消費税
//					D10 ON: 未成工事支出金(未成工事タイプのみ完成工事振替)
//			0		消費税科目でない
//------------------------------------------------------------
int CDipZmSub::km_syattr( DBKNREC* pKn )
{
	BYTE	attr[10] = {0};
	int retn;

	memmove( &attr, &pKn->knsgn, sizeof(pKn->knsgn) );

	if( attr[0] && attr[2] & 0x0f )
			retn = attr[2] & 0x0f;
	else	retn = 0;

	if( retn == 0 ) {
		if( pKn->stock ) {
			retn |= 0x100;
		}

		DWORD dwCode;
		dwCode = pKn->kncod;
		hl_rev( &dwCode, 4 );
		// 仮受・仮払
	//	if( dwCode == 0x01071101 || dwCode == 0x04051601 )
	//		retn |= 0x0200;

		// 06.27 /12
		DWORD svkcd1, svkcd2;
		svkcd1 = _atoh( (LPSTR)(LPCTSTR)szvol->SVkcod1 );
		svkcd2 = _atoh( (LPSTR)(LPCTSTR)szvol->SVkcod2 );

		if( (dwCode == svkcd1 || dwCode == svkcd2) || 
			((dwCode&0x80) && (dwCode & 0xffffff00) == (svkcd1 & 0xffffff00)) ||
			((dwCode&0x80) && (dwCode & 0xffffff00) == (svkcd2 & 0xffffff00)) ) {
		//if( (dwCode == svkcd1 || dwCode == svkcd2) ) {
			retn |= 0x0200;
		}
		else {
			//個人兼業
			if( zvol->apno == 0x10 && (zvol->ind_table&0x01) ) {
				if( dwCode == 0x01071110 || dwCode == 0x01071120 || dwCode == 0x04051610 || dwCode == 0x04051620 )
					retn |= 0x0200;
			}
			if( zvol->apno == 0x20 && M_MISEIKOJI_TYPE ) {
				if( (dwCode & 0xffffff00) == 0x01060200 ) {
					retn |= 0x0400;
				}
			}
		}
	}

	return( retn );
}

// 消費税 仮払・仮受 コード
void CDipZmSub::SyzKariCode( CString& kbarai, CString& kuke )
{
	kbarai	= szvol->SVkcod1;
	kuke	= szvol->SVkcod2;
}


//------------------------------------------------------------
//	科目が消費税科目か？(同上)
//------------------------------------------------------------
int CDipZmSub::km_syattr( CString& kcode )
{
	DBKNREC* pKn;
	pKn = DB_PjisToKnrec( kcode, TRUE );
	int ret = 0;

	if( pKn ) {
		ret = km_syattr( pKn );
	}
	return ret;
}


//------------------------------------------------------------
//	科目が輸出売上科目か？
//------------------------------------------------------------
int CDipZmSub::km_yusyutsu(DBKNREC* pKn)
{
	int ret = 0;

	if( pKn->knsgn[0] == 0x02 && pKn->knsgn[1] == 0x09 )
		ret = 1;

	return ret;
}

//------------------------------------------------------------
//	科目が輸出売上科目か？
//------------------------------------------------------------
int CDipZmSub::km_yusyutsu(CString& kcode)
{
	DBKNREC* pKn;
	pKn = DB_PjisToKnrec(kcode, TRUE);
	int ret = 0;

	if( pKn ) {
		ret = km_yusyutsu(pKn);
	}
	return ret;
}


//------------------------------------------------------------
//	科目の税区分を取得する
//
//	return	0	内税
//			1	外税
//		   -1	対象外
//------------------------------------------------------------
int CDipZmSub::km_zeikbn( DBKNREC* pKn )
{
	BYTE	attr[10] = {0};
	int retn = -1;

	memmove( &attr, &pKn->knsgn, sizeof(pKn->knsgn) );

	BYTE	chkBit = (attr[0]&0x0f);

	if( chkBit != 0 ) {
		//資産・売上・仕入・経費が対象
		if( (zvol->s_sgn7 & chkBit) )	retn = 1;	//外税
		else							retn = 0;	//内税
	}
	return retn;
}

//同上
int CDipZmSub::km_zeikbn( CString& kcode )
{
	DBKNREC* pKn;
	pKn = DB_PjisToKnrec( kcode, TRUE );
	int ret = 0;

	if( pKn ) {
		ret = km_zeikbn( pKn );
	}
	return ret;
}


//------------------------------------------------------------
//	科目の税率を取得する
//
//	return	0=既定税率
//			1=３%
//			2=０%
//			3=５％
//			4=８％
//			5=８％（軽減税率）
//			6=１０％
//------------------------------------------------------------
int CDipZmSub::km_zeiritsu( DBKNREC* pKn )
{
	BYTE	attr[10] = {0};

	memmove( &attr, &pKn->knsgn, sizeof(pKn->knsgn) );

	int	ritsu = ((attr[3]&0xf0)>>4);

//MyTrace( "km_zeiritsu %08x, ritsu = %d\n", pKn->kncod, ritsu );

	return ritsu;
}

//同上
int CDipZmSub::km_zeiritsu( CString& kcode )
{
	DBKNREC* pKn;
	pKn = DB_PjisToKnrec( kcode, TRUE );
	int ret = 0;

	if( pKn ) {
		ret = km_zeiritsu( pKn );
	}
	return ret;
}


//------------------------------------------------------------
//	仕訳オープンモード を返送
//	return -2:	全ユーザ
//	return  0:	カレント
//------------------------------------------------------------
int CDipZmSub::data_openmode()
{
	int mode = 0;

	if( (user_att & 0x11) )
		mode = -2;

	return mode;
}

//------------------------------------------------------------
//	仕訳権限 を返送
//	return  1:	他のユーザの仕訳も閲覧可
//	return  0:	自ユーザの仕訳のみ
//------------------------------------------------------------
int CDipZmSub::data_authority()
{
	int mode = 0;

	if( (user_att & 0x11) )
		mode = 1;

	return mode;
}


//------------------------------------------------------------
//	残高閲覧の権限 を返送
//	return  1:	残高閲覧可能
//			2:	閲覧可能だか、利益金額は不可
//			0:	残高閲覧不可
//------------------------------------------------------------
int CDipZmSub::zanview_authority()
{
	int mode = 0;

#ifdef CLOSE	// 11.18 /11
//	if( user_att == 0x08 || user_att == 0x10 ) {
	if( (user_att & 0x08) || (user_att & 0x10) ) {
#else
	int uat = (user_att&0xff);
	if( uat == 0x08 || uat == 0x10 ) {
#endif
		mode = 0;
	}
	else {
		if( (user_att & 0x01) || (user_att & 0x04) ) {
			mode = 1;
		}
		else if( (user_att & 0x02) ) {
			mode = 2;
		}
	}

	return mode;
}


//------------------------------------------------------------
//	入力日付をチェックする	[09.06 /07]
//
//	BYTE* orgdate:	変更前の日付[BCD]
//	char* check:	入力月日 文字列	
//
//	return -1:	エラー
//		    0:	OK
//------------------------------------------------------------
int CDipZmSub::inpdate_shimechk( BYTE* orgdate, char* check )
{
	BYTE	bcd[10] = {0};
	char	buf[36];
	CVolDateDB vd;
	int sgn, ofst, newofst, st;
	int smd, emd, inp_day, s_day, e_day;
	int sofs, eofs, sy, ey;

	get_datelimit( sofs, eofs, sy, ey );
	// 複数月は、チェックせず
	if( sofs != eofs )
		return 0;

	sgn = KESY_SG_GET( orgdate[0] );
	switch( sgn ) {
	case 1 :
		bcd[0] = 0x61;	break;
	case 2 :
		bcd[0] = 0x62;	break;
	case 3 :
		bcd[0] = 0x63;	break;
	case 4 :
		bcd[0] = 0x00;	break;
	default:
		bcd[0] = orgdate[0];
		break;
	}
	bcd[1] = orgdate[1];

	if( vd.db_vd_offset( bcd, &ofst, this ) != 0 )
		return -1;

	CArith ar;
	buf[0] = check[2];
	buf[1] = check[3];
	buf[2] = '\0';

	ar.l_pack( &bcd[1], buf, 2 );
	inp_day = atoi( buf );

	st = vd.db_vd_check( bcd, this );

	if( st == 0 ) {
		if( vd.db_vd_offset( bcd, &newofst, this ) != 0 )
			return -1;

		if( ofst == newofst )
			return 0;
	}

	// 締日の関係上、日入力で月オフセットが替わった場合、または、
	// 入力日付が不正な場合[ 0230 など]
	smd = emd = 0;
	minfo->MoveFirst();
	do {
		if( minfo->ofset == ofst ) {
			smd = minfo->smd;
			emd = minfo->emd;
			break;
		}
	} while( minfo->MoveNext() == 0 );

	if( smd == 0 || emd == 0 )
		return -1;

	s_day = (smd %100);
	e_day = (emd %100);

	if( inp_day < s_day ) {
		// 終了月をセット
		sprintf_s( buf, sizeof buf, "%02d", (emd %10000) /100 ); 
		if( ! sgn ) {
			check[0] = buf[0];
			check[1] = buf[1];
		}
	}
	else if( inp_day > e_day ) {
		// 開始月をセット
		sprintf_s( buf, sizeof buf, "%02d", (smd %10000) /100 ); 
		if( ! sgn ) {
			check[0] = buf[0];
			check[1] = buf[1];
		}
	}

	return 0;
}

// 簡易課税か個別対応か？
int CDipZmSub::IsKaniOrKobetsu()
{
	int ret = 0;

	if( IsKaniSyz() || IsKobetsuSyz() )
		ret++;

	return ret;
}


// 簡易課税か個別対応か？
int CDipZmSub::IsKaniSyz()
{
	int ret = 0;

	if( (zvol->s_sgn2 & 0x0f) >= 2 )
		ret++;

	return ret;
}

// 免税？
int CDipZmSub::IsMenzeiSyz()
{
	int ret = 0;

	if( (zvol->s_sgn2 & 0x0f) == 1 )
		ret++;

	return ret;
}


int CDipZmSub::IsKobetsuSyz()
{
	int ret = 0;

	if( (zvol->s_sgn4 & 0x80) )
		ret++;

	return ret;
}


// 変動事由・振替科目かの 名称を取得
//
//	strtype		0:	標準名称
//				1:	短縮名称
//				2:	長い名称
//
LPCTSTR CDipZmSub::get_hjiyutitle( int strtype )
{
	static const char* t1 = _T("変事");
	static const char* t2 = _T("振科");
	static const char* t3 = _T("内部");
	static const char* t4 = _T("資調");

	static const char* l_t1 = _T("変動事由");
	static const char* l_t2 = _T("振替科目");
	static const char* l_t3 = _T("内部取引");
	static const char* l_t4 = _T("資金調整");

	static const char* s_t1 = _T("変");
	static const char* s_t2 = _T("振");
	static const char* s_t3 = _T("内");
	static const char* s_t4 = _T("資");

	LPCTSTR p;

	if( strtype == 2 ) {
		if( zvol->apno < 0x50 ) {
			p = l_t1;
		}
		else {
			if( IsSyafuKaisei( zvol ) ) {
				p = l_t3;
			}
			else if( IsGakkouH27Master( this ) ) {
				p = l_t4;
			}
			else {
				p = l_t2;
			}
		}
	}
	else {
		if( zvol->apno < 0x50 ) {
			p = strtype ? s_t1 : t1;
		}
		else {
			if( IsSyafuKaisei( zvol ) ) {
				p = strtype ? s_t3 : t3;
			}
			else if( IsGakkouH27Master( this ) ) {
				p = strtype ? s_t4 : t4;;
			}
			else {
				p = strtype ? s_t2 : t2;
			}
		}
	}

	return p;
}



// 拡張合計名称の 元合計名称取得
DBGKREC* CDipZmSub::get_gksrc( DWORD gkcode )
{
	DWORD dwCode, tmp;
	DBGKREC* pGk;

	do {
		pGk = this->PjisToGkrec( gkcode );

		if( pGk ) {
			tmp = pGk->gksrc;
			hl_rev( &tmp, 4 );
			
			dwCode = (tmp & 0x0000f000);
			if( (dwCode & 0x00008000) <= 0 ) 
				break;
			else {
				// まだ、追加小分類
				gkcode = pGk->gksrc;
				pGk = NULL;
			}
		}
		else {
			pGk = NULL;
			break;
		}
	} while( 1 );

	// 追加基本小分類
	return pGk;
}

// 仕訳コピー後の処理
int CDipZmSub::SwkCopyAfterJob()
{
	int ret = 0;

	// マスター全体確定
	if( ! (zvol->tl_cor & 0x10) || IsJzSansyo() )
		return 0;

	CString filter;
	// 仕訳数セット
	filter = _T( "seq = 1" );
	dcntl->Requery( filter );
	if( dcntl->st != -1 ) {
		m_dacnt	= dcntl->cnt;
	}

	filter.Format( "seq = %d", m_dacnt );

	dbdata->Requery( -2, 0, filter, 0, 0 );
	// 次回表示のため、リクエリ が必要
	dbdata_Requery() = TRUE;

	// 最終書き込みデータを保存
	GetCDBData( &m_lastdata );
	m_lastdata.m_seq++;		// 次データ用にSEQセット
	
	MakeInpDate( &m_lastdata );

	// 追加修正サイン
	IsModify() = TRUE;
	// 科目残高 クエリーサイン
	kzrec_Requery() = TRUE;

	return ret;

}

//
//　月指定バーのUCOM用文字列作成
//		char*	sbf			開始日付格納バッファ
//		int		sz_start	　　　　〃　　　　　サイズ
//		char*	ebf			終了日付格納バッファ
//		int		sz_end		　　　　〃　　　　　サイズ
//
//
int CDipZmSub::MakeUcomSeldate( char* sbf, int sz_start, char* ebf, int sz_end )
{
	int sofs, eofs, sdate, edate;
	int yy, dd, sgn, ofst;
	char* pbuf;
	int   pbufsize;
	
	get_datelimit( sofs, eofs, sdate, edate );

	for( int i = 0; i < 2; i++ ) {

		if( i == 0 ) {
			ofst = sofs;
			pbuf = sbf;
			pbufsize = sz_start;
		}
		else {
			ofst = eofs;
			pbuf = ebf;
			pbufsize = sz_end;
		}

		// 決修サイン取得
		minfo->MoveFirst();
		do {
			if( minfo->ofset == ofst ) {
				sgn = minfo->kes_sgn;
				break;
			}
		} while( minfo->MoveNext() == 0 );

		if( sgn != 0 ) {
			if( i == 0 ) {
				yy = sdate / 10000;
				dd = sdate % 100;
			}
			else {
				yy = edate / 10000;
				dd = edate % 100;
			}
			switch( sgn ) {
			case 1:	sprintf_s( pbuf, pbufsize, "%04d61%02d", yy, dd );	break;
			case 2:	sprintf_s( pbuf, pbufsize, "%04d62%02d", yy, dd );	break;
			case 3:	sprintf_s( pbuf, pbufsize, "%04d63%02d", yy, dd );	break;
			case 4:	sprintf_s( pbuf, pbufsize, "%04d00%02d", yy, dd );	break;
			}
		}
		else {
			if( i == 0 ) {
				yy = sdate;
			}
			else {
				yy = edate;
			}
			sprintf_s( pbuf, pbufsize, "%d", yy );
		}
	}
//MyTrace( "@-->CDipZmSub::MakeUcomSeldate GetMonthOffset %s, %s\n", sbf, ebf );

	return 0;
}


// 社会福祉 按分の登録があるか？
void CDipZmSub::CheckRegisterNphw()
{
	m_bNPanbn = FALSE;

	if( IsSyafuKaiseiAnbun( this ) ) {
		m_bNPanbn = TRUE;
	}

}

// 仮受・仮払 の枝番が自動で増えたかのチェック用
int CDipZmSub::GetKariAutoBrnCnt()
{
	int edcnt = 0;

if ((zvol->s_sgn1 & 0xf0) != 0 && !(zvol->s_sgn4 & 0x10)) {
	CString filter;
	filter = _T("seq = 3");
	dcntl->Requery(filter);
	if (dcntl->st != -1) {
		edcnt = dcntl->cnt;
	}
}

return edcnt;
}


// 科目内部コードが不正かどうかチェックする
int CDipZmSub::KamokuCodeCheck(CString& kmcode)
{
	int len;

	//MyTrace( "KamokuCodeCheck TOP kmcode = %s\n", kmcode );

	len = kmcode.GetLength();

	//先頭に '0' が 
	if (len == 7) {
		CString stmp;
		stmp.Format("0%s", kmcode);

		DWORD dwCode;
		dwCode = _atoh((char*)(LPCTSTR)stmp);
		hl_rev(&dwCode, 4);

		//MyTrace( "KamokuCodeCheck len(%d), stmp = %s, dwCode = %08x\n", len, stmp, dwCode );

		DBKNREC* pKn;
		pKn = PjisToKnrec(dwCode);
		if (pKn) {
			stmp.TrimRight();
			kmcode = stmp;
		}
	}

	//MyTrace( "KamokuCodeCheck END kmcode = %s\n", kmcode );

	return 0;
}


// 現在仕訳数をチェック
void CDipZmSub::CheckSiwakeCount()
{
	CString filter;
	filter = _T("seq = 1");
	dcntl->Requery(filter);

	if (dcntl->st != -1) {
		if (m_dacnt != dcntl->cnt) {
			insseq_make() = FALSE;
			m_dacnt = dcntl->cnt;
		}
	}
}

//
// 科目と枝番残高で消費税の設定が異なるか？
BOOL CDipZmSub::IsDiffEdabanSyzSgn(char* kmkcd, int brn)
{
	CString filter, buf;
	BOOL bret = FALSE;

	//枝番消費税登録サイン ＯＦＦ
	if (!(zvol->s_sgn2 & 0x10))
		return FALSE;

	ezdb_init(this);
	if (ezrec->Find((LPCTSTR)kmkcd, brn) > 0) {

		DWORD tmp;
		tmp = _atoh(kmkcd);
		hl_rev(&tmp, 4);

		DBKNREC* pKn;
		pKn = PjisToKnrec(tmp);

		if (pKn) {
			if (memcmp(&pKn->knsgn[2], &ezrec->ezsysgn[2], 3) != 0) {
				bret = TRUE;
			}
		}
	}
	return bret;
}


// 個別対応で、部門消費税登録あり？
int CDipZmSub::IsKobetsuBmnSyz()
{
	int ret = 0;

	if (zvol->s_sgn4 & 0x80) {
		if ( (zvol->sub_sw & 0x02) && (zvol->s_sgn2 & 0x20) ) {
			ret = 1;
		}
	}

	return ret;
}



void BmnNameDataInit(BMNAME_DATA* bd)
{
	bd->bnbnm.Empty();

	::ZeroMemory(bd->bnhsg, sizeof bd->bnhsg);
	bd->dmy = 0;
	::ZeroMemory(bd->bnhw, sizeof bd->bnhw);

	bd->prbcd = 0;
	bd->klevel = 0;
	bd->pseq = 0;
	bd->dseq = 0;

	bd->bnsysgn = 0;
}


// 部門名称データ map 作成
int CDipZmSub::MakeBmnNameMap()
{
	m_bmnMap.clear();

	if (!(zvol->sub_sw & 0x02)) {
		return 0;
	}
	
	bmname->Requery("", "");

	BMNAME_DATA bd;
	BmnNameDataInit(&bd);

	if (bmname->MoveFirst() == 0) {
		do {
			bd.bnbnm = bmname->bnbnm;

			memcpy(bd.bnhsg, bmname->bnhsg, sizeof bmname->bnhsg);
			memcpy(bd.bnhw, bmname->bnhw, sizeof bmname->bnhw);

			bd.prbcd = bmname->prbcd;
			bd.klevel = bmname->klevel;
			bd.pseq = bmname->pseq;
			bd.dseq = bmname->dseq;

			bd.bnsysgn = bmname->bnsysgn;

			m_bmnMap.insert(make_pair(bmname->bnbcd, bd));

		} while (bmname->MoveNext() == 0);
	}

	return 0;
}


DWORD CDipZmSub::KobetsuSiwakeCheck(CDBINPDataRec* rec, int bmn)
{
	DWORD dwCode = 0;
	BYTE	dsign;

	if (bmn == -1)	return 0;

	if ((rec->m_dsign[0] & 0x40) && (rec->m_dsign[2] & 0x0f) == 0x01) {
		dsign = (rec->m_dsign[3] & 0xf0);

//MyTrace("KobetsuSiwakeCheck dsign3 = %02x\n", dsign);

		if (dsign != 0) {
			dsign >>= 4;

			auto it = m_bmnMap.find(bmn);
			if (it != m_bmnMap.end()) {
				if (it->second.bnsysgn != 0) {
				//	if (dsign != it->second.bnsysgn) {
						switch (it->second.bnsysgn) {
						case 1:	dwCode = CD_KAURI;	break;
						case 2:	dwCode = CD_HIURI;	break;
						case 3:	dwCode = CD_KYOURI;	break;
						}
				//	}
				}
				else {
					switch ((zvol->s_sgn6 & 0x0f)) {
					case 1:	dwCode = CD_KAURI;	break;
					case 2:	dwCode = CD_HIURI;	break;
					case 3:	dwCode = CD_KYOURI;	break;
					}
				}
			}
			else {
				switch ((zvol->s_sgn6 & 0x0f)) {
				case 1:	dwCode = CD_KAURI;	break;
				case 2:	dwCode = CD_HIURI;	break;
				case 3:	dwCode = CD_KYOURI;	break;
				}
			}
		}
	}
//MyTrace("KobetsuSiwakeCheck ret(1) = %08x\n", dwCode);

	if (dwCode != 0)	return dwCode;
	else {
		if (zvol->apno < 0x50) {
			return 0;
		}
		//不課税の特定収入か？
		if ( IsTokuteiSyunyuData(rec ) ) {
			dsign = (rec->m_dsign[5] & 0xf0);

//MyTrace("KobetsuSiwakeCheck 不課税 dsign5 = %02x\n", dsign);

			if (dsign != 0) {
				dsign >>= 4;

				auto it = m_bmnMap.find(bmn);
				if (it != m_bmnMap.end()) {
					if (it->second.bnsysgn != 0) {
					//	if (dsign != it->second.bnsysgn) {
							switch (it->second.bnsysgn) {
							case 1:	dwCode = CD_TK_KAURI;	break;
							case 2:	dwCode = CD_TK_HIURI;	break;
							case 3:	dwCode = CD_TK_KYOURI;	break;
							}
					//	}
					}
					else {
						switch ((zvol->s_sgn6 & 0x0f)) {
						case 1:	dwCode = CD_TK_KAURI;	break;
						case 2:	dwCode = CD_TK_HIURI;	break;
						case 3:	dwCode = CD_TK_KYOURI;	break;
						}
					}
				}
				else {
					switch ((zvol->s_sgn6 & 0x0f)) {
					case 1:	dwCode = CD_TK_KAURI;	break;
					case 2:	dwCode = CD_TK_HIURI;	break;
					case 3:	dwCode = CD_TK_KYOURI;	break;
					}
				}
			}
		}
	}

//MyTrace("KobetsuSiwakeCheck  ret(2) = %08x\n", dwCode);

	return dwCode;
}


//不課税・特定収入の仕訳か？
BOOL CDipZmSub::IsTokuteiSyunyuData(CDBINPDataRec* rec)
{
	if (zvol->apno < 0x50) {
		return FALSE;
	}
	BOOL ret = FALSE;

	//不課税の特定収入か？
	if ((rec->m_dsign[0] & 0x40) && (rec->m_dsign[2] & 0x0f) == 0x02) {
		switch ((rec->m_dsign[5] & 0x0f)) {
		case 1:	case 4:
		case 6:	case 8:
		case 10:
			ret = TRUE;
			break;
		}
	}
	return ret;
}


//------------------------------------------------------------
//	科目が仕入科目か？
//
//	return  1	仕入科目である
//			0	仕入科目でない
//------------------------------------------------------------
int CDipZmSub::km_siire(DBKNREC* pKn)
{
	int sgn = sire_attr(pKn);

	return sgn;
}


extern
BOOL IsKaribarai(DWORD kncd);
extern
BOOL IsKariuke(DWORD kncd);

// 登録番号をセットできる仕訳か？
BOOL CDipZmSub::is_invnosiwake(CDBINPDataRec* prec)
{
	//
	BOOL bClear = FALSE;

	DBKNREC* pKn1, *pKn2;
	pKn1 = DB_PjisToKnrec(prec->m_dbt);
	pKn2 = DB_PjisToKnrec(prec->m_cre);
	int siire = 0;
	if( pKn1 && km_siire(pKn1) ) {
		siire++;
	}
	if( pKn2 && km_siire(pKn2) ) {
		siire++;
	}

	int kariba = 0;

	int tmp = 0;
	tmp = _atoh((char*)(LPCTSTR)prec->m_dbt);
	if( IsKaribarai(tmp) ) {
		kariba++;
	}

	tmp = _atoh((char*)(LPCTSTR)prec->m_cre);
	if( IsKaribarai(tmp) ) {
		kariba++;
	}

	if( kariba ) {
		if( !is_karibarai_menzei(&prec->m_dsign[0]) ) {
			bClear = TRUE;
		}
		else {
			return TRUE;
		}
		//switch( prec->m_dsign[4] ) { //仮払、以下の消費税区分は登録番号を付与しない
		//case 0x04: case 0x0A: case 0x0B: //輸入仕入、棚卸加算・減算
		//case 0x1C:						 //精算
		//case 0x20: case 0x21: case 0x22: //輸入(国税), 輸入(地方税), 輸入(国地合算),
		//	bClear = TRUE;
		//	break;
		//default:
		//	return TRUE;
		//}
	}
	else {
		if( (prec->m_dsign[0] & 0x40) && (prec->m_dsign[2] & 0x0f) == 0x01 ) {
			if( siire ) {
				//仕入科目の消費税区分ありはクリア
				if( prec->m_dsign[4] ) {
					bClear = TRUE;
				}
			}
			else {
				switch( prec->m_dsign[4] ) { //仕入でない科目で、以下の消費税区分は登録番号を付与
				case 0x17: case 0x15: //経費、仕入
				case 0x06: case 0x08: //販売奨励金収入、仕入割引
					bClear = FALSE;
					break;
				default:
					bClear = TRUE;
					break;
				}
			}
		}
		else {
			//非課税・不課税はクリア
			bClear = TRUE;
		}
	}

	// インボイス登録番号 セットできる仕訳でない
	if( bClear ) {
		::ZeroMemory(prec->m_invno, sizeof prec->m_invno);
	}

	return bClear ? FALSE : TRUE;
}

