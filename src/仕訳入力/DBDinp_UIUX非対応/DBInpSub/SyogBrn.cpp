/*****************************************************


		諸口枝番モジュール

		(CBrTek のスレーブ)

	SyogBrn.cpp : インプリメンテーション ファイル

		Auth.	:	麓
		File	:	SyogBrn.h SyogBrn.cpp
		Date	:	11.27 /98

Date		Comment
---------	------------------------------------------
11.27 /98	Begin.
******************************************************/
#include "stdafx.h"
#include "stdio.h"

#include "BrnTkyDef.h"
#include "SyogBrn.h"

#include "DBInpSubFunc.h"
#include "KJVOL.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 資金繰諸口枝番文字色
#define SYOGBR_ForeCOL		RGB(   0,   0, 128)		//RGB(  0,  0, 255)


/////////////////////////////////////////////////////////////////////////////
// CSyogBrn

/*----------------------------------
	テーブルフリー
-----------------------------------*/
static void tbl_free( BRN_TB *tbl )
{
	if( tbl->pBRN_REC )
		delete []tbl->pBRN_REC;
	tbl->pBRN_REC = NULL;
}

/*----------------------------------
	テーブル アロケート
	-> テーブルマックス
-----------------------------------*/
static int tbl_aloc( BRN_TB *tbl, long max )
{
	if( tbl->pBRN_REC = new BRN_REC[max] )
	{
		return max;
	}
	else
	{
		return 0;
	}
}


///////////////////////////////////////////////////////////////////////
// コンストラクタ
CSyogBrn::CSyogBrn()
{
	syogbrn_tbliniz();
	// 作成時のみクリア
	// 以降は、tbl_free() で行う
	m_pApSyogBrnTBL.pBRN_REC = NULL;

	m_pZm	= NULL;
	m_pAna	= NULL;

	m_bKmcod = FALSE;
//_CrtSetBreakAlloc(28787);

}

// デストラクタ
CSyogBrn::~CSyogBrn()
{
	// デストラクタ以前に、分析表は、開放すること！
	tbl_free( &m_pApSyogBrnTBL );
}


//==========================================
//	諸口枝番クラスをイニシャライズ
//==========================================
int	CSyogBrn::syogbrn_iniz( CDBZmSub* pZm )
{
	m_pZm = pZm;

	if( m_pAna ) {
		m_pAna->Close();
		delete m_pAna;
		m_pAna = NULL;
	}

	if( (m_pAna = new CDBAna) == NULL ) {
		return -1;
	}

	if( m_pAna->Open( m_pZm->m_database ) != 0 )
		return -1;

	// クエリーしておく
	CString filter;
	filter.Format( "hyono = 3 and itmno > 0" );

	class CANAHYOITM	*pa;
	pa = m_pAna->anahyoitm;

	pa->Requery( filter );
//return 0;

	m_itmData.RemoveAll();
	pa->MoveFirst();

	CANAHYOITMdata	data;
	data.clear();

	if (pa->st != -1 && syogbrn_master()) {
		do {

			data.hyono	= pa->hyono;
			data.seq	= pa->seq;
			data.itmno	= pa->itmno;
			data.itmcd	= pa->itmcd;
			data.kriksi = pa->kriksi;
			data.nmatt	= pa->nmatt;
			data.itmsgn.Copy( pa->itmsgn );
			data.mdfnmkbn = pa->mdfnmkbn;
			data.regnm = pa->regnm;
			data.mdfnm = pa->mdfnm;

			m_itmData.Add(data);

		} while (pa->MoveNext() == 0);
	}

	int rcnt = pa->GetRecordCount();
	tbl_aloc( &m_pApSyogBrnTBL, rcnt+5 );

	// 初回作成
	m_ApSyogBrnNUM = MakeApSyogBrn( &m_pApSyogBrnTBL, TRUE );

	return 0;
}


//================================================
//	諸口枝番クラスを開放(再読み込み等のため)
//================================================
void	CSyogBrn::syogbrn_free()
{

	if( m_pAna ) {
		m_pAna->Close();
		delete m_pAna;
		m_pAna = NULL;
	}

	tbl_free( &m_pApSyogBrnTBL );
	syogbrn_tbliniz();

}

int CSyogBrn::make_syogbrn()
{
	m_ApSyogBrnNUM = MakeApSyogBrn(&m_pApSyogBrnTBL, TRUE);
	return 0;
}

//==========================================
//	アクセス用テーブル作成
//==========================================
int CSyogBrn::MakeApSyogBrn( BRN_TB *tbl, BOOL full )
{
	int n;
	class CANAHYOITMdata	*pa;

	tbl->code = SYOGT;		// 科目コード
	tbl->brn_kn50.Empty();	// 50音
	tbl->full = full;		// 科目名称も

	n = 0;

	CString strSyog;

	if (IsKmkcodeDisp()) {
		DBKNREC* pKn;
		pKn = DB_PjisToKnrec(SYOGT, m_pZm);
		if (pKn) {
			strSyog.Format("%04d諸口.", pKn->knicod);
		}
		else {
			strSyog = "諸口.";
		}
	}
	else {
		strSyog = "諸口.";
	}

	if( syogbrn_master() ) {

		for( int itm = 0; itm < m_itmData.GetCount(); itm++ ) {
			pa = &m_itmData[itm];

			if( (pa->itmsgn[2] & 0x01) )
				continue;

			if( pa->mdfnmkbn == 0 ) {
				if( !full )
					(tbl->pBRN_REC + n)->brmsg.Format( ".%04d%s", pa->itmno, pa->regnm );
				else
					(tbl->pBRN_REC + n)->brmsg.Format( "%s%s",  strSyog, pa->regnm );
			}
			else {
				if( !full )
					(tbl->pBRN_REC + n)->brmsg.Format( ".%04d%s", pa->itmno, pa->mdfnm );
				else
					(tbl->pBRN_REC + n)->brmsg.Format( "%s%s",  strSyog, pa->mdfnm );
			}
			(tbl->pBRN_REC + n)->kn50.Empty();		// 50音
			(tbl->pBRN_REC + n)->brn = pa->itmno;	// 枝番
			(tbl->pBRN_REC + n)->tkno = 0;			// 摘要コード

			++n;

		}
	}

	//建設(工事)マスター 追加
	if (m_pZm->zvol->apno == 0x20 && m_pZm->object_check(2, "KJVOL") != 0) {
		CKJVOL kjv(m_pZm->m_database);
		CString strError;

		TRY{
			if (kjv.Open()) {
				while (!kjv.IsEOF()) {
					kjv.MoveNext();
				}
				if (kjv.GetRecordCount() > 0) {
					int kj_eda;
					CString	edanam;

					for (int eda = 0; eda < 3; eda++) {

						switch (eda) {
						case 0:	kj_eda = kjv.m_VOL_PEDA01;	
								edanam = "現預金(工事)";	 break;
						case 1:	kj_eda = kjv.m_VOL_PEDA02;
								edanam = "受取手形(工事)";	 break; 
						case 2:	kj_eda = kjv.m_VOL_PEDA03;
								edanam = "相殺(工事)";	 break;
						}

						if (!full)
							(tbl->pBRN_REC + n)->brmsg.Format(".%04d%s", kj_eda, edanam);
						else
							(tbl->pBRN_REC + n)->brmsg.Format("%s%s", strSyog, edanam);

						(tbl->pBRN_REC + n)->kn50.Empty();
						(tbl->pBRN_REC + n)->brn = kj_eda;
						(tbl->pBRN_REC + n)->tkno = 0;
						n++;
					}
				}
				kjv.Close();
			}
		}
		CATCH(CMemoryException, me) {
			me->GetErrorMessage(strError.GetBuffer(_MAX_PATH), _MAX_PATH);
			strError.ReleaseBuffer();
			TRACE("%s\n", strError);
	//		ermset( 0, (char*)(LPCTSTR)strError );
	//		return;	
		}
		CATCH(CDBException, de) {
			strError = de->m_strError;
			TRACE("%s\n", strError);
	//		ermset( 0, (char*)(LPCTSTR)strError );
	//		return;
		}
		END_CATCH
	}
	else if (m_pZm->zvol->apno >= 0x50 && m_pZm->zvol->apno != 0x59 ) {	//宗教以外の非営利
		int sh_eda = 0;
		CString edanam;

		for (int eda = 0; eda < 2; eda++) {
			//学校以外は、現預金対象のみ
			if( eda == 1) {
				if (m_pZm->zvol->apno != 0x58)
					break;
			}
			switch (eda) {
			case 0:	sh_eda = 1;
				edanam = "現預金対象仕訳";	 break;
			case 1:	sh_eda = 4;
				edanam = "相殺仕訳(振替仕訳)";	 break;
			}

			if (!full)
				(tbl->pBRN_REC + n)->brmsg.Format(".%04d%s", sh_eda, edanam);
			else
				(tbl->pBRN_REC + n)->brmsg.Format("%s%s", strSyog, edanam);

			(tbl->pBRN_REC + n)->kn50.Empty();
			(tbl->pBRN_REC + n)->brn = sh_eda;
			(tbl->pBRN_REC + n)->tkno = 0;
			n++;
		}
	}

	tbl->rec_n = n;		// レコード数

	tbl->page_max = 1;	// 最大ページ
	tbl->page_now = 1;	// 現在ページ
	tbl->page_recn = n;	// 1ページ分のレコード数

	return n;
}

//====================================================
//	諸口枝番を処理するマスターか？ [10.16 /08]
//====================================================
BOOL CSyogBrn::syogbrn_master()
{
	if( m_pZm->zvol->apno == 0x10 && (m_pZm->zvol->ind_table&0x01) )
		return FALSE;

	return TRUE;
}


//=========================================	
//	syogbrntbl_inz()
//	内部テーブルイニシャライズ
//
//=========================================	
void CSyogBrn::syogbrn_tbliniz()
{
	/*----------------------------------------
		アクセス用諸口枝番テーブル登録数
		イニシャライズ
	-----------------------------------------*/
	m_ApSyogBrnNUM = 0;

	/*----------------------------------------
		テーブルイニシャライズ
	-----------------------------------------*/
//	memset( &m_pApSyogBrnTBL, '\0', (unsigned long)&m_pApSyogBrnTBL.pBRN_REC - (unsigned long)&m_pApSyogBrnTBL );
	m_pApSyogBrnTBL.code.Empty();
	m_pApSyogBrnTBL.brn_kn50.Empty();
	m_pApSyogBrnTBL.ForeCol = 0;
	m_pApSyogBrnTBL.full = 0;	
	m_pApSyogBrnTBL.page_max = 0;
	m_pApSyogBrnTBL.page_now = 0;
	m_pApSyogBrnTBL.page_recn = 0;
	m_pApSyogBrnTBL.rec_n = 0;
//	m_pApSyogBrnTBL.pBRN_REC = NULL;

	m_pApSyogBrnTBL.ForeCol = SYOGBR_ForeCOL;	// 表示文字色
}


//-------------------------------------------------
//			syogbrn_check()
//			初期化関数
//			->	TRUE ... ok
//				FALSE .. No
//-------------------------------------------------
BOOL CSyogBrn::syogbrn_check( BOOL full )
{

	if( m_pApSyogBrnTBL.full != full )
	{
		/*-----------------------------
			アクセス用テーブル作成
		------------------------------*/
		m_ApSyogBrnNUM = MakeApSyogBrn( &m_pApSyogBrnTBL, full );
	}
	return TRUE;
}


//-------------------------------------------------
//	get_syogbrtb()
//		アクセス用諸口枝番テーブル返送
//
//-------------------------------------------------
BRN_TB *CSyogBrn::get_syogbrtb( BOOL full )
{
	if( syogbrn_check( full ) )
	{
		return &m_pApSyogBrnTBL;
	}
	return 0;
}


//-------------------------------------------------	
//		get_syogbrnmsg()
//	
//			諸口枝番名称リード
//	
//			->	"諸口枝番名称"
//			0 ... Not Found
//-------------------------------------------------
BRN_NAME *CSyogBrn::get_syogbrnmsg( CString& code, int brn, BOOL full )
{
	int i;
	char ret_txt[64]={0};
	static BRN_NAME ret_name;

	if( code == SYOGT && brn != -1 )
		if( syogbrn_check( full ) )
			if( m_ApSyogBrnNUM )
			{
				i = brn;

				for( i = 0; i < m_ApSyogBrnNUM; i++ ) {
					if( m_pApSyogBrnTBL.pBRN_REC[i].brn == brn ) {
						sprintf_s( ret_txt, sizeof ret_txt, "%s", (LPCTSTR)m_pApSyogBrnTBL.pBRN_REC[i].brmsg );

						ret_name.Name = ret_txt;
						ret_name.ForeColor = SYOGBR_ForeCOL;	// 表示文字色
						return &ret_name;
					}
				}
			}
	return NULL;
}

//
//	科目コードを頭につけるか？
//
BOOL&	CSyogBrn::IsKmkcodeDisp()
{
	return m_bKmcod;
}