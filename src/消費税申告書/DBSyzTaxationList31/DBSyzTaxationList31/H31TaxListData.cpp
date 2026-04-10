
#include "stdafx.h"
#include "H31TaxListData.h"
#include "DBShinListInt.h"

#include "ShinSell31Idx.h"
// kasai add -->
#include "ShinSell31ExIdx.h"
// kasai add <--
// tanaka add -->
#include "ShinSell31Ex2Idx.h"
// tanaka add <--
//#include "ShinSellIdx.h"		// '14.09.22

// 23/10/11 higuchi add -->
#include <l_dotnet.h>
// 23/10/11 higuchi add <--

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CH31TaxListData::CH31TaxListData()
{
	m_Arith.l_defn( 0x16 );

	memset( &KnTbl, '\0', sizeof(TB_PAR) );
	KnTbl.tp_tno = -1;
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CH31TaxListData::~CH31TaxListData()
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
int CH31TaxListData::GetData( CDBNpSub *pDBNpSub, CSnHeadData *pSnHeadData, int sn_seq )
{
	CString			filter;
	CDBSyzAccess*	pDBSyzAcc;

// 230718Aw -->
	BOOL			syzSgn = FALSE;
	long			eymd    = 0;

	if(!(pSnHeadData->Sn_SKKBN % 2)) {
		eymd = pSnHeadData->Sn_MDAYE;
	}
	else {
		eymd = pSnHeadData->Sn_KDAYE;
	}
	// 課税期間が2023/12/31以降の場合、GetData2()へ移行
	//　23.10.10課税取引金額計算表の制御を12/31→10/01切り替えに
	//if(eymd >= TAX_R05_EDAY) {
	if (eymd >= ICS_SH_INVOICE_DAY) {
		if(GetData2(pDBNpSub, pSnHeadData, sn_seq)) {
			return -1;
		}
		else {
			return 0;
		}
	}
// 230718Aw <--

	// 消費税拡張処理クラス
	pDBSyzAcc = NULL;
	pDBSyzAcc = new CDBSyzAccess;
	if( pDBSyzAcc == NULL ){
//		errmes = _T("課税取引金額計算表マスターが取得出来ませんでした。");
		return ERR;
	}
	// オープン
	filter.Format( _T("kssgn5 = %d and kslist >= 101 and kslist <= 106"), sn_seq );//フィルタ追加 kasai
	if( pDBSyzAcc->KjsnkOpen( pDBNpSub->m_database, filter ) == ERR ) {
//		errmes = _T("課税取引金額計算表マスターのオープンが出来ませんでした。");
		delete pDBSyzAcc;
		return ERR;
	}

	memset( &Head, '\0', sizeof( KJNHEAD ) );
	memset( &NRec, '\0', sizeof( KJNREC ) );
	char	yy=0, mm=0, dd=0;
	pSnHeadData->GetYmdDataGen( ID_ICSSH_KESSAN_TO, &yy, &mm, &dd );
	YEAR = yy;

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
			// 令和5年1月1日以降の課税期間の場合は、会社登録の経理方式の設定に従う
			long symd = 0, eymd = 0;
			GetKazeiKikan( pSnHeadData, symd, eymd );
			if( symd >= R05_SDAY ) {
				if( pDBNpSub->zvol->s_sgn1 & 0xc0 ){
					// 税抜き
					pSnHeadData->SVsign &= 0xdf;
					Head.KHstax &= 0xfe;
					Head.KHstax |= 0x01;
				}
				else {
					// 税込み
					pSnHeadData->SVsign &= 0xdf;
					pSnHeadData->SVsign |= 0x20;
					Head.KHstax &= 0xfe;
				}
			}
			else {
				// メンテ
				if( pSnHeadData->Sn_Sign2&0x08 ){
					Head.KHstax = 0x01;
				}
				else{
					Head.KHstax = 0x00;
				}
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
			case 101 :	pKkval = &NRec.KJval[pDBSyzAcc->m_pKjsnk->ksline-1];	break;
			case 102 :	pKkval = &NRec.KFval[pDBSyzAcc->m_pKjsnk->ksline-1];	break;
			case 103 :	pKkval = &NRec.KNval[pDBSyzAcc->m_pKjsnk->ksline-1];	break;
			case 104 :	pKkval = &NRec.KUval[pDBSyzAcc->m_pKjsnk->ksline-1];	break;
			case 105 :	pKkval = &NRec.KSval[pDBSyzAcc->m_pKjsnk->ksline-1];	break;
			case 106 :	pKkval = &NRec.KUval[pDBSyzAcc->m_pKjsnk->ksline-1];	break;	// 事業別課税売上高 kasai
			default:	flg = 1;	break;
		}
		if( flg )	continue;
		// 変換
		pKkval->KKtype = pDBSyzAcc->m_pKjsnk->kssgn1;
		pKkval->KKsign = pDBSyzAcc->m_pKjsnk->kssgn2;
		pKkval->KKauto = pDBSyzAcc->m_pKjsnk->kssgn3;
		switch( pDBSyzAcc->m_pKjsnk->kslist )	{
			case 104 :	if( pDBSyzAcc->m_pKjsnk->ksline == 1 )	{
							m_Util.cstring_to_char( (unsigned char *)&Head.KHkunm, pDBSyzAcc->m_pKjsnk->ksname, 10 );
						}
						break;
			case 105 :	if( pDBSyzAcc->m_pKjsnk->ksline == 1 )	{
							m_Util.cstring_to_char( (unsigned char *)&Head.KHksnm, pDBSyzAcc->m_pKjsnk->ksname, 10 );
						}
						break;
			default:
						m_Util.cstring_to_char( (unsigned char *)&pKkval->KKnam, pDBSyzAcc->m_pKjsnk->ksname, 40 );
						break;
		}

		if( (pDBSyzAcc->m_pKjsnk->kslist == 106) && (pDBSyzAcc->m_pKjsnk->ksitem == 1) ){	// 事業別課税売上高　うち税率分の金額
			m_Util.val_to_bin( (unsigned char*)pKkval->KKval5,	pDBSyzAcc->m_pKjsnk->ksval0 );	// 7.8
			m_Util.val_to_bin( (unsigned char*)pKkval->KKval6,	pDBSyzAcc->m_pKjsnk->ksval1 );	// 7.8
			m_Util.val_to_bin( (unsigned char*)pKkval->KKval7,	pDBSyzAcc->m_pKjsnk->ksval2 );	// 6.24
			m_Util.val_to_bin( (unsigned char*)pKkval->KKval8,	pDBSyzAcc->m_pKjsnk->ksval3 );	// 6.24	
			m_Util.val_to_bin( (unsigned char*)pKkval->KKval9,	pDBSyzAcc->m_pKjsnk->ksval4 );	// 6.3
			m_Util.val_to_bin( (unsigned char*)pKkval->KKval10,	pDBSyzAcc->m_pKjsnk->ksval5 );	// 6.3
		}
		else {
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

// 230718Aw -->
//-----------------------------------------------------------------------------
// DB からデータ読込み(SYZ_SNK_LST_VALから読込み)
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラスのポインタ
//		pSnHeadData	：	消費税ヘッダ情報
//		sn_seq		：	対象の履歴シーケンス
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CH31TaxListData::GetData2(CDBNpSub *pDBNpSub, CSnHeadData *pSnHeadData, int sn_seq)
{
	int				ii;
	int				nowVersion = 2;
	int				Rct;
	int				Count;
	char			yy=0, mm=0, dd=0;
	CString			filter = "";
	CDBSyzAccess*	pDBSyzAcc;

	// 消費税拡張処理クラス
	pDBSyzAcc = NULL;
	pDBSyzAcc = new CDBSyzAccess;
	if(pDBSyzAcc == NULL) {
		return ERR;
	}

	memset(&Head, '\0', sizeof(KJNHEAD));
	memset(&NRec, '\0', sizeof(KJNREC));

	// 決算期間(至)の年月日を取得
	pSnHeadData->GetYmdDataGen(ID_ICSSH_KESSAN_TO, &yy, &mm, &dd);
	YEAR = yy;

	// 業種区分
	if(!(GKBN = pDBNpSub->zvol->ind_type)) {
		GKBN = 0x07;
	}

//static char		*ICS_TAXLIST_01_HYOID	=	_T("SHE020");	// 課税取引金額計算表(事業所得用)
//static char		*ICS_TAXLIST_02_HYOID	=	_T("SHE100");	// 課税取引金額計算表(農業所得用)
//static char		*ICS_TAXLIST_03_HYOID	=	_T("SHE040");	// 課税取引金額計算表(不動産所得用)
//static char		*ICS_TAXLIST_04_HYOID	=	_T("SHE060");	// 課税売上高計算表
//static char		*ICS_TAXLIST_05_HYOID	=	_T("SHE080");	// 課税仕入高計算表

	filter.Format("sn_seq = %d and version = %d and (", sn_seq, nowVersion);
// 230728Aw del -->
	//filter.Format(filter + _T("hyoId = \'%s\' or "), ICS_TAXLIST_01_HYOID);	// 課税取引金額計算表(事業所得用)
	//filter.Format(filter + _T("hyoId = \'%s\' or "), ICS_TAXLIST_02_HYOID);	// 課税取引金額計算表(農業所得用)
	//filter.Format(filter + _T("hyoId = \'%s\' or "), ICS_TAXLIST_03_HYOID);	// 課税取引金額計算表(不動産所得用)
	//filter.Format(filter + _T("hyoId = \'%s\' or "), ICS_TAXLIST_04_HYOID);	// 課税売上高計算表
	//filter.Format(filter + _T("hyoId = \'%s\' ) "),  ICS_TAXLIST_05_HYOID);	// 課税仕入高計算表
// 230728Aw del <--
// 230728Aw add -->
	if(m_Util.IsUnConnectMst(pDBNpSub->zvol) && (m_Util.GetKazeihoushiki(pDBNpSub->zvol) == ID_ICSSH_KANNI)) {
		filter.Format(filter + _T("hyoId = \'%s\' or "), ICS_TAXLIST_01_HYOID);	// 課税取引金額計算表(事業所得用)
		filter.Format(filter + _T("hyoId = \'%s\' or "), ICS_TAXLIST_02_HYOID);	// 課税取引金額計算表(農業所得用)
		filter.Format(filter + _T("hyoId = \'%s\' or "), ICS_TAXLIST_03_HYOID);	// 課税取引金額計算表(不動産所得用)
		filter.Format(filter + _T("hyoId = \'%s\' or "), ICS_TAXLIST_04_HYOID);	// 課税売上高計算表
		filter.Format(filter + _T("hyoId = \'%s\' or "), ICS_TAXLIST_05_HYOID);	// 課税仕入高計算表
		filter.Format(filter + _T("hyoId = \'%s\' ) "),  ICS_TAXLIST_06_HYOID);	// 事業別売上計算表
	}
	else {
		filter.Format(filter + _T("hyoId = \'%s\' or "), ICS_TAXLIST_01_HYOID);	// 課税取引金額計算表(事業所得用)
		filter.Format(filter + _T("hyoId = \'%s\' or "), ICS_TAXLIST_02_HYOID);	// 課税取引金額計算表(農業所得用)
		filter.Format(filter + _T("hyoId = \'%s\' or "), ICS_TAXLIST_03_HYOID);	// 課税取引金額計算表(不動産所得用)
		filter.Format(filter + _T("hyoId = \'%s\' or "), ICS_TAXLIST_04_HYOID);	// 課税売上高計算表
		filter.Format(filter + _T("hyoId = \'%s\' ) "),  ICS_TAXLIST_05_HYOID);	// 課税仕入高計算表
	}
// 230728Aw add <--

	if(pDBSyzAcc->m_pSH_LST_VAL) {
		pDBSyzAcc->m_pSH_LST_VAL->Requery(filter);
	}
	else {
		if(pDBSyzAcc->OpenShLstVal(pDBNpSub->m_database, filter)) {
			return -1;
		}
	}

	int		kjct = pDBSyzAcc->m_pSH_LST_VAL->GetRecordCount();

	if(m_Util.IsUnConnectMst(pDBNpSub->zvol)) {
		// 非連動時
		if(kjct == 0) {
			// 新規
			pSnHeadData->SVsign &= 0xdf;
			if(!(pDBNpSub->zvol->s_sgn1 & 0xc0)) {
				pSnHeadData->SVsign |= 0x20;	// D5を使用　ON:込み OFF=抜き
			}
			else {
				Head.KHstax = 0x01;				// 抜き
			}
			pSnHeadData->Sn_Sign2 &= 0xf7;
			if(Head.KHstax & 0x01) {
				pSnHeadData->Sn_Sign2 |= 0x08;
			}
		}
		else {
			// 令和5年1月1日以降の課税期間の場合は、会社登録の経理方式の設定に従う
			long symd = 0, eymd = 0;
			GetKazeiKikan(pSnHeadData, symd, eymd);
			if(symd >= R05_SDAY) {
				if(pDBNpSub->zvol->s_sgn1 & 0xc0) {
					// 税抜き
					pSnHeadData->SVsign &= 0xdf;
					Head.KHstax &= 0xfe;
					Head.KHstax |= 0x01;
				}
				else {
					// 税込み
					pSnHeadData->SVsign &= 0xdf;
					pSnHeadData->SVsign |= 0x20;
					Head.KHstax &= 0xfe;
				}
			}
			else {
				// メンテ
				if(pSnHeadData->Sn_Sign2 & 0x08) {
					Head.KHstax = 0x01;
				}
				else {
					Head.KHstax = 0x00;
				}
			}
		}
		Head.KHkoke = 0x00;
		pSnHeadData->Sn_Sign4 &= ~0x02;
	}
	else {
		if(kjct == 0) {
			// 新規
			pSnHeadData->SVsign &= 0xdf;
			Head.KHstax &= 0xfe;
			if(!(pDBNpSub->zvol->s_sgn1 & 0xc0)) {
				pSnHeadData->SVsign |= 0x20;	// D5を使用　ON:込み OFF=抜き
			}
			else {
				Head.KHstax = 0x01;				// 抜き
			}
		}
		else {
			int	Mssw = 0;
			// メンテ
			if(pDBNpSub->zvol->s_sgn1 & 0xc0) {
				// 税抜き
				pSnHeadData->SVsign &= 0xdf;
				Head.KHstax &= 0xfe;
				Head.KHstax |= 0x01;
			}
			else {
				// 税込み
				pSnHeadData->SVsign &= 0xdf;
				pSnHeadData->SVsign |= 0x20;
				Head.KHstax &= 0xfe;
			}
		}
		if(pSnHeadData->Sn_Sign4 & 0x02) {
			Head.KHkoke = 0x01;
		}
		else {
			Head.KHkoke = 0x00;
		}
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

	// 課税取引金額計算表（事業所得用）
	Rct = 0;
	Count = 0;
	while(1) {
		if(Count >= KKVAL1MAX) {
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
	while(1) {
		if(Count >= KKVAL2MAX) {
			break;
		}
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
	while(1) {
		if(Count >= KKVAL3MAX) {
			break;
		}
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
	while(1) {
		if(Count >= KKVAL4MAX) {
			break;
		}
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
	while(1) {
		if(Count >= KKVAL5MAX) {
			break;
		}
		NRec.KSval[Rct].KKtype = KkVal5[Rct].KKtype;
		NRec.KSval[Rct].KKsign = KkVal5[Rct].KKsign;
		NRec.KSval[Rct].KKauto = KkVal5[Rct].KKauto;
		Rct++;
		Count++;
	}
	NRec.KSval[Rct].KKtype = 0xFF;

	CSH_LST_VAL		*pShLstVal = pDBSyzAcc->m_pSH_LST_VAL;

	for(ii = 0; ; ii++) {
		if(ii == 0) {
			if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {
				break;
			}
		}
		else {
			if((pShLstVal->MoveNext() == -1) || (pShLstVal->st == -1)) {
				break;
			}
		}

		// 課税取引金額計算表(事業所得用) SHE020
		if(pShLstVal->hyoId == ICS_TAXLIST_01_HYOID)	{
			// 売上(収入)金額
			Rct = 0;
			// Ａ（決定額）
			if(pShLstVal->itmId == _T("AMB00010")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval1, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AMB00020")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval2, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AMB00030")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval3, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("AMB00060")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval5, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AMB00070")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval6, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ

			// 売上原価／期首商品棚卸高 --------------------------------------------
			Rct++;
			// Ａ（決定額）
			if(pShLstVal->itmId == _T("AMC00020")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval1, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// 売上原価／仕入金額 --------------------------------------------
			Rct++;
			// Ａ（決定額）
			if(pShLstVal->itmId == _T("AMC00040")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval1, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AMC00050")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval2, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AMC00060")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval3, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("AMC00083")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval5, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｅ
			else if(pShLstVal->itmId == _T("AMC00084")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval7, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AMC00087")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval6, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AMC00088")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval8, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}

			// 売上原価／小計 --------------------------------------------
			Rct++;
			// Ａ（決定額）
			if(pShLstVal->itmId == _T("AMC00100")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval1, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}

			// 売上原価／期末棚卸高 --------------------------------------------
			Rct++;
			// Ａ（決定額）
			if(pShLstVal->itmId == _T("AMC00120")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval1, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}

			// 売上原価／差引原価 --------------------------------------------
			Rct++;
			// Ａ（決定額）
			if(pShLstVal->itmId == _T("AMC00140")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval1, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}

			// 差引原価 --------------------------------------------
			Rct++;
			// Ａ（決定額）
			if(pShLstVal->itmId == _T("AMD00010")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval1, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／租税公課 --------------------------------------------
			Rct++;
			// Ａ（決定額）
			if(pShLstVal->itmId == _T("AMD00020")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval1, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AMD00030")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval2, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AMD00040")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval3, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AMD00067")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval6, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AMD00068")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval8, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／荷造運賃 --------------------------------------------
			Rct++;
			// Ａ（決定額）
			if(pShLstVal->itmId == _T("AME00080")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval1, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AME00090")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval2, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AME00100")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval3, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AME00127")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval6, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AME00128")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval8, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／水道光熱費 --------------------------------------------
			Rct++;
			// Ａ（決定額）
			if(pShLstVal->itmId == _T("AME00140")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval1, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AME00150")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval3, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AME00177")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval6, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AME00178")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval8, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／旅費交通費 --------------------------------------------
			Rct++;
			// Ａ（決定額）
			if(pShLstVal->itmId == _T("AME00190")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval1, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AME00200")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval2, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AME00210")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval3, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AME00237")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval6, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AME00238")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval8, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／通信費 --------------------------------------------
			Rct++;
			// Ａ（決定額）
			if(pShLstVal->itmId == _T("AME00250")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval1, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AME00260")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval2, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AME00270")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval3, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("AME00293")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval5, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｅ
			else if(pShLstVal->itmId == _T("AME00294")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval7, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AME00297")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval6, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AME00298")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval8, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／広告宣伝費 --------------------------------------------
			Rct++;
			// Ａ（決定額）
			if(pShLstVal->itmId == _T("AME00310")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval1, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AME00320")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval2, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AME00330")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval3, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("AME00353")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval5, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｅ
			else if(pShLstVal->itmId == _T("AME00354")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval7, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AME00357")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval6, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AME00358")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval8, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／接待交際費 --------------------------------------------
			Rct++;
			// Ａ（決定額）
			if(pShLstVal->itmId == _T("AME00370")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval1, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AME00380")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval2, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AME00390")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval3, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("AME00413")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval5, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｅ
			else if(pShLstVal->itmId == _T("AME00414")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval7, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AME00417")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval6, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AME00418")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval8, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／損害保険料 --------------------------------------------
			Rct++;
			// Ａ（決定額）
			if(pShLstVal->itmId == _T("AME00430")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval1, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AME00440")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval2, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			// Ｄ（うち軽減税率6.24％適用分）
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			// Ｇ

			// 経費／修繕費 --------------------------------------------
			Rct++;
			// Ａ（決定額）
			if(pShLstVal->itmId == _T("AME00460")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval1, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AME00470")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval3, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AME00497")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval6, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AME00498")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval8, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／消耗品費 --------------------------------------------
			Rct++;
			// Ａ（決定額）
			if(pShLstVal->itmId == _T("AME00510")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval1, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AME00520")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval3, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("AME00543")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval5, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｅ
			else if(pShLstVal->itmId == _T("AME00544")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval7, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AME00547")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval6, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AME00548")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval8, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／減価償却費 --------------------------------------------
			Rct++;
			// Ａ（決定額）
			if(pShLstVal->itmId == _T("AME00560")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval1, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AME00570")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval2, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			// Ｄ（うち軽減税率6.24％適用分）
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			// Ｇ

			// 経費／福利厚生費 --------------------------------------------
			Rct++;
			// Ａ（決定額）
			if(pShLstVal->itmId == _T("AME00590")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval1, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AME00600")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval2, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AME00610")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval3, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("AME00633")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval5, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｅ
			else if(pShLstVal->itmId == _T("AME00634")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval7, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AME00637")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval6, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AME00638")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval8, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／給料賃金 --------------------------------------------
			Rct++;
			// Ａ（決定額）
			if(pShLstVal->itmId == _T("AME00650")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval1, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AME00660")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval2, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AME00670")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval3, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AME00697")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval6, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AME00698")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval8, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／外注工賃 --------------------------------------------
			Rct++;
			// Ａ（決定額）
			if(pShLstVal->itmId == _T("AME00710")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval1, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AME00720")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval2, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AME00724")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval3, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AME01160")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval6, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AME01161")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval8, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／利子割引料 --------------------------------------------
			Rct++;
			// Ａ（決定額）
			if(pShLstVal->itmId == _T("AME00740")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval1, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AME00750")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval2, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			// Ｄ（うち軽減税率6.24％適用分）
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			// Ｇ

			// 経費／地代家賃 --------------------------------------------
			Rct++;
			// Ａ（決定額）
			if(pShLstVal->itmId == _T("AME00800")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval1, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AME00810")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval2, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AME00814")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval3, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AME01180")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval6, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AME01181")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval8, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／貸倒金 --------------------------------------------
			Rct++;
			// Ａ（決定額）
			if(pShLstVal->itmId == _T("AME00830")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval1, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AME00840")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval2, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			// Ｄ（うち軽減税率6.24％適用分）
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			// Ｇ

			// 経費／任意科目 --------------------------------------------
			for(int jj = 0; jj < 6; jj++) {
				Rct++;
				CString cs;
				// 項目名
				cs.Format(_T("AME0950%d"), jj+1);
				if(pShLstVal->itmId == cs) {
					m_Util.cstring_to_char((unsigned char *)&NRec.KJval[Rct].KKnam, pShLstVal->ksname, 40);
					NRec.KJval[Rct].KKauto = pShLstVal->sign1;
				}
				// Ａ（決定額）
				cs.Format(_T("AME0960%d"), jj+1);
				if(pShLstVal->itmId == cs) {
					m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval1, pShLstVal->val);
					NRec.KJval[Rct].KKauto = pShLstVal->sign1;
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				cs.Format(_T("AME0970%d"), jj+1);
				if(pShLstVal->itmId == cs) {
					m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval2, pShLstVal->val);
					NRec.KJval[Rct].KKauto = pShLstVal->sign1;
				}
				// Ｃ（課税取引金額(A-B)）
				cs.Format(_T("AME0980%d"), jj+1);
				if(pShLstVal->itmId == cs) {
					m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval3, pShLstVal->val);
					NRec.KJval[Rct].KKauto = pShLstVal->sign1;
				}
				// Ｄ（うち軽減税率6.24％適用分）
				cs.Format(_T("AME1003%d"), jj+1);
				if(pShLstVal->itmId == cs) {
					m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval5, pShLstVal->val);
					NRec.KJval[Rct].KKauto = pShLstVal->sign1;
				}
				// Ｅ
				cs.Format(_T("AME1004%d"), jj+1);
				if(pShLstVal->itmId == cs) {
					m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval7, pShLstVal->val);
					NRec.KJval[Rct].KKauto = pShLstVal->sign1;
				}
				// Ｆ（うち標準税率7.8％適用分）
				cs.Format(_T("AME1007%d"), jj+1);
				if(pShLstVal->itmId == cs) {
					m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval6, pShLstVal->val);
					NRec.KJval[Rct].KKauto = pShLstVal->sign1;
				}
				// Ｇ
				cs.Format(_T("AME1008%d"), jj+1);
				if(pShLstVal->itmId == cs) {
					m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval8, pShLstVal->val);
					NRec.KJval[Rct].KKauto = pShLstVal->sign1;
				}
			}

			// 経費／雑費 --------------------------------------------
			Rct++;
			// Ａ（決定額）
			if(pShLstVal->itmId == _T("AME01020")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval1, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AME01030")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval2, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AME01040")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval3, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("AME01063")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval5, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｅ
			else if(pShLstVal->itmId == _T("AME01064")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval7, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AME01067")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval6, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AME01068")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval8, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／計 --------------------------------------------
			Rct++;
			// Ａ（決定額）
			if(pShLstVal->itmId == _T("AME01080")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval1, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AME01090")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval2, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AME01100")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval3, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("AME01130")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval5, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｅ
			else if(pShLstVal->itmId == _T("AME01131")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval7, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AME01140")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval6, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AME01141")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval8, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}

			// 差し引き金額 --------------------------------------------
			Rct++;
			// Ａ（決定額）
			if(pShLstVal->itmId == _T("AMF00010")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval1, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			// Ｃ（課税取引金額(A-B)）
			// Ｄ（うち軽減税率6.24％適用分）
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			// Ｇ

			// ３＋３２ --------------------------------------------
			Rct++;
			// Ａ（決定額）
			if(pShLstVal->itmId == _T("AMG00010")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval1, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AMG00020")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval3, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("AMG00040")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval5, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｅ
			else if(pShLstVal->itmId == _T("AMG00041")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval7, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AMG00050")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval6, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AMG00051")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval8, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}

			// 売上(収入)金額/下段(※itmIdは存在しないので適当) -----------------
			Rct++;
			// Ａ（決定額）
			if(pShLstVal->itmId == _T("AMB00110")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval1, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AMB00120")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval2, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AMB00130")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval3, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("AMB00160")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval5, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AMB00170")) {
				m_Util.val_to_bin((unsigned char*)NRec.KJval[Rct].KKval6, pShLstVal->val);
				NRec.KJval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
		}
		// 課税取引金額計算表(農業所得用) SHE100
		else if(pShLstVal->hyoId == ICS_TAXLIST_02_HYOID) {
			// 収入金額／販売金額--------------------------
			Rct = 0;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUB00020")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AUB00030")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval2, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AUB00040")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval3, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("AUB00063")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval5, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AUB00067")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval6, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ

			// 収入金額／家事消費金額--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUB00080")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AUB00090")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval3, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("AUB00113")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval5, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AUB00117")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval6, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ

			// 収入金額／事業消費金額--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUB00130")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AUB00140")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval2, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AUB00150")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval3, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("AUB00173")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval5, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AUB00177")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval6, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ

			// 収入金額／雑収入--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUB00190")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AUB00200")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval2, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AUB00210")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval3, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("AUB00233")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval5, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AUB00237")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval6, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ

			// 収入金額／未成熟果樹収入--------------------------
			Rct++;
			// Ａ（決算額）
			// Ｂ（Ａのうち課税取引にならないもの）
			// Ｃ（課税取引金額(A-B)）
			if(pShLstVal->itmId == _T("AUB00250")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval3, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("AUB00273")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval5, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AUB00277")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval6, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ

			// 収入金額／小計--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUB00290")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AUB00300")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval2, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AUB00310")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval3, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("AUB00333")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval5, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AUB00337")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval6, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ

			// 収入金額／農作物の棚卸高期首--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUB00350")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			// Ｃ（課税取引金額(A-B)）
			// Ｄ（うち軽減税率6.24％適用分）
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			// Ｇ

			// 収入金額／農作物の棚卸高期末--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUB00370")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			// Ｃ（課税取引金額(A-B)）
			// Ｄ（うち軽減税率6.24％適用分）
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			// Ｇ

			// 収入金額／計--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUB00390")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			// Ｃ（課税取引金額(A-B)）
			// Ｄ（うち軽減税率6.24％適用分）
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			// Ｇ

			// 経費／租税公課--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUC00020")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AUC00030")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval2, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AUC00040")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval3, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			//else if(pShLstVal->itmId == _T("AUC00063")) {
			//	m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval5, pShLstVal->val);
			//}
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AUC00067")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval6, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AUC00068")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval8, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／種苗費--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUC00080")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AUC00090")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval2, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AUC00100")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval3, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("AUC00123")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval5, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｅ
			else if(pShLstVal->itmId == _T("AUC00124")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval7, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AUC00127")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval6, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AUC00128")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval8, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／素畜費--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUC00140")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AUC00150")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval2, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AUC00160")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval3, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			//else if(pShLstVal->itmId == _T("AUC00183")) {
			//	m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval5, pShLstVal->val);
			//}
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AUC00187")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval6, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AUC00188")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval8, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／肥料費--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUC00200")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AUC00210")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval2, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AUC00220")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval3, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("AUC00243")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval5, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｅ
			else if(pShLstVal->itmId == _T("AUC00244")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval7, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AUC00247")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval6, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AUC00248")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval8, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／飼料費--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUC00260")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AUC00270")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval2, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AUC00280")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval3, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("AUC00303")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval5, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｅ
			else if(pShLstVal->itmId == _T("AUC00304")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval7, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AUC00307")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval6, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AUC00308")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval8, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／農具費--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUC00320")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AUC00330")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval3, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			//else if(pShLstVal->itmId == _T("AUC00353")) {
			//	m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval5, pShLstVal->val);
			//}
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AUC00357")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval6, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AUC00358")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval8, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／農薬・衛生費--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUC00370")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AUC00380")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval3, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			//else if(pShLstVal->itmId == _T("AUC00403")) {
			//	m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval5, pShLstVal->val);
			//}
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AUC00407")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval6, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AUC00408")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval8, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／諸材料費--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUC00420")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AUC00430")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval3, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			//else if(pShLstVal->itmId == _T("AUC00453")) {
			//	m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval5, pShLstVal->val);
			//}
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AUC00457")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval6, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AUC00458")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval8, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／修繕費--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUC00470")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AUC00480")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval3, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			//else if(pShLstVal->itmId == _T("AUC00503")) {
			//	m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval5, pShLstVal->val);
			//}
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AUC00507")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval6, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AUC00508")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval8, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／動力光熱費--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUC00520")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AUC00530")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval2, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AUC00540")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval3, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			//else if(pShLstVal->itmId == _T("AUC00563")) {
			//	m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval5, pShLstVal->val);
			//}
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AUC00567")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval6, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AUC00568")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval8, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／作業用衣料費--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUC00580")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AUC00590")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval3, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			//else if(pShLstVal->itmId == _T("AUC00613")) {
			//	m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval5, pShLstVal->val);
			//}
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AUC00617")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval6, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AUC00618")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval8, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／農業共済掛金--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUC00630")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AUC00640")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval2, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			// Ｄ（うち軽減税率6.24％適用分）
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			// Ｇ

			// 経費／減価償却費--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUC00660")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AUC00670")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval2, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			// Ｄ（うち軽減税率6.24％適用分）
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			// Ｇ

			// 経費／荷造運賃手数料--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUC00690")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AUC00700")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval2, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AUC00710")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval3, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			//else if(pShLstVal->itmId == _T("AUC00733")) {
			//	m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval5, pShLstVal->val);
			//}
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AUC00737")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval6, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AUC00738")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval8, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／雇人費--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUC00750")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AUC00760")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval2, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AUC00770")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval3, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			//else if(pShLstVal->itmId == _T("AUC00793")) {
			//	m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval5, pShLstVal->val);
			//}
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AUC00797")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval6, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AUC00798")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval8, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／利子割引料--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUC00810")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AUC00820")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval2, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			// Ｄ（うち軽減税率6.24％適用分）
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			// Ｇ

			// 経費／地代・賃借料--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUC00840")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AUC00850")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval2, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AUC00860")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval3, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			//else if(pShLstVal->itmId == _T("AUC00883")) {
			//	m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval5, pShLstVal->val);
			//}
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AUC00887")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval6, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AUC00888")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval8, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／土地改良費--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUC00900")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AUC00910")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval2, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AUC00920")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval3, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			//else if(pShLstVal->itmId == _T("AUC00943")) {
			//	m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval5, pShLstVal->val);
			//}
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AUC00947")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval6, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AUC00948")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval8, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／貸倒金--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUC00960")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AUC00970")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval2, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			// Ｄ（うち軽減税率6.24％適用分）
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			// Ｇ

			// 経費／任意科目行３行--------------------------
			for(int jj = 0; jj < 3; jj++) {
				CString cs;

				Rct++;
				// 項目名
				cs.Format(_T("AUC0990%d"), jj+1);
				if(pShLstVal->itmId == cs) {
					m_Util.cstring_to_char((unsigned char *)&NRec.KNval[Rct].KKnam, pShLstVal->ksname, 40);
					NRec.KNval[Rct].KKauto = pShLstVal->sign1;
				}
				// Ａ（決算額）
				cs.Format(_T("AUC1000%d"), jj+1);
				if(pShLstVal->itmId == cs) {
					m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
					NRec.KNval[Rct].KKauto = pShLstVal->sign1;
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				cs.Format(_T("AUC1010%d"), jj+1);
				if(pShLstVal->itmId == cs) {
					m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval2, pShLstVal->val);
					NRec.KNval[Rct].KKauto = pShLstVal->sign1;
				}
				// Ｃ（課税取引金額(A-B)）
				cs.Format(_T("AUC1020%d"), jj+1);
				if(pShLstVal->itmId == cs) {
					m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval3, pShLstVal->val);
					NRec.KNval[Rct].KKauto = pShLstVal->sign1;
				}
				// Ｄ（うち軽減税率6.24％適用分）
				cs.Format(_T("AUC1043%d"), jj+1);
				if(pShLstVal->itmId == cs) {
					m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval5, pShLstVal->val);
					NRec.KNval[Rct].KKauto = pShLstVal->sign1;
				}
				// Ｅ
				cs.Format(_T("AUC1044%d"), jj+1);
				if(pShLstVal->itmId == cs) {
					m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval7, pShLstVal->val);
					NRec.KNval[Rct].KKauto = pShLstVal->sign1;
				}
				// Ｆ（うち標準税率7.8％適用分）
				cs.Format(_T("AUC1047%d"), jj+1);
				if(pShLstVal->itmId == cs) {
					m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval6, pShLstVal->val);
					NRec.KNval[Rct].KKauto = pShLstVal->sign1;
				}
				// Ｇ
				cs.Format(_T("AUC1048%d"), jj+1);
				if(pShLstVal->itmId == cs) {
					m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval8, pShLstVal->val);
					NRec.KNval[Rct].KKauto = pShLstVal->sign1;
				}
			}

			// 経費／雑費--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUC01060")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AUC01070")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval2, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AUC01080")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval3, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("AUC01103")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval5, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｅ
			else if(pShLstVal->itmId == _T("AUC01104")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval7, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AUC01107")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval6, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AUC01108")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval8, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／小計--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUC01120")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AUC01130")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval2, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AUC01140")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval3, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("AUC01163")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval5, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｅ
			else if(pShLstVal->itmId == _T("AUC01164")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval7, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AUC01167")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval6, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AUC01168")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval8, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／農産物以外の棚卸高期首--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUC01180")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			// Ｃ（課税取引金額(A-B)）
			// Ｄ（うち軽減税率6.24％適用分）
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			// Ｇ

			// 経費／農産物以外の棚卸高期末--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUC01200")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			// Ｃ（課税取引金額(A-B)）
			// Ｄ（うち軽減税率6.24％適用分）
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			// Ｇ

			// 経費／経費から差し引く果樹牛馬等の育成費用--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUC01220")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			// Ｃ（課税取引金額(A-B)）
			// Ｄ（うち軽減税率6.24％適用分）
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			// Ｇ

			// 経費／計--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUC01240")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			// Ｃ（課税取引金額(A-B)）
			// Ｄ（うち軽減税率6.24％適用分）
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			// Ｇ

			// 経費／差引金額--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUD00010")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			// Ｃ（課税取引金額(A-B)）
			// Ｄ（うち軽減税率6.24％適用分）
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			// Ｇ

			// 収入金額／小計（返還等対価）--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AUB01290")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval1, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AUB01300")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval2, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AUB01310")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval3, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("AUB01333")) {
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval5, pShLstVal->val);
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AUB01337")) {
				//m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval6, pShLstVal->val);	// 230727Aw del
				m_Util.val_to_bin((unsigned char*)NRec.KNval[Rct].KKval6, pShLstVal->val);		// 230727Aw add
				NRec.KNval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
		}
		// 課税取引金額計算表(不動産所得用) SHE040
		else if(pShLstVal->hyoId == ICS_TAXLIST_03_HYOID) {
			// 収入金額／賃借料--------------------------
			Rct = 0;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AOB00020")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval1, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AOB00030")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval2, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AOB00040")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval3, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			//else if(pShLstVal->itmId == _T("AOB00063")) {
			//	m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval5, pShLstVal->val);
			//}
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AOB00067")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval6, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ

			// 収入金額／礼金・権利金更新料--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AOB00080")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval1, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AOB00090")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval2, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AOB00100")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval3, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			//else if(pShLstVal->itmId == _T("AOB00123")) {
			//	m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval5, pShLstVal->val);
			//}
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AOB00127")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval6, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ

			// 収入金額／任意科目--------------------------
			Rct++;
			// 項目名
			if(pShLstVal->itmId == _T("AOB00140")) {
				m_Util.cstring_to_char((unsigned char *)&NRec.KFval[Rct].KKnam, pShLstVal->ksname, 40);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ａ（決算額）
			else if(pShLstVal->itmId == _T("AOB00150")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval1, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AOB00160")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval2, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AOB00170")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval3, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("AOB00193")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval5, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AOB00197")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval6, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ

			// 収入金額／計--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AOB00210")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval1, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AOB00220")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval2, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AOB00230")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval3, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("AOB00260")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval5, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｅ
			else if(pShLstVal->itmId == _T("AOB00263")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval5, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AOB00270")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval6, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ

			// 経費／租税公課--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AOC00020")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval1, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AOC00030")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval2, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AOC00040")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval3, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			//else if(pShLstVal->itmId == _T("AOC00063")) {
			//	m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval5, pShLstVal->val);
			//}
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AOC00067")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval6, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AOC00068")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval8, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／損害保険料--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AOC00080")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval1, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AOC00090")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval2, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			// Ｄ（うち軽減税率6.24％適用分）
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			// Ｇ

			// 経費／修繕費--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AOC00110")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval1, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AOC00120")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval3, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			//else if(pShLstVal->itmId == _T("AOC00143")) {
			//	m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval5, pShLstVal->val);
			//}
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AOC00147")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval6, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AOC00148")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval8, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／減価償却費--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AOC00160")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval1, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AOC00170")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval2, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			// Ｄ（うち軽減税率6.24％適用分）
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			// Ｇ

			// 経費／借入金利子--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AOC00190")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval1, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AOC00200")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval2, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			// Ｄ（うち軽減税率6.24％適用分）
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			// Ｇ

			// 経費／地代家賃--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AOC00220")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval1, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AOC00230")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval2, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AOC00240")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval3, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			//else if(pShLstVal->itmId == _T("AOC00263")) {
			//	m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval5, pShLstVal->val);
			//}
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AOC00267")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval6, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AOC00268")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval8, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／給料賃金--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AOC00280")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval1, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AOC00290")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval2, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AOC00300")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval3, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			//else if(pShLstVal->itmId == _T("AOC00323")) {
			//	m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval5, pShLstVal->val);
			//}
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AOC00327")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval6, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AOC00328")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval8, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／任意科目--------------------------
			Rct++;
			// 項目名
			if(pShLstVal->itmId == _T("AOC00340")) {
				m_Util.cstring_to_char((unsigned char *)&NRec.KFval[Rct].KKnam, pShLstVal->ksname, 40);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ａ（決算額）
			else if(pShLstVal->itmId == _T("AOC00350")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval1, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AOC00360")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval2, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AOC00370")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval3, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("AOC00393")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval5, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｅ
			else if(pShLstVal->itmId == _T("AOC00394")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval7, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AOC00397")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval6, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AOC00398")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval8, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／その他の経費--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AOC00410")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval1, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AOC00420")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval2, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AOC00430")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval3, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("AOC00453")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval5, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｅ
			else if(pShLstVal->itmId == _T("AOC00454")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval7, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AOC00457")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval6, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AOC00458")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval8, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}

			// 経費／計--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AOC00470")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval1, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AOC00480")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval2, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AOC00490")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval3, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("AOC00520")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval5, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｅ
			else if(pShLstVal->itmId == _T("AOC00521")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval7, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AOC00530")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval6, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
			else if(pShLstVal->itmId == _T("AOC00531")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval8, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}

			// 差し引き金額--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AOD00010")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval1, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			// Ｃ（課税取引金額(A-B)）
			// Ｄ（うち軽減税率6.24％適用分）
			// Ｅ
			// Ｆ（うち標準税率7.8％適用分）
			// Ｇ

			// 収入金額／返還等対価(計の下段)--------------------------
			Rct++;
			// Ａ（決算額）
			if(pShLstVal->itmId == _T("AOB01210")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval1, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｂ（Ａのうち課税取引にならないもの）
			else if(pShLstVal->itmId == _T("AOB01220")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval2, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｃ（課税取引金額(A-B)）
			else if(pShLstVal->itmId == _T("AOB01230")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval3, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｄ（うち軽減税率6.24％適用分）
// 230727Aw del -->
			//else if(pShLstVal->itmId == _T("AOB01260")) {
			//	m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval5, pShLstVal->val);
			//}
// 230727Aw del <--
			else if(pShLstVal->itmId == _T("AOB01263")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval5, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｅ
			else if(pShLstVal->itmId == _T("AOB01263")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval5, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｆ（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("AOB01270")) {
				m_Util.val_to_bin((unsigned char*)NRec.KFval[Rct].KKval6, pShLstVal->val);
				NRec.KFval[Rct].KKauto = pShLstVal->sign1;
			}
			// Ｇ
		}
		// 課税売上高計算表 SHE060
		else if(pShLstVal->hyoId == ICS_TAXLIST_04_HYOID) {
			Rct = 0;

			// （3）任意科目　名称 --------------------------------------------
			if(pShLstVal->itmId == _T("AQD00010")) {
				m_Util.cstring_to_char((unsigned char *)&Head.KHkunm, pShLstVal->ksname, 40);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}

			// (1)事業所得に係る課税売上高 --------------------------------------------
			// 営業等課税売上高 --------------------------------------------
			//Rct++;
			// 金額
			if(pShLstVal->itmId == _T("AQB00040")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval1, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;		// 230802Aw_46 add
			}
			//うち軽減税率6.24％適用分
			else if(pShLstVal->itmId == _T("AQB00070")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval3, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;		// 230802Aw_46 add
			}
			//うち標準税率7.8％適用分
			else if(pShLstVal->itmId == _T("AQB00080")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval4, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;		// 230802Aw_46 add
			}

			// 農業課税売上高 --------------------------------------------
			Rct++;
			//金額
			if(pShLstVal->itmId == _T("AQB00100")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval1, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}
			//うち軽減税率6.24％適用分
			else if(pShLstVal->itmId == _T("AQB00120")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval3, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}
			//うち標準税率7.8％適用分
			else if(pShLstVal->itmId == _T("AQB00130")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval4, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}

			// 不動産所得に係る課税売上高 --------------------------------------------
			// 課税売上高 --------------------------------------------
			Rct++;
			//金額
			if(pShLstVal->itmId == _T("AQC00040")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval1, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}
			//うち軽減税率6.24％適用分
			else if(pShLstVal->itmId == _T("AQC00070")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval3, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}
			//うち標準税率7.8％適用分
			else if(pShLstVal->itmId == _T("AQC00080")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval4, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}

			// （3）所得に係る課税売上高 --------------------------------------------
			// 損益計算書の収入金額 --------------------------------------------
			Rct++;
			//金額
			if(pShLstVal->itmId == _T("AQD00020")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval1, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}
			//うち軽減税率6.24％適用分
			else if(pShLstVal->itmId == _T("AQD00025")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval3, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}
			//うち標準税率7.8％適用分 
			else if(pShLstVal->itmId == _T("AQD00027")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval4, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}

			// ④のうち、課税売上にならないもの --------------------------------------------
			Rct++;
			//金額
			if(pShLstVal->itmId == _T("AQD00030")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval1, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}
			//うち軽減税率6.24％適用分
			else if(pShLstVal->itmId == _T("AQD00035")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval3, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}
			//うち標準税率7.8％適用分
			else if(pShLstVal->itmId == _T("AQD00037")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval4, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}

			// 差引課税売上高 --------------------------------------------
			Rct++;
			//金額
			if(pShLstVal->itmId == _T("AQD00050")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval1, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}
			//うち軽減税率6.24％適用分
			else if(pShLstVal->itmId == _T("AQD00080")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval3, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}
			//うち標準税率7.8％適用分
			else if(pShLstVal->itmId == _T("AQD00090")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval4, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}

			// (4)業務用資産の譲渡所得に係る課税売上高 --------------------------------------------
			// 業務用固定資産等の譲渡収入金額 --------------------------------------------
			Rct++;
			//金額
			if(pShLstVal->itmId == _T("AQE00010")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval1, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}
			//うち軽減税率6.24％適用分
			else if(pShLstVal->itmId == _T("AQE00015")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval3, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}
			//うち標準税率7.8％適用分
			else if(pShLstVal->itmId == _T("AQE00017")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval4, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}

			// ⑦のうち、課税売上にならないもの --------------------------------------------
			Rct++;
			//金額
			if(pShLstVal->itmId == _T("AQE00020")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval1, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}
			//うち軽減税率6.24％適用分
			else if(pShLstVal->itmId == _T("AQE00025")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval3, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}
			//うち標準税率7.8％適用分
			else if(pShLstVal->itmId == _T("AQE00027")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval4, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}

			// 差引課税売上高 --------------------------------------------
			Rct++;
			//金額
			if(pShLstVal->itmId == _T("AQE00040")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval1, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}
			//うち軽減税率6.24％適用分
			else if(pShLstVal->itmId == _T("AQE00070")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval3, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}
			//うち標準税率7.8％適用分
			//else if(pShLstVal->itmId == _T("AQD00080")) {	// 230727Aw del
			else if(pShLstVal->itmId == _T("AQE00080")) {	// 230727Aw add
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval4, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}

			// 課税売上高の合計額 --------------------------------------------
			Rct++;
			//金額
			if(pShLstVal->itmId == _T("AQF00010")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval1, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}
			//うち軽減税率6.24％適用分
			else if(pShLstVal->itmId == _T("AQF00040")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval3, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}
			//うち標準税率7.8％適用分
			else if(pShLstVal->itmId == _T("AQF00050")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval4, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}

			// (6)課税資産の譲渡等の対価の額の計算 --------------------------------------------
			// （軽減税率6.24％適用分） --------------------------------------------
			Rct++;
			Rct++;
			// 1円未満の端数切捨て
			if(pShLstVal->itmId == _T("AQG00044")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval1, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}
			// ○○円×100/108
			else if(pShLstVal->itmId == _T("AQG00042")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval2, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}
			// 仮受消費税等(上段)
			else if(pShLstVal->itmId == _T("AQG00041")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval3, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}

			// （標準税率7.8％適用分） --------------------------------------------
			Rct++;
			// 1円未満の端数切捨て
			if(pShLstVal->itmId == _T("AQG00048")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval1, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}
			// ○○円×100/108
			else if(pShLstVal->itmId == _T("AQG00046")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval2, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}
			// 仮受消費税等(上段)
			else if(pShLstVal->itmId == _T("AQG00045")) {
				m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval3, pShLstVal->val);
				NRec.KUval[Rct].KKauto = pShLstVal->sign1;
			}

		}
		// 課税仕入高計算表 SHE080
		else if(pShLstVal->hyoId == ICS_TAXLIST_05_HYOID) {		// 230728Aw add
			Rct = 0;

			// （3）任意科目　名称 --------------------------------------------
			if(pShLstVal->itmId == _T("ASD00010")) {
				m_Util.cstring_to_char((unsigned char *)&Head.KHksnm, pShLstVal->ksname, 40);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}

			// (1)事業所得に係る課税仕入高 --------------------------------------------
			// 営業等課税仕入高 --------------------------------------------
			//Rct++;
			//金額
			if(pShLstVal->itmId == _T("ASB00040")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval1, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//課税仕入高（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("ASB00070")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval3, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//経過措置適用（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("ASB00071")) {
				//m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval6, pShLstVal->val);	// 230728Aw del
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval7, pShLstVal->val);		// 230728Aw add
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//課税仕入高（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("ASB00080")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval4, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//経過措置適用（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("ASB00081")) {
				//m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval7, pShLstVal->val);	// 230728Aw del
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval8, pShLstVal->val);		// 230728Aw add
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}

			// 農業課税仕入高 --------------------------------------------
			Rct++;
			//金額
			if(pShLstVal->itmId == _T("ASB00100")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval1, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//課税仕入高（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("ASB00120")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval3, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//経過措置適用（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("ASB00121")) {
				//m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval6, pShLstVal->val);	// 230728Aw del
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval7, pShLstVal->val);		// 230728Aw add
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//課税仕入高（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("ASB00130")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval4, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//経過措置適用（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("ASB00131")) {
				//m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval7, pShLstVal->val);	// 230728Aw del
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval8, pShLstVal->val);		// 230728Aw add
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}

		// 不動産所得に係る課税仕入高 --------------------------------------------
			// 課税仕入高 --------------------------------------------
			Rct++;
			//金額
			if(pShLstVal->itmId == _T("ASC00040")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval1, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//課税仕入高（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("ASC00070")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval3, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//経過措置適用（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("ASC00071")) {
				//m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval6, pShLstVal->val);	// 230728Aw del
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval7, pShLstVal->val);		// 230728Aw add
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//課税仕入高（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("ASC00080")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval4, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//経過措置適用（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("ASC00081")) {
				//m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval7, pShLstVal->val);	// 230728Aw del
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval8, pShLstVal->val);		// 230728Aw add
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}

		// （　）所得に係る課税仕入高 --------------------------------------------
			// 損益計算書の仕入金額と経費の金額の合計額 --------------------------------------------
			Rct++;
			//金額
			if(pShLstVal->itmId == _T("ASD00020")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval1, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//課税仕入高（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("ASD00025")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval3, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//経過措置適用（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("ASD00026")) {
				//m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval6, pShLstVal->val);	// 230728Aw del
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval7, pShLstVal->val);		// 230728Aw add
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//課税仕入高（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("ASD00027")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval4, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//経過措置適用（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("ASD00028")) {
				//m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval7, pShLstVal->val);	// 230728Aw del
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval8, pShLstVal->val);		// 230728Aw add
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}

			// ④のうち、課税仕入にならないもの --------------------------------------------
			Rct++;
			//金額
			if(pShLstVal->itmId == _T("ASD00030")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval1, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//課税仕入高（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("ASD00035")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval3, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//経過措置適用（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("ASD00036")) {
				//m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval6, pShLstVal->val);	// 230728Aw del
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval7, pShLstVal->val);		// 230728Aw add
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//課税仕入高（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("ASD00037")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval4, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//経過措置適用（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("ASD00038")) {
				//m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval7, pShLstVal->val);	// 230728Aw del
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval8, pShLstVal->val);		// 230728Aw add
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}

			// 差引課税仕入高 --------------------------------------------
			Rct++;
			//金額
			if(pShLstVal->itmId == _T("ASD00050")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval1, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//課税仕入高（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("ASD00080")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval3, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//経過措置適用（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("ASD00081")) {
				//m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval6, pShLstVal->val);	// 230728Aw del
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval7, pShLstVal->val);		// 230728Aw add
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//課税仕入高（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("ASD00090")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval4, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//経過措置適用（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("ASD00091")) {
				//m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval7, pShLstVal->val);	// 230728Aw del
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval8, pShLstVal->val);		// 230728Aw add
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}

		// 業務用資産の取得に係る課税仕入高 --------------------------------------------
			// 業務用固定資産等の取得費 --------------------------------------------
			Rct++;
			//金額
			if(pShLstVal->itmId == _T("ASE00010")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval1, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//課税仕入高（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("ASE00015")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval3, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//経過措置適用（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("ASE00016")) {
				//m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval6, pShLstVal->val);	// 230728Aw del
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval7, pShLstVal->val);		// 230728Aw add
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//課税仕入高（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("ASE00017")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval4, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//経過措置適用（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("ASE00018")) {
				//m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval7, pShLstVal->val);	// 230728Aw del
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval8, pShLstVal->val);		// 230728Aw add
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}

			// ⑦のうち、課税仕入にならないもの --------------------------------------------
			Rct++;
			//金額
			if(pShLstVal->itmId == _T("ASE00020")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval1, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//課税仕入高（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("ASE00025")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval3, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//経過措置適用（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("ASE00026")) {
				//m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval6, pShLstVal->val);	// 230728Aw del
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval7, pShLstVal->val);		// 230728Aw add
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//課税仕入高（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("ASE00027")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval4, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//経過措置適用（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("ASE00028")) {
				//m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval7, pShLstVal->val);	// 230728Aw del
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval8, pShLstVal->val);		// 230728Aw add
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}

			// 差引課税仕入高 --------------------------------------------
			Rct++;
			//金額
			if(pShLstVal->itmId == _T("ASE00040")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval1, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//課税仕入高（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("ASE00070")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval3, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//経過措置適用（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("ASE00071")) {
				//m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval6, pShLstVal->val);	// 230728Aw del
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval7, pShLstVal->val);		// 230728Aw add
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//課税仕入高（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("ASE00080")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval4, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//経過措置適用（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("ASE00081")) {
				//m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval7, pShLstVal->val);	// 230728Aw del
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval8, pShLstVal->val);		// 230728Aw add
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}

		// 課税仕入高の合計額 --------------------------------------------
			Rct++;
			//金額
			if(pShLstVal->itmId == _T("ASF00010")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval1, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//課税仕入高（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("ASF00040")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval3, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//経過措置適用（うち軽減税率6.24％適用分）
			else if(pShLstVal->itmId == _T("ASF00041")) {
				//m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval6, pShLstVal->val);	// 230728Aw del
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval7, pShLstVal->val);		// 230728Aw add
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//課税仕入高（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("ASF00050")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval4, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			//経過措置適用（うち標準税率7.8％適用分）
			else if(pShLstVal->itmId == _T("ASF00051")) {
				//m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval7, pShLstVal->val);	// 230728Aw del
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval8, pShLstVal->val);		// 230728Aw add
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}

			// (6)課税仕入に係る消費税額の計算 --------------------------------------------
			Rct++;

			// （軽減税率6.24％適用分） --------------------------------------------
			Rct++;
			// 金額
			if(pShLstVal->itmId == _T("ASG00044")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval1, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			// ○円×6.24/108
			else if(pShLstVal->itmId == _T("ASG00042")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval2, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			// 仮払消費税等
			else if(pShLstVal->itmId == _T("ASG00041")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval3, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
// 230728Aw del -->
			//// 経過措置適用（軽減税率6.24％適用分） --------------------------------------------
			//// 金額
			//else if(pShLstVal->itmId == _T("ASG00144")) {
			//	m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval4, pShLstVal->val);
			//}
			//// ○円×6.24/108×80%
			//else if(pShLstVal->itmId == _T("ASG00142")) {
			//	m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval5, pShLstVal->val);
			//}
			//// 仮払消費税等
			//else if(pShLstVal->itmId == _T("ASG00141")) {
			//	m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval6, pShLstVal->val);
			//}
// 230728Aw del <--

			// （標準税率7.8％適用分） --------------------------------------------
			Rct++;

			// 金額
			if(pShLstVal->itmId == _T("ASG00048")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval1, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			// ○円×7.8/108
			else if(pShLstVal->itmId == _T("ASG00046")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval2, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			// 仮払消費税等
			else if(pShLstVal->itmId == _T("ASG00045")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval3, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}

// 230728Aw add -->
			// 経過措置適用（軽減税率6.24％適用分） --------------------------------------------
			Rct++;
			// 金額
			if(pShLstVal->itmId == _T("ASG00144")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval1, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			// ○円×6.24/108×80%
			else if(pShLstVal->itmId == _T("ASG00142")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval2, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			// 仮払消費税等
			else if(pShLstVal->itmId == _T("ASG00141")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval3, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
// 230728Aw add <--

// 230728Aw del -->
			//// 経過措置適用（標準税率7.8％適用分） --------------------------------------------
			//// 金額
			//else if(pShLstVal->itmId == _T("ASG00148")) {
			//	m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval4, pShLstVal->val);
			//}
			//// ○円×7.8/108×80%
			//else if(pShLstVal->itmId == _T("ASG00146")) {
			//	m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval5, pShLstVal->val);
			//}
			//// 仮払消費税等
			//else if(pShLstVal->itmId == _T("ASG00145")) {
			//	m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval6, pShLstVal->val);
			//}
// 230728Aw del <--
// 230728Aw add -->
			// 経過措置適用（標準税率7.8％適用分） --------------------------------------------
			Rct++;
			// 金額
			if(pShLstVal->itmId == _T("ASG00148")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval1, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			// ○円×7.8/108×80%
			else if(pShLstVal->itmId == _T("ASG00146")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval2, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
			// 仮払消費税等
			else if(pShLstVal->itmId == _T("ASG00145")) {
				m_Util.val_to_bin((unsigned char*)NRec.KSval[Rct].KKval3, pShLstVal->val);
				NRec.KSval[Rct].KKauto = pShLstVal->sign1;
			}
// 230728Aw add <--
		}
// 230728Aw add -->
		// 事業別売上計算表
		else {
			if(m_Util.IsUnConnectMst(pDBNpSub->zvol) && (m_Util.GetKazeihoushiki(pDBNpSub->zvol) == ID_ICSSH_KANNI)) {
				Rct = 40;
				// 第一種事業(卸売業)
				// 決定額
				if(pShLstVal->itmId == _T("ZAA00010")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval1, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// 課税取引外
				else if(pShLstVal->itmId == _T("ZAA00011")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval2, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
// 修正No.168646 add -->
				// 課税取引金額(自動計算)
				else if(pShLstVal->itmId == _T("ZAA00012")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval3, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
// 修正No.168646 add <--
				// 返還等課税取引
				else if(pShLstVal->itmId == _T("ZAA00013")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval4, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// うち税率7.8％適用分
				// 課税取引金額
				else if(pShLstVal->itmId == _T("ZAA00022")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval5, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// 返還等課税取引
				else if(pShLstVal->itmId == _T("ZAA00023")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval6, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// うち税率6.24％適用分
				// 課税取引金額
				else if(pShLstVal->itmId == _T("ZAA00032")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval7, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// 返還等課税取引
				else if(pShLstVal->itmId == _T("ZAA00033")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval8, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				//6.3追加分
				// 課税取引金額
				else if (pShLstVal->itmId == _T("ZAA00042")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval9, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				//　返還等課税取引
				else if (pShLstVal->itmId == _T("ZAA00043")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval10, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}

				// 第二種事業(小売業等)
				Rct++;
				// 決定額
				if(pShLstVal->itmId == _T("ZAB00010")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval1, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// 課税取引外
				else if(pShLstVal->itmId == _T("ZAB00011")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval2, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
// 修正No.168646 add -->
				// 課税取引金額(自動計算)
				else if(pShLstVal->itmId == _T("ZAB00012")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval3, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
// 修正No.168646 add <--
				// 返還等課税取引
				else if(pShLstVal->itmId == _T("ZAB00013")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval4, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// うち税率7.8％適用分
				// 課税取引金額
				else if(pShLstVal->itmId == _T("ZAB00022")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval5, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// 返還等課税取引
				else if(pShLstVal->itmId == _T("ZAB00023")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval6, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// うち税率6.24％適用分
				// 課税取引金額
				else if(pShLstVal->itmId == _T("ZAB00032")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval7, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// 返還等課税取引
				else if(pShLstVal->itmId == _T("ZAB00033")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval8, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				//6.3追加分
				// 課税取引金額
				else if (pShLstVal->itmId == _T("ZAB00062")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval9, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				//　返還等課税取引
				else if (pShLstVal->itmId == _T("ZAB00063")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval10, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}

				// 第三種事業(製造業等)
				Rct++;
				// 決定額
				if(pShLstVal->itmId == _T("ZAC00010")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval1, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// 課税取引外
				else if(pShLstVal->itmId == _T("ZAC00011")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval2, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
// 修正No.168646 add -->
				// 課税取引金額(自動計算)
				else if(pShLstVal->itmId == _T("ZAC00012")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval3, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
// 修正No.168646 add <--
				// 返還等課税取引
				else if(pShLstVal->itmId == _T("ZAC00013")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval4, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// うち税率7.8％適用分
				// 課税取引金額
				else if(pShLstVal->itmId == _T("ZAC00022")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval5, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// 返還等課税取引
				else if(pShLstVal->itmId == _T("ZAC00023")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval6, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// うち税率6.24％適用分
				// 課税取引金額
				else if(pShLstVal->itmId == _T("ZAC00032")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval7, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// 返還等課税取引
				else if(pShLstVal->itmId == _T("ZAC00033")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval8, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				//6.3追加分
				// 課税取引金額
				else if (pShLstVal->itmId == _T("ZAC00062")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval9, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				//　返還等課税取引
				else if (pShLstVal->itmId == _T("ZAC00063")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval10, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}

				// 第三種事業(製造業等)／農業事業者
				Rct++;
				// 決定額
				if(pShLstVal->itmId == _T("ZAC00040")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval1, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// 課税取引外
				else if(pShLstVal->itmId == _T("ZAC00041")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval2, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
// 修正No.168646 add -->
				// 課税取引金額(自動計算)
				else if(pShLstVal->itmId == _T("ZAC00042")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval3, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
// 修正No.168646 add <--
				// 返還等課税取引
				else if(pShLstVal->itmId == _T("ZAC00043")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval4, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// うち税率7.8％適用分
				// 課税取引金額
				else if(pShLstVal->itmId == _T("ZAC00052")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval5, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// 返還等課税取引
				else if(pShLstVal->itmId == _T("ZAC00053")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval6, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				//6.3追加分
				// 課税取引金額
				else if (pShLstVal->itmId == _T("ZAC00072")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval9, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				//　返還等課税取引
				else if (pShLstVal->itmId == _T("ZAC00073")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval10, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				
				// 第四種事業(その他)
				Rct++;
				// 決定額
				if(pShLstVal->itmId == _T("ZAD00010")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval1, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// 課税取引外
				else if(pShLstVal->itmId == _T("ZAD00011")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval2, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
// 修正No.168646 add -->
				// 課税取引金額(自動計算)
				else if(pShLstVal->itmId == _T("ZAD00012")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval3, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
// 修正No.168646 add <--
				// 返還等課税取引
				else if(pShLstVal->itmId == _T("ZAD00013")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval4, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// うち税率7.8％適用分
				// 課税取引金額
				else if(pShLstVal->itmId == _T("ZAD00022")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval5, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// 返還等課税取引
				else if(pShLstVal->itmId == _T("ZAD00023")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval6, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// うち税率6.24％適用分
				// 課税取引金額
				else if(pShLstVal->itmId == _T("ZAD00032")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval7, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// 返還等課税取引
				else if(pShLstVal->itmId == _T("ZAD00033")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval8, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				//6.3追加分
				// 課税取引金額
				else if (pShLstVal->itmId == _T("ZAD00072")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval9, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				//　返還等課税取引
				else if (pShLstVal->itmId == _T("ZAD00073")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval10, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}

				// 第五種事業(サービス業等)
				Rct++;
				// 決定額
				if(pShLstVal->itmId == _T("ZAE00010")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval1, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// 課税取引外
				else if(pShLstVal->itmId == _T("ZAE00011")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval2, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
// 修正No.168646 add -->
				// 課税取引金額(自動計算)
				else if(pShLstVal->itmId == _T("ZAE00012")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval3, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
// 修正No.168646 add <--
				// 返還等課税取引
				else if(pShLstVal->itmId == _T("ZAE00013")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval4, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// うち税率7.8％適用分
				// 課税取引金額
				else if(pShLstVal->itmId == _T("ZAE00022")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval5, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// 返還等課税取引
				else if(pShLstVal->itmId == _T("ZAE00023")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval6, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// うち税率6.24％適用分
				// 課税取引金額
				else if(pShLstVal->itmId == _T("ZAE00032")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval7, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// 返還等課税取引
				else if(pShLstVal->itmId == _T("ZAE00033")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval8, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				//6.3追加分
				// 課税取引金額
				else if (pShLstVal->itmId == _T("ZAE00042")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval9, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				//　返還等課税取引
				else if (pShLstVal->itmId == _T("ZAE00043")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval10, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}

				// 第六種事業(不動産業)
				Rct++;
				// 決定額
				if(pShLstVal->itmId == _T("ZAF00010")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval1, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// 課税取引外
				else if(pShLstVal->itmId == _T("ZAF00011")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval2, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
// 修正No.168646 add -->
				// 課税取引金額(自動計算)
				else if(pShLstVal->itmId == _T("ZAF00012")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval3, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
// 修正No.168646 add <--
				// 返還等課税取引
				else if(pShLstVal->itmId == _T("ZAF00013")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval4, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// うち税率7.8％適用分
				// 課税取引金額
				else if(pShLstVal->itmId == _T("ZAF00022")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval5, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// 返還等課税取引
				else if(pShLstVal->itmId == _T("ZAF00023")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval6, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// うち税率6.24％適用分
				// 課税取引金額
				else if(pShLstVal->itmId == _T("ZAF00032")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval7, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// 返還等課税取引
				else if(pShLstVal->itmId == _T("ZAF00033")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval8, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				//6.3追加分
				// 課税取引金額
				else if (pShLstVal->itmId == _T("ZAF00042")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval9, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				//　返還等課税取引
				else if (pShLstVal->itmId == _T("ZAF00043")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval10, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				
				// 合計
				Rct++;
				// 決定額
				if(pShLstVal->itmId == _T("ZAG00010")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval1, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// 課税取引外
				else if(pShLstVal->itmId == _T("ZAG00011")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval2, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// 課税取引金額
				else if(pShLstVal->itmId == _T("ZAG00012")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval3, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// 返還等課税取引
				else if(pShLstVal->itmId == _T("ZAG00013")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval4, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// うち税率7.8％適用分
				// 課税取引金額
				else if(pShLstVal->itmId == _T("ZAG00022")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval5, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// 返還等課税取引
				else if(pShLstVal->itmId == _T("ZAG00023")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval6, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// うち税率6.24％適用分
				// 課税取引金額
				else if(pShLstVal->itmId == _T("ZAG00032")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval7, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// 返還等課税取引
				else if(pShLstVal->itmId == _T("ZAG00033")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval8, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				//6.3追加分
				// 課税取引金額
				else if (pShLstVal->itmId == _T("ZAG00042")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval9, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				//　返還等課税取引
				else if (pShLstVal->itmId == _T("ZAG00043")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval10, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}

				// 第四種事業(その他)／固定資産等の譲渡
				Rct++;
				// 決定額
				if(pShLstVal->itmId == _T("ZAD00040")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval1, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// 課税取引外
				else if(pShLstVal->itmId == _T("ZAD00041")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval2, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
// 修正No.168646 add -->
				// 課税取引金額(自動計算)
				else if(pShLstVal->itmId == _T("ZAD00042")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval3, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
// 修正No.168646 add <--
				// 返還等課税取引
				else if(pShLstVal->itmId == _T("ZAD00043")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval4, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// うち税率7.8％適用分
				// 課税取引金額
				else if(pShLstVal->itmId == _T("ZAD00052")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval5, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				//// 返還等課税取引
				//else if(pShLstVal->itmId == _T("ZAD00053")) {
				//	m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval6, pShLstVal->val);
				//}
				// うち税率6.24％適用分
				// 課税取引金額
				else if(pShLstVal->itmId == _T("ZAD00062")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval7, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				//// 返還等課税取引
				//else if(pShLstVal->itmId == _T("ZAD00063")) {
				//	m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval8, pShLstVal->val);
				//}
				//6.3追加分
				// 課税取引金額
				else if (pShLstVal->itmId == _T("ZAD00082")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval9, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				////　返還等課税取引
				//else if (pShLstVal->itmId == _T("ZAD00083")) {
				//	m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval10, pShLstVal->val);
				//	NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				//}

				// 第二種事業(小売業等)／農業事業者
				Rct++;
				// 決定額
				if(pShLstVal->itmId == _T("ZAB00040")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval1, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// 課税取引外
				else if(pShLstVal->itmId == _T("ZAB00041")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval2, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
// 修正No.168646 add -->
				// 課税取引金額(自動計算)
				else if(pShLstVal->itmId == _T("ZAB00042")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval3, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
// 修正No.168646 add <--
				// 返還等課税取引
				else if(pShLstVal->itmId == _T("ZAB00043")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval4, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// うち税率6.24％適用分
				// 課税取引金額
				else if(pShLstVal->itmId == _T("ZAB00052")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval7, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
				// 返還等課税取引
				else if(pShLstVal->itmId == _T("ZAB00053")) {
					m_Util.val_to_bin((unsigned char*)NRec.KUval[Rct].KKval8, pShLstVal->val);
					NRec.KUval[Rct].KKauto = pShLstVal->sign1;
				}
			}
		}
// 230728Aw add <--
	}

	// 後始末
	delete pDBSyzAcc;

	return 0;
}
// 230718Aw <--

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
int CH31TaxListData::SetData( CDBNpSub *pDBNpSub, CSnHeadData *pSnHeadData, int sn_seq )
{
	CString			filter;
	CDBSyzAccess*	pDBSyzAcc;
	char			buf[512]={0};

// 230718Aw -->
	BOOL			syzSgn = FALSE;
	long			eymd    = 0;

	if(!(pSnHeadData->Sn_SKKBN % 2))    {
		eymd = pSnHeadData->Sn_MDAYE;
	}
	else    {
		eymd = pSnHeadData->Sn_KDAYE;
	}
	// 課税期間が2023/12/31以降の場合、SetData2()へ移行
	//　23.10.10課税取引金額計算表の制御を12/31→10/01切り替えに
	//if(eymd >= TAX_R05_EDAY) {
	if (eymd >= ICS_SH_INVOICE_DAY) {
		if(SetData2(pDBNpSub, pSnHeadData, sn_seq)) {
			return -1;
		}
		else {
			return 0;
		}
	}
// 230718Aw <--

	// 消費税拡張処理クラス
	pDBSyzAcc = NULL;
	pDBSyzAcc = new CDBSyzAccess;
	if( pDBSyzAcc == NULL ){
		return ERR;
	}
	// オープン
	filter.Format( "kssgn5 = %d and kslist >= 101 and kslist <= 106", sn_seq );
	if( pDBSyzAcc->KjsnkOpen( pDBNpSub->m_database, filter ) == ERR ) {
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
		pDBSyzAcc->m_pKjsnk->kslist = 101;
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
		pDBSyzAcc->m_pKjsnk->kslist = 102;
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
		pDBSyzAcc->m_pKjsnk->kslist = 103;
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
		pDBSyzAcc->m_pKjsnk->kslist = 104;
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
		pDBSyzAcc->m_pKjsnk->kslist = 105;
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


	// 事業別課税売上高
	if( m_Util.IsUnConnectMst(pDBNpSub->zvol) && (m_Util.GetKazeihoushiki(pDBNpSub->zvol)==ID_ICSSH_KANNI) ){
		Rct = 40;
		Count = 0;

		// うち税率分以外の金額を保存（決算額、課税取引外、課税取引額、返還等課税取引）
		while(1)	{
			if( Count >= 10){
				break;
			}

			// 新規データ作成　送信
			pDBSyzAcc->m_pKjsnk->AddNew();
			pDBSyzAcc->m_pKjsnk->kslist = 106;
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

			if( !Rct ) {
				m_Util.char_to_cstring( &pDBSyzAcc->m_pKjsnk->ksname, (unsigned char*)Head.KHkunm, 10 );
			}
			m_Arith.l_print( buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS" );	// 決算額
			pDBSyzAcc->m_pKjsnk->ksval0 = buf;
			pDBSyzAcc->m_pKjsnk->ksval0.TrimLeft();		// スペースカット
			m_Arith.l_print( buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS" );	// 課税取引外
			pDBSyzAcc->m_pKjsnk->ksval1 = buf;
			pDBSyzAcc->m_pKjsnk->ksval1.TrimLeft();		// スペースカット
			m_Arith.l_print( buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS" );	// 課税取引額
			pDBSyzAcc->m_pKjsnk->ksval2 = buf;
			pDBSyzAcc->m_pKjsnk->ksval2.TrimLeft();		// スペースカット
			m_Arith.l_print( buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS" );	// 返還等課税取引
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


		// うち税率分の金額を保存（6.3％、6.24％、7.8％）
		Rct = 40;
		Count = 0;
		while(1)	{
			if( Count >= 10){
				break;
			}

			// 新規データ作成　送信
			pDBSyzAcc->m_pKjsnk->AddNew();
			pDBSyzAcc->m_pKjsnk->kslist = 106;
			pDBSyzAcc->m_pKjsnk->ksline = Rct + 1;
			pDBSyzAcc->m_pKjsnk->ksitem = 1;	// アイテムを１にする（うち税率の分）
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
			m_Arith.l_print( buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS" );	// 7.8％　課税取引額
			pDBSyzAcc->m_pKjsnk->ksval0 = buf;
			pDBSyzAcc->m_pKjsnk->ksval0.TrimLeft();		// スペースカット
			m_Arith.l_print( buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS" );	// 7.8％　返還課税取引
			pDBSyzAcc->m_pKjsnk->ksval1 = buf;
			pDBSyzAcc->m_pKjsnk->ksval1.TrimLeft();		// スペースカット
			m_Arith.l_print( buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS" );	// 6.24％　課税取引額
			pDBSyzAcc->m_pKjsnk->ksval2 = buf;
			pDBSyzAcc->m_pKjsnk->ksval2.TrimLeft();		// スペースカット
			m_Arith.l_print( buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS" );	// 6.24％　返還課税取引
			pDBSyzAcc->m_pKjsnk->ksval3 = buf;
			pDBSyzAcc->m_pKjsnk->ksval3.TrimLeft();		// スペースカット
			m_Arith.l_print( buf, NRec.KUval[Rct].KKval9, "SSSSSSSSSSSSSSS" );	// 6.3％　課税取引額
			pDBSyzAcc->m_pKjsnk->ksval4 = buf;
			pDBSyzAcc->m_pKjsnk->ksval4.TrimLeft();		// スペースカット
			m_Arith.l_print( buf, NRec.KUval[Rct].KKval10, "SSSSSSSSSSSSSSS" );	// 6.3％　返還課税取引
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

// 230718Aw -->
//-----------------------------------------------------------------------------
// DB へデータ書込み(SYZ_SNK_LST_VALへ書込み)
//-----------------------------------------------------------------------------
// 引数	pDBNpSub	：	財務クラスのポインタ
//		pSnHeadData	：	消費税ヘッダ情報
//		sn_seq		：	対象の履歴シーケンス
//-----------------------------------------------------------------------------
// 返送値	0		：	正常終了
//			-1		：	エラー
//-----------------------------------------------------------------------------
int CH31TaxListData::SetData2(CDBNpSub *pDBNpSub, CSnHeadData *pSnHeadData, int sn_seq)
{
	int				nowVersion = 2;
	int				Rct = 0;
	int				Count = 0;
	int				ii;
	char			buf[512]={0};
	CString			filter;
	CDBSyzAccess*	pDBSyzAcc;

	// 消費税拡張処理クラス
	pDBSyzAcc = NULL;
	pDBSyzAcc = new CDBSyzAccess;
	if(pDBSyzAcc == NULL) {
		return ERR;
	}

//static char		*ICS_TAXLIST_01_HYOID	=	_T("SHE020");	// 課税取引金額計算表(事業所得用)
//static char		*ICS_TAXLIST_02_HYOID	=	_T("SHE100");	// 課税取引金額計算表(農業所得用)
//static char		*ICS_TAXLIST_03_HYOID	=	_T("SHE040");	// 課税取引金額計算表(不動産所得用)
//static char		*ICS_TAXLIST_04_HYOID	=	_T("SHE060");	// 課税売上高計算表
//static char		*ICS_TAXLIST_05_HYOID	=	_T("SHE080");	// 課税仕入高計算表

	filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
// 230728Aw del -->
	//filter.Format(filter + _T("hyoId = \'%s\' or "), ICS_TAXLIST_01_HYOID);	// 課税取引金額計算表(事業所得用)
	//filter.Format(filter + _T("hyoId = \'%s\' or "), ICS_TAXLIST_02_HYOID);	// 課税取引金額計算表(農業所得用)
	//filter.Format(filter + _T("hyoId = \'%s\' or "), ICS_TAXLIST_03_HYOID);	// 課税取引金額計算表(不動産所得用)
	//filter.Format(filter + _T("hyoId = \'%s\' or "), ICS_TAXLIST_04_HYOID);	// 課税売上高計算表
	//filter.Format(filter + _T("hyoId = \'%s\' ) "),  ICS_TAXLIST_05_HYOID);	// 課税仕入高計算表
// 230728Aw del <--

// 240220 戎居 del -->
// 230728Aw add -->
	//if(m_Util.IsUnConnectMst(pDBNpSub->zvol) && (m_Util.GetKazeihoushiki(pDBNpSub->zvol) == ID_ICSSH_KANNI)) {
	//	filter.Format(filter + _T("hyoId = \'%s\' or "), ICS_TAXLIST_01_HYOID);	// 課税取引金額計算表(事業所得用)
	//	filter.Format(filter + _T("hyoId = \'%s\' or "), ICS_TAXLIST_02_HYOID);	// 課税取引金額計算表(農業所得用)
	//	filter.Format(filter + _T("hyoId = \'%s\' or "), ICS_TAXLIST_03_HYOID);	// 課税取引金額計算表(不動産所得用)
	//	filter.Format(filter + _T("hyoId = \'%s\' or "), ICS_TAXLIST_04_HYOID);	// 課税売上高計算表
	//	filter.Format(filter + _T("hyoId = \'%s\' or "), ICS_TAXLIST_05_HYOID);	// 課税仕入高計算表
	//	filter.Format(filter + _T("hyoId = \'%s\' ) "),  ICS_TAXLIST_06_HYOID);	// 事業別売上高計算表
	//}
	//else {
	//	filter.Format(filter + _T("hyoId = \'%s\' or "), ICS_TAXLIST_01_HYOID);	// 課税取引金額計算表(事業所得用)
	//	filter.Format(filter + _T("hyoId = \'%s\' or "), ICS_TAXLIST_02_HYOID);	// 課税取引金額計算表(農業所得用)
	//	filter.Format(filter + _T("hyoId = \'%s\' or "), ICS_TAXLIST_03_HYOID);	// 課税取引金額計算表(不動産所得用)
	//	filter.Format(filter + _T("hyoId = \'%s\' or "), ICS_TAXLIST_04_HYOID);	// 課税売上高計算表
	//	filter.Format(filter + _T("hyoId = \'%s\' ) "),  ICS_TAXLIST_05_HYOID);	// 課税仕入高計算表
	//}
// 230728Aw add <--
// 240220 戎居 del <--

// 240220 戎居 add -->
//　事業別売上高を分離
	filter.Format(filter + _T("hyoId = \'%s\' or "), ICS_TAXLIST_01_HYOID);	// 課税取引金額計算表(事業所得用)
	filter.Format(filter + _T("hyoId = \'%s\' or "), ICS_TAXLIST_02_HYOID);	// 課税取引金額計算表(農業所得用)
	filter.Format(filter + _T("hyoId = \'%s\' or "), ICS_TAXLIST_03_HYOID);	// 課税取引金額計算表(不動産所得用)
	filter.Format(filter + _T("hyoId = \'%s\' or "), ICS_TAXLIST_04_HYOID);	// 課税売上高計算表
	filter.Format(filter + _T("hyoId = \'%s\' ) "), ICS_TAXLIST_05_HYOID);	// 課税仕入高計算表
// 240220 戎居 add <--

	// オープン
	if(pDBSyzAcc->m_pSH_LST_VAL) {
		pDBSyzAcc->m_pSH_LST_VAL->Requery(filter);
	}
	else {
		if(pDBSyzAcc->OpenShLstVal(pDBNpSub->m_database, filter) == ERR) {
			delete pDBSyzAcc;
			return ERR;
		}
	}

	CSH_LST_VAL		*pShLstVal = pDBSyzAcc->m_pSH_LST_VAL;

	if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
	// 課税取引金額計算表(事業所得用)
		// 売上(収入)金額 --------------------------------------------
		Rct = 0;
		// Ａ（決定額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMB00010");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;	 // 230802Aw_46 add
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMB00020");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMB00030");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｄ（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMB00060");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMB00070");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// 売上原価／期首商品棚卸高 --------------------------------------------
		Rct++;
		// Ａ（決定額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMC00020");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// 売上原価／仕入金額 --------------------------------------------
		Rct++;
		// Ａ（決定額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMC00040");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMC00050");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMC00060");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｄ（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMC00083");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｅ　（経過措置適用　6.24％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMC00084");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMC00087");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMC00088");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// 売上原価／小計 --------------------------------------------
		Rct++;
		// Ａ（決定額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMC00100");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// 売上原価／期末棚卸高 --------------------------------------------
		Rct++;
		// Ａ（決定額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMC00120");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// 売上原価／差引原価 --------------------------------------------
		Rct++;
		// Ａ（決定額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMC00140");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// 差引原価 --------------------------------------------
		Rct++;
		// Ａ（決定額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMD00010");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// 経費／租税公課 --------------------------------------------
		Rct++;
		// Ａ（決定額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMD00020");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMD00030");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMD00040");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMD00067");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMD00068");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();


		// 経費／荷造運賃 --------------------------------------------
		Rct++;
		// Ａ（決定額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00080");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00090");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00100");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00127");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00128");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// 経費／水道光熱費 --------------------------------------------
		Rct++;
		// Ａ（決定額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00140");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00150");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00177");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00178");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// 経費／旅費交通費 --------------------------------------------
		Rct++;
		// Ａ（決定額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00190");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00200");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00210");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00237");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00238");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// 経費／通信費 --------------------------------------------
		Rct++;
		// Ａ（決定額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00250");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00260");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00270");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｄ（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00293");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｅ　（経過措置適用　6.24％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00294");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00297");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00298");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// 経費／広告宣伝費 --------------------------------------------
		Rct++;
		// Ａ（決定額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00310");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00320");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00330");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｄ（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00353");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｅ　（経過措置適用　6.24％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00354");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00357");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00358");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// 経費／接待交際費 --------------------------------------------
		Rct++;
		// Ａ（決定額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00370");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00380");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00390");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｄ（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00413");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｅ　（経過措置適用　6.24％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00414");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00417");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00418");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// 経費／損害保険料 --------------------------------------------
		Rct++;
		// Ａ（決定額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00430");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00440");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// 経費／修繕費 --------------------------------------------
		Rct++;
		// Ａ（決定額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00460");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00470");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00497");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00498");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// 経費／消耗品費 --------------------------------------------
		Rct++;
		// Ａ（決定額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00510");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00520");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｄ（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00543");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｅ　（経過措置適用　6.24％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00544");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00547");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00548");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// 経費／減価償却費 --------------------------------------------
		Rct++;
		// Ａ（決定額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00560");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00570");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// 経費／福利厚生費 --------------------------------------------
		Rct++;
		// Ａ（決定額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00590");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00600");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00610");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｄ（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00633");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｅ　（経過措置適用　6.24％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00634");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00637");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00638");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// 経費／給料賃金 --------------------------------------------
		Rct++;
		// Ａ（決定額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00650");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00660");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00670");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00697");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00698");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// 経費／外注工賃 --------------------------------------------
		Rct++;
		// Ａ（決定額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00710");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00720");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00724");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME01160");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME01161");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// 経費／利子割引料 --------------------------------------------
		Rct++;
		// Ａ（決定額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00740");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00750");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// 経費／地代家賃 --------------------------------------------
		Rct++;
		// Ａ（決定額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00800");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00810");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00814");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME01180");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME01181");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// 経費／貸倒金 --------------------------------------------
		Rct++;
		// Ａ（決定額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00830");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME00840");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// 経費／任意科目１ --------------------------------------------
		for(int jj = 0; jj < 6; jj++) {
			Rct++;
			// 項目名
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
			pShLstVal->itmId.Format(_T("AME0950%d"), jj+1);
			pShLstVal->val.Empty();
			pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
			m_Util.char_to_cstring(&pShLstVal->ksname, (unsigned char*)NRec.KJval[Rct].KKnam, 40);
			pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
			pShLstVal->Update();

			// Ａ（決定額）
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
			pShLstVal->itmId.Format(_T("AME0960%d"), jj+1);
			m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
			pShLstVal->val = buf;
			pShLstVal->val.TrimLeft();		// スペースカット
			pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
			pShLstVal->Update();

			// Ｂ（Ａのうち課税取引にならないもの）
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
			pShLstVal->itmId.Format(_T("AME0970%d"), jj+1);
			m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
			pShLstVal->val = buf;
			pShLstVal->val.TrimLeft();		// スペースカット
			pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
			pShLstVal->Update();

			// Ｃ（課税取引金額(A-B)）
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
			pShLstVal->itmId.Format(_T("AME0980%d"), jj+1);
			m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
			pShLstVal->val = buf;
			pShLstVal->val.TrimLeft();		// スペースカット
			pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
			pShLstVal->Update();

			// Ｄ（うち軽減税率6.24％適用分）
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
			pShLstVal->itmId.Format(_T("AME1003%d"), jj+1);
			m_Arith.l_print(buf, NRec.KJval[Rct].KKval5, "SSSSSSSSSSSSSSS");
			pShLstVal->val = buf;
			pShLstVal->val.TrimLeft();		// スペースカット
			pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
			pShLstVal->Update();

			// Ｅ　（経過措置適用　6.24％）
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
			pShLstVal->itmId.Format(_T("AME1004%d"), jj + 1);
			m_Arith.l_print(buf, NRec.KJval[Rct].KKval7, "SSSSSSSSSSSSSSS");
			pShLstVal->val = buf;
			pShLstVal->val.TrimLeft();		// スペースカット
			pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
			pShLstVal->Update();

			// Ｆ（うち標準税率7.8％適用分）
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
			pShLstVal->itmId.Format(_T("AME1007%d"), jj+1);
			m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
			pShLstVal->val = buf;
			pShLstVal->val.TrimLeft();		// スペースカット
			pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
			pShLstVal->Update();

			// Ｇ　（経過措置適用　7.8％）
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
			pShLstVal->itmId.Format(_T("AME1008%d"), jj + 1);
			m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
			pShLstVal->val = buf;
			pShLstVal->val.TrimLeft();		// スペースカット
			pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
			pShLstVal->Update();
		}

		// 経費／雑費 --------------------------------------------
		Rct++;
		// Ａ（決定額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME01020");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME01030");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME01040");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｄ（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME01063");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｅ　（経過措置適用　6.24％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME01064");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME01067");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME01068");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// 経費／計 --------------------------------------------
		Rct++;
		// Ａ（決定額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME01080");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME01090");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME01100");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｄ（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME01130");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｅ　（経過措置適用　6.24％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME01131");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME01140");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AME01141");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// 差し引き金額 --------------------------------------------
		Rct++;
		// Ａ（決定額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMF00010");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// ３＋３２ --------------------------------------------
		Rct++;
		// Ａ（決定額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMG00010");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMG00020");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｄ（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMG00040");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｅ　（経過措置適用　6.24％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMG00041");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMG00050");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMG00051");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// 売上(収入)金額/下段(※itmIdは存在しないので適当) -----------------
		Rct++;
		// Ａ（決定額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMB00110");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMB00120");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMB00130");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｄ（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMB00160");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_01_HYOID;
		pShLstVal->itmId = _T("AMB00170");
		m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KJval[Rct].KKtype;
		pShLstVal->Update();


		// ------------------------------------------------------------------------
		// 課税取引金額計算表(農業所得用)
	// 収入金額
		// 販売金額 --------------------------------------------
		Rct = 0;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB00020");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB00030");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB00040");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｄ（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB00063");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB00067");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


		// 家事消費金額 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB00080");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB00090");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｄ（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB00113");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB00117");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


		// 事業消費金額 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB00130");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB00140");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB00150");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｄ（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB00173");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB00177");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


		// 雑収入 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB00190");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB00200");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB00210");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｄ（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB00233");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB00237");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


		// 未成熟果樹収入 --------------------------------------------
		Rct++;
		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB00250");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｄ（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB00273");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB00277");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


		// 小計 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB00290");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		//pShLstVal->itmId = _T("AUB0300");		// 230726Aw del
		pShLstVal->itmId = _T("AUB00300");		// 230726Aw add
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB00310");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｄ（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB00333");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB00337");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


		// 農産物の棚卸高（期首） --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB00350");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


		// 農産物の棚卸高（期末） --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB00370");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


		// 計 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB00390");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


	// 経費
		// 租税公課 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00020");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00030");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00040");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00067");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00068");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


		// 種苗費 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00080");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00090");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00100");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｄ（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00123");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();
		
		// Ｅ　（経過措置適用　6.24％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00124");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00127");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00128");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


		// 素畜費 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00140");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00150");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00160");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00187");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00188");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


		// 肥料費 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00200");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00210");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00220");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｄ（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00243");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｅ　（経過措置適用　6.24％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00244");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00247");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00248");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


		// 飼料費 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00260");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00270");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00280");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｄ（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00303");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｅ　（経過措置適用　6.24％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00304");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00307");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00308");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


		// 農具費 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00320");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00330");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00357");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00358");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


		// 農薬・衛生費 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00370");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00380");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00407");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00408");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


		// 諸材料費 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00420");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00430");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00457");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00458");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


		// 修繕費 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00470");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00480");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00507");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00508");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


		// 動力光熱費 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00520");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00530");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00540");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00567");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00568");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


		// 作業用衣料費 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00580");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00590");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00617");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00618");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


		// 農業共済掛金 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00630");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00640");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


		// 減価償却費 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00660");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00670");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


		// 荷造運賃手数料 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00690");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00700");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00710");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00737");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00738");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


		// 雇人費 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00750");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00760");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00770");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00797");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00798");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


		// 利子割引料 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00810");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00820");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


		// 地代・賃貸料 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00840");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00850");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00860");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00887");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00888");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


		// 土地改良費 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00900");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00910");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00920");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00947");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00948");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


		// 貸倒金 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00960");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC00970");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


		// 任意科目（経費） --------------------------------------------
		for (int jj = 0; jj < 3; jj++) {
			Rct++;
			// 項目名
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
			//pShLstVal->itmId.Format(_T("AME0990%d"), jj + 1);	// 230725Aw del
			pShLstVal->itmId.Format(_T("AUC0990%d"), jj + 1);	// 230725Aw add
			pShLstVal->val.Empty();
			pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
			m_Util.char_to_cstring(&pShLstVal->ksname, (unsigned char*)NRec.KNval[Rct].KKnam, 40);
			pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
			pShLstVal->Update();

			// Ａ（決算額）
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
// 230725Aw del -->
			//pShLstVal->itmId.Format(_T("AME1000%d"), jj + 1);
			//m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
// 230725Aw del <--
// 230725Aw add -->
			pShLstVal->itmId.Format(_T("AUC1000%d"), jj + 1);
			m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
// 230725Aw add <--
			pShLstVal->val = buf;
			pShLstVal->val.TrimLeft();		// スペースカット
			pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
			pShLstVal->Update();

			// Ｂ（Ａのうち課税取引にならないもの）
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
			//pShLstVal->itmId.Format(_T("AME1010%d"), jj + 1);	// 230725Aw del
			pShLstVal->itmId.Format(_T("AUC1010%d"), jj + 1);	// 230725Aw add
			m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
			pShLstVal->val = buf;
			pShLstVal->val.TrimLeft();		// スペースカット
			pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
			pShLstVal->Update();

			// Ｃ（課税取引金額(A-B)）
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
			//pShLstVal->itmId.Format(_T("AME1020%d"), jj + 1);	// 230725Aw del
			pShLstVal->itmId.Format(_T("AUC1020%d"), jj + 1);	// 230725Aw add
			m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
			pShLstVal->val = buf;
			pShLstVal->val.TrimLeft();		// スペースカット
			pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
			pShLstVal->Update();

			// Ｄ（うち軽減税率6.24％適用分）
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
			//pShLstVal->itmId.Format(_T("AME1043%d"), jj + 1);	// 230725Aw del
			pShLstVal->itmId.Format(_T("AUC1043%d"), jj + 1);	// 230725Aw add
			m_Arith.l_print(buf, NRec.KNval[Rct].KKval5, "SSSSSSSSSSSSSSS");
			pShLstVal->val = buf;
			pShLstVal->val.TrimLeft();		// スペースカット
			pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
			pShLstVal->Update();

			// Ｅ（経過措置適用　6.24％）
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
			//pShLstVal->itmId.Format(_T("AME1044%d"), jj + 1);	// 230725Aw del
			pShLstVal->itmId.Format(_T("AUC1044%d"), jj + 1);	// 230725Aw add
			m_Arith.l_print(buf, NRec.KNval[Rct].KKval7, "SSSSSSSSSSSSSSS");
			pShLstVal->val = buf;
			pShLstVal->val.TrimLeft();		// スペースカット
			pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
			pShLstVal->Update();

			// Ｆ（うち標準税率7.8％適用分）
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
			//pShLstVal->itmId.Format(_T("AME1047%d"), jj + 1);	// 230725Aw del
			pShLstVal->itmId.Format(_T("AUC1047%d"), jj + 1);	// 230725Aw add
			m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
			pShLstVal->val = buf;
			pShLstVal->val.TrimLeft();		// スペースカット
			pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
			pShLstVal->Update();

			// Ｇ　（経過措置適用　7.8％）
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
			//pShLstVal->itmId.Format(_T("AME1048%d"), jj + 1);	// 230725Aw del
			pShLstVal->itmId.Format(_T("AUC1048%d"), jj + 1);	// 230725Aw add
			m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
			pShLstVal->val = buf;
			pShLstVal->val.TrimLeft();		// スペースカット
			pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
			pShLstVal->Update();
		}

		// 雑費 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC01060");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC01070");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC01080");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｄ（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC01103");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｅ　（経過措置適用　6.24％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC01104");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC01107");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC01108");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


		// 小計 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC01120");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC01130");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC01140");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｄ（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC01163");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｅ　（経過措置適用　6.24％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC01164");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC01167");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC01168");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


		// 農産物以外の棚卸高（期首） --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC01180");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


		// 農産物以外の棚卸高（期末） --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC01200");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


		// 経費から差し引く果樹牛馬等の育成費用 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC01220");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


		// 計 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUC01240");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


	// 差引金額
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUD00010");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// 小計（返還等対価）(※itmIdは存在しない) -----------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB01290");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB01300");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB01310");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｄ（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB01333");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_02_HYOID;
		pShLstVal->itmId = _T("AUB01337");
		m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KNval[Rct].KKtype;
		pShLstVal->Update();


		// ----------------------------------------------
		// 課税取引金額計算表(不動産所得用)

	// 収入金額
		// 賃貸料 --------------------------------------------
		Rct = 0;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOB00020");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOB00030");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOB00040");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOB00067");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// 礼金・権利金・更新料 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOB00080");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOB00090");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOB00100");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOB00127");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// 任意科目 --------------------------------------------
		Rct++;
		// 項目名
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId.Format(_T("AOB00140"));
		pShLstVal->val.Empty();
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		m_Util.char_to_cstring(&pShLstVal->ksname, (unsigned char*)NRec.KFval[Rct].KKnam, 40);
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOB00150");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOB00160");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOB00170");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｄ（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOB00193");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOB00197");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// 計 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOB00210");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOB00220");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOB00230");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｄ（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOB00263");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOB00270");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();


	// 経費 --------------------------------------------
		// 租税公課 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00020");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00030");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00040");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00067");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00068");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();


		// 損害保険料 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00080");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00090");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// 修繕費 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00110");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00120");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00147");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00148");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();


		// 減価償却費 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00160");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00170");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();


		// 借入金利子 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00190");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00200");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();


		// 地代家賃 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00220");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00230");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00240");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00267");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00268");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();


		// 給料賃金 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00280");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00290");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00300");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00327");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00328");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();


		// 任意科目 --------------------------------------------
		Rct++;
		//項目名
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId.Format(_T("AOC00340"));
		pShLstVal->val.Empty();
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		m_Util.char_to_cstring(&pShLstVal->ksname, (unsigned char*)NRec.KFval[Rct].KKnam, 40);
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00350");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00360");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00370");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｄ（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00393");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｅ　（経過措置適用　6.24％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00394");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00397");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00398");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();


		// その他の経費 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00410");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00420");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00430");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｄ（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00453");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｅ　（経過措置適用　6.24％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00454");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00457");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00458");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();


		// 計 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00470");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00480");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00490");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｄ（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00520");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｅ　（経過措置適用　6.24％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00521");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00530");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｇ　（経過措置適用　7.8％）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOC00531");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();


	// 差引金額 --------------------------------------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOD00010");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();


		// 計（返還等対価）(※itmIdは存在しない) -----------------
		Rct++;
		// Ａ（決算額）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOB01210");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｂ（Ａのうち課税取引にならないもの）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOB01220");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｃ（課税取引金額(A-B)）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOB01230");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｄ（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOB01263");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();

		// Ｆ（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_03_HYOID;
		pShLstVal->itmId = _T("AOB01270");
		m_Arith.l_print(buf, NRec.KFval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KFval[Rct].KKtype;
		pShLstVal->Update();


	// ----------------------------------------------
		// 課税売上高計算表
		Rct = 0;

		// (3)任意科目　名称 --------------------------------------------
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId.Format(_T("AQD00010"));
		pShLstVal->val.Empty();
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		m_Util.char_to_cstring(&pShLstVal->ksname, (unsigned char*)Head.KHkunm, 40);
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		// (1)事業所得に係る課税売上高 --------------------------------------------
		// 営業等課税売上高 --------------------------------------------
		//Rct++;
		//金額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQB00040");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		//うち軽減税率6.24％適用分
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQB00070");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		//うち標準税率7.8％適用分
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQB00080");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		// 農業課税売上高 --------------------------------------------
		Rct++;
		//金額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQB00100");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		//うち軽減税率6.24％適用分
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQB00120");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		//うち標準税率7.8％適用分
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQB00130");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		// (2)不動産所得に係る課税売上高 --------------------------------------------
		// 課税売上高 --------------------------------------------
		Rct++;
		//金額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQC00040");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		//うち軽減税率6.24％適用分
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQC00070");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		//うち標準税率7.8％適用分
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQC00080");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();


		// (3)所得に係る課税売上高 --------------------------------------------
		// 損益計算書の収入金額 --------------------------------------------
		Rct++;
		//金額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQD00020");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		//うち軽減税率6.24％適用分
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQD00025");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		//うち標準税率7.8％適用分 
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQD00027");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		// ④のうち、課税売上にならないもの --------------------------------------------
		Rct++;
		//金額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQD00030");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		//うち軽減税率6.24％適用分
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQD00035");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		//うち標準税率7.8％適用分
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQD00037");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		// 差引課税売上高 --------------------------------------------
		Rct++;
		//金額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQD00050");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		//うち軽減税率6.24％適用分
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQD00080");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		//うち標準税率7.8％適用分
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQD00090");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		// (4)業務用資産の譲渡所得に係る課税売上高 --------------------------------------------
		// 業務用固定資産等の譲渡収入金額 --------------------------------------------
		Rct++;
		//金額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQE00010");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		//うち軽減税率6.24％適用分
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQE00015");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		//うち標準税率7.8％適用分
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQE00017");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		// ⑦のうち、課税売上にならないもの --------------------------------------------
		Rct++;
		//金額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQE00020");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		//うち軽減税率6.24％適用分
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQE00025");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		//うち標準税率7.8％適用分
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQE00027");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();


		// 差引課税売上高 --------------------------------------------
		Rct++;
		//金額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQE00040");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		//うち軽減税率6.24％適用分
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQE00070");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		//うち標準税率7.8％適用分
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		//pShLstVal->itmId = _T("AQD00080");	// 230727Aw del
		pShLstVal->itmId = _T("AQE00080");		// 230727Aw add
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		// (5)課税売上高の合計額 --------------------------------------------
		Rct++;
		//金額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQF00010");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		//うち軽減税率6.24％適用分
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQF00040");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		//うち標準税率7.8％適用分
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQF00050");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		Rct++;
		// (6)課税資産の譲渡等の対価の額の計算 --------------------------------------------
		// （軽減税率6.24％適用分） --------------------------------------------
		Rct++;
		// 1円未満の端数切捨て
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQG00044");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		// ○○円×100/108
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQG00042");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		// 仮受消費税等(上段)
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQG00041");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		// （標準税率7.8％適用分） --------------------------------------------
		Rct++;
		// 1円未満の端数切捨て
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQG00048");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		// ○○円×100/108
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQG00046");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

		// 仮受消費税等(上段)
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_04_HYOID;
		pShLstVal->itmId = _T("AQG00045");
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		pShLstVal->Update();

	// -----------------------------------------------------------------------------
		// 課税仕入高計算表
		Rct = 0;

		// (3)任意科目　名称 --------------------------------------------
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId.Format(_T("ASD00010"));
		pShLstVal->val.Empty();
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		m_Util.char_to_cstring(&pShLstVal->ksname, (unsigned char*)Head.KHksnm, 40);
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();


		// (1)事業所得に係る課税仕入高 --------------------------------------------
		// 営業等課税仕入高 --------------------------------------------
		//Rct++;	// 230728Aw del
		//金額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASB00040");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//課税仕入高（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASB00070");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//経過措置適用（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASB00071");
		//m_Arith.l_print(buf, NRec.KSval[Rct].KKval6, "SSSSSSSSSSSSSSS");	// 230728Aw del
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw add
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//課税仕入高（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASB00080");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//経過措置適用（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASB00081");
		//m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw del
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval8, "SSSSSSSSSSSSSSS");	// 230728Aw add
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();


		// 農業課税仕入高 --------------------------------------------
		Rct++;
		//金額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASB00100");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//課税仕入高（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASB00120");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//経過措置適用（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASB00121");
		//m_Arith.l_print(buf, NRec.KSval[Rct].KKval6, "SSSSSSSSSSSSSSS");	// 230728Aw del
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw add
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//課税仕入高（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASB00130");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//経過措置適用（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASB00131");
		//m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw del
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval8, "SSSSSSSSSSSSSSS");	// 230728Aw add
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();


		// (2)不動産所得に係る課税仕入高 --------------------------------------------
		// 課税仕入高 --------------------------------------------
		Rct++;
		//金額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASC00040");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//課税仕入高（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASC00070");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//経過措置適用（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASC00071");
		//m_Arith.l_print(buf, NRec.KSval[Rct].KKval6, "SSSSSSSSSSSSSSS");	// 230728Aw del
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw add
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//課税仕入高（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASC00080");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//経過措置適用（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASC00081");
		//m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw del
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval8, "SSSSSSSSSSSSSSS");	// 230728Aw add
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		// (3)所得に係る課税仕入高 --------------------------------------------
		// 損益計算書の仕入金額と経費の金額の合計額 --------------------------------------------
		Rct++;
		//金額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASD00020");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//課税仕入高（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASD00025");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//経過措置適用（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASD00026");
		//m_Arith.l_print(buf, NRec.KSval[Rct].KKval6, "SSSSSSSSSSSSSSS");	// 230728Aw del
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw add
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//課税仕入高（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASD00027");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//経過措置適用（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASD00028");
		//m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw del
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval8, "SSSSSSSSSSSSSSS");	// 230728Aw add
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();


		// ④のうち、課税仕入にならないもの --------------------------------------------
		Rct++;
		//金額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASD00030");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//課税仕入高（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASD00035");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//経過措置適用（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASD00036");
		//m_Arith.l_print(buf, NRec.KSval[Rct].KKval6, "SSSSSSSSSSSSSSS");	// 230728Aw del
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw add
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//課税仕入高（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASD00037");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//経過措置適用（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASD00038");
		//m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw del
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval8, "SSSSSSSSSSSSSSS");	// 230728Aw add
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();


		// 差引課税仕入高 --------------------------------------------
		Rct++;
		//金額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASD00050");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//課税仕入高（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASD00080");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//経過措置適用（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASD00081");
		//m_Arith.l_print(buf, NRec.KSval[Rct].KKval6, "SSSSSSSSSSSSSSS");	// 230728Aw del
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw add
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//課税仕入高（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASD00090");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//経過措置適用（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASD00091");
		//m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw del
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval8, "SSSSSSSSSSSSSSS");	// 230728Aw add
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();


		// (4)業務用資産の取得に係る課税仕入高 --------------------------------------------
		// 業務用固定資産等の取得費 --------------------------------------------
		Rct++;
		//金額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASE00010");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//課税仕入高（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASE00015");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//経過措置適用（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASE00016");
		//m_Arith.l_print(buf, NRec.KSval[Rct].KKval6, "SSSSSSSSSSSSSSS");	// 230728Aw del
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw add
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//課税仕入高（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASE00017");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//経過措置適用（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASE00018");
		//m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw del
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval8, "SSSSSSSSSSSSSSS");	// 230728Aw add
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		// ⑦のうち、課税仕入にならないもの --------------------------------------------
		Rct++;
		//金額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASE00020");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//課税仕入高（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASE00025");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//経過措置適用（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASE00026");
		//m_Arith.l_print(buf, NRec.KSval[Rct].KKval6, "SSSSSSSSSSSSSSS");	// 230728Aw del
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw add
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//課税仕入高（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASE00027");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//経過措置適用（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASE00028");
		//m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw del
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval8, "SSSSSSSSSSSSSSS");	// 230728Aw add
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();


		// 差引課税仕入高 --------------------------------------------
		Rct++;
		//金額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASE00040");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//課税仕入高（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASE00070");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//経過措置適用（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASE00071");
		//m_Arith.l_print(buf, NRec.KSval[Rct].KKval6, "SSSSSSSSSSSSSSS");	// 230728Aw del
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw add
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//課税仕入高（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASE00080");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//経過措置適用（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASE00081");
		//m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw del
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval8, "SSSSSSSSSSSSSSS");	// 230728Aw add
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();


		// 課税仕入高の合計額 --------------------------------------------
		Rct++;
		//金額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASF00010");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//課税仕入高（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASF00040");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//経過措置適用（うち軽減税率6.24％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASF00041");
		//m_Arith.l_print(buf, NRec.KSval[Rct].KKval6, "SSSSSSSSSSSSSSS");	// 230728Aw del
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw add
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//課税仕入高（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASF00050");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		//経過措置適用（うち標準税率7.8％適用分）
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASF00051");
		//m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw del
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval8, "SSSSSSSSSSSSSSS");	// 230728Aw add
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		// (6)課税仕入に係る消費税額の計算 --------------------------------------------
		Rct++;

		// （軽減税率6.24％適用分） --------------------------------------------
		Rct++;
		// 金額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASG00044");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		// ○円×6.24/108
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASG00042");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		// 仮払消費税等
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASG00041");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

// 230728Aw del -->
		//// 経過措置適用（軽減税率6.24％適用分） --------------------------------------------
		//// 金額
		//pShLstVal->AddNew();
		//pShLstVal->sn_seq = sn_seq;
		//pShLstVal->version = nowVersion;
		//pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		//pShLstVal->itmId = _T("ASG00144");
		//m_Arith.l_print(buf, NRec.KSval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		//pShLstVal->val = buf;
		//pShLstVal->val.TrimLeft();		// スペースカット
		//pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		//pShLstVal->ksname.Empty();
		//pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		//pShLstVal->Update();

		//// ○円×6.24/108×80%
		//pShLstVal->AddNew();
		//pShLstVal->sn_seq = sn_seq;
		//pShLstVal->version = nowVersion;
		//pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		//pShLstVal->itmId = _T("ASG00142");
		//m_Arith.l_print(buf, NRec.KSval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		//pShLstVal->val = buf;
		//pShLstVal->val.TrimLeft();		// スペースカット
		//pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		//pShLstVal->ksname.Empty();
		//pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		//pShLstVal->Update();

		//// 仮払消費税等
		//pShLstVal->AddNew();
		//pShLstVal->sn_seq = sn_seq;
		//pShLstVal->version = nowVersion;
		//pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		//pShLstVal->itmId = _T("ASG00141");
		//m_Arith.l_print(buf, NRec.KSval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		//pShLstVal->val = buf;
		//pShLstVal->val.TrimLeft();		// スペースカット
		//pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		//pShLstVal->ksname.Empty();
		//pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		//pShLstVal->Update();
// 230728Aw del <--

		// （標準税率7.8％適用分） --------------------------------------------
		Rct++;

		// 金額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASG00048");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		// ○円×7.8/108
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASG00046");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		// 仮払消費税等
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASG00045");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

// 230728Aw add -->
		// 経過措置適用（軽減税率6.24％適用分） --------------------------------------------
		Rct++;
		// 金額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASG00144");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		// ○円×6.24/108×80%
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASG00142");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		// 仮払消費税等
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASG00141");
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();
// 230728Aw add <--

		// 経過措置適用（標準税率7.8％適用分） --------------------------------------------
		Rct++;	// 230728Aw add

		// 金額
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASG00148");
		//m_Arith.l_print(buf, NRec.KSval[Rct].KKval4, "SSSSSSSSSSSSSSS");	// 230728Aw del
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval1, "SSSSSSSSSSSSSSS");	// 230728Aw add
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		// ○円×7.8/108×80%
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASG00146");
		//m_Arith.l_print(buf, NRec.KSval[Rct].KKval5, "SSSSSSSSSSSSSSS");	// 230728Aw del
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval2, "SSSSSSSSSSSSSSS");	// 230728Aw add
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

		// 仮払消費税等
		pShLstVal->AddNew();
		pShLstVal->sn_seq = sn_seq;
		pShLstVal->version = nowVersion;
		pShLstVal->hyoId = ICS_TAXLIST_05_HYOID;
		pShLstVal->itmId = _T("ASG00145");
		//m_Arith.l_print(buf, NRec.KSval[Rct].KKval6, "SSSSSSSSSSSSSSS");	// 230728Aw del
		m_Arith.l_print(buf, NRec.KSval[Rct].KKval3, "SSSSSSSSSSSSSSS");	// 230728Aw add
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
		pShLstVal->ksname.Empty();
		pShLstVal->kssgn1 = NRec.KSval[Rct].KKtype;
		pShLstVal->Update();

// 240220 戎居 del -->
//// 230728Aw add -->
//	// -----------------------------------------------------------------------------
//		// 事業別売上計算表 -------------------------------------------------------
//		if(m_Util.IsUnConnectMst(pDBNpSub->zvol) && (m_Util.GetKazeihoushiki(pDBNpSub->zvol) == ID_ICSSH_KANNI)) {
//			Rct = 40;
//
//			// 第一種事業(卸売業)
//			// 決定額
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAA00010");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 課税取引外
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAA00011");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 返還等課税取引
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAA00013");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// うち税率7.8％適用分
//			// 課税取引金額
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAA00022");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 返還等課税取引
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAA00023");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// うち税率6.24％適用分
//			// 課税取引金額
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAA00032");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 返還等課税取引
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAA00033");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 第二種事業(小売業等)
//			Rct++;
//			// 決定額
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAB00010");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 課税取引外
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAB00011");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 返還等課税取引
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAB00013");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// うち税率7.8％適用分
//			// 課税取引金額
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAB00022");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 返還等課税取引
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAB00023");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// うち税率6.24％適用分
//			// 課税取引金額
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAB00032");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 返還等課税取引
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAB00033");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 第三種事業(製造業等)
//			Rct++;
//			// 決定額
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAC00010");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 課税取引外
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAC00011");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 返還等課税取引
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAC00013");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// うち税率7.8％適用分
//			// 課税取引金額
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAC00022");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 返還等課税取引
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAC00023");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// うち税率6.24％適用分
//			// 課税取引金額
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAC00032");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 返還等課税取引
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAC00033");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 第三種事業(製造業等)／農業事業者
//			Rct++;
//			// 決定額
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAC00040");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 課税取引外
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAC00041");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 返還等課税取引
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAC00043");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// うち税率7.8％適用分
//			// 課税取引金額
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAC00052");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 返還等課税取引
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAC00053");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 第四種事業(その他)
//			Rct++;
//			// 決定額
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAD00010");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 課税取引外
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAD00011");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 返還等課税取引
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAD00013");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// うち税率7.8％適用分
//			// 課税取引金額
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAD00022");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 返還等課税取引
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAD00023");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// うち税率6.24％適用分
//			// 課税取引金額
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAD00032");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 返還等課税取引
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAD00033");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 第五種事業(サービス業等)
//			Rct++;
//			// 決定額
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAE00010");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 課税取引外
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAE00011");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 返還等課税取引
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAE00013");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// うち税率7.8％適用分
//			// 課税取引金額
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAE00022");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 返還等課税取引
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAE00023");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// うち税率6.24％適用分
//			// 課税取引金額
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAE00032");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 返還等課税取引
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAE00033");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 第六種事業(不動産業)
//			Rct++;
//			// 決定額
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAF00010");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 課税取引外
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAF00011");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 返還等課税取引
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAF00013");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// うち税率7.8％適用分
//			// 課税取引金額
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAF00022");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 返還等課税取引
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAF00023");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// うち税率6.24％適用分
//			// 課税取引金額
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAF00032");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 返還等課税取引
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAF00033");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 合計
//			Rct++;
//			// 決定額
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAG00010");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 課税取引外
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAG00011");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 課税取引金額
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAG00012");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 返還等課税取引
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAG00013");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// うち税率7.8％適用分
//			// 課税取引金額
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAG00022");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 返還等課税取引
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAG00023");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// うち税率6.24％適用分
//			// 課税取引金額
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAG00032");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 返還等課税取引
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAG00033");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 第四事業（その他）／固定資産等の譲渡
//			Rct++;
//			// 決定額
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAD00040");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 課税取引外
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAD00041");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 返還等課税取引
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAD00043");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// うち税率7.8％適用分
//			// 課税取引金額
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAD00052");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			//// 返還等課税取引
//			//pShLstVal->AddNew();
//			//pShLstVal->sn_seq = sn_seq;
//			//pShLstVal->version = nowVersion;
//			//pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			//pShLstVal->itmId = _T("ZAD00053");
//			//m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
//			//pShLstVal->val = buf;
//			//pShLstVal->val.TrimLeft();		// スペースカット
//			//pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			//pShLstVal->ksname.Empty();
//			//pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			//pShLstVal->Update();
//
//			// うち税率6.24％適用分
//			// 課税取引金額
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAD00062");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			//// 返還等課税取引
//			//pShLstVal->AddNew();
//			//pShLstVal->sn_seq = sn_seq;
//			//pShLstVal->version = nowVersion;
//			//pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			//pShLstVal->itmId = _T("ZAD00063");
//			//m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
//			//pShLstVal->val = buf;
//			//pShLstVal->val.TrimLeft();		// スペースカット
//			//pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			//pShLstVal->ksname.Empty();
//			//pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			//pShLstVal->Update();
//
//			// 第二種事業(小売業等)／農業事業者
//			Rct++;
//			// 決定額
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAB00040");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 課税取引外
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAB00041");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 返還等課税取引
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAB00043");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// うち税率6.24％適用分
//			// 課税取引金額
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAB00052");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//
//			// 返還等課税取引
//			pShLstVal->AddNew();
//			pShLstVal->sn_seq = sn_seq;
//			pShLstVal->version = nowVersion;
//			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
//			pShLstVal->itmId = _T("ZAB00053");
//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
//			pShLstVal->val = buf;
//			pShLstVal->val.TrimLeft();		// スペースカット
//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//			pShLstVal->ksname.Empty();
//			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
//			pShLstVal->Update();
//		}
//// 230728Aw add <--
// 240220 戎居 del <--
	}
	else {															// データあり
		CStringArray	updTagAry;

		for(ii = 0; ; ii++) {
			if(ii == 0) {
				if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {
					break;
				}
			}
			else {
				if((pShLstVal->MoveNext() == -1) || (pShLstVal->st == -1)) {
					break;
				}
			}

			// 課税取引金額計算表(事業所得用)-------------------------------------------
			if(pShLstVal->hyoId == ICS_TAXLIST_01_HYOID) {
				// 売上(収入)金額 ------------------------------------------------------
				Rct = 0;
				// Ａ（決定額）
				if(pShLstVal->itmId == _T("AMB00010")) {
					pShLstVal->Edit();
					m_Arith.l_print( buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS" );
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AMB00020")) {
					pShLstVal->Edit();
					m_Arith.l_print( buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS" );
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AMB00030")) {
					pShLstVal->Edit();
					m_Arith.l_print( buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS" );
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("AMB00060")) {
					pShLstVal->Edit();
					m_Arith.l_print( buf, NRec.KJval[Rct].KKval5, "SSSSSSSSSSSSSSS" );
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｅ
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AMB00070")) {
					pShLstVal->Edit();
					m_Arith.l_print( buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS" );
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ

				// 売上原価／期首商品棚卸高 --------------------------------------------
				Rct++;
				// Ａ（決定額）
				if(pShLstVal->itmId == _T("AMC00020")) {
					pShLstVal->Edit();
					m_Arith.l_print( buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS" );
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 売上原価／仕入金額 --------------------------------------------
				Rct++;
				// Ａ（決定額）
				if(pShLstVal->itmId == _T("AMC00040")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AMC00050")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AMC00060")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("AMC00083")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval5, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｅ（経過措置適用　6.24％）
				else if(pShLstVal->itmId == _T("AMC00084")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval7, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AMC00087")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AMC00088")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 売上原価／小計 --------------------------------------------
				Rct++;
				// Ａ（決定額）
				if(pShLstVal->itmId == _T("AMC00100")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 売上原価／期末棚卸高 --------------------------------------------
				Rct++;
				// Ａ（決定額）
				if(pShLstVal->itmId == _T("AMC00120")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 売上原価／差引原価 --------------------------------------------
				Rct++;
				// Ａ（決定額）
				if(pShLstVal->itmId == _T("AMC00140")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 差引原価 --------------------------------------------
				Rct++;
				// Ａ（決定額）
				if(pShLstVal->itmId == _T("AMD00010")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 経費／租税公課 --------------------------------------------
				Rct++;
				// Ａ（決定額）
				if(pShLstVal->itmId == _T("AMD00020")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AMD00030")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AMD00040")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				// Ｅ
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AMD00067")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AMD00068")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 経費／荷造運賃 --------------------------------------------
				Rct++;
				// Ａ（決定額）
				if(pShLstVal->itmId == _T("AME00080")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AME00090")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AME00100")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				// Ｅ
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AME00127")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AME00128")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 経費／水道光熱費 --------------------------------------------
				Rct++;
				// Ａ（決定額）
				if(pShLstVal->itmId == _T("AME00140")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AME00150")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				// Ｅ
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AME00177")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AME00178")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 経費／旅費交通費 --------------------------------------------
				Rct++;
				// Ａ（決定額）
				if(pShLstVal->itmId == _T("AME00190")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AME00200")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AME00210")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				// Ｅ
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AME00237")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AME00238")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 経費／通信費 --------------------------------------------
				Rct++;
				// Ａ（決定額）
				if(pShLstVal->itmId == _T("AME00250")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AME00260")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AME00270")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("AME00293")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval5, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｅ（経過措置適用　6.24％）
				else if(pShLstVal->itmId == _T("AME00294")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval7, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AME00297")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AME00298")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 経費／広告宣伝費 --------------------------------------------
				Rct++;
				// Ａ（決定額）
				if(pShLstVal->itmId == _T("AME00310")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AME00320")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AME00330")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("AME00353")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval5, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｅ（経過措置適用　6.24％）
				else if(pShLstVal->itmId == _T("AME00354")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval7, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AME00357")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AME00358")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 経費／接待交際費 --------------------------------------------
				Rct++;
				// Ａ（決定額）
				if(pShLstVal->itmId == _T("AME00370")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AME00380")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AME00390")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("AME00413")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval5, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｅ（経過措置適用　6.24％）
				else if(pShLstVal->itmId == _T("AME00414")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval7, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AME00417")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AME00418")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 経費／損害保険料 --------------------------------------------
				Rct++;
				// Ａ（決定額）
				if(pShLstVal->itmId == _T("AME00430")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AME00440")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				// Ｄ（うち軽減税率6.24％適用分）
				// Ｅ
				// Ｆ（うち標準税率7.8％適用分）
				// Ｇ

				// 経費／修繕費 --------------------------------------------
				Rct++;
				// Ａ（決定額）
				if(pShLstVal->itmId == _T("AME00460")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AME00470")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				// Ｅ
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AME00497")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AME00498")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 経費／消耗品費 --------------------------------------------
				Rct++;
				// Ａ（決定額）
				if(pShLstVal->itmId == _T("AME00510")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AME00520")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("AME00543")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval5, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｅ（経過措置適用　6.24％）
				else if(pShLstVal->itmId == _T("AME00544")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval7, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AME00547")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AME00548")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 経費／減価償却費 --------------------------------------------
				Rct++;
				// Ａ（決定額）
				if(pShLstVal->itmId == _T("AME00560")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AME00570")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				// Ｄ（うち軽減税率6.24％適用分）
				// Ｅ
				// Ｆ（うち標準税率7.8％適用分）
				// Ｇ

				// 経費／福利厚生費 --------------------------------------------
				Rct++;
				// Ａ（決定額）
				if(pShLstVal->itmId == _T("AME00590")) {
					pShLstVal->Edit();	
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AME00600")) {
					pShLstVal->Edit();	
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AME00610")) {
					pShLstVal->Edit();	
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("AME00633")) {
					pShLstVal->Edit();	
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval5, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｅ（経過措置適用　6.24％）
				else if(pShLstVal->itmId == _T("AME00634")) {
					pShLstVal->Edit();	
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval7, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AME00637")) {
					pShLstVal->Edit();	
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ
				else if(pShLstVal->itmId == _T("AME00638")) {
					pShLstVal->Edit();	
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 経費／給料賃金 --------------------------------------------
				Rct++;
				// Ａ（決定額）
				if(pShLstVal->itmId == _T("AME00650")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AME00660")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AME00670")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				// Ｅ
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AME00697")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AME00698")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
	
				// 経費／外注工賃 --------------------------------------------
				Rct++;
				// Ａ（決定額）
				if(pShLstVal->itmId == _T("AME00710")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AME00720")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AME00724")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				// Ｅ
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AME01160")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AME01161")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 経費／利子割引料 --------------------------------------------
				Rct++;
				// Ａ（決定額）
				if(pShLstVal->itmId == _T("AME00740")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AME00750")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				// Ｄ（うち軽減税率6.24％適用分）
				// Ｅ
				// Ｆ（うち標準税率7.8％適用分）
				// Ｇ

				// 経費／地代家賃 --------------------------------------------
				Rct++;
				// Ａ（決定額）
				if(pShLstVal->itmId == _T("AME00800")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AME00810")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AME00814")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				// Ｅ
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AME01180")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AME01181")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
	
				// 経費／貸倒金 --------------------------------------------
				Rct++;
				// Ａ（決定額）
				if(pShLstVal->itmId == _T("AME00830")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AME00840")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				// Ｄ（うち軽減税率6.24％適用分）
				// Ｅ
				// Ｆ（うち標準税率7.8％適用分）
				// Ｇ

				// 経費／任意科目１ --------------------------------------------
				for(int jj = 0; jj < 6; jj++) {
					Rct++;
					CString cs;
					// 項目名
					cs.Format(_T("AME0950%d"), jj+1);
					if(pShLstVal->itmId == cs) {
						pShLstVal->Edit();
						m_Util.char_to_cstring(&pShLstVal->ksname, (unsigned char*)NRec.KJval[Rct].KKnam, 40);
						pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
						pShLstVal->Update();
						updTagAry.Add(pShLstVal->itmId);
					}
					// Ａ（決定額）
					cs.Format(_T("AME0960%d"), jj+1);
					if(pShLstVal->itmId == cs) {
						pShLstVal->Edit();
						m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
						pShLstVal->val = buf;
						pShLstVal->val.TrimLeft();		// スペースカット
						pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
						pShLstVal->Update();
						updTagAry.Add(pShLstVal->itmId);
					}
					// Ｂ（Ａのうち課税取引にならないもの）
					cs.Format(_T("AME0970%d"), jj+1);
					if(pShLstVal->itmId == cs) {
						pShLstVal->Edit();
						m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
						pShLstVal->val = buf;
						pShLstVal->val.TrimLeft();		// スペースカット
						pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
						pShLstVal->Update();
						updTagAry.Add(pShLstVal->itmId);
					}
					// Ｃ（課税取引金額(A-B)）
					cs.Format(_T("AME0980%d"), jj+1);
					if(pShLstVal->itmId == cs) {
						pShLstVal->Edit();
						m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
						pShLstVal->val = buf;
						pShLstVal->val.TrimLeft();		// スペースカット
						pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
						pShLstVal->Update();
						updTagAry.Add(pShLstVal->itmId);
					}
					// Ｄ（うち軽減税率6.24％適用分）
					cs.Format(_T("AME1003%d"), jj+1);
					if(pShLstVal->itmId == cs) {
						pShLstVal->Edit();
						m_Arith.l_print(buf, NRec.KJval[Rct].KKval5, "SSSSSSSSSSSSSSS");
						pShLstVal->val = buf;
						pShLstVal->val.TrimLeft();		// スペースカット
						pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
						pShLstVal->Update();
						updTagAry.Add(pShLstVal->itmId);
					}
					// Ｅ（経過措置適用　6.24％）
					cs.Format(_T("AME1004%d"), jj+1);
					if(pShLstVal->itmId == cs) {
						pShLstVal->Edit();
						m_Arith.l_print(buf, NRec.KJval[Rct].KKval7, "SSSSSSSSSSSSSSS");
						pShLstVal->val = buf;
						pShLstVal->val.TrimLeft();		// スペースカット
						pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
						pShLstVal->Update();
						updTagAry.Add(pShLstVal->itmId);
					}
					// Ｆ（うち標準税率7.8％適用分）
					cs.Format(_T("AME1007%d"), jj+1);
					if(pShLstVal->itmId == cs) {
						pShLstVal->Edit();
						m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
						pShLstVal->val = buf;
						pShLstVal->val.TrimLeft();		// スペースカット
						pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
						pShLstVal->Update();
						updTagAry.Add(pShLstVal->itmId);
					}
					// Ｇ（経過措置適用　7.8％）
					cs.Format(_T("AME1008%d"), jj+1);
					if(pShLstVal->itmId == cs) {
						pShLstVal->Edit();
						m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
						pShLstVal->val = buf;
						pShLstVal->val.TrimLeft();		// スペースカット
						pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
						pShLstVal->Update();
						updTagAry.Add(pShLstVal->itmId);
					}
				}

				// 経費／雑費 --------------------------------------------
				Rct++;
				// Ａ（決定額）
				if(pShLstVal->itmId == _T("AME01020")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AME01030")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AME01040")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("AME01063")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval5, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｅ（経過措置適用　6.24％）
				else if(pShLstVal->itmId == _T("AME01064")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval7, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AME01067")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AME01068")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 経費／計 --------------------------------------------
				Rct++;
				// Ａ（決定額）
				if(pShLstVal->itmId == _T("AME01080")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AME01090")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AME01100")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("AME01130")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval5, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｅ（経過措置適用　6.24％）
				else if(pShLstVal->itmId == _T("AME01131")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval7, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AME01140")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AME01141")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 差し引き金額 --------------------------------------------
				Rct++;
				// Ａ（決定額）
				if(pShLstVal->itmId == _T("AMF00010")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				// Ｃ（課税取引金額(A-B)）
				// Ｄ（うち軽減税率6.24％適用分）
				// Ｅ
				// Ｆ（うち標準税率7.8％適用分）
				// Ｇ

				// ３＋３２ --------------------------------------------
				Rct++;
				// Ａ（決定額）
				if(pShLstVal->itmId == _T("AMG00010")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AMG00020")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("AMG00040")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval5, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｅ（経過措置適用　6.24％）
				else if(pShLstVal->itmId == _T("AMG00041")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval7, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AMG00050")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AMG00051")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 売上(収入)金額/下段(※itmIdは存在しないので適当) -----------------
				Rct++;
				// Ａ（決定額）
				if(pShLstVal->itmId == _T("AMB00110")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AMB00120")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AMB00130")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("AMB00160")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval5, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｅ
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AMB00170")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KJval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KJval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ
			}
			// 課税取引金額計算表(農業所得用)
			else if(pShLstVal->hyoId == ICS_TAXLIST_02_HYOID) {
				// 収入金額／販売金額 --------------------------------------------
				Rct = 0;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUB00020")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AUB00030")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AUB00040")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("AUB00063")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval5, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AUB00067")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 収入金額／家事消費金額 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUB00080")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AUB00090")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("AUB00113")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval5, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AUB00117")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 収入金額／事業消費金額 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUB00130")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AUB00140")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AUB00150")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("AUB00173")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval5, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AUB00177")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 収入金額／雑収入 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUB00190")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AUB00200")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AUB00210")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("AUB00233")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval5, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AUB00237")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 収入金額／未成熟果樹収入 --------------------------------------------
				Rct++;
				// Ｃ（課税取引金額(A-B)）
				if(pShLstVal->itmId == _T("AUB00250")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("AUB00273")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval5, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AUB00277")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 収入金額／小計 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUB00290")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				//else if(pShLstVal->itmId == _T("AUB0300")) {	// 230726Aw del
				else if(pShLstVal->itmId == _T("AUB00300")) {	// 230726Aw add
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AUB00310")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("AUB00333")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval5, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AUB00337")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 収入金額／農産物の棚卸高（期首） --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUB00350")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 収入金額／農産物の棚卸高（期末） --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUB00370")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 収入金額／計 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUB00390")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 経費／租税公課 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUC00020")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AUC00030")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AUC00040")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AUC00067")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ　（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AUC00068")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 経費／種苗費 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUC00080")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AUC00090")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AUC00100")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("AUC00123")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval5, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｅ　（経過措置適用　6.24％）
				else if(pShLstVal->itmId == _T("AUC00124")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval7, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AUC00127")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ　（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AUC00128")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 素畜費 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUC00140")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AUC00150")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AUC00160")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AUC00187")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ　（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AUC00188")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 肥料費 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUC00200")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AUC00210")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AUC00220")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("AUC00243")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval5, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｅ　（経過措置適用　6.24％）
				else if(pShLstVal->itmId == _T("AUC00244")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval7, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AUC00247")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ　（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AUC00248")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 飼料費 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUC00260")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AUC00270")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AUC00280")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("AUC00303")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval5, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｅ　（経過措置適用　6.24％）
				else if(pShLstVal->itmId == _T("AUC00304")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval7, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AUC00307")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ　（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AUC00308")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 農具費 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUC00320")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AUC00330")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AUC00357")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ　（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AUC00358")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 農薬・衛生費 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUC00370")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AUC00380")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AUC00407")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ　（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AUC00408")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 諸材料費 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUC00420")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AUC00430")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AUC00457")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ　（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AUC00458")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 修繕費 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUC00470")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AUC00480")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AUC00507")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ　（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AUC00508")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 動力光熱費 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUC00520")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AUC00530")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AUC00540")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AUC00567")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ　（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AUC00568")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 作業用衣料費 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUC00580")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AUC00590")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AUC00617")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ　（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AUC00618")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 農業共済掛金 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUC00630")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AUC00640")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 減価償却費 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUC00660")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AUC00670")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 荷造運賃手数料 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUC00690")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AUC00700")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AUC00710")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AUC00737")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ　（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AUC00738")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 雇人費 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUC00750")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AUC00760")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AUC00770")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AUC00797")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ　（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AUC00798")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 利子割引料 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUC00810")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AUC00820")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 地代・賃貸料 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUC00840")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AUC00850")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AUC00860")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AUC00887")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ　（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AUC00888")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 土地改良費 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUC00900")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AUC00910")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AUC00920")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AUC00947")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ　（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AUC00948")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 貸倒金 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUC00960")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AUC00970")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 任意科目（経費） --------------------------------------------
				for (int jj = 0; jj < 3; jj++) {
					CString	cs;

					Rct++;
					// 項目名
					//cs.Format(_T("AME0990%d"), jj + 1);	// 230725Aw del
					cs.Format(_T("AUC0990%d"), jj + 1);		// 230725Aw add
					if(pShLstVal->itmId == cs) {
						pShLstVal->Edit();
						m_Util.char_to_cstring(&pShLstVal->ksname, (unsigned char*)NRec.KNval[Rct].KKnam, 40);
						pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
						pShLstVal->Update();
						updTagAry.Add(pShLstVal->itmId);
					}
					// Ａ（決算額）
					//cs.Format(_T("AME1000%d"), jj + 1);	// 230725Aw del
					cs.Format(_T("AUC1000%d"), jj + 1);		// 230725Aw add
					if(pShLstVal->itmId == cs) {
						pShLstVal->Edit();
						//m_Arith.l_print(buf, NRec.KJval[Rct].KKval1, "SSSSSSSSSSSSSSS");	// 230726Aw del
						m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");	// 230726Aw add
						pShLstVal->val = buf;
						pShLstVal->val.TrimLeft();		// スペースカット
						pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
						pShLstVal->Update();
						updTagAry.Add(pShLstVal->itmId);
					}
					// Ｂ（Ａのうち課税取引にならないもの）
					//cs.Format(_T("AME1010%d"), jj + 1);	// 230725Aw del
					cs.Format(_T("AUC1010%d"), jj + 1);		// 230725Aw add
					if(pShLstVal->itmId == cs) {
						pShLstVal->Edit();
						m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
						pShLstVal->val = buf;
						pShLstVal->val.TrimLeft();		// スペースカット
						pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
						pShLstVal->Update();
						updTagAry.Add(pShLstVal->itmId);
					}
					// Ｃ（課税取引金額(A-B)）
					//cs.Format(_T("AME1020%d"), jj + 1);	// 230725Aw del
					cs.Format(_T("AUC1020%d"), jj + 1);		// 230725Aw add
					if(pShLstVal->itmId == cs) {
						pShLstVal->Edit();
						m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
						pShLstVal->val = buf;
						pShLstVal->val.TrimLeft();		// スペースカット
						pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
						pShLstVal->Update();
						updTagAry.Add(pShLstVal->itmId);
					}
					// Ｄ（うち軽減税率6.24％適用分）
					//cs.Format(_T("AME1043%d"), jj + 1);	// 230725Aw del
					cs.Format(_T("AUC1043%d"), jj + 1);		// 230725Aw add
					if(pShLstVal->itmId == cs) {
						pShLstVal->Edit();
						m_Arith.l_print(buf, NRec.KNval[Rct].KKval5, "SSSSSSSSSSSSSSS");
						pShLstVal->val = buf;
						pShLstVal->val.TrimLeft();		// スペースカット
						pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
						pShLstVal->Update();
						updTagAry.Add(pShLstVal->itmId);
					}
					// Ｅ　（経過措置適用　6.24％）
					//cs.Format(_T("AME1044%d"), jj + 1);	// 230725Aw del
					cs.Format(_T("AUC1044%d"), jj + 1);		// 230725Aw add
					if(pShLstVal->itmId == cs) {
						pShLstVal->Edit();
						m_Arith.l_print(buf, NRec.KNval[Rct].KKval7, "SSSSSSSSSSSSSSS");
						pShLstVal->val = buf;
						pShLstVal->val.TrimLeft();		// スペースカット
						pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
						pShLstVal->Update();
						updTagAry.Add(pShLstVal->itmId);
					}
					// Ｆ（うち標準税率7.8％適用分）
					//cs.Format(_T("AME1047%d"), jj + 1);	// 230725Aw del
					cs.Format(_T("AUC1047%d"), jj + 1);		// 230725Aw add
					if(pShLstVal->itmId == cs) {
						pShLstVal->Edit();
						m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
						pShLstVal->val = buf;
						pShLstVal->val.TrimLeft();		// スペースカット
						pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
						pShLstVal->Update();
						updTagAry.Add(pShLstVal->itmId);
					}
					// Ｇ　（経過措置適用　7.8％）
					//cs.Format(_T("AME1048%d"), jj + 1);	// 230725Aw del
					cs.Format(_T("AUC1048%d"), jj + 1);		// 230725Aw add
					if(pShLstVal->itmId == cs) {
						pShLstVal->Edit();
						m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
						pShLstVal->val = buf;
						pShLstVal->val.TrimLeft();		// スペースカット
						pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
						pShLstVal->Update();
						updTagAry.Add(pShLstVal->itmId);
					}
				}

				// 雑費 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUC01060")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AUC01070")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AUC01080")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("AUC01103")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval5, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｅ　（経過措置適用　6.24％）
				else if(pShLstVal->itmId == _T("AUC01104")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval7, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AUC01107")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ　（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AUC01108")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 小計 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUC01120")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AUC01130")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AUC01140")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("AUC01163")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval5, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｅ　（経過措置適用　6.24％）
				else if(pShLstVal->itmId == _T("AUC01164")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval7, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AUC01167")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ　（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AUC01168")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 農産物以外の棚卸高（期首） --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUC01180")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 農産物以外の棚卸高（期末） --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUC01200")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 経費から差し引く果樹牛馬等の育成費用 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUC01220")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 計 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUC01240")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 差引金額 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUD00010")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 小計（返還等対価）(※itmIdは存在しない) -----------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AUB01290")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AUB01300")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AUB01310")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("AUB01333")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval5, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AUB01337")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KNval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KNval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
			}
			// 課税取引金額計算表(不動産所得用)
			else if(pShLstVal->hyoId == ICS_TAXLIST_03_HYOID) {
				// 収入金額／賃貸料 --------------------------------------------
				Rct = 0;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AOB00020")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AOB00030")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AOB00040")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AOB00067")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 収入金額／礼金・権利金・更新料 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AOB00080")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AOB00090")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AOB00100")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AOB00127")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 収入金額／任意科目 --------------------------------------------
				Rct++;
				// 項目名
				if(pShLstVal->itmId == _T("AOB00140")) {
					pShLstVal->Edit();
					m_Util.char_to_cstring(&pShLstVal->ksname, (unsigned char*)NRec.KFval[Rct].KKnam, 40);
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ａ（決算額）
				else if(pShLstVal->itmId == _T("AOB00150")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AOB00160")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AOB00170")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("AOB00193")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval5, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AOB00197")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 計 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AOB00210")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AOB00220")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AOB00230")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("AOB00263")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval5, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AOB00270")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 経費／租税公課 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AOC00020")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AOC00030")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AOC00040")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AOC00067")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ　（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AOC00068")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 損害保険料 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AOC00080")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AOC00090")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 修繕費 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AOC00110")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AOC00120")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AOC00147")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ　（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AOC00148")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 減価償却費 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AOC00160")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AOC00170")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 借入金利子 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AOC00190")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AOC00200")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 地代家賃 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AOC00220")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AOC00230")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AOC00240")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AOC00267")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ　（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AOC00268")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 給料賃金 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AOC00280")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AOC00290")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AOC00300")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AOC00327")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ　（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AOC00328")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 任意科目 --------------------------------------------
				Rct++;
				//項目名
				if(pShLstVal->itmId == _T("AOC00340")) {
					pShLstVal->Edit();
					m_Util.char_to_cstring(&pShLstVal->ksname, (unsigned char*)NRec.KFval[Rct].KKnam, 40);
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ａ（決算額）
				else if(pShLstVal->itmId == _T("AOC00350")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AOC00360")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AOC00370")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("AOC00393")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval5, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｅ　（経過措置適用　6.24％）
				else if(pShLstVal->itmId == _T("AOC00394")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval7, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AOC00397")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ　（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AOC00398")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// その他の経費 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AOC00410")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AOC00420")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AOC00430")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("AOC00453")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval5, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｅ　（経過措置適用　6.24％）
				else if(pShLstVal->itmId == _T("AOC00454")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval7, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AOC00457")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ　（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AOC00458")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 計 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AOC00470")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AOC00480")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AOC00490")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("AOC00520")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval5, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｅ　（経過措置適用　6.24％）
				else if(pShLstVal->itmId == _T("AOC00521")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval7, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AOC00530")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｇ　（経過措置適用　7.8％）
				else if(pShLstVal->itmId == _T("AOC00531")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval8, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 差引金額 --------------------------------------------
				Rct++;
				// Ａ（決算額）
				//pShLstVal->itmId = _T("");		// 230727Aw del
				if(pShLstVal->itmId == _T("AOD00010")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 計（返還等対価）(※itmIdは存在しない) -----------------
				Rct++;
				// Ａ（決算額）
				if(pShLstVal->itmId == _T("AOB01210")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｂ（Ａのうち課税取引にならないもの）
				else if(pShLstVal->itmId == _T("AOB01220")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｃ（課税取引金額(A-B)）
				else if(pShLstVal->itmId == _T("AOB01230")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｄ（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("AOB01263")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval5, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// Ｆ（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("AOB01270")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KFval[Rct].KKval6, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KFval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
			}
			// 課税売上高計算表
			else if(pShLstVal->hyoId == ICS_TAXLIST_04_HYOID) {
				Rct = 0;

				// （3）任意科目　名称 --------------------------------------------
				if(pShLstVal->itmId == _T("AQD00010")) {
					pShLstVal->Edit();
					m_Util.char_to_cstring(&pShLstVal->ksname, (unsigned char*)Head.KHkunm, 40);
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// (1)事業所得に係る課税売上高 --------------------------------------------
				// 営業等課税売上高 --------------------------------------------
				//Rct++;
				// 金額
				if(pShLstVal->itmId == _T("AQB00040")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;	// 230802Aw_46 add
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//うち軽減税率6.24％適用分
				else if(pShLstVal->itmId == _T("AQB00070")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;	// 230802Aw_46 add
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//うち標準税率7.8％適用分
				else if(pShLstVal->itmId == _T("AQB00080")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;	// 230802Aw_46 add
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 農業課税売上高 --------------------------------------------
				Rct++;
				//金額
				if(pShLstVal->itmId == _T("AQB00100")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//うち軽減税率6.24％適用分
				else if(pShLstVal->itmId == _T("AQB00120")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//うち標準税率7.8％適用分
				else if(pShLstVal->itmId == _T("AQB00130")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 不動産所得に係る課税売上高 --------------------------------------------
				// 課税売上高 --------------------------------------------
				Rct++;
				//金額
				if(pShLstVal->itmId == _T("AQC00040")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//うち軽減税率6.24％適用分
				else if(pShLstVal->itmId == _T("AQC00070")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//うち標準税率7.8％適用分
				else if(pShLstVal->itmId == _T("AQC00080")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// （3）所得に係る課税売上高 --------------------------------------------
				// 損益計算書の収入金額 --------------------------------------------
				Rct++;
				//金額
				if(pShLstVal->itmId == _T("AQD00020")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//うち軽減税率6.24％適用分
				else if(pShLstVal->itmId == _T("AQD00025")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//うち標準税率7.8％適用分 
				else if(pShLstVal->itmId == _T("AQD00027")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// ④のうち、課税売上にならないもの --------------------------------------------
				Rct++;
				//金額
				if(pShLstVal->itmId == _T("AQD00030")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//うち軽減税率6.24％適用分
				else if(pShLstVal->itmId == _T("AQD00035")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//うち標準税率7.8％適用分
				else if(pShLstVal->itmId == _T("AQD00037")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 差引課税売上高 --------------------------------------------
				Rct++;
				//金額
				if(pShLstVal->itmId == _T("AQD00050")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//うち軽減税率6.24％適用分
				else if(pShLstVal->itmId == _T("AQD00080")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//うち標準税率7.8％適用分
				else if(pShLstVal->itmId == _T("AQD00090")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// (4)業務用資産の譲渡所得に係る課税売上高 --------------------------------------------
				// 業務用固定資産等の譲渡収入金額 --------------------------------------------
				Rct++;
				//金額
				if(pShLstVal->itmId == _T("AQE00010")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//うち軽減税率6.24％適用分
				else if(pShLstVal->itmId == _T("AQE00015")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//うち標準税率7.8％適用分
				else if(pShLstVal->itmId == _T("AQE00017")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// ⑦のうち、課税売上にならないもの --------------------------------------------
				Rct++;
				//金額
				if(pShLstVal->itmId == _T("AQE00020")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//うち軽減税率6.24％適用分
				else if(pShLstVal->itmId == _T("AQE00025")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//うち標準税率7.8％適用分
				else if(pShLstVal->itmId == _T("AQE00027")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 差引課税売上高 --------------------------------------------
				Rct++;
				//金額
				if(pShLstVal->itmId == _T("AQE00040")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//うち軽減税率6.24％適用分
				else if(pShLstVal->itmId == _T("AQE00070")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//うち標準税率7.8％適用分
				//else if(pShLstVal->itmId == _T("AQD00080")) {	// 230727Aw del
				else if(pShLstVal->itmId == _T("AQE00080")) {	// 230727Aw add
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 課税売上高の合計額 --------------------------------------------
				Rct++;
				//金額
				if(pShLstVal->itmId == _T("AQF00010")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//うち軽減税率6.24％適用分
				else if(pShLstVal->itmId == _T("AQF00040")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//うち標準税率7.8％適用分
				else if(pShLstVal->itmId == _T("AQF00050")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				Rct++;
				// (6)課税資産の譲渡等の対価の額の計算 --------------------------------------------
				// （軽減税率6.24％適用分） --------------------------------------------
				Rct++;
				// 1円未満の端数切捨て
				if(pShLstVal->itmId == _T("AQG00044")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// ○○円×100/108
				else if(pShLstVal->itmId == _T("AQG00042")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// 仮受消費税等(上段)
				else if(pShLstVal->itmId == _T("AQG00041")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// （標準税率7.8％適用分） --------------------------------------------
				Rct++;
				// 1円未満の端数切捨て
				if(pShLstVal->itmId == _T("AQG00048")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// ○○円×100/110
				else if(pShLstVal->itmId == _T("AQG00046")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// 仮受消費税等(上段)
				else if(pShLstVal->itmId == _T("AQG00045")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
			}
			// 課税仕入高計算表
			else if(pShLstVal->hyoId == ICS_TAXLIST_05_HYOID) {	// 230728Aw add
				Rct = 0;

				// （3）任意科目　名称 --------------------------------------------
				if(pShLstVal->itmId == _T("ASD00010")) {
					pShLstVal->Edit();
					m_Util.char_to_cstring(&pShLstVal->ksname, (unsigned char*)Head.KHksnm, 40);
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// (1)事業所得に係る課税仕入高 --------------------------------------------
				// 営業等課税仕入高 --------------------------------------------
				//Rct++;
				//金額
				if(pShLstVal->itmId == _T("ASB00040")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//課税仕入高（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("ASB00070")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//経過措置適用（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("ASB00071")) {
					pShLstVal->Edit();
					//m_Arith.l_print(buf, NRec.KSval[Rct].KKval6, "SSSSSSSSSSSSSSS");	// 230728Aw del
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw add
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//課税仕入高（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("ASB00080")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval4, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//経過措置適用（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("ASB00081")) {
					pShLstVal->Edit();
					//m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw del
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval8, "SSSSSSSSSSSSSSS");	// 230728Aw del
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 農業課税仕入高 --------------------------------------------
				Rct++;
				//金額
				if(pShLstVal->itmId == _T("ASB00100")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//課税仕入高（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("ASB00120")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//経過措置適用（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("ASB00121")) {
					pShLstVal->Edit();
					//m_Arith.l_print(buf, NRec.KSval[Rct].KKval6, "SSSSSSSSSSSSSSS");	// 230728Aw del
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw add
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//課税仕入高（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("ASB00130")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval4, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//経過措置適用（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("ASB00131")) {
					pShLstVal->Edit();
					//m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw del
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval8, "SSSSSSSSSSSSSSS");	// 230728Aw add
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

			// 不動産所得に係る課税仕入高 --------------------------------------------
				// 課税仕入高 --------------------------------------------
				Rct++;
				//金額
				if(pShLstVal->itmId == _T("ASC00040")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//課税仕入高（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("ASC00070")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//経過措置適用（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("ASC00071")) {
					pShLstVal->Edit();
					//m_Arith.l_print(buf, NRec.KSval[Rct].KKval6, "SSSSSSSSSSSSSSS");	// 230728Aw del
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw add
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//課税仕入高（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("ASC00080")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval4, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//経過措置適用（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("ASC00081")) {
					pShLstVal->Edit();
					//m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw del
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval8, "SSSSSSSSSSSSSSS");	// 230728Aw add
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

			// （　）所得に係る課税仕入高 --------------------------------------------
				// 損益計算書の仕入金額と経費の金額の合計額 --------------------------------------------
				Rct++;
				//金額
				if(pShLstVal->itmId == _T("ASD00020")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//課税仕入高（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("ASD00025")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//経過措置適用（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("ASD00026")) {
					pShLstVal->Edit();
					//m_Arith.l_print(buf, NRec.KSval[Rct].KKval6, "SSSSSSSSSSSSSSS");	// 230728Aw del
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw add
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//課税仕入高（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("ASD00027")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval4, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//経過措置適用（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("ASD00028")) {
					pShLstVal->Edit();
					//m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw del
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval8, "SSSSSSSSSSSSSSS");	// 230728Aw add
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// ④のうち、課税仕入にならないもの --------------------------------------------
				Rct++;
				//金額
				if(pShLstVal->itmId == _T("ASD00030")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//課税仕入高（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("ASD00035")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//経過措置適用（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("ASD00036")) {
					pShLstVal->Edit();
					//m_Arith.l_print(buf, NRec.KSval[Rct].KKval6, "SSSSSSSSSSSSSSS");	// 230728Aw del
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw add
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//課税仕入高（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("ASD00037")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval4, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//経過措置適用（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("ASD00038")) {
					pShLstVal->Edit();
					//m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw del
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval8, "SSSSSSSSSSSSSSS");	// 230728Aw add
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 差引課税仕入高 --------------------------------------------
				Rct++;
				//金額
				if(pShLstVal->itmId == _T("ASD00050")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//課税仕入高（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("ASD00080")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//経過措置適用（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("ASD00081")) {
					pShLstVal->Edit();
					//m_Arith.l_print(buf, NRec.KSval[Rct].KKval6, "SSSSSSSSSSSSSSS");	// 230728Aw del
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw add
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//課税仕入高（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("ASD00090")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval4, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//経過措置適用（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("ASD00091")) {
					pShLstVal->Edit();
					//m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw del
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval8, "SSSSSSSSSSSSSSS");	// 230728Aw add
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

			// 業務用資産の取得に係る課税仕入高 --------------------------------------------
				// 業務用固定資産等の取得費 --------------------------------------------
				Rct++;
				//金額
				if(pShLstVal->itmId == _T("ASE00010")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//課税仕入高（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("ASE00015")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//経過措置適用（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("ASE00016")) {
					pShLstVal->Edit();
					//m_Arith.l_print(buf, NRec.KSval[Rct].KKval6, "SSSSSSSSSSSSSSS");	// 230728Aw del
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw add
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//課税仕入高（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("ASE00017")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval4, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//経過措置適用（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("ASE00018")) {
					pShLstVal->Edit();
					//m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw del
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval8, "SSSSSSSSSSSSSSS");	// 230728Aw add
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// ⑦のうち、課税仕入にならないもの --------------------------------------------
				Rct++;
				//金額
				if(pShLstVal->itmId == _T("ASE00020")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//課税仕入高（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("ASE00025")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//経過措置適用（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("ASE00026")) {
					pShLstVal->Edit();
					//m_Arith.l_print(buf, NRec.KSval[Rct].KKval6, "SSSSSSSSSSSSSSS");	// 230728Aw del
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw add
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//課税仕入高（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("ASE00027")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval4, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//経過措置適用（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("ASE00028")) {
					pShLstVal->Edit();
					//m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw del
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval8, "SSSSSSSSSSSSSSS");	// 230728Aw add
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// 差引課税仕入高 --------------------------------------------
				Rct++;
				//金額
				if(pShLstVal->itmId == _T("ASE00040")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//課税仕入高（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("ASE00070")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//経過措置適用（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("ASE00071")) {
					pShLstVal->Edit();
					//m_Arith.l_print(buf, NRec.KSval[Rct].KKval6, "SSSSSSSSSSSSSSS");	// 230728Aw del
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw add
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//課税仕入高（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("ASE00080")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval4, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//経過措置適用（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("ASE00081")) {
					pShLstVal->Edit();
					//m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw del
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval8, "SSSSSSSSSSSSSSS");	// 230728Aw add
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

			// 課税仕入高の合計額 --------------------------------------------
				Rct++;
				//金額
				if(pShLstVal->itmId == _T("ASF00010")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//課税仕入高（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("ASF00040")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//経過措置適用（うち軽減税率6.24％適用分）
				else if(pShLstVal->itmId == _T("ASF00041")) {
					pShLstVal->Edit();
					//m_Arith.l_print(buf, NRec.KSval[Rct].KKval6, "SSSSSSSSSSSSSSS");	// 230728Aw del
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw add
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//課税仕入高（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("ASF00050")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval4, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				//経過措置適用（うち標準税率7.8％適用分）
				else if(pShLstVal->itmId == _T("ASF00051")) {
					pShLstVal->Edit();
					//m_Arith.l_print(buf, NRec.KSval[Rct].KKval7, "SSSSSSSSSSSSSSS");	// 230728Aw del
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval8, "SSSSSSSSSSSSSSS");	// 230728Aw add
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

				// (6)課税仕入に係る消費税額の計算 --------------------------------------------
				Rct++;

				// （軽減税率6.24％適用分） --------------------------------------------
				Rct++;
				// 金額
				if(pShLstVal->itmId == _T("ASG00044")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// ○円×6.24/108
				else if(pShLstVal->itmId == _T("ASG00042")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// 仮払消費税等
				else if(pShLstVal->itmId == _T("ASG00041")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
// 230728Aw del -->
				//// 経過措置適用（軽減税率6.24％適用分） --------------------------------------------
				//// 金額
				//else if(pShLstVal->itmId == _T("ASG00144")) {
				//	pShLstVal->Edit();
				//	m_Arith.l_print(buf, NRec.KSval[Rct].KKval4, "SSSSSSSSSSSSSSS");
				//	pShLstVal->val = buf;
				//	pShLstVal->val.TrimLeft();		// スペースカット
				//	pShLstVal->Update();
				//	updTagAry.Add(pShLstVal->itmId);
				//}
				//// ○円×6.24/108×80%
				//else if(pShLstVal->itmId == _T("ASG00142")) {
				//	pShLstVal->Edit();
				//	m_Arith.l_print(buf, NRec.KSval[Rct].KKval5, "SSSSSSSSSSSSSSS");
				//	pShLstVal->val = buf;
				//	pShLstVal->val.TrimLeft();		// スペースカット
				//	pShLstVal->Update();
				//	updTagAry.Add(pShLstVal->itmId);
				//}
				//// 仮払消費税等
				//else if(pShLstVal->itmId == _T("ASG00141")) {
				//	pShLstVal->Edit();
				//	m_Arith.l_print(buf, NRec.KSval[Rct].KKval6, "SSSSSSSSSSSSSSS");
				//	pShLstVal->val = buf;
				//	pShLstVal->val.TrimLeft();		// スペースカット
				//	pShLstVal->Update();
				//	updTagAry.Add(pShLstVal->itmId);
				//}
// 230728Aw del <--

				// （標準税率7.8％適用分） --------------------------------------------
				Rct++;

				// 金額
				if(pShLstVal->itmId == _T("ASG00048")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// ○円×7.8/108
				else if(pShLstVal->itmId == _T("ASG00046")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// 仮払消費税等
				else if(pShLstVal->itmId == _T("ASG00045")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}

// 230728Aw add -->
				// 経過措置適用（軽減税率6.24％適用分） --------------------------------------------
				Rct++;
				// 金額
				if(pShLstVal->itmId == _T("ASG00144")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// ○円×6.24/108×80%
				else if(pShLstVal->itmId == _T("ASG00142")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// 仮払消費税等
				else if(pShLstVal->itmId == _T("ASG00141")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
// 230728Aw add <--

				// 経過措置適用（標準税率7.8％適用分） --------------------------------------------
// 230728Aw del -->
				//// 金額
				//else if(pShLstVal->itmId == _T("ASG00148")) {
				//	pShLstVal->Edit();
				//	m_Arith.l_print(buf, NRec.KSval[Rct].KKval4, "SSSSSSSSSSSSSSS");
				//	pShLstVal->val = buf;
				//	pShLstVal->val.TrimLeft();		// スペースカット
				//	pShLstVal->Update();
				//	updTagAry.Add(pShLstVal->itmId);
				//}
				//// ○円×7.8/108×80%
				//else if(pShLstVal->itmId == _T("ASG00146")) {
				//	pShLstVal->Edit();
				//	m_Arith.l_print(buf, NRec.KSval[Rct].KKval5, "SSSSSSSSSSSSSSS");
				//	pShLstVal->val = buf;
				//	pShLstVal->val.TrimLeft();		// スペースカット
				//	pShLstVal->Update();
				//	updTagAry.Add(pShLstVal->itmId);
				//}
				//// 仮払消費税等
				//else if(pShLstVal->itmId == _T("ASG00145")) {
				//	pShLstVal->Edit();
				//	m_Arith.l_print(buf, NRec.KSval[Rct].KKval6, "SSSSSSSSSSSSSSS");
				//	pShLstVal->val = buf;
				//	pShLstVal->val.TrimLeft();		// スペースカット
				//	pShLstVal->Update();
				//	updTagAry.Add(pShLstVal->itmId);
				//}
// 230728Aw del <--
// 230728Aw add -->
				Rct++;
				// 金額
				if(pShLstVal->itmId == _T("ASG00148")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval1, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// ○円×7.8/108×80%
				else if(pShLstVal->itmId == _T("ASG00146")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval2, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
				// 仮払消費税等
				else if(pShLstVal->itmId == _T("ASG00145")) {
					pShLstVal->Edit();
					m_Arith.l_print(buf, NRec.KSval[Rct].KKval3, "SSSSSSSSSSSSSSS");
					pShLstVal->val = buf;
					pShLstVal->val.TrimLeft();		// スペースカット
					pShLstVal->sign1 = NRec.KSval[Rct].KKauto;
					pShLstVal->Update();
					updTagAry.Add(pShLstVal->itmId);
				}
// 230728Aw add <--
			}

// 240220 戎居 del -->
//// 230728Aw add -->
//			// 事業別売上計算表
//			else {
//				if(m_Util.IsUnConnectMst(pDBNpSub->zvol) && (m_Util.GetKazeihoushiki(pDBNpSub->zvol) == ID_ICSSH_KANNI)) {
//					Rct = 40;
//					// 第一種事業(卸売業)
//					// 決定額
//					if(pShLstVal->itmId == _T("ZAA00010")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 課税取引外
//					else if(pShLstVal->itmId == _T("ZAA00011")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 返還等課税取引
//					else if(pShLstVal->itmId == _T("ZAA00013")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// うち税率7.8％適用分
//					// 課税取引金額
//					else if(pShLstVal->itmId == _T("ZAA00022")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 返還等課税取引
//					else if(pShLstVal->itmId == _T("ZAA00023")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// うち税率6.24％適用分
//					// 課税取引金額
//					else if(pShLstVal->itmId == _T("ZAA00032")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 返還等課税取引
//					else if(pShLstVal->itmId == _T("ZAA00033")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//
//					// 第二種事業(小売業等)
//					Rct++;
//					// 決定額
//					if(pShLstVal->itmId == _T("ZAB00010")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 課税取引外
//					else if(pShLstVal->itmId == _T("ZAB00011")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}				
//					// 返還等課税取引
//					else if(pShLstVal->itmId == _T("ZAB00013")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// うち税率7.8％適用分
//					// 課税取引金額
//					else if(pShLstVal->itmId == _T("ZAB00022")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 返還等課税取引
//					else if(pShLstVal->itmId == _T("ZAB00023")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// うち税率6.24％適用分
//					// 課税取引金額
//					else if(pShLstVal->itmId == _T("ZAB00032")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 返還等課税取引
//					else if(pShLstVal->itmId == _T("ZAB00033")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//				
//					// 第三種事業(製造業等)
//					Rct++;
//					// 決定額
//					if(pShLstVal->itmId == _T("ZAC00010")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 課税取引外
//					else if(pShLstVal->itmId == _T("ZAC00011")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 返還等課税取引
//					else if(pShLstVal->itmId == _T("ZAC00013")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// うち税率7.8％適用分
//					// 課税取引金額
//					else if(pShLstVal->itmId == _T("ZAC00022")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 返還等課税取引
//					else if(pShLstVal->itmId == _T("ZAC00023")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// うち税率6.24％適用分
//					// 課税取引金額
//					else if(pShLstVal->itmId == _T("ZAC00032")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 返還等課税取引
//					else if(pShLstVal->itmId == _T("ZAC00033")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//
//					// 第三種事業(製造業等)／農業事業者
//					Rct++;
//					// 決定額
//					if(pShLstVal->itmId == _T("ZAC00040")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 課税取引外
//					else if(pShLstVal->itmId == _T("ZAC00041")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 返還等課税取引
//					else if(pShLstVal->itmId == _T("ZAC00043")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// うち税率7.8％適用分
//					// 課税取引金額
//					else if(pShLstVal->itmId == _T("ZAC00052")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 返還等課税取引
//					else if(pShLstVal->itmId == _T("ZAC00053")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//				
//					// 第四種事業(その他)
//					Rct++;
//					// 決定額
//					if(pShLstVal->itmId == _T("ZAD00010")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 課税取引外
//					else if(pShLstVal->itmId == _T("ZAD00011")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 返還等課税取引
//					else if(pShLstVal->itmId == _T("ZAD00013")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// うち税率7.8％適用分
//					// 課税取引金額
//					else if(pShLstVal->itmId == _T("ZAD00022")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 返還等課税取引
//					else if(pShLstVal->itmId == _T("ZAD00023")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// うち税率6.24％適用分
//					// 課税取引金額
//					else if(pShLstVal->itmId == _T("ZAD00032")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 返還等課税取引
//					else if(pShLstVal->itmId == _T("ZAD00033")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//
//					// 第五種事業(サービス業等)
//					Rct++;
//					// 決定額
//					if(pShLstVal->itmId == _T("ZAE00010")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 課税取引外
//					else if(pShLstVal->itmId == _T("ZAE00011")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 返還等課税取引
//					else if(pShLstVal->itmId == _T("ZAE00013")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// うち税率7.8％適用分
//					// 課税取引金額
//					else if(pShLstVal->itmId == _T("ZAE00022")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 返還等課税取引
//					else if(pShLstVal->itmId == _T("ZAE00023")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// うち税率6.24％適用分
//					// 課税取引金額
//					else if(pShLstVal->itmId == _T("ZAE00032")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 返還等課税取引
//					else if(pShLstVal->itmId == _T("ZAE00033")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//
//					// 第六種事業(不動産業)
//					Rct++;
//					// 決定額
//					if(pShLstVal->itmId == _T("ZAF00010")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 課税取引外
//					else if(pShLstVal->itmId == _T("ZAF00011")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 返還等課税取引
//					else if(pShLstVal->itmId == _T("ZAF00013")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// うち税率7.8％適用分
//					// 課税取引金額
//					else if(pShLstVal->itmId == _T("ZAF00022")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 返還等課税取引
//					else if(pShLstVal->itmId == _T("ZAF00023")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// うち税率6.24％適用分
//					// 課税取引金額
//					else if(pShLstVal->itmId == _T("ZAF00032")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 返還等課税取引
//					else if(pShLstVal->itmId == _T("ZAF00033")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//				
//					// 合計
//					Rct++;
//					// 決定額
//					if(pShLstVal->itmId == _T("ZAG00010")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 課税取引外
//					else if(pShLstVal->itmId == _T("ZAG00011")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 課税取引金額
//					else if(pShLstVal->itmId == _T("ZAG00012")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 返還等課税取引
//					else if(pShLstVal->itmId == _T("ZAG00013")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// うち税率7.8％適用分
//					// 課税取引金額
//					else if(pShLstVal->itmId == _T("ZAG00022")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 返還等課税取引
//					else if(pShLstVal->itmId == _T("ZAG00023")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// うち税率6.24％適用分
//					// 課税取引金額
//					else if(pShLstVal->itmId == _T("ZAG00032")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 返還等課税取引
//					else if(pShLstVal->itmId == _T("ZAG00033")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//
//					// 第四種事業(その他)／固定資産等の譲渡
//					Rct++;
//					// 決定額
//					if(pShLstVal->itmId == _T("ZAD00040")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 課税取引外
//					else if(pShLstVal->itmId == _T("ZAD00041")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 返還等課税取引
//					else if(pShLstVal->itmId == _T("ZAD00043")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// うち税率7.8％適用分
//					// 課税取引金額
//					else if(pShLstVal->itmId == _T("ZAD00052")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// うち税率6.24％適用分
//					// 課税取引金額
//					else if(pShLstVal->itmId == _T("ZAD00062")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//
//					// 第二種事業(小売業等)／農業事業者
//					Rct++;
//					// 決定額
//					if(pShLstVal->itmId == _T("ZAB00040")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 課税取引外
//					else if(pShLstVal->itmId == _T("ZAB00041")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 返還等課税取引
//					else if(pShLstVal->itmId == _T("ZAB00043")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// うち税率6.24％適用分
//					// 課税取引金額
//					else if(pShLstVal->itmId == _T("ZAB00052")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//					// 返還等課税取引
//					else if(pShLstVal->itmId == _T("ZAB00053")) {
//						pShLstVal->Edit();
//						m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
//						pShLstVal->val = buf;
//						pShLstVal->val.TrimLeft();		// スペースカット
//						pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
//						pShLstVal->Update();
//						updTagAry.Add(pShLstVal->itmId);
//					}
//				}
//			}
//// 230728Aw add <--
// 240220 戎居 del <--
		}
	}

// 240220 戎居 add -->
	//　事業別売上高
	if (m_Util.IsUnConnectMst(pDBNpSub->zvol) && (m_Util.GetKazeihoushiki(pDBNpSub->zvol) == ID_ICSSH_KANNI)) {

// 240221 Aw cor -->
		//filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		//filter.Format(filter + _T("hyoId = \'%s\' ) "), ICS_TAXLIST_06_HYOID);	// 事業別売上高計算表

		//// オープン
		//if (pDBSyzAcc->m_pSH_LST_VAL) {
		//	pDBSyzAcc->m_pSH_LST_VAL->Requery(filter);
		//}
		//else {
		//	if (pDBSyzAcc->OpenShLstVal(pDBNpSub->m_database, filter) == ERR) {
		//		delete pDBSyzAcc;
		//		return ERR;
		//	}
		//}

		//CSH_LST_VAL		*pShLstVal = pDBSyzAcc->m_pSH_LST_VAL;

		//if ((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
		//	// 事業別売上計算表
		//	//if (m_Util.IsUnConnectMst(pDBNpSub->zvol) && (m_Util.GetKazeihoushiki(pDBNpSub->zvol) == ID_ICSSH_KANNI)) {
		//	Rct = 40;

		//	// 第一種事業(卸売業)
		//	// 決定額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAA00010");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 課税取引外
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAA00011");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAA00013");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// うち税率7.8％適用分
		//	// 課税取引金額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAA00022");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAA00023");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// うち税率6.24％適用分
		//	// 課税取引金額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAA00032");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAA00033");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	//6.3追加分
		//	// 課税取引金額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAA00042");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval9, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	//　返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAA00043");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval10, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 第二種事業(小売業等)
		//	Rct++;
		//	// 決定額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAB00010");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 課税取引外
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAB00011");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAB00013");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// うち税率7.8％適用分
		//	// 課税取引金額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAB00022");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAB00023");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// うち税率6.24％適用分
		//	// 課税取引金額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAB00032");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAB00033");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();
	
		//	//6.3追加分
		//	// 課税取引金額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAB00062");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval9, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	//　返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAB00063");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval10, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 第三種事業(製造業等)
		//	Rct++;
		//	// 決定額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAC00010");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 課税取引外
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAC00011");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAC00013");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// うち税率7.8％適用分
		//	// 課税取引金額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAC00022");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAC00023");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// うち税率6.24％適用分
		//	// 課税取引金額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAC00032");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAC00033");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	//6.3追加分
		//	// 課税取引金額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAC00062");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval9, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	//　返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAC00063");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval10, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 第三種事業(製造業等)／農業事業者
		//	Rct++;
		//	// 決定額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAC00040");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 課税取引外
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAC00041");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAC00043");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// うち税率7.8％適用分
		//	// 課税取引金額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAC00052");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAC00053");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	//6.3追加分
		//	// 課税取引金額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAC00072");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval9, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	//　返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAC00073");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval10, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 第四種事業(その他)
		//	Rct++;
		//	// 決定額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAD00010");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 課税取引外
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAD00011");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAD00013");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// うち税率7.8％適用分
		//	// 課税取引金額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAD00022");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAD00023");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// うち税率6.24％適用分
		//	// 課税取引金額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAD00032");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAD00033");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	//6.3追加分
		//	// 課税取引金額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAD00072");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval9, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	//　返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAD00073");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval10, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 第五種事業(サービス業等)
		//	Rct++;
		//	// 決定額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAE00010");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 課税取引外
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAE00011");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAE00013");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// うち税率7.8％適用分
		//	// 課税取引金額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAE00022");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAE00023");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// うち税率6.24％適用分
		//	// 課税取引金額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAE00032");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAE00033");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();
	
		//	//6.3追加分
		//	// 課税取引金額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAE00042");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval9, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	//　返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAE00043");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval10, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 第六種事業(不動産業)
		//	Rct++;
		//	// 決定額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAF00010");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 課税取引外
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAF00011");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAF00013");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// うち税率7.8％適用分
		//	// 課税取引金額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAF00022");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAF00023");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// うち税率6.24％適用分
		//	// 課税取引金額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAF00032");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAF00033");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	//6.3追加分
		//	// 課税取引金額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAF00042");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval9, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	//　返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAF00043");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval10, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 合計
		//	Rct++;
		//	// 決定額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAG00010");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 課税取引外
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAG00011");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 課税取引金額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAG00012");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAG00013");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// うち税率7.8％適用分
		//	// 課税取引金額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAG00022");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAG00023");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// うち税率6.24％適用分
		//	// 課税取引金額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAG00032");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAG00033");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();
	
		//	//6.3追加分
		//	// 課税取引金額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAG00042");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval9, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	//　返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAG00043");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval10, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 第四事業（その他）／固定資産等の譲渡
		//	Rct++;
		//	// 決定額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAD00040");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 課税取引外
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAD00041");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAD00043");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// うち税率7.8％適用分
		//	// 課税取引金額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAD00052");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	//// 返還等課税取引
		//	//pShLstVal->AddNew();
		//	//pShLstVal->sn_seq = sn_seq;
		//	//pShLstVal->version = nowVersion;
		//	//pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	//pShLstVal->itmId = _T("ZAD00053");
		//	//m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		//	//pShLstVal->val = buf;
		//	//pShLstVal->val.TrimLeft();		// スペースカット
		//	//pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	//pShLstVal->ksname.Empty();
		//	//pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	//pShLstVal->Update();

		//	// うち税率6.24％適用分
		//	// 課税取引金額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAD00062");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	//// 返還等課税取引
		//	//pShLstVal->AddNew();
		//	//pShLstVal->sn_seq = sn_seq;
		//	//pShLstVal->version = nowVersion;
		//	//pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	//pShLstVal->itmId = _T("ZAD00063");
		//	//m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		//	//pShLstVal->val = buf;
		//	//pShLstVal->val.TrimLeft();		// スペースカット
		//	//pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	//pShLstVal->ksname.Empty();
		//	//pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	//pShLstVal->Update();
	
		//	//6.3追加分
		//	// 課税取引金額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAD00082");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval9, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	////　返還等課税取引
		//	//pShLstVal->AddNew();
		//	//pShLstVal->sn_seq = sn_seq;
		//	//pShLstVal->version = nowVersion;
		//	//pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	//pShLstVal->itmId = _T("ZAD00083");
		//	//m_Arith.l_print(buf, NRec.KUval[Rct].KKval10, "SSSSSSSSSSSSSSS");
		//	//pShLstVal->val = buf;
		//	//pShLstVal->val.TrimLeft();		// スペースカット
		//	//pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	//pShLstVal->ksname.Empty();
		//	//pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	//pShLstVal->Update();

		//	// 第二種事業(小売業等)／農業事業者
		//	Rct++;
		//	// 決定額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAB00040");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 課税取引外
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAB00041");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAB00043");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// うち税率6.24％適用分
		//	// 課税取引金額
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAB00052");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();

		//	// 返還等課税取引
		//	pShLstVal->AddNew();
		//	pShLstVal->sn_seq = sn_seq;
		//	pShLstVal->version = nowVersion;
		//	pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//	pShLstVal->itmId = _T("ZAB00053");
		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		//	pShLstVal->val = buf;
		//	pShLstVal->val.TrimLeft();		// スペースカット
		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//	pShLstVal->ksname.Empty();
		//	pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//	pShLstVal->Update();
		//}
		//else {															// データあり
		//// 事業別売上計算表
		////if (m_Util.IsUnConnectMst(pDBNpSub->zvol) && (m_Util.GetKazeihoushiki(pDBNpSub->zvol) == ID_ICSSH_KANNI)) {

		//	CStringArray	updTagAry;
		//	for (ii = 0; ; ii++) {
		//		if (ii == 0) {
		//			if ((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {
		//				break;
		//			}
		//		}
		//		else {
		//			if ((pShLstVal->MoveNext() == -1) || (pShLstVal->st == -1)) {
		//				break;
		//			}
		//		}

		//		Rct = 40;
		//		// 第一種事業(卸売業)
		//		// 決定額
		//		if (pShLstVal->itmId == _T("ZAA00010")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 課税取引外
		//		else if (pShLstVal->itmId == _T("ZAA00011")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAA00013")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// うち税率7.8％適用分
		//		// 課税取引金額
		//		else if (pShLstVal->itmId == _T("ZAA00022")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAA00023")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// うち税率6.24％適用分
		//		// 課税取引金額
		//		else if (pShLstVal->itmId == _T("ZAA00032")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAA00033")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}

		//		//6.3追加分
		//		// 課税取引金額
		//		else if (pShLstVal->itmId == _T("ZAA00042")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval9, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		//　返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAA00043")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval10, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}

		//		// 第二種事業(小売業等)
		//		Rct++;
		//		// 決定額
		//		if (pShLstVal->itmId == _T("ZAB00010")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 課税取引外
		//		else if (pShLstVal->itmId == _T("ZAB00011")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAB00013")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// うち税率7.8％適用分
		//		// 課税取引金額
		//		else if (pShLstVal->itmId == _T("ZAB00022")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAB00023")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// うち税率6.24％適用分
		//		// 課税取引金額
		//		else if (pShLstVal->itmId == _T("ZAB00032")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAB00033")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		//6.3追加分
		//		// 課税取引金額
		//		else if (pShLstVal->itmId == _T("ZAB00062")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval9, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		//　返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAB00063")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval10, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}

		//		// 第三種事業(製造業等)
		//		Rct++;
		//		// 決定額
		//		if (pShLstVal->itmId == _T("ZAC00010")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 課税取引外
		//		else if (pShLstVal->itmId == _T("ZAC00011")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAC00013")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// うち税率7.8％適用分
		//		// 課税取引金額
		//		else if (pShLstVal->itmId == _T("ZAC00022")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAC00023")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// うち税率6.24％適用分
		//		// 課税取引金額
		//		else if (pShLstVal->itmId == _T("ZAC00032")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAC00033")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		//6.3追加分
		//		// 課税取引金額
		//		else if (pShLstVal->itmId == _T("ZAC00062")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval9, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		//　返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAC00063")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval10, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}

		//		// 第三種事業(製造業等)／農業事業者
		//		Rct++;
		//		// 決定額
		//		if (pShLstVal->itmId == _T("ZAC00040")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 課税取引外
		//		else if (pShLstVal->itmId == _T("ZAC00041")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAC00043")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// うち税率7.8％適用分
		//		// 課税取引金額
		//		else if (pShLstVal->itmId == _T("ZAC00052")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAC00053")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		//6.3追加分
		//		// 課税取引金額
		//		else if (pShLstVal->itmId == _T("ZAC00072")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval9, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		//　返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAC00073")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval10, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}

		//		// 第四種事業(その他)
		//		Rct++;
		//		// 決定額
		//		if (pShLstVal->itmId == _T("ZAD00010")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 課税取引外
		//		else if (pShLstVal->itmId == _T("ZAD00011")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAD00013")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// うち税率7.8％適用分
		//		// 課税取引金額
		//		else if (pShLstVal->itmId == _T("ZAD00022")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAD00023")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// うち税率6.24％適用分
		//		// 課税取引金額
		//		else if (pShLstVal->itmId == _T("ZAD00032")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAD00033")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		//6.3追加分
		//		// 課税取引金額
		//		else if (pShLstVal->itmId == _T("ZAD00072")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval9, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		//　返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAD00073")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval10, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}

		//		// 第五種事業(サービス業等)
		//		Rct++;
		//		// 決定額
		//		if (pShLstVal->itmId == _T("ZAE00010")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 課税取引外
		//		else if (pShLstVal->itmId == _T("ZAE00011")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAE00013")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// うち税率7.8％適用分
		//		// 課税取引金額
		//		else if (pShLstVal->itmId == _T("ZAE00022")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAE00023")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// うち税率6.24％適用分
		//		// 課税取引金額
		//		else if (pShLstVal->itmId == _T("ZAE00032")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAE00033")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		//6.3追加分
		//		// 課税取引金額
		//		else if (pShLstVal->itmId == _T("ZAE00042")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval9, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		//　返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAE00043")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval10, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}

		//		// 第六種事業(不動産業)
		//		Rct++;
		//		// 決定額
		//		if (pShLstVal->itmId == _T("ZAF00010")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 課税取引外
		//		else if (pShLstVal->itmId == _T("ZAF00011")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAF00013")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// うち税率7.8％適用分
		//		// 課税取引金額
		//		else if (pShLstVal->itmId == _T("ZAF00022")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAF00023")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// うち税率6.24％適用分
		//		// 課税取引金額
		//		else if (pShLstVal->itmId == _T("ZAF00032")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAF00033")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		//6.3追加分
		//		// 課税取引金額
		//		else if (pShLstVal->itmId == _T("ZAF00042")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval9, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		//　返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAF00043")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval10, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}

		//		// 合計
		//		Rct++;
		//		// 決定額
		//		if (pShLstVal->itmId == _T("ZAG00010")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 課税取引外
		//		else if (pShLstVal->itmId == _T("ZAG00011")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 課税取引金額
		//		else if (pShLstVal->itmId == _T("ZAG00012")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAG00013")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// うち税率7.8％適用分
		//		// 課税取引金額
		//		else if (pShLstVal->itmId == _T("ZAG00022")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAG00023")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// うち税率6.24％適用分
		//		// 課税取引金額
		//		else if (pShLstVal->itmId == _T("ZAG00032")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAG00033")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		//6.3追加分
		//		// 課税取引金額
		//		else if (pShLstVal->itmId == _T("ZAG00042")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval9, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		//　返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAG00043")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval10, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}

		//		// 第四種事業(その他)／固定資産等の譲渡
		//		Rct++;
		//		// 決定額
		//		if (pShLstVal->itmId == _T("ZAD00040")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 課税取引外
		//		else if (pShLstVal->itmId == _T("ZAD00041")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAD00043")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// うち税率7.8％適用分
		//		// 課税取引金額
		//		else if (pShLstVal->itmId == _T("ZAD00052")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// うち税率6.24％適用分
		//		// 課税取引金額
		//		else if (pShLstVal->itmId == _T("ZAD00062")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		//6.3追加分
		//		// 課税取引金額
		//		else if (pShLstVal->itmId == _T("ZAD00082")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval9, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		////　返還等課税取引
		//		//else if (pShLstVal->itmId == _T("ZAD00083")) {
		//		//	pShLstVal->Edit();
		//		//	m_Arith.l_print(buf, NRec.KUval[Rct].KKval10, "SSSSSSSSSSSSSSS");
		//		//	pShLstVal->val = buf;
		//		//	pShLstVal->val.TrimLeft();		// スペースカット
		//		//	pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//		//	pShLstVal->Update();
		//		//	updTagAry.Add(pShLstVal->itmId);
		//		//}

		//		// 第二種事業(小売業等)／農業事業者
		//		Rct++;
		//		// 決定額
		//		if (pShLstVal->itmId == _T("ZAB00040")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 課税取引外
		//		else if (pShLstVal->itmId == _T("ZAB00041")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAB00043")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// うち税率6.24％適用分
		//		// 課税取引金額
		//		else if (pShLstVal->itmId == _T("ZAB00052")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//		// 返還等課税取引
		//		else if (pShLstVal->itmId == _T("ZAB00053")) {
		//			pShLstVal->Edit();
		//			m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		//			pShLstVal->val = buf;
		//			pShLstVal->val.TrimLeft();		// スペースカット
		//			pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//			pShLstVal->Update();
		//			updTagAry.Add(pShLstVal->itmId);
		//		}
		//	}
		//}


//------------------
		CStringArray	updTagAry;

		// 事業別売上計算表
		Rct = 40;

		// 第一種事業(卸売業)
		// 決定額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAA00010"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAA00010");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 課税取引外
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAA00011"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAA00011");

			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

// 修正No.168646 add -->
		// 課税取引金額（自動計算）
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAA00012"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAA00012");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();
// 修正No.168646 add <--

		// 返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAA00013"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAA00013");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// うち税率7.8％適用分
		// 課税取引金額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAA00022"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAA00022");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAA00023"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAA00023");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// うち税率6.24％適用分
		// 課税取引金額
		// 返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAA00032"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAA00032");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAA00033"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAA00033");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		//6.3追加分
		// 課税取引金額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAA00042"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAA00042");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval9, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		//　返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAA00043"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAA00043");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval10, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 第二種事業(小売業等)
		Rct++;

		// 決定額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAB00010"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAB00010");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 課税取引外
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAB00011"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAB00011");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

// 修正No.168646 add -->
		// 課税取引金額（自動計算）
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAB00012"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAB00012");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();
// 修正No.168646 add <--

		// 返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAB00013"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAB00013");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// うち税率7.8％適用分
		// 課税取引金額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAB00022"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAB00022");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAB00023"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAB00023");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// うち税率6.24％適用分
		// 課税取引金額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAB00032"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAB00032");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAB00033"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAB00033");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();
	
		//6.3追加分
		// 課税取引金額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAB00062"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAB00062");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval9, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		//　返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAB00063"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAB00063");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval10, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 第三種事業(製造業等)
		Rct++;

		// 決定額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAC00010"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAC00010");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 課税取引外
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAC00011"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAC00011");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

// 修正No.168646 add -->
		// 課税取引金額（自動計算）
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAC00012"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAC00012");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();
// 修正No.168646 add <--

		// 返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAC00013"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAC00013");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// うち税率7.8％適用分
		// 課税取引金額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAC00022"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAC00022");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAC00023"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAC00023");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// うち税率6.24％適用分
		// 課税取引金額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAC00032"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAC00032");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAC00033"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAC00033");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		//6.3追加分
		// 課税取引金額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAC00062"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAC00062");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval9, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		//　返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAC00063"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAC00063");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval10, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 第三種事業(製造業等)／農業事業者
		Rct++;

		// 決定額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAC00040"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAC00040");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 課税取引外
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAC00041"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAC00041");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

// 修正No.168646 add -->
		// 課税取引金額（自動計算）
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAC00042"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAC00042");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();
// 修正No.168646 add <--

		// 返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAC00043"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAC00043");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// うち税率7.8％適用分
		// 課税取引金額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAC00052"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAC00052");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAC00053"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAC00053");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		//6.3追加分
		// 課税取引金額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAC00072"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAC00072");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval9, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		//　返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAC00073"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAC00073");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval10, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 第四種事業(その他)
		Rct++;

		// 決定額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAD00010"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAD00010");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 課税取引外
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAD00011"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAD00011");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

// 修正No.168646 add -->
		// 課税取引金額（自動計算）
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAD00012"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAD00012");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();
// 修正No.168646 add <--

		// 返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAD00013"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAD00013");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// うち税率7.8％適用分
		// 課税取引金額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAD00022"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAD00022");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAD00023"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAD00023");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// うち税率6.24％適用分
		// 課税取引金額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAD00032"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAD00032");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAD00033"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAD00033");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		//6.3追加分
		// 課税取引金額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAD00072"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAD00072");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval9, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		//　返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAD00073"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAD00073");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval10, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 第五種事業(サービス業等)
		Rct++;

		// 決定額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAE00010"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAE00010");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 課税取引外
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAE00011"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAE00011");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

// 修正No.168646 add -->
		// 課税取引金額（自動計算）
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAE00012"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAE00012");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();
// 修正No.168646 add <--

		// 返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAE00013"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAE00013");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// うち税率7.8％適用分
		// 課税取引金額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAE00022"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAE00022");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAE00023"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAE00023");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// うち税率6.24％適用分
		// 課税取引金額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAE00032"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAE00032");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAE00033"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAE00033");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();
	
		//6.3追加分
		// 課税取引金額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAE00042"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAE00042");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval9, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		//　返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAE00043"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAE00043");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval10, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 第六種事業(不動産業)
		Rct++;

		// 決定額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAF00010"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAF00010");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 課税取引外
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAF00011"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAF00011");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

// 修正No.168646 add -->
		// 課税取引金額（自動計算）
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAF00012"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAF00012");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();
// 修正No.168646 add <--

		// 返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAF00013"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAF00013");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// うち税率7.8％適用分
		// 課税取引金額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAF00022"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAF00022");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAF00023"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAF00023");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// うち税率6.24％適用分
		// 課税取引金額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAF00032"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAF00032");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAF00033"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAF00033");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		//6.3追加分
		// 課税取引金額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAF00042"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAF00042");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval9, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		//　返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAF00043"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAF00043");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval10, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 合計
		Rct++;

		// 決定額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAG00010"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAG00010");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 課税取引外
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAG00011"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAG00011");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 課税取引金額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAG00012"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAG00012");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAG00013"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAG00013");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// うち税率7.8％適用分
		// 課税取引金額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAG00022"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAG00022");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAG00023"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAG00023");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// うち税率6.24％適用分
		// 課税取引金額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAG00032"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAG00032");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAG00033"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAG00033");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();
	
		//6.3追加分
		// 課税取引金額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAG00042"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAG00042");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval9, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		//　返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAG00043"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAG00043");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval10, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 第四事業（その他）／固定資産等の譲渡
		Rct++;

		// 決定額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAD00040"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAD00040");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 課税取引外
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAD00041"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAD00041");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

// 修正No.168646 add -->
		// 課税取引金額（自動計算）
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAD00042"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAD00042");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();
// 修正No.168646 add <--

		// 返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAD00043"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAD00043");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// うち税率7.8％適用分
		// 課税取引金額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAD00052"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAD00052");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval5, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		//// 返還等課税取引
		//pShLstVal->AddNew();
		//pShLstVal->sn_seq = sn_seq;
		//pShLstVal->version = nowVersion;
		//pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//pShLstVal->itmId = _T("ZAD00053");
		//m_Arith.l_print(buf, NRec.KUval[Rct].KKval6, "SSSSSSSSSSSSSSS");
		//pShLstVal->val = buf;
		//pShLstVal->val.TrimLeft();		// スペースカット
		//pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//pShLstVal->ksname.Empty();
		//pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//pShLstVal->Update();

		// うち税率6.24％適用分
		// 課税取引金額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAD00062"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAD00062");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		//// 返還等課税取引
		//pShLstVal->AddNew();
		//pShLstVal->sn_seq = sn_seq;
		//pShLstVal->version = nowVersion;
		//pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//pShLstVal->itmId = _T("ZAD00063");
		//m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		//pShLstVal->val = buf;
		//pShLstVal->val.TrimLeft();		// スペースカット
		//pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//pShLstVal->ksname.Empty();
		//pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//pShLstVal->Update();
	
		//6.3追加分
		// 課税取引金額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAD00082"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAD00082");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval9, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		////　返還等課税取引
		//pShLstVal->AddNew();
		//pShLstVal->sn_seq = sn_seq;
		//pShLstVal->version = nowVersion;
		//pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
		//pShLstVal->itmId = _T("ZAD00083");
		//m_Arith.l_print(buf, NRec.KUval[Rct].KKval10, "SSSSSSSSSSSSSSS");
		//pShLstVal->val = buf;
		//pShLstVal->val.TrimLeft();		// スペースカット
		//pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		//pShLstVal->ksname.Empty();
		//pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		//pShLstVal->Update();

		// 第二種事業(小売業等)／農業事業者
		Rct++;

		// 決定額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAB00040"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAB00040");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval1, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 課税取引外
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAB00041"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAB00041");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval2, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

// 修正No.168646 add -->
		// 課税取引金額（自動計算）
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAB00042"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAB00042");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval3, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();
// 修正No.168646 add <--

		// 返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAB00043"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAB00043");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval4, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// うち税率6.24％適用分
		// 課税取引金額
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAB00052"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAB00052");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval7, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();

		// 返還等課税取引
		filter.Format("sn_seq = %d and version = %d and ( ", sn_seq, nowVersion);
		filter.Format(filter + _T("hyoId = \'%s\' and itmId = \'%s\' ) "), ICS_TAXLIST_06_HYOID, _T("ZAB00053"));
		pShLstVal->Requery(filter);
		if((pShLstVal->MoveFirst() == -1) || (pShLstVal->st == -1)) {// データなし
			pShLstVal->AddNew();
			pShLstVal->sn_seq = sn_seq;
			pShLstVal->version = nowVersion;
			pShLstVal->hyoId = ICS_TAXLIST_06_HYOID;
			pShLstVal->itmId = _T("ZAB00053");
			pShLstVal->ksname.Empty();
			pShLstVal->kssgn1 = NRec.KUval[Rct].KKtype;
		}
		else {
			pShLstVal->Edit();
			updTagAry.Add(pShLstVal->itmId);
		}
		m_Arith.l_print(buf, NRec.KUval[Rct].KKval8, "SSSSSSSSSSSSSSS");
		pShLstVal->val = buf;
		pShLstVal->val.TrimLeft();		// スペースカット
		pShLstVal->sign1 = NRec.KUval[Rct].KKauto;
		pShLstVal->Update();
// 240221 Aw cor <--
	}
// 240220 戎居 add <--

	// 税込き・税抜き
	pSnHeadData->SVsign &= 0xdf;
	if(!(Head.KHstax & 0x01)) {
		pSnHeadData->SVsign |= 0x20;	// 込み
	}

	if(Head.KHkoke & 0x01) {
		pSnHeadData->Sn_Sign4 |= 0x02;
	}
	else {
		pSnHeadData->Sn_Sign4 &= ~0x02;
	}

	// 後始末
	delete( pDBSyzAcc );

	return 0;
}
// 230718Aw <--

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
int CH31TaxListData::ConnectZmdata( CDBNpSub *pDBNpSub, CSyzSyukei *pSyzSyukei, int smon, int emon, BOOL IsTransitionalMeasure )
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
		memset( NRec.KJval[i].KKval6, '\0', sizeof(NRec.KJval[i].KKval6) );
		// 課税取引金額計算表（不動産取得用）
		NRec.KFval[i].KKauto = 0x00;
		memset( NRec.KFval[i].KKval1, '\0', sizeof(NRec.KFval[i].KKval1) );
		memset( NRec.KFval[i].KKval2, '\0', sizeof(NRec.KFval[i].KKval2) );
		memset( NRec.KFval[i].KKval3, '\0', sizeof(NRec.KFval[i].KKval3) );
		memset( NRec.KFval[i].KKval4, '\0', sizeof(NRec.KFval[i].KKval4) );
		memset( NRec.KFval[i].KKval5, '\0', sizeof(NRec.KFval[i].KKval5) );
		memset( NRec.KFval[i].KKval6, '\0', sizeof(NRec.KFval[i].KKval6) );
		// 課税取引金額計算表（農業得用）
		NRec.KNval[i].KKauto = 0x00;
		memset( NRec.KNval[i].KKval1, '\0', sizeof(NRec.KNval[i].KKval1) );
		memset( NRec.KNval[i].KKval2, '\0', sizeof(NRec.KNval[i].KKval2) );
		memset( NRec.KNval[i].KKval3, '\0', sizeof(NRec.KNval[i].KKval3) );
		memset( NRec.KNval[i].KKval4, '\0', sizeof(NRec.KNval[i].KKval4) );
		memset( NRec.KNval[i].KKval5, '\0', sizeof(NRec.KNval[i].KKval5) );
		memset( NRec.KNval[i].KKval6, '\0', sizeof(NRec.KNval[i].KKval6) );

		if( (i==KSELLINE4) || (i==KSELLINE5) || (i==KSELLINE6) ){
			continue;
		}
		
		// 課税売上計算表
		NRec.KUval[i].KKauto = 0x00;
		memset( NRec.KUval[i].KKval1, '\0', sizeof(NRec.KUval[i].KKval1) );
		memset( NRec.KUval[i].KKval2, '\0', sizeof(NRec.KUval[i].KKval2) );
		memset( NRec.KUval[i].KKval3, '\0', sizeof(NRec.KUval[i].KKval3) );
		memset( NRec.KUval[i].KKval4, '\0', sizeof(NRec.KUval[i].KKval4) );
		memset( NRec.KUval[i].KKval5, '\0', sizeof(NRec.KUval[i].KKval5) );
		memset( NRec.KUval[i].KKval6, '\0', sizeof(NRec.KUval[i].KKval6) );
		// 課税仕入計算表
		NRec.KSval[i].KKauto = 0x00;
		memset( NRec.KSval[i].KKval1, '\0', sizeof(NRec.KSval[i].KKval1) );
		memset( NRec.KSval[i].KKval2, '\0', sizeof(NRec.KSval[i].KKval2) );
		memset( NRec.KSval[i].KKval3, '\0', sizeof(NRec.KSval[i].KKval3) );
		memset( NRec.KSval[i].KKval4, '\0', sizeof(NRec.KSval[i].KKval4) );
		memset( NRec.KSval[i].KKval5, '\0', sizeof(NRec.KSval[i].KKval5) );
		memset( NRec.KSval[i].KKval6, '\0', sizeof(NRec.KSval[i].KKval6) );
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
					if( lstKsOfs == j ){
						k = 0;
					}
					else{
						k = 1;
					}
					break;
				case 2 :
					if( lstKsOfs == j ){
						k = 1;
					}
					else{
						k = 2;
					}
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
					if( lstKsOfs == j ){
						k = 3;
					}
					else{
						k = 4;
					}
					break;
				case 6 :
					if( lstKsOfs == j ){
						k = 4;
					}
					else{
						k = 5;
					}
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
					if( lstKsOfs == j ){
						k = 6;
					}
					else{
						k = 7;
					}
					break;
				case 10:
					if( lstKsOfs == j ){
						k = 7;
					}
					else{
						k = 8;
					}
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
					if( lstKsOfs == j ){
						k = 9;
					}
					else{
						k = 10;
					}
					break;
				case 14:
					if( lstKsOfs == j ){
						k = 10;
					}
					else{
						k = 11;
					}
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
			if( knrec.Kn_att&0x01 ){
				m_Arith.l_sub( Val, Ksi, Kri );
			}
			else{
				m_Arith.l_sub( Val, Kri, Ksi );
			}
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
				if( (!(knrec.Kn_att&0x01) && (NRec.KJval[Putno].KKtype&0x01)) ||
					((knrec.Kn_att&0x01) && !(NRec.KJval[Putno].KKtype&0x01)) ){
					m_Arith.l_neg( Kzn );
				}
				m_Arith.l_add( NRec.KJval[Putno].KKval1, NRec.KJval[Putno].KKval1, Kzn );
				m_Arith.l_add( NRec.KJval[Putno].KKval2, NRec.KJval[Putno].KKval2, Kzn );
				break;

			case 0x02 :	// 不動産									   
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

//			shisan_zino
//			0x00：5%
//			0x01：3%
//			0x03：8%
//			0x04：8%軽減
//			0x05：10%

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
					if( money[i][0].attr.shisan_beki ){
						if( money[i][0].attr.shisan_zino == 0x03/*8%*/ ){
							m_Util.percent( Bkz, Tag, 10, 80, 0 );
							m_Arith.l_add( Tag, Tag, Bkz );
						}
						else if( money[i][0].attr.shisan_zino == 0x04/*8%軽減*/ ){
							m_Util.percent( Bkz, Tag, 10, 80, 0 );
							m_Arith.l_add( Tag, Tag, Bkz );
						}
						else if (money[i][0].attr.shisan_zino == 0x05/*10%*/) {//清水
							m_Util.percent(Bkz, Tag, 14, 100, 0);
							m_Arith.l_add(Tag, Tag, Bkz);
						}
					}
				}
				else{
					m_Arith.l_add( Tag, Tag, money[i][0].arith );
				}
				IsKziKmk = FALSE;
			}
			else if( IsTkkzRelKmk(&money[i][0]) == TRUE ){
				if( (money[i][0].attr.shisan_gp_kubun&0x80) == 0x00 ){
					continue;
				}
				else{
					m_Arith.l_add( Tag, Tag, money[i][0].arith );
					IsKziKmk = FALSE;
				}
			}
			// 課税取引
			else{
				if( shinsw ){
					m_Arith.l_add( Kzi, Kzi, money[i][2].arith );
					if( money[i][0].attr.shisan_beki ){
						if( money[i][0].attr.shisan_zino == 0x03/*8%*/ ){
							m_Util.percent( Bkz, Kzi, 10, 80, 0 );
							m_Arith.l_add( Kzi, Kzi, Bkz );
						}
						else if( money[i][0].attr.shisan_zino == 0x04/*8%軽減*/ ){
							m_Util.percent( Bkz, Kzi, 10, 80, 0 );
							m_Arith.l_add( Kzi, Kzi, Bkz );
						}
						else if (money[i][0].attr.shisan_zino == 0x05/*10%*/) {//清水メモ
							m_Util.percent(Bkz, Kzi, 14, 100, 0);
							m_Arith.l_add(Kzi, Kzi, Bkz);
						}
					}
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
						pKsKkval = &NRec.KUval[KSELLINE7];
						pTgKkval = &NRec.KUval[KSELLINE8];
					}
					else{
						pKsKkval = &NRec.KUval[KSELLINE7];
						pTgKkval = &NRec.KUval[KSELLINE8];
					}
				}
				else{							// 取得
					if( IsTransitionalMeasure ){
						pKsKkval = &NRec.KSval[KSELLINE7];
						pTgKkval = &NRec.KSval[KSELLINE8];
					}
					else{
						pKsKkval = &NRec.KSval[KSELLINE7];
						pTgKkval = &NRec.KSval[KSELLINE8];
					}
				}

				if( pKsKkval && pTgKkval ){
					m_Arith.l_add( pKsKkval->KKval1, pKsKkval->KKval1, Kzn );
					m_Arith.l_add( pTgKkval->KKval1, pTgKkval->KKval1, Kot );
					if( IsKziKmk ){
						if( money[i][0].attr.sc_zino == 0x03/*8%*/ ){
							m_Arith.l_add( pKsKkval->KKval2, pKsKkval->KKval2, Kzn );
							m_Arith.l_add( pTgKkval->KKval2, pTgKkval->KKval2, Kot );
						}
						else if( money[i][0].attr.sc_zino == 0x04/*8%軽減*/ ){
							m_Arith.l_add( pKsKkval->KKval3, pKsKkval->KKval3, Kzn );
							m_Arith.l_add( pTgKkval->KKval3, pTgKkval->KKval3, Kot );
						}
						else if (money[i][0].attr.sc_zino == 0x05/*10%*/) {
							m_Arith.l_add( pKsKkval->KKval4, pKsKkval->KKval4, Kzn);//清水メモ
							m_Arith.l_add( pTgKkval->KKval4, pTgKkval->KKval4, Kot );
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
							pKkval = &NRec.KJval[34];
							break;
						case 0x02 :	// 不動産
							pKkval = &NRec.KFval[15];
							break;
						case 0x04 :	// 農業
							pKkval = &NRec.KNval[38];
							break;
						default   :
							break;
					}

					if( pKkval ){
						if( (!(knrec.Kn_att&0x01) && (pKkval->KKtype&0x01)) ||
							((knrec.Kn_att&0x01) && !(pKkval->KKtype&0x01)) ){
							m_Arith.l_neg( Kzi );
						}
						m_Arith.l_add( pKkval->KKval1, pKkval->KKval1, Kzi );
						if( IsKziKmk ){
							if( money[i][0].attr.sc_zino == 0x03/*8%*/ ){
								m_Arith.l_add( pKkval->KKval4, pKkval->KKval4, Kzi );
							}
							else if( money[i][0].attr.sc_zino == 0x04/*8%軽減*/ ){
								m_Arith.l_add( pKkval->KKval5, pKkval->KKval5, Kzi );
							}
							else if (money[i][0].attr.sc_zino == 0x05/*10%*/) {//清水メモ
								m_Arith.l_add(pKkval->KKval6, pKkval->KKval6, Kzi);
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
					if( (!(knrec.Kn_att&0x01) && (pKkval->KKtype&0x01)) ||
						((knrec.Kn_att&0x01) && !(pKkval->KKtype&0x01)) ){
						m_Arith.l_neg( Kzn );
						m_Arith.l_neg( Kot );
					}
					if( IsTagKmk == FALSE ){
						m_Arith.l_add( pKkval->KKval1, pKkval->KKval1, Kzn );
						m_Arith.l_add( pKkval->KKval2, pKkval->KKval2, Kot );
					}
					else{
						m_Arith.l_sub( pKkval->KKval2, pKkval->KKval2, Kzi );
					}
					if( IsKziKmk ){
						if( money[i][0].attr.sc_zino == 0x03/*8%*/ ){
							m_Arith.l_add( pKkval->KKval4, pKkval->KKval4, Kzn );
						}
						else if( money[i][0].attr.sc_zino == 0x04/*8%軽減*/ ){
							m_Arith.l_add( pKkval->KKval5, pKkval->KKval5, Kzn );
						}
						else if (money[i][0].attr.sc_zino == 0x05/*10%*/) {//清水メモ
							m_Arith.l_add(pKkval->KKval6, pKkval->KKval6, Kzn);
						}
					}
				}
			}
		}
	}

	return 0;
}


//-----------------------------------------------------------------------------
// 財務連動
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
int CH31TaxListData::ConnectZmdata( CDBNpSub *pDBNpSub, CSyzSyukei *pSyzSyukei, int smon, int emon, BOOL IsTransitionalMeasure, CSnHeadData *pSnHeadData )
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
		memset( NRec.KJval[i].KKval6, '\0', sizeof(NRec.KJval[i].KKval6) );
// インボイス_23/07/21_higuchi add -->
		memset( NRec.KJval[i].KKval7, '\0', sizeof(NRec.KJval[i].KKval7) );
		memset( NRec.KJval[i].KKval8, '\0', sizeof(NRec.KJval[i].KKval8) );
// インボイス_23/07/21_higuchi add <--
		// 課税取引金額計算表（不動産取得用）
		NRec.KFval[i].KKauto = 0x00;
		memset( NRec.KFval[i].KKval1, '\0', sizeof(NRec.KFval[i].KKval1) );
		memset( NRec.KFval[i].KKval2, '\0', sizeof(NRec.KFval[i].KKval2) );
		memset( NRec.KFval[i].KKval3, '\0', sizeof(NRec.KFval[i].KKval3) );
		memset( NRec.KFval[i].KKval4, '\0', sizeof(NRec.KFval[i].KKval4) );
		memset( NRec.KFval[i].KKval5, '\0', sizeof(NRec.KFval[i].KKval5) );
		memset( NRec.KFval[i].KKval6, '\0', sizeof(NRec.KFval[i].KKval6) );
// インボイス_23/07/21_higuchi add -->
		memset( NRec.KFval[i].KKval7, '\0', sizeof(NRec.KFval[i].KKval7) );
		memset( NRec.KFval[i].KKval8, '\0', sizeof(NRec.KFval[i].KKval8) );
// インボイス_23/07/21_higuchi add <--
		// 課税取引金額計算表（農業得用）
		NRec.KNval[i].KKauto = 0x00;
		memset( NRec.KNval[i].KKval1, '\0', sizeof(NRec.KNval[i].KKval1) );
		memset( NRec.KNval[i].KKval2, '\0', sizeof(NRec.KNval[i].KKval2) );
		memset( NRec.KNval[i].KKval3, '\0', sizeof(NRec.KNval[i].KKval3) );
		memset( NRec.KNval[i].KKval4, '\0', sizeof(NRec.KNval[i].KKval4) );
		memset( NRec.KNval[i].KKval5, '\0', sizeof(NRec.KNval[i].KKval5) );
		memset( NRec.KNval[i].KKval6, '\0', sizeof(NRec.KNval[i].KKval6) );
// インボイス_23/07/21_higuchi add -->
		memset( NRec.KNval[i].KKval7, '\0', sizeof(NRec.KNval[i].KKval7) );
		memset( NRec.KNval[i].KKval8, '\0', sizeof(NRec.KNval[i].KKval8) );
// インボイス_23/07/21_higuchi add <--

		if( (i==KSELLINE4) || (i==KSELLINE5) || (i==KSELLINE6) ){
			continue;
		}

		// 課税売上計算表
		NRec.KUval[i].KKauto = 0x00;
		memset( NRec.KUval[i].KKval1, '\0', sizeof(NRec.KUval[i].KKval1) );
		memset( NRec.KUval[i].KKval2, '\0', sizeof(NRec.KUval[i].KKval2) );
		memset( NRec.KUval[i].KKval3, '\0', sizeof(NRec.KUval[i].KKval3) );
		memset( NRec.KUval[i].KKval4, '\0', sizeof(NRec.KUval[i].KKval4) );
		memset( NRec.KUval[i].KKval5, '\0', sizeof(NRec.KUval[i].KKval5) );
		memset( NRec.KUval[i].KKval6, '\0', sizeof(NRec.KUval[i].KKval6) );
// インボイス_23/07/21_higuchi add -->
		memset( NRec.KUval[i].KKval7, '\0', sizeof(NRec.KUval[i].KKval7) );
		memset( NRec.KUval[i].KKval8, '\0', sizeof(NRec.KUval[i].KKval8) );
// インボイス_23/07/21_higuchi add <--
		// 課税仕入計算表
		NRec.KSval[i].KKauto = 0x00;
		memset( NRec.KSval[i].KKval1, '\0', sizeof(NRec.KSval[i].KKval1) );
		memset( NRec.KSval[i].KKval2, '\0', sizeof(NRec.KSval[i].KKval2) );
		memset( NRec.KSval[i].KKval3, '\0', sizeof(NRec.KSval[i].KKval3) );
		memset( NRec.KSval[i].KKval4, '\0', sizeof(NRec.KSval[i].KKval4) );
		memset( NRec.KSval[i].KKval5, '\0', sizeof(NRec.KSval[i].KKval5) );
		memset( NRec.KSval[i].KKval6, '\0', sizeof(NRec.KSval[i].KKval6) );
// インボイス_23/07/21_higuchi add -->
		memset( NRec.KSval[i].KKval7, '\0', sizeof(NRec.KSval[i].KKval7) );
		memset( NRec.KSval[i].KKval8, '\0', sizeof(NRec.KSval[i].KKval8) );
// インボイス_23/07/21_higuchi add <--
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
				if( lstKsOfs == j ){
					k = 0;
				}
				else{
					k = 1;
				}
				break;
			case 2 :
				if( lstKsOfs == j ){
					k = 1;
				}
				else{
					k = 2;
				}
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
				if( lstKsOfs == j ){
					k = 3;
				}
				else{
					k = 4;
				}
				break;
			case 6 :
				if( lstKsOfs == j ){
					k = 4;
				}
				else{
					k = 5;
				}
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
				if( lstKsOfs == j ){
					k = 6;
				}
				else{
					k = 7;
				}
				break;
			case 10:
				if( lstKsOfs == j ){
					k = 7;
				}
				else{
					k = 8;
				}
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
				if( lstKsOfs == j ){
					k = 9;
				}
				else{
					k = 10;
				}
				break;
			case 14:
				if( lstKsOfs == j ){
					k = 10;
				}
				else{
					k = 11;
				}
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
			if( knrec.Kn_att&0x01 ){
				m_Arith.l_sub( Val, Ksi, Kri );
			}
			else{
				m_Arith.l_sub( Val, Kri, Ksi );
			}
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
			if( (!(knrec.Kn_att&0x01) && (NRec.KJval[Putno].KKtype&0x01)) ||
				((knrec.Kn_att&0x01) && !(NRec.KJval[Putno].KKtype&0x01)) ){
				m_Arith.l_neg( Kzn );
			}
			m_Arith.l_add( NRec.KJval[Putno].KKval1, NRec.KJval[Putno].KKval1, Kzn );
			m_Arith.l_add( NRec.KJval[Putno].KKval2, NRec.KJval[Putno].KKval2, Kzn );
			break;

		case 0x02 :	// 不動産									   
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

											//			shisan_zino
											//			0x00：5%
											//			0x01：3%
											//			0x03：8%
											//			0x04：8%軽減
											//			0x05：10%

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
			//else if( (money[i][0].attr.sc_kbn1==0x04) && (money[i][0].attr.sc_kbn2==0x04) ){
			else if ( ( ( money[i][0].attr.sc_kbn1 == 0x04 ) && ( money[i][0].attr.sc_kbn2 == 0x04 ) ) ||
					  ( ( money[i][0].attr.sc_kbn1 == 0x04 ) && ( money[i][0].attr.sc_kbn2 == 0x11 ) ) ||
					  ( ( money[i][0].attr.sc_kbn1 == 0x04 ) && ( money[i][0].attr.sc_kbn2 == 0x12 ) ) ||
					  ( ( money[i][0].attr.sc_kbn1 == 0x04 ) && ( money[i][0].attr.sc_kbn2 == 0x13 ) ) ) {
				if ( money[i][0].attr.sc_kbn2 == 0x04 ) {
					if( shinsw ){
						// 税込
						m_Arith.l_add( Tag, Tag, money[i][2].arith );
						if( money[i][0].attr.shisan_beki ){
							if( money[i][0].attr.shisan_zino == 0x03/*8%*/ ){
								m_Util.percent( Bkz, Tag, 10, 80, 0 );
								m_Arith.l_add( Tag, Tag, Bkz );
							}
							else if( money[i][0].attr.shisan_zino == 0x04/*8%軽減*/ ){
								m_Util.percent( Bkz, Tag, 10, 80, 0 );
								m_Arith.l_add( Tag, Tag, Bkz );
							}
							else if (money[i][0].attr.shisan_zino == 0x05/*10%*/) {//清水
								m_Util.percent(Bkz, Tag, 14, 100, 0);
								m_Arith.l_add(Tag, Tag, Bkz);
							}
						}
					}
					else{
						m_Arith.l_add( Tag, Tag, money[i][0].arith );
					}
					IsKziKmk = FALSE;
				}
				else if ( ( money[i][0].attr.sc_kbn2 == 0x11 ) || ( money[i][0].attr.sc_kbn2 == 0x12 ) || ( money[i][0].attr.sc_kbn2 == 0x13 ) ) {
					// 輸入仕入 国税, 地方税, 国地合算は集計しない
				}
			}
			else if( IsTkkzRelKmk(&money[i][0]) == TRUE ){
				if( (money[i][0].attr.shisan_gp_kubun&0x80) == 0x00 ){
					continue;
				}
				else{
					m_Arith.l_add( Tag, Tag, money[i][0].arith );
					IsKziKmk = FALSE;
				}
			}
			// 課税取引
			else{
				if( shinsw ){
					m_Arith.l_add( Kzi, Kzi, money[i][2].arith );
					// 簡易計算がONの場合には税込みの金額を受け取っているため集計しない
					if ( ( pSnHeadData->Sn_SPECIAL & 0x01 ) == 0x00 ) {
						if( money[i][0].attr.shisan_beki ){	
// 23/10/11_higuchi cor -->
							//集計方法を期間をみて分ける様に変更('23.10.18)		修正依頼163688
							long			eymd = 0;
							if (!(pSnHeadData->Sn_SKKBN % 2)) {
								eymd = pSnHeadData->Sn_MDAYE;
							}
							else {
								eymd = pSnHeadData->Sn_KDAYE;
							}
							if (eymd < ICS_SH_INVOICE_DAY) {
								if( money[i][0].attr.shisan_zino == 0x03/*8%*/ ){
									m_Util.percent( Bkz, Kzi, 10, 80, 0 );
									m_Arith.l_add( Kzi, Kzi, Bkz );
								}
								else if( money[i][0].attr.shisan_zino == 0x04/*8%軽減*/ ){
									m_Util.percent( Bkz, Kzi, 10, 80, 0 );
									m_Arith.l_add( Kzi, Kzi, Bkz );
								}
								else if (money[i][0].attr.shisan_zino == 0x05/*10%*/) {//清水メモ
									m_Util.percent(Bkz, Kzi, 14, 100, 0);
									m_Arith.l_add(Kzi, Kzi, Bkz);
								}
							}
							else {
// ------------------------
								char	dest[6] = { 0 };	// 答え
								char	src[6] = { 0 };		// 元の金額
								char	nume[6] = { 0 };	// 分子
								char	deno[6] = { 0 };	// 分母
								memcpy(src, Kzi, 6);
								// 8%
								if (money[i][0].attr.shisan_zino == 0x03) {
									// 50%控除
									if (money[i][0].attr.sc_zchg & 0x20) {
										m_Arith.l_input(nume, _T("1080"));
										m_Arith.l_input(deno, _T("1040"));
									}
									// 80%控除
									else if (money[i][0].attr.sc_zchg & 0x10) {
										m_Arith.l_input(nume, _T("1080"));
										m_Arith.l_input(deno, _T("1016"));
									}
									// 「免税事業者等からの課税仕入れ」ではない
									else {
										m_Arith.l_input(nume, _T("1080"));
										m_Arith.l_input(deno, _T("1000"));
									}
								}
								// 軽減8%
								else if (money[i][0].attr.shisan_zino == 0x04) {
									// 50%控除
									if (money[i][0].attr.sc_zchg & 0x20) {
										m_Arith.l_input(nume, _T("1080"));
										m_Arith.l_input(deno, _T("1040"));
									}
									// 80%控除
									else if (money[i][0].attr.sc_zchg & 0x10) {
										m_Arith.l_input(nume, _T("1080"));
										m_Arith.l_input(deno, _T("1016"));
									}
									// 「免税事業者等からの課税仕入れ」ではない
									else {
										m_Arith.l_input(nume, _T("1080"));
										m_Arith.l_input(deno, _T("1000"));
									}
								}
								// 10%
								else if (money[i][0].attr.shisan_zino == 0x05) {
									// 50%控除
									if (money[i][0].attr.sc_zchg & 0x20) {
										m_Arith.l_input(nume, _T("1100"));
										m_Arith.l_input(deno, _T("1050"));
									}
									// 80%控除
									else if (money[i][0].attr.sc_zchg & 0x10) {
										m_Arith.l_input(nume, _T("1100"));
										m_Arith.l_input(deno, _T("1020"));
									}
									// 「免税事業者等からの課税仕入れ」ではない
									else {
										m_Arith.l_input(nume, _T("1100"));
										m_Arith.l_input(deno, _T("1000"));
									}
								}
								// 3%
								else if (money[i][0].attr.shisan_zino == 0x01) {
									// 50%控除
									if (money[i][0].attr.sc_zchg & 0x20) {
										m_Arith.l_input(nume, _T("1030"));
										m_Arith.l_input(deno, _T("1015"));
									}
									// 80%控除
									else if (money[i][0].attr.sc_zchg & 0x10) {
										m_Arith.l_input(nume, _T("1030"));
										m_Arith.l_input(deno, _T("1006"));
									}
									// 「免税事業者等からの課税仕入れ」ではない
									else {
										m_Arith.l_input(nume, _T("1030"));
										m_Arith.l_input(deno, _T("1000"));
									}
								}
								// 5%
								else {
									// 50%控除
									if (money[i][0].attr.sc_zchg & 0x20) {
										m_Arith.l_input(nume, _T("1050"));
										m_Arith.l_input(deno, _T("1025"));
									}
									// 80%控除
									else if (money[i][0].attr.sc_zchg & 0x10) {
										m_Arith.l_input(nume, _T("1050"));
										m_Arith.l_input(deno, _T("1010"));
									}
									// 「免税事業者等からの課税仕入れ」ではない
									else {
										m_Arith.l_input(nume, _T("1050"));
										m_Arith.l_input(deno, _T("1000"));
									}
								}
								l_pardotnet(dest, src, nume, deno);
								memcpy(Kzi, dest, 6);
							}
// 23/10/11_higuchi cor <--
						}
					}
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
						pKsKkval = &NRec.KUval[KSELLINE7];
						pTgKkval = &NRec.KUval[KSELLINE8];
					}
					else{
						pKsKkval = &NRec.KUval[KSELLINE7];
						pTgKkval = &NRec.KUval[KSELLINE8];
					}
				}
				else{							// 取得
					if( IsTransitionalMeasure ){
						pKsKkval = &NRec.KSval[KSELLINE7];
						pTgKkval = &NRec.KSval[KSELLINE8];
					}
					else{
						pKsKkval = &NRec.KSval[KSELLINE7];
						pTgKkval = &NRec.KSval[KSELLINE8];
					}
				}

				if( pKsKkval && pTgKkval ){
					m_Arith.l_add( pKsKkval->KKval1, pKsKkval->KKval1, Kzn );
					m_Arith.l_add( pTgKkval->KKval1, pTgKkval->KKval1, Kot );
					if( IsKziKmk ){
						if( money[i][0].attr.sc_zino == 0x03/*8%*/ ){
							m_Arith.l_add( pKsKkval->KKval2, pKsKkval->KKval2, Kzn );
							m_Arith.l_add( pTgKkval->KKval2, pTgKkval->KKval2, Kot );
						}
						else if( money[i][0].attr.sc_zino == 0x04/*8%軽減*/ ){
// インボイス_23/07/21_higuchi cor -->
							//m_Arith.l_add( pKsKkval->KKval3, pKsKkval->KKval3, Kzn );
							//m_Arith.l_add( pTgKkval->KKval3, pTgKkval->KKval3, Kot );
// -----------------------------------
							if((money[i][0].attr.sc_zchg & 0x10) == 0)	{
								m_Arith.l_add( pKsKkval->KKval3, pKsKkval->KKval3, Kzn );
								m_Arith.l_add( pTgKkval->KKval3, pTgKkval->KKval3, Kot );
							}
							else	{
								m_Arith.l_add( pKsKkval->KKval7, pKsKkval->KKval7, Kzn );
								m_Arith.l_add( pTgKkval->KKval7, pTgKkval->KKval7, Kot );
							}
// インボイス_23/07/21_higuchi cor <--
						}
						else if (money[i][0].attr.sc_zino == 0x05/*10%*/) {
// インボイス_23/07/21_higuchi cor -->
							//m_Arith.l_add( pKsKkval->KKval4, pKsKkval->KKval4, Kzn);//清水メモ
							//m_Arith.l_add( pTgKkval->KKval4, pTgKkval->KKval4, Kot );
// -----------------------------------
							if((money[i][0].attr.sc_zchg & 0x10) == 0)	{
								m_Arith.l_add( pKsKkval->KKval4, pKsKkval->KKval4, Kzn);//清水メモ
								m_Arith.l_add( pTgKkval->KKval4, pTgKkval->KKval4, Kot );
							}
							else	{
								m_Arith.l_add( pKsKkval->KKval8, pKsKkval->KKval8, Kzn);//清水メモ
								m_Arith.l_add( pTgKkval->KKval8, pTgKkval->KKval8, Kot );
							}
// インボイス_23/07/21_higuchi cor <--
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
						pKkval = &NRec.KJval[34];
						break;
					case 0x02 :	// 不動産
						pKkval = &NRec.KFval[15];
						break;
					case 0x04 :	// 農業
						pKkval = &NRec.KNval[38];
						break;
					default   :
						break;
					}

					if( pKkval ){
						if( (!(knrec.Kn_att&0x01) && (pKkval->KKtype&0x01)) ||
							((knrec.Kn_att&0x01) && !(pKkval->KKtype&0x01)) ){
							m_Arith.l_neg( Kzi );
						}
						m_Arith.l_add( pKkval->KKval1, pKkval->KKval1, Kzi );
						if( IsKziKmk ){
							if( money[i][0].attr.sc_zino == 0x03/*8%*/ ){
								m_Arith.l_add( pKkval->KKval4, pKkval->KKval4, Kzi );
							}
							else if( money[i][0].attr.sc_zino == 0x04/*8%軽減*/ ){
// インボイス_23/07/21_higuchi cor -->
								//m_Arith.l_add( pKkval->KKval5, pKkval->KKval5, Kzi );
// -----------------------------------
								if((money[i][0].attr.sc_zchg & 0x10) == 0)	{
									m_Arith.l_add( pKkval->KKval5, pKkval->KKval5, Kzi );
								}
								else	{
									m_Arith.l_add( pKkval->KKval7, pKkval->KKval7, Kzi );
								}
// インボイス_23/07/21_higuchi cor <--
							}
							else if (money[i][0].attr.sc_zino == 0x05/*10%*/) {//清水メモ
// インボイス_23/07/21_higuchi cor -->
								//m_Arith.l_add(pKkval->KKval6, pKkval->KKval6, Kzi);
// -----------------------------------
								if((money[i][0].attr.sc_zchg & 0x10) == 0)	{
									m_Arith.l_add(pKkval->KKval6, pKkval->KKval6, Kzi);
								}
								else	{
									m_Arith.l_add(pKkval->KKval8, pKkval->KKval8, Kzi);
								}
// インボイス_23/07/21_higuchi cor <--
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
					if( (!(knrec.Kn_att&0x01) && (pKkval->KKtype&0x01)) ||
						((knrec.Kn_att&0x01) && !(pKkval->KKtype&0x01)) ){
						m_Arith.l_neg( Kzn );
						m_Arith.l_neg( Kot );
					}
					if( IsTagKmk == FALSE ){
						m_Arith.l_add( pKkval->KKval1, pKkval->KKval1, Kzn );
						m_Arith.l_add( pKkval->KKval2, pKkval->KKval2, Kot );
					}
					else{
						m_Arith.l_sub( pKkval->KKval2, pKkval->KKval2, Kzi );
					}
					if( IsKziKmk ){
						if( money[i][0].attr.sc_zino == 0x03/*8%*/ ){
							m_Arith.l_add( pKkval->KKval4, pKkval->KKval4, Kzn );
						}
						else if( money[i][0].attr.sc_zino == 0x04/*8%軽減*/ ){
// インボイス_23/07/21_higuchi cor -->
							//m_Arith.l_add( pKkval->KKval5, pKkval->KKval5, Kzn );
// -----------------------------------
							if((money[i][0].attr.sc_zchg & 0x10) == 0)	{
								m_Arith.l_add( pKkval->KKval5, pKkval->KKval5, Kzn );
							}
							else	{
								m_Arith.l_add( pKkval->KKval7, pKkval->KKval7, Kzn );
							}
// インボイス_23/07/21_higuchi cor <--
						}
						else if (money[i][0].attr.sc_zino == 0x05/*10%*/) {//清水メモ
// インボイス_23/07/21_higuchi cor -->
							//m_Arith.l_add(pKkval->KKval6, pKkval->KKval6, Kzn);
// -----------------------------------
							if((money[i][0].attr.sc_zchg & 0x10) == 0)	{
								m_Arith.l_add(pKkval->KKval6, pKkval->KKval6, Kzn);
							}
							else	{
								m_Arith.l_add(pKkval->KKval8, pKkval->KKval8, Kzn);
							}
// インボイス_23/07/21_higuchi cor <--
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
int CH31TaxListData::GetRecodeNo( char type, char ctg, int cnt )
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
int CH31TaxListData::ReadKnTabl( CDBNpSub *pDBNpSub )
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
BOOL CH31TaxListData::IsTkkzRelKmk( MoneyBase* pMbase )
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
int CH31TaxListData::GetKoketsuMeisyo( CDBNpSub *pDBNpSub, int type, int no, CString &name )
{
	pDBNpSub->kprf->MoveFirst();

	CString tmp,prf_no_tmp;
	tmp.Format("%d",no);

	// 白色申告用のprf_noをセットする。青色申告はそのままでOK
	if( pDBNpSub->zvol->ind_type & 0x10 ){
		if( type & 0x01 ){	// 事業
			if( no == 25 ){
				tmp.Format( "%s", "ｦ" );
			}
			if( no == 26 ){
				tmp.Format( "%s", "ﾜ" );
			}
			if( no == 27 ){
				tmp.Format( "%s", "ｶ" );
			}
			if( no == 28 ){
				tmp.Format( "%s", "ﾖ" );
			}
			if( no == 29 ){
				tmp.Format( "%s", "ﾀ" );
			}
			if( no == 30 ){
				tmp.Format( "%s", "ﾚ" );
			}
		}
		else if( type & 0x02 ){	// 不動産
			if( no == 3 ){
				;// 収入金額は白色申告も青色申告も同じprf_no
			}
			if( no == 12 ){
				//tmp.Format( "%s", "ﾆ" );
				tmp.Format( "%d", 8 );	// 白色の場合は「その他経費」ではなく「貸倒金」をセット
			}
		}
		else if( type & 0x04 ){	// 農業
			if( no == 26 ){
				tmp.Format( "%s", "ﾖ" );
			}
			//--> 24/02/29 h.wada change 
			//収支(農業)で個別決算書の項目名称が正しく取得できていなかったため修正(従前から)
			//個別決算書の経費1～4を課税取引金額計算表(農業)の経費1～経費3に転記する際、
			//農業所得.経費1 = 個別決算書.経費1 + 個別決算書.経費2、 農業所得.経費2 = 個別決算書.経費3、 農業所得.経費3 = 個別決算書.経費4 から
			//農業所得.経費1 = 個別決算書.経費1、 農業所得.経費2 = 個別決算書.経費2、 農業所得.経費3 = 個別決算書.経費3 + 個別決算書.経費4 に仕様変更した際の対応漏れ？
			if( no == 27/*28*/ ){
				tmp.Format( "%s", "ﾀ" );
			}
			if( no == 28/*29*/ ){
				tmp.Format( "%s", "ﾚ" );
			}
			//<-- 24/02/29 h.wada change 
		}
	}


	while(1){
		//if(( pDBNpSub->kprf->prf_type == type )&&( pDBNpSub->kprf->prf_no == no )&&( pDBNpSub->kprf->sgn2 == 0 )){
		prf_no_tmp = pDBNpSub->kprf->prf_no;
		prf_no_tmp.Trim();

		if( type & 0x02 ){
			if(( pDBNpSub->kprf->prf_type == type )&&( strcmp(prf_no_tmp, tmp) == 0 )){
				name = pDBNpSub->kprf->prf_name;
				break;
			}else{
				if( pDBNpSub->kprf->MoveNext() == -1 ){
					return -1;
				}
			}
		}
		else{
			if(( pDBNpSub->kprf->prf_type == type )&&( strcmp(prf_no_tmp, tmp) == 0 )&&( pDBNpSub->kprf->sgn2 == 0 )){
				name = pDBNpSub->kprf->prf_name;
				break;
			}else{
				if( pDBNpSub->kprf->MoveNext() == -1 ){
					return -1;
				}
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
bool CH31TaxListData::IsLinkKjprfname()
{
	if( Head.KHkoke == 1 ){
		return true;
	}
	else{
		return false;
	}
}


//-----------------------------------------------------------------------------
// ８％→１０％へのコンバージョン
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー不正終了
//-----------------------------------------------------------------------------
int CH31TaxListData::GetPastData( CDBNpSub *pDBNpSub, CSnHeadData *pSnHeadData, int sn_seq )
{

	// 連動のときは過去データを移さない
	if( !(pSnHeadData->Sn_Sign4 & 0x80) ){
		return 0;
	}

	CString			filter;
	CDBSyzAccess*	pDBSyzAcc;

	// 消費税拡張処理クラス
	pDBSyzAcc = NULL;
	pDBSyzAcc = new CDBSyzAccess;
	if( pDBSyzAcc == NULL ){
		return ERR;
	}
	// オープン
	filter.Format( _T("kssgn5 = %d and kslist >= 1 and kslist <= 6"), sn_seq );	// ８％のデータ
	if( pDBSyzAcc->KjsnkOpen( pDBNpSub->m_database, filter ) == ERR ) {
		delete pDBSyzAcc;
		return ERR;
	}

	memset( &Head, '\0', sizeof( KJNHEAD ) );
	memset( &NRec, '\0', sizeof( KJNREC ) );
	char	yy=0, mm=0, dd=0;
	pSnHeadData->GetYmdDataGen( ID_ICSSH_KESSAN_TO, &yy, &mm, &dd );
	YEAR = yy;

	
	// 業種区分
	if( !(GKBN = pDBNpSub->zvol->ind_type) ){
		GKBN = 0x07;
	}
	int kjct = pDBSyzAcc->m_pKjsnk->GetRecordCount();	

	// 管理部
	if( m_Util.IsUnConnectMst(pDBNpSub->zvol) ){
		// 非連動時
		if( kjct == 0 ){
			// 新規
			pSnHeadData->SVsign &= 0xdf;
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
			// 令和5年1月1日以降の課税期間の場合は、会社登録の経理方式の設定に従う
			long symd = 0, eymd = 0;
			GetKazeiKikan( pSnHeadData, symd, eymd );
			if( symd >= R05_SDAY ) {
				if( pDBNpSub->zvol->s_sgn1 & 0xc0 ){
					// 税抜き
					pSnHeadData->SVsign &= 0xdf;
					Head.KHstax &= 0xfe;
					Head.KHstax |= 0x01;
				}
				else {
					// 税込み
					pSnHeadData->SVsign &= 0xdf;
					pSnHeadData->SVsign |= 0x20;
					Head.KHstax &= 0xfe;
				}
			}
			else {
				// メンテ
				if( pSnHeadData->Sn_Sign2&0x08 ){
					Head.KHstax = 0x01;
				}
				else{
					Head.KHstax = 0x00;
				}
			}
		}
		Head.KHkoke = 0x00;
		pSnHeadData->Sn_Sign4 &= ~0x02;
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
		if( pSnHeadData->Sn_Sign4&0x02 ){
			Head.KHkoke = 0x01;
		}else{
			Head.KHkoke = 0x00;
		}
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
		NRec.KSval[Rct].KKtype = KkVal5[Rct].KKtype;
		NRec.KSval[Rct].KKsign = KkVal5[Rct].KKsign;
		NRec.KSval[Rct].KKauto = KkVal5[Rct].KKauto;
		Rct++;
		Count++;
	}
	NRec.KSval[Rct].KKtype = 0xFF;




	// 過去データの読み込み
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
			case 1 :	pKkval = &NRec.KJval[pDBSyzAcc->m_pKjsnk->ksline-1];	break;	// 事業所得	
			case 2 :	pKkval = &NRec.KFval[pDBSyzAcc->m_pKjsnk->ksline-1];	break;	// 不動産
			case 3 :	pKkval = &NRec.KNval[pDBSyzAcc->m_pKjsnk->ksline-1];	break;	// 農業
			case 4 :	pKkval = &NRec.KUval[pDBSyzAcc->m_pKjsnk->ksline-1];	break;	// 売上高
			case 5 :	pKkval = &NRec.KSval[pDBSyzAcc->m_pKjsnk->ksline-1];	break;	// 仕入高
			case 6 :	pKkval = &NRec.KUval[pDBSyzAcc->m_pKjsnk->ksline-1];	break;	// 事業別課税売上高
			default:	flg = 1;	break;
		}
		if( flg )	continue;


		int ksline = pDBSyzAcc->m_pKjsnk->ksline;

		switch( pDBSyzAcc->m_pKjsnk->kslist )	{

			case 1 :	if( ksline == 34 ){	// 返還等対価
							pKkval = &NRec.KJval[34];
						}
						m_Util.cstring_to_char( (unsigned char *)&pKkval->KKnam, pDBSyzAcc->m_pKjsnk->ksname, 40 );	// 名称
						m_Util.val_to_bin( (unsigned char*)pKkval->KKval1,	pDBSyzAcc->m_pKjsnk->ksval0 );			// 決算額
						m_Util.val_to_bin( (unsigned char*)pKkval->KKval2,	pDBSyzAcc->m_pKjsnk->ksval1 );			// 課税取引にならないもの
						m_Util.val_to_bin( (unsigned char*)pKkval->KKval4,	pDBSyzAcc->m_pKjsnk->ksval4 );			// 6.3％
						if( pDBSyzAcc->m_pKjsnk->kssgn3 & 0x01 ){
							pKkval->KKauto |= pDBSyzAcc->m_pKjsnk->kssgn3;
						}
						if( pDBSyzAcc->m_pKjsnk->kssgn3 & 0x04 ){	// 10％では6.3％の手入力サインが0x02のため、ここで変換
							pKkval->KKauto |= 0x02;
						}
						break;

			case 2 :    m_Util.cstring_to_char( (unsigned char *)&pKkval->KKnam, pDBSyzAcc->m_pKjsnk->ksname, 40 );	// 名称
						m_Util.val_to_bin( (unsigned char*)pKkval->KKval1,	pDBSyzAcc->m_pKjsnk->ksval0 );			// 決算額
						m_Util.val_to_bin( (unsigned char*)pKkval->KKval2,	pDBSyzAcc->m_pKjsnk->ksval1 );			// 課税取引にならないもの
						m_Util.val_to_bin( (unsigned char*)pKkval->KKval4,	pDBSyzAcc->m_pKjsnk->ksval4 );			// 6.3%
						if( pDBSyzAcc->m_pKjsnk->kssgn3 & 0x01 ){
							pKkval->KKauto |= pDBSyzAcc->m_pKjsnk->kssgn3;
						}
						if( pDBSyzAcc->m_pKjsnk->kssgn3 & 0x04 ){
							pKkval->KKauto |= 0x02;
						}
						break;

			case 3 :	m_Util.cstring_to_char( (unsigned char *)&pKkval->KKnam, pDBSyzAcc->m_pKjsnk->ksname, 40 );	// 名称
						m_Util.val_to_bin( (unsigned char*)pKkval->KKval1,	pDBSyzAcc->m_pKjsnk->ksval0 );			// 決算額
						m_Util.val_to_bin( (unsigned char*)pKkval->KKval2,	pDBSyzAcc->m_pKjsnk->ksval1 );			// 課税取引にならないもの
						m_Util.val_to_bin( (unsigned char*)pKkval->KKval3,	pDBSyzAcc->m_pKjsnk->ksval2 );			// 課税取引金額
						m_Util.val_to_bin( (unsigned char*)pKkval->KKval4,	pDBSyzAcc->m_pKjsnk->ksval4 );			// 6.3%
						if( pDBSyzAcc->m_pKjsnk->kssgn3 & 0x01 ){
							pKkval->KKauto |= pDBSyzAcc->m_pKjsnk->kssgn3;
						}
						if( pDBSyzAcc->m_pKjsnk->kssgn3 & 0x04 ){
							pKkval->KKauto |= 0x02;
						}
						break;

			case 4 :	if( ksline == 1 ){
							m_Util.cstring_to_char( (unsigned char *)&Head.KHkunm, pDBSyzAcc->m_pKjsnk->ksname, 10 );	// 名称
							continue;
						}
						else if( ksline == 10 ){
							pKkval = &NRec.KUval[3];
						}
						else if( ksline == 11 ){
							pKkval = &NRec.KUval[4];
						}
						else if( ksline == 12 ){	// 集計等のからみがあるため、6.3％の金額を4行目に移す
							pKkval = &NRec.KUval[3];
							m_Util.val_to_bin( (unsigned char*)pKkval->KKval2,	pDBSyzAcc->m_pKjsnk->ksval2 );	// 6.3%
							continue;
						}
						else if( ksline == 13 ){
							pKkval = &NRec.KUval[6];
						}
						else if( ksline == 14 ){
							pKkval = &NRec.KUval[7];
						}
						else if( ksline == 15 ){	// 集計等のからみがあるため、6.3％の金額を7行目に移す
							pKkval = &NRec.KUval[6];
							m_Util.val_to_bin( (unsigned char*)pKkval->KKval2,	pDBSyzAcc->m_pKjsnk->ksval2 );	// 6.3%
							continue;
						}
						else {
							continue;
						}
						m_Util.cstring_to_char( (unsigned char *)&pKkval->KKnam, pDBSyzAcc->m_pKjsnk->ksname, 40 );	// 名称
						m_Util.val_to_bin( (unsigned char*)pKkval->KKval1,	pDBSyzAcc->m_pKjsnk->ksval0 );			// 金額
						if( pDBSyzAcc->m_pKjsnk->kssgn3 & 0x01 ){
							pKkval->KKauto |= pDBSyzAcc->m_pKjsnk->kssgn3;
						}
						if( pDBSyzAcc->m_pKjsnk->kssgn3 & 0x04 ){	// 6.3％手入力サイン
							pKkval->KKauto |= 0x02;
						}

						// そのほかの金額に関しては集計や他表からの転記でまかなえるので省略
						break;

			case 5 :	if( ksline == 1 ){
							m_Util.cstring_to_char( (unsigned char *)&Head.KHksnm, pDBSyzAcc->m_pKjsnk->ksname, 10 );	// 名称
							continue;
						}
						else if( ksline == 10 ){
							pKkval = &NRec.KSval[3];
						}
						else if( ksline == 11 ){
							pKkval = &NRec.KSval[4];
						}
						else if( ksline == 12 ){	// 集計等のからみがあるため、6.3％の金額を4行目に移す
							pKkval = &NRec.KSval[3];
							m_Util.val_to_bin( (unsigned char*)pKkval->KKval2,	pDBSyzAcc->m_pKjsnk->ksval2 );	// 6.3%
							continue;
						}
						else if( ksline == 13 ){
							pKkval = &NRec.KSval[6];
						}
						else if( ksline == 14 ){
							pKkval = &NRec.KSval[7];
						}
						else if( ksline == 15 ){	// 集計等のからみがあるため、6.3％の金額を7行目に移す
							pKkval = &NRec.KSval[6];
							m_Util.val_to_bin( (unsigned char*)pKkval->KKval2,	pDBSyzAcc->m_pKjsnk->ksval2 );	// 6.3%
							continue;
						}
						else {
							continue;
						}
						m_Util.cstring_to_char( (unsigned char *)&pKkval->KKnam, pDBSyzAcc->m_pKjsnk->ksname, 40 );	// 名称
						m_Util.val_to_bin( (unsigned char*)pKkval->KKval1,	pDBSyzAcc->m_pKjsnk->ksval0 );			// 金額
						if( pDBSyzAcc->m_pKjsnk->kssgn3 & 0x01 ){
							pKkval->KKauto |= pDBSyzAcc->m_pKjsnk->kssgn3;
						}
						if( pDBSyzAcc->m_pKjsnk->kssgn3 & 0x04 ){	// 6.3％手入力サイン
							pKkval->KKauto |= 0x02;
						}

						// そのほかの金額に関しては集計や他表からの転記でまかなえるので省略
						break;

			case 6 :	if( pDBSyzAcc->m_pKjsnk->ksitem == 0 ){	// 決算額、課税取引外、課税取引金額、返還等課税取引
							m_Util.val_to_bin( (unsigned char*)pKkval->KKval1,	pDBSyzAcc->m_pKjsnk->ksval0 );
							m_Util.val_to_bin( (unsigned char*)pKkval->KKval2,	pDBSyzAcc->m_pKjsnk->ksval1 );
							m_Util.val_to_bin( (unsigned char*)pKkval->KKval3,	pDBSyzAcc->m_pKjsnk->ksval2 );
							m_Util.val_to_bin( (unsigned char*)pKkval->KKval4,	pDBSyzAcc->m_pKjsnk->ksval3 );
						}
						else if( pDBSyzAcc->m_pKjsnk->ksitem == 1 ){	// うち税率分の金額
							m_Util.val_to_bin( (unsigned char*)pKkval->KKval9,	pDBSyzAcc->m_pKjsnk->ksval0 );
							m_Util.val_to_bin( (unsigned char*)pKkval->KKval10,	pDBSyzAcc->m_pKjsnk->ksval1 );
						}
						m_Util.cstring_to_char( (unsigned char *)&pKkval->KKnam, pDBSyzAcc->m_pKjsnk->ksname, 40 );	// 名称
						pKkval->KKtype = pDBSyzAcc->m_pKjsnk->kssgn1;
						pKkval->KKsign = pDBSyzAcc->m_pKjsnk->kssgn2;
						if( pDBSyzAcc->m_pKjsnk->kssgn3 & 0x04 ){
							pKkval->KKauto |= 0x02;
						}

						break;
		}
	}

	// 後始末
	pDBSyzAcc->KjsnkClose();
	delete pDBSyzAcc;


	return 0;
}

//-----------------------------------------------------------------------------
//　課税取引金額計算表コンバージョン
//-----------------------------------------------------------------------------
// 返送値	0	：	正常終了
//			-1	：	エラー不正終了
//
//インボイス改正に伴い、データ保存場所を変更したため（230919）
//-----------------------------------------------------------------------------
int CH31TaxListData::GetPastData2(CDBNpSub *pDBNpSub, CSnHeadData *pSnHeadData, int sn_seq)
{
	// 連動のときは過去データを移さない
	if (!(pSnHeadData->Sn_Sign4 & 0x80)) {
		return 0;
	}

	CString			filter;
	CDBSyzAccess*	pDBSyzAcc;

	// 消費税拡張処理クラス
	pDBSyzAcc = NULL;
	pDBSyzAcc = new CDBSyzAccess;
	if (pDBSyzAcc == NULL) {
		return ERR;
	}

	// オープン
	filter.Format("kssgn5 = %d and kslist >= 101 and kslist <= 106", sn_seq);
	if (pDBSyzAcc->KjsnkOpen(pDBNpSub->m_database, filter) == ERR) {
		delete pDBSyzAcc;
		return ERR;
	}

	memset(&Head, '\0', sizeof(KJNHEAD));
	memset(&NRec, '\0', sizeof(KJNREC));
	char	yy = 0, mm = 0, dd = 0;
	pSnHeadData->GetYmdDataGen(ID_ICSSH_KESSAN_TO, &yy, &mm, &dd);
	YEAR = yy;


	// 業種区分
	if (!(GKBN = pDBNpSub->zvol->ind_type)) {
		GKBN = 0x07;
	}
	int kjct = pDBSyzAcc->m_pKjsnk->GetRecordCount();

	// 管理部
	if (m_Util.IsUnConnectMst(pDBNpSub->zvol)) {
		// 非連動時
		if (kjct == 0) {
			// 新規
			pSnHeadData->SVsign &= 0xdf;
			if (!(pDBNpSub->zvol->s_sgn1 & 0xc0)) {
				pSnHeadData->SVsign |= 0x20;	// D5を使用　ON:込み OFF=抜き
			}
			else {
				Head.KHstax = 0x01;				// 抜き
			}
			pSnHeadData->Sn_Sign2 &= 0xf7;
			if (Head.KHstax & 0x01) {
				pSnHeadData->Sn_Sign2 |= 0x08;
			}
		}
		else {
			// 令和5年1月1日以降の課税期間の場合は、会社登録の経理方式の設定に従う
			long symd = 0, eymd = 0;
			GetKazeiKikan(pSnHeadData, symd, eymd);
			if (symd >= R05_SDAY) {
				if (pDBNpSub->zvol->s_sgn1 & 0xc0) {
					// 税抜き
					pSnHeadData->SVsign &= 0xdf;
					Head.KHstax &= 0xfe;
					Head.KHstax |= 0x01;
				}
				else {
					// 税込み
					pSnHeadData->SVsign &= 0xdf;
					pSnHeadData->SVsign |= 0x20;
					Head.KHstax &= 0xfe;
				}
			}
			else {
				// メンテ
				if (pSnHeadData->Sn_Sign2 & 0x08) {
					Head.KHstax = 0x01;
				}
				else {
					Head.KHstax = 0x00;
				}
			}
		}
		Head.KHkoke = 0x00;
		pSnHeadData->Sn_Sign4 &= ~0x02;
	}
	else {
		if (kjct == 0) {
			// 新規
			pSnHeadData->SVsign &= 0xdf;
			Head.KHstax &= 0xfe;
			if (!(pDBNpSub->zvol->s_sgn1 & 0xc0)) {
				pSnHeadData->SVsign |= 0x20;	// D5を使用　ON:込み OFF=抜き
			}
			else {
				Head.KHstax = 0x01;				// 抜き
			}
		}
		else {
			int	Mssw = 0;
			// メンテ
			if (pDBNpSub->zvol->s_sgn1 & 0xc0) {
				// 税抜き
				pSnHeadData->SVsign &= 0xdf;
				Head.KHstax &= 0xfe;
				Head.KHstax |= 0x01;
			}
			else {
				// 税込み
				pSnHeadData->SVsign &= 0xdf;
				pSnHeadData->SVsign |= 0x20;
				Head.KHstax &= 0xfe;
			}
		}
		if (pSnHeadData->Sn_Sign4 & 0x02) {
			Head.KHkoke = 0x01;
		}
		else {
			Head.KHkoke = 0x00;
		}
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
	while (1) {
		if (Count >= KKVAL1MAX) {
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
	while (1) {
		if (Count >= KKVAL2MAX) {
			break;
		}
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
	while (1) {
		if (Count >= KKVAL3MAX) {
			break;
		}
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
	while (1) {
		if (Count >= KKVAL4MAX) {
			break;
		}
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
	while (1) {
		if (Count >= KKVAL5MAX) {
			break;
		}
		NRec.KSval[Rct].KKtype = KkVal5[Rct].KKtype;
		NRec.KSval[Rct].KKsign = KkVal5[Rct].KKsign;
		NRec.KSval[Rct].KKauto = KkVal5[Rct].KKauto;
		Rct++;
		Count++;
	}
	NRec.KSval[Rct].KKtype = 0xFF;


	// 過去データの読み込み
	int	rct = 0;
	while (1) {
		if (kjct == rct)	break;
		if (!rct) {
			if (pDBSyzAcc->m_pKjsnk->MoveFirst() == ERR)	break;
		}
		else {
			if (pDBSyzAcc->m_pKjsnk->MoveNext() == ERR)		break;
		}
		rct++;

		int flg = 0;
		switch (pDBSyzAcc->m_pKjsnk->kslist) {
		case 101:	pKkval = &NRec.KJval[pDBSyzAcc->m_pKjsnk->ksline - 1];	break;	// 事業所得	
		case 102:	pKkval = &NRec.KFval[pDBSyzAcc->m_pKjsnk->ksline - 1];	break;	// 不動産
		case 103:	pKkval = &NRec.KNval[pDBSyzAcc->m_pKjsnk->ksline - 1];	break;	// 農業
		case 104:	pKkval = &NRec.KUval[pDBSyzAcc->m_pKjsnk->ksline - 1];	break;	// 売上高
		case 105:	pKkval = &NRec.KSval[pDBSyzAcc->m_pKjsnk->ksline - 1];	break;	// 仕入高
		case 106:	pKkval = &NRec.KUval[pDBSyzAcc->m_pKjsnk->ksline - 1];	break;	// 事業別課税売上高
		default:	flg = 1;	break;
		}
		if (flg)	continue;


		int ksline = pDBSyzAcc->m_pKjsnk->ksline;

		switch (pDBSyzAcc->m_pKjsnk->kslist) {

		case 101:	
			//if (ksline == 34) {	// 返還等対価
			//pKkval = &NRec.KJval[34];
			//}
			m_Util.cstring_to_char((unsigned char *)&pKkval->KKnam, pDBSyzAcc->m_pKjsnk->ksname, 40);	// 名称
			m_Util.val_to_bin((unsigned char*)pKkval->KKval1, pDBSyzAcc->m_pKjsnk->ksval0);			// 決算額
			m_Util.val_to_bin((unsigned char*)pKkval->KKval2, pDBSyzAcc->m_pKjsnk->ksval1);			// 課税取引にならないもの
			m_Util.val_to_bin((unsigned char*)pKkval->KKval5, pDBSyzAcc->m_pKjsnk->ksval4);			// 6.24％
			m_Util.val_to_bin((unsigned char*)pKkval->KKval6, pDBSyzAcc->m_pKjsnk->ksval5);			// 7.8％
		//	if (pDBSyzAcc->m_pKjsnk->kssgn3 & 0x01) {
				pKkval->KKauto |= pDBSyzAcc->m_pKjsnk->kssgn3;
		//	}
			break;

		case 102:    
			m_Util.cstring_to_char((unsigned char *)&pKkval->KKnam, pDBSyzAcc->m_pKjsnk->ksname, 40);	// 名称
			m_Util.val_to_bin((unsigned char*)pKkval->KKval1, pDBSyzAcc->m_pKjsnk->ksval0);			// 決算額
			m_Util.val_to_bin((unsigned char*)pKkval->KKval2, pDBSyzAcc->m_pKjsnk->ksval1);			// 課税取引にならないもの
			m_Util.val_to_bin((unsigned char*)pKkval->KKval5, pDBSyzAcc->m_pKjsnk->ksval4);			// 6.24％
			m_Util.val_to_bin((unsigned char*)pKkval->KKval6, pDBSyzAcc->m_pKjsnk->ksval5);			// 7.8％
		//	if (pDBSyzAcc->m_pKjsnk->kssgn3 & 0x01) {
				pKkval->KKauto |= pDBSyzAcc->m_pKjsnk->kssgn3;
		//	}
			break;

		case 103:	
			m_Util.cstring_to_char((unsigned char *)&pKkval->KKnam, pDBSyzAcc->m_pKjsnk->ksname, 40);	// 名称
			m_Util.val_to_bin((unsigned char*)pKkval->KKval1, pDBSyzAcc->m_pKjsnk->ksval0);			// 決算額
			m_Util.val_to_bin((unsigned char*)pKkval->KKval2, pDBSyzAcc->m_pKjsnk->ksval1);			// 課税取引にならないもの
			m_Util.val_to_bin((unsigned char*)pKkval->KKval3, pDBSyzAcc->m_pKjsnk->ksval2);			// 課税取引金額
			m_Util.val_to_bin((unsigned char*)pKkval->KKval5, pDBSyzAcc->m_pKjsnk->ksval4);			// 6.24％
			m_Util.val_to_bin((unsigned char*)pKkval->KKval6, pDBSyzAcc->m_pKjsnk->ksval5);			// 7.8％
		//	if (pDBSyzAcc->m_pKjsnk->kssgn3 & 0x01) {
				pKkval->KKauto |= pDBSyzAcc->m_pKjsnk->kssgn3;
		//	}
			break;

		case 104:	
			//（３）（４）以外の金額に関しては集計や他表からの転記でまかなえるので省略
			if (ksline == 1) {
				m_Util.cstring_to_char((unsigned char *)&Head.KHkunm, pDBSyzAcc->m_pKjsnk->ksname, 10);	// 名称
				continue;
			}
			else if (ksline == 4) {
				pKkval = &NRec.KUval[3];
				m_Util.val_to_bin((unsigned char*)pKkval->KKval1, pDBSyzAcc->m_pKjsnk->ksval0);	// 金額
				m_Util.val_to_bin((unsigned char*)pKkval->KKval3, pDBSyzAcc->m_pKjsnk->ksval2);	// 6.24%
				m_Util.val_to_bin((unsigned char*)pKkval->KKval4, pDBSyzAcc->m_pKjsnk->ksval3);	// 7.8%
			}
			else if (ksline == 5) {
				pKkval = &NRec.KUval[4];
				m_Util.val_to_bin((unsigned char*)pKkval->KKval1, pDBSyzAcc->m_pKjsnk->ksval0);	// 金額
				m_Util.val_to_bin((unsigned char*)pKkval->KKval3, pDBSyzAcc->m_pKjsnk->ksval2);	// 6.24%
				m_Util.val_to_bin((unsigned char*)pKkval->KKval4, pDBSyzAcc->m_pKjsnk->ksval3);	// 7.8%
			}
			else if (ksline == 7) {
				pKkval = &NRec.KUval[6];
				m_Util.val_to_bin((unsigned char*)pKkval->KKval1, pDBSyzAcc->m_pKjsnk->ksval0);	// 金額
				m_Util.val_to_bin((unsigned char*)pKkval->KKval3, pDBSyzAcc->m_pKjsnk->ksval2);	// 6.24%
				m_Util.val_to_bin((unsigned char*)pKkval->KKval4, pDBSyzAcc->m_pKjsnk->ksval3);	// 7.8%
			}
			else if (ksline == 8) {
				pKkval = &NRec.KUval[7];
				m_Util.val_to_bin((unsigned char*)pKkval->KKval1, pDBSyzAcc->m_pKjsnk->ksval0);	// 金額
				m_Util.val_to_bin((unsigned char*)pKkval->KKval3, pDBSyzAcc->m_pKjsnk->ksval2);	// 6.24%
				m_Util.val_to_bin((unsigned char*)pKkval->KKval4, pDBSyzAcc->m_pKjsnk->ksval3);	// 7.8%
			}

		//	if (pDBSyzAcc->m_pKjsnk->kssgn3 & 0x01) {
				pKkval->KKauto |= pDBSyzAcc->m_pKjsnk->kssgn3;
		//	}
			break;

		case 105:	
			//（３）（４）以外の金額に関しては集計や他表からの転記でまかなえるので省略
			if (ksline == 1) {
				m_Util.cstring_to_char((unsigned char *)&Head.KHksnm, pDBSyzAcc->m_pKjsnk->ksname, 10);	// 名称
				continue;
			}
			else if (ksline == 4) {
				pKkval = &NRec.KSval[3];
				m_Util.val_to_bin((unsigned char*)pKkval->KKval1, pDBSyzAcc->m_pKjsnk->ksval0);	// 金額
				m_Util.val_to_bin((unsigned char*)pKkval->KKval3, pDBSyzAcc->m_pKjsnk->ksval2);	// 6.24%
				m_Util.val_to_bin((unsigned char*)pKkval->KKval4, pDBSyzAcc->m_pKjsnk->ksval3);	// 7.8%
			}
			else if (ksline == 5) {
				pKkval = &NRec.KSval[4];
				m_Util.val_to_bin((unsigned char*)pKkval->KKval1, pDBSyzAcc->m_pKjsnk->ksval0);	// 金額
				m_Util.val_to_bin((unsigned char*)pKkval->KKval3, pDBSyzAcc->m_pKjsnk->ksval2);	// 6.24%
				m_Util.val_to_bin((unsigned char*)pKkval->KKval4, pDBSyzAcc->m_pKjsnk->ksval3);	// 7.8%
			}
			else if (ksline == 7) {
				pKkval = &NRec.KSval[6];
				m_Util.val_to_bin((unsigned char*)pKkval->KKval1, pDBSyzAcc->m_pKjsnk->ksval0);	// 金額
				m_Util.val_to_bin((unsigned char*)pKkval->KKval3, pDBSyzAcc->m_pKjsnk->ksval2);	// 6.24%
				m_Util.val_to_bin((unsigned char*)pKkval->KKval4, pDBSyzAcc->m_pKjsnk->ksval3);	// 7.8%
			}
			else if (ksline == 8) {
				pKkval = &NRec.KSval[7];
				m_Util.val_to_bin((unsigned char*)pKkval->KKval1, pDBSyzAcc->m_pKjsnk->ksval0);	// 金額
				m_Util.val_to_bin((unsigned char*)pKkval->KKval3, pDBSyzAcc->m_pKjsnk->ksval2);	// 6.24%
				m_Util.val_to_bin((unsigned char*)pKkval->KKval4, pDBSyzAcc->m_pKjsnk->ksval3);	// 7.8%
			}
			else {
				continue;
			}

		//	if (pDBSyzAcc->m_pKjsnk->kssgn3 & 0x01) {
				pKkval->KKauto |= pDBSyzAcc->m_pKjsnk->kssgn3;
		//	}
			break;

		case 106:
			if (pDBSyzAcc->m_pKjsnk->ksitem == 0) {	// 決算額、課税取引外、課税取引金額、返還等課税取引
				m_Util.val_to_bin((unsigned char*)pKkval->KKval1, pDBSyzAcc->m_pKjsnk->ksval0);
				m_Util.val_to_bin((unsigned char*)pKkval->KKval2, pDBSyzAcc->m_pKjsnk->ksval1);
				m_Util.val_to_bin((unsigned char*)pKkval->KKval4, pDBSyzAcc->m_pKjsnk->ksval3);
			}
			else if (pDBSyzAcc->m_pKjsnk->ksitem == 1) {	// うち税率分の金額
				m_Util.val_to_bin((unsigned char*)pKkval->KKval5, pDBSyzAcc->m_pKjsnk->ksval0);
				m_Util.val_to_bin((unsigned char*)pKkval->KKval6, pDBSyzAcc->m_pKjsnk->ksval1);
				m_Util.val_to_bin((unsigned char*)pKkval->KKval7, pDBSyzAcc->m_pKjsnk->ksval2);
				m_Util.val_to_bin((unsigned char*)pKkval->KKval8, pDBSyzAcc->m_pKjsnk->ksval3);
			}

			//m_Util.cstring_to_char((unsigned char *)&pKkval->KKnam, pDBSyzAcc->m_pKjsnk->ksname, 40);	// 名称
			pKkval->KKtype = pDBSyzAcc->m_pKjsnk->kssgn1;
			pKkval->KKsign = pDBSyzAcc->m_pKjsnk->kssgn2;
			//if (pDBSyzAcc->m_pKjsnk->kssgn3 & 0x04) {
			//	pKkval->KKauto |= 0x02;
			//}
			pKkval->KKauto |= pDBSyzAcc->m_pKjsnk->kssgn3;

			break;
		}
	}

	// 後始末
	pDBSyzAcc->KjsnkClose();
	delete pDBSyzAcc;

	return 0;
}

int CH31TaxListData::IsNeedOldDataCnv( CDBNpSub *pZmsub, int sn_seq )
{
	CDBSyzAccess	DBSyzAccess;
	CString filter;
	filter.Format( "kssgn5 = %d and kslist >= 101 and kslist <= 106", sn_seq );
	if ( DBSyzAccess.KjsnkOpen ( pZmsub->m_database, filter ) ) {
		return FALSE;
	}
	int st = DBSyzAccess.m_pKjsnk->st;

	DBSyzAccess.KjsnkClose();

	if ( st == -1 ) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

//======================================================
// 課税期間の取得
//------------------------------------------------------
// 引数
//		pSnHeadData	:		消費税ヘッダ情報
//		symd		:		課税期間（自）
//		eymd		:		課税期間（至）
//======================================================
void CH31TaxListData::GetKazeiKikan( CSnHeadData *pSnHeadData, long &symd, long &eymd )
{
	long tmpSymd = 0, tmpEymd = 0;

	// 初期化
	symd = 0;
	eymd = 0;

	ASSERT( pSnHeadData );
	if( pSnHeadData == nullptr ) {
		return;
	}

	if( !( pSnHeadData->Sn_SKKBN % 2 )) {
		// 中間申告
		tmpSymd = pSnHeadData->Sn_MDAYS;
		tmpEymd = pSnHeadData->Sn_MDAYE;
	}
	else {
		// 確定申告
		tmpSymd = pSnHeadData->Sn_KDAYS;
		tmpEymd = pSnHeadData->Sn_KDAYE;
	}

	symd = tmpSymd;
	eymd = tmpEymd;
}
