// shzcalc.cpp
// 計算表集計クラス
#include "stdafx.h"
//#include "DBSyzShin.h"

//#include "MainFrm.h"
//#include "DBSyzShinDoc.h"
//#include "DBSyzShinView.h"
#include "ShinCalc.h"


//---------->'14.08.20
#include "ShinBuyKeikaIdx.h"
#include "ShinSellKeikaIdx.h"
#include "ShinJigyoKeikaIdx.h"
#include "ShinFudousanKeikaIdx.h"
#include "ShinNougyouKeikaIdx.h"

#include "ShinKniinpKeikaIdx.h"


//<------------------


// メインビューの財務クラスを参照
//CDBSyzShinView *CShzCalc::pSyzShin;

CShzCalc::CShzCalc()
	: m_pTaxListData(NULL)
	, m_pSnHeadData(NULL)
	, m_pZmSub(NULL)
	, m_Arith(0x16)
//2016.10.20 INSERT START
	, m_SixKindFlg(FALSE)
//2016.10.20 INSERT END
{
}

CShzCalc::~CShzCalc()
{
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 引数	pTaxListData	：	課税取引金額計算表データ
//		pSnHeadData		：	消費税ヘッダ情報
//		pZmSub			：	公益クラス
//-----------------------------------------------------------------------------
// 返送値	0			：	正常終了
//			-1			：	エラー
//-----------------------------------------------------------------------------
int CShzCalc::InitShzCalc( CH26TaxListData **pTaxListData, CSnHeadData **pSnHeadData, CDBNpSub *pZmSub )
{
	ASSERT( pTaxListData );
	if( pTaxListData == NULL ){
		return -1;
	}
	m_pTaxListData = pTaxListData;

	ASSERT( pSnHeadData );
	if( pSnHeadData == NULL ){
		return -1;
	}
	m_pSnHeadData = pSnHeadData;

	ASSERT( pZmSub );
	if( pZmSub == NULL ){
		return -1;
	}
	m_pZmSub = pZmSub;

	return 0;
}

//-----------------------------------------------------------------------------
// 計算表全集計
//-----------------------------------------------------------------------------
// 引数	mode	：
//-----------------------------------------------------------------------------
// 返送値		：
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_AllCalc( int mode )
{
//	if( pSyzShin->KniFlg&0x01 ){	// 非連動ｄｂで簡易課税　D0:ON [13'09.18]

	if( Util.IsUnConnectMst(m_pZmSub->zvol) && (Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_KANNI) ){// 非連動ｄｂで簡易課税
		Shz_KniInput();
		Shz_KniPost();
	}
	else{
		switch( mode ){
			case 1:
				Shz_JigyoCalc();
				break;
			case 2:	
				Shz_FudohCalc();
				break;
			case 3:	
				Shz_NohgyoCalc();	
				break;
			default:
				Shz_JigyoCalc();
				Shz_FudohCalc();
				Shz_NohgyoCalc();
				break;
		}
		Shz_SalePost();
		Shz_BuyPost();
	}
	return 0;
}

//-----------------------------------------------------------------------------
// 横の計算
//-----------------------------------------------------------------------------
// 引数	pK		：
//		mode	：
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_LinCalc( KKVAL* pK, int mode )
{
	switch( mode ){
		case 0:		
			m_Arith.l_sub( pK->KKval3, pK->KKval1, pK->KKval2 );	
			break;
		case 1:		
			memcpy( pK->KKval3, pK->KKval1, 6 );	
			break;
		case 2:		
			memcpy( pK->KKval2, pK->KKval1, 6 );	
			break;
	}
	return 0;
}


#define	JVAL	34

//-----------------------------------------------------------------------------
// 事業所得
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_JigyoCalc()
{
	if( ((*m_pTaxListData)->GKBN==0x02) || ((*m_pTaxListData)->GKBN==0x04) ){
		return 0;
	}
	int	mode;
	for( int i=0; i<JVAL; i++ ){
		mode = 0;
		switch( i ){
			case 1:
			case 4:		// なし
			case 3:
			case 5:
			case 6:
			case 31: 
			case 32:	// 計欄
				continue;
			case 9:
			case 15:
			case 16:	// A欄をC欄へ転記
				mode = 1;
				break;
			case 14:
			case 17:
			case 21:
			case 23:	// A欄をB欄へ転記
				mode = 2;	
				break;
		}
		Shz_LinCalc( &(*m_pTaxListData)->NRec.KJval[i], mode );
	}

	KKVAL	*pK;
	// 売上原価　小計(4)A
	pK = &(*m_pTaxListData)->NRec.KJval[3];
	m_Arith.l_add( pK->KKval1, (*m_pTaxListData)->NRec.KJval[1].KKval1, (*m_pTaxListData)->NRec.KJval[2].KKval1 );
	// 売上原価　差引原価(6)A
	pK = &(*m_pTaxListData)->NRec.KJval[5];
	m_Arith.l_sub( pK->KKval1, (*m_pTaxListData)->NRec.KJval[3].KKval1, (*m_pTaxListData)->NRec.KJval[4].KKval1 );
	// 差引金額(6)A
	pK = &(*m_pTaxListData)->NRec.KJval[6];
	m_Arith.l_sub( pK->KKval1, (*m_pTaxListData)->NRec.KJval[0].KKval1, (*m_pTaxListData)->NRec.KJval[5].KKval1 );

	// 経費　計(32)
	pK = &(*m_pTaxListData)->NRec.KJval[31];
	::ZeroMemory( pK->KKval1, 6 );
	::ZeroMemory( pK->KKval2, 6 );
	for( int i=7; i<31; i++ ){
		m_Arith.l_add( pK->KKval1, pK->KKval1, (*m_pTaxListData)->NRec.KJval[i].KKval1 );
		if( (i==9) || (i==15) || (i==16) ){
			;
		}
		else{
			m_Arith.l_add( pK->KKval2, pK->KKval2, (*m_pTaxListData)->NRec.KJval[i].KKval2 );
		}
	}
	Shz_LinCalc( pK, 0 );

	// 経費　計(33)A
	pK = &(*m_pTaxListData)->NRec.KJval[32];
	m_Arith.l_sub( pK->KKval1, (*m_pTaxListData)->NRec.KJval[6].KKval1, (*m_pTaxListData)->NRec.KJval[31].KKval1 );

	return 0;
}

#define	FVAL	16

//-----------------------------------------------------------------------------
// 不動産所得
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_FudohCalc()
{
	if( ((*m_pTaxListData)->GKBN==0x01) || ((*m_pTaxListData)->GKBN==0x04) ){
		return 0;
	}
	int	mode;
	for( int i=0; i<FVAL; i++ ){
		mode = 0;
		switch( i ){
			case 3: 
			case 13: 
			case 14:	// 計欄
				continue;
			case 5: 
			case 7: 
			case 8:		// A欄をB欄へ転記
				mode = 2;
				break;
			case 6:		// A欄をC欄へ転記
				mode = 1;	
				break;
		}
		Shz_LinCalc( &(*m_pTaxListData)->NRec.KFval[i], mode );
	}

	KKVAL	*pK;
	// 収入金額　計(4)
	pK = &(*m_pTaxListData)->NRec.KFval[3];
	::ZeroMemory( pK->KKval1, 6 );
	::ZeroMemory( pK->KKval2, 6 );
	for( int i=0; i<3; i++ ){
		m_Arith.l_add( pK->KKval1, pK->KKval1, (*m_pTaxListData)->NRec.KFval[i].KKval1 );
		m_Arith.l_add( pK->KKval2, pK->KKval2, (*m_pTaxListData)->NRec.KFval[i].KKval2 );
	}
	Shz_LinCalc( pK, 0 );

	// 必要経費　計(14)
	pK = &(*m_pTaxListData)->NRec.KFval[13];
	::ZeroMemory( pK->KKval1, 6 );
	::ZeroMemory( pK->KKval2, 6 );
	for( int i=4; i<13; i++ ){
		m_Arith.l_add( pK->KKval1, pK->KKval1, (*m_pTaxListData)->NRec.KFval[i].KKval1 );
		if( i == 6 ){
			;
		}
		else{
			m_Arith.l_add( pK->KKval2, pK->KKval2, (*m_pTaxListData)->NRec.KFval[i].KKval2 );
		}
	}
	Shz_LinCalc( pK, 0 );

	// 差引金額(15)A
	pK = &(*m_pTaxListData)->NRec.KFval[14];
	m_Arith.l_sub( pK->KKval1, (*m_pTaxListData)->NRec.KFval[3].KKval1, (*m_pTaxListData)->NRec.KFval[13].KKval1 );

	return 0;
}

#define	NVAL	39

//-----------------------------------------------------------------------------
// 農業所得
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_NohgyoCalc()
{
	if( ((*m_pTaxListData)->GKBN==0x01) || ((*m_pTaxListData)->GKBN==0x02) ){
		return 0;
	}

	int		mode;
	char	val[6] = {0};
	for( int i=0; i<NVAL; i++ ){
		mode = 0;
		switch( i ){
			case 4:		// 未成熟果樹収入
			case 5: 
			case 8: 
			case 32: 
			case 36: 
			case 37:	// 計欄
			case 6:
			case 7:
			case 33: 
			case 34:
			case 35:	// 金額なし
				continue;
			case 1: 
			case 14:
			case 15: 
			case 16: 
			case 17: 
			case 19:	// A欄をC欄へ転記
				mode = 1;
				break;
			case 20: 
			case 21: 
			case 24:
			case 27:	// A欄をB欄へ転記
				mode = 2;
				break;
		}
		Shz_LinCalc( &(*m_pTaxListData)->NRec.KNval[i], mode );
	}

	KKVAL	*pK;
	// 収入金額　小計(4)
	pK = &(*m_pTaxListData)->NRec.KNval[5];
	::ZeroMemory( pK->KKval1, 6 );
	::ZeroMemory( pK->KKval2, 6 );
	::ZeroMemory( pK->KKval3, 6 );
	for( int i=0; i<5; i++ ){
		if( i == 4 ){
			;
		}
		else{
			m_Arith.l_add( pK->KKval1, pK->KKval1, (*m_pTaxListData)->NRec.KNval[i].KKval1 );
		}
		if( (i==1) || (i==4) ){
			;
		}
		else{
			m_Arith.l_add( pK->KKval2, pK->KKval2, (*m_pTaxListData)->NRec.KNval[i].KKval2 );
		}
		m_Arith.l_add( pK->KKval3, pK->KKval3, (*m_pTaxListData)->NRec.KNval[i].KKval3 );
	}
	// 収入金額　計(7)A
	pK = &(*m_pTaxListData)->NRec.KNval[8];
	m_Arith.l_sub( val, (*m_pTaxListData)->NRec.KNval[6].KKval1, (*m_pTaxListData)->NRec.KNval[7].KKval1 );
	m_Arith.l_sub( pK->KKval1, (*m_pTaxListData)->NRec.KNval[5].KKval1, val );

	// 経費　小計(31)
	pK = &(*m_pTaxListData)->NRec.KNval[32];
	::ZeroMemory( pK->KKval1, 6 );
	::ZeroMemory( pK->KKval2, 6 );
	for( int i=9; i<32; i++ ){
		m_Arith.l_add( pK->KKval1, pK->KKval1, (*m_pTaxListData)->NRec.KNval[i].KKval1 );
		if( (i==14) || (i==15) || (i==16) || (i==17) || (i==19) ){
			;
		}
		else{
			m_Arith.l_add( pK->KKval2, pK->KKval2, (*m_pTaxListData)->NRec.KNval[i].KKval2 );
		}
	}
	Shz_LinCalc( pK, 0 );
	// 経費　計(35)
	pK = &(*m_pTaxListData)->NRec.KNval[36];
	m_Arith.l_sub( val, (*m_pTaxListData)->NRec.KNval[33].KKval1, (*m_pTaxListData)->NRec.KNval[34].KKval1 );
	m_Arith.l_sub( val, val, (*m_pTaxListData)->NRec.KNval[35].KKval1 );
	m_Arith.l_add( pK->KKval1, (*m_pTaxListData)->NRec.KNval[32].KKval1, val );
	// 差引金額(36)
	pK = &(*m_pTaxListData)->NRec.KNval[37];
	m_Arith.l_sub( pK->KKval1, (*m_pTaxListData)->NRec.KNval[8].KKval1, (*m_pTaxListData)->NRec.KNval[36].KKval1 );

	return 0;
}

//-----------------------------------------------------------------------------
// 課税売上高への転記
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_SalePost()
{
//	if( !(pSyzShin->KniFlg&0x01) ){
	if( !( Util.IsUnConnectMst(m_pZmSub->zvol) && (Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_KANNI) ) ){// 非連動ｄｂで簡易課税じゃ無い場合//'14.10.08

		KKVAL*	pK = (*m_pTaxListData)->NRec.KUval;

		if( (*m_pTaxListData)->GKBN & 0x01 ){	// 事業所
			if( !((pK+0)->KKauto&0x01) ){
				memcpy( (pK+0)->KKval1, (*m_pTaxListData)->NRec.KJval[0].KKval1, 6 );
			}
			if( !((pK+1)->KKauto&0x01) ){
				memcpy( (pK+1)->KKval1, (*m_pTaxListData)->NRec.KJval[0].KKval2, 6 );
			}
		}
		if( (*m_pTaxListData)->GKBN & 0x02 ){	// 不動産
			if( !((pK+6)->KKauto&0x01) ){
				memcpy( (pK+6)->KKval1, (*m_pTaxListData)->NRec.KFval[3].KKval1, 6 );
			}
			if( !((pK+7)->KKauto&0x01) ){
				memcpy( (pK+7)->KKval1, (*m_pTaxListData)->NRec.KFval[3].KKval2, 6 );
			}
		}
		if( (*m_pTaxListData)->GKBN & 0x04 ){	// 農業
			if( !((pK+3)->KKauto&0x01) ){
				memcpy( (pK+3)->KKval1, (*m_pTaxListData)->NRec.KNval[5].KKval1, 6 );
			}
			if( !((pK+4)->KKauto&0x01) ){
				memcpy( (pK+4)->KKval1, (*m_pTaxListData)->NRec.KNval[5].KKval2, 6 );
			}
		}
	}
	else{
		Shz_KniPost();
	}

	Shz_SaleCalc();
	return 0;
}

//-----------------------------------------------------------------------------
// 課税仕入高への転記
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_BuyPost()
{

//////
//	if( pSyzShin->KniFlg&0x10 ) return 0;	//★//[13'09.18];
//////

	KKVAL*	pK = (*m_pTaxListData)->NRec.KSval;
	if( (*m_pTaxListData)->GKBN & 0x01 ){	// 事業所
		if( !((pK+0)->KKauto&0x01) ){
			m_Arith.l_add( (pK+0)->KKval1, (*m_pTaxListData)->NRec.KJval[2].KKval1, (*m_pTaxListData)->NRec.KJval[31].KKval1 );
		}
		if( !((pK+1)->KKauto&0x01) ){
			m_Arith.l_add( (pK+1)->KKval1, (*m_pTaxListData)->NRec.KJval[2].KKval2, (*m_pTaxListData)->NRec.KJval[31].KKval2 );
		}
	}
	if( (*m_pTaxListData)->GKBN & 0x02 ){	// 不動産
		if( !((pK+6)->KKauto&0x01) ){
			memcpy( (pK+6)->KKval1, (*m_pTaxListData)->NRec.KFval[13].KKval1, 6 );
		}
		if( !((pK+7)->KKauto&0x01) ){
			memcpy( (pK+7)->KKval1, (*m_pTaxListData)->NRec.KFval[13].KKval2, 6 );
		}
	}
	if( (*m_pTaxListData)->GKBN & 0x04 ){	// 農業
		if( !((pK+3)->KKauto&0x01) ){
			memcpy( (pK+3)->KKval1, (*m_pTaxListData)->NRec.KNval[32].KKval1, 6 );
		}
		if( !((pK+4)->KKauto&0x01) ){
			memcpy( (pK+4)->KKval1, (*m_pTaxListData)->NRec.KNval[32].KKval2, 6 );
		}
	}

	Shz_BuyCalc();
	return 0;
}

//-----------------------------------------------------------------------------
// 課税売上高
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_SaleCalc()
{
	KKVAL*	pK = (*m_pTaxListData)->NRec.KUval;

	m_Arith.l_sub( (pK+2)->KKval1, (pK+0)->KKval1, (pK+1)->KKval1 );
	m_Arith.l_sub( (pK+5)->KKval1, (pK+3)->KKval1, (pK+4)->KKval1 );
	m_Arith.l_add( (pK+5)->KKval1, (pK+5)->KKval1, (*m_pTaxListData)->NRec.KNval[4].KKval3 );
	m_Arith.l_sub( (pK+8)->KKval1, (pK+6)->KKval1, (pK+7)->KKval1 );
	m_Arith.l_sub( (pK+11)->KKval1, (pK+9)->KKval1, (pK+10)->KKval1 );
	m_Arith.l_sub( (pK+14)->KKval1, (pK+12)->KKval1, (pK+13)->KKval1 );

	::ZeroMemory( (pK+15)->KKval1, 6 );
	m_Arith.l_add( (pK+15)->KKval1, (pK+2)->KKval1, (pK+5)->KKval1 );
	m_Arith.l_add( (pK+15)->KKval1, (pK+15)->KKval1, (pK+8)->KKval1 );
	m_Arith.l_add( (pK+15)->KKval1, (pK+15)->KKval1, (pK+11)->KKval1 );
	m_Arith.l_add( (pK+15)->KKval1, (pK+15)->KKval1, (pK+14)->KKval1 );

	char	val[6]={0};

	// 仮受消費税
	if( (*m_pTaxListData)->Head.KHstax ){
		if( !((pK+16)->KKauto&0x04) ){
			::ZeroMemory( val, 6 );
			Shz_shohizei8_100( val, (pK+15)->KKval1 );
			memcpy( (pK+16)->KKval3, val, 6 );
		}
	}

	if( !((pK+16)->KKauto&0x02) ){
		if( (*m_pTaxListData)->Head.KHstax ){
			m_Arith.l_add( (pK+16)->KKval2, (pK+15)->KKval1, (pK+16)->KKval3 );
		}
		else{
			memcpy( (pK+16)->KKval2, (pK+15)->KKval1, 6 );
		}
	}
	if( !((pK+16)->KKauto&0x01) ){
		::ZeroMemory( val, 6 );
		//Shz_urcalc( val, (pK+16)->KKval2 );
		Shz_shohizei100_108( val, (pK+16)->KKval2 );
		memcpy( (pK+16)->KKval1, val, 6 );
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 課税仕入高
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_BuyCalc()
{
	KKVAL*	pK = (*m_pTaxListData)->NRec.KSval;

	m_Arith.l_sub( (pK+2)->KKval1, (pK+0)->KKval1, (pK+1)->KKval1 );
	m_Arith.l_sub( (pK+5)->KKval1, (pK+3)->KKval1, (pK+4)->KKval1 );

	m_Arith.l_sub( (pK+8)->KKval1, (pK+6)->KKval1, (pK+7)->KKval1 );
	m_Arith.l_sub( (pK+11)->KKval1, (pK+9)->KKval1, (pK+10)->KKval1 );
	m_Arith.l_sub( (pK+14)->KKval1, (pK+12)->KKval1, (pK+13)->KKval1 );

	::ZeroMemory( (pK+15)->KKval1, 6 );
	m_Arith.l_add( (pK+15)->KKval1, (pK+2)->KKval1, (pK+5)->KKval1 );
	m_Arith.l_add( (pK+15)->KKval1, (pK+15)->KKval1, (pK+8)->KKval1 );
	m_Arith.l_add( (pK+15)->KKval1, (pK+15)->KKval1, (pK+11)->KKval1 );
	m_Arith.l_add( (pK+15)->KKval1, (pK+15)->KKval1, (pK+14)->KKval1 );

	char	val[6];

	// 仮払消費税
	if( (*m_pTaxListData)->Head.KHstax ){
		if( !((pK+16)->KKauto&0x04) ){
			::ZeroMemory( val, 6 );
			Shz_shohizei8_100( val, (pK+15)->KKval1 );
			memcpy( (pK+16)->KKval3, val, 6 );
		}
	}

	if( !((pK+16)->KKauto&0x02) ){
		if( (*m_pTaxListData)->Head.KHstax ){
			m_Arith.l_add( (pK+16)->KKval2, (pK+15)->KKval1, (pK+16)->KKval3 );
		}
		else{
			memcpy( (pK+16)->KKval2, (pK+15)->KKval1, 6 );
		}
	}
	if( !((pK+16)->KKauto&0x01) ){
		::ZeroMemory( val, 6 );
		Shz_shohizei63( val, (pK+16)->KKval2 );
		memcpy( (pK+16)->KKval1, val, 6 );
	}

	return 0;
}

#define	KINP	40
#define	KLIN	7

//-----------------------------------------------------------------------------
// 簡易課税
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_KniInput()
{
	KKVAL*	pK = (*m_pTaxListData)->NRec.KUval;
	int		id;
	char	val[6][6];

	::ZeroMemory( val, sizeof(val));
	for( int i=0; i<KLIN; i++ ){
		id = KINP+i;
		Shz_LinCalc( (pK+id), 0 );
		m_Arith.l_add( val[0], val[0], (pK+id)->KKval1 );
		m_Arith.l_add( val[1], val[1], (pK+id)->KKval2 );
		m_Arith.l_add( val[2], val[2], (pK+id)->KKval3 );
		m_Arith.l_add( val[3], val[3], (pK+id)->KKval4 );
	}

	// 固定資産等の譲渡 [13'09.18]
	id = KINP+8;
	Shz_LinCalc( (pK+id), 0 );
	m_Arith.l_add( val[0], val[0], (pK+id)->KKval1 );
	m_Arith.l_add( val[1], val[1], (pK+id)->KKval2 );
	m_Arith.l_add( val[2], val[2], (pK+id)->KKval3 );
	m_Arith.l_add( val[3], val[3], (pK+id)->KKval4 );

	id = KINP+KLIN;
	memcpy( (pK+id)->KKval1, val[0], 6 );
	memcpy( (pK+id)->KKval2, val[1], 6 );
	memcpy( (pK+id)->KKval3, val[2], 6 );
	memcpy( (pK+id)->KKval4, val[3], 6 );


	return 0;
}

//-----------------------------------------------------------------------------
// 課税売上高への転記（簡易課税）
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_KniPost()
{
	KKVAL*	pK = (*m_pTaxListData)->NRec.KUval;
	char	val[6];

	if( (*m_pTaxListData)->GKBN & 0x01 ){	// 事業所
		if( !((pK+0)->KKauto&0x01) ){
			::ZeroMemory( val, 6 );
			m_Arith.l_add( val, (pK+40)->KKval1, (pK+41)->KKval1 );
			m_Arith.l_add( val, val, (pK+44)->KKval1 );
			//★//[13'09.18]
			m_Arith.l_add( val, val, (pK+42)->KKval1 );
			m_Arith.l_add( val, val, (pK+45)->KKval1 );
			memcpy( (pK+0)->KKval1, val, 6 );
		}
		if( !((pK+1)->KKauto&0x01) ){
			::ZeroMemory( val, 6 );
			m_Arith.l_add( val, (pK+40)->KKval2, (pK+41)->KKval2 );
			m_Arith.l_add( val, val, (pK+44)->KKval2 );
			//★//[13'09.18]
			m_Arith.l_add( val, val, (pK+42)->KKval2 );
			m_Arith.l_add( val, val, (pK+45)->KKval2 );
			memcpy( (pK+1)->KKval1, val, 6 );
		}
	}
//2016.10.20 DELETE START
//	if( (*m_pTaxListData)->GKBN & 0x02 )	{	// 不動産
//2016.10.20 DELETE END
		if( !((pK+6)->KKauto&0x01) ){
			::ZeroMemory( val, 6 );
			memcpy( (pK+6)->KKval1, (pK+46)->KKval1, 6 );
		}
		if( !((pK+7)->KKauto&0x01) ){
			::ZeroMemory( val, 6 );
			memcpy( (pK+7)->KKval1, (pK+46)->KKval2, 6 );
		}
		//★//[13'09.18] 固定資産等の譲渡 追加
		//if( !((pK+12)->KKauto&0x01) ){
		//	::ZeroMemory( val, 6 );
		//	memcpy( (pK+12)->KKval1, (pK+48)->KKval1, 6 );
		//}
		//if( !((pK+13)->KKauto&0x01) ){
		//	::ZeroMemory( val, 6 );
		//	memcpy( (pK+13)->KKval1, (pK+48)->KKval2, 6 );
		//}
//2016.10.20 DELETE START
//	}
//2016.10.20 DELETE END
//2016.10.20 DELETE START
//	if( (*m_pTaxListData)->GKBN & 0x04 )	{	// 農業
//2016.10.20 DELETE END
		if( !((pK+3)->KKauto&0x01) ){
			::ZeroMemory( val, 6 );
			memcpy( (pK+3)->KKval1, (pK+43)->KKval1, 6 );
		}
		if( !((pK+4)->KKauto&0x01) ){
			::ZeroMemory( val, 6 );
			memcpy( (pK+4)->KKval1, (pK+43)->KKval2, 6 );
		}
//2016.10.20 DELETE START
//	}
//2016.10.20 DELETE END

	//★//[13'09.18] 固定資産等の譲渡 追加
	if( !((pK+12)->KKauto&0x01) ){
		::ZeroMemory( val, 6 );
		memcpy( (pK+12)->KKval1, (pK+48)->KKval1, 6 );
	}
	if( !((pK+13)->KKauto&0x01) ){
		::ZeroMemory( val, 6 );
		memcpy( (pK+13)->KKval1, (pK+48)->KKval2, 6 );
	}


	Shz_SaleCalc();
	return 0;
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_DelData()
{
	// 事業所得
	Shz_DelKkval( (*m_pTaxListData)->NRec.KJval, (*m_pTaxListData)->Head.KHkjmx );
	// 不動産所得
	Shz_DelKkval( (*m_pTaxListData)->NRec.KFval, (*m_pTaxListData)->Head.KHkfmx );
	// 農業所得
	Shz_DelKkval( (*m_pTaxListData)->NRec.KNval, (*m_pTaxListData)->Head.KHknmx );
	// 課税売上高計算表
	Shz_DelKkval( (*m_pTaxListData)->NRec.KUval, (*m_pTaxListData)->Head.KHkumx );
	// 課税仕入高計算表
	Shz_DelKkval( (*m_pTaxListData)->NRec.KSval, (*m_pTaxListData)->Head.KHksmx );

	// 項目名
	::ZeroMemory( (*m_pTaxListData)->Head.KHkunm, sizeof((*m_pTaxListData)->Head.KHkunm));
	::ZeroMemory( (*m_pTaxListData)->Head.KHksnm, sizeof((*m_pTaxListData)->Head.KHksnm));
	return 0;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
// 引数	pK	：
//		max	：
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_DelKkval( KKVAL* pK, int max )
{
	for( int i=0; i<max; i++ ){
		(pK+i)->KKauto = 0x00;
		::ZeroMemory( (pK+i)->KKnam, sizeof((pK+i)->KKnam));
		::ZeroMemory( (pK+i)->KKval1, sizeof((pK+i)->KKval1));
		::ZeroMemory( (pK+i)->KKval2, sizeof((pK+i)->KKval2));
		::ZeroMemory( (pK+i)->KKval3, sizeof((pK+i)->KKval3));
		::ZeroMemory( (pK+i)->KKval4, sizeof((pK+i)->KKval4));
		::ZeroMemory( (pK+i)->KKval5, sizeof((pK+i)->KKval5));
		::ZeroMemory( (pK+i)->KKval6, sizeof((pK+i)->KKval6));
		::ZeroMemory( (pK+i)->KKval7, sizeof((pK+i)->KKval7));
		::ZeroMemory( (pK+i)->KKval8, sizeof((pK+i)->KKval8));

	}
	return 0;
}

//-----------------------------------------------------------------------------
// 実額変更チェック
//-----------------------------------------------------------------------------
// 引数	str	：
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_ChkValChange( CString &str )
{
	int		st, flg;

	flg = 0;
	str.Empty();

	st = Shz_ChkKKauto( (*m_pTaxListData)->NRec.KUval, (*m_pTaxListData)->Head.KHkumx );
	flg = st;
	if( st ){
		str += _T("課税売上高計算表の金額が変更されています。\n\n");
	}

	st = Shz_ChkKKauto( (*m_pTaxListData)->NRec.KSval, (*m_pTaxListData)->Head.KHksmx );
	flg |= st;
	if( st ){
		str += _T("課税仕入高計算表の金額が変更されています。\n\n");
	}

	if( flg ){
		str += _T("このまま処理を終了しますか？\n");
	}
	return flg;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
// 引数	pK	：
//		max	：
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_ChkKKauto( KKVAL* pK, int max )
{
	int	st = 0;
	for( int i=0; i<max; i++ ){
		if( (pK+i)->KKauto ){
			st = 1;	
			break;
		}
	}
	return st;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// 引数	ans	：	
//		val	：	
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_shohizei( char* ans, char* val )
{
	char	PW1[6], PW2[6];

	m_Arith.l_input( PW1, _T("5") );
	m_Arith.l_input( PW2, _T("100") );
	m_Arith.l_mul( ans, val, PW1 );
	m_Arith.l_div( ans, ans, PW2 );

	return 0;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// 引数	ans	：
//		val	：
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_urcalc( char* ans, char* val )
{
	char	PW1[6], PW2[6];

	m_Arith.l_input( PW1, _T("100") );
	m_Arith.l_input( PW2, _T("105") );
	m_Arith.l_mul( ans, val, PW1 );
	m_Arith.l_div( ans, ans, PW2 );

	return 0;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// 引数	ans	：
//		val	：
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_sicalc( char* ans, char* val ) //Shz_shohizei4と同じ
{
	char	PW1[6], PW2[6];

	m_Arith.l_input( PW1, "4" );
	m_Arith.l_input( PW2, "105" );
	m_Arith.l_mul( ans, val, PW1 );
	m_Arith.l_div( ans, ans, PW2 );

	return 0;
}

//-----------------------------------------------------------------------------
// 非連動　申告書転記
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_ReportPost()
{
	int		ret=0;

//2016.06.22 UPDATE START
//	if( Util.IsUnConnectMst(m_pZmSub->zvol) && (Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_KANNI) ){// 非連動ｄｂで簡易課税
	if( Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_KANNI ){// 簡易課税
//2016.06.22 UPDATE END

		if( (*m_pSnHeadData)->SVmzsw == 1 ){//経過措置あり
			if( Shz_DataCpyKniKei() == -1 ){
				ret = -1;
			}
		}
		else{
			if( Shz_DataCpyKni() == -1 ){
				ret = -1;
			}
		}
	}
	else{
		if( (*m_pSnHeadData)->SVmzsw == 1 ){//経過措置あり
			if( Shz_DataCpyUsuKei() == -1 ){
				ret = -1;
			}
		}
		else{
			if( Shz_DataCpyUsu() == -1 ){
				ret = -1;
			}
		}
	}

	return ret;
}


int	CShzCalc::Shz_FileRead( void )
{
	return 0;
}

int	CShzCalc::Shz_FileWrite(void )
{
	return 0;
}

//-----------------------------------------------------------------------------
// 通常課税
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_DataCpyUsu()
{
	::ZeroMemory(&tenkidata , sizeof(tenkidata));

	KKVAL*	pJ = (*m_pTaxListData)->NRec.KJval;
	KKVAL*	pF = (*m_pTaxListData)->NRec.KFval;
	KKVAL*	pN = (*m_pTaxListData)->NRec.KNval;

	KKVAL*	pU = (*m_pTaxListData)->NRec.KUval;
	KKVAL*	pS = (*m_pTaxListData)->NRec.KSval;
	char	val[6], val2[6], PW1[6], PW2[6];

	// 第27表-(1)号様式
	// ①
	::ZeroMemory( val, 6 );
	::ZeroMemory( PW1, 6 );
	m_Arith.l_input( PW1, _T("1000") );
	m_Arith.l_div( val, (pU+16)->KKval1, PW1 );
	m_Arith.l_mul( val, val, PW1 );
	memcpy(tenkidata.Sn_1F1C , val , 6);

	// ②
	::ZeroMemory( val, 6 );
	::ZeroMemory( PW1, 6 );
	::ZeroMemory( PW2, 6 );
	m_Arith.l_input( PW1, _T("63") );
	m_Arith.l_input( PW2, _T("1000") );
	m_Arith.l_mul( val,tenkidata.Sn_1F1C, PW1 );
	m_Arith.l_div( val, val, PW2 );
	memcpy(tenkidata.Sn_1F2C , val , 6);


	// ④

	// ⑤
	::ZeroMemory( val, 6 );
	memcpy( val, (pJ+33)->KKval3, 6 );
	m_Arith.l_add( val, val, (pF+15)->KKval3 );
	m_Arith.l_add( val, val, (pN+38)->KKval3 );
	if( (*m_pTaxListData)->Head.KHstax ){
		::ZeroMemory( val2, 6 );

		
		m_Arith.l_input( PW1, _T("8") );
		m_Arith.l_input( PW2, _T("100") );
		m_Arith.l_mul( val2,val, PW1 );
		m_Arith.l_div( val2, val2, PW2 );

		m_Arith.l_add( val, val, val2 );
	}
	::ZeroMemory( val2, 6 );
//-- '15.01.09 --
//	Shz_sicalc( val2, val );
//	memcpy(tenkidata.Sn_1F5C , val , 6);//'15.01.09
//---------------
	Shz_shohizei63( val2, val );
	memcpy( tenkidata.Sn_1F5C, val2 , 6 );
//---------------

	// 付表2
	// ①
	::ZeroMemory( val, 6 );
	memcpy( val, (pJ+33)->KKval3, 6 );
	m_Arith.l_add( val, val, (pF+15)->KKval3 );
	m_Arith.l_add( val, val, (pN+38)->KKval3 );
	if( !(*m_pTaxListData)->Head.KHstax ){
		Shz_shohizei100_108( val, val );//'15.01.09
//		Shz_shohizei63( val, val );
	}

	m_Arith.l_sub( tenkidata.Sn_2F1C, (pU+16)->KKval1, val );
	memcpy(tenkidata.Sn_2F8C , (pS+16)->KKval2 , 6);
	memcpy(tenkidata.Sn_2F9C , (pS+16)->KKval1 , 6);


	return 0;

}

//-----------------------------------------------------------------------------
// 簡易課税
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_DataCpyKni()
{
	KKVAL*	pU = (*m_pTaxListData)->NRec.KUval;
//2017.12.04 INSERT START
	KKVAL*	pJ = (*m_pTaxListData)->NRec.KJval;
	KKVAL*	pF = (*m_pTaxListData)->NRec.KFval;
	KKVAL*	pN = (*m_pTaxListData)->NRec.KNval;
//2017.12.04 INSERT END
	char	val[6], val2[6], PW1[6], PW2[6], clc[6];

	::ZeroMemory(&tenkidata , sizeof(tenkidata));

	// 第27表-(2)号様式
	// ①
	::ZeroMemory( val, 6 );
	::ZeroMemory( PW1, 6 );
	m_Arith.l_input( PW1, _T("1000") );
	m_Arith.l_div( val, (pU+16)->KKval1, PW1 );
	m_Arith.l_mul( val, val, PW1 );
	memcpy(tenkidata.Sn_1F1C , val , 6);

	// ②
	::ZeroMemory( val, 6 );
	::ZeroMemory( PW1, 6 );
	::ZeroMemory( PW2, 6 );
	m_Arith.l_input( PW1, _T("63") );
	m_Arith.l_input( PW2, _T("1000") );
	m_Arith.l_mul( val, tenkidata.Sn_1F1C, PW1 );
	m_Arith.l_div( val, val, PW2 );
	memcpy(tenkidata.Sn_1F2C , val , 6);

	// ④

	// ⑤
//2017.12.04 INSERT START
	if( Util.IsUnConnectMst(m_pZmSub->zvol) ){
//2017.12.04 INSERT END
	::ZeroMemory( clc, 6 );
	for( int i=0, id=40; i<5; i++ ){
		::ZeroMemory( val, 6 );
		memcpy( val, (pU+id)->KKval4, 6 );
		if( (i==2) || (i==4) ){
			m_Arith.l_add( val, val, (pU+(id+1))->KKval4 );
			id++;
		}
		if( (*m_pTaxListData)->Head.KHstax ){
			::ZeroMemory( val2, 6 );

			
			m_Arith.l_input( PW1, _T("8") );
			m_Arith.l_input( PW2, _T("100") );
			m_Arith.l_mul( val2,val, PW1 );
			m_Arith.l_div( val2, val2, PW2 );
			
			m_Arith.l_add( val, val, val2 );
		}
		::ZeroMemory( val2, 6 );
		Shz_shohizei63( val2, val );

		id++;
		m_Arith.l_add( clc, clc, val2 );
	}
	memcpy(tenkidata.Sn_1F5C , clc , 6);
//2017.12.04 INSERT START
	}else{
		::ZeroMemory( val, 6 );
		memcpy( val, (pJ+33)->KKval3, 6 );
		m_Arith.l_add( val, val, (pF+15)->KKval3 );
		m_Arith.l_add( val, val, (pN+38)->KKval3 );
		if( (*m_pTaxListData)->Head.KHstax ){
			::ZeroMemory( val2, 6 );

			
			m_Arith.l_input( PW1, _T("8") );
			m_Arith.l_input( PW2, _T("100") );
			m_Arith.l_mul( val2,val, PW1 );
			m_Arith.l_div( val2, val2, PW2 );

			m_Arith.l_add( val, val, val2 );
		}
		::ZeroMemory( val2, 6 );
		Shz_shohizei63( val2, val );
		memcpy( tenkidata.Sn_1F5C, val2 , 6 );
	}
//2017.12.04 INSERT END

	// ⑮
	::ZeroMemory( val, 6 );
	memcpy( val, (pU+47)->KKval3, 6 );
	if( !(*m_pTaxListData)->Head.KHstax ){
//		Shz_urcalc( val, val );
		Shz_shohizei100_108( val, val );

	}
	::ZeroMemory( val2, 6 );
	memcpy( val2, (pU+47)->KKval4, 6 );
	if( !(*m_pTaxListData)->Head.KHstax ){
//		Shz_urcalc( val2, val2 );
		Shz_shohizei100_108( val2, val2 );
	}
	m_Arith.l_sub( val, val, val2 );

//	memcpy(tenkidata.Sn_KZURI , clc , 6);

	// 付表5
	::ZeroMemory( clc, 6 );
//2016.10.20 INSERT START
	if( m_SixKindFlg == TRUE ){
		for( int i=0, id=40; i<6; i++ ){
			::ZeroMemory( val, 6 );
			memcpy( val, (pU+id)->KKval3, 6 );
			if(i==2){
				m_Arith.l_add( val, val, (pU+(id+1))->KKval3 );
			}
			else if( i == 3 ){		// 10/04/06 追加
				m_Arith.l_add( val, val, (pU+48)->KKval3 );
			}
			if( !(*m_pTaxListData)->Head.KHstax ){
	//			Shz_urcalc( val, val );
				Shz_shohizei100_108( val, val );
			}

			::ZeroMemory( val2, 6 );
			memcpy( val2, (pU+id)->KKval4, 6 );
			if(i==2){
				m_Arith.l_add( val2, val2, (pU+(id+1))->KKval4 );
				id++;
			}
			if( !(*m_pTaxListData)->Head.KHstax ){
	//			Shz_urcalc( val2, val2 );
				Shz_shohizei100_108( val2, val2 );
			}
			id++;
			m_Arith.l_sub( val, val, val2 );

			//---->'14.11.06
			//マイナス値の場合には０にして転記する
			if( m_Arith.l_test(val) < 0 ){
				::ZeroMemory(val , sizeof(val));
			}
			//<-------------

			switch( i ){
				case 0:		
					memcpy(tenkidata.Sn_5FURI1C , val , 6);
					break;
				case 1:		
					memcpy(tenkidata.Sn_5FURI2C , val , 6);
					break;
				
				case 2:		
					memcpy(tenkidata.Sn_5FURI3C , val , 6);
					break;

				case 3:		
					memcpy(tenkidata.Sn_5FURI4C , val , 6);
					break;

				case 4:		
					memcpy(tenkidata.Sn_5FURI5C , val , 6);
					break;

				case 5:		
					memcpy(tenkidata.Sn_5FURI6C , val , 6);
					break;
			}
			m_Arith.l_add( clc, clc, val );
		}
	}
	else{
//2016.10.20 INSERT END
		for( int i=0, id=40; i<5; i++ ){
			::ZeroMemory( val, 6 );
			memcpy( val, (pU+id)->KKval3, 6 );
			if( (i==2) || (i==4) ){
				m_Arith.l_add( val, val, (pU+(id+1))->KKval3 );
			}
			else if( i == 3 ){		// 10/04/06 追加
				m_Arith.l_add( val, val, (pU+48)->KKval3 );
			}
			if( !(*m_pTaxListData)->Head.KHstax ){
	//			Shz_urcalc( val, val );
				Shz_shohizei100_108( val, val );
			}

			::ZeroMemory( val2, 6 );
			memcpy( val2, (pU+id)->KKval4, 6 );
			if( (i==2) || (i==4) ){
				m_Arith.l_add( val2, val2, (pU+(id+1))->KKval4 );
				id++;
			}
			if( !(*m_pTaxListData)->Head.KHstax ){
	//			Shz_urcalc( val2, val2 );
				Shz_shohizei100_108( val2, val2 );
			}
			id++;
			m_Arith.l_sub( val, val, val2 );

			//---->'14.11.06
			//マイナス値の場合には０にして転記する
			if( m_Arith.l_test(val) < 0 ){
				::ZeroMemory(val , sizeof(val));
			}
			//<-------------

			switch( i ){
				case 0:		
					memcpy(tenkidata.Sn_5FURI1C , val , 6);
					break;
				case 1:		
					memcpy(tenkidata.Sn_5FURI2C , val , 6);
					break;
				
				case 2:		
					memcpy(tenkidata.Sn_5FURI3C , val , 6);
					break;

				case 3:		
					memcpy(tenkidata.Sn_5FURI4C , val , 6);
					break;

				case 4:		
					memcpy(tenkidata.Sn_5FURI5C , val , 6);
					break;
			}
			m_Arith.l_add( clc, clc, val );
		}
//2016.10.20 INSERT START
	}
//2016.10.20 INSERT END

//	// 付表5
	memcpy(tenkidata.Sn_5FURITA , clc , 6);

	::ZeroMemory( tenkidata.Sn_5FUZTT, 6 );
	::ZeroMemory( tenkidata.Sn_5FUZT1, 6 );
	::ZeroMemory( tenkidata.Sn_5FUZT2, 6 );
	::ZeroMemory( tenkidata.Sn_5FUZT3, 6 );
	::ZeroMemory( tenkidata.Sn_5FUZT4, 6 );
	::ZeroMemory( tenkidata.Sn_5FUZT5, 6 );
	::ZeroMemory( tenkidata.Sn_5FUZT6, 6 );	// '16.12.27

	return 0;
}


//'14.08.20
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

//------------------->以下８％経過措置用集計関数
//-----------------------------------------------------------------------------
// 課税仕入高への転記
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_BuyKeiPost()
{
	if( !( Util.IsUnConnectMst(m_pZmSub->zvol) && (Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_KANNI) ) ){// 非連動ｄｂで簡易課税じゃ無い場合
	
		KKVAL*	pK = (*m_pTaxListData)->NRec.KSval;
		if( (*m_pTaxListData)->GKBN & 0x01 ){	// 事業所
			if( !((pK+KBUYLINE1)->KKauto&0x01) ){
				m_Arith.l_add( (pK+KBUYLINE1)->KKval1, (*m_pTaxListData)->NRec.KJval[2].KKval1, (*m_pTaxListData)->NRec.KJval[31].KKval1 );
			}
			if( !((pK+KBUYLINE2)->KKauto&0x01) ){
				m_Arith.l_add( (pK+KBUYLINE2)->KKval1, (*m_pTaxListData)->NRec.KJval[2].KKval2, (*m_pTaxListData)->NRec.KJval[31].KKval2 );
			}
			if( !((pK+KBUYLINE3)->KKauto&0x02) ){
				m_Arith.l_add( (pK+KBUYLINE3)->KKval2, (*m_pTaxListData)->NRec.KJval[2].KKval4, (*m_pTaxListData)->NRec.KJval[31].KKval4 );
			}
			if( !((pK+KBUYLINE3)->KKauto&0x04) ){
				m_Arith.l_add( (pK+KBUYLINE3)->KKval3, (*m_pTaxListData)->NRec.KJval[2].KKval5, (*m_pTaxListData)->NRec.KJval[31].KKval5 );
			}
		}
		if( (*m_pTaxListData)->GKBN & 0x02 ){	// 不動産
			if( !((pK+KBUYLINE4)->KKauto&0x01) ){
				memcpy( (pK+KBUYLINE4)->KKval1, (*m_pTaxListData)->NRec.KFval[13].KKval1, 6 );
			}
			if( !((pK+KBUYLINE5)->KKauto&0x01) ){
				memcpy( (pK+KBUYLINE5)->KKval1, (*m_pTaxListData)->NRec.KFval[13].KKval2, 6 );
			}
			if( !((pK+KBUYLINE6)->KKauto&0x02) ){
				memcpy( (pK+KBUYLINE6)->KKval2, (*m_pTaxListData)->NRec.KFval[13].KKval4, 6 );
			}
			if( !((pK+KBUYLINE6)->KKauto&0x04) ){
				memcpy( (pK+KBUYLINE6)->KKval3, (*m_pTaxListData)->NRec.KFval[13].KKval5, 6 );
			}
		}
		/*
		if( (*m_pTaxListData)->GKBN & 0x04 ){	// 農業
			if( !((pK+3)->KKauto&0x01) ){
				memcpy( (pK+3)->KKval1, (*m_pTaxListData)->NRec.KNval[32].KKval1, 6 );
			}
			if( !((pK+4)->KKauto&0x01) ){
				memcpy( (pK+4)->KKval1, (*m_pTaxListData)->NRec.KNval[32].KKval2, 6 );
			}
		}
		*/
		if( (*m_pTaxListData)->GKBN & 0x04 ){	// 農業
			if( !((pK+KBUYLINE1)->KKauto&0x01) ){
				m_Arith.l_add( (pK+KBUYLINE1)->KKval1, (*m_pTaxListData)->NRec.KJval[2].KKval1, (*m_pTaxListData)->NRec.KJval[31].KKval1 );
				m_Arith.l_add( (pK+KBUYLINE1)->KKval1, (pK+KBUYLINE1)->KKval1, (*m_pTaxListData)->NRec.KNval[32].KKval1 );
			}
			if( !((pK+KBUYLINE2)->KKauto&0x01) ){
				m_Arith.l_add( (pK+KBUYLINE2)->KKval1, (*m_pTaxListData)->NRec.KJval[2].KKval2, (*m_pTaxListData)->NRec.KJval[31].KKval2 );
				m_Arith.l_add( (pK+KBUYLINE2)->KKval1, (pK+KBUYLINE2)->KKval1, (*m_pTaxListData)->NRec.KNval[32].KKval2 );
			}
			if( !((pK+KBUYLINE3)->KKauto&0x02) ){
				m_Arith.l_add( (pK+KBUYLINE3)->KKval2, (*m_pTaxListData)->NRec.KJval[2].KKval4, (*m_pTaxListData)->NRec.KJval[31].KKval4 );
				m_Arith.l_add( (pK+KBUYLINE3)->KKval2, (pK+KBUYLINE3)->KKval2, (*m_pTaxListData)->NRec.KNval[32].KKval4 );
			}
			if( !((pK+KBUYLINE3)->KKauto&0x04) ){
				m_Arith.l_add( (pK+KBUYLINE3)->KKval3, (*m_pTaxListData)->NRec.KJval[2].KKval5, (*m_pTaxListData)->NRec.KJval[31].KKval5 );
				m_Arith.l_add( (pK+KBUYLINE3)->KKval3, (pK+KBUYLINE3)->KKval3, (*m_pTaxListData)->NRec.KNval[32].KKval5 );
			}
		}
	}
	

	Shz_BuyKeiCalc();
	return 0;
}


//-----------------------------------------------------------------------------
// 課税売上高への転記
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_SaleKeiPost()
{
//	if( !(pSyzShin->KniFlg&0x01) ){
	if( !( Util.IsUnConnectMst(m_pZmSub->zvol) && (Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_KANNI) ) ){// 非連動ｄｂで簡易課税じゃ無い場合'14.10.08

		KKVAL*	pK = (*m_pTaxListData)->NRec.KUval;

		if( (*m_pTaxListData)->GKBN & 0x01 ){	// 事業所
			if( !((pK+KSELLINE1)->KKauto&0x01) ){
				memcpy( (pK+KSELLINE1)->KKval1, (*m_pTaxListData)->NRec.KJval[0].KKval1, 6 );
			}
			if( !((pK+KSELLINE2)->KKauto&0x01) ){
				memcpy( (pK+KSELLINE2)->KKval1, (*m_pTaxListData)->NRec.KJval[0].KKval2, 6 );
			}
			if( !((pK+KSELLINE3)->KKauto&0x02) ){
				memcpy( (pK+KSELLINE3)->KKval2, (*m_pTaxListData)->NRec.KJval[0].KKval4, 6 );
			}
			if( !((pK+KSELLINE3)->KKauto&0x04) ){
				memcpy( (pK+KSELLINE3)->KKval3, (*m_pTaxListData)->NRec.KJval[0].KKval5, 6 );
			}
		}
		if( (*m_pTaxListData)->GKBN & 0x02 ){	// 不動産
			if( !((pK+KSELLINE4)->KKauto&0x01) ){
				memcpy( (pK+KSELLINE4)->KKval1, (*m_pTaxListData)->NRec.KFval[3].KKval1, 6 );
			}
			if( !((pK+KSELLINE5)->KKauto&0x01) ){
				memcpy( (pK+KSELLINE5)->KKval1, (*m_pTaxListData)->NRec.KFval[3].KKval2, 6 );
			}
			if( !((pK+KSELLINE6)->KKauto&0x02) ){
				memcpy( (pK+KSELLINE6)->KKval2, (*m_pTaxListData)->NRec.KFval[3].KKval4, 6 );
			}
			if( !((pK+KSELLINE6)->KKauto&0x04) ){
				memcpy( (pK+KSELLINE6)->KKval3, (*m_pTaxListData)->NRec.KFval[3].KKval5, 6 );
			}
		}
		if( (*m_pTaxListData)->GKBN & 0x04 ){	// 農業
			if( !((pK+KSELLINE1)->KKauto&0x01) ){
				memcpy( (pK+KSELLINE1)->KKval1, (*m_pTaxListData)->NRec.KJval[0].KKval1, 6 );
				m_Arith.l_add( (pK+KSELLINE1)->KKval1, (pK+KSELLINE1)->KKval1, (*m_pTaxListData)->NRec.KNval[5].KKval1 );
			}
			if( !((pK+KSELLINE2)->KKauto&0x01) ){
				memcpy( (pK+KSELLINE2)->KKval1, (*m_pTaxListData)->NRec.KJval[0].KKval2, 6 );
				m_Arith.l_add( (pK+KSELLINE2)->KKval1, (pK+KSELLINE2)->KKval1, (*m_pTaxListData)->NRec.KNval[5].KKval2 );
			}
			if( !((pK+KSELLINE3)->KKauto&0x02) ){
				memcpy( (pK+KSELLINE3)->KKval2, (*m_pTaxListData)->NRec.KJval[0].KKval4, 6 );
				m_Arith.l_add( (pK+KSELLINE3)->KKval2, (pK+KSELLINE3)->KKval2, (*m_pTaxListData)->NRec.KNval[5].KKval4 );
			}
			if( !((pK+KSELLINE3)->KKauto&0x04) ){
				memcpy( (pK+KSELLINE3)->KKval3, (*m_pTaxListData)->NRec.KJval[0].KKval5, 6 );
				m_Arith.l_add( (pK+KSELLINE3)->KKval3, (pK+KSELLINE3)->KKval3, (*m_pTaxListData)->NRec.KNval[5].KKval5 );
			}
		}

	}else{
		Shz_KniKeiPost();
	}
	

	Shz_SaleKeiCalc();
	return 0;
}


//-----------------------------------------------------------------------------
// 課税仕入高
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_BuyKeiCalc()
{

	KKVAL*	pK = (*m_pTaxListData)->NRec.KSval;

	m_Arith.l_sub( (pK + KBUYLINE3)->KKval1, (pK + KBUYLINE1)->KKval1, (pK + KBUYLINE2)->KKval1 );		//【事業所得に係る課税仕入高】差引課税仕入高（①－②）
	m_Arith.l_sub( (pK + KBUYLINE6)->KKval1, (pK + KBUYLINE4)->KKval1, (pK + KBUYLINE5)->KKval1 );		//【不動産所得に係る課税仕入高】農業　差引課税仕入高（④－⑤）
	m_Arith.l_sub( (pK + KBUYLINE9)->KKval1, (pK + KBUYLINE7)->KKval1, (pK + KBUYLINE8)->KKval1 );		//【 [   ]所得に係る課税仕入高】差引課税仕入高（⑩－⑪）
	m_Arith.l_sub( (pK + KBUYLINE12)->KKval1, (pK + KBUYLINE10)->KKval1, (pK + KBUYLINE11)->KKval1 );	//【業務用資産の取得に係る課税仕入高】差引課税仕入高（⑬－⑭）

	//課税仕入高の合計額(③＋⑥＋⑨＋⑫) A
	::ZeroMemory( (pK + KBUYLINE13)->KKval1, 6 );
	m_Arith.l_add( (pK + KBUYLINE13)->KKval1, (pK + KBUYLINE3)->KKval1, (pK + KBUYLINE6)->KKval1 );
	m_Arith.l_add( (pK + KBUYLINE13)->KKval1, (pK + KBUYLINE13)->KKval1, (pK + KBUYLINE9)->KKval1 );
	m_Arith.l_add( (pK + KBUYLINE13)->KKval1, (pK + KBUYLINE13)->KKval1, (pK + KBUYLINE12)->KKval1 );
	
	//課税仕入高の合計額(③＋⑥＋⑨＋⑫) B
	::ZeroMemory( (pK + KBUYLINE13)->KKval2, 6 );
	m_Arith.l_add( (pK + KBUYLINE13)->KKval2, (pK + KBUYLINE3)->KKval2, (pK + KBUYLINE6)->KKval2 );
	m_Arith.l_add( (pK + KBUYLINE13)->KKval2, (pK + KBUYLINE13)->KKval2, (pK + KBUYLINE9)->KKval2 );
	m_Arith.l_add( (pK + KBUYLINE13)->KKval2, (pK + KBUYLINE13)->KKval2, (pK + KBUYLINE12)->KKval2 );


	//課税仕入高の合計額(③＋⑥＋⑨＋⑫) C
	::ZeroMemory( (pK + KBUYLINE13)->KKval3, 6 );
	m_Arith.l_add( (pK + KBUYLINE13)->KKval3, (pK + KBUYLINE3)->KKval3, (pK + KBUYLINE6)->KKval3 );
	m_Arith.l_add( (pK + KBUYLINE13)->KKval3, (pK + KBUYLINE13)->KKval3, (pK + KBUYLINE9)->KKval3 );
	m_Arith.l_add( (pK + KBUYLINE13)->KKval3, (pK + KBUYLINE13)->KKval3, (pK + KBUYLINE12)->KKval3 );


	char	val[6];

	// 仮払消費税(⑬B欄の金額)
	if( (*m_pTaxListData)->Head.KHstax ){
		if( !((pK + KBUYLINE14)->KKauto&0x04) ){
			::ZeroMemory( val, 6 );
//			Shz_shohizei4( val, (pK + KBUYLINE13)->KKval2 );
			Shz_shohizei5_100( val, (pK + KBUYLINE13)->KKval2 );
			memcpy( (pK + KBUYLINE14)->KKval3, val, 6 );
		}
	}

	if( !((pK + KBUYLINE14)->KKauto&0x02) ){
		if( (*m_pTaxListData)->Head.KHstax ){
			m_Arith.l_add( (pK + KBUYLINE14)->KKval2, (pK + KBUYLINE13)->KKval2, (pK + KBUYLINE14)->KKval3 );
		}
		else{
			memcpy( (pK + KBUYLINE14)->KKval2, (pK + KBUYLINE13)->KKval2, 6 );
		}
	}
	if( !((pK + KBUYLINE14)->KKauto&0x01) ){
		::ZeroMemory( val, 6 );
		Shz_shohizei4( val, (pK + KBUYLINE14)->KKval2 );
		memcpy( (pK + KBUYLINE14)->KKval1, val, 6 );
	}

	// 仮払消費税(⑬C欄の金額)
	if( (*m_pTaxListData)->Head.KHstax ){
		if( !((pK + KBUYLINE15)->KKauto&0x04) ){
			::ZeroMemory( val, 6 );
//			Shz_shohizei63( val, (pK + KBUYLINE13)->KKval3 );
			Shz_shohizei8_100( val, (pK + KBUYLINE13)->KKval3 );
			memcpy( (pK + KBUYLINE15)->KKval3, val, 6 );
		}
	}

	if( !((pK + KBUYLINE15)->KKauto&0x02) ){
		if( (*m_pTaxListData)->Head.KHstax ){
			m_Arith.l_add( (pK + KBUYLINE15)->KKval2, (pK + KBUYLINE13)->KKval3, (pK + KBUYLINE15)->KKval3 );
		}
		else{
			memcpy( (pK + KBUYLINE15)->KKval2, (pK + KBUYLINE13)->KKval3, 6 );
		}
	}
	if( !((pK + KBUYLINE15)->KKauto&0x01) ){
		::ZeroMemory( val, 6 );
		Shz_shohizei63( val, (pK + KBUYLINE15)->KKval2 );
		memcpy( (pK + KBUYLINE15)->KKval1, val, 6 );
	}

	//合計（⑭＋⑮）
	::ZeroMemory( (pK + KBUYLINE16)->KKval1, 6 );
	m_Arith.l_add( (pK + KBUYLINE16)->KKval1, (pK + KBUYLINE14)->KKval1, (pK + KBUYLINE15)->KKval1 );
	



	/*
	KKVAL*	pK = (*m_pTaxListData)->NRec.KSval;

	m_Arith.l_sub( (pK+2)->KKval1, (pK+0)->KKval1, (pK+1)->KKval1 );
	m_Arith.l_sub( (pK+5)->KKval1, (pK+3)->KKval1, (pK+4)->KKval1 );
	m_Arith.l_sub( (pK+8)->KKval1, (pK+6)->KKval1, (pK+7)->KKval1 );
	m_Arith.l_sub( (pK+11)->KKval1, (pK+9)->KKval1, (pK+10)->KKval1 );
	m_Arith.l_sub( (pK+14)->KKval1, (pK+12)->KKval1, (pK+13)->KKval1 );

	::ZeroMemory( (pK+15)->KKval1, 6 );
	m_Arith.l_add( (pK+15)->KKval1, (pK+2)->KKval1, (pK+5)->KKval1 );
	m_Arith.l_add( (pK+15)->KKval1, (pK+15)->KKval1, (pK+8)->KKval1 );
	m_Arith.l_add( (pK+15)->KKval1, (pK+15)->KKval1, (pK+11)->KKval1 );
	m_Arith.l_add( (pK+15)->KKval1, (pK+15)->KKval1, (pK+14)->KKval1 );

	char	val[6];

	// 仮払消費税
	if( (*m_pTaxListData)->Head.KHstax ){
		if( !((pK+16)->KKauto&0x04) ){
			::ZeroMemory( val, 6 );
			Shz_shohizei( val, (pK+15)->KKval1 );
			memcpy( (pK+16)->KKval3, val, 6 );
		}
	}

	if( !((pK+16)->KKauto&0x02) ){
		if( (*m_pTaxListData)->Head.KHstax ){
			m_Arith.l_add( (pK+16)->KKval2, (pK+15)->KKval1, (pK+16)->KKval3 );
		}
		else{
			memcpy( (pK+16)->KKval2, (pK+15)->KKval1, 6 );
		}
	}
	if( !((pK+16)->KKauto&0x01) ){
		::ZeroMemory( val, 6 );
		Shz_sicalc( val, (pK+16)->KKval2 );
		memcpy( (pK+16)->KKval1, val, 6 );
	}

	*/


	return 0;
}

//-----------------------------------------------------------------------------
// 課税売上高
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_SaleKeiCalc()
{

	KKVAL*	pK = (*m_pTaxListData)->NRec.KUval;

	m_Arith.l_sub( (pK + KSELLINE3)->KKval1, (pK + KSELLINE1)->KKval1, (pK + KSELLINE2)->KKval1 );		//【事業所得に係る課税売上高】差引課税売上高（①－②）
	m_Arith.l_sub( (pK + KSELLINE6)->KKval1, (pK + KSELLINE4)->KKval1, (pK + KSELLINE5)->KKval1 );		//【不動産所得に係る課税売上高】農業　差引課税売上高（④－⑤）
	m_Arith.l_sub( (pK + KSELLINE9)->KKval1, (pK + KSELLINE7)->KKval1, (pK + KSELLINE8)->KKval1 );		//【 [   ]所得に係る課税売上高】差引課税売上高（⑩－⑪）
	m_Arith.l_sub( (pK + KSELLINE12)->KKval1, (pK + KSELLINE10)->KKval1, (pK + KSELLINE11)->KKval1 );	//【業務用資産の取得に係る課税売上高】差引課税仕売上高（⑬－⑭）

	//課税売上高の合計額(③＋⑥＋⑨＋⑫) A
	::ZeroMemory( (pK + KSELLINE13)->KKval1, 6 );
	m_Arith.l_add( (pK + KSELLINE13)->KKval1, (pK + KSELLINE3)->KKval1, (pK + KSELLINE6)->KKval1 );
	m_Arith.l_add( (pK + KSELLINE13)->KKval1, (pK + KSELLINE13)->KKval1, (pK + KSELLINE9)->KKval1 );
	m_Arith.l_add( (pK + KSELLINE13)->KKval1, (pK + KSELLINE13)->KKval1, (pK + KSELLINE12)->KKval1 );
	
	//課税売上高の合計額(③＋⑥＋⑨＋⑫) B
	::ZeroMemory( (pK + KSELLINE13)->KKval2, 6 );
	m_Arith.l_add( (pK + KSELLINE13)->KKval2, (pK + KSELLINE3)->KKval2, (pK + KSELLINE6)->KKval2 );
	m_Arith.l_add( (pK + KSELLINE13)->KKval2, (pK + KSELLINE13)->KKval2, (pK + KSELLINE9)->KKval2 );
	m_Arith.l_add( (pK + KSELLINE13)->KKval2, (pK + KSELLINE13)->KKval2, (pK + KSELLINE12)->KKval2 );


	//課税売上高の合計額(③＋⑥＋⑨＋⑫) C
	::ZeroMemory( (pK + KSELLINE13)->KKval3, 6 );
	m_Arith.l_add( (pK + KSELLINE13)->KKval3, (pK + KSELLINE3)->KKval3, (pK + KSELLINE6)->KKval3 );
	m_Arith.l_add( (pK + KSELLINE13)->KKval3, (pK + KSELLINE13)->KKval3, (pK + KSELLINE9)->KKval3 );
	m_Arith.l_add( (pK + KSELLINE13)->KKval3, (pK + KSELLINE13)->KKval3, (pK + KSELLINE12)->KKval3 );


	char	val[6];

	// 仮払消費税(⑬B欄の金額)
	if( (*m_pTaxListData)->Head.KHstax ){
		if( !((pK + KSELLINE14)->KKauto&0x04) ){
			::ZeroMemory( val, 6 );
//			Shz_shohizei4( val, (pK + KSELLINE13)->KKval2 );
			Shz_shohizei5_100( val, (pK + KSELLINE13)->KKval2 );
			memcpy( (pK + KSELLINE14)->KKval3, val, 6 );
		}
	}

	if( !((pK + KSELLINE14)->KKauto&0x02) ){
		if( (*m_pTaxListData)->Head.KHstax ){
			m_Arith.l_add( (pK + KSELLINE14)->KKval2, (pK + KSELLINE13)->KKval2, (pK + KSELLINE14)->KKval3 );
		}
		else{
			memcpy( (pK + KSELLINE14)->KKval2, (pK + KSELLINE13)->KKval2, 6 );
		}
	}
	if( !((pK + KSELLINE14)->KKauto&0x01) ){
		::ZeroMemory( val, 6 );
//		Shz_shohizei4( val, (pK + KSELLINE14)->KKval2 );
		Shz_shohizei100_105( val, (pK + KSELLINE14)->KKval2 );
		memcpy( (pK + KSELLINE14)->KKval1, val, 6 );
	}

	// 仮払消費税(⑬C欄の金額)
	if( (*m_pTaxListData)->Head.KHstax ){
		if( !((pK + KSELLINE15)->KKauto&0x04) ){
			::ZeroMemory( val, 6 );
//			Shz_shohizei63( val, (pK + KSELLINE13)->KKval3 );
			Shz_shohizei8_100( val, (pK + KSELLINE13)->KKval3 );
			memcpy( (pK + KSELLINE15)->KKval3, val, 6 );
		}
	}

	if( !((pK + KSELLINE15)->KKauto&0x02) ){
		if( (*m_pTaxListData)->Head.KHstax ){
			m_Arith.l_add( (pK + KSELLINE15)->KKval2, (pK + KSELLINE13)->KKval3, (pK + KSELLINE15)->KKval3 );
		}
		else{
			memcpy( (pK + KSELLINE15)->KKval2, (pK + KSELLINE13)->KKval3, 6 );
		}
	}
	if( !((pK + KSELLINE15)->KKauto&0x01) ){
		::ZeroMemory( val, 6 );
//		Shz_shohizei63( val, (pK + KSELLINE15)->KKval2 );
		Shz_shohizei100_108( val, (pK + KSELLINE15)->KKval2 );
		memcpy( (pK + KSELLINE15)->KKval1, val, 6 );
	}

	//合計（⑭＋⑮）
	::ZeroMemory( (pK + KSELLINE16)->KKval1, 6 );
	m_Arith.l_add( (pK + KSELLINE16)->KKval1, (pK + KSELLINE14)->KKval1, (pK + KSELLINE15)->KKval1 );
	

	return 0;



	/*
	KKVAL*	pK = (*m_pTaxListData)->NRec.KUval;

	m_Arith.l_sub( (pK+2)->KKval1, (pK+0)->KKval1, (pK+1)->KKval1 );
	m_Arith.l_sub( (pK+5)->KKval1, (pK+3)->KKval1, (pK+4)->KKval1 );
	m_Arith.l_add( (pK+5)->KKval1, (pK+5)->KKval1, (*m_pTaxListData)->NRec.KNval[4].KKval3 );
	m_Arith.l_sub( (pK+8)->KKval1, (pK+6)->KKval1, (pK+7)->KKval1 );
	m_Arith.l_sub( (pK+11)->KKval1, (pK+9)->KKval1, (pK+10)->KKval1 );
	m_Arith.l_sub( (pK+14)->KKval1, (pK+12)->KKval1, (pK+13)->KKval1 );

	::ZeroMemory( (pK+15)->KKval1, 6 );
	m_Arith.l_add( (pK+15)->KKval1, (pK+2)->KKval1, (pK+5)->KKval1 );
	m_Arith.l_add( (pK+15)->KKval1, (pK+15)->KKval1, (pK+8)->KKval1 );
	m_Arith.l_add( (pK+15)->KKval1, (pK+15)->KKval1, (pK+11)->KKval1 );
	m_Arith.l_add( (pK+15)->KKval1, (pK+15)->KKval1, (pK+14)->KKval1 );

	char	val[6]={0};

	// 仮受消費税
	if( (*m_pTaxListData)->Head.KHstax ){
		if( !((pK+16)->KKauto&0x04) ){
			::ZeroMemory( val, 6 );
			Shz_shohizei( val, (pK+15)->KKval1 );
			memcpy( (pK+16)->KKval3, val, 6 );
		}
	}

	if( !((pK+16)->KKauto&0x02) ){
		if( (*m_pTaxListData)->Head.KHstax ){
			l_add( (pK+16)->KKval2, (pK+15)->KKval1, (pK+16)->KKval3 );
		}
		else{
			memcpy( (pK+16)->KKval2, (pK+15)->KKval1, 6 );
		}
	}
	if( !((pK+16)->KKauto&0x01) ){
		::ZeroMemory( val, 6 );
		Shz_urcalc( val, (pK+16)->KKval2 );
		memcpy( (pK+16)->KKval1, val, 6 );
	}

	return 0;
	*/
}


//-----------------------------------------------------------------------------
//税率４％計算
//-----------------------------------------------------------------------------
// 引数	ans	：	
//		val	：	
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_shohizei4( char* ans, char* val ) //Shz_sicalcと同じ
{
	char	PW1[6], PW2[6];

	m_Arith.l_input( PW1, _T("4") );
	m_Arith.l_input( PW2, _T("105") );
	m_Arith.l_mul( ans, val, PW1 );
	m_Arith.l_div( ans, ans, PW2 );

	return 0;
}

//-----------------------------------------------------------------------------
//税率６．３％計算
//-----------------------------------------------------------------------------
// 引数	ans	：	
//		val	：	
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_shohizei63( char* ans, char* val )
{
	char	PW1[6], PW2[6];

//	m_Arith.l_input( PW1, _T("6.3") );
//	m_Arith.l_input( PW2, _T("108") );
	m_Arith.l_input( PW1, _T("63") );
	m_Arith.l_input( PW2, _T("1080") );
	m_Arith.l_mul( ans, val, PW1 );
	m_Arith.l_div( ans, ans, PW2 );

	return 0;
}

//-----------------------------------------------------------------------------
//税率４％計算
//-----------------------------------------------------------------------------
// 引数	ans	：	
//		val	：	
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_shohizei100_105( char* ans, char* val ) //Shz_urcalcと同じ
{
	char	PW1[6], PW2[6];

	m_Arith.l_input( PW1, _T("100") );
	m_Arith.l_input( PW2, _T("105") );
	m_Arith.l_mul( ans, val, PW1 );
	m_Arith.l_div( ans, ans, PW2 );

	return 0;
}

//-----------------------------------------------------------------------------
//税率６．３％計算
//-----------------------------------------------------------------------------
// 引数	ans	：	
//		val	：	
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_shohizei100_108( char* ans, char* val )
{
	char	PW1[6], PW2[6];

	m_Arith.l_input( PW1, _T("100") );
	m_Arith.l_input( PW2, _T("108") );
	m_Arith.l_mul( ans, val, PW1 );
	m_Arith.l_div( ans, ans, PW2 );

	return 0;
}

//-----------------------------------------------------------------------------
//税率４％計算
//-----------------------------------------------------------------------------
// 引数	ans	：	
//		val	：	
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_shohizei5_100( char* ans, char* val ) //Shz_shohizeiと同じ
{
	char	PW1[6], PW2[6];

	m_Arith.l_input( PW1, _T("5") );
	m_Arith.l_input( PW2, _T("100") );
	m_Arith.l_mul( ans, val, PW1 );
	m_Arith.l_div( ans, ans, PW2 );

	return 0;
}

//-----------------------------------------------------------------------------
//税率６．３％計算
//-----------------------------------------------------------------------------
// 引数	ans	：	
//		val	：	
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_shohizei8_100( char* ans, char* val )
{
	char	PW1[6], PW2[6];

	m_Arith.l_input( PW1, _T("8") );
	m_Arith.l_input( PW2, _T("100") );
	m_Arith.l_mul( ans, val, PW1 );
	m_Arith.l_div( ans, ans, PW2 );

	return 0;
}

//'14.08.21
//-----------------------------------------------------------------------------
// 計算表全集計
//-----------------------------------------------------------------------------
// 引数	mode	：
//-----------------------------------------------------------------------------
// 返送値		：
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_KeiAllCalc( int mode )
{

	//	if( pSyzShin->KniFlg&0x01 ){	// 非連動ｄｂで簡易課税　D0:ON [13'09.18]
	if( Util.IsUnConnectMst(m_pZmSub->zvol) && (Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_KANNI) ){// 非連動ｄｂで簡易課税
		Shz_KniKeiInput();
		Shz_KniKeiPost();
	}
	else{
		switch( mode ){
			case 1:
				Shz_JigyoKeiCalc();
				break;
			case 2:	
				Shz_FudohKeiCalc();
				break;
			case 3:	
				Shz_NohgyoKeiCalc();	
				break;
			default:
				Shz_JigyoKeiCalc();
				Shz_FudohKeiCalc();
				Shz_NohgyoKeiCalc();
				break;
		}
		Shz_SaleKeiPost();
		Shz_BuyKeiPost();
	}

	return 0;

	/*
//	if( pSyzShin->KniFlg&0x01 ){	// 非連動ｄｂで簡易課税　D0:ON [13'09.18]
	if( 0 ){
		Shz_KniInput();
		Shz_KniPost();
	}
	else{
		switch( mode ){
			case 1:
				Shz_JigyoCalc();
				break;
			case 2:	
				Shz_FudohCalc();
				break;
			case 3:	
				Shz_NohgyoCalc();	
				break;
			default:
				Shz_JigyoCalc();
				Shz_FudohCalc();
				Shz_NohgyoCalc();
				break;
		}
		Shz_SalePost();
		Shz_BuyPost();
	}
	return 0;
	*/
}

//#define	FVAL	16

//-----------------------------------------------------------------------------
// 不動産所得
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_FudohKeiCalc()
{
	if( ((*m_pTaxListData)->GKBN==0x01) || ((*m_pTaxListData)->GKBN==0x04) ){
		return 0;
	}
	int	mode;
	for( int i = 0; i < FKLINCNT; i++ ){
		mode = 0;
		switch( i ){
			case KFUDLINE4:		// 収入金額　計
			case KFUDLINE14:	// 必要経費　計
			case KFUDLINE15:	// 差引金額
				continue;
			
			// A欄をC欄へ転記
			case KFUDLINE7:		// 修繕費
				mode = 1;	
				break;
			
			// A欄をB欄へ転記
			case KFUDLINE6:		// 損害保険料
			case KFUDLINE8:		// 減価償却費
			case KFUDLINE9:		// 借入金利子
				mode = 2;
				break;
		}

		Shz_LinCalc( &(*m_pTaxListData)->NRec.KFval[i], mode );
	}

	KKVAL	*pK;
	// 収入金額　計(4)
	pK = &(*m_pTaxListData)->NRec.KFval[KFUDLINE4];
	::ZeroMemory( pK->KKval1, 6 );
	::ZeroMemory( pK->KKval2, 6 );
	::ZeroMemory( pK->KKval4, 6 );//Dの欄（うち税率４％適用分）
	::ZeroMemory( pK->KKval5, 6 );//Eの欄（うち税率６．３％適用分）
	for( int i = KFUDLINE1 ; i < KFUDLINE4 ; i++ ){
		m_Arith.l_add( pK->KKval1, pK->KKval1, (*m_pTaxListData)->NRec.KFval[i].KKval1 );
		m_Arith.l_add( pK->KKval2, pK->KKval2, (*m_pTaxListData)->NRec.KFval[i].KKval2 );
		m_Arith.l_add( pK->KKval4, pK->KKval4, (*m_pTaxListData)->NRec.KFval[i].KKval4 );//Dの欄（うち税率４％適用分）
		m_Arith.l_add( pK->KKval5, pK->KKval5, (*m_pTaxListData)->NRec.KFval[i].KKval5 );//Eの欄（うち税率６．３％適用分）
	}
	Shz_LinCalc( pK, 0 );//Cをセット


	// 必要経費　計(14)
	pK = &(*m_pTaxListData)->NRec.KFval[KFUDLINE14];
	::ZeroMemory( pK->KKval1, 6 );
	::ZeroMemory( pK->KKval2, 6 );
	::ZeroMemory( pK->KKval4, 6 );//Dの欄（うち税率４％適用分）
	::ZeroMemory( pK->KKval5, 6 );//Eの欄（うち税率６．３％適用分）
	for( int i = KFUDLINE5 ; i < KFUDLINE14 ; i++ ){
		m_Arith.l_add( pK->KKval1, pK->KKval1, (*m_pTaxListData)->NRec.KFval[i].KKval1 );

		if( i == KFUDLINE7 ){//修繕費はスキップ
			continue;
		}
		else{
			m_Arith.l_add( pK->KKval2, pK->KKval2, (*m_pTaxListData)->NRec.KFval[i].KKval2 );
		}
	}

	for( int i = KFUDLINE5 ; i < KFUDLINE14 ; i++ ){
		if( i == KFUDLINE6 || i == KFUDLINE8 || i == KFUDLINE9 ){//損害保険料、減価償却費、借入金利子はスキップ
			continue;
		}
		else{
			m_Arith.l_add( pK->KKval4, pK->KKval4, (*m_pTaxListData)->NRec.KFval[i].KKval4 );//Dの欄（うち税率４％適用分）
			m_Arith.l_add( pK->KKval5, pK->KKval5, (*m_pTaxListData)->NRec.KFval[i].KKval5 );//Eの欄（うち税率６．３％適用分）
		}
	}

	Shz_LinCalc( pK, 0 );//Cをセット

	// 差引金額(15)A
	pK = &(*m_pTaxListData)->NRec.KFval[KFUDLINE15];
	m_Arith.l_sub( pK->KKval1, (*m_pTaxListData)->NRec.KFval[KFUDLINE4].KKval1, (*m_pTaxListData)->NRec.KFval[KFUDLINE14].KKval1 );

	return 0;
}


//-----------------------------------------------------------------------------
// 事業所得
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_JigyoKeiCalc()
{
	if( ((*m_pTaxListData)->GKBN==0x02) || ((*m_pTaxListData)->GKBN==0x04) ){
		return 0;
	}
	int	mode;
	for( int i=0; i<JKLINCNT; i++ ){
		mode = 0;
		switch( i ){
			case KJIGLINE2:		//期首商品棚卸高
			case KJIGLINE4:		//小計
			case KJIGLINE5:		//仕入金額
			case KJIGLINE6:		//差引原価
			case KJIGLINE7:		//差引金額
			case KJIGLINE32:	//経費　計
			case KJIGLINE33:	//差引金額
				continue;

			// A欄をC欄へ転記
			case KJIGLINE10:	//水道光熱費
			case KJIGLINE16:	//修繕費
			case KJIGLINE17:	//消耗品費
				mode = 1;
				break;

			// A欄をB欄へ転記
			case KJIGLINE15:	//損害保険料
			case KJIGLINE18:	//減価償却費
			case KJIGLINE22:	//利子割引料
			case KJIGLINE24:	//貸倒金
				mode = 2;	
				break;
		}
		Shz_LinCalc( &(*m_pTaxListData)->NRec.KJval[i], mode );
	}


	KKVAL	*pK;
	// 売上原価　小計(4)A
	pK = &(*m_pTaxListData)->NRec.KJval[KJIGLINE4];
	m_Arith.l_add( pK->KKval1, (*m_pTaxListData)->NRec.KJval[KJIGLINE2].KKval1, (*m_pTaxListData)->NRec.KJval[KJIGLINE3].KKval1 );

	// 売上原価　差引原価(6)A
	pK = &(*m_pTaxListData)->NRec.KJval[KJIGLINE6];
	m_Arith.l_sub( pK->KKval1, (*m_pTaxListData)->NRec.KJval[KJIGLINE4].KKval1, (*m_pTaxListData)->NRec.KJval[KJIGLINE5].KKval1 );

	// 差引金額(6)A
	pK = &(*m_pTaxListData)->NRec.KJval[KJIGLINE7];
	m_Arith.l_sub( pK->KKval1, (*m_pTaxListData)->NRec.KJval[KJIGLINE1].KKval1, (*m_pTaxListData)->NRec.KJval[KJIGLINE6].KKval1 );


	// 経費　計(32)
	pK = &(*m_pTaxListData)->NRec.KJval[KJIGLINE32];
	::ZeroMemory( pK->KKval1, 6 );
	::ZeroMemory( pK->KKval2, 6 );
	::ZeroMemory( pK->KKval4, 6 );//Dの欄（うち税率４％適用分）
	::ZeroMemory( pK->KKval5, 6 );//Eの欄（うち税率６．３％適用分）

	for( int i = KJIGLINE8 ; i < KJIGLINE32 ; i++ ){
		m_Arith.l_add( pK->KKval1, pK->KKval1, (*m_pTaxListData)->NRec.KJval[i].KKval1 );
		if( ( i == KJIGLINE10 ) || ( i == KJIGLINE16 ) || ( i== KJIGLINE17 ) ){//水道光熱費、修繕費、消耗品費はスキップ
			;
		}
		else{
			m_Arith.l_add( pK->KKval2, pK->KKval2, (*m_pTaxListData)->NRec.KJval[i].KKval2 );
		}
	}
	Shz_LinCalc( pK, 0 );

	for( int i = KJIGLINE8 ; i < KJIGLINE32 ; i++ ){
		if( i == KJIGLINE15 || i == KJIGLINE18 || i == KJIGLINE22 || i == KJIGLINE24 ){//損害保険料、減価償却費、利子割引料、貸倒金はスキップ
			continue;
		}
		else{
			m_Arith.l_add( pK->KKval4, pK->KKval4, (*m_pTaxListData)->NRec.KJval[i].KKval4 );//Dの欄（うち税率４％適用分）
			m_Arith.l_add( pK->KKval5, pK->KKval5, (*m_pTaxListData)->NRec.KJval[i].KKval5 );//Eの欄（うち税率６．３％適用分）
		}
	}


	// 差引金額(33)A
	pK = &(*m_pTaxListData)->NRec.KJval[KJIGLINE33];
	m_Arith.l_sub( pK->KKval1, (*m_pTaxListData)->NRec.KJval[KJIGLINE7].KKval1, (*m_pTaxListData)->NRec.KJval[KJIGLINE32].KKval1 );

	return 0;
}


//-----------------------------------------------------------------------------
// 農業所得
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_NohgyoKeiCalc()
{
	if( ((*m_pTaxListData)->GKBN==0x01) || ((*m_pTaxListData)->GKBN==0x02) ){
		return 0;
	}

	int		mode;
	char	val[6] = {0};
	for( int i = 0 ; i < NKLINCNT ; i++ ){
		mode = 0;
		switch( i ){
			case KNOULINE3M:		//未成熟果樹収入
			case KNOULINE4:			//収入金額　小計
			case KNOULINE7:			//計
			case KNOULINE31:		//経費　小計
			case KNOULINE35:		//計
			case KNOULINE36:		//差引金額
			case KNOULINE5:			//農産物の棚卸高　期首
			case KNOULINE6:			//農産物の棚卸高　期末
			case KNOULINE32:		//農産物以外の棚卸高　期首		
			case KNOULINE33:		//農産物以外の棚卸高　期末		
			case KNOULINE34:		//経費から差し引く果樹牛馬等の育成費用
				continue;

			// A欄をC欄へ転記
			case KNOULINE2K:	//家事消費
			case KNOULINE13:	//農具費
			case KNOULINE14:	//農薬・衛生費
			case KNOULINE15:	//諸材料費
			case KNOULINE16:	//修繕費
			case KNOULINE18:	//作業用衣料費
				mode = 1;
				break;

			// A欄をB欄へ転記
			case KNOULINE19:	//農業共済掛金
			case KNOULINE20:	//減価償却費
			case KNOULINE23:	//利子割引料
			case KNOULINE26:	//貸倒金
				mode = 2;
				break;
		}
		Shz_LinCalc( &(*m_pTaxListData)->NRec.KNval[i], mode );
	}

	KKVAL	*pK;
	// 収入金額　小計(4)
	pK = &(*m_pTaxListData)->NRec.KNval[KNOULINE4];
	::ZeroMemory( pK->KKval1, 6 );
	::ZeroMemory( pK->KKval2, 6 );
	::ZeroMemory( pK->KKval3, 6 );
	::ZeroMemory( pK->KKval4, 6 );//Dの欄（うち税率４％適用分）
	::ZeroMemory( pK->KKval5, 6 );//Eの欄（うち税率６．３％適用分）

	for( int i = 0; i < KNOULINE4; i++ ){
		if( i == KNOULINE3M ){//未成熟果樹収入
			;
		}
		else{
			m_Arith.l_add( pK->KKval1, pK->KKval1, (*m_pTaxListData)->NRec.KNval[i].KKval1 );
		}
		if( (i == KNOULINE2K) || (i == KNOULINE3M) ){//家事消費、未成熟果樹収入
			;
		}
		else{
			m_Arith.l_add( pK->KKval2, pK->KKval2, (*m_pTaxListData)->NRec.KNval[i].KKval2 );
		}
		m_Arith.l_add( pK->KKval3, pK->KKval3, (*m_pTaxListData)->NRec.KNval[i].KKval3 );
		m_Arith.l_add( pK->KKval4, pK->KKval4, (*m_pTaxListData)->NRec.KNval[i].KKval4 );
		m_Arith.l_add( pK->KKval5, pK->KKval5, (*m_pTaxListData)->NRec.KNval[i].KKval5 );
	}


	// 収入金額　計(7)A
	pK = &(*m_pTaxListData)->NRec.KNval[KNOULINE7];
	m_Arith.l_sub( val, (*m_pTaxListData)->NRec.KNval[KNOULINE5].KKval1, (*m_pTaxListData)->NRec.KNval[KNOULINE6].KKval1 );
	m_Arith.l_sub( pK->KKval1, (*m_pTaxListData)->NRec.KNval[KNOULINE4].KKval1, val );


	// 経費　小計(31)
	pK = &(*m_pTaxListData)->NRec.KNval[KNOULINE31];
	::ZeroMemory( pK->KKval1, 6 );
	::ZeroMemory( pK->KKval2, 6 );
	::ZeroMemory( pK->KKval4, 6 );
	::ZeroMemory( pK->KKval5, 6 );
	for( int i = KNOULINE8; i < KNOULINE31; i++ ){
		m_Arith.l_add( pK->KKval1, pK->KKval1, (*m_pTaxListData)->NRec.KNval[i].KKval1 );
		if(i == KNOULINE13 || i == KNOULINE14 || i == KNOULINE15 || i == KNOULINE16 || i == KNOULINE18){//農具費、農薬・衛生費、諸材料費、修繕費、作業用衣料費はスキップ（入力不可の為）
			;
		}
		else{
			m_Arith.l_add( pK->KKval2, pK->KKval2, (*m_pTaxListData)->NRec.KNval[i].KKval2 );
		}
	}
	Shz_LinCalc( pK, 0 );

	for( int i = KNOULINE8 ; i < KNOULINE31 ; i++ ){//'14.09.01
		if(i == KNOULINE19 || i == KNOULINE20 || i == KNOULINE23 || i == KNOULINE26){//農業共済掛金、減価償却費、利子割引料、貸倒金、はスキップ（入力不可の為）
			continue;
		}
		else{
			m_Arith.l_add( pK->KKval4, pK->KKval4, (*m_pTaxListData)->NRec.KNval[i].KKval4 );//Dの欄（うち税率４％適用分）
			m_Arith.l_add( pK->KKval5, pK->KKval5, (*m_pTaxListData)->NRec.KNval[i].KKval5 );//Eの欄（うち税率６．３％適用分）
		}
	}


	// 経費　計(35)
	pK = &(*m_pTaxListData)->NRec.KNval[KNOULINE35];
	m_Arith.l_sub( val, (*m_pTaxListData)->NRec.KNval[KNOULINE32].KKval1, (*m_pTaxListData)->NRec.KNval[KNOULINE33].KKval1 );
	m_Arith.l_sub( val, val, (*m_pTaxListData)->NRec.KNval[KNOULINE34].KKval1 );
	m_Arith.l_add( pK->KKval1, (*m_pTaxListData)->NRec.KNval[KNOULINE31].KKval1, val );

	// 差引金額(36)
	pK = &(*m_pTaxListData)->NRec.KNval[KNOULINE36];
	m_Arith.l_sub( pK->KKval1, (*m_pTaxListData)->NRec.KNval[KNOULINE7].KKval1, (*m_pTaxListData)->NRec.KNval[KNOULINE35].KKval1 );

	return 0;
}


//-----------------------------------------------------------------------------
// 簡易課税
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_KniKeiInput()
{

	KKVAL*	pK = (*m_pTaxListData)->NRec.KUval;
	int		id;
//	char	val[6][6];
	char	val[12][6];

	::ZeroMemory( val, sizeof(val));
	for( int i=0; i<JSKLINCNT; i++ ){
		id = KINP + i;

//2016.10.20 DELETE START
//		if(i == 3){
//			continue;
//		}
//2016.10.20 DELETE END

		if(id == KINP+KLIN){//合計行はスキップ
			continue;
		}
		Shz_LinCalc( (pK+id), 0 );
		m_Arith.l_add( val[0], val[0], (pK+id)->KKval1 );
		m_Arith.l_add( val[1], val[1], (pK+id)->KKval2 );
		m_Arith.l_add( val[2], val[2], (pK+id)->KKval3 );
		m_Arith.l_add( val[3], val[3], (pK+id)->KKval4 );
		m_Arith.l_add( val[4], val[4], (pK+id)->KKval5 );
		m_Arith.l_add( val[5], val[5], (pK+id)->KKval6 );
		m_Arith.l_add( val[6], val[6], (pK+id)->KKval7 );
		m_Arith.l_add( val[7], val[7], (pK+id)->KKval8 );

	}

//2016.10.20 DELETE START
	//// 固定資産等の譲渡 [13'09.18]
	//id = KINP+8;
	//Shz_LinCalc( (pK+id), 0 );
	//m_Arith.l_add( val[0], val[0], (pK+id)->KKval1 );
	//m_Arith.l_add( val[1], val[1], (pK+id)->KKval2 );
	//m_Arith.l_add( val[2], val[2], (pK+id)->KKval3 );
	//m_Arith.l_add( val[3], val[3], (pK+id)->KKval4 );
	//m_Arith.l_add( val[4], val[4], (pK+id)->KKval5 );
	//m_Arith.l_add( val[5], val[5], (pK+id)->KKval6 );
	//m_Arith.l_add( val[6], val[6], (pK+id)->KKval7 );
	//m_Arith.l_add( val[7], val[7], (pK+id)->KKval8 );
//2016.10.20 DELETE END
	
	id = KINP+KLIN;
	memcpy( (pK+id)->KKval1, val[0], 6 );
	memcpy( (pK+id)->KKval2, val[1], 6 );
	memcpy( (pK+id)->KKval3, val[2], 6 );
	memcpy( (pK+id)->KKval4, val[3], 6 );
	memcpy( (pK+id)->KKval5, val[4], 6 );
	memcpy( (pK+id)->KKval6, val[5], 6 );
	memcpy( (pK+id)->KKval7, val[6], 6 );
	memcpy( (pK+id)->KKval8, val[7], 6 );



	/*

	KKVAL*	pK = (*m_pTaxListData)->NRec.KUval;
	int		id;
	char	val[6][6];

	::ZeroMemory( val, sizeof(val));
	for( int i=0; i<KLIN; i++ ){
		id = KINP+i;
		Shz_LinCalc( (pK+id), 0 );
		m_Arith.l_add( val[0], val[0], (pK+id)->KKval1 );
		m_Arith.l_add( val[1], val[1], (pK+id)->KKval2 );
		m_Arith.l_add( val[2], val[2], (pK+id)->KKval3 );
		m_Arith.l_add( val[3], val[3], (pK+id)->KKval4 );
	}

	// 固定資産等の譲渡 [13'09.18]
	id = KINP+8;
	Shz_LinCalc( (pK+id), 0 );
	m_Arith.l_add( val[0], val[0], (pK+id)->KKval1 );
	m_Arith.l_add( val[1], val[1], (pK+id)->KKval2 );
	m_Arith.l_add( val[2], val[2], (pK+id)->KKval3 );
	m_Arith.l_add( val[3], val[3], (pK+id)->KKval4 );

	id = KINP+KLIN;
	memcpy( (pK+id)->KKval1, val[0], 6 );
	memcpy( (pK+id)->KKval2, val[1], 6 );
	memcpy( (pK+id)->KKval3, val[2], 6 );
	memcpy( (pK+id)->KKval4, val[3], 6 );

	*/

	return 0;
}

//-----------------------------------------------------------------------------
// 課税売上高への転記（簡易課税）
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_KniKeiPost()
{

	KKVAL*	pK = (*m_pTaxListData)->NRec.KUval;
	char	val[6];

	if( (*m_pTaxListData)->GKBN & 0x01 ){	// 事業所

		if( !((pK+0)->KKauto&0x01) ){
			::ZeroMemory( val, 6 );
			m_Arith.l_add( val, (pK+40)->KKval1, (pK+41)->KKval1 );
			m_Arith.l_add( val, val, (pK+44)->KKval1 );
			//★//[13'09.18]
			m_Arith.l_add( val, val, (pK+42)->KKval1 );
			m_Arith.l_add( val, val, (pK+45)->KKval1 );
//2016.10.20 INSERT START
			m_Arith.l_add( val, val, (pK+43)->KKval1 );
//2016.10.20 INSERT END
			memcpy( (pK+0)->KKval1, val, 6 );
		}
		if( !((pK+1)->KKauto&0x01) ){
			::ZeroMemory( val, 6 );
			m_Arith.l_add( val, (pK+40)->KKval2, (pK+41)->KKval2 );
			m_Arith.l_add( val, val, (pK+44)->KKval2 );
			//★//[13'09.18]
			m_Arith.l_add( val, val, (pK+42)->KKval2 );
			m_Arith.l_add( val, val, (pK+45)->KKval2 );
//2016.10.20 INSERT START
			m_Arith.l_add( val, val, (pK+43)->KKval2 );
//2016.10.20 INSERT END
			memcpy( (pK+1)->KKval1, val, 6 );
		}

		//------->差引課税売上高　うち税率４％適用分
		if( !((pK+2)->KKauto&0x02) ){
			::ZeroMemory( val, 6 );
			m_Arith.l_add( val, (pK+40)->KKval7, (pK+41)->KKval7 );
			m_Arith.l_add( val, val, (pK+44)->KKval7 );
			m_Arith.l_add( val, val, (pK+42)->KKval7 );
			m_Arith.l_add( val, val, (pK+45)->KKval7 );
//2016.10.20 INSERT START
			m_Arith.l_add( val, val, (pK+43)->KKval7 );
//2016.10.20 INSERT END
			memcpy( (pK+2)->KKval2, val, 6 );
		}

		//差引課税売上高　うち税率６．３％適用分
		if( !((pK+2)->KKauto&0x04) ){
			::ZeroMemory( val, 6 );
			m_Arith.l_add( val, (pK+40)->KKval5, (pK+41)->KKval5 );
			m_Arith.l_add( val, val, (pK+44)->KKval5 );
			m_Arith.l_add( val, val, (pK+42)->KKval5 );
			m_Arith.l_add( val, val, (pK+45)->KKval5 );
//2016.10.20 INSERT START
			m_Arith.l_add( val, val, (pK+43)->KKval5 );
//2016.10.20 INSERT END
			memcpy( (pK+2)->KKval3, val, 6 );
		}
		//<---------------------------------------


	}

//2016.10.20 INSERT START
	else{
	//【農業事業者】
		if( !((pK+0)->KKauto&0x01) ){
			memcpy( (pK+0)->KKval1, (pK+43)->KKval1, 6 );
		}
		if( !((pK+1)->KKauto&0x01) ){
			memcpy( (pK+1)->KKval1, (pK+43)->KKval2, 6 );
		}
		if( !((pK+2)->KKauto&0x02) ){
			memcpy( (pK+2)->KKval2, (pK+43)->KKval7, 6 );
		}
		if( !((pK+2)->KKauto&0x04) ){
			memcpy( (pK+2)->KKval3, (pK+43)->KKval5, 6 );
		}
	}
//2016.10.20 INSERT END

//2016.10.20 DELETE START
//	if( (*m_pTaxListData)->GKBN & 0x02 )	{	// 不動産
//2016.10.20 DELETE END		
//		if( !((pK+3)->KKauto&0x01) ){
		if( !((pK+6)->KKauto&0x01) ){
			::ZeroMemory( val, 6 );
//			memcpy( (pK+3)->KKval1, (pK+46)->KKval1, 6 );
			memcpy( (pK+6)->KKval1, (pK+46)->KKval1, 6 );
		}
//		if( !((pK+4)->KKauto&0x01) ){
		if( !((pK+7)->KKauto&0x01) ){
			::ZeroMemory( val, 6 );
//			memcpy( (pK+4)->KKval1, (pK+46)->KKval2, 6 );
			memcpy( (pK+7)->KKval1, (pK+46)->KKval2, 6 );
		}

		//------->差引課税売上高　うち税率４％適用分
		if( !((pK+8)->KKauto&0x02) ){
			::ZeroMemory( val, 6 );
			memcpy( (pK+8)->KKval2, (pK+46)->KKval7, 6 );
		}

		//差引課税売上高　うち税率６．３％適用分
		if( !((pK+8)->KKauto&0x04) ){
			::ZeroMemory( val, 6 );
			memcpy( (pK+8)->KKval3, (pK+46)->KKval5, 6 );
		}
		//<---------------------------------------------------
//2016.10.20 DELETE START
//	}
//2016.10.20 DELETE END

	//★//[13'09.18] 固定資産等の譲渡 追加
	if( !((pK+12)->KKauto&0x01) ){
		::ZeroMemory( val, 6 );
		memcpy( (pK+12)->KKval1, (pK+48)->KKval1, 6 );
	}
	if( !((pK+13)->KKauto&0x01) ){
		::ZeroMemory( val, 6 );
		memcpy( (pK+13)->KKval1, (pK+48)->KKval2, 6 );
	}

	//>差引課税売上高　うち税率４％適用分
	if( !((pK+14)->KKauto&0x02) ){
		::ZeroMemory( val, 6 );
		memcpy( (pK+14)->KKval2, (pK+48)->KKval7, 6 );
	}

	//差引課税売上高　うち税率６．３％適用
	if( !((pK+14)->KKauto&0x04) ){
		::ZeroMemory( val, 6 );
		memcpy( (pK+14)->KKval3, (pK+48)->KKval5, 6 );
	}

	Shz_SaleKeiCalc();

	return 0;
}

//'14.10.08
//-----------------------------------------------------------------------------
// 非連動で簡易課税(経過措置用)
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_DataCpyKniKei()
{
	KKVAL*	pU = (*m_pTaxListData)->NRec.KUval;
//2017.12.04 INSERT START
	KKVAL*	pJ = (*m_pTaxListData)->NRec.KJval;
	KKVAL*	pF = (*m_pTaxListData)->NRec.KFval;
	KKVAL*	pN = (*m_pTaxListData)->NRec.KNval;
//2017.12.04 INSERT END
	char	val[6], val2[6], PW1[6], PW2[6], clc[6];

	::ZeroMemory(&tenkidata , sizeof(tenkidata));

	// 課税標準額６．３％
	::ZeroMemory( val, 6 );
	::ZeroMemory( PW1, 6 );
	m_Arith.l_input( PW1, _T("1000") );
	m_Arith.l_div( val, (pU+17)->KKval1, PW1 );
	m_Arith.l_mul( val, val, PW1 );
	memcpy(tenkidata.Sn_1F1C , val , 6);

	// 課税標準額４％
	::ZeroMemory( val, 6 );
	::ZeroMemory( PW1, 6 );
	m_Arith.l_input( PW1, _T("1000") );
	m_Arith.l_div( val, (pU+16)->KKval1, PW1 );
	m_Arith.l_mul( val, val, PW1 );
	memcpy(tenkidata.Sn_1F1B , val , 6);


	//消費税率6.3%
	::ZeroMemory( val, 6 );
	::ZeroMemory( PW1, 6 );
	::ZeroMemory( PW2, 6 );
	m_Arith.l_input( PW1, _T("63") );
	m_Arith.l_input( PW2, _T("1000") );
	m_Arith.l_mul( val, tenkidata.Sn_1F1C, PW1 );
	m_Arith.l_div( val, val, PW2 );
	memcpy(tenkidata.Sn_1F2C , val , 6);

	//消費税率4%
	::ZeroMemory( val, 6 );
	::ZeroMemory( PW1, 6 );
	::ZeroMemory( PW2, 6 );
	m_Arith.l_input( PW1, _T("4") );
	m_Arith.l_input( PW2, _T("100") );
	m_Arith.l_mul( val, tenkidata.Sn_1F1B, PW1 );
	m_Arith.l_div( val, val, PW2 );
	memcpy(tenkidata.Sn_1F2B , val , 6);


	//返還等課税取引６．３％を本表側の返還等対価に係る税額の６．３％へ
//2017.12.04 INSERT START
	if( Util.IsUnConnectMst(m_pZmSub->zvol) ){
//2017.12.04 INSERT END
	::ZeroMemory( clc, 6 );
	for( int i=0, id=40; i<5; i++ ){
		::ZeroMemory( val, 6 );
		memcpy( val, (pU+id)->KKval6, 6 );
		if( (i==2) || (i==4) ){
			m_Arith.l_add( val, val, (pU+(id+1))->KKval6 );
			id++;
		}
		if( (*m_pTaxListData)->Head.KHstax ){
			::ZeroMemory( val2, 6 );
			m_Arith.l_input( PW1, _T("8") );
			m_Arith.l_input( PW2, _T("100") );
			m_Arith.l_mul( val2,val, PW1 );
			m_Arith.l_div( val2, val2, PW2 );
			m_Arith.l_add( val, val, val2 );
		}
		::ZeroMemory( val2, 6 );
		Shz_shohizei63( val2, val );
		id++;
		m_Arith.l_add( clc, clc, val2 );
	}
	memcpy(tenkidata.Sn_1F5C , clc , 6);
//2017.12.04 INSERT START
	}else{
		::ZeroMemory( val, 6 );
		memcpy( val, (pJ+33)->KKval5, 6 );
		m_Arith.l_add( val, val, (pF+15)->KKval5 );
		m_Arith.l_add( val, val, (pN+38)->KKval5 );
		if((*m_pTaxListData)->Head.KHstax ){
			::ZeroMemory( val2, 6 );

			
			m_Arith.l_input( PW1, _T("8") );
			m_Arith.l_input( PW2, _T("100") );
			m_Arith.l_mul( val2,val, PW1 );
			m_Arith.l_div( val2, val2, PW2 );
			
			m_Arith.l_add( val, val, val2 );
		}
		::ZeroMemory( val2, 6 );
		Shz_shohizei63( val2, val );
		memcpy(tenkidata.Sn_1F5C , val2 , 6);
	}
//2017.12.04 INSERT END


	//返還等課税取引４％を本表側の返還等対価に係る税額の４％へ
	::ZeroMemory( clc, 6 );
//2016.10.20 INSERT START
	if( m_SixKindFlg == TRUE ){

//2017.12.04 INSERT START
		if( Util.IsUnConnectMst(m_pZmSub->zvol) ){
//2017.12.04 INSERT END
		for( int i=0, id=40; i<6; i++ ){
			::ZeroMemory( val, 6 );
			memcpy( val, (pU+id)->KKval8, 6 );
			if( i==2 ){
				m_Arith.l_add( val, val, (pU+(id+1))->KKval8 );
				id++;
			}
			if( (*m_pTaxListData)->Head.KHstax ){
				::ZeroMemory( val2, 6 );

				m_Arith.l_input( PW1, _T("5") );
				m_Arith.l_input( PW2, _T("100") );
				m_Arith.l_mul( val2,val, PW1 );
				m_Arith.l_div( val2, val2, PW2 );
				m_Arith.l_add( val, val, val2 );
			}
			::ZeroMemory( val2, 6 );
			Shz_shohizei4( val2, val );
			id++;
			m_Arith.l_add( clc, clc, val2 );
		}
		memcpy(tenkidata.Sn_1F5B , clc , 6);
//2017.12.04 INSERT START
		}else{
			::ZeroMemory( val, 6 );
			memcpy( val, (pJ+33)->KKval4, 6 );
			m_Arith.l_add( val, val, (pF+15)->KKval4 );
			m_Arith.l_add( val, val, (pN+38)->KKval4 );
			if( (*m_pTaxListData)->Head.KHstax ){
				::ZeroMemory( val2, 6 );

				
				m_Arith.l_input( PW1, _T("5") );
				m_Arith.l_input( PW2, _T("100") );
				m_Arith.l_mul( val2,val, PW1 );
				m_Arith.l_div( val2, val2, PW2 );
				
				m_Arith.l_add( val, val, val2 );
			}
			::ZeroMemory( val2, 6 );
			Shz_shohizei4( val2, val );
			memcpy(tenkidata.Sn_1F5B , val2 , 6);
		}
//2017.12.04 INSERT END


		// 第１種～第５種までの金額を付表５－２に転記　６．３％分
		::ZeroMemory( clc, 6 );
		for( int i=0, id=40; i<6; i++ ){
			::ZeroMemory( val, 6 );
			memcpy( val, (pU+id)->KKval5, 6 );
			if( i==2 ){
				m_Arith.l_add( val, val, (pU+(id+1))->KKval5 );
			}
			else if( i == 3 ){		// 10/04/06 追加
				m_Arith.l_add( val, val, (pU+48)->KKval5 );
			}
			if( !(*m_pTaxListData)->Head.KHstax ){
				Shz_shohizei100_108( val, val );
			}

			::ZeroMemory( val2, 6 );
			memcpy( val2, (pU+id)->KKval6, 6 );
			if( i==2 ){
				m_Arith.l_add( val2, val2, (pU+(id+1))->KKval6 );
				id++;
			}
			if( !(*m_pTaxListData)->Head.KHstax ){
				Shz_shohizei100_108( val2, val2 );
			}
			id++;
			m_Arith.l_sub( val, val, val2 );

			//マイナス値の場合には０にして転記する
			if( m_Arith.l_test(val) < 0 ){
				::ZeroMemory(val , sizeof(val));
			}

			switch( i ){
				case 0:		
					memcpy(tenkidata.Sn_5FURI1C , val , 6);
					break;
				case 1:		
					memcpy(tenkidata.Sn_5FURI2C , val , 6);
					break;
				
				case 2:		
					memcpy(tenkidata.Sn_5FURI3C , val , 6);
					break;

				case 3:		
					memcpy(tenkidata.Sn_5FURI4C , val , 6);
					break;

				case 4:		
					memcpy(tenkidata.Sn_5FURI5C , val , 6);
					break;

				case 5:		
					memcpy(tenkidata.Sn_5FURI6C , val , 6);
					break;
			}
			m_Arith.l_add( clc, clc, val );
		}

		// 第１種～第５種までの金額を付表５－２に転記　４％分
		for( int i=0, id=40; i<6; i++ ){
			::ZeroMemory( val, 6 );
			memcpy( val, (pU+id)->KKval7, 6 );
			if( i==2 ){
				m_Arith.l_add( val, val, (pU+(id+1))->KKval7 );
			}
			else if( i == 3 ){		// 10/04/06 追加
				m_Arith.l_add( val, val, (pU+48)->KKval7 );
			}
			if( !(*m_pTaxListData)->Head.KHstax ){
				Shz_urcalc( val, val );
			}

			::ZeroMemory( val2, 6 );
			memcpy( val2, (pU+id)->KKval8, 6 );
			if( i==2 ){
				m_Arith.l_add( val2, val2, (pU+(id+1))->KKval8 );
				id++;
			}
			if( !(*m_pTaxListData)->Head.KHstax ){
				Shz_urcalc( val2, val2 );
			}
			id++;
			m_Arith.l_sub( val, val, val2 );

			//マイナス値の場合には０にして転記する
			if( m_Arith.l_test(val) < 0 ){
				::ZeroMemory(val , sizeof(val));
			}

			switch( i ){
				case 0:
					memcpy(tenkidata.Sn_5FURI1B , val , 6);
					break;
				case 1:		
					memcpy(tenkidata.Sn_5FURI2B , val , 6);
					break;
				
				case 2:		
					memcpy(tenkidata.Sn_5FURI3B , val , 6);
					break;

				case 3:		
					memcpy(tenkidata.Sn_5FURI4B , val , 6);
					break;

				case 4:		
					memcpy(tenkidata.Sn_5FURI5B , val , 6);
					break;

				case 5:		
					memcpy(tenkidata.Sn_5FURI6B , val , 6);
					break;
			}
			m_Arith.l_add( clc, clc, val );
		}
	}else{
//2016.10.20 INSERT END
//2017.12.04 INSERT START
	if( Util.IsUnConnectMst(m_pZmSub->zvol) ){
//2017.12.04 INSERT END

	for( int i=0, id=40; i<5; i++ ){
		::ZeroMemory( val, 6 );
		memcpy( val, (pU+id)->KKval8, 6 );
		if( (i==2) || (i==4) ){
			m_Arith.l_add( val, val, (pU+(id+1))->KKval8 );
			id++;
		}
		if( (*m_pTaxListData)->Head.KHstax ){
			::ZeroMemory( val2, 6 );

			m_Arith.l_input( PW1, _T("5") );
			m_Arith.l_input( PW2, _T("100") );
			m_Arith.l_mul( val2,val, PW1 );
			m_Arith.l_div( val2, val2, PW2 );
			m_Arith.l_add( val, val, val2 );
		}
		::ZeroMemory( val2, 6 );
		Shz_shohizei4( val2, val );
		id++;
		m_Arith.l_add( clc, clc, val2 );
	}
	memcpy(tenkidata.Sn_1F5B , clc , 6);

//2017.12.04 INSERT START
	}else{
		::ZeroMemory( val, 6 );
		memcpy( val, (pJ+33)->KKval4, 6 );
		m_Arith.l_add( val, val, (pF+15)->KKval4 );
		m_Arith.l_add( val, val, (pN+38)->KKval4 );
		if( (*m_pTaxListData)->Head.KHstax ){
			::ZeroMemory( val2, 6 );

			
			m_Arith.l_input( PW1, _T("5") );
			m_Arith.l_input( PW2, _T("100") );
			m_Arith.l_mul( val2,val, PW1 );
			m_Arith.l_div( val2, val2, PW2 );
			
			m_Arith.l_add( val, val, val2 );
		}
		::ZeroMemory( val2, 6 );
		Shz_shohizei4( val2, val );
		memcpy(tenkidata.Sn_1F5B , val2 , 6);
	}
//2017.12.04 INSERT END


	// 本表の課税標準額（これは付表から金額取るようになっているので処理カット）
//	::ZeroMemory( val, 6 );
//	memcpy( val, (pU+47)->KKval3, 6 );
//	if( !(*m_pTaxListData)->Head.KHstax ){
//		Shz_urcalc( val, val );
//	}
//	::ZeroMemory( val2, 6 );
//	memcpy( val2, (pU+47)->KKval4, 6 );
//	if( !(*m_pTaxListData)->Head.KHstax ){
//		Shz_urcalc( val2, val2 );
//	}
//	m_Arith.l_sub( val, val, val2 );
//	memcpy(tenkidata.Sn_KZURI , clc , 6);

	// 第１種～第５種までの金額を付表５－２に転記　６．３％分
	::ZeroMemory( clc, 6 );
	for( int i=0, id=40; i<5; i++ ){
		::ZeroMemory( val, 6 );
		memcpy( val, (pU+id)->KKval5, 6 );
		if( (i==2) || (i==4) ){
			m_Arith.l_add( val, val, (pU+(id+1))->KKval5 );
		}
		else if( i == 3 ){		// 10/04/06 追加
			m_Arith.l_add( val, val, (pU+48)->KKval5 );
		}
		if( !(*m_pTaxListData)->Head.KHstax ){
//			Shz_urcalc( val, val );
			Shz_shohizei100_108( val, val );
		}

		::ZeroMemory( val2, 6 );
		memcpy( val2, (pU+id)->KKval6, 6 );
		if( (i==2) || (i==4) ){
			m_Arith.l_add( val2, val2, (pU+(id+1))->KKval6 );
			id++;
		}
		if( !(*m_pTaxListData)->Head.KHstax ){
//			Shz_urcalc( val2, val2 );
			Shz_shohizei100_108( val2, val2 );
		}
		id++;
		m_Arith.l_sub( val, val, val2 );

		//---->'14.11.06
		//マイナス値の場合には０にして転記する
		if( m_Arith.l_test(val) < 0 ){
			::ZeroMemory(val , sizeof(val));
		}
		//<-------------

		switch( i ){
			case 0:		
				memcpy(tenkidata.Sn_5FURI1C , val , 6);
				break;
			case 1:		
				memcpy(tenkidata.Sn_5FURI2C , val , 6);
				break;
			
			case 2:		
				memcpy(tenkidata.Sn_5FURI3C , val , 6);
				break;

			case 3:		
				memcpy(tenkidata.Sn_5FURI4C , val , 6);
				break;

			case 4:		
				memcpy(tenkidata.Sn_5FURI5C , val , 6);
				break;
		}
		m_Arith.l_add( clc, clc, val );
	}

	// 第１種～第５種までの金額を付表５－２に転記　４％分
//	::ZeroMemory( clc, 6 );
	for( int i=0, id=40; i<5; i++ ){
		::ZeroMemory( val, 6 );
		memcpy( val, (pU+id)->KKval7, 6 );
		if( (i==2) || (i==4) ){
			m_Arith.l_add( val, val, (pU+(id+1))->KKval7 );
		}
		else if( i == 3 ){		// 10/04/06 追加
			m_Arith.l_add( val, val, (pU+48)->KKval7 );
		}
		if( !(*m_pTaxListData)->Head.KHstax ){
			Shz_urcalc( val, val );
		}

		::ZeroMemory( val2, 6 );
		memcpy( val2, (pU+id)->KKval8, 6 );
		if( (i==2) || (i==4) ){
			m_Arith.l_add( val2, val2, (pU+(id+1))->KKval8 );
			id++;
		}
		if( !(*m_pTaxListData)->Head.KHstax ){
			Shz_urcalc( val2, val2 );
		}
		id++;
		m_Arith.l_sub( val, val, val2 );

		//---->'14.11.06
		//マイナス値の場合には０にして転記する
		if( m_Arith.l_test(val) < 0 ){
			::ZeroMemory(val , sizeof(val));
		}
		//<-------------

		switch( i ){
			case 0:
				memcpy(tenkidata.Sn_5FURI1B , val , 6);
				break;
			case 1:		
				memcpy(tenkidata.Sn_5FURI2B , val , 6);
				break;
			
			case 2:		
				memcpy(tenkidata.Sn_5FURI3B , val , 6);
				break;

			case 3:		
				memcpy(tenkidata.Sn_5FURI4B , val , 6);
				break;

			case 4:		
				memcpy(tenkidata.Sn_5FURI5B , val , 6);
				break;
		}
		m_Arith.l_add( clc, clc, val );
	}
//2016.10.20 INSERT START
	}
//2016.10.20 INSERT END

//	// 付表5
	memcpy(tenkidata.Sn_5FURITA , clc , 6);

	::ZeroMemory( tenkidata.Sn_5FUZTT, 6 );
	::ZeroMemory( tenkidata.Sn_5FUZT1, 6 );
	::ZeroMemory( tenkidata.Sn_5FUZT2, 6 );
	::ZeroMemory( tenkidata.Sn_5FUZT3, 6 );
	::ZeroMemory( tenkidata.Sn_5FUZT4, 6 );
	::ZeroMemory( tenkidata.Sn_5FUZT5, 6 );
	::ZeroMemory( tenkidata.Sn_5FUZT6, 6 );	// '16.12.27

	return 0;
}


//-----------------------------------------------------------------------------
// 非連動で通常課税(経過措置用)
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzCalc::Shz_DataCpyUsuKei()
{

	::ZeroMemory(&tenkidata , sizeof(tenkidata));

	KKVAL*	pJ = (*m_pTaxListData)->NRec.KJval;
	KKVAL*	pF = (*m_pTaxListData)->NRec.KFval;
	KKVAL*	pN = (*m_pTaxListData)->NRec.KNval;

	KKVAL*	pU = (*m_pTaxListData)->NRec.KUval;
	KKVAL*	pS = (*m_pTaxListData)->NRec.KSval;
	char	val[6], val2[6], PW1[6], PW2[6];

	// 第27表-(1)号様式
	// 課税標準額６．３％
	::ZeroMemory( val, 6 );
	::ZeroMemory( PW1, 6 );
	m_Arith.l_input( PW1, _T("1000") );
	m_Arith.l_div( val, (pU+17)->KKval1, PW1 );
	m_Arith.l_mul( val, val, PW1 );
	memcpy(tenkidata.Sn_1F1C , val , 6);

	// 課税標準額４％
	::ZeroMemory( val, 6 );
	::ZeroMemory( PW1, 6 );
	m_Arith.l_input( PW1, _T("1000") );
	m_Arith.l_div( val, (pU+16)->KKval1, PW1 );
	m_Arith.l_mul( val, val, PW1 );
	memcpy(tenkidata.Sn_1F1B , val , 6);

	//消費税率6.3%
	::ZeroMemory( val, 6 );
	::ZeroMemory( PW1, 6 );
	::ZeroMemory( PW2, 6 );
	m_Arith.l_input( PW1, _T("63") );
	m_Arith.l_input( PW2, _T("1000") );
	m_Arith.l_mul( val,tenkidata.Sn_1F1C, PW1 );
	m_Arith.l_div( val, val, PW2 );
	memcpy(tenkidata.Sn_1F2C , val , 6);

	//消費税率4%
	::ZeroMemory( val, 6 );
	::ZeroMemory( PW1, 6 );
	::ZeroMemory( PW2, 6 );
	m_Arith.l_input( PW1, _T("4") );
	m_Arith.l_input( PW2, _T("100") );
	m_Arith.l_mul( val, tenkidata.Sn_1F1B, PW1 );
	m_Arith.l_div( val, val, PW2 );
	memcpy(tenkidata.Sn_1F2B , val , 6);


	//返還等課税取引６．３％を本表側の返還等対価に係る税額の６．３％へ
	::ZeroMemory( val, 6 );
	memcpy( val, (pJ+33)->KKval5, 6 );
	m_Arith.l_add( val, val, (pF+15)->KKval5 );
	m_Arith.l_add( val, val, (pN+38)->KKval5 );
	if((*m_pTaxListData)->Head.KHstax ){
		::ZeroMemory( val2, 6 );

		
		m_Arith.l_input( PW1, _T("8") );
		m_Arith.l_input( PW2, _T("100") );
		m_Arith.l_mul( val2,val, PW1 );
		m_Arith.l_div( val2, val2, PW2 );
		
		m_Arith.l_add( val, val, val2 );
	}
	::ZeroMemory( val2, 6 );
//	Shz_sicalc( val2, val );
	Shz_shohizei63( val2, val );
	memcpy(tenkidata.Sn_1F5C , val2 , 6);//'15.01.09
//	memcpy(tenkidata.Sn_1F5C , val , 6);


	//返還等課税取引４％を本表側の返還等対価に係る税額の４％へ
	//------->'15.01.09
	::ZeroMemory( val, 6 );
	memcpy( val, (pJ+33)->KKval4, 6 );
	m_Arith.l_add( val, val, (pF+15)->KKval4 );
	m_Arith.l_add( val, val, (pN+38)->KKval4 );
	if( (*m_pTaxListData)->Head.KHstax ){
		::ZeroMemory( val2, 6 );

		
		m_Arith.l_input( PW1, _T("5") );
		m_Arith.l_input( PW2, _T("100") );
		m_Arith.l_mul( val2,val, PW1 );
		m_Arith.l_div( val2, val2, PW2 );
		
		m_Arith.l_add( val, val, val2 );
	}
	::ZeroMemory( val2, 6 );
//	Shz_sicalc( val2, val );
	Shz_shohizei4( val2, val );
	memcpy(tenkidata.Sn_1F5B , val2 , 6);
	//<---------------------------

/*	::ZeroMemory( val, 6 );
	for( int i=0, id=40; i<5; i++ ){
		::ZeroMemory( val, 6 );
		memcpy( val, (pU+id)->KKval4, 6 );
		if( (i==2) || (i==4) ){
			m_Arith.l_add( val, val, (pU+(id+1))->KKval4 );
			id++;
		}
		if( (*m_pTaxListData)->Head.KHstax ){
			::ZeroMemory( val2, 6 );

			m_Arith.l_input( PW1, _T("5") );
			m_Arith.l_input( PW2, _T("100") );
			m_Arith.l_mul( val2,val, PW1 );
			m_Arith.l_div( val2, val2, PW2 );
			m_Arith.l_add( val, val, val2 );
		}
		::ZeroMemory( val2, 6 );
		Shz_shohizei4( val2, val );
		id++;
		m_Arith.l_add( val, val, val2 );
	}
	memcpy(tenkidata.Sn_1F5B , val , 6);
*/

	// 付表2
	// ①
	::ZeroMemory( val, 6 );
	memcpy( val, (pJ+33)->KKval5, 6 );
	m_Arith.l_add( val, val, (pF+15)->KKval5 );
	m_Arith.l_add( val, val, (pN+38)->KKval5 );
	if( !(*m_pTaxListData)->Head.KHstax ){
		Shz_shohizei100_108( val, val );//'15.01.09
//		Shz_shohizei63( val, val );
	}
	
	m_Arith.l_sub( tenkidata.Sn_2F1C, (pU+17)->KKval1, val );

	::ZeroMemory( val, 6 );
	memcpy( val, (pJ+33)->KKval4, 6 );
	m_Arith.l_add( val, val, (pF+15)->KKval4 );
	m_Arith.l_add( val, val, (pN+38)->KKval4 );
	if( !(*m_pTaxListData)->Head.KHstax ){
		Shz_shohizei100_105( val, val );//'15.01.09
//		Shz_shohizei4( val, val );
	}
	
	m_Arith.l_sub( tenkidata.Sn_2F1B, (pU+16)->KKval1, val );


	memcpy(tenkidata.Sn_2F8C , (pS+17)->KKval2 , 6);
	memcpy(tenkidata.Sn_2F8B , (pS+16)->KKval2 , 6);

	memcpy(tenkidata.Sn_2F9C , (pS+17)->KKval1 , 6);
	memcpy(tenkidata.Sn_2F9B , (pS+16)->KKval1 , 6);


	return 0;

}
