
#include "stdafx.h"

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

#include "MainFrm.h"

// CDBDinpView
#include "DBDinp.h"
#include "DBDinpDoc.h"
#include "DBDinpView.h"
extern class CDBDinpView *p_DBDinpView;
extern int	BOTOM_SEQ;

// 貸借合計
#include "TaiTtl.h"
extern CTaiTtl m_TaiTtl;


// 入力選択項目Get/Set
//#include "DBInpSub/SETUPTBL.h"
extern CDinpINI DinpSW;

// 入力設定項目選択
#include "AutoInp.h"
extern struct _AUTOSEL *pAUTOSEL;

#include "BmnChk.h"
extern CBmnHaniCheck	BmChk;
extern CKojiDataCheck	KjChk;

// プログラムＯＫ サイン
extern int PROGRAM_OK;

//func
#include "mstrw.h"
#include "scan.h"
#include "Dinpsub.h"

#include "SubFnc.h"
#include "BmnNoInp.h"


//消費税モジュール
//extern CDBSyohi* pDBsy;

static int km_dsp( /*int**,*/ int *, int);

extern struct _DataTable SelFix8[2*4];
extern struct _DataTable SelVal32[8*4];

// data display
extern struct _DATA_LINE DDATA_LINE[];
//extern int SCROLL_NL;

//
// 仕訳状態ビットテーブル
#include "bit.h"
extern CBit	MODEFY_BIT;		//修正仕訳ビットテーブル
extern CBit	SYZMDFY_BIT;	//消費税サイン修正ビットテーブル

#include "SEQ.h"
extern CSEQ	APPEND_SEQ;		//追加仕訳ＳＥＱ登録テーブル
extern CSEQ	SEQ_APPEND;		//追加仕訳ＳＥＱ登録テーブル管理テーブル
extern CSEQ	SCMDFY_SEQ;		//日付順修正前データＳＥＱ管理テーブル
extern CSEQ	SEQ_SCMDFY;		//日付順修正前データＳＥＱ管理テーブル

static BOOL syogtai( CDBINPDataRec *dta, int add_sub );
static BOOL genktai( CDBINPDataRec *dta, int add_sub );

extern CSEQ	INSPOS_SEQ;		//挿入仕訳対応ＳＥＱ ポジション格納テーブル


// MstRw.cpp
//
//	仕訳書き込み時の部門チェック
//	return	0x1001	借方 部門修正
//			0x1002	貸方 部門修正
//
int bmchk_sub( CDBINPDataRec* prec, int bmchk, CWnd* pwnd )
{
	DWORD dwCode;
	int dc_sw = 0;
	int ret = 0;

	if( ((CMainFrame*)GetDBDinpMainFrame())->IsTkyOnly() )
		return 0;

	if( ! BmChk.BmnHaniCheck( prec->m_dbmn, prec->m_dbt ) ) {
		dwCode = _atoh( (LPSTR)(LPCTSTR)prec->m_dbt );

		dc_sw |= 0x01;

		if( (Voln1->bm_isw & 0x10)) {
			if( dwCode <= SONEKICODE ) {
				prec->m_dbmn = -1;
				dc_sw &= ~0x01;
			}
		}
	}
	if( ! BmChk.BmnHaniCheck( prec->m_cbmn, prec->m_cre ) ) {
		dwCode = _atoh( (LPSTR)(LPCTSTR)prec->m_cre );
		dc_sw |= 0x02;

		if( (Voln1->bm_isw & 0x10)) {
			if( dwCode <= SONEKICODE ) {
				prec->m_cbmn = -1;
				dc_sw &= ~0x02;
			}
		}
	}

	if( dc_sw ) {
		DBKNREC* pKn;
		CString str, msg;
		char buf[32];
		int st;

		if( bmchk ) {
			if( dc_sw & 0x01 ) {
				if( ( pKn = pDBzm->DB_PjisToKnrec( prec->m_dbt ) ) == NULL )
					return -1;

				pDBzm->BumonCodeToStr( buf, sizeof buf, prec->m_dbmn );
				str.Format( "\r\n借方科目「%.14s」に、部門番号『%s』は設定できません。", pKn->knnam, buf );
				msg += str;
			}

			if( dc_sw & 0x02 ) {
				if( ( pKn = pDBzm->DB_PjisToKnrec( prec->m_cre ) ) == NULL )
					return -1;

				pDBzm->BumonCodeToStr( buf, sizeof buf, prec->m_cbmn );
				str.Format( "\r\n貸方科目「%.14s」に、部門番号『%s』は設定できません。", pKn->knnam, buf );
				msg += str;
			}

			msg += "\r\n\r\n仕訳を修正しますか？\r\nいいえの場合、部門を取消して仕訳を書き込みます。";

			st = myICSMessageBox( (LPCSTR)msg, MB_YESNO,0,0,pwnd );
		}
		else {
			st = IDNO;
		}

		if( st == IDYES ) {
			ret = dc_sw;
			ret |= 0x1000;
		}
		else {
			if( dc_sw & 0x01 ) {
				prec->m_dbmn = -1;
			}
			if( dc_sw & 0x02 ) {
				prec->m_cbmn = -1;
			}
			ret = 0;
		}
	}

	return ret;
}


//
//	仕訳書き込み時の工事番号チェック
//	return	0x1001	借方 工事番号
//			0x1002	貸方 工事番号
//
int kjchk_sub( CDBINPDataRec* prec, int kjchk, CWnd* pwnd )
{
	DWORD dwCode;
	int dc_sw = 0;
	int ret = 0;

	if( ((CMainFrame*)GetDBDinpMainFrame())->IsTkyOnly() )
		return 0;

	if( pAUTOSEL->KJIKMK_OPT != OPT_KJIKMK)
		return 0;

	if( ! prec->m_dkno.IsEmpty() ) {
		if( ! KjChk.KojiKmkCheck( prec->m_dbt ) ) {
			dwCode = _atoh( (LPSTR)(LPCTSTR)prec->m_dbt );
			dc_sw |= 0x01;
		}
	}

	if( ! prec->m_ckno.IsEmpty() ) {
		if( ! KjChk.KojiKmkCheck( prec->m_cre ) ) {
			dwCode = _atoh( (LPSTR)(LPCTSTR)prec->m_cre );
			dc_sw |= 0x02;
		}
	}

	if( dc_sw ) {
		DBKNREC* pKn;
		CString str, msg;
		char buf[32];
		int st;

		if( kjchk ) {
			if( dc_sw & 0x01 ) {
				if( ( pKn = pDBzm->DB_PjisToKnrec( prec->m_dbt ) ) == NULL )
					return -1;

				pDBzm->KojiCodeToStr( buf, sizeof buf, prec->m_dkno );
				str.Format( "\r\n借方科目「%.14s」に、工事番号『%s』は設定できません。", pKn->knnam, buf );
				msg += str;
			}

			if( dc_sw & 0x02 ) {
				if( ( pKn = pDBzm->DB_PjisToKnrec( prec->m_cre ) ) == NULL )
					return -1;

				pDBzm->KojiCodeToStr( buf, sizeof buf, prec->m_ckno );
				str.Format( "\r\n貸方科目「%.14s」に、工事番号『%s』は設定できません。", pKn->knnam, buf );
				msg += str;
			}

			msg += "\r\n\r\n仕訳を修正しますか？\r\nいいえの場合、工事番号を取消して仕訳を書き込みます。";

		//	st = myICSMessageBox( (LPCSTR)msg, MB_YESNO,0,0,pwnd );
			st = IDNO;	// メッセージ表示しない。
		}
		else {
			st = IDNO;
		}

		if( st == IDYES ) {
			ret = dc_sw;
			ret |= 0x2000;
		}
		else {
			if( dc_sw & 0x01 ) {
				prec->m_dkno.Empty();
			}
			if( dc_sw & 0x02 ) {
				prec->m_ckno.Empty();
			}
			ret = 0;
		}
	}

	return ret;
}



//
//	仕訳書き込み時の枝番未入力チェック
//	return	0x0001	借方 枝番
//			0x0002	貸方 工事番号
//
int brninp_chksub( CDBINPDataRec* prec, int brnchk, CWnd* pwnd )
{
	int dc_sw = 0;
	int ret = 0;

	if( ((CMainFrame*)GetDBDinpMainFrame())->IsTkyOnly() )
		return 0;

	if( pAUTOSEL->BRNKAK_OPT != OPT_BRNKAK)
		return 0;

	if( prec->m_dbr == -1 ) {
		// 枝番残高があるか？
		if( pDBzm->DB_EdabanZanSearch( prec->m_dbt, -1 ) == 0 ) {
			dc_sw |= 0x01;
		}
	}

	if( prec->m_cbr == -1 ) {
		// 枝番残高があるか？
		if( pDBzm->DB_EdabanZanSearch( prec->m_cre, -1 ) == 0 ) {
			dc_sw |= 0x02;
		}
	}

	if( dc_sw ) {
		DBKNREC* pKn;
		CString str, msg;
		char buf[32];
		int st;

		if( brnchk ) {
			if( dc_sw & 0x01 ) {
				if( ( pKn = pDBzm->DB_PjisToKnrec( prec->m_dbt ) ) == NULL )
					return -1;

				pDBzm->KojiCodeToStr( buf, sizeof buf, prec->m_dkno );
				str.Format( "\r\n借方科目「%.14s」に、枝番が入力されていません！", pKn->knnam, buf );
				msg += str;
			}

			if( dc_sw & 0x02 ) {
				if( ( pKn = pDBzm->DB_PjisToKnrec( prec->m_cre ) ) == NULL )
					return -1;

				pDBzm->KojiCodeToStr( buf, sizeof buf, prec->m_ckno );
				str.Format( "\r\n貸方科目「%.14s」に、枝番が入力されていません！", pKn->knnam, buf );
				msg += str;
			}

			msg += "\r\n\r\n仕訳を修正しますか？";

			((CMainFrame*)GetDBDinpMainFrame())->MsgBoxFlag() = TRUE;

			st = myICSMessageBox( (LPCSTR)msg, MB_YESNO,0,0,pwnd );
		//	st = IDNO;	// メッセージ表示しない。
		}
		else {
			st = IDNO;
		}

		if( st == IDYES ) {
			ret = dc_sw;
			ret |= 0x4000;
		}
		else {
			if( dc_sw & 0x01 ) {
				prec->m_dbr = -1;
			}
			if( dc_sw & 0x02 ) {
				prec->m_cbr = -1;
			}
			ret = 0;
		}
	}

	return ret;
}




#include <afxmt.h>
extern CCriticalSection thread_data_scan_criticalSection;


/*--------------------------------
	data update append / correct
---------------------------------*/
int dataupd( CDBINPDataRec *dta, CDBINPDataRec *SCAN_rec, int mode, int bmchk, CWnd* pwnd )
{
	int ret = 0;

	if( mode == _MODEFY || mode == _MDFEND || mode == _KEYSCAN )
	{

		BOOL bOKchg = FALSE;
		if( PROGRAM_OK ) {	
			PROGRAM_OK = FALSE;
			bOKchg = TRUE;
		}

		// 部門範囲チェック
		if( (ret = bmchk_sub( dta, bmchk, pwnd )) != 0 ) {
			goto FNC_END;
		}
		// 工事番号チェック
		if( (ret = kjchk_sub( dta, bmchk, pwnd )) != 0 ) {
			goto FNC_END;
		}
		// 枝番チェック
		if( (ret = brninp_chksub( dta, bmchk, pwnd )) != 0 ) {
			goto FNC_END;
		}

		if( bOKchg ) {
			PROGRAM_OK = TRUE;
		}

		pDBzm->KamokuCodeCheck( dta->m_dbt );
		pDBzm->KamokuCodeCheck( dta->m_cre );
		//インボイス登録番号対象か？
		pDBzm->is_invnosiwake(dta);

		//修正ビットサインＯＮ
		if( *dta != *SCAN_rec )
		{
#ifdef CLOSE	// 12.10 /01
			//スキャン
			if( mode == _KEYSCAN )
				SCANMDFY_BIT.onbitn( dta->m_seq - 1);
#endif

//MyTrace( "■■■ DB_DataCorrect dta seq = %d, cno = %d\n", dta->m_seq, dta->m_cno );
//BYTE* p1 = &dta->m_dsign[0];
//MyTrace( "■■■ DB_DataCorrect dta  seq = %d, dsign %02x%02x%02x%02x%02x%02x%02x%02x\n", dta->m_seq, *p1, *(p1+1),*(p1+2),*(p1+3),*(p1+4),*(p1+5),*(p1+6), *(p1+7) );
//p1 = &SCAN_rec->m_dsign[0];
//MyTrace( "■■■ DB_DataCorrect moto seq = %d, dsign %02x%02x%02x%02x%02x%02x%02x%02x\n", SCAN_rec->m_seq, *p1, *(p1+1),*(p1+2),*(p1+3),*(p1+4),*(p1+5),*(p1+6), *(p1+7) );

			int c_ret;
			// データリライト
			c_ret = pDBzm->DB_DataCorrect( dta );

			if( c_ret >= 0 ) {
				MODEFY_BIT.onbitn( dta->m_seq - 1);

				if( isSyzdiff_data( dta, SCAN_rec ) ) {
					SYZMDFY_BIT.onbitn( dta->m_seq - 1);
				}
			}

			if( c_ret == ERR )
			{
				ermset(-1, (char*)(LPCTSTR)pDBzm->GetZmErrMsg() );
				ret = -1;
			}
			else if( c_ret == -2 ) {
				pDBzm->GetCDBData( dta );
				if( INP_mode == _KEYSCAN ) {
					m_TaiTtl.sub_taidata( SCAN_rec );	// 貸借データから減算
					m_TaiTtl.add_taidata( dta );		// 貸借データへ加算
				}
				ret = -2;

			}
			// スキャン貸借合計
			else if( INP_mode == _KEYSCAN )
			{
				m_TaiTtl.sub_taidata( SCAN_rec );	// 貸借データから減算
				m_TaiTtl.add_taidata( dta );		// 貸借データへ加算
			}
		}
		else {
			p_DBDinpView->IsChangeLinkDocSeq();
		}
	}
	else if( mode == _APPEND )
	{
		int bseq = p_DBDinpView->get_insseq();

		//インボイス登録番号対象か？
		pDBzm->is_invnosiwake(dta);

		if( pDBzm->DB_DataAppend( dta, bseq ) == ERR )
	//	if( pDBzm->DataAppend(  dta  ) == ERR )
		{
			ermset(-1, (char*)(LPCTSTR)pDBzm->GetZmErrMsg() );
			ret = -1;
		}

		if( bseq != -1 ) {
			p_DBDinpView->set_insrec( dta, bseq );
		}
		// 追加仕訳管理テーブル編集
		SEQ_APPEND.nsetseq( SEQ_NO, dta->m_seq - 1 );
		APPEND_SEQ.nsetseq( dta->m_seq, SEQ_NO - 1 );
	}

FNC_END:
//FILETrace( "dataupd END = %d\n", ret );

	return ret;
}



//***********************************************************
//	出納帳などで追加入力した場合の 通常形式の変数を更新
//
//
//***********************************************************
void DBDinpAppendParamUpdate( CDBINPDataRec* prec, int bseq )
{
	BOTOM_SEQ = ++SEQ_NO;

	int lastseq;
	lastseq = prec->m_seq;

	::SREC = *prec;
	::SREC.m_dtype = 0;
	// サンプルレコードの更新
	if( bseq == -1 ) {	//挿入仕訳の時は更新せず
	//	p_DBDinpView->SaveDinpDataSREC( _APPEND );
		p_DBDinpView->SaveDinpParaSREC( _APPEND );
	}

	// 追加仕訳管理テーブル編集
	SEQ_APPEND.nsetseq( SEQ_NO, lastseq - 1 );
	APPEND_SEQ.nsetseq( lastseq, SEQ_NO - 1 );

	REAL_SEQ_NO = lastseq;
}

//
// 元帳検索等で仕訳が追加されたときの 追加仕訳管理テーブル 更新
//
void DBDinpAppendParamReload( int olddacnt, int newdacnt )
{
	CString filter;

	if( olddacnt < newdacnt ) {
		filter.Format( "seq > %d", olddacnt );
	}
	else {
		return;
	}

	// カレントユーザ, SEQ 順
	pDBzm->data->Requery( 0, 0, filter, 0, 0 );

	pDBzm->data->MoveFirst();

	if( pDBzm->data->st == -1 ) {
		return;
	}

	do {

		BOTOM_SEQ = ++SEQ_NO;

		int lastseq;
		lastseq = pDBzm->data->seq;

		// 追加仕訳管理テーブル編集
		SEQ_APPEND.nsetseq( SEQ_NO, lastseq - 1 );
		APPEND_SEQ.nsetseq( lastseq, SEQ_NO - 1 );

		REAL_SEQ_NO = lastseq;
	
	} while( pDBzm->data->MoveNext() == 0 );


	// 挿入対応。
	if( ! pDBzm->insseq_make() ) {
		insseqpos_make( NULL );
	}

	// 08.18 /11
	pDBzm->data->MoveLast();
	CDBINPDataRec	ld;

	int pos = INSPOS_SEQ.ngetseq( pDBzm->data->seq -1 );

	//次データ用にSEQをセット
	pDBzm->m_lastdata.m_seq = pDBzm->data->seq;
	pDBzm->m_lastdata.m_seq++;

	if( pos >= pDBzm->data->seq ) {
		pDBzm->GetCData( &ld );
		ld.m_dtype = 0;
		::SREC = ld;
		// サンプルレコードの更新
	//	p_DBDinpView->SaveDinpDataSREC( _APPEND );
		p_DBDinpView->SaveDinpParaSREC( _APPEND );
	}
}


// 外税仕訳か？
BOOL sotozei_data( CDBINPDataRec *drec )
{
	
	return _sotozei_data( &drec->m_dsign[0] );
}

// 外税仕訳か？
BOOL _sotozei_data( BYTE *dsign )
{
	BOOL bRet = FALSE;
	
	if( (dsign[0] & 0x40) && (dsign[1]&0xf0)==0x00 &&
		(dsign[2] & 0x0f) == 0x01 ) {
			bRet = TRUE;
	}

	return bRet;
}


// 内税仕訳か？
BOOL uchizei_data( CDBINPDataRec *drec )
{
	
	return _uchizei_data( &drec->m_dsign[0] );
}

// 内税仕訳か？
BOOL _uchizei_data( BYTE *dsign )
{
	BOOL bRet = FALSE;
	
	if( (dsign[0] & 0x40) && (dsign[1]&0xf0)==0x10 &&
		(dsign[2] & 0x0f) == 0x01 ) {
			bRet = TRUE;
	}

	return bRet;
}

// 譲渡仕訳か？
BOOL _joto_data( BYTE *dsign )
{
	BOOL bRet = FALSE;
	
	if( (dsign[0] & 0x40) && (dsign[2] & 0x30) == 0x30 ) {
			bRet = TRUE;
	}

	return bRet;
}



// 仕訳を比較して変更されたかどうか？
// 仕訳確定サインが立っていたら無視する
//
BOOL isdiff_data( CDBINPDataRec *newdata, CDBINPDataRec *moto )
{
	BOOL ret;

	// 確定仕訳が修正不可
	if( ! IsModifyConfirmData() ) {
		if( moto->m_dsign[7] & 0x30 ) {
			if( ! (newdata->m_dsign[7] & 0x30) ) {
				if( IsEnkakuUser() )	ret = FALSE;
				else					ret = TRUE;
			}
			else {
				ret = FALSE;
			}
		}
		else {
			pDBzm->KamokuCodeCheck( newdata->m_dbt );
			pDBzm->KamokuCodeCheck( newdata->m_cre );

			ret = (*newdata != *moto);
		}
	}
	else {
		// 確定仕訳を修正可
		pDBzm->KamokuCodeCheck( newdata->m_dbt );
		pDBzm->KamokuCodeCheck( newdata->m_cre );

		ret = (*newdata != *moto);
	}

	return ret;
}


BOOL _isSyzdiff_data( CDBINPDataRec *newdata, CDBINPDataRec *moto )
{
	BOOL ret = FALSE;
	BYTE	newdsign[10] = { 0 };
	BYTE	motodsign[10] = { 0 };

	const int invLen = 14;

	memcpy( newdsign, &newdata->m_dsign[0], 7 );
	memcpy( motodsign, &moto->m_dsign[0], 7 );

	if( (newdsign[0]&0x40) != (motodsign[0]&0x40) ) {
		ret = TRUE;
	}
	else {
		if( newdsign[2] != motodsign[2] ) {
			ret = TRUE;
		}
		else {
			if( (newdsign[0] & 0x40) && (motodsign[0] & 0x40) ) {
				if( (newdsign[2] & 0x0f) == (motodsign[2] & 0x0f) ) {
					//非課税・不課税は、税率の比較しない
					if( (newdsign[2] & 0x0f) == 0 || (newdsign[2] & 0x0f) == 2 ) {
						newdsign[1] &= ~0x0f;
						motodsign[1] &= ~0x0f;
					}
				}
				else {
					ret = TRUE;
				}
			}
			else {
				//消費税対象外の場合は、仮受・仮払のときは、税率で比較する
				int new_dbt = pDBzm->km_syattr( newdata->m_dbt );
				int new_cre = pDBzm->km_syattr( newdata->m_cre );
				int moto_dbt = pDBzm->km_syattr( moto->m_dbt );
				int moto_cre = pDBzm->km_syattr( moto->m_cre );

				if( (new_dbt & 0x0200) || (new_cre & 0x0200) || (moto_dbt & 0x0200) || (moto_cre & 0x0200) ) {
				}
				else {
					newdsign[1] &= ~0x0f;
					motodsign[1] &= ~0x0f;
				}
			}
			if( !ret ) {
				if( memcmp(&newdsign[1], &motodsign[1], 5) != 0 ) {
					ret = TRUE;
				}
			}

			if( !ret ) {
				if( bInvMaster && bInvUse ) {
					if( (newdsign[6] & 0xE0) != (motodsign[6] & 0xE0) ) {
						ret = TRUE;
					}
					if( bInvDisp ) {
						//どちらも登録番号ある
						if( newdata->m_invno[0] && moto->m_invno[0] ) {
							if( memcmp(newdata->m_invno, moto->m_invno, invLen) != 0 ) {
								SRCTrace;
								ret = TRUE;
							}
						}
						else if( newdata->m_invno[0] != moto->m_invno[0] ) {
							ret = TRUE;
						}
					}
				}
			}
		}
	}
//	MyTrace("_isSyzdiff_data newdata->m_invno %s,  moto->invno = %s\n", newdata->m_invno, moto->m_invno);
//MyTrace( "_isSyzdiff_data newdsign [%02x%02x%02x%02x%02x%02x], motodsign [%02x%02x%02x%02x%02x%02x]",
//		newdsign[0],newdsign[1],newdsign[2],newdsign[3],newdsign[4],newdsign[5],  motodsign[0],motodsign[1],motodsign[2],motodsign[3],motodsign[4],motodsign[5] );

	return ret;
}


// 仕訳を比較して消費税が変更されたかどうか？
// 仕訳確定サインが立っていたら無視する
//
BOOL isSyzdiff_data( CDBINPDataRec *newdata, CDBINPDataRec *moto )
{
	BOOL ret = FALSE;

	// 確定仕訳が修正不可
	if( ! IsModifyConfirmData() ) {
		ret = _isSyzdiff_data( newdata, moto );
	}
	else {
		// 確定仕訳を修正可
		ret = _isSyzdiff_data( newdata, moto );
	}

	return ret;
}

/**********************
	諸口・現金貸借
***********************/
void syog_genk( CDBINPDataRec *dta, int add_sub )
{
	if( syogtai( dta, add_sub ) == TRUE )
		syogdsp( (char *)SG_def );
		
	if( genktai( dta, add_sub ) == TRUE )
		genkdsp( (char *)GK_def );
}

/*---------------------
	諸口貸借
----------------------*/
static BOOL syogtai( CDBINPDataRec *dta, int add_sub )
{
	/* 諸口合計 */
	l_defn( 0x16 );

	if( isSYOGT_CODE( dta->m_dbt ) || isSYOGT_CODE( dta->m_cre ) )
	{
		// 外税 の場合、税額を＋しないといけないため
		BOOL sgn = sotozei_data( dta );

		if( isSYOGT_CODE( dta->m_dbt ) ) 
		{
			if( !add_sub ) {
				l_add( (char *)SYOG_d, (char *)SYOG_d, (char *)dta->m_val );
				if( sgn )	l_add( (char *)SYOG_d, (char *)SYOG_d, (char *)dta->m_zei );
			}
			else {
				l_sub( (char *)SYOG_d, (char *)SYOG_d, (char *)dta->m_val );
				if( sgn )	l_sub( (char *)SYOG_d, (char *)SYOG_d, (char *)dta->m_zei );
			}
		}
		if( isSYOGT_CODE( dta->m_cre ) )
		{
			if( !add_sub ) {
				l_add( (char *)SYOG_c, (char *)SYOG_c, (char *)dta->m_val );
				if( sgn )	l_add( (char *)SYOG_c, (char *)SYOG_c, (char *)dta->m_zei );
			}
			else {
				l_sub( (char *)SYOG_c, (char *)SYOG_c, (char *)dta->m_val );
				if( sgn )	l_sub( (char *)SYOG_c, (char *)SYOG_c, (char *)dta->m_zei );
			}
		}

		l_sub( (char *)SG_def, (char *)SYOG_d, (char *)SYOG_c );

		return TRUE;
	}
	return FALSE;
}

static
BOOL isDCKM_CODE( CString& code )
{
	int len;
	BOOL bRet;

	if( pAUTOSEL->DCKM_CODE[0] ) {
		len = strlen( pAUTOSEL->DCKM_CODE );

		if( ! _strnicmp( code,  pAUTOSEL->DCKM_CODE, len ) )
			bRet = TRUE;
		else
			bRet = FALSE;
	}
	else {
		bRet = isGNKIN_CODE( code, Voln1->apno );
	}

	return bRet;
}

/*---------------------
	現金貸借
----------------------*/
static BOOL genktai( CDBINPDataRec *dta, int add_sub )
{
	/* 現金合計	*/
	l_defn( 0x16 );

	// 外税 の場合、税額を＋しないといけないため
	BOOL sgn = sotozei_data( dta );

	if( isDCKM_CODE( dta->m_dbt ) || isDCKM_CODE( dta->m_cre ) )
	{
		if( isDCKM_CODE( dta->m_dbt ) ) {
			if( !add_sub ) {
				l_add( (char *)GENK_d, (char *)GENK_d, (char *)dta->m_val );
				if( sgn )	l_add( (char *)GENK_d, (char *)GENK_d, (char *)dta->m_zei );
			}
			else {
				l_sub( (char *)GENK_d, (char *)GENK_d, (char *)dta->m_val );
				if( sgn )	l_sub( (char *)GENK_d, (char *)GENK_d, (char *)dta->m_zei );
			}
		}
		if( isDCKM_CODE( dta->m_cre ) )	{
			if( !add_sub ) {
				l_add( (char *)GENK_c, (char *)GENK_c, (char *)dta->m_val );
				if( sgn )	l_add( (char *)GENK_c, (char *)GENK_c, (char *)dta->m_zei );
			}
			else {
				l_sub( (char *)GENK_c, (char *)GENK_c, (char *)dta->m_val );
				if( sgn )	l_sub( (char *)GENK_c, (char *)GENK_c, (char *)dta->m_zei );
			}
		}
		l_sub( (char *)GK_def, (char *)GENK_d, (char *)GENK_c );

		// 反転する
		if( pAUTOSEL->DCKM_CODE[0] ) {
			DBKNREC* pKn = pDBzm->DB_PjisToKnrec( CString(pAUTOSEL->DCKM_CODE) );
			if( pKn != NULL ) {
				if( pKn->knatt&0x01 )	l_neg( GK_def );
			}
		}

		return TRUE;
	}
	return FALSE;
}



/*ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ
	ｾｰﾌﾞ ﾃﾞｰﾀ ﾋｮｳｼﾞ (3ﾗｲﾝ ｽｸﾛｰﾙ)
ｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰｰ*/
void datadsp( int zrec_ofs, int sgn, int insln )
{
	int		i, j;
	int		lin;

	if( sgn ) {
		if( insln != -1 ) {	//挿入時
			//挿入行より上をスクロール
			for( i = 0; i < (insln-1); i++ ) {
				DDATA_LINE[i] = DDATA_LINE[i+1];
			}
			if( insln > 1 ) {
				recdsp( &ZREC[0], zrec_ofs, DDATA_LINE, (insln-1) - 1 );
			}
		}
		else {
			scroll_up( DDATA_LINE, SCROLL_NL(), 1 );
			recdsp( &ZREC[0], zrec_ofs, DDATA_LINE, SCROLL_NL() - 1 );
		}
	}
	else	{
		lin = 0;
		for( i = SCROLL_NL()-1; i >= 0; i--, lin++ ) {
			j = zrec_ofs - i;
			if( j < 0 ) j += RECMAX;

			recdsp( &ZREC[0], j, DDATA_LINE, lin );
		}
	}

}

//--------------------------------------------
//	仕訳MAXチェック
//	int seq		追加SEQ番号
//	int sg		メッセージ表示
//--------------------------------------------
int scheck( int seq, int sg )
{
#ifdef MAXCHECK_CUT
	// 仕訳Ｍａｘチェック
	if( pDBzm->m_damax <=  seq )
	{
		if( sg ) 
			return max_msg( sg );
		return( 1 );	// over
	}
#endif

	return( 0 );	// ok
}


int scheck( int sg )
{
#ifdef DB_OLD_CLOSE ////////////////////
	// 残り仕訳数表示はしない(データベースからより)
	p_DBDinpView->inpn_dsp( (int)(Voln1->da_max - Voln1->dlin_c)/*SEQ_NO*/ );
#endif

#ifdef MAXCHECK_CUT
	// 仕訳Ｍａｘチェック
	if( pDBzm->m_damax <= pDBzm->m_dacnt ) {
		if( sg ) 
			return max_msg( sg );
		return( 1 );	// over
	}
#endif

	return( 0 );	// ok
}

//	確定仕訳のモードを取得
//		0:確定仕訳修正不可 
//		1:確定仕訳修正可能
//
int Confirm_Mode()
{
	if( ! (Voln1->tl_cor & 0x10) )	// マスター全体確定 or 参照モード
		return 0;
	else if( ! IsModifyConfirmData() )	// 確定仕訳 修正不可
		return 0;
	else								// 確定仕訳 修正可
		return 1;
}


// 転送仕訳の 背景色取得
COLORREF	GetConfirmBkColor()
{
	if( Confirm_Mode() == 0 ) {
		return RGB_TENSO_DATA;
	}
	else {
		return RGB_TENSOMDFY_DATA;
	}
}


// 背景色が、確定・転送済み仕訳か？
//
BOOL	CheckConfirmBkColor( COLORREF back )
{
	BOOL bRet = FALSE;

	if( Confirm_Mode() == 0 ) {
		bRet = (back == RGB_TENSO_DATA);
		if( ! bRet ) {
			bRet = (back == RGB_KAKUT_DATA);
		}
	}
	else {
		bRet = (back == RGB_TENSOMDFY_DATA);
	}

	return bRet;
}

//------------------------------------
//	確定仕訳修正可能？
//
//------------------------------------
BOOL	IsModifyConfirmData()
{
	BOOL bRet = FALSE;
	int st;

	if( (Voln1->tl_cor & 0x01) ) {
		st = GET_USERTYPE();

		if( st == 0 ) {
			bRet = TRUE;
			if( IsJozuEnvCloudMaster() )
				bRet = FALSE;
		}
		else {
			bRet = FALSE;	//顧問先ユーザーの場合は、修正不可
		}
	}

	return bRet;
}

//------------------------------------
//	遠隔地ユーザか？
//
//------------------------------------
BOOL	IsEnkakuUser()
{
	int st = GET_USERTYPE();

	return (st != 0);
}


// 転送済み仕訳か？
//		-1	修正不可
//		-2	転送済み仕訳で修正不可
//		 0	修正可能
int tline_chk( CDBINPDataRec *drec )
{
	return _tline_chk( &drec->m_dsign );
}

int _tline_chk( CByteArray *dsign )
{
#ifndef CONFIRM_CHECK
	if( ! (Voln1->tl_cor & 0x10) )	// マスター全体確定
		return -1;
	else if( /*! (Voln1->tl_cor & 0x01) &&*/ (dsign->GetAt(7) & 0x10) )// 確定仕訳
		return -1;
	else if( /*! (Voln1->tl_cor & 0x01) &&*/ (dsign->GetAt(7) & 0x20) )// 転送済み仕訳
		return -2;
#else
	if( ! (Voln1->tl_cor & 0x01) && (dsign->GetAt(7) & 0x10) )// 確定仕訳 修正不可
		return -1;
#endif
	else
		return 0;
}

// 修正可能な仕訳か？
//		TRUE:	修正可能
//		FALSE:	修正不可
//
BOOL modefy_ok( int mode, CDBINPDataRec *drec )
{

//MyTrace( "modefy_ok mode = %d, (drec->m_dsign[7] = %02x\n", mode, drec->m_dsign[7]  );

#ifndef CONFIRM_CHECK
	if( ! (Voln1->tl_cor & 0x10) || IsJzSansyo() )	// マスター全体確定
		return FALSE;
	else if( mode != _APPEND && ! IsModifyConfirmData() && (drec->m_dsign[7] & 0x30) )// 確定仕訳 修正不可
		return FALSE;
#else
	if( mode != _APPEND && ! (Voln1->tl_cor & 0x01) && (drec->m_dsign[7] & 0x30) )// 確定仕訳 修正不可
		return FALSE;
#endif
	else
		return TRUE;
}


int max_msg( int sg )
{

char pbuf[MAX_PATH] = {0};

#ifdef REGDATA	// 11.06 /00
	// 定型仕訳テーブル削除
	p_DBDinpView->EndRdata();
#endif

	Buzzer();

	switch( sg )
	{
	case 1:	// 通常
		sprintf_s( pbuf, sizeof pbuf, "仕訳が最大(%d仕訳)まで登録されました。追加入力はできません。\n\n「仕訳検索」に進みますか？\n\n「いいえ」の場合処理を終わります。", Voln1->da_max );
		break;
	case 2:	// 既にＭＡＸ
		sprintf_s( pbuf, sizeof pbuf, "仕訳が既に最大(%d仕訳)まで登録されていました。最後の仕訳は入力できませんでした。\n\n「仕訳検索」に進みますか？\n\n「いいえ」の場合処理を終わります。", Voln1->da_max );
		break;
	default: // 既にＭＡＸ（エンドライト時） sg == 3
		sprintf_s( pbuf, sizeof pbuf, "仕訳が既に最大(%d仕訳)まで登録されていました。最後の仕訳は入力できませんでした。\n\n", Voln1->da_max );
		break;
	}

	switch( myICSMessageBox( pbuf, (sg == 3 ? MB_OK : MB_YESNO)) )
	{
	case IDYES:
		bSCAN_ONLY_MODE = TRUE;
		return 0;
	default:
		return 1;
	}

}


/*------------------------------
	scan mode display
  -------------------------------*/
void inpmdisp( int mode )
{

	if( mode != INP_mode )
	{
		p_DBDinpView->enable_fixval( p_DBDinpView->fixvalcnd() );	// 元背景色強制描画のため

		// 自動項目タイトル欄色表示
		p_DBDinpView->AutoSelIndicator( pAUTOSEL, mode );
	}

#ifdef KEEP_CLOSE
	if( bCONFIRM_MASTER ) {
		// 参照モードか確定で、変更不可
		if( IsJzSansyo() )
			p_DBDinpView->set_inpmsg(mode, "参照モード");
		else
			p_DBDinpView->set_inpmsg(mode, "確　定");
	}
#else
	if( IsMAsterKAKUTEI() ) {
		p_DBDinpView->set_inpmsg(mode, "確　定");
	}
#endif
	else {
		switch( mode ) {
		case _APPEND :
			if( p_DBDinpView->get_InsLine() != -1 ) {
				p_DBDinpView->set_inpmsg( mode, "入力(挿入)" );
			}
			else {
			//	if( IsMasterType(MST_INSERT) ) {
			//		p_DBDinpView->set_inpmsg( mode, "入力(追加)" );
			//	}
			//	else {
					p_DBDinpView->set_inpmsg( mode, "入　力" );
			//	}
			}
			break;
//		case _RETREV :
//			p_DBDinpView->set_inpmsg( mode, "検索項目入力" );
//			break;
		case _KEYSCAN :
			if( ((CMainFrame*)GetDBDinpMainFrame())->IsTkyOnly() ) {
				p_DBDinpView->set_inpmsg( mode, "摘要のみ修正" );
			}
			else
				p_DBDinpView->set_inpmsg( mode, "修　正" );

			break;
		case _MODEFY :
		case _MDFEND :
			p_DBDinpView->set_inpmsg( mode, "修　正" );
			break;
		default	:
			break;
		}
	}
}

// 入力モードをセット
void inpmode_set( int mode )
{
	INP_mode = mode;
}

/*-------------------------
	scan mode reupdate
--------------------------*/
void reupdat( CDBINPDataRec *dta)
{

	SREC = *dta;
	SCAN_rec = *dta;

	//消費税
//	pDBsy->Sy_iniz( dta, 1 );
	DB_SyIniz( dta );
}



/*------------------------------
	scan initial set
-------------------------------*/
void intscan( void )
{
//	if( TKDsw ) resetpag( 0 );

	inpmdisp( _RETREV );

}

/*-------------------------------
		isSTRAGE()
	電子データ保存マスター?
--------------------------------*/
BOOL isSTRAGE()
{
//	return Voln1->e_save ? TRUE : FALSE;		//  1=電子データ保存

	return FALSE;
}

/*-------------------------------
		isDataDelOK()
		データ取消ＯＫ？
--------------------------------*/
BOOL isDataDelOK( int delsgn )
{
	if( !isSTRAGE() )
		return TRUE;	// 電子データ保存以外

	// 電子データ保存
	if( !delsgn )
	{
		// 取消
		switch( myICSMessageBox( "取り消したデータは復活できません。\n\n取り消しますか？", MB_YESNO) )
		{
		case IDYES:
			return TRUE;
		default:
			return FALSE;
		}
	}
	else
	{
		// 取消の取消
		Buzzer();
		myICSMessageBox( "１度取り消したデータを復活させることはできません。", MB_OK );
		return FALSE;

	}
}

//
// 摘要部の取出し（イメージの場合はオール漢字スペース）
char *get_TKS( char *dst, char *tks, int knjn )
{
	if( *tks == (char)0xff )	// イメージ摘要
		kspclr( dst, knjn );
	else
		memmove( dst, tks, knjn*2 );

	return dst;
}


// 08.30 /04 -- 部門名称 表示モードゲット
BOOL GetBmnDspMode()
{
	return DinpSW.IsBumonDisp();
}


//
// 枝番摘要選択関係
// 枝番摘要モジュール
extern CBrnTky m_BrTek;


static void _set_brntkydisp( BOOL bDISP );
static BOOL _get_brntkydisp();
static int _brntky_sch( struct _TKYBRN_SEL *tkbrn, int bmn, CString& code, int kana  );
static void _brntky_dsp( struct _DataTable *sel32D, struct _TKYBRN_SEL *tkbrn, int start_no );

static void _brntky_col( CICSSel *sel32C, struct _TKYBRN_SEL *tkbrn )
{
//	if( code == SYOGT )	// 資金繰諸口枝番
//		sel32C->SetForeColor( SYOGBR_ForeCOL );	// 文字色
//	else				// 枝番摘要
//		sel32C->SetForeColor( BRTEK_ForeCOL );	// 文字色

	sel32C->SetForeColor( tkbrn->BRN_ForeCol );	// 文字色
	sel32C->SetBackColor( BRTEK_BackCOL );	// 背景色
}

//
// 枝番摘要検索＆表示
int brntky_sch( struct _TKYBRN_SEL *tkbrn, struct _DataTable *sel32D, CICSSel *sel32C, int bmn, CString& code, int kana/*=0*/, BOOL bDISP/*=FALSE*/ )
{
	int ret;

	_set_brntkydisp( bDISP );

	if( ret = _brntky_sch( tkbrn, bmn, code, kana ) )
	{
		// 表示
		_brntky_dsp( sel32D, tkbrn, 9 );
		_brntky_col( sel32C, tkbrn );
	//!	sel32C->SetData( (long)sel32D );
		p_DBDinpView->SetICSSel32( (long)sel32D );
	}
	return ret;
}

static int _brntky_sch( struct _TKYBRN_SEL *tkbrn, int bmn, CString& code, int kana  )
{
	BRN_TB *ptbl;
	BOOL ret;
	CString sKana;
	if( kana == 0 )	sKana.Empty();
	else {
		char kanabf[6] = {0};
		kanabf[0] = (char)kana;
		sKana = kanabf;
	}

	ret = m_BrTek.get_brtektb( &ptbl, bmn, code, sKana, AC_MAX, 0, FALSE, GetBrTekMode() );

	if( ret )
	{
		tkbrn->BRN_KCOD = ptbl->code;
		tkbrn->BRN_KANA = ptbl->brn_kn50;
		tkbrn->BRN_PAGE = ptbl->page_now-1;
		tkbrn->BRN_ForeCol = ptbl->ForeCol;

		return 1;	// found
	}
	else
	{
//		memset( tkbrn, '\0', sizeof( struct _TKYBRN_SEL) );
		tkbrn->TS_Reset();

		return 0;	// Not found
	}
}

//
// 枝番摘要表示ページセット
static void _brntky_dsp( struct _DataTable *sel32D, struct _TKYBRN_SEL *tkbrn, int start_no )
{
	BRN_TB *ptbl;
	BOOL ret;
	int i;
	char fmt[20];

	ret = m_BrTek.get_brtektb( &ptbl, tkbrn->BRN_BMN, tkbrn->BRN_KCOD, tkbrn->BRN_KANA, AC_MAX, tkbrn->BRN_PAGE, FALSE, GetBrTekMode() );

	if( ret )
	{

		for( i = 0 ; i < AC_MAX ; ++i )
		{
			(sel32D+i)->number = start_no + i;
			if( i < ptbl->rec_n )
			{
				wsprintf( fmt, "%%.%ds", get_strcnt( (char*)(LPCTSTR)(ptbl->pBRN_REC+i)->brmsg, 21 ));
				(sel32D+i)->Data.Format( fmt, (ptbl->pBRN_REC + i)->brmsg );
			}
			else
				(sel32D+i)->Data = "";
		}
	}
}

//
// 枝番摘要ページアクセス
int brntky_page( struct _TKYBRN_SEL *tkbrn, struct _DataTable *sel32D, CICSSel *sel32C, int page )
{
	BRN_TB *ptbl;
	BOOL ret;

	ret = m_BrTek.get_brtektb( &ptbl, tkbrn->BRN_BMN, tkbrn->BRN_KCOD, tkbrn->BRN_KANA, AC_MAX, page, FALSE, GetBrTekMode() );

	if( ret )
	{
		tkbrn->BRN_PAGE = ptbl->page_now-1;

		// 表示
		_brntky_dsp( sel32D, tkbrn, 9 );
		_brntky_col( sel32C, tkbrn );
	//!	sel32C->SetData( (long)sel32D );
		p_DBDinpView->SetICSSel32( (long)sel32D );

		return 1;	// OK
	}
	else
		return 0;	// over
}

//
// 科目.枝番リード
int brntky_get( CString *code, int *brn, struct _TKYBRN_SEL *tkbrn, int no )
{
	BRN_TB *ptbl;
	BOOL ret;

	ret = m_BrTek.get_brtektb( &ptbl, tkbrn->BRN_BMN, tkbrn->BRN_KCOD, tkbrn->BRN_KANA, AC_MAX, tkbrn->BRN_PAGE, FALSE, GetBrTekMode() );

	if( ret && no > 0 && no <= ptbl->rec_n )
	{
		*code = ptbl->code;
		*brn = (ptbl->pBRN_REC + (no - 1))->brn;

		return 1;	// OK
	}
	else
		return 0;	// NO

}

//#* 枝番表示モードサイン ... 03.18 /02
static BOOL bBRTKDISP = FALSE;

static void _set_brntkydisp( BOOL bDISP )
{
	bBRTKDISP = bDISP;
}

static BOOL _get_brntkydisp()
{
	return bBRTKDISP;
}
//----------------------------------------

//
// 枝番摘要表示モードゲット
int GetBrTekMode()
{
	if( _get_brntkydisp() )
		return BT_NOMALBRN_TEK|BT_SYOGTBRN_TEK;

	return _GetBrTekMode();
}

//#* 03.18 /02
// 枝番摘要表示モードゲット ( ファイルのサインのみ ) 表示用
// get_brtekmsg で Call
int _GetBrTekMode()
{	
	switch( DinpSW.isBrTekDisp() )
	{
	case 0x01: return BT_NOMALBRN_TEK;
	case 0x02: return BT_SYOGTBRN_TEK;
	case 0x03: return BT_NOMALBRN_TEK|BT_SYOGTBRN_TEK;
	}
	return 0;
}


//-----------------------------------------------------------
// 入力枝番タイプ
//	int nowtype			枝番入力ﾀｲﾌﾟ INP_type(LINE_CND)
//	CString nowcode		現　科目コード
//	int nowbrn			現　枝番コード
//	CString mdfycode	修正科目コード
//	int mdfybrn			修正枝番コード
//
//	return =0 科目枝番が変更になって、枝番が未入力 
//		   =1 科目枝番が変更になって、枝番が入力された
//
//-----------------------------------------------------------
int brn_inptype( int nowtype, CString nowcode, int nowbrn, CString mdfycode, int mdfybrn )
{
	// 科目が変わった場合
	if( nowcode != mdfycode )
	{
		if( mdfybrn == -1 )
			return 0;
		else
			return 1;
	}
	// 枝番が変わった場合
	else if( nowbrn != mdfybrn )
	{
		if( mdfybrn == -1 )
			return 0;
		else
			return 1;
	}
	return nowtype;
}

//--------------------------------------------------------------
// 初回入力枝番タイプ
//	int denp = 1: 伝票形式からの呼び出し
//	return =0 ... 枝番未入力, = 1 枝番入力, 99(伝票形式時)
//
//--------------------------------------------------------------
int brn_dfltinptype( int brn, int denp/*=0*/ )
{
	int rval = 1;
	if( denp )	rval = 99;

	return (brn == -1 ? 0 : rval);
}

//-----------------------------------------------------------
// 科目.枝番 に変更があったか？
//	return =1 科目か枝番に変更あり
//-----------------------------------------------------------
int is_mdfybrnitem( CString nowcode, int nowbrn, CString mdfycode, int mdfybrn )
{
	return ( nowcode != mdfycode || nowbrn != mdfybrn );
}



//---------------------------------------------------------
//	レコードに 入力開始日付をセット
//
//
//---------------------------------------------------------
void set_inpdate( CDBINPDataRec* data )
{
	data->m_ddate = pDBzm->zvol->ip_ymd;

	CVolDateDB vd;
	VDPAC vpac;
	vd.db_vd_dpacGen( 0, 0, pDBzm->zvol->ip_ymd, &vpac, pDBzm );

	data->m_mofs = vpac.Doffset;
	data->m_ksign = (vpac.Dsgn & 0x80) ? 1 : 0;
}

// 04.03 /03 諸口ブザー **********************************************************
typedef struct _SYO_BUZ
{
	BOOL	bFlag;		// 初回入力されたか
	BOOL	bDateBuz,	// 日付変更     buzzer サイン
			bCnoBuz,	// 伝票番号変更 buzzer サイン
			bCnoRing;	//		〃		buzzer を鳴らしたか
	CDBINPDataRec	sb_data,	// 入力データ
					prev_dt;	// 変更入力した 日付・伝票番号格納レコード
	_SYO_BUZ(){
		bFlag = FALSE;
		bDateBuz = FALSE;
		bCnoBuz = FALSE;
		bCnoRing = FALSE;
		sb_data.Reset();
		prev_dt.Reset();
	}
} SYO_BUZ;

static SYO_BUZ SB;

// 諸口ブザー元仕訳セット
//	 新規ラインが書き込みされた時に よばれる
//
void set_syobz_data( CDBINPDataRec* data )
{
	SYO_BUZ* psb;
	psb = &SB;

	if( !psb->bFlag ) psb->bFlag = TRUE;

	// 諸口ブザー元仕訳
	psb->sb_data = *data;
	// 入力変更格納 仕訳
	psb->prev_dt = *data;
}


/*================================================
	諸口ブザーを鳴らすか

	int pn .... DATE_PN --> 日付
				DENP_PN --> 伝票番号
				TKY_PN	--> 摘要欄
================================================*/
int	chk_syobz( CDBINPDataRec* data, int pn )
{
	SYO_BUZ* psb;
	int	ret = 0;
	psb = &SB;

	// 新規ラインを一度でも入力した場合
	if( psb->bFlag ) {
		// 諸口貸借が合っていない
		if( l_test( SG_def ) != 0 ) {
			// 日付が違っているか 伝票番号が変更された( 09.17 /03 ) 場合
			// Buzzer サイン ON
			if( pn == DATE_PN ) {
				if( psb->sb_data.m_ddate != data->m_ddate || psb->sb_data.m_mofs != data->m_mofs ) {
					// 前回入力されたものと同じ時は、鳴らさない
					if( psb->prev_dt.m_ddate == data->m_ddate && psb->sb_data.m_mofs == data->m_mofs)
						psb->bDateBuz = FALSE;
					else {
						psb->bDateBuz = TRUE;
					}
				}
				psb->prev_dt.m_ddate = data->m_ddate;
			}
			else if( pn == DENP_PN ) {
				if( psb->sb_data.m_cno != data->m_cno ) {
					// 前回入力されたものと同じ時は、鳴らさない
					if( psb->prev_dt.m_cno == data->m_cno )
						psb->bCnoBuz = FALSE;
					else {
						psb->bCnoBuz = TRUE;
					}
				}
				psb->prev_dt.m_cno = data->m_cno;
			}
		}
	}

	return ret;
}

/*================================================
	諸口ブザー を 鳴らさないといけないか？
	int pn .... DATE_PN --> 日付
				DENP_PN --> 伝票番号
				
			仕訳書き込み時
				TKY_PN		摘要欄
				HUSEN_PN	付箋

	return TRUE  -> 鳴らす
		   FALSE -> 鳴らさない
================================================*/
BOOL is_syobuzzer_on( int pn )
{
	BOOL bRet = FALSE;
	SYO_BUZ* psb;
	psb = &SB;

	if( pn == DATE_PN ) {
		if( psb->bDateBuz ) {
			bRet = TRUE;
			psb->bDateBuz = FALSE;	// 日付ブザーサイン OFF	
		}
	}
	else if( pn == DENP_PN ) {
		if( psb->bCnoBuz ) {
			bRet = TRUE;
			psb->bCnoBuz = FALSE;	// 伝票ブザーサイン OFF	
		}
	}

	return bRet;
}


//--------------------------------------------------------------
//	指定コードの部門残高があるか？(部門未入力チェック用 )
//		return  1 部門残高がある
//				0 なし
//--------------------------------------------------------------
int bmnzan_count( CDatabase* database, CString& code, CString& errmsg )
{
	CBmnNoInp	bminp(database);
	int	cnt = 0;

	TRY {
		bminp.m_query.Format( "kzkcd = '%s'", code );

		if( ! bminp.Open() ) {
			errmsg = _T( "部門未入力レコードセットをオープンできません！" );
			return -1;
		}

		if( bminp.IsBOF() ) {
			//レコードなし
			bminp.Close();
		}
		else {
			do {
				// 部門コード 範囲チェック
				if( BmChk.BmnHaniCheck( bminp.m_count, code ) ) {
					cnt = 1;
					break;
				}

				bminp.MoveNext();
			} while( ! bminp.IsEOF() );

			bminp.Close();
		}

	}
	CATCH( CMemoryException, me ) {
		me->GetErrorMessage( errmsg.GetBuffer(_MAX_PATH), _MAX_PATH );
		errmsg.ReleaseBuffer();
		return -1;	
	}
	CATCH( CDBException, de ) {
		errmsg = de->m_strError;
		return -1;
	}
	END_CATCH

	return cnt;
}


//電子帳簿保存マスターでの仕訳挿入チェック
BOOL isInsertMaster()
{
	BOOL bRet = TRUE;

	if( Voln1->e_save ) {
		PROGRAM_OK = FALSE;

		myICSMessageBox("電子帳簿保存マスターでは、仕訳の挿入はできません！");
		bRet = FALSE;

		PROGRAM_OK = TRUE;
	}

	return bRet;
}


void SREC_Trace(char* pstr)
{
	MyTrace( "%s-@SREC_Trace m_seq = %d, m_ddate = %d, m_dbt = %s, m_cre = %s", pstr,
		SREC.m_seq, SREC.m_ddate, SREC.m_dbt, SREC.m_cre );
}

// type == 0 半角ハイフンが文字列にある場合は、半角長音に変換する。
// type == 1 全角長音    が文字列にある場合は、全角ハイフンに変換する。
// type == 2 全角マイナスが文字列にある場合は、全角長音に変換する。
int HaihunCheck( LPCTSTR string, CString& chgStr, int type )
{
	chgStr = string;
	int ret = 0;

	if( type == 0 ) {
		int idx = chgStr.Find( '-' );

		if( idx != -1 ) {
			chgStr.Replace( '-', 'ｰ' );
			ret = 1;
		}
	}
	else if( type == 1 ) {
		int idx = chgStr.Find( "ー" );

		if( idx != -1 ) {
			chgStr.Replace( "ー", "－" );
			ret = 1;
		}
	}
	else {
		int idx = chgStr.Find( "－" );

		if( idx != -1 ) {
			chgStr.Replace( "－", "ー" );
			ret = 1;
		}
	}

	return ret;
}


void DenpInputSyzMdfyVectorSet( DENP_INPUTPAR* para )
{
	int last = pDBzm->m_lastdata.m_seq;

	for( int n = 0; n < last; n++ ) {
		int sg = SYZMDFY_BIT.bitncnd( n );
		if( sg ) {
			para->syzseq.push_back( n+1 );
		}
	}
}


void DenpEnd_addmdfcheck( DENP_INPUTPAR* para )
{
	vector<int>::iterator	it, itend;
	vector<int>::iterator	it2;

	it	= para->addseq.begin();
	it2	= para->addins_seq.begin();
	int bseq;
	CDBINPDataRec rec;

	itend = para->addseq.end();

	for( ; it != para->addseq.end(); it++, it2++ ) {
		CString stmp;
		stmp.Format( "seq = %d", *it );

		pDBzm->data->Requery( -2, 0, stmp, 0, 0 );
		if( pDBzm->data->st == -1 )
			continue;

		bseq = -1;
		if( it2 != para->addins_seq.end() ) {
			bseq = *it2;
		}

		pDBzm->GetCData( &rec );
		DBDinpAppendParamUpdate( &rec, bseq );
		pDBzm->m_lastdata = rec;
		pDBzm->m_lastdata.m_seq++;
	}

	it = para->mdfseq.begin();
	for( ; it != para->mdfseq.end(); it++ ) {
		MODEFY_BIT.onbitn( *it - 1  );	
	}
	it = para->syzseq.begin();
	for( ; it != para->syzseq.end(); it++ ) {
		SYZMDFY_BIT.onbitn( *it - 1 );
	}

	pDBzm->IsModify() = TRUE;
}


BOOL IsNotMultiWindow()
{
	int typ = GetMultiWindowType();

	if( typ == MULTIWINDOW_NOP || typ == MULTIWINDOW_NONE )
		return TRUE;

	return FALSE;
}




//Value
extern struct _DATA_LINE	INP_LINE;

//現在ビュー比率
static VIEW_RATE NOW_VIEW_RATE;

/***************************
	ビュー比率セーブ
****************************/
void set_viewrate( double w, double h )
{
	NOW_VIEW_RATE.w = w;
	NOW_VIEW_RATE.h = h;
}

/***************************
	ビュー比率ゲット
****************************/
void get_viewrate( double &w, double &h )
{
	w = NOW_VIEW_RATE.w;
	h = NOW_VIEW_RATE.h;
}

/**************************
	Shift key check
***************************/
BOOL is_SHIFT()
{
	return( GetKeyState( VK_SHIFT ) < 0 );
}

/**************************
	Ctrl key check
***************************/
BOOL is_CTRL()
{
	return( GetKeyState( VK_CONTROL ) < 0 );
}

/***********************************
	入力エラーメッセージボックス
************************************/
void inp_errmsg( int pn, int mode )
{
	char txt[128];
	char *item;
	char *mode_msg;

	Buzzer();
	switch( pn )
	{
	case DEBT_PN:	item = "借方科目が"; break;
	case CRED_PN:	item = "貸方科目が"; break;
	case VAL_PN:	item = "金額が"; break;
	default:
		return;
	}

	switch( mode )
	{
	case 0:	mode_msg = "入力されていません。"; break;
	default:
		return;
	}

	sprintf_s( txt, sizeof txt, "%s%s", item, mode_msg );

	myICSMessageBox( txt );
}

/**************************************
		inpline_inz()
	入力ラインバッファイニシャライズ
***************************************/
void inpline_inz( struct _DATA_LINE *dtline )
{
	int i;

	dtline->ZREC_NO = 0;

	for( i = SEQ_PN ; i <= DOCEVI_PN ; ++i )
	{
		switch( i )
		{
		case SEQ_PN:
			if( dtline == &INP_LINE )
				dtline->_XXDTA.dd_seq = REAL_SEQ_NO + 1;
			else
				dtline->_XXDTA.dd_seq = 0;
			break;
		case DATE_PN:
			dtline->_XXDTA.dd_ddate = -1;
			dtline->_XXDTA.dd_mofs = 0;
			dtline->_XXDTA.dd_ksign = 0;
			::ZeroMemory( dtline->BcdDate, sizeof dtline->BcdDate );
			break;
		case DENP_PN:
			dtline->_XXDTA.dd_cno = -1;
			break;
		case BMON_PN:
			dtline->_XXDTA.dd_dbmn = -1;
			dtline->_XXDTA.dd_dbmn_etc.Empty();
			break;
		case KOJI_PN:
			dtline->_XXDTA.dd_dkno.Empty();
			dtline->_XXDTA.dd_dkno_etc.Empty();
			break;
		case CBMN_PN:
			dtline->_XXDTA.dd_cbmn = -1;
			dtline->_XXDTA.dd_cbmn_etc.Empty();
			break;
		case CKJI_PN:
			dtline->_XXDTA.dd_ckno.Empty();
			dtline->_XXDTA.dd_ckno_etc.Empty();
			break;
		case DEBT_PN:
			dtline->_XXDTA.dd_dbt.Empty();
			dtline->_XXDTA.dd_deb_name.Empty();
			dtline->_XXDTA.dd_dbr = -1;
			dtline->_XXDTA.dd_deb_etc.Empty();
			break;
		case CRED_PN:
			dtline->_XXDTA.dd_cre.Empty();
			dtline->_XXDTA.dd_cre_name.Empty();
			dtline->_XXDTA.dd_cbr = -1;
			dtline->_XXDTA.dd_cre_etc.Empty();
			break;
		case VAL_PN:
			_fillbuf( (char *)&dtline->_XXDTA.dd_val, sizeof(dtline->_XXDTA.dd_val), 0);
			break;
		case ZEI_PN:
			_fillbuf( (char *)&dtline->_XXDTA.dd_zei, sizeof(dtline->_XXDTA.dd_zei), 0);
			dtline->_XXDTA.dd_ctbl[DATA_ZEI].back = (COLORREF)-1; 
			break;
		case SKBN_PN:
			dtline->_XXDTA.dd_syz_str.Empty();
			break;
		case HJIYU_PN:
			dtline->_XXDTA.dd_hjiyu.Empty();
			break;
		case TKY_PN:
			dtline->_XXDTA.dd_tek.tekiyo.Empty();
			dtline->_XXDTA.dd_tek.image = NULL;
			break;
		case SYON_PN:
			dtline->_XXDTA.dd_snumber.Empty();
			break;		
		case TGDT_PN:
			dtline->_XXDTA.dd_tdate = -1;
			dtline->_XXDTA.dd_tdstr.Empty();
			break;
		case HSEN_PN:
			dtline->_XXDTA.dd_husen = 0;
			dtline->_XXDTA.dd_hscomment.Empty();
			break;
		case DOCEVI_PN:
			dtline->_XXDTA.dd_docevi.Empty();
			break;

		default:
			continue;
			break;
		}
		//コンディションサイン OFF
		memset( (char *)&dtline->LINE_CND[i], 0, sizeof(dtline->LINE_CND[i]) );
		dtline->ImgSEQ = 0;

		// 09.24 /13
		dtline->_XXDTA.dd_dsign = 0;
		
		dtline->_XXDTA.dd_ctbl[DATA_SYZ].back = (COLORREF)-1;
	}

	dtline->_XXDTA.dd_ctbl[DATA_SEQ].back = (COLORREF)-1; //[改良依頼220255]
}

/************************************
	入力サンプル行 SREC のリセット
*************************************/
void set_SREC( CDBINPDataRec *srec, CDBINPDataRec *zrec, struct _DATA_LINE *data_line, int scroll_nl, struct  _DATA_LINE *inp_line, int inp_mode )
{
	int i,j;
	CDBINPDataRec *pzrec;	//zrec ポインタ

	data_line += (scroll_nl - 1 );	//Bottom

	for( i = 0 ; i < scroll_nl ; ++i, --data_line )
	{
		if( !data_line->ZREC_NO )	// データなし
			return;

		pzrec = zrec + (data_line->ZREC_NO - 1);	// 目的 zrec[]

		if( srec->m_seq == pzrec->m_seq )	// 該当行発見
		{
			if( inp_mode != _APPEND )
			{
				*srec = *pzrec;
				return;
			}
			for( j = DATE_PN ; j <= HSEN_PN ; ++j )
			{
				//未入力の項目のみ ZREC[] から SREC へ
				//但し金額、消費税区分は行わない。
				if( j != TKY_PN && inp_line->LINE_CND[ j ].INP_sg )
					continue;

				switch( j )
				{
				case DATE_PN:
					srec->m_ddate = pzrec->m_ddate;
					srec->m_ksign = pzrec->m_ksign;
					srec->m_mofs = pzrec->m_mofs;
					break;
				case DENP_PN:
					srec->m_cno = pzrec->m_cno;
					break;
				case BMON_PN:
					srec->m_dbmn = pzrec->m_dbmn;
					break;
				case KOJI_PN:
					srec->m_dkno = pzrec->m_dkno;
					break;
				case DEBT_PN:
					srec->m_dbt = pzrec->m_dbt;
					srec->m_dbr = pzrec->m_dbr;
					break;
				case CBMN_PN:
					srec->m_cbmn = pzrec->m_cbmn;
					break;
				case CKJI_PN:
					srec->m_ckno = pzrec->m_ckno;
					break;
				case CRED_PN:
					srec->m_cre = pzrec->m_cre;
					srec->m_cbr = pzrec->m_cbr;
					break;
				case VAL_PN:
					memmove( srec->m_val, pzrec->m_val, sizeof(srec->m_val));
					break;
				case ZEI_PN:
					memmove( srec->m_zei, pzrec->m_zei, sizeof(srec->m_zei));
					break;
				case TKY_PN:
					srec->m_tekiyo = pzrec->m_tekiyo;
					break;
				case SYON_PN:
					srec->m_rnumber = pzrec->m_rnumber;
					break;
				case TGDT_PN:
					srec->m_tdate = pzrec->m_tdate;
					break;
				case SKIN_PN:
					break;
				case HSEN_PN:
					break;
				}
			}
			return;
		}
	}
}



static int bINP_mode = _APPEND;
int* fncINP_mode()
{
	return &bINP_mode;
}


// 現在行数を返送
int SCROLL_NL()
{
	if( IsMAsterKAKUTEI() ) {	// 確定マスター
		return SCROLL_NL_;
	}
	else {
		return (INP_mode == _KEYSCAN ? SCROLL_NL_ : SCROLL_NL_ - 1);
	}
}

// 業種・売り仕入マスターチェック
int is_symstyp()
{
int ret;

	if( pDBsy->IsSyohizeiMaster() )
	{
		if( pDBsy->IsKaniMaster() )
			ret = KANI_SYMST;
		else if( pDBsy->IsKobetsuMaster() )
			ret = KOBET_SYMST;
		else
			ret = NORMAL_SYMST;
	}
	else
		ret = NORMAL_MST;

	return ret;
}

// 消費税仕訳か？　(完成振替を除く)
BOOL is_syohizeisiwake(CDBDATA* dbdata)
{
	CDBINPDataRec drec;
	pDBzm->GetCDBData(&drec, dbdata);

	return is_syohizeisiwake(&drec);
}

// 消費税仕訳か？　(完成振替を除く)
BOOL is_syohizeisiwake(CDBINPDataRec* data)
{
	if( pDBzm != NULL ) {
		if( pDBzm->zvol->apno == 0x20 ) {
			// m_dsign[6] D5bitが免税事業者からの課税仕入れのサインとなり、==で判定できないため修正
			//if (data->m_dsign[0] & 0x40 && data->m_dsign[4] == 0x1f && data->m_dsign[6] == 0x10) {
			if( data->m_dsign[0] & 0x40 && data->m_dsign[4] == 0x1f && data->m_dsign[6] & 0x10 ) {
				//	return FALSE;
			}
		}
	}
	BOOL ret = FALSE;
	ret = (data->m_dsign[0] & 0x40) ? TRUE : FALSE;

	return ret;
}


// 輸出売上仕訳か？
BOOL is_yusyutsusiwake(CDBDATA* dbdata)
{
	CDBINPDataRec drec;

	pDBzm->GetCDBData(&drec, dbdata);

	return is_yusyutsusiwake(&drec);
}

// 輸出売上仕訳か？　
BOOL is_yusyutsusiwake(CDBINPDataRec* data)
{
	BOOL ret = FALSE;
	if( data->m_dsign[0] & 0x40 ) {
		if( data->m_dsign[4] == 0x0d ) {
			ret = TRUE;
		}
		else {
			if( pDBzm->km_yusyutsu(data->m_dbt) || pDBzm->km_yusyutsu(data->m_cre) )
				ret = TRUE;
		}
	}
	return ret;
}


// 消費税がデフォルトと異なる税額か？
BOOL isnot_defzeigaku(CDBDATA* dbdata, COLORREF& backcol)
{
	CDBINPDataRec drec;
	pDBzm->GetCDBData(&drec, dbdata);

	return isnot_defzeigaku(&drec, backcol);
}

// 消費税がデフォルトと異なる税額か？
BOOL isnot_defzeigaku(CDBINPDataRec* data, COLORREF& backcol)
{
	BOOL ret = FALSE;

	if( is_sotomensiwake(data) ) {
		ret = TRUE;
		backcol = RGB_ZEI_MDFY;
		return ret;
	}

	if( data->m_dsign[0] & 0x40 && (data->m_dsign[2] & 0x0f) == 1 ) {
		LPCTSTR zeistr = pZeiCl->GetDefZeiData(data);
		CArith ar(0x16);
		char def_zei[10] = { 0 };
		ar.l_input(def_zei, (void*)zeistr);
		//MyTrace("seq = %d, def zeistr = %s\n", data->m_seq, zeistr);
		//L_PRINT("data->m_zei=", data->m_zei);
		if( ar.l_cmp(def_zei, data->m_zei) != 0 ) {
			//デフォルトと異なる場合、背景色を取得
			backcol = RGB_ZEI_MDFY;
			//	if (_tline_chk(&data->m_dsign) < 0 && IsModifyConfirmData()) {
			//		backcol = RGB_SYZ_MDFY;
			//	}
			ret = TRUE;
		}
	}
	return ret;
}


extern
BOOL IsKaribarai(DWORD kncd);
extern
BOOL IsKariuke(DWORD kncd);

//消費税欄に止まる仕訳か？
BOOL IsMustStopSyz(CDBINPDataRec* data)
{
	BOOL bret = FALSE;

	if( SyzStopMode() ) {
		if( is_syohizeisiwake(data) ) {
			bret = TRUE;
		}
		else {
			int syattr;

			syattr = pDBzm->km_syattr(data->m_dbt);
			if( syattr & 0x700 ) {
				bret = TRUE;
			}

			syattr = pDBzm->km_syattr(data->m_cre);
			if( syattr & 0x700 ) {
				bret = TRUE;
			}
		}
	}
	return bret;
}



// 外税仕訳で免税事業者等からの課税仕入れ 仕訳か？
BOOL is_sotomensiwake(CDBINPDataRec* data)
{
	BOOL ret = FALSE;

	if( !bInvMaster )
		return FALSE;

	// 外税仕訳か？
	if( _sotozei_data(&data->m_dsign[0]) ) {
		if( data->m_dsign[6] & 0x20 ) {
			ret = TRUE;
		}
	}
	return ret;
}

// 外税仕訳で免税事業者等からの課税仕入れ 仕訳か？
BOOL is_sotomensiwake(CDBDATA* dbdata)
{
	BOOL ret = FALSE;

	if( !bInvMaster )
		return FALSE;

	// 外税仕訳か？
	if( _sotozei_data(&dbdata->dsign[0]) ) {
		if( dbdata->dsign[6] & 0x20 ) {
			ret = TRUE;
		}
	}
	return ret;
}


//仮払科目の場合で、免税がつけれられる仕訳区分か？
BOOL is_karibarai_menzei(BYTE* dsign)
{
	BOOL bRet = TRUE;

	switch( dsign[4] ) { //仮払、以下の消費税区分は登録番号を付与しない
	case 0x04: case 0x0A: case 0x0B: //輸入仕入、棚卸加算・減算
	case 0x1C:						 //精算
	case 0x20: case 0x21: case 0x22: //輸入(国税), 輸入(地方税), 輸入(国地合算),
		bRet = FALSE;
		break;
	default:
		break;
	}
	return bRet;
}


