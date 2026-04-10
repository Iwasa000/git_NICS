#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HINSTANCE syzMakeInstance;

//-----------------------------------------------------------------------------
// 消費税集計テーブル内の不正仕訳チェック
//-----------------------------------------------------------------------------
// 引数	pDatabase	：	チェック対象のマスターDB
//		isKani		：	簡易課税？
//-----------------------------------------------------------------------------
// 返送値	-1以外	：	不正仕訳の個数
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::ChkSzitmInSzsyk( CDatabase *pDatabase, BOOL isKani )
{
	ASSERT( pDatabase );
	if( pDatabase == NULL ){
		m_ErrMsg = _T("不正な引数が指定されました");
		return -1;
	}
	if( isKani == FALSE ){
		return 0;
	}

	CString		strCnt;
	CRecordset	dbrec( pDatabase );
	try {
//-- '15.02.24 --
//		CString strSql = _T("select count(*) as cnt from szsyk where (szitm&0x0f) >= 6");
//---------------
		CString strSql = _T("select count(*) as cnt from szsyk where (szitm&0x0f) >= 7");
//---------------
		dbrec.Open( CRecordset::forwardOnly, strSql );
		while( dbrec.IsEOF() == FALSE ){
			dbrec.GetFieldValue( _T("cnt"), strCnt );
			dbrec.MoveNext();
		}

	}catch( CDBException *pDbe ){
		m_ErrMsg = pDbe->m_strError;
		pDbe->Delete();
		return -1;
	}
	dbrec.Close();

	return atoi(strCnt);
}

//-----------------------------------------------------------------------------
// 消費税集計テーブル内の不正仕訳チェック
//-----------------------------------------------------------------------------
// 引数	pDatabase	：	チェック対象のマスターDB
//		pStYymmdd	：	開始年月日
//		pEdYymmdd	：	終了年月日
//-----------------------------------------------------------------------------
// 返送値	0		：	正常（不正仕訳無し）
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::CheckWrongData( CDBZmSub *pZmsub, long StYymmdd, long EdYymmdd )
{
	ASSERT( pZmsub );
	ASSERT( pZmsub->m_database );
	if( pZmsub->m_database == NULL ){
		m_ErrMsg = _T("財務マスターがオープンされていません！");
		return -1;
	}

	int	rt = 0;

	// 消費税バージョン
	int	s_ver = 0;
	int m_type = 0;
	if( GetZvolumeSverMtype(pZmsub->m_database, s_ver, m_type) ){
		return -1;
	}

	// チェック用の最終日
	long chkEdYymmdd = EdYymmdd;

	// 税率８％の仕訳有り！
	// -> チェック対象の税率をマスターバージョンによって切り分ける
	int		st = 0;
	int		mzst = 0;
	bool	isInvchk = false;
	if( (m_type&0x01) == 0x01 ){
		// 合併マスターの時には無条件に残高チェック！
		if( PER10_SYZ_YMD > StYymmdd ){
			st = 1;
			if( chkEdYymmdd > BF_PER10_LST_YMD ){
				chkEdYymmdd = BF_PER10_LST_YMD;
			}
		}
		else if( INVOICE_SYZ_YMD > StYymmdd ){
			mzst = st = 1;
			isInvchk = true;
			if( chkEdYymmdd > BF_INVOICE_LST_YMD ){
				chkEdYymmdd = BF_INVOICE_LST_YMD;
			}
		}
	}
	else{
		if( PER10_SYZ_VER <= s_ver ){

			if( PER10_SYZ_YMD<=StYymmdd ){
					// 10%改正の仕訳があっても良い期間なのでチェックしない
				;
			}
			else{
				if( chkEdYymmdd >= PER10_SYZ_YMD ){
					chkEdYymmdd = BF_PER10_LST_YMD;
				}

				st = ChkInputZei10Data(pZmsub->m_database, StYymmdd, chkEdYymmdd);
				if( st == 0 ){
					st = ChkInputZei8RdData(pZmsub->m_database, StYymmdd, chkEdYymmdd);
				}
				if( st == 0 ){
					chkEdYymmdd = EdYymmdd;
				}
			}

			if( st == 0 ){
				if( INVOICE_SYZ_YMD <= StYymmdd ){
					// 免税事業者からの仕入があっても良い期間なのでチェックしない
					;
				}
				else{
					if( chkEdYymmdd > BF_INVOICE_LST_YMD ){
						chkEdYymmdd = BF_INVOICE_LST_YMD;
					}
					st = ChkInputZeiMzSrData(pZmsub->m_database, StYymmdd, chkEdYymmdd);
					mzst = st;
					if( mzst==1 ){
						isInvchk = true;
					}
				}
			}
		}
		else if( PER8_SYZ_VER <= s_ver ){
			if( PER8_SYZ_YMD <= StYymmdd ){
				// 8%の仕訳があっても良い期間なのでチェックしない
				;
			}
			else{
				st = ChkInputZei8Data(pZmsub->m_database, StYymmdd, chkEdYymmdd);
			}
		}
	}
	if( st == -1 ){
		return -1;
	}
	else if( st == 1 ){
		// 該当月取得
		BOOL	openFlg = FALSE;
		CString	filter;
		if( pZmsub->minfo == NULL ){
			if( pZmsub->MinfoOpen() ){
				return -1;
			}
			openFlg = TRUE;
		}
		else{
			pZmsub->minfo->Requery( filter );
		}
		int	mofs = -1;
		for( int i=0;; i++ ){
			if( i==0 ){
				if( (pZmsub->minfo->MoveFirst()==-1) || (pZmsub->minfo->st==-1) ){
					break;
				}
			}
			else{
				if( (pZmsub->minfo->MoveNext()==-1) || (pZmsub->minfo->st==-1) ){
					break;
				}
			}
			if( pZmsub->minfo->emd > chkEdYymmdd ){
				break;
			}
			else{
				mofs = pZmsub->minfo->ofset;
			}
		}

		if( openFlg ){
			pZmsub->MinfoClose();
			openFlg = FALSE;
		}

		// 仮受・仮払消費税
		if( pZmsub->szvol == NULL ){
			if( pZmsub->SyohizeiOpen() ){
				return -1;
			}
			openFlg = TRUE;
		}
		CString	strKribaraiKcd, strKriukeKcd;
		strKribaraiKcd = pZmsub->szvol->SVkcod1;
		strKriukeKcd = pZmsub->szvol->SVkcod2;

		if( PER10_SYZ_VER <= s_ver ){
			if( mzst == 1 ){
				filter.Format(_T("SZzchg&0x10 = 0x10"));
				if( (m_type&0x01) == 0x01 ){
					mzst = 0;
				}
			}
			else{
				filter.Format(_T("SZzkbn = 4 or SZzkbn = 5"));
			}
		}
		else if( PER8_SYZ_VER <= s_ver ){
			filter.Format(_T("SZzkbn = 3"));
		}
		if( pZmsub->szsyk == NULL ){
			pZmsub->SzsykOpen( filter );
			openFlg = TRUE;
		}
		else{
			pZmsub->szsyk->Requery( filter );
		}

		BOOL	notZeroFlg = FALSE;
		CArith	arith(0x16);
		char	mony[6]={0}, summony[6]={0};
		char	komiz[6]={0}, nukiz[6]={0};
		char	bekki[6]={0}, taig[6]={0};

		for( int i=0;; i++ ){
			if( i==0 ){
				if( (pZmsub->szsyk->MoveFirst()==-1) || (pZmsub->szsyk->st==-1) ){
					break;
				}
			}
			else{
				if( (pZmsub->szsyk->MoveNext()==-1) || (pZmsub->szsyk->st==-1) ){
					break;
				}
			}

			arith.l_clear( komiz );
			arith.l_clear( nukiz );
			arith.l_clear( mony );
			arith.l_clear( summony );
			arith.l_clear( bekki );
			arith.l_clear( taig );

			if( (pZmsub->szsyk->SZkcod==strKribaraiKcd) || (pZmsub->szsyk->SZkcod==strKriukeKcd) ){
				for( int j=0; j<mofs; j++ ){
					arith.l_input( mony, pZmsub->szsyk->SZtaig[j].GetBuffer() );
					arith.l_add( taig, taig, mony );
				}
			}
			else{
				for( int j=0; j<=mofs; j++ ){
					arith.l_input( komiz, pZmsub->szsyk->SZkomz[j].GetBuffer() );
					arith.l_add( summony, summony, komiz );
					arith.l_input( nukiz, pZmsub->szsyk->SZnukz[j].GetBuffer() );
					arith.l_add( summony, summony, nukiz );

					arith.l_input( mony, pZmsub->szsyk->SZbeki[j].GetBuffer() );
					arith.l_add( bekki, bekki, mony );
				}
			}

			if( arith.l_test(summony) ){
				notZeroFlg = TRUE;
				break;
			}
			else if( arith.l_test(bekki) ){
				notZeroFlg = TRUE;
				break;
			}
			else if( arith.l_test(taig) ){
				notZeroFlg = TRUE;
				break;
			}
		}

		if( openFlg ){
			pZmsub->SzsykClose();
			openFlg = FALSE;
		}

		if( notZeroFlg || (mzst==1) ){
			if( PER10_SYZ_VER <= s_ver ){
				if( isInvchk ){
					if( (m_type&0x01) == 0x01 ){
						//m_ErrMsg.Format(_T("令和5年9月30日以前に免税事業者等からの課税仕入の残高が発生しています。\n処理を中断して、残高を確認しますか？"));
						m_ErrMsg.Format(_T("令和5年9月30日以前に免税事業者等からの課税仕入の残高が発生しています。\n残高を確認してください。\n処理を継続しますか？"));
					}
					else{
						//m_ErrMsg.Format(_T("令和5年9月30日以前に免税事業者等からの課税仕入の仕訳が入力されています。\n処理を中断して、入力済みの仕訳を確認しますか？"));
						m_ErrMsg.Format(_T("令和5年9月30日以前に免税事業者等からの課税仕入の仕訳が入力されています。\n入力済み仕訳を確認してください。\n処理を継続しますか？"));
					}
				}
				else{
					if( (m_type&0x01) == 0x01 ){
						m_ErrMsg.Format(_T("令和1年9月30日以前に税率10％または8％軽減の残高が発生しているため、処理できません。\n残高をご確認ください。"));
					}
					else{
						m_ErrMsg.Format(_T("令和1年9月30日以前に税率10％または8％軽減の仕訳が入力されているため、処理できません。\n入力済みの仕訳をご確認ください。"));
					}
				}
			}
			else if( PER8_SYZ_VER <= s_ver ){
				if( (m_type&0x01) == 0x01 ){
					m_ErrMsg.Format(_T("平成26年3月31日以前に税率８％の残高が発生しているため、処理できません。\n残高をご確認ください。"));
				}
				else{
					m_ErrMsg.Format(_T("平成26年3月31日以前に税率８％の仕訳が入力されているため、処理できません。\n入力済みの仕訳をご確認ください。"));
				}
			}
			rt = -1;
		}
	}

	return rt;
}

//-----------------------------------------------------------------------------
// インボイス施行日以降に残高発生有り？（szsyk テーブル基準）
//-----------------------------------------------------------------------------
// 引数	pZmsub	：	チェック対象に接続済のマスター
//-----------------------------------------------------------------------------
// 返送値		0	：	発生無し
//			1	：	発生有り
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::CheckZanAfterInvoice(CDBZmSub *pZmsub)
{
	//== 決算期間の情報を取得 ==
	bool	openflg = false;
	int		zvol_ss_ymd=0, zvol_ee_ymd=0;
	if( pZmsub->zvol == NULL ){
		if( pZmsub->VolumeOpen() ){
			return -1;
		}
		openflg = true;
	}
	zvol_ss_ymd = pZmsub->zvol->ss_ymd;
	zvol_ee_ymd = pZmsub->zvol->ee_ymd;
	if( openflg ){
		pZmsub->VolumeClose();
		openflg = false;
	}

	//== チェック対象の期間を取得 ==
	int StYmd=0, EdYmd=0;
	if( zvol_ee_ymd < INVOICE_SYZ_YMD ){
		// 完全に以前のマスターなので、残高発生のしようがない
		return 0;
	}
	else if( INVOICE_SYZ_YMD <= zvol_ee_ymd ){
		if( zvol_ss_ymd < INVOICE_SYZ_YMD ){
			StYmd = INVOICE_SYZ_YMD;
			EdYmd = zvol_ee_ymd;
		}
		else{
			StYmd = zvol_ss_ymd;
			EdYmd = zvol_ee_ymd;
		}
	}
	int StMofs=0, EdMofs=0;
	if( GetChkMonOfset(pZmsub, StYmd, EdYmd, StMofs, EdMofs) ){
		return -1;
	}

	//== 仮受・仮払の情報を取得 ==
	bool vlopenflg = false;
	if( pZmsub->szvol == NULL ){
		if( pZmsub->zvol == NULL ){
			if( pZmsub->VolumeOpen() ){
				return -1;
			}
			vlopenflg = true;
		}
		if( pZmsub->SyohizeiOpen() ){
			return -1;
		}
		openflg = true;
	}
	CString	strKribaraiKcd, strKriukeKcd;
	strKribaraiKcd = pZmsub->szvol->SVkcod1;
	strKriukeKcd = pZmsub->szvol->SVkcod2;
	if( openflg ){
		pZmsub->SyohizeiClose();
		openflg = false;
	}
	if( vlopenflg ){
		pZmsub->VolumeClose();
		vlopenflg = false;
	}

	//== 期間内の残高のチェック ==
	if( pZmsub->szsyk == NULL ){
		pZmsub->SzsykOpen(_T(""));
		openflg = true;
	}
	else{
		pZmsub->szsyk->Requery(_T(""));
	}

	bool	notZeroFlg = false;
	CArith	arith(0x16);
	char	mony[6]={ 0 }, summony[6]={ 0 };
	char	komiz[6]={ 0 }, nukiz[6]={ 0 };
	char	bekki[6]={ 0 }, taig[6]={ 0 };

	for( int i=0;; i++ ){
		if( i==0 ){
			if( (pZmsub->szsyk->MoveFirst()==-1) || (pZmsub->szsyk->st==-1) ){
				break;
			}
		}
		else{
			if( (pZmsub->szsyk->MoveNext()==-1) || (pZmsub->szsyk->st==-1) ){
				break;
			}
		}

		arith.l_clear(komiz);
		arith.l_clear(nukiz);
		arith.l_clear(mony);
		arith.l_clear(summony);
		arith.l_clear(bekki);
		arith.l_clear(taig);

		if( (pZmsub->szsyk->SZkcod==strKribaraiKcd) || (pZmsub->szsyk->SZkcod==strKriukeKcd) ){
			for( int j=StMofs; j<=EdMofs; j++ ){
				arith.l_input(mony, pZmsub->szsyk->SZtaig[j].GetBuffer());
				arith.l_add(taig, taig, mony);
			}
		}
		else{
			for( int j=StMofs; j<=EdMofs; j++ ){
				arith.l_input(komiz, pZmsub->szsyk->SZkomz[j].GetBuffer());
				arith.l_add(summony, summony, komiz);
				arith.l_input(nukiz, pZmsub->szsyk->SZnukz[j].GetBuffer());
				arith.l_add(summony, summony, nukiz);

				arith.l_input(mony, pZmsub->szsyk->SZbeki[j].GetBuffer());
				arith.l_add(bekki, bekki, mony);
			}
		}

		if( arith.l_test(summony) ){
			notZeroFlg = true;
			break;
		}
		else if( arith.l_test(bekki) ){
			notZeroFlg = true;
			break;
		}
		else if( arith.l_test(taig) ){
			notZeroFlg = true;
			break;
		}
	}

	if( openflg ){
		pZmsub->SzsykClose();
		openflg = false;
	}

	if( notZeroFlg ){
		return 1;
	}
	else{
		return 0;
	}
}

//-----------------------------------------------------------------------------
// 簡易税不正仕訳のチェック('15.02.23)
//-----------------------------------------------------------------------------
// 引数	pZmsub		：	チェック対象の財務クラス
//		StYymmdd	：	開始年月日
//		EdYymmdd	：	終了年月日
//-----------------------------------------------------------------------------
// 返送値	-1以外	：	不正仕訳の個数
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::CheckWrongKaniData( CDBZmSub *pZmsub, long StYymmdd, long EdYymmdd )
{
	ASSERT( pZmsub );
	if( pZmsub == NULL ){
		return -1;
	}
	ASSERT( pZmsub->m_database );
	if( pZmsub->m_database == NULL ){
		return -1;
	}

	int st = ChkInputZeiKani6Data( pZmsub->m_database, StYymmdd, EdYymmdd );
	if( st == -1 ){
		return -1;
	}

	return st;
}

//-----------------------------------------------------------------------------
// 特定収入不正仕訳のチェック('15.03.11)
//-----------------------------------------------------------------------------
// 引数	pZmsub		：	チェック対象の財務クラス
//		StYymmdd	：	開始年月日
//		EdYymmdd	：	終了年月日
//-----------------------------------------------------------------------------
// 返送値	-1以外	：	不正仕訳の個数
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::CheckWrongSpcData( CDBZmSub *pZmsub, long StYymmdd, long EdYymmdd )
{
	ASSERT( pZmsub );
	if( pZmsub == NULL ){
		return -1;
	}
	ASSERT( pZmsub->m_database );
	if( pZmsub->m_database == NULL ){
		return -1;
	}

	int st = ChkInputZeiSpcData( pZmsub->m_database, StYymmdd, EdYymmdd );
	if( st == -1 ){
		return -1;
	}

	return st;
}

//-----------------------------------------------------------------------------
// 当期の不正月別情報の修正('20.08.28)
//   InitMonthInfo からの流用
//-----------------------------------------------------------------------------
// 引数	pZmsub		：	修正対象の財務クラス
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::RepairMonthInfo( CDBZmSub *pZmsub )
{
	int rt = 0;

	ASSERT(pZmsub);
	if( pZmsub == NULL ){
		return -1;
	}

	int	openFlg=0x00;	// □□□□ □□□□ □□□□ □□□□ □□□□ □□□□ □□□□ □□■■
						// D0:ボリュームラベル, D1:月別情報
	if( pZmsub->zvol == NULL ){
		if( pZmsub->VolumeOpen() ){
			return -1;
		}
		openFlg |= 0x01;
	}
	if( pZmsub->minfo == NULL ){
		if( pZmsub->MinfoOpen() ){
			return -1;
		}
		openFlg |= 0x02;
	}

	// 期首年月日と一ヶ月目の開始日のチェック
	bool bIsNeedRepair = false;
	if( pZmsub->zvol->ss_ymd != pZmsub->minfo->smd ){
		bIsNeedRepair = true;
	}

	while( bIsNeedRepair ){
		// 締め日
		int tmpSime = pZmsub->zvol->ee_ymd%100;

		pZmsub->minfo->MoveFirst();
		BYTE m, d;
		int_bcd(&m, ((pZmsub->minfo->emd/100)%100), 1);
		int_bcd(&d, (pZmsub->minfo->emd%100), 1);
		BYTE day = _vd_geteday(m);	// 該当月末日取得
		if( d >= day ){	// 月末
			day = 0;
		}
		else{			// 中間
			day = d;
		}
		tmpSime = bcd_int(&day, 1);	// 締日取得

		// 決算期間より月別情報



		// 期首・期末・計算上期首年月日（西暦→和暦）
		int ssymd=0, eeymd=0, csymd=0, ssgengo=0, eegegno=0, csgengo=0;
		m_VolDate.db_datecnvGen(0, pZmsub->zvol->ss_ymd, &ssgengo, &ssymd, 0, 0);
		m_VolDate.db_datecnvGen(0, pZmsub->zvol->ee_ymd, &eegegno, &eeymd, 0, 0);
		m_VolDate.db_datecnvGen(0, pZmsub->zvol->ss_ymd, &csgengo, &csymd, 0, 0);	// 計算上の期首年月日は、通常の期首年月日と同値として扱う

		UCHAR	bcd_ssymd[3]={ 0 }, bcd_eeymd[3]={ 0 }, bcd_csymd[3]={ 0 };
		int_bcd(bcd_ssymd, ssymd, 3);
		int_bcd(bcd_eeymd, eeymd, 3);
		int_bcd(bcd_csymd, csymd, 3);

//		DAPACGEN_BCD	tmpDapac[MONTHALL*5];	// 開始・終了月日（当期～４期前）
		if( m_VolDate.db_datesetexGen(ssgengo, bcd_ssymd, eegegno, bcd_eeymd, csgengo, bcd_csymd, tmpSime, m_Dapac) < 0 ){
			rt = -1;
			break;;
		}

		// 期間情報をセット（当期分のみ）
		for( int i=0; i<MONTHALL; i++ ){
			m_MSgengo[i] = m_Dapac[i].Sgengo;
			memmove(m_MSymd[i], m_Dapac[i].Symd, 3);
			m_MEgengo[i] = m_Dapac[i].Egengo;
			memmove(m_MEymd[i], m_Dapac[i].Eymd, 3);
		}

		// 決算修正サイン更新
		// この内部は別途実装する必要性あり
		m_mkZVolume.ss_ymd = pZmsub->zvol->ss_ymd;
		m_mkZVolume.ee_ymd = pZmsub->zvol->ee_ymd;
		m_mkZVolume.cs_ymd = pZmsub->zvol->ss_ymd;
		UpdateKIsgn();

		// 実テーブル修正
		try{
			char tmpDbSvrName[128]={ 0 };
			PCOM_GetString(_T("SelectSqlServer"), tmpDbSvrName);

			pZmsub->m_database->BeginTrans();
			pZmsub->m_database->ExecuteSQL(_T("delete from dbo.month_info"));

			char	buf[512] ={ 0 };
			CString	str;
			for( int i=0; i<MONTHALL; i++ ){
				// 引数を変更して実装
				memset(buf, '\0', sizeof(buf));
				MakeMonthInfoDataSql(i, buf, tmpDbSvrName);
//				str.Format(_T("INSERT INTO [%s].[dbo].[%s] VALUES (%s)"), m_mkCoName, _T("month_info"), buf);
				str.Format(_T("INSERT INTO [dbo].[%s] VALUES (%s)"), _T("month_info"), buf);
				pZmsub->m_database->ExecuteSQL(str);
			}
		}
		catch( CDBException* dbe ){
			pZmsub->m_database->Rollback();
			dbe->Delete();
			rt = -1;
		}
		pZmsub->m_database->CommitTrans();

		break;
	}

	// 後処理
	if( openFlg&0x01 ){
		pZmsub->VolumeClose();
	}
	if( openFlg&0x02 ){
		pZmsub->MinfoClose();
	}

	return rt;
}

//-----------------------------------------------------------------------------
// 前期以前の不正月別情報の修正('16.12.19)
//-----------------------------------------------------------------------------
// 引数	pZmsub		：	修正対象の財務クラス
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::RepairZmonthInfo( CDBZmSub *pZmsub )
{
	int rt = 0;

	ASSERT( pZmsub );
	if( pZmsub == NULL ){
		return -1;
	}

	int	openFlg=0x00;	// □□□□ □□□□ □□□□ □□□□ □□□□ □□□□ □□□□ □□■■
						// D0:ボリュームラベル, D1:月別情報
	if( pZmsub->zvol == NULL ){
		if( pZmsub->VolumeOpen() ){
			return -1;
		}
		openFlg |= 0x01;
	}
	if( pZmsub->minfo == NULL ){
		if( pZmsub->MinfoOpen() ){
			return -1;
		}
		openFlg |= 0x02;
	}

	// 締め日
	int tmpSime = pZmsub->zvol->ee_ymd%100;

	pZmsub->minfo->MoveFirst();
	BYTE m, d;
	int_bcd( &m, ((pZmsub->minfo->emd/100)%100), 1 );
	int_bcd( &d, (pZmsub->minfo->emd%100), 1 );
	BYTE day = _vd_geteday( m );	// 該当月末日取得
	if( d >= day ){	// 月末
		day = 0;
	}
	else{			// 中間
		day = d;
	}
	tmpSime = bcd_int( &day, 1 );	// 締日取得

	// 会社名作成
	UCHAR	tmpSymd[4]={0};
	UCHAR	tmpEymd[4]={0};
	int_bcd( tmpSymd, pZmsub->zvol->ss_ymd, 4 );
	int_bcd( tmpEymd, pZmsub->zvol->ss_ymd, 4 );
	memset( m_mkCoName, '\0', sizeof(m_mkCoName) );
	sprintf_s( m_mkCoName, sizeof(m_mkCoName), _T("Z%02x_C%08d_Y%02x%02x"), pZmsub->zvol->apno, pZmsub->zvol->v_cod, tmpEymd[1], tmpEymd[2] );

	while( 1 ){
/*		// 期首・期末・計算上期首年月日 平成変換
		UCHAR ssymd[3] = {0}, eeymd[3] = {0}, csymd[3] = {0};
		UCHAR ssy[3] = {0}, eey[3] = {0};
		int_bcd( ssy, (pZmsub->zvol->ss_ymd%1000000), 3 );
		int_bcd( eey, (pZmsub->zvol->ee_ymd%1000000), 3 );
		int_bcd( ssymd, (pZmsub->zvol->ss_ymd%1000000), 3 );
		int_bcd( eeymd, (pZmsub->zvol->ee_ymd%1000000), 3 );
		int_bcd( csymd, (pZmsub->zvol->cs_ymd%1000000), 3 );

		m_VolDate.db_datecnv( ssymd, ssymd, 0, 0 );
		m_VolDate.db_datecnv( eeymd, eeymd, 0, 0 );
		m_VolDate.db_datecnv( csymd, csymd, 0, 0 );*/

		// 期首・期末・計算上期首年月日（西暦→和暦）
		int ssymd=0, eeymd=0, csymd=0, ssgengo=0, eegegno=0, csgengo=0;
		m_VolDate.db_datecnvGen( 0, pZmsub->zvol->ss_ymd, &ssgengo, &ssymd, 0, 0 );
		m_VolDate.db_datecnvGen( 0, pZmsub->zvol->ee_ymd, &eegegno, &eeymd, 0, 0 );
		m_VolDate.db_datecnvGen( 0, pZmsub->zvol->ss_ymd, &csgengo, &csymd, 0, 0 );//20.08.30 cs_ymdが減価償却更新などで更新されていないことがあるので、期首年月日を使う
//		m_VolDate.db_datecnvGen( 0, pZmsub->zvol->cs_ymd, &csgengo, &csymd, 0, 0 );

		UCHAR	bcd_ssymd[3]={0}, bcd_eeymd[3]={0}, bcd_csymd[3]={0};
		int_bcd( bcd_ssymd, ssymd, 3 );
		int_bcd( bcd_eeymd, eeymd, 3 );
		int_bcd( bcd_csymd, csymd, 3 );

/*		if( m_VolDate.db_datesetex(ssymd, eeymd, csymd, tmpSime, m_Dapac) < 0 ){
			rt = -1;
			break;
		}*/


		if( m_VolDate.db_datesetexGen(ssgengo, bcd_ssymd, eegegno, bcd_eeymd, csgengo, bcd_csymd, tmpSime, m_Dapac) < 0 ){
			return -1;
		}

		// 当期決算期間より前期決算期間を取得
/*		UCHAR pssymd[3] = { 0 }, peeymd[3] = { 0 };
		m_VolDate.vd_getprevdateex( peeymd, ssy );
		m_VolDate.vd_getsymd( pssymd, peeymd );*/

		// 当期決算期間より前期決算期間を取得（和暦）
		int pregengo=0;
		UCHAR pssymd[3]={0}, peeymd[3]={0}, tmpymd[3]={0};
		m_VolDate.vd_getprevdateexGen( &pregengo, tmpymd, ssgengo, bcd_ssymd );
		// 和暦→西暦
		int tmpgengo=0;
		m_VolDate.db_datecnvGen( pregengo, tmpymd, &tmpgengo, peeymd, 1, 0 );
		// 西暦で期末日より期首日を計算
		m_VolDate.vd_getsymd( pssymd, peeymd );

		// 前期月別情報取得
		for( int i=0; i<4; i++ ){
			if( i!=0 ){
				// 期首年取得
				if( pssymd[0] != 0x00 ){
					int y = bcd_int( &pssymd[0], 1 );
					y--;
					int_bcd( &pssymd[0], y, 1 );
				}
				else{
					pssymd[0] = 0x99;
				}
				// 期末年取得
				if( peeymd[0] != 0x00 ){
					int y = bcd_int( &peeymd[0], 1 );
					y--;
					int_bcd( &peeymd[0], y, 1 );
				}
				else{
					peeymd[0] = 0x99;
				}
			}

			m_VolDate.vd_getfebeday( pssymd );
			m_VolDate.vd_getfebeday( peeymd );
			
			// 西暦→和暦
			UCHAR pssy[3] = { 0 }, peey[3] = { 0 };
//			m_VolDate.db_datecnv( pssymd, pssy, 0, 0 );
//			m_VolDate.db_datecnv( peeymd, peey, 0, 0 );
			int	psgengo=0, pegengo=0;
			m_VolDate.db_datecnvGen( 0, pssymd, &psgengo, pssy, 0, 0 );
			m_VolDate.db_datecnvGen( 0, peeymd, &pegengo, peey, 0, 0 );

//			if( m_VolDate.db_dateset( pssy, peey, m_nSime, &m_Dapac[(i+1)*MONTHALL] ) < 0 ) {
			if( m_VolDate.db_datesetGen( psgengo, pssy, pegengo, peey, tmpSime, &m_Dapac[(i+1)*MONTHALL] ) < 0 ) {
				// 閏年チェック
				if( (pssy[1] == 0x2) && (pssy[2] == 0x29) ){
					pssy[2] = 0x28;
				}
				else if ( (peey[1] == 0x2) && (peey[2] == 0x29) ){
					peey[2] = 0x28;
				}
				else{
					rt = -1;
					break;
				}
//				if( m_VolDate.db_dateset( pssy, peey, tmpSime, &m_Dapac[(i+1)*MONTHALL] ) < 0 ){
				if( m_VolDate.db_datesetGen( psgengo, pssy, pegengo, peey, tmpSime, &m_Dapac[(i+1)*MONTHALL] ) < 0 ){
					rt = -1;
					break;
				}
			}
		}

		// 期間情報をセット
		for( int i=0; i<MONTHALL*5; i++ ){
			m_MSgengo[i] = m_Dapac[i].Sgengo;
			memmove( m_MSymd[i], m_Dapac[i].Symd, 3 );
			m_MEgengo[i] = m_Dapac[i].Egengo;
			memmove( m_MEymd[i], m_Dapac[i].Eymd, 3 );
		}

		SYZMAKE_MKCOINFO	mkcoinfo={0};
		PCOM_GetString( _T("SelectSqlServer"), mkcoinfo.dbSvrName );

		try{
			pZmsub->m_database->BeginTrans();

			pZmsub->m_database->ExecuteSQL( _T("delete from dbo.zmonth_info") );
		
			char buf[512] = { 0 };
			for( int j=0; j<4; j++ ){
				for( int k=0; k<TSCOUNT; k++ ){
					MakeZMonthInfoDataSql( j, k, buf, mkcoinfo );

					CString str;
					str.Format( _T("INSERT INTO [dbo].[zmonth_info] VALUES (%s)"), buf );

					pZmsub->m_database->ExecuteSQL( str );

					memset( buf, '\0', sizeof(buf) );
				}
			}
		}
		catch( CDBException* dbe ){
			pZmsub->m_database->Rollback();
			dbe->Delete();
			rt = -1;
			break;
		}
		pZmsub->m_database->CommitTrans();

		break;
	}

	if( openFlg&0x01 ){
		pZmsub->VolumeClose();
	}
	if( openFlg&0x02 ){
		pZmsub->MinfoClose();
	}

	return rt;
}

//-----------------------------------------------------------------------------
// 消費税集計テーブル内の不正仕訳チェック
//-----------------------------------------------------------------------------
// 引数	pDatabase	：	チェック対象のマスターDB
//		isKani		：	簡易課税？
//		pStYymmdd	：	開始年月日
//		pEdYymmdd	：	終了年月日
//-----------------------------------------------------------------------------
// 返送値	0		：	正常（不正仕訳無し）
//			1		：	正常（不正仕訳あり）
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::ChkInput3or5ImportData(CDBZmSub *pZmsub, long StYymmdd, long EdYymmdd)
{
	ASSERT(pZmsub);
	ASSERT(pZmsub->m_database);
	if( pZmsub->m_database == NULL ){
		m_ErrMsg = _T("財務マスターがオープンされていません！");
		return -1;
	}

	int	rt = 0;

	// 消費税バージョン
	int	s_ver = 0;
	int m_type = 0;
	if( GetZvolumeSverMtype(pZmsub->m_database, s_ver, m_type) ){
		return -1;
	}

	int		st = 0;
	bool	isImport = false;
	if( (m_type&0x01) == 0x01 ){
		// 合併マスターの時には無条件に残高チェック！
		st = 1;
	}
	else{
		st = ChkInput3or5ImportData(pZmsub->m_database, StYymmdd, EdYymmdd);
	}

	if( st == -1 ){
		return -1;
	}
	else if( st == 1 ){
		// 該当月取得
		int stmofs=0, edmofs=0;
		if( GetChkMonOfset(pZmsub, StYymmdd, EdYymmdd, stmofs, edmofs) ){
			return -1;
		}

		// 仮受・仮払消費税
		BOOL	openFlg = FALSE;
		if( pZmsub->szvol == NULL ){
			if( pZmsub->SyohizeiOpen() ){
				return -1;
			}
			openFlg = TRUE;
		}
		CString	strKribaraiKcd, strKriukeKcd;
		strKribaraiKcd = pZmsub->szvol->SVkcod1;
		strKriukeKcd = pZmsub->szvol->SVkcod2;

		CString	filter;
		filter.Format(_T("(((SZgrp1=4)and(SZgrp2=4)) or ((SZgrp1=4)and(SZgrp2=17)) or ((SZgrp1=4)and(SZgrp2=18)) or ((SZgrp1=4)and(SZgrp2=19))) and ((SZzkbn=0)or(SZzkbn=1))"));

		if( pZmsub->szsyk == NULL ){
			pZmsub->SzsykOpen(filter);
			openFlg = TRUE;
		}
		else{
			pZmsub->szsyk->Requery(filter);
		}

		BOOL	notZeroFlg = FALSE;
		CArith	arith(0x16);
		char	mony[6]={ 0 }, summony[6]={ 0 };
		char	komiz[6]={ 0 }, nukiz[6]={ 0 };
		char	bekki[6]={ 0 }, taig[6]={ 0 };

		for( int i=0;; i++ ){
			if( i==0 ){
				if( (pZmsub->szsyk->MoveFirst()==-1) || (pZmsub->szsyk->st==-1) ){
					break;
				}
			}
			else{
				if( (pZmsub->szsyk->MoveNext()==-1) || (pZmsub->szsyk->st==-1) ){
					break;
				}
			}

			arith.l_clear(komiz);
			arith.l_clear(nukiz);
			arith.l_clear(mony);
			arith.l_clear(summony);
			arith.l_clear(bekki);
			arith.l_clear(taig);

			if( (pZmsub->szsyk->SZkcod==strKribaraiKcd) || (pZmsub->szsyk->SZkcod==strKriukeKcd) ){
				for( int j=stmofs; j<=edmofs; j++ ){
					arith.l_input(mony, pZmsub->szsyk->SZtaig[j].GetBuffer());
					arith.l_add(taig, taig, mony);
				}
			}
			else{
				for( int j=stmofs; j<=edmofs; j++ ){
					arith.l_input(komiz, pZmsub->szsyk->SZkomz[j].GetBuffer());
					arith.l_add(summony, summony, komiz);
					arith.l_input(nukiz, pZmsub->szsyk->SZnukz[j].GetBuffer());
					arith.l_add(summony, summony, nukiz);

					arith.l_input(mony, pZmsub->szsyk->SZbeki[j].GetBuffer());
					arith.l_add(bekki, bekki, mony);
				}
			}

			if( arith.l_test(summony) ){
				notZeroFlg = TRUE;
				break;
			}
			else if( arith.l_test(bekki) ){
				notZeroFlg = TRUE;
				break;
			}
			else if( arith.l_test(taig) ){
				notZeroFlg = TRUE;
				break;
			}
		}

		if( openFlg ){
			pZmsub->SzsykClose();
			openFlg = FALSE;
		}

		if( notZeroFlg ){
			if( (m_type&0x01) == 0x01 ){
				m_ErrMsg.Format(_T("税率5％または3％の輸入取引の残高が発生しているため、処理できません。\n残高をご確認ください"));
			}
			else{
				m_ErrMsg.Format(_T("税率5％または3％の輸入取引仕訳があるため、処理できません。\n入力済みの仕訳をご確認ください"));
			}
			rt = 1;
		}
	}

	return rt;
}

//-----------------------------------------------------------------------------
// 別記仕訳のチェック
//-----------------------------------------------------------------------------
// 返送値	0		：	正常（不正仕訳無し）
//			1		：	正常（不正仕訳あり）
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::CheckSepParaData(CDBZmSub *pZmsub, long StYymmdd, long EdYymmdd)
{
	ASSERT(pZmsub);
	ASSERT(pZmsub->m_database);
	if( pZmsub->m_database == NULL ){
		m_ErrMsg = _T("財務マスターがオープンされていません！");
		return -1;
	}

	int	rt = 0;

	// 消費税バージョン
	int	s_ver = 0;
	int m_type = 0;
	if( GetZvolumeSverMtype(pZmsub->m_database, s_ver, m_type) ){
		return -1;
	}

	int		st1=0, st2=0;
	bool	isImport = false;
	if( (m_type&0x01) == 0x01 ){
		// 合併マスターの時には無条件に残高チェック！
		st1 = 1;
	}
	else{
		st1 = ChkInputSepParaData(pZmsub, StYymmdd, EdYymmdd);
		if( st1 == 0 ){
			st2 = ChkInputKariUBData(pZmsub, StYymmdd, EdYymmdd);
		}
	}

	if( (st1==-1) || (st2==-1) ){
		return -1;
	}
	else if( (st1==1) || (st2==1) ){
		// 該当月取得
		int stmofs=0, edmofs=0;
		if( GetChkMonOfset(pZmsub, StYymmdd, EdYymmdd, stmofs, edmofs) ){
			return -1;
		}

		// 仮受・仮払消費税
		CString	strKribaraiKcd, strKriukeKcd;
		if( GetKariUkeBaraiKcd(pZmsub, strKribaraiKcd, strKriukeKcd) ){
			return -1;
		}

		BOOL	openFlg = FALSE;
		CString	filter;
		filter.Format(_T("((SZgrp1!=4)or(SZgrp2!=4)) and ((SZgrp1!=4)or(SZgrp2!=17)) and ((SZgrp1!=4)or(SZgrp2!=18)) and ((SZgrp1!=4)or(SZgrp2!=19)) and ((SZgrp1!=4)or(SZgrp2!=7)) and ((SZgrp1!=2)or(SZgrp2!=10))"));
		if( pZmsub->szsyk == NULL ){
			pZmsub->SzsykOpen(filter);
			openFlg = TRUE;
		}
		else{
			pZmsub->szsyk->Requery(filter);
		}

		BOOL	notZeroFlg = FALSE;
		CArith	arith(0x16);
		char	bekki[6]={ 0 }, taig[6]={ 0 }, mony[6]={ 0 };

		for( int i=0;; i++ ){
			if( i==0 ){
				if( (pZmsub->szsyk->MoveFirst()==-1) || (pZmsub->szsyk->st==-1) ){
					break;
				}
			}
			else{
				if( (pZmsub->szsyk->MoveNext()==-1) || (pZmsub->szsyk->st==-1) ){
					break;
				}
			}

			arith.l_clear(bekki);
			arith.l_clear(taig);

			if( (pZmsub->szsyk->SZkcod==strKribaraiKcd) || (pZmsub->szsyk->SZkcod==strKriukeKcd) ){
				for( int j=stmofs; j<=edmofs; j++ ){
					arith.l_input(mony, pZmsub->szsyk->SZtaig[j].GetBuffer());
					arith.l_add(taig, taig, mony);
				}
			}
			else{
				for( int j=stmofs; j<=edmofs; j++ ){
					arith.l_input(mony, pZmsub->szsyk->SZbeki[j].GetBuffer());
					arith.l_add(bekki, bekki, mony);
				}
			}

			if( arith.l_test(bekki) ){
				notZeroFlg = TRUE;
				break;
			}
			else if( arith.l_test(taig) ){
				notZeroFlg = TRUE;
				break;
			}
		}

		if( openFlg ){
			pZmsub->SzsykClose();
			openFlg = FALSE;
		}

		if( notZeroFlg ){
			if( (m_type&0x01) == 0x01 ){
				m_ErrMsg.Format(_T("別記残高、もしくは仮受（仮払）消費税の残高が発生しています。\nこの場合の税額の特例計算には対応していません。\n\n別記残高を内税もしくは外税残高に変更していただくか、非連動処理にて申告書を作成してください。"));
			}
			else{
				m_ErrMsg.Format(_T("別記仕訳、もしくは仮受（仮払）消費税の仕訳が入力されています。\nこの場合の税額の特例計算には対応していません。\n\n別記仕訳を内税もしくは外税仕訳に変更していただくか、非連動処理にて申告書を作成してください。"));
			}
			rt = 1;
		}
	}

	return rt;
}

//-----------------------------------------------------------------------------
// 仮受・仮払消費税のコード取得
//-----------------------------------------------------------------------------
// 引数	pZmsub		：	取得対象の財務クラス
//		KribaraiKcd	：	仮払消費税の取得先
//		KriukeKcd	：	仮受消費税の取得先
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::GetKariUkeBaraiKcd(CDBZmSub *pZmsub, CString& KribaraiKcd, CString& KriukeKcd)
{
	BOOL	openFlg = FALSE;
	if( pZmsub->szvol == NULL ){
		if( pZmsub->SyohizeiOpen() ){
			return -1;
		}
		openFlg = TRUE;
	}
	KribaraiKcd = pZmsub->szvol->SVkcod1;
	KriukeKcd = pZmsub->szvol->SVkcod2;

	if( openFlg ){
		pZmsub->SyohizeiClose();
	}

	return 0;
}


//-----------------------------------------------------------------------------
// 該当月オフセットの取得
//-----------------------------------------------------------------------------
// 引数	pZmsub		：	チェック対象の財務クラス
//		chkStYymmdd	：	チェック開始年月日
//		chkEdYymmdd	：	チェック終了年月日
//		StMofs		：	チェック開始月オフセット
//		EdMofs		：	チェック終了月オフセット
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::GetChkMonOfset( CDBZmSub *pZmsub, int chkStYymmdd, int chkEdYymmdd, int& StMofs, int& EdMofs )
{
	StMofs = EdMofs = 0;

	BOOL	openFlg = FALSE;
	CString	filter = _T("");
	if( pZmsub->minfo == NULL ){
		if( pZmsub->MinfoOpen() ){
			return -1;
		}
		openFlg = TRUE;
	}
	else{
		pZmsub->minfo->Requery(filter);
	}
	int	stmofs=-1, edmofs=-1;
	for( int i=0;; i++ ){
		if( i==0 ){
			if( (pZmsub->minfo->MoveFirst()==-1) || (pZmsub->minfo->st==-1) ){
				break;
			}
		}
		else{
			if( (pZmsub->minfo->MoveNext()==-1) || (pZmsub->minfo->st==-1) ){
				break;
			}
		}

		if( (stmofs==-1) && (pZmsub->minfo->smd<=chkStYymmdd) && (chkStYymmdd<=pZmsub->minfo->emd) ){
			stmofs = pZmsub->minfo->ofset;
		}
		if( (pZmsub->minfo->smd<=chkEdYymmdd) && (chkEdYymmdd<=pZmsub->minfo->emd) ){
			edmofs = pZmsub->minfo->ofset;
		}
	}

	if( openFlg ){
		pZmsub->MinfoClose();
		openFlg = FALSE;
	}

	if( (stmofs==-1) || (edmofs==-1) ){
		if( pZmsub->zvol == NULL ){
			if( pZmsub->VolumeOpen() ){
				return -1;
			}
			openFlg = TRUE;
		}

		if( stmofs == -1 ){
			if( chkStYymmdd < pZmsub->zvol->ss_ymd ){
				stmofs = 0;
			}
			else if( pZmsub->zvol->ee_ymd < chkStYymmdd ){
				stmofs = 15;
			}
		}
		if( edmofs == -1 ){
			if( chkEdYymmdd < pZmsub->zvol->ss_ymd ){
				edmofs = -1;
			}
			else if( pZmsub->zvol->ee_ymd < chkEdYymmdd ){
				edmofs = 15;
			}
		}
		if( openFlg ){
			pZmsub->VolumeClose();
			openFlg = FALSE;
		}
	}

	StMofs = stmofs;
	EdMofs = edmofs;

	return 0;
}

//-----------------------------------------------------------------------------
// 免税事業者からの課税仕入 有無チェック ('23.03.29)
//-----------------------------------------------------------------------------
// 引数	pZmsub		：	チェック対象の財務クラス
//		StYymmdd	：	開始年月日
//		EdYymmdd	：	終了年月日
//-----------------------------------------------------------------------------
// 返送値		0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::CheckMzSrData(CDBZmSub *pZmsub, long StYymmdd, long EdYymmdd)
{
	ASSERT(pZmsub);
	if( pZmsub == NULL ){
		return -1;
	}
	ASSERT(pZmsub->m_database);
	if( pZmsub->m_database == NULL ){
		return -1;
	}

	int st = ChkInputZeiMzSrData(pZmsub->m_database, StYymmdd, EdYymmdd);
	if( st == -1 ){
		return -1;
	}

	return st;
}

//-----------------------------------------------------------------------------
// 免税事業者からの課税仕入不正仕訳のチェック('23.03.29)
//-----------------------------------------------------------------------------
// 引数	pZmsub		：	チェック対象の財務クラス
//-----------------------------------------------------------------------------
// 返送値		0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::CheckWrongMzSrData(CDBZmSub *pZmsub)
{
	ASSERT(pZmsub);
	if( pZmsub == NULL ){
		return -1;
	}
	ASSERT(pZmsub->m_database);
	if( pZmsub->m_database == NULL ){
		return -1;
	}

	int st = ChkInputZeiMzSrData(pZmsub->m_database, -1, BF_INVOICE_LST_YMD);
	if( st == -1 ){
		return -1;
	}
	else if( st == 1 ){
		m_ErrMsg.Format(_T("令和5年9月30日以前に免税事業者等からの課税仕入の仕訳が入力されているため、処理できません。\n入力済みの仕訳をご確認ください。"));
		st = -1;
	}

	return st;
}

//-----------------------------------------------------------------------------
// 消費税の集計済サインを落とす('23.08.28)
//-----------------------------------------------------------------------------
// 引数	pZmsub		：	対象の財務クラス
//-----------------------------------------------------------------------------
// 返送値		0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::TrunOffSyzCksw(CDBZmSub *pZmsub)
{
	ASSERT(pZmsub);
	ASSERT(pZmsub->m_database);

	int rt = -1;

	try{
		pZmsub->m_database->ExecuteSQL(UPDATE_CKSW_IN_MINFO_SQL);
		rt = 0;
	}
	catch( CDBException *pDe ){
		m_ErrMsg = pDe->m_strError;
		pDe->Delete();
		return rt;
	}

	return rt;
}

//-----------------------------------------------------------------------------
// 指定期間内の仕訳に打たれているインボイス登録番号の正当性をチェックする.
// Web-APIの判定時間に時間がかかるためチェック中待ちダイアログを表示する.
//-----------------------------------------------------------------------------
// 引数	pZmsub		     ：	マスターに接続済の財務クラスのポインタ
//		StYymmdd	     ：	開始年月日
//		EdYymmdd	     ：	終了年月日
//      WrongInvoiceDataList： 不正データ詳細
//      pParent           :  親ウィンドウ
//-----------------------------------------------------------------------------
// 返送値	0		：	正常（インボイス登録番号データ不正なし）
//			1		：	正常（インボイス登録番号データ不正あり）
//		   -1		：	エラー
//-----------------------------------------------------------------------------
int CSyzMakeSubW::CheckWrongInvoiceData(CDBZmSub *pZmSub, long StYymmdd, long EdYymmdd, SYZ_WrongInvoiceDataList& WrongInvoiceDataList, CWnd* pParent)
{

	ASSERT(pZmSub);
	ASSERT(pZmSub->m_database);
	if (pZmSub->m_database == NULL) {
		m_ErrMsg = _T("財務マスターがオープンされていません！");
		return -1;
	}

    //インボイス対応済みマスターチェック
	CZmGen8 ZmGen;
	bool bInvoiceVerup = ZmGen.IsInvoiceVupMaster(pZmSub);
    if( bInvoiceVerup == false ){
		m_ErrMsg = _T("インボイス改正バージョンアップ済みマスターではありません！");
        return -1;
	}

    //インボイス施行日後の指定期間かチェック.
    if( (StYymmdd < INVOICE_SYZ_YMD) && (EdYymmdd < INVOICE_SYZ_YMD) ) {
		m_ErrMsg = _T("インボイス施工日前の期間が設定されています。");
		return -1;
	}
    
    int sddate = 0, eddate = 0;
    if(StYymmdd < INVOICE_SYZ_YMD){
        sddate = INVOICE_SYZ_YMD;
	}
    else {
        sddate = StYymmdd;
	}
    eddate = EdYymmdd;

	int rt = 0;

	// リソースハンドルの切り替え
	HINSTANCE hInst = AfxGetResourceHandle();
	AfxSetResourceHandle( syzMakeInstance );

	CWaitDialog* pWaitDialog = new CWaitDialog(pParent);
	if (pWaitDialog != NULL) {
		BOOL ret = pWaitDialog->Create(IDD_WAIT_DIALOG, pParent);
		if (!ret) { //create failed.
			AfxSetResourceHandle( hInst );
			return -1;
		}
		pWaitDialog->ShowWindow(SW_SHOW);
		pWaitDialog->GetDlgItem(IDC_STATIC_WAIT)->SetWindowTextA(_T("適格請求書発行事業者登録番号 判定中・・・"));
	}

    //仕訳データから判定データを抽出.
    CString filter, subfilter;
    filter.Empty();
	subfilter.Empty();

	//適請求書発行事業者登録番号が入力されている 且つ 取り消しサイン:OFF
	filter += _T("(invno IS NOT NULL) and ((SUBSTRING(dsign, 1, 1) & 1) = 0) ");
	filter += _T("AND ");
    //指定期間内
    subfilter.Format(_T("((%d <= ddate) AND (ddate <= %d))"), sddate, eddate);
    filter += subfilter;

	bool openflg = false;
	if (pZmSub->data == NULL) {
		if (pZmSub->DataOpen(filter)) {
			m_ErrMsg = _T("仕訳データのオープンに失敗しました。");
			pWaitDialog->DestroyWindow();
			delete pWaitDialog;
			pWaitDialog = NULL;
			AfxSetResourceHandle( hInst );
			return -1;
		}
		openflg = true;
	}
	else {
		if (pZmSub->data->Requery(filter, 0, 0)) {
			m_ErrMsg = _T("仕訳データのRequeryに失敗しました。");
			pWaitDialog->DestroyWindow();
			delete pWaitDialog;
			pWaitDialog = NULL;
			AfxSetResourceHandle( hInst );
			return -1;
		}
	}

    CString strErr;
    CString pre_invno;
    int pre_ddate;
	int errorStatusHojin; 
	int errorStatusKojin;
	WrongInvoiceDataList.CntNG = 0;

	for (int i = 0;; i++) {
		if (i == 0) {
			if ((pZmSub->data->MoveFirst()) || (pZmSub->data->st == -1)) {
				break;
			}
		}
		else {
			if ((pZmSub->data->MoveNext()) || (pZmSub->data->st == -1)) {
				break;
			}
		}

		//DbgViewTrace(_T("【DEBUG】%d %s() seq:%d ddate:%d invno:%s\n"), __LINE__, __FUNCTION__, pZmSub->data->seq, pZmSub->data->ddate, pZmSub->data->invno);
        if( (pre_invno == pZmSub->data->invno) && (pre_ddate == pZmSub->data->ddate)){
			//前回と同じ登録番号,日付なら同じ結果なのでWebAPIは呼ばない.
            continue;
		}

		errorStatusHojin = _CheckRegistratedNo(1/*法人*/, pZmSub->data->invno, strErr, pZmSub->data->ddate);
		//DbgViewTrace(_T("【DEBUG】%d %s() errorstatus(法人):%d\n"), __LINE__, __FUNCTION__, errorStatusHojin);
		errorStatusKojin = _CheckRegistratedNo(0/*個人*/, pZmSub->data->invno, strErr, pZmSub->data->ddate);
		//DbgViewTrace(_T("【DEBUG】%d %s() errorstatus(個人):%d\n"), __LINE__, __FUNCTION__, errorStatusKojin);

		if ((errorStatusHojin == 0) || (errorStatusKojin == 0)) {
			//   法人、個人どちらかのチェックが正常ならOK.
		}
		else {
			//	エラー
			WrongInvoiceDataList.WrongData[WrongInvoiceDataList.CntNG].errNo = WrongInvoiceDataList.CntNG + 1;
			WrongInvoiceDataList.WrongData[WrongInvoiceDataList.CntNG].errStatusHojin = errorStatusHojin;
			WrongInvoiceDataList.WrongData[WrongInvoiceDataList.CntNG].errStatusKojin = errorStatusKojin;
			WrongInvoiceDataList.WrongData[WrongInvoiceDataList.CntNG].seq = pZmSub->data->seq;
			WrongInvoiceDataList.WrongData[WrongInvoiceDataList.CntNG].ddate = pZmSub->data->ddate;
			WrongInvoiceDataList.WrongData[WrongInvoiceDataList.CntNG].invno = pZmSub->data->invno;
			DbgViewTrace(_T("【DEBUG】%d %s() WrongInvoiceDataList.CntNG:%d\n"),
				__LINE__, __FUNCTION__, WrongInvoiceDataList.CntNG + 1);
			DbgViewTrace(_T("【DEBUG】%d %s() WrongInvoiceDataList.WrongData[%d].errNo:%d\n"),
				__LINE__, __FUNCTION__, WrongInvoiceDataList.CntNG, WrongInvoiceDataList.WrongData[WrongInvoiceDataList.CntNG].errNo);
			DbgViewTrace(_T("【DEBUG】%d %s() WrongInvoiceDataList.WrongData[%d].errStatusHojin:%d\n"),
				__LINE__, __FUNCTION__, WrongInvoiceDataList.CntNG, WrongInvoiceDataList.WrongData[WrongInvoiceDataList.CntNG].errStatusHojin);
			DbgViewTrace(_T("【DEBUG】%d %s() WrongInvoiceDataList.WrongData[%d].errStatusKojin:%d\n"),
				__LINE__, __FUNCTION__, WrongInvoiceDataList.CntNG, WrongInvoiceDataList.WrongData[WrongInvoiceDataList.CntNG].errStatusKojin);
			DbgViewTrace(_T("【DEBUG】%d %s() WrongInvoiceDataList.WrongData[%d].seq:%d\n"),
				__LINE__, __FUNCTION__, WrongInvoiceDataList.CntNG, WrongInvoiceDataList.WrongData[WrongInvoiceDataList.CntNG].seq);
			DbgViewTrace(_T("【DEBUG】%d %s() WrongInvoiceDataList.WrongData[%d].ddate:%d\n"),
				__LINE__, __FUNCTION__, WrongInvoiceDataList.CntNG, WrongInvoiceDataList.WrongData[WrongInvoiceDataList.CntNG].ddate);
			DbgViewTrace(_T("【DEBUG】%d %s() WrongInvoiceDataList.WrongData[%d].ddate:%s\n"),
				__LINE__, __FUNCTION__, WrongInvoiceDataList.CntNG, WrongInvoiceDataList.WrongData[WrongInvoiceDataList.CntNG].invno);
			WrongInvoiceDataList.CntNG++;

			//240624 add -->
			if (openflg) {
				pZmSub->DataClose();
				AfxSetResourceHandle(hInst);
				openflg = false;
			}
			pWaitDialog->DestroyWindow();
			delete pWaitDialog;
			pWaitDialog = NULL;
			AfxSetResourceHandle(hInst);

			//不正な番号がひとつでも見つかったらreturn
			return 1;
			//<--

	//-->240624 依頼24-0098 del
		//      if( (errorStatusHojin == 0) || (errorStatusKojin == 0) ) {
		//          法人、個人どちらかのチェックが正常ならOK.
			//}
			//else{
			//	エラー
			//	WrongInvoiceDataList.WrongData[WrongInvoiceDataList.CntNG].errNo = WrongInvoiceDataList.CntNG + 1;
			//	WrongInvoiceDataList.WrongData[WrongInvoiceDataList.CntNG].errStatusHojin = errorStatusHojin;
			//	WrongInvoiceDataList.WrongData[WrongInvoiceDataList.CntNG].errStatusKojin = errorStatusKojin;
			//	WrongInvoiceDataList.WrongData[WrongInvoiceDataList.CntNG].seq = pZmSub->data->seq;
			//	WrongInvoiceDataList.WrongData[WrongInvoiceDataList.CntNG].ddate = pZmSub->data->ddate;
			//	WrongInvoiceDataList.WrongData[WrongInvoiceDataList.CntNG].invno = pZmSub->data->invno;
			//	DbgViewTrace(_T("【DEBUG】%d %s() WrongInvoiceDataList.CntNG:%d\n"),
			//		__LINE__, __FUNCTION__, WrongInvoiceDataList.CntNG + 1);
			//	DbgViewTrace(_T("【DEBUG】%d %s() WrongInvoiceDataList.WrongData[%d].errNo:%d\n"),
			//		__LINE__, __FUNCTION__, WrongInvoiceDataList.CntNG, WrongInvoiceDataList.WrongData[WrongInvoiceDataList.CntNG].errNo);
			//	DbgViewTrace(_T("【DEBUG】%d %s() WrongInvoiceDataList.WrongData[%d].errStatusHojin:%d\n"),
			//		__LINE__, __FUNCTION__, WrongInvoiceDataList.CntNG, WrongInvoiceDataList.WrongData[WrongInvoiceDataList.CntNG].errStatusHojin);
			//	DbgViewTrace(_T("【DEBUG】%d %s() WrongInvoiceDataList.WrongData[%d].errStatusKojin:%d\n"),
			//		__LINE__, __FUNCTION__, WrongInvoiceDataList.CntNG, WrongInvoiceDataList.WrongData[WrongInvoiceDataList.CntNG].errStatusKojin);
			//	DbgViewTrace(_T("【DEBUG】%d %s() WrongInvoiceDataList.WrongData[%d].seq:%d\n"),
			//		__LINE__, __FUNCTION__, WrongInvoiceDataList.CntNG, WrongInvoiceDataList.WrongData[WrongInvoiceDataList.CntNG].seq);
			//	DbgViewTrace(_T("【DEBUG】%d %s() WrongInvoiceDataList.WrongData[%d].ddate:%d\n"),
			//		__LINE__, __FUNCTION__, WrongInvoiceDataList.CntNG, WrongInvoiceDataList.WrongData[WrongInvoiceDataList.CntNG].ddate);
			//	DbgViewTrace(_T("【DEBUG】%d %s() WrongInvoiceDataList.WrongData[%d].ddate:%s\n"),
			//		__LINE__, __FUNCTION__, WrongInvoiceDataList.CntNG, WrongInvoiceDataList.WrongData[WrongInvoiceDataList.CntNG].invno);
			//	WrongInvoiceDataList.CntNG++;
			//}
			//if (WrongInvoiceDataList.CntNG > 255) {
			//	m_ErrMsg = _T("不正データの数が多すぎます。");
			//	if (openflg) {
			//		pZmSub->DataClose();
			//		AfxSetResourceHandle(hInst);
			//		openflg = false;
			//	}
			//	pWaitDialog->DestroyWindow();
			//	delete pWaitDialog;
			//	pWaitDialog = NULL;
			//	AfxSetResourceHandle(hInst);
			//	return -1;
			//	
			//}
	//<--24-0098 del
		}
		pre_invno = pZmSub->data->invno;
		pre_ddate = pZmSub->data->ddate;
	}

	if (openflg) {
		pZmSub->DataClose();
		openflg = false;
	}

	pWaitDialog->DestroyWindow();
	delete pWaitDialog;
	pWaitDialog = NULL;

	//if(WrongInvoiceDataList.CntNG) {
 //       rt = 1;
	//}
	AfxSetResourceHandle( hInst );

    return rt;
}