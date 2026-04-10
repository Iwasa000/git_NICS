// shzcalc.cpp
// 計算表集計クラス
#include "stdafx.h"
//#include "DBSyzShin.h"

//#include "MainFrm.h"
//#include "DBSyzShinDoc.h"
//#include "DBSyzShinView.h"
#include "ShzTax31Calc.h"


//---------->'14.08.20
#include "ShinBuy31Idx.h"
#include "ShinSell31Idx.h"
#include "ShinJigyo31Idx.h"
// kasai add -->
#include "ShinBuy31ExIdx.h"
#include "ShinSell31ExIdx.h"
#include "ShinJigyo31ExIdx.h"
// kasai add <--
// tanaka add -->
#include "ShinBuy31Ex2Idx.h"
#include "ShinSell31Ex2Idx.h"
// tanaka add <--
#include "ShinFudousan31Idx.h"
#include "ShinNougyou31Idx.h"
// 211129_h add -->
#include "ShinFudousan31ExIdx.h"
#include "ShinNougyou31ExIdx.h"
// 211129_h add <--
#include "ShinKniinp31Idx.h"//清水メモ
// 211201_h add -->
#include "ShinKniinp31ExIdx.h"
// 211201_h add <--
//<------------------

// 2023/10/05 higuchi 依頼No.163608
#include <l_dotnet.h>

// メインビューの財務クラスを参照
//CDBSyzShinView *CShzTax31Calc::pSyzShin;

CShzTax31Calc::CShzTax31Calc()
	: m_pTaxListData(NULL)
	, m_pSnHeadData(NULL)
	, m_pZmSub(NULL)
	, m_Arith(0x16)
//2016.10.20 INSERT START
	, m_SixKindFlg(TRUE)
//2016.10.20 INSERT END
{
}

CShzTax31Calc::~CShzTax31Calc()
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
int CShzTax31Calc::InitShzCalc( CH31TaxListData **pTaxListData, CSnHeadData **pSnHeadData, CDBNpSub *pZmSub )
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
// 横の計算
//-----------------------------------------------------------------------------
// 引数	pK		：
//		mode	：
// 22-0167,22-0361 add -->
//		mode2	：1,2=事業別売上高入力からの呼び出し(2=農業事業者)
// 22-0167,22-0361 add <--
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
// 22-0167,22-0361 cor -->
//int	CShzTax31Calc::Shz_LinCalc( KKVAL* pK, int mode )
// -----------------------
int	CShzTax31Calc::Shz_LinCalc( KKVAL* pK, int mode, int mode2 )
// 22-0167,22-0361 cor <--
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
// 22-0167,22-0361 add -->
	// うち軽減税6.24%適用分またはうち標準税率7.8%適用分を自動計算（新様式のみ）
	long	eymd=0;
	if(!((*m_pSnHeadData)->Sn_SKKBN % 2))	eymd = (*m_pSnHeadData)->Sn_MDAYE;
	else									eymd = (*m_pSnHeadData)->Sn_KDAYE;
// インボイス_23/07/24_higuchi cor -->
	//if((*m_pSnHeadData)->SVmzsw != 1 && eymd >= TAX_R04_SDAY)	{
// -----------------------------------
	// R05.10以降は自動計算機能をカットしている
//	if( (mode2 == 1 && (*m_pSnHeadData)->SVmzsw != 1 ) ||
//		((*m_pSnHeadData)->SVmzsw != 1 && eymd >= TAX_R04_SDAY && eymd < TAX_R05_EDAY) )	{
	//課税売上高のみ自動計算あり
	if ((mode2 == 1 && (*m_pSnHeadData)->SVmzsw != 1) ||
//----->('23.10.05 tanaka cor)
//		((*m_pSnHeadData)->SVmzsw != 1 && eymd >= TAX_R04_SDAY )) {
//------
	//	((*m_pSnHeadData)->SVmzsw != 1 && eymd >= TAX_R04_SDAY && eymd < TAX_R05_EDAY)) {
		//　23.10.10課税取引金額計算表の制御を12/31→10/01切り替えに
		((*m_pSnHeadData)->SVmzsw != 1 && eymd >= TAX_R04_SDAY && eymd < ICS_SH_INVOICE_DAY)) {
//<-----

// インボイス_23/07/24_higuchi cor <--
		if(mode == 0 || mode == 1 || mode == 3)	{
			// 事業別売上高入力（消費税申告書ｄｂ非連動＋簡易課税）
			if(mode2 == 1)	{
				//if(pK->KKauto & 0x40)	{
				//	m_Arith.l_sub(pK->KKval7,pK->KKval3,pK->KKval5);
				//	// 返還等課税取引
				//	m_Arith.l_sub(pK->KKval8,pK->KKval4,pK->KKval6);
				//}
				//else if(pK->KKauto & 0x80)	{
				if((*m_pSnHeadData)->Sn_GenericSgn & 0x800)	{
					m_Arith.l_sub(pK->KKval5,pK->KKval3,pK->KKval7);
					// 返還等課税取引
					m_Arith.l_sub(pK->KKval6,pK->KKval4,pK->KKval8);
				}
			}
			else if(mode2 == 0)	{
				//// うち軽減税率6.24%適用分(D) = 課税取引金額(C) - うち標準税率7.8%適用分(D)
				//if(pK->KKauto & 0x40)	{
				//	m_Arith.l_sub(pK->KKval5,pK->KKval3,pK->KKval6);
				//}
				// うち標準税率7.8%適用分(D)  = 課税取引金額(C) - うち軽減税率6.24%適用分(D)
				//else if(pK->KKauto & 0x80)	{
				if((*m_pSnHeadData)->Sn_GenericSgn & 0x800)	{
					m_Arith.l_sub(pK->KKval6,pK->KKval3,pK->KKval5);
				}
			}
		}
	}
// 22-0167,22-0361 add <--
	return 0;
}


#define	JVAL	34

//-----------------------------------------------------------------------------
// 事業所得
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int	CShzTax31Calc::Shz_JigyoCalc()
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
int	CShzTax31Calc::Shz_FudohCalc()
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
int	CShzTax31Calc::Shz_NohgyoCalc()
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
int	CShzTax31Calc::Shz_SalePost()
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
int	CShzTax31Calc::Shz_BuyPost()
{

//////
//	if( pSyzShin->KniFlg&0x10 ) return 0;	//★//[13'09.18];
//////

	KKVAL*	pK = (*m_pTaxListData)->NRec.KSval;
	if( (*m_pTaxListData)->GKBN & 0x01 ){	// 事業所
		if( !((pK+0)->KKauto&0x01) ){
			l_add( (pK+0)->KKval1, (*m_pTaxListData)->NRec.KJval[2].KKval1, (*m_pTaxListData)->NRec.KJval[31].KKval1 );
		}
		if( !((pK+1)->KKauto&0x01) ){
			l_add( (pK+1)->KKval1, (*m_pTaxListData)->NRec.KJval[2].KKval2, (*m_pTaxListData)->NRec.KJval[31].KKval2 );
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
int	CShzTax31Calc::Shz_SaleCalc()
{
	KKVAL*	pK = (*m_pTaxListData)->NRec.KUval;

	//4-5
	m_Arith.l_sub( (pK+5)->KKval1, (pK+3)->KKval1, (pK+4)->KKval1 );
	//7-8
	m_Arith.l_sub( (pK+8)->KKval1, (pK+6)->KKval1, (pK+7)->KKval1 );

	//1+2+3+6+9
	::ZeroMemory( (pK+9)->KKval1, 6 );
	m_Arith.l_add( (pK+9)->KKval1, (pK+0)->KKval1, (pK+1)->KKval1 );
	m_Arith.l_add( (pK+9)->KKval1, (pK+9)->KKval1, (pK+2)->KKval1 );
	m_Arith.l_add( (pK+9)->KKval1, (pK+9)->KKval1, (pK+5)->KKval1 );
	m_Arith.l_add( (pK+9)->KKval1, (pK+9)->KKval1, (pK+8)->KKval1 );

	char	val[6]={0};

	// 仮受消費税
	if( (*m_pTaxListData)->Head.KHstax ){
		if( !((pK+10)->KKauto&0x04) ){
			::ZeroMemory( val, 6 );
			Shz_shohizei8_100( val, (pK+9)->KKval1 );
			memcpy( (pK+10)->KKval3, val, 6 );
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
int	CShzTax31Calc::Shz_BuyCalc()
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

// インボイス_23/07/24_higuchi del -->
// 未使用だったので削除
////-----------------------------------------------------------------------------
//// 簡易課税
////-----------------------------------------------------------------------------
//// 返送値	
////-----------------------------------------------------------------------------
//int	CShzTax31Calc::Shz_KniInput()
//{
//	/*KKVAL*	pK = (*m_pTaxListData)->NRec.KUval;
//	int		id;
//	char	val[6][6];
//
//	::ZeroMemory( val, sizeof(val));
//	for( int i=0; i<KLIN; i++ ){
//		id = KINP+i;
//		Shz_LinCalc( (pK+id), 0 );
//		m_Arith.l_add( val[0], val[0], (pK+id)->KKval1 );
//		m_Arith.l_add( val[1], val[1], (pK+id)->KKval2 );
//		m_Arith.l_add( val[2], val[2], (pK+id)->KKval3 );
//		m_Arith.l_add( val[3], val[3], (pK+id)->KKval4 );
//	}
//
//	// 固定資産等の譲渡 [13'09.18]
//	id = KINP+8;
//	Shz_LinCalc( (pK+id), 0 );
//	m_Arith.l_add( val[0], val[0], (pK+id)->KKval1 );
//	m_Arith.l_add( val[1], val[1], (pK+id)->KKval2 );
//	m_Arith.l_add( val[2], val[2], (pK+id)->KKval3 );
//	m_Arith.l_add( val[3], val[3], (pK+id)->KKval4 );
//
//	// 第２種農業
//	id = KINP + 9;
//	Shz_LinCalc((pK + id), 0);
//	m_Arith.l_add(val[0], val[0], (pK + id)->KKval1);
//	m_Arith.l_add(val[1], val[1], (pK + id)->KKval2);
//	m_Arith.l_add(val[2], val[2], (pK + id)->KKval3);
//	m_Arith.l_add(val[3], val[3], (pK + id)->KKval4);
//
//
//	id = KINP+KLIN;
//	memcpy( (pK+id)->KKval1, val[0], 6 );
//	memcpy( (pK+id)->KKval2, val[1], 6 );
//	memcpy( (pK+id)->KKval3, val[2], 6 );
//	memcpy( (pK+id)->KKval4, val[3], 6 );*/
//
//
//	KKVAL* pK = ( *m_pTaxListData )->NRec.KUval;
//	int id = 0;
//	char val[12][6];
//
//	::ZeroMemory ( val, sizeof ( val ) );
//	for ( int i = 0; i < JSKLINCNT; i++ ) {
//		id = KINP + i;
//
//
//		if ( id == KINP + KLIN ) {
//			continue;
//		}
//
//		Shz_LinCalc ( ( pK + id ), 0 );
//		m_Arith.l_add ( val[0], val[0], ( pK + id )->KKval1 );
//		m_Arith.l_add ( val[1], val[1], ( pK + id )->KKval2 );
//		m_Arith.l_add ( val[2], val[2], ( pK + id )->KKval3 );
//		m_Arith.l_add ( val[3], val[3], ( pK + id )->KKval4 );
//		m_Arith.l_add ( val[4], val[4], ( pK + id )->KKval5 );
//		m_Arith.l_add ( val[5], val[5], ( pK + id )->KKval6 );
//		m_Arith.l_add ( val[6], val[6], ( pK + id )->KKval7 );
//		m_Arith.l_add ( val[7], val[7], ( pK + id )->KKval8 );
//
//	}
//
//	id = KINP+KLIN;
//	memcpy( ( pK + id )->KKval1, val[0], 6 );
//	memcpy( ( pK + id )->KKval2, val[1], 6 );
//	memcpy( ( pK + id )->KKval3, val[2], 6 );
//	memcpy( ( pK + id )->KKval4, val[3], 6 );
//	memcpy( ( pK + id )->KKval5, val[4], 6 );
//	memcpy( ( pK + id )->KKval6, val[5], 6 );
//	memcpy( ( pK + id )->KKval7, val[6], 6 );
//	memcpy( ( pK + id )->KKval8, val[7], 6 );
//
//	return 0;
//}
// インボイス_23/07/24_higuchi del <--

//-----------------------------------------------------------------------------
// 課税売上高への転記（簡易課税）
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int	CShzTax31Calc::Shz_KniPost()
{
	KKVAL* pK = ( *m_pTaxListData )->NRec.KUval;
	char val[MONY_BUF_SIZE] = { 0 };

	if ( ( *m_pTaxListData )->GKBN & 0x01 ) {	// 事業所

		if ( !( ( pK + 0 )->KKauto & 0x01 ) ) {
			::ZeroMemory ( val, 6 );
			m_Arith.l_sub ( val, ( pK + 47 )->KKval3, ( pK + 43 )->KKval3 );	// 合計 - 農業 - 固定資産　－ 不動産
			m_Arith.l_sub ( val, val, ( pK + 48 )->KKval3 );
			m_Arith.l_sub ( val, val, ( pK + 46 )->KKval3 );
			m_Arith.l_sub ( val, val, ( pK + 49 )->KKval3 );//追加農業
			memcpy ( ( pK + 0 )->KKval1, val, 6 );
		}

		// 差引課税売上高　うち税率７．８％適用分
		if ( !( ( pK + 0 )->KKauto & 0x08 ) ) {
			::ZeroMemory ( val, 6 );
			m_Arith.l_sub ( val, ( pK + 47 )->KKval5, ( pK + 43 )->KKval5 );
			m_Arith.l_sub ( val, val, ( pK + 48 )->KKval5 );
			m_Arith.l_sub ( val, val, ( pK + 46 )->KKval5 );
			m_Arith.l_sub ( val, val, ( pK + 49 )->KKval5);
			memcpy ( ( pK + 0 )->KKval4, val, 6 );
		}
		// 差引課税売上高　うち税率６．２４％適用分
		if ( !( ( pK + 0 )->KKauto & 0x04 ) ) {
			::ZeroMemory ( val, 6 );
			m_Arith.l_sub ( val, ( pK + 47 )->KKval7, ( pK + 43 )->KKval7 );
			m_Arith.l_sub ( val, val, ( pK + 48 )->KKval7 );
			m_Arith.l_sub ( val, val, ( pK + 46 )->KKval7 );
			m_Arith.l_sub ( val, val, ( pK + 49 )->KKval7 );
			memcpy ( ( pK + 0 )->KKval3, val, 6 );
		}
	}


	if ( !( ( pK + 1 )->KKauto & 0x01 ) ) {
		::ZeroMemory ( val, 6 );
		m_Arith.l_add ( val, ( pK + 43 )->KKval3, ( pK + 49 )->KKval3 );
		memcpy ( ( pK + 1 )->KKval1, val, 6 );


	}
	if ( !( ( pK + 1 )->KKauto & 0x08 ) ) {	// 7.8%
		::ZeroMemory ( val, 6 );
		m_Arith.l_add ( val, ( pK + 43 )->KKval5, ( pK + 49 )->KKval5 );
		memcpy ( ( pK + 1 )->KKval4, val, 6 );

	}

	if ( !( ( pK + 1 )->KKauto & 0x04 ) ) {	// 6.24%
		::ZeroMemory ( val, 6 );
		m_Arith.l_add ( val, ( pK + 43 )->KKval7, ( pK + 49 )->KKval7 );
		memcpy ( ( pK + 1 )->KKval3, val, 6 );

	}
	
	// 第六種
	if ( !( ( pK + 2 )->KKauto & 0x01 ) ) {
		::ZeroMemory ( val, 6 );
		memcpy ( ( pK + 2 )->KKval1, ( pK + 46 )->KKval3, 6 );
	}
	if ( !( ( pK + 2 )->KKauto & 0x08 ) ) {
		::ZeroMemory ( val, 6 );
		memcpy ( ( pK + 2 )->KKval4, ( pK + 46 )->KKval5, 6 );
	}
	if ( !( ( pK + 2 )->KKauto & 0x04 ) ) {
		::ZeroMemory ( val, 6 );
		memcpy ( ( pK + 2 )->KKval3, ( pK + 46 )->KKval7, 6 );
	}

	// 固定資産等の譲渡
	if ( !( ( pK + 6 )->KKauto & 0x01 ) ) {
		::ZeroMemory ( val, 6 );
		// 決算額を取得
		memcpy ( ( pK + 6 )->KKval1, ( pK + 48 )->KKval1, 6 );
	}
	// 課税取引外も取得
	if ( !( ( pK + 7 )->KKauto & 0x01 ) ) {
		::ZeroMemory ( val, 6 );
		memcpy ( ( pK + 7 )->KKval1, (pK + 48 )->KKval2, 6 );
	}
	if ( !( ( pK + 6 )->KKauto & 0x08 ) ) {
		::ZeroMemory ( val, 6 );
		memcpy ( ( pK + 6 )->KKval4, ( pK + 48 )->KKval5, 6 );
	}
	if ( !( ( pK + 6 )->KKauto & 0x04 ) ) {
		::ZeroMemory ( val, 6 );
		memcpy ( ( pK + 6 )->KKval3, ( pK + 48 )->KKval7, 6 );
	}

	Shz_Sale31Calc();

	return 0;

	/*
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

		//yoshida
		if (!((pK + 3)->KKauto & 0x01)) {
			::ZeroMemory(val, 6);
			memcpy((pK + 3)->KKval1, (pK + 49)->KKval1, 6);
		}
		if (!((pK + 4)->KKauto & 0x01)) {
			::ZeroMemory(val, 6);
			memcpy((pK + 4)->KKval1, (pK + 49)->KKval2, 6);
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
	return 0;*/
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzTax31Calc::Shz_DelData()
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
int	CShzTax31Calc::Shz_DelKkval( KKVAL* pK, int max )
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
		::ZeroMemory( (pK+i)->KKval9, sizeof((pK+i)->KKval9));
		::ZeroMemory( (pK+i)->KKval10, sizeof((pK+i)->KKval10));
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
int	CShzTax31Calc::Shz_ChkValChange( CString &str )
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
int	CShzTax31Calc::Shz_ChkKKauto( KKVAL* pK, int max )
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
int	CShzTax31Calc::Shz_shohizei( char* ans, char* val )
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
int	CShzTax31Calc::Shz_urcalc( char* ans, char* val )
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
int	CShzTax31Calc::Shz_sicalc( char* ans, char* val ) //Shz_shohizei4と同じ
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
int	CShzTax31Calc::Shz_ReportPost()
{
	int		ret=0;
//----->('23.09.14 Tanaka Add)
	long	eymd = 0;

	if (!( (*m_pSnHeadData)->Sn_SKKBN % 2 ) ) {
		eymd = (*m_pSnHeadData)->Sn_MDAYE;
	}
	else {
		eymd = (*m_pSnHeadData)->Sn_KDAYE;
	}
//<-----

//2016.06.22 UPDATE START
//	if( Util.IsUnConnectMst(m_pZmSub->zvol) && (Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_KANNI) ){// 非連動ｄｂで簡易課税
	if( Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_KANNI ){// 簡易課税
//2016.06.22 UPDATE END

		if( (*m_pSnHeadData)->SVmzsw == 1 ){//経過措置あり
		//----->('23.10.10 tanaka cor)
			//if( Shz_DataCpyKniKei() == -1 ){
			//	ret = -1;
			//}
		//------
			if (eymd >= ICS_SH_INVOICE_DAY) {
				if (Shz_InvDataCpyKniKei() == -1) {
					ret = -1;
				}
			}
			else {
				if (Shz_DataCpyKniKei() == -1) {
					ret = -1;
				}
			}
		//<-----
		}
		else{
		//----->('23.09.26 tanaka cor)
			//if( Shz_DataCpyKni() == -1 ){
			//	ret = -1;
			//}
		//------
		//　23.10.10課税取引金額計算表の制御を12/31→10/01切り替えに
		//	if (eymd >= TAX_R05_EDAY) {
			if (eymd >= ICS_SH_INVOICE_DAY) {
				if (Shz_InvDataCpyKni() == -1) {
					ret = -1;
				}
			}
			else {
				if (Shz_DataCpyKni() == -1) {
					ret = -1;
				}
			}
		//<-----
		}
	}
	else{
		if( (*m_pSnHeadData)->SVmzsw == 1 ){//経過措置あり
		//----->('23.10.10 Tanaka cor)
			//if( Shz_DataCpyUsuKei() == -1 ){
			//	ret = -1;
			//}
		//------
			if (eymd >= ICS_SH_INVOICE_DAY) {
				if (Shz_InvDataCpyUsuKei() == -1) {
					ret = -1;
				}
			}
			else {
				if (Shz_DataCpyUsuKei() == -1) {
					ret = -1;
				}
			}
		//<-----
		}
		else{
		//----->('23.09.14 Tanaka cor)
			//if (Shz_InvDataCpyUsu() == -1) {
			//	ret = -1;
			//}
		//------
		//　23.10.10課税取引金額計算表の制御を12/31→10/01切り替えに
		//	if (eymd >= TAX_R05_EDAY) {
			if (eymd >= ICS_SH_INVOICE_DAY) {
				if (Shz_InvDataCpyUsu() == -1) {
					ret = -1;
				}
			}
			else {
				if (Shz_DataCpyUsu() == -1) {
					ret = -1;
				}
			}
		//<-----
		}
	}

	return ret;
}


int	CShzTax31Calc::Shz_FileRead( void )
{
	return 0;
}

int	CShzTax31Calc::Shz_FileWrite(void )
{
	return 0;
}

//-----------------------------------------------------------------------------
// 通常課税
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzTax31Calc::Shz_DataCpyUsu()
{
	::ZeroMemory(&tenkidata , sizeof(tenkidata));

	KKVAL*	pJ = (*m_pTaxListData)->NRec.KJval;
	KKVAL*	pF = (*m_pTaxListData)->NRec.KFval;
	KKVAL*	pN = (*m_pTaxListData)->NRec.KNval;

	KKVAL*	pU = (*m_pTaxListData)->NRec.KUval;
	KKVAL*	pS = (*m_pTaxListData)->NRec.KSval;
	char	val[6], val2[6], PW1[6], PW2[6];

	// 第27表-(1)号様式
	// ①1000円未満切り捨て廃止されたため変更
	::ZeroMemory( val, 6 );
	//::ZeroMemory( PW1, 6 );
	//m_Arith.l_input( PW1, _T("1000") );
	//m_Arith.l_div( val, (pU+12)->KKval1, PW1 );//清水変更
	//m_Arith.l_mul( val, val, PW1 );
	memcpy(tenkidata.Sn_1F1D , (pU+12)->KKval1 , 6);

	::ZeroMemory( val, 6 );
	//::ZeroMemory( PW1, 6 );
	//m_Arith.l_input( PW1, _T("1000") );
	//m_Arith.l_div( val, (pU+11)->KKval1, PW1 );
	//m_Arith.l_mul( val, val, PW1 );
	memcpy(tenkidata.Sn_1F1C , (pU+11)->KKval1 , 6);

	// ②
	::ZeroMemory( val, 6 );
	::ZeroMemory( PW1, 6 );
	::ZeroMemory( PW2, 6 );
	m_Arith.l_input( PW1, _T("624") );
	m_Arith.l_input( PW2, _T("10000") );
	m_Arith.l_mul( val,tenkidata.Sn_1F1C, PW1 );
	m_Arith.l_div( val, val, PW2 );
	memcpy(tenkidata.Sn_1F2C , val , 6);

	::ZeroMemory( val, 6 );
	::ZeroMemory( PW1, 6 );
	::ZeroMemory( PW2, 6 );
	m_Arith.l_input( PW1, _T("78") );
	m_Arith.l_input( PW2, _T("1000") );
	m_Arith.l_mul( val,tenkidata.Sn_1F1D, PW1 );
	m_Arith.l_div( val, val, PW2 );
	memcpy(tenkidata.Sn_1F2D , val , 6);

	// ④

	// ⑤　返還等対価
	::ZeroMemory( val, 6 );
	//--->修正依頼168643　戎居
	if ((*m_pTaxListData)->GKBN & 0x01) {	// 事業
		m_Arith.l_add(val, val, (pJ + KJIGLINE1TA)->KKval5);
	}
	if ((*m_pTaxListData)->GKBN & 0x02) {	// 不動産
		m_Arith.l_add(val, val, (pF + KFUDLINE4TA)->KKval5);
	}
	if ((*m_pTaxListData)->GKBN & 0x04) {	// 農業
		m_Arith.l_add(val, val, (pN + KNOULINE4TA)->KKval5);
	}
	//memcpy( val, (pJ+KJIGLINE1TA)->KKval5, 6 );//清水変更
	//m_Arith.l_add( val, val, (pF+KFUDLINE4TA)->KKval5 );//清水変更
	//m_Arith.l_add( val, val, (pN+KNOULINE4TA)->KKval5 );//清水変更
	//<---
	if( (*m_pTaxListData)->Head.KHstax ){
		::ZeroMemory( val2, 6 );

		m_Arith.l_input( PW1, _T("8") );
		m_Arith.l_input( PW2, _T("100") );
		m_Arith.l_mul( val2,val, PW1 );
		m_Arith.l_div( val2, val2, PW2 );

		m_Arith.l_add( val, val, val2 );
	}
	::ZeroMemory( val2, 6 );
	Shz_shohizei624( val2, val );
	memcpy( tenkidata.Sn_1F5C, val2 , 6 );
//追加
	::ZeroMemory( val, 6 );
	//--->修正依頼168643　戎居
	if ((*m_pTaxListData)->GKBN & 0x01) {	// 事業
		m_Arith.l_add(val, val, (pJ + KJIGLINE1TA)->KKval6);
	}
	if ((*m_pTaxListData)->GKBN & 0x02) {	// 不動産
		m_Arith.l_add(val, val, (pF + KFUDLINE4TA)->KKval6);
	}
	if ((*m_pTaxListData)->GKBN & 0x04) {	// 農業
		m_Arith.l_add(val, val, (pN + KNOULINE4TA)->KKval6);
	}
	//memcpy( val, (pJ+KJIGLINE1TA)->KKval6, 6 );//清水変更
	//m_Arith.l_add( val, val, (pF+KFUDLINE4TA)->KKval6 );//清水変更
	//m_Arith.l_add( val, val, (pN+KNOULINE4TA)->KKval6 );//清水変更
	//<---
	if( (*m_pTaxListData)->Head.KHstax ){
		::ZeroMemory( val2, 6 );

		m_Arith.l_input( PW1, _T("10") );
		m_Arith.l_input( PW2, _T("100") );
		m_Arith.l_mul( val2,val, PW1 );
		m_Arith.l_div( val2, val2, PW2 );

		m_Arith.l_add( val, val, val2 );
	}
	::ZeroMemory( val2, 6 );
	Shz_shohizei78( val2, val );
	memcpy( tenkidata.Sn_1F5D, val2 , 6 );

	//---------------

	// 付表2
	// ①
	::ZeroMemory( val, 6 );
	memcpy( val, (pJ+KJIGLINE1TA)->KKval5, 6 );//清水変更
	m_Arith.l_add( val, val, (pF+KFUDLINE4TA)->KKval5 );//清水変更
	m_Arith.l_add( val, val, (pN+KNOULINE4TA)->KKval5 );//清水変更
	if( !(*m_pTaxListData)->Head.KHstax ){
		//Shz_shohizei100_110( val, val );
		Shz_shohizei100_108( val, val );
	}
	m_Arith.l_sub( tenkidata.Sn_2F1C, (pU+11)->KKval1, val );

//追加
	::ZeroMemory( val, 6 );
	memcpy( val, (pJ+KJIGLINE1TA)->KKval6, 6 );//清水変更33
	m_Arith.l_add( val, val, (pF+KFUDLINE4TA)->KKval6 );//清水変更15
	m_Arith.l_add( val, val, (pN+KNOULINE4TA)->KKval6 );//清水変更38
	if( !(*m_pTaxListData)->Head.KHstax ){
		Shz_shohizei100_110( val, val );//'15.01.09
	}
	m_Arith.l_sub( tenkidata.Sn_2F1D, (pU+12)->KKval1, val );

	//m_Arith.l_sub( tenkidata.Sn_2F1C, (pU+11)->KKval1, val );//清水変更16
	memcpy(tenkidata.Sn_2F8C , (pS+11)->KKval2 , 6);//清水変更
	memcpy(tenkidata.Sn_2F9C , (pS+11)->KKval1 , 6);//清水変更

	//m_Arith.l_sub( tenkidata.Sn_2F1D, (pU+18)->KKval1, val );//清水変更16
	memcpy(tenkidata.Sn_2F8D , (pS+12)->KKval2 , 6);//清水変更
	memcpy(tenkidata.Sn_2F9D , (pS+12)->KKval1 , 6);//清水変更

//----->('23.09.14 Tanaka del)
	// インボイス_23/07/31_higuchi add -->
		//memcpy(tenkidata.Sn_2F11C , (pS+13)->KKval2 , 6);
		//memcpy(tenkidata.Sn_2F12C , (pS+13)->KKval1 , 6);
		//memcpy(tenkidata.Sn_2F11D , (pS+14)->KKval2 , 6);
		//memcpy(tenkidata.Sn_2F12D , (pS+14)->KKval1 , 6);
	// インボイス_23/07/31_higuchi add <--
//<-----

	return 0;

}

//-----------------------------------------------------------------------------
// 簡易課税
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzTax31Calc::Shz_DataCpyKni()
{
	//経過措置用の処理を元にこの関数の処理作成しなおしyoshida200525

	KKVAL*	pU = (*m_pTaxListData)->NRec.KUval;
	KKVAL*	pJ = (*m_pTaxListData)->NRec.KJval;
	KKVAL*	pF = (*m_pTaxListData)->NRec.KFval;
	KKVAL*	pN = (*m_pTaxListData)->NRec.KNval;
	char	val[6], val2[6], PW1[6], PW2[6], clc[6];

	::ZeroMemory(&tenkidata, sizeof(tenkidata));

	// 課税標準額7.8％
	::ZeroMemory(val, 6);
	memcpy(tenkidata.Sn_1F1D, (pU + 12)->KKval1, 6);

	// 課税標準額6.24％
	::ZeroMemory(val, 6);
	memcpy(tenkidata.Sn_1F1C, (pU + 11)->KKval1, 6);

	//消費税率7.8%
	::ZeroMemory(val, 6);
	::ZeroMemory(PW1, 6);
	::ZeroMemory(PW2, 6);
	m_Arith.l_input(PW1, _T("78"));
	m_Arith.l_input(PW2, _T("1000"));
	m_Arith.l_mul(val, tenkidata.Sn_1F1D, PW1);
	m_Arith.l_div(val, val, PW2);
	memcpy(tenkidata.Sn_1F2D, val, 6);

	//消費税率6.24%
	::ZeroMemory(val, 6);
	::ZeroMemory(PW1, 6);
	::ZeroMemory(PW2, 6);
	m_Arith.l_input(PW1, _T("624"));
	m_Arith.l_input(PW2, _T("10000"));
	m_Arith.l_mul(val, tenkidata.Sn_1F1C, PW1);
	m_Arith.l_div(val, val, PW2);
	memcpy(tenkidata.Sn_1F2C, val, 6);


	//返還等課税取引7.8％を本表側の返還等対価に係る税額の7.8％へ
	if (Util.IsUnConnectMst(m_pZmSub->zvol)) {
		::ZeroMemory(clc, 6);
		for (int i = 0, id = 40; i < 5; i++) {//下if群２回通りidxが飛ばされるのでループは前と同じ５のまま。yoshida190728
//		for( int i=0, id=40; i<6; i++ ){
			::ZeroMemory(val, 6);
			memcpy(val, (pU + id)->KKval6, 6);
			if ((i == 2) || (i == 4)) {
				m_Arith.l_add(val, val, (pU + (id + 1))->KKval6);
				id++;
			}
			if ((*m_pTaxListData)->Head.KHstax) {
				::ZeroMemory(val2, 6);
				m_Arith.l_input(PW1, _T("10"));
				m_Arith.l_input(PW2, _T("100"));
				m_Arith.l_mul(val2, val, PW1);
				m_Arith.l_div(val2, val2, PW2);
				m_Arith.l_add(val, val, val2);
			}
			::ZeroMemory(val2, 6);
			Shz_shohizei78(val2, val);
			id++;
			m_Arith.l_add(clc, clc, val2);
		}
		memcpy(tenkidata.Sn_1F5D, clc, 6);
	}
	else {
		::ZeroMemory(val, 6);
		memcpy(val, (pJ + KJIGLINE1TA)->KKval6, 6);
		m_Arith.l_add(val, val, (pF + KFUDLINE4TA)->KKval6);
		m_Arith.l_add(val, val, (pN + KNOULINE4TA)->KKval6);
		if ((*m_pTaxListData)->Head.KHstax) {
			::ZeroMemory(val2, 6);
			m_Arith.l_input(PW1, _T("10"));
			m_Arith.l_input(PW2, _T("100"));
			m_Arith.l_mul(val2, val, PW1);
			m_Arith.l_div(val2, val2, PW2);

			m_Arith.l_add(val, val, val2);
		}
		::ZeroMemory(val2, 6);
		Shz_shohizei78(val2, val);
		memcpy(tenkidata.Sn_1F5D, val2, 6);
	}

	//返還等課税取引6.24％を本表側の返還等対価に係る税額の6.24％へ
	if (Util.IsUnConnectMst(m_pZmSub->zvol)) {
		::ZeroMemory(clc, 6);
		for (int i = 0, id = 40; i < 6; i++) {
			::ZeroMemory(val, 6);
			memcpy(val, (pU + id)->KKval8, 6);
			if (i == 1) {//第２種のループの際
				m_Arith.l_add(val, val, (pU + 49)->KKval8);//第２種農業の軽減分をプラス
			}
			else if (i == 2) {//第３種のループの際
				id++;
			}
			if ((*m_pTaxListData)->Head.KHstax) {
				::ZeroMemory(val2, 6);
				m_Arith.l_input(PW1, _T("8"));
				m_Arith.l_input(PW2, _T("100"));
				m_Arith.l_mul(val2, val, PW1);
				m_Arith.l_div(val2, val2, PW2);
				m_Arith.l_add(val, val, val2);
			}
			::ZeroMemory(val2, 6);
			Shz_shohizei624(val2, val);
			id++;
			m_Arith.l_add(clc, clc, val2);
		}
		memcpy(tenkidata.Sn_1F5C, clc, 6);
	}
	else {
		::ZeroMemory(val, 6);
		memcpy(val, (pJ + KJIGLINE1TA)->KKval5, 6);
		m_Arith.l_add(val, val, (pF + KFUDLINE4TA)->KKval5);
		m_Arith.l_add(val, val, (pN + KNOULINE4TA)->KKval5);
		if ((*m_pTaxListData)->Head.KHstax) {
			::ZeroMemory(val2, 6);
			m_Arith.l_input(PW1, _T("8"));
			m_Arith.l_input(PW2, _T("100"));
			m_Arith.l_mul(val2, val, PW1);
			m_Arith.l_div(val2, val2, PW2);

			m_Arith.l_add(val, val, val2);
		}
		::ZeroMemory(val2, 6);
		Shz_shohizei624(val2, val);
		memcpy(tenkidata.Sn_1F5C, val2, 6);
	}

	// 第１種～第５種までの金額を付表５－２に転記　7.8％分
	//課税取引金額　７．８％
	::ZeroMemory(clc, 6);
	if (Util.IsUnConnectMst(m_pZmSub->zvol)) {
		for (int i = 0, id = 40; i < 6; i++) {
			::ZeroMemory(val, 6);
			memcpy(val, (pU + id)->KKval5, 6);
			if (i == 2) {
				m_Arith.l_add(val, val, (pU + (id + 1))->KKval5);
			}
			else if (i == 3) {		// 10/04/06 追加
				m_Arith.l_add(val, val, (pU + 48)->KKval5);
			}
			if (!(*m_pTaxListData)->Head.KHstax) {
				Shz_shohizei100_110(val, val);
			}

			::ZeroMemory(val2, 6);
			memcpy(val2, (pU + id)->KKval6, 6);
			if (i == 2) {
				m_Arith.l_add(val2, val2, (pU + (id + 1))->KKval6);
				id++;
			}
			if (!(*m_pTaxListData)->Head.KHstax) {
				Shz_shohizei100_110(val2, val2);
			}
			id++;
			m_Arith.l_sub(val, val, val2);

			switch (i) {
				case 0:
					memcpy(tenkidata.Sn_5FURI1D, val, 6);
					break;
				case 1:
					memcpy(tenkidata.Sn_5FURI2D, val, 6);
					break;

				case 2:
					memcpy(tenkidata.Sn_5FURI3D, val, 6);
					break;

				case 3:
					memcpy(tenkidata.Sn_5FURI4D, val, 6);
					break;

				case 4:
					memcpy(tenkidata.Sn_5FURI5D, val, 6);
					break;

				case 5:
					memcpy(tenkidata.Sn_5FURI6D, val, 6);
					break;
			}
			m_Arith.l_add(clc, clc, val);
		}
	}
	else {
		::ZeroMemory(tenkidata.Sn_5FURI1D, 6);
		::ZeroMemory(tenkidata.Sn_5FURI2D, 6);
		::ZeroMemory(tenkidata.Sn_5FURI3D, 6);
		::ZeroMemory(tenkidata.Sn_5FURI4D, 6);
		::ZeroMemory(tenkidata.Sn_5FURI5D, 6);
		::ZeroMemory(tenkidata.Sn_5FURI6D, 6);
	}

	// 第１種～第５種までの金額を付表５－２に転記　6.24％分

	//課税取引金額６．２４％（返還等課税取引引く）
	//修正168614　戎居
	//::ZeroMemory(clc, 6);
	if (Util.IsUnConnectMst(m_pZmSub->zvol)) {
		//yoshida190728
		for (int i = 0, id = 40; i < 6; i++) {
			::ZeroMemory(val, 6);
			memcpy(val, (pU + id)->KKval7, 6);
			if (i == 1) {//第２種農業分足す
				m_Arith.l_add(val, val, (pU + 49)->KKval7);
			}
			else if (i == 3) {
				m_Arith.l_add(val, val, (pU + 48)->KKval7);
			}
			if (!(*m_pTaxListData)->Head.KHstax) {
				Shz_shohizei100_108(val, val);
			}

			::ZeroMemory(val2, 6);
			memcpy(val2, (pU + id)->KKval8, 6);
			if (i == 1) {//第２種農業分足す
				m_Arith.l_add(val2, val2, (pU + 49)->KKval8);
			}
			else if (i == 2) {
				id++;
			}
			if (!(*m_pTaxListData)->Head.KHstax) {
				Shz_shohizei100_108(val2, val2);
			}
			id++;
			m_Arith.l_sub(val, val, val2);


			switch (i) {
				case 0:
					memcpy(tenkidata.Sn_5FURI1C, val, 6);
					break;
				case 1:
					memcpy(tenkidata.Sn_5FURI2C, val, 6);
					break;

				case 2:
					memcpy(tenkidata.Sn_5FURI3C, val, 6);
					break;

				case 3:
					memcpy(tenkidata.Sn_5FURI4C, val, 6);
					break;

				case 4:
					memcpy(tenkidata.Sn_5FURI5C, val, 6);
					break;

				case 5:
					memcpy(tenkidata.Sn_5FURI6C, val, 6);
					break;
			}
			m_Arith.l_add(clc, clc, val);
		}
	}
	else {
		::ZeroMemory(tenkidata.Sn_5FURI1C, 6);
		::ZeroMemory(tenkidata.Sn_5FURI2C, 6);
		::ZeroMemory(tenkidata.Sn_5FURI3C, 6);
		::ZeroMemory(tenkidata.Sn_5FURI4C, 6);
		::ZeroMemory(tenkidata.Sn_5FURI5C, 6);
		::ZeroMemory(tenkidata.Sn_5FURI6C, 6);
	}

	// 付表5
	if (Util.IsUnConnectMst(m_pZmSub->zvol)) {
		memcpy(tenkidata.Sn_5FURITA, clc, 6);
	}
	else {//転記しない場合は初期化
		::ZeroMemory(tenkidata.Sn_5FURITA, 6);
	}

	::ZeroMemory(tenkidata.Sn_5FUZTT, 6);
	::ZeroMemory(tenkidata.Sn_5FUZT1, 6);
	::ZeroMemory(tenkidata.Sn_5FUZT2, 6);
	::ZeroMemory(tenkidata.Sn_5FUZT3, 6);
	::ZeroMemory(tenkidata.Sn_5FUZT4, 6);
	::ZeroMemory(tenkidata.Sn_5FUZT5, 6);
	::ZeroMemory(tenkidata.Sn_5FUZT6, 6);	// '16.12.27

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
int	CShzTax31Calc::Shz_Buy31Post()
{
	if( !( Util.IsUnConnectMst(m_pZmSub->zvol) && (Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_KANNI) ) ){// 非連動ｄｂで簡易課税じゃ無い場合
	// 22-0167,22-0361 add -->
		long	eymd=0;
		int		ii=0,old=0,sw=0;
		if(!((*m_pSnHeadData)->Sn_SKKBN % 2))	eymd = (*m_pSnHeadData)->Sn_MDAYE;
		else									eymd = (*m_pSnHeadData)->Sn_KDAYE;
// インボイス_23/07/24_higuchi cor -->
		//if((*m_pSnHeadData)->SVmzsw == 1 || eymd < TAX_R04_SDAY)	old=1;
// -----------------------------------
		//　23.10.10課税取引金額計算表の制御を12/31→10/01切り替えに
	//	if((*m_pSnHeadData)->SVmzsw == 1 || eymd < TAX_R04_SDAY || eymd >= TAX_R05_EDAY)	{
		if ((*m_pSnHeadData)->SVmzsw == 1 || eymd < TAX_R04_SDAY || eymd >= ICS_SH_INVOICE_DAY) {
			old=1;
		}
// インボイス_23/07/24_higuchi cor <--
// 22-0167,22-0361 add <--
		KKVAL*	pK = (*m_pTaxListData)->NRec.KSval;
		if( (*m_pTaxListData)->GKBN & 0x01 ){	// 事業所
			if( !((pK+KBUYLINE1)->KKauto&0x01) ){
				memcpy( (pK+KBUYLINE1)->KKval1, (*m_pTaxListData)->NRec.KJval[KJIGLINE34].KKval3, 6 );
			}
			if( !((pK+KBUYLINE1)->KKauto&0x02) ){
				if ((*m_pSnHeadData)->SVmzsw == 1) {
					memcpy((pK + KBUYLINE1)->KKval2, (*m_pTaxListData)->NRec.KJval[KJIGLINE34].KKval4, 6);
				}
				else {
					::ZeroMemory((pK + KBUYLINE1)->KKval2, 6);
				}
			}
			if( !((pK+KBUYLINE1)->KKauto&0x04) ){
				memcpy( (pK+KBUYLINE1)->KKval3, (*m_pTaxListData)->NRec.KJval[KJIGLINE34].KKval5, 6 );
			}
// 22-0167,22-0361 cor -->
			//if( !((pK+KBUYLINE1)->KKauto&0x08) ){
			//	memcpy( (pK+KBUYLINE1)->KKval4, (*m_pTaxListData)->NRec.KJval[KJIGLINE34].KKval6, 6 );
			//}
// -----------------------
			sw=0;
			if(old)	{
				if(!((pK+KBUYLINE1)->KKauto&0x08))	sw=1;
			}
			else	{
				if((!((pK+KBUYLINE1)->KKauto&0x08)) && (!((*m_pSnHeadData)->Sn_GenericSgn & 0x800)))	sw=1;
			}
			if(sw)	{
				memcpy( (pK+KBUYLINE1)->KKval4, (*m_pTaxListData)->NRec.KJval[KJIGLINE34].KKval6, 6 );
			}
// 22-0167,22-0361 cor <--
// インボイス_23/07/28_higuchi add -->
			if( !((pK+KBUYLINE1)->KKauto&0x10) ){
				memcpy( (pK+KBUYLINE1)->KKval7, (*m_pTaxListData)->NRec.KJval[KJIGLINE34].KKval7, 6 );
			}
			if( !((pK+KBUYLINE1)->KKauto&0x20) ){
				memcpy( (pK+KBUYLINE1)->KKval8, (*m_pTaxListData)->NRec.KJval[KJIGLINE34].KKval8, 6 );
			}
// インボイス_23/07/28_higuchi add <--
		}

		if( (*m_pTaxListData)->GKBN & 0x04 ){	// 農業
			if( !((pK+KBUYLINE2)->KKauto&0x01) ){
				memcpy( (pK+KBUYLINE2)->KKval1, (*m_pTaxListData)->NRec.KNval[KNOULINE31].KKval3, 6 );
			}
			if( !((pK+KBUYLINE2)->KKauto&0x02) ){
				if ((*m_pSnHeadData)->SVmzsw == 1) {
					memcpy((pK + KBUYLINE2)->KKval2, (*m_pTaxListData)->NRec.KNval[KNOULINE31].KKval4, 6);
				}
				else {
					::ZeroMemory((pK + KBUYLINE2)->KKval2, 6);
				}
			}
			if( !((pK+KBUYLINE2)->KKauto&0x04) ){
				memcpy( (pK+KBUYLINE2)->KKval3, (*m_pTaxListData)->NRec.KNval[KNOULINE31].KKval5, 6 );
			}
// 22-0167,22-0361 cor -->
			//if( !((pK+KBUYLINE2)->KKauto&0x08) ){
			//	memcpy( (pK+KBUYLINE2)->KKval4, (*m_pTaxListData)->NRec.KNval[KNOULINE31].KKval6, 6 );
			//}
// -----------------------
			sw=0;
			if(old)	{
				if(!((pK+KBUYLINE2)->KKauto&0x08))	sw=1;
			}
			else	{
				if((!((pK+KBUYLINE2)->KKauto&0x08)) && (!((*m_pSnHeadData)->Sn_GenericSgn & 0x800)))	sw=1;
			}
			if(sw)	{
				memcpy( (pK+KBUYLINE2)->KKval4, (*m_pTaxListData)->NRec.KNval[KNOULINE31].KKval6, 6 );
			}
// 22-0167,22-0361 cor <--
// インボイス_23/07/28_higuchi add -->
			if( !((pK+KBUYLINE2)->KKauto&0x10) ){
				memcpy( (pK+KBUYLINE2)->KKval7, (*m_pTaxListData)->NRec.KNval[KNOULINE31].KKval7, 6 );
			}
			if( !((pK+KBUYLINE2)->KKauto&0x20) ){
				memcpy( (pK+KBUYLINE2)->KKval8, (*m_pTaxListData)->NRec.KNval[KNOULINE31].KKval8, 6 );
			}
// インボイス_23/07/28_higuchi add <--
		}

		if( (*m_pTaxListData)->GKBN & 0x02 ){	// 不動産
			if( !((pK+KBUYLINE3)->KKauto&0x01) ){
				memcpy( (pK+KBUYLINE3)->KKval1, (*m_pTaxListData)->NRec.KFval[KFUDLINE14].KKval3, 6 );
			}
			if( !((pK+KBUYLINE3)->KKauto&0x02) ){
				if ((*m_pSnHeadData)->SVmzsw == 1) {
					memcpy((pK + KBUYLINE3)->KKval2, (*m_pTaxListData)->NRec.KFval[KFUDLINE14].KKval4, 6);
				}
				else {
					::ZeroMemory((pK + KBUYLINE3)->KKval2, 6);
				}
			}
			if( !((pK+KBUYLINE3)->KKauto&0x04) ){
				memcpy( (pK+KBUYLINE3)->KKval3, (*m_pTaxListData)->NRec.KFval[KFUDLINE14].KKval5, 6 );
			}
// 22-0167,22-0361 cor -->
			//if( !((pK+KBUYLINE3)->KKauto&0x08) ){
			//	memcpy( (pK+KBUYLINE3)->KKval4, (*m_pTaxListData)->NRec.KFval[KFUDLINE14].KKval6, 6 );
			//}
// -----------------------
			sw=0;
			if(old)	{
				if(!((pK+KBUYLINE3)->KKauto&0x08))	sw=1;
			}
			else	{
				if((!((pK+KBUYLINE3)->KKauto&0x08)) && (!((*m_pSnHeadData)->Sn_GenericSgn & 0x800)))	sw=1;
			}
			if(sw)	{
				memcpy( (pK+KBUYLINE3)->KKval4, (*m_pTaxListData)->NRec.KFval[KFUDLINE14].KKval6, 6 );
			}
// 22-0167,22-0361 cor <--
// インボイス_23/07/28_higuchi add -->
			if( !((pK+KBUYLINE3)->KKauto&0x10) ){
				memcpy( (pK+KBUYLINE3)->KKval7, (*m_pTaxListData)->NRec.KFval[KFUDLINE14].KKval7, 6 );
			}
			if( !((pK+KBUYLINE3)->KKauto&0x20) ){
				memcpy( (pK+KBUYLINE3)->KKval8, (*m_pTaxListData)->NRec.KFval[KFUDLINE14].KKval8, 6 );
			}
// インボイス_23/07/28_higuchi add <--
		}
// 22-0167,22-0361 add -->
		// 新様式の場合、自動計算チェックが付いていれば自動計算する
// インボイス_23/07/24_higuchi cor -->
		//if((*m_pSnHeadData)->SVmzsw != 1 && eymd >= TAX_R04_SDAY)	{
// -----------------------------------
		//　23.10.10課税取引金額計算表の制御を12/31→10/01切り替えに
		//if((*m_pSnHeadData)->SVmzsw != 1 && eymd >= TAX_R04_SDAY && eymd < TAX_R05_EDAY)	{
		if ((*m_pSnHeadData)->SVmzsw != 1 && eymd >= TAX_R04_SDAY && eymd < ICS_SH_INVOICE_DAY) {
				// インボイス_23/07/24_higuchi cor <--
			for(ii=KBUYLINE1; ii<=KBUYLINE8; ii++)	{
				if(ii == KBUYLINE6)	continue;
				// (4)業務用資産…、⑦のうち、課税売上げにならないもの
				if(ii == KSELLINE8)	{
					// 連動時
					if(!((*m_pSnHeadData)->Sn_Sign4 & 0x80))	{
						continue;
					}
				}
				// 金額 - 6.24% = 7.8%
				if((*m_pSnHeadData)->Sn_GenericSgn & 0x800)	{
					m_Arith.l_sub((pK+ii)->KKval4,(pK+ii)->KKval1,(pK+ii)->KKval3);
					if(ii == KSELLINE7)	{
						// (4)業務用資産…、業務用固定資産等の譲渡収入金額
						// 連動時は課税売上げにならないものも差し引く
						if(!((*m_pSnHeadData)->Sn_Sign4 & 0x80))	{
							m_Arith.l_sub((pK+ii)->KKval4,(pK+ii)->KKval4,(pK+ii+1)->KKval1);
						}
					}
					(pK+ii)->KKauto &= 0xf7;
				}
			}
		}
// 22-0167,22-0361 add <--
	}
	

	Shz_Buy31Calc();
	return 0;
}


//-----------------------------------------------------------------------------
// 課税売上高への転記
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int	CShzTax31Calc::Shz_Sale31Post()
{
//	if( !(pSyzShin->KniFlg&0x01) ){
	if( !( Util.IsUnConnectMst(m_pZmSub->zvol) && (Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_KANNI) ) ){// 非連動ｄｂで簡易課税じゃ無い場合'14.10.08
// 22-0167,22-0361 add -->
		long	eymd=0;
		int		ii=0,old=0,sw=0;
		if(!((*m_pSnHeadData)->Sn_SKKBN % 2))	eymd = (*m_pSnHeadData)->Sn_MDAYE;
		else									eymd = (*m_pSnHeadData)->Sn_KDAYE;
// インボイス_23/07/24_higuchi cor -->
		//if((*m_pSnHeadData)->SVmzsw == 1 || eymd < TAX_R04_SDAY)	old=1;
// -----------------------------------
		//　23.10.10課税取引金額計算表の制御を12/31→10/01切り替えに
	//	if((*m_pSnHeadData)->SVmzsw == 1 || eymd < TAX_R04_SDAY || eymd >= TAX_R05_EDAY)	{
		if ((*m_pSnHeadData)->SVmzsw == 1 || eymd < TAX_R04_SDAY || eymd >= ICS_SH_INVOICE_DAY) {
			old=1;
		}
// インボイス_23/07/24_higuchi cor <--
// 22-0167,22-0361 add <--

		KKVAL*	pK = (*m_pTaxListData)->NRec.KUval;

		if( (*m_pTaxListData)->GKBN & 0x01 ){	// 事業所
			if( !((pK+KSELLINE1)->KKauto&0x01) ){
				memcpy( (pK+KSELLINE1)->KKval1, (*m_pTaxListData)->NRec.KJval[KJIGLINE1].KKval3, 6 );
			}
			if( !((pK+KSELLINE1)->KKauto&0x02) ){
				if ((*m_pSnHeadData)->SVmzsw == 1) {
					memcpy((pK + KSELLINE1)->KKval2, (*m_pTaxListData)->NRec.KJval[KJIGLINE1].KKval4, 6);
				}
				else {
					::ZeroMemory( (pK + KSELLINE1)->KKval2, 6 );
				}
			}
			if( !((pK+KSELLINE1)->KKauto&0x04) ){
				memcpy( (pK+KSELLINE1)->KKval3, (*m_pTaxListData)->NRec.KJval[KJIGLINE1].KKval5, 6 );
			}
// 22-0167,22-0361 cor -->
			//if( !((pK+KSELLINE1)->KKauto&0x08)) {
			//	memcpy((pK + KSELLINE1)->KKval4, (*m_pTaxListData)->NRec.KJval[KJIGLINE1].KKval6, 6);
			//}
// -----------------------
			sw=0;
			if(old)	{
				if(!((pK+KSELLINE1)->KKauto&0x08))	sw=1;
			}
			else	{
				if((!((pK+KSELLINE1)->KKauto&0x08)) && (!((*m_pSnHeadData)->Sn_GenericSgn & 0x800)))	sw=1;
			}
			if(sw)	{
				memcpy((pK + KSELLINE1)->KKval4, (*m_pTaxListData)->NRec.KJval[KJIGLINE1].KKval6, 6);
			}
// 22-0167,22-0361 cor <--
		}

		if( (*m_pTaxListData)->GKBN & 0x04 ){	// 農業
			if( !((pK+KSELLINE2)->KKauto&0x01) ){
				memcpy( (pK+KSELLINE2)->KKval1, (*m_pTaxListData)->NRec.KNval[KNOULINE4].KKval3, 6 );
			}
			if( !((pK+KSELLINE2)->KKauto&0x02) ){
				if ((*m_pSnHeadData)->SVmzsw == 1) {
					memcpy((pK + KSELLINE2)->KKval2, (*m_pTaxListData)->NRec.KNval[KNOULINE4].KKval4, 6);
				}
				else {
					::ZeroMemory( (pK + KSELLINE2)->KKval2, 6 );
				}
			}
			if( !((pK+KSELLINE2)->KKauto&0x04) ){
				memcpy( (pK+KSELLINE2)->KKval3, (*m_pTaxListData)->NRec.KNval[KNOULINE4].KKval5, 6 );
			}
// 22-0167,22-0361 cor -->
			//if( !((pK+KSELLINE2)->KKauto&0x08) ){
			//	memcpy( (pK+KSELLINE2)->KKval4, (*m_pTaxListData)->NRec.KNval[KNOULINE4].KKval6, 6);
			//}
// -----------------------
			sw=0;
			if(old)	{
				if(!((pK+KSELLINE2)->KKauto&0x08))	sw=1;
			}
			else	{
				if((!((pK+KSELLINE2)->KKauto&0x08)) && (!((*m_pSnHeadData)->Sn_GenericSgn & 0x800)))	sw=1;
			}
			if(sw)	{
				memcpy( (pK+KSELLINE2)->KKval4, (*m_pTaxListData)->NRec.KNval[KNOULINE4].KKval6, 6);
			}
// 22-0167,22-0361 cor <--
		}

		if( (*m_pTaxListData)->GKBN & 0x02 ){	// 不動産
			if( !((pK+KSELLINE3)->KKauto&0x01) ){
				memcpy( (pK+KSELLINE3)->KKval1, (*m_pTaxListData)->NRec.KFval[KFUDLINE4].KKval3, 6 );
			}	
			if( !((pK+KSELLINE3)->KKauto&0x02) ){
				if ((*m_pSnHeadData)->SVmzsw == 1) {
					memcpy((pK + KSELLINE3)->KKval2, (*m_pTaxListData)->NRec.KFval[KFUDLINE4].KKval4, 6);
				}
				else {
					::ZeroMemory( (pK + KSELLINE3)->KKval2, 6 );
				}
			}
			if( !((pK+KSELLINE3)->KKauto&0x04) ){
				memcpy( (pK+KSELLINE3)->KKval3, (*m_pTaxListData)->NRec.KFval[KFUDLINE4].KKval5, 6 );
			}
// 22-0167,22-0361 cor -->
			//if( !((pK+KSELLINE3)->KKauto&0x08) ) {
			//	memcpy( (pK+KSELLINE3)->KKval4, (*m_pTaxListData)->NRec.KFval[KFUDLINE4].KKval6, 6);
			//}
// -----------------------
			sw=0;
			if(old)	{
				if(!((pK+KSELLINE3)->KKauto&0x08))	sw=1;
			}
			else	{
				if((!((pK+KSELLINE3)->KKauto&0x08)) && (!((*m_pSnHeadData)->Sn_GenericSgn & 0x800)))	sw=1;
			}
			if(sw)	{
				memcpy( (pK+KSELLINE3)->KKval4, (*m_pTaxListData)->NRec.KFval[KFUDLINE4].KKval6, 6);
			}
// 22-0167,22-0361 cor <--
		}
// 22-0167,22-0361 add -->
		// 新様式の場合、自動計算チェックが付いていれば自動計算する
// インボイス_23/07/24_higuchi cor -->
		//if((*m_pSnHeadData)->SVmzsw != 1 && eymd >= TAX_R04_SDAY)	{
// -----------------------------------
		//if((*m_pSnHeadData)->SVmzsw != 1 && eymd >= TAX_R04_SDAY && eymd < TAX_R05_EDAY)	{
	//5.10.24 ebisui
	//  04新様式かつ経過措置無し　または
	//	05.10インボイス施行後　　の場合、自動計算を行う
		//if ((*m_pSnHeadData)->SVmzsw != 1 && eymd >= TAX_R04_SDAY ) {
		if ((*m_pSnHeadData)->SVmzsw != 1 && ICS_SH_INVOICE_DAY > eymd && eymd >= TAX_R04_SDAY || eymd >= ICS_SH_INVOICE_DAY) {
			// インボイス_23/07/24_higuchi cor <--
			for(ii=KSELLINE1; ii<=KSELLINE8; ii++)	{
				if(ii == KSELLINE6)	continue;
				// (4)業務用資産…、⑦のうち、課税売上げにならないもの
				if(ii == KSELLINE8)	{
					// 連動時
					if(!((*m_pSnHeadData)->Sn_Sign4 & 0x80))	{
						continue;
					}
				}
				// 金額 - 6.24% = 7.8%
				if((*m_pSnHeadData)->Sn_GenericSgn & 0x800)	{
					m_Arith.l_sub((pK+ii)->KKval4,(pK+ii)->KKval1,(pK+ii)->KKval3);
					if(ii == KSELLINE7)	{
						// (4)業務用資産…、業務用固定資産等の譲渡収入金額
						// 連動時は課税売上げにならないものも差し引く
						if(!((*m_pSnHeadData)->Sn_Sign4 & 0x80))	{
							m_Arith.l_sub((pK+ii)->KKval4,(pK+ii)->KKval4,(pK+ii+1)->KKval1);
						}
					}
					(pK+ii)->KKauto &= 0xf7;
				}
			}
		}
// 22-0167,22-0361 add <--
	}
	else {
		Shz_KniKeiPost();
	}


	Shz_Sale31Calc();
	return 0;
}


//-----------------------------------------------------------------------------
// 課税仕入高
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzTax31Calc::Shz_Buy31Calc()
{

	KKVAL*	pK = (*m_pTaxListData)->NRec.KSval;

	//差引課税仕入高(4-5)
	m_Arith.l_sub( (pK + KBUYLINE6)->KKval1, (pK + KBUYLINE4)->KKval1, (pK + KBUYLINE5)->KKval1 );
	m_Arith.l_sub( (pK + KBUYLINE6)->KKval2, (pK + KBUYLINE4)->KKval2, (pK + KBUYLINE5)->KKval2 );
	m_Arith.l_sub( (pK + KBUYLINE6)->KKval3, (pK + KBUYLINE4)->KKval3, (pK + KBUYLINE5)->KKval3 );
	m_Arith.l_sub( (pK + KBUYLINE6)->KKval4, (pK + KBUYLINE4)->KKval4, (pK + KBUYLINE5)->KKval4 );
//----->('23.07.26 Add)
	m_Arith.l_sub( (pK + KBUYLINE6)->KKval7, (pK + KBUYLINE4)->KKval7, (pK + KBUYLINE5)->KKval7 );
	m_Arith.l_sub( (pK + KBUYLINE6)->KKval8, (pK + KBUYLINE4)->KKval8, (pK + KBUYLINE5)->KKval8 );
//<-----

	//差引課税仕入高(7-8)
	m_Arith.l_sub( (pK + KBUYLINE9)->KKval1, (pK + KBUYLINE7)->KKval1, (pK + KBUYLINE8)->KKval1 );
	m_Arith.l_sub( (pK + KBUYLINE9)->KKval2, (pK + KBUYLINE7)->KKval2, (pK + KBUYLINE8)->KKval2 );
	m_Arith.l_sub( (pK + KBUYLINE9)->KKval3, (pK + KBUYLINE7)->KKval3, (pK + KBUYLINE8)->KKval3 );
	m_Arith.l_sub( (pK + KBUYLINE9)->KKval4, (pK + KBUYLINE7)->KKval4, (pK + KBUYLINE8)->KKval4 );
//----->('23.07.26 Add)
	m_Arith.l_sub( (pK + KBUYLINE9)->KKval7, (pK + KBUYLINE7)->KKval7, (pK + KBUYLINE8)->KKval7 );
	m_Arith.l_sub( (pK + KBUYLINE9)->KKval8, (pK + KBUYLINE7)->KKval8, (pK + KBUYLINE8)->KKval8 );
//<-----

	//課税仕入高の合計額(①+②＋③＋⑥＋⑨) A
	::ZeroMemory( (pK + KBUYLINE10)->KKval1, 6 );
	m_Arith.l_add( (pK + KBUYLINE10)->KKval1, (pK + KBUYLINE1)->KKval1, (pK + KBUYLINE2)->KKval1 );
	m_Arith.l_add( (pK + KBUYLINE10)->KKval1, (pK + KBUYLINE10)->KKval1, (pK + KBUYLINE3)->KKval1 );
	m_Arith.l_add( (pK + KBUYLINE10)->KKval1, (pK + KBUYLINE10)->KKval1, (pK + KBUYLINE6)->KKval1 );
	m_Arith.l_add( (pK + KBUYLINE10)->KKval1, (pK + KBUYLINE10)->KKval1, (pK + KBUYLINE9)->KKval1 );

	//課税仕入高の合計額(①+②＋③＋⑥＋⑨) B
	::ZeroMemory( (pK + KBUYLINE10)->KKval2, 6 );
	m_Arith.l_add( (pK + KBUYLINE10)->KKval2, (pK + KBUYLINE1)->KKval2, (pK + KBUYLINE2)->KKval2 );
	m_Arith.l_add( (pK + KBUYLINE10)->KKval2, (pK + KBUYLINE10)->KKval2, (pK + KBUYLINE3)->KKval2 );
	m_Arith.l_add( (pK + KBUYLINE10)->KKval2, (pK + KBUYLINE10)->KKval2, (pK + KBUYLINE6)->KKval2 );
	m_Arith.l_add( (pK + KBUYLINE10)->KKval2, (pK + KBUYLINE10)->KKval2, (pK + KBUYLINE9)->KKval2 );

	//課税仕入高の合計額(①+②＋③＋⑥＋⑨) C
	::ZeroMemory( (pK + KBUYLINE10)->KKval3, 6 );
	m_Arith.l_add( (pK + KBUYLINE10)->KKval3, (pK + KBUYLINE1)->KKval3, (pK + KBUYLINE2)->KKval3 );
	m_Arith.l_add( (pK + KBUYLINE10)->KKval3, (pK + KBUYLINE10)->KKval3, (pK + KBUYLINE3)->KKval3 );
	m_Arith.l_add( (pK + KBUYLINE10)->KKval3, (pK + KBUYLINE10)->KKval3, (pK + KBUYLINE6)->KKval3 );
	m_Arith.l_add( (pK + KBUYLINE10)->KKval3, (pK + KBUYLINE10)->KKval3, (pK + KBUYLINE9)->KKval3 );

	//課税仕入高の合計額(①+②＋③＋⑥＋⑨) D
	::ZeroMemory( (pK + KBUYLINE10)->KKval4, 6 );
	m_Arith.l_add( (pK + KBUYLINE10)->KKval4, (pK + KBUYLINE1)->KKval4, (pK + KBUYLINE2)->KKval4 );
	m_Arith.l_add( (pK + KBUYLINE10)->KKval4, (pK + KBUYLINE10)->KKval4, (pK + KBUYLINE3)->KKval4 );
	m_Arith.l_add( (pK + KBUYLINE10)->KKval4, (pK + KBUYLINE10)->KKval4, (pK + KBUYLINE6)->KKval4 );
	m_Arith.l_add( (pK + KBUYLINE10)->KKval4, (pK + KBUYLINE10)->KKval4, (pK + KBUYLINE9)->KKval4 );
//----->('23.07.26 Add)
	//課税仕入高の合計額(①+②＋③＋⑥＋⑨) E
	::ZeroMemory( (pK + KBUYLINE10)->KKval7, 6);
	m_Arith.l_add( (pK + KBUYLINE10)->KKval7, (pK + KBUYLINE1)->KKval7, (pK + KBUYLINE2)->KKval7 );
	m_Arith.l_add( (pK + KBUYLINE10)->KKval7, (pK + KBUYLINE10)->KKval7, (pK + KBUYLINE3)->KKval7 );
	m_Arith.l_add( (pK + KBUYLINE10)->KKval7, (pK + KBUYLINE10)->KKval7, (pK + KBUYLINE6)->KKval7 );
	m_Arith.l_add( (pK + KBUYLINE10)->KKval7, (pK + KBUYLINE10)->KKval7, (pK + KBUYLINE9)->KKval7 );

	//課税仕入高の合計額(①+②＋③＋⑥＋⑨) F
	::ZeroMemory((pK + KBUYLINE10)->KKval8, 6);
	m_Arith.l_add( (pK + KBUYLINE10)->KKval8, (pK + KBUYLINE1)->KKval8, (pK + KBUYLINE2)->KKval8 );
	m_Arith.l_add( (pK + KBUYLINE10)->KKval8, (pK + KBUYLINE10)->KKval8, (pK + KBUYLINE3)->KKval8 );
	m_Arith.l_add( (pK + KBUYLINE10)->KKval8, (pK + KBUYLINE10)->KKval8, (pK + KBUYLINE6)->KKval8 );
	m_Arith.l_add( (pK + KBUYLINE10)->KKval8, (pK + KBUYLINE10)->KKval8, (pK + KBUYLINE9)->KKval8 );
//<-----
	char	val[6];

	// 仮払消費税(⑪B欄の金額)
	if( (*m_pTaxListData)->Head.KHstax ){
		if( !((pK + KBUYLINE11)->KKauto&0x04) ){
			::ZeroMemory( val, 6 );
//			Shz_shohizei4( val, (pK + KBUYLINE13)->KKval2 );
			Shz_shohizei8_100( val, (pK + KBUYLINE10)->KKval2 );
			memcpy( (pK + KBUYLINE11)->KKval3, val, 6 );
		}
	}

	if( !((pK + KBUYLINE11)->KKauto&0x02) ){
		if( (*m_pTaxListData)->Head.KHstax ){
			m_Arith.l_add( (pK + KBUYLINE11)->KKval2, (pK + KBUYLINE10)->KKval2, (pK + KBUYLINE11)->KKval3 );
		}
		else{
			memcpy( (pK + KBUYLINE11)->KKval2, (pK + KBUYLINE10)->KKval2, 6 );
		}
	}
	if( !((pK + KBUYLINE11)->KKauto&0x01) ){
		::ZeroMemory( val, 6 );
		Shz_shohizei63( val, (pK + KBUYLINE11)->KKval2 );
		memcpy( (pK + KBUYLINE11)->KKval1, val, 6 );
	}

	// 仮払消費税(⑫C欄の金額)
	if( (*m_pTaxListData)->Head.KHstax ){
		if( !((pK + KBUYLINE12)->KKauto&0x04) ){
			::ZeroMemory( val, 6 );
//			Shz_shohizei63( val, (pK + KBUYLINE13)->KKval3 );
			Shz_shohizei8_100( val, (pK + KBUYLINE10)->KKval3 );
			memcpy( (pK + KBUYLINE12)->KKval3, val, 6 );
		}
	}

	if( !((pK + KBUYLINE12)->KKauto&0x02) ){
		if( (*m_pTaxListData)->Head.KHstax ){
			m_Arith.l_add( (pK + KBUYLINE12)->KKval2, (pK + KBUYLINE10)->KKval3, (pK + KBUYLINE12)->KKval3 );
		}
		else{
			memcpy( (pK + KBUYLINE12)->KKval2, (pK + KBUYLINE10)->KKval3, 6 );
		}
	}
	if( !((pK + KBUYLINE12)->KKauto&0x01) ){
		::ZeroMemory( val, 6 );
		Shz_shohizei624( val, (pK + KBUYLINE12)->KKval2 );
		memcpy( (pK + KBUYLINE12)->KKval1, val, 6 );
	}

	// 仮払消費税(⑬C欄の金額)
	if( (*m_pTaxListData)->Head.KHstax ){
		if( !((pK + KBUYLINE13)->KKauto&0x04) ){
			::ZeroMemory( val, 6 );
//			Shz_shohizei63( val, (pK + KBUYLINE13)->KKval3 );
			Shz_shohizei10_100( val, (pK + KBUYLINE10)->KKval4 );
			memcpy( (pK + KBUYLINE13)->KKval3, val, 6 );
		}
	}

	if( !((pK + KBUYLINE13)->KKauto&0x02) ){
		if( (*m_pTaxListData)->Head.KHstax ){
			m_Arith.l_add( (pK + KBUYLINE13)->KKval2, (pK + KBUYLINE10)->KKval4, (pK + KBUYLINE13)->KKval3 );
		}
		else{
			memcpy( (pK + KBUYLINE13)->KKval2, (pK + KBUYLINE10)->KKval4, 6 );
		}
	}
	if( !((pK + KBUYLINE13)->KKauto&0x01) ){
		::ZeroMemory( val, 6 );
		Shz_shohizei78( val, (pK + KBUYLINE13)->KKval2 );
		memcpy( (pK + KBUYLINE13)->KKval1, val, 6 );
	}

// インボイス_23/07/28_higuchi cor -->
////----->('23.07.26 Add)
//	// 仮払消費税(⑭C欄の金額)
//	if ((*m_pTaxListData)->Head.KHstax) {
//		if (!((pK + KBUYLINE14)->KKauto & 0x04)) {
//			::ZeroMemory(val, 6);
//			//			Shz_shohizei63( val, (pK + KBUYLINE13)->KKval3 );
//			Shz_shohizei10_100(val, (pK + KBUYLINE10)->KKval4);
//			memcpy((pK + KBUYLINE14)->KKval3, val, 6);
//		}
//	}
//
//	if (!((pK + KBUYLINE14)->KKauto & 0x02)) {
//		if ((*m_pTaxListData)->Head.KHstax) {
//			m_Arith.l_add((pK + KBUYLINE14)->KKval2, (pK + KBUYLINE10)->KKval4, (pK + KBUYLINE14)->KKval3);
//		}
//		else {
//			memcpy((pK + KBUYLINE14)->KKval2, (pK + KBUYLINE10)->KKval4, 6);
//		}
//	}
//	if (!((pK + KBUYLINE14)->KKauto & 0x01)) {
//		::ZeroMemory(val, 6);
//		Shz_shohizei78(val, (pK + KBUYLINE14)->KKval2);
//		memcpy((pK + KBUYLINE14)->KKval1, val, 6);
//	}
////<-----
// -----------------------------------
	// 仮払消費税(⑬C欄の金額)
	if ((*m_pTaxListData)->Head.KHstax) {
		if (!((pK + KBUYLINE14)->KKauto & 0x04)) {
			::ZeroMemory(val, 6);
		//----->('23.10.17 tanaka cor)
			//Shz_shohizei8_100(val, (pK + KBUYLINE10)->KKval7);
		//------
			Shz_shohizei64_1016(val, (pK + KBUYLINE10)->KKval7);
		//<-----
			memcpy((pK + KBUYLINE14)->KKval3, val, 6);
		}
	}
	if (!((pK + KBUYLINE14)->KKauto & 0x02)) {
		if ((*m_pTaxListData)->Head.KHstax) {
			m_Arith.l_add((pK + KBUYLINE14)->KKval2, (pK + KBUYLINE10)->KKval7, (pK + KBUYLINE14)->KKval3);
		}
		else {
			memcpy((pK + KBUYLINE14)->KKval2, (pK + KBUYLINE10)->KKval7, 6);
		}
	}
	if (!((pK + KBUYLINE14)->KKauto & 0x01)) {
// 2023/10/05 higuchi 依頼No.163608
		//char	val2[6]={0};
		//::ZeroMemory(val2, 6);
		//Shz_shohizei624(val2, (pK + KBUYLINE14)->KKval2);
		//::ZeroMemory(val, 6);
		//Shz_shohizei80_100(val, val2);
		//memcpy((pK + KBUYLINE14)->KKval1, val, 6);

		char	ans[6]={0};
		char	src[6]={0};
		char	bs1[6]={0};
		char	bb1[6]={0};
		char	bs2[6]={0};
		char	bb2[6]={0};
		int		hasu=0;		// 切り捨て

		memcpy(src,(pK + KBUYLINE14)->KKval2,6);
		m_Arith.l_input(bs1,_T("624"));
		m_Arith.l_input(bb1,_T("10800"));
		m_Arith.l_input(bs2,_T("80"));
		m_Arith.l_input(bb2,_T("100"));
		l_pardotnet3(ans,src,bs1,bb1,bs2,bb2,hasu);
		memcpy((pK + KBUYLINE14)->KKval1,ans,6);
	}
	// 仮払消費税(⑭C欄の金額)
	if( (*m_pTaxListData)->Head.KHstax ){
		if( !((pK + KBUYLINE15)->KKauto&0x04) ){
			::ZeroMemory( val, 6 );
		//----->('23.10.17 tanaka cor)
			//Shz_shohizei10_100( val, (pK + KBUYLINE10)->KKval8 );
		//------
			Shz_shohizei80_1020( val, (pK + KBUYLINE10)->KKval8 );
		//<-----
			memcpy( (pK + KBUYLINE15)->KKval3, val, 6 );
		}
	}

	if( !((pK + KBUYLINE15)->KKauto&0x02) ){
		if( (*m_pTaxListData)->Head.KHstax ){
			m_Arith.l_add( (pK + KBUYLINE15)->KKval2, (pK + KBUYLINE10)->KKval8, (pK + KBUYLINE15)->KKval3 );
		}
		else{
			memcpy( (pK + KBUYLINE15)->KKval2, (pK + KBUYLINE10)->KKval8, 6 );
		}
	}
	if( !((pK + KBUYLINE15)->KKauto&0x01) ){
// 2023/10/05 higuchi 依頼No.163608
		//char	val2[6]={0};
		//::ZeroMemory( val2, 6 );
		//Shz_shohizei78( val2, (pK + KBUYLINE15)->KKval2 );
		//::ZeroMemory(val, 6);
		//Shz_shohizei80_100(val, val2);
		//memcpy( (pK + KBUYLINE15)->KKval1, val, 6 );

		char	ans[6]={0};
		char	src[6]={0};
		char	bs1[6]={0};
		char	bb1[6]={0};
		char	bs2[6]={0};
		char	bb2[6]={0};
		int		hasu=0;		// 切り捨て

		memcpy(src,(pK + KBUYLINE15)->KKval2,6);
		m_Arith.l_input(bs1,_T("780"));
		m_Arith.l_input(bb1,_T("11000"));
		m_Arith.l_input(bs2,_T("80"));
		m_Arith.l_input(bb2,_T("100"));
		l_pardotnet3(ans,src,bs1,bb1,bs2,bb2,hasu);
		memcpy((pK + KBUYLINE15)->KKval1,ans,6);
	}

// インボイス_23/07/28_higuchi cor <--
	//合計（⑭＋⑮）
//	::ZeroMemory( (pK + KBUYLINE16)->KKval1, 6 );
//	m_Arith.l_add( (pK + KBUYLINE16)->KKval1, (pK + KBUYLINE14)->KKval1, (pK + KBUYLINE15)->KKval1 );
	
	return 0;
}

//-----------------------------------------------------------------------------
// 課税売上高
//-----------------------------------------------------------------------------
// 返送値	
//-----------------------------------------------------------------------------
int	CShzTax31Calc::Shz_Sale31Calc()
{

	KKVAL*	pK = (*m_pTaxListData)->NRec.KUval;

	m_Arith.l_sub( (pK + KSELLINE6)->KKval1, (pK + KSELLINE4)->KKval1, (pK + KSELLINE5)->KKval1 );		//4-5
	m_Arith.l_sub( (pK + KSELLINE9)->KKval1, (pK + KSELLINE7)->KKval1, (pK + KSELLINE8)->KKval1 );		//7-8
	m_Arith.l_sub( (pK + KSELLINE6)->KKval2, (pK + KSELLINE4)->KKval2, (pK + KSELLINE5)->KKval2 );		//4-5
	m_Arith.l_sub( (pK + KSELLINE9)->KKval2, (pK + KSELLINE7)->KKval2, (pK + KSELLINE8)->KKval2 );		//7-8
	m_Arith.l_sub( (pK + KSELLINE6)->KKval3, (pK + KSELLINE4)->KKval3, (pK + KSELLINE5)->KKval3 );		//4-5
	m_Arith.l_sub( (pK + KSELLINE9)->KKval3, (pK + KSELLINE7)->KKval3, (pK + KSELLINE8)->KKval3 );		//7-8
	m_Arith.l_sub( (pK + KSELLINE6)->KKval4, (pK + KSELLINE4)->KKval4, (pK + KSELLINE5)->KKval4 );		//4-5
	m_Arith.l_sub( (pK + KSELLINE9)->KKval4, (pK + KSELLINE7)->KKval4, (pK + KSELLINE8)->KKval4 );		//7-8

	//課税売上高の合計額(①＋②＋③＋⑥＋⑨) A
	::ZeroMemory( (pK + KSELLINE10)->KKval1, 6 );
	m_Arith.l_add( (pK + KSELLINE10)->KKval1, (pK + KSELLINE1)->KKval1, (pK + KSELLINE2)->KKval1 );
	m_Arith.l_add( (pK + KSELLINE10)->KKval1, (pK + KSELLINE10)->KKval1, (pK + KSELLINE3)->KKval1 );
	m_Arith.l_add( (pK + KSELLINE10)->KKval1, (pK + KSELLINE10)->KKval1, (pK + KSELLINE6)->KKval1 );
	m_Arith.l_add( (pK + KSELLINE10)->KKval1, (pK + KSELLINE10)->KKval1, (pK + KSELLINE9)->KKval1 );

	//課税売上高の合計額(①＋②＋③＋⑥＋⑨) B
	::ZeroMemory((pK + KSELLINE10)->KKval2, 6);
	m_Arith.l_add((pK + KSELLINE10)->KKval2, (pK + KSELLINE1)->KKval2, (pK + KSELLINE2)->KKval2);
	m_Arith.l_add((pK + KSELLINE10)->KKval2, (pK + KSELLINE10)->KKval2, (pK + KSELLINE3)->KKval2);
	m_Arith.l_add((pK + KSELLINE10)->KKval2, (pK + KSELLINE10)->KKval2, (pK + KSELLINE6)->KKval2);
	m_Arith.l_add((pK + KSELLINE10)->KKval2, (pK + KSELLINE10)->KKval2, (pK + KSELLINE9)->KKval2);

	//課税売上高の合計額(①＋②＋③＋⑥＋⑨) C
	::ZeroMemory((pK + KSELLINE10)->KKval3, 6);
	m_Arith.l_add((pK + KSELLINE10)->KKval3, (pK + KSELLINE1)->KKval3, (pK + KSELLINE2)->KKval3);
	m_Arith.l_add((pK + KSELLINE10)->KKval3, (pK + KSELLINE10)->KKval3, (pK + KSELLINE3)->KKval3);
	m_Arith.l_add((pK + KSELLINE10)->KKval3, (pK + KSELLINE10)->KKval3, (pK + KSELLINE6)->KKval3);
	m_Arith.l_add((pK + KSELLINE10)->KKval3, (pK + KSELLINE10)->KKval3, (pK + KSELLINE9)->KKval3);

	//課税売上高の合計額(①＋②＋③＋⑥＋⑨) D
	::ZeroMemory((pK + KSELLINE10)->KKval4, 6);
	m_Arith.l_add((pK + KSELLINE10)->KKval4, (pK + KSELLINE1)->KKval4, (pK + KSELLINE2)->KKval4);
	m_Arith.l_add((pK + KSELLINE10)->KKval4, (pK + KSELLINE10)->KKval4, (pK + KSELLINE3)->KKval4);
	m_Arith.l_add((pK + KSELLINE10)->KKval4, (pK + KSELLINE10)->KKval4, (pK + KSELLINE6)->KKval4);
	m_Arith.l_add((pK + KSELLINE10)->KKval4, (pK + KSELLINE10)->KKval4, (pK + KSELLINE9)->KKval4);

	char	val[6];

	// 仮払消費税(⑪B欄の金額)
	if( (*m_pTaxListData)->Head.KHstax ){
		if( !((pK + KSELLINE11)->KKauto&0x04) ){
			::ZeroMemory( val, 6 );
			Shz_shohizei8_100( val, (pK + KSELLINE10)->KKval2 );
			memcpy( (pK + KSELLINE11)->KKval3, val, 6 );
		}
	}

	if( !((pK + KSELLINE11)->KKauto&0x02) ){
		if( (*m_pTaxListData)->Head.KHstax ){
			m_Arith.l_add( (pK + KSELLINE11)->KKval2, (pK + KSELLINE10)->KKval2, (pK + KSELLINE11)->KKval3 );
		}
		else{
			memcpy( (pK + KSELLINE11)->KKval2, (pK + KSELLINE10)->KKval2, 6 );
		}
	}
	if( !((pK + KSELLINE11)->KKauto&0x01) ){
		::ZeroMemory( val, 6 );
		Shz_shohizei100_108( val, (pK + KSELLINE11)->KKval2 );
		memcpy( (pK + KSELLINE11)->KKval1, val, 6 );
	}

	// 仮払消費税(⑫C欄の金額)
	if( (*m_pTaxListData)->Head.KHstax ){
		if( !((pK + KSELLINE12)->KKauto&0x04) ){
			::ZeroMemory( val, 6 );
			Shz_shohizei8_100( val, (pK + KSELLINE10)->KKval3 );
			memcpy( (pK + KSELLINE12)->KKval3, val, 6 );
		}
	}

	if( !((pK + KSELLINE12)->KKauto&0x02) ){
		if( (*m_pTaxListData)->Head.KHstax ){
			m_Arith.l_add( (pK + KSELLINE12)->KKval2, (pK + KSELLINE10)->KKval3, (pK + KSELLINE12)->KKval3 );
		}
		else{
			memcpy( (pK + KSELLINE12)->KKval2, (pK + KSELLINE10)->KKval3, 6 );
		}
	}
	if( !((pK + KSELLINE12)->KKauto&0x01) ){
		::ZeroMemory( val, 6 );
		Shz_shohizei100_108( val, (pK + KSELLINE12)->KKval2 );
		memcpy( (pK + KSELLINE12)->KKval1, val, 6 );
	}

	// 仮払消費税(⑬D欄の金額)
	if ((*m_pTaxListData)->Head.KHstax) {
		if (!((pK + KSELLINE13)->KKauto & 0x04)) {
			::ZeroMemory(val, 6);
			Shz_shohizei10_100(val, (pK + KSELLINE10)->KKval4);
			memcpy((pK + KSELLINE13)->KKval3, val, 6);
		}
	}

	if (!((pK + KSELLINE13)->KKauto & 0x02)) {
		if ((*m_pTaxListData)->Head.KHstax) {
			m_Arith.l_add((pK + KSELLINE13)->KKval2, (pK + KSELLINE10)->KKval4, (pK + KSELLINE13)->KKval3);
		}
		else {
			memcpy((pK + KSELLINE13)->KKval2, (pK + KSELLINE10)->KKval4, 6);
		}
	}
	if (!((pK + KSELLINE13)->KKauto & 0x01)) {
		::ZeroMemory(val, 6);
		Shz_shohizei100_110(val, (pK + KSELLINE13)->KKval2);
		memcpy((pK + KSELLINE13)->KKval1, val, 6);
	}

	//合計（⑭＋⑮）
//	::ZeroMemory( (pK + KSELLINE16)->KKval1, 6 );
//	m_Arith.l_add( (pK + KSELLINE16)->KKval1, (pK + KSELLINE14)->KKval1, (pK + KSELLINE15)->KKval1 );


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
int	CShzTax31Calc::Shz_shohizei4( char* ans, char* val ) //Shz_sicalcと同じ
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
int	CShzTax31Calc::Shz_shohizei63( char* ans, char* val )
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
//税率６．２４％計算
//-----------------------------------------------------------------------------
// 引数	ans	：	
//		val	：	
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzTax31Calc::Shz_shohizei624(char* ans, char* val)
{
	char	PW1[6], PW2[6];

	//	m_Arith.l_input( PW1, _T("6.3") );
	//	m_Arith.l_input( PW2, _T("108") );
	m_Arith.l_input(PW1, _T("624"));
	m_Arith.l_input(PW2, _T("10800"));
	m_Arith.l_mul(ans, val, PW1);
	m_Arith.l_div(ans, ans, PW2);

	return 0;
}

//-----------------------------------------------------------------------------
//税率７．８％計算
//-----------------------------------------------------------------------------
// 引数	ans	：	
//		val	：	
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzTax31Calc::Shz_shohizei78(char* ans, char* val)
{
	char	PW1[6], PW2[6];

	//	m_Arith.l_input( PW1, _T("6.3") );
	//	m_Arith.l_input( PW2, _T("108") );
	m_Arith.l_input(PW1, _T("78"));
	m_Arith.l_input(PW2, _T("1100"));
	m_Arith.l_mul(ans, val, PW1);
	m_Arith.l_div(ans, ans, PW2);

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
int	CShzTax31Calc::Shz_shohizei100_105( char* ans, char* val ) //Shz_urcalcと同じ
{
	char	PW1[6], PW2[6];

	m_Arith.l_input( PW1, _T("100") );
	m_Arith.l_input( PW2, _T("105") );
	m_Arith.l_mul( ans, val, PW1 );
	m_Arith.l_div( ans, ans, PW2 );

	return 0;
}

//-----------------------------------------------------------------------------
//税率６．３％/６．２４％計算
//-----------------------------------------------------------------------------
// 引数	ans	：	
//		val	：	
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzTax31Calc::Shz_shohizei100_108( char* ans, char* val )
{
	char	PW1[6], PW2[6];

	m_Arith.l_input( PW1, _T("100") );
	m_Arith.l_input( PW2, _T("108") );
	m_Arith.l_mul( ans, val, PW1 );
	m_Arith.l_div( ans, ans, PW2 );

	return 0;
}
//-----------------------------------------------------------------------------
//税率７．８％計算
//-----------------------------------------------------------------------------
// 引数	ans	：	
//		val	：	
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzTax31Calc::Shz_shohizei100_110(char* ans, char* val) //Shz_urcalcと同じ
{
	char	PW1[6], PW2[6];

	m_Arith.l_input(PW1, _T("100"));
	m_Arith.l_input(PW2, _T("110"));
	m_Arith.l_mul(ans, val, PW1);
	m_Arith.l_div(ans, ans, PW2);

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
int	CShzTax31Calc::Shz_shohizei5_100( char* ans, char* val ) //Shz_shohizeiと同じ
{
	char	PW1[6], PW2[6];

	m_Arith.l_input( PW1, _T("5") );
	m_Arith.l_input( PW2, _T("100") );
	m_Arith.l_mul( ans, val, PW1 );
	m_Arith.l_div( ans, ans, PW2 );

	return 0;
}

//-----------------------------------------------------------------------------
//税率６．３％/６．２４％計算
//-----------------------------------------------------------------------------
// 引数	ans	：	
//		val	：	
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzTax31Calc::Shz_shohizei8_100( char* ans, char* val )
{
	char	PW1[6], PW2[6];

	m_Arith.l_input( PW1, _T("8") );
	m_Arith.l_input( PW2, _T("100") );
	m_Arith.l_mul( ans, val, PW1 );
	m_Arith.l_div( ans, ans, PW2 );

	return 0;
}

// インボイス_23/07/28_higuchi add -->
//-----------------------------------------------------------------------------
//免税事業者８０％計算
//-----------------------------------------------------------------------------
// 引数	ans	：	
//		val	：	
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzTax31Calc::Shz_shohizei80_100( char* ans, char* val )
{
	char	PW1[6], PW2[6];

	m_Arith.l_input( PW1, _T("80") );
	m_Arith.l_input( PW2, _T("100") );
	m_Arith.l_mul( ans, val, PW1 );
	m_Arith.l_div( ans, ans, PW2 );

	return 0;
}
// インボイス_23/07/28_higuchi add <--

//----->('23.10.17 tanaka Add)
//-----------------------------------------------------------------------------
//免税事業者８０％、税率６．３％計算
//-----------------------------------------------------------------------------
// 引数	ans	：	
//		val	：	
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzTax31Calc::Shz_shohizei64_1016(char* ans, char* val)
{
	char	PW1[6], PW2[6];

	m_Arith.l_input(PW1, _T("64"));
	m_Arith.l_input(PW2, _T("1016"));
	m_Arith.l_mul(ans, val, PW1);
	m_Arith.l_div(ans, ans, PW2);

	return 0;
}

//-----------------------------------------------------------------------------
//免税事業者８０％、税率６．３％計算
//-----------------------------------------------------------------------------
// 引数	ans	：	
//		val	：	
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzTax31Calc::Shz_shohizei80_1020(char* ans, char* val)
{
	char	PW1[6], PW2[6];

	m_Arith.l_input(PW1, _T("80"));
	m_Arith.l_input(PW2, _T("1020"));
	m_Arith.l_mul(ans, val, PW1);
	m_Arith.l_div(ans, ans, PW2);

	return 0;
}
//<-----

//-----------------------------------------------------------------------------
//税率７．８％計算
//-----------------------------------------------------------------------------
// 引数	ans	：	
//		val	：	
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzTax31Calc::Shz_shohizei10_100(char* ans, char* val)
{
	char	PW1[6], PW2[6];

	m_Arith.l_input(PW1, _T("10"));
	m_Arith.l_input(PW2, _T("100"));
	m_Arith.l_mul(ans, val, PW1);
	m_Arith.l_div(ans, ans, PW2);

	return 0;
}

//'14.08.21
//-----------------------------------------------------------------------------
// 計算表全集計
//-----------------------------------------------------------------------------
// 引数	mode	：	どの個人業種の集計
//-----------------------------------------------------------------------------
// 返送値		：
//-----------------------------------------------------------------------------
int	CShzTax31Calc::Shz_31AllCalc( int mode )
{

	if( Util.IsUnConnectMst(m_pZmSub->zvol) && (Util.GetKazeihoushiki(m_pZmSub->zvol)==ID_ICSSH_KANNI) ){// 非連動ｄｂで簡易課税
		if ((*m_pSnHeadData)->SVmzsw == 1) {
			Shz_KniKeiInput();
			Shz_KniKeiPost();
		}
		else {
			Shz_KniKeiInput();
			Shz_KniKeiPost();
//			Shz_KniInput();
//			Shz_KniPost();
		}
	}
	else{
		switch( mode ){
			case 1:
				Shz_Jigyo31Calc();
				break;
			case 2:	
				Shz_Fudoh31Calc();
				break;
			case 3:	
				Shz_Nohgyo31Calc();	
				break;
			default:
				Shz_Jigyo31Calc();
				Shz_Fudoh31Calc();
				Shz_Nohgyo31Calc();
				break;
		}
		Shz_Sale31Post();
		Shz_Buy31Post();
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
int	CShzTax31Calc::Shz_Fudoh31Calc()
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
	::ZeroMemory( pK->KKval4, 6 );//Dの欄（うち税率６．３％適用分）
	::ZeroMemory( pK->KKval5, 6 );//Eの欄（うち税率６．２４％適用分）
	::ZeroMemory( pK->KKval6, 6 );//Fの欄（うち税率７．８％適用分）
	for( int i = KFUDLINE1 ; i < KFUDLINE4 ; i++ ){
		m_Arith.l_add( pK->KKval1, pK->KKval1, (*m_pTaxListData)->NRec.KFval[i].KKval1 );
		m_Arith.l_add( pK->KKval2, pK->KKval2, (*m_pTaxListData)->NRec.KFval[i].KKval2 );

		//---->yoshida201216
		if (i == KFUDLINE1 || i == KFUDLINE2) {
			if (((*m_pSnHeadData)->m_DispTabSgn & 0x1000) && !((*m_pSnHeadData)->Sn_Sign4 & 0x01)) {//新様式チェックあり かつ入力ロックじゃない時
				//チェックある場合は該当箇所の6.24％部分は斜線になる為集計しない（金額クリアする）
				::ZeroMemory((*m_pTaxListData)->NRec.KFval[i].KKval5, 6);
			}

			// 簡易課税では6.3%を常に表示するため計算する
			//if ((*m_pSnHeadData)->SVmzsw == 1) {
			//	m_Arith.l_add(pK->KKval4, pK->KKval4, (*m_pTaxListData)->NRec.KFval[i].KKval4);//Dの欄（うち税率６．３％適用分）
			//}
			if ( ( Util.GetKazeihoushiki ( m_pZmSub->zvol ) == ID_ICSSH_KANNI ) || ( ( *m_pSnHeadData )->SVmzsw == 1 ) ) {
				// 簡易課税、もしくは旧税率が存在する場合
				m_Arith.l_add ( pK->KKval4, pK->KKval4, ( *m_pTaxListData )->NRec.KFval[i].KKval4 );
			}

			m_Arith.l_add(pK->KKval5, pK->KKval5, (*m_pTaxListData)->NRec.KFval[i].KKval5);//Eの欄（うち税率６．２４％適用分）
			m_Arith.l_add(pK->KKval6, pK->KKval6, (*m_pTaxListData)->NRec.KFval[i].KKval6);//Fの欄（うち税率７．８％適用分）

		}
		//<-------------------
		else {
			// 簡易課税では6.3%を常に表示するため計算する
			//if ((*m_pSnHeadData)->SVmzsw == 1) {
			//	m_Arith.l_add(pK->KKval4, pK->KKval4, (*m_pTaxListData)->NRec.KFval[i].KKval4);//Dの欄（うち税率６．３％適用分）
			//}
			if ( ( Util.GetKazeihoushiki ( m_pZmSub->zvol ) == ID_ICSSH_KANNI ) || ( ( *m_pSnHeadData )->SVmzsw == 1 ) ) {
				// 簡易課税、もしくは旧税率が存在する場合
				m_Arith.l_add ( pK->KKval4, pK->KKval4, ( *m_pTaxListData )->NRec.KFval[i].KKval4 );
			}

			m_Arith.l_add(pK->KKval5, pK->KKval5, (*m_pTaxListData)->NRec.KFval[i].KKval5);//Eの欄（うち税率６．２４％適用分）
			m_Arith.l_add(pK->KKval6, pK->KKval6, (*m_pTaxListData)->NRec.KFval[i].KKval6);//Fの欄（うち税率７．８％適用分）
		}
	}
	Shz_LinCalc( pK, 0 );//Cをセット

	// 必要経費　計(14)
	pK = &(*m_pTaxListData)->NRec.KFval[KFUDLINE14];
	::ZeroMemory( pK->KKval1, 6 );
	::ZeroMemory( pK->KKval2, 6 );
	::ZeroMemory( pK->KKval4, 6 );//Dの欄（うち税率６．３％適用分）
	::ZeroMemory( pK->KKval5, 6 );//Eの欄（うち税率６．２４％適用分）
	::ZeroMemory( pK->KKval6, 6 );//Fの欄（うち税率７．８％適用分）
// インボイス_23/07/24_higuchi add -->
	::ZeroMemory( pK->KKval7, 6 );
	::ZeroMemory( pK->KKval8, 6 );
// インボイス_23/07/24_higuchi add <--
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

		//---->yoshida201216
		else if ( i == KFUDLINE5 || i == KFUDLINE7 || i == KFUDLINE10 || i == KFUDLINE11) {
			if (((*m_pSnHeadData)->m_DispTabSgn & 0x1000) && !((*m_pSnHeadData)->Sn_Sign4 & 0x01)) {//新様式チェックあり かつ入力ロックじゃない時
				//チェックある場合は該当箇所の6.24％部分は斜線になる為集計しない（金額クリアする）
				::ZeroMemory((*m_pTaxListData)->NRec.KFval[i].KKval5, 6);
// インボイス_23/07/24_higuchi add -->
				::ZeroMemory((*m_pTaxListData)->NRec.KFval[i].KKval7, 6);
// インボイス_23/07/24_higuchi add <--
			}

			// 簡易課税では6.3%を常に表示するため計算する
			//if ((*m_pSnHeadData)->SVmzsw == 1) {
			//	m_Arith.l_add(pK->KKval4, pK->KKval4, (*m_pTaxListData)->NRec.KFval[i].KKval4);//Dの欄（うち税率６．３％適用分）
			//}
			if ( ( Util.GetKazeihoushiki ( m_pZmSub->zvol ) == ID_ICSSH_KANNI ) || ( ( *m_pSnHeadData )->SVmzsw == 1 ) ) {
				// 簡易課税、もしくは旧税率が存在する場合
				m_Arith.l_add ( pK->KKval4, pK->KKval4, ( *m_pTaxListData )->NRec.KFval[i].KKval4 );
			}

			m_Arith.l_add(pK->KKval5, pK->KKval5, (*m_pTaxListData)->NRec.KFval[i].KKval5);//Eの欄（うち税率６．２４％適用分）
			m_Arith.l_add(pK->KKval6, pK->KKval6, (*m_pTaxListData)->NRec.KFval[i].KKval6);//Fの欄（うち税率７．８％適用分）
// インボイス_23/07/24_higuchi add -->
			m_Arith.l_add(pK->KKval7, pK->KKval7, (*m_pTaxListData)->NRec.KFval[i].KKval7);
			m_Arith.l_add(pK->KKval8, pK->KKval8, (*m_pTaxListData)->NRec.KFval[i].KKval8);
// インボイス_23/07/24_higuchi add <--

		}
		//<-------------------


		else{
			// 簡易課税では6.3%を常に表示するため計算する
			//if ((*m_pSnHeadData)->SVmzsw == 1) {
			//	m_Arith.l_add(pK->KKval4, pK->KKval4, (*m_pTaxListData)->NRec.KFval[i].KKval4);//Dの欄（うち税率６．３％適用分）
			//}
			if ( ( Util.GetKazeihoushiki ( m_pZmSub->zvol ) == ID_ICSSH_KANNI ) || ( ( *m_pSnHeadData )->SVmzsw == 1 ) ) {
				// 簡易課税、もしくは旧税率が存在する場合
				m_Arith.l_add ( pK->KKval4, pK->KKval4, ( *m_pTaxListData )->NRec.KFval[i].KKval4 );
			}

			m_Arith.l_add( pK->KKval5, pK->KKval5, (*m_pTaxListData)->NRec.KFval[i].KKval5 );//Eの欄（うち税率６．２４％適用分）
			m_Arith.l_add( pK->KKval6, pK->KKval6, (*m_pTaxListData)->NRec.KFval[i].KKval6 );//Fの欄（うち税率７．８％適用分）
// インボイス_23/07/24_higuchi add -->
			m_Arith.l_add( pK->KKval7, pK->KKval7, (*m_pTaxListData)->NRec.KFval[i].KKval7 );
			m_Arith.l_add( pK->KKval8, pK->KKval8, (*m_pTaxListData)->NRec.KFval[i].KKval8 );
// インボイス_23/07/24_higuchi add <--
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
int	CShzTax31Calc::Shz_Jigyo31Calc()
{
	if( ((*m_pTaxListData)->GKBN==0x02) || ((*m_pTaxListData)->GKBN==0x04) ){
		return 0;
	}
	int	mode;
	for( int i=0; i< JIGLINCNT; i++ ){
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
	::ZeroMemory( pK->KKval4, 6 );//Dの欄（うち税率６．３％適用分）
	::ZeroMemory( pK->KKval5, 6 );//Eの欄（うち税率６．２４％適用分）
// 22-0167,22-0361 del -->
	//::ZeroMemory( pK->KKval6, 6 );//Fの欄（うち税率７．８％適用分）
// 22-0167,22-0361 del <--
// 230727Aw add -->
	::ZeroMemory( pK->KKval7, 6 );	// 経過措置適用6.24％
// 230727Aw add <--

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

// 22-0167,22-0361 del -->
	// 自動計算の場合、Shz_LinCalcで横計が行われるのでクリアはここで行う
	::ZeroMemory(pK->KKval6,6);//Fの欄（うち税率７．８％適用分）
// 22-0167,22-0361 del <--
// 230727Aw add -->
	::ZeroMemory( pK->KKval8, 6 );	// 経過措置適用7.8％
// 230727Aw add <--

	for( int i = KJIGLINE8 ; i < KJIGLINE32 ; i++ ){
		if( i == KJIGLINE15 || i == KJIGLINE18 || i == KJIGLINE22 || i == KJIGLINE24 ){//損害保険料、減価償却費、利子割引料、貸倒金はスキップ
			continue;
		}

		//---->yoshida201216
		else if (i == KJIGLINE8 || i == KJIGLINE9 || i == KJIGLINE10 || i == KJIGLINE11 || i == KJIGLINE16 || i == KJIGLINE20 || i == KJIGLINE21 || i == KJIGLINE23) {
			if (((*m_pSnHeadData)->m_DispTabSgn & 0x1000) && !((*m_pSnHeadData)->Sn_Sign4 & 0x01)) {//新様式チェックあり かつ入力ロックじゃない時
				//チェックある場合は該当箇所の6.24％部分は斜線になる為集計しない（金額クリアする）
				::ZeroMemory((*m_pTaxListData)->NRec.KJval[i].KKval5, 6);
// インボイス_23/07/24_higuchi add -->
				::ZeroMemory((*m_pTaxListData)->NRec.KJval[i].KKval7, 6);
// インボイス_23/07/24_higuchi add <--
			}
			//Dの欄（うち税率６．３％適用分）
			// 簡易課税では6.3%を常に表示するため計算する
			//if ((*m_pSnHeadData)->SVmzsw == 1) {
			//	m_Arith.l_add(pK->KKval4, pK->KKval4, (*m_pTaxListData)->NRec.KJval[i].KKval4);
			//}
			if ( ( Util.GetKazeihoushiki ( m_pZmSub->zvol ) == ID_ICSSH_KANNI ) || ( ( *m_pSnHeadData )->SVmzsw == 1 ) ) {
				// 簡易課税、もしくは旧税率が存在する場合
				m_Arith.l_add ( pK->KKval4, pK->KKval4, ( *m_pTaxListData )->NRec.KJval[i].KKval4 );
			}
			m_Arith.l_add(pK->KKval5, pK->KKval5, (*m_pTaxListData)->NRec.KJval[i].KKval5);//Eの欄（うち税率６．２４％適用分）
			m_Arith.l_add(pK->KKval6, pK->KKval6, (*m_pTaxListData)->NRec.KJval[i].KKval6);//Fの欄（うち税率７．８％適用分）
// インボイス_23/07/24_higuchi add -->
			m_Arith.l_add(pK->KKval7, pK->KKval7, (*m_pTaxListData)->NRec.KJval[i].KKval7);
			m_Arith.l_add(pK->KKval8, pK->KKval8, (*m_pTaxListData)->NRec.KJval[i].KKval8);
// インボイス_23/07/24_higuchi add <--

		}
		//<-------------------

		else{
			//Dの欄（うち税率６．３％適用分）
			// 簡易課税では6.3%を常に表示するため計算する
			//if ((*m_pSnHeadData)->SVmzsw == 1) {
			//	m_Arith.l_add(pK->KKval4, pK->KKval4, (*m_pTaxListData)->NRec.KJval[i].KKval4);
			//}
			if ( ( Util.GetKazeihoushiki ( m_pZmSub->zvol ) == ID_ICSSH_KANNI ) || ( ( *m_pSnHeadData )->SVmzsw == 1 ) ) {
				// 簡易課税、もしくは旧税率が存在する場合
				m_Arith.l_add ( pK->KKval4, pK->KKval4, ( *m_pTaxListData )->NRec.KJval[i].KKval4 );
			}

			m_Arith.l_add( pK->KKval5, pK->KKval5, (*m_pTaxListData)->NRec.KJval[i].KKval5 );//Eの欄（うち税率６．２４％適用分）
			m_Arith.l_add( pK->KKval6, pK->KKval6, (*m_pTaxListData)->NRec.KJval[i].KKval6 );//Fの欄（うち税率７．８％適用分）
// インボイス_23/07/24_higuchi add -->
			m_Arith.l_add( pK->KKval7, pK->KKval7, (*m_pTaxListData)->NRec.KJval[i].KKval7 );
			m_Arith.l_add( pK->KKval8, pK->KKval8, (*m_pTaxListData)->NRec.KJval[i].KKval8 );
// インボイス_23/07/24_higuchi add <--

		}
	}


	// 差引金額(33)A
	pK = &(*m_pTaxListData)->NRec.KJval[KJIGLINE33];
	m_Arith.l_sub( pK->KKval1, (*m_pTaxListData)->NRec.KJval[KJIGLINE7].KKval1, (*m_pTaxListData)->NRec.KJval[KJIGLINE32].KKval1 );

	// 3 + 32(34)
	pK = &(*m_pTaxListData)->NRec.KJval[KJIGLINE34];
	m_Arith.l_add( pK->KKval1, (*m_pTaxListData)->NRec.KJval[KJIGLINE3].KKval1, (*m_pTaxListData)->NRec.KJval[KJIGLINE32].KKval1 );
	m_Arith.l_add( pK->KKval3, (*m_pTaxListData)->NRec.KJval[KJIGLINE3].KKval3, (*m_pTaxListData)->NRec.KJval[KJIGLINE32].KKval3 );

	// 簡易課税では6.3%を常に表示するため計算する
	//if ( ( *m_pSnHeadData )->SVmzsw == 1 ) {
	//	m_Arith.l_add( pK->KKval4, (*m_pTaxListData)->NRec.KJval[KJIGLINE3].KKval4, (*m_pTaxListData)->NRec.KJval[KJIGLINE32].KKval4 );
	//}
	if ( ( Util.GetKazeihoushiki ( m_pZmSub->zvol ) == ID_ICSSH_KANNI ) || ( ( *m_pSnHeadData )->SVmzsw == 1 ) ) {
		// 簡易課税、もしくは旧税率が存在する場合
		m_Arith.l_add ( pK->KKval4, ( *m_pTaxListData )->NRec.KJval[KJIGLINE3].KKval4, ( *m_pTaxListData )->NRec.KJval[KJIGLINE32].KKval4 );
	}
	else {
		::ZeroMemory(pK->KKval4, 6);
	}
	m_Arith.l_add( pK->KKval5, (*m_pTaxListData)->NRec.KJval[KJIGLINE3].KKval5, (*m_pTaxListData)->NRec.KJval[KJIGLINE32].KKval5 );
	m_Arith.l_add( pK->KKval6, (*m_pTaxListData)->NRec.KJval[KJIGLINE3].KKval6, (*m_pTaxListData)->NRec.KJval[KJIGLINE32].KKval6 );
// インボイス_23/07/24_higuchi add -->
	m_Arith.l_add( pK->KKval7, (*m_pTaxListData)->NRec.KJval[KJIGLINE3].KKval7, (*m_pTaxListData)->NRec.KJval[KJIGLINE32].KKval7 );
	m_Arith.l_add( pK->KKval8, (*m_pTaxListData)->NRec.KJval[KJIGLINE3].KKval8, (*m_pTaxListData)->NRec.KJval[KJIGLINE32].KKval8 );
// インボイス_23/07/24_higuchi add <--

	return 0;
}


//-----------------------------------------------------------------------------
// 農業所得
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzTax31Calc::Shz_Nohgyo31Calc()
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
// 22-0167,22-0361 add -->
				// 6.24%,7.8%の自動計算のみ行う
				// ※旧様式は関数の中で対象外としている
				mode=3;
				break;
// 22-0167,22-0361 add <--

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
	::ZeroMemory( pK->KKval4, 6 );//Dの欄（うち税率６．３％適用分）
	::ZeroMemory( pK->KKval5, 6 );//Eの欄（うち税率６．２４％適用分）
	::ZeroMemory( pK->KKval6, 6 );//Fの欄（うち税率７．８％適用分）

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

		// 簡易課税では常に6.3%を表示するため計算する
		//if ((*m_pSnHeadData)->SVmzsw == 1) {
		//	m_Arith.l_add(pK->KKval4, pK->KKval4, (*m_pTaxListData)->NRec.KNval[i].KKval4);
		//}
		if ( ( Util.GetKazeihoushiki ( m_pZmSub->zvol ) == ID_ICSSH_KANNI ) || ( ( *m_pSnHeadData )->SVmzsw == 1 ) ) {
			// 簡易課税、もしくは旧税率が存在する場合
			m_Arith.l_add ( pK->KKval4, pK->KKval4, ( *m_pTaxListData )->NRec.KNval[i].KKval4 );
		}

		m_Arith.l_add( pK->KKval5, pK->KKval5, (*m_pTaxListData)->NRec.KNval[i].KKval5 );
		m_Arith.l_add( pK->KKval6, pK->KKval6, (*m_pTaxListData)->NRec.KNval[i].KKval6 );
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
// 22-0167,22-0361 del -->
	//::ZeroMemory( pK->KKval6, 6 );
// 22-0167,22-0361 del <--
// 230727Aw add -->
	::ZeroMemory( pK->KKval7, 6 );	// 経過措置適用　6.24％
// 230727Aw add <--
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

// 22-0167,22-0361 del -->
	// 自動計算の場合、Shz_LinCalcで横計が行われるのでクリアはここで行う
	::ZeroMemory(pK->KKval6,6);//Fの欄（うち税率７．８％適用分）
// 22-0167,22-0361 del <--
// 230727Aw add -->
	::ZeroMemory( pK->KKval8, 6 );	// 経過措置適用7.8％
// 230727Aw add <--

	for( int i = KNOULINE8 ; i < KNOULINE31 ; i++ ){//'14.09.01
		if(i == KNOULINE19 || i == KNOULINE20 || i == KNOULINE23 || i == KNOULINE26){//農業共済掛金、減価償却費、利子割引料、貸倒金、はスキップ（入力不可の為）
			continue;
		}
		//---->yoshida201216
		else if (i == KNOULINE8 || i == KNOULINE10 || i == KNOULINE13 || i == KNOULINE14 || i == KNOULINE15 || i == KNOULINE16 ||
			i == KNOULINE17 || i == KNOULINE18 || i == KNOULINE21 || i == KNOULINE22 || i == KNOULINE24 || i == KNOULINE25) {
			if (((*m_pSnHeadData)->m_DispTabSgn & 0x1000) && !((*m_pSnHeadData)->Sn_Sign4 & 0x01)) {//新様式チェックあり かつ入力ロックじゃない時
				//チェックある場合は該当箇所の6.24％部分は斜線になる為集計しない（金額クリアする）
				::ZeroMemory((*m_pTaxListData)->NRec.KNval[i].KKval5, 6);
// インボイス_23/07/24_higuchi add -->
				::ZeroMemory((*m_pTaxListData)->NRec.KNval[i].KKval7, 6);
// インボイス_23/07/24_higuchi add <--
			}

			// 簡易課税では常に6.3%を表示するため計算する
			//if ((*m_pSnHeadData)->SVmzsw == 1) {
			//	m_Arith.l_add(pK->KKval4, pK->KKval4, (*m_pTaxListData)->NRec.KNval[i].KKval4);//Dの欄（うち税率６．３％適用分）
			//}
			if ( ( Util.GetKazeihoushiki ( m_pZmSub->zvol ) == ID_ICSSH_KANNI ) || ( ( *m_pSnHeadData )->SVmzsw == 1 ) ) {
				// 簡易課税、もしくは旧税率が存在する場合
				m_Arith.l_add ( pK->KKval4, pK->KKval4, ( *m_pTaxListData )->NRec.KNval[i].KKval4 );
			}

			m_Arith.l_add(pK->KKval5, pK->KKval5, (*m_pTaxListData)->NRec.KNval[i].KKval5);//Eの欄（うち税率６．２４％適用分）
			m_Arith.l_add(pK->KKval6, pK->KKval6, (*m_pTaxListData)->NRec.KNval[i].KKval6);//Fの欄（うち税率７．８％適用分）
// インボイス_23/07/24_higuchi add -->
			m_Arith.l_add(pK->KKval7, pK->KKval7, (*m_pTaxListData)->NRec.KNval[i].KKval7);
			m_Arith.l_add(pK->KKval8, pK->KKval8, (*m_pTaxListData)->NRec.KNval[i].KKval8);
// インボイス_23/07/24_higuchi add <--
		}
		//<-----
		else{
			// 簡易課税では常に6.3%を表示するため計算する
			//if ((*m_pSnHeadData)->SVmzsw == 1) {
			//	m_Arith.l_add(pK->KKval4, pK->KKval4, (*m_pTaxListData)->NRec.KNval[i].KKval4);//Dの欄（うち税率６．３％適用分）
			//}
			if ( ( Util.GetKazeihoushiki ( m_pZmSub->zvol ) == ID_ICSSH_KANNI ) || ( ( *m_pSnHeadData )->SVmzsw == 1 ) ) {
				// 簡易課税、もしくは旧税率が存在する場合
				m_Arith.l_add ( pK->KKval4, pK->KKval4, ( *m_pTaxListData )->NRec.KNval[i].KKval4 );
			}

			m_Arith.l_add( pK->KKval5, pK->KKval5, (*m_pTaxListData)->NRec.KNval[i].KKval5 );//Eの欄（うち税率６．２４％適用分）
			m_Arith.l_add( pK->KKval6, pK->KKval6, (*m_pTaxListData)->NRec.KNval[i].KKval6 );//Fの欄（うち税率７．８％適用分）
// インボイス_23/07/24_higuchi add -->
			m_Arith.l_add( pK->KKval7, pK->KKval7, (*m_pTaxListData)->NRec.KNval[i].KKval7 );
			m_Arith.l_add( pK->KKval8, pK->KKval8, (*m_pTaxListData)->NRec.KNval[i].KKval8 );
// インボイス_23/07/24_higuchi add <--
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
int	CShzTax31Calc::Shz_KniKeiInput()
{

	KKVAL*	pK = (*m_pTaxListData)->NRec.KUval;
	int		id;
//	char	val[6][6];
	char	val[12][6];
// 22-0167,22-0361 add -->
	int		mode2=0;
// 22-0167,22-0361 add <--

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
// 22-0167,22-0361 cor -->
		//Shz_LinCalc( (pK+id), 0 );
// -----------------------
		if(i == 3 || i == 9)	mode2=2;	// 【農業事業者】は自動計算をしない
		else					mode2=1;
		Shz_LinCalc((pK+id),0,mode2);
// 22-0167,22-0361 cor <--
		m_Arith.l_add( val[0], val[0], (pK+id)->KKval1 );
		m_Arith.l_add( val[1], val[1], (pK+id)->KKval2 );
		m_Arith.l_add( val[2], val[2], (pK+id)->KKval3 );
		m_Arith.l_add( val[3], val[3], (pK+id)->KKval4 );
		m_Arith.l_add( val[4], val[4], (pK+id)->KKval5 );
		m_Arith.l_add( val[5], val[5], (pK+id)->KKval6 );
		m_Arith.l_add( val[6], val[6], (pK+id)->KKval7 );
		m_Arith.l_add( val[7], val[7], (pK+id)->KKval8 );
		m_Arith.l_add( val[8], val[8], (pK+id)->KKval9 );	// kasai 追加
		m_Arith.l_add( val[9], val[9], (pK+id)->KKval10 );

	}
	
	id = KINP+KLIN;
	memcpy( (pK+id)->KKval1, val[0], 6 );
	memcpy( (pK+id)->KKval2, val[1], 6 );
	memcpy( (pK+id)->KKval3, val[2], 6 );
	memcpy( (pK+id)->KKval4, val[3], 6 );
	memcpy( (pK+id)->KKval5, val[4], 6 );
	memcpy( (pK+id)->KKval6, val[5], 6 );
	memcpy( (pK+id)->KKval7, val[6], 6 );
	memcpy( (pK+id)->KKval8, val[7], 6 );
	memcpy( (pK+id)->KKval9, val[8], 6 );	// kasai 追加
	memcpy( (pK+id)->KKval10, val[9], 6 );


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
int	CShzTax31Calc::Shz_KniKeiPost()
{

	KKVAL*	pK = (*m_pTaxListData)->NRec.KUval;
	char	val[6];

// 22-0167,22-0361 add -->
	long	eymd=0;
	int		ii=0,old=0,sw=0;
	if(!((*m_pSnHeadData)->Sn_SKKBN % 2))	eymd = (*m_pSnHeadData)->Sn_MDAYE;
	else									eymd = (*m_pSnHeadData)->Sn_KDAYE;
// インボイス_23/07/24_higuchi cor -->
	//if((*m_pSnHeadData)->SVmzsw == 1 || eymd < TAX_R04_SDAY)	old=1;
// -----------------------------------
	//　23.10.10課税取引金額計算表の制御を12/31→10/01切り替えに
//	if((*m_pSnHeadData)->SVmzsw == 1 || eymd < TAX_R04_SDAY || eymd >= TAX_R05_EDAY)	{
	if ((*m_pSnHeadData)->SVmzsw == 1 || eymd < TAX_R04_SDAY || eymd >= ICS_SH_INVOICE_DAY) {
		old=1;
	}
// インボイス_23/07/24_higuchi cor <--
// 22-0167,22-0361 add <--

	if( (*m_pTaxListData)->GKBN & 0x01 ){	// 事業所

		if( !((pK+0)->KKauto&0x01) ){
			::ZeroMemory( val, 6 );
			m_Arith.l_sub ( val, (pK+47)->KKval3, (pK+43)->KKval3 );	// 合計 - 農業 - 固定資産　－ 不動産
			m_Arith.l_sub ( val, val, (pK+48)->KKval3 );
			m_Arith.l_sub ( val, val, (pK+46)->KKval3 );
			m_Arith.l_sub ( val, val, (pK+49)->KKval3 );//追加農業
			memcpy( (pK+0)->KKval1, val, 6 );
		}

// 22-0167,22-0361 cor -->
		//// 差引課税売上高　うち税率７．８％適用分
		////if( !((pK+0)->KKauto&0x02) ){
		//if( !((pK+0)->KKauto&0x08) ){
		//	::ZeroMemory( val, 6 );
		//	m_Arith.l_sub ( val, (pK+47)->KKval5, (pK+43)->KKval5 );
		//	m_Arith.l_sub ( val, val, (pK+48)->KKval5 );
		//	m_Arith.l_sub ( val, val, (pK+46)->KKval5 );
		//	m_Arith.l_sub ( val, val, (pK+49)->KKval5);
		//	memcpy( (pK+0)->KKval4, val, 6 );
		//}
// -----------------------
		// 差引課税売上高　うち税率７．８％適用分
		sw=0;
		if(old)	{
			if(!((pK+0)->KKauto&0x08))	sw=1;
		}
		else	{
			if((!((pK+0)->KKauto&0x08)) && (!((*m_pSnHeadData)->Sn_GenericSgn & 0x800)))	sw=1;
		}
		if(sw)	{
			::ZeroMemory( val, 6 );
			m_Arith.l_sub ( val, (pK+47)->KKval5, (pK+43)->KKval5 );
			m_Arith.l_sub ( val, val, (pK+48)->KKval5 );
			m_Arith.l_sub ( val, val, (pK+46)->KKval5 );
			m_Arith.l_sub ( val, val, (pK+49)->KKval5);
			memcpy( (pK+0)->KKval4, val, 6 );
		}
// 22-0167,22-0361 cor <--
		// 差引課税売上高　うち税率６．２４％適用分
		if( !((pK+0)->KKauto&0x04) ){
			::ZeroMemory( val, 6 );
			m_Arith.l_sub ( val, (pK+47)->KKval7, (pK+43)->KKval7 );
			m_Arith.l_sub ( val, val, (pK+48)->KKval7 );
			m_Arith.l_sub ( val, val, (pK+46)->KKval7 );
			m_Arith.l_sub ( val, val, (pK+49)->KKval7 );
			memcpy( (pK+0)->KKval3, val, 6 );
		}

		// 差引課税売上高　うち税率６．３％適用分
		//if( !((pK+0)->KKauto&0x08) ){
		if( !((pK+0)->KKauto&0x02) ){
			::ZeroMemory( val, 6 );
			m_Arith.l_sub ( val, (pK+47)->KKval9, (pK+43)->KKval9 );
			m_Arith.l_sub ( val, val, (pK+48)->KKval9 );
			m_Arith.l_sub ( val, val, (pK+46)->KKval9 );
			m_Arith.l_sub ( val, val, (pK+49)->KKval9 );
			memcpy( (pK+0)->KKval2, val, 6 );
		}
	}

	//if( (*m_pTaxListData)->GKBN & 0x04 ){	// 農業事業者

		if( !((pK+1)->KKauto&0x01) ){
			::ZeroMemory( val, 6 );
			m_Arith.l_add(val, (pK + 43)->KKval3, (pK + 49)->KKval3);
			memcpy((pK + 1)->KKval1, val, 6);


//			memcpy( (pK+1)->KKval1, (pK+43)->KKval3, 6 );
		}
// 22-0167,22-0361 cor -->
		//if( !((pK+1)->KKauto&0x08) ){	// 7.8%
//			::ZeroMemory(val, 6);
//			m_Arith.l_add(val, (pK + 43)->KKval5, (pK + 49)->KKval5);
//			memcpy((pK + 1)->KKval4, val, 6);
//
////			memcpy( (pK+1)->KKval4, (pK+43)->KKval5, 6 );
//		}
// -----------------------
		sw=0;
		if(old)	{
			if(!((pK+1)->KKauto&0x08))	sw=1;
		}
		else	{
			if((!((pK+1)->KKauto&0x08)) && (!((*m_pSnHeadData)->Sn_GenericSgn & 0x800)))	sw=1;
		}
		if(sw)	{
			::ZeroMemory(val, 6);
			m_Arith.l_add(val, (pK + 43)->KKval5, (pK + 49)->KKval5);
			memcpy((pK + 1)->KKval4, val, 6);
		}
// 22-0167,22-0361 cor <--
		if( !((pK+1)->KKauto&0x04) ){	// 6.24%
			::ZeroMemory(val, 6);
			m_Arith.l_add(val, (pK + 43)->KKval7, (pK + 49)->KKval7);
			memcpy((pK + 1)->KKval3, val, 6);

//			memcpy( (pK+1)->KKval3, (pK+43)->KKval7, 6 );
		}
		//if( !((pK+1)->KKauto&0x08) ){	// 6.3%
		if( !((pK+1)->KKauto&0x02) ){	// 6.3%
			::ZeroMemory(val, 6);
			m_Arith.l_add(val, (pK + 43)->KKval9, (pK + 49)->KKval9);
			memcpy((pK + 1)->KKval2, val, 6);

//			memcpy( (pK+1)->KKval2, (pK+43)->KKval9, 6 );
		}

		

	//}


	// 第六種
	if( !((pK+2)->KKauto&0x01) ){
		::ZeroMemory( val, 6 );
		memcpy( (pK+2)->KKval1, (pK+46)->KKval3, 6 );
	}
// 22-0167,22-0361 cor -->
	//if( !((pK+2)->KKauto&0x08) ){
	//	::ZeroMemory( val, 6 );
	//	memcpy( (pK+2)->KKval4, (pK+46)->KKval5, 6 );
	//}
// -----------------------
	sw=0;
	if(old)	{
		if(!((pK+2)->KKauto&0x08))	sw=1;
	}
	else	{
		if((!((pK+2)->KKauto&0x08)) && (!((*m_pSnHeadData)->Sn_GenericSgn & 0x800)))	sw=1;
	}
	if(sw)	{
		memcpy( (pK+2)->KKval4, (pK+46)->KKval5, 6 );
	}
// 22-0167,22-0361 cor <--
	if( !((pK+2)->KKauto&0x04) ){
		::ZeroMemory( val, 6 );
		memcpy( (pK+2)->KKval3, (pK+46)->KKval7, 6 );
	}

	if( !((pK+2)->KKauto&0x02) ){
		::ZeroMemory( val, 6 );
		memcpy( (pK+2)->KKval2, (pK+46)->KKval9, 6 );
	}

	// 差引課税売上高　うち税率７．８％適用分
/*	if( !((pK+8)->KKauto&0x02) ){
		::ZeroMemory( val, 6 );
		memcpy( (pK+8)->KKval2, (pK+46)->KKval9, 6 );
	}
	// 差引課税売上高　うち税率６．２４％適用分
	if( !((pK+8)->KKauto&0x04) ){
		::ZeroMemory( val, 6 );
		memcpy( (pK+8)->KKval3, (pK+46)->KKval7, 6 );
	}
	// 差引課税売上高　うち税率６．３％適用分　仮
	if( !((pK+8)->KKauto&0x08) ){
		::ZeroMemory( val, 6 );
		memcpy( (pK+8)->KKval4, (pK+46)->KKval5, 6 );
	}
*/

	// 固定資産等の譲渡
	if( !((pK+6)->KKauto&0x01) ){
		::ZeroMemory( val, 6 );
		// 決算額を取得
		//memcpy( (pK+6)->KKval1, (pK+48)->KKval3, 6 );
		memcpy( (pK+6)->KKval1, (pK+48)->KKval1, 6 );
	}
	// 課税取引外も取得
	if( !((pK+7)->KKauto&0x01) ){
		::ZeroMemory( val, 6 );
		memcpy( (pK+7)->KKval1, (pK+48)->KKval2, 6 );
	}
// 22-0167,22-0361 cor -->
	//if( !((pK+6)->KKauto&0x08) ){
	//	::ZeroMemory( val, 6 );
	//	memcpy( (pK+6)->KKval4, (pK+48)->KKval5, 6 );
	//}
// -----------------------
	sw=0;
	if(old)	{
		if(!((pK+6)->KKauto&0x08))	sw=1;
	}
	else	{
		if((!((pK+6)->KKauto&0x08)) && (!((*m_pSnHeadData)->Sn_GenericSgn & 0x800)))	sw=1;
	}
	if(sw)	{
		memcpy( (pK+6)->KKval4, (pK+48)->KKval5, 6 );
	}
// 22-0167,22-0361 cor <--
	if( !((pK+6)->KKauto&0x04) ){
		::ZeroMemory( val, 6 );
		memcpy( (pK+6)->KKval3, (pK+48)->KKval7, 6 );
	}

	if( !((pK+6)->KKauto&0x02) ){
		::ZeroMemory( val, 6 );
		memcpy( (pK+6)->KKval2, (pK+48)->KKval9, 6 );
	}
/*
	// 差引課税売上高　うち税率７．８％適用分
	if( !((pK+14)->KKauto&0x02) ){
		::ZeroMemory( val, 6 );
		memcpy( (pK+14)->KKval2, (pK+48)->KKval9, 6 );
	}
	// 差引課税売上高　うち税率６．２４％適用
	if( !((pK+14)->KKauto&0x04) ){
		::ZeroMemory( val, 6 );
		memcpy( (pK+14)->KKval3, (pK+48)->KKval7, 6 );
	}
	// 差引課税売上高　うち税率６．３％適用
	if( !((pK+14)->KKauto&0x08) ){
		::ZeroMemory( val, 6 );
		memcpy( (pK+14)->KKval4, (pK+48)->KKval5, 6 );
	}
*/

// 修正依頼No.161189対応
	if(old)	{
		// 旧様式で「⑦のうち、課税売上げにならないもの」の「うち7.8%適用分」の
		// 連動サインOFFであれば金額をクリアする
		if(!((pK+7)->KKauto&0x08))	{
			memset((pK+7)->KKval4,0,6);
		}
	}

// 22-0167,22-0361 add -->
	// 新様式の場合、自動計算チェックが付いていれば自動計算する
// インボイス_23/07/24_higuchi cor -->
	//if((*m_pSnHeadData)->SVmzsw != 1 && eymd >= TAX_R04_SDAY)	{
// -----------------------------------
	//if((*m_pSnHeadData)->SVmzsw != 1 && eymd >= TAX_R04_SDAY && eymd < TAX_R05_EDAY)	{
	if ((*m_pSnHeadData)->SVmzsw != 1 && eymd >= TAX_R04_SDAY ) {
// インボイス_23/07/24_higuchi cor <--
		for(ii=KSELLINE1; ii<=KSELLINE8; ii++)	{
			if(ii == KSELLINE6)	continue;
			//if(ii == KSELLINE8)	continue;	// (4)業務用資産…、⑦のうち、課税売上げにならないもの
			// 金額 - 6.24% = 7.8%
			if((*m_pSnHeadData)->Sn_GenericSgn & 0x800)	{
				m_Arith.l_sub((pK+ii)->KKval4,(pK+ii)->KKval1,(pK+ii)->KKval3);
				//if(ii == KSELLINE7)	{
				//	// (4)業務用資産…、業務用固定資産等の譲渡収入金額
				//	// 課税売上げにならないものも差し引く
				//	m_Arith.l_sub((pK+ii)->KKval4,(pK+ii)->KKval4,(pK+ii+1)->KKval1);
				//}
				(pK+ii)->KKauto &= 0xf7;
			}
		}
	}
// 22-0167,22-0361 add <--

	Shz_Sale31Calc();

	return 0;


	
	// 26のときの処理
	/*
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

	Shz_Sale31Calc();

	return 0;
	*/
}

//'14.10.08
//-----------------------------------------------------------------------------
// 非連動で簡易課税(経過措置用)
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzTax31Calc::Shz_DataCpyKniKei()
{
	KKVAL*	pU = (*m_pTaxListData)->NRec.KUval;
//2017.12.04 INSERT START
	KKVAL*	pJ = (*m_pTaxListData)->NRec.KJval;
	KKVAL*	pF = (*m_pTaxListData)->NRec.KFval;
	KKVAL*	pN = (*m_pTaxListData)->NRec.KNval;
//2017.12.04 INSERT END
	char	val[6], val2[6], PW1[6], PW2[6], clc[6];

	::ZeroMemory(&tenkidata , sizeof(tenkidata));

	// ①1000円未満切り捨て廃止されたため変更
	// 課税標準額7.8％
	::ZeroMemory( val, 6 );
	//::ZeroMemory( PW1, 6 );
	//m_Arith.l_input( PW1, _T("1000") );
	//m_Arith.l_div( val, (pU+12)->KKval1, PW1 );
	//m_Arith.l_mul( val, val, PW1 );
	memcpy(tenkidata.Sn_1F1D , (pU+12)->KKval1 , 6);
	// 課税標準額6.24％
	::ZeroMemory( val, 6 );
	//::ZeroMemory( PW1, 6 );
	//m_Arith.l_input( PW1, _T("1000") );
	//m_Arith.l_div( val, (pU+11)->KKval1, PW1 );
	//m_Arith.l_mul( val, val, PW1 );
	memcpy(tenkidata.Sn_1F1C , (pU+11)->KKval1 , 6);
	// 課税標準額6.3％
	::ZeroMemory( val, 6 );
	//::ZeroMemory( PW1, 6 );
	//m_Arith.l_input( PW1, _T("1000") );
	//m_Arith.l_div( val, (pU+10)->KKval1, PW1 );
	//m_Arith.l_mul( val, val, PW1 );
	memcpy(tenkidata.Sn_1F1B , (pU+10)->KKval1 , 6);

	//消費税率7.8%
	::ZeroMemory( val, 6 );
	::ZeroMemory( PW1, 6 );
	::ZeroMemory( PW2, 6 );
	m_Arith.l_input( PW1, _T("78") );
	m_Arith.l_input( PW2, _T("1000") );
	m_Arith.l_mul( val, tenkidata.Sn_1F1D, PW1 );
	m_Arith.l_div( val, val, PW2 );
	memcpy(tenkidata.Sn_1F2D , val , 6);

	//消費税率6.24%
	::ZeroMemory( val, 6 );
	::ZeroMemory( PW1, 6 );
	::ZeroMemory( PW2, 6 );
	m_Arith.l_input( PW1, _T("624") );
	m_Arith.l_input( PW2, _T("10000") );
	m_Arith.l_mul( val, tenkidata.Sn_1F1C, PW1 );
	m_Arith.l_div( val, val, PW2 );
	memcpy(tenkidata.Sn_1F2C , val , 6);

	//消費税率6.3%
	::ZeroMemory( val, 6 );
	::ZeroMemory( PW1, 6 );
	::ZeroMemory( PW2, 6 );
	m_Arith.l_input( PW1, _T("63") );
	m_Arith.l_input( PW2, _T("1000") );//yoshida200127
//	m_Arith.l_input( PW2, _T("100") );
	m_Arith.l_mul( val, tenkidata.Sn_1F1B, PW1 );
	m_Arith.l_div( val, val, PW2 );
	memcpy(tenkidata.Sn_1F2B , val , 6);


	//返還等課税取引7.8％を本表側の返還等対価に係る税額の7.8％へ
	if( Util.IsUnConnectMst(m_pZmSub->zvol) ){
		::ZeroMemory( clc, 6 );
		for( int i=0, id=40; i<5; i++ ){//下if群２回通りidxが飛ばされるのでループは前と同じ５のまま。yoshida190728
//		for( int i=0, id=40; i<6; i++ ){
			::ZeroMemory( val, 6 );
			memcpy( val, (pU+id)->KKval6, 6 );
			if( (i==2) || (i==4) ){
				m_Arith.l_add( val, val, (pU+(id+1))->KKval6 );
				id++;
			}
			if( (*m_pTaxListData)->Head.KHstax ){
				::ZeroMemory( val2, 6 );
				m_Arith.l_input( PW1, _T("10") );
				m_Arith.l_input( PW2, _T("100") );
				m_Arith.l_mul( val2,val, PW1 );
				m_Arith.l_div( val2, val2, PW2 );
				m_Arith.l_add( val, val, val2 );
			}
			::ZeroMemory( val2, 6 );
			Shz_shohizei78( val2, val );
			id++;
			m_Arith.l_add( clc, clc, val2 );
		}
		memcpy(tenkidata.Sn_1F5D , clc , 6);
	}else{
		::ZeroMemory( val, 6 );
		//memcpy( val, (pJ+KJIGLINE1TA)->KKval5, 6 );
		//m_Arith.l_add( val, val, (pF+KFUDLINE4TA)->KKval5 );
		//m_Arith.l_add( val, val, (pN+KNOULINE4TA)->KKval5 );
		memcpy(val, (pJ + KJIGLINE1TA)->KKval6, 6);
		m_Arith.l_add(val, val, (pF + KFUDLINE4TA)->KKval6);
		m_Arith.l_add(val, val, (pN + KNOULINE4TA)->KKval6);
		if((*m_pTaxListData)->Head.KHstax ){
			::ZeroMemory( val2, 6 );
			m_Arith.l_input( PW1, _T("10") );
			m_Arith.l_input( PW2, _T("100") );
			m_Arith.l_mul( val2,val, PW1 );
			m_Arith.l_div( val2, val2, PW2 );
			
			m_Arith.l_add( val, val, val2 );
		}
		::ZeroMemory( val2, 6 );
		Shz_shohizei78( val2, val );
		memcpy(tenkidata.Sn_1F5D , val2 , 6);
	}

	//返還等課税取引6.24％を本表側の返還等対価に係る税額の6.24％へ
	if( Util.IsUnConnectMst(m_pZmSub->zvol) ){
		::ZeroMemory( clc, 6 );

		//yoshida190728
		for (int i = 0, id = 40; i < 6; i++) {
			::ZeroMemory(val, 6);
			memcpy(val, (pU + id)->KKval8, 6);
			if(i == 1){//第２種のループの際
				m_Arith.l_add(val, val, (pU + 49)->KKval8);//第２種農業の軽減分をプラス
			}
			else if(i == 2) {//第３種のループの際
				id++;
			}
			if ((*m_pTaxListData)->Head.KHstax) {
				::ZeroMemory(val2, 6);
				m_Arith.l_input(PW1, _T("8"));
				m_Arith.l_input(PW2, _T("100"));
				m_Arith.l_mul(val2, val, PW1);
				m_Arith.l_div(val2, val2, PW2);
				m_Arith.l_add(val, val, val2);
			}
			::ZeroMemory(val2, 6);
			Shz_shohizei624(val2, val);
			id++;
			m_Arith.l_add(clc, clc, val2);
		}

		/*
		for( int i=0, id=40; i<6; i++ ){
			::ZeroMemory( val, 6 );
			memcpy( val, (pU+id)->KKval8, 6 );
			if( (i==2) || (i==4) ){
				m_Arith.l_add( val, val, (pU+(id+1))->KKval8 );
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
			Shz_shohizei624( val2, val );
			id++;
			m_Arith.l_add( clc, clc, val2 );
		}
		*/
		memcpy(tenkidata.Sn_1F5C , clc , 6);
	}
	else{
		::ZeroMemory( val, 6 );
		//memcpy( val, (pJ+KJIGLINE1TA)->KKval7, 6 );
		//m_Arith.l_add( val, val, (pF+KFUDLINE4TA)->KKval7 );
		//m_Arith.l_add( val, val, (pN+KNOULINE4TA)->KKval7 );
		memcpy(val, (pJ + KJIGLINE1TA)->KKval5, 6);
		m_Arith.l_add(val, val, (pF + KFUDLINE4TA)->KKval5);
		m_Arith.l_add(val, val, (pN + KNOULINE4TA)->KKval5);
		if((*m_pTaxListData)->Head.KHstax ){
			::ZeroMemory( val2, 6 );
			m_Arith.l_input( PW1, _T("8") );
			m_Arith.l_input( PW2, _T("100") );
			m_Arith.l_mul( val2,val, PW1 );
			m_Arith.l_div( val2, val2, PW2 );
			
			m_Arith.l_add( val, val, val2 );
		}
		::ZeroMemory( val2, 6 );
		Shz_shohizei624( val2, val );
		memcpy(tenkidata.Sn_1F5C , val2 , 6);
	}

	//返還等課税取引6.3％を本表側の返還等対価に係る税額の6.3％へ
	::ZeroMemory( clc, 6 );
	if( Util.IsUnConnectMst(m_pZmSub->zvol) ){
		for( int i=0, id=40; i<6; i++ ){
			::ZeroMemory( val, 6 );
			memcpy( val, (pU+id)->KKval10, 6 );
			if( i==2 ){
				m_Arith.l_add( val, val, (pU+(id+1))->KKval10 );
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
		memcpy(tenkidata.Sn_1F5B , clc , 6);
//2017.12.04 INSERT START
	}else{
		::ZeroMemory( val, 6 );
		//memcpy( val, (pJ+KJIGLINE1TA)->KKval9, 6 );
		//m_Arith.l_add( val, val, (pF+KFUDLINE4TA)->KKval9 );
		//m_Arith.l_add( val, val, (pN+KNOULINE4TA)->KKval9 );
		memcpy(val, (pJ + KJIGLINE1TA)->KKval4, 6);
		m_Arith.l_add(val, val, (pF + KFUDLINE4TA)->KKval4);
		m_Arith.l_add(val, val, (pN + KNOULINE4TA)->KKval4);
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
		memcpy(tenkidata.Sn_1F5B , val2 , 6);
	}

	// 第１種～第５種までの金額を付表５－２に転記　7.8％分

	//課税取引金額　７．８％
	::ZeroMemory( clc, 6 );
	if (Util.IsUnConnectMst(m_pZmSub->zvol)) {// 事業別売上高を利用していない場合は転記しない
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
				Shz_shohizei100_110( val, val );
			}

			::ZeroMemory( val2, 6 );
			memcpy( val2, (pU+id)->KKval6, 6 );
			if( i==2 ){
				m_Arith.l_add( val2, val2, (pU+(id+1))->KKval6 );
				id++;
			}
			if( !(*m_pTaxListData)->Head.KHstax ){
				Shz_shohizei100_110( val2, val2 );
			}
			id++;
			m_Arith.l_sub( val, val, val2 );

			//-->yoshida190806
			//マイナス値の場合も転記すると依頼（連動がその為）修正依頼６９５
			/*
			//マイナス値の場合には０にして転記する
			if( m_Arith.l_test(val) < 0 ){
				::ZeroMemory(val , sizeof(val));
			}
			*/
			//<------

			switch( i ){
				case 0:		
					memcpy(tenkidata.Sn_5FURI1D , val , 6);
					break;
				case 1:		
					memcpy(tenkidata.Sn_5FURI2D , val , 6);
					break;
				
				case 2:		
					memcpy(tenkidata.Sn_5FURI3D , val , 6);
					break;

				case 3:		
					memcpy(tenkidata.Sn_5FURI4D , val , 6);
					break;

				case 4:		
					memcpy(tenkidata.Sn_5FURI5D , val , 6);
					break;

				case 5:		
					memcpy(tenkidata.Sn_5FURI6D , val , 6);
					break;
			}
			m_Arith.l_add( clc, clc, val );
		}
	}
	else {
		::ZeroMemory(tenkidata.Sn_5FURI1D, 6);
		::ZeroMemory(tenkidata.Sn_5FURI2D, 6);
		::ZeroMemory(tenkidata.Sn_5FURI3D, 6);
		::ZeroMemory(tenkidata.Sn_5FURI4D, 6);
		::ZeroMemory(tenkidata.Sn_5FURI5D, 6);
		::ZeroMemory(tenkidata.Sn_5FURI6D, 6);
	}

	// 第１種～第５種までの金額を付表５－２に転記　6.24％分

	//課税取引金額６．２４％（返還等課税取引引く）
	//修正168614　戎居
	//::ZeroMemory(clc, 6);
	if (Util.IsUnConnectMst(m_pZmSub->zvol)) {
		//yoshida190728
		for (int i = 0, id = 40; i < 6; i++) {
			::ZeroMemory(val, 6);
			memcpy(val, (pU + id)->KKval7, 6);
			if (i == 1) {//第２種農業分足す
				m_Arith.l_add(val, val, (pU + 49)->KKval7);
			}
			else if (i == 3) {
				m_Arith.l_add(val, val, (pU + 48)->KKval7);
			}
			if (!(*m_pTaxListData)->Head.KHstax) {
				Shz_shohizei100_108(val, val);
			}

			::ZeroMemory(val2, 6);
			memcpy(val2, (pU + id)->KKval8, 6);

			//-->yoshida190809
			if (i == 1) {//第２種農業分足す
				m_Arith.l_add(val2, val2, (pU + 49)->KKval8);
			}
			//--->yoshida190812
			else if(i == 2){
				id++;
			}
			//----
	//		if (i == 2) {
	//			m_Arith.l_add(val2, val2, (pU + 49)->KKval8);
	//			id++;
	//		}
			//<-------
			if (!(*m_pTaxListData)->Head.KHstax) {
				Shz_shohizei100_108(val2, val2);
			}
			id++;
			m_Arith.l_sub(val, val, val2);

			//-->yoshida190806
			//マイナス値の場合も転記すると依頼（連動がその為）修正依頼６９５
			/*
			//マイナス値の場合には０にして転記する
			if (m_Arith.l_test(val) < 0) {
				::ZeroMemory(val, sizeof(val));
			}
			*/
			//<------

			switch (i) {
			case 0:
				memcpy(tenkidata.Sn_5FURI1C, val, 6);
				break;
			case 1:
				memcpy(tenkidata.Sn_5FURI2C, val, 6);
				break;

			case 2:
				memcpy(tenkidata.Sn_5FURI3C, val, 6);
				break;

			case 3:
				memcpy(tenkidata.Sn_5FURI4C, val, 6);
				break;

			case 4:
				memcpy(tenkidata.Sn_5FURI5C, val, 6);
				break;

			case 5:
				memcpy(tenkidata.Sn_5FURI6C, val, 6);
				break;
		}
		m_Arith.l_add(clc, clc, val);

		/*
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
				Shz_shohizei100_108( val, val );
			}

			::ZeroMemory( val2, 6 );
			memcpy( val2, (pU+id)->KKval8, 6 );
			if( i==2 ){
				m_Arith.l_add( val2, val2, (pU+(id+1))->KKval8 );
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
			*/
		}
	}
	else {
		::ZeroMemory(tenkidata.Sn_5FURI1C, 6);
		::ZeroMemory(tenkidata.Sn_5FURI2C, 6);
		::ZeroMemory(tenkidata.Sn_5FURI3C, 6);
		::ZeroMemory(tenkidata.Sn_5FURI4C, 6);
		::ZeroMemory(tenkidata.Sn_5FURI5C, 6);
		::ZeroMemory(tenkidata.Sn_5FURI6C, 6);
	}

	// 第１種～第５種までの金額を付表５－２に転記　6.3％分
	if (Util.IsUnConnectMst(m_pZmSub->zvol)) {
		for( int i=0, id=40; i<6; i++ ){
			::ZeroMemory( val, 6 );
			memcpy( val, (pU+id)->KKval9, 6 );
			if( i==2 ){
				m_Arith.l_add( val, val, (pU+(id+1))->KKval9 );
			}
			else if( i == 3 ){		// 10/04/06 追加
				m_Arith.l_add( val, val, (pU+48)->KKval9 );
			}
			if( !(*m_pTaxListData)->Head.KHstax ){
				Shz_shohizei100_108( val, val );
			}

			::ZeroMemory( val2, 6 );
			memcpy( val2, (pU+id)->KKval10, 6 );
			if( i==2 ){
				m_Arith.l_add( val2, val2, (pU+(id+1))->KKval10 );
				id++;
			}
			if( !(*m_pTaxListData)->Head.KHstax ){
				Shz_shohizei100_108( val2, val2 );
			}
			id++;
			m_Arith.l_sub( val, val, val2 );

			//-->yoshida190806
			//マイナス値の場合も転記すると依頼（連動がその為）修正依頼６９５
			/*
			//マイナス値の場合には０にして転記する
			if( m_Arith.l_test(val) < 0 ){
				::ZeroMemory(val , sizeof(val));
			}
			*/
			//<------

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
	}
	else {
	::ZeroMemory(tenkidata.Sn_5FURI1B, 6);
	::ZeroMemory(tenkidata.Sn_5FURI2B, 6);
	::ZeroMemory(tenkidata.Sn_5FURI3B, 6);
	::ZeroMemory(tenkidata.Sn_5FURI4B, 6);
	::ZeroMemory(tenkidata.Sn_5FURI5B, 6);
	::ZeroMemory(tenkidata.Sn_5FURI6B, 6);
	}

//	// 付表5
	if (Util.IsUnConnectMst(m_pZmSub->zvol)) {
		memcpy(tenkidata.Sn_5FURITA , clc , 6);
	}
	else {//転記しない場合は初期化
		::ZeroMemory(tenkidata.Sn_5FURITA, 6);
	}

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
int	CShzTax31Calc::Shz_DataCpyUsuKei()
{

	::ZeroMemory(&tenkidata , sizeof(tenkidata));

	KKVAL*	pJ = (*m_pTaxListData)->NRec.KJval;
	KKVAL*	pF = (*m_pTaxListData)->NRec.KFval;
	KKVAL*	pN = (*m_pTaxListData)->NRec.KNval;

	KKVAL*	pU = (*m_pTaxListData)->NRec.KUval;
	KKVAL*	pS = (*m_pTaxListData)->NRec.KSval;
	char	val[6], val2[6], PW1[6], PW2[6];

	// ①1000円未満切り捨て廃止されたため変更
	// 第27表-(1)号様式
	// 課税標準額7.8%
	::ZeroMemory( val, 6 );
	//::ZeroMemory( PW1, 6 );
	//m_Arith.l_input( PW1, _T("1000") );
	//m_Arith.l_div( val, (pU+12)->KKval1, PW1 );
	//m_Arith.l_mul( val, val, PW1 );
	memcpy(tenkidata.Sn_1F1D , (pU+12)->KKval1 , 6);

	// 課税標準額6.24％
	::ZeroMemory( val, 6 );
	//::ZeroMemory( PW1, 6 );
	//m_Arith.l_input( PW1, _T("1000") );
	//m_Arith.l_div( val, (pU+11)->KKval1, PW1 );
	//m_Arith.l_mul( val, val, PW1 );
	memcpy(tenkidata.Sn_1F1C , (pU+11)->KKval1 , 6);

	// 課税標準額6.3％
	::ZeroMemory( val, 6 );
	//::ZeroMemory( PW1, 6 );
	//m_Arith.l_input( PW1, _T("1000") );
	//m_Arith.l_div( val, (pU+10)->KKval1, PW1 );
	//m_Arith.l_mul( val, val, PW1 );
	memcpy(tenkidata.Sn_1F1B , (pU+10)->KKval1 , 6);

	//消費税率7.8%
	::ZeroMemory( val, 6 );
	::ZeroMemory( PW1, 6 );
	::ZeroMemory( PW2, 6 );
	m_Arith.l_input( PW1, _T("78") );
	m_Arith.l_input( PW2, _T("1000") );
	m_Arith.l_mul( val,tenkidata.Sn_1F1D, PW1 );
	m_Arith.l_div( val, val, PW2 );
	memcpy(tenkidata.Sn_1F2D , val , 6);

	//消費税率6.24%
	::ZeroMemory( val, 6 );
	::ZeroMemory( PW1, 6 );
	::ZeroMemory( PW2, 6 );
	m_Arith.l_input( PW1, _T("624") );
	m_Arith.l_input( PW2, _T("10000") );
	m_Arith.l_mul( val,tenkidata.Sn_1F1C, PW1 );
	m_Arith.l_div( val, val, PW2 );
	memcpy(tenkidata.Sn_1F2C , val , 6);

	//消費税率6.3%
	::ZeroMemory( val, 6 );
	::ZeroMemory( PW1, 6 );
	::ZeroMemory( PW2, 6 );
	m_Arith.l_input( PW1, _T("63") );
	m_Arith.l_input( PW2, _T("1000") );//yoshida200127
//	m_Arith.l_input( PW2, _T("100") );
	m_Arith.l_mul( val, tenkidata.Sn_1F1B, PW1 );
	m_Arith.l_div( val, val, PW2 );
	memcpy(tenkidata.Sn_1F2B , val , 6);

	//返還等課税取引7.8％を本表側の返還等対価に係る税額の7.8％へ
	::ZeroMemory( val, 6 );
	//--->修正依頼168643　戎居
	if ((*m_pTaxListData)->GKBN & 0x01) {	// 事業
		m_Arith.l_add(val, val, (pJ + KJIGLINE1TA)->KKval6);
	}
	if ((*m_pTaxListData)->GKBN & 0x02) {	// 不動産
		m_Arith.l_add(val, val, (pF + KFUDLINE4TA)->KKval6);
	}
	if ((*m_pTaxListData)->GKBN & 0x04) {	// 農業
		m_Arith.l_add(val, val, (pN + KNOULINE4TA)->KKval6);
	}
	//memcpy( val, (pJ+KJIGLINE1TA)->KKval6, 6 );
	//m_Arith.l_add( val, val, (pF+KFUDLINE4TA)->KKval6 );
	//m_Arith.l_add( val, val, (pN+KNOULINE4TA)->KKval6 );
	//<---
	if((*m_pTaxListData)->Head.KHstax ){
		::ZeroMemory( val2, 6 );
		m_Arith.l_input( PW1, _T("10") );
		m_Arith.l_input( PW2, _T("100") );
		m_Arith.l_mul( val2,val, PW1 );
		m_Arith.l_div( val2, val2, PW2 );
		
		m_Arith.l_add( val, val, val2 );
	}
	::ZeroMemory( val2, 6 );
	Shz_shohizei78( val2, val );
	memcpy(tenkidata.Sn_1F5D , val2 , 6);//'15.01.09

	//返還等課税取引6.24％を本表側の返還等対価に係る税額の6.24％へ
	::ZeroMemory( val, 6 );
	//--->修正依頼168643　戎居
	if ((*m_pTaxListData)->GKBN & 0x01) {	// 事業
		m_Arith.l_add(val, val, (pJ + KJIGLINE1TA)->KKval5);
	}
	if ((*m_pTaxListData)->GKBN & 0x02) {	// 不動産
		m_Arith.l_add(val, val, (pF + KFUDLINE4TA)->KKval5);
	}
	if ((*m_pTaxListData)->GKBN & 0x04) {	// 農業
		m_Arith.l_add(val, val, (pN + KNOULINE4TA)->KKval5);
	}
	//memcpy( val, (pJ+KJIGLINE1TA)->KKval5, 6 );
	//m_Arith.l_add( val, val, (pF+KFUDLINE4TA)->KKval5 );
	//m_Arith.l_add( val, val, (pN+KNOULINE4TA)->KKval5 );
	//<---
	if((*m_pTaxListData)->Head.KHstax ){
		::ZeroMemory( val2, 6 );
		m_Arith.l_input( PW1, _T("8") );
		m_Arith.l_input( PW2, _T("100") );
		m_Arith.l_mul( val2,val, PW1 );
		m_Arith.l_div( val2, val2, PW2 );
		
		m_Arith.l_add( val, val, val2 );
	}
	::ZeroMemory( val2, 6 );
	Shz_shohizei624( val2, val );
	memcpy(tenkidata.Sn_1F5C , val2 , 6);//'15.01.09

	//返還等課税取引6.3％を本表側の返還等対価に係る税額の6.3％へ
	//------->'15.01.09
	::ZeroMemory( val, 6 );
	memcpy( val, (pJ+KJIGLINE1TA)->KKval4, 6 );
	m_Arith.l_add( val, val, (pF+KFUDLINE4TA)->KKval4 );
	m_Arith.l_add( val, val, (pN+KNOULINE4TA)->KKval4 );
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
	memcpy(tenkidata.Sn_1F5B , val2 , 6);
	//<---------------------------

	// 付表2
	// ①
	::ZeroMemory( val, 6 );
	memcpy( val, (pJ+KJIGLINE1TA)->KKval6, 6 );
	m_Arith.l_add( val, val, (pF+KFUDLINE4TA)->KKval6 );
	m_Arith.l_add( val, val, (pN+KNOULINE4TA)->KKval6 );
	if( !(*m_pTaxListData)->Head.KHstax ){
		Shz_shohizei100_110( val, val );
	}
	
	m_Arith.l_sub( tenkidata.Sn_2F1D, (pU+12)->KKval1, val );

	::ZeroMemory( val, 6 );
	memcpy( val, (pJ+KJIGLINE1TA)->KKval5, 6 );
	m_Arith.l_add( val, val, (pF+KFUDLINE4TA)->KKval5 );
	m_Arith.l_add( val, val, (pN+KNOULINE4TA)->KKval5 );
	if( !(*m_pTaxListData)->Head.KHstax ){
		//Shz_shohizei100_110( val, val );//'15.01.09
		Shz_shohizei100_108( val, val );
	}
	
	m_Arith.l_sub( tenkidata.Sn_2F1C, (pU+11)->KKval1, val );

	::ZeroMemory( val, 6 );
	memcpy( val, (pJ+KJIGLINE1TA)->KKval4, 6 );
	m_Arith.l_add( val, val, (pF+KFUDLINE4TA)->KKval4 );
	m_Arith.l_add( val, val, (pN+KNOULINE4TA)->KKval4 );
	if( !(*m_pTaxListData)->Head.KHstax ){
		Shz_shohizei100_108( val, val );//'15.01.09
	}
	
	m_Arith.l_sub( tenkidata.Sn_2F1B, (pU+10)->KKval1, val );

	memcpy(tenkidata.Sn_2F8D , (pS+12)->KKval2 , 6);
	memcpy(tenkidata.Sn_2F8C , (pS+11)->KKval2 , 6);
	memcpy(tenkidata.Sn_2F8B , (pS+10)->KKval2 , 6);

	memcpy(tenkidata.Sn_2F9D , (pS+12)->KKval1 , 6);
	memcpy(tenkidata.Sn_2F9C , (pS+11)->KKval1 , 6);
	memcpy(tenkidata.Sn_2F9B , (pS+10)->KKval1 , 6);
	return 0;

}

// インボイス_23/07/24_higuchi del -->
////-----------------------------------------------------------------------------
//// 計算表全集計(経過措置(8割控除)用)
////-----------------------------------------------------------------------------
//// 引数	mode	：	集計を行う個人業種(1=事業、2=不動産、3=農業、それ以外=全て)
////-----------------------------------------------------------------------------
//// 返送値		：
////-----------------------------------------------------------------------------
//int	CShzTax31Calc::Shz_R5AllCalc(int mode)
//{
//
//	if (Util.IsUnConnectMst(m_pZmSub->zvol) && (Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI)) {// 非連動ｄｂで簡易課税
//		Shz_KniKeiInput();
//		Shz_KniKeiPost();
//		/*
//		if ((*m_pSnHeadData)->SVmzsw == 1) {
//			Shz_KniKeiInput();
//			Shz_KniKeiPost();
//		}
//		else {
//			Shz_KniKeiInput();
//			Shz_KniKeiPost();
//			//			Shz_KniInput();
//			//			Shz_KniPost();
//		}
//		*/
//	}
//	else {
//		switch (mode) {
//		case 1:
//			Shz_JigyoR5Calc();
//			break;
//		case 2:
//			Shz_Fudoh31Calc();
//			break;
//		case 3:
//			Shz_Nohgyo31Calc();
//			break;
//		default:
//			Shz_JigyoR5Calc();
//			Shz_Fudoh31Calc();
//			Shz_Nohgyo31Calc();
//			break;
//		}
//		Shz_Sale31Post();
//		Shz_Buy31Post();
//	}
//
//	return 0;
//}
////-----------------------------------------------------------------------------
//// 事業所得(経過措置(8割控除)用)
////-----------------------------------------------------------------------------
//// 返送値	
////-----------------------------------------------------------------------------
//int	CShzTax31Calc::Shz_JigyoR5Calc()
//{
//	if (((*m_pTaxListData)->GKBN == 0x02) || ((*m_pTaxListData)->GKBN == 0x04)) {
//		return 0;
//	}
//	int	mode;
//	for (int i = 0; i < JIGLINCNT; i++) {
//		mode = 0; //A欄-B欄をC欄に入れる
//		switch (i) {
//		//A欄のみの行及び合計行はスキップ
//		case KJIGLINE2:		//期首商品棚卸高
//		case KJIGLINE4:		//小計
//		case KJIGLINE5:		//仕入金額
//		case KJIGLINE6:		//差引原価
//		case KJIGLINE7:		//差引金額
//		case KJIGLINE32:	//経費　計
//		case KJIGLINE33:	//差引金額
//			continue;
//
//		// A欄をC欄へ転記
//		case KJIGLINE10:	//水道光熱費
//		case KJIGLINE16:	//修繕費
//		case KJIGLINE17:	//消耗品費
//			mode = 1;
//			break;
//
//		// A欄をB欄へ転記
//		case KJIGLINE15:	//損害保険料
//		case KJIGLINE18:	//減価償却費
//		case KJIGLINE22:	//利子割引料
//		case KJIGLINE24:	//貸倒金
//			mode = 2;
//			break;
//		}
//		Shz_LinCalc(&(*m_pTaxListData)->NRec.KJval[i], mode);
//	}
//
//
//	KKVAL	*pK;
//	// 売上原価　小計(4)A
//	pK = &(*m_pTaxListData)->NRec.KJval[KJIGLINE4];
//	m_Arith.l_add(pK->KKval1, (*m_pTaxListData)->NRec.KJval[KJIGLINE2].KKval1, (*m_pTaxListData)->NRec.KJval[KJIGLINE3].KKval1);
//
//	// 売上原価　差引原価(6)A
//	pK = &(*m_pTaxListData)->NRec.KJval[KJIGLINE6];
//	m_Arith.l_sub(pK->KKval1, (*m_pTaxListData)->NRec.KJval[KJIGLINE4].KKval1, (*m_pTaxListData)->NRec.KJval[KJIGLINE5].KKval1);
//
//	// 差引金額(6)A
//	pK = &(*m_pTaxListData)->NRec.KJval[KJIGLINE7];
//	m_Arith.l_sub(pK->KKval1, (*m_pTaxListData)->NRec.KJval[KJIGLINE1].KKval1, (*m_pTaxListData)->NRec.KJval[KJIGLINE6].KKval1);
//
//
//	// 経費　計(32)
//	pK = &(*m_pTaxListData)->NRec.KJval[KJIGLINE32];
//	::ZeroMemory(pK->KKval1, 6);
//	::ZeroMemory(pK->KKval2, 6);
//	::ZeroMemory(pK->KKval4, 6);//Dの欄（うち税率６．３％適用分）
//	::ZeroMemory(pK->KKval5, 6);//Eの欄（うち税率６．２４％適用分）
//// 22-0167,22-0361 del -->
//	//::ZeroMemory( pK->KKval6, 6 );//Fの欄（うち税率７．８％適用分）
//// 22-0167,22-0361 del <--
//
//	for (int i = KJIGLINE8; i < KJIGLINE32; i++) {
//		m_Arith.l_add(pK->KKval1, pK->KKval1, (*m_pTaxListData)->NRec.KJval[i].KKval1);
//		if ((i == KJIGLINE10) || (i == KJIGLINE16) || (i == KJIGLINE17)) {//水道光熱費、修繕費、消耗品費はスキップ
//			;
//		}
//		else {
//			m_Arith.l_add(pK->KKval2, pK->KKval2, (*m_pTaxListData)->NRec.KJval[i].KKval2);
//		}
//	}
//	Shz_LinCalc(pK, 0);
//
//	// 22-0167,22-0361 del -->
//		// 自動計算の場合、Shz_LinCalcで横計が行われるのでクリアはここで行う
//	::ZeroMemory(pK->KKval6, 6);//Fの欄（うち税率７．８％適用分）
//// 22-0167,22-0361 del <--
//
//	for (int i = KJIGLINE8; i < KJIGLINE32; i++) {
//		if (i == KJIGLINE15 || i == KJIGLINE18 || i == KJIGLINE22 || i == KJIGLINE24) {//損害保険料、減価償却費、利子割引料、貸倒金はスキップ
//			continue;
//		}
//
//		//---->yoshida201216
//		else if (i == KJIGLINE8 || i == KJIGLINE9 || i == KJIGLINE10 || i == KJIGLINE11 || i == KJIGLINE16 || i == KJIGLINE20 || i == KJIGLINE21 || i == KJIGLINE23) {
//			if (((*m_pSnHeadData)->m_DispTabSgn & 0x1000) && !((*m_pSnHeadData)->Sn_Sign4 & 0x01)) {//新様式チェックあり かつ入力ロックじゃない時
//				//チェックある場合は該当箇所の6.24％部分は斜線になる為集計しない（金額クリアする）
//				::ZeroMemory((*m_pTaxListData)->NRec.KJval[i].KKval5, 6);
//			}
//			//Dの欄（うち税率６．３％適用分）
//			// 簡易課税では6.3%を常に表示するため計算する
//			//if ((*m_pSnHeadData)->SVmzsw == 1) {
//			//	m_Arith.l_add(pK->KKval4, pK->KKval4, (*m_pTaxListData)->NRec.KJval[i].KKval4);
//			//}
//			if ((Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI) || ((*m_pSnHeadData)->SVmzsw == 1)) {
//				// 簡易課税、もしくは旧税率が存在する場合
//				m_Arith.l_add(pK->KKval4, pK->KKval4, (*m_pTaxListData)->NRec.KJval[i].KKval4);
//			}
//			m_Arith.l_add(pK->KKval5, pK->KKval5, (*m_pTaxListData)->NRec.KJval[i].KKval5);//Eの欄（うち税率６．２４％適用分）
//			m_Arith.l_add(pK->KKval6, pK->KKval6, (*m_pTaxListData)->NRec.KJval[i].KKval6);//Fの欄（うち税率７．８％適用分）
//
//		}
//		//<-------------------
//
//		else {
//			if ((Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI) || ((*m_pSnHeadData)->SVmzsw == 1)) {
//				// 簡易課税、もしくは旧税率が存在する場合
//				m_Arith.l_add(pK->KKval4, pK->KKval4, (*m_pTaxListData)->NRec.KJval[i].KKval4);
//			}
//
//			m_Arith.l_add(pK->KKval5, pK->KKval5, (*m_pTaxListData)->NRec.KJval[i].KKval5);//Eの欄（うち税率６．２４％適用分）
//			m_Arith.l_add(pK->KKval6, pK->KKval6, (*m_pTaxListData)->NRec.KJval[i].KKval6);//Fの欄（うち税率７．８％適用分）
//
//		}
//	}
//
//
//	// 差引金額(33)A
//	pK = &(*m_pTaxListData)->NRec.KJval[KJIGLINE33];
//	m_Arith.l_sub(pK->KKval1, (*m_pTaxListData)->NRec.KJval[KJIGLINE7].KKval1, (*m_pTaxListData)->NRec.KJval[KJIGLINE32].KKval1);
//
//	// 3 + 32(34)
//	pK = &(*m_pTaxListData)->NRec.KJval[KJIGLINE34];
//	m_Arith.l_add(pK->KKval1, (*m_pTaxListData)->NRec.KJval[KJIGLINE3].KKval1, (*m_pTaxListData)->NRec.KJval[KJIGLINE32].KKval1);
//	m_Arith.l_add(pK->KKval3, (*m_pTaxListData)->NRec.KJval[KJIGLINE3].KKval3, (*m_pTaxListData)->NRec.KJval[KJIGLINE32].KKval3);
//
//
//	if ((Util.GetKazeihoushiki(m_pZmSub->zvol) == ID_ICSSH_KANNI) || ((*m_pSnHeadData)->SVmzsw == 1)) {
//		// 簡易課税、もしくは旧税率が存在する場合
//		m_Arith.l_add(pK->KKval4, (*m_pTaxListData)->NRec.KJval[KJIGLINE3].KKval4, (*m_pTaxListData)->NRec.KJval[KJIGLINE32].KKval4);
//	}
//	else {
//		::ZeroMemory(pK->KKval4, 6);
//	}
//	m_Arith.l_add(pK->KKval5, (*m_pTaxListData)->NRec.KJval[KJIGLINE3].KKval5, (*m_pTaxListData)->NRec.KJval[KJIGLINE32].KKval5);
//	m_Arith.l_add(pK->KKval6, (*m_pTaxListData)->NRec.KJval[KJIGLINE3].KKval6, (*m_pTaxListData)->NRec.KJval[KJIGLINE32].KKval6);
//
//	return 0;
//}
// インボイス_23/07/24_higuchi del <--

//-----------------------------------------------------------------------------
// 通常課税
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzTax31Calc::Shz_InvDataCpyUsu()
{
	::ZeroMemory(&inv_tenkidata, sizeof(inv_tenkidata));

	KKVAL*	pJ = (*m_pTaxListData)->NRec.KJval;
	KKVAL*	pF = (*m_pTaxListData)->NRec.KFval;
	KKVAL*	pN = (*m_pTaxListData)->NRec.KNval;

	KKVAL*	pU = (*m_pTaxListData)->NRec.KUval;
	KKVAL*	pS = (*m_pTaxListData)->NRec.KSval;
	char	val[6], val2[6], PW1[6], PW2[6];

	// 第27表-(1)号様式
	// ①1000円未満切り捨て廃止されたため変更
	::ZeroMemory(val, 6);
	//::ZeroMemory( PW1, 6 );
	//m_Arith.l_input( PW1, _T("1000") );
	//m_Arith.l_div( val, (pU+12)->KKval1, PW1 );//清水変更
	//m_Arith.l_mul( val, val, PW1 );
	memcpy(inv_tenkidata.Sn_1F1D, (pU + 12)->KKval1, 6);

	::ZeroMemory(val, 6);
	//::ZeroMemory( PW1, 6 );
	//m_Arith.l_input( PW1, _T("1000") );
	//m_Arith.l_div( val, (pU+11)->KKval1, PW1 );
	//m_Arith.l_mul( val, val, PW1 );
	memcpy(inv_tenkidata.Sn_1F1C, (pU + 11)->KKval1, 6);

	// ②
	::ZeroMemory(val, 6);
	::ZeroMemory(PW1, 6);
	::ZeroMemory(PW2, 6);
	m_Arith.l_input(PW1, _T("624"));
	m_Arith.l_input(PW2, _T("10000"));
	m_Arith.l_mul(val, inv_tenkidata.Sn_1F1C, PW1);
	m_Arith.l_div(val, val, PW2);
	memcpy(inv_tenkidata.Sn_1F2C, val, 6);

	::ZeroMemory(val, 6);
	::ZeroMemory(PW1, 6);
	::ZeroMemory(PW2, 6);
	m_Arith.l_input(PW1, _T("78"));
	m_Arith.l_input(PW2, _T("1000"));
	m_Arith.l_mul(val, inv_tenkidata.Sn_1F1D, PW1);
	m_Arith.l_div(val, val, PW2);
	memcpy(inv_tenkidata.Sn_1F2D, val, 6);

	// ④

	// ⑤ 返還等対価
	::ZeroMemory(val, 6);
	//--->修正依頼168643　戎居
	if ((*m_pTaxListData)->GKBN & 0x01) {	// 事業
		m_Arith.l_add(val, val, (pJ + KJIGLINE1TA)->KKval5);
	}
	if ((*m_pTaxListData)->GKBN & 0x02) {	// 不動産
		m_Arith.l_add(val, val, (pF + KFUDLINE4TA)->KKval5);
	}
	if ((*m_pTaxListData)->GKBN & 0x04) {	// 農業
		m_Arith.l_add(val, val, (pN + KNOULINE4TA)->KKval5);
	}
	//memcpy(val, (pJ + KJIGLINE1TA)->KKval5, 6);//清水変更
	//m_Arith.l_add(val, val, (pF + KFUDLINE4TA)->KKval5);//清水変更
	//m_Arith.l_add(val, val, (pN + KNOULINE4TA)->KKval5);//清水変更
	//<---
	if ((*m_pTaxListData)->Head.KHstax) {
		::ZeroMemory(val2, 6);

		m_Arith.l_input(PW1, _T("8"));
		m_Arith.l_input(PW2, _T("100"));
		m_Arith.l_mul(val2, val, PW1);
		m_Arith.l_div(val2, val2, PW2);

		m_Arith.l_add(val, val, val2);
	}
	::ZeroMemory(val2, 6);
	Shz_shohizei624(val2, val);
	memcpy(inv_tenkidata.Sn_1F5C, val2, 6);
	//追加
	::ZeroMemory(val, 6);
	//--->修正依頼168643　戎居
	if ((*m_pTaxListData)->GKBN & 0x01) {	// 事業
		m_Arith.l_add(val, val, (pJ + KJIGLINE1TA)->KKval6);
	}
	if ((*m_pTaxListData)->GKBN & 0x02) {	// 不動産
		m_Arith.l_add(val, val, (pF + KFUDLINE4TA)->KKval6);
	}
	if ((*m_pTaxListData)->GKBN & 0x04) {	// 農業
		m_Arith.l_add(val, val, (pN + KNOULINE4TA)->KKval6);
	}
	//memcpy(val, (pJ + KJIGLINE1TA)->KKval6, 6);//清水変更
	//m_Arith.l_add(val, val, (pF + KFUDLINE4TA)->KKval6);//清水変更
	//m_Arith.l_add(val, val, (pN + KNOULINE4TA)->KKval6);//清水変更
	//<---
	if ((*m_pTaxListData)->Head.KHstax) {
		::ZeroMemory(val2, 6);

		m_Arith.l_input(PW1, _T("10"));
		m_Arith.l_input(PW2, _T("100"));
		m_Arith.l_mul(val2, val, PW1);
		m_Arith.l_div(val2, val2, PW2);

		m_Arith.l_add(val, val, val2);
	}
	::ZeroMemory(val2, 6);
	Shz_shohizei78(val2, val);
	memcpy(inv_tenkidata.Sn_1F5D, val2, 6);

	//---------------

	// 付表2
	// ①
	::ZeroMemory(val, 6);
	memcpy(val, (pJ + KJIGLINE1TA)->KKval5, 6);//清水変更
	m_Arith.l_add(val, val, (pF + KFUDLINE4TA)->KKval5);//清水変更
	m_Arith.l_add(val, val, (pN + KNOULINE4TA)->KKval5);//清水変更
	if (!(*m_pTaxListData)->Head.KHstax) {
		//Shz_shohizei100_110( val, val );
		Shz_shohizei100_108(val, val);
	}
	m_Arith.l_sub(inv_tenkidata.Sn_2F1C, (pU + 11)->KKval1, val);

	//追加
	::ZeroMemory(val, 6);
	memcpy(val, (pJ + KJIGLINE1TA)->KKval6, 6);//清水変更33
	m_Arith.l_add(val, val, (pF + KFUDLINE4TA)->KKval6);//清水変更15
	m_Arith.l_add(val, val, (pN + KNOULINE4TA)->KKval6);//清水変更38
	if (!(*m_pTaxListData)->Head.KHstax) {
		Shz_shohizei100_110(val, val);//'15.01.09
	}
	m_Arith.l_sub(inv_tenkidata.Sn_2F1D, (pU + 12)->KKval1, val);

	//m_Arith.l_sub( tenkidata.Sn_2F1C, (pU+11)->KKval1, val );//清水変更16
	memcpy(inv_tenkidata.Sn_2F8C, (pS + 11)->KKval2, 6);//清水変更
	memcpy(inv_tenkidata.Sn_2F9C, (pS + 11)->KKval1, 6);//清水変更

	//m_Arith.l_sub( tenkidata.Sn_2F1D, (pU+18)->KKval1, val );//清水変更16
	memcpy(inv_tenkidata.Sn_2F8D, (pS + 12)->KKval2, 6);//清水変更
	memcpy(inv_tenkidata.Sn_2F9D, (pS + 12)->KKval1, 6);//清水変更

// インボイス_23/07/31_higuchi add -->
	memcpy(inv_tenkidata.Sn_2F11C, (pS + 13)->KKval2, 6);
	memcpy(inv_tenkidata.Sn_2F12C, (pS + 13)->KKval1, 6);
	memcpy(inv_tenkidata.Sn_2F11D, (pS + 14)->KKval2, 6);
	memcpy(inv_tenkidata.Sn_2F12D, (pS + 14)->KKval1, 6);
	// インボイス_23/07/31_higuchi add <--

	return 0;

}


//-----------------------------------------------------------------------------
// 簡易課税
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzTax31Calc::Shz_InvDataCpyKni()
{
	//経過措置用の処理を元にこの関数の処理作成しなおしyoshida200525

	KKVAL*	pU = (*m_pTaxListData)->NRec.KUval;
	KKVAL*	pJ = (*m_pTaxListData)->NRec.KJval;
	KKVAL*	pF = (*m_pTaxListData)->NRec.KFval;
	KKVAL*	pN = (*m_pTaxListData)->NRec.KNval;
	char	val[6], val2[6], PW1[6], PW2[6], clc[6];

	::ZeroMemory(&inv_tenkidata, sizeof(inv_tenkidata));

	// 課税標準額7.8％
	::ZeroMemory(val, 6);
	memcpy(inv_tenkidata.Sn_1F1D, (pU + 12)->KKval1, 6);

	// 課税標準額6.24％
	::ZeroMemory(val, 6);
	memcpy(inv_tenkidata.Sn_1F1C, (pU + 11)->KKval1, 6);

	//消費税率7.8%
	::ZeroMemory(val, 6);
	::ZeroMemory(PW1, 6);
	::ZeroMemory(PW2, 6);
	m_Arith.l_input(PW1, _T("78"));
	m_Arith.l_input(PW2, _T("1000"));
	m_Arith.l_mul(val, inv_tenkidata.Sn_1F1D, PW1);
	m_Arith.l_div(val, val, PW2);
	memcpy(inv_tenkidata.Sn_1F2D, val, 6);

	//消費税率6.24%
	::ZeroMemory(val, 6);
	::ZeroMemory(PW1, 6);
	::ZeroMemory(PW2, 6);
	m_Arith.l_input(PW1, _T("624"));
	m_Arith.l_input(PW2, _T("10000"));
	m_Arith.l_mul(val, inv_tenkidata.Sn_1F1C, PW1);
	m_Arith.l_div(val, val, PW2);
	memcpy(inv_tenkidata.Sn_1F2C, val, 6);


	//返還等課税取引7.8％を本表側の返還等対価に係る税額の7.8％へ
	if (Util.IsUnConnectMst(m_pZmSub->zvol)) {
		::ZeroMemory(clc, 6);
		for (int i = 0, id = 40; i < 5; i++) {//下if群２回通りidxが飛ばされるのでループは前と同じ５のまま。yoshida190728
//		for( int i=0, id=40; i<6; i++ ){
			::ZeroMemory(val, 6);
			memcpy(val, (pU + id)->KKval6, 6);
			if ((i == 2) || (i == 4)) {
				m_Arith.l_add(val, val, (pU + (id + 1))->KKval6);
				id++;
			}
			if ((*m_pTaxListData)->Head.KHstax) {
				::ZeroMemory(val2, 6);
				m_Arith.l_input(PW1, _T("10"));
				m_Arith.l_input(PW2, _T("100"));
				m_Arith.l_mul(val2, val, PW1);
				m_Arith.l_div(val2, val2, PW2);
				m_Arith.l_add(val, val, val2);
			}
			::ZeroMemory(val2, 6);
			Shz_shohizei78(val2, val);
			id++;
			m_Arith.l_add(clc, clc, val2);
		}
		memcpy(inv_tenkidata.Sn_1F5D, clc, 6);
	}
	else {
		::ZeroMemory(val, 6);
		memcpy(val, (pJ + KJIGLINE1TA)->KKval6, 6);
		m_Arith.l_add(val, val, (pF + KFUDLINE4TA)->KKval6);
		m_Arith.l_add(val, val, (pN + KNOULINE4TA)->KKval6);
		if ((*m_pTaxListData)->Head.KHstax) {
			::ZeroMemory(val2, 6);
			m_Arith.l_input(PW1, _T("10"));
			m_Arith.l_input(PW2, _T("100"));
			m_Arith.l_mul(val2, val, PW1);
			m_Arith.l_div(val2, val2, PW2);

			m_Arith.l_add(val, val, val2);
		}
		::ZeroMemory(val2, 6);
		Shz_shohizei78(val2, val);
		memcpy(inv_tenkidata.Sn_1F5D, val2, 6);
	}

	//返還等課税取引6.24％を本表側の返還等対価に係る税額の6.24％へ
	if (Util.IsUnConnectMst(m_pZmSub->zvol)) {
		::ZeroMemory(clc, 6);
		for (int i = 0, id = 40; i < 6; i++) {
			::ZeroMemory(val, 6);
			memcpy(val, (pU + id)->KKval8, 6);
			if (i == 1) {//第２種のループの際
				m_Arith.l_add(val, val, (pU + 49)->KKval8);//第２種農業の軽減分をプラス
			}
			else if (i == 2) {//第３種のループの際
				id++;
			}
			if ((*m_pTaxListData)->Head.KHstax) {
				::ZeroMemory(val2, 6);
				m_Arith.l_input(PW1, _T("8"));
				m_Arith.l_input(PW2, _T("100"));
				m_Arith.l_mul(val2, val, PW1);
				m_Arith.l_div(val2, val2, PW2);
				m_Arith.l_add(val, val, val2);
			}
			::ZeroMemory(val2, 6);
			Shz_shohizei624(val2, val);
			id++;
			m_Arith.l_add(clc, clc, val2);
		}
		memcpy(inv_tenkidata.Sn_1F5C, clc, 6);
	}
	else {
		::ZeroMemory(val, 6);
		memcpy(val, (pJ + KJIGLINE1TA)->KKval5, 6);
		m_Arith.l_add(val, val, (pF + KFUDLINE4TA)->KKval5);
		m_Arith.l_add(val, val, (pN + KNOULINE4TA)->KKval5);
		if ((*m_pTaxListData)->Head.KHstax) {
			::ZeroMemory(val2, 6);
			m_Arith.l_input(PW1, _T("8"));
			m_Arith.l_input(PW2, _T("100"));
			m_Arith.l_mul(val2, val, PW1);
			m_Arith.l_div(val2, val2, PW2);

			m_Arith.l_add(val, val, val2);
		}
		::ZeroMemory(val2, 6);
		Shz_shohizei624(val2, val);
		memcpy(inv_tenkidata.Sn_1F5C, val2, 6);
	}

	// 第１種～第５種までの金額を付表５－２に転記　7.8％分
	//課税取引金額　７．８％
	::ZeroMemory(clc, 6);
	if (Util.IsUnConnectMst(m_pZmSub->zvol)) {// 事業別売上高を利用していない場合は転記しない
		for (int i = 0, id = 40; i < 6; i++) {
			::ZeroMemory(val, 6);
			memcpy(val, (pU + id)->KKval5, 6);
			if (i == 2) {
				m_Arith.l_add(val, val, (pU + (id + 1))->KKval5);
			}
			else if (i == 3) {		// 10/04/06 追加
				m_Arith.l_add(val, val, (pU + 48)->KKval5);
			}
			if (!(*m_pTaxListData)->Head.KHstax) {
				Shz_shohizei100_110(val, val);
			}

			::ZeroMemory(val2, 6);
			memcpy(val2, (pU + id)->KKval6, 6);
			if (i == 2) {
				m_Arith.l_add(val2, val2, (pU + (id + 1))->KKval6);
				id++;
			}
			if (!(*m_pTaxListData)->Head.KHstax) {
				Shz_shohizei100_110(val2, val2);
			}
			id++;
			m_Arith.l_sub(val, val, val2);

			switch (i) {
			case 0:
				memcpy(inv_tenkidata.Sn_5FURI1D, val, 6);
				break;
			case 1:
				memcpy(inv_tenkidata.Sn_5FURI2D, val, 6);
				break;

			case 2:
				memcpy(inv_tenkidata.Sn_5FURI3D, val, 6);
				break;

			case 3:
				memcpy(inv_tenkidata.Sn_5FURI4D, val, 6);
				break;

			case 4:
				memcpy(inv_tenkidata.Sn_5FURI5D, val, 6);
				break;

			case 5:
				memcpy(inv_tenkidata.Sn_5FURI6D, val, 6);
				break;
			}
			m_Arith.l_add(clc, clc, val);
		}
	}
	else {
		::ZeroMemory(inv_tenkidata.Sn_5FURI1D, 6);
		::ZeroMemory(inv_tenkidata.Sn_5FURI2D, 6);
		::ZeroMemory(inv_tenkidata.Sn_5FURI3D, 6);
		::ZeroMemory(inv_tenkidata.Sn_5FURI4D, 6);
		::ZeroMemory(inv_tenkidata.Sn_5FURI5D, 6);
		::ZeroMemory(inv_tenkidata.Sn_5FURI6D, 6);
	}

	// 第１種～第５種までの金額を付表５－２に転記　6.24％分

	//課税取引金額６．２４％（返還等課税取引引く）

	//修正168614　戎居
	//::ZeroMemory(clc, 6);
	if (Util.IsUnConnectMst(m_pZmSub->zvol)) {// 事業別売上高を利用していない場合は転記しない
		//yoshida190728
		for (int i = 0, id = 40; i < 6; i++) {
			::ZeroMemory(val, 6);
			memcpy(val, (pU + id)->KKval7, 6);
			if (i == 1) {//第２種農業分足す
				m_Arith.l_add(val, val, (pU + 49)->KKval7);
			}
			else if (i == 3) {
				m_Arith.l_add(val, val, (pU + 48)->KKval7);
			}
			if (!(*m_pTaxListData)->Head.KHstax) {
				Shz_shohizei100_108(val, val);
			}

			::ZeroMemory(val2, 6);
			memcpy(val2, (pU + id)->KKval8, 6);
			if (i == 1) {//第２種農業分足す
				m_Arith.l_add(val2, val2, (pU + 49)->KKval8);
			}
			else if (i == 2) {
				id++;
			}
			if (!(*m_pTaxListData)->Head.KHstax) {
				Shz_shohizei100_108(val2, val2);
			}
			id++;
			m_Arith.l_sub(val, val, val2);


			switch (i) {
			case 0:
				memcpy(inv_tenkidata.Sn_5FURI1C, val, 6);
				break;
			case 1:
				memcpy(inv_tenkidata.Sn_5FURI2C, val, 6);
				break;

			case 2:
				memcpy(inv_tenkidata.Sn_5FURI3C, val, 6);
				break;

			case 3:
				memcpy(inv_tenkidata.Sn_5FURI4C, val, 6);
				break;

			case 4:
				memcpy(inv_tenkidata.Sn_5FURI5C, val, 6);
				break;

			case 5:
				memcpy(inv_tenkidata.Sn_5FURI6C, val, 6);
				break;
			}
			m_Arith.l_add(clc, clc, val);
		}
	}
	else {
		::ZeroMemory(inv_tenkidata.Sn_5FURI1C, 6);
		::ZeroMemory(inv_tenkidata.Sn_5FURI2C, 6);
		::ZeroMemory(inv_tenkidata.Sn_5FURI3C, 6);
		::ZeroMemory(inv_tenkidata.Sn_5FURI4C, 6);
		::ZeroMemory(inv_tenkidata.Sn_5FURI5C, 6);
		::ZeroMemory(inv_tenkidata.Sn_5FURI6C, 6);
	}

	// 付表5
	if (Util.IsUnConnectMst(m_pZmSub->zvol)) {// 事業別売上高を利用していない場合は転記しない
		memcpy(inv_tenkidata.Sn_5FURITA, clc, 6);
	}
	else{//転記しない場合は初期化
		::ZeroMemory(inv_tenkidata.Sn_5FURITA, 6);
	}

	::ZeroMemory(inv_tenkidata.Sn_5FUZTT, 6);
	::ZeroMemory(inv_tenkidata.Sn_5FUZT1, 6);
	::ZeroMemory(inv_tenkidata.Sn_5FUZT2, 6);
	::ZeroMemory(inv_tenkidata.Sn_5FUZT3, 6);
	::ZeroMemory(inv_tenkidata.Sn_5FUZT4, 6);
	::ZeroMemory(inv_tenkidata.Sn_5FUZT5, 6);
	::ZeroMemory(inv_tenkidata.Sn_5FUZT6, 6);	// '16.12.27

	return 0;
}

//-----------------------------------------------------------------------------
// 非連動で簡易課税(インボイス期間の経過措置用)
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzTax31Calc::Shz_InvDataCpyKniKei()
{
	KKVAL*	pU = (*m_pTaxListData)->NRec.KUval;
	//2017.12.04 INSERT START
	KKVAL*	pJ = (*m_pTaxListData)->NRec.KJval;
	KKVAL*	pF = (*m_pTaxListData)->NRec.KFval;
	KKVAL*	pN = (*m_pTaxListData)->NRec.KNval;
	//2017.12.04 INSERT END
	char	val[6], val2[6], PW1[6], PW2[6], clc[6];

	::ZeroMemory(&inv_tenkidata, sizeof(inv_tenkidata));

	// ①1000円未満切り捨て廃止されたため変更
	// 課税標準額7.8％
	::ZeroMemory(val, 6);
	//::ZeroMemory( PW1, 6 );
	//m_Arith.l_input( PW1, _T("1000") );
	//m_Arith.l_div( val, (pU+12)->KKval1, PW1 );
	//m_Arith.l_mul( val, val, PW1 );
	memcpy(inv_tenkidata.Sn_1F1D, (pU + 12)->KKval1, 6);
	// 課税標準額6.24％
	::ZeroMemory(val, 6);
	//::ZeroMemory( PW1, 6 );
	//m_Arith.l_input( PW1, _T("1000") );
	//m_Arith.l_div( val, (pU+11)->KKval1, PW1 );
	//m_Arith.l_mul( val, val, PW1 );
	memcpy(inv_tenkidata.Sn_1F1C, (pU + 11)->KKval1, 6);
	// 課税標準額6.3％
	::ZeroMemory(val, 6);
	//::ZeroMemory( PW1, 6 );
	//m_Arith.l_input( PW1, _T("1000") );
	//m_Arith.l_div( val, (pU+10)->KKval1, PW1 );
	//m_Arith.l_mul( val, val, PW1 );
	memcpy(inv_tenkidata.Sn_1F1B, (pU + 10)->KKval1, 6);

	//消費税率7.8%
	::ZeroMemory(val, 6);
	::ZeroMemory(PW1, 6);
	::ZeroMemory(PW2, 6);
	m_Arith.l_input(PW1, _T("78"));
	m_Arith.l_input(PW2, _T("1000"));
	m_Arith.l_mul(val, inv_tenkidata.Sn_1F1D, PW1);
	m_Arith.l_div(val, val, PW2);
	memcpy(inv_tenkidata.Sn_1F2D, val, 6);

	//消費税率6.24%
	::ZeroMemory(val, 6);
	::ZeroMemory(PW1, 6);
	::ZeroMemory(PW2, 6);
	m_Arith.l_input(PW1, _T("624"));
	m_Arith.l_input(PW2, _T("10000"));
	m_Arith.l_mul(val, inv_tenkidata.Sn_1F1C, PW1);
	m_Arith.l_div(val, val, PW2);
	memcpy(inv_tenkidata.Sn_1F2C, val, 6);

	//消費税率6.3%
	::ZeroMemory(val, 6);
	::ZeroMemory(PW1, 6);
	::ZeroMemory(PW2, 6);
	m_Arith.l_input(PW1, _T("63"));
	m_Arith.l_input(PW2, _T("1000"));//yoshida200127
//	m_Arith.l_input( PW2, _T("100") );
	m_Arith.l_mul(val, inv_tenkidata.Sn_1F1B, PW1);
	m_Arith.l_div(val, val, PW2);
	memcpy(inv_tenkidata.Sn_1F2B, val, 6);


	//返還等課税取引7.8％を本表側の返還等対価に係る税額の7.8％へ
	if (Util.IsUnConnectMst(m_pZmSub->zvol)) {
		::ZeroMemory(clc, 6);
		for (int i = 0, id = 40; i < 5; i++) {//下if群２回通りidxが飛ばされるのでループは前と同じ５のまま。yoshida190728
//		for( int i=0, id=40; i<6; i++ ){
			::ZeroMemory(val, 6);
			memcpy(val, (pU + id)->KKval6, 6);
			if ((i == 2) || (i == 4)) {
				m_Arith.l_add(val, val, (pU + (id + 1))->KKval6);
				id++;
			}
			if ((*m_pTaxListData)->Head.KHstax) {
				::ZeroMemory(val2, 6);
				m_Arith.l_input(PW1, _T("10"));
				m_Arith.l_input(PW2, _T("100"));
				m_Arith.l_mul(val2, val, PW1);
				m_Arith.l_div(val2, val2, PW2);
				m_Arith.l_add(val, val, val2);
			}
			::ZeroMemory(val2, 6);
			Shz_shohizei78(val2, val);
			id++;
			m_Arith.l_add(clc, clc, val2);
		}
		memcpy(inv_tenkidata.Sn_1F5D, clc, 6);
	}
	else {
		::ZeroMemory(val, 6);
		//memcpy( val, (pJ+KJIGLINE1TA)->KKval5, 6 );
		//m_Arith.l_add( val, val, (pF+KFUDLINE4TA)->KKval5 );
		//m_Arith.l_add( val, val, (pN+KNOULINE4TA)->KKval5 );
		memcpy(val, (pJ + KJIGLINE1TA)->KKval6, 6);
		m_Arith.l_add(val, val, (pF + KFUDLINE4TA)->KKval6);
		m_Arith.l_add(val, val, (pN + KNOULINE4TA)->KKval6);
		if ((*m_pTaxListData)->Head.KHstax) {
			::ZeroMemory(val2, 6);
			m_Arith.l_input(PW1, _T("10"));
			m_Arith.l_input(PW2, _T("100"));
			m_Arith.l_mul(val2, val, PW1);
			m_Arith.l_div(val2, val2, PW2);

			m_Arith.l_add(val, val, val2);
		}
		::ZeroMemory(val2, 6);
		Shz_shohizei78(val2, val);
		memcpy(inv_tenkidata.Sn_1F5D, val2, 6);
	}

	//返還等課税取引6.24％を本表側の返還等対価に係る税額の6.24％へ
	if (Util.IsUnConnectMst(m_pZmSub->zvol)) {
		::ZeroMemory(clc, 6);

		//yoshida190728
		for (int i = 0, id = 40; i < 6; i++) {
			::ZeroMemory(val, 6);
			memcpy(val, (pU + id)->KKval8, 6);
			if (i == 1) {//第２種のループの際
				m_Arith.l_add(val, val, (pU + 49)->KKval8);//第２種農業の軽減分をプラス
			}
			else if (i == 2) {//第３種のループの際
				id++;
			}
			if ((*m_pTaxListData)->Head.KHstax) {
				::ZeroMemory(val2, 6);
				m_Arith.l_input(PW1, _T("8"));
				m_Arith.l_input(PW2, _T("100"));
				m_Arith.l_mul(val2, val, PW1);
				m_Arith.l_div(val2, val2, PW2);
				m_Arith.l_add(val, val, val2);
			}
			::ZeroMemory(val2, 6);
			Shz_shohizei624(val2, val);
			id++;
			m_Arith.l_add(clc, clc, val2);
		}
		memcpy(inv_tenkidata.Sn_1F5C, clc, 6);
	}
	else {
		::ZeroMemory(val, 6);
		//memcpy( val, (pJ+KJIGLINE1TA)->KKval7, 6 );
		//m_Arith.l_add( val, val, (pF+KFUDLINE4TA)->KKval7 );
		//m_Arith.l_add( val, val, (pN+KNOULINE4TA)->KKval7 );
		memcpy(val, (pJ + KJIGLINE1TA)->KKval5, 6);
		m_Arith.l_add(val, val, (pF + KFUDLINE4TA)->KKval5);
		m_Arith.l_add(val, val, (pN + KNOULINE4TA)->KKval5);
		if ((*m_pTaxListData)->Head.KHstax) {
			::ZeroMemory(val2, 6);
			m_Arith.l_input(PW1, _T("8"));
			m_Arith.l_input(PW2, _T("100"));
			m_Arith.l_mul(val2, val, PW1);
			m_Arith.l_div(val2, val2, PW2);

			m_Arith.l_add(val, val, val2);
		}
		::ZeroMemory(val2, 6);
		Shz_shohizei624(val2, val);
		memcpy(inv_tenkidata.Sn_1F5C, val2, 6);
	}

	//返還等課税取引6.3％を本表側の返還等対価に係る税額の6.3％へ
	::ZeroMemory(clc, 6);
	if (Util.IsUnConnectMst(m_pZmSub->zvol)) {
		for (int i = 0, id = 40; i < 6; i++) {
			::ZeroMemory(val, 6);
			memcpy(val, (pU + id)->KKval10, 6);
			if (i == 2) {
				m_Arith.l_add(val, val, (pU + (id + 1))->KKval10);
				id++;
			}
			if ((*m_pTaxListData)->Head.KHstax) {
				::ZeroMemory(val2, 6);

				m_Arith.l_input(PW1, _T("8"));
				m_Arith.l_input(PW2, _T("100"));
				m_Arith.l_mul(val2, val, PW1);
				m_Arith.l_div(val2, val2, PW2);
				m_Arith.l_add(val, val, val2);
			}
			::ZeroMemory(val2, 6);
			Shz_shohizei63(val2, val);
			id++;
			m_Arith.l_add(clc, clc, val2);
		}
		memcpy(inv_tenkidata.Sn_1F5B, clc, 6);
		//2017.12.04 INSERT START
	}
	else {
		::ZeroMemory(val, 6);
		//memcpy( val, (pJ+KJIGLINE1TA)->KKval9, 6 );
		//m_Arith.l_add( val, val, (pF+KFUDLINE4TA)->KKval9 );
		//m_Arith.l_add( val, val, (pN+KNOULINE4TA)->KKval9 );
		memcpy(val, (pJ + KJIGLINE1TA)->KKval4, 6);
		m_Arith.l_add(val, val, (pF + KFUDLINE4TA)->KKval4);
		m_Arith.l_add(val, val, (pN + KNOULINE4TA)->KKval4);
		if ((*m_pTaxListData)->Head.KHstax) {
			::ZeroMemory(val2, 6);


			m_Arith.l_input(PW1, _T("8"));
			m_Arith.l_input(PW2, _T("100"));
			m_Arith.l_mul(val2, val, PW1);
			m_Arith.l_div(val2, val2, PW2);

			m_Arith.l_add(val, val, val2);
		}
		::ZeroMemory(val2, 6);
		Shz_shohizei63(val2, val);
		memcpy(inv_tenkidata.Sn_1F5B, val2, 6);
	}

	// 第１種～第５種までの金額を付表５－２に転記　7.8％分

	//課税取引金額　７．８％
	::ZeroMemory(clc, 6);
	if (Util.IsUnConnectMst(m_pZmSub->zvol)) {// 事業別売上高を利用していない場合は転記しない
		for (int i = 0, id = 40; i < 6; i++) {
			::ZeroMemory(val, 6);
			memcpy(val, (pU + id)->KKval5, 6);
			if (i == 2) {
				m_Arith.l_add(val, val, (pU + (id + 1))->KKval5);
			}
			else if (i == 3) {		// 10/04/06 追加
				m_Arith.l_add(val, val, (pU + 48)->KKval5);
			}
			if (!(*m_pTaxListData)->Head.KHstax) {
				Shz_shohizei100_110(val, val);
			}

			::ZeroMemory(val2, 6);
			memcpy(val2, (pU + id)->KKval6, 6);
			if (i == 2) {
				m_Arith.l_add(val2, val2, (pU + (id + 1))->KKval6);
				id++;
			}
			if (!(*m_pTaxListData)->Head.KHstax) {
				Shz_shohizei100_110(val2, val2);
			}
			id++;
			m_Arith.l_sub(val, val, val2);

			//-->yoshida190806
			//マイナス値の場合も転記すると依頼（連動がその為）修正依頼６９５
			/*
			//マイナス値の場合には０にして転記する
			if( m_Arith.l_test(val) < 0 ){
				::ZeroMemory(val , sizeof(val));
			}
			*/
			//<------

			switch (i) {
			case 0:
				memcpy(inv_tenkidata.Sn_5FURI1D, val, 6);
				break;
			case 1:
				memcpy(inv_tenkidata.Sn_5FURI2D, val, 6);
				break;

			case 2:
				memcpy(inv_tenkidata.Sn_5FURI3D, val, 6);
				break;

			case 3:
				memcpy(inv_tenkidata.Sn_5FURI4D, val, 6);
				break;

			case 4:
				memcpy(inv_tenkidata.Sn_5FURI5D, val, 6);
				break;

			case 5:
				memcpy(inv_tenkidata.Sn_5FURI6D, val, 6);
				break;
			}
			m_Arith.l_add(clc, clc, val);
		}
	}
	else {
	::ZeroMemory(inv_tenkidata.Sn_5FURI1D, 6);
	::ZeroMemory(inv_tenkidata.Sn_5FURI2D, 6);
	::ZeroMemory(inv_tenkidata.Sn_5FURI3D, 6);
	::ZeroMemory(inv_tenkidata.Sn_5FURI4D, 6);
	::ZeroMemory(inv_tenkidata.Sn_5FURI5D, 6);
	::ZeroMemory(inv_tenkidata.Sn_5FURI6D, 6);
	}

	// 第１種～第５種までの金額を付表５－２に転記　6.24％分

	//課税取引金額６．２４％（返還等課税取引引く）

	//修正168614　戎居
	//::ZeroMemory(clc, 6);
	if (Util.IsUnConnectMst(m_pZmSub->zvol)) {// 事業別売上高を利用していない場合は転記しない
		//yoshida190728
		for (int i = 0, id = 40; i < 6; i++) {
			::ZeroMemory(val, 6);
			memcpy(val, (pU + id)->KKval7, 6);
			if (i == 1) {//第２種農業分足す
				m_Arith.l_add(val, val, (pU + 49)->KKval7);
			}
			else if (i == 3) {
				m_Arith.l_add(val, val, (pU + 48)->KKval7);
			}
			if (!(*m_pTaxListData)->Head.KHstax) {
				Shz_shohizei100_108(val, val);
			}

			::ZeroMemory(val2, 6);
			memcpy(val2, (pU + id)->KKval8, 6);

			//-->yoshida190809
			if (i == 1) {//第２種農業分足す
				m_Arith.l_add(val2, val2, (pU + 49)->KKval8);
			}
			//--->yoshida190812
			else if (i == 2) {
				id++;
			}
			//----
	//		if (i == 2) {
	//			m_Arith.l_add(val2, val2, (pU + 49)->KKval8);
	//			id++;
	//		}
			//<-------
			if (!(*m_pTaxListData)->Head.KHstax) {
				Shz_shohizei100_108(val2, val2);
			}
			id++;
			m_Arith.l_sub(val, val, val2);

			//-->yoshida190806
			//マイナス値の場合も転記すると依頼（連動がその為）修正依頼６９５
			/*
			//マイナス値の場合には０にして転記する
			if (m_Arith.l_test(val) < 0) {
				::ZeroMemory(val, sizeof(val));
			}
			*/
			//<------

			switch (i) {
			case 0:
				memcpy(inv_tenkidata.Sn_5FURI1C, val, 6);
				break;
			case 1:
				memcpy(inv_tenkidata.Sn_5FURI2C, val, 6);
				break;

			case 2:
				memcpy(inv_tenkidata.Sn_5FURI3C, val, 6);
				break;

			case 3:
				memcpy(inv_tenkidata.Sn_5FURI4C, val, 6);
				break;

			case 4:
				memcpy(inv_tenkidata.Sn_5FURI5C, val, 6);
				break;

			case 5:
				memcpy(inv_tenkidata.Sn_5FURI6C, val, 6);
				break;
			}
			m_Arith.l_add(clc, clc, val);
		}
	}
	else {
		::ZeroMemory(inv_tenkidata.Sn_5FURI1C, 6);
		::ZeroMemory(inv_tenkidata.Sn_5FURI2C, 6);
		::ZeroMemory(inv_tenkidata.Sn_5FURI3C, 6);
		::ZeroMemory(inv_tenkidata.Sn_5FURI4C, 6);
		::ZeroMemory(inv_tenkidata.Sn_5FURI5C, 6);
		::ZeroMemory(inv_tenkidata.Sn_5FURI6C, 6);
	}

	// 第１種～第５種までの金額を付表５－２に転記　6.3％分
	if (Util.IsUnConnectMst(m_pZmSub->zvol)) {// 事業別売上高を利用していない場合は転記しない
		for (int i = 0, id = 40; i < 6; i++) {
			::ZeroMemory(val, 6);
			memcpy(val, (pU + id)->KKval9, 6);
			if (i == 2) {
				m_Arith.l_add(val, val, (pU + (id + 1))->KKval9);
			}
			else if (i == 3) {		// 10/04/06 追加
				m_Arith.l_add(val, val, (pU + 48)->KKval9);
			}
			if (!(*m_pTaxListData)->Head.KHstax) {
				Shz_shohizei100_108(val, val);
			}

			::ZeroMemory(val2, 6);
			memcpy(val2, (pU + id)->KKval10, 6);
			if (i == 2) {
				m_Arith.l_add(val2, val2, (pU + (id + 1))->KKval10);
				id++;
			}
			if (!(*m_pTaxListData)->Head.KHstax) {
				Shz_shohizei100_108(val2, val2);
			}
			id++;
			m_Arith.l_sub(val, val, val2);

			//-->yoshida190806
			//マイナス値の場合も転記すると依頼（連動がその為）修正依頼６９５
			/*
			//マイナス値の場合には０にして転記する
			if( m_Arith.l_test(val) < 0 ){
				::ZeroMemory(val , sizeof(val));
			}
			*/
			//<------

			switch (i) {
			case 0:
				memcpy(inv_tenkidata.Sn_5FURI1B, val, 6);
				break;
			case 1:
				memcpy(inv_tenkidata.Sn_5FURI2B, val, 6);
				break;

			case 2:
				memcpy(inv_tenkidata.Sn_5FURI3B, val, 6);
				break;

			case 3:
				memcpy(inv_tenkidata.Sn_5FURI4B, val, 6);
				break;

			case 4:
				memcpy(inv_tenkidata.Sn_5FURI5B, val, 6);
				break;

			case 5:
				memcpy(inv_tenkidata.Sn_5FURI6B, val, 6);
				break;
			}
			m_Arith.l_add(clc, clc, val);
		}
	}
	else {
		::ZeroMemory(inv_tenkidata.Sn_5FURI1B, 6);
		::ZeroMemory(inv_tenkidata.Sn_5FURI2B, 6);
		::ZeroMemory(inv_tenkidata.Sn_5FURI3B, 6);
		::ZeroMemory(inv_tenkidata.Sn_5FURI4B, 6);
		::ZeroMemory(inv_tenkidata.Sn_5FURI5B, 6);
		::ZeroMemory(inv_tenkidata.Sn_5FURI6B, 6);
	}

	// 付表5
	if (Util.IsUnConnectMst(m_pZmSub->zvol)) {// 事業別売上高を利用していない場合は転記しない
		memcpy(inv_tenkidata.Sn_5FURITA, clc, 6);
	}
	else {//転記しない場合は初期化
		::ZeroMemory(inv_tenkidata.Sn_5FURITA, 6);
	}

	::ZeroMemory(inv_tenkidata.Sn_5FUZTT, 6);
	::ZeroMemory(inv_tenkidata.Sn_5FUZT1, 6);
	::ZeroMemory(inv_tenkidata.Sn_5FUZT2, 6);
	::ZeroMemory(inv_tenkidata.Sn_5FUZT3, 6);
	::ZeroMemory(inv_tenkidata.Sn_5FUZT4, 6);
	::ZeroMemory(inv_tenkidata.Sn_5FUZT5, 6);
	::ZeroMemory(inv_tenkidata.Sn_5FUZT6, 6);	// '16.12.27

	return 0;
}

//-----------------------------------------------------------------------------
// 非連動で通常課税(経過措置用)
//-----------------------------------------------------------------------------
// 返送値
//-----------------------------------------------------------------------------
int	CShzTax31Calc::Shz_InvDataCpyUsuKei()
{

	::ZeroMemory(&inv_tenkidata, sizeof(inv_tenkidata));

	KKVAL*	pJ = (*m_pTaxListData)->NRec.KJval;
	KKVAL*	pF = (*m_pTaxListData)->NRec.KFval;
	KKVAL*	pN = (*m_pTaxListData)->NRec.KNval;

	KKVAL*	pU = (*m_pTaxListData)->NRec.KUval;
	KKVAL*	pS = (*m_pTaxListData)->NRec.KSval;
	char	val[6], val2[6], PW1[6], PW2[6];

	// ①1000円未満切り捨て廃止されたため変更
	// 第27表-(1)号様式
	// 課税標準額7.8%
	::ZeroMemory(val, 6);
	//::ZeroMemory( PW1, 6 );
	//m_Arith.l_input( PW1, _T("1000") );
	//m_Arith.l_div( val, (pU+12)->KKval1, PW1 );
	//m_Arith.l_mul( val, val, PW1 );
	memcpy(inv_tenkidata.Sn_1F1D, (pU + 12)->KKval1, 6);

	// 課税標準額6.24％
	::ZeroMemory(val, 6);
	//::ZeroMemory( PW1, 6 );
	//m_Arith.l_input( PW1, _T("1000") );
	//m_Arith.l_div( val, (pU+11)->KKval1, PW1 );
	//m_Arith.l_mul( val, val, PW1 );
	memcpy(inv_tenkidata.Sn_1F1C, (pU + 11)->KKval1, 6);

	// 課税標準額6.3％
	::ZeroMemory(val, 6);
	//::ZeroMemory( PW1, 6 );
	//m_Arith.l_input( PW1, _T("1000") );
	//m_Arith.l_div( val, (pU+10)->KKval1, PW1 );
	//m_Arith.l_mul( val, val, PW1 );
	memcpy(inv_tenkidata.Sn_1F1B, (pU + 10)->KKval1, 6);

	//消費税率7.8%
	::ZeroMemory(val, 6);
	::ZeroMemory(PW1, 6);
	::ZeroMemory(PW2, 6);
	m_Arith.l_input(PW1, _T("78"));
	m_Arith.l_input(PW2, _T("1000"));
	m_Arith.l_mul(val, inv_tenkidata.Sn_1F1D, PW1);
	m_Arith.l_div(val, val, PW2);
	memcpy(inv_tenkidata.Sn_1F2D, val, 6);

	//消費税率6.24%
	::ZeroMemory(val, 6);
	::ZeroMemory(PW1, 6);
	::ZeroMemory(PW2, 6);
	m_Arith.l_input(PW1, _T("624"));
	m_Arith.l_input(PW2, _T("10000"));
	m_Arith.l_mul(val, inv_tenkidata.Sn_1F1C, PW1);
	m_Arith.l_div(val, val, PW2);
	memcpy(inv_tenkidata.Sn_1F2C, val, 6);

	//消費税率6.3%
	::ZeroMemory(val, 6);
	::ZeroMemory(PW1, 6);
	::ZeroMemory(PW2, 6);
	m_Arith.l_input(PW1, _T("63"));
	m_Arith.l_input(PW2, _T("1000"));//yoshida200127
//	m_Arith.l_input( PW2, _T("100") );
	m_Arith.l_mul(val, inv_tenkidata.Sn_1F1B, PW1);
	m_Arith.l_div(val, val, PW2);
	memcpy(inv_tenkidata.Sn_1F2B, val, 6);

	//返還等課税取引7.8％を本表側の返還等対価に係る税額の7.8％へ
	::ZeroMemory(val, 6);
	//--->修正依頼168643　戎居
	if ((*m_pTaxListData)->GKBN & 0x01) {	// 事業
		m_Arith.l_add(val, val, (pJ + KJIGLINE1TA)->KKval6);
	}
	if ((*m_pTaxListData)->GKBN & 0x02) {	// 不動産
		m_Arith.l_add(val, val, (pF + KFUDLINE4TA)->KKval6);
	}
	if ((*m_pTaxListData)->GKBN & 0x04) {	// 農業
		m_Arith.l_add(val, val, (pN + KNOULINE4TA)->KKval6);
	}
	//memcpy(val, (pJ + KJIGLINE1TA)->KKval6, 6);
	//m_Arith.l_add(val, val, (pF + KFUDLINE4TA)->KKval6);
	//m_Arith.l_add(val, val, (pN + KNOULINE4TA)->KKval6);
	//<---
	if ((*m_pTaxListData)->Head.KHstax) {
		::ZeroMemory(val2, 6);
		m_Arith.l_input(PW1, _T("10"));
		m_Arith.l_input(PW2, _T("100"));
		m_Arith.l_mul(val2, val, PW1);
		m_Arith.l_div(val2, val2, PW2);

		m_Arith.l_add(val, val, val2);
	}
	::ZeroMemory(val2, 6);
	Shz_shohizei78(val2, val);
	memcpy(inv_tenkidata.Sn_1F5D, val2, 6);//'15.01.09

	//返還等課税取引6.24％を本表側の返還等対価に係る税額の6.24％へ
	::ZeroMemory(val, 6);
	//--->修正依頼168643　戎居
	if ((*m_pTaxListData)->GKBN & 0x01) {	// 事業
		m_Arith.l_add(val, val, (pJ + KJIGLINE1TA)->KKval5);
	}
	if ((*m_pTaxListData)->GKBN & 0x02) {	// 不動産
		m_Arith.l_add(val, val, (pF + KFUDLINE4TA)->KKval5);
	}
	if ((*m_pTaxListData)->GKBN & 0x04) {	// 農業
		m_Arith.l_add(val, val, (pN + KNOULINE4TA)->KKval5);
	}
	//memcpy(val, (pJ + KJIGLINE1TA)->KKval5, 6);
	//m_Arith.l_add(val, val, (pF + KFUDLINE4TA)->KKval5);
	//m_Arith.l_add(val, val, (pN + KNOULINE4TA)->KKval5);
	//<---
	if ((*m_pTaxListData)->Head.KHstax) {
		::ZeroMemory(val2, 6);
		m_Arith.l_input(PW1, _T("8"));
		m_Arith.l_input(PW2, _T("100"));
		m_Arith.l_mul(val2, val, PW1);
		m_Arith.l_div(val2, val2, PW2);

		m_Arith.l_add(val, val, val2);
	}
	::ZeroMemory(val2, 6);
	Shz_shohizei624(val2, val);
	memcpy(inv_tenkidata.Sn_1F5C, val2, 6);//'15.01.09

	//返還等課税取引6.3％を本表側の返還等対価に係る税額の6.3％へ
	//------->'15.01.09
	::ZeroMemory(val, 6);
	memcpy(val, (pJ + KJIGLINE1TA)->KKval4, 6);
	m_Arith.l_add(val, val, (pF + KFUDLINE4TA)->KKval4);
	m_Arith.l_add(val, val, (pN + KNOULINE4TA)->KKval4);
	if ((*m_pTaxListData)->Head.KHstax) {
		::ZeroMemory(val2, 6);
		m_Arith.l_input(PW1, _T("8"));
		m_Arith.l_input(PW2, _T("100"));
		m_Arith.l_mul(val2, val, PW1);
		m_Arith.l_div(val2, val2, PW2);

		m_Arith.l_add(val, val, val2);
	}
	::ZeroMemory(val2, 6);
	Shz_shohizei63(val2, val);
	memcpy(inv_tenkidata.Sn_1F5B, val2, 6);
	//<---------------------------

	// 付表2
	// ①
	::ZeroMemory(val, 6);
	memcpy(val, (pJ + KJIGLINE1TA)->KKval6, 6);
	m_Arith.l_add(val, val, (pF + KFUDLINE4TA)->KKval6);
	m_Arith.l_add(val, val, (pN + KNOULINE4TA)->KKval6);
	if (!(*m_pTaxListData)->Head.KHstax) {
		Shz_shohizei100_110(val, val);
	}

	m_Arith.l_sub(inv_tenkidata.Sn_2F1D, (pU + 12)->KKval1, val);

	::ZeroMemory(val, 6);
	memcpy(val, (pJ + KJIGLINE1TA)->KKval5, 6);
	m_Arith.l_add(val, val, (pF + KFUDLINE4TA)->KKval5);
	m_Arith.l_add(val, val, (pN + KNOULINE4TA)->KKval5);
	if (!(*m_pTaxListData)->Head.KHstax) {
		//Shz_shohizei100_110( val, val );//'15.01.09
		Shz_shohizei100_108(val, val);
	}

	m_Arith.l_sub(inv_tenkidata.Sn_2F1C, (pU + 11)->KKval1, val);

	::ZeroMemory(val, 6);
	memcpy(val, (pJ + KJIGLINE1TA)->KKval4, 6);
	m_Arith.l_add(val, val, (pF + KFUDLINE4TA)->KKval4);
	m_Arith.l_add(val, val, (pN + KNOULINE4TA)->KKval4);
	if (!(*m_pTaxListData)->Head.KHstax) {
		Shz_shohizei100_108(val, val);//'15.01.09
	}

	m_Arith.l_sub(inv_tenkidata.Sn_2F1B, (pU + 10)->KKval1, val);

	memcpy(inv_tenkidata.Sn_2F8D, (pS + 12)->KKval2, 6);
	memcpy(inv_tenkidata.Sn_2F8C, (pS + 11)->KKval2, 6);
	memcpy(inv_tenkidata.Sn_2F8B, (pS + 10)->KKval2, 6);

	memcpy(inv_tenkidata.Sn_2F9D, (pS + 12)->KKval1, 6);
	memcpy(inv_tenkidata.Sn_2F9C, (pS + 11)->KKval1, 6);
	memcpy(inv_tenkidata.Sn_2F9B, (pS + 10)->KKval1, 6);

//----->('23.10.20 tanaka Add)
	memcpy(inv_tenkidata.Sn_2F11C , (pS + 13)->KKval2 , 6);
	memcpy(inv_tenkidata.Sn_2F12C , (pS + 13)->KKval1 , 6);
	memcpy(inv_tenkidata.Sn_2F11D , (pS + 14)->KKval2 , 6);
	memcpy(inv_tenkidata.Sn_2F12D , (pS + 14)->KKval1 , 6);
//<-----

	return 0;

}