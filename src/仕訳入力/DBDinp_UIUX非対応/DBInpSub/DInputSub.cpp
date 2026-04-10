#include "stdafx.h"

#include "DBInpSubFunc.h"
#include "DInputSub.h"
#include "DInpCol.h"
#include "CStRecordSet.h"
#include "DBInpSub8.h"
#include "jz2cloudopt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace	Jz2CloudApi;

/////////////////////////////////////////////////////////////////////////////
// 元帳検索からの伝票形式呼び出し時に使う。
int CDInputSub::m_jzSansyo = -1;
int CDInputSub::sv_apno = -1;
int CDInputSub::sv_code = -1;
int CDInputSub::sv_yymmdd = -1;

#define SIZE_DSINE 6

// m_Seldata より呼ばれる関数
//	固定科目部分に内容をセットする
int CALLBACK CDInputSub::SetSelectFix8(_SelTable* pSel, SelSetProcPar* pSetProc, int cnt, CWnd* lp)
{
#ifdef INP_CLOSE
	BOOL bNumber;
	CDnpInpDialog* pDlg;
	pDlg = (CDnpInpDialog*)lp;
	CDnpBaseClass* pBase = pDlg->m_base;

	if( pBase->m_Seldata.GetKmDispMode() == SL_KMK_CODE )
		bNumber = FALSE;
	else
		bNumber = TRUE;

	CICSSel*	psel8 = NULL;
	psel8 = pBase->m_pselFix8;

	if( psel8 == NULL )	return 0;

	if( psel8->GetNumber() != bNumber )
		psel8->SetNumber(bNumber);

	psel8->SetData((long)pSel);

#endif
	return 0;
}


// m_Seldata より呼ばれる関数
//	科目選択部分に内容をセットする
int CALLBACK CDInputSub::SetSelectSel32(_SelTable* pSel, SelSetProcPar* pSetProc, int cnt, CWnd* lp)
{
#ifdef INP_CLOSE

	BOOL bNumber;
	CDnpInpDialog* pDlg;
	pDlg = (CDnpInpDialog*)lp;
	CDnpBaseClass* pBase = pDlg->m_base;

	switch( pSetProc->sp_nowpn ) {
	case SL_KAMOKU_PN:
	case SL_KMKANA_PN:
		if( pBase->m_Seldata.GetKmDispMode() == SL_KMK_CODE )
			bNumber = FALSE;
		else
			bNumber = TRUE;
		break;
	case SL_BUMON_PN:
	case SL_BMNKANA_PN:
	case SL_KOJI_PN:
	case SL_KOJIKANA_PN:
		if( pBase->pAUTOSEL->BMNSEL_OPT == OPT_NONOPT )
			bNumber = FALSE;
		else
			bNumber = TRUE;
		break;

	default:
		bNumber = TRUE;
		if( pBase->m_bNotNumber )
			bNumber = FALSE;
		break;
	}

	CICSSel*	psel32 = NULL;
	psel32 = pBase->m_pselSel32;

	if( psel32->GetNumber() != bNumber )
		psel32->SetNumber(bNumber);

	if( pSetProc->sp_nowpn == SL_BRNTKY_PN ) {
		// 選択画面の色
		COLORREF txt = pBase->m_Seldata.GetSel32TextColor();
		COLORREF back = pBase->m_Seldata.GetSel32BackColor();

		psel32->SetForeColor(txt);	// 文字色
		psel32->SetBackColor(back);	// 背景色

		pBase->BRNTKY_sel = 1;
	}
	else {
		if( pSetProc->sp_prevpn == SL_BRNTKY_PN ) {
			// 選択画面の色を元に戻す
			pBase->enable_fixval(TRUE);	// 元背景色強制描画のため
		}
		else {
			if( psel32->GetBackColor() == BRTEK_BackCOL ) { //枝番の背景色が残る場合があった。
				pBase->enable_fixval(TRUE);
			}
		}
		pBase->BRNTKY_sel = 0;
	}

	pBase->m_SelDispPN = pSetProc->sp_nowpn;
	CString	stitle = pSetProc->seljob;

	// 03.16 /11
	if( pSetProc->sp_nowpn == SL_BUMON_PN || pSetProc->sp_nowpn == SL_KOJI_PN ||
		pSetProc->sp_nowpn == SL_BMNKANA_PN || pSetProc->sp_nowpn == SL_KOJIKANA_PN ) {
		if( (pBase->pAUTOSEL->BMNSEL_OPT == OPT_BMNSEL) )
			stitle += _T("\r\n (画面番号優先) ");
		else {
			if( pSetProc->sp_nowpn == SL_BUMON_PN || pSetProc->sp_nowpn == SL_BMNKANA_PN )
				stitle += _T("\r\n (部門番号優先) ");
			else
				stitle += _T("\r\n (工事番号優先) ");
		}

		if( pSetProc->sp_nowpn == SL_KOJI_PN || pSetProc->sp_nowpn == SL_KOJIKANA_PN ) {
			stitle += _T("\r\n [*]は完成工事 ");
		}
	}

	pBase->set_valtitle((char*)(LPCTSTR)stitle);
	psel32->SetData((long)pSel);

	//FILETrace( ">> SetSelectSel32 BRNTKY %d\n", pBase->BRNTKY_sel );
#endif
	return 0;
}


//CSWKDSub クラスからの呼び出しでは、クラス初期化時に毎回チェックする
BOOL CDInputSub::IsJzSansyo()
{
	if( m_bSWKDsub ) {
		return m_SWKDjzSansyo == 1 ? TRUE : FALSE;
	}
	else {
		return m_jzSansyo == 1 ? TRUE : FALSE;
	}
}

CDInputSub::CDInputSub()
{
	m_pZm = NULL;
	m_pSy = NULL;
	pZeiCl = NULL;
	pTkjour = NULL;

	m_bModify = FALSE;

	dwMST_TYPE = 0;;
	bCONFIRM_MASTER = FALSE;
	m_bNPanbn = FALSE;
	
	::ZeroMemory( &m_InpBcd, sizeof m_InpBcd );
	m_dacnt = 0;

	::ZeroMemory( &m_SYOGdbt, sizeof m_SYOGdbt );
	::ZeroMemory( &m_SYOGcre, sizeof m_SYOGcre );
	::ZeroMemory( &m_SYOGzan, sizeof m_SYOGzan );

	JozuEnvCloud = -1;
	m_bSWKDsub = FALSE;
	m_SWKDjzSansyo = -1;

	m_inpSofs = m_inpEofs = -1;
	m_inpSy = m_inpEy = -1;
	m_pParent = NULL;

	lnkOpen = jourOpen = 0;
}


CDInputSub::~CDInputSub()
{

}

//CSWKDsub クラスの内部クラスとして使用する場合
void CDInputSub::UseSWKDsub() 
{
	m_bSWKDsub = TRUE;
}


int CDInputSub::InitSub(CDBZmSub* pZm, CDBSyohi* pSy, struct _AUTOSEL *pAuto, CWnd* selParent)
{
	return func_initsub(pZm, pSy, pAuto, selParent);
}


int CDInputSub::InitSub( CDBZmSub* pZm, CDBSyohi* pSy, struct _AUTOSEL *pAuto )
{
	return func_initsub(pZm, pSy, pAuto, NULL);
}


int CDInputSub::func_initsub(CDBZmSub* pZm, CDBSyohi* pSy, struct _AUTOSEL *pAuto, CWnd* selParent)
{
	int ret = 0;

	m_pZm = pZm;
	m_pSy = pSy;
	m_pAsel = pAuto;
	m_pParent = selParent;

	//マスターサイン
	CZmGen8		ZmGen;
	dwMST_TYPE = 0;
//	int vver = GetMasterVver( m_pZm );
	int vver = ZmGen.CheckValiableVersion( m_pZm->m_database, ID_H26SYZ_VUP_CHK );
//	int vver = 1;
	bInvDisp = FALSE;

	if( vver >= 1 ) {
		dwMST_TYPE |= MST_INSERT;
		dwMST_TYPE |= MST_TKY20;

		int ver2 = ZmGen.CheckValiableVersion( m_pZm->m_database, ID_H31SYZ_VUP_CHK );
//		EnumIdSyzKikan	syzKikan = ZmGen.GetSyzKikan(pDBzm->zvol->ee_ymd);
		
		if( ver2 >= 1 ) {
			if( m_pZm->zvol->ss_ymd >= SYZ10_YMD ) {
				dwMST_TYPE |= MST_SYZ10;
			}
			else if( m_pZm->zvol->ee_ymd >= SYZ10_YMD ) {
				dwMST_TYPE |= MST_SYZ8_10;
			}

		}
		if( m_pZm->zvol->ss_ymd >= SYZ8_YMD ) {
			dwMST_TYPE |= MST_SYZ8;
		}
		else if( m_pZm->zvol->ee_ymd >= SYZ8_YMD ) {
			dwMST_TYPE |= MST_SYZ5_8;
		}
		int ver3 = ZmGen.CheckValiableVersion(m_pZm->m_database, ID_INV_SYZ_VUP_CHK);
		if( ver3 >= 1 ) {
			if( ZmGen.IsAbleToUseInvoice(m_pZm) ) {
				dwMST_TYPE |= MST_SYZ_INV;
				// 登録番号を表示するか
				bInvDisp = ZmGen.IsDispInvno(m_pZm);
			}
		}
	}
	else if( vver == -1 ) {
		ret = -1;
		m_ErrMsg = ZmGen.GetErrorMsg();
	}
	char	sqlsvr[256], apno[20], cocode[20], kikan[20];
	if( PCOM_GetString("SELECTCODSN", sqlsvr, apno, cocode, kikan) != 4 ) {
		m_ErrMsg = _T("コモン情報の取得に失敗しました！");
		return -1;
	}
	int nApno, nCode, nKikan;
	nApno = atoi(apno);
	nCode = atoi(cocode);
	nKikan = atoi(kikan);

	if( m_bSWKDsub ) {
		if( m_SWKDjzSansyo == -1 ) {
			//上手君参照モード
			if( IsCloudSansyoMode(nApno, nCode, nKikan, sqlsvr) ) {
				MyTrace("IsCloudSansyoMode ret != 0\n");
				m_SWKDjzSansyo = 1;
			}
			else {
				MyTrace("IsCloudSansyoMode ret == 0\n");
				m_SWKDjzSansyo = 0;
			}
		}
		//	AfxMessageBox("※InpDLL※上手参照モードテスト中");
		//	m_jzSansyo = 1;
		//m_SWKDjzSansyo = 0;
	}
	else {
		if( m_jzSansyo != -1 ) {
			if( sv_apno != nApno || sv_code != nCode || sv_yymmdd != nKikan ) {
				m_jzSansyo = -1;
			}
		}
	
		if( m_jzSansyo == -1 ) {
			//上手君参照モード
			sv_apno = nApno;
			sv_code = nCode;
			sv_yymmdd = nKikan;
	
			if( IsCloudSansyoMode(nApno, nCode, nKikan, sqlsvr) ) {
				MyTrace("IsCloudSansyoMode ret != 0\n");
				m_jzSansyo = 1;
			}
			else {
				MyTrace("IsCloudSansyoMode ret == 0\n");
				m_jzSansyo = 0;
			}
		}
		//AfxMessageBox("※InpDLL※上手参照モードテスト中");
		//m_jzSansyo = 1;
	}

	// マスター全体確定か or 参照モード？
	if( ! (m_pZm->zvol->tl_cor & 0x10) )
		bCONFIRM_MASTER = TRUE;
	else
		bCONFIRM_MASTER = FALSE;

	if( m_jzSansyo == 1 ) {
		bCONFIRM_MASTER |= 0x100;
	}

	if( IsSyafuKaiseiAnbun( m_pZm ) ) {
		m_bNPanbn = TRUE;
	}

	if( m_pZm->zvol->apno == 0x20 ) {
	//	M_KOJI = (m_pZm->zvol->sub_sw&0x04) ? TRUE : FALSE;

		M_MISEIKOJI_TYPE = FALSE;
		//処理タイプ → 未成工事支出金タイプ
		if( m_pZm->object_check( 2, "KJVOL") != 0 ) {
			CStRecordSet st_rec( m_pZm->m_database );
			st_rec.m_sql = _T( "select VOL_MTYP as st from KJVOL" );

			TRY {
				if( ! st_rec.Open() ) {
					m_ErrMsg =  _T( "工事マスタータイプのチェックが行えません！");
					return -1;
				}
				if( st_rec.m_st > 0 ) {
					M_MISEIKOJI_TYPE = TRUE;
				}
				st_rec.Close();
			}
			CATCH( CMemoryException, me ) {
				me->GetErrorMessage( m_ErrMsg.GetBuffer(_MAX_PATH), _MAX_PATH );
				m_ErrMsg.ReleaseBuffer();
				return -1;
			}
			CATCH( CDBException, de ) {
				m_ErrMsg = de->m_strError;
				return -1;
			}
			END_CATCH
		}
	}
	else {
		M_MISEIKOJI_TYPE = FALSE;
	}

	if( m_BrTek.brntky_iniz(m_pZm) != 0 ) {
		ret = -1;
		m_ErrMsg =  "枝番摘要処理の初期化ができませんでした。";
	}

	// 枝番文字列に科目コード追加 サインをセット
	m_BrTek.brntky_kmcode( m_pAsel->SCNCD_OPT == OPT_SCNCD );

	if( m_Seldata.Init(pZm, selParent, SetSelectFix8, SetSelectSel32) != 0 ) {
		ret = -1;
		m_ErrMsg = "選択処理の初期化ができませんでした。";
	}

	// 現在仕訳数を再セット
	CString filter;
	filter = _T( "seq = 1" );
	m_pZm->dcntl->Requery( filter );
	if( m_pZm->dcntl->st != -1 ) {
		m_dacnt	= m_pZm->dcntl->cnt;
	}

	if (pZeiCl == NULL) {
		pZeiCl = new CDataZeiCalq();
		if (pZeiCl->DZCIniz(m_pZm) != 0)
			ret = -1;
	}
	minfo = m_pZm->minfo;
	zvol = m_pZm->zvol;

	if( m_pZm->tklnk == NULL ) {
		if( m_pZm->TklnkOpen() == -1 ) {
			ret = -1;
			m_ErrMsg = "摘要リンクテーブルのオープンに失敗しました";
		}
		lnkOpen = 1;
	}
	if( m_pZm->tkjour == NULL ) {
		if( m_pZm->TkjourOpen() == -1 ) {
			m_ErrMsg = "摘要仕訳テーブルのオープンに失敗しました";
		}
		jourOpen = 1;
	}

	//摘要対応
	if( pTkjour == NULL ) {
		if( (pTkjour = new CZIM_TkjourUtil()) == NULL ) {
			ret = -1;
			m_ErrMsg = "摘要対応オブジェクトの作成に失敗しました！";
		}
	}

	ICS_TKJOURUTIL_PAC jpac = { 0 };
	jpac.zmsub = m_pZm;
	jpac.CallFlg = 5;
	if( pTkjour->Init(jpac) == -1 ) {
		m_ErrMsg = "摘要対応オブジェクトの初期化に失敗しました！";
		ret = -1;
	}

	return ret;
}


void CDInputSub::CloseSub()
{
	m_BrTek.brntky_close();
	m_Seldata.Close();

	if( lnkOpen ) {
		lnkOpen = 0;
		m_pZm->TklnkClose();
	}
	if( jourOpen ) {
		jourOpen = 0;
		m_pZm->TkjourClose();
	}

	if (pZeiCl != NULL) {
		pZeiCl->DZCClose();
		delete pZeiCl;	pZeiCl = NULL;
	}
	//メモリー開放
	if( pTkjour != NULL ) {
		pTkjour->End();
		delete pTkjour;	pTkjour = NULL;
	}
}

//証憑番号入力可？
BOOL	CDInputSub::IsSyonum()
{
	BOOL ret;

	ret = (m_pZm->zvol->g_hsw & 0x04) ? TRUE : FALSE;

	return ret;
}

BOOL	CDInputSub::IsEdaban()
{
	BOOL ret;

	ret = (m_pZm->zvol->br_sw & 0x01) ? TRUE : FALSE;

	return ret;
}


BOOL	CDInputSub::IsBumon()
{
	BOOL ret;

	ret = (m_pZm->zvol->sub_sw&0x02) ? TRUE : FALSE;

	return ret;
}


BOOL	CDInputSub::IsKoji()
{
	BOOL ret;

	if( m_pZm->zvol->apno == 0x20 ) {
		ret = (m_pZm->zvol->sub_sw&0x04) ? TRUE : FALSE;
	}
	else {
		ret = FALSE;
	}
	return ret;
}



//------------------------------------------------------------
//	科目が消費税科目か？
//
//	return	0以外	消費税科目(1=不課税, 2=課税, 3=非課税)
//					D8 ON: 棚卸し科目
//					D9 ON: 仮受・仮払消費税
//					D10 ON: 未成工事支出金(未成工事タイプのみ完成工事振替)
//			0		消費税科目でない
//------------------------------------------------------------
int CDInputSub::km_syattr( DBKNREC* pKn )
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
		svkcd1 = _atoh( (LPSTR)(LPCTSTR)m_pZm->szvol->SVkcod1 );
		svkcd2 = _atoh( (LPSTR)(LPCTSTR)m_pZm->szvol->SVkcod2 );

		if( (dwCode == svkcd1 || dwCode == svkcd2) || 
			((dwCode&0x80) && (dwCode & 0xffffff00) == (svkcd1 & 0xffffff00)) ||
			((dwCode&0x80) && (dwCode & 0xffffff00) == (svkcd2 & 0xffffff00)) ) {
		//if( (dwCode == svkcd1 || dwCode == svkcd2) ) {
			retn |= 0x0200;
		}
		else {
			//個人兼業
			if( m_pZm->zvol->apno == 0x10 && (m_pZm->zvol->ind_table&0x01) ) {
				if( dwCode == 0x01071110 || dwCode == 0x01071120 || dwCode == 0x04051610 || dwCode == 0x04051620 )
					retn |= 0x0200;
			}
			if( m_pZm->zvol->apno == 0x20 && M_MISEIKOJI_TYPE ) {
				if( (dwCode & 0xffffff00) == 0x01060200 ) {
					retn |= 0x0400;
				}
			}
		}

	}

	return( retn );
}


//------------------------------------------------------------
//	科目が消費税科目か？(同上)
//------------------------------------------------------------
int CDInputSub::km_syattr( CString& kcode )
{
	DBKNREC* pKn;
	pKn = DB_PjisToKnrec( kcode, m_pZm, TRUE );
	int ret = 0;

	if( pKn ) {
		ret = km_syattr( pKn );
	}
	return ret;
}


//------------------------------------------------------------
//	科目が輸出売上科目か？
//------------------------------------------------------------
int CDInputSub::km_yusyutsu(DBKNREC* pKn)
{
	int ret = 0;

	if( pKn->knsgn[0] == 0x02 && pKn->knsgn[1] == 0x09 )
		ret = 1;

	return ret;
}

//------------------------------------------------------------
//	科目が輸出売上科目か？
//------------------------------------------------------------
int CDInputSub::km_yusyutsu(CString& kcode)
{
	DBKNREC* pKn;
	pKn = DB_PjisToKnrec(kcode, m_pZm, TRUE);
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
int CDInputSub::km_zeikbn( DBKNREC* pKn )
{
	BYTE	attr[10] = {0};
	int retn = -1;

	memmove( &attr, &pKn->knsgn, sizeof(pKn->knsgn) );

	BYTE	chkBit = (attr[0]&0x0f);

	if( chkBit != 0 ) {
		//資産・売上・仕入・経費が対象
		if( (m_pZm->zvol->s_sgn7 & chkBit) )	retn = 1;	//外税
		else									retn = 0;	//内税
	}
	return retn;
}

//同上
int CDInputSub::km_zeikbn( CString& kcode )
{
	DBKNREC* pKn;
	pKn = DB_PjisToKnrec( kcode, m_pZm, TRUE );
	int ret = 0;

	if( pKn ) {
		ret = km_zeikbn( pKn );
	}
	return ret;
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

void CDInputSub::GetCData( CDBipDataRec* rec )
{
	CDATA* data;
	data = m_pZm->data;

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


void CDInputSub::GetCData(CDBINPDataRec* rec)
{
	CDATA* data;
	data = m_pZm->data;

	if( data->seq == -1 ) {
		rec->Reset();
	}
	else {
		rec->m_seq = data->seq;
		rec->m_ksign = data->ksign;
		rec->m_ddate = data->ddate;
		rec->m_mofs = data->mofs;
		rec->m_cno = data->cno;
		rec->m_dbmn = data->dbmn;
		rec->m_dkno = data->dkno;
		rec->m_dkno.TrimRight();
		rec->m_dbt = data->dbt;
		rec->m_dbt.TrimRight();
		rec->m_dbr = data->dbr;

		rec->m_cbmn = data->cbmn;
		rec->m_ckno = data->ckno;
		rec->m_ckno.TrimRight();
		rec->m_cre = data->cre;
		rec->m_cre.TrimRight();
		rec->m_cbr = data->cbr;

		l_input(rec->m_val, (void*)(LPCTSTR)data->val);
		l_input(rec->m_zei, (void*)(LPCTSTR)data->zei);

		rec->m_dsign.Copy(data->dsign);
		rec->m_owner = data->owner;
		rec->m_dtype = data->dtype;

		rec->m_tekiyo = data->tekiyo;
		rec->m_imgsq = data->imgsq;
		rec->m_snumber = data->snumber;
		rec->m_tdate = data->tdate;
		rec->m_edate = data->edate;
		rec->m_udate = data->udate;
		rec->m_icno = data->icno;

		rec->m_skn = data->skn;
		rec->m_tag = data->tag;

		rec->m_hjiyu = data->hjiyu;
		l_input(rec->m_taika, (void*)(LPCTSTR)data->taika);

		rec->m_nbcd = -1;

		// インボイス対応
	//	sprintf_s( rec->m_invno, sizeof( rec->m_invno ), _T( "%s" ), data->invno );
		invmove(rec->m_invno, sizeof(rec->m_invno), data->invno);

		rec->m_rnumber = data->rnumber;

#ifdef NAIBU_VER2
		if( IsSyafuKaisei(zvol) ) {
			CString filter;
			filter.Format("seq = %d", data->seq);
			pDBnc->m_pNbtdarec->RequeryEx(filter);
			if( pDBnc->m_pNbtdarec->st == 0 ) {
				rec->m_nbcd = pDBnc->m_pNbtdarec->m_nbtbmn;
			}
		}
#endif
	}
}



void CDInputSub::SetCDBData(CDBipDataRec* rec)
{
	if( rec->m_seq <= 0 )	return;

	CDBDATA* dbdata;
	dbdata = m_pZm->dbdata;

	dbdata->seq = rec->m_seq;
	dbdata->ksign = rec->m_ksign;
	dbdata->ddate = rec->m_ddate;
	dbdata->mofs = rec->m_mofs;
	dbdata->cno = rec->m_cno;
	dbdata->dbmn = rec->m_dbmn;
	dbdata->dkno = rec->m_dkno;
	dbdata->dbt = rec->m_dbt;
	dbdata->dbr = rec->m_dbr;

	dbdata->cbmn = rec->m_cbmn;
	dbdata->ckno = rec->m_ckno;
	dbdata->cre = rec->m_cre;
	dbdata->cbr = rec->m_cbr;

	l_print(dbdata->val.GetBuffer(100), rec->m_val, "sssssssssss9");
	dbdata->val.ReleaseBuffer();
	l_print(dbdata->zei.GetBuffer(100), rec->m_zei, "sssssssssss9");
	dbdata->zei.ReleaseBuffer();

	dbdata->dsign.Copy(rec->m_dsign);
	dbdata->owner = rec->m_owner;/*DBUserNo()*/;
	dbdata->dtype = rec->m_dtype;

	dbdata->tekiyo = rec->m_tekiyo;
	dbdata->imgsq = rec->m_imgsq;
	dbdata->snumber = rec->m_snumber;
	dbdata->tdate = rec->m_tdate;
	dbdata->edate = rec->m_edate;
	dbdata->udate = rec->m_udate;
	dbdata->icno = rec->m_icno;

	dbdata->skn = rec->m_skn;
	dbdata->tag = rec->m_tag;

	dbdata->hjiyu = rec->m_hjiyu;
	l_print(dbdata->taika.GetBuffer(100), rec->m_taika, "sssssssssss9");
	dbdata->taika.ReleaseBuffer();

	// インボイス対応
	dbdata->invno = rec->m_invno;
}


void CDInputSub::SetCDBData(CDBINPDataRec* rec)
{
	if( rec->m_seq <= 0 )	return;

	CDBDATA* dbdata;
	dbdata = m_pZm->dbdata;

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

	// インボイス対応
	dbdata->invno = rec->m_invno;
	// 債権記録番号
	dbdata->rnumber = rec->m_rnumber;
}


void CDInputSub::GetCDBData( CDBipDataRec* rec, CDBDATA* ptr/*= NULL*/ )
{

	CDBDATA* dbdata;

	if( ptr != NULL ) {
		dbdata = ptr;
	}
	else {
		dbdata = m_pZm->dbdata;
	}

	if( dbdata->seq == -1 ) {
		rec->Reset();
	}
	else {
		rec->m_seq = dbdata->seq;
		rec->m_ksign = dbdata->ksign;
		rec->m_ddate = dbdata->ddate;
		rec->m_mofs = dbdata->mofs;
		rec->m_cno = dbdata->cno;
		rec->m_dbmn = dbdata->dbmn;
		rec->m_dkno = dbdata->dkno;
		rec->m_dkno.TrimRight();
		rec->m_dbt = dbdata->dbt;
		rec->m_dbt.TrimRight();
		rec->m_dbr = dbdata->dbr;

		rec->m_cbmn = dbdata->cbmn;
		rec->m_ckno = dbdata->ckno;
		rec->m_ckno.TrimRight();
		rec->m_cre = dbdata->cre;
		rec->m_cre.TrimRight();
		rec->m_cbr = dbdata->cbr;

		l_input(rec->m_val, (void*)(LPCTSTR)dbdata->val);
		l_input(rec->m_zei, (void*)(LPCTSTR)dbdata->zei);

		rec->m_dsign.Copy(dbdata->dsign);
		rec->m_owner = dbdata->owner;
		rec->m_dtype = dbdata->dtype;

		rec->m_tekiyo = dbdata->tekiyo;
		rec->m_imgsq = dbdata->imgsq;
		rec->m_snumber = dbdata->snumber;
		rec->m_tdate = dbdata->tdate;
		rec->m_edate = dbdata->edate;
		rec->m_udate = dbdata->udate;
		rec->m_icno = dbdata->icno;

		rec->m_skn = dbdata->skn;
		rec->m_tag = dbdata->tag;

		rec->m_hjiyu = dbdata->hjiyu;
		l_input(rec->m_taika, (void*)(LPCTSTR)dbdata->taika);

		rec->m_nbcd = -1;

		// インボイス対応
	//	sprintf_s( rec->m_invno, sizeof( rec->m_invno ), _T( "%s" ), dbdata->invno );
		invmove(rec->m_invno, sizeof(rec->m_invno), dbdata->invno);
#ifdef NAIBU_VER2
		if( IsSyafuKaisei(zvol) ) {
			CString filter;
			filter.Format("seq = %d", dbdata->seq);
			pDBnc->m_pNbtdarec->RequeryEx(filter);
			if( pDBnc->m_pNbtdarec->st == 0 ) {
				rec->m_nbcd = pDBnc->m_pNbtdarec->m_nbtbmn;
			}
		}
#endif
	}
}



void CDInputSub::GetCDBData(CDBINPDataRec* rec, CDBDATA* ptr/*= NULL*/ )
{
	CDBDATA* dbdata;

	if (ptr != NULL) {
		dbdata = ptr;
	}
	else {
		dbdata = m_pZm->dbdata;
	}

	if( dbdata->seq == -1 ) {
		rec->Reset();
	}
	else {
		rec->m_seq	= dbdata->seq;
		rec->m_ksign = dbdata->ksign;
		rec->m_ddate = dbdata->ddate;
		rec->m_mofs	= dbdata->mofs;
		rec->m_cno	= dbdata->cno;
		rec->m_dbmn	= dbdata->dbmn;
		rec->m_dkno	= dbdata->dkno;
		rec->m_dkno.TrimRight();
		rec->m_dbt	= dbdata->dbt;
		rec->m_dbt.TrimRight();
		rec->m_dbr	= dbdata->dbr;

		rec->m_cbmn	= dbdata->cbmn;
		rec->m_ckno	= dbdata->ckno;
		rec->m_ckno.TrimRight();
		rec->m_cre	= dbdata->cre;
		rec->m_cre.TrimRight();
		rec->m_cbr	= dbdata->cbr;

		l_input( rec->m_val, (void*)(LPCTSTR)dbdata->val );
		l_input( rec->m_zei, (void*)(LPCTSTR)dbdata->zei );

		rec->m_dsign.Copy( dbdata->dsign );
		rec->m_owner	= dbdata->owner;
		rec->m_dtype	= dbdata->dtype;

		rec->m_tekiyo	= dbdata->tekiyo;
		rec->m_imgsq	= dbdata->imgsq;
		rec->m_snumber	= dbdata->snumber;
		rec->m_tdate	= dbdata->tdate;
		rec->m_edate	= dbdata->edate;
		rec->m_udate	= dbdata->udate;
		rec->m_icno		= dbdata->icno;

		rec->m_skn		= dbdata->skn;
		rec->m_tag		= dbdata->tag;

		rec->m_hjiyu	= dbdata->hjiyu;
		l_input( rec->m_taika, (void*)(LPCTSTR)dbdata->taika );

		rec->m_nbcd = -1;

		rec->m_rnumber = dbdata->rnumber;

		// インボイス対応
	//	sprintf_s( rec->m_invno, sizeof( rec->m_invno ), _T( "%s" ), dbdata->invno );
		invmove(rec->m_invno, sizeof(rec->m_invno), dbdata->invno);
#ifdef NAIBU_VER2
		if( IsSyafuKaisei( zvol ) ) {
			CString filter;
			filter.Format( "seq = %d", dbdata->seq );
			pDBnc->m_pNbtdarec->RequeryEx( filter );
			if( pDBnc->m_pNbtdarec->st == 0 ) {
				rec->m_nbcd = pDBnc->m_pNbtdarec->m_nbtbmn;
			}
		}
#endif
	}
}


int CDInputSub::DB_DataCorrect(CDBipDataRec* data)
{
	CDBINPDataRec newdata;
	convNewDataRec(data, &newdata);

	return DB_DataCorrect(&newdata);
}


int CDInputSub::DB_DataCorrect(CDBINPDataRec* data )
{
	if( data->m_seq <= 0 )
		return 0;

	// マスター全体確定 or 参照モード
	if( ! (m_pZm->zvol->tl_cor & 0x10) || IsJzSansyo() )
		return 0;

	CString filter;
	filter.Format( "seq = %d", data->m_seq );
	m_pZm->dbdata->Requery( -2, 0, filter, 0, 0 );
//	dbdata_Requery() = TRUE;

#ifdef NAIBU_VER2
	if( IsSyafuKaisei( zvol ) ) {
		if( data->m_hjiyu == 1 && data->m_nbcd == 0 )
			data->m_hjiyu = 0;
	}
#endif
	SetCDBData( data );

	if( data->m_dbmn != -1 ) {
		BumonZanTorok( data->m_dbmn, data->m_dbt );
	}
	if( data->m_cbmn != -1 && ((data->m_dbmn != data->m_cbmn) || (data->m_dbt != data->m_cre)) ) {
		BumonZanTorok( data->m_cbmn, data->m_cre );
	}

	// チェックリストの出力サインを落とす。
//	dbdata->dsign[0] &= ~0x02;
	m_pZm->dbdata->dsign[0] &= ~0x0E;

#ifdef LATER_CLOSE
	// 追加修正サイン
	IsModify() = TRUE;
	// 科目残高 クエリーサイン
	kzrec_Requery() = TRUE;
	// CDATA クエリーサイン ON --- 表示を更新するため
	data_Requery()	= TRUE;

	// 仮受・仮払自動枝番チェック
	int old_edcnt = 0;
	old_edcnt = GetKariAutoBrnCnt();
#endif

	int st = m_pZm->dbdata->DataCorrect();

#ifdef LATER_CLOSE
	if( old_edcnt != GetKariAutoBrnCnt() ) {
		ezquery_flag() = TRUE;
	}
#endif
#ifdef NAIBU_VER2
	if( IsSyafuKaisei( zvol ) ) {
		pDBnc->NaibutorihikiDataAdd( dbdata->seq, data->m_nbcd );
	}
#endif
//MyTrace( "@DB_DataCorrect end dbdata seq = %d, st = %d\n", dbdata->seq, st );

	return st;
	
}



// 変動事由・振替科目・内部取引消去を行える業種か？
int CDInputSub::IsHjiyuItemGyosyu()
{
	int ret = 0;

	if( m_pZm == NULL )	return 0;

	if( ! IsSyafuKaisei( m_pZm->zvol ) ) {
		if( IsGakkouH27Master( m_pZm ) ) {	//学校H27 改正
			ret = 4;
		}
		else {
			if( IsOKGyosyu( m_pZm->zvol->apno, SW_SOCIAL|SW_SYUKYO|SW_SCHOOL)/* == 0x52*/ ) {
				return 0;
			}
		}

		if( IsOKGyosyu( m_pZm->zvol->apno, SW_PUB|SW_PUB3)) {
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


// 消費税イニシャライズ
int CDInputSub::DB_SyIniz( CDBipDataRec* data )
{
	SetCDBData( data );

	// 07.28 /10
	CByteArray	ba;
	ba.Copy( data->m_dsign );

//MyTrace( "*** m_pSy->SyIniz(1) seq = %d [%s]-[%s], zei=<%s>\n", m_pZm->dbdata->seq, 
//						m_pZm->dbdata->dbt, m_pZm->dbdata->cre, m_pZm->dbdata->zei );
//MyTrace( "*** m_pSy->SyIniz(2) dsign = %02x%02x%02x%02x%02x\n",m_pZm->dbdata->dsign[0],
//	m_pZm->dbdata->dsign[1],m_pZm->dbdata->dsign[2],m_pZm->dbdata->dsign[3],m_pZm->dbdata->dsign[4] );

	int st = m_pSy->SyIniz( m_pZm->dbdata );

	// 07.28 /10
	// 税額だけ保存しておく(仕訳書き込み時の差異チェックに引っかかるように)
	if( (ba[0] & 0x40) ) {
		//消費税対象 → 対象外
		if( !(m_pZm->dbdata->dsign[0] & 0x40) ) {
			l_input( data->m_zei, (void*)(LPCTSTR)m_pZm->dbdata->zei );
		}
	}
	else {
		//消費税対象外 → 対象仕訳に変更
		if( (m_pZm->dbdata->dsign[0] & 0x40) ) {
			l_input( data->m_zei, (void*)(LPCTSTR)m_pZm->dbdata->zei );
		}
	}
//MyTrace( "*** m_pSy->SyIniz(3) seq = %d [%s]-[%s], zei=<%s>, st = %d\n", m_pZm->dbdata->seq, 
//						m_pZm->dbdata->dbt, m_pZm->dbdata->cre, m_pZm->dbdata->zei, st );
//MyTrace( "*** m_pSy->SyIniz(4) dsign = %02x%02x%02x%02x%02x\n",m_pZm->dbdata->dsign[0],
//	m_pZm->dbdata->dsign[1],m_pZm->dbdata->dsign[2],m_pZm->dbdata->dsign[3],m_pZm->dbdata->dsign[4] );

	return st;
}


int	CDInputSub::DB_SyIniz(CDBINPDataRec* data)
{
	SetCDBData(data);

	// 07.28 /10
	CByteArray	ba;
	ba.Copy(data->m_dsign);

	//MyTrace( "*** m_pSy->SyIniz(1) seq = %d [%s]-[%s], zei=<%s>\n", m_pZm->dbdata->seq, 
	//						m_pZm->dbdata->dbt, m_pZm->dbdata->cre, m_pZm->dbdata->zei );
	//MyTrace( "*** m_pSy->SyIniz(2) dsign = %02x%02x%02x%02x%02x\n",m_pZm->dbdata->dsign[0],
	//	m_pZm->dbdata->dsign[1],m_pZm->dbdata->dsign[2],m_pZm->dbdata->dsign[3],m_pZm->dbdata->dsign[4] );

	int st = m_pSy->SyIniz(m_pZm->dbdata);

	// 07.28 /10
	// 税額だけ保存しておく(仕訳書き込み時の差異チェックに引っかかるように)
	if( (ba[0] & 0x40) ) {
		//消費税対象 → 対象外
		if( !(m_pZm->dbdata->dsign[0] & 0x40) ) {
			l_input(data->m_zei, (void*)(LPCTSTR)m_pZm->dbdata->zei);
		}
	}
	else {
		//消費税対象外 → 対象仕訳に変更
		if( (m_pZm->dbdata->dsign[0] & 0x40) ) {
			l_input(data->m_zei, (void*)(LPCTSTR)m_pZm->dbdata->zei);
		}
	}
	//MyTrace( "*** m_pSy->SyIniz(3) seq = %d [%s]-[%s], zei=<%s>, st = %d\n", m_pZm->dbdata->seq, 
	//						m_pZm->dbdata->dbt, m_pZm->dbdata->cre, m_pZm->dbdata->zei, st );
	//MyTrace( "*** m_pSy->SyIniz(4) dsign = %02x%02x%02x%02x%02x\n",m_pZm->dbdata->dsign[0],
	//	m_pZm->dbdata->dsign[1],m_pZm->dbdata->dsign[2],m_pZm->dbdata->dsign[3],m_pZm->dbdata->dsign[4] );

	return st;
}



//--------------------------------------------------------------
//	消費税文字列取得
//
//
//--------------------------------------------------------------
LPCTSTR CDInputSub::sy_chk_txt( CDBipDataRec *data, struct _SY_MSG_PACK2* smp )
{
//ASSERT( data->m_seq == m_pZm->dbdata->seq );

	static CString txt;

	struct _SY_MSG_PACK2 smpk, *pSmp;
	pSmp = (smp == NULL) ? &smpk : smp;

	SetCDBData( data );
//#ifdef SYZDSP_CLOSE
	if( m_pSy->SyMsg( pSmp ) == 0) {
		txt = m_pSy->Sy_get_message( m_pZm->dbdata, SYGET_TYPE );
		data->m_dsign.Copy( m_pZm->dbdata->dsign );
	}
	else	txt.Empty();
//#else
//	txt = m_pSy->Sy_get_message( m_pZm->dbdata, SYGET_TYPE );
//	data->m_dsign.Copy( m_pZm->dbdata->dsign );
//#endif

	return txt;
}


//--------------------------------------------------------------
//	消費税文字列取得
//
//
//--------------------------------------------------------------
LPCTSTR CDInputSub::sy_chk_txt(CDBINPDataRec *data, struct _SY_MSG_PACK2* smp)
{
	//ASSERT( data->m_seq == m_pZm->dbdata->seq );

	static CString txt;

	struct _SY_MSG_PACK2 smpk, *pSmp;
	pSmp = (smp == NULL) ? &smpk : smp;

	SetCDBData(data);
	//#ifdef SYZDSP_CLOSE
	if( m_pSy->SyMsg(pSmp) == 0 ) {
		txt = m_pSy->Sy_get_message(m_pZm->dbdata, SYGET_TYPE);
		data->m_dsign.Copy(m_pZm->dbdata->dsign);
	}
	else	txt.Empty();
	//#else
	//	txt = m_pSy->Sy_get_message( m_pZm->dbdata, SYGET_TYPE );
	//	data->m_dsign.Copy( m_pZm->dbdata->dsign );
	//#endif

	return txt;
}



//-------------------------------------------
//	枝番 が登録されているかチェック
//
//	return 99 登録処理
//			0 ゼロ登録した
//			1 枝番なし
//		　 -1 エラー
//-------------------------------------------
int CDInputSub::brchk1( CDBipDataRec *dta, int dbcr_sg )
{
	int		rval;

	rval = 0;

	if( !dbcr_sg )
		rval = bchk_dbr( dta, 1 );
	else
		rval = bchk_cbr( dta, 1 );

	/* 枝番取消	*/
	if( rval && (rval != 99 && rval != 90) )
	{
		del_dtaeda( dta, dbcr_sg );
	}

	return( rval );
}


//-------------------------------------------
//	枝番 が登録されているかチェック
//
//	return 99 登録処理
//			0 ゼロ登録した
//			1 枝番なし
//		　 -1 エラー
//-------------------------------------------
int CDInputSub::brchk1(CDBINPDataRec *dta, int dbcr_sg)
{
	int		rval;

	rval = 0;

	if( !dbcr_sg )
		rval = bchk_dbr(dta, 1);
	else
		rval = bchk_cbr(dta, 1);

	/* 枝番取消	*/
	if( rval && (rval != 99 && rval != 90) )
	{
		del_dtaeda(dta, dbcr_sg);
	}

	return(rval);
}



//
// 仕訳枝番の取り消し
void CDInputSub::del_dtaeda( CDBipDataRec *dta, int dbcr_sg )
{
	if( !dbcr_sg )	{
		if( dta->m_dbr != -1 ) dta->m_dbr = -1;
	}
	else	{
		if( dta->m_cbr != -1 ) dta->m_cbr = -1;
	}
}

//
// 仕訳枝番の取り消し
void CDInputSub::del_dtaeda(CDBINPDataRec *dta, int dbcr_sg)
{
	if( !dbcr_sg ) {
		if( dta->m_dbr != -1 ) dta->m_dbr = -1;
	}
	else {
		if( dta->m_cbr != -1 ) dta->m_cbr = -1;
	}
}

//
// 借方枝番チェック・登録
//
int CDInputSub::bchk_dbr(CDBipDataRec *dta, int msg_sg)
{
	CDBINPDataRec newdata;
	convNewDataRec(dta, &newdata);
	return bchk_dbr(&newdata, msg_sg);
}

int CDInputSub::bchk_dbr(CDBINPDataRec *dta, int msg_sg )
{
	int		rval;
	int		st;

	rval = 0;
	CDBipEZREC	brec;
	
	if( ! codeinp_chk( dta->m_dbt ) && (dta->m_dbr != -1) )
	{
		// 枝番マスターではない
		if( !IsEdaban() ) {
			rval = 1;	// rval == 1 : 枝番マスターではない
			goto	BCHK01;
		}
		/* 既に登録有り	*/
		if( brmsrch( &brec, dta, 0) == 0) {
			goto	BCHK01;
		}
		if( IsConFirmMaster() ) {
			rval = 1;	// 決算確定 → 追加登録不可
			goto	BCHK01;
		}
		/*　追加登録	*/
		if(( st = br_entry( &brec, dta, 0, msg_sg )) == 0 ) {
			// ゼロ登録
			rval = 90;
			IsModify() = TRUE;
			goto BCHK01;
		}
		else if( st == 99 )
		{
			rval = 99;			// 登録タイプ選択を行う
			goto BCHK01;
		}
		else { /* no entry */
			rval = 2;			// エラー
			goto BCHK01;
		}
	}

BCHK01:

	if( rval ) bchk_msg( rval, 0, dta->m_dbmn, dta->m_dbt, dta->m_dbr );
	return( rval );
}


//
// 貸方枝番チェック・登録
//
int CDInputSub::bchk_cbr(CDBipDataRec *dta, int msg_sg)
{
	CDBINPDataRec newdata;
	convNewDataRec(dta, &newdata);
	return bchk_cbr(&newdata, msg_sg);
}

int CDInputSub::bchk_cbr(CDBINPDataRec *dta, int msg_sg )
{
	int		rval;
	int		ST;
	CDBipEZREC	brec2;

	rval = 0;

	if( ! codeinp_chk( dta->m_cre ) && (dta->m_cbr != -1) )
	{
		if( !IsEdaban() ) {
			rval = 1;
			goto	BCHK02;
		}

		/* 既に登録有り	*/
		if( brmsrch( &brec2, dta, 1) == 0 ) {
			goto BCHK02;
		}
		if( IsConFirmMaster() ) {
			rval = 1;	// 決算確定 → 追加登録不可
			goto	BCHK02;
		}
		
		/* 追加登録	*/
//		if( m_pZm->ezrec->GetRecordCount() < Voln1->ed_max ) {
			if( (ST=br_entry( &brec2, dta, 1, msg_sg )) == 0 ) {
				// ゼロ登録
				rval = 90;
				IsModify() = TRUE;
				goto BCHK02;
			}
			else if( ST == 99 )
			{
				rval = 99;			// 登録選択
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
//	dbcr_sg = -1 のときは 借／貸 名称を表示しない
//
//
void CDInputSub::bchk_msg( int rval, int dbcr_sg, int bmn, CString& code, int brn )
{
	char	msg[MAX_PATH], buf[20], bmnbuf[20];
	char*	str;
	int n;

	switch( dbcr_sg ) {
	case 0:	str = "借方";	break;
	case 1:	str = "貸方";	break;
	default:str = "";		break;
	}

	int ed_max = 0;
//	ed_max = m_pZm->m_edmax;

	if( rval == 1 || rval == 3 )
	{
		if( rval == 1 ) { //枝番なしOR決算確定
			if( !IsEdaban() ) {
				n = sprintf_s(msg, sizeof msg, "%s", "枝番マスターではないため枝番は使用できません。");
			}
			else {
				n = sprintf_s(msg, sizeof msg, "%s", "決算確定状態のため、新規の枝番を登録できません。");
			}
		}
//		else if( rval == 3 )
//			n = sprintf_s( msg, sizeof msg, "枝番最大登録数(=%u)を超えて登録しようとしました。", ed_max );

		struct _DBKNREC* pKn;
		pKn = DB_PjisToKnrec( code, m_pZm );
		set_codestr( buf, sizeof buf, brn, m_pZm->zvol->edcol );

#ifdef BMEDA_CLOSE	// 部門枝番の細かいチェックはしない
		if( bmn != -1 ) {
			m_pZm->BumonCodeToStr( bmnbuf, sizeof bmnbuf, bmn );
			sprintf_s( &msg[n], (sizeof msg - n), "\n\n%s部門[%s] 科目「%.14s」の枝番%sはキャンセルされます。",
						str, bmnbuf, pKn->knnam, buf );
		}
		else {
#endif
			sprintf_s( &msg[n], (sizeof msg - n), "\n\n%s科目「%.14s」の枝番%sはキャンセルされます。",
						str, pKn->knnam, buf );
//		}

		ICSMessageBox( msg, 0, 0, 0, m_pParent);
	}
}



//
//枝番入力可能の科目か？
//
BOOL CDInputSub::codeinp_chk( LPCTSTR code )
{
	int st = 0;

/*- '08.11.28 -*/
//	if( Voln1->apno == 0x50 ) {
//		//諸口資金は除く
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


// 枝番残高登録
//		 0 = 登録OK
//		-1 = cancel or error 枝番を取り消す
//
int  CDInputSub::brzan_torok( CWnd* pwnd, int bmn, CString code, int brn, BOOL dbsgn )
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
//	((CMainFrame*)GetDBDinpMainFrame())->PopupCloseState(2);

	st = EdabanTorokDialog( &ed, m_pZm, pwnd, &bQuery );
	if( bQuery ) {
	//	m_pZm->tkrec->Requery( "", 0 );
	//	m_pZm->tkrec_kanamode() = FALSE;
		tkrec_queryAll();
	}

//	((CMainFrame*)GetDBDinpMainFrame())->PopupCloseState(0);

	if( st == IDCANCEL )
		st = -1;
	else if( st == -1 ) {
		char	eda_buf[20];
		char* msgp;
		msgp = dbsgn ? "貸方" : "借方";
		set_codestr( eda_buf, sizeof eda_buf, ed.ed_ecod, m_pZm->zvol->edcol );
		DBKNREC* pKn;
		pKn = DB_PjisToKnrec( code, m_pZm );

		str.Format( "%s「%.14s.%s」（コード=%s.%s）の登録に失敗しました。",
								msgp, pKn->knnam, eda_buf, ed.ed_kcod, eda_buf ); 
		ICSMessageBox( str, 0, 0, 0, m_pParent);
		st = -1;
	}

	if( st != -1 ) {
		IsEdaAdd() = TRUE;
	}
//PROGRAM_OK = TRUE;

	return st;
}



// データベース 枝番マスター 検索
int CDInputSub::DB_EdabanMasterSearch(CDBipDataRec* data, int dbcr_sg)
{
	CDBINPDataRec newdata;
	convNewDataRec(data, &newdata);

	return DB_EdabanMasterSearch(&newdata, dbcr_sg );
}

int CDInputSub::DB_EdabanMasterSearch(CDBINPDataRec* data, int dbcr_sg )
{
	CString filter, buf;

	ezdb_init( m_pZm );
	if( ! dbcr_sg ) {
		if( data->m_dbr == -1 )
			return -1;
//		filter.Format( "ezkcd = '%s' AND ezecd = %d", data->m_dbt, data->m_dbr );
		if( m_pZm->ezrec->Find( (LPCTSTR)data->m_dbt, data->m_dbr ) > 0 ) return 0;
	}
	else {
		if( data->m_cbr == -1 )
			return -1;
//		filter.Format( "ezkcd = '%s' AND ezecd = %d", data->m_cre, data->m_cbr );
		if( m_pZm->ezrec->Find( (LPCTSTR)data->m_cre, data->m_cbr ) > 0 ) return 0;
	}

/*	if( ezrec->Requery( filter, 1 ) == 0 ) {
		return 0;
	}*/

	return -1;
}


// 枝番マスターサーチ
//		 0 = found
//		-1 = not found
//
int CDInputSub::brmsrch(CDBipEZREC *brec, CDBipDataRec *data, int dbcr_sg)
{
	CDBINPDataRec newdata;
	convNewDataRec(data, &newdata);

	return brmsrch(brec, &newdata, dbcr_sg);
}

int CDInputSub::brmsrch( CDBipEZREC *brec, CDBINPDataRec *data, int dbcr_sg)
{
	//借方
	if ( dbcr_sg == 0) {

		if( DB_EdabanMasterSearch( data, 0 ) != 0 )
			return -1;

#ifdef BMEDA_CLOSE
		if( data->m_dbmn != -1 ) {
			if( DB_BmnEdabanMasterSearch( data, 0 ) != 0 )
				return -1;
		}
		else {
			if( DB_EdabanMasterSearch( data, 0 ) != 0 )
				return -1;
		}
#endif
	}
	//貸方
	else {

		if( DB_EdabanMasterSearch( data, 1 ) != 0 )
			return -1;

#ifdef BMEDA_CLOSE
		if( data->m_cbmn != -1 ) {
			if( DB_BmnEdabanMasterSearch( data, 1 ) != 0 )
				return -1;
		}
		else {
			if( DB_EdabanMasterSearch( data, 1 ) != 0 )
				return -1;
		}
#endif
	}

	return 0;	// Found
}



//=================================================
//	枝番マスターへ登録
//	CDBipEZREC *brec ------- 枝番バッファ
//	CDBipDataRec *data ----- 仕訳レコード
//	int dbcr_sg ----- D7 ON で 借／貸文字列を表示しない
//					  D0-D3 0 = 借方, 1 = 貸方
//	int msg_sg	
//
//	return 99 登録処理
//			0 ゼロ登録した
//			1 枝番なし
//		　 -1 エラー
//=================================================
int CDInputSub::br_entry( CDBipEZREC *brec, CDBipDataRec *data, int dbcr_sg, int msg_sg )
{
	int	st,n;
	char	*msgp;
	char	pbuf[300], edabuf[20], bmnbuf[24];;
	CString ezkm_code, kmkcode;

	brec->Reset();

	int dc_sgn;
	dc_sgn = (dbcr_sg&0x0f);

	if( dc_sgn == 0 ) {
		if( ! (dbcr_sg & 0x80) )	msgp = "借方"; 
		else						msgp = "";
		brec->m_ezbmn = data->m_dbmn;
		brec->m_ezkcd = data->m_dbt;
		brec->m_ezecd = data->m_dbr;
	}
	else if( dc_sgn == 1 )	{
		if( ! (dbcr_sg & 0x80) )	msgp = "貸方"; 
		else						msgp = "";
		brec->m_ezbmn = data->m_cbmn;
		brec->m_ezkcd = data->m_cre;
		brec->m_ezecd = data->m_cbr;
	}

	if( msg_sg )
	{
		struct _DBKNREC	*pKn;
		pKn = DB_PjisToKnrec( brec->m_ezkcd, m_pZm );
		set_codestr( edabuf, sizeof edabuf, brec->m_ezecd, m_pZm->zvol->edcol );

#ifdef BMEDA_CLOSE
		if( brec->m_ezbmn != -1 ) {
			BumonCodeToStr( bmnbuf, sizeof bmnbuf, brec->m_ezbmn );
			n = sprintf_s( pbuf, sizeof pbuf, "《部門枝番未登録》\n\n%s部門[%s] 科目「%.14s」の枝番%sは登録されていません。　登録しますか？",
							msgp, bmnbuf, pKn->knnam, edabuf );
			sprintf_s( &pbuf[n], (sizeof pbuf - n), "\n\n「いいえ」の場合、%s部門[%s] 科目「%.14s」の枝番%sはキャンセルされます。",
							msgp, bmnbuf, pKn->knnam, edabuf );
		}
		else {
#endif
			n = sprintf_s( pbuf, sizeof pbuf, "《枝番未登録》\n\n%s科目「%.14s」の枝番%sは登録されていません。　登録しますか？",
									msgp, pKn->knnam, edabuf );
			sprintf_s( &pbuf[n], (sizeof pbuf - n), "\n\n「いいえ」の場合、%s科目「%.14s」の枝番%sはキャンセルされます。",
				msgp, pKn->knnam, edabuf );
//		}


		CString title;
//		title.LoadString(AFX_IDS_APP_TITLE);
		CFrameWnd* pfrm = m_pParent->GetParentFrame();
		pfrm->GetWindowText(title);
		st = ICSExtBox( title, pbuf, _T( "はい(&Y)" ), _T( "ゼロ登録(&Z)" ), _T( "いいえ(&N)" ), MB_DEFBUTTON1, 0, 0, m_pParent );	

		if( st == /*IDOK*/ IDYES )
		{

			DWORD tmp;
			tmp = pKn->kncod;
			hl_rev( &tmp, 4 );
			kmkcode.Format( "%08x", tmp );
			kmkcode.TrimRight();
			if( kmkcode != brec->m_ezkcd ) {
				sprintf_s( pbuf, sizeof pbuf, "%s「%.14s」の科目コードは登録されていません。", msgp, pKn->knnam );
				ICSMessageBox( pbuf, 0, 0, 0, m_pParent);
				return 1;
			}

			return 99;		// 登録選択

		}
		else if( st == IDNO ) {
			// ゼロ登録 は 下の zroeda_torok(関数で)
		
		}
		else
		{
			return 1;
		}

	}
	return zroeda_torok( data, dbcr_sg );	// 単なる登録
}

//---------------------------------------------------------------
// 単なる登録
//	int dbcr_sg ----- D7 ON で 借／貸文字列を表示しない
//					  D0-D3 0 = 借方, 1 = 貸方
//  -> 0 ... ok  -1 ... 失敗
//---------------------------------------------------------------
int CDInputSub::zroeda_torok(CDBipDataRec *data, int dbcr_sg)
{
	int	att, bmn;

	char	*msgp;
	char	pbuf[300];
	char	eda_buf[20];
	CDBipEZREC brec;

	int dc_sgn;
	dc_sgn = (dbcr_sg&0x0f);

	if( dc_sgn == 0 )
	{
		if( ! (dbcr_sg & 0x80) )	msgp = "借方"; 
		else						msgp = "";

		brec.Reset();
		brec.m_ezkcd = data->m_dbt;
		brec.m_ezecd = data->m_dbr;
		bmn = data->m_dbmn;
	}
	else
	{
		if( ! (dbcr_sg & 0x80) )	msgp = "貸方"; 
		else						msgp = "";

		brec.Reset();
		brec.m_ezkcd = data->m_cre;
		brec.m_ezecd = data->m_cbr;
		bmn = data->m_cbmn;
	}

	// 科目名称リード
	struct _DBKNREC* pKn;
	pKn = DB_PjisToKnrec( brec.m_ezkcd, m_pZm );
	set_codestr( eda_buf, sizeof eda_buf, brec.m_ezecd, m_pZm->zvol->edcol );

	att = pKn->knatt;
	brec.m_ezatt |= (att & 0x01); 
	brec.m_ezatt &= 0x7f;
	memcpy(brec.m_ezsysgn, pKn->knsgn, sizeof brec.m_ezsysgn);

	// DB に枝番残を登録

#ifdef BMEDA_CLOSE
	if( bmn == -1 ) {
#endif
		if( EdabanZanTorok( &brec ) ) {
			sprintf_s( pbuf, sizeof pbuf, "%s「%.14s.%s」（コード=%s.%s）の登録に失敗しました。",
									msgp, pKn->knnam, eda_buf, (LPCTSTR)brec.m_ezkcd, eda_buf ); 
			ICSMessageBox( pbuf, 0, 0, 0, m_pParent);
			del_dtaeda( data, dbcr_sg );	//仕訳枝番取り消し
			return(-1);
		}
		else {
			if( bmn != -1 && (m_pZm->zvol->bm_isw & 0x02) ) {
				//部門枝番自動登録ON
				if( BmnEdabanZanTorok( &brec, bmn ) ) {
					sprintf_s( pbuf, sizeof pbuf, "%s 部門[%d]「%.14s.%s」（コード=%s.%s）の登録に失敗しました。",
											msgp, bmn, pKn->knnam, eda_buf, (LPCTSTR)brec.m_ezkcd, eda_buf ); 
					ICSMessageBox( pbuf, 0, 0, 0, m_pParent);
					del_dtaeda( data, dbcr_sg );	//仕訳枝番取り消し
					return(-1);
				}	
			}
		}

#ifdef BMEDA_CLOSE
	}
	else {
		if( m_pZm->BmnEdabanZanTorok( brec, bmn ) ) {
			sprintf_s( pbuf, sizeof pbuf, "%s 部門[%d]「%.14s.%s」（コード=%s.%s）の登録に失敗しました。",
									msgp, bmn, pKn->knnam, eda_buf, brec->m_ezkcd, eda_buf ); 
			ICSMessageBox( pbuf, 0, 0, 0, m_pParent);
			del_dtaeda( data, dbcr_sg );	//仕訳枝番取り消し
			return(-1);
		}	
	}
#endif

	return(0);

}


//=================================================
//	枝番マスターへ登録
//	CDBipEZREC *brec ------- 枝番バッファ
//	CDBipDataRec *data ----- 仕訳レコード
//	int dbcr_sg ----- D7 ON で 借／貸文字列を表示しない
//					  D0-D3 0 = 借方, 1 = 貸方
//	int msg_sg	
//
//	return 99 登録処理
//			0 ゼロ登録した
//			1 枝番なし
//		　 -1 エラー
//=================================================
int CDInputSub::br_entry(CDBipEZREC *brec, CDBINPDataRec *data, int dbcr_sg, int msg_sg)
{
	int	st, n;
	char	*msgp;
	char	pbuf[300], edabuf[20], bmnbuf[24];;
	CString ezkm_code, kmkcode;

	brec->Reset();

	int dc_sgn;
	dc_sgn = (dbcr_sg & 0x0f);

	if( dc_sgn == 0 ) {
		if( !(dbcr_sg & 0x80) )	msgp = "借方";
		else						msgp = "";
		brec->m_ezbmn = data->m_dbmn;
		brec->m_ezkcd = data->m_dbt;
		brec->m_ezecd = data->m_dbr;
	}
	else if( dc_sgn == 1 ) {
		if( !(dbcr_sg & 0x80) )	msgp = "貸方";
		else						msgp = "";
		brec->m_ezbmn = data->m_cbmn;
		brec->m_ezkcd = data->m_cre;
		brec->m_ezecd = data->m_cbr;
	}

	if( msg_sg )
	{
		struct _DBKNREC	*pKn;
		pKn = DB_PjisToKnrec(brec->m_ezkcd, m_pZm);
		set_codestr(edabuf, sizeof edabuf, brec->m_ezecd, m_pZm->zvol->edcol);

#ifdef BMEDA_CLOSE
		if( brec->m_ezbmn != -1 ) {
			BumonCodeToStr(bmnbuf, sizeof bmnbuf, brec->m_ezbmn);
			n = sprintf_s(pbuf, sizeof pbuf, "《部門枝番未登録》\n\n%s部門[%s] 科目「%.14s」の枝番%sは登録されていません。　登録しますか？",
				msgp, bmnbuf, pKn->knnam, edabuf);
			sprintf_s(&pbuf[n], (sizeof pbuf - n), "\n\n「いいえ」の場合、%s部門[%s] 科目「%.14s」の枝番%sはキャンセルされます。",
				msgp, bmnbuf, pKn->knnam, edabuf);
		}
		else {
#endif
			n = sprintf_s(pbuf, sizeof pbuf, "《枝番未登録》\n\n%s科目「%.14s」の枝番%sは登録されていません。　登録しますか？",
				msgp, pKn->knnam, edabuf);
			sprintf_s(&pbuf[n], (sizeof pbuf - n), "\n\n「いいえ」の場合、%s科目「%.14s」の枝番%sはキャンセルされます。",
				msgp, pKn->knnam, edabuf);
			//		}


			CString title;
			//		title.LoadString(AFX_IDS_APP_TITLE);
			CFrameWnd* pfrm = m_pParent->GetParentFrame();
			pfrm->GetWindowText(title);
			st = ICSExtBox(title, pbuf, _T("はい(&Y)"), _T("ゼロ登録(&Z)"), _T("いいえ(&N)"), MB_DEFBUTTON1, 0, 0, m_pParent);

			if( st == /*IDOK*/ IDYES )
			{

				DWORD tmp;
				tmp = pKn->kncod;
				hl_rev(&tmp, 4);
				kmkcode.Format("%08x", tmp);
				kmkcode.TrimRight();
				if( kmkcode != brec->m_ezkcd ) {
					sprintf_s(pbuf, sizeof pbuf, "%s「%.14s」の科目コードは登録されていません。", msgp, pKn->knnam);
					ICSMessageBox(pbuf, 0, 0, 0, m_pParent);
					return 1;
				}

				return 99;		// 登録選択

			}
			else if( st == IDNO ) {
				// ゼロ登録 は 下の zroeda_torok(関数で)

			}
			else
			{
				return 1;
			}

		}
		return zroeda_torok(data, dbcr_sg);	// 単なる登録
	}

	//---------------------------------------------------------------
	// 単なる登録
	//	int dbcr_sg ----- D7 ON で 借／貸文字列を表示しない
	//					  D0-D3 0 = 借方, 1 = 貸方
	//  -> 0 ... ok  -1 ... 失敗
	//---------------------------------------------------------------
	int CDInputSub::zroeda_torok(CDBINPDataRec *data, int dbcr_sg)
	{
		int	att, bmn;

		char	*msgp;
		char	pbuf[300];
		char	eda_buf[20];
		CDBipEZREC brec;

		int dc_sgn;
		dc_sgn = (dbcr_sg & 0x0f);

		if( dc_sgn == 0 )
		{
			if( !(dbcr_sg & 0x80) )	msgp = "借方";
			else						msgp = "";

			brec.Reset();
			brec.m_ezkcd = data->m_dbt;
			brec.m_ezecd = data->m_dbr;
			bmn = data->m_dbmn;
		}
		else
		{
			if( !(dbcr_sg & 0x80) )	msgp = "貸方";
			else						msgp = "";

			brec.Reset();
			brec.m_ezkcd = data->m_cre;
			brec.m_ezecd = data->m_cbr;
			bmn = data->m_cbmn;
		}

		// 科目名称リード
		struct _DBKNREC* pKn;
		pKn = DB_PjisToKnrec(brec.m_ezkcd, m_pZm);
		set_codestr(eda_buf, sizeof eda_buf, brec.m_ezecd, m_pZm->zvol->edcol);

		att = pKn->knatt;
		brec.m_ezatt |= (att & 0x01);
		brec.m_ezatt &= 0x7f;
		memcpy(brec.m_ezsysgn, pKn->knsgn, sizeof brec.m_ezsysgn);

		// DB に枝番残を登録

#ifdef BMEDA_CLOSE
		if( bmn == -1 ) {
#endif
			if( EdabanZanTorok(&brec) ) {
				sprintf_s(pbuf, sizeof pbuf, "%s「%.14s.%s」（コード=%s.%s）の登録に失敗しました。",
					msgp, pKn->knnam, eda_buf, (LPCTSTR)brec.m_ezkcd, eda_buf);
				ICSMessageBox(pbuf, 0, 0, 0, m_pParent);
				del_dtaeda(data, dbcr_sg);	//仕訳枝番取り消し
				return(-1);
			}
			else {
				if( bmn != -1 && (m_pZm->zvol->bm_isw & 0x02) ) {
					//部門枝番自動登録ON
					if( BmnEdabanZanTorok(&brec, bmn) ) {
						sprintf_s(pbuf, sizeof pbuf, "%s 部門[%d]「%.14s.%s」（コード=%s.%s）の登録に失敗しました。",
							msgp, bmn, pKn->knnam, eda_buf, (LPCTSTR)brec.m_ezkcd, eda_buf);
						ICSMessageBox(pbuf, 0, 0, 0, m_pParent);
						del_dtaeda(data, dbcr_sg);	//仕訳枝番取り消し
						return(-1);
					}
				}
			}

#ifdef BMEDA_CLOSE
		}
		else {
			if( m_pZm->BmnEdabanZanTorok(brec, bmn) ) {
				sprintf_s(pbuf, sizeof pbuf, "%s 部門[%d]「%.14s.%s」（コード=%s.%s）の登録に失敗しました。",
					msgp, bmn, pKn->knnam, eda_buf, brec->m_ezkcd, eda_buf);
				ICSMessageBox(pbuf, 0, 0, 0, m_pParent);
				del_dtaeda(data, dbcr_sg);	//仕訳枝番取り消し
				return(-1);
			}
		}
#endif

		return(0);

	}


// 枝番残高登録
int CDInputSub::EdabanZanTorok( CDBipEZREC* ez )
{
	CString filter;

	filter.Format( "ezkcd = '%s' AND ezecd = %d", ez->m_ezkcd, ez->m_ezecd );

	if( m_pZm->ezrec->Requery( filter, 1 ) != 0 ) {

		m_pZm->ezrec->AddNew();
		// 先に追加だけしておく
		m_pZm->ezrec->ezkcd	= ez->m_ezkcd;
		m_pZm->ezrec->ezecd	= ez->m_ezecd;
		m_pZm->ezrec->eztcd	= ez->m_eztcd;
		m_pZm->ezrec->ezatt	= ez->m_ezatt;
		memcpy(&m_pZm->ezrec->ezsysgn[2], &ez->m_ezsysgn[2], 4);
		m_pZm->ezrec->ezsysgn[5] = 0;

		m_pZm->ezrec->Update();
	//	m_pZm->ezrec->Requery("", 1);

		IsEdaAdd() = TRUE;

		if ((m_pZm->zvol->s_sgn2 & 0x10)) {
			m_pZm->ezrec->Requery("", 1);
			ezquery_flag() = FALSE;
		}
	}

	return 0;
}


// 部門枝番残高登録
int CDInputSub::BmnEdabanZanTorok( CDBipEZREC* ez, int bmn )
{
	CString filter;

	filter.Format( "ezbmn = %d AND ezkcd = '%s' AND ezecd = %d", bmn, ez->m_ezkcd, ez->m_ezecd );

	if( m_pZm->bmezrec->Requery( filter, 1 ) != 0 ) {

		m_pZm->bmezrec->AddNew();
		// 先に追加だけしておく
		m_pZm->bmezrec->ezbmn	= bmn;
		m_pZm->bmezrec->ezkcd	= ez->m_ezkcd;
		m_pZm->bmezrec->ezecd	= ez->m_ezecd;
		m_pZm->bmezrec->eztcd	= ez->m_eztcd;
		m_pZm->bmezrec->ezatt	= ez->m_ezatt;
	
		m_pZm->bmezrec->Update();
	}

	return 0;
}


// 枝番残高検索
int CDInputSub::DB_EdabanZanSearch( CString& kmkcode, int brn )
{
//	CString filter;

	ezdb_init( m_pZm );

	if( brn != -1 ) {
//		filter.Format( "ezkcd = '%s' AND ezecd = %d", kmkcode, brn );
		if( m_pZm->ezrec->Find( (LPCTSTR)kmkcode, brn ) > 0 ) return 0;
	}
	else {
//		filter.Format( "ezkcd = '%s'", kmkcode );
		if( m_pZm->ezrec->Find( (LPCTSTR)kmkcode ) > 0 ) return 0;
	}

/*	if( ezrec->Requery( filter, 1 ) == 0 ) {
		return 0;
	}*/

	return -1;
}


// 枝番残高検索
int CDInputSub::DB_EdabanZanSearch2( CString& kmkcode, int tkcod )
{
//	CString filter;
	ezdb_init( m_pZm );

	if( tkcod != -1 ) {
//		filter.Format( "ezkcd = '%s' AND eztcd = %d", kmkcode, tkcod );
		if( m_pZm->ezrec->Find2( (LPCTSTR)kmkcode, tkcod ) > 0 ) return 0;
	}
	else {
//		filter.Format( "ezkcd = '%s'", kmkcode );
		if( m_pZm->ezrec->Find( (LPCTSTR)kmkcode ) > 0 ) return 0;
	}

/*	if( ezrec->Requery( filter, 1 ) == 0 ) {
		return 0;
	}*/

	return -1;
}



//摘要カナ検索をした後の、摘要全部をクエリーする。
void CDInputSub::tkrec_queryAll()
{
	m_pZm->tkrec->Requery( "", 0 );
	tkrec_kanamode() = FALSE;
	m_BrTek.set_tkquery( FALSE );
}


//-------------------------------------------------------------------
//　追加名称を取得
//	return TRUE ... 追加名称あり, FALSE ... 追加名称なし
//
//-------------------------------------------------------------------
BOOL CDInputSub::_set_kmketc( struct _SET_KMKETC* psk, BOOL full/*=TRUE*/ )
{
	BOOL bRet = FALSE;	
	BRN_NAME *p;

#ifdef LATER_CLOSE
	// 摘要かな検索 をしていたら、元に戻す
	if( m_pZm->tkrec_kanamode() ) {
	//	m_pZm->tkrec->Requery( "", 0 );
	//	m_pZm->tkrec_kanamode() = FALSE;
		tkrec_queryAll();
	}
#endif

	int bmncd = -1;
	if( m_pAsel->BMNTKBR_OPT == OPT_CHKON )	bmncd = psk->bmn;

	int mode = 0x03;
	BOOL sw1, sw2;
	sw1 = (m_pAsel->BRTEK_OPT == OPT_DSP);
	sw2 = (m_pAsel->SYGBRN_OPT == OPT_DSP);

	if( sw1 && !sw2 )
		mode = BT_NOMALBRN_TEK;	// 枝番
	else if( !sw1 && sw2 )
		mode = BT_SYOGTBRN_TEK;	// 諸口枝番
	else if( sw1 && sw2 )
		BT_NOMALBRN_TEK|BT_SYOGTBRN_TEK;	// 枝番＋諸口枝番

	if( p = m_BrTek.get_brtekmsg( bmncd/*psk->bmn*/, psk->code, psk->brn, full, mode ) )
	{
		bRet = TRUE;
		psk->etc.Format( "%s", p->Name );
		psk->etc_col = p->ForeColor;
	}
//MyTrace( "@_set_kmketc code = %s, brn = %06d, etc = %s\n",  psk->code, psk->brn, psk->etc );

	return bRet;
}


int CDInputSub::get_brtek(int bmn, CString& code, int brn)
{
	int tno = m_BrTek.get_brtek(bmn, code, brn);
	return tno;
}


// データベース 摘要読み込み
int CDInputSub::DB_TekiyoNoRead( CDBipTKREC* tek_rec, int no )
{
	BOOL bRet = FALSE;
	if( no < 1 )	return -1;

//	tkrec->SetAbsolutePosition( no );
	int tkyo_len = TKYO_LEN;
	if( ! IsMasterType(MST_TKY20) )	tkyo_len = (TKYO_LEN/2);

	if( tkrec_kanamode() ) {
		m_pZm->tkrec->Requery( "", 0 );
		tkrec_kanamode() = FALSE;
	}
	if( m_pZm->tkrec->MoveFirst() == 0 ) {
		do {
			if( no == m_pZm->tkrec->tkcod ) {
				tek_rec->m_tkcod = m_pZm->tkrec->tkcod;
				tek_rec->m_tkana = m_pZm->tkrec->tkana;
				tek_rec->m_tksgn.Copy( m_pZm->tkrec->tksgn );
				tek_rec->m_tkname = m_pZm->tkrec->tkname;
				// 後ろスペースをカット
				LPSTR	p = tek_rec->m_tkname.GetBuffer(256);
				int n = kjlen( p, tkyo_len );
				*(p + n) = '\0';
				tek_rec->m_tkname.ReleaseBuffer();

				tek_rec->m_tkari = m_pZm->tkrec->tkari;
				tek_rec->m_tkasi = m_pZm->tkrec->tkasi;

				// インボイス対応
				int szdsign = m_pZm->tkrec->dsign.GetCount();
				for( int i = 0; i < SIZE_DSINE; i++ ) {
					if( i < szdsign ) {
						tek_rec->m_tkdsgn[i] = m_pZm->tkrec->dsign[i];
					}
					else {
						tek_rec->m_tkdsgn[i] = 0;
					}
				}
				CString invno = m_pZm->tkrec->invno;
				invno.Replace( _T( "T" ), _T( "" ));
				sprintf_s( tek_rec->m_tkinvno, sizeof( tek_rec->m_tkinvno ), _T( "%s" ), invno );

				bRet = TRUE;
				break;
			}
		} while( m_pZm->tkrec->MoveNext() == 0 );
	}

	return bRet ? 0 : -1;
}



//---------------------------------------------------------------
//	引数で指定したタイプの該当のマスターか？
//	 DWORD typbit: タイプビット
//
//---------------------------------------------------------------
BOOL CDInputSub::IsMasterType( DWORD typbit )
{
	return (dwMST_TYPE & typbit);
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
int CDInputSub::tbrasrch( CDBipDataRec *data, int tkcd, char *dbr_inptype, char *cbr_inptype )
{
int ret = 0;
CDBipDataRec sv;
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
	if( m_pAsel->BMNTKBR_OPT == OPT_CHKON )
		bBmn = TRUE;

	DB_TekiyoEdabanSet( data, tkcd, bBmn );

	if( *dbr_inptype != 1 )
		if( data->m_dbr != -1 && data->m_dbr != sv.m_dbr )
		{
			*dbr_inptype = 2;	// 自動
			ret |= 0x01;	//借方にセット
		}
		else
			data->m_dbr = sv_dbr;

	if( *cbr_inptype != 1 )
		if( data->m_cbr != -1 && data->m_cbr != sv.m_cbr )
		{
			*cbr_inptype = 2;	// 自動
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
int CDInputSub::tbrasrch(CDBINPDataRec *data, int tkcd, char *dbr_inptype, char *cbr_inptype)
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
	if( m_pAsel->BMNTKBR_OPT == OPT_CHKON )
		bBmn = TRUE;

	DB_TekiyoEdabanSet(data, tkcd, bBmn);

	if( *dbr_inptype != 1 )
		if( data->m_dbr != -1 && data->m_dbr != sv.m_dbr )
		{
			*dbr_inptype = 2;	// 自動
			ret |= 0x01;	//借方にセット
		}
		else
			data->m_dbr = sv_dbr;

	if( *cbr_inptype != 1 )
		if( data->m_cbr != -1 && data->m_cbr != sv.m_cbr )
		{
			*cbr_inptype = 2;	// 自動
			ret |= 0x02;	//貸方にセット
		}
		else
			data->m_cbr = sv_cbr;

	return ret;
}



// 摘要コードからデータレコードに摘要枝番をセット
int CDInputSub::DB_TekiyoEdabanSet( CDBipDataRec *data, int tkcode, BOOL bBmn )
{
	if( ! bBmn ) {
		for( int n = 0; n < 2; n++ ) {

			if( n == 0 ) {
				if( DB_EdabanZanSearch2( data->m_dbt, tkcode ) == 0 ) {
					if( data->m_dbr == -1 )
						data->m_dbr =	m_pZm->ezrec->ezecd;
					if( data->m_dbt == data->m_cre ) {
						if( data->m_cbr == -1 )
							data->m_cbr = m_pZm->ezrec->ezecd;
						break;
					}
				}
			}
			else {
				if( DB_EdabanZanSearch2( data->m_cre, tkcode ) == 0 ) {
					if( data->m_cbr == -1 )
						data->m_cbr = m_pZm->ezrec->ezecd;
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
							data->m_dbr = m_pZm->bmezrec->ezecd;
						if( data->m_dbt == data->m_cre && data->m_dbmn == data->m_cbmn ) {
							if( data->m_cbr == -1 )
								data->m_cbr = m_pZm->bmezrec->ezecd;
							break;
						}
					}
				}
				else {
					if( DB_EdabanZanSearch2( data->m_dbt, tkcode ) == 0 ) {
						if( data->m_dbr == -1 )
							data->m_dbr = m_pZm->ezrec->ezecd;
						if( data->m_dbt == data->m_cre && data->m_dbmn == data->m_cbmn ) {
							if( data->m_cbr == -1 )
								data->m_cbr = m_pZm->ezrec->ezecd;
							break;
						}
					}
				}
			}
			else {
				if( data->m_cbmn != -1 ) {
					if( DB_BmnEdabanMasterSearch( data, 1, tkcode ) == 0 ) {
						if( data->m_cbr == -1 )
							data->m_cbr = m_pZm->bmezrec->ezecd;
					}
				}
				else {
					if( DB_EdabanZanSearch2( data->m_cre, tkcode ) == 0 ) {
						if( data->m_cbr == -1 )
							data->m_cbr = m_pZm->ezrec->ezecd;
					}
				}
			}
		}
	}

	return 0;
}


// 摘要コードからデータレコードに摘要枝番をセット
int CDInputSub::DB_TekiyoEdabanSet(CDBINPDataRec *data, int tkcode, BOOL bBmn)
{
	if( !bBmn ) {
		for( int n = 0; n < 2; n++ ) {

			if( n == 0 ) {
				if( DB_EdabanZanSearch2(data->m_dbt, tkcode) == 0 ) {
					if( data->m_dbr == -1 )
						data->m_dbr = m_pZm->ezrec->ezecd;
					if( data->m_dbt == data->m_cre ) {
						if( data->m_cbr == -1 )
							data->m_cbr = m_pZm->ezrec->ezecd;
						break;
					}
				}
			}
			else {
				if( DB_EdabanZanSearch2(data->m_cre, tkcode) == 0 ) {
					if( data->m_cbr == -1 )
						data->m_cbr = m_pZm->ezrec->ezecd;
				}
			}
		}
	}
	else {
		//部門枝番
		for( int n = 0; n < 2; n++ ) {
			if( n == 0 ) {

				if( data->m_dbmn != -1 ) {
					if( DB_BmnEdabanMasterSearch(data, 0, tkcode) == 0 ) {
						if( data->m_dbr == -1 )
							data->m_dbr = m_pZm->bmezrec->ezecd;
						if( data->m_dbt == data->m_cre && data->m_dbmn == data->m_cbmn ) {
							if( data->m_cbr == -1 )
								data->m_cbr = m_pZm->bmezrec->ezecd;
							break;
						}
					}
				}
				else {
					if( DB_EdabanZanSearch2(data->m_dbt, tkcode) == 0 ) {
						if( data->m_dbr == -1 )
							data->m_dbr = m_pZm->ezrec->ezecd;
						if( data->m_dbt == data->m_cre && data->m_dbmn == data->m_cbmn ) {
							if( data->m_cbr == -1 )
								data->m_cbr = m_pZm->ezrec->ezecd;
							break;
						}
					}
				}
			}
			else {
				if( data->m_cbmn != -1 ) {
					if( DB_BmnEdabanMasterSearch(data, 1, tkcode) == 0 ) {
						if( data->m_cbr == -1 )
							data->m_cbr = m_pZm->bmezrec->ezecd;
					}
				}
				else {
					if( DB_EdabanZanSearch2(data->m_cre, tkcode) == 0 ) {
						if( data->m_cbr == -1 )
							data->m_cbr = m_pZm->ezrec->ezecd;
					}
				}
			}
		}
	}

	return 0;
}


// データベース 部門枝番マスター 検索
int CDInputSub::DB_BmnEdabanMasterSearch(CDBipDataRec* data, int dbcr_sg)
{
	CDBINPDataRec newdata;
	convNewDataRec(data, &newdata);

	return DB_BmnEdabanMasterSearch(&newdata, dbcr_sg);
}

int CDInputSub::DB_BmnEdabanMasterSearch(CDBINPDataRec* data, int dbcr_sg )
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

	if( m_pZm->bmezrec->Requery( filter, 1 ) == 0 ) {
		return 0;
	}

	return -1;
}


// データベース 部門枝番マスター 検索( 摘要コード )
int CDInputSub::DB_BmnEdabanMasterSearch(CDBipDataRec* data, int dbcr_sg, int tkcod)
{
	CDBINPDataRec newdata;
	convNewDataRec(data, &newdata);

	return DB_BmnEdabanMasterSearch(&newdata, dbcr_sg, tkcod);
}
int CDInputSub::DB_BmnEdabanMasterSearch(CDBINPDataRec* data, int dbcr_sg, int tkcod )
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

	if( m_pZm->bmezrec->Requery( filter, 1 ) == 0 ) {
		return 0;
	}

	return -1;
}



//電子帳簿保存マスターでの仕訳挿入チェック
BOOL CDInputSub::IsInsertMaster()
{
	BOOL bRet = TRUE;

	if( m_pZm->zvol->e_save ) {
		ICSMessageBox("電子帳簿保存マスターでは、仕訳の挿入はできません！", 0, 0, 0, m_pParent);
		bRet = FALSE;
	}

	return bRet;
}



//	確定仕訳のモードを取得
//		0:確定仕訳修正不可 
//		1:確定仕訳修正可能
//
int CDInputSub::Confirm_Mode()
{
	if( ! (m_pZm->zvol->tl_cor & 0x10) )	// マスター全体確定 or 参照モード
		return 0;
	else if( ! IsModifyConfirmData() )	// 確定仕訳 修正不可
		return 0;
	else								// 確定仕訳 修正可
		return 1;
}


// 転送仕訳の 背景色取得
COLORREF CDInputSub::GetConfirmBkColor()
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
BOOL CDInputSub::CheckConfirmBkColor( COLORREF back )
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
BOOL CDInputSub::IsModifyConfirmData()
{
	BOOL bRet = FALSE;
	int st;

	if( (m_pZm->zvol->tl_cor & 0x01) ) {
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


//クラウド共有＋上手くん環境か？
BOOL CDInputSub::IsJozuEnvCloudMaster()
{
	BOOL ret;

	//処理を高速化するため、関数呼び出し数を制限する
	if( JozuEnvCloud == -1 ) {
		CZmGen8		ZmGen;
		JozuEnvCloud = 0;
		if( ZmGen.IsJzCloudMaster(m_pZm) ) {
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


// 変動事由・振替科目かの 名称を取得
//
//	strtype		0:	標準名称
//				1:	短縮名称
//				2:	長い名称
//
LPCTSTR CDInputSub::get_hjiyutitle( int strtype )
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
		if( m_pZm->zvol->apno < 0x50 ) {
			p = l_t1;
		}
		else {
			if( IsSyafuKaisei( m_pZm->zvol ) ) {
				p = l_t3;
			}
			else if( IsGakkouH27Master( m_pZm ) ) {
				p = l_t4;
			}
			else {
				p = l_t2;
			}
		}
	}
	else {
		if( m_pZm->zvol->apno < 0x50 ) {
			p = strtype ? s_t1 : t1;
		}
		else {
			if( IsSyafuKaisei( m_pZm->zvol ) ) {
				p = strtype ? s_t3 : t3;
			}
			else if( IsGakkouH27Master( m_pZm ) ) {
				p = strtype ? s_t4 : t4;;
			}
			else {
				p = strtype ? s_t2 : t2;
			}
		}
	}

	return p;
}



// 合計部門かどうか？
BOOL CDInputSub::IsGokeiBumon( int bmn )
{
	if( ! m_pZm->bmname )	return FALSE;

	BOOL bGk = FALSE;

	//社会福祉改正マスターは合計部門入力可
	if( IsSyafuKaisei( m_pZm->zvol ) && m_bNPanbn ) {
		return FALSE;
	}

	m_pZm->bmname->MoveFirst();

	do {
		if( m_pZm->bmname->st == 0 ) {
			if( m_pZm->bmname->bnbcd == bmn ) {
				if( (m_pZm->bmname->bnhsg[0] & 0x02) ) bGk = TRUE;
				
				break;
			}
		}
		else	break;
	} while ( m_pZm->bmname->MoveNext() == 0 ) ;

	return bGk;
}



//------------------------------------------------------------------------
//	部門名称を取得
//	struct _SET_KMKETC *sk
//				
//------------------------------------------------------------------------
BOOL  CDInputSub::_set_bmnetc( struct _SET_KMKETC *sk )
{
	BOOL ret = FALSE;

	if( ! m_pZm->bmname )
		return FALSE;

	m_pZm->bmname->MoveFirst();
	if (m_pZm->bmname->st == -1)
		return FALSE;

	if (sk->bmn == -1)	return FALSE;

	do {
		if( sk->bmn == m_pZm->bmname->bnbcd ) {
			//社会福祉 按分あり
			if( IsSyafuKaisei( m_pZm->zvol ) && m_bNPanbn ) {
				sk->etc = m_pZm->bmname->bnbnm;

				if( !sk->etc.IsEmpty() ) {
					if( kjlen( (void*)(LPCTSTR)sk->etc, sk->etc.GetLength() ) ) {
						sk->etc_col = RGB_BLUE;
						ret = TRUE;
					}
				}
			}
			else {
				if( ! (m_pZm->bmname->bnhsg[0] & 0x02) ) {
					sk->etc = m_pZm->bmname->bnbnm;

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
	} while( m_pZm->bmname->MoveNext() == 0 );

	return ret;
}


// 転送済み仕訳か？
//		-1	修正不可
//		-2	転送済み仕訳で修正不可
//		 0	修正可能
int CDInputSub::tline_chk(CDBipDataRec *drec)
{
	CDBINPDataRec newdata;
	convNewDataRec(drec, &newdata);
	return tline_chk(&newdata);
}

int CDInputSub::tline_chk(CDBINPDataRec *drec )
{
	if( ! (m_pZm->zvol->tl_cor & 0x10) )	// マスター全体確定
		return -1;
	else if( /*! (m_pZm->zvol->->tl_cor & 0x01) &&*/ (drec->m_dsign.GetAt(7) & 0x10) )// 確定仕訳
		return -1;
	else if( /*! (m_pZm->zvol->->tl_cor & 0x01) &&*/ (drec->m_dsign.GetAt(7) & 0x20) )// 転送済み仕訳
		return -2;
	else
		return 0;
}


// 部門／工事コードが 登録されたものか？
BOOL CDInputSub::bmncode_ok( int bmn )
{
	if( ! m_pZm->bmname ) return FALSE;

	BOOL ret = FALSE;
	m_pZm->bmname->MoveFirst();

	do {
		if( bmn == m_pZm->bmname->bnbcd ) {
			//社会福祉改正マスターは合計部門入力可
			if( IsSyafuKaisei( m_pZm->zvol ) && m_bNPanbn ) {
				ret = TRUE;
			}
			else {
				if( ! (m_pZm->bmname->bnhsg[0] & 0x02) ) {
					ret = TRUE;
				}
			}
			break;
		}
	} while( m_pZm->bmname->MoveNext() == 0 );

	return ret;
}


int CDInputSub::GetLastDataSeq()
{
	CStRecordSet seq_rec( m_pZm->m_database );
	seq_rec.m_sql = _T( "select top(1) seq as st from datarec order by seq DESC" );
	CString emsg;
	int seq = -1;

	TRY {
		if( ! seq_rec.Open() ) {
			emsg = _T( "最終SEQを調べられません！" );
			return -99;
		}

		if( ! seq_rec.IsBOF() ) {
			seq = seq_rec.m_st;
		}

		seq_rec.Close();
	}
	CATCH( CMemoryException, me ) {
		me->GetErrorMessage( emsg.GetBuffer(_MAX_PATH), _MAX_PATH );
		emsg.ReleaseBuffer();
		ICSMessageBox( emsg, 0, 0, 0, m_pParent);
		return -99;	
	}
	CATCH( CDBException, de ) {
		emsg = de->m_strError;
		ICSMessageBox( emsg, 0, 0, 0, m_pParent);
		return -99;
	}
	END_CATCH

	return seq;
}




// 仮受・仮払 の枝番が自動で増えたかのチェック用
int CDInputSub::GetKariAutoBrnCnt()
{
	int edcnt = 0;

	if( (m_pZm->zvol->s_sgn1 & 0xf0) != 0 && !(m_pZm->zvol->s_sgn4&0x10) ) {
		CString filter;
		filter = _T( "seq = 3" );
		m_pZm->dcntl->Requery( filter );
		if( m_pZm->dcntl->st != -1 ) {
			edcnt = m_pZm->dcntl->cnt;
		}
	}

	return edcnt;
}



// 部門残高登録( 0として登録 )
int CDInputSub::BumonZanTorok( int bmn, CString  kmcod )
{
	CString filter;
	BOOL bKmkOK;

	if( ! (m_pZm->zvol->sub_sw & 0x02) )
		return 0;

	// 該当科目コードが削除されている恐れがあるので
	// 科目コードチェック
	filter.Format( "kncod = '%s'", kmcod );
	m_pZm->knrec->Requery( filter, 0 );
	if( m_pZm->knrec->st != 0 )	bKmkOK = FALSE;
	else					bKmkOK = TRUE;
	m_pZm->knrec->Requery( "", 0 );

ASSERT( m_pZm->bmname != NULL );
ASSERT( m_pZm->bmkzrec != NULL );

	// 名称ファイルがあるか
	filter.Format( "bnbcd = %d", bmn );
	int dseq = -1;
	int nam_sw = 0;
	int msai_bmn = 0;

	class CBMNAME	*pbmn;
	pbmn = m_pZm->bmname;

	pbmn->Requery( "", "" );
	if( pbmn->MoveFirst() == 0 ) {
		do {
			if( bmn == pbmn->bnbcd ) {
				nam_sw = 1;
				if( ! (pbmn->bnhsg[0] & 0x02) )
					msai_bmn = 1;
				
				break;
			}
			if( pbmn->klevel == 0 ) {
				if( dseq < pbmn->dseq ) {
					dseq = pbmn->dseq;
				}
			}
		} while( pbmn->MoveNext() == 0 );
	}

	if( ! nam_sw ) {
		dseq++;
		pbmn->AddNew();
		pbmn->bnbcd = bmn;
		pbmn->prbcd = -1;
		pbmn->pseq = -1;
		pbmn->dseq = dseq;

		msai_bmn = 1;
		pbmn->Update();
		// 選択欄に表示用
		pbmn->Requery( "", "" );

	//	m_pBmChk->BmnCheckUpdate();
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


int CDInputSub::DB_DataAppend( CDBipDataRec* data, int bseq )
{
	if( data->m_seq <= 0 )
		return 0;
	
	// マスター全体確定 or 参照モード
	if( ! (m_pZm->zvol->tl_cor & 0x10) || IsJzSansyo() )
		return 0;

	// 仮受・仮払自動枝番チェック
	int old_edcnt = 0;
	old_edcnt = GetKariAutoBrnCnt();

	data->m_owner = m_pZm->DBUserNo();

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

	m_pZm->dbdata->dsign[8] = 6;
	if( bseq != -1 ) {
		//挿入
		if( m_pZm->dbdata->DataAppend( bseq ) == 0 ) {
			ret = 0;
		}
	}
	else {
		if( m_pZm->dbdata->DataAppend() == 0 ) {
			ret = 0;
		}
	}
#ifdef LATER_CLOSE
	if( ! ret ) {
		insseq_make() = FALSE;	//SEQテーブル再作成
		m_insCnt = 0;
	}
#endif

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


#ifdef LATER_CLOSE
	// 最終書き込みデータを保存
	m_lastdata = *data;
	m_lastdata.m_seq++;		// 次データ用にSEQセット
	
	MakeInpDate( &m_lastdata );

	// 追加時のみ リクエリ が必要
	dbdata_Requery() = TRUE;

	// CDATA クエリーサイン ON --- 表示を更新するため
	data_Requery()	= TRUE;

#endif

	// 追加修正サイン
	IsModify() = TRUE;

#ifdef LATER_CLOSE
	// 科目残高 クエリーサイン
	kzrec_Requery() = TRUE;
#endif

	// 現在仕訳数を再セット
	CString filter;
	filter = _T( "seq = 1" );
	m_pZm->dcntl->Requery( filter );
	if( m_pZm->dcntl->st != -1 ) {
		m_dacnt	= m_pZm->dcntl->cnt;
	}

	// 仮受・仮払自動枝番された？
	if( old_edcnt != GetKariAutoBrnCnt() ) {
		ezquery_flag() = TRUE;
	}

	return ret;
}



int CDInputSub::DB_DataAppend(CDBINPDataRec* data, int bseq)
{
	if( data->m_seq <= 0 )
		return 0;

	// マスター全体確定 or 参照モード
	if( !(m_pZm->zvol->tl_cor & 0x10) || IsJzSansyo() )
		return 0;

	// 仮受・仮払自動枝番チェック
	int old_edcnt = 0;
	old_edcnt = GetKariAutoBrnCnt();

	data->m_owner = m_pZm->DBUserNo();

#ifdef NAIBU_VER2
	if( IsSyafuKaisei(zvol) ) {
		if( data->m_hjiyu == 1 && data->m_nbcd == 0 )
			data->m_hjiyu = 0;
	}
#endif

	SetCDBData(data);
	int ret = -1;

	if( data->m_dbmn != -1 ) {
		BumonZanTorok(data->m_dbmn, data->m_dbt);
	}
	if( data->m_cbmn != -1 && ((data->m_dbmn != data->m_cbmn) || (data->m_dbt != data->m_cre)) ) {
		BumonZanTorok(data->m_cbmn, data->m_cre);
	}

	//MyTrace("DataAppend seq = %d, ddate = %d, dbt = %s, cre = %s\n", data->m_seq, data->m_ddate, data->m_dbt, data->m_cre );
	//MyTrace("DataAppend ddate = %d\n", dbdata->ddate);

	m_pZm->dbdata->dsign[8] = 6;
	if( bseq != -1 ) {
		//挿入
		if( m_pZm->dbdata->DataAppend(bseq) == 0 ) {
			ret = 0;
		}
	}
	else {
		if( m_pZm->dbdata->DataAppend() == 0 ) {
			ret = 0;
		}
	}
#ifdef LATER_CLOSE
	if( !ret ) {
		insseq_make() = FALSE;	//SEQテーブル再作成
		m_insCnt = 0;
	}
#endif

	// 番号更新や、登録日等を反映
#ifdef NAIBU_VER2
	int old_nbcd = data->m_nbcd;
#endif
	GetCDBData(data);

#ifdef NAIBU_VER2
	//内部取引消去
	data->m_nbcd = old_nbcd;
	if( IsSyafuKaisei(zvol) ) {
		pDBnc->NaibutorihikiDataAdd(data->m_seq, data->m_nbcd);
	}
#endif


#ifdef LATER_CLOSE
	// 最終書き込みデータを保存
	m_lastdata = *data;
	m_lastdata.m_seq++;		// 次データ用にSEQセット

	MakeInpDate(&m_lastdata);

	// 追加時のみ リクエリ が必要
	dbdata_Requery() = TRUE;

	// CDATA クエリーサイン ON --- 表示を更新するため
	data_Requery() = TRUE;

#endif

	// 追加修正サイン
	IsModify() = TRUE;

#ifdef LATER_CLOSE
	// 科目残高 クエリーサイン
	kzrec_Requery() = TRUE;
#endif

	// 現在仕訳数を再セット
	CString filter;
	filter = _T("seq = 1");
	m_pZm->dcntl->Requery(filter);
	if( m_pZm->dcntl->st != -1 ) {
		m_dacnt = m_pZm->dcntl->cnt;
	}

	// 仮受・仮払自動枝番された？
	if( old_edcnt != GetKariAutoBrnCnt() ) {
		ezquery_flag() = TRUE;
	}

	return ret;
}



// 入力用 BCD 日付を取得
void CDInputSub::GetInpDate( BYTE* bcddate )
{
	memcpy( bcddate, m_InpBcd, 3 );
}

// 入力用 BCD 日付をセット
void CDInputSub::SetInpDate( int ddate, int ksign )
{
	int nDate = ddate;

	if( nDate == -1 ) {
		nDate = m_pZm->zvol->ip_ymd;
	}

	DateConv( m_InpBcd, (int*)&nDate, CONV2_1 );
}


int CDInputSub::GetInputStrLen( unsigned char *buf )
{
	int ln, lengs;

	lengs = m_pZm->zvol->tk_ln * 2;

	ln = kjlen( buf, lengs );

	return ln;
}

// 仕訳を比較して変更されたかどうか？
// 仕訳確定サインが立っていたら無視する
//
BOOL CDInputSub::isdiff_data( CDBipDataRec *newdata, CDBipDataRec *moto )
{
	BOOL ret;

	// 確定仕訳が修正不可
	if( ! IsModifyConfirmData() ) {
		if( moto->m_dsign[7] & 0x30 ) {
			if( ! (newdata->m_dsign[7] & 0x30) ) {
				//遠隔地ユーザか？
				int usrtyp = GET_USERTYPE();
				if( usrtyp )	ret = FALSE;
				else			ret = TRUE;
			}
			else {
				ret = FALSE;
			}
		}
		else {
			ret = (*newdata != *moto);
		}
	}
	else {
		// 確定仕訳を修正可
		ret = (*newdata != *moto);
	}
	MyTrace("■■ seq=%d, newdata inv = %s, moto inv = %s, ret = (%d)\n", newdata->m_seq, newdata->m_invno, moto->m_invno, ret);
	DSIGN_TRACE(newdata->m_dsign);
	DSIGN_TRACE(moto->m_dsign);
	DSIGN_TRACE2(newdata->m_dsign);
	DSIGN_TRACE2(moto->m_dsign);

	DSIGN_TRACE(newdata->m_invno);
	DSIGN_TRACE(moto->m_invno);


	MyTrace("■■ seq=%d, m_ksign(%d) m_ddate(%d), m_mofs(%d) m_cno(%d), m_dbm(%d)\n", newdata->m_seq, newdata->m_ksign, newdata->m_ddate, newdata->m_mofs, newdata->m_cno, newdata->m_dbmn);
	MyTrace("□□ seq=%d, m_ksign(%d) m_ddate(%d), m_mofs(%d) m_cno(%d), m_dbm(%d)\n", moto->m_seq, moto->m_ksign, moto->m_ddate, moto->m_mofs, moto->m_cno, moto->m_dbmn);

	MyTrace("■■ m_val=%02x%02x%02x%02x%02x%02x, m_zei %02x%02x%02x%02x%02x%02x\n", newdata->m_val[0], newdata->m_val[1], newdata->m_val[2], newdata->m_val[3], newdata->m_val[4], newdata->m_val[5],
														newdata->m_zei[0], newdata->m_zei[1], newdata->m_zei[2], newdata->m_zei[3], newdata->m_zei[4], newdata->m_zei[5]);
	MyTrace("□□ m_val=%02x%02x%02x%02x%02x%02x, m_zei %02x%02x%02x%02x%02x%02x\n", moto->m_val[0], moto->m_val[1], moto->m_val[2], moto->m_val[3], moto->m_val[4], moto->m_val[5],
														moto->m_zei[0], moto->m_zei[1], moto->m_zei[2], moto->m_zei[3], moto->m_zei[4], moto->m_zei[5]);

	MyTrace("■■ m_dkno=%s, m_dbt(%s) m_dbr(%d), m_cbmn(%d) m_ckno(%s), m_cre(%s), m_cbr(%d) \n", newdata->m_dkno, newdata->m_dbt, newdata->m_dbr, newdata->m_cbmn, newdata->m_ckno, newdata->m_cre, newdata->m_cbr);
	MyTrace("□□ m_dkno=%s, m_dbt(%s) m_dbr(%d), m_cbmn(%d) m_ckno(%s), m_cre(%s), m_cbr(%d) \n", moto->m_dkno, moto->m_dbt, moto->m_dbr, moto->m_cbmn, moto->m_ckno, moto->m_cre, moto->m_cbr);

	MyTrace("■■ m_owner=%d, m_dtype(%d) m_tekiyo(%s), m_imgsq(%d) m_snumber(%s), m_tdate(%d), m_edate(%d) \n", newdata->m_owner, newdata->m_dtype, newdata->m_tekiyo, newdata->m_imgsq, newdata->m_snumber, newdata->m_tdate, newdata->m_edate);
	MyTrace("□□ m_owner=%d, m_dtype(%d) m_tekiyo(%s), m_imgsq(%d) m_snumber(%s), m_tdate(%d), m_edate(%d) \n", moto->m_owner, moto->m_dtype, moto->m_tekiyo, moto->m_imgsq, moto->m_snumber, moto->m_tdate, moto->m_edate);

	MyTrace("■■ m_udate=%d, m_icno(%d) m_skn(%s), m_tag(%s) m_hjiyu(%d), m_nbcd(%d) \n", newdata->m_udate, newdata->m_icno, newdata->m_skn, newdata->m_tag, newdata->m_hjiyu, newdata->m_nbcd);
	MyTrace("□□ m_udate=%d, m_icno(%d) m_skn(%s), m_tag(%s) m_hjiyu(%d), m_nbcd(%d) \n", moto->m_udate, moto->m_icno, moto->m_skn, moto->m_tag, moto->m_hjiyu, moto->m_nbcd);

	MyTrace("■■ m_taika =%02x%02x%02x%02x%02x%02x\n", newdata->m_taika[0], newdata->m_taika[1], newdata->m_taika[2], newdata->m_taika[3], newdata->m_taika[4], newdata->m_taika[5]);
	MyTrace("□□ m_taika =%02x%02x%02x%02x%02x%02x\n", moto->m_taika[0], moto->m_taika[1], moto->m_taika[2], moto->m_taika[3], moto->m_taika[4], moto->m_taika[5]);

#ifdef CLOSE
	if( m_seq == data.m_seq &&
		m_ksign == data.m_ksign &&
		m_ddate == data.m_ddate &&
		m_mofs == data.m_mofs &&
		m_cno == data.m_cno &&
		m_dbmn == data.m_dbmn &&
		m_dkno == data.m_dkno &&
		m_dbt == data.m_dbt &&
		m_dbr == data.m_dbr &&

		m_cbmn == data.m_cbmn &&
		m_ckno == data.m_ckno &&
		m_cre == data.m_cre &&
		m_cbr == data.m_cbr &&

		!memcmp(m_val, data.m_val, sizeof m_val) &&
		!memcmp(m_zei, data.m_zei, sizeof m_zei) &&

		m_owner == data.m_owner &&
		m_dtype == data.m_dtype &&

		m_tekiyo == data.m_tekiyo &&
		m_imgsq == data.m_imgsq &&
		m_snumber == data.m_snumber &&
		m_tdate == data.m_tdate &&
		m_edate == data.m_edate &&

		m_udate == data.m_udate	&&
		m_icno == data.m_icno	&&
		m_skn == data.m_skn &&
		m_tag == data.m_tag &&
		m_hjiyu == data.m_hjiyu &&
		!memcmp(m_taika, data.m_taika, sizeof m_taika) &&
		m_nbcd == data.m_nbcd &&
		!memcmp(m_invno, data.m_invno, sizeof(m_invno))	// インボイス対応
		) {
#endif
	return ret;
}


BOOL CDInputSub::isdiff_data(CDBINPDataRec *newdata, CDBINPDataRec *moto)
{
	BOOL ret;

	// 確定仕訳が修正不可
	if( !IsModifyConfirmData() ) {
		if( moto->m_dsign[7] & 0x30 ) {
			if( !(newdata->m_dsign[7] & 0x30) ) {
				//遠隔地ユーザか？
				int usrtyp = GET_USERTYPE();
				if( usrtyp )	ret = FALSE;
				else			ret = TRUE;
			}
			else {
				ret = FALSE;
			}
		}
		else {
			ret = (*newdata != *moto);
		}
	}
	else {
		// 確定仕訳を修正可
		ret = (*newdata != *moto);
	}
	MyTrace("■■ seq=%d, newdata inv = %s, moto inv = %s, ret = (%d)\n", newdata->m_seq, newdata->m_invno, moto->m_invno, ret);
	DSIGN_TRACE(newdata->m_dsign);
	DSIGN_TRACE(moto->m_dsign);
	DSIGN_TRACE2(newdata->m_dsign);
	DSIGN_TRACE2(moto->m_dsign);

	DSIGN_TRACE(newdata->m_invno);
	DSIGN_TRACE(moto->m_invno);


	MyTrace("■■ seq=%d, m_ksign(%d) m_ddate(%d), m_mofs(%d) m_cno(%d), m_dbm(%d)\n", newdata->m_seq, newdata->m_ksign, newdata->m_ddate, newdata->m_mofs, newdata->m_cno, newdata->m_dbmn);
	MyTrace("□□ seq=%d, m_ksign(%d) m_ddate(%d), m_mofs(%d) m_cno(%d), m_dbm(%d)\n", moto->m_seq, moto->m_ksign, moto->m_ddate, moto->m_mofs, moto->m_cno, moto->m_dbmn);

	MyTrace("■■ m_val=%02x%02x%02x%02x%02x%02x, m_zei %02x%02x%02x%02x%02x%02x\n", newdata->m_val[0], newdata->m_val[1], newdata->m_val[2], newdata->m_val[3], newdata->m_val[4], newdata->m_val[5],
		newdata->m_zei[0], newdata->m_zei[1], newdata->m_zei[2], newdata->m_zei[3], newdata->m_zei[4], newdata->m_zei[5]);
	MyTrace("□□ m_val=%02x%02x%02x%02x%02x%02x, m_zei %02x%02x%02x%02x%02x%02x\n", moto->m_val[0], moto->m_val[1], moto->m_val[2], moto->m_val[3], moto->m_val[4], moto->m_val[5],
		moto->m_zei[0], moto->m_zei[1], moto->m_zei[2], moto->m_zei[3], moto->m_zei[4], moto->m_zei[5]);

	MyTrace("■■ m_dkno=%s, m_dbt(%s) m_dbr(%d), m_cbmn(%d) m_ckno(%s), m_cre(%s), m_cbr(%d) \n", newdata->m_dkno, newdata->m_dbt, newdata->m_dbr, newdata->m_cbmn, newdata->m_ckno, newdata->m_cre, newdata->m_cbr);
	MyTrace("□□ m_dkno=%s, m_dbt(%s) m_dbr(%d), m_cbmn(%d) m_ckno(%s), m_cre(%s), m_cbr(%d) \n", moto->m_dkno, moto->m_dbt, moto->m_dbr, moto->m_cbmn, moto->m_ckno, moto->m_cre, moto->m_cbr);

	MyTrace("■■ m_owner=%d, m_dtype(%d) m_tekiyo(%s), m_imgsq(%d) m_snumber(%s), m_tdate(%d), m_edate(%d) \n", newdata->m_owner, newdata->m_dtype, newdata->m_tekiyo, newdata->m_imgsq, newdata->m_snumber, newdata->m_tdate, newdata->m_edate);
	MyTrace("□□ m_owner=%d, m_dtype(%d) m_tekiyo(%s), m_imgsq(%d) m_snumber(%s), m_tdate(%d), m_edate(%d) \n", moto->m_owner, moto->m_dtype, moto->m_tekiyo, moto->m_imgsq, moto->m_snumber, moto->m_tdate, moto->m_edate);

	MyTrace("■■ m_rnumber(%s) \n", newdata->m_rnumber);
	MyTrace("□□ m_rnumber(%s) \n", moto->m_rnumber);

	MyTrace("■■ m_udate=%d, m_icno(%d) m_skn(%s), m_tag(%s) m_hjiyu(%d), m_nbcd(%d) \n", newdata->m_udate, newdata->m_icno, newdata->m_skn, newdata->m_tag, newdata->m_hjiyu, newdata->m_nbcd);
	MyTrace("□□ m_udate=%d, m_icno(%d) m_skn(%s), m_tag(%s) m_hjiyu(%d), m_nbcd(%d) \n", moto->m_udate, moto->m_icno, moto->m_skn, moto->m_tag, moto->m_hjiyu, moto->m_nbcd);

	MyTrace("■■ m_taika =%02x%02x%02x%02x%02x%02x\n", newdata->m_taika[0], newdata->m_taika[1], newdata->m_taika[2], newdata->m_taika[3], newdata->m_taika[4], newdata->m_taika[5]);
	MyTrace("□□ m_taika =%02x%02x%02x%02x%02x%02x\n", moto->m_taika[0], moto->m_taika[1], moto->m_taika[2], moto->m_taika[3], moto->m_taika[4], moto->m_taika[5]);
	return ret;
}


int CDInputSub::memzrec_get( LPCTSTR code, struct _DBKZREC* zrec )
{
	int i, sgn = 0;
	DWORD	dwCode;
	CString tmp, strCode;
	struct _DBKZREC*	pz;
	BOOL bSyog;

	::ZeroMemory( zrec, sizeof(struct _DBKZREC) );
	bSyog = isSYOGT_CODE( code );

	for( i = 0; i < m_pZm->kzn_info->reccount; i++ ) {
		pz		= (m_pZm->pKzrec+i);
		dwCode	= pz->kzkcd;
		hl_rev( &dwCode, 4 );
		strCode.Format( "%08x", dwCode );

		if( bSyog ) {
			if( isSYOGT_CODE( strCode ) ) {	// 複数諸口がある場合のため
				zrec->kzatt	= pz->kzatt;
				
				// 集計
				l_add( zrec->kzzan, zrec->kzzan, pz->kzzan );	// 開始残高
				for( i = 0 ; i < 19; ++i )		// 各月
				{
					l_add( zrec->kzval[i].Kari, zrec->kzval[i].Kari, pz->kzval[i].Kari );	// 借方
					l_add( zrec->kzval[i].Kasi, zrec->kzval[i].Kasi, pz->kzval[i].Kasi );	// 貸方
				}
				sgn++;
			}
			else if( sgn )	break;
		}
		else {
			if( strCode.CompareNoCase( code ) ) {
				memcpy( zrec, pz, sizeof(struct _DBKZREC) );
				break;
			}
		}
	}
	return 0;
}



int CDInputSub::dbzrec_get( LPCTSTR code, struct _DBKZREC* zrec )
{
	if( m_pZm->kzrec == NULL )
		return -1;

	m_pZm->kzrec->MoveFirst();
	int i, sgn = 0;
	::ZeroMemory( zrec, sizeof(struct _DBKZREC) );
	CString tmp;

	BOOL bSyog;
	bSyog = isSYOGT_CODE( code );

	do {
		tmp	= m_pZm->kzrec->kzkcd;
		tmp.TrimRight();

		if( bSyog ) {
			if( isSYOGT_CODE( tmp ) ) {	// 複数諸口がある場合のため
				zrec->kzatt	= m_pZm->kzrec->kzatt;
				char tmp[6];
				// 集計
				l_input( tmp, (void*)(LPCTSTR)m_pZm->kzrec->kzzan );
				l_add( zrec->kzzan, zrec->kzzan, tmp );	// 開始残高
				for( i = 0 ; i < 16 ; ++i )					// 各月
				{
					l_input( tmp, (void*)(LPCTSTR)m_pZm->kzrec->lhs[i] );
					l_add( zrec->kzval[i].Kari, zrec->kzval[i].Kari, tmp );	// 借方
					l_input( tmp, (void*)(LPCTSTR)m_pZm->kzrec->rhs[i] );
					l_add( zrec->kzval[i].Kasi, zrec->kzval[i].Kasi, tmp );	// 貸方
				}
				sgn++;
			}
			else if( sgn )	break;
		}
		else {
			if( tmp == code ) {
				DWORD dwCode = _atoh( (LPSTR)(LPCTSTR)tmp );
				hl_rev( &dwCode, 4 );
				zrec->kzkcd = dwCode;
				zrec->kzatt	= m_pZm->kzrec->kzatt;

				l_input( zrec->kzzan, (void*)(LPCTSTR)m_pZm->kzrec->kzzan );

				for( int i = 0; i < 16; i++ ) {
					l_input( zrec->kzval[i].Kari, (void*)(LPCTSTR)m_pZm->kzrec->lhs[i] );
					l_input( zrec->kzval[i].Kasi, (void*)(LPCTSTR)m_pZm->kzrec->rhs[i] );
				}
				
				break;
			}
		}

	} while( m_pZm->kzrec->MoveNext() == 0 );

	return 0;
}



int CDInputSub::zrecget( LPCTSTR code, struct _DBKZREC* zrec )
{
//	if( m_pZm->pKzrec ) {
//		return memzrec_get( code, zrec );
//	}
//	else {
		return dbzrec_get( code, zrec );
//	}
//	return 0;
}

//
//		諸口 合計計算
//
//
int CDInputSub::CalqSyogTotal( LPCTSTR syogCode, char *pSyogd, char *pSyogc, char *zan )
{
	struct _DBKZREC		mrec;

	int		i;
	char	val[6] = {0};

	::ZeroMemory( m_SYOGdbt, sizeof m_SYOGdbt );
	::ZeroMemory( m_SYOGcre, sizeof m_SYOGcre );
	::ZeroMemory( m_SYOGzan, sizeof m_SYOGcre );

	CString strCode;
	strCode = syogCode;

	struct _DBKNREC* pKn;
	pKn = DB_PjisToKnrec( strCode, m_pZm );

	if( pKn == NULL ) {
		memcpy( pSyogd, val, sizeof val );
		memcpy( pSyogc, val, sizeof val );
		memcpy( zan, val, sizeof val );
		return 0;
	}

	l_defn(0x16);

	if( zrecget( strCode, &mrec ))
	{
		memcpy( pSyogd, val, sizeof val );
		memcpy( pSyogc, val, sizeof val );
		memcpy( zan, val, sizeof val );
		return(-1);
	}
	// 未払金等など貸方科目の場合は、反転
	memcpy( val, mrec.kzzan, sizeof val );
	if( (mrec.kzatt & 0x01) )
		l_neg( val );

	l_add( m_SYOGdbt, m_SYOGdbt, val );

	for( i = 0; i < 16; i++ )
	{
		l_add( m_SYOGdbt, m_SYOGdbt, (char *)&mrec.kzval[i].Kari );
		l_add( m_SYOGcre, m_SYOGcre, (char *)&mrec.kzval[i].Kasi );
	}

	l_sub( m_SYOGzan, m_SYOGdbt, m_SYOGcre );

	// 未払金等など貸方科目の場合は、反転
	if( (mrec.kzatt & 0x01) )	l_neg( m_SYOGzan );

	memcpy( pSyogd, m_SYOGdbt, sizeof m_SYOGdbt );
	memcpy( pSyogc, m_SYOGcre, sizeof m_SYOGcre );
	memcpy( zan, m_SYOGzan, sizeof m_SYOGzan );

	return(0);
}



// BCD 日付 から int 日付へ 変換
int CDInputSub::BcdDateToYmd( BYTE* bcddate, int* ymd )
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

	if( vd.db_vd_yearGen( 0, mmdd, &gengo, &date, &y_sw, m_pZm ) == 0 ) {
		int seireki;
		vd.db_datecnvGen( 0, date, &gengo, &seireki, 1, 0 );
		*ymd = seireki;
		ret = 0;
	}

	return ret;
}



// 月日を 変換 仕訳レコードより
int	CDInputSub::DateConv( BYTE* bcddate, CDBipDataRec* d_date, int sgn )
{
	char buf[30];
	class CMINFO* minfo = m_pZm->minfo;

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

			if( vd.db_vd_offset(tmp, &ofs, m_pZm) == -1 ) {
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



// 月日を 変換 仕訳レコードより
int	CDInputSub::DateConv(BYTE* bcddate, CDBINPDataRec* d_date, int sgn)
{
	char buf[30];
	class CMINFO* minfo = m_pZm->minfo;

	if( sgn == CV1_2 ) {
		// BCD -> Record	
		BcdDateToYmd(bcddate, &d_date->m_ddate);

		int ks_sgn;
		ks_sgn = KESY_SG_GET(bcddate[0]);

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
			BYTE tmp[10] = { 0 };
			tmp[0] = (bcddate[0] & 0x1f);
			tmp[1] = (bcddate[1] & 0xff);

			d_date->m_ksign = 0;
			CVolDateDB vd;
			int ofs;

			if( vd.db_vd_offset(tmp, &ofs, m_pZm) == -1 ) {
				return -1;
			}
			d_date->m_mofs = (BYTE)ofs;
		}
	}
	else {
		// Record → BCD
		sprintf_s(buf, sizeof buf, "%d", d_date->m_ddate);
		if( strlen(buf) == 8 ) {
			l_pack(bcddate, &buf[4], 4);
		}
		else {
			l_pack(bcddate, &buf[0], 4);
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

			if( !sgn ) {
				ASSERT(FALSE);
				return -1;
			}

			KESY_SG_SET(bcddate[0], sgn);
		}
	}

	return 0;
}


// 月日を 変換
int	CDInputSub::DateConv( BYTE* bcddate, int* date, int sgn )
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

BOOL CDInputSub::isSyzdiff_data(CDBipDataRec *newdata, CDBipDataRec *moto)
{
	CDBINPDataRec new_ndta, moto_ndta;
	convNewDataRec(newdata, &new_ndta);
	convNewDataRec(moto, &moto_ndta);

	return isSyzdiff_data(&new_ndta, &moto_ndta);
}

BOOL CDInputSub::isSyzdiff_data(CDBINPDataRec *newdata, CDBINPDataRec *moto )
{
	BOOL ret = FALSE;
	BYTE	newdsign[10] = { 0 };
	BYTE	motodsign[10] = { 0 };

	memcpy( newdsign, &newdata->m_dsign[0], 7 );
	memcpy( motodsign, &moto->m_dsign[0], 7 );

	DSIGN_TRACE(newdsign);
	DSIGN_TRACE(motodsign);
	const int invLen = 14;

	MyTrace("new invno = %s, moto inv = %s\n", newdata->m_invno, moto->m_invno);

	if( (newdsign[0]&0x40) != (motodsign[0]&0x40) )	{
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
				int new_dbt = km_syattr( newdata->m_dbt );
				int new_cre = km_syattr( newdata->m_cre );
				int moto_dbt = km_syattr( moto->m_dbt );
				int moto_cre = km_syattr( moto->m_cre );

				if( (new_dbt & 0x0200) || (new_cre & 0x0200) || (moto_dbt & 0x0200) || (moto_cre & 0x0200) ) {
				}
				else {
					newdsign[1] &= ~0x0f;
					motodsign[1] &= ~0x0f;
				}
			}
			if( !ret ) {
				if( memcmp( &newdsign[1], &motodsign[1], 5 ) != 0 ) 
					ret = TRUE;
			}

			if( !ret ) {
				if( IsMasterType(MST_SYZ_INV) ) {
					if( (newdsign[6] & 0xE0) != (motodsign[6] & 0xE0) ) {
						ret = TRUE;
					}
					if( bInvDisp ) {
						//どちらも登録番号ある
						if( newdata->m_invno[0] && moto->m_invno[0] ) {
							if( memcmp(newdata->m_invno, moto->m_invno, invLen) != 0 ) {
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
//MyTrace( "isSyzdiff_data newdsign [%02x%02x%02x%02x%02x%02x], motodsign [%02x%02x%02x%02x%02x%02x]",
//		newdsign[0],newdsign[1],newdsign[2],newdsign[3],newdsign[4],newdsign[5],  motodsign[0],motodsign[1],motodsign[2],motodsign[3],motodsign[4],motodsign[5] );

	return ret;
}

// 消費税仕訳か？　(完成振替を除く)
BOOL CDInputSub::is_syohizeisiwake( CDBDATA* dbdata )
{
	CDBipDataRec drec;

	GetCDBData( &drec, dbdata );

	return is_syohizeisiwake( &drec );
}

// 消費税仕訳か？　(完成振替を除く)
BOOL CDInputSub::is_syohizeisiwake(CDBipDataRec* data)
{
	CDBINPDataRec newdata;
	convNewDataRec(data, &newdata);
	return is_syohizeisiwake(&newdata);
}

BOOL CDInputSub::is_syohizeisiwake(CDBINPDataRec* data )
{
	if (m_pZm != NULL) {
		if (m_pZm->zvol->apno == 0x20) {
			// m_dsign[6] D5bitが免税事業者からの課税仕入れのサインとなり、==で判定できないため修正
			//if (data->m_dsign[0] & 0x40 && data->m_dsign[4] == 0x1f && data->m_dsign[6] == 0x10) {
			if (data->m_dsign[0] & 0x40 && data->m_dsign[4] == 0x1f && data->m_dsign[6] & 0x10) {
			//	return FALSE;
			}
		}
	}
	BOOL ret = FALSE;
	ret = (data->m_dsign[0] & 0x40) ? TRUE : FALSE;

	return ret;
}


//
// 科目と枝番残高で消費税の設定が異なるか？
BOOL CDInputSub::IsDiffEdabanSyzSgn(char* kmkcd, int brn)
{
	CString filter, buf;
	BOOL bret = FALSE;

	//枝番消費税登録サイン ＯＦＦ
	if (!(m_pZm->zvol->s_sgn2 & 0x10))
		return FALSE;

	ezdb_init(m_pZm);
	if (m_pZm->ezrec->Find((LPCTSTR)kmkcd, brn) > 0) {

		DWORD tmp;
		tmp = _atoh(kmkcd);
		hl_rev(&tmp, 4);

		DBKNREC* pKn;
		pKn = m_pZm->PjisToKnrec(tmp);

		if (pKn) {
			if (memcmp(&pKn->knsgn[2], &m_pZm->ezrec->ezsysgn[2], 3) != 0) {
				bret = TRUE;
			}
		}
	}
	return bret;
}


// 消費税がデフォルトと異なる税額か？
BOOL CDInputSub::isnot_defzeigaku(CDBDATA* dbdata, COLORREF& back)
{
	CDBipDataRec drec;
	GetCDBData(&drec, dbdata);

	return isnot_defzeigaku(&drec, back);
}


// 消費税がデフォルトと異なる税額か？
BOOL CDInputSub::isnot_defzeigaku(CDBipDataRec* data, COLORREF& back)
{
	CDBINPDataRec newdata;
	convNewDataRec(data, &newdata);
	return isnot_defzeigaku(&newdata, back);
}

// 消費税がデフォルトと異なる税額か？
BOOL CDInputSub::isnot_defzeigaku(CDBINPDataRec* data, COLORREF& back)
{
	BOOL ret = FALSE;

	if( is_sotomensiwake(data) ) {
		ret = TRUE;
		back = RGB_ZEI_MDFY;
		return ret;
	}

	if (data->m_dsign[0] & 0x40 && (data->m_dsign[2] & 0x0f) == 1) {
		LPCTSTR zeistr = pZeiCl->GetDefZeiData(data);
		CArith ar(0x16);
		char def_zei[10] = { 0 };
		ar.l_input(def_zei, (void*)zeistr);
		//MyTrace("seq = %d, def zeistr = %s\n", data->m_seq, zeistr);
		//L_PRINT("data->m_zei=", data->m_zei);
		if (ar.l_cmp(def_zei, data->m_zei) != 0) {
			ret = TRUE;
			back = RGB_ZEI_MDFY;
		//	if (tline_chk(data) < 0 && IsModifyConfirmData()) {
		//		back = RGB_SYZ_MDFY;
		//	}
		}
	}
	return ret;
}

// 輸出売上仕訳か？
BOOL CDInputSub::is_yusyutsusiwake(CDBDATA* dbdata)
{
	CDBipDataRec drec;

	GetCDBData(&drec, dbdata);

	return is_yusyutsusiwake(&drec);
}

// 輸出売上仕訳か？　
BOOL CDInputSub::is_yusyutsusiwake(CDBipDataRec* data)
{
	CDBINPDataRec newdata;
	convNewDataRec(data, &newdata);

	return is_yusyutsusiwake(&newdata);
}
BOOL CDInputSub::is_yusyutsusiwake(CDBINPDataRec* data)
{
	BOOL ret = FALSE;
	if( data->m_dsign[0] & 0x40 ) {
		if( data->m_dsign[4] == 0x0d ) {
			ret = TRUE;
		}
		else {
			if( km_yusyutsu(data->m_dbt) || km_yusyutsu(data->m_cre) )
				ret = TRUE;
		}
	}
	return ret;
}


// 簡易課税か個別対応か？
int CDInputSub::IsKaniOrKobetsu()
{
	int ret = 0;

	if( (m_pZm->zvol->s_sgn2 & 0x0f) >= 2 || (m_pZm->zvol->s_sgn4 & 0x80) )
		ret++;
	return ret;
}


// 免税マスターか？
int	CDInputSub::IsMenzei()
{
	int ret = 0;

	if( (zvol->s_sgn2 & 0x0f) == 1 )
		ret++;

	return ret;
}


// 簡易課税か？
int	CDInputSub::IsKani()
{
	int ret = 0;

	if( (zvol->s_sgn2 & 0x0f) >= 2 )
		ret++;

	return ret;
}

// 個別対応か？
int	CDInputSub::IsKobetsu()
{
	int ret = 0;

	if( (zvol->s_sgn4 & 0x80) )
		ret++;

	return ret;
}


void CDInputSub::KamokuString(DBKNREC* pKn, CString& string)
{
	KamokuString(pKn, string.GetBuffer(64), 64);
	string.ReleaseBuffer();
}

void CDInputSub::KamokuString(DBKNREC* pKn, char* buf, int bufsize)
{
	if( m_pAsel->SCNCD_OPT == OPT_SCNCD ) {
		kmkcode_string(2, pKn, buf, bufsize);
	}
	else {
		sprintf_s(buf, bufsize, "%.14s", pKn->knnam);
	}
}


void CDInputSub::get_datelimit(int& sofs, int& eofs, int& sy, int& ey)
{
	sofs = m_inpSofs;
	eofs = m_inpEofs;
	sy = m_inpSy;
	ey = m_inpEy;
}

void CDInputSub::set_datelimit(int sofs, int eofs, int sy, int ey)
{
	m_inpSofs = sofs;
	m_inpEofs = eofs;
	m_inpSy = sy;
	m_inpEy = ey;
}

BOOL CDInputSub::check_datelimit(BYTE* bcddate)
{
	if( m_inpSofs == -1 )
		return TRUE;

	CVolDateDB vd;
	int ofset;

	//MyTrace( "::check_datelimit %02x%02x\n", bcddate[0], bcddate[1] );

	if( vd.db_vd_offset(bcddate, &ofset, m_pZm) != 0 )
		return FALSE;

	//MyTrace( "::check_datelimit ofs = %d, m_inpSofs %d, m_inpSofs %d\n", ofset, m_inpSofs, m_inpEofs );

	if( ofset > m_inpSofs && ofset < m_inpEofs )
		return TRUE;

	int arg_date, date, tmp, d_gengo;
	char buf[64];
	VDPAC vpac;

	if( bcddate[1] != (BYTE)0xff )
		sprintf_s(buf, sizeof buf, "%02x%02x", bcddate[0], bcddate[1]);
	else
		sprintf_s(buf, sizeof buf, "%02x00", bcddate[0]);

	tmp = atoi(buf);

	//MyTrace( "::check_datelimit tmp = %d, m_inpSy = %d, m_inpEy = %d\n", tmp, m_inpSy, m_inpEy );

	if( ofset == m_inpSofs ) {
		// 開始日付と入力日付をチェック
		if( vd.db_vd_dpacGen(1, 0, tmp, &vpac, m_pZm) != 0 )
			return FALSE;

		vd.db_datecnvGen(vpac.Dgengo, vpac.Dymd, &d_gengo, &arg_date, 1, 0);

#ifdef CLOSE
		if( vd.db_vd_dpacGen(1, 0, m_inpSy, &vpac, m_pZm) != 0 )
			return FALSE;
		vd.db_datecnvGen(vpac.Dgengo, vpac.Dymd, &d_gengo, &date, 1, 0);
#endif
		date = m_inpSy;

		if( arg_date >= date )
			return TRUE;
	}
	else if( ofset == m_inpEofs ) {
		// 終了日付と入力日付をチェック
		if( vd.db_vd_dpacGen(1, 0, tmp, &vpac, m_pZm) != 0 )
			return FALSE;

		vd.db_datecnvGen(vpac.Dgengo, vpac.Dymd, &d_gengo, &arg_date, 1, 0);

#ifdef CLOSE
		if( vd.db_vd_dpacGen(1, 0, m_inpEy, &vpac, m_pZm) != 0 )
			return FALSE;
		vd.db_datecnvGen(vpac.Dgengo, vpac.Dymd, &d_gengo, &date, 1, 0);
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
void CDInputSub::get_ofsdate(int ofs, int sgn, BYTE* bcddate)
{
	CDBipDataRec tmp;
	BYTE check[10] = { 0 };
	int flg = 0;

	minfo->MoveFirst();
	do {
		if( minfo->ofset == ofs ) {
			tmp.m_ddate = (sgn == 0) ? minfo->smd : minfo->emd;
			tmp.m_mofs = minfo->ofset;
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
			date = (zvol->ip_ymd % 10000);

			if( vd.db_vd_offset(0, date, &ofset, m_pZm) != -1 ) {
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
			DateConv(check, &tmp, CV2_1);
			memcpy(bcddate, check, 2);
		}
	}
}


// 指定オフセットの日付を BCDで返送(決算修正は 末日)
//	int ofs			オフセット
//	BYTE* bcddate 
//
void CDInputSub::get_ofsdate(int ofs, BYTE* bcddate)
{
	CDBipDataRec tmp;
	BYTE check[10] = { 0 };
	int flg = 0;

	minfo->MoveFirst();
	do {
		if( minfo->ofset == ofs ) {
			tmp.m_mofs = minfo->ofset;
			tmp.m_ksign = minfo->kes_sgn ? 1 : 0;

			if( tmp.m_ksign == 0 ) {
				if( zvol->ip_ymd > minfo->smd )
					tmp.m_ddate = zvol->ip_ymd;
				else
					tmp.m_ddate = minfo->smd;
			}
			else {
				tmp.m_ddate = minfo->emd;
			}

			flg = 1;
			break;
		}
	} while( minfo->MoveNext() == 0 );

	if( flg ) {
		DateConv(check, &tmp, CV2_1);
		memcpy(bcddate, check, 2);
	}
}

int CDInputSub::inpdate_shimechk(BYTE* orgdate, char* check)
{
	BYTE	bcd[10] = { 0 };
	char	buf[36];
	CVolDateDB vd;
	int sgn, ofst, newofst, st;
	int smd, emd, inp_day, s_day, e_day;
	int sofs, eofs, sy, ey;

	get_datelimit(sofs, eofs, sy, ey);
	// 複数月は、チェックせず
	if( sofs != eofs )
		return 0;

	sgn = KESY_SG_GET(orgdate[0]);
	switch( sgn ) {
	case 1:
		bcd[0] = 0x61;	break;
	case 2:
		bcd[0] = 0x62;	break;
	case 3:
		bcd[0] = 0x63;	break;
	case 4:
		bcd[0] = 0x00;	break;
	default:
		bcd[0] = orgdate[0];
		break;
	}
	bcd[1] = orgdate[1];

	if( vd.db_vd_offset(bcd, &ofst, m_pZm) != 0 )
		return -1;

	CArith ar;
	buf[0] = check[2];
	buf[1] = check[3];
	buf[2] = '\0';

	ar.l_pack(&bcd[1], buf, 2);
	inp_day = atoi(buf);

	st = vd.db_vd_check(bcd, m_pZm);

	if( st == 0 ) {
		if( vd.db_vd_offset(bcd, &newofst, m_pZm) != 0 )
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

	s_day = (smd % 100);
	e_day = (emd % 100);

	if( inp_day < s_day ) {
		// 終了月をセット
		sprintf_s(buf, sizeof buf, "%02d", (emd % 10000) / 100);
		if( !sgn ) {
			check[0] = buf[0];
			check[1] = buf[1];
		}
	}
	else if( inp_day > e_day ) {
		// 開始月をセット
		sprintf_s(buf, sizeof buf, "%02d", (smd % 10000) / 100);
		if( !sgn ) {
			check[0] = buf[0];
			check[1] = buf[1];
		}
	}

	return 0;
}



int CDInputSub::Myvd_chek(BYTE* date)
{
	BYTE bcd_smd[6] = { 0 };
	BYTE bcd_emd[6] = { 0 };
	BYTE chk[6] = { 0 };
	int ret = -1;
	int ks_sgn = 0;

	if( minfo == NULL )
		return -1;

	ASSERT((date[1] > 0x00 && date[1] <= 0x31) || date[1] == (BYTE)0xff);

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

	if( vd.db_vd_check(date, m_pZm) == 0 )
		ret = 0;


	return ret;
}

// 月日をBCDで返送
//	date[0] = 0x80->決修, 0x60 中間, 0x00 
//
//  dst_date[0],[1] には 通常の日付をセット
//	dst_date[0] ... D7-D5 bit が 0 = 通常, 1 = 決修, 2 = 中間
// 
int CDInputSub::Myvd_sign(BYTE* date, BYTE* dst_date)
{
	if( minfo->GetRecordCount() > 14 )
		return ::Myvd_sign(date, dst_date, m_pZm);
	else {
		int ret = -1;

		BYTE work[6] = { 0 };
		BYTE smd[4] = { 0 };
		BYTE emd[4] = { 0 };
		BYTE sgn = 0;

		ASSERT((date[1] > 0x00 && date[1] <= 0x31) || date[1] == (BYTE)0xff);

		if( *date == 0x00 ) {	//決修
			minfo->MoveFirst();
			do {
				if( minfo->ofset == 13 ) {
					DateConv(smd, (int*)&minfo->smd, CV2_1);
					DateConv(emd, (int*)&minfo->emd, CV2_1);

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
						memcpy(work, emd, 2);
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
					DateConv(smd, (int*)&minfo->smd, CV2_1);
					DateConv(emd, (int*)&minfo->emd, CV2_1);

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
						memcpy(work, emd, 2);
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

				DateConv(work, (int*)&minfo->emd, CV2_1);
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
					yy = minfo->smd / 10000;
					mm = (minfo->smd % 10000) / 100;
					if( mm == 2 ) {
						// うるう年のチェック
						if( !uru_year(yy) )
							return -1;

						break;
					}
				} while( minfo->MoveNext() == 0 );
			}
			memcpy(dst_date, work, 2);

			KESY_SG_SET(dst_date[0], sgn);
			ret = 0;
		}

		return ret;
	}

}



int	CDInputSub::IsKobetsuBmnSyz()
{
	int ret = 0;

	if( zvol->s_sgn4 & 0x80 ) {
		if( (zvol->sub_sw & 0x02) && (zvol->s_sgn2 & 0x20) ) {
			ret = 1;
		}
	}

	return ret;
}

int	CDInputSub::KobetsuSiwakeCheck(CDBipDataRec* rec, int bmn)
{
	CDBINPDataRec newdata;
	convNewDataRec(rec, &newdata);
	return KobetsuSiwakeCheck(&newdata, bmn);
}
int	CDInputSub::KobetsuSiwakeCheck(CDBINPDataRec* rec, int bmn)
{
	DWORD dwCode = 0;
	BYTE	dsign;

	if( bmn == -1 )	return 0;

	if( (rec->m_dsign[0] & 0x40) && (rec->m_dsign[2] & 0x0f) == 0x01 ) {
		dsign = (rec->m_dsign[3] & 0xf0);

		//MyTrace("KobetsuSiwakeCheck dsign3 = %02x\n", dsign);

		if( dsign != 0 ) {
			dsign >>= 4;
#if _MSC_VER >= 1900
			auto it = m_bmnMap.find(bmn);
			if( it != m_bmnMap.end() ) {
				if( it->second.bnsysgn != 0 ) {
					//	if (dsign != it->second.bnsysgn) {
					switch( it->second.bnsysgn ) {
					case 1:	dwCode = CD_KAURI;	break;
					case 2:	dwCode = CD_HIURI;	break;
					case 3:	dwCode = CD_KYOURI;	break;
					}
					//	}
				}
				else {
					switch( (zvol->s_sgn6 & 0x0f) ) {
					case 1:	dwCode = CD_KAURI;	break;
					case 2:	dwCode = CD_HIURI;	break;
					case 3:	dwCode = CD_KYOURI;	break;
					}
				}
			}
			else {
				switch( (zvol->s_sgn6 & 0x0f) ) {
				case 1:	dwCode = CD_KAURI;	break;
				case 2:	dwCode = CD_HIURI;	break;
				case 3:	dwCode = CD_KYOURI;	break;
				}
			}
#endif
		}
	}
	//MyTrace("KobetsuSiwakeCheck ret(1) = %08x\n", dwCode);

	if( dwCode != 0 )	return dwCode;
	else {
		if( zvol->apno < 0x50 ) {
			return 0;
		}
		//不課税の特定収入か？
		if( IsTokuteiSyunyuData(rec) ) {
			dsign = (rec->m_dsign[5] & 0xf0);

			//MyTrace("KobetsuSiwakeCheck 不課税 dsign5 = %02x\n", dsign);

			if( dsign != 0 ) {
				dsign >>= 4;
#if _MSC_VER >= 1900
				auto it = m_bmnMap.find(bmn);

				if( it != m_bmnMap.end() ) {
					if( it->second.bnsysgn != 0 ) {
						//	if (dsign != it->second.bnsysgn) {
						switch( it->second.bnsysgn ) {
						case 1:	dwCode = CD_TK_KAURI;	break;
						case 2:	dwCode = CD_TK_HIURI;	break;
						case 3:	dwCode = CD_TK_KYOURI;	break;
						}
						//	}
					}
					else {
						switch( (zvol->s_sgn6 & 0x0f) ) {
						case 1:	dwCode = CD_TK_KAURI;	break;
						case 2:	dwCode = CD_TK_HIURI;	break;
						case 3:	dwCode = CD_TK_KYOURI;	break;
						}
					}
				}
				else {
					switch( (zvol->s_sgn6 & 0x0f) ) {
					case 1:	dwCode = CD_TK_KAURI;	break;
					case 2:	dwCode = CD_TK_HIURI;	break;
					case 3:	dwCode = CD_TK_KYOURI;	break;
					}
				}
#endif
			}
		}
	}
	//MyTrace("KobetsuSiwakeCheck  ret(2) = %08x\n", dwCode);

	return dwCode;
}

//不課税の特定収入か？
BOOL CDInputSub::IsTokuteiSyunyuData(CDBipDataRec* rec)
{
	CDBINPDataRec newdata;
	convNewDataRec(rec, &newdata);
	return IsTokuteiSyunyuData(&newdata);
}

BOOL CDInputSub::IsTokuteiSyunyuData(CDBINPDataRec* rec)
{
	if( zvol->apno < 0x50 ) {
		return FALSE;
	}
	BOOL ret = FALSE;

	//不課税の特定収入か？
	if( (rec->m_dsign[0] & 0x40) && (rec->m_dsign[2] & 0x0f) == 0x02 ) {
		switch( (rec->m_dsign[5] & 0x0f) ) {
		case 1:	case 4:
		case 6:	case 8:
		case 10:
			ret = TRUE;
			break;
		}
	}
	return ret;
}


//消費税マスタータイプ返送
int CDInputSub::SyzeiMasterType()
{
	int ret;

	if( m_pSy->IsSyohizeiMaster() )
	{
		if( m_pSy->IsKaniMaster() )
			ret = KANI_SYMST;
		else if( m_pSy->IsKobetsuMaster() )
			ret = KOBET_SYMST;
		else
			ret = NORMAL_SYMST;
	}
	else
		ret = NORMAL_MST;

	return ret;
}



// 変動事由コードより名称を取得
void CDInputSub::get_hjiyustr(BYTE h_code, CString& str)
{
	m_pZm->kshnam->MoveFirst();

	do {
		if( m_pZm->kshnam->kofs > 0 )
			break;

		if( m_pZm->kshnam->hnjcod == h_code && (m_pZm->kshnam->hnpflg & 0x01) ) {
			str = m_pZm->kshnam->hnname;
			break;
		}
	} while( m_pZm->kshnam->MoveNext() == 0 );
}


//--------------------------------------------------------------
//	変動事由コードエリアより 名称を取得
//	[マスターによって 変動事由名称か、振替科目区分か]
//
//--------------------------------------------------------------
BOOL CDInputSub::GetHjiyuEtcStr(int hjcd, int nbcd, CString& str)
{
	BOOL ret = FALSE;

	if( m_pZm->zvol->apno < 0x50 ) {
		str.Empty();
		get_hjiyustr(hjcd, str);
		if( !str.IsEmpty() )	ret = TRUE;
	}
	else {
		if( !IsSyafuKaisei(m_pZm->zvol) ) {
			if( m_Seldata.GetNPfuriStr(hjcd, str) ) {
				ret = TRUE;
			}
		}
		else {
			if( m_Seldata.GetNaibuString(hjcd, nbcd, str) ) {
				ret = TRUE;
			}
		}
	}

	return ret;
}


//--------------------------------------------------------------
//	工事名称を取得
//
//--------------------------------------------------------------
BOOL CDInputSub::_set_kojietc(struct _SET_KMKETC *sk)
{
	BOOL ret = FALSE;

	if( m_Seldata.GetKojiName(sk->kno, sk->etc, FALSE) ) {
		sk->etc_col = RGB_BLUE;
		ret = TRUE;
	}

	return ret;
}


//--------------------------------------------------------------
//	仕入科目か？
//	return  1	仕入科目である
//			0	仕入科目でない
//--------------------------------------------------------------
int CDInputSub::km_siire(DBKNREC* pKn)
{
	int sgn = sire_attr(pKn);

	return sgn;
}


// 消費税仕訳か？　(完成振替を除く)
BOOL CDInputSub::is_sotomensiwake(CDBDATA* dbdata)
{
	CDBipDataRec drec;

	GetCDBData(&drec, dbdata);

	return is_sotomensiwake(&drec);
}

BOOL CDInputSub::is_sotomensiwake(CDBipDataRec* data)
{
	CDBINPDataRec newdata;
	convNewDataRec(data, &newdata);
	return is_sotomensiwake(&newdata);
}

BOOL CDInputSub::is_sotomensiwake(CDBINPDataRec* data)
{
	BOOL ret = FALSE;

	if( ! IsMasterType(MST_SYZ_INV) )
		return FALSE;

	// 外税仕訳か？
	if( _sotozei_data(&data->m_dsign[0]) ) {
		if( data->m_dsign[6] & 0x20 ) {
			ret = TRUE;
		}
	}
	return ret;
}


// 仮払消費税？
BOOL CDInputSub::IsKaribarai(DWORD kncd)
{
	BOOL bRet = FALSE;

	DWORD	svkcd1;
	svkcd1 = _atoh((LPSTR)(LPCTSTR)m_pZm->szvol->SVkcod1);

	if( (kncd & 0xffffff00) == (svkcd1 & 0xffffff00) ) {
		bRet = TRUE;
	}

	return bRet;
}


// 仮受消費税？
BOOL CDInputSub::IsKariuke(DWORD kncd)
{
	BOOL bRet = FALSE;

	DWORD	svkcd2;
	svkcd2 = _atoh((LPSTR)(LPCTSTR)m_pZm->szvol->SVkcod2);

	if( (kncd & 0xffffff00) == (svkcd2 & 0xffffff00) ) {
		bRet = TRUE;
	}

	return bRet;
}


// 登録番号をセットできる仕訳か？
BOOL CDInputSub::is_invnosiwake(CDBipDataRec* prec)
{
	CDBINPDataRec newdata;
	convNewDataRec(prec, &newdata);

	BOOL ret = is_invnosiwake(&newdata);
	//引数のレコードの登録番号をクリアする
	if( !ret ) {
		::ZeroMemory(prec->m_invno, sizeof prec->m_invno);
	}
	return ret;
}

// 登録番号をセットできる仕訳か？
BOOL CDInputSub::is_invnosiwake(CDBINPDataRec* prec)
{
	//
	BOOL bClear = FALSE;

	DBKNREC* pKn1, *pKn2;
	pKn1 = DB_PjisToKnrec(prec->m_dbt, m_pZm, TRUE);
	pKn2 = DB_PjisToKnrec(prec->m_cre, m_pZm, TRUE);
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
		switch( prec->m_dsign[4] ) { //仮払、以下の消費税区分は登録番号を付与しない
		case 0x04: case 0x0A: case 0x0B: //輸入仕入、棚卸加算・減算
		case 0x1C:						 //精算
		case 0x20: case 0x21: case 0x22: //輸入(国税), 輸入(地方税), 輸入(国地合算),
			bClear = TRUE;
			break;
		default:
			return TRUE;
		}
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


bool CDInputSub::CheckTkrec(_SY_TKREC_ *syrec)
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
	if( IsKani() ) {
		if( (syrec->tk_dsign[3] & 0x0f) > 0 ) {
			bRet = true;
		}
	}
	else if( IsKobetsu() ) {
		if( (syrec->tk_dsign[4] & 0x0f) > 0 ) {
			bRet = true;
		}
	}

	return bRet;
}



//仮払科目の場合で、免税がつけれられる仕訳区分か？
BOOL  CDInputSub::is_karibarai_menzei(BYTE* dsign)
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