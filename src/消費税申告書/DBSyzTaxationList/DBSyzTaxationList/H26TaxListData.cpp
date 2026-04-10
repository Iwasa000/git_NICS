
#include "stdafx.h"
#include "H26TaxListData.h"
#include "DBShinListInt.h"

#include "ShinSellKeikaIdx.h"	// '14.09.22
#include "ShinSellIdx.h"		// '14.09.22

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CH26TaxListData::CH26TaxListData()
{
	m_Arith.l_defn( 0x16 );

	memset( &KnTbl, '\0', sizeof(TB_PAR) );
	KnTbl.tp_tno = -1;
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CH26TaxListData::~CH26TaxListData()
{
	if( KnTbl.tp_tno != -1 ){
		m_TblHdl.th_close( KnTbl.tp_tno );
		KnTbl.tp_tno = -1;
	}
}

//-----------------------------------------------------------------------------
// DB からデータ読込み
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラスのポインタ
//		pSnHeadData	：	消費税ヘッダ情報
//		sn_seq		：	対象の履歴シーケンス
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CH26TaxListData::GetData( CDBNpSub *pDBNpSub, CSnHeadData *pSnHeadData, int sn_seq )
{
	CString			filter;
	CDBSyzAccess*	pDBSyzAcc;

	// 消費税拡張処理クラス
	pDBSyzAcc = NULL;
	pDBSyzAcc = new CDBSyzAccess;
	if( pDBSyzAcc == NULL ){
//		errmes = _T("課税取引金額計算表マスターが取得出来ませんでした。");
		return ERR;
	}
	// オープン
	filter.Format( _T("kssgn5 = %d"), sn_seq );
	if( pDBSyzAcc->KjsnkOpen( pDBNpSub->m_database, filter ) == ERR ) {
//		errmes = _T("課税取引金額計算表マスターのオープンが出来ませんでした。");
		delete pDBSyzAcc;
		return ERR;
	}

	memset( &Head, '\0', sizeof( KJNHEAD ) );
	memset( &NRec, '\0', sizeof( KJNREC ) );
	char	yy=0, mm=0, dd=0;
	int		tmpGengo=0;
	pSnHeadData->GetYmdDataGen( ID_ICSSH_KESSAN_TO, &yy, &mm, &dd, &tmpGengo );
	YEAR = yy;
	Y_GENGO = tmpGengo;

	// 業種区分
	if( !(GKBN = pDBNpSub->zvol->ind_type) ){
		GKBN = 0x07;
	}
	int kjct = pDBSyzAcc->m_pKjsnk->GetRecordCount();	

	if( m_Util.IsUnConnectMst(pDBNpSub->zvol) ){
		// 非連動時
		if( kjct == 0 ){
			// 新規
			pSnHeadData->SVsign &= 0xdf;
//			Head.KHstax &= 0xfe;
			if( !(pDBNpSub->zvol->s_sgn1&0xc0) ){
				pSnHeadData->SVsign |= 0x20;	// D5を使用　ON:込み OFF=抜き
			}
			else{
				Head.KHstax = 0x01;				// 抜き
			}
			pSnHeadData->Sn_Sign2 &= 0xf7;
			if( Head.KHstax & 0x01 ){
				pSnHeadData->Sn_Sign2 |= 0x08;
			}
		}
		else{
			// メンテ
			if( pSnHeadData->Sn_Sign2&0x08 ){
				Head.KHstax = 0x01;
			}
			else{
				Head.KHstax = 0x00;
			}
		}
//2016.06.22 INSERT START
		Head.KHkoke = 0x00;
		pSnHeadData->Sn_Sign4 &= ~0x02;
//2016.06.22 INSERT END
	}
	else{
		if( kjct == 0 ){
			// 新規
			pSnHeadData->SVsign &= 0xdf;
			Head.KHstax &= 0xfe;
			if( !(pDBNpSub->zvol->s_sgn1&0xc0) ){
				pSnHeadData->SVsign |= 0x20;	// D5を使用　ON:込み OFF=抜き
			}
			else{
				Head.KHstax = 0x01;				// 抜き
			}
		}
		else{
			int	Mssw = 0;
			// メンテ
			if( pDBNpSub->zvol->s_sgn1&0xc0 ){
				// 税抜き
				pSnHeadData->SVsign &= 0xdf;
				Head.KHstax &= 0xfe;
				Head.KHstax |= 0x01;
			}
			else{
				// 税込み
				pSnHeadData->SVsign &= 0xdf;
				pSnHeadData->SVsign |= 0x20;
				Head.KHstax &= 0xfe;
			}
		}
//2016.06.22 INSERT START
		if( pSnHeadData->Sn_Sign4&0x02 ){
			Head.KHkoke = 0x01;
		}else{
			Head.KHkoke = 0x00;
		}
//2016.06.22 INSERT END
	}

	// 事業所得最大登録数
	Head.KHkjmx = 50;
	// 不動産所得最大登録数
	Head.KHkfmx = 50;
	// 農業所得最大登録数
	Head.KHknmx = 50;
	// 課税売上高計算表最大登録数
	Head.KHkumx = 50;
	// 課税仕入高計算表最大登録数
	Head.KHksmx = 50;

	KKVAL		*pKkval;		// 基本計算表集計レコード
	int			Rct;
	int			Count = 0;

	// 課税取引金額計算表（事業所得用）
	Rct = 0;
	Count = 0;
	while( 1 ){
		if(Count >= KKVAL1MAX){
//		if( (KkVal1[Rct].KKtype&0xFF) == 0xFF ){
			break;
		}
		NRec.KJval[Rct].KKtype = KkVal1[Rct].KKtype;
		NRec.KJval[Rct].KKsign = KkVal1[Rct].KKsign;
		NRec.KJval[Rct].KKauto = KkVal1[Rct].KKauto;
		Rct++;
		Count++;
	}
	NRec.KJval[Rct].KKtype = 0xFF;
	// 課税取引金額計算表（不動産取得用）
	Rct = 0;
	Count = 0;
	while(1)	{
		if(Count >= KKVAL2MAX){
			break;
		}
//		if( (KkVal2[Rct].KKtype&0xFF) == 0xFF )	break;
		NRec.KFval[Rct].KKtype = KkVal2[Rct].KKtype;
		NRec.KFval[Rct].KKsign = KkVal2[Rct].KKsign;
		NRec.KFval[Rct].KKauto = KkVal2[Rct].KKauto;
		Rct++;
		Count++;
	}
	NRec.KFval[Rct].KKtype = 0xFF;
	// 課税取引金額計算表（農業得用）
	Rct = 0;
	Count = 0;
	while(1)	{
		if(Count >= KKVAL3MAX){
			break;
		}
//		if( (KkVal3[Rct].KKtype&0xFF) == 0xFF )	break;
		NRec.KNval[Rct].KKtype = KkVal3[Rct].KKtype;
		NRec.KNval[Rct].KKsign = KkVal3[Rct].KKsign;
		NRec.KNval[Rct].KKauto = KkVal3[Rct].KKauto;
		Rct++;
		Count++;
	}
	NRec.KNval[Rct].KKtype = 0xFF;
	// 課税売上計算表
	Rct = 0;
	Count = 0;
	while(1)	{
		if(Count >= KKVAL4MAX){
			break;
		}
//		if( (KkVal4[Rct].KKtype&0xFF) == 0xFF )	break;
		NRec.KUval[Rct].KKtype = KkVal4[Rct].KKtype;
		NRec.KUval[Rct].KKsign = KkVal4[Rct].KKsign;
		NRec.KUval[Rct].KKauto = KkVal4[Rct].KKauto;
		Rct++;
		Count++;
	}
	NRec.KUval[Rct].KKtype = 0xFF;
	// 課税仕入計算表
	Rct = 0;
	Count = 0;
	while(1)	{
		if(Count >= KKVAL5MAX){
			break;
		}
//		if( (KkVal5[Rct].KKtype&0xFF) == 0xFF )	break;
		NRec.KSval[Rct].KKtype = KkVal5[Rct].KKtype;
		NRec.KSval[Rct].KKsign = KkVal5[Rct].KKsign;
		NRec.KSval[Rct].KKauto = KkVal5[Rct].KKauto;
		Rct++;
		Count++;
	}
	NRec.KSval[Rct].KKtype = 0xFF;

	// 読み込み
	int	rct = 0;
	while(1) {
		if( kjct == rct )	break;
		if( !rct ) {
			if( pDBSyzAcc->m_pKjsnk->MoveFirst() == ERR )	break;
		}
		else	{
			if( pDBSyzAcc->m_pKjsnk->MoveNext() == ERR )		break;
		}
		rct++;
		int flg = 0;
		switch( pDBSyzAcc->m_pKjsnk->kslist )	{
			case 1 :	pKkval = &NRec.KJval[pDBSyzAcc->m_pKjsnk->ksline-1];	break;
			case 2 :	pKkval = &NRec.KFval[pDBSyzAcc->m_pKjsnk->ksline-1];	break;
			case 3 :	pKkval = &NRec.KNval[pDBSyzAcc->m_pKjsnk->ksline-1];	break;
			case 4 :	pKkval = &NRec.KUval[pDBSyzAcc->m_pKjsnk->ksline-1];	break;
			case 5 :	pKkval = &NRec.KSval[pDBSyzAcc->m_pKjsnk->ksline-1];	break;
			case 6 :	pKkval = &NRec.KUval[pDBSyzAcc->m_pKjsnk->ksline-1];	break;	//★//[13'09.18]
			default:	flg = 1;	break;
		}
		if( flg )	continue;
		// 変換
		pKkval->KKtype = pDBSyzAcc->m_pKjsnk->kssgn1;
		pKkval->KKsign = pDBSyzAcc->m_pKjsnk->kssgn2;
		pKkval->KKauto = pDBSyzAcc->m_pKjsnk->kssgn3;
		switch( pDBSyzAcc->m_pKjsnk->kslist )	{
			case 4 :	if( pDBSyzAcc->m_pKjsnk->ksline == 1 )	{
							m_Util.cstring_to_char( (unsigned char *)&Head.KHkunm, pDBSyzAcc->m_pKjsnk->ksname, 10 );
						}
						break;
			case 5 :	if( pDBSyzAcc->m_pKjsnk->ksline == 1 )	{
						m_Util.cstring_to_char( (unsigned char *)&Head.KHksnm, pDBSyzAcc->m_pKjsnk->ksname, 10 );
						}
						break;
			default:
						m_Util.cstring_to_char( (unsigned char *)&pKkval->KKnam, pDBSyzAcc->m_pKjsnk->ksname, 40 );
						break;
		}

		//'14.10.08
		if(pDBSyzAcc->m_pKjsnk->kslist == 6){//経過措置ありの場合)
//		if( pSnHeadData->SVmzsw == 1 && pDBSyzAcc->m_pKjsnk->kslist == 6){//経過措置ありの場合)

			if(pDBSyzAcc->m_pKjsnk->ksitem == 1){
				m_Util.val_to_bin( (unsigned char*)pKkval->KKval5,	pDBSyzAcc->m_pKjsnk->ksval0 );
				m_Util.val_to_bin( (unsigned char*)pKkval->KKval6,	pDBSyzAcc->m_pKjsnk->ksval1 );
				m_Util.val_to_bin( (unsigned char*)pKkval->KKval7,	pDBSyzAcc->m_pKjsnk->ksval2 );
				m_Util.val_to_bin( (unsigned char*)pKkval->KKval8,	pDBSyzAcc->m_pKjsnk->ksval3 );
			}
			else{
				m_Util.val_to_bin( (unsigned char*)pKkval->KKval1,	pDBSyzAcc->m_pKjsnk->ksval0 );
				m_Util.val_to_bin( (unsigned char*)pKkval->KKval2,	pDBSyzAcc->m_pKjsnk->ksval1 );
				m_Util.val_to_bin( (unsigned char*)pKkval->KKval3,	pDBSyzAcc->m_pKjsnk->ksval2 );
				m_Util.val_to_bin( (unsigned char*)pKkval->KKval4,	pDBSyzAcc->m_pKjsnk->ksval3 );
			//	m_Util.val_to_bin( (unsigned char*)pKkval->KKval5,	pDBSyzAcc->m_pKjsnk->ksval4 );
			//	m_Util.val_to_bin( (unsigned char*)pKkval->KKval6,	pDBSyzAcc->m_pKjsnk->ksval5 );
			}
		}
		else{

			m_Util.val_to_bin( (unsigned char*)pKkval->KKval1,	pDBSyzAcc->m_pKjsnk->ksval0 );
			m_Util.val_to_bin( (unsigned char*)pKkval->KKval2,	pDBSyzAcc->m_pKjsnk->ksval1 );
			m_Util.val_to_bin( (unsigned char*)pKkval->KKval3,	pDBSyzAcc->m_pKjsnk->ksval2 );
			m_Util.val_to_bin( (unsigned char*)pKkval->KKval4,	pDBSyzAcc->m_pKjsnk->ksval3 );
			m_Util.val_to_bin( (unsigned char*)pKkval->KKval5,	pDBSyzAcc->m_pKjsnk->ksval4 );
			m_Util.val_to_bin( (unsigned char*)pKkval->KKval6,	pDBSyzAcc->m_pKjsnk->ksval5 );
		}
	}

	// 後始末
	pDBSyzAcc->KjsnkClose();
	delete pDBSyzAcc;

	return 0;
}

//-----------------------------------------------------------------------------
// DB へデータ書込み
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラスのポインタ
//		pSnHeadData	：	消費税ヘッダ情報
//		sn_seq		：	対象の履歴シーケンス
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CH26TaxListData::SetData( CDBNpSub *pDBNpSub, CSnHeadData *pSnHeadData, int sn_seq )
{
	CString			filter;
	CDBSyzAccess*	pDBSyzAcc;
	char			buf[512]={0};

	// 消費税拡張処理クラス
	pDBSyzAcc = NULL;
	pDBSyzAcc = new CDBSyzAccess;
	if( pDBSyzAcc == NULL ){
//		errmes = _T("課税取引金額計算表マスターが取得出来ませんでした。");
		return ERR;
	}
	// オープン
	filter.Format( "kssgn5 = %d and kslist >= 1 and kslist <= 6", sn_seq );
	if( pDBSyzAcc->KjsnkOpen( pDBNpSub->m_database, filter ) == ERR ) {
//		errmes = _T("課税取引金額計算表マスターがオープンが出来ませんでした。");
		delete pDBSyzAcc;
		return ERR;
	}

	int kjct = pDBSyzAcc->m_pKjsnk->GetRecordCount();

	// 課税取引金額計算表マスターの取り消し
	pDBSyzAcc->m_pKjsnk->MoveFirst();
	for( int Mct = 0; Mct != kjct; Mct++ ){
		pDBSyzAcc->m_pKjsnk->Delete();
	}

	// 課税取引金額計算表（事業所得用）
	int Rct = 0;
	int Count = 0;
	while( 1 ){
		if( Count >= KKVAL1MAX){
//		if( (NRec.KJval[Rct].KKtype&0xFF) == 0xFF ){
			break;
		}
		// 新規データ作成　送信
		pDBSyzAcc->m_pKjsnk->AddNew();
		pDBSyzAcc->m_pKjsnk->kslist = 1;
		pDBSyzAcc->m_pKjsnk->ksline = Rct + 1;
		pDBSyzAcc->m_pKjsnk->ksitem = 0;
		pDBSyzAcc->m_pKjsnk->kssgn1 = NRec.KJval[Rct].KKtype;
		pDBSyzAcc->m_pKjsnk->kssgn2 = NRec.KJval[Rct].KKsign;
		pDBSyzAcc->m_pKjsnk->kssgn3 = NRec.KJval[Rct].KKauto;
		pDBSyzAcc->m_pKjsnk->kssgn4 = 0;
		pDBSyzAcc->m_pKjsnk->kssgn5 = 0;
		pDBSyzAcc->m_pKjsnk->ksname.Empty();
		pDBSyzAcc->m_pKjsnk->ksval0.Empty();
		pDBSyzAcc->m_pKjsnk->ksval1.Empty();
		pDBSyzAcc->m_pKjsnk->ksval2.Empty();
		pDBSyzAcc->m_pKjsnk->ksval3.Empty();
		pDBSyzAcc->m_pKjsnk->ksval4.Empty();
		pDBSyzAcc->m_pKjsnk->ksval5.Empty();
		m_Util.char_to_cstring( &pDBSyzAcc->m_pKjsnk->ksname, (unsigned char*)NRec.KJval[Rct].KKnam, 40 );
		m_Arith.l_print( buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS" );
		pDBSyzAcc->m_pKjsnk->ksval0 = buf;
		pDBSyzAcc->m_pKjsnk->ksval0.TrimLeft();		// スペースカット
		m_Arith.l_print( buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS" );
		pDBSyzAcc->m_pKjsnk->ksval1 = buf;
		pDBSyzAcc->m_pKjsnk->ksval1.TrimLeft();		// スペースカット
		m_Arith.l_print( buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS" );
		pDBSyzAcc->m_pKjsnk->ksval2 = buf;
		pDBSyzAcc->m_pKjsnk->ksval2.TrimLeft();		// スペースカット
		m_Arith.l_print( buf, NRec.KJval[Rct].KKval4, "SSSSSSSSSSSSSSS" );
		pDBSyzAcc->m_pKjsnk->ksval3 = buf;
		pDBSyzAcc->m_pKjsnk->ksval3.TrimLeft();		// スペースカット
		m_Arith.l_print( buf, NRec.KJval[Rct].KKval5, "SSSSSSSSSSSSSSS" );
		pDBSyzAcc->m_pKjsnk->ksval4 = buf;
		pDBSyzAcc->m_pKjsnk->ksval4.TrimLeft();		// スペースカット
		m_Arith.l_print( buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS" );
		pDBSyzAcc->m_pKjsnk->ksval5 = buf;
		pDBSyzAcc->m_pKjsnk->ksval5.TrimLeft();		// スペースカット
		pDBSyzAcc->m_pKjsnk->kssgn5 = sn_seq;		// 履歴番号
		// 更新
		if( pDBSyzAcc->m_pKjsnk->Update() == ERR )	{
			pDBSyzAcc->KjsnkClose();
			delete( pDBSyzAcc );
			return( ERR );
		}
		Rct++;
		Count++;
	}
	// 課税取引金額計算表（不動産取得用）
	Rct = 0;
	Count = 0;
	while( 1 ){
		if( Count >= KKVAL2MAX){
//		if( (NRec.KFval[Rct].KKtype&0xFF) == 0xFF ){
			break;
		}
		// 新規データ作成　送信
		pDBSyzAcc->m_pKjsnk->AddNew();
		pDBSyzAcc->m_pKjsnk->kslist = 2;
		pDBSyzAcc->m_pKjsnk->ksline = Rct + 1;
		pDBSyzAcc->m_pKjsnk->ksitem = 0;
		pDBSyzAcc->m_pKjsnk->kssgn1 = NRec.KFval[Rct].KKtype;
		pDBSyzAcc->m_pKjsnk->kssgn2 = NRec.KFval[Rct].KKsign;
		pDBSyzAcc->m_pKjsnk->kssgn3 = NRec.KFval[Rct].KKauto;
		pDBSyzAcc->m_pKjsnk->kssgn4 = 0;
		pDBSyzAcc->m_pKjsnk->kssgn5 = 0;
		pDBSyzAcc->m_pKjsnk->ksname.Empty();
		pDBSyzAcc->m_pKjsnk->ksval0.Empty();
		pDBSyzAcc->m_pKjsnk->ksval1.Empty();
		pDBSyzAcc->m_pKjsnk->ksval2.Empty();
		pDBSyzAcc->m_pKjsnk->ksval3.Empty();
		pDBSyzAcc->m_pKjsnk->ksval4.Empty();
		pDBSyzAcc->m_pKjsnk->ksval5.Empty();
		m_Util.char_to_cstring( &pDBSyzAcc->m_pKjsnk->ksname, (unsigned char*)NRec.KFval[Rct].KKnam, 40 );
		m_Arith.l_print( buf, NRec.KFval[Rct].KKval1, "SSSSSSSSSSSSSSS" );
		pDBSyzAcc->m_pKjsnk->ksval0 = buf;
		pDBSyzAcc->m_pKjsnk->ksval0.TrimLeft();		// スペースカット
		m_Arith.l_print( buf, NRec.KFval[Rct].KKval2, "SSSSSSSSSSSSSSS" );
		pDBSyzAcc->m_pKjsnk->ksval1 = buf;
		pDBSyzAcc->m_pKjsnk->ksval1.TrimLeft();		// スペースカット
		m_Arith.l_print( buf, NRec.KFval[Rct].KKval3, "SSSSSSSSSSSSSSS" );
		pDBSyzAcc->m_pKjsnk->ksval2 = buf;
		pDBSyzAcc->m_pKjsnk->ksval2.TrimLeft();		// スペースカット
		m_Arith.l_print( buf, NRec.KFval[Rct].KKval4, "SSSSSSSSSSSSSSS" );
		pDBSyzAcc->m_pKjsnk->ksval3 = buf;
		pDBSyzAcc->m_pKjsnk->ksval3.TrimLeft();		// スペースカット
		m_Arith.l_print( buf, NRec.KFval[Rct].KKval5, "SSSSSSSSSSSSSSS" );
		pDBSyzAcc->m_pKjsnk->ksval4 = buf;
		pDBSyzAcc->m_pKjsnk->ksval4.TrimLeft();		// スペースカット
		m_Arith.l_print( buf, NRec.KFval[Rct].KKval6, "SSSSSSSSSSSSSSS" );
		pDBSyzAcc->m_pKjsnk->ksval5 = buf;
		pDBSyzAcc->m_pKjsnk->ksval5.TrimLeft();		// スペースカット
		pDBSyzAcc->m_pKjsnk->kssgn5 = sn_seq;		// 履歴番号
		// 更新
		if( pDBSyzAcc->m_pKjsnk->Update() == ERR )	{
			pDBSyzAcc->KjsnkClose();
			delete( pDBSyzAcc );
			return( ERR );
		}
		Rct++;
		Count++;
	}

	// 課税取引金額計算表（農業得用）
	Rct = 0;
	Count = 0;
	while(1)	{
		if( Count >= KKVAL3MAX){
			break;
		}
//		if( (NRec.KNval[Rct].KKtype&0xFF) == 0xFF )	break;
		// 新規データ作成　送信
		pDBSyzAcc->m_pKjsnk->AddNew();
		pDBSyzAcc->m_pKjsnk->kslist = 3;
		pDBSyzAcc->m_pKjsnk->ksline = Rct + 1;
		pDBSyzAcc->m_pKjsnk->ksitem = 0;
		pDBSyzAcc->m_pKjsnk->kssgn1 = NRec.KNval[Rct].KKtype;
		pDBSyzAcc->m_pKjsnk->kssgn2 = NRec.KNval[Rct].KKsign;
		pDBSyzAcc->m_pKjsnk->kssgn3 = NRec.KNval[Rct].KKauto;
		pDBSyzAcc->m_pKjsnk->kssgn4 = 0;
		pDBSyzAcc->m_pKjsnk->kssgn5 = 0;
		pDBSyzAcc->m_pKjsnk->ksname.Empty();
		pDBSyzAcc->m_pKjsnk->ksval0.Empty();
		pDBSyzAcc->m_pKjsnk->ksval1.Empty();
		pDBSyzAcc->m_pKjsnk->ksval2.Empty();
		pDBSyzAcc->m_pKjsnk->ksval3.Empty();
		pDBSyzAcc->m_pKjsnk->ksval4.Empty();
		pDBSyzAcc->m_pKjsnk->ksval5.Empty();
		m_Util.char_to_cstring( &pDBSyzAcc->m_pKjsnk->ksname, (unsigned char*)NRec.KNval[Rct].KKnam, 40 );
		m_Arith.l_print( buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS" );
		pDBSyzAcc->m_pKjsnk->ksval0 = buf;
		pDBSyzAcc->m_pKjsnk->ksval0.TrimLeft();		// スペースカット
		m_Arith.l_print( buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS" );
		pDBSyzAcc->m_pKjsnk->ksval1 = buf;
		pDBSyzAcc->m_pKjsnk->ksval1.TrimLeft();		// スペースカット
		m_Arith.l_print( buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS" );
		pDBSyzAcc->m_pKjsnk->ksval2 = buf;
		pDBSyzAcc->m_pKjsnk->ksval2.TrimLeft();		// スペースカット
		m_Arith.l_print( buf, NRec.KNval[Rct].KKval4, "SSSSSSSSSSSSSSS" );
		pDBSyzAcc->m_pKjsnk->ksval3 = buf;
		pDBSyzAcc->m_pKjsnk->ksval3.TrimLeft();		// スペースカット
		m_Arith.l_print( buf, NRec.KNval[Rct].KKval5, "SSSSSSSSSSSSSSS" );
		pDBSyzAcc->m_pKjsnk->ksval4 = buf;
		pDBSyzAcc->m_pKjsnk->ksval4.TrimLeft();		// スペースカット
		m_Arith.l_print( buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS" );
		pDBSyzAcc->m_pKjsnk->ksval5 = buf;
		pDBSyzAcc->m_pKjsnk->ksval5.TrimLeft();		// スペースカット
		pDBSyzAcc->m_pKjsnk->kssgn5 = sn_seq;		// 履歴番号
		// 更新
		if( pDBSyzAcc->m_pKjsnk->Update() == ERR )	{
			pDBSyzAcc->KjsnkClose();
			delete( pDBSyzAcc );
			return( ERR );
		}
		Rct++;
		Count++;
	}

	// 課税売上計算表
	Rct = 0;
	Count = 0;
	while(1)	{
		if( Count >= KKVAL4MAX){
//		if( (NRec.KUval[Rct].KKtype&0xFF) == 0xFF )	break;
			break;
		}
		// 新規データ作成　送信
		pDBSyzAcc->m_pKjsnk->AddNew();
		pDBSyzAcc->m_pKjsnk->kslist = 4;
		pDBSyzAcc->m_pKjsnk->ksline = Rct + 1;
		pDBSyzAcc->m_pKjsnk->ksitem = 0;
		pDBSyzAcc->m_pKjsnk->kssgn1 = NRec.KUval[Rct].KKtype;
		pDBSyzAcc->m_pKjsnk->kssgn2 = NRec.KUval[Rct].KKsign;
		pDBSyzAcc->m_pKjsnk->kssgn3 = NRec.KUval[Rct].KKauto;
		pDBSyzAcc->m_pKjsnk->kssgn4 = 0;
		pDBSyzAcc->m_pKjsnk->kssgn5 = 0;
		pDBSyzAcc->m_pKjsnk->ksname.Empty();
		pDBSyzAcc->m_pKjsnk->ksval0.Empty();
		pDBSyzAcc->m_pKjsnk->ksval1.Empty();
		pDBSyzAcc->m_pKjsnk->ksval2.Empty();
		pDBSyzAcc->m_pKjsnk->ksval3.Empty();
		pDBSyzAcc->m_pKjsnk->ksval4.Empty();
		pDBSyzAcc->m_pKjsnk->ksval5.Empty();
		if( !Rct ) {
			m_Util.char_to_cstring( &pDBSyzAcc->m_pKjsnk->ksname, (unsigned char*)Head.KHkunm, 10 );
		}
		m_Arith.l_print( buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS" );
		pDBSyzAcc->m_pKjsnk->ksval0 = buf;
		pDBSyzAcc->m_pKjsnk->ksval0.TrimLeft();		// スペースカット
		m_Arith.l_print( buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS" );
		pDBSyzAcc->m_pKjsnk->ksval1 = buf;
		pDBSyzAcc->m_pKjsnk->ksval1.TrimLeft();		// スペースカット
		m_Arith.l_print( buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS" );
		pDBSyzAcc->m_pKjsnk->ksval2 = buf;
		pDBSyzAcc->m_pKjsnk->ksval2.TrimLeft();		// スペースカット
		m_Arith.l_print( buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS" );
		pDBSyzAcc->m_pKjsnk->ksval3 = buf;
		pDBSyzAcc->m_pKjsnk->ksval3.TrimLeft();		// スペースカット
		m_Arith.l_print( buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS" );
		pDBSyzAcc->m_pKjsnk->ksval4 = buf;
		pDBSyzAcc->m_pKjsnk->ksval4.TrimLeft();		// スペースカット
		m_Arith.l_print( buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS" );
		pDBSyzAcc->m_pKjsnk->ksval5 = buf;
		pDBSyzAcc->m_pKjsnk->ksval5.TrimLeft();		// スペースカット
		pDBSyzAcc->m_pKjsnk->kssgn5 = sn_seq;		// 履歴番号
		// 更新
		if( pDBSyzAcc->m_pKjsnk->Update() == ERR )	{
			pDBSyzAcc->KjsnkClose();
			delete( pDBSyzAcc );
			return( ERR );
		}
		Rct++;
		Count++;
	}
//★//[13'09.18]///
	// 非連動ｄｂで簡易課税は事業別売上高を書き込む
//	if( m_Util.IsUnConnectMst(pDBNpSub->zvol) && (pDBNpSub->zvol->s_sgn2 & 0x0f) > 1 ) {	//★//[13'09.18]
	if( m_Util.IsUnConnectMst(pDBNpSub->zvol) && (m_Util.GetKazeihoushiki(pDBNpSub->zvol)==ID_ICSSH_KANNI) ){
		Rct = 40;
		Count = 0;

		//'14.10.08
		while(1)	{
			if( Count >= 10){
				break;
			}
//			if( (NRec.KUval[Rct].KKtype&0xFF) == 0xFF )	break;
			// 新規データ作成　送信
			pDBSyzAcc->m_pKjsnk->AddNew();
			pDBSyzAcc->m_pKjsnk->kslist = 6;
			pDBSyzAcc->m_pKjsnk->ksline = Rct + 1;
			pDBSyzAcc->m_pKjsnk->ksitem = 0;
			pDBSyzAcc->m_pKjsnk->kssgn1 = NRec.KUval[Rct].KKtype;
			pDBSyzAcc->m_pKjsnk->kssgn2 = NRec.KUval[Rct].KKsign;
			pDBSyzAcc->m_pKjsnk->kssgn3 = NRec.KUval[Rct].KKauto;
			pDBSyzAcc->m_pKjsnk->kssgn4 = 0;
			pDBSyzAcc->m_pKjsnk->kssgn5 = 0;
			pDBSyzAcc->m_pKjsnk->ksname.Empty();
			pDBSyzAcc->m_pKjsnk->ksval0.Empty();
			pDBSyzAcc->m_pKjsnk->ksval1.Empty();
			pDBSyzAcc->m_pKjsnk->ksval2.Empty();
			pDBSyzAcc->m_pKjsnk->ksval3.Empty();
//			pDBSyzAcc->m_pKjsnk->ksval4.Empty();
//			pDBSyzAcc->m_pKjsnk->ksval5.Empty();
			if( !Rct ) {
				m_Util.char_to_cstring( &pDBSyzAcc->m_pKjsnk->ksname, (unsigned char*)Head.KHkunm, 10 );
			}
			m_Arith.l_print( buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS" );
			pDBSyzAcc->m_pKjsnk->ksval0 = buf;
			pDBSyzAcc->m_pKjsnk->ksval0.TrimLeft();		// スペースカット
			m_Arith.l_print( buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS" );
			pDBSyzAcc->m_pKjsnk->ksval1 = buf;
			pDBSyzAcc->m_pKjsnk->ksval1.TrimLeft();		// スペースカット
			m_Arith.l_print( buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS" );
			pDBSyzAcc->m_pKjsnk->ksval2 = buf;
			pDBSyzAcc->m_pKjsnk->ksval2.TrimLeft();		// スペースカット
			m_Arith.l_print( buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS" );
			pDBSyzAcc->m_pKjsnk->ksval3 = buf;
			pDBSyzAcc->m_pKjsnk->ksval3.TrimLeft();		// スペースカット
//			m_Arith.l_print( buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS" );
//			pDBSyzAcc->m_pKjsnk->ksval4 = buf;
//			pDBSyzAcc->m_pKjsnk->ksval4.TrimLeft();		// スペースカット
//			m_Arith.l_print( buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS" );
//			pDBSyzAcc->m_pKjsnk->ksval5 = buf;
//			pDBSyzAcc->m_pKjsnk->ksval5.TrimLeft();		// スペースカット
			pDBSyzAcc->m_pKjsnk->kssgn5 = sn_seq;		// 履歴番号
			// 更新
			if( pDBSyzAcc->m_pKjsnk->Update() == ERR )	{
				pDBSyzAcc->KjsnkClose();
				delete( pDBSyzAcc );
				return( ERR );
			}
			Rct++;
			Count++;
		}

		//'14.10.08
	//	if( pSnHeadData->SVmzsw == 1 ){//経過措置ありの場合
			Rct = 40;
			Count = 0;
			while(1)	{
				if( Count >= 10){
					break;
				}
		//		if( (NRec.KUval[Rct].KKtype&0xFF) == 0xFF )	break;
				// 新規データ作成　送信
				pDBSyzAcc->m_pKjsnk->AddNew();
				pDBSyzAcc->m_pKjsnk->kslist = 6;
				pDBSyzAcc->m_pKjsnk->ksline = Rct + 1;
				pDBSyzAcc->m_pKjsnk->ksitem = 1;//アイテムを１にする（うち税率の分）
				pDBSyzAcc->m_pKjsnk->kssgn1 = NRec.KUval[Rct].KKtype;
				pDBSyzAcc->m_pKjsnk->kssgn2 = NRec.KUval[Rct].KKsign;
				pDBSyzAcc->m_pKjsnk->kssgn3 = NRec.KUval[Rct].KKauto;
				pDBSyzAcc->m_pKjsnk->kssgn4 = 0;
				pDBSyzAcc->m_pKjsnk->kssgn5 = 0;
				pDBSyzAcc->m_pKjsnk->ksname.Empty();
				pDBSyzAcc->m_pKjsnk->ksval0.Empty();
				pDBSyzAcc->m_pKjsnk->ksval1.Empty();
				pDBSyzAcc->m_pKjsnk->ksval2.Empty();
				pDBSyzAcc->m_pKjsnk->ksval3.Empty();
//				pDBSyzAcc->m_pKjsnk->ksval4.Empty();
//				pDBSyzAcc->m_pKjsnk->ksval5.Empty();
//				pDBSyzAcc->m_pKjsnk->ksval6.Empty();
//				pDBSyzAcc->m_pKjsnk->ksval7.Empty();
//				pDBSyzAcc->m_pKjsnk->ksval8.Empty();
				if( !Rct ) {
					m_Util.char_to_cstring( &pDBSyzAcc->m_pKjsnk->ksname, (unsigned char*)Head.KHkunm, 10 );
				}
				m_Arith.l_print( buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS" );
				pDBSyzAcc->m_pKjsnk->ksval0 = buf;
				pDBSyzAcc->m_pKjsnk->ksval0.TrimLeft();		// スペースカット
				m_Arith.l_print( buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS" );
				pDBSyzAcc->m_pKjsnk->ksval1 = buf;
				pDBSyzAcc->m_pKjsnk->ksval1.TrimLeft();		// スペースカット
				m_Arith.l_print( buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS" );
				pDBSyzAcc->m_pKjsnk->ksval2 = buf;
				pDBSyzAcc->m_pKjsnk->ksval2.TrimLeft();		// スペースカット
				m_Arith.l_print( buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS" );
				pDBSyzAcc->m_pKjsnk->ksval3 = buf;
				pDBSyzAcc->m_pKjsnk->ksval3.TrimLeft();		// スペースカット
				pDBSyzAcc->m_pKjsnk->kssgn5 = sn_seq;		// 履歴番号
				// 更新
				if( pDBSyzAcc->m_pKjsnk->Update() == ERR )	{
					pDBSyzAcc->KjsnkClose();
					delete( pDBSyzAcc );
					return( ERR );
				}
				Rct++;
				Count++;
			}


	//	}



		/*  '14.10.08データベースの保存方法変える

		while(1)	{
			if( (NRec.KUval[Rct].KKtype&0xFF) == 0xFF )	break;
			// 新規データ作成　送信
			pDBSyzAcc->m_pKjsnk->AddNew();
			pDBSyzAcc->m_pKjsnk->kslist = 6;
			pDBSyzAcc->m_pKjsnk->ksline = Rct + 1;
			pDBSyzAcc->m_pKjsnk->ksitem = 0;
			pDBSyzAcc->m_pKjsnk->kssgn1 = NRec.KUval[Rct].KKtype;
			pDBSyzAcc->m_pKjsnk->kssgn2 = NRec.KUval[Rct].KKsign;
			pDBSyzAcc->m_pKjsnk->kssgn3 = NRec.KUval[Rct].KKauto;
			pDBSyzAcc->m_pKjsnk->kssgn4 = 0;
			pDBSyzAcc->m_pKjsnk->kssgn5 = 0;
			pDBSyzAcc->m_pKjsnk->ksname.Empty();
			pDBSyzAcc->m_pKjsnk->ksval0.Empty();
			pDBSyzAcc->m_pKjsnk->ksval1.Empty();
			pDBSyzAcc->m_pKjsnk->ksval2.Empty();
			pDBSyzAcc->m_pKjsnk->ksval3.Empty();
			pDBSyzAcc->m_pKjsnk->ksval4.Empty();
			pDBSyzAcc->m_pKjsnk->ksval5.Empty();
			if( !Rct ) {
				m_Util.char_to_cstring( &pDBSyzAcc->m_pKjsnk->ksname, (unsigned char*)Head.KHkunm, 10 );
			}
			m_Arith.l_print( buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS" );
			pDBSyzAcc->m_pKjsnk->ksval0 = buf;
			pDBSyzAcc->m_pKjsnk->ksval0.TrimLeft();		// スペースカット
			m_Arith.l_print( buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS" );
			pDBSyzAcc->m_pKjsnk->ksval1 = buf;
			pDBSyzAcc->m_pKjsnk->ksval1.TrimLeft();		// スペースカット
			m_Arith.l_print( buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS" );
			pDBSyzAcc->m_pKjsnk->ksval2 = buf;
			pDBSyzAcc->m_pKjsnk->ksval2.TrimLeft();		// スペースカット
			m_Arith.l_print( buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS" );
			pDBSyzAcc->m_pKjsnk->ksval3 = buf;
			pDBSyzAcc->m_pKjsnk->ksval3.TrimLeft();		// スペースカット
			m_Arith.l_print( buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS" );
			pDBSyzAcc->m_pKjsnk->ksval4 = buf;
			pDBSyzAcc->m_pKjsnk->ksval4.TrimLeft();		// スペースカット
			m_Arith.l_print( buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS" );
			pDBSyzAcc->m_pKjsnk->ksval5 = buf;
			pDBSyzAcc->m_pKjsnk->ksval5.TrimLeft();		// スペースカット
			pDBSyzAcc->m_pKjsnk->kssgn5 = sn_seq;		// 履歴番号
			// 更新
			if( pDBSyzAcc->m_pKjsnk->Update() == ERR )	{
				pDBSyzAcc->KjsnkClose();
				delete( pDBSyzAcc );
				return( ERR );
			}
			Rct++;
		}
		*/
	}
///////////////////

	// 課税仕入計算表
	Rct = 0;
	Count = 0;
	while(1)	{
		if( Count >= KKVAL5MAX){
//		if( (NRec.KSval[Rct].KKtype&0xFF) == 0xFF )	break;
			break;
		}
		// 新規データ作成　送信
		pDBSyzAcc->m_pKjsnk->AddNew();
		pDBSyzAcc->m_pKjsnk->kslist = 5;
		pDBSyzAcc->m_pKjsnk->ksline = Rct + 1;
		pDBSyzAcc->m_pKjsnk->ksitem = 0;
		pDBSyzAcc->m_pKjsnk->kssgn1 = NRec.KSval[Rct].KKtype;
		pDBSyzAcc->m_pKjsnk->kssgn2 = NRec.KSval[Rct].KKsign;
		pDBSyzAcc->m_pKjsnk->kssgn3 = NRec.KSval[Rct].KKauto;
		pDBSyzAcc->m_pKjsnk->kssgn4 = 0;
		pDBSyzAcc->m_pKjsnk->kssgn5 = 0;
		pDBSyzAcc->m_pKjsnk->ksname.Empty();
		pDBSyzAcc->m_pKjsnk->ksval0.Empty();
		pDBSyzAcc->m_pKjsnk->ksval1.Empty();
		pDBSyzAcc->m_pKjsnk->ksval2.Empty();
		pDBSyzAcc->m_pKjsnk->ksval3.Empty();
		pDBSyzAcc->m_pKjsnk->ksval4.Empty();
		pDBSyzAcc->m_pKjsnk->ksval5.Empty();
		if( !Rct ) {
			m_Util.char_to_cstring( &pDBSyzAcc->m_pKjsnk->ksname, (unsigned char*)Head.KHksnm, 10 );
		}
		m_Arith.l_print( buf, NRec.KSval[Rct].KKval1, "SSSSSSSSSSSSSSS" );
		pDBSyzAcc->m_pKjsnk->ksval0 = buf;
		pDBSyzAcc->m_pKjsnk->ksval0.TrimLeft();		// スペースカット
		m_Arith.l_print( buf, NRec.KSval[Rct].KKval2, "SSSSSSSSSSSSSSS" );
		pDBSyzAcc->m_pKjsnk->ksval1 = buf;
		pDBSyzAcc->m_pKjsnk->ksval1.TrimLeft();		// スペースカット
		m_Arith.l_print( buf, NRec.KSval[Rct].KKval3, "SSSSSSSSSSSSSSS" );
		pDBSyzAcc->m_pKjsnk->ksval2 = buf;
		pDBSyzAcc->m_pKjsnk->ksval2.TrimLeft();		// スペースカット
		m_Arith.l_print( buf, NRec.KSval[Rct].KKval4, "SSSSSSSSSSSSSSS" );
		pDBSyzAcc->m_pKjsnk->ksval3 = buf;
		pDBSyzAcc->m_pKjsnk->ksval3.TrimLeft();		// スペースカット
		m_Arith.l_print( buf, NRec.KSval[Rct].KKval5, "SSSSSSSSSSSSSSS" );
		pDBSyzAcc->m_pKjsnk->ksval4 = buf;
		pDBSyzAcc->m_pKjsnk->ksval4.TrimLeft();		// スペースカット
		m_Arith.l_print( buf, NRec.KSval[Rct].KKval6, "SSSSSSSSSSSSSSS" );
		pDBSyzAcc->m_pKjsnk->ksval5 = buf;
		pDBSyzAcc->m_pKjsnk->ksval5.TrimLeft();		// スペースカット
		pDBSyzAcc->m_pKjsnk->kssgn5 = sn_seq;		// 履歴番号
		// 更新
		if( pDBSyzAcc->m_pKjsnk->Update() == ERR )	{
			pDBSyzAcc->KjsnkClose();
			delete( pDBSyzAcc );
			return( ERR );
		}
		Rct++;
		Count++;
	}

	// 税込き・税抜き
	pSnHeadData->SVsign &= 0xdf;
	if( !(Head.KHstax&0x01) ){
		pSnHeadData->SVsign |= 0x20;	// 込み
	}

//2016.06.22 INSERT START
	if( Head.KHkoke&0x01 ){
		pSnHeadData->Sn_Sign4 |= 0x02;
	}else{
		pSnHeadData->Sn_Sign4 &= ~0x02;
	}
//2016.06.22 INSERT END

	// 後始末
	pDBSyzAcc->KjsnkClose();
	delete( pDBSyzAcc );

	return 0;
}

//-----------------------------------------------------------------------------
// 財務連動（'14.08.08）
//-----------------------------------------------------------------------------
// 引数	pDBNpSub				：	財務クラスのポインタ
//		pSyzSyukei				：	消費税集計クラスのポインタ
//		smon					：	開始月
//		emon					：	終了月
//		IsTransitionalMeasure	：	経過措置あり？
//-----------------------------------------------------------------------------
// 返送値	0					：	正常終了
//			-1					：	エラー
//-----------------------------------------------------------------------------
int CH26TaxListData::ConnectZmdata( CDBNpSub *pDBNpSub, CSyzSyukei *pSyzSyukei, int smon, int emon, BOOL IsTransitionalMeasure )
{
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}
	ASSERT( pSyzSyukei );
	if( pSyzSyukei == NULL ){
		return -1;
	}

	// 科目名称の読み込み
	if( KnTbl.tp_tno == -1 ){
		if( ReadKnTabl(pDBNpSub) ){
			return -1;
		}
	}

	// 集計レコードの初期設定
	for( int i=0; i<50; i++ ){
		// 課税取引金額計算表（事業所得用）
		NRec.KJval[i].KKauto = 0x00;
		memset( NRec.KJval[i].KKval1, '\0', sizeof(NRec.KJval[i].KKval1) );
		memset( NRec.KJval[i].KKval2, '\0', sizeof(NRec.KJval[i].KKval2) );
		memset( NRec.KJval[i].KKval3, '\0', sizeof(NRec.KJval[i].KKval3) );
		memset( NRec.KJval[i].KKval4, '\0', sizeof(NRec.KJval[i].KKval4) );
		memset( NRec.KJval[i].KKval5, '\0', sizeof(NRec.KJval[i].KKval5) );
		// 課税取引金額計算表（不動産取得用）
		NRec.KFval[i].KKauto = 0x00;
		memset( NRec.KFval[i].KKval1, '\0', sizeof(NRec.KFval[i].KKval1) );
		memset( NRec.KFval[i].KKval2, '\0', sizeof(NRec.KFval[i].KKval2) );
		memset( NRec.KFval[i].KKval3, '\0', sizeof(NRec.KFval[i].KKval3) );
		memset( NRec.KFval[i].KKval4, '\0', sizeof(NRec.KFval[i].KKval4) );
		memset( NRec.KFval[i].KKval5, '\0', sizeof(NRec.KFval[i].KKval5) );
		// 課税取引金額計算表（農業得用）
		NRec.KNval[i].KKauto = 0x00;
		memset( NRec.KNval[i].KKval1, '\0', sizeof(NRec.KNval[i].KKval1) );
		memset( NRec.KNval[i].KKval2, '\0', sizeof(NRec.KNval[i].KKval2) );
		memset( NRec.KNval[i].KKval3, '\0', sizeof(NRec.KNval[i].KKval3) );
		memset( NRec.KNval[i].KKval4, '\0', sizeof(NRec.KNval[i].KKval4) );
		memset( NRec.KNval[i].KKval5, '\0', sizeof(NRec.KNval[i].KKval5) );

//-- '14.09.22 --
//		if( (i==9) || (i==10) || (i==11) ){
//			continue;
//		}
//---------------
		if( IsTransitionalMeasure ){
			if( (i==KSELLINE7) || (i==KSELLINE8) || (i==KSELLINE9) ){
				continue;
			}
		}
		else{
			if( (i==SELLINE10) || (i==SELLINE11) || (i==SELLINE12) ){
				continue;
			}
		}
//---------------
		
		// 課税売上計算表
		NRec.KUval[i].KKauto = 0x00;
		memset( NRec.KUval[i].KKval1, '\0', sizeof(NRec.KUval[i].KKval1) );
		memset( NRec.KUval[i].KKval2, '\0', sizeof(NRec.KUval[i].KKval2) );
		memset( NRec.KUval[i].KKval3, '\0', sizeof(NRec.KUval[i].KKval3) );
		memset( NRec.KUval[i].KKval4, '\0', sizeof(NRec.KUval[i].KKval4) );
		memset( NRec.KUval[i].KKval5, '\0', sizeof(NRec.KUval[i].KKval5) );
		// 課税仕入計算表
		NRec.KSval[i].KKauto = 0x00;
		memset( NRec.KSval[i].KKval1, '\0', sizeof(NRec.KSval[i].KKval1) );
		memset( NRec.KSval[i].KKval2, '\0', sizeof(NRec.KSval[i].KKval2) );
		memset( NRec.KSval[i].KKval3, '\0', sizeof(NRec.KSval[i].KKval3) );
		memset( NRec.KSval[i].KKval4, '\0', sizeof(NRec.KSval[i].KKval4) );
		memset( NRec.KSval[i].KKval5, '\0', sizeof(NRec.KSval[i].KKval5) );
	}

	// 税抜き及び税込みに設定
	int shinsw = 0;
	if( Head.KHstax&0x01 ){
		shinsw = 0;	// 抜き「抜き又は一括税抜き」
	}
	else{
		shinsw = 1;	// 込み「込み」
	}

	// 最終決算修正月
	int		lstKsOfs = -1;
	// 除外月
	char	exceptMonth[20]={0};
	if( pDBNpSub->minfo == NULL ){
		if( pDBNpSub->MinfoOpen() ){
			return -1;
		}
	}
	for( int i=0; i<16; i++ ){
		if( i==0 ){
			if( (pDBNpSub->minfo->MoveFirst()==-1) || (pDBNpSub->minfo->st==-1) ){
				break;
			}
		}
		else{
			if( (pDBNpSub->minfo->MoveNext()==-1) || (pDBNpSub->minfo->st==-1) ){
				break;
			}
		}
		exceptMonth[i] = pDBNpSub->minfo->kes_syk_cut;
		
		if( (pDBNpSub->minfo->kes_sgn==4) && (pDBNpSub->minfo->insw==1) ){
			lstKsOfs = pDBNpSub->minfo->ofset;
		}
	}

	// 科目残高の読み込み
	if( pDBNpSub->zvol == NULL ){
		return -1;
	}

	MONY_BUF_SIZE;
	char			codbf[20]={0}, Ascbf[20]={0};
	struct _KN_REC	knrec;
	char			Kzn[MONY_BUF_SIZE]={0}, Kot[MONY_BUF_SIZE]={0}, Kzi[MONY_BUF_SIZE]={0}, Tag[MONY_BUF_SIZE]={0},
					Hka[MONY_BUF_SIZE]={0}, Kmi[MONY_BUF_SIZE]={0}, Kmz[MONY_BUF_SIZE]={0}, Nki[MONY_BUF_SIZE]={0},
					Nkz[MONY_BUF_SIZE]={0}, Bki[MONY_BUF_SIZE]={0}, Bkz[MONY_BUF_SIZE]={0}, Kri[MONY_BUF_SIZE]={0},
					Ksi[MONY_BUF_SIZE]={0}, Val[MONY_BUF_SIZE]={0};
	int				Pct=0, Putno=0;

	CKZREC			*pTmpKzrec = NULL;
	if( (pDBNpSub->zvol->m_type&0xff) != 0x01 ){	// 合併マスター以外
		if( pDBNpSub->xkzrec ){
			pDBNpSub->XKamokuZanClose();
		}
		if( pDBNpSub->XKamokuZanOpen((shinsw+1), _T(""), 0) == -1 ){
			return -1;
		}
		pTmpKzrec = pDBNpSub->xkzrec;
	}
	else{
		if( pDBNpSub->kzrec ){
			pDBNpSub->KamokuZanClose();
		}
		if( pDBNpSub->KamokuZanOpen() == -1 ){
			return -1;
		}
		pTmpKzrec = pDBNpSub->kzrec;
	}

	for( int i=0;; i++ ){
		if( i==0 ){
			if( (pTmpKzrec->MoveFirst()==-1) || (pTmpKzrec->st==-1) ){
				break;
			}
		}
		else{
			if( (pTmpKzrec->MoveNext()==-1) || (pTmpKzrec->st==-1) ){
				break;
			}
		}

		// 大文字変換
		memset( codbf, '\0', sizeof(codbf) );
		memmove( codbf, pTmpKzrec->kzkcd, 8 );
		_strupr_s( codbf, sizeof(codbf) );
		if( pDBNpSub->szvol->SVkcod1.Compare(pTmpKzrec->kzkcd) == 0 ){	// 仮払消費税？
			continue;
		}
		if( pDBNpSub->szvol->SVkcod2.Compare(pTmpKzrec->kzkcd) == 0 ){	// 仮受消費税？
			continue;
		}

		memset( &knrec, '\0', sizeof(struct _KN_REC) );
		memmove( knrec.Kn_cod, pTmpKzrec->kzkcd, 8 );
		if( m_TblHdl.th_acs( thRead, &knrec, &KnTbl ) ){
			continue;
		}

		// 損益科目？
		if( strncmp((char*)knrec.Kn_cod, _T("08"), 2) < 0 ){
			continue;
		}
		if( (knrec.Kn_sz3&0x0f) != 0x00 ){
			continue;
		}

		// 消費税対象外のみ集計
		memset( Kzn, '\0', MONY_BUF_SIZE );
		for( int j=0, k=0; j<16; j++ ){
			// 除外チェック
			switch( j ){
				case 0 :
					k = 0;
					break;
				case 1 :
//-- '16.12.07 --
//					k = 1;
//---------------
					if( lstKsOfs == j ){
						k = 0;
					}
					else{
						k = 1;
					}
//---------------
					break;
				case 2 :
//-- '16.12.07 --
//					k = 2;
//---------------
					if( lstKsOfs == j ){
						k = 1;
					}
					else{
						k = 2;
					}
//---------------
					break;
				case 3 :
					if( (emon!=2) && exceptMonth[j] ){	// 除外チェック
						continue;
					}
					else{
						k = 2;
					}
					break;

				case 4 :
					k = 3;
					break;
				case 5 :
//-- '16.12.07 --
//					k = 4;
//---------------
					if( lstKsOfs == j ){
						k = 3;
					}
					else{
						k = 4;
					}
//---------------
					break;
				case 6 :
//-- '16.12.07 --
//					k = 5;
//---------------
					if( lstKsOfs == j ){
						k = 4;
					}
					else{
						k = 5;
					}
//---------------
					break;
				case 7 : 
					if( (emon!=5) && exceptMonth[j] ){
						continue;			// 除外チェック
					}
					else{
						k = 5;
					}
					break;

				case 8 :
					k = 6;
					break;
				case 9 :
//-- '16.12.07 --
//					k = 7;
//---------------
					if( lstKsOfs == j ){
						k = 6;
					}
					else{
						k = 7;
					}
//---------------
					break;
				case 10:
//-- '16.12.07 --
//					k = 8;
//---------------
					if( lstKsOfs == j ){
						k = 7;
					}
					else{
						k = 8;
					}
//---------------
					break;
				case 11: 
					if( (emon!=8) && exceptMonth[j] ){
						continue;			// 除外チェック
					}
					else{
						k = 8;
					}
					break;

				case 12:
					k = 9;
					break;
				case 13:
//-- '16.12.07 --
//					k =10;
//---------------
					if( lstKsOfs == j ){
						k = 9;
					}
					else{
						k = 10;
					}
//---------------
					break;
				case 14:
//-- '16.12.07 --
//					k =11;
//---------------
					if( lstKsOfs == j ){
						k = 10;
					}
					else{
						k = 11;
					}
//---------------
					break;
				case 15:
					k =11;
					break;
			}
			if( (k<smon) || (k>emon) ){
				continue;
			}

			memset( Kri, '\0', MONY_BUF_SIZE );
			strcpy_s( Ascbf, pTmpKzrec->lhs[j] );
			m_Arith.l_input( Kri, Ascbf );
			memset( Ksi, '\0', MONY_BUF_SIZE );
			strcpy_s( Ascbf, pTmpKzrec->rhs[j] );
			m_Arith.l_input( Ksi, Ascbf );
			memset( Val, '\0', MONY_BUF_SIZE );
//-- '14.10.22 --
//			m_Arith.l_sub( Val, Kri, Ksi );
//---------------
			if( knrec.Kn_att&0x01 ){
				m_Arith.l_sub( Val, Ksi, Kri );
			}
			else{
				m_Arith.l_sub( Val, Kri, Ksi );
			}
//---------------
			m_Arith.l_add( Kzn, Kzn, Val );
		}

		// 決算書項目オフセットの取得
		if( pDBNpSub->zvol->ind_type&0x10 ){
			Pct = knrec.Kn_prf2;		// 白色
		}
		else{
			Pct = knrec.Kn_prf1;		// 青色
		}
		if( (Putno = GetRecodeNo(pDBNpSub->zvol->ind_type, knrec.Kn_ctg, Pct)) == 0 ){
			continue;
		}
		Putno--;

		switch( knrec.Kn_ctg&0x0f ){
			case 0x01 :	// 一般
//				if( (knrec.Kn_att&0x01) && (NRec.KJval[Putno].KKtype&0x01) ){
				if( (!(knrec.Kn_att&0x01) && (NRec.KJval[Putno].KKtype&0x01)) ||
					((knrec.Kn_att&0x01) && !(NRec.KJval[Putno].KKtype&0x01)) ){
					m_Arith.l_neg( Kzn );
				}
				m_Arith.l_add( NRec.KJval[Putno].KKval1, NRec.KJval[Putno].KKval1, Kzn );
				m_Arith.l_add( NRec.KJval[Putno].KKval2, NRec.KJval[Putno].KKval2, Kzn );
				break;

			case 0x02 :	// 不動産									   
//				if( (knrec.Kn_att&0x01) && (NRec.KFval[Putno].KKtype&0x01) ){
				if( (!(knrec.Kn_att&0x01) && (NRec.KFval[Putno].KKtype&0x01)) ||
					((knrec.Kn_att&0x01) && !(NRec.KFval[Putno].KKtype&0x01)) ){
					m_Arith.l_neg( Kzn );
				}
				m_Arith.l_add( NRec.KFval[Putno].KKval1, NRec.KFval[Putno].KKval1, Kzn );
				m_Arith.l_add( NRec.KFval[Putno].KKval2, NRec.KFval[Putno].KKval2, Kzn );
				break;

			case 0x04 :	// 農業		
				if( Putno == 1 ){				// 家事消費等
					if( knrec.Kn_prf3 == 5 ){	// 事業消費等
						Putno = 2;
					}
				}
//				if( (knrec.Kn_att&0x01) && (NRec.KNval[Putno].KKtype&0x01) ){
				if( (!(knrec.Kn_att&0x01) && (NRec.KNval[Putno].KKtype&0x01)) ||
					((knrec.Kn_att&0x01) && !(NRec.KNval[Putno].KKtype&0x01)) ){
					m_Arith.l_neg( Kzn );
				}
				m_Arith.l_add( NRec.KNval[Putno].KKval1, NRec.KNval[Putno].KKval1, Kzn );
				m_Arith.l_add( NRec.KNval[Putno].KKval2, NRec.KNval[Putno].KKval2, Kzn );
				break;
			default   :
				break;
		}
	}

	// 後処理
	if( pDBNpSub->xkzrec ){
		pDBNpSub->XKamokuZanClose();
	}

	// 消費税残高の読み込み
	MoneyBasejagArray	money;
	BOOL				IsKziKmk = TRUE;	// '14.10.21
	BOOL				IsTagKmk = FALSE;	// '15.02.13

	for( int j=0; j<2; j++ ){
		if( j==0 ){
			money = pSyzSyukei->GetShisanData( PR_URIAGE );
		}
		else if( j==1 ){
			money = pSyzSyukei->GetShisanData( PR_SHIIRE );
		}
		else{
			break;
		}

		int	inmax;
		int max = (int)money.GetCount();
		for( int i=0; i<max; i++ ){
			inmax = (int)money[i].GetCount();
			if( inmax == 0 ){
				continue;
			}
			else if( money[i][0].attr.shisan_syukei_type != -1 ){
				// 明細以外は全て飛ばす
				continue;
			}

			if( pDBNpSub->szvol->SVkcod1.Left(6) == money[i][0].code.Left(6) ){			// 仮払消費税？
				continue;
			}
			else if( pDBNpSub->szvol->SVkcod2.Left(6) == money[i][0].code.Left(6) ){	// 仮受消費税？
				continue;
			}

			// 棚卸調整はカット
			if( ((money[i][0].attr.sc_kbn1==0x04)&&(money[i][0].attr.sc_kbn2==0x05)) ||
				((money[i][0].attr.sc_kbn1==0x04)&&(money[i][0].attr.sc_kbn2==0x06)) ){
				continue;
			}

			// 科目名称の読み込み
			memset( &knrec, '\0', sizeof(struct _KN_REC) );
			memmove( knrec.Kn_cod, money[i][0].code, 8 );
			if( m_TblHdl.th_acs( thRead, &knrec, &KnTbl ) ){
//-- '14.10.30 --
//				return -1;
//---------------
				continue;
//---------------
			}

//--> '15.02.13 INS START
			IsTagKmk = FALSE;
			if( (strncmp((char*)knrec.Kn_cod, _T("08"), 2)>=0) && ((knrec.Kn_sz3&0x0f)==0x00) ){
				IsTagKmk = TRUE;
			}
//<-- '15.02.13 INS END

			// 残高集計
			memset( Kzn, '\0', sizeof(Kzn) );
			memset( Kot, '\0', sizeof(Kot) );
			memset( Kzi, '\0', sizeof(Kzi) );
			memset( Tag, '\0', sizeof(Tag) );
			memset( Hka, '\0', sizeof(Hka) );
			memset( Kmi, '\0', sizeof(Kmi) );
			memset( Kmz, '\0', sizeof(Kmz) );
			memset( Nki, '\0', sizeof(Nki) );
			memset( Nkz, '\0', sizeof(Nkz) );
			memset( Bki, '\0', sizeof(Bki) );
			memset( Bkz, '\0', sizeof(Bkz) );
//--> '14.10.21 INS START
			IsKziKmk = TRUE;
//<-- '14.10.21 INS END

/*			for( int j=smon; j<=emon; j++ ){
				m_Arith.l_add( Tag, Tag, &szrec.SZdtbl[Mct].SDval[0] );	// 対象外
				m_Arith.l_add( Hka, Hka, &szrec.SZdtbl[Mct].SDval[1] );	// 非課税
				m_Arith.l_add( Kmi, Kmi, &szrec.SZdtbl[Mct].SDval[3] );	// 込み
				m_Arith.l_add( Kmz, Kmz, &szrec.SZdtbl[Mct].SDval[4] );	// 込み税額
				m_Arith.l_add( Nki, Nki, &szrec.SZdtbl[Mct].SDval[5] );	// 抜き
				m_Arith.l_add( Nkz, Nkz, &szrec.SZdtbl[Mct].SDval[6] );	// 抜き税額
				m_Arith.l_add( Bki, Bki, &szrec.SZdtbl[Mct].SDval[7] );	// 別記
			}*/

			if( money[i][0].attr.shisan_hikazei_hukazei == 0x01 ){
				m_Arith.l_add( Hka, Hka, money[i][2].arith );
			}
			else if( money[i][0].attr.shisan_hikazei_hukazei == 0x02 ){
				m_Arith.l_add( Tag, Tag, money[i][2].arith );
			}
			// 輸入仕入
			else if( (money[i][0].attr.sc_kbn1==0x04) && (money[i][0].attr.sc_kbn2==0x04) ){
				if( shinsw ){
					// 税込
					m_Arith.l_add( Tag, Tag, money[i][2].arith );

//--> '14.10.21 INS START
					if( money[i][0].attr.shisan_beki ){
						if( money[i][0].attr.shisan_zino == 0x00/*5%*/ ){
							m_Util.percent( Bkz, Tag, 2, 50, 0 );
							m_Arith.l_add( Tag, Tag, Bkz );
						}
						else if( money[i][0].attr.shisan_zino == 0x03/*8%*/ ){
							m_Util.percent( Bkz, Tag, 10, 80, 0 );
							m_Arith.l_add( Tag, Tag, Bkz );
						}
					}
//<-- '14.10.21 INS END
				}
				else{
					m_Arith.l_add( Tag, Tag, money[i][0].arith );
				}
//--> '14.10.21 INS START
				IsKziKmk = FALSE;
//<-- '14.10.21 INS END
			}
//--> '15.08.26 INS START
			else if( IsTkkzRelKmk(&money[i][0]) == TRUE ){
				if( (money[i][0].attr.shisan_gp_kubun&0x80) == 0x00 ){
					continue;
				}
				else{
					m_Arith.l_add( Tag, Tag, money[i][0].arith );
					IsKziKmk = FALSE;
				}
			}
//<-- '15.08.26 INS END
			// 課税取引
			else{
				if( shinsw ){
					m_Arith.l_add( Kzi, Kzi, money[i][2].arith );
//--> '14.10.21 INS START
					if( money[i][0].attr.shisan_beki ){
						if( money[i][0].attr.shisan_zino == 0x00/*5%*/ ){
							m_Util.percent( Bkz, Kzi, 2, 50, 0 );
							m_Arith.l_add( Kzi, Kzi, Bkz );
						}
						else if( money[i][0].attr.shisan_zino == 0x03/*8%*/ ){
							m_Util.percent( Bkz, Kzi, 10, 80, 0 );
							m_Arith.l_add( Kzi, Kzi, Bkz );
						}
					}
//<-- '14.10.21 INS END
				}
				else{
					m_Arith.l_add( Kzi, Kzi, money[i][0].arith );
				}
			}

			// 貸倒損失
			if( (money[i][0].attr.sc_kbn1==0x08) && (money[i][0].attr.sc_kbn2==0x03) ){
				memmove( Kot, Kzi, sizeof(Kot) );
				memset( Kzi, '\0', sizeof(Kzi) );
//--> '14.10.23 INS START
				IsKziKmk = FALSE;
//<-- '14.10.23 INS END
			}

//-- '15.08.26 --
//			// 輸出売上及び輸入仕入は対象外
//			if( ((money[i][0].attr.sc_kbn1==0x02)&&(money[i][0].attr.sc_kbn2==0x09)) ||
//				((money[i][0].attr.sc_kbn1==0x04)&&(money[i][0].attr.sc_kbn2==0x04)) ){
//				memmove( Kot, Kzi, sizeof(Kot) );
//				memset( Kzi, '\0', sizeof(Kzi) );
//			}
//---------------
			// 輸出売上及び輸入仕入及び特定課税仕入は対象外
			if( ((money[i][0].attr.sc_kbn1==0x02)&&(money[i][0].attr.sc_kbn2==0x09)) ||
				((money[i][0].attr.sc_kbn1==0x04)&&(money[i][0].attr.sc_kbn2==0x04)) ||
				(IsTkkzRelKmk(&money[i][0])==TRUE) ){
				memmove( Kot, Kzi, sizeof(Kot) );
				memset( Kzi, '\0', sizeof(Kzi) );
			}
//---------------

			// 不課税・非課税取引
			m_Arith.l_add( Kot, Kot, Tag );
			m_Arith.l_add( Kot, Kot, Hka );
			// 決算額
			m_Arith.l_add( Kzn, Kzn, Kot );
			m_Arith.l_add( Kzn, Kzn, Kzi );

			// 貸借科目
			if( strncmp((char*)knrec.Kn_cod, _T("08"), 2) < 0 ){
				if( money[i][0].attr.sc_kbn1 != 0x11 ){	// 固定資産の譲渡又は取得チェック
					continue;		
				}

				// 計算対象
				KKVAL	*pKsKkval = NULL;	// 決算額
				KKVAL	*pTgKkval = NULL;	// 課税取引外
				
				if( money[i][0].attr.sc_sjsg ){	// 譲渡
					if( IsTransitionalMeasure ){						
						pKsKkval = &NRec.KUval[12];
						pTgKkval = &NRec.KUval[13];
					}
					else{
						pKsKkval = &NRec.KUval[12];
						pTgKkval = &NRec.KUval[13];
					}
				}
				else{							// 取得
					if( IsTransitionalMeasure ){
						pKsKkval = &NRec.KSval[12];
						pTgKkval = &NRec.KSval[13];
					}
					else{
						pKsKkval = &NRec.KSval[12];
						pTgKkval = &NRec.KSval[13];
					}
				}

				if( pKsKkval && pTgKkval ){
					m_Arith.l_add( pKsKkval->KKval1, pKsKkval->KKval1, Kzn );
					m_Arith.l_add( pTgKkval->KKval1, pTgKkval->KKval1, Kot );
					if( IsKziKmk ){
						if( money[i][0].attr.sc_zino == 0x00/*5%*/ ){
							m_Arith.l_add( (pKsKkval+2)->KKval2, (pKsKkval+2)->KKval2, Kzn );
						}
						else if( money[i][0].attr.sc_zino == 0x03/*8%*/ ){
							m_Arith.l_add( (pKsKkval+2)->KKval3, (pKsKkval+2)->KKval3, Kzn );
						}
					}
				}
			}
			// 損益科目
			else{
				// 計算対象
				KKVAL	*pKkval = NULL;

				// 集計対象外の売上値引き・販売奨励金収入・仕入割引は収入には含めずに別集計を行なう。
				if( (money[i][0].attr.sc_kbn1==0x02) &&
					((money[i][0].attr.sc_kbn2==0x01)||(money[i][0].attr.sc_kbn2==0x06)||(money[i][0].attr.sc_kbn2==0x07)) ){

					pKkval = NULL;

					switch( knrec.Kn_ctg&0x0f ){
						case 0x01 :	// 一般
							pKkval = &NRec.KJval[33];
							break;
						case 0x02 :	// 不動産
//-- '14.10.22 --
//							pKkval = &NRec.KJval[15];
//---------------
							pKkval = &NRec.KFval[15];
//---------------
							break;
						case 0x04 :	// 農業
//-- '14.10.22 --
//							pKkval = &NRec.KJval[38];
//---------------
							pKkval = &NRec.KNval[38];
//---------------
							break;
						default   :
							break;
					}

					if( pKkval ){
//--> '14,10.20 INS START
						if( (!(knrec.Kn_att&0x01) && (pKkval->KKtype&0x01)) ||
							((knrec.Kn_att&0x01) && !(pKkval->KKtype&0x01)) ){
							m_Arith.l_neg( Kzi );
						}
//<-- '14,10.20 INS END
						m_Arith.l_add( pKkval->KKval1, pKkval->KKval1, Kzi );
						if( IsKziKmk ){
							if( money[i][0].attr.sc_zino == 0x00/*5%*/ ){
								m_Arith.l_add( pKkval->KKval4, pKkval->KKval4, Kzi );
							}
							else if( money[i][0].attr.sc_zino == 0x03/*8%*/ ){
								m_Arith.l_add( pKkval->KKval5, pKkval->KKval5, Kzi );
							}
						}
					}

					continue;
				}

				// 決算書項目オフセットの取得
				int Cnt = 0;
				if( pDBNpSub->zvol->ind_type&0x10 ){	// 白色
					Cnt = knrec.Kn_prf2;
				}
				else{									// 青色
					Cnt = knrec.Kn_prf1;
				}
				if( !(Putno = GetRecodeNo(pDBNpSub->zvol->ind_type, knrec.Kn_ctg, Cnt)) ){
					continue;
				}
				Putno--;

#ifdef	_DEBUG_EX
				TRACE("\nPjis=%-8.8s ctg=%02x no=%3d[%d %d %d]", knrec.Kn_cod, knrec.Kn_ctg, Putno, knrec.Kn_prf1, knrec.Kn_prf2, knrec.Kn_prf3 );
				char	buf[128]={0};
				m_Arith.l_print( buf, Kzn, "SSS,SSS,SSS,SS9" );
				TRACE(" [%s ", buf );
				m_Arith.l_print( buf, Kot, "SSS,SSS,SSS,SS9" );
				TRACE(" %s]", buf );
#endif
				// 初期化
				pKkval = NULL;

				switch( knrec.Kn_ctg&0x0f ){
					case 0x01 :	// 一般
						pKkval = &NRec.KJval[Putno];
						break;

					case 0x02 :	// 不動産									   
						pKkval = &NRec.KFval[Putno];
						break;

					case 0x04 :	// 農業		
						if( Putno == 1 ){				// 家事消費等
							if( knrec.Kn_prf3 == 5 ){	// 事業消費等
								Putno = 2;
							}
						}
						pKkval = &NRec.KNval[Putno];
						break;

					default   :
						break;
				}

				if( pKkval ){
//--> '14,10.20 INS START
					if( (!(knrec.Kn_att&0x01) && (pKkval->KKtype&0x01)) ||
						((knrec.Kn_att&0x01) && !(pKkval->KKtype&0x01)) ){
						m_Arith.l_neg( Kzn );
						m_Arith.l_neg( Kot );
					}
//<-- '14,10.20 INS END
//-- '15.02.13 --
//					m_Arith.l_add( pKkval->KKval1, pKkval->KKval1, Kzn );
//					m_Arith.l_add( pKkval->KKval2, pKkval->KKval2, Kot );
//---------------
					if( IsTagKmk == FALSE ){
						m_Arith.l_add( pKkval->KKval1, pKkval->KKval1, Kzn );
						m_Arith.l_add( pKkval->KKval2, pKkval->KKval2, Kot );
					}
					else{
						m_Arith.l_sub( pKkval->KKval2, pKkval->KKval2, Kzi );
					}
//---------------
					if( IsKziKmk ){
						if( money[i][0].attr.sc_zino == 0x00/*5%*/ ){
							m_Arith.l_add( pKkval->KKval4, pKkval->KKval4, Kzn );
						}
						else if( money[i][0].attr.sc_zino == 0x03/*8%*/ ){
							m_Arith.l_add( pKkval->KKval5, pKkval->KKval5, Kzn );
						}
					}
				}
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 財務連動（'20.12.28）
//-----------------------------------------------------------------------------
// 引数	pDBNpSub				：	財務クラスのポインタ
//		pSyzSyukei				：	消費税集計クラスのポインタ
//		smon					：	開始月
//		emon					：	終了月
//		IsTransitionalMeasure	：	経過措置あり？
//		pSnHeadData				：	消費税ヘッダ情報
//-----------------------------------------------------------------------------
// 返送値	0					：	正常終了
//			-1					：	エラー
//-----------------------------------------------------------------------------
int CH26TaxListData::ConnectZmdata( CDBNpSub *pDBNpSub, CSyzSyukei *pSyzSyukei, int smon, int emon, BOOL IsTransitionalMeasure, CSnHeadData *pSnHeadData )
{
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}
	ASSERT( pSyzSyukei );
	if( pSyzSyukei == NULL ){
		return -1;
	}

	ASSERT ( pSnHeadData );
	if ( pSnHeadData == NULL ) {
		return -1;
	}

	// 科目名称の読み込み
	if( KnTbl.tp_tno == -1 ){
		if( ReadKnTabl(pDBNpSub) ){
			return -1;
		}
	}

	// 集計レコードの初期設定
	for( int i=0; i<50; i++ ){
		// 課税取引金額計算表（事業所得用）
		NRec.KJval[i].KKauto = 0x00;
		memset( NRec.KJval[i].KKval1, '\0', sizeof(NRec.KJval[i].KKval1) );
		memset( NRec.KJval[i].KKval2, '\0', sizeof(NRec.KJval[i].KKval2) );
		memset( NRec.KJval[i].KKval3, '\0', sizeof(NRec.KJval[i].KKval3) );
		memset( NRec.KJval[i].KKval4, '\0', sizeof(NRec.KJval[i].KKval4) );
		memset( NRec.KJval[i].KKval5, '\0', sizeof(NRec.KJval[i].KKval5) );
		// 課税取引金額計算表（不動産取得用）
		NRec.KFval[i].KKauto = 0x00;
		memset( NRec.KFval[i].KKval1, '\0', sizeof(NRec.KFval[i].KKval1) );
		memset( NRec.KFval[i].KKval2, '\0', sizeof(NRec.KFval[i].KKval2) );
		memset( NRec.KFval[i].KKval3, '\0', sizeof(NRec.KFval[i].KKval3) );
		memset( NRec.KFval[i].KKval4, '\0', sizeof(NRec.KFval[i].KKval4) );
		memset( NRec.KFval[i].KKval5, '\0', sizeof(NRec.KFval[i].KKval5) );
		// 課税取引金額計算表（農業得用）
		NRec.KNval[i].KKauto = 0x00;
		memset( NRec.KNval[i].KKval1, '\0', sizeof(NRec.KNval[i].KKval1) );
		memset( NRec.KNval[i].KKval2, '\0', sizeof(NRec.KNval[i].KKval2) );
		memset( NRec.KNval[i].KKval3, '\0', sizeof(NRec.KNval[i].KKval3) );
		memset( NRec.KNval[i].KKval4, '\0', sizeof(NRec.KNval[i].KKval4) );
		memset( NRec.KNval[i].KKval5, '\0', sizeof(NRec.KNval[i].KKval5) );

		//-- '14.09.22 --
		//		if( (i==9) || (i==10) || (i==11) ){
		//			continue;
		//		}
		//---------------
		if( IsTransitionalMeasure ){
			if( (i==KSELLINE7) || (i==KSELLINE8) || (i==KSELLINE9) ){
				continue;
			}
		}
		else{
			if( (i==SELLINE10) || (i==SELLINE11) || (i==SELLINE12) ){
				continue;
			}
		}
		//---------------

		// 課税売上計算表
		NRec.KUval[i].KKauto = 0x00;
		memset( NRec.KUval[i].KKval1, '\0', sizeof(NRec.KUval[i].KKval1) );
		memset( NRec.KUval[i].KKval2, '\0', sizeof(NRec.KUval[i].KKval2) );
		memset( NRec.KUval[i].KKval3, '\0', sizeof(NRec.KUval[i].KKval3) );
		memset( NRec.KUval[i].KKval4, '\0', sizeof(NRec.KUval[i].KKval4) );
		memset( NRec.KUval[i].KKval5, '\0', sizeof(NRec.KUval[i].KKval5) );
		// 課税仕入計算表
		NRec.KSval[i].KKauto = 0x00;
		memset( NRec.KSval[i].KKval1, '\0', sizeof(NRec.KSval[i].KKval1) );
		memset( NRec.KSval[i].KKval2, '\0', sizeof(NRec.KSval[i].KKval2) );
		memset( NRec.KSval[i].KKval3, '\0', sizeof(NRec.KSval[i].KKval3) );
		memset( NRec.KSval[i].KKval4, '\0', sizeof(NRec.KSval[i].KKval4) );
		memset( NRec.KSval[i].KKval5, '\0', sizeof(NRec.KSval[i].KKval5) );
	}

	// 税抜き及び税込みに設定
	int shinsw = 0;
	if( Head.KHstax&0x01 ){
		shinsw = 0;	// 抜き「抜き又は一括税抜き」
	}
	else{
		shinsw = 1;	// 込み「込み」
	}

	// 最終決算修正月
	int		lstKsOfs = -1;
	// 除外月
	char	exceptMonth[20]={0};
	if( pDBNpSub->minfo == NULL ){
		if( pDBNpSub->MinfoOpen() ){
			return -1;
		}
	}
	for( int i=0; i<16; i++ ){
		if( i==0 ){
			if( (pDBNpSub->minfo->MoveFirst()==-1) || (pDBNpSub->minfo->st==-1) ){
				break;
			}
		}
		else{
			if( (pDBNpSub->minfo->MoveNext()==-1) || (pDBNpSub->minfo->st==-1) ){
				break;
			}
		}
		exceptMonth[i] = pDBNpSub->minfo->kes_syk_cut;

		if( (pDBNpSub->minfo->kes_sgn==4) && (pDBNpSub->minfo->insw==1) ){
			lstKsOfs = pDBNpSub->minfo->ofset;
		}
	}

	// 科目残高の読み込み
	if( pDBNpSub->zvol == NULL ){
		return -1;
	}

	MONY_BUF_SIZE;
	char			codbf[20]={0}, Ascbf[20]={0};
	struct _KN_REC	knrec;
	char			Kzn[MONY_BUF_SIZE]={0}, Kot[MONY_BUF_SIZE]={0}, Kzi[MONY_BUF_SIZE]={0}, Tag[MONY_BUF_SIZE]={0},
		Hka[MONY_BUF_SIZE]={0}, Kmi[MONY_BUF_SIZE]={0}, Kmz[MONY_BUF_SIZE]={0}, Nki[MONY_BUF_SIZE]={0},
		Nkz[MONY_BUF_SIZE]={0}, Bki[MONY_BUF_SIZE]={0}, Bkz[MONY_BUF_SIZE]={0}, Kri[MONY_BUF_SIZE]={0},
		Ksi[MONY_BUF_SIZE]={0}, Val[MONY_BUF_SIZE]={0};
	int				Pct=0, Putno=0;

	CKZREC			*pTmpKzrec = NULL;
	if( (pDBNpSub->zvol->m_type&0xff) != 0x01 ){	// 合併マスター以外
		if( pDBNpSub->xkzrec ){
			pDBNpSub->XKamokuZanClose();
		}
		if( pDBNpSub->XKamokuZanOpen((shinsw+1), _T(""), 0) == -1 ){
			return -1;
		}
		pTmpKzrec = pDBNpSub->xkzrec;
	}
	else{
		if( pDBNpSub->kzrec ){
			pDBNpSub->KamokuZanClose();
		}
		if( pDBNpSub->KamokuZanOpen() == -1 ){
			return -1;
		}
		pTmpKzrec = pDBNpSub->kzrec;
	}

	for( int i=0;; i++ ){
		if( i==0 ){
			if( (pTmpKzrec->MoveFirst()==-1) || (pTmpKzrec->st==-1) ){
				break;
			}
		}
		else{
			if( (pTmpKzrec->MoveNext()==-1) || (pTmpKzrec->st==-1) ){
				break;
			}
		}

		// 大文字変換
		memset( codbf, '\0', sizeof(codbf) );
		memmove( codbf, pTmpKzrec->kzkcd, 8 );
		_strupr_s( codbf, sizeof(codbf) );
		if( pDBNpSub->szvol->SVkcod1.Compare(pTmpKzrec->kzkcd) == 0 ){	// 仮払消費税？
			continue;
		}
		if( pDBNpSub->szvol->SVkcod2.Compare(pTmpKzrec->kzkcd) == 0 ){	// 仮受消費税？
			continue;
		}

		memset( &knrec, '\0', sizeof(struct _KN_REC) );
		memmove( knrec.Kn_cod, pTmpKzrec->kzkcd, 8 );
		if( m_TblHdl.th_acs( thRead, &knrec, &KnTbl ) ){
			continue;
		}

		// 損益科目？
		if( strncmp((char*)knrec.Kn_cod, _T("08"), 2) < 0 ){
			continue;
		}
		if( (knrec.Kn_sz3&0x0f) != 0x00 ){
			continue;
		}

		// 消費税対象外のみ集計
		memset( Kzn, '\0', MONY_BUF_SIZE );
		for( int j=0, k=0; j<16; j++ ){
			// 除外チェック
			switch( j ){
			case 0 :
				k = 0;
				break;
			case 1 :
				//-- '16.12.07 --
				//					k = 1;
				//---------------
				if( lstKsOfs == j ){
					k = 0;
				}
				else{
					k = 1;
				}
				//---------------
				break;
			case 2 :
				//-- '16.12.07 --
				//					k = 2;
				//---------------
				if( lstKsOfs == j ){
					k = 1;
				}
				else{
					k = 2;
				}
				//---------------
				break;
			case 3 :
				if( (emon!=2) && exceptMonth[j] ){	// 除外チェック
					continue;
				}
				else{
					k = 2;
				}
				break;

			case 4 :
				k = 3;
				break;
			case 5 :
				//-- '16.12.07 --
				//					k = 4;
				//---------------
				if( lstKsOfs == j ){
					k = 3;
				}
				else{
					k = 4;
				}
				//---------------
				break;
			case 6 :
				//-- '16.12.07 --
				//					k = 5;
				//---------------
				if( lstKsOfs == j ){
					k = 4;
				}
				else{
					k = 5;
				}
				//---------------
				break;
			case 7 : 
				if( (emon!=5) && exceptMonth[j] ){
					continue;			// 除外チェック
				}
				else{
					k = 5;
				}
				break;

			case 8 :
				k = 6;
				break;
			case 9 :
				//-- '16.12.07 --
				//					k = 7;
				//---------------
				if( lstKsOfs == j ){
					k = 6;
				}
				else{
					k = 7;
				}
				//---------------
				break;
			case 10:
				//-- '16.12.07 --
				//					k = 8;
				//---------------
				if( lstKsOfs == j ){
					k = 7;
				}
				else{
					k = 8;
				}
				//---------------
				break;
			case 11: 
				if( (emon!=8) && exceptMonth[j] ){
					continue;			// 除外チェック
				}
				else{
					k = 8;
				}
				break;

			case 12:
				k = 9;
				break;
			case 13:
				//-- '16.12.07 --
				//					k =10;
				//---------------
				if( lstKsOfs == j ){
					k = 9;
				}
				else{
					k = 10;
				}
				//---------------
				break;
			case 14:
				//-- '16.12.07 --
				//					k =11;
				//---------------
				if( lstKsOfs == j ){
					k = 10;
				}
				else{
					k = 11;
				}
				//---------------
				break;
			case 15:
				k =11;
				break;
			}
			if( (k<smon) || (k>emon) ){
				continue;
			}

			memset( Kri, '\0', MONY_BUF_SIZE );
			strcpy_s( Ascbf, pTmpKzrec->lhs[j] );
			m_Arith.l_input( Kri, Ascbf );
			memset( Ksi, '\0', MONY_BUF_SIZE );
			strcpy_s( Ascbf, pTmpKzrec->rhs[j] );
			m_Arith.l_input( Ksi, Ascbf );
			memset( Val, '\0', MONY_BUF_SIZE );
			//-- '14.10.22 --
			//			m_Arith.l_sub( Val, Kri, Ksi );
			//---------------
			if( knrec.Kn_att&0x01 ){
				m_Arith.l_sub( Val, Ksi, Kri );
			}
			else{
				m_Arith.l_sub( Val, Kri, Ksi );
			}
			//---------------
			m_Arith.l_add( Kzn, Kzn, Val );
		}

		// 決算書項目オフセットの取得
		if( pDBNpSub->zvol->ind_type&0x10 ){
			Pct = knrec.Kn_prf2;		// 白色
		}
		else{
			Pct = knrec.Kn_prf1;		// 青色
		}
		if( (Putno = GetRecodeNo(pDBNpSub->zvol->ind_type, knrec.Kn_ctg, Pct)) == 0 ){
			continue;
		}
		Putno--;

		switch( knrec.Kn_ctg&0x0f ){
		case 0x01 :	// 一般
					//				if( (knrec.Kn_att&0x01) && (NRec.KJval[Putno].KKtype&0x01) ){
			if( (!(knrec.Kn_att&0x01) && (NRec.KJval[Putno].KKtype&0x01)) ||
				((knrec.Kn_att&0x01) && !(NRec.KJval[Putno].KKtype&0x01)) ){
				m_Arith.l_neg( Kzn );
			}
			m_Arith.l_add( NRec.KJval[Putno].KKval1, NRec.KJval[Putno].KKval1, Kzn );
			m_Arith.l_add( NRec.KJval[Putno].KKval2, NRec.KJval[Putno].KKval2, Kzn );
			break;

		case 0x02 :	// 不動産									   
					//				if( (knrec.Kn_att&0x01) && (NRec.KFval[Putno].KKtype&0x01) ){
			if( (!(knrec.Kn_att&0x01) && (NRec.KFval[Putno].KKtype&0x01)) ||
				((knrec.Kn_att&0x01) && !(NRec.KFval[Putno].KKtype&0x01)) ){
				m_Arith.l_neg( Kzn );
			}
			m_Arith.l_add( NRec.KFval[Putno].KKval1, NRec.KFval[Putno].KKval1, Kzn );
			m_Arith.l_add( NRec.KFval[Putno].KKval2, NRec.KFval[Putno].KKval2, Kzn );
			break;

		case 0x04 :	// 農業		
			if( Putno == 1 ){				// 家事消費等
				if( knrec.Kn_prf3 == 5 ){	// 事業消費等
					Putno = 2;
				}
			}
			//				if( (knrec.Kn_att&0x01) && (NRec.KNval[Putno].KKtype&0x01) ){
			if( (!(knrec.Kn_att&0x01) && (NRec.KNval[Putno].KKtype&0x01)) ||
				((knrec.Kn_att&0x01) && !(NRec.KNval[Putno].KKtype&0x01)) ){
				m_Arith.l_neg( Kzn );
			}
			m_Arith.l_add( NRec.KNval[Putno].KKval1, NRec.KNval[Putno].KKval1, Kzn );
			m_Arith.l_add( NRec.KNval[Putno].KKval2, NRec.KNval[Putno].KKval2, Kzn );
			break;
		default   :
			break;
		}
	}

	// 後処理
	if( pDBNpSub->xkzrec ){
		pDBNpSub->XKamokuZanClose();
	}

	// 消費税残高の読み込み
	MoneyBasejagArray	money;
	BOOL				IsKziKmk = TRUE;	// '14.10.21
	BOOL				IsTagKmk = FALSE;	// '15.02.13

	for( int j=0; j<2; j++ ){
		if( j==0 ){
			money = pSyzSyukei->GetShisanData( PR_URIAGE );
		}
		else if( j==1 ){
			money = pSyzSyukei->GetShisanData( PR_SHIIRE );
		}
		else{
			break;
		}

		int	inmax;
		int max = (int)money.GetCount();
		for( int i=0; i<max; i++ ){
			inmax = (int)money[i].GetCount();
			if( inmax == 0 ){
				continue;
			}
			else if( money[i][0].attr.shisan_syukei_type != -1 ){
				// 明細以外は全て飛ばす
				continue;
			}

			if( pDBNpSub->szvol->SVkcod1.Left(6) == money[i][0].code.Left(6) ){			// 仮払消費税？
				continue;
			}
			else if( pDBNpSub->szvol->SVkcod2.Left(6) == money[i][0].code.Left(6) ){	// 仮受消費税？
				continue;
			}

			// 棚卸調整はカット
			if( ((money[i][0].attr.sc_kbn1==0x04)&&(money[i][0].attr.sc_kbn2==0x05)) ||
				((money[i][0].attr.sc_kbn1==0x04)&&(money[i][0].attr.sc_kbn2==0x06)) ){
				continue;
			}

			// 科目名称の読み込み
			memset( &knrec, '\0', sizeof(struct _KN_REC) );
			memmove( knrec.Kn_cod, money[i][0].code, 8 );
			if( m_TblHdl.th_acs( thRead, &knrec, &KnTbl ) ){
				//-- '14.10.30 --
				//				return -1;
				//---------------
				continue;
				//---------------
			}

			//--> '15.02.13 INS START
			IsTagKmk = FALSE;
			if( (strncmp((char*)knrec.Kn_cod, _T("08"), 2)>=0) && ((knrec.Kn_sz3&0x0f)==0x00) ){
				IsTagKmk = TRUE;
			}
			//<-- '15.02.13 INS END

			// 残高集計
			memset( Kzn, '\0', sizeof(Kzn) );
			memset( Kot, '\0', sizeof(Kot) );
			memset( Kzi, '\0', sizeof(Kzi) );
			memset( Tag, '\0', sizeof(Tag) );
			memset( Hka, '\0', sizeof(Hka) );
			memset( Kmi, '\0', sizeof(Kmi) );
			memset( Kmz, '\0', sizeof(Kmz) );
			memset( Nki, '\0', sizeof(Nki) );
			memset( Nkz, '\0', sizeof(Nkz) );
			memset( Bki, '\0', sizeof(Bki) );
			memset( Bkz, '\0', sizeof(Bkz) );
			//--> '14.10.21 INS START
			IsKziKmk = TRUE;
			//<-- '14.10.21 INS END

			/*			for( int j=smon; j<=emon; j++ ){
			m_Arith.l_add( Tag, Tag, &szrec.SZdtbl[Mct].SDval[0] );	// 対象外
			m_Arith.l_add( Hka, Hka, &szrec.SZdtbl[Mct].SDval[1] );	// 非課税
			m_Arith.l_add( Kmi, Kmi, &szrec.SZdtbl[Mct].SDval[3] );	// 込み
			m_Arith.l_add( Kmz, Kmz, &szrec.SZdtbl[Mct].SDval[4] );	// 込み税額
			m_Arith.l_add( Nki, Nki, &szrec.SZdtbl[Mct].SDval[5] );	// 抜き
			m_Arith.l_add( Nkz, Nkz, &szrec.SZdtbl[Mct].SDval[6] );	// 抜き税額
			m_Arith.l_add( Bki, Bki, &szrec.SZdtbl[Mct].SDval[7] );	// 別記
			}*/

			if( money[i][0].attr.shisan_hikazei_hukazei == 0x01 ){
				m_Arith.l_add( Hka, Hka, money[i][2].arith );
			}
			else if( money[i][0].attr.shisan_hikazei_hukazei == 0x02 ){
				m_Arith.l_add( Tag, Tag, money[i][2].arith );
			}
			// 輸入仕入
			else if( (money[i][0].attr.sc_kbn1==0x04) && (money[i][0].attr.sc_kbn2==0x04) ){
				if( shinsw ){
					// 税込
					m_Arith.l_add( Tag, Tag, money[i][2].arith );

					//--> '14.10.21 INS START
					if( money[i][0].attr.shisan_beki ){
						if( money[i][0].attr.shisan_zino == 0x00/*5%*/ ){
							m_Util.percent( Bkz, Tag, 2, 50, 0 );
							m_Arith.l_add( Tag, Tag, Bkz );
						}
						else if( money[i][0].attr.shisan_zino == 0x03/*8%*/ ){
							m_Util.percent( Bkz, Tag, 10, 80, 0 );
							m_Arith.l_add( Tag, Tag, Bkz );
						}
					}
					//<-- '14.10.21 INS END
				}
				else{
					m_Arith.l_add( Tag, Tag, money[i][0].arith );
				}
				//--> '14.10.21 INS START
				IsKziKmk = FALSE;
				//<-- '14.10.21 INS END
			}
			//--> '15.08.26 INS START
			else if( IsTkkzRelKmk(&money[i][0]) == TRUE ){
				if( (money[i][0].attr.shisan_gp_kubun&0x80) == 0x00 ){
					continue;
				}
				else{
					m_Arith.l_add( Tag, Tag, money[i][0].arith );
					IsKziKmk = FALSE;
				}
			}
			//<-- '15.08.26 INS END
			// 課税取引
			else{
				if( shinsw ){
					m_Arith.l_add( Kzi, Kzi, money[i][2].arith );
					// 簡易計算がONの場合には税込みの金額を受け取っているため集計しない
					if ( ( pSnHeadData->Sn_SPECIAL & 0x01 ) == 0x00 ) {
						//--> '14.10.21 INS START
						if( money[i][0].attr.shisan_beki ){
							if( money[i][0].attr.shisan_zino == 0x00/*5%*/ ){
								m_Util.percent( Bkz, Kzi, 2, 50, 0 );
								m_Arith.l_add( Kzi, Kzi, Bkz );
							}
							else if( money[i][0].attr.shisan_zino == 0x03/*8%*/ ){
								m_Util.percent( Bkz, Kzi, 10, 80, 0 );
								m_Arith.l_add( Kzi, Kzi, Bkz );
							}
						}
					}
					//<-- '14.10.21 INS END
				}
				else{
					m_Arith.l_add( Kzi, Kzi, money[i][0].arith );
				}
			}

			// 貸倒損失
			if( (money[i][0].attr.sc_kbn1==0x08) && (money[i][0].attr.sc_kbn2==0x03) ){
				memmove( Kot, Kzi, sizeof(Kot) );
				memset( Kzi, '\0', sizeof(Kzi) );
				//--> '14.10.23 INS START
				IsKziKmk = FALSE;
				//<-- '14.10.23 INS END
			}

			//-- '15.08.26 --
			//			// 輸出売上及び輸入仕入は対象外
			//			if( ((money[i][0].attr.sc_kbn1==0x02)&&(money[i][0].attr.sc_kbn2==0x09)) ||
			//				((money[i][0].attr.sc_kbn1==0x04)&&(money[i][0].attr.sc_kbn2==0x04)) ){
			//				memmove( Kot, Kzi, sizeof(Kot) );
			//				memset( Kzi, '\0', sizeof(Kzi) );
			//			}
			//---------------
			// 輸出売上及び輸入仕入及び特定課税仕入は対象外
			if( ((money[i][0].attr.sc_kbn1==0x02)&&(money[i][0].attr.sc_kbn2==0x09)) ||
				((money[i][0].attr.sc_kbn1==0x04)&&(money[i][0].attr.sc_kbn2==0x04)) ||
				(IsTkkzRelKmk(&money[i][0])==TRUE) ){
				memmove( Kot, Kzi, sizeof(Kot) );
				memset( Kzi, '\0', sizeof(Kzi) );
			}
			//---------------

			// 不課税・非課税取引
			m_Arith.l_add( Kot, Kot, Tag );
			m_Arith.l_add( Kot, Kot, Hka );
			// 決算額
			m_Arith.l_add( Kzn, Kzn, Kot );
			m_Arith.l_add( Kzn, Kzn, Kzi );

			// 貸借科目
			if( strncmp((char*)knrec.Kn_cod, _T("08"), 2) < 0 ){
				if( money[i][0].attr.sc_kbn1 != 0x11 ){	// 固定資産の譲渡又は取得チェック
					continue;		
				}

				// 計算対象
				KKVAL	*pKsKkval = NULL;	// 決算額
				KKVAL	*pTgKkval = NULL;	// 課税取引外

				if( money[i][0].attr.sc_sjsg ){	// 譲渡
					if( IsTransitionalMeasure ){						
						pKsKkval = &NRec.KUval[12];
						pTgKkval = &NRec.KUval[13];
					}
					else{
						pKsKkval = &NRec.KUval[12];
						pTgKkval = &NRec.KUval[13];
					}
				}
				else{							// 取得
					if( IsTransitionalMeasure ){
						pKsKkval = &NRec.KSval[12];
						pTgKkval = &NRec.KSval[13];
					}
					else{
						pKsKkval = &NRec.KSval[12];
						pTgKkval = &NRec.KSval[13];
					}
				}

				if( pKsKkval && pTgKkval ){
					m_Arith.l_add( pKsKkval->KKval1, pKsKkval->KKval1, Kzn );
					m_Arith.l_add( pTgKkval->KKval1, pTgKkval->KKval1, Kot );
					if( IsKziKmk ){
						if( money[i][0].attr.sc_zino == 0x00/*5%*/ ){
							m_Arith.l_add( (pKsKkval+2)->KKval2, (pKsKkval+2)->KKval2, Kzn );
						}
						else if( money[i][0].attr.sc_zino == 0x03/*8%*/ ){
							m_Arith.l_add( (pKsKkval+2)->KKval3, (pKsKkval+2)->KKval3, Kzn );
						}
					}
				}
			}
			// 損益科目
			else{
				// 計算対象
				KKVAL	*pKkval = NULL;

				// 集計対象外の売上値引き・販売奨励金収入・仕入割引は収入には含めずに別集計を行なう。
				if( (money[i][0].attr.sc_kbn1==0x02) &&
					((money[i][0].attr.sc_kbn2==0x01)||(money[i][0].attr.sc_kbn2==0x06)||(money[i][0].attr.sc_kbn2==0x07)) ){

					pKkval = NULL;

					switch( knrec.Kn_ctg&0x0f ){
					case 0x01 :	// 一般
						pKkval = &NRec.KJval[33];
						break;
					case 0x02 :	// 不動産
								//-- '14.10.22 --
								//							pKkval = &NRec.KJval[15];
								//---------------
						pKkval = &NRec.KFval[15];
						//---------------
						break;
					case 0x04 :	// 農業
								//-- '14.10.22 --
								//							pKkval = &NRec.KJval[38];
								//---------------
						pKkval = &NRec.KNval[38];
						//---------------
						break;
					default   :
						break;
					}

					if( pKkval ){
						//--> '14,10.20 INS START
						if( (!(knrec.Kn_att&0x01) && (pKkval->KKtype&0x01)) ||
							((knrec.Kn_att&0x01) && !(pKkval->KKtype&0x01)) ){
							m_Arith.l_neg( Kzi );
						}
						//<-- '14,10.20 INS END
						m_Arith.l_add( pKkval->KKval1, pKkval->KKval1, Kzi );
						if( IsKziKmk ){
							if( money[i][0].attr.sc_zino == 0x00/*5%*/ ){
								m_Arith.l_add( pKkval->KKval4, pKkval->KKval4, Kzi );
							}
							else if( money[i][0].attr.sc_zino == 0x03/*8%*/ ){
								m_Arith.l_add( pKkval->KKval5, pKkval->KKval5, Kzi );
							}
						}
					}

					continue;
				}

				// 決算書項目オフセットの取得
				int Cnt = 0;
				if( pDBNpSub->zvol->ind_type&0x10 ){	// 白色
					Cnt = knrec.Kn_prf2;
				}
				else{									// 青色
					Cnt = knrec.Kn_prf1;
				}
				if( !(Putno = GetRecodeNo(pDBNpSub->zvol->ind_type, knrec.Kn_ctg, Cnt)) ){
					continue;
				}
				Putno--;

#ifdef	_DEBUG_EX
				TRACE("\nPjis=%-8.8s ctg=%02x no=%3d[%d %d %d]", knrec.Kn_cod, knrec.Kn_ctg, Putno, knrec.Kn_prf1, knrec.Kn_prf2, knrec.Kn_prf3 );
				char	buf[128]={0};
				m_Arith.l_print( buf, Kzn, "SSS,SSS,SSS,SS9" );
				TRACE(" [%s ", buf );
				m_Arith.l_print( buf, Kot, "SSS,SSS,SSS,SS9" );
				TRACE(" %s]", buf );
#endif
				// 初期化
				pKkval = NULL;

				switch( knrec.Kn_ctg&0x0f ){
				case 0x01 :	// 一般
					pKkval = &NRec.KJval[Putno];
					break;

				case 0x02 :	// 不動産									   
					pKkval = &NRec.KFval[Putno];
					break;

				case 0x04 :	// 農業		
					if( Putno == 1 ){				// 家事消費等
						if( knrec.Kn_prf3 == 5 ){	// 事業消費等
							Putno = 2;
						}
					}
					pKkval = &NRec.KNval[Putno];
					break;

				default   :
					break;
				}

				if( pKkval ){
					//--> '14,10.20 INS START
					if( (!(knrec.Kn_att&0x01) && (pKkval->KKtype&0x01)) ||
						((knrec.Kn_att&0x01) && !(pKkval->KKtype&0x01)) ){
						m_Arith.l_neg( Kzn );
						m_Arith.l_neg( Kot );
					}
					//<-- '14,10.20 INS END
					//-- '15.02.13 --
					//					m_Arith.l_add( pKkval->KKval1, pKkval->KKval1, Kzn );
					//					m_Arith.l_add( pKkval->KKval2, pKkval->KKval2, Kot );
					//---------------
					if( IsTagKmk == FALSE ){
						m_Arith.l_add( pKkval->KKval1, pKkval->KKval1, Kzn );
						m_Arith.l_add( pKkval->KKval2, pKkval->KKval2, Kot );
					}
					else{
						m_Arith.l_sub( pKkval->KKval2, pKkval->KKval2, Kzi );
					}
					//---------------
					if( IsKziKmk ){
						if( money[i][0].attr.sc_zino == 0x00/*5%*/ ){
							m_Arith.l_add( pKkval->KKval4, pKkval->KKval4, Kzn );
						}
						else if( money[i][0].attr.sc_zino == 0x03/*8%*/ ){
							m_Arith.l_add( pKkval->KKval5, pKkval->KKval5, Kzn );
						}
					}
				}
			}
		}
	}

	return 0;
}


//-----------------------------------------------------------------------------
// 書き込みオフセットの取得('14.08.08)
//-----------------------------------------------------------------------------
// 引数	type	：	個人業種サイン(財務マスター)
//		ctg		：	個人業種サイン(科目)
//		cnt		：	決算書項目番号
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int CH26TaxListData::GetRecodeNo( char type, char ctg, int cnt )
{
	int				Ans;
	struct	BW_SYS	*pBwSys;
	
	Ans = 0;
	if( type&0x10 ){
		switch( ctg&0x0f ){
			case 0x01 : // 一般
				pBwSys = WmTbl;
				break;
			case 0x02 : // 不動産
				pBwSys = WfTbl;
				break;
			case 0x04 : // 農業
				pBwSys = WnTbl;
				break;
			default   :
				return 0;
				break;
		}
		Ans = (pBwSys+(cnt-1))->Kc_Cnv;
	}
	else{
		switch( ctg&0x0f ){
			case 0x01 : // 一般
				pBwSys = BmTbl;
				break;
			case 0x02 : // 不動産
				pBwSys = BfTbl;
				break;
			case 0x04 : // 農業
				pBwSys = BnTbl;
				break;
			default   :
				return 0;
				break;
		}
		Ans = (pBwSys+(cnt-1))->Kc_Put;
	}

	return Ans;
}

//-----------------------------------------------------------------------------
// 科目名称の読み込み('14.08.08)
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラスのポインタ
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CH26TaxListData::ReadKnTabl( CDBNpSub *pDBNpSub )
{
	ASSERT( pDBNpSub );
	if( pDBNpSub == NULL ){
		return -1;
	}

	if( pDBNpSub->knrec == NULL ){
		if( pDBNpSub->KamokuMeisyoOpen() ){
			return -1;
		}
	}

	int				thd;
	unsigned int	rsiz, rcnt;
	unsigned long	tsiz;
	struct _KN_REC	Knrec;

	rsiz = (unsigned int)sizeof( struct _KN_REC );						// レコード長
	rcnt = (unsigned int)(pDBNpSub->knrec->GetRecordCount() + 10);		// レコード数
	tsiz = (unsigned long)rsiz * (unsigned long)rcnt;

	// テーブル イニシャライズ
	if( (thd=m_TblHdl.th_open(tsiz, rsiz)) < 0 ){
		return -1;
	}
	memset( &KnTbl, '\0', sizeof(struct TB_PAR) );	
	KnTbl.tp_tno  = thd;
	KnTbl.tp_rmax = rcnt;
	KnTbl.tp_rnum = 0;
	KnTbl.tp_rlen = rsiz;
	KnTbl.tp_koff = 0;
	KnTbl.tp_klen = 8;
	KnTbl.tp_dupm = 0;
	KnTbl.tp_sufm = 0;
	KnTbl.tp_cp   = 0;
	KnTbl.tp_err  = 0;

	for( int i=0;; i++ ){
		if( i==0 ){
			if( (pDBNpSub->knrec->MoveFirst()==-1) || (pDBNpSub->knrec->st==-1) ){
				break;
			}
		}
		else{
			if( (pDBNpSub->knrec->MoveNext()==-1) || (pDBNpSub->knrec->st==-1) ){
				break;
			}
		}

		if( pDBNpSub->knrec->knvoid == 1 ){
			continue;
		}
		memset( &Knrec, '\0', sizeof(struct _KN_REC) );
		memset( Knrec.Kn_cod, '0', sizeof(Knrec.Kn_cod) );
		memmove( Knrec.Kn_cod, pDBNpSub->knrec->kncod, 8 );
		Knrec.Kn_att = pDBNpSub->knrec->knatt;			// 科目属性（貸借サイン）
		Knrec.Kn_sz1 = pDBNpSub->knrec->knsgn[0];		// 消費税処理グループ区分
		Knrec.Kn_sz2 = pDBNpSub->knrec->knsgn[1];		// 消費税処理グループ区分明細
		Knrec.Kn_sz3 = pDBNpSub->knrec->knsgn[2];		// 消費税属性／自動分離
		Knrec.Kn_sz4 = pDBNpSub->knrec->knsgn[3];		// 消費税課税区分
		Knrec.Kn_sz5 = pDBNpSub->knrec->knsgn[4];		// 消費税仕入区分
		Knrec.Kn_ctg = pDBNpSub->knrec->ind_ctg;		// 業種番号
		Knrec.Kn_prf1 = pDBNpSub->knrec->ind_prf1;		// 青色項目番号
		Knrec.Kn_prf2 = pDBNpSub->knrec->ind_prf2;		// 白色項目番号
		Knrec.Kn_prf3 = pDBNpSub->knrec->ind_prf3;		// 月別
		if( m_TblHdl.th_add(&Knrec, &KnTbl) ){
			return -1;
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 特定課税仕入関連の科目？('15.08.26)
//-----------------------------------------------------------------------------
// 引数		pMbase	：	チェック対象
//-----------------------------------------------------------------------------
// 返送値	TRUE	：	特定課税仕入関連科目
//			FALSE	：	特定課税仕入関連科目でない
//-----------------------------------------------------------------------------
BOOL CH26TaxListData::IsTkkzRelKmk( MoneyBase* pMbase )
{
	BOOL	bRt = FALSE;

	ASSERT( pMbase );
	if( pMbase == NULL ){
		return bRt;
	}

	if( ((pMbase->attr.sc_kbn1==0x04)&&(pMbase->attr.sc_kbn2==0x0b)) ||
		((pMbase->attr.sc_kbn1==0x04)&&(pMbase->attr.sc_kbn2==0x0c)) ||
		((pMbase->attr.sc_kbn1==0x04)&&(pMbase->attr.sc_kbn2==0x0d)) ||
		((pMbase->attr.sc_kbn1==0x04)&&(pMbase->attr.sc_kbn2==0x0e)) ){
		bRt = TRUE;
	}

	return bRt;
}

//2016.06.22 INSERT START
/////////////////////////////////////////////////////////
//処理概要：個人決算書名称変更の名称を取得
//
//引数　　：type	業種区分　一般…1 不動産…2　農業…4
//　　　　　no		行no
//返送値　：0…正常終了 -1…異常終了
/////////////////////////////////////////////////////////
int CH26TaxListData::GetKoketsuMeisyo( CDBNpSub *pDBNpSub, int type, int no, CString &name )
{
	pDBNpSub->kprf->MoveFirst();

	CString tmp,prf_no_tmp;
	tmp.Format("%d",no);


	while(1){
		//if(( pDBNpSub->kprf->prf_type == type )&&( pDBNpSub->kprf->prf_no == no )&&( pDBNpSub->kprf->sgn2 == 0 )){
		prf_no_tmp = pDBNpSub->kprf->prf_no;
		prf_no_tmp.Trim();
		if(( pDBNpSub->kprf->prf_type == type )&&( strcmp(prf_no_tmp, tmp) == 0 )&&( pDBNpSub->kprf->sgn2 == 0 )){
			name = pDBNpSub->kprf->prf_name;
			break;
		}else{
			if( pDBNpSub->kprf->MoveNext() == -1 ){
				return -1;
			}
		}
	}
	
	return 0;
}

//-----------------------------------------------------------------------------
// 個人決算書連動？('16.12.07)
//-----------------------------------------------------------------------------
// 返送値	true	：	連動
//			false	：	連動していない
//-----------------------------------------------------------------------------
bool CH26TaxListData::IsLinkKjprfname()
{
	if( Head.KHkoke == 1 ){
		return true;
	}
	else{
		return false;
	}
}