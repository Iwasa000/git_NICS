
#include "stdafx.h"
#include "H31SpcListData.h"

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CH31SpcListData::CH31SpcListData( BOOL isTransitionalMeasure )
	: m_isTransitionalMeasure(isTransitionalMeasure)
{
	memset( &Spval, '\0', sizeof(_SPCVAL) );
	memset( &stval, '\0', sizeof(_STUVAL) );

	memset( &SpTbl, '\0', sizeof(TB_PAR) );
	SpTbl.tp_tno = -1;
	memset( &TsTbl, '\0', sizeof(TB_PAR) );
	TsTbl.tp_tno = -1;
// 24/02/14_特定収入 add -->
	memset( &TsTbl52, '\0', sizeof(TB_PAR) );
	TsTbl52.tp_tno = -1;
// 24/02/14_特定収入 add <--

	m_Arith.l_defn( 0x16 );
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CH31SpcListData::~CH31SpcListData()
{
	if( SpTbl.tp_tno != -1 ){
		m_TblHdl.th_close( SpTbl.tp_tno );
		SpTbl.tp_tno = -1;
	}
	if( TsTbl.tp_tno != -1 ){
		m_TblHdl.th_close( TsTbl.tp_tno );
		TsTbl.tp_tno = -1;
	}
// 24/02/14_特定収入 add -->
	if( TsTbl52.tp_tno != -1 ){
		m_TblHdl.th_close( TsTbl52.tp_tno );
		TsTbl52.tp_tno = -1;
	}
// 24/02/14_特定収入 add <--
}

//-----------------------------------------------------------------------------
// 特定収入集計テ－ブルの作成＆読み込み
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラスポインタ
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CH31SpcListData::InitSpData( CDBNpSub *pDBNpSub )
{
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}

	BOOL IsOpenFlg = FALSE;
	if( pDBNpSub->m_pSpsyk == NULL ){
		if( pDBNpSub->NpTokuteiSyunyuOpen() == -1 ){
			return -1;
		}
		IsOpenFlg = TRUE;
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

	if( IsOpenFlg ){
		pDBNpSub->NpTokuteiSyunyuClose();
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 特定収入計算テーブルの再初期化
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー
//-----------------------------------------------------------------------------
int CH31SpcListData::ReInitTsTbl()
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

// 24/02/29_特定収入 del -->
//// 24/02/14_特定収入 add -->
//	// 特定収入　計算表５－２のテーブル
//	if( TsTbl52.tp_tno != -1 ){
//		m_TblHdl.th_close( TsTbl52.tp_tno );
//	}
//	rsiz = (unsigned int)sizeof( TSREC52 );					// レコード長
//	rcnt = 200;												// 最大登録数
//	tsiz = (unsigned long)rsiz * (unsigned long)rcnt;
//
//	// テーブル イニシャライズ
//	if( (thd=m_TblHdl.th_open(tsiz, rsiz)) < 0 ){
//		return -1;
//	}
//	memset( &TsTbl52, '\0', sizeof(TB_PAR) );	
//	TsTbl52.tp_tno  = thd;
//	TsTbl52.tp_rmax = rcnt;
//	TsTbl52.tp_rnum = 0;
//	TsTbl52.tp_rlen = rsiz;
//	TsTbl52.tp_koff = 0;
//	TsTbl52.tp_klen = 0;			// キーなし
//	TsTbl52.tp_dupm = 0;
//	TsTbl52.tp_sufm = 0;
//	TsTbl52.tp_cp   = 0;
//	TsTbl52.tp_err  = 0;
//// 24/02/14_特定収入 add <--
// 24/02/29_特定収入 del <--

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
int CH31SpcListData::CalqForUriRatio( CDBNpSub *pDBNpSub, CSyzSyukei *pSyzSyukei, CSnHeadData *pSnHeadData, int smon, int emon )
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
	//１項目だけなので、再読み込みせず
	char sv_SpKgjt[6] = {0};
	memmove( sv_SpKgjt, stval.SpKgjt, sizeof(sv_SpKgjt) ); //国外における資産の譲渡等の対価の額 退避
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
		else if( SPrec.Sp_zkbn == 4 ){	// 8% ( 軽減 )
			zrIdx = 2;
		}
		else if( SPrec.Sp_zkbn == 5 ){	// 10%
			zrIdx = 3;
		}
		else{
			ASSERT( !_T("不正な税率！") );
			continue;
		}

		for( int j=smon; j<=emon; ++j ){
			m_Arith.l_add( stval.SpSsyu, stval.SpSsyu, &SPrec.Sp_tkzn[j] );
			// 〃　　　（非課税に係る）
			m_Arith.l_add( stval.SpHsyu, stval.SpHsyu, &SPrec.Sp_thzn[j] );
			// 〃　　　（共通に係る）
			m_Arith.l_add( stval.SpKsyu, stval.SpKsyu, &SPrec.Sp_tczn[j] );
			//	不特定収入
			m_Arith.l_add( stval.SpFsyu, stval.SpFsyu, &SPrec.Sp_tszn[j] );
			// 特定収入（課税に係る＋非課税に係る＋共通に係る）
			m_Arith.l_add( stval.SpTsyu, stval.SpTsyu, &SPrec.Sp_tkzn[j] );
			m_Arith.l_add( stval.SpTsyu, stval.SpTsyu, &SPrec.Sp_thzn[j] );
			m_Arith.l_add( stval.SpTsyu, stval.SpTsyu, &SPrec.Sp_tczn[j] );

			// 計算表２の明細	
			// 特定収入（課税売上に係る）
			m_Arith.l_add( TSrec.TsDtl[zrIdx].TsSsyu, TSrec.TsDtl[zrIdx].TsSsyu, &SPrec.Sp_tkzn[j] );
			// 〃　　　（非課税に係る）
			m_Arith.l_add( TSrec.TsDtl[zrIdx].TsHsyu, TSrec.TsDtl[zrIdx].TsHsyu, &SPrec.Sp_thzn[j] );
			// 〃　　　（共通に係る）
			m_Arith.l_add( TSrec.TsDtl[zrIdx].TsKsyu, TSrec.TsDtl[zrIdx].TsKsyu, &SPrec.Sp_tczn[j] );
			// 不特定収入
			m_Arith.l_add( TSrec.TsDtl[zrIdx].TsFsyu, TSrec.TsDtl[zrIdx].TsFsyu, &SPrec.Sp_tszn[j] );
			// 外特定収入
			m_Arith.l_add( TSrec.TsDtl[zrIdx].TsNosyu, TSrec.TsDtl[zrIdx].TsNosyu, &SPrec.Sp_tfzn[j] );
			// 特定収入（課税に係る＋非課税に係る＋共通に係る）★計算表の特殊合計用（個別対応専用）
			m_Arith.l_add( TSrec.TsDtl[zrIdx].TsTsyu, TSrec.TsDtl[zrIdx].TsTsyu,  &SPrec.Sp_tkzn[j] ); 
			m_Arith.l_add( TSrec.TsDtl[zrIdx].TsTsyu, TSrec.TsDtl[zrIdx].TsTsyu,  &SPrec.Sp_thzn[j] ); 
			m_Arith.l_add( TSrec.TsDtl[zrIdx].TsTsyu, TSrec.TsDtl[zrIdx].TsTsyu,  &SPrec.Sp_tczn[j] ); 
		}

		if( SPrec.Sp_zkbn == 3 ){
			for( int j=smon; j<=emon; ++j ){
				m_Arith.l_add( stval.SpSsyu63, stval.SpTsyu63, &SPrec.Sp_tkzn[j] );
				// 〃　　　（非課税に係る）
				m_Arith.l_add( stval.SpHsyu63, stval.SpHsyu63, &SPrec.Sp_thzn[j] );
				// 〃　　　（共通に係る）
				m_Arith.l_add( stval.SpKsyu63, stval.SpKsyu63, &SPrec.Sp_tczn[j] );
				//	不特定収入
				m_Arith.l_add( stval.SpFsyu63, stval.SpFsyu63, &SPrec.Sp_tszn[j] );
			}
		}
		else if( SPrec.Sp_zkbn == 0 ){
			for( int j=smon; j<=emon; ++j ){
				m_Arith.l_add( stval.SpSsyu4, stval.SpSsyu4, &SPrec.Sp_tkzn[j] );
				// 〃　　　（非課税に係る）
				m_Arith.l_add( stval.SpHsyu4, stval.SpHsyu4, &SPrec.Sp_thzn[j] );
				// 〃　　　（共通に係る）
				m_Arith.l_add( stval.SpKsyu4, stval.SpKsyu4, &SPrec.Sp_tczn[j] );
				//	不特定収入
				m_Arith.l_add( stval.SpFsyu4, stval.SpFsyu4, &SPrec.Sp_tszn[j] );
			}
		}
		else if( SPrec.Sp_zkbn == 4 ){	// 8% ( 軽減税率 )
			for( int j=smon; j<=emon; ++j ){
				m_Arith.l_add( stval.SpSsyu624, stval.SpSsyu624, &SPrec.Sp_tkzn[j] );
				// 〃　　　（非課税に係る）
				m_Arith.l_add( stval.SpHsyu624, stval.SpHsyu624, &SPrec.Sp_thzn[j] );
				// 〃　　　（共通に係る）
				m_Arith.l_add( stval.SpKsyu624, stval.SpKsyu624, &SPrec.Sp_tczn[j] );
				//	不特定収入
				m_Arith.l_add( stval.SpFsyu624, stval.SpFsyu624, &SPrec.Sp_tszn[j] );
			}
		}
		else if( SPrec.Sp_zkbn == 5 ){	// 10%
			for( int j=smon; j<=emon; ++j ){
				m_Arith.l_add( stval.SpSsyu78, stval.SpSsyu78, &SPrec.Sp_tkzn[j] );
				//---------------
				// 〃　　　（非課税に係る）
				m_Arith.l_add( stval.SpHsyu78, stval.SpHsyu78, &SPrec.Sp_thzn[j] );
				// 〃　　　（共通に係る）
				m_Arith.l_add( stval.SpKsyu78, stval.SpKsyu78, &SPrec.Sp_tczn[j] );
				//	不特定収入
				m_Arith.l_add( stval.SpFsyu78, stval.SpFsyu78, &SPrec.Sp_tszn[j] );
			}
		}

		// 計算表２の明細
		m_Arith.l_neg( TSrec.TsDtl[zrIdx].TsTsyu );
		m_Arith.l_neg( TSrec.TsDtl[zrIdx].TsHsyu );
		m_Arith.l_neg( TSrec.TsDtl[zrIdx].TsKsyu );
		m_Arith.l_neg( TSrec.TsDtl[zrIdx].TsFsyu );
		m_Arith.l_neg( TSrec.TsDtl[zrIdx].TsSsyu );
		m_Arith.l_neg( TSrec.TsDtl[zrIdx].TsNosyu );
		m_Arith.l_add( TSrec.TsDtl[zrIdx].TsGsyu, TSrec.TsDtl[zrIdx].TsGsyu, TSrec.TsDtl[zrIdx].TsSsyu );
		m_Arith.l_add( TSrec.TsDtl[zrIdx].TsGsyu, TSrec.TsDtl[zrIdx].TsGsyu, TSrec.TsDtl[zrIdx].TsHsyu );
		m_Arith.l_add( TSrec.TsDtl[zrIdx].TsGsyu, TSrec.TsDtl[zrIdx].TsGsyu, TSrec.TsDtl[zrIdx].TsKsyu );
		m_Arith.l_add( TSrec.TsDtl[zrIdx].TsGsyu, TSrec.TsDtl[zrIdx].TsGsyu, TSrec.TsDtl[zrIdx].TsFsyu );

		if( !m_Arith.l_test( TSrec.TsDtl[zrIdx].TsSsyu ) && 
			!m_Arith.l_test( TSrec.TsDtl[zrIdx].TsHsyu ) &&
			!m_Arith.l_test( TSrec.TsDtl[zrIdx].TsKsyu ) && 
			!m_Arith.l_test( TSrec.TsDtl[zrIdx].TsFsyu ) &&
			!m_Arith.l_test( TSrec.TsDtl[zrIdx].TsGsyu ) &&
			!m_Arith.l_test( TSrec.TsDtl[zrIdx].TsNosyu ) ){
			continue;
		}

		// 資産の譲渡等の対価以外の収入（特定収入＋不特定収入＋外特定収入）
		m_Arith.l_add( TSrec.TsJsyu, TSrec.TsJsyu, TSrec.TsDtl[zrIdx].TsGsyu );
		m_Arith.l_add( TSrec.TsJsyu, TSrec.TsJsyu, TSrec.TsDtl[zrIdx].TsNosyu );

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

	}

	m_Arith.l_neg( stval.SpTsyu );
	m_Arith.l_neg( stval.SpTsyu4 );
	m_Arith.l_neg( stval.SpTsyu63 );
	m_Arith.l_neg( stval.SpTsyu624 );
	m_Arith.l_neg( stval.SpTsyu78 );
	m_Arith.l_neg( stval.SpHsyu );
	m_Arith.l_neg( stval.SpHsyu4 );
	m_Arith.l_neg( stval.SpHsyu63 );
	m_Arith.l_neg( stval.SpHsyu624 );
	m_Arith.l_neg( stval.SpHsyu78 );
	m_Arith.l_neg( stval.SpKsyu );
	m_Arith.l_neg( stval.SpKsyu4 );
	m_Arith.l_neg( stval.SpKsyu63 );
	m_Arith.l_neg( stval.SpKsyu624 );
	m_Arith.l_neg( stval.SpKsyu78 );
	m_Arith.l_neg( stval.SpFsyu );
	m_Arith.l_neg( stval.SpFsyu4 );
	m_Arith.l_neg( stval.SpFsyu63 );
	m_Arith.l_neg( stval.SpFsyu624 );
	m_Arith.l_neg( stval.SpFsyu78 );
	m_Arith.l_neg( stval.SpSsyu );
	m_Arith.l_neg( stval.SpSsyu4 );
	m_Arith.l_neg( stval.SpSsyu63 );
	m_Arith.l_neg( stval.SpSsyu624 );
	m_Arith.l_neg( stval.SpSsyu78 );


	m_Arith.l_add( stval.SpGsyu, stval.SpGsyu, stval.SpSsyu );
	m_Arith.l_add( stval.SpGsyu, stval.SpGsyu, stval.SpHsyu );
	m_Arith.l_add( stval.SpGsyu, stval.SpGsyu, stval.SpKsyu );
	m_Arith.l_add( stval.SpGsyu, stval.SpGsyu, stval.SpFsyu );

	m_Arith.l_add( stval.SpGsyu4, stval.SpGsyu4, stval.SpSsyu4 );
	m_Arith.l_add( stval.SpGsyu4, stval.SpGsyu4, stval.SpHsyu4 );
	m_Arith.l_add( stval.SpGsyu4, stval.SpGsyu4, stval.SpKsyu4 );
	m_Arith.l_add( stval.SpGsyu4, stval.SpGsyu4, stval.SpFsyu4 );

	m_Arith.l_add( stval.SpGsyu63, stval.SpGsyu63, stval.SpSsyu63 );
	m_Arith.l_add( stval.SpGsyu63, stval.SpGsyu63, stval.SpHsyu63 );
	m_Arith.l_add( stval.SpGsyu63, stval.SpGsyu63, stval.SpKsyu63 );
	m_Arith.l_add( stval.SpGsyu63, stval.SpGsyu63, stval.SpFsyu63 );

	// 8% ( 軽減税率 )
	m_Arith.l_add( stval.SpGsyu624, stval.SpGsyu624, stval.SpSsyu624 );
	m_Arith.l_add( stval.SpGsyu624, stval.SpGsyu624, stval.SpHsyu624 );
	m_Arith.l_add( stval.SpGsyu624, stval.SpGsyu624, stval.SpKsyu624 );
	m_Arith.l_add( stval.SpGsyu624, stval.SpGsyu624, stval.SpFsyu624 );

	// 10%
	m_Arith.l_add( stval.SpGsyu78, stval.SpGsyu78, stval.SpSsyu78 );
	m_Arith.l_add( stval.SpGsyu78, stval.SpGsyu78, stval.SpHsyu78 );
	m_Arith.l_add( stval.SpGsyu78, stval.SpGsyu78, stval.SpKsyu78 );
	m_Arith.l_add( stval.SpGsyu78, stval.SpGsyu78, stval.SpFsyu78 );

	for( int i=0 ; i<TsTbl.tp_rnum; ++i ){
		if( m_TblHdl.th_rget(&TSrec, &TsTbl, i) ){
			return -1;
		}
		TSrec.TsLine = (i + 1);
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

	// 消費税残高の読み込み
	MoneyBasejagArray	money;

	// 課税売上割合計算用金額の取得
	money = pSyzSyukei->GetShinkokuData( _T("91") );
	int max = (int)money.GetCount();
	for( int i=0; i<max; i++ ){
		int inmax = (int)money[i].GetCount();
		for( int j=0; j<inmax; j++ ){
			if( money[i][j].code == _T("910400") ){
				memmove( stval.SpKuws, money[i][j].arith, sizeof(stval.SpKuws) );			// 課税売上高割合（分子）
			}
			else if( money[i][j].code == _T("910700") ){
				memmove( stval.SpKuwb, money[i][j].arith, sizeof(stval.SpKuwb) );			// 〃　　　　　　（分母）
			}
			else if( money[i][j].code == _T("910800") ){
				memmove( stval.SpUwri, money[i][j].arith, sizeof(stval.SpUwri) );			// 課税売上高割合
			}
			else if( money[i][j].code == _T("911202") ){
				memmove( stval.SpUnb4, money[i][j].arith, sizeof(stval.SpUnb4) );			// 〃　　　　4%
			}
			else if( money[i][j].code == _T("911203") ){
				memmove( stval.SpUnb63, money[i][j].arith, sizeof(stval.SpUnb63) );			// 〃　　　　6.3%
			}
			else if( money[i][j].code == _T("911204") ){
				memmove( stval.SpUnb624, money[i][j].arith, sizeof(stval.SpUnb624) );			// 〃　　　　6.24%
			}
			else if( money[i][j].code == _T("911205") ){
				memmove( stval.SpUnb78, money[i][j].arith, sizeof(stval.SpUnb78) );			// 〃　　　　7.8%
			}
			else if( money[i][j].code == _T("911200") ){
				// 売上返還 合計
				memmove( stval.SpUnbg, money[i][j].arith, sizeof(stval.SpUnbg) );			// 〃　　　　合計
			}
			else if( money[i][j].code == _T("911100") ){
				// 課税売上
				memmove( stval.SpKzur, money[i][j].arith, sizeof(stval.SpKzur) );			// 課税売上（純売上）
			}
			else if( money[i][j].code == _T("911102") ){
				// 課税売上 4%
				memmove( stval.SpKzur4, money[i][j].arith, sizeof(stval.SpKzur4) );			// 課税売上（純売上）
			}
			else if( money[i][j].code == _T("911103") ){
				// 課税売上 6.3%
				memmove( stval.SpKzur63, money[i][j].arith, sizeof(stval.SpKzur63) );		// 課税売上（純売上）
			}
			else if( money[i][j].code == _T("911104") ){
				// 課税売上 6.24%
				memmove( stval.SpKzur624, money[i][j].arith, sizeof(stval.SpKzur624) );		// 課税売上（純売上）
			}
			else if( money[i][j].code == _T("911105") ){
				// 課税売上 7.8%
				memmove( stval.SpKzur78, money[i][j].arith, sizeof(stval.SpKzur78) );		// 課税売上（純売上）
			}
			else if( money[i][j].code == _T("910200") ){
				memmove( stval.SpMzur, money[i][j].arith, sizeof(stval.SpMzur) );			// 免税売上
			}
			else if( money[i][j].code == _T("910600") ){
				memmove( stval.SpHkur, money[i][j].arith, sizeof(stval.SpHkur) );			// 非課税売上（有価証券を全額で含む）
			}
			else if( money[i][j].code == _T("911600") ){
				// 有価証券譲渡収入の5%
			}
		}
	}

	memmove( stval.SpKgjt, sv_SpKgjt, sizeof(stval.SpKgjt) ); //国外における資産の譲渡等の対価の額


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


	// 計算表１用の金額
	m_Arith.l_clear( stval.SpTuwb );
	m_Arith.l_add( stval.SpTuwb, stval.SpTuwb, stval.SpKzur );
	m_Arith.l_add( stval.SpTuwb, stval.SpTuwb, stval.SpMzur );
	m_Arith.l_add( stval.SpTuwb, stval.SpTuwb, stval.SpHkur );
	m_Arith.l_add( stval.SpTuwb, stval.SpTuwb, stval.SpKgjt );

	// 課税仕入れ等の税額の取得
	char	moji[64] = { 0 };
	char	PW0[MONY_BUF_SIZE] = { 0 };
	BOOL	Over = FALSE;
	BOOL	Kobetsu = FALSE;
	wsprintf ( moji, _T( "%d" ), pSnHeadData->m_UriRatioBorder );
	m_Arith.l_input ( PW0, moji );
	if ( m_Arith.l_cmp ( stval.SpUwri, PW0 ) >= 0 ) {	// 課税売上割合９５％以上
		Over = TRUE;
	}

	Kobetsu = pSnHeadData->IsKobetuSiireAnbun();

	money = pSyzSyukei->GetShinkokuData ( _T( "32" ) );
	max = ( int )money.GetCount();
	for ( int i = 0; i < max; i++ ) {
		int inmax = ( int )money[i].GetCount();
		for ( int j = 0; j < inmax; j++ ) {
			if ( Over ) {	// 課税売上割合９５％以上
				if ( money[i][j].code == _T( "321602" ) ) {
					memmove ( stval.SpKgz4, money[i][j].arith, sizeof ( stval.SpKgz4 ) );
				}
				else if ( money[i][j].code == _T( "321603" ) ) {
					memmove ( stval.SpKgz63, money[i][j].arith, sizeof ( stval.SpKgz63 ) );
				}
				else if ( money[i][j].code == _T( "324102" ) ) {
					memmove ( stval.SpKgz624, money[i][j].arith, sizeof ( stval.SpKgz624 ) );
				}
				else if ( money[i][j].code == _T( "324103" ) ) {
					memmove ( stval.SpKgz78, money[i][j].arith, sizeof ( stval.SpKgz78 ) );
				}
			}
			else {					// 課税売上割合９５％以下
				if ( Kobetsu ) {	// 個別対応
					if ( money[i][j].code == _T( "321902" ) ) {
						memmove ( stval.SpKgz4, money[i][j].arith, sizeof ( stval.SpKgz4 ) );
					}
					else if ( money[i][j].code == _T( "321903" ) ) {
						memmove ( stval.SpKgz63, money[i][j].arith, sizeof ( stval.SpKgz63 ) );
					}
					else if ( money[i][j].code == _T( "324402" ) ) {
						memmove ( stval.SpKgz624, money[i][j].arith, sizeof ( stval.SpKgz624 ) );
					}
					else if ( money[i][j].code == _T( "324403" ) ) {
						memmove ( stval.SpKgz78, money[i][j].arith, sizeof ( stval.SpKgz78 ) );
					}
				}
				else {				// 比例配分
					if ( money[i][j].code == _T( "322002" ) ) {
						memmove ( stval.SpKgz4, money[i][j].arith, sizeof ( stval.SpKgz4 ) );
					}
					else if ( money[i][j].code == _T( "322003" ) ) {
						memmove ( stval.SpKgz63, money[i][j].arith, sizeof ( stval.SpKgz63 ) );
					}
					else if ( money[i][j].code == _T( "324502" ) ) {
						memmove ( stval.SpKgz624, money[i][j].arith, sizeof ( stval.SpKgz624 ) );
					}
					else if ( money[i][j].code == _T( "324503" ) ) {
						memmove ( stval.SpKgz78, money[i][j].arith, sizeof ( stval.SpKgz78 ) );
					}
				}
			}
		}
	}

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

		if( money[i][0].attr.shisan_syukei_type == 1 ){
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
				else if( money[i][0].attr.shisan_zino == 4 ){
					memmove( stval.SpKsi624, money[i][0].arith, sizeof(stval.SpKsi624) );		// 6.24%：税込み仕入
				}
				else if( money[i][0].attr.shisan_zino == 5 ){
					memmove( stval.SpKsi78, money[i][0].arith, sizeof(stval.SpKsi78) );			// 7.8%：税込み仕入
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
				else if( money[i][0].attr.shisan_zino == 4 ){
					memmove( stval.SpKsb624, money[i][0].arith, sizeof(stval.SpKsb624) );		// 6.24%：税込み仕入値引
				}
				else if( money[i][0].attr.shisan_zino == 5 ){
					memmove( stval.SpKsb78, money[i][0].arith, sizeof(stval.SpKsb78) );			// 7.8%：税込み仕入値引
				}
			}
		}
	}

	// 通常の控除対象仕入税額の合計
	m_Arith.l_add( stval.SpKgzg, stval.SpKgz3, stval.SpKgz4 );
	m_Arith.l_add( stval.SpKgzg, stval.SpKgzg, stval.SpKgz63 );
	m_Arith.l_add( stval.SpKgzg, stval.SpKgzg, stval.SpKgz624 );
	m_Arith.l_add( stval.SpKgzg, stval.SpKgzg, stval.SpKgz78 );

	return 0;
}

//-----------------------------------------------------------------------------
// 特定収入にかかる調整税額計算（サブ）
//-----------------------------------------------------------------------------
// 引数	pSnHeadData	：	ヘッダ情報
//-----------------------------------------------------------------------------
void CH31SpcListData::SPassOffCalqMain( CSnHeadData *pSnHeadData )
{
	ASSERT( pSnHeadData );
	if( pSnHeadData == NULL ){
		return;
	}

// 24/04/26特定収入追加 add -->
	m_bIs52Data=FALSE;
// 24/04/26特定収入追加 add <--

	// 初期化
	if( pSnHeadData->IsKobetuSiireAnbun() == FALSE ){
		memset( stval.SpHsyu, '\0', sizeof(stval.SpHsyu) );
		memset( stval.SpHsyu4, '\0', sizeof(stval.SpHsyu4) );
		memset( stval.SpHsyu63, '\0', sizeof(stval.SpHsyu63) );
		memset( stval.SpHsyu624, '\0', sizeof(stval.SpHsyu624) );
		memset( stval.SpHsyu78, '\0', sizeof(stval.SpHsyu78) );
	}

	// 課税売上割合を変更するがオフの場合は、手入力サインを外す
	if ( !( pSnHeadData->Sn_UPERS & 0x10 ) ) {
		stval.SpUwSgn52[0] &= ~0x0f;
		stval.SpUwSgn53[0] &= ~0x0f;
	}

	// 旧税率が存在しない時は、旧税率の手入力を外す
	if ( pSnHeadData->SVmzsw == 0 ) {
		stval.SpUwSgn52[0] &= ~0x03;
		stval.SpUwSgn53[0] &= ~0x03;
	}

	// 特定収入割合
	// 分母
	memset( stval.SpTswb, '\0', sizeof(stval.SpTswb) );
	m_Arith.l_add( stval.SpTswb, stval.SpTuwb, stval.SpGsyu );

	// 特定収入割合
	//---->'20.07.27
	char SUB[6] = { 0 };
	m_Arith.l_input(SUB, _T("1000"));
	
	//切り上げ用の新たなモジュール関数作成
	m_Util.l_pardotnet_syz2(stval.SpTswr, SUB , stval.SpGsyu , stval.SpTswb , 1);
	//-------
	//元処理
//	m_Arith.l_par100( (unsigned char*)stval.SpTswr, (unsigned char*)stval.SpGsyu, (unsigned char*)stval.SpTswb, 1, 9 );
	//<------

/*	char	buf[128] = { 0 };
	m_Arith.l_print(buf, stval.SpGsyu, "SSS,SSS,SSS,SS9");
	//::AfxMessageBox(buf);

	char	buf2[128] = { 0 };
	m_Arith.l_print(buf, stval.SpTswb, "SSS,SSS,SSS,SS9");
	//::AfxMessageBox(buf);

	char	buf3[128] = { 0 };
	m_Arith.l_print(buf, stval.SpTswr, "SSS,SSS,SSS,SS9");
	//::AfxMessageBox(buf);
*/
	// 調整割合
	m_Arith.l_add( stval.SpTywb, stval.SpTuwb, stval.SpFsyu );
	m_Arith.l_par100( (unsigned char *)stval.SpTwar, (unsigned char *)stval.SpFsyu,	(unsigned char *)stval.SpTywb, 2, 0 );


// 24/02/19wd_特定収入対応 add -->
	int		invsw=0;
	long	eymd=0;
	if(!(pSnHeadData->Sn_SKKBN % 2))	eymd = pSnHeadData->Sn_MDAYE;
	else								eymd = pSnHeadData->Sn_KDAYE;
	if(eymd >= ICS_SH_INVOICE_DAY)	{	// ICS_SH_INVOICE_DAY=20231001
		invsw = 1;
	}
// 24/02/19wd_特定収入対応 add <--

	if( pSnHeadData->SVmzsw == 1 ){
// 24/02/27wd_特定収入対応 add -->
		if(invsw == 1)	{
			SPassOffCalqSub2(pSnHeadData, ID_ICSSH_SPC_8PER);
			SPassOffCalqSub2(pSnHeadData, ID_ICSSH_SPC_5PER);
			SPassOffCalqSub2(pSnHeadData, ID_ICSSH_SPC_8PERK);
			SPassOffCalqSub2(pSnHeadData, ID_ICSSH_SPC_10PER);
		}
// 24/02/27wd_特定収入対応 add <--

		SPassOffCalqSub( pSnHeadData, ID_ICSSH_SPC_8PER );
		SPassOffCalqSub( pSnHeadData, ID_ICSSH_SPC_5PER );
		SPassOffCalqSub( pSnHeadData, ID_ICSSH_SPC_8PERK );
		SPassOffCalqSub( pSnHeadData, ID_ICSSH_SPC_10PER );
	}
	else{
// 24/02/27wd_特定収入対応 add -->
		if(invsw == 1)	{
			SPassOffCalqSub2(pSnHeadData, ID_ICSSH_SPC_8PERK);
			SPassOffCalqSub2(pSnHeadData, ID_ICSSH_SPC_10PER);
		}
// 24/02/27wd_特定収入対応 add <--

		SPassOffCalqSub( pSnHeadData, ID_ICSSH_SPC_8PERK );
		SPassOffCalqSub( pSnHeadData, ID_ICSSH_SPC_10PER );
	}

	// 合計
	char	WORK0[MONY_BUF_SIZE]={0}, WORK1[MONY_BUF_SIZE]={0}, WORK2[MONY_BUF_SIZE]={0}, PW0[MONY_BUF_SIZE]={0}, PW6[MONY_BUF_SIZE]={0}; 

	// 計算表1は連動処理でも手入力可能なため、非連動の判定を外す
	// 控除対象仕入税額への転記
	//if( pSnHeadData->Sn_Sign4 & 0x80 ){
		pSnHeadData->Sn_Sign2 &= 0xfd;
		memset( WORK0, '\0', sizeof(WORK0) );
		m_Arith.l_input( WORK0, _T("50") );
		pSnHeadData->Sn_Sign2 &= 0xfd;
		if( m_Arith.l_cmp( stval.SpTswr, WORK0 ) <= 0 ){
			pSnHeadData->Sn_Sign2 |= 0x02;
		}
	//}
}

//-----------------------------------------------------------------------------
// 特定収入にかかる調整税額計算（サブ）
//-----------------------------------------------------------------------------
// 引数	pSnHeadData	：	ヘッダ情報
//		zeiType		：	税率
//-----------------------------------------------------------------------------
void CH31SpcListData::SPassOffCalqSub( CSnHeadData *pSnHeadData, int zeiType )
{
	char	WORK0[MONY_BUF_SIZE]={0}, WORK1[MONY_BUF_SIZE]={0}, WORK2[MONY_BUF_SIZE]={0}, PW0[MONY_BUF_SIZE]={0}, PW6[MONY_BUF_SIZE]={0}; 
	char	moji[64]={0};

	// 特定収入に係る税額
	memset( PW0, '\0', sizeof(PW0) );
	wsprintf( moji, _T("%d"), pSnHeadData->m_UriRatioBorder );
	m_Arith.l_input( PW0, moji );
	//if( m_Arith.l_cmp( stval.SpUwri, PW0 ) >= 0 ){	// 課税売上割合９５％以上
	// 課税売上割合９５％以上 
	// または、課税売上割合に準ずる割合を使用していない
	if ( ( m_Arith.l_cmp( stval.SpUwri, PW0 ) >= 0 ) && ( pSnHeadData->IsUseSemiUriRatio() == FALSE ) ) {
		memset( WORK0, '\0', sizeof(WORK0) );
		if( zeiType == ID_ICSSH_SPC_8PER ){
			// 特定収入税額 合計
			m_Util.percent( stval.SpGszg63, stval.SpTsyu63, 9, 63, 0 );
		}
		else if( zeiType == ID_ICSSH_SPC_5PER ){
			// 特定収入税額 合計
			m_Util.percent( stval.SpGszg4, stval.SpTsyu4, 4, 40, 0 );
		}
		else if( zeiType == ID_ICSSH_SPC_8PERK ){
			// 特定収入税額 合計
			m_Util.percent( stval.SpGszg624, stval.SpTsyu624, 11, 62, 0 );
		}
		else if( zeiType == ID_ICSSH_SPC_10PER ){
			// 特定収入税額 合計
			m_Util.percent( stval.SpGszg78, stval.SpTsyu78, 12, 78, 0 );
		}
	}
	else{									// 課税売上割合９５％以下
		char	PER100BUF[MONY_BUF_SIZE] = {0};
		m_Arith.l_input( PER100BUF, _T("10000000") );

		char per[6] = { 0 };
		m_Arith.l_input ( per, _T( "10000" ) );

		if( pSnHeadData->IsKobetuSiireAnbun() ){
			if( zeiType == ID_ICSSH_SPC_8PER ){ 
				// 控除対象仕入税額 （個別対応方式）
				// 特定収入税額
				m_Util.percent( stval.SpTszg63, stval.SpSsyu63, 9, 63, 0 );		// 63/1080
				// 共通に係る特定収入税額Ｘ課税売上割合
				memset( WORK0, '\0', sizeof(WORK0) );
				memset( WORK1, '\0', sizeof(WORK1) );
				m_Util.percent( WORK0, stval.SpKsyu63, 9, 63, 0 );				// 63/1080

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

				if ( pSnHeadData->Sn_UPERS & 0x10 ) {
					if ( !( stval.SpUwSgn52[0] & 0x02 ) ) {
						m_Util.l_pardotnet_syz( stval.SpKszg63, WORK0, stval.SpUwri, per );
					}
				}
				else {
					if( m_Arith.l_cmp(WORK1, PER100BUF) < 0 ){
						m_Util.l_pardotnet_syz( stval.SpKszg63, WORK0, stval.SpKuws, stval.SpKuwb );
					}
					else{
						m_Util.l_pardotnet_syz( stval.SpKszg63, WORK0, stval.SpKuws, stval.SpKuws );
					}
				}
				// 特定収入税額 合計
				m_Arith.l_add( stval.SpGszg63, stval.SpTszg63, stval.SpKszg63 );
			}
			else if( zeiType == ID_ICSSH_SPC_5PER ){
				// 控除対象仕入税額 （個別対応方式）
				// 特定収入税額
				m_Util.percent( stval.SpTszg4, stval.SpSsyu4, 4, 40, 0 );		// 40/1050
				// 共通に係る特定収入税額Ｘ課税売上割合
				memset( WORK0, '\0', sizeof(WORK0) );
				memset( WORK1, '\0', sizeof(WORK1) );
				m_Util.percent( WORK0, stval.SpKsyu4, 4, 40, 0 );				// 40/1050

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

				if ( pSnHeadData->Sn_UPERS & 0x10 ) {
					if ( !( stval.SpUwSgn52[0] & 0x01 ) ) {
						m_Util.l_pardotnet_syz( stval.SpKszg4, WORK0, stval.SpUwri, per );
					}
				}
				else {
					if( m_Arith.l_cmp(WORK1, PER100BUF) < 0 ){
						m_Util.l_pardotnet_syz( stval.SpKszg4, WORK0, stval.SpKuws, stval.SpKuwb );
					}
					else{
						m_Util.l_pardotnet_syz( stval.SpKszg4, WORK0, stval.SpKuws, stval.SpKuws );
					}
				}
				// 特定収入税額 合計
				m_Arith.l_add( stval.SpGszg4, stval.SpTszg4, stval.SpKszg4 );
			}
			else if( zeiType == ID_ICSSH_SPC_8PERK ){	// 8% ( 軽減税率 )
				// 控除対象仕入税額 （個別対応方式）
				// 特定収入税額
				m_Util.percent( stval.SpTszg624, stval.SpSsyu624, 11, 62, 0 );	// 624/1080
																				// 共通に係る特定収入税額Ｘ課税売上割合
				memset( WORK0, '\0', sizeof(WORK0) );
				memset( WORK1, '\0', sizeof(WORK1) );
				m_Util.percent( WORK0, stval.SpKsyu624, 11, 62, 0 );			// 624/1080

				memmove( stval.SpZszg624, WORK0, sizeof(stval.SpZszg624) );

				m_Arith.l_par100( (unsigned char *)WORK1, (unsigned char *)stval.SpKuws, (unsigned char *)stval.SpKuwb, 5, 0 );
				// 売上割合がマイナスの場合は０％で計算する。[03'05.17]
				memset( PW6, '\0', sizeof(PW6) );
				if( m_Arith.l_cmp( stval.SpKuws, PW6 ) < 0 ){
					memset( WORK1, '\0', sizeof(WORK1) );
				}
				if( m_Arith.l_cmp( stval.SpKuwb, PW6 ) < 0 ){
					memset( WORK1, '\0', sizeof(WORK1) );
				}

				if ( pSnHeadData->Sn_UPERS & 0x10 ) {
					if ( !( stval.SpUwSgn52[0] & 0x04 ) ) {
						m_Util.l_pardotnet_syz( stval.SpKszg624, WORK0, stval.SpUwri, per );
					}
				}
				else {
					if( m_Arith.l_cmp(WORK1, PER100BUF) < 0 ){
						m_Util.l_pardotnet_syz( stval.SpKszg624, WORK0, stval.SpKuws, stval.SpKuwb );
					}
					else{
						m_Util.l_pardotnet_syz( stval.SpKszg624, WORK0, stval.SpKuws, stval.SpKuws );
					}
				}
				// 特定収入税額 合計
				m_Arith.l_add( stval.SpGszg624, stval.SpTszg624, stval.SpKszg624 );
			}
			else if( zeiType == ID_ICSSH_SPC_10PER ){	// 10%
				// 控除対象仕入税額 （個別対応方式）
				// 特定収入税額
				m_Util.percent( stval.SpTszg78, stval.SpSsyu78, 12, 78, 0 );	// 780/1100
																				// 共通に係る特定収入税額Ｘ課税売上割合
				memset( WORK0, '\0', sizeof(WORK0) );
				memset( WORK1, '\0', sizeof(WORK1) );
				m_Util.percent( WORK0, stval.SpKsyu78, 12, 78, 0 );			// 780/1100
				
				memmove( stval.SpZszg78, WORK0, sizeof(stval.SpZszg78) );

				m_Arith.l_par100( (unsigned char *)WORK1, (unsigned char *)stval.SpKuws, (unsigned char *)stval.SpKuwb, 5, 0 );
				// 売上割合がマイナスの場合は０％で計算する。[03'05.17]
				memset( PW6, '\0', sizeof(PW6) );
				if( m_Arith.l_cmp( stval.SpKuws, PW6 ) < 0 ){
					memset( WORK1, '\0', sizeof(WORK1) );
				}
				if( m_Arith.l_cmp( stval.SpKuwb, PW6 ) < 0 ){
					memset( WORK1, '\0', sizeof(WORK1) );
				}

				if ( pSnHeadData->Sn_UPERS & 0x10 ) {
					if ( !( stval.SpUwSgn52[0] & 0x08 ) ) {
						m_Util.l_pardotnet_syz( stval.SpKszg78, WORK0, stval.SpUwri, per );
					}
				}
				else {
					if( m_Arith.l_cmp(WORK1, PER100BUF) < 0 ){
						m_Util.l_pardotnet_syz( stval.SpKszg78, WORK0, stval.SpKuws, stval.SpKuwb );
					}
					else{
						m_Util.l_pardotnet_syz( stval.SpKszg78, WORK0, stval.SpKuws, stval.SpKuws );
					}
				}
				// 特定収入税額 合計
				m_Arith.l_add( stval.SpGszg78, stval.SpTszg78, stval.SpKszg78 );
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

				if ( pSnHeadData->Sn_UPERS & 0x10 ) {
					if ( !( stval.SpUwSgn53[0] & 0x02 ) ) {
						m_Util.l_pardotnet_syz( stval.SpTszg63, WORK0, stval.SpUwri, per );
					}
				}
				else {
					if( m_Arith.l_cmp(WORK1, PER100BUF) < 0 ){
						m_Util.l_pardotnet_syz( stval.SpTszg63, WORK0, stval.SpKuws, stval.SpKuwb );
					}
					else{
						m_Util.l_pardotnet_syz( stval.SpTszg63, WORK0, stval.SpKuws, stval.SpKuws );
					}
				}
				
				// 特定収入税額 合計
				m_Arith.l_add( stval.SpGszg63, stval.SpTszg63, stval.SpKszg63 );
			}
			else if( zeiType == ID_ICSSH_SPC_5PER ){
				// 特定収入税額＝（特定収入＋共特定収入）ｘ 40/1050
				memset( stval.SpKszg4, '\0', sizeof(stval.SpKszg4) );
				memset( WORK0, '\0', sizeof(WORK0) );
				memset( WORK1, '\0', sizeof(WORK1) );
				m_Util.percent( WORK0, stval.SpTsyu4, 4, 40, 0 );				// 40/1050

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

				if ( pSnHeadData->Sn_UPERS & 0x10 ) {
					if ( !( stval.SpUwSgn53[0] & 0x01 ) ) {
						m_Util.l_pardotnet_syz( stval.SpTszg4, WORK0, stval.SpUwri, per );
					}
				}
				else {
					if( m_Arith.l_cmp(WORK1, PER100BUF) < 0 ){
						m_Util.l_pardotnet_syz( stval.SpTszg4, WORK0, stval.SpKuws, stval.SpKuwb );
					}
					else{
						m_Util.l_pardotnet_syz( stval.SpTszg4, WORK0, stval.SpKuws, stval.SpKuws );
					}
				}
				
				// 特定収入税額 合計
				m_Arith.l_add( stval.SpGszg4, stval.SpTszg4, stval.SpKszg4 );
			}
			else if( zeiType == ID_ICSSH_SPC_8PERK ){	// 8% ( 軽減税率 )
				// 特定収入税額＝（特定収入＋共特定収入）ｘ 624/1080
				memset( stval.SpKszg624, '\0', sizeof(stval.SpKszg624) );
				memset( WORK0, '\0', sizeof(WORK0) );
				memset( WORK1, '\0', sizeof(WORK1) );
				m_Util.percent( WORK0, stval.SpTsyu624, 11, 62, 0 );				// 624/1080

				memmove( stval.SpZszg624, WORK0, sizeof(stval.SpZszg624) );
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

				if ( pSnHeadData->Sn_UPERS & 0x10 ) {
					if ( !( stval.SpUwSgn53[0] & 0x04 ) ) {
						m_Util.l_pardotnet_syz( stval.SpTszg624, WORK0, stval.SpUwri, per );
					}
				}
				else {
					if( m_Arith.l_cmp(WORK1, PER100BUF) < 0 ){
						m_Util.l_pardotnet_syz( stval.SpTszg624, WORK0, stval.SpKuws, stval.SpKuwb );
					}
					else{
						m_Util.l_pardotnet_syz( stval.SpTszg624, WORK0, stval.SpKuws, stval.SpKuws );
					}
				}

				// 特定収入税額 合計
				m_Arith.l_add( stval.SpGszg624, stval.SpTszg624, stval.SpKszg624 );
			}
			else if( zeiType == ID_ICSSH_SPC_10PER ){	// 10%
				// 特定収入税額＝（特定収入＋共特定収入）ｘ 780/1100
				memset( stval.SpKszg78, '\0', sizeof(stval.SpKszg78) );
				memset( WORK0, '\0', sizeof(WORK0) );
				memset( WORK1, '\0', sizeof(WORK1) );
				m_Util.percent( WORK0, stval.SpTsyu78, 12, 78, 0 );				// 780/1100

				memmove( stval.SpZszg78, WORK0, sizeof(stval.SpZszg78) );
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

				if ( pSnHeadData->Sn_UPERS & 0x10 ) {
					if ( !( stval.SpUwSgn53[0] & 0x08 ) ) {
						m_Util.l_pardotnet_syz( stval.SpTszg78, WORK0, stval.SpUwri, per );
					}
				}
				else {
					if( m_Arith.l_cmp(WORK1, PER100BUF) < 0 ){
						m_Util.l_pardotnet_syz( stval.SpTszg78, WORK0, stval.SpKuws, stval.SpKuwb );
					}
					else{
						m_Util.l_pardotnet_syz( stval.SpTszg78, WORK0, stval.SpKuws, stval.SpKuws );
					}
				}

				// 特定収入税額 合計
				m_Arith.l_add( stval.SpGszg78, stval.SpTszg78, stval.SpKszg78 );
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
	else if( zeiType == ID_ICSSH_SPC_8PERK ){	// 8% ( 軽減税率 )
		m_Arith.l_sub( WORK0, stval.SpKgz624, stval.SpGszg624 );
	}
	else if( zeiType == ID_ICSSH_SPC_10PER ){	// 10%
		m_Arith.l_sub( WORK0, stval.SpKgz78, stval.SpGszg78 );
	}

	// 調整割合額
	m_Arith.l_add( stval.SpTywb, stval.SpTuwb, stval.SpFsyu );
	memset( WORK2, '\0', sizeof(WORK2) );

	//---->'19.12.03 修正依頼155497対応。（小数点以下の計算を課税売上割合と同様に。（計算モジュールを使用する形に変更））
	if (zeiType == ID_ICSSH_SPC_8PER) {
//		m_Arith.l_mul100((unsigned char *)stval.SpTwag63, (unsigned char *)WORK0, (unsigned char *)WORK2, 5, 0);
		m_Util.l_pardotnet_syz(stval.SpTwag63, WORK0, stval.SpFsyu, stval.SpTywb);

	}
	else if (zeiType == ID_ICSSH_SPC_5PER) {
//		m_Arith.l_mul100((unsigned char *)stval.SpTwag4, (unsigned char *)WORK0, (unsigned char *)WORK2, 5, 0);
		m_Util.l_pardotnet_syz(stval.SpTwag4, WORK0, stval.SpFsyu, stval.SpTywb);
	}
	else if (zeiType == ID_ICSSH_SPC_8PERK) {	// 8% ( 軽減税率 )
//		m_Arith.l_mul100((unsigned char *)stval.SpTwag624, (unsigned char *)WORK0, (unsigned char *)WORK2, 5, 0);
		m_Util.l_pardotnet_syz(stval.SpTwag624, WORK0, stval.SpFsyu, stval.SpTywb);
	}
	else if (zeiType == ID_ICSSH_SPC_10PER) {	// 10%
//		m_Arith.l_mul100((unsigned char *)stval.SpTwag78, (unsigned char *)WORK0, (unsigned char *)WORK2, 5, 0);
		m_Util.l_pardotnet_syz(stval.SpTwag78, WORK0, stval.SpFsyu, stval.SpTywb);
	}
	
	
	//------

/*
	m_Arith.l_par100( (unsigned char *)WORK2, (unsigned char *)stval.SpFsyu, (unsigned char *)stval.SpTywb, 5, 9 );//yoshida191119
//	m_Arith.l_par100( (unsigned char *)WORK2, (unsigned char *)stval.SpFsyu, (unsigned char *)stval.SpTywb, 5, 0 );

	

	if( zeiType == ID_ICSSH_SPC_8PER ){
		m_Arith.l_mul100( (unsigned char *)stval.SpTwag63, (unsigned char *)WORK0, (unsigned char *)WORK2, 5, 0 );
	}
	else if( zeiType == ID_ICSSH_SPC_5PER ){
		m_Arith.l_mul100( (unsigned char *)stval.SpTwag4, (unsigned char *)WORK0, (unsigned char *)WORK2, 5, 0 );
	}
	else if( zeiType == ID_ICSSH_SPC_8PERK ){	// 8% ( 軽減税率 )
		m_Arith.l_mul100( (unsigned char *)stval.SpTwag624, (unsigned char *)WORK0, (unsigned char *)WORK2, 5, 0 );
	}
	else if( zeiType == ID_ICSSH_SPC_10PER ){	// 10%
		m_Arith.l_mul100( (unsigned char *)stval.SpTwag78, (unsigned char *)WORK0, (unsigned char *)WORK2, 5, 0 );
	}

*/
//<---------
	// 減額調整額
	if( zeiType == ID_ICSSH_SPC_8PER ){
		m_Arith.l_add( stval.SpGgt63, stval.SpGszg63, stval.SpTwag63 );
	}
	else if( zeiType == ID_ICSSH_SPC_5PER ){
		m_Arith.l_add( stval.SpGgt4, stval.SpGszg4, stval.SpTwag4 );
	}
	else if( zeiType == ID_ICSSH_SPC_8PERK ){	// 8% ( 軽減税率 )
		m_Arith.l_add( stval.SpGgt624, stval.SpGszg624, stval.SpTwag624 );
	}
	else if( zeiType == ID_ICSSH_SPC_10PER ){
		m_Arith.l_add( stval.SpGgt78, stval.SpGszg78, stval.SpTwag78 );
	}

	// 控除対象仕入税額
// 24/02/27wd_特定収入対応 cor -->
	//if( zeiType == ID_ICSSH_SPC_8PER ){
	//	m_Arith.l_sub( stval.SpSiz63, stval.SpKgz63, stval.SpGgt63 );
	//}
	//else if( zeiType == ID_ICSSH_SPC_5PER ){
	//	m_Arith.l_sub( stval.SpSiz4, stval.SpKgz4, stval.SpGgt4 );
	//}
	//else if( zeiType == ID_ICSSH_SPC_8PERK ){	// 8% ( 軽減税率 )
	//	m_Arith.l_sub( stval.SpSiz624, stval.SpKgz624, stval.SpGgt624 );
	//}
	//else if( zeiType == ID_ICSSH_SPC_10PER ){	// 10%
	//	m_Arith.l_sub( stval.SpSiz78, stval.SpKgz78, stval.SpGgt78 );
	//}
// -------------------------------
	int		invsw=-1;
	long	eymd=0;
	long	flg;
	BOOL	syzSign = FALSE;
	char	val[6];

	if(!(pSnHeadData->Sn_SKKBN % 2))	eymd = pSnHeadData->Sn_MDAYE;
	else								eymd = pSnHeadData->Sn_KDAYE;
// 24/04/19特定収入追加 cor -->
	//if((pSnHeadData->SVmzsw == 1) || (eymd < R03YEAR))	{
// ----------------------------
	if(eymd < R03YEAR)	{
// 24/04/19特定収入追加 cor <--
		// 旧税率ありもしくは令和3年12月1日より前
		syzSign = TRUE;
	}
	if(eymd >= ICS_SH_INVOICE_DAY)	{	// ICS_SH_INVOICE_DAY=20231001		
		// 課税売上割合95%以上かつ、課税売上割合に準ずる割合を使用していない
		if((IsRatioOver95(pSnHeadData)) && (pSnHeadData->IsUseSemiUriRatio() == FALSE))	{
			// 計算表５（１）
			if(syzSign == FALSE)	{	// 旧税率なし
				invsw = 0;
			}
		}
		else	{
			// 課税売上割合95%未満
			if(pSnHeadData->IsKobetuSiireAnbun())	{	// 個別対応
				// 計算表５（２）
				if(syzSign == FALSE)	{	// 旧税率なし
					invsw = 1;
				}
			}
			else	{									// 比例配分
				// 計算表５（３）
				if(syzSign == FALSE)	{	// 旧税率なし
					invsw = 2;
				}
			}
		}
	}

	if(invsw < 0)	{	// 従来通り
		if(zeiType == ID_ICSSH_SPC_8PER) {
			m_Arith.l_sub(stval.SpSiz63, stval.SpKgz63, stval.SpGgt63);
		}
		else if(zeiType == ID_ICSSH_SPC_5PER) {
			m_Arith.l_sub(stval.SpSiz4, stval.SpKgz4, stval.SpGgt4);
		}
		else if(zeiType == ID_ICSSH_SPC_8PERK) {	// 8% ( 軽減税率 )
			m_Arith.l_sub(stval.SpSiz624, stval.SpKgz624, stval.SpGgt624);
		}
		else if(zeiType == ID_ICSSH_SPC_10PER) {	// 10%
			m_Arith.l_sub(stval.SpSiz78, stval.SpKgz78, stval.SpGgt78);
		}
	}
	else	{	// 決算期至が2023年10月01日以降
		m_Arith.l_clear(val);

		if(zeiType == ID_ICSSH_SPC_8PER) {
			//m_Arith.l_sub(stval.SpSiz63, stval.SpKgz63, stval.SpGgt63);
			if(invsw == 0)		m_Arith.l_add(val, stval.SpKgz63, stval.Sp51kei63);
			else if(invsw == 1)	m_Arith.l_add(val, stval.SpKgz63, stval.Sp52kei63);
			else				m_Arith.l_add(val, stval.SpKgz63, stval.Sp53kei63);
			m_Arith.l_sub(stval.SpSiz63, val, stval.SpGgt63);
		}
		else if(zeiType == ID_ICSSH_SPC_5PER) {
			//m_Arith.l_sub(stval.SpSiz4, stval.SpKgz4, stval.SpGgt4);
			if(invsw == 0)		m_Arith.l_add(val, stval.SpKgz4, stval.Sp51kei4);
			else if(invsw == 1)	m_Arith.l_add(val, stval.SpKgz4, stval.Sp52kei4);
			else				m_Arith.l_add(val, stval.SpKgz4, stval.Sp53kei4);
			m_Arith.l_sub(stval.SpSiz4, val, stval.SpGgt4);
		}
		else if(zeiType == ID_ICSSH_SPC_8PERK) {	// 8% ( 軽減税率 )
			//m_Arith.l_sub(stval.SpSiz624, stval.SpKgz624, stval.SpGgt624);
			if(invsw == 0)		m_Arith.l_add(val, stval.SpKgz624, stval.Sp51kei624);
			else if(invsw == 1)	m_Arith.l_add(val, stval.SpKgz624, stval.Sp52kei624);
			else				m_Arith.l_add(val, stval.SpKgz624, stval.Sp53kei624);
			m_Arith.l_sub(stval.SpSiz624, val, stval.SpGgt624);
		}
		else if(zeiType == ID_ICSSH_SPC_10PER) {	// 10%
			//m_Arith.l_sub(stval.SpSiz78, stval.SpKgz78, stval.SpGgt78);
			if(invsw == 0)		m_Arith.l_add(val, stval.SpKgz78, stval.Sp51kei78);
			else if(invsw == 1)	m_Arith.l_add(val, stval.SpKgz78, stval.Sp52kei78);
			else				m_Arith.l_add(val, stval.SpKgz78, stval.Sp53kei78);
			m_Arith.l_sub(stval.SpSiz78, val, stval.SpGgt78);
		}
	}

// 24/02/27wd_特定収入対応 cor <--
}

// 24/02/19wd_特定収入対応 add -->
//-------------------------------------------------------------------------------------
// 特定収入にかかる調整税額計算（サブ２／控除対象仕入れに係る調整対象額の合計額の計算）
//-------------------------------------------------------------------------------------
// 引数	pSnHeadData	：	ヘッダ情報
//		zeiType		：	税率
//-------------------------------------------------------------------------------------
void CH31SpcListData::SPassOffCalqSub2(CSnHeadData *pSnHeadData, int zeiType)
{
	int			ii=0;
	char		val[6], 
				val522[2][6], 
				val5231[2][6], val5232[2][6], val5233[2][6],
				val5241[2][6], val5242[2][6], val5243[2][6];
	char		PW0[MONY_BUF_SIZE]={0}; 
	char		moji[64]={0};
	TSREC52		TSrec52;

	for(ii=0; ii<2; ii++)	{
		m_Arith.l_clear(val522[ii]);
		m_Arith.l_clear(val5231[ii]);
		m_Arith.l_clear(val5232[ii]);
		m_Arith.l_clear(val5233[ii]);
		m_Arith.l_clear(val5241[ii]);
		m_Arith.l_clear(val5242[ii]);
		m_Arith.l_clear(val5243[ii]);
	}

	// 8%、5%はないため、そのままリターン
	if(zeiType == ID_ICSSH_SPC_8PER || zeiType == ID_ICSSH_SPC_5PER)	{	// 8% or 5%
		return;
	}

	// 合計を計算
	for(ii=0; ii<TsTbl52.tp_rnum; ii++)	{
		if(m_TblHdl.th_rget(&TSrec52, &TsTbl52, ii))	{
			break;
		}

// 240305_160344 add -->
		if(TSrec52.ts521.sel == 0x01)	{
// 240305_160344 add <--
			// 計算表５(２)
			if(zeiType == ID_ICSSH_SPC_8PERK)	{		// 8%(軽減税率)
				m_Arith.l_add(val522[0], val522[0], TSrec52.ts522.val201);
			}
			else if(zeiType == ID_ICSSH_SPC_10PER)	{	// 10%
				m_Arith.l_add(val522[1], val522[1], TSrec52.ts522.val202);
			}
// 240305_160344 add -->
		}
		else if(TSrec52.ts521.sel == 0x02)	{
// 240305_160344 add <--
			// 計算表５(３)－１
			if(zeiType == ID_ICSSH_SPC_8PERK)	{		// 8%(軽減税率)
				m_Arith.l_add(val5231[0], val5231[0], TSrec52.ts5231.val151);
			}
			else if(zeiType == ID_ICSSH_SPC_10PER)	{	// 10%
				m_Arith.l_add(val5231[1], val5231[1], TSrec52.ts5231.val152);
			}
			// 計算表５(３)－２
			if(zeiType == ID_ICSSH_SPC_8PERK)	{		// 8%(軽減税率)
				m_Arith.l_add(val5232[0], val5232[0], TSrec52.ts5232.val151);
			}
			else if(zeiType == ID_ICSSH_SPC_10PER)	{	// 10%
				m_Arith.l_add(val5232[1], val5232[1], TSrec52.ts5232.val152);
			}
			// 計算表５(３)－３
			if(zeiType == ID_ICSSH_SPC_8PERK)	{		// 8%(軽減税率)
				m_Arith.l_add(val5233[0], val5233[0], TSrec52.ts5233.val151);
			}
			else if(zeiType == ID_ICSSH_SPC_10PER)	{	// 10%
				m_Arith.l_add(val5233[1], val5233[1], TSrec52.ts5233.val152);
			}
// 240305_160344 add -->
		}
		else if(TSrec52.ts521.sel == 0x04)	{
// 240305_160344 add <--
			// 計算表５(４)－１
			if(zeiType == ID_ICSSH_SPC_8PERK)	{		// 8%(軽減税率)
				m_Arith.l_add(val5241[0], val5241[0], TSrec52.ts5241.val101);
			}
			else if(zeiType == ID_ICSSH_SPC_10PER)	{	// 10%
				m_Arith.l_add(val5241[1], val5241[1], TSrec52.ts5241.val102);
			}
			// 計算表５(４)－２
			if(zeiType == ID_ICSSH_SPC_8PERK)	{		// 8%(軽減税率)
				m_Arith.l_add(val5242[0], val5242[0], TSrec52.ts5242.val101);
			}
			else if(zeiType == ID_ICSSH_SPC_10PER)	{	// 10%
				m_Arith.l_add(val5242[1], val5242[1], TSrec52.ts5242.val102);
			}
			// 計算表５(４)－３
			if(zeiType == ID_ICSSH_SPC_8PERK)	{		// 8%(軽減税率)
				m_Arith.l_add(val5243[0], val5243[0], TSrec52.ts5243.val101);
			}
			else if(zeiType == ID_ICSSH_SPC_10PER)	{	// 10%
				m_Arith.l_add(val5243[1], val5243[1], TSrec52.ts5243.val102);
			}
// 240305_160344 add -->
		}
// 240305_160344 add <--
	}

	// 特定収入に係る税額
	memset(PW0, '\0', sizeof(PW0)); 
	wsprintf(moji, _T("%d"), pSnHeadData->m_UriRatioBorder);
	m_Arith.l_input(PW0, moji);

	m_Arith.l_clear(val);
	// 課税売上割合９５％以上 または、課税売上割合に準ずる割合を使用していない
	if((m_Arith.l_cmp(stval.SpUwri, PW0) >= 0) && (pSnHeadData->IsUseSemiUriRatio() == FALSE)) {
		// 計算表５(１) 控除対象外仕入れに係る調整額の合計額
		// （計算表5-2(2)[20]、計算表5-2(3)-1[15]、計算表5-2(4)-1[10]）（複数枚作成している場合は、全ての合計額）
		if(zeiType == ID_ICSSH_SPC_8PERK) {		// 8%(軽減税率)
			m_Arith.l_add(val, val, val522[0]);
			m_Arith.l_add(val, val, val5231[0]);
			m_Arith.l_add(val, val, val5241[0]);

			memmove(stval.Sp51kei624, val, sizeof(stval.Sp51kei624));
// 24/04/26特定収入追加 add -->
			if(m_Arith.l_test(stval.Sp51kei624))	m_bIs52Data=TRUE;
// 24/04/26特定収入追加 add <--
		}
		else if(zeiType == ID_ICSSH_SPC_10PER) {	// 10%
			m_Arith.l_add(val, val, val522[1]);
			m_Arith.l_add(val, val, val5231[1]);
			m_Arith.l_add(val, val, val5241[1]);

			memmove(stval.Sp51kei78, val, sizeof(stval.Sp51kei78));
// 24/04/26特定収入追加 add -->
			if(m_Arith.l_test(stval.Sp51kei78))	m_bIs52Data=TRUE;
// 24/04/26特定収入追加 add <--
		}
	}
	// 課税売上割合９５％以下
	else {
		// 控除対象仕入税額 （個別対応方式）
		if(pSnHeadData->IsKobetuSiireAnbun()) {
			// 計算表５(２) 控除対象外仕入れに係る調整額の合計額
			// （計算表5-2(2)[20]、計算表5-2(3)-1[15]、計算表5-2(4)-1[10]）（複数枚作成している場合は、全ての合計額）
			if(zeiType == ID_ICSSH_SPC_8PERK) {		// 8%(軽減税率)
				m_Arith.l_add(val, val, val522[0]);
				m_Arith.l_add(val, val, val5231[0]);
				m_Arith.l_add(val, val, val5241[0]);

				memmove(stval.Sp52kei624, val, sizeof(stval.Sp52kei624));
// 24/04/26特定収入追加 add -->
				if(m_Arith.l_test(stval.Sp52kei624))	m_bIs52Data=TRUE;
// 24/04/26特定収入追加 add <--
			}
			else if(zeiType == ID_ICSSH_SPC_10PER) {	// 10%
				m_Arith.l_add(val, val, val522[1]);
				m_Arith.l_add(val, val, val5231[1]);
				m_Arith.l_add(val, val, val5241[1]);

				memmove(stval.Sp52kei78, val, sizeof(stval.Sp52kei78));
// 24/04/26特定収入追加 add -->
				if(m_Arith.l_test(stval.Sp52kei78))	m_bIs52Data=TRUE;
// 24/04/26特定収入追加 add <--
			}
		}
		// 控除対象仕入税額 （一括比例配分方式）
		else	{
			// 計算表５(３) 控除対象外仕入れに係る調整額の合計額
			// （計算表5-2(2)[20]、計算表5-2(3)-1[15]、計算表5-2(4)-2[10]）（複数枚作成している場合は、全ての合計額）
			if(zeiType == ID_ICSSH_SPC_8PERK) {		// 8%(軽減税率)
				m_Arith.l_add(val, val, val522[0]);
				m_Arith.l_add(val, val, val5231[0]);
				m_Arith.l_add(val, val, val5241[0]);

				memmove(stval.Sp53kei624, val, sizeof(stval.Sp53kei624));
// 24/04/26特定収入追加 add -->
				if(m_Arith.l_test(stval.Sp53kei624))	m_bIs52Data=TRUE;
// 24/04/26特定収入追加 add <--
			}
			else if(zeiType == ID_ICSSH_SPC_10PER) {	// 10%
				m_Arith.l_add(val, val, val522[1]);
				m_Arith.l_add(val, val, val5231[1]);
				m_Arith.l_add(val, val, val5241[1]);

				memmove(stval.Sp53kei78, val, sizeof(stval.Sp53kei78));
// 24/04/26特定収入追加 add -->
				if(m_Arith.l_test(stval.Sp53kei78))	m_bIs52Data=TRUE;
// 24/04/26特定収入追加 add <--
			}
		}
	}
}
// 24/02/19wd_特定収入対応 add <--

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
int CH31SpcListData::GetKnrecNameEx( CDBNpSub *pDBNpSub, unsigned char *Code, char *Name, long &Seq )
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

//-----------------------------------------------------------------------------
// DB からデータ読込み
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 返送値	1		：	
//			0		：	
//-----------------------------------------------------------------------------
int CH31SpcListData::GetData( CDBNpSub *pDBNpSub, CSnHeadData *pSnHeadData, int sn_seq )
{
	TSREC	TSrec;
	CString	filter;

	// 特定収入計算表
	// 初期設定
	memset( &Spval, '\0', sizeof( struct _SPCVAL ) );
	memset( &stval, '\0', sizeof( struct _STUVAL ) );
	if( ReInitTsTbl() ){
		return -1;
	}

// 24/02/29_特定収入 add -->
	// 特定収入　計算表５－２のテーブル
	if( TsTbl52.tp_tno != -1 ){
		m_TblHdl.th_close( TsTbl52.tp_tno );
	}
	int	rsiz = (unsigned int)sizeof( TSREC52 );					// レコード長
	int	rcnt = 200;												// 最大登録数
	int	tsiz = (unsigned long)rsiz * (unsigned long)rcnt;
	int	thd=0;

	// テーブル イニシャライズ
	if( (thd=m_TblHdl.th_open(tsiz, rsiz)) < 0 ){
		return -1;
	}
	memset( &TsTbl52, '\0', sizeof(TB_PAR) );	
	TsTbl52.tp_tno  = thd;
	TsTbl52.tp_rmax = rcnt;
	TsTbl52.tp_rnum = 0;
	TsTbl52.tp_rlen = rsiz;
	TsTbl52.tp_koff = 0;
	TsTbl52.tp_klen = 0;			// キーなし
	TsTbl52.tp_dupm = 0;
	TsTbl52.tp_sufm = 0;
	TsTbl52.tp_cp   = 0;
	TsTbl52.tp_err  = 0;
// 24/02/29_特定収入 add <--

	// 前回のマスターを取り消す
	if( !(pSnHeadData->Sn_Sign2&0x01) )	{
		// 取り消し
		if( pDBNpSub->NpTokuteiSyunyuKeisanOpen(filter) == ERR ){
			m_ErrMsg = pDBNpSub->GetZmErrMsg();
			return -1;
		}
		try{
			CString	sql;
// 24/02/19wd_特定収入対応 cor -->
			//sql.Format( _T("delete from dbo.splist where spseq=%d"), sn_seq );
// ---------------------------------------
			sql.Format( _T("delete from dbo.splist where spseq=%d and spitem < 1000"), sn_seq);
// 24/02/19wd_特定収入対応 cor <--
			pDBNpSub->m_database->ExecuteSQL( sql );
		}
		catch( CDBException *pde ){
			m_ErrMsg = pde->m_strError;
			pde->Delete();
			return -1;
		}
		pDBNpSub->NpTokuteiSyunyuKeisanClose();
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
				case 401 :
					if ( m_isTransitionalMeasure ) {			// 課税売上（免税・非課税売上・値引を含まない）
						pVal = stval.SpKzur4;	
					}
					break;
				case 402 :
					if ( m_isTransitionalMeasure ) {			// 課税売上（免税・非課税売上・値引を含まない）
						pVal = stval.SpKzur63;	
					}
					break;
				case 403 :	pVal = stval.SpKzur624;	break;		// 課税売上（免税・非課税売上・値引を含まない）
				case 404 :	pVal = stval.SpKzur78;	break;		// 課税売上（免税・非課税売上・値引を含まない）
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
				case 901 :
					if ( m_isTransitionalMeasure ) {			// 〃		 ６．３％
						pVal = stval.SpUnb63;
					}
					break;
				case 902 :	pVal = stval.SpUnb624;	break;		// 〃		 ６．２４％
				case 903 :	pVal = stval.SpUnb78;	break;		// 〃		 ７．８％

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
				case 1201:
					if ( m_isTransitionalMeasure ) {			// 通常の控除対象仕入税額の６．３％（個別）
						pVal = stval.SpKgz63;
					}
					break;
				case 1202:	pVal = stval.SpKgz624;	break;		// 通常の控除対象仕入税額の６．２４％（個別）
				case 1203:	pVal = stval.SpKgz78;	break;		// 通常の控除対象仕入税額の７．８％（個別）

				case 13:	pVal = stval.SpYuka;	break;		// 非課税有価証券　全額分
				case 14:
					if ( m_isTransitionalMeasure ) {			// 〃　　　　　　　５％分
						pVal = stval.SpYuk5;
					}
					break;

				case 15:	pVal = stval.SpTsyu;	break;		// 特定収入						（比例／個別）
				case 1501:										// 特定収入合計	４％			（比例／個別）
					if( m_isTransitionalMeasure ){
						pVal = stval.SpTsyu4;
					}
					break;
				case 1502:
					if ( m_isTransitionalMeasure ) {			// 特定収入合計	６．３％		（比例／個別）
						pVal = stval.SpTsyu63;
					}
					break;
				case 1503:	pVal = stval.SpTsyu624;	break;		// 特定収入合計	６．２４％		（比例／個別）
				case 1504:	pVal = stval.SpTsyu78;	break;		// 特定収入合計	７．８％		（比例／個別）

				case 16:	pVal = stval.SpKsyu;	break;		// 特定収入（共通に係る）		（個別）
				case 1601:										// 不特定収入	４％			（比例／個別）
					if( m_isTransitionalMeasure ){
						pVal = stval.SpKsyu4;
					}
					break;
				case 1602:
					if ( m_isTransitionalMeasure ) {			// 不特定収入	６．３％		（比例／個別）
						pVal = stval.SpKsyu63;
					}
					break;
				case 1603:	pVal = stval.SpKsyu624;	break;		// 不特定収入	６．２４％		（比例／個別）
				case 1604:	pVal = stval.SpKsyu78;	break;		// 不特定収入	７．８％		（比例／個別）

				case 17:	pVal = stval.SpFsyu;	break;		// 不特定収入					（比例／個別）					
				case 1701:										// 不特定収入	４％			（比例／個別）
					if( m_isTransitionalMeasure ){
						pVal = stval.SpFsyu4;
					}
					break;
				case 1702:
					if ( m_isTransitionalMeasure ) {			// 不特定収入	６．３％		（比例／個別）					
						pVal = stval.SpFsyu63;
					}
					break;
				case 1703:	pVal = stval.SpFsyu624;	break;		// 不特定収入	６．２４％		（比例／個別）					
				case 1704:	pVal = stval.SpFsyu78;	break;		// 不特定収入	７．８％		（比例／個別）					

				case 18:	pVal = stval.SpGsyu;	break;		// 特定収入合計					（比例／個別）
				case 1801:										// 特定収入合計	４％			（比例／個別）
					if( m_isTransitionalMeasure ){
						pVal = stval.SpGsyu4;
					}
					break;
				case 1802:
					if ( m_isTransitionalMeasure ) {			// 特定収入合計	６．３％		（比例／個別）
						pVal = stval.SpGsyu63;
					}
					break;
				case 1803:	pVal = stval.SpGsyu624;	break;		// 特定収入合計	６．２４％		（比例／個別）
				case 1804:	pVal = stval.SpGsyu78;	break;		// 特定収入合計	７．８％		（比例／個別）

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
				case 2101:
					if ( m_isTransitionalMeasure ) {			// 課税仕入れ等の金額の６．３％・税込み		（個別）
						pVal = stval.SpKsi63;
					}
					break;
				case 2102:	pVal = stval.SpKsi624;	break;		// 課税仕入れ等の金額の６．２４％・税込み	（個別）
				case 2103:	pVal = stval.SpKsi78;	break;		// 課税仕入れ等の金額の７．８％・税込み		（個別）

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
				case 2401:
					if ( m_isTransitionalMeasure ) {			// 課税仕入れ等に係る対価の返還等の金額の６．３％・税込み	（個別）
						pVal = stval.SpKsb63;
					}
					break;
				case 2402:	pVal = stval.SpKsb624;	break;		// 課税仕入れ等に係る対価の返還等の金額の６．２４％・税込み	（個別）
				case 2403:	pVal = stval.SpKsb78;	break;		// 課税仕入れ等に係る対価の返還等の金額の７．８％・税込み	（個別）

				// 内部で計算
				case 25:	pVal = stval.SpGszg;	break;		// 特定収入税額合計				（比例／個別）
				case 2501:										// 特定収入税額合計	４％		（比例／個別）
					if ( m_isTransitionalMeasure ) {
						pVal = stval.SpGszg4;
					}
					break;
				case 2502:	
					if ( m_isTransitionalMeasure ) {			// 特定収入税額合計	６．３％	（比例／個別）
						pVal = stval.SpGszg63;
					}
					break;
				case 2503:	pVal = stval.SpGszg624;	break;		// 特定収入税額合計	６．２４％	（比例／個別）
				case 2504:	pVal = stval.SpGszg78;	break;		// 特定収入税額合計	７．８％	（比例／個別）

				case 26:	pVal = stval.SpTszg;	break;		// 特定収入税額					（比例／個別）
				case 2601:										// 特定収入税額	４％			（比例／個別）
					if( m_isTransitionalMeasure ){
						pVal = stval.SpTszg4;
					}
					break;
				case 2602:
					if ( m_isTransitionalMeasure ) {			// 特定収入税額	６．３％		（比例／個別）
						pVal = stval.SpTszg63;
					}
					break;
				case 2603:	pVal = stval.SpTszg624;	break;		// 特定収入税額	６．２４％		（比例／個別）
				case 2604:	pVal = stval.SpTszg78;	break;		// 特定収入税額	７．８	％		（比例／個別）

				case 27:	pVal = stval.SpKszg;	break;		// 共特定収入税額				（個別）
				case 2701:										// 共特定収入税額 ４％			（個別）
					if( m_isTransitionalMeasure ){
						pVal = stval.SpKszg4;
					}
					break;
				case 2702:
					if ( m_isTransitionalMeasure ) {			// 共特定収入税額 ６．３％		（個別）
						pVal = stval.SpKszg63;
					}
					break;
				case 2703:	pVal = stval.SpKszg624;	break;		// 共特定収入税額 ６．２４％	（個別）
				case 2704:	pVal = stval.SpKszg78;	break;		// 共特定収入税額 ７．８％		（個別）

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
				case 2803:
					if ( m_isTransitionalMeasure ) {			// 売上割合計算前の税額６．３％
						pVal = stval.SpZszg63;
					}
					break;
				case 2804:	pVal = stval.SpZszg624;	break;		// 売上割合計算前の税額６．２４％
				case 2805:	pVal = stval.SpZszg78;	break;		// 売上割合計算前の税額７．８％

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
				case 3403:
					if ( m_isTransitionalMeasure ) {			// 調整割合額６．３％
						pVal = stval.SpTwag63;
					}
					break;
				case 3404:	pVal = stval.SpTwag624;	break;		// 調整割合額６．２４％
				case 3405:	pVal = stval.SpTwag78;	break;		// 調整割合額７．８％

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
				case 3701:
					if ( m_isTransitionalMeasure ) {			// 配賦額６．３％
						pVal = stval.SpKsh63;
					}
					break;
				case 3702:	pVal = stval.SpKsh624;	break;		// 配賦額６．２４％
				case 3703:	pVal = stval.SpKsh78;	break;		// 配賦額７．８％

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
				case 4201:
					if ( m_isTransitionalMeasure ) {			// 減額調整額の６．３％			（個別）
						pVal = stval.SpGgt63;
					}
					break;
				case 4202:	pVal = stval.SpGgt624;	break;		// 減額調整額の６．２４％		（個別）
				case 4203:	pVal = stval.SpGgt78;	break;		// 減額調整額の７．８％			（個別）

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
				case 4501:
					if ( m_isTransitionalMeasure ) {			// 特例計算後の控除対象仕入税額６．３％（個別）
						pVal = stval.SpSiz63;
					}
					break;
				case 4502:	pVal = stval.SpSiz624;	break;		// 特例計算後の控除対象仕入税額６．２４％（個別）
				case 4503:	pVal = stval.SpSiz78;	break;		// 特例計算後の控除対象仕入税額７．８％（個別）

				case 46:	pVal = stval.SpHsyu;	break;		// 特定収入（非課税に係る）
				case 4601:										// 特定収入（非課税に係る）４％
					if( m_isTransitionalMeasure ){
						pVal = stval.SpHsyu4;
					}
					break;
				case 4602:
					if ( m_isTransitionalMeasure ) {			// 特定収入（非課税に係る）６．３％
						pVal = stval.SpHsyu63;
					}
					break;
				case 4603:	pVal = stval.SpHsyu624;	break;		// 特定収入（非課税に係る）６．２４％
				case 4604:	pVal = stval.SpHsyu78;	break;		// 特定収入（非課税に係る）７．８％

				case 47:	pVal = stval.SpHszg;	break;		// 特定収入（非課税に係る税額）
				case 4701:										// 特定収入（非課税に係る税額）４％
					if( m_isTransitionalMeasure ){
						pVal = stval.SpHszg4;
					}
					break;
				case 4702:
					if ( m_isTransitionalMeasure ) {			// 特定収入（非課税に係る税額）６．３％
						pVal = stval.SpHszg63;
					}
					break;
				case 4703:	pVal = stval.SpHszg624;	break;		// 特定収入（非課税に係る税額）６．２４％
				case 4704:	pVal = stval.SpHszg78;	break;		// 特定収入（非課税に係る税額）７．８％

				case 48:	pVal = stval.SpSsyu;	break;		// 特定収入	(課税に係る)
				case 4801:										// 特定収入	(課税に係る) ４％
					if( m_isTransitionalMeasure ){
						pVal = stval.SpSsyu4;
					}
					break;
				case 4802:
					if ( m_isTransitionalMeasure ) {			// 特定収入	(課税に係る) ６．３％
						pVal = stval.SpSsyu63;
					}
					break;
				case 4803:	pVal = stval.SpSsyu624;	break;		// 特定収入	(課税に係る) ６．２４％
				case 4804:	pVal = stval.SpSsyu78;	break;		// 特定収入	(課税に係る) ７．８％
				case 49:	pVal = stval.SpKgjt;	break;		//国外における資産の譲渡等の対価の額
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
				return -1;
			}
		}
		// 手入力保存カラムが存在しないため、計算表2の項目名称用のカラムに保存
		else if ( ( pDBNpSub->m_pSplist->m_spitem == 1 ) && ( pDBNpSub->m_pSplist->m_spitln == 2704 ) ) {
			// 計算表5(2)
			if ( !pDBNpSub->m_pSplist->m_spitnm.IsEmpty() ) {
				m_Util.cstring_to_char ( ( unsigned char* )stval.SpUwSgn52, pDBNpSub->m_pSplist->m_spitnm, sizeof ( stval.SpUwSgn52 ) );
			}
		}
		else if ( ( pDBNpSub->m_pSplist->m_spitem == 1 ) && ( pDBNpSub->m_pSplist->m_spitln == 2604 ) ) {
			// 計算表5(3)
			if ( !pDBNpSub->m_pSplist->m_spitnm.IsEmpty() ) {
				m_Util.cstring_to_char ( ( unsigned char* )stval.SpUwSgn53, pDBNpSub->m_pSplist->m_spitnm, sizeof ( stval.SpUwSgn53 ) );
			}
		}
// 24/02/14_特定収入 add -->
		// 計算表５－２
		else if(pDBNpSub->m_pSplist->m_spitem >= 101 && pDBNpSub->m_pSplist->m_spitem <= 108)	{
			// ｄｂからTB_PARにロード
			Ts52_DbToTbl(pDBNpSub->m_pSplist);
		}
// 24/02/14_特定収入 add <--
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

			// == ５％分 ==
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

			// == ８％分 ==
			case 9 :	// 特定収入		比例
				if( m_isTransitionalMeasure ){
					pVal = TSrec.TsDtl[1].TsTsyu;
				}
				break;
			case 10 :	// 不特定収入	比例
				if( m_isTransitionalMeasure ){
					pVal = TSrec.TsDtl[1].TsFsyu;
				}
				break;
			case 11 :	// 特定収入合計	比例
				if( m_isTransitionalMeasure ){
					pVal = TSrec.TsDtl[1].TsGsyu;
				}
				break;
			case 12 :	// 特定収入　　 個別　課税売上に係る
				if( m_isTransitionalMeasure ){
					if( pSnHeadData->IsKobetuSiireAnbun() ){
						pVal = TSrec.TsDtl[1].TsSsyu;
					}
				}
				break;
			case 13 :	// 〃			〃　　非課税に係る
				if( m_isTransitionalMeasure ){
					if( pSnHeadData->IsKobetuSiireAnbun() ){
						pVal = TSrec.TsDtl[1].TsHsyu;
					}
				}
				break;
			case 14 :	// 〃			〃　　共通にかかつ
				if( m_isTransitionalMeasure ){
					if( pSnHeadData->IsKobetuSiireAnbun() ){
						pVal = TSrec.TsDtl[1].TsKsyu;
					}
				}
				break;
			case 15 :	// 外特定収入　 比例
				if( m_isTransitionalMeasure ){
					pVal = TSrec.TsDtl[1].TsNosyu;
				}
				break;

			// == ８％軽減分 ==
			case 16 :	// 特定収入		比例
				pVal = TSrec.TsDtl[2].TsTsyu;
				break;
			case 17 :	// 不特定収入	比例
				pVal = TSrec.TsDtl[2].TsFsyu;
				break;
			case 18 :	// 特定収入合計	比例
				pVal = TSrec.TsDtl[2].TsGsyu;
				break;
			case 19 :	// 特定収入　　 個別　課税売上に係る
				if( pSnHeadData->IsKobetuSiireAnbun() ){
					pVal = TSrec.TsDtl[2].TsSsyu;
				}
				break;
			case 20 :	// 〃			〃　　非課税に係る
				if( pSnHeadData->IsKobetuSiireAnbun() ){
					pVal = TSrec.TsDtl[2].TsHsyu;
				}
				break;
			case 21 :	// 〃			〃　　共通にかかつ
				if( pSnHeadData->IsKobetuSiireAnbun() ){
					pVal = TSrec.TsDtl[2].TsKsyu;
				}
				break;
			case 22 :	// 外特定収入　 比例
				pVal = TSrec.TsDtl[2].TsNosyu;
				break;

			// == １０％分 ==
			case 23 :	// 特定収入		比例
				pVal = TSrec.TsDtl[3].TsTsyu;
				break;
			case 24 :	// 不特定収入	比例
				pVal = TSrec.TsDtl[3].TsFsyu;
				break;
			case 25 :	// 特定収入合計	比例
				pVal = TSrec.TsDtl[3].TsGsyu;
				break;
			case 26 :	// 特定収入　　 個別　課税売上に係る
				if( pSnHeadData->IsKobetuSiireAnbun() ){
					pVal = TSrec.TsDtl[3].TsSsyu;
				}
				break;
			case 27 :	// 〃			〃　　非課税に係る
				if( pSnHeadData->IsKobetuSiireAnbun() ){
					pVal = TSrec.TsDtl[3].TsHsyu;
				}
				break;
			case 28 :	// 〃			〃　　共通にかかつ
				if( pSnHeadData->IsKobetuSiireAnbun() ){
					pVal = TSrec.TsDtl[3].TsKsyu;
				}
				break;
			case 29 :	// 外特定収入　 比例
				pVal = TSrec.TsDtl[3].TsNosyu;
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

// 24/02/14_特定収入 add -->
	// 計算表５－２の計算
	TSREC52		TSrec52;
	//for(int i=0; i<TsTbl52.tp_rnum; i++){
	//	if(m_TblHdl.th_rget(&TSrec52,&TsTbl52,i))	{
	//		break;
	//	}
	//	Ts52_Calc(&TSrec52,0xff);	// 0xff:全計算表が対象
	//}
	if(TsTbl52.tp_rnum == 0)	{
		// 無ければ１レコードだけ作っておく（tp_rnumをダイアログの最大件数に使用しているので）
		memset(&TSrec52,0,sizeof(TSREC52));
		m_TblHdl.th_rput(&TSrec52,&TsTbl52,0);
	}
// 24/02/14_特定収入 add <--

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
int CH31SpcListData::SetData( CDBNpSub *pDBNpSub, CSnHeadData *pSnHeadData, int sn_seq )
{
	int		count = 0;

	TSREC	TSrec;
	char	Nam[20];
	CString	filter;
// 24/03/26_特定収入 add -->
	int		eymd=0;
	int		ver=0;

	if(!(pSnHeadData->Sn_SKKBN % 2))	eymd = pSnHeadData->Sn_MDAYE;
	else								eymd = pSnHeadData->Sn_KDAYE;
	if(eymd >= ICS_SH_INVOICE_DAY)	ver=2;
// 24/03/26_特定収入 add <--

	// 特定収入計算表
	// ダミーフィルタ
	filter.Format( "spseq = %d", -1 );
	if( pDBNpSub->NpTokuteiSyunyuKeisanOpen(filter) == ERR )	{
		m_ErrMsg = pDBNpSub->GetZmErrMsg();
		return( -1 );
	}

	// 既存データクリア
	try{
		CString	sql;
// 24/02/19wd_特定収入対応 cor -->
		//sql.Format( _T("delete from dbo.splist where spseq=%d"), sn_seq );
// ---------------------------------------
		sql.Format( _T("delete from dbo.splist where spseq=%d and spitem < 1000"), sn_seq );
// 24/02/19wd_特定収入対応 cor <--
		pDBNpSub->m_database->ExecuteSQL( sql );
	}
	catch( CDBException *pde ){
		m_ErrMsg = pde->m_strError;
		pde->Delete();
		return -1;
	}
	pDBNpSub->m_pSplist->RequeryEx( filter );

	// 計算表
	for( count = 0; count < STUVALCNT; count++ ) {
		// 新規データ作成　送信
		pDBNpSub->m_pSplist->AddNewEx();
		pDBNpSub->m_pSplist->m_spseq  = sn_seq;
		pDBNpSub->m_pSplist->m_spitem = 1;
		pDBNpSub->m_pSplist->m_spitcl = 1;
		char	*pVal = NULL;
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

			case 10 :// 〃		 ６．２４％
				pVal = stval.SpUnb624;
				pDBNpSub->m_pSplist->m_spitln = 902;
				break;

			case 11 :// 〃		 ７．８％
				pVal = stval.SpUnb78;
				pDBNpSub->m_pSplist->m_spitln = 903;
				break;

			case 12 :// 通常の控除対象仕入税額の合計（比例／個別）
				pVal = stval.SpKgzg;
				pDBNpSub->m_pSplist->m_spitln = 10;
				break;

			case 13 :// 通常の控除対象仕入税額の３％（個別）
				pVal = stval.SpKgz3;
				pDBNpSub->m_pSplist->m_spitln = 11;
				break;

			case 14 :// 通常の控除対象仕入税額の４％（個別）
				pVal = stval.SpKgz4;
				pDBNpSub->m_pSplist->m_spitln = 12;
				break;

			case 15 :// 通常の控除対象仕入税額の６．３％（個別）
				pVal = stval.SpKgz63;
				pDBNpSub->m_pSplist->m_spitln = 1201;
				break;

			case 16 :// 通常の控除対象仕入税額の６．２４％（個別）
				pVal = stval.SpKgz624;
				pDBNpSub->m_pSplist->m_spitln = 1202;
				break;

			case 17 :// 通常の控除対象仕入税額の７．８％（個別）
				pVal = stval.SpKgz78;
				pDBNpSub->m_pSplist->m_spitln = 1203;
				break;

			case 18 :// 非課税有価証券　全額分
				pVal = stval.SpYuka;
				pDBNpSub->m_pSplist->m_spitln = 13;
				break;

			case 19 :// 〃　　　　　　　５％分
				pVal = stval.SpYuk5;
				pDBNpSub->m_pSplist->m_spitln = 14;
				break;

			case 20 :// 特定収入						（比例／個別）
				pVal = stval.SpTsyu;
				pDBNpSub->m_pSplist->m_spitln = 15;
				break;

			case 21 :// 特定収入	４％				（比例／個別）
				pVal = stval.SpTsyu4;
				pDBNpSub->m_pSplist->m_spitln = 1501;
				break;

			case 22 :// 特定収入	６．３％			（比例／個別）
				pVal = stval.SpTsyu63;
				pDBNpSub->m_pSplist->m_spitln = 1502;
				break;

			case 23 :// 特定収入	６．２４％			（比例／個別）
				pVal = stval.SpTsyu624;
				pDBNpSub->m_pSplist->m_spitln = 1503;
				break;

			case 24 :// 特定収入	７．８％			（比例／個別）
				pVal = stval.SpTsyu78;
				pDBNpSub->m_pSplist->m_spitln = 1504;
				break;

			case 25 :// 特定収入（共通に係る）			（個別）
				pVal = stval.SpKsyu;
				pDBNpSub->m_pSplist->m_spitln = 16;
				break;

			case 26 :// 特定収入（共通に係る）４％		（個別）
				pVal = stval.SpKsyu4;
				pDBNpSub->m_pSplist->m_spitln = 1601;
				break;

			case 27 :// 特定収入（共通に係る）６．３％	（個別）
				pVal = stval.SpKsyu63;
				pDBNpSub->m_pSplist->m_spitln = 1602;
				break;

			case 28 :// 特定収入（共通に係る）６．２４％（個別）
				pVal = stval.SpKsyu624;
				pDBNpSub->m_pSplist->m_spitln = 1603;
				break;

			case 29 :// 特定収入（共通に係る）７．８％	（個別）
				pVal = stval.SpKsyu78;
				pDBNpSub->m_pSplist->m_spitln = 1604;
				break;

			case 30 :// 不特定収入					（比例／個別）
				pVal = stval.SpFsyu;
				pDBNpSub->m_pSplist->m_spitln = 17;
				break;

			case 31 :// 不特定収入 ４％				（比例／個別）					
				pVal = stval.SpFsyu4;
				pDBNpSub->m_pSplist->m_spitln = 1701;
				break;

			case 32 :// 不特定収入 ６．３％			（比例／個別）
				pVal = stval.SpFsyu63;
				pDBNpSub->m_pSplist->m_spitln = 1702;
				break;

			case 33 :// 不特定収入 ６．２４％		（比例／個別）
				pVal = stval.SpFsyu624;
				pDBNpSub->m_pSplist->m_spitln = 1703;
				break;

			case 34 :// 不特定収入 ７．８％			（比例／個別）
				pVal = stval.SpFsyu78;
				pDBNpSub->m_pSplist->m_spitln = 1704;
				break;

			case 35 :// 特定収入合計					（比例／個別）
				pVal = stval.SpGsyu;
				pDBNpSub->m_pSplist->m_spitln = 18;
				break;

			case 36 :// 特定収入合計	４％			（比例／個別）
				pVal = stval.SpGsyu4;
				pDBNpSub->m_pSplist->m_spitln = 1801;
				break;

			case 37 :// 特定収入合計	６．３％		（比例／個別）
				pVal = stval.SpGsyu63;
				pDBNpSub->m_pSplist->m_spitln = 1802;
				break;

			case 38 :// 特定収入合計	６．２４％		（比例／個別）
				pVal = stval.SpGsyu624;
				pDBNpSub->m_pSplist->m_spitln = 1803;
				break;

			case 39 :// 特定収入合計	７．８％		（比例／個別）
				pVal = stval.SpGsyu78;
				pDBNpSub->m_pSplist->m_spitln = 1804;
				break;

			case 40 :// 課税仕入れ等の金額の合計・税込み	（個別）
				pVal = stval.SpKsig;
				pDBNpSub->m_pSplist->m_spitln = 19;
				break;

			case 41 :// 課税仕入れ等の金額の３％・税込み	（個別）
				pVal = stval.SpKsi3;
				pDBNpSub->m_pSplist->m_spitln = 20;
				break;

			case 42 :// 課税仕入れ等の金額の４％・税込み	（個別）
				pVal = stval.SpKsi4;
				pDBNpSub->m_pSplist->m_spitln = 21;
				break;

			case 43 :// 課税仕入れ等の金額の６．３％・税込み	（個別）
				pVal = stval.SpKsi63;
				pDBNpSub->m_pSplist->m_spitln = 2101;
				break;

			case 44 :// 課税仕入れ等の金額の６．２４％・税込み	（個別）
				pVal = stval.SpKsi624;
				pDBNpSub->m_pSplist->m_spitln = 2102;
				break;

			case 45 :// 課税仕入れ等の金額の７．８％・税込み	（個別）
				pVal = stval.SpKsi78;
				pDBNpSub->m_pSplist->m_spitln = 2103;
				break;

			case 46 :// 課税仕入れ等に係る対価の返還等の金額の合計・税込み（個別）
				pVal = stval.SpKsbg;
				pDBNpSub->m_pSplist->m_spitln = 22;
				break;

			case 47 :// 課税仕入れ等に係る対価の返還等の金額の３％・税込み（個別）
				pVal = stval.SpKsb3;
				pDBNpSub->m_pSplist->m_spitln = 23;
				break;

			case 48 :// 課税仕入れ等に係る対価の返還等の金額の４％・税込み（個別）
				pVal = stval.SpKsb4;
				pDBNpSub->m_pSplist->m_spitln = 24;
				break;

			case 49 :// 課税仕入れ等に係る対価の返還等の金額の６．３％・税込み（個別）
				pVal = stval.SpKsb63;
				pDBNpSub->m_pSplist->m_spitln = 2401;
				break;

			case 50 :// 課税仕入れ等に係る対価の返還等の金額の６．２４％・税込み（個別）
				pVal = stval.SpKsb624;
				pDBNpSub->m_pSplist->m_spitln = 2402;
				break;

			case 51 :// 課税仕入れ等に係る対価の返還等の金額の７．８％・税込み（個別）
				pVal = stval.SpKsb78;
				pDBNpSub->m_pSplist->m_spitln = 2403;
				break;

			case 52 :// 特定収入税額合計				（比例／個別）
				pVal = stval.SpGszg;
				pDBNpSub->m_pSplist->m_spitln = 25;
				break;

			case 53 :// 特定収入税額合計	４％		（比例／個別）
				pVal = stval.SpGszg4;
				pDBNpSub->m_pSplist->m_spitln = 2501;
				break;

			case 54 :// 特定収入税額合計	６．３％	（比例／個別）
				pVal = stval.SpGszg63;
				pDBNpSub->m_pSplist->m_spitln = 2502;
				break;

			case 55 :// 特定収入税額合計	６．２４％	（比例／個別）
				pVal = stval.SpGszg624;
				pDBNpSub->m_pSplist->m_spitln = 2503;
				break;

			case 56 :// 特定収入税額合計	７．８％	（比例／個別）
				pVal = stval.SpGszg78;
				pDBNpSub->m_pSplist->m_spitln = 2504;
				break;

			case 57 :// 特定収入税額					（比例／個別）
				pVal = stval.SpTszg;
				pDBNpSub->m_pSplist->m_spitln = 26;
				break;

			case 58 :// 特定収入税額	 ４％			（比例／個別）
				pVal = stval.SpTszg4;
				pDBNpSub->m_pSplist->m_spitln = 2601;
				break;

			case 59 :// 特定収入税額	 ６．３％		（比例／個別）
				pVal = stval.SpTszg63;
				pDBNpSub->m_pSplist->m_spitln = 2602;
				break;

			case 60 :// 特定収入税額	 ６．２４％		（比例／個別）
				pVal = stval.SpTszg624;
				pDBNpSub->m_pSplist->m_spitln = 2603;
				break;

			case 61 :// 特定収入税額	 ７．８％		（比例／個別）
				pVal = stval.SpTszg78;
				pDBNpSub->m_pSplist->m_spitln = 2604;
				m_Util.char_to_cstring ( &pDBNpSub->m_pSplist->m_spitnm, ( unsigned char* )stval.SpUwSgn53, sizeof ( stval.SpUwSgn53 ) );
				break;

			case 62 :// 共特定収入税額				（個別）
				pVal = stval.SpKszg;
				pDBNpSub->m_pSplist->m_spitln = 27;
				break;

			case 63 :// 共特定収入税額 ４％			（個別）
				pVal = stval.SpKszg4;
				pDBNpSub->m_pSplist->m_spitln = 2701;
				break;

			case 64 :// 共特定収入税額 ６．３％		（個別）
				pVal = stval.SpKszg63;
				pDBNpSub->m_pSplist->m_spitln = 2702;
				break;

			case 65 :// 共特定収入税額 ６．２４％		（個別）
				pVal = stval.SpKszg624;
				pDBNpSub->m_pSplist->m_spitln = 2703;
				break;

			case 66 :// 共特定収入税額 ７．８％		（個別）
				pVal = stval.SpKszg78;
				pDBNpSub->m_pSplist->m_spitln = 2704;
				m_Util.char_to_cstring ( &pDBNpSub->m_pSplist->m_spitnm, ( unsigned char* )stval.SpUwSgn52, sizeof ( stval.SpUwSgn52 ) );
				break;

			case 67 :// 売上割合計算前の税額
				pVal = stval.SpZszg;
				pDBNpSub->m_pSplist->m_spitln = 28;
				break;

			case 68 :// 売上割合計算前の税額３％
				pVal = stval.SpZszg3;
				pDBNpSub->m_pSplist->m_spitln = 2801;
				break;

			case 69 :// 売上割合計算前の税額４％
				pVal = stval.SpZszg4;
				pDBNpSub->m_pSplist->m_spitln = 2802;
				break;

			case 70 :// 売上割合計算前の税額６．３％
				pVal = stval.SpZszg63;
				pDBNpSub->m_pSplist->m_spitln = 2803;
				break;

			case 71 :// 売上割合計算前の税額６．２４％
				pVal = stval.SpZszg624;
				pDBNpSub->m_pSplist->m_spitln = 2804;
				break;

			case 72 :// 売上割合計算前の税額７．８％
				pVal = stval.SpZszg78;
				pDBNpSub->m_pSplist->m_spitln = 2805;
				break;

			case 73 :// 特定割合及び、調整割合計算用の分母の基礎
				pVal = stval.SpTuwb;
				pDBNpSub->m_pSplist->m_spitln = 29;
				break;

			case 74 :// 特定収入割合の分母（全額）
				pVal = stval.SpTswb;
				pDBNpSub->m_pSplist->m_spitln = 30;
				break;

			case 75 :// 調整割合の分母　　（全額）
				pVal = stval.SpTywb;
				pDBNpSub->m_pSplist->m_spitln = 31;
				break;

			case 76 :// 特定収入割	（少数点代位２位）
				pVal = stval.SpTswr;
				pDBNpSub->m_pSplist->m_spitln = 32;
				break;

			case 77 :// 調整割合		（少数点代位２位）
				pVal = stval.SpTwar;
				pDBNpSub->m_pSplist->m_spitln = 33;
				break;

			case 78 :// 調整割合額
				pVal = stval.SpTwag;
				pDBNpSub->m_pSplist->m_spitln = 34;
				break;

			case 79 :// 調整割合額３％
				pVal = stval.SpTwag3;
				pDBNpSub->m_pSplist->m_spitln = 3401;
				break;

			case 80 :// 調整割合額４％
				pVal = stval.SpTwag4;
				pDBNpSub->m_pSplist->m_spitln = 3402;
				break;

			case 81 :// 調整割合額６．３％
				pVal = stval.SpTwag63;
				pDBNpSub->m_pSplist->m_spitln = 3403;
				break;

			case 82 :// 調整割合額６．２４％
				pVal = stval.SpTwag624;
				pDBNpSub->m_pSplist->m_spitln = 3404;
				break;

			case 83 :// 調整割合額７．８％
				pVal = stval.SpTwag78;
				pDBNpSub->m_pSplist->m_spitln = 3405;
				break;

			case 84 :// 配賦割合		（少数点代位２位）
				pVal = stval.SpKshf;
				pDBNpSub->m_pSplist->m_spitln = 35;
				break;

			case 85 :// 配賦額３％
				pVal = stval.SpKsh3;
				pDBNpSub->m_pSplist->m_spitln = 36;
				break;

			case 86 :// 配賦額４％
				pVal = stval.SpKsh4;
				pDBNpSub->m_pSplist->m_spitln = 37;
				break;

			case 87 :// 配賦額６．３％
				pVal = stval.SpKsh63;
				pDBNpSub->m_pSplist->m_spitln = 3701;
				break;

			case 88 :// 配賦額６．２４％
				pVal = stval.SpKsh624;
				pDBNpSub->m_pSplist->m_spitln = 3702;
				break;

			case 89 :// 配賦額７．８％
				pVal = stval.SpKsh78;
				pDBNpSub->m_pSplist->m_spitln = 3703;
				break;

			case 90 :// 配賦割合の分子（全額）
				pVal = stval.SpHfgs;
				pDBNpSub->m_pSplist->m_spitln = 38;
				break;

			case 91 :// 配賦割合の分母（全額）
				pVal = stval.SpHfgb;
				pDBNpSub->m_pSplist->m_spitln = 39;
				break;

			case 92 :// 減額調整額の合計				（比例／個別）
				pVal = stval.SpGgtg;
				pDBNpSub->m_pSplist->m_spitln = 40;
				break;

			case 93 :// 減額調整額の３％				（個別）
				pVal = stval.SpGgt3;
				pDBNpSub->m_pSplist->m_spitln = 41;
				break;

			case 94 :// 減額調整額の４％				（個別）
				pVal = stval.SpGgt4;
				pDBNpSub->m_pSplist->m_spitln = 42;
				break;

			case 95 :// 減額調整額の６．３％			（個別）
				pVal = stval.SpGgt63;
				pDBNpSub->m_pSplist->m_spitln = 4201;
				break;

			case 96 :// 減額調整額の６．２４％			（個別）
				pVal = stval.SpGgt624;
				pDBNpSub->m_pSplist->m_spitln = 4202;
				break;

			case 97 :// 減額調整額の７．８％			（個別）
				pVal = stval.SpGgt78;
				pDBNpSub->m_pSplist->m_spitln = 4203;
				break;

			case 98 :// 特例計算後の控除対象仕入税額合計（比例／個別）
				pVal = stval.SpSizg;
				pDBNpSub->m_pSplist->m_spitln = 43;
				break;

			case 99 :// 特例計算後の控除対象仕入税額３％（個別）
				pVal = stval.SpSiz3;
				pDBNpSub->m_pSplist->m_spitln = 44;
				break;

			case 100 :// 特例計算後の控除対象仕入税額４％（個別）
				pVal = stval.SpSiz4;
				pDBNpSub->m_pSplist->m_spitln = 45;
				break;

			case 101 :// 特例計算後の控除対象仕入税額６．３％（個別）
				pVal = stval.SpSiz63;
				pDBNpSub->m_pSplist->m_spitln = 4501;
				break;

			case 102 :// 特例計算後の控除対象仕入税額６．２４％（個別）
				pVal = stval.SpSiz624;
				pDBNpSub->m_pSplist->m_spitln = 4502;
				break;

			case 103 :// 特例計算後の控除対象仕入税額７．８％（個別）
				pVal = stval.SpSiz78;
				pDBNpSub->m_pSplist->m_spitln = 4503;
				break;

			//== 合計 ==
			case 104 :// 特定収入（非課税に係る）
				pVal = stval.SpHsyu;
				pDBNpSub->m_pSplist->m_spitln = 46;
				break;

			case 105 :// 特定収入（非課税に係る）４％
				pVal = stval.SpHsyu4;
				pDBNpSub->m_pSplist->m_spitln = 4601;
				break;

			case 106 :// 特定収入（非課税に係る）６．３％
				pVal = stval.SpHsyu63;
				pDBNpSub->m_pSplist->m_spitln = 4602;
				break;

			case 107 :// 特定収入（非課税に係る）６．２４％
				pVal = stval.SpHsyu624;
				pDBNpSub->m_pSplist->m_spitln = 4603;
				break;

			case 108 :// 特定収入（非課税に係る）７．８％
				pVal = stval.SpHsyu78;
				pDBNpSub->m_pSplist->m_spitln = 4604;
				break;

			case 109 :// 特定収入（非課税に係る税額）
				pVal = stval.SpHszg;
				pDBNpSub->m_pSplist->m_spitln = 47;
				break;

			case 110 :// 特定収入（非課税に係る税額）４％
				pVal = stval.SpHszg4;
				pDBNpSub->m_pSplist->m_spitln = 4701;
				break;

			case 111 :// 特定収入（非課税に係る税額）６．３％
				pVal = stval.SpHszg63;
				pDBNpSub->m_pSplist->m_spitln = 4702;
				break;

			case 112 :// 特定収入	(課税に係る)
				pVal = stval.SpSsyu;
				pDBNpSub->m_pSplist->m_spitln = 48;
				break;

			case 113 :// 特定収入	(課税に係る) ４％
				pVal = stval.SpSsyu4;
				pDBNpSub->m_pSplist->m_spitln = 4801;
				break;

			case 114 :// 特定収入	(課税に係る) ６．３％
				pVal = stval.SpSsyu63;
				pDBNpSub->m_pSplist->m_spitln = 4802;
				break;

			case 115 :// 特定収入	(課税に係る) ６．２４％
				pVal = stval.SpSsyu624;
				pDBNpSub->m_pSplist->m_spitln = 4803;
				break;

			case 116 :// 特定収入	(課税に係る) ７．８％
				pVal = stval.SpSsyu78;
				pDBNpSub->m_pSplist->m_spitln = 4804;
				break;

			case 117	://国外における資産の譲渡等の対価の額
				pVal = stval.SpKgjt;
				pDBNpSub->m_pSplist->m_spitln = 49;
				break;

			case 118 :// 課税売上（免税・非課税売上・値引を含まない）
				pVal = stval.SpKzur4;
				pDBNpSub->m_pSplist->m_spitln = 401;
				break;

			case 119 :// 課税売上（免税・非課税売上・値引を含まない）
				pVal = stval.SpKzur63;
				pDBNpSub->m_pSplist->m_spitln = 402;
				break;

			case 120 :// 課税売上（免税・非課税売上・値引を含まない）
				pVal = stval.SpKzur624;
				pDBNpSub->m_pSplist->m_spitln = 403;
				break;

			case 121 :// 課税売上（免税・非課税売上・値引を含まない）
				pVal = stval.SpKzur78;
				pDBNpSub->m_pSplist->m_spitln = 404;
				break;

			default:	break;			 
		}

		if ( pVal != NULL ) {
			m_Util.val_to_asci( &pDBNpSub->m_pSplist->m_spitval, (unsigned char*)pVal );
		}
// 24/03/26_特定収入 add -->
		pDBNpSub->m_pSplist->m_spver = ver;
// 24/03/26_特定収入 add <--
		
		// 更新
		if( pDBNpSub->m_pSplist->UpdateEx() == ERR )	{
			pDBNpSub->NpTokuteiSyunyuKeisanClose();
			return( -1 );
		}
	}


	// 特定収入
	int rno = 0;
	

	while( 1 ) {
		if( rno == TsTbl.tp_rnum )	break;
		if( m_TblHdl.th_rget( &TSrec, &TsTbl, rno ) )	{
			pDBNpSub->NpTokuteiSyunyuKeisanClose();
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
// 24/03/26_特定収入 add -->
		pDBNpSub->m_pSplist->m_spver = ver;
// 24/03/26_特定収入 add <--
		if( pDBNpSub->m_pSplist->UpdateEx() == ERR )	{
			pDBNpSub->NpTokuteiSyunyuKeisanClose();
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
// 24/03/26_特定収入 add -->
		pDBNpSub->m_pSplist->m_spver = ver;
// 24/03/26_特定収入 add <--
		if( pDBNpSub->m_pSplist->UpdateEx() == ERR )	{
			pDBNpSub->NpTokuteiSyunyuKeisanClose();
			return( -1 );
		}
		spitcl_pos++;

		for(int i = 0 ; i < TS_ZR_MAX - 6 ; i++){	// ５％、８％、８％軽減(6.24%)、１０％分を回す
			// 特定収入		比例
			pDBNpSub->m_pSplist->AddNewEx();
			pDBNpSub->m_pSplist->m_spseq  = sn_seq;
			pDBNpSub->m_pSplist->m_spitem = 2;
			pDBNpSub->m_pSplist->m_spitln = rno+1;
			pDBNpSub->m_pSplist->m_spitcl = spitcl_pos;	// 5%→2, 8%→9, 8%軽減→16, 10%→23
			m_Util.val_to_asci( &pDBNpSub->m_pSplist->m_spitval, (unsigned char*)TSrec.TsDtl[i].TsTsyu );
// 24/03/26_特定収入 add -->
			pDBNpSub->m_pSplist->m_spver = ver;
// 24/03/26_特定収入 add <--
			if( pDBNpSub->m_pSplist->UpdateEx() == ERR )	{
				pDBNpSub->NpTokuteiSyunyuKeisanClose();
				return( -1 );
			}
			spitcl_pos++;
		
			// 不特定収入	比例
			pDBNpSub->m_pSplist->AddNewEx();
			pDBNpSub->m_pSplist->m_spseq  = sn_seq;
			pDBNpSub->m_pSplist->m_spitem = 2;
			pDBNpSub->m_pSplist->m_spitln = rno+1;
			pDBNpSub->m_pSplist->m_spitcl = spitcl_pos;	// 5%→3, 8%→10, 8%軽減→17, 10%→24
			m_Util.val_to_asci( &pDBNpSub->m_pSplist->m_spitval, (unsigned char*)TSrec.TsDtl[i].TsFsyu );
// 24/03/26_特定収入 add -->
			pDBNpSub->m_pSplist->m_spver = ver;
// 24/03/26_特定収入 add <--
			if( pDBNpSub->m_pSplist->UpdateEx() == ERR )	{
				pDBNpSub->NpTokuteiSyunyuKeisanClose();
				return( -1 );
			}
			spitcl_pos++;
	
			// 特定収入合計	比例
			pDBNpSub->m_pSplist->AddNewEx();
			pDBNpSub->m_pSplist->m_spseq  = sn_seq;
			pDBNpSub->m_pSplist->m_spitem = 2;
			pDBNpSub->m_pSplist->m_spitln = rno+1;
			pDBNpSub->m_pSplist->m_spitcl = spitcl_pos;	// 5%→4, 8%→11, 8%軽減→18, 10%→25
			m_Util.val_to_asci( &pDBNpSub->m_pSplist->m_spitval, (unsigned char*)TSrec.TsDtl[i].TsGsyu );
// 24/03/26_特定収入 add -->
			pDBNpSub->m_pSplist->m_spver = ver;
// 24/03/26_特定収入 add <--
			if( pDBNpSub->m_pSplist->UpdateEx() == ERR )	{
				pDBNpSub->NpTokuteiSyunyuKeisanClose();
				return( -1 );
			}
			spitcl_pos++;
	
			// 特定収入　（個別・課税売上にのみに係る）
			pDBNpSub->m_pSplist->AddNewEx();
			pDBNpSub->m_pSplist->m_spseq  = sn_seq;
			pDBNpSub->m_pSplist->m_spitem = 2;
			pDBNpSub->m_pSplist->m_spitln = rno+1;
			pDBNpSub->m_pSplist->m_spitcl = spitcl_pos;	// 5%→5, 8%→12, 8%軽減→19, 10%→26
			m_Util.val_to_asci( &pDBNpSub->m_pSplist->m_spitval, (unsigned char*)TSrec.TsDtl[i].TsSsyu );
// 24/03/26_特定収入 add -->
			pDBNpSub->m_pSplist->m_spver = ver;
// 24/03/26_特定収入 add <--
			if( pDBNpSub->m_pSplist->UpdateEx() == ERR )	{
				pDBNpSub->NpTokuteiSyunyuKeisanClose();
				return( -1 );
			}
			spitcl_pos++;
	
			// 特定収入		（個別・非課税売上げのみに係る）
			pDBNpSub->m_pSplist->AddNewEx();
			pDBNpSub->m_pSplist->m_spseq  = sn_seq;
			pDBNpSub->m_pSplist->m_spitem = 2;
			pDBNpSub->m_pSplist->m_spitln = rno+1;
			pDBNpSub->m_pSplist->m_spitcl = spitcl_pos;	// 5%→6, 8%→13, 8%軽減→20, 10%→27
			m_Util.val_to_asci( &pDBNpSub->m_pSplist->m_spitval, (unsigned char*)TSrec.TsDtl[i].TsHsyu );
// 24/03/26_特定収入 add -->
			pDBNpSub->m_pSplist->m_spver = ver;
// 24/03/26_特定収入 add <--
			if( pDBNpSub->m_pSplist->UpdateEx() == ERR )	{
				pDBNpSub->NpTokuteiSyunyuKeisanClose();
				return( -1 );
			}
			spitcl_pos++;
		
			// 特定収入		（個別・共通に係る）
			pDBNpSub->m_pSplist->AddNewEx();
			pDBNpSub->m_pSplist->m_spseq  = sn_seq;
			pDBNpSub->m_pSplist->m_spitem = 2;
			pDBNpSub->m_pSplist->m_spitln = rno+1;
			pDBNpSub->m_pSplist->m_spitcl = spitcl_pos;	// 5%→7, 8%→14, 8%軽減→21, 10%→28
			m_Util.val_to_asci( &pDBNpSub->m_pSplist->m_spitval, (unsigned char*)TSrec.TsDtl[i].TsKsyu );
// 24/03/26_特定収入 add -->
			pDBNpSub->m_pSplist->m_spver = ver;
// 24/03/26_特定収入 add <--
			if( pDBNpSub->m_pSplist->UpdateEx() == ERR )	{
				pDBNpSub->NpTokuteiSyunyuKeisanClose();
				return( -1 );
			}
			spitcl_pos++;
		
			// 外特定収入	比例
			pDBNpSub->m_pSplist->AddNewEx();
			pDBNpSub->m_pSplist->m_spseq  = sn_seq;
			pDBNpSub->m_pSplist->m_spitem = 2;
			pDBNpSub->m_pSplist->m_spitln = rno+1;
			pDBNpSub->m_pSplist->m_spitcl = spitcl_pos;	// 5%→8, 8%→15, 8%軽減→22, 10%→29
			m_Util.val_to_asci( &pDBNpSub->m_pSplist->m_spitval, (unsigned char*)TSrec.TsDtl[i].TsNosyu );
// 24/03/26_特定収入 add -->
			pDBNpSub->m_pSplist->m_spver = ver;
// 24/03/26_特定収入 add <--
			if( pDBNpSub->m_pSplist->UpdateEx() == ERR )	{
				pDBNpSub->NpTokuteiSyunyuKeisanClose();
				return( -1 );
			}
			spitcl_pos++;
		}
		rno++;
	}

// 24/02/14_特定収入 add -->
	TSREC52		TSrec52;
	for(rno=0; rno<TsTbl52.tp_rnum; rno++) {
		if(m_TblHdl.th_rget(&TSrec52,&TsTbl52,rno))	{
			break;
		}
		// TB_PARからｄｂに書き込み
// 24/03/26_特定収入 cor -->
		//Ts52_TblToDb(&TSrec52,pDBNpSub->m_pSplist,sn_seq,rno+1);
// -------------------------
		Ts52_TblToDb(&TSrec52,pDBNpSub->m_pSplist,sn_seq,rno+1,ver);
// 24/03/26_特定収入 cor <--
	}
// 24/02/14_特定収入 add <--

	pDBNpSub->NpTokuteiSyunyuKeisanClose();

	return 0;
}

//-----------------------------------------------------------------------------
// 課税売上割合95%以上？
//-----------------------------------------------------------------------------
// 引数	pSnHeadData	：	
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	課税売上割合95%以上
//			FALSE	：	課税売上割合95%以上でない
//-----------------------------------------------------------------------------
BOOL CH31SpcListData::IsRatioOver95( CSnHeadData *pSnHeadData )
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
// 控除対象仕入税額の取込
//-----------------------------------------------------------------------------
// 引数	pSyzToSpc	：	取込元データ
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
//int CH31SpcListData::SetSkjUriMony ( SYZTOSPC_DATA *pSyzToSpc )
int CH31SpcListData::SetSkjUriMony ( SYZTOSPC_DATA *pSyzToSpc, CSnHeadData *pSnHeadData )
{
	ASSERT ( pSyzToSpc );
	if ( pSyzToSpc == NULL ){
		return -1;
	}

	// 連動のときのみ金額を取得
	if( !(pSnHeadData->Sn_Sign4 & 0x80) ){
		memmove ( stval.SpKzur, pSyzToSpc->SpKzur, sizeof ( stval.SpKzur ) );
		memmove ( stval.SpKzur4, pSyzToSpc->SpKzur4, sizeof ( stval.SpKzur4 ) );
		memmove ( stval.SpKzur63, pSyzToSpc->SpKzur63, sizeof ( stval.SpKzur63 ) );
		memmove ( stval.SpKzur624, pSyzToSpc->SpKzur624, sizeof ( stval.SpKzur624 ) );
		memmove ( stval.SpKzur78, pSyzToSpc->SpKzur78, sizeof ( stval.SpKzur78 ) );
		memmove ( stval.SpMzur, pSyzToSpc->SpMzur, sizeof ( stval.SpMzur ) );
	}

	memmove ( stval.SpKuws, pSyzToSpc->SpKuws, sizeof ( stval.SpKuws) );
	memmove ( stval.SpKuwb, pSyzToSpc->SpKuwb, sizeof ( stval.SpKuwb) );


//	m_Arith.l_add ( stval.SpKuws, pSyzToSpc->SpKzur, pSyzToSpc->SpMzur );

	// 連動のときのみ取得した金額で計算
	if( !(pSnHeadData->Sn_Sign4 & 0x80) ){
		m_Arith.l_add ( stval.SpTuwb, pSyzToSpc->SpKzur, pSyzToSpc->SpMzur );
	}
	else{
		m_Arith.l_add ( stval.SpTuwb, stval.SpKzur, stval.SpMzur );
	}
	m_Arith.l_add ( stval.SpTuwb, stval.SpTuwb, stval.SpHkur );
	m_Arith.l_add ( stval.SpTuwb, stval.SpTuwb, stval.SpKgjt );


//	m_Arith.l_add ( stval.SpKuwb, pSyzToSpc->SpKzur, pSyzToSpc->SpMzur );
//	m_Arith.l_add ( stval.SpKuwb, stval.SpKuwb, pSyzToSpc->SpHkur );
	
	//m_Arith.l_par100 ( ( unsigned char* )stval.SpUwri, ( unsigned char* )stval.SpKuws, ( unsigned char* )stval.SpKuwb, 2, 0 );
	// 課税売上割合
	memmove ( stval.SpUwri, pSyzToSpc->SpUwri, sizeof ( stval.SpUwri ) );

	memmove ( stval.SpKgz3, pSyzToSpc->SpKgz3, sizeof ( stval.SpKgz3 ) );
	memmove ( stval.SpKgz4, pSyzToSpc->SpKgz4, sizeof ( stval.SpKgz4 ) );
	memmove ( stval.SpKgz63, pSyzToSpc->SpKgz63, sizeof ( stval.SpKgz63 ) );
	memmove ( stval.SpKgz624, pSyzToSpc->SpKgz624, sizeof ( stval.SpKgz624 ) );
	memmove ( stval.SpKgz78, pSyzToSpc->SpKgz78, sizeof ( stval.SpKgz78 ) );

	return 0;
}

//-----------------------------------------------------------------------------
// 特定収入を加味した控除対象仕入税額の取得
//-----------------------------------------------------------------------------
// 引数	pSyzToSpc	：	取得先データ
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CH31SpcListData::GetSkjMonyAfterSpc( SYZTOSPC_DATA *pSyzToSpc )
{
	ASSERT( pSyzToSpc );
	if( pSyzToSpc == NULL ){
		return -1;
	}

	memmove( pSyzToSpc->SpSiz3, stval.SpSiz3, sizeof(pSyzToSpc->SpSiz3) );
	memmove( pSyzToSpc->SpSiz4, stval.SpSiz4, sizeof(pSyzToSpc->SpSiz4) );
	memmove( pSyzToSpc->SpSiz63, stval.SpSiz63, sizeof(pSyzToSpc->SpSiz63) );
	memmove( pSyzToSpc->SpSiz624, stval.SpSiz624, sizeof(pSyzToSpc->SpSiz624) );
	memmove( pSyzToSpc->SpSiz78, stval.SpSiz78, sizeof(pSyzToSpc->SpSiz78) );

	return 0;
}

// 24/02/14_特定収入 add -->
//-----------------------------------------------------------------------------
// 計算表５－２　ｄｂからTB_PARにロード
//-----------------------------------------------------------------------------
// 引数		class Csplist*	：	特定収入計算表テーブル
//-----------------------------------------------------------------------------
// 返送値	なし
//-----------------------------------------------------------------------------
void CH31SpcListData::Ts52_DbToTbl( class Csplist* pSp )
{
	TSREC52		TSrec52;
	CString		cs=_T("");
	char		bf[128]={0};
	int			ii=0;
	//int			dsgn=0;

	// 該当ページのデータを読み込む
	memset(&TSrec52,0,sizeof(TSREC52));
	m_TblHdl.th_rget(&TSrec52,&TsTbl52,pSp->m_spitln-1);
	// 計算表を判断
	switch(pSp->m_spitem)	{
		case	101:	// 計算表5-2(1)
			if(pSp->m_spitcl == 0)	{
				// 課税仕入れ等に係る特定収入の種類・名称等
				cs = pSp->m_spitnm;
				cs.TrimRight();
				if(cs.IsEmpty() == FALSE)	{
					memset(bf,0,sizeof(bf));
					strcpy_s(bf,sizeof(bf),cs);
					memcpy(TSrec52.ts521.name,bf,strlen(bf));
					//dsgn=1;
				}
				// 課税仕入れ等に係る特定収入のあった課税期間（自）
				// 課税仕入れ等に係る特定収入のあった課税期間（至）
				for(ii=0; ii<2; ii++)	{
					if(ii == 0)	cs = pSp->m_spitval;
					else		cs = pSp->m_spitper;
					cs.TrimRight();
					if(cs.IsEmpty() == FALSE)	{
						memset(bf,0,sizeof(bf));
						sprintf_s(bf,sizeof(bf),_T("%06d"),_tstoi(cs));
						if(ii == 0)	memcpy(TSrec52.ts521.kikans,bf,6);
						else		memcpy(TSrec52.ts521.kikane,bf,6);
						//dsgn=1;
					}
				}
			}
			else if(pSp->m_spitcl == 1)	{
				// ①
				cs = pSp->m_spitval;
				cs.TrimRight();
				if(cs.IsEmpty() == FALSE)	{
					m_Util.val_to_bin((unsigned char *)TSrec52.ts521.val1,cs);
					//dsgn=1;
				}
				// ②
				cs = pSp->m_spitper;
				cs.TrimRight();
				if(cs.IsEmpty() == FALSE)	{
					m_Util.val_to_bin((unsigned char *)TSrec52.ts521.val2,pSp->m_spitper);
					//dsgn=1;
				}
			}
			else if(pSp->m_spitcl == 2)	{
				// ③
				cs = pSp->m_spitval;
				cs.TrimRight();
				if(cs.IsEmpty() == FALSE)	{
					m_Util.val_to_bin((unsigned char *)TSrec52.ts521.val3,cs);
					//dsgn=1;
				}
				if(pSp->m_spitnm.IsEmpty() == FALSE)	{
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts521.msgn,pSp->m_spitnm,sizeof(TSrec52.ts521.msgn));
					//dsgn=1;
				}
			}
			else if(pSp->m_spitcl == 3)	{
				// 計算表選択
				cs = pSp->m_spitval;
				cs.TrimRight();
				TSrec52.ts521.sel = _tstoi(cs);
			}
			//if(dsgn != 0)	{
			//	TSrec52.dsgn = TSrec52.dsgn | 0x01;
			//}
			break;
		case	102:	// 計算表5-2(2)
			if (pSp->m_spitcl == 0) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts522.val011, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts522.val012, pSp->m_spitper);
					//dsgn=1;
				}
			}
			else if (pSp->m_spitcl == 1) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts522.val021, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts522.val022, pSp->m_spitper);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts522.msgn, pSp->m_spitnm, sizeof(TSrec52.ts522.msgn));
					//dsgn=1;
				}
			}
			else if (pSp->m_spitcl == 2) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts522.val031, cs);
					//dsgn=1;
				}
			}
			else if (pSp->m_spitcl == 3) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts522.val041, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts522.msgn, pSp->m_spitnm, sizeof(TSrec52.ts522.msgn));
					//dsgn=1;
				}
			}
			else if (pSp->m_spitcl == 4) {
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts522.val052, cs);
					//dsgn=1;
				}
			}
			else if (pSp->m_spitcl == 5) {
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts522.val062, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts522.msgn, pSp->m_spitnm, sizeof(TSrec52.ts522.msgn));
					//dsgn=1;
				}
			}
			else if (pSp->m_spitcl == 6) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts522.val071, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts522.val072, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts522.msgn, pSp->m_spitnm, sizeof(TSrec52.ts522.msgn));
					//dsgn=1;
				}
			}
			else if (pSp->m_spitcl == 7) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts522.val081, cs);
					//dsgn=1;
				}
			}
			else if (pSp->m_spitcl == 8) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts522.val091, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts522.msgn, pSp->m_spitnm, sizeof(TSrec52.ts522.msgn));
					//dsgn=1;
				}
			}
			else if (pSp->m_spitcl == 9) {
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts522.val102, cs);
					//dsgn=1;
				}
			}
			else if (pSp->m_spitcl == 10) {
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts522.val112, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts522.msgn, pSp->m_spitnm, sizeof(TSrec52.ts522.msgn));
					//dsgn=1;
				}
			}
			else if (pSp->m_spitcl == 11) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts522.val121, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts522.val122, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts522.msgn, pSp->m_spitnm, sizeof(TSrec52.ts522.msgn));
					//dsgn=1;
				}
			}
			else if (pSp->m_spitcl == 12) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts522.val131, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts522.val132, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts522.msgn, pSp->m_spitnm, sizeof(TSrec52.ts522.msgn));
					//dsgn=1;
				}
			}
			else if (pSp->m_spitcl == 13) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts522.val141, cs);
					//dsgn=1;
				}
			}
			else if (pSp->m_spitcl == 14) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts522.val151, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts522.msgn, pSp->m_spitnm, sizeof(TSrec52.ts522.msgn));
					//dsgn=1;
				}
			}
			else if (pSp->m_spitcl == 15) {
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts522.val162, cs);
					//dsgn=1;
				}
			}
			else if (pSp->m_spitcl == 16) {
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts522.val172, cs);
					//dsgn=1;
				}
			}
			else if (pSp->m_spitcl == 17) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts522.val181, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts522.val182, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts522.msgn, pSp->m_spitnm, sizeof(TSrec52.ts522.msgn));
					//dsgn=1;
				}
			}
			else if (pSp->m_spitcl == 18) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts522.val191, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts522.val192, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts522.msgn, pSp->m_spitnm, sizeof(TSrec52.ts522.msgn));
					//dsgn=1;
				}
			}
			else if (pSp->m_spitcl == 19) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts522.val201, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts522.val202, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts522.msgn, pSp->m_spitnm, sizeof(TSrec52.ts522.msgn));
					//dsgn=1;
				}
			}
			break;
		case	103:	// 計算表5-2(3)-1
			if (pSp->m_spitcl == 0) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5231.val011, cs);
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 1) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5231.val021, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5231.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5231.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 2) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5231.val031, cs);
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 3) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5231.val041, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5231.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5231.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 4) {
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5231.val052, cs);
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 5) {
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5231.val062, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5231.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5231.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 6) {
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5231.val072, cs);
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 7) {
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5231.val082, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5231.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5231.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 8) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5231.val091, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5231.val092, cs);
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 9) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5231.val101, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5231.val102, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5231.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5231.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 10) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5231.val111, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5231.val112, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5231.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5231.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 11) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5231.val121, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5231.val122, cs);
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 12) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5231.val131, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5231.val132, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5231.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5231.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 13) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5231.val141, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5231.val142, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5231.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5231.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 14) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5231.val151, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5231.val152, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5231.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5231.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 15) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				TSrec52.ts5231.sel = _tstoi(cs);
			}
			if (pSp->m_spitcl == 16) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5231.val090, cs);
					//dsgn=1;
				}
			}
			break;
		case	104:	// 計算表5-2(3)-2
			if (pSp->m_spitcl == 0) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5232.val011, cs);
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 1) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5232.val021, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5232.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5232.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 2) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5232.val031, cs);
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 3) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5232.val041, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5232.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5232.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 4) {
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5232.val052, cs);
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 5) {
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5232.val062, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5232.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5232.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 6) {
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5232.val072, cs);
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 7) {
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5232.val082, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5232.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5232.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 8) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5232.val091, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5232.val092, cs);
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 9) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5232.val101, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5232.val102, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5232.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5232.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 10) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5232.val111, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5232.val112, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5232.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5232.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 11) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5232.val121, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5232.val122, cs);
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 12) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5232.val131, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5232.val132, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5232.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5232.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 13) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5232.val141, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5232.val142, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5232.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5232.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 14) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5232.val151, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5232.val152, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5232.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5232.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 15) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				TSrec52.ts5232.sel = _tstoi(cs);
			}
			if (pSp->m_spitcl == 16) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5232.val090, cs);
					//dsgn=1;
				}
			}
			break;
		case	105:	// 計算表5-2(3)-3
			if (pSp->m_spitcl == 0) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5233.val011, cs);
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 1) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5233.val021, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5233.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5233.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 2) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5233.val031, cs);
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 3) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5233.val041, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5233.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5233.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 4) {
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5233.val052, cs);
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 5) {
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5233.val062, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5233.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5233.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 6) {
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5233.val072, cs);
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 7) {
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5233.val082, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5233.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5233.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 8) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5233.val091, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5233.val092, cs);
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 9) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5233.val101, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5233.val102, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5233.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5233.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 10) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5233.val111, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5233.val112, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5233.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5233.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 11) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5233.val121, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5233.val122, cs);
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 12) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5233.val131, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5233.val132, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5233.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5233.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 13) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5233.val141, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5233.val142, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5233.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5233.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 14) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5233.val151, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5233.val152, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5233.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5233.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 15) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				TSrec52.ts5233.sel = _tstoi(cs);
			}
			if (pSp->m_spitcl == 16) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5233.val090, cs);
					//dsgn=1;
				}
			}
			break;
		case	106:	// 計算表5-2(4)-1
			if (pSp->m_spitcl == 0) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5241.val011, cs);
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 1) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5241.val021, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5241.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5241.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 2) {
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5241.val032, cs);
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 3) {
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5241.val042, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5241.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5241.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 4) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5241.val051, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5241.val052, cs);
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 5) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5241.val061, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5241.val062, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5241.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5241.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 6) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5241.val071, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5241.val072, cs);
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 7) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5241.val081, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5241.val082, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5241.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5241.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 8) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5241.val091, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5241.val092, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5241.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5241.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 9) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5241.val101, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5241.val102, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5241.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5241.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 10) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				TSrec52.ts5241.sel = _tstoi(cs);
			}
			if (pSp->m_spitcl == 11) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5241.val050, cs);
					//dsgn=1;
				}
			}
			break;
		case	107:	// 計算表5-2(4)-2
			if (pSp->m_spitcl == 0) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5242.val011, cs);
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 1) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5242.val021, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5242.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5242.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 2) {
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5242.val032, cs);
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 3) {
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5242.val042, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5242.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5242.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 4) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5242.val051, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5242.val052, cs);
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 5) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5242.val061, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5242.val062, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5242.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5242.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 6) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5242.val071, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5242.val072, cs);
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 7) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5242.val081, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5242.val082, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5242.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5242.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 8) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5242.val091, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5242.val092, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5242.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5242.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 9) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5242.val101, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5242.val102, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5242.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5242.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 10) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				TSrec52.ts5242.sel = _tstoi(cs);
			}
			if (pSp->m_spitcl == 11) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5242.val050, cs);
					//dsgn=1;
				}
			}
			break;
		case	108:	// 計算表5-2(4)-3
			if (pSp->m_spitcl == 0) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5243.val011, cs);
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 1) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5243.val021, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5243.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5243.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 2) {
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5243.val032, cs);
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 3) {
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5243.val042, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5243.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5243.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 4) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5243.val051, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5243.val052, cs);
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 5) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5243.val061, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5243.val062, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5243.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5243.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 6) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5243.val071, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5243.val072, cs);
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 7) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5243.val081, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5243.val082, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5243.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5243.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 8) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5243.val091, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5243.val092, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5243.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5243.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 9) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5243.val101, cs);
					//dsgn=1;
				}
				cs = pSp->m_spitper;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5243.val102, cs);
					//dsgn=1;
				}
				//実額サイン
				if (pSp->m_spitnm.IsEmpty() == FALSE) {
					m_Util.cstring_to_char((unsigned char*)TSrec52.ts5243.msgn, pSp->m_spitnm, sizeof(TSrec52.ts5243.msgn));
					//dsgn=1;
				}
			}
			if (pSp->m_spitcl == 10) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				TSrec52.ts5243.sel = _tstoi(cs);
			}
			if (pSp->m_spitcl == 11) {
				cs = pSp->m_spitval;
				cs.TrimRight();
				if (cs.IsEmpty() == FALSE) {
					m_Util.val_to_bin((unsigned char *)TSrec52.ts5243.val050, cs);
					//dsgn=1;
				}
			}
			break;
	}
	// 該当ページにデータを書き込む
	m_TblHdl.th_rput(&TSrec52,&TsTbl52,pSp->m_spitln-1);
}

//-----------------------------------------------------------------------------
// 計算表５－２　TB_PARからｄｂに書き込み
//-----------------------------------------------------------------------------
// 引数		TSREC52*		：	特定収入計算表テーブル（TB_PAR）
//			class Csplist*	：	特定収入計算表テーブル（ｄｂ）
//			int				：	テーブルＳＥＱ
//			int				：	頁
// 24/03/26_特定収入 add -->
//			int				：	バージョン
// 24/03/26_特定収入 add <--
//-----------------------------------------------------------------------------
// 返送値	なし
//-----------------------------------------------------------------------------
// 24/03/26_特定収入 cor -->
//void CH31SpcListData::Ts52_TblToDb( TSREC52* pTb, class Csplist* pSp, int pSeq, int pPg )
// -------------------------
void CH31SpcListData::Ts52_TblToDb( TSREC52* pTb, class Csplist* pSp, int pSeq, int pPg, int pVer )
// 24/03/26_特定収入 cor <--
{
	CString		cs1=_T(""),cs2=_T(""),cs3=_T("");
	char		bf[128]={0};
	char		hsgn=0;

	// 計算表５－２(1)
	hsgn=0;
	//if(pTb->dsgn & 0x01)	{
		// spitcl：0
		// 課税仕入れ等に係る特定収入の種類・名称等
		memset(bf,0,sizeof(bf));
		memcpy(bf,pTb->ts521.name,sizeof(pTb->ts521.name));
		cs1.Format(_T("%s"),bf);
		// 課税仕入れ等に係る特定収入のあった期間（自）	和暦６桁
		memset(bf,0,sizeof(bf));
		memcpy(bf,pTb->ts521.kikans,sizeof(pTb->ts521.kikans));
		cs2.Format(_T("%s"),bf);
		// 課税仕入れ等に係る特定収入のあった期間（至）	和暦６桁
		memset(bf,0,sizeof(bf));
		memcpy(bf,pTb->ts521.kikane,sizeof(pTb->ts521.kikane));
		cs3.Format(_T("%s"),bf);
// 240306_168684 del -->
		//if(cs1.IsEmpty() == FALSE || cs2.IsEmpty() == FALSE || cs3.IsEmpty() == FALSE)	{
// 240306_168684 del <--
			pSp->AddNewEx();
			pSp->m_spseq	= pSeq;
			pSp->m_spitem	= 101;
			pSp->m_spitln	= pPg;
			pSp->m_spitcl	= 0;
			pSp->m_spitnm	= cs1;
			pSp->m_spitval	= cs2;
			pSp->m_spitper	= cs3;
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn=1;
// 240306_168684 del -->
		//}
// 240306_168684 del <--
		// spitcl：1
		// ①課税仕入れ等に係る特定収入により支出された課税仕入れに係る支払対価の額の合計額
		m_Util.val_to_asci(&cs1,(unsigned char*)pTb->ts521.val1);
		// ②課税仕入れ等に係る特定収入により支出された控除対象外仕入れに係る支払対価の額の合計額
		m_Util.val_to_asci(&cs2,(unsigned char*)pTb->ts521.val2);
		if(m_Arith.l_test(pTb->ts521.val1) || m_Arith.l_test(pTb->ts521.val2))	{
			pSp->AddNewEx();
			pSp->m_spseq	= pSeq;
			pSp->m_spitem	= 101;
			pSp->m_spitln	= pPg;
			pSp->m_spitcl	= 1;
			pSp->m_spitval	= cs1;
			pSp->m_spitper	= cs2;
			pSp->m_spitnm	= _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn=1;
		}
		// spitcl：2
		// ③取戻し対象特定収入の判定（②÷①）
		m_Util.val_to_asci(&cs1,(unsigned char*)pTb->ts521.val3);
		if( m_Arith.l_test(pTb->ts521.val3) || 
			pTb->ts521.msgn[0] != 0 || pTb->ts521.msgn[1] != 0 || pTb->ts521.msgn[2] != 0 || 
			pTb->ts521.msgn[3] != 0 || pTb->ts521.msgn[4] != 0 || pTb->ts521.msgn[5] != 0 )	{
			pSp->AddNewEx();
			pSp->m_spseq	= pSeq;
			pSp->m_spitem	= 101;
			pSp->m_spitln	= pPg;
			pSp->m_spitcl	= 2;
			pSp->m_spitval	= cs1;
			pSp->m_spitper	= _T("");
			m_Util.char_to_cstring(&pSp->m_spitnm,(unsigned char*)pTb->ts521.msgn,sizeof(pTb->ts521.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn=1;
		}
		// spitcl：3
		if(hsgn)	{
			cs1.Format(_T("%d"),pTb->ts521.sel);
			pSp->AddNewEx();
			pSp->m_spseq	= pSeq;
			pSp->m_spitem	= 101;
			pSp->m_spitln	= pPg;
			pSp->m_spitcl	= 3;
			pSp->m_spitval	= cs1;
			pSp->m_spitper	= _T("");
			pSp->m_spitnm	= _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
		}
	//}
	// 計算表５－２(2)
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts522.val011);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts522.val012);
		if (m_Arith.l_test(pTb->ts522.val011) || m_Arith.l_test(pTb->ts522.val012)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 102;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 0;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts522.val021);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts522.val022);
		if (m_Arith.l_test(pTb->ts522.val021) || m_Arith.l_test(pTb->ts522.val022)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 102;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 1;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts522.msgn, sizeof(pTb->ts522.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts522.val031);
		if (m_Arith.l_test(pTb->ts522.val031)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 102;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 2;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts522.val041);
		if (m_Arith.l_test(pTb->ts522.val041)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 102;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 3;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			pSp->m_spitnm = _T("");
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts522.msgn, sizeof(pTb->ts522.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts522.val052);
		if (m_Arith.l_test(pTb->ts522.val052)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 102;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 4;
			pSp->m_spitval = _T("");
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts522.val062);
		if (m_Arith.l_test(pTb->ts522.val062)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 102;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 5;
			pSp->m_spitval = _T("");
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts522.msgn, sizeof(pTb->ts522.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts522.val071);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts522.val072);
		if (m_Arith.l_test(pTb->ts522.val071) || m_Arith.l_test(pTb->ts522.val072)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 102;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 6;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts522.msgn, sizeof(pTb->ts522.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts522.val081);
		if (m_Arith.l_test(pTb->ts522.val081)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 102;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 7;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			pSp->m_spitnm = _T("");
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts522.msgn, sizeof(pTb->ts522.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts522.val091);
		if (m_Arith.l_test(pTb->ts522.val091)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 102;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 8;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			pSp->m_spitnm = _T("");
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts522.msgn, sizeof(pTb->ts522.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts522.val102);
		if (m_Arith.l_test(pTb->ts522.val102)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 102;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 9;
			pSp->m_spitval = _T("");
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts522.val112);
		if (m_Arith.l_test(pTb->ts522.val112)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 102;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 10;
			pSp->m_spitval = _T("");
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts522.msgn, sizeof(pTb->ts522.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts522.val121);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts522.val122);
		if (m_Arith.l_test(pTb->ts522.val121) || m_Arith.l_test(pTb->ts522.val122)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 102;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 11;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts522.msgn, sizeof(pTb->ts522.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts522.val131);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts522.val132);
		if (m_Arith.l_test(pTb->ts522.val131) || m_Arith.l_test(pTb->ts522.val132)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 102;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 12;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts522.msgn, sizeof(pTb->ts522.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts522.val141);
		if (m_Arith.l_test(pTb->ts522.val141)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 102;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 13;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts522.val151);
		if (m_Arith.l_test(pTb->ts522.val151)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 102;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 14;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			pSp->m_spitnm = _T("");
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts522.msgn, sizeof(pTb->ts522.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts522.val162);
		if (m_Arith.l_test(pTb->ts522.val162)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 102;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 15;
			pSp->m_spitval = _T("");
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts522.val172);
		if (m_Arith.l_test(pTb->ts522.val172)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 102;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 16;
			pSp->m_spitval = _T("");
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts522.msgn, sizeof(pTb->ts522.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts522.val181);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts522.val182);
		if (m_Arith.l_test(pTb->ts522.val181) || m_Arith.l_test(pTb->ts522.val182)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 102;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 17;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts522.msgn, sizeof(pTb->ts522.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts522.val191);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts522.val192);
		if (m_Arith.l_test(pTb->ts522.val191) || m_Arith.l_test(pTb->ts522.val192)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 102;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 18;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts522.msgn, sizeof(pTb->ts522.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts522.val201);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts522.val202);
		if (m_Arith.l_test(pTb->ts522.val201) || m_Arith.l_test(pTb->ts522.val202)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 102;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 19;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts522.msgn, sizeof(pTb->ts522.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}

		// 計算表５－２(3)－１
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5231.val011);
		if (m_Arith.l_test(pTb->ts5231.val011)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 103;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 0;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5231.val021);
		if (m_Arith.l_test(pTb->ts5231.val021)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 103;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 1;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5231.msgn, sizeof(pTb->ts5231.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5231.val031);
		if (m_Arith.l_test(pTb->ts5231.val031)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 103;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 2;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5231.val041);
		if (m_Arith.l_test(pTb->ts5231.val041)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 103;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 3;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5231.msgn, sizeof(pTb->ts5231.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5231.val052);
		if (m_Arith.l_test(pTb->ts5231.val052)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 103;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 4;
			pSp->m_spitval = _T("");
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5231.val062);
		if (m_Arith.l_test(pTb->ts5231.val062)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 103;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 5;
			pSp->m_spitval = _T("");
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5231.msgn, sizeof(pTb->ts5231.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5231.val072);
		if (m_Arith.l_test(pTb->ts5231.val072)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 103;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 6;
			pSp->m_spitval = _T("");
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5231.val082);
		if (m_Arith.l_test(pTb->ts5231.val082)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 103;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 7;
			pSp->m_spitval = _T("");
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5231.msgn, sizeof(pTb->ts5231.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5231.val091);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5231.val092);
		if (m_Arith.l_test(pTb->ts5231.val091) || m_Arith.l_test(pTb->ts5231.val092)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 103;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 8;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5231.val101);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5231.val102);
		if (m_Arith.l_test(pTb->ts5231.val101) || m_Arith.l_test(pTb->ts5231.val102)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 103;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 9;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5231.msgn, sizeof(pTb->ts5231.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5231.val111);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5231.val112);
		if (m_Arith.l_test(pTb->ts5231.val111) || m_Arith.l_test(pTb->ts5231.val112)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 103;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 10;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5231.msgn, sizeof(pTb->ts5231.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5231.val121);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5231.val122);
		if (m_Arith.l_test(pTb->ts5231.val121) || m_Arith.l_test(pTb->ts5231.val122)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 103;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 11;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5231.val131);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5231.val132);
		if (m_Arith.l_test(pTb->ts5231.val131) || m_Arith.l_test(pTb->ts5231.val132)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 103;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 12;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5231.msgn, sizeof(pTb->ts5231.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5231.val141);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5231.val142);
		if (m_Arith.l_test(pTb->ts5231.val141) || m_Arith.l_test(pTb->ts5231.val142)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 103;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 13;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5231.msgn, sizeof(pTb->ts5231.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5231.val151);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5231.val152);
		if (m_Arith.l_test(pTb->ts5231.val151) || m_Arith.l_test(pTb->ts5231.val152)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 103;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 14;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5231.msgn, sizeof(pTb->ts5231.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		cs1.Format(_T("%d"), pTb->ts5231.sel);
		if (hsgn) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 103;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 15;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5231.val090);
		if (m_Arith.l_test(pTb->ts5231.val090)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 103;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 16;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}

		// 計算表５－２(3)－２
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5232.val011);
		if (m_Arith.l_test(pTb->ts5232.val011)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 104;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 0;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5232.val021);
		if (m_Arith.l_test(pTb->ts5232.val021)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 104;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 1;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5232.msgn, sizeof(pTb->ts5232.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5232.val031);
		if (m_Arith.l_test(pTb->ts5232.val031)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 104;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 2;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5232.val041);
		if (m_Arith.l_test(pTb->ts5232.val041)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 104;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 3;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5232.msgn, sizeof(pTb->ts5232.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5232.val052);
		if (m_Arith.l_test(pTb->ts5232.val052)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 104;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 4;
			pSp->m_spitval = _T("");
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5232.val062);
		if (m_Arith.l_test(pTb->ts5232.val062)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 104;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 5;
			pSp->m_spitval = _T("");
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5232.msgn, sizeof(pTb->ts5232.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5232.val072);
		if (m_Arith.l_test(pTb->ts5232.val072)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 104;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 6;
			pSp->m_spitval = _T("");
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5232.val082);
		if (m_Arith.l_test(pTb->ts5232.val082)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 104;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 7;
			pSp->m_spitval = _T("");
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5232.msgn, sizeof(pTb->ts5232.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5232.val091);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5232.val092);
		if (m_Arith.l_test(pTb->ts5232.val091) || m_Arith.l_test(pTb->ts5232.val092)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 104;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 8;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5232.val101);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5232.val102);
		if (m_Arith.l_test(pTb->ts5232.val101) || m_Arith.l_test(pTb->ts5232.val102)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 104;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 9;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5232.msgn, sizeof(pTb->ts5232.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5232.val111);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5232.val112);
		if (m_Arith.l_test(pTb->ts5232.val111) || m_Arith.l_test(pTb->ts5232.val112)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 104;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 10;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5232.msgn, sizeof(pTb->ts5232.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5232.val121);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5232.val122);
		if (m_Arith.l_test(pTb->ts5232.val121) || m_Arith.l_test(pTb->ts5232.val122)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 104;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 11;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5232.val131);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5232.val132);
		if (m_Arith.l_test(pTb->ts5232.val131) || m_Arith.l_test(pTb->ts5232.val132)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 104;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 12;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5232.msgn, sizeof(pTb->ts5232.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5232.val141);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5232.val142);
		if (m_Arith.l_test(pTb->ts5232.val141) || m_Arith.l_test(pTb->ts5232.val142)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 104;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 13;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5232.msgn, sizeof(pTb->ts5232.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5232.val151);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5232.val152);
		if (m_Arith.l_test(pTb->ts5232.val151) || m_Arith.l_test(pTb->ts5232.val152)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 104;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 14;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5232.msgn, sizeof(pTb->ts5232.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		cs1.Format(_T("%d"), pTb->ts5232.sel);
		if (hsgn) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 104;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 15;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5232.val090);
		if (m_Arith.l_test(pTb->ts5232.val090)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 104;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 16;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}

		// 計算表５－２(3)－３
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5233.val011);
		if (m_Arith.l_test(pTb->ts5233.val011)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 105;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 0;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5233.val021);
		if (m_Arith.l_test(pTb->ts5233.val021)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 105;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 1;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5233.msgn, sizeof(pTb->ts5233.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5233.val031);
		if (m_Arith.l_test(pTb->ts5233.val031)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 105;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 2;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5233.val041);
		if (m_Arith.l_test(pTb->ts5233.val041)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 105;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 3;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5233.msgn, sizeof(pTb->ts5233.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5233.val052);
		if (m_Arith.l_test(pTb->ts5233.val052)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 105;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 4;
			pSp->m_spitval = _T("");
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5233.val062);
		if (m_Arith.l_test(pTb->ts5233.val062)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 105;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 5;
			pSp->m_spitval = _T("");
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5233.msgn, sizeof(pTb->ts5233.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5233.val072);
		if (m_Arith.l_test(pTb->ts5233.val072)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 105;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 6;
			pSp->m_spitval = _T("");
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5233.val082);
		if (m_Arith.l_test(pTb->ts5233.val082)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 105;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 7;
			pSp->m_spitval = _T("");
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5233.msgn, sizeof(pTb->ts5233.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5233.val091);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5233.val092);
		if (m_Arith.l_test(pTb->ts5233.val091) || m_Arith.l_test(pTb->ts5233.val092)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 105;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 8;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5233.val101);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5233.val102);
		if (m_Arith.l_test(pTb->ts5233.val101) || m_Arith.l_test(pTb->ts5233.val102)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 105;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 9;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5233.msgn, sizeof(pTb->ts5233.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5233.val111);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5233.val112);
		if (m_Arith.l_test(pTb->ts5233.val111) || m_Arith.l_test(pTb->ts5233.val112)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 105;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 10;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5233.msgn, sizeof(pTb->ts5233.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5233.val121);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5233.val122);
		if (m_Arith.l_test(pTb->ts5233.val121) || m_Arith.l_test(pTb->ts5233.val122)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 105;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 11;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5233.val131);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5233.val132);
		if (m_Arith.l_test(pTb->ts5233.val131) || m_Arith.l_test(pTb->ts5233.val132)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 105;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 12;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5233.msgn, sizeof(pTb->ts5233.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5233.val141);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5233.val142);
		if (m_Arith.l_test(pTb->ts5233.val141) || m_Arith.l_test(pTb->ts5233.val142)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 105;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 13;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5233.msgn, sizeof(pTb->ts5233.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5233.val151);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5233.val152);
		if (m_Arith.l_test(pTb->ts5233.val151) || m_Arith.l_test(pTb->ts5233.val152)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 105;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 14;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5233.msgn, sizeof(pTb->ts5233.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		cs1.Format(_T("%d"), pTb->ts5233.sel);
		if (hsgn) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 105;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 15;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5233.val090);
		if (m_Arith.l_test(pTb->ts5233.val090)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 105;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 16;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}

		// 計算表５－２(4)－１
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5241.val011);
		if (m_Arith.l_test(pTb->ts5241.val011)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 106;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 0;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5241.val021);
		if (m_Arith.l_test(pTb->ts5241.val021)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 106;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 1;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5241.msgn, sizeof(pTb->ts5241.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5241.val032);
		if (m_Arith.l_test(pTb->ts5241.val032)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 106;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 2;
			pSp->m_spitval = _T("");
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5241.val042);
		if (m_Arith.l_test(pTb->ts5241.val042)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 106;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 3;
			pSp->m_spitval = _T("");
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5241.msgn, sizeof(pTb->ts5241.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5241.val051);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5241.val052);
		if (m_Arith.l_test(pTb->ts5241.val051) || m_Arith.l_test(pTb->ts5241.val052)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 106;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 4;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2; 
			pSp->m_spitnm = _T(""); 
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5241.val061);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5241.val062);
		if (m_Arith.l_test(pTb->ts5241.val061) || m_Arith.l_test(pTb->ts5241.val062)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 106;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 5;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2; 
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5241.msgn, sizeof(pTb->ts5241.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5241.val071);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5241.val072);
		if (m_Arith.l_test(pTb->ts5241.val071) || m_Arith.l_test(pTb->ts5241.val072)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 106;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 6;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5241.val081);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5241.val082);
		if (m_Arith.l_test(pTb->ts5241.val081) || m_Arith.l_test(pTb->ts5241.val082)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 106;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 7;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5241.msgn, sizeof(pTb->ts5241.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5241.val091);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5241.val092);
		if (m_Arith.l_test(pTb->ts5241.val091) || m_Arith.l_test(pTb->ts5241.val092)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 106;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 8;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5241.msgn, sizeof(pTb->ts5241.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5241.val101);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5241.val102);
		if (m_Arith.l_test(pTb->ts5241.val101) || m_Arith.l_test(pTb->ts5241.val102)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 106;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 9;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5241.msgn, sizeof(pTb->ts5241.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		cs1.Format(_T("%d"), pTb->ts5241.sel);
		if (hsgn) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 106;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 10;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5241.val050);
		if (m_Arith.l_test(pTb->ts5241.val050)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 106;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 11;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		// 計算表５－２(4)－２
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5242.val011);
		if (m_Arith.l_test(pTb->ts5242.val011)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 107;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 0;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5242.val021);
		if (m_Arith.l_test(pTb->ts5242.val021)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 107;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 1;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5242.msgn, sizeof(pTb->ts5242.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5242.val032);
		if (m_Arith.l_test(pTb->ts5242.val032)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 107;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 2;
			pSp->m_spitval = _T("");
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5242.val042);
		if (m_Arith.l_test(pTb->ts5242.val042)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 107;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 3;
			pSp->m_spitval = _T("");
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5242.msgn, sizeof(pTb->ts5242.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5242.val051);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5242.val052);
		if (m_Arith.l_test(pTb->ts5242.val051) || m_Arith.l_test(pTb->ts5242.val052)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 107;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 4;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5242.val061);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5242.val062);
		if (m_Arith.l_test(pTb->ts5242.val061) || m_Arith.l_test(pTb->ts5242.val062)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 107;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 5;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5242.msgn, sizeof(pTb->ts5242.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5242.val071);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5242.val072);
		if (m_Arith.l_test(pTb->ts5242.val071) || m_Arith.l_test(pTb->ts5242.val072)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 107;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 6;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5242.val081);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5242.val082);
		if (m_Arith.l_test(pTb->ts5242.val081) || m_Arith.l_test(pTb->ts5242.val082)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 107;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 7;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5242.msgn, sizeof(pTb->ts5242.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5242.val091);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5242.val092);
		if (m_Arith.l_test(pTb->ts5242.val091) || m_Arith.l_test(pTb->ts5242.val092)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 107;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 8;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5242.msgn, sizeof(pTb->ts5242.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5242.val101);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5242.val102);
		if (m_Arith.l_test(pTb->ts5242.val101) || m_Arith.l_test(pTb->ts5242.val102)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 107;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 9;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5242.msgn, sizeof(pTb->ts5242.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		cs1.Format(_T("%d"), pTb->ts5242.sel);
		if (hsgn) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 107;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 10;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5242.val050);
		if (m_Arith.l_test(pTb->ts5242.val050)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 107;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 11;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		// 計算表５－２(4)－３
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5243.val011);
		if (m_Arith.l_test(pTb->ts5243.val011)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 108;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 0;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5243.val021);
		if (m_Arith.l_test(pTb->ts5243.val021)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 108;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 1;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5243.msgn, sizeof(pTb->ts5243.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5243.val032);
		if (m_Arith.l_test(pTb->ts5243.val032)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 108;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 2;
			pSp->m_spitval = _T("");
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5243.val042);
		if (m_Arith.l_test(pTb->ts5243.val042)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 108;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 3;
			pSp->m_spitval = _T("");
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5243.msgn, sizeof(pTb->ts5243.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5243.val051);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5243.val052);
		if (m_Arith.l_test(pTb->ts5243.val051) || m_Arith.l_test(pTb->ts5243.val052)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 108;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 4;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5243.val061);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5243.val062);
		if (m_Arith.l_test(pTb->ts5243.val061) || m_Arith.l_test(pTb->ts5243.val062)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 108;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 5;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5243.msgn, sizeof(pTb->ts5243.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5243.val071);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5243.val072);
		if (m_Arith.l_test(pTb->ts5243.val071) || m_Arith.l_test(pTb->ts5243.val072)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 108;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 6;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5243.val081);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5243.val082);
		if (m_Arith.l_test(pTb->ts5243.val081) || m_Arith.l_test(pTb->ts5243.val082)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 108;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 7;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5243.msgn, sizeof(pTb->ts5243.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5243.val091);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5243.val092);
		if (m_Arith.l_test(pTb->ts5243.val091) || m_Arith.l_test(pTb->ts5243.val092)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 108;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 8;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5243.msgn, sizeof(pTb->ts5243.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5243.val101);
		m_Util.val_to_asci(&cs2, (unsigned char*)pTb->ts5243.val102);
		if (m_Arith.l_test(pTb->ts5243.val101) || m_Arith.l_test(pTb->ts5243.val102)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 108;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 9;
			pSp->m_spitval = cs1;
			pSp->m_spitper = cs2;
			m_Util.char_to_cstring(&pSp->m_spitnm, (unsigned char*)pTb->ts5243.msgn, sizeof(pTb->ts5243.msgn));
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
		cs1.Format(_T("%d"), pTb->ts5243.sel);
		if (hsgn) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 108;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 10;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
		}
		m_Util.val_to_asci(&cs1, (unsigned char*)pTb->ts5243.val050);
		if (m_Arith.l_test(pTb->ts5243.val050)) {
			pSp->AddNewEx();
			pSp->m_spseq = pSeq;
			pSp->m_spitem = 108;
			pSp->m_spitln = pPg;
			pSp->m_spitcl = 11;
			pSp->m_spitval = cs1;
			pSp->m_spitper = _T("");
			pSp->m_spitnm = _T("");
// 24/03/26_特定収入 add -->
			pSp->m_spver	= pVer;
// 24/03/26_特定収入 add <--
			pSp->UpdateEx();
			hsgn = 1;
		}
}

////-----------------------------------------------------------------------------
//// 計算表５－２　計算
////-----------------------------------------------------------------------------
//// 引数		TSREC52*	：	計算表５－２テーブル
////			int			：	計算を行う計算表
////								D0:計算表５－２(1)
////								D1:計算表５－２(2)
////								D2:計算表５－２(3)-1
////								D3:計算表５－２(3)-2
////								D4:計算表５－２(3)-3
////								D5:計算表５－２(4)-1
////								D6:計算表５－２(4)-2
////								D7:計算表５－２(4)-3
////-----------------------------------------------------------------------------
//// 返送値	なし
////-----------------------------------------------------------------------------
//void CH31SpcListData::Ts52_Calc( TSREC52* pTr, int pSw )
//{
//	// 計算表５－２(1)
//	//if((pSw & 0x01) && (pTr->dsgn & 0x01))	{
//	if(pSw & 0x01)	{
//	}
//}
// 24/02/14_特定収入 add <--

//// 修正No.168722 add -->
////-----------------------------------------------------------------------------
//// 初期化
////-----------------------------------------------------------------------------
//// 引数		int pRendo		0:連動、1:非連動
////-----------------------------------------------------------------------------
//int CH31SpcListData::Spc_DelData(int pRendo)
//{
//	int			ii = 0;
//	TSREC52		tsrec52;
//
////※既存の計算表１～5(1)、5(2)、5(3)は後日対応
//	////if(pRendo) {	// 非連動
//	////	// 計算表１
//	////	memset(stval.SpKzur4, 0, sizeof(stval.SpKzur4));
//	////	memset(stval.SpKzur63, 0, sizeof(stval.SpKzur63));
//	////	memset(stval.SpKzur624, 0, sizeof(stval.SpKzur624));
//	////	memset(stval.SpKzur78, 0, sizeof(stval.SpKzur78));
//	////	memset(stval.SpKzur, 0, sizeof(stval.SpKzur));
//	////	memset(stval.SpHkur, 0, sizeof(stval.SpHkur));
//	////	memset(stval.SpKgjt, 0, sizeof(stval.SpKgjt));
//
//	////	// 計算表２
//	////	//TsTbl
//	////	TsTbl.tp_rnum = 0;
//	////}
//	////else {	// 連動
//	////	// 計算表１　国外における資産の譲渡等の対価の額/合計
//	////	memset(stval.SpKgjt, 0, sizeof(stval.SpKgjt));
//	////}
//
//	// 計算表５－２
//	TsTbl52.tp_rnum = 0;
//	// 1頁分だけ追加
//	memset(&tsrec52, 0, sizeof(TSREC52));
//	m_TblHdl.th_rput(&tsrec52, &TsTbl52, 0);
//
//	return(0);
//}
//// 修正No.168722 add <--
