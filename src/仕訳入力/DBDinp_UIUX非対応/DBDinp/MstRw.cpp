#include "stdafx.h"
#include "DBDinp.h"

//#include "perform_chk.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "resource.h"
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

// 入力選択項目Get/Set
CDinpINI DinpSW;

// 入力設定項目選択
#include "AutoInp.h"
extern struct _AUTOSEL *pAUTOSEL;

//func
#include "mstrw.h"
#include "scan.h"

#include "DataListCtrl.h"
#include "Dinpsub.h"

#include "MainFrm.h"
#include "CStRecordSet.h"

#include <jzcheck.h>
#include <OpeAtdwLic.h>


// 定型仕訳
extern CMKRDSub Mkrd;

#include "BmnChk.h"
extern CBmnHaniCheck BmChk;
extern CKojiDataCheck KjChk;

//消費税モジュール
extern CDBSyohi* pDBsy;

//枝番摘要
extern CBrnTky m_BrTek;


//消費税画面
//extern CSyzInp* m_pSkbnIn;

static unsigned short	SY_errno;
static char	SY_ermsg[128];

// data display
extern struct _DATA_LINE DDATA_LINE[];
//extern int SCROLL_NL();

extern int BOTOM_SEQ;

extern int MST_OPEN_OK;	// マスター準備ＯＫサイン


void DebugDocsubUser()
{
	pDCsb->I_userOpen();
	pDCsb->i_user->MoveFirst();
	CString msg;

	if( pDCsb->i_user->st != -1 ) {
		do {
			msg.Format("DocSub: id(%d) real = %s, user = %s", pDCsb->i_user->u_id, pDCsb->i_user->real_name, pDCsb->i_user->user_name);
			AfxMessageBox(msg);
		} while( pDCsb->i_user->MoveNext() == 0 );
	}
	pDCsb->I_userClose();

	msg.Format("DocSub: DBUserNo (%d) DBRealName = %s, DBUserName = %s", pDCsb->DBUserNo(), pDCsb->DBRealName(), pDCsb->DBUserName());
	AfxMessageBox(msg);
}


//this func only

// MstRw.cpp
int get_adyear( BYTE* date );


static BOOL shutter = FALSE;

// マスタータイプ
static DWORD	dwMST_TYPE = 0;
// 上手参照モード
static int jzSansyo = -1;

BOOL IsJzSansyo()
{
	return jzSansyo == 1 ? TRUE : FALSE;
}

// クラウド共有 +上手君 環境 チェック
static int JozuEnvCloud = -1;


//確定マスターか（上手君参照モードではない)
BOOL IsMAsterKAKUTEI()
{
	BOOL ret = FALSE;

	if( !IsJzSansyo() ) {
		if( bCONFIRM_MASTER )	ret = TRUE;
	}
	else {
		if( bCONFIRM_MASTER & 0x01 )	ret = TRUE;
	}
	return ret;
}



using namespace	Jz2CloudApi;

//
//	code = -1 のときは、ICSExitを呼ばない( すでにWM_CLOSE されている場合など )
//
//
void ermset( short code, char *msg )
{
	static	CErrBlk	ebk(0);

	if( shutter == FALSE )
	{
		shutter = TRUE;

		throw CErrBlk( code, msg );
	}
}

void ErrExit( short code, LPCTSTR msg )
{
	// VolumeOpen でのエラー
	if( code == -999 ) {
		vol_close();
		code = -2;
	}
	if( code == -1 ) {
		myICSMessageBox( msg, MB_ICONSTOP );
		GetDBDinpMainFrame()->PostMessage( WM_CLOSE, 1, 0 );
	}
	else if( code == -2 ) {
		//メッセージを表示せずに終了
		GetDBDinpMainFrame()->PostMessage( WM_CLOSE, 1, 0 );
	//	ErrorMessage( 0, msg );
		;
	}
	else
	{
		ICSExit( code, (char*)msg );
	}
}


//-------------------------------------------------------------------------------------------
//	CoSel* Cook		会社確認オブジェクト
//	CWnd* pWin		会社確認を出すウィンドウポインタ
//	BOOL co_chg		他のプロセスで会社変更された場合=TRUE
//					あるいは、当プロが起動した場合は=TRUE(会社確認を引数なしで呼ぶため)
//-------------------------------------------------------------------------------------------
int cocod_ok( CoSel* Cook, CWnd* pWin, BOOL co_chg )
{
	COPAK okctl;
	int st;

	//コモン情報取得状態チェック
	if( COM_Check() == -1 )
		ermset( ERROR_ENV, "コモン情報が取得できません。");

	if( ! co_chg )	{
		// 当プロで会社変更(業務メニューの会社を引き継がない)
		okctl.sgn0 = 0x10;
#ifdef _DEBUG
		okctl.pksw = 1;
		okctl.subs = 1;
#endif

		st = Cook->co_ok( okctl );
	}
	else {
		// 他のプロセスで会社変更
		st = Cook->co_ok();
	}

	return st;
}




BOOL DocDBCheck(BYTE nApno, long nCono, long nYmd )
{
	BOOL bRet;

	CImgMng* pImgMng;
	pImgMng = new CImgMng;

	IMGDB_COINFO	ici = {0};
	ici.apno	= (BYTE)nApno;
	ici.cocode	= nCono;
	ici.ymd		= nYmd;

	if( pImgMng->IsAbleToUseImgDB( pDBzm, &ici ) )
		bRet	= TRUE;
	else
		bRet	= FALSE;

	delete pImgMng;

//	MyTrace("DocDBCheck ret = %d\n", bRet);

	return bRet;
}



// マスターオープン
//  ボリュームラベルなど
//	return -1	エラー
//	return -2	残高合併で処理できない
//	return -3	確定マスターで仕訳なし
//
int init_open(void)
{

	if( pDBzm == NULL ) {
		if( (pDBzm = new CDipZmSub) == NULL ) {
			ermset(ERROR_MEMORY, "財務クラスオブジェクトを作成できません！");
			return -1;
		}
	}

	char	sqlsvr[256], apno[20], cocode[20], kikan[20];
	if( PCOM_GetString("SELECTCODSN", sqlsvr, apno, cocode, kikan) == 4 ) {
		if( pDBzm->SetMasterDB(SMDB_VERS, sqlsvr, atoi(apno), atoi(cocode), atoi(kikan)) == -1 ) {
			//終了処理が不要なため、財務クラス解放
			delete pDBzm;
			pDBzm = NULL;
			ermset(-1, (char*)(LPCTSTR)pDBzm->GetZmErrMsg());
			return -1;
		}
	}
	else {
		ermset(ERROR_STOP, "DB 情報[SELECTCODSN]を取得できないので 処理を中止します。");
		return -1;
	}

	//#ifdef CLOSE
		// マスタの状態チェック関数
	CZmGen8		ZmGen;
	CString		str;
	ZMGEN_CHKINFO	chkinfo = { 0 };
	int rt = ZmGen.CheckDBMasterState(pDBzm, str, chkinfo);

	if( (rt == -1) || (rt == -2) ) {		// エラー
		// エラー処理
		ICSMessageBox(ZmGen.GetErrorMsg(), MB_ICONSTOP);
		ermset(-2, "");
		return -1;
	}
	else if( rt == 1 ) {	// メッセージを表示する必要がある正常終了
		// 注意喚起のメッセージ表示をして、処理継続
		ICSMessageBox(str);
	}

	bool buyscannersv = false;
	if( ZmGen.IsBuyScannerSv() ) {
		buyscannersv = true;
	}
#ifdef _DEBUG
	buyscannersv = true;
#endif
	if( ZmGen.IsKeiriJzEB() ) {
		((CMainFrame*)GetDBDinpMainFrame())->IsKeiriJzEBEnv() = TRUE;
		buyscannersv = true;
	}
	// WL+電帳オプション
	else if( ZmGen.IsKeiriJzWLEB() ) {
		((CMainFrame*)GetDBDinpMainFrame())->IsKeiriJzEBEnv() = TRUE;
		buyscannersv = true;
	}
	else {
		((CMainFrame*)GetDBDinpMainFrame())->IsKeiriJzEBEnv() = FALSE;
	}
	//速度アップのため、上手クラウド共有フラグを初期化
	JozuEnvCloud = -1;
	//#endif

	CBlkIns::pcom_sqlsvr = sqlsvr;
	CBlkIns::pcom_apno = apno;
	CBlkIns::pcom_cocode = cocode;
	CBlkIns::pcom_kikan = kikan;

	// SYSCTL 読み込み
#ifndef _SCANNER_SV_
	GCOM_GetString("SystemDataBase", sqlsvr);
	if( pDBzm->SysCtlDB(sqlsvr) ) {
		ermset(-1, (char*)(LPCTSTR)pDBzm->GetZmErrMsg());
		return -1;
	}
#else
	char	syssvr[256] = { 0 };
	GCOM_GetString("SystemDataBase", syssvr);
	if( pDBzm->SysCtlDB(syssvr) ) {
		ermset(-1, (char*)(LPCTSTR)pDBzm->GetZmErrMsg());
		return -1;
	}
#endif

	if( pDBzm->VolumeOpen() == ERR )
		ermset(-999, (char*)(LPCTSTR)pDBzm->GetZmErrMsg());

	Voln1 = pDBzm->zvol;
	Minfo = pDBzm->minfo;

	if( Voln1->m_type & 0x01 ) {
		//		ermset( 0, "残高合併マスターは処理できません。" );
		return -2;
	}

	// インボイス対応
	// バージョンアップマスターか判定
	bInvMaster = ZmGen.IsInvoiceVupMaster( pDBzm );
	if( bInvMaster ) {
#ifdef CLOSE
		if( (Voln1->s_sgn2 & 0x0f) >= 2 ) {
			// 簡易課税の場合は免税事業者からの課税仕入れと登録番号を非表示にするため、インボイス期間外としておく
			bInvUse = false;
		}
		else {
#endif
			// インボイス期間マスターか
			bInvUse = ZmGen.IsAbleToUseInvoice(pDBzm);
//		}
		// 登録番号を表示するか
		bInvDisp = ZmGen.IsDispInvno( pDBzm );
	}
	else {
		bInvUse = false;
		bInvDisp = false;
	}

	//ドキュメントオープン
	if( buyscannersv ) {
		int ret = OpenDocDBMaster(sqlsvr, apno, cocode, kikan);
		if( ret == -1 ) {
				return -1;
			}
		else if( ret == 1 ) {
	//		DebugDocsubUser();
			bSCANSAVE_Master = TRUE;
		}
		else {
			bSCANSAVE_Master = FALSE;
		}
	}
	else {
		bSCANSAVE_Master = FALSE;
	}

	// 検索データ オープン
	// ユーザー権限で仕訳データの読み取り制限を行う
	ICSMCTL	mctrl;
	mctl_usertbl	utbl;

	if( mctrl.SetDatabase() == 0 ) {
		mctrl.get_usertbl(utbl);
		pDBzm->user_att = utbl.user_att;	// ユーザ属性

		DOPEN_MODE = pDBzm->data_openmode();
	}
	else {
		ermset(0, "ボリューム管理テーブルにアクセスできません！");
		return -1;
	}

	// マスター全体確定か？
	bCONFIRM_MASTER = FALSE;
#ifndef CONFIRM_CHECK
	if( !(Voln1->tl_cor & 0x10) )
		bCONFIRM_MASTER = TRUE;
#endif

	//会社切り替えたときもチェックする必要がある。
	//上手君参照モード
	if( IsCloudSansyoMode(atoi(apno), atoi(cocode), atoi(kikan), sqlsvr) ) {
		jzSansyo = 1;
	}
	else {
		jzSansyo = 0;
	}

//	AfxMessageBox("上手参照モードテスト中");
//	jzSansyo = 1;

	if( jzSansyo == 1 ) {
		bCONFIRM_MASTER |= 0x100;
	}

	BMON_MST = (Voln1->sub_sw&0x02) ? TRUE : FALSE;

	if( Voln1->apno == 0x20 ) {
		M_KOJI = (Voln1->sub_sw&0x04) ? TRUE : FALSE;

		M_MISEIKOJI_TYPE = FALSE;
		//処理タイプ → 未成工事支出金タイプ
		if( pDBzm->object_check( 2, "KJVOL") != 0 ) {
			CStRecordSet st_rec( pDBzm->m_database );
			st_rec.m_sql = _T( "select VOL_MTYP as st from KJVOL" );

			TRY {
				if( ! st_rec.Open() ) {
					ermset( 0, _T( "工事マスタータイプのチェックが行えません！" ) );
					return -1;
				}
				if( st_rec.m_st > 0 ) {
					M_MISEIKOJI_TYPE = TRUE;
				}
				st_rec.Close();
			}
			CATCH( CMemoryException, me ) {
				me->GetErrorMessage( str.GetBuffer(_MAX_PATH), _MAX_PATH );
				str.ReleaseBuffer();
				ermset( 0, (char*)(LPCTSTR)str );
				return -1;
			}
			CATCH( CDBException, de ) {
				str = de->m_strError;
				ermset( 0, (char*)(LPCTSTR)str );
				return -1;
			}
			END_CATCH
		}
	}
	else {
		M_KOJI = FALSE;
		M_MISEIKOJI_TYPE = FALSE;
	}

	BMON_LNG = Voln1->bmcol;	/* 部門入力桁数	*/
	/* 科目コードレングス */
	KMKCD_LN = 8;

	if( (Voln1->br_sw & 0x01) ) {
		BRmst = 1;
	}
	else
		BRmst = 0;

	IMG_master = ERR;

	if( (pDBzm->zvol->ocr_idev&0x0f) == 3 )
	{
		// イメージ摘要ファイルのオープン 
		if( (IMG_master = pDBzm->ImageOpen()) == ERR )	// ERR = 無し OK = 有り
			ermset( -1, (char*)(LPCTSTR)pDBzm->GetZmErrMsg() );
	}
	else
		IMG_master = ERR;

	//証憑番号	[11.26 /12]
	if( Voln1->g_hsw & 0x04 ) {
		bDOCEVI_Master = TRUE;
	}
	else {
		bDOCEVI_Master = FALSE;
	}

	//マスターサイン
	dwMST_TYPE = 0;

//	int vver = GetMasterVver( pDBzm );
	int vver = ZmGen.CheckValiableVersion( pDBzm->m_database, ID_H26SYZ_VUP_CHK );
//	int vver = 1;

	if( vver >= 1 ) {
		dwMST_TYPE |= MST_INSERT;
		dwMST_TYPE |= MST_TKY20;

		int ver2 = ZmGen.CheckValiableVersion( pDBzm->m_database, ID_H31SYZ_VUP_CHK );

//		EnumIdSyzKikan	syzKikan = ZmGen.GetSyzKikan(pDBzm->zvol->ee_ymd);
		
		if( ver2 >= 1 ) {
			if( pDBzm->zvol->ss_ymd >= SYZ10_YMD ) {
				dwMST_TYPE |= MST_SYZ10;
			}
			else if( pDBzm->zvol->ee_ymd >= SYZ10_YMD ) {
				dwMST_TYPE |= MST_SYZ8_10;
			}

		}
		if( pDBzm->zvol->ss_ymd >= SYZ8_YMD ) {
			dwMST_TYPE |= MST_SYZ8;
		}
		else if( pDBzm->zvol->ee_ymd >= SYZ8_YMD ) {
			dwMST_TYPE |= MST_SYZ5_8;
		}

		int ver3 = ZmGen.CheckValiableVersion(pDBzm->m_database, ID_INV_SYZ_VUP_CHK);
		if( ver3 >= 1 ) {
			if( ZmGen.IsAbleToUseInvoice(pDBzm) ) {
				dwMST_TYPE |= MST_SYZ_INV;
			}
		}
	}
	else if( vver == -1 ) {
		ermset( -1, (char*)(LPCTSTR)ZmGen.GetErrorMsg() );
	}

	// プログラム開始時の 最終 SEQ, 最大数を保存
	// 
	CString filter;
	
	filter = _T( "seq = 1" );		//darec
	pDBzm->dcntl->Requery( filter );

	pDBzm->m_lastdata.Reset(); 

	if( pDBzm->dcntl->st != -1 ) {
		START_SEQ		= pDBzm->dcntl->cnt;
		pDBzm->m_dacnt	= pDBzm->dcntl->cnt;
#ifdef MAXCHECK_CUT
		pDBzm->m_damax	= 999999/*pDBzm->dcntl->mcnt*/;
#else
		pDBzm->m_damax	= 300000;	// 最初に確保するメモリー量
#endif
		// 新規書き込みデータ
		pDBzm->m_lastdata.m_seq = pDBzm->m_dacnt + 1; 
	}

	// 権限がないユーザは、
	if( pDBzm->m_dacnt <= 0 && bCONFIRM_MASTER ) {
		return -3;
	}

	filter = _T( "seq = 3" );		//ezrec
	pDBzm->dcntl->Requery( filter );
	if( pDBzm->dcntl->st != -1 ) {
		pDBzm->m_edmax = pDBzm->dcntl->mcnt;
	}
	filter = _T( "seq = 7" );		//tkrec
	pDBzm->dcntl->Requery( filter );
	if( pDBzm->dcntl->st != -1 ) {
		pDBzm->m_tkmax = pDBzm->dcntl->mcnt;
	}

	// 設定クラスの初期化( owntb オープンクローズはDLLで )
	DinpSW.Init( pDBzm );
	// 入力モードゲット
	pAUTOSEL = get_inpmode();

	// 11.07 /08
	KamokucodeSign( (pAUTOSEL->SCNCD_OPT == OPT_SCNCD) );

	// 個人科目表示制限
	int type = IsKojinGyosyuMaster( pDBzm );

	pDBzm->m_dwIndSgn = pAUTOSEL->INDSGN_OPT;
	if( pDBzm->m_dwIndSgn == 0 )
		pDBzm->m_dwIndSgn	= SL_IND_ALL;
	else {
		if( (pDBzm->m_dwIndSgn & SL_IND_IPPAN) ) {
			if( !(type & SL_IND_IPPAN) )	pDBzm->m_dwIndSgn	= SL_IND_ALL;
		}
		if( (pDBzm->m_dwIndSgn & SL_IND_HUDOSAN) ) {
			if( !(type & SL_IND_HUDOSAN) )	pDBzm->m_dwIndSgn	= SL_IND_ALL;
		}
		if( (pDBzm->m_dwIndSgn & SL_IND_NOGYO) ) {
			if( !(type & SL_IND_NOGYO) )	pDBzm->m_dwIndSgn	= SL_IND_ALL;
		}
	}

	// 公益科目表示制限 ('08.12.01)
	pDBzm->m_Jgykcd = pAUTOSEL->NPJGY_OPT;
	pDBzm->m_Knrkcd = pAUTOSEL->NPKNR_OPT;

	// 10.01 /12
	if( pDBzm->m_Jgykcd == 0 )
		pDBzm->m_Jgykcd = 0xffffffff;
	if( pDBzm->m_Knrkcd == 0 )
		pDBzm->m_Knrkcd = 0xffffffff;

//	pDBzm->m_dwIndSgn = 0;
//	pDBzm->m_dwIndSgn	= SL_IND_ALL;
//	if( type & 0x01 )	pDBzm->m_dwIndSgn |= SL_IND_IPPAN;
//	if( type & 0x02 )	pDBzm->m_dwIndSgn |= SL_IND_HUDOSAN;
//	if( type & 0x04 )	pDBzm->m_dwIndSgn |= SL_IND_NOGYO;

#ifdef LATER_CHECK	
	// 輸入仕入仕訳作成ＳＷセット
	pDBsy->Sy_impdta_mk( DinpSW.isMkSyImpDta( pDBzm ) );
#endif

	return(0);
}


// 2008.4.4 追加( コンバージョンマスターで SEQ が飛んでいる場合、SEQ を振りなおす
static
void RensoTekiyoSeqReset()
{

	pDBzm->dcntl->Requery( "seq = 15" );
	int cnt = pDBzm->dcntl->cnt;

	pDBzm->rtrec->Requery( "" );
	int rpg = -1;
	int rseq = -1;
	for( int i=0; i < pDBzm->dcntl->cnt; i++ ) {
		if( pDBzm->rtrec->SetAbsolutePosition( i+1 ) == -1 ) break;

		if( pDBzm->rtrec->rtseq == -1 ) break;
		if( pDBzm->rtrec->rtpgn != rpg ) {
			rpg = pDBzm->rtrec->rtpgn;
			rseq = 1;
		}
		if( pDBzm->rtrec->rtseq != rseq ) {
			long rcd = pDBzm->rtrec->rtcod;
			pDBzm->rtrec->Delete( rpg, pDBzm->rtrec->rtseq );
			pDBzm->rtrec->Append( rpg, rseq, rcd );
			pDBzm->rtrec->Requery( "" );
		}
		rseq++;
	}
}

static int _kmset( void )
{
	// 科目名称リード
	if( pDBzm->KamokuMeisyoRead() == ERR )
		ermset( -1, (char*)(LPCTSTR)pDBzm->GetZmErrMsg() );

	// 合計名称リード --- 07.07 /09 ---
	if( pDBzm->GokeiMeisyoRead() == ERR )
		ermset( -1, (char*)(LPCTSTR)pDBzm->GetZmErrMsg() );

	// 消費税モジュールのためオープン
	if( pDBzm->KamokuMeisyoOpen() == ERR )
		ermset( -1, (char*)(LPCTSTR)pDBzm->GetZmErrMsg() );

	// 部門名称
	if( BMON_MST ) {
		if( pDBzm->BmnameOpen() == ERR )
			ermset( -1, (char*)(LPCTSTR)pDBzm->GetZmErrMsg() );
		if( pDBzm->BumonzanOpen() == ERR )
			ermset( -1, (char*)(LPCTSTR)pDBzm->GetZmErrMsg() );
		if( pDBzm->BumonEdabanOpen(_T("ezbmn = -1"),0,1) == ERR )
			ermset( -1, (char*)(LPCTSTR)pDBzm->GetZmErrMsg() );

		pDBzm->MakeBmnNameMap();
	}

	// 連想摘要リード
	if( pDBzm->RensoTekiyoOpen() == ERR )
		ermset( -1, (char*)(LPCTSTR)pDBzm->GetZmErrMsg() );
	// 連想摘要修復
	RensoTekiyoSeqReset();

	if( pDBzm->KanaTekiyoOpen() == ERR )
		ermset( -1, (char*)(LPCTSTR)pDBzm->GetZmErrMsg() );

	// 変動事由オープン
	if( pDBzm->KshctlOpen(-1) == ERR )
		ermset( -1, (char*)(LPCTSTR)pDBzm->GetZmErrMsg() );
	if( pDBzm->KshnamOpen(-1) == ERR )
		ermset( -1, (char*)(LPCTSTR)pDBzm->GetZmErrMsg() );
	if( pDBzm->KshrecOpen(-1) == ERR )
		ermset( -1, (char*)(LPCTSTR)pDBzm->GetZmErrMsg() );
	
#ifdef SELECTDATA_CLOSE
	int		i;
	char	_c[8];
/* FIX item table , Page item table */
	_fillbuf( _c, sizeof( _c ), 0 );

	struct _DBKNREC	*pKn;
	for( i = 0; i < pDBzm->knm_info->reccount; i++ ) {
		pKn = kmtbl_get( i );
		if( pKn->knfkn >= 1 && pKn->knfkn <= 8 )
			FIXTBL[ pKn->knfkn-1 ] = pKn->knseq;

		if( pKn->knkpg >= 1 && pKn->knkpg <= 8 ) {
			if( _c[ pKn->knkpg-1 ] <= 32 ) {
//					TRACE( "**pKn->knseq %d\n", pKn->knseq );
				KDPAG[ pKn->knkpg-1 ][ _c[ pKn->knkpg-1 ] ] = pKn->knseq;
				_c[ pKn->knkpg-1 ] += 1;
			}
		}
	}
#endif

	// 現金貸借科目 個人科目表示制限
	int type = IsKojinGyosyuMaster( pDBzm );

	if( type ) {
		DBKNREC* pKn;
		DWORD dwBaseCode = _atoh( pAUTOSEL->DCKM_CODE );
		BOOL	bOK = FALSE;
		BOOL	bSet = FALSE;
		DWORD	dwCode;
		int		i;

		for( i = 0; i < pDBzm->knm_info->reccount; i++ ) {
			pKn = pDBzm->pKnrec+i;
			dwCode = pKn->kncod;
			hl_rev( &dwCode, 4 );

			if( dwCode == dwBaseCode ) {
				if( CheckKamokuRestrict( pDBzm, pKn ) ) {
					bOK = TRUE;
				}
				break;
			}
		}

		if( ! bOK ) {
			for( i = 0; i < pDBzm->knm_info->reccount; i++ ) {
				pKn = pDBzm->pKnrec+i;
				dwCode = pKn->kncod;
				hl_rev( &dwCode, 4 );

				if( dwCode == dwBaseCode ) {
					continue;
				}

				if( dwCode >= 0x01010200 )
					break;

				if( (dwCode & 0xffffff00) == 0x01010100 ) {
					if( ! CheckKamokuRestrict( pDBzm, pKn ) )
						continue;

					sprintf_s( pAUTOSEL->DCKM_CODE, sizeof pAUTOSEL->DCKM_CODE, "%08x", dwCode );

					bSet = TRUE;
					break;
				}
			}

			if( ! bSet ) {
				::ZeroMemory( pAUTOSEL->DCKM_CODE, sizeof pAUTOSEL->DCKM_CODE );
			}
		}
	}
	//定型伝票修復
	RDdnpDataRepair();

	return(0);
}


// 入力モードゲット

struct _AUTOSEL *get_inpmode()
{
	return DinpSW.GetDinpSW();
}


// 入力モードセット
void set_inpmode( struct _AUTOSEL *autosel )
{
	DinpSW.SetDinpSW( autosel);
}



/* -----------------------------
	摘要枝番　サーチ＆セット
------------------------------- */
int tbrasrch( CDBINPDataRec *data, int tkcd, struct _LINE_CND *cnd )
{
int ret = 0;
CDBINPDataRec sv;
int sv_dbr, sv_cbr;

	sv = *data;

	if( (cnd+DEBT_PN)->INP_type != 1 )
	{
		sv_dbr = data->m_dbr;
		data->m_dbr = -1;
	}
	if( (cnd+CRED_PN)->INP_type != 1 )
	{
		sv_cbr = data->m_cbr;
		data->m_cbr = -1;
	}

	BOOL bBmn = FALSE;
	if( pAUTOSEL->BMNTKBR_OPT == OPT_CHKON )
		bBmn = TRUE;

	pDBzm->DB_TekiyoEdabanSet( data, tkcd, bBmn );

	if( (cnd+DEBT_PN)->INP_type != 1 )
//		if( darec._DBR != 0xffff && data->darec._DBR != sv.darec._DBR )
		if( data->m_dbr != -1 && data->m_dbr != sv.m_dbr )
		{
			(cnd+DEBT_PN)->INP_type = 2;	// 自動

			ret |= 0x01;	//借方にセット
		}
		else
			data->m_dbr = sv_dbr;

	if( (cnd+CRED_PN)->INP_type != 1 )
//		if( data->darec._CBR != 0xffff && data->darec._CBR != sv.darec._CBR )
		if( data->m_cbr != -1 && data->m_cbr != sv.m_cbr )
		{
			(cnd+CRED_PN)->INP_type = 2;	// 自動

			ret |= 0x02;	//貸方にセット
		}
		else
			data->m_cbr = sv_cbr;


	return ret;
}

/* ------------------------------------------------------
	摘要枝番　サーチ＆セット

	char *dbr_inptype	借方枝番入力ﾀｲﾌﾟ
	char *cbr_inptype	貸方枝番入力ﾀｲﾌﾟ

	==== 枝番入力ﾀｲﾌﾟ =======================
	０...なし
	１...直接入力
	２...「借貸枝番」＝摘要枝番による枝番
	=========================================
  ------------------------------------------------------ */
int tbrasrch( CDBINPDataRec *data, int tkcd, char *dbr_inptype, char *cbr_inptype )
{
int ret = 0;
CDBINPDataRec sv;
int sv_dbr, sv_cbr;

	sv = *data;

	if( *dbr_inptype != 1 )
	{
		sv_dbr = data->m_dbr;
		data->m_dbr = -1;
	}
	if( *cbr_inptype != 1 )
	{
		sv_cbr = data->m_cbr;
		data->m_cbr = -1;
	}

	BOOL bBmn = FALSE;
	if( pAUTOSEL->BMNTKBR_OPT == OPT_CHKON )
		bBmn = TRUE;

	pDBzm->DB_TekiyoEdabanSet( data, tkcd, bBmn );

	if( *dbr_inptype != 1 )
//		if( darec._DBR != 0xffff && data->darec._DBR != sv.darec._DBR )
		if( data->m_dbr != -1 && data->m_dbr != sv.m_dbr )
		{
			*dbr_inptype = 2;	// 自動
			ret |= 0x01;	//借方にセット
		}
		else
			data->m_dbr = sv_dbr;

	if( *cbr_inptype != 1 )
//		if( data->darec._CBR != 0xffff && data->darec._CBR != sv.darec._CBR )
		if( data->m_cbr != -1 && data->m_cbr != sv.m_cbr )
		{
			*cbr_inptype = 2;	// 自動
			ret |= 0x02;	//貸方にセット
		}
		else
			data->m_cbr = sv_cbr;

	return ret;
}


struct _DBKNREC	*kmtbl_get( unsigned int rno )
{
char	 pbuf[128];

	if( rno >= 0 && rno < (UINT)pDBzm->knm_info->reccount )
		return pDBzm->pKnrec + rno;
	else {
		sprintf_s( pbuf, sizeof pbuf, "科目名称テーブル ( %d ) データ取り出しエラー", rno );
		ermset( 0,  pbuf );
		return NULL;
	}
}


static int _mst_open(void)
{

	// データ オープン２
	CString filter;

//FILETrace( "_mst_open 00 START_SEQ = %d\n", START_SEQ );

	// オープン時の速度向上のため、SEQ を一つだけ指定
	filter.Format( "seq = %d", START_SEQ );

	pDBzm->data_Requery() = TRUE;
	if( pDBzm->DataOpenX( DOPEN_MODE, 0, filter ) != 0 )
		ermset( -1, (char*)(LPCTSTR)pDBzm->GetZmErrMsg() );
	// 変更・追加用データ オープン
	pDBzm->dbdata_Requery() = TRUE; 
	if( pDBzm->DBDataOpenX( -2, 0, filter ) != 0 )
		ermset( -1, (char*)(LPCTSTR)pDBzm->GetZmErrMsg() );

#ifdef KEEP_CLOSE
	// 出納帳用 元帳クラスオープン
	if( pDBzm->LedgerOpen( 0, "seq = 1" ) ) {
		ermset( -1, (char*)(LPCTSTR)pDBzm->GetZmErrMsg() );
	}
#endif

	//消費税
	if( pDBsy == NULL ) {
		if( (pDBsy = new CDBSyohi()) == NULL ) {
			ermset( ERROR_MEMORY, "消費税オブジェクトの作成に失敗しました！" );
		}
	}

	if( pDBsy->SyOpen( pDBzm ) == -1 ) {
		pDBsy->Get_sy_error( &SY_errno, SY_ermsg );
		ermset( SY_errno,  SY_ermsg );
	}

	// 枝番 オープン
	if( BRmst ) {
#ifdef SPEED_CLOSE //速度改善
		if( pDBzm->EdabanOpen( "", 0, 1 ) != 0 )
#endif
		if( pDBzm->EdabanOpen( "ezecd = -1", 0, 1 ) != 0 )
			ermset( -1, (char*)(LPCTSTR)pDBzm->GetZmErrMsg() );
	}
	// 科目残高リード
	if( pDBzm->KamokuZanOpen() == ERR )
	{
		ermset( -1, (char*)(LPCTSTR)pDBzm->GetZmErrMsg() );
	}
	if( pDBzm->KamokuZanRead() == ERR )
	{
		ermset( -1, (char*)(LPCTSTR)pDBzm->GetZmErrMsg() );
	}
	// 仕訳対応摘要
	if( pDBzm->SiwakeTaioTekiyoOpen() == ERR )
	{
		ermset( -1, (char*)(LPCTSTR)pDBzm->GetZmErrMsg() );
	}

#ifdef SPEED_CLOSE //速度改善、F9での呼び出し時に行う
	// 当期貸借金額
	if( pDBzm->TaisyakuRead() == ERR ) {
		ermset( -1, (char*)(LPCTSTR)pDBzm->GetZmErrMsg() );
	}
	// 当期月別仕訳数
	if( pDBzm->DatalinRead() == ERR ) {
		ermset( -1, (char*)(LPCTSTR)pDBzm->GetZmErrMsg() );
	}
#endif

	// 社会福祉改正対応
#ifdef NAIBU_VER2
	if( IsSyafuKaisei( pDBzm->zvol ) ) {
		if( pDBnc == NULL ) {
			if( (pDBnc = new CDBNpCal(pDBzm)) == NULL ) {
				ermset( ERROR_MEMORY, "内部取引消去オブジェクトの作成に失敗しました！" );
			}

			if( pDBnc->NpNaibutorihikiDataOpen("seq=1") == -1 ) {
				ermset( -1, (char*)(LPCTSTR)pDBnc->GetErrMsg() );
 			}
		}
	}
#endif
	// 社会福祉 按分登録チェック
	pDBzm->CheckRegisterNphw();

	if( pDBzm->TklnkOpen() == -1 ) {
		ermset(-1, "摘要リンクテーブルのオープンに失敗しました");

	}

	if( pDBzm->TkjourOpen() == -1 ) {
		ermset(-1, "摘要仕訳テーブルのオープンに失敗しました");
	}

	if( pDBzm->LnkSeqOpen() == ERR ) {
		ermset(-1, "原票関連付けテーブルのオープンに失敗しました");
	}

	//原票のカード対応で。
	if( pDBzm->CardFreqOpen() ) {
		return-1;
	}

	//摘要対応
	if( pTkjour == NULL ) {
		if( (pTkjour = new CZIM_TkjourUtil()) == NULL ) {
			ermset(ERROR_MEMORY, "摘要対応オブジェクトの作成に失敗しました！");
		}
	}

	ICS_TKJOURUTIL_PAC jpac = { 0 };
	jpac.zmsub = pDBzm;
	jpac.CallFlg = 5;
	if( pTkjour->Init(jpac) == -1 ) {
		ermset(-1, "摘要対応オブジェクトの初期化に失敗しました！");
	}

	MST_OPEN_OK = TRUE;	// マスター準備ＯＫサイン

//FILETrace( "_mst_open END\n" );

	return(0);
}


int mst_open(void)
{
int ret;

	CWaitCursor wait;

	ret = _kmset();

	if( ! ret ) {
		ret = _mst_open();
	}
	if (!ret) {
		if (pZeiCl == NULL) {
			pZeiCl = new CDataZeiCalq();
			if (pZeiCl->DZCIniz(pDBzm) != 0)
				ret = -1;
		}
	}
	return ret;
}


// 他のプログラムから戻った場合のマスター読み込み
int mst_reload( void )
{

	if( pDBzm == NULL ) {
//		AfxMessageBox( "再読み込み時、財務クラスがNULLです！" );
		return -1;
	}

	pDBzm->VolumeClose();
	pDBzm->VolumeOpen();
	//ボリュームラベルオープンしたら以下の変数にセット！
	Voln1 = pDBzm->zvol;
	Minfo = pDBzm->minfo;

	if (Voln1->z_fail != 0) {
		ermset(0, "マスター再集計が必要です。\r\n導入・更新のマスター修復処理を行ってください。");
	}
	else if (Minfo == NULL) {
		ermset(0, "月別情報が取得できません。\r\n処理を終了します。");
	}

	pDBzm->KamokuMeisyoFree();
	pDBzm->KamokuMeisyoRead();

	if( pDBzm->pGkrec ){	// '08.12.01
		pDBzm->GokeiMeisyoFree();
		// 09.24 /11
		pDBzm->GokeiMeisyoRead();
	}

	if (pDBzm->bmname != NULL) {
		pDBzm->bmname->Requery("", "");
		pDBzm->MakeBmnNameMap();
	}

	pDBzm->knrec->Requery( "", 0 );
	pDBzm->rtrec->Requery("");
	pDBzm->strec->Requery("");

//	pDBzm->tkrec_kanamode() = FALSE;
//	pDBzm->tkrec->Requery( "", 0 );
	tkrec_queryAll();

	pDBzm->insseq_make() = FALSE;

	CString filter;
	filter = _T( "seq = 1" );
	pDBzm->dcntl->Requery( filter );

	if( pDBzm->dcntl->st != -1 ) {
		DBDinpAppendParamReload( pDBzm->m_dacnt, pDBzm->dcntl->cnt );
		pDBzm->m_dacnt	= pDBzm->dcntl->cnt;
	}
	pDBzm->dbdata_Requery() = TRUE;
	pDBzm->dbdata_Reload()	= TRUE;

	pDBzm->dbdata_Speed()	= FALSE;

	pDBzm->data_Requery()	= TRUE;
	pDBzm->kzrec_Requery()	= TRUE;

	pDBsy->SyClose();
	if( pDBsy->SyOpen( pDBzm ) == -1 ) {
		ermset( 0, "消費税モジュール再オープンに失敗しました。" );
		return -1;
	}

	BmChk.BmnCheckInit();
	KjChk.KojiCheckInit();

	// 社会福祉 按分登録チェック
	pDBzm->CheckRegisterNphw();

	pZeiCl->DZCClose();
	if (pZeiCl->DZCIniz(pDBzm) != 0) {
		ermset(0, "消費税税額モジュール再オープンに失敗しました。");
		return -1;
	}

	pTkjour->ReReadData();

	return 0;
}


#ifdef ONFORK	// 09.21 /00
// 仕訳状態ビットテーブル
#include "bit.h"
extern CBit	MODEFY_BIT;		//修正仕訳ビットテーブル
#endif

#include "SEQ.h"
extern CSEQ	APPEND_SEQ;		//追加仕訳ＳＥＱ登録テーブル
extern CSEQ	SEQ_APPEND;		//追加仕訳ＳＥＱ登録テーブル管理テーブル

extern CSEQ	INSPOS_SEQ;		//挿入仕訳対応ＳＥＱ ポジション格納テーブル
extern CSEQ	SEQ_INSPOS;		//挿入仕訳対応ＳＥＱ 格納テーブル


/*----------------------
	last data display
-----------------------*/
int ldata_dsp( void )
{
	int sv_seq_no;
	int line;
	int i, j;
	char *pmsg;

	sv_seq_no = SEQ_NO;

FILETrace( "ldata_dsp SEQ_NO = %d,USERS_SEQ = %d, st = %d\n", SEQ_NO, USERS_SEQ, pDBzm->dbdata->st );
FILETrace( "dbdata seq = %d, ddate = %d, dbt = %s, cre = %s\n", pDBzm->dbdata->seq, pDBzm->dbdata->ddate, pDBzm->dbdata->dbt, pDBzm->dbdata->cre );

	if( !SEQ_NO )
	{
		if( pDBzm->dbdata->st == 0 ) {
			SEQ_NO = pDBzm->dbdata->seq;
			REAL_SEQ_NO = pDBzm->dbdata->seq;

			/* 一般ユーザでの絞込み */
			if( DOPEN_MODE == 0 ) {
				CString strError;

				// 最終仕訳の番号
				CStRecordSet st_rec( pDBzm->m_database );
				st_rec.m_sql.Format( "select top(1) seq as st from datarec where owner = %d order by seq DESC", pDBzm->DBUserNo() );

				TRY {
					if( ! st_rec.Open() ) {
						ermset( ERROR_STOP, _T( "最終仕訳の番号が調べられません。！" ) );
						return -1;
					}

					while( ! st_rec.IsEOF() ) {
						st_rec.MoveNext();
					}

					if( st_rec.GetRecordCount() > 0 ) {
						USERS_SEQ	= st_rec.m_st;
					}
					else {
						USERS_SEQ	= 0;
					}

					st_rec.Close();

				}
				CATCH( CMemoryException, me ) {
					me->GetErrorMessage( strError.GetBuffer(_MAX_PATH), _MAX_PATH );
					strError.ReleaseBuffer();
					ermset( 0, (char*)(LPCTSTR)strError );
					return -1;	
				}
				CATCH( CDBException, de ) {
					strError = de->m_strError;
					ermset( 0, (char*)(LPCTSTR)strError );
					return -1;
				}
				END_CATCH
			}
			else {
				USERS_SEQ = SEQ_NO;
			}
		}
		else {
			SEQ_NO = REAL_SEQ_NO = 0;
		}
	}
	else
		SEQ_NO = SEQ_NO;

	/* ﾃﾞｰﾀ ﾚｺｰﾄﾞ ｲﾆｼｬﾗｲｽﾞ */
//	TKDflg = 0x10;

	CDBINPDataRec key;
	key.m_seq = USERS_SEQ;

	SREC.Reset();
	CREC = SREC;

	i = SEQ_NO -1;

//FILETrace( "ldata_dsp (1) i = %d\n", i );

/*
	if( key.m_seq > 0 ) {
		for( n = 0; ;n++ ) {
			if( ! n )	st = data_back( &key, &SREC, 1 );
			else		st = data_nxt_back( &key, &SREC, 1 );
			
			if( st > 0 ) {
				i = (SREC.m_seq - 1);
				if( !(SREC.m_dsign[0] & 0x01) || MODEFY_BIT.bitncnd( i ) ) {
					break;
				}
				key.m_seq = SREC.m_seq;

				SREC.m_seq = -1;
			}
			else {
				SREC.m_seq = -1;
				i = -1;
				break;
			}
		}
	}
*/

//FILETrace( "ldata_dsp (2) SREC seq = %d\n", SREC.m_seq );

	line = i + 1;	// 表示開始データ番号

	for( j = 0; j < sizeof ZREC / sizeof ZREC[0]; j++ ) {
		ZREC[j].Reset();
	}

	CDBINPDataRec tmp;
	tmp.m_seq = i+1;
	tmp.m_ddate = 0;

	j = 0;

//FILETrace( "ldata_dsp (3)\n" );

	if( key.m_seq > 0 ) {
	//	j = scandat_getn( &ZREC[0], &tmp, SCROLL_NL() );
		j = scaninsdat_getn( &ZREC[0], -1, SCROLL_NL() );
	}

//FILETrace( "ldata_dsp (4)\n" );

	// 確定マスターで 現ユーザの仕訳がない。
	if( bCONFIRM_MASTER ) {
		if( j <= 0 ) {
			if( ! pDBzm->data_authority() ) {
				ermset( ERROR_STOP, _T( "仕訳が入力されていません！" ) );
				return -1;
			}
			line = 0;	// 01.28 /14
		}
	}
	scandat_dspn( &ZREC[0], DDATA_LINE, SCROLL_NL() );
	D_SEQN = SCROLL_NL()-1;

	if( j > 0 ) {
		SREC = ZREC[D_SEQN];
	}

	if( !sv_seq_no && SEQ_NO && SREC.m_seq > 0 ) {

		if( SREC.m_dsign[0] & 0x80 ) {
			SREC.m_tekiyo.Empty();
		}

		if( pAUTOSEL->DENP_OPT == OPT_AUTO ) {
			DENP_NO = int_seetno( SREC.m_cno );
			if( DENP_NO )		DENP_NO++;
		}
		else if( pAUTOSEL->DENP_OPT == OPT_EQU ) {
			if( SREC.m_seq > 0 )	DENP_NO = (SREC.m_seq+1);
		}
		else {
			// 伝票番号を後で自動加算に変更された場合のため。
			DENP_NO = int_seetno( SREC.m_cno );
			if( DENP_NO )		DENP_NO++;
		}
	}
	else
	{
		int err_flg = 0;
		// 現月または開始月サーチ
		Minfo->MoveFirst();
		long st_ymd, st_ymd2;
		int ofs;
		i = 0;
		do {
			if( Minfo->st == 0 ) {
				if( Minfo->insw & 0x01 )	// 入力許可月ＯＮ
				{
					pmsg = _T("入力開始日が設定できません。ボリュームラベルが壊れている可能性があります。");

					if( Minfo->kes_sgn )	st_ymd = Minfo->emd;
					else					st_ymd = Minfo->smd;
					st_ymd2 = Voln1->ip_ymd;

					if( st_ymd < st_ymd2 ) {
						SREC.m_ddate = st_ymd2;
						CVolDateDB vd;
						int date;
						date = (SREC.m_ddate%10000);
					
						if( vd.db_vd_offset( 0, date, &ofs, pDBzm ) != -1 )
							SREC.m_mofs = ofs;
						else {
							SREC.m_mofs = 0;
							ermset( 0, pmsg );
						}
						SREC.m_ksign = 0;
					//	SREC.m_ksign = (SREC.m_mofs==6||SREC.m_mofs==13) ? 1 : 0;
					}
					else {
						SREC.m_ddate = st_ymd;
						SREC.m_mofs = Minfo->ofset;
						SREC.m_ksign = Minfo->kes_sgn;
						break;
					}
				}
			}
			i++;
		} while( Minfo->MoveNext() == 0 );

		if( err_flg )
		{
			ermset( 0, "仕訳入力月が設定されていません。" );
		}

		// 伝票番号
		if( pAUTOSEL->DENP_OPT == OPT_AUTO ) {
			DENP_NO = int_seetno( SREC.m_cno );
			if( DENP_NO < 0 )	DENP_NO = 0;
			if( DENP_NO )		DENP_NO++;
		}
		else if( pAUTOSEL->DENP_OPT == OPT_EQU ) {
			if( SREC.m_seq > 0 )	DENP_NO = (SREC.m_seq+1);
			else					DENP_NO = 1;
		}
		else {
			// 伝票番号を後で自動加算に変更された場合のため。
			DENP_NO = int_seetno( SREC.m_cno );
			if( DENP_NO )		DENP_NO++;
		}
	}

	BYTE check[4] = {0}, bcd[4] = {0};
	int sofs, eofs, sy, ey, sgn;
	pDBzm->DateConv( check, &SREC, CV2_1 );
	sgn = KESY_SG_GET( check[0] );

	switch( sgn ) {
	case 1 :	bcd[0] = 0x61;	break;
	case 2 :	bcd[0] = 0x62;	break;
	case 3 :	bcd[0] = 0x63;	break;
	case 4 :	bcd[0] = 0x00;	break;
	default:
		bcd[0] = check[0];	break;
	}
	bcd[1] = check[1];

	if( ! pDBzm->check_datelimit( bcd ) ) {
		pDBzm->get_datelimit( sofs, eofs, sy, ey );

		::ZeroMemory( check, sizeof check );
		pDBzm->get_ofsdate( eofs, check ); //決算修正は末日

		if( check[0] == 0 && check[1] == 0 ) {
			ermset( 0, "仕訳入力日が設定できません！" );
			return -1;
		}

		SREC.m_mofs = eofs;
		sgn = KESY_SG_GET( check[0] );
		SREC.m_ksign = sgn ? 1 : 0;
		pDBzm->BcdDateToYmd( check, &SREC.m_ddate );
	}
	// 開始入力日を作成
	pDBzm->MakeInpDate( &SREC );

	CREC.m_ddate = SREC.m_ddate;
	CREC.m_ksign = SREC.m_ksign;
	CREC.m_mofs  = SREC.m_mofs;

	SEQ_NO = sv_seq_no; 

	CREC.m_seq = REAL_SEQ_NO+1;

	//SEQ取得用など、基本レコードセット
	pDBzm->m_lastdata = CREC;

//FILETrace( "ldata_dsp END line = %d\n", line );
/**/FILETrace( "ldata_dsp sv_seq_no = %d, REAL_SEQ_NO = %d\n", sv_seq_no, REAL_SEQ_NO );

	return line;
}


//
// 伝票番号変換
//
int int_seetno( int CNO )
{
	int		shno;

	shno = 0;

	if( CNO != -1 ) {
		shno = CNO;
		if( shno > 9999999 ) shno = 0;
	}

	return( shno );
}



void jb_end( int mode )
{

	l_defn( 0x16 );
	
	if( mode != -1 )
		mst_close();
}


void Docsub_Volume_Close()
{
	if( !pDCsb ) return;

	if( pDCsb->dvol ) pDCsb->VolumeClose();

	if( pDCsb->dbdoc )  pDCsb->DBDocClose();
	if( pDCsb->xdbdoc )  pDCsb->XDBDocClose();

	if( pDCsb->dbdocext1 ) pDCsb->DBDocExt1Close();
	if( pDCsb->xdbdocext1 ) pDCsb->XDBDocExt1Close();

	if( pDCsb->dbdocext2 ) pDCsb->DBDocExt2Close();
	if( pDCsb->xdbdocext2 ) pDCsb->XDBDocExt2Close();

	if( pDCsb->tst ) pDCsb->TstClose();

	if( pDCsb->hash ) pDCsb->HashClose();

	if( pDCsb->sheet_type ) pDCsb->Sheet_typeClose();

	if( pDCsb->taglnk ) pDCsb->TaglnkClose();
	if( pDCsb->xtaglnk ) pDCsb->XTaglnkClose();

	if( pDCsb->doctag ) pDCsb->DoctagClose();

	if( pDCsb->grpent )  pDCsb->GrpentClose();

	if( pDCsb->rcgmsai ) pDCsb->RcgMsaidataClose();

	if( pDCsb->rcg_general ) pDCsb->Rcg_GeneralClose();

	if( pDCsb->shoprec ) pDCsb->ShopRecClose();

	if( pDCsb->owntb ) pDCsb->OwnTblClose();
}



static void _mst_close(int sgn)
{

	if( MST_OPEN_OK == FALSE ) {
		goto DELETE_JOB;
		return;
	}
	else
		MST_OPEN_OK = FALSE;	// マスター準備ＯＫサイン

	// データ クローズ
	pDBzm->DataClose();
	pDBzm->DBDataClose();
#ifdef KEEP_CLOSE
	pDBzm->LedgerClose();
#endif
	// 残高ライト
	pDBzm->KamokuZanClose();

	// 消費税クローズ
	pDBsy->SyClose(0);

	// イメージファイルクローズ
	if( IMG_master != ERR )
	{
		if( pDBzm->ImageClose() == ERR )
			return;
	}
	// FOR_DB
	pDBzm->KamokuMeisyoClose();	// 	消費税モジュールのためオープン
	if( BRmst )	pDBzm->EdabanClose();
	pDBzm->RensoTekiyoClose();
	pDBzm->KanaTekiyoClose();
	pDBzm->DBTekiyoClose();

	pDBzm->VolumeClose();
	pDBzm->SiwakeTaioTekiyoClose();

	pDBzm->BmnameClose();
	pDBzm->BumonzanClose();
	pDBzm->BumonEdabanClose();

	pDBzm->KshctlClose();
	pDBzm->KshnamClose();
	pDBzm->KshrecClose();

	pDBzm->TaisyakuFree();
	pDBzm->DatalinFree();

	pDBzm->TklnkClose();
	pDBzm->TkjourClose();
	pDBzm->LnkSeqClose();
	pDBzm->CardFreqClose();

	Mkrd.MkrdClose();

	if( Mkrd.dnpLabel != NULL ) {
		Mkrd.RDNPClose();
	}

	set_inpmode( pAUTOSEL );

	m_BrTek.brntky_close();

	pZeiCl->DZCClose();

#ifdef NAIBU_VER2
	//内部取引消去
	if( pDBnc != NULL ) {
		pDBnc->NpNaibutorihikiDataClose();
	}
#endif
	MyTrace("_mst_close %d\n", __LINE__);

	Docsub_Volume_Close();

DELETE_JOB:
	//メモリー開放
	if( pTkjour != NULL ) {
		pTkjour->End();
		delete pTkjour;	pTkjour = NULL;
	}
	if( pDBzm != NULL ) {
		delete pDBzm;	pDBzm = NULL;
	}
	if( pDBsy != NULL ) {
		delete pDBsy;	pDBsy = NULL;
	}
#ifdef NAIBU_VER2
	if( pDBnc != NULL ) {
		delete pDBnc;	pDBnc = NULL;
	}
#endif
	if( pDCsb != NULL ) {
		delete pDCsb;	pDCsb = NULL;
	}
	if (pZeiCl != NULL) {
		delete pZeiCl;	pZeiCl = NULL;
	}

	MyTrace("_mst_close sgn = %d\n", sgn);
	if( ! sgn ) {
		if( pAddInv != NULL ) {
			delete pAddInv;	pAddInv = NULL;
		}
	}
	MyTrace("_mst_close END\n");
}




void mst_close(int sgn)
{
//	AfxGetApp()->BeginWaitCursor();		//Wait Cursor ON
	CWaitCursor wait;

	_mst_close(sgn);

//	AfxGetApp()->EndWaitCursor();		//Wait Cursor OFF
}


//-----------------------------------------------------
//	ボリュームラベルクローズ
//	残高マスターなど、処理できないマスターを選んだ場合
//-----------------------------------------------------
void vol_close(void)
{
	pDBzm->VolumeClose();
	//メモリー開放
	delete pDBzm;	pDBzm = NULL;
}


int date_chk( int term )
{

	int ofs;
//	ofs = pDBzm->Myvd_ofset( SREC.m_ddate );
	ofs = SREC.m_mofs;
	if( ofs == -1 )	return -1;

//	memmove( (char *)&CREC.darec._MM, (char *)&SREC.darec._MM, 2 );
	CREC.m_ddate = SREC.m_ddate;
	CREC.m_ksign = SREC.m_ksign;
	CREC.m_mofs  = SREC.m_mofs;

	/* 新車サインチェック	*/
//	Sy_chk_sign( &CREC, INP_mode ? 1: 0, 1 );

	return( 0 );
}


// 消費税イニシャライズ
int DB_SyIniz( CDBINPDataRec* data )
{
	pDBzm->SetCDBData( data );

	// 07.28 /10
	CByteArray	ba;
	ba.Copy( data->m_dsign );

//MyTrace( "*** pDBsy->SyIniz(1) seq = %d [%s]-[%s], zei=<%s>\n", pDBzm->dbdata->seq, 
//						pDBzm->dbdata->dbt, pDBzm->dbdata->cre, pDBzm->dbdata->zei );
	DSIGN_TRACE( pDBzm->dbdata->dsign);

	int st = pDBsy->SyIniz( pDBzm->dbdata );

	// 07.28 /10
	// 税額だけ保存しておく(仕訳書き込み時の差異チェックに引っかかるように)
	if( (ba[0] & 0x40) ) {
		//消費税対象 → 対象外
		if( !(pDBzm->dbdata->dsign[0] & 0x40) ) {
			l_input( data->m_zei, (void*)(LPCTSTR)pDBzm->dbdata->zei );
		}
	}
	else {
		//消費税対象外 → 対象仕訳に変更
		if( (pDBzm->dbdata->dsign[0] & 0x40) ) {
			l_input( data->m_zei, (void*)(LPCTSTR)pDBzm->dbdata->zei );
		}
	}
//MyTrace( "*** pDBsy->SyIniz(3) seq = %d [%s]-[%s], zei=<%s>, st = %d\n", pDBzm->dbdata->seq, 
//						pDBzm->dbdata->dbt, pDBzm->dbdata->cre, pDBzm->dbdata->zei, st );
//MyTrace( "*** pDBsy->SyIniz(4) dsign = %02x%02x%02x%02x%02x\n",pDBzm->dbdata->dsign[0],
//	pDBzm->dbdata->dsign[1],pDBzm->dbdata->dsign[2],pDBzm->dbdata->dsign[3],pDBzm->dbdata->dsign[4] );

	return st;
}

//
// 消費税表示テキスト関係
//		02.26 /98

// Sy_chk_sign() text get


// 文字列スペース調整
void systr_adj( CString& systr )
{
	int idx = systr.Find( '/' );

	if( idx != -1 ) {
		CString a, b, str;
		a = systr.Left( idx );

		int n = kjlen( (void*)(LPCTSTR)a, idx );
		str = a.Left(n);
		if( n < idx )	str += " ";
//MyTrace( "@systr_adj a = %s, n = %d, str = %s", a, n, str );

		int len = systr.GetLength();
		b = systr.Right( len-idx );

		systr = str + b;
	}
}


char *sy_chk_txt( CDBINPDataRec *data,  struct _SY_MSG_PACK2* smp, int inp_mode )
{
//ASSERT( data->m_seq == pDBzm->dbdata->seq );

	static CString txt;

	struct _SY_MSG_PACK2 smpk, *pSmp;
	pSmp = (smp == NULL) ? &smpk : smp;

//MyTrace( "@sy_chk_txt data->m_seq[%d]\n", data->m_seq );
//DSIGN_TRACE(data->m_dsign);
//MyTrace( "@sy_chk_txt data->m_invno[%s]\n", data->m_invno );

	pDBzm->SetCDBData( data );

//MyTrace("@sy_chk_txt data dbt,dbr = %s.%4d, cre,cbr = %s.%4d\n", pDBzm->dbdata->dbt, pDBzm->dbdata->dbr, pDBzm->dbdata->cre, pDBzm->dbdata->cbr);
//DSIGN_TRACE(pDBzm->dbdata->dsign);
//MyTrace("@sy_chk_txt dbdata->val = %s, dbdata->zei = %s\n", pDBzm->dbdata->val, pDBzm->dbdata->zei);

	if( pDBsy->SyMsg( pSmp ) == 0) {
	//	txt = pSmp->SY_LIN_MSG;

//MyTrace( "@sy_chk_txt SyMsg -> zkbn %s\n", pSmp->SY_SELSTR_ZEI );
//MyTrace( "@sy_chk_txt SyMsg -> zeigaku %s\n", pSmp->SY_MSG_SYZEI );
//MyTrace( "@sy_chk_txt SyMsg zei = %s\n", pDBzm->dbdata->zei );
DSIGN_TRACE(data->m_dsign);

		if( inp_mode == _APPEND )
			// インボイス対応
			// 消費税属性もしくは、免税事業者からの課税仕入れサイン、登録番号セットされているか
			//if( TKattr ) {
			if( TKattr || pDBzm->CheckTkrec( &gTKattrec )) {
			//	pDBzm->SetCDBData( data );
				// インボイス対応
				pDBsy->Sy_tkyo_inp( TKattr );		// 摘要消費税属性
				sprintf_s( CREC.m_invno, sizeof( CREC.m_invno ), _T( "%s" ), pDBzm->dbdata->invno );
			}

//MyTrace( "@sy_chk_txt(2) TKattr = %d\n", TKattr );
//MyTrace( "@sy_chk_txt(2) SyMsg dsign %02x%02x%02x%02x%02x\n", pDBzm->dbdata->dsign[0],
//	pDBzm->dbdata->dsign[1],pDBzm->dbdata->dsign[2],pDBzm->dbdata->dsign[3],pDBzm->dbdata->dsign[4] );
//MyTrace( "@sy_chk_txt(2) SyMsg zei = %s\n", pDBzm->dbdata->zei );

		data->m_dsign.Copy( pDBzm->dbdata->dsign );
		CArith	ar(0x16);
		ar.l_input( data->m_zei, (void*)(LPCTSTR)pDBzm->dbdata->zei );

//MyTrace( "@sy_chk_txt(2) dbdata->zei = %s\n", pDBzm->dbdata->zei );

		//この関数を呼ぶと、借方が消費税あり、貸方 消費税なしのとき、税額(dbdata->zei) が0になる。
		txt = pDBsy->Sy_get_message( pDBzm->dbdata, SYGET_TYPE );
	}
	else	txt.Empty();

	systr_adj( txt );

//MyTrace("@sy_chk_txt(END) txt = %s\n", txt);

	return (char*)(LPCTSTR)txt;

}

// Sy_line_dsp() text get
char *sy_line_txt( CDBINPDataRec *data )
{
//ASSERT( data->m_seq == pDBzm->dbdata->seq );
//MyTrace("@sy_line_txt top (%d), dbdata->seq = %d\n", data->m_seq, pDBzm->dbdata->seq);

	static CString txt;
	if( data->m_seq != pDBzm->dbdata->seq ) {
		pDBzm->SetCDBData( data );
	}
	DSIGN_TRACE(pDBzm->dbdata->dsign);

	txt = pDBsy->Sy_get_message( pDBzm->dbdata, SYGET_TYPE );

//MyTrace( "@sy_line_txt (%d), txt %s\n", data->m_seq, txt );

#ifdef KEEP
MyTrace( "sy_line_txt: syz '%s', seq = %d, %02x%02x%02x%02x\n", txt, pDBzm->dbdata->seq, pDBzm->dbdata->dsign[0],
														pDBzm->dbdata->dsign[1], pDBzm->dbdata->dsign[2], pDBzm->dbdata->dsign[3]);

	struct _SY_MSG_PACK2 smpk;
	if( pDBsy->SyMsg( &smpk ) == 0 ) {
		txt = smpk.SY_LIN_MSG;
	}
	else	txt.Empty();
#endif

	systr_adj( txt );

	return (char*)(LPCTSTR)txt;

}



// 枝番残高登録
//		 0 = 登録OK
//		-1 = cancel or error 枝番を取り消す
//
int brzan_torok( CWnd* pwnd, int bmn, CString code, int brn, BOOL dbsgn )
{
	int st = -1;
	CString str;

	EDA_RECORD ed;

	// 部門残高登録の自動登録用にセット
	ed.ed_bmn = bmn;

	strcpy_s( ed.ed_kcod, sizeof ed.ed_kcod, (LPCTSTR)code );
	ed.ed_ecod = brn;
	ed.ed_tkcod = 0;
	::ZeroMemory( ed.ed_zan, sizeof ed.ed_zan );

//PROGRAM_OK = FALSE;

	BOOL bQuery;

//END_PERFORMANCE( "brzan_torok01" )
	((CMainFrame*)GetDBDinpMainFrame())->PopupCloseState(2);

	st = EdabanTorokDialog( &ed, pDBzm, pwnd, &bQuery );
	if( bQuery ) {
	//	pDBzm->tkrec->Requery( "", 0 );
	//	pDBzm->tkrec_kanamode() = FALSE;
		tkrec_queryAll();
	}

	((CMainFrame*)GetDBDinpMainFrame())->PopupCloseState(0);

	if( st == IDCANCEL )
		st = -1;
	else if( st == -1 ) {
		char	eda_buf[20];
		char* msgp;
		msgp = dbsgn ? "貸方" : "借方";
		pDBzm->EdabanToStr( eda_buf, sizeof eda_buf, ed.ed_ecod );
		DBKNREC* pKn;
		pKn = pDBzm->DB_PjisToKnrec( code );

		str.Format( "%s「%.14s.%s」（コード=%s.%s）の登録に失敗しました。",
								msgp, pKn->knnam, eda_buf, ed.ed_kcod, eda_buf ); 
		myICSMessageBox( str );
		st = -1;
	}

	if( st != -1 ) {
		pDBzm->IsEdaAdd() = TRUE;
	}
//PROGRAM_OK = TRUE;

	return st;
}


static int tky_sysflg = 0;
static int val_sysflg = 0;
static int treg_sysflg = 0;
static int zmv_sysflg = 0;
static int tktab_sysflg = 0;
static int Jzcloud_kakuflg = 0;

static int DenpType_sysflg = 0;
static int DateTab_sysflg = 0;
static int RDsiwakeZritu_flg = 0;

static int FromReadOCR_flg = 0;

static int KanaSearch_sysflg = 0;
//消費税欄毎回停止
static int SyzStop_sysflg = 0;


//--------------------------------------------
//	摘要番号 ターミネーションフラグを取得
//
//--------------------------------------------
int& TekiyoKeyMode()
{
	return tky_sysflg;
}

//--------------------------------------------
//	金額 Enter 複写 フラグを取得 [09.08 /08]
//
//--------------------------------------------
int& ValEnterCopyMode()
{
	return val_sysflg;
}


//--------------------------------------------
//	摘要登録画面 フラグを取得 [10.01 /08]
//	0 = 簡易、1 = 詳細
//--------------------------------------------
int& TkyRegMode()
{
	return treg_sysflg;
}

//--------------------------------------------
//	残高問合せ移行 フラグを取得 [06.30 /11]
//	0 = 月指定バー
//	1 = 現在仕訳行の日付の月
//--------------------------------------------
int& ZmoveMode()
{
	return zmv_sysflg;
}


// 消費税モジュールのSyFnc を呼び出す
//
//
int DB_SyFnc( DWORD code, int sgn )
{


	pDBsy->SyFnc( code );

	return 0;
}

// 消費税欄の幅をどうするか？
int CheckWidthSyohi()
{
	int ret = 0;

	CSize *sz;
	sz = GetScreenSize();

	if( pDBzm->IsKaniOrKobetsu() && sz->cx < 1280 ) 
		ret++;

	return ret;
}

// 摘要欄の１文字コピー
BOOL tekyo_tabcopy( CWnd* pwnd, int idc, char *tks )
{
int max = get_inpl( (unsigned char*)tks, Voln1->tk_ln );	// コピー元適要欄の文字ﾊﾞｲﾄ数
int maxbyte = (Voln1->tk_ln * 2);
int now, pos, btpos, cnt, i, st;
char txt[50] = {0};

	// 現在の文字ﾊﾞｲﾄ数
	VARIANT var;
	((CICSDBEDT*)pwnd->GetDlgItem( idc ))->GetData( &var, ICSDBEDT_TYPE_STRING, 0);
	now = strlen( (LPCTSTR)var.pbVal );

	// ﾊﾞｲﾄ単位のｶｰｿﾙ位置
	btpos = ((CICSDBEDT*)pwnd->GetDlgItem( idc ))->GetCaretPosition();
	pos = btpos;

	//  02.09 /02 -ANKINP- 
	if( now == btpos && btpos < maxbyte )
	{
		for( i = 0, cnt = 0; i < max;  ) {
			st = IsKindOfLetter(tks, i);
			if( cnt == pos ) {
				if( st == 1 ) {			// ASCII
					txt[0] = *(tks + i);	
				}
				else if( st == 2 ) {	// 漢字
					if( (btpos+2) <= maxbyte )	memcpy(txt, (tks+i), 2 );
					else	return FALSE;
				}
				else {
					return FALSE;	// それ以外
				}				
				break;
			}
			else {
				if( st == 1 ) {
					cnt++;	i++;
				}
				else if( st == 2 ) {
					cnt += 2;	i += 2;
				}
			}
		}

		if( txt[0] == '\0' )	return FALSE;

		var.pbVal = (BYTE*)txt;
		((CICSDBEDT*)pwnd->GetDlgItem( idc ))->InsertData( &var, ICSDBEDT_TYPE_STRING, 0, btpos );
		
	}
	else if( btpos >= now )
	{
		return FALSE;
	}
	else {
		st = IsKindOfLetter(tks, btpos);
		int mv = 0;
		if( st == 1 )		mv = 1;
		else if( st == 2 )	mv = 2;
		// ポジションを１文字分進める
		((CICSDBEDT*)pwnd->GetDlgItem( idc ))->SetCaretPosition( btpos+mv );
	}

	return TRUE;

}


// 業種番号のチェック
BOOL IsOKGyosyu( int apno, DWORD check_bit )
{
	int check = 0;

	BOOL bRet = FALSE;

	if( check_bit & SW_HOJIN ) {
		if( apno == 0x00 )	check++;
	}
	if( check_bit & SW_KOJIN ) {
		if( apno == 0x10 )	check++;
	}
	if( check_bit & SW_UNSO ) {
		if( apno == 0x01 )	check++;
	}
	if( check_bit & SW_KENST ) {
		if( apno == 0x20 )	check++;
	}
	if( check_bit & SW_BYOIN ) {
		if( apno == 0x30 )	check++;
	}
	if( check_bit & SW_PUB ) {
		if( apno == 0x50 )	check++;
	}
	if( check_bit & SW_PUB3 ) {
		if( apno == 0x51 )	check++;
	}
	if( check_bit & SW_SOCIAL ) {
		if( apno == 0x52 )	check++;
	}
	if( check_bit & SW_SOCIAL2 ) {
		if( apno == 0x53 )	check++;
	}
	if( check_bit & SW_SYUKYO ) {
		if( apno == 0x59 )	check++;
	}
	//学校対応
	if( check_bit & SW_SCHOOL ) {
		if( apno == 0x58 )	check++;
	}

	if( check )	bRet = TRUE;

	return bRet;
}


//摘要カナ検索をした後の、摘要全部をクエリーする。
void tkrec_queryAll()
{
	pDBzm->tkrec->Requery( "", 0 );
	pDBzm->tkrec_kanamode() = FALSE;
	m_BrTek.set_tkquery( FALSE );
}


// 変動事由・振替科目・内部取引消去を行える業種か？
int IsHjiyuItemGyosyu()
{
	int ret = 0;

	if( pDBzm == NULL )	return 0;

	if( ! IsSyafuKaisei( pDBzm->zvol ) ) {
		if( IsGakkouH27Master( pDBzm ) ) {	//学校H27 改正
			ret = 4;
		}
		else {
			if( IsOKGyosyu( Voln1->apno, SW_SOCIAL|SW_SYUKYO|SW_SCHOOL)/* == 0x52*/ ) {
				return 0;
			}
		}

		if( IsOKGyosyu( Voln1->apno, SW_PUB|SW_PUB3)) {
			ret = 2;	//振替科目
		}
		else {
			ret = 1;	//変動事由
		}
	}
	else {
		ret = 3;		//内部取引消去
	}

	return ret;
}


int insseqpos_make( CWnd* pwnd )
{
//	SeqPosMakeThreadProc( NULL );

	CStRecordSet	seqpos;
	CString emsg;
	int ret = 0;

	// init_open( void ) 呼出し後

	INSPOS_SEQ.del_seqtbl();
	SEQ_INSPOS.del_seqtbl();

	if( INSPOS_SEQ.mk_seqtbl( pDBzm->m_damax ) == -1 )		//仕訳挿入対応ＳＥＱテーブル管理テーブル
		ermset( ERROR_MEMORY, "仕訳挿入対応ＳＥＱテーブル管理テーブルが作成できませんでした。");

	if( SEQ_INSPOS.mk_seqtbl( pDBzm->m_damax ) == -1 )		//仕訳挿入対応ＳＥＱテーブル管理テーブル
		ermset( ERROR_MEMORY, "仕訳挿入対応ＳＥＱテーブル管理テーブルが作成できませんでした。");

	seqpos.m_pDatabase = pDBzm->m_database;

	/* 一般ユーザでの絞込み */
	if( IsMasterType(MST_INSERT) ) { 
		if( DOPEN_MODE == 0 ) {
			seqpos.m_sql.Format( "select seq as st from datarec where owner = %d order by dbo.seqret( seq, stseq ), dbo.subseqret( sbseq )", pDBzm->DBUserNo() );
		}
		else {
			seqpos.m_sql.Format( "select seq as st from datarec order by dbo.seqret( seq, stseq ), dbo.subseqret( sbseq )" );
		}
	}
	else {
		if( DOPEN_MODE == 0 ) {
			seqpos.m_sql.Format( "select seq as st from datarec where owner = %d order by seq", pDBzm->DBUserNo() );
		}
		else {
			seqpos.m_sql.Format( "select seq as st from datarec order by seq" );
		}
	}

//MyTrace("insseqpos_make (1)\n");

	TRY {
		if( ! seqpos.Open() ) {
			emsg = _T( "SEQ順を調べられません！" );
			return -99;
		}
		int abspos = 0;

		if( ! seqpos.IsBOF() ) {
			int idx;
			abspos = 1;

			while( !seqpos.IsEOF() ) {
				idx = seqpos.m_st - 1;
				INSPOS_SEQ.nsetseq( abspos, idx );
				SEQ_INSPOS.nsetseq( seqpos.m_st, abspos-1 );

				seqpos.MoveNext();
				abspos++;
			}
			// seqpos.IsEOF()の判定前のカウントマイナス
			abspos--;
		}

		pDBzm->insseq_make() = TRUE;
		pDBzm->m_insCnt = abspos;

		seqpos.Close();

//MyTrace("insseqpos_make (2) pDBzm->m_insCnt = %d, m_dacnt = %d\n", pDBzm->m_insCnt, pDBzm->m_dacnt );
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

//MyTrace("insseqpos_make end\n");

	return 0;
}


//---------------------------------------------------------------
//	摘要欄でのコード＋タブキー動作フラグを取得 [02.07 /13]
//
//	0 = 数字として入力
//	1 = 摘要枝番のみセットする。(文字としての数字は入力しない)
//---------------------------------------------------------------
int& TKtabMode()
{
	return tktab_sysflg;
}

//---------------------------------------------------------------
//	引数で指定したタイプの該当のマスターか？
//	 DWORD typbit: タイプビット
//
//---------------------------------------------------------------
BOOL IsMasterType( DWORD typbit )
{
	return (dwMST_TYPE & typbit);
}



//---------------------------------------------------------------
//	定型仕訳ラベル最大数 返送
//
//---------------------------------------------------------------
int GetRDLabelMax()
{
	int rval = 50;	//消費税バージョンアップ前

	if( IsMasterType(MST_INSERT) ) {
		rval = LBLREC_MAX;
	}

	return rval;
}


//---------------------------------------------------------------
//	定型仕訳データ最大数 返送
//
//---------------------------------------------------------------
int GetRDDataMax()
{
	int rval = 50;	//消費税バージョンアップ前

	if( IsMasterType(MST_INSERT) ) {
		rval = DTAREC_MAX;
	}

	return rval;
}

//---------------------------------------------------------------
//	定型仕訳登録時 最大数 メッセージ
//
//---------------------------------------------------------------
void RegDataTorokMsg()
{
	CString sMsg;

	sMsg.Format( "%d仕訳以上選択されています。\r\n最後の%d仕訳を定型仕訳に登録します。", RDADD_MAX, RDADD_MAX );
	myICSMessageBox( sMsg );
}

//---------------------------------------------------------------
//	定型伝票の確定サイン等を落とす
//
//---------------------------------------------------------------
int RDdnpDataRepair()
{
	CStRecordSet st_rec( pDBzm->m_database );

	// not found
	if( pDBzm->object_check( 2, _T("rdnpdata") ) == 0 ) {
//MyTrace( "RDdnpDataRepair object_check = 0\n" );
		return 0;
	}

	st_rec.m_sql = _T( "select count(*) from rdnpdata \
where cast(substring(rdp_ddsgn, 8,1) as int) & 0x70 >= 0x10 or cast(substring(rdp_cdsgn, 8,1) as int) & 0x70 >= 0x10" );

	CString strError, sql;
	int ret = 0;

	TRY {
		if( ! st_rec.Open() ) {
			ICSMessageBox( _T( "定型伝票のチェックが行えません！" ) );
			return -1;
		}

		if( ! st_rec.IsBOF() ) {
			ret = st_rec.m_st;
		}

		st_rec.Close();
//MyTrace( "RDdnpDataRepair ret = %d\n", ret );

		if( ret > 0 ) {
			//BITOFF
			sql = _T( "update dbo.rdnpdata set rdp_ddsgn = dbo.set_byte2( rdnpdata.rdp_ddsgn, 7, 0xf0, 0 ) \
where cast(substring(rdp_ddsgn, 8,1) as int) & 0x70 >= 0x10" );
			pDBzm->m_database->ExecuteSQL( sql );

			sql = _T( "update dbo.rdnpdata set rdp_cdsgn = dbo.set_byte2( rdnpdata.rdp_cdsgn, 7, 0xf0, 0 ) \
where cast(substring(rdp_cdsgn, 8,1) as int) & 0x70 >= 0x10" );
			pDBzm->m_database->ExecuteSQL( sql );
		}
	}
	CATCH( CMemoryException, me ) {
		me->GetErrorMessage( strError.GetBuffer(_MAX_PATH), _MAX_PATH );
		strError.ReleaseBuffer();
		ICSMessageBox( strError );
		return -1;
	}
	CATCH( CDBException, de ) {
		strError = de->m_strError;
		ICSMessageBox( strError );
		return -1;
	}
	END_CATCH

	return ret;

}

//---------------------------------------------------------------
//	スキャナ保存購入環境か？
//
//---------------------------------------------------------------
bool IsBuyScannerSave()
{
	static	int	IsBuyScanner = -1;

#ifdef CLOSE
	//経理上手くん(電子帳簿版)
	if( ((CMainFrame*)GetDBDinpMainFrame())->IsKeiriJzEBEnv() ) {
		return true;
	}
#endif
	if( IsBuyScanner == 1 ){
		return true;
	}
	else if( IsBuyScanner == 0 ){
		return false;
	}
	else{
		CZmGen8	ZmGen;
	//	bool rt = ZmGen.IsBuyScannerSv(); //OCR-Sは含まない
		bool rt = ZmGen.IsBuyOnlyScannerSv();
		if( rt == true ){
			IsBuyScanner = 1;
		}
		else{
			IsBuyScanner = 0;
		}
#ifdef _DEBUG
		IsBuyScanner = 1;
#endif

		return rt;
	}
}

//== 製品コード ==
static	char	*ICS_IMGMNG_OCR_PACKAGE_CODE	=	_T("554");	// 新ＯＣＲでの伝票読み取り（コクヨ伝票を含む）
static	char	*ICS_IMGMNG_SLIP_PACKAGE_CODE	=	_T("555");	// 新ＯＣＲで原票会計の読み取り

//---------------------------------------------------------------
// 新ＯＣＲでの伝票読み取り購入環境か？
//
//---------------------------------------------------------------
bool IsBuyOCRScan()
{
	static	int	IsBuyOCRScan = -1;

	if( IsBuyOCRScan == 1 ){
		return true;
	}
	else if( IsBuyOCRScan == 0 ){
		return false;
	}
	else{
		bool bRt = false;
		int st = IsPakageCd( ICS_IMGMNG_OCR_PACKAGE_CODE );
		if( st == 0 ){
			IsBuyOCRScan = 1;
			bRt = true;
		}
		else{
			IsBuyOCRScan = 0;
		}

		return bRt;
	}
}

//---------------------------------------------------------------
//	上手君クラウド 仕訳確定移行メッセージサイン[11.11 /16]
//
//	0 = 仕訳確定移行のメッセージを表示する
//	1 = 　　〃　　　			を表示しない
//---------------------------------------------------------------
int& JZcloudEndKakutei()
{
	return Jzcloud_kakuflg;
}


//終了時のクラウド設定ができる環境か？
BOOL IsCloudSetupEnv()
{
	//顧問先は対象外
	if( IsJz2() ) return FALSE;

	//クラウドマスターの判定を先にしないと、WebReq が クラウドでないマスターでも呼ばれる。 05.16 /18
	CZmGen8		ZmGen;
//	if( ZmGen.IsJzCloudMaster( pDBzm ) == 0) return FALSE;
	if( ZmGen.IsJzCloudMaster( pDBzm->zvol->apno, pDBzm->zvol->v_cod ) == 0) return FALSE;

	//170524 3拠点対応により　事務所のみを通すように変更
	//　親以外は通さない
	Jz2Option	jz20option;         //上手クラウド情報取得クラス

	if(jz20option.WebReq_PCType() != 0){
		return 0;
	}

	if( bCONFIRM_MASTER )	return FALSE;

	return TRUE;
}



//クラウド共有＋上手くん環境か？
BOOL IsJozuEnvCloudMaster()
{
	BOOL ret;

	//処理を高速化するため、関数呼び出し数を制限する
	if( JozuEnvCloud == -1 ) {
		JozuEnvCloud = 0;
		CZmGen8		ZmGen;
		if( ZmGen.IsJzCloudMaster(pDBzm) ) {
			//顧問先
			if( IsJz2() ) JozuEnvCloud = 1;
		}
	}
	if( JozuEnvCloud == 0 )	ret = FALSE;
	else {
		ret = TRUE;
	}

	return ret;
}

//--------------------------------------------
//	伝票タイプ フラグを取得
//
//--------------------------------------------
int& DenpTypeMode()
{
	return DenpType_sysflg;
}


//--------------------------------------------
//	伝票データか？
//
//--------------------------------------------
BOOL IsDenpyoSiwake( CDBINPDataRec* prec )
{
	BOOL ret = FALSE;

	if(  prec->m_icno > 0 && 
		(prec->m_dtype == 0x10 || prec->m_dtype == 0x20 || prec->m_dtype == 0x30) )
		ret = TRUE;

	return ret;
}


//--------------------------------------------
//	日付欄 タブキーモード
//
//--------------------------------------------
int& DateTabKeyMode()
{
	return DateTab_sysflg;
}


//--------------------------------------------
//	定型仕訳の日付で税率自動判定 チェック
//
//--------------------------------------------
int& RDsiwakeZeirituMode()
{
	return RDsiwakeZritu_flg;
}


void GetConfirmString(CString& str)
{
	if (bCONFIRM_MASTER & 0x100) {
		str.Format("『クラウド参照モード』");
	}
	else {
		str.Format("『決算確定マスター』");
	}
}


//--------------------------------------------
//	ReadOCR から起動か？
//
//--------------------------------------------
int& FromReadOCR()
{
	return FromReadOCR_flg;
}


//--------------------------------------------
//	科目欄カナ文字検索タイプ
//	0: 1文字
//  1: 複数文字
//--------------------------------------------
int& KanaSearchType()
{
	return KanaSearch_sysflg;
}

//--------------------------------------------
//	科目欄カナ検索 桁数取得
//--------------------------------------------
int KanaSearchKeta()
{
	return KanaSearch_sysflg ? 10 : 1;
}

//-----------------------------------------------------------
//	消費税欄 原則課税で止まるか？
//
//-----------------------------------------------------------
int SyzStopMode()
{
	//簡易課税、免税を除く
	if( pDBzm->IsKaniSyz() || pDBzm->IsMenzeiSyz() )
		return 0;

	return SyzStop_sysflg;
}

//-----------------------------------------------------------
//	消費税欄 原則課税で止まる mctl の値セット・ゲット
//
//-----------------------------------------------------------
int& SyzStopModeValue()
{
	return SyzStop_sysflg;
}

//#define PACKAGE_810 1

//== ICS請求書管理(WIZ) の製品コード ==
#ifndef PACKAGE_810
static	char	*ICS_WIZ_PACKAGE_CODE = _T("810");
#else
static	char	*ICS_WIZ_PACKAGE_CODE = _T("555");
#endif

/*
・上手くん環境
	DxCGの製品コード：901
	請求書管理オプションの製品コード：902
*/
static	char	*ICS_JZ_WIZ_PACKAGE_CODE = _T("902");

//---------------------------------------------------------------
// ICS請求書管理(WIZ)購入環境か？
//
//---------------------------------------------------------------
bool IsBuyWizOption()
{
	static	int	IsBuyWizOption = -1;

#ifndef PACKAGE_810
#else
	myICSMessageBox("package 555");
#endif

	//ATOMⅡクラウド 上手くん共有マスター
	CZmGen8		ZmGen;
	if( ZmGen.IsATOM2CloudMaster(pDBzm) ) {
		return false;
	}
	//上手WL顧問先ユーザはちがう
	if( GET_JZUSER() ) {
		return false;
	}
//#ifdef _DEBUG	//ATOMⅡクラウド 上手くん共有マスター, 上手WL顧問先ユーザのテスト用
//	return false;
//#endif

	if( IsBuyWizOption == 1 ) {
		return true;
	}
	else if( IsBuyWizOption == 0 ) {
		return false;
	}
	else {
		bool bRt = false;
		int st = -1;

		//モジュールにてライセンス判定
		bRt = ZmGen.IsBuyWizOption();
		if( bRt == true ) {
			IsBuyWizOption = 1;
		}
		else {
			IsBuyWizOption = 0;
		}
#ifdef PH1_5_CLOSE
		// 上手くん
		if( IsJz2() ) {
			// 取得しているライセンス
			// D0:経理上手くん	D1:給与上手くん	D2:請求書管理
			int				lictype = 0;
			// ライセンスファイルのオープン
			CString			tmpOptionKey;
			Jz2LicenseXML	tmpXML;
			if( tmpXML.LicenseOpen(1) ) {
				IsBuyWizOption = 0;
				return false;
			}
			Jz2LicenseArray	LcAry;
			if( tmpXML.EnumLicense(LcAry) ) {
				IsBuyWizOption = 0;
				return false;
			}
			CTime			ctm = CTime::GetCurrentTime();
			CString			nowDate;
			nowDate.Format(_T("%04d%02d%02d"), ctm.GetYear(), ctm.GetMonth(), ctm.GetDay());

			int				cnt = 0;
			int				max = (int)LcAry.GetCount();
			CString			tmpDate;
			for( int ii = 0; ii < max; ii++ ) {
				// 最終期限前
				tmpDate.Format(_T("%04d%02d%02d"), LcAry[ii].ptime.GetYear(), LcAry[ii].ptime.GetMonth(), LcAry[ii].ptime.GetDay());
				if( atoi(nowDate) <= atoi(tmpDate) ) {
					// 経理上手くん
					if( LcAry[ii].pid.Left(1) == _T("1") || LcAry[ii].pid.Left(1) == _T("2") ) {
						lictype = lictype | 0x01;
					}
					// 給与上手くん
					else if( LcAry[ii].pid.Left(1) == _T("3") ) {
						lictype = lictype | 0x02;
					}
					// 請求書管理
					else if( LcAry[ii].pid == ICS_JZ_WIZ_PACKAGE_CODE ) {
						lictype = lictype | 0x04;
					}
				}
			}
			// 3:使用可能なシステムがない
			if( lictype == 0 ) {
				st = -1;
			}
			// 0:請求書オプションなし
			else if( (lictype & 0x04) == 0 ) {
				st = -1;
			}
			// 1:経理上手、給与上手で請求書オプションあり
			else if( (lictype & 0x01) != 0 || (lictype & 0x02) != 0 ) {
				st = 0;
			}
			// 2:請求書オプションのみ
			else {
				st = 0;
			}
		}
		// ＡＴＯＭⅡ
		else if( IsICSAtom() ) {
			CStringArray	codeArray;
			codeArray.Add(ICS_WIZ_PACKAGE_CODE);
			if( IsOkLastDate2(codeArray) == 0 ) {
				st = 0;
			}
		}
		// ＡＴＬＡＳ
		else {
			if( IsPakageCd(ICS_WIZ_PACKAGE_CODE) == 0 ) {
				st = 0;
			}
		}
		if( st == 0 ) {
			IsBuyWizOption = 1;
			bRt = true;
		}
		else {
			IsBuyWizOption = 0;
		}
#endif

#ifdef _DEBUG
		IsBuyWizOption = 1;
		bRt = true;
#endif
		return bRt;
	}
}

// WIZ連携マスターサイン
static int WizRenkeiMast = -1;

void ResetWizRenkei()
{
	WizRenkeiMast = -1;
}

//---------------------------------------------------------------
// 選択マスターが WIZ連携マスターか？
//
//---------------------------------------------------------------
bool IsMasterWizRenkei(short apno, int code)
{
	if( WizRenkeiMast == -1 ) {
		CZmGen8 zmGen;
		bool ret = zmGen.IsWizMaster(apno,code);
		if( ret == true ) {
			WizRenkeiMast = 1;
		}
		else {
			WizRenkeiMast = 0;
		}
	}
	if( WizRenkeiMast ) {
		return true;
	}
	else {
		return false;
	}
}


//他の会社で WIZ連携マスターがあるか？
bool IsWizRenkeiExist(short apno, int code)
{
	//選択マスターがWIZマスターでない
	short chk_apno = -1;
	long  chk_cocode = -1;
	ICSMCTL	mctrl;
	CMasterId* ptbl;

	if( WizRenkeiMast <= 0 ) {
		mctrl.SetDatabase();
		//他の業種の会社も探す
		ptbl = mctrl.mid_open();
		if( ptbl != NULL ) {
			int rcnt = ptbl->GetRecordCount();

			ptbl->MoveFirst();
			for( int n = 0; n < rcnt; n++ ) {
				if( ptbl->optionflg & 0x40 ) {
					chk_apno = ptbl->apno;
					chk_cocode = ptbl->cocode;
					break;
				}
				ptbl->MoveNext();
			}
		}
	}
	//他に連携マスターがない
	if( chk_cocode == -1 ) {
		return false;
	}
	else {
		if( chk_apno == apno && code == chk_cocode ) //選択マスターと同じ
			return false;

		return true;
	}
}

//---------------------------------------------------------------
// 選択マスターをWIZ連携マスターとする
//
//---------------------------------------------------------------
int  RenkeiWizMaster(short apno, int code)
{
	long chk_cocode = -1;
	ICSMCTL	mctrl;
	CMasterId* ptbl;
	int st = -1;

	if( mctrl.SetDatabase() == 0 ) {

		try {
			ptbl = mctrl.mid_open(apno, code);
			if( ptbl != NULL ) {
				int rcnt = ptbl->GetRecordCount();

				if( rcnt == 1 ) {
					ptbl->Edit();
					ptbl->optionflg |= 0x40;
					st = ptbl->Update();
				}
				else if( rcnt == 0 ) {
					CString masterid;
					long optionflg = 0x40;
					st = mctrl.mid_addnew(apno, code, masterid, optionflg);
				}
				mctrl.mid_close();
			}
		}
		catch( CDBException* de ) {
			ermset(-1, (char*)(LPCTSTR)de->m_strError);
			return -1;
		}
	}
	return 0;
}




//-----------------------------------------------------------------------------
// 他にマスターDBに接続しているセッションがあるかをチェックする
//-----------------------------------------------------------------------------
// 返送値	1	：	他にセッションがある
//			0	：	当プログラムのみのセッション
//			-1	：	エラー
//-----------------------------------------------------------------------------
int DBsessionCheck()
{
	CRecordset	rcset(pDBzm->m_database);
	CString	sql, strvalue;
	DWORD pid = GetCurrentProcessId();
	DWORD process_id;
	vector<DWORD>	vecpid;

	CString mastDB;
	mastDB.Format(_T("Z%02x_C%08d_Y%04d"), Voln1->apno, Voln1->v_cod, ((Voln1->ee_ymd / 100) % 10000));

	try {
		// DBSesionを取得
		sql.Format(_T("SELECT s.session_id, s.login_name, s.host_process_id, s.status, s.host_name, s.program_name, s.database_id, s.login_time FROM sys.dm_exec_sessions AS s WHERE s.database_id = DB_ID('%s')"), mastDB);
		rcset.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly);

		for( int i = 0; ; i++ ) {

			if( i )
				rcset.MoveNext();
			if( rcset.IsEOF() )
				break;

			//host_process_id
			rcset.GetFieldValue((SHORT)2, strvalue);
			process_id = atoi((LPCTSTR)strvalue);

			//自身のプロセスIDは除く
			if( pid == process_id ) {
				continue;
			}
			vecpid.push_back(pid);
		}
		rcset.Close();
	}
	catch( CDBException *pde ) {
		pde->Delete();
		return -1;
	}

	//他にセッションなし
	if( vecpid.size() == 0 )
		return 0;

	// 重複を削除
	sort(vecpid.begin(), vecpid.end());
	vecpid.erase(unique(vecpid.begin(), vecpid.end()), vecpid.end());

	int ret = 0;
	if( vecpid.size() > 0 )	ret = 1;

	return ret;
}



//不正な WIZのoptionflg状態か？
//	返送値：WIZ 連携ビットが ON のマスター数
//
int InvalidWizOptionFlg()
{
	ICSMCTL	mctrl;
	CMasterId* ptbl;

	mctrl.SetDatabase();
	//他の業種の会社も探す
	ptbl = mctrl.mid_open();
	int renkei = 0;

	if( ptbl != NULL ) {
		int rcnt = ptbl->GetRecordCount();

		ptbl->MoveFirst();
		for( int n = 0; n < rcnt; n++ ) {
			if( ptbl->optionflg & 0x40 ) {
				renkei++;
			}
			ptbl->MoveNext();
		}
	}

	return renkei;
}


// WIZ optionflg をリセットする
int ResetWizOptionFlg()
{
	ICSMCTL	mctrl;
	CMasterId* ptbl;

	mctrl.SetDatabase();
	//他の業種の会社も探す
	ptbl = mctrl.mid_open();
	int ret = 0;

	try {
		if( ptbl != NULL ) {
			int rcnt = ptbl->GetRecordCount();

			ptbl->MoveFirst();
			for( int n = 0; n < rcnt; n++ ) {
				if( ptbl->optionflg & 0x40 ) {
					ptbl->Edit();
					ptbl->optionflg &= ~0x40;
					ret = ptbl->Update();
				}
				ptbl->MoveNext();
			}
		}
	}
	catch( CDBException* de ) {
		ermset(-1, (char*)(LPCTSTR)de->m_strError);
		return -1;
	}

	return ret;
}


// 該当マスターのみ WIZ optionflg をリセットする 
int ResetMasterWizOptionFlg()
{
	ICSMCTL	mctrl;
	CMasterId* ptbl;

	mctrl.SetDatabase();
	ptbl = mctrl.mid_open(Voln1->apno, Voln1->v_cod);
	int ret = 0;

	try {
		if( ptbl != NULL ) {
			int rcnt = ptbl->GetRecordCount();

			if( rcnt == 1 ) {
				ptbl->Edit();
				ptbl->optionflg &= ~0x40;
				ptbl->Update();
				ret = 1;
			}
		}
	}
	catch( CDBException* de ) {
		ermset(-1, (char*)(LPCTSTR)de->m_strError);
		return -1;
	}

	return ret;
}


// DocumentDB のオープン処理
//	返送値： -1	エラー
//			　1	DocmentDB オープン成功
//			  0 DocmentDBなし
//
int OpenDocDBMaster(LPCTSTR sqlsvr, LPCTSTR apno, LPCTSTR cocode, LPCTSTR kikan )
{
	if( DocDBCheck(atoi(apno), atoi(cocode), atoi(kikan)) ) {
		if( pDCsb == NULL ) {
			if( (pDCsb = new CDocSub) == NULL ) {
				ermset(ERROR_MEMORY, "ドキュメントクラスオブジェクトを作成できません！");
				return -1;
			}
		}
		pDCsb->ExitMode(1);
		if( pDCsb->SetMasterDB(2, sqlsvr, atoi(apno), atoi(cocode), atoi(kikan), 0, 0, 0) == -1 ) {
			ermset(-1, (char*)(LPCTSTR)pDCsb->GetZmErrMsg());
			return -1;
		}
		CImgMng ImgMng;
		CReadOCRCnv ReadOCRCnv;
		//if( ReadOCRCnv.CheckMasterVersion(pDCsb, pDBzm) == -1 ) {
		if( ReadOCRCnv.CheckMasterVersion2(pDCsb, pDBzm) == -1 ) {
			ermset(-1, (char*)(LPCTSTR)ReadOCRCnv.GetErrmsg());
			return -1;
		}
		if( ImgMng.VerUpDocDB(pDCsb) ) {
			ermset(-1, (char*)(LPCTSTR)ImgMng.GetErrorMsg());
			return -1;
		}
		if( pDCsb->VolumeOpen() == -1 ) {
			ermset(-1, (char*)(LPCTSTR)pDCsb->GetZmErrMsg());
			return -1;
		}

		if( pDCsb->DBDocOpen() ) {
			ermset(-1, (char*)(LPCTSTR)pDCsb->GetZmErrMsg());
			return -1;
		}
		if( pDCsb->XDBDocOpen() ) {
			ermset(-1, (char*)(LPCTSTR)pDCsb->GetZmErrMsg());
			return -1;
		}
		if( pDCsb->XDBDocExt1Open() ) {
			ermset(-1, (char*)(LPCTSTR)pDCsb->GetZmErrMsg());
			return -1;
		}
		if( pDCsb->XDBDocExt2Open() ) {
			ermset(-1, (char*)(LPCTSTR)pDCsb->GetZmErrMsg());
			return -1;
		}
		if( pDCsb->DBDocExt1Open() ) {
			ermset(-1, (char*)(LPCTSTR)pDCsb->GetZmErrMsg());
			return -1;
		}
		if( pDCsb->DBDocExt2Open() ) {
			ermset(-1, (char*)(LPCTSTR)pDCsb->GetZmErrMsg());
			return -1;
		}
		if( pDCsb->HashOpen() ) {
			ermset(-1, (char*)(LPCTSTR)pDCsb->GetZmErrMsg());
			return -1;
		}
		//	if( pDCsb->TstOpen() ) return FALSE;
		if( pDCsb->TstOpenFast("") ) {
			ermset(-1, (char*)(LPCTSTR)pDCsb->GetZmErrMsg());
			return -1;
		}
		if( pDCsb->Sheet_typeOpen() ) {
			ermset(-1, (char*)(LPCTSTR)pDCsb->GetZmErrMsg());
			return -1;
		}
		if( pDCsb->TaglnkOpen() ) {
			ermset(-1, (char*)(LPCTSTR)pDCsb->GetZmErrMsg());
			return -1;
		}
		if( pDCsb->XTaglnkOpen() ) {
			ermset(-1, (char*)(LPCTSTR)pDCsb->GetZmErrMsg());
			return -1;
		}
		if( pDCsb->DoctagOpen() ) {
			ermset(-1, (char*)(LPCTSTR)pDCsb->GetZmErrMsg());
			return -1;
		}
		if( pDCsb->GrpentOpen() ) {
			ermset(-1, (char*)(LPCTSTR)pDCsb->GetZmErrMsg());
			return -1;
		}
		if( pDCsb->RcgMsaidataOpen() ) {
			ermset(-1, (char*)(LPCTSTR)pDCsb->GetZmErrMsg());
			return -1;
		}
		if( pDCsb->Rcg_GeneralOpen() ) {
			ermset(-1, (char*)(LPCTSTR)pDCsb->GetZmErrMsg());
			return -1;
		}
		//追加20190626
		if( pDCsb->ShopRecOpen() ) {
			ermset(-1, (char*)(LPCTSTR)pDCsb->GetZmErrMsg());
			return -1;
		}
		//書類種別
		if( pDCsb->Sheet_typeOpen() ) {
			ermset(-1, (char*)(LPCTSTR)pDCsb->GetZmErrMsg());
			return -1;
		}
		if( pDCsb->OwnTblOpen() ) {
			ermset(-1, (char*)(LPCTSTR)pDCsb->GetZmErrMsg());
			return -1;
		}

		return 1;
	}
	return 0;
}

