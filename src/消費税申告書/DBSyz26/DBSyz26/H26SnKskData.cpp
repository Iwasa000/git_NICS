#include "StdAfx.h"
#include "H26SnKskData.h"

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CH26SnKskData::CH26SnKskData()
{
	memset( &KsHead, '\0', sizeof(KSIHEAD) );
	memset( &KsData, '\0', sizeof(KSIDATA) );
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CH26SnKskData::~CH26SnKskData()
{
}

//-----------------------------------------------------------------------------
// DB からデータ読込み
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラス
//		pSnHeadData	：	消費税ヘッダ情報
//		sn_seq		：	履歴シーケンス
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CH26SnKskData::GetData( CDBNpSub *pDBNpSub, CSnHeadData *pSnHeadData, int sn_seq )
{
	ASSERT( pDBNpSub );
	ASSERT( pSnHeadData );

	CString			filter;
	CDBSyzAccess*	pDBSyzAcc;

	// 消費税拡張処理クラス
	pDBSyzAcc = NULL;
	pDBSyzAcc = new CDBSyzAccess;
	if( pDBSyzAcc == NULL )	{
//		errmes = _T("更正の請求マスターが取得出来ませんでした。");
		return ERR;
	}
	// オープン
	filter.Format( _T("KSkksw = %d"), sn_seq );
	if( pDBSyzAcc->ReaskOpen(pDBNpSub->m_database, filter) == ERR ) {
//		errmes = _T("更正の請求マスターのオープンが出来ませんでした。");
		delete( pDBSyzAcc );
		return ERR;
	}

	memset( &KsHead, '\0', sizeof( KSIHEAD ) );
	memset( &KsData, '\0', sizeof( KSIDATA ) );

// 必要？？？
//	shin_datecnv( Vol1.EE_YMD[0], (unsigned char *)&YEAR, 0 );

	// 業種区分
	if( pSnHeadData->IsSoleProprietor() ){
		KsHead.KHflg = 0x01;
	}

	int	tmpGengo=0;
	if( pDBSyzAcc->m_pReask->GetRecordCount() )	{
		// データ読み込み
		tmpGengo = 0;
		m_Util.date_to_bcdGen( 0x04, &tmpGengo, &KsData.KSdata[1], pDBSyzAcc->m_pReask->KSdata );			// 提出年月日
		KsData.KSdata[0] = tmpGengo;

		tmpGengo = 0;
		m_Util.date_to_bcdGen( 0x04, &tmpGengo, &KsData.KSktdt[1], pDBSyzAcc->m_pReask->KSktdt );			// 更正決定通知書日付
		KsData.KSktdt[0] = tmpGengo;

		KsData.KSktsw = pDBSyzAcc->m_pReask->KSktsw;														// 0=申告 1=更正 2=決定

		tmpGengo = 0;
		m_Util.date_to_bcdGen( 0x04, &tmpGengo, &KsData.KSstdt[1], pDBSyzAcc->m_pReask->KSstdt );			// 修正申告提出年月日(又は更正決定通知書受理年月日)
		KsData.KSstdt[0] = tmpGengo;

		tmpGengo = 0;
		m_Util.date_to_bcdGen( 0x04, &tmpGengo, &KsData.KSjrdt[1], pDBSyzAcc->m_pReask->KSjrdt );			// 更正決定通知書受理年月日
		KsData.KSjrdt[0] = tmpGengo;

		m_Util.cstring_to_char( (unsigned char *)&KsData.KSreas[0],  pDBSyzAcc->m_pReask->KSreas1, 80 );	// 更正の請求をする理由等	１行目
		m_Util.cstring_to_char( (unsigned char *)&KsData.KSreas[80], pDBSyzAcc->m_pReask->KSreas2, 80 );	// 〃						２行目
		m_Util.cstring_to_char( (unsigned char *)&KsData.KSreas[160],pDBSyzAcc->m_pReask->KSreas3, 80 );	// 〃						３行目
		m_Util.cstring_to_char( (unsigned char *)&KsData.KSdocu[0],  pDBSyzAcc->m_pReask->KSdocu1, 30 );	// 添付書類					１行目
		m_Util.cstring_to_char( (unsigned char *)&KsData.KSdocu[30], pDBSyzAcc->m_pReask->KSdocu2, 30 );	// 〃						２行目

		KsData.KSval[0].KVvflg = pDBSyzAcc->m_pReask->KVvflg1;										//  1 実額サイン
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[0].KVsval,	pDBSyzAcc->m_pReask->KVsval1 );		// 　 確定額
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[0].KVkval,	pDBSyzAcc->m_pReask->KVkval1 );		// 　 正当とする額
		KsData.KSval[1].KVvflg = pDBSyzAcc->m_pReask->KVvflg2;										//  2 実額サイン
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[1].KVsval,	pDBSyzAcc->m_pReask->KVsval2 );		// 　 確定額
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[1].KVkval,	pDBSyzAcc->m_pReask->KVkval2 );		// 　 正当とする額
		KsData.KSval[2].KVvflg = pDBSyzAcc->m_pReask->KVvflg3;										//  3 実額サイン
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[2].KVsval,	pDBSyzAcc->m_pReask->KVsval3 );		// 　 確定額
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[2].KVkval,	pDBSyzAcc->m_pReask->KVkval3 );		// 　 正当とする額
		KsData.KSval[3].KVvflg = pDBSyzAcc->m_pReask->KVvflg4;										//  4 実額サイン
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[3].KVsval,	pDBSyzAcc->m_pReask->KVsval4 );		// 　 確定額
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[3].KVkval,	pDBSyzAcc->m_pReask->KVkval4 );		// 　 正当とする額
		KsData.KSval[4].KVvflg = pDBSyzAcc->m_pReask->KVvflg5;										//  5 実額サイン
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[4].KVsval,	pDBSyzAcc->m_pReask->KVsval5 );		// 　 確定額
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[4].KVkval,	pDBSyzAcc->m_pReask->KVkval5 );		// 　 正当とする額
		KsData.KSval[5].KVvflg = pDBSyzAcc->m_pReask->KVvflg6;										//  6 実額サイン
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[5].KVsval,	pDBSyzAcc->m_pReask->KVsval6 );		// 　 確定額
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[5].KVkval,	pDBSyzAcc->m_pReask->KVkval6 );		// 　 正当とする額
		KsData.KSval[6].KVvflg = pDBSyzAcc->m_pReask->KVvflg7;										//  7 実額サイン
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[6].KVsval,	pDBSyzAcc->m_pReask->KVsval7 );		// 　 確定額
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[6].KVkval,	pDBSyzAcc->m_pReask->KVkval7 );		// 　 正当とする額
		KsData.KSval[7].KVvflg = pDBSyzAcc->m_pReask->KVvflg8;										//  8 実額サイン
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[7].KVsval,	pDBSyzAcc->m_pReask->KVsval8 );		// 　 確定額
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[7].KVkval,	pDBSyzAcc->m_pReask->KVkval8 );		// 　 正当とする額
		KsData.KSval[8].KVvflg = pDBSyzAcc->m_pReask->KVvflg9;										//  9 実額サイン
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[8].KVsval,	pDBSyzAcc->m_pReask->KVsval9 );		// 　 確定額
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[8].KVkval,	pDBSyzAcc->m_pReask->KVkval9 );		// 　 正当とする額
		KsData.KSval[9].KVvflg = pDBSyzAcc->m_pReask->KVvflg10;										// 10 実額サイン
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[9].KVsval,	pDBSyzAcc->m_pReask->KVsval10 );	// 　 確定額
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[9].KVkval,	pDBSyzAcc->m_pReask->KVkval10 );	// 　 正当とする額
		KsData.KSval[10].KVvflg = pDBSyzAcc->m_pReask->KVvflg11;									// 11 実額サイン
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[10].KVsval, pDBSyzAcc->m_pReask->KVsval11 );		// 　 確定額
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[10].KVkval, pDBSyzAcc->m_pReask->KVkval11 );		// 　 正当とする額
		KsData.KSval[11].KVvflg = pDBSyzAcc->m_pReask->KVvflg12;									// 12 実額サイン
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[11].KVsval, pDBSyzAcc->m_pReask->KVsval12 );		// 　 確定額
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[11].KVkval, pDBSyzAcc->m_pReask->KVkval12 );		// 　 正当とする額
		KsData.KSval[12].KVvflg = pDBSyzAcc->m_pReask->KVvflg13;									// 13 実額サイン
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[12].KVsval, pDBSyzAcc->m_pReask->KVsval13 );		// 　 確定額
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[12].KVkval, pDBSyzAcc->m_pReask->KVkval13 );		// 　 正当とする額
		KsData.KSval[13].KVvflg = pDBSyzAcc->m_pReask->KVvflg14;									// 14 実額サイン
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[13].KVsval, pDBSyzAcc->m_pReask->KVsval14 );		// 　 確定額
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[13].KVkval, pDBSyzAcc->m_pReask->KVkval14 );		// 　 正当とする額
		KsData.KSval[14].KVvflg = pDBSyzAcc->m_pReask->KVvflg15;									// 15 実額サイン
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[14].KVsval, pDBSyzAcc->m_pReask->KVsval15 );		// 　 確定額
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[14].KVkval, pDBSyzAcc->m_pReask->KVkval15 );		// 　 正当とする額
		KsData.KSval[15].KVvflg = pDBSyzAcc->m_pReask->KVvflg16;									// 16 実額サイン
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[15].KVsval, pDBSyzAcc->m_pReask->KVsval16 );		// 　 確定額
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[15].KVkval, pDBSyzAcc->m_pReask->KVkval16 );		// 　 正当とする額
		KsData.KSval[16].KVvflg = pDBSyzAcc->m_pReask->KVvflg17;									// 17 実額サイン
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[16].KVsval, pDBSyzAcc->m_pReask->KVsval17 );		// 　 確定額
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[16].KVkval, pDBSyzAcc->m_pReask->KVkval17 );		// 　 正当とする額
		KsData.KSval[17].KVvflg = pDBSyzAcc->m_pReask->KVvflg18;									// 18 実額サイン
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[17].KVsval, pDBSyzAcc->m_pReask->KVsval18 );		// 　 確定額
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[17].KVkval, pDBSyzAcc->m_pReask->KVkval18 );		// 　 正当とする額
		KsData.KSval[18].KVvflg = pDBSyzAcc->m_pReask->KVvflg19;									// 19 実額サイン
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[18].KVsval, pDBSyzAcc->m_pReask->KVsval19 );		// 　 確定額
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[18].KVkval, pDBSyzAcc->m_pReask->KVkval19 );		// 　 正当とする額
		KsData.KSval[19].KVvflg = pDBSyzAcc->m_pReask->KVvflg20;									// 20 実額サイン
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[19].KVsval, pDBSyzAcc->m_pReask->KVsval20 );		// 　 確定額
		m_Util.val_to_bin( (unsigned char *)KsData.KSval[19].KVkval, pDBSyzAcc->m_pReask->KVkval20 );		// 　 正当とする額

		m_Util.cstring_to_char( (unsigned char *)&KsData.KSbank.Bname, pDBSyzAcc->m_pReask->Bname, 32 );	// 銀行名
		KsData.KSbank.Bkbn = pDBSyzAcc->m_pReask->Bkbn;												// 銀行区分
		m_Util.cstring_to_char( (unsigned char *)&KsData.KSbank.Sname, pDBSyzAcc->m_pReask->Sname, 32 );	// 本支店名
		KsData.KSbank.Skbn = pDBSyzAcc->m_pReask->Skbn;												// 本支店区分
		m_Util.cstring_to_char( (unsigned char *)&KsData.KSbank.Dname, pDBSyzAcc->m_pReask->Dname, 24 );	// 預金名
		m_Util.cstring_to_char( (unsigned char *)&KsData.KSbank.Accnt, pDBSyzAcc->m_pReask->Accnt, 16 );	// 預金番号

		m_Util.cstring_to_char( (unsigned char *)&KsData.KSpost.Pname, pDBSyzAcc->m_pReask->Pname, 46 );	// 預金名
		m_Util.cstring_to_char( (unsigned char *)&KsData.KSpost.Acnt1, pDBSyzAcc->m_pReask->Acnt1, 10 );	// 口座番号１
		m_Util.cstring_to_char( (unsigned char *)&KsData.KSpost.Acnt2, pDBSyzAcc->m_pReask->Acnt2, 16 );	// 口座番号２

		KsData.KSbpsw = pDBSyzAcc->m_pReask->KSbpsw;												// 銀行又は郵便局選択サイン
		KsData.KSthsw = pDBSyzAcc->m_pReask->KSthsw;												// 適用法令の区分サイン
		KsData.KSkksw = pDBSyzAcc->m_pReask->KSkksw;												// 確定額区分サイン
	}

	// 後始末
	pDBSyzAcc->ReaskClose();
	delete( pDBSyzAcc );

	return 0;
}

//-----------------------------------------------------------------------------
// DB へデータ書込み
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	
//		sn_seq		：	
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CH26SnKskData::SetData( CDBNpSub *pDBNpSub, int sn_seq )
{
	ASSERT( pDBNpSub );

	CString			filter;
	CDBSyzAccess*	pDBSyzAcc;

	// 消費税拡張処理クラス
	pDBSyzAcc = NULL;
	pDBSyzAcc = new CDBSyzAccess;
	if( pDBSyzAcc == NULL )	{
//		errmes = _T("更正の請求マスターが取得出来ませんでした。");
		return ERR;
	}
	// オープン
	filter.Format( _T("KSkksw = %d"), sn_seq );
	if( pDBSyzAcc->ReaskOpen( pDBNpSub->m_database, filter ) == ERR ) {
//		errmes = _T("更正の請求マスターのオープンが出来ませんでした。");
		delete( pDBSyzAcc );
		return ERR;
	}
	// 削除
	if(	pDBSyzAcc->m_pReask->GetRecordCount() ){
		pDBSyzAcc->m_pReask->MoveFirst();
		pDBSyzAcc->m_pReask->Delete();
	}
	pDBSyzAcc->m_pReask->AddNew();

	// データ書込み
	pDBSyzAcc->m_pReask->KStype = 1;
	m_Util.date_to_bin( 0, &pDBSyzAcc->m_pReask->KSdata, &KsData.KSdata[1] );							// 提出年月日
	m_Util.date_to_bin( 0, &pDBSyzAcc->m_pReask->KSktdt, &KsData.KSktdt[1] );							// 更正決定通知書日付
	pDBSyzAcc->m_pReask->KSktsw = KsData.KSktsw;												// 0=申告 1=更正 2=決定
	m_Util.date_to_bin( 0, &pDBSyzAcc->m_pReask->KSstdt, &KsData.KSstdt[1] );							// 修正申告提出年月日(又は更正決定通知書受理年月日)	(平成固定)
	m_Util.date_to_bin( 0, &pDBSyzAcc->m_pReask->KSjrdt, &KsData.KSjrdt[1] );							// 更正決定通知書受理年月日	(平成固定)]
	m_Util.char_to_cstring( &pDBSyzAcc->m_pReask->KSreas1, (unsigned char *)&KsData.KSreas[0],  80 );	// 更正の請求をする理由等	１行目
	m_Util.char_to_cstring( &pDBSyzAcc->m_pReask->KSreas2, (unsigned char *)&KsData.KSreas[80], 80 );	// 〃						２行目
	m_Util.char_to_cstring( &pDBSyzAcc->m_pReask->KSreas3, (unsigned char *)&KsData.KSreas[160],80 );	// 〃						３行目
	m_Util.char_to_cstring( &pDBSyzAcc->m_pReask->KSdocu1, (unsigned char *)&KsData.KSdocu[0],  30 );	// 添付書類					１行目
	m_Util.char_to_cstring( &pDBSyzAcc->m_pReask->KSdocu2, (unsigned char *)&KsData.KSdocu[30], 30 );	// 〃						２行目
	pDBSyzAcc->m_pReask->KVvflg1 = KsData.KSval[0].KVvflg;										//  1 実額サイン
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVsval1, (unsigned char *)KsData.KSval[0].KVsval );		// 　 確定額
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVkval1, (unsigned char *)KsData.KSval[0].KVkval );		// 　 正当とする額
	pDBSyzAcc->m_pReask->KVvflg2 = KsData.KSval[1].KVvflg;										//  2 実額サイン
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVsval2, (unsigned char *)KsData.KSval[1].KVsval );		// 　 確定額
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVkval2, (unsigned char *)KsData.KSval[1].KVkval );		// 　 正当とする額
	pDBSyzAcc->m_pReask->KVvflg3 = KsData.KSval[2].KVvflg;										//  3 実額サイン
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVsval3, (unsigned char *)KsData.KSval[2].KVsval );		// 　 確定額
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVkval3, (unsigned char *)KsData.KSval[2].KVkval );		// 　 正当とする額
	pDBSyzAcc->m_pReask->KVvflg4 = KsData.KSval[3].KVvflg;										//  4 実額サイン
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVsval4, (unsigned char *)KsData.KSval[3].KVsval );		// 　 確定額
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVkval4, (unsigned char *)KsData.KSval[3].KVkval );		// 　 正当とする額
	pDBSyzAcc->m_pReask->KVvflg5 = KsData.KSval[4].KVvflg;										//  5 実額サイン
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVsval5, (unsigned char *)KsData.KSval[4].KVsval );		// 　 確定額
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVkval5, (unsigned char *)KsData.KSval[4].KVkval );		// 　 正当とする額
	pDBSyzAcc->m_pReask->KVvflg6 = KsData.KSval[5].KVvflg;										//  6 実額サイン
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVsval6, (unsigned char *)KsData.KSval[5].KVsval );		// 　 確定額
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVkval6, (unsigned char *)KsData.KSval[5].KVkval );		// 　 正当とする額
	pDBSyzAcc->m_pReask->KVvflg7 = KsData.KSval[6].KVvflg;										//  7 実額サイン
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVsval7, (unsigned char *)KsData.KSval[6].KVsval );		// 　 確定額
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVkval7, (unsigned char *)KsData.KSval[6].KVkval );		// 　 正当とする額
	pDBSyzAcc->m_pReask->KVvflg8 = KsData.KSval[7].KVvflg;										//  8 実額サイン
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVsval8, (unsigned char *)KsData.KSval[7].KVsval );		// 　 確定額
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVkval8, (unsigned char *)KsData.KSval[7].KVkval );		// 　 正当とする額
	pDBSyzAcc->m_pReask->KVvflg9 = KsData.KSval[8].KVvflg;										//  9 実額サイン
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVsval9, (unsigned char *)KsData.KSval[8].KVsval );		// 　 確定額
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVkval9, (unsigned char *)KsData.KSval[8].KVkval );		// 　 正当とする額
	pDBSyzAcc->m_pReask->KVvflg10 = KsData.KSval[9].KVvflg;										// 10 実額サイン
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVsval10, (unsigned char *)KsData.KSval[9].KVsval );		// 　 確定額
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVkval10, (unsigned char *)KsData.KSval[9].KVkval );		// 　 正当とする額
	pDBSyzAcc->m_pReask->KVvflg11 = KsData.KSval[10].KVvflg;									// 11 実額サイン
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVsval11, (unsigned char *)KsData.KSval[10].KVsval );	// 　 確定額
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVkval11, (unsigned char *)KsData.KSval[10].KVkval );	// 　 正当とする額
	pDBSyzAcc->m_pReask->KVvflg12 = KsData.KSval[11].KVvflg;									// 12 実額サイン
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVsval12, (unsigned char *)KsData.KSval[11].KVsval );	// 　 確定額
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVkval12, (unsigned char *)KsData.KSval[11].KVkval );	// 　 正当とする額
	pDBSyzAcc->m_pReask->KVvflg13 = KsData.KSval[12].KVvflg;									// 13 実額サイン
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVsval13, (unsigned char *)KsData.KSval[12].KVsval );	// 　 確定額
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVkval13, (unsigned char *)KsData.KSval[12].KVkval );	// 　 正当とする額
	pDBSyzAcc->m_pReask->KVvflg14 = KsData.KSval[13].KVvflg;									// 14 実額サイン
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVsval14, (unsigned char *)KsData.KSval[13].KVsval );	// 　 確定額
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVkval14, (unsigned char *)KsData.KSval[13].KVkval );	// 　 正当とする額
	pDBSyzAcc->m_pReask->KVvflg15 = KsData.KSval[14].KVvflg;									// 15 実額サイン
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVsval15, (unsigned char *)KsData.KSval[14].KVsval );	// 　 確定額
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVkval15, (unsigned char *)KsData.KSval[14].KVkval );	// 　 正当とする額
	pDBSyzAcc->m_pReask->KVvflg16 = KsData.KSval[15].KVvflg;									// 16 実額サイン
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVsval16, (unsigned char *)KsData.KSval[15].KVsval );	// 　 確定額
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVkval16, (unsigned char *)KsData.KSval[15].KVkval );	// 　 正当とする額
	pDBSyzAcc->m_pReask->KVvflg17 = KsData.KSval[16].KVvflg;									// 17 実額サイン
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVsval17, (unsigned char *)KsData.KSval[16].KVsval );	// 　 確定額
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVkval17, (unsigned char *)KsData.KSval[16].KVkval );	// 　 正当とする額
	pDBSyzAcc->m_pReask->KVvflg18 = KsData.KSval[17].KVvflg;									// 18 実額サイン
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVsval18, (unsigned char *)KsData.KSval[17].KVsval );	// 　 確定額
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVkval18, (unsigned char *)KsData.KSval[17].KVkval );	// 　 正当とする額
	pDBSyzAcc->m_pReask->KVvflg19 = KsData.KSval[18].KVvflg;									// 19 実額サイン
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVsval19, (unsigned char *)KsData.KSval[18].KVsval );	// 　 確定額
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVkval19, (unsigned char *)KsData.KSval[18].KVkval );	// 　 正当とする額
	pDBSyzAcc->m_pReask->KVvflg20 = KsData.KSval[19].KVvflg;									// 20 実額サイン
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVsval20, (unsigned char *)KsData.KSval[19].KVsval );	// 　 確定額
	m_Util.val_to_asci( &pDBSyzAcc->m_pReask->KVkval20, (unsigned char *)KsData.KSval[19].KVkval );	// 　 正当とする額

	m_Util.char_to_cstring( &pDBSyzAcc->m_pReask->Bname, (unsigned char *)KsData.KSbank.Bname, 32 );	// 銀行名
	pDBSyzAcc->m_pReask->Bkbn = KsData.KSbank.Bkbn;												// 銀行区分
	m_Util.char_to_cstring( &pDBSyzAcc->m_pReask->Sname, (unsigned char *)KsData.KSbank.Sname, 32 );	// 本支店名
	pDBSyzAcc->m_pReask->Skbn = KsData.KSbank.Skbn;												// 本支店区分
	m_Util.char_to_cstring( &pDBSyzAcc->m_pReask->Dname, (unsigned char *)KsData.KSbank.Dname, 24 );	// 預金名
	m_Util.char_to_cstring( &pDBSyzAcc->m_pReask->Accnt, (unsigned char *)KsData.KSbank.Accnt, 16 );	// 預金名

	m_Util.char_to_cstring( &pDBSyzAcc->m_pReask->Pname, (unsigned char *)KsData.KSpost.Pname, 46 );	// 預金名
	m_Util.char_to_cstring( &pDBSyzAcc->m_pReask->Acnt1, (unsigned char *)KsData.KSpost.Acnt1, 10 );	// 口座番号１
	m_Util.char_to_cstring( &pDBSyzAcc->m_pReask->Acnt2, (unsigned char *)KsData.KSpost.Acnt2, 16 );	// 口座番号２
	pDBSyzAcc->m_pReask->KSbpsw = KsData.KSbpsw;												// 銀行又は郵便局選択サイン
	pDBSyzAcc->m_pReask->KSthsw = KsData.KSthsw;												// 適用法令の区分サイン
	pDBSyzAcc->m_pReask->KSkksw = KsData.KSkksw;												// 確定額区分サイン
	pDBSyzAcc->m_pReask->KSkksw = sn_seq;														// 履歴番号

	// 更新
	if( pDBSyzAcc->m_pReask->Update() == ERR ){
//		errmes = _T("更正の請求マスターの書き込み中に障害が発生しました。");
		pDBSyzAcc->ReaskClose();
		delete( pDBSyzAcc );
		return ERR;
	}
	
	// 後始末
	pDBSyzAcc->ReaskClose();
	delete( pDBSyzAcc );

	return 0;
}

//-----------------------------------------------------------------------------
// データクリア
//-----------------------------------------------------------------------------
void CH26SnKskData::ClearData()
{
	memset( &KsData, '\0', sizeof(KSIDATA) );
}

//-----------------------------------------------------------------------------
// データ計算
//-----------------------------------------------------------------------------
// 引数	pSnHeadData	：	ヘッダ情報
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int CH26SnKskData::CalqBillVal( CSnHeadData *pSnHeadData )
{
	ASSERT( pSnHeadData );
	if( pSnHeadData == NULL ){
		return -1;
	}

	CArithEx	Arith(0x16);

	KSIVAL	*pK = KsData.KSval;
//-- '15.04.04 --
//	char	val[MONY_BUF_SIZE], PW[MONY_BUF_SIZE];
//---------------
	char	val[MONY_BUF_SIZE];
//---------------

	// この請求前の金額(確定額)
	// 7
	::ZeroMemory((pK+6)->KVsval, 6 );
	Arith.l_add( (pK+6)->KVsval, (pK+3)->KVsval, (pK+4)->KVsval );
	Arith.l_add( (pK+6)->KVsval, (pK+6)->KVsval, (pK+5)->KVsval );

	// 8/9
	::ZeroMemory( val, sizeof( val ));
	::ZeroMemory((pK+7)->KVsval, 6 );
	::ZeroMemory((pK+8)->KVsval, 6 );
	Arith.l_add( val, (pK+1)->KVsval, (pK+2)->KVsval );
	Arith.l_sub( val, val, (pK+6)->KVsval );
	if( Arith.l_test( val ) < 0 ){
		Arith.l_neg( val );
		memmove( (pK+7)->KVsval, val, 6 );
	}
	else{
//		p_calq( (char *)(pK+8)->KVsval, (char *)val, 0x10 );	// 100円未満切り捨て
		m_Util.l_calq( (char *)(pK+8)->KVsval, (char *)val, 0x10 );	// 100円未満切り捨て
	}

	// 11/12
	::ZeroMemory( val, sizeof( val ));
	::ZeroMemory((pK+10)->KVsval, 6 );
	::ZeroMemory((pK+11)->KVsval, 6 );
	Arith.l_sub( val, (pK+8)->KVsval, (pK+9)->KVsval );
//	p_calq( (char *)val, (char *)val, 0x10 );	// 100円未満切り捨て
	m_Util.l_calq( (char *)val, (char *)val, 0x10 );	// 100円未満切り捨て
	if( Arith.l_test( val ) < 0 ){
		Arith.l_neg( val );
		memmove( (pK+11)->KVsval, val, 6 );
	}
	else{
		memmove( (pK+10)->KVsval, val, 6 );
	}

//--- '14.09.09 ---	集計をかけないように！
//	if( (pSnHeadData->SVmzsw&0x01) == 0x00 ){
//		// 13/14
//		::ZeroMemory((pK+12)->KVsval, 6 );
//		memmove( (pK+12)->KVsval, (pK+7)->KVsval, 6 );
//		::ZeroMemory((pK+13)->KVsval, 6 );
//		memmove( (pK+13)->KVsval, (pK+8)->KVsval, 6 );
//
//		// 15/16
//		::ZeroMemory( val, sizeof( val ));
//		::ZeroMemory((pK+14)->KVsval, 6 );
//		::ZeroMemory((pK+15)->KVsval, 6 );
////-- '14.09.08 --
////		Arith.l_input( PW, "250" );
////		if( Arith.l_test( (pK+12)->KVsval ) ){
////			Arith.l_mul100( (unsigned char *)(pK+14)->KVsval, (unsigned char *)(pK+12)->KVsval, (unsigned char *)PW, 1, 0 );
////		}
////		if( Arith.l_test( (pK+13)->KVsval ) ){
////			Arith.l_mul100( (unsigned char *)val, (unsigned char *)(pK+13)->KVsval, (unsigned char *)PW, 1, 0 );
//////			p_calq( (char *)(pK+15)->KVsval, (char *)val, 0x10 );	// １００円未満　切り捨て
////			m_Util.l_calq( (char *)(pK+15)->KVsval, (char *)val, 0x10 );	// １００円未満　切り捨て
////		}
////---------------
//		if( Arith.l_test( (pK+12)->KVsval ) ){
//			m_Util.percent( (pK+14)->KVsval, (pK+12)->KVsval, 8, 17, 0 );
//		}
//		if( Arith.l_test( (pK+13)->KVsval ) ) {
//			m_Util.percent( val, (pK+13)->KVsval, 8, 17, 0 );
//			m_Util.l_calq( (char *)(pK+15)->KVsval, (char *)val, 0x10 );	// １００円未満　切り捨て
//		}
////---------------
//	}
//--- '14.09.09 ---	集計をかけないように！

	// 18/19
	::ZeroMemory( val, sizeof( val ));
	::ZeroMemory((pK+17)->KVsval, 6 );
	::ZeroMemory((pK+18)->KVsval, 6 );
	Arith.l_sub( val, (pK+15)->KVsval, (pK+16)->KVsval );
	if ( Arith.l_test( val ) < 0 ) {
		Arith.l_neg( val );
//		p_calq( (char *)(pK+18)->KVsval, (char *)val, 0x10 );		// １００円未満　切り捨て
		m_Util.l_calq( (char *)(pK+18)->KVsval, (char *)val, 0x10 );		// １００円未満　切り捨て
	}
	else{
//		p_calq( (char *)(pK+17)->KVsval, (char *)val, 0x10 );		// １００円未満　切り捨て
		m_Util.l_calq( (char *)(pK+17)->KVsval, (char *)val, 0x10 );		// １００円未満　切り捨て
	}

	// // 更正の請求金額(正当とする額)
	// 7
	::ZeroMemory((pK+6)->KVkval, 6 );
	Arith.l_add( (pK+6)->KVkval, (pK+3)->KVkval, (pK+4)->KVkval );
	Arith.l_add( (pK+6)->KVkval, (pK+6)->KVkval, (pK+5)->KVkval );

	// 8/9
	::ZeroMemory( val, sizeof( val ));
	::ZeroMemory((pK+7)->KVkval, 6 );
	::ZeroMemory((pK+8)->KVkval, 6 );
	Arith.l_add( val, (pK+1)->KVkval, (pK+2)->KVkval );
	Arith.l_sub( val, val, (pK+6)->KVkval );
	if( Arith.l_test( val ) < 0 ){
		Arith.l_neg( val );
		memmove( (pK+7)->KVkval, val, 6 );
	}
	else{
//		p_calq( (char *)(pK+8)->KVkval, (char *)val, 0x10 );	// 100円未満切り捨て
		m_Util.l_calq( (char *)(pK+8)->KVkval, (char *)val, 0x10 );	// 100円未満切り捨て
	}

	// 11/12
	::ZeroMemory( val, sizeof( val ));
	::ZeroMemory((pK+10)->KVkval, 6 );
	::ZeroMemory((pK+11)->KVkval, 6 );
	Arith.l_sub( val, (pK+8)->KVkval, (pK+9)->KVkval );
//	p_calq( (char *)val, (char *)val, 0x10 );	// 100円未満切り捨て
	m_Util.l_calq( (char *)val, (char *)val, 0x10 );	// 100円未満切り捨て
	if( Arith.l_test( val ) < 0 ){
		Arith.l_neg( val );
		memmove( (pK+11)->KVkval, val, 6 );
	}
	else{
		memmove( (pK+10)->KVkval, val, 6 );
	}

	if( (pSnHeadData->SVmzsw&0x01) == 0x00 ) {
		// 13/14
		::ZeroMemory((pK+12)->KVkval, 6 );
		memmove( (pK+12)->KVkval, (pK+7)->KVkval, 6 );
		::ZeroMemory((pK+13)->KVkval, 6 );
		memmove( (pK+13)->KVkval, (pK+8)->KVkval, 6 );

		// 15/16
		::ZeroMemory( val, sizeof( val ));
		::ZeroMemory((pK+14)->KVkval, 6 );
		::ZeroMemory((pK+15)->KVkval, 6 );
//-- '14.09.08 --
//		Arith.l_input( PW, "250" );
//		if( Arith.l_test( (pK+12)->KVkval ) ){
//			Arith.l_mul100( (unsigned char *)(pK+14)->KVkval, (unsigned char *)(pK+12)->KVkval, (unsigned char *)PW, 1, 0 );
//		}
//		if( Arith.l_test( (pK+13)->KVkval ) ) {
//			Arith.l_mul100( (unsigned char *)val, (unsigned char *)(pK+13)->KVkval, (unsigned char *)PW, 1, 0 );
////			p_calq( (char *)(pK+15)->KVkval, (char *)val, 0x10 );	// １００円未満　切り捨て
//			m_Util.l_calq( (char *)(pK+15)->KVkval, (char *)val, 0x10 );	// １００円未満　切り捨て
//		}
//---------------
		if( Arith.l_test( (pK+12)->KVkval ) ){
			m_Util.percent( (pK+14)->KVkval, (pK+12)->KVkval, 8, 17, 0 );
		}
		if( Arith.l_test( (pK+13)->KVkval ) ) {
			m_Util.percent( val, (pK+13)->KVkval, 8, 17, 0 );
			m_Util.l_calq( (char *)(pK+15)->KVkval, (char *)val, 0x10 );	// １００円未満　切り捨て
		}
//---------------
	}

	// 18/19
	::ZeroMemory( val, sizeof( val ));
	::ZeroMemory((pK+17)->KVkval, 6 );
	::ZeroMemory((pK+18)->KVkval, 6 );
	Arith.l_sub( val, (pK+15)->KVkval, (pK+16)->KVkval );
	if( Arith.l_test( val ) < 0 ){
		Arith.l_neg( val );
//		p_calq( (char *)(pK+18)->KVkval, (char *)val, 0x10 );		// １００円未満　切り捨て
		m_Util.l_calq( (char *)(pK+18)->KVkval, (char *)val, 0x10 );		// １００円未満　切り捨て
	}
	else{
//		p_calq( (char *)(pK+17)->KVkval, (char *)val, 0x10 );	// １００円未満　切り捨て
		m_Util.l_calq( (char *)(pK+17)->KVkval, (char *)val, 0x10 );	// １００円未満　切り捨て
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 集計済みデータのセット
//-----------------------------------------------------------------------------
// 引数	mode	：	
//		pAns	：	セット対象データ
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//-----------------------------------------------------------------------------
int CH26SnKskData::SetCalqedData( int mode, SH_ANS *pAns )
{
	int		lin;
	KSIVAL	*pK = NULL;

	for( int i=0; i<19; i++ ){
		if( i <= 11 ){
			lin = i;
		}
		else{
			lin = i+4;
		}
		pK = &KsData.KSval[i];
		if( !mode ){
			pK->KVvflg &= 0xfe;
			memmove( pK->KVsval, pAns[lin].val, MONY_BUF_SIZE );
		}
		else{
			pK->KVvflg &= 0xfd;
			memmove( pK->KVkval, pAns[lin].val, MONY_BUF_SIZE );
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 金融機関の取込
//-----------------------------------------------------------------------------
// 引数	pSnHeadData	：	ヘッダ情報
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//-----------------------------------------------------------------------------
void CH26SnKskData::SetReaskFinancial( CSnHeadData *pSnHeadData )
{
	ASSERT( pSnHeadData );

	int		len;
	char	buf[128]={0};
	char	CBF[512]={0};
	char	KBF[512]={0};

	// 金融機関選択SW
	KsData.KSbpsw = (WORD)pSnHeadData->Sn_BYsw;

	// 銀行
	::ZeroMemory( buf, sizeof(buf));
	::ZeroMemory( KsData.KSbank.Bname, sizeof(KsData.KSbank.Bname));
	memcpy( buf, pSnHeadData->Sn_BANK, sizeof(pSnHeadData->Sn_BANK));
	len = kjlen( buf, sizeof(buf) );
	memcpy( KsData.KSbank.Bname, buf, len );
	switch( pSnHeadData->Sn_FINANCE ){
		case 1:		KsData.KSbank.Bkbn = 1;		break;
		case 2:		KsData.KSbank.Bkbn = 2;		break;
		case 3:		KsData.KSbank.Bkbn = 4;		break;
		case 4:		KsData.KSbank.Bkbn = 3;		break;
		default:	KsData.KSbank.Bkbn = 0;		break;
	}
	::ZeroMemory( buf, sizeof(buf));
	::ZeroMemory( KsData.KSbank.Sname, sizeof(KsData.KSbank.Sname));
	memcpy( buf, pSnHeadData->Sn_SITEN, sizeof(pSnHeadData->Sn_SITEN));
	len = kjlen( buf, sizeof(buf) );
	if( len > 20 ){
		len = 20;
	}
	memcpy( KsData.KSbank.Sname, buf, len );
	switch(pSnHeadData->Sn_CHAIN){
		case 0:		KsData.KSbank.Skbn = 1;		break;
		case 1:		KsData.KSbank.Skbn = 2;		break;
		case 2:		KsData.KSbank.Skbn = 3;		break;
		case 3:		KsData.KSbank.Skbn = 4;		break;
		case 4:		KsData.KSbank.Skbn = 0;		break;
		case 5:		KsData.KSbank.Skbn = 5;		break;
		case 6:		KsData.KSbank.Skbn = 6;		break;		//★//[13'09.25]
		default:	KsData.KSbank.Skbn = 0;		break;
	}
	::ZeroMemory( buf, sizeof(buf));
	::ZeroMemory( KsData.KSbank.Dname, sizeof(KsData.KSbank.Dname));
	memcpy( buf, pSnHeadData->Sn_YOKIN, sizeof(pSnHeadData->Sn_YOKIN));
	len = kjlen( buf, sizeof(buf) );
	memcpy( KsData.KSbank.Dname, buf, len );
	if( (pSnHeadData->Sn_KOZA[0]&0xff) != 0xff )	{
		memmove( CBF, pSnHeadData->Sn_KOZA, 5 );
		l_unpac( KBF, CBF, 10 );
		memcpy( KsData.KSbank.Accnt, KBF, 10);
	}
	// 郵便局
	::ZeroMemory( buf, sizeof(buf));
	::ZeroMemory( KsData.KSpost.Pname, sizeof(KsData.KSpost.Pname));
	memcpy( buf, pSnHeadData->Sn_YUBIN, sizeof(pSnHeadData->Sn_YUBIN));
	len = kjlen( buf, sizeof(buf) );
	if( len > 16 ){
		len = 16;
	}
	memcpy( KsData.KSpost.Pname, buf, len );
	memcpy( KsData.KSpost.Acnt1, pSnHeadData->Sn_YUNUM1, sizeof(pSnHeadData->Sn_YUNUM1));
	memcpy( KsData.KSpost.Acnt2, pSnHeadData->Sn_YUNUM2, sizeof(pSnHeadData->Sn_YUNUM2));
}




