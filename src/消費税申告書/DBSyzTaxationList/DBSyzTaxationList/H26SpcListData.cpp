
#include "stdafx.h"
#include "H26SpcListData.h"

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CH26SpcListData::CH26SpcListData( BOOL isTransitionalMeasure )
	: m_isTransitionalMeasure(isTransitionalMeasure)
{
	memset( &Spval, '\0', sizeof(_SPCVAL) );
	memset( &stval, '\0', sizeof(_STUVAL) );

	memset( &SpTbl, '\0', sizeof(TB_PAR) );
	SpTbl.tp_tno = -1;
	memset( &TsTbl, '\0', sizeof(TB_PAR) );
	TsTbl.tp_tno = -1;

	m_Arith.l_defn( 0x16 );
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CH26SpcListData::~CH26SpcListData()
{
	if( SpTbl.tp_tno != -1 ){
		m_TblHdl.th_close( SpTbl.tp_tno );
		SpTbl.tp_tno = -1;
	}
	if( TsTbl.tp_tno != -1 ){
		m_TblHdl.th_close( TsTbl.tp_tno );
		TsTbl.tp_tno = -1;
	}
}

//-----------------------------------------------------------------------------
// 特定収入集計テ－ブルの作成＆読み込み
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラスポインタ
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CH26SpcListData::InitSpData( CDBNpSub *pDBNpSub )
{
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}

//--> '15.04.30 INS START
	BOOL IsOpenFlg = FALSE;
//<-- '15.04.30 INS END
	if( pDBNpSub->m_pSpsyk == NULL ){
		if( pDBNpSub->NpTokuteiSyunyuOpen() == -1 ){
			return -1;
		}
//--> '15.04.30 INS START
		IsOpenFlg = TRUE;
//<-- '15.04.30 INS END
	}
	else{
		pDBNpSub->m_pSpsyk->RequeryEx();
	}
	int m_Specnt = pDBNpSub->m_pSpsyk->GetRecordCountEx();

	int				thd, Cnt, Mct, Tct;
	unsigned int	rsiz, rcnt;
	unsigned long	tsiz;
	char			Ascbf[20]={0};
	char			Mony1[MONY_BUF_SIZE]={0}, Mony2[MONY_BUF_SIZE]={0}, Mony3[MONY_BUF_SIZE]={0},
					Mony4[MONY_BUF_SIZE]={0}, Mony5[MONY_BUF_SIZE]={0}, Mony6[MONY_BUF_SIZE]={0};
	SPCLQ			Sprec;

	// 集計テーブルの作成
	if( SpTbl.tp_tno != -1 ){
		m_TblHdl.th_close( SpTbl.tp_tno );
	}
	rsiz = (unsigned int)sizeof( SPCLQ );
	rcnt = (unsigned int)(m_Specnt + 10 );
	tsiz = (unsigned long)rsiz * (unsigned long)rcnt;

	// テーブル イニシャライズ
	if((thd = m_TblHdl.th_open(tsiz, rsiz)) < 0 ){
		return -1;
	}
	memset( &SpTbl, '\0', sizeof(TB_PAR) );	
	SpTbl.tp_tno  = thd;
	SpTbl.tp_rmax = rcnt;
	SpTbl.tp_rnum = 0;
	SpTbl.tp_rlen = rsiz;
	SpTbl.tp_koff = 0;
	SpTbl.tp_klen = 9;	// 科目（内部）コード (８桁) + 税区分
	SpTbl.tp_dupm = 0;
	SpTbl.tp_sufm = 0;
	SpTbl.tp_cp   = 0;
	SpTbl.tp_err  = 0;

	// 消費税集計マスターの読み込み
	Cnt = 0;
	for( int i=0;; i++ ){
		if( i==0 ){
			if( pDBNpSub->m_pSpsyk->MoveFirstEx() ){
				break;
			}
		}
		else{
			if( pDBNpSub->m_pSpsyk->MoveNextEx() ){
				break;
			}
		}
		memset( &Sprec, '\0', sizeof(SPCLQ) );
		// 内部コード
		memset( Sprec.Sp_kcod, '0', sizeof(Sprec.Sp_kcod) );
		memmove( Sprec.Sp_kcod, pDBNpSub->m_pSpsyk->m_SPkcod, 8 );
		// グループサイン
		Sprec.Sp_grp1 = pDBNpSub->m_pSpsyk->m_SPgrp[0];
		// 貸借サイン
		Sprec.Sp_katt = pDBNpSub->m_pSpsyk->m_SPkatt;
		// 税区分
		Sprec.Sp_zkbn = pDBNpSub->m_pSpsyk->m_SPzkbn;

		for( Mct=0, Tct=0; Mct!=16/*MAX_MONTH*/; Mct++ ){
			// 除外チェック
			switch( Mct ){
				case 0 : Tct = 0;	break;
				case 1 : Tct = 1;	break;
				case 2 : Tct = 2;	break;
				case 3 : Tct = 2;	break;
				case 4 : Tct = 3;	break;
				case 5 : Tct = 4;	break;
				case 6 : Tct = 5;	break;
				case 7 : Tct = 5;	break;
				case 8 : Tct = 6;	break;
				case 9 : Tct = 7;	break;
				case 10: Tct = 8;	break;
				case 11: Tct = 8;	break;
				case 12: Tct = 9;	break;
				case 13: Tct =10;	break;
				case 14: Tct =11;	break;
				case 15: Tct =11;	break;
			}
			
			// 設立時のチェック
/*			int Last_mon = Establish();
			if( Last_mon != -1 ) {
				if( Last_mon == Mct ) {
					Tct = Last_mon - 1;
					if( shin_ofst( &Vol1.EE_YMD[1], &Tct, &Vol1, &Vol2 ) )	Tct = 0;
				}
			}*/

			strcpy_s( Ascbf, pDBNpSub->m_pSpsyk->m_SPtkzn[Mct] );		// 特定収入	個別対応時　課税売上に要する
			m_Arith.l_input( Mony1, Ascbf );	
			strcpy_s( Ascbf, pDBNpSub->m_pSpsyk->m_SPthzn[Mct] );		// 〃		〃　　　　　非課税売上に要する
			m_Arith.l_input( Mony2, Ascbf );
			strcpy_s( Ascbf, pDBNpSub->m_pSpsyk->m_SPtczn[Mct] );		// 〃		〃　　　　　共通に要する
			m_Arith.l_input( Mony3, Ascbf );
			strcpy_s( Ascbf, pDBNpSub->m_pSpsyk->m_SPtszn[Mct] );		// 使途が特定されていない特定収入
			m_Arith.l_input( Mony4, Ascbf );
			strcpy_s( Ascbf, pDBNpSub->m_pSpsyk->m_SPtfzn[Mct] );		// 特定とならない課税対象外収入
			m_Arith.l_input( Mony5, Ascbf );
			strcpy_s( Ascbf, pDBNpSub->m_pSpsyk->m_SPtzan[Mct] );		// リザーブ
			m_Arith.l_input( Mony6, Ascbf );

			// 貸借逆転
			if( Sprec.Sp_katt & 0x01 ){
				m_Arith.l_neg( Mony1 );
				m_Arith.l_neg( Mony2 );
				m_Arith.l_neg( Mony3 );
				m_Arith.l_neg( Mony4 );
				m_Arith.l_neg( Mony5 );
				m_Arith.l_neg( Mony6 );
			}
			m_Arith.l_add( &Sprec.Sp_tkzn[Tct], &Sprec.Sp_tkzn[Tct], Mony1 );	
			m_Arith.l_add( &Sprec.Sp_thzn[Tct], &Sprec.Sp_thzn[Tct], Mony2 );
			m_Arith.l_add( &Sprec.Sp_tczn[Tct], &Sprec.Sp_tczn[Tct], Mony3 );
			m_Arith.l_add( &Sprec.Sp_tszn[Tct], &Sprec.Sp_tszn[Tct], Mony4 );
			m_Arith.l_add( &Sprec.Sp_tfzn[Tct], &Sprec.Sp_tfzn[Tct], Mony5 );
			m_Arith.l_add( &Sprec.Sp_tzan[Tct], &Sprec.Sp_tzan[Tct], Mony6 );
		}

		if( m_TblHdl.th_add(&Sprec, &SpTbl) ){
			return -1;
		}
	}

	// 特定収入　計算表２の明細テーブル
	if( ReInitTsTbl() ){
		return -1;
	}

//--> '15.04.30 INS START
	if( IsOpenFlg ){
		pDBNpSub->NpTokuteiSyunyuClose();
	}
//<-- '15.04.30 INS END

	return 0;
}

//-----------------------------------------------------------------------------
// 特定収入計算テーブルの再初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CH26SpcListData::ReInitTsTbl()
{
	int				thd;
	unsigned int	rsiz, rcnt;
	unsigned long	tsiz;

	// 特定収入　計算表２の明細テーブル
	if( TsTbl.tp_tno != -1 ){
		m_TblHdl.th_close( TsTbl.tp_tno );
	}
	rsiz = (unsigned int)sizeof( TSREC );					// レコード長
	rcnt = 210;												// 最大登録数
	tsiz = (unsigned long)rsiz * (unsigned long)rcnt;

	// テーブル イニシャライズ
	if( (thd=m_TblHdl.th_open(tsiz, rsiz)) < 0 ){
		return -1;
	}
	memset( &TsTbl, '\0', sizeof(TB_PAR) );	
	TsTbl.tp_tno  = thd;
	TsTbl.tp_rmax = rcnt;
	TsTbl.tp_rnum = 0;
	TsTbl.tp_rlen = rsiz;
	TsTbl.tp_koff = 0;
	TsTbl.tp_klen = 12;			// 行番号＋科目コード
	TsTbl.tp_dupm = 0;
	TsTbl.tp_sufm = 0;
	TsTbl.tp_cp   = 0;
	TsTbl.tp_err  = 0;

	return 0;
}

//-----------------------------------------------------------------------------
// 課税売上割合計算用集計
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラスポインタ
//		pSyzSyukei	：	消費税集計クラスポインタ
//		pSnHeadData	：	ヘッダ情報
//		smon		：	開始月
//		emon		：	終了月
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CH26SpcListData::CalqForUriRatio( CDBNpSub *pDBNpSub, CSyzSyukei *pSyzSyukei, CSnHeadData *pSnHeadData, int smon, int emon )
{
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}
	ASSERT( pSyzSyukei );
	if( pSyzSyukei == NULL ){
		return -1;
	}
	ASSERT( pSnHeadData );
	if( pSnHeadData == NULL ){
		return -1;
	}

	SPCLQ	SPrec;
	TSREC	TSrec;
	int		zrIdx;

	// 計算用テーブルクリア
	if( ReInitTsTbl() ){
		return -1;
	}
//2017.03.01 INSERT START
	//１項目だけなので、再読み込みせず
	char sv_SpKgjt[6] = {0};
	memmove( sv_SpKgjt, stval.SpKgjt, sizeof(sv_SpKgjt) ); //国外における資産の譲渡等の対価の額 退避
//2017.03.01 INSERT END
	memset( &stval, '\0', sizeof(stval) );
	memset( &Spval, '\0', sizeof(Spval) );

	BOOL	isNewData = TRUE;
	for( int i=0; i<SpTbl.tp_rnum; ++i ){
		if( m_TblHdl.th_rget(&SPrec, &SpTbl, i) ){
			return -1;
		}

		// 科目コードをキーの一部
		isNewData = TRUE;
		memset( &TSrec, '\0', sizeof(TSREC) );
		memmove( TSrec.TsKcod, SPrec.Sp_kcod, sizeof(SPrec.Sp_kcod) );
		GetKnrecNameEx( pDBNpSub, SPrec.Sp_kcod, TSrec.TsName, TSrec.TsLine );

		if( m_TblHdl.th_read('s', &TSrec, &TsTbl) != -1 ){
			isNewData = FALSE;
		}

		if( SPrec.Sp_zkbn == 3 ){
			zrIdx = 1;
		}
		else if( SPrec.Sp_zkbn == 0 ){
			zrIdx = 0;
		}
		// 2019/10/31
		// 10％の仕訳が入力されていると不正な税率として判定されてしまうため処理をカット
		else{
			//ASSERT( !_T("不正な税率！") );
			continue;
		}

//		memset( &TSrec, '\0', sizeof(TSREC) );

//-- '15.04.01 --
//		for( int j=smon; j<emon; ++j ){
//---------------
		for( int j=smon; j<=emon; ++j ){
//---------------
			// 特定収入（課税売上に係る）
//-- '15.04.27 --
//			m_Arith.l_add( stval.SpTsyu, stval.SpTsyu, &SPrec.Sp_tkzn[j] );
//---------------
			m_Arith.l_add( stval.SpSsyu, stval.SpSsyu, &SPrec.Sp_tkzn[j] );
//---------------
			// 〃　　　（非課税に係る）
			m_Arith.l_add( stval.SpHsyu, stval.SpHsyu, &SPrec.Sp_thzn[j] );
			// 〃　　　（共通に係る）
			m_Arith.l_add( stval.SpKsyu, stval.SpKsyu, &SPrec.Sp_tczn[j] );
			//	不特定収入
			m_Arith.l_add( stval.SpFsyu, stval.SpFsyu, &SPrec.Sp_tszn[j] );
//--> '15.04.27 INS START
			// 特定収入（課税に係る＋非課税に係る＋共通に係る）
			m_Arith.l_add( stval.SpTsyu, stval.SpTsyu, &SPrec.Sp_tkzn[j] );
			m_Arith.l_add( stval.SpTsyu, stval.SpTsyu, &SPrec.Sp_thzn[j] );
			m_Arith.l_add( stval.SpTsyu, stval.SpTsyu, &SPrec.Sp_tczn[j] );
//<-- '15.04.27 INS END

			// 計算表２の明細	
			// 特定収入（課税売上に係る）
//-- '15.04.25 --
//			m_Arith.l_add( TSrec.TsDtl[zrIdx].TsTsyu, TSrec.TsDtl[zrIdx].TsTsyu, &SPrec.Sp_tkzn[j] );
//---------------
			m_Arith.l_add( TSrec.TsDtl[zrIdx].TsSsyu, TSrec.TsDtl[zrIdx].TsSsyu, &SPrec.Sp_tkzn[j] );
//---------------
			// 〃　　　（非課税に係る）
			m_Arith.l_add( TSrec.TsDtl[zrIdx].TsHsyu, TSrec.TsDtl[zrIdx].TsHsyu, &SPrec.Sp_thzn[j] );
			// 〃　　　（共通に係る）
			m_Arith.l_add( TSrec.TsDtl[zrIdx].TsKsyu, TSrec.TsDtl[zrIdx].TsKsyu, &SPrec.Sp_tczn[j] );
			// 不特定収入
			m_Arith.l_add( TSrec.TsDtl[zrIdx].TsFsyu, TSrec.TsDtl[zrIdx].TsFsyu, &SPrec.Sp_tszn[j] );
			// 外特定収入
			m_Arith.l_add( TSrec.TsDtl[zrIdx].TsNosyu, TSrec.TsDtl[zrIdx].TsNosyu, &SPrec.Sp_tfzn[j] );
			// 特定収入（課税に係る＋非課税に係る＋共通に係る）★計算表の特殊合計用（個別対応専用）
//-- '15.04.25 --
//			m_Arith.l_add( TSrec.TsDtl[zrIdx].TsSsyu, TSrec.TsDtl[zrIdx].TsSsyu,  &SPrec.Sp_tkzn[j] ); 
//			m_Arith.l_add( TSrec.TsDtl[zrIdx].TsSsyu, TSrec.TsDtl[zrIdx].TsSsyu,  &SPrec.Sp_thzn[j] ); 
//			m_Arith.l_add( TSrec.TsDtl[zrIdx].TsSsyu, TSrec.TsDtl[zrIdx].TsSsyu,  &SPrec.Sp_tczn[j] ); 
//---------------
			m_Arith.l_add( TSrec.TsDtl[zrIdx].TsTsyu, TSrec.TsDtl[zrIdx].TsTsyu,  &SPrec.Sp_tkzn[j] ); 
			m_Arith.l_add( TSrec.TsDtl[zrIdx].TsTsyu, TSrec.TsDtl[zrIdx].TsTsyu,  &SPrec.Sp_thzn[j] ); 
			m_Arith.l_add( TSrec.TsDtl[zrIdx].TsTsyu, TSrec.TsDtl[zrIdx].TsTsyu,  &SPrec.Sp_tczn[j] ); 
//---------------
		}

		if( SPrec.Sp_zkbn == 3 ){
//-- '15.04.01 --
//			for( int j=smon; j<emon; ++j ){
//---------------
			for( int j=smon; j<=emon; ++j ){
//---------------
				// 特定収入（課税売上に係る）
//-- '15.04.27 --
//				m_Arith.l_add( stval.SpTsyu63, stval.SpTsyu63, &SPrec.Sp_tkzn[j] );
//---------------
				m_Arith.l_add( stval.SpSsyu63, stval.SpTsyu63, &SPrec.Sp_tkzn[j] );
//---------------
				// 〃　　　（非課税に係る）
				m_Arith.l_add( stval.SpHsyu63, stval.SpHsyu63, &SPrec.Sp_thzn[j] );
				// 〃　　　（共通に係る）
				m_Arith.l_add( stval.SpKsyu63, stval.SpKsyu63, &SPrec.Sp_tczn[j] );
				//	不特定収入
				m_Arith.l_add( stval.SpFsyu63, stval.SpFsyu63, &SPrec.Sp_tszn[j] );
			}
		}
		else if( SPrec.Sp_zkbn == 0 ){
//-- '15.04.01 --
//			for( int j=smon; j<emon; ++j ){
//---------------
			for( int j=smon; j<=emon; ++j ){
//---------------
				// 特定収入（課税売上に係る）
//-- '15.04.27 --
//				m_Arith.l_add( stval.SpTsyu4, stval.SpTsyu4, &SPrec.Sp_tkzn[j] );
//---------------
				m_Arith.l_add( stval.SpSsyu4, stval.SpSsyu4, &SPrec.Sp_tkzn[j] );
//---------------
				// 〃　　　（非課税に係る）
				m_Arith.l_add( stval.SpHsyu4, stval.SpHsyu4, &SPrec.Sp_thzn[j] );
				// 〃　　　（共通に係る）
				m_Arith.l_add( stval.SpKsyu4, stval.SpKsyu4, &SPrec.Sp_tczn[j] );
				//	不特定収入
				m_Arith.l_add( stval.SpFsyu4, stval.SpFsyu4, &SPrec.Sp_tszn[j] );
			}
		}

		// 計算表２の明細
		m_Arith.l_neg( TSrec.TsDtl[zrIdx].TsTsyu );
		m_Arith.l_neg( TSrec.TsDtl[zrIdx].TsHsyu );
		m_Arith.l_neg( TSrec.TsDtl[zrIdx].TsKsyu );
		m_Arith.l_neg( TSrec.TsDtl[zrIdx].TsFsyu );
		m_Arith.l_neg( TSrec.TsDtl[zrIdx].TsSsyu );
		m_Arith.l_neg( TSrec.TsDtl[zrIdx].TsNosyu );
//-- '15.04.27 --
//		m_Arith.l_add( TSrec.TsDtl[zrIdx].TsGsyu, TSrec.TsDtl[zrIdx].TsGsyu, TSrec.TsDtl[zrIdx].TsTsyu );
//---------------
		m_Arith.l_add( TSrec.TsDtl[zrIdx].TsGsyu, TSrec.TsDtl[zrIdx].TsGsyu, TSrec.TsDtl[zrIdx].TsSsyu );
//---------------
		m_Arith.l_add( TSrec.TsDtl[zrIdx].TsGsyu, TSrec.TsDtl[zrIdx].TsGsyu, TSrec.TsDtl[zrIdx].TsHsyu );
		m_Arith.l_add( TSrec.TsDtl[zrIdx].TsGsyu, TSrec.TsDtl[zrIdx].TsGsyu, TSrec.TsDtl[zrIdx].TsKsyu );
		m_Arith.l_add( TSrec.TsDtl[zrIdx].TsGsyu, TSrec.TsDtl[zrIdx].TsGsyu, TSrec.TsDtl[zrIdx].TsFsyu );

//-- '15.03.07 --
// 資産の譲渡等の対価以外の収入を判定しない場合
//		if( !m_Arith.l_test( TSrec.TsDtl[zrIdx].TsTsyu ) && 
//			!m_Arith.l_test( TSrec.TsDtl[zrIdx].TsHsyu ) &&
//			!m_Arith.l_test( TSrec.TsDtl[zrIdx].TsKsyu ) && 
//			!m_Arith.l_test( TSrec.TsDtl[zrIdx].TsFsyu ) &&
//			!m_Arith.l_test( TSrec.TsDtl[zrIdx].TsGsyu ) ){
//			continue;
//		}
//---------------
//--- '15.04.27 ---
//		if( !m_Arith.l_test( TSrec.TsDtl[zrIdx].TsTsyu ) && 
//-----------------
		if( !m_Arith.l_test( TSrec.TsDtl[zrIdx].TsSsyu ) && 
//-----------------
			!m_Arith.l_test( TSrec.TsDtl[zrIdx].TsHsyu ) &&
			!m_Arith.l_test( TSrec.TsDtl[zrIdx].TsKsyu ) && 
			!m_Arith.l_test( TSrec.TsDtl[zrIdx].TsFsyu ) &&
			!m_Arith.l_test( TSrec.TsDtl[zrIdx].TsGsyu ) &&
			!m_Arith.l_test( TSrec.TsDtl[zrIdx].TsNosyu ) ){
			continue;
		}
//---------------

		// 資産の譲渡等の対価以外の収入（特定収入＋不特定収入＋外特定収入）
		m_Arith.l_add( TSrec.TsJsyu, TSrec.TsJsyu, TSrec.TsDtl[zrIdx].TsGsyu );
		m_Arith.l_add( TSrec.TsJsyu, TSrec.TsJsyu, TSrec.TsDtl[zrIdx].TsNosyu );

//		GetKnrecNameEx( pDBNpSub, SPrec.Sp_kcod, TSrec.TsName, TSrec.TsLine );
//-- '15.03.02 --
//		if( m_TblHdl.th_acs(thWritex, &TSrec, &TsTbl) ){
//			return -1;
//		}
//---------------
		if( isNewData ){
			if( m_TblHdl.th_add(&TSrec, &TsTbl) == -1 ){
				return -1;
			}
		}
		else{
			if( m_TblHdl.th_update(&TSrec, &TSrec, &TsTbl) == -1 ){
				return -1;
			}
		}
//---------------

#ifdef	_DEBUG
		int		line = TSrec.TsLine;
		char	wbuf[64]={0};
		hl_rev( &line, 4 );
		TRACE("\n(%4d)", line );
		m_Arith.l_print( wbuf, TSrec.TsDtl[zrIdx].TsGsyu, _T("SSS,SSS,SSS,SS9") );
		TRACE(" TsGyu=%s ", wbuf );
		m_Arith.l_print( wbuf, TSrec.TsDtl[zrIdx].TsTsyu, _T("SSS,SSS,SSS,SS9") );
		TRACE(" TsTsyu=%s ", wbuf );
		m_Arith.l_print( wbuf, TSrec.TsDtl[zrIdx].TsHsyu, _T("SSS,SSS,SSS,SS9") );
		TRACE(" TsHsyu=%s ", wbuf );
		m_Arith.l_print( wbuf, TSrec.TsDtl[zrIdx].TsKsyu, _T("SSS,SSS,SSS,SS9") );
		TRACE(" TsKsyu=%s ", wbuf );
		m_Arith.l_print( wbuf, TSrec.TsDtl[zrIdx].TsFsyu, _T("SSS,SSS,SSS,SS9") );
		TRACE(" TsFsyu=%s ", wbuf );
		m_Arith.l_print( wbuf, TSrec.TsDtl[zrIdx].TsSsyu, _T("SSS,SSS,SSS,SS9") );
		TRACE(" TsSsyu=%s ", wbuf );
		TRACE("\n");
#endif
	}

	m_Arith.l_neg( stval.SpTsyu );
	m_Arith.l_neg( stval.SpTsyu4 );
	m_Arith.l_neg( stval.SpTsyu63 );
	m_Arith.l_neg( stval.SpHsyu );
	m_Arith.l_neg( stval.SpHsyu4 );
	m_Arith.l_neg( stval.SpHsyu63 );
	m_Arith.l_neg( stval.SpKsyu );
	m_Arith.l_neg( stval.SpKsyu4 );
	m_Arith.l_neg( stval.SpKsyu63 );
	m_Arith.l_neg( stval.SpFsyu );
	m_Arith.l_neg( stval.SpFsyu4 );
	m_Arith.l_neg( stval.SpFsyu63 );
//--> '15.04.27 INS START
	m_Arith.l_neg( stval.SpSsyu );
	m_Arith.l_neg( stval.SpSsyu4 );
	m_Arith.l_neg( stval.SpSsyu63 );
//<-- '15.04.27 INS END

//-- '15.04.27 --
//	m_Arith.l_add( stval.SpGsyu, stval.SpGsyu, stval.SpTsyu );
//---------------
	m_Arith.l_add( stval.SpGsyu, stval.SpGsyu, stval.SpSsyu );
//---------------
	m_Arith.l_add( stval.SpGsyu, stval.SpGsyu, stval.SpHsyu );
	m_Arith.l_add( stval.SpGsyu, stval.SpGsyu, stval.SpKsyu );
	m_Arith.l_add( stval.SpGsyu, stval.SpGsyu, stval.SpFsyu );
//-- '15.04.27 --
//	m_Arith.l_add( stval.SpGsyu4, stval.SpGsyu4, stval.SpTsyu4 );
//---------------
	m_Arith.l_add( stval.SpGsyu4, stval.SpGsyu4, stval.SpSsyu4 );
//---------------
	m_Arith.l_add( stval.SpGsyu4, stval.SpGsyu4, stval.SpHsyu4 );
	m_Arith.l_add( stval.SpGsyu4, stval.SpGsyu4, stval.SpKsyu4 );
	m_Arith.l_add( stval.SpGsyu4, stval.SpGsyu4, stval.SpFsyu4 );
//-- '15.04.27 --
//	m_Arith.l_add( stval.SpGsyu63, stval.SpGsyu63, stval.SpTsyu63 );
//---------------
	m_Arith.l_add( stval.SpGsyu63, stval.SpGsyu63, stval.SpSsyu63 );
//---------------
	m_Arith.l_add( stval.SpGsyu63, stval.SpGsyu63, stval.SpHsyu63 );
	m_Arith.l_add( stval.SpGsyu63, stval.SpGsyu63, stval.SpKsyu63 );
	m_Arith.l_add( stval.SpGsyu63, stval.SpGsyu63, stval.SpFsyu63 );

	for( int i=0 ; i<TsTbl.tp_rnum; ++i ){
		if( m_TblHdl.th_rget(&TSrec, &TsTbl, i) ){
			return -1;
		}
		TSrec.TsLine = (i + 1);
#ifdef	_DEBUG
		TRACE("\n(%4d)", TSrec.TsLine );
#endif
		if( m_TblHdl.th_rput(&TSrec, &TsTbl, i) ){
			return -1;
		}
	}

	// 他の集計額の取得
	// 有価証券譲渡収入の５％
	if( ((pDBNpSub->zvol->apno&0xff)==0x52) || ((pDBNpSub->zvol->apno&0xff)==0x59) ){
	}
	else{
	}

	// 課税売上割合
/*	memmove( stval.SpKuws, stval.kwbuns, sizeof(stval.SpKuws) );			// 課税売上高割合（分子）
	memmove( stval.SpKuwb, stval.kwbunb, sizeof(stval.SpKuwb) );			// 〃　　　　　　（分母）
	memmove( stval.SpUwri, stval.uwriai, sizeof(stval.SpUwri) );			// 課税売上高割合

	memmove( stval.SpUnb3, stval.knebik_3, sizeof(stval.SpUnb3) );			// 売上返還　3%
	memmove( stval.SpUnb4, stval.knebik, sizeof(stval.SpUnb4) );			// 〃　　　　4%
	m_Arith.l_add( stval.SpUnbg, stval.SpUnb3, stval.SpUnb4 );				// 〃　　　　合計

	m_Arith.l_add( stval.SpKzur, stval.kazeiu, stval.kazeiu_3 );			// 課税売上（準売上）
	memmove( stval.SpMzur, stval.exptur, sizeof(stval.SpMzur) );			// 免税売上

	memmove( stval.SpHkur, stval.hikazu, sizeof(stval.SpHkur) );			// 非課税売上（有価証券を全額で含む）
	m_Arith.l_sub( stval.SpHkur, stval.SpHkur, stval.SpYuk5 );
	m_Arith.l_add( stval.SpHkur, stval.SpHkur, stval.SpYuka );

	// 仕入
	memmove( stval.SpKsi3, stval.komisi_3, sizeof(stval.SpKsi3) );			// 3%：税込み仕入
	memmove( stval.SpKsi4, stval.komisi, sizeof(stval.SpKsi4) );			// 4%：税込み仕入
	m_Arith.l_add( stval.SpKsig, stval.komisi_3, stval.komisi );			// 合計：税込み仕入	

	// 仕入値引
	memmove( stval.SpKsb3, stval.sinebk_3, sizeof(stval.SpKsb3) );			// 3%：税込み仕入値引
	memmove( stval.SpKsb4, stval.sinebk, sizeof(stval.SpKsb4) );			// 4%：税込み仕入値引
	m_Arith.l_add( stval.SpKsbg, stval.sinebk_3, stval.sinebk );			// 合計：税*/


	// 消費税残高の読み込み
	MoneyBasejagArray	money;
//	BOOL				IsKziKmk = TRUE;	// '14.10.21
//	BOOL				IsTagKmk = FALSE;	// '15.02.13

	// 課税売上割合計算用金額の取得
	money = pSyzSyukei->GetShinkokuData( _T("90") );
	int max = (int)money.GetCount();
	for( int i=0; i<max; i++ ){
		int inmax = (int)money[i].GetCount();
		for( int j=0; j<inmax; j++ ){
			if( money[i][j].code == _T("900400") ){
				memmove( stval.SpKuws, money[i][j].arith, sizeof(stval.SpKuws) );			// 課税売上高割合（分子）
			}
			else if( money[i][j].code == _T("900700") ){
				memmove( stval.SpKuwb, money[i][j].arith, sizeof(stval.SpKuwb) );			// 〃　　　　　　（分母）
			}
			else if( money[i][j].code == _T("900800") ){
				memmove( stval.SpUwri, money[i][j].arith, sizeof(stval.SpUwri) );			// 課税売上高割合
			}
			else if( money[i][j].code == _T("901201") ){
				memmove( stval.SpUnb3, money[i][j].arith, sizeof(stval.SpUnb3) );			// 売上返還　3%
			}
			else if( money[i][j].code == _T("901202") ){
				memmove( stval.SpUnb4, money[i][j].arith, sizeof(stval.SpUnb4) );			// 〃　　　　4%
			}
			else if( money[i][j].code == _T("901203") ){
				// 売上返還6.3%
			}
			else if( money[i][j].code == _T("901200") ){
				// 売上返還 合計
//				m_Arith.l_add( stval.SpUnbg, stval.SpUnb3, stval.SpUnb4 );					// 〃　　　　合計
				memmove( stval.SpUnbg, money[i][j].arith, sizeof(stval.SpUnbg) );			// 〃　　　　合計
			}
			else if( money[i][j].code == _T("901100") ){
				// 課税売上
//				m_Arith.l_add( stval.SpKzur, stval.kazeiu, stval.kazeiu_3 );				// 課税売上（純売上）
				memmove( stval.SpKzur, money[i][j].arith, sizeof(stval.SpKzur) );			// 課税売上（純売上）
			}
//2017.03.01 INSERT START
			else if( money[i][j].code == _T("901102") ){
				// 課税売上 4%
				memmove( stval.SpKzur4, money[i][j].arith, sizeof(stval.SpKzur4) );			// 課税売上（純売上）
			}
			else if( money[i][j].code == _T("901103") ){
				// 課税売上 6.3%
				memmove( stval.SpKzur63, money[i][j].arith, sizeof(stval.SpKzur63) );			// 課税売上（純売上）
			}
//2017.03.01 INSERT END
			else if( money[i][j].code == _T("900200") ){
				memmove( stval.SpMzur, money[i][j].arith, sizeof(stval.SpMzur) );			// 免税売上
			}
			else if( money[i][j].code == _T("900600") ){
				memmove( stval.SpHkur, money[i][j].arith, sizeof(stval.SpHkur) );			// 非課税売上（有価証券を全額で含む）
//				m_Arith.l_sub( stval.SpHkur, stval.SpHkur, stval.SpYuk5 );
//				m_Arith.l_add( stval.SpHkur, stval.SpHkur, stval.SpYuka );
			}
			else if( money[i][j].code == _T("901600") ){
				// 有価証券譲渡収入の5%
			}
		}
	}

//2017.03.01 INSERT START
	memmove( stval.SpKgjt, sv_SpKgjt, sizeof(stval.SpKgjt) ); //国外における資産の譲渡等の対価の額
//2017.03.01 INSERT END

//--> '15.05.18 INS START
	// 売上金額の取得
	int	uriCalq = 0;	// 1:課税仕入, 2:課税仕入の返還
	money = pSyzSyukei->GetShisanData( PR_URIAGE );
	max = (int)money.GetCount();
	for( int i=0; i<max; i++ ){
		int inmax = (int)money[i].GetCount();
		if( inmax == 0 ){
			continue;
		}

		if( money[i][0].name == _T("【非課税売上】") ){
			uriCalq = 1;
		}
		else if( money[i][0].name == _T("【不課税売上】") ){
			uriCalq = 0;
		}

		unsigned char sc_kbn1 = money[i][0].attr.sc_kbn1;
		unsigned char sc_kbn2 = money[i][0].attr.sc_kbn2;

		if( uriCalq == 1 ){
			if( (money[i][0].attr.sc_kbn1==0x11) && (money[i][0].attr.sc_kbn2==0x03) ){
				m_Arith.l_add( stval.SpHkur, stval.SpHkur, money[i][0].arith );
			}
			else if( (money[i][0].attr.sc_kbn1==0x02) && (money[i][0].attr.sc_kbn2==0x0B) ){
				m_Arith.l_add( stval.SpHkur, stval.SpHkur, money[i][0].arith );
			}
		}
	}
//<-- '15.05.18 INS END


//--> '15.03.07 INS START
	// 計算表１用の金額
	m_Arith.l_clear( stval.SpTuwb );
	m_Arith.l_add( stval.SpTuwb, stval.SpTuwb, stval.SpKzur );
	m_Arith.l_add( stval.SpTuwb, stval.SpTuwb, stval.SpMzur );
	m_Arith.l_add( stval.SpTuwb, stval.SpTuwb, stval.SpHkur );
//<-- '15.03.07 INS END
//2017.03.01 INSERT START
	m_Arith.l_add( stval.SpTuwb, stval.SpTuwb, stval.SpKgjt );
//2017.03.01 INSERT END

//--> '15.03.07 INS START
	// 課税仕入れ等の税額の取得
	char	moji[64]={0};
	char	PW0[MONY_BUF_SIZE]={0};
	wsprintf( moji, _T("%d"), pSnHeadData->m_UriRatioBorder );
	m_Arith.l_input( PW0, moji );
	if( m_Arith.l_cmp(stval.SpUwri, PW0) >= 0 ){	// 課税売上割合９５％以上
		money = pSyzSyukei->GetShinkokuData( _T("1213") );
	}
	else{											// 課税売上割合９５％以下
		if( pSnHeadData->IsKobetuSiireAnbun() ){
			money = pSyzSyukei->GetShinkokuData( _T("1216") );
		}
		else{
			money = pSyzSyukei->GetShinkokuData( _T("1217") );
		}
	}

	max = (int)money.GetCount();
	for( int i=0; i<max; i++ ){
		int inmax = (int)money[i].GetCount();
		for( int j=0; j<inmax; j++ ){
			if( (money[i][j].code==_T("121301")) || (money[i][j].code==_T("121601")) || (money[i][j].code==_T("121701")) ){
				memmove( stval.SpKgz3, money[i][j].arith, sizeof(stval.SpKgz3) );
			}
			else if( (money[i][j].code==_T("121302")) || (money[i][j].code==_T("121602")) || (money[i][j].code==_T("121702")) ){
				memmove( stval.SpKgz4, money[i][j].arith, sizeof(stval.SpKgz4) );
			}
			else if( (money[i][j].code==_T("121303")) || (money[i][j].code==_T("121603")) || (money[i][j].code==_T("121703")) ){
				memmove( stval.SpKgz63, money[i][j].arith, sizeof(stval.SpKgz63) );
			}
		}
	}
//<-- '15.03.07 INS END

	// 仕入金額の取得
	int	shileCalq = 0;	// 1:課税仕入, 2:課税仕入の返還
	money = pSyzSyukei->GetShisanData( PR_SHIIRE );
	max = (int)money.GetCount();
	for( int i=0; i<max; i++ ){
		int inmax = (int)money[i].GetCount();
		if( inmax == 0 ){
			continue;
		}
		else if( money[i][0].attr.shisan_syukei_type == -1 ){
			// 明細は飛ばす
			continue;
		}

		if( money[i][0].name == _T("【課税仕入】") ){
			shileCalq = 1;
		}
		else if( money[i][0].name == _T("【課税仕入の返還】") ){
			shileCalq = 2;
		}
		else if( money[i][0].name == _T("【課税仕入に係る支払対価】") ){
			break;
		}

//		if( (money[i][0].attr.shisan_gp_kubun==(char)0x84) && (money[i][0].attr.shisan_syukei_type==1) ){
		if( money[i][0].attr.shisan_syukei_type == 1 ){
#ifdef _DEBUG
			CString	tmpName = money[i][0].name;
#endif
			if( shileCalq == 1 ){
				if( money[i][0].attr.shisan_zino == 1 ){
					memmove( stval.SpKsi3, money[i][0].arith, sizeof(stval.SpKsi3) );			// 3%：税込み仕入
				}
				else if( money[i][0].attr.shisan_zino == 0 ){
					memmove( stval.SpKsi4, money[i][0].arith, sizeof(stval.SpKsi4) );			// 4%：税込み仕入
				}
				else if( money[i][0].attr.shisan_zino == 3 ){
					memmove( stval.SpKsi63, money[i][0].arith, sizeof(stval.SpKsi63) );			// 6.3%：税込み仕入
				}
			}
			else if( shileCalq == 2 ){
				if( money[i][0].attr.shisan_zino == 1 ){
					memmove( stval.SpKsb3, money[i][0].arith, sizeof(stval.SpKsb3) );			// 3%：税込み仕入値引
				}
				else if( money[i][0].attr.shisan_zino == 0 ){
					memmove( stval.SpKsb4, money[i][0].arith, sizeof(stval.SpKsb4) );			// 4%：税込み仕入値引
				}
				else if( money[i][0].attr.shisan_zino == 3 ){
					memmove( stval.SpKsb63, money[i][0].arith, sizeof(stval.SpKsb63) );			// 6.3%：税込み仕入値引
				}
			}
		}
	}

//--> '15.03.07 INS START
	// 通常の控除対象仕入税額の合計
//	m_Arith.l_sub( stval.SpKgz63, stval.SpKsi63, stval.SpKsb63 );
//	m_Arith.l_sub( stval.SpKgz4, stval.SpKsi4, stval.SpKsb4 );
//	m_Arith.l_sub( stval.SpKgz3, stval.SpKsi3, stval.SpKsb3 );
	m_Arith.l_add( stval.SpKgzg, stval.SpKgz3, stval.SpKgz4 );
	m_Arith.l_add( stval.SpKgzg, stval.SpKgzg, stval.SpKgz63 );
//<-- '15.03.07 INS END

	return 0;
}

//-----------------------------------------------------------------------------
// 特定収入にかかる調整税額計算（サブ）
//-----------------------------------------------------------------------------
// 引数	pSnHeadData	：	ヘッダ情報
//-----------------------------------------------------------------------------
void CH26SpcListData::SPassOffCalqMain( CSnHeadData *pSnHeadData )
{
	ASSERT( pSnHeadData );
	if( pSnHeadData == NULL ){
		return;
	}

	// 初期化
	if( pSnHeadData->IsKobetuSiireAnbun() == FALSE ){
		memset( stval.SpHsyu, '\0', sizeof(stval.SpHsyu) );
		memset( stval.SpHsyu4, '\0', sizeof(stval.SpHsyu4) );
		memset( stval.SpHsyu63, '\0', sizeof(stval.SpHsyu63) );
	}

	// 特定収入割合
	// 分母
	memset( stval.SpTswb, '\0', sizeof(stval.SpTswb) );
	m_Arith.l_add( stval.SpTswb, stval.SpTuwb, stval.SpGsyu );

	// 特定収入割合
	m_Arith.l_par100( (unsigned char*)stval.SpTswr, (unsigned char*)stval.SpGsyu, (unsigned char*)stval.SpTswb, 1, 9 );
	// 調整割合
	m_Arith.l_add( stval.SpTywb, stval.SpTuwb, stval.SpFsyu );
	m_Arith.l_par100( (unsigned char *)stval.SpTwar, (unsigned char *)stval.SpFsyu,	(unsigned char *)stval.SpTywb, 2, 0 );

	if( pSnHeadData->SVmzsw == 1 ){
		SPassOffCalqSub( pSnHeadData, ID_ICSSH_SPC_8PER );
		SPassOffCalqSub( pSnHeadData, ID_ICSSH_SPC_5PER );
	}
	else{
		SPassOffCalqSub( pSnHeadData, ID_ICSSH_SPC_8PER );
	}

	// 合計
	char	WORK0[MONY_BUF_SIZE]={0}, WORK1[MONY_BUF_SIZE]={0}, WORK2[MONY_BUF_SIZE]={0}, PW0[MONY_BUF_SIZE]={0}, PW6[MONY_BUF_SIZE]={0}; 

	// 控除対象仕入税額への転記
	if( pSnHeadData->Sn_Sign4 & 0x80 ){
		pSnHeadData->Sn_Sign2 &= 0xfd;
		memset( WORK0, '\0', sizeof(WORK0) );
		m_Arith.l_input( WORK0, _T("50") );
		pSnHeadData->Sn_Sign2 &= 0xfd;
		if( m_Arith.l_cmp( stval.SpTswr, WORK0 ) <= 0 ){
			pSnHeadData->Sn_Sign2 |= 0x02;
		}
		else{
			// 経過措置対象の課税資産の譲渡等無し
//			memset( Snd.Sn_2F21A, '\0', 6 );
//			memset( Snd.Sn_2F21B, '\0', 6 );
//			Snd.Sn_2F21sw &= 0xef;
//			memmove( Snd.Sn_2F21B, stval.SpSizg, 6 );
		}
	}
}

//-----------------------------------------------------------------------------
// 特定収入にかかる調整税額計算（サブ）
//-----------------------------------------------------------------------------
// 引数	pSnHeadData	：	ヘッダ情報
//		zeiType		：	税率
//-----------------------------------------------------------------------------
void CH26SpcListData::SPassOffCalqSub( CSnHeadData *pSnHeadData, int zeiType )
{
	char	WORK0[MONY_BUF_SIZE]={0}, WORK1[MONY_BUF_SIZE]={0}, WORK2[MONY_BUF_SIZE]={0}, PW0[MONY_BUF_SIZE]={0}, PW6[MONY_BUF_SIZE]={0}; 
	char	moji[64]={0};

	// 特定収入に係る税額
	memset( PW0, '\0', sizeof(PW0) );
	wsprintf( moji, _T("%d"), pSnHeadData->m_UriRatioBorder );
	m_Arith.l_input( PW0, moji );
	if( m_Arith.l_cmp( stval.SpUwri, PW0 ) >= 0 ){	// 課税売上割合９５％以上
		memset( WORK0, '\0', sizeof(WORK0) );
		if( zeiType == ID_ICSSH_SPC_8PER ){
//-- '15.04.28 --
//			m_Arith.l_add( WORK0, WORK0, stval.SpTsyu63 );
//			m_Arith.l_add( WORK0, WORK0, stval.SpKsyu63 );
//			m_Arith.l_add( WORK0, WORK0, stval.SpHsyu63 );
//			// 特定収入税額 合計
//			m_Util.percent( stval.SpGszg63, WORK0, 9, 63, 0 );
//---------------
			// 特定収入税額 合計
			m_Util.percent( stval.SpGszg63, stval.SpTsyu63, 9, 63, 0 );
//---------------
		}
		else if( zeiType == ID_ICSSH_SPC_5PER ){
//-- '15.04.28 --
//			m_Arith.l_add( WORK0, WORK0, stval.SpTsyu4 );
//			m_Arith.l_add( WORK0, WORK0, stval.SpKsyu4 );
//			m_Arith.l_add( WORK0, WORK0, stval.SpHsyu4 );
//			// 特定収入税額 合計
//			m_Util.percent( stval.SpGszg4, WORK0, 4, 40, 0 );
//---------------
			// 特定収入税額 合計
			m_Util.percent( stval.SpGszg4, stval.SpTsyu4, 4, 40, 0 );
//---------------
		}
	}
	else{									// 課税売上割合９５％以下
//--> '15.04.15 INS START
		char	PER100BUF[MONY_BUF_SIZE] = {0};
		m_Arith.l_input( PER100BUF, _T("10000000") );
//<-- '15.04.15 INS END
		if( pSnHeadData->IsKobetuSiireAnbun() ){
			if( zeiType == ID_ICSSH_SPC_8PER ){ 
				// 控除対象仕入税額 （個別対応方式）
				// 特定収入税額
//-- '15.04.25 --
//				m_Util.percent( stval.SpTszg63, stval.SpTsyu63, 9, 63, 0 );		// 63/1080
//---------------
				m_Util.percent( stval.SpTszg63, stval.SpSsyu63, 9, 63, 0 );		// 63/1080
//---------------
				// 共通に係る特定収入税額Ｘ課税売上割合
				memset( WORK0, '\0', sizeof(WORK0) );
				memset( WORK1, '\0', sizeof(WORK1) );
				m_Util.percent( WORK0, stval.SpKsyu63, 9, 63, 0 );				// 63/1080

//				memmove( stval.SpZszg, WORK0, sizeof(stval.SpZszg) );					// 売上割合計算前の税額
				memmove( stval.SpZszg63, WORK0, sizeof(stval.SpZszg63) );					// 売上割合計算前の税額	疑問？？？？？？

				m_Arith.l_par100( (unsigned char *)WORK1, (unsigned char *)stval.SpKuws, (unsigned char *)stval.SpKuwb, 5, 0 );
				// 売上割合がマイナスの場合は０％で計算する。[03'05.17]
				memset( PW6, '\0', sizeof(PW6) );
				if( m_Arith.l_cmp( stval.SpKuws, PW6 ) < 0 ){
					memset( WORK1, '\0', sizeof(WORK1) );
				}
				if( m_Arith.l_cmp( stval.SpKuwb, PW6 ) < 0 ){
					memset( WORK1, '\0', sizeof(WORK1) );
				}

//-- '15.04.15 --
//				m_Arith.l_mul100( (unsigned char *)stval.SpKszg63, (unsigned char *)WORK0, (unsigned char *)WORK1, 5, 0 );
//---------------
				if( m_Arith.l_cmp(WORK1, PER100BUF) < 0 ){
					m_Util.l_pardotnet_syz( stval.SpKszg63, WORK0, stval.SpKuws, stval.SpKuwb );
				}
				else{
					m_Util.l_pardotnet_syz( stval.SpKszg63, WORK0, stval.SpKuws, stval.SpKuws );
				}
//---------------
				// 特定収入税額 合計
				m_Arith.l_add( stval.SpGszg63, stval.SpTszg63, stval.SpKszg63 );
			}
			else if( zeiType == ID_ICSSH_SPC_5PER ){
				// 控除対象仕入税額 （個別対応方式）
				// 特定収入税額
//-- '15.04.25 --
//				m_Util.percent( stval.SpTszg4, stval.SpTsyu4, 4, 40, 0 );		// 40/1050
//---------------
				m_Util.percent( stval.SpTszg4, stval.SpSsyu4, 4, 40, 0 );		// 40/1050
//---------------
				// 共通に係る特定収入税額Ｘ課税売上割合
				memset( WORK0, '\0', sizeof(WORK0) );
				memset( WORK1, '\0', sizeof(WORK1) );
				m_Util.percent( WORK0, stval.SpKsyu4, 4, 40, 0 );				// 40/1050

//				memmove( stval.SpZszg, WORK0, sizeof(stval.SpZszg) );					// 売上割合計算前の税額
				memmove( stval.SpZszg4, WORK0, sizeof(stval.SpZszg4) );					// 売上割合計算前の税額	疑問？？？？？？

				m_Arith.l_par100( (unsigned char *)WORK1, (unsigned char *)stval.SpKuws, (unsigned char *)stval.SpKuwb, 5, 0 );
				// 売上割合がマイナスの場合は０％で計算する。[03'05.17]
				memset( PW6, '\0', sizeof(PW6) );
				if( m_Arith.l_cmp( stval.SpKuws, PW6 ) < 0 ){
					memset( WORK1, '\0', sizeof(WORK1) );
				}
				if( m_Arith.l_cmp( stval.SpKuwb, PW6 ) < 0 ){
					memset( WORK1, '\0', sizeof(WORK1) );
				}

//-- '15.04.15 --
//				m_Arith.l_mul100( (unsigned char *)stval.SpKszg4, (unsigned char *)WORK0, (unsigned char *)WORK1, 5, 0 );
//---------------
				if( m_Arith.l_cmp(WORK1, PER100BUF) < 0 ){
					m_Util.l_pardotnet_syz( stval.SpKszg4, WORK0, stval.SpKuws, stval.SpKuwb );
				}
				else{
					m_Util.l_pardotnet_syz( stval.SpKszg4, WORK0, stval.SpKuws, stval.SpKuws );
				}
//---------------
				// 特定収入税額 合計
				m_Arith.l_add( stval.SpGszg4, stval.SpTszg4, stval.SpKszg4 );
			}
		}
		else {
			// 控除対象仕入税額 （一括比例配分方式）
			if( zeiType == ID_ICSSH_SPC_8PER ){ 
				// 特定収入税額＝（特定収入＋共特定収入）ｘ 63/1080
				memset( stval.SpKszg63, '\0', sizeof(stval.SpKszg63) );
				memset( WORK0, '\0', sizeof(WORK0) );
				memset( WORK1, '\0', sizeof(WORK1) );
				m_Util.percent( WORK0, stval.SpTsyu63, 9, 63, 0 );				// 63/1080

//				memmove( stval.SpZszg, WORK0, sizeof(stval.SpZszg) );			// 売上割合計算前の税額
				memmove( stval.SpZszg63, WORK0, sizeof(stval.SpZszg63) );			// 売上割合計算前の税額	疑問？？？？

				// 特定収入税額Ｘ課税売上割合
				m_Arith.l_par100( (unsigned char *)WORK1, (unsigned char *)stval.SpKuws, (unsigned char *)stval.SpKuwb, 5, 0 );
				// 売上割合がマイナスの場合は０％で計算する。[03'05.17]
				memset( PW6, '\0', sizeof( PW6 ) );
				if( m_Arith.l_cmp( stval.SpKuws, PW6 ) < 0 ){
					memset( WORK1, '\0', sizeof(WORK1) );
				}
				if( m_Arith.l_cmp( stval.SpKuwb, PW6 ) < 0 ){
					memset( WORK1, '\0', sizeof(WORK1) );
				}

//-- '15.04.15 --
//				m_Arith.l_mul100( (unsigned char *)stval.SpTszg63, (unsigned char *)WORK0, (unsigned char *)WORK1, 5, 0 );
//---------------
				if( m_Arith.l_cmp(WORK1, PER100BUF) < 0 ){
					m_Util.l_pardotnet_syz( stval.SpTszg63, WORK0, stval.SpKuws, stval.SpKuwb );
				}
				else{
					m_Util.l_pardotnet_syz( stval.SpTszg63, WORK0, stval.SpKuws, stval.SpKuws );
				}
//---------------
				
				// 特定収入税額 合計
				m_Arith.l_add( stval.SpGszg63, stval.SpTszg63, stval.SpKszg63 );
			}
			else if( zeiType == ID_ICSSH_SPC_5PER ){
				// 特定収入税額＝（特定収入＋共特定収入）ｘ 40/1050
				memset( stval.SpKszg4, '\0', sizeof(stval.SpKszg4) );
				memset( WORK0, '\0', sizeof(WORK0) );
				memset( WORK1, '\0', sizeof(WORK1) );
				m_Util.percent( WORK0, stval.SpTsyu4, 4, 40, 0 );				// 40/1050

//				memmove( stval.SpZszg, WORK0, sizeof(stval.SpZszg) );			// 売上割合計算前の税額
				memmove( stval.SpZszg4, WORK0, sizeof(stval.SpZszg4) );			// 売上割合計算前の税額	疑問？？？？

				// 特定収入税額Ｘ課税売上割合
				m_Arith.l_par100( (unsigned char *)WORK1, (unsigned char *)stval.SpKuws, (unsigned char *)stval.SpKuwb, 5, 0 );
				// 売上割合がマイナスの場合は０％で計算する。[03'05.17]
				memset( PW6, '\0', sizeof( PW6 ) );
				if( m_Arith.l_cmp( stval.SpKuws, PW6 ) < 0 ){
					memset( WORK1, '\0', sizeof(WORK1) );
				}
				if( m_Arith.l_cmp( stval.SpKuwb, PW6 ) < 0 ){
					memset( WORK1, '\0', sizeof(WORK1) );
				}

//-- '15.04.15 --
//				m_Arith.l_mul100( (unsigned char *)stval.SpTszg4, (unsigned char *)WORK0, (unsigned char *)WORK1, 5, 0 );
//---------------
				if( m_Arith.l_cmp(WORK1, PER100BUF) < 0 ){
					m_Util.l_pardotnet_syz( stval.SpTszg4, WORK0, stval.SpKuws, stval.SpKuwb );
				}
				else{
					m_Util.l_pardotnet_syz( stval.SpTszg4, WORK0, stval.SpKuws, stval.SpKuws );
				}
//---------------
				
				// 特定収入税額 合計
				m_Arith.l_add( stval.SpGszg4, stval.SpTszg4, stval.SpKszg4 );
			}
		}
	}
	
	// 通常の控除額－特定収入税額
	memset( WORK0, '\0', sizeof(WORK0) );
	if( zeiType == ID_ICSSH_SPC_8PER ){
		m_Arith.l_sub( WORK0, stval.SpKgz63, stval.SpGszg63 );
	}
	else if( zeiType == ID_ICSSH_SPC_5PER ){
		m_Arith.l_sub( WORK0, stval.SpKgz4, stval.SpGszg4 );
	}

	// 調整割合額
	m_Arith.l_add( stval.SpTywb, stval.SpTuwb, stval.SpFsyu );
	memset( WORK2, '\0', sizeof(WORK2) );
	m_Arith.l_par100( (unsigned char *)WORK2, (unsigned char *)stval.SpFsyu, (unsigned char *)stval.SpTywb, 5, 0 );
	if( zeiType == ID_ICSSH_SPC_8PER ){
		m_Arith.l_mul100( (unsigned char *)stval.SpTwag63, (unsigned char *)WORK0, (unsigned char *)WORK2, 5, 0 );
	}
	else if( zeiType == ID_ICSSH_SPC_5PER ){
		m_Arith.l_mul100( (unsigned char *)stval.SpTwag4, (unsigned char *)WORK0, (unsigned char *)WORK2, 5, 0 );
	}

	// 減額調整額
	if( zeiType == ID_ICSSH_SPC_8PER ){
		m_Arith.l_add( stval.SpGgt63, stval.SpGszg63, stval.SpTwag63 );
	}
	else if( zeiType == ID_ICSSH_SPC_5PER ){
		m_Arith.l_add( stval.SpGgt4, stval.SpGszg4, stval.SpTwag4 );
	}

	// 控除対象仕入税額
	if( zeiType == ID_ICSSH_SPC_8PER ){
		m_Arith.l_sub( stval.SpSiz63, stval.SpKgz63, stval.SpGgt63 );
	}
	else if( zeiType == ID_ICSSH_SPC_5PER ){
		m_Arith.l_sub( stval.SpSiz4, stval.SpKgz4, stval.SpGgt4 );
	}
}

#ifdef _20150313_CUT_START_
//-----------------------------------------------------------------------------
// 特定収入にかかる調整税額計算(経過措置無し)
//-----------------------------------------------------------------------------
// 引数	pSnHeadData	：	ヘッダ情報
//-----------------------------------------------------------------------------
void CH26SpcListData::SPassOffCalqEx( CSnHeadData *pSnHeadData )
{
	char	WORK0[MONY_BUF_SIZE]={0}, WORK1[MONY_BUF_SIZE]={0}, WORK2[MONY_BUF_SIZE]={0}, PW0[MONY_BUF_SIZE]={0}, PW6[MONY_BUF_SIZE]={0}; 
	char	moji[64]={0};

	if( pSnHeadData->IsKobetuSiireAnbun() == FALSE ){
		memset( stval.SpHsyu, '\0', sizeof(stval.SpHsyu) );
	}

	// 特定収入割合
	// 分母
	memset( stval.SpTswb, '\0', sizeof(stval.SpTswb) );
	m_Arith.l_add( stval.SpTswb, stval.SpTuwb, stval.SpGsyu );

	// 特定収入割合
	m_Arith.l_par100( (unsigned char*)stval.SpTswr, (unsigned char*)stval.SpGsyu, (unsigned char*)stval.SpTswb, 1, 9 );
	// 調整割合
	m_Arith.l_add( stval.SpTywb, stval.SpTuwb, stval.SpFsyu );
	m_Arith.l_par100( (unsigned char *)stval.SpTwar, (unsigned char *)stval.SpFsyu,	(unsigned char *)stval.SpTywb, 2, 0 );
	// 特定収入に係る税額
	memset( PW0, '\0', sizeof(PW0) );
	wsprintf( moji, _T("%d"), pSnHeadData->m_UriRatioBorder );
	m_Arith.l_input( PW0, moji );
	if( m_Arith.l_cmp( stval.SpUwri, PW0 ) >= 0 ) {	// 課税売上割合９５％以上
		memset( WORK0, '\0', sizeof(WORK0) );
		m_Arith.l_add( WORK0, WORK0, stval.SpTsyu );
		m_Arith.l_add( WORK0, WORK0, stval.SpKsyu );
		m_Arith.l_add( WORK0, WORK0, stval.SpHsyu );
		// 特定収入税額 合計
		m_Util.percent( stval.SpGszg, WORK0, 9, 63, 0 );					// 63/1080
	}
	else{									// 課税売上割合９５％以下
		if( pSnHeadData->IsKobetuSiireAnbun() ){
			// 控除対象仕入税額 （個別対応方式）
			// 特定収入税額
			m_Util.percent( stval.SpTszg, stval.SpTsyu, 9, 63, 0 );			// 63/1080
			// 共通に係る特定収入税額Ｘ課税売上割合
			memset( WORK0, '\0', sizeof(WORK0) );
			memset( WORK1, '\0', sizeof(WORK1) );
			m_Util.percent( WORK0, stval.SpKsyu, 9, 63, 0 );				// 63/1080

			memmove( stval.SpZszg, WORK0, sizeof(stval.SpZszg) );					// 売上割合計算前の税額

			m_Arith.l_par100( (unsigned char *)WORK1, (unsigned char *)stval.SpKuws, (unsigned char *)stval.SpKuwb, 5, 0 );
			// 売上割合がマイナスの場合は０％で計算する。[03'05.17]
			memset( PW6, '\0', sizeof(PW6) );
			if( m_Arith.l_cmp( stval.SpKuws, PW6 ) < 0 ){
				memset( WORK1, '\0', sizeof(WORK1) );
			}
			if( m_Arith.l_cmp( stval.SpKuwb, PW6 ) < 0 ){
				memset( WORK1, '\0', sizeof(WORK1) );
			}

			m_Arith.l_mul100( (unsigned char *)stval.SpKszg, (unsigned char *)WORK0, (unsigned char *)WORK1, 5, 0 );
			// 特定収入税額 合計
			m_Arith.l_add( stval.SpGszg, stval.SpTszg, stval.SpKszg );
		}
		else {
			// 控除対象仕入税額 （一括比例配分方式）
			// 特定収入税額＝（特定収入＋共特定収入）ｘ 63/1080
			memset( stval.SpKszg, '\0', sizeof(stval.SpKszg) );
			memset( WORK0, '\0', sizeof(WORK0) );
			memset( WORK1, '\0', sizeof(WORK1) );
			m_Util.percent( WORK0, stval.SpTsyu, 9, 63, 0 );				// 63/1080

			memmove( stval.SpZszg, WORK0, sizeof(stval.SpZszg) );			// 売上割合計算前の税額

			// 特定収入税額Ｘ課税売上割合
			m_Arith.l_par100( (unsigned char *)WORK1, (unsigned char *)stval.SpKuws, (unsigned char *)stval.SpKuwb, 5, 0 );
			// 売上割合がマイナスの場合は０％で計算する。[03'05.17]
			memset( PW6, '\0', sizeof( PW6 ) );
			if( m_Arith.l_cmp( stval.SpKuws, PW6 ) < 0 ){
				memset( WORK1, '\0', sizeof(WORK1) );
			}
			if( m_Arith.l_cmp( stval.SpKuwb, PW6 ) < 0 ){
				memset( WORK1, '\0', sizeof(WORK1) );
			}

			m_Arith.l_mul100( (unsigned char *)stval.SpTszg, (unsigned char *)WORK0, (unsigned char *)WORK1, 5, 0 );
			
			// 特定収入税額 合計
			m_Arith.l_add( stval.SpGszg, stval.SpTszg, stval.SpKszg );
		}
	}
	// 通常の控除額－特定収入税額
	memset( WORK0, '\0', sizeof(WORK0) );
	m_Arith.l_sub( WORK0, stval.SpKgzg, stval.SpGszg );
	// 調整割合額
	m_Arith.l_add( stval.SpTywb, stval.SpTuwb, stval.SpFsyu );
	memset( WORK2, '\0', sizeof(WORK2) );
	m_Arith.l_par100( (unsigned char *)WORK2, (unsigned char *)stval.SpFsyu, (unsigned char *)stval.SpTywb, 5, 0 );
	m_Arith.l_mul100( (unsigned char *)stval.SpTwag, (unsigned char *)WORK0, (unsigned char *)WORK2, 5, 0 );

	// 減額調整額
	m_Arith.l_add( stval.SpGgtg, stval.SpGszg, stval.SpTwag );

	// 控除対象仕入税額
	m_Arith.l_sub( stval.SpSizg, stval.SpKgzg, stval.SpGgtg );

	if( pSnHeadData->Sn_Sign4 & 0x80 ){
		pSnHeadData->Sn_Sign2 &= 0xfd;
		memset( WORK0, '\0', sizeof(WORK0) );
		m_Arith.l_input( WORK0, _T("50") );
		pSnHeadData->Sn_Sign2 &= 0xfd;
		if( m_Arith.l_cmp( stval.SpTswr, WORK0 ) <= 0 ){
			pSnHeadData->Sn_Sign2 |= 0x02;
		}
		else{
			// 経過措置対象の課税資産の譲渡等無し
//			memset( Snd.Sn_2F21A, '\0', 6 );
//			memset( Snd.Sn_2F21B, '\0', 6 );
//			Snd.Sn_2F21sw &= 0xef;
//			memmove( Snd.Sn_2F21B, stval.SpSizg, 6 );
		}
	}
}

//-----------------------------------------------------------------------------
// 特定収入に係る調整税額計算(経過措置有り)
//-----------------------------------------------------------------------------
// 引数	pSnHeadData	：	ヘッダ情報
//-----------------------------------------------------------------------------
void CH26SpcListData::SPassOnCalqEx( CSnHeadData *pSnHeadData )
{ 
	char	WORK0[MONY_BUF_SIZE]={0}, WORK1[MONY_BUF_SIZE]={0}, WORK2[MONY_BUF_SIZE]={0}, PW0[MONY_BUF_SIZE]={0}, PW6[MONY_BUF_SIZE]={0};
	char	moji[64]={0};

	// 調整額計算用　課税売上割合（分母） 課税売上＋非課税売上（非課税有価証券全額分）
	if( pSnHeadData->IsKobetuSiireAnbun() == FALSE ){
		memset( stval.SpHsyu, '\0', sizeof(stval.SpHsyu) );
	}

	// 特定収入割合
	// 分母
	memset( stval.SpTswb, '\0', sizeof(stval.SpTswb) );
	m_Arith.l_add( stval.SpTswb, stval.SpTuwb, stval.SpGsyu );

	// 特定収入割合
	// [04'07.29]
	m_Arith.l_par100( (unsigned char*)stval.SpTswr, (unsigned char*)stval.SpGsyu, (unsigned char*)stval.SpTswb, 1, 9 );

	// 調整割合
	m_Arith.l_add( stval.SpTywb, stval.SpTuwb, stval.SpFsyu );
	m_Arith.l_par100( (unsigned char *)stval.SpTwar, (unsigned char *)stval.SpFsyu,	(unsigned char *)stval.SpTywb, 2, 0 );
	// 特定収入に係る税額
	memset( PW0, '\0', sizeof(PW0) );
	wsprintf( moji, _T("%d"), pSnHeadData->m_UriRatioBorder );
	m_Arith.l_input( PW0, moji );
	if( m_Arith.l_cmp(stval.SpUwri, PW0) >= 0 ){	// 課税売上割合９５％以上
		memset( WORK0, '\0', sizeof(WORK0) );
		m_Arith.l_add( WORK0, WORK0, stval.SpTsyu );
		m_Arith.l_add( WORK0, WORK0, stval.SpKsyu );
		m_Arith.l_add( WORK0, WORK0, stval.SpHsyu );
		// 特定収入税額 合計
		m_Util.percent( stval.SpGszg, WORK0, 4, 40, 0 );					// ４０／１０５０
	}
	else{									// 課税売上割合９５％以下
		if( pSnHeadData->IsKobetuSiireAnbun() == TRUE ){
			// 控除対象仕入税額 （個別対応方式）
			// 特定収入税額
			m_Util.percent( stval.SpTszg, stval.SpTsyu, 4, 40, 0 );	// ４０／１０５０
			// 共通に係る特定収入税額Ｘ課税売上割合
			memset( WORK0, '\0', sizeof(WORK0) );
			memset( WORK1, '\0', sizeof(WORK1) );
			m_Util.percent( WORK0, stval.SpKsyu, 4, 40, 0 );			// ４０／１０５０
			memmove( stval.SpZszg, WORK0, sizeof(stval.SpZszg) );			// 売上割合計算前の税額
			m_Arith.l_par100( (unsigned char *)WORK1, (unsigned char *)stval.SpKuws, (unsigned char *)stval.SpKuwb, 5, 0 );
			// 売上割合がマイナスの場合は０％で計算する。[03'05.17]
			memset( PW6, '\0', sizeof(PW6) );
			if( m_Arith.l_cmp(stval.SpKuws, PW6) < 0 ){
				memset( WORK1, '\0', sizeof(WORK1) );
			}
			if( m_Arith.l_cmp(stval.SpKuwb, PW6) < 0 ){
				memset( WORK1, '\0', sizeof(WORK1) );
			}

			m_Arith.l_mul100( (unsigned char *)stval.SpKszg, (unsigned char *)WORK0, (unsigned char *)WORK1, 5, 0 );
			// 特定収入税額 合計
			m_Arith.l_add( stval.SpGszg, stval.SpTszg, stval.SpKszg );
		}
		else{
			// 控除対象仕入税額 （一括比例配分方式）
			// 特定収入税額＝（特定収入＋共特定収入）ｘ４０／１０５０
			memset( WORK0, '\0', sizeof(WORK0) );
			memset( WORK1, '\0', sizeof(WORK1) );
			m_Util.percent( WORK0, stval.SpTsyu, 4, 40, 0 );	// ４０／１０５０
			memmove( stval.SpZszg, WORK0, sizeof(stval.SpZszg) );			// 売上割合計算前の税額
			// 特定収入税額Ｘ課税売上割合
			m_Arith.l_par100( (unsigned char *)WORK1, (unsigned char *)stval.SpKuws, (unsigned char *)stval.SpKuwb, 5, 0 );
			// 売上割合がマイナスの場合は０％で計算する。[03'05.17]
			memset( PW6, '\0', sizeof( PW6 ) );
			if( m_Arith.l_cmp(stval.SpKuws, PW6) < 0 ){
				memset( WORK1, '\0', sizeof(WORK1) );
			}
			if( m_Arith.l_cmp(stval.SpKuwb, PW6) < 0 ){
				memset( WORK1, '\0', sizeof(WORK1) );
			}

			m_Arith.l_mul100( (unsigned char *)stval.SpTszg, (unsigned char *)WORK0, (unsigned char *)WORK1, 5, 0 );
			// 特定収入税額 合計
			m_Arith.l_add( stval.SpGszg, stval.SpTszg, stval.SpKszg );
		}
	}
	// 配賦割合
	m_Arith.l_add( stval.SpHfgs, stval.SpHfgs, stval.SpKsi4 );
	m_Arith.l_sub( stval.SpHfgs, stval.SpHfgs, stval.SpTsyu );
	m_Arith.l_sub( stval.SpHfgs, stval.SpHfgs, stval.SpKsb4 );

	m_Arith.l_add( stval.SpHfgb, stval.SpHfgb, stval.SpKsig );
	m_Arith.l_sub( stval.SpHfgb, stval.SpHfgb, stval.SpTsyu );
	m_Arith.l_sub( stval.SpHfgb, stval.SpHfgb, stval.SpKsbg );

	m_Arith.l_par100( (unsigned char *)stval.SpKshf, (unsigned char *)stval.SpHfgs,	(unsigned char *)stval.SpHfgb, 2, 0 );
	// 通常の控除額－特定収入税額
	m_Arith.l_sub( WORK0, stval.SpKgzg, stval.SpGszg );
	// 調整割合額
	m_Arith.l_add( stval.SpTywb, stval.SpTuwb, stval.SpFsyu );
	memset( WORK2, '\0', sizeof(WORK2) );
	m_Arith.l_par100( (unsigned char *)WORK2, (unsigned char *)stval.SpFsyu, (unsigned char *)stval.SpTywb, 5, 0 );
	m_Arith.l_mul100( (unsigned char *)stval.SpTwag, (unsigned char *)WORK0, (unsigned char *)WORK2, 5, 0 );

	// 新税率適用分の配布額

	// 配賦割合額
	memset( WORK2, '\0', sizeof(WORK2) );
	m_Arith.l_par100( (unsigned char *)WORK2, (unsigned char *)stval.SpTwag, (unsigned char *)stval.SpHfgb, 5, 0 );
	m_Arith.l_mul100( (unsigned char *)stval.SpKsh4, (unsigned char *)stval.SpHfgs, (unsigned char *)WORK2, 5, 0 );

	// 減額調整額
	m_Arith.l_add( stval.SpGgt4, stval.SpGszg, stval.SpKsh4 );
	// 控除対象仕入税額
	m_Arith.l_sub( stval.SpSiz4, stval.SpKgz4, stval.SpGgt4 );

	// 旧税率適用分の配布額

	// 特定収入に係る課税仕入れ等の合計
	memset( WORK0, '\0', sizeof(WORK0) );
	m_Arith.l_add( WORK0, stval.SpTwag, stval.SpGszg );
	m_Arith.l_sub( stval.SpKsh3, WORK0, stval.SpGgt4 );

	// 減額調整額
	m_Arith.l_add( stval.SpGgtg, stval.SpGszg, stval.SpTwag );

	// 控除対象仕入税額
	m_Arith.l_sub( stval.SpSiz3, stval.SpKgz3, stval.SpKsh3 );

	//
	m_Arith.l_add( stval.SpSizg, stval.SpSiz3, stval.SpSiz4 );
	
	if( pSnHeadData->Sn_Sign4 & 0x80 ) {
		pSnHeadData->Sn_Sign2 &= 0xfd;
		memset( WORK0, '\0', sizeof(WORK0) );
		m_Arith.l_input( WORK0, _T("50") );
		pSnHeadData->Sn_Sign2 &= 0xfd;
		if( m_Arith.l_cmp(stval.SpTswr, WORK0) <= 0 ){
			pSnHeadData->Sn_Sign2 |= 0x02;
		}
		else{
			// 経過措置対象の課税資産の譲渡等有り
//			memset( Snd.Sn_2F21A, '\0', 6 );
//			memset( Snd.Sn_2F21B, '\0', 6 );
			// 経過措置対象の課税資産の譲渡等有り
//			Snd.Sn_2F21sw &= 0xfe;
//			memmove( Snd.Sn_2F21A, stval.SpSiz3, 6 );
//			Snd.Sn_2F21sw &= 0xef;
//			memmove( Snd.Sn_2F21B, stval.SpSiz4, 6 );
		}
	}
}
#endif

//-----------------------------------------------------------------------------
// 科目名称の取得
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラス
//		Code		：	科目コード
//		Name		：	科目名称
//		Seq			：	科目名称テーブルでのシーケンス
//-----------------------------------------------------------------------------
// 返送値	1		：	取得
//			0		：	無し
//-----------------------------------------------------------------------------
int CH26SpcListData::GetKnrecNameEx( CDBNpSub *pDBNpSub, unsigned char *Code, char *Name, long &Seq )
{
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return 0;
	}

	int		rmx=0, rno=0, ans=0;
	char	Codebf[10]={0};

	// コード転送
	memset( Codebf, '\0', sizeof(Codebf) );
	memmove( Codebf, Code, 8 );

	// 大文字変換
	char	Asci[10]={0};
	memset( Asci, '\0', sizeof(Asci) );
	memmove( Asci, Codebf, 8 );
	_strupr_s( Asci, sizeof(Asci) );

	ans = 0;
	rmx = pDBNpSub->knrec->GetRecordCount();
	for( rno=0; rno<rmx ; rno++ ){
		pDBNpSub->knrec->SetAbsolutePosition( rno+1 );

		if( strncmp( pDBNpSub->knrec->kncod, Codebf, 8 ) ){
			continue;
		}
		memmove( Name, pDBNpSub->knrec->knnam, 14 );

		hl_rev( &rno, 4 );
		Seq = rno;

		ans = 1;
		break;
	}
	return ans;
}

//---------------------->yoshida150309
//-----------------------------------------------------------------------------
// DB からデータ読込み
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 返送値	1		：	
//			0		：	
//-----------------------------------------------------------------------------
int CH26SpcListData::GetData( CDBNpSub *pDBNpSub, CSnHeadData *pSnHeadData, int sn_seq )
{
	TSREC	TSrec;
	CString	filter;

//<-- '15.03.13 MOVE START
//	// 特定収入計算表
//	filter.Format( "spseq = %d", sn_seq );
//
//	if( pDBNpSub->NpTokuteiSyunyuKeisanOpen(filter) == ERR )	{
////		ErMs = pZaimuSub->GetZmErrMsg();
//		return( -1 );
//	}
//	int m_Splist_cnt = pDBNpSub->m_pSplist->GetRecordCountEx();
//--> '15.03.13 MOVE END

	// 初期設定
	memset( &Spval, '\0', sizeof( struct _SPCVAL ) );
	memset( &stval, '\0', sizeof( struct _STUVAL ) );
	if( ReInitTsTbl() ){
		return -1;
	}

	// 前回のマスターを取り消す
	if( !(pSnHeadData->Sn_Sign2&0x01) )	{
		// 取り消し
//-- '15.03.13 --
//		for( count = 0 ; count < m_Splist_cnt ; count++ ) {
//			pDBNpSub->m_pSplist->SetAbsolutePositionEx( count + 1 );
//			pDBNpSub->m_pSplist->DeleteEx();
//		}
//---------------
		if( pDBNpSub->NpTokuteiSyunyuKeisanOpen(filter) == ERR ){
			m_ErrMsg = pDBNpSub->GetZmErrMsg();
			return -1;
		}
		try{
			CString	sql;
			sql.Format( _T("delete from dbo.splist where spseq=%d"), sn_seq );
			pDBNpSub->m_database->ExecuteSQL( sql );
		}
		catch( CDBException *pde ){
			m_ErrMsg = pde->m_strError;
			pde->Delete();
			return -1;
		}
		pDBNpSub->NpTokuteiSyunyuKeisanClose();
//---------------
		pSnHeadData->Sn_Sign2 |= 0x01;
		return 0;
	}

	// 特定収入計算表
	filter.Format( _T("spseq = %d"), sn_seq );

	if( pDBNpSub->NpTokuteiSyunyuKeisanOpen(filter) == ERR ){
		m_ErrMsg = pDBNpSub->GetZmErrMsg();
		return -1;
	}

	// 消費税集計マスターの読み込み
	int count = 0;
	while( 1 ){

		// 最終レコードチェック
//		if( m_Splist_cnt == count )	break;		// 最終チェック

		// レコードの読み込み
		if( count == 0 ){
			if( pDBNpSub->m_pSplist->MoveFirstEx() ){
				break;
			}
		}
		else{
			if( pDBNpSub->m_pSplist->MoveNextEx() ){
				break;
			}
		}
		count++;
	
		if ( pDBNpSub->m_pSplist->m_spitem == 1 ) {

			//Spvalに読み込まずに直接stvalに読み込む。旧ではSpvalは保存用の為だけに使用されている感じでどこでも使用されていなかった為。
			// 計算表２以外の計算表
			char	*pVal = NULL;
			switch( pDBNpSub->m_pSplist->m_spitln )	{	// 行
				case 1 :	pVal = stval.SpKuws;	break;		// 課税売上高割合の分子（全額）
				case 2 :	pVal = stval.SpKuwb;	break;		// 課税売上高割合の分母（全額）
				case 3 :	pVal = stval.SpUwri;	break;		// 課税売上割合（少数点代位２位）
				case 4 :	pVal = stval.SpKzur;	break;		// 課税売上（免税・非課税売上・値引を含まない）
//2017.03.01 INSERT START
				case 401 :	pVal = stval.SpKzur4;	break;		// 課税売上（免税・非課税売上・値引を含まない）
				case 402 :	pVal = stval.SpKzur63;	break;		// 課税売上（免税・非課税売上・値引を含まない）
//2017.03.01 INSERT END
				case 5 :	pVal = stval.SpMzur;	break;		// 免税売上
				case 6 :	pVal = stval.SpHkur;	break;		// 非課税売上（有価証券を全額で含む）										// 
				case 7 :	pVal = stval.SpUnbg;	break;		// 売上返還　合計
				case 8 :										// 〃　　　　３％
					if( m_isTransitionalMeasure ){
						pVal = stval.SpUnb3;
					}
					break;
				case 9 :										// 〃		 ４％
					if( m_isTransitionalMeasure ){
						pVal = stval.SpUnb4;
					}
					break;
				case 901 :	pVal = stval.SpUnb63;	break;		// 〃		 ６．３％

				case 10:	pVal = stval.SpKgzg;	break;		// 通常の控除対象仕入税額の合計（比例／個別）
				case 11:										// 通常の控除対象仕入税額の３％（個別）
					if( m_isTransitionalMeasure ){
						pVal = stval.SpKgz3;
					}
					break;
				case 12:										// 通常の控除対象仕入税額の４％（個別）
					if( m_isTransitionalMeasure ){
						pVal = stval.SpKgz4;
					}
					break;
				case 1201:	pVal = stval.SpKgz63;	break;		// 通常の控除対象仕入税額の６．３％（個別）

				case 13:	pVal = stval.SpYuka;	break;		// 非課税有価証券　全額分
				case 14:	pVal = stval.SpYuk5;	break;		// 〃　　　　　　　５％分

				case 15:	pVal = stval.SpTsyu;	break;		// 特定収入						（比例／個別）
				case 1501:										// 特定収入合計	４％			（比例／個別）
					if( m_isTransitionalMeasure ){
						pVal = stval.SpTsyu4;
					}
					break;
				case 1502:	pVal = stval.SpTsyu63;	break;		// 特定収入合計	６．３％		（比例／個別）

				case 16:	pVal = stval.SpKsyu;	break;		// 特定収入（共通に係る）		（個別）
				case 1601:										// 不特定収入	４％			（比例／個別）
					if( m_isTransitionalMeasure ){
						pVal = stval.SpKsyu4;
					}
					break;
				case 1602:	pVal = stval.SpKsyu63;	break;		// 不特定収入	６．３％		（比例／個別）

				case 17:	pVal = stval.SpFsyu;	break;		// 不特定収入					（比例／個別）					
				case 1701:										// 不特定収入	４％			（比例／個別）
					if( m_isTransitionalMeasure ){
						pVal = stval.SpFsyu4;
					}
					break;
				case 1702:	pVal = stval.SpFsyu63;	break;		// 不特定収入	６．３％		（比例／個別）					

				case 18:	pVal = stval.SpGsyu;	break;		// 特定収入合計					（比例／個別）
				case 1801:										// 特定収入合計	４％			（比例／個別）
					if( m_isTransitionalMeasure ){
						pVal = stval.SpGsyu4;
					}
					break;
				case 1802:	pVal = stval.SpGsyu63;	break;		// 特定収入合計	６．３％		（比例／個別）

				case 19:	pVal = stval.SpKsig;	break;		// 課税仕入れ等の金額の合計・税込み	（個別）
				case 20:										// 課税仕入れ等の金額の３％・税込み	（個別）
					if( m_isTransitionalMeasure ){
						pVal = stval.SpKsi3;
					}
					break;
				case 21:										// 課税仕入れ等の金額の４％・税込み	（個別）
					if( m_isTransitionalMeasure ){
						pVal = stval.SpKsi4;
					}
					break;
				case 2101:	pVal = stval.SpKsi63;	break;		// 課税仕入れ等の金額の６．３％・税込み	（個別）

				case 22:	pVal = stval.SpKsbg;	break;		// 課税仕入れ等に係る対価の返還等の金額の合計・税込み（個別）
				case 23:										// 課税仕入れ等に係る対価の返還等の金額の３％・税込み（個別）
					if( m_isTransitionalMeasure ){
						pVal = stval.SpKsb3;
					}
					break;
				case 24:										// 課税仕入れ等に係る対価の返還等の金額の４％・税込み（個別）
					if( m_isTransitionalMeasure ){
						pVal = stval.SpKsb4;
					}
					break;
				case 2401:	pVal = stval.SpKsb63;	break;		// 課税仕入れ等に係る対価の返還等の金額の６．３％・税込み（個別）

				// 内部で計算
				case 25:	pVal = stval.SpGszg;	break;		// 特定収入税額合計				（比例／個別）
				case 2501:										// 特定収入税額合計	４％		（比例／個別）
					pVal = stval.SpGszg4;
					break;
				case 2502:	pVal = stval.SpGszg63;	break;		// 特定収入税額合計	６．３％	（比例／個別）

				case 26:	pVal = stval.SpTszg;	break;		// 特定収入税額					（比例／個別）
				case 2601:										// 特定収入税額	４％			（比例／個別）
					if( m_isTransitionalMeasure ){
						pVal = stval.SpTszg4;
					}
					break;
				case 2602:	pVal = stval.SpTszg63;	break;		// 特定収入税額	６．３％		（比例／個別）

				case 27:	pVal = stval.SpKszg;	break;		// 共特定収入税額				（個別）
				case 2701:										// 共特定収入税額 ４％			（個別）
					if( m_isTransitionalMeasure ){
						pVal = stval.SpKszg4;
					}
					break;
				case 2702:	pVal = stval.SpKszg63;	break;		// 共特定収入税額 ６．３％		（個別）

				case 28:	pVal = stval.SpZszg;	break;		// 売上割合計算前の税額
				case 2801:										// 売上割合計算前の税額３％
					if( m_isTransitionalMeasure ){
						pVal = stval.SpZszg3;
					}
					break;
				case 2802:										// 売上割合計算前の税額４％
					if( m_isTransitionalMeasure ){
						pVal = stval.SpZszg4;
					}
					break;
				case 2803:	pVal = stval.SpZszg63;	break;		// 売上割合計算前の税額６．３％

				case 29:	pVal = stval.SpTuwb;	break;		// 特定割合及び、調整割合計算用の分母の基礎
				case 30:	pVal = stval.SpTswb;	break;		// 特定収入割合の分母（全額）
				case 31:	pVal = stval.SpTywb;	break;		// 調整割合の分母　　（全額）
				case 32:	pVal = stval.SpTswr;	break;		// 特定収入割	（少数点代位２位）
				case 33:	pVal = stval.SpTwar;	break;		// 調整割合		（少数点代位２位）

				case 34:	pVal = stval.SpTwag;	break;		// 調整割合額
				case 3401:										// 調整割合額３％
					if( m_isTransitionalMeasure ){
						pVal = stval.SpTwag3;
					}
					break;
				case 3402:										// 調整割合額４％
					if( m_isTransitionalMeasure ){
						pVal = stval.SpTwag4;
					}
					break;
				case 3403:	pVal = stval.SpTwag63;	break;		// 調整割合額６．３％

				case 35:	pVal = stval.SpKshf;	break;		// 配賦割合		（少数点代位２位）
				case 36:										// 配賦額３％
					if( m_isTransitionalMeasure ){
						pVal = stval.SpKsh3;
					}
					break;		
				case 37:										// 配賦額４％
					if( m_isTransitionalMeasure ){
						pVal = stval.SpKsh4;
					}
					break;
				case 3701:	pVal = stval.SpKsh63;	break;		// 配賦額６．３％

				case 38:	pVal = stval.SpHfgs;	break;		// 配賦割合の分子（全額）
				case 39:	pVal = stval.SpHfgb;	break;		// 配賦割合の分母（全額）
				case 40:	pVal = stval.SpGgtg;	break;		// 減額調整額の合計				（比例／個別）
				case 41:										// 減額調整額の３％				（個別）
					if( m_isTransitionalMeasure ){
						pVal = stval.SpGgt3;
					}
					break;
				case 42:										// 減額調整額の４％				（個別）
					if( m_isTransitionalMeasure ){
						pVal = stval.SpGgt4;
					}
					break;
				case 4201:	pVal = stval.SpGgt63;	break;		// 減額調整額の６．３％			（個別）

				case 43:	pVal = stval.SpSizg;	break;		// 特例計算後の控除対象仕入税額合計（比例／個別）
				case 44:										// 特例計算後の控除対象仕入税額３％（個別）
					if( m_isTransitionalMeasure ){
						pVal = stval.SpSiz3;
					}
					break;
				case 45:										// 特例計算後の控除対象仕入税額４％（個別）
					if( m_isTransitionalMeasure ){
						pVal = stval.SpSiz4;
					}
					break;
				case 4501:	pVal = stval.SpSiz63;	break;		// 特例計算後の控除対象仕入税額６．３％（個別）

				case 46:	pVal = stval.SpHsyu;	break;		// 特定収入（非課税に係る）
				case 4601:										// 特定収入（非課税に係る）４％
					if( m_isTransitionalMeasure ){
						pVal = stval.SpHsyu4;
					}
					break;
				case 4602:	pVal = stval.SpHsyu63;	break;		// 特定収入（非課税に係る）６．３％

				case 47:	pVal = stval.SpHszg;	break;		// 特定収入（非課税に係る税額）
				case 4701:										// 特定収入（非課税に係る税額）４％
					if( m_isTransitionalMeasure ){
						pVal = stval.SpHszg4;
					}
					break;
				case 4702:	pVal = stval.SpHszg63;	break;		// 特定収入（非課税に係る税額）６．３％

//--> '15.04.25 INS START
				case 48:	pVal = stval.SpSsyu;	break;		// 特定収入	(課税に係る)
				case 4801:										// 特定収入	(課税に係る) ４％
					if( m_isTransitionalMeasure ){
						pVal = stval.SpSsyu4;
					}
					break;
				case 4802:	pVal = stval.SpSsyu63;	break;		// 特定収入	(課税に係る) ６．３％
//<-- '15.04.25 INS END
//2017.03.01 INSERT START
				case 49:	pVal = stval.SpKgjt;	break;		//国外における資産の譲渡等の対価の額
//2017.03.01 INSERT END
				default:	break;

			}
			if( pVal != NULL ) {
				m_Util.val_to_bin( ( unsigned char *)pVal, pDBNpSub->m_pSplist->m_spitval );
			}
		}

		if( (pDBNpSub->m_pSplist->m_spitem==2) && (pDBNpSub->m_pSplist->m_spitcl==0) ){
			// 計算表２
			memset( &TSrec, '\0', sizeof( TSREC ) );
			char	buf[128] = {0};
			TSrec.TsLine = pDBNpSub->m_pSplist->m_spitln;
			memmove( TSrec.TsName, pDBNpSub->m_pSplist->m_spitnm, 40 ); 
			if( m_TblHdl.th_acs( thWritex, &TSrec, &TsTbl ) )	{
				pDBNpSub->NpTokuteiSyunyuKeisanClose();
		//		errmes = _T("特定収入計算表のマスターの読み込み中に窓外が発生しました。");
				return -1;
			}
		}

	}

	// クエリによる絞り込み
	filter.Format( _T("spitem=2 and spseq=%d"), sn_seq );
	pDBNpSub->m_pSplist->RequeryEx( filter );


	// 特定収入レコード作成
	for( int i=0;;i++ ){
		// レコードの読み込み
		if( i == 0 ){
			if( pDBNpSub->m_pSplist->MoveFirstEx() ){
				break;
			}
		}
		else{
			if( pDBNpSub->m_pSplist->MoveNextEx() ){
				break;
			}
		}
		
		// レコード作成
		memset( &TSrec, '\0', sizeof(TSrec) );
		TSrec.TsLine = pDBNpSub->m_pSplist->m_spitln;

		if( m_TblHdl.th_read('s', &TSrec, &TsTbl) == -1 ){
			continue;
		}

		char	*pVal = NULL;
		switch( pDBNpSub->m_pSplist->m_spitcl ){
			case 1 :	// 資産の譲渡等の対価以外の収入（実額入力）
				pVal = TSrec.TsJsyu;
				break;

			case 2 :	// 特定収入		比例
				if( m_isTransitionalMeasure ){
					pVal = TSrec.TsDtl[0].TsTsyu;
				}
				break;
			case 3 :	// 不特定収入	比例
				if( m_isTransitionalMeasure ){
					pVal = TSrec.TsDtl[0].TsFsyu;
				}
				break;
			case 4 :	// 特定収入合計	比例
				if( m_isTransitionalMeasure ){
					pVal = TSrec.TsDtl[0].TsGsyu;
				}
				break;
			case 5 :	// 特定収入　　 個別　課税売上に係る
				if( m_isTransitionalMeasure ){
					if( pSnHeadData->IsKobetuSiireAnbun() ){
						pVal = TSrec.TsDtl[0].TsSsyu;
					}
				}
				break;
			case 6 :	// 〃			〃　　非課税に係る
				if( m_isTransitionalMeasure ){
					if( pSnHeadData->IsKobetuSiireAnbun() ){
						pVal = TSrec.TsDtl[0].TsHsyu;
					}
				}
				break;
			case 7 :	// 〃			〃　　共通にかかつ
				if( m_isTransitionalMeasure ){
					if( pSnHeadData->IsKobetuSiireAnbun() ){
						pVal = TSrec.TsDtl[0].TsKsyu;
					}
				}
				break;
			case 8 :	// 外特定収入　 比例
				if( m_isTransitionalMeasure ){
					pVal = TSrec.TsDtl[0].TsNosyu;
				}
				break;

			case 9 :	// 特定収入		比例
				pVal = TSrec.TsDtl[1].TsTsyu;
				break;
			case 10 :	// 不特定収入	比例
				pVal = TSrec.TsDtl[1].TsFsyu;
				break;
			case 11 :	// 特定収入合計	比例
				pVal = TSrec.TsDtl[1].TsGsyu;
				break;
			case 12 :	// 特定収入　　 個別　課税売上に係る
				if( pSnHeadData->IsKobetuSiireAnbun() ){
					pVal = TSrec.TsDtl[1].TsSsyu;
				}
				break;
			case 13 :	// 〃			〃　　非課税に係る
				if( pSnHeadData->IsKobetuSiireAnbun() ){
					pVal = TSrec.TsDtl[1].TsHsyu;
				}
				break;
			case 14 :	// 〃			〃　　共通にかかつ
				if( pSnHeadData->IsKobetuSiireAnbun() ){
					pVal = TSrec.TsDtl[1].TsKsyu;
				}
				break;
			case 15 :	// 外特定収入　 比例
				pVal = TSrec.TsDtl[1].TsNosyu;
				break;
			default:
				break;							
		}
		if( pVal != NULL ){
			m_Util.val_to_bin( ( unsigned char *)pVal, pDBNpSub->m_pSplist->m_spitval );
		}
		if( m_TblHdl.th_update(&TSrec, &TSrec, &TsTbl) == -1 ){
			pDBNpSub->NpTokuteiSyunyuKeisanClose();
			return -1;
		}
	}

	pDBNpSub->NpTokuteiSyunyuKeisanClose();

	// 合計額の計算
	for( int i=0; i<TsTbl.tp_rnum; i++ ){
		if( m_TblHdl.th_rget(&TSrec, &TsTbl, i) ){
			break;
		}
		// 合計額
		for( int j=0; j<TS_ZR_MAX; j++ ){
			m_Arith.l_add( TSrec.sumDtl.TsTsyu, TSrec.sumDtl.TsTsyu, TSrec.TsDtl[j].TsTsyu );
			m_Arith.l_add( TSrec.sumDtl.TsFsyu, TSrec.sumDtl.TsFsyu, TSrec.TsDtl[j].TsFsyu );
			m_Arith.l_add( TSrec.sumDtl.TsNosyu, TSrec.sumDtl.TsNosyu, TSrec.TsDtl[j].TsNosyu );
			m_Arith.l_add( TSrec.sumDtl.TsGsyu, TSrec.sumDtl.TsGsyu, TSrec.TsDtl[j].TsGsyu );
			m_Arith.l_add( TSrec.sumDtl.TsSsyu, TSrec.sumDtl.TsSsyu, TSrec.TsDtl[j].TsSsyu );
			m_Arith.l_add( TSrec.sumDtl.TsHsyu, TSrec.sumDtl.TsHsyu, TSrec.TsDtl[j].TsHsyu );
			m_Arith.l_add( TSrec.sumDtl.TsKsyu, TSrec.sumDtl.TsKsyu, TSrec.TsDtl[j].TsKsyu );
		}

		if( m_TblHdl.th_rput(&TSrec, &TsTbl, i) ){
			break;
		}
	}

	return 0;

}



//-----------------------------------------------------------------------------
// DB へデータ書込み
//-----------------------------------------------------------------------------
// 引数
//-----------------------------------------------------------------------------
// 返送値	1		：	
//			0		：	
//-----------------------------------------------------------------------------
int CH26SpcListData::SetData( CDBNpSub *pDBNpSub, CSnHeadData *pSnHeadData, int sn_seq )
{
	int		count = 0;

	TSREC	TSrec;
	char	Nam[20];
	CString	filter;

	// 特定収入計算表
//-- '15.04.25 --
//	filter.Format( "spseq = %d", sn_seq );
//---------------
	// ダミーフィルタ
	filter.Format( "spseq = %d", -1 );
//---------------
	if( pDBNpSub->NpTokuteiSyunyuKeisanOpen(filter) == ERR )	{
		m_ErrMsg = pDBNpSub->GetZmErrMsg();
		return( -1 );
	}

//-- '15.04.25 --
//	int m_Splist_cnt = pDBNpSub->m_pSplist->GetRecordCountEx();
//
//	// 取り消し
//	for( count = 0; count < m_Splist_cnt; count++ ) {
//		pDBNpSub->m_pSplist->SetAbsolutePositionEx( count+1 );
//		pDBNpSub->m_pSplist->DeleteEx();
//	}
//---------------
	// 既存データクリア
	try{
		CString	sql;
		sql.Format( _T("delete from dbo.splist where spseq=%d"), sn_seq );
		pDBNpSub->m_database->ExecuteSQL( sql );
	}
	catch( CDBException *pde ){
		m_ErrMsg = pde->m_strError;
		pde->Delete();
		return -1;
	}
	pDBNpSub->m_pSplist->RequeryEx( filter );
//---------------

	// 計算表
	for( count = 0; count < STUVALCNT; count++ ) {
		// 新規データ作成　送信
		pDBNpSub->m_pSplist->AddNewEx();
		pDBNpSub->m_pSplist->m_spseq  = sn_seq;
		pDBNpSub->m_pSplist->m_spitem = 1;
		pDBNpSub->m_pSplist->m_spitcl = 1;
		char	*pVal;
		switch( count )	{
			case 0:// 課税売上高割合の分子（全額）
				pVal = stval.SpKuws;
				pDBNpSub->m_pSplist->m_spitln = 1;
				break;

			case 1 :// 課税売上高割合の分母（全額）
				pVal = stval.SpKuwb;
				pDBNpSub->m_pSplist->m_spitln = 2;
				break;

			case 2 :// 課税売上割合（少数点代位２位）
				pVal = stval.SpUwri;
				pDBNpSub->m_pSplist->m_spitln = 3;
				break;

			case 3 :// 課税売上（免税・非課税売上・値引を含まない）
				pVal = stval.SpKzur;
				pDBNpSub->m_pSplist->m_spitln = 4;
				break;

			case 4 :// 免税売上
				pVal = stval.SpMzur;
				pDBNpSub->m_pSplist->m_spitln = 5;
				break;

			case 5 :// 非課税売上（有価証券を全額で含む）
				pVal = stval.SpHkur;
				pDBNpSub->m_pSplist->m_spitln = 6;
				break;

			case 6 :// 売上返還　合計
				pVal = stval.SpUnbg;
				pDBNpSub->m_pSplist->m_spitln = 7;
				break;

			case 7 :// 〃　　　　３％
				pVal = stval.SpUnb3;
				pDBNpSub->m_pSplist->m_spitln = 8;
				break;

			case 8 :// 〃		 ４％
				pVal = stval.SpUnb4;
				pDBNpSub->m_pSplist->m_spitln = 9;
				break;

			case 9 :// 〃		 ６．３％
				pVal = stval.SpUnb63;
				pDBNpSub->m_pSplist->m_spitln = 901;
				break;

			case 10 :// 通常の控除対象仕入税額の合計（比例／個別）
				pVal = stval.SpKgzg;
				pDBNpSub->m_pSplist->m_spitln = 10;
				break;

			case 11 :// 通常の控除対象仕入税額の３％（個別）
				pVal = stval.SpKgz3;
				pDBNpSub->m_pSplist->m_spitln = 11;
				break;

			case 12 :// 通常の控除対象仕入税額の４％（個別）
				pVal = stval.SpKgz4;
				pDBNpSub->m_pSplist->m_spitln = 12;
				break;

			case 13 :// 通常の控除対象仕入税額の６．３％（個別）
				pVal = stval.SpKgz63;
				pDBNpSub->m_pSplist->m_spitln = 1201;
				break;

			case 14 :// 非課税有価証券　全額分
				pVal = stval.SpYuka;
				pDBNpSub->m_pSplist->m_spitln = 13;
				break;

			case 15 :// 〃　　　　　　　５％分
				pVal = stval.SpYuk5;
				pDBNpSub->m_pSplist->m_spitln = 14;
				break;

			case 16 :// 特定収入						（比例／個別）
				pVal = stval.SpTsyu;
				pDBNpSub->m_pSplist->m_spitln = 15;
				break;

			case 17 :// 特定収入	４％				（比例／個別）
				pVal = stval.SpTsyu4;
				pDBNpSub->m_pSplist->m_spitln = 1501;
				break;

			case 18 :// 特定収入	６．３％			（比例／個別）
				pVal = stval.SpTsyu63;
				pDBNpSub->m_pSplist->m_spitln = 1502;
				break;

			case 19 :// 特定収入（共通に係る）			（個別）
				pVal = stval.SpKsyu;
				pDBNpSub->m_pSplist->m_spitln = 16;
				break;

			case 20 :// 特定収入（共通に係る）４％		（個別）
				pVal = stval.SpKsyu4;
				pDBNpSub->m_pSplist->m_spitln = 1601;
				break;

			case 21 :// 特定収入（共通に係る）６．３％	（個別）
				pVal = stval.SpKsyu63;
				pDBNpSub->m_pSplist->m_spitln = 1602;
				break;

			case 22 :// 不特定収入					（比例／個別）
				pVal = stval.SpFsyu;
				pDBNpSub->m_pSplist->m_spitln = 17;
				break;

			case 23 :// 不特定収入 ４％				（比例／個別）					
				pVal = stval.SpFsyu4;
				pDBNpSub->m_pSplist->m_spitln = 1701;
				break;

			case 24 :// 不特定収入 ６．３％			（比例／個別）
				pVal = stval.SpFsyu63;
				pDBNpSub->m_pSplist->m_spitln = 1702;
				break;

			case 25 :// 特定収入合計					（比例／個別）
				pVal = stval.SpGsyu;
				pDBNpSub->m_pSplist->m_spitln = 18;
				break;

			case 26 :// 特定収入合計	４％			（比例／個別）
				pVal = stval.SpGsyu4;
				pDBNpSub->m_pSplist->m_spitln = 1801;
				break;

			case 27 :// 特定収入合計	６．３％		（比例／個別）
				pVal = stval.SpGsyu63;
				pDBNpSub->m_pSplist->m_spitln = 1802;
				break;

			case 28 :// 課税仕入れ等の金額の合計・税込み	（個別）
				pVal = stval.SpKsig;
				pDBNpSub->m_pSplist->m_spitln = 19;
				break;

			case 29 :// 課税仕入れ等の金額の３％・税込み	（個別）
				pVal = stval.SpKsi3;
				pDBNpSub->m_pSplist->m_spitln = 20;
				break;

			case 30 :// 課税仕入れ等の金額の４％・税込み	（個別）
				pVal = stval.SpKsi4;
				pDBNpSub->m_pSplist->m_spitln = 21;
				break;

			case 31 :// 課税仕入れ等の金額の６．３％・税込み	（個別）
				pVal = stval.SpKsi63;
				pDBNpSub->m_pSplist->m_spitln = 2101;
				break;

			case 32 :// 課税仕入れ等に係る対価の返還等の金額の合計・税込み（個別）
				pVal = stval.SpKsbg;
				pDBNpSub->m_pSplist->m_spitln = 22;
				break;

			case 33 :// 課税仕入れ等に係る対価の返還等の金額の３％・税込み（個別）
				pVal = stval.SpKsb3;
				pDBNpSub->m_pSplist->m_spitln = 23;
				break;

			case 34 :// 課税仕入れ等に係る対価の返還等の金額の４％・税込み（個別）
				pVal = stval.SpKsb4;
				pDBNpSub->m_pSplist->m_spitln = 24;
				break;

			case 35 :// 課税仕入れ等に係る対価の返還等の金額の６．３％・税込み（個別）
				pVal = stval.SpKsb63;
				pDBNpSub->m_pSplist->m_spitln = 2401;
				break;

			case 36 :// 特定収入税額合計				（比例／個別）
				pVal = stval.SpGszg;
				pDBNpSub->m_pSplist->m_spitln = 25;
				break;

			case 37 :// 特定収入税額合計	４％		（比例／個別）
				pVal = stval.SpGszg4;
				pDBNpSub->m_pSplist->m_spitln = 2501;
				break;

			case 38 :// 特定収入税額合計	６．３％	（比例／個別）
				pVal = stval.SpGszg63;
				pDBNpSub->m_pSplist->m_spitln = 2502;
				break;

			case 39 :// 特定収入税額					（比例／個別）
				pVal = stval.SpTszg;
				pDBNpSub->m_pSplist->m_spitln = 26;
				break;

			case 40 :// 特定収入税額	 ４％			（比例／個別）
				pVal = stval.SpTszg4;
				pDBNpSub->m_pSplist->m_spitln = 2601;
				break;

			case 41 :// 特定収入税額	 ６．３％		（比例／個別）
				pVal = stval.SpTszg63;
				pDBNpSub->m_pSplist->m_spitln = 2602;
				break;

			case 42 :// 共特定収入税額				（個別）
				pVal = stval.SpKszg;
				pDBNpSub->m_pSplist->m_spitln = 27;
				break;

			case 43 :// 共特定収入税額 ４％			（個別）
				pVal = stval.SpKszg4;
				pDBNpSub->m_pSplist->m_spitln = 2701;
				break;

			case 44 :// 共特定収入税額 ６．３％		（個別）
				pVal = stval.SpKszg63;
				pDBNpSub->m_pSplist->m_spitln = 2702;
				break;

			case 45 :// 売上割合計算前の税額
				pVal = stval.SpZszg;
				pDBNpSub->m_pSplist->m_spitln = 28;
				break;

			case 46 :// 売上割合計算前の税額３％
				pVal = stval.SpZszg3;
				pDBNpSub->m_pSplist->m_spitln = 2801;
				break;

			case 47 :// 売上割合計算前の税額４％
				pVal = stval.SpZszg4;
				pDBNpSub->m_pSplist->m_spitln = 2802;
				break;

			case 48 :// 売上割合計算前の税額６．３％
				pVal = stval.SpZszg63;
				pDBNpSub->m_pSplist->m_spitln = 2803;
				break;

			case 49 :// 特定割合及び、調整割合計算用の分母の基礎
				pVal = stval.SpTuwb;
				pDBNpSub->m_pSplist->m_spitln = 29;
				break;

			case 50 :// 特定収入割合の分母（全額）
				pVal = stval.SpTswb;
				pDBNpSub->m_pSplist->m_spitln = 30;
				break;

			case 51 :// 調整割合の分母　　（全額）
				pVal = stval.SpTywb;
				pDBNpSub->m_pSplist->m_spitln = 31;
				break;

			case 52 :// 特定収入割	（少数点代位２位）
				pVal = stval.SpTswr;
				pDBNpSub->m_pSplist->m_spitln = 32;
				break;

			case 53 :// 調整割合		（少数点代位２位）
				pVal = stval.SpTwar;
				pDBNpSub->m_pSplist->m_spitln = 33;
				break;

			case 54 :// 調整割合額
				pVal = stval.SpTwag;
				pDBNpSub->m_pSplist->m_spitln = 34;
				break;

			case 55 :// 調整割合額３％
				pVal = stval.SpTwag3;
				pDBNpSub->m_pSplist->m_spitln = 3401;
				break;

			case 56 :// 調整割合額４％
				pVal = stval.SpTwag4;
				pDBNpSub->m_pSplist->m_spitln = 3402;
				break;

			case 57 :// 調整割合額６．３％
				pVal = stval.SpTwag63;
				pDBNpSub->m_pSplist->m_spitln = 3403;
				break;

			case 58 :// 配賦割合		（少数点代位２位）
				pVal = stval.SpKshf;
				pDBNpSub->m_pSplist->m_spitln = 35;
				break;

			case 59 :// 配賦額３％
				pVal = stval.SpKsh3;
				pDBNpSub->m_pSplist->m_spitln = 36;
				break;

			case 60 :// 配賦額４％
				pVal = stval.SpKsh4;
				pDBNpSub->m_pSplist->m_spitln = 37;
				break;

			case 61 :// 配賦額６．３％
				pVal = stval.SpKsh63;
				pDBNpSub->m_pSplist->m_spitln = 3701;
				break;

			case 62 :// 配賦割合の分子（全額）
				pVal = stval.SpHfgs;
				pDBNpSub->m_pSplist->m_spitln = 38;
				break;

			case 63 :// 配賦割合の分母（全額）
				pVal = stval.SpHfgb;
				pDBNpSub->m_pSplist->m_spitln = 39;
				break;

			case 64 :// 減額調整額の合計				（比例／個別）
				pVal = stval.SpGgtg;
				pDBNpSub->m_pSplist->m_spitln = 40;
				break;

			case 65 :// 減額調整額の３％				（個別）
				pVal = stval.SpGgt3;
				pDBNpSub->m_pSplist->m_spitln = 41;
				break;

			case 66 :// 減額調整額の４％				（個別）
				pVal = stval.SpGgt4;
				pDBNpSub->m_pSplist->m_spitln = 42;
				break;

			case 67 :// 減額調整額の６．３％			（個別）
				pVal = stval.SpGgt63;
				pDBNpSub->m_pSplist->m_spitln = 4201;
				break;

			case 68 :// 特例計算後の控除対象仕入税額合計（比例／個別）
				pVal = stval.SpSizg;
				pDBNpSub->m_pSplist->m_spitln = 43;
				break;

			case 69 :// 特例計算後の控除対象仕入税額３％（個別）
				pVal = stval.SpSiz3;
				pDBNpSub->m_pSplist->m_spitln = 44;
				break;

			case 70 :// 特例計算後の控除対象仕入税額４％（個別）
				pVal = stval.SpSiz4;
				pDBNpSub->m_pSplist->m_spitln = 45;
				break;

			case 71 :// 特例計算後の控除対象仕入税額６．３％（個別）
				pVal = stval.SpSiz63;
				pDBNpSub->m_pSplist->m_spitln = 4501;
				break;

			//== 合計 ==
			case 72 :// 特定収入（非課税に係る）
				pVal = stval.SpHsyu;
				pDBNpSub->m_pSplist->m_spitln = 46;
				break;

			case 73 :// 特定収入（非課税に係る）４％
				pVal = stval.SpHsyu4;
				pDBNpSub->m_pSplist->m_spitln = 4601;
				break;

			case 74 :// 特定収入（非課税に係る）６．３％
				pVal = stval.SpHsyu63;
				pDBNpSub->m_pSplist->m_spitln = 4602;
				break;

			case 75 :// 特定収入（非課税に係る税額）
				pVal = stval.SpHszg;
				pDBNpSub->m_pSplist->m_spitln = 47;
				break;

			case 76 :// 特定収入（非課税に係る税額）４％
				pVal = stval.SpHszg4;
				pDBNpSub->m_pSplist->m_spitln = 4701;
				break;

			case 77 :// 特定収入（非課税に係る税額）６．３％
				pVal = stval.SpHszg63;
				pDBNpSub->m_pSplist->m_spitln = 4702;
				break;

//--> '15.04.25 INS START
			case 78 :// 特定収入	(課税に係る)
				pVal = stval.SpSsyu;
				pDBNpSub->m_pSplist->m_spitln = 48;
				break;

			case 79 :// 特定収入	(課税に係る) ４％
				pVal = stval.SpSsyu4;
				pDBNpSub->m_pSplist->m_spitln = 4801;
				break;

			case 80 :// 特定収入	(課税に係る) ６．３％
				pVal = stval.SpSsyu63;
				pDBNpSub->m_pSplist->m_spitln = 4802;
				break;
//<-- '15.04.25 INS END
//2017.03.01 INSERT START
			case 81	://国外における資産の譲渡等の対価の額
				pVal = stval.SpKgjt;
				pDBNpSub->m_pSplist->m_spitln = 49;
				break;

			case 82 :// 課税売上（免税・非課税売上・値引を含まない）
				pVal = stval.SpKzur4;
				pDBNpSub->m_pSplist->m_spitln = 401;
				break;

			case 83 :// 課税売上（免税・非課税売上・値引を含まない）
				pVal = stval.SpKzur63;
				pDBNpSub->m_pSplist->m_spitln = 402;
				break;

//2017.03.01 INSERT END
			default:	break;			 
		}

		m_Util.val_to_asci( &pDBNpSub->m_pSplist->m_spitval, (unsigned char*)pVal );
		
		// 更新
		if( pDBNpSub->m_pSplist->UpdateEx() == ERR )	{
			pDBNpSub->NpTokuteiSyunyuKeisanClose();
	//		errmes = _T("特定収入計算表のマスターの書き込み中に窓外が発生しました。(0)");
			return( -1 );
		}
	}


	// 特定収入
	int rno = 0;
	

	while( 1 ) {
		if( rno == TsTbl.tp_rnum )	break;
		if( m_TblHdl.th_rget( &TSrec, &TsTbl, rno ) )	{
			pDBNpSub->NpTokuteiSyunyuKeisanClose();
		//	errmes = _T("特定収入計算表のマスターの書き込み中に窓外が発生しました。(1)");
			return( -1 );
		}

		int spitcl_pos = 0;

		// カラム０
		// 新規データ作成　送信
		pDBNpSub->m_pSplist->AddNewEx();
		pDBNpSub->m_pSplist->m_spseq  = sn_seq;
		pDBNpSub->m_pSplist->m_spitem = 2;
		pDBNpSub->m_pSplist->m_spitln = rno+1;
		pDBNpSub->m_pSplist->m_spitcl = spitcl_pos;	// 0
		

		// 科目名称
		pDBNpSub->m_pSplist->m_spitnm.Empty();

		memset( Nam, '\0', sizeof( Nam ) );
		memmove( Nam, TSrec.TsName, 14 );
		pDBNpSub->m_pSplist->m_spitnm = Nam;
		if( pDBNpSub->m_pSplist->UpdateEx() == ERR )	{
			pDBNpSub->NpTokuteiSyunyuKeisanClose();
//			errmes = _T("特定収入計算表のマスターの書き込み中に窓外が発生しました。(2)");
			return( -1 );
		}
		spitcl_pos++;

		
		// 資産の譲渡等の対価以外の収入（実額入力）
		pDBNpSub->m_pSplist->AddNewEx();
		pDBNpSub->m_pSplist->m_spseq  = sn_seq;
		pDBNpSub->m_pSplist->m_spitem = 2;
		pDBNpSub->m_pSplist->m_spitln = rno+1;
		pDBNpSub->m_pSplist->m_spitcl = spitcl_pos;	// 1
		m_Util.val_to_asci( &pDBNpSub->m_pSplist->m_spitval, (unsigned char*)TSrec.TsJsyu );
		if( pDBNpSub->m_pSplist->UpdateEx() == ERR )	{
			pDBNpSub->NpTokuteiSyunyuKeisanClose();
//			errmes = _T("特定収入計算表のマスターの書き込み中に窓外が発生しました。(3)");
			return( -1 );
		}
		spitcl_pos++;

		for(int i = 0 ; i < TS_ZR_MAX - 8 ; i++){//現状は５％と８％の分だけ回す。後に増えてきた時に引き算やめる。	
			// 特定収入		比例
			pDBNpSub->m_pSplist->AddNewEx();
			pDBNpSub->m_pSplist->m_spseq  = sn_seq;
			pDBNpSub->m_pSplist->m_spitem = 2;
			pDBNpSub->m_pSplist->m_spitln = rno+1;
			pDBNpSub->m_pSplist->m_spitcl = spitcl_pos;	// 2, 9
			m_Util.val_to_asci( &pDBNpSub->m_pSplist->m_spitval, (unsigned char*)TSrec.TsDtl[i].TsTsyu );
			if( pDBNpSub->m_pSplist->UpdateEx() == ERR )	{
				pDBNpSub->NpTokuteiSyunyuKeisanClose();
		//		errmes = _T("特定収入計算表のマスターの書き込み中に窓外が発生しました。(3)");
				return( -1 );
			}
			spitcl_pos++;
		
			// 不特定収入	比例
			pDBNpSub->m_pSplist->AddNewEx();
			pDBNpSub->m_pSplist->m_spseq  = sn_seq;
			pDBNpSub->m_pSplist->m_spitem = 2;
			pDBNpSub->m_pSplist->m_spitln = rno+1;
			pDBNpSub->m_pSplist->m_spitcl = spitcl_pos;	// 3, 10
			m_Util.val_to_asci( &pDBNpSub->m_pSplist->m_spitval, (unsigned char*)TSrec.TsDtl[i].TsFsyu );
			if( pDBNpSub->m_pSplist->UpdateEx() == ERR )	{
				pDBNpSub->NpTokuteiSyunyuKeisanClose();
	//			errmes = _T("特定収入計算表のマスターの書き込み中に窓外が発生しました(6)。");
				return( -1 );
			}
			spitcl_pos++;
	
			// 特定収入合計	比例
			pDBNpSub->m_pSplist->AddNewEx();
			pDBNpSub->m_pSplist->m_spseq  = sn_seq;
			pDBNpSub->m_pSplist->m_spitem = 2;
			pDBNpSub->m_pSplist->m_spitln = rno+1;
			pDBNpSub->m_pSplist->m_spitcl = spitcl_pos;	// 4, 11
			m_Util.val_to_asci( &pDBNpSub->m_pSplist->m_spitval, (unsigned char*)TSrec.TsDtl[i].TsGsyu );
			if( pDBNpSub->m_pSplist->UpdateEx() == ERR )	{
				pDBNpSub->NpTokuteiSyunyuKeisanClose();
		//		errmes = _T("特定収入計算表のマスターの書き込み中に窓外が発生しました。(7)");
				return( -1 );
			}
			spitcl_pos++;
	
			// 特定収入　（個別・課税売上にのみに係る）
			pDBNpSub->m_pSplist->AddNewEx();
			pDBNpSub->m_pSplist->m_spseq  = sn_seq;
			pDBNpSub->m_pSplist->m_spitem = 2;
			pDBNpSub->m_pSplist->m_spitln = rno+1;
			pDBNpSub->m_pSplist->m_spitcl = spitcl_pos;	// 5, 12
			m_Util.val_to_asci( &pDBNpSub->m_pSplist->m_spitval, (unsigned char*)TSrec.TsDtl[i].TsSsyu );
			if( pDBNpSub->m_pSplist->UpdateEx() == ERR )	{
				pDBNpSub->NpTokuteiSyunyuKeisanClose();
		//		errmes = _T("特定収入計算表のマスターの書き込み中に窓外が発生しました。(5)");
				return( -1 );
			}
			spitcl_pos++;
	
			// 特定収入		（個別・非課税売上げのみに係る）
			pDBNpSub->m_pSplist->AddNewEx();
			pDBNpSub->m_pSplist->m_spseq  = sn_seq;
			pDBNpSub->m_pSplist->m_spitem = 2;
			pDBNpSub->m_pSplist->m_spitln = rno+1;
			pDBNpSub->m_pSplist->m_spitcl = spitcl_pos;	// 6, 13
			m_Util.val_to_asci( &pDBNpSub->m_pSplist->m_spitval, (unsigned char*)TSrec.TsDtl[i].TsHsyu );
			if( pDBNpSub->m_pSplist->UpdateEx() == ERR )	{
				pDBNpSub->NpTokuteiSyunyuKeisanClose();
	//			errmes = _T("特定収入計算表のマスターの書き込み中に窓外が発生しました。(8)");
				return( -1 );
			}
			spitcl_pos++;
		
			// 特定収入		（個別・共通に係る）
			pDBNpSub->m_pSplist->AddNewEx();
			pDBNpSub->m_pSplist->m_spseq  = sn_seq;
			pDBNpSub->m_pSplist->m_spitem = 2;
			pDBNpSub->m_pSplist->m_spitln = rno+1;
			pDBNpSub->m_pSplist->m_spitcl = spitcl_pos;	// 7, 14
			m_Util.val_to_asci( &pDBNpSub->m_pSplist->m_spitval, (unsigned char*)TSrec.TsDtl[i].TsKsyu );
			if( pDBNpSub->m_pSplist->UpdateEx() == ERR )	{
				pDBNpSub->NpTokuteiSyunyuKeisanClose();
	//			errmes = _T("特定収入計算表のマスターの書き込み中に窓外が発生しました。(5)");
				return( -1 );
			}
			spitcl_pos++;
		
			// 外特定収入	比例
			pDBNpSub->m_pSplist->AddNewEx();
			pDBNpSub->m_pSplist->m_spseq  = sn_seq;
			pDBNpSub->m_pSplist->m_spitem = 2;
			pDBNpSub->m_pSplist->m_spitln = rno+1;
			pDBNpSub->m_pSplist->m_spitcl = spitcl_pos;	// 8, 15
			m_Util.val_to_asci( &pDBNpSub->m_pSplist->m_spitval, (unsigned char*)TSrec.TsDtl[i].TsNosyu );
			if( pDBNpSub->m_pSplist->UpdateEx() == ERR )	{
				pDBNpSub->NpTokuteiSyunyuKeisanClose();
	//			errmes = _T("特定収入計算表のマスターの書き込み中に窓外が発生しました(6)。");
				return( -1 );
			}
			spitcl_pos++;
		}
		rno++;
	}

	pDBNpSub->NpTokuteiSyunyuKeisanClose();

	return 0;
}
//<-----------------------------------

//-----------------------------------------------------------------------------
// 課税売上割合95%以上？	('15.03.20)
//-----------------------------------------------------------------------------
// 引数	pSnHeadData	：	
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	課税売上割合95%以上
//			FALSE	：	課税売上割合95%以上でない
//-----------------------------------------------------------------------------
BOOL CH26SpcListData::IsRatioOver95( CSnHeadData *pSnHeadData )
{
	BOOL	bRt = FALSE;

	ASSERT( pSnHeadData );
	if( pSnHeadData == NULL ){
		return bRt;
	}

	char	moji[64]={0};
	char	PW0[MONY_BUF_SIZE]={0};
	wsprintf( moji, _T("%d"), pSnHeadData->m_UriRatioBorder );
	m_Arith.l_input( PW0, moji );
	if( m_Arith.l_cmp(stval.SpUwri, PW0) >= 0 ){	// 課税売上割合９５％以上
		bRt = TRUE;
	}

	return bRt;
}

//-----------------------------------------------------------------------------
// 控除対象仕入税額の取込('15.04.03)
//-----------------------------------------------------------------------------
// 引数	pSyzToSpc	：	取込元データ
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CH26SpcListData::SetSkjUriMony( SYZTOSPC_DATA *pSyzToSpc )
{
	ASSERT( pSyzToSpc );
	if( pSyzToSpc == NULL ){
		return -1;
	}

//-- '15.05.18 --
//	memmove( stval.SpKzur, pSyzToSpc->SpKzur, sizeof(stval.SpKzur) );
//	memmove( stval.SpMzur, pSyzToSpc->SpMzur, sizeof(stval.SpMzur) );
//	memmove( stval.SpHkur, pSyzToSpc->SpHkur, sizeof(stval.SpHkur) );
//
//	m_Arith.l_add( stval.SpKuws, pSyzToSpc->SpKzur, pSyzToSpc->SpMzur );
//	m_Arith.l_add( stval.SpTuwb, pSyzToSpc->SpKzur, pSyzToSpc->SpMzur );
//	m_Arith.l_add( stval.SpTuwb, stval.SpTuwb, pSyzToSpc->SpHkur );
//	memmove( stval.SpKuwb, stval.SpTuwb, sizeof(stval.SpKuwb) );
//---------------
	memmove( stval.SpKzur, pSyzToSpc->SpKzur, sizeof(stval.SpKzur) );
	memmove( stval.SpMzur, pSyzToSpc->SpMzur, sizeof(stval.SpMzur) );
//	memmove( stval.SpHkur, pSyzToSpc->SpHkur, sizeof(stval.SpHkur) );
//2017.03.01 INSERT START
	memmove( stval.SpKzur4, pSyzToSpc->SpKzur4, sizeof(stval.SpKzur4) );
	memmove( stval.SpKzur63, pSyzToSpc->SpKzur63, sizeof(stval.SpKzur63) );
//2017.03.01 INSERT END

	m_Arith.l_add( stval.SpKuws, pSyzToSpc->SpKzur, pSyzToSpc->SpMzur );
	m_Arith.l_add( stval.SpTuwb, pSyzToSpc->SpKzur, pSyzToSpc->SpMzur );
	m_Arith.l_add( stval.SpTuwb, stval.SpTuwb, stval.SpHkur );
//2017.03.01 INSERT START
	m_Arith.l_add( stval.SpTuwb, stval.SpTuwb, stval.SpKgjt );
//2017.03.01 INSERT END

	m_Arith.l_add( stval.SpKuwb, pSyzToSpc->SpKzur, pSyzToSpc->SpMzur );
	m_Arith.l_add( stval.SpKuwb, stval.SpKuwb, pSyzToSpc->SpHkur );
//---------------
	m_Arith.l_par100( (unsigned char *)stval.SpUwri, (unsigned char *)stval.SpKuws, (unsigned char *)stval.SpKuwb, 2, 0 );

	memmove( stval.SpKgz3, pSyzToSpc->SpKgz3, sizeof(stval.SpKgz3) );
	memmove( stval.SpKgz4, pSyzToSpc->SpKgz4, sizeof(stval.SpKgz4) );
	memmove( stval.SpKgz63, pSyzToSpc->SpKgz63, sizeof(stval.SpKgz63) );

	return 0;
}

//-----------------------------------------------------------------------------
// 特定収入を加味した控除対象仕入税額の取得('15.04.04)
//-----------------------------------------------------------------------------
// 引数	pSyzToSpc	：	取得先データ
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CH26SpcListData::GetSkjMonyAfterSpc( SYZTOSPC_DATA *pSyzToSpc )
{
	ASSERT( pSyzToSpc );
	if( pSyzToSpc == NULL ){
		return -1;
	}

	memmove( pSyzToSpc->SpSiz3, stval.SpSiz3, sizeof(pSyzToSpc->SpSiz3) );
	memmove( pSyzToSpc->SpSiz4, stval.SpSiz4, sizeof(pSyzToSpc->SpSiz4) );
	memmove( pSyzToSpc->SpSiz63, stval.SpSiz63, sizeof(pSyzToSpc->SpSiz63) );

	return 0;
}
