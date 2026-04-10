/*=====================

 	data scan sub 
	scan.c

=======================*/
#include "stdafx.h"

//#include "zmgdtsub.h"

#include "DBDinp.h"

#include "sys\types.h"
#include "sys\stat.h"
#include "io.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
#include "scan.h"

#include "CStRecordSet.h"


//消費税モジュール
//extern CDBSyohi* pDBsy;


//
// 仕訳状態ビットテーブル
#include "bit.h"
extern CBit	MODEFY_BIT;		//修正仕訳ビットテーブル

#include "SEQ.h"
extern CSEQ	APPEND_SEQ;		//追加仕訳ＳＥＱ登録テーブル
extern CSEQ	SEQ_APPEND;		//追加仕訳ＳＥＱ登録テーブル管理テーブル
extern CSEQ	SEQ_SCAN;		//スキャン仕訳ＳＥＱ登録テーブル管理テーブル

extern CSEQ	SCMDFY_SEQ;
//extern CSEQ	SEQ_SCMDFY;
extern vector<DWORD>	SEQ_SCMDFY;
extern CSEQ	MDFYDATA_SEQ;

extern CSEQ	INSPOS_SEQ;
extern CSEQ	SEQ_INSPOS;


extern DB_SCANINFO	SCAN_PGTBL;
extern SCAN_ITEMEX	SC_Item;

//ICS ocx

#include "DataListCtrl.h"
#include "Dinpsub.h"


#ifdef LATER_CLOSE

// 貸借合計問合わせ
#include "TaiTtl.h"
extern CTaiTtl m_TaiTtl;
#include "TaiDsp.h"
extern CTaiDsp* m_pTaiDsp;

#endif


// end of scan
void endscan(void)
{
	inpmdisp( _APPEND );
}

extern void DbgDataLine();


void scroll_up( struct _DATA_LINE *ddata, int n, int ln )
{
	int start_pn, end_pn;
	int i;
	
	start_pn = 0;	//スクロール開始 DDATA_LINE ポジション
	end_pn = n - 1;	//スクロール終了 DDATA_LINE ポジション
	while( ln-- )	//スクロール行数
	{
		for( i = start_pn ; i < end_pn ; ++i )
		{
		//	memmove( (ddata + i), (ddata + i + 1), sizeof( struct _DATA_LINE) );
			*(ddata + i) = *(ddata + i + 1);
		}
		inpline_inz( ddata + i );
	}

//	DbgDataLine();
}

void scroll_down( struct _DATA_LINE *ddata, int n, int ln )
{
	int		start_pn, end_pn;
	int		i;

	start_pn = 0;	//スクロール開始 DDATA_LINE ポジション
	end_pn = n - 1;	//スクロール終了 DDATA_LINE ポジション
	while( ln-- )	//スクロール行数
	{
		for( i = end_pn ; i > start_pn ; --i )
		{
			*(ddata + i) = *(ddata + i - 1);
		}
		inpline_inz( ddata + i );
	}
}

/************************************

  日付順 SEQの順番 セット

*************************************/
//
// 日付順 SEQの順番( 1から始まる ) 
static int DATE_ORDER_NUM;

void set_order_num( int o_num ) 
{
	DATE_ORDER_NUM = o_num;
}

int get_order_num() 
{
	return DATE_ORDER_NUM;
}

/***************************************/


#ifndef INSERT_CLOSE	//仕訳挿入対応


//仕訳読み込み　本体のスレーブ（１仕訳読み込み）
//	key->m_seq には 検索開始の基準となる SEQ を指定する
//	
//	int nxt		キーとして指定した SEQ( key->m_seq )を含めないで次の仕訳を読み込む
//
static int _DBdata_read( int vect, CDBINPDataRec *key, CDBINPDataRec *data, int seq = 0, int nxt = 0 )
{
CDBINPDataRec dta;
dta.Reset();
int vect_n, i, j, n, rno, npos;
BOOL move_sgn	= FALSE;
BOOL first_sgn	= FALSE;
BOOL add_ok;

	vect_n = (vect < 0 ) ? -1 : 1;

	pDBzm->dbdata_Sort()	=	DBDATA_SORT_SEQ;

int SeqNum, search;
CString filter;

	SeqNum = key->m_seq;

	for( ; ; ) {
		if( ! pDBzm->insseq_make() ) {
			insseqpos_make( NULL );
		}

		if( ! first_sgn ) {
			if( nxt ) 	search = 1;
			else		search = 0;
		}
		else {
			search = 1;
		}

		if( search ) {
			if( vect < 0 ) {
				int pos = INSPOS_SEQ.ngetseq( SeqNum-1 );

				if( pos > 0 ) {
					pos--;	// ひとつ前へ
					SeqNum = SEQ_INSPOS.ngetseq( pos-1 );

					if( SeqNum > 0 ) {
				//		pDBzm->dbdata->SetAbsolutePosition( seq );
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
			}
			else {
				int pos = INSPOS_SEQ.ngetseq( SeqNum-1 );

				if( pos > 0 ) {
					pos++;	// ひとつ後へ
					SeqNum = SEQ_INSPOS.ngetseq( pos-1 );

					if( SeqNum > 0 ) {
				//		pDBzm->dbdata->SetAbsolutePosition( seq );
					}
					else {
						return -1;
					}
				}
				else {
					return -1;
				}
			}
		}

		if( pDBzm->dbdata_Requery() || pDBzm->dbdata_Job() != DBDATA_JOB_NORMAL ) {
			// 
			BOOL bOK = FALSE;

			if( pDBzm->dbdata_Speed() ) {
				pDBzm->dbdata->MoveFirst();

				if( pDBzm->dbdata->st != -1 ) {
					do {
						if( pDBzm->dbdata->seq == SeqNum ) {
							bOK = TRUE;
							break;
						}
					} while( pDBzm->dbdata->MoveNext() == 0 );
				}
			}

			if( ! bOK ) {
				pDBzm->dbdata->Requery( -2/*DOPEN_MODE*/, 0, filter, 0, 0 );
				pDBzm->dbdata_Speed() = FALSE;
				npos = SeqNum;
				pDBzm->dbdata->SetAbsolutePosition(npos);

				pDBzm->dbdata_Requery()	=	FALSE;
			}

			pDBzm->dbdata_Job()		=	DBDATA_JOB_NORMAL;
		}
		else if( pDBzm->dbdata->st == -1 && ! move_sgn ) {

			pDBzm->dbdata->MoveFirst();	// データがセットされている状態にする

			if( pDBzm->dbdata->st != -1 ) {
				npos = SeqNum;
				pDBzm->dbdata->SetAbsolutePosition(npos);
			}
			move_sgn = TRUE;
		}
		else {
		//	if( ! first_sgn ) {
				// CDBDATA を消費税のモジュールに渡すバッファとして使用しているため、
				// 現在のポジションと、dbdata->seq の 値が異なる場合のために
				// Move して、データを再読み込み
			//	pDBzm->dbdata->MoveNext();
			//	pDBzm->dbdata->MovePrev();
		//	}
			npos = SeqNum;
			pDBzm->dbdata->SetAbsolutePosition(npos);
		}

		if( SeqNum != pDBzm->dbdata->seq )
			return -1;

		pDBzm->GetCDBData( &dta );

		// データなしで bitncnd を呼ばない
		if( dta.m_seq <= 0 )
			return -1;

		// 今回の仕訳追加分かどうかを SEQ ﾃｰﾌﾞﾙよりチェック
		add_ok = TRUE;

		if( dta.m_seq > START_SEQ ) {
			n = dta.m_seq - 1;

			i = SEQ_APPEND.ngetseq( n );
			if( i < 0 )		add_ok = FALSE;
			else if( !i )	add_ok = FALSE;
			else {
				// 追加 SEQ のチェック
				n = i - 1;
				j = APPEND_SEQ.ngetseq( n );
				ASSERT( j == dta.m_seq );
			}
		}

		// 本プログラム中に発生した取消もＯＫとする
		i = MODEFY_BIT.bitncnd( dta.m_seq -1 );

		if( add_ok ) {
			if( !(dta.m_dsign[0] & 0x01) || i > 0 || (seq ? dta.m_seq == seq : FALSE ) )
			{
				*data = dta;
				rno = dta.m_seq -1;

				return rno;		//OK
			}
		}

		if( ! first_sgn )	first_sgn = TRUE;
	}

	return -1;
}

#else ///////////////////////////////////////////////////
//
//	処理速度向上のためのクエリー
//	int keyseq	このSEQ にレコードセット
//
static int _speed_query( int keyseq )
{
	CString filter;
	
	//開始時の全レコードをのせる
	filter.Format( "seq <= %d", pDBzm->m_dacnt );
	pDBzm->dbdata_Speed() = FALSE;

	pDBzm->dbdata->Requery( DOPEN_MODE, 0, filter, 0, 0 );

	if( pDBzm->dbdata->st == -1 )
		return -1;

	int x, xx, st, last = 0;
	x = (keyseq - pDBzm->dbdata->seq);

	if( x > 10000 ) {
		pDBzm->dbdata->MoveLast();
		xx = (keyseq - pDBzm->dbdata->seq);

		if( xx > x ) {
			pDBzm->dbdata->MoveFirst();
		}
		else {
			last = 1;
		}
	}

	st = -1;
	if( pDBzm->dbdata->st == -1 )
		return -1;

	while( 1 ) {
		if( last ) {	// 最後のデータより
			if( keyseq >= pDBzm->dbdata->seq ) {
				st = 0;
				break;
			}
			pDBzm->dbdata->MovePrev();
			if( pDBzm->dbdata->st == -1 )
				return -1;
		}
		else {			// 最初のデータより
			if( keyseq <= pDBzm->dbdata->seq ) {
				st = 0;
				break;
			}
			pDBzm->dbdata->MoveNext();
			if( pDBzm->dbdata->st == -1 )
				return -1;
		}
	}
	return st;
}


//仕訳読み込み　本体のスレーブ（１仕訳読み込み）
//	key->m_seq には 検索開始の基準となる SEQ を指定する
//	
//	int nxt		キーとして指定した SEQ( key->m_seq )を含めないで次の仕訳を読み込む
//
static int _DBdata_read( int vect, CDBINPDataRec *key, CDBINPDataRec *data, int seq = 0, int nxt = 0 )
{
CDBINPDataRec dta;
dta.Reset();
int vect_n, i, j, n, rno;
BOOL move_sgn	= FALSE;
BOOL first_sgn	= FALSE;
BOOL add_ok;

	vect_n = (vect < 0 ) ? -1 : 1;

	pDBzm->dbdata_Sort()	=	DBDATA_SORT_SEQ;

	for( ; ; )
	{
		CString filter;

		if( pDBzm->dbdata_Requery() || pDBzm->dbdata_Job() != DBDATA_JOB_NORMAL ) {
			int num = 500;
			if( (pDBzm->m_dacnt - num) > 1 ) {
				// 速度UPのため全データの読み込みはしない
				pDBzm->dbdata_Speed() = TRUE;
				filter.Format( "seq >= %d and seq <= %d", (pDBzm->m_dacnt - num), pDBzm->m_dacnt );
			}
			else {
				filter.Format( "seq <= %d", pDBzm->m_dacnt );
			}

			pDBzm->dbdata->Requery( DOPEN_MODE, 0, filter, 0, 0 );

			pDBzm->dbdata_Requery()	=	FALSE;
			pDBzm->dbdata_Job()		=	DBDATA_JOB_NORMAL;
		}
		else if( pDBzm->dbdata->st == -1 && ! move_sgn ) {
			pDBzm->dbdata->MoveFirst();	// データがセットされている状態にする

			if( pDBzm->dbdata->st != -1 ) {
				if( key->m_seq <= pDBzm->dbdata->seq && vect_n < 0 ) {
					if( pDBzm->dbdata_Speed() ) {
						filter.Format( "seq <= %d", pDBzm->m_dacnt );
						pDBzm->dbdata_Speed() = FALSE;

						pDBzm->dbdata->Requery( DOPEN_MODE, 0, filter, 0, 0 );
					}
				}
				// First Last 現データに近いほうへ移動
				int x, xx;
				x = (key->m_seq - pDBzm->dbdata->seq);

				if( x > 10000 ) {
					pDBzm->dbdata->MoveLast();
					xx = (key->m_seq - pDBzm->dbdata->seq);

					if( xx > x )	pDBzm->dbdata->MoveFirst();

				}
			}
			move_sgn = TRUE;
		}
		else {
			if( ! first_sgn ) {
				// CDBDATA を消費税のモジュールに渡すバッファとして使用しているため、
				// 現在のポジションと、dbdata->seq の 値が異なる場合のために
				// Move して、データを再読み込み
				pDBzm->dbdata->MoveNext();
				pDBzm->dbdata->MovePrev();
			}
		}

		if( ! first_sgn ) {
			// 最初の SEQまで検索
			int sgn = 0;
			do {

				if( pDBzm->dbdata->st == -1 ) {	//データなし
					if( pDBzm->dbdata_Speed() && vect_n < 0 ) {
						// 戻りのときのみ データをフルで読み込む
						if( _speed_query( key->m_seq ) != 0 )
							return -1;
					}
					else	return -1;
				}

				if( pDBzm->dbdata->seq == key->m_seq )
					break;
			
				if( pDBzm->dbdata->seq > key->m_seq ) {
					if( ! sgn )	sgn = -1;
					// 無限ループ防止
					if( sgn > 0 ) 	break;

					pDBzm->dbdata->MovePrev();
				}
				else {
					if( ! sgn )	sgn = 1;
					// 無限ループ防止
					if( sgn < 0 )	break;

					pDBzm->dbdata->MoveNext();
				}
			} while( 1 );

			if( pDBzm->dbdata->st == -1 )	//データなし
				return -1;

			first_sgn = TRUE;

			if( ! nxt ) goto GETDATA;
		}

		if( vect < 0 ) {
			pDBzm->dbdata->MovePrev();
		}
		else {
			pDBzm->dbdata->MoveNext();
		}

		if( pDBzm->dbdata->st == -1 ) {
			//データなし
			if( pDBzm->dbdata_Speed() && vect_n < 0 ) {
				// 戻りのときのみ データをフルで読み込む
				if( _speed_query( key->m_seq ) != 0 )
					return -1;
				// 一つ前に戻る
				pDBzm->dbdata->MovePrev();
				//データなし
				if( pDBzm->dbdata->st == -1 )
					return -1;
			}
			else {
				return -1;
			}
		}

GETDATA:
		pDBzm->GetCDBData( &dta );

		// データなしで bitncnd を呼ばない
		if( dta.m_seq <= 0 )
			return -1;

		// 今回の仕訳追加分かどうかを SEQ ﾃｰﾌﾞﾙよりチェック
		add_ok = TRUE;

		if( dta.m_seq > START_SEQ ) {
			n = dta.m_seq - 1;

			i = SEQ_APPEND.ngetseq( n );
			if( i < 0 )		add_ok = FALSE;
			else if( !i )	add_ok = FALSE;
			else {
				// 追加 SEQ のチェック
				n = i - 1;
				j = APPEND_SEQ.ngetseq( n );
				ASSERT( j == dta.m_seq );
			}
		}

		// 本プログラム中に発生した取消もＯＫとする
		i = MODEFY_BIT.bitncnd( dta.m_seq -1 );

		if( add_ok ) {
			if( !(dta.m_dsign[0] & 0x01) || i > 0 || (seq ? dta.m_seq == seq : FALSE ) )
			{
				*data = dta;
				rno = dta.m_seq -1;

				return rno;		//OK
			}
		}
	}

	return -1;
}


#endif


//仕訳読み込み（通常）
//	nxt		初回時は key->m_seq で指定されたキーを
//
static int DBdata_read1( int vect, CDBINPDataRec *key, CDBINPDataRec *data, int nl, int seq = 0, int nxt = 0 )
{
CDBINPDataRec dta;
dta.Reset();
int n,vect_n, st;

	vect_n = (vect < 0 ) ? -1 : 1;

	for( n = 0 ; n < nl ; )
	{
		st = _DBdata_read( vect, key, &dta, seq, nxt );

		if( st == -1 )	//over access
		{
			break;
		}
//TRACE( "DBdata_read1 %d\n", st );
		//データセット
		*data = dta;

		data += vect_n;
		n++;
		key->m_seq = (st+1);
		if( n > 0 )	nxt = 1;
	}

	return( n );
}


// 指定 SEQ, 日付が ページ範囲かどうか？
// 返送地は 以下の #define
#define TBLCHK_NONE		0		// 範囲外
#define TBLCHK_START	1		// 開始データ
#define TBLCHK_END		2		// 終了データ
#define TBLCHK_HANI		3		// 範囲内(開始、終了ではない)
#define TBLCHK_ONE		4		// 範囲内に一つのみ

static int datatbl_hcheck( int mode, DB_PGTBL *dbtbl, int seq, int ddate, int ksign )
{
	int ret = TBLCHK_NONE;

	if( mode == 0 ) {	// SEQ 順
		if( seq == dbtbl->s_seq ) {
			if( seq == dbtbl->e_seq )	ret = TBLCHK_ONE;
			else						ret = TBLCHK_START;
		}
		else if( seq == dbtbl->e_seq )	ret = TBLCHK_END;	
		else if( seq > dbtbl->s_seq && seq < dbtbl->e_seq ) {
			ret = TBLCHK_HANI;
		}
	}
	else {			// 日付 順
		int cur_seq, s_seq, e_seq;
		cur_seq = SCMDFY_SEQ.ngetseq( seq-1 );
		s_seq = SCMDFY_SEQ.ngetseq( dbtbl->s_seq-1 );
		e_seq = SCMDFY_SEQ.ngetseq( dbtbl->e_seq-1 );

		if( cur_seq == s_seq ) {
			if( cur_seq == e_seq )	ret = TBLCHK_ONE;
			else					ret = TBLCHK_START;
		}
		else if( cur_seq == e_seq )	ret = TBLCHK_END;	
		else if( cur_seq > s_seq && cur_seq < e_seq ) {
			ret = TBLCHK_HANI;
		}
	}

	return ret;
}


// クリックソート順での 比較
int datarec_sortcmp( int seq, int seq2 )
{
	int order1, order2;
	int ret;
	
	order1 = SCMDFY_SEQ.ngetseq( seq-1 );
	order2 = SCMDFY_SEQ.ngetseq( seq2-1 );

	if( order1 == order2 )		ret = 0;
	else if( order1 < order2 )	ret = -1;
	else						ret = 1;

	return ret;
}

// mode = 0->SEQ順, 1->日付順
//
int datarec_seqcmp( int mode, int seq, int seq2 )
{
	int ret;
//MyTrace( "datarec_seqcmp top mode = %d, seq( %d, %d )\n", mode, seq, seq2 );

	if( SCAN_PGTBL.sort_id != -1 || mode < 0 ) {
		return datarec_sortcmp( seq, seq2 );
	}

	if( mode == 0 ) {
		if( ! pDBzm->insseq_make() ) {
			insseqpos_make( NULL );
		}

		int ins_pos		= INSPOS_SEQ.ngetseq( seq-1 );
		int ins_pos2	= INSPOS_SEQ.ngetseq( seq2-1 );
//MyTrace( "seq = %d( p = %d), seq2 = %d( p = %d)\n", seq, ins_pos, seq2, ins_pos2 );

		if( ins_pos == ins_pos2 )		ret = 0;
		else if( ins_pos < ins_pos2 )	ret = -1;
		else							ret = 1;
	}
	else {
		// スキャンの表示高速化のため、ここで行う
		if( make_scandateseq() ) {
			// 05.13 /08	dbdata のレコード位置が変更されている。
			seq = pDBzm->dbdata->seq;
		}

		int order_seq	= SCMDFY_SEQ.ngetseq( seq-1 );
		int order_seq2	= SCMDFY_SEQ.ngetseq( seq2-1 );

		if( order_seq == order_seq2 )		ret = 0;
		else if( order_seq < order_seq2 )	ret = -1;
		else								ret = 1;
	}
	return ret;

}


//-----------------------------------------------------------
//	レコード位置 の判定
//		返送値	 0:	同じ
//				 1:	最初のチェックレコードのほうが 大きい
//				-1:	最初のチェックレコードのほうが 小さい
//-----------------------------------------------------------
int dbip_data_cmp( int mode, int seq, int ddate, int ksign, int seq2, int ddate2, int ksign2 )
{
	return datarec_seqcmp( mode, seq, seq2 );

#ifdef CLOSE
	int ret;

	if( mode == 0 ) {
		if( seq == seq2 )		ret = 0;
		else if( seq < seq2 )	ret = -1;
		else					ret = 1;
	}
	else {
		// スキャンの表示高速化のため、ここで行う
		if( make_scandateseq() ) {
			// 05.13 /08	dbdata のレコード位置が変更されている。
			seq = pDBzm->dbdata->seq;
		}

		int order_seq	= SCMDFY_SEQ.ngetseq( seq-1 );
		int order_seq2	= SCMDFY_SEQ.ngetseq( seq2-1 );

		if( order_seq == order_seq2 )		ret = 0;
		else if( order_seq < order_seq2 )	ret = -1;
		else								ret = 1;
	}
	return ret;
#endif
}

//----------------------------------------------------------------------------
//	日付順のスキャン SEQテーブル作成
//		return	0: 作成済み
//				1: 今回作成したため、dbdata のレコード位置変更
//----------------------------------------------------------------------------
int make_scandateseq()
{
	int ret = 0;

	if( ! SCAN_PGTBL.mk_datetbl ) {
		CWaitCursor wc;

		int n = 1;
		pDBzm->dbdata->MoveFirst();
		if( pDBzm->dbdata->st != -1 ) {
			do {
				SCMDFY_SEQ.nsetseq( n, pDBzm->dbdata->seq-1);
				n++;
			} while( pDBzm->dbdata->MoveNext() == 0 );

			pDBzm->dbdata->MoveFirst();
		}
		SCAN_PGTBL.mk_datetbl = 1;


		ret = 1;
	}

	return ret;
}

//-----------------------------------------------------------
//	レコード位置 の判定
//		返送値	 0:	同じ
//				 1:	最初のチェックレコードのほうが 大きい
//				-1:	最初のチェックレコードのほうが 小さい
//-----------------------------------------------------------
int dbip_data_cmp( int mode, CDBINPDataRec* rec1, CDBINPDataRec* rec2 )
{
	int ret;

//	if( mode == 0 ) {
//		if( rec1->m_seq == rec2->m_seq )		ret = 0;
//		else if( rec1->m_seq < rec2->m_seq )	ret = -1;
//		else									ret = 1;
//	}
//	else {
		ret = dbip_data_cmp( mode, rec1->m_seq, rec1->m_ddate, rec1->m_ksign, rec2->m_seq, rec2->m_ddate, rec2->m_ksign );
//	}
	return ret;
}



static int sort_read( int vect, CDBINPDataRec *base, CDBINPDataRec *data, int nl, int seq = 0, int nxt = 0 )
{

CDBINPDataRec dta;
dta.Reset();
CString filter, tmp;
int n,vect_n, flg, nxt_sgn, sort_seq;

	vect_n = (vect < 0 ) ? -1 : 1;
	nxt_sgn = nxt;

	// dbdata は、消費税のバッファとして使用しているので、
	// dbdata->seq が、RecordSet 上と 今セットしている SEQ が異なる場合があるので、
	// RecordSet の データを セットする
	pDBzm->dbdata->MoveNext();
	pDBzm->dbdata->MovePrev();

	int pos = SCMDFY_SEQ.ngetseq( base->m_seq - 1 );

	check_scan_insertquery();

	for( n = flg = 0 ; n < nl ; )
	{
		if( nxt ) {
			if( vect > 0 )	pos++;
			else			pos--;
		}

		if( pos > SCAN_PGTBL.sort_cnt || pos < 1 )
			break;

		sort_seq = SEQ_SCMDFY[ pos-1 ];

		int dbpos = MDFYDATA_SEQ.ngetseq( sort_seq-1 );

		pDBzm->dbdata->SetAbsolutePosition( dbpos );

		if( pDBzm->dbdata->st == -1 ) {
			break;
		}

		BYTE bt = pDBzm->dbdata->dsign[0];

		// 本プログラム中に発生した取消もＯＫとする
		if( pDBzm->IsDelScan() || !(pDBzm->dbdata->dsign[0] & 0x01) || MODEFY_BIT.bitncnd( pDBzm->dbdata->seq-1 ) || (seq ? pDBzm->dbdata->seq == seq : FALSE ) ) {
			//データセット
			if( pDBzm->dbdata_Reload() ) {
				cdata_read( pDBzm->dbdata->seq-1, &dta );
			}
			else {
				pDBzm->GetCDBData( &dta );
			}
			*data = dta;
			data += vect_n;
			n++;
		}

		// 次データ検索
		if( ! flg ) {
			flg = 1;
			nxt = 1;
		}
	}

	return n;
}


//仕訳読み込み（スキャン）
// nxt = 1 で キーとなる仕訳を含まないで 次／前(vect) の仕訳 を探す
//
static int scan_read2( int vect, CDBINPDataRec *base, CDBINPDataRec *data, int nl, int seq = 0, int nxt = 0 )
{
CDBINPDataRec dta;
dta.Reset();
CString filter, tmp;
int n,vect_n, i, flg, st, nowpg, cur_page;
int curseq, curdate, curksign, nxt_sgn;

	vect_n = (vect < 0 ) ? -1 : 1;
	curseq = base->m_seq;
	curdate = base->m_ddate;
	curksign = base->m_ksign;
	nxt_sgn = nxt;

	// dbdata は、消費税のバッファとして使用しているので、
	// dbdata->seq が、RecordSet 上と 今セットしている SEQ が異なる場合があるので、
	// RecordSet の データを せっとする
	pDBzm->dbdata->MoveNext();
	pDBzm->dbdata->MovePrev();

	nowpg = cur_page = 0;
	for( n = flg = 0 ; n < nl ; )
	{
		if( flg == 0 ) {

			if( pDBzm->dbdata->st == -1 ) {
				pDBzm->dbdata->MoveNext();
				if( pDBzm->dbdata->st == -1 )
					pDBzm->dbdata->MovePrev();
			}

			// 指定の SEQ 日付まで サーチ
			i = 0;
			st = dbip_data_cmp( SCAN_PGTBL.data_mode, pDBzm->dbdata->seq, pDBzm->dbdata->ddate, pDBzm->dbdata->ksign, curseq, curdate, curksign );

			// CDBDATA のポジションが同じか、指定 より手前
			if( st <= 0 ) {
				do {
					if( i > 0 ) {
						st = dbip_data_cmp( SCAN_PGTBL.data_mode, pDBzm->dbdata->seq, pDBzm->dbdata->ddate, pDBzm->dbdata->ksign, curseq, curdate, curksign );
					}

					if( st == 0 ) {
						break;
					}
					else if( st > 0 ) {
						if( vect_n < 0 ) {
							pDBzm->dbdata->MovePrev();
						}
						break;
					}
					i++;

				} while ( pDBzm->dbdata->MoveNext() == 0 );
			}
			else {
				// CDBDATA のポジションが 指定 より後ろにある
				do {
					if( i > 0 ) {
						st = dbip_data_cmp( SCAN_PGTBL.data_mode, pDBzm->dbdata->seq, pDBzm->dbdata->ddate, pDBzm->dbdata->ksign, curseq, curdate, curksign );
					}

					if( st == 0 ) {
						break;
					}
					if( st < 0 ) {
						if( vect_n > 0 ) {
							pDBzm->dbdata->MoveNext();
						}
						break;
					}
					i++;

				} while ( pDBzm->dbdata->MovePrev() == 0 );
			}
			flg = 1;
		}

		// データ取得
		if( vect_n < 0 ) {
			if( nxt_sgn ) {
				if( pDBzm->dbdata->MovePrev() != 0 ) {
					flg = 0;
					nxt_sgn = 1;
				}
			}
			if( pDBzm->dbdata->st == -1 ) {	// データなし
				break;
			}
		}
		else {
			if( nxt_sgn ) {
				if( pDBzm->dbdata->MoveNext() != 0 ) {
					flg = 0;
					nxt_sgn = 1;
				}
			}		
			if( pDBzm->dbdata->st == -1 ) {	// データなし
				break;
			}
		}

		if( ! flg )	continue;

		// 本プログラム中に発生した取消もＯＫとする
		if( pDBzm->IsDelScan() || !(pDBzm->dbdata->dsign[0] & 0x01) || MODEFY_BIT.bitncnd( pDBzm->dbdata->seq-1 ) || (seq ? pDBzm->dbdata->seq == seq : FALSE ) ) {
			//データセット
			if( pDBzm->dbdata_Reload() ) {
				cdata_read( pDBzm->dbdata->seq-1, &dta );
			}
			else {
				pDBzm->GetCDBData( &dta );
			//	ispread( pDBzm->dbdata->seq-1, &dta );
			}
			*data = dta;
			data += vect_n;
			n++;
		}

		curseq = pDBzm->dbdata->seq;
		curdate = pDBzm->dbdata->ddate;
		curksign = pDBzm->dbdata->ksign;
		nxt_sgn = 1;
	}

	return( n );
}



//仕訳読み込み（スキャン）
// nxt = 1 で キーとなる仕訳を含まないで 次／前(vect) の仕訳 を探す
//
static int DBdata_read2( int vect, CDBINPDataRec *base, CDBINPDataRec *data, int nl, int seq = 0, int nxt = 0 )
{
	int n;

	if( SCAN_PGTBL.sort_id != -1 ) {
		n = sort_read( vect, base, data, nl, seq, nxt );
	}
	else {
		n = scan_read2( vect, base, data, nl, seq, nxt );
	}

	return n;
}


//仕訳読み込み　本体
static int data_read( int vect, CDBINPDataRec *key, CDBINPDataRec *data, int nl, int seq = 0, int nxt = 0 )
{
int n;

	if( INP_mode == _KEYSCAN )
		n = DBdata_read2( vect, key, data, nl, seq, nxt );	//スキャン
	else
		n = DBdata_read1( vect, key, data, nl, seq, nxt );	//通常


	return n;
}


//
//実仕訳読み込み to For
int data_for( CDBINPDataRec *key, CDBINPDataRec *data, int nl )
{
	int vect = 1;

	return data_read( vect, key, data, nl );
}

//
//実仕訳読み込み to Back 検索の基本となるレコードは含めない
int data_nxt_for( CDBINPDataRec *key, CDBINPDataRec *data, int nl )
{
	int vect = 1;

	return data_read( vect, key, data, nl, 0, 1 );
}

//
//実仕訳読み込み to Back
int data_back( CDBINPDataRec *key, CDBINPDataRec *data,int nl, int seq )
{
	int vect = -1;

	return data_read( vect, key, data, nl, seq );
}

//
//実仕訳読み込み to Back 検索の基本となるレコードは含めない
int data_nxt_back( CDBINPDataRec *key, CDBINPDataRec *data,int nl, int seq )
{
	int vect = -1;

	return data_read( vect, key, data, nl, seq, 1 );
}

int data_keyscan_back( CDBINPDataRec *key, CDBINPDataRec *data,int nl, int seq )
{
	int n;
	int vect = -1;

	if( SCAN_PGTBL.sort_id != -1 && SCAN_PGTBL.sort_id != INS_KEYSCAN_SORT) {
		n = sort_read( vect, key, data, nl, seq, 0 );
	}
	else {
		n = scan_read2( vect, key, data, nl, seq, 0 );
	}

	return n;
}


//
// 07.30 /08
//	一仕訳読み込み
int data_search( CDBINPDataRec *key, CDBINPDataRec *data, int nl, int seq/* = 0*/ )
{
//	int vect = 1;
	int vect = -1;	//05.14 /12
	
	return data_read( vect, key, data, nl, seq );
}


//--------------------------------------------------------------------------
//	最終データから取得する
//
//--------------------------------------------------------------------------
int scandat_func( int keySeq, CDBINPDataRec *getrec, int getcnt, int& lastSeq )
{
	int pos, nSeq;
	int linecnt = getcnt;
	int idx, n, i, j, cnt;
	BOOL add_ok;
	CString filter, sTmp;

//	filter.Format( "seq = %d", keySeq );

	lastSeq = -1;

	if( ! pDBzm->insseq_make() ) {
		insseqpos_make( NULL );
	}

	if( keySeq != -1 ) {
		pos = INSPOS_SEQ.ngetseq( keySeq-1 );
	}
	else {
		pos = pDBzm->m_insCnt;
	}

	int reccnt = 0;

	for( n = 0; n < RECMAX; n++ ) {
		if( pos > 0 ) {
			nSeq = SEQ_INSPOS.ngetseq( pos-1 );	//該当absoluteposの SEQ取得

			if( nSeq > 0 ) {
				sTmp.Format( "seq = %d", nSeq );

				reccnt++;

				if( n > 0 ) {
					filter += " or ";
				}
				filter += sTmp;

				lastSeq = nSeq;
			}
			else {
				lastSeq = -1;
				break;
			}
			pos--;
		}
		else {
			lastSeq = -1;
			break;
		}
	}

	if( filter.IsEmpty() ) {
		if( lastSeq == -1 ) {
			filter = "seq = -1"; //仕訳なし
		}
	}

	pDBzm->dbdata->Requery( -2, 0, filter, 0, 3 );
	pDBzm->dbdata->MoveLast();
	pDBzm->dbdata_Speed()	= TRUE;
	pDBzm->dbdata_Requery() = TRUE;
	pDBzm->dbdata_Job()		= DBDATA_JOB_NORMAL;

	idx = linecnt-1;

	j = 0;
	cnt = 0;

	if( pDBzm->dbdata->st != -1 ) {
		do {
			// 今回の仕訳追加分かどうかを SEQ ﾃｰﾌﾞﾙよりチェック
			add_ok = TRUE;

			if( pDBzm->dbdata->seq > START_SEQ ) {
				n = pDBzm->dbdata->seq - 1;

				i = SEQ_APPEND.ngetseq( n );
				if( i < 0 )		add_ok = FALSE;
				else if( !i )	add_ok = FALSE;
				else {
					// 追加 SEQ のチェック
					n = i - 1;
					j = APPEND_SEQ.ngetseq( n );
					ASSERT( j == pDBzm->dbdata->seq );
				}
			}

			// 本プログラム中に発生した取消もＯＫとする
			i = MODEFY_BIT.bitncnd( pDBzm->dbdata->seq -1 );

			if( add_ok ) {
				if( !(pDBzm->dbdata->dsign[0] & 0x01) || i > 0 )
				{
					cnt++;
					pDBzm->GetCDBData( &ZREC[idx] );
					idx--;
					if( idx < 0 )
						break;
				}
			}

		} while( pDBzm->dbdata->MovePrev() == 0 );
	}

	return cnt;

}


int scandat_getn( CDBINPDataRec *getrec, CDBINPDataRec *keyrec, int getcnt, int seq )
{
//	return DBdata_read1( -1, keyrec, getrec + getcnt - 1, getcnt, seq, 0 );

	int lastSeq, cnt, n, nSeq, pos, j;

	nSeq = keyrec->m_seq;

	j = 0;

	for( n = 0; ;n++ ) {
		cnt = scandat_func( nSeq, getrec, getcnt- j, lastSeq );
		j += cnt;

		if( lastSeq != -1 ) {
			if( cnt < getcnt ) {
				pos = INSPOS_SEQ.ngetseq( lastSeq-1 );

				if( pos > 0 ) {
					pos--;
					nSeq = SEQ_INSPOS.ngetseq( pos-1 );	//ひとつ前のSEQ取得
					if( nSeq < 0 )
						break;
				}
			}
			else {
				break;
			}
		}
		else {
			break;
		}
	}

	return j;
}


// 03.01 /12
int endkeyscan_getn( CDBINPDataRec *getrec, CDBINPDataRec *keyrec, int getcnt, int line, int seq/*=0*/ )
{

	// カレント行 から 上部分
	int sv_seq = keyrec->m_seq;
	int n = DBdata_read1( -1, keyrec, getrec + line - 1, line, seq, 0 );

	// カレント行 から 下部分
	int down_cnt = (getcnt-line)+1;
	if( down_cnt > 0 ) {
		keyrec->m_seq = sv_seq;
		n += DBdata_read1( 1, keyrec, getrec + line - 1, down_cnt, seq, 0 );
	}
	return n;
}


void scandat_dspn( CDBINPDataRec *dsprec, struct _DATA_LINE *ddata, int dspcnt )
{
	int		i;
	int		lin;

	lin = 0;
	for( i=0; i < dspcnt; i++, lin++ ) {
		// データ初期化
		nline_del( ddata, lin );

		if( (dsprec+i)->m_seq != -1 )
			recdsp( dsprec, i, ddata, lin );	// 表示

	}
}


//
// データテーブル再表示 ... Fork END
//
void scandat_rdsp( CDBINPDataRec *dsprec, struct _DATA_LINE *ddata, int max )		
{
	int recno;
	int n;

	for( n = 0 ; n < max ; ++n )
	{
		recno = (int)(ddata+n)->ZREC_NO;
		nline_del( ddata, n );
		if( recno )
			recdsp( dsprec, recno - 1, ddata, n );
	}

}




int scaninsdat_getn( CDBINPDataRec *getrec, int keyseq, int getcnt, int seq )
{
	int lastSeq, cnt, n, nSeq, pos, j;

	nSeq = keyseq;

	j = 0;

	for( n = 0; ;n++ ) {
		cnt = scandat_func( nSeq, getrec, getcnt- j, lastSeq );
		j += cnt;

		if( lastSeq != -1 ) {
			if( cnt < getcnt ) {
				pos = INSPOS_SEQ.ngetseq( lastSeq-1 );

				if( pos > 0 ) {
					pos--;
					nSeq = SEQ_INSPOS.ngetseq( pos-1 );	//ひとつ前のSEQ取得
					if( nSeq < 0 )
						break;
				}
			}
			else {
				break;
			}
		}
		else {
			break;
		}

		if( getcnt- j <= 0 ) {
			break;
		}
	}

	return j;
}




//----------------------------------------------------------------------------
//	カラムクリックによる読取順の変更 [07.23 /13]
//		return	0: 作成済み
//				1: 今回作成したため、dbdata のレコード位置変更
//----------------------------------------------------------------------------
int make_scanorder( int order_id, int ad_typ )
{
	CStRecordSet	seqpos;
	CString emsg;
	CString sOrder;
	int ret = 0;

	SCMDFY_SEQ.fill();
	SEQ_SCMDFY.clear();

	if( ! SCAN_PGTBL.mk_dbdata ) {
		MDFYDATA_SEQ.fill();
	}

	seqpos.m_pDatabase = pDBzm->m_database;

	const char* syz1   = "CASE (cast(substring(dsign, 1, 1) as int))&0x40";
	const char* syz2_a = " WHEN 0x40 THEN 1 ELSE 2 END,";
	const char* syz2_d = " WHEN 0x40 THEN 2 ELSE 1 END,";
	const char* syz3_a = "(cast(substring( dsign, 3, 1 ) as int)&0x0f)"; //課税区分(固定資産のサインは無視)
	const char* syz3_b = "(cast(substring( dsign, 2, 1 ) as int))";	//税率・税区分
	const char* syz3_c = "(cast(substring( dsign, 4, 3 ) as int))";	//売上仕入区分・消費税仕訳区分・特定収入

	CString syzorder;

	if (ad_typ == DCI_SORTASCENDING) {	//昇順
		syzorder.Format("%s%s %s,%s,%s", syz1, syz2_a, syz3_a, syz3_b, syz3_c);
	}
	else {
		syzorder.Format("%s%s %s DESC,%s DESC,%s DESC", syz1, syz2_a, syz3_a, syz3_b, syz3_c);
	}

	switch( order_id ) {
	case DATA_SEQ:
		if( IsMasterType(MST_INSERT) ) { 
			if( ad_typ == DCI_SORTASCENDING ) {	//昇順
				sOrder = "dbo.seqret( seq, stseq ), dbo.subseqret( sbseq )";
			}
			else {	//降順
				sOrder = "dbo.seqret( seq, stseq ) DESC, dbo.subseqret( sbseq ) DESC";
			}
		}
		else {
			if( ad_typ == DCI_SORTASCENDING ) {	//昇順
				sOrder = "seq";
			}
			else {	//降順
				sOrder = "seq DESC";
			}
		}
		break;
	case DATA_DATE:
		if( ad_typ == DCI_SORTASCENDING ) {
			sOrder = "ddate,mofs,ksign";
		}
		else {
			sOrder = "ddate DESC,mofs DESC,ksign DESC";
		}
		break;
	case DATA_DENP:
		if( ad_typ == DCI_SORTASCENDING ) {
			sOrder = "cno";
		}
		else {
			sOrder = "cno DESC";
		}
		break;
	case DATA_DEB_BMON:
	case DATA_CRE_BMON:
		if( ad_typ == DCI_SORTASCENDING ) {
			sOrder = "dbmn,cbmn";
		}
		else {
			sOrder = "dbmn DESC, cbmn DESC";
		}
		break;
	case DATA_DEB_KNO:
	case DATA_CRE_KNO:
		if( ad_typ == DCI_SORTASCENDING ) {
			sOrder = "(space( 12-len(dkno)) + dkno),(space(12-len(ckno)) + ckno)";
		}
		else {
			sOrder = "(space( 12-len(dkno)) + dkno) DESC, (space(12-len(ckno)) + ckno) DESC";
		}
		break;
	case DATA_DEBT:
		if( ad_typ == DCI_SORTASCENDING ) {
			sOrder = "dbt,dbr";
		}
		else {
			sOrder = "dbt DESC, dbr DESC";
		}
		break;
	case DATA_CRED:
		if( ad_typ == DCI_SORTASCENDING ) {
			sOrder = "cre,cbr";
		}
		else {
			sOrder = "cre DESC, cbr DESC";
		}
		break;
	case DATA_VAL:
		if( ad_typ == DCI_SORTASCENDING ) {
			sOrder = "val";
		}
		else {
			sOrder = "val DESC";
		}
		break;
	case DATA_ZEI:
		if( ad_typ == DCI_SORTASCENDING ) {
			sOrder.Format( "zei, %s", syzorder );
		}
		else {
			sOrder.Format( "zei DESC, %s", syzorder );
		}
		break;
	case DATA_SYZ:
		sOrder = syzorder;
		break;
	case DATA_TEKIYO:
		if( ad_typ == DCI_SORTASCENDING ) {
			sOrder = "tekiyo";
		}
		else {
			sOrder = "tekiyo DESC";
		}
		break;
	case DATA_SYONO:
		if( ad_typ == DCI_SORTASCENDING ) {
			sOrder = "rnumber";
		}
		else {
			sOrder = "rnumber DESC";
		}
		break;
	case DATA_TGDATE:
		if( ad_typ == DCI_SORTASCENDING ) {
			sOrder = "tdate";
		}
		else {
			sOrder = "tdate DESC";
		}
		break;
	case DATA_SKIN:
	case DATA_HUSEN:
	case DATA_HJIYU:
		if( ad_typ == DCI_SORTASCENDING ) {
			sOrder = "hjiyu";
		}
		else {
			sOrder = "hjiyu DESC";
		}
		break;
	case DATA_DOCEVI:
		if( ad_typ == DCI_SORTASCENDING ) {
			sOrder = "(space( 8-len(skn)) + skn)";
		}
		else {
			sOrder = "(space( 8-len(skn)) + skn) DESC";
		}
		break;

	case DATA_USER1:
		//出納帳形式も前に表示する(伝票種別があって、内部伝票番号がない不正なデータは先頭へ）
		if( ad_typ == DCI_SORTASCENDING ) {
			sOrder = "CASE dtype WHEN  0  THEN 0\
 ELSE case when icno = 0 THEN 0\
 WHEN icno is NULL THEN 0\
 ELSE 1 END\
 END,\
 CASE dtype WHEN  0x40 THEN 0 ELSE 1 END, dtype, icno";
		}
		else {
			sOrder = "CASE dtype WHEN 0 THEN 0x41\
 ELSE case when icno = 0 THEN 0x41\
 WHEN icno is NULL THEN 0x41\
 ELSE 0 END\
 END,\
 CASE dtype WHEN  0x40 THEN 0x42 ELSE 0 END, dtype DESC, icno DESC";
		}
		break;
	}

	if( sOrder.IsEmpty() ) {
		SCAN_PGTBL.sort_id		= -1;
		SCAN_PGTBL.sort_type	= 0;

		return 0;
	}

	SCAN_PGTBL.sort_id		= order_id;
	SCAN_PGTBL.sort_type	= ad_typ;

	CString sTmpOrder, scanFilter;

	if( SCAN_PGTBL.data_mode == 0 ) {
		if( order_id != DATA_SEQ ) {
			if( IsMasterType(MST_INSERT) ) {
				if( ad_typ == DCI_SORTASCENDING ) {	//昇順
					sTmpOrder = ", dbo.seqret( seq, stseq ), dbo.subseqret( sbseq )";
				}
				else {
					sTmpOrder = ", dbo.seqret( seq, stseq ) DESC, dbo.subseqret( sbseq )  DESC";
				}
			}
			else {
				if( ad_typ == DCI_SORTASCENDING ) {	//昇順
					sTmpOrder = ", seq";
				}
				else {
					sTmpOrder = ", seq DESC";
				}
			}
		}
		sOrder += sTmpOrder;
	}
	else {
		CString tmpSeq;
		//日付順
		if( order_id != DATA_DATE ) {
			if( ad_typ == DCI_SORTASCENDING ) {	//昇順
				sTmpOrder = ",ddate,mofs,ksign";
			}
			else {
				sTmpOrder = ",ddate DESC, mofs DESC, ksign DESC";
			}
		}

		if( order_id != DATA_SEQ ) {
			if( IsMasterType(MST_INSERT) ) {
				if( ad_typ == DCI_SORTASCENDING ) {	//昇順
					tmpSeq = ", dbo.seqret( seq, stseq ), dbo.subseqret( sbseq )";
				}
				else {
					tmpSeq = ", dbo.seqret( seq, stseq ) DESC, dbo.subseqret( sbseq )  DESC";
				}
			}
			else {
				if( ad_typ == DCI_SORTASCENDING ) {	//昇順
					tmpSeq = ", seq";
				}
				else {
					tmpSeq = ", seq DESC";
				}
			}
		}

		sTmpOrder += tmpSeq;
		sOrder += sTmpOrder;
	}
	SCAN_PGTBL.sortOrder = sOrder;
	/* 一般ユーザでの絞込み */
	if( SCAN_PGTBL.data_owner == 0 ) {
		if( SCAN_PGTBL.filter.IsEmpty() ) {
			seqpos.m_sql.Format( "select seq as st from datarec where owner = %d order by %s", pDBzm->DBUserNo(), sOrder );
		}
		else {
			get_scanfilter( scanFilter );
			seqpos.m_sql.Format( "select seq as st from datarec where owner = %d and %s order by %s", pDBzm->DBUserNo(), scanFilter, sOrder );
		}
	}
	else {
		if( SCAN_PGTBL.filter.IsEmpty() ) {
			seqpos.m_sql.Format( "select seq as st from datarec order by %s", sOrder );
		}
		else {
			get_scanfilter( scanFilter );
			seqpos.m_sql.Format( "select seq as st from datarec where %s order by %s", scanFilter, sOrder );
		}
	}

//MyTrace("make_scanorder (1)\n");

	TRY {
		if( ! seqpos.Open() ) {
			emsg = _T( "SEQ順を調べられません！" );
			ICSMessageBox( emsg );
			return -99;
		}
		int abspos = 0;

		if( ! seqpos.IsBOF() ) {
			int idx;
			abspos = 1;

			while( !seqpos.IsEOF() ) {
//TRACE( "CLMSORT--> seqpos.m_st(%d) = %d\n", abspos, seqpos.m_st );

				idx = seqpos.m_st - 1;
				SCMDFY_SEQ.nsetseq( abspos, idx );
				SEQ_SCMDFY.push_back( seqpos.m_st );

				seqpos.MoveNext();
				abspos++;
			}
			// seqpos.IsEOF()の判定前のカウントマイナス
			abspos--;

			SCAN_PGTBL.sort_cnt = abspos;
		}

		seqpos.Close();

	}
	CATCH( CMemoryException, me ) {
		me->GetErrorMessage( emsg.GetBuffer(_MAX_PATH), _MAX_PATH );
		emsg.ReleaseBuffer();
		ICSMessageBox( emsg );
		return -99;	
	}
	CATCH( CDBException, de ) {
		emsg = de->m_strError;
		ICSMessageBox( emsg );
		return -99;
	}
	END_CATCH

//MyTrace("make_scanorder end\n");

	// SEQより dbdataの	AbsolutePositionをGETするためのテーブル作成
	if( ! SCAN_PGTBL.mk_dbdata ) {
		CWaitCursor wc;

		int n = 1;
		pDBzm->dbdata->MoveFirst();
		if( pDBzm->dbdata->st != -1 ) {
			do {
				MDFYDATA_SEQ.nsetseq( n, pDBzm->dbdata->seq-1 );
				n++;
			} while( pDBzm->dbdata->MoveNext() == 0 );

			pDBzm->dbdata->MoveFirst();
		}
		SCAN_PGTBL.mk_dbdata = 1;

		ret = 1;
	}

	return 0;
}

// 
int make_acbksortseq()
{
	int ret = 0;

	CWaitCursor wc;

	int n = 1;
	pDBzm->data->MoveFirst();
	if( pDBzm->data->st != -1 ) {
		do {
			SCMDFY_SEQ.nsetseq( n, pDBzm->data->seq-1);
			n++;
		} while( pDBzm->data->MoveNext() == 0 );

		pDBzm->data->MoveFirst();
	}

	return ret;
}
// 検索中に挿入した時、データの並びに挿入分を追加する。
int insert_keyscanseq( int ins_seq, int bseq )
{
	int ret = 0;

	//データの並びを作成する。
	if( SCAN_PGTBL.sort_id == -1 ) {

		SCAN_PGTBL.sort_id = INS_KEYSCAN_SORT;

		SCMDFY_SEQ.fill();
		SEQ_SCMDFY.clear();

		CWaitCursor wc;

		int n = 1;
		pDBzm->dbdata->MoveFirst();
		if( pDBzm->dbdata->st != -1 ) {

			do {
				SCMDFY_SEQ.nsetseq( n, pDBzm->dbdata->seq-1 );
				SEQ_SCMDFY.push_back( pDBzm->dbdata->seq );
				n++;
			} while( pDBzm->dbdata->MoveNext() == 0 );

			SCAN_PGTBL.sort_cnt = n-1;

			pDBzm->dbdata->MoveFirst();
		}
	}

	SCAN_PGTBL.sort_cnt++;	//挿入分
	SCAN_PGTBL.mk_dbdata = 0;

	int sort_pos = SCMDFY_SEQ.ngetseq( bseq-1 );

	vector<DWORD>::iterator	it;
	vector<DWORD>::iterator	it2;

	it = SEQ_SCMDFY.begin();
	it += (sort_pos-1);

	it2 = SEQ_SCMDFY.insert( it, ins_seq );
	SCMDFY_SEQ.nsetseq( sort_pos, *it2 - 1 );
//TRACE( "*it2 = %d\n", *it2 );

	it2++;
	//挿入以後のポジションをずらす。
	for( ; it2 != SEQ_SCMDFY.end(); it2++ ) {
		sort_pos = SCMDFY_SEQ.ngetseq( *it2 - 1 );
		sort_pos++;
		SCMDFY_SEQ.nsetseq( sort_pos, *it2 - 1 );
	}

	return ret;
}

// スキャン中に仕訳を挿入したら、dbdataをクエリーして、ソート順を保存する。
void check_scan_insertquery()
{
	if( SCAN_PGTBL.ins_query ) {
		CString		filter;

		get_scanfilter( filter );

		pDBzm->dbdata->Requery(SCAN_PGTBL.data_owner, 0, filter, 0, 0);
		pDBzm->dbdata->MoveFirst();

		if( pDBzm->dbdata->st == -1 ) {
			ICSExit( 0, "仕訳レコードの取得に失敗しました！" );
			return;
		}

		MDFYDATA_SEQ.fill();
		int abspos = 1;

		do {
			MDFYDATA_SEQ.nsetseq( abspos, pDBzm->dbdata->seq-1 );
			abspos++;
		} while( pDBzm->dbdata->MoveNext() == 0 );

		SCAN_PGTBL.ins_query = 0;
	}
}


void get_scanfilter( CString& filter )
{
	CString stmp;
	filter = SCAN_PGTBL.filter;

	if( filter.IsEmpty() )	return;

	for( int n = 0; n < SCAN_PGTBL.scanInsSeq.GetCount(); n++ ) {
		stmp.Format( " or seq = %d", SCAN_PGTBL.scanInsSeq[n] );
		filter += stmp;
	}
}



// 伝票形式からの修正の終わり。
void query_denpend( int icno, int seq )
{
	CString	filter, stmp;

	filter = SCAN_PGTBL.filter;

	SCAN_ITEMEX *key = &SC_Item;

	// 表示順
	int sort_sign;
	switch( (key->sc_type & 0x0f) ) {
	case SCT_ORDERSEQ:	sort_sign = 3;	break;
	default:			sort_sign = 1;	break;
	}

	if( filter.IsEmpty() )	{
		//全データを検索
	}
	else {
		//検索条件に 選択ＳＥＱを追加
		stmp.Format( " or seq = %d", seq );
		filter += stmp;
	}

	pDBzm->dbdata->Requery(SCAN_PGTBL.data_owner, 0, filter, 0, sort_sign);
	pDBzm->dbdata->MoveFirst();

	if( pDBzm->dbdata->st == -1 ) {
		ICSExit( 0, "仕訳レコードの取得に失敗しました！" );
		return;
	}

	MDFYDATA_SEQ.fill();
	int abspos = 1;

	do {
		MDFYDATA_SEQ.nsetseq( abspos, pDBzm->dbdata->seq-1 );
		abspos++;
	} while( pDBzm->dbdata->MoveNext() == 0 );
}
